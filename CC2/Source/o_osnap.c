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

#include <forwin.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bib_e.h"
#include "allegext.h"
#include "rysuj_e.h"
#include "o_osnap.h"
#include "o_libfun.h"
#include "o_object.h"

#include "leak_detector_c.h"

#define o_male_p  0.001

static double df__dxjump, df__dyjump ;
static double Xsel,Ysel;
static int stretch_vector=0;
static BOOL global_set_stretch=FALSE;
static AVECTOR *VectorS=NULL;

extern NODE bnodes[MAX_NODES];
extern double curviness;
extern void calc_tangents(int closed, int* node_count, double* bnodes_2_tangent);
extern void set_bnodes(SPLINE* ptrs_spline, float *ad_xy, int* node_count_);
extern void End_Spline(double *x, double *y, double x0, double y0, SPLINE *bspline);
extern void get_control_points_reversed(NODE n1, NODE n2, float points[]);
extern void calc_bspline(int lp, int n, float points[], int p1, float* out_x, float* out_y);
extern void get_control_points(NODE n1, NODE n2, float points[]);
extern void get_control_points_reversed(NODE n1, NODE n2, float points[]);
extern void get_control_points_end(NODE n1, NODE n2, float points[]);
extern BOOL End_Elliptical_Arc_XY(double *x, double *y, double x0, double y0, int numpoints, double xy[]);
extern int Get_Ellipse_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double xy[]);
extern int Get_EllipticalArc_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double df_sangle, double df_eangle, double xy[]);
extern void Set_Ell_Verno (int i_ell_verno);
extern int Get_Ell_Verno (void);

extern int EllipseIntersectLine(double x, double y, double rx, double ry, double angle, double x1 , double y1 , double x2 , double y2, double *xi1 , double *yi1 , double *xi2, double *yi2);
extern BOOL nearest_point_on_ellipse(double cx, double cy, double a, double b, double angle, double kat1, double kat2, double px, double py, double *x1, double *y1);
extern BOOL find_tangent_lines_in_ellipse(double x0, double y0, double a, double b, double angle, double p0, double q0, double *m1, double *h1, double *m2, double *h2, double x[], double y[]);
extern int solidarc_elements(SOLIDARC *sa, LINIA *L_left, LINIA *L_right, LUK *l_inner1, LUK *l_inner2, LUK *l_outer1, LUK *l_outer2);
extern void set_decimal_format(char *text, double l, double precision);
extern double Get_TextLen (TEXT *ptrs_text, char *ptrsz_t);
extern double Get_Point_Size (void);
extern double normalize_txt_angle(double angle0);
extern void normalize_txt(TEXT *ptrs_text);
extern char *load_symbol[];
extern int vector_text_wybrany(LINIA *L, TEXT *Vtxt, AVECTOR* v);
extern int vector_magnitude_text_wybrany(TEXT *Vtxt, TEXT *Vtxt1, AVECTOR* v, LINIA *L);
extern int arc_to_isometric_ellipticalarc_a_ea(enum PlaneType plane, LUK *l, ELLIPTICALARC *ea);
extern void srodekea_(double *x,double *y, double *tangent, void *adr);
extern void Get_EllipticalArc_EndPoints (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle,double df_sangle0,double df_eangle0,double *x1, double *y1, double *x2, double *y2);
extern int isometric_vector_to_cartesian(double dx_iso, double dy_iso, double *dx_cart, double *dy_cart);
extern double cartesian_angle_to_isometric_angle(double cart_angle_rad);
extern double isometric_angle_to_cartesian_angle_rad(double iso_angle_rad);
extern int cartesian_to_isometric(double cx, double cy, double *ix, double *iy);
extern int trapezoid_base_isometric_x(float x1, float y1, float x2, float y2, float *base_x1, float *base_y1, float *base_x2, float *base_y2);
extern int trapezoid_base_isometric_y(float x1, float y1, float x2, float y2,float *base_x1, float *base_y1, float *base_x2, float *base_y2);
extern void koniec1el_(double *x,double *y,void *adr);

extern double depth_magnitude; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double thermal_magnitude; //units per mm  default 1 Celsius per 1 mm on drawing paper
extern double load_magnitude; //units per mm  default 10kN/m force per 1 mm on drawing paper
extern double flood_magnitude; //units per mm  default 10kN/m² load per 1 mm on drawing paper
extern double shear_magnitude; //units per mm  default 10kN/m shear/reaction per 1 mm on drawing paper
extern double force_magnitude; //units per mm  default 10kN force per 1 mm on drawing paper

extern double dim_precision;
extern double force_precision;
extern double displacement_precision;
extern double moment_precision;
extern double rotation_precision;
extern double load_precision;


void set_global_set_stretch(BOOL v_s_s)
{
    global_set_stretch=v_s_s;
}

int get_stretch_vector(void)
{
    return stretch_vector;
}

void reset_stretch_vector(void)
{
    stretch_vector=0;
}

void *get_vector_s(void)
{
    return VectorS;
}

#include <stdbool.h>
#include <math.h>

// Function to check if point (x, y) lies on the line segment from (x1, y1) to (x2, y2)
// with epsilon tolerance to exclude points near the endpoints.
BOOL CheckIsPointOnLineSegment_(float x, float y, float x1, float y1, float x2, float y2, float epsilon) {
    // Compute vectors: P - P1 and P2 - P1
    float dx = x2 - x1;
    float dy = y2 - y1;
    float px = x - x1;
    float py = y - y1;

    // Check if the segment is a point (degenerate case)
    if (fabs(dx) < epsilon && fabs(dy) < epsilon) {
        return FALSE; // Degenerate segment (P1 == P2)
    }

    // Compute the cross product to find the distance from point to line
    // Cross product: |(P - P1) x (P2 - P1)| / |P2 - P1|
    float cross = fabs(px * dy - py * dx);
    float len = sqrt(dx * dx + dy * dy); // Length of segment
    float distance = cross / len;

    // Check if the point is collinear (distance to line is nearly zero)
    if (distance > epsilon) { // Use small tolerance for floating-point comparison
        return FALSE; // Point is not on the line
    }

    // Compute the projection parameter t = [(P - P1) . (P2 - P1)] / |P2 - P1|^2
    float dot = px * dx + py * dy;
    float t = dot / (len * len);

    // Check if the point lies within the segment (excluding endpoints)
    if (t <= 0.0f || t >= 1.0f) {
        return FALSE; // Point lies on or beyond endpoints
    }

    // Check distances from the point to both endpoints to enforce epsilon tolerance
    float dist_to_p1 = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
    float dist_to_p2 = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));

    if (dist_to_p1 <= epsilon || dist_to_p2 <= epsilon) {
        return FALSE; // Point is too close to an endpoint
    }

    return TRUE; // Point lies on the segment interior
}

