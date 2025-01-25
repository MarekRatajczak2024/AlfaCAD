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

#pragma warning(disable : 4996)
#include<forwin.h>
#ifndef LINUX
#include<dos.h>
#endif
#define ALLEGWIN
#include <allegext.h>
#include<math.h>
#include<setjmp.h>
#include<string.h>
#include "bib_e.h"
#include "alffont.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "b_libfun.h"
#include "bib_blok.h"
#include "o_object.h"

#define OZero 1.0E-7
#define OCon  1.0E+7

#define n_typ_normal 0
#define n_etykieta_wew 1
#define n_etykieta_sklad 2
#define n_typ_symbol 3
#define n_typ_typ 4
#define n_typ_pin_name 5
#define n_typ_zasilanie 6
#define n_typ_port 7
#define n_typ_atrybut 8
#define n_typ_schemat 9
#define n_typ_plik 10
#define n_typ_siec 11
#define n_typ_komentarz 12
#define n_typ_zmienna 13
#define n_typ_symbol_drabinki 14
#define n_typ_opis_drabinki 15

#define j_do_lewej 0
#define j_do_prawej 1
#define j_srodkowo 2
#define j_centralnie 3
#define j_nazwa_zacisku 4

#define MY_DOT_FILL  HATCH_FILL //LTSLASH_FILL //HATCH_FILL  //WIDE_DOT_FILL
#define MY_DOT_FILL1 WIDE_DOT_FILL

#define cursor_off(X,Y) out_cur_off(X,Y)
#define cursor_on(X,Y) out_cur_on(X,Y)


int msx, msy;
extern double Get_TextLen (TEXT *ptrs_text, char *ptrsz_t) ;
extern double Get_Text_Len (TEXT *ptrs_text, char *ptrsz_t, double font_scale, int font_index, int *t_width, int *t_height, double *matrix_d, int *i_matrix_d);
extern BOOL Buff_Dan_Realloc (void) ;;
extern void Draw_Kolo (T_PixelTVal xr, T_PixelTVal yr, T_PixelTVal R) ;
extern OKNO* Get_Stretch_Window (void) ;
extern BOOL Get_Stretch_Mode (void) ;
extern void Draw_Point (T_Point *ad, int mode, int kolor) ;
extern void Draw_Vector (AVECTOR *ad, int mode, int kolor, int redraw_obj) ;
extern void mvcurbpT (unsigned) ;
extern void setcolor(int kolor);
extern void setlinestyle2(int line_style, unsigned short u_pattern, int thickness);
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);
extern void setwritemode( int mode );
extern void setfillstyle_(int pattern, int color);
extern void set_mode_solid();
extern void set_mode_trans();
extern void setlinestyle3(int line_style, unsigned short u_pattern, int thickness);
extern int linestyle_solid(int typ);
extern void ClearWindow(void);
extern int Solid04(char *adr0, char *adr1, int kolor);
extern char readmouse(void);
extern char readmouse_move(void);
extern int linestyle_xor(int typ);
extern int Point_Selected (T_Point *ptrs_point);
extern int Vector_Selected (AVECTOR *ptrs_vector);
extern BOOL get_pattern_count(void);
extern void set_pattern_count(BOOL p_count);
extern void set_pattern_offset(int offs);
extern int set_sketch_offset(int sk_offs);

static void *trans_obiect ;
extern BOOL TTF_redraw;

extern BOOL global_no_pcx;
extern void     drawpoly(int numpoints, int *polypoints);
extern void calc_bspline(int lp, int n, float points[], int p1, float *out_x, float *out_y);
extern BOOL Get_Point_View(void);
extern BOOL Get_Control_Points_View(void);
extern BOOL Get_Curviness_Only(void);
extern B_PCX* GlobalPCX;
extern int TRANSLUCENCY;
extern int HALFTRANSLUCENCY;
extern POINTF map_point(float Px, float Py);
extern int quadtoquadmatrix(QUAD one, QUAD two);
extern BOOL get_dragging_quad(void);
extern BOOL Check_ifConvex(QUAD *quad);
extern void outlinetext(TEXT *t, QUAD *outline, double margin);
extern BOOL dragging_quad_move;
extern void reset_dane0(void);
extern void Get_EllipticalArc_EndPoints (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double df_sangle0, double df_eangle0, double *x1, double *y1, double *x2, double *y2);
extern BOOL Ellipse_Of_Centre_And_Two_Points(double h, double k, double p, double q, double m, double n, double *a, double *b);
extern void Tangential_Point_To_Ellipse(double cx, double cy, double rx, double ry, double xx1, double yy1, double xx2, double yy2, double *x, double *y);

extern char *Get_PTR__GTMPFIRST(void);
extern void Set_PTR__GTMPFIRST(char *first);
extern char *Get_PTR__GTMPLAST(void);
extern void Set_PTR__GTMPLAST(char *last);

extern BOOL trans_scale_arcs_ (char *ADP_b, char *ADK_b, double df_basex, double df_basey, double df_sx, double df_sy, BOOL enforce);
extern int get_3p_arc(LUK *l, POINTD *p1, POINTD *p2, POINTD *p3);
extern int get_arc_points(LUK *l, double **xy, int n0, BOOL reversed, BOOL printer);
extern void my_fillpoly(int numpoints, int* polypoints, int translucency, int kolory_paper);
extern void fillpoly(int numpoints, AL_CONST int* polypoints);

extern int solidarc_wybrany(SOLIDARC *sa);
extern BOOL Point_in_SolidArc (SOLIDARC *ptr_sa, double x, double y);
extern void reset_trans_blender(void);
extern long pXp,pYp,pXk,pYk;

extern void fillpolypattern(int numpoints, AL_CONST int *polypoints, char *s_pattern, long origin_x, long origin_y);
extern void Get_oknoS(RECTD *rectd);
extern int solidarc_elements(SOLIDARC *sa, LINIA *L_left, LINIA *L_right, LUK *l_inner1, LUK *l_inner2, LUK *l_outer1, LUK *l_outer2);

extern BITMAP *Get_Screenplay(void);
extern void Set_Screenplay(BITMAP *ctx_bitmap);

extern int get_stretch_vector(void);
extern void *get_vector_c(void);

NODE bnodes[MAX_NODES];

double curviness = 0.75;   //0.0 to 1.0  //parameter will be recorded at the end of xy points coordinates, default is 0.25

int show_control_points = 0;

static long_long pattern_add_limit = 0;

#define LAST_POLYGONS_NO 2

static p_vector first_polygon_vector = { 0 };
static p_vector last_polygon_vector = { 0 };
static T_PixelTVal last_polygon[LAST_POLYGONS_NO][8] = { 0 };

static void transformacja_obiekt(void *ad,double x,double y,double k1,double k2,
	     double xa, double ya, double ka1, double ka2, int trans, double z) ;

static void mirrorP (double apx1, double apy1, double apx2, double apy2, double x, double y, double *xret, double *yret);

QUAD quad0, quad1;

BOOL enforce_vector_scale=FALSE;

char last_buf[sizeof(LINIA)+2];

typedef struct
{
    char *ad;
    POINTF p[2];
} AD_BUF;
/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/

void set_pattern_add_limit(long_long n)
{
	pattern_add_limit = n;
}

static void zwpcx (char  *adr, double dx, double dy)
/*------------------------------------------------------------*/
{
  B_PCX *pcx ;

  pcx = (B_PCX*)adr ;
  pcx->x += dx ;
  pcx->y += dy ;
  pcx->widoczny=1;
  if ((!pcx->on_front) && ((Check_if_Equal(dx,0.0)==FALSE) || (Check_if_Equal(dy,0.0)==FALSE)))
           regen_ctx=TRUE;
}

static void zwlinia(char  *adr,double dx,double dy)
{ LINIA *l;
  l=(LINIA*)adr;
  switch(l->obiektt1)
   { case Guma    :
     case Sztywny :
	  l->x1+=dx;
	  l->x2+=dx;
	  l->y1+=dy;
	  l->y2+=dy;
	  break;
     case Utwierdzony1 :
	  l->x2+=dx;
	  l->y2+=dy;
	  break;
     case Utwierdzony2 :
	  l->x1+=dx;
	  l->y1+=dy;
	  break;
   }
  if(linia_wybrana(l)) l->widoczny=1;
  else l->widoczny=0;
}

static void zwvector(char  *adr,double dx,double dy)
{   AVECTOR *v;
    AVECTOR *vc;
    v=(AVECTOR*)adr;
    switch (get_stretch_vector())
        case 0: {
            switch (v->obiektt1) {
                case Guma    :
                case Sztywny :
                    v->x1 += dx;
                    v->x2 += dx;
                    v->y1 += dy;
                    v->y2 += dy;
                    break;
                case Utwierdzony1 :
                    v->x2 += dx;
                    v->y2 += dy;
                    break;
                case Utwierdzony2 :
                    v->x1 += dx;
                    v->y1 += dy;
                    break;
            }
            break;
            case 1: //stretching line and value of force or displacement
            vc=get_vector_c();
            memmove(v, vc, sizeof(AVECTOR));
            break;
            case 2: //stretching radius of moment or rotation
            vc=get_vector_c();
            memmove(v, vc, sizeof(AVECTOR));
            break;
            case 3: //stretching angle and value of moment or rotation
            vc=get_vector_c();
            memmove(v, vc, sizeof(AVECTOR));
            break;
            case 4: //stretching angle and value of moment or rotation
            vc=get_vector_c();
            memmove(v, vc, sizeof(AVECTOR));
            break;
            case 5: //stretching angle and value of moment or rotation
            vc=get_vector_c();
            memmove(v, vc, sizeof(AVECTOR));
            break;
            case 6: //stretching thermal bar size
            case 7:
            vc=get_vector_c();
            memmove(v, vc, sizeof(AVECTOR));
            break;
            case 8: //stretching node size
            vc=get_vector_c();
            memmove(v, vc, sizeof(AVECTOR));
            break;
            default:
                break;
        }
    if(vector_wybrany(v)) v->widoczny=1;
    else v->widoczny=0;
}

static void zwlinia3D(char  *adr,double dx,double dy)
{ LINIA3D *l;
  l=(LINIA3D*)adr;
  switch(l->obiektt1)
   { case Guma    :
     case Sztywny :
	  l->x1+=dx;
	  l->x2+=dx;
	  l->y1+=dy;
	  l->y2+=dy;
	  break;
     case Utwierdzony1 :
	  l->x2+=dx;
	  l->y2+=dy;
	  break;
     case Utwierdzony2 :
	  l->x1+=dx;
	  l->y1+=dy;
	  break;
   }
  if(linia_wybrana((LINIA *)l)) l->widoczny=1;
  else l->widoczny=0;
}

static void zwlinia3DZ(char  *adr,double dx,double dy)
{ LINIA3D *l;
  double dz;
  l=(LINIA3D*)adr;
  dz=sqrt(dx*dx + dy*dy);
  if ((dx<0) || (dy<0)) dz*=(-1);

  switch(l->obiektt1)
   { case Guma    :
     case Sztywny :
	  l->z1+=dz;
	  l->z2+=dz;
	  break;
     case Utwierdzony1 :
	  l->z2+=dz;
	  break;
     case Utwierdzony2 :
	  l->z1+=dz;
	  break;
   }
  if(linia_wybrana((LINIA *)l)) l->widoczny=1;
  else l->widoczny=0;
}

static void zwblock (char  *adr, double dx, double dy)
/*------------------------------------------------------------*/
{
  BLOK 			*ptrs_block ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;

  ptrs_block = (BLOK*)adr ;
  ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
  if (((ptrs_block->kod_obiektu == B_EXPORT) ||
	   (ptrs_block->kod_obiektu == B_HATCH) ||
       (ptrs_block->kod_obiektu == B_PPOMIAR) ||
       (ptrs_block->kod_obiektu == B_KPOMIAR) ||
       (ptrs_block->kod_obiektu == B_ODWIERT_OLD) ||
       (ptrs_block->kod_obiektu == B_ODWIERT) ||
       (ptrs_block->kod_obiektu == B_INSTALACJE) ||
       (ptrs_block->kod_obiektu == B_HEKTOMETRY) ||
       (ptrs_block->kod_obiektu == B_SEKTORY) ||
       (ptrs_block->kod_obiektu == B_WIRE) ||
       (ptrs_block->kod_obiektu == B_ARM) ||
       (ptrs_block->kod_obiektu == B_SIEC) ||
       (ptrs_block->kod_obiektu == B_VERTEX)
      )
     && ptrs_desc_bl->flags == EBF_IP)
  {
    ptrs_desc_bl->x += dx ;
    ptrs_desc_bl->y += dy ;
  }
}

static void zwpoint (char  *adr, double dx, double dy)
/*------------------------------------------------------------*/
{
  T_Point *ptrs_point ;

  ptrs_point = (T_Point*)adr ;
  ptrs_point->x += dx ;
  ptrs_point->y += dy ;
  ptrs_point->widoczny=Point_Selected(ptrs_point);
}


static void zwtekst(char  *adr,double dx,double dy)
/*--------------------------------------------------------*/
{
  TEXT *t;
  t=(TEXT*)adr;
  t->x+=dx;
  t->y+=dy;
  t->widoczny=tekst_wybrany(t);
}

static void zwspline0(char  *adr, double dx, double dy)
/*--------------------------------------------------------*/
{
	int i;
	SPLINE *s;
	s = (SPLINE*)adr;
	for (i = 0; i < (int)s->lp; i+=2)
	{
		s->xy[i] += dx;
		s->xy[i+1] += dy;
	}
	s->widoczny = spline_wybrany(s);
}

static void zwluk(char  *adr,double dx,double dy)
{
  LUK *l;
  double xs,ys,xe,ye,lc_2,h,x0,y0,xr,yr, dl,xs0,ys0,si,co;
  double kat1, kat2, ws;

  l=(LUK*)adr;
  kat1 = Angle_Normal (l->kat1);
  kat2 = Angle_Normal (l->kat2);
  if ( kat2 < kat1 )
  {
    kat2 += Pi2;
  }
  ws = -1;
  if (kat2 - kat1 > M_PI)
  {
    ws = 1;
  }
  switch(l->obiektt1)
   { case Guma    :
     case Sztywny :
	  l->x+=dx;
	  l->y+=dy;
	  break;
     case Utwierdzony1 :
     case Utwierdzony2 :
	  xs=l->x+l->r*cos(l->kat1);
	  ys=l->y+l->r*sin(l->kat1);
	  xe=l->x+l->r*cos(l->kat2);
	  ye=l->y+l->r*sin(l->kat2);
	  lc_2 = ((xe-xs)*(xe-xs)+(ye-ys)*(ye-ys)) / 4 ;
	  h=l->r + ws * sqrt(fabs(l->r*l->r-lc_2));
	  if(h<OZero) return;
	  if(l->obiektt1==Utwierdzony1) { xe+=dx;ye+=dy; }
	  else { xs+=dx;ys+=dy; }
	  lc_2 =.25*((xe-xs)*(xe-xs)+(ye-ys)*(ye-ys));
	  l->r = (lc_2 + h * h) / (2 * h);
	  x0=(xe+xs)/2;
	  y0=(ye+ys)/2;
	  xs0 = xs - x0 ;
	  ys0 = ys - y0 ;
	  dl = sqrt (lc_2) ;
	  si = ys0 / dl ;
	  co = xs0 / dl ;
	  obru(si,co,xs0,ys0,&xs0,&ys0);
	  xr=0;
	  yr=sqrt (fabs (l->r*l->r-xs0*xs0));
	  ws = (h < l->r) ?  -1 : 1;
	  yr= ws * yr;
	  obrd(si,co,xr,yr,&xr,&yr);
	  l->x=xr+x0; l->y=yr+y0;
	  xs=xs-l->x;  xe=xe-l->x;
	  ys=ys-l->y;  ye=ye-l->y;
	  l->kat1=Atan2(ys,xs);
	  l->kat2=Atan2(ye,xe);
	  break;
   }
  l->widoczny=luk_wybrany(l);
}

static void zwsolidarc(char  *adr,double dx,double dy)
{
    SOLIDARC *sa;
    double xs,ys,xe,ye,lc_2,h,x0,y0,xr,yr, dl,xs0,ys0,si,co;
    double kat1, kat2, ws;

    sa=(SOLIDARC*)adr;
    kat1 = Angle_Normal (sa->kat1);
    kat2 = Angle_Normal (sa->kat2);
    if ( kat2 < kat1 )
    {
        kat2 += Pi2;
    }
    ws = -1;
    if (kat2 - kat1 > M_PI)
    {
        ws = 1;
    }
    switch(sa->obiektt1)
    { case Guma    :
        case Sztywny :
            sa->x+=dx;
            sa->y+=dy;
            break;
        case Utwierdzony1 :
        case Utwierdzony2 :
            xs=sa->x+sa->r*cos(sa->kat1);
            ys=sa->y+sa->r*sin(sa->kat1);
            xe=sa->x+sa->r*cos(sa->kat2);
            ye=sa->y+sa->r*sin(sa->kat2);
            lc_2 = ((xe-xs)*(xe-xs)+(ye-ys)*(ye-ys)) / 4 ;
            h=sa->r + ws * sqrt(fabs(sa->r*sa->r-lc_2));
            if(h<OZero) return;
            if(sa->obiektt1==Utwierdzony1) { xe+=dx;ye+=dy; }
            else { xs+=dx;ys+=dy; }
            lc_2 =.25*((xe-xs)*(xe-xs)+(ye-ys)*(ye-ys));
            sa->r = (lc_2 + h * h) / (2 * h);
            x0=(xe+xs)/2;
            y0=(ye+ys)/2;
            xs0 = xs - x0 ;
            ys0 = ys - y0 ;
            dl = sqrt (lc_2) ;
            si = ys0 / dl ;
            co = xs0 / dl ;
            obru(si,co,xs0,ys0,&xs0,&ys0);
            xr=0;
            yr=sqrt (fabs (sa->r*sa->r-xs0*xs0));
            ws = (h < sa->r) ?  -1 : 1;
            yr= ws * yr;
            obrd(si,co,xr,yr,&xr,&yr);
            sa->x=xr+x0; sa->y=yr+y0;
            xs=xs-sa->x;  xe=xe-sa->x;
            ys=ys-sa->y;  ye=ye-sa->y;
            sa->kat1=Atan2(ys,xs);
            sa->kat2=Atan2(ye,xe);
            break;
    }
    sa->widoczny=solidarc_wybrany(sa);
}

static void zwokrag(char  *adr,double dx,double dy)
{ OKRAG *o;
  o=(OKRAG*)adr;
  o->x+=dx;
  o->y+=dy;
  o->widoczny=okrag_wybrany(o);
}
static void zwkolo(char  *adr,double dx,double dy)
{ OKRAG *k;
  k=(OKRAG*)adr;
  k->x+=dx;
  k->y+=dy;
  k->widoczny=okrag_wybrany(k);
}

static void zwellipticalarc(char  *adr,double dx,double dy)
{ ELLIPTICALARC *ea;
    double xs,ys,xe,ye,lc_2,h,x0,y0,xr,yr, dl,xs0,ys0,si,co;
    double kat1, kat2, ws;
    double xxs, yys, xxe, yye;
    static double a,b;
    double ddx, ddy;
    double xt, yt;
    double eax, eay;
    double delx, dely, delxx, delyy;
    BOOL ret;
    double lambda;
    double dl1, dl2;
    double vx1, vy1, vx2, vy2;
    double dot,det;
    double angle;

    ea=(ELLIPTICALARC*)adr;

    kat1 = Angle_Normal (ea->kat1);
    kat2 = Angle_Normal (ea->kat2);
    if ( kat2 < kat1 )
    {
        kat2 += Pi2;
    }
    ws = -1;
    if (kat2 - kat1 > M_PI)
    {
        ws = 1;
    }
    switch(ea->obiektt1)
    { case Guma    :
        case Sztywny :
            ea->x+=dx;
            ea->y+=dy;
            break;
        case Utwierdzony1 :
        case Utwierdzony2 :

            Get_EllipticalArc_EndPoints (ea->x, ea->y, ea->rx, ea->ry, ea->angle, ea->kat1, ea->kat2, &xs, &ys, &xe, &ye);

            if(ea->obiektt1==Utwierdzony1) {

                //rotation of ellipse
                vx1 = xe - xs;
                vy1 = ye - ys;
                vx2 = xe + dx - xs;
                vy2 = ye + dy - ys;
                dot = vx1 * vx2 + vy1 * vy2;  // Dot product between [vx1, vy1] and [vx2, vy2]
                det = vx1 * vy2 - vy1 * vx2;  // Determinant
                kat1 = -atan2(det, dot);  // atan2(y, x) or atan2(sin, cos)
                kat1 = fmod(kat1, Pi2);
                if (kat1 < 0) kat1 += Pi2;

                si = sin(kat1);
                co = cos(kat1);

                angle=ea->angle+kat1;
                angle= Angle_Normal(angle);

                //moving centre and expanding a
                dl1=sqrt((vx1*vx1)+(vy1*vy1));
                dl2=sqrt((vx2*vx2)+(vy2*vy2));
                lambda=dl2/dl1;

                double lambda1=(dl2-dl1)/dl1;
                double ddxx=(xe-ea->x)*lambda1;
                double ddyy=-(ye- ea->y)*lambda1;

                Rotate_Point(si, co, xs, ys, ea->x, ea->y,  &eax, &eay);

                ea->x=eax + (ddxx)*co;
                ea->y=eay + (ddxx)*si;

                ea->rx*=lambda;
                ea->angle=angle;

                xe+=dx;
                ye+=dy;

            }
            else
            {
                //rotation of ellipse
                vx1 = xs - xe;
                vy1 = ys - ye;
                vx2 = (xs + dx) - xe;
                vy2 = (ys + dy) - ye;
                dot = vx1 * vx2 + vy1 * vy2;  // Dot product between [vx1, vy1] and [vx2, vy2]
                det = vx1 * vy2 - vy1 * vx2;  // Determinant
                kat1 = atan2(det, dot);  // atan2(y, x) or atan2(sin, cos)
                kat1 = fmod(kat1, Pi2);
                if (kat1 < 0) kat1 += Pi2;

                si = sin(kat1);
                co = cos(kat1);

                angle=ea->angle+kat1;
                angle= Angle_Normal(angle);

                //moving centre and expanding a
                dl1=sqrt((vx1*vx1)+(vy1*vy1));
                dl2=sqrt((vx2*vx2)+(vy2*vy2));
                lambda=dl2/dl1;

                double lambda1=(dl2-dl1)/dl1;
                double ddxx=(ea->x-xe)*lambda1;
                double ddyy=-(ea->y-ye)*lambda1;


                Rotate_Point(si, co, xe, ye, ea->x, ea->y,  &eax, &eay);

                ea->x=eax + (ddxx)*co;
                ea->y=eay + (ddxx)*si;


                ea->rx*=lambda;
                ea->angle=angle;

                xs+=dx;
                ys+=dy;

            }

            ea->kat1=Atan2(ys-(ea->y), xs-(ea->x)) - angle;
            ea->kat2=Atan2(ye-(ea->y), xe-(ea->x)) - angle;

            ea->kat1= Angle_Normal(ea->kat1);
            ea->kat2= Angle_Normal(ea->kat2);

            xxs=xs;
            yys=ys;
            xxe=xe;
            yye=ye;

            si = sin(-ea->angle);
            co = cos(-ea->angle);

            Rotate_Point(si, co, ea->x, ea->y, xxs, yys, &xxs, &yys);
            Rotate_Point(si, co, ea->x, ea->y, xxe, yye, &xxe, &yye);

            a=ea->rx;
            b=ea->ry;

            ret = Ellipse_Of_Centre_And_Two_Points(ea->x, ea->y, xxs, yys, xxe, yye, &a, &b);

            if (!ret)  return;

            ea->rx = a;
            ea->ry = b;


            break;
        case Utwierdzony1*100 :  //TEMPORARY
        case Utwierdzony2*100 :

            Get_EllipticalArc_EndPoints (ea->x, ea->y, ea->rx, ea->ry, ea->angle, ea->kat1, ea->kat2, &xs, &ys, &xe, &ye);

            xxs=xs;
            yys=ys;
            xxe=xe;
            yye=ye;

            si = sin(-ea->angle);
            co = cos(-ea->angle);

            Rotate_Point(si, co, ea->x, ea->y, xxs, yys, &xxs, &yys);
            Rotate_Point(si, co, ea->x, ea->y, xxe, yye, &xxe, &yye);

            Rotate_Point(si, co, 0, 0, dx, dy, &ddx, &ddy);


            if(ea->obiektt1==Utwierdzony1)
            {
                if (Check_if_Equal3(xxs,xxe)) delx=0;
                else delx=ddx/(xxs-xxe);
                if (Check_if_Equal3(yys,yye)) dely=0;
                else dely=ddy/(yys-yye);
                ////lambda=sqrt(((ea->x-xxs)*(ea->x-xxs)+(ea->y-yys)*(ea->y-yys))/((xxe-xxs)*(xxe-xxs)+(yye-yys)*(yye-yys)));
                lambda=0.5;
                eax=ea->x+(ddx*lambda);
                eay=ea->y+(ddy*lambda);

                xxe+=ddx;yye+=ddy;

            }
            else
            {
                if (Check_if_Equal3(xxs,xxe)) delx=0;
                else delx=ddx/(xxe-xxs);
                if (Check_if_Equal3(yys,yye)) dely=0;
                else dely=ddy/(yye-yys);
                ////lambda=sqrt(((ea->x-xxe)*(ea->x-xxe)+(ea->y-yye)*(ea->y-yye))/((xxe-xxs)*(xxe-xxs)+(yye-yys)*(yye-yys)));
                lambda=0.5;
                eax=ea->x+(ddx*lambda);
                eay=ea->y+(ddy*lambda);

                xxs+=ddx;yys+=ddy;

            }

            ret = Ellipse_Of_Centre_And_Two_Points(eax, eay, xxs, yys, xxe, yye, &a, &b);
            if (!ret)  return;

            //restoring angles
            ea->kat1=Atan2(yys-(eay), xxs-(eax));
            ea->kat2=Atan2(yye-(eay), xxe-(eax));

            ea->kat1= Angle_Normal(ea->kat1);
            ea->kat2= Angle_Normal(ea->kat2);

            si = sin(ea->angle);
            co = cos(ea->angle);

            Rotate_Point(si, co, 0, 0, delxx, delyy, &delx, &dely);

            ea->x+=dx*lambda;
            ea->y+=dy*lambda;

            ea->rx=a;
            ea->ry=b;

            break;
    }
    ea->widoczny=elipsa_wybrana_prec((ELLIPSE*)ea);
}

