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

#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<math.h>
#include<stddef.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "o_object.h"
#include "bib_arc.h"
#include "o_ellip1.h"

extern void my_fillpoly(int numpoints, AL_CONST int* polypoints, int translucency, int kolory_paper);
extern void fillpoly(int numpoints, AL_CONST int* polypoints);
extern void setfillstyle_(int pattern, int color);
extern void set_mode_solid(void);
extern void set_mode_trans(void);
extern void setcolor(int kolor);
extern int TRANSLUCENCY;
extern void set_pattern_count(BOOL p_count);
extern void set_pattern_offset(int offs);

static int i__ell_count ;
static int i__Ell_Verno = 60; //120 is too many for hatching with color or image
static int i__Ell_Angle = 0 ;

static BOOL elipse_seg (double X0,double Y0,double X1,double Y1,double X2,double Y2,int i_mode,BLOK **ptrptr_adb,void *ptr_ob,int i_type,int obiektt2,
            int *x1, int *y1, int *x2, int *y2, int type, int translucency);
BOOL elipse_seg_shadow(double X0, double Y0, double X1, double Y1, double X2, double Y2, int i_mode, BLOK *ptrptr_adb, void *ptr_ob, int i_type, int obiektt2);

/*  in python
from typing import Tuple
import numpy as np

def find_tangent_lines(
        center: Tuple[float, float],
semi_axes: Tuple[float, float],
rotation: float,
reference_point: Tuple[float, float],
):
"""Find the Ellipse's two tangents that go through a reference point.

Args:
center: The center of the ellipse.
semi_axes: The semi-major and semi-minor axes of the ellipse.
rotation: The counter-clockwise rotation of the ellipse in radians.
reference_point: The coordinates of the reference point.

Returns:
(m1, h1): Slope and intercept of the first tangent.
(m2, h2): Slope and intercept of the second tangent.
"""
x0, y0 = center
a, b = semi_axes
s, c = np.sin(rotation), np.cos(rotation)
p0, q0 = reference_point

A = (-a**2*s**2 - b**2*c**2 + (y0-q0)**2)
B = 2*(c*s*(a**2-b**2) - (x0-p0)*(y0-q0))
C = (-a**2*c**2 - b**2*s**2 + (x0-p0)**2)

if B**2 - 4*A*C < 0:
raise ValueError('Reference point lies inside the ellipse')

t1, t2 = (
        (-B + np.sqrt(B**2 - 4*A*C))/(2*A),
                (-B - np.sqrt(B**2 - 4*A*C))/(2*A),
)
return (
(1/t1, q0 - p0/t1),
(1/t2, q0 - p0/t2),
)
*/

BOOL find_tangent_lines_in_ellipse(double x00, double y00, double a, double b, double angle, double p00, double q00, double *m1, double *h1, double *m2, double *h2, double x[], double y[])
{
//Find the Ellipse's two tangents that go through a reference point.
//Returns:
//(m1, h1): Slope and intercept of the first tangent.
//(m2, h2): Slope and intercept of the second tangent.
//x[2], y[2] touch points

//x0, y0 = center
//a, b = semi_axes
//s, c = np.sin(rotation), np.cos(rotation)
//p0, q0 = reference_point

double si=sin(-angle);
double co=cos(-angle);
double x0=0.0;
double y0=0.0;

double p0=p00-x00;
double q0=q00-y00;

    Rotate_Point(si, co, 0,0, p0,q0, &p0, &q0);

    si=0.0;
    co=1.0;

double A = (-a*a*si*si - b*b*co*co + (y0-q0)*(y0-q0));
double B = 2*(co*si*(a*a-b*b) - (x0-p0)*(y0-q0));
double C = (-a*a*co*co - b*b*si*si + (x0-p0)*(x0-p0));

if ((B*B - 4*A*C) < 0) return FALSE;  //Reference point lies inside the ellipse

double t1= (-B + sqrt(B*B - 4*A*C))/(2*A);
double t2= (-B - sqrt(B*B - 4*A*C))/(2*A);

*m1=1/t1;
*h1=q0 - p0/t1;
*m2=1/t2;
*h2=q0 - p0/t2;

//touch points
double xx[4], yy[4];

 xx[0]=-a*a*(*m1)/sqrt(a*a*(*m1)*(*m1)+b*b);
 yy[0]=b*b/sqrt(a*a*(*m1)*(*m1)+b*b);
 xx[1]=a*a*(*m1)/sqrt(a*a*(*m1)*(*m1)+b*b);
 yy[1]=-b*b/sqrt(a*a*(*m1)*(*m1)+b*b);

 xx[2]=-a*a*(*m2)/sqrt(a*a*(*m2)*(*m2)+b*b);
 yy[2]=b*b/sqrt(a*a*(*m2)*(*m2)+b*b);
 xx[3]=a*a*(*m2)/sqrt(a*a*(*m2)*(*m2)+b*b);
 yy[3]=-b*b/sqrt(a*a*(*m2)*(*m2)+b*b);

 //checking points
 int j=0;
 if (Check_if_Equal3((*m1)*xx[0]+(*h1), yy[0])) {x[j]=xx[0]; y[j]=yy[0]; j++; }
 if (Check_if_Equal3((*m1)*xx[1]+(*h1), yy[1])) {x[j]=xx[1]; y[j]=yy[1]; j++; }
 if (j<2) {if (Check_if_Equal3((*m2)*xx[2]+(*h2), yy[2])) {x[j]=xx[2]; y[j]=yy[2]; j++; }}
 if (j<2) {if (Check_if_Equal3((*m2)*xx[3]+(*h2), yy[3])) {x[j]=xx[3]; y[j]=yy[3]; j++; }}

    si=sin(angle);
    co=cos(angle);

    Rotate_Point(si, co, 0, 0, x[0], y[0], &x[0], &y[0]);
    Rotate_Point(si, co, 0, 0, x[1], y[1], &x[1], &y[1]);

    x[0]+=x00;
    y[0]+=y00;
    x[1]+=x00;
    y[1]+=y00;

return TRUE;

}

BOOL check_if_point_on_ellipse(double cx, double cy, double a, double b, double angle,double px, double py)
{   double x, y;
    double si, co;
    //shifting point
    x=px-cx;
    y=py-cy;
    si=sin(-angle);
    co=cos(-angle);
    Rotate_Point(si, co, 0, 0, x, y, &x, &y);
    if (Check_if_Equal3(((x*x)/(a*a)+(y*y)/(b*b)), 1.0)) return TRUE;
    else return FALSE;
}

#define maxIter 512  //max is 1074

double GetRoot(double r0, double z0, double z1, double g)
{
    double n0 = r0 * z0;
    double s0 = z1 - 1;
    double s1 = (g < 0 ? 0 : sqrt(n0 * n0 + z1 * z1) - 1);
    double s = 0;
    for (int i = 0; i < maxIter; ++i) {
        s = (s0 + s1) / 2;
        if (s == s0 || s == s1) {
            break;
        }
        double ratio0 = n0 / (s + r0);
        double ratio1 = z1 / (s + 1);
        g = ratio0 * ratio0 + ratio1 * ratio1 - 1;
        if (g > 0) {
            s0 = s;
        } else if (g < 0) {
            s1 = s;
        } else {
            break;
        }
    }
    return s;
}

