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
#include<stddef.h>
//#include<graphics.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include "bib_e.h"
#include "b_sel.h"
#include "rysuj_e.h"
#include "b_textw.h"
#include "o_libfun.h"
#include "o_object.h"
#include "o_osnap.h"
#include "alffont.h"
#include "alf_res.h"

static long X1sel = 0;
static long Y1sel = 0;
static long X2sel = 0;
static long Y2sel = 0;
static double X1, Y1, X2, Y2;

#define noopM (void (*)(int x,int y))nooop
#define noopS (int (*)(TMENU  *menu))nooop1

#ifndef BIGNOF
#define BIGNOF 1e20
#endif

extern BOOL INTL_CURSOR;
extern float BRIGHTNESS_DIFF;
extern GrContext *get_second_screen(void);

extern double Ds_x ;
extern double rpp ;
extern double skala_p_x ;
extern int znacznik_aplikacji ;
extern double dynamic_block ;

extern BOOL global_point_in_solid;

extern void setwritemode( int mode );
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);
extern void setcolor(int kolor);
extern void rectangle(int left, int top, int right, int bottom);
extern int Point_Selected (T_Point *ptrs_point);
extern int Vector_Selected (AVECTOR *ptrs_vector);
extern double Get_Text_Len(TEXT *ptrs_text, char *ptrsz_t, double font_scale, int font_index, int *t_width, int *t_height, double *matrix_d, int *i_matrix_d);
extern T_Font_Style s__Fonts_Style[MaxNumberTextStyle];
extern void out_cur_off(double X, double Y);
extern void out_cur_on(double X, double Y);
extern void calc_bspline(int lp, int n, float points[], int p1, float* out_x, float* out_y);
extern void out_blok2_pcx(double x, double y, double k1, double k2,
	double xa, double ya, double ka1, double ka2, int trans, double z);
extern void calc_tangents(int closed, int* node_count, double* bnodes_2_tangent);
extern void set_bnodes(SPLINE* ptrs_spline, float *xy, int* node_count_);
extern void get_control_points(NODE n1, NODE n2, float points[]);
extern void get_control_points_reversed(NODE n1, NODE n2, float points[]);
extern void get_control_points_end(NODE n1, NODE n2, float points[]);
extern void draw_mickey_hand(double x, double y);
extern int get_pXp(void);
extern int get_pXk(void);
extern int get_pYp(void);
extern int get_pYk(void);

static int  tab_ixy1[MaxNumPoints], lp;
static int test_circle_sel(OKRAG *ad);

extern NODE bnodes[MAX_NODES];
extern double curviness;

extern int Get_Ellipse_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double xy[]);
extern int Get_EllipticalArc_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double df_sangle, double df_eangle, double xy[]);

extern void linen(long X1, long Y1, long X2, long Y2, int mode);

extern int get_3p_arc(LUK *l, POINTD *p1, POINTD *p2, POINTD *p3);

extern double depth_magnitude; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double thermal_magnitude; //units per mm  default 1 Celsius per 1 mm on drawing paper
extern double load_magnitude; //units per mm  default 10kN/m load per 1 mm on drawing paper
extern double flood_magnitude; //units per mm  default 10kN/m² load per 1 mm on drawing paper
extern double shear_magnitude; //units per mm  default 10kN/m shear/reaction per 1 mm on drawing paper
extern double force_magnitude; //units per mm  default 10kN force per 1 mm on drawing paper
extern double moment_magnitude; //units per mm  default 10kNm force per 1 mm radius on drawing paper
extern double displacement_magnitude; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
extern double rotation_magnitude;
extern void create_solid_on_line (LINIA *L, WIELOKAT *s, double width1, double width2, double axis);
extern void set_decimal_format(char *text, double l, double precision);
extern double normalize_txt_angle(double angle0);
extern void normalize_txt(TEXT *ptrs_text);
extern char *load_symbol[];

extern double dim_precision;
extern double force_precision;
extern double displacement_precision;
extern double moment_precision;
extern double rotation_precision;
extern double load_precision;

static int vector_filter=-1;

void set_vector_filter(int vf)
{
    vector_filter=vf;
}

void set_XY(double del_minx, double del_miny, double del_maxx, double del_maxy)
{
	X1 = del_minx;
	Y1 = del_miny;
    X2 = del_maxx;
    Y2 = del_maxy;

	//oknoS(del_minx, del_miny, del_maxx, del_maxy);
}

void get_XY(double *del_minx, double *del_miny, double *del_maxx, double *del_maxy)
{
    *del_minx = X1;
    *del_miny = Y1;
    *del_maxx = X2;
    *del_maxy = Y2;
}


int linia_frozen(LINIA *ad0, char frozen)
{ 
	if (Layers[ad0->warstwa].edit==frozen) return 1;
	else return 0;
}

int tekst_frozen(TEXT *ad0, char frozen)
{ 
	if (Layers[ad0->warstwa].edit==frozen) return 1;
	else return 0;
}

int okrag_frozen(OKRAG *ad0, char frozen)
{ 
	if (Layers[ad0->warstwa].edit==frozen) return 1;
	else return 0;
}

int point_frozen(T_Point *ad0, char frozen)
{
    if (Layers[ad0->warstwa].edit==frozen) return 1;
    else return 0;
}

int ellipse_frozen(ELLIPSE *ad0, char frozen)
{
    if (Layers[ad0->warstwa].edit==frozen) return 1;
    else return 0;
}

int ellipticalarc_frozen(ELLIPTICALARC *ad0, char frozen)
{
    if (Layers[ad0->warstwa].edit==frozen) return 1;
    else return 0;
}

int wielokat_frozen(WIELOKAT *ad0, char frozen)
{ 
	if (Layers[ad0->warstwa].edit==frozen) return 1;
	else return 0;
}

int solidarc_frozen(SOLIDARC *ad0, char frozen)
{
    if (Layers[ad0->warstwa].edit==frozen) return 1;
    else return 0;
}

int spline_frozen(SPLINE *ad0, char frozen)
{
	if (Layers[ad0->warstwa].edit == frozen) return 1;
	else return 0;
}

int vector_frozen(AVECTOR *ad0, char frozen)
{
    if (Layers[ad0->warstwa].edit == frozen) return 1;
    else return 0;
}

int luk_frozen(LUK *ad0, char frozen)
{ 
	if (Layers[ad0->warstwa].edit==frozen) return 1;
	else return 0;
}

long jednostkiN(double jednostki)
{ return /*Double_to_Long*/(long)(jednostki * Ds_x) ;}

void oknoS(double x1,double y1,double x2,double y2)
{
  X1sel=jednostkiN(X1=x1);
  X2sel=jednostkiN(X2=x2);
  Y1sel=jednostkiN(Y1=y1);
  Y2sel=jednostkiN(Y2=y2);

  //Set_Draw_Window (X1, Y1, X2, Y2);
}

void Get_oknoS(RECTD *rectd)
{
    rectd->xp=X1;
    rectd->xk=X2;
    rectd->yp=Y1;
    rectd->yk=Y2;
}

void Change_oknoS(double dx1,double dy1,double dx2,double dy2)
{ X1sel=jednostkiN(X1+=dx1);
  X2sel=jednostkiN(X2+=dx2);
  Y1sel=jednostkiN(Y1+=dy1);
  Y2sel=jednostkiN(Y2+=dy2);
}



/*#define cursel_off cursel_on*/
void cursel_off__(double x,double y)  /* cursor dla sel akt i gor */
{
   flip_screen();
}

void cursel_off(double x,double y)  /* cursor dla sel akt i gor */
{
	out_cur_off(x,y);
}

void cursel_on(double x,double y)
{ out_cur_on(x,y);
}

void cursel_hand_on(double x, double y)
{
	out_cur_on(x, y);
	draw_mickey_hand(x, y);
}

void out_blok1_pcx(double x, double y, double k1, double k2, int trans, double z)
/*-----------------------------------------------------------------------------*/
{
	out_blok2_pcx(x, y, k1, k2, 0, 0, 0, 0, trans, z);
}

/*#define cursel_off cursel_on*/
void cursel_off_pcx(double x, double y)  /* cursor dla sel akt i gor */
{
	out_blok1_pcx(x, y, 0, 0, Tprzesuw, 0);
	out_cur_off(x, y);
}

void cursel_on_pcx(double x, double y)
{
	out_cur_on(x, y);
	out_blok1_pcx(x, y, 0, 0, Tprzesuw, 0);
    ////out_cur_on(x, y);
}

double Get_Point_Size (void)
/*--------------------------*/
{
  return df__Point_Size ;
}


void Set_Point_Size (double df_ps)
/*------------------------------*/
{
  df__Point_Size = df_ps ;
}


BOOL Get_Point_View (void)
/*-----------------------*/
{
  return b__Point_View ;
}

BOOL Get_Control_Points_View(void)
/*-----------------------------*/
{
	return b__Control_Points_View;
}

BOOL Get_Bitmap_Outline(void)
/*-----------------------------*/
{
	return b__Bitmap_Outline;
}


void Set_Point_View (BOOL b_pv)
/*------------------------------*/
{
  b__Point_View = b_pv ;
}

void Set_Control_Points_View(BOOL b_pv)
/*----------------------------------*/
{
	b__Control_Points_View = b_pv;
}

void Set_Bitmap_Outline(BOOL b_pv)
{
	b__Bitmap_Outline = b_pv;
}

static int near Inside(long x,long x1,long x2)
{ int ix;
  ix=0;
  if (x<x1) ix=-1;
  else if (x>x2) ix=1;
  return ix;
}

static int ix1,ix2,iy1,iy2;	/*dla wstepnego oszacowania czy punkt jest wewnatrz wielokata*/
static int near prostokat_odcinek(long x1,long y1,long x2,long y2)
{
  long xx,yy;
  ix1=Inside(x1,X1sel,X2sel);
  ix2=Inside(x2,X1sel,X2sel);
  iy1=Inside(y1,Y1sel,Y2sel);
  iy2=Inside(y2,Y1sel,Y2sel);
  if (ix1*ix2==1 || iy1*iy2==1) return 0;
  if (ix1==0 && iy1==0 && ix2==0 && iy2==0) return 3;
  if (ix1==0 && iy1==0) return 1;
  if (ix2==0 && iy2==0) return 2;
  if (ix1)
   { xx=ix1>0 ? X2sel : X1sel;
     y1=y1+(y2-y1)*((double)(xx-x1))/(x2-x1);
     x1=xx;
     iy1=Inside(y1,Y1sel,Y2sel);
     if (iy1*iy2==1) return 0;
   }
  if (iy1)
   { yy=iy1>0 ? Y2sel : Y1sel;
     x1=x1+(x2-x1)*((double)(yy-y1))/(y2-y1);
     ix1=Inside(x1,X1sel,X2sel);
     if (ix1) return 0;
   }
  return 4;
}


static int near punkty_pin_odcinek(LINIA *ad)
{ int i;
  int kod_powrotu;
  int max_kod_powrotu;
  /*sprawdzenie po wszystkich punktach PIN_TAB[] czy punktu x1,y1; x2,y2
    sa zgodne. Jezeli x1==PIN_TAB[].x i y1=PIN_TAB[].y to return 1.
    Jezeli x2==PIN_TAB[].x i y2=PIN_TAB[].y to return2.
    Jezeli obydwa, to return 3. W przeciwnym razie return 4*/
  kod_powrotu=0;  
  max_kod_powrotu=0;
  if (PIN_COUNT>0)
   { 
    for (i=1; i<=PIN_COUNT; i++)
     {
     kod_powrotu=0;  

     if ((TRUE==Check_if_Equal(ad->x1,PIN_TAB_X[i])) && (TRUE==Check_if_Equal(ad->y1,PIN_TAB_Y[i])))
       kod_powrotu+=1;
     if ((TRUE==Check_if_Equal(ad->x2,PIN_TAB_X[i])) && (TRUE==Check_if_Equal(ad->y2,PIN_TAB_Y[i])))
       kod_powrotu+=2;
      if (kod_powrotu>max_kod_powrotu)
      {
       max_kod_powrotu=kod_powrotu;
      }    
     }  
   } 
  return max_kod_powrotu;
}

int punkt_w_prostokacie(double x, double  y)
{ long x0,y0;
  x0=jednostkiN(x);
  y0=jednostkiN(y);
  return (X1sel<=x0 && x0<=X2sel && Y1sel<=y0 && y0<=Y2sel) ? 1 : 0 ;
}


int punkt_w_tekscie(TEXT  *ad0, double X1, double Y1)
{
	double Xr, Yr, X01, Y01, x01, y01, xt1, yt1;
	double addl, adsin, adcos;
	double WysokoscT;
	TEXT ad1;
	TEXT *ad;

	adsin = sin(ad0->kat);
	adcos = cos(ad0->kat);
	if (TRUE == Get_Point_View())
	{
		if ((strlen(ad0->text) == 0) && (ad0->typ > 2))
		{
			memmove(&ad1, ad0, sizeof(TEXT_NAG));
			ad1.text[0] = 255;
			ad1.text[1] = '\0';
			ad = &ad1;
		}
		else ad = ad0;
	}
	else ad = ad0;

	addl = Get_TextLen(ad, NULL);

  WysokoscT = ad->wysokosc;
  x01 = ad->x ;
  y01 = ad->y ;
  
    switch (ad->justowanie)
     {
  case 1: xt1 = x01 - addl * adcos ;  //do prawej
          yt1 = y01 - addl * adsin ; 
          x01 = xt1;
          y01 = yt1;
          break;
  case 2: xt1 = x01 - (addl/2) * adcos ;  //srodkowo
          yt1 = y01 - (addl)/2 * adsin ; 
          x01 = xt1;
          y01 = yt1;
          break;
  case 3: xt1 = x01 - (addl/2) * adcos + (WysokoscT/2) * adsin;  //centralnie
          yt1 = y01 - (addl)/2 * adsin - (WysokoscT/2) * adcos; 
          x01 = xt1;
          y01 = yt1;
          break;   
  default: break;             
     }  
  
  X01=X1-x01/*ad->x*/;   Y01=Y1-y01/*ad->y*/;
  Xr=X01*adcos-Ky*Y01*adsin;
  Yr=+Ky*X01*adsin+Y01*adcos;
  if(0<=Xr && Xr<=addl && ad->wysokosc>=Yr && Yr>=0 ) return 1;
  return 0;
}

int linia_w_prostokacie(LINIA *ad)
{ return (punkt_w_prostokacie(ad->x1,ad->y1) &&
	  punkt_w_prostokacie(ad->x2,ad->y2));
}

int luk_pomX(LUK *ad,double xx,double Y1,double Y2)
{ double alf1,alf2,ypom,yy,co,a1,a2;
  int k1,k2;
  co=(xx-ad->x)/ad->r;
  if(fabs(co)>1) return 0;
  alf1=acos(co);
  alf2=Pi2-alf1;
  a1=ad->kat1;
  a2=ad->kat2;
  if(fabs(a1)>=Pi2) a1-=floor(0.5+a1/Pi2)*Pi2;
  if(fabs(a2)>=Pi2) a2-=floor(0.5+a2/Pi2)*Pi2;
  if(a1<0) a1+=Pi2;
  if(a2<0) a2+=Pi2;
  if(a1>a2) a2+=Pi2;
  a2-=a1;
  alf1-=a1;
  alf2-=a1;
  if(fabs(alf1)>=Pi2) alf1-=floor(0.5+alf1/Pi2)*Pi2;
  if(fabs(alf2)>=Pi2) alf2-=floor(0.5+alf2/Pi2)*Pi2;
  if(alf1<0) alf1+=Pi2;
  if(alf2<0) alf2+=Pi2;
  k1=(alf1>=0 && alf1<=a2);
  k2=(alf2>=0 && alf2<=a2);
  if(k1 || k2)
   { ypom=sqrt (fabs (ad->r*ad->r-(xx-ad->x)*(xx-ad->x)));
     if (k1)
      { yy=ad->y+ypom;
	if (Y1<=yy && yy<=Y2) return 1;
      }
     if (k2)
      { yy=ad->y-ypom;
        if (Y1<=yy && yy<=Y2) return 1;
      }
   }
  return 0;
}

int luk_pomY(LUK *ad,double yy,double X1,double X2)
{ double alf,alf1,alf2,xpom,xx,si,a1,a2;
  int k1,k2;
  si=(yy-ad->y)/ad->r;
  if(fabs(si)>1) return 0;
  alf=asin(si);
  if (alf>=0) { alf1=alf;alf2=Pi-alf; }
  else { alf2=Pi-alf; alf1=Pi2+alf;  }
  a1=ad->kat1;
  a2=ad->kat2;
  if(fabs(a1)>=Pi2) a1-=floor(0.5+a1/Pi2)*Pi2;
  if(fabs(a2)>=Pi2) a2-=floor(0.5+a2/Pi2)*Pi2;
  if(a1<0) a1+=Pi2;
  if(a2<0) a2+=Pi2;
  if(a1>a2) a2+=Pi2;
  a2-=a1;
  alf1-=a1;
  alf2-=a1;
  if(fabs(alf1)>=Pi2) alf1-=floor(0.5+alf1/Pi2)*Pi2;
  if(fabs(alf2)>=Pi2) alf2-=floor(0.5+alf2/Pi2)*Pi2;
  if(alf1<0) alf1+=Pi2;
  if(alf2<0) alf2+=Pi2;
  k1=(alf1>=0 && alf1<=a2);
  k2=(alf2>=0 && alf2<=a2);
  if(k1 || k2)
   { xpom=sqrt(fabs (ad->r*ad->r-(yy-ad->y)*(yy-ad->y)));
     if (k1)
      { xx=ad->x+xpom;
	if (X1<=xx && xx<=X2) return 1;
      }
     if (k2)
      { xx=ad->x-xpom;
        if (X1<=xx && xx<=X2) return 1;
      }
   }
  return 0;
}


int okrag_w_prostokacie(OKRAG *ad)
{ long x,y,r;
  x=jednostkiN(ad->x);
  y=jednostkiN(ad->y);
  r=jednostkiN(ad->r);
  return  (X1sel<=x-r && X2sel>=x+r &&
	   Y1sel<=y-r && Y2sel>=y+r); }

/*
int qsort_by_val(double *e1, double *e2)
 { int delta;
  delta=(*e1) - (*e2);
  return delta;
 }
*/

int qsort_by_val(const void *e1, const void *e2)
{ int delta;
    delta=(*(const double*)e1) - (*(const double*)e2);
    return delta;
}