BOOL CheckIsPointOnLineSegment__(float x, float y, float x1, float y1, float x2, float y2, float epsilon)
{

    float minX = min(x1, x2);
    float maxX = max(x1, x2);

    float minY = min(y1, y2);
    float maxY = max(y1, y2);

    if (!(minX <= x) || !(x <= maxX) || !(minY <= y) || !(y <= maxY))
    {
        return FALSE;
    }

    if (fabs(x1 - x2) < epsilon)
    {
        return fabs(x1 - x) < epsilon || fabs(x2 - x) < epsilon;
    }

    if (fabs(y1 - y2) < epsilon)
    {
        return fabs(y1 - y) < epsilon || fabs(y2 - y) < epsilon;
    }

    if (fabs((x - x1) / (x2 - x1) - (y - y1) / (y2 - y1)) < epsilon)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL CheckIsPointOnLineSegment(double x, double y, double x1, double y1, double x2, double y2, double epsilon)
{

    double minX = min(x1, x2);
    double maxX = max(x1, x2);

    double minY = min(y1, y2);
    double maxY = max(y1, y2);

    if (!(minX <= x) || !(x <= maxX) || !(minY <= y) || !(y <= maxY))
    {
        return FALSE;
    }

    if (fabs(x1 - x2) < epsilon)
    {
        return fabs(x1 - x) < epsilon || fabs(x2 - x) < epsilon;
    }

    if (fabs(y1 - y2) < epsilon)
    {
        return fabs(y1 - y) < epsilon || fabs(y2 - y) < epsilon;
    }

    if (fabs((x - x1) / (x2 - x1) - (y - y1) / (y2 - y1)) < epsilon)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

int koniecL_factory (double *x, double *y, void *adr)
/*--------------------------------------------*/
{
  LINIA  *adrl;
  PLINIA PL;
  double a,a1,X01, df_min_prec ;

  adrl = (LINIA*)adr ;
  if (TRUE == Check_if_Equal (adrl->x1, adrl->x2))
  {
    if (fabs (*y - adrl->y1) > fabs (*y - adrl->y2))
    {
      *x = adrl->x2 ;
      *y = adrl->y2 ;
      return 2;
    }
    else
    {
      *x = adrl->x1 ;
      *y = adrl->y1 ;
      return 1;
    }
  }
  else
  if (TRUE == Check_if_Equal (adrl->y1, adrl->y2))
  {
    if (fabs (*x - adrl->x1) > fabs (*x - adrl->x2))
    {
      *x = adrl->x2 ;
      *y = adrl->y2 ;
      return 2;
    }
    else
    {
      *x = adrl->x1 ;
      *y = adrl->y1 ;
      return 1;
    }
  }
  else
  {
    parametry_lini ((LINIA*)adrl, &PL) ;
    df_min_prec = Get_Prec (MPREC_DOUBLE, 1, 1) ;
    if (fabs (PL.sin) < df_min_prec)
    {
      PL.sin = df_min_prec ;
    }
    if (fabs (PL.cos) < df_min_prec)
    {
      PL.cos = df_min_prec ;
    }
    a = PL.sin / PL.cos ;
    a1 = 1 / a ;
    X01 = X ;
    *x = (Y - adrl->y1 + a * adrl->x1 + X01 * a1) / (a + a1) ;
    *y = Y - a1 * (*x - X01) ;
    if (fabs (*x - adrl->x1) > fabs (*x - adrl->x2))
    {
      *x = adrl->x2 ;
      *y = adrl->y2 ;
      return 2;
    }
    else
    {
      *x = adrl->x1 ;
      *y = adrl->y1 ;
      return 1;
    }
  }
  return 0;
}

void set_stretch_vector(int s_v, char *vector)
{
    stretch_vector = s_v;
    VectorS = vector;
}

void koniecL_ (double *x, double *y, void *adr)
{
    int ret=koniecL_factory (x, y, adr);
}

void koniec1L_ (double *x, double *y, void *adr)
/*--------------------------------------------*/
{
    LINIA  *adrl;
    PLINIA PL;
    double a,a1,X01, df_min_prec ;

    if (global_set_stretch) set_stretch_vector(0, NULL);

    adrl = (LINIA*)adr ;
    if (TRUE == Check_if_Equal (adrl->x1, adrl->x2))
    {
        if (fabs (*y - adrl->y1) > fabs (*y - adrl->y2))
        {
            *x = adrl->x2 ;
            *y = adrl->y2 ;
            if (global_set_stretch) set_stretch_vector(1, adr); //stretching line and value of force or displacement
        }
        else
        {
            *x = adrl->x1 ;
            *y = adrl->y1 ;
        }
    }
    else
    if (TRUE == Check_if_Equal (adrl->y1, adrl->y2))
    {
        if (fabs (*x - adrl->x1) > fabs (*x - adrl->x2))
        {
            *x = adrl->x2 ;
            *y = adrl->y2 ;
            if (global_set_stretch) set_stretch_vector(1, adr); //stretching line and value of force or displacement
        }
        else
        {
            *x = adrl->x1 ;
            *y = adrl->y1 ;
        }
    }
    else
    {
        parametry_lini ((LINIA*)adrl, &PL) ;
        df_min_prec = Get_Prec (MPREC_DOUBLE, 1, 1) ;
        if (fabs (PL.sin) < df_min_prec)
        {
            PL.sin = df_min_prec ;
        }
        if (fabs (PL.cos) < df_min_prec)
        {
            PL.cos = df_min_prec ;
        }
        a = PL.sin / PL.cos ;
        a1 = 1 / a ;
        X01 = X ;
        *x = (Y - adrl->y1 + a * adrl->x1 + X01 * a1) / (a + a1) ;
        *y = Y - a1 * (*x - X01) ;
        if (fabs (*x - adrl->x1) > fabs (*x - adrl->x2))
        {
            *x = adrl->x2 ;
            *y = adrl->y2 ;
            if (global_set_stretch) set_stretch_vector(1, adr); //stretching line and value of force or displacement
        }
        else
        {
            *x = adrl->x1 ;
            *y = adrl->y1 ;
        }
    }
}


void koniec1L___ (double *x, double *y, void *adr)
/*--------------------------------------------*/
{
    LINIA  *adrl;

    adrl = (LINIA*)adr ;

    *x = adrl->x1 ;
    *y = adrl->y1 ;
}

void koniec2L_ (double *x, double *y, void *adr)
/*--------------------------------------------*/
{
    LINIA  *adrl;

    adrl = (LINIA*)adr ;

    *x = adrl->x2 ;
    *y = adrl->y2 ;
}


void wielokat_ (double *x, double *y, void *adr, void (*fun)(double*, double*, void*))
/*----------------------------------------------------------------------------------*/
{
  LINIA L = Ldef ;
  WIELOKAT *w ;

  int i;
  w = adr ;
  for (i = 0 ; i < w->lp - 2 ; i += 2)
  {
    L.x1 = w->xy [i + 0] ;
    L.y1 = w->xy [i + 1] ;
    L.x2 = w->xy [i + 2] ;
    L.y2 = w->xy [i + 3] ;
    if (0 != linia_wybrana (&L))
    {
      fun (x, y, &L) ;
      return ;
    }
  }
  L.x1 = w->xy [0] ;
  L.y1 = w->xy [1] ;
  L.x2 = w->xy [w->lp-2] ;
  L.y2 = w->xy [w->lp-1] ;
  if (0 != linia_wybrana (&L))
  {
    fun (x, y, &L) ;
    return ;
  }
}

 void wielokat_linia_ (double *x, double *y, void *adr, void *adr1, void (*fun)(double*, double*, void*))
/*------------------------------------------------------------------------------------------------------*/
{
    LINIA *L;
    WIELOKAT *w ;

    int i;
    w = adr ;
    L= adr1;
    for (i = 0 ; i < w->lp - 2 ; i += 2)
    {
        L->x1 = w->xy [i + 0] ;
        L->y1 = w->xy [i + 1] ;
        L->x2 = w->xy [i + 2] ;
        L->y2 = w->xy [i + 3] ;
        if (0 != linia_wybrana (L))
        {
            fun (x, y, L) ;
            return ;
        }
    }
    L->x1 = w->xy [0] ;
    L->y1 = w->xy [1] ;
    L->x2 = w->xy [w->lp-2] ;
    L->y2 = w->xy [w->lp-1] ;
    if (0 != linia_wybrana (L))
    {
        fun (x, y, L) ;
        return ;
    }
}

void wspline_(double *x, double *y, void *adr, void(*fun)(double*, double*, void*))
	/*---------------------------------------------------------------------------*/
{
	LINIA L = Ldef;
	SPLINE *s;

	int i;
	s = adr;
    L.x1 = s->xy[0];
    L.y1 = s->xy[1];
    L.x2 = s->xy[s->lp - 2];
    L.y2 = s->xy[s->lp - 1];
    fun(x, y, &L);
    return;
}

void wellipticalarc_(double *x, double *y, void *adr, void(*fun)(double*, double*, void*))
/*--------------------------------------------------------------------------------------*/
{
    LINIA L = Ldef;
    ELLIPTICALARC *ea;
    int numpoints;
    double xy[256];

    int i;
    ea = adr;
    numpoints= Get_EllipticalArc_Points(ea->x, ea->y, ea->rx, ea->ry, ea->angle, ea->kat1, ea->kat2, xy);
    L.x1 = xy[0];
    L.y1 = xy[1];
    L.x2 = xy[numpoints - 2];
    L.y2 = xy[numpoints - 1];
    fun(x, y, &L);
    return;
}

void wellipticalarc_ea_(double *x, double *y, void *adr, void(*fun)(double*, double*, double *, void*))
/*---------------------------------------------------------------------------------------------------*/
{
    double tangent;
    fun(x, y, &tangent, adr);
    return;
}

int wsolidarc_(double *x, double *y, void *adr, void(*funL)(double*, double*, void*), void(*funl)(double*, double*, void*))
/*-----------------------------------------------------------------------------------------------------------------------*/
{
    LINIA L_left, L_right;
    LUK l_inner1, l_inner2, l_outer1, l_outer2;
    SOLIDARC *sa;
    int ln;
    double x1, y1, x2, y2, dxy1, dxy2;
    LUK l_center=ldef;

    sa = adr;
    ln=solidarc_elements(sa, &L_left, &L_right, &l_inner1, &l_inner2, &l_outer1, &l_outer2);

    if ((0 != linia_wybrana(&L_left)) || (0 != linia_wybrana(&L_right)))
    {
        l_center.x=sa->x;
        l_center.y=sa->y;
        l_center.r=sa->r;
        l_center.kat1=sa->kat1;
        l_center.kat2=sa->kat2;

        funl (x, y, &l_center);
        if (punkt_w_prostokacie(*x, *y)) return 1;
    }

    if (funL!=NULL)
    {
        if (0 != linia_wybrana(&L_left)) {
            funL(x, y, &L_left);
            return 1;
        }

        if (0 != linia_wybrana(&L_right)) {
            funL(x, y, &L_right);
            return 1;
        }
    }

    if (0 != luk_wybrany (&l_inner1))
    {
        funl (x, y, &l_inner1) ;
        return 1;
    }

    if (0 != luk_wybrany (&l_outer1))
    {
        funl (x, y, &l_outer1) ;
        return 1;
    }

    if (ln>1)
    {
        if (0 != luk_wybrany (&l_inner2))
        {
            funl (x, y, &l_inner2) ;
            return 1;
        }

        if (0 != luk_wybrany (&l_outer2))
        {
            funl (x, y, &l_outer2) ;
            return 1;
        }
    }

    return 0;
}

int wvector_(double *x, double *y, void *adr, void(*funL)(double*, double*, void*), void(*funl)(double*, double*, void*), void(*fun1L)(double*, double*, void*), void(*fun1l)(double*, double*, void*))
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
{
    AVECTOR *v;
    LUK l=ldef;
    OKRAG O=Odef;
    TEXT Vtxt=Tdef, Vtxt1;
    LINIA Lt, Lth, Lt1, Lt2, LL=Ldef, Lp=Ldef;
    ELLIPTICALARC ea=eldef;
    double n;
    int ret;
    PLINIA PL, PLth;
    double kats, kata2;
    double kos1, koc1, kos1th, koc1th;
    double xmax, ymax;
    double ra;
    double _precision_;
    int plane, reverse;
    double x1, y1, tangent;
    double ea_start_x, ea_start_y, ea_end_x, ea_end_y;
    double arrow_angle;

#define arrowf 1.0

    if (global_set_stretch) stretch_vector=0;

    v = adr;

    Vtxt.warstwa = v->warstwa;
    Vtxt.atrybut=v->atrybut;
    Vtxt.czcionka=zmwym.czcionka;
    Vtxt.width_factor=zmwym.width_factor;
    Vtxt.wysokosc=zmwym.wysokosc;
    memcpy(&Vtxt1, &Vtxt, sizeof(TEXT));

    switch (v->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            if (0 != linia_wybrana(v))
            {
                funL(x, y, v);
                if (punkt_w_prostokacie(*x, *y)) return 1;
            }
            break;
        case 4:  //force
        case 18:  //slab force
        case 19:  //force z
        case 7:  //displacement
        case 27:  //displacement z
            if (0 != linia_wybrana(v))
            {
                fun1L(x, y, v);
                if (punkt_w_prostokacie(*x, *y)) return 1;
            }


            if ((v->style==4) || (v->style==18)) _precision_=force_precision;
            else _precision_=displacement_precision;

            parametry_lini((LINIA*)v, &PL);

            kos1=sin(Angle_Normal((PL.kat-90)*Pi/180));
            koc1=cos(Angle_Normal((PL.kat-90)*Pi/180));

            if (v->x1<=v->x2) n=1; else n=-1;

            Vtxt.x=(v->x1+v->x2)/2-((n*ra/4)*koc1);
            Vtxt.y=(v->y1+v->y2)/2-((n*ra/4)*kos1);

            set_decimal_format(Vtxt.text, v->magnitude1, _precision_);

            Vtxt.kat=normalize_txt_angle(PL.kat*Pi/180);
            Vtxt.justowanie=j_srodkowo;

            if (tekst_wybrany(&Vtxt))
            {
                *x=v->x2; *y=v->y2;
                if (global_set_stretch) set_stretch_vector(1, adr);
                return 1;
            }

            Vtxt.wysokosc=zmwym.wysokosc/2;
            Vtxt.x=(v->x1+v->x2)/2+n*((Vtxt.wysokosc+ra/4)*koc1);
            Vtxt.y=(v->y1+v->y2)/2+n*((Vtxt.wysokosc+ra/4)*kos1);


            if ((v->style==4) || (v->style==18))   //displacement doesn't have any load type
            {
                if (v->variant > 0)
                    sprintf(&Vtxt.text, "%s%d", load_symbol[(int) v->load], v->variant);
                else sprintf(&Vtxt.text, "%s", load_symbol[(int) v->load]);
                normalize_txt(&Vtxt);

                if (tekst_wybrany(&Vtxt)) {
                    if (global_set_stretch) set_stretch_vector(9, adr); //load character edit
                    *x = Vtxt.x;
                    *y = Vtxt.y;
                    return 1;
                }
            }

            break;
        case 5:  //moment
        case 6:  //-moment
        case 8:  //rotation
        case 9:  //rotation
            l.warstwa=v->warstwa;
            l.x=v->x1;
            l.y=v->y1;
            l.r=v->r;
            l.kat1=v->angle1;
            l.kat2=v->angle2;
            if (punkt_w_prostokacie(l.x, l.y))
            {
                *x=v->x1;
                *y=v->y1;
                return 1;
            }
            if (0 != luk_wybrany(&l))
            {
                fun1l(x, y, &l);
                switch (v->style)
                {
                    //reverse
                    case 6:
                    case 9:
                        if (global_set_stretch) {
                            if (stretch_vector == 2)
                                stretch_vector = 3;  //stretching radius of moment or rotation to stretching angle and value of moment or rotation
                            else stretch_vector = 2; //stretching angle and value of moment or rotation to stretching radius of moment or rotation
                        }
                        break;
                }
                if (punkt_w_prostokacie(*x, *y)) return 1;
            }

            if (v->style<8) _precision_=moment_precision;
            else _precision_=rotation_precision;

            if (v->angle2<v->angle1)
                kata2=v->angle2+Pi2;
            else kata2=v->angle2;
            kats=Angle_Normal((v->angle1+kata2)/2);

            Vtxt.x=v->x1+(v->r+0.5)*cos(kats);
            Vtxt.y=v->y1+(v->r+0.5)*sin(kats);
            Vtxt.kat=Angle_Normal(kats-Pi_/2);
            Vtxt.justowanie=j_srodkowo;
            set_decimal_format(Vtxt.text, v->magnitude1, _precision_);
            normalize_txt(&Vtxt);

            if (tekst_wybrany(&Vtxt))
            {
                if ((v->style==5) || (v->style==8))
                {
                    *x = v->x1 + v->r * cos(v->angle2);
                    *y = v->y1 + v->r * sin(v->angle2);
                }
                else
                {
                    *x = v->x1 + v->r * cos(v->angle1);
                    *y = v->y1 + v->r * sin(v->angle1);
                }

                if (global_set_stretch) set_stretch_vector(3, adr);
                if (punkt_w_prostokacie(*x, *y)) return 1;
            }

            Vtxt.wysokosc=zmwym.wysokosc/2;
            Vtxt.x=v->x1+(v->r - Vtxt.wysokosc - 0.5)*cos(kats);
            Vtxt.y=v->y1+(v->r - Vtxt.wysokosc - 0.5)*sin(kats);

            if ((v->style==5) || (v->style==6)) //displacement doesn't have any load type
            {
                if (v->variant > 0)
                    sprintf(&Vtxt.text, "%s%d", load_symbol[(int) v->load], v->variant);
                else sprintf(&Vtxt.text, "%s", load_symbol[(int) v->load]);
                normalize_txt(&Vtxt);

                if (tekst_wybrany(&Vtxt)) {
                    if (global_set_stretch) set_stretch_vector(9, adr); //load character edit
                    *x = Vtxt.x;
                    *y = Vtxt.y;
                    return 1;
                }
            }
            break;

        case 21:  //moment x //TO DO ISOMETRIC
        case 22:  //-moment x
        case 23:  //moment y
        case 24:  //-moment y
        case 25:  //moment xy
        case 26:  //-moment xy
        case 28:  //rotation x
        case 29:  //rotation x
        case 30:  //rotation y
        case 31:  //rotation y
        case 32:  //rotation xy
        case 33:  //rotation xy

            switch (v->style) {
                case 21:  // moment x //TO DO ISOMETRIC
                    plane = XZ_PLANE;
                    reverse=0;
                    break;
                case 23:
                    plane = YZ_PLANE;
                    reverse=0;
                    break;
                case 25:
                    plane = XY_PLANE;
                    reverse=0;
                    break;
                case 22:  // moment x //TO DO ISOMETRIC
                    plane = XZ_PLANE;
                    reverse=1;
                    break;
                case 24:
                    plane = YZ_PLANE;
                    reverse=1;
                    break;
                case 26:
                    plane = XY_PLANE;
                    reverse=1;
                    break;
                case 28:  // moment x //TO DO ISOMETRIC
                    plane = XZ_PLANE;
                    reverse=0;
                    break;
                case 30:
                    plane = YZ_PLANE;
                    reverse=1;
                    break;
                case 32:
                    plane = XY_PLANE;
                    reverse=0;
                    break;
                case 29:  // moment x //TO DO ISOMETRIC
                    plane = XZ_PLANE;
                    reverse=1;
                    break;
                case 31:
                    plane = YZ_PLANE;
                    reverse=0;
                    break;
                case 33:
                    plane = XY_PLANE;
                    reverse=1;
                    break;
            }

            l.warstwa=v->warstwa;
            l.x=v->x1;
            l.y=v->y1;
            l.r=v->r;
            l.kat1=v->angle1;
            l.kat2=v->angle2;

            if (Check_if_Equal(v->angle1, v->angle2))
            {
                kats=Angle_Normal(v->angle1);
                x1=v->x1+v->r*cos(kats);
                y1=v->y1+v->r*sin(kats);
                tangent=kats - Pi_ / 2.;
                if (tangent < 0) tangent += 2.0 * M_PI;
                ea.x=l.x;
                ea.y=l.y;
                ea.rx=ea.ry=l.r;
                ea.angle=0.;
                ea.kat1=l.kat1;
                ea.kat2=l.kat2;
            }
            else {
                ret = arc_to_isometric_ellipticalarc_a_ea(plane, &l, &ea);
                srodekea_(&x1, &y1, &tangent, &ea);
                tangent = fmod(tangent + M_PI, 2.0 * M_PI);
                kats = tangent + Pi_ / 2.;
                if (kats < 0) kats += 2.0 * M_PI;
            }

            ea.warstwa=v->warstwa;
            ea.atrybut=v->atrybut;

            if (punkt_w_prostokacie(l.x, l.y))
            {
                *x=v->x1;
                *y=v->y1;
                return 1;
            }
            if (0 != (lukeliptyczny_wybrany_prec(&ea)))
            {
                //fun1l(x, y, &l); ////it's not arc anymore
                koniec1el_(x, y, &ea);
                switch (v->style)
                {
                    //reverse
                    case 6:
                    case 22:
                    case 24:
                    case 26:
                    case 9:
                    case 29:
                    case 31:
                    case 33:
                        if (global_set_stretch) {
                            if (stretch_vector == 2)
                                stretch_vector = 3;  //stretching radius of moment or rotation to stretching angle and value of moment or rotation
                            else stretch_vector = 2; //stretching angle and value of moment or rotation to stretching radius of moment or rotation
                        }
                        break;
                }
                if (punkt_w_prostokacie(*x, *y)) return 1;
            }

            if (v->style<28) _precision_=moment_precision;
            else _precision_=rotation_precision;

            Vtxt.x=(float)(x1+0.5*cos(kats));
            Vtxt.y=(float)(y1+0.5*sin(kats));

            Vtxt.kat=(float)tangent;
            Vtxt.justowanie=j_srodkowo;
            set_decimal_format(Vtxt.text, v->magnitude1, _precision_);
            normalize_txt(&Vtxt);

            if (tekst_wybrany(&Vtxt))
            {
                if (!reverse)
                {
                    *x = v->x1 + v->r * cos(v->angle2);
                    *y = v->y1 + v->r * sin(v->angle2);
                }
                else
                {
                    *x = v->x1 + v->r * cos(v->angle1);
                    *y = v->y1 + v->r * sin(v->angle1);
                }

                if (global_set_stretch) set_stretch_vector(3, adr);
                if (punkt_w_prostokacie(*x, *y)) return 1;
            }

            Vtxt.wysokosc=zmwym.wysokosc/2;
            Vtxt.x=(float)(x1+(-Vtxt.wysokosc - 0.5)*cos(kats));
            Vtxt.y=(float)(y1+(-Vtxt.wysokosc - 0.5)*sin(kats));

            if (v->style<28)//displacement doesn't have any load type
            {
                if (v->variant > 0)
                    sprintf(&Vtxt.text, "%s%d", load_symbol[(int) v->load], v->variant);
                else sprintf(&Vtxt.text, "%s", load_symbol[(int) v->load]);
                normalize_txt(&Vtxt);

                if (tekst_wybrany(&Vtxt)) {
                    if (global_set_stretch) set_stretch_vector(9, adr); //load character edit
                    *x = Vtxt.x;
                    *y = Vtxt.y;
                    return 1;
                }
            }

            break;

        case 15: //trapezium T
            //first load and variant is checked
            parametry_lini((LINIA*)v, &PL);

            kos1=sin(Pi*(PL.kat+90)/180);
            koc1=cos(Pi*(PL.kat+90)/180);

            Lth.x1 = (v->x1+v->x2)/2 + (v->r/depth_magnitude)*koc1; //thermal_depth_size
            Lth.y1 = (v->y1+v->y2)/2 + (v->r/depth_magnitude)*kos1;
            Lth.x2 = (v->x1+v->x2)/2 - (v->r/depth_magnitude)*koc1;
            Lth.y2 = (v->y1+v->y2)/2 - (v->r/depth_magnitude)*kos1;

            parametry_lini(&Lth, &PLth);
            kos1th=sin(Pi*(PLth.kat+90)/180);
            koc1th=cos(Pi*(PLth.kat+90)/180);

            Lt.x1 = Lth.x1 + (v->magnitude1/thermal_magnitude)*koc1th;
            Lt.y1 = Lth.y1 + (v->magnitude1/thermal_magnitude)*kos1th;
            Lt.x2 = Lth.x2 + (v->magnitude2/thermal_magnitude)*koc1th;
            Lt.y2 = Lth.y2 + (v->magnitude2/thermal_magnitude)*kos1th;

            Vtxt.justowanie=j_srodkowo;
            Vtxt.wysokosc=zmwym.wysokosc/2;

            if (v->variant>0)
                sprintf(&Vtxt.text, "%s%d",load_symbol[(int)v->load], v->variant);
            else sprintf(&Vtxt.text, "%s",load_symbol[(int)v->load]);

            if (vector_text_wybrany(&Lt, &Vtxt, v))
            {
                if (global_set_stretch) set_stretch_vector(9, v); //load character edit
                *x=Vtxt.x; *y=Vtxt.y;
                return 1;
            }

            Vtxt.wysokosc=zmwym.wysokosc;
        case 10: //trapezium Y
        case 17: //trapezium Y slab
        case 20: //trapezium Z
        case 11: //trapezium X
        case 12: //trapezium N
        case 13:  //trapezium H
        case 14: //trapezium V
             if (0 != linia_wybrana(v))
            {
                funL(x, y, v);
                if (punkt_w_prostokacie(*x, *y))
                {
                    if (global_set_stretch) set_stretch_vector(-1, v);
                    return 1;
                }
                else
                {
                    if (global_set_stretch) set_stretch_vector(-1, v);
                    return 0;
                }
            }
            else
            {
                switch (v->style)
                {  case 10: //trapezium Y
                   case 17: //trapezium Y slab
                   case 20: //trapezium Z
                        LL.x1=v->x1;
                        LL.x2=v->x2;
                        LL.y1=v->y1;
                        LL.y2=v->y2;

                        if (Check_if_Equal(LL.x1, LL.x2))
                        {
                            LL.x2+=0.001;
                        }

                        if (LL.x1 < LL.x2) n = 1;
                        else n = -1;

                        if (v->flags & 1) n*=-1;

                        if ((!(v->cartflags & 1)) || (v->style!=10))
                        {
                            Lt.x1 = LL.x1;
                            Lt.y1 = LL.y1 + n * (v->magnitude1 / ((v->style == 10) ? load_magnitude : flood_magnitude));
                            Lt.x2 = LL.x2;
                            Lt.y2 = LL.y2 + n * (v->magnitude2 / ((v->style == 10) ? load_magnitude : flood_magnitude));
                        }
                        else
                        {
                            double dx1_cart, dy1_cart, dx2_cart, dy2_cart;
                            ret = isometric_vector_to_cartesian(0,  n * (v->magnitude1 / ((v->style == 10) ? load_magnitude : flood_magnitude)),  &dx1_cart, &dy1_cart);
                            ret = isometric_vector_to_cartesian(0,  n * (v->magnitude2 / ((v->style == 10) ? load_magnitude : flood_magnitude)),  &dx2_cart, &dy2_cart);
                            Lt.x1 = LL.x1 + dx1_cart;
                            Lt.y1 = LL.y1 + dy1_cart;
                            Lt.x2 = LL.x2 + dx2_cart;
                            Lt.y2 = LL.y2 + dy2_cart;
                        }

                        Lp.x1=LL.x1;
                        Lp.y1=LL.y1;
                        Lp.x2=LL.x2;
                        Lp.y2=LL.y2;

                    break;
                    case 11: //trapezium X
                        LL.x1=v->x1;
                        LL.x2=v->x2;
                        LL.y1=v->y1;
                        LL.y2=v->y2;
                        if (Check_if_Equal(LL.y1, LL.y2))
                        {
                            LL.y2+=0.001;
                        }

                        if (LL.y1<LL.y2) n=1;
                        else n=-1;

                        if (v->flags & 1) n*=-1;

                        if (!(v->cartflags & 1))
                        {
                            Lt.x1 = LL.x1 - n * (v->magnitude1 / load_magnitude);
                            Lt.y1 = LL.y1;
                            Lt.x2 = LL.x2 - n * (v->magnitude2 / load_magnitude);
                            Lt.y2 = LL.y2;
                        }
                        else
                        {
                            double dx1_cart, dy1_cart, dx2_cart, dy2_cart;
                            ret = isometric_vector_to_cartesian(- n * (v->magnitude1 / load_magnitude), 0, &dx1_cart, &dy1_cart);
                            ret = isometric_vector_to_cartesian(- n * (v->magnitude2 / load_magnitude), 0, &dx2_cart, &dy2_cart);
                            Lt.x1 = LL.x1 + dx1_cart;
                            Lt.y1 = LL.y1 + dy1_cart;
                            Lt.x2 = LL.x2 + dx2_cart;
                            Lt.y2 = LL.y2 + dy2_cart;
                        }

                        Lp.x1=LL.x1;
                        Lp.y1=LL.y1;
                        Lp.x2=LL.x2;
                        Lp.y2=LL.y2;

                    break;
                    case 12: //trapezium N
                        LL.x1=v->x1;
                        LL.x2=v->x2;
                        LL.y1=v->y1;
                        LL.y2=v->y2;

                        parametry_lini((LINIA*)v, &PL);

                        if (!(v->cartflags & 1))
                        {
                            arrow_angle = M_PI * (PL.kat + 90.0) / 180.0;
                            kos1 = sin(arrow_angle);
                            koc1 = cos(arrow_angle);
                        }
                        else {
                            double iso_angle = cartesian_angle_to_isometric_angle(M_PI * PL.kat / 180);
                            // Add 90° in isometric space (counter-clockwise perpendicular)
                            double iso_perp = fmod(iso_angle + M_PI / 2.0, 2.0 * M_PI);
                            if (iso_perp < 0.0) iso_perp += 2.0 * M_PI;
                            arrow_angle = isometric_angle_to_cartesian_angle_rad(iso_perp);
                            kos1 = sin(arrow_angle);
                            koc1 = cos(arrow_angle);
                        }

                        n=1;
                        if (v->flags & 1) n*=-1;

                        Lt.x1 = LL.x1 + n*(v->magnitude1/load_magnitude)*koc1;
                        Lt.y1 = LL.y1 + n*(v->magnitude1/load_magnitude)*kos1;
                        Lt.x2 = LL.x2 + n*(v->magnitude2/load_magnitude)*koc1;
                        Lt.y2 = LL.y2 + n*(v->magnitude2/load_magnitude)*kos1;

                        Lp.x1=LL.x1;
                        Lp.y1=LL.y1;
                        Lp.x2=LL.x2;
                        Lp.y2=LL.y2;
                    break;
                    case 13:  //trapezium H
                        LL.x1=v->x1;
                        LL.x2=v->x2;
                        LL.y1=v->y1;
                        LL.y2=v->y2;

                        if (!(v->cartflags & 1))
                        {
                            if (LL.x1 < LL.x2) {
                                n = 1;
                                ymax = max(LL.y1, LL.y2);
                            } else {
                                n = -1;
                                ymax = min(LL.y1, LL.y2);
                            }

                            if (v->flags & 1) n *= -1;

                            Lt.x1 = LL.x1;
                            Lt.y1 = ymax + n * (v->magnitude1 / load_magnitude);
                            Lt.x2 = LL.x2;
                            Lt.y2 = ymax + n * (v->magnitude2 / load_magnitude);

                            //trapezoid base
                            Lp.x1=LL.x1;
                            Lp.y1=ymax;
                            Lp.x2=LL.x2;
                            Lp.y2=ymax;
                        }
                        else
                        {
                            //projection on x axis
                            double iso_L1_x1, iso_L1_y1, iso_L1_x2, iso_L1_y2;
                            ret=cartesian_to_isometric(LL.x1, LL.y1, &iso_L1_x1, &iso_L1_y1);
                            ret=cartesian_to_isometric(LL.x2, LL.y2, &iso_L1_x2, &iso_L1_y2);

                            if (iso_L1_x1<iso_L1_x2) n=1;
                            else n=-1;

                            ret = trapezoid_base_isometric_x(LL.x1, LL.y1, LL.x2, LL.y2,&Lp.x1, &Lp.y1, &Lp.x2, &Lp.y2);

                            double dx1_cart, dy1_cart, dx2_cart, dy2_cart;
                            ret = isometric_vector_to_cartesian(0.,  n * (v->magnitude1 / load_magnitude),  &dx1_cart, &dy1_cart);
                            ret = isometric_vector_to_cartesian(0.,  n * (v->magnitude2 / load_magnitude),  &dx2_cart, &dy2_cart);
                            Lt.x1 = Lp.x1 + (float)dx1_cart;
                            Lt.y1 = Lp.y1 + (float)dy1_cart;
                            Lt.x2 = Lp.x2 + (float)dx2_cart;
                            Lt.y2 = Lp.y2 + (float)dy2_cart;
                        }
                    break;
                    case 14: //trapezium V
                        LL.x1=v->x1;
                        LL.x2=v->x2;
                        LL.y1=v->y1;
                        LL.y2=v->y2;

                        if (!(v->cartflags & 1))
                        {
                            if (LL.y1<LL.y2)
                            {
                                n=1;
                                xmax=min(LL.x1, LL.x2);
                            }
                            else
                            {
                                n=-1;
                                xmax=max(LL.x1, LL.x2);
                            }

                            if (v->flags & 1) n*=-1;

                            Lt.x1 = xmax - n * (v->magnitude1 / load_magnitude);
                            Lt.y1 = LL.y1;
                            Lt.x2 = xmax - n * (v->magnitude2 / load_magnitude);
                            Lt.y2 = LL.y2;

                            //trapezoid base
                            Lp.x1=xmax;
                            Lp.y1=LL.y1;
                            Lp.x2=xmax;
                            Lp.y2=LL.y2;
                        }
                        else
                        {
                            //projection on x axis
                            double iso_L1_x1, iso_L1_y1, iso_L1_x2, iso_L1_y2;
                            ret=cartesian_to_isometric(LL.x1, LL.y1, &iso_L1_x1, &iso_L1_y1);
                            ret=cartesian_to_isometric(LL.x2, LL.y2, &iso_L1_x2, &iso_L1_y2);

                            if (iso_L1_y1<iso_L1_y2) n=1;
                            else n=-1;

                            ret = trapezoid_base_isometric_y(LL.x1, LL.y1, LL.x2, LL.y2,&Lp.x1, &Lp.y1, &Lp.x2, &Lp.y2);

                            double dx1_cart, dy1_cart, dx2_cart, dy2_cart;
                            ret = isometric_vector_to_cartesian(-n * (v->magnitude1 / load_magnitude), 0., &dx1_cart, &dy1_cart);
                            ret = isometric_vector_to_cartesian(-n * (v->magnitude2 / load_magnitude), 0., &dx2_cart, &dy2_cart);
                            Lt.x1 = Lp.x1 + (float)dx1_cart;
                            Lt.y1 = Lp.y1 + (float)dy1_cart;
                            Lt.x2 = Lp.x2 + (float)dx2_cart;
                            Lt.y2 = Lp.y2 + (float)dy2_cart;
                        }
                    break;
                    case 15: //thermal

                        /*    //is already set
                        */

                        break;
                }

                if (0 != linia_wybrana(&Lt))
                {
                    ret= koniecL_factory(x, y, &Lt);
                    if (ret==1)
                    {
                        if (global_set_stretch) set_stretch_vector(4, v); //magnitude1
                    }
                    else if (ret==2)
                    {
                        if (global_set_stretch) set_stretch_vector(5, v); //magnitude2
                    }
                    if (punkt_w_prostokacie(*x, *y)) return 1;
                }
                else if ((v->style==15) && (0 != linia_wybrana(&Lth)))
                {
                    ret= koniecL_factory(x, y, &Lth);
                    if (ret==1)
                    {
                        if (global_set_stretch) set_stretch_vector(6, v); //thermal bar size side 1
                    }
                    else if (ret==2)
                    {
                        if (global_set_stretch) set_stretch_vector(7, v); //thermal bar size side 2
                    }
                    if (punkt_w_prostokacie(*x, *y)) return 1;
                }

                set_decimal_format(Vtxt.text, v->magnitude1, load_precision);
                set_decimal_format(Vtxt1.text, v->magnitude2, load_precision);
                int ret=vector_magnitude_text_wybrany(&Vtxt, &Vtxt1, v, &LL);
                if (ret==1)
                {
                    if (global_set_stretch) set_stretch_vector(4, v); //magnitude1 edit
                    *x=Lt.x1; *y=Lt.y1;
                    return 1;
                }
                else if (ret==2)
                {
                    if (global_set_stretch) set_stretch_vector(5, v); //magnitude2 edit
                    *x=Lt.x2; *y=Lt.y2;
                    return 1;
                }

                    Vtxt.justowanie=j_srodkowo;
                    Vtxt.wysokosc = zmwym.wysokosc / 2;

                    if (v->variant > 0)
                        sprintf(&Vtxt.text, "%s%d", load_symbol[(int) v->load], v->variant);
                    else sprintf(&Vtxt.text, "%s", load_symbol[(int) v->load]);

                    if (vector_text_wybrany(&Lt, &Vtxt, v)) {
                        if (global_set_stretch) set_stretch_vector(9, v); //load character edit
                        *x = Vtxt.x;
                        *y = Vtxt.y;
                        return 1;
                    }

                if (v->style==15) {
                    Lt1.x1 = Lt.x1;
                    Lt1.y1 = Lt.y1;
                    Lt1.x2 = Lth.x1;
                    Lt1.y2 = Lth.y1;
                    Lt2.x1 = Lt.x2;
                    Lt2.y1 = Lt.y2;
                    Lt2.x2 = Lth.x2;
                    Lt2.y2 = Lth.y2;
                }
                else
                {
                    Lt1.x1 = Lt.x1;
                    Lt1.y1 = Lt.y1;
                    Lt1.x2 = Lp.x1; //v->x1;
                    Lt1.y2 = Lp.y1; //v->y1;
                    Lt2.x1 = Lt.x2;
                    Lt2.y1 = Lt.y2;
                    Lt2.x2 = Lp.x2; //v->x2;
                    Lt2.y2 = Lp.y2; //v->y2;
                }
                if ((0 != linia_wybrana(&Lt1)) || (0 != linia_wybrana(&Lt2)))
                {
                    if (global_set_stretch) set_stretch_vector(9, v); //load character edit
                    *x=Vtxt.x; *y=Vtxt.y;
                    return 1;
                }

                return -1;
            }
            break;

        case 16:
            if (0 != linia_wybrana(v))
            {
                funL(x, y, v);
                if (punkt_w_prostokacie(*x, *y)) return 1;
                else return 0;
            }
            else
            {
                O.warstwa = v->warstwa;
                O.x = v->x1;
                O.y = v->y1;
                O.r = v->r;
                if (punkt_w_prostokacie(O.x, O.y)) {
                    *x = v->x1;
                    *y = v->y1;
                    return 1;
                }
                if (0 != okrag_wybrany(&O))
                {
                    fun1l(x, y, &O);
                    if (punkt_w_prostokacie(*x, *y)) return 1;
                }

                Vtxt.wysokosc=zmwym.wysokosc*0.75;
                Vtxt.x=v->x2;
                Vtxt.y=v->y2;
                Vtxt.kat=0;

                Vtxt.justowanie=j_do_lewej;

                set_decimal_format(Vtxt1.text, v->magnitude1, dim_precision);
                sprintf(Vtxt.text,"R%s",Vtxt1.text);

                double t_len_mm = Get_TextLen(&Vtxt, Vtxt.text);
                double direction=v->x2-v->x1;

                Lt.warstwa=v->warstwa;
                Lt.x1=v->x2;
                Lt.y1=v->y2;
                Lt.y2=Lt.y1;

                if (direction > 0) //to the right
                {
                    Vtxt.x = v->x2 + Vtxt.wysokosc / 4.0;
                    Vtxt.y = v->y2 + Vtxt.wysokosc / 4.0;
                    Vtxt.justowanie = j_do_lewej;

                    if (t_len_mm == 0.0) Lt.x2 = v->x2 + 0.01;
                    else Lt.x2 = v->x2 + t_len_mm + Vtxt.wysokosc / 2.5;
                } else  //to the left
                {
                    Vtxt.x = v->x2 - Vtxt.wysokosc / 4.0;
                    Vtxt.y = v->y2 + Vtxt.wysokosc / 4.0;
                    Vtxt.justowanie = j_do_prawej;
                    if (t_len_mm == 0.0) Lt.x2 = v->x2 - 0.01;
                    else Lt.x2 = v->x2 - t_len_mm - Vtxt.wysokosc / 2.0;
                }
                if (tekst_wybrany(&Vtxt))
                {
                    *x=v->x1; *y=v->y1;
                    if (global_set_stretch) set_stretch_vector(8, v);
                    return 1;
                }

                if (0 != linia_wybrana(&Lt))
                {
                    *x=v->x2; *y=v->y2;
                    if (global_set_stretch) set_stretch_vector(0, v);
                    return 0;
                }
            }
        break;
        default:
            if (0 != linia_wybrana(v))
            {
                funL(x, y, v);
                if (punkt_w_prostokacie(*x, *y)) return 1;
            }
            else return -1;
            break;
    }
    return 0;
}


int mwspline_(double* x, double* y, void* adr, int(*fun)(double*, double*, void*))
    /*--------------------------------------------------------------------------*/
{
    LINIA L = Ldef;
    SPLINE* s;
    int i, j;
    float out_x[MaxNpts*3], out_y[MaxNpts*3];
    SPLINE tmp_spline = Splinedef;
    double bnodes_2_tangent_org, bnodes_2_tangent;
    int node_count;
    int found=0;
    double dl;
    double mindl=MAXDOUBLE;
    double x0, y0, x1, y1;

    s = adr;
    x0=*x;
    y0=*y;
    
#define Npts (90*3)  //for much more precission
    if (s->npts < 5)
    {
        calc_bspline(s->lp, s->npts, s->xy, Npts, &out_x, &out_y);
        for (i = 0; i < Npts - 1; i++)
        {
            L.x1 = out_x[i];
            L.y1 = out_y[i];
            L.x2 = out_x[i + 1];
            L.y2 = out_y[i + 1];
            if (fun(x, y, &L))
            {
                found++;
                dl = sqrt((*x - x0) * (*x - x0) + (*y - y0) * (*y - y0));
                if (dl<mindl)
                {
                    mindl=dl;
                    x1=*x, y1=*y;
                }
            }
        }
    }
    else
    {
        curviness = s->xy[s->lp];

        tmp_spline.npts = 4;
        tmp_spline.lp = 8;
        tmp_spline.kolor = s->kolor;
        tmp_spline.typ = s->typ;

        node_count = 1;

        set_bnodes(s, s->xy, &node_count);

        calc_tangents(s->closed, &node_count, &bnodes_2_tangent);

        if (s->closed == 0)
        {
            bnodes_2_tangent_org = bnodes[2].tangent;
            bnodes[2].tangent = bnodes_2_tangent;
            get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
            calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
            for (i = 0; i < Npts - 1; i++)
            {
                L.x1 = out_x[i];
                L.y1 = out_y[i];
                L.x2 = out_x[i + 1];
                L.y2 = out_y[i + 1];

                    if (fun(x, y, &L))
                    {
                        found++;
                        dl = sqrt((*x - x0) * (*x - x0) + (*y - y0) * (*y - y0));
                        if (dl<mindl)
                        {
                            mindl=dl;
                            x1=*x, y1=*y;
                        }
                    }

            }
            bnodes[2].tangent = bnodes_2_tangent_org;
        }

        for (j = 2; j < node_count - 2; j++)  //1 - extra_node
        {
            get_control_points(bnodes[j], bnodes[j + 1], tmp_spline.xy);
            calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
            for (i = 0; i < Npts - 1; i++)
            {
                L.x1 = out_x[i];
                L.y1 = out_y[i];
                L.x2 = out_x[i + 1];
                L.y2 = out_y[i + 1];

                    if (fun(x, y, &L))
                    {
                        found++;
                        dl = sqrt((*x - x0) * (*x - x0) + (*y - y0) * (*y - y0));
                        if (dl<mindl)
                        {
                            mindl=dl;
                            x1=*x, y1=*y;
                        }
                    }

            }
        }

        if ((node_count > 3) && (s->closed == 0))
        {
            get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
            calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
            for (i = 0; i < Npts - 1; i++)
            {
                L.x1 = out_x[i];
                L.y1 = out_y[i];
                L.x2 = out_x[i + 1];
                L.y2 = out_y[i + 1];

                    if (fun(x, y, &L))
                    {
                        found++;
                        dl = sqrt((*x - x0) * (*x - x0) + (*y - y0) * (*y - y0));
                        if (dl<mindl)
                        {
                            mindl=dl;
                            x1=*x, y1=*y;
                        }
                    }
            }
        }

    }
    if (found>0)
    {
        *x=x1;
        *y=y1;
        return 1;
    }
    return 0;
}

int mwellipse_(double* x, double* y, void* adr, int(*fun)(double*, double*, void*))
/*--------------------------------------------------------------------------*/
{
    LINIA L = Ldef;
    ELLIPSE* e;
    int i, j;
    int numpoints;
    double xy[512];
    int found=0;
    double dl;
    double mindl=MAXDOUBLE;
    double x0, y0, x1, y1;
    int verno;

    verno = Get_Ell_Verno () ;

    e = adr;
    x0=*x;
    y0=*y;

    Set_Ell_Verno (250) ;
    numpoints=Get_Ellipse_Points(e->x, e->y, e->rx, e->ry, e->angle, xy);
    Set_Ell_Verno (verno) ;

    xy[numpoints] = xy[0];
    xy[numpoints+1] = xy[1];
    numpoints+=2;

    for (i = 0; i < (numpoints - 2); i+=2)
    {
        L.x1 = xy[i];
        L.y1 = xy[i+1];
        L.x2 = xy[i + 2];
        L.y2 = xy[i + 3];

        if (fun(x, y, &L))
        {
           found++;
           dl = sqrt((*x - x0) * (*x - x0) + (*y - y0) * (*y - y0));
           if (dl<mindl)
           {
               mindl=dl;
               x1=*x, y1=*y;
           }
        }
    }
    if (found>0)
    {
        *x=x1;
        *y=y1;
        return 1;
    }

    return 0;
}

int mwellipticalarc_(double* x, double* y, void* adr, int(*fun)(double*, double*, void*))
/*--------------------------------------------------------------------------*/
{
    LINIA L = Ldef;
    ELLIPTICALARC* ea;
    int i, j;
    int numpoints;
    double xy[512];
    int found=0;
    double dl;
    double mindl=MAXDOUBLE;
    double x0, y0, x1, y1;
    int verno;

    verno = Get_Ell_Verno () ;

    ea = adr;
    x0=*x;
    y0=*y;

    Set_Ell_Verno (250) ;
    numpoints=Get_EllipticalArc_Points(ea->x, ea->y, ea->rx, ea->ry, ea->angle, ea->kat1, ea->kat2, xy);
    Set_Ell_Verno (verno) ;

    for (i = 0; i < numpoints - 2; i+=2)
    {
        L.x1 = xy[i];
        L.y1 = xy[i+1];
        L.x2 = xy[i + 2];
        L.y2 = xy[i + 3];

        if (fun(x, y, &L))
        {
            found++;
            dl = sqrt((*x - x0) * (*x - x0) + (*y - y0) * (*y - y0));
            if (dl<mindl)
            {
                mindl=dl;
                x1=*x, y1=*y;
            }
        }

    }

    if (found>0)
    {
        *x=x1;
        *y=y1;
        return 1;
    }

    return 0;
}

void koniecw_ (double *x, double *y, void *adr)
/*-------------------------------------------*/
{
  LINIA L = Ldef ;
  WIELOKAT *w ;

  int i;
  w = adr ;
  for (i = 0 ; i < w->lp - 2 ; i += 2)
  {
    L.x1 = w->xy [i + 0] ;
    L.y1 = w->xy [i + 1] ;
    L.x2 = w->xy [i + 2] ;
    L.y2 = w->xy [i + 3] ;
    if (0 != linia_wybrana (&L))
    {
      koniecL_ (x, y, &L) ;
      return ;
    }
  }
  L.x1 = w->xy [0] ;
  L.y1 = w->xy [1] ;
  L.x2 = w->xy [w->lp-2] ;
  L.y2 = w->xy [w->lp-1] ;
  if (0 != linia_wybrana (&L))
  {
    koniecL_ (x, y, &L) ;
    return ;
  }
}

void koniecs_(double *x, double *y, void *adr)
/*-------------------------------------------*/
{
	LINIA L = Ldef;
	SPLINE *s;

	int i;
	s = adr;
	
	L.x1 = s->xy[0];
	L.y1 = s->xy[1];
	L.x2 = s->xy[(s->lp)-2];  //6
	L.y2 = s->xy[(s->lp)-1];  //7
	if (0 != linia_wybrana(&L))
	{
		koniecL_(x, y, &L);
		return;
	}
	
}


int przeciecieLL_(double *x, double *y, void *adr, void *adr1)
/*-----------------------------------------------------------*/
{
  double t1, t2, l ;

  if (0 == przeciecieLL_tt(x, y, adr, adr1, &t1, &t2))
  {
    return 0;
  }
  l = (t1 >= 0-o_male_p && t1 <= 1+o_male_p && t2 >= 0-o_male_p && t2 <= 1+o_male_p) ? 1 : 0 ;
  return l ;
}


double dP1P2 (double x1,double y1, double x2, double y2)
{ return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ); }



int przeciecieLO_older (double *x, double *y, void *adr, void *adr1)
/*-----------------------------------------------------------*/
{
  double x1, x2, y1, y2, t1, t2, t0, x0, y0, l ;

  if (((NAGLOWEK*)adr)->obiekt==Ookrag) {
      if (0 == przeciecieLO_tt(&x1, &y1, &t1, &x2, &y2, &t2, adr1, adr)) {
          return 0;
      }
  }
  else
  {
      if (0 == przeciecieLO_tt(&x1, &y1, &t1, &x2, &y2, &t2, adr, adr1)) {
          return 0;
      }
  }
  if (dP1P2   (X, Y, x1, y1) <= dP1P2   (X, Y, x2, y2))
  {
    x0 = x1 ;
    y0 = y1 ;
    t0 = t1 ;
 }
  else
  {
    x0 = x2 ;
    y0 = y2 ;
    t0 = t2 ;
  }
  l = 0 ;
  if (t0 >= 0 - o_male_p && t0 <= 1 + o_male_p)
  {
    l = 1 ;
    *x = x0 ;
    *y = y0 ;
  }
  return l ;
}

int przeciecieLO_ (double *x, double *y, void *adr, void *adr1)
/*-----------------------------------------------------------*/
{
    double x1, x2, y1, y2, t1, t2;

    if (((NAGLOWEK*)adr)->obiekt==Ookrag) {
        if (0 == przeciecieLO_tt(&x1, &y1, &t1, &x2, &y2, &t2, adr1, adr)) {
            return 0;
        }
    }
    else {
        if (0 == przeciecieLO_tt(&x1, &y1, &t1, &x2, &y2, &t2, adr, adr1)) {
            return 0;
        }
    }

    //priority for closer point
    if ((dP1P2  (X, Y, x1, y1) <= dP1P2   (X, Y, x2, y2)) && (t1 >= 0 - o_male_p && t1 <= 1 + o_male_p))
    {
        *x = x1 ;
        *y = y1 ;
        return 1;
    }
    else if (t2 >= 0 - o_male_p && t2 <= 1 + o_male_p)
    {
        *x = x2 ;
        *y = y2 ;
        return 1;
    }

    //now no priority
    if (t1 >= 0 - o_male_p && t1 <= 1 + o_male_p)
    {
        *x = x1 ;
        *y = y1 ;
        return 1;
    }
    else if (t2 >= 0 - o_male_p && t2 <= 1 + o_male_p)
    {
        *x = x2 ;
        *y = y2 ;
        return 1;
    }

    return 0 ;
}

int przeciecieLO_r (double *x, double *y, void *adr, void *adr1)
{
    return przeciecieLO_ (x, y, adr1, adr);
}


int przeciecieLl_(double *x, double *y, void *adr, void *adr1)
/*----------------------------------------------------------*/
{ LUK *luk;
  LINIA *L;
  double a,b,c,d2,d,t,t1,t2,x1,x2,y1,y2,x0,y0;
  int l;

  if (((NAGLOWEK*)adr)->obiekt==Oluk)
  {
      L = (LINIA*)adr1;
      luk = (LUK*)adr;
  }
  else {
      L = (LINIA *) adr;
      luk = (LUK *) adr1;
  }
  a=(L->x2-L->x1)*(L->x2-L->x1)+(L->y2-L->y1)*(L->y2-L->y1);
  if (TRUE == Check_if_Equal (a, 0))
  {
    return 0 ;		/*odcinek 'zerowy'*/
  }
  b=2*((L->x1-luk->x)*(L->x2-L->x1)+(L->y1-luk->y)*(L->y2-L->y1));
  c=(L->x1-luk->x)*(L->x1-luk->x) + (L->y1-luk->y)*(L->y1-luk->y)-luk->r*luk->r;
  d2=b*b-4*a*c;
  if (d2<0) return 0;
  d=sqrt(d2);
  t1=(-b+d)/(2*a);
  x1=L->x1+t1*(L->x2-L->x1);
  y1=L->y1+t1*(L->y2-L->y1);
  t2=(-b-d)/(2*a);
  x2=L->x1+t2*(L->x2-L->x1);
  y2=L->y1+t2*(L->y2-L->y1);
  if(dP1P2  (X,Y,x1,y1)<=dP1P2  (X,Y,x2,y2)) { x0=x1; y0=y1;t=t1; }
  else { x0=x2; y0=y2;t=t2; }
  l=(t>=0-o_male_p && t<=1+o_male_p);
  if ( l && Point_Arc ( x0 , y0 , luk ) )
  {
    *x=x0; *y=y0;
    return 1;
  }
  else
   return 0;
}


static void srodekL_(double *x, double *y, void *adr)
{ LINIA  *adrl;
  adrl=(LINIA*)adr;
  *x=adrl->x1+(adrl->x2-adrl->x1)/2;
  *y=adrl->y1+(adrl->y2-adrl->y1)/2;
}

void najblizszyL_ (double *x, double *y, void *adr)
{
  double X01;
  double a, a1, df_min_prec ;
  LINIA  *adrl;
  PLINIA PL;

  adrl=(LINIA*)adr;
  if (TRUE == Check_if_Equal (adrl->x2, adrl->x1))
  {
    *x = adrl->x1 ;
  }
  else
  if (TRUE == Check_if_Equal (adrl->y2, adrl->y1))
  {
    *y = adrl->y1 ;
  }
  else
  {
    parametry_lini ((LINIA*)adrl, &PL) ;
    df_min_prec = Get_Prec (MPREC_DOUBLE, 1, 1) ;
    if (fabs (PL.sin) < df_min_prec)
    {
      PL.sin = df_min_prec ;
    }
    if (fabs (PL.cos) < df_min_prec)
    {
      PL.cos = df_min_prec ;
    }
    a = PL.sin / PL.cos ;
    a1 = 1 / a ;
    X01 = *x ;
    *x = (*y - adrl->y1 + a * adrl->x1 + X01 * a1) / (a + a1) ;
    *y =*y - a1 * (*x - X01) ;
  }
}

int najblizszyLx_(double* x, double* y, void* adr)
{
    double X01;
    double a, a1, df_min_prec;
    LINIA* adrl;
    PLINIA PL;
    double x1, y1;
    double dL, dl;

    adrl = (LINIA*)adr;
    if (TRUE == Check_if_Equal(adrl->x2, adrl->x1))
    {
        dL = fabs(*y - adrl->y1) + fabs(adrl->y2 - *y);
        dl = fabs(adrl->y2 - adrl->y1);
        if (fabs(dL - dl) < 0.01)
        {
            if (0 != punkt_w_prostokacie(adrl->x1, *y))
            {
                *x = adrl->x1;
                return 1;
            }
        }
    }
    else
        if (TRUE == Check_if_Equal(adrl->y2, adrl->y1))
        {
            dL = fabs(*x - adrl->x1) + fabs(adrl->x2 - *x);
            dl = fabs(adrl->x2 - adrl->x1);
            if (fabs(dL - dl) < 0.01)
            {
                if (0 != punkt_w_prostokacie(*x, adrl->y1))
                {
                    *y = adrl->y1;
                    return 1;
                }
            }
        }
        else
        {
            parametry_lini((LINIA*)adrl, &PL);
            df_min_prec = Get_Prec(MPREC_DOUBLE, 1, 1);
            if (fabs(PL.sin) < df_min_prec)
            {
                PL.sin = df_min_prec;
            }
            if (fabs(PL.cos) < df_min_prec)
            {
                PL.cos = df_min_prec;
            }
            a = PL.sin / PL.cos;
            a1 = 1 / a;
            X01 = *x;


            x1 = (*y - adrl->y1 + a * adrl->x1 + X01 * a1) / (a + a1);
            y1 = *y - a1 * (x1 - X01);

            dL = sqrt((x1 - adrl->x1) * (x1 - adrl->x1) + (y1 - adrl->y1) * (y1 - adrl->y1)) +
                sqrt((adrl->x2 - x1) * (adrl->x2 - x1) + (adrl->y2 - y1) * (adrl->y2 - y1));

            dl = PL.dl;

            if (fabs(dL - dl) < 0.01)
            {
                if (0 != punkt_w_prostokacie(x1, y1))
                {
                    *x = x1;
                    *y = y1;
                    return 1;
                }
            }
            
        }

    return 0;
}

int prostopadlew_ (double *x,double *y,void *adr)
{
  if(!poczL) return 0 ;
  wielokat_(x, y, adr, (void (*)(double*, double*, void*))prostopadleL_) ;
  return 1 ;
}

int prostopadleSA_ (double *x,double *y,void *adr)
{
    if(!poczL) return 0 ;
    return wsolidarc_(x, y, adr, (void (*)(double*, double*, void*))prostopadleL_, (void (*)(double*, double*, void*))prostopadlel_) ;
}

int prostopadles_(double* x, double* y, void* adr)
{
    if (!poczL) return 0;
    return mwspline_(x, y, adr, (int (*)(double*, double*, void*))prostopadleLx_);
}

int prostopadlee_(double* x, double* y, void* adr)
{
    if (!poczL) return 0;
    return mwellipse_(x, y, adr, (int (*)(double*, double*, void*))prostopadleLx_);
}

int prostopadleea_(double* x, double* y, void* adr)
{
    if (!poczL) return 0;
    return mwellipticalarc_(x, y, adr, (int (*)(double*, double*, void*))prostopadleLx_);
}


int prostopadleL_(double *x,double *y,void *adr)
{
  double a, a1, df_min_prec ;
  LINIA *adrl, *L ;
  PLINIA PL ;

  if (!poczL) return 0 ;
  L = &LiniaG ;
  adrl = (LINIA*)adr ;
  if (TRUE == Check_if_Equal (adrl->x2, adrl->x1))
  {
    *x = adrl->x1 ;
    *y = L->y1 ;
  }
  else
  if (TRUE == Check_if_Equal (adrl->y2, adrl->y1))
  {
    *y = adrl->y1 ;
    *x = L->x1 ;
  }
  else
  {
    parametry_lini ((LINIA*)adrl, &PL) ;
    df_min_prec = Get_Prec (MPREC_DOUBLE, 1, 1) ;
    if (fabs (PL.sin) < df_min_prec)
    {
      PL.sin = df_min_prec ;
    }
    if (fabs (PL.cos) < df_min_prec)
    {
      PL.cos = df_min_prec ;
    }
    a = PL.sin / PL.cos ;
    a1 = 1 / a ;
    *x = (L->y1 - adrl->y1 + a * adrl->x1 + L->x1 * a1) /(a + a1) ;
    *y = L->y1 - a1 * (*x - L->x1) ;
  }
  return 1;
}

int prostopadleL_from_point(double *x,double *y, double *l, void *adr)
{
    double a, a1, df_min_prec ;
    LINIA *adrl;
    PLINIA PL ;
    double x1, y1;

    x1=*x;
    y1=*y;

    adrl = (LINIA*)adr ;
    if (TRUE == Check_if_Equal (adrl->x2, adrl->x1))
    {
        *x = adrl->x1 ;
        *y = y1 ;
        *l=fabs(adrl->y2-adrl->y1);
    }
    else
    if (TRUE == Check_if_Equal (adrl->y2, adrl->y1))
    {
        *y = adrl->y1 ;
        *x = x1 ;
        *l=fabs(adrl->x2-adrl->x1);
    }
    else
    {
        parametry_lini ((LINIA*)adrl, &PL) ;
        df_min_prec = Get_Prec (MPREC_DOUBLE, 1, 1) ;
        if (fabs (PL.sin) < df_min_prec)
        {
            PL.sin = df_min_prec ;
        }
        if (fabs (PL.cos) < df_min_prec)
        {
            PL.cos = df_min_prec ;
        }
        a = PL.sin / PL.cos ;
        a1 = 1 / a ;
        *x = (y1 - adrl->y1 + a * adrl->x1 + x1 * a1) /(a + a1) ;
        *y = y1 - a1 * (*x - x1) ;
        *l=PL.dl;
    }

    return 1;
}

int prostopadleV_(double *x,double *y,void *adr)
{
    double a, a1, df_min_prec ;
    AVECTOR *adrv;
    LINIA *L ;
    PLINIA PL ;
    OKRAG O;

    if (!poczL) return 0 ;
    L = &LiniaG ;
    adrv = (AVECTOR*)adr ;
    switch (adrv->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        if (TRUE == Check_if_Equal(adrv->x2, adrv->x1)) {
            *x = adrv->x1;
            *y = L->y1;
        } else if (TRUE == Check_if_Equal(adrv->y2, adrv->y1)) {
            *y = adrv->y1;
            *x = L->x1;
        } else {
            parametry_lini((LINIA*)adrv, &PL);
            df_min_prec = Get_Prec(MPREC_DOUBLE, 1, 1);
            if (fabs(PL.sin) < df_min_prec) {
                PL.sin = df_min_prec;
            }
            if (fabs(PL.cos) < df_min_prec) {
                PL.cos = df_min_prec;
            }
            a = PL.sin / PL.cos;
            a1 = 1 / a;
            *x = (L->y1 - adrv->y1 + a * adrv->x1 + L->x1 * a1) / (a + a1);
            *y = L->y1 - a1 * (*x - L->x1);
        }
        return 1;
        break;
        case 16:
            O.x=adrv->x1;
            O.y=adrv->y1;
            O.r=adrv->r;
            return prostopadleO_(x, y, &O);
            break;
        default:
            break;
    }

    return 0;
}

int prostopadleLx_(double* x, double* y, void* adr)
{
    double a, a1, df_min_prec;
    LINIA* adrl, * L;
    PLINIA PL;
    double dL, dl, x1, y1;

    if (!poczL) return 0;
    L = &LiniaG;
    adrl = (LINIA*)adr;

    parametry_lini((LINIA*)adrl, &PL);
    df_min_prec = Get_Prec(MPREC_DOUBLE, 1, 1);
    if (fabs(PL.sin) < df_min_prec)
    {
        PL.sin = df_min_prec;
    }
    if (fabs(PL.cos) < df_min_prec)
    {
        PL.cos = df_min_prec;
    }
    a = PL.sin / PL.cos;
    a1 = 1.0 / a;

    x1 = (L->y1 - adrl->y1 + a * adrl->x1 + L->x1 * a1) / (a + a1);
    y1 = L->y1 - a1 * (x1 - L->x1);


    if (CheckIsPointOnLineSegment(x1, y1, adrl->x1, adrl->y1, adrl->x2, adrl->y2, 0.0001))
    {
            *x = x1;
            *y = y1;
            return 1;
    }

    return 0;
}

void prostopadO(double LX,double LY,double *x, double *y, void *adr)
{ double l,Lx,Ly,si,co,e;
  OKRAG *O;
  O=(OKRAG*)adr;
  if (TRUE == Check_if_Equal (LX, O->x) && TRUE == Check_if_Equal (LY, O->y))
  {
    *x = O->x + O->r ;
    *y = O->y ;
    return ;
  }
  l = sqrt ((LX - O->x) * (LX - O->x) + (LY - O->y) * (LY - O->y)) ;
  Lx = LX - O->x ;
  Ly = LY - O->y ;
  si = Ly / l ;
  co = Lx / l ;
  obru (si, co, X - O->x, Y - O->y, x, y) ;
  e = *x > 0 ? 1 : -1 ;
  obrd (si, co, e*O->r, 0, x, y) ;
  *x += O->x ;
  *y += O->y ;
}

BOOL prostopadE(double LX,double LY,double *x, double *y, void *adr)
{ double l,Lx,Ly,si,co,e;
    BOOL ret;
    ELLIPSE *E;
    E=(ELLIPSE*)adr;

    return nearest_point_on_ellipse(E->x, E->y, E->rx, E->ry, E->angle, 0, Pi2,LX, LY, x, y);

}

BOOL prostopadEA(double LX,double LY,double *x, double *y, void *adr)
{ double l,Lx,Ly,si,co,e;
    BOOL ret;
    ELLIPTICALARC *EA;
    EA=(ELLIPTICALARC *)adr;

    return nearest_point_on_ellipse(EA->x, EA->y, EA->rx, EA->ry, EA->angle, EA->kat1, EA->kat2, LX, LY, x, y);

}


void najblizszyO (double *x, double *y, void *adr)
{
  double l,Lx,Ly,si,co,e;
  OKRAG *O;

  O = (OKRAG*)adr ;
  if (TRUE == Check_if_Equal (*x, O->x) && TRUE == Check_if_Equal (*y, O->y))
  {
    *x = O->x + O->r ;
    *y = O->y ;
    return ;
  }
  l = sqrt ((*x - O->x) * (*x - O->x) + (*y - O->y) * (*y - O->y)) ;
  if (TRUE == Check_if_Equal (l, 0))
  {
    *x = O->x + O->r ;
    *y = O->y ;
    return ;
  }
  Lx = *x - O->x ;
  Ly = *y - O->y ;
  si = Ly / l ;
  co = Lx / l ;
  obru (si , co, *x - O->x, *y - O->y, x, y) ;
  e =*x > 0 ? 1 : -1 ;
  obrd (si, co, e*O->r, 0, x, y) ;
  *x += O->x ;
  *y += O->y ;
}

void najblizszyE (double *x, double *y, void *adr)
{
    double l,Lx,Ly,si,co,e, si1, co1;
    ELLIPSE *E;
    int numpoints;
    double xy[256];
    double Ex, Ey;
    double denom;
    double x1, y1, x2, y2, l1, l2, x_, y_;
    static double df_k;

    x_=*x;
    y_=*y;

    E = (ELLIPSE*)adr ;
    numpoints=Get_Ellipse_Points(E->x, E->y, E->rx, E->ry, E->angle, xy);
    if (TRUE == Check_if_Equal (*x, E->x) && TRUE == Check_if_Equal (*y, E->y))
    {
        *x = xy[0] ;
        *y = xy[1] ;
        return ;
    }
    l = sqrt ((*x - E->x) * (*x - E->x) + (*y - E->y) * (*y - E->y)) ;
    if (TRUE == Check_if_Equal (l, 0))
    {
        *x = xy[0] ;
        *y = xy[1] ;
        return ;
    }

    si=sin(-E->angle);
    co=cos(-E->angle);
    Rotate_Point(si,co,E->x, E->y, *x, *y, x, y);

    Lx = *x - E->x ;
    Ly = *y - E->y ;
    si = Ly / l ;
    co = Lx / l ;

    *x=E->rx*co;
    *y=E->ry*si;

    denom=sqrt(E->ry*co*E->ry*co+E->rx*si*E->rx*si);

    y1=(E->rx*E->ry*si)/denom;
    x1=(E->rx*E->ry*co)/denom;

    y2=-(E->rx*E->ry*si)/denom;
    x2=-(E->rx*E->ry*co)/denom;

    si1=sin(E->angle);
    co1=cos(E->angle);

    Rotate_Point(si1,co1,0, 0, x1, y1, &x1, &y1);
    x1 += E->x ;
    y1 += E->y ;

    Rotate_Point(si1,co1,0, 0, x2, y2, &x2, &y2);
    x2 += E->x ;
    y2 += E->y ;

    l1 = sqrt ((x1 - x_) * (x1 - x_) + (y1 - y_) * (y1 - y_)) ;
    l2 = sqrt ((x2 - x_) * (x2 - x_) + (y2 - y_) * (y2 - y_)) ;

    if (l1<l2)
    {
        *x=x1;
        *y=y1;
    }
    else
    {
        *x=x2;
        *y=y2;
    }

}

void Elliptical_Arc_Ends (void *adr, double *x1, double *y1, double *x2, double *y2)
{
    ELLIPTICALARC *E;
    int numpoints;
    double xy[20]; //[256];
    int verno;

    verno=Get_Ell_Verno ();

    E = (ELLIPTICALARC*)adr ;

    Set_Ell_Verno (8);
    numpoints=Get_EllipticalArc_Points(E->x, E->y, E->rx, E->ry, E->angle, E->kat1, E->kat2, xy);
    Set_Ell_Verno (verno);

    *x1 = xy[0] ;
    *y1 = xy[1] ;
    *x2 = xy[numpoints-2] ;
    *y2 = xy[numpoints-1] ;
}

void najblizszyO_(double *x,double *y,void *adr)
{ prostopadO(X,Y,x,y,adr);
}

void NearestO_ (double *ptrdf_x, double *ptrdf_y, void *ptr_ob)
/*-----------------------------------------------------------*/
{
  prostopadO (*ptrdf_x, *ptrdf_y, ptrdf_x, ptrdf_y, ptr_ob) ;
}

BOOL najblizszyE_(double *x,double *y,void *adr)
{ return prostopadE(X,Y,x,y,adr);
}

BOOL prostopadleE_(double *x,double *y,void *adr)
{   LINIA *L;
    if(!poczL) return 0;
    L=&LiniaG;
    return prostopadE(L->x1,L->y1,x,y,adr);
}


BOOL najblizszyEA_(double *x,double *y,void *adr)
{
    return prostopadEA(X,Y,x,y,adr);
}

BOOL prostopadleEA_(double *x,double *y,void *adr)
{
    LINIA *L;
    if(!poczL) return 0;
    L=&LiniaG;
    return prostopadEA(L->x1,L->y1,x,y,adr);
}


int prostopadleO_(double *x,double *y,void *adr)
{ LINIA *L;
  if(!poczL) return 0;
  L=&LiniaG;
  prostopadO(L->x1,L->y1,x,y,adr);
  return 1;
}

int prostopadlel(double LX,double LY,double *x, double *y, void *adr)
/*------------------------------------------------------------------------*/
{ double l,Lx,Ly,si,co,e;
  double x1, y1;
  LUK *luk;

  luk = (LUK*)adr ;
  if (TRUE == Check_if_Equal (LX, luk->x) && TRUE == Check_if_Equal (LY, luk->y))
   {
     return 0;
   }
  l = sqrt ((LX - luk->x) * (LX - luk->x) + (LY - luk->y) * (LY - luk->y)) ;
  if (TRUE == Check_if_Equal (l, 0))
  {
    return 0 ;
  }
  Lx = LX - luk->x ; Ly = LY - luk->y ;
  si = Ly / l ;
  co = Lx / l ;
  obru(si,co,*x-luk->x,*y-luk->y,&x1,&y1);
  e=x1>0 ? 1:-1;
  obrd(si,co,e*luk->r,0,&x1,&y1);
  x1+=luk->x;
  y1+=luk->y;
  if (Point_Arc ( x1,  y1, luk))
   {
     *x = x1;
     *y = y1;
     return 1;
   }
  else
   {
     e *= -1;
     obrd(si,co,e*luk->r,0,&x1,&y1);
     x1+=luk->x;
     y1+=luk->y;
     if (Point_Arc ( x1,  y1, luk))
      {
	*x = x1;
	*y = y1;
	return 1;
      }
   }
  return 0;
}



int prostopadlel_(double *x,double *y,void *adr)
/*---------------------------------------------*/
{
  LINIA *L;
  if(!poczL) return 0;
  L=&LiniaG;
  return prostopadlel(L->x1,L->y1,x,y,adr);
}


int przeciecieOO_(double *x, double *y, void *adr, void *adr1)
{
  OKRAG *O1,*O2;
  double l,Lx,Ly,si,co,e,x0,y0,x1,y1,Xk,Yk;

  O1 = (OKRAG*)adr ;
  O2 = (OKRAG*)adr1 ;
  Lx = O2->x - O1->x ;
  Ly = O2->y - O1->y ;
  if (TRUE == Check_if_Equal (Lx, 0) && TRUE == Check_if_Equal (Ly, 0))
  {
    return 0 ;
  }
  l = sqrt (Lx * Lx + Ly * Ly) ;
  si = Ly / l ;
  co = Lx / l ;
  obru (si, co, Lx, Ly, &x0, &y0) ;
  if( (O1->r+O2->r<x0)   ||
      (O1->r>x0 + O2->r) ||
      (O2->r>x0 + O1->r)    ) return 0;
  if (TRUE == Check_if_Equal (x0, 0))
  {
    return FALSE ;		/* sytuacja w zasadzie nie mozliwa */
  }
  x1 = (x0 * x0 + O1->r * O1->r - O2->r * O2->r) / (2 * x0) ;
  obru(si,co,X-O1->x,Y-O1->y,&Xk,&Yk);
  e=Yk>0 ? 1:-1;
  y1=O1->r*O1->r-x1*x1;
  if (y1<0) return 0;
  y1=sqrt(y1);
  obrd(si,co,x1,e*y1,x,y);
  *x+=O1->x;
  *y+=O1->y;
  return 1;
}


int przecieciell_(double *x, double *y, void *adr, void *adr1)
/*-----------------------------------------------------------*/
{
  LUK *luk1,*luk2;
  double l,Lx,Ly,si,co,e,x0,y0,x1,y1,Xk,Yk;


    //check if 2 arcs have the same ends
    double x11, y11, x12, y12, x21, y21, x22, y22;

    Arc_To_Points(adr, &x11, &y11, &x12, &y12);
    Arc_To_Points(adr1, &x21, &y21, &x22, &y22);

    if (Check_if_Equal2(x11,x21) && Check_if_Equal2(y11, y21))  {*x=x11; *y=y11;return 1;}
    else if (Check_if_Equal2(x12,x22) && Check_if_Equal2(y12, y22))  {*x=x12; *y=y12;return 1;}
    else if (Check_if_Equal2(x11,x22) && Check_if_Equal2(y11, y22))  {*x=x11; *y=y11;return 1;}
    else if (Check_if_Equal2(x12,x21) && Check_if_Equal2(y12, y21))  {*x=x12; *y=y12;return 1;}


  luk1 = (LUK*)adr ;
  luk2 = (LUK*)adr1 ;
  Lx = luk2->x - luk1->x ;
  Ly = luk2->y - luk1->y ;
  if (TRUE == Check_if_Equal (Lx, 0) && TRUE == Check_if_Equal (Ly, 0))
  {
    return 0 ;
  }
  l=sqrt(Lx*Lx+Ly*Ly);
  si=Ly/l;  co=Lx/l;
  obru(si,co,Lx,Ly,&x0,&y0);
  if( (luk1->r+luk2->r<x0)   ||
      (luk1->r>x0+2*luk2->r) ||
      (luk2->r>x0+2*luk1->r)    ) return 0;
  if (TRUE == Check_if_Equal (x0, 0))
  {
    return FALSE ;		/* sytuacja w zasadzie nie mozliwa */
  }
  x1=(x0*x0+luk1->r*luk1->r-luk2->r*luk2->r)/(2*x0);
  obru(si,co,X-luk1->x,Y-luk1->y,&Xk,&Yk);
  e=Yk>0 ? 1:-1;
  y1=luk1->r*luk1->r-x1*x1;
  if (y1<0) return 0;
  y1=sqrt(y1);
  obrd(si,co,x1,e*y1,&x1,&y1);
  x1+=luk1->x;
  y1+=luk1->y;
  if ( Point_Arc ( x1, y1, luk1) && Point_Arc ( x1, y1, luk2) )
   {
     *x = x1;
     *y = y1;
     return 1;
   }
  return 0;
}



int przeciecielO_(double *x, double *y, void *adr, void *adr1)
/*------------------------------------------------------------*/
{
  OKRAG *O2;
  LUK * luk;
  double l,Lx,Ly,si,co,e,x0,y0,x1,y1,Xk,Yk;

if (((NAGLOWEK*)adr)->obiekt==Ookrag)
    {
        luk = (LUK *) adr1;
        O2 = (OKRAG *) adr;
    }
    else
    {
        luk = (LUK *) adr;
        O2 = (OKRAG *) adr1;
    }

  Lx = O2->x - luk->x ;
  Ly = O2->y - luk->y ;
  if (TRUE == Check_if_Equal (Lx,0) && TRUE == Check_if_Equal (Ly, 0))
  {
    return 0 ;
  }
  l = sqrt (Lx * Lx + Ly * Ly) ;
  si = Ly / l ;
  co = Lx / l ;
  obru (si, co, Lx, Ly, &x0, &y0) ;
  if( (luk->r+O2->r<x0)   ||
      (luk->r>x0+2*O2->r) ||
      (O2->r>x0+2*luk->r)    ) return 0;
  if (TRUE == Check_if_Equal (x0, 0))
  {
    return FALSE ;		/* sytuacja w zasadzie nie mozliwa */
  }
  x1=(x0*x0+luk->r*luk->r-O2->r*O2->r)/(2*x0);
  obru(si,co,X-luk->x,Y-luk->y,&Xk,&Yk);
  e=Yk>0 ? 1:-1;
  y1=luk->r*luk->r-x1*x1;
  if (y1<0) return 0;
  y1=sqrt(y1);
  obrd(si,co,x1,e*y1,&x1,&y1);
  x1+=luk->x;
  y1+=luk->y;
  if ( Point_Arc ( x1, y1, luk) )
   {
     *x = x1;
     *y = y1;
     return 1;
   }
  return 0;
}

int przeciecielO_r(double *x, double *y, void *adr, void *adr1)
{
    return przeciecielO_(x, y, adr1, adr);
}

/*-----------------------------------------------------------*/

BOOL wielokat_przec (double *x, double *y, void *adr, void *adr1,
		int (*fun)(double*, double*, void*, void*))
/*--------------------------------------------------------*/
{
  LINIA L = Ldef ;
  WIELOKAT *w ;
  double x0, y0 ;
  BOOL b_ret ;

  int i;

  b_ret = FALSE ;
  w = (WIELOKAT*)adr ;
  for (i = 0 ; i < w->lp - 2 ; i += 2)
  {
    L.x1 = w->xy [i + 0] ;
    L.y1 = w->xy [i + 1] ;
    L.x2 = w->xy [i + 2] ;
    L.y2 = w->xy [i + 3] ;
    if (0 != linia_wybrana (&L) 	      &&
	0 != fun (&x0, &y0, (void*)&L, adr1))
    {
      b_ret = TRUE ;
      if ( 0 != punkt_w_prostokacie(x0, y0))
      {
	*x = x0 ;
	*y = y0 ;
	return 1 ;
      }
    }
  }
  L.x1 = w->xy [0] ;
  L.y1 = w->xy [1] ;
  L.x2 = w->xy [w->lp-2] ;
  L.y2 = w->xy [w->lp-1] ;
  if (0 != linia_wybrana (&L) 	      &&
      0 != fun (&x0, &y0, (void*)&L, adr1))
  {
    b_ret = TRUE ;
    if ( 0 != punkt_w_prostokacie(x0, y0))
    {
      *x = x0 ;
      *y = y0 ;
      return 1 ;
    }
  }
  if (b_ret == TRUE)
  {
    *x = x0 ;
    *y = y0 ;
  }
  return b_ret ;
}

BOOL solidarc_przec (double *x, double *y, void *adr, void *adr1,
                     int (*funLine)(double*, double*, void*, void*), int (*funArc)(double*, double*, void*, void*))
/*----------------------------------------------------------------------------------------------------------------*/
{
    LINIA L_right=Ldef, L_left=Ldef ;
    LUK l_inner1=ldef, l_inner2=ldef, l_outer1=ldef, l_outer2=ldef;
    SOLIDARC *sa ;
    double x0, y0 ;
    BOOL b_ret ;
    int ln;

    b_ret = FALSE ;
    sa = (SOLIDARC*)adr ;

    ln=solidarc_elements(sa, &L_left, &L_right, &l_inner1, &l_inner2,&l_outer1, &l_outer2);

    //lines

    if (0 != linia_wybrana (&L_left) 	      &&
        0 != funLine (&x0, &y0, (void*)&L_left, adr1))
    {
        b_ret = TRUE ;
        if ( 0 != punkt_w_prostokacie(x0, y0))
        {
            *x = x0 ;
            *y = y0 ;
            return 1 ;
        }
    }

    if (0 != linia_wybrana (&L_right) 	      &&
        0 != funLine (&x0, &y0, (void*)&L_right, adr1))
    {
        b_ret = TRUE ;
        if ( 0 != punkt_w_prostokacie(x0, y0))
        {
            *x = x0 ;
            *y = y0 ;
            return 1 ;
        }
    }

    //arcs
    if (0 != luk_wybrany (&l_inner1) 	      &&
        0 != funArc (&x0, &y0, adr1, (void*)&l_inner1))
    {
        b_ret = TRUE ;
        if ( 0 != punkt_w_prostokacie(x0, y0))
        {
            *x = x0 ;
            *y = y0 ;
            return 1 ;
        }
    }

    if (0 != luk_wybrany (&l_outer1) 	      &&
        0 != funArc (&x0, &y0, adr1, (void*)&l_outer1))
    {
        b_ret = TRUE ;
        if ( 0 != punkt_w_prostokacie(x0, y0))
        {
            *x = x0 ;
            *y = y0 ;
            return 1 ;
        }
    }

    if (ln>1)
    {
        if (0 != luk_wybrany (&l_inner2) 	      &&
            0 != funArc (&x0, &y0, adr1, (void*)&l_inner2))
        {
            b_ret = TRUE ;
            if ( 0 != punkt_w_prostokacie(x0, y0))
            {
                *x = x0 ;
                *y = y0 ;
                return 1 ;
            }
        }

        if (0 != luk_wybrany (&l_outer2) 	      &&
            0 != funArc (&x0, &y0, adr1, (void*)&l_outer2))
        {
            b_ret = TRUE ;
            if ( 0 != punkt_w_prostokacie(x0, y0))
            {
                *x = x0 ;
                *y = y0 ;
                return 1 ;
            }
        }
    }

    if (b_ret == TRUE)
    {
        *x = x0 ;
        *y = y0 ;
    }
    return b_ret ;
}

BOOL vector_przec (double *x, double *y, void *adr, void *adr1,
                     int (*funLine)(double*, double*, void*, void*), int (*funArc)(double*, double*, void*, void*))
/*----------------------------------------------------------------------------------------------------------------*/
{
    LINIA L_right=Ldef, L_left=Ldef ;
    LUK l_inner1=ldef, l_inner2=ldef, l_outer1=ldef, l_outer2=ldef;
    AVECTOR *v ;
    double x0, y0 ;
    BOOL b_ret ;
    int ln;

    b_ret = FALSE ;
    v = (AVECTOR*)adr ;

    //lines

    switch (v->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            if (0 != linia_wybrana (v) 	      &&
                0 != funLine (&x0, &y0, (void*)v, adr1))
            {
                b_ret = TRUE ;
                if ( 0 != punkt_w_prostokacie(x0, y0))
                {
                    *x = x0 ;
                    *y = y0 ;
                    return 1 ;
                }
            }
            break;
        case 4:
        case 18:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
        case 20:
        case 17:
            break;
        default:
            break;
    }


    return 0 ;
}

BOOL spline_przec(double *x, double *y, void *adr, void *adr1,
	int(*fun)(double*, double*, void*, void*))
	/*--------------------------------------------------------*/
{
	LINIA L = Ldef;
	SPLINE *s;
	double x0, y0;
	BOOL b_ret;
	int i, j;
	float out_x[MaxNpts], out_y[MaxNpts];

    SPLINE tmp_spline = Splinedef;
    double bnodes_2_tangent_org, bnodes_2_tangent;
    int node_count;

	b_ret = FALSE;
	s = (SPLINE*)adr;

#define Npts 90  //for more precission
    if (s->npts < 5)
    {
        calc_bspline(s->lp, s->npts, s->xy, Npts, &out_x, &out_y);
        for (i = 0; i < Npts - 1; i++)
        {
            L.x1 = out_x[i];
            L.y1 = out_y[i];
            L.x2 = out_x[i + 1];
            L.y2 = out_y[i + 1];
            if (0 != linia_wybrana(&L) &&
                0 != fun(&x0, &y0, (void*)&L, adr1))
            {
                b_ret = TRUE;
                if (0 != punkt_w_prostokacie(x0, y0))
                {
                    *x = x0;
                    *y = y0;
                    return 1;
                }
            }
        }
    }
    else
    {
        curviness = s->xy[s->lp];

        tmp_spline.npts = 4;
        tmp_spline.lp = 8;
        tmp_spline.kolor = s->kolor;
        tmp_spline.typ = s->typ;

        node_count = 1;

        set_bnodes(s, s->xy, &node_count);

        calc_tangents(s->closed, &node_count, &bnodes_2_tangent);

        if (s->closed == 0)
        {
            bnodes_2_tangent_org = bnodes[2].tangent;
            bnodes[2].tangent = bnodes_2_tangent;
            get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
            calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
            for (i = 0; i < Npts - 1; i++)
            {
                L.x1 = out_x[i];
                L.y1 = out_y[i];
                L.x2 = out_x[i + 1];
                L.y2 = out_y[i + 1];
                if (0 != linia_wybrana(&L) &&
                    0 != fun(&x0, &y0, (void*)&L, adr1))
                {
                    b_ret = TRUE;
                    if (0 != punkt_w_prostokacie(x0, y0))
                    {
                        *x = x0;
                        *y = y0;
                        return 1;
                    }
                }
            }
            bnodes[2].tangent = bnodes_2_tangent_org;
        }

        for (j = 2; j < node_count - 2; j++)  //1 - extra_node
        {
            get_control_points(bnodes[j], bnodes[j + 1], tmp_spline.xy);
            calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
            for (i = 0; i < Npts - 1; i++)
            {
                L.x1 = out_x[i];
                L.y1 = out_y[i];
                L.x2 = out_x[i + 1];
                L.y2 = out_y[i + 1];
                if (0 != linia_wybrana(&L) &&
                    0 != fun(&x0, &y0, (void*)&L, adr1))
                {
                    b_ret = TRUE;
                    if (0 != punkt_w_prostokacie(x0, y0))
                    {
                        *x = x0;
                        *y = y0;
                        return 1;
                    }
                }
            }
        }

        if ((node_count > 3) && (s->closed == 0))
        {
            get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
            calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
            for (i = 0; i < Npts - 1; i++)
            {
                L.x1 = out_x[i];
                L.y1 = out_y[i];
                L.x2 = out_x[i + 1];
                L.y2 = out_y[i + 1];
                if (0 != linia_wybrana(&L) &&
                    0 != fun(&x0, &y0, (void*)&L, adr1))
                {
                    b_ret = TRUE;
                    if (0 != punkt_w_prostokacie(x0, y0))
                    {
                        *x = x0;
                        *y = y0;
                        return 1;
                    }
                }
            }
        }

    }
	
	if (b_ret == TRUE)
	{
		*x = x0;
		*y = y0;
	}
	return b_ret;
}


int ellipse_przec(double *x, double *y, void *adr, void *adr1,
                  int(*fun)(double*, double*, void*, void*))
/*--------------------------------------------------------*/
{
    LINIA L = Ldef;
    ELLIPSE *e;
    double x0, y0;
    int b_ret=0;
    int i, j;
    int numpoints;
    double xy[256];
    double xi[4], yi[4];
    double dl;
    double dlmin=MAXDOUBLE;

    b_ret = FALSE;
    e = (ELLIPSE*)adr;

    numpoints= Get_Ellipse_Points(e->x, e->y, e->rx, e->ry, e->angle, xy);
    xy[numpoints] = xy[0];
    xy[numpoints+1] = xy[1];
    numpoints+=2;

    for (i = 0; i < numpoints - 2; i+=2) {
        L.x1 = xy[i];
        L.y1 = xy[i + 1];
        L.x2 = xy[i + 2];
        L.y2 = xy[i + 3];
        if (0 != linia_wybrana(&L) &&
            0 != fun(&x0, &y0, (void *) &L, adr1))
            {
                xi[b_ret] = x0;
                yi[b_ret] = y0;
                b_ret += 1;
            }
        }

    if (b_ret>0)
    {

        for (i=0; i<b_ret; i++)
        {
            //choosing nearest_ point
            dl=(X-xi[i])*(X-xi[i])+(Y-yi[i])*(Y-yi[i]);
            if (dl<dlmin) {
                *x = xi[i];
                *y = yi[i];
            }
        }
    }

    return b_ret;
}

int przeciecieLW_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Owwielokat)
        return wielokat_przec (x, y, adr, adr1, przeciecieLL_) ;
    else return wielokat_przec (x, y, adr1, adr, przeciecieLL_) ;
}

