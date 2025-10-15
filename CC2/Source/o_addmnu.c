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

#define __ADDMENU__
#pragma warning(disable : 4996)
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<stddef.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<values.h>
#include<stdio.h>
#include "bib_e.h"
#include "o_addmnu.h"
#include "o_libfun.h"
#include "o_dialog.h"
#include "o_image.h"
#include "rysuj_e.h"
#include "b_textw.h"
#include "b_menu.h"
#include "alf_res.h"
#include "o_measur.h"

#include "menu.h"

#define MB_ABORTRETRYIGNORE         0x00000002L

static BOOL b_active_w = FALSE ;
static int(*SW[7])();

static BOOL previewed=FALSE;

static int  nooop1(void)
{return 0;}
#define noop (int ( *)(TMENU  *menu))nooop1
extern void color_bar(void);
extern double Get_Point_Size (void) ;
extern void Set_Point_Size (double df_ps) ;
extern BOOL Get_Point_View (void) ;
extern void Set_Point_View (BOOL) ;
extern void Redraw_Point (BOOL) ;
extern void Redraw_Vector (BOOL) ;
extern void Point_Sel (void) ;
extern void JumpB (void) ;
extern void JumpW (void) ;
extern void JumpL (void) ;
extern void not_implemented (void) ;
extern void Wez_warstwe (void) ;
extern void Wez_kolor (void) ;
extern void Wez_typ (void) ;
extern void Find_Text (void) ;
extern void Information_about (void);
extern void getviewsettings(struct viewporttype  *viewport);
extern void setviewport(int left, int top, int right, int bottom, int clip);
extern void setfillstyle_(int pattern, int color);
extern int colors(void);
extern void view_line_type(LINIA *L);
extern void set_increased(int x, int y);
extern void set_original_window(void);
extern void set_last_window(void);
extern int Solid04(char *adr0, char *adr1, int kolor);
extern void ObiektTok_Layer (long_long *off, long_long offk, long_long *ad, int obiekt, int warstwa);
extern void Redraw_Text (BOOL b_draw);
extern void return_menu_par0 (char *pole, char * par);
extern void standard_func(void);
extern void zero_lw_counter(void);
extern void beep(long sound);
extern void calc_bspline(int lp, int n, float points[], int p1, float* out_x, float* out_y);
extern void redraw_preview(int dark);
extern void Redraw_Line_225(BOOL b_draw);
extern void Redraw_Arc_225(BOOL b_draw);
extern void Redraw_Circle_225(BOOL b_draw);
extern void Redraw_Bitmap_Outline(int kolor0);
extern void Set_Bitmap_Outline(BOOL b_pv);
extern void Redraw_Spline(BOOL b_draw);
extern void Set_Control_Points_View(BOOL b_pv);
extern void Set_Ell_Verno (int i_ell_verno);
extern int Get_Ell_Verno (void);
extern int Get_Ellipse_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double xy[]);
extern int Get_EllipticalArc_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double df_sangle, double df_eangle, double xy[]);
extern void bar1_margin(void);
extern int solidarc_elements(SOLIDARC *sa, LINIA *L_left, LINIA *L_right, LUK *l_inner1, LUK *l_inner2, LUK *l_outer1, LUK *l_outer2);
extern void set_bnodes(SPLINE *ad, float *ad_xy, int *node_count_);
extern void calc_tangents(int closed, int *node_count, double *bnodes_2_tangent);
extern NODE bnodes[MAX_NODES];
extern void get_control_points_reversed(NODE n1, NODE n2, float points[]);
extern void get_control_points(NODE n1, NODE n2, float points[8]);
extern void get_control_points_end(NODE n1, NODE n2, float points[]);
extern void set_decimal_format(char *text, double l, double precision);
extern double view_vector_scale;

extern double depth_magnitude; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double thermal_magnitude; //units per mm  default 1 Celsius per 1 mm on drawing paper
extern double load_magnitude; //units per mm  default 10kN/m force per 1 mm on drawing paper
extern double flood_magnitude; //units per mm  default 10kN/m² load per 1 mm on drawing paper
extern double force_magnitude; //units per mm  default 10kN force per 1 mm on drawing paper
extern double moment_magnitude; //units per mm  default 10kNm force per 1 mm radius on drawing paper
extern double displacement_magnitude; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
extern double rotation_magnitude;
extern double dim_precision;

void SkalaE(void);
void skala_format (int);
void skala_window(double xmin, double xmax, double ymin, double ymax, int dark);
void SkalaW (void);
double get_skala(void);
static void solidarc_place (SOLIDARC *sa, double *xmin, double *ymin, double *xmax, double *ymax);
//static
void okrag_place (OKRAG *o, double *xmin, double *ymin, double *xmax, double *ymax);
static void tekst_place (TEXT *t, double *xmin, double *ymin, double *xmax, double *ymax);
void luk_place (LUK *l, double *xmin, double *ymin, double *xmax, double *ymax);

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

#define MPMAX 7 //6

#define YES _YES_
#define NO _NO_

typedef struct
{
  double skala;
  double X,Y;
  double Xp, Yp;
  char flags;
} VIEW;

#define MAXLASTVIEW 16
static int no_view = 0;
static double krok_old = 2.5;
static VIEW  tab_last_view [MAXLASTVIEW];
static VIEW  last_view_preview;

#define MAXPLACEMARKER 5
static VIEW  tab_place_marker [MAXPLACEMARKER];

static  OKNO O;
extern BOOL Auto_Pan;

extern void X_Y(void);
extern void Xp0_Yp0(double xp, double yp);
extern void layer_info (void) ;
extern void clean_kbd(void);

        void menu_par_new(char * pole, const char * par);
static void skala_menup(void);
static BOOL skala_w (double xmin, double xmax, double ymin, double ymax, int dark) ;

/*TMENU mTryb uzyto w b_message*/
/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/

static void  nooop (void)
/*------------------------*/
{}

void reset_previewed(void)
{
    previewed=FALSE;
}

int Save_Message0_Image (void)
/*----------------------------*/
{
  struct viewporttype viewinfo;
  int x1, y1, x2, y2 ;
  int image_no ;

  getviewsettings (&viewinfo);
  setviewport(0, 0, getmaxx(),getmaxy(), 1);

  x1 = 0 ;
  y1 = 0 ;
#define PKu130x 210   		/*przeniesione z bib_e.c*/
  x2 = maxX - PKu130x ;
  y2 = ED_INF_HEIGHT - 1 ;
  image_no = Get_Image_Tmp (x1, y1, x2, y2) ;
  setfillstyle_(SOLID_FILL, BKCOLOR) ;
  bar1_margin();
  setviewport(viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1);
  return image_no ;
}


void Put_Message0_Image (int image_no)
/*------------------------------------*/
{
  struct viewporttype viewinfo;

  if (image_no != -1)
  {
    getviewsettings (&viewinfo) ;
    setviewport (0, 0, getmaxx (), getmaxy (), 1) ;
    Put_Image_Tmp (image_no, 0, 0) ;
    setviewport (viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1) ;
  }
}

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/


static void kolorL(void)
/*-----------------------------*/
{
  int i_poz, i_color ;
  char  sk[48], *sk1;

  i_poz = mKolorAC.foff + mKolorAC.poz ;
  i_color = Menu_No_to_Color_No (i_poz) ;
  Cur_offd(X,Y);
  OkragG.kolor=PointG.kolor=TextG.kolor=LukG.kolor = LiniaG.kolor = SolidG.kolor = VectorG.kolor = i_color ;
  Layers[Current_Layer].color=i_color;
  Cur_ond (X,Y) ;
  strcpy(sk,(*mKolorAC.pola)[i_poz].txt);
  sk1=strpbrk(sk," ");
  menu_par_new((*mParametrym.pola)[4].txt,++sk1);
  color_bar();
}

static void kolorX(void)
/*-----------------------------*/
{
  int i_poz, i_color ;

  char *sk1;
  int retval_no = 1 ;
  char buf [100];

  i_color=colors();  
  Cur_offd(X, Y);
  OkragG.kolor = PointG.kolor = TextG.kolor = LukG.kolor = LiniaG.kolor = SolidG.kolor = VectorG.kolor = i_color;
  Layers[Current_Layer].color = i_color;
  Cur_ond(X, Y);
  i_poz=16;
  strcpy(buf,(*mKolorAC.pola)[i_poz].txt);
  sk1=strpbrk(buf," ");
  color_bar();
  return;
}


static void typ_linii(void)
{
  char  sk[48] ;
  int typ ;

  Cur_offd (X,Y) ;
  typ = ( LiniaG.typ & 224 );  /*grubosc*/
  typ += mTypyLinii.poz + mTypyLinii.foff ;
  
  LiniaG.typ=OkragG.typ=LukG.typ=VectorG.typ=typ ;
  Layers[Current_Layer].line_type = typ ;
  view_line_type( &LiniaG ) ;
  Cur_ond (X,Y) ;
  strcpy(sk,(*mTypyLinii.pola)[mTypyLinii.poz + mTypyLinii.foff].txt) ;
  menu_par_new((*mParametrym.pola)[6].txt,sk);
}


static void grubosc_linii(void)
{ char  sk[48];
  int typ;
  Cur_offd(X,Y);
  typ = ( LiniaG.typ & 31 );  /*typ linii*/
  typ = typ + 32 /*LiczbaWzorcowLinii*/ * mGruboscLinii.poz ;
  
  LiniaG.typ=OkragG.typ=LukG.typ=VectorG.typ=typ;
  Layers[Current_Layer].line_type = typ;
  view_line_type(&LiniaG);
  Cur_ond (X,Y) ;
  strcpy(sk,(*mGruboscLinii.pola)[mGruboscLinii.poz].txt);
  menu_par_new((*mParametrym.pola)[5].txt,sk);
}

static void swobodny_blok_off(void)
{
  static char stT[2]=NO;
  if (swobodny_blok==FALSE) return;
  swobodny_blok=FALSE;
  menu_par_new((*mParametrym.pola)[12].txt,stT);
 }

static void swobodny_blok_on(void)
{
  static char stT[2]=YES;
  if (swobodny_blok==TRUE) return;
  swobodny_blok=TRUE;
  menu_par_new((*mParametrym.pola)[12].txt,stT);
 }
 
static void swobodny_tekst_off(void)
{
  static char stT[2]=NO;
  if (swobodny_tekst==FALSE) return;
  swobodny_tekst=FALSE;
  menu_par_new((*mParametrym.pola)[13].txt,stT);
 }

static void swobodny_tekst_on(void)
{
  static char stT[2]=YES;
  if (swobodny_tekst==TRUE) return;
  swobodny_tekst=TRUE;
  menu_par_new((*mParametrym.pola)[13].txt,stT);
 }


static void normalizacja_tekstu_off(void)
{
  static char stT[2]=NO;
  if (normalize_text==FALSE) return;
  normalize_text=FALSE;
  menu_par_new((*mParametrym.pola)[22].txt,stT);
 }

static void normalizacja_tekstu_on(void)
{
  static char stT[2]=YES;
  if (normalize_text==TRUE) return;
  normalize_text=TRUE;
  menu_par_new((*mParametrym.pola)[22].txt,stT);
 }



int get_lock_prof_aktual(void)
{
 return options1.lock_prof_aktual;
}

void put_lock_prof_aktual(int lock)
{
  options1.lock_prof_aktual = lock;
}
 
static void scale_DIM_off(void)
{
  static char stT[2]=NO;
  if (options1.scale_DIM==0) return;
  options1.scale_DIM=0;
  menu_par_new((*mParametrym.pola)[14].txt,stT);
 }

static void scale_DIM_on(void)
{
  static char stT[2]=YES;
  if (options1.scale_DIM==1) return;
  options1.scale_DIM=1;
  menu_par_new((*mParametrym.pola)[14].txt,stT);
 }  
 
static void ciagniecie_linii_off(void)
{
  static char stT[2]=NO;
  if (ciagniecie_linii==FALSE) return;
  ciagniecie_linii=FALSE;
  menu_par_new((*mParametrym.pola)[15].txt,stT);
 }

static void ciagniecie_linii_on(void)
{
  static char stT[2]=YES;
  if (ciagniecie_linii==TRUE) return;
  ciagniecie_linii=TRUE;
  menu_par_new((*mParametrym.pola)[15].txt,stT);
 }  
 
static void rozcinanie_linii_off(void)
{
  static char stT[2]=NO;
  if (rozcinanie_linii==FALSE) return;
  rozcinanie_linii=FALSE;
  menu_par_new((*mParametrym.pola)[16].txt,stT);
 }

static void rozcinanie_linii_on(void)
{
  static char stT[2]=YES;
  if (rozcinanie_linii==TRUE) return;
  rozcinanie_linii=TRUE;
  menu_par_new((*mParametrym.pola)[16].txt,stT);
 }   
 
static void ukrywanie_typu_off(void)
{
  static char stT[2]=NO;
  if (ukrywanie_typu==FALSE) return;
  ukrywanie_typu=FALSE;
  menu_par_new((*mParametrym.pola)[17].txt,stT);
 }

static void ukrywanie_typu_on(void)
{
  static char stT[2]=YES;
  if (ukrywanie_typu==TRUE) return;
  ukrywanie_typu=TRUE;
  menu_par_new((*mParametrym.pola)[17].txt,stT);
 }   
 
static void type_view_off(void)
{
  static char stT[2]=NO;
  if (type_view==FALSE) return;
  type_view=FALSE;
  redraw () ;

  menu_par_new((*mParametrym.pola)[18].txt,stT);

 }

