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

#define __O_BLOK__
#include <forwin.h>
#ifndef LINUX
#include <io.h>
#include <dos.h>
#include <process.h>
#endif
#include <fcntl.h>
#define ALLEGWIN
#include <allegext.h>
#include <ctype.h>
#include	<string.h>
#include	<math.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<limits.h>
#include <sys/stat.h>
#include	"bib_e.h"
#include	"rysuj_e.h"
#include	"o_bloklb.h"
#include	"o_block1.h"
#include	"bib_blok.h"
#include	"bib_edit.h"
#include	"b_messag.h"
#include	"o_loadf.h"
#include	"o_libfun.h"
#include	"o_object.h"
#include	"o_debug.h"
#include	"o_spec.h"
#include	"area.h"
#include    "alffont.h"

#include "menu.h"
/*--------------------------------------------*/

#define cursor_off(X,Y) out_cur_off(X,Y)
#define cursor_on(X,Y) out_cur_on(X,Y)
#define max_no_wiersz_symboli	255
#define MAXLINE 255

#ifndef isnan
#define isnan(a) (a != a)
#endif

BOOL global_no_pcx = FALSE;
BOOL global_only_pcx = FALSE;
BOOL global_only_solid = FALSE;
BOOL global_point_in_solid = FALSE;
BOOL global_any_choice = FALSE;
static char *ADP_0, *ADK_0;
static char *adr_sheet=NULL;
static char *adr_object=NULL;
static char *adres_pcx;

static double block_angle=0.0;

static OKNO o={0,0,0,0,0,0,0,0};

BOOL TTF_redraw = FALSE;


static int linia_tekst = 0;

BOOL ent_poly_info=TRUE;

BOOL dragging_quad=FALSE;
BOOL dragging_quad_move=FALSE;

#define SELECT_CROSS 0
#define SELECT_ENTITY 1

static int selection=SELECT_CROSS;

extern TMENU mBlok_Imp;
extern TMENU mBlok_Divide;
extern TMENU mCopy_Divide;
extern int	global_mode;
extern char numbers[16];
extern void undo_pcx_blockzap(void);
extern void set_hatch_pattern (int i_ev, int opcja, BOOL insulation_h);
extern QUAD quad0, quad1;
static void redcrq (char typ);
extern void Get_EllipticalArc_EndPoints (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double df_sangle0, double df_eangle0, double *x1, double *y1, double *x2, double *y2);
extern void set_komunikat0(int n, char *message);
extern void out_eCustom(void);
extern void qkoniec(double *x,double *y);
extern void qkoniec_o(double *x,double *y, void *adr);
extern void Set_Block_Window (OKNO *o);
extern int blok_single(int (*DZI)(void *), int (*ODZI)(void *), void (*REDROWd)(void), int (*COMND[])(void));
extern int vector_drag_wybrany(AVECTOR *ad);
extern void set_set_arc_stage(int stage);
extern int Inverse_VectorC(void);
extern int Inverse_VectorP(void);
extern int Reverse_VectorC(void);
extern int Reverse_VectorP(void);
extern void set_global_set_stretch(BOOL v_s_s);
extern void orto_l(LINIA *L, int *Orto_Dir);

enum OBIEKTT3C	  { NoAblokC,AblokC};
#define def67 67
#define def68 68

static double df_apx1, df_apy1, df_apx2, df_apy2 ;

static BOOL entire_poly=TRUE;

/*--------------------------------------------*/

/*-----------------------------------------------------------*/
static void	nooop	(void)
{}

static int nooop1	(void)
{
  return	0 ;
}

extern void adrem_obiekt(void *ad, int (*DZI)(void *), int (*ODZI)(void *), BOOL);
extern void	layer_info (void);
extern void	Change_Properties_dlg (void) ;
extern BOOL	Get_List_Block	(double *, double	*,	RYSPOZ *, RYSPOZ *, char *, int)	;
extern long_long Get_Size_Block(long_long offp, long_long offk) ;
extern long_long GetFreeMemSize(void)	;
extern int Extend_Proc (void)	;
extern int	Mirror_Proc	(void) ;
extern int	Mirror_Proc_X	(void) ;
extern int	Mirror_Proc_Y	(void) ;
extern void	Array_Rect_Proc (void) ;
extern void	Array_Polar_Proc (BOOL)	;
extern void	Divide_Block (double, double)	;
extern int	Hatch_Proc (BOOL,	double, double, double,	double);
extern int Trim_Proc	(void) ;
extern BOOL	axis_point (double *df_apx, double *df_apy);
extern void	redcrMP (int type, int type1);
extern void	view_font_name(TEXT *t);
extern void mazovia2latin(char *wiersz);
extern void mazovia2w1250(char *wiersz);
extern void latin2w1250(char *wiersz);
extern void setfillstyle_(int pattern, int color);
extern void setcolor(int kolor);
extern BOOL get_extend1 (char  **adp_l,char  **adk_l,int atrybut);
extern BOOL get_all (char  **adp_l,char  **adk_l);
extern BOOL get_all_special (char  **adp_l,char  **adk_l);
extern BOOL get_special_hatch25_from_layer (char  **adp_l,char  **adk_l, int nr_warstwy, int opcja);
extern BOOL get_all_from_layer (char  **adp_l,char  **adk_l, int nr_warstwy);
extern BOOL get_all_from_next_layers (char  **adp_l,char  **adk_l, int nr_pierwszej_warstwy);
extern BOOL get_all_texts (char  **adp_l,char  **adk_l);
extern BOOL get_all_texts_symbols (char  **adp_l,char  **adk_l) ;
extern BOOL get_all_layers (char  **adp_l,char  **adk_l);
extern int blok_Ctrl_R(int (*DZI)(void *), int (*ODZI)(void *), void (*REDROWd)(void),void (*COMND[])(void));
extern void delay(int time);
extern void Info_about_object(char *ad);
extern BLOK *FIRSTB(char  *ado);
extern void blokzap_no_pcx(char  *adp,char  *adk,int atrybut,int mode, int kolor);
extern int get_net(char  **adr);
extern int get_pomiar_param(char  **adr);
extern int get_f_s_pomiar_param(char **adr, int kanal_pomiar, double *x_pomiar, double *y_pomiar);
extern void standard_func(void);
extern BOOL Block_Proc_Wez_hw (int (*DZI)(void *), int (*ODZI)(void *), void (*REDROWd)(void), int (*COMND[])(void), int info);
extern void view_line_type(LINIA *L);
extern int blok_FIRSTB(int (*DZI)(void *), int (*ODZI)(void *), void (*REDROWd)(void),void (*COMND[])(void));
extern char *find_block_atrybut(char *adrp, char *adrk, char atrybut);
extern int Rotate_ProcXZ (void);
extern int Rotate_ProcYZ (void);
extern int __file_exists(char *name);
extern void blok_special(int (*DZI)(void *), int (*ODZI)(void *), void (*REDROWd)(void),void (*COMND[])(void));
extern void get_base_point(double *x_origin, double *y_origin);
extern void put_base_point(double *x_origin, double *y_origin);
extern void put_hatch_angle(double *hatch_angle);
extern void put_hatch_scale(double *hatch_scale);
extern void change_hatch_pattern(int hatch_type);
extern void put_hatch_distance(double hatch_distance);
extern BOOL Hatch_Proc_Test1 ( int comput_area, double df_apx1, double df_apy1, double df_apx2, double df_apy2 );
extern void pcx_outlines_view_on(void);
extern void pcx_outlines_view_off(void);
extern void set_ellipse_poligon_num(int msg_no);
extern void Change_Delete_Mode(void);
extern void rysuj_obiekt_no_pcx(char* ad, int mode, int kolor);
extern BOOL Block_Proc_Set_w(int (*DZI)(void*), int (*ODZI)(void*),
	void (*REDROWd)(void), int (*COMND[])(void), int info);
extern BOOL get_del_quick(void);
extern void set_solidhatch_pattern_name();
extern int zmien_atrybut_hatch_outline(char* adr, char* adrk, int aold, int anew, int blok);
extern int ask_question(int n_buttons, char* esc_string, char* ok_string, char* cont_string, char* comment_string, int color_comment, char* comment1_string, int color1_comment, int cien, int image);
extern void control_points_view_on(void);
extern void control_points_view_off(void);
extern BOOL get_b_delete_old(void);
extern void set_solid_pattern_name();
extern void set_solid_pattern_trans(char *sk);
extern void set_trace_pattern_trans(char *sk);
extern void blokquad(char  *adp,char  *adk,int atrybut,int mode, int kolor);
extern void blokquadfix(char  *adp,char  *adk,int atrybut,int mode, int kolor);
extern BOOL trans_scale_arcs_ (char *ADP_b, char *ADK_b, double df_basex, double df_basey, double df_sx, double df_sy, BOOL enforce);
extern int make_spline_shadows(long_long l_offb, long_long l_offe, int atrybut, BOOL join, int s_atrybut, int prec, int to_block);
extern BOOL Check_Attribute (int i_attribute, int i_check);

extern PTMENU m__list_hatch;
extern BOOL alert_on_object;
extern int get_stretch_vector(void);
extern void reset_stretch_vector(void);
extern void *get_vector_s(void);
extern int Vf_n (BOOL b_graph_value);
extern int Vf1_n (BOOL b_graph_value);
extern int Vf2_n (BOOL b_graph_value);
extern int Vth1_n (BOOL b_graph_value);
extern int Vm_n (BOOL b_graph_value);
extern int Vn_n (BOOL b_graph_value);
extern int Vr_n (BOOL b_graph_value);
extern int Vd_n (BOOL b_graph_value);
extern int Ve_n (BOOL b_graph_value);
extern void outvectoror (LINIA *L, AVECTOR *V, int mode,int pl);
extern void outvectoror1 (LINIA *L, AVECTOR *V, int mode,int pl);
extern void outvectoror2 (LINIA *L, AVECTOR *V, int mode,int pl);
extern void outvectoror3 (LINIA *L, AVECTOR *V, int mode,int pl);
extern void outvectoror4 (LINIA *L, AVECTOR *V, int mode,int pl);
extern void magnitude2angle(AVECTOR *V, double factor);

extern void set_eVa(ESTR *eVx);
extern void reset_eVa(void);

extern TMENU mLoad_Char_Thermal;
extern TMENU mLoad_Char;
extern TMENU mLoadVariant;

extern int get_menu_level(void);
extern void set_menu_level(int menu_l);

extern int Simple_Menu_Proc (TMENU *) ;
extern void reset_trace_block(void);
extern void blokzap_deep(char  *adp,char  *adk,int atrybut,int mode, int kolor);

extern double depth_magnitude; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double thermal_magnitude; //units per mm  default 1 Celsius per 1 mm on drawing paper
extern double load_magnitude; //units per mm  default 10kN/m force per 1 mm on drawing paper
extern double force_magnitude; //units per mm  default 10kN force per 1 mm on drawing paper
extern double moment_magnitude; //units per mm  default 10kNm force per 1 mm radius on drawing paper
extern double displacement_magnitude; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
extern double rotation_magnitude;

AVECTOR VectorC=Vdef;

void set_trace_pattern_name();

static double DX,DY,Px,Py ;
static LINIA L=Ldef;
int *win0;
int win00;

static char *format_float="%#9.4lf";
static char *format_floatd="%#11.6lf";
static char *format_float2="%#9.4lf;%#9.4lf";
static char *format2_float2="%#9.4lf\0%#9.4lf;%#9.4lf";

static void	obrot_kopiuj_K(void);

/*------------  menu poziome ------------------------*/
	 
TMENU	mBlokm={6,0,0,20,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmBlokm,NULL,NULL};

TMENU	mScalem = { 1,0,0,20,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmScalem,NULL,NULL };

TMENU	mBlokmc={3,0,0,20,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmBlokmc,NULL,NULL};

TMENU	mSelectMode={2,0,0,20,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmSelectMode,NULL,NULL};

TMENU	mBlock_wez_w =	{2,0,0,15,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmBlock_wez_w,NULL,NULL}	;
TMENU	mBlock_wez_info =	{2,0,0,15,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmBlock_wez_info,NULL,NULL}	;
TMENU	mBlock_dq =	{4,0,0,15,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmBlock_dq,NULL,NULL}	;
TMENU	mBlock_dn =	{9,0,0,17,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmBlock_dn,NULL,NULL}	;


TMENU	mBlok_ex={7,0,0,17,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmBlok_ex,NULL,NULL};

TMENU	mBlok_chp={9,0,0,17,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmBlok_chp,NULL,NULL};

TMENU	mBlok_cht={5,0,0,17,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmBlok_cht,NULL,NULL};

TMENU	mBlok_chts={6,0,0,17,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmBlok_chts,NULL,NULL};

TMENU	mCopy_Divide={1,0,0,15,56,4,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmCopy_Divide,	NULL,	NULL}	;

static TMENU mWszystko_Okno={2,0,0,8,56,4,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,(POLE(*)[]) &pmWszystko_Okno,	NULL,	NULL}	;

void *get_vector_c(void)
{
    return &VectorC;
}


static void	winf_draw (void)
//---------------------------
{
  int	*win ;
  int s30;
  int s11;
   
  s30 = 30 * WIDTH / 8;
  s11 = 11 * HEIGHT / 12;

  win	= Block_Proc_Win_Mode_Ptr () ;
  setfillstyle_	(SOLID_FILL, BKCOLOR) ;
  bar	(maxX	/ 2,	0,	maxX / 2 	+ 2*s30,	s11) ;
  moveto	(maxX	/ 2 + 2,	1)	;
  setcolor (kolory.ink)	;
  if (*win == Windoww)
  {
	 outtext_r (window) ;
  }
  else
  {
	 outtext_r (cross)	;
  }
  return	;
}

static int winf (void)
//--------------------
{
  int	*win ;

  win	= Block_Proc_Win_Mode_Ptr () ;
  if (*win == Windoww)
  {
	 *win	= Cross ;
  }
  else
  {
	 *win	= Windoww	;
  }
  winf_draw	()	;
  return	0 ;
}

static void	addf_draw (void)
//---------------------------
{
  BOOL *add	;
  int s60;
  int s11;

  s60 = 60 * WIDTH / 8;
  s11 = 11 * HEIGHT / 12;

  add	= Block_Proc_Add_Mode_Ptr () ;
  setfillstyle_	(SOLID_FILL, BKCOLOR) ;
  bar	(maxX	/ 2 +	s60, 0, maxX / 2 + 3*s60,	s11) ;
  moveto	(maxX	/ 2 +	s60	+ 2, 1) ;
  setcolor (kolory.ink)	;
  if (*add == DEL_MODE)
  {
	 outtext_r (_remove_) ;
  }
  else
  {
	 outtext_r (_addto_)	;
  }
}

static int addf (void)
//--------------------
{
  BOOL *add	;

  add	= Block_Proc_Add_Mode_Ptr () ;
  if (*add == ADD_MODE)
  {
	 *add	= DEL_MODE ;
  }
  else
  {
	 *add	= ADD_MODE ;
  }
  addf_draw	()	;
  return	0 ;
}

static int select_cross(void)
{
    selection=SELECT_CROSS;
    return 1;
}

static int select_entity(void)
{
    selection=SELECT_ENTITY;
    return 1;
}

int get_selection(void)
{
    return selection;
}

int set_selection(int new_selection)
{
    selection=new_selection;
}

static int auto_on (void)
/*---------------------*/
{

  Set_Block_Proc_Auto_Mode	(TRUE) ;
  menu_par_new	((*mBlokm.pola)[5].txt,	Yes);
  menu_par_new	((*mBlokmc.pola)[2].txt, Yes);
  menu_par_new	((*mBlok_ex.pola)[5].txt, Yes);
  menu_par_new	((*mBlok_chp.pola)[5].txt,	Yes);
  menu_par_new	((*mBlok_cht.pola)[4].txt,	Yes);
  menu_par_new	((*mBlock_dn.pola)[5].txt,	Yes);
  menu_par_new	((*mBlock_dq.pola)[1].txt,	Yes);
  menu_par_new	((*mBlock_wez_w.pola)[1].txt,	Yes);
  return 0;//	1;
}

static int auto_off (void)
/*---------------------*/
{
  Set_Block_Proc_Auto_Mode	(FALSE) ;
  menu_par_new	((*mBlokm.pola)[5].txt,	"N");
  menu_par_new	((*mBlokmc.pola)[2].txt, "N");
  menu_par_new	((*mBlok_ex.pola)[5].txt, "N");
  menu_par_new	((*mBlok_chp.pola)[5].txt,	"N");
  menu_par_new	((*mBlok_cht.pola)[5].txt,	"N");
  menu_par_new	((*mBlock_dn.pola)[5].txt,	"N");
  menu_par_new	((*mBlock_dq.pola)[1].txt,	"N");
  menu_par_new	((*mBlock_wez_w.pola)[1].txt,	"N");
  return 0; //	1;
}

static int entire_polyline(void)
{
    entire_poly=TRUE;
    menu_par_new((*menup.pola)[6].txt, _entire_polyline_);
}

static int single_object(void)
{
    entire_poly=FALSE;
    menu_par_new((*menup.pola)[6].txt, _single_object_);
}

BOOL get_entire_poly(void)
{
    return entire_poly;
}

void set_entire_poly(BOOL ent_poly)
{
    entire_poly=ent_poly;
}

static int ostatni_blok	(void)
/*---------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;
  BOOL b_hid, b_edit;
  BOOL kk;

  kk=get_extend1 (&adp0, &adk0,Aoblok);
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  b_hid = check_if_hidden (adp0,	adk0,	Aoblok);
  b_edit	= check_if_edit (adp0, adk0, Aoblok);
  if (b_edit == TRUE)
  {
	 ErrList	(54) ;
  }
  if (b_hid	==	TRUE)
  {
	 ErrList	(50) ;
  }
  Cur_offd(X, Y);
  blokzap(adp0, adk0, Aoblok,	XOR_PUT,	1);
  zmien_atrybut(adp0, adk0, Aoblok,	Ablok);
  Cur_ond(X, Y);
  
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
  return 0;
}

int wszystko (void)
/*---------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;
  BOOL b_hid, b_edit;
  BOOL kk;

  kk=get_all (&adp0,	&adk0);
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  b_hid = check_if_hidden (adp0, adk0, Ablok);  //16.08.2007
  b_edit = check_if_edit (adp0, adk0, Ablok);  //16.08.2007
  if (b_edit == TRUE)
  {
	 ErrList	(54) ;
  }
  if (b_hid	==	TRUE)
  {
	 ErrList	(50) ;
  }
  blokzap(adp0, adk0, Ablok, XOR_PUT, 1);
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
  CUR_OFF(X,Y);
  CUR_ON(X,Y);
  return	0;
}

int wszystko_special	(void)
/*------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;
  BOOL b_hid, b_edit;
  BOOL kk;

  blokzap(dane, dane	+ dane_size, Ablok, XOR_PUT, 1);
  kk=get_all_special	(&adp0, &adk0);
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  b_hid = check_if_hidden (adp0, adk0, Ablok); //16.08.2007
  b_edit = check_if_edit (adp0, adk0, Ablok); //16.08.2007
  if (b_edit == TRUE)
  {
	 ErrList	(54) ;
  }
  if (b_hid	==	TRUE)
  {
	 ErrList	(50) ;
  }
  blokzap(adp0, adk0, Ablok, XOR_PUT, 1);
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
  return	0;
}

int special_hatch25_z_warstwy (int nr_warstwy)
/*-----------------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;
  BOOL b_hid, b_edit;
  BOOL kk;

  blokzap(dane, dane	+ dane_size, Ablok, XOR_PUT, 1);
  kk=get_special_hatch25_from_layer(&adp0, &adk0, nr_warstwy, 3);  //1 - special, 2-hatch25, 3-all, 4-normal, 8-aparat
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  b_hid = check_if_hidden (adp0, adk0, Ablok); //16.08.2007
  b_edit = check_if_edit (adp0, adk0, Ablok);  //16.08.2007
  if (b_edit == TRUE)
  {
	 ErrList	(54) ;
  }
  if (b_hid	==	TRUE)
  {
	 ErrList	(50) ;
  }
  blokzap(adp0, adk0, Ablok, XOR_PUT, 1);
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
  return	1;
}

int special_hatch25_otwory_z_warstwy (int nr_warstwy, double bok_otworu)
/*--------------------------------------------------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;
  BOOL b_hid, b_edit;

  blokzap(dane, dane	+ dane_size, Ablok, XOR_PUT, 1);
  get_special_hatch25_from_layer(&adp0, &adk0, nr_warstwy, 11);  //1 - special, 2-hatch25, 3-all, 4-normal, 8-aparat
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  b_hid = check_if_hidden (adp0, adk0, Ablok); //16.08.2007
  b_edit = check_if_edit (adp0, adk0, Ablok); //16.08.2007
  if (b_edit == TRUE)
  {
	 ErrList	(54) ;
  }
  if (b_hid	==	TRUE)
  {
	 ErrList	(50) ;
  }
  blokzap(adp0, adk0, Ablok, XOR_PUT, 1);
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
  return	1;
}

int special_z_warstwy (int nr_warstwy)
/*-----------------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;
  BOOL b_hid, b_edit;

  blokzap(dane, dane	+ dane_size, Ablok, XOR_PUT, 1);
  get_special_hatch25_from_layer(&adp0, &adk0, nr_warstwy, 1);
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  b_hid = check_if_hidden (adp0, adk0, Ablok); //16.08.2007
  b_edit = check_if_edit (adp0, adk0, Ablok); //16.08.2007
  if (b_edit == TRUE)
  {
	 ErrList	(54) ;
  }
  if (b_hid	==	TRUE)
  {
	 ErrList	(50) ;
  }
  blokzap(adp0, adk0, Ablok, XOR_PUT, 1);
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
  return	1;
}

int special_hatch25_border_z_warstwy (int nr_warstwy)
/*-------------------------------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;
  BOOL b_hid, b_edit;

  blokzap(dane, dane	+ dane_size, Ablok, XOR_PUT, 1);
  get_special_hatch25_from_layer(&adp0, &adk0, nr_warstwy, 7);
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  b_hid = check_if_hidden (adp0, adk0, Ablok);  //16.08.2007
  b_edit = check_if_edit (adp0, adk0, Ablok); //16.08.2007
  if (b_edit == TRUE)
  {
	 ErrList	(54) ;
  }
  if (b_hid	==	TRUE)
  {
	 ErrList	(50) ;
  }
  blokzap(adp0, adk0, Ablok, XOR_PUT, 1);
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
  return	1;
}

int aparaty_z_warstwy (int nr_warstwy)
/*---------------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;
  BOOL b_hid, b_edit;

  blokzap(dane, dane	+ dane_size, Ablok, XOR_PUT, 1);
  get_special_hatch25_from_layer(&adp0, &adk0, nr_warstwy, 8);
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  b_hid = check_if_hidden (adp0, adk0, Ablok); //16.08.2007
  b_edit = check_if_edit (adp0, adk0, Ablok); //16.08.2007
  if (b_edit == TRUE)
  {
	 ErrList	(54) ;
  }
  if (b_hid	==	TRUE)
  {
	 ErrList	(50) ;
  }
  blokzap(adp0, adk0, Ablok, XOR_PUT, 1);
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
  return	1;
}


int wszystko_z_warstwy (int nr_warstwy)
/*-----------------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;
  BOOL b_hid, b_edit;
  BOOL kk;

  blokzap(dane, dane	+ dane_size, Ablok, XOR_PUT, 1);
  kk=get_all_from_layer	(&adp0, &adk0, nr_warstwy);
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  b_hid = check_if_hidden (adp0, adk0, Ablok); //16.08.2007
  b_edit = check_if_edit (adp0, adk0, Ablok); //16.08.2007
  if (b_edit == TRUE)
  {
	 ErrList	(54) ;
  }
  if (b_hid	==	TRUE)
  {
	 ErrList	(50) ;
  }
  blokzap(adp0, adk0, Ablok, XOR_PUT, 1);
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
  return	1;
}

int wszystko_od_warstwy (int nr_pierwszej_warstwy)
/*----------------------------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;
  BOOL b_hid, b_edit;
  BOOL kk;

  blokzap(dane, dane	+ dane_size, Ablok, XOR_PUT, 1);
  kk=get_all_from_next_layers (&adp0, &adk0, nr_pierwszej_warstwy);
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  b_hid = check_if_hidden (adp0, adk0, Ablok);  //16.08.2007
  b_edit = check_if_edit (adp0, adk0, Ablok); //16.08.2007
  if (b_edit == TRUE)
  {
	 ErrList	(54) ;
  }
  if (b_hid	==	TRUE)
  {
	 ErrList	(50) ;
  }
  blokzap(adp0, adk0, Ablok, XOR_PUT, 1);
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
  return	1;
}

static int wszystkie_teksty (void)
/*---------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;
  BOOL b_hid, b_edit;
  BOOL kk;

  blokzap(dane, dane	+ dane_size, Ablok, XOR_PUT, 1);
  kk=get_all_texts (&adp0,	&adk0);
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  b_hid = check_if_hidden (adp0, adk0, Ablok);  //16.08.2007
  b_edit = check_if_edit (adp0, adk0, Ablok); //16.08.2007
  if (b_edit == TRUE)
  {
	 ErrList	(54) ;
  }
  if (b_hid	==	TRUE)
  {
	 ErrList	(50) ;
  }
  blokzap(dane, dane	+ dane_size, Ablok, XOR_PUT, 1);
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
  return	0;
}

static int wszystkie_teksty_symbole	(void)
/*--------------------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;
  BOOL b_hid, b_edit;
  BOOL kk;

  blokzap(dane, dane	+ dane_size, Ablok, XOR_PUT, 1);
  kk=get_all_texts_symbols	(&adp0, &adk0);
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  b_hid = check_if_hidden (adp0, adk0, Ablok);  //16.08.2007
  b_edit = check_if_edit (adp0, adk0, Ablok); //16.08.2007
  if (b_edit == TRUE)
  {
	 ErrList	(54) ;
  }
  if (b_hid	==	TRUE)
  {
	 ErrList	(50) ;
  }
  blokzap(dane, dane	+ dane_size, Ablok, XOR_PUT, 1);
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
  return	0;
}

static int wszystkie_warstwy (void)
/*------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;
  BOOL b_hid, b_edit;
  BOOL kk;

  blokzap(dane, dane	+ dane_size, Ablok, XOR_PUT, 1);
  kk=get_all_layers (&adp0, &adk0);
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  b_hid = check_if_hidden (adp0,	adk0,	Ablok);
  b_edit	= check_if_edit (adp0, adk0, Ablok);
  if (b_edit == TRUE)
  {
	 ErrList	(54) ;
  }
  if (b_hid	==	TRUE)
  {
	 ErrList	(50) ;
  }
  blokzap(adp0, adk0, Ablok, XOR_PUT, 1);
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
  return	0;
}


static int all_from_layer(void)
/*-------------------------------*/
{
	char  *adp0 = NULL, *adk0 = NULL;
	int sel_layer;
	char sk[MaxTextLen] = "";

	if (!get_string(sk, numbers, MaxTextLen, 0, 195)) return 0;
	sel_layer = atoi(sk) - 1;
	if ((sel_layer > 255) || (sel_layer < 0))
	{
		ErrList(214);
		return 1;
	}

	wszystko_z_warstwy(sel_layer);
	return 0;
}

int wszystkie_warstwy1 (void)
/*------------------------*/
{
  char  *adp0 = NULL,  *adk0 = NULL;

  get_all_layers (&adp0, &adk0);
  if (adp0 == NULL)
  {
	 return 0 ;
  }
  if (ADP >	adp0 || ADP	==	NULL)	ADP =	adp0;
  if(ADK	< adk0)	ADK =	adk0;
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
  return	0;
}

static int	(*COMNDmb[])(void)=
{ostatni_blok, wszystko, wszystkie_warstwy,
 winf, addf, nooop1, nooop1,
 auto_on, auto_off };
static int (*COMNDmbc[])(void)=
{ winf,addf,	nooop1, nooop1, nooop1,
  auto_on,	auto_off };
static int (*COMNDmbs[])(void)=
        { select_cross,select_entity};

/*-------  przesun k  ------------------------*/

/*------------------kopia z rysuj linia (poprawic)-------*/
#define r18	18
#define r22	16 //22
static int strwyj;
static double DX0, DY0;

static int L_p	(BOOL	b_graph_value)
{
  PLINIA	PL;
  double	l,k;
  double	angle_l;

  b_graph_value =	b_graph_value ;
  if (eL.val_no <	1)
  {
	 return 0 ;
  }
  l =	eL.values [0] ;
  if ( orto	!=	0 || eL.val_no	==	1)
  {
	 parametry_linior(&L,&PL);
	 k=PL.kat;
  }
  else
  {
	 angle_l=get_angle_l();
	 if (angle_l!=0)
	 {
	  k =	eL.values [1] + angle_l;
	  if (k<0) k+=360;
	 }
	  else
		 k	= eL.values	[1] ;
  }
  l =	jednostkiOb(l);
  k =	k * Pi / 180.0;
  DX0	= Double_to_Float	(l	* cos	(k));
  DY0	= Double_to_Float	(l	* sin	(k));
  strwyj=1;
  return	1;
}

static int Vf_n_(BOOL b_graph_value)
{
    int ret = Vf_n(b_graph_value);
    strwyj=1;
    VectorC.x2=VectorG.x2;
    VectorC.y2=VectorG.y2;
    VectorC.magnitude1=VectorG.magnitude1;
    return ret;
}

static int Vf1_n_(BOOL b_graph_value)
{
    int ret = Vf1_n(b_graph_value);
    strwyj=1;
    VectorC.magnitude1=VectorG.magnitude1;
    return ret;
}

static int Vf2_n_(BOOL b_graph_value)
{
    int ret = Vf2_n(b_graph_value);
    strwyj=1;
    VectorC.magnitude2=VectorG.magnitude2;
    return ret;
}

static int Vth1_n_(BOOL b_graph_value)
{
    int ret = Vth1_n(b_graph_value);
    strwyj=1;
    VectorC.r=VectorG.r;
    return ret;
}