int przeciecieLW_r(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Owwielokat)
        return przeciecieLW_(x, y, adr, adr1);
    else return przeciecieLW_(x, y, adr1, adr);
}

int przeciecieLSA_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Osolidarc)
        return solidarc_przec (x, y, adr, adr1, przeciecieLL_, przeciecieLl_) ;
    else return solidarc_przec (x, y, adr1, adr, przeciecieLL_, przeciecieLl_) ;
}

int przeciecieLV_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr1)->obiekt==Olinia)
        return vector_przec (x, y, adr, adr1, przeciecieLL_, przeciecieLl_) ;
    else
        return vector_przec (x, y, adr1, adr, przeciecieLL_, przeciecieLl_) ;
}

int przeciecieLSA_r(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Osolidarc)
        return przeciecieLSA_(x, y, adr, adr1);
    else return przeciecieLSA_(x, y, adr1, adr);
}

int przeciecieLS_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Ospline)
        return spline_przec(x, y, adr, adr1, przeciecieLL_);
	return spline_przec(x, y, adr1, adr, przeciecieLL_);
}

int przeciecielW_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Owwielokat)
        return wielokat_przec (x, y, adr, adr1, przeciecieLl_) ;
  else return wielokat_przec (x, y, adr1, adr, przeciecieLl_) ;
}