static void type_view_on(void)
{
  static char stT[2]=YES;
  if (type_view==TRUE) return;
  type_view=TRUE;
  redraw () ;

  menu_par_new((*mParametrym.pola)[18].txt,stT);

 }   
 
static void wypelnianie_pcx_off(void)
{
  static char stT[2]=NO;
  if (options1.fill_pcx_solid==0) return;
  options1.fill_pcx_solid=0;
  redraw () ;
  menu_par_new((*mParametrym.pola)[19].txt,stT);
 } 
 
static void wypelnianie_pcx_on(void)
{
  static char stT[2]=YES;
  if (options1.fill_pcx_solid==1) return;
  options1.fill_pcx_solid=1;
  redraw () ;
  menu_par_new((*mParametrym.pola)[19].txt,stT);
 }

 static void view_only_c_l_off(void)
{
  static char stT[2]=NO;
  if (options1.view_only_current_layer==0) return;
  options1.view_only_current_layer=0;
  redraw () ;

  menu_par_new((*mParametrym.pola)[20].txt,stT);

  
 } 
 
static void view_only_c_l_on(void)
{
  static char stT[2]=YES;
  if (options1.view_only_current_layer==1) return;
  options1.view_only_current_layer=1;
  redraw () ;

  menu_par_new((*mParametrym.pola)[20].txt,stT);

 }

 static void buffering_pcx_enable_off(void)
{
  static char stT[2]=NO;
  if (options1.ignore_buffering_pcx==1) return;
  options1.ignore_buffering_pcx=1;
//  redraw () ;
  menu_par_new((*mParametrym.pola)[21].txt,stT);
 } 
 
static void buffering_pcx_enable_on(void)
{
  static char stT[2]=YES;
  if (options1.ignore_buffering_pcx==0) return;
  options1.ignore_buffering_pcx=0;
  regen_ctx=TRUE;
//  redraw () ;
  menu_par_new((*mParametrym.pola)[21].txt,stT);
 }  

static void Orto_on(void)
{
  static char stT[2]=YES;
  if (orto) return;
  Cur_offd(X, Y) ;
  orto=1;
  Orto_Dir = I_Orto_NoDir;
  Cur_ond (X,Y);
  menu_par_new((*mParametrym.pola)[3].txt,stT);
  layer_info () ;
 }

void Orto_on_(void)
 {
   Orto_on();
 }
 
static void Choose_Layer(int nr_warstwy)
{ int w_color, w_typ;

  if (nr_warstwy > (No_Layers - 1)) 
   {
    komunikat(117);
    return;
   } 
   
  Cur_offd (X, Y) ; 
  
  if ((nr_warstwy >= 0) && (nr_warstwy<256)) Current_Layer=nr_warstwy;
  w_color=Layers[Current_Layer].color;
  w_typ=Layers[Current_Layer].line_type;
  
  OkragG.kolor=PointG.kolor=TextG.kolor=LukG.kolor = LiniaG.kolor= SolidG.kolor = VectorG.kolor = w_color ;
  LiniaG.typ=OkragG.typ=LukG.typ=VectorG.typ=w_typ ;
  OkragG.warstwa=PointG.warstwa=TextG.warstwa=LukG.warstwa = LiniaG.warstwa= SolidG.warstwa = VectorG.warstwa = nr_warstwy ;
  
  layer_info () ;
  view_line_type( &LiniaG ) ;
  
  Cur_ond (X,Y) ;
} 
 
int Choose_Layer_1_(void)
{
 Choose_Layer(0); 
 return 0;
}

int Choose_Layer_2_(void) 
{
 Choose_Layer(1); 
 return 0;
}

int Choose_Layer_3_(void) 
{
 Choose_Layer(2); 
 return 0;
}

int Choose_Layer_4_(void)
{
 Choose_Layer(3); 
 return 0;
}

int Choose_Layer_5_(void)
{
 Choose_Layer(4); 
 return 0;
}

int Choose_Layer_6_(void)
{
 Choose_Layer(5); 
 return 0;
}

int Choose_Layer_7_(void)
{
 Choose_Layer(6); 
 return 0;
}

int Choose_Layer_8_(void)
{
 Choose_Layer(7); 
 return 0;
}

int Choose_Layer_9_(void)
{
 Choose_Layer(8); 
 return 0;
}

int Choose_Layer_10_(void)
{
 Choose_Layer(9); 
 return 0;
}


int Inc_Layer_(void)
{ int w_color, w_typ;
  int nr_warstwy;
  
  nr_warstwy = Current_Layer + 1;
  
  if (nr_warstwy > (No_Layers - 1))
    { 
      komunikat(117);
      return 0;
    }
    
  Cur_offd (X, Y) ;  
  
  Current_Layer=nr_warstwy;
  
  w_color=Layers[Current_Layer].color;
  w_typ=Layers[Current_Layer].line_type;
  
  OkragG.kolor=PointG.kolor=TextG.kolor=LukG.kolor = LiniaG.kolor = VectorG.kolor = w_color ;
  LiniaG.typ=OkragG.typ=LukG.typ=VectorG.typ=w_typ ;

  layer_info () ;
  view_line_type( &LiniaG ) ;
  
  Cur_ond (X, Y) ;
  return 0;
}

int Dec_Layer_(void)
{ int w_color, w_typ;
  int nr_warstwy;
  
  nr_warstwy = Current_Layer - 1;
  
  if (nr_warstwy<=0) 
    { 
      komunikat(116);
      return 0;
    }
    
  Cur_offd (X, Y) ;    
  
  Current_Layer=nr_warstwy;
  
  w_color=Layers[Current_Layer].color;
  w_typ=Layers[Current_Layer].line_type;
  
  OkragG.kolor=PointG.kolor=TextG.kolor=LukG.kolor = LiniaG.kolor= VectorG.kolor = w_color ;
  LiniaG.typ=OkragG.typ=LukG.typ=VectorG.typ=w_typ ;

  layer_info () ;
  view_line_type( &LiniaG ) ;
  
  Cur_ond (X, Y) ; 
  return 0;
}


int All_Layers_On_(void)
{ int i;
  for (i=0; i<No_Layers; i++) Layers[i].on=1;
  redraw();
  Change=TRUE;
  return 0;
}

int Reverse_Layers_(void)
{ int i;
  BOOL first_set=FALSE;
  int no_warstw_on=0;

  ErrList(0);
  layer_info () ;
 
  last_active_layer=Current_Layer;

  for (i=0; i<No_Layers; i++)
  {
	  if (Layers[i].on==1) Layers[i].on=0; 
	  else 
	  {
		Layers[i].on=1;
		no_warstw_on++;
		if (!first_set) 
		{
		  if (last_active_layer_arch<0) 
		  {
             Current_Layer=i;
			 first_set=TRUE;
		  }
		  else if (last_active_layer_arch==i)
		  {
			 Current_Layer=i;
			 first_set=TRUE;
		  }
		}
	  }
  }

  //koncowe sprawdzenie obecnosci warstw;
  if (no_warstw_on==0) 
  {
    Layers[0].on=1;
	Current_Layer=0;
  }

  last_active_layer_arch=last_active_layer;
  if (bitmap_exist)
  {
	  Image_Free();
	  Ini_Global_Object();
	  //sprawdzenie czy potrzeba regenerowac bitmapy
	  for (i = 0; i < No_Layers; i++)  //ewentualnie 256
	  {
		  if (Warstwy_On_PCX[i].check == 1)
		  {
			  if (Warstwy_On_PCX[i].on != Layers[i].on)
			  {
				  regen_ctx = TRUE;
			  }
		  }
	  }
  }
  redraw();
  layer_info () ;
  if (no_warstw_on==0) 
  {
    ErrList(210);
  }

  Change=TRUE;

  return 0;
}

static void Orto_off(void)
{ static char stN[2]="N";
  if (!orto) return;
  Cur_offd(X,Y);
  orto=0;
  Cur_ond(X,Y);
  menu_par_new((*mParametrym.pola)[3].txt,stN);
  layer_info () ;
}

void Orto_off_(void)
 {
   Orto_off();
 }

int Change_Orto (void)
/*--------------------*/
{
  if (orto == 1)
  {
    Orto_off () ;
  }
  else
  {
    Orto_on () ;
  }
  return 0 ;
}


static void grid_on_(void)
{
  if (grid_) return;
  grid_=1;

  menu_par_new((*mParametrym.pola)[2].txt,YES);

  redraw();
 }

static  void grid_off_(void)
{
  if (!grid_) return;
  grid_=0;

  menu_par_new((*mParametrym.pola)[2].txt,NO);

  redraw();
}

static void KrokK (void)
//---------------------
{
  static double g [] = {2.5, 1.25, 0.25, 0.5, 1, 5, 10} ;
  static char *ptrsz_snap [] = {u8"2.5","1.25","0.25","0.5","1","5", "10"} ;
  int poz ;

  poz = mKrok_k.poz + mKrok_k.foff ;
  if (snap_ && krok_s == jednostkiOb (g [poz])) return ;
  krok_s = jednostkiOb (g [poz]) ;
  snap_ = 1 ;
  kasowanie_licznikow=TRUE;

  menu_par_new ((*mParametrym.pola)[0].txt, ptrsz_snap [poz]) ;

  layer_info () ;
}