BOOL nearest_point_on_ellipse(double cx, double cy, double a, double b, double angle, double kat1, double kat2, double px, double py, double *xx1, double *yy1)
{
//Pseudocode for robustly computing the closest ellipse point and distance to a query point. It
//is required that e0 >= e1 > 0, y0 >= 0, and y1 >= 0.
//e0,e1 = ellipse dimension 0 and 1, where 0 is greater and both are positive.
//y0,y1 = initial point on ellipse axis (center of ellipse is 0,0)
//x0,x1 = intersection point
    double e0, e1, y0, y1, x0, x1;
    double distance;
    double kos, koc;
    double xx, yy;
    double anglep, anglep1, angle1, angle2;
    double XSel, YSel;
    double df_x1, df_y1;
    int quarters, fx, fy;

    if (a<b)
    {
        e0=b;
        e1=a;
        angle= Angle_Normal(angle+(Pi/2.0));
    } else
    {
        e0=a;
        e1=b;
    }

    xx=px-cx;
    yy=py-cy;

    kos=sin(-angle);
    koc=cos(-angle);

    Rotate_Point(kos, koc, 0, 0, xx, yy, &y0, &y1);

    //checking reference point, which must be always x>=0, y>=0
    quarters=((y0>=0) * 1) + ((y1>=0) * 2);

   ///   2 | 3
   ///   --|--
   ///   0 |1

    switch (quarters)
    {
        case 0:
            fx=-1;
            fy=-1;
            break;
        case 1:
            fx=1;
            fy=-1;
            break;
        case 2:
            fx=-1;
            fy=1;
            break;
        default:
            fx=1;
            fy=1;
            break;
    }

    y0=fabs(y0);
    y1=fabs(y1);

        if (y1 > 0) {
            if (y0 > 0) {
                double z0 = y0 / e0;
                double z1 = y1 / e1;
                double g = z0 * z0 + z1 * z1 - 1;
                if (g != 0) {
                    double r0 = (e0 / e1) * (e0 / e1);
                    double sbar = GetRoot(r0, z0, z1, g);
                    x0 = r0 * y0 / (sbar + r0);
                    x1 = y1 / (sbar + 1);
                    distance = sqrt((x0 - y0) * (x0 - y0) + (x1 - y1) * (x1 - y1));
                } else {
                    x0 = y0;
                    x1 = y1;
                    distance = 0;
                }
            } else // y0 == 0
            {
                x0 = 0;
                x1 = e1;
                distance = fabs(y1 - e1);
            }
        } else { // y1 == 0
            double numer0 = e0 * y0, denom0 = e0 * e0 - e1 * e1;
            if (numer0 < denom0) {
                double xde0 = numer0 / denom0;
                x0 = e0 * xde0;
                x1 = e1 * sqrt(1 - xde0 * xde0);
                distance = sqrt((x0 - y0) * (x0 - y0) + x1 * x1);
            } else {
                x0 = e0;
                x1 = 0;
                distance = fabs(y0 - e0);
            }
        }

    //////////////////
    df_x1 = x0 * fx;
    df_y1 = x1 * fy;

    XSel=X-cx;
    YSel=Y-cy;
    Rotate_Point(kos, koc, 0, 0, XSel, YSel, &XSel, &YSel);

    if ((Check_if_Equal(kat1, 0.0)==FALSE) || (Check_if_Equal(kat2, Pi2)==FALSE)) {
        //check if point on elliptical arc
        if (Check_if_Equal(df_x1, cx)) {
            if (df_y1 > cy) anglep1 = Pi / 2.0;
            else anglep1 = 3 * Pi / 2;
        } else {
            anglep1 = Atan2(df_y1, df_x1);
        }

        anglep1 = Angle_Normal(anglep1);

        angle1 = Angle_Normal(kat1);
        angle2 = Angle_Normal(kat2);

        if (angle2 < angle1) {
            angle2 += Pi2;
            if ((anglep1 < angle1) && ((anglep1 + Pi2) > angle2)) {
                *xx1 = px;
                *yy1 = py;
                return FALSE;
            }
        } else {
            if ((anglep1 < angle1) || (anglep1 > angle2)) {
                *xx1 = px;
                *yy1 = py;
                return FALSE;
            }
        }
    }

    kos=sin(angle);
    koc=cos(angle);

    Rotate_Point(kos, koc, 0, 0, df_x1, df_y1, xx1, yy1);

    *xx1+=cx;
    *yy1+=cy;

    return TRUE;

}