int przeciecielW_r(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Owwielokat)
        return przeciecielW_(x, y, adr, adr1);
    else return przeciecielW_(x, y, adr1, adr);
}

int przeciecieSAW_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Owwielokat)
        return solidarc_przec (x, y, adr1, adr, przeciecieLW_, przeciecielW_r);
    else return solidarc_przec (x, y, adr, adr1, przeciecieLW_, przeciecielW_r);
}

int przeciecieVW_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Owwielokat)
        return vector_przec (x, y, adr1, adr, przeciecieLW_, przeciecielW_r);
    else return vector_przec (x, y, adr, adr1, przeciecieLW_, przeciecielW_r);
}

int przeciecielSA_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Osolidarc)
        return solidarc_przec (x, y, adr, adr1, przeciecieLl_, przecieciell_) ;
    else solidarc_przec (x, y, adr1, adr, przeciecieLl_, przecieciell_) ;
}

int przeciecielV_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr1)->obiekt==Oluk)
        return vector_przec (x, y, adr, adr1, przeciecieLl_, przecieciell_) ;
    else return vector_przec (x, y, adr1, adr, przeciecieLl_, przecieciell_) ;
}

int przeciecielV_r(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr1)->obiekt==Oluk)
        return przeciecielV_(x, y, adr, adr1);
    return przeciecielV_(x, y, adr1, adr);
}