static  void KrokKX (void)
//------------------------
{
  
  char sk [MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d;
  

  sk [0] = '\0' ;
  DF_to_String (sk, "%-6.2f", milimetryob (krok_s), 0) ;
  if (!read_esc(sk,60,7))  return;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  if ( d <= 0 )
  {
    ErrList (30) ;
    return ;
  }
  krok_s = jednostkiOb (d) ;
  snap_ = 1 ;
  DF_to_String (sk, "%-6.2f", milimetryob (krok_s), 6) ;
  kasowanie_licznikow=TRUE;

  menu_par_new ((*mParametrym.pola)[0].txt, sk) ;

  menu_par_new ((*mKrok_k.pola)[7].txt, sk) ;
  layer_info () ;
}

/*static*/ void KrokM(void)
{ char sk[48];
  snap_=0;
  krok_s=jednostkiY(1);
  strcpy(sk,"Min");
  kasowanie_licznikow=TRUE;

  menu_par_new((*mParametrym.pola)[0].txt,sk);

  layer_info () ;
}

static void Gestosc (void)
//-----------------------
{
  static double g [] = {2.5, 1, 5, 10, 100} ;
  static char *ptrsz_grid [] = {u8"2.5", "1", "5", "10", "100"} ;
  int poz;

  poz = mGestosc.poz + mGestosc.foff ;
  if(krok_g == jednostkiOb (g [poz])) return ;
  krok_g = jednostkiOb (g [poz]) ;

  menu_par_new ((*mParametrym.pola)[1].txt, ptrsz_grid [poz]) ;

  if (grid_) redraw () ;
}

static void GestoscX(void)
//----------------------
{
  char sk [MaxTextLen] ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d;

  sk [0] = '\0' ;
  DF_to_String (sk, "%-6.2f", milimetryob (krok_g), 0) ;
  if (!read_esc(sk,60,8))  return;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  if ( d <= 0 )
  {
    ErrList (30) ;
    return ;
  }
  krok_g = jednostkiOb (d) ;
  DF_to_String (sk, "%-6.2f", milimetryob (krok_g), 6) ;
  menu_par_new ((*mGestosc.pola)[5].txt, sk) ;

  menu_par_new ((*mParametrym.pola)[1].txt, sk) ;

  if (grid_) redraw () ;
}


static BOOL change_view (double xp_v, double yp_v , double x_v,
			double y_v,  double skala_v)
/*--------------------------------------------------*/
{
/*czy nastapilo zmniejszenie rozmiarow rysunku*/
  if ( x_v < Xmin || x_v > Xmax  || y_v < Ymin || y_v > Ymax ||
	  xp_v < Xmin || yp_v < Ymin )
  {
    ErrList (13) ;
    return FALSE ;
  }
  skala = skala_v ;
  X = x_v ;
  Y = y_v ;

  d_2d();
  Xp0_Yp0 (xp_v, yp_v);
  if(!snap_) krok_s = jednostkiY (1) ;
  
  skala_menup () ;
  redraw() ;
  return TRUE ;
}


static BOOL change_view2(double xp_v, double yp_v, double x_v,
	double y_v, double skala_v)
	/*--------------------------------------------------*/
{
	/*czy nastapilo zmniejszenie rozmiarow rysunku*/
	if (x_v < Xmin || x_v > Xmax || y_v < Ymin || y_v > Ymax ||
		xp_v < Xmin || yp_v < Ymin)
	{
		ErrList(13);
		return FALSE;
	}
	skala = skala_v;
	X = x_v;
	Y = y_v;

	d_2d();
	Xp0_Yp0(xp_v, yp_v);
	skala_menup();
	return TRUE;
}

void Clear_View (void)
/*--------------------*/
{
  no_view = 0 ;
}

void Save_View (void)
/*-------------------*/
{
  if (no_view == MAXLASTVIEW )
  {
    memmove ( &tab_last_view[0], &tab_last_view[1], sizeof(VIEW) * (no_view-1));
    no_view--;
  }
  tab_last_view [no_view].skala = skala;
  tab_last_view [no_view].X = X;
  tab_last_view [no_view].Y = Y;
  tab_last_view [no_view].Xp = Xp;
  tab_last_view [no_view].Yp = Yp;
  no_view++;
}

void Save_View_Preview(void)
/*-------------------*/
{
	last_view_preview.skala = skala;
	last_view_preview.X = X;
	last_view_preview.Y = Y;
	last_view_preview.Xp = Xp;
	last_view_preview.Yp = Yp;
}

static void last_view(void)
/*---------------------------*/
{
	double xp_v, yp_v, x_v, y_v, skala_v;

	if (no_view < 2) return;  /*komunikat*/
	no_view -= 2;
	skala_v = tab_last_view[no_view].skala;
	x_v = tab_last_view[no_view].X;
	y_v = tab_last_view[no_view].Y;
	xp_v = tab_last_view[no_view].Xp;
	yp_v = tab_last_view[no_view].Yp;
	if (FALSE == change_view(xp_v, yp_v, x_v, y_v, skala_v))
	{
		tab_last_view[no_view].flags = 0;
	}
	no_view++;
}




void Restore_View_Preview(void)
/*---------------------------*/
{
	double xp_v, yp_v, x_v, y_v, skala_v;

	skala_v = last_view_preview.skala;
	x_v = last_view_preview.X;
	y_v = last_view_preview.Y;
	xp_v = last_view_preview.Xp;
	yp_v = last_view_preview.Yp;
	if (FALSE == change_view2(xp_v, yp_v, x_v, y_v, skala_v))
	{
		;
	}
}


void Ini_Place_Marker (void)
/*---------------------------*/
{
  int i;
  for (i = 0; i < MAXPLACEMARKER; i++)
   tab_place_marker [ i ].flags = 0;
}


static void set_marker (void)
/*--------------------------*/
{
  int poz;

  poz = mSetMarker.poz + mSetMarker.foff;
  tab_place_marker [poz].flags = 1;
  tab_place_marker [poz].skala = skala;
  tab_place_marker [poz].X = X;
  tab_place_marker [poz].Y = Y;
  tab_place_marker [poz].Xp = Xp;
  tab_place_marker [poz].Yp = Yp;
}

static void find_marker (void)
/*---------------------------*/
{
  double xp_fm, yp_fm , x_fm, y_fm, skala_fm;
  int poz;

  poz = mFindMarker.poz + mFindMarker.foff;
  if ( tab_place_marker [poz].flags == 0) return;
  skala_fm = tab_place_marker[poz].skala ;
  x_fm =     tab_place_marker[poz].X;
  y_fm =     tab_place_marker[poz].Y;
  xp_fm =    tab_place_marker[poz].Xp;
  yp_fm =    tab_place_marker[poz].Yp;
  if (FALSE == change_view (xp_fm, yp_fm , x_fm, y_fm, skala_fm))
  {
     tab_place_marker [poz].flags = 0;
  }
}

void put_setmarker (int poz)
/*---------------------------------*/
{
  mSetMarker.poz=poz;
  mSetMarker.foff=0;
  set_marker();
}

void put_findmarker (int poz)
/*---------------------------------*/
{
  mFindMarker.poz=poz;
  mFindMarker.foff=0;
  find_marker();
}



#ifdef aaaaa
static void find_marker (void)
/*---------------------------*/
{
  double xp_fm, yp_fm , x_fm, y_fm, skala_fm;
  int poz;

  poz = mFindMarker.poz + mFindMarker.foff;
  if ( tab_place_marker [poz].flags == 0) return;
  skala_fm = tab_place_marker[poz].skala ;
  x_fm =     tab_place_marker[poz].X;
  y_fm =     tab_place_marker[poz].Y;
  xp_fm =    tab_place_marker[poz].Xp;
  yp_fm =    tab_place_marker[poz].Yp;
/*czy nastapilo zmniejszenie rozmiarow rysunku*/
  if ( x_fm < Xmin || x_fm > Xmax  || y_fm < Ymin || y_fm > Ymax ||
  xp_fm < Xmin || yp_fm < Ymin )
   {
     ErrList (13);
     tab_place_marker [poz].flags = 0;
     return;
   }
  skala = skala_fm;
  X = x_fm;
  Y = y_fm;

  d_2d();
  Xp0_Yp0 (xp_fm, yp_fm);
  if ((X_max<=X2d) || (Y_max<=Y2d))
   { setfillstyle_(EMPTY_FILL,0);
     bar(xp,yp,maxX-xk,maxY-yk);
   }
  if(!snap_)krok_s=jednostkiY(1);
  skala_menup();
  redraw();
}
#endif



static void SkalaR(void)
{ regen_ctx=TRUE;
  redraw(); 
}


void SkalaZC(void)
{ d_2d();
  Xp_Yp();

  if(!snap_)krok_s=jednostkiY(1);
  redraw();
#ifdef ALLEGRO5
  flip_screen();
  CUR_ON(X, Y);
#endif
  Save_View();
}

static void skala_menup(void)
{ char sk[48];
  double d;
  
  
  if(skala>999) strcpy(sk,">999");
  else if(skala<.01) strcpy(sk,"<0.01");
  else
   { 

  if (skala>=100)
    { d=floor(skala*1+0.5)*1;
      sprintf(sk,"%4.0g",d);
    }
   else 
    {d=floor(skala*100+0.5)*.01;
     sprintf(sk,"%4.3g",d);
    } 
   }
  menu_par_new((*menup.pola)[1].txt,sk);  /*!!!!!!!!!!!!!!*/
}

void skala_menup_(void)
{
	skala_menup();
}

void SkalaZ(void)
{ static double g[]={1.,1.,1.,1.,1.,1.,  1.,.5,.2,.1};
  int poz;
  poz=mSkala.poz+mSkala.foff;
  if(skala==g[poz]) return;
  skala=g[poz];
  SkalaZC();
  skala_menup();
}

int Skala_In_(void)
{ skala*=1.1;
  SkalaZC();
  skala_menup();
  return 0;
}

int Skala_Out_(void)
{ skala*=0.9;
  SkalaZC();
  skala_menup();
  return 0;
}

int SkalaZ1_(void)
{ skala=1;
  SkalaZC();
  skala_menup();
  return 0;
}

int SkalaZ05_(void)
{ skala=0.5;
  SkalaZC();
  skala_menup();
  return 0;
}

int SkalaZ02_(void)
{ skala=0.2;
  SkalaZC();
  skala_menup();
  return 0;
}

int SkalaZ01_(void)
{ skala=0.1;
  SkalaZC();
  skala_menup();
  return 0;
}

int SkalaZ_Plus(int sclfct)
{ double dsc;
  double skala1;
  dsc=sclfct;
  dsc*=0.1;
  dsc=min(99,dsc);
  skala1=skala*(1.0+dsc);
  if (skala1>9999) skala1=9999;
  if (skala1>skala)
  {
    skala=skala1;
    SkalaZC();
    skala_menup();
  }
  return 0;
}

int SkalaZ_Minus(int sclfct)
{ double dsc;
  double skala1;
  dsc=sclfct;
  dsc*=0.1;
  dsc=min(0.99,dsc);
  skala1=skala*(1.0-dsc);
  if (skala1<0.001) skala1=0.001;
  if (skala1<skala)
  {
    skala=skala1;
    SkalaZC();
    skala_menup();
  }
  return 0;
}



int SkalaW_(void)
{
SkalaW();
return 0;
}

int SkalaE_(void)
{
SkalaE();
return 0;
}


int skala_format_(void)
{
skala_format(0);
return 0;
}

int Choose_Layer_1(void)
{
 Choose_Layer_1_();
 return 0;
}

int Choose_Layer_2(void)
{
 Choose_Layer_2_();
 return 0;
}

int Choose_Layer_3(void)
{
 Choose_Layer_3_();
 return 0;
}

int Choose_Layer_4(void)
{
 Choose_Layer_4_();
 return 0;
}

int Choose_Layer_5(void)
{
 Choose_Layer_5_();
 return 0;
}

int Choose_Layer_6(void)
{
 Choose_Layer_6_();
 return 0;
}

int Choose_Layer_7(void)
{
 Choose_Layer_7_();
 return 0;
}

int Choose_Layer_8(void)
{
 Choose_Layer_8_();
 return 0;
}

int Choose_Layer_9(void)
{
 Choose_Layer_9_();
 return 0;
}

int Choose_Layer_10(void)
{
 Choose_Layer_10_();
 return 0;
}

int Inc_Layer(void)
{
 Inc_Layer_();
 return 0;
}

int Dec_Layer(void)
{
 Dec_Layer_();
 return 0;
}

int SkalaZC_(void)
{
SkalaZC();
return 0;
}

int SkalaR_(void)
{
SkalaR();
return 0;
}

int last_view_(void)
{
last_view();
return 0;
}

static void SkalaZX(void)
{
  static char sk[48] ;
  double d;
  int retval_no = 1;
  double buf_ret[1];

  if (!read_esc(sk, 10, 96)) return ;

  if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
  {
	  return;
  }
  d = buf_ret[0];

  if (d <= 0)
  {
    ErrList(30);
    return;
  }
  skala = d ;
  SkalaZC () ;
  skala_menup () ;
}

/*--------------------------------------------*/
/*------------  SkalaW  ------------------------*/

static void zamien(double *x1, double *x2)
{ double pom;
  pom=*x1;
  *x1=*x2;
  *x2=pom;
}

static void  cur_offo(double x,double y)
{
    flip_screen();
}

static void  cur_offo__(double x,double y)
{ x = x; y = y;
  outokno(&O,0,0);
}

static void  cur_ono(double x,double y)
{ O.x2=x; O.y2=y;
  outokno(&O,0,0);
}

static void redcrok(void)
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  komunikat0(Komunikat_R0=31);
  O.x2=X;
  O.y2=Y;
  Cur_offd(X,Y);
  CUR_oFF=CUR_OFF;  CUR_OFF=cur_offo;
  CUR_oN=CUR_ON;    CUR_ON=cur_ono;
  Cur_ond (X,Y) ;
}

static int oknok(void)
{
  double X0,Y0,dx,dy,dx1,dy1,skala0=skala;
  EVENT *ev;

  redcrok();
  
  while(1)
   { ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0) return 0;
     if (ev->What == evKeyDown &&  ev->Number==ENTER)
      {
	Cur_offd(X,Y);
	if (X!=X0 || Y!=Y0) { O.x2=X0; O.y2=Y0; }
	O.x01=O.x1; O.y01=O.y1;
	O.x02=O.x2; O.y02=O.y2;
	if(O.x01>O.x02) zamien(&(O.x01),&(O.x02));
	if(O.y01>O.y02) zamien(&(O.y01),&(O.y02));
	Cur_ond (X,Y) ;
	if((dx=O.x02-O.x01)>0 && (dy=O.y02-O.y01)>0)
	 { if(X2d/Y2d > dx/dy) { dy1=dy/2;dx1=dy1*X2d/Y2d; skala0*=Y2d/dy; }
	   else { dx1=dx/2; dy1=dx1*Y2d/X2d; skala0*=X2d/dx; }
	   if(skala0<20000) skala=skala0;
	   else skala=20000;
	   X=O.x01+dx1;
	   Y=O.y01+dy1;
	   return 1;
	 }
	return 0;
      }
   }
}


int set_decrease_x(void)
{
	if (GFX_WIN!=1) return 0;
	set_increased(-1,0);
	return 0;
}

int set_increase_x(void)
{
	if (GFX_WIN!=1) return 0;
	set_increased(1,0);
	return 0;
}

int set_decrease_y(void)
{
	if (GFX_WIN!=1) return 0;
	set_increased(0,-1);
	return 0;
}

int set_increase_y(void)
{
	if (GFX_WIN!=1) return 0;
	set_increased(0,1);
	return 0;
}

int set_orig_window(void)
{
   if (GFX_WIN!=1) return 0;
   set_original_window();
   window_was_resized=FALSE;
   return 0;
}

int set_lastest_window(void)
{
   if (GFX_WIN!=1) return 0;
   set_last_window();
   window_was_resized=TRUE;
   return 0;
}

static void redcr(char typ)
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int komunikat_R, komunikat_R0, ss;
  static int image_no ;
  static void (*RED_Add_M)(void);
  if (typ==0)
   {
     REDRAW_ADD_MENU () ;
     RED_Add_M= REDRAW_ADD_MENU; REDRAW_ADD_MENU = nooop ;
     ss=sel.akt;
     sel.akt=ASel;
     komunikat_R=Komunikat_R;
     komunikat_R0 = Komunikat_R0 ;
     komunikat(0);
     image_no = Save_Message0_Image () ; 
     
     komunikat0(30);
     Cur_offd (X,Y);
     CUR_oFF=CUR_OFF;   CUR_OFF=cursel_off;
     CUR_oN=CUR_ON;     CUR_ON=cursel_on;
     SW[0]=SERV[73];    SERV[73]=sel_t;
     SW[1]=SERV[81];    SERV[81]=sel_d;
     Cur_ond (X,Y);
     b_active_w = TRUE ;
   }
  else
   {
     Cur_offd(X,Y);
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     SERV[73]=(void *)SW[0];
     SERV[81]=(void *)SW[1];
     Cur_ond (X,Y) ;
     sel.akt=ss;
     komunikat(komunikat_R);
     Komunikat_R0 = komunikat_R0 ;
     Put_Message0_Image (image_no) ;
     REDRAW_ADD_MENU = RED_Add_M ;
     b_active_w = FALSE ;
   }
}


void SkalaW (void)
/*----------------------*/
{ int n;
  EVENT *ev;
  double X0, Y0;

  if (b_active_w == TRUE)
  {
    return ;
  }
   redcr(0);
   while(1)
   { ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0)
      { redcr(1);
        return;
      }
     if (ev->What == evKeyDown &&  ev->Number==ENTER)
      {
	O.x1=X0;
	O.y1=Y0;
	n=oknok();
	redcr(1);
	if (n)
	 {
	   SkalaZC();
	   skala_menup();
	 }
	return;
      }
   }
}

/*-------SkalaE--------------------------------------*/

