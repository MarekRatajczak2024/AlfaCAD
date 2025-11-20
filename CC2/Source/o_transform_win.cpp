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

#ifdef LINUX
#include <QTransform>
#else
#include <qtransform.h>
//#include <QPolygonF>
#include "qpolygon.h"
//#include <QPointF>
#include "qpoint.h"
#endif

typedef struct
{
  float xy[8] ;
} QUAD;

typedef struct
{
    float x;
    float y;
} POINTF;

QTransform trans;

#ifdef __cplusplus
extern "C" {
#endif
int quadtoquadmatrix(QUAD one, QUAD two);
POINTF map_point(float Px, float Py);

#ifdef __cplusplus
}
#endif

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