int przeciecielSA_r(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Osolidarc)
        return przeciecielSA_(x, y, adr, adr1);
    else return przeciecielSA_(x, y, adr1, adr);
}

int przeciecieSASA_(double *x, double *y, void *adr, void *adr1)
{
    return solidarc_przec (x, y, adr, adr1, przeciecieLSA_, przeciecielSA_r);
}

int przeciecieVV_(double *x, double *y, void *adr, void *adr1)
{
    return vector_przec (x, y, adr, adr1, przeciecieLV_, przeciecielV_r);
}

int przeciecielS_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Ospline)
        return spline_przec(x, y, adr, adr1, przeciecieLl_);
	else return spline_przec(x, y, adr1, adr, przeciecieLl_);
}

int przeciecielS_r(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Ospline)
        return przeciecielS_(x, y, adr, adr1);
    else return przeciecielS_(x, y, adr1, adr);
}

int przeciecieSAS_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Ospline)
        return solidarc_przec(x, y, adr1, adr, przeciecieLS_, przeciecielS_r);
    else return solidarc_przec(x, y, adr, adr1, przeciecieLS_, przeciecielS_r);
}

int przeciecieVS_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Ospline)
        return vector_przec(x, y, adr1, adr, przeciecieLS_, przeciecielS_r);
    else return vector_przec(x, y, adr, adr1, przeciecieLS_, przeciecielS_r);
}

int przeciecieWO_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Ookrag)
        return wielokat_przec (x, y, adr1, adr, przeciecieLO_) ;
    else return wielokat_przec (x, y, adr, adr1, przeciecieLO_) ;
}

int przeciecieSAO_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Ookrag)
        return solidarc_przec (x, y, adr1, adr, przeciecieLO_, przeciecielO_r) ;
    else return solidarc_przec (x, y, adr, adr1, przeciecieLO_, przeciecielO_r) ;
}

int przeciecieVO_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Ookrag)
        return vector_przec (x, y, adr1, adr, przeciecieLO_, przeciecielO_r) ;
    else return vector_przec (x, y, adr, adr1, przeciecieLO_, przeciecielO_r) ;
}

int przeciecieSO_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Ookrag)
        return spline_przec(x, y, adr1, adr, przeciecieLO_);
	else return spline_przec(x, y, adr, adr1, przeciecieLO_);
}

int przeciecieSE_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Oellipse)
        return spline_przec(x, y, adr1, adr, przeciecieLE_);
    else return spline_przec(x, y, adr, adr1, przeciecieLE_);
}

int przeciecieEE_(double *x, double *y, void *adr, void *adr1)
{
    return ellipse_przec(x, y, adr, adr1, przeciecieLE_);
}

int przeciecieWW_(double *x, double *y, void *adr, void *adr1)
{
  LINIA L = Ldef, L1 = Ldef ;
  WIELOKAT *w, *w1 ;
  double x0, y0 ;
  double x1, y1, x11, y11 ;
  BOOL b_ret ;
  int i, j ;

  b_ret = FALSE ;
  w = (WIELOKAT*)adr ;
  w1 = (WIELOKAT*)adr1 ;

  x1 = w->xy [w->lp-2] ;
  y1 = w->xy [w->lp-1] ;
  for (i = 0 ; i < w->lp ; i += 2)
  {
    L.x1 = x1 ;
    L.y1 = y1 ;
    L.x2 = w->xy [i + 0] ;
    L.y2 = w->xy [i + 1] ;
    if (0 == linia_wybrana (&L))
    {
      x1 = L.x2 ;
      y1 = L.y2 ;
      continue ;
    }
    x11 = w1->xy [w1->lp-2] ;
    y11 = w1->xy [w1->lp-1] ;
    for (j = 0 ; j < w1->lp ; j += 2)
    {
      L1.x1 = x11 ;
      L1.y1 = y11 ;
      L1.x2 = w1->xy [j + 0] ;
      L1.y2 = w1->xy [j + 1] ;
      if (0 == linia_wybrana (&L1))
      {
	x11 = L1.x2 ;
	y11 = L1.y2 ;
	continue ;
      }
      if (0 != przeciecieLL_ (&x0, &y0, (void*)&L, (void*)&L1))
      {
	b_ret = TRUE ;
	if ( 0 != punkt_w_prostokacie(x0, y0))
	{
	  *x = x0 ;
	  *y = y0 ;
	  return b_ret ;
	}
      }
      x11 = L1.x2 ;
      y11 = L1.y2 ;
    }
    x1 = L.x2 ;
    y1 = L.y2 ;
  }
  if (b_ret == TRUE)
  {
    *x = x0 ;
    *y = y0 ;
  }
  return b_ret ;
}

/*------------------------------------------------------------*/

int przeciecieWS_(double *x, double *y, void *adr, void *adr1)
	/*--------------------------------------------------------*/
	{
		LINIA L = Ldef, L1 = Ldef;
		WIELOKAT *w;
		SPLINE *s;
		double x0, y0;
		double x1, y1;
		BOOL b_ret;
		int i, j, k;
		float out_x[MaxNpts], out_y[MaxNpts];

        SPLINE tmp_spline = Splinedef;
        double bnodes_2_tangent_org, bnodes_2_tangent;
        int node_count;

		b_ret = FALSE;

        if (((NAGLOWEK*)adr)->obiekt==Ospline)
        {
            w = (WIELOKAT*)adr1;
            s = (SPLINE*)adr;
        }
        else {
            w = (WIELOKAT *) adr;
            s = (SPLINE *) adr1;
        }

#define Npts 90  //for more precission
        if (s->npts < 5)
        {

            calc_bspline(s->lp, s->npts, s->xy, Npts, &out_x, &out_y);

            x1 = w->xy[w->lp - 2];
            y1 = w->xy[w->lp - 1];
            for (i = 0; i < w->lp; i += 2)
            {
                L.x1 = x1;
                L.y1 = y1;
                L.x2 = w->xy[i + 0];
                L.y2 = w->xy[i + 1];
                if (0 == linia_wybrana(&L))
                {
                    x1 = L.x2;
                    y1 = L.y2;
                    continue;
                }

                for (j = 0; j < Npts - 1; j++)
                {
                    L1.x1 = out_x[j];
                    L1.y1 = out_y[j];
                    L1.x2 = out_x[j + 1];
                    L1.y2 = out_y[j + 1];
                    if (0 == linia_wybrana(&L1))
                    {
                        continue;
                    }
                    if (0 != przeciecieLL_(&x0, &y0, (void*)&L, (void*)&L1))
                    {
                        b_ret = TRUE;
                        if (0 != punkt_w_prostokacie(x0, y0))
                        {
                            *x = x0;
                            *y = y0;
                            return b_ret;
                        }
                    }
                }
                x1 = L.x2;
                y1 = L.y2;
            }
        }
        else
        {
            curviness = s->xy[s->lp];

            tmp_spline.npts = 4;
            tmp_spline.lp = 8;
            tmp_spline.kolor = s->kolor;
            tmp_spline.typ = s->typ;

            node_count = 1;

            set_bnodes(s, s->xy, &node_count);

            calc_tangents(s->closed, &node_count, &bnodes_2_tangent);

            if (s->closed == 0)
            {
                bnodes_2_tangent_org = bnodes[2].tangent;
                bnodes[2].tangent = bnodes_2_tangent;
                get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
                calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
                x1 = w->xy[w->lp - 2];
                y1 = w->xy[w->lp - 1];
                for (i = 0; i < w->lp; i += 2)
                {
                    L.x1 = x1;
                    L.y1 = y1;
                    L.x2 = w->xy[i + 0];
                    L.y2 = w->xy[i + 1];
                    if (0 == linia_wybrana(&L))
                    {
                        x1 = L.x2;
                        y1 = L.y2;
                        continue;
                    }

                    for (j = 0; j < Npts - 1; j++)
                    {
                        L1.x1 = out_x[j];
                        L1.y1 = out_y[j];
                        L1.x2 = out_x[j + 1];
                        L1.y2 = out_y[j + 1];
                        if (0 == linia_wybrana(&L1))
                        {
                            continue;
                        }
                        if (0 != przeciecieLL_(&x0, &y0, (void*)&L, (void*)&L1))
                        {
                            b_ret = TRUE;
                            if (0 != punkt_w_prostokacie(x0, y0))
                            {
                                *x = x0;
                                *y = y0;
                                return b_ret;
                            }
                        }
                    }
                    x1 = L.x2;
                    y1 = L.y2;
                }
                bnodes[2].tangent = bnodes_2_tangent_org;
            }

            for (k = 2; k < node_count - 2; k++)  //1 - extra_node
            {
                get_control_points(bnodes[k], bnodes[k + 1], tmp_spline.xy);
                calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
                x1 = w->xy[w->lp - 2];
                y1 = w->xy[w->lp - 1];
                for (i = 0; i < w->lp; i += 2)
                {
                    L.x1 = x1;
                    L.y1 = y1;
                    L.x2 = w->xy[i + 0];
                    L.y2 = w->xy[i + 1];
                    if (0 == linia_wybrana(&L))
                    {
                        x1 = L.x2;
                        y1 = L.y2;
                        continue;
                    }

                    for (j = 0; j < Npts - 1; j++)
                    {
                        L1.x1 = out_x[j];
                        L1.y1 = out_y[j];
                        L1.x2 = out_x[j + 1];
                        L1.y2 = out_y[j + 1];
                        if (0 == linia_wybrana(&L1))
                        {
                            continue;
                        }
                        if (0 != przeciecieLL_(&x0, &y0, (void*)&L, (void*)&L1))
                        {
                            b_ret = TRUE;
                            if (0 != punkt_w_prostokacie(x0, y0))
                            {
                                *x = x0;
                                *y = y0;
                                return b_ret;
                            }
                        }
                    }
                    x1 = L.x2;
                    y1 = L.y2;
                }
            }

            if ((node_count > 3) && (s->closed == 0))
            {
                get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
                calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
                x1 = w->xy[w->lp - 2];
                y1 = w->xy[w->lp - 1];
                for (i = 0; i < w->lp; i += 2)
                {
                    L.x1 = x1;
                    L.y1 = y1;
                    L.x2 = w->xy[i + 0];
                    L.y2 = w->xy[i + 1];
                    if (0 == linia_wybrana(&L))
                    {
                        x1 = L.x2;
                        y1 = L.y2;
                        continue;
                    }

                    for (j = 0; j < Npts - 1; j++)
                    {
                        L1.x1 = out_x[j];
                        L1.y1 = out_y[j];
                        L1.x2 = out_x[j + 1];
                        L1.y2 = out_y[j + 1];
                        if (0 == linia_wybrana(&L1))
                        {
                            continue;
                        }
                        if (0 != przeciecieLL_(&x0, &y0, (void*)&L, (void*)&L1))
                        {
                            b_ret = TRUE;
                            if (0 != punkt_w_prostokacie(x0, y0))
                            {
                                *x = x0;
                                *y = y0;
                                return b_ret;
                            }
                        }
                    }
                    x1 = L.x2;
                    y1 = L.y2;
                }
            }
        }

		if (b_ret == TRUE)
		{
			*x = x0;
			*y = y0;
		}
		return b_ret;
	}

int przeciecieSS_(double *x, double *y, void *adr, void *adr1)
	/*--------------------------------------------------------*/
	{
		LINIA L = Ldef, L1 = Ldef;
		SPLINE *s, *s1, *s_;
		double x0, y0;
		BOOL b_ret;
		int i, j, k;
		float out_x[MaxNpts], out_y[MaxNpts];
		float out_x1[MaxNpts], out_y1[MaxNpts];

        SPLINE tmp_spline = Splinedef;
        double bnodes_2_tangent_org, bnodes_2_tangent;
        int node_count;

        SPLINE tmp_spline1 = Splinedef;
        double bnodes_2_tangent_org1, bnodes_2_tangent1;
        int node_count1;

        int NODES_NO, NODES_NO1;

		b_ret = FALSE;
		s = (SPLINE*)adr;
		s1 = (SPLINE*)adr1;

        if ((s->npts < 5) && (s1->npts < 5))
        {
            ;  //it's fine
        }
        else if ((s->npts > 5) && (s1->npts < 5))
        {
            s_ = s1;
            s1 = s;
            s = s_;
        }
        //else both are multipoints

#define Npts 90  //for more precission
        if (s->npts < 5)  //can be bezier or multipoints
        {
            calc_bspline(s->lp, s->npts, s->xy, Npts, &out_x, &out_y);
            if (s1->npts < 5)  //can be 
            {
                calc_bspline(s1->lp, s1->npts, s1->xy, Npts, &out_x1, &out_y1);

                for (i = 0; i < Npts - 1; i++)
                {
                    L.x1 = out_x[i];
                    L.y1 = out_y[i];
                    L.x2 = out_x[i + 1];
                    L.y2 = out_y[i + 1];
                    if (0 == linia_wybrana(&L))
                    {
                        continue;
                    }

                    for (j = 0; j < Npts - 1; j++)
                    {
                        L1.x1 = out_x1[j];
                        L1.y1 = out_y1[j];
                        L1.x2 = out_x1[j + 1];
                        L1.y2 = out_y1[j + 1];
                        if (0 == linia_wybrana(&L1))
                        {
                            continue;
                        }
                        if (0 != przeciecieLL_(&x0, &y0, (void*)&L, (void*)&L1))
                        {
                            b_ret = TRUE;
                            if (0 != punkt_w_prostokacie(x0, y0))
                            {
                                *x = x0;
                                *y = y0;
                                return b_ret;
                            }
                        }
                    }
                }
            }
            else
            {
                curviness = s->xy[s->lp];

                tmp_spline.npts = 4;
                tmp_spline.lp = 8;
                tmp_spline.kolor = s->kolor;
                tmp_spline.typ = s->typ;

                node_count = 1;

                set_bnodes(s1, s->xy, &node_count);

                calc_tangents(s1->closed, &node_count, &bnodes_2_tangent);

                if (s1->closed == 0)
                {
                    bnodes_2_tangent_org = bnodes[2].tangent;
                    bnodes[2].tangent = bnodes_2_tangent;
                    get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline1.xy);
                    calc_bspline(tmp_spline1.lp, tmp_spline1.npts, tmp_spline1.xy, Npts, out_x1, out_y1);
                    for (i = 0; i < Npts - 1; i++)
                    {
                        L.x1 = out_x[i];
                        L.y1 = out_y[i];
                        L.x2 = out_x[i + 1];
                        L.y2 = out_y[i + 1];
                        if (0 == linia_wybrana(&L))
                        {
                            continue;
                        }

                        for (j = 0; j < Npts - 1; j++)
                        {
                            L1.x1 = out_x1[j];
                            L1.y1 = out_y1[j];
                            L1.x2 = out_x1[j + 1];
                            L1.y2 = out_y1[j + 1];
                            if (0 == linia_wybrana(&L1))
                            {
                                continue;
                            }
                            if (0 != przeciecieLL_(&x0, &y0, (void*)&L, (void*)&L1))
                            {
                                b_ret = TRUE;
                                if (0 != punkt_w_prostokacie(x0, y0))
                                {
                                    *x = x0;
                                    *y = y0;
                                    return b_ret;
                                }
                            }
                        }
                    }
                    bnodes[2].tangent = bnodes_2_tangent_org;
                }

                for (k = 2; k < node_count - 2; k++)  //1 - extra_node
                {
                    get_control_points(bnodes[k], bnodes[k + 1], tmp_spline1.xy);
                    calc_bspline(tmp_spline1.lp, tmp_spline1.npts, tmp_spline1.xy, Npts, out_x1, out_y1);
                    for (i = 0; i < Npts - 1; i++)
                    {
                        L.x1 = out_x[i];
                        L.y1 = out_y[i];
                        L.x2 = out_x[i + 1];
                        L.y2 = out_y[i + 1];
                        if (0 == linia_wybrana(&L))
                        {
                            continue;
                        }

                        for (j = 0; j < Npts - 1; j++)
                        {
                            L1.x1 = out_x1[j];
                            L1.y1 = out_y1[j];
                            L1.x2 = out_x1[j + 1];
                            L1.y2 = out_y1[j + 1];
                            if (0 == linia_wybrana(&L1))
                            {
                                continue;
                            }
                            if (0 != przeciecieLL_(&x0, &y0, (void*)&L, (void*)&L1))
                            {
                                b_ret = TRUE;
                                if (0 != punkt_w_prostokacie(x0, y0))
                                {
                                    *x = x0;
                                    *y = y0;
                                    return b_ret;
                                }
                            }
                        }
                    }
                }

                if ((node_count > 3) && (s1->closed == 0))
                {
                    get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline1.xy);
                    calc_bspline(tmp_spline1.lp, tmp_spline1.npts, tmp_spline1.xy, Npts, out_x1, out_y1);
                    for (i = 0; i < Npts - 1; i++)
                    {
                        L.x1 = out_x[i];
                        L.y1 = out_y[i];
                        L.x2 = out_x[i + 1];
                        L.y2 = out_y[i + 1];
                        if (0 == linia_wybrana(&L))
                        {
                            continue;
                        }

                        for (j = 0; j < Npts - 1; j++)
                        {
                            L1.x1 = out_x1[j];
                            L1.y1 = out_y1[j];
                            L1.x2 = out_x1[j + 1];
                            L1.y2 = out_y1[j + 1];
                            if (0 == linia_wybrana(&L1))
                            {
                                continue;
                            }
                            if (0 != przeciecieLL_(&x0, &y0, (void*)&L, (void*)&L1))
                            {
                                b_ret = TRUE;
                                if (0 != punkt_w_prostokacie(x0, y0))
                                {
                                    *x = x0;
                                    *y = y0;
                                    return b_ret;
                                }
                            }
                        }
                    }
                }

            }
        }
        else  //both are multipoints so there will be different approach:
        {
            //first spline
            int points_count = 0;
            int points_count1 = 0;

            NODES_NO = s->lp / 2 + 5;
            NODES_NO1 = s1->lp / 2 + 5;

            float* all_out_x = malloc(90 * NODES_NO * sizeof(float));
            if (all_out_x == NULL) return b_ret;
            float* all_out_y = malloc(90 * NODES_NO * sizeof(float));
            if (all_out_y == NULL)
            {
                free(all_out_x);
                return FALSE;
            }
            float* all_out_x1 = malloc(90 * NODES_NO1 * sizeof(float));
            if (all_out_x1 == NULL)
            {
                free(all_out_x);
                free(all_out_y);
                return FALSE;
            }
            float* all_out_y1 = malloc(90 * NODES_NO1 * sizeof(float));
            if (all_out_y1 == NULL)
            {
                free(all_out_x);
                free(all_out_y);
                free(all_out_x1);
                return FALSE;
            }

            curviness = s->xy[s->lp];

            tmp_spline.npts = 4;
            tmp_spline.lp = 8;
            tmp_spline.kolor = s->kolor;
            tmp_spline.typ = s->typ;

            node_count = 1;

            set_bnodes(s, s->xy, &node_count);

            calc_tangents(s->closed, &node_count, &bnodes_2_tangent);
            
            if (s->closed == 0)
            {
                bnodes_2_tangent_org = bnodes[2].tangent;
                bnodes[2].tangent = bnodes_2_tangent;
                get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
                calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

                memmove(&all_out_x[points_count], &out_x, sizeof(float)* Npts);
                memmove(&all_out_y[points_count], &out_y, sizeof(float)* Npts);
                points_count += Npts;
                bnodes[2].tangent = bnodes_2_tangent_org;
            }
            for (k = 2; k < node_count - 2; k++)  //1 - extra_node
            {
                get_control_points(bnodes[k], bnodes[k + 1], tmp_spline.xy);
                calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

                memmove(&all_out_x[points_count], &out_x, sizeof(float)* Npts);
                memmove(&all_out_y[points_count], &out_y, sizeof(float)* Npts);
                points_count += Npts;
            }

            if ((node_count > 3) && (s->closed == 0))
            {
                get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
                calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

                memmove(&all_out_x[points_count], &out_x, sizeof(float)* Npts);
                memmove(&all_out_y[points_count], &out_y, sizeof(float)* Npts);
                points_count += Npts;
            }

            curviness = s1->xy[s1->lp];

            tmp_spline.npts = 4;
            tmp_spline.lp = 8;
            tmp_spline.kolor = s1->kolor;
            tmp_spline.typ = s1->typ;

            node_count = 1;

            set_bnodes(s1, s->xy, &node_count);

            calc_tangents(s1->closed, &node_count, &bnodes_2_tangent);

            if (s1->closed == 0)
            {
                bnodes_2_tangent_org = bnodes[2].tangent;
                bnodes[2].tangent = bnodes_2_tangent;
                get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
                calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

                memmove(&all_out_x1[points_count1], &out_x, sizeof(float)* Npts);
                memmove(&all_out_y1[points_count1], &out_y, sizeof(float)* Npts);
                points_count1 += Npts;
                bnodes[2].tangent = bnodes_2_tangent_org;
            }
            for (k = 2; k < node_count - 2; k++)  //1 - extra_node
            {
                get_control_points(bnodes[k], bnodes[k + 1], tmp_spline.xy);
                calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

                memmove(&all_out_x1[points_count1], &out_x, sizeof(float)* Npts);
                memmove(&all_out_y1[points_count1], &out_y, sizeof(float)* Npts);
                points_count1 += Npts;
            }

            if ((node_count > 3) && (s1->closed == 0))
            {
                get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
                calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);

                memmove(&all_out_x1[points_count1], &out_x, sizeof(float) * Npts);
                memmove(&all_out_y1[points_count1], &out_y, sizeof(float) * Npts);
                points_count1 += Npts;
            }


            for (i = 0; i < points_count - 1; i++)
            {
                L.x1 = all_out_x[i];
                L.y1 = all_out_y[i];
                L.x2 = all_out_x[i + 1];
                L.y2 = all_out_y[i + 1];
                if (0 == linia_wybrana(&L))
                {
                    continue;
                }

                for (j = 0; j < points_count1 - 1; j++)
                {
                    L1.x1 = all_out_x1[j];
                    L1.y1 = all_out_y1[j];
                    L1.x2 = all_out_x1[j + 1];
                    L1.y2 = all_out_y1[j + 1];
                    if (0 == linia_wybrana(&L1))
                    {
                        continue;
                    }
                    if (0 != przeciecieLL_(&x0, &y0, (void*)&L, (void*)&L1))
                    {
                        b_ret = TRUE;
                        if (0 != punkt_w_prostokacie(x0, y0))
                        {
                            *x = x0;
                            *y = y0;

                            free(all_out_x);
                            free(all_out_y);

                            free(all_out_x1);
                            free(all_out_y1);

                            return b_ret;
                        }
                    }
                }
            }

            free(all_out_x);
            free(all_out_y);

            free(all_out_x1);
            free(all_out_y1);
           
        }
		if (b_ret == TRUE)
		{
			*x = x0;
			*y = y0;
		}
		return b_ret;
	}