int pcx_w_prostokacie(B_PCX *ad)
{ double x_[4],y_[4];
  B_PCX *pcx;
  double width, height;
  REAL_PCX *rpcx;
  //double width_d;
  double pcx_x;
  double x_min, x_max, y_min, y_max;
  double kos, koc;

  pcx=(B_PCX *)ad;
  rpcx = (REAL_PCX*) pcx->pcx;

  if (rpcx->header.bits_per_pixel ==  8)
   {
     //width  = rpcx->header.xmax - rpcx->header.xmin + 1;         // image dimensions...
     //height = rpcx->header.ymax - rpcx->header.ymin + 1;
	  width = pcx->i_x;
	  height = pcx->i_y;
     pcx_x=pcx->x; // - pcx->dx;
   }
    else
     {
       //width  = (((rpcx->header.xmax - rpcx->header.xmin)+8)/8)*8;         // image dimensions...
       //height= (rpcx->header.ymax - rpcx->header.ymin)+1;
	    width = pcx->i_x;
	    height = pcx->i_y;
       pcx_x=pcx->x; // + (pcx->dx*8.0);
     }


  if (Check_if_Equal(pcx->kat,0.0)==TRUE)
   {
    x_min = pcx_x ;
    y_max = pcx->y ;
    x_max = pcx_x + width * pcx->dx;
    y_min = pcx->y - height * pcx->dy ;
  }
   else
   {
    kos=sin(pcx->kat);
    koc=cos(pcx->kat);

    width *= pcx->dx;
    height *= pcx->dy;

    x_[0] = pcx_x ;
    y_[0] = pcx->y ;

   Rotate_Point(kos,koc,x_[0],y_[0],x_[0]+width,y_[0], &x_[1], &y_[1]);
   Rotate_Point(kos,koc,x_[0],y_[0],x_[0]+width,y_[0]-height, &x_[2], &y_[2]);
   Rotate_Point(kos,koc,x_[0],y_[0],x_[0],y_[0]-height, &x_[3], &y_[3]);

#ifndef LINUX
    qsort(x_,4,sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);
    qsort(y_,4,sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);

    //qsort(x_, 4, sizeof(double),(int(*)(const double*, const double*)) qsort_by_val);
    //qsort(y_, 4, sizeof(double),(int(*)(const double*, const double*)) qsort_by_val);
#else
#ifndef MACOS
      qsort(x_, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
      qsort(y_, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
#else
      qsort(x_, 4, sizeof(double),  qsort_by_val);
       qsort(y_, 4, sizeof(double),  qsort_by_val);
#endif
#endif

    x_min=x_[0];
    y_min=y_[0];
    x_max=x_[3];
    y_max=y_[3];

  }

  if ((x_min>X2) || (x_max<X1) || (y_min>Y2) || (y_max<Y1)) return 0;

  return 1;
}

int pcx_wybrany(B_PCX *ad, BOOL printing)
{ double x1,y1,x2,y2,x3,y3,x4,y4;
  B_PCX *pcx;
  double width, height;
  REAL_PCX *rpcx;
  double pcx_x;
  double kos, koc;
  long XSel, YSel;

  pcx=(B_PCX *)ad;
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

  kos=sin(pcx->kat);
  koc=cos(pcx->kat);

  width *= pcx->dx;
  height *= pcx->dy;

  x1 = pcx_x ;
  y1 = pcx->y ;

  x2 = pcx_x + height * kos ;
  y2 = pcx->y - height * koc ;

  x3 = pcx_x + width * koc + height * kos ;
  y3 = pcx->y + width * kos - height * koc ;

  x4 = pcx_x + width * koc ;
  y4 = pcx->y + width * kos ;

    //center of the selector
  XSel = (X1sel + X2sel) / 2;
  YSel = (Y1sel + Y2sel) / 2;

    if  (prostokat_odcinek(jednostkiN(x1),jednostkiN(y1),
			    jednostkiN(x2),jednostkiN(y2)) ||
         prostokat_odcinek(jednostkiN(x2),jednostkiN(y2),
 			    jednostkiN(x3),jednostkiN(y3)) ||
         prostokat_odcinek(jednostkiN(x3),jednostkiN(y3),
 			    jednostkiN(x4),jednostkiN(y4)) ||
         prostokat_odcinek(jednostkiN(x4),jednostkiN(y4),
			    jednostkiN(x1),jednostkiN(y1))) return 1;


	if (printing)
	{
		//now try point in solid  --> this is not necessary, image should be taken by side, not inside area
		WIELOKAT w = S4def;
		w.xy[0] = jednostkiN(x1);
		w.xy[1] = jednostkiN(y1);
		w.xy[2] = jednostkiN(x2);
		w.xy[3] = jednostkiN(y2);
		w.xy[4] = jednostkiN(x3);
		w.xy[5] = jednostkiN(y3);
		w.xy[6] = jednostkiN(x4);
		w.xy[7] = jednostkiN(y4);

		Normalize_Solid(&w);
		if (Point_in_Solid(&w, XSel, YSel)) return 1;
	}

   return 0;
}


int linia_wybrana(LINIA *ad)
{
	return prostokat_odcinek(jednostkiN(ad->x1),jednostkiN(ad->y1),
			    jednostkiN(ad->x2),jednostkiN(ad->y2)); }


int vector_magnitude_text_wybrany(TEXT *Vtxt, TEXT *Vtxt1, AVECTOR* v, LINIA *L)
{
    int ret=0;
    double n;
    LINIA Lt, Lth;
    double Ltx, Lty;
    PLINIA PL, PL1, PLth;
    double koc1, kos1, koc2, kos2, koc1th, kos1th;
    double dx, dy, dx1, dy1;
    float ymax, xmax;
    
    switch (v->style) {
        case 10:   //trapezium Y
        case 17:   //trapezium Y slab
            if (L->x1<L->x2) n=1;
            else n=-1;

            if (v->flags & 1) n*=-1;

            Lt.x1 = L->x1;
            Lt.y1 = L->y1 + n*(v->magnitude1/((v->style==10) ? load_magnitude : flood_magnitude));
            Lt.x2 = L->x2;
            Lt.y2 = L->y2 + n*(v->magnitude2/((v->style==10) ? load_magnitude : flood_magnitude));

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos1=sin(Pi*(PL1.kat+90)/180);
            koc1=cos(Pi*(PL1.kat+90)/180);

            if (Lt.x1<Lt.x2)
            {
                if (Lt.y1<L->y1)
                {
                    dx=-Vtxt->wysokosc*koc1;
                    dy=-Vtxt->wysokosc*kos1;
                }
                else
                {
                    dx = 0;
                    dy = 0;
                }

                if (Lt.y2<L->y2)
                {
                    dx1=-Vtxt->wysokosc*koc1;
                    dy1=-Vtxt->wysokosc*kos1;
                }
                else {
                    dx1 = 0;
                    dy1 = 0;
                }
            }
            else
            {
                if (Lt.y1>L->y1)
                {
                    dx = 0;
                    dy = 0;
                }
                else
                {
                    dx = Vtxt->wysokosc * koc1;
                    dy = Vtxt->wysokosc * kos1;
                }

                if (Lt.y2>L->y2)
                {
                    dx1 = 0;
                    dy1 = 0;
                }
                else
                {
                    dx1 = Vtxt->wysokosc * koc1;
                    dy1 = Vtxt->wysokosc * kos1;
                }
            }

            Vtxt->x=Lt.x1+dx;
            Vtxt->y=Lt.y1+dy;
            Vtxt->kat=normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt->justowanie=j_do_lewej;
            else Vtxt->justowanie=j_do_prawej;
            Vtxt1->x=Lt.x2+dx1;
            Vtxt1->y=Lt.y2+dy1;
            Vtxt1->kat=normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt1->justowanie=j_do_prawej;
            else  Vtxt1->justowanie=j_do_lewej;

            normalize_txt(Vtxt);
            if (tekst_wybrany(Vtxt)) return 1;
            normalize_txt(Vtxt1);
            if (tekst_wybrany(Vtxt1)) return 2;
            break;
        case 11: //trapezium X

            if (L->y1<L->y2) n=1;
            else n=-1;

            if (v->flags & 1) n*=-1;

            Lt.x1 = L->x1 - n*(v->magnitude1/load_magnitude);;
            Lt.y1 = L->y1;
            Lt.x2 = L->x2 - n*(v->magnitude2/load_magnitude);
            Lt.y2 = L->y2;

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos1=sin(Pi*(PL1.kat+90)/180);
            koc1=cos(Pi*(PL1.kat+90)/180);

            if (Lt.y1<Lt.y2)
            {
                if (Lt.x1>L->x1)
                {
                    dx=-Vtxt->wysokosc*koc1;
                    dy=-Vtxt->wysokosc*kos1;
                }
                else
                {
                    dx = 0;
                    dy = 0;
                }
                if (Lt.x2>L->x2)
                {
                    dx1=-Vtxt->wysokosc*koc1;
                    dy1=-Vtxt->wysokosc*kos1;
                }
                else {
                    dx1 = 0;
                    dy1 = 0;
                }
            }
            else
            {
                if (Lt.x1>L->x1)
                {
                    dx = 0;
                    dy = 0;
                }
                else
                {
                    dx = -Vtxt->wysokosc * koc1;
                    dy = -Vtxt->wysokosc * kos1;
                }
                if (Lt.x2>L->x2)
                {
                    dx1 = 0;
                    dy1 = 0;
                }
                else
                {
                    dx1 = -Vtxt->wysokosc * koc1;
                    dy1 = -Vtxt->wysokosc * kos1;
                }
            }

            Vtxt->x=Lt.x1+dx;
            Vtxt->y=Lt.y1+dy;
            Vtxt->kat=(PL1.kat*Pi/180);
            Vtxt->justowanie=j_do_lewej;
            Vtxt1->x=Lt.x2+dx1;
            Vtxt1->y=Lt.y2+dy1;
            Vtxt1->kat=(PL1.kat*Pi/180);
            Vtxt1->justowanie=j_do_prawej;

            normalize_txt(Vtxt);
            if (tekst_wybrany(Vtxt)) return 1;
            normalize_txt(Vtxt1);
            if (tekst_wybrany(Vtxt1)) return 2;
            break;
        case 12:  //trapezium N

            parametry_lini(v, &PL);
            kos1=sin(Pi*(PL.kat+90)/180);
            koc1=cos(Pi*(PL.kat+90)/180);

            n=1;
            if (v->flags & 1) n*=-1;

            Lt.x1 = v->x1 + n*(v->magnitude1/load_magnitude)*koc1;
            Lt.y1 = v->y1 + n*(v->magnitude1/load_magnitude)*kos1;
            Lt.x2 = v->x2 + n*(v->magnitude2/load_magnitude)*koc1;
            Lt.y2 = v->y2 + n*(v->magnitude2/load_magnitude)*kos1;

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos2=sin(Pi*(PL1.kat+90)/180);
            koc2=cos(Pi*(PL1.kat+90)/180);

            dx = 0;
            dy = 0;
            dx1 = 0;
            dy1 = 0;

            if (((v->magnitude1<0) && !(v->flags & 1)) ||
                ((v->magnitude1>0) && (v->flags & 1)))
            {
                dx = -Vtxt->wysokosc * koc2;
                dy = -Vtxt->wysokosc * kos2;
            }

            if (((v->magnitude2<0) && !(v->flags & 1)) ||
                ((v->magnitude2>0) && (v->flags & 1)))
            {
                dx1 = -Vtxt->wysokosc * koc2;
                dy1 = -Vtxt->wysokosc * kos2;
            }

            Vtxt->x=Lt.x1+dx;
            Vtxt->y=Lt.y1+dy;
            Vtxt->kat=(PL1.kat*Pi/180);
            Vtxt->justowanie=j_do_lewej;
            Vtxt1->x=Lt.x2+dx1;
            Vtxt1->y=Lt.y2+dy1;
            Vtxt1->kat=(PL1.kat*Pi/180);
            Vtxt1->justowanie=j_do_prawej;

            normalize_txt(Vtxt);
            if (tekst_wybrany(Vtxt)) return 1;
            normalize_txt(Vtxt1);
            if (tekst_wybrany(Vtxt1)) return 2;
            break;
        case 13: //trapezium H
            if (v->x1<v->x2)
            {
                n=1;
                ymax=max(v->y1, v->y2);
            }
            else
            {
                n=-1;
                ymax=min(v->y1, v->y2);
            }

            if (v->flags & 1) n*=-1;

            Lt.x1 = v->x1;
            Lt.y1 = ymax + n*(v->magnitude1/load_magnitude);
            Lt.x2 = v->x2;
            Lt.y2 = ymax + n*(v->magnitude2/load_magnitude);

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos1=sin(Pi*(PL1.kat+90)/180);
            koc1=cos(Pi*(PL1.kat+90)/180);

            dx=0;
            dy=0;
            dx1=0;
            dy1=0;

            if (Lt.x1<Lt.x2)
            {

                if (((v->magnitude1<0) && !(v->flags & 1)) ||
                    ((v->magnitude1>0) && (v->flags & 1)))
                {
                    dx=-Vtxt->wysokosc*koc1;
                    dy=-Vtxt->wysokosc*kos1;
                }

                if (((v->magnitude2<0) && !(v->flags & 1)) ||
                    ((v->magnitude2>0) && (v->flags & 1)))
                {
                    dx1=-Vtxt->wysokosc*koc1;
                    dy1=-Vtxt->wysokosc*kos1;
                }
            }
            else
            {
                if (((v->magnitude1>=0) && !(v->flags & 1)) ||
                    ((v->magnitude1<0) && (v->flags & 1)))
                {
                    dx = Vtxt->wysokosc * koc1;
                    dy = Vtxt->wysokosc * kos1;
                }
                if (((v->magnitude2>=0) && !(v->flags & 1)) ||
                    ((v->magnitude2<0) && (v->flags & 1)))
                {
                    dx1 = Vtxt->wysokosc * koc1;
                    dy1 = Vtxt->wysokosc * kos1;
                }
            }

            Vtxt->x=Lt.x1+dx;
            Vtxt->y=Lt.y1+dy;
            Vtxt->kat=normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt->justowanie=j_do_lewej;
            else Vtxt->justowanie=j_do_prawej;
            Vtxt1->x=Lt.x2+dx1;
            Vtxt1->y=Lt.y2+dy1;
            Vtxt1->kat=normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt1->justowanie=j_do_prawej;
            else  Vtxt1->justowanie=j_do_lewej;

            normalize_txt(Vtxt);
            if (tekst_wybrany(Vtxt)) return 1;
            normalize_txt(Vtxt1);
            if (tekst_wybrany(Vtxt1)) return 2;
            break;
        case 14: //trapezium V
            if (v->y1<v->y2)
            {
                n=1;
                xmax=min(v->x1, v->x2);
            }
            else
            {
                n=-1;
                xmax=max(v->x1, v->x2);
            }

            if (v->flags & 1) n*=-1;

            Lt.x1 = xmax - n*(v->magnitude1/load_magnitude);
            Lt.y1 = v->y1;
            Lt.x2 = xmax - n*(v->magnitude2/load_magnitude);
            Lt.y2 = v->y2;

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos1=sin(Pi*(PL1.kat+90)/180);
            koc1=cos(Pi*(PL1.kat+90)/180);

            dx=0;
            dy=0;
            dx1=0;
            dy1=0;

            if (Lt.y1<Lt.y2)
            {
                if (((v->magnitude1<0) && !(v->flags & 1)) ||
                    ((v->magnitude1>0) && (v->flags & 1)))
                {
                    dx=-Vtxt->wysokosc*koc1;
                    dy=-Vtxt->wysokosc*kos1;
                }
                if (((v->magnitude2<0) && !(v->flags & 1)) ||
                    ((v->magnitude2>0) && (v->flags & 1)))
                {
                    dx1=-Vtxt->wysokosc*koc1;
                    dy1=-Vtxt->wysokosc*kos1;
                }
            }
            else
            {
                if (((v->magnitude1<0) && !(v->flags & 1)) ||
                    ((v->magnitude1>0) && (v->flags & 1)))
                {
                    dx = -Vtxt->wysokosc * koc1;
                    dy = -Vtxt->wysokosc * kos1;
                }
                if (((v->magnitude2<0) && !(v->flags & 1)) ||
                    ((v->magnitude2>0) && (v->flags & 1)))
                {
                    dx1 = -Vtxt->wysokosc * koc1;
                    dy1 = -Vtxt->wysokosc * kos1;
                }
            }

            Vtxt->x=Lt.x1+dx;
            Vtxt->y=Lt.y1+dy;
            Vtxt->kat=(PL1.kat*Pi/180);
            Vtxt->justowanie=j_do_lewej;
            Vtxt1->x=Lt.x2+dx1;
            Vtxt1->y=Lt.y2+dy1;
            Vtxt1->kat=(PL1.kat*Pi/180);
            Vtxt1->justowanie=j_do_prawej;

            normalize_txt(Vtxt);
            if (tekst_wybrany(Vtxt)) return 1;
            normalize_txt(Vtxt1);
            if (tekst_wybrany(Vtxt1)) return 2;
            break;
        case 15:  //thermal
            parametry_lini(v, &PL);
            kos1=sin(Pi*(PL.kat+90)/180);
            koc1=cos(Pi*(PL.kat+90)/180);

            n=1;

            Lth.x1 = (v->x1+v->x2)/2 + n*(v->r/depth_magnitude)*koc1;   //thermal_depth_size
            Lth.y1 = (v->y1+v->y2)/2 + n*(v->r/depth_magnitude)*kos1;
            Lth.x2 = (v->x1+v->x2)/2 - n*(v->r/depth_magnitude)*koc1;
            Lth.y2 = (v->y1+v->y2)/2 - n*(v->r/depth_magnitude)*kos1;

            parametry_lini(&Lth, &PLth);
            kos1th=sin(Pi*(PLth.kat+90)/180);
            koc1th=cos(Pi*(PLth.kat+90)/180);

            Lt.x1 = Lth.x1 + (v->magnitude1/thermal_magnitude)*koc1th;
            Lt.y1 = Lth.y1 + (v->magnitude1/thermal_magnitude)*kos1th;
            Lt.x2 = Lth.x2 + (v->magnitude2/thermal_magnitude)*koc1th;
            Lt.y2 = Lth.y2 + (v->magnitude2/thermal_magnitude)*kos1th;

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos2=sin(Pi*(PL1.kat+90)/180);
            koc2=cos(Pi*(PL1.kat+90)/180);

            dx = 0;
            dy = 0;
            dx1 = 0;
            dy1 = 0;

            if (v->magnitude1<0)
            {
                dx = -Vtxt->wysokosc * koc2;
                dy = -Vtxt->wysokosc * kos2;
            }

            if (v->magnitude2<0)
            {
                dx1 = -Vtxt->wysokosc * koc2;
                dy1 = -Vtxt->wysokosc * kos2;
            }

            Vtxt->x=Lt.x1+dx;
            Vtxt->y=Lt.y1+dy;
            Vtxt->kat=(PL1.kat*Pi/180);
            Vtxt->justowanie=j_do_lewej;
            Vtxt1->x=Lt.x2+dx1;
            Vtxt1->y=Lt.y2+dy1;
            Vtxt1->kat=(PL1.kat*Pi/180);
            Vtxt1->justowanie=j_do_prawej;

            normalize_txt(Vtxt);
            if (tekst_wybrany(Vtxt)) return 1;
            normalize_txt(Vtxt1);
            if (tekst_wybrany(Vtxt1)) return 2;
            break;
    }
    return ret;
}

int vector_text_wybrany(LINIA *L, TEXT *Vtxt, AVECTOR* v)
{  double Ltx, Lty;
    PLINIA PL, PL1;
    double koc1, kos1;
    double dx2, dy2;

    Ltx = (L->x1 + L->x2) / 2;
    Lty = (L->y1 + L->y2) / 2;

    parametry_lini(L, &PL1);
    kos1 = sin(Pi * (PL1.kat + 90) / 180);
    koc1 = cos(Pi * (PL1.kat + 90) / 180);

    switch (v->style) {
        case 10:   //trapezium Y
        case 17:   //trapezium Y slab
        if (L->x1 < L->x2) {
            if (L->y2 < v->y2)
            {
                dx2 = -Vtxt->wysokosc * koc1;
                dy2 = -Vtxt->wysokosc * kos1;
            } else {
                dx2 = 0;
                dy2 = 0;
            }
        } else {
            if (L->y2 > v->y2)
            {
                dx2 = 0;
                dy2 = 0;
            } else {
                dx2 = Vtxt->wysokosc * koc1;
                dy2 = Vtxt->wysokosc * kos1;
            }
        }
        break;
        case 11: //trapezium X
            if (L->y1<L->y2)
            {
                if (L->x2>v->x2)
                {
                    dx2=-Vtxt->wysokosc*koc1;
                    dy2=-Vtxt->wysokosc*kos1;
                }
                else {
                    dx2 = 0;
                    dy2 = 0;
                }
            }
            else
            {
                if (L->x2>v->x2)
                {
                    dx2 = 0;
                    dy2 = 0;
                }
                else
                {
                    dx2 = -Vtxt->wysokosc * koc1;
                    dy2 = -Vtxt->wysokosc * kos1;
                }
            }
            break;
        case 12:  //trapezium N
            dx2 = 0;
            dy2 = 0;

            if (((v->magnitude2<0) && !(v->flags & 1)) ||
                ((v->magnitude2>0) && (v->flags & 1)))
            {
                dx2 = -Vtxt->wysokosc * koc1;
                dy2 = -Vtxt->wysokosc * kos1;
            }
            break;
        case 13: //trapezium H
            dx2=0;
            dy2=0;
            if (L->x1<L->x2)
            {

                if (((v->magnitude2<0) && !(v->flags & 1)) ||
                    ((v->magnitude2>0) && (v->flags & 1)))
                {
                    dx2=-Vtxt->wysokosc*koc1;
                    dy2=-Vtxt->wysokosc*kos1;
                }
            }
            else
            {

                if (((v->magnitude2>=0) && !(v->flags & 1)) ||
                    ((v->magnitude2<0) && (v->flags & 1)))
                {
                    dx2 = Vtxt->wysokosc * koc1;
                    dy2 = Vtxt->wysokosc * kos1;
                }
            }
            break;
        case 14: //trapezium V
            dx2=0;
            dy2=0;
            if (L->y1<L->y2)
            {

                if (((v->magnitude2<0) && !(v->flags & 1)) ||
                    ((v->magnitude2>0) && (v->flags & 1)))
                {
                    dx2=-Vtxt->wysokosc*koc1;
                    dy2=-Vtxt->wysokosc*kos1;
                }
            }
            else
            {

                if (((v->magnitude2<0) && !(v->flags & 1)) ||
                    ((v->magnitude2>0) && (v->flags & 1)))
                {
                    dx2 = -Vtxt->wysokosc * koc1;
                    dy2 = -Vtxt->wysokosc * kos1;
                }
            }
            break;
        case 15:  //thermal
            dx2=0;
            dy2=0;
            if (v->magnitude2<0)
            {
                dx2 = -Vtxt->wysokosc * koc1;
                dy2 = -Vtxt->wysokosc * kos1;
            }
            break;
        default:
            return 0;
            break;
    }

    Vtxt->x=Ltx+dx2;
    Vtxt->y=Lty+dy2;
    Vtxt->justowanie=j_srodkowo;
    Vtxt->kat=(PL1.kat*Pi/180);

    normalize_txt(Vtxt);

    return tekst_wybrany(Vtxt);
}

int vector_wybrany(AVECTOR *ad)
{
    double x0,y0;
    double n;
    LINIA L=Ldef, L1=Ldef, Lth=Ldef, Lt=Ldef, LL=Ldef;
    LUK l;
    OKRAG O;
    TEXT Vtxt=Tdef, Vtxt1;
    PLINIA PL, PLth;
    double kats, kata2;
    double kat, kos, koc, kos1th, koc1th;
    double ra;
    double _precision_;
    int ret;

    if (vector_filter>-1) {
        int vmarker=pow(2,ad->style);
        if (!(vmarker & vector_filter))
            return 0;
    }

#define arrowf 1.0

    Vtxt.warstwa = ad->warstwa;
    Vtxt.atrybut=ad->atrybut;
    Vtxt.czcionka=zmwym.czcionka;
    Vtxt.wysokosc=zmwym.wysokosc;
    Vtxt.width_factor=zmwym.width_factor;
    memcpy(&Vtxt1, &Vtxt, sizeof(TEXT));

    ra=Get_Point_Size () / arrowf;

    switch (ad->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            return prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                     jednostkiN(ad->x2), jednostkiN(ad->y2));
            break;
        case 4:
        case 18:
        case 7:
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                     jednostkiN(ad->x2), jednostkiN(ad->y2))) return 1;

            if (ad->style==4) _precision_=force_precision;
              else _precision_=displacement_precision;

            parametry_lini((LINIA*)ad, &PL);

            kos=sin(Angle_Normal((PL.kat-90)*Pi/180));
            koc=cos(Angle_Normal((PL.kat-90)*Pi/180));

            if (ad->x1<=ad->x2) n=1; else n=-1;

            Vtxt.x=(ad->x1+ad->x2)/2-((n*ra/4)*koc);
            Vtxt.y=(ad->y1+ad->y2)/2-((n*ra/4)*kos);

            set_decimal_format(&Vtxt.text, ad->magnitude1, _precision_);

            Vtxt.kat=normalize_txt_angle(PL.kat*Pi/180);
            Vtxt.justowanie=j_srodkowo;

            normalize_txt(&Vtxt);

            if (tekst_wybrany(&Vtxt))return 1;

            Vtxt.wysokosc=zmwym.wysokosc/2;
            Vtxt.width=0;
            Vtxt.height=0;
            Vtxt.x=(ad->x1+ad->x2)/2+n*((Vtxt.wysokosc+ra/4)*koc);
            Vtxt.y=(ad->y1+ad->y2)/2+n*((Vtxt.wysokosc+ra/4)*kos);

            if (ad->variant>0)
                sprintf(&Vtxt.text, "%s%d",load_symbol[(int)ad->load], ad->variant);
            else sprintf(&Vtxt.text, "%s",load_symbol[(int)ad->load]);
            normalize_txt(&Vtxt);

            if (tekst_wybrany(&Vtxt))return 1;
            break;
        case 5: //arc
        case 6:
        case 8:
        case 9:
            if (punkt_w_prostokacie(ad->x1,ad->y1)) return 1;

            if(ad->r==0) return 0;
            l.x=ad->x1;
            l.y=ad->y1;
            l.r=ad->r;
            l.kat1=ad->angle1;
            l.kat2=ad->angle2;
            if (luk_pomX(&l,X1,Y1,Y2)) return 1;
            if (luk_pomX(&l,X2,Y1,Y2)) return 1;
            if (luk_pomY(&l,Y1,X1,X2)) return 1;
            if (luk_pomY(&l,Y2,X1,X2)) return 1;
            x0=ad->x1+ad->r*cos(ad->angle1);
            y0=ad->y1+ad->r*sin(ad->angle1);
            if (punkt_w_prostokacie(x0,y0)) return 1;
            x0=ad->x1+ad->r*cos(ad->angle2);
            y0=ad->y1+ad->r*sin(ad->angle2);
            if (punkt_w_prostokacie(x0,y0)) return 1;

            if (ad->style<8) _precision_=moment_precision;
            else _precision_=rotation_precision;

            if (ad->angle2<ad->angle1)
                kata2=ad->angle2+Pi2;
            else kata2=ad->angle2;
            kats=Angle_Normal((ad->angle1+kata2)/2);
            Vtxt.x=ad->x1+(ad->r+0.5)*cos(kats);
            Vtxt.y=ad->y1+(ad->r+0.5)*sin(kats);
            Vtxt.kat=Angle_Normal(kats-Pi_/2);
            Vtxt.justowanie=j_srodkowo;
            set_decimal_format(&Vtxt.text, ad->magnitude1, _precision_);
            normalize_txt(&Vtxt);

            if (tekst_wybrany(&Vtxt)) return 1;

            Vtxt.wysokosc=zmwym.wysokosc/2;
            Vtxt.x=ad->x1+(ad->r - Vtxt.wysokosc - 0.5)*cos(kats);
            Vtxt.y=ad->y1+(ad->r - Vtxt.wysokosc - 0.5)*sin(kats);

            if (ad->variant>0)
                sprintf(&Vtxt.text, "%s%d",load_symbol[(int)ad->load], ad->variant);
            else sprintf(&Vtxt.text, "%s",load_symbol[(int)ad->load]);
            normalize_txt(&Vtxt);

            if (tekst_wybrany(&Vtxt)) return 1;

            return 0;
            break;
        case 10:
        case 17:

            LL.x1=ad->x1;
            LL.x2=ad->x2;
            LL.y1=ad->y1;
            LL.y2=ad->y2;

            if (Check_if_Equal(LL.x1, LL.x2))
            {
                LL.x2+=0.001;
            }

            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                     jednostkiN(ad->x2), jednostkiN(ad->y2))) return 1;
            L.warstwa=ad->warstwa;

            L.x1=LL.x1;
            L.x2=LL.x2;

            if (L.x1<L.x2) n=1; else n=-1;

            if (ad->flags & 1) n*=-1;

            L.y1=LL.y1+n*ad->magnitude1/((ad->style==10) ? load_magnitude : flood_magnitude);
            L.y2=LL.y2+n*ad->magnitude2/((ad->style==10) ? load_magnitude : flood_magnitude);

            if (prostokat_odcinek(jednostkiN(L.x1), jednostkiN(L.y1),
                                  jednostkiN(L.x2), jednostkiN(L.y2))) return 1;
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(L.x1), jednostkiN(L.y1))) return 1;
            if (prostokat_odcinek(jednostkiN(ad->x2), jednostkiN(ad->y2),
                                  jednostkiN(L.x2), jednostkiN(L.y2))) return 1;

            set_decimal_format(&Vtxt.text, ad->magnitude1, load_precision);
            set_decimal_format(&Vtxt1.text, ad->magnitude2, load_precision);

            ret=vector_magnitude_text_wybrany(&Vtxt, &Vtxt1, ad, &LL);
            if (ret>0) return 1;

            Vtxt.justowanie=j_srodkowo;
            Vtxt.wysokosc=zmwym.wysokosc/2;

            if (ad->variant>0)
                sprintf(&Vtxt.text, "%s%d",load_symbol[(int)ad->load], ad->variant);
            else sprintf(&Vtxt.text, "%s",load_symbol[(int)ad->load]);


            if (vector_text_wybrany(&L, &Vtxt, ad)) return 1;

            return 0;
            break;
        case 11:
            LL.x1=ad->x1;
            LL.x2=ad->x2;
            LL.y1=ad->y1;
            LL.y2=ad->y2;

            if (Check_if_Equal(LL.y1, LL.y2))
            {
                LL.y2+=0.001;
            }


            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(ad->x2), jednostkiN(ad->y2))) return 1;
            L.warstwa=ad->warstwa;

            L.y1=LL.y1;
            L.y2=LL.y2;

            if (L.y1<L.y2) n=-1; else n=1;
            if (ad->flags & 1) n*=-1;

            L.x1=LL.x1+n*ad->magnitude1/load_magnitude;
            L.x2=LL.x2+n*ad->magnitude2/load_magnitude;

            if (prostokat_odcinek(jednostkiN(L.x1), jednostkiN(L.y1),
                                  jednostkiN(L.x2), jednostkiN(L.y2))) return 1;
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(L.x1), jednostkiN(L.y1))) return 1;
            if (prostokat_odcinek(jednostkiN(ad->x2), jednostkiN(ad->y2),
                                  jednostkiN(L.x2), jednostkiN(L.y2))) return 1;

            set_decimal_format(&Vtxt.text, ad->magnitude1, load_precision);
            set_decimal_format(&Vtxt1.text, ad->magnitude2, load_precision);

            ret=vector_magnitude_text_wybrany(&Vtxt, &Vtxt1, ad, &LL);
            if (ret>0) return 1;

            Vtxt.justowanie=j_srodkowo;
            Vtxt.wysokosc=zmwym.wysokosc/2;

            if (ad->variant>0)
                sprintf(&Vtxt.text, "%s%d",load_symbol[(int)ad->load], ad->variant);
            else sprintf(&Vtxt.text, "%s",load_symbol[(int)ad->load]);

            if (vector_text_wybrany(&L, &Vtxt, ad)) return 1;

            return 0;
            break;
        case 12:
            LL.x1=ad->x1;
            LL.x2=ad->x2;
            LL.y1=ad->y1;
            LL.y2=ad->y2;

            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(ad->x2), jednostkiN(ad->y2))) return 1;
            L.warstwa=ad->warstwa;
            parametry_lini((LINIA*)ad, &PL);
            kat=PL.kat;
            kos=sin(Pi*(PL.kat+90)/180);
            koc=cos(Pi*(PL.kat+90)/180);

            n=1;
            if (ad->flags & 1) n*=-1;

            L.x1 = ad->x1 + n*(ad->magnitude1/load_magnitude)*koc;
            L.y1 = ad->y1 + n*(ad->magnitude1/load_magnitude)*kos;
            L.x2 = ad->x2 + n*(ad->magnitude2/load_magnitude)*koc;
            L.y2 = ad->y2 + n*(ad->magnitude2/load_magnitude)*kos;

            if (prostokat_odcinek(jednostkiN(L.x1), jednostkiN(L.y1),
                                  jednostkiN(L.x2), jednostkiN(L.y2))) return 1;
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(L.x1), jednostkiN(L.y1))) return 1;
            if (prostokat_odcinek(jednostkiN(ad->x2), jednostkiN(ad->y2),
                                  jednostkiN(L.x2), jednostkiN(L.y2))) return 1;

            set_decimal_format(&Vtxt.text, ad->magnitude1, load_precision);
            set_decimal_format(&Vtxt1.text, ad->magnitude2, load_precision);

            ret=vector_magnitude_text_wybrany(&Vtxt, &Vtxt1, ad, &LL);
            if (ret>0) return 1;

            Vtxt.justowanie=j_srodkowo;
            Vtxt.wysokosc=zmwym.wysokosc/2;

            if (ad->variant>0)
                sprintf(&Vtxt.text, "%s%d",load_symbol[(int)ad->load], ad->variant);
            else sprintf(&Vtxt.text, "%s",load_symbol[(int)ad->load]);


            if (vector_text_wybrany(&L, &Vtxt, ad)) return 1;

            return 0;
            break;
        case 13:
            LL.x1=ad->x1;
            LL.x2=ad->x2;
            LL.y1=ad->y1;
            LL.y2=ad->y2;

            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(ad->x2), jednostkiN(ad->y2))) return 1;
            L.warstwa=L1.warstwa=ad->warstwa;
            L.x1=L1.x1=ad->x1;
            L.x2=L1.x2=ad->x2;
            if (ad->x1<ad->x2)
            {
                n=1;
                L1.y1=L1.y2=max(ad->y1, ad->y2);
            }
            else
            {
                n=-1;
                L1.y1=L1.y2=min(ad->y1, ad->y2);
            }
            if (ad->flags & 1) n*=-1;

            L.y1=L1.y1+n*ad->magnitude1/load_magnitude;
            L.y2=L1.y1+n*ad->magnitude2/load_magnitude;
            if (prostokat_odcinek(jednostkiN(L.x1), jednostkiN(L.y1),
                                  jednostkiN(L.x2), jednostkiN(L.y2))) return 1;
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(L.x1), jednostkiN(L.y1))) return 1;
            if (prostokat_odcinek(jednostkiN(ad->x2), jednostkiN(ad->y2),
                                  jednostkiN(L.x2), jednostkiN(L.y2))) return 1;

            if (prostokat_odcinek(jednostkiN(L1.x1), jednostkiN(L1.y2),
                                  jednostkiN(L1.x2), jednostkiN(L1.y2))) return 1;

            set_decimal_format(&Vtxt.text, ad->magnitude1, load_precision);
            set_decimal_format(&Vtxt1.text, ad->magnitude2, load_precision);

            ret=vector_magnitude_text_wybrany(&Vtxt, &Vtxt1, ad, &LL);
            if (ret>0) return 1;

            Vtxt.justowanie=j_srodkowo;
            Vtxt.wysokosc=zmwym.wysokosc/2;

            if (ad->variant>0)
                sprintf(&Vtxt.text, "%s%d",load_symbol[(int)ad->load], ad->variant);
            else sprintf(&Vtxt.text, "%s",load_symbol[(int)ad->load]);

            if (vector_text_wybrany(&L, &Vtxt, ad)) return 1;

            return 0;
            break;
        case 14:
            LL.x1=ad->x1;
            LL.x2=ad->x2;
            LL.y1=ad->y1;
            LL.y2=ad->y2;

            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(ad->x2), jednostkiN(ad->y2))) return 1;
            L.warstwa=L1.warstwa=ad->warstwa;
            L.y1=L1.y1=ad->y1;
            L.y2=L1.y2=ad->y2;
            if (ad->y1<ad->y2)
            {
                n=-1;
                L1.x1=L1.x2=min(ad->x1, ad->x2);
            }
            else
            {
                n=1;
                L1.x1=L1.x2=max(ad->x1, ad->x2);
            }
            if (ad->flags & 1) n*=-1;

            L.x1=L1.x1+n*ad->magnitude1/load_magnitude;
            L.x2=L1.x2+n*ad->magnitude2/load_magnitude;
            if (prostokat_odcinek(jednostkiN(L.x1), jednostkiN(L.y1),
                                  jednostkiN(L.x2), jednostkiN(L.y2))) return 1;
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(L.x1), jednostkiN(L.y1))) return 1;
            if (prostokat_odcinek(jednostkiN(ad->x2), jednostkiN(ad->y2),
                                  jednostkiN(L.x2), jednostkiN(L.y2))) return 1;

            if (prostokat_odcinek(jednostkiN(L1.x1), jednostkiN(L.y1),
                                  jednostkiN(L1.x1), jednostkiN(L.y2))) return 1;

            set_decimal_format(&Vtxt.text, ad->magnitude1, load_precision);
            set_decimal_format(&Vtxt1.text, ad->magnitude2, load_precision);

            ret=vector_magnitude_text_wybrany(&Vtxt, &Vtxt1, ad, &LL);
            if (ret>0) return 1;

            Vtxt.justowanie=j_srodkowo;
            Vtxt.wysokosc=zmwym.wysokosc/2;

            if (ad->variant>0)
                sprintf(&Vtxt.text, "%s%d",load_symbol[(int)ad->load], ad->variant);
            else sprintf(&Vtxt.text, "%s",load_symbol[(int)ad->load]);

            if (vector_text_wybrany(&L, &Vtxt, ad)) return 1;

            return 0;
            break;
        case 15:
            LL.x1=ad->x1;
            LL.x2=ad->x2;
            LL.y1=ad->y1;
            LL.y2=ad->y2;

            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(ad->x2), jednostkiN(ad->y2))) return 1;
            L.warstwa=ad->warstwa;
            parametry_lini((LINIA*)ad, &PL);
            kat=PL.kat;
            kos=sin(Pi*(PL.kat+90)/180);
            koc=cos(Pi*(PL.kat+90)/180);

            Lth.x1 = (ad->x1+ad->x2)/2 + (ad->r/depth_magnitude)*koc; //thermal_depth_size
            Lth.y1 = (ad->y1+ad->y2)/2 + (ad->r/depth_magnitude)*kos;
            Lth.x2 = (ad->x1+ad->x2)/2 - (ad->r/depth_magnitude)*koc;
            Lth.y2 = (ad->y1+ad->y2)/2 - (ad->r/depth_magnitude)*kos;

            if (prostokat_odcinek(jednostkiN(Lth.x1), jednostkiN(Lth.y1),
                                  jednostkiN(Lth.x2), jednostkiN(Lth.y2))) return 1;

            parametry_lini(&Lth, &PLth);
            kos1th=sin(Pi*(PLth.kat+90)/180);
            koc1th=cos(Pi*(PLth.kat+90)/180);

            L.x1 = Lth.x1 + (ad->magnitude1/thermal_magnitude)*koc1th;
            L.y1 = Lth.y1 + (ad->magnitude1/thermal_magnitude)*kos1th;
            L.x2 = Lth.x2 + (ad->magnitude2/thermal_magnitude)*koc1th;
            L.y2 = Lth.y2 + (ad->magnitude2/thermal_magnitude)*kos1th;

            if (prostokat_odcinek(jednostkiN(L.x1), jednostkiN(L.y1),
                                  jednostkiN(L.x2), jednostkiN(L.y2))) return 1;
            if (prostokat_odcinek(jednostkiN(Lth.x1), jednostkiN(Lth.y1),
                                  jednostkiN(L.x1), jednostkiN(L.y1))) return 1;
            if (prostokat_odcinek(jednostkiN(Lth.x2), jednostkiN(Lth.y2),
                                  jednostkiN(L.x2), jednostkiN(L.y2))) return 1;

            set_decimal_format(&Vtxt.text, ad->magnitude1, load_precision);
            set_decimal_format(&Vtxt1.text, ad->magnitude2, load_precision);

            ret=vector_magnitude_text_wybrany(&Vtxt, &Vtxt1, ad, &LL);
            if (ret>0) return 1;

            Vtxt.justowanie=j_srodkowo;
            Vtxt.wysokosc=zmwym.wysokosc/2;

            if (ad->variant>0)
                sprintf(&Vtxt.text, "%s%d",load_symbol[(int)ad->load], ad->variant);
            else sprintf(&Vtxt.text, "%s",load_symbol[(int)ad->load]);

            if (vector_text_wybrany(&L, &Vtxt, ad)) return 1;

            return 0;
            break;
        case 16:
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(ad->x2), jednostkiN(ad->y2))) return 1;


            O.x=ad->x1;
            O.y=ad->y1;
            O.r=ad->r;

            if (okrag_wybrany(&O))  return 1;

            Vtxt.warstwa = ad->warstwa;
            Vtxt.atrybut=ad->atrybut;
            Vtxt.czcionka=zmwym.czcionka;
            Vtxt.wysokosc=zmwym.wysokosc*0.75;
            Vtxt.width_factor=zmwym.width_factor;

            Vtxt.x=ad->x2;
            Vtxt.y=ad->y2;
            Vtxt.kat=0;

            Vtxt.justowanie=j_do_lewej;

            set_decimal_format(&Vtxt1.text, ad->magnitude1, dim_precision);
            sprintf(&Vtxt.text,"R%s",Vtxt1.text);

            double t_len_mm = Get_TextLen(&Vtxt, Vtxt.text);
            double direction=ad->x2-ad->x1;

            Lt.warstwa=ad->warstwa;
            Lt.x1=ad->x2;
            Lt.y1=ad->y2;
            Lt.y2=Lt.y1;

            if (direction > 0) //to the right
            {
                Vtxt.x = ad->x2 + Vtxt.wysokosc / 4.0;
                Vtxt.y = ad->y2 + Vtxt.wysokosc / 4.0;
                Vtxt.justowanie = j_do_lewej;

                if (t_len_mm == 0.0) Lt.x2 = ad->x2 + 0.01;
                else Lt.x2 = ad->x2 + t_len_mm + Vtxt.wysokosc / 2.5;
            } else  //to the left
            {
                Vtxt.x = ad->x2 - Vtxt.wysokosc / 4.0;
                Vtxt.y = ad->y2 + Vtxt.wysokosc / 4.0;
                Vtxt.justowanie = j_do_prawej;
                if (t_len_mm == 0.0) Lt.x2 = ad->x2 - 0.01;
                else Lt.x2 = ad->x2 - t_len_mm - Vtxt.wysokosc / 2.0;
            }
            if (tekst_wybrany(&Vtxt)) return 1;

            if (0 != linia_wybrana(&Lt)) return 1;

            break;
        default:
            return 0;
            break;
    }

    return 0;
}