static int Vth2_n_(BOOL b_graph_value)
{
    int ret = Vth1_n(b_graph_value);
    strwyj=1;
    VectorC.r=VectorG.r;
    return ret;
}

static int Vm_n_(BOOL b_graph_value)
{   unsigned int style;
    style=VectorG.style;
    VectorG.style=VectorC.style;
    int ret = Vm_n(b_graph_value);
    strwyj=1;
    VectorC.magnitude1=VectorG.magnitude1;
    VectorC.style=VectorG.style;  //in case of changing sign
    magnitude2angle(&VectorC, moment_magnitude);
    VectorG.style=style;
    return ret;
}

static int Vr_n_(BOOL b_graph_value)
{   unsigned int style;
    style=VectorG.style;
    VectorG.style=VectorC.style;
    int ret = Vr_n(b_graph_value);
    strwyj=1;
    VectorC.magnitude1=VectorG.magnitude1;
    VectorC.style=VectorG.style;  //in case of changing sign
    magnitude2angle(&VectorC, rotation_magnitude);
    VectorG.style=style;
    return ret;
}

static int Vd_n_(BOOL b_graph_value)
{
    int ret = Vd_n(b_graph_value);
    strwyj=1;
    VectorC.x2=VectorG.x2;
    VectorC.y2=VectorG.y2;
    VectorC.magnitude1=VectorG.magnitude1;
    return ret;
}

static int Vn_n_(BOOL b_graph_value)
{
    int ret = Vn_n(b_graph_value);
    strwyj=1;
    VectorC.magnitude1=VectorG.magnitude1;
    VectorC.r=VectorG.r;
    return ret;
}

static int Ve_n_(BOOL b_graph_value)
{
    int ret = Ve_n(b_graph_value);
    strwyj=1;
    VectorC.property_no=VectorG.property_no;
    return ret;
}

/*------------------*/
static void	cur_offq(double x,double y)
{
    flip_screen();
}

static void	cur_offk(double x,double y)
{
    flip_screen();
}

static void	cur_offk__(double x,double y)
{ 
  cursel_off(x, y);
  out_blok1(DX,DY,0,0,Tprzesuw,0);
  outlineor(&L,XOR_PUT,0);
}

static void	cur_offkZ(double x,double y)
{
  cursel_off(x, y);
  out_blok1(DX,DY,0,0,TprzesuwZ,0);
  outlineor(&L,XOR_PUT,0);
}


BOOL get_dragging_quad(void)
{
    return dragging_quad;
}


static void	cur_onq(double	x,double	y)
{
    PLINIA	PL;
    cursel_on(x, y);
    L.x2=x; L.y2=y;
    outlineor(&L,COPY_PUT,1);
    parametry_linior (&L,	&PL);
    DX = PL.dl *	PL.cos;	DY= PL.dl *	PL.sin;
    out_blok1(DX,DY,0,0,Tprzesuw,0);
    //block transformation
    blokquad(ADPQ, ADKQ, Aoblok, COPY_PUT, 1);
    cursel_on(x, y);
}

static void	cur_onk(double	x,double	y)
{
  PLINIA	PL;
  cursel_on(x, y);
  L.x2=x; L.y2=y;
  outlineor(&L,COPY_PUT,1);
  parametry_linior (&L,	&PL);
  DX = PL.dl *	PL.cos;	DY= PL.dl *	PL.sin;
  out_blok1(DX,DY,0,0,Tprzesuw,0);
  cursel_on(x, y);
}

static void  cur_onv(double x,double y)
/*----------------------------------------*/
{
    LiniaG.x2=x; LiniaG.y2=y;
    outvectoror (&LiniaG, &VectorC, COPY_PUT, 1) ;
    cursor_on(x, y);
}

static void  cur_onv1(double x,double y)
/*----------------------------------------*/
{
    LiniaG.x2=x; LiniaG.y2=y;
    outvectoror1 (&LiniaG, &VectorC, COPY_PUT, 1) ;
    cursor_on(x, y);
}

static void  cur_onv2(double x,double y)
/*----------------------------------------*/
{
    LiniaG.x2=x; LiniaG.y2=y;
    outvectoror2 (&LiniaG, &VectorC, COPY_PUT, 1) ;
    cursor_on(x, y);
}

static void  cur_onv3(double x,double y)
/*----------------------------------------*/
{
    LiniaG.x2=x; LiniaG.y2=y;
    outvectoror3 (&LiniaG, &VectorC, COPY_PUT, 1) ;
    cursor_on(x, y);
}

static void  cur_onv4(double x,double y)
/*----------------------------------------*/
{
    LiniaG.x2=x; LiniaG.y2=y;
    outvectoror3 (&LiniaG, &VectorC, COPY_PUT, 1) ;
    cursor_on(x, y);
}

static void  cur_onv5(double x,double y)
/*----------------------------------------*/
{
    LiniaG.x2=x; LiniaG.y2=y;
    outvectoror4 (&LiniaG, &VectorC, COPY_PUT, 1) ;
    cursor_on(x, y);
}

static void	cur_onkZ(double	x,double	y)
{
  PLINIA	PL;
  cursel_on(x, y);
  L.x2=x; L.y2=y;
  outlineor(&L,XOR_PUT,1);
  parametry_linior (&L,	&PL);
  DX = PL.dl *	PL.cos;	DY= PL.dl *	PL.sin;
  out_blok1(DX,DY,0,0,TprzesuwZ,0);
  cursel_on(x, y);
}


static void	redcrpk(char typ)
{
	static int np;
  static	int (*SW[2])();

	switch (typ)
	{
	  case 0:
		 Cur_offd(X, Y);
		 mvcurbp(1);
		 CUR_OFF=cur_offk;
		 CUR_ON=cur_onk;
		 eL.x=maxX-PL266;
		 eL.y= ESTR_Y;
		 eL.lmax=r22;
		 eL.val_no_max	= 2 ;
		 eL.mode	= GV_VECTOR	;
		 eL.format = "%#12.9lg\0%#12.9lg;%#8.3lf"	;
		 eL.ESTRF=L_p;
		 eL.extend = 0;
		 np=dodajstr(&eL);
		 SW [0] = SERV	[73] ;  SERV [73]	= sel_t ;
		 SW [1] = SERV	[81] ;  SERV [81]	= sel_d ;
		 Cur_ond(X, Y);
		 break;
	  case 1:
        pcx_outlines_view_off();
        blokzap(ADP, ADK, Ablok, COPY_PUT, 1);

	  case 2:
		 usunstr(np);
		 SERV	[73] = (void *)SW [0] ;
		 SERV	[81] = (void *)SW [1] ;
		 break;
  }
}

static void	redcrpkZ(char typ)
{
	static int np;
  static	int (*SW[2])();

	switch (typ)
	{
	  case 0:
		 mvcurbp(1);
		 CUR_OFF=cur_offkZ;
		 CUR_ON=cur_onkZ;
		 eL.x=maxX-PL266;
		 eL.y= ESTR_Y;
		 eL.lmax=r22;
		 eL.val_no_max	= 2 ;
		 eL.mode	= GV_VECTOR	;
		 eL.format = "%#12.9lg\0%#12.9lg;%#8.3lf"	;
		 eL.ESTRF=L_p;
		 eL.extend = 0;
		 np=dodajstr(&eL);
		 SW [0] = SERV	[73] ;  SERV [73]	= sel_t ;
		 SW [1] = SERV	[81] ;  SERV [81]	= sel_d ;
		 break;
	  case 1:
		 Cur_offd(X,Y);
	  case 2:
		 usunstr(np);
		 SERV	[73] = (void*)SW [0] ;
		 SERV	[81] = (void*)SW [1] ;
		 break;
  }
}

/*--------------------------------------------*/
/*------  przesun p  -------------------------*/
static void	 cur_offp(double x,double y)
{ 
	cursel_off(x,y);
}

static void	 cur_onp(double x,double y)
{ 
	cursel_on(x,y);
 }

static void	redcrpp(char typ)
{
  static	void (*REDdod)(void)=nooop;
  static	int (	*SW[2])();

  switch(typ)
	{ case 0	:
		Cur_offd(X, Y);
		CUR_OFF=cur_offp;
		CUR_ON=cur_onp;
		REDdod=REDDOD;	REDDOD=Redraw_Block;
		SW	[0] =	SERV [73] ;	 SERV	[73] = sel_t ;
		SW	[1] =	SERV [81] ;	 SERV	[81] = sel_d ;
		Cur_ond(X, Y);
		break;
	  case 1	:Cur_offd(X,Y);
	  case 2	:
		REDDOD=REDdod;
		SERV [73] = (void*)SW	[0] ;
		SERV [81] = (void*)SW	[1] ;
		komunikat0(0);
		break;
	  default :	break;
	}
}

static int przesunp(void)
{
  double	X0,Y0;
  EVENT *ev;
  redcrpp(0);
  while(1)
	{
	  ev=Get_Event_Point(NULL,	&X0, &Y0);
	  if(ev->What == evKeyDown	&&	 ev->Number==0) {	redcrpp(1);	return ESC;	}
	  if (ev->What	==	evKeyDown &&  ev->Number== ENTER)
		 {	redcrpp(2);
	       return ENTER;
		 }
	  if (ev->What == evCommandP && ev->Number == 0)
	  {
		  set_ellipse_poligon_num(192);
	  }
	}
}

static int przesunpZ(void)
{
  double	X0,Y0;
  EVENT *ev;
  redcrpp(0);
  while(1)
	{
	  ev=Get_Event_Point(NULL,	&X0, &Y0);
	  if(ev->What == evKeyDown	&&	 ev->Number==0) {	redcrpp(1);	return ESC;	}
	  if (ev->What	==	evKeyDown &&  ev->Number== ENTER)
		 {	redcrpp(2);
	 return ENTER;
		 }
	}
}


static int przesunp0(void)
{
  redcrpp(0);
  redcrpp(2);
  return ENTER;
}


/*--------------------------------------------*/

int dzi(void *ad)
{
  NAGLOWEK *nag;

  nag	= (NAGLOWEK*)ad ;
  rysuj_obiekt	(ad, XOR_PUT, 1) ;
  nag->atrybut=Ablok;
  if ((nag->obiekt == Olinia || nag->obiekt == Ovector || nag->obiekt == Oluk || nag->obiekt == Oellipticalarc || nag->obiekt == Osolidarc) &&
		(nag->obiektt1	==	Utwierdzony1 || nag->obiektt1	==	Utwierdzony2))
  {
	 nag->obiektt1	= Guma ;
  }
  return	1;
}

int dzi_pcx(void *ad)
{
  NAGLOWEK *nag;

  nag	= (NAGLOWEK*)ad ;
  if (nag -> obiekt == Opcx)
  {
    rysuj_obiekt	(ad, XOR_PUT, 1) ;
    nag->atrybut=Ablok;
    adres_pcx=ad;
    return	1;
  }
 return 0;
}




int dzi_special(void *ad)
{
  NAGLOWEK *nag;

  nag	= (NAGLOWEK*)ad ;
  if ((nag->obiektt2==O2BlockPline) ||
      (nag->obiektt2==O2BlockSpecial) ||
      (nag->obiektt2==O2BlockAparat))
   {
    rysuj_obiekt	(ad, XOR_PUT, 1) ;
    nag->atrybut=Ablok;
    if ((nag->obiekt == Olinia || nag->obiekt == Oluk) &&
		(nag->obiektt1	==	Utwierdzony1 || nag->obiektt1	==	Utwierdzony2))
    {
   	 nag->obiektt1	= Guma ;
    }
   }
  return	1;
}



int odzi(void *ad)
{
  rysuj_obiekt	(ad, COPY_PUT, 1) ;
  ((NAGLOWEK*)ad)->atrybut=Anormalny;
  return	1;
}


int dzi_off(void *ad)
{
  NAGLOWEK *nag;

  nag	= (NAGLOWEK*)ad ;
  nag->atrybut=Ablok;
  if ((nag->obiekt == Olinia  || nag->obiekt == Oluk) &&
		(nag->obiektt1	==	Utwierdzony1 || nag->obiektt1	==	Utwierdzony2))
  {
	 nag->obiektt1	= Guma ;
  }
  return	1;
}



int odzi_off(void	*ad)
{
  ((NAGLOWEK*)ad)->atrybut=Anormalny;
  return	1;
}



static void	skala_kopiuj (void)
/*---------------------------*/
{
  char sk [MaxTextLen] ;
  int	retval_no =	2 ;
  double	buf_ret [2]	;
  double	k01, k02	;
  static	BOOL b_check_arc ;

  sk [0]	= '\0' ;
  if (!get_string	(sk, "",	MaxTextLen,	0,	17)) return	;
  if (FALSE	==	calculator (sk, &retval_no, buf_ret)  || retval_no	< 1)
  {
	 return ;
  }
  k02	= k01	= buf_ret [0] ;
  if ( retval_no > 1)
  {
	 k02 = buf_ret	[1] ;
  }
  if (k01 == 0	||	k02 == 0)
  {
	 return ;
  }
  if (b_check_arc	==	TRUE && fabs (k01) != fabs	(k02))
  {
	 ErrList	(56) ;
	 return ;
  }
  Cur_offd (X,	Y)	;
  transformacja_blok	(ADP,	ADK, L.x1 ,	L.y1 /*Px, Py*/, k01, k02,	Tskala,0) ;
  Cur_ond (X, Y) ;
}

static void	obrot_kopiuj (void)
/*---------------------------*/
{
  char sk [MaxTextLen] ;
  int	retval_no =	1 ;
  double	buf_ret [1]	;
  double	kos, koc	;
  double kat_obrotu;

  sk [0]	= '\0' ;
  if (!get_string	(sk, "",	MaxTextLen,	0,	16)) return	;
  if (FALSE	==	calculator (sk, &retval_no, buf_ret)  || retval_no	< 1)
  {
	 return ;
  }

  kat_obrotu = buf_ret[0] * Pi / 180;
  kos = sin(kat_obrotu);
  koc = cos(kat_obrotu);
  Cur_offd (X,	Y)	;
  transformacja_blok	(ADP,	ADK, L.x1 ,	L.y1 /*Px, Py*/, kos, koc,	Tobrot,0);
  block_angle += kat_obrotu;
  block_angle = Angle_Normal(block_angle);
  Cur_ond (X, Y) ;
}

static void	obrot_kopiuj_K(void)
/*---------------------------*/
{
	char sk[MaxTextLen];
	int	retval_no = 1;
	double	buf_ret[1];
	double	kos, koc;
	double kat_obrotu;

	sk[0] = '\0';
	if (!get_string(sk, "", MaxTextLen, 0, 16)) return;
	if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
	{
		return;
	}

	kat_obrotu = buf_ret[0] * Pi / 180;
	kos	= sin	(kat_obrotu - block_angle) ;
	koc	= cos	(kat_obrotu - block_angle) ;
	Cur_offd(X, Y);
	transformacja_blok(ADP, ADK, L.x1, L.y1 /*Px, Py*/, kos, koc, Tobrot, 0);
	block_angle = kat_obrotu;
	block_angle = Angle_Normal(block_angle);
	Cur_ond(X, Y);
}

static void obrot_kopiuj_paral(double kat)
{
	double	kos, koc;
	double kat_linii=0;
	unsigned typ;
	void  *adr;
	PLINIA PL;

	sel.akt = ASel;
	sel.gor = NGSel;

	typ = Blinia | Btekst;
	if ((adr = select_n(&typ, NULL, 75)) != NULL)
	{
		Cur_offd(X, Y);
		if (typ == Btekst) kat_linii = kat * Pi / 180 + ((TEXT*)adr)->kat;
		else
		{
			parametry_lini((LINIA*)adr, &PL);
			kat_linii = (kat + PL.kat) * Pi / 180;
		}
		Cur_ond(X, Y);
	}

    sel.akt = NASel;
	sel.gor = NGSel;


	kos = sin(kat_linii - block_angle);
	koc = cos(kat_linii - block_angle);
	Cur_offd(X, Y);
	transformacja_blok(ADP, ADK, Px, Py, kos, koc, Tobrot, 0);
	block_angle = kat_linii;
	block_angle = Angle_Normal(block_angle);
	Cur_ond(X, Y);

}


static void obrot_kopiuj_paralK(void)
{
	double	kos, koc;
	double kat_linii = 0;
	unsigned typ;
	double kat;
	void  *adr;
	PLINIA PL;
	char sk[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];

	sel.akt = ASel;
	sel.gor = NGSel;

	typ = Blinia | Btekst;
	if ((adr = select_n(&typ, NULL, 75)) != NULL)
	{
		if (get_string(sk, "", MaxTextLen, 0, 15))
		{
			if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
			{
				return;
			}
			kat = buf_ret[0];
			kat = kat * Pi / 180;
			Cur_offd(X, Y);
			if (typ == Btekst) kat = kat + ((TEXT*)adr)->kat;
			else
			{
				parametry_lini((LINIA*)adr, &PL);
				kat = kat + PL.kat * Pi / 180;
			}
			kat_linii = kat;
			Cur_ond(X, Y);
		}
	}

	sel.akt = NASel;
	sel.gor = NGSel;

	kos = sin(kat_linii - block_angle);
	koc = cos(kat_linii - block_angle);
	Cur_offd(X, Y);
	transformacja_blok(ADP, ADK, Px, Py, kos, koc, Tobrot, 0);
	block_angle = kat_linii;
	block_angle = Angle_Normal(block_angle);
	Cur_ond(X, Y);
}

static void	obrot_kopiuj_kat (double kat_obrotu)
/*--------------------------------------------*/
{
  double	kos, koc	;
  double kat_obrotu_r;

  kat_obrotu_r = kat_obrotu * Pi / 180;

  kos	= sin	(kat_obrotu_r - block_angle) ;
  koc	= cos	(kat_obrotu_r - block_angle) ;
  Cur_offd (X,	Y)	;
  transformacja_blok	(ADP,	ADK, Px,	Py, kos,	koc, Tobrot,0);
  block_angle = kat_obrotu_r;
  block_angle = Angle_Normal(block_angle);
  Cur_ond (X, Y) ;
}


static void	lustro_y_y_kopiuj	(void)
/*---------------------------*/
{

  Cur_offd (X,	Y)	;
  transformacja_blok	(ADP,	ADK, Px,	Py, Px, Py + 100,	Tmirror,0);
  Cur_ond (X, Y) ;
}

static void	lustro_x_x_kopiuj	(void)
/*---------------------------*/
{

  Cur_offd (X,	Y)	;
  transformacja_blok	(ADP,	ADK, Px,	Py, Px +	100, Py,	Tmirror,0);
  Cur_ond (X, Y) ;
}

/*------Kopiuj-----------------------------------------------------*/
void Add_Arm_Block(char *adp, char *adk, long *l_block_size, double DX, double DY)
{
  Add_Block (adp,adk, l_block_size, DX,DY,0,0, 0,0,0,0, Tprzesuw);
}

int kopiujk(void)
{
  double	X0,Y0;
  EVENT *ev;
  long l_block_size ;

  redcrpk(0);
  komunikat0(25);

  menupini (&mCopy_Divide, _DIVIDE_, _DIVIDE_C_, 50)	;

  while(1)
	{
	  strwyj	= 0;
	  ev=Get_Event_Point(NULL,	&X0, &Y0);
	  if(ev->What == evKeyDown	&&	 ev->Number==0)
	  {
		 redcrpk(1)	;
		 menupini (NULL, "",' ', 0) ;
		 return ESC	;
	  }
	  if ((ev->What == evKeyDown &&	ev->Number== ENTER) || strwyj)
		{ 
		    ////Cur_offd(X,Y);
			if(strwyj)
			{
			  DX = DX0;	DY	= DY0;
			}

			dane_size0 = dane_size;
	
			Add_Block (ADP,ADK, &l_block_size, DX,DY,0,0, 0,0,0,0, Tprzesuw);
			if (TTF_redraw) redraw();
			else CUR_OFF(X,Y);
			CUR_ON(X, Y);
		}
	  if(ev->What == evCommandP && ev->Number	==	0)
	  {
		 komunikat0	(-1) ; 	/*wspolrzedne wektora przemieszczenia*/
		 Divide_Block (L.x1,	L.y1)	;

		 menupini(&mCopy_Divide, _DIVIDE_, _DIVIDE_C_, 50);
	  }
	}
}


int kopiujkw(void)
{
  double	X0,Y0;
  EVENT *ev;
  long l_block_size ;

    redcrpk(0);
  komunikat0(25);

  menupini(&mBlok_Divide, _DIVIDE_, _DIVIDE_C_, 1);  //50

  while(1)
	{

	  strwyj	= 0;
	  ev=Get_Event_Point(NULL,	&X0, &Y0);
	  if(ev->What == evKeyDown	&&	 ev->Number==0)
	  {
		 redcrpk(1)	;
		 menupini (NULL, "", ' ', 0) ;
		 return ESC	;
	  }
	  if ((ev->What == evKeyDown &&	ev->Number== ENTER) || strwyj)
		{
			if(strwyj)
			{
				DX = DX0;	DY	= DY0;
			}

			dane_size0 = dane_size;
			Add_Block (ADP,ADK, &l_block_size, DX,DY,0,0, 0,0,0,0, Tprzesuw);

			if (TTF_redraw) redraw();
            CUR_OFF(X,Y);
            CUR_ON(X,Y);

		}
	  if(ev->What == evCommandP)
	  {
		  switch (ev->Number)
		  {
			case 0:
				skala_kopiuj();
				break;
			case 2:
				  lustro_y_y_kopiuj();
				  break;
			case 3:
				  lustro_x_x_kopiuj();
				  break;
			case 4:
				  komunikat0(-1);
				  Divide_Block(L.x1, L.y1);
				  break;
			case 5:
				  obrot_kopiuj();
				  break;
			case 6:
				  obrot_kopiuj_kat(45);
				  break;
			case 7:
				  obrot_kopiuj_kat(90);
				  break;
			case 8:
				  obrot_kopiuj_kat(135);
				  break;
			case 9:
				  obrot_kopiuj_kat(180);
				  break;
			case 10:
				  obrot_kopiuj_kat(225);
				  break;
			case 11:
				  obrot_kopiuj_kat(270);
				  break;
			case 12:
				  obrot_kopiuj_kat(315);
				  break;
			case 13:
				obrot_kopiuj_kat(0);
				break;
			case 14:
				obrot_kopiuj_K();
				break;
			case 15:
				  obrot_kopiuj_paral(0);
				  break;
			case 16:
				  obrot_kopiuj_paral(90);
				  break;
			case 17:
				  obrot_kopiuj_paralK();
				  break;
		  }
	  }
	}
}


static void	redcrK(char	typ)
{ static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static unsigned short cod = ' ';
  static	int sel_akt, sel_cur, sel_gor;
  static int iconno=0;
  switch(typ)
	{ case 0	:
		Cur_offd(X, Y);
		komunikat(0);
		pcx_outlines_view_on();
		komunikat0(23);
		sel_akt=sel.akt;	sel.akt=1;
		sel_cur = sel.cur;
		sel_gor = sel.gor;
		DX=DY=0;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod,&iconno);

		menupini(&mBlokm, _BLOCK_, _BLOCK_C_, 1);

		dane_size0 = dane_size;

		Cur_ond(X, Y);
	
		break;
	  case 1	: 
		pcx_outlines_view_off();
		SERV[def67]= (void*)SW[0];
		SERV[def68]= (void*)SW[1];
		menupini(menu,st,cod, iconno);
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		komunikat0(24);

		break;
	  case 2	:
	  case 3	:
		
		CUR_OFF=CUR_oFF;
		CUR_ON=CUR_oN;
		sel.akt=sel_akt;
		sel.cur = sel_cur;
		sel.gor = sel_gor;
		memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
		zmien_atrybut_undo(dane, dane	+ dane_size);

        blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
		zmien_atrybut(ADP,ADK,Ablok,Anormalny/*Aoblok*/);
		if	(dane_size>dane_size0) zmien_atrybut(dane,dane + dane_size - dane_size0	- 1,Anormalny,Aoblok);

        CUR_OFF(X,Y);
		CUR_ON(X,Y);
		komunikat (0) ;
		komunikat0 (0)	;
		break;
	  case 4	:
		sel_akt=sel.akt;	sel.akt=1; 
		DX=DY=0;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		break;
	  case 5	:

		memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
		zmien_atrybut_undo(dane, dane	+ dane_size);

        blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
		zmien_atrybut(ADP,ADK,Ablok,Anormalny/*Aoblok*/);
		
		if	(dane_size>dane_size0) zmien_atrybut(dane,dane + dane_size - dane_size0	- 1,Anormalny,Aoblok);

        CUR_OFF=CUR_oFF;
        CUR_ON=CUR_oN;
		
		komunikat (0) ;
		komunikat0 (0)	;
		break;
      case 7	:
        CUR_OFF=CUR_oFF;
		CUR_ON=CUR_oN;
		sel.akt=sel_akt;
		sel.cur = sel_cur;
		sel.gor = sel_gor;
		komunikat (0) ;
		komunikat0 (0)	;
		break;
		break;
	  default :	break;
	}
}

void Kopiuj(void)
{
  TTF_redraw = FALSE;
  redcrK(0);
  blok(dzi,odzi,Redraw_Block,COMNDmb);
  redcrK(1);
  if (przesunp()==ESC)
	{ redcrK(2);
	  return;
	}
  L.x1=L.x2=X;
  L.y1=L.y2=Y;
  Px=X;Py=Y;
  outlineor(&L,XOR_PUT,0);
  kopiujk();
  redcrK(3);

  return;
}

int Kopiuj_Ctrl_R(void)
{ static	int ( *SW[2])();
  SW [0] = SERV [58] ;  SERV [58]	= nooop1 ;
  redcrK(0);
  if (blok_Ctrl_R(dzi,odzi,Redraw_Block,COMNDmb))
   {
     redcrK(1);

     L.x1=L.x2=X;
     L.y1=L.y2=Y;
     Px=X;Py=Y;
     outlineor(&L,XOR_PUT,0);
     kopiujk();
     redcrK(3);
     SERV [58] = (void*)SW [0];
     return 0;
    }
     else
     {
      redcrK(1);
      redcrK(7);

      SERV [58] = (void*)SW [0];
      return 0;
     }
}

void KopiujM(void)
{
  redcrK(4);

  ostatni_blok	();

  L.x1=L.x2=X;
  L.y1=L.y2=Y;
  Px=X;Py=Y;
  outlineor(&L,XOR_PUT,0);
  kopiujkw();
  redcrK(5);
  return;
}

/*---------Przesun--------------------------------------------------*/

static int przesunk(void)
{
  double	X0,Y0;
  EVENT *ev;

  redcrpk(0);
  komunikat0(5);
  strwyj	= 0;
  while(1)
	{
	  ev=Get_Event_Point(NULL,	&X0, &Y0);
	  if(ev->What == evKeyDown	&&	 ev->Number==0) {	redcrpk(1);	return ESC;	}
	  if ((ev->What == evKeyDown &&	ev->Number== ENTER) || strwyj)
		{ Cur_offd(X,Y);

		blokzap(ADP,ADK,Ablok,COPY_PUT,0);

		if(strwyj)
		{
		  DX = DX0;	DY	= DY0;
		}
		transformacja_blok(ADP,ADK,DX,DY,0,0,Tprzesuw,0);

		blokzap(ADP,ADK,Ablok,COPY_PUT,1);

		redcrpk(2);
		return ENTER;
		}
	}
}

static int przesunkZ(void)
{
  double	X0,Y0;
  EVENT *ev;

  TTF_redraw = FALSE;
  redcrpkZ(0);
  komunikat0(5);
  strwyj	= 0;
  while(1)
	{ ev=Get_Event_Point(NULL,	&X0, &Y0);
	  if(ev->What == evKeyDown	&&	 ev->Number==0) {	redcrpkZ(1);	return ESC;	}
	  if ((ev->What == evKeyDown &&	ev->Number== ENTER) || strwyj)
		{ Cur_offd(X,Y);
	blokzap(ADP,ADK,Ablok,COPY_PUT,0);
	if(strwyj)
	{
	  DX = DX0;	DY	= DY0;
	}
	transformacja_blok(ADP,ADK,DX,DY,0,0,TprzesuwZ,0);
	blokzap(ADP,ADK,Ablok,COPY_PUT,1);
	redcrpkZ(2);
	if (TTF_redraw) redraw();
	return ENTER;
		}
	}
}


void winf_addf(void)
{
    if (SERV[def67]==winf) winf_draw();
    if (SERV[def68]==addf) addf_draw();
}