int przeciecieLE_prec_(double *x, double *y, void *adr, void *adr1)
{   NAGLOWEK *nag;
    LINIA *L;
    ELLIPSE *e;
    int b_ret;
    double xi1, xi2 , yi1 , yi2;
    double dl1, dl2;

    nag=(NAGLOWEK*)adr;
    if (nag->obiekt==Olinia)
    {
        L=(LINIA*)adr;
        e=(ELLIPSE*)adr1;
    }
    else
    {
        L=(LINIA*)adr1;
        e=(ELLIPSE*)adr;
    }

    b_ret=EllipseIntersectLine(e->x, e->y, e->rx, e->ry, e->angle, L->x1 , L->y1 , L->x2 , L->y2, &xi1 , &yi1 , &xi2 , &yi2);

    if (b_ret>0)
    {
        if (b_ret==2)
        {
            //choosing nearest_ point
            dl1=(X-xi1)*(X-xi1)+(Y-yi1)*(Y-yi1);
            dl2=(X-xi2)*(X-xi2)+(Y-yi2)*(Y-yi2);

            if (dl1<dl2)
            {
                *x=xi1;
                *y=yi1;
            } else {
                *x = xi2;
                *y = yi2;
            }
        }
        else {
            *x = xi1;
            *y = yi1;
        }
    }

   return b_ret;
}

int przeciecieLE_tt_prec_(double *x1, double *y1, double *x2, double *y2, void *adr, void *adr1)
{   NAGLOWEK *nag;
    LINIA *L;
    ELLIPSE *e;
    int b_ret;
    BOOL lay1=FALSE, lay2=FALSE;
    double AB, AP, PB;
    double x1_, y1_;


    nag=(NAGLOWEK*)adr;
    if (nag->obiekt==Olinia)
    {
        L=(LINIA*)adr;
        e=(ELLIPSE*)adr1;
    }
    else
    {
        L=(LINIA*)adr1;
        e=(ELLIPSE*)adr;
    }

    b_ret=EllipseIntersectLine(e->x, e->y, e->rx, e->ry, e->angle, L->x1 , L->y1 , L->x2 , L->y2, x1 , y1 , x2 , y2);

    if (b_ret>0) {

        //check if x1, y1 and x2, y2 lay on L
        AB = sqrt((L->x2 - L->x1) * (L->x2 - L->x1) + (L->y2 - L->y1) * (L->y2 - L->y1));
        AP = sqrt((*x1 - L->x1) * (*x1 - L->x1) + (*y1 - L->y1) * (*y1 - L->y1));
        PB = sqrt((L->x2 - *x1) * (L->x2 - *x1) + (L->y2 - *y1) * (L->y2 - *y1));
        if (Check_if_Equal(AB, AP + PB)) lay1 = TRUE;

        if (b_ret>1) {
            AP = sqrt((*x2 - L->x1) * (*x2 - L->x1) + (*y2 - L->y1) * (*y2 - L->y1));
            PB = sqrt((L->x2 - *x2) * (L->x2 - *x2) + (L->y2 - *y2) * (L->y2 - *y2));
            if (Check_if_Equal(AB, AP + PB)) lay2 = TRUE;
        }

        if ((lay1) && (lay2)) return b_ret;
        if (lay2) {

            x1_ = *x1;
            y1_ = *y1;

            *x1 = *x2;
            *y1 = *y2;
            *x2 = x1_;
            *y2 = y1_;
            return 1;
        }
        else return 1;
    }

    return b_ret;
}

int przeciecieLEA_tt_prec_(double *x1, double *y1, double *x2, double *y2, void *adr, void *adr1)
{   NAGLOWEK *nag;
    LINIA *L;
    ELLIPTICALARC *ea;
    int b_ret;
    BOOL lay1=FALSE, lay2=FALSE;
    double AB, AP, PB;
    double x1_, y1_;

    nag=(NAGLOWEK*)adr;
    if (nag->obiekt==Olinia)
    {
        L=(LINIA*)adr;
        ea=(ELLIPTICALARC*)adr1;
    }
    else
    {
        L=(LINIA*)adr1;
        ea=(ELLIPTICALARC*)adr;
    }

    b_ret=EllipseIntersectLine(ea->x, ea->y, ea->rx, ea->ry, ea->angle, L->x1 , L->y1 , L->x2 , L->y2, x1 , y1 , x2 , y2);

    if (b_ret>0) {

        //check if x1, y1 and x2, y2 lay on L
        AB = sqrt((L->x2 - L->x1) * (L->x2 - L->x1) + (L->y2 - L->y1) * (L->y2 - L->y1));
        AP = sqrt((*x1 - L->x1) * (*x1 - L->x1) + (*y1 - L->y1) * (*y1 - L->y1));
        PB = sqrt((L->x2 - *x1) * (L->x2 - *x1) + (L->y2 - *y1) * (L->y2 - *y1));
        if (Check_if_Equal(AB, AP + PB)) lay1 = TRUE;

        if (b_ret>1) {
            AP = sqrt((*x2 - L->x1) * (*x2 - L->x1) + (*y2 - L->y1) * (*y2 - L->y1));
            PB = sqrt((L->x2 - *x2) * (L->x2 - *x2) + (L->y2 - *y2) * (L->y2 - *y2));
            if (Check_if_Equal(AB, AP + PB)) lay2 = TRUE;
        }

        if ((lay1) && (lay2)) return b_ret;
        if (lay2) {

            x1_ = *x1;
            y1_ = *y1;

            *x1 = *x2;
            *y1 = *y2;
            *x2 = x1_;
            *y2 = y1_;
            return 1;
        }
        else return 1;
    }

    return b_ret;
}


int przeciecieLE_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Oellipse)
        return ellipse_przec(x, y, adr, adr1, przeciecieLL_);
    else return ellipse_przec(x, y, adr1, adr, przeciecieLL_);
}

int przeciecielE_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Oellipse)
        return ellipse_przec(x, y, adr, adr1, przeciecieLl_);
    else return ellipse_przec(x, y, adr1, adr, przeciecieLl_);
}

int przeciecielE_r(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Oellipse)
        return przeciecielE_(x, y, adr, adr1);
    else return przeciecielE_(x, y, adr1, adr);
}

int przeciecieSAE_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Oellipse)
        return solidarc_przec(x, y, adr1, adr, przeciecieLE_, przeciecielE_r);
    else return solidarc_przec(x, y, adr, adr1, przeciecieLE_, przeciecielE_r);
}

int przeciecieVE_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Oellipse)
        return vector_przec(x, y, adr1, adr, przeciecieLE_, przeciecielE_r);
    else return vector_przec(x, y, adr, adr1, przeciecieLE_, przeciecielE_r);
}

int przeciecieEO_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Ookrag)
        return ellipse_przec(x, y, adr1, adr, przeciecieLO_);
    else return ellipse_przec(x, y, adr, adr1, przeciecieLO_);
}

int przeciecieWE_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Oellipse)
        return ellipse_przec(x, y, adr, adr1, przeciecieLW_);
    else return ellipse_przec(x, y, adr1, adr, przeciecieLW_);
}

int przeciecieLEA_prec_(double *x, double *y, void *adr, void *adr1)
{  NAGLOWEK *nag;
    LINIA *L;
    ELLIPTICALARC *ea;
    int b_ret;
    double xi1, xi2 , yi1 , yi2;
    double dl1, dl2, dx, dy;
    double angle, angle1, angle2;

    nag=(NAGLOWEK*)adr;
    if (nag->obiekt==Olinia)
    {
        L=(LINIA*)adr;
        ea=(ELLIPTICALARC*)adr1;
    }
    else
    {
        L=(LINIA*)adr1;
        ea=(ELLIPTICALARC*)adr;
    }

    b_ret=EllipseIntersectLine(ea->x, ea->y, ea->rx, ea->ry, ea->angle, L->x1 , L->y1 , L->x2 , L->y2, &xi1 , &yi1 , &xi2 , &yi2);

    if (b_ret>0)
    {
        if (b_ret==2)
        {
            //choosing nearest_ point
            dl1=(X-xi1)*(X-xi1)+(Y-yi1)*(Y-yi1);
            dl2=(X-xi2)*(X-xi2)+(Y-yi2)*(Y-yi2);

            if (dl1<dl2)
            {
                *x=xi1;
                *y=yi1;
            } else {
                *x = xi2;
                *y = yi2;
            }
        }
        else {
            *x = xi1;
            *y = yi1;
        }
    }

    //check if point on elliptical arc
    if (Check_if_Equal(*x, ea->x))
    {
        if (*y>ea->y) angle=Pi/2.0;
        else angle=3*Pi/2;
    }
    else {
        dy=*y - ea->y;
        dx=*x - ea->x;
        angle=Atan2(dy, dx);
    }

    angle=Angle_Normal(angle);

    angle1=ea->angle+ea->kat1;
    angle2=ea->angle+ea->kat2;
    angle1= Angle_Normal(angle1);
    angle2= Angle_Normal(angle2);

    if (angle2<angle1)
    {
        angle2+=Pi2;
        if ((angle<angle1) && ((angle+Pi2)>angle2)) b_ret=0;
    }
    else
    {
        if ((angle < angle1) || (angle > angle2)) b_ret = 0;
    }

    return b_ret;
}


int Check_EllipticalArc_Inter(int b_ret0, double *x, double *y, void *adr)
{
    double angle, angle1, angle2;
    int b_ret;
    double dx, dy;
    ELLIPTICALARC *ea;

    ea = (ELLIPTICALARC *) adr;

    b_ret=b_ret0;

    if (b_ret > 0) {
        //check if point on elliptical arc
        if (Check_if_Equal(*x, ea->x)) {
            if (*y > ea->y) angle = Pi / 2.0;
            else angle = 3 * Pi / 2;
        } else {
            dy = *y - ea->y;
            dx = *x - ea->x;
            angle = Atan2(dy, dx);
        }

        angle = Angle_Normal(angle);

        angle1 = ea->angle + ea->kat1;
        angle2 = ea->angle + ea->kat2;
        angle1 = Angle_Normal(angle1);
        angle2 = Angle_Normal(angle2);

        if (angle2 < angle1) {
            angle2 += Pi2;
            if ((angle < angle1) && ((angle + Pi2) > angle2)) b_ret = 0;
        } else {
            if ((angle < angle1) || (angle > angle2)) b_ret = 0;
        }
    }

    return b_ret;
}

int przeciecieLEA_(double *x, double *y, void *adr, void *adr1)
{
    int b_ret;

    if (((NAGLOWEK*)adr)->obiekt==Oellipticalarc) {
        b_ret = ellipse_przec(x, y, adr, adr1, przeciecieLL_);
        return Check_EllipticalArc_Inter(b_ret, x, y, adr);
    }
    else {
        b_ret = ellipse_przec(x, y, adr1, adr, przeciecieLL_);
        return Check_EllipticalArc_Inter(b_ret, x, y, adr1);
    }
}

int przeciecieEAO_(double *x, double *y, void *adr, void *adr1)
{
    int b_ret;

    if (((NAGLOWEK*)adr)->obiekt==Ookrag) {
        b_ret = ellipse_przec(x, y, adr1, adr, przeciecieLO_);
        return Check_EllipticalArc_Inter(b_ret, x, y, adr1);
    }
    else
    {
        b_ret = ellipse_przec(x, y, adr, adr1, przeciecieLO_);
        return Check_EllipticalArc_Inter(b_ret, x, y, adr);
    }
}

int przeciecieWEA_(double *x, double *y, void *adr, void *adr1)
{
    int b_ret;

    if (((NAGLOWEK*)adr)->obiekt==Oellipticalarc) {
        b_ret = ellipse_przec(x, y, adr, adr1, przeciecieLW_);
        return Check_EllipticalArc_Inter(b_ret, x, y, adr);
    }
    else
    {
        b_ret = ellipse_przec(x, y, adr1, adr, przeciecieLW_);
        return Check_EllipticalArc_Inter(b_ret, x, y, adr1);
    }
}

int przeciecieSEA_(double *x, double *y, void *adr, void *adr1)
{
    int b_ret;

    if (((NAGLOWEK*)adr)->obiekt==Oellipticalarc) {
        b_ret = spline_przec(x, y, adr, adr1, przeciecieLE_);
        return Check_EllipticalArc_Inter(b_ret, x, y, adr1);
    }
    else
    {
        b_ret = spline_przec(x, y, adr1, adr, przeciecieLE_);
        return Check_EllipticalArc_Inter(b_ret, x, y, adr);
    }
}

int przeciecieEEA_(double *x, double *y, void *adr, void *adr1)
{
    int b_ret;

    if (((NAGLOWEK*)adr)->obiekt==Oellipticalarc) {
        b_ret = ellipse_przec(x, y, adr, adr1, przeciecieLE_);
        return Check_EllipticalArc_Inter(b_ret, x, y, adr);
    }
    else
    {
        b_ret = ellipse_przec(x, y, adr1, adr, przeciecieLE_);
        return Check_EllipticalArc_Inter(b_ret, x, y, adr1);
    }
}

int przeciecieEAEA_(double *x, double *y, void *adr, void *adr1)
{
    int b_ret, ret1;

    b_ret=ellipse_przec(x, y, adr1, adr, przeciecieLE_);

    ret1=Check_EllipticalArc_Inter(b_ret, x, y, adr1);
    if (ret1>0) return Check_EllipticalArc_Inter(b_ret, x, y, adr);
    else return ret1;
}

int przeciecielEA_(double *x, double *y, void *adr, void *adr1)
{
    int b_ret;

    if (((NAGLOWEK*)adr)->obiekt==Oellipticalarc) {
        b_ret = ellipse_przec(x, y, adr, adr1, przeciecieLl_);
        return Check_EllipticalArc_Inter(b_ret, x, y, adr);
    }
    else
    {
        b_ret = ellipse_przec(x, y, adr1, adr, przeciecieLl_);
        return Check_EllipticalArc_Inter(b_ret, x, y, adr1);
    }
}

int przeciecielEA_r(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Oellipticalarc)
        return przeciecielEA_(x, y, adr, adr1);
    else return przeciecielEA_(x, y, adr1, adr);

}

int przeciecieSAEA_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Oellipticalarc)
     return  solidarc_przec(x, y, adr1, adr, przeciecieLEA_, przeciecielEA_r);
    else return  solidarc_przec(x, y, adr, adr1, przeciecieLEA_, przeciecielEA_r);
}

int przeciecieVEA_(double *x, double *y, void *adr, void *adr1)
{
    if (((NAGLOWEK*)adr)->obiekt==Oellipticalarc)
    return  vector_przec(x, y, adr1, adr, przeciecieLEA_, przeciecielEA_r);
    else return  vector_przec(x, y, adr, adr1, przeciecieLEA_, przeciecielEA_r);
}
/*------------------------------------------------------------*/

void najblizszyl_ (double *x, double *y, void *adr)
/*------------------------------------------------*/
{
  OKRAG  okrag = Odef ;
  LUK * ptrs_arc ;
  double df_angle, x1, y1, x2, y2 ;

  ptrs_arc = (LUK*)adr ;
  okrag.x = ptrs_arc->x ;
  okrag.y = ptrs_arc->y ;
  okrag.r = ptrs_arc->r ;
  najblizszyO_ (x, y, (void *)&okrag) ;

  df_angle = Atan2 (*y - ptrs_arc->y, *x - ptrs_arc->x) ;
  if (FALSE == Check_Angle (df_angle, ptrs_arc->kat1, ptrs_arc->kat2))
  {
    x1 = ptrs_arc->x + ptrs_arc->r * cos (ptrs_arc->kat1) ;
    y1 = ptrs_arc->y + ptrs_arc->r * sin (ptrs_arc->kat1) ;
    x2 = ptrs_arc->x + ptrs_arc->r * cos (ptrs_arc->kat2) ;
    y2 = ptrs_arc->y + ptrs_arc->r * sin (ptrs_arc->kat2) ;
    if (dP1P2  (*x, *y, x1, y1) > dP1P2  (*x, *y, x2, y2))
    {
      *x = x2 ;
      *y = y2 ;
    }
    else
    {
      *x = x1 ;
      *y = y1 ;
    }
  }
}


void koniecSpline_(double *x, double *y, void *adr)
{
	End_Spline(x, y, X, Y, (SPLINE *)adr);
}

void koniecl_(double *x,double *y,void *adr)
{
  End_Arc (x, y, X, Y, (LUK *)adr);
}

void koniec1l_(double *x,double *y,void *adr)
{
    if (End_Arc (x, y, X, Y, (LUK *)adr))
    {
        if (global_set_stretch) set_stretch_vector(2, adr); //stretching radius of moment or rotation
    }
    else
    {
        if (global_set_stretch) set_stretch_vector(3, adr); //stretching angle and value of moment or rotation
    }
}

void koniec1el_(double *x,double *y,void *adr)
{
    if (End_Elliptical_Arc (x, y, X, Y, (ELLIPTICALARC *)adr))
    {
        if (global_set_stretch) set_stretch_vector(2, adr); //stretching radius of moment or rotation
    }
    else
    {
        if (global_set_stretch) set_stretch_vector(3, adr); //stretching angle and value of moment or rotation
    }
}

void koniec1O_(double *x,double *y,void *adr)
{
    if (global_set_stretch) set_stretch_vector(8, adr); //stretching angle and value of moment or rotation
}

void koniecEA_XY_(double *x,double *y,int numpoints, double xy[])
{  BOOL ret;
   ret = End_Elliptical_Arc_XY (x, y, X, Y, numpoints, xy);
}

void srodekl_(double *x,double *y,void *adr)
{ LUK *l;
  double a1,a2;
  l=(LUK*)adr;
  a1 = Angle_Normal (l->kat1) ;
  a2 = Angle_Normal (l->kat2) ;
  if(a1>a2) a2+=Pi2;
  *x=l->x+l->r*cos((a1+a2)/2);
  *y=l->y+l->r*sin((a1+a2)/2);
}
////////////////////////

// Helper: Integrated function for elliptical arc length: sqrt((dx/dt)^2 + (dy/dt)^2)
double ellipse_arc_integrand(double t, double xr, double yr) {
    double dx = -xr * sin(t);
    double dy = yr * cos(t);
    return sqrt(dx * dx + dy * dy);
}

// Numerical Integration using Simpson's Rule
double get_arc_length(double t1, double t2, double xr, double yr) {
    int n = 100; // Even number of intervals
    double h = (t2 - t1) / n;
    double sum = ellipse_arc_integrand(t1, xr, yr) + ellipse_arc_integrand(t2, xr, yr);

    for (int i = 1; i < n; i++) {
        double t = t1 + i * h;
        sum += (i % 2 == 0 ? 2 : 4) * ellipse_arc_integrand(t, xr, yr);
    }
    return (h / 3.0) * sum;
}

// Finding the arc-length midpoint via binary search
void srodekea_true_(double *x,double *y,void *adr)  //slow, most precise
{
    ELLIPTICALARC *ea;
    ea=(ELLIPTICALARC*)adr;

    // 1. Convert to parametric angles t1, t2 (ensure t1 < t2)
    double t1 = atan2f(ea->rx * sinf(ea->kat1), ea->ry * cosf(ea->kat1));
    double t2 = atan2f(ea->rx * sinf(ea->kat2), ea->ry * cosf(ea->kat2));
    while (t2 < t1) t2 += 2.0 * M_PI;

    // 2. Target length is half the total segment length
    double total_length = get_arc_length(t1, t2, ea->rx, ea->ry);
    double target_length = total_length / 2.0;

    // 3. Binary search for t_mid where arc_length(t1, t_mid) == target_length
    double low = t1, high = t2, t_mid = t1;
    for (int i = 0; i < 20; i++) { // 20 iterations for high precision
        t_mid = (low + high) / 2.0;
        if (get_arc_length(t1, t_mid, ea->rx, ea->ry) < target_length) low = t_mid;
        else high = t_mid;
    }

    // 4. Transform to global coordinates (same as before)
    double x_local = ea->rx * cos(t_mid);
    double y_local = ea->ry * sin(t_mid);
    double cos_a = cosf(ea->angle), sin_a = sinf(ea->angle);

    *x = ea->x + (x_local * cos_a - y_local * sin_a);
    *y = ea->y + (x_local * sin_a + y_local * cos_a);
}
////////////////////////

void srodekea_fast_(double *x,double *y,void *adr) //very fast but not precise
{
  ELLIPTICALARC *ea;
  ea=(ELLIPTICALARC*)adr;

    // 1. Convert geometric angles to parametric 't'
    double t_start = atan2f(ea->rx * sinf(ea->kat1), ea->ry * cosf(ea->kat1));
    double t_end   = atan2f(ea->rx * sinf(ea->kat2), ea->ry * cosf(ea->kat2));

    // 2. ENFORCE ORDERING: Mirror the ang1 < ang2 logic
    // Since atan2 returns [-PI, PI], t2 might be less than t1 even if ang2 > ang1.
    // We add 2*PI increments until t2 is greater than t1 to match the arc sweep.
    while (t_end < t_start) { t_end += 2.0 * M_PI;}

    // Handle wrap-around for the shortest arc
    //if (fabs(t_end - t_start) > M_PI) {
    //    if (t_end > t_start) t_start += 2.0 * M_PI;
    //    else t_end += 2.0 * M_PI;
    //}

    // 2. Find midpoint parametric angle
    double t_mid = (t_start + t_end) / 2.0;

    // 3. Local coordinates (unrotated)
    double x_local = ea->rx * cos(t_mid);
    double y_local = ea->ry * sin(t_mid);

    // 4. Global rotation and translation
    double cos_a = cosf(ea->angle);
    double sin_a = sinf(ea->angle);

    *x = ea->x + (x_local * cos_a - y_local * sin_a);
    *y = ea->y + (x_local * sin_a + y_local * cos_a);
}