static void zwellipse(char  *adr,double dx,double dy)
{ ELLIPSE *e;
    e=(ELLIPSE*)adr;
    e->x+=dx;
    e->y+=dy;
    e->widoczny=elipsa_wybrana_prec(e);
}

static void zwfilledellipse(char  *adr,double dx,double dy)
{ ELLIPSE *fe;
    fe=(ELLIPSE*)adr;
    fe->x+=dx;
    fe->y+=dy;
    fe->widoczny=elipsa_wybrana_prec(fe);
}

static void zwspline(char  *adr, double dx, double dy)
{
	SPLINE *s;
	int i;
	BOOL b_stretch;
	OKNO * ptr_win;

	s = (SPLINE*)adr;
	b_stretch = FALSE;
	if (Get_Stretch_Mode() == TRUE && s->temp1 == OT3W_Drag)
	{
		b_stretch = TRUE;
		ptr_win = Get_Stretch_Window();
	}
	for (i = 0; i < (int)s->lp; i += 2)
	{
		if (b_stretch == FALSE ||
			(ptr_win->x01 <= s->xy[i] &&
				s->xy[i] <= ptr_win->x02 &&
				ptr_win->y01 <= s->xy[i + 1] &&
				s->xy[i + 1] <= ptr_win->y02))
		{
			s->xy[i] += dx;
			s->xy[i + 1] += dy;
		}
	}
	if (b_stretch == TRUE)
	{
		/*    Normalize_Solid (w) ;*/
		/*      Normalize_Object ((NAGLOWEK *)w) ;*/
	}

	s->widoczny = spline_wybrany(s);
}

static void zwwwielokat(char  *adr,double dx,double dy)
{
  WIELOKAT *w ;
  int i ;
  BOOL b_stretch ;
  OKNO * ptr_win ;


  w=(WIELOKAT*)adr;
  b_stretch = FALSE ;
  if (Get_Stretch_Mode () == TRUE && w->temp1 == OT3W_Drag)
  {
    b_stretch = TRUE ;
    ptr_win = Get_Stretch_Window () ;
  }
  for (i = 0 ; i < (int)w->lp ; i += 2)
  {
    if (b_stretch == FALSE ||
       (ptr_win->x01 <= w->xy[i] &&
	w->xy[i] <= ptr_win->x02 &&
	ptr_win->y01 <= w->xy[i + 1] &&
	w->xy[i + 1] <= ptr_win->y02) )
    {
       w->xy[i] += dx ;
       w->xy[i + 1] += dy ;
    }
  }
  if (b_stretch == TRUE)
  {
/*    Normalize_Solid (w) ;*/
/*      Normalize_Object ((NAGLOWEK *)w) ;*/
  }

  w->widoczny=wielokat_wybrany(w);
}

static void zwwwielokat3DZ(char  *adr,double dx,double dy)
{
  WIELOKAT *w ;
  int i,j ;
  BOOL b_stretch ;
  OKNO * ptr_win ;
  double dz;

  dz=sqrt(dx*dx + dy*dy);
  if ((dx<0) || (dy<0)) 
	  dz*=(-1);

  w=(WIELOKAT*)adr;
  b_stretch = FALSE ;
  if (Get_Stretch_Mode () == TRUE && w->temp1 == OT3W_Drag)
  {
    b_stretch = TRUE ;
    ptr_win = Get_Stretch_Window () ;
  }
  for (i=0, j=0 ; i < (int)w->lp ; i+=2, j++)
  {
    if (b_stretch == FALSE ||
       (ptr_win->x01 <= w->xy[i] &&
	w->xy[i] <= ptr_win->x02 &&
	ptr_win->y01 <= w->xy[i + 1] &&
	w->xy[i + 1] <= ptr_win->y02) )
    {
      w->xy[w->lp + j] += dz ;
    }
  }
  if (b_stretch == TRUE)
  {
/*    Normalize_Solid (w) ;*/
/*      Normalize_Object ((NAGLOWEK *)w) ;*/
  }

  w->widoczny=wielokat_wybrany(w);
}


/*-----------------------------------------------------------*/
void Scale_Point (double k1, double k2, double x1,double y1,double x2,double y2,double *x,double *y)
{ *x=x1*(1-k1)+x2*k1;
  *y=y1*(1-k2)+y2*k2;
}

void Scale_Point3D (double k1, double k2, double k3, double x1,double y1,double z1,double x2,double y2,double z2,double *x,double *y, double *z)
{ *x=x1*(1-k1)+x2*k1;
  *y=y1*(1-k2)+y2*k2;
  *z=z1*(1-k3)+z2*k3;
}


static void sklinia(char  *adr,double x,double y,double k1, double k2)
{ LINIA *l;
  double xp, yp, punkt_sx, punkt_sy, del_x1, del_y1, del_x2, del_y2;
  l=(LINIA*)adr;
  
  if ((l->obiektt2==O2BlockDim) && (l->typ == 64) && (options1.scale_DIM == 0))
  {
   punkt_sx=(l->x1 + l->x2)/2;
   punkt_sy=(l->y1 + l->y2)/2;
   del_x1=l->x1-punkt_sx;
   del_y1=l->y1-punkt_sy;
   del_x2=l->x2-punkt_sx;
   del_y2=l->y2-punkt_sy;
   
   Scale_Point (k1, k2 ,x,y,punkt_sx,punkt_sy,&xp,&yp);
   l->x1=xp+del_x1;
   l->y1=yp+del_y1;
   l->x2=xp+del_x2;
   l->y2=yp+del_y2;
  }
  else
  {
   Scale_Point (k1, k2 ,x,y,l->x1,l->y1,&xp,&yp);
   l->x1=xp;l->y1=yp;
   Scale_Point (k1, k2 ,x,y,l->x2,l->y2,&xp,&yp);
   l->x2=xp;l->y2=yp;
  }
  
  if(linia_wybrana(l)) l->widoczny=1;
  else l->widoczny=0;
}

static void skvector(char  *adr,double x,double y,double k1, double k2)
{ AVECTOR *v;
    double xp, yp, del_x, del_y;
    v=(AVECTOR*)adr;

    switch (v->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            Scale_Point (k1, k2 ,x,y,v->x1,v->y1,&xp,&yp);
            v->x1=xp;v->y1=yp;
            Scale_Point (k1, k2 ,x,y,v->x2,v->y2,&xp,&yp);
            v->x2=xp;v->y2=yp;
            break;
        case 4:  //force
        case 7:  //displacement
            del_x=(v->x2 - v->x1);
            del_y=(v->y2 - v->y1);
            Scale_Point (k1, k2 ,x,y,v->x1,v->y1,&xp,&yp);
            v->x1=xp; v->y1=yp;
            v->x2=v->x1+del_x; v->y2=v->y1+del_y;
            break;
        case 5:  //arcs
        case 6:
        case 8:
        case 9:
            Scale_Arc_Vector(v, x, y, k1, k2);
            break;
        case 16:
            //Scale_Circle_Vector(v, x, y, k1, k2);
            Scale_Point (k1, k2 ,x,y,v->x1,v->y1,&xp,&yp) ;
            v->x1 = xp; v->y1 = yp;
            Scale_Point (k1, k2 ,x,y,v->x2,v->y2,&xp,&yp);
            v->x2=xp;v->y2=yp;
            v->r*=k1;  //k2 is ignored
            v->magnitude1=milimetryob(v->r);
            break;
    }

    if(vector_wybrany(v)) v->widoczny=1;
    else v->widoczny=0;
}

static void sklinia3D(char  *adr,double x,double y,double k1, double k2)
{ LINIA3D *l;
double xp, yp, zp; // *punkt_sx, punkt_sy, del_x1, del_y1, del_x2, del_y2;
  double z, k3;
  l=(LINIA3D*)adr;

   k3=k1;
   z=0;
   Scale_Point3D (k1, k2, k3 ,x,y,z,l->x1,l->y1,l->z1,&xp,&yp,&zp);
   l->x1=xp;l->y1=yp;l->z1=zp;
   Scale_Point3D (k1, k2, k3 ,x,y,z,l->x2,l->y2,l->z2,&xp,&yp,&zp);
   l->x2=xp;l->y2=yp;l->z2=zp;
  
  if(linia_wybrana((LINIA *)l)) l->widoczny=1;
  else l->widoczny=0;
}

static void skblock (char  *adr,double x,double y,double k1, double k2)
/*------------------------------------------------------------*/
{
  BLOK 			*ptrs_block ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  double 		xp, yp ;

  ptrs_block = (BLOK*)adr ;
  ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
  if (((ptrs_block->kod_obiektu == B_EXPORT) ||
	   (ptrs_block->kod_obiektu == B_HATCH) ||
       (ptrs_block->kod_obiektu == B_PPOMIAR) ||
       (ptrs_block->kod_obiektu == B_KPOMIAR) ||
       (ptrs_block->kod_obiektu == B_ODWIERT_OLD) ||
       (ptrs_block->kod_obiektu == B_ODWIERT) ||
       (ptrs_block->kod_obiektu == B_INSTALACJE) ||
       (ptrs_block->kod_obiektu == B_HEKTOMETRY) ||
       (ptrs_block->kod_obiektu == B_SEKTORY) ||
       (ptrs_block->kod_obiektu == B_WIRE) ||
       (ptrs_block->kod_obiektu == B_ARM) ||
       (ptrs_block->kod_obiektu == B_SIEC) ||
       (ptrs_block->kod_obiektu == B_VERTEX)
      )
      && ptrs_desc_bl->flags == EBF_IP)
  {
    Scale_Point (k1, k2 ,x, y, ptrs_desc_bl->x, ptrs_desc_bl->y, &xp, &yp) ;
    ptrs_desc_bl->x = xp ;
    ptrs_desc_bl->y = yp ;
  }
}

static void skpoint (char  *adr,double x,double y,double k1, double k2)
/*-----------------------------------------------------------------------------*/
{
  T_Point *ptrs_point ;
  double xp, yp ;

  ptrs_point = (T_Point*)adr ;
  Scale_Point (k1, k2 ,x, y, ptrs_point->x, ptrs_point->y, &xp, &yp) ;
  ptrs_point->x = xp ;
  ptrs_point->y = yp ;
}

static void skpcx (char  *adr,double x,double y,double k1, double k2)
/*-----------------------------------------------------------------------------*/
{
  B_PCX *pcx ;
  double xp, yp ;

  pcx = (B_PCX*)adr ;
  Scale_Point (k1, k2 ,x, y, pcx->x, pcx->y, &xp, &yp) ;
  pcx->x = xp ;
  pcx->y = yp ;
  pcx->dx *= k1 ;
  pcx->dy *= k2 ;
  if ((!pcx->on_front) && ((Check_if_Equal(k1,1.0)==FALSE) || (Check_if_Equal(k2,1.0)==FALSE)))
     regen_ctx=TRUE;
}



static void sktekst(char  *adr,double x,double y,double k1, double k2)
{ TEXT *t;
  double xp,yp;
  t=(TEXT*)adr;
  Scale_Point (k1, k2 ,x,y,t->x,t->y,&xp,&yp);
  t->x=xp;t->y=yp;
  if ((t->obiektt2!=O2BlockDim) || (options1.scale_DIM == 1)) 
   {
     t->wysokosc*= fabs (k2);  //  zmieniono z uwagi na skalowanie w profilu
     if (k2==0.0) k2=1.0;
     t->width_factor*= fabs (k1/k2);  //
	 t->width = 0;
	 t->height = 0;
   }  
  t->widoczny=tekst_wybrany(t);
}

void Scale_Arc (LUK *ptrs_arc ,double x, double y, double k1, double k2)
/*--------------------------------------------------------------------*/
{
  double xp,yp, x1, y1, x2, y2 ;

  if (FALSE == Check_if_Equal (fabs (k1), fabs (k2)))
  {
    //converting to elliptical arc
    return;
  }
  Arc_To_Points (ptrs_arc, &x1, &y1, &x2, &y2);
  Scale_Point (k1, k2, x, y, x1, y1, &x1, &y1) ;
  Scale_Point (k1, k2, x, y, x2, y2, &x2, &y2) ;
  Scale_Point (k1, k2 ,x,y,ptrs_arc->x,ptrs_arc->y,&xp,&yp) ;
  ptrs_arc->x = xp;
  ptrs_arc->y = yp;
  if (x1 != x2 || y1 != y2)
  {
    if ( (k1 > 0 && k2 > 0) || (k1 < 0 && k2 < 0) )
    {
      Points_To_Arc (ptrs_arc, x1, y1, x2, y2) ;
    }
    else
    {
      Points_To_Arc (ptrs_arc, x2, y2, x1, y1) ;
    }
  }
  else
  {
    ptrs_arc->r = 0;
  }
}

void Scale_Arc_Vector (AVECTOR *ptrs_vector ,double x, double y, double k1, double k2)
/*------------------------------------------------------------------------------*/
{
    double xp,yp, x1, y1, x2, y2 ;

    Vector_Arc_To_Points (ptrs_vector, &x1, &y1, &x2, &y2);
    Scale_Point (k1, k2, x, y, x1, y1, &x1, &y1) ;
    Scale_Point (k1, k2, x, y, x2, y2, &x2, &y2) ;
    Scale_Point (k1, k2 ,x,y,ptrs_vector->x1,ptrs_vector->y1,&xp,&yp) ;
    ptrs_vector->x1 = xp;
    ptrs_vector->y1 = yp;
    if (x1 != x2 || y1 != y2)
    {
        if ( (k1 > 0 && k2 > 0) || (k1 < 0 && k2 < 0) )
        {
            Points_To_Arc_Vector (ptrs_vector, x1, y1, x2, y2) ;
        }
        else
        {
            Points_To_Arc_Vector (ptrs_vector, x2, y2, x1, y1) ;
        }
    }
    else
    {
        ptrs_vector->r = 0;
    }
}

void Scale_Arc_Dim (LUK *ptrs_arc ,double x, double y, double k1, double k2)
/*--------------------------------------------------------------------*/
{
    double xp,yp, x1, y1, x2, y2 ;
    double l1, k0;

    if (FALSE == Check_if_Equal (fabs (k1), fabs (k2)))
    {
        return ;
    }

    //expanding angles
    /*
    if (ptrs_arc->kat2<ptrs_arc->kat1) l1=(ptrs_arc->kat2-(PI*2.0-ptrs_arc->kat1))*ptrs_arc->r;
    else l1=(ptrs_arc->kat2-ptrs_arc->kat1)*ptrs_arc->r;
    if (l1>5.0) k0=1.5/ptrs_arc->r; else k0=0.0;
    ptrs_arc->kat1-=k0;
    ptrs_arc->kat2+=k0;
     */

    l1=(ptrs_arc->kat2-ptrs_arc->kat1)*(ptrs_arc->r-zmwym.linia_ob);
    if (l1>5.0) k0=2.49/ptrs_arc->r; else k0=0.0;

    ptrs_arc->kat1-=k0;
    ptrs_arc->kat2+=k0;

    Arc_To_Points (ptrs_arc, &x1, &y1, &x2, &y2);
    Scale_Point (k1, k2, x, y, x1, y1, &x1, &y1) ;
    Scale_Point (k1, k2, x, y, x2, y2, &x2, &y2) ;
    Scale_Point (k1, k2 ,x,y,ptrs_arc->x,ptrs_arc->y,&xp,&yp) ;
    ptrs_arc->x = xp;
    ptrs_arc->y = yp;
    if (x1 != x2 || y1 != y2)
    {
        if ( (k1 > 0 && k2 > 0) || (k1 < 0 && k2 < 0) )
        {
            Points_To_Arc (ptrs_arc, x1, y1, x2, y2) ;
        }
        else
        {
            Points_To_Arc (ptrs_arc, x2, y2, x1, y1) ;
        }
        //l1=(ptrs_arc->kat2-ptrs_arc->kat1)*ptrs_arc->r;
        //if (l1>5.0) k0=1.5/ptrs_arc->r; else k0=0.0;


        l1=(ptrs_arc->kat2-ptrs_arc->kat1)*(ptrs_arc->r-zmwym.linia_ob);
        if (l1>5.0) k0=2.49/ptrs_arc->r; else k0=0.0;

        ptrs_arc->kat1+=k0;
        ptrs_arc->kat2-=k0;

    }
    else
    {
        ptrs_arc->r = 0;
    }
}

void Scale_SolidArc (SOLIDARC *ptrs_solidarc ,double x, double y, double k1, double k2)
/*-----------------------------------------------------------------------------------*/
{
    double xp,yp, x1, y1, x2, y2 ;

    if (FALSE == Check_if_Equal (fabs (k1), fabs (k2)))
    {
        //converting to elliptical arc
        return;
    }
    SolidArc_To_Points (ptrs_solidarc, &x1, &y1, &x2, &y2);
    Scale_Point (k1, k2, x, y, x1, y1, &x1, &y1) ;
    Scale_Point (k1, k2, x, y, x2, y2, &x2, &y2) ;
    Scale_Point (k1, k2 ,x,y,ptrs_solidarc->x,ptrs_solidarc->y,&xp,&yp) ;
    ptrs_solidarc->x = xp;
    ptrs_solidarc->y = yp;
    if (x1 != x2 || y1 != y2)
    {
        if ( (k1 > 0 && k2 > 0) || (k1 < 0 && k2 < 0) )
        {
            Points_To_SolidArc (ptrs_solidarc, x1, y1, x2, y2) ;
        }
        else
        {
            Points_To_SolidArc (ptrs_solidarc, x2, y2, x1, y1) ;
        }
    }
    else
    {
        ptrs_solidarc->r = 0;
    }

    ptrs_solidarc->width1*=k1;
    ptrs_solidarc->width2*=k1;

}

static void skluk(char  *adr,double x,double y,double k1, double k2)
{
  LUK * ptrs_arc ;

  ptrs_arc = (LUK*)adr ;

    if (ptrs_arc->obiektt2!=O2BlockDim)
    {
        Scale_Arc(ptrs_arc, x, y, k1, k2);
    }
    else {
        Scale_Arc_Dim(ptrs_arc, x, y, k1, k2);
    }

  ptrs_arc->widoczny = luk_wybrany (ptrs_arc) ;
}

static void sksolidarc(char  *adr,double x,double y,double k1, double k2)
{
    SOLIDARC * ptrs_solidarc ;

    ptrs_solidarc = (SOLIDARC*)adr ;

    Scale_SolidArc(ptrs_solidarc, x, y, k1, k2);

    ptrs_solidarc->widoczny = solidarc_wybrany (ptrs_solidarc) ;
}


static void skokrag(char  *adr,double x,double y,double k1, double k2)
{
  OKRAG *o;
  double xp,yp;


  if (FALSE == Check_if_Equal (fabs (k1), fabs (k2)))
  {
    return ;
  }

  o=(OKRAG*)adr;
  Scale_Point (k1, k2 ,x,y,o->x,o->y,&xp,&yp);
  o->x=xp;o->y=yp;
  o->r*=fabs (k1);
  o->widoczny=okrag_wybrany(o);
}
static void skkolo(char  *adr,double x,double y,double k1, double k2)
{
  OKRAG *K;
  double xp,yp;


  if (FALSE == Check_if_Equal (fabs (k1), fabs (k2)))
  {
     return ;
  }

  K=(OKRAG*)adr;
  Scale_Point (k1, k2 ,x,y,K->x,K->y,&xp,&yp);
  K->x=xp;K->y=yp;
  if ((K->obiektt2!=O2BlockDim) || (options1.scale_DIM == 1)) 
    {
      K->r*= fabs(k1);
    }  
  K->widoczny=okrag_wybrany(K);
}


static void skellipse(char  *adr,double x,double y,double k1, double k2)
{
    ELLIPSE *e;
    double xp,yp, ex1, ey1, ex2, ey2;
    double koc, kos;
    double erx, ery;


    e=(ELLIPSE*)adr;

    if (FALSE == Check_if_Equal (fabs (k1), fabs (k2)))
    {
        if (Check_if_Equal(fmod(e->angle, Pi/2.0), 0.0)==FALSE)
        return ;
    }

    kos=sin(e->angle);
    koc=cos(e->angle);

    Rotate_Point(kos,koc,e->x, e->y, e->x+e->rx, e->y, &ex1, &ey1);
    Rotate_Point(kos,koc,e->x, e->y, e->x, e->y+e->ry, &ex2, &ey2);

    Scale_Point (k1, k2 ,x,y,e->x,e->y,&xp,&yp);
    Scale_Point (k1, k2 ,x,y,ex1,ey1,&ex1,&ey1);
    Scale_Point (k1, k2 ,x,y,ex2,ey2,&ex2,&ey2);

    e->x=xp;e->y=yp;

    e->angle = Angle_Normal(atan2(ey1 - e->y, ex1 - e->x));  ////

    e->rx= sqrt((ex1-e->x)*(ex1-e->x) + (ey1-e->y)*(ey1-e->y));
    e->ry= sqrt((ex2-e->x)*(ex2-e->x) + (ey2-e->y)*(ey2-e->y));

    e->widoczny=elipsa_wybrana_prec(e);
}

static void skfilledellipse(char  *adr,double x,double y,double k1, double k2)
{
    skellipse(adr,x,y,k1,k2);
}

static void skellipticalarc(char  *adr,double x,double y,double k1, double k2)
{
    ELLIPTICALARC *ea;
    double xp,yp, ex1, ey1, ex2, ey2;
    double koc, kos;
    double erx, ery;
    double px, py, kx, ky, px1, py1, kx1, ky1;
    double dot,det;
    double vx1, vy1, vx2, vy2;
    double df_sangle, df_eangle;

    ea=(ELLIPTICALARC *)adr;

    //angles
    Get_EllipticalArc_EndPoints (ea->x, ea->y, ea->rx, ea->ry, ea->angle, ea->kat1, ea->kat2, &px, &py, &kx, &ky);

    if (FALSE == Check_if_Equal (fabs (k1), fabs (k2)))
    {
        if (Check_if_Equal(fmod(ea->angle, Pi/2.0), 0.0)==FALSE)
            return ;
    }

    kos=sin(ea->angle);
    koc=cos(ea->angle);

    Rotate_Point(kos,koc,ea->x, ea->y, ea->x+ea->rx, ea->y, &ex1, &ey1);
    Rotate_Point(kos,koc,ea->x, ea->y, ea->x, ea->y+ea->ry, &ex2, &ey2);

    Rotate_Point(kos,koc,ea->x, ea->y, px, py, &px1, &py1);
    Rotate_Point(kos,koc,ea->x, ea->y, kx, ky, &kx1, &ky1);


    Scale_Point (k1, k2 ,x,y,ea->x,ea->y,&xp,&yp);
    Scale_Point (k1, k2 ,x,y,ex1,ey1,&ex1,&ey1);
    Scale_Point (k1, k2 ,x,y,ex2,ey2,&ex2,&ey2);
    Scale_Point (k1, k2 ,x,y,px1,py1,&px1,&py1);
    Scale_Point (k1, k2 ,x,y,kx1,ky1,&kx1,&ky1);

    ea->x=xp;ea->y=yp;

    ea->rx= sqrt((ex1-ea->x)*(ex1-ea->x) + (ey1-ea->y)*(ey1-ea->y));
    ea->ry= sqrt((ex2-ea->x)*(ex2-ea->x) + (ey2-ea->y)*(ey2-ea->y));


    //angles
    vx1 = px1 - ea->x;
    vy1 = py1 - ea->y;
    vx2 = 1.0;
    vy2 = 0.0;

    dot = vx1 * vx2 + vy1 * vy2;  // Dot product between [vx1, vy1] and [vx2, vy2]
    det = vx1 * vy2 - vy1 * vx2;  // Determinant
    df_sangle = -atan2(det, dot) - ea->angle;  // atan2(y, x) or atan2(sin, cos)
    df_sangle=fmod(df_sangle, Pi2);
    if (df_sangle < 0) df_sangle += Pi2;


    vx1 = kx1 - ea->x;
    vy1 = ky1 - ea->y;
    vx2 = 1.0;
    vy2 = 0.0;

    dot = vx1 * vx2 + vy1 * vy2;     // Dot product between [vx1, vy1] and [vx2, vy2]
    det = vx1 * vy2 - vy1 * vx2;     // Determinant
    df_eangle = -atan2(det, dot) - ea->angle;  // atan2(y, x) or atan2(sin, cos)
    df_eangle=fmod(df_eangle, Pi2);
    if (df_eangle < 0) df_eangle += Pi2;

    ea->widoczny=elipsa_wybrana_prec((ELLIPSE*)ea);
}


static void skspline(char  *adr, double x, double y, double k1, double k2)
{
	SPLINE *s;
	int i;
	double xp, yp;

	s = (SPLINE*)adr;

	for (i = 0; i < (int)s->lp; i += 2)
	{
		Scale_Point(k1, k2, x, y, s->xy[i], s->xy[i + 1], &xp, &yp);
		s->xy[i] = xp; s->xy[i + 1] = yp;
	}
	
	s->widoczny = spline_wybrany(s);
}


