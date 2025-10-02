/*   ______   ___       ___         ____     ______  ____
*   /\  _  \ /\_ \    /'___\       /\  _`\  /\  _  \/\  _`\
*   \ \ \L\ \\//\ \  /\ \__/   __  \ \ \/\_\\ \ \L\ \ \ \/\ \
*    \ \  __ \ \ \ \ \ \ ,__\/'__`\ \ \ \/_/_\ \  __ \ \ \ \ \
*     \ \ \/\ \ \_\ \_\ \ \_/\ \L\.\_\ \ \L\ \\ \ \/\ \ \ \_\ \
*      \ \_\ \_\/\____\\ \_\\ \__/.\_\\ \____/ \ \_\ \_\ \____/
*       \/_/\/_/\/____/ \/_/ \/__/\/_/ \/___/   \/_/\/_/\/___/
*
*   A CAD
*
*   By Marek Ratajczak 2024
*
*   See readme_alfacad.txt for copyright information.
*
*/
#ifndef BIT64  //LINUX
#include "o_transform_win.cpp"
#else
#ifdef LINUX
#include <QTransform>
#else
#include "qtransform.h"
//#include <QPolygonF>
#include "qpolygon.h"
//#include <QPointF>
#include "qpoint.h"
#endif

#include "qtools_p.h"
#include <qalloc.h>
#include <QtCore/qarraydata.h>
#include <qmath.h>

typedef struct
{
  float xy[8] ;
} QUAD;

//#ifdef LINUX
typedef struct
{
    float x;
    float y;
} POINTF;
//#endif

QTransform trans;

#ifdef __cplusplus
extern "C" {
#endif
int quadtoquadmatrix(QUAD one, QUAD two);
POINTF map_point(float Px, float Py);

#ifdef __cplusplus
}
#endif



qsizetype qCalculateBlockSize(qsizetype elementCount, qsizetype elementSize, qsizetype headerSize) noexcept
{
    Q_ASSERT(elementSize);

    size_t bytes;
    if (Q_UNLIKELY(qMulOverflow(size_t(elementSize), size_t(elementCount), &bytes)) ||
        Q_UNLIKELY(qAddOverflow(bytes, size_t(headerSize), &bytes)))
        return -1;
    if (Q_UNLIKELY(qsizetype(bytes) < 0))
        return -1;

    return qsizetype(bytes);
}

size_t QtPrivate::expectedAllocSize(size_t allocSize, size_t alignment) noexcept
{
    Q_ASSERT(qPopulationCount(alignment) == 1);
////#if QT_CONFIG(jemalloc)  //by MAREK
 ////   return ::nallocx(allocSize, MALLOCX_ALIGN(alignment));
////#endif
    Q_UNUSED(allocSize);
    Q_UNUSED(alignment);
    return 0;
}

CalculateGrowingBlockSizeResult
qCalculateGrowingBlockSize(qsizetype elementCount, qsizetype elementSize, qsizetype headerSize) noexcept
{
    CalculateGrowingBlockSizeResult result = {
            qsizetype(-1), qsizetype(-1)
    };

    qsizetype bytes = qCalculateBlockSize(elementCount, elementSize, headerSize);
    if (bytes < 0)
        return result;

    size_t morebytes = static_cast<size_t>(qNextPowerOfTwo(quint64(bytes)));
    if (Q_UNLIKELY(qsizetype(morebytes) < 0)) {
        // grow by half the difference between bytes and morebytes
        // this slows the growth and avoids trying to allocate exactly
        // 2G of memory (on 32bit), something that many OSes can't deliver
        bytes += (morebytes - bytes) / 2;
    } else {
        bytes = qsizetype(morebytes);
    }
    size_t fittedBytes = QtPrivate::expectedAllocSize(bytes, alignof(std::max_align_t));
    if (fittedBytes != 0)
        bytes = fittedBytes;

    result.elementCount = (bytes - headerSize) / elementSize;
    result.size = result.elementCount * elementSize + headerSize;
    return result;
}


using QtPrivate::AlignedQArrayData;

static qsizetype calculateHeaderSize(qsizetype alignment)
{
    qsizetype headerSize = sizeof(AlignedQArrayData);
    const qsizetype headerAlignment = alignof(AlignedQArrayData);

    if (alignment > headerAlignment) {
        // Allocate extra (alignment - Q_ALIGNOF(AlignedQArrayData)) padding
        // bytes so we can properly align the data array. This assumes malloc is
        // able to provide appropriate alignment for the header -- as it should!
        // Effectively, we allocate one QTypedArrayData<T>::AlignmentDummy.
        headerSize += alignment - headerAlignment;
    }
    Q_ASSERT(headerSize > 0);

    return headerSize;
}