//static
void line_place (LINIA *l, double *xmin, double *ymin, double *xmax, double *ymax)
{
  if ( l->x1 < *xmin ) *xmin = l->x1;
  if ( l->x2 < *xmin ) *xmin = l->x2;
  if ( l->x1 > *xmax ) *xmax = l->x1;
  if ( l->x2 > *xmax ) *xmax = l->x2;

  if ( l->y1 < *ymin ) *ymin = l->y1;
  if ( l->y2 < *ymin ) *ymin = l->y2;
  if ( l->y1 > *ymax ) *ymax = l->y1;
  if ( l->y2 > *ymax ) *ymax = l->y2;
}

static void
vector_place (AVECTOR *V, double *xmin, double *ymin, double *xmax, double *ymax)
{
    SOLIDARC sa=sadef;
    LUK l=ldef;
    OKRAG O=Odef;
    TEXT Vtxt=Tdef, Vtxt1=Tdef;
    double ra;
    LINIA Lt=Ldef, Ln1=Ldef;
    double n;
    PLINIA PL;
    double kos, koc;
    double xmax1, ymax1;
#ifndef arrowf
#define arrowf 1.0
#endif

    switch (V->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            line_place((LINIA*)V, xmin, ymin, xmax, ymax);
            break;
        case 4:
        case 18:
            line_place((LINIA*)V, xmin, ymin, xmax, ymax);
            break;
        case 7:
            line_place((LINIA*)V, xmin, ymin, xmax, ymax);
            break;
        case 5:
        case 6:
            sa.warstwa=V->warstwa;
            sa.x=V->x1;
            sa.y=V->y1;
            sa.r=V->r;
            sa.kat1=V->angle1;
            sa.kat2=V->angle2;
            ra=Get_Point_Size () / arrowf;
            sa.width1=ra/4;
            sa.width2=ra/4;
            solidarc_place(&sa, xmin, ymin, xmax, ymax);
            break;
        case 8:
        case 9:
            l.warstwa=V->warstwa;
            l.x=V->x1;
            l.y=V->y1;
            l.r=V->r;
            l.kat1=V->angle1;
            l.kat2=V->angle2;
            luk_place (&l, xmin, ymin, xmax, ymax) ;
            break;
        case 10:
        case 17:
            line_place((LINIA*)V, xmin, ymin, xmax, ymax);

            if (V->x1<V->x2) n=1;
            else n=-1;

            Lt.x1 = V->x1;
            Lt.y1 = V->y1 + n*(V->magnitude1/((V->style==10) ? load_magnitude : flood_magnitude));
            Lt.x2 = V->x2;
            Lt.y2 = V->y2 + n*(V->magnitude2/((V->style==10) ? load_magnitude : flood_magnitude));

            line_place(&Lt, xmin, ymin, xmax, ymax);
            break;
        case 11:
            line_place((LINIA*)V, xmin, ymin, xmax, ymax);

            if (V->y1<V->y2) n=1;
            else n=-1;

            Lt.x1 = V->x1 - n*(V->magnitude1/load_magnitude);
            Lt.y1 = V->y1;
            Lt.x2 = V->x2 - n*(V->magnitude2/load_magnitude);
            Lt.y2 = V->y2;

            line_place(&Lt, xmin, ymin, xmax, ymax);
            break;
        case 12:
            line_place((LINIA*)V, xmin, ymin, xmax, ymax);

            parametry_lini(V, &PL);
            kos=sin(Pi*(PL.kat+90)/180);
            koc=cos(Pi*(PL.kat+90)/180);

            Lt.x1 = V->x1 + (V->magnitude1/load_magnitude)*koc;
            Lt.y1 = V->y1 + (V->magnitude1/load_magnitude)*kos;
            Lt.x2 = V->x2 + (V->magnitude2/load_magnitude)*koc;
            Lt.y2 = V->y2 + (V->magnitude2/load_magnitude)*kos;

            line_place(&Lt, xmin, ymin, xmax, ymax);
            break;
        case 13:
            line_place((LINIA*)V, xmin, ymin, xmax, ymax);

            if (V->x1<V->x2)
            {
                n=1;
                ymax1=max(V->y1, V->y2);
            }
            else
            {
                n=-1;
                ymax1=min(V->y1, V->y2);
            }

            Lt.x1 = V->x1;
            Lt.y1 = ymax1 + n*(V->magnitude1/load_magnitude);
            Lt.x2 = V->x2;
            Lt.y2 = ymax1 + n*(V->magnitude2/load_magnitude);

            line_place(&Lt, xmin, ymin, xmax, ymax);
            break;
        case 14:
            line_place((LINIA*)V, xmin, ymin, xmax, ymax);

            if (V->y1<V->y2)
            {
                n=1;
                xmax1=min(V->x1, V->x2);
            }
            else
            {
                n=-1;
                xmax1=max(V->x1, V->x2);
            }

            Lt.x1 = xmax1 - n*(V->magnitude1/load_magnitude);
            Lt.y1 = V->y1;
            Lt.x2 = xmax1 - n*(V->magnitude2/load_magnitude);
            Lt.y2 = V->y2;

            line_place(&Lt, xmin, ymin, xmax, ymax);
            break;
        case 15:
            line_place((LINIA*)V, xmin, ymin, xmax, ymax);

            parametry_lini((LINIA*)V, &PL);
            kos=sin(Pi*(PL.kat+90)/180);
            koc=cos(Pi*(PL.kat+90)/180);

            Lt.x1 = (V->x1+V->x2)/2 + (V->r/depth_magnitude)*koc;   //thermal_depth_size
            Lt.y1 = (V->y1+V->y2)/2 + (V->r/depth_magnitude)*kos;
            Lt.x2 = (V->x1+V->x2)/2 - (V->r/depth_magnitude)*koc;
            Lt.y2 = (V->y1+V->y2)/2 - (V->r/depth_magnitude)*kos;

            line_place(&Lt, xmin, ymin, xmax, ymax);
            break;
        case 16:
            line_place((LINIA*)V, xmin, ymin, xmax, ymax);

            O.warstwa=V->warstwa;
            O.x=V->x1;
            O.y=V->y1;
            O.r=V->r;
            okrag_place(&O, xmin, ymin, xmax, ymax) ;

            Vtxt.warstwa=V->warstwa;
            Vtxt.x=V->x2;
            Vtxt.y=V->y2;
            Vtxt.kat=0;
            Vtxt.wysokosc=zmwym.wysokosc*0.75;
            Vtxt.justowanie=j_do_lewej;

            set_decimal_format(&Vtxt1.text, V->magnitude1, dim_precision);
            sprintf(&Vtxt.text,"R%s",Vtxt1.text);

            Ln1.x1=V->x2;
            Ln1.y1=V->y2;
            Ln1.y2=V->y1;

            double t_len_mm = Get_TextLen(&Vtxt, Vtxt.text);
            double direction=V->x2-V->x1;
            if (direction > 0) //to the right
            {
                Vtxt.x = V->x2 + Vtxt.wysokosc / 4.0;
                Vtxt.y = V->y2 + Vtxt.wysokosc / 4.0;
                Vtxt.justowanie = j_do_lewej;

                if (t_len_mm == 0.0) Ln1.x2 = V->x2 + 0.01;
                else Ln1.x2 = V->x2 + t_len_mm + Vtxt.wysokosc / 2.5;
            } else  //to the left
            {
                Vtxt.x = V->x2 - Vtxt.wysokosc / 4.0;
                Vtxt.y = V->y2 + Vtxt.wysokosc / 4.0;
                Vtxt.justowanie = j_do_prawej;
                if (t_len_mm == 0.0) Ln1.x2 = V->x2 - 0.01;
                else Ln1.x2 = V->x2 - t_len_mm - Vtxt.wysokosc / 2.0;
            }

            line_place(&Ln1, xmin, ymin, xmax, ymax);
            tekst_place(&Vtxt, xmin, ymin, xmax, ymax);

            break;
    }

}


static void
line_place3D (LINIA3D *l, double *xmin, double *ymin, double *xmax, double *ymax)
{
  if ( l->x1 < *xmin ) *xmin = l->x1;
  if ( l->x2 < *xmin ) *xmin = l->x2;
  if ( l->x1 > *xmax ) *xmax = l->x1;
  if ( l->x2 > *xmax ) *xmax = l->x2;

  if ( l->y1 < *ymin ) *ymin = l->y1;
  if ( l->y2 < *ymin ) *ymin = l->y2;
  if ( l->y1 > *ymax ) *ymax = l->y1;
  if ( l->y2 > *ymax ) *ymax = l->y2;
}

static void
point_place (T_Point *p, double *xmin, double *ymin, double *xmax, double *ymax)
{  double x1, y1, x2, y2, x12, y12;
    double df_psize, r;
    double dxy, dd, dd1;

    double xmm, ymm, xmx, ymx;

    if (p->typ<8) df_psize = (Get_Point_Size() *view_vector_scale) / 2;
    else df_psize = (Get_Point_Size() *view_vector_scale) / 4;

    x1 = (p->x - df_psize) ;
    y1 = (p->y - df_psize) ;
    x2 = (p->x + df_psize) ;
    y2 = (p->y + df_psize) ;

    x12 = (x1 + x2)/2;
    y12 = (y1 + y2)/2;

    if (p->typ <= 8)
    {
        if ( x1 < *xmin ) *xmin = x1;
        if ( x2 > *xmax ) *xmax = x2;

        if ( y1 < *ymin ) *ymin = y1;
        if ( y2 > *ymax ) *ymax = y2;
    }
    else if ((p->typ > 8) && (p->typ < 12))  //those points do not exist, but it's for past compatibility
    {
        if (x1 < *xmin) *xmin = x1;
        if (x2 > *xmax) *xmax = x2;

        if (y1 < *ymin) *ymin = y1;
        if (y2 > *ymax) *ymax = y2;
    }
    else
    {
        switch (p->typ)
        {
            case 12:
            case 13:
            case 14:
            case 15:
            case 20:
            case 21:
            case 22:
            case 23:
                r = 1.5*df_psize;

                dxy= df_psize*2.5;  //2
                dd = df_psize * 1.5;  //*1.0
                dd1= df_psize;

                switch (p->typ)
                {
                    case 12:
                    case 15:
                        xmm=x1-dxy;
                        xmx=x2+dxy;

                        ymm=y12-dd;
                        ymx=y12+dd;
                        break;
                    case 13:
                    case 14:
                        xmm=x12-dd;
                        xmx=x12+dd;

                        ymm=y1-dxy;
                        ymx=y2+dxy;
                        break;
                    case 20:
                    case 23:
                        xmm=x1-dxy;
                        xmx=x2+dxy;

                        ymm=y12-dd-dd1;
                        ymx=y12+dd+dd1;
                        break;
                    case 21:
                    case 22:
                        xmm=x12-dd-dd1;
                        xmx=x12+dd+dd1;

                        ymm=y1-dxy;
                        ymx=y2+dxy;
                        break;
                }
                break;
            case 16:
            case 17:
            case 18:
            case 19:
            case 24:
            case 25:
            case 26:
            case 27:
                r = 2*df_psize;

                dxy= df_psize*3;
                dd = df_psize*2;
                dd1= df_psize;
                switch (p->typ)
                {
                    case 16:
                    case 19:
                        xmm=x1-dxy;
                        xmx=x2+dxy;

                        ymm=y12-dd;
                        ymx=y12+dd;
                        break;
                    case 17:
                    case 18:
                        xmm=x12-dd;
                        xmx=x12+dd;

                        ymm=y1-dxy;
                        ymx=y2+dxy;
                        break;

                    case 24:
                    case 27:
                        xmm=x1-dxy;
                        xmx=x2+dxy;

                        ymm=y12-dd-dd1;
                        ymx=y12+dd+dd1;
                        break;
                    case 25:
                    case 26:
                        xmm=x12-dd-dd1;
                        xmx=x12+dd+dd1;

                        ymm=y1-dxy;
                        ymx=y2+dxy;
                        break;
                }
                break;
            case 28:
                r = 1.5*df_psize;

                dxy= df_psize*2.325;

                xmm=x12-dxy;
                xmx=x12+dxy;

                ymm=y1-dxy;
                ymx=y2+dxy;

                break;
        }
        if ( xmm < *xmin ) *xmin = xmm;
        if ( xmx > *xmax ) *xmax = xmx;

        if ( ymm < *ymin ) *ymin = ymm;
        if ( ymx > *ymax ) *ymax = ymx;
    }
}