BOOL nearest_point_on_ellipse_ver(double cx, double cy, double a, double b, double angle, double kat1, double kat2, double px, double py, double *x1, double *y1) {

#define nump 1
    double x, y, xx, yy, ecos_ret, esin_ret;
    double kos, koc;
    double df_x1, df_y1, df_x[nump], df_y[nump];
    double anglep, anglep1, angle1, angle2;
    double dl;
    double XSel, YSel;
    double dlmin=MAXDOUBLE;
    double mm, nn;

    xx=px-cx;
    yy=py-cy;

    kos=sin(-angle);
    koc=cos(-angle);

    Rotate_Point(kos, koc, 0, 0, xx, yy, &x, &y);

    for (int i=0; i<nump; i++) {

        double ax = fabs(a * x);
        double by = fabs(b * y);
        double r = b * b - a * a;
        double c, d;
        int switched = 0;

        if (ax <= by) {
            if (by == 0) {
                if (r >= 0) {
                    ecos_ret = 1;
                    esin_ret = 0;
                }
                else {
                    ecos_ret = 0;
                    esin_ret = 1;
                }
                return FALSE;
            }
            c = (ax - r) / by;
            d = (ax + r) / by;
        } else {
            c = (by + r) / ax;
            d = (by - r) / ax;
            switched = 1;
        }

        double cc = c * c;
        double D0 = 12 * (c * d + 1);      // *-4
        double D1 = 54 * (d * d - cc);     // *4
        double D = D1 * D1 + D0 * D0 * D0;  // *16

        double St;
        if (D < 0) {
            double t = sqrt(-D0);             // *2
            double phi = acos(D1 / (t * t * t));
            St = 2 * t * cos((1.0 / 3) * phi);        // *2
        } else {
            double Q = cbrt(D1 + sqrt(D));    // *2
            St = Q - D0 / Q;                  // *2
        }

        double p = 3 * cc;                          // *-2
        double SS = (1.0 / 3) * (p + St);              // *4
        double S = sqrt(SS);                      // *2
        double q = 2 * cc * c + 4 * d;                  // *2
        double l = sqrt(p - SS + q / S) - S - c;  // *2
        double ll = l * l;                           // *4
        double ll4 = ll + 4;                        // *4
        double esin = (4 * l) / ll4;
        double ecos = (4 - ll) / ll4;

        if (switched) {
            double t = esin;
            esin = ecos;
            ecos = t;
        }

        ecos_ret = copysign(ecos, a * x);
        esin_ret = copysign(esin, b * y);

        if ((ecos_ret != ecos_ret) || (esin_ret != esin_ret)) return FALSE;

        anglep = atan2(esin_ret, ecos_ret);

        anglep = Angle_Normal(anglep);

        df_x[i] = a * cos(anglep);
        df_y[i] = b * sin(anglep);


        //point on the other side
        if (Check_if_Equal(x, 0.0))
        {
            y=-y;
        } else {

            mm=y/x;
            nn=y-mm*x;
            //point on the other side
            x = -x;
            y = mm*x+nn;
        }

    }

    //choosing nearest_ point to X,Y
    XSel=X-cx;
    YSel=Y-cy;
    Rotate_Point(kos, koc, 0, 0, XSel, YSel, &XSel, &YSel);

    for (int i=0; i<nump; i++) {
        dl = (XSel - df_x[i]) * (XSel - df_x[i]) + (YSel - df_y[i]) * (YSel - df_y[i]);
        if (dl < dlmin) {
            dlmin=dl;
            df_x1 = df_x[i];
            df_y1 = df_y[i];
        }
    }

    if ((Check_if_Equal(kat1, 0.0)==FALSE) || (Check_if_Equal(kat2, Pi2)==FALSE)) {
        //check if point on elliptical arc
        if (Check_if_Equal(df_x1, cx)) {
            if (df_y1 > cy) anglep1 = Pi / 2.0;
            else anglep1 = 3 * Pi / 2;
        } else {
            anglep1 = Atan2(df_y1, df_x1);
        }

        anglep1 = Angle_Normal(anglep1);

        angle1 = Angle_Normal(kat1);
        angle2 = Angle_Normal(kat2);

        if (angle2 < angle1) {
            angle2 += Pi2;
            if ((anglep1 < angle1) && ((anglep1 + Pi2) > angle2)) {
                *x1 = px;
                *y1 = py;
                return FALSE;
            }
        } else {
            if ((anglep1 < angle1) || (anglep1 > angle2)) {
                *x1 = px;
                *y1 = py;
                return FALSE;
            }
        }
    }

    kos=sin(angle);
    koc=cos(angle);

    Rotate_Point(kos, koc, 0, 0, df_x1, df_y1, x1, y1);

    *x1+=cx;
    *y1+=cy;


    return TRUE;

}


int EllipseIntersectLine(double x, double y, double rx, double ry, double angle, double x1 , double y1 , double x2 , double y2, double *xi1 , double *yi1 , double *xi2 , double *yi2)
{
    double mm, nn, cc, f1, f2, f3, f4, f5, f6;
    double xx1, yy1, xx2, yy2, xxi1, yyi1, xxi2, yyi2;
    double kos, koc;
    int p=3;

    //rotating points x1, y1, y2, y2 by ellipse angle
    kos=sin(-angle);
    koc=cos(-angle);

    xx1=x1-x;
    yy1=y1-y;
    xx2=x2-x;
    yy2=y2-y;

    Rotate_Point(kos, koc, 0, 0, xx1, yy1, &xx1, &yy1);
    Rotate_Point(kos, koc, 0, 0, xx2, yy2, &xx2, &yy2);

    if (Check_if_Equal(xx2, xx1))
    {
        if ((xx1>rx) || (xx1<-rx)) return 0;

        xxi1=xx1;
        yyi1=sqrt(ry*ry*(1-(xxi1*xxi1)/(rx*rx)));

        xxi2=xx1;
        yyi2=-sqrt(ry*ry*(1-(xxi1*xxi1)/(rx*rx)));

        //rotating back
        kos=sin(angle);
        koc=cos(angle);

        Rotate_Point(kos, koc, 0, 0, xxi1, yyi1, &xxi1, &yyi1);
        Rotate_Point(kos, koc, 0, 0, xxi2, yyi2, &xxi2, &yyi2);

        xxi1+=x;
        yyi1+=y;
        xxi2+=x;
        yyi2+=y;

        *xi1=xxi1;
        *yi1=yyi1;
        *xi2 = xxi2;
        *yi2 = yyi2;

        return 2;
    }

    mm=(yy2-yy1)/(xx2-xx1);
    cc=yy1-mm*xx1;

    f1=2*rx*rx*cc*mm;       //2a^2cm
    f2=ry*ry-cc*cc;         //b^2-c^2
    f3=rx*rx*cc*cc*mm*mm;   //a^2c^2m^2
    f4=ry*ry+rx*rx*mm*mm;   //b^2+a^2m^2

    f5=f1/(2*f4);
    f6=rx*sqrt(f2/f4+f3/(f4*f4));

    xxi1=-f5+f6;
    yyi1=mm*(-f5+f6)+cc;

    xxi2=-f5-f6;
    yyi2=mm*(-f5-f6)+cc;

    if ((xxi1 != xxi1) && (yyi1 != yyi1)) p-=1;
    if ((xxi2 != xxi2) && (yyi2 != yyi2)) p-=2;

    if (p==0) return 0;

    //rotating back
    kos=sin(angle);
    koc=cos(angle);

    if (p&1)
    {
        Rotate_Point(kos, koc, 0, 0, xxi1, yyi1, &xxi1, &yyi1);
        xxi1+=x;
        yyi1+=y;
    }
    if (p&2)
    {
        Rotate_Point(kos, koc, 0, 0, xxi2, yyi2, &xxi2, &yyi2);
        xxi2+=x;
        yyi2+=y;
    }

    if ((p&1) && (p&2))
    {
        *xi1=xxi1;
        *yi1=yyi1;

        *xi2 = xxi2;
        *yi2 = yyi2;

        return 2;
    }
    else if (p&1)
    {
        *xi1=xxi1;
        *yi1=yyi1;

        return 1;
    }
    else if (p&2)
    {
        *xi1 = xxi2;
        *yi1 = yyi2;

        return 1;
    }

    return 0;
}

void Set_Ell_Verno (int i_ell_verno)
/*----------------------------------*/
{
  if (i_ell_verno > 7 && i_ell_verno < 254)
  {
    i__Ell_Verno = i_ell_verno ;
  }
}

void Set_Ell_Angle (int i_ell_angle)
/*----------------------------------*/
{
    i__Ell_Angle = i_ell_angle ;
}

int Get_Ell_Verno (void)
/*---------------------*/
{
  return i__Ell_Verno ;
}

int Get_Ell_Angle (void)
/*---------------------*/
{
    return i__Ell_Angle ;
}


#define sin2(x) pow(sin(x),2)
#define cos2(x) pow(cos(x),2)