static inline CalculateGrowingBlockSizeResult
calculateBlockSize(qsizetype capacity, qsizetype objectSize, qsizetype headerSize, QArrayData::AllocationOption option)
{
    // Adjust the header size up to account for the trailing null for QString
    // and QByteArray. This is not checked for overflow because headers sizes
    // should not be anywhere near the overflow limit.
    constexpr qsizetype FooterSize = qMax(sizeof(QString::value_type), sizeof(QByteArray::value_type));
    if (objectSize <= FooterSize)
        headerSize += FooterSize;

    // allocSize = objectSize * capacity + headerSize, but checked for overflow
    // plus padded to grow in size
    if (option == QArrayData::Grow) {
        return qCalculateGrowingBlockSize(capacity, objectSize, headerSize);
    } else {
        return { qCalculateBlockSize(capacity, objectSize, headerSize), capacity };
    }
}

namespace {
    struct AllocationResult {
        void *data;
        QArrayData *header;
    };
}

static inline AllocationResult
allocateHelper(qsizetype objectSize, qsizetype alignment, qsizetype capacity,
               QArrayData::AllocationOption option) noexcept
{
    if (capacity == 0)
        return {};

    const qsizetype headerSize = calculateHeaderSize(alignment);
    Q_ASSERT(headerSize > 0);

    auto blockSize = calculateBlockSize(capacity, objectSize, headerSize, option);
    capacity = blockSize.elementCount;
    qsizetype allocSize = blockSize.size;
    if (Q_UNLIKELY(allocSize < 0))      // handle overflow. cannot allocate reliably
        return {};

    void *data = nullptr;
    QArrayData *header = static_cast<QArrayData *>(::malloc(size_t(allocSize)));
    if (Q_LIKELY(header)) {
        header->ref_.storeRelaxed(1);
        header->flags = {};
        // find where offset should point to so that data() is aligned to alignment bytes
        data = QTypedArrayData<void>::dataStart(header, alignment);
        header->alloc = capacity;
    }

    return { data, header };
}


// Generic size and alignment allocation function
void *QArrayData::allocate(QArrayData **dptr, qsizetype objectSize, qsizetype alignment,
                           qsizetype capacity, AllocationOption option) noexcept
{
    Q_ASSERT(dptr);
    // Alignment is a power of two
    Q_ASSERT(alignment >= qsizetype(alignof(QArrayData))
             && !(alignment & (alignment - 1)));

    auto r = allocateHelper(objectSize, alignment, capacity, option);
    *dptr = r.header;
    return r.data;
}

std::pair<QArrayData *, void *>
QArrayData::reallocateUnaligned(QArrayData *data, void *dataPointer,
                                qsizetype objectSize, qsizetype capacity, AllocationOption option) noexcept
{
    Q_ASSERT(!data || !data->isShared());

    const qsizetype headerSize = sizeof(AlignedQArrayData);
    auto r = calculateBlockSize(capacity, objectSize, headerSize, option);
    qsizetype allocSize = r.size;
    capacity = r.elementCount;
    if (Q_UNLIKELY(allocSize < 0))
        return {};

    const qptrdiff offset = dataPointer
                            ? reinterpret_cast<char *>(dataPointer) - reinterpret_cast<char *>(data)
                            : headerSize;
    Q_ASSERT(offset > 0);
    Q_ASSERT(offset <= allocSize); // equals when all free space is at the beginning

    QArrayData *header = static_cast<QArrayData *>(::realloc(data, size_t(allocSize)));
    if (header) {
        header->alloc = capacity;
        dataPointer = reinterpret_cast<char *>(header) + offset;
    } else {
        dataPointer = nullptr;
    }
    return {header, dataPointer};
}


void QPolygon::setPoint(int index, int x, int y)
{
    (*this)[index] = QPoint(x, y);
}


/*!
    Resizes the polygon to \a nPoints and populates it with the given
    \a points.

    The example code creates a polygon with two points (10, 20) and
    (30, 40):

    \snippet polygon/polygon.cpp 2

    \sa setPoint(), putPoints()
*/

void QPolygon::setPoints(int nPoints, const int *points)
{
    resize(nPoints);
    int i = 0;
    while (nPoints--) {
        setPoint(i++, *points, *(points+1));
        points += 2;
    }
}

/*!
    \overload

    Resizes the polygon to \a nPoints and populates it with the points
    specified by the variable argument list.  The points are given as a
    sequence of integers, starting with \a firstx then \a firsty, and
    so on.

    The example code creates a polygon with two points (10, 20) and
    (30, 40):

    \snippet polygon/polygon.cpp 3
*/

void QPolygon::setPoints(int nPoints, int firstx, int firsty, ...)
{
    va_list ap;
    resize(nPoints);
    setPoint(0, firstx, firsty);
    int i = 0, x, y;
    va_start(ap, firsty);
    while (--nPoints) {
        x = va_arg(ap, int);
        y = va_arg(ap, int);
        setPoint(++i, x, y);
    }
    va_end(ap);
}


/*!
    \fn QPolygonF::QPolygonF(const QPolygon &polygon)

    Constructs a float based polygon from the specified integer based
    \a polygon.

    \sa toPolygon()
*/

QPolygonF::QPolygonF(const QPolygon &a)
{
    reserve(a.size());
    for (int i=0; i<a.size(); ++i)
        append(a.at(i));
}