int vector_w_prostokacie(AVECTOR *ad)
{
    double x0,y0;
    double n;
    LINIA L=Ldef, L1=Ldef, Lth=Ldef, Lt=Ldef;
    LUK l;
    OKRAG O;
    TEXT Vtxt=Tdef, Vtxt1;
    PLINIA PL, PLth;
    double kat, kos, koc, koc1th, kos1th;
    double ra;
    int ret;

#define arrowf 1.0

    Vtxt.warstwa = ad->warstwa;
    Vtxt.atrybut=ad->atrybut;
    Vtxt.czcionka=zmwym.czcionka;
    Vtxt.wysokosc=zmwym.wysokosc;
    Vtxt.width_factor=zmwym.width_factor;
    memcpy(&Vtxt1, &Vtxt, sizeof(TEXT));

    ra=Get_Point_Size () / arrowf;

    switch (ad->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 18:
        case 7:
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                     jednostkiN(ad->x2), jednostkiN(ad->y2))!=3)  return 0;
            return 1;
            break;
        case 5:  //arcs
        case 6:
        case 8:
        case 9:
            l.x=ad->x1;
            l.y=ad->y1;
            l.r=ad->r;
            l.kat1=ad->angle1;
            l.kat2=ad->angle2;
            ret=luk_w_prostokacie(&l);
            if (ret==0) return 0;
            return 1;
            break;
        case 10:
        case 17:
            if (linia_wybrana((LINIA*)ad)!=3) return 0;

            L.x1=ad->x1;
            L.x2=ad->x2;
            if (ad->x1<ad->x2) n=1; else n=-1;
            if (ad->flags & 1) n*=-1;
            L.y1=ad->y1+n*ad->magnitude1/((ad->style==10) ? load_magnitude : flood_magnitude);
            L.y2=ad->y2+n*ad->magnitude2/((ad->style==10) ? load_magnitude : flood_magnitude);
            if (prostokat_odcinek(jednostkiN(L.x1), jednostkiN(L.y1),
                                  jednostkiN(L.x2), jednostkiN(L.y2))!=3)  return 0;
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(L.x1), jednostkiN(L.y1))!=3)  return 0;
            if (prostokat_odcinek(jednostkiN(ad->x2), jednostkiN(ad->y2),
                                  jednostkiN(L.x2), jednostkiN(L.y2)) !=3) return 0;
            return 1;
            break;
        case 11:
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(ad->x2), jednostkiN(ad->y2)) !=3) return 0;
            L.y1=ad->y1;
            L.y2=ad->y2;
            if (ad->y1<ad->y2) n=-1; else n=1;
            if (ad->flags & 1) n*=-1;
            L.x1=ad->x1+n*ad->magnitude1/load_magnitude;
            L.x2=ad->x2+n*ad->magnitude2/load_magnitude;
            if (prostokat_odcinek(jednostkiN(L.x1), jednostkiN(L.y1),
                                  jednostkiN(L.x2), jednostkiN(L.y2)) !=3) return 0;
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(L.x1), jednostkiN(L.y1)) !=3) return 0;
            if (prostokat_odcinek(jednostkiN(ad->x2), jednostkiN(ad->y2),
                                  jednostkiN(L.x2), jednostkiN(L.y2)) !=3) return 0;
            return 1;
            break;
        case 12:
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(ad->x2), jednostkiN(ad->y2))  !=3) return 0;

            parametry_lini((LINIA*)ad, &PL);
            kat=PL.kat;
            kos=sin(Pi*(PL.kat+90)/180);
            koc=cos(Pi*(PL.kat+90)/180);

            n=1;
            if (ad->flags & 1) n*=-1;

            L.x1 = ad->x1 + n*(ad->magnitude1/load_magnitude)*koc;
            L.y1 = ad->y1 + n*(ad->magnitude1/load_magnitude)*kos;
            L.x2 = ad->x2 + n*(ad->magnitude2/load_magnitude)*koc;
            L.y2 = ad->y2 + n*(ad->magnitude2/load_magnitude)*kos;

            if (prostokat_odcinek(jednostkiN(L.x1), jednostkiN(L.y1),
                                  jednostkiN(L.x2), jednostkiN(L.y2)) !=3) return 0;
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(L.x1), jednostkiN(L.y1)) !=3) return 0;
            if (prostokat_odcinek(jednostkiN(ad->x2), jednostkiN(ad->y2),
                                  jednostkiN(L.x2), jednostkiN(L.y2)) !=3) return 0;

            return 1;
            break;
        case 13:
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(ad->x2), jednostkiN(ad->y2)) !=3) return 0;
            L.warstwa=L1.warstwa=ad->warstwa;
            L.x1=L1.x1=ad->x1;
            L.x2=L1.x2=ad->x2;
            if (ad->x1<ad->x2)
            {
                n=1;
                L1.y1=L1.y2=max(ad->y1, ad->y2);
            }
            else
            {
                n=-1;
                L1.y1=L1.y2=min(ad->y1, ad->y2);
            }
            if (ad->flags & 1) n*=-1;
            L.y1=L1.y1+n*ad->magnitude1/load_magnitude;
            L.y2=L1.y1+n*ad->magnitude2/load_magnitude;
            if (prostokat_odcinek(jednostkiN(L.x1), jednostkiN(L.y1),
                                  jednostkiN(L.x2), jednostkiN(L.y2)) !=3) return 0;
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(L.x1), jednostkiN(L.y1)) !=3) return 0;
            if (prostokat_odcinek(jednostkiN(ad->x2), jednostkiN(ad->y2),
                                  jednostkiN(L.x2), jednostkiN(L.y2)) !=3) return 0;

            if (prostokat_odcinek(jednostkiN(L1.x1), jednostkiN(L1.y2),
                                  jednostkiN(L1.x2), jednostkiN(L1.y2)) !=3) return 0;
            return 1;
            break;
        case 14:
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(ad->x2), jednostkiN(ad->y2)) !=3) return 0;
            L.y1=L1.y1=ad->y1;
            L.y2=L1.y2=ad->y2;
            if (ad->y1<ad->y2)
            {
                n=-1;
                L1.x1=L1.x2=min(ad->x1, ad->x2);
            }
            else
            {
                n=1;
                L1.x1=L1.x2=max(ad->x1, ad->x2);
            }
            if (ad->flags & 1) n*=-1;
            L.x1=L1.x1+n*ad->magnitude1/load_magnitude;
            L.x2=L1.x2+n*ad->magnitude2/load_magnitude;
            if (prostokat_odcinek(jednostkiN(L.x1), jednostkiN(L.y1),
                                  jednostkiN(L.x2), jednostkiN(L.y2)) !=3) return 0;
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(L.x1), jednostkiN(L.y1)) !=3) return 0;
            if (prostokat_odcinek(jednostkiN(ad->x2), jednostkiN(ad->y2),
                                  jednostkiN(L.x2), jednostkiN(L.y2)) !=3) return 0;

            if (prostokat_odcinek(jednostkiN(L1.x1), jednostkiN(L1.y2),
                                  jednostkiN(L1.x2), jednostkiN(L1.y2)) !=3) return 0;
            return 1;
            break;
        case 15:
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(ad->x2), jednostkiN(ad->y2))  !=3) return 0;

            parametry_lini((LINIA*)ad, &PL);
            kat=PL.kat;
            kos=sin(Pi*(PL.kat+90)/180);
            koc=cos(Pi*(PL.kat+90)/180);

            Lth.x1 = (ad->x1+ad->x2)/2 + (ad->r/depth_magnitude)*koc; //thermal_depth_size
            Lth.y1 = (ad->y1+ad->y2)/2 + (ad->r/depth_magnitude)*kos;
            Lth.x2 = (ad->x1+ad->x2)/2 - (ad->r/depth_magnitude)*koc;
            Lth.y2 = (ad->y1+ad->y2)/2 - (ad->r/depth_magnitude)*kos;

            parametry_lini(&Lth, &PLth);
            kos1th=sin(Pi*(PLth.kat+90)/180);
            koc1th=cos(Pi*(PLth.kat+90)/180);

            L.x1 = Lth.x1 + (ad->magnitude1/thermal_magnitude)*koc1th;
            L.y1 = Lth.y1 + (ad->magnitude1/thermal_magnitude)*kos1th;
            L.x2 = Lth.x2 + (ad->magnitude2/thermal_magnitude)*koc1th;
            L.y2 = Lth.y2 + (ad->magnitude2/thermal_magnitude)*kos1th;

            if (prostokat_odcinek(jednostkiN(L.x1), jednostkiN(L.y1),
                                  jednostkiN(L.x2), jednostkiN(L.y2)) !=3) return 0;
            if (prostokat_odcinek(jednostkiN(Lth.x1), jednostkiN(Lth.y1),
                                  jednostkiN(L.x1), jednostkiN(L.y1)) !=3) return 0;
            if (prostokat_odcinek(jednostkiN(Lth.x2), jednostkiN(Lth.y2),
                                  jednostkiN(L.x2), jednostkiN(L.y2)) !=3) return 0;

            return 1;
            break;
        case 16:
            if (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(ad->x2), jednostkiN(ad->y2)) !=3) return 0;

            ////if(ad->r==0) return 0;

            O.x=ad->x1;
            O.y=ad->y1;
            O.r=ad->r;

            ret=okrag_w_prostokacie(&O);
            if (ret==0) return 0;

            Vtxt.warstwa = ad->warstwa;
            Vtxt.atrybut=ad->atrybut;
            Vtxt.czcionka=zmwym.czcionka;
            Vtxt.wysokosc=zmwym.wysokosc*0.75;
            Vtxt.width_factor=zmwym.width_factor;

            Vtxt.x=ad->x2;
            Vtxt.y=ad->y2;
            Vtxt.kat=0;

            Vtxt.justowanie=j_do_lewej;

            set_decimal_format(&Vtxt1.text, ad->magnitude1, dim_precision);
            sprintf(&Vtxt.text,"R%s",Vtxt1.text);

            double t_len_mm = Get_TextLen(&Vtxt, Vtxt.text);
            double direction=ad->x2-ad->x1;

            Lt.warstwa=ad->warstwa;
            Lt.x1=ad->x2;
            Lt.y1=ad->y2;
            Lt.y2=Lt.y1;

            if (direction > 0) //to the right
            {
                Vtxt.x = ad->x2 + Vtxt.wysokosc / 4.0;
                Vtxt.y = ad->y2 + Vtxt.wysokosc / 4.0;
                Vtxt.justowanie = j_do_lewej;

                if (t_len_mm == 0.0) Lt.x2 = ad->x2 + 0.01;
                else Lt.x2 = ad->x2 + t_len_mm + Vtxt.wysokosc / 2.5;
            } else  //to the left
            {
                Vtxt.x = ad->x2 - Vtxt.wysokosc / 4.0;
                Vtxt.y = ad->y2 + Vtxt.wysokosc / 4.0;
                Vtxt.justowanie = j_do_prawej;
                if (t_len_mm == 0.0) Lt.x2 = ad->x2 - 0.01;
                else Lt.x2 = ad->x2 - t_len_mm - Vtxt.wysokosc / 2.0;
            }
            ret = tekst_w_prostokacie(&Vtxt);
            if (ret==0) return 0;

            ret = linia_wybrana(&Lt);

            if (ret==0) return 0;

            return 1;
            break;
        default:
            return 0;
            break;
    }

    return 0;
}