static void
tekst_place_row (TEXT *t, char *zn, float t_x, float t_y, double *xmin, double *ymin, double *xmax, double *ymax)
{ double x[4], y[4], tsin, tcos, tdl;
  int i;

  tsin=sin(t->kat);
  tcos=cos(t->kat);

      tdl = Get_TextLen (t, zn) ;

      switch (t->justowanie)
         {
         case 0 : x[0]=t_x;                  y[0]=t_y;
                  x[1]=x[0]+tdl*tcos;         y[1]=y[0]-Ky*tdl*tsin;
                  x[3]=x[0]-t->wysokosc*tsin; y[3]=y[0]-Ky*t->wysokosc*tcos;
                  x[2]=-x[0]+x[1]+x[3];       y[2]=-y[0]+y[1]+y[3];

                  break;
         case 1 : x[1]=t_x;                  y[1]=t_y;  /*do prawej*/
                  x[0]=x[1]-tdl*tcos;         y[0]=y[1]+Ky*tdl*tsin;
                  x[3]=x[0]-t->wysokosc*tsin; y[3]=y[0]-Ky*t->wysokosc*tcos;
                  x[2]=-x[0]+x[1]+x[3];       y[2]=-y[0]+y[1]+y[3];
                  break;
         case 2 : x[0]=t_x-0.5*tdl*tcos;     y[0]=t_y+0.5*Ky*tdl*tsin; /*srodkowo*/
                  x[1]=x[0]+tdl*tcos;         y[1]=y[0]-Ky*tdl*tsin;
                  x[3]=x[0]-t->wysokosc*tsin; y[3]=y[0]-Ky*t->wysokosc*tcos;
                  x[2]=-x[0]+x[1]+x[3];       y[2]=-y[0]+y[1]+y[3];
                  break;
         case 3 : x[0]=t_x-0.5*tdl*tcos+0.5*(t->wysokosc)*tsin;  /*centralnie*/
                  y[0]=t_y+0.5*Ky*tdl*tsin+0.5*(t->wysokosc)*tcos;
                  x[1]=x[0]+tdl*tcos;         y[1]=y[0]-Ky*tdl*tsin;
                  x[3]=x[0]-t->wysokosc*tsin; y[3]=y[0]-Ky*t->wysokosc*tcos;
                  x[2]=-x[0]+x[1]+x[3];       y[2]=-y[0]+y[1]+y[3];
                  break;
         default :x[0]=t_x;                  y[0]=t_y;
                  x[1]=x[0]+tdl*tcos;         y[1]=y[0]-Ky*tdl*tsin;
                  x[3]=x[0]-t->wysokosc*tsin; y[3]=y[0]-Ky*t->wysokosc*tcos;
                  x[2]=-x[0]+x[1]+x[3];       y[2]=-y[0]+y[1]+y[3];
                  break;
         }

      for(i=0; i<4; i++)
       { if ( x[i] < *xmin ) *xmin = x[i];
         if ( x[i] > *xmax ) *xmax = x[i];
         if ( y[i] < *ymin ) *ymin = y[i];
         if ( y[i] > *ymax ) *ymax = y[i];
       }
}


static void tekst_place (TEXT *t, double *xmin, double *ymin, double *xmax, double *ymax)
{
    double box_h;
    int n_i=0;
    double  xmin_r, ymin_r, xmax_r, ymax_r;
    float t_x, t_y;
    double tsin, tcos;


    t_x=t->x;
    t_y=t->y;

    xmin_r=*xmin;
    ymin_r=*ymin;
    xmax_r=*xmax;
    ymax_r=*ymax;

    tsin = sin(t->kat);
    tcos = cos(t->kat);

    if (t->multiline==1)
    {
    //number of \n
    char *t_text=t->text;
    char *ptr_n=strchr(t_text, '\n');
    while (ptr_n!=NULL)
        {
            tekst_place_row (t, t_text, t_x, t_y, &xmin_r, &ymin_r, &xmax_r, &ymax_r);

            n_i++;
            t_text=ptr_n+1;

            t_x += t->wysokosc * ((float)(t->spacing) / 10.0 + 1.0) *tsin; //SPACING
            t_y -= t->wysokosc * ((float)(t->spacing) / 10.0 + 1.0) *tcos; //SPACING

            ptr_n=strchr(t_text, '\n');

            if ( xmin_r < *xmin ) *xmin = xmin_r;
            if ( xmax_r > *xmax ) *xmax = xmax_r;
            if ( ymin_r < *ymin ) *ymin = ymin_r;
            if ( ymax_r > *ymax ) *ymax = ymax_r;
        }
        tekst_place_row (t, t_text, t_x, t_y, &xmin_r, &ymin_r, &xmax_r, &ymax_r);
        if ( xmin_r < *xmin ) *xmin = xmin_r;
        if ( xmax_r > *xmax ) *xmax = xmax_r;
        if ( ymin_r < *ymin ) *ymin = ymin_r;
        if ( ymax_r > *ymax ) *ymax = ymax_r;
    }
    else
    {
        tekst_place_row (t, t->text, t_x, t_y, &xmin_r, &ymin_r, &xmax_r, &ymax_r);
        if ( xmin_r < *xmin ) *xmin = xmin_r;
        if ( xmax_r > *xmax ) *xmax = xmax_r;
        if ( ymin_r < *ymin ) *ymin = ymin_r;
        if ( ymax_r > *ymax ) *ymax = ymax_r;
    }

}

//static
void okrag_place (OKRAG *o, double *xmin, double *ymin, double *xmax, double *ymax)
{
  if ( o->x - o->r < *xmin ) *xmin = o->x - o->r;
  if ( o->y - o->r < *ymin ) *ymin = o->y - o->r;
  if ( o->x + o->r > *xmax ) *xmax = o->x + o->r;
  if ( o->y + o->r > *ymax ) *ymax = o->y + o->r;
}


static void
ellipse_place (ELLIPSE *e, double *xmin, double *ymin, double *xmax, double *ymax)
{   int numpoints;
    double xy[256];
    int verno;

    numpoints= Get_Ellipse_Points(e->x, e->y, e->rx, e->ry, e->angle, xy);

    for (int i=0; i<(numpoints-2); i+=2) {
        if (xy[i] < *xmin) *xmin = xy[i];
        if (xy[i+1] < *ymin) *ymin = xy[i+1];
        if (xy[i] > *xmax) *xmax = xy[i];
        if (xy[i+1] > *ymax) *ymax = xy[i+1];
    }
}

static void
ellipticalarc_place (ELLIPTICALARC *ea, double *xmin, double *ymin, double *xmax, double *ymax)
{   int numpoints;
    double xy[256];

    numpoints= Get_EllipticalArc_Points(ea->x, ea->y, ea->rx, ea->ry, ea->angle, ea->kat1, ea->kat2, xy);

    for (int i=0; i<(numpoints-2); i+=2) {
        if (xy[i] < *xmin) *xmin = xy[i];
        if (xy[i+1] < *ymin) *ymin = xy[i+1];
        if (xy[i] > *xmax) *xmax = xy[i];
        if (xy[i+1] > *ymax) *ymax = xy[i+1];
    }
}


double Normalized(double kat)
{
	if (kat < 0) return (kat + 360.0);
	else return kat;
}

void luk_place (LUK *l, double *xmin, double *ymin, double *xmax, double *ymax)
{
  double x1,y1,x2,y2,kat1,kat2;
  double x_min, x_max, y_min, y_max;

  kat1=Normalized(l->kat1/Pi2*360);
  kat2=Normalized(l->kat2/Pi2*360);
  y1=l->y+l->r*sin(l->kat1);
  x1=l->x+l->r*cos(l->kat1);
  x_min = x1;
  x_max = x1;
  y_min = y1;
  y_max = y1;

  y2=l->y+l->r*sin(l->kat2);
  x2=l->x+l->r*cos(l->kat2);
  if ( x2 < x_min ) x_min = x2;
  if ( x2 > x_max ) x_max = x2;
  if ( y2 < y_min ) y_min = y2;
  if ( y2 > y_max ) y_max = y2;


  if (kat2<kat1)  //luk przechodzi przez 0
   {
    kat2+=360;
    if ((kat1<90) && (kat2>90)) y_max = l->y + l->r;
    if ((kat1<180) && (kat2>180)) x_min = l->x - l->r;
    if ((kat1<270) && (kat2>270)) y_min = l->y - l->r;

    if ((kat1<360) && (kat2>360)) x_max = l->x + l->r;
    if ((kat1<360) && (kat2>450)) y_max = l->y + l->r;
    if ((kat1<360) && (kat2>540)) x_min = l->x - l->r;
    if ((kat1<360) && (kat2>630)) y_min = l->y - l->r;
   }
    else  //luk nie przechodzi przez 0
     {
       if ((kat1<90) && (kat2>90)) y_max = l->y + l->r;
       if ((kat1<180) && (kat2>180)) x_min = l->x - l->r;
       if ((kat1<270) && (kat2>270)) y_min = l->y - l->r;
     }

  if ( x_min < *xmin ) *xmin = x_min;
  if ( x_max > *xmax ) *xmax = x_max;
  if ( y_min < *ymin ) *ymin = y_min;
  if ( y_max > *ymax ) *ymax = y_max;
 return;
}

static void
wielokat_place (WIELOKAT *w, double *xmin, double *ymin, double *xmax, double *ymax)
{ int i;
  for(i=0; i<(int)w->lp && i<MaxNumPoints; i++)
  if ((i % 2) == 0)
   { if ( w->xy[i] < *xmin ) *xmin = w->xy[i];
     if ( w->xy[i] > *xmax ) *xmax = w->xy[i];
   }                                                 else
   { if ( w->xy[i] < *ymin ) *ymin = w->xy[i];
     if ( w->xy[i] > *ymax ) *ymax = w->xy[i];
   }
}

static void
solidarc_place (SOLIDARC *sa, double *xmin, double *ymin, double *xmax, double *ymax)
{
    LINIA L_left, L_right;
    LUK l_inner1, l_inner2, l_outer1, l_outer2;
    int ln;

    ln=solidarc_elements(sa, &L_left, &L_right, &l_inner1, &l_inner2, &l_outer1, &l_outer2);

    line_place (&L_left, xmin, ymin, xmax, ymax);
    line_place (&L_right, xmin, ymin, xmax, ymax);
    luk_place (&l_inner1, xmin, ymin, xmax, ymax);
    luk_place (&l_outer1, xmin, ymin, xmax, ymax);

    if (ln>1)
    {
        luk_place (&l_inner2, xmin, ymin, xmax, ymax);
        luk_place (&l_outer2, xmin, ymin, xmax, ymax);
    }
}

static void
spline_place(SPLINE *s, double *xmin, double *ymin, double *xmax, double *ymax)
{
	int i, j;
	float out_x[MaxNpts], out_y[MaxNpts];
    long TotL;
    T_PixelTVal PolyPoints[MaxNumPoints];
    int npts;
    double curviness = 0.75;
    SPLINE tmp_spline = Splinedef;
    int node_count;
    double bnodes_2_tangent_org, bnodes_2_tangent;

    if (s->lp<6) {  //it would be the line
        for (i = 0; i < (s->lp/2); i++)
        {
            if (s->xy[i*2] < *xmin) *xmin = s->xy[i*2];
            if (s->xy[i*2] > *xmax) *xmax = s->xy[i*2];

            if (s->xy[i*2+1] < *ymin) *ymin = s->xy[i*2+1];
            if (s->xy[i*2+1] > *ymax) *ymax = s->xy[i*2+1];
        }
        return;
    }

    if (s->npts < 5)
    {
        TotL = 0;

        for (i = 0; i < (int)(s->lp) && i < (MaxNumPoints); i += 2)
        {
            PolyPoints[i] = pikseleX0(s->xy[i]);
            PolyPoints[i + 1] = pikseleY0(s->xy[i + 1]);
        }

        for (i = 0; i < (int)(s->lp - 2) && i < (MaxNumPoints); i += 2)
        {
            TotL += (long)sqrt((float)((PolyPoints[i + 2] - PolyPoints[i]) * (PolyPoints[i + 2] - PolyPoints[i])) + (float)((PolyPoints[i + 3] - PolyPoints[i + 1]) * (PolyPoints[i + 3] - PolyPoints[i + 1])));
        }

#define NPA 30

        npts = 8 + TotL / 30;
        if (npts > NPA)
        {
            npts = NPA;
        }

        calc_bspline(s->lp, s->npts, s->xy, npts, out_x, out_y);

        for (i = 0; i < npts; i++)
        {
            if (out_x[i] < *xmin) *xmin = out_x[i];
            if (out_x[i] > *xmax) *xmax = out_x[i];

            if (out_y[i] < *ymin) *ymin = out_y[i];
            if (out_y[i] > *ymax) *ymax = out_y[i];
        }
    }
    else //multipoint
    {
        curviness = s->xy[s->lp];  //always on the end is curviness of uniform spline
        tmp_spline.atrybut=s->atrybut;
        tmp_spline.npts = 4;
        tmp_spline.lp = 8;
        tmp_spline.kolor = s->kolor;
        tmp_spline.typ = s->typ;
        tmp_spline.widoczny = 0;

        node_count = 1;

        set_bnodes(s, s->xy, &node_count);

        calc_tangents(s->closed, &node_count, &bnodes_2_tangent);

        if (s->closed == 0)
        {
            bnodes_2_tangent_org = bnodes[2].tangent;
            bnodes[2].tangent = bnodes_2_tangent;
            get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);

            npts=30; //high precision is not necessary
            //rysuj_spline_(&tmp_spline, &tmp_spline.xy, mode, kolor, count_mvcurb, TRUE, moving);
            calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

            for (i = 0; i < npts; i++)
            {
                if (out_x[i] < *xmin) *xmin = out_x[i];
                if (out_x[i] > *xmax) *xmax = out_x[i];

                if (out_y[i] < *ymin) *ymin = out_y[i];
                if (out_y[i] > *ymax) *ymax = out_y[i];
            }
            bnodes[2].tangent = bnodes_2_tangent_org;
        }


        for (j = 2; j < node_count - 2; j++)  //1 - extra_node
        {
            get_control_points(bnodes[j], bnodes[j+1], tmp_spline.xy);
            npts=30; //high precision is not necessary
            calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

            for (i = 0; i < npts; i++)
            {
                if (out_x[i] < *xmin) *xmin = out_x[i];
                if (out_x[i] > *xmax) *xmax = out_x[i];

                if (out_y[i] < *ymin) *ymin = out_y[i];
                if (out_y[i] > *ymax) *ymax = out_y[i];
            }
        }

        if ((node_count > 3)  &&  (s->closed == 0))
        {
            get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
            npts=30; //high precision is not necessary
            calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, npts, out_x, out_y);

            for (i = 0; i < npts; i++)
            {
                if (out_x[i] < *xmin) *xmin = out_x[i];
                if (out_x[i] > *xmax) *xmax = out_x[i];

                if (out_y[i] < *ymin) *ymin = out_y[i];
                if (out_y[i] > *ymax) *ymax = out_y[i];
            }
        }

    }
}