/*!
    Translate all points in the polygon by the given \a offset.

    \sa translated()
*/

void QPolygonF::translate(const QPointF &offset)
{
    if (offset.isNull())
        return;

    QPointF *p = data();
    int i = size();
    while (i--) {
        *p += offset;
        ++p;
    }
}

/*!
    \fn void QPolygonF::translate(qreal dx, qreal dy)
    \overload

    Translates all points in the polygon by (\a{dx}, \a{dy}).

    \sa translated()
*/

/*!
    Returns a copy of the polygon that is translated by the given \a offset.

    \since 4.6
    \sa translate()
*/
QPolygonF QPolygonF::translated(const QPointF &offset) const
{
    QPolygonF copy(*this);
    copy.translate(offset);
    return copy;
}


QPolygon::QPolygon(int nPoints, const int *points)
{
    setPoints(nPoints, points);
}

QPolygon QPolygon::translated(int dx, int dy) const
{
    QPolygon copy(*this);
    copy.translate(dx, dy);
    return copy;
}

void QPolygon::translate(int dx, int dy)
{
    if (dx == 0 && dy == 0)
        return;

    QPoint *p = data();
    int i = size();
    QPoint pt(dx, dy);
    while (i--) {
        *p += pt;
        ++p;
    }
}

#define Q_NEAR_CLIP (sizeof(qreal) == sizeof(double) ? 0.000001 : 0.0001)

void QTransform::do_map(qreal x, qreal y, qreal &nx, qreal &ny) const
{
    const TransformationType t = inline_type();
    switch (t) {
    case QTransform::TxNone:
        nx = x;
        ny = y;
        return;
    case QTransform::TxTranslate:
        nx = x + m_matrix[2][0];
        ny = y + m_matrix[2][1];
        return;
    case QTransform::TxScale:
        nx = m_matrix[0][0] * x + m_matrix[2][0];
        ny = m_matrix[1][1] * y + m_matrix[2][1];
        return;
    case QTransform::TxRotate:
    case QTransform::TxShear:
    case QTransform::TxProject:
        nx = m_matrix[0][0] * x + m_matrix[1][0] * y + m_matrix[2][0];
        ny = m_matrix[0][1] * x + m_matrix[1][1] * y + m_matrix[2][1];
        if (t == QTransform::TxProject) {
            qreal w = (m_matrix[0][2] * x + m_matrix[1][2] * y + m_matrix[2][2]);
            if (w < qreal(Q_NEAR_CLIP)) w = qreal(Q_NEAR_CLIP);
            w = qreal(1.)/w;
            nx *= w;
            ny *= w;
        }
        return;
    }
    Q_UNREACHABLE_RETURN();
}

/*!
    Returns the adjoint of this matrix.
*/
QTransform QTransform::adjoint() const
{
    qreal h11, h12, h13,
          h21, h22, h23,
          h31, h32, h33;
    h11 = m_matrix[1][1] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][1];
    h21 = m_matrix[1][2] * m_matrix[2][0] - m_matrix[1][0] * m_matrix[2][2];
    h31 = m_matrix[1][0] * m_matrix[2][1] - m_matrix[1][1] * m_matrix[2][0];
    h12 = m_matrix[0][2] * m_matrix[2][1] - m_matrix[0][1] * m_matrix[2][2];
    h22 = m_matrix[0][0] * m_matrix[2][2] - m_matrix[0][2] * m_matrix[2][0];
    h32 = m_matrix[0][1] * m_matrix[2][0] - m_matrix[0][0] * m_matrix[2][1];
    h13 = m_matrix[0][1] * m_matrix[1][2] - m_matrix[0][2] * m_matrix[1][1];
    h23 = m_matrix[0][2] * m_matrix[1][0] - m_matrix[0][0] * m_matrix[1][2];
    h33 = m_matrix[0][0] * m_matrix[1][1] - m_matrix[0][1] * m_matrix[1][0];

    return QTransform(h11, h12, h13,
                      h21, h22, h23,
                      h31, h32, h33);
}


/*!
    \fn QPoint operator*(const QPoint &point, const QTransform &matrix)
    \relates QTransform

    This is the same as \a{matrix}.map(\a{point}).

    \sa QTransform::map()
*/
QPoint QTransform::map(const QPoint &p) const
{
    qreal fx = p.x();
    qreal fy = p.y();

    qreal x = 0, y = 0;

    do_map(fx, fy, x, y);

    return QPoint(qRound(x), qRound(y));
}


/*!
  Returns the transformation type of this matrix.

  The transformation type is the highest enumeration value
  capturing all of the matrix's transformations. For example,
  if the matrix both scales and shears, the type would be \c TxShear,
  because \c TxShear has a higher enumeration value than \c TxScale.

  Knowing the transformation type of a matrix is useful for optimization:
  you can often handle specific types more optimally than handling
  the generic case.
  */