int vector_drag_wybrany(AVECTOR *ad)
{
    double x0,y0;
    double n;
    LINIA L=Ldef, L1=Ldef;
    LUK l;
    OKRAG O;
    PLINIA PL;
    double kat, kos, koc;
    int ret;

    switch (ad->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 18:
        case 7:
            return prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                     jednostkiN(ad->x2), jednostkiN(ad->y2));
            break;
        case 5:  //arcs
        case 6:
        case 8:
        case 9:
            if (X1 > ad->x1 + ad->r || X2 < ad->x1 - ad->r ||
                Y1 >  ad->y1 +  ad->r || Y2 <  ad->y1 -  ad->r) return 0;
            if(ad->r==0) return 0;
            if (punkt_w_prostokacie(ad->x1,ad->y1)) return 4;

            x0=ad->x1+ad->r*cos(ad->angle1);
            y0=ad->y1+ad->r*sin(ad->angle1);
            if (punkt_w_prostokacie(x0,y0)) return 1;
            x0=ad->x1+ad->r*cos(ad->angle2);
            y0=ad->y1+ad->r*sin(ad->angle2);
            if (punkt_w_prostokacie(x0,y0)) return 2;
            return 0;
            break;
        case 10:
        case 17:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            return (prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                  jednostkiN(ad->x2), jednostkiN(ad->y2)));
            break;
        case 16:
            ret=prostokat_odcinek(jednostkiN(ad->x1), jednostkiN(ad->y1),
                                     jednostkiN(ad->x2), jednostkiN(ad->y2));
            if (ret>0) return ret;
            if(ad->r==0) return 0;
            if (punkt_w_prostokacie(ad->x1,ad->y1)) return 4;
            O.x=ad->x1;
            O.y=ad->y1;
            O.r=ad->r;
            return okrag_wybrany(&O);
            break;
        default:
            return 0;
            break;
    }

    return 0;
}

			    
int linia_wybrana_pin(LINIA *ad)
{ return punkty_pin_odcinek(ad); }	

int tekst_wybrany_exe(TEXT *ad0, BOOL to_printer)
{ double x1,y1,x2,y2,x3,y3,x4,y4,x01,y01,xt1,yt1, x001, y001;
  double adsin, adcos;
  double addl;
  double WysokoscT;
  TEXT ad1;
  TEXT *ad;
  unsigned char *zn;
  char *ptr_zn;
  BOOL out_break;
  double t_line_width;


  if ((ad0->ukryty == 1) && (type_view == FALSE)) return 0;
  if (punkt_w_tekscie(ad0,.5*(X2+X1),.5*(Y2+Y1))) return 1;
  adsin=sin(ad0->kat);
  adcos=cos(ad0->kat);
  if (TRUE == Get_Point_View ())
  {
  if ((strlen(ad0->text) == 0) && (ad0->typ>2))
      {
      memmove(&ad1,ad0,sizeof(TEXT_NAG));
      ad1.text[0]=255;
      ad1.text[1]='\0';
      ad=&ad1;
      }
      else ad=ad0;
  }
   else ad=ad0;    


  t_line_width = ad->wysokosc * s__Fonts_Style[ad->czcionka].df_width_line;
  if (ad->bold == 1) t_line_width *= 1.5;
  t_line_width /= 2.0;


  if (ad0->underline)
  {

	  switch (ad0->justowanie)
	  {
	  case 0:
		  Underl_del = 0.25*(ad0->wysokosc);
		  break;
	  case 1:
		  Underl_del = 0.25*(ad0->wysokosc);
		  break;
	  case 2:
		  Underl_del = 0.25*(ad0->wysokosc);
		  break;
	  case 3:
		  Underl_del = 0.75*(ad0->wysokosc);
		  break;
	  }

	  t_line_width += Underl_del;
  }


  zn = (unsigned char *)ad->text;

  addl = Get_Text_Len(ad, zn, 1.0, 0, NULL, NULL, NULL, NULL);

  WysokoscT = ad->wysokosc;

  x01 = ad->x ;
  y01 = ad->y ;
  x001 = ad->x;
  y001 = ad->y;
  
    switch (ad->justowanie)
     {
  case 1: xt1 = x01 - addl * adcos ;  /*to the right*/
          yt1 = y01 - addl * adsin ; 
          x01 = xt1;
          y01 = yt1;

          break;
  case 2: xt1 = x01 - (addl/2) * adcos ;  /*middle*/
          yt1 = y01 - (addl)/2 * adsin ; 
          x01 = xt1;
          y01 = yt1;

          break;
  case 3: xt1 = x01 - (addl/2) * adcos + (WysokoscT/2) * adsin;  /*central*/
          yt1 = y01 - (addl)/2 * adsin - (WysokoscT/2) * adcos; 
          x01 = xt1;
          y01 = yt1;

          break;        
  default: break;        
     }  

  out_break = FALSE;

  while ((zn != '\0') && (!out_break))
  {
	  x1 = jednostkiN(x01 - t_line_width);
	  y1 = jednostkiN(y01 - t_line_width);
	  x2 = x1 + jednostkiN((addl+ t_line_width )*adcos);
	  y2 = y1 - jednostkiN(Ky*(addl+ t_line_width)*adsin);
	  x4 = x1 - jednostkiN((ad->wysokosc+ t_line_width )*adsin);
	  y4 = y1 - jednostkiN(Ky*(ad->wysokosc+ t_line_width )*adcos);
	  x3 = x2 + x4 - x1;
	  y3 = y2 + y4 - y1;
	  if (punkt_w_prostokacie(x01, y01)) return 1;
	  if (prostokat_odcinek(x1, y1, x2, y2)) return 1;
	  if (prostokat_odcinek(x3, y3, x4, y4)) return 1;
	  if (prostokat_odcinek(x2, y2, x3, y3)) return 1;
	  if (prostokat_odcinek(x1, y1, x4, y4)) return 1;

	  //next line if exists
	  ptr_zn = strchr(zn, '\n');
	  if (ptr_zn != NULL)
	  {
		  zn = ptr_zn + 1;
		  addl = Get_Text_Len(ad, (char*)zn, 1.0, 0, NULL, NULL, NULL, NULL);


		  x001 += WysokoscT * ((float)(ad->spacing) / 10.0 + 1.0) *adsin; //SPACING
		  y001 -= WysokoscT * ((float)(ad->spacing) / 10.0 + 1.0) *adcos; //SPACING


		  x01 = x001;
		  y01 = y001;

		  //////////////////
		  switch (ad->justowanie)
		  {
		  case 1: xt1 = x01 - addl * adcos;  /*to the right*/
			  yt1 = y01 + Ky * addl * adsin;
			  x01 = xt1;
			  y01 = yt1;

			  break;
		  case 2: xt1 = x01 - (addl / 2) * adcos;  /*middle*/
			  yt1 = y01 + (Ky * addl) / 2 * adsin;
			  x01 = xt1;
			  y01 = yt1;

			  break;
		  case 3: xt1 = x01 - (addl / 2) * adcos + (WysokoscT / 2) * adsin;  /*central*/
			  yt1 = y01 + (Ky * addl) / 2 * adsin - (WysokoscT / 2) * adcos;
			  x01 = xt1;
			  y01 = yt1;

			  break;
		  }
		  ///////////////////
	  }
	  else out_break = TRUE;

  }
  return 0;
}