static void skwwielokat(char  *adr,double x,double y,double k1, double k2)
{ WIELOKAT *w;
  int i;
  double xp,yp;
  double del_x1, del_y1, del_x2, del_y2;
  
  w=(WIELOKAT*)adr;
  if ((w->obiektt2!=O2BlockDim) || (options1.scale_DIM == 1)) 
   {
     for (i=0;i<(int)w->lp;i+=2)
     { Scale_Point (k1, k2 ,x,y,w->xy[i],w->xy[i+1],&xp,&yp);
       w->xy[i]=xp;w->xy[i+1]=yp;
     }
   }
   else
    {
     del_x1=w->xy[0]-w->xy[2];
     del_y1=w->xy[1]-w->xy[3];
     del_x2=w->xy[4]-w->xy[2];
     del_y2=w->xy[5]-w->xy[3];
     Scale_Point (k1, k2 ,x,y,w->xy[2],w->xy[3],&xp,&yp);
       w->xy[2]=xp;w->xy[3]=yp;
       
       w->xy[0]=w->xy[2]+del_x1;
       w->xy[1]=w->xy[3]+del_y1;
       w->xy[4]=w->xy[2]+del_x2;
       w->xy[5]=w->xy[3]+del_y2;
       if (w->lp==8)
       {
           w->xy[6]=w->xy[4];
           w->xy[7]=w->xy[5];
       }
    }  
  w->widoczny=wielokat_wybrany(w);
}


static void skwwielokat3D(char  *adr,double x,double y,double k1, double k2)
{ WIELOKAT *w;
  int i, j;
  double xp,yp,zp;
  double del_x1, del_y1, del_x2, del_y2;
  double z, k3;
  
  w=(WIELOKAT*)adr;
  if ((w->obiektt2!=O2BlockDim) || (options1.scale_DIM == 1)) 
   {
     for (i=0,j=0; i<(int)w->lp;i+=2, j++)
     { //Scale_Point (k1, k2 ,x,y,w->xy[i],w->xy[i+1],&xp,&yp);
       k3=k1;
       z=0;
       Scale_Point3D (k1, k2, k3 ,x,y,z,w->xy[i],w->xy[i+1],w->xy[w->lp+j],&xp,&yp,&zp);
       w->xy[i]=xp;w->xy[i+1]=yp;w->xy[w->lp+j]=zp;
     }
   }
   else
    {
     del_x1=w->xy[0]-w->xy[2];
     del_y1=w->xy[1]-w->xy[3];
     del_x2=w->xy[4]-w->xy[2];
     del_y2=w->xy[5]-w->xy[3];
     Scale_Point (k1, k2 ,x,y,w->xy[2],w->xy[3],&xp,&yp);
       w->xy[2]=xp;w->xy[3]=yp;
       
       w->xy[0]=w->xy[2]+del_x1;
       w->xy[1]=w->xy[3]+del_y1;
       w->xy[4]=w->xy[2]+del_x2;
       w->xy[5]=w->xy[3]+del_y2;
    }  
  w->widoczny=wielokat_wybrany(w);
}
/*-----------------------------------------------------------*/

void Rotate_Line (void *adr, double df_x, double df_y, double df_si, double df_co)
/*-------------------------------------------------------------------------------------------*/
{ LINIA *l;
  double xp,yp;
  l=(LINIA*)adr;
  Rotate_Point (df_si, df_co, df_x, df_y, l->x1, l->y1, &xp, &yp) ;
  l->x1=xp;l->y1=yp;
  Rotate_Point (df_si, df_co, df_x, df_y, l->x2, l->y2, &xp, &yp) ;
  l->x2=xp;l->y2=yp;
}

void Rotate_Line_Vector (void *adr, double df_x, double df_y, double df_si, double df_co)
/*-------------------------------------------------------------------------------------------*/
{ AVECTOR *v;
    double xp,yp;
    v=(AVECTOR*)adr;
    Rotate_Point (df_si, df_co, df_x, df_y, v->x1, v->y1, &xp, &yp) ;
    v->x1=xp;v->y1=yp;
    Rotate_Point (df_si, df_co, df_x, df_y, v->x2, v->y2, &xp, &yp) ;
    v->x2=xp;v->y2=yp;
}

void Rotate_Line3D (void *adr, double df_x, double df_y, double df_si, double df_co)
/*-------------------------------------------------------------------------------*/
{ LINIA3D *l;
  double xp,yp;
  l=(LINIA3D*)adr;
  Rotate_Point (df_si, df_co, df_x, df_y, l->x1, l->y1, &xp, &yp) ;
  l->x1=xp;l->y1=yp;
  Rotate_Point (df_si, df_co, df_x, df_y, l->x2, l->y2, &xp, &yp) ;
  l->x2=xp;l->y2=yp;
}

void Rotate_Line3DXZ (void *adr, double df_x, double df_y, double df_z, double df_si, double df_co)
/*-----------------------------------------------------------------------------------------------*/
{ LINIA3D *l;
  double xp,zp;
  l=(LINIA3D*)adr;
  Rotate_Point (df_si, df_co, df_x, df_z, l->x1, l->z1, &xp, &zp) ;
  l->x1=xp;l->z1=zp;
  Rotate_Point (df_si, df_co, df_x, df_z, l->x2, l->z2, &xp, &zp) ;
  l->x2=xp;l->z2=zp;
}

void Rotate_Line3DYZ (void *adr, double df_x, double df_y, double df_z, double df_si, double df_co)
/*------------------------------------------------------------------------------------------------*/
{ LINIA3D *l;
  double yp,zp;
  l=(LINIA3D*)adr;
  Rotate_Point (df_si, df_co, df_y, df_z, l->y1, l->z1, &yp, &zp) ;
  l->y1=yp;l->z1=zp;
  Rotate_Point (df_si, df_co, df_y, df_z, l->y2, l->z2, &yp, &zp) ;
  l->y2=yp;l->z2=zp;
}

void Rotate_Arc_Vector (void *adr, double df_x, double df_y, double df_si, double df_co)
/*------------------------------------------------------------------------------*/
{
    AVECTOR *v ;
    double xp, yp, xs, ys, xe, ye ;

    v = (AVECTOR*)adr ;
    xs=v->x1+v->r*cos(v->angle1);
    ys=v->y1+v->r*sin(v->angle1);
    xe=v->x1+v->r*cos(v->angle2);
    ye=v->y1+v->r*sin(v->angle2);
    Rotate_Point (df_si, df_co, df_x,df_y,v->x1,v->y1,&xp,&yp);
    v->x1=xp;v->y1=yp;
    Rotate_Point (df_si, df_co, df_x,df_y,xs,ys,&xs,&ys);
    Rotate_Point (df_si, df_co, df_x,df_y,xe,ye,&xe,&ye);
    xs-=v->x1;  xe-=v->x1;
    ys-=v->y1;  ye-=v->y1;
    v->angle1 = Angle_Normal (Atan2 (ys,xs)) ;
    v->angle2 = Angle_Normal (Atan2 (ye, xe)) ;
}

static void oblinia(char  *adr,double x,double y,double k1,double k2)
{
  LINIA *l;

  l=(LINIA*)adr;
  Rotate_Line ((void*)adr, x, y, k1, k2) ;
  if(linia_wybrana(l)) l->widoczny=1;
  else l->widoczny=0;
}

static void obvector(char  *adr,double x,double y,double k1,double k2)
{
    AVECTOR *v;

    v=(AVECTOR*)adr;
    switch (v->style)
    {
        case 5:
        case 6:
        case 8:
        case 9:
            Rotate_Arc_Vector ((void*)adr, x, y, k1, k2) ;
        break;
        default:
            Rotate_Line_Vector ((void*)adr, x, y, k1, k2) ;
        break;
    }
    if(vector_wybrany(v)) v->widoczny=1;
    else v->widoczny=0;
}

static void oblinia3D(char  *adr,double x,double y,double k1,double k2)
{
  LINIA3D *l;

  l=(LINIA3D*)adr;
  Rotate_Line3D ((void*)adr, x, y, k1, k2) ;
  if(linia_wybrana((LINIA *)l)) l->widoczny=1;
  else l->widoczny=0;
}

static void oblinia3DXZ(char *adr,double x,double y,double z,double k1,double k2)
{
  LINIA3D *l;

  l=(LINIA3D*)adr;
  Rotate_Line3DXZ ((void*)adr, x, y, z, k1, k2) ;
  if(linia_wybrana((LINIA *)l)) l->widoczny=1;
  else l->widoczny=0;
}

static void oblinia3DYZ(char  *adr,double x,double y,double z,double k1,double k2)
{
  LINIA3D *l;

  l=(LINIA3D*)adr;
  Rotate_Line3DYZ ((void*)adr, x, y, z, k1, k2) ;
  if(linia_wybrana((LINIA *)l)) l->widoczny=1;
  else l->widoczny=0;
}


double angle_from_sin_cos( double sinx, double cosx ) //result in -pi to +pi range
{
    double ang_from_cos = acos(cosx);
    double ang_from_sin = asin(sinx);
    double sin2 = sinx*sinx;
    if(sinx<0)
    {
        ang_from_cos = -ang_from_cos;
        if(cosx<0) //both negative
            ang_from_sin = -PI -ang_from_sin;
    }
    else if(cosx<0)
        ang_from_sin = PI - ang_from_sin;
    //now favor the computation coming from the
    //smaller of sinx and cosx, as the smaller
    //the input value, the smaller the error
    return (1.0-sin2)*ang_from_sin + sin2*ang_from_cos;
}

static void obblock (char  *adr,double x,double y,double k1, double k2)
/*------------------------------------------------------------*/
{
  BLOK 			*ptrs_block ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  double 		xp, yp ;
  float kk1, kk0;
  double kk01;

  ptrs_block = (BLOK*)adr ;
  ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
  if (((ptrs_block->kod_obiektu == B_EXPORT) ||
	   (ptrs_block->kod_obiektu == B_HATCH) ||
       (ptrs_block->kod_obiektu == B_PPOMIAR) ||
       (ptrs_block->kod_obiektu == B_KPOMIAR) ||
       (ptrs_block->kod_obiektu == B_ODWIERT_OLD) ||
       (ptrs_block->kod_obiektu == B_ODWIERT) ||
       (ptrs_block->kod_obiektu == B_INSTALACJE) ||
       (ptrs_block->kod_obiektu == B_HEKTOMETRY) ||
       (ptrs_block->kod_obiektu == B_SEKTORY) ||
       (ptrs_block->kod_obiektu == B_WIRE) ||
       (ptrs_block->kod_obiektu == B_ARM) ||
       (ptrs_block->kod_obiektu == B_SIEC) ||
       (ptrs_block->kod_obiektu == B_VERTEX)
     )
     && ptrs_desc_bl->flags == EBF_IP)
  {
    Rotate_Point (k1, k2 ,x, y, ptrs_desc_bl->x, ptrs_desc_bl->y, &xp, &yp) ;
    ptrs_desc_bl->x = xp ;
    ptrs_desc_bl->y = yp ;
  }
   else if ((ptrs_block->kod_obiektu == B_DIM) || (ptrs_block->kod_obiektu == B_DIM2))
    {

	 if (ptrs_block->dlugosc_opisu_obiektu >= 4) memmove(&kk0, &ptrs_block->opis_obiektu[ptrs_block->dlugosc_opisu_obiektu - sizeof(kk0)], sizeof(kk0));
	  else memmove(&kk0,&(ptrs_block->opis_obiektu[0]),sizeof(kk0));

     kk01=angle_from_sin_cos(k1, k2);
     kk1=kk0+((kk01/Pi2)*360);
     if (kk1>360) kk1-=360;
     if (kk1<0) kk1+=360;

	 if (ptrs_block->dlugosc_opisu_obiektu >= 4) memmove( &ptrs_block->opis_obiektu[ptrs_block->dlugosc_opisu_obiektu - sizeof(kk0)], &kk1, sizeof(kk1));
	 else memmove(&(ptrs_block->opis_obiektu[0]),&kk1,sizeof(kk1)); 
     
    }
}

static void obpoint (char  *adr,double x,double y,double k1, double k2)
/*------------------------------------------------------------*/
{
  T_Point *ptrs_point ;
  double  xp, yp ;

  ptrs_point = (T_Point*)adr ;
  Rotate_Point (k1, k2 ,x, y, ptrs_point->x, ptrs_point->y, &xp, &yp) ;
  ptrs_point->x = xp ;
  ptrs_point->y = yp ;
}

static void obpcx (char  *adr,double x,double y,double k1, double k2)
/*------------------------------------------------------------*/
{
  B_PCX *pcx ;
  double  xp, yp ;
  double angle;
  double pcx_kat;

  pcx = (B_PCX*)adr ;
  pcx_kat=pcx->kat;
  Rotate_Point (k1, k2 ,x, y, pcx->x, pcx->y, &xp, &yp) ;
  pcx->x = xp ;
  pcx->y = yp ;

  angle = Atan2(k1, k2);
 
  pcx->kat += angle;

  if (pcx->kat >= Pi2) pcx->kat-=Pi2;
  if (pcx->kat <0) pcx->kat+=Pi2;
  
  if ((!pcx->on_front) && (Check_if_Equal(pcx_kat,pcx->kat)==FALSE)) regen_ctx=TRUE;
}


static void obtekst(char  *adr,double x,double y,double k1,double k2)
{ TEXT *ptrs_text, ptrs_text_=Tdef;
  double xp,yp,X2,Y2,tdl,tsin,tcos,xt1,yt1,xt2,yt2,xt3,yt3,xt4,yt4;
  double del_wys=1.0;
  char *ptr_index1,*ptr_index2;
  
  ptrs_text = (TEXT*)adr;

  tdl = Get_TextLen (ptrs_text, NULL) ;
  ptr_index1=strchr(ptrs_text->text,'^');
  ptr_index2=strchr(ptrs_text->text,'~');
  if ((ptr_index1!=NULL) && (ptr_index2==NULL)) del_wys=1.15;
  else if ((ptr_index1==NULL) && (ptr_index2!=NULL)) del_wys=0.86956;

  if (tdl==0)
   {
     memmove(&ptrs_text_,ptrs_text, sizeof(NAGLOWEK) + ptrs_text->n);
     strcat(ptrs_text_.text," ");
     ptrs_text_.dl+=1;
     ptrs_text_.n+=1;
     tdl = Get_TextLen (&ptrs_text_, NULL) ;
   }
  tsin=sin(ptrs_text->kat);
  tcos=cos(ptrs_text->kat);
  X2=ptrs_text->x+tdl*tcos;  Y2=ptrs_text->y-Ky*tdl*tsin;
  Rotate_Point (k1,k2,x,y,ptrs_text->x,ptrs_text->y,&xp,&yp); 
  ptrs_text->x=xp;ptrs_text->y=yp;
  Rotate_Point (k1,k2,x,y,X2,Y2,&X2,&Y2); 

     ptrs_text->kat=Atan2(Y2-ptrs_text->y,X2-ptrs_text->x);

     tsin = sin (ptrs_text->kat) ;
     tcos = cos (ptrs_text->kat) ;
     xt1 = ptrs_text->x ;
     yt1 = ptrs_text->y;
     switch (ptrs_text->justowanie)  
     {
     case 0 : xt2 = xt1 + tdl * tcos ;   /*do lewej*/
              yt2 = yt1 - Ky * tdl * tsin ;
              xt3 = xt2 - (ptrs_text->wysokosc * del_wys) * tsin ;
              yt3 = yt2 - Ky * (ptrs_text->wysokosc * del_wys) * tcos ;
              xt4 = xt1 - (ptrs_text->wysokosc * del_wys) * tsin ;
              yt4 = yt1 + (ptrs_text->wysokosc * del_wys) * tcos ;
              break;
     case 1 : xt2 = xt1 - tdl * tcos ;   /*do prawej*/
              yt2 = yt1 + Ky * tdl * tsin ;
              xt3 = xt2 - (ptrs_text->wysokosc * del_wys) * tsin ;
              yt3 = yt2 - Ky * (ptrs_text->wysokosc * del_wys) * tcos ;
              xt4 = xt1 - (ptrs_text->wysokosc * del_wys) * tsin ;
              yt4 = yt1 + (ptrs_text->wysokosc * del_wys) * tcos ;
              break; 
     case 2 : xt2 = xt1;   /*srodkowo*/
              yt2 = yt1;
              xt3 = xt2 - (ptrs_text->wysokosc * del_wys) * tsin ;
              yt3 = yt2 - Ky * (ptrs_text->wysokosc * del_wys) * tcos ;
              break;
     case 3 : xt2 = xt1;   /*centralnie*/
              yt2 = yt1;
              xt3 = xt2 - 0.5 * (ptrs_text->wysokosc) * (del_wys -1) * tsin ;
              yt3 = yt2 - 0.5 * Ky * (ptrs_text->wysokosc) * (del_wys -1) * tcos ;
              break;
     default :
              break;
     }
     /**/
     if (ptrs_text->kat > Pi2) ptrs_text->kat -= Pi2;
     if (ptrs_text->kat < 0) ptrs_text->kat += Pi2;


     if (normalize_text==TRUE)
      {
        ////ptrs_text->width=0.0;

       if ((ptrs_text->kat > (1.001*Pi2/4)) && (ptrs_text->kat < (1.001*Pi2*0.75)))
        {
         ptrs_text->kat -= (Pi2/2);
         if (ptrs_text->kat > Pi2) ptrs_text->kat -= Pi2;
         ptrs_text->x = xt3 ;
         ptrs_text->y = yt3 ;
         
         if (ptrs_text->typ == n_typ_pin_name)
          {
           if (ptrs_text->justowanie == j_do_lewej)
            {
             ptrs_text->justowanie = j_do_prawej;
             ptrs_text->x = xt4 ;
             ptrs_text->y = yt4 ;
            }
            else
             {
              if (ptrs_text->justowanie == j_do_prawej)
               {
                ptrs_text->justowanie = j_do_lewej;
                ptrs_text->x = xt4 ;
                ptrs_text->y = yt4 ;
               }
             }
          }

           
        }
      }

  ptrs_text->widoczny=tekst_wybrany(ptrs_text);
}


void obtekst_(char  *adr,double x,double y,double k1,double k2)
{ TEXT *ptrs_text, ptrs_text_=Tdef;
  double xp,yp,X2,Y2,tdl,tsin,tcos,xt1,yt1,xt2,yt2,xt3,yt3,xt4,yt4;
  double del_wys=1;
  
  ptrs_text = (TEXT*)adr;
  tdl = Get_TextLen (ptrs_text, NULL) ;
  if (tdl==0)
   {
     memmove(&ptrs_text_,ptrs_text, sizeof(NAGLOWEK) + ptrs_text->n);
     strcat(ptrs_text_.text," ");
     ptrs_text_.dl+=1;
     ptrs_text_.n+=1;
     tdl = Get_TextLen (&ptrs_text_, NULL) ;
   }
  tsin=sin(ptrs_text->kat);
  tcos=cos(ptrs_text->kat);
  X2=ptrs_text->x+tdl*tcos;  Y2=ptrs_text->y-Ky*tdl*tsin;
  Rotate_Point (k1,k2,x,y,ptrs_text->x,ptrs_text->y,&xp,&yp); 
  ptrs_text->x=xp;ptrs_text->y=yp;
  Rotate_Point (k1,k2,x,y,X2,Y2,&X2,&Y2); 

     ptrs_text->kat=Atan2(Y2-ptrs_text->y,X2-ptrs_text->x);

     tsin = sin (ptrs_text->kat) ;
     tcos = cos (ptrs_text->kat) ;
     xt1 = ptrs_text->x ;
     yt1 = ptrs_text->y;
     switch (ptrs_text->justowanie)  
     {
     case 0 : xt2 = xt1 + tdl * tcos ;   /*do lewej*/
              yt2 = yt1 - Ky * tdl * tsin ;
              xt3 = xt2 - (ptrs_text->wysokosc * del_wys) * tsin ;
              yt3 = yt2 - Ky * (ptrs_text->wysokosc * del_wys) * tcos ;
              xt4 = xt1 - (ptrs_text->wysokosc * del_wys) * tsin ;
              yt4 = yt1 + (ptrs_text->wysokosc * del_wys) * tcos ;
              break;
     case 1 : xt2 = xt1 - tdl * tcos ;   /*do prawej*/
              yt2 = yt1 + Ky * tdl * tsin ;
              xt3 = xt2 - (ptrs_text->wysokosc * del_wys) * tsin ;
              yt3 = yt2 - Ky * (ptrs_text->wysokosc * del_wys) * tcos ;
              xt4 = xt1 - (ptrs_text->wysokosc * del_wys) * tsin ;
              yt4 = yt1 + (ptrs_text->wysokosc * del_wys) * tcos ;
              break; 
     case 2 : xt2 = xt1;   /*srodkowo*/
              yt2 = yt1;
              xt3 = xt2 - (ptrs_text->wysokosc * del_wys) * tsin ;
              yt3 = yt2 - Ky * (ptrs_text->wysokosc * del_wys) * tcos ;
              break;
     case 3 : xt2 = xt1;   /*centralnie*/
              yt2 = yt1;
              xt3 = xt2 - 0.5 * (ptrs_text->wysokosc) * (del_wys -1) * tsin ;
              yt3 = yt2 - 0.5 * Ky * (ptrs_text->wysokosc) * (del_wys -1) * tcos ;
              break;
     default :
              break;
     }

     if (ptrs_text->kat > Pi2) ptrs_text->kat -= Pi2;
     if (ptrs_text->kat < 0) ptrs_text->kat += Pi2;


     if (normalize_text==TRUE)
      {
       if ((ptrs_text->kat > (1.001*Pi2/4)) && (ptrs_text->kat < (1.001*Pi2*0.75)))
        {
         ptrs_text->kat -= (Pi2/2);
         if (ptrs_text->kat > Pi2) ptrs_text->kat -= Pi2;
         ptrs_text->x = xt3 ;
         ptrs_text->y = yt3 ;
         
         if (ptrs_text->typ == n_typ_pin_name)
          {
           if (ptrs_text->justowanie == j_do_lewej)
            {
             ptrs_text->justowanie = j_do_prawej;
             ptrs_text->x = xt4 ;
             ptrs_text->y = yt4 ;
            }
            else
             {
              if (ptrs_text->justowanie == j_do_prawej)
               {
                ptrs_text->justowanie = j_do_lewej;
                ptrs_text->x = xt4 ;
                ptrs_text->y = yt4 ;
               }
             }
          }
          

           
        }
      }
  ptrs_text->widoczny=tekst_wybrany(ptrs_text);
}

void Rotate_Arc (void *adr, double df_x, double df_y, double df_si, double df_co)
/*------------------------------------------------------------------------------*/
{
  LUK *l ;
  double xp, yp, xs, ys, xe, ye ;

  l = (LUK*)adr ;
  xs=l->x+l->r*cos(l->kat1);
  ys=l->y+l->r*sin(l->kat1);
  xe=l->x+l->r*cos(l->kat2);
  ye=l->y+l->r*sin(l->kat2);
  Rotate_Point (df_si, df_co, df_x,df_y,l->x,l->y,&xp,&yp);
  l->x=xp;l->y=yp;
  Rotate_Point (df_si, df_co, df_x,df_y,xs,ys,&xs,&ys);
  Rotate_Point (df_si, df_co, df_x,df_y,xe,ye,&xe,&ye);
  xs-=l->x;  xe-=l->x;
  ys-=l->y;  ye-=l->y;
  l->kat1 = Angle_Normal (Atan2 (ys,xs)) ;
  l->kat2 = Angle_Normal (Atan2 (ye, xe)) ;
}

void Rotate_SolidArc(void *adr, double df_x, double df_y, double df_si, double df_co)
/*---------------------------------------------------------------------------------*/
{
    SOLIDARC *sa ;
    double xp, yp, xs, ys, xe, ye ;

    sa = (SOLIDARC*)adr ;
    xs=sa->x+sa->r*cos(sa->kat1);
    ys=sa->y+sa->r*sin(sa->kat1);
    xe=sa->x+sa->r*cos(sa->kat2);
    ye=sa->y+sa->r*sin(sa->kat2);
    Rotate_Point (df_si, df_co, df_x,df_y,sa->x,sa->y,&xp,&yp);
    sa->x=xp;sa->y=yp;
    Rotate_Point (df_si, df_co, df_x,df_y,xs,ys,&xs,&ys);
    Rotate_Point (df_si, df_co, df_x,df_y,xe,ye,&xe,&ye);
    xs-=sa->x;  xe-=sa->x;
    ys-=sa->y;  ye-=sa->y;
    sa->kat1 = Angle_Normal (Atan2 (ys,xs)) ;
    sa->kat2 = Angle_Normal (Atan2 (ye, xe)) ;
}

static void obluk (char  *adr,double x,double y,double k1,double k2)
{
  LUK *l;

  Rotate_Arc ((void*)adr, x, y, k1, k2) ;
  l = (LUK*)adr ;
  l->widoczny = luk_wybrany (l) ;
}

static void obsolidarc (char  *adr,double x,double y,double k1,double k2)
{
    SOLIDARC *sa;

    Rotate_SolidArc ((void*)adr, x, y, k1, k2) ;
    sa = (SOLIDARC*)adr ;
    sa->widoczny = solidarc_wybrany (sa) ;
}

static void obokrag(char  *adr,double x,double y,double k1,double k2)
{ OKRAG *o;
  double xp,yp;
  o=(OKRAG*)adr;
  Rotate_Point (k1,k2,x,y,o->x,o->y,&xp,&yp);
  o->x=xp;o->y=yp;
  o->widoczny=okrag_wybrany(o);
}