static void
pcx_place (B_PCX *w, double *xmin, double *ymin, double *xmax, double *ymax)
{
  WIELOKAT S4=Stdef;

  Solid04((char *)w, (char *) & S4, kolory.blok);
  wielokat_place (&S4, xmin, ymin, xmax, ymax) ;
}

void Get_Limits_PCX (long_long off, long_long offk, int atrybut, double *xmin, double *xmax, double *ymin, double *ymax)
/*----------------------------------------------------------------------------------------------------------*/
{
  NAGLOWEK *nag ;
  long_long ad ;

  *xmin = MAXDOUBLE ;
  *xmax = -MAXDOUBLE ;
  *ymin = MAXDOUBLE ;
  *ymax = -MAXDOUBLE ;
  ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  while (ad != -1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    if (atrybut == ANieOkreslony || atrybut == nag->atrybut)
    {
      switch (nag->obiekt)
      {
	   case Opcx :
	    pcx_place ((B_PCX*)nag, xmin, ymin, xmax, ymax) ;
	   break ;
       default :
       break ;
      } /*switch*/
    }
    ObiektTok (&off, offk, &ad, ONieOkreslony);
  }
  return;
}

void over_limits(double *xmin, double *xmax, double *ymin, double *ymax, int type) {
    int a;
    if (*xmin < -9999999)
        a = 0;
    if (*xmax > 9999999)
        a = 0;
    if (*ymin < -9999999)
        a = 0;
    if (*ymax > 9999999)
        a = 0;
}

void Get_Limits (long_long off, long_long offk, int atrybut, double *xmin, double *xmax, double *ymin, double *ymax)
/*------------------------------------------------------------------------------------------------------*/
{
  NAGLOWEK *nag ;
  long_long ad ;
  int a;
 
  *xmin = MAXDOUBLE ;
  *xmax = -MAXDOUBLE ;
  *ymin = MAXDOUBLE ;
  *ymax = -MAXDOUBLE ;
  ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  while (ad != -1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    if (atrybut == ANieOkreslony || atrybut == nag->atrybut)
    {
      switch (nag->obiekt)
      {
	    case Olinia :
	    line_place ((LINIA*)nag, xmin, ymin, xmax, ymax) ;
         //// over_limits(xmin, xmax, ymin, ymax,0);
	    break ;
	    case Otekst :
	    tekst_place ((TEXT*)nag, xmin, ymin, xmax, ymax) ;
          ////    over_limits(xmin, xmax, ymin, ymax,2);
	    break ;
	    case Okolo :
        case Ookrag :
	    okrag_place ((OKRAG*)nag, xmin, ymin, xmax, ymax) ;
          ////    over_limits(xmin, xmax, ymin, ymax,3);
	    break ;
        case Oellipse :
        case Ofilledellipse :
        ellipse_place ((ELLIPSE*)nag, xmin, ymin, xmax, ymax) ;
              ////    over_limits(xmin, xmax, ymin, ymax,3);
        break ;
        case Oluk :
	    luk_place ((LUK*)nag, xmin, ymin, xmax, ymax) ;
          ////    over_limits(xmin, xmax, ymin, ymax,4);
	    break ;
        case Oellipticalarc :
        ellipticalarc_place ((ELLIPTICALARC *)nag, xmin, ymin, xmax, ymax) ;
              ////    over_limits(xmin, xmax, ymin, ymax,4);
        break ;
        case Owwielokat :
	    wielokat_place ((WIELOKAT*)nag, xmin, ymin, xmax, ymax) ;
           ////   over_limits(xmin, xmax, ymin, ymax,5);
	    break ;
        case Osolidarc :
        solidarc_place ((SOLIDARC*)nag, xmin, ymin, xmax, ymax) ;
           ////   over_limits(xmin, xmax, ymin, ymax,5);
        break ;
        case Ospline:
	    spline_place((SPLINE*)nag, xmin, ymin, xmax, ymax);
           ////   over_limits(xmin, xmax, ymin, ymax,6);
	    break;
        case Opoint :
	    point_place ((T_Point*)nag, xmin, ymin, xmax, ymax) ;
           ////   over_limits(xmin, xmax, ymin, ymax,7);
	    break ;
        case Ovector :
        vector_place ((AVECTOR*)nag, xmin, ymin, xmax, ymax) ;
          ////   over_limits(xmin, xmax, ymin, ymax,7);
        break ;
       default :
	    break ;
      } /*switch*/
    }

    ObiektTok (&off, offk, &ad, ONieOkreslony);
  }
  return;
}


void Get_Limits_NO_255 (long_long off, long_long offk, int atrybut,
	double *xmin, double *xmax, double *ymin, double *ymax)
/*-------------------------------------------------------------*/
{
  NAGLOWEK *nag ;
  NAGLOWEK_EXT *nag_ext ;
  long_long ad ;


  *xmin = MAXDOUBLE ;
  *xmax = -MAXDOUBLE ;
  *ymin = MAXDOUBLE ;
  *ymax = -MAXDOUBLE ;
  ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  while (ad != -1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
	nag_ext = (NAGLOWEK_EXT*)(dane + ad) ;
    if ((atrybut == ANieOkreslony || atrybut == nag->atrybut) && (nag_ext->warstwa!=255))
    {
      switch (nag->obiekt)
      {
	case Olinia :
	  line_place ((LINIA*)nag, xmin, ymin, xmax, ymax) ;
	  break ;
	case Otekst :
	  tekst_place ((TEXT*)nag, xmin, ymin, xmax, ymax) ;
	  break ;
	case Okolo :
       case Ookrag :
	  okrag_place ((OKRAG*)nag, xmin, ymin, xmax, ymax) ;
	 break ;
       case Oluk :
	   luk_place ((LUK*)nag, xmin, ymin, xmax, ymax) ;
	 break ;
       case Owwielokat :
	  wielokat_place ((WIELOKAT*)nag, xmin, ymin, xmax, ymax) ;
	 break ;
       case Opoint :
	  point_place ((T_Point*)nag, xmin, ymin, xmax, ymax) ;
	 break ;	 
       default :
	 break ;
      } /*switch*/
    }
    ObiektTok (&off, offk, &ad, ONieOkreslony);
  }
  return;
}

void Get_Limits_no_DIM (long_long off, long_long offk, int atrybut,
	double *xmin, double *xmax, double *ymin, double *ymax)
/*-------------------------------------------------------------*/
{
  NAGLOWEK *nag ;
  long_long ad ;

  *xmin = MAXDOUBLE ;
  *xmax = -MAXDOUBLE ;
  *ymin = MAXDOUBLE ;
  *ymax = -MAXDOUBLE ;
  ObiektTok (&off, offk, &ad, ONoInDimBlock) ;
  while (ad != -1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    if (atrybut == ANieOkreslony || atrybut == nag->atrybut)
    {
      switch (nag->obiekt)
      {
	case Olinia :
	  line_place ((LINIA*)nag, xmin, ymin, xmax, ymax) ;
	  break ;
	case Otekst :
	  tekst_place ((TEXT*)nag, xmin, ymin, xmax, ymax) ;
	  break ;
	case Okolo :
   case Ookrag :
	  okrag_place ((OKRAG*)nag, xmin, ymin, xmax, ymax) ;
	 break ;
   case Oluk :
	   luk_place ((LUK*)nag, xmin, ymin, xmax, ymax) ;
	 break ;
   case Owwielokat :
	  wielokat_place ((WIELOKAT*)nag, xmin, ymin, xmax, ymax) ;
	 break ;
       case Opoint :
	  point_place ((T_Point*)nag, xmin, ymin, xmax, ymax) ;
	 break ;	 
       default :
	 break ;
      } /*switch*/
    }
    ObiektTok (&off, offk, &ad, ONoInDimBlock);
  }
  return;
}

void Get_Limits_no_DIM_no_Hatch25 (long_long off, long_long offk, int atrybut,
	double *xmin, double *xmax, double *ymin, double *ymax)
/*-------------------------------------------------------------*/
{
  NAGLOWEK *nag ;
  long_long ad ;

  *xmin = MAXDOUBLE ;
  *xmax = -MAXDOUBLE ;
  *ymin = MAXDOUBLE ;
  *ymax = -MAXDOUBLE ;
  ObiektTok (&off, offk, &ad, ONoInDimBlock) ;
  while (ad != -1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    if ((atrybut == ANieOkreslony || atrybut == nag->atrybut) && (nag->obiektt2 != O2BlockHatch25))
    {
      switch (nag->obiekt)
      {
	case Olinia :
	  line_place ((LINIA*)nag, xmin, ymin, xmax, ymax) ;
	  break ;
	case Otekst :
	  tekst_place ((TEXT*)nag, xmin, ymin, xmax, ymax) ;
	  break ;
	case Okolo :
   case Ookrag :
	  okrag_place ((OKRAG*)nag, xmin, ymin, xmax, ymax) ;
	 break ;
   case Oluk :
	   luk_place ((LUK*)nag, xmin, ymin, xmax, ymax) ;
	 break ;
   case Owwielokat :
	  wielokat_place ((WIELOKAT*)nag, xmin, ymin, xmax, ymax) ;
	 break ;
       case Opoint :
	  point_place ((T_Point*)nag, xmin, ymin, xmax, ymax) ;
	 break ;	 
       default :
	 break ;
      } /*switch*/
    }
    ObiektTok (&off, offk, &ad, ONoInDimBlock);
  }
  return;
}

void Get_Limits_no_DIM_no_Hatch25_from_layer (long_long off, long_long offk, int atrybut,
	double *xmin, double *xmax, double *ymin, double *ymax, int warstwa)
/*-------------------------------------------------------------------*/
{
  NAGLOWEK *nag ;
  long_long ad ;
  LINIA *L;

  *xmin = MAXDOUBLE ;
  *xmax = -MAXDOUBLE ;
  *ymin = MAXDOUBLE ;
  *ymax = -MAXDOUBLE ;
  ObiektTok_Layer (&off, offk, &ad, ONoInDimBlock, warstwa) ;
  while (ad != -1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    if ((atrybut == ANieOkreslony || atrybut == nag->atrybut) && (nag->obiektt2 != O2BlockHatch25))
    {
     L=(LINIA*)(dane + ad);
     if (L->warstwa==warstwa)
     {
      switch (nag->obiekt)
      {
	case Olinia :
	  line_place ((LINIA*)nag, xmin, ymin, xmax, ymax) ;
	  break ;
	case Otekst :
	  tekst_place ((TEXT*)nag, xmin, ymin, xmax, ymax) ;
	  break ;
	case Okolo :
   case Ookrag :
	  okrag_place ((OKRAG*)nag, xmin, ymin, xmax, ymax) ;
	 break ;
   case Oluk :
	   luk_place ((LUK*)nag, xmin, ymin, xmax, ymax) ;
	 break ;
   case Owwielokat :
	  wielokat_place ((WIELOKAT*)nag, xmin, ymin, xmax, ymax) ;
	 break ;
       case Opoint :
	  point_place ((T_Point*)nag, xmin, ymin, xmax, ymax) ;
	 break ;	 
       default :
	 break ;
      } /*switch*/
     }
    }
    ObiektTok_Layer (&off, offk, &ad, ONoInDimBlock, warstwa);
  }
  return;
}


void SkalaE(void)
/*--------------*/
{
  double xmin, xmax, ymin, ymax ;
  long off = 0, offk = dane_size ;

  Get_Limits (off, offk, ANieOkreslony, &xmin, &xmax, &ymin, &ymax) ;

  if ((xmin >= Xmax) ||
      (ymin >= Ymax) ||
      (TRUE == Check_if_Equal (xmax, 0)) ||
      (TRUE == Check_if_Equal (ymax, 0)))
      {
       skala_format(0);
      }
     else
       skala_w (xmin, xmax, ymin, ymax, 0) ;
  return ;
}

void SkalaE_S(double skala_e, int dark)
/*-----------------------------------*/
{
  double xmin, xmax, ymin, ymax ;
  long off = 0, offk = dane_size ;
  double del_x, del_y;

  Get_Limits (off, offk, ANieOkreslony, &xmin, &xmax, &ymin, &ymax) ;
  //korekta okna
  del_x=fabs(xmax-xmin)*(1-skala_e)/2;
  del_y=fabs(ymax-ymin)*(1-skala_e)/2;
  xmin-=del_x;
  xmax+=del_x;
  ymin-=del_y;
  ymax+=del_y;

  if ((xmin >= Xmax) ||
      (ymin >= Ymax) ||
      (TRUE == Check_if_Equal (xmax, 0)) ||
      (TRUE == Check_if_Equal (ymax, 0)))
      {
       skala_format(dark);
      }
     else
       skala_w (xmin, xmax, ymin, ymax, dark) ;
  return ;
}



static BOOL skala_w(double xmin, double xmax, double ymin, double ymax, int dark)
/*------------------------------------------------------------------------------*/
{
	double dx, dy;

	if (xmin >= xmax && ymin >= ymax)
	{
		return FALSE;
	}
	if (xmax > Xmax) xmax = Xmax;
	if (ymax > Ymax) ymax = Ymax;
	if (xmin < Xmin) xmin = Xmin;
	if (ymin < Ymin) ymin = Ymin;
	skala = 1;
	d_2d();
	dx = jednostkiX(maxXr);
	dy = jednostkiY(maxYr);
	skala = max((xmax - xmin) / dx, (ymax - ymin) / dy);
	skala = 1 / skala - o_male;
	if (skala <= 0)
	{
		return FALSE;
	}
	d_2d();
	X = xmin + (xmax - xmin) / 2;
	Y = ymin + (ymax - ymin) / 2;
	Xp_Yp();
	if (!snap_)krok_s = jednostkiY(1);
	if (dark < 0)
    {
       redraw_preview(dark);
    }
	else
	{
		if (dark == 0) redraw();
		skala_menup();
		Save_View();
	}
  return TRUE ;
}