int tekst_wybrany(TEXT *ad0)
{
	return tekst_wybrany_exe(ad0, FALSE);
}

int tekst_wybrany_prn(TEXT *ad0)
{
	return tekst_wybrany_exe(ad0, TRUE);
}

int tekst_wybrany000(TEXT *ad0)
{
	double x1, y1, x2, y2, x3, y3, x4, y4, x01, y01, xt1, yt1, x001, y001;
	double adsin, adcos;
	double addl;
	double WysokoscT;
	TEXT ad1;
	TEXT *ad;
	unsigned char *zn;
	char *ptr_zn;
	BOOL out_break;


	if ((ad0->ukryty == 1) && (type_view == FALSE)) return 0;
	if (punkt_w_tekscie(ad0, .5*(X2 + X1), .5*(Y2 + Y1))) return 1;
	adsin = sin(ad0->kat);
	adcos = cos(ad0->kat);
	if (TRUE == Get_Point_View())
	{
		if ((strlen(ad0->text) == 0) && (ad0->typ > 2))
		{
			memmove(&ad1, ad0, sizeof(TEXT_NAG));
			ad1.text[0] = 255;
			ad1.text[1] = '\0';
			ad = &ad1;
		}
		else ad = ad0;
	}
	else ad = ad0;


	zn = (unsigned char *)ad->text;

	addl = Get_Text_Len(ad, zn, 1.0, 0, NULL, NULL, NULL, NULL);

	WysokoscT = ad->wysokosc;

	x01 = ad->x;
	y01 = ad->y;
	x001 = ad->x;
	y001 = ad->y;

	switch (ad->justowanie)
	{
	case 1: xt1 = x01 - addl * adcos;  /*do prawej*/
		yt1 = y01 - addl * adsin;
		x01 = xt1;
		y01 = yt1;
		x001 = xt1;
		y001 = yt1;
		break;
	case 2: xt1 = x01 - (addl / 2) * adcos;  /*srodkowo*/
		yt1 = y01 - (addl) / 2 * adsin;
		x01 = xt1;
		y01 = yt1;
		x001 = xt1;
		y001 = yt1;
		break;
	case 3: xt1 = x01 - (addl / 2) * adcos + (WysokoscT / 2) * adsin;  /*centralnie*/
		yt1 = y01 - (addl) / 2 * adsin - (WysokoscT / 2) * adcos;
		x01 = xt1;
		y01 = yt1;
		x001 = xt1;
		y001 = yt1;
		break;
	default: break;
	}

	out_break = FALSE;

	while ((zn != '\0') && (!out_break))
	{
		x1 = jednostkiN(x01/*ad->x*/);
		y1 = jednostkiN(y01/*ad->y*/);
		x2 = x1 + jednostkiN(addl*adcos);
		y2 = y1 - jednostkiN(Ky*addl*adsin);
		x4 = x1 - jednostkiN(ad->wysokosc*adsin);
		y4 = y1 - jednostkiN(Ky*ad->wysokosc*adcos);
		x3 = x2 + x4 - x1;
		y3 = y2 + y4 - y1;
		if (punkt_w_prostokacie(x01/*ad->x*/, y01/*ad->y*/)) return 1;
		if (prostokat_odcinek(x1, y1, x2, y2)) return 1;
		if (prostokat_odcinek(x3, y3, x4, y4)) return 1;
		if (prostokat_odcinek(x2, y2, x3, y3)) return 1;
		if (prostokat_odcinek(x1, y1, x4, y4)) return 1;

		//next line if exists
		ptr_zn = strchr(zn, '\n');
		if (ptr_zn != NULL)
		{
			zn = ptr_zn + 1;
			addl = Get_Text_Len(ad, (char*)zn, 1.0, 0, NULL, NULL, NULL, NULL);

			x001 += WysokoscT * ((float)(ad->spacing) / 10.0 + 1.0)*adsin; //SPACING
			y001 -= WysokoscT * ((float)(ad->spacing) / 10.0 + 1.0)*adcos; //SPACING

			x01 = x001;
			y01 = y001;
		}
		else out_break = TRUE;

	}
	return 0;
}

int spline_wybrany(SPLINE *ad)
{
	int ret=0;
	int i,j;
	float out_x[MaxNpts], out_y[MaxNpts];

	SPLINE tmp_spline = Splinedef;
	double bnodes_2_tangent_org, bnodes_2_tangent;
	int node_count;

#define Npts 30  //90

	if (ad->npts < 5)
	{
		calc_bspline(ad->lp, ad->npts, ad->xy, Npts, out_x, out_y);
		for (i = 0; i < (Npts - 1); i++)
		{
			ret = prostokat_odcinek(jednostkiN(out_x[i]), jednostkiN(out_y[i]),
				jednostkiN(out_x[i + 1]), jednostkiN(out_y[i + 1]));
			if (ret > 0) return 1;
		}
		return 0;
	}
	else  //multipoints
	{

		curviness = ad->xy[ad->lp];

		tmp_spline.npts = 4;
		tmp_spline.lp = 8;
		tmp_spline.kolor = ad->kolor;
		tmp_spline.typ = ad->typ;

		node_count = 1;


		set_bnodes(ad, ad->xy, &node_count);

		calc_tangents(ad->closed, &node_count, &bnodes_2_tangent);

		if (ad->closed == 0)
		{
			bnodes_2_tangent_org = bnodes[2].tangent;
			bnodes[2].tangent = bnodes_2_tangent;
			get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);

			calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
			for (i = 0; i < (Npts - 1); i++)
			{
				ret = prostokat_odcinek(jednostkiN(out_x[i]), jednostkiN(out_y[i]),
					jednostkiN(out_x[i + 1]), jednostkiN(out_y[i + 1]));
				if (ret > 0) return 1;
			}
			bnodes[2].tangent = bnodes_2_tangent_org;
		}

		for (j = 2; j < node_count - 2; j++)  //1 - extra_node
		{
			get_control_points(bnodes[j], bnodes[j + 1], tmp_spline.xy);

			calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
			for (i = 0; i < (Npts - 1); i++)
			{
				ret = prostokat_odcinek(jednostkiN(out_x[i]), jednostkiN(out_y[i]),
					jednostkiN(out_x[i + 1]), jednostkiN(out_y[i + 1]));
				if (ret > 0) return 1;
			}
		}

		if ((node_count > 3) && (ad->closed == 0))
		{
			get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);

			calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, Npts, out_x, out_y);
			for (i = 0; i < (Npts - 1); i++)
			{
				ret = prostokat_odcinek(jednostkiN(out_x[i]), jednostkiN(out_y[i]),
					jednostkiN(out_x[i + 1]), jednostkiN(out_y[i + 1]));
				if (ret > 0) return 1;
			}
		}
		return 0;
	}
}

int w_spline_wybrany(SPLINE *s)
{
	int i, j;
	int ret;

	j = 0;
	for (i = 0; i < (int)(s->lp - 2); i += 2)
	{
		if (prostokat_odcinek(jednostkiN(s->xy[i]), jednostkiN(s->xy[i + 1]),
			jednostkiN(s->xy[i + 2]), jednostkiN(s->xy[i + 3])))
		{
			return 1;
		}
		tab_ixy1[j++] = ix1;
		tab_ixy1[j++] = iy1;
	}
	ret = (prostokat_odcinek(jednostkiN(s->xy[s->lp - 2]), jednostkiN(s->xy[s->lp - 1]),
		jednostkiN(s->xy[0]), jednostkiN(s->xy[1])) > 0) ? 1 : 0;
	tab_ixy1[j++] = ix1;
	tab_ixy1[j++] = iy1;
	lp = j;
	return ret;
}

int luk_wybrany(LUK *ad)
{
  double x0,y0;

  if (X1 > ad->x + ad->r || X2 < ad->x - ad->r ||
      Y1 >  ad->y +  ad->r || Y2 <  ad->y -  ad->r) return 0;
  if(ad->r==0) return 0;
  if (luk_pomX(ad,X1,Y1,Y2)) return 1;
  if (luk_pomX(ad,X2,Y1,Y2)) return 1;
  if (luk_pomY(ad,Y1,X1,X2)) return 1;
  if (luk_pomY(ad,Y2,X1,X2)) return 1;
  x0=ad->x+ad->r*cos(ad->kat1);
  y0=ad->y+ad->r*sin(ad->kat1);
  if (punkt_w_prostokacie(x0,y0)) return 1;
  x0=ad->x+ad->r*cos(ad->kat2);
  y0=ad->y+ad->r*sin(ad->kat2);
  if (punkt_w_prostokacie(x0,y0)) return 1;
  return 0;
}

int luk_wybrany_prn(LUK *ad)
{
    double x0,y0;
    double X11,X22,Y11,Y22;

    X11=X1sel/Ds_x;
    X22=X2sel/Ds_x;
    Y11=Y1sel/Ds_x;
    Y22=Y2sel/Ds_x;

    if (X11 > ad->x + ad->r || X22 < ad->x - ad->r ||
        Y11 >  ad->y +  ad->r || Y22 <  ad->y -  ad->r) return 0;
    if(ad->r==0) return 0;
    if (luk_pomX(ad,X11,Y11,Y22)) return 1;
    if (luk_pomX(ad,X22,Y11,Y22)) return 1;
    if (luk_pomY(ad,Y11,X11,X22)) return 1;
    if (luk_pomY(ad,Y22,X11,X22)) return 1;

    x0=ad->x+ad->r*cos(ad->kat1);
    y0=ad->y+ad->r*sin(ad->kat1);
    if (punkt_w_prostokacie(x0,y0)) return 1;
    x0=ad->x+ad->r*cos(ad->kat2);
    y0=ad->y+ad->r*sin(ad->kat2);
    if (punkt_w_prostokacie(x0,y0)) return 1;
    return 0;
}

int luk_w_prostokacie(LUK *ad)
{ double x,y;
  x=ad->x+ad->r*cos(ad->kat1);
  y=ad->y+ad->r*sin(ad->kat1);
  if (!punkt_w_prostokacie(x,y)) return 0;
  x=ad->x+ad->r*cos(ad->kat2);
  y=ad->y+ad->r*sin(ad->kat2);
  if (!punkt_w_prostokacie(x,y)) return 0;
  if(ad->r==0) return 0;
  return !(luk_pomX(ad,X1,Y1,Y2) || luk_pomX(ad,X2,Y1,Y2) ||
	   luk_pomY(ad,Y1,X1,X2) || luk_pomY(ad,Y2,X1,X2) ) ;
}

int prostokat_luk(LUK *ad)
{ double x0,y0;
  long x,y,r;
  int pom,k1,k2;
  x=jednostkiN(ad->x);
  y=jednostkiN(ad->y);
  r=jednostkiN(ad->r);
  if (X1sel>x+r || X2sel<x-r ||
      Y1sel>y+r || Y2sel<y-r) return 0;
  if(ad->r==0) return 0;
  pom=luk_pomX(ad,X1,Y1,Y2)+luk_pomX(ad,X2,Y1,Y2)+
      luk_pomY(ad,Y1,X1,X2)+luk_pomY(ad,Y2,X1,X2);
  x0=ad->x+ad->r*cos(ad->kat1);
  y0=ad->y+ad->r*sin(ad->kat1);
  k1=punkt_w_prostokacie(x0,y0);
  x0=ad->x+ad->r*cos(ad->kat2);
  y0=ad->y+ad->r*sin(ad->kat2);
  k2=punkt_w_prostokacie(x0,y0);
  if(k1 && !k2) return 1;           /*pierwszy koniec  */
  if(k2 && !k1) return 2;           /*drugi koniec     */
  if(k1 && k2 && !pom) return 3;    /*luk w prostokacie*/
  if(!k1 && !k2 && pom) return 4;   /*  */
  return 0;
}

int prostokat_solidarc(SOLIDARC *ad)  //
{
    double x0,y0;
    long x,y,r;
    int pom,k1,k2;
    x=jednostkiN(ad->x);
    y=jednostkiN(ad->y);
    r=jednostkiN(ad->r);
    if (X1sel>x+r || X2sel<x-r ||
        Y1sel>y+r || Y2sel<y-r) return 0;
    if(ad->r==0) return 0;
    pom=luk_pomX((LUK*)ad,X1,Y1,Y2)+luk_pomX((LUK*)ad,X2,Y1,Y2)+
        luk_pomY((LUK*)ad,Y1,X1,X2)+luk_pomY((LUK*)ad,Y2,X1,X2);
    x0=ad->x+ad->r*cos(ad->kat1);
    y0=ad->y+ad->r*sin(ad->kat1);
    k1=punkt_w_prostokacie(x0,y0);
    x0=ad->x+ad->r*cos(ad->kat2);
    y0=ad->y+ad->r*sin(ad->kat2);
    k2=punkt_w_prostokacie(x0,y0);
    if(k1 && !k2) return 1;           //pierwszy koniec
    if(k2 && !k1) return 2;           //drugi koniec
    if(k1 && k2 && !pom) return 3;    //luk w prostokacie
    if(!k1 && !k2 && pom) return 4;
    return 0;
}

int prostokat_ellipticalarc(ELLIPTICALARC *ad)
{ double xs,ys,xe,ye;
    long x,y,r;
    int pom,k1,k2;
    ELLIPTICALARC *ea;
    int numpoints;
    double xy[256];

    ea=(ELLIPTICALARC *)ad;

    numpoints=Get_EllipticalArc_Points(ea->x, ea->y, ea->rx, ea->ry, ea->angle, ea->kat1, ea->kat2, xy);
    k1=punkt_w_prostokacie(xy[0],xy[1]);
    k2=punkt_w_prostokacie(xy[numpoints-2],xy[numpoints-1]);

    pom=0;
    for (int i=0; i<(numpoints-2); i+=2)
    {
        if (!punkt_w_prostokacie(xy[i],xy[i+1]))
        {
            pom=1;
            break;
        }
    }

    if(k1 && !k2) return 1;           /*pierwszy koniec  */
    if(k2 && !k1) return 2;           /*drugi koniec     */
    if(k1 && k2 && !pom) return 3;    /*luk w prostokacie*/
    if(!k1 && !k2 && pom) return 4;   /*  */
    return 0;
}

int Point_Selected(T_Point *ptrs_point)
/*------------------------------------*/
{
	long x1, y1, x2, y2, x12, y12;
	double df_psize;
	OKRAG Okr;
    double dxy, dd, dd1 ;

    if (FALSE == Get_Point_View () && (ptrs_point->typ<7))
    {
        return 0;
    }

    if (punkt_w_prostokacie(ptrs_point->x, ptrs_point->y)) return 1;

    Okr.x = ptrs_point->x;
    Okr.y = ptrs_point->y;

    if (ptrs_point->typ<8) df_psize = Get_Point_Size () / 2;
    else df_psize = Get_Point_Size ()  / 4 ;

  if (ptrs_point->typ <= 8)
  {

      x1 = jednostkiN (ptrs_point->x - df_psize) ;
      y1 = jednostkiN (ptrs_point->y - df_psize) ;
      x2 = jednostkiN (ptrs_point->x + df_psize) ;
      y2 = jednostkiN (ptrs_point->y + df_psize) ;

      if ((ptrs_point->typ ==0) || (ptrs_point->typ ==1))
      {
          if (prostokat_odcinek(x1, y1, x2, y2)) return 1;
          if (prostokat_odcinek(x1, y2, x2, y1)) return 1;
      }

      if (ptrs_point->typ ==1)
      {
          Okr.r = Get_Point_Size() / 5;
          return (test_circle_sel(&Okr) == 1) ? 1 : 0;
      }
      else if (ptrs_point->typ == 7)  //junction
      {
          Okr.r = Get_Point_Size() / 5;

          return (test_circle_sel(&Okr) == 1) ? 1 : 0;
      }
      else if (ptrs_point->typ == 8)  //pin
      {
          Okr.r = Get_Point_Size() / 6;
          if (prostokat_odcinek(x1, y1, x2, y2)) return 1;

          return (test_circle_sel(&Okr) == 1) ? 1 : 0;
      }
  }
  else if ((ptrs_point->typ > 8)  && (ptrs_point->typ < 12)) /*pin=8, pin_g=9, pin_d=10, pin_s=11*/
  {
   df_psize = Get_Point_Size () / 3 ;
   Okr.r = df_psize;
   if (test_circle_sel(&Okr) == 1) return 1;

   x1 = jednostkiN (ptrs_point->x - df_psize) ;
   y1 = jednostkiN (ptrs_point->y - df_psize) ;
   x2 = jednostkiN (ptrs_point->x + df_psize) ;
   y2 = jednostkiN (ptrs_point->y + df_psize) ;

   if (prostokat_odcinek (x1, y1, x2, y2)) return 1 ;
  }
  else
   {

       switch (ptrs_point->typ)
      {
          case 12:
          case 13:
          case 14:
          case 15:
          case 20:
          case 21:
          case 22:
          case 23:
              df_psize = Get_Point_Size() / 4;
              Okr.r = 1.5*df_psize;

              x1 = jednostkiN (ptrs_point->x - df_psize) ;
              y1 = jednostkiN (ptrs_point->y - df_psize) ;
              x2 = jednostkiN (ptrs_point->x + df_psize) ;
              y2 = jednostkiN (ptrs_point->y + df_psize) ;

              x12 = (x1 + x2)/2;
              y12 = (y1 + y2)/2;

              dxy= jednostkiN(df_psize*2.5);  //2
              dd = jednostkiN(2*df_psize / 1.5);  //*1.0
              dd1= jednostkiN(df_psize);

              break;
          case 16:
          case 17:
          case 18:
          case 19:
          case 24:
          case 25:
          case 26:
          case 27:
              df_psize = Get_Point_Size() / 4;
              Okr.r = 2*df_psize;

              x1 = jednostkiN (ptrs_point->x - df_psize) ;
              y1 = jednostkiN (ptrs_point->y - df_psize) ;
              x2 = jednostkiN (ptrs_point->x + df_psize) ;
              y2 = jednostkiN (ptrs_point->y + df_psize) ;
              x12 = (x1 + x2)/2;
              y12 = (y1 + y2)/2;

              dxy= jednostkiN(df_psize*3);
              dd = jednostkiN(df_psize*2);
              dd1= jednostkiN(df_psize);

              break;
          case 28:
              df_psize = Get_Point_Size() / 4;
              Okr.r = 1.5*df_psize;

              x1 = jednostkiN (ptrs_point->x - df_psize) ;
              y1 = jednostkiN (ptrs_point->y - df_psize) ;
              x2 = jednostkiN (ptrs_point->x + df_psize) ;
              y2 = jednostkiN (ptrs_point->y + df_psize) ;

              x12 = (x1 + x2)/2;
              y12 = (y1 + y2)/2;

              dxy= jednostkiN(df_psize*2.325);
              dd = jednostkiN(2*df_psize*0.25);
              dd1= jednostkiN(2*df_psize*0.618);
              break;
      }

       if (test_circle_sel(&Okr) == 1) return 1;


       switch (ptrs_point->typ) {
           case 12:
           case 16:
           //bottom
           if (prostokat_odcinek(x1 - dxy, y12 - dd, x2 + dxy, y12 - dd)) return 1;
           break;
           case 13:
           case 17:
           //left
           if (prostokat_odcinek(x12 - dd, y1 - dxy, x12 - dd, y2 + dxy)) return 1;
           break;

           case 14:
           case 18:
           //right
           if (prostokat_odcinek(x12 + dd, y1 - dxy, x12 + dd, y2 + dxy)) return 1;
           break;

           case 15:
           case 19:
           //top
           if (prostokat_odcinek(x1 - dxy, y12 + dd, x2 + dxy, y12 + dd)) return 1;
           break;
           case 20:
           case 24:
               //bottom
               if (prostokat_odcinek(x1-dxy, y12-dd, x2+dxy, y12-dd)) return 1 ;
               if (prostokat_odcinek(x1-dxy, y12-dd-dd1, x2+dxy, y12-dd-dd1)) return 1 ;
               break;
           case 21:
           case 25:
               //left
               if (prostokat_odcinek(x12-dd, y1-dxy, x12-dd, y2+dxy)) return 1 ;
               if (prostokat_odcinek(x12-dd-dd1, y1-dxy, x12-dd-dd1, y2+dxy)) return 1 ;
               break;
           case 22:
           case 26:
               //right
               if (prostokat_odcinek(x12+dd, y1-dxy, x12+dd, y2+dxy)) return 1 ;
               if (prostokat_odcinek(x12+dd+dd1, y1-dxy, x12+dd+dd1, y2+dxy)) return 1 ;
               break;
           case 23:
           case 27:
               //top
               if (prostokat_odcinek(x1-dxy, y12+dd, x2+dxy, y12+dd)) return 1 ;
               if (prostokat_odcinek(x1-dxy, y12+dd+dd1, x2+dxy, y12+dd+dd1)) return 1 ;
               break;
           case 28:
               if (prostokat_odcinek(x12-dxy, y12+dd, x12-dd1, y12+dd)) return 1 ;
               if (prostokat_odcinek(x12+dd1, y12+dd, x12+dxy, y12+dd)) return 1 ;
               if (prostokat_odcinek(x12-dxy, y12-dd, x12-dd1, y12-dd)) return 1 ;
               if (prostokat_odcinek(x12+dd1, y12-dd, x12+dxy, y12-dd)) return 1 ;

               if (prostokat_odcinek(x12-dd, y12+dxy, x12-dd, y12+dd1)) return 1 ;
               if (prostokat_odcinek(x12-dd, y12-dd1, x12-dd, y12-dxy)) return 1 ;
               if (prostokat_odcinek(x12+dd, y12+dxy, x12+dd, y12+dd1)) return 1 ;
               if (prostokat_odcinek(x12+dd, y12-dd1, x12+dd, y12-dxy)) return 1 ;

               break;
      }
   }
  return 0 ;
}