BOOL Make_Ellipse_variant(double df_xc,
                       double df_yc,
                       double df_xaxis,
                       double df_yaxis,
                       double df_angle,
                       int    i_mode,
                       BLOK   **ptrptr_adb,
                       void  *ptr_ob,
                       int    i_type,
                       int obiektt2)
{
    int nseg=Get_Ell_Verno();

    double ab = df_xaxis * df_yaxis;
    double asq = df_xaxis * df_xaxis;
    double bsq = df_yaxis * df_yaxis;
    double r = ab/sqrt(asq * cos2(-df_angle) + bsq * sin2(-df_angle));
    double lastX = df_xc + cos(-df_angle) * r;
    double lastY = df_yc + sin(-df_angle) * r;
    double lastX0 = lastX;
    double lastY0 = lastY;
    for (int i = 0; i < nseg; i++) {
        double theta = (double)i/(double)nseg * M_PI * 2 - df_angle;
        double s = sin(theta), c = cos(theta);
        r = ab/sqrt(asq * pow(c,2) + bsq * pow(s,2));
        double x = df_xc + c * r;
        double y = df_yc + s * r;
        //drawLine(lastX, lastY, x, y);
        linen (pikseleX0 (lastX), pikseleY0 (lastY), pikseleX0 (x), pikseleY0 (y), i_mode) ;
        lastX = x;
        lastY = y;
    }
    linen (pikseleX0 (lastX), pikseleY0 (lastY), pikseleX0 (lastX0), pikseleY0 (lastY0), i_mode) ;
}



BOOL Make_Ellipse (double df_xc,
		   double df_yc,
		   double df_xaxis,
		   double df_yaxis,
		   double df_angle,
		   int    i_mode,
		   BLOK   **ptrptr_adb,
		   void  *ptr_ob,
		   int    i_type,
		   int obiektt2,
           int type,
           int translucency,
           int kolor,
           int kolor0)
/*--------------------------------*/
{
  double df_si, df_co, df_dangle, df_elangle ;
  double df_x1, df_y1, df_x2, df_y2, df_sx, df_sy ;
  int i ;
  int xy[256];
  int n_xy=0;

  set_pattern_count(TRUE);

  df_si = sin (df_angle) ;
  df_co = cos (df_angle) ;
  df_dangle = Pi2 / i__Ell_Verno ;
  df_x1 = df_xc + df_xaxis ;
  df_y1 = df_yc ;
  Rotate_Point (df_si, df_co, df_xc, df_yc, df_x1, df_y1, &df_x1, &df_y1) ;
  df_sx = df_x1 ;
  df_sy = df_y1 ;
  for (i = 1, df_elangle = df_dangle ; i < i__Ell_Verno ; i++, df_elangle += df_dangle)
  {
    if( mvcurb.akton && mvcurb.mvc ) { i__ell_count = i; return TRUE ; }
    if( mvcurb.aktoff && i__ell_count == i ) return TRUE ;
    df_x2 = df_xc + df_xaxis * cos (df_elangle) ;
    df_y2 = df_yc + df_yaxis * sin (df_elangle) ;
    Rotate_Point (df_si, df_co, df_xc, df_yc, df_x2, df_y2, &df_x2, &df_y2) ;
    if (FALSE == elipse_seg (df_xc, df_yc, df_x1, df_y1, df_x2, df_y2, i_mode, ptrptr_adb, ptr_ob, i_type, obiektt2,
                             &xy[n_xy], &xy[n_xy+1],&xy[n_xy+2],&xy[n_xy+3], type, translucency))
    {
        set_pattern_count(FALSE);
        set_pattern_offset(0);
      return FALSE ;
    }
    df_x1 = df_x2 ;
    df_y1 = df_y2 ;

    n_xy+=2;
  }
  if (FALSE == elipse_seg (df_xc, df_yc, df_x1, df_y1, df_sx, df_sy, i_mode, ptrptr_adb, ptr_ob, i_type, obiektt2,
                           &xy[n_xy], &xy[n_xy+1],&xy[n_xy+2],&xy[n_xy+3], type, translucency))
  {
      set_pattern_count(FALSE);
      set_pattern_offset(0);
    return FALSE ;
  }
  if( mvcurb.akton )
  {
    i__ell_count = i + 1 ;
  }

  if ((i_type==DE_Draw) && (type==1)) {

      if (i_mode==COPY_PUT) {
          if (!kolor0) {
              setfillstyle_(SOLID_FILL, kolor);
              setcolor(kolor);
              set_mode_solid();
          } else{
              setfillstyle_(SOLID_FILL, GetColorAC(kolor));
              SetColorAC(kolor);
              if (translucency<255) {
                  set_trans_blender(0, 0, 0, translucency);
                  set_mode_trans();
              }
              else set_mode_solid();
          }
      }
      else
      {
          setcolor(kolor);
          setfillstyle_(SOLID_FILL, kolor);
          if (translucency<255) {
              set_trans_blender(0, 0, 0, translucency);
              set_mode_trans();
          }
          else set_mode_solid();
      }

      //my_fillpoly(i__Ell_Verno, xy, translucency, kolor);
      fillpoly(i__Ell_Verno, xy);

      set_mode_solid();
  }

    set_pattern_count(FALSE);
    set_pattern_offset(0);
  return TRUE ;
}


BOOL Make_EllipticalArc (double df_xc,
                   double df_yc,
                   double df_xaxis,
                   double df_yaxis,
                   double df_angle,
                   double df_sangle0,
                   double df_eangle0,
                   int    i_mode,
                   BLOK   **ptrptr_adb,
                   void  *ptr_ob,
                   int    i_type,
                   int atrybut,
                   int obiektt1,
                   int obiektt2,
                   int type,
                   int kolor,
                   int kolor0)