void srodekea_(double *x,double *y, double *tangent, void *adr)  //fast and realtively precise
{
//Point get_true_arc_length_midpoint_fast(double cx, double cy, double xr, double yrdouble main_axis_angle, double ang1, double ang2)
    ELLIPTICALARC *ea;
    ea=(ELLIPTICALARC*)adr;
    // 1. Convert to parametric angles t1, t2
    double t1 = atan2f(ea->rx * sinf(ea->kat1), ea->ry * cosf(ea->kat1));
    double t2 = atan2f(ea->rx * sinf(ea->kat2), ea->ry * cosf(ea->kat2));
    while (t2 < t1) t2 += 2.0 * M_PI;

    // 2. Define the Target Length (Half of the total arc)
    double total_length = get_arc_length(t1, t2, ea->rx, ea->ry);
    double target_length = total_length / 2.0;

    // 3. Newton-Raphson Solver
    // We want to find t_mid such that: f(t_mid) = get_arc_length(t1, t_mid) - target_length = 0
    double t_mid = (t1 + t2) / 2.0; // Initial guess (parametric mid)

    for (int i = 0; i < 8; i++) { // 8 iterations provide extreme CAD precision
        double current_len = get_arc_length(t1, t_mid, ea->rx, ea->ry);
        double error = current_len - target_length;

        // The derivative of our function f(t) is just the integrand at t_mid
        double derivative = ellipse_arc_integrand(t_mid, ea->rx, ea->ry);

        // Newton's step: x_new = x_old - f(x)/f'(x)
        t_mid = t_mid - (error / derivative);
    }

    // 4. Final Transformation
    double x_local = ea->rx * cos(t_mid);
    double y_local = ea->ry * sin(t_mid);
    double cos_a = cosf(ea->angle);
    double sin_a = sinf(ea->angle);

    *x = ea->x + (x_local * cos_a - y_local * sin_a);
    *y = ea->y + (x_local * sin_a + y_local * cos_a);

    double local_tan_angle = atan2(ea->ry * cos(t_mid), -ea->rx * sin(t_mid));
    *tangent = ea->angle + local_tan_angle;
}

/**
 * Calculates the absolute tangent angle directly from a parametric angle 't'.
 * Use this with the t_mid returned by your arc-length solver.
 */
double get_tangent_angle_from_parametric(double main_axis_angle, double xr, double yr, double t) {
    // Local tangent vector components: dx/dt = -xr*sin(t), dy/dt = yr*cos(t)
    double local_tan_angle = atan2(yr * cos(t), -xr * sin(t));

    // Global rotation
    return main_axis_angle + local_tan_angle;
}

static void centrumO_(double *x,double *y,void *adr)
{ OKRAG *O;
  O=(OKRAG*)adr;
  *x=O->x;
  *y=O->y;
}

static void centrumE_(double *x,double *y,void *adr)
{ ELLIPSE *e;
    e=(ELLIPSE*)adr;
    *x=e->x;
    *y=e->y;
}

static void centruml_(double *x,double *y,void *adr)
{ LUK *l;
  l=(LUK*)adr;
  *x=l->x;
  *y=l->y;
}

static BOOL centrum_vector_(double *x,double *y,void *adr)
{   AVECTOR *v;
    v=(LUK*)adr;
    switch (v->style)
    {
        case 5:
        case 6:
        case 8:
        case 9:
        case 16:
        *x = v->x1;
        *y = v->y1;
        return TRUE;
        break;
        default:
            return FALSE;
        break;
    }
}

static void centrumel_(double *x,double *y,void *adr)
{ ELLIPTICALARC *el;
    el=(ELLIPTICALARC*)adr;
    *x=el->x;
    *y=el->y;
}

void Polygon_Center (double x1p, double y1p, double x2p, double y2p,
		int n, double *xc, double *yc)
/*------------------------------------------------------------*/
{
/*wyznaczenie srodka wielokata*/
  double df_dangle, si, co, a ;
  double x, y , x0, y0, x2, y2 ;

  if (n < 3)
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
  df_dangle = Pi2 / (2 * n) ;
  x = (x2p + x1p) / 2 ;
  y = (y2p + y1p) / 2 ;
  x2 = x2p - x ;
  y2 = y2p - y ;
  a = Atan2 (y2, x2) ;
  si = sin (a) ;
  co = cos (a) ;
  x0 = 0 ;
  y0 = dP1P2 (0, 0, x2, y2) / tan (df_dangle) ;
  obrd (si, co, x0, y0, &x0, &y0) ;
  x0 += x ;
  y0 += y ;
  *xc = x0 ;
  *yc = y0 ;
}


static BOOL centrum_poly_ (double *x, double *y, void *adr)
/*--------------------------------------------------------*/
{
  BLOK *ptrs_b ;
  int n ;
  LINIA *ptrs_l ;

  if (NULL == (ptrs_b = Parent_Block ((char  *)adr)))
  {
    Internal_Error (__LINE__,__FILE__);
    return FALSE ;
  }
  if (ptrs_b->obiektt1 != OB1NOCHANGE)
  {
    return FALSE ;
  }
  n = Get_Polygon_Num (ptrs_b) ;
  ptrs_l = (LINIA*)((char *)ptrs_b +
		sizeof(NAGLOWEK) + B3 + ptrs_b->dlugosc_opisu_obiektu) ;
  if (ptrs_l->obiekt != Olinia)
  {
    return FALSE ;
  }
  Polygon_Center (ptrs_l->x1, ptrs_l->y1, ptrs_l->x2, ptrs_l->y2, n, x, y) ;
  return TRUE ;
}

static BOOL centrum_solidarc_ (double *x, double *y, void *adr)
{
    SOLIDARC *sa;
    LINIA L_left, L_right;
    LUK l_inner1, l_inner2, l_outer1, l_outer2;
    int ln;

    sa = (SOLIDARC *) adr;
    ln=solidarc_elements(sa, &L_left, &L_right, &l_inner1, &l_inner2, &l_outer1, &l_outer2);

    if (0 != linia_wybrana (&L_left))
    {
        *x=sa->x;
        *y=sa->y;
        return TRUE;
    }

    if (0 != linia_wybrana (&L_right))
    {
        *x=sa->x;
        *y=sa->y;
        return TRUE;
    }

    if (0 != luk_wybrany (&l_inner1))
    {
        *x=l_inner1.x;
        *y=l_inner1.y;
        return TRUE;
    }

    if (0 != luk_wybrany (&l_outer1))
    {
        *x=l_outer1.x;
        *y=l_outer1.y;
        return TRUE;
    }

    if (ln>1)
    {
        if (0 != luk_wybrany (&l_inner2))
        {
            *x=l_inner2.x;
            *y=l_inner2.y;
            return TRUE;
        }

        if (0 != luk_wybrany (&l_outer2))
        {
            *x=l_outer2.x;
            *y=l_outer2.y;
            return TRUE;
        }
    }

    return FALSE;
}

static int styczniel_(double *x,double *y,void *adr)
/*-------------------------------------------------*/
{
  double l,Lx,Ly,si,co,e,F;
  double x1,y1;
  LINIA *L;
  LUK *luk;
  if(!poczL) return 0;
  L=&LiniaG;
  luk=(LUK*)adr;
  l=sqrt((L->x1-luk->x)*(L->x1-luk->x)+(L->y1-luk->y)*(L->y1-luk->y));
  if(l < luk->r || TRUE == Check_if_Equal (luk->r, 0))
   {
     return 0 ;
   }
  Lx=L->x1-luk->x; Ly=L->y1-luk->y;
  si=Ly/l;  co=Lx/l;
  obru(si,co,X-luk->x,Y-luk->y,&x1,&y1);
  e=y1>0 ? 1:-1;
  F=luk->r/l;
  obrd(si,co,luk->r*F,e*luk->r*sqrt(fabs(1-F*F)),&x1,&y1);
  x1+=luk->x;
  y1+=luk->y;
  if (Point_Arc ( x1,  y1, luk))
   {
     *x = x1;
     *y = y1;
     return 1;
   }
  else
   {
     e *= -1;
     obrd(si,co,luk->r*F,e*luk->r*sqrt(fabs(1-F*F)),&x1,&y1);
     x1+=luk->x;
     y1+=luk->y;
     if (Point_Arc ( x1,  y1, luk))
      {
	*x = x1;
	*y = y1;
	return 1;
      }
   }
  return 0;
}

static int stycznieO_(double *x,double *y,void *adr)
{ double l,Lx,Ly,si,co,e,F;
  LINIA *L;
  OKRAG *O;
  if(!poczL) return 0;
  L=&LiniaG;
  O=(OKRAG*)adr;
  l=sqrt((L->x1-O->x)*(L->x1-O->x)+(L->y1-O->y)*(L->y1-O->y));
  if (l < O->r || TRUE == Check_if_Equal (O->r, 0))
  {
    return 0 ;
  }
  Lx=L->x1-O->x; Ly=L->y1-O->y;
  si=Ly/l;  co=Lx/l;
  obru(si,co,X-O->x,Y-O->y,x,y);
  e=*y>0 ? 1:-1;
  F=O->r/l;
  obrd(si,co,O->r*F,e*O->r*sqrt(fabs(1-F*F)),x,y);
  *x+=O->x;
  *y+=O->y;
  return 1;
}

static int stycznieE_(double *x,double *y,void *adr)
{ double l,Lx,Ly,si,co,e,F;
    LINIA *L;
    ELLIPSE *E;
    int ret;
    double m1, h1, m2, h2;
    double xx[2], yy[2];
    double dl1, dl2;

    if(!poczL) return 0;
    L=&LiniaG;
    E=(ELLIPSE*)adr;


    ret=find_tangent_lines_in_ellipse(E->x, E->y, E->rx, E->ry, E->angle, L->x1, L->y1, &m1, &h1, &m2, &h2, xx, yy);
    if (!ret) return 0;

    dl1=sqrt((xx[0]-(*x))*(xx[0]-(*x)) + (yy[0]-(*y))*(yy[0]-(*y)));
    dl2=sqrt((xx[1]-(*x))*(xx[1]-(*x)) + (yy[1]-(*y))*(yy[1]-(*y)));

    if (dl1<dl2) {*x=xx[0]; *y=yy[0];}
    else {*x=xx[1]; *y=yy[1];}

    return 1;
}

static int stycznieEA_(double *x,double *y,void *adr)
{
    double l,Lx,Ly,si,co,e,F;
    LINIA *L;
    ELLIPTICALARC *EA;
    int ret;
    double m1, h1, m2, h2;
    double xx[2], yy[2];
    double dl1, dl2, dx, dy;
    double angle, angle1, angle2;
    int p_ok[2];

    if(!poczL) return 0;
    L=&LiniaG;
    EA=(ELLIPTICALARC*)adr;

    ret=find_tangent_lines_in_ellipse(EA->x, EA->y, EA->rx, EA->ry, EA->angle, L->x1, L->y1, &m1, &h1, &m2, &h2, xx, yy);
    if (!ret) return 0;

    if ((Check_if_Equal(EA->kat1, 0.0)==FALSE) || (Check_if_Equal(EA->kat2, Pi2)==FALSE)) {
        //check if point on elliptical arc

        for (int i=0; i<2; i++) {
            if (Check_if_Equal(xx[i], EA->x)) {
                if (yy[i] > EA->y) angle = Pi / 2.0;
                else angle = 3 * Pi / 2;
            } else {
                dy = yy[i] - EA->y;
                dx = xx[i] - EA->x;
                angle = Atan2(dy, dx);
            }

            angle = Angle_Normal(angle);

            angle1 = EA->angle + EA->kat1;
            angle2 = EA->angle + EA->kat2;
            angle1 = Angle_Normal(angle1);
            angle2 = Angle_Normal(angle2);

            p_ok[i] = 1;

            if (angle2 < angle1) {
                angle2 += Pi2;
                if ((angle < angle1) && ((angle + Pi2) > angle2)) p_ok[i] = 0;
            } else {
                if ((angle < angle1) || (angle > angle2)) p_ok[i] = 0;
            }
        }
    }

    if (p_ok[0]==1) dl1=sqrt((xx[0]-(*x))*(xx[0]-(*x)) + (yy[0]-(*y))*(yy[0]-(*y)));

    if (p_ok[1]==1) dl2=sqrt((xx[1]-(*x))*(xx[1]-(*x)) + (yy[1]-(*y))*(yy[1]-(*y)));

    if (p_ok[0]==1)
    {
        if (p_ok[1]==1)
        {
            if (dl1<dl2) {*x=xx[0]; *y=yy[0];}
            else {*x=xx[1]; *y=yy[1];}
        }
        else {*x=xx[0]; *y=yy[0];}
    }
    else
    {
        if (p_ok[1]==1)
        {
            {*x=xx[1]; *y=yy[1];}
        }
        else return 0;
    }

    return 1;
}
/*----------------------------------------------------------*/

void srodek(void)
{
  unsigned typ;
  void  *adr;
  int ret;
  typ=Blinia | Bluk | Bwwielokat | Bsolidarc | Bvector | Bellipticalarc;
  if ((adr=select_n(&typ,NULL,68))!=NULL)
   { Xsel=X;Ysel=Y;
     switch(typ)
      { case Blinia : srodekL_(&Xsel,&Ysel,adr);
		      break;
	case Bluk   : srodekl_(&Xsel,&Ysel,adr);
		      break;
	case Bwwielokat: wielokat_ (&Xsel, &Ysel, adr, srodekL_) ;
		      break;
    case Bsolidarc: ret=wsolidarc_ (&Xsel, &Ysel, adr, srodekL_, srodekl_) ;
              break;
    case Bvector: ret=wvector_ (&Xsel, &Ysel, adr, srodekL_, srodekl_, srodekL_, srodekl_) ;
              break;
    case Bellipticalarc: wellipticalarc_ea_(&Xsel, &Ysel, adr, srodekea_) ;
	default     : break;
      }
     sel.wyj=1;
   }
}

void najblizszyP_ (double *x, double *y, void *adr)
{ T_Point *ptrs_point ;

  ptrs_point = (T_Point*)adr ;
  *x = ptrs_point->x ;
  *y = ptrs_point->y ;
}


void najblizszy(void)
{ unsigned typ;
  void  *adr;
  int l=0;
  BOOL ret;
  
  typ=Blinia | Bokrag | Bkolo | Bluk | Bwwielokat | Bpoint | Bspline | Bellipse | Bfilledellipse | Bellipticalarc | Bsolidarc | Bvector;
  if ((adr=select_n(&typ,NULL,73))!=NULL)
   { Xsel=X;Ysel=Y;
     switch(typ)
      { case Blinia : najblizszyL_(&Xsel,&Ysel,adr);
		      break;
	case Bkolo   :
	case Bokrag : najblizszyO_(&Xsel,&Ysel,adr);
		      break;
	case Bluk   : najblizszyl_(&Xsel,&Ysel,adr);
		      break;
    case Bellipse   :
    case Bfilledellipse   : ret=najblizszyE_(&Xsel,&Ysel,adr);
    if (!ret) {
        sel.wyj=0;
        return;
    }
              break;
    case Bellipticalarc   : ret=najblizszyEA_(&Xsel,&Ysel,adr);
              if (!ret) {
                  sel.wyj=0;
                  return;
              }
              break;
	case Bwwielokat: wielokat_ (&Xsel, &Ysel, adr, najblizszyL_) ;
		      break;
   case Bpoint: najblizszyP_(&Xsel,&Ysel,adr);
            break;
   case Bspline: l = mwspline_(&Xsel, &Ysel, adr, najblizszyLx_);
       break;
   case Bsolidarc: l = wsolidarc_(&Xsel, &Ysel, adr, najblizszyL_, najblizszyl_);
       break;
   case Bvector: l = wvector_(&Xsel, &Ysel, adr, najblizszyL_, najblizszyl_, najblizszyL_, najblizszyl_);
       break;
	default     : break;
      }
     sel.wyj=1;
   }
}


void centrum(void)
{
  unsigned typ;
  void  *adr;
  BOOL b_ret ;
  typ=Bluk | Bokrag | Bkolo | Bpolygon | Bellipse | Bfilledellipse | Bellipticalarc | Bsolidarc | Bvector;
  if ((adr=select_n(&typ,NULL,70))!=NULL)
   { Xsel=X;Ysel=Y;
     switch(typ)
     {
	case Bkolo  :
	case Bokrag : centrumO_(&Xsel,&Ysel,adr);
		      break;
     case Bellipse :
     case Bfilledellipse:
         centrumE_ (&Xsel,&Ysel,adr) ;
             break;
	case Bluk   : centruml_(&Xsel,&Ysel,adr);
		      break;
     case Bellipticalarc   : centrumel_ (&Xsel,&Ysel, adr) ;
             break;
	case Bpolygon: b_ret = centrum_poly_(&Xsel,&Ysel,adr);
		      break;
    case Bsolidarc: b_ret = centrum_solidarc_(&Xsel,&Ysel,adr);
             break;
     case Bvector: b_ret = centrum_vector_(&Xsel,&Ysel,adr);
             break;
	default     : break;
     }
     sel.wyj=1;
     if (typ == Bpolygon && b_ret == FALSE)
     {
       sel.wyj = 0 ;
     }
   }
}


BLOK *koniecP(double *xsel, double *ysel)
{ unsigned typ;
    void *adr;
    int ret;

    typ=Blinia | Bluk;
    if ((adr=select_n(&typ,NULL,67))!=NULL)
    { *xsel=X;*ysel=Y;
        switch(typ)
        {
            case Blinia :     koniecL_ (&xsel, &ysel, adr) ;
                break;
            case Bluk   :     koniecl_ (&xsel, &ysel, adr) ;
                break;
            default     :     break;
        }
        return FIRSTB(adr);
    }
    return NULL;
}

void koniec(void)
{ unsigned typ;
  void *adr;
  int ret;
  typ=Blinia | Bluk | Bwwielokat | Bspline | Bellipticalarc | Bsolidarc | Bvector;
  if ((adr=select_n(&typ,NULL,67))!=NULL)
   { Xsel=X;Ysel=Y;
     switch(typ)
     {
	case Blinia :     koniecL_ (&Xsel, &Ysel, adr) ;
			  break;
	case Bluk   :     koniecl_ (&Xsel, &Ysel, adr) ;
			  break;
	case Bwwielokat: wielokat_ (&Xsel, &Ysel, adr, koniecL_) ;
			  break;
    case Bspline: wspline_(&Xsel, &Ysel, adr, koniecL_);
              break;
    case Bellipticalarc: wellipticalarc_(&Xsel, &Ysel, adr, koniecL_);
             break;
     case Bsolidarc: ret = wsolidarc_(&Xsel, &Ysel, adr, koniecL_, koniecl_);
             break;
     case Bvector: ret = wvector_(&Xsel, &Ysel, adr, koniecL_, koniecl_, koniecL_, koniecl_);
             break;
	default     :     break;
     }
     sel.wyj=1;
   }
}


void koniecS(void)
{
  if (selectS(&Xsel,&Ysel,66)) sel.wyj=1;
}


void przeciecie(void)
{ unsigned typ;
  void  *adr,*adr1;
  int l=0;

  typ=Blinia | Bluk | Bkolo |  Bokrag |  Bwwielokat | Bspline |  Bellipse | Bfilledellipse | Bellipticalarc | Bprzeciecie | Bsolidarc | Bvector;
  if ((adr=select_n(&typ,&adr1,69))!=NULL  && adr1!=NULL )
   { Xsel=X;Ysel=Y;
     switch(typ)
      { case Blinia | Blinia : l=przeciecieLL_(&Xsel,&Ysel,adr,adr1);
				break;
	case Blinia | Bokrag : l=przeciecieLO_(&Xsel,&Ysel,adr,adr1);
				break;
	case Bokrag | Bokrag : l=przeciecieOO_(&Xsel,&Ysel,adr,adr1);
				break;
	case Blinia | Bluk   : l=przeciecieLl_(&Xsel,&Ysel,adr,adr1);
				break;
	case Bluk   | Bokrag : l=przeciecielO_(&Xsel,&Ysel,adr,adr1);
				break;
	case Bluk   | Bluk   : l=przecieciell_(&Xsel,&Ysel,adr,adr1);
				break;
	case Blinia | Bwwielokat: l=przeciecieLW_(&Xsel,&Ysel,adr,adr1);
				break;
	case Bluk   | Bwwielokat: l=przeciecielW_(&Xsel,&Ysel,adr,adr1);
                break;
	case Bwwielokat | Bokrag : l=przeciecieWO_(&Xsel,&Ysel,adr,adr1);
				break;
	case Bwwielokat | Bwwielokat: l=przeciecieWW_(&Xsel,&Ysel,adr,adr1);
				break;
    case Blinia | Bellipse:
    case Blinia | Bfilledellipse: l = przeciecieLE_(&Xsel, &Ysel, adr, adr1);
                break;
    case Blinia | Bellipticalarc: l = przeciecieLEA_(&Xsel, &Ysel, adr, adr1);
                break;
    case Bluk | Bellipse:
    case Bluk | Bfilledellipse: l = przeciecielE_(&Xsel, &Ysel, adr, adr1);
                break;
    case Bluk | Bellipticalarc: l = przeciecielEA_(&Xsel, &Ysel, adr, adr1);
                break;
    case Bokrag | Bellipse:
    case Bokrag | Bfilledellipse: l = przeciecieEO_(&Xsel, &Ysel, adr, adr1);
                break;
     case Bokrag | Bellipticalarc: l = przeciecieEAO_(&Xsel, &Ysel, adr, adr1);
                break;
      case Bwwielokat | Bellipse:
      case Bwwielokat | Bfilledellipse: l = przeciecieWE_(&Xsel, &Ysel, adr, adr1);
                break;
      case Bwwielokat | Bellipticalarc: l = przeciecieWEA_(&Xsel, &Ysel, adr, adr1);
                break;
      case Bspline | Bellipse:
      case Bspline | Bfilledellipse: l = przeciecieSE_(&Xsel, &Ysel, adr, adr1);
                break;
      case Bspline | Bellipticalarc: l = przeciecieSEA_(&Xsel, &Ysel, adr, adr1);
                break;
      case Bellipse | Bellipse:
      case Bellipse | Bfilledellipse: l = przeciecieEE_(&Xsel, &Ysel, adr, adr1);
                break;
      case Bellipse | Bellipticalarc: l = przeciecieEEA_(&Xsel, &Ysel, adr, adr1);
                break;
      case Bfilledellipse | Bfilledellipse: l = przeciecieEE_(&Xsel, &Ysel, adr, adr1);
                break;
      case Bfilledellipse | Bellipticalarc: l = przeciecieEEA_(&Xsel, &Ysel, adr, adr1);
                break;
      case Bellipticalarc | Bellipticalarc: l = przeciecieEAEA_(&Xsel, &Ysel, adr, adr1);
                break;
      case Blinia | Bspline: l = przeciecieLS_(&Xsel, &Ysel, adr, adr1);
                break;
      case Bluk | Bspline: l = przeciecielS_(&Xsel, &Ysel, adr, adr1);
                break;
	  case Bspline | Bokrag: l = przeciecieSO_(&Xsel, &Ysel, adr, adr1);
		        break;
	  case Bwwielokat | Bspline: l = przeciecieWS_(&Xsel, &Ysel, adr, adr1);
		        break;
	  case Bspline | Bspline: l = przeciecieSS_(&Xsel, &Ysel, adr, adr1);
		        break;
      case Bsolidarc   | Bwwielokat: l=przeciecieSAW_(&Xsel,&Ysel,adr,adr1);
                break;
      case Bsolidarc   | Bsolidarc: l=przeciecieSASA_(&Xsel,&Ysel,adr,adr1);
                break;
      case Bluk   | Bsolidarc: l=przeciecielSA_(&Xsel,&Ysel,adr,adr1);
                break;
      case Bsolidarc | Bokrag : l=przeciecieSAO_(&Xsel,&Ysel,adr,adr1);
                break;
      case Bsolidarc | Bellipse:
      case Bsolidarc | Bfilledellipse: l = przeciecieSAE_(&Xsel, &Ysel, adr, adr1);
                break;
      case Bsolidarc | Bellipticalarc: l = przeciecieSAEA_(&Xsel, &Ysel, adr, adr1);
                break;
      case Blinia | Bsolidarc: l=przeciecieLSA_(&Xsel,&Ysel,adr,adr1);
                break;
      case Bsolidarc | Bspline: l = przeciecieSAS_(&Xsel, &Ysel, adr, adr1);
                break;

      case Blinia | Bvector : l=przeciecieLV_(&Xsel,&Ysel,adr,adr1);
                break;
      case Bluk | Bvector : l=przeciecielV_(&Xsel,&Ysel,adr,adr1);
                break;
      case Bvector   | Bvector: l=przeciecieVV_(&Xsel,&Ysel,adr,adr1);
              break;
      case Bvector   | Bwwielokat: l=przeciecieVW_(&Xsel,&Ysel,adr,adr1);
              break;
      case Bvector | Bokrag : l=przeciecieVO_(&Xsel,&Ysel,adr,adr1);
              break;
      case Bvector | Bellipse:
      case Bvector | Bfilledellipse: l = przeciecieVE_(&Xsel, &Ysel, adr, adr1);
              break;
      case Bvector | Bellipticalarc: l = przeciecieVEA_(&Xsel, &Ysel, adr, adr1);
              break;
      case Bvector | Bspline: l = przeciecieVS_(&Xsel, &Ysel, adr, adr1);
              break;
	  default    : break;
      }
     if(l) sel.wyj=1;
   }
}