QTransform::TransformationType QTransform::type() const
{
    if (m_dirty == TxNone || m_dirty < m_type)
        return static_cast<TransformationType>(m_type);

    switch (static_cast<TransformationType>(m_dirty)) {
    case TxProject:
        if (!qFuzzyIsNull(m_matrix[0][2]) || !qFuzzyIsNull(m_matrix[1][2]) || !qFuzzyIsNull(m_matrix[2][2] - 1)) {
             m_type = TxProject;
             break;
        }
        Q_FALLTHROUGH();
    case TxShear:
    case TxRotate:
        if (!qFuzzyIsNull(m_matrix[0][1]) || !qFuzzyIsNull(m_matrix[1][0])) {
            const qreal dot = m_matrix[0][0] * m_matrix[1][0] + m_matrix[0][1] * m_matrix[1][1];
            if (qFuzzyIsNull(dot))
                m_type = TxRotate;
            else
                m_type = TxShear;
            break;
        }
        Q_FALLTHROUGH();
    case TxScale:
        if (!qFuzzyIsNull(m_matrix[0][0] - 1) || !qFuzzyIsNull(m_matrix[1][1] - 1)) {
            m_type = TxScale;
            break;
        }
        Q_FALLTHROUGH();
    case TxTranslate:
        if (!qFuzzyIsNull(m_matrix[2][0]) || !qFuzzyIsNull(m_matrix[2][1])) {
            m_type = TxTranslate;
            break;
        }
        Q_FALLTHROUGH();
    case TxNone:
        m_type = TxNone;
        break;
    }

    m_dirty = TxNone;
    return static_cast<TransformationType>(m_type);
}


/*!
    \fn QPointF operator*(const QPointF &point, const QTransform &matrix)
    \relates QTransform

    Same as \a{matrix}.map(\a{point}).

    \sa QTransform::map()
*/

/*!
    \overload

    Creates and returns a QPointF object that is a copy of the given point,
    \a p, mapped into the coordinate system defined by this matrix.
*/
QPointF QTransform::map(const QPointF &p) const
{
    qreal fx = p.x();
    qreal fy = p.y();

    qreal x = 0, y = 0;

    do_map(fx, fy, x, y);

    return QPointF(x, y);
}

/*!
    \fn QPoint QTransform::map(const QPoint &point) const
    \overload

    Creates and returns a QPoint object that is a copy of the given \a
    point, mapped into the coordinate system defined by this
    matrix. Note that the transformed coordinates are rounded to the
    nearest integer.
*/

/*!
    \fn QLineF operator*(const QLineF &line, const QTransform &matrix)
    \relates QTransform

    This is the same as \a{matrix}.map(\a{line}).

    \sa QTransform::map()
*/

/*!
    \fn QLine operator*(const QLine &line, const QTransform &matrix)
    \relates QTransform

    This is the same as \a{matrix}.map(\a{line}).

    \sa QTransform::map()
*/

/*!
    \overload

    Creates and returns a QLineF object that is a copy of the given line,
    \a l, mapped into the coordinate system defined by this matrix.
*/
QLine QTransform::map(const QLine &l) const
{
    qreal fx1 = l.x1();
    qreal fy1 = l.y1();
    qreal fx2 = l.x2();
    qreal fy2 = l.y2();

    qreal x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    do_map(fx1, fy1, x1, y1);
    do_map(fx2, fy2, x2, y2);

    return QLine(qRound(x1), qRound(y1), qRound(x2), qRound(y2));
}

/*!
    \overload

    \fn QLineF QTransform::map(const QLineF &line) const

    Creates and returns a QLine object that is a copy of the given \a
    line, mapped into the coordinate system defined by this matrix.
    Note that the transformed coordinates are rounded to the nearest
    integer.
*/

QLineF QTransform::map(const QLineF &l) const
{
    qreal fx1 = l.x1();
    qreal fy1 = l.y1();
    qreal fx2 = l.x2();
    qreal fy2 = l.y2();

    qreal x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    do_map(fx1, fy1, x1, y1);
    do_map(fx2, fy2, x2, y2);

    return QLineF(x1, y1, x2, y2);
}

/*!
    \fn QPolygonF operator *(const QPolygonF &polygon, const QTransform &matrix)
    \since 4.3
    \relates QTransform

    This is the same as \a{matrix}.map(\a{polygon}).

    \sa QTransform::map()
*/

/*!
    \fn QPolygon operator*(const QPolygon &polygon, const QTransform &matrix)
    \relates QTransform

    This is the same as \a{matrix}.map(\a{polygon}).

    \sa QTransform::map()
*/

/*!
    \fn QPolygonF QTransform::map(const QPolygonF &polygon) const
    \overload

    Creates and returns a QPolygonF object that is a copy of the given
    \a polygon, mapped into the coordinate system defined by this
    matrix.
*/
QPolygonF QTransform::map(const QPolygonF &a) const
{
    TransformationType t = inline_type();
    ////if (t <= TxTranslate)
    ////    return a.translated(m_matrix[2][0], m_matrix[2][1]);  //TEMPORARY

    int size = a.size();
    int i;
    QPolygonF p(size);
    const QPointF *da = a.constData();
    QPointF *dp = p.data();

    for(i = 0; i < size; ++i) {
        do_map(da[i].x(), da[i].y(), dp[i].rx(), dp[i].ry());
    }
    return p;
}