/*--------------------------------*/
{
    double df_si, df_co, df_dangle, df_elangle ;
    double df_x1, df_y1, df_x2, df_y2, df_sx, df_sy ;
    int i ;
    int xy[256];
    int n_xy=0;
    double del_angle;
    double df_sangle;
    double df_eangle;
    double as_add=0.0;
    double ae_add=0.0;

    ////////////////
    double vx1, vy1, vx2, vy2;
    double dot,det;
    ////////////////

    set_pattern_count(TRUE);

    df_angle= Angle_Normal(df_angle);

    if (Check_if_Equal(df_sangle0, 0.0)) df_sangle=0.0;
    else if (Check_if_Equal(df_sangle0, Pi/2.0)) df_sangle=Pi/2.0;
    else if (Check_if_Equal(df_sangle0, Pi)) df_sangle=Pi;
    else if (Check_if_Equal(df_sangle0, 3.0*Pi/2.0)) df_sangle=3.0*Pi/2.0;
    else {
        if (Check_if_GE(df_sangle0, 3.0 * Pi / 2.0)) as_add = 2.0 * Pi;
        else if (Check_if_GE(df_sangle0, Pi)) as_add = Pi;
        else if (Check_if_GE(df_sangle0, Pi / 2.0)) as_add = Pi;

        df_sangle = Angle_Normal(atan(df_xaxis * tan(df_sangle0)/df_yaxis)+as_add);
    }

    if (Check_if_Equal(df_eangle0, 0.0)) df_eangle=0.0;
    else if (Check_if_Equal(df_eangle0, Pi/2.0)) df_eangle=Pi/2.0;
    else if (Check_if_Equal(df_eangle0, Pi)) df_eangle=Pi;
    else if (Check_if_Equal(df_eangle0, 3.0*Pi/2.0)) df_eangle=3.0*Pi/2.0;
    else {
        if (Check_if_GE(df_eangle0, 3.0 * Pi / 2.0)) ae_add = 2.0 * Pi;
        else if (Check_if_GE(df_eangle0, Pi)) ae_add = Pi;
        else if (Check_if_GE(df_eangle0, Pi / 2.0)) ae_add = Pi;

        df_eangle = Angle_Normal(atan(df_xaxis * tan(df_eangle0)/df_yaxis)+ae_add);
    }


    if (df_eangle<df_sangle) df_eangle+=Pi2;

    df_elangle = df_sangle;
    del_angle=df_eangle-df_sangle;

    df_si = sin (df_angle) ;
    df_co = cos (df_angle) ;

    df_dangle = del_angle / i__Ell_Verno ;
    df_x1 = df_xc + df_xaxis * cos (df_elangle) ;
    df_y1 = df_yc + df_yaxis * sin (df_elangle) ;

    Rotate_Point (df_si, df_co, df_xc, df_yc, df_x1, df_y1, &df_x1, &df_y1) ;
    df_sx = df_x1 ;
    df_sy = df_y1 ;
    df_elangle += df_dangle;

    ////////////////

    if ((i_mode==DE_Draw) && (atrybut==Ablok) && ((obiektt1==Utwierdzony1) || (obiektt1==Utwierdzony2))) {
        long x1 = pikseleX0(df_xc);
        long y1 = pikseleY0(df_yc);

        if (type == 0) linen(x1 - 2, y1, x1 + 2, y1, i_mode);
        if (type == 0) linen(x1, y1 - 2, x1, y1 + 2, i_mode);
    }

    ////////////////

    for (i = 1; i <= i__Ell_Verno ; i++, df_elangle += df_dangle)
    {
        if( mvcurb.akton && mvcurb.mvc ) { i__ell_count = i; return TRUE ; }
        if( mvcurb.aktoff && i__ell_count == i ) return TRUE ;

        df_x2 = df_xc + df_xaxis * cos (df_elangle) ;
        df_y2 = df_yc + df_yaxis * sin (df_elangle) ;

        Rotate_Point (df_si, df_co, df_xc, df_yc, df_x2, df_y2, &df_x2, &df_y2) ;
        if (FALSE == elipse_seg (df_xc, df_yc, df_x1, df_y1, df_x2, df_y2, i_mode, ptrptr_adb, ptr_ob, i_type, obiektt2,
                                 &xy[n_xy], &xy[n_xy+1],&xy[n_xy+2],&xy[n_xy+3], type, 255))
        {
            set_pattern_count(FALSE);
            set_pattern_offset(0);
            return FALSE ;
        }
        df_x1 = df_x2 ;
        df_y1 = df_y2 ;

        n_xy+=2;
    }


    if( mvcurb.akton )
    {
        i__ell_count = i + 1 ;
    }

    set_pattern_count(FALSE);
    set_pattern_offset(0);

    //flip_screen(); ////TEMPORARY
    return TRUE ;
}


int Get_Ellipse_Points (double df_xc,
                   double df_yc,
                   double df_xaxis,
                   double df_yaxis,
                   double df_angle,
                   double xy[])
/*--------------------------------*/
{
    double df_si, df_co, df_dangle, df_elangle ;
    double df_x1, df_y1, df_x2, df_y2; //, df_sx, df_sy ;
    int i ;
    int n_xy=0;

    df_si = sin (df_angle) ;
    df_co = cos (df_angle) ;
    df_dangle = Pi2 / i__Ell_Verno ;
    df_x1 = df_xc + df_xaxis ;
    df_y1 = df_yc ;
    Rotate_Point (df_si, df_co, df_xc, df_yc, df_x1, df_y1, &df_x1, &df_y1) ;

    xy[n_xy]=df_x1;
    xy[n_xy+1]=df_y1;

    n_xy+=2;

    for (i = 1, df_elangle = df_dangle ; i < i__Ell_Verno ; i++, df_elangle += df_dangle)
    {
        df_x2 = df_xc + df_xaxis * cos (df_elangle) ;
        df_y2 = df_yc + df_yaxis * sin (df_elangle) ;
        Rotate_Point (df_si, df_co, df_xc, df_yc, df_x2, df_y2, &df_x2, &df_y2) ;

        xy[n_xy]=df_x2;
        xy[n_xy+1]=df_y2;

        n_xy+=2;
    }

    return n_xy ;
}

int Get_EllipticalArc_Points (double df_xc,
                        double df_yc,
                        double df_xaxis,
                        double df_yaxis,
                        double df_angle,
                        double df_sangle0,
                        double df_eangle0,
                        double xy[])
/*--------------------------------*/
{
    double df_si, df_co, df_dangle, df_elangle ;
    double df_x1, df_y1, df_x2, df_y2;
    int i ;
    int n_xy=0;
    double del_angle;
    double df_sangle;
    double df_eangle;
    double as_add=0.0;
    double ae_add=0.0;

    df_angle= Angle_Normal(df_angle);

    if (Check_if_Equal(df_sangle0, 0.0)) df_sangle=0.0;
    else if (Check_if_Equal(df_sangle0, Pi/2.0)) df_sangle=Pi/2.0;
    else if (Check_if_Equal(df_sangle0, Pi)) df_sangle=Pi;
    else if (Check_if_Equal(df_sangle0, 3.0*Pi/2.0)) df_sangle=3.0*Pi/2.0;
    else {
        if (Check_if_GE(df_sangle0, 3.0 * Pi / 2.0)) as_add = 2.0 * Pi;
        else if (Check_if_GE(df_sangle0, Pi)) as_add = Pi;
        else if (Check_if_GE(df_sangle0, Pi / 2.0)) as_add = Pi;

        df_sangle = Angle_Normal(atan(df_xaxis * tan(df_sangle0)/df_yaxis)+as_add);
    }

    if (Check_if_Equal(df_eangle0, 0.0)) df_eangle=0.0;
    else if (Check_if_Equal(df_eangle0, Pi/2.0)) df_eangle=Pi/2.0;
    else if (Check_if_Equal(df_eangle0, Pi)) df_eangle=Pi;
    else if (Check_if_Equal(df_eangle0, 3.0*Pi/2.0)) df_eangle=3.0*Pi/2.0;
    else {
        if (Check_if_GE(df_eangle0, 3.0 * Pi / 2.0)) ae_add = 2.0 * Pi;
        else if (Check_if_GE(df_eangle0, Pi)) ae_add = Pi;
        else if (Check_if_GE(df_eangle0, Pi / 2.0)) ae_add = Pi;

        df_eangle = Angle_Normal(atan(df_xaxis * tan(df_eangle0)/df_yaxis)+ae_add);
    }

    if (df_eangle<df_sangle) df_eangle+=Pi2;

    df_elangle = df_sangle;
    del_angle=df_eangle-df_sangle;

    df_si = sin (df_angle) ;
    df_co = cos (df_angle) ;

    df_dangle = del_angle / i__Ell_Verno ;
    df_x1 = df_xc + df_xaxis * cos (df_sangle) ;
    df_y1 = df_yc + df_yaxis * sin (df_sangle) ;

    Rotate_Point (df_si, df_co, df_xc, df_yc, df_x1, df_y1, &df_x1, &df_y1) ;
    df_elangle += df_dangle;

    xy[n_xy]=df_x1;
    xy[n_xy+1]=df_y1;

    n_xy+=2;
    for (i = 1; i <= i__Ell_Verno ; i++, df_elangle += df_dangle)
    {
        df_x2 = df_xc + df_xaxis * cos (df_elangle) ;
        df_y2 = df_yc + df_yaxis * sin (df_elangle) ;

        Rotate_Point (df_si, df_co, df_xc, df_yc, df_x2, df_y2, &df_x2, &df_y2) ;

        xy[n_xy]=df_x2;
        xy[n_xy+1]=df_y2;

        n_xy+=2;
    }

    return n_xy;
}