static BOOL skala_w_p(double xmin, double xmax, double ymin, double ymax, int dark)
/*------------------------------------------------------------------------------*/
{
    double dx, dy;

    if (xmin >= xmax && ymin >= ymax)
    {
        return FALSE;
    }
    if (xmax > Xmax) xmax = Xmax;
    if (ymax > Ymax) ymax = Ymax;
    if (xmin < Xmin) xmin = Xmin;
    if (ymin < Ymin) ymin = Ymin;
    skala = 1;
    d_2d();
    dx = jednostkiX(maxXr);
    dy = jednostkiY(maxYr);
    skala = max((xmax - xmin) / dx, (ymax - ymin) / dy);
    skala = 1 / skala - o_male;
    if (skala <= 0)
    {
        return FALSE;
    }
    d_2d_p();
    X = xmin + (xmax - xmin) / 2;
    Y = ymin + (ymax - ymin) / 2;

    Xp_Yp_p(xmin, ymin);
    if (!snap_)krok_s = jednostkiY(1);
    if (dark < 0)
    {
        redraw_preview(dark);
    }
    else
    {
        if (dark == 0) redraw();
        skala_menup();
        Save_View();
    }
    return TRUE ;
}

void skala_window(double xmin, double xmax, double ymin, double ymax, int dark)
{
    skala_w (xmin, xmax, ymin, ymax, dark) ;
}

void skala_window_p(double xmin, double xmax, double ymin, double ymax, int dark)
{
    skala_w_p (xmin, xmax, ymin, ymax, dark) ;
}

void skala_format (int dark)
/*----------------------------*/
{
  Xmax=X_max+Xmin;
  Ymax=Y_max+Ymin;
  skala_w (Xmin, Xmax, Ymin, Ymax, dark) ;
}

void Skala_format (void)
{
 skala_format(0);
}

/*---------------------------------------------*/

static void WSkok(void)
/*---------------------*/
{
  char st [MaxTextLen] ;
  int retval_no = 2 ;
  double xd,yd,xp,yp;
  double buf_ret [2] ;
  double angle_l,sina,cosa;

  st[0]='\0';
  if (!read_esc(st,60,22))  return;
  if (0 == calculator (st, &retval_no, buf_ret)  || retval_no < 2)
  {
    return ;
  }
  if (options1.uklad_geodezyjny==0)
   {
    xd = buf_ret [0];
    yd = buf_ret [1];
   }
    else
     {
      xd = buf_ret [1];
      yd = buf_ret [0];
     }

  xp = jednostkiOb(xd);
  yp = jednostkiOb(yd);

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
  mvcurp(xd,yd);
}

static void LSkok (void)
/*---------------------*/
{
  char st [MaxTextLen] ;
  int retval_no = 2 ;
  double df_l, df_angle, df_dx, df_dy ;
  double buf_ret [2] ;
  double angle_l, df_angle1;

  st[0]='\0';
  if (!read_esc(st,60,30))  return;
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
  mvcurp(df_dx, df_dy) ;
}



static void BSkok(void)
/*---------------------*/
{
  char st [MaxTextLen] ;
  int retval_no = 2 ;
  double xd,yd,xd1,yd1,xp,yp;
  double buf_ret [2] ;
  double sina, cosa, angle_l;

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

  mvcurp( xd - X, yd - Y ) ;

}


#ifdef aaa
static void WSkok(void)
{ char *endptr1,*endptr2,st[21],*stptr;
  double xd,yd,xd1,yd1;
  double angle_l, sina, cosa;
  st[0]='\0';
  if (!read_esc(st,20,22))  return;
  if((stptr=strpbrk(st,".-+0123456789"))==NULL)
   { ErrList(16);return;}
  xd=strtod(stptr,&endptr1);
  if((stptr=strpbrk(endptr1,".-+0123456789"))==NULL)
   { ErrList(16);return;}
  yd=strtod(stptr,&endptr2);

  xp = jednostkiOb(xd);
  yp = jednostkiOb(yd);

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
  mvcurp(xd,yd);
}



static void BSkok(void)
{ char *endptr1,*endptr2,st[21],*stptr;
  double xd,yd,xd1,yd1,xp,yp;
  st[0]='\0';
  if (!read_esc(st,20,23)) return;
  if((stptr=strpbrk(st,".-+0123456789"))==NULL)
   { ErrList(16);return;}
  xd=strtod(stptr,&endptr1);
  if((stptr=strpbrk(endptr1,".-+0123456789"))==NULL)
   { ErrList(16);return;}
  yd=strtod(stptr,&endptr2);

  xp = jednostkiOb (xd);
  yp = jednostkiOb (yd);

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

 /* mvcurp(jednostkiOb(xd) - ( X - get_localx() ), jednostkiOb(yd) - ( Y - get_localy() )) ; */
  mvcurp( xd - X, yd - Y ) ;

}
#endif


static void TrybS(void)
{

  int poz;
  poz=mTryb.poz+mTryb.foff;

  sel.nr=poz;
   
  if(poz==9)
   { ////if(sel.akt && !sel.akw && !sel.cur && sel.gor) out_sel(X,Y);
     sel.gor=0;
   }
  else
   { ////if(sel.akt && !sel.akw && !sel.cur && !sel.gor) out_sel(X,Y);
     sel.gor=1;
   }
  menu_par_new((*mPunkt.pola)[0].txt,ggsel[poz]);
  layer_info () ;
}

void Set_TrybS(int poz)
{
	sel.nr = poz;

	if (poz <= mTryb.max)
	{
		mTryb.poz = poz;
		mTryb.foff = 0;
	}
	else
	{
		mTryb.foff = mTryb.max-poz;
		mTryb.poz = poz - mTryb.foff;
	}

	if (poz == 9)
	{
		////if (sel.akt && !sel.akw && !sel.cur && sel.gor) out_sel(X, Y);
		sel.gor = 0;
	}
	else
	{
		////if (sel.akt && !sel.akw && !sel.cur && !sel.gor) out_sel(X, Y);
		sel.gor = 1;
	}
	menu_par_new((*mPunkt.pola)[0].txt, ggsel[poz]);
	layer_info();
}


static void point_view_on (void)
/*---------------------------*/
{
  if (FALSE == Get_Point_View ())
  {
    Set_Point_View (TRUE) ;
    Redraw_Point (TRUE) ;
	Redraw_Line_225(TRUE);
	Redraw_Arc_225(TRUE);
	Redraw_Circle_225(TRUE);
    Redraw_Text (TRUE);

    Cur_offd (X,Y) ;
    Cur_ond (X,Y) ;

	menu_par_new ((*mParametrym.pola)[7].txt, YES) ;

  }
}

void pcx_outlines_view_on(void)
{
	if (bitmap_exist || bitmap_on_front_exist)
	{
		Redraw_Bitmap_Outline(1);
		Set_Bitmap_Outline(TRUE);
    }
}

void pcx_outlines_view_off(void)
{
	if (bitmap_exist || bitmap_on_front_exist)
	{
		Redraw_Bitmap_Outline(0);
	}
	Set_Bitmap_Outline(FALSE);
}

void control_points_view_on(void)
/*------------------------------------*/
{
	if (FALSE == Get_Control_Points_View())
	{
		Cur_offd(X, Y);
		Redraw_Spline(FALSE);
		Set_Control_Points_View(TRUE);
		Redraw_Spline(TRUE);
		Cur_offd(X, Y);
		Cur_ond(X, Y);
	}
}

void control_points_view_on__(void)
/*------------------------------------*/
{
	if (FALSE == Get_Control_Points_View())
	{
		Cur_offd(X, Y);
		Redraw_Spline(FALSE);
		Set_Control_Points_View(TRUE);
		Redraw_Spline(TRUE);
		Cur_ond(X, Y);
	}
}

static void point_view_off (void)
/*-----------------------------*/
{
  if (TRUE == Get_Point_View ())
  {
    Redraw_Point (FALSE) ;
	Redraw_Line_225(FALSE);
	Redraw_Arc_225(FALSE);
	Redraw_Circle_225(FALSE);
    Redraw_Text (FALSE) ;
    Cur_offd (X,Y) ;
    Cur_ond (X,Y) ;
    Set_Point_View (FALSE) ;

    menu_par_new ((*mParametrym.pola)[7].txt, NO) ;

  }
}

void control_points_view_off(void)
/*-------------------------------------*/
{
	if (TRUE == Get_Control_Points_View())
	{
		Cur_offd(X, Y);
		Redraw_Spline(FALSE);
		Set_Control_Points_View(FALSE);
		Redraw_Spline(TRUE);
		Cur_ond(X, Y);
	}
}