/*!
    \fn QPolygon QTransform::map(const QPolygon &polygon) const
    \overload

    Creates and returns a QPolygon object that is a copy of the given
    \a polygon, mapped into the coordinate system defined by this
    matrix. Note that the transformed coordinates are rounded to the
    nearest integer.
*/
QPolygon QTransform::map(const QPolygon &a) const
{
    TransformationType t = inline_type();
    if (t <= TxTranslate)
        return a.translated(qRound(m_matrix[2][0]), qRound(m_matrix[2][1]));

    int size = a.size();
    int i;
    QPolygon p(size);
    const QPoint *da = a.constData();
    QPoint *dp = p.data();

    for(i = 0; i < size; ++i) {
        qreal nx = 0, ny = 0;
        do_map(da[i].x(), da[i].y(), nx, ny);
        dp[i].rx() = qRound(nx);
        dp[i].ry() = qRound(ny);
    }
    return p;
}


/*!
    \fn bool QTransform::operator==(const QTransform &matrix) const
    Returns \c true if this matrix is equal to the given \a matrix,
    otherwise returns \c false.
*/
bool QTransform::operator==(const QTransform &o) const
{
    return m_matrix[0][0] == o.m_matrix[0][0] &&
           m_matrix[0][1] == o.m_matrix[0][1] &&
           m_matrix[1][0] == o.m_matrix[1][0] &&
           m_matrix[1][1] == o.m_matrix[1][1] &&
           m_matrix[2][0] == o.m_matrix[2][0] &&
           m_matrix[2][1] == o.m_matrix[2][1] &&
           m_matrix[0][2] == o.m_matrix[0][2] &&
           m_matrix[1][2] == o.m_matrix[1][2] &&
           m_matrix[2][2] == o.m_matrix[2][2];
}

/*!
    \since 5.6
    \qhashold{QTransform}
*/
/*
size_t qHash(const QTransform &key, size_t seed) noexcept
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, key.m11());
    seed = hash(seed, key.m12());
    seed = hash(seed, key.m21());
    seed = hash(seed, key.m22());
    seed = hash(seed, key.dx());
    seed = hash(seed, key.dy());
    seed = hash(seed, key.m13());
    seed = hash(seed, key.m23());
    seed = hash(seed, key.m33());
    return seed;
}
*/

/*!
    \fn bool QTransform::operator!=(const QTransform &matrix) const
    Returns \c true if this matrix is not equal to the given \a matrix,
    otherwise returns \c false.
*/
bool QTransform::operator!=(const QTransform &o) const
{
    return !operator==(o);
}