static void	redcrP(char	typ)
{ static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static unsigned short cod=L' ';
  static	int sel_akt, sel_cur, sel_gor;
  static    int iconno;
  switch(typ)
	{ case 0	:
     case 10:
     case 13:
     case 16:
		Cur_offd(X, Y);
		
		komunikat(0);
		pcx_outlines_view_on();
      if (typ==0) komunikat0(22); else if (typ==10) komunikat0(131); else if (typ==13) komunikat0(132);
         else komunikat0(133);
		sel_akt=sel.akt;	sel.akt=1;
		sel_cur = sel.cur;
		sel_gor = sel.gor;
		DX=DY=0;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod,&iconno);

		if (typ==0) menupini(&mBlokm, _BLOCK_, _BLOCK_C_, 1); else menupini(&mBlokmc,"Block",'B', 1);

		Cur_ond(X, Y);
		
		break;
	  case 1: 
		  Cur_offd(X, Y);
     case 11:
     case 14:
     case 17:
		SERV[def67]= (void*)SW[0];
		SERV[def68]= (void*)SW[1];
      ADP_0=ADP;
      ADK_0=ADK;
		menupini(menu,st,cod, iconno);
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		if (typ==1) komunikat0(4); else komunikat0(0);
		Cur_ond(X, Y);
		break;
	  case 2	:
     case 12:
              ADP=ADP_0;
              ADK=ADK_0;
	           //blokzap(ADP,ADK,Ablok,XOR_PUT,1);
               blokzap(ADP,ADK,Ablok,COPY_PUT,1);
               CUR_OFF(X,Y);
               CUR_ON(X,Y);
			break;
      case 15:
      case 18:
			break;
	  case 3	: break;
	  case 4	: break;
	  default: break;
	}
  if(typ==2	||	typ==12 || typ==15 || typ==18 || typ==3 || typ==4)
	{ 
	  ADP=ADP_0;
     ADK=ADK_0;
	  CUR_OFF=CUR_oFF;
	  CUR_ON=CUR_oN;
	  sel.akt=sel_akt;
	  sel.cur = sel_cur;
	  sel.gor = sel_gor;

      pcx_outlines_view_off();
	  
	  if (typ==4) 
		 {	
			zmien_atrybut_undo(dane, dane	+ dane_size);
         zmien_atrybut(ADP,ADK,Ablok,Aoblok);
		 
         Cur_ond(X,Y);
         komunikat (0) ;
	      komunikat0 (0) ;
		 }	
		 else	if (typ==15)
         {
		     zmien_atrybut(dane,dane+dane_size,Ablok,Anormalny);
           Cur_ond(X,Y);
	        komunikat0 (0) ;
         }
        else	if (typ==18)
         {
           zmien_atrybut_undo(dane, dane	+ dane_size);
		     zmien_atrybut(dane,dane+dane_size,Ablok,Aoblok);
           Cur_ond(X,Y);
	        komunikat0 (0) ;
         }
         else
           {
             zmien_atrybut(ADP,ADK,Ablok,Aoblok);
             Cur_ond(X,Y);
	          komunikat (0) ;
	          komunikat0 (0) ;
           }

	}
}


void Przesun(void)
{
  TTF_redraw = FALSE;
  redcrP(0);
  blok(dzi,odzi,Redraw_Block,COMNDmb);
  redcrP(1);
  if (przesunp()==ESC)
	{ 
	  redcrP(2);
	  return;
	}
  L.x1=L.x2=X;
  L.y1=L.y2=Y;
  Px=X;Py=Y;
  Cur_offd(X, Y);
    outlineor(&L,COPY_PUT,0);
  Cur_ond(X, Y);
  if(przesunk()==ESC) 
	  redcrP(3);
  else 
	{
	 redcrP(4);
	} 
  if (TTF_redraw)  redraw();
  else flip_screen();
  return;
}


void redcrsb(char typ, int n)
{ static	void (*CUR_oN)(double ,double);
    static	void (*CUR_oFF)(double ,double);
    static	int (	*SW[2])();
    static	TMENU	*menu;
    static	char st[POLE_TXT_MAX];
    static unsigned short cod=L' ';
    static	int sel_akt, sel_cur, sel_gor;
    static    int iconno;
    switch(typ)
    { case 0	:
            Cur_offd(X, Y);

            komunikat(0);
            komunikat0(n);
            sel_akt=sel.akt;	sel.akt=1;
            sel_cur = sel.cur;
            sel_gor = sel.gor;
            DX=DY=0;
            CUR_oFF=CUR_OFF;
            CUR_oN=CUR_ON;
            SW[0]=SERV[def67];  SERV[def67]=winf;
            SW[1]=SERV[def68];  SERV[def68]=addf;
            *(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
            winf_draw ();
            addf_draw ();
            getmenupini(&menu,st,&cod,&iconno);

            menupini(&mBlokm, _BLOCK_, _BLOCK_C_, 1);

            Cur_ond(X, Y);

            break;
        case 1:
            Cur_offd(X, Y);
            SERV[def67]= (void*)SW[0];
            SERV[def68]= (void*)SW[1];
            ADP_0=ADP;
            ADK_0=ADK;
            menupini(menu,st,cod, iconno);
            setfillstyle_(SOLID_FILL, BKCOLOR);
            bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
            komunikat0(0);
            Cur_ond(X, Y);
            break;
        case 2:
            ADP=ADP_0;
            ADK=ADK_0;
            CUR_OFF=CUR_oFF;
            CUR_ON=CUR_oN;
            sel.akt=sel_akt;
            sel.cur = sel_cur;
            sel.gor = sel_gor;
            Cur_ond(X,Y);
            komunikat (0) ;
            komunikat0 (0) ;

            CUR_OFF(X,Y);
            CUR_ON(X,Y);
            break;
        default:
            break;
    }
}

void select_blok(void)
{
    blok(dzi,odzi,Redraw_Block,COMNDmb);
}

void PrzesunZ(void)
{
  redcrP(0);
  blok(dzi,odzi,Redraw_Block,COMNDmb);
  redcrP(1);
  if (przesunpZ()==ESC)
	{ redcrP(2);
	  return;
	}
  L.x1=L.x2=X;
  L.y1=L.y2=Y;
  Px=X;Py=Y;
  outlineor(&L,XOR_PUT,0);
  if(przesunkZ()==ESC) redcrP(3);
  else 
	{
	 redcrP(4);
	} 
  return;
}


/*---------Usun - normalny--------------------------------------------------*/

int	delete_block (void)
//----------------------------
{ deleted_pcx=FALSE;
  deleted_on_front = FALSE;
  deleted_TTF = FALSE;
  deleted_pattern = FALSE;
  blokzap (ADP, ADK,	Ablok, COPY_PUT, 0) ;
  zmien_atrybut (ADP, ADK,	Ablok, Ausuniety)	;
  ADP=ADK=NULL;
  Cur_offd (X,	Y)	;
  Cur_ond (X, Y) ;
  //sprawdzenie czy wsrod usunietych obiektow sa bitmapy
  if ((deleted_pcx==TRUE) || (deleted_TTF==TRUE) || (deleted_pattern == TRUE) || (deleted_on_front == TRUE))
    {
	  if (deleted_pcx == TRUE) regen_ctx=TRUE;
      redraw();
    }
  return	0 ;
}

int	delete_block1 (void)
//----------------------------
{
  Cur_offd (X,	Y)	;
  blokzap (ADP, ADK,	Ablok, COPY_PUT, 0) ;
  zmien_atrybut (ADP, ADK,	Ablok, Ausuniety)	;
  ADP=ADK=NULL;
  Cur_ond (X, Y) ;
  return	0 ;
}

static int change_mode (void)
//----------------------------
{

  Change_Delete_Mode();

  return	1 ;
}

static int local_undo (void)
//----------------------------
{
  undo_blok	()	;
  return	0 ;
}


static int (*COMNDmb_dn	[])(void) =
{ ostatni_blok,wszystko,wszystkie_warstwy,winf,addf, nooop1, delete_block,	local_undo,	change_mode,
  auto_on, auto_off } ;


static void	redcr_del_nor (char typ)
//----------------------------------
{
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static unsigned short cod = L' ';
  static	int (	*SW[3])() ;
  static int iconno=0;
  static	int sel_akt, sel_gor, sel_cur, sel_nr;

  switch(typ)
  {
	 case	0 :
		Cur_offd(X, Y);

		sel_akt = sel.akt;
		sel_cur = sel.cur;
		sel_gor = sel.gor; sel.gor = 0;
		sel_nr = sel.nr;
		layer_info();

		komunikat0 (21) ;
		SW	[0] =	SERV [def67] ;	 SERV	[def67] = winf	;
		SW	[1] =	SERV [def68] ;	 SERV	[def68] = addf	;
		SW	[2] =	SERV [83];		 SERV	[83] = delete_block ;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw  () ;
		addf_draw  () ;
		getmenupini	(&menu, st,	&cod, &iconno)	;

		menupini	(&mBlock_dn, _BLOCK_, _BLOCK_C_, 1) ;

		Cur_ond(X, Y);
		break	;
	  case 1	:
		menupini	(menu, st, cod, iconno) ;
		setfillstyle_(SOLID_FILL, BKCOLOR) ;
		bar(maxX	/ 2 +	1,	0,	maxX/2+100,	11	/*9*/) ;
		SERV [def67] = (void*)SW	[0] ;
		SERV [def68] = (void*)SW	[1] ;
		SERV [83] = (void*)SW	[2] ;
		memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
		zmien_atrybut_undo (dane, dane +	dane_size) ;
        blokzap (ADP, ADK, Ablok, COPY_PUT, 1) ;
		zmien_atrybut (ADP, ADK, Ablok, Aoblok) ;
        Cur_offd(X, Y);
		sel.akt = sel_akt;
		sel.cur = sel_cur;
		sel.gor = sel_gor;
		sel.nr = sel_nr;
		Cur_ond (X,	Y)	;
		break	;
	  default: break;
  }
}

static int delete_normal (void)
//------------------------------
{
  int	i_ret	;

  redcr_del_nor (0) ;
  i_ret = Block_Proc	(dzi,	odzi,	Redraw_Block, COMNDmb_dn) ;
  if (i_ret	==	TRUE)
  {
	 redcr_del_nor	(1) ;
  }
  else
  {
	 redcr_del_nor	(1) ;
  }
  return	i_ret	;
}


static /*const*/ int (*COMNDwez_w	[])(void) =
{
        winf, nooop1, auto_on,
        auto_off
} ;

static /*const*/ int (*COMNDwez_info	[])(void) =
{
        entire_polyline, single_object
} ;

/*---------Usun - szybki--------------------------------------------------*/

static int (*COMNDmb_dq	[])(void) =
{
  winf, nooop1, local_undo, change_mode,
  auto_on, auto_off
} ;

int dziWez_info(void	*ad)	 /*info*/
{
  okno_r();
  rysuj_obiekt_no_pcx(ad,COPY_PUT,1); //0
  delay(50);
  rysuj_obiekt_no_pcx(ad,COPY_PUT,1);
  okno_all();
  Info_about_object(ad);
  ADP=ADK=NULL;
  return	1;
}

int dziWez_object(void	*ad)	 /*object*/
{
    okno_r();
    rysuj_obiekt_no_pcx(ad,COPY_PUT,1); //0
    delay(50);
    rysuj_obiekt_no_pcx(ad,COPY_PUT,1);
    okno_all();
    ADP=ADK=NULL;
    adr_object=ad;
    return	1;
}

int dziWez_sheet(void	*ad)	 /*sheet*/
{ BLOK *ptrs_block;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  BOOL found_sheet;
  BOOL find_next;
  char *ad1;
  char sk[60];

  found_sheet=FALSE;
  find_next=TRUE;
  ad1=ad;
  adr_sheet=NULL;

  while (find_next==TRUE)
  {
   if (NULL != (ptrs_block = (BLOK  *)FIRSTB (ad1-1)))
    {
     ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
     if (ptrs_desc_bl->sz_type [0] != '\0')
       {
         strcpy(sk,ptrs_desc_bl->sz_type);
         if (strncmp(sk,"@SHEET",6)==0)
          {
           found_sheet=TRUE;
           find_next=FALSE;
           ad1=(char*)ptrs_block;
          }
         else ad1=(char*)ptrs_block;
       }
        else ad1=(char*)ptrs_block;
    }
     else find_next=FALSE;
  }

  if (found_sheet==TRUE)
   {
    blokzap_no_pcx(ad1, ad1 + sizeof(NAGLOWEK) + ptrs_block->n -1, ANieOkreslony, COPY_PUT, 0);
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
    delay(50);
    blokzap_no_pcx(ad1, ad1 + sizeof(NAGLOWEK) + ptrs_block->n -1, ANieOkreslony, COPY_PUT, 1);
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
    adr_sheet=ad1;
    return	1;
   }
    else  return 0;
}


int dziWez_w(void	*ad)	/*warstwa*/
{
  rysuj_obiekt(ad,COPY_PUT,0);
  delay(50);
  rysuj_obiekt(ad,COPY_PUT,1);
  Current_Layer=((LINIA*)ad)->warstwa;
  Ini_Global_Object ();
  
  ADP=ADK=NULL;
  layer_info ();
  return	1;
}

int dziWez_hw(void	*ad)	/*ukryj warstwe*/
{ int Selected_Layer;

 rysuj_obiekt(ad,COPY_PUT,0);

 rysuj_obiekt(ad,COPY_PUT,1);
  Selected_Layer=((LINIA*)ad)->warstwa;
  if (Current_Layer==Selected_Layer)
  {
    if (error_layer==0) 
	{
		ErrList(37);
		error_layer=1;
	}
  }
  else
  {
    Change = TRUE;
    Layers[Selected_Layer].on=0;
	force_redraw=TRUE;
  }

  return	1;
}

int dziWez_k(void	*ad)	 /*kolor*/
{
  rysuj_obiekt(ad,COPY_PUT,0);
  delay(50);
  rysuj_obiekt(ad,COPY_PUT,1);
  if (((NAGLOWEK*)ad)->obiekt==Olinia ||
		((NAGLOWEK*)ad)->obiekt==Otekst ||
		((NAGLOWEK*)ad)->obiekt==Oluk	||
		((NAGLOWEK*)ad)->obiekt==Ookrag ||
		((NAGLOWEK*)ad)->obiekt==Okolo ||
        ((NAGLOWEK*)ad)->obiekt==Oellipse ||
        ((NAGLOWEK*)ad)->obiekt==Ofilledellipse ||
        ((NAGLOWEK*)ad)->obiekt==Oellipticalarc ||
		((NAGLOWEK*)ad)->obiekt==Owwielokat	||
	    ((NAGLOWEK*)ad)->obiekt == Ospline ||
        ((NAGLOWEK*)ad)->obiekt == Osolidarc ||
		((NAGLOWEK*)ad)->obiekt==Opoint)
	{	 
	 Layers[Current_Layer].color=((LINIA*)ad)->kolor;
	 Ini_Global_Object ();
	} 
	 ADP=ADK=NULL;
  return	1;
}

int dziWez_t(void	*ad)	 /*typ i grubosc linii, parametry tekstu */
{
  rysuj_obiekt(ad,COPY_PUT,0);
  delay(50);
  rysuj_obiekt(ad,COPY_PUT,1);
  if (((NAGLOWEK*)ad)->obiekt==Olinia ||
		((NAGLOWEK*)ad)->obiekt==Oluk	||
		((NAGLOWEK*)ad)->obiekt==Ookrag ||
        ((NAGLOWEK*)ad)->obiekt==Oellipse ||
        ((NAGLOWEK*)ad)->obiekt==Oellipticalarc)
	{	
	 Layers[Current_Layer].line_type=((LINIA*)ad)->typ;
	 Ini_Global_Object ();
	 ADP=ADK=NULL;
	 linia_tekst =	0;
	}
	 else
	  if (((NAGLOWEK*)ad)->obiekt==Otekst)
		{
		 TextG.czcionka=((TEXT*)ad)->czcionka;
		 TextG.bold=((TEXT*)ad)->bold;
		 TextG.wysokosc=((TEXT*)ad)->wysokosc;
		 TextG.width_factor=((TEXT*)ad)->width_factor;
		 TextG.italics=((TEXT*)ad)->italics;
		 TextG.typ=((TEXT*)ad)->typ;
		 TextG.justowanie=((TEXT*)ad)->justowanie;
		 TextG.ukryty=((TEXT*)ad)->ukryty;
		 linia_tekst =	1;
		}
	ADP=ADK=NULL;
	return 1;
}


int dziWez_s_pattern(void* ad)	 /*get solid pattern if filled with image*/
{
	WIELOKAT* w;
    SOLIDARC* sa;
	char* scale_ptr;
	char* dx_ptr;
	char* angle_ptr;
	char* dy_ptr;
	char* name_ptr;
	char* translucency_ptr;
	int transluc;
	double d_trans = 12.75;
	char sk[10];


	if (((NAGLOWEK*)ad)->obiekt == Owwielokat)
	{
		w = (WIELOKAT*)ad;
		
		rysuj_obiekt(ad, COPY_PUT, 0);
        CUR_OFF(X,Y);
        CUR_ON(X,Y);
		delay(50);
		rysuj_obiekt(ad, COPY_PUT, 1);
        CUR_OFF(X,Y);
        CUR_ON(X,Y);

		if ((w->empty_typ == 0) && (w->pattern == 1))
		{

			scale_ptr = w->xy;
			scale_ptr += (w->lp * sizeof(float));
			dx_ptr = scale_ptr;
			dx_ptr += sizeof(short int);
			angle_ptr = dx_ptr;
			angle_ptr += sizeof(short int);
			dy_ptr = angle_ptr;
			dy_ptr += sizeof(short int);
			name_ptr = dy_ptr;
			name_ptr += sizeof(short int);
			memmove(&SolidPatternScale, scale_ptr, sizeof(short int));
			memmove(&SolidPatternDx, dx_ptr, sizeof(short int));
			memmove(&SolidPatternAngle, angle_ptr, sizeof(short int));
			memmove(&SolidPatternDy, dy_ptr, sizeof(short int));
			memmove(&SolidPatternName, name_ptr, strlen(name_ptr) + 1);

			SolidPattern = 1;
			set_solid_pattern_name();
			SolidG.empty_typ = 0;
			SolidG.pattern = 1;
			SolidFillTyp = 0;

		}
		else if ((w->empty_typ == 0) && (w->pattern == 0))
		{
			SolidPattern = 0;
			SolidTranslucent = w->translucent;
			SolidG.empty_typ = w->empty_typ;
			SolidFillTyp = w->empty_typ;
			SolidG.kolor = w->kolor;

			if (w->translucent == 1)
			{
				translucency_ptr = w->xy;
				translucency_ptr += (w->lp * sizeof(float));
				memmove(&SolidTranslucency, translucency_ptr, sizeof(unsigned char));

			}
			else SolidTranslucency = 255;

			transluc = (int)((SolidTranslucency / d_trans) + 0.5);
			sprintf(sk, "%d%%", transluc * 5);

			set_solid_pattern_trans(sk);

		}
		else
		{
			SolidPattern = 0;
			SolidTranslucent = 0;
			SolidTranslucency = 255;
			SolidG.empty_typ = w->empty_typ;
			SolidFillTyp = w->empty_typ;
			SolidG.kolor = w->kolor;
			
			transluc = (int)((SolidTranslucency / d_trans) +0.5);
			sprintf(sk, "%d%%", transluc * 5);

			set_solid_pattern_trans(sk);
		}
	}
    else if (((NAGLOWEK*)ad)->obiekt == Osolidarc)
    {
        sa = (SOLIDARC*)ad;

        rysuj_obiekt(ad, COPY_PUT, 0);
        CUR_OFF(X,Y);
        CUR_ON(X,Y);
        delay(50);
        rysuj_obiekt(ad, COPY_PUT, 1);
        CUR_OFF(X,Y);
        CUR_ON(X,Y);

        if ((sa->empty_typ == 0) && (sa->pattern == 1))
        {

            SolidPatternScale=sa->scale;
            SolidPatternDx=sa->dx;
            SolidPatternAngle=sa->dy;
            strcpy(&SolidPatternName,sa->patternname);

            SolidPattern = 1;
            set_solid_pattern_name();
            SolidG.empty_typ = 0;
            SolidG.pattern = 1;
            SolidFillTyp = 0;

        }
        else if ((sa->empty_typ == 0) && (sa->pattern == 0))
        {
            SolidPattern = 0;
            SolidTranslucent = sa->translucent;
            SolidG.empty_typ = sa->empty_typ;
            SolidFillTyp = sa->empty_typ;
            SolidG.kolor = sa->kolor;

            if (sa->translucent == 1) SolidTranslucency=sa->translucency;
            else SolidTranslucency = 255;

            transluc = (int)((SolidTranslucency / d_trans) + 0.5);
            sprintf(sk, "%d%%", transluc * 5);

            set_solid_pattern_trans(sk);

        }
        else
        {
            SolidPattern = 0;
            SolidTranslucent = 0;
            SolidTranslucency = 255;
            SolidG.empty_typ = sa->empty_typ;
            SolidFillTyp = sa->empty_typ;
            SolidG.kolor = sa->kolor;

            transluc = (int)((SolidTranslucency / d_trans) +0.5);
            sprintf(sk, "%d%%", transluc * 5);

            set_solid_pattern_trans(sk);
        }
    }
	ADP = ADK = NULL;
	return 1;
}

int dziSet_s_pattern(void* ad)	 /*set solid pattern if filled with image*/
{
	WIELOKAT* w0;
    SOLIDARC* sa0;
	char* scale_ptr;
	char* dx_ptr;
	char* angle_ptr;
	char* dy_ptr;
	char* name_ptr;
	WIELOKAT w;
    SOLIDARC sa;
	WIELOKAT* w_change;
    SOLIDARC* sa_change;
	char* translucency_ptr;

	if (((NAGLOWEK*)ad)->obiekt == Owwielokat)
	{
		
		w0 = (WIELOKAT*)ad;
		memcpy(&w, w0, sizeof(WIELOKAT));

		if (w.obiektt3 != 1) //is not hatching
		{
			rysuj_obiekt(ad, COPY_PUT, 0);

			if (SolidPattern == 1)
			{
				w.empty_typ = 0;
				w.pattern = 1;

				scale_ptr = w.xy;
				scale_ptr += (w.lp * sizeof(float));
				dx_ptr = scale_ptr;
				dx_ptr += sizeof(short int);
				angle_ptr = dx_ptr;
				angle_ptr += sizeof(short int);
				dy_ptr = angle_ptr;
				dy_ptr += sizeof(short int);
				name_ptr = dy_ptr;
				name_ptr += sizeof(short int);

				memmove(scale_ptr, &SolidPatternScale, sizeof(short int));
				memmove(dx_ptr, &SolidPatternDx, sizeof(short int));
				memmove(angle_ptr, &SolidPatternAngle, sizeof(short int));
				memmove(dy_ptr, &SolidPatternDy, sizeof(short int));
				memmove(name_ptr, &SolidPatternName, strlen(SolidPatternName) + 1);

				w.n = 8 + w.lp * sizeof(float) + 4 * sizeof(short int) + (int)strlen(SolidPatternName) + 1;
			}
			else
			{
				w.empty_typ = SolidFillTyp;
				w.pattern = 0;
				w.kolor = SolidG.kolor;
				w.empty_typ = SolidFillTyp;
				w.pattern = 0;
				w.kolor = SolidG.kolor;

				w.translucent = SolidTranslucent;
				if (w.translucent == 0)
					w.n = 8 + w.lp * sizeof(float);
				else
				{
					translucency_ptr = w.xy;
					translucency_ptr += (w.lp * sizeof(float));
					memmove(translucency_ptr, &SolidTranslucency, sizeof(unsigned char));

					w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);
				}
			}
				
			if ((w_change = korekta_obiekt((void*)w0, (void*)&w)) == NULL)
				ErrList(15);

			rysuj_obiekt((void*)w_change, COPY_PUT, 1);

		}
	}
    else if (((NAGLOWEK*)ad)->obiekt == Osolidarc)
    {

        sa0 = (SOLIDARC*)ad;
        memcpy(&sa, sa0, sizeof(SOLIDARC));
        
        rysuj_obiekt(ad, COPY_PUT, 0);

        if (SolidPattern == 1)
        {
            sa.empty_typ = 0;
            sa.pattern = 1;
            
            sa.scale=SolidPatternScale;
            sa.dx=SolidPatternDx;
            sa.angle=SolidPatternAngle;
            sa.dy=SolidPatternDy;
            strcpy(&sa.patternname, SolidPatternName);

            sa.n = SOLIDARC_N + (int)strlen(SolidPatternName) ;
        }
        else
        {
            sa.empty_typ = SolidFillTyp;
            sa.pattern = 0;
            sa.kolor = SolidG.kolor;
            sa.empty_typ = SolidFillTyp;
            sa.pattern = 0;
            sa.kolor = SolidG.kolor;

            sa.translucent = SolidTranslucent;
            if (sa.translucent == 0) sa.translucency=255;
            else sa.translucency=SolidTranslucency;
        }

        if ((sa_change = korekta_obiekt((void*)sa0, (void*)&sa)) == NULL)
            ErrList(15);

        rysuj_obiekt((void*)sa_change, COPY_PUT, 1);
            
    }
	ADP = ADK = NULL;
	return 1;
}

int dziWez_t_pattern(void* ad)	 /*get trace pattern if filled with image*/
{
	WIELOKAT* w;
    SOLIDARC* sa;
	char* scale_ptr;
	char* dx_ptr;
	char* angle_ptr;
	char* dy_ptr;
	char* name_ptr;
	char* translucency_ptr;
	int transluc;
	int d_trans = 12;
	char sk[10];


	if (((NAGLOWEK*)ad)->obiekt == Owwielokat)
	{
		w = (WIELOKAT*)ad;

		rysuj_obiekt(ad, COPY_PUT, 0);
		delay(50);
		rysuj_obiekt(ad, COPY_PUT, 1);

		if ((w->empty_typ == 0) && (w->pattern == 1))
		{

			scale_ptr = w->xy;
			scale_ptr += (w->lp * sizeof(float));
			dx_ptr = scale_ptr;
			dx_ptr += sizeof(short int);
			angle_ptr = dx_ptr;
			angle_ptr += sizeof(short int);
			dy_ptr = angle_ptr;
			dy_ptr += sizeof(short int);
			name_ptr = dy_ptr;
			name_ptr += sizeof(short int);
			memmove(&TracePatternScale, scale_ptr, sizeof(short int));
			memmove(&TracePatternDx, dx_ptr, sizeof(short int));
			memmove(&TracePatternAngle, angle_ptr, sizeof(short int));
			memmove(&TracePatternDy, dy_ptr, sizeof(short int));
			memmove(&TracePatternName, name_ptr, strlen(name_ptr) + 1);

			TracePattern = 1;
			set_trace_pattern_name();
		}
		else if ((w->empty_typ == 0) && (w->pattern == 0))
		{
			TracePattern = 0;
			TraceTranslucent = w->translucent;
			
			if (w->translucent == 1)
			{
				translucency_ptr = w->xy;
				translucency_ptr += (w->lp * sizeof(float));
				memmove(&TraceTranslucency, translucency_ptr, sizeof(unsigned char));

			}
			else TraceTranslucency = 255;

			transluc = (TraceTranslucency / d_trans) - 1;
			sprintf(sk, "%d%%", transluc * 5);

			set_trace_pattern_trans(sk);

		}
		else
		{
			TracePattern = 0;
			TraceTranslucent = 0;
			TraceTranslucency = 255;
			
			transluc = (TraceTranslucency / d_trans) - 1;
			sprintf(sk, "%d%%", transluc * 5);

			set_trace_pattern_trans(sk);
		}
	}
    else if (((NAGLOWEK*)ad)->obiekt == Osolidarc)
    {
        sa = (SOLIDARC*)ad;

        rysuj_obiekt(ad, COPY_PUT, 0);
        delay(50);
        rysuj_obiekt(ad, COPY_PUT, 1);

        if ((sa->empty_typ == 0) && (sa->pattern == 1))
        {
            TracePatternScale=sa->scale;
            TracePatternDx=sa->dx;
            TracePatternAngle=sa->dy;
            strcpy(&TracePatternName,sa->patternname);

            TracePattern = 1;
            set_trace_pattern_name();
        }
        else if ((sa->empty_typ == 0) && (sa->pattern == 0))
        {
            TracePattern = 0;
            TraceTranslucent = sa->translucent;

            if (w->translucent == 1) TraceTranslucency=sa->translucency;
            else TraceTranslucency = 255;

            transluc = (TraceTranslucency / d_trans) - 1;
            sprintf(sk, "%d%%", transluc * 5);

            set_trace_pattern_trans(sk);

        }
        else
        {
            TracePattern = 0;
            TraceTranslucent = 0;
            TraceTranslucency = 255;

            transluc = (TraceTranslucency / d_trans) - 1;
            sprintf(sk, "%d%%", transluc * 5);

            set_trace_pattern_trans(sk);
        }
    }
	ADP = ADK = NULL;
	return 1;
}

int dziSet_t_pattern(void* ad)	 /*set trace pattern if filled with image*/
{
	WIELOKAT* w0;
    SOLIDARC* sa0;
	char* scale_ptr;
	char* dx_ptr;
	char* angle_ptr;
	char* dy_ptr;
	char* name_ptr;
	WIELOKAT w;
    SOLIDARC sa;
	WIELOKAT* w_change;
    SOLIDARC* sa_change;
	char* translucency_ptr;

	if (((NAGLOWEK*)ad)->obiekt == Owwielokat)
	{
		w0 = (WIELOKAT*)ad;
		memcpy(&w, w0, sizeof(WIELOKAT));

		if (w.obiektt3 != 1) //is not hatching
		{
			rysuj_obiekt(ad, COPY_PUT, 0);

			if (TracePattern == 1)
			{
				w.empty_typ = 0;
				w.pattern = 1;
				scale_ptr = w.xy;
				scale_ptr += (w.lp * sizeof(float));
				dx_ptr = scale_ptr;
				dx_ptr += sizeof(short int);
				angle_ptr = dx_ptr;
				angle_ptr += sizeof(short int);
				dy_ptr = angle_ptr;
				dy_ptr += sizeof(short int);
				name_ptr = dy_ptr;
				name_ptr += sizeof(short int);
				memmove(scale_ptr, &TracePatternScale, sizeof(short int));
				memmove(dx_ptr, &TracePatternDx, sizeof(short int));
				memmove(angle_ptr, &TracePatternAngle, sizeof(short int));
				memmove(dy_ptr, &TracePatternDy, sizeof(short int));
				memmove(name_ptr, &TracePatternName, strlen(TracePatternName) + 1);

				w.n = 8 + w.lp * sizeof(float) + 4 * sizeof(short int) + (int)strlen(TracePatternName) + 1;
			}
			else
			{
				w.empty_typ = SolidFillTyp; 
				w.pattern = 0;
				w.kolor = SolidG.kolor;

				w.translucent = TraceTranslucent;
				if (w.translucent == 0)
					w.n = 8 + w.lp * sizeof(float);
				else
				{
					translucency_ptr = w.xy;
					translucency_ptr += (w.lp * sizeof(float));
					memmove(translucency_ptr, &TraceTranslucency, sizeof(unsigned char));

					w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);
				}
			}

			if ((w_change = korekta_obiekt((void*)w0, (void*)&w)) == NULL)
				ErrList(15);

			rysuj_obiekt((void*)w_change, COPY_PUT, 1);

		}
	}
    else if (((NAGLOWEK*)ad)->obiekt == Osolidarc)
    {
        sa0 = (SOLIDARC *) ad;
        memcpy(&sa, sa0, sizeof(SOLIDARC));

        rysuj_obiekt(ad, COPY_PUT, 0);

            if (TracePattern == 1) {
                sa.empty_typ = SolidFillTyp;
                sa.pattern = TracePattern;

                if (sa.pattern == 1)
                {
                    sa.scale=TracePatternScale; //sizeof(short int)
                    sa.dx=TracePatternDx;  //sizeof(short int)
                    sa.dy=TracePatternDy;  //sizeof(short int)
                    sa.angle=TracePatternAngle;  //sizeof(short int)
                    memmove(sa.patternname, &TracePatternName, strlen(TracePatternName) + 1);
                    sa.n = SOLIDARC_N + (int)strlen(TracePatternName) ;
                }
                else
                {
                    sa.translucent = TraceTranslucent;
                    sa.translucency = TraceTranslucency;

                    sa.n = SOLIDARC_N;
                }

            if ((sa_change = korekta_obiekt((void *) sa0, (void *) &sa)) == NULL)
                ErrList(15);

            rysuj_obiekt((void *) sa_change, COPY_PUT, 1);
        }
    }
	ADP = ADK = NULL;
	return 1;
}