static void obkolo(char  *adr,double x,double y,double k1,double k2)
{ OKRAG *K;
  double xp,yp;
  K=(OKRAG*)adr;
  Rotate_Point (k1,k2,x,y,K->x,K->y,&xp,&yp);
  K->x=xp;K->y=yp;
  K->widoczny=okrag_wybrany(K);
}

static void obellipticalarc(char  *adr,double x,double y,double k1,double k2)
{ ELLIPTICALARC *ea;
    double xp,yp;
    ea=(ELLIPTICALARC*)adr;
    Rotate_Point (k1,k2,x,y,ea->x,ea->y,&xp,&yp);
    ea->x=xp;ea->y=yp;
    ea->angle = Angle_Normal(ea->angle+Atan2(k1, k2));
    ea->widoczny=lukeliptyczny_wybrany_prec(ea);
}

static void obellipse(char  *adr,double x,double y,double k1,double k2)
{ ELLIPSE *e;
    double xp,yp;
    e=(ELLIPSE*)adr;
    Rotate_Point (k1,k2,x,y,e->x,e->y,&xp,&yp);
    e->x=xp;e->y=yp;
    e->angle = Angle_Normal(e->angle+Atan2(k1, k2));
    e->widoczny=elipsa_wybrana_prec(e);
}

static void obfilledellipse(char  *adr,double x,double y,double k1,double k2)
{ ELLIPSE *fe;
    double xp,yp;
    fe=(ELLIPSE*)adr;
    Rotate_Point (k1,k2,x,y,fe->x,fe->y,&xp,&yp);
    fe->x=xp;fe->y=yp;
    fe->angle = Angle_Normal(fe->angle+Atan2(k1, k2));
    fe->widoczny=elipsa_wybrana_prec(fe);
}


static void obspline(char  *adr, double x, double y, double k1, double k2)
{
	SPLINE *s;
	int i;
	double xp, yp;
	s = (SPLINE*)adr;
	for (i = 0; i < (int)s->lp; i += 2)
	{
		Rotate_Point(k1, k2, x, y, s->xy[i], s->xy[i + 1], &xp, &yp);
		s->xy[i] = xp; s->xy[i + 1] = yp;
	}
	s->widoczny = spline_wybrany(s);
}

static void obwwielokat(char  *adr,double x,double y,double k1,double k2)
{ WIELOKAT *w;
  int i;
  double xp,yp;
  w=(WIELOKAT*)adr;
  for (i=0;i< (int)w->lp;i+=2)
   { Rotate_Point (k1,k2,x,y,w->xy[i],w->xy[i+1],&xp,&yp);
     w->xy[i]=xp;w->xy[i+1]=yp;
   }
  w->widoczny=wielokat_wybrany(w);
}

static void obwwielokatPCX(char  *adr,double x,double y,double k1,double k2)
{ WIELOKAT *w;
  int i;
  double xp,yp;
  w=(WIELOKAT*)adr;
  for (i=0;i< (int)w->lp;i+=2)
   { Rotate_Point (k1,k2,x,y,w->xy[i],w->xy[i+1],&xp,&yp);
     w->xy[i]=xp;w->xy[i+1]=yp;
   }
  w->widoczny=wielokat_wybrany(w);
}

static void obwwielokat3DXZ(char  *adr,double x,double y,double z,double k1,double k2)
{ WIELOKAT *w;
  int i,j;
  double xp,zp;
  w=(WIELOKAT*)adr;
  for (i=0,j=0;i< (int)w->lp;i+=2,j++)
   { Rotate_Point (k1,k2,x,z,w->xy[i],w->xy[w->lp+j],&xp,&zp);
     w->xy[i]=xp;w->xy[w->lp+j]=zp;
   }
  w->widoczny=wielokat_wybrany(w);
}

static void obwwielokat3DYZ(char  *adr,double x,double y,double z,double k1,double k2)
{ WIELOKAT *w;
  int i,j;
  double yp,zp;
  w=(WIELOKAT*)adr;
  for (i=0,j=0;i< (int)w->lp;i+=2,j++)
   { Rotate_Point (k1,k2,y,z,w->xy[i+1],w->xy[w->lp+j],&yp,&zp);
     w->xy[i+1]=yp;w->xy[w->lp+j]=zp;
   }
  w->widoczny=wielokat_wybrany(w);
}
/*-----------------------------------------------------------*/

static void mirrorP (double apx1, double apy1, double apx2, double apy2,
		     double x, double y, double *xret, double *yret)
/*----------------------------------------------------------------*/
{
  double dx, dy, a, si, co ;

  dx = apx2 - apx1 ;
  dy = apy2 - apy1 ;
  a = Atan2 (dy, dx) ;
  si = sin (a) ;
  co = cos (a) ;
  x -= apx1 ;
  y -= apy1 ;
  obru (si, co, x, y, &x, &y) ;
  obrd (si, co, x, -y, &x, &y) ;
  x += apx1 ;
  y += apy1 ;
  *xret = x ;
  *yret = y ;
}


static void mirrlinia(char  *adr,double x1, double y1,
			double x2, double y2)
/*-----------------------------------------------------------*/
{
  LINIA *l ;
  double xp, yp ;

  l = (LINIA*)adr ;
  mirrorP (x1, y1, x2 ,y2, l->x1, l->y1, &xp, &yp) ;
  l->x1 = xp ;
  l->y1 = yp ;
  mirrorP (x1, y1 ,x2, y2, l->x2, l->y2, &xp, &yp) ;
  l->x2 = xp ;
  l->y2 = yp ;
  if (linia_wybrana (l)) l->widoczny = 1 ;
  else l->widoczny = 0 ;
}

static void mirrvector(char  *adr,double x1, double y1,
                      double x2, double y2)
/*-----------------------------------------------------------*/
{
    AVECTOR *v ;
    double xp, yp ;

    v = (AVECTOR*)adr ;
    mirrorP (x1, y1, x2 ,y2, v->x1, v->y1, &xp, &yp) ;
    v->x1 = xp ;
    v->y1 = yp ;
    mirrorP (x1, y1 ,x2, y2, v->x2, v->y2, &xp, &yp) ;
    v->x2 = xp ;
    v->y2 = yp ;
    if (vector_wybrany (v)) v->widoczny = 1 ;
    else v->widoczny = 0 ;
}

static void mirrlinia3D(char  *adr,double x1, double y1,
			double x2, double y2)
/*-----------------------------------------------------------*/
{
  LINIA3D *l ;
  double xp, yp ;

  l = (LINIA3D*)adr ;
  mirrorP (x1, y1, x2 ,y2, l->x1, l->y1, &xp, &yp) ;
  l->x1 = xp ;
  l->y1 = yp ;
  mirrorP (x1, y1 ,x2, y2, l->x2, l->y2, &xp, &yp) ;
  l->x2 = xp ;
  l->y2 = yp ;
  if (linia_wybrana ((LINIA *)l)) l->widoczny = 1 ;
  else l->widoczny = 0 ;
}


static void mirrblock (char  *adr,double x1, double y1,double x2, double y2)
/*------------------------------------------------------------*/
{
  BLOK 			*ptrs_block ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  double 		xp, yp ;

  ptrs_block = (BLOK*)adr ;
  ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
  if (((ptrs_block->kod_obiektu == B_EXPORT) ||
	   (ptrs_block->kod_obiektu == B_HATCH) ||
       (ptrs_block->kod_obiektu == B_PPOMIAR) ||
       (ptrs_block->kod_obiektu == B_KPOMIAR) ||
       (ptrs_block->kod_obiektu == B_ODWIERT_OLD) ||
       (ptrs_block->kod_obiektu == B_ODWIERT) ||
       (ptrs_block->kod_obiektu == B_INSTALACJE) ||
       (ptrs_block->kod_obiektu == B_HEKTOMETRY) ||
       (ptrs_block->kod_obiektu == B_SEKTORY) ||
       (ptrs_block->kod_obiektu == B_WIRE) ||
       (ptrs_block->kod_obiektu == B_ARM) ||
       (ptrs_block->kod_obiektu == B_SIEC) ||
       (ptrs_block->kod_obiektu == B_VERTEX)
     )
      && ptrs_desc_bl->flags == EBF_IP)
  {
    mirrorP (x1, y1, x2, y2, ptrs_desc_bl->x, ptrs_desc_bl->y, &xp, &yp) ;
    ptrs_desc_bl->x = xp ;
    ptrs_desc_bl->y = yp ;
  }
}

static void mirrpoint (char  *adr,double x1, double y1,double x2, double y2)
/*------------------------------------------------------------*/
{
  T_Point *ptrs_point ;
  double  xp, yp ;

  ptrs_point = (T_Point*)adr ;
  mirrorP (x1, y1, x2, y2, ptrs_point->x, ptrs_point->y, &xp, &yp) ;
  ptrs_point->x = xp ;
  ptrs_point->y = yp ;
}



static void mirrtekst (char  *adr,double x1, double y1,double x2, double y2)
/*-----------------------------------------------------------*/
{
  TEXT *t;
  double tsin, tcos, tdl, xt1, yt1, xt2, yt2, xt3, yt3, xt4, yt4 ;
  double del_wys=1;

  t=(TEXT*)adr;
  if (t->czcionka == 13) del_wys=1.25;
    else if (t->czcionka == 14) del_wys=1.375;
  
  xt1 = t->x ;
  yt1 = t->y;

  tsin = sin (t->kat) ;
  tcos = cos (t->kat) ;
  tdl = Get_TextLen (t, NULL) ;
  
  switch (t->justowanie)  
     {
     case 0 : xt2 = xt1 + tdl * tcos ;   /*do lewej*/
              yt2 = yt1 - Ky * tdl * tsin ;
              xt3 = xt2 - (t->wysokosc  * del_wys) * tsin ;
              yt3 = yt2 - Ky * (t->wysokosc * del_wys) * tcos ;
              xt4 = xt1 - (t->wysokosc * del_wys) * tsin ;
              yt4 = yt1 - Ky * (t->wysokosc * del_wys) * tcos ;
              break;
     case 1 : xt2 = xt1 ;
              yt2 = yt1 ; 
              xt1 = xt2 - tdl * tcos ;   /*do prawej*/
              yt1 = yt2 + Ky *  tdl * tsin ;
              xt3 = xt2 - (t->wysokosc * del_wys) * tsin ;
              yt3 = yt2 - Ky * (t->wysokosc * del_wys) * tcos ;
              xt4 = xt1 - (t->wysokosc * del_wys) * tsin ;
              yt4 = yt1 - Ky * (t->wysokosc * del_wys) * tcos ;
              break; 
     case 2 : xt1 = xt1 - 0.5 * tdl * tcos;
              yt1 = yt1 + 0.5 * Ky * tdl * tsin;
              xt2 = xt1 + tdl * tcos ;   /*srodkowo*/
              yt2 = yt1 - Ky * tdl * tsin;
              xt3 = xt2 - (t->wysokosc * del_wys) * tsin ;
              yt3 = yt2 - Ky * (t->wysokosc * del_wys) * tcos ;
              xt4 = xt1 - (t->wysokosc * del_wys) * tsin ;
              yt4 = yt1 - Ky * (t->wysokosc * del_wys) * tcos ;
              break;
     case 3 : xt1 = xt1 - 0.5 * tdl * tcos + 0.5 * (t->wysokosc * del_wys) * tsin;
              yt1 = yt1 + 0.5 * Ky * tdl * tsin + 0.5 * (t->wysokosc * del_wys) * tcos;
              xt2 = xt1 + tdl * tcos ;   /*centralnie*/
              yt2 = yt1 - Ky * tdl * tsin;
              xt3 = xt2 - (t->wysokosc * del_wys) * tsin ;
              yt3 = yt2 - Ky * (t->wysokosc * del_wys) * tcos ;
              xt4 = xt1 - (t->wysokosc * del_wys) * tsin ;
              yt4 = yt1 - Ky * (t->wysokosc * del_wys) * tcos ;
              break;
     default :xt2 = xt1 + tdl * tcos ;   /*do lewej*/
              yt2 = yt1 - Ky * tdl * tsin ;
              xt3 = xt2 - (t->wysokosc * del_wys) * tsin ;
              yt3 = yt2 - Ky * (t->wysokosc * del_wys) * tcos ;
              xt4 = xt1 - (t->wysokosc * del_wys) * tsin ;
              yt4 = yt1 - Ky * (t->wysokosc * del_wys) * tcos ;
              break;
     }
     
  mirrorP (x1, y1, x2, y2, xt1, yt1, &xt1, &yt1) ;
  mirrorP (x1, y1, x2, y2, xt2, yt2, &xt2, &yt2) ;
  mirrorP (x1, y1, x2, y2, xt3, yt3, &xt3, &yt3) ;
  mirrorP (x1, y1, x2, y2, xt4, yt4, &xt4, &yt4) ; /*&xp, &yp) ;*/
  switch (t->justowanie)
  {
  case 0 : t->x = xt2 ;
           t->y = yt2 ;
           break;
  case 1 : t->x = xt1 ;
           t->y = yt1 ;
           break;
  case 2 : t->x = 0.5 * (xt1 + xt2) ;
           t->y = 0.5 * (yt1 + yt2) ;
           break;
  case 3 : t->x = 0.5 * (xt1 + xt3) ;
           t->y = 0.5 * (yt1 + yt3) ;
           break;                           
  default : break;         
  }
  if (FALSE == Check_if_Equal (tdl, 0))
  {
    t->kat =  Atan2 (yt2 - yt1, xt2 - xt1) + (Pi2/2);
    if (t->kat > Pi2) t->kat -= Pi2;
    if (t->kat < 0) t->kat += Pi2;


    if ((t->kat > (1.001*Pi2/4)) && (t->kat < (1.001*Pi2*0.75)))
     {
     t->kat -= (Pi2/2);
     if (t->kat > Pi2) t->kat -= Pi2;
     
     if (t->typ!=n_typ_pin_name)
     { 
      switch (t->justowanie)
      {
  case 0 : t->x = xt4 ;
           t->y = yt4 ;
           break;
  case 1 : t->x = xt3 ;
           t->y = yt3 ;
           break; 
  case 2 : t->x = 0.5 * (xt3 + xt4) ;
           t->y = 0.5 * (yt3 + yt4) ;
           break;
  case 3 : break;                           
  default : break;           
      }
    }
     else
      {
      if (t->justowanie==0)
       {
        t->justowanie=1;
        t->x = xt1 ;
        t->y = yt1 ;
       }
       else
        if (t->justowanie==1)
          {
           t->justowanie=0;
           t->x = xt2 ;
           t->y = yt2 ;
          }
      }
     }
      else 
       {
        if (t->typ==n_typ_pin_name)
        {
         if (t->justowanie==0)
         {
          t->justowanie=1;
          t->x = xt1 ;
          t->y = yt1 ;
         }
          else
           if (t->justowanie==1)
            {
             t->justowanie=0;
             t->x = xt2 ;
             t->y = yt2 ;
            }
        } 
       }  
       
  }
  t->widoczny = tekst_wybrany (t) ;
}
static void mirrluk (char  *adr, double x1, double y1, double x2, double y2)
/*-----------------------------------------------------------*/
{
  LUK *l;
  double xp,yp, ax1, ay1, ax2, ay2 ;

  l = (LUK*)adr ;
  Arc_To_Points (l, &ax1, &ay1, &ax2, &ay2) ;
  mirrorP (x1, y1, x2, y2, ax1, ay1, &ax1, &ay1) ;
  mirrorP (x1, y1, x2, y2, ax2, ay2, &ax2, &ay2) ;
  mirrorP (x1, y1 ,x2, y2, l->x, l->y, &xp,&yp) ;
  l->x = xp;
  l->y = yp;
  if (ax1 != ax2 || ay1 != ay2)
  {
      Points_To_Arc (l, ax2, ay2, ax1, ay1) ;
  }
  else
  {
    l->r = 0 ;
  }
  l->widoczny = luk_wybrany (l) ;
}

static void mirrsolidarc (char  *adr, double x1, double y1, double x2, double y2)
/*-----------------------------------------------------------------------------*/
{
    SOLIDARC *sa;
    double xp,yp, ax1, ay1, ax2, ay2 ;
    double width1;

    sa = (SOLIDARC*)adr ;
    SolidArc_To_Points (sa, &ax1, &ay1, &ax2, &ay2) ;
    mirrorP (x1, y1, x2, y2, ax1, ay1, &ax1, &ay1) ;
    mirrorP (x1, y1, x2, y2, ax2, ay2, &ax2, &ay2) ;
    mirrorP (x1, y1 ,x2, y2, sa->x, sa->y, &xp,&yp) ;
    sa->x = xp;
    sa->y = yp;
    if (ax1 != ax2 || ay1 != ay2)
    {
        Points_To_SolidArc (sa, ax2, ay2, ax1, ay1) ;
    }
    else
    {
        sa->r = 0 ;
    }

    width1=sa->width1;
    sa->width1=sa->width2;
    sa->width2=width1;
    sa->reversed=!(sa->reversed);
    sa->widoczny = solidarc_wybrany (sa) ;
}

static void mirrokrag(char  *adr,double x1, double y1,double x2, double y2)
/*-----------------------------------------------------------*/
{
  OKRAG *o;
  double xp,yp;

  o=(OKRAG*)adr;
  mirrorP (x1, y1 ,x2, y2, o->x, o->y, &xp, &yp) ;
  o->x = xp ;
  o->y = yp ;
  o->widoczny = okrag_wybrany (o) ;
}

static void mirrkolo (char  *adr,double x1, double y1,double x2, double y2)
/*-----------------------------------------------------------*/
{
  OKRAG *K ;
  double xp, yp ;

  K = (OKRAG*)adr ;
  mirrorP (x1, y1, x2, y2, K->x, K->y, &xp, &yp) ;
  K->x = xp ;
  K->y = yp ;
  K->widoczny = okrag_wybrany (K) ;
}


static void mirrellipse (char  *adr,double x1, double y1,double x2, double y2)
/*--------------------------------------------------------------------------*/
{
    ELLIPSE *e ;
    double xp, yp, ex1, ey1;
    double kos,koc;

    e = (ELLIPSE*)adr ;

    kos=sin(e->angle);
    koc=cos(e->angle);
    Rotate_Point(kos,koc,e->x, e->y, e->x+e->rx, e->y, &ex1, &ey1);
    mirrorP (x1, y1, x2, y2, ex1, ey1, &ex1, &ey1) ;

    mirrorP (x1, y1, x2, y2, e->x, e->y, &xp, &yp) ;
    e->x = xp ;
    e->y = yp ;

    e->angle = atan2(ey1 - e->y, ex1 - e->x);

    e->widoczny = elipsa_wybrana_prec(e) ;
}

static void mirrellipticalarc (char  *adr,double x1, double y1,double x2, double y2)
/*--------------------------------------------------------------------------------*/
{
    mirrellipse (adr,x1, y1,x2,y2);
}

static void mirrfilledellipse (char  *adr,double x1, double y1,double x2, double y2)
/*---------------------------------------------------------------------------------*/
{
    mirrellipse (adr,x1, y1,x2,y2);

}


static void mirrspline(char  *adr, double x1, double y1, double x2, double y2)
/*--------------------------------------------------------------------------*/
{
	SPLINE *s;
	int i;
	double xp, yp;

	s = (SPLINE*)adr;
	for (i = 0; i < (int)s->lp; i += 2)
	{
		mirrorP(x1, y1, x2, y2, s->xy[i], s->xy[i + 1], &xp, &yp);
		s->xy[i] = xp; s->xy[i + 1] = yp;
	}
	s->widoczny = spline_wybrany(s);
}