/*!
    \fn QTransform & QTransform::operator*=(const QTransform &matrix)
    \overload

    Returns the result of multiplying this matrix by the given \a
    matrix.
*/
QTransform & QTransform::operator*=(const QTransform &o)
{
    const TransformationType otherType = o.inline_type();
    if (otherType == TxNone)
        return *this;

    const TransformationType thisType = inline_type();
    if (thisType == TxNone)
        return operator=(o);

    TransformationType t = qMax(thisType, otherType);
    switch(t) {
    case TxNone:
        break;
    case TxTranslate:
        m_matrix[2][0] += o.m_matrix[2][0];
        m_matrix[2][1] += o.m_matrix[2][1];
        break;
    case TxScale:
    {
        qreal m11 = m_matrix[0][0] * o.m_matrix[0][0];
        qreal m22 = m_matrix[1][1] * o.m_matrix[1][1];

        qreal m31 = m_matrix[2][0] * o.m_matrix[0][0] + o.m_matrix[2][0];
        qreal m32 = m_matrix[2][1] * o.m_matrix[1][1] + o.m_matrix[2][1];

        m_matrix[0][0] = m11;
        m_matrix[1][1] = m22;
        m_matrix[2][0] = m31; m_matrix[2][1] = m32;
        break;
    }
    case TxRotate:
    case TxShear:
    {
        qreal m11 = m_matrix[0][0] * o.m_matrix[0][0] + m_matrix[0][1] * o.m_matrix[1][0];
        qreal m12 = m_matrix[0][0] * o.m_matrix[0][1] + m_matrix[0][1] * o.m_matrix[1][1];

        qreal m21 = m_matrix[1][0] * o.m_matrix[0][0] + m_matrix[1][1] * o.m_matrix[1][0];
        qreal m22 = m_matrix[1][0] * o.m_matrix[0][1] + m_matrix[1][1] * o.m_matrix[1][1];

        qreal m31 = m_matrix[2][0] * o.m_matrix[0][0] + m_matrix[2][1] * o.m_matrix[1][0] + o.m_matrix[2][0];
        qreal m32 = m_matrix[2][0] * o.m_matrix[0][1] + m_matrix[2][1] * o.m_matrix[1][1] + o.m_matrix[2][1];

        m_matrix[0][0] = m11;
        m_matrix[0][1] = m12;
        m_matrix[1][0] = m21;
        m_matrix[1][1] = m22;
        m_matrix[2][0] = m31;
        m_matrix[2][1] = m32;
        break;
    }
    case TxProject:
    {
        qreal m11 = m_matrix[0][0] * o.m_matrix[0][0] + m_matrix[0][1] * o.m_matrix[1][0] + m_matrix[0][2] * o.m_matrix[2][0];
        qreal m12 = m_matrix[0][0] * o.m_matrix[0][1] + m_matrix[0][1] * o.m_matrix[1][1] + m_matrix[0][2] * o.m_matrix[2][1];
        qreal m13 = m_matrix[0][0] * o.m_matrix[0][2] + m_matrix[0][1] * o.m_matrix[1][2] + m_matrix[0][2] * o.m_matrix[2][2];

        qreal m21 = m_matrix[1][0] * o.m_matrix[0][0] + m_matrix[1][1] * o.m_matrix[1][0] + m_matrix[1][2] * o.m_matrix[2][0];
        qreal m22 = m_matrix[1][0] * o.m_matrix[0][1] + m_matrix[1][1] * o.m_matrix[1][1] + m_matrix[1][2] * o.m_matrix[2][1];
        qreal m23 = m_matrix[1][0] * o.m_matrix[0][2] + m_matrix[1][1] * o.m_matrix[1][2] + m_matrix[1][2] * o.m_matrix[2][2];

        qreal m31 = m_matrix[2][0] * o.m_matrix[0][0] + m_matrix[2][1] * o.m_matrix[1][0] + m_matrix[2][2] * o.m_matrix[2][0];
        qreal m32 = m_matrix[2][0] * o.m_matrix[0][1] + m_matrix[2][1] * o.m_matrix[1][1] + m_matrix[2][2] * o.m_matrix[2][1];
        qreal m33 = m_matrix[2][0] * o.m_matrix[0][2] + m_matrix[2][1] * o.m_matrix[1][2] + m_matrix[2][2] * o.m_matrix[2][2];

        m_matrix[0][0] = m11; m_matrix[0][1] = m12; m_matrix[0][2] = m13;
        m_matrix[1][0] = m21; m_matrix[1][1] = m22; m_matrix[1][2] = m23;
        m_matrix[2][0] = m31; m_matrix[2][1] = m32; m_matrix[2][2] = m33;
    }
    }

    m_dirty = t;
    m_type = t;

    return *this;
}