int dziWez_h_pattern(void* ad)	 /*get trace pattern if filled with image*/
{
	WIELOKAT* w;
    SOLIDARC* sa;
	char* scale_ptr;
	char* dx_ptr;
	char* angle_ptr;
	char* dy_ptr;
	char* name_ptr;
	char* translucency_ptr;
	int transluc;
	int d_trans = 12;
	char sk[10];

	

	if (((NAGLOWEK*)ad)->obiekt == Owwielokat)
	{
		w = (WIELOKAT*)ad;
		if ((w->empty_typ == 0) && (w->pattern == 1))
		{

			rysuj_obiekt(ad, COPY_PUT, 0);
			delay(50);
			rysuj_obiekt(ad, COPY_PUT, 1);

			scale_ptr = w->xy;
			scale_ptr += (w->lp * sizeof(float));
			dx_ptr = scale_ptr;
			dx_ptr += sizeof(short int);
			angle_ptr = dx_ptr;
			angle_ptr += sizeof(short int);
			dy_ptr = angle_ptr;
			dy_ptr += sizeof(short int);
			name_ptr = dy_ptr;
			name_ptr += sizeof(short int);
			memmove(&SolidHatchPatternScale, scale_ptr, sizeof(short int));
			memmove(&SolidHatchPatternDx, dx_ptr, sizeof(short int));
			memmove(&SolidHatchPatternAngle, angle_ptr, sizeof(short int));
			memmove(&SolidHatchPatternDy, dy_ptr, sizeof(short int));
			memmove(&SolidHatchPatternName, name_ptr, strlen(name_ptr) + 1);

			SolidHatchPattern = 1;
			SolidHatch = 0;
			set_solidhatch_pattern_name();

			ADP = ADK = NULL;
			return 1;

		}
		else if ((w->empty_typ == 0) && (w->pattern == 0))
		{
			SolidHatchPattern = 0;
			SolidHatch = 1;
			SolidHatchTranslucent = w->translucent;

			if (w->translucent == 1)
			{
				translucency_ptr = w->xy;
				translucency_ptr += (w->lp * sizeof(float));
				memmove(&SolidHatchTranslucency, translucency_ptr, sizeof(unsigned char));

			}
			else SolidHatchTranslucency = 255;

			transluc = (SolidHatchTranslucency / d_trans) - 1;
			sprintf(sk, "%d%%", transluc * 5);

			set_hatch_pattern(0, 1, FALSE);

			menu_par_new((*m__list_hatch.pola)[0].txt, sk);

			ADP = ADK = NULL;
			return 1;
		}
	}
    else if (((NAGLOWEK*)ad)->obiekt == Osolidarc)
    {
        sa = (SOLIDARC*)ad;
        if ((sa->empty_typ == 0) && (sa->pattern == 1))
        {

            rysuj_obiekt(ad, COPY_PUT, 0);
            delay(50);
            rysuj_obiekt(ad, COPY_PUT, 1);
            
            SolidHatchPatternScale=sa->scale;
            SolidHatchPatternDx=sa->dx;
            SolidHatchPatternAngle=sa->angle;
            SolidHatchPatternDy=sa->dy;
            strcpy(&SolidHatchPatternName, sa->patternname);
            
            SolidHatchPattern = 1;
            SolidHatch = 0;
            set_solidhatch_pattern_name();

            ADP = ADK = NULL;
            return 1;

        }
        else if ((sa->empty_typ == 0) && (sa->pattern == 0))
        {
            SolidHatchPattern = 0;
            SolidHatch = 1;
            SolidHatchTranslucent = w->translucent;

            if (sa->translucent == 1) SolidHatchTranslucency=sa->translucency;
            else SolidHatchTranslucency = 255;

            transluc = (SolidHatchTranslucency / d_trans) - 1;
            sprintf(sk, "%d%%", transluc * 5);

            set_hatch_pattern(0, 1, FALSE);

            menu_par_new((*m__list_hatch.pola)[0].txt, sk);

            ADP = ADK = NULL;
            return 1;
        }
    }
	ADP = ADK = NULL;
	return 0;
}

int odziWez_w(void *ad)
{
  ad = ad ;
  return	1;
}

int odziWez_hw(void *ad)
{
  ad = ad ;
  return	1;
}

int dziU(void *ad)
{
  ((NAGLOWEK*)ad)->atrybut=Ausuniety;
  if (((NAGLOWEK*)ad)->obiekt == Opcx)
  {
	  if (((B_PCX*)ad)->on_front == 0) deleted_pcx = TRUE;
	  else deleted_on_front = TRUE;
  }
  if ((((NAGLOWEK*)ad)->obiekt==Owwielokat) && (((WIELOKAT*)ad)->empty_typ == 0) && (((WIELOKAT*)ad)->pattern == 1)) deleted_pattern = TRUE;
  else if ((((NAGLOWEK*)ad)->obiekt==Osolidarc) && (((SOLIDARC*)ad)->empty_typ == 0) && (((SOLIDARC*)ad)->pattern == 1)) deleted_pattern = TRUE;
  else if ((((NAGLOWEK*)ad)->obiekt == Otekst) && (PTRS__Text_Style[((TEXT*)ad)->czcionka]->type ==2)) deleted_TTF = TRUE;
  else if ((((NAGLOWEK*)ad)->obiekt == Ovector) && (PTRS__Text_Style[zmwym.czcionka]->type ==2)) deleted_TTF = TRUE;
    
  rysuj_obiekt(ad,COPY_PUT,0);
  ADP=ADK=NULL;
  Change	= TRUE;
  Set_Auto_Backup	(TRUE);
  return	1;
}

int odziU(void	*ad)
{
  ad = ad ;
  return	1;
}


static void	redcr_wez_object (char typ, char *kom) {
    static	void (*CUR_oN)(double ,double);
    static	void (*CUR_oFF)(double ,double);
    static	int (	*SW[2])();
    static	TMENU	*menu;
    static	char st[POLE_TXT_MAX];
    static    unsigned short cod = L' ';
    static	int sel_akt;
    static	int sel_cur;
    static	int sel_gor;
    static int iconno=0;
    static BOOL ent_poly;

    if (typ == 0)    /*object*/
    {
        Cur_offd(X, Y);
        set_komunikat0(CUSTOM_KOM, kom);
        komunikat0(CUSTOM_KOM);
        out_eCustom();
        sel_akt = sel.akt;
        sel.akt = 1;

        CUR_oFF = CUR_OFF;
        CUR_oN = CUR_ON;
        SW[0] = SERV[def67];

        SERV[def67] = nooop1;
        *(Block_Proc_Add_Mode_Ptr()) = ADD_MODE;              //konieczne

        Cur_ond(X, Y);
    }
    else
    {
        Cur_offd(X, Y);

        CUR_OFF=CUR_oFF;
        CUR_ON=CUR_oN;
        SERV[def67]= (void*)SW[0];
        sel.akt=sel_akt;

        komunikat0(0);
        Cur_ond(X,Y);
    }

}

static void	redcr_wez_warstwe_kolor_typ (char typ)
/*----------------------------------------------*/
{
  static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt;
  static	int sel_cur;
  static	int sel_gor;
  static int iconno=0;
  static BOOL ent_poly;
  
  if(typ==0)  /*warstwa*/
	{
	  Cur_offd(X, Y);
	  komunikat	(80) ;  
	  sel_akt=sel.akt;  sel.akt=1;

	  CUR_oFF=CUR_OFF;
	  CUR_oN=CUR_ON;
	  SW[0]=SERV[def67];	 SERV[def67]=winf;
	  *(Block_Proc_Add_Mode_Ptr ()) = ADD_MODE ;			  //konieczne
	  winf_draw	 () ;
	  getmenupini (&menu,st,&cod, &iconno);

	  menupini (&mBlock_wez_w, _BLOCK_, _BLOCK_C_, 1) ;

        ent_poly=get_entire_poly();
        set_entire_poly(FALSE);

	  Cur_ond(X, Y);
	}
	else if(typ==2) /*kolor*/
	{
	  Cur_offd(X, Y);
	  komunikat	(82) ;  
	  sel_akt=sel.akt;  sel.akt=1;

	  CUR_oFF=CUR_OFF;
	  CUR_oN=CUR_ON;
	  SW[0]=SERV[def67];	 SERV[def67]=winf;
	  *(Block_Proc_Add_Mode_Ptr ()) = ADD_MODE ;			  //konieczne
	  winf_draw	 () ;
	  getmenupini (&menu,st,&cod,&iconno);

        ent_poly=get_entire_poly();
        set_entire_poly(FALSE);

	  menupini (&mBlock_wez_w, _BLOCK_, _BLOCK_C_, 1) ;

	  Cur_ond(X, Y);
	}
	else if(typ==3)	/*typ*/
	 {
	  Cur_offd(X, Y);
	  komunikat	(83) ;  
	  sel_akt=sel.akt;  sel.akt=1;

	  CUR_oFF=CUR_OFF;
	  CUR_oN=CUR_ON;
	  SW[0]=SERV[def67];	 SERV[def67]=winf;
	  *(Block_Proc_Add_Mode_Ptr ()) = ADD_MODE ;			  //konieczne
	  winf_draw	 () ;
	  getmenupini (&menu,st,&cod,&iconno);

         ent_poly=get_entire_poly();
         set_entire_poly(FALSE);

	  menupini (&mBlock_wez_w, _BLOCK_, _BLOCK_C_, 1) ;

	  Cur_ond (X,	Y)	;
	}
	else if(typ==4)	/*info*/
	 {
	  Cur_offd(X, Y);
	  komunikat	(110)	;	
	  sel_akt=sel.akt;  sel.akt=1;

	  CUR_oFF=CUR_OFF;
	  CUR_oN=CUR_ON;
	  SW[0]=SERV[def67];	 SERV[def67]=winf;
	  *(Block_Proc_Add_Mode_Ptr ()) = ADD_MODE ;			  //konieczne
	  getmenupini (&menu,st,&cod,&iconno);

      ent_poly=get_entire_poly();

	  menupini (&mBlock_wez_info, _OBJECT_, _OBJECT_C_, 686) ;
      if (entire_poly==TRUE) menu_par_new((*menup.pola)[6].txt,_entire_polyline_);
      else menu_par_new((*menup.pola)[6].txt, _single_object_);

	  Cur_ond (X,	Y)	;
	}
	else if ((typ == 5)	/*get pattern*/ || (typ == 6))  /*set pattern*/
  {
	  Cur_offd(X, Y);
	  if (typ == 5) komunikat(196);
	  else komunikat(197);
	  sel_akt = sel.akt;  sel.akt = 1;

	  CUR_oFF = CUR_OFF;
	  CUR_oN = CUR_ON;

	  CUR_OFF = cursel_off;
	  CUR_ON = cursel_on;

	  SW[0] = SERV[def67];	 SERV[def67] = winf;
	  *(Block_Proc_Add_Mode_Ptr()) = ADD_MODE;			  //konieczne
	  winf_draw();
	  getmenupini(&menu, st, &cod, &iconno);

	  menupini(&mBlock_wez_w, _BLOCK_, _BLOCK_C_, 1);

	  Cur_ond(X, Y);
  }
	else if(typ==9)  /*ukryj warstwe*/
	{
	  Cur_offd(X, Y);
	  komunikat	(191) ;  
	  sel_akt=sel.akt;  sel.akt=1;

	  CUR_oFF=CUR_OFF;
	  CUR_oN=CUR_ON;
	  SW[0]=SERV[def67];	 SERV[def67]=winf;
	  *(Block_Proc_Add_Mode_Ptr ()) = ADD_MODE ;			  //konieczne
	  winf_draw	 () ;
	  getmenupini (&menu,st,&cod,&iconno);

	  menupini (&mBlock_wez_w, _BLOCK_, _BLOCK_C_, 1) ;

	  Cur_ond (X,	Y)	;
	}
   else if(typ==10)	/*sheet*/
	 {
	  Cur_offd(X, Y);
	  komunikat	(182)	;
	  sel_akt=sel.akt;  sel.akt=1;

	  CUR_oFF=CUR_OFF;
	  CUR_oN=CUR_ON;
	  SW[0]=SERV[def67];	 SERV[def67]=winf;
	  *(Block_Proc_Add_Mode_Ptr ()) = ADD_MODE ;			  //konieczne
	  winf_draw	 () ;
	  getmenupini (&menu,st,&cod,&iconno);

	  menupini (&mBlock_wez_w, _BLOCK_, _BLOCK_C_, 1) ;

	  Cur_ond (X,	Y)	;
	}
   else if(typ==11)	/*object*/
   {
      Cur_offd(X, Y);
      komunikat	(182)	;
      sel_akt=sel.akt;  sel.akt=1;

      CUR_oFF=CUR_OFF;
      CUR_oN=CUR_ON;
      SW[0]=SERV[def67];	 SERV[def67]=winf;
      *(Block_Proc_Add_Mode_Ptr ()) = ADD_MODE ;			  //konieczne
      winf_draw	 () ;
      getmenupini (&menu,st,&cod,&iconno);

      menupini (&mBlock_wez_w, _BLOCK_, _BLOCK_C_, 1) ;

      Cur_ond (X,	Y)	;
    }

	else /*1*/
	{ 
	  Cur_offd(X, Y);
	  menupini(menu,st,cod, iconno);
	  setfillstyle_(SOLID_FILL,	BKCOLOR);
	  bar(maxX/2+1,0,maxX/2+(100*getmaxx()/640),ED_INF_HEIGHT /*11*/);
	  CUR_OFF=CUR_oFF;
	  CUR_ON=CUR_oN;
	  SERV[def67]= (void*)SW[0];
	  sel.akt=sel_akt;

	  komunikat(0);

      set_entire_poly(ent_poly);

	  Cur_ond(X,Y);
	}
	
}


static void	redcr_del_quick (char typ)
//------------------------------------
{
  static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt, sel_gor, sel_cur, sel_nr;
  static    int iconno;
  if(!typ)
	{
	  Cur_offd(X, Y);
	  komunikat0 (95)	;
	  sel_akt=sel.akt;  sel.akt=1;
	  sel_cur = sel.cur;
	  sel_gor = sel.gor; sel.gor = 0;
	  sel_nr = sel.nr;
	  layer_info();

	  CUR_oFF=CUR_OFF;
	  CUR_oN=CUR_ON;
	  SW[0]=SERV[def67];	 SERV[def67]=winf;
	  *(Block_Proc_Add_Mode_Ptr ()) = ADD_MODE ;			  //konieczne
	  winf_draw	 () ;
	  getmenupini (&menu,st,&cod,&iconno);

	  menupini (&mBlock_dq, _BLOCK_, _BLOCK_C_, 1) ;

	  Cur_ond(X, Y);
	}
  else
	{ 
	  Cur_offd(X, Y);
	  menupini(menu,st,cod, iconno);
	  setfillstyle_(SOLID_FILL,	BKCOLOR);
	  bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
	  CUR_OFF=CUR_oFF;
	  CUR_ON=CUR_oN;
	  SERV[def67]= (void*)SW[0];
	  sel.akt=sel_akt;
	  sel.cur = sel_cur;
	  sel.gor = sel_gor;
	  sel.nr = sel_nr;
	  Cur_ond(X,Y);
	}
}



void Information_about (void)
/*-------------------------*/
{
  int	i_ret	;

  standard_func();	
poczatek:  
  if (wez_warstwe_typ_kolor) return; //	0;
  wez_warstwe_typ_kolor=TRUE;
  redcr_wez_warstwe_kolor_typ	(4);
  set_entire_poly(ent_poly_info);
  i_ret = Block_Proc_Wez_w	(dziWez_info, odziWez_w, nooop, COMNDwez_info, 1) ;
  ent_poly_info=get_entire_poly();
  redcr_wez_warstwe_kolor_typ	(1) ;
  wez_warstwe_typ_kolor=FALSE;
  if (i_ret==1) goto	poczatek;
  return;
}

char *Get_Sheet (void)
/*-------------------------*/
{
  int	i_ret	;

  standard_func();
  adr_sheet=NULL;
poczatek1:
  if (wez_warstwe_typ_kolor) return	0;
  wez_warstwe_typ_kolor=TRUE;
  redcr_wez_warstwe_kolor_typ	(10);
  i_ret = Block_Proc_Wez_w	(dziWez_sheet, odziWez_w, nooop, COMNDwez_w, 1) ;
  redcr_wez_warstwe_kolor_typ	(1) ;
  wez_warstwe_typ_kolor=FALSE;
  if (i_ret==0) return NULL;
    else if (adr_sheet==NULL) goto poczatek1;
  return	adr_sheet;
}

char *Get_Object (unsigned int typ, char *kom)
/*-------------------------------------------*/
{
    int	i_ret	;

    adr_object=NULL;
    poczatek1:

    wez_warstwe_typ_kolor=TRUE;
    redcr_wez_object(0, kom) ;
    i_ret = Block_Proc_Wez_w	(dziWez_object, odziWez_w, nooop, COMNDwez_w, typ) ;
    redcr_wez_object(1, kom) ;
    wez_warstwe_typ_kolor=FALSE;
    if (i_ret==0) return NULL;
    return	adr_object;
}

static int Hide_Layers (void)
//------------------------------
{
  int	i_ret	;
  if (wez_warstwe_typ_kolor) return	0;
  wez_warstwe_typ_kolor=TRUE;
  redcr_wez_warstwe_kolor_typ	(9);
  i_ret = Block_Proc_Wez_hw	(dziWez_hw, odziWez_hw, nooop, COMNDwez_w, 0) ;
  redcr_wez_warstwe_kolor_typ	(1) ;
  wez_warstwe_typ_kolor=FALSE;
  return	i_ret	;
}

static int wez_warstwe (void)
//------------------------------
{
  int	i_ret	;
  if (wez_warstwe_typ_kolor) return	0;
  wez_warstwe_typ_kolor=TRUE;
  redcr_wez_warstwe_kolor_typ	(0);
  i_ret = Block_Proc_Wez_w	(dziWez_w, odziWez_w, nooop, COMNDwez_w, 0) ;
  redcr_wez_warstwe_kolor_typ	(1) ;
  wez_warstwe_typ_kolor=FALSE;
  return	i_ret	;
}

static int wez_kolor	(void)
//------------------------------
{
  int	i_ret	;

  if (wez_warstwe_typ_kolor) return	0;
  wez_warstwe_typ_kolor=TRUE;
  redcr_wez_warstwe_kolor_typ	(2);
  i_ret = Block_Proc_Wez_w	(dziWez_k, odziWez_w, nooop, COMNDwez_w, 1) ;
  redcr_wez_warstwe_kolor_typ	(1) ;
  wez_warstwe_typ_kolor=FALSE;
  return	i_ret	;
}

static int wez_typ (void)
//------------------------------
{
  int	i_ret	;

  if (wez_warstwe_typ_kolor) return	0;
  wez_warstwe_typ_kolor=TRUE;
  redcr_wez_warstwe_kolor_typ	(3);
  i_ret = Block_Proc_Wez_w	(dziWez_t, odziWez_w, nooop, COMNDwez_w, 1) ;
  redcr_wez_warstwe_kolor_typ	(1) ;
  if (linia_tekst==0) view_line_type(&LiniaG);
	 else	if	(linia_tekst==1) view_font_name(&TextG);
  wez_warstwe_typ_kolor=FALSE;
  return	i_ret	;
}

static int wez_solid_pattern(void)
//--------------------------------
{
	int	i_ret;
	if (wez_warstwe_typ_kolor) return	0;
	wez_warstwe_typ_kolor = TRUE;
	redcr_wez_warstwe_kolor_typ(5);
	global_only_solid = TRUE;
	i_ret = Block_Proc_Wez_w(dziWez_s_pattern, odziWez_w, nooop, COMNDwez_w, 0);
	global_only_solid = FALSE;
	redcr_wez_warstwe_kolor_typ(1);

	wez_warstwe_typ_kolor = FALSE;
	return	i_ret;
}

static int set_solid_pattern(void)
//--------------------------------
{
	int	i_ret;
	if (wez_warstwe_typ_kolor) return	0;
	wez_warstwe_typ_kolor = TRUE;
	redcr_wez_warstwe_kolor_typ(6);
	global_only_solid = TRUE;
	i_ret = Block_Proc_Set_w(dziSet_s_pattern, odziWez_w, nooop, COMNDwez_w, 0);
	global_only_solid = FALSE;
	redcr_wez_warstwe_kolor_typ(1);

	wez_warstwe_typ_kolor = FALSE;
	return	i_ret;
}

static int wez_trace_pattern(void)
//--------------------------------
{
	int	i_ret;
	if (wez_warstwe_typ_kolor) return	0;
	wez_warstwe_typ_kolor = TRUE;
	redcr_wez_warstwe_kolor_typ(5);
	global_only_solid = TRUE;
	i_ret = Block_Proc_Wez_w(dziWez_t_pattern, odziWez_w, nooop, COMNDwez_w, 0);
	global_only_solid = FALSE;
	redcr_wez_warstwe_kolor_typ(1);

	wez_warstwe_typ_kolor = FALSE;
	return	i_ret;
}

static int set_trace_pattern(void)
//--------------------------------
{
	int	i_ret;
	if (wez_warstwe_typ_kolor) return	0;
	wez_warstwe_typ_kolor = TRUE;
	redcr_wez_warstwe_kolor_typ(6);
	global_only_solid = TRUE;
	i_ret = Block_Proc_Set_w(dziSet_t_pattern, odziWez_w, nooop, COMNDwez_w, 0);
	global_only_solid = FALSE;
	redcr_wez_warstwe_kolor_typ(1);

	wez_warstwe_typ_kolor = FALSE;
	return	i_ret;
}

static int wez_hatchsolid_pattern(void)
//--------------------------------
{
	int	i_ret;
	if (wez_warstwe_typ_kolor) return	0;
	wez_warstwe_typ_kolor = TRUE;
	redcr_wez_warstwe_kolor_typ(5);
	global_only_solid = TRUE;
	global_point_in_solid = TRUE;
	i_ret = Block_Proc_Wez_w(dziWez_h_pattern, odziWez_w, nooop, COMNDwez_w, 0);
	global_point_in_solid = FALSE;
	global_only_solid = FALSE;
	redcr_wez_warstwe_kolor_typ(1);

	wez_warstwe_typ_kolor = FALSE;
	return	i_ret;
}


static int delete_quick	(void)
//------------------------------
{ static	int (	*SW[9])();
  int	i_ret	;

  SW [2] = SERV [58];
  SW [3] = SERV [59];
  SW [4]	= SERV [60];
  SW [5]	= SERV [61];
  SW [6]	= SERV [62];
  SW [7]	= SERV [63];
  SW [8]	= SERV [64];

  SERV [58] = nooop1;
  SERV [59] = nooop1;
  SERV [60]	= nooop1;
  SERV [61]	= nooop1;
  SERV [62]	= nooop1;
  SERV [63]	= nooop1;
  SERV [64]	= nooop1;
   redcr_del_quick	(0);
  i_ret = Block_Proc	(dziU, odziU, nooop,	COMNDmb_dq)	;
  redcr_del_quick	(1) ;
  SERV [58] = (void*)SW [2];
  SERV [59] = (void*)SW [3];
  SERV [60]	= (void*)SW [4];
  SERV [61]	= (void*)SW [5];
  SERV [62]	= (void*)SW [6];
  SERV [63]	= (void*)SW [7];
  SERV [64]	= (void*)SW [8];
  return	i_ret	;
}

static BOOL	b__del_quick =	TRUE ;


void Wez_warstwe (void)
/*-------------------*/
{ int	k;
  k=wez_warstwe ();
  if ((k) && (options1.view_only_current_layer==1)) redraw();
}

void Wez_kolor	(void)
/*-------------------*/
{ int	k;
  k=wez_kolor ();
}

void Wez_typ (void)
/*-------------------*/
{ int	k;
  k=wez_typ	();
}

void Wez_Solid_Pattern(void)
/*------------------------*/
{
	int	k;
	k = wez_solid_pattern();
}

void Set_Solid_Pattern(void)
/*------------------------*/
{
	int	k;
	k = set_solid_pattern();
}

void Wez_Trace_Pattern(void)
/*------------------------*/
{
	int	k;
	k = wez_trace_pattern();
}

void Set_Trace_Pattern(void)
/*------------------------*/
{
	int	k;
	k = set_trace_pattern();
}

int Wez_HatchSolid_Pattern(void)
/*------------------------*/
{
	return wez_hatchsolid_pattern();
}

int Wez_warstwe_ (void)
/*-------------------*/
{
 Wez_warstwe ();
 return 0;
}

int Hide_Layers_ (void)
/*-------------------*/
{
 Hide_Layers ();
 return 0;
}

int Wez_kolor_	(void)
/*-------------------*/
{
 Wez_kolor ();
 return 0;
}

int Wez_typ_ (void)
/*-------------------*/
{
 Wez_typ	();
 return 0;
}


void Usun (void)
//-------------
{
  TTF_redraw = FALSE;
  Cur_offd (X,	Y)	;
  pcx_outlines_view_on();
  usun_blok	(dane, dane	+ dane_size) ;
  Cur_ond (X, Y) ;
  b__del_quick = get_del_quick();
  while (1)
  {
	 if (b__del_quick	==	TRUE)
	 {
		if	(FALSE == delete_quick ())
		{
		  break ;
		}
		else
		{
		   b__del_quick	= FALSE ;
		}
	 }
	 else
	 {
		if	(FALSE == delete_normal	())
		{
		  break ;
		}
		else
		{
		   b__del_quick	= TRUE ;
		}
	 }
  }
  Cur_offd(X, Y);
  pcx_outlines_view_off();
  Cur_ond(X, Y);
  komunikat	(0) ;
  komunikat0 (0) ;
}

int Usun_F5	(void)
//-----------------
{
  Cur_offd (X,	Y)	;
  usun_blok	(dane, dane	+ dane_size) ;
  Cur_ond (X, Y) ;
  while (1)
  {

		if	(FALSE == delete_quick ())
		{
		  break ;
		}

  }
  komunikat	(0) ;
  komunikat0 (0) ;
  return	0;
}




/*---------Explode--------------------------------------------------*/

int dzi_ex (void *ad)
/*------------------*/
{
  NAGLOWEK *nag;

  nag=(NAGLOWEK*)ad;
  if ((nag->obiekt	!=	OdBLOK)  && (nag->blok	==	NoElemBlok))
  {

	  if (komunikaty_bloku==FALSE) 
		 {
			komunikat (76); 
			komunikaty_bloku=TRUE;
		 }	 
	  return	0;
  }
  rysuj_obiekt(ad,XOR_PUT,1);
  nag->atrybut=Ablok;
  return	1;
}

int odzi_ex(void *ad)
/*------------------*/
{
  rysuj_obiekt(ad,XOR_PUT,1);
  ((NAGLOWEK*)ad)->atrybut=Anormalny;
  return	1;
}


static void	explode_block (void*	ptr_ob)
/*-------------------------------------*/
{
  BLOK *ptrs_bl ;
  long_long off ,offp,	offk ;
  NAGLOWEK *nag ;
  char  *ptrh_temp ;

  ptrs_bl =	(BLOK*)ptr_ob ;
  ptrh_temp	= (char *)ptrs_bl	+
	 sizeof(NAGLOWEK)	+ B3 + ptrs_bl->dlugosc_opisu_obiektu ;
  offp =	ptrh_temp -	dane ;
  ptrh_temp	= (char *)ptrs_bl	+ sizeof(NAGLOWEK) +	ptrs_bl->n - 1	;
  offk =	ptrh_temp -	dane ;
  ObiektTok	(&offp, offk, &off, ONoInBlock) ;
  while (off != -1)
  {
	 nag =(NAGLOWEK*)(dane + off)	;
	 nag->blok = NoElemBlok	;
    if (nag->obiekt != OdBLOK)
     {
      if ((nag->obiektt2==O2BlockHatch25) || (nag->obiektt2==O2BlockHatch50) || (nag->obiektt2==O2BlockAparat))
       {
        ;  //nic nie jest robione
       }
      else
       {
    	   if (nag->obiektt2 != O2NoBlockS) /*element bloku specjalnego przestaje nim byc*/
  	       {
  	        nag->obiektt2 = O2NoBlockS;
	       }
       }
     }
	 ObiektTok (&offp, offk, &off, ONoInBlock) ;
  }
  ptrs_bl->n =	B3	+ ptrs_bl->dlugosc_opisu_obiektu	;
  ptrs_bl->atrybut =	Abad ;
}