int Point_Rectangle(T_Point *ptrs_point)
/*------------------------------------*/
{
    long x1, y1, x2, y2, x12, y12;
    double df_psize;
    OKRAG Okr;
    double dxy, dd, dd1 ;
    int w;

    if (!punkt_w_prostokacie(ptrs_point->x, ptrs_point->y)) return 0;

    Okr.x = ptrs_point->x;
    Okr.y = ptrs_point->y;

    if (ptrs_point->typ <= 8)
    {
        if (FALSE == Get_Point_View ())
        {
            return 0;
        }

        if (ptrs_point->typ<8) df_psize = Get_Point_Size () / 2;
        else df_psize = Get_Point_Size () / 4 ;

        x1 = jednostkiN (ptrs_point->x - df_psize) ;
        y1 = jednostkiN (ptrs_point->y - df_psize) ;
        x2 = jednostkiN (ptrs_point->x + df_psize) ;
        y2 = jednostkiN (ptrs_point->y + df_psize) ;

        if ((ptrs_point->typ ==0) || (ptrs_point->typ ==1))
        {
            if (prostokat_odcinek(x1, y1, x2, y2)!=3) return 0;
            if (prostokat_odcinek(x1, y2, x2, y1)!=3) return 0;
        }

        if (ptrs_point->typ ==1)
        {
            Okr.r = Get_Point_Size() / 5;
            if (!okrag_w_prostokacie(&Okr)) return 0;
        }
        else if (ptrs_point->typ == 7)  //junction
        {
            Okr.r = Get_Point_Size() / 5;

            if (!okrag_w_prostokacie(&Okr)) return 0;
        }
        else if (ptrs_point->typ == 8)  //pin
        {
            Okr.r = Get_Point_Size() / 6;
            if (prostokat_odcinek(x1, y1, x2, y2)!=3) return 0;

            if (!okrag_w_prostokacie(&Okr)) return 0;
        }
    }
    else if ((ptrs_point->typ > 8)  && (ptrs_point->typ < 12)) /*pin=8, pin_g=9, pin_d=10, pin_s=11*/
    {
        df_psize = Get_Point_Size () / 3 ;
        Okr.r = df_psize;

        if (!okrag_w_prostokacie(&Okr)) return 0;

        x1 = jednostkiN (ptrs_point->x - df_psize) ;
        y1 = jednostkiN (ptrs_point->y - df_psize) ;
        x2 = jednostkiN (ptrs_point->x + df_psize) ;
        y2 = jednostkiN (ptrs_point->y + df_psize) ;

        if (prostokat_odcinek (x1, y1, x2, y2)!=3) return 0 ;
    }
    else
    {

        switch (ptrs_point->typ)
        {
            case 12:
            case 13:
            case 14:
            case 15:
            case 20:
            case 21:
            case 22:
            case 23:
                df_psize = Get_Point_Size() / 4;
                Okr.r = 1.5*df_psize;
                x1 = jednostkiN (ptrs_point->x - df_psize) ;
                y1 = jednostkiN (ptrs_point->y - df_psize) ;
                x2 = jednostkiN (ptrs_point->x + df_psize) ;
                y2 = jednostkiN (ptrs_point->y + df_psize) ;

                x12 = (x1 + x2)/2;
                y12 = (y1 + y2)/2;

                dxy=(x2 - x1)*1.25;
                dd = (x2 - x1) / 1.5;  //2
                dd1=(x2-x1)/2;
                break;
            case 16:
            case 17:
            case 18:
            case 19:
            case 24:
            case 25:
            case 26:
            case 27:
                df_psize = Get_Point_Size() / 4;
                Okr.r = 2*df_psize;

                x1 = jednostkiN (ptrs_point->x - df_psize) ;
                y1 = jednostkiN (ptrs_point->y - df_psize) ;
                x2 = jednostkiN (ptrs_point->x + df_psize) ;
                y2 = jednostkiN (ptrs_point->y + df_psize) ;
                x12 = (x1 + x2)/2;
                y12 = (y1 + y2)/2;


                dxy=(x2 - x1)*1.5;
                dd = (x2 - x1);
                dd1=(x2-x1)/2;
                break;
            case 28:
                df_psize = Get_Point_Size() / 4;
                Okr.r = 1.5*df_psize;
                x1 = jednostkiN (ptrs_point->x - df_psize) ;
                y1 = jednostkiN (ptrs_point->y - df_psize) ;
                x2 = jednostkiN (ptrs_point->x + df_psize) ;
                y2 = jednostkiN (ptrs_point->y + df_psize) ;

                x12 = (x1 + x2)/2;
                y12 = (y1 + y2)/2;

                dxy=(x2 - x1)*2.325*0.5;
                dd = (x2 - x1) *0.25;  //2
                dd1=(x2-x1)*0.618;
                break;
        }

        if (!okrag_w_prostokacie(&Okr)) return 0;


        switch (ptrs_point->typ) {
            case 12:
            case 16:
                //bottom
                if (prostokat_odcinek(x1 - dxy, y12 - dd, x2 + dxy, y12 - dd)!=3) return 0;
                break;
            case 13:
            case 17:
                //left
                if (prostokat_odcinek(x12 - dd, y1 - dxy, x12 - dd, y2 + dxy)!=3) return 0;
                break;

            case 14:
            case 18:
                //right
                if (prostokat_odcinek(x12 + dd, y1 - dxy, x12 + dd, y2 + dxy)!=3) return 0;
                break;

            case 15:
            case 19:
                //top
                if (prostokat_odcinek(x1 - dxy, y12 + dd, x2 + dxy, y12 + dd)!=3) return 0;
                break;
            case 20:
            case 24:
                //bottom
                if (prostokat_odcinek(x1-dxy, y12-dd, x2+dxy, y12-dd)!=3) return 0 ;
                if (prostokat_odcinek(x1-dxy, y12-dd-dd1, x2+dxy, y12-dd-dd1)!=3) return 0 ;
                break;
            case 21:
            case 25:
                //left
                if (prostokat_odcinek(x12-dd, y1-dxy, x12-dd, y2+dxy)!=3) return 0 ;
                if (prostokat_odcinek(x12-dd-dd1, y1-dxy, x12-dd-dd1, y2+dxy)!=3) return 0 ;
                break;
            case 22:
            case 26:
                //right
                if (prostokat_odcinek(x12+dd, y1-dxy, x12+dd, y2+dxy)!=3) return 0 ;
                if (prostokat_odcinek(x12+dd+dd1, y1-dxy, x12+dd+dd1, y2+dxy)!=3) return 0 ;
                break;
            case 23:
            case 27:
                //top
                if (prostokat_odcinek(x1-dxy, y12+dd, x2+dxy, y12+dd)!=3) return 0 ;
                if (prostokat_odcinek(x1-dxy, y12+dd+dd1, x2+dxy, y12+dd+dd1)!=3) return 0 ;
                break;
            case 28:
                if (prostokat_odcinek(x12-dxy, y12+dd, x12-dd1, y12+dd)!=3) return 0 ;
                if (prostokat_odcinek(x12+dd1, y12+dd, x12+dxy, y12+dd)!=3) return 0 ;
                if (prostokat_odcinek(x12-dxy, y12-dd, x12-dd1, y12-dd)!=3) return 0 ;
                if (prostokat_odcinek(x12+dd1, y12-dd, x12+dxy, y12-dd)!=3) return 0 ;

                if (prostokat_odcinek(x12-dd, y12+dxy, x12-dd, y12+dd1)!=3) return 0 ;
                if (prostokat_odcinek(x12-dd, y12-dd1, x12-dd, y12-dxy)!=3) return 0 ;
                if (prostokat_odcinek(x12+dd, y12+dxy, x12+dd, y12+dd1)!=3) return 0 ;
                if (prostokat_odcinek(x12+dd, y12-dd1, x12+dd, y12-dxy)!=3) return 0 ;
                break;
        }
    }
    return 1 ;
}

int Point_in_Rectangle (T_Point *ptrs_point, int dwc)
/*-------------------------------------------------*/
{
  double x1, y1, x2, y2 ;
  double df_psize ;

    if (dwc==Windoww)
    {
        return Point_Rectangle(ptrs_point);
    }
    else return Point_Selected(ptrs_point);

}

int vector_okrag_wybrany(AVECTOR *V, int secondend, double r)
{
    OKRAG o;

    o.x=(secondend==0) ? V->x1 : V->x2;
    o.y=(secondend==0) ? V->y1 : V->y2;
    o.r=r;
    return okrag_wybrany (&o);
}

int Vector_Selected(AVECTOR *ptrs_vector)
/*------------------------------------*/
{  LUK l;
   OKRAG o;
   LINIA L1, L2, Ls=Ldef, Lt=Ldef, Lth=Ldef;
   WIELOKAT w=S4def;
   SOLIDARC sa=sadef;
   double df_psize, df_psize1, ra ;
   double kat, kat1, kos, koc;
   PLINIA PL, PLth;
   double katS=25.0;
   double Kp2s;
   double koc1, kos1, koc2, kos2, koc1th, kos1th;
   double n;
   float ymax, xmax;

#ifndef arrowf
#define arrowf 1.0
#endif

    df_psize = Get_Point_Size () / 4;   //disk
    df_psize1 = Get_Point_Size () / 2 ; //circle

    switch (ptrs_vector->style)
    {
        case 0:
            if (linia_wybrana((LINIA*)ptrs_vector)) return 1;
            if (vector_okrag_wybrany(ptrs_vector, 0, df_psize)) return 1;
            if (vector_okrag_wybrany(ptrs_vector, 1, df_psize)) return 1;
            break;
        case 1:
            if (linia_wybrana((LINIA*)ptrs_vector)) return 1;
            if (vector_okrag_wybrany(ptrs_vector, 0, df_psize)) return 1;
            if (vector_okrag_wybrany(ptrs_vector, 1, df_psize1)) return 1;
            break;
        case 2:
            if (linia_wybrana((LINIA*)ptrs_vector)) return 1;
            if (vector_okrag_wybrany(ptrs_vector, 0, df_psize1)) return 1;
            if (vector_okrag_wybrany(ptrs_vector, 1, df_psize)) return 1;
            break;
        case 3:
            if (linia_wybrana((LINIA*)ptrs_vector)) return 1;
            if (vector_okrag_wybrany(ptrs_vector, 0, df_psize1)) return 1;
            if (vector_okrag_wybrany(ptrs_vector, 1, df_psize1)) return 1;
            break;
        case 4:
        case 18:
            ra=Get_Point_Size () / arrowf;
            L1.x1=ptrs_vector->x1;
            L1.y1=ptrs_vector->y1;
            L1.x2=ptrs_vector->x2;
            L1.y2=ptrs_vector->y2;
            parametry_lini(&L1, &PL);
            kat1=PL.kat;
            kos=sin(PL.kat*Pi/180);
            koc=cos(PL.kat*Pi/180);
            L2.x1 = L1.x1 + ra * koc;
            L2.y1 = L1.y1 + ra * kos;
            L2.x2 = L1.x2 - ra * koc;
            L2.y2 = L1.y2 - ra * kos;

            Ls.x1=L2.x1;
            Ls.y1=L2.y1;
            Ls.x2=L1.x2;
            Ls.y2=L1.y2;

            create_solid_on_line (&Ls, &w, ra/4, ra/4, 0);
            if (wielokat_wybrany(&w)) return 1;

            Kp2s = Get_Point_Size () / (arrowf*cos(Pi*katS/180)) ;

            koc1=cos(Pi*(kat1-katS)/180);
            koc2=cos(Pi*(kat1+katS)/180);
            kos1=sin(Pi*(kat1-katS)/180);
            kos2=sin(Pi*(kat1+katS)/180);

            w.xy[2]=ptrs_vector->x1;
            w.xy[3]=ptrs_vector->y1;
            w.xy[0]=ptrs_vector->x1+Kp2s*koc1;
            w.xy[1]=ptrs_vector->y1+Kp2s*kos1;
            w.xy[4]=ptrs_vector->x1+Kp2s*koc2;
            w.xy[5]=ptrs_vector->y1+Kp2s*kos2;

            w.lp=6;
            if (wielokat_wybrany(&w)) return 1;
            break;
        case 7:
            if (linia_wybrana((LINIA*)ptrs_vector)) return 1;
            if (vector_okrag_wybrany(ptrs_vector, 0, df_psize1)) return 1;

            L1.x1 = ptrs_vector->x1;
            L1.y1 = ptrs_vector->y1;
            L1.x2 = ptrs_vector->x2;
            L1.y2 = ptrs_vector->y2;
            parametry_lini(&L1, &PL);
            kat1 = PL.kat;

            Kp2s = Get_Point_Size () / (arrowf*cos(Pi*katS/180)) ;

            koc1=cos(Pi*(kat1-katS)/180);
            koc2=cos(Pi*(kat1+katS)/180);
            kos1=sin(Pi*(kat1-katS)/180);
            kos2=sin(Pi*(kat1+katS)/180);

            w.xy[2]=ptrs_vector->x1;
            w.xy[3]=ptrs_vector->y1;
            w.xy[0]=ptrs_vector->x1+Kp2s*koc1;
            w.xy[1]=ptrs_vector->y1+Kp2s*kos1;
            w.xy[4]=ptrs_vector->x1+Kp2s*koc2;
            w.xy[5]=ptrs_vector->y1+Kp2s*kos2;

            w.empty_typ=3;
            if (wielokat_wybrany(&w)) return 1;

            break;
        case 5:
            ra=Get_Point_Size () / arrowf;
            sa.atrybut=ptrs_vector->atrybut;
            sa.warstwa=ptrs_vector->warstwa;
            sa.kolor=ptrs_vector->kolor;
            sa.x=ptrs_vector->x1;
            sa.y=ptrs_vector->y1;
            sa.r=ptrs_vector->r;
            sa.kat1=ptrs_vector->angle1;
            sa.kat2=ptrs_vector->angle2;
            sa.width1=ra/4;
            sa.width2=ra/4;
            if (solidarc_wybrany(&sa)) return 1;
            //arrow
            break;
        case 6:
            ra=Get_Point_Size () / arrowf;
            sa.atrybut=ptrs_vector->atrybut;
            sa.warstwa=ptrs_vector->warstwa;
            sa.kolor=ptrs_vector->kolor;
            sa.x=ptrs_vector->x1;
            sa.y=ptrs_vector->y1;
            sa.r=ptrs_vector->r;
            sa.kat1=ptrs_vector->angle1;
            sa.kat2=ptrs_vector->angle2;
            sa.width1=ra/4;
            sa.width2=ra/4;
            if (solidarc_wybrany(&sa)) return 1;
            //arrow
            break;
        case 8:
            memmove(&l, ptrs_vector, sizeof(NAGLOWEK));
            l.x=ptrs_vector->x1;
            l.y=ptrs_vector->y1;
            l.r=ptrs_vector->r;
            l.kat1=ptrs_vector->angle1;
            l.kat2=ptrs_vector->angle2;
            if (luk_wybrany(&l)) return 1;

            //arrow
            break;
        case 9:
            memmove(&l, ptrs_vector, sizeof(NAGLOWEK));
            l.x=ptrs_vector->x1;
            l.y=ptrs_vector->y1;
            l.r=ptrs_vector->r;
            l.kat1=ptrs_vector->angle1;
            l.kat2=ptrs_vector->angle2;
            if (luk_wybrany(&l)) return 1;

            //arrow
            break;
        case 10:
        case 17:
            if (linia_wybrana((LINIA*)ptrs_vector)) return 1;

            if (ptrs_vector->x1<ptrs_vector->x2) n=1;
            else n=-1;
            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = ptrs_vector->x1;
            Lt.y1 = ptrs_vector->y1 + n*(ptrs_vector->magnitude1/load_magnitude);
            Lt.x2 = ptrs_vector->x2;
            Lt.y2 = ptrs_vector->y2 + n*(ptrs_vector->magnitude2/load_magnitude);

            w.xy[0]=ptrs_vector->x1;
            w.xy[1]=ptrs_vector->y1;
            w.xy[2]=ptrs_vector->x2;
            w.xy[3]=ptrs_vector->y2;
            w.xy[4]=ptrs_vector->x2;
            w.xy[5]=Lt.y2;
            w.xy[6]=ptrs_vector->x1;
            w.xy[7]=Lt.y1;

            w.lp=8;
            if (wielokat_wybrany(&w)) return 1;

            break;
        case 11:
            if (linia_wybrana((LINIA*)ptrs_vector)) return 1;

            if (ptrs_vector->y1<ptrs_vector->y2) n=1;
            else n=-1;
            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = ptrs_vector->x1 - n*(ptrs_vector->magnitude1/load_magnitude);
            Lt.y1 = ptrs_vector->y1;
            Lt.x2 = ptrs_vector->x2 - n*(ptrs_vector->magnitude2/load_magnitude);
            Lt.y2 = ptrs_vector->y2;

            w.xy[0]=ptrs_vector->x1;
            w.xy[1]=ptrs_vector->y1;
            w.xy[2]=ptrs_vector->x2;
            w.xy[3]=ptrs_vector->y2;
            w.xy[4]=Lt.x2;
            w.xy[5]=ptrs_vector->y2;
            w.xy[6]=Lt.x1;
            w.xy[7]=ptrs_vector->y1;

            w.lp=8;
            if (wielokat_wybrany(&w)) return 1;

            break;
        case 12:
            if (linia_wybrana((LINIA*)ptrs_vector)) return 1;

            parametry_lini((LINIA*)ptrs_vector, &PL);

            kos1=sin(Pi*(PL.kat+90)/180);
            koc1=cos(Pi*(PL.kat+90)/180);

            n=1;
            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = ptrs_vector->x1 + n*(ptrs_vector->magnitude1/load_magnitude)*koc1;
            Lt.y1 = ptrs_vector->y1 + n*(ptrs_vector->magnitude1/load_magnitude)*kos1;
            Lt.x2 = ptrs_vector->x2 + n*(ptrs_vector->magnitude2/load_magnitude)*koc1;
            Lt.y2 = ptrs_vector->y2 + n*(ptrs_vector->magnitude2/load_magnitude)*kos1;

            w.xy[0]=ptrs_vector->x1;
            w.xy[1]=ptrs_vector->y1;
            w.xy[2]=ptrs_vector->x2;
            w.xy[3]=ptrs_vector->y2;
            w.xy[4]=Lt.x2;
            w.xy[5]=Lt.y2;
            w.xy[6]=Lt.x1;
            w.xy[7]=Lt.y1;

            w.lp=8;
            if (wielokat_wybrany(&w)) return 1;

            break;
        case 13:
            if (linia_wybrana((LINIA*)ptrs_vector)) return 1;

            if (ptrs_vector->x1<ptrs_vector->x2)
            {
                n=1;
                w.xy[1]=w.xy[3]=max(ptrs_vector->y1, ptrs_vector->y2);
                ymax=max(ptrs_vector->y1, ptrs_vector->y2);
            }
            else
            {
                n=-1;
                w.xy[1]=w.xy[3]=min(ptrs_vector->y1, ptrs_vector->y2);
                ymax=min(ptrs_vector->y1, ptrs_vector->y2);
            }
            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = ptrs_vector->x1;
            Lt.y1 = ymax + n*(ptrs_vector->magnitude1/load_magnitude);
            Lt.x2 = ptrs_vector->x2;
            Lt.y2 = ymax + n*(ptrs_vector->magnitude2/load_magnitude);


            w.xy[0]=ptrs_vector->x1;
            w.xy[2]=ptrs_vector->x2;
            w.xy[4]=ptrs_vector->x2;
            w.xy[5]=Lt.y2;
            w.xy[6]=ptrs_vector->x1;
            w.xy[7]=Lt.y1;

            w.lp=8;
            if (wielokat_wybrany(&w)) return 1;

            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=ptrs_vector->x1;
            L1.y2=ptrs_vector->y1;
            if (linia_wybrana(&L1)) return 1;

            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=ptrs_vector->x2;
            L1.y2=ptrs_vector->y2;
            if (linia_wybrana(&L1)) return 1;

            break;
        case 14:
            if (linia_wybrana((LINIA*)ptrs_vector)) return 1;

            if (ptrs_vector->y1<ptrs_vector->y2)
            {
                n=1;
                w.xy[0]=w.xy[2]=min(ptrs_vector->x1, ptrs_vector->x2);
                xmax=min(ptrs_vector->x1, ptrs_vector->x2);
            }
            else
            {
                n=-1;
                w.xy[0]=w.xy[2]=max(ptrs_vector->x1, ptrs_vector->x2);
                xmax=max(ptrs_vector->x1, ptrs_vector->x2);
            }

            Lt.x1 = xmax - n*(ptrs_vector->magnitude1/load_magnitude);
            Lt.y1 = ptrs_vector->y1;
            Lt.x2 = xmax - n*(ptrs_vector->magnitude2/load_magnitude);
            Lt.y2 = ptrs_vector->y2;

            w.xy[1]=ptrs_vector->y1;
            w.xy[3]=ptrs_vector->y2;
            w.xy[4]=Lt.x2;
            w.xy[5]=ptrs_vector->y2;
            w.xy[6]=Lt.x1;
            w.xy[7]=ptrs_vector->y1;

            w.lp=8;
            if (wielokat_wybrany(&w)) return 1;

            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=ptrs_vector->x1;
            L1.y2=ptrs_vector->y1;
            if (linia_wybrana(&L1)) return 1;

            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=ptrs_vector->x2;
            L1.y2=ptrs_vector->y2;
            if (linia_wybrana(&L1)) return 1;

            break;
        case 15:
            if (linia_wybrana((LINIA*)ptrs_vector)) return 1;

            parametry_lini((LINIA*)ptrs_vector, &PL);

            kos1=sin(Pi*(PL.kat+90)/180);
            koc1=cos(Pi*(PL.kat+90)/180);

            L1.x1 = ptrs_vector->x1;
            L1.y1 = ptrs_vector->y1;
            L1.x2 = ptrs_vector->x2;
            L1.y2 = ptrs_vector->y2;

            Lth.x1 = (ptrs_vector->x1 + ptrs_vector->x2)/2 + (ptrs_vector->r/depth_magnitude)*koc1;  //thermal_depth_size
            Lth.y1 = (ptrs_vector->y1 + ptrs_vector->y2)/2 + (ptrs_vector->r/depth_magnitude)*kos1;
            Lth.x2 = (ptrs_vector->x1 + ptrs_vector->x2)/2 - (ptrs_vector->r/depth_magnitude)*koc1;
            Lth.y2 = (ptrs_vector->y1 + ptrs_vector->y2)/2 - (ptrs_vector->r/depth_magnitude)*kos1;

            parametry_lini(&Lth, &PLth);
            kos1th=sin(Pi*(PLth.kat+90)/180);
            koc1th=cos(Pi*(PLth.kat+90)/180);

            Lt.x1 = Lth.x1 + (ptrs_vector->magnitude1/thermal_magnitude)*koc1th;
            Lt.y1 = Lth.y1 + (ptrs_vector->magnitude1/thermal_magnitude)*kos1th;
            Lt.x2 = Lth.x2 + (ptrs_vector->magnitude2/thermal_magnitude)*koc1th;
            Lt.y2 = Lth.y2 + (ptrs_vector->magnitude2/thermal_magnitude)*kos1th;

            w.xy[0]=Lth.x1;
            w.xy[1]=Lth.y1;
            w.xy[2]=Lth.x2;
            w.xy[3]=Lth.y2;
            w.xy[4]=Lt.x2;
            w.xy[5]=Lt.y2;
            w.xy[6]=Lt.x1;
            w.xy[7]=Lt.y1;

            w.lp=8;
            if (wielokat_wybrany(&w)) return 1;

            break;
        case 16:
            if (linia_wybrana((LINIA*)ptrs_vector)) return 1;

            memmove(&o, ptrs_vector, sizeof(NAGLOWEK));
            o.x=ptrs_vector->x1;
            o.y=ptrs_vector->y1;
            o.r=ptrs_vector->r;
            if (okrag_wybrany(&o)) return 1;

            break;
        default:
            break;
    }
    return 0;
}