BOOL Ellipse_Of_Centre_And_Two_Points(double h, double k, double p, double q, double m, double n, double *a, double *b)
{

    double diff = fabs((p-m)*(k-n)-(q-n)*(h-m));

    if (Check_if_LT(diff, 0.05))
    {
        //all on straight line
        //both halves must be the same
        double diff1=fabs(sqrt((h-m)*(h-m)+(k-n)*(k-n))-sqrt((p-h)*(p-h)+(q-k)*(q-k)));

        if  (Check_if_GT(diff1, 0.001)) return FALSE;
        *a=sqrt((h-m)*(h-m)+(k-n)*(k-n));
        //*b is not modified
        if ((*a != *a) || (*b != *b)) return FALSE;
        return TRUE;
    }

    double f1=(p-h)*(p-h);
    double f2=(p-m)*(p+m-(2*h));
    double f3=(q-k)*(q-k);
    double f4=(n-q)*(n+q-(2*k));

    *a=sqrt(((f1*f4)/f2) + f3) * sqrt(f2/f4);
    *b=sqrt((((f1*f4)/f2) + f3));

    if ((*a != *a) || (*b != *b)) return FALSE;

    return TRUE;
}

void Tangential_Point_To_Ellipse(double cx, double cy, double rx, double ry, double xx1, double yy1, double xx2, double yy2, double *x, double *y)
{
    double mm, cc, dl1, dl2;
    double dot, det;
    double df_angle;
    double kos, koc;
    double x1, y1, x2, y2;
    double f1;

    //line parameters
    mm=(yy2-yy1)/(xx2-xx1);
    cc=yy1-mm*xx1;

    //maximum distance from the ellipise-arc to the chord, ellipse and crossing rotated by opposite ellipse angle
    dl1=fabs(cc-sqrt((rx*rx*mm*mm)+(ry*ry)))/sqrt(1+(mm*mm));
    dl2=fabs(cc+sqrt((rx*rx*mm*mm)+(ry*ry)))/sqrt(1+(mm*mm));

    f1=sqrt(((rx*rx*mm*mm)+(ry*ry)));

    x1=-(rx*rx*mm)/f1;
    x1=-(ry*ry)/f1;

    x2=(rx*rx*mm)/f1;
    x2=(ry*ry)/f1;

    //choosing point
    *x=x1+cx;
    *y=y1+cy;

}


void Get_EllipticalArc_EndPoints (double df_xc,
                              double df_yc,
                              double df_xaxis,
                              double df_yaxis,
                              double df_angle,
                              double df_sangle0,
                              double df_eangle0,
                              double *x1, double *y1, double *x2, double *y2)
/*-------------------------------------------------------------------------*/
{
    double df_si, df_co;
    double df_x1, df_y1, df_x2, df_y2;
    int i ;
    double df_sangle;
    double df_eangle;
    double as_add=0.0;
    double ae_add=0.0;

    df_angle= Angle_Normal(df_angle);

    if (Check_if_Equal(df_sangle0, 0.0)) df_sangle=0.0;
    else if (Check_if_Equal(df_sangle0, Pi/2.0)) df_sangle=Pi/2.0;
    else if (Check_if_Equal(df_sangle0, Pi)) df_sangle=Pi;
    else if (Check_if_Equal(df_sangle0, 3.0*Pi/2.0)) df_sangle=3.0*Pi/2.0;
    else {
        if (Check_if_GE(df_sangle0, 3.0 * Pi / 2.0)) as_add = 2.0 * Pi;
        else if (Check_if_GE(df_sangle0, Pi)) as_add = Pi;
        else if (Check_if_GE(df_sangle0, Pi / 2.0)) as_add = Pi;

        df_sangle = Angle_Normal(atan(df_xaxis * tan(df_sangle0)/df_yaxis)+as_add);
    }

    if (Check_if_Equal(df_eangle0, 0.0)) df_eangle=0.0;
    else if (Check_if_Equal(df_eangle0, Pi/2.0)) df_eangle=Pi/2.0;
    else if (Check_if_Equal(df_eangle0, Pi)) df_eangle=Pi;
    else if (Check_if_Equal(df_eangle0, 3.0*Pi/2.0)) df_eangle=3.0*Pi/2.0;
    else {
        if (Check_if_GE(df_eangle0, 3.0 * Pi / 2.0)) ae_add = 2.0 * Pi;
        else if (Check_if_GE(df_eangle0, Pi)) ae_add = Pi;
        else if (Check_if_GE(df_eangle0, Pi / 2.0)) ae_add = Pi;

        df_eangle = Angle_Normal(atan(df_xaxis * tan(df_eangle0)/df_yaxis)+ae_add);
    }

    if (df_eangle<df_sangle) df_eangle+=Pi2;

    df_si = sin (df_angle) ;
    df_co = cos (df_angle) ;

    df_x1 = df_xc + df_xaxis * cos (df_sangle) ;
    df_y1 = df_yc + df_yaxis * sin (df_sangle) ;

    Rotate_Point (df_si, df_co, df_xc, df_yc, df_x1, df_y1, x1, y1) ;

    df_x2 = df_xc + df_xaxis * cos (df_eangle) ;
    df_y2 = df_yc + df_yaxis * sin (df_eangle) ;

    Rotate_Point (df_si, df_co, df_xc, df_yc, df_x2, df_y2, x2, y2) ;
}



void DrawEllipse(ELLIPSE *e, int mode, int kolor, int kolor0)
{
  Make_Ellipse (e->x, e->y, e->rx, e->ry, e->angle, mode, NULL, NULL, DE_Draw, 0, 0, 255, kolor, kolor0);
}

void DrawEllipticalArc(ELLIPTICALARC *ea, int mode, int kolor, int kolor0)
{
    Make_EllipticalArc (ea->x, ea->y, ea->rx, ea->ry, ea->angle, ea->kat1, ea->kat2, mode, NULL, NULL, DE_Draw, ea->atrybut, ea->obiektt1, 0, 0, kolor, kolor0);
}