static void mirrwwielokat (char  *adr,double x1, double y1,double x2, double y2)
/*-----------------------------------------------------------*/
{
  WIELOKAT *w ;
  int i ;
  double xp, yp ;

  w = (WIELOKAT*)adr ;
  for (i = 0 ; i < (int)w->lp ; i += 2)
  {
    mirrorP (x1, y1, x2, y2, w->xy[i], w->xy[i+1], &xp, &yp) ;
    w->xy [i] = xp; w->xy [i+1] = yp ;
  }
  w->widoczny = wielokat_wybrany (w) ;
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/


static void nooop(void)
{}
#define noop (void (near*)(char  *adr,double dx,double dy))nooop
#define noop1 (void (near*)(char  *adr,double dx,double dy,double k))nooop
#define noop2 (void (near*)(char  *adr,double dx,double dy,double k1,double k2))nooop
#define noop3 (void (near*)(char  *adr,double dx,double dy,double dz,double k1,double k2))nooop

static void (*moveB[16])(char  *adr,double dx,double dy)=
	      {zwpcx, zwlinia,zwtekst,zwluk,zwokrag,zwkolo,zwwwielokat,
	       zwpoint, zwblock,zwsolidarc,zwellipticalarc,zwellipse,zwfilledellipse,zwspline,zwvector,noop };

static void (*moveBZ[16])(char  *adr,double dx,double dy)=
         {noop,noop,noop,noop,noop,noop,
           noop,noop,noop /*zwlinia3DZ*/,noop,noop,noop,noop,noop /*zwwwielokat3DZ*/,noop,noop, };

static void (*skalaB[16])(char  *adr,double dx,double dy,double k1,double k2)=
	      {skpcx,sklinia,sktekst,skluk,skokrag,skkolo,skwwielokat,
       skpoint,skblock,sksolidarc,skellipticalarc, skellipse, skfilledellipse,skspline,skvector,noop2, };

static void (*obrotB[16])(char  *adr,double dx,double dy,double k1,double k2)=
	      {obpcx,oblinia,obtekst,obluk,obokrag,obkolo,obwwielokat,
       obpoint,obblock,obsolidarc,obellipticalarc, obellipse, obfilledellipse,obspline,obvector,noop2, };

static void (*obrotB_PCX[16])(char  *adr,double dx,double dy,double k1,double k2)=
	      {obpcx,noop2,noop2,noop2,noop2,noop2,obwwielokatPCX,
       noop2,noop2,noop2,noop2,noop2,noop2,noop2,noop2,noop2, };

static void (*obrotBXZ[16])(char  *adr,double dx,double dy,double dz,double k1,double k2)=
	      {noop3,noop3,noop3,noop3,noop3,noop3,
       noop3,noop3,noop3 /*oblinia3DXZ*/,noop3,noop3,noop3,noop3,noop3 /*obwwielokat3DXZ*/,noop3,noop3, };

static void (*obrotBYZ[16])(char  *adr,double dx,double dy,double dz,double k1,double k2)=
	      {noop3,noop3,noop3,noop3,noop3,noop3,
       noop3,noop3,noop3 /*oblinia3DYZ*/,noop3,noop3,noop3,noop3,noop3 /*obwwielokat3DYZ*/,noop3,noop3, };

static void (*mirrorB[16])(char  *adr,double x1,double y1,double x2,double y2)=
	      {mirrlinia,mirrtekst,mirrluk,mirrokrag,mirrkolo,mirrwwielokat,
       mirrpoint,mirrblock,mirrsolidarc,mirrellipse,mirrellipse,mirrellipse,mirrspline,mirrvector,noop2,noop2, };


/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/
double get_angle_sin_cos(double k1, double k2)
{
	double kat;
	int Q;

	if ((k1 == 0.0) && (k2 == 1.0)) kat = 0.0;
	else if ((k1 == 1.0) && (k2 == 0.0)) kat = Pi_ * 0.5;
	else if ((k1 == 0.0) && (k2 == -1.0)) kat = Pi_;
	else if ((k1 == -1.0) && (k2 == 0.0)) kat = Pi_ * 1.5;
	else
	{
		if ((k1 > 0) && (k2 > 0)) Q = 1;
		else if ((k1 > 0) && (k2 < 0)) Q = 2;
		else if ((k1 < 0) && (k2 < 0)) Q = 3;
		else Q = 4;

		kat = asin(k1);
		if (kat < 0) kat = kat + Pi2;

		if (Q == 1)
		{
			if (kat > (Pi_ * 0.5)) kat = -(kat - Pi_);  //e.g. kat=170 -> kat=-(170-180)=10
		}
		else if (Q == 2)
		{
			if (kat < (Pi_ * 0.5)) kat = -(kat - Pi_);  //e.g. kat=10  ->  kat=-(10-180)=170
		}
		else if (Q == 3)
		{
			if (kat > (Pi_ * 1.5)) kat = -(kat - Pi2 * 1.5);  //e.g. kat=350  -> kat=-(350-540)=190
		}
		else if (Q == 4)
		{
			if (kat < (Pi_ * 1.5)) kat = -(kat - Pi2 * 1.5); //e.g. kat=190  ->  kat=-(190-540)=350
		}

		if (kat<0) kat += Pi2;
		else if (kat > Pi2) kat -= Pi2;
	}
	return kat;
}


void transformacja_blok2 (char  *adr,char  *adrk,double x,double y,double k1,double k2,
		double xa, double ya, double ka1, double ka2, int trans, double z)
{
 NAGLOWEK *nag;
 BLOK  *b;
 double kat0;
 
   switch (trans)
   {
   case Tprzesuw: UNDO_REC.x = -x;
                  UNDO_REC.y = -y;
                  UNDO_REC.k1 = k1;
                  UNDO_REC.k2 = k2;
   break;
   case TprzesuwZ:UNDO_REC.x = -x;
                  UNDO_REC.y = -y;
                  UNDO_REC.k1 = k1;
                  UNDO_REC.k2 = k2;
   break;
   case Tobrot  :
   case TobrotPCX :
                  UNDO_REC.x = x;
                  UNDO_REC.y = y;
				  kat0 = get_angle_sin_cos(k1, k2);
                  UNDO_REC.k1 = sin (Pi2 - kat0);
                  UNDO_REC.k2 = cos (Pi2 - kat0);
   break;
   case TobrotXZ: UNDO_REC.x = x;
                  UNDO_REC.y = y;
				  kat0 =get_angle_sin_cos(k1, k2);
                  UNDO_REC.k1 = sin (Pi2 - kat0);
                  UNDO_REC.k2 = cos (Pi2 - kat0);
   break;
   case TobrotYZ: UNDO_REC.x = x;
                  UNDO_REC.y = y;
				  kat0 = get_angle_sin_cos(k1, k2);
                  UNDO_REC.k1 = sin (Pi2 - kat0);
                  UNDO_REC.k2 = cos (Pi2 - kat0);
   break; 
   case Tskala  : UNDO_REC.x = x;
                  UNDO_REC.y = y;
                  if (k1!=0) UNDO_REC.k1 = 1/k1; else UNDO_REC.k1=1;
                  if (k2!=0) UNDO_REC.k2 = 1/k2; else UNDO_REC.k2=1;
   break;   
   case Tmirror : UNDO_REC.x = x;
                  UNDO_REC.y = y;
                  UNDO_REC.k1 = k1;
                  UNDO_REC.k2 = k2;
   break;              
   default      : UNDO_REC.x = x;
                  UNDO_REC.y = y;
                  UNDO_REC.k1 = k1;
                  UNDO_REC.k2 = k2;
   break;                  
   }
   UNDO_REC.xa = xa;
   UNDO_REC.ya = ya;
   UNDO_REC.ka1 = ka1;
   UNDO_REC.ka2 = ka2;
   UNDO_REC.trans = trans;
 
 if (adr == NULL || adrk == NULL)
 {
   return;
 }
 nag=(NAGLOWEK*)adr;

 oknoS(Xp,Yp,Xk,Yk);
 while (nag->obiekt!=Okoniec && adr<=adrk)
  {

     if (nag->atrybut==Ablok)
    {
      transformacja_obiekt ((void*)adr,x,y,k1,k2, xa, ya, ka1, ka2, trans, 0) ;
    }
    if (nag->obiekt==OdBLOK)
     { b=(BLOK *)adr;
       adr+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
     }
    else adr+=nag->n+sizeof(NAGLOWEK);
    nag=(NAGLOWEK*)adr;
  }
}

void transformacja_blok (char  *adr,char  *adrk,double x,double y,double k1,double k2,int trans, double z)
{
    BOOL ret;
   transformacja_blok2 (adr, adrk, x, y, k1, k2, 0,0,0,0, trans, z) ;
}


static void rysuj_linia_(LINIA *ad,int mode,int kolor)
{
  int grubosc, grubosc1 ;
  NAGLOWEK *nag1, *nag2;
  TEXT *t;
  QUAD t_outline;
  int ret;
  LINIA L_tmp, L_tmp1;

  if ((ad->typ == HATCH_OUTLINE_TYPE) && (Get_Point_View()==FALSE)) return;

  if (Layers [ad->warstwa].on == 0)
  {
    return ;
  }

  grubosc=linestyle(ad->typ);

    int typl = (ad->typ & 31);
    if (typl==0) set_pattern_offset(0);
  
  if ((mode==XOR_PUT) || ((draw_hatch_pattern==TRUE) && (ad->atrybut==Aoblok)))
   { setcolor(kolory.blok);
   }
   else /* if((mode==COPY_PUT) */
    { if(!kolor) setcolor(kolory.paper);
      else
       {
        if ((ad->warstwa==Current_Layer) || (options1.view_only_current_layer==0)) SetColorAC(ad->kolor);
          else SetColorAC(8);
       }
    }
 
   
   if (draw_hatch_pattern==TRUE)
    {
      SetColorAC(LiniaG.kolor);
      grubosc1 = (LiniaG.typ & 224) / 32;
      grubosc = linestyle(LiniaG.typ);
      LiniaG.typ = grubosc1 * 32;
      if (grubosc1<2)
       {
         setlinestyle1(DOTTED_LINE,0,1);
       }
        else setlinestyle2(SOLID_LINE,0,grubosc);
    }


    //checking if line is dimensioning line
    if ((ad->blok == 1) && (ad->obiektt1 == 0) && (ad->obiektt2 == 1) && (ad->obiektt3 == 0) && (ad>dane) && (ad<(dane+dane_size)))
    {
        //check if next object is text or line then text
        nag1 = (char*)ad + sizeof(NAGLOWEK) + ad->n;
        if (nag1->obiekt == Otekst)
        {
            t = (TEXT*)nag1;
            if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
            {

                outlinetext(t, &t_outline, 0.5);
                Normalize_Quad (&t_outline);
                //for (i=0; i<4; i++)
                //    ret=lineC(pikseleX0(t_outline.xy[i*2]), pikseleY0(t_outline.xy[i*2+1]), pikseleX0(t_outline.xy[(i*2+2)%8]), pikseleY0(t_outline.xy[(i*2+3)%8]));
                ret=trim_line_to_quad(ad, &t_outline, &L_tmp, &L_tmp1);
                switch (ret)
                {
                    case 0:
                        ad->widoczny = lineC(pikseleX0(ad->x1), pikseleY0(ad->y1), pikseleX0(ad->x2),pikseleY0(ad->y2));
                        break;
                    case 1: ad->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                        break;
                    case 2: ad->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                        ad->widoczny = lineC(pikseleX0(L_tmp1.x1), pikseleY0(L_tmp1.y1), pikseleX0(L_tmp1.x2),pikseleY0(L_tmp1.y2));
                        break;
                    default:
                        break;
                }
                //L->widoczny = lineC(pikseleX0(L->x1), pikseleY0(L->y1), pikseleX0(L->x2), pikseleY0(L->y2));
            }
            else ad->widoczny = lineC(pikseleX0(ad->x1), pikseleY0(ad->y1), pikseleX0(ad->x2), pikseleY0(ad->y2));
        }

        else if (nag1->obiekt == Olinia)
        {
            nag2 = (char*)nag1 + sizeof(NAGLOWEK) + nag1->n;
            if (nag2->obiekt == Otekst)
            {
                t = (TEXT*)nag2;
                if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
                {
                    //checking crossing with text
                    outlinetext(t, &t_outline, 0.5);
                    Normalize_Quad (&t_outline);
                    //for (i=0; i<4; i++)
                    //    ret=lineC(pikseleX0(t_outline.xy[i*2]), pikseleY0(t_outline.xy[i*2+1]), pikseleX0(t_outline.xy[(i*2+2)%8]), pikseleY0(t_outline.xy[(i*2+3)%8]));
                    ret=trim_line_to_quad(ad, &t_outline, &L_tmp, &L_tmp1);
                    switch (ret)
                    {
                        case 0:
                            ad->widoczny = lineC(pikseleX0(ad->x1), pikseleY0(ad->y1), pikseleX0(ad->x2),pikseleY0(ad->y2));
                            break;
                        case 1: ad->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                            break;
                        case 2: ad->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                            ad->widoczny = lineC(pikseleX0(L_tmp1.x1), pikseleY0(L_tmp1.y1), pikseleX0(L_tmp1.x2),pikseleY0(L_tmp1.y2));
                            break;
                        default:
                            break;
                    }
                }
                else ad->widoczny = lineC(pikseleX0(ad->x1), pikseleY0(ad->y1), pikseleX0(ad->x2), pikseleY0(ad->y2));
            }
            else ad->widoczny = lineC(pikseleX0(ad->x1), pikseleY0(ad->y1), pikseleX0(ad->x2), pikseleY0(ad->y2));
        }

    }
   else
      lineC (pikseleX0(ad->x1),pikseleY0(ad->y1),pikseleX0(ad->x2),pikseleY0(ad->y2));
   
  if(mvcurb.akton) mvcurb.L++;
  if(mvcurb.aktoff) mvcurb.L--;
}

extern void outtextxy_w_(TEXT  *t,int mode);
extern void outtextxy_wP_(TEXT  *t,int mode);

static void rysuj_tekst_(TEXT *ad,int mode,int kolor)
{
  if (Layers [ad->warstwa].on == 0)
  {
    return ;
  }
  if(mode==COPY_PUT)
   { if(!kolor) { setcolor(kolory.paper); outtextxy_w_(ad,COPY_PUT); }
     else {
         outtextxy_w(ad,COPY_PUT);
         }
   }
  else
   { setcolor(kolory.blok);
     outtextxy_w(ad,XOR_PUT);
   }
  if(mvcurb.akton) mvcurb.T++;
  if(mvcurb.aktoff) mvcurb.T--;
}

static void rysuj_ellipse_(ELLIPSE *ad,int mode,int kolor)
{
  int grubosc;
  int kolor1;

  if (Layers [ad->warstwa].on == 0)
  {
    return ;
  }
   grubosc=linestyle(ad->typ);
  if(mode==COPY_PUT) {
          if (!kolor)
          {
              kolor1=kolory.paper;
              setcolor(kolor1);
          }
          else {
              if ((ad->warstwa == Current_Layer) || (options1.view_only_current_layer == 0)) kolor1=ad->kolor;
              else kolor1=8;
              SetColorAC(kolor1);
          }
  }
  else
   { kolor1=kolory.blok;
       setcolor(kolor1);
   }

    set_pattern_count(TRUE);

    DrawEllipse(ad, COPY_PUT, kolor1, kolor);

    set_pattern_count(FALSE);
    set_pattern_offset(0);

  if(mvcurb.akton) mvcurb.E++;
  if(mvcurb.aktoff) mvcurb.E--;
}

static void rysuj_ellipticalarc_(ELLIPTICALARC *ad,int mode,int kolor)
{
    int grubosc;
    int kolor1;

    if (Layers [ad->warstwa].on == 0)
    {
        return ;
    }
    grubosc=linestyle(ad->typ);

    if(mode==COPY_PUT) {
        if (!kolor)
        {
            kolor1=kolory.paper;
            setcolor(kolor1);
        }
        else {
            if ((ad->warstwa == Current_Layer) || (options1.view_only_current_layer == 0)) kolor1=ad->kolor;
            else kolor1=8;
            SetColorAC(kolor1);
        }
    }
    else
    { kolor1=kolory.blok;
        setcolor(kolor1);

    }

    set_pattern_count(TRUE);

    DrawEllipticalArc(ad, COPY_PUT, kolor1, kolor);

    set_pattern_count(FALSE);
    set_pattern_offset(0);

    if(mvcurb.akton) mvcurb.EA++;
    if(mvcurb.aktoff) mvcurb.EA--;
}

int rysuj_solidarc_factory___(SOLIDARC *sa, LINIA *L_left, LINIA *L_right, LUK *l_inner, LUK *l_outer, double *xy)
{  int n;
    n=0;

    xy[n++] = L_right->x1;
    xy[n++] = L_right->y1;
    xy[n++] = L_right->x2;
    xy[n++] = L_right->y2;

    n = get_arc_points(l_inner, xy, n, FALSE, FALSE);

    xy[n++] = L_left->x1;
    xy[n++] = L_left->y1;
    xy[n++] = L_left->x2;
    xy[n++] = L_left->y2;

    n = get_arc_points(l_outer, xy, n, TRUE, FALSE);

    return n;

}

int rysuj_solidarc_factory(SOLIDARC *sa, int mode, int kolor, int side, int in, double *xy, BOOL printer)
{   int n;

    if (Layers[sa->warstwa].on == 0) {
        return in;
    }

    POINTD p1l, p2l, p3l, p4l, p5l, p6l, p7l, p8l, p9l, p10l;
    double midkat;
    double halfwidth1, halfwidth2, halfmidwidth;
    double axis1, axis2, midaxis;
    double kos, koc;
    LUK louter = ldef;
    LUK linner = ldef;
    int retl;
    double kat1, kat2;

    n=in;

    axis1 = sa->axis1;
    axis2 = sa->axis2;
    halfwidth1 = sa->width1 / 2;
    halfwidth2 = sa->width2 / 2;

    halfmidwidth = (halfwidth1 + halfwidth2) / 2;
    midaxis=(axis1 + axis2) / 2;

    kat1 = Angle_Normal(sa->kat1);
    kat2 = Angle_Normal(sa->kat2);

    if (kat2 < kat1) kat2 += Pi2;

    midkat = (kat2 + kat1) / 2;

    //finding points 1 and 2 (base arc)
    p1l.x = sa->x + sa->r * cos(kat2);
    p1l.y = sa->y + sa->r * sin(kat2);
    p2l.x = sa->x + sa->r * cos(kat1);
    p2l.y = sa->y + sa->r * sin(kat1);

    //finding points 7 and 10
    koc = cos(kat1);
    kos = sin(kat1);
    p7l.x = sa->x + (sa->r - halfwidth2 - axis2) * koc;
    p7l.y = sa->y + (sa->r - halfwidth2 - axis2) * kos;
    p10l.x = sa->x + (sa->r + halfwidth2 - axis2) * koc;
    p10l.y = sa->y + (sa->r + halfwidth2 - axis2) * kos;

    //finding points 5 and 8
    koc = cos(kat2);
    kos = sin(kat2);
    p5l.x = sa->x + (sa->r - halfwidth1 - axis1) * koc;
    p5l.y = sa->y + (sa->r - halfwidth1 - axis1) * kos;
    p8l.x = sa->x + (sa->r + halfwidth1 - axis1) * koc;
    p8l.y = sa->y + (sa->r + halfwidth1 - axis1) * kos;

    //finding points 6 and 9 (base arc)
    koc = cos(midkat);
    kos = sin(midkat);
    p6l.x = sa->x + (sa->r - halfmidwidth - midaxis) * koc;
    p6l.y = sa->y + (sa->r - halfmidwidth - midaxis) * kos;
    p9l.x = sa->x + (sa->r + halfmidwidth - midaxis) * koc;
    p9l.y = sa->y + (sa->r + halfmidwidth - midaxis) * kos;

    retl = get_3p_arc(&linner, &p5l, &p6l, &p7l);
    retl = get_3p_arc(&louter, &p8l, &p9l, &p10l);

    if (side==0)
    {
        xy[n++] = p10l.x;
        xy[n++] = p10l.y;
        xy[n++] = p7l.x;
        xy[n++] = p7l.y;

        n = get_arc_points(&linner, xy, n, FALSE, printer);

        xy[n++] = p5l.x;
        xy[n++] = p5l.y;
        xy[n++] = p8l.x;
        xy[n++] = p8l.y;

        n = get_arc_points(&louter, xy, n, TRUE, printer);
    }
    else if (side==1)
    {
        n = get_arc_points(&louter, xy, n, TRUE, printer);

        xy[n++] = p10l.x;
        xy[n++] = p10l.y;
        xy[n++] = p7l.x;
        xy[n++] = p7l.y;

        n = get_arc_points(&linner, xy, n, FALSE, printer);
    }
    else if (side==2)
    {
        n = get_arc_points(&linner, xy, n, FALSE, printer);

        xy[n++] = p5l.x;
        xy[n++] = p5l.y;
        xy[n++] = p8l.x;
        xy[n++] = p8l.y;

        n = get_arc_points(&louter, xy, n, TRUE, printer);
    }

    return n;
}

void rysuj_solidarc_exe(SOLIDARC *sa, int mode, int kolor, int n, double *xy, BOOL as_redraw)
{   int i;
    int polypoints[264];
    double xs, ys;
    unsigned char translucency;
    int grubosc;
    int PolyPointsMon[8];
    SOLID_PATTERN solid_pattern = {2, 0, 0, 0, 0, ""};
    int nside1, nside2;

        for (i = 0; i < n; i += 2)
        {
            polypoints[i] = pikseleX0(xy[i]);
            polypoints[i + 1] = pikseleY0(xy[i + 1]);
        }
        //////////////////////////////
        if(!(sa->widoczny) && (sa->pattern == 0))  //is not crossing and no pattern
        {
            translucency = 255;

            if ((sa->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
            {
                if (((sa->atrybut==Ablok) && (as_redraw)) ||
                    ((dragging_quad_move == TRUE) && (sa->atrybut == Aoblok)))
                {
                    setfillstyle_(SOLID_FILL,kolory.blok);
                    setcolor(kolory.blok);
                } else {
                    setfillstyle_(SOLID_FILL, GetColorAC(sa->kolor));
                    SetColorAC(sa->kolor);
                }
            }
            else setfillstyle_ (SOLID_FILL, GetColorAC (8)) ;

            if ((sa->translucent == 1) || (TRANSLUCENCY<255))
            {
                if (sa->translucent == 1)
                {
                    translucency=sa->translucency;
                }
                else translucency = 255;
                if (translucency > TRANSLUCENCY) translucency = TRANSLUCENCY;
                set_trans_blender(0, 0, 0, (int)translucency);
                set_mode_trans();
            }
            else set_mode_solid();

            PolyPointsMon[0] = pXp; PolyPointsMon[1] = /*pYk*/ 0; PolyPointsMon[2] = pXk; PolyPointsMon[3] = /*pYk*/ 0;
            PolyPointsMon[4] = pXk; PolyPointsMon[5] = pYp; PolyPointsMon[6] = pXp; PolyPointsMon[7] = pYp;
            my_fillpoly(4, PolyPointsMon, translucency, kolory.paper);

            if (sa->translucent == 1)
            {
                reset_trans_blender();
            }
            set_mode_solid();
            return;
        }

        ///////////////////////////////
        translucency = 255;


        if ((sa->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
        {
            if (((sa->atrybut==Ablok) && (as_redraw)) ||
                ((dragging_quad_move == TRUE) && (sa->atrybut == Aoblok)))
            {
                setcolor(kolory.blok);
                setfillstyle_(SOLID_FILL,kolory.blok);
            }
            else
            {
                SetColorAC(sa->kolor);
                setfillstyle_(SOLID_FILL,GetColorAC(sa->kolor));
            }
        }
        else
        {
            SetColorAC(8);
            setfillstyle_(SOLID_FILL,GetColorAC(8));
        }

        if (mode == COPY_PUT)
        {
            if (sa->empty_typ == 0)
            {

                setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);

                if(!kolor)
                {
                    setfillstyle_(SOLID_FILL,kolory.paper);
                    setcolor(kolory.paper);

                    set_mode_solid();

                    my_fillpoly(n / 2, polypoints, translucency, kolory.paper);
                }
                else
                {
                    if ((sa->translucent == 1) || (TRANSLUCENCY < 255)) {
                        if (sa->translucent == 1) {
                            translucency = sa->translucency;

                        } else translucency = 255;

                        if (translucency > TRANSLUCENCY) translucency = TRANSLUCENCY;
                        set_trans_blender(0, 0, 0, (int) translucency);
                        set_mode_trans();
                    } else set_mode_solid();

                    if (sa->pattern == 0) {
                        my_fillpoly(n / 2, polypoints, translucency, kolory.paper);
                    }
                    else  //pattern
                    {

                        if ((sa->warstwa == Current_Layer) || (options1.view_only_current_layer == 0))
                            SetColorAC(sa->kolor);
                        else SetColorAC(8);

                        setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
                        if ((sa->warstwa == Current_Layer) || (options1.view_only_current_layer == 0)) {
                            solid_pattern.flag = 0;
                            setfillstyle_(SOLID_FILL, GetColorAC(sa->kolor));
                        } else {
                            solid_pattern.flag = 1;  //hide
                            setfillstyle_(SOLID_FILL, GetColorAC(8));
                        }
                        set_mode_trans();

                        solid_pattern.scale = sa->scale;
                        solid_pattern.dx = sa->dx;
                        solid_pattern.angle = sa->angle;
                        solid_pattern.dy = sa->dy;

                        strcpy(&solid_pattern.pattern, &sa->patternname);

                        if (!sa->widoczny) //filling screen only
                        {
                            polypoints[0] = pXp;
                            polypoints[1] = /*pYk*/ 0;
                            polypoints[2] = pXk;
                            polypoints[3] = /*pYk*/ 0;
                            polypoints[4] = pXk;
                            polypoints[5] = pYp;
                            polypoints[6] = pXp;
                            polypoints[7] = pYp;
                            n = 8;
                        }

                        fillpolypattern(n / 2, polypoints, &solid_pattern, pikseleX0(0), pikseleY0(0));

                        if (((sa->atrybut == Ablok) && (as_redraw)) ||
                            ((dragging_quad_move == TRUE) && (sa->atrybut == Aoblok))) {
                            setcolor(kolory.blok);
                            setfillstyle_(SOLID_FILL, kolory.blok);

                            set_trans_blender(0, 0, 0, (int) HALFTRANSLUCENCY);
                            set_mode_trans();

                            my_fillpoly(n / 2, polypoints, HALFTRANSLUCENCY, kolory.paper);

                            reset_trans_blender();
                            set_mode_solid();
                        }

                    }
                    if (sa->translucent == 1) {
                        reset_trans_blender();
                        set_mode_solid();
                    }
                }

            }

            else if (sa->empty_typ < 6)
            {
                if(!kolor) setcolor(kolory.paper);
                else
                {
                    if ((sa->warstwa==Current_Layer) || (options1.view_only_current_layer==0)) SetColorAC(sa->kolor);
                    else SetColorAC(8);
                }
                grubosc = linestyle_solid(sa->empty_typ);
                set_mode_solid();
                for (i = 2; i < n; i += 2) {

                        lineC((long) polypoints[i - 2], (long) polypoints[i - 1], (long) polypoints[i],
                              (long) polypoints[i + 1]);
                }

                if (i > 2)
                    lineC((long) polypoints[i - 2], (long) polypoints[i - 1], (long) polypoints[0],
                          (long) polypoints[1]);
            }


        }
        else //XOR_PUT
        {
            setcolor(kolory.blok);
            setfillstyle_(MY_DOT_FILL,kolory.blok);
            if (sa->empty_typ==0)
            {
                translucency = TRANSLUCENCY;

                if ((sa->translucent == 1) || (TRANSLUCENCY < 255))
                {
                    if (sa->translucent == 1)
                    {
                        translucency=sa->translucency;
                    }
                    else translucency = 255;
                    if (translucency > TRANSLUCENCY) translucency = TRANSLUCENCY;
                    set_trans_blender(0, 0, 0, translucency);
                    set_mode_trans();
                }
                my_fillpoly(n / 2, polypoints, translucency, kolory.paper);
            }
            else if (sa->empty_typ < 6) {
                grubosc = linestyle_solid(sa->empty_typ);
                set_mode_solid();
                for (i = 2; i < n; i += 2) {

                        lineC((long) polypoints[i - 2], (long) polypoints[i - 1], (long) polypoints[i],
                              (long) polypoints[i + 1]);
                }
                if (i > 2)
                    lineC((long) polypoints[i - 2], (long) polypoints[i - 1], (long) polypoints[0],
                          (long) polypoints[1]);
            }
        }
        set_mode_solid();
}

void rysuj_solidarc_(SOLIDARC *sa, int mode, int kolor, BOOL as_redraw, BOOL enforce)
{
    SOLIDARC sa1, sa2;
    double midkat;
    double midwidth;
    double kat1, kat2, dkat;
    RECTD rectd;
    double xy[264];
    int in;
    double xs, ys;


        //saving selection rectangle
        Get_oknoS(&rectd);
        //setting view rectangle
        oknoS(Xp, Yp, Xk, Yk);  //TYMCZASOWO for solidarc_wybrany()

        if(!(sa->widoczny=(solidarc_wybrany(sa) | enforce)))
        {
            xs = (Xp + Xk) / 2 ;
            ys = (Yp + Yk) / 2 ;
            if (FALSE == Point_in_SolidArc (sa, xs, ys)) return;
        }


        kat1 = Angle_Normal(sa->kat1);
        kat2 = Angle_Normal(sa->kat2);

        if (kat2 < kat1) kat2 += Pi2;

        dkat = kat2 - kat1;
        if (Check_if_LE(dkat, Pi_)) in=rysuj_solidarc_factory(sa, mode, kolor, 0, 0, xy, FALSE);
        else {
            memmove(&sa1, sa, sizeof(SOLIDARC));
            memmove(&sa2, sa, sizeof(SOLIDARC));

            midwidth = (sa->width1 + sa->width2) / 2;

            midkat = (kat2 + kat1) / 2;

            sa1.kat1 = kat1;
            sa1.kat2 = midkat;

            sa2.kat1 = midkat;
            sa2.kat2 =  kat2;

            if (sa1.reversed) {
                sa1.width2 = sa->width2;
                sa1.width1 = midwidth;
                sa2.width2 = midwidth;
                sa2.width1 = sa->width1;
            }
            else {
                sa1.width1 = midwidth; //sa->width1;
                sa1.width2 = sa->width2; //midwidth;
                sa2.width1 = sa->width1; //midwidth;
                sa2.width2 = midwidth; //sa->width2;
            }


            in = rysuj_solidarc_factory(&sa1, mode, kolor, 1, 0, xy, FALSE);
            in = rysuj_solidarc_factory(&sa2, mode, kolor, 2, in, xy, FALSE);
        }

        rysuj_solidarc_exe(sa, mode, kolor, in, xy, as_redraw);

        //restoring selection rectangle
        oknoS(rectd.xp, rectd.yp, rectd.xk, rectd.yk);
}

void rysuj_solidarc___(SOLIDARC *sa, int mode, int kolor, BOOL as_redraw, BOOL enforce)
{
    LINIA L_left=Ldef, L_right=Ldef;
    LUK l_outer1 = ldef, l_outer2 = ldef, l_outer = ldef, l_inner1 = ldef, l_inner2 = ldef, l_inner = ldef;
    int ln;
    int in;
    double xy[264];
    RECTD rectd;
    double xs, ys;

    //id sa->widoczny is 1, solidarc edge is crossing screen; if 0, solidarc overlaps screen

    //saving selection rectangle
    Get_oknoS(&rectd);
    //setting view rectangle
    oknoS(Xp, Yp, Xk, Yk);  //TYMCZASOWO for solidarc_wybrany()

    if(!(sa->widoczny=(solidarc_wybrany(sa) | enforce)))
    {
        xs = (Xp + Xk) / 2 ;
        ys = (Yp + Yk) / 2 ;
        if (FALSE == Point_in_SolidArc (sa, xs, ys)) return;
    }

    ln = solidarc_elements(sa, &L_left, &L_right, &l_inner1, &l_inner2, &l_outer1, &l_outer2);
    //arcs consolidation
    memmove(&l_inner, &l_inner1, sizeof (LUK));
    l_inner.kat2=l_inner2.kat2;
    memmove(&l_outer, &l_outer1, sizeof (LUK));
    l_outer.kat2=l_outer2.kat2;
    in=rysuj_solidarc_factory___(sa, &L_left, &L_right, &l_inner, &l_outer, xy);

    rysuj_solidarc_exe(sa, mode, kolor, in, xy, as_redraw);

    //restoring selection rectangle
    oknoS(rectd.xp, rectd.yp, rectd.xk, rectd.yk);

}

static void rysuj_filledellipse_(ELLIPSE *ad,int mode,int kolor)
{
    int grubosc;
    int kolor1;

    if (Layers [ad->warstwa].on == 0)
    {
        return ;
    }

    if(mode==COPY_PUT) {
        if (!kolor) {
            setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
            setfillstyle_(SOLID_FILL, kolory.paper);
            kolor1=kolory.paper;
            setcolor(kolor1);
            set_mode_solid();
        } else {
            setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
            if ((ad->warstwa == Current_Layer) || (options1.view_only_current_layer == 0))
            {
                setfillstyle_(SOLID_FILL, GetColorAC(ad->kolor));
                kolor1=ad->kolor;
            }
            else
            {
                setfillstyle_(SOLID_FILL, GetColorAC(8));
                kolor1=8;
            }
            SetColorAC(kolor1);
            set_mode_trans();
        }
    }else {
        kolor1=kolory.blok;
        setcolor(kolor1);
        setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
        setfillstyle_(SOLID_FILL, kolory.blok);

        set_mode_trans();
    }

    set_pattern_count(TRUE);
    DrawFilledEllipse(ad, COPY_PUT, kolor1, kolor);

    set_pattern_count(FALSE);
    set_pattern_offset(0);

    if(mvcurb.akton) mvcurb.FE++;
    if(mvcurb.aktoff) mvcurb.FE--;
}


static void rysuj_okrag_(OKRAG *ad,int mode,int kolor)
{
    int grubosc;

    if (Layers [ad->warstwa].on == 0)
    {
        return ;
    }
    grubosc=linestyle(ad->typ);

    if(mode==COPY_PUT) {
        if (draw_hatch_pattern == TRUE) {
            SetColorAC(LiniaG.kolor);
        }
        else
        {
            if (!kolor) setcolor(kolory.paper);
            else {
                if ((ad->warstwa == Current_Layer) || (options1.view_only_current_layer == 0)) SetColorAC(ad->kolor);
                else SetColorAC(8);
            }
        }
    }
    else
    { setcolor(kolory.blok);
    }

    set_pattern_count(TRUE);
    DrawCircle(pikseleX0(ad->x),pikseleY0(ad->y),pikseleDX(ad->r)/*R*/, COPY_PUT); //mode
    set_pattern_count(FALSE);
    set_pattern_offset(0);
    if(mvcurb.akton) mvcurb.O++;
    if(mvcurb.aktoff) mvcurb.O--;
}


void rysuj_punkt_ (T_Point *ad,int mode,int kolor)
/*----------------------------------------------------------*/
{ 
  if (Layers [ad->warstwa].on == 0)
  {
    return ;
  }
  if(mode==COPY_PUT)
    {
      if(!kolor)
      {
          setfillstyle_(SOLID_FILL, kolory.paper);
          setcolor(kolory.paper);
          set_mode_solid();
      }
      else
       {
		  if ((ad->warstwa == Current_Layer) || (options1.view_only_current_layer == 0))
		  {
			  SetColorAC(ad->kolor);
			  setfillstyle_(SOLID_FILL, GetColorAC(ad->kolor));
		  }
		  else
		  {
			  SetColorAC(8);
			  setfillstyle_(SOLID_FILL, GetColorAC(8));
		  }
       }
      setlinestyle1 (SOLID_LINE, 0, NORM_WIDTH) ;
    }
  else
   { setcolor (kolory.blok) ;
     setfillstyle_(SOLID_FILL, kolory.blok);
   }
  Draw_Point (ad, COPY_PUT, kolor) ; //mode
  if(mvcurb.akton) mvcurb.P++;
  if(mvcurb.aktoff) mvcurb.P--;
}

static void rysuj_vector_ (AVECTOR *ad,int mode,int kolor)
/*-----------------------------------------------------*/
{
    int grubosc;
    if (Layers [ad->warstwa].on == 0)
    {
        return ;
    }
    grubosc=linestyle(ad->typ);
    if(mode==COPY_PUT)
    {
        if(!kolor)
        {
            setfillstyle_(SOLID_FILL, kolory.paper);
            setcolor(kolory.paper);
            set_mode_solid();
        }
        else
        {
            if ((ad->warstwa == Current_Layer) || (options1.view_only_current_layer == 0))
            {
                SetColorAC(ad->kolor);
                setfillstyle_(SOLID_FILL, GetColorAC(ad->kolor));
            }
            else
            {
                SetColorAC(8);
                setfillstyle_(SOLID_FILL, GetColorAC(8));
            }
        }
    }
    else
    { setcolor (kolory.blok) ;
        setfillstyle_(SOLID_FILL, kolory.blok);

    }
    Draw_Vector (ad, mode, kolor, 0) ; //mode COPY_PUT
    if(mvcurb.akton) mvcurb.P++;
    if(mvcurb.aktoff) mvcurb.P--;
}



static void rysuj_kolo_(OKRAG *ad,int mode,int kolor)
{
  if (Layers [ad->warstwa].on == 0)
  {
    return ;
  }
  if(mode==COPY_PUT) {
      if (!kolor) {
          setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
          setfillstyle_(SOLID_FILL, kolory.paper);
          setcolor(kolory.paper);
          set_mode_solid();
      } else {
          setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
          if ((ad->warstwa == Current_Layer) || (options1.view_only_current_layer == 0))
          {
              setfillstyle_(SOLID_FILL, GetColorAC(ad->kolor));
              SetColorAC(ad->kolor);
          }
          else
          {
              setfillstyle_(SOLID_FILL, GetColorAC(ad->kolor));
              SetColorAC(8);
          }

          set_mode_trans();
      }
  }else {
      setcolor(kolory.blok);
      setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
      setfillstyle_(SOLID_FILL, kolory.blok);

      set_mode_trans();
  }

   Draw_Kolo ((long_long)pikseleX0(ad->x), (long_long)pikseleY0(ad->y), (long_long)pikseleDX(ad->r)) ;
   set_mode_solid();


  if(mvcurb.akton) mvcurb.K++;
  if(mvcurb.aktoff) mvcurb.K--;
}


static void rysuj_luk_(LUK *ad,int mode,int kolor)
{
    int grubosc;
    NAGLOWEK *nag1, *nag2;
    TEXT *t;
    QUAD t_outline;
    int ret;
    LUK l_tmp, l_tmp1;


    if (Check_if_Equal2(ad->kat1, ad->kat2))
        return;

  if ((ad->typ == HATCH_OUTLINE_TYPE) && (Get_Point_View() == FALSE)) return;

  if (Layers [ad->warstwa].on == 0)
  {
    return ;
  }
  grubosc=linestyle(ad->typ);
  if(mode==COPY_PUT)
    { if(!kolor) setcolor(kolory.paper);
      else
       {
       if ((ad->warstwa==Current_Layer) || (options1.view_only_current_layer==0)) SetColorAC(ad->kolor);
         else SetColorAC(8);
       }
    }
  else
   { setcolor(kolory.blok);

   }
  set_pattern_count(TRUE);

    //checking if line is dimensioning line
    if ((ad->blok == 1) && (ad->obiektt1 == 0) && (ad->obiektt2 == 1) && (ad->obiektt3 == 0) && (ad>dane) && (ad<(dane+dane_size)))
    {
        //check if next object is text or line then text
        nag1 = (char*)ad + sizeof(NAGLOWEK) + ad->n;
        if (nag1->obiekt == Otekst)
        {
            t = (TEXT*)nag1;
            if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
            {
                //checking crossing with text
                //for (i=0; i<4; i++)
                //    ret=lineC(pikseleX0(t_outline.xy[i*2]), pikseleY0(t_outline.xy[i*2+1]), pikseleX0(t_outline.xy[(i*2+2)%8]), pikseleY0(t_outline.xy[(i*2+3)%8]));
                //checking crossing with text
                outlinetext(t, &t_outline, 0.5);
                Normalize_Quad (&t_outline);
                //for (i=0; i<4; i++)
                //    ret=lineC(pikseleX0(t_outline.xy[i*2]), pikseleY0(t_outline.xy[i*2+1]), pikseleX0(t_outline.xy[(i*2+2)%8]), pikseleY0(t_outline.xy[(i*2+3)%8]));
                ret=trim_arc_to_quad(ad, &t_outline, &l_tmp, &l_tmp1);
                ////
                switch (ret)
                {
                    case 0:
                        //L->widoczny = lineC(pikseleX0(L->x1), pikseleY0(L->y1), pikseleX0(L->x2),pikseleY0(L->y2));
                        DrawArc(pikseleX0(ad->x),pikseleY0(ad->y),ad->kat1,ad->kat2,pikseleDX(ad->r), COPY_PUT);
                        break;
                    case 1: //L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                        DrawArc(pikseleX0(l_tmp.x),pikseleY0(l_tmp.y),l_tmp.kat1,l_tmp.kat2,pikseleDX(l_tmp.r), COPY_PUT);
                        break;
                    case 2:// L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                        DrawArc(pikseleX0(l_tmp.x),pikseleY0(l_tmp.y),l_tmp.kat1,l_tmp.kat2,pikseleDX(l_tmp.r), COPY_PUT);
                        // L->widoczny = lineC(pikseleX0(L_tmp1.x1), pikseleY0(L_tmp1.y1), pikseleX0(L_tmp1.x2),pikseleY0(L_tmp1.y2));
                        DrawArc(pikseleX0(l_tmp1.x),pikseleY0(l_tmp1.y),l_tmp1.kat1,l_tmp1.kat2,pikseleDX(l_tmp1.r), COPY_PUT);
                        break;
                    default:
                        break;
                }
                //L->widoczny = lineC(pikseleX0(L->x1), pikseleY0(L->y1), pikseleX0(L->x2), pikseleY0(L->y2));
            }
            else DrawArc(pikseleX0(ad->x),pikseleY0(ad->y),ad->kat1,ad->kat2,pikseleDX(ad->r), COPY_PUT);
        }

        else if (nag1->obiekt == Olinia)
        {
            nag2 = (char*)nag1 + sizeof(NAGLOWEK) + nag1->n;
            if (nag2->obiekt != Otekst) {
                nag1=nag2;
                nag2 = (char *) nag1 + sizeof(NAGLOWEK) + nag1->n;
            }
            if (nag2->obiekt == Otekst)
            {
                t = (TEXT*)nag2;
                if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
                {
                    //checking crossing with text
                    outlinetext(t, &t_outline, 0.5);
                    Normalize_Quad (&t_outline);
                    //for (i=0; i<4; i++)
                    //    ret=lineC(pikseleX0(t_outline.xy[i*2]), pikseleY0(t_outline.xy[i*2+1]), pikseleX0(t_outline.xy[(i*2+2)%8]), pikseleY0(t_outline.xy[(i*2+3)%8]));
                    ret=trim_arc_to_quad(ad, &t_outline, &l_tmp, &l_tmp1);
                    ////
                    switch (ret)
                    {
                        case 0:
                            // L->widoczny = lineC(pikseleX0(L->x1), pikseleY0(L->y1), pikseleX0(L->x2),pikseleY0(L->y2));
                            DrawArc(pikseleX0(ad->x),pikseleY0(ad->y),ad->kat1,ad->kat2,pikseleDX(ad->r), COPY_PUT);
                            break;
                        case 1: //L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                            DrawArc(pikseleX0(l_tmp.x),pikseleY0(l_tmp.y),l_tmp.kat1,l_tmp.kat2,pikseleDX(l_tmp.r), COPY_PUT);
                            break;
                        case 2: //L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                            DrawArc(pikseleX0(l_tmp.x),pikseleY0(l_tmp.y),l_tmp.kat1,l_tmp.kat2,pikseleDX(l_tmp.r), COPY_PUT);
                            //L->widoczny = lineC(pikseleX0(L_tmp1.x1), pikseleY0(L_tmp1.y1), pikseleX0(L_tmp1.x2),pikseleY0(L_tmp1.y2));
                            DrawArc(pikseleX0(l_tmp1.x),pikseleY0(l_tmp1.y),l_tmp1.kat1,l_tmp1.kat2,pikseleDX(l_tmp1.r), COPY_PUT);
                            break;
                        default:
                            break;
                    }
                }
                else DrawArc(pikseleX0(ad->x),pikseleY0(ad->y),ad->kat1,ad->kat2,pikseleDX(ad->r), COPY_PUT);
            }
            else DrawArc(pikseleX0(ad->x),pikseleY0(ad->y),ad->kat1,ad->kat2,pikseleDX(ad->r), COPY_PUT);
        }
    }
    else
        DrawArc(pikseleX0(ad->x),pikseleY0(ad->y),ad->kat1,ad->kat2,pikseleDX(ad->r), COPY_PUT);

  set_pattern_count(FALSE);
  set_pattern_offset(0);
  if(mvcurb.akton) mvcurb.l++;
  if(mvcurb.aktoff) mvcurb.l--;
}


/* constructs nodes to go at the ends of the list, for tangent calculations */
NODE dummy_node(NODE bnode, NODE bprev)
{
    NODE n;

    n.x = bnode.x - (bprev.x - bnode.x) / 8.0;
    n.y = bnode.y - (bprev.y - bnode.y) / 8.0;
    n.tangent = 0.0;

    return n;
}

NODE dummy_node_close(NODE bnode)
{
    NODE n;

    n.x = bnode.x;
    n.y = bnode.y;
    n.tangent = 0.0;

    return n;
}

/* calculates a set of node tangents */
void calc_tangents(int closed, int *node_count, double *bnodes_2_tangent)
{
    int i;
    int node_count_;

    node_count_ = *node_count;

    if (closed == 0)
    {
        bnodes[0] = dummy_node_close(bnodes[1]);

        bnodes[1].tangent = 0.0;

        for (i = 2; i < node_count_-1; i++)
            bnodes[i].tangent = Atan2((bnodes[i + 1].y - bnodes[i - 1].y), (bnodes[i + 1].x - bnodes[i - 1].x));

        *bnodes_2_tangent = Atan2((bnodes[1].y - bnodes[3].y), (bnodes[1].x - bnodes[3].x));

    }
    else
    {
        bnodes[0] = dummy_node_close(bnodes[node_count_ - 1]);

        bnodes[node_count_] = dummy_node_close(bnodes[1]);
        node_count_++;
        bnodes[node_count_] = dummy_node_close(bnodes[2]);
        node_count_++;
        bnodes[node_count_] = dummy_node_close(bnodes[3]);
        node_count_++;

        for (i = 1; i < node_count_-1; i++)
            bnodes[i].tangent = Atan2((bnodes[i + 1].y - bnodes[i - 1].y), (bnodes[i + 1].x - bnodes[i - 1].x));

        //bnodes[node_count].tangent = 0;
        *bnodes_2_tangent = 0;
    }

    *node_count = node_count_;
}

//#define MaxNpts 92 //128 //max number of points for bspline

/* calculates the distance between two bnodes */
float node_dist(NODE n1, NODE n2)
{

    float dx = (n1.x - n2.x);
    float dy = (n1.y - n2.y);

    return sqrt((dx * dx) + (dy * dy));
}

void get_control_points(NODE n1, NODE n2, float points[8])
{
    float dist = node_dist(n1, n2) * (1-curviness);

    points[0] = n1.x;
    points[1] = n1.y;

    points[2] = n1.x + cos(n1.tangent) * dist;
    points[3] = n1.y + sin(n1.tangent) * dist;

    points[4] = n2.x - cos(n2.tangent) * dist;
    points[5] = n2.y - sin(n2.tangent) * dist;

    points[6] = n2.x;
    points[7] = n2.y;
}

void get_control_points_3_difference(NODE n1, NODE n2, NODE n3, float points[8])
{
    float dist = (node_dist(n1, n2) + node_dist(n2, n3)) * 0.5 * (1 - curviness);

    points[0] = n1.x;
    points[1] = n1.y;

    points[2] = n1.x + cos(n1.tangent) * dist;
    points[3] = n1.y + sin(n1.tangent) * dist;

    points[4] = n2.x - cos(n2.tangent) * dist;
    points[5] = n2.y - sin(n2.tangent) * dist;

    points[6] = n2.x;
    points[7] = n2.y;
}

void get_control_points_reversed(NODE n1, NODE n2, float points[])
{
    float dist = node_dist(n1, n2) * (1-curviness);

    points[6] = n1.x;
    points[7] = n1.y;

    points[4] = n1.x + cos(n1.tangent) * dist;
    points[5] = n1.y + sin(n1.tangent) * dist;

    points[2] = n2.x;
    points[3] = n2.y;

    points[0] = n2.x;
    points[1] = n2.y;
}


void get_control_points_end(NODE n1, NODE n2, float points[])
{
    float dist = node_dist(n1, n2) * (1-curviness);

    points[0] = n1.x;
    points[1] = n1.y;

    points[2] = n1.x + cos(n1.tangent) * dist;
    points[3] = n1.y + sin(n1.tangent) * dist;

    points[4] = n2.x;
    points[5] = n2.y;

    points[6] = n2.x;
    points[7] = n2.y;
}


double get_first_bnods_direction(SPLINE* ptr_spline)
{
    float points[8];
    double al;
    int j, node_count;

    node_count = 0;
    for (j = 0; j < (int)ptr_spline->lp / 2 ; j++)
        {
            bnodes[node_count].x = ptr_spline->xy[j * 2];
            bnodes[node_count].y = ptr_spline->xy[j * 2 +1];
            node_count++;
        } 

    get_control_points_reversed(bnodes[0], bnodes[1], points);
    al = Atan2(points[3] - points[5], points[2] - points[4]);
    return al;
}

double get_last_bnods_direction(SPLINE *ptr_spline)
{
    float points[8];
    double al;
    int j, node_count;

    node_count = 0;
    for (j = 0; j < (int)ptr_spline->lp / 2; j++)
    {
        bnodes[node_count].x = ptr_spline->xy[j * 2];
        bnodes[node_count].y = ptr_spline->xy[j * 2 + 1];
        node_count++;
    }
    get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], points);
    al = Atan2(points[3] - points[5], points[2] - points[4]);
    return al;
}

void Set_Curviness(double n_curviness)
{
    curviness = n_curviness;
}


void set_bnodes(SPLINE *ad, float *ad_xy, int *node_count_)
{
    int node_count;
    int j;
    double dx0, dx1, dy0, dy1, px0, py0, px1, py1;
    double magnitude = 3.0;

    //if spline is type 6 (ad->npts == 6) market points are control points, so is necessary to find waypoints, like in type 5
            //
    node_count = 1;

    if (ad->npts == 5)
    {
        for (j = 0; j < (int)ad->lp / 2; j++)
        {
            bnodes[node_count].x = ad_xy[j * 2];  //ad->xy
            bnodes[node_count].y = ad_xy[j * 2 + 1];  //ad->xy
            node_count++;
        }
    }
    else //typ 6
    {
        //For 0 < k < (int)ad->lp / 2, divide each segment B[k−1] B[k] into three equal parts with subdivision points P[k−1], Q[k], such
        //    that, in the direction from B[0] to B[n], each B[k] has Q[k] and P[k] as its immediate neighbor to the leftand to
        //    the right, respectively; denote by S[k] the midpoint of segment Q[k] P[k] and put S[0] = B[0], S[n] = [B]
        //For 0 < k ≤ (int)ad->lp / 2, take a cubic Bézier curve C[k] with control points S[k−1], P[k−1], Q[k], and S=[k] represented by a
        //    cubic Bézier formula

        bnodes[node_count].x = ad_xy[0];   //ad->xy
        bnodes[node_count].y = ad_xy[1];   //ad->xy

        node_count++;

        for (j = 1; j < ((int)ad->lp - 2) / 2; j++)
        {

            //P[k] points
            dx0 = ad_xy[j * 2] - ad_xy[j * 2 - 2];   //ad->xy
            dy0 = ad_xy[j * 2 + 1] - ad_xy[j * 2 - 1];   //ad->xy

            dx1 = ad_xy[j * 2 + 2] - ad_xy[j * 2];   //ad->xy
            dy1 = ad_xy[j * 2 + 3] - ad_xy[j * 2 + 1];   //ad->xy

            px0 = ad_xy[j * 2] - dx0 / magnitude;   //ad->xy
            py0 = ad_xy[j * 2 + 1] - dy0 / magnitude;   //ad->xy

            px1 = ad_xy[j * 2] + dx1 / magnitude;   //ad->xy
            py1 = ad_xy[j * 2 + 1] + dy1 / magnitude;   //ad->xy

            bnodes[node_count].x = (px0 + px1) / 2.0f;
            bnodes[node_count].y = (py0 + py1) / 2.0f;

            node_count++;
        }

        bnodes[node_count].x = ad_xy[(int)ad->lp - 2];   //ad->xy
        bnodes[node_count].y = ad_xy[(int)ad->lp - 1];   //ad->xy

        node_count++;

        if (node_count < 4)
        {
            bnodes[node_count].x = ad_xy[(int)ad->lp - 2];   //ad->xy
            bnodes[node_count].y = ad_xy[(int)ad->lp - 1];   //ad->xy

            node_count++;
        }
    }
    *node_count_ = node_count;
}

void rysuj_spline_(SPLINE *ad, float *ad_xy, int mode, int kolor, BOOL count_mvcurb, BOOL partial, BOOL moving)
{
	T_PixelTVal PolyPoints[MaxNumPoints];
	int grubosc;
	int i, j;
	int npts;
	float out_x[MaxNpts], out_y[MaxNpts];
	int outx1, outy1, outx2, outy2;
	long TotL;
    double bnodes_2_tangent_org, bnodes_2_tangent;
    SPLINE tmp_spline = Splinedef;
    int ret;
    int extra_node = 0;

    int node_count;

	if (Layers[ad->warstwa].on == 0)
	{
		return;
	}

    grubosc=linestyle(ad->typ);
	if (mode == COPY_PUT)
	{
		if (!kolor) setcolor(kolory.paper);
		else
		{
			if ((ad->warstwa == Current_Layer) || (options1.view_only_current_layer == 0))
            {
                if (((ad->atrybut == Ablok) ||
                    ((dragging_quad_move == TRUE) && (ad->atrybut == Aoblok))) && (!moving))
                {
                    setcolor(kolory.blok);

                } else
                {
                    SetColorAC(ad->kolor);

                }
            }
			else SetColorAC(8);

		}

	}
	else
	{
		if (kolor!=-1) setcolor(kolory.blok);
        else
        {
            if (ad->kolor == 0) SetColorAC(15); else SetColorAC(ad->kolor);
        }

	}

    if (ad->npts < 5)
    {
        TotL = 0;

        for (i = 0; i < (int)(ad->lp) && i < (MaxNumPoints); i += 2)
        {
            PolyPoints[i] = pikseleX0(ad_xy[i]);  //ad->xy
            PolyPoints[i + 1] = pikseleY0(ad_xy[i + 1]);  //ad->xy
        }

        for (i = 0; i < (int)(ad->lp - 2) && i < (MaxNumPoints); i += 2)
        {
            TotL += (long)sqrt((float)((PolyPoints[i + 2] - PolyPoints[i]) * (PolyPoints[i + 2] - PolyPoints[i])) + (float)((PolyPoints[i + 3] - PolyPoints[i + 1]) * (PolyPoints[i + 3] - PolyPoints[i + 1])));
        }

#define NPA 90 //56

        npts = 8 + TotL / 30;
        if (npts > NPA)
        {
            npts = NPA;
        }

        calc_bspline(ad->lp, ad->npts, ad_xy, npts, out_x, out_y);  //ad->xy

        set_pattern_count(TRUE);
    
        for (i = 0; i < (npts - 1); i++)
        {
            outx1 = pikseleX0(out_x[i]);
            outy1 = pikseleY0(out_y[i]);
            outx2 = pikseleX0(out_x[i + 1]);
            outy2 = pikseleY0(out_y[i + 1]);
            ret = lineC(outx1, outy1, outx2, outy2);
            if (ret) ad->widoczny = 1;
        }

        set_pattern_count(FALSE);
        set_pattern_offset(0);

        if (Get_Control_Points_View() && (!partial))
        {

            if (mode == XOR_PUT) linestyle(7);  //36
            else linestyle(7); //32

            setcolor(kolory.blok);

            if (mode == COPY_PUT)
            {
                if (!kolor) setcolor(kolory.paper);
                else
                {
                    setcolor(kolory.blok);
                }
            }
            else
            {
                setcolor(kolory.blok);
            }

            if (!Get_Curviness_Only())
            {
                for (i = 0; i < (int)ad->lp - 2 && i < MaxNumPoints; i += 2)
                {
                    ret = lineC(PolyPoints[i], PolyPoints[i + 1], PolyPoints[i + 2], PolyPoints[i + 3]);
                    if (ret) ad->widoczny = ret;
                }
            }
        }

        if (count_mvcurb)
        {
            if (mvcurb.akton) mvcurb.l++;
            if (mvcurb.aktoff) mvcurb.l--;
        }
    }
    else //multipoint
    {
        
        curviness = ad->xy[ad->lp];  //always on the end is curviness of uniform spline

        if (ad->lp < 6)
        {
            ret = lineC(pikseleX0(ad_xy[0]), pikseleY0(ad_xy[1]), pikseleX0(ad_xy[2]), pikseleY0(ad_xy[3])); //ad->xy
            if (count_mvcurb)
            {
                if (mvcurb.akton) mvcurb.l++;
                if (mvcurb.aktoff) mvcurb.l--;
            }
            return;
        }
        tmp_spline.atrybut=ad->atrybut;
        tmp_spline.warstwa=ad->warstwa;
        tmp_spline.npts = 4;
        tmp_spline.lp = 8;
        tmp_spline.kolor = ad->kolor;
        tmp_spline.typ = ad->typ;
        tmp_spline.widoczny = 0;
        

        node_count = 1;

        set_bnodes(ad, ad_xy, &node_count);

        calc_tangents(ad->closed, &node_count, &bnodes_2_tangent);

        set_pattern_count(TRUE);

        if (ad->closed == 0)
        {
                bnodes_2_tangent_org = bnodes[2].tangent;
                bnodes[2].tangent = bnodes_2_tangent;
                get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
                rysuj_spline_(&tmp_spline, &tmp_spline.xy, mode, kolor, count_mvcurb, TRUE, moving);
                bnodes[2].tangent = bnodes_2_tangent_org;
        }
        

        for (i = 2; i < node_count - 2; i++)  //1 - extra_node
        {
            get_control_points(bnodes[i], bnodes[i+1], tmp_spline.xy);
            //get_control_points_3_difference(bnodes[i], bnodes[i + 1], bnodes[i-1], &tmp_spline.xy);  //it's for Catmull–Rom spline: https://en.wikipedia.org/wiki/Cubic_Hermite_spline
            rysuj_spline_(&tmp_spline, &tmp_spline.xy ,mode, kolor, count_mvcurb, TRUE, moving);
        }

        
        if ((node_count > 3)  &&  (ad->closed == 0))
        {
            get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
            rysuj_spline_(&tmp_spline, &tmp_spline.xy ,mode, kolor, count_mvcurb, TRUE, moving);
        }


        ad->widoczny = tmp_spline.widoczny;
       

        set_pattern_count(FALSE);
        set_pattern_offset(0);

        if (Get_Control_Points_View())
        {

            if (mode == XOR_PUT) linestyle(7);  //36
            else linestyle(7); //32

            setcolor(kolory.blok);

            if (mode == COPY_PUT)
            {
                if (!kolor) setcolor(kolory.paper);
                else
                {
                    setcolor(kolory.blok);
                }
            }
            else
            {
                setcolor(kolory.blok);
            }
            if (!Get_Curviness_Only())
            {
                for (i = 0; i < (int)ad->lp - 2; i += 2)
                {
                    ret = lineC(pikseleX0(ad_xy[i]), pikseleY0(ad_xy[i + 1]), pikseleX0(ad_xy[i + 2]), pikseleY0(ad_xy[i + 3]));  //ad->xy
                    if (ret) ad->widoczny = ret;
                }
                if (ad->closed==1) ret = lineC(pikseleX0(ad_xy[i]), pikseleY0(ad_xy[i+1]), pikseleX0(ad_xy[0]), pikseleY0(ad_xy[1]));  //ad->xy
                if (ret) ad->widoczny = ret;
            }
        }
       
    }

}

static int grubosc_linii_solid[5]={1,1,1,3,3};

static void rysuj_wypelnienie_(WIELOKAT *ad,int mode,int kolor, int W3D, BOOL wpcx, BOOL as_redraw)
{
  T_PixelTVal PolyPoints [MaxNumPoints] ;
  T_Float PolyPoints_Z[MaxNumPoints_Z] ;
  int  PolyPoints_i [MaxNumPoints] ;
  int NumPoints, i, j ;
  int grubosc;
  BOOL hatch_solid = FALSE;
  SOLID_PATTERN  solid_pattern = {3, 0, 0, 0, 0, ""};
  char* scale_ptr;
  char* dx_ptr;
  char* angle_ptr;
  char* dy_ptr;
  char* name_ptr;
  char* translucency_ptr;
  unsigned char translucency=255;
  int found;
  BOOL ignore_p;
  BOOL ret_move;
  int ii;

  if (Layers [ad->warstwa].on == 0)
  {
    return ;
  }

  if (W3D==1) ad->empty_typ=0; ///////UWAGA 

  for(i=0; i< (int)ad->lp && i<MaxNumPoints; i++)
   if ((i % 2) == 0) 
   {
	   j=i/2;
	   PolyPoints[i]=pikseleX0(ad->xy[i]);
	   if (W3D==1) PolyPoints_Z[j]=ad->xy[ad->lp+j];
   }
   else PolyPoints[i]=pikseleY0(ad->xy[i]);

  NumPoints=i/2;

  setwritemode(COPY_PUT);


    if ((ad->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
    {
        solid_pattern.flag = 0;
        if (((ad->atrybut==Ablok) && (as_redraw)) ||
            ((dragging_quad_move == TRUE) && (ad->atrybut == Aoblok)))
        {
            setcolor(kolory.blok);
            setfillstyle_(SOLID_FILL,kolory.blok);
        }
        else
        {
            SetColorAC(ad->kolor);
            setfillstyle_(SOLID_FILL,GetColorAC(ad->kolor));
        }
    }
    else
    {
        solid_pattern.flag = 1;  //hide
        SetColorAC(8);
        setfillstyle_(SOLID_FILL,GetColorAC(8));
    }

  if (mode==COPY_PUT)
  {
     if (ad->empty_typ==0)
      {
        setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
        if(!kolor)
         { setfillstyle_(SOLID_FILL,kolory.paper);
           setcolor(kolory.paper);

		   set_mode_solid();
     
		   Draw_Solid (NumPoints, PolyPoints, ad->pcx_solid, ad->obiektt3, pikseleX0(0 /*ad->xy[0]*/), pikseleY0(0 /*ad->xy[1]*/), NULL, 255) ;

         }
        else
        {
            if ((ad->warstwa == Current_Layer) || (options1.view_only_current_layer == 0))
            {
                if (wpcx==FALSE) {
                    setfillstyle_(SOLID_FILL, GetColorAC(ad->kolor));
                    SetColorAC(ad->kolor);
                }
                else {
                    setfillstyle_(SOLID_FILL, ad->kolor);
                    setcolor(ad->kolor);
                }
            }
            else
            {
                setfillstyle_(SOLID_FILL, GetColorAC(8));
                SetColorAC(8);
            }


            if ((ad->pattern == 0) || (ad->n==(ad->lp*sizeof(float)+8)))
            {
                //Draw_Solid(NumPoints, PolyPoints, ad->pcx_solid, ad->obiektt3, pikseleX0(0 /*ad->xy[0]*/), pikseleY0(0 /*ad->xy[1]*/), NULL, translucency);
                if ((ad->warstwa == Current_Layer) || (options1.view_only_current_layer == 0)) setfillstyle_(SOLID_FILL, GetColorAC(ad->kolor));
                else setfillstyle_(SOLID_FILL, GetColorAC(8));

                ignore_p = FALSE;

                translucency = TRANSLUCENCY;

                if ((ad->translucent == 1) || (TRANSLUCENCY < 255))
                {

                    if (ad->translucent == 1)
                    {
                        translucency_ptr = ad->xy;
                        translucency_ptr += (ad->lp * sizeof(float));
                        memmove(&translucency, translucency_ptr, sizeof(unsigned char));
                    }
                    else translucency = 255;
                    if (translucency > TRANSLUCENCY) translucency = TRANSLUCENCY;
                    set_trans_blender(0, 0, 0, translucency);
                    set_mode_trans();
                }

                if (!ignore_p) Draw_Solid(NumPoints, PolyPoints, ad->pcx_solid, ad->obiektt3, pikseleX0(0), pikseleY0(0), NULL, translucency);

            }
            else
            {

                scale_ptr = (char *)ad->xy;
                scale_ptr+=(ad->lp * sizeof(float));
                dx_ptr = scale_ptr;
                dx_ptr += sizeof(short int);
                angle_ptr = dx_ptr;
                angle_ptr+=sizeof(short int);
                dy_ptr = angle_ptr;
                dy_ptr += sizeof(short int);
                name_ptr = dy_ptr;
                name_ptr+=sizeof(short int);
                memmove(&solid_pattern.scale, scale_ptr, sizeof(short int));
                memmove(&solid_pattern.dx, dx_ptr, sizeof(short int));
                memmove(&solid_pattern.angle, angle_ptr, sizeof(short int));
                memmove(&solid_pattern.dy, dy_ptr, sizeof(short int));
                strcpy(solid_pattern.pattern, name_ptr);
                Draw_Solid(NumPoints, PolyPoints, ad->pcx_solid, ad->obiektt3, pikseleX0(0 /*ad->xy[0]*/), pikseleY0(0 /*ad->xy[1]*/), &solid_pattern, translucency);


                ////////////////  if Ablok
                if (((ad->atrybut==Ablok) && (as_redraw)) ||
                    ((dragging_quad_move == TRUE) && (ad->atrybut == Aoblok)))
                {
                    setcolor(kolory.blok);
                    setfillstyle_(SOLID_FILL,kolory.blok);

                    set_trans_blender(0, 0, 0, (int) HALFTRANSLUCENCY);
                    set_mode_trans();

                    Draw_Solid(NumPoints, PolyPoints, ad->pcx_solid, ad->obiektt3, pikseleX0(0), pikseleY0(0), NULL, HALFTRANSLUCENCY);

                    reset_trans_blender();
                    set_mode_solid();
                }
                ///////////////
            }

         }
      }
     else if (ad->empty_typ==6)
      {
        setlinestyle3(DASHED_LINE,0,NORM_WIDTH);
        if(!kolor)
         { setfillstyle_(MY_DOT_FILL,kolory.paper);
           setcolor(kolory.paper);
         }
        else
         {  if ((ad->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
              { setfillstyle_(MY_DOT_FILL,GetColorAC(ad->kolor));
	             SetColorAC(ad->kolor);
              }
            else
              { setfillstyle_(MY_DOT_FILL,GetColorAC(8));
	             SetColorAC(8);
              }
         }
        Draw_Solid (NumPoints, PolyPoints, ad->pcx_solid, ad->obiektt3, pikseleX0(0 /*ad->xy[0]*/), pikseleY0(0 /*ad->xy[1]*/), NULL, translucency) ;
      }
      else if (ad->empty_typ==7)
      {

         if(!kolor) setcolor(kolory.paper);
          else
           {
            if ((ad->warstwa==Current_Layer) || (options1.view_only_current_layer==0))  SetColorAC(ad->kolor);
              else SetColorAC(8);
           }
          grubosc=linestyle_solid(ad->empty_typ);
         // setlinestyle(SOLID_LINE,0,grubosc);
          for(i=0; i< (int)ad->lp && i<MaxNumPoints; i+=2)
           {
            if (i==(ad->lp-2)) lineC (PolyPoints[i], PolyPoints[i+1], PolyPoints[0], PolyPoints[1]);
             else lineC (PolyPoints[i], PolyPoints[i+1], PolyPoints[i+2], PolyPoints[i+3]);
           }
      }
       else
        {
          if(!kolor) setcolor(kolory.paper);
          else
           {
            if ((ad->warstwa==Current_Layer) || (options1.view_only_current_layer==0)) {
                if (wpcx==FALSE)  SetColorAC(ad->kolor);
                else (setcolor(ad->kolor));
            }
              else SetColorAC(8);
           }
          grubosc=linestyle_solid(ad->empty_typ);

          for(i=0; i< (int)ad->lp && i<MaxNumPoints; i+=2)
           {
            if (i==(ad->lp-2)) lineC (PolyPoints[i], PolyPoints[i+1], PolyPoints[0], PolyPoints[1]);
             else lineC (PolyPoints[i], PolyPoints[i+1], PolyPoints[i+2], PolyPoints[i+3]);
           }
        }
  }
  else  //XOR_PUT
   {

      setcolor(kolory.blok);
      setfillstyle_(MY_DOT_FILL,kolory.blok);

    if (draw_hatch_pattern == TRUE)
    {
       if (draw_hatch_image_pattern) SetColorAC(ad->kolor);
       else SetColorAC(SolidG.kolor);
    }

     PolyPoints[i++]=pikseleX0(ad->xy[0]);
     PolyPoints[i]=pikseleY0(ad->xy[1]);
     if (ad->empty_typ==0)
      {
       NumPoints++;
       for (i = 0; i < 2 * NumPoints; i++)
          PolyPoints_i [i] = (int)(PolyPoints [i] +0.5);


          translucency = TRANSLUCENCY;

          if ((ad->translucent == 1) || (TRANSLUCENCY < 255))
          {
              if (ad->translucent == 1)
              {
                  translucency_ptr = ad->xy;
                  translucency_ptr += (ad->lp * sizeof(float));
                  memmove(&translucency, translucency_ptr, sizeof(unsigned char));
              }
              else translucency = 255;
              if (translucency > TRANSLUCENCY) translucency = TRANSLUCENCY;
              set_trans_blender(0, 0, 0, translucency);
              set_mode_trans();
          }

          Draw_Solid (NumPoints, PolyPoints, ad->pcx_solid, ad->obiektt3, pikseleX0(0 /*ad->xy[0]*/), pikseleY0(0 /*ad->xy[1]*/), NULL, translucency) ;


      }
      else
       {
        grubosc=linestyle_solid(ad->empty_typ);

        for(i=0; i< (int)ad->lp && i<MaxNumPoints; i+=2)
         {
          if (i==(ad->lp-2)) lineC (PolyPoints[i], PolyPoints[i+1], PolyPoints[0], PolyPoints[1]);
            else lineC (PolyPoints[i], PolyPoints[i+1], PolyPoints[i+2], PolyPoints[i+3]);
         }
       }
   }

    set_mode_solid();
  if(mvcurb.akton) mvcurb.W++;
  if(mvcurb.aktoff) mvcurb.W--;
}

void rysuj_bitmap_outline(char *ad, int mode, BOOL enforce, int kolor0, int kolor, int gr)
{
	WIELOKAT S4 = Stdef;


		Solid04(ad, (char *)&S4, kolor);  //blok
        S4.empty_typ=gr;
        //S4.kolor=kolor;
		rysuj_wypelnienie_(&S4, mode, kolor0, 0, TRUE, FALSE);

}

void rysuj_obiekt_(char *ad, int mode,int kolor)
{ // int kk;
   WIELOKAT S4=Stdef;
   BLOK *b1;

  switch(((NAGLOWEK*)ad)->obiekt)
    { 
    case OdBLOK:
		b1 = (BLOK *)ad;
		if ((b1->kod_obiektu == B_PLINE) &&
			((b1->opis_obiektu[0] == PL_PLINE) ||
			(b1->opis_obiektu[0] == PL_ELLIPSE) ||
				(b1->opis_obiektu[0] == PL_SKETCH) ||
				(b1->opis_obiektu[0] == PL_CURVE) ||
				(b1->opis_obiektu[0] == PL_ELLIPSE_ARC)
				))
		{
			set_pattern_count(TRUE);
			set_pattern_offset(0);
			pattern_add_limit = (long_long)(ad) + b1->n - 1;
		}
		break;
    case Olinia :
		    rysuj_linia_((LINIA*)ad, mode, kolor);
	break;
      case Otekst :

	        rysuj_tekst_((TEXT*)ad,mode,kolor);
	break;
      case   Okolo :

	        rysuj_kolo_((OKRAG*)ad,mode,kolor);
	break;
      case   Ookrag :

	        rysuj_okrag_((OKRAG*)ad,mode,kolor);
	break;
    case   Oellipse :

            rysuj_ellipse_((ELLIPSE*)ad,mode,kolor);
    break;
    case   Oellipticalarc :

            rysuj_ellipticalarc_((ELLIPTICALARC *)ad,mode,kolor);
            break;
    case Osolidarc:
            rysuj_solidarc_((SOLIDARC *)ad,mode,kolor, FALSE, TRUE);
        break;
    case   Ofilledellipse :

            rysuj_filledellipse_((ELLIPSE*)ad,mode,kolor);
    break;
      case   Oluk :
            rysuj_luk_((LUK*)ad,mode,kolor);
   break;
      case   Owwielokat :
            rysuj_wypelnienie_((WIELOKAT*)ad,mode,kolor,0,FALSE, FALSE);
   break;
	  case   Ospline:
		    rysuj_spline_((SPLINE*)ad, ((SPLINE*)ad)->xy, mode, kolor, TRUE, FALSE, TRUE);
   break;
       case Opoint :
            rysuj_punkt_ ((T_Point*)ad,mode, kolor) ;
   break;
        case Ovector :
            rysuj_vector_ ((AVECTOR*)ad,mode, kolor) ;
    break;
   case Opcx :

        bitmap_exist=TRUE;
		if (((B_PCX *)ad)->kod_obiektu == 2) bitmap_png_exist = TRUE;

        if ((mode==0) && ((kolor==0) || (kolor==1)))
         {
          if (kolor==1) TTF_redraw = TRUE;
         }
          else if (mode==XOR_PUT)
           {
             
			if ((global_no_pcx==FALSE) && ((NAGLOWEK*)ad)->atrybut!=Ablok)             //if (((NAGLOWEK*)ad)->blok!=Ablok)
			{
			 rysuj_bitmap_outline((char *)ad, mode, FALSE, kolor, kolory.cur, 5);
			 }
            Solid04(ad, (char *)&S4, kolory.blok);
            S4.empty_typ=5;
            rysuj_wypelnienie_(&S4,mode,kolor,0,TRUE, FALSE);
           }
      break;
      default  : break;
    }
}


void rysuj_obiekt_no_pcx(char *ad, int mode,int kolor)
{
   WIELOKAT S4=Stdef;
   BLOK *b1;

  switch(((NAGLOWEK*)ad)->obiekt)
    { 
    case OdBLOK:
	  b1 = (BLOK *)ad;
	  if ((b1->kod_obiektu == B_PLINE) &&
		  ((b1->opis_obiektu[0] == PL_PLINE) ||
		  (b1->opis_obiektu[0] == PL_ELLIPSE) ||
			  (b1->opis_obiektu[0] == PL_SKETCH) ||
			  (b1->opis_obiektu[0] == PL_CURVE) ||
			  (b1->opis_obiektu[0] == PL_ELLIPSE_ARC)
			  ))
	  {
          set_pattern_offset(0);
		  pattern_add_limit = (long_long)(ad) + b1->n - 1;
	  }
	  break;
    case Olinia :
   rysuj_linia_((LINIA*)ad,mode,kolor);
	break;
      case Otekst :

	rysuj_tekst_((TEXT*)ad,mode,kolor);
	break;
      case   Okolo :

	rysuj_kolo_((OKRAG*)ad,mode,kolor);
	break;
      case   Ookrag :

   rysuj_okrag_((OKRAG*)ad,mode,kolor);
   break;
    case   Oellipse :

        rysuj_ellipse_((ELLIPSE*)ad,mode,kolor);
        break;
    case   Oellipticalarc :

        rysuj_ellipticalarc_((ELLIPTICALARC *)ad,mode,kolor);
        break;
    case Osolidarc:
        rysuj_solidarc_((SOLIDARC *)ad,mode,kolor, FALSE, TRUE);
        break;
    case   Ofilledellipse :

        rysuj_filledellipse_((ELLIPSE*)ad,mode,kolor);
        break;
   case   Oluk :

        rysuj_luk_((LUK*)ad,mode,kolor);
   break;
      case   Owwielokat :
        rysuj_wypelnienie_((WIELOKAT*)ad,mode,kolor,0,FALSE, FALSE);
   break;
	  case   Ospline:
		  rysuj_spline_((SPLINE*)ad, ((SPLINE*)ad)->xy, mode, kolor, TRUE, FALSE, FALSE);
   break;
       case Opoint :

   rysuj_punkt_ ((T_Point*)ad,mode, kolor) ;
   break;
   case Opcx :
      break;
      default  : break;
    }
}


void rysuj_obiekt (char *ad, int mode, int kolor)
/*----------------------------------------------*/
{
    if (ad == NULL)
    {
        Internal_Error (__LINE__,__FILE__);
        return;
    }

    Set_Second_Screen();

    setwritemode (COPY_PUT) ;
    rysuj_obiekt_ (ad, mode, kolor) ;

    if ((mode == COPY_PUT) && (kolor!=0))
    {
        ;
    }

    Set_Screen();
}

void rysuj_obiekt__ (char *ad, int mode, int kolor)
/*----------------------------------------------*/
{
  //if ((mode==COPY_PUT)  && (kolor!=0)) cursor_out();
  if (ad == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
  okno_r () ;
  setwritemode (mode) ;
  rysuj_obiekt_ (ad, mode, kolor) ;
  okno_all () ;
  if ((mode == COPY_PUT) && (kolor!=0))
  {
	  ;
  }
}

static void transformacja_obiekt (void *ad,double x,double y,double k1,double k2,
	     double xa, double ya, double ka1, double ka2, int trans,double z)
{
  NAGLOWEK *nag;

  if (ad == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
  Change = TRUE;
  Set_Auto_Backup (TRUE);
  nag=(NAGLOWEK*)ad;
  if ((nag->obiekt == Otekst) /*|| (nag->obiekt == Otekst3D)*/)
  {
	  if (PTRS__Text_Style[((TEXT*)nag)->czcionka]->type == 2) TTF_redraw = TRUE;
  }
  switch(trans)
  {
     case Tprzesuw : (*moveB[nag->obiekt])((char  *)ad,x,y);
		     break;

     case TprzesuwZ: (*moveBZ[nag->obiekt-1])((char  *)ad,x,y);
		     break;

     case Tobrot   : (*obrotB[nag->obiekt])((char  *)ad,x,y,k1,k2);
		     break;

     case TobrotPCX : (*obrotB_PCX[nag->obiekt])((char  *)ad,x,y,k1,k2);
		     break;

     case TobrotXZ : (*obrotBXZ[nag->obiekt-1])((char  *)ad,x,y,z,k1,k2);
		     break;

     case TobrotYZ : (*obrotBYZ[nag->obiekt-1])((char  *)ad,x,y,z,k1,k2);
		     break;

     case Tskala   : (*skalaB[nag->obiekt])((char  *)ad,x,y,k1,k2);
		     break;

     case Tobrot | Tskala :
		     (*obrotB[nag->obiekt])((char  *)ad,x,y,k1,k2) ;
		     (*skalaB[nag->obiekt])((char  *)ad,xa,ya,ka1, ka2) ;
		     break;

     case TobrotXZ | Tskala :
		     (*obrotBXZ[nag->obiekt-1])((char  *)ad,x,y,z,k1,k2) ;
		     (*skalaB[nag->obiekt])((char  *)ad,xa,ya,ka1, ka2) ;
		     break;

     case Tobrot | Tprzesuw :
		     (*obrotB[nag->obiekt])((char  *)ad,x,y,k1,k2) ;
		     (*moveB[nag->obiekt])((char  *)ad,xa,ya) ;
		     break;

     case TobrotYZ | Tprzesuw :
		     (*obrotBYZ[nag->obiekt-1])((char  *)ad,x,y,z,k1,k2) ;
		     (*moveB[nag->obiekt])((char  *)ad,xa,ya) ;
		     break;

     case Tmirror: (*mirrorB [nag->obiekt - 1])((char  *)ad, x, y, k1, k2) ;
		     break;

    default        : break;
  }
}


void out_blok2_pcx(double x, double y, double k1, double k2,
    double xa, double ya, double ka1, double ka2, int trans, double z)
{

    okno_r();
    oknoS(Xp, Yp, Xk, Yk);
    setwritemode(XOR_PUT);

    GlobalPCX->x = (float)x;
    GlobalPCX->y = (float)y;
    rysuj_obiekt_((char*)GlobalPCX, XOR_PUT, 1);
    okno_all();
}
/*-----------------------------------------------------------*/


/*________________________________________________________________________*/
#define i64000 64000u
void mvcurbp(unsigned i)      /* ini */
/*---------------------*/
{
  if (i)
  {
    i = i64000 ;
    mvcurbplL (i) ;
    mvcurbpT (i) ;
  }
  mvcurb.L = mvcurb.T = mvcurb.K = mvcurb.O =
  mvcurb.l = mvcurb.W = mvcurb.P = mvcurb.E = mvcurb.FE = mvcurb.EA = i ;
}

void blokquad(char  *adp,char  *adk,int atrybut,int mode, int kolor)
{
    NAGLOWEK *ad;
    int l;
    LINIA buf;
    TEXT tbuf;
    SPLINE spbuf;
    WIELOKAT sbuf;
    POINTF xy;
    T_Point pbuf;
    BLOK *bbuf;
    int i;
    QUAD outline, outline1;
    double angle, angle1;
    double lt, lt1, ht, ht1;
    T_Desc_Ex_Block *ptrs_desc_bl;
    char keym;

    if (adp == NULL || adk == NULL)
    {
        return;
    }

    Set_Screen();
    okno_r();

    setwritemode(COPY_PUT);
    obiekt_tok((char*)adp,adk,(char **) &ad,ONieOkreslony);

    if(mvcurb.akton) { mvcurb.mvc=0;mvcurbp(0);mskp();}

    while (ad!=NULL)
    {
#ifndef LINUX
       keym = readmouse_move();
#endif

       if (mvcurb.akton && mvcurb.mvc)
       {
           goto e;
       }
        if (mvcurb.aktoff && !mvcurb.L && !mvcurb.T && !mvcurb.K &&
            !mvcurb.O && !mvcurb.l && !mvcurb.W && !mvcurb.P)
        {
            goto e;
        }


        ad->widoczny=1;

        if (ad->widoczny &&  TRUE == Check_Attribute (ad->atrybut, atrybut)) {

            if (ad->obiekt==Olinia) {
                l = sizeof(NAGLOWEK) + ad->n;
                memmove(&buf, ad, l);
                xy=map_point(buf.x1, buf.y1);
                buf.x1=xy.x; buf.y1=xy.y;
                xy=map_point(buf.x2, buf.y2);
                buf.x2=xy.x; buf.y2=xy.y;
                rysuj_obiekt_((char *) &buf, mode, kolor);
            }

            else if (ad->obiekt==Ospline)
            {
                l = sizeof(NAGLOWEK) + ad->n;
                memmove(&spbuf, ad, l);

                for (i=0; i<spbuf.lp; i+=2)
                {
                    xy=map_point(spbuf.xy[i], spbuf.xy[i+1]);
                    spbuf.xy[i]=xy.x; spbuf.xy[i+1]=xy.y;
                }

                rysuj_obiekt_((char *) &spbuf, mode, kolor);
            }
            else if (ad->obiekt==Owwielokat)
            {
                l = sizeof(NAGLOWEK) + ad->n;
                memmove(&sbuf, ad, l);

                for (i=0; i<(sbuf.lp); i+=2)
                {
                    xy=map_point(sbuf.xy[i], sbuf.xy[i+1]);
                    sbuf.xy[i]=xy.x; sbuf.xy[i+1]=xy.y;
                }
                Normalize_Solid(&sbuf);
                rysuj_obiekt_((char *) &sbuf, mode, kolor);
            }
            else if (ad->obiekt==Otekst)
            {
                outlinetext((TEXT*)ad, &outline, 0.0);
                angle=atan2(outline.xy[3]-outline.xy[1], outline.xy[2]-outline.xy[0]);
                lt=sqrt(((outline.xy[3]-outline.xy[1])*(outline.xy[3]-outline.xy[1]))+((outline.xy[2]-outline.xy[0])*(outline.xy[2]-outline.xy[0])));
                ht=sqrt(((outline.xy[7]-outline.xy[1])*(outline.xy[7]-outline.xy[1]))+((outline.xy[6]-outline.xy[0])*(outline.xy[6]-outline.xy[0])));

                xy=map_point(outline.xy[0], outline.xy[1]);
                outline1.xy[0]=xy.x; outline1.xy[1]=xy.y;
                xy=map_point(outline.xy[2], outline.xy[3]);
                outline1.xy[2]=xy.x; outline1.xy[3]=xy.y;
                xy=map_point(outline.xy[4], outline.xy[5]);
                outline1.xy[4]=xy.x; outline1.xy[5]=xy.y;
                xy=map_point(outline.xy[6], outline.xy[7]);
                outline1.xy[6]=xy.x; outline1.xy[7]=xy.y;

                //calculating the angle change
                angle1=atan2(outline1.xy[3]-outline1.xy[1], outline1.xy[2]-outline1.xy[0]);
                lt1=sqrt(((outline1.xy[3]-outline1.xy[1])*(outline1.xy[3]-outline1.xy[1]))+((outline1.xy[2]-outline1.xy[0])*(outline1.xy[2]-outline1.xy[0])));
                ht1=sqrt(((outline1.xy[7]-outline1.xy[1])*(outline1.xy[7]-outline1.xy[1]))+((outline1.xy[6]-outline1.xy[0])*(outline1.xy[6]-outline1.xy[0])));

                l = sizeof(NAGLOWEK) + ad->n;
                memmove(&tbuf, ad, l);

                if (Check_if_Equal(angle, angle1)==FALSE) tbuf.kat +=(angle1-angle);
                if (Check_if_Equal(lt, 0.0)==FALSE) {
                    if (Check_if_Equal(lt, lt1) == FALSE) {
                        tbuf.width_factor *= (lt1 / lt);
                        tbuf.width = 0;
                        tbuf.height = 0;
                    }
                }
                    if (Check_if_Equal(ht, 0.0)==FALSE) {
                        if (Check_if_Equal(ht, ht1) == FALSE) {
                            tbuf.wysokosc *= (ht1 / ht);
                            tbuf.width_factor *= (ht / ht1);
                            tbuf.width = 0;
                            tbuf.height = 0;
                        }
                }

                xy=map_point(tbuf.x, tbuf.y);
                tbuf.x=xy.x; tbuf.y=xy.y;
                rysuj_obiekt_((char *) &tbuf, mode, kolor);
            }
            else if (ad->obiekt==Opoint) {
                l = sizeof(NAGLOWEK) + ad->n;
                memmove(&pbuf, ad, l);
                xy=map_point(pbuf.x, pbuf.y);
                pbuf.x=xy.x; pbuf.y=xy.y;
                rysuj_obiekt_((char *) &pbuf, mode, kolor);
            }
        }
        obiekt_tok(NULL,adk,(char **) &ad,ONieOkreslony);
    }
    e:
    okno_all();
    //Set_Screen();
}

void blokquadfix(char  *adp,char  *adk,int atrybut,int mode, int kolor)
{
    NAGLOWEK *ad;
    LINIA *buf;
    TEXT *tbuf;
    SPLINE *spbuf;
    WIELOKAT *sbuf;
    T_Point *pbuf;
    BLOK *bbuf;
    POINTF xy;
    int i;
    QUAD outline, outline1;
    double angle, angle1;
    double lt, lt1, ht, ht1;
    T_Desc_Ex_Block *ptrs_desc_bl;


    if (adp == NULL || adk == NULL)
    {
        return;
    }

    Set_Second_Screen();
    okno_r_second();

    setwritemode(COPY_PUT);
    obiekt_tok((char*)adp,adk,(char **) &ad,ONieOkreslony);
    while (ad!=NULL)
    {
        ad->widoczny=1;

        if (ad->widoczny && TRUE == Check_Attribute (ad->atrybut, atrybut)) {
            if (ad->obiekt==OdBLOK) {
                bbuf=ad;
                if (bbuf->dlugosc_opisu_obiektu>17)
                {
                    ptrs_desc_bl = (T_Desc_Ex_Block*)(&bbuf->opis_obiektu[0]);
                    xy=map_point(ptrs_desc_bl->x, ptrs_desc_bl->y);
                    ptrs_desc_bl->x=xy.x; ptrs_desc_bl->y=xy.y;
                }
            }
            else if (ad->obiekt==Olinia) {
                buf=(LINIA*)ad;
                xy=map_point(buf->x1, buf->y1);
                buf->x1=xy.x; buf->y1=xy.y;
                xy=map_point(buf->x2, buf->y2);
                buf->x2=xy.x; buf->y2=xy.y;
                rysuj_obiekt_((char*)buf, mode, kolor);
            }
            else if (ad->obiekt==Ospline)
            {
                spbuf=(SPLINE*)ad;

                for (i=0; i<spbuf->lp; i+=2)
                {
                    xy=map_point(spbuf->xy[i], spbuf->xy[i+1]);
                    spbuf->xy[i]=xy.x; spbuf->xy[i+1]=xy.y;
                }
                rysuj_obiekt_((char *)spbuf, mode, kolor);
            }
            else if (ad->obiekt==Owwielokat)
            {
                sbuf=(WIELOKAT*)ad;

                for (i=0; i<(sbuf->lp); i+=2)
                {
                    xy=map_point(sbuf->xy[i], sbuf->xy[i+1]);
                    sbuf->xy[i]=xy.x; sbuf->xy[i+1]=xy.y;
                }
                Normalize_Solid(sbuf);
                rysuj_obiekt_((char *)sbuf, mode, kolor);
            }
            else if (ad->obiekt==Otekst)
            {
                outlinetext((TEXT*)ad, &outline, 0.0);
                angle=atan2(outline.xy[3]-outline.xy[1], outline.xy[2]-outline.xy[0]);
                lt=sqrt(((outline.xy[3]-outline.xy[1])*(outline.xy[3]-outline.xy[1]))+((outline.xy[2]-outline.xy[0])*(outline.xy[2]-outline.xy[0])));
                ht=sqrt(((outline.xy[7]-outline.xy[1])*(outline.xy[7]-outline.xy[1]))+((outline.xy[6]-outline.xy[0])*(outline.xy[6]-outline.xy[0])));

                xy=map_point(outline.xy[0], outline.xy[1]);
                outline1.xy[0]=xy.x; outline1.xy[1]=xy.y;
                xy=map_point(outline.xy[2], outline.xy[3]);
                outline1.xy[2]=xy.x; outline1.xy[3]=xy.y;
                xy=map_point(outline.xy[4], outline.xy[5]);
                outline1.xy[4]=xy.x; outline1.xy[5]=xy.y;
                xy=map_point(outline.xy[6], outline.xy[7]);
                outline1.xy[6]=xy.x; outline1.xy[7]=xy.y;

                //calculating the angle change
                angle1=atan2(outline1.xy[3]-outline1.xy[1], outline1.xy[2]-outline1.xy[0]);
                lt1=sqrt(((outline1.xy[3]-outline1.xy[1])*(outline1.xy[3]-outline1.xy[1]))+((outline1.xy[2]-outline1.xy[0])*(outline1.xy[2]-outline1.xy[0])));
                ht1=sqrt(((outline1.xy[7]-outline1.xy[1])*(outline1.xy[7]-outline1.xy[1]))+((outline1.xy[6]-outline1.xy[0])*(outline1.xy[6]-outline1.xy[0])));

                tbuf=(TEXT*)ad;

                if (Check_if_Equal(angle, angle1)==FALSE) tbuf->kat +=(angle1-angle);
                if (Check_if_Equal(lt, 0.0)==FALSE) {
                    if (Check_if_Equal(lt, lt1) == FALSE) {
                        tbuf->width_factor *= (lt1 / lt);
                        tbuf->width = 0;
                        tbuf->height = 0;
                    }
                }
                if (Check_if_Equal(ht, 0.0)==FALSE) {
                    if (Check_if_Equal(ht, ht1) == FALSE) {
                        tbuf->wysokosc *= (ht1 / ht);
                        tbuf->width_factor *= (ht / ht1);
                        tbuf->width = 0;
                        tbuf->height = 0;
                    }
                }

                xy=map_point(tbuf->x, tbuf->y);
                tbuf->x=xy.x; tbuf->y=xy.y;
                rysuj_obiekt_((char *) tbuf, mode, kolor);

                if (PTRS__Text_Style[tbuf->czcionka]->type==2)
                     TTF_redraw=TRUE;
            }
            else if (ad->obiekt==Opoint) {
                pbuf=(T_Point*)ad;
                xy=map_point(pbuf->x, pbuf->y);
                pbuf->x=xy.x; pbuf->y=xy.y;
                rysuj_obiekt_((char*)pbuf, mode, kolor);
            }
        }
        obiekt_tok(NULL,adk,(char **) &ad,ONieOkreslony);
    }

    okno_all_second();
    Set_Screen();
}

void blokzap(char  *adp,char  *adk,int atrybut,int mode, int kolor)
{
  NAGLOWEK *ad;

  reset_dane0();

 if (adp == NULL || adk == NULL)
 {
   return;
 }

  Set_Second_Screen();

  setwritemode(COPY_PUT);
  obiekt_tok((char*)adp,adk,(char **) &ad,ONieOkreslony);
  while (ad!=NULL)
  {
    if (ad->widoczny && TRUE == Check_Attribute (ad->atrybut, atrybut))
    rysuj_obiekt_((char*)ad,mode,kolor);
    obiekt_tok(NULL,adk,(char **) &ad,ONieOkreslony);
    if ((get_pattern_count() == TRUE) && (pattern_add_limit > 0) && ((long_long)ad > pattern_add_limit))
    {
        set_pattern_count(FALSE);
        pattern_add_limit = 0;
        set_pattern_offset(0);
    }
  }

  Set_Screen();
 }

void blokzap_v(char  *adp, char  *adk, int atrybut, int mode, int kolor)
{
	NAGLOWEK *ad;
    BITMAP *screenplay=Get_Screenplay();

    Set_Screenplay(screenplay);

	if (adp == NULL || adk == NULL)
	{
		return;
	}
	setwritemode(mode);
    //set_global_hidden_blocks_visibility(TRUE);
	obiekt_tok((char*)adp, adk, (char **)&ad, ONieOkreslony);
	while (ad != NULL)
	{
		if (TRUE == Check_Attribute(ad->atrybut, atrybut))
			rysuj_obiekt_((char*)ad, mode, kolor);
		obiekt_tok(NULL, adk, (char **)&ad, ONieOkreslony);
		if ((get_pattern_count() == TRUE) && (pattern_add_limit > 0) && ((long_long)ad > pattern_add_limit))
		{
			set_pattern_count(FALSE);
			pattern_add_limit = 0;
			set_pattern_offset(0);
		}
	}
    //set_global_hidden_blocks_visibility(FALSE);
}

void blokzap_no_pcx(char  *adp,char  *adk,int atrybut,int mode, int kolor)
{
  NAGLOWEK *ad;

 if (adp == NULL || adk == NULL)
 {
   return;
 }

  Set_Second_Screen();
  setwritemode(COPY_PUT);

  obiekt_tok((char*)adp,adk,(char **) &ad,ONieOkreslony);
  while (ad!=NULL)
  {
    if (ad->widoczny && TRUE == Check_Attribute (ad->atrybut, atrybut))
    rysuj_obiekt_no_pcx((char*)ad,mode,kolor);
    obiekt_tok(NULL,adk,(char **) &ad,ONieOkreslony);
	if ((get_pattern_count() == TRUE) && (pattern_add_limit > 0) && ((long_long)ad > pattern_add_limit))
	{
		set_pattern_count(FALSE);
		pattern_add_limit = 0;
		set_pattern_offset(0); ////????
	}
  }
  Set_Screen();
}


void out_blok(void)
{
 NAGLOWEK *ad;

 if (ADP == NULL || ADK == NULL)
 {
   Internal_Error (__LINE__,__FILE__);
   return;
 }
 if(mvcurb.akton) { mvcurb.mvc=0;mvcurbp(0);mskp();}
 okno_r();
 setwritemode(COPY_PUT);
 obiekt_tok((char*)ADP,ADK,(char **) &ad,ONieOkreslony);
 while (ad!=NULL)
  { if( mvcurb.akton && mvcurb.mvc )
	 goto e;
    if( mvcurb.aktoff && !mvcurb.L && !mvcurb.T)
		goto e;
    if(ad->atrybut==Ablok && (ad->obiekt==Olinia || ad->obiekt == Oluk || ad->obiekt == Owwielokat || ad->obiekt == Okolo || ad->obiekt==Otekst) )
      rysuj_obiekt_((char*)ad,XOR_PUT,1);
    obiekt_tok(NULL,ADK,(char **) &ad,ONieOkreslony);
  }
e: okno_all();
}

int Solid04(char *adr0, char *adr1, int kolor)
/*-----------------------------------------------------------------------------*/
{
  //int k ;
  WIELOKAT *Solid;
  B_PCX *pcx;
  double width, height;
  //double width_d;
  REAL_PCX *rpcx;
  double pcx_x;
  double kos,koc;

  pcx=(B_PCX *)adr0;
  rpcx = (REAL_PCX*) pcx->pcx;
  if (rpcx->header.bits_per_pixel ==  8)
   {
     width  = rpcx->header.xmax - rpcx->header.xmin + 1;         // image dimensions...
     height = rpcx->header.ymax - rpcx->header.ymin + 1;
     pcx_x=pcx->x;
   }
    else
     {
       width  = (((rpcx->header.xmax - rpcx->header.xmin)+8)/8)*8;         // image dimensions...
       height= (rpcx->header.ymax - rpcx->header.ymin)+1;
       pcx_x=pcx->x;
     }

  width *= pcx->dx;
  height *= pcx->dy;

  Solid=(WIELOKAT *)adr1;
  Solid->warstwa = pcx->warstwa ;
  Solid->blok=1;
  Solid->obiektt1 = 0;
  Solid->obiektt2 = O2BlockPline;
  Solid->kolor = kolor ;
  Solid->empty_typ=4;  //linia bardzo gruba //linia najgrubsza
  Solid->lp = 8 ;
  Solid->n = 8 + Solid->lp * sizeof (float) ;


  if (Check_if_Equal(pcx->kat,0.0)==TRUE)
   {
    Solid->xy [0] = pcx_x ;
    Solid->xy [1] = pcx->y ;
    Solid->xy [2] = pcx_x ;
    Solid->xy [3] = pcx->y - height ;
    Solid->xy [4] = pcx_x + width ;
    Solid->xy [5] = pcx->y - height ;
    Solid->xy [6] = pcx_x + width;
    Solid->xy [7] = pcx->y;
   }
    else
     {
       kos=sin(pcx->kat);
       koc=cos(pcx->kat);

       Solid->xy [0] = pcx_x ;
       Solid->xy [1] = pcx->y ;

       Solid->xy [2] = pcx_x + height * kos ;
       Solid->xy [3] = pcx->y - height * koc ;

       Solid->xy [4] = pcx_x + width * koc + height * kos ;
       Solid->xy [5] = pcx->y + width * kos - height * koc ;

       Solid->xy [6] = pcx_x + width * koc ;
       Solid->xy [7] = pcx->y + width * kos ;
      }
//  Normalize_Solid (Solid) ;
  return 1 ;
}


int get_quad(char *ad, int ad_buf_no, AD_BUF *ad_buf, QUAD *quad, BOOL *convex)
{
    BLOK *b=NULL;
    LINIA *L;
    POINTF p[8];
    int ni = 0;
    char *adp, *adk;
    BOOL found;

    b = LASTB(ad);

    if ((b !=NULL) && (b->kod_obiektu==B_PLINE) && (b->opis_obiektu[0]==PL_RECTANGLE))
    {
        //counting lines
        adp = (char*)b + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
        adk = (char*)b + sizeof(NAGLOWEK) + b->n - 1;
        while (adp<adk)
            {
               if (((NAGLOWEK *)adp)->obiekt==Olinia) {
                   if (ni < 8) {
                       //searching ad_buf
                       found=FALSE;
                       for (int i=0; i<ad_buf_no; i++)
                       {
                           if (adp==ad_buf[i].ad)
                           {
                               p[ni].x = ad_buf[i].p[0].x;
                               p[ni].y = ad_buf[i].p[0].y;
                               ni++;
                               p[ni].x = ad_buf[i].p[1].x;
                               p[ni].y = ad_buf[i].p[1].y;
                               ni++;
                               found=TRUE;
                               break;
                           }
                       }
                       L = (LINIA *) adp;
                       if (!found) {
                           p[ni].x = L->x1;
                           p[ni].y = L->y1;
                           ni++;
                           p[ni].x = L->x2;
                           p[ni].y = L->y2;
                           ni++;
                       }
                       adp += sizeof(NAGLOWEK) + L->n;
                   } else {
                       ni = 0;
                       break;
                   }
               }
            }
    }
    if (ni==8)
    {
        // use nested for loop to find the duplicate elements in array
        for (int i = 0; i<ni;i ++)
        {
            for (int j = i + 1;j<ni;j++)
            {
            // use if statement to check duplicate element
            if (
                Check_if_Equal(p[i].x, p[j].x) &&
                Check_if_Equal(p[i].y, p[j].y))
                {
                    // delete the current position of the duplicate element
                    for (int k = j;k < ni - 1; k++)
                    {
                        p[k].x = p[k + 1].x;
                        p[k].y = p[k + 1].y;
                    }
                    // decrease the size of array after removing duplicate element
                    ni--;
                    // if the position of the elements is changes, don't increase the index j
                    j--;
                }
            }
        }
    }
    else return 0;
    if (ni==4) //only 4 vertices stay
    {
        for (int i = 0;i<ni;i++)
        {
            quad->xy[2*i] = p[i].x;
            quad->xy[2*i+1] = p[i].y;
        }
        *convex = Check_ifConvex(quad);
        return ni;
    }
    else return 0;
}

void out_blok2 (double x,double y,double k1,double k2,
	     double xa, double ya, double ka1, double ka2, int trans, double z, BOOL update_par, int no_object)
/*-------------------------------------------------------------------------------------------------------*/
{ NAGLOWEK *ad, *adl;
  WIELOKAT *w;
  BLOK *b=NULL;
  LINIA *L;
  POINTF p[8];
  int l;
  static char buf[MaxSizeObiekt];
  WIELOKAT S4=Stdef;
  char keym;
  BOOL retb;
  int ni1=0, ni2=0;
  int ad_buf_no=0;
  AD_BUF ad_buf[4];
  BOOL convex0, convex1;

 if (ADP == NULL || ADK == NULL)
 {
   return;
 }
 if(mvcurb.akton) { mvcurb.mvc=0;mvcurbp(0);mskp();}
 okno_r();
 oknoS(Xp,Yp,Xk,Yk);
 setwritemode(COPY_PUT);
 obiekt_tok((char*)ADP,ADK,(char **) &ad,ONieOkreslony);
 
 while (ad!=NULL)
  { if((ad->atrybut==Ablok) && (ad->obiekt != no_object))
     {

#ifdef LINUX
          keym=readmouse();
#else
          keym=readmouse();
#endif

       if( mvcurb.akton && mvcurb.mvc ) 
		   goto e;
       if( mvcurb.aktoff && !mvcurb.L && !mvcurb.T && !mvcurb.K &&
	   !mvcurb.O && !mvcurb.l && !mvcurb.W && !mvcurb.P)
           goto e;

       if(ad->obiekt==Olinia || ad->obiekt==Otekst || ad->obiekt==Okolo ||
	  ad->obiekt==Ookrag || ad->obiekt==Oluk ||ad->obiekt==Owwielokat || ad->obiekt == Ospline ||
	  ad->obiekt==Opoint ||
      ad->obiekt==Oellipticalarc || ad->obiekt==Oellipse || ad->obiekt== Ofilledellipse || ad->obiekt== Osolidarc || ad->obiekt==Ovector ||
      ad->obiekt==Opcx)

	{ l=sizeof(NAGLOWEK)+ad->n;
     if (ad->obiekt==Opcx)
       {
        //zdefiniowanie obszaru prostokatnego
        Solid04((char *)ad, (char *)&S4, kolory.blok);
        if (trans==Tobrot) transformacja_obiekt((char *)&S4,x,y,k1,k2,xa, ya, ka1, ka2, TobrotPCX,z);
          else transformacja_obiekt((char *)&S4,x,y,k1,k2,xa, ya, ka1, ka2, trans,z);
           S4.empty_typ=5;
           rysuj_wypelnienie_(&S4,COPY_PUT,1,0,TRUE,FALSE);
       }
        else
         {
            memmove(buf,ad,l);

            if (get_dragging_quad()) {
                if (((NAGLOWEK *) ad)->obiekt == Owwielokat) {
                    w = (WIELOKAT *) ad;
                    if (w->lp == 8) memcpy(quad0.xy, w->xy, 8 * sizeof(float));
                    else {
                        memcpy(quad0.xy, w->xy, 6 * sizeof(float));
                        quad0.xy[6] = w->xy[4];
                        quad0.xy[7] = w->xy[5];
                    }
                    ni1=4;
                }
                else if (((NAGLOWEK *) ad)->obiekt == Olinia)
                {
                    adl=ad;
                }
            }

	        transformacja_obiekt(buf,x,y,k1,k2,xa, ya, ka1, ka2, trans,z);

             if (get_dragging_quad()) {
                 if (((NAGLOWEK *) ad)->obiekt == Owwielokat) {
                     w = (WIELOKAT *) buf;
                     if (w->lp == 8) memcpy(quad1.xy, w->xy, 8 * sizeof(float));
                     else {
                         memcpy(quad1.xy, w->xy, 6 * sizeof(float));
                         quad1.xy[6] = w->xy[4];
                         quad1.xy[7] = w->xy[5];
                     }
                     ni2=4;
                 }
                 else if (((NAGLOWEK *) buf)->obiekt == Olinia)
                 {
                     if (ad_buf_no<4) {
                         ad_buf[ad_buf_no].ad=ad;
                         ad_buf[ad_buf_no].p[0].x=((LINIA*)buf)->x1;
                         ad_buf[ad_buf_no].p[0].y=((LINIA*)buf)->y1;
                         ad_buf[ad_buf_no].p[1].x=((LINIA*)buf)->x2;
                         ad_buf[ad_buf_no].p[1].y=((LINIA*)buf)->y2;
                         ad_buf_no++;
                     }
                 }
             }

             if (((NAGLOWEK*)buf)->obiekt==Olinia)
             {
                 if (update_par)
                 {
                     ((LINIA *) buf)->kolor = LiniaG.kolor;
                     ((LINIA *) buf)->typ = LiniaG.typ;
                 }
                 if (((((LINIA *) buf)->obiektt1)==2) && ((((LINIA *) buf)->obiektt2)==1) && ((((LINIA *) buf)->obiektt3)==1))
                 {
                     memmove(last_buf, buf, sizeof(NAGLOWEK) + (((LINIA *) buf)->n));
                 }
             }

             rysuj_obiekt_(buf,COPY_PUT,1);

         }
	}
     }
    obiekt_tok(NULL,ADK,(char **) &ad,ONieOkreslony);
  }
  //setting quads if any
    if (get_dragging_quad()) {
        if ((ni1!=4) && (ni2!=4))  //is now solid
        {
            if (ad_buf_no>0)  //polyline
            {
                ni1 = get_quad(adl, 0, NULL, &quad0, &convex0);
                ni2 = get_quad(adl, ad_buf_no, ad_buf, &quad1, &convex1);
            }
        }
        else //solid
        {
            convex0 = Check_ifConvex(&quad0);
            convex1 = Check_ifConvex(&quad1);
        }

        if ((ni1==4) && (ni2==4) && (convex0==convex1))  retb = quadtoquadmatrix(quad0, quad1);
    }


e: okno_all();
}

void out_blok1(double x,double y,double k1,double k2,int trans,double z)
/*-----------------------------------------------------------------------------*/
{
  out_blok2 (x, y, k1, k2, 0, 0, 0, 0, trans,z, FALSE, -1) ;
}