static void	explode (void)	
/*---------------------------*/
{
  long_long off ,offp,	offk ;
  NAGLOWEK *nag ;
  BLOK 			*ptrs_block	;
  BOOL go_explode;
  char nazwa_bloku[12];
  T_Desc_Ex_Block		*ptrs_desc_bl ;
  BOOL komunikat84;
  int li;
  int ret;

  komunikat84 = FALSE;

  blokzap(ADP,ADK,Ablok,COPY_PUT,1);
  if ((ADP!=NULL)	&&	(ADK!=NULL))
  {
	offp = ADP - dane	;
	offk = ADK - dane	;
	ObiektTok (&offp,	offk,	&off,	ONoInBlock)	;
	while	(off != -1)
	{
	 nag = (NAGLOWEK*)(dane	+ off) ;
	 if ((nag->atrybut	==	Ablok)	&&	(nag->obiekt	==	OdBLOK) )
	 {
	  ptrs_block =	(BLOK*)(dane +	off);
	  go_explode=TRUE;

     if (ptrs_block->dlugosc_opisu_obiektu > 1)
     {
  	    ptrs_desc_bl	= (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu	[0]) ;
	    strncpy(nazwa_bloku,&ptrs_desc_bl->sz_type	[0],6) ;
	  
	    if (strlen(nazwa_bloku)>0)
	  	  {
		   if ((strncmp ( nazwa_bloku,"$",1 )	==	0))
		    { /*aparat*/
			   go_explode=FALSE;
		    }
		    else
			  {
			   if ((strncmp ( nazwa_bloku,"*",1 )	==	0))
			    {/*pin*/
				  go_explode=FALSE;
			    }
			  }
		  }

     }
     if (ptrs_block->kod_obiektu == B_SEKTORY) go_explode=FALSE;
      else
      {
	    if (go_explode==TRUE)
		  {
		   explode_block	((void*)nag) ;
		  }
		  else
		   {
		   /*komunikat ze jest to blok nieeksplodowalny - aparat*/
		    if (komunikat84	==	FALSE)
		    {

			   komunikat84 =	TRUE;
		    }
		   }
      }
	 }
	 ObiektTok (&offp, offk, &off, ONoInBlock) ;
	}
	memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
	zmien_atrybut_undo (dane, dane +	dane_size) ;
	zmien_atrybut (ADP, ADK, Ablok, Aoblok) ;

	//deleting hatch outline
	li=zmien_atrybut_hatch_outline(ADP, ADK, Aoblok, Ausuniety, 0);
	usun_blok(dane, dane + dane_size);
    pcx_outlines_view_off();
    CUR_OFF (X, Y) ;
	CUR_ON (X, Y) ;
	ADP=ADK=NULL;
	if	(komunikat84==TRUE)
	 {
	 ret = ask_question(1, "", "OK", "", get_komunikat_ptr(84), 12, "", 11, 1, 61);
	 }
	Change =	TRUE;
	Set_Auto_Backup (TRUE);
  }
  else
  {
	CUR_ON (X, Y) ;
	ADP=ADK=NULL;
  } 
  if (li > 0) redraw();
}

static void	explode_aparat	(void) 
/*---------------------------*/
{
  long_long off ,offp,	offk ;
  NAGLOWEK *nag ;
  BLOK 			*ptrs_block	;
  BOOL go_explode;
  char nazwa_bloku[12];
  T_Desc_Ex_Block		*ptrs_desc_bl ;
  BOOL komunikat85;
  int ret;

  komunikat85 = FALSE;
  blokzap(ADP,ADK,Ablok,COPY_PUT,1);
  if ((ADP!=NULL)	&&	(ADK!=NULL))
  {
	offp = ADP - dane	;
	offk = ADK - dane	;
	ObiektTok (&offp,	offk,	&off,	ONoInBlock)	;
	while	(off != -1)
	{
	 nag = (NAGLOWEK*)(dane	+ off) ;
	 if ((nag->atrybut	==	Ablok)	&&	(nag->obiekt	==	OdBLOK ))
	 {
	  ptrs_block =	(BLOK*)(dane +	off);
	  go_explode=FALSE;
	  
	  ptrs_desc_bl	= (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu	[0]) ;
	  strncpy(nazwa_bloku,&ptrs_desc_bl->sz_type	[0], 6) ;
	  
	  if (strlen(nazwa_bloku)>0)
		{ 
		 if ((strncmp ( nazwa_bloku,"$",1 )	==	0)) 
		  { /*aparat*/
			 go_explode=TRUE;
		  }
		  else
			{	
			 if ((strncmp ( nazwa_bloku,"*",1 )	==	0)) 
			  {/*pin*/
				go_explode=TRUE;
			  } 
			}	  
		} 
	  if (go_explode==TRUE)
		{
		 explode_block	((void*)nag) ;	
		} 
		else
		 {
		 /*komunikat ze blok nie jest aparatem*/
		 if (komunikat85 == FALSE)
		  {
			komunikat85 =	TRUE;
		  }  
		 }
	 }
	 ObiektTok (&offp, offk, &off, ONoInBlock) ;
	}
	memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
	zmien_atrybut_undo (dane, dane +	dane_size) ;
	zmien_atrybut (ADP, ADK, Ablok, Aoblok) ;
    pcx_outlines_view_off();
    CUR_OFF (X, Y) ;
	CUR_ON (X, Y) ;
	ADP=ADK=NULL;
	if	(komunikat85==TRUE)
	 {
	 ret = ask_question(1, "", "OK", "", get_komunikat_ptr(85), 12, "", 11, 1, 61);
	 }
	Change =	TRUE;
	Set_Auto_Backup (TRUE);
  }
  else
  {
	CUR_ON (X, Y) ;
	ADP=ADK=NULL;
  } 
}

static void	explode1	(void)
/*---------------------------*/
{
  long_long off ,offp,	offk ;
  NAGLOWEK *nag ;

  CUR_OFF (X, Y) ;
  if ((ADP!=NULL)	&&	(ADK!=NULL))
  {
	offp = ADP - dane	;
	offk = ADK - dane	;
	ObiektTok (&offp,	offk,	&off,	ONoInBlock)	;
	while	(off != -1)
	{
	 nag = (NAGLOWEK*)(dane	+ off) ;
	 if ((nag->atrybut	==	Ablok)	&&	(nag->obiekt	==	OdBLOK))
	 {
		explode_block ((void*)nag)	;
	 }
	 ObiektTok (&offp, offk, &off, ONoInBlock) ;
	}
	memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
	zmien_atrybut_undo (dane, dane +	dane_size) ;
	zmien_atrybut (ADP, ADK, Ablok, Aoblok) ;
	CUR_ON (X, Y) ;
	ADP=ADK=NULL;
	Change =	TRUE;
	Set_Auto_Backup (TRUE);
  }
  else
  {
	CUR_ON (X, Y) ;
	ADP=ADK=NULL;
  } 
	
}

static void	(*COMNDmb_ex[])(void)=
{(void(*)(void)) ostatni_blok, (void (*)(void))wszystko, (void (*)(void))wszystkie_warstwy,
 (void (*)(void))winf, (void	(*)(void))addf, nooop, (void (*)(void))explode,
 (void (*)(void))auto_on, (void	(*)(void))auto_off };
  
static /*const*/ void	(*COMNDmb_ex_ap[])(void)=
{ (void (*)(void))ostatni_blok,(void (*)(void))wszystko,(void (*)(void))wszystkie_warstwy,
  (void (*)(void))winf,	(void	(*)(void))addf, nooop, (void (*)(void))explode_aparat,
  (void (*)(void))auto_on,	(void	(*)(void))auto_off };  


static void	redcr_ex(char typ)
{ static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt;
  static int iconno=0;
  switch(typ)
	{ case 0	:
		Cur_offd(X, Y);
		komunikat(0);
		pcx_outlines_view_on();
		komunikat0(41);
		sel_akt=sel.akt;	sel.akt=1;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod,&iconno);

		menupini(&mBlok_ex, _BLOCK_, _BLOCK_C_, 1);

		Cur_ond(X, Y);
		break;
	  case 1	:
		pcx_outlines_view_off();
		menupini(menu,st,cod, iconno);
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		CUR_OFF=CUR_oFF;
		CUR_ON=CUR_oN;
		SERV[def67]= (void*)SW[0];
		SERV[def68]= (void*)SW[1];
		sel.akt=sel_akt;
		if	(ADP != NULL && ADK != NULL)
		{
		 blokzap(ADP,ADK,Ablok,COPY_PUT,1);
		 memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
		 zmien_atrybut_undo(dane, dane +	dane_size);
		 zmien_atrybut(ADP,ADK,Ablok,Aoblok);
		}
        CUR_OFF(X,Y);
        CUR_ON(X,Y);
		komunikat (0) ;
		komunikat0 (0)	;
	}
}

static void	redcr_ex_ap(char typ)
{ static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt;
  static    int iconno;
  switch(typ)
	{ case 0	:
		Cur_offd(X, Y);
		komunikat(0);
		pcx_outlines_view_on();
		komunikat0(112);
		sel_akt=sel.akt;	sel.akt=1;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod,&iconno);

		menupini(&mBlok_ex, _BLOCKF_, _BLOCK_C_, 1);

		Cur_ond(X, Y);
		break;
	  case 1	:
		pcx_outlines_view_off();
		menupini(menu,st,cod, iconno);
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		CUR_OFF=CUR_oFF;
		CUR_ON=CUR_oN;
		SERV[def67]= (void*)SW[0];
		SERV[def68]= (void*)SW[1];
		sel.akt=sel_akt;
		if	(ADP != NULL && ADK != NULL)
		{
		 blokzap(ADP,ADK,Ablok,COPY_PUT,1);
		 memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
		 zmien_atrybut_undo(dane, dane +	dane_size);
		 zmien_atrybut(ADP,ADK,Ablok,Aoblok);
		}
        CUR_OFF(X,Y);
        CUR_ON(X,Y);
		komunikat (0) ;
		komunikat0 (0)	;
	}
}

int Explode_dlg(BOOL frozen)
{
	char frozen_str[12];
	int ret;
	char explode_str[64];

	if (frozen) strcpy(frozen_str, _FROZEN_);
	else strcpy(frozen_str, "");
	sprintf(explode_str, _EXPLODE_BLOCKS_, frozen_str);
	ret = ask_question(2, _No_, _Yes_, "", explode_str, 12, "", 11, 1, 0);
	//1 ok; 0 - rezygnuj; 2 - Powrot
	return ret;	
}

void Explode (void)
/*-----------------*/
{
  redcr_ex(0);
  global_any_choice=TRUE;
  blok(dzi_ex,odzi_ex, Redraw_Block, COMNDmb_ex);
  global_any_choice=FALSE;
  if (check_if_obiekt(dane, dane + dane_size, Ablok, ONieOkreslony))
  {
	  if (Explode_dlg(FALSE)) explode();
  }
  redcr_ex(1);
  return;
}

void Explode_ap (void)
/*-----------------*/
{
  redcr_ex_ap(0);
  blok(dzi_ex,odzi_ex, Redraw_Block, COMNDmb_ex_ap);
  if (check_if_obiekt(dane, dane + dane_size, Ablok, ONieOkreslony))
  {
	  if (Explode_dlg(TRUE)) explode_aparat();
  }
  redcr_ex_ap(1);
  return;
}

void Explode1 (void)
{
 explode1 ();
}


/*-----skala------------------------------------*/

static void	redcrS(char	typ)
{ static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt;
  static    int iconno;
  switch(typ)
	{ case 0	:
		Cur_offd(X, Y);
		komunikat(0);
		pcx_outlines_view_on();
		komunikat0(26);
		sel_akt=sel.akt;	sel.akt=1;
		DX=DY=0;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod, &iconno);

		menupini(&mBlokm, _BLOCK_, _BLOCK_C_, 1);

		Cur_ond(X, Y);
		break;
	  case 1	: 
		Cur_offd(X, Y);
		pcx_outlines_view_off();
		SERV[def67]= (void*)SW[0];
		SERV[def68]= (void*)SW[1];

        menupini(menu,st,cod, iconno);

		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		komunikat0(27);
		Cur_ond(X, Y);
		break;
	  case 2 :
      case 3 :
            blokzap(ADP,ADK,Ablok,COPY_PUT,1);
            CUR_OFF(X,Y);
			CUR_ON(X,Y);
           break;
	  case 4	: break;
	  default: break;
	}
  if(typ==2	||	typ==3 || typ==4)
	{ CUR_OFF=CUR_oFF;
	  CUR_ON=CUR_oN;
	  sel.akt=sel_akt;

	  menupini(menu,st,cod, iconno);

	  if (typ==4)
	  {
		zmien_atrybut_undo(dane, dane	+ dane_size);
		zmien_atrybut(ADP,ADK,Ablok,Aoblok);
	  } 
	  else
		 zmien_atrybut(ADP,ADK,Ablok,Anormalny);

      //Cur_ond(X,Y);
	  //Cur_ond(X,Y);
        CUR_OFF(X,Y);
        CUR_ON(X,Y);
	  komunikat	(0) ;
	  komunikat0 (0) ;
	}
}

void Skala(void)
{
  TTF_redraw = FALSE;
  redcrS(0);
  blok(dzi,odzi,Redraw_Block,COMNDmb);
  redcrS(1);
  if (przesunp()==ESC)
	{ redcrS(2);
	  return;
	}
  if (Scale_Proc () == ESC) redcrS (3)	;
  else
  {
	  redcrS(4);
	  if (TTF_redraw) redraw();
      else flip_screen();
  }
  return	;
}

/*----obrot-------------------------------------------------------*/

static void	redcrO(char	typ)
{ static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt, sel_cur, sel_gor;
  static    int iconno;
  switch(typ)
	{ case 0	:
		Cur_offd(X, Y);
		komunikat(0);
		pcx_outlines_view_on();
		komunikat0 (32) ;
		sel_akt=sel.akt; sel.akt=1;
		sel_cur = sel.cur;
		sel_gor= sel.gor;

		DX=DY=0;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod,&iconno);

		menupini(&mBlokm, _BLOCK_, _BLOCK_C_, 1);

		Cur_ond(X, Y);
		break;
	  case 1	: 
		Cur_offd(X, Y);

		pcx_outlines_view_off();
		SERV[def67]= (void*)SW[0];
		SERV[def68]= (void*)SW[1];
      ADP_0=ADP;
      ADK_0=ADK;
		menupini(menu,st,cod, iconno);
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		komunikat0 (27) ;
		Cur_ond(X, Y);
		break;
	  case 2	:
		       ADP=ADP_0;
               ADK=ADK_0;
	           blokzap(ADP,ADK,Ablok,XOR_PUT,1);
               CUR_OFF(X,Y);
               CUR_ON(X,Y);
	  case 3	: 
	  case 4	: 

		  ADP=ADP_0;
		  ADK=ADK_0;
		  CUR_OFF=CUR_oFF;
		  CUR_ON=CUR_oN;
		  sel.akt=sel_akt;
		  sel.cur = sel_cur;
		  sel.gor = sel_gor;

		  if (typ==4)
			{
			 zmien_atrybut_undo(dane, dane +	dane_size);
			 zmien_atrybut(ADP,ADK,Ablok,Aoblok);
			} 
			else {
              blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
              zmien_atrybut(ADP, ADK, Ablok, Anormalny);
          }
		  Cur_ond(X,Y);

		  komunikat	(0) ;
		  komunikat0 (0) ;
		  break;
	  default: 
		  break;
	}

}

void Obrot(void)
{
  TTF_redraw = FALSE;
  redcrO(0);
  blok(dzi,odzi,Redraw_Block,COMNDmb);
  redcrO(1);
  if (przesunp()==ESC)
	{ redcrO(2);
	  return;
	}
  if (Rotate_Proc	()	==	ESC)
	  redcrO	(3) ;
  else
  {
	  redcrO(4);
	  if (TTF_redraw) redraw();
      else flip_screen();
  }
  return	;
}


void Obrot1(void)
{ BLOK *ptrs_block;
  T_Desc_Ex_Block		*ptrs_desc_bl ;
  BOOL point_ok;
  char *adp;

  TTF_redraw = FALSE;
while (1)
 {
  redcrO(0);
  
  
  if (blok_FIRSTB (dzi, odzi, Redraw_Block,	COMNDmb) == 0)
   {
    redcrO	(1) ;
	Cur_offd(X, Y);
    redcrO (2);
    return ;
   }
  
  redcrO(1);

 
  //odszukanie punktu bazowego
  point_ok=FALSE;
  adp=find_block_atrybut(dane,dane+dane_size,Ablok);
  if (adp!=NULL)
  {
   ptrs_block=(BLOK *)adp;
   if (ptrs_block->dlugosc_opisu_obiektu > 1)
     {
      ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu	[0]) ;
      CUR_OFF(X,Y);
      X=ptrs_desc_bl->x;
      Y=ptrs_desc_bl->y;
      CUR_ON(X,Y);
      point_ok=TRUE;
     }
   else
   {
	  CUR_OFF(X,Y);
   }
  }

 if (point_ok==FALSE)
 {
  redcrO	(1) ; //
  redcrO	(2) ; //
  continue;
 }

  if (Rotate_Proc	()	==	ESC)
	  redcrO	(3) ;
  else
  {
	  redcrO(4);
	  if (TTF_redraw) redraw();
      else flip_screen();
  }
 }
  
  return	;
}

void ObrotXZ(void)
{
  redcrO(0);
  blok(dzi,odzi,Redraw_Block,COMNDmb);
  redcrO(1);
  if (przesunp()==ESC)
	{ redcrO(2);
	  return;
	}
  if (Rotate_ProcXZ	()	==	ESC)
	  redcrO	(3) ;
  else
	  redcrO	(4) ;
  return	;
}

void ObrotYZ(void)
{
  redcrO(0);
  blok(dzi,odzi,Redraw_Block,COMNDmb);
  redcrO(1);
  if (przesunp()==ESC)
	{ redcrO(2);
	  return;
	}
  if (Rotate_ProcYZ	()	==	ESC)
	  redcrO	(3) ;
  else
	  redcrO	(4) ;
  return	;
}

/*---------Ciagnij--------------------------------------------------*/

static void	set_block_change_mode_drag	(long_long	offp)
/*-----------------------------------------------------*/
{
  long_long offk, off ;
  NAGLOWEK *nag ;
  BLOK *	ptrs_block ;

  ptrs_block =	(BLOK*)(dane +	offp)	;
  offk =	offp + sizeof(NAGLOWEK)	+ ptrs_block->n -	1 ;
  offp += sizeof(NAGLOWEK)	+ B3 + ptrs_block->dlugosc_opisu_obiektu ;
  ObiektTok	(&offp, offk, &off, ONoInBlock) ;
  while (off != -1)
  {
	 nag =(NAGLOWEK*)(dane + off)	;
	 if (nag->obiekt == OdBLOK)
	 {
		 set_block_change_mode_drag (off) ;
	 }
	 else
	 {
		
	  if (nag->atrybut == Ablok &&
		((nag->obiekt == Olinia || nag->obiekt == Oluk) &&
		 (nag->obiektt1 == Utwierdzony1 || nag->obiektt1 == Utwierdzony2)) ||
			  ((nag->obiekt == Owwielokat) && ((WIELOKAT*)nag)->temp1 == OT3W_Drag) ||
		      ((nag->obiekt == Ospline) && ((SPLINE*)nag)->temp1 == OT3W_Drag)
		  )

		{
	if	(ptrs_block->obiektt1 <	OB1CHANGE_DRAG)
	{
	  ptrs_block->obiektt1 = OB1CHANGE_DRAG ;
	}
		}
	 }
	 ObiektTok (&offp, offk, &off, ONoInBlock) ;
  }
}


static void	set_change_mode_drag	(long_long	offp,	long_long offk)
/*-------------------------------------------------------------*/
{
  long_long off ;
  BLOK *	ptrs_block ;

  if (NULL != (ptrs_block = Parent_Block ((char	 *)(dane	+ offp))))
  {
	 offp	= (char *)ptrs_block	- dane ;
  }
  ObiektTok	(&offp, offk, &off, OdBLOK) ;
  while (off != -1)
  {
	 ptrs_block	=(BLOK*)(dane + off)	;
	 set_block_change_mode_drag (off) ;
	 offp	= off	+ sizeof(NAGLOWEK) +	ptrs_block->n ;
	 ObiektTok (&offp, offk, &off, OdBLOK)	;
  }
}

static void	redcrcq(char typ)
{
    static	int np;
    static	int (	*SW[2])();
    switch(typ)
    { case 0	:
            komunikat0(5);
            L.x1=L.x2=X;
            L.y1=L.y2=Y;
            Px=X;Py=Y;
            DX=DY=0;
            eL.x=maxX-PL266;
            eL.y= ESTR_Y;
            eL.lmax=r22;
            eL.val_no_max	= 2 ;
            eL.mode	= GV_VECTOR	;
            eL.format = "%#12.9lg\0%#12.9lg;%#8.3lf"	;
            eL.ESTRF=L_p;
            eL.extend = 0;
            np=dodajstr(&eL);
            outlineor(&L,XOR_PUT,0);
            mvcurbp(1);
            CUR_OFF=cur_offq;
            CUR_ON=cur_onq;
            SW [0] = SERV	[73] ;  SERV [73]	= sel_t ;
            SW [1] = SERV	[81] ;  SERV [81]	= sel_d ;
            break;
        case 1	:
            CUR_OFF(X, Y);
        case 2:
            usunstr(np);
            SERV	[73] = (void*)SW [0] ;
            SERV	[81] = (void*)SW [1] ;
            break;
        default :	break;
    }
}