void DrawFilledEllipse(ELLIPSE *e, int mode, int kolor, int kolor0)
{
    Make_Ellipse (e->x, e->y, e->rx, e->ry, e->angle, mode, NULL, NULL, DE_Draw, 0, 1, e->translucency, kolor, kolor0);
}


BOOL Make_Ellipse_Arc (double df_xc,
		   double df_yc,
		   double df_xaxis,
		   double df_yaxis,
		   BLOK   **ptrptr_adb,
		   void  *ptr_ob,
		   int    i_type,
		   int obiektt2)
/*--------------------------------*/
{
  double df_dangle, df_elangle ;
  double df_angle_begin, df_angle_end ;
  double df_x1, df_y1, df_x2, df_y2, df_da ;
  int i, N ;
  LUK *ptrs_arc ;
    int xy[255];
    int n_xy=0;

    ptrs_arc = (LUK*)ptr_ob ;
  df_angle_begin = Angle_Normal (ptrs_arc->kat1) ;
  df_angle_end = Angle_Normal (ptrs_arc->kat2) ;
  df_dangle = Pi2 / i__Ell_Verno ;
  df_x1 = df_xc + df_xaxis * cos (df_angle_begin) ;
  df_y1 = df_yc + df_yaxis * sin (df_angle_begin) ;
  df_da = df_angle_end - df_angle_begin ;
  if (FALSE == Check_if_Equal (df_da, 0)  &&  df_angle_begin > df_angle_end)
  {
    df_da += Pi2 ;
  }
  N =  (int)(df_da / df_dangle) ;
  for (i = 0,
       df_elangle = df_angle_begin + df_dangle ;
       i < N ;
       i++,
       df_elangle += df_dangle)
  {
    df_x2 = df_xc + df_xaxis * cos (df_elangle) ;
    df_y2 = df_yc + df_yaxis * sin (df_elangle) ;
    if (FALSE == elipse_seg (df_xc, df_yc, df_x1, df_y1, df_x2, df_y2, 0, ptrptr_adb, ptr_ob, i_type, obiektt2,
                             &xy[n_xy], &xy[n_xy+1],&xy[n_xy+2],&xy[n_xy+3], 0, 255))
    {
      return FALSE ;
    }
    df_x1 = df_x2 ;
    df_y1 = df_y2 ;
  }
  df_x2 = df_xc + df_xaxis * cos (df_angle_end) ;
  df_y2 = df_yc + df_yaxis * sin (df_angle_end) ;
  if (FALSE == elipse_seg (df_xc, df_yc, df_x1, df_y1, df_x2, df_y2, 0, ptrptr_adb, ptr_ob, i_type, obiektt2,
                           &xy[n_xy], &xy[n_xy+1],&xy[n_xy+2],&xy[n_xy+3], 0, 255))
  {
    return FALSE ;
  }
  return TRUE ;
}

int Get_Circle_Points(double df_xc, double df_yc, double df_r, double xy[])
{
    double df_si, df_co, df_dangle, df_elangle;
    double df_x1, df_y1, df_x2, df_y2, df_sx, df_sy;
    int i;

    xy[0] = df_xc + df_r;
    xy[1] = df_yc;

    df_dangle = Pi2 / i__Ell_Verno;

    for (i = 1, df_elangle = df_dangle; i < i__Ell_Verno; i++, df_elangle += df_dangle)
    {
        xy[i*2] = df_xc + df_r * cos(df_elangle);
        xy[i*2+1] = df_yc + df_r * sin(df_elangle);
    }
}

BOOL Make_Ellipse_Shadow(double df_xc,
	double df_yc,
	double df_xaxis,
	double df_yaxis,
	double df_angle,
	int    i_mode,
	BLOK   *ptrptr_adb,
	void  *ptr_ob,
	int    i_type,
	int obiektt2)
	/*--------------------------------*/
{
	double df_si, df_co, df_dangle, df_elangle;
	double df_x1, df_y1, df_x2, df_y2, df_sx, df_sy;
	int i;

	df_si = sin(df_angle);
	df_co = cos(df_angle);
	df_dangle = Pi2 / i__Ell_Verno;
	df_x1 = df_xc + df_xaxis;
	df_y1 = df_yc;
	Rotate_Point(df_si, df_co, df_xc, df_yc, df_x1, df_y1, &df_x1, &df_y1);
	df_sx = df_x1;
	df_sy = df_y1;
	for (i = 1, df_elangle = df_dangle; i < i__Ell_Verno; i++, df_elangle += df_dangle)
	{
		if (mvcurb.akton && mvcurb.mvc) { i__ell_count = i; return TRUE; }
		if (mvcurb.aktoff && i__ell_count == i) return TRUE;
		df_x2 = df_xc + df_xaxis * cos(df_elangle);
		df_y2 = df_yc + df_yaxis * sin(df_elangle);
		Rotate_Point(df_si, df_co, df_xc, df_yc, df_x2, df_y2, &df_x2, &df_y2);
        PTR__GTMP7 = ptrptr_adb;
        if (FALSE == elipse_seg_shadow(df_xc, df_yc, df_x1, df_y1, df_x2, df_y2, i_mode, ptrptr_adb, ptr_ob, i_type, obiektt2))
		{
			return FALSE;
		}
        ptrptr_adb=PTR__GTMP7;
		df_x1 = df_x2;
		df_y1 = df_y2;
	}
    PTR__GTMP7 = ptrptr_adb;
    if (FALSE == elipse_seg_shadow(df_xc, df_yc, df_x1, df_y1, df_sx, df_sy, i_mode, ptrptr_adb, ptr_ob, i_type, obiektt2))
	{
		return FALSE;
	}
    ptrptr_adb=PTR__GTMP7;
	if (mvcurb.akton)
	{
		i__ell_count = i + 1;
	}
	return TRUE;
}

BOOL Make_Ellipse_Arc_Shadow(double df_xc,
	double df_yc,
	double df_xaxis,
	double df_yaxis,
	BLOK   *ptrptr_adb,
	void  *ptr_ob,
	int    i_type,
	int obiektt2)
	/*--------------------------------*/
{
	double df_dangle, df_elangle;
	double df_angle_begin, df_angle_end;
	double df_x1, df_y1, df_x2, df_y2, df_da;
	int i, N;
	LUK *ptrs_arc;

	ptrs_arc = (LUK*)ptr_ob;
	df_angle_begin = Angle_Normal(ptrs_arc->kat1);
	df_angle_end = Angle_Normal(ptrs_arc->kat2);
	df_dangle = Pi2 / i__Ell_Verno;
	df_x1 = df_xc + df_xaxis * cos(df_angle_begin);
	df_y1 = df_yc + df_yaxis * sin(df_angle_begin);
	df_da = df_angle_end - df_angle_begin;
	if (FALSE == Check_if_Equal(df_da, 0) && df_angle_begin > df_angle_end)
	{
		df_da += Pi2;
	}
	N = (int)(df_da / df_dangle);
	for (i = 0,
		df_elangle = df_angle_begin + df_dangle;
		i < N;
		i++,
		df_elangle += df_dangle)
	{
		df_x2 = df_xc + df_xaxis * cos(df_elangle);
		df_y2 = df_yc + df_yaxis * sin(df_elangle);
        PTR__GTMP7 = ptrptr_adb;
		if (FALSE == elipse_seg_shadow(df_xc, df_yc, df_x1, df_y1, df_x2, df_y2, 0, ptrptr_adb, ptr_ob, i_type, obiektt2))
		{
			return FALSE;
		}
        ptrptr_adb=PTR__GTMP7;
		df_x1 = df_x2;
		df_y1 = df_y2;
	}
	df_x2 = df_xc + df_xaxis * cos(df_angle_end);
	df_y2 = df_yc + df_yaxis * sin(df_angle_end);
    PTR__GTMP7 = ptrptr_adb;
	if (FALSE == elipse_seg_shadow(df_xc, df_yc, df_x1, df_y1, df_x2, df_y2, 0, ptrptr_adb, ptr_ob, i_type, obiektt2))
	{
		return FALSE;
	}
    ptrptr_adb=PTR__GTMP7;
	return TRUE;
}