/*!
    \fn QTransform QTransform::operator*(const QTransform &matrix) const
    Returns the result of multiplying this matrix by the given \a
    matrix.

    Note that matrix multiplication is not commutative, i.e. a*b !=
    b*a.
*/
QTransform QTransform::operator*(const QTransform &m) const
{
    const TransformationType otherType = m.inline_type();
    if (otherType == TxNone)
        return *this;

    const TransformationType thisType = inline_type();
    if (thisType == TxNone)
        return m;

    QTransform t;
    TransformationType type = qMax(thisType, otherType);
    switch(type) {
    case TxNone:
        break;
    case TxTranslate:
        t.m_matrix[2][0] = m_matrix[2][0] + m.m_matrix[2][0];
        t.m_matrix[2][1] = m_matrix[2][1] + m.m_matrix[2][1];
        break;
    case TxScale:
    {
        qreal m11 = m_matrix[0][0] * m.m_matrix[0][0];
        qreal m22 = m_matrix[1][1] * m.m_matrix[1][1];

        qreal m31 = m_matrix[2][0] * m.m_matrix[0][0] + m.m_matrix[2][0];
        qreal m32 = m_matrix[2][1] * m.m_matrix[1][1] + m.m_matrix[2][1];

        t.m_matrix[0][0] = m11;
        t.m_matrix[1][1] = m22;
        t.m_matrix[2][0] = m31;
        t.m_matrix[2][1] = m32;
        break;
    }
    case TxRotate:
    case TxShear:
    {
        qreal m11 = m_matrix[0][0] * m.m_matrix[0][0] + m_matrix[0][1] * m.m_matrix[1][0];
        qreal m12 = m_matrix[0][0] * m.m_matrix[0][1] + m_matrix[0][1] * m.m_matrix[1][1];

        qreal m21 = m_matrix[1][0] * m.m_matrix[0][0] + m_matrix[1][1] * m.m_matrix[1][0];
        qreal m22 = m_matrix[1][0] * m.m_matrix[0][1] + m_matrix[1][1] * m.m_matrix[1][1];

        qreal m31 = m_matrix[2][0] * m.m_matrix[0][0] + m_matrix[2][1] * m.m_matrix[1][0] + m.m_matrix[2][0];
        qreal m32 = m_matrix[2][0] * m.m_matrix[0][1] + m_matrix[2][1] * m.m_matrix[1][1] + m.m_matrix[2][1];

        t.m_matrix[0][0] = m11; t.m_matrix[0][1] = m12;
        t.m_matrix[1][0] = m21; t.m_matrix[1][1] = m22;
        t.m_matrix[2][0] = m31; t.m_matrix[2][1] = m32;
        break;
    }
    case TxProject:
    {
        qreal m11 = m_matrix[0][0] * m.m_matrix[0][0] + m_matrix[0][1] * m.m_matrix[1][0] + m_matrix[0][2] * m.m_matrix[2][0];
        qreal m12 = m_matrix[0][0] * m.m_matrix[0][1] + m_matrix[0][1] * m.m_matrix[1][1] + m_matrix[0][2] * m.m_matrix[2][1];
        qreal m13 = m_matrix[0][0] * m.m_matrix[0][2] + m_matrix[0][1] * m.m_matrix[1][2] + m_matrix[0][2] * m.m_matrix[2][2];

        qreal m21 = m_matrix[1][0] * m.m_matrix[0][0] + m_matrix[1][1] * m.m_matrix[1][0] + m_matrix[1][2] * m.m_matrix[2][0];
        qreal m22 = m_matrix[1][0] * m.m_matrix[0][1] + m_matrix[1][1] * m.m_matrix[1][1] + m_matrix[1][2] * m.m_matrix[2][1];
        qreal m23 = m_matrix[1][0] * m.m_matrix[0][2] + m_matrix[1][1] * m.m_matrix[1][2] + m_matrix[1][2] * m.m_matrix[2][2];

        qreal m31 = m_matrix[2][0] * m.m_matrix[0][0] + m_matrix[2][1] * m.m_matrix[1][0] + m_matrix[2][2] * m.m_matrix[2][0];
        qreal m32 = m_matrix[2][0] * m.m_matrix[0][1] + m_matrix[2][1] * m.m_matrix[1][1] + m_matrix[2][2] * m.m_matrix[2][1];
        qreal m33 = m_matrix[2][0] * m.m_matrix[0][2] + m_matrix[2][1] * m.m_matrix[1][2] + m_matrix[2][2] * m.m_matrix[2][2];

        t.m_matrix[0][0] = m11; t.m_matrix[0][1] = m12; t.m_matrix[0][2] = m13;
        t.m_matrix[1][0] = m21; t.m_matrix[1][1] = m22; t.m_matrix[1][2] = m23;
        t.m_matrix[2][0] = m31; t.m_matrix[2][1] = m32; t.m_matrix[2][2] = m33;
    }
    }

    t.m_dirty = type;
    t.m_type = type;

    return t;
}

/*!
    \fn QTransform & QTransform::operator*=(qreal scalar)
    \overload

    Returns the result of performing an element-wise multiplication of this
    matrix with the given \a scalar.
*/

/*!
    \fn QTransform & QTransform::operator/=(qreal scalar)
    \overload

    Returns the result of performing an element-wise division of this
    matrix by the given \a scalar.
*/

/*!
    \fn QTransform & QTransform::operator+=(qreal scalar)
    \overload

    Returns the matrix obtained by adding the given \a scalar to each
    element of this matrix.
*/

/*!
    \fn QTransform & QTransform::operator-=(qreal scalar)
    \overload

    Returns the matrix obtained by subtracting the given \a scalar from each
    element of this matrix.
*/

/*!
    \fn QTransform &QTransform::operator=(const QTransform &matrix) noexcept

    Assigns the given \a matrix's values to this matrix.
*/


/*!
    Returns an inverted copy of this matrix.

    If the matrix is singular (not invertible), the returned matrix is
    the identity matrix. If \a invertible is valid (i.e. not 0), its
    value is set to true if the matrix is invertible, otherwise it is
    set to false.

    \sa isInvertible()
*/
QTransform QTransform::inverted(bool *invertible) const
{
    QTransform invert;
    bool inv = true;

    switch(inline_type()) {
    case TxNone:
        break;
    case TxTranslate:
        invert.m_matrix[2][0] = -m_matrix[2][0];
        invert.m_matrix[2][1] = -m_matrix[2][1];
        break;
    case TxScale:
        inv = !qFuzzyIsNull(m_matrix[0][0]);
        inv &= !qFuzzyIsNull(m_matrix[1][1]);
        if (inv) {
            invert.m_matrix[0][0] = 1. / m_matrix[0][0];
            invert.m_matrix[1][1] = 1. / m_matrix[1][1];
            invert.m_matrix[2][0] = -m_matrix[2][0] * invert.m_matrix[0][0];
            invert.m_matrix[2][1] = -m_matrix[2][1] * invert.m_matrix[1][1];
        }
        break;
    default:
        // general case
        qreal det = determinant();
        inv = !qFuzzyIsNull(det);
        if (inv)
            invert = adjoint() / det;
        break;
    }

    if (invertible)
        *invertible = inv;

    if (inv) {
        // inverting doesn't change the type
        invert.m_type = m_type;
        invert.m_dirty = m_dirty;
    }

    return invert;
}