static void	redcrck(char typ)
{
  static	int np;
  static	int (	*SW[4])();
  double n;
  PLINIA PL, PLth;
  LINIA Lth=Ldef;
  double koc1, kos1, koc1th, kos1th;
  double xmax, ymax;
  int stretch_v;

  switch(typ)
	{ case 0	:
         SW[2]=SERV[71];
         SW[3]=SERV[79];
         CUR_OFF(X, Y);
		 komunikat0(5);

        stretch_v=get_stretch_vector();
        if (stretch_v==1)
         {
             memmove(&VectorC, (AVECTOR*)get_vector_s(), sizeof(AVECTOR));
             SERV[71]=Inverse_VectorC;

             LiniaG.x1=VectorC.x1;
             LiniaG.y1=VectorC.y1;

             LiniaG.x2=VectorC.x2;
             LiniaG.y2=VectorC.y2;

             Px=LiniaG.x2;
             Py=LiniaG.y2;
             DX=LiniaG.x2-LiniaG.x1;
             DY=LiniaG.y2-LiniaG.y1;

             if (VectorC.style==4) //force
             {
                 eVf.x = maxX / 2 + 5;
                 eVf.y = ESTR_Y;
                 eVf.lmax = 16;
                 eVf.val_no_max = 2;
                 eVf.mode = GV_VECTOR;
                 eVf.format = format_float2;
                 eVf.ESTRF = Vf_n_;
                 eVf.extend = 0;
                 np = dodajstr(&eVf);
             }
             else //displacement
             {
                 eVd.x = maxX / 2 + 5;
                 eVd.y = ESTR_Y;
                 eVd.lmax = 16;
                 eVd.val_no_max = 2;
                 eVd.mode = GV_VECTOR;
                 eVd.format = format_float2;
                 eVd.ESTRF = Vd_n_;
                 eVd.extend = 0;
                 np = dodajstr(&eVd);
             }
             CUR_ON=cur_onv;
         }
         else if (stretch_v==2)
            {
                memmove(&VectorC, (AVECTOR*)get_vector_s(), sizeof(AVECTOR));
                SERV[71]=Inverse_VectorC;

                LiniaG.x1=VectorC.x1;
                LiniaG.y1=VectorC.y1;

                LiniaG.x2=VectorC.x1-VectorC.r*cos(VectorC.angle1);
                LiniaG.y2=VectorC.y1+VectorC.r*sin(VectorC.angle1);

                Px=LiniaG.x2;
                Py=LiniaG.y2;
                DX=LiniaG.x2-LiniaG.x1;
                DY=LiniaG.y2-LiniaG.y1;

                eL.x = maxX - PL266;
                eL.y = ESTR_Y;
                eL.lmax = r22;
                eL.val_no_max = 2;
                eL.mode = GV_VECTOR;
                eL.format = "%#12.9lg\0%#12.9lg;%#8.3lf";
                eL.ESTRF = L_p;
                eL.extend = 0;
                np = dodajstr(&eL);
                outlineor(&L, XOR_PUT, 0);

                set_set_arc_stage(2);
                CUR_ON=cur_onv;
            }
         else if (stretch_v==3)
         {
             memmove(&VectorC, (AVECTOR*)get_vector_s(), sizeof(AVECTOR));
             SERV[71]=Inverse_VectorC;

             LiniaG.x1=VectorC.x1;
             LiniaG.y1=VectorC.y1;

             LiniaG.x2=VectorC.x1-VectorC.r*cos(VectorC.angle2);
             LiniaG.y2=VectorC.y1+VectorC.r*sin(VectorC.angle2);

             Px=LiniaG.x2;
             Py=LiniaG.y2;
             DX=LiniaG.x2-LiniaG.x1;
             DY=LiniaG.y2-LiniaG.y1;

             if ((VectorC.style==5) || (VectorC.style==6))//moment
             {
                 eVm.x=maxX/2 + 5 ;
                 eVm.y= ESTR_Y;
                 eVm.lmax=12;
                 eVm.val_no_max	= 1 ;
                 eVm.mode	= GV_DIST	;
                 eVm.format = format_float;
                 eVm.ESTRF=Vm_n_;
                 eVm.extend = 0;
                 np = dodajstr(&eVm);
             }
             else //rotation
             {
                 eVr.x=maxX/2 + 5 ;
                 eVr.y= ESTR_Y;
                 eVr.lmax=12;
                 eVr.val_no_max	= 1 ;
                 eVr.mode	= GV_DIST	;
                 eVr.format = format_floatd;
                 eVr.ESTRF=Vr_n_;
                 eVr.extend = 0;
                 np = dodajstr(&eVr);
             }
             set_set_arc_stage(1);
             CUR_ON=cur_onv;
         }
         else if (stretch_v==4)   //magnitude1
         {
             memmove(&VectorC, (AVECTOR *) get_vector_s(), sizeof(AVECTOR));
             SERV[71]=Inverse_VectorC;

             switch (VectorC.style)
             {
                 case 10:
                     if (VectorC.x1<VectorC.x2) n=1;
                     else n=-1;
                     LiniaG.x1 = VectorC.x1;
                     LiniaG.y1 = VectorC.y1;
                     LiniaG.x2 = VectorC.x1;
                     LiniaG.y2 = VectorC.y1 + n*(VectorC.magnitude1/load_magnitude);
                     parametry_lini(&VectorC, &PL);
                     break;
                 case 11:
                     if (VectorC.y1<VectorC.y2) n=1;
                     else n=-1;

                     LiniaG.x1 = VectorC.x1;
                     LiniaG.y1 = VectorC.y1;
                     LiniaG.x2 = VectorC.x1 - n*(VectorC.magnitude1/load_magnitude);
                     LiniaG.y2 = VectorC.y1;
                     break;
                 case 12:
                     parametry_lini(&VectorC, &PL);

                     kos1=sin(Pi*(PL.kat+90)/180);
                     koc1=cos(Pi*(PL.kat+90)/180);

                     LiniaG.x1 = VectorC.x1;
                     LiniaG.y1 = VectorC.y1;
                     LiniaG.x2 = VectorC.x1 + (VectorC.magnitude1/load_magnitude)*koc1;
                     LiniaG.y2 = VectorC.y1 + (VectorC.magnitude1/load_magnitude)*kos1;
                     break;
                 case 13:
                     if (VectorC.x1<VectorC.x2)
                     {
                         n=1;
                         ymax=max(VectorC.y1, VectorC.y2);
                     }
                     else
                     {
                         n=-1;
                         ymax=min(VectorC.y1, VectorC.y2);
                     }

                     LiniaG.x1 = VectorC.x1;
                     LiniaG.y1 = ymax;
                     LiniaG.x2 = VectorC.x1;
                     LiniaG.y2 = ymax + n*(VectorC.magnitude1/load_magnitude);
                     break;
                 case 14:
                     if (VectorC.y1<VectorC.y2)
                     {
                         n=1;
                         xmax=min(VectorC.x1, VectorC.x2);
                     }
                     else
                     {
                         n=-1;
                         xmax=max(VectorC.x1, VectorC.x2);
                     }

                     LiniaG.x1 = xmax;
                     LiniaG.y1 = VectorC.y1;
                     LiniaG.x2 = xmax - n*(VectorC.magnitude1/load_magnitude);
                     LiniaG.y2 = VectorC.y1;
                     break;
                 case 15:
                     parametry_lini(&VectorC, &PL);

                     kos1=sin(Pi*(PL.kat+90)/180);
                     koc1=cos(Pi*(PL.kat+90)/180);

                     Lth.x1 = (VectorC.x1+VectorC.x2)/2 + (VectorC.r/depth_magnitude)*koc1;   //thermal_depth_size
                     Lth.y1 = (VectorC.y1+VectorC.y2)/2 + (VectorC.r/depth_magnitude)*kos1;
                     Lth.x2 = (VectorC.x1+VectorC.x2)/2 - (VectorC.r/depth_magnitude)*koc1;
                     Lth.y2 = (VectorC.y1+VectorC.y2)/2 - (VectorC.r/depth_magnitude)*kos1;

                     parametry_lini(&Lth, &PLth);
                     kos1th=sin(Pi*(PLth.kat+90)/180);
                     koc1th=cos(Pi*(PLth.kat+90)/180);

                     LiniaG.x1 = Lth.x1;
                     LiniaG.y1 = Lth.y1;
                     LiniaG.x2 = Lth.x1 + (VectorC.magnitude1/thermal_magnitude)*koc1th;
                     LiniaG.y2 = Lth.y1 + (VectorC.magnitude1/thermal_magnitude)*kos1th;
                     break;
             }
             eVf.x=maxX/2 + 5 ;
             eVf.y= ESTR_Y;
             eVf.lmax=12;
             eVf.val_no_max	= 1 ;
             eVf.mode	= GV_DIST	;
             eVf.format = format_float;
             eVf.ESTRF=Vf1_n_;
             eVf.extend = 0;
             np = dodajstr(&eVf);

             set_eVa(&eVf);

             CUR_ON=cur_onv1;
         }
         else if (stretch_v==5)   //magnitude2
         {
             memmove(&VectorC, (AVECTOR *) get_vector_s(), sizeof(AVECTOR));
             SERV[71]=Inverse_VectorC;

             switch (VectorC.style)
             {
                 case 10:
                     if (VectorC.x1<VectorC.x2) n=1;
                     else n=-1;
                     LiniaG.x1 = VectorC.x2;
                     LiniaG.y1 = VectorC.y2;
                     LiniaG.x2 = VectorC.x2;
                     LiniaG.y2 = VectorC.y2 + n*(VectorC.magnitude2/load_magnitude);
                     break;
                 case 11:
                     if (VectorC.y1<VectorC.y2) n=1;
                     else n=-1;

                     LiniaG.x1 = VectorC.x2;
                     LiniaG.y1 = VectorC.y2;
                     LiniaG.x2 = VectorC.x2 - n*(VectorC.magnitude2/load_magnitude);
                     LiniaG.y2 = VectorC.y2;
                     break;
                 case 12:
                     parametry_lini(&VectorC, &PL);

                     kos1=sin(Pi*(PL.kat+90)/180);
                     koc1=cos(Pi*(PL.kat+90)/180);

                     LiniaG.x1 = VectorC.x2;
                     LiniaG.y1 = VectorC.y2;
                     LiniaG.x2 = VectorC.x2 + (VectorC.magnitude2/load_magnitude)*koc1;
                     LiniaG.y2 = VectorC.y2 + (VectorC.magnitude2/load_magnitude)*kos1;
                     break;
                 case 13:
                     if (VectorC.x1<VectorC.x2)
                     {
                         n=1;
                         ymax=max(VectorC.y1, VectorC.y2);
                     }
                     else
                     {
                         n=-1;
                         ymax=min(VectorC.y1, VectorC.y2);
                     }

                     LiniaG.x1 = VectorC.x2;
                     LiniaG.y1 = ymax;
                     LiniaG.x2 = VectorC.x2;
                     LiniaG.y2 = ymax + n*(VectorC.magnitude2/load_magnitude);
                     break;
                 case 14:
                     if (VectorC.y1<VectorC.y2)
                     {
                         n=1;
                         xmax=min(VectorC.x1, VectorC.x2);
                     }
                     else
                     {
                         n=-1;
                         xmax=max(VectorC.x1, VectorC.x2);
                     }

                     LiniaG.x1 = xmax;
                     LiniaG.y1 = VectorC.y2;
                     LiniaG.x2 = xmax - n*(VectorC.magnitude2/load_magnitude);
                     LiniaG.y2 = VectorC.y2;
                     break;
                 case 15:
                     parametry_lini(&VectorC, &PL);

                     kos1=sin(Pi*(PL.kat+90)/180);
                     koc1=cos(Pi*(PL.kat+90)/180);

                     Lth.x1 = (VectorC.x1+VectorC.x2)/2 + (VectorC.r/depth_magnitude)*koc1;  //thermal_depth_size
                     Lth.y1 = (VectorC.y1+VectorC.y2)/2 + (VectorC.r/depth_magnitude)*kos1;
                     Lth.x2 = (VectorC.x1+VectorC.x2)/2 - (VectorC.r/depth_magnitude)*koc1;
                     Lth.y2 = (VectorC.y1+VectorC.y2)/2 - (VectorC.r/depth_magnitude)*kos1;

                     parametry_lini(&Lth, &PLth);
                     kos1th=sin(Pi*(PLth.kat+90)/180);
                     koc1th=cos(Pi*(PLth.kat+90)/180);

                     LiniaG.x1 = Lth.x2;
                     LiniaG.y1 = Lth.y2;
                     LiniaG.x2 = Lth.x2 + (VectorC.magnitude2/thermal_magnitude)*koc1th;
                     LiniaG.y2 = Lth.y2 + (VectorC.magnitude2/thermal_magnitude)*kos1th;
                     break;
             }
             eVf.x=maxX/2 + 5 ;
             eVf.y= ESTR_Y;
             eVf.lmax=12;
             eVf.val_no_max	= 1 ;
             eVf.mode	= GV_DIST	;
             eVf.format = format_float;
             eVf.ESTRF=Vf2_n_;
             eVf.extend = 0;
             np = dodajstr(&eVf);

             set_eVa(&eVf);

             CUR_ON=cur_onv2;
         }
         else if (stretch_v==6)   //thermal bar side 1
         {
             memmove(&VectorC, (AVECTOR *) get_vector_s(), sizeof(AVECTOR));
             if (VectorC.style==15)
             {
                 parametry_lini(&VectorC, &PL);

                 kos1=sin(Pi*(PL.kat+90)/180);
                 koc1=cos(Pi*(PL.kat+90)/180);

                 Lth.x1 = (VectorC.x1+VectorC.x2)/2 + (VectorC.r/depth_magnitude)*koc1;  //thermal_depth_size
                 Lth.y1 = (VectorC.y1+VectorC.y2)/2 + (VectorC.r/depth_magnitude)*kos1;
                 Lth.x2 = (VectorC.x1+VectorC.x2)/2 - (VectorC.r/depth_magnitude)*koc1;
                 Lth.y2 = (VectorC.y1+VectorC.y2)/2 - (VectorC.r/depth_magnitude)*kos1;

                 LiniaG.x1 = (Lth.x1+Lth.x2)/2.0;
                 LiniaG.y1 = (Lth.y1+Lth.y2)/2.0;
                 LiniaG.x2 = Lth.x2;
                 LiniaG.y2 = Lth.y2;

                 eVf.x=maxX/2 + 5 ;
                 eVf.y= ESTR_Y;
                 eVf.lmax=12;
                 eVf.val_no_max	= 1 ;
                 eVf.mode	= GV_DIST	;
                 eVf.format = format_float;
                 eVf.ESTRF=Vth1_n_;
                 eVf.extend = 0;
                 np = dodajstr(&eVf);

                 set_eVa(&eVf);

                 CUR_ON=cur_onv3;
             }
         }
         else if (stretch_v==7)   //thermal bar side 2
         {
             memmove(&VectorC, (AVECTOR *) get_vector_s(), sizeof(AVECTOR));
             if (VectorC.style==15)
             {
                 parametry_lini(&VectorC, &PL);

                 kos1=sin(Pi*(PL.kat+90)/180);
                 koc1=cos(Pi*(PL.kat+90)/180);

                 Lth.x1 = (VectorC.x1+VectorC.x2)/2 + (VectorC.r/depth_magnitude)*koc1;  //thermal_depth_size
                 Lth.y1 = (VectorC.y1+VectorC.y2)/2 + (VectorC.r/depth_magnitude)*kos1;
                 Lth.x2 = (VectorC.x1+VectorC.x2)/2 - (VectorC.r/depth_magnitude)*koc1;
                 Lth.y2 = (VectorC.y1+VectorC.y2)/2 - (VectorC.r/depth_magnitude)*kos1;

                 LiniaG.x1 = (Lth.x1+Lth.x2)/2.0;
                 LiniaG.y1 = (Lth.y1+Lth.y2)/2.0;
                 LiniaG.x2 = Lth.x1;
                 LiniaG.y2 = Lth.y1;

                 eVf.x=maxX/2 + 5 ;
                 eVf.y= ESTR_Y;
                 eVf.lmax=12;
                 eVf.val_no_max	= 1 ;
                 eVf.mode	= GV_DIST	;
                 eVf.format = format_float;
                 eVf.ESTRF=Vth2_n_;
                 eVf.extend = 0;
                 np = dodajstr(&eVf);

                 set_eVa(&eVf);

                 CUR_ON=cur_onv4;
             }
         }
         else if (stretch_v==8)   //stretching radius of node
         {
             memmove(&VectorC, (AVECTOR*)get_vector_s(), sizeof(AVECTOR));

             LiniaG.x1=VectorC.x1;
             LiniaG.y1=VectorC.y1;

             LiniaG.x2=VectorC.x1-VectorC.r*cos(0);
             LiniaG.y2=VectorC.y1+VectorC.r*sin(0);

             Px=LiniaG.x2;
             Py=LiniaG.y2;
             DX=LiniaG.x2-LiniaG.x1;
             DY=LiniaG.y2-LiniaG.y1;

             eVn.x=maxX/2 + 5 ;
             eVn.y= ESTR_Y;
             eVn.lmax=12;
             eVn.val_no_max	= 1 ;
             eVn.mode	= GV_DIST	;
             eVn.format = format_float;
             eVn.ESTRF=Vn_n_;
             eVn.extend = 0;
             np = dodajstr(&eVn);


             CUR_ON=cur_onv5;

         }
         else
         {
             if (stretch_v==-1)
             {
                 SERV[71]=Inverse_VectorP;
                 SERV[79]=Reverse_VectorP;
                 reset_stretch_vector();
             }

             L.x1=L.x2=X;
             L.y1=L.y2=Y;
             Px=X;Py=Y;
             DX=DY=0;

             eL.x = maxX - PL266;
             eL.y = ESTR_Y;
             eL.lmax = r22;
             eL.val_no_max = 2;
             eL.mode = GV_VECTOR;
             eL.format = "%#12.9lg\0%#12.9lg;%#8.3lf";
             eL.ESTRF = L_p;
             eL.extend = 0;
             np = dodajstr(&eL);
             outlineor(&L, XOR_PUT, 0);

             CUR_ON=cur_onk;
             ////reset_trace_block();
         }

		 mvcurbp(1);
		 CUR_OFF=cur_offk;
		 SW [0] = SERV	[73] ;  SERV [73]	= sel_t ;
		 SW [1] = SERV	[81] ;  SERV [81]	= sel_d ;
         CUR_ON(X,Y);
		 break;
	  case 1	:
		 CUR_OFF(X, Y);
	  case 2:
          reset_eVa();
		 usunstr(np);
		 SERV[73] = (void*)SW [0] ;
		 SERV[81] = (void*)SW [1] ;
         SERV[71] = (void*)SW [2];
         SERV[79] = (void*)SW [3];
		 break;
	  default :	break;
	}
}

static int ciagnijk_quad(void)
{
    double	X0,Y0;
    EVENT *ev;

    redcrcq(0);
    strwyj	= 0;
    while(1)

    {ev=Get_Event_Point(NULL,	&X0, &Y0);
        if(ev->What == evKeyDown	&&	 ev->Number==0) {	redcrcq(1);	return ESC;	}
        if((ev->What	==	evKeyDown &&  ev->Number== ENTER)	||	strwyj)
        { Cur_offd(X,Y);
            blokzap(ADP,ADK,Ablok,COPY_PUT,0);
            if(strwyj)
            {
                DX = DX0;	DY	= DY0;
            }
            transformacja_blok(ADP,ADK,DX,DY,0,0,Tprzesuw,0);
            blokzap(ADP,ADK,Ablok,COPY_PUT,1);
            redcrcq(2);
            return ENTER;
        }
    }
}

static int ciagnijk(void)
{
  double	X0,Y0;
  EVENT *ev;

  redcrck(0);
  strwyj	= 0;
  while(1)
	
	 {ev=Get_Event_Point(NULL,	&X0, &Y0);
	  if(ev->What == evKeyDown	&&	 ev->Number==0)
      {	redcrck(1);
          return ESC;
      }
	  if((ev->What	==	evKeyDown &&  ev->Number== ENTER)	||	strwyj)
		{ Cur_offd(X,Y);
	blokzap(ADP,ADK,Ablok,COPY_PUT,0);
	if(strwyj)
	{
	  DX = DX0;	DY	= DY0;
	}
	transformacja_blok(ADP,ADK,DX,DY,0,0,Tprzesuw,0);

    reset_stretch_vector();

    reset_trace_block();
	blokzap(ADP,ADK,Ablok,COPY_PUT,0);
    ////In case of traces, should be deep refresh of entire trace
    blokzap_deep(ADP,ADK,Ablok,COPY_PUT,1);
    //flip_screen();
	redcrck(2);
	return ENTER;
		}
	}
}

/*----dzi,odzi-------*/

static void	utwierdzenie_linii(LINIA *adl)
{ int	w;
  w=linia_wybrana(adl);
  if (w)
	{ if(adl->obiektt1!=Sztywny)
	  {if	((adl->blok	!=	ElemBlok) || (swobodny_blok == TRUE))
		{
		 switch(w)
	  { case	1 : if (adl->obiektt1==Utwierdzony1) adl->obiektt1=Guma;
            else adl->obiektt1=Utwierdzony2;
          break;
		 case	2 : if (adl->obiektt1==Utwierdzony2) adl->obiektt1=Guma;
            else adl->obiektt1=Utwierdzony1;
         break;
		 case	3 :
		 case	4 : adl->obiektt1=Guma;
         break;
		 default	: break;
	  }
		}
		else 
		{ 
		 if ((adl->obiektt2==O2BlockDim)	||	(adl->obiektt2==O2BlockPline))
		 {
		 switch(w)
	  { case	1 : adl->obiektt1=Utwierdzony2;break;
		 case	2 : adl->obiektt1=Utwierdzony1;break;
		 case	3 :
		 case	4 : adl->obiektt1=Guma;break;
		 default	: break;
	  }
		 }
		 else
			{
			 switch(w)
	  { case	1 : 
		 case	2 : 
		 case	3 :
		 case	4 : adl->obiektt1=Guma;break;
		 default	: break;
	  }
			}
		}	
	  }	  
	  
	}
}

static void	utwierdzenie_vector(AVECTOR *adl)
{ int	w;
    if (get_stretch_vector()==9) return;  //edit load character

    if (get_stretch_vector()<4)
       w=vector_drag_wybrany(adl);
    else return;  //in case of 4 and 5 it's not needed
    if (w)
    { if(adl->obiektt1!=Sztywny)
        {if	((adl->blok	!=	ElemBlok) || (swobodny_blok == TRUE))
            {
                switch(w)
                { case	1 :
                        switch (adl->style)
                        {
                            case 4:
                            case 7:
                                adl->obiektt1 = Guma;
                                break;
                            default:
                                if (adl->obiektt1 == Utwierdzony1) adl->obiektt1 = Guma;
                                else adl->obiektt1 = Utwierdzony2;
                                break;
                        }
                        break;
                    case	2 :
                        switch (adl->style)
                        {
                            case 4:
                            case 7:
                                if (get_stretch_vector()<=0) {
                                    adl->atrybut = Anormalny;  //!!!!!!!!!!!!!!!!!
                                    adl->przec=NoAblokC;
                                }
                                else {
                                    if (adl->obiektt1 == Utwierdzony2) adl->obiektt1 = Guma;
                                    else adl->obiektt1 = Utwierdzony1;
                                }
                                break;
                            case 5:
                            case 6:
                            case 8:
                            case 9:
                                if (get_stretch_vector()<=0) {
                                    adl->atrybut = Anormalny;  //!!!!!!!!!!!!!!!!!
                                    adl->przec=NoAblokC;
                                }
                                else if (get_stretch_vector()==2)
                                {
                                    if (adl->obiektt1 == Utwierdzony1) adl->obiektt1 = Guma;
                                    else adl->obiektt1 = Utwierdzony2;
                                }
                                else
                                {
                                    if (adl->obiektt1 == Utwierdzony2) adl->obiektt1 = Guma;
                                    else adl->obiektt1 = Utwierdzony1;
                                }
                            break;
                            default:
                                if (adl->obiektt1==Utwierdzony2) adl->obiektt1=Guma;
                                else adl->obiektt1=Utwierdzony1;
                            break;
                        }
                        break;
                    case	3 :
                    case	4 : adl->obiektt1=Guma;
                        break;
                    case 16:
                        if (get_stretch_vector()==8)
                        {
                            if (adl->obiektt1 == Utwierdzony1) adl->obiektt1 = Guma;
                            else adl->obiektt1 = Utwierdzony2;
                        }
                        break;
                    default	: break;
                }
            }
        }
    }
    else
    {
        adl->atrybut = Anormalny;  //!!!!!!!!!!!!!!!!!
        adl->przec=NoAblokC;
    }
}

static void	utwierdzenie_luku(LUK *adluk)
{ int	w1,w2;
  double	x,y;
  if(adluk->obiektt1!=Sztywny)
  {if	((adluk->blok != ElemBlok)	||	(swobodny_blok	==	TRUE))
	 {
	  x=adluk->x+adluk->r*cos(adluk->kat1);
	  y=adluk->y+adluk->r*sin(adluk->kat1);
	  w1=punkt_w_prostokacie(x,y);
	  x=adluk->x+adluk->r*cos(adluk->kat2);
	  y=adluk->y+adluk->r*sin(adluk->kat2);
	  w2=punkt_w_prostokacie(x,y);
	  if(w1 && !w2) adluk->obiektt1=Utwierdzony2;
	  else if(!w1 && w2)	adluk->obiektt1=Utwierdzony1;
	  else adluk->obiektt1=Guma;
	 }
  }  
}

static void	utwierdzenie_solidarc(SOLIDARC *adsa)
{ int	w1,w2;
    double	x,y;
    if(adsa->obiektt1!=Sztywny)
    {if	((adsa->blok != ElemBlok)	||	(swobodny_blok	==	TRUE))
        {
            x=adsa->x+adsa->r*cos(adsa->kat1);
            y=adsa->y+adsa->r*sin(adsa->kat1);
            w1=punkt_w_prostokacie(x,y);
            x=adsa->x+adsa->r*cos(adsa->kat2);
            y=adsa->y+adsa->r*sin(adsa->kat2);
            w2=punkt_w_prostokacie(x,y);
            if(w1 && !w2) adsa->obiektt1=Utwierdzony2;
            else if(!w1 && w2)	adsa->obiektt1=Utwierdzony1;
            else adsa->obiektt1=Guma;
        }
    }
}

static void	utwierdzenie_lukueliptycznego(ELLIPTICALARC *ea)
{  int	w1,w2;
    double	xs,ys,xe,ye;
    if(ea->obiektt1!=Sztywny)
    {if	((ea->blok != ElemBlok)	||	(swobodny_blok	==	TRUE))
        {
            Get_EllipticalArc_EndPoints (ea->x, ea->y, ea->rx, ea->ry, ea->angle, ea->kat1, ea->kat2, &xs, &ys, &xe, &ye);
            w1=punkt_w_prostokacie(xs,ys);
            w2=punkt_w_prostokacie(xe,ye);
            if(w1 && !w2) ea->obiektt1=Utwierdzony2;
            else if(!w1 && w2)	ea->obiektt1=Utwierdzony1;
            else ea->obiektt1=Guma;
        }
    }
}

static void	utwierdzenie_wielokata (WIELOKAT* adw)
/*---------------------------------------------*/
{
				/*zaklada sie ze obiekt jest wybrany*/
  adw->temp1 =	OT3W_Drag ;
  if (wielokat_w_prostokacie (adw))
  {
	 adw->temp1	= OT3W_Move;
  }
  else
  {
	if	((adw->blok	==	ElemBlok) && (swobodny_blok == FALSE) &&
		(adw->obiektt2!=O2BlockDim) && (adw->obiektt2!=O2BlockPline))
	 {
	  adw->temp1 =	OT3W_Move;
	 }	  
  }  
}

static void	utwierdzenie_spline(SPLINE* adw)
/*---------------------------------------------*/
{
	/*zaklada sie ze obiekt jest wybrany*/
	adw->temp1 = OT3W_Drag;
	if (spline_w_prostokacie(adw))
	{
		adw->temp1 = OT3W_Move;
	}
	else
	{
		if ((adw->blok == ElemBlok) && (swobodny_blok == FALSE) && (adw->obiektt2 != O2BlockPline))
		{
			adw->temp1 = OT3W_Move;
		}
	}
}

static void	test_utwierdzenia(void *ad, OKNO	*O)
{ NAGLOWEK *nag;
  oknoS(O->x01,O->y01,O->x02,O->y02);
  nag=(NAGLOWEK*)ad;
  switch(nag->obiekt)
	{ case Olinia :
	utwierdzenie_linii((LINIA*)ad);
	break;
	  case Oluk	:
	utwierdzenie_luku((LUK*)ad);
	break;
    case Osolidarc	:
    utwierdzenie_solidarc((SOLIDARC*)ad);
    break;
    case Oellipticalarc	:
    utwierdzenie_lukueliptycznego((ELLIPTICALARC*)ad);
    break;
	  case Owwielokat	:
	utwierdzenie_wielokata ((WIELOKAT*)ad);
	break;
	 case Ospline:
     utwierdzenie_spline((SPLINE*)ad);
	 break;
     case Ovector :
     utwierdzenie_vector((AVECTOR*)ad);
      break;
	  default :	break;
	}
}

int dzi_q(void *ad)
{  BOOL found=FALSE;
    int i;

    rysuj_obiekt(ad,XOR_PUT,1);

    ((NAGLOWEK*)ad)->atrybut=Ablok;
    return 1;

}

int odzi_q(void *ad)
{   int i, j;

    rysuj_obiekt(ad,COPY_PUT,1);
    ((NAGLOWEK*)ad)->atrybut=Anormalny;
    return 1;

}


int edit_load_character(AVECTOR *v)
{   int poz, poz1;
    int m_level;
    int n0, n1, n2;

    if (v->style==15) //thermal
    {
        if (v->load>0) poz=1;
        else poz=0;

        if (v->variant>0) poz1=(v->variant);
        else poz1=0;

        if ((mLoad_Char_Thermal.maxw==0) || (mLoad_Char_Thermal.maxw>mLoad_Char_Thermal.poz))
        {
            mLoad_Char_Thermal.foff=0;
            mLoad_Char_Thermal.poz=poz;
        }
        else
        {
            mLoad_Char_Thermal.foff=poz - mLoad_Char_Thermal.maxw + 1;
            mLoad_Char_Thermal.poz=mLoad_Char_Thermal.maxw-1;
        }

        if ((mLoadVariant.maxw==0) || (mLoadVariant.maxw>mLoadVariant.poz))
        {
            mLoad_Char_Thermal.foff=0;
            mLoad_Char_Thermal.poz=poz1;
        }
        else
        {
            mLoadVariant.foff=poz1 - mLoadVariant.maxw + 1;
            mLoadVariant.poz=mLoadVariant.maxw-1;
        }

        m_level=get_menu_level();
        m_level++;
        set_menu_level(m_level);

        menu_address[m_level-1]=&mLoad_Char_Thermal;
        n0 = Simple_Menu_Proc(&mLoad_Char_Thermal);

        if (n0)
        {
            n1 = mLoad_Char_Thermal.foff + mLoad_Char_Thermal.poz;
            n2 = mLoadVariant.foff + mLoadVariant.poz;

            if (n1 > 0) v->load = 10;
            else v->load = 0;
            if (v->load == 0) v->variant = 0;
            else v->variant = n2;

            Change = TRUE;

        }
    }
    else
    {
        poz=v->load;
        if (v->variant>0) poz1=(v->variant);
        else poz1=0;

        if ((mLoad_Char.maxw==0) || (mLoad_Char.maxw>mLoad_Char.poz))
        {
            mLoad_Char.foff=0;
            mLoad_Char.poz=poz;
        }
        else
        {
            mLoad_Char.foff=poz - mLoad_Char.maxw + 1;
            mLoad_Char.poz=mLoad_Char.maxw-1;
        }

        if ((mLoadVariant.maxw==0) || (mLoadVariant.maxw>mLoadVariant.poz))
        {
            mLoad_Char_Thermal.foff=0;
            mLoad_Char_Thermal.poz=poz1;
        }
        else
        {
            mLoadVariant.foff=poz1 - mLoadVariant.maxw + 1;
            mLoadVariant.poz=mLoadVariant.maxw-1;
        }

        m_level=get_menu_level();
        m_level++;
        set_menu_level(m_level);

        menu_address[m_level-1]=&mLoad_Char;
        n0 = Simple_Menu_Proc(&mLoad_Char);

        if (n0)
        {
            n1 = mLoad_Char.foff + mLoad_Char.poz;
            n2 = mLoadVariant.foff + mLoadVariant.poz;

            v->load = n1;
            if (v->load == 0) v->variant = 0;
            else v->variant = n2;

            Change = TRUE;

        }
    }
}

int dzic(void *ad)
{
  BLOK *b;

  ((NAGLOWEK*)ad)->atrybut=Ablok;
  ((NAGLOWEK*)ad)->przec=AblokC;
  test_utwierdzenia(ad,	Get_Block_Window ());
  if (((NAGLOWEK*)ad)->atrybut==Ablok)
  {
      ////b=FIRSTB(ad);
      ////if (b!=NULL) rysuj_obiekt(b,XOR_PUT,1); ////
      rysuj_obiekt(ad,XOR_PUT,1);
  }
  return 1;
}

int dzic_o(void *ad)
{   double x, y;

    ((NAGLOWEK*)ad)->atrybut=Ablok;
    ((NAGLOWEK*)ad)->przec=AblokC;
    x=X; y=Y;
    set_global_set_stretch(TRUE);
    qkoniec_o(&x, &y, ad);
    set_global_set_stretch(FALSE);
    if ((!isnan(x)) && (!isnan(y)))
    {
        o.x1 = x;
        o.y1 = y;
        o.x2 = x;
        o.y2 = y;
        o.x01 = x;
        o.y01 = y;
        o.x02 = x;
        o.y02 = y;
        Set_Block_Window(&o);
        test_utwierdzenia(ad, Get_Block_Window());
        ADP=ad; ADK= (char*)ad + ((NAGLOWEK *) ad)->n + sizeof(NAGLOWEK) - 1;
        if (((NAGLOWEK *) ad)->atrybut == Ablok)
        {
            if (get_stretch_vector() >= 8)  //it's necessary to dim text
            {
                rysuj_obiekt(ad, NOT_PUT, 1);
            }
            else rysuj_obiekt(ad, XOR_PUT, 1);
        }

        if (get_stretch_vector() == 9)  //edit
        {

            ((NAGLOWEK*)ad)->atrybut=Anormalny;
            ((NAGLOWEK*)ad)->przec=NoAblokC;

            edit_load_character((AVECTOR *)ad);

            rysuj_obiekt(ad, COPY_PUT, 1);

            CUR_OFF(X,Y);
            CUR_ON(X,Y);

            alert_on_object=TRUE;
            return 0;
        }

        return 1;
    }
    else
    {
        ((NAGLOWEK*)ad)->atrybut=Anormalny;
        ((NAGLOWEK*)ad)->przec=NoAblokC;
        alert_on_object=TRUE;
        return 0;
    }
}

int dzic_q(void *ad)
{   BLOK *bl=NULL;
    BOOL rectangle=FALSE;
    int i,j;

    if (((NAGLOWEK*)ad)->obiekt==Olinia)
    {
        bl=(LASTB(ad));
        if (bl!=NULL)
        {
            if ((bl->kod_obiektu=B_PLINE) && (bl->opis_obiektu[0]==PL_RECTANGLE))
                rectangle=TRUE;
        }
    }

    if ((rectangle==TRUE) || (((NAGLOWEK*)ad)->obiekt==Owwielokat)) {
        rysuj_obiekt(ad, XOR_PUT, 1);
        ((NAGLOWEK *) ad)->atrybut = Ablok;
        ((NAGLOWEK *) ad)->przec = AblokC;
        test_utwierdzenia(ad, Get_Block_Window());

        return 1;
    }
    else return 0;
}

int dzic2(void* ad)
{
	if (((NAGLOWEK*)ad)->przec == AblokC)
	{
		rysuj_obiekt(ad, XOR_PUT, 1);
		((NAGLOWEK*)ad)->atrybut = Ablok;
	}
	return	0;
}

int odzic2(void *ad)
{ if(((NAGLOWEK*)ad)->przec==AblokC)
	{
        //rysuj_obiekt(ad,XOR_PUT,1);
        rysuj_obiekt(ad,COPY_PUT,1);
	  ((NAGLOWEK*)ad)->atrybut=Anormalny;
	}
  return	0;
}

void redrawbc(void)
{ outokno (Get_Stretch_Window	(), 0, 0) ;
  blokzap(ADP,ADK,Ablok,XOR_PUT,1);
}

/*---------*/


static void	normalize_par_objectt1 (void)
{
  NAGLOWEK *nag ;

  obiekt_tok ((char *)ADP,	ADK, (char **)&nag, ONieOkreslony) ;
  while (nag != NULL)
  {
	  if ((nag->obiekt == Olinia || nag->obiekt == Ovector || nag->obiekt == Oluk) &&
	 (nag->obiektt1 == Utwierdzony1 || nag->obiektt1 == Utwierdzony2))
	  {
		 nag->obiektt1	= Guma ;
	  }
	  obiekt_tok (NULL, ADK, (char **)&nag, ONieOkreslony) ;
	}
}