int Pcx_in_Rectangle (B_PCX *pcx, int dwc)
/*-------------------------------------------------*/
{ REAL_PCX *rpcx;
  double x1, y1, x2, y2 ;
  double width, height;
  //double width_d;
  double pcx_x;
  double kos, koc;
  double x_[4], y_[4];
  double xx_[4], yy_[4];


  rpcx = (REAL_PCX*) pcx->pcx;

  if (rpcx->header.bits_per_pixel ==  8)
   {

	  width = pcx->i_x;
	  height = pcx->i_y;
     pcx_x=pcx->x; // - pcx->dx;
   }
    else
     {

	  width = pcx->i_x;
	  height = pcx->i_y;
       pcx_x=pcx->x; // + (pcx->dx*8.0);
     }

    width *= pcx->dx;
    height *= pcx->dy;

   if (Check_if_Equal(pcx->kat, 0.0) == TRUE)
   {

	   x1 = pcx_x;
	   y1 = pcx->y;
	   x2 = pcx_x + width;
	   y2 = pcx->y - height;

	   xx_[0] = pcx_x;
	   yy_[0] = pcx->y;

	   xx_[1] = pcx_x;
	   yy_[1] = pcx->y - height;

	   xx_[2] = pcx_x + width;
	   yy_[2] = pcx->y - height;

	   xx_[3] = pcx_x + width;
	   yy_[3] = pcx->y;
   }
   else
   {

	   kos = sin(pcx->kat);
	   koc = cos(pcx->kat);

	   x_[0] = pcx_x;
	   y_[0] = pcx->y;

	   x_[1] = pcx_x;
	   y_[1] = pcx->y - height;

	   Rotate_Point(kos, koc, x_[0], y_[0], x_[1], y_[1], &x_[1], &y_[1]);

	   x_[2] = pcx_x + width;
	   y_[2] = pcx->y - height;

	   Rotate_Point(kos, koc, x_[0], y_[0], x_[2], y_[2], &x_[2], &y_[2]);

	   x_[3] = pcx_x + width;
	   y_[3] = pcx->y;

	   Rotate_Point(kos, koc, x_[0], y_[0], x_[3], y_[3], &x_[3], &y_[3]);

	   memmove(xx_, x_, sizeof(x_));
	   memmove(yy_, y_, sizeof(y_));

#ifndef LINUX
       //good for Clion
	   qsort(x_, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);
	   qsort(y_, 4, sizeof(double), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_val);

       //good for VS
       //qsort(x_, 4, sizeof(double),(int(*)(const double*, const double*)) qsort_by_val);
       //qsort(y_, 4, sizeof(double),(int(*)(const double*, const double*)) qsort_by_val);
#else
#ifndef MACOS
       qsort(x_, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
       qsort(y_, 4, sizeof(double), (__compar_fn_t) qsort_by_val);
#else
       qsort(x_, 4, sizeof(double), qsort_by_val);
       qsort(y_, 4, sizeof(double), qsort_by_val);
#endif
#endif

	   x1 = x_[0];
	   y1 = y_[0];
	   x2 = x_[3];
	   y2 = y_[3];
   }

  if (dwc==Windoww)
   {

     return (punkt_w_prostokacie(x1, y1) &&
	  punkt_w_prostokacie(x1, y2) &&
	  punkt_w_prostokacie(x2, y1) &&
	  punkt_w_prostokacie(x2, y2)) ;

    }
    else
     {


	  return ((prostokat_odcinek(jednostkiN(xx_[0]), jednostkiN(yy_[0]), jednostkiN(xx_[1]), jednostkiN(yy_[1]))) ||
		  (prostokat_odcinek(jednostkiN(xx_[1]), jednostkiN(yy_[1]), jednostkiN(xx_[2]), jednostkiN(yy_[2]))) ||
		  (prostokat_odcinek(jednostkiN(xx_[2]), jednostkiN(yy_[2]), jednostkiN(xx_[3]), jednostkiN(yy_[3]))) ||
		  (prostokat_odcinek(jednostkiN(xx_[3]), jednostkiN(yy_[3]), jednostkiN(xx_[4]), jednostkiN(yy_[4]))));
     }	  
}

static int okragtest(int iy0, double xx,double Y1,double Y2,
				   double x,double y,double r)
{ double d,yy;
  d=r*r-(xx-x)*(xx-x);
  if(d<0) return 0;
  yy=y+iy0*sqrt(d);
  return ( Y1<=yy && yy<=Y2  );
}

static int test_circle_sel (OKRAG *ad)
/*-----------------------------------*/
{
  int iy0,ix0;

  if (X1 > ad->x + ad->r || X2 < ad->x - ad->r ||
      Y1 >  ad->y +  ad->r || Y2 <  ad->y -  ad->r) return 0;
  if (okrag_w_prostokacie(ad) )  return 1;
  if (ad->y > Y1+(Y2-Y1)/2) iy0=-1;
  else iy0=+1;
  if ( okragtest(iy0,X1,Y1,Y2,ad->x,ad->y,ad->r) ) return 1;
  if ( okragtest(iy0,X2,Y1,Y2,ad->x,ad->y,ad->r) ) return 1;
  if (ad->x > X1+(X2-X1)/2) ix0=-1;
  else ix0=+1;
  if ( okragtest(ix0,Y1,X1,X2,ad->y,ad->x,ad->r) ) return 1;
  if ( okragtest(ix0,Y2,X1,X2,ad->y,ad->x,ad->r) ) return 1;
  return -1 ;
}


int okrag_wybrany (OKRAG *ad)
/*-------------------------*/
{
  return (test_circle_sel (ad) == 1) ? 1 : 0 ;
}

static int test_ellipse_sel (ELLIPSE *ad)
/*---------------------------------------*/
{
    double df_si, df_co;
    double df_x, df_y;
    WIELOKAT w=S4def;

    df_si = sin (ad->angle) ;
    df_co = cos (ad->angle) ;

    df_x = ad->x + ad->rx ;
    df_y = ad->y + ad->ry ;
    Rotate_Point (df_si, df_co, ad->x, ad->y, df_x, df_y, &df_x, &df_y) ;
    w.xy[0]=df_x; w.xy[1]=df_y;

    df_x = ad->x + ad->rx ;
    df_y = ad->y - ad->ry ;
    Rotate_Point (df_si, df_co, ad->x, ad->y, df_x, df_y, &df_x, &df_y) ;
    w.xy[2]=df_x; w.xy[3]=df_y;

    df_x = ad->x - ad->rx ;
    df_y = ad->y - ad->ry ;
    Rotate_Point (df_si, df_co, ad->x, ad->y, df_x, df_y, &df_x, &df_y) ;
    w.xy[4]=df_x; w.xy[5]=df_y;

    df_x = ad->x - ad->rx ;
    df_y = ad->y + ad->ry ;
    Rotate_Point (df_si, df_co, ad->x, ad->y, df_x, df_y, &df_x, &df_y) ;
    w.xy[6]=df_x; w.xy[7]=df_y;

    if (ad->obiekt==Ofilledellipse)
    {
        if (ad->widoczny=Point_in_Solid(&w, X, Y)) return 1;
    }

    if (ad->widoczny=wielokat_wybrany(&w)) return 1;

    return 0;

}

static int test_ellipse_sel_prec (ELLIPSE *ad, int win)
/*---------------------------------------------------*/
{
    int iy0,ix0;
    double xy[256];
    double *ptr_xy=xy;
    int n_xy;
    int i, j ;
    int ret ;
    long x0, y0;
    long xyj[256];
    int sum=0;

    n_xy=Get_Ellipse_Points ((double)ad->x, (double)ad->y, (double)ad->rx, (double)ad->ry, (double)ad->angle, xy);

    xyj[0]=jednostkiN(xy[0]);
    xyj[1]=jednostkiN(xy[1]);

    for (i = 0 ; i < (n_xy-2) ; i += 2)
    {
        xyj[i+2]=jednostkiN(xy[i+2]);
        xyj[i+3]=jednostkiN(xy[i+3]);
        ret = prostokat_odcinek(xyj[i], xyj[i+1], xyj[i+2], xyj[i+3]);
        if ((ret>0) && (win==Cross))
        {
            return ret;
        }
        sum+=ret;
    }

    ret = prostokat_odcinek (xyj[i], xyj[i+1], xyj[0], xyj[1]);
    sum+=ret;

    if ((ret>0) && (win==Cross))
    {
        return ret;
    }

    return (sum==((n_xy)/2*3)) ;
}

static int test_ellipticalarc_sel_prec (ELLIPTICALARC *ad, int win)
/*--------------------------------------------------------------*/
{
    int iy0,ix0;
    double xy[256];
    double *ptr_xy=xy;
    int n_xy;
    int i, j ;
    int ret ;
    long x0, y0;
    long xyj[256];
    int sum=0;

    n_xy=Get_EllipticalArc_Points ((double)ad->x, (double)ad->y, (double)ad->rx, (double)ad->ry, (double)ad->angle,  (double)  ad->kat1, (double) ad->kat2, xy);

    xyj[0]=jednostkiN(xy[0]);
    xyj[1]=jednostkiN(xy[1]);

    for (i = 0 ; i < (n_xy-2) ; i += 2)
    {
        xyj[i+2]=jednostkiN(xy[i+2]);
        xyj[i+3]=jednostkiN(xy[i+3]);
        ret = prostokat_odcinek(xyj[i], xyj[i+1], xyj[i+2], xyj[i+3]);
        if ((ret>0)  && (win==Cross))
        {
            return ret;
        }
        sum+=ret;
    }

    return (sum==((n_xy-2)/2*3)) ;

}


int elipsa_wybrana (ELLIPSE *ad)
/*---------------------------*/
{
    return (test_ellipse_sel (ad) >= 1) ? 1 : 0 ;

}

int lukeliptyczny_wybrany (ELLIPTICALARC *ad)
/*-----------------------------------------*/
{
    //return (test_ellipticalarc_sel_prec (ad) == 1) ? 1 : 0 ;

    return (test_ellipticalarc_sel_prec (ad, Cross) >= 1) ? 1 : 0 ;

    //return (test_ellipticalarc_sel_prec (ad));
}

int elipsa_wybrana_prec (ELLIPSE *ad)
/*--------------------------------*/
{
    return (test_ellipse_sel_prec (ad, Cross) >= 1) ? 1 : 0 ;

}

int elipsa_w_prostokacie (ELLIPSE *ad)
/*----------------------------------*/
{
    return (test_ellipse_sel_prec (ad, Windoww) >= 1) ? 1 : 0 ;

}

int lukeliptyczny_wybrany_prec (ELLIPTICALARC *ad)
/*----------------------------------------------*/
{

    return (test_ellipticalarc_sel_prec (ad, Cross) >= 1) ? 1 : 0 ;
}

int lukeliptyczny_w_prostokacie (ELLIPTICALARC *ad)
/*----------------------------------------------*/
{

    return (test_ellipticalarc_sel_prec (ad, Windoww) >= 1) ? 1 : 0 ;
}

BOOL Check_Draw_Pieslice (OKRAG *ad)
/*--------------------------------*/
{
  int ret ;

  ret = test_circle_sel (ad) ;
  if (ret == -1)
  {
    ret = 0 ;
    if ((X1- ad->x) * (X1- ad->x) + (Y1- ad->y) * (Y1- ad->y)  <= ad->r * ad->r)
    {
      ret = 1 ;
    }
  }
  return ret ;
}

BOOL okrag_visible (OKRAG *ad)
/*--------------------------------*/
{
    if (Xp > ad->x + ad->r || Xk < ad->x - ad->r ||
        Yp >  ad->y +  ad->r || Yk <  ad->y -  ad->r) return 0;

    return 1 ;
}


int wielokat_wybrany(WIELOKAT *w)
{
  int i, j ;
  int ret ;


  if ((global_point_in_solid) && (w->obiektt3==O3HatchSolid))
  {
	  //Normalize_Solid(w);
	  if (Point_in_Solid(w, X, Y)) return 1;
  }
  j = 0 ;
  for (i = 0 ; i < (int)(w->lp - 2) ; i += 2)
  {
    if(prostokat_odcinek(jednostkiN(w->xy[i]),jednostkiN(w->xy[i+1]),
	jednostkiN(w->xy[i+2]),jednostkiN(w->xy[i+3])))
    {
      return 1;
    }
    tab_ixy1 [j++] = ix1 ;
    tab_ixy1 [j++] = iy1 ;
  }
  ret = (prostokat_odcinek (jednostkiN(w->xy[w->lp-2]), jednostkiN(w->xy[w->lp-1]),
      jednostkiN(w->xy[0]), jednostkiN(w->xy[1])) > 0) ? 1 : 0 ;
  tab_ixy1 [j++] = ix1 ;
  tab_ixy1 [j++] = iy1 ;
  lp = j ;


  return ret ;
}

BOOL linia_visible(LINIA *L)
{
    double minx = BIGNOF;
    double miny = BIGNOF;
    double maxx = -BIGNOF;
    double maxy = -BIGNOF;
    int i;

    if (L->x1 < minx) minx = L->x1;
    if (L->x1 > maxx) maxx = L->x1;
    if (L->x2 < minx) minx = L->x2;
    if (L->x2 > maxx) maxx = L->x2;

    if (L->y1 < miny) miny = L->y1;
    if (L->y1 > maxy) maxy = L->y1;
    if (L->y2 < miny) miny = L->y2;
    if (L->y2 > maxy) maxy = L->y2;

    if (Xp > maxx || Xk < minx ||
        Yp >  maxy || Yk <  miny) return 0;
    return 1;
}

BOOL wielokat_visible(WIELOKAT *w)
{
    double minx = BIGNOF;
    double miny = BIGNOF;
    double maxx = -BIGNOF;
    double maxy = -BIGNOF;
    int i;

    for (i = 0 ; i < (int)(w->lp - 2) ; i += 2)
    {
        if (w->xy[i] < minx) minx = w->xy[i];
        if (w->xy[i] > maxx) maxx = w->xy[i];

        if (w->xy[i+1] < miny) miny = w->xy[i+1];
        if (w->xy[i+1] > maxy) maxy = w->xy[i+1];
    }
    if (Xp > maxx || Xk < minx ||
        Yp >  maxy || Yk <  miny) return 0;

    return 1;
}

BOOL wielokat_visible_in_frame(WIELOKAT *w, RECTD *rect)
{
    double minx = BIGNOF;
    double miny = BIGNOF;
    double maxx = -BIGNOF;
    double maxy = -BIGNOF;
    int i;

    for (i = 0 ; i < (int)(w->lp - 1) ; i += 2)
    {
        if (w->xy[i] < minx) minx = w->xy[i];
        if (w->xy[i] > maxx) maxx = w->xy[i];

        if (w->xy[i+1] < miny) miny = w->xy[i+1];
        if (w->xy[i+1] > maxy) maxy = w->xy[i+1];
    }

    if (rect->xp > maxx || rect->xk < minx ||
            rect->yp >  maxy || rect->yk <  miny) return 0;
    return 1;
}

int solidarc_wybrany_factory(SOLIDARC *sa, int side)
{
    double halfwidth1, halfwidth2, halfmidwidth;
    double axis1, axis2, midaxis;
    double midkat;
    double kos, koc;
    POINTD p1l, p2l, p3l, p4l, p5l, p6l, p7l, p8l, p9l, p10l;
    LUK louter=ldef;
    LUK linner=ldef;
    LINIA lstart=Ldef;
    LINIA lend=Ldef;
    int retl;
    double kat1, kat2;

    axis1 = sa->axis1;
    axis2 = sa->axis2;
    halfwidth1=sa->width1/2;
    halfwidth2=sa->width2/2;

    halfmidwidth=(halfwidth1+halfwidth2)/2;
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

    if (side!=2)
    {
        lstart.x1=p7l.x;
        lstart.y1=p7l.y;
        lstart.x2=p10l.x;
        lstart.y2=p10l.y;
        lstart.warstwa=sa->warstwa;

        if (linia_wybrana (&lstart)) return 1;
    }

    //finding points 5 and 8
    koc = cos(kat2);
    kos = sin(kat2);
    p5l.x = sa->x + (sa->r - halfwidth1 - axis1) * koc;
    p5l.y = sa->y + (sa->r - halfwidth1 - axis1) * kos;
    p8l.x = sa->x + (sa->r + halfwidth1 - axis1) * koc;
    p8l.y = sa->y + (sa->r + halfwidth1 - axis1) * kos;

    if (side!=1)
    {
    lend.x1=p5l.x;
    lend.y1=p5l.y;
    lend.x2=p8l.x;
    lend.y2=p8l.y;
    lend.warstwa=sa->warstwa;

    if (linia_wybrana (&lend)) return 1;
    }

    koc=cos (midkat);
    kos=sin (midkat);
    p6l.x = sa->x + (sa->r - halfmidwidth - midaxis) * koc ;
    p6l.y = sa->y + (sa->r - halfmidwidth - midaxis) * kos ;
    p9l.x = sa->x + (sa->r + halfmidwidth - midaxis) * koc ;
    p9l.y = sa->y + (sa->r + halfmidwidth - midaxis) * kos ;

    retl=get_3p_arc(&linner, &p5l, &p6l, &p7l);
    linner.warstwa=sa->warstwa;
    if (luk_wybrany(&linner)) return 1;
    retl=get_3p_arc(&louter, &p8l, &p9l, &p10l);
    louter.warstwa=sa->warstwa;
    if (luk_wybrany(&louter)) return 1;

    return 0;
}

int solidarc_elements_factory(SOLIDARC *sa, int side, LINIA *L_left, LINIA *L_right, LUK *l_inner, LUK *l_outer)
{
    double halfwidth1, halfwidth2, halfmidwidth;
    double axis1, axis2, midaxis;
    double midkat;
    double kos, koc;
    POINTD p1l, p2l, p3l, p4l, p5l, p6l, p7l, p8l, p9l, p10l;

    int retl;
    double kat1, kat2;

    axis1 = sa->axis1;
    axis2 = sa->axis2;
    halfwidth1=sa->width1/2;
    halfwidth2=sa->width2/2;

    halfmidwidth=(halfwidth1+halfwidth2)/2;
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

    if (L_right!=NULL)
    {
        L_right->x1=p10l.x;
        L_right->y1=p10l.y;
        L_right->x2=p7l.x;
        L_right->y2=p7l.y;
        L_right->warstwa=sa->warstwa;
        L_right->widoczny=sa->widoczny;
        L_right->kolor=sa->kolor;
    }

    //finding points 5 and 8
    koc = cos(kat2);
    kos = sin(kat2);
    p5l.x = sa->x + (sa->r - halfwidth1 - axis1) * koc;
    p5l.y = sa->y + (sa->r - halfwidth1 - axis1) * kos;
    p8l.x = sa->x + (sa->r + halfwidth1 - axis1) * koc;
    p8l.y = sa->y + (sa->r + halfwidth1 - axis1) * kos;

    if (L_left!=NULL)
    {
        L_left->x1=p5l.x;
        L_left->y1=p5l.y;
        L_left->x2=p8l.x;
        L_left->y2=p8l.y;
        L_left->warstwa=sa->warstwa;
        L_left->widoczny=sa->widoczny;
        L_left->kolor=sa->kolor;
    }


    koc=cos (midkat);
    kos=sin (midkat);
    p6l.x = sa->x + (sa->r - halfmidwidth - midaxis) * koc ;
    p6l.y = sa->y + (sa->r - halfmidwidth - midaxis) * kos ;
    p9l.x = sa->x + (sa->r + halfmidwidth - midaxis) * koc ;
    p9l.y = sa->y + (sa->r + halfmidwidth - midaxis) * kos ;

    retl=get_3p_arc(l_inner, &p5l, &p6l, &p7l);
    l_inner->warstwa=sa->warstwa;
    l_inner->widoczny=sa->widoczny;
    l_inner->kolor=sa->kolor;

    retl=get_3p_arc(l_outer, &p8l, &p9l, &p10l);
    l_outer->warstwa=sa->warstwa;
    l_outer->widoczny=sa->widoczny;
    l_outer->kolor=sa->kolor;

    return 1;
}

int solidarc_w_prostokacie_factory(SOLIDARC *sa)
{

    double halfwidth1, halfwidth2, halfmidwidth;
    double axis1, axis2, midaxis;
    double midkat;
    double kos, koc;
    POINTD p1l, p2l, p3l, p4l, p5l, p6l, p7l, p8l, p9l, p10l;
    LUK louter=ldef;
    LUK linner=ldef;

    int retl;
    double kat1, kat2;
    int w;

    axis1 = sa->axis1;
    axis2 = sa->axis2;
    halfwidth1=sa->width1/2;
    halfwidth2=sa->width2/2;

    halfmidwidth=(halfwidth1+halfwidth2)/2;
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

    koc = cos(kat1);
    kos = sin(kat1);
    p7l.x = sa->x + (sa->r - halfwidth2 - axis2) * koc;
    p7l.y = sa->y + (sa->r - halfwidth2 - axis2) * kos;
    p10l.x = sa->x + (sa->r + halfwidth2 - axis2) * koc;
    p10l.y = sa->y + (sa->r + halfwidth2 - axis2) * kos;

    koc = cos(kat2);
    kos = sin(kat2);
    p5l.x = sa->x + (sa->r - halfwidth1 - axis1) * koc;
    p5l.y = sa->y + (sa->r - halfwidth1 - axis1) * kos;
    p8l.x = sa->x + (sa->r + halfwidth1 - axis1) * koc;
    p8l.y = sa->y + (sa->r + halfwidth1 - axis1) * kos;

    //finding points 6 and 9 (base arc)
    koc=cos (midkat);
    kos=sin (midkat);
    p6l.x = sa->x + (sa->r - halfmidwidth - midaxis) * koc ;
    p6l.y = sa->y + (sa->r - halfmidwidth - midaxis) * kos ;
    p9l.x = sa->x + (sa->r + halfmidwidth - midaxis) * koc ;
    p9l.y = sa->y + (sa->r + halfmidwidth - midaxis) * kos ;

    retl=get_3p_arc(&linner, &p5l, &p6l, &p7l);
    linner.warstwa=sa->warstwa;
    if (!luk_w_prostokacie(&linner)) return 0;
    retl=get_3p_arc(&louter, &p8l, &p9l, &p10l);
    louter.warstwa=sa->warstwa;
    if (!luk_w_prostokacie(&louter)) return 0;

    return 1;
}

int solidarc_w_prostokacie(SOLIDARC *sa)
{
    int ret;
    SOLIDARC sa1, sa2;
    double midkat;
    double midwidth;
    double kat1, kat2, dkat;

    ret=1;

    kat1 = Angle_Normal(sa->kat1);
    kat2 = Angle_Normal(sa->kat2);

    if (kat2<kat1) kat2+=Pi2;
    dkat=kat2-kat1;
    if (Check_if_LE(dkat, Pi_))
        return solidarc_w_prostokacie_factory(sa);

    else
    {
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

        return (solidarc_w_prostokacie_factory(&sa1) & solidarc_w_prostokacie_factory(&sa2));
    }

    return ret;

}

int solidarc_wybrany(SOLIDARC *sa)
{
    int ret;
    SOLIDARC sa1, sa2;
    double midkat;
    double midwidth;
    double kat1, kat2, dkat;

    ret=1;

    kat1 = Angle_Normal(sa->kat1);
    kat2 = Angle_Normal(sa->kat2);

    if (kat2<kat1) kat2+=Pi2;
    dkat=kat2-kat1;
    if (Check_if_LE(dkat, Pi_))
        return solidarc_wybrany_factory(sa, 0);

    else
    {
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

        return (solidarc_wybrany_factory(&sa1, 1) | solidarc_wybrany_factory(&sa2, 2));

    }

    return ret;
}

int solidarc_elements(SOLIDARC *sa, LINIA *L_left, LINIA *L_right, LUK *l_inner1, LUK *l_inner2, LUK *l_outer1, LUK *l_outer2)
{
    int ret, ret1, ret2;
    SOLIDARC sa1, sa2;
    double midkat;
    double midwidth;
    double kat1, kat2, dkat;

    ret=1;

    kat1 = Angle_Normal(sa->kat1);
    kat2 = Angle_Normal(sa->kat2);

    if (kat2<kat1) kat2+=Pi2;
    dkat=kat2-kat1;
    if (Check_if_LE(dkat, Pi_))
        return solidarc_elements_factory(sa, 0, L_left, L_right, l_inner1, l_outer1);
    else
    {
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

        ret1=solidarc_elements_factory(&sa1, 1,  NULL, L_right, l_inner1, l_outer1);
        ret2=solidarc_elements_factory(&sa2, 2,  L_left, NULL, l_inner2, l_outer2);

        return ret1 + ret2;
    }
}

BOOL Check_DArea_in_Last_Solid (void)
/*---------------------------------*/
{
  BOOL b_ret ;
  int i ;

  b_ret = FALSE;
  for (i = 2 ; i < lp ; i += 2)
  {
    if (tab_ixy1 [0] != tab_ixy1 [i])
    {
	  b_ret = SCREEN_BY_SOLID_OVERRIDIND;
      //b_ret = TRUE ;
      break ;
    }
  }
  if (b_ret == FALSE)
  {
    return b_ret ;
  }
  b_ret = FALSE;
  for (i = 3 ; i < lp ; i += 2)
  {
    if (tab_ixy1 [1] != tab_ixy1 [i])
    {
      b_ret = TRUE ;
      break ;
    }
  }
  return b_ret ;
}


BOOL Point_in_Solid (WIELOKAT *ptr_w, double x, double y)
/*------------------------------------------------------*/
{
  BOOL b_ret ;

  b_ret = TRUE ;

  if (ptr_w->lp == 8 && ptr_w->obiektt1 != SIMPW)
  {
    WIELOKAT w1 = Stdef, w2 = Stdef ;

    Get_Rec_2TrAngle (ptr_w, &w1, &w2) ;
    Normalize_Solid (&w1) ;
    if (TRUE == Point_in_Solid (&w1, x, y))
      return b_ret ;
    else
    {
      Normalize_Solid (&w2) ;
      if (TRUE == Point_in_Solid (&w2, x, y))
	return b_ret ;
    }
    return FALSE ;
  }
  else if (ptr_w->lp == 8)
  {
	  WIELOKAT w1 = Stdef, w2 = Stdef;

	  Get_Rec_Division(ptr_w, &w1, &w2);
	  Normalize_Solid(&w1);
	  if (TRUE == Point_in_Solid(&w1, x, y))
		  return b_ret;
	  else
	  {
		  Normalize_Solid(&w2);
		  if (TRUE == Point_in_Solid(&w2, x, y))
			  return b_ret;
	  }
	  return FALSE;
  }
  else
  {

	  double as_x = x - ptr_w->xy[0];
	  double as_y = y - ptr_w->xy[1];

	  BOOL s_ab = (ptr_w->xy[2] - ptr_w->xy[0]) * as_y - (ptr_w->xy[3] - ptr_w->xy[1]) * as_x > 0;
	  if ((ptr_w->xy[4] - ptr_w->xy[0]) * as_y - (ptr_w->xy[5] - ptr_w->xy[1]) * as_x > 0 == s_ab) return FALSE;
	  if ((ptr_w->xy[4] - ptr_w->xy[2]) * (y - ptr_w->xy[3]) - (ptr_w->xy[5] - ptr_w->xy[3]) * (x - ptr_w->xy[2]) > 0 != s_ab) return FALSE;
	  return TRUE;

  }
  return b_ret ;
}

BOOL Point_in_SolidArc (SOLIDARC *ptr_sa, double x, double y)
{
    double dot,det;
    double df_x0, df_y0;
    double vx1, vy1, vx2, vy2;
    double df_angle;
    double dist, dist_inner, dist_outer;
    double kat1, kat2;

    //check if point x,y lays inside solidarc

    vx1 = x - ptr_sa->x;
    vy1 = y - ptr_sa->y;
    vx2 = 1.0;
    vy2 = 0.0;

    dot = vx1 * vx2 + vy1 * vy2;     // Dot product between [vx1, vy1] and [vx2, vy2]
    det = vx1 * vy2 - vy1 * vx2;     // Determinant
    df_angle = Angle_Normal(-atan2(det, dot));

    dist=sqrt((vx1*vx1)+(vy1*vy1));

    dist_inner=ptr_sa->r-(max(ptr_sa->width1,ptr_sa->width2) /2);
    dist_outer=ptr_sa->r+(max(ptr_sa->width1,ptr_sa->width2) /2);

    kat1= Angle_Normal(ptr_sa->kat1);
    kat2=Angle_Normal(ptr_sa->kat2);

    if (kat2<kat1) kat2+=Pi2;
    if (df_angle<kat1) df_angle+=Pi2;

    if ((dist>=dist_inner) && (dist<=dist_outer) &&
            (df_angle>=kat1) && (df_angle<=kat2))  return TRUE;
    else return FALSE;
}


BOOL Point_in_Quad (QUAD *quad, double x, double y)
/*-----------------------------------------------*/
{
    BOOL b_ret ;
    WIELOKAT w=S4def;

    memcpy(&w.xy, quad, sizeof(QUAD));

    return Point_in_Solid (&w, x, y);
}


void out_sel_off(double X, double Y)
{
    flip_screen();
}


void out_sel_on(double X, double Y)
{
	int x, y, x1, y1;
	float f_x, f_y;

    float background_r;
    float background_g;
    float background_b;
    int rgb, r,g,b;
    float A, A1;
    float brightness_b, brightness_c;
    int color, color_r, color_g, color_b;
    int cursor_color;
    int xx, yy;
    BITMAP *second_screen;
    int pXp1, pYp1, pXk1, pYk1;

    pXp1=get_pXp();
    pXk1=get_pXk();
    pYp1=get_pYp();
    pYk1=get_pYk();

    background_r = 0.0;
    background_g = 0.0;
    background_b = 0.0;

	f_x = X;
	f_y = Y;
	setwritemode(COPY_PUT);
	setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);

    x = (int)pikseleX0(f_x);
    y = (int)pikseleY0(f_y);

    x1 = (int)pikseleX(f_x);
    y1 = (int)pikseleY(f_y);

    setcolor(kolory.cur);
    cursor_color = palette_color[kolory.cur];

    //checking background color
    //TEMPORARY
    if (INTL_CURSOR)
    {

        okno_all();

        A=0;
        A1=0;

        for (xx = x1 - (int)(sel.size*sk_x); xx < x1 + (int)(sel.size*sk_x); xx++)
        {
            for (yy = y1 - sel.size; yy <= y1 + sel.size; yy += (2*sel.size))
            {
                if ((xx>pXp1) && (xx<pXk1) && (yy>pYk1) && (yy<pYp1))
                {
                    color = _getpixel32(screen, xx, yy);
                    r = getr(color);
                    g = getg(color);
                    b = getb(color);
                    A1 += 1.0;
                    background_r = (background_r * A + r) / A1;
                    background_g = (background_g * A + g) / A1;
                    background_b = (background_b * A + b) / A1;
                    A = A1;
                }
            }
        }

        for (int yy = y1 - sel.size; yy < y1 + sel.size; yy++)
        {
            for (xx=x1 - (int)(sel.size*sk_x); xx<=x1 + (int)(sel.size*sk_x); xx += (2*sel.size))
            {
                if ((xx>pXp1) && (xx<pXk1) && (yy>pYk1) && (yy<pYp1))
                {
                    color = _getpixel32(screen, xx, yy);
                    r = getr(color);
                    g = getg(color);
                    b = getb(color);
                    A1 += 1.0;
                    background_r = (background_r * A + r) / A1;
                    background_g = (background_g * A + g) / A1;
                    background_b = (background_b * A + b) / A1;
                    A = A1;
                }
            }
        }


        brightness_b = 0.299 * background_r + 0.587 * background_g + 0.114 * background_b;
        color_r = getr(cursor_color);
        color_g = getg(cursor_color);
        color_b = getb(cursor_color);
        brightness_c = 0.299 * color_r + 0.587 * color_g + 0.114 * color_b;

        if (fabs(brightness_c - brightness_b) < BRIGHTNESS_DIFF)
        {

            cursor_color = 0xFFFFFF - palette_color[kolory.cur];
            setcolor(cursor_color);
        }
    }

	okno_r();

	rectangle(x - (int)(sel.size*sk_x), y - sel.size, x + (int)(sel.size*sk_x), y + sel.size);
	okno_all();
}