void prostopadle(void)
{ unsigned typ;
  void *adr;
  int l=0;
  l=0;
  again:
  typ=Blinia | Bokrag | Bluk | Bwwielokat | Bspline | Bellipse | Bfilledellipse | Bellipticalarc | Bsolidarc | Bvector;
  if ((adr=select_n(&typ,NULL,71))!=NULL)
   { Xsel=X;Ysel=Y;
     switch(typ)
      { case Blinia : l=prostopadleL_(&Xsel,&Ysel,adr) ;
		      break ;
	case Bokrag : l=prostopadleO_(&Xsel,&Ysel,adr) ;
		      break ;
	case Bluk   : l=prostopadlel_(&Xsel,&Ysel,adr) ;
		      break ;
	case Bwwielokat: l=prostopadlew_ (&Xsel, &Ysel, adr) ;
		      break ;
    case Bspline: l = prostopadles_(&Xsel, &Ysel, adr);
        if (!l) goto again;
        break;
    case Bellipse:
    case Bfilledellipse:
             l = prostopadleE_(&Xsel, &Ysel,adr);
        if (!l) goto again;
        break;
    case Bellipticalarc:
            l = prostopadleEA_(&Xsel, &Ysel,adr);
          if (!l) goto again;
        break;
    case Bsolidarc:
          l = prostopadleSA_(&Xsel, &Ysel,adr);
          if (!l) goto again;
          break;
      case Bvector : l=prostopadleV_(&Xsel,&Ysel,adr) ;
          break ;
	default     : break ;
      }
     if(l) sel.wyj=1 ;
   }
}

void stycznie(void)
{ unsigned typ;
  void *adr;
  int l=0;
  l=0;
  typ=Bluk | Bokrag | Bkolo | Bellipse | Bfilledellipse | Bellipticalarc | Bsolidarc;
  if ((adr=select_n(&typ,NULL,72))!=NULL)
   { Xsel=X;Ysel=Y;
     switch(typ)
      { case Bluk   : l = styczniel_(&Xsel,&Ysel,adr);
		      break;
	case Bokrag :
    case Bkolo :
        l = stycznieO_(&Xsel,&Ysel,adr);
		      break;
    case Bellipse :
    case Bfilledellipse :
        l = stycznieE_(&Xsel,&Ysel,adr);
              break;
    case Bellipticalarc :
        l = stycznieEA_(&Xsel,&Ysel,adr);
              break;
    case Bsolidarc :
        l = wsolidarc_(&Xsel, &Ysel, adr, NULL, (void (*)(double *, double *, void *)) styczniel_);
              break;
	default     : break;
      }
     if (l)
      {
	sel.wyj=1;
      }
   }
}


void Point_Sel (void)
/*------------------*/
{
  unsigned typ ;
  void *adr ;
  T_Point *ptrs_point ;

  typ = Bpoint ;
  if ((adr = select_n (&typ, NULL,74)) != NULL)
  {
    ptrs_point = (T_Point*)adr ;
    Xsel = ptrs_point->x ;
    Ysel = ptrs_point->y ;
    sel.wyj=1;
  }
}


void JumpB (void)
/*--------------*/
{
  char st [MaxTextLen] ;
  int retval_no = 2 ;
  double xd,yd,xd1,yd1,xp,yp;
  double buf_ret [2] ;
  double angle_l, sina, cosa;

  if (!sel.akt || sel.akw) return ;
  st[0]='\0';
  if (!read_esc(st,60,23))  return;
  if (0 == calculator (st, &retval_no, buf_ret)  || retval_no < 2)
  {
    return ;
  }
  if (options1.uklad_geodezyjny==0)
   {
    xp = jednostkiOb (buf_ret [0]);
    yp = jednostkiOb (buf_ret [1]);
   }
    else
     {
      xp = jednostkiOb (buf_ret [1]);
      yp = jednostkiOb (buf_ret [0]);
     }

  angle_l=get_angle_l();
  if (angle_l!=0)
  {
  sina=get_sina();
  cosa=get_cosa();
  xd1=(xp*cosa) - (yp*sina);
  yd1=(xp*sina) + (yp*cosa);
  }
  else
   {
   xd1=xp;
   yd1=yp;
   }

  xd = xd1 + get_localx();
  yd = yd1 + get_localy();

  Xsel = xd ;
  Ysel = yd ;
  sel.wyj = 1 ;
}


void JumpW (void)
/*-----------------*/
{
  char st [MaxTextLen] ;
  int retval_no = 2 ;
  double xd,yd,xp,yp;
  double buf_ret [2] ;
  double angle_l, sina, cosa;

  if (!sel.akt) return ;
  st[0]='\0';
  if (!read_esc(st,60,22))  return;
  if (0 == calculator (st, &retval_no, buf_ret)  || retval_no < 2)
  {
    return ;
  }
  if (options1.uklad_geodezyjny==0)
   {
    xp = jednostkiOb (buf_ret [0]) ;
    yp = jednostkiOb (buf_ret [1]) ;
   }
    else
     {
      xp = jednostkiOb (buf_ret [1]) ;
      yp = jednostkiOb (buf_ret [0]) ;
     }

  angle_l=get_angle_l();
  if (angle_l!=0)
  {
  sina=get_sina();
  cosa=get_cosa();
  xd=(xp*cosa) - (yp*sina);
  yd=(xp*sina) + (yp*cosa);
  }
  else
   {
   xd=xp;
   yd=yp;
   }

  if (sel.akw == 1)
  {
    sel.jump = 1 ;
    df__dxjump = xd ;
    df__dyjump = yd ;
  }
  else
  {
    Xsel = X + xd ;
    Ysel = Y + yd ;
    sel.wyj = 1 ;
  }
}

int JumpW_ (void)
/*-------------*/
{
  char st [MaxTextLen] ;
  int retval_no = 2 ;
  double xd,yd,xp,yp;
  double buf_ret [2] ;
  double angle_l, sina, cosa;

  if (!sel.akt) return 2;   //wyjscie
  st[0]='\0';
  if (!read_esc(st,60,22))  return 2;   //ESC
  if (0 == calculator (st, &retval_no, buf_ret)  || retval_no < 2)
  {
    return 0;   //blad
  }
  if (options1.uklad_geodezyjny==0)
   {
    xp = jednostkiOb (buf_ret [0]) ;
    yp = jednostkiOb (buf_ret [1]) ;
   }
    else
     {
      xp = jednostkiOb (buf_ret [1]) ;
      yp = jednostkiOb (buf_ret [0]) ;
     }

  angle_l=get_angle_l();
  if (angle_l!=0)
  {
  sina=get_sina();
  cosa=get_cosa();
  xd=(xp*cosa) - (yp*sina);
  yd=(xp*sina) + (yp*cosa);
  }
  else
   {
   xd=xp;
   yd=yp;
   }

  if (sel.akw == 1)
  {
    sel.jump = 1 ;
    df__dxjump = xd ;
    df__dyjump = yd ;
  }
  else
  {
    Xsel = X + xd ;
    Ysel = Y + yd ;
    sel.wyj = 1 ;
  }
  return 1;
}

void JumpL (void)
/*---------------*/
{
  char st [MaxTextLen] ;
  int retval_no = 2 ;
  double df_l, df_angle, df_dx, df_dy ;
  double buf_ret [2] ;
  double angle_l, df_angle1;

  if (!sel.akt) return ;
  st[0]='\0';
  if (!read_esc(st,60, 30))  return;
  if (0 == calculator (st, &retval_no, buf_ret)  || retval_no < 2)
  {
    return ;
  }
  df_l = jednostkiOb (buf_ret [0]) ;
  angle_l=get_angle_l();
  if (angle_l!=0)
  {
  df_angle1 = buf_ret [1] + angle_l;
  if (df_angle1<0) df_angle1+=360;
  }
   else df_angle1 = buf_ret [1];
  df_angle = df_angle1 * Pi / 180 ;
  df_dx = df_l * cos (df_angle) ;
  df_dy = df_l * sin (df_angle) ;
  if (sel.akw == 1)
  {
    sel.jump = 1 ;
    df__dxjump = df_dx ;
    df__dyjump = df_dy ;
  }
  else
  {
    Xsel = X + df_dx ;
    Ysel = Y + df_dy ;
    sel.wyj = 1 ;
  }
}



/*----------------------------------------------------------*/

void qkoniec_o(double *x,double *y, void *adr)
{ unsigned obiekt;
    //void *adr;
    NAGLOWEK *nag;
    int ret;

    obiekt=((NAGLOWEK*)adr)->obiekt;

    if (obiekt==Olinia) koniecL_(x,y,adr);
        else if (obiekt==Oluk) koniecl_(x,y,adr);
        else if (obiekt == Owwielokat) wielokat_(x, y, adr, koniecL_) ;
        else if (obiekt == Ospline) wspline_(x, y, adr, koniecL_);
        else if (obiekt == Oellipticalarc) wellipticalarc_(x, y, adr, koniecL_);
        else if (obiekt == Osolidarc) wsolidarc_(x, y, adr, koniecL_, koniecl_);
        else if (obiekt == Ovector)
        {
            if (((AVECTOR*)adr)->style==16) ret=wvector_(x, y, adr, koniecL_, koniecl_, koniec1L_, koniec1O_);
            else ret=wvector_(x, y, adr, koniecL_, koniecl_, koniec1L_, koniec1l_);
            if (stretch_vector!=0) VectorS=adr;
            if (ret==-1)
            {
#ifdef LINUX
                *x=0.0/0.0;
                *y=0.0/0.0;
#else          
                *x= NAN;
                *y = NAN;
#endif
            }
        }
        else
        {
            *x=X; *y=Y; return;
        }

    if (do_not_dim_DIM==1)
    {
        if (sel.gor)
        {
            nag=(NAGLOWEK *)adr;
            if ((nag->blok==1) && (nag->obiektt2==O2BlockDim))
            {
                *x=X; *y=Y; return;
            }
        }
    }
}

void qkoniec(double *x,double *y)
{ unsigned typ;
  void *adr;
  NAGLOWEK *nag;
  int ret;

  typ=Blinia | Bluk | Bwwielokat | Bspline | Bellipticalarc | Bsolidarc | Bvector;
  if ((adr=select_w(&typ,NULL))==NULL) { *x=X; *y=Y; return; }
   { if (typ==Blinia) koniecL_(x,y,adr);
     else if (typ==Bluk) koniecl_(x,y,adr);
     else if (typ == Bwwielokat) wielokat_(x, y, adr, koniecL_) ;
	 else if (typ == Bspline) wspline_(x, y, adr, koniecL_);
     else if (typ == Bellipticalarc) wellipticalarc_(x, y, adr, koniecL_);
     else if (typ == Bsolidarc) wsolidarc_(x, y, adr, koniecL_, koniecl_);
     else if (typ == Bvector)
       {
         ret=wvector_(x, y, adr, koniecL_, koniecl_, koniec1L_, koniec1l_);
         if (stretch_vector!=0) VectorS=adr;
         if (ret==-1)
         {
#ifdef LINUX
             *x = 0.0 / 0.0;
             *y = 0.0 / 0.0;
#else          
             *x = NAN;
             *y = NAN;
#endif
         }
       }
   }
if (do_not_dim_DIM==1)
 {
  if (sel.gor)
   {
    nag=(NAGLOWEK *)adr;
    if ((nag->blok==1) && (nag->obiektt2==O2BlockDim))
     {
       *x=X; *y=Y; return;
     }
   }
 }
}

void qsrodek(double *x,double *y)
{ unsigned typ;
  void *adr;
  NAGLOWEK *nag;
  typ=Blinia | Bluk | Bwwielokat;
  if ((adr=select_w(&typ,NULL))==NULL) { *x=X; *y=Y; return; }
   { if (typ==Blinia) srodekL_(x,y,adr);
     if (typ==Bluk) srodekl_(x,y,adr);
     if (typ == Bwwielokat) wielokat_(x, y, adr, srodekL_) ;
   }
if (do_not_dim_DIM==1)
 {
  if (sel.gor)
   {
    nag=(NAGLOWEK *)adr;
    if ((nag->blok==1) && (nag->obiektt2==O2BlockDim))
     {
       *x=X; *y=Y; return;
     }
   }
 }
}

void qprzeciecie(double *x,double *y)
{ unsigned typ;
  void *adr,*adr1;
  NAGLOWEK *nag;
  int l=0;
  typ=Blinia | Bluk | Bokrag | Bkolo | Bwwielokat | Bspline | Bellipse | Bfilledellipse | Bellipticalarc | Bprzeciecie | Bsolidarc;
  if ((adr=select_w(&typ,&adr1))==NULL) { *x=X; *y=Y; return; }
   if (adr1==NULL) return;
   switch(typ)
    { case Blinia | Blinia : l=przeciecieLL_(x,y,adr,adr1);
			      break;
      case Blinia | Bokrag : l=przeciecieLO_(x,y,adr,adr1);
			      break;
      case Bokrag | Bokrag : l=przeciecieOO_(x,y,adr,adr1);
			      break;
      case Blinia | Bluk :   l=przeciecieLl_(x,y,adr,adr1);
			      break;
      case Bluk | Bokrag :   l=przeciecielO_(x,y,adr,adr1);
			      break;
      case Bluk | Bluk :   l=przecieciell_(x,y,adr,adr1);
			      break;
      case Blinia | Bwwielokat: l=przeciecieLW_(x,y,adr,adr1);
			      break;
      case Bluk   | Bwwielokat: l=przeciecielW_(x,y,adr,adr1);
			      break;
      case Bwwielokat | Bokrag: l=przeciecieWO_(x,y,adr,adr1);
			      break;
      case Bwwielokat | Bwwielokat: l=przeciecieWW_(x,y,adr,adr1);
			      break;
      case Blinia | Bellipse:
      case Blinia | Bfilledellipse: l = przeciecieLE_(x, y, adr, adr1);
                break;
      case Blinia | Bellipticalarc: l = przeciecieLEA_(x, y, adr, adr1);
                break;
      case Bluk | Bellipse:
      case Bluk | Bfilledellipse: l = przeciecielE_(x, y, adr, adr1);
                break;
      case Bluk | Bellipticalarc: l = przeciecielEA_(x, y, adr, adr1);
                break;
      case Bokrag | Bellipse:
      case Bokrag | Bfilledellipse: l = przeciecieEO_(x, y, adr, adr1);
                break;
      case Bokrag | Bellipticalarc: l = przeciecieEAO_(x, y, adr, adr1);
                break;
      case Bwwielokat | Bellipse:
      case Bwwielokat | Bfilledellipse: l = przeciecieWE_(x, y, adr, adr1);
                break;
      case Bwwielokat | Bellipticalarc: l = przeciecieWEA_(x, y, adr, adr1);
                break;
      case Bspline | Bellipse:
      case Bspline | Bfilledellipse: l = przeciecieSE_(x, y, adr, adr1);
                break;
      case Bspline | Bellipticalarc: l = przeciecieSEA_(x, y, adr, adr1);
                break;
      case Bellipse | Bellipse:
      case Bellipse | Bfilledellipse: l = przeciecieEE_(x, y, adr, adr1);
                break;
      case Bellipse | Bellipticalarc: l = przeciecieEEA_(x, y, adr, adr1);
                break;
      case Bfilledellipse | Bfilledellipse: l = przeciecieEE_(x, y, adr, adr1);
                break;
      case Bfilledellipse | Bellipticalarc: l = przeciecieEEA_(x, y, adr, adr1);
                break;
      case Bellipticalarc | Bellipticalarc: l = przeciecieEAEA_(x, y, adr, adr1);
                break;
	  case Blinia | Bspline: l=przeciecieLS_(x, y, adr, adr1);
		        break;
	  case Bluk | Bspline: l=przeciecielS_(x, y, adr, adr1);
		        break;
	  case Bspline | Bokrag: l=przeciecieSO_(x, y, adr, adr1);
		        break;
	  case Bwwielokat | Bspline: l=przeciecieWS_(x, y, adr, adr1);
		        break;
	  case Bspline | Bspline: l=przeciecieSS_(x, y, adr, adr1);
		        break;
      case Bsolidarc   | Bwwielokat: l=przeciecieSAW_(x, y, adr,adr1);
                break;
      case Bsolidarc   | Bsolidarc: l=przeciecieSASA_(x, y, adr,adr1);
                break;
      case Bluk   | Bsolidarc: l=przeciecielSA_(x, y, adr,adr1);
                break;
      case Bsolidarc | Bokrag : l=przeciecieSAO_(x, y, adr,adr1);
                break;
      case Bsolidarc | Bellipse:
      case Bsolidarc | Bfilledellipse: l = przeciecieSAE_(x, y, adr, adr1);
                break;
      case Bsolidarc | Bellipticalarc: l = przeciecieSAEA_(x, y, adr, adr1);
                break;
      case Blinia | Bsolidarc: l=przeciecieLSA_(x, y, adr,adr1);
                break;
      case Bsolidarc | Bspline: l = przeciecieSAS_(x, y, adr, adr1);
                break;
      default    : break;
    }
if (do_not_dim_DIM==1)
 {
   if (sel.gor)
   {
    nag=(NAGLOWEK *)adr;
    if ((nag->blok==1) && (nag->obiektt2==O2BlockDim))
     {
       *x=X; *y=Y; return;
     }
    nag=(NAGLOWEK *)adr1;
    if ((nag->blok==1) && (nag->obiektt2==O2BlockDim))
     {
       *x=X; *y=Y; return;
     }
   }
 }
}

void qcentrum(double *x,double *y)
{ unsigned typ;
  void *adr;
  NAGLOWEK *nag;
  typ=Bokrag | Bkolo | Bluk | Bpolygon | Bellipse | Bfilledellipse | Bellipticalarc;
  if ((adr=select_w(&typ,NULL))==NULL) { *x=X; *y=Y; return; }
  switch(typ)
   { case Bkolo  :
     case Bokrag : centrumO_ (x, y, adr) ;
		   break;
     case Bellipse :
     case Bfilledellipse:
         centrumE_ (x, y, adr) ;
           break;
     case Bluk   : centruml_ (x, y, adr) ;
		   break;
     case Bellipticalarc   : centrumel_ (x, y, adr) ;
           break;
    case Bpolygon: centrum_poly_ (x, y, adr) ;
		   break ;
     default     : break ;
   }
   nag=(NAGLOWEK *)adr;
    if ((nag->blok==1) && (nag->obiektt2==O2BlockDim))
     {
       *x=X; *y=Y; return;
     }
}

void qnajblizszy(double *x,double *y)
{ unsigned typ;
  void *adr;
  NAGLOWEK *nag;
  BOOL ret;
  int l;
  typ=Blinia | Bluk | Bokrag | Bkolo | Bwwielokat | Bpoint | Bspline | Bellipse | Bfilledellipse | Bellipticalarc;
  if ((adr=select_w(&typ,NULL))==NULL) { *x=X; *y=Y; return; }
  switch(typ)
   { case Blinia : najblizszyL_(x, y, adr);
		   break;
     case Bkolo   :
     case Bokrag : najblizszyO_(x, y, adr);
		   break;
     case Bluk   : najblizszyl_(x, y, adr);
		   break;
       case Bellipse   :
       case Bfilledellipse   : ret=najblizszyE_(&Xsel,&Ysel,adr);
           break;
       case Bellipticalarc   : ret=najblizszyEA_(&Xsel,&Ysel,adr);
           break;
     case Bwwielokat: wielokat_(x, y, adr, najblizszyL_);
		   break;
     case Bpoint: najblizszyP_(x, y, adr);
            break;
       case Bspline: l = mwspline_(x, x, adr, najblizszyLx_);
           break;
     default     : break;
   }
   nag=(NAGLOWEK *)adr;
    if ((nag->blok==1) && (nag->obiektt2==O2BlockDim))
     {
       *x=X; *y=Y; return;
     }
}

void qprostopadle(double *x,double *y)
{ unsigned typ;
  void *adr;
  NAGLOWEK *nag;
  int l;
  typ=Blinia | Bokrag | Bkolo | Bluk | Bwwielokat | Bspline | Bellipse | Bfilledellipse | Bellipticalarc;
  if ((adr=select_w(&typ,NULL))==NULL) { *x=X; *y=Y; return; }
   switch(typ)
    { case Blinia : l=prostopadleL_(x,y,adr);
		    break;
      case Bkolo  :
      case Bokrag : l=prostopadleO_(x,y,adr);
		    break;
      case Bluk :   l=prostopadlel_(x,y,adr);
		    break;
      case Bwwielokat: wielokat_(x, y, adr, (void (*)(double*, double*, void*))prostopadleL_) ;
		    break;
        case Bspline: l = prostopadles_(x, y, adr);
            break;
        case Bellipse:
        case Bfilledellipse:
            l = prostopadleE_(x, y,adr);
            break;
        case Bellipticalarc:
            l = prostopadleEA_(x, y,adr);
            break;
      default     : break;
    }
    nag=(NAGLOWEK *)adr;
    if ((nag->blok==1) && (nag->obiektt2==O2BlockDim))
     {
       *x=X; *y=Y; return;
     }
}

void qstycznie(double *x,double *y)
{ unsigned typ;
  void *adr;
  int l;
  NAGLOWEK *nag;
  typ=Bluk | Bokrag | Bkolo | Bellipse | Bfilledellipse | Bellipticalarc;
  if ((adr=select_w(&typ,NULL))==NULL) { *x=X; *y=Y; return; }
   switch(typ)
    { case Bluk   : l=styczniel_(x,y,adr);
		    break;
      case Bkolo  :
      case Bokrag : l=stycznieO_(x,y,adr);
		    break;
        case Bellipse :
        case Bfilledellipse :
            l = stycznieE_(x,y,adr);
            break;
        case Bellipticalarc :
            l = stycznieEA_(x,y,adr);
            break;
      default     : break;
    }
    nag=(NAGLOWEK *)adr;
    if ((nag->blok==1) && (nag->obiektt2==O2BlockDim))
     {
       *x=X; *y=Y; return;
     }
}

void qpoint (double *x, double *y)
/*------------------------------*/
{
  unsigned typ ;
  void *adr ;
  T_Point *ptrs_point ;

  typ = Bpoint ;
  if ((adr = select_w (&typ, NULL)) == NULL)
  {
    *x = X ;
    *y = Y ;
    return ;
  }
  ptrs_point = (T_Point*)adr ;
  *x = ptrs_point->x ;
  *y = ptrs_point->y ;
}

void Get_Sel_XY (double *x, double *y)
/*----------------------------------*/
{
  *x = Xsel ;
  *y = Ysel ;
  if (sel.jump == 1)
  {
    *x += df__dxjump ;
    *y += df__dyjump ;
  }
}