/*!
    Creates a transformation matrix, \a trans, that maps a unit square
    to a four-sided polygon, \a quad. Returns \c true if the transformation
    is constructed or false if such a transformation does not exist.

    \sa quadToSquare(), quadToQuad()
*/
bool QTransform::squareToQuad(const QPolygonF &quad, QTransform &trans)
{
    if (quad.size() != (quad.isClosed() ? 5 : 4))
        return false;

    qreal dx0 = quad[0].x();
    qreal dx1 = quad[1].x();
    qreal dx2 = quad[2].x();
    qreal dx3 = quad[3].x();

    qreal dy0 = quad[0].y();
    qreal dy1 = quad[1].y();
    qreal dy2 = quad[2].y();
    qreal dy3 = quad[3].y();

    double ax  = dx0 - dx1 + dx2 - dx3;
    double ay  = dy0 - dy1 + dy2 - dy3;

    if (!ax && !ay) { //afine transform
        trans.setMatrix(dx1 - dx0, dy1 - dy0,  0,
                        dx2 - dx1, dy2 - dy1,  0,
                        dx0,       dy0,  1);
    } else {
        double ax1 = dx1 - dx2;
        double ax2 = dx3 - dx2;
        double ay1 = dy1 - dy2;
        double ay2 = dy3 - dy2;

        /*determinants */
        double gtop    =  ax  * ay2 - ax2 * ay;
        double htop    =  ax1 * ay  - ax  * ay1;
        double bottom  =  ax1 * ay2 - ax2 * ay1;

        double a, b, c, d, e, f, g, h;  /*i is always 1*/

        if (!bottom)
            return false;

        g = gtop/bottom;
        h = htop/bottom;

        a = dx1 - dx0 + g * dx1;
        b = dx3 - dx0 + h * dx3;
        c = dx0;
        d = dy1 - dy0 + g * dy1;
        e = dy3 - dy0 + h * dy3;
        f = dy0;

        trans.setMatrix(a, d, g,
                        b, e, h,
                        c, f, 1.0);
    }

    return true;
}

/*!
    \fn bool QTransform::quadToSquare(const QPolygonF &quad, QTransform &trans)

    Creates a transformation matrix, \a trans, that maps a four-sided polygon,
    \a quad, to a unit square. Returns \c true if the transformation is constructed
    or false if such a transformation does not exist.

    \sa squareToQuad(), quadToQuad()
*/
bool QTransform::quadToSquare(const QPolygonF &quad, QTransform &trans)
{
    if (!squareToQuad(quad, trans))
        return false;

    bool invertible = false;
    trans = trans.inverted(&invertible);

    return invertible;
}

/*!
    Creates a transformation matrix, \a trans, that maps a four-sided
    polygon, \a one, to another four-sided polygon, \a two.
    Returns \c true if the transformation is possible; otherwise returns
    false.

    This is a convenience method combining quadToSquare() and
    squareToQuad() methods. It allows the input quad to be
    transformed into any other quad.

    \sa squareToQuad(), quadToSquare()
*/
bool QTransform::quadToQuad(const QPolygonF &one,
                            const QPolygonF &two,
                            QTransform &trans)
{
    QTransform stq;
    if (!quadToSquare(one, trans))
        return false;
    if (!squareToQuad(two, stq))
        return false;
    trans *= stq;
    //qDebug()<<"Final = "<<trans;
    return true;
}

/*!
    Sets the matrix elements to the specified values, \a m11,
    \a m12, \a m13 \a m21, \a m22, \a m23 \a m31, \a m32 and
    \a m33. Note that this function replaces the previous values.
    QTransform provides the translate(), rotate(), scale() and shear()
    convenience functions to manipulate the various matrix elements
    based on the currently defined coordinate system.

    \sa QTransform()
*/

void QTransform::setMatrix(qreal m11, qreal m12, qreal m13,
                           qreal m21, qreal m22, qreal m23,
                           qreal m31, qreal m32, qreal m33)
{
    m_matrix[0][0] = m11; m_matrix[0][1] = m12; m_matrix[0][2] = m13;
    m_matrix[1][0] = m21; m_matrix[1][1] = m22; m_matrix[1][2] = m23;
    m_matrix[2][0] = m31; m_matrix[2][1] = m32; m_matrix[2][2] = m33;
    m_type = TxNone;
    m_dirty = TxProject;
}

int quadtoquadmatrix(QUAD one, QUAD two)
{
    QPolygonF qone, qtwo;
    qone.resize(4);
    qtwo.resize(4);
    for (int i=0; i<4; i++) {
        (qone)[i] = QPointF(one.xy[i*2], one.xy[i*2+1]);
        (qtwo)[i] = QPointF(two.xy[i*2], two.xy[i*2+1]);
    }

    return QTransform::quadToQuad(qone, qtwo, trans);
}

POINTF map_point(float Px, float Py)
{
    QPointF p, p0;
    static POINTF p1;
    p0.setX(Px);
    p0.setY(Py);
    p=trans.map(p0);
    p1.x=p.x();
    p1.y=p.y();
    return p1;
}
#endif