static BOOL
elipse_seg (double X0, double Y0,
	    double X1, double Y1,
	    double X2, double Y2,
	    int    i_mode,
	    BLOK   **ptrptr_adb,
	    void   *ptr_ob,
	    int    i_type,
	    int obiektt2,
        int *x1, int *y1, int *x2, int *y2,
        int type,
        int translucency
        )
/*-------------------------------*/
{
  BOOL b_ret ;
  OKRAG *ptrs_circle ;
  LUK *ptrs_arc ;
  NAGLOWEK *nag ;
  LINIA s_line = Ldef ;
  WIELOKAT s_solid = Stdef ;
  char* translucency_ptr;

  b_ret = TRUE ;
  switch (i_type)
  {
    case DE_Draw :
       *x1=pikseleX0 (X1);
       *y1=pikseleY0 (Y1);
       *x2=pikseleX0 (X2);
       *y2=pikseleY0 (Y2);

       if (type==0) linen (*x1, *y1, *x2, *y2, i_mode) ;
       break ;
    case DE_Add :
       LiniaG.x1 = X1 ;
       LiniaG.y1 = Y1 ;
       LiniaG.x2 = X2 ;
       LiniaG.y2 = Y2 ;
       if (dodaj_obiekt (*ptrptr_adb, (void*)&LiniaG) == NULL)
       {
	 b_ret = FALSE ;
       }
       break ;
    case DE_Add_SGP :
    case DE_ADD_Ell_Arc_SGP :
       nag = (NAGLOWEK*)ptr_ob ;
       if (nag->obiekt == Oluk)
       {
	 ptrs_arc = (LUK*)ptr_ob ;
	 s_line.atrybut = ptrs_arc->atrybut ;
	 s_line.warstwa = ptrs_arc->warstwa ;
	 s_line.kolor = ptrs_arc->kolor ;
	 s_line.typ = ptrs_arc->typ ;
       }
       else
       {
	 ptrs_circle = (OKRAG*)ptr_ob ;
	 s_line.atrybut = ptrs_circle->atrybut ;
	 s_line.warstwa = ptrs_circle->warstwa ;
	 s_line.kolor = ptrs_circle->kolor ;
	 s_line.typ = ptrs_circle->typ ;
       }
       s_line.blok = 1 ;
       s_line.obiektt2 = obiektt2 ;//O2BlockPline ;
       s_line.x1 = X1 ;
       s_line.y1 = Y1 ;
       s_line.x2 = X2 ;
       s_line.y2 = Y2 ;
       if(linia_wybrana(&s_line))
	 s_line.widoczny = 1 ;
       else
	s_line.widoczny = 0 ;
       if (NULL == Add_Object_Set_Global_Ptr (ptrptr_adb, (void*)&s_line))
       {
	 b_ret = FALSE ;
       }
       break ;
    case DE_Add_Ell_Fill_SGP :
       ptrs_circle = (OKRAG*)ptr_ob ;
       s_solid.atrybut = ptrs_circle->atrybut ;
       s_solid.blok = 1 ;
       s_solid.lp = 6 ;
       s_solid.xy [0] = X0 ;
       s_solid.xy [1] = Y0 ;
       s_solid.xy [2] = X1 ;
       s_solid.xy [3] = Y1 ;
       s_solid.xy [4] = X2 ;
       s_solid.xy [5] = Y2 ;
       s_solid.warstwa = ptrs_circle->warstwa ;
       s_solid.kolor = ptrs_circle->kolor ;
       if (translucency<255)
       {
           s_solid.translucent=1;

           translucency_ptr = s_solid.xy;
           translucency_ptr += (s_solid.lp * sizeof(float));
           memmove(translucency_ptr, &translucency, sizeof(unsigned char));

           s_solid.n = 8 + s_solid.lp * sizeof(float) + sizeof(unsigned char);
       } else{
           s_solid.n = 8 + s_solid.lp * sizeof(float);
       }
       if (wielokat_wybrany (&s_solid))
	        s_solid.widoczny = 1 ;
       else
	        s_solid.widoczny = 0 ;
       if (NULL == Add_Object_Set_Global_Ptr (ptrptr_adb, (void*)&s_solid))
       {
	 b_ret = FALSE ;
       }
       break ;
    default :
       break ;
  }
  return b_ret ;
}


BOOL elipse_seg_shadow(double X0, double Y0,
	double X1, double Y1,
	double X2, double Y2,
	int    i_mode,
	BLOK   *ptrptr_adb,
	void   *ptr_ob,
	int    i_type,
	int obiektt2)
	/*-------------------------------*/
{
	BOOL b_ret;
	OKRAG *ptrs_circle;
	LUK *ptrs_arc;
	NAGLOWEK *nag;
	LINIA s_line = Ldef;

	b_ret = TRUE;

	nag = (NAGLOWEK*)ptr_ob;
	
	if (nag->obiekt == Oluk)
	{
		ptrs_arc = (LUK*)ptr_ob;
		s_line.atrybut = ptrs_arc->atrybut;
		s_line.warstwa = ptrs_arc->warstwa;
		s_line.kolor = ptrs_arc->kolor;
		s_line.typ = ptrs_arc->typ;
	}
	else
	{
		ptrs_circle = (OKRAG*)ptr_ob;
		s_line.atrybut = ptrs_circle->atrybut;
		s_line.warstwa = ptrs_circle->warstwa;
		s_line.kolor = ptrs_circle->kolor;
		s_line.typ = ptrs_circle->typ;
	}
	s_line.blok = 1;
	s_line.obiektt2 = obiektt2;//O2BlockPline ;
	s_line.x1 = (float)X1;
	s_line.y1 = (float)Y1;
	s_line.x2 = (float)X2;
	s_line.y2 = (float)Y2;


	if (dodaj_obiekt_reversed(ptrptr_adb, (void*)&s_line) == NULL)
	{
		b_ret = FALSE;
	}

	return b_ret;
}