static void	redcrC(char	typ)
{ static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt, sel_cur, sel_gor;
  static    int iconno;
  long_long off,	offk ;
 
  
  switch(typ)
	{ case 0	:
		Cur_offd(X, Y);
		komunikat(0);
		komunikat0(34);
		sel_akt=sel.akt;	sel.akt=1;
		win0 = Block_Proc_Win_Mode_Ptr () ;
		if	(*win0 == Windoww)
		 {
		  win00 = Windoww ;
		 }
		  else
		  {
			win00	= Cross ;
		  }
		*(Block_Proc_Win_Mode_Ptr ())	= Dragg ;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		Set_Stretch_Mode (TRUE)	;
		CUR_oFF = CUR_OFF; CUR_OFF = out_sel_off; //cursel_off; // out_sel;
		CUR_oN = CUR_ON;   CUR_ON = out_sel_on; //cursel_on; // out_sel;
		SW[0] = SERV[73];  SERV[73] = sel_t;
		SW[1] = SERV[81];  SERV[81] = sel_d;

        getmenupini(&menu,st,&cod,&iconno);
        menupini(&mSelectMode, _BLOCK_SELECT_, _BLOCK_SELECT_C_, 721);

		Cur_ond(X, Y);
		break;
	  case 1	:
        menupini(menu,st,cod,iconno);

		komunikat0(0);
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Win_Mode_Ptr ())	= Cross ;
		*(Block_Proc_Add_Mode_Ptr ())	= DEL_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod,&iconno);
        break;
      case 11	:
            menupini(menu,st,cod,iconno);
            komunikat0(0);
            getmenupini(&menu,st,&cod,&iconno);
        break;
	  case 2	:
		SERV[def67]= (void*)SW[0];
		SERV[def68]= (void*)SW[1];
		menupini(menu,st,cod,iconno);
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		komunikat0(4);
		break;
	  case 3	: 
	  case 4	:
				if ((ADP!=NULL)	&&	(ADK!=NULL))
                {
                    blokzap(ADP, ADK, Ablok, COPY_PUT, 0);   //XOR_PUT
                    blokzap_deep(ADP, ADK, Ablok, COPY_PUT, 1);   //XOR_PUT
                }
	  case 5	:
			  if ((ADP!=NULL) && (ADK!=NULL))					 
			 {
				off =	ADP -	dane ;
				offk = ADK - dane	;
				if	(typ == 5)
				{
				 set_change_mode_drag	(off,	offk)	;
				}
				Normalize_Objects	(off,	offk,	Ablok) ;	  	/*normalizacja obszaru*/

			  } 
		  break;
	  case 6	: 
		      CUR_OFF(X, Y);
			  break;
     case 7 : 
		      CUR_OFF(X,Y);
              break;
	  default: break;
	}
  if(typ==3	||	typ==4 || typ==5 || typ==6)
	{
      menupini(menu,st,cod,iconno);

      oknoS(Xp,Yp,Xk,Yk);

	  CUR_OFF=CUR_oFF;
	  CUR_ON=CUR_oN;
	  Set_Stretch_Mode (FALSE)	;
	  Set_Object_Temp_Flag (NoAblokC, 0L, dane_size	- 1) ;
	  *win0 = win00 ;
	  sel.akt=sel_akt;
	  normalize_par_objectt1 () ;

      if (dragging_quad==TRUE) {
          if (typ == 5) {
              blokzap(ADPQ, ADKQ, Aoblok, COPY_PUT, 0);
              blokquadfix(ADPQ, ADKQ, Aoblok, COPY_PUT, 1);
          } else blokzap(ADPQ, ADKQ, Aoblok, COPY_PUT, 1);

          zmien_atrybut(ADP, ADK, Ablok, Anormalny);
      }
      else //if (typ==5)
      {
          memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
          zmien_atrybut_undo(dane, dane + dane_size);
          zmien_atrybut(ADP, ADK, Ablok, Aoblok);
      }

      CUR_OFF(X,Y);
      CUR_ON(X,Y);
	  komunikat	(0) ;
	  komunikat0 (0) ;
	}
}


void ciagnij(void)
{ int	ret_blokc;
    int a;
    unsigned u__type_object, u__last_type_object;
    void *ptr__object  ; /*adres wskazanego obiektu, dla polilinii adres polilinii*/

    TTF_redraw = FALSE;

    control_points_view_on();

    redcrC(0);

blokc_again:


    reset_trace_block();

    if (selection==SELECT_CROSS)
    {
        ret_blokc = blokc(dzic, COMNDmbs);
    }
    else
    {
        reset_stretch_vector();
        ret_blokc = blok_single(dzic_o,odzic2,Redraw_Block,COMNDmbs);
    }

    if (ret_blokc==-1)
    {
        goto blokc_again;
    }

    if(ret_blokc==0)
    {   redcrC(6);
        control_points_view_off();
        return;
    }

    if (selection==SELECT_CROSS)
    {
        redcrC(1);
        blok(dzic, odzic2, redrawbc, COMNDmbc);

        if (memcmp(Get_Stretch_Window(), Get_Block_Window(), sizeof(OKNO)) != 0) {
            a = 1;  //O was added to Oc and it is necessary to make array of rectangles   TO DO
        }

        redcrC(2);
        if (przesunp()==ESC)
        { redcrC(3);
            control_points_view_off();
            return;
        }

    }
    else
    {
        redcrC(11);

        DX=o.x01-X; DY=o.y01-Y;
        if (DX!=0 || DY!=0) mvcurp(DX,DY); //MOVING CURSOR

        memmove(Get_Stretch_Window(), Get_Block_Window(), sizeof(OKNO));
    }

    if (ciagnijk() == ESC)
        redcrC(4);
    else
        redcrC(5);

	if (selection != SELECT_CROSS)
	{
		if (TTF_redraw) redraw();
		TTF_redraw = FALSE;
		redcrC(0);
		goto blokc_again;
	}
    control_points_view_off();
    if (TTF_redraw) redraw();
    return;
}


void mysort(char* p[],int n)
{   char *temp;
    int i,j;
    for(i=0; i<n-1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if ((char *) p[j] < (char *) p[j + 1]) {
                temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

int blokq(int (*DZI)(void *), int (*ODZI)(void *))
{
    EVENT *ev;
    double X0, Y0;
    int ret_oknok;

    redcrq(0);
    blok(DZI,ODZI,Redraw_Block,COMNDmb);
    redcrq(1);

}

void find_adpq_adkq(int atrybut)
{   char  *adp, *adk;
    NAGLOWEK *ad, *adn;

    adp=dane;
    adk=dane+dane_size;

    ADPQ=ADKQ=NULL;

    obiekt_tok((char*)adp,adk,(char **) &ad,ONieOkreslony);
    while (ad!=NULL)
    {
        if (TRUE == Check_Attribute (ad->atrybut, atrybut))
        {
           if (ADPQ==NULL) ADPQ=ad;
           adn=ad+sizeof(NAGLOWEK)+ad->n - 1;
           if (adn> ADKQ)  ADKQ=adn;
        }
        obiekt_tok(NULL,adk,(char **) &ad,ONieOkreslony);
    }
}



void ciagnij_quad(void)  //rectangle, square and solid only
{ int	ret_blokc, ret_blokq;
    long_long ad_dist;
    char *adp_b, *adk_b, *adp_b1;
    long_long dane0, dane1;
    TTF_redraw = FALSE;
    int shadows_no;
    long_long l_offb, l_offe;
    BLOK* ptrs_b = NULL;
    long_long ptrc_b_offs;
    long_long ptrs_pl_n;
    char* buff_ptrs_b;
    BLOK *ptrs_temp ;
    BOOL add_to_block;
    BOOL make_spline_polylines;
    long_long del_dane;

    dragging_quad=TRUE;

ret_blokq=blokq(dzi_q, odzi_q);

//searching for ADP, ADK
find_adpq_adkq(Ablok);

if (ADPQ==NULL)
    return;

oknoS(Xp,Yp,Xk,Yk);

    InfoList(2);

    blokzap(dane, dane+dane_size, Ablok, COPY_PUT, 0);

    trans_scale_arcs_(dane, dane+dane_size, 0, 0, 1.0, 1.0, TRUE);

    //searching for ADP, ADK
    find_adpq_adkq(Ablok);

    blokzap(ADPQ, ADKQ, Ablok, XOR_PUT, 1);

    memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
    zmien_atrybut_undo (dane, dane +	dane_size) ;

    //changing Ablok attribut to A, all quading will based on Aoblok and Ablok for stretching frame
    zmien_atrybut(ADPQ, ADKQ, Ablok, Aoblok);

    dragging_quad_move=TRUE;

    InfoList(0);

    CUR_OFF(X,Y);
    CUR_ON(X,Y);

  redcrC(0);

  ret_blokc=blokc(dzic_q, COMNDmbs);

  if(ret_blokc==0)
	{ redcrC(6);
        dragging_quad_move=FALSE;
        dragging_quad=FALSE;
	  return;
	}

  redcrC(1);
  blok(dzic_q /*dzic2*/,odzic2,redrawbc,COMNDmbc);

  redcrC(2);
  if (przesunp()==ESC)
	{ redcrC(3);
        dragging_quad_move=FALSE;
        dragging_quad=FALSE;
	  return;
	}
  if(ciagnijk_quad()==ESC) {
      redcrC(4);
  }
  else {
      redcrC(5);
  }
    dragging_quad_move=FALSE;
  if (TTF_redraw) redraw();
    dragging_quad=FALSE;
  return;
}

/*------Export-----------------------------------------------------*/


int FileName(char	*fn,int i_maxlen)	  	/* funkcja nie uzywana w tym module*/
{
  if (!read_esc(fn,i_maxlen-1,26)) return	0;
	else return	1;
}


int FileNameE(char *fn,int	i_maxlen)
{
  BOOL ret;

  i_maxlen = i_maxlen ;
  ret	= Load_File	(fn, SAVE_BLOK, TRUE) ;
  return	ret ;
}

int BlockName (char *blok_name, int	i_maxlen)
/*----------------------------------------*/
{
  if (!get_string (blok_name, "", i_maxlen - 1, 0, 11)) return 0 ;
  else return 1 ;
}

int Block_Type	(char	*blok_type,	int i_maxtype)
/*-------------------------------------------*/
{
  if (!read_esc (blok_type, i_maxtype -	1,	14)) return	0 ;
  else return 1;
}

/*--------------------------------------------*/
void redcrq (char typ)
/*--------------------------------------*/
{ static	void (*CUR_oN)(double ,double);
    static	void (*CUR_oFF)(double ,double);
    static	int (	*SW[2])();
    static	TMENU	*menu;
    static	char st[POLE_TXT_MAX];
    static    unsigned short cod = L' ';
    static	 int sel_akt, sel_cur, sel_gor, sel_nr;
    static    int iconno;
    switch(typ)
    { case 0	:
            Cur_offd(X, Y);
            komunikat (0) ;
            komunikat0 (147)	;

            sel_akt = sel.akt; sel.akt = 1;
            sel_cur = sel.cur; sel.cur=1;
            sel_gor = sel.gor; sel.gor = 0;
            sel_nr = sel.nr;

            CUR_oFF=CUR_OFF;
            CUR_oN=CUR_ON;
            SW[0]=SERV[def67];  SERV[def67]=winf;
            SW[1]=SERV[def68];  SERV[def68]=addf;
            *(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
            winf_draw ();
            addf_draw ();
            getmenupini(&menu,st,&cod, &iconno);

            menupini(&mBlokm, _BLOCK_, _BLOCK_C_, 1);

            Cur_ond(X, Y);
            break;
        case 1	: //Cur_offd(X, Y);
            SERV[def67]= (void*)SW[0];
            SERV[def68]= (void*)SW[1];

            sel.akt=sel_akt;
            sel.cur=sel_cur;
            sel.gor = sel_gor;
            sel.nr = sel_nr;

            menupini(menu,st,cod, iconno);
            setfillstyle_(SOLID_FILL, BKCOLOR);
            bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
            komunikat0 (0);
            break;
        default :	break	;
    }
}

static void	redcrE (char typ,	int b_ex)
/*--------------------------------------*/
{ static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt, temp;
  static    int iconno;
  switch(typ)
	{ case 0	:
		Cur_offd(X, Y);
		komunikat (0) ;
		pcx_outlines_view_on();
		temp = 0;
		switch (b_ex)
		{
		case 0: temp = 64; //grouping
			break;
		case 1: temp = 36; //export
			break;
		case 2: temp = 133; //save in clipboard
			break;
		}
		komunikat0 (temp)	;
		sel_akt=sel.akt;	sel.akt=1;
		DX=DY=0;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod, &iconno);

		menupini(&mBlokm, _BLOCK_, _BLOCK_C_, 1);

		Cur_ond(X, Y);
		break;
	  case 1	: //Cur_offd(X, Y);
		SERV[def67]= (void*)SW[0];
		SERV[def68]= (void*)SW[1];
		menupini(menu,st,cod, iconno);
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		komunikat0 (37);
		break;
	  case 3	:
	  case 2	:
		  pcx_outlines_view_off();
		if	(typ == 2 || typ == 3)
		{
	    blokzap(ADP,ADK,Ablok,COPY_PUT,1);
		}
		CUR_OFF=CUR_oFF;
		CUR_ON=CUR_oN;
		sel.akt=sel_akt;
		memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
		zmien_atrybut_undo (dane, dane +	dane_size) ;
		zmien_atrybut (ADP, ADK, Ablok, Aoblok) ;
        CUR_OFF(X,Y);
        CUR_ON(X,Y);

		komunikat0 (0)	;
		break	;
	  default :	break	;
	}
}

void Export(void)
{
  static void (*REDdod)(void)=nooop;
  char fn [MAXPATH] ;
  int	i_maxlen	= Max_Spec_Value ;
  int i_maxtype = Max_Spec_Name ;
  char blok_name [Max_Spec_Block] ;
  char blok_type [Max_Spec_Block] ;
  int key;
  char st1[MaxTextLen];

  fn [0]	= '\0' ;
  blok_name	[0] =	'\0' ;
  blok_type	[0] =	'\0' ;
  redcrE(0,	1);
  blok(dzi,odzi,Redraw_Block,COMNDmb);
  redcrE(1,	1)	;
  if (przesunp()==ESC)
	{ redcrE(2,	1)	;
	  return;
	}
  REDdod	= REDDOD	;
  REDDOD	= Redraw_Block	;
  
  if (TRUE == FileNameE	(fn, MAXPATH))
  {
#ifndef LINUX
	 /*
	if	(__file_exists(fn)) 
	 {
	  strcpy(st1, _FILE_);
	  strcat(st1,fn);
	  strcat(st1, _FILE_EXISTS_);
	  komunikat_str(st1);
	  key	= Get_Legal_Key (_YES_NO_ESC_);
	  komunikat	(0);
	  if (key == ESC)	return;
	  if (key != _YES_ && key != _yes_)
	  {
		 REDDOD=REDdod;
		 redcrE (3,	1)	;
		 return;
	  }
	 }
	 */
#endif
	 if (TRUE != Read_Spec_Block (fn, blok_name,	i_maxlen,
		blok_type, i_maxtype,	FALSE))
	 {
		blok_name [0] ='\0' ;
		blok_type [0] = '\0'	;
	 }
	 if (	BlockName (blok_name, i_maxlen)	&&
	 Block_Type	(blok_type,	i_maxtype))
	 {
		 CUR_OFF(X,Y);
		 WriteBlock(fn,X,Y, blok_name, blok_type,0, 1);
		 CUR_ON(X,Y);
	 }
  }
  REDDOD=REDdod;
  redcrE	(3, 1) ;
  return;
}

int Export_Upgrade(char *fn, double Px, double Py, char *b_name, char *b_type)
{
    int	i_maxlen = Max_Spec_Value ;
    int i_maxtype = Max_Spec_Name ;
    char blok_name [Max_Spec_Block] ;
    char blok_type [Max_Spec_Block] ;
    int ret;

        if (TRUE != Read_Spec_Block (fn, blok_name,	i_maxlen, blok_type, i_maxtype,	FALSE))
        {
            return 0;
        }

        if ((strcmp(blok_type, b_type)!=0) || (strcmp(blok_name, b_name)!=0))
        {
            int a=0;
        }


        ret=WriteBlock(fn,Px,Py, b_name, b_type,0, 0);

    return ret;
}

void Zapamietaj(void)
{
  static void (*REDdod)(void)=nooop;
  char fn [MAXPATH] ;
  int	i_maxlen	= Max_Spec_Value ;
  int i_maxtype = Max_Spec_Name ;
  char blok_name [Max_Spec_Block] ;
  char blok_type [Max_Spec_Block] ;

  fn [0]	= '\0' ;
  blok_name	[0] =	'\0' ;
  blok_type	[0] =	'\0' ;
  redcrE(0,	2);
  blok(dzi,odzi,Redraw_Block,COMNDmb);
  redcrE(1,	2)	;
  if (przesunp()==ESC)
	{ redcrE(2,	2)	;
	  return;
	}
  REDdod	= REDDOD	;
  REDDOD	= Redraw_Block	;
  strcpy(fn,"blk00000.alx");
  blok_name	[0] ='\0' ;
  blok_type	[0] =	'\0' ;

  strcpy(blok_name, _SAVED_BLOCK_);

  if (Block_Type (blok_type, i_maxtype))
	{
		 CUR_OFF(X,Y);
		 WriteBlock(fn,X,Y, blok_name, blok_type,0, 1);
		 CUR_ON(X,Y);
	}
  REDDOD=REDdod;
  redcrE	(3, 2) ;
  return;
}

/*------Set_Block-----------------------------------------------------*/

void Set_Block	(void)
/*------------------*/
{
  static void (*REDdod)(void)=nooop;
  char blok_type [Max_Spec_Block] =	"";
  redcrE	(0, 0) ;
  blok (dzi, odzi, Redraw_Block,	COMNDmb)	;
  redcrE	(1, 0) ;
  if (przesunp	()	==	ESC)
  {
	 redcrE (2,	0) ;
	 return ;
  }
  REDdod	= REDDOD	;
  REDDOD	= Redraw_Block	;
  if (Block_Type (blok_type, Max_Spec_Block))
  {
	 CUR_OFF	(X, Y) ;
	 Set_Block_Proc (X, Y, blok_type) ;
	 CUR_ON (X,	Y)	;
  }
  REDDOD	= REDdod	;
  redcrE	(3, 0) ;
  return	;
}


static void	(*COMNDmark_t [])	(void) =
{ (void (*)(void))ostatni_blok,
  (void (*)(void))wszystkie_teksty,
  (void (*)(void))winf,	(void	(*)(void))addf, nooop, 
  (void (*)(void))auto_on,	(void	(*)(void))auto_off };
  
static /*const*/ void	(*COMNDmark_ts	[]) (void) =
{ (void (*)(void))ostatni_blok,
  (void (*)(void))wszystkie_teksty_symbole,
  (void (*)(void))wszystkie_teksty,
  (void (*)(void))winf,	(void	(*)(void))addf, nooop, 
  (void (*)(void))auto_on,	(void	(*)(void))auto_off };  


static void	redcr_mark_t (char typ)
{
  static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt;
  static    int iconno;
  switch(typ)
	{ case 0	:
		komunikat(0);
		komunikat0(118);
		sel_akt=sel.akt;	sel.akt=1;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod,&iconno);

		menupini(&mBlok_cht, _CHANGE_, _CHANGE_C_, 4);

        Cur_offd(X, Y);
		Cur_ond(X,Y);
		break;
	  case 1	:
		komunikat(0);
		komunikat0(118);
		sel_akt=sel.akt;	sel.akt=1;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod,&iconno);

		menupini(&mBlok_chts, _CHANGE_, _CHANGE_C_, 4);

		Cur_offd(X,Y);
        Cur_ond(X,Y);
		break; 
	  case 3	:
		menupini(menu,st,cod, iconno);
		CUR_OFF=CUR_oFF;
		CUR_ON=CUR_oN;
		SERV[def67]= (void*)SW[0];
		SERV[def68]= (void*)SW[1];
		sel.akt=sel_akt;
		if	(ADP != NULL && ADK != NULL)
		{
         blokzap(ADP,ADK,Ablok,COPY_PUT,1);
         memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
         zmien_atrybut_undo(dane, dane +	dane_size);
         zmien_atrybut(ADP,ADK,Ablok,Aoblok);
		}
        Cur_offd(X, Y);
		Cur_ond(X,Y);
		komunikat (0) ;
		komunikat0 (0)	;
		break;
	  case 4	:
		menupini(menu,st,cod, iconno);
		CUR_OFF=CUR_oFF;
		CUR_ON=CUR_oN;
		SERV[def67]= (void*)SW[0];
		SERV[def68]= (void*)SW[1];
		sel.akt=sel_akt;
		if	(ADP != NULL && ADK != NULL)
		{
         memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
         zmien_atrybut_undo(dane, dane +	dane_size);
         zmien_atrybut(ADP,ADK,Ablok,Aoblok);
		}
        Cur_offd(X, Y);
        Cur_ond(X,Y);
		komunikat (0) ;
		komunikat0 (0)	; 
		break;
	}
}
/*------------------------------------------------------*/

void Mark_Texts (int	opcja)
/*-----------------------*/
{
  redcr_mark_t	(opcja);
  if (opcja==0) blok	(dzi,	odzi,	Redraw_Block, COMNDmark_t)	;
	 else	if	(opcja==1) blok (dzi, odzi, Redraw_Block,	COMNDmark_ts) ;
		else blok (dzi, odzi, Redraw_Block,	COMNDmark_t) ;
  return;
}

void Unmark_Texts	(int opcja)
/*-------------------------*/
{
  redcr_mark_t	(opcja);
  return;
}
/*------------------------------------------------------*/	
/**/

/*--------------------------------------------*/


static /*const*/ void	(*COMNDmb_chp [])	(void) =
{ (void (*)(void))ostatni_blok,(void (*)(void))wszystko,(void (*)(void))wszystkie_warstwy,
  (void(*)(void))all_from_layer,
  (void (*)(void))wszystkie_teksty,
  (void (*)(void))winf,	(void	(*)(void))addf, nooop, Change_Properties_dlg,
  (void (*)(void))auto_on,	(void	(*)(void))auto_off };


static void	redcr_chp (char typ)
{
  static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt;
  static    int iconno;
  switch(typ)
	{ case 0	:
		Cur_offd(X, Y);
		komunikat(0);
		pcx_outlines_view_on();
		komunikat0(50);
		sel_akt=sel.akt;	sel.akt=1;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod,&iconno);

		menupini(&mBlok_chp, _CHANGE_, _CHANGE_C_, 43);

		Cur_ond(X, Y);
		break;
	  case 1	:
		pcx_outlines_view_off();
		menupini(menu,st,cod, iconno);
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		CUR_OFF=CUR_oFF;
		CUR_ON=CUR_oN;
		SERV[def67]= (void*)SW[0];
		SERV[def68]= (void*)SW[1];
		sel.akt=sel_akt;
		if	(ADP != NULL && ADK != NULL)
		{
            blokzap(ADP,ADK,Ablok,COPY_PUT,0); //to avoid translucency interference
		 blokzap(ADP,ADK,Ablok,COPY_PUT,1);
		 memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
		 zmien_atrybut_undo(dane, dane +	dane_size);
		 zmien_atrybut(ADP,ADK,Ablok,Aoblok);
		}
        Cur_offd(X, Y);
		Cur_ond(X,Y);
		komunikat (0) ;
		komunikat0 (0)	;
	}
}

static void	redcr_pcx (char typ)
{
  static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt;
  static    int iconno;
  switch(typ)
	{ case 0	:
		Cur_offd(X, Y);
		komunikat(0);
		pcx_outlines_view_on();
		komunikat0(134);
		sel_akt=sel.akt;	sel.akt=1;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod,&iconno);

		menupini(&mBlokmc, _BLOCK_, _BLOCK_C_, 1);

		Cur_ond(X, Y);
		break;
     case 1	:
		Cur_offd(X, Y);
		komunikat(0);
		pcx_outlines_view_on();
		komunikat0(135);
		sel_akt=sel.akt;	sel.akt=1;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod,&iconno);

		menupini(&mBlokmc, _BLOCK_, _BLOCK_C_, 1);

		Cur_ond(X,Y);
		break;
	  case 2	:
		Cur_offd(X, Y);
		pcx_outlines_view_off();
		menupini(menu,st,cod, iconno);
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		CUR_OFF=CUR_oFF;
		CUR_ON=CUR_oN;
		SERV[def67]= (void*)SW[0];
		SERV[def68]= (void*)SW[1];
		sel.akt=sel_akt;
		if	(ADP != NULL && ADK != NULL)
		{
		 blokzap(ADP,ADK,Ablok,XOR_PUT,1);
		 memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
		 zmien_atrybut_undo(dane, dane +	dane_size);
		 zmien_atrybut(ADP,ADK,Ablok,Aoblok);
		}
		Cur_ond(X,Y);
		komunikat (0) ;
		komunikat0 (0)	;
	}
}
/*------------------------------------------------------*/

void Change_Properties (void)
/*---------------------------*/
{
  redcr_chp	(0);
  blok (dzi, odzi, Redraw_Block,	COMNDmb_chp) ;
  if (check_if_obiekt(dane, dane+dane_size, Ablok, ONieOkreslony)) Change_Properties_dlg();
  redcr_chp	(1);
  return;
}

char * Get_PCX (int option)
/*-----------------------*/
{
	int i_ret;
  if (option==0) redcr_pcx	(0); else  redcr_pcx (1);
  adres_pcx=NULL;
  global_only_pcx = TRUE;
  i_ret = Block_Proc_Wez_w (dzi_pcx, odzi, Redraw_Block, COMNDmb, 0) ;
  global_only_pcx = FALSE;
  redcr_pcx	(2);
  return adres_pcx;
}

void ReGet_PCX (void)
/*----------------*/
{

  return;
}

/*---------Wydluz---------------------------------------------*/

static void	redcrEx (char typ)
/*--------------------------*/
{
  static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[4])();
  static	TMENU	*menu;
  static	void (*REDdod)(void)=nooop;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt, sel_cur;
  static    int iconno;

  switch(typ)
  {
	 case	0 :
		CUR_OFF(X, Y);
		komunikat(0);
		komunikat0 (51) ;
		sel_akt=sel.akt;	sel.akt=1;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini(&menu,st,&cod,&iconno);

		menupini(&mBlokm, _BLOCK_, _BLOCK_C_, 1);

		CUR_ON(X, Y);
		break;
	 case	1 :
		CUR_OFF(X, Y);
		REDdod =	REDDOD ;	REDDOD =	Redraw_Block ;
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		sel.akt=0;
		sel_cur=sel.cur; sel.cur=1;
		CUR_oFF=CUR_OFF; CUR_OFF=out_sel_off;
		CUR_oN=CUR_ON;	  CUR_ON=out_sel_on;
		SW[2]=SERV[73];  SERV[73]=sel_t;
		SW[3]=SERV[81];  SERV[81]=sel_d;
		CUR_ON(X, Y);
		SERV [def67] = (void*)SW[0]	;
		SERV [def68] = (void*)SW[1]	;
		menupini	(menu, st, cod, iconno) ;
		komunikat0 (52) ;
		
		break;
	 case	2 :
	  REDDOD=REDdod;
	  CUR_OFF =	CUR_oFF ;
	  CUR_ON	= CUR_oN	;
	  SERV[73] = (void*)SW[2] ;
	  SERV[81] = (void*)SW[3] ;
	  sel.akt =	sel_akt ;
	  sel.cur =	sel_cur ;
	  memmove(&UNDO_REC,	&UNDO_REC_255,	sizeof(UNDO_TAB_REC));
	  zmien_atrybut_undo	(dane, dane	+ dane_size) ;
      blokzap (ADP, ADK,	Ablok, COPY_PUT, 1) ;
	  zmien_atrybut (ADP, ADK,	Ablok, Aoblok)	;
      CUR_OFF(X, Y);
	  CUR_ON(X, Y);
	  komunikat	(0) ;
	  komunikat0 (0) ;
	  break ;
	default:
	  break;
	}
}

void Extend	(void)
/*----------------*/
{
	while (1)
	{
		redcrEx(0);
		ADP = NULL;
		ADK = NULL;
		blok(dzi, odzi, Redraw_Block, COMNDmb);
		if ((ADP == NULL) && (ADK == NULL))
		{
			redcrEx(2);
			return;
		}
		redcrEx(1);
		Extend_Proc();
		redcrEx(2);
	}
    return;
}


/*--------------  Trim  ---------------------------------------*/


static void	redcrTr (char typ)
/*--------------------------*/
{
  static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[4])();
  static	TMENU	*menu;
  static	void (*REDdod)(void)=nooop;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt, sel_cur;
  static    int iconno;

  switch(typ)
  {
	 case	0 :
		CUR_OFF(X, Y);
		komunikat(0);
		komunikat0 (68) ;
		sel_akt=sel.akt;	sel.akt=1;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[def67];  SERV[def67]=winf;
		SW[1]=SERV[def68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini	(&menu,st,&cod,&iconno) ;

		menupini	(&mBlokm, _BLOCK_, _BLOCK_C_, 1)	;

		CUR_ON(X, Y);
		break;
	 case	1 :
		 CUR_OFF(X, Y);
		REDdod =	REDDOD ;	REDDOD =	Redraw_Block ;
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		sel.akt=0;
		sel_cur=sel.cur; sel.cur=1;
		CUR_oFF=CUR_OFF; CUR_OFF=out_sel_off;
		CUR_oN=CUR_ON;	  CUR_ON=out_sel_on;
		SW[2]=SERV[73];  SERV[73]=sel_t;
		SW[3]=SERV[81];  SERV[81]=sel_d;
		CUR_ON(X, Y);
		SERV [def67] = (void*)SW[0]	;
		SERV [def68] = (void*)SW[1]	;
		menupini	(menu, st, cod, iconno) ;
		komunikat0 (69) ;
		break;
	 case	2 :
	  REDDOD=REDdod;
	  blokzap (ADP, ADK,	Ablok, XOR_PUT, 1) ;
	  CUR_OFF =	CUR_oFF ;
	  CUR_ON	= CUR_oN	;
	  SERV[73] = (void*)SW[2] ;
	  SERV[81] = (void*)SW[3] ;
	  sel.akt =	sel_akt ;
	  sel.cur =	sel_cur ;
	  memmove(&UNDO_REC,	&UNDO_REC_255,	sizeof(UNDO_TAB_REC));
	  zmien_atrybut_undo	(dane, dane	+ dane_size) ;
      blokzap (ADP, ADK,	Ablok, COPY_PUT, 1) ;
	  zmien_atrybut (ADP, ADK,	Ablok, Aoblok)	;
      CUR_OFF(X, Y);
	  CUR_ON(X, Y);
	  menupini(menu, st, cod, iconno);
	  komunikat0 (0) ;
	  break ;
	default:
	  break;
	}
}