static void point_size (void)
/*--------------------------*/
{
  char sk [MaxTextLen] = "", *str ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d ;

  if (EOF == sprintf(sk, "%-6.2f", jednostki_d_to_plt_d (Get_Point_Size ())))
  {
    strcpy (sk, "");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
   if (!read_esc(sk,60,25))  return;

  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  if (d <= 0)
  {
    ErrList (30) ;
    return ;
  }
  if (d > 1000) d = 1000 ;
  Redraw_Point (FALSE) ;
  Redraw_Vector (FALSE) ;
  Set_Point_Size (plt_d_to_jednostki_d (d)) ;
  Redraw_Point (TRUE) ;
  Redraw_Vector (TRUE) ;
  sprintf (sk, "%#6.2lf", d) ;
  if (strlen (sk) > 6) sk [6] = '\0' ;
  strcat (sk, "mm") ;

  CUR_OFF(X, Y);
  CUR_ON(X, Y);
  menu_par_new ((*mParametrym.pola)[8].txt, sk) ;

}




void menupini(TMENU *menu,char *st,unsigned short cod, int iconno)
/*--------------------------------------------------------------*/
{
  char *pole;

  (*menup.pola)[MPMAX].menu=menu;

  pole=(*menup.pola)[MPMAX].txt;
  strncpy (pole, st, POLE_TXT_MAX - 1) ;

  (*menup.pola)[MPMAX].iconno=iconno;  
  (*menup.pola)[MPMAX].wcod=cod;  //win32todo

}

int get_MPMAX(void)
{
    return MPMAX;
}

void getmenupini (TMENU **menu,char *st,unsigned short *cod, int *iconno)
/*---------------------------------------------------------------------*/
{
  char *pole ;

  *menu=(*menup.pola)[MPMAX].menu;
  pole=(*menup.pola)[MPMAX].txt;
  strncpy (st, pole, POLE_TXT_MAX - 1) ;
  st [POLE_TXT_MAX - 1] = '\0' ;
  *cod=(*menup.pola)[MPMAX].wcod;
  *iconno=(*menup.pola)[MPMAX].iconno;
}

static void uaktualnij_pola_mp(void)
/*--------------------------------*/
{
  int i, i_menu_color, i_line_type ;
  char st[8], sk[48], *sk1;
  char sz_temp [MaxTextLen + 1] ;
  static char stY [8]=YES, stN [8]=NO;
  double d ;

  i_line_type = (Layers[Current_Layer].line_type & 31);
  Get_Menu_Param (i_line_type, mTypyLinii.maxw, mTypyLinii.max,
		 &mTypyLinii.foff, &mTypyLinii.poz) ;
  strcpy(sk,(*mTypyLinii.pola)[mTypyLinii.poz + mTypyLinii.foff].txt);

  menu_par_new((*mParametrym.pola)[6].txt,sk);

  mGruboscLinii.poz = (Layers[Current_Layer].line_type & 224 ) / 32;
  strcpy(sk,(*mGruboscLinii.pola)[mGruboscLinii.poz].txt);

  menu_par_new((*mParametrym.pola)[5].txt,sk);  
  
  i_menu_color = Color_No_to_Menu_No (Layers[Current_Layer].color) ;
  Test_Menu (&mKolorAC) ;
  if (i_menu_color>=16) 
  {
  i_menu_color=16;
  }
  Get_Menu_Param (i_menu_color, mKolorAC.maxw, mKolorAC.max,
		 &mKolorAC.foff, &mKolorAC.poz) ;
  if (i_menu_color>=16)
   {
     strcpy((char *)& sk, "");
     sprintf((char *)sk, _OTHER_NO_,Layers[Current_Layer].color);
     sk1=strpbrk(sk," ");
   }  
   else
   {
	   return_menu_par0((*mKolorAC.pola)[i_menu_color].txt,(char *) & sk);
	   sk1=strpbrk(sk," ");
   }
   menu_par_new((*mParametrym.pola)[4].txt,++sk1);

  strcpy(st,stY) ;
  if (orto == 0)
  {
    strcpy(st,stN) ;
  }

  menu_par_new ((*mParametrym.pola)[3].txt, st) ;  

  d = jednostki_d_to_plt_d (Get_Point_Size ()) ;
  sprintf (sz_temp, "%#6.2lf", d) ;
  if (strlen (sz_temp) > 6) sz_temp [6] = '\0' ;
  strcat (sz_temp, "mm") ;

  menu_par_new ((*mParametrym.pola)[8].txt, sz_temp) ;


  strcpy(st,stY) ;
  if (FALSE == Get_Point_View ())
  {
    strcpy(st,stN) ;
  }

  menu_par_new ((*mParametrym.pola)[7].txt, st) ; 

  for (i = 0; i < MAXPLACEMARKER ; i++)
   {
     if (tab_place_marker [i].flags == 0) strcpy(st,"");
     else strcpy(st,"+");
     menu_par_new((*mSetMarker.pola)[i].txt,st);     
	 
   }
   
  if (swobodny_blok==FALSE) strcpy(st,stN);
    else strcpy(st,stY);
  menu_par_new((*mParametrym.pola)[12].txt,st);
  
  if (swobodny_tekst==FALSE) strcpy(st,stN);
    else strcpy(st,stY);
  menu_par_new((*mParametrym.pola)[13].txt,st);
  
  if (options1.scale_DIM==0) strcpy(st,stN);
    else strcpy(st,stY);
  menu_par_new((*mParametrym.pola)[14].txt,st);
  
   if (ciagniecie_linii==FALSE) strcpy(st,stN);
    else strcpy(st,stY);
  menu_par_new((*mParametrym.pola)[15].txt,st);
  
   if (rozcinanie_linii==FALSE) strcpy(st,stN);
    else strcpy(st,stY);
  menu_par_new((*mParametrym.pola)[16].txt,st);
  
   if (ukrywanie_typu==FALSE) strcpy(st,stN);
    else strcpy(st,stY);
  menu_par_new((*mParametrym.pola)[17].txt,st);
  
  if (type_view==FALSE) strcpy(st,stN);
    else strcpy(st,stY);

  menu_par_new((*mParametrym.pola)[18].txt,st);

  if (options1.fill_pcx_solid==0) strcpy(st,stN);
    else strcpy(st,stY);
  menu_par_new((*mParametrym.pola)[19].txt,st);

  if (options1.view_only_current_layer==0) strcpy(st,stN);
    else strcpy(st,stY);

  menu_par_new((*mParametrym.pola)[20].txt,st);

  if (options1.ignore_buffering_pcx==1) strcpy(st,stN);
    else strcpy(st,stY);
  menu_par_new((*mParametrym.pola)[21].txt,st);

  if (normalize_text==0) strcpy(st,stN);
    else strcpy(st,stY);
  menu_par_new((*mParametrym.pola)[22].txt,st);
  
  menu_par_new((*mPunkt.pola)[0].txt, ggsel[sel.nr]);
}
#define def67 67
#define def68 68
#define def69 69

static void hide_func(void)
{  int n1;

  SW[4] = SERV[def67];
  SW[5] = SERV[def68];
  SW[6] = MEDITFUN;

  SW[3]= SERV[117];

   for (n1 = 0; n1 < SVMAX-84 ; n1++)
        {
         SERV [n1+84] = nooop1;
        }

        SERV[117] = (void*)SW[3];

        SERV [60] = nooop1;
		SERV [61] = nooop1;
		SERV [62] = nooop1;
		SERV [63] = nooop1;
		SERV [64] = nooop1;
		SERV [65] = nooop1;

        SERV [66] = nooop1;
} 
     

static void return_func(void)
{
 
  SW[3] = SERV[117];

  standard_func();	 

  SERV[def67] = (void*)SW[4];
  SERV[def68] = (void*)SW[5];
  MEDITFUN = (void*)SW[6];

  SERV[117] = (void*)SW[3];
	  
}	  

static void (* COMNDm[])(void)={
/*0  menup*/    nooop, nooop, nooop,
                Set_Layers,
                nooop, Find_Text, nooop,

/*7  pUnkt*/    nooop, koniecS, koniec, srodek, przeciecie, centrum,
                prostopadle, stycznie, najblizszy, Point_Sel, JumpB, JumpW, JumpL,
/*20 tryb*/     TrybS, TrybS, TrybS, TrybS, TrybS, TrybS, TrybS, TrybS, TrybS, TrybS,

/*30 skala*/    SkalaR, nooop, nooop, SkalaE, SkalaZC, (void (*)(void)) Pan_Shift/*nooop*/, SkalaZ, SkalaZ, SkalaZ,
                SkalaZ, SkalaZX, SkalaW, Skala_format, last_view, BSkok, WSkok, LSkok,
/*47   17   2*/	   nooop, nooop, /*auto_pan_on, auto_pan_off,*/
/*49   19   5*/	   set_marker, set_marker, set_marker, set_marker, set_marker,
/*54   24   5*/	   find_marker, find_marker, find_marker, find_marker, find_marker,

/*59 parametry 0-3 */ swobodny_blok_on, swobodny_blok_off, swobodny_tekst_on, swobodny_tekst_off,

/*             4   */ ciagniecie_linii_on,
/*			   5-7 */  ciagniecie_linii_off, rozcinanie_linii_on, rozcinanie_linii_off,
/*             8-11*/   point_size, Wez_kolor, Wez_typ, Wez_warstwe,
/*71 (69)   12-20  */   KrokK, KrokK, KrokK, KrokK, KrokK, KrokK, KrokK, KrokKX, KrokM,
/*80 (78)   21-27  */   Gestosc, Gestosc, Gestosc, Gestosc, Gestosc, GestoscX, nooop,
/*87 (85)   28-29  */   grid_on_, grid_off_,
/*89 (87)   30-31  */   Orto_on, Orto_off,
/*91 (89)   32-64  */  typ_linii, typ_linii, typ_linii, typ_linii, typ_linii, typ_linii,
                typ_linii, typ_linii, typ_linii, typ_linii, typ_linii, typ_linii,
                typ_linii, typ_linii, typ_linii, typ_linii, typ_linii, typ_linii,
                typ_linii, typ_linii, typ_linii, typ_linii, typ_linii, typ_linii,
                typ_linii, typ_linii, typ_linii, typ_linii, typ_linii, typ_linii,
                typ_linii, typ_linii, typ_linii,
/*124 (122) 65-69  */grubosc_linii, grubosc_linii, grubosc_linii, grubosc_linii,
                grubosc_linii,

/*129 (127) 70-71 3*/point_view_on, point_view_off,
/*131 (129) 72-88 */ kolorL, kolorL, kolorL, kolorL, kolorL, kolorL, kolorL, kolorL,
                kolorL, kolorL, kolorL, kolorL, kolorL, kolorL, kolorL, kolorL, kolorX,

                ukrywanie_typu_on , ukrywanie_typu_off, type_view_on, type_view_off,
                scale_DIM_on, scale_DIM_off,

/*154       89    */Calculator_Measure, Point_Measure, Dist_Measure,
                Angle_Measure1, not_implemented, Information_about, nooop,
/*160       95    2*/    (void(*)(void))Break_Sum_Measure, (void(*)(void))Sum_Measure, 

/*163       97*/  nooop, wypelnianie_pcx_on, wypelnianie_pcx_off, view_only_c_l_on, view_only_c_l_off,
                buffering_pcx_enable_on, buffering_pcx_enable_off, nooop,

		   /*112*/ normalizacja_tekstu_on, normalizacja_tekstu_off,
                nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop,
                nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop, nooop,
                nooop, nooop,
           };


void set_plugin_func_in_COMNDm(int poz, void *plugin_f)
{
    COMNDm[poz]=plugin_f;
}

void (*CUSTOM_COMND)(int)=NULL;

int invoke_custom_comnd(char* c_comnd)
{
    CUSTOM_COMND=(void*)c_comnd;
    return 0;
}

int reject_custom_comnd(void)
{
    CUSTOM_COMND=NULL;
    return 0;
}


int small_step(void)
//------------------
{
 if (snap_ == 1) krok_old = milimetryob(krok_s);
 KrokM();
 return 0;
}

int large_step(void)
//------------------
{ char sk[48];
 if (snap_ == 1) return 0;
  else
   {
     krok_s = jednostkiOb (krok_old) ;
     snap_ = 1 ;
     DF_to_String (sk, "%-6.2f", milimetryob (krok_s), 6) ;
     kasowanie_licznikow=TRUE;

	 menu_par_new ((*mParametrym.pola)[0].txt, sk) ; 
	 menu_par_new ((*mKrok_k.pola)[7].txt, sk) ;
   }
 layer_info () ;
 return 0;
}

int small_large_step(void)
//------------------
{
	int n;
	if (snap_ == 1)  n = small_step();
	else n = large_step();
	return 0;
}

int osnap_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   if((n=getwsp1(&mPunkt)-1)==-1) 
     {
      return 0;  /* n=-1 -> esc */
     } 
   (*MPINI)(n);
   (*COMNDm[n])();
   return -1;
}


int koniecS_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   n=8;
   (*MPINI)(n);
   if (sel.akw) 
   { dzwonek();
    return 0;
   }
   (*COMNDm[n])();
   if (sel.wyj) return -1;
   else return 0;
}

int koniec_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   n=9;
   (*MPINI)(n);
   if (sel.akw) 
   { dzwonek();
    return 0;
   } 
   (*COMNDm[n])();
   return -1;
}

int srodek_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   n=10;
   (*MPINI)(n);
   if (sel.akw) 
   { dzwonek();
    return 0;
   }
   (*COMNDm[n])();
   if (sel.wyj) return -1;
   else return 0;
}

int przeciecie_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   n=11;
   (*MPINI)(n);
   if (sel.akw) 
   { dzwonek();
    return 0;
   }
   (*COMNDm[n])();

   if (sel.wyj) return -1;
   else return 0;
}

int centrum_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   n=12;
   (*MPINI)(n);
   if (sel.akw) 
   { dzwonek();
    return 0;
   }
   (*COMNDm[n])();
   if (sel.wyj) return -1;
   else return 0;
}

int prostopadle_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   n=13;
   (*MPINI)(n);
   if (sel.akw) 
   { dzwonek();
    return 0;
   }
   (*COMNDm[n])();
   if (sel.wyj) return -1;
   else return 0;
}

int stycznie_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   n=14;
   (*MPINI)(n);
   if (sel.akw) 
   { dzwonek();
    return 0;
   }
   (*COMNDm[n])();
   if (sel.wyj) return -1;
   else return 0;
}

int najblizszy_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   n=15;
   (*MPINI)(n);
   if (sel.akw) 
   { dzwonek();
    return 0;
   }
   (*COMNDm[n])();
   if (sel.wyj) return -1;
   else return 0;
}

int Point_Sel_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   n=16;
   (*MPINI)(n);
   if (sel.akw) 
   { dzwonek();
    return 0;
   }
   (*COMNDm[n])();
   if (sel.wyj) return -1;
   else return 0;
}

int JumpB_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   n=17;
   (*MPINI)(n);
   if (sel.akw) 
   { dzwonek();
    return 0;
   }
   (*COMNDm[n])();
   if (sel.wyj) return -1;
   else return 0;
}

int JumpL_(void)
{
	int n;
	uaktualnij_pola_mp();
	if (Error)
	{
		komunikat(Komunikat_R);
		Error = 0;
	}
	n = 19;
	(*MPINI)(n);
	if (sel.akw)
	{
		dzwonek();
		return 0;
	}
	(*COMNDm[n])();
	if (sel.wyj) return -1;
	else return 0;
}

int kolorL_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   if((n=getwsp1(&mKolorAC)-1)==-1) 
     {
      return 0;  /* n=-1 -> esc */
     }
   n+=59;
   (*MPINI)(n);
   (*COMNDm[n])();
   return 0;
}

int kolorL_bitmap(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   if((n=getwsp1(&mKolorAC)-1)==-1) 
     {
      return 0;  /* n=-1 -> esc */
     }
   n+=59;
   (*MPINI)(n);
   (*COMNDm[n])();
   return 1;
}

int grubosc_linii_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   if((n=getwsp1(&mGruboscLinii)-1)==-1) 
     {
      return 0;  /* n=-1 -> esc */
     }
   n+=59;
   (*MPINI)(n);
   (*COMNDm[n])();
   return 0;
}

int typ_linii_(void)
{ int n;
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
     Error=0;
   }
   if((n=getwsp1(&mTypyLinii)-1)==-1) 
     {
      return 0;  /* n=-1 -> esc */
     }
   n+=59;
   (*MPINI)(n);
   (*COMNDm[n])();
   return 0;
}

double get_skala(void)
{
	return skala;
}


int menupoz(void)
/*---------------*/
{
  int n;

   /*wylaczenie klawiszy funkcyjnych*/
   
   hide_func();
   clean_kbd();
   
   uaktualnij_pola_mp ();
   if(Error)
   { komunikat(Komunikat_R);
	 Error=0;
   }
   while(1)
	if((n=getwsp1(&menup)-1)==-1) 
	 {/*wlaczenie klawiszy funkcyjnych*/
          return_func(); 
	  return 0;  /* n=-1 -> esc */
	 } 
	 else
	 {
 	 (*MPINI)(n);
	 if (n>=8 && n<=17 && sel.akw) 
	  { dzwonek();
	    /*przywrocenie klawiszy funkcyjnych*/
	    return_func(); 
	    return 0;
	  }

    if(n>=1000) //custom menu
    {
        if (CUSTOM_COMND!=NULL)
            (*CUSTOM_COMND)(n-1000);
        /*przywrocenie klawiszy funkcyjnych*/
        return_func();
        return 0;
    }

    if(n>=COMNDmnr)
	 {
	  /*przywrocenie klawiszy funkcyjnych*/
          return_func(); 
	  return COMNDmnr-n-2;
	 } 
	 if (n < 0) return -1;  //ignore
	 (*COMNDm[n])();

	 if (n==35) LASTFUN = COMNDm[n];  //Pan_Shift

	 if (n>=8 && n<=19) 
	 {
	 /*przywrocenie klawiszy funkcyjnych*/
          return_func(); 
	  return -1;
	 } 
	 else 
	 {
	  /*przywrocenie klawiszy funkcyjnych*/
          return_func(); 
	  return 0;
	 } 
	}
  return_func(); 
  return 0;
}

#undef __ADMENU__