void Trim(void)
/*-------------*/
{
	while (1)
	{
		redcrTr(0);
		ADP = NULL;
		ADK = NULL;
		blok(dzi, odzi, Redraw_Block, COMNDmb);
		if ((ADP == NULL) && (ADK == NULL))
		{
			redcrTr(2);
			return;
		}
		redcrTr(1);
		Trim_Proc();
		redcrTr(2);
	}
    return;
}

/*---------------------------------------------------------*/

/*--------------  Mirror ---------------------------------------*/


static void	redcrMirr (char typ)
/*--------------------------*/
{
  static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[4])();
  static	TMENU	*menu;
  static	void (*REDdod)(void)=nooop;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt, sel_cur;
  static    int iconno;

  switch(typ)
  {
	 case	0 :
		Cur_offd(X, Y);
		komunikat(0);
		komunikat0 (70) ;
		sel_akt=sel.akt;	sel.akt=1;
		sel_cur=sel.cur;	sel.cur=0;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[67];  SERV[def67]=winf;
		SW[1]=SERV[68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini	(&menu,st,&cod,&iconno) ;

		menupini	(&mBlokm, _BLOCK_, _BLOCK_C_, 1)	;

		Cur_ond(X, Y);
		break;
	 case	1 :
		Cur_offd(X, Y);
		REDdod =	REDDOD ;	REDDOD =	Redraw_Block ;
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		Cur_ond (X,	Y)	;
		SERV [def67] = (void*)SW[0]	;
		SERV [def68] = (void*)SW[1]	;
		SW[2]=SERV[73]	;	SERV[73]=sel_t;
		SW[3]=SERV[81]	;	SERV[81]=sel_d;
		menupini	(menu, st, cod, iconno) ;
		break;
	 case	2 :
	  blokzap (ADP, ADK,	Ablok, COPY_PUT, 1) ;
     case   3 :
      REDDOD=REDdod;
      Cur_offd (X,	Y)	;
	  CUR_OFF =	CUR_oFF ;
	  CUR_ON	= CUR_oN	;
	  SERV[73] = (void*)SW[2] ;
	  SERV[81] = (void*)SW[3] ;
	  sel.akt =	sel_akt ;
	  sel.cur =	sel_cur ;
	  if (!get_b_delete_old()) memmove(&UNDO_REC,	&UNDO_REC_255,	sizeof(UNDO_TAB_REC)); //disabling undo
	  zmien_atrybut_undo	(dane, dane	+ dane_size) ;
	  zmien_atrybut (ADP, ADK,	Ablok, Aoblok)	;
	  komunikat0 (0) ;
	  Cur_ond(X, Y);
	  break ;
	default:
	  break;
	}
}

void Mirror(void)
/*---------------*/
{
	int ret;
	TTF_redraw = FALSE;
	redcrMirr(0);
	global_no_pcx = TRUE;
	blok(dzi, odzi, Redraw_Block, COMNDmb);
	global_no_pcx = FALSE;
	redcrMirr(1);
	ret = Mirror_Proc();
	if (!ret) redcrMirr(2);
	else
	{
        redcrMirr(3);
	  if (TTF_redraw) redraw();
	}
  return	;
}

void Mirror_X	(void)
/*---------------*/
{
 int ret;
 TTF_redraw = FALSE;
 while (1)
 {
  redcrMirr	(0) ;
  global_no_pcx = TRUE;
  if (blok_FIRSTB (dzi, odzi, Redraw_Block,	COMNDmb) == 0)
   {
	  global_no_pcx = FALSE;
      redcrMirr	(1) ;
      redcrMirr	(2) ;
      return ;
   }
  global_no_pcx = FALSE;
  redcrMirr	(1) ;
  ret = Mirror_Proc_X () ;
  if (!ret) redcrMirr	(2) ;
  else
  {
      redcrMirr	(2) ;
	  if (TTF_redraw) redraw();
  }
 }
  return	;
}

void Mirror_Y	(void)
/*---------------*/
{
 int ret;
 TTF_redraw = FALSE;
 while (1)
 {
  redcrMirr	(0) ;
  global_no_pcx = TRUE;
  if (blok_FIRSTB (dzi, odzi, Redraw_Block,	COMNDmb)	== 0)
   {
	  global_no_pcx = FALSE;
      redcrMirr	(1) ;
      redcrMirr	(2) ;
      return ;
   }
  global_no_pcx = FALSE;
  redcrMirr	(1) ;
  ret=Mirror_Proc_Y () ;
  if (!ret) redcrMirr	(2) ;
  else
  {
      redcrMirr	(2) ;
	  if (TTF_redraw) redraw();
  }
 }
  return	;
}

static void redcrEH1(char typ, void(*REDROWd)(void))
{
	static void(*CUR_oN)(double, double);
	static void(*CUR_oFF)(double, double);
	static int(*SW[2])(), sel_akt, sel_cur;
	static void(*REDdod)(void);
	if (typ == 0)
	{
		sel_akt = sel.akt; sel.akt = 0;
		sel_cur = sel.cur; sel.cur = 1;
		CUR_oFF = CUR_OFF; CUR_OFF = out_sel_off;
		CUR_oN = CUR_ON;   CUR_ON = out_sel_on;
		SW[0] = SERV[73];  SERV[73] = sel_t;
		SW[1] = SERV[81];  SERV[81] = sel_d;
		
		ADP = NULL;
		ADK = NULL;
		
		REDdod = REDDOD; REDDOD = REDROWd;
		CUR_ON(X, Y);
	}
	else if (typ == 1)
	{
		CUR_OFF(X, Y);
		REDDOD = REDdod;
		CUR_OFF = CUR_oFF;
		CUR_ON = CUR_oN;
		SERV[73] = (void*)SW[0];
		SERV[81] = (void*)SW[1];
	
		sel.akt = sel_akt;
		sel.cur = sel_cur;
	}
	else if (typ == 2)
	{
		SERV[73] = (void*)SW[0];
		SERV[81] = (void*)SW[1];
	}
	else if (typ == 3)
	{
		SW[0] = SERV[73];  SERV[73] = sel_t;
		SW[1] = SERV[81];  SERV[81] = sel_d;
	}
}

static void	redcrEH(char typ)
/*--------------------------*/
{
	static	void(*CUR_oN)(double, double);
	static	void(*CUR_oFF)(double, double);
	static	int(*SW[4])();
	static	TMENU	*menu;
	static	void(*REDdod)(void);
	static	char st[POLE_TXT_MAX];
	static    unsigned short cod = L' ';
	static	int sel_akt, sel_cur;
	static    int iconno;

	switch (typ)
	{
	case	0:
		Cur_offd(X, Y);
		komunikat(0);
		komunikat0(138);
		sel_akt = sel.akt;	sel.akt = 0;
		sel_cur = sel.cur;	sel.cur = 1;
		CUR_oFF = CUR_OFF;  CUR_OFF = out_sel_off;
		CUR_oN = CUR_ON; CUR_ON = out_sel_on;

		SW[0] = SERV[73];  SERV[73] = sel_t;
		SW[1] = SERV[81];  SERV[81] = sel_d;

		*(Block_Proc_Add_Mode_Ptr()) = ADD_MODE;
		winf_draw();
		addf_draw();
		getmenupini(&menu, st, &cod, &iconno);

		menupini(&mBlokm, _BLOCK_, _BLOCK_C_, 1);

		break;
	case	1:
		REDdod = REDDOD;	REDDOD = Redraw_Block;
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX / 2 + 1, 0, maxX / 2 + 100, 11 /*9*/);
		Cur_ond(X, Y);
		SERV[def67] = (void*)SW[0];
		SERV[def68] = (void*)SW[1];
		SW[2] = SERV[73];	SERV[73] = sel_t;
		SW[3] = SERV[81];	SERV[81] = sel_d;
		menupini(menu, st, cod, iconno);
		break;
	case	2:
		REDDOD = REDdod;
		Cur_offd(X, Y);
		blokzap(ADP, ADK, Ablok, XOR_PUT, 1);
		CUR_OFF = CUR_oFF;
		CUR_ON = CUR_oN;
		SERV[73] = (void*)SW[2];
		SERV[81] = (void*)SW[3];
		sel.akt = sel_akt;
		sel.cur = sel_cur;
		memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
		zmien_atrybut_undo(dane, dane + dane_size);
		zmien_atrybut(ADP, ADK, Ablok, Aoblok);
		Cur_ond(X, Y);
		komunikat0(0);
		break;
	default:
		break;
	}
}


char  *find_first_hatch_block(void *ad1, double *x0b, double *y0b)
/*--------------------------------------------------------------*/
{
	BLOK *ptrs_block;
	T_Desc_Ex_Block *ptrs_desc_bl;

	if (((LINIA *)ad1)->blok == NoElemBlok) return NULL;
	else
	{
		if (NULL != (ptrs_block = (BLOK  *)FIRSTB(ad1)))
		{
			if ((ptrs_block->kod_obiektu == B_HATCH) && (ptrs_block->dlugosc_opisu_obiektu > 16))
			{
				ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu[0]);
				*x0b = ptrs_desc_bl->x;
				*y0b = ptrs_desc_bl->y;
				return (char*)ptrs_block;
			}
			else return NULL;
		}
		else return NULL;
	}
}

/*--------------  Array ---------------------------------------*/

enum ARRAY {A_Rect =	1,	A_Polar = 2, A_Polar_Rot =	3}	;

static void	redcrArray (char typ, int array_mode)
/*-----------------------------------------------*/
{
  static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[4])();
  static	TMENU	*menu;
  static	void (*REDdod)(void)=nooop;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt, sel_cur, kom0	;
  static    int iconno;

  switch(typ)
  {
	 case	0 :
		komunikat(0);
		pcx_outlines_view_on();
		kom0 = 73 ;
		if	(A_Rect != array_mode)
		{
	      kom0 = 74 ;
		}
		Cur_offd(X, Y);
		komunikat0 (kom0)	;
		sel_akt=sel.akt;	sel.akt=1;
		sel_cur=sel.cur;	sel.cur=0;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[67];  SERV[def67]=winf;
		SW[1]=SERV[68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini	(&menu,st,&cod,&iconno) ;

		menupini	(&mBlokm, _BLOCK_, _BLOCK_C_, 1)	;

		Cur_ond(X,Y);
		
		break;
	 case	1 : 
		Cur_offd(X, Y);
		pcx_outlines_view_off();
		REDdod =	REDDOD ;	REDDOD =	Redraw_Block ;
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+(WIDTH*12),HEIGHT);
		SERV [def67] = (void*)SW[0]	;
		SERV [def68] = (void*)SW[1]	;
		SW[2]=SERV[73]	;	SERV[73]=sel_t;
		SW[3]=SERV[81]	;	SERV[81]=sel_d;
		Cur_ond(X, Y);
		menupini	(menu, st, cod, iconno) ;
		break;
	 case	2 :
	  REDDOD=REDdod;
      blokzap (ADP, ADK, Ablok, COPY_PUT, 1) ;
	  CUR_OFF =	CUR_oFF ;
	  CUR_ON	= CUR_oN	;
	  SERV[73] = (void*)SW[2] ;
	  SERV[81] = (void*)SW[3] ;
	  sel.akt =	sel_akt ;
	  sel.cur =	sel_cur ;
	  memmove(&UNDO_REC,	&UNDO_REC_255,	sizeof(UNDO_TAB_REC));
	  zmien_atrybut_undo	(dane, dane	+ dane_size) ;
	  zmien_atrybut (ADP, ADK,	Ablok, Aoblok)	;
	  ADP	= ADK	= NULL ;
      Cur_offd (X, Y) ;
	  Cur_ond (X, Y) ;
	  komunikat0 (0) ;
	  break ;
	default:
	  break;
	}
}

static void	array_rect_polar (int array_mode)
/*-------------------------------------------*/
{
  TTF_redraw = FALSE;
  redcrArray (0, array_mode) ;
  blok (dzi, odzi, Redraw_Block,	COMNDmb)	;
  redcrArray (1, array_mode) ;
  switch	(array_mode)
  {
	 case	A_Rect :
		 Array_Rect_Proc () ;
		 break ;
	 case	A_Polar :
	 Array_Polar_Proc	(FALSE) ;
		 break ;
	 case	A_Polar_Rot	:
	 Array_Polar_Proc	(TRUE) ;
		 break ;
	 default :
		 break ;
  }
  redcrArray (2, array_mode) ;
  if (TTF_redraw) redraw();
  return	;
}

void Array_Rect (void)
/*-------------------*/
{
  array_rect_polar (A_Rect) ;
}


void Array_Polar (void)
/*-------------------*/
{
  array_rect_polar (A_Polar) ;
}

void Array_Polar_Rot	(void)
/*-----------------------*/
{
  array_rect_polar (A_Polar_Rot)	;
}

/*--------------  Hatch ---------------------------------------*/

void Os_odniesienia (void)
/*---------------------*/
{

aa:
  redcrMP (0,1) ;
  if (FALSE	==	axis_point (&df_apx1, &df_apy1))
  {
	 redcrMP	(1,1)	;
	 return ;
  }
  redcrMP (2,1) ;
  if (FALSE	==	axis_point (&df_apx2, &df_apy2))
  {
	 goto	aa	;
  }
  redcrMP (3,1) ;
}


static void	redcrHatch (char typ)
/*--------------------------*/
{
  static	void (*CUR_oN)(double ,double);
  static	void (*CUR_oFF)(double ,double);
  static	int (	*SW[2])();
  static	TMENU	*menu;
  static	void (*REDdod)(void)=nooop;
  static	char st[POLE_TXT_MAX];
  static    unsigned short cod = L' ';
  static	int sel_akt, sel_cur;
  static    int iconno;

  switch(typ)
  {
	 case	0 :
		Cur_offd(X, Y);
		komunikat(0);
		komunikat0 (87) ;
		sel_akt=sel.akt;
		sel_cur=sel.cur;
		CUR_oFF = CUR_OFF;
		CUR_oN = CUR_ON;
		SW[0]=SERV[67];  SERV[def67]=winf;
		SW[1]=SERV[68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini	(&menu,st,&cod,&iconno) ;

		menupini	(&mBlokm, _BLOCK_, _BLOCK_C_, 1)	;   //&mBlokmh

		CUR_ON(X, Y);
		break;
	 case  11:
		 Cur_offd(X, Y);
		 komunikat(0);
		 komunikat0(138);
		 sel_akt = sel.akt;	sel.akt = 1;
		 sel_cur = sel.cur;	sel.cur = 0;
		 CUR_oFF = CUR_OFF; CUR_OFF = out_sel_off;
		 CUR_oN = CUR_ON; CUR_ON = out_sel_on;

		 SW[0] = SERV[73];  SERV[73] = sel_t;
		 SW[1] = SERV[81];  SERV[81] = sel_d;

		 *(Block_Proc_Add_Mode_Ptr()) = ADD_MODE;
		 winf_draw();
		 addf_draw();
		 getmenupini(&menu, st, &cod, &iconno);

		 menupini(&mBlokm, _BLOCK_, _BLOCK_C_, 1);

		CUR_ON(X, Y);
		//Cur_ond(X, Y);
		 break;
	 case	10	:
		Cur_offd(X, Y);
		komunikat(0);
		komunikat0 (123) ;
		sel_akt=sel.akt;	sel.akt=1;
		sel_cur=sel.cur;	sel.cur=0;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[67];  SERV[def67]=winf;
		SW[1]=SERV[68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini	(&menu,st,&cod,&iconno) ;

		menupini	(&mBlokm, _BLOCK_, _BLOCK_C_, 1)	;
		
		break;
	 case	1 :
		CUR_OFF (X, Y)	;
		REDdod =	REDDOD ;	////REDDOD =	Redraw_Block ;
		setfillstyle_(SOLID_FILL, BKCOLOR);
		bar(maxX/2+1,0,maxX/2+100,11 /*9*/);
		SERV [def67] = (void*)SW[0]	;
		SERV [def68] = (void*)SW[1]	;
		menupini	(menu, st, cod, iconno) ;
		break;
	 case	2 :
	  REDDOD=REDdod;
      blokzap (dane, dane	+ dane_size, Ablok, COPY_PUT, 1) ;
	  CUR_OFF =	CUR_oFF ;
	  CUR_ON	= CUR_oN	;
	  sel.akt =	sel_akt ;
	  sel.cur =	sel_cur ;
	  memmove(&UNDO_REC,	&UNDO_REC_255,	sizeof(UNDO_TAB_REC));
	  zmien_atrybut_undo(dane, dane	+ dane_size) ;
	  zmien_atrybut (dane, dane+dane_size,	Ablok, Aoblok)	;

	  menupini(menu, st, cod, iconno);
      CUR_OFF (X, Y)	;
	  CUR_ON(X, Y);
	  break ;

	  case 3	:
		Cur_offd(X, Y);
		komunikat(0);
		komunikat0 (97) ;
		sel_akt=sel.akt;	sel.akt=1;
		sel_cur=sel.cur;	sel.cur=0;
		CUR_oFF=CUR_OFF;
		CUR_oN=CUR_ON;
		SW[0]=SERV[67];  SERV[def67]=winf;
		SW[1]=SERV[68];  SERV[def68]=addf;
		*(Block_Proc_Add_Mode_Ptr ())	= ADD_MODE ;
		winf_draw ();
		addf_draw ();
		getmenupini	(&menu,st,&cod,&iconno) ;

		menupini	(&mBlokm, _BLOCK_, _BLOCK_C_, 1)	;

		CUR_ON(X, Y);
		break;
	default:
	  break;
	}
}


void find_base_point	(double *x_origin, double *y_origin)
/*-----------------------------------------------------*/
{
  BLOK  *b;
  NAGLOWEK_EXT *nag;
  T_Point *P;
  char  *adp,	*adk;
  BOOL first_base_point;
  double del_xx, del_yy, del_xy, min_xy;
  double xx_origin, yy_origin;

  adp	= dane ;
  adk	= dane +	dane_size ;
  first_base_point=FALSE;
  
  nag =	(NAGLOWEK_EXT*)adp;
  while ((nag->obiekt != Okoniec)	&&	(adp <= adk))
  {
	 if (nag->obiekt	==	OdBLOK)
	 {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
	 }
	 else
	 {	if	(adp>=last_find_addr) 
		{
		if	((nag->obiekt	==	Opoint) && (nag->atrybut != Abad) && (nag->atrybut != Ausuniety) &&
			 (Layers[nag->warstwa].on==TRUE))
		 {
		  P =	(T_Point*)adp;
        if ((P->obiektt2 == O2BlockSpecial) || (P->typ==1))
        {
        if (first_base_point==FALSE)
         {
           xx_origin=P->x;
		     yy_origin=P->y;
           del_xx=fabs(xx_origin-X);
           del_yy=fabs(yy_origin-Y);
           del_xy=sqrt(del_xx*del_xx+del_yy*del_yy);
           min_xy=del_xy;
           first_base_point=TRUE;
         }
          else
           {
             del_xx=fabs(P->x-X);
             del_yy=fabs(P->y-Y);
             del_xy=sqrt(del_xx*del_xx+del_yy*del_yy);
             if (del_xy<min_xy)
              {
               xx_origin=P->x;
		         yy_origin=P->y;
               min_xy=del_xy;
              }
           }
        }
		 }
		} 
		adp += nag->n	+ sizeof(NAGLOWEK) ;
	 }
	 nag	= (NAGLOWEK_EXT*)adp;
  }
  if (first_base_point==TRUE)
   {
    *x_origin=xx_origin;
	 *y_origin=yy_origin;
   }
    else
     {
      *x_origin=0;
  	   *y_origin=0;
     }
  return;
}

void Hatch_Brick	(int hatch_type)
/*------------------------------*/
{
  double	x_origin, y_origin;
  LINIA L;
  double hatch_scale, hatch_angle;
  int ret;

  L.typ=LiniaG.typ;
  L.kolor=LiniaG.kolor;
  LiniaG.typ=32;
  LiniaG.kolor=43;
  switch	(hatch_type)
  { case	0:	LiniaG.obiektt2=O2BlockHatch25;	 //25
				LiniaG.obiektt3=O3Bold;				 //36.5
				break;
	 case	1:	LiniaG.obiektt2=O2BlockHatch25;	 //25
				LiniaG.obiektt3=O3Thin;				 //24
				break;
	 case	2:	LiniaG.obiektt2=O2BlockHatch50;	 //50
				LiniaG.obiektt3=O3Bold;				 //36.5
				break;
	 case	3:	LiniaG.obiektt2=O2BlockAparat;	 //37
				LiniaG.obiektt3=O3Bold;				 //18
				break;
    case	4:	LiniaG.obiektt2=O2BlockHatch50;	 //50
				LiniaG.obiektt3=O3Thin;				 //8
				break;
    case	5:	LiniaG.obiektt2=O2BlockHatch25;	 //24.5
				LiniaG.obiektt3=0;    				 //36.3
				break;
	 case	6:	LiniaG.obiektt2=O2BlockHatch50;	 //24.5
				LiniaG.obiektt3=0;    				 //23.8
				break;
	 case	7:	LiniaG.obiektt2=O2BlockAparat;	 //49.5
				LiniaG.obiektt3=0;  				    //11.3
				break;
   default:	LiniaG.obiektt2=O2BlockHatch25;	 //25
				LiniaG.obiektt3=O3Bold;				 //36.5
				break;
  }
  add_zbior_2=0;
  comput_area = 0	;
  redcrHatch (10)	;
  /* 1. odszukanie granic kreskowania*/
  blok_special	(dzi,	odzi,	Redraw_Block, COMNDmb) ;
  /* 2. odszukanie punktu bazowego*/
  get_base_point(&x_origin, &y_origin);
  find_base_point(&x_origin, &y_origin);
  put_base_point(&x_origin, &y_origin);
  /* 3. ustawienie kata=0*/
  hatch_angle=0;
  put_hatch_angle(&hatch_angle);
  /* 4. ustawienie skali*/
  hatch_scale=50/SkalaF;
  put_hatch_scale(&hatch_scale);
  /* 5. ustawienie wzoru kreskowania kreskowania*/
  change_hatch_pattern(hatch_type);

  redcrHatch (1) ;
  /* 4. wypelnienie*/
  ret = Hatch_Proc (	comput_area, 0,0,0,0	) ;
  redcrHatch (2) ;
  add_zbior_2=1;
  LiniaG.typ=L.typ;
  LiniaG.kolor=L.kolor;
  LiniaG.obiektt2=0;
  LiniaG.obiektt3=0;
  return	;
}

void Hatch_Arm_Filigran(double kat_zb, double rozstaw_zb, double ls_zb_x, double ls_zb_y, int typ_linii, int kolor_linii, int warstwa)
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
{
  double	x_origin, y_origin;
  LINIA L;
  double hatch_scale, hatch_angle;
  int ret;

  memmove(&L, &LiniaG, sizeof(LINIA));
  LiniaG.typ=typ_linii;
  LiniaG.kolor=kolor_linii;
  LiniaG.warstwa=warstwa;
  LiniaG.obiektt2=O2BlockHatch50;
  add_zbior_2=0;
  comput_area = -1	;
  out_pole_ignore=TRUE;
  x_origin=ls_zb_x;
  y_origin=ls_zb_y;
  put_base_point(&x_origin, &y_origin);
  /* 3. ustawienie kata=0*/
  hatch_angle=kat_zb;
  put_hatch_angle(&hatch_angle);
  /* 4. ustawienie skali*/
  hatch_scale=1;
  put_hatch_scale(&hatch_scale);
  /* 5. ustawienie wzoru kreskowania kreskowania*/
  change_hatch_pattern(-1);

  put_hatch_distance(rozstaw_zb);

  redcrHatch (1) ;
  /* 4. wypelnienie*/
  ret = Hatch_Proc ( comput_area, 0,0,0,0 );
  comput_area=0;
  out_pole_ignore=FALSE;
  zmien_atrybut (dane, dane+dane_size,	Ablok, Aoblok)	;
  memmove(&LiniaG, &L, sizeof(LINIA));
  LiniaG.obiektt2=0;
  LiniaG.obiektt3=0;
  return	;
}


void Hatch (void)
/*---------------*/
{
	int ret;
  add_zbior_2=0;
  comput_area = 0	;
  out_pole_ignore=FALSE;
  redcrHatch (0) ;
  blok (dzi, odzi, Redraw_Block,	COMNDmb)	;
  redcrHatch (1) ;
  ret = Hatch_Proc (	comput_area, 0,0,0,0	) ;
 
  redcrHatch (2) ;

  add_zbior_2=1;
  return	;
}

void Change_Hatch(void)
{
	int ret = 1;

	while (ret == 1)
	{
		add_zbior_2 = 0;
		comput_area = 10; //editing
		out_pole_ignore = FALSE;
		redcrHatch(0);
		redcrHatch(1);
		ret = Hatch_Proc(comput_area, 0, 0, 0, 0);
		if (ret == 1)
			redcrHatch(2);
		else redcrHatch(1);
		add_zbior_2 = 1;

	}

	return;
}

int Change_Hatch_Pattern_Proc(char *ad, double x0b, double y0b)
{
	BLOK *adb;
	adb = (BLOK *)ad;
	return 0;
}

void Area (void)
/*---------------*/
{
	int ret;
  add_zbior_2=0;
  comput_area = 1	;
  out_pole_ignore=FALSE;
  redcrHatch (3) ;
  blok (dzi, odzi, Redraw_Block,	COMNDmb)	;
  redcrHatch (1) ;
  ret = Hatch_Proc (	comput_area, 0,0,0,0	) ;
  redcrHatch (2) ;
  add_zbior_2=1;
  redraw	();
  return	;
}

void GetArea (void)
/*-----------------*/
{
	int ret;
  counted_area0=0;
  counted_area=0;
  comput_area = 1	;
  redcrHatch (1) ;
  out_pole_ignore=TRUE;
  ret = Hatch_Proc ( comput_area, 0,0,0,0 );
  zmien_atrybut (dane, dane+dane_size,	Ablok, Aoblok)	;
  out_pole_ignore=FALSE;
  return;
}


void Test_Area(void)  //Test_Area 
{ int ret;
  add_zbior_2=0;
  comput_area = 1	;
  out_pole_ignore=FALSE;
  redcrHatch (3) ;
  blok (dzi, odzi, Redraw_Block,	COMNDmb)	;
  redcrHatch (1) ;
  ret=Hatch_Proc_Test1 (	comput_area, 0,0,0,0	) ; 
 
  if (ret==1) komunikat_str(_POINT_IN_AREA_);
    else if (ret==0) komunikat_str(_POINT_NOT_IN_AREA_);

  redcrHatch (2) ;
  add_zbior_2=1;
  return	;
}

void Area1 (void)
/*---------------*/
{
	int ret;
  add_zbior_2=0;
  comput_area = 1	;
  out_pole_ignore=FALSE;
  redcrHatch (3) ;
  blok (dzi_special, odzi, Redraw_Block,	COMNDmb)	;
  redcrHatch (1) ;
  ret = Hatch_Proc (	comput_area, 0,0,0,0	) ;
  redcrHatch (2) ;
  add_zbior_2=1;
  redraw	();
  return	;
}

void Srodek_ciezkosci (void)
/*------------------------*/
{
	int ret;
  add_zbior_2=0;
  comput_area = 2	;
  out_pole_ignore=FALSE;
  redcrHatch (3) ;
  blok (dzi, odzi, Redraw_Block,	COMNDmb)	;
  redcrHatch (1) ;
  ret = Hatch_Proc (	comput_area, 0,0,0,0	) ;
  redcrHatch (2) ;
  add_zbior_2=1;
  redraw	();
  return	;
}

void Moment_statyczny (void)
/*-----------------------*/
{
    LINIA L;
	int ret;
  add_zbior_2=0;
  comput_area = 3	;
  out_pole_ignore=FALSE;
  redcrHatch (3) ;
  blok (dzi, odzi, Redraw_Block,	COMNDmb)	;
  redcrHatch (1) ;
  Os_odniesienia () ;

    if (orto) {
        L.x1 = df_apx1;
        L.y1 = df_apy1;
        L.x2 = df_apx2;
        L.y2 = df_apy2;
        orto_l(&L, &Orto_Dir);
        df_apx2 = L.x2;
        df_apy2 = L.y2;
    }

  redcrHatch(1);
  ret = Hatch_Proc (	comput_area, df_apx1, df_apy1, df_apx2, df_apy2	) ;
  redcrHatch (2) ;
  add_zbior_2=1;
  redraw	();
  return	;
}
void Moment_bezwladnosci (void)
/*---------------------------*/
{
    LINIA L;
	int ret;
  add_zbior_2=0;
  comput_area = 4	;
  out_pole_ignore=FALSE;
  redcrHatch (3) ;
  blok (dzi, odzi, Redraw_Block,	COMNDmb)	;
  redcrHatch (1) ;

  Os_odniesienia () ;

    if (orto) {
        L.x1 = df_apx1;
        L.y1 = df_apy1;
        L.x2 = df_apx2;
        L.y2 = df_apy2;
        orto_l(&L, &Orto_Dir);
        df_apx2 = L.x2;
        df_apy2 = L.y2;
    }

  redcrHatch(1);
  ret = Hatch_Proc (	comput_area, df_apx1, df_apy1, df_apx2, df_apy2	) ;
  redcrHatch (2) ;
  add_zbior_2=1;
  redraw	();
  return	;
}

void Centr_mom_bezwl	(void)
/*---------------------------*/
{
	int ret;
  add_zbior_2=0;
  comput_area = 5	;
  out_pole_ignore=FALSE;
  redcrHatch (3) ;
  blok (dzi, odzi, Redraw_Block,	COMNDmb)	;
  redcrHatch (1) ;
  ret = Hatch_Proc (	comput_area, 0,0,0,0	) ;
  redcrHatch (2) ;
  add_zbior_2=1;
  redraw	();
  return	;
}


/*---------------------------------------------------------*/
void not_still_implemented	(void)
/*----------------------*/
{ int	key;

  komunikat	(107);
  key	= Get_Legal_Key ("\033\015");
  komunikat	(0);
}	


#undef __O_BLOK__