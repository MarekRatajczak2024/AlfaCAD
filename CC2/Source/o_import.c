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

#define __O_IMPORT__
#include <forwin.h>
#define ALLEGWIN
#include <allegext.h>
#ifndef LINUX
#include <dos.h>
#endif
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_loadf.h"
#include "o_dialog.h"

#include "menu.h"
#include "o_osnap.h"


#define ID_UP_END         8
#define ID_UP             1
#define ID_DN             2
#define ID_DN_END         9
#define ID_FOLDER         3
#define ID_FOLDER0        4
#define ID_BIGGER         5
#define ID_SMALLER        6
#define ID_ESCAPE         7
#define ID_ALPHABET       10
#define ID_TIME           11
#define ID_IMPORT  Dlg_Ret_Val_IMPORT_BLOCK
#define ID_INSERT  Dlg_Ret_Val_INSERT_BLOCK

static DLG_COLOR color_dlg;

static BOOL re_sort=TRUE;
static int sorting_type=0;
static int n_list_begin_blk = 0;

char background_pathname[MAXPATH] = __BACKGROUND__;
char background_pathname0[32] = __BACKGROUND__;

extern int SetPattern(char* pattern_path_name, char* pattern_file_name, char* ext);
extern void SetBackground(char *background_file_name);
extern int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext);

extern BOOL b__temp;
extern double current_hatch_scale, new_hatch_scale, current_hatch_angle, new_hatch_angle;
extern double current_hatch_distans, new_hatch_distans; 
extern int i__hatch_pattern_no;
static int sel_akt, sel_cur, sel_gor;
extern void redcrP (char) ;
extern void hatch_angle_rotate (void) ;
extern void hatch_scale (void) ;
extern void hatch_snap_base (void) ;
extern void hatch_snap_base_point (void) ;
extern void hatch_dist (void) ;
extern void set_hatch_pattern (int i_ev, int opcja, BOOL insulation_h);
extern void set_solidhatchtranslucency(int poz0, int opcja);
extern void hatch_image_pattern_dx(int poz);
extern void hatch_image_pattern_dy(int poz);
extern void hatch_image_pattern_scale(int poz);
extern void hatch_image_pattern_angle(int poz);
extern void hatch_angle_rotate_perpendicular(void);
extern void hatch_angle_rotate_parallel(void);

extern void change_layer (char  *adr,char  *adrk, int new_layer);
extern void normalize_layer (char *adr, char *adrk);
extern unsigned char *Change_Block_Descript(BLOK *, void *, int) ;
extern int InsBibFile(char *fn);
extern int ReadBlock_(char *fn,double *Px,double *Py,RYSPOZ *adp,RYSPOZ *adk, char *buf, int len, int *object_no, BOOL b_current_ver);
extern BOOL check_if_obiekt (char  *adp, char  *adk, int atrybut, int obiekt) ;
extern BOOL Get_List_Block (double *, double *, RYSPOZ *, RYSPOZ *, char *, int) ;
extern void Divide_Block (double, double) ;
void change_properties_h (char *, char *, T_change_param *);
int hatch_rotate_t3(void);
int hatch_rotate_t_3(void);
int hatch_rotate_t45(void);
int hatch_rotate_t_45(void);
int noooph(void);
int hatch_pattern_rotate_t45(void);
int hatch_pattern_rotate_t_45(void);
extern void Hatch_Rotate_t45(void);
extern void Hatch_Rotate_t_45(void);
extern void Hatch_Rotate_t3(void);
extern void Hatch_Rotate_t_3(void);
extern void obiekt_tok_all (char  *adr, char  *adrk, char  **ad, int obiekt);
extern BOOL Check_if_Equal0 (double x, double y);
extern BOOL Check_if_Equal (double x, double y);
extern BOOL Check_if_Equal2(double x, double y);
extern void delay(int time);
extern void KopiujM(void);
extern int kreowanie_profilu(char *plik_profilu);
extern int kreowanie_zegarow(char *plik_profilu);
extern int ReadPCX_real(char *fn,double *Px,double *Py,RYSPOZ *adp,RYSPOZ *adk, char *buf, int lenmax, int *object_no, BOOL b_current_ver);
extern void getimage(int left, int top, int right, int bottom, void  *bitmap);
extern void putimage(int left, int top, void  *bitmap, int op);

extern double jednostkiX0(double xx_);
extern double jednostkiY0(double yy_);
extern void null_path_bl(void);
extern void restore_path_bl(void);
extern double Angle_Normal(double angle);

extern char **strarray;
extern BOOL TTF_redraw;
extern char *strlwr2(char *_s);

extern void get_lines_width_dxf_glb(void);
extern BOOL global_no_pcx;

extern void *obiekt_Ll_wybrany(void);
extern char  *find_first_hatch_block(char *ad1, double *x0b, double *y0b);
extern BITMAP* cursor_pattern_bitmap0;
extern BITMAP* cursor_pattern_bitmap;
extern void* obiekt_LlS_wybrany(void);

extern int fixangles[4];

void WezSymbol(char * sym_name);
void WezSymbol_DXF(char * sym_name);
static void redcrH(char typ);

extern BOOL global_point_in_solid;
extern BOOL global_any_choice;
extern void set_World(long pXp1, long pYp1, long pXk1, long pYk1);
extern void Set_Enable_Button(TDIALOG* dlg, int id_button, BOOL enable);
extern int My_GetFiles(char* szDir, int* n_list, char* pattern, char* pattern1, char* pattern2, char* pattern3, BOOL blocks);
extern int Read_PNG_JPG_real(char* fn, double* Px, double* Py, RYSPOZ* adp, RYSPOZ* adk, char* buf, int lenmax, int* object_no, BOOL b_current_ver, int type);
extern int Convert_Image_to_ALX(char *fn);

extern void param_line_out(void);
extern void sw_callback_locked_proc(void);
extern void hatch_image_pattern_angle_menu(int angle);
extern void hatch_file_reopen(void);

extern int get_pXp(void);
extern int get_pXk(void);
extern int get_pYp(void);
extern int get_pYk(void);

extern int __file_exists(char *name);

extern int insulation_hatch_pattern[2];
extern double insulation_hatch_scale[2];
extern double insulation_hatch_scale_div[2];
extern char insulation_hatch_name[24][2];
extern int insulation_hatch_no;
extern BOOL insulation_hatch;
extern void	layer_info (void);
extern void view_scale(void);
extern int czytaj_dxf_blok(char *fn, double *X0, double *Y0, RYSPOZ *adp, RYSPOZ *adk, char *buf, double Jednostki_dxf, double SkalaF_dxf, int latin2_dxf, int block_view);
extern void backup_hatch_pattern(void);
extern void restore_hatch_pattern(void);

extern BOOL trans_scale_arcs_ (char *ADP_b, char *ADK_b, double df_basex, double df_basey, double df_sx, double df_sy, BOOL enforce);
extern BOOL trans_scale_arcs (double df_basex, double df_basey, double df_sx, double df_sy, BOOL enforce);
extern void Out_Edited_Draw_Param (ESTR *, BOOL ) ;
extern BOOL free_mem_hatch_pattern_ (int i_type);
extern void set_menu_hatch_pattern (void);

extern double Atan2 (double y, double x);
extern double get_angle_tangent_to_object_ (void *ptr_ob, double df_x, double df_y);
extern void Rotate_Point(double si, double co,
                         double x1, double y1,  			    /*center point*/
                         double x2, double y2, double *x, double *y); /*rotate point*/

extern int Save_Update_flex(int save_upd, int *curr_h, int *curr_v);

extern char *Get_PTR__GTMPFIRST(void);
extern void Set_PTR__GTMPFIRST(char *first);
extern char *Get_PTR__GTMPLAST(void);
extern void Set_PTR__GTMPLAST(char *last);

extern void Get_Limits (long off, long offk, int atrybut, double *xmin, double *xmax, double *ymin, double *ymax) ;

extern void disable_F11(void);
extern void enable_F11(void);

extern BOOL get_block_changed(void);

#define MaxLen 255
#define r18 18
#define r22 16 //22
#define r12 12
#define cursor_off(X,Y) out_cur_off(X,Y)
#define cursor_on(X,Y) out_cur_on(X,Y)

static TEXT TextG_bak=Tdef;
static LINIA LiniaG_bak=Ldef;
static OKRAG OkragG_bak=Odef;
static WIELOKAT SolidG_bak = S4def;
static LUK LukG_bak=ldef;
static T_Point PointG_bak=PointDef;
static B_PCX PcxG_bak=PCXdef;

int n_list_begin = 0;
int n_list_begin_bkg = 0;
int n_list_begin_patterns = 0;
int n_list_till = 0;
int n_list_end = 1000;
static double block_angle=0.0;

char catalog_fn[MaxLen] = "";

char *background_file_name_ptr;

BOOL hatching_in_progress = FALSE;

int npA;
static void nooop_(double a, double b)
{}

enum HATCH {
	ID_PATTERN = 0,
	ID_PATTERN_FOLDER = 1,
	ID_ANGLE_ROTATE = 2,
	ID_SCALE = 3,
	ID_SNAP_BASE = 4,
	ID_SNAP_BASE_POINT = 5,
	ID_DIST = 6,
	ID_INSULATION = 7,
	ID_VPATTERN_ANGLE = 9,
	ID_VPATTERN_PARALLEL = 10,
	ID_VPATTERN_PERPENDICULAR = 11,
	ID_IMAGE_PATTERN = 12,
	ID_GRAB_IMAGE_PATTERN = 17,
	ID_IMAGE_PATTERN_SCALE = 18,
	ID_IMAGE_PATTERN_ANGLE = 25,
	ID_IMAGE_PATTERN_DX = 29,
	ID_IMAGE_PATTERN_DY = 39,
	ID_TRANSLUCENCY = 561,
	ID_END = 49
};

static double DX,DY,Px,Py ;
static BOOL b_check_arc ;
static int w__object_no ;
char symbol_name[MAXPATH];

static double fixed_block_scale_x=1.0, fixed_block_scale_y=1.0;
static double current_block_scale_x=1.0, current_block_scale_y=1.0;

TMENU mBlok_Imp = {7, 0, 0, 32, 56, 4, ICONS | TADD, CMNU, CMBR, CMTX, 0, COMNDmnr, 0,0,0,(POLE(*)[]) &pmBlok_Imp, NULL, NULL} ;

TMENU mBlok_Divide = { 1, 0, 0, 10, 56, 4, ICONS | TADD, CMNU, CMBR, CMTX, 0, COMNDmnr+4, 0,0,0,(POLE(*)[]) &pmBlok_Divide, NULL, NULL };

static TMENU mBlok_Imp_GEO = {5, 0, 0, 10, 56, 4, ICONS | TADD, CMNU, CMBR, CMTX, 0, COMNDmnr, 0,0,0,(POLE(*)[]) &pmBlok_Imp_GEO, NULL, NULL} ;

void set_Px_Py(double Px_, double Py_)
{
    Px=Px_;
    Py=Py_;
}

void change_properties_h
		(char  	  *adp,
		 char  	  *adk,
		 T_change_param   *lps_change_param)

{
  LINIA *ob;
  WIELOKAT *obw;
  TEXT *ptrs_t ;

  if (adp == NULL || adk == NULL)
  {
    return ;
  }
  obiekt_tok_all ((char*)adp, adk, (char **)&ob, ONieOkreslony) ;
  while (ob != NULL)
  {
    if (ob->atrybut == Ablok &&  ob->obiekt != OdBLOK)
    {
       if (lps_change_param->b_layer)
       {
			ob->warstwa = lps_change_param->layer ;
       }
       if (lps_change_param->b_color)
       {
			ob->kolor = lps_change_param->color ;
       }
       if (ob->obiekt == Olinia || ob->obiekt == Oluk || ob->obiekt == Ookrag)
       {
		 if (lps_change_param->b_width)
		 {
		   ob->typ = (ob->typ & 31) | lps_change_param->width ;
		 }
		 if (lps_change_param->b_type)
		 {
		   ob->typ = ((ob->typ & 224) / 32) | lps_change_param->type ;
		 }
       }
	   if (ob->obiekt == Owwielokat)
		   {
		   if (lps_change_param->b_drawpoly)
			   obw = (WIELOKAT *)ob;
		       obw->drawpoly = 1;
		   }
    }
    obiekt_tok_all (NULL, adk, (char **)&ob, ONieOkreslony) ;
  }

  return ;
}

void backup_type_width_kolor(void)
{
    memmove(&TextG_bak, &TextG, sizeof(TEXT));
    memmove(&LiniaG_bak, &LiniaG, sizeof(LINIA));
    memmove(&OkragG_bak, &OkragG, sizeof(OKRAG));
    memmove(&SolidG_bak, &SolidG, sizeof(WIELOKAT));
    memmove(&LukG_bak, &LukG, sizeof(LUK));
    memmove(&PointG_bak, &PointG, sizeof(T_Point));
}

void restore_type_width_kolor(void)
{
    memmove(&TextG, &TextG_bak, sizeof(TEXT));
    memmove(&LiniaG, &LiniaG_bak, sizeof(LINIA));
    memmove(&OkragG, &OkragG_bak, sizeof(OKRAG));
    memmove(&SolidG, &SolidG_bak, sizeof(WIELOKAT));
    memmove(&LukG, &LukG_bak, sizeof(LUK));
    memmove(&PointG, &PointG_bak, sizeof(T_Point));
}

BOOL change_block_size(void *ptr, long del_size)
/*---------------------------*/
{
  BLOK 			*ptrs_block ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  T_Desc_Ex_Block_Old   *ptrs_desc_bl_old ;
  int 			len_desc_old, len_desc_new ;
  T_Desc_Ex_Block	*ptr_buf ;

  ptrs_block = (BLOK*)ptr ;
  ptrs_block->n += del_size;
  return TRUE ;
}


BOOL set_block_flag (void *ptr)
/*---------------------------*/
{
  BLOK 			*ptrs_block ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  T_Desc_Ex_Block_Old   *ptrs_desc_bl_old ;
  int 			len_desc_old, len_desc_new ;
  T_Desc_Ex_Block	*ptr_buf ;

  ptrs_block = (BLOK*)ptr ;
  ptrs_block->blok = 1;
  return TRUE ;
}


BOOL set_insert_point (double x, double y, void *ptr)
/*----------------------------------------------------------*/
{
  BLOK 			*ptrs_block ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  T_Desc_Ex_Block_Old   *ptrs_desc_bl_old ;
  int 			len_desc_old, len_desc_new ;
  T_Desc_Ex_Block	*ptr_buf ;
  char *ptrs_long;

  if (ptr == NULL) return FALSE;

  ptrs_block = (BLOK*)ptr ;
  //if (ptrs_block->obiekt != OdBLOK || w__object_no != 1)
  if (ptrs_block->obiekt != OdBLOK)
  {
    return FALSE ;
  }
  if (0 == ptrs_block->dlugosc_opisu_obiektu)
  {
    return FALSE ;	/*pierwsza wersja blokow nie zawierala opisu bloku*/
  }
  ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
  if (ptrs_desc_bl->flags != EBF_IP ) /*EBF_Old  i 30 w wyniku bledu w save_spec0*/
  {
    len_desc_old = ptrs_block->dlugosc_opisu_obiektu ;
    ptrs_desc_bl_old = (T_Desc_Ex_Block_Old *)(&ptrs_block->opis_obiektu [0]) ;
    len_desc_new = len_desc_old + 2 * sizeof (float) ;
    if (NULL == (ptr_buf = (T_Desc_Ex_Block*)malloc /*getmem*/ (len_desc_new)))
    {
      return FALSE ;
    }
    ptr_buf->flags = EBF_IP ;
    ptr_buf->len = ptrs_desc_bl_old->len ;
    strcpy (&ptr_buf->sz_type [0], &ptrs_desc_bl_old->sz_type [0]) ;
    ptrs_long = Change_Block_Descript (ptrs_block, (void *)ptr_buf,len_desc_new) ;
    free(ptr_buf);
    ptrs_block = (BLOK*)ptrs_long;
    if (ptrs_block == NULL)
    {
      return FALSE ;
    }
    ADP = (char  *)ptrs_block ;  /*Change_Block_Description 'przesowa
					dane, a nie zmienia ADK*/
    ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
  }
  ptrs_desc_bl->x = x ;
  ptrs_desc_bl->y = y ;
  return TRUE ;
}

BOOL change_block_name (void *ptr, BOOL set_angle, double angle)
/*------------------------------------------------------------*/
{
  BLOK 			*ptrs_block ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  T_Desc_Ex_Block_Old   *ptrs_desc_bl_old ;
  int 			len_desc_old, len_desc_new ;
  T_Desc_Ex_Block	*ptr_buf ;
  float angle_f;

  ptrs_block = (BLOK*)ptr ;
  ptrs_block->flag=0;   //rezerwa
  if (ptrs_block->obiekt != OdBLOK)
  {
    return FALSE ;
  }
  if (0 == ptrs_block->dlugosc_opisu_obiektu)
  {
    return FALSE ;
  }
  ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu[0]);
  if (!set_angle)
  {
	  if (ptrs_desc_bl->len > 0) ptrs_desc_bl->sz_type[0] = '.';
  }
  else
  {
	  angle_f = (float)angle * 360 / Pi2;
	  if (ptrs_block->dlugosc_opisu_obiektu >= 4)
	  {
		memmove(ptrs_block->opis_obiektu, &angle_f, sizeof(float));
	  }
	  else if (ptrs_desc_bl->len > 0) ptrs_desc_bl->sz_type[0] = '.';
  }
  return TRUE ;
}

static void skala_import (void)
/*---------------------------*/
{
  char sk [MaxTextLen] ;
  int retval_no = 2 ;
  double buf_ret [2] ;
  double k01, k02, k01_, k02_ ;

  if (Check_if_Equal(current_block_scale_x, current_block_scale_y)==FALSE)
      sprintf(sk, "%#8.4f;%#8.4f", current_block_scale_x, current_block_scale_y);
  else sprintf(sk, "%#8.4f", current_block_scale_x);
  if (!get_string (sk, "", MaxTextLen, 0, 17)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  k02 = k01 = buf_ret [0] ;
  if ( retval_no > 1)
  {
    k02 = buf_ret [1] ;
  }
  if (k01 == 0 || k02 == 0)
  {
    return ;
  }

  k01_=k01 / current_block_scale_x;
  k02_=k02 / current_block_scale_y;

  current_block_scale_x=k01;
  current_block_scale_y=k02;

    if (Check_if_Equal(current_block_scale_x, current_block_scale_y)==FALSE)
    sprintf(sk, "%#8.4f;%#8.4f", current_block_scale_x, fixed_block_scale_y);
    else sprintf(sk, "%#8.4f", current_block_scale_x);
    menu_par_new ((*mBlok_Imp.pola)[0].txt, sk) ;

  if ((Check_if_Equal(k01_, 1.0)==FALSE) || (Check_if_Equal(k02_, 1.0)==FALSE)) {
      Cur_offd(X, Y);

      transformacja_blok(ADP, ADK, Px, Py, k01_, k02_, Tskala, 0);
      if (FALSE == Check_if_Equal (k01_, k02_))
      {
          trans_scale_arcs (Px,Py, k01_, k02_, FALSE) ;
      }

      Cur_ond(X, Y);
  }

}

static void ustalona_skala_import(void)
/*---------------------------*/
{
    char sk [MaxTextLen] ;
    int retval_no = 2 ;
    double buf_ret [2] ;
    double k01, k02, k01_, k02_ ;

    if (Check_if_Equal(current_block_scale_x, current_block_scale_y)==FALSE)
        sprintf(sk, "%#8.4f;%#8.4f", current_block_scale_x, current_block_scale_y);
    else sprintf(sk, "%#8.4f", current_block_scale_x);
    if (!get_string (sk, "", MaxTextLen, 0, 198)) return ;
    if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
    {
        return ;
    }
    k02 = k01 = buf_ret [0] ;
    if ( retval_no > 1)
    {
        k02 = buf_ret [1] ;
    }
    if (k01 == 0 || k02 == 0)
    {
        return ;
    }

    k01_=k01 / current_block_scale_x;
    k02_=k02 / current_block_scale_y;

    current_block_scale_x=k01;
    current_block_scale_y=k02;
    fixed_block_scale_x=k01;
    fixed_block_scale_y=k02;

    if (Check_if_Equal(current_block_scale_x, current_block_scale_y)==FALSE)
        sprintf(sk, "%#8.4f;%#8.4f", current_block_scale_x, fixed_block_scale_y);
    else sprintf(sk, "%#8.4f", current_block_scale_x);
    menu_par_new ((*mBlok_Imp.pola)[0].txt, sk) ;
    menu_par_new ((*mBlok_Imp.pola)[1].txt, sk) ;

    if ((Check_if_Equal(k01_, 1.0)==FALSE) || (Check_if_Equal(k02_, 1.0)==FALSE)) {
        Cur_offd(X, Y);

        transformacja_blok(ADP, ADK, Px, Py, k01_, k02_, Tskala, 0);
        if (FALSE == Check_if_Equal (k01_, k02_))
        {
            //trans_scale_arcs_ (ADP, ADK, Px,Py, k01_, k02_, FALSE) ;
            trans_scale_arcs (Px,Py, k01_, k02_, FALSE) ;
            //set_block_change_mode_scale ((long)(ADP - dane), (long)(ADK - dane)) ;
        }
        Cur_ond(X, Y);
    }
}

void obrot_import (void)
/*---------------------------*/
{
  char sk [MaxTextLen] ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double kos, koc ;
  double kat_obrotu;

  sk [0] = '\0' ;
  if (!get_string (sk, "", MaxTextLen, 0, 16)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }

  kat_obrotu = buf_ret[0] * Pi / 180;
  kos = sin(kat_obrotu);
  koc = cos(kat_obrotu);
  Cur_offd (X, Y) ;
  transformacja_blok (ADP, ADK, Px, Py, kos, koc, Tobrot,0);
  block_angle += kat_obrotu;
  block_angle = Angle_Normal(block_angle);

    sprintf(eA.st, "%#8.4f", block_angle*180/Pi);
    Out_Edited_Draw_Param ((ESTR *)&eA, TRUE) ;

  Cur_ond (X, Y) ;
}

void obrot_import_K(void)
/*---------------------------*/
{
	char sk[MaxTextLen];
	int retval_no = 1;
	double buf_ret[1];
	double kos, koc;
	double kat_obrotu;

	sk[0] = '\0';
	if (!get_string(sk, "", MaxTextLen, 0, 16)) return;
	if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
	{
		return;
	}

	kat_obrotu = buf_ret[0] * Pi / 180.0;
	kos = sin (kat_obrotu - block_angle) ;
	koc = cos (kat_obrotu - block_angle) ;
	Cur_offd(X, Y);
	transformacja_blok(ADP, ADK, Px, Py, kos, koc, Tobrot, 0);
	block_angle = kat_obrotu;
	block_angle = Angle_Normal(block_angle);

    sprintf(eA.st, "%#8.4f", block_angle*180/Pi);
    Out_Edited_Draw_Param ((ESTR *)&eA, TRUE) ;

	Cur_ond(X, Y);
}

void	obrot_kopiuj_paral(double kat)
{
	double	kos, koc;
	double kat_linii = 0;
	unsigned typ;
	void  *adr;
	PLINIA PL;
    LINIA L=Ldef;
	char sk[MaxTextLen];
	unsigned int sei_akt, sel_gor;
    BOOL ret;
    int l;
    double xx, yy, si, co, m;

	Cur_offd(X, Y);

	sel_akt = sel.akt;
	sel_gor = sel.gor;
	sel.akt = ASel;
	sel.gor = NGSel;

	Cur_ond(X, Y);

	typ = Blinia | Btekst | Bluk | Bokrag | Bkolo | Bwwielokat | Bellipse | Bfilledellipse | Bellipticalarc | Bspline;
	if ((adr = select_n(&typ, NULL, 75)) != NULL)
	{
        switch (typ) {
            case Btekst: kat_linii = Angle_Normal(kat + ((TEXT *) adr)->kat);
            break;
            case Blinia:
                parametry_lini((LINIA *) adr, &PL);
                kat_linii = Angle_Normal(kat + (PL.kat * Pi / 180.0));
            break;
            case Bluk:
                najblizszyl_(&X, &Y, adr);
                kat_linii = Angle_Normal(Atan2(Y-((LUK*)adr)->y, X-((LUK*)adr)->x)+kat);
                break;
            case Bokrag:
            case Bkolo:
                najblizszyO_(&X, &Y, adr);
                kat_linii = Angle_Normal(Atan2(Y-((OKRAG*)adr)->y, X-((OKRAG*)adr)->x)+kat);
                break;
            case Bwwielokat:
                wielokat_linia_ (&X, &Y, adr, &L, najblizszyL_) ;
                parametry_lini(&L, &PL);
                kat_linii = Angle_Normal(kat + (PL.kat * Pi / 180.0));
                break;
            case Bellipse:
            case Bfilledellipse:
                najblizszyE(&X,&Y,adr);
                xx=X-((ELLIPSE*)adr)->x;
                yy=Y-((ELLIPSE*)adr)->y;
                si=sin(-((ELLIPSE*)adr)->angle);
                co=cos(-((ELLIPSE*)adr)->angle);
                Rotate_Point(si,co, 0,0,xx,yy,&xx,&yy);
                m=(-xx*((ELLIPSE*)adr)->ry*((ELLIPSE*)adr)->ry)/(yy*((ELLIPSE*)adr)->rx*((ELLIPSE*)adr)->rx);
                kat_linii=Angle_Normal(atan(m)+((ELLIPSE*)adr)->angle + kat);
                if (yy<0) kat_linii+=Pi;
                break;
            case Bellipticalarc:
                ret = najblizszyEA_(&X,&Y,adr);
                xx=X-((ELLIPSE*)adr)->x;
                yy=Y-((ELLIPSE*)adr)->y;
                si=sin(-((ELLIPSE*)adr)->angle);
                co=cos(-((ELLIPSE*)adr)->angle);
                Rotate_Point(si,co, 0,0,xx,yy,&xx,&yy);
                m=(-xx*((ELLIPSE*)adr)->ry*((ELLIPSE*)adr)->ry)/(yy*((ELLIPSE*)adr)->rx*((ELLIPSE*)adr)->rx);
                kat_linii=Angle_Normal(atan(m)+((ELLIPSE*)adr)->angle + kat);
                if (yy<0) kat_linii+=Pi;
                break;
            case Bspline:
                l = mwspline_(&X, &Y, adr, najblizszyLx_);
                if (l>0) kat_linii=Angle_Normal(get_angle_tangent_to_object_ (adr, X, Y) + kat);
                break;
            default:
            break;
        }
	}

	Cur_offd(X, Y);

	sel.akt = sel_akt;
	sel.gor = sel_gor;

	kos = sin(kat_linii - block_angle);
	koc = cos(kat_linii - block_angle);
	transformacja_blok(ADP, ADK, Px, Py, kos, koc, Tobrot, 0);
	block_angle = kat_linii;
	block_angle = Angle_Normal(block_angle);

    sprintf(eA.st, "%#8.4f", block_angle*180/Pi);
    Out_Edited_Draw_Param ((ESTR *)&eA, TRUE) ;

	Cur_ond(X, Y);

}

void obrot_kopiuj_paralK(void)
{
	double kat;
	char sk[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];

    if (get_string(sk, "", MaxTextLen, 0, 15))
    {
        if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
        {
            return;
        }
        kat = buf_ret[0];
        kat = kat * Pi / 180.0;

        obrot_kopiuj_paral(kat);

    }
}

static void obrot_import_XZ (void)
/*------------------------------*/
{
  char sk [MaxTextLen] ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double kos, koc ;

  sk [0] = '\0' ;
  if (!get_string (sk, "", MaxTextLen, 0, 16)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  kos = sin (buf_ret [0] * Pi / 180.0) ;
  koc = cos (buf_ret [0] * Pi / 180.0) ;
  Cur_offd (X, Y) ;
  transformacja_blok (ADP, ADK, Px, Py, kos, koc, TobrotXZ,0);
  Cur_ond (X, Y) ;
}

static void obrot_import_YZ (void)
/*------------------------------*/
{
  char sk [MaxTextLen] ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double kos, koc ;

  sk [0] = '\0' ;
  if (!get_string (sk, "", MaxTextLen, 0, 16)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  kos = sin (buf_ret [0] * Pi / 180.0) ;
  koc = cos (buf_ret [0] * Pi / 180.0) ;
  Cur_offd (X, Y) ;
  transformacja_blok (ADP, ADK, Px, Py, kos, koc, TobrotYZ,0);
  Cur_ond (X, Y) ;
}

void obrot_import_kat (double kat_obrotu)
/*--------------------------------------------*/
{
  double kos, koc ;
  double kat_obrotu_r;

  kat_obrotu_r = kat_obrotu * Pi / 180.0 - block_angle;
  kat_obrotu_r = Angle_Normal(kat_obrotu_r);
  
  kos = sin(kat_obrotu_r); // -block_angle);
  koc = cos(kat_obrotu_r); // -block_angle);
  Cur_offd (X, Y) ;
  transformacja_blok (ADP, ADK, Px, Py, kos, koc, Tobrot,0);
  block_angle = kat_obrotu * Pi / 180.0;

  if (eA.st != NULL) {
      sprintf(eA.st, "%#8.4f", block_angle * 180 / Pi);
      Out_Edited_Draw_Param((ESTR *) &eA, TRUE);
  }

  Cur_ond (X, Y) ;
}

void obrot_import_kat_rel (double kat_obrotu)
/*--------------------------------------------*/
{
    double kos, koc ;
    double kat_obrotu_r;

    kat_obrotu_r = kat_obrotu * Pi / 180.0;

    kos = sin(kat_obrotu_r);
    koc = cos(kat_obrotu_r);
    Cur_offd (X, Y) ;
    transformacja_blok (ADP, ADK, Px, Py, kos, koc, Tobrot,0);
    block_angle += kat_obrotu_r;
    block_angle = Angle_Normal(block_angle);

    sprintf(eA.st, "%#8.4f", block_angle*180/Pi);
    Out_Edited_Draw_Param ((ESTR *)&eA, TRUE) ;

    Cur_ond (X, Y) ;
}


static void lustro_y_y_import (void)
/*---------------------------*/
{
  char sk [MaxTextLen] ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double kos, koc ;

  Cur_offd (X, Y) ;
  transformacja_blok (ADP, ADK, Px, Py, Px, Py + 100, Tmirror,0);
  Cur_ond (X, Y) ;
}

static void lustro_x_x_import (void)
/*---------------------------*/
{
  char sk [MaxTextLen] ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double kos, koc ;

  Cur_offd (X, Y) ;
  transformacja_blok (ADP, ADK, Px, Py, Px + 100, Py, Tmirror,0);
  Cur_ond (X, Y) ;
}

static void center_to_cursor_import(double *X0, double *Y0)
{
    double xmin, xmax, ymin, ymax ;
    double x0, y0;
    BLOK 			*ptrs_block ;
    T_Desc_Ex_Block 	*ptrs_desc_bl ;
    char *ADP_, *ADK_;

    Cur_offd (X, Y) ;
    ADP_=dane;
    ptrs_block = (BLOK*)ADP_ ;
    ADK_=ADP_+sizeof(NAGLOWEK)+ptrs_block->n;
    ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
    Get_Limits (ADP_-dane, ADK_-dane, Ablok, &xmin, &xmax, &ymin, &ymax) ;
    x0=(xmax+xmin)/2.0;
    y0=(ymax+ymin)/2.0;
    ptrs_desc_bl->x=(float)x0;
    ptrs_desc_bl->y=(float)y0;

    Px=x0;
    Py=y0;

    *X0=x0;
    *Y0=y0;

    Cur_ond (X, Y) ;
}

static int e_imp_o (BOOL b_graph_value)
/*---------------------*/
{
  double a, b, kos, koc ;
  double kat_obrotu_r;


    b_graph_value = b_graph_value ;
    if (eA.val_no < 1) return 0;
    a = eA.values [0] ;

    kat_obrotu_r = a * Pi / 180.0 - block_angle;
    kat_obrotu_r = Angle_Normal(kat_obrotu_r);

    kos = sin(kat_obrotu_r);
    koc = cos(kat_obrotu_r);
    Cur_offd (X, Y) ;
    transformacja_blok (ADP, ADK, Px, Py, kos, koc, Tobrot,0);

    block_angle = a * Pi / 180.0;
    Cur_ond (X, Y) ;

  return 1 ;
}

static void cur_offpki(double x,double y)
{
    flip_screen();
}

static void cur_offpkisel(double x, double y)
{
    flip_screen();
}

static void cur_onpki(double x,double y)
{
  ////cursel_on(x, y);
  DX=x-Px;  DY=y-Py;
  out_blok1(DX,DY,0,0,Tprzesuw,0);
  cursel_on(x, y);
}

static void cur_onpkisel(double x, double y)
{
    ////cursel_on(x, y);
	DX = x - Px;  DY = y - Py;
	out_blok1(DX, DY, 0, 0, Tprzesuw, 0);
	cursel_on(x, y);
}

int FileNameI (char *fn,int Max_Len)
{
  BOOL ret;

  Max_Len = Max_Len ;
  ret = Load_File (fn, INI_BLOK, FALSE);
  return ret;
}

int FileNameIDXF(char *fn, int Max_Len)
{
	BOOL ret;

	Max_Len = Max_Len;
	ret = Load_File(fn, INI_BLOK_DXF, FALSE);
	return ret;
}

#define DX_BUT_def 72
#define DY_BUT_def 48
#define dx_BUT 5
#define dy_BUT 3 //5
#define DXButCat 40 //48
#define DYButCat 28 //32

#define DYComboBox	12

#define BlockFileNameMax 512

int DX_BUT[] = { 24, 32, 42, 54, 72, 90, 112, 140, 175};
int DY_BUT[] = { 16, 21, 28, 36, 48, 60, 75, 94, 118};
int ButSizeBlk = 4;
int ButSizeBkg = 4;
int ButSizeCat = 4;
int ButSizePat = 4;
int ButSizeMax = 8; //6;


static  DARK_LIGHT_LINE line_cat[]=
{
	{ 0, 0, 100, 1, LINE_NULL, COLOR_NULL},
};

static LABEL lab_cat[] = 
{
	 {0, 0, 0, 0, COLOR_NULL,COLOR_NULL,"",0,0},
};

static GROUP_BOX gr_box_cat[] =
{
	{ 0, 0, 100, 100,
	COLOR_NULL ,COLOR_NULL, "", 0, NULL,
  },
};

static INPUTLINE edit_cat[] =
{
	{
	0, 0, 100, 100 ,
	COLOR_NULL,COLOR_NULL, COLOR_NULL,COLOR_NULL,COLOR_NULL,
	5,0,1, "", -1,
	},
};

#define nof_but  BlockFileNameMax + 7
#define nof_but_bkg  BlockFileNameMax + 5
#define nof_but_pattern  BlockFileNameMax + 5
#define nof_but_blk  BlockFileNameMax + 6


static BUTTON buttons_def =
{ 0, 0, DX_BUT_def, DY_BUT_def, COLOR_NULL,
COLOR_NULL, COLOR_NULL, "", 0, B_PUSHBUTTON, 0, 1, 0, ID_IMPORT,
};

static BUTTON buttons_blk_def =
        { 0, 0, DX_BUT_def, DY_BUT_def, COLOR_NULL,
          COLOR_NULL, COLOR_NULL, "", 0, B_PUSHBUTTON, 0, 1, 0, ID_INSERT,
        };

#define FIXED_BUT 7

static BUTTON buttons_cat[nof_but] =
{
  { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "UP", 0,B_PUSHBUTTON, 69, 1,0, ID_UP,
  },
  { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "DN", 0,B_PUSHBUTTON, 70, 1,0, ID_DN,
  },
  { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "*", 0,B_PUSHBUTTON, 71, 1,0, ID_FOLDER,
  },
  { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "*", 0,B_PUSHBUTTON, 72, 1,0, ID_FOLDER0,
  },
   { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "B", 0,B_PUSHBUTTON, 73, 1,0, ID_BIGGER,
  },
   { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "S", 0,B_PUSHBUTTON, 74, 1,0, ID_SMALLER,
  },
  { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "ESC", 0,B_PUSHBUTTON, 99, 1,0, ID_ESCAPE,
  },

};

#define FIXED_BUT_BKG 6

static BUTTON buttons_bkg[nof_but_bkg] =
{
  { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "UP END", 0,B_PUSHBUTTON, 120, 1,0, ID_UP_END,
  },
  { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "UP", 0,B_PUSHBUTTON, 69, 1,0, ID_UP,
  },
  { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "DN", 0,B_PUSHBUTTON, 70, 1,0, ID_DN,
  },

   { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "B", 0,B_PUSHBUTTON, 73, 1,0, ID_BIGGER,
  },
   { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "S", 0,B_PUSHBUTTON, 74, 1,0, ID_SMALLER,
  },
  { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "ESC", 0,B_PUSHBUTTON, 99, 1,0, ID_ESCAPE,
  },

};

#define FIXED_BUT_BLK 9

static BUTTON buttons_blk[nof_but_blk] =
        {
                { 100, 100, DXButCat, DYButCat, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, "UP END", 0,B_PUSHBUTTON, 120, 1,0, ID_UP_END,
                },
                { 100, 100, DXButCat, DYButCat, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, "UP", 0,B_PUSHBUTTON, 69, 1,0, ID_UP,
                },
                { 100, 100, DXButCat, DYButCat, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, "DN", 0,B_PUSHBUTTON, 70, 1,0, ID_DN,
                },
                { 100, 100, DXButCat, DYButCat, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, "DN END", 0,B_PUSHBUTTON, 190, 1,0, ID_DN_END,
                },
                { 100, 100, DXButCat, DYButCat, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, "B", 0,B_PUSHBUTTON, 73, 1,0, ID_BIGGER,
                },
                { 100, 100, DXButCat, DYButCat, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, "S", 0,B_PUSHBUTTON, 74, 1,0, ID_SMALLER,
                },
                { 100, 100, DXCheckBox, DYComboBox, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, "", 0,B_RADIOBUTTON, 0, 1,0, ID_ALPHABET,
                },
                { 100, 100, DXCheckBox, DYComboBox, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, "", 0,B_RADIOBUTTON, 0, 1,0, ID_TIME,
                },
                { 100, 100, DXButCat, DYButCat, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, "ESC", 0,B_PUSHBUTTON, 99, 1,0, ID_ESCAPE,
                },
        };

#define FIXED_BUT_PATTERN 5

static BUTTON buttons_pattern[nof_but_pattern] =
{
  { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "UP", 0,B_PUSHBUTTON, 69, 1,0, ID_UP,
  },
  { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "DN", 0,B_PUSHBUTTON, 70, 1,0, ID_DN,
  },

   { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "B", 0,B_PUSHBUTTON, 73, 1,0, ID_BIGGER,
  },
   { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "S", 0,B_PUSHBUTTON, 74, 1,0, ID_SMALLER,
  },
  { 100, 100, DXButCat, DYButCat, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "ESC", 0,B_PUSHBUTTON, 99, 1,0, ID_ESCAPE,
  },

};

static IMAGE images_cat[] =
{
	{ 100, 100, 48, 48, 114,catalog_tip},
	{ 100, 100, 32, 32, 0,""},
	{ 100, 100, 32, 32, 0,""},
};

static IMAGE images_bkg[] =
{
	{ 100, 100, 48, 48, 114,background_tip},
	{ 100, 100, 32, 32, 0,""},
	{ 100, 100, 32, 32, 0,""},
};

static IMAGE images_blk[] =
        {
                { 100, 100, 48, 48, 114,blocklist_tip},
                { 100, 100, 32, 32, 191,alpha_sorting_tip},
                { 100, 100, 32, 32, 192,timelapse_tip},
        };

static IMAGE images_pattern[] =
{
	{ 100, 100, 48, 48, 114,patterns_tip},
	{ 100, 100, 32, 32, 0,""},
	{ 100, 100, 32, 32, 0,""},
};


static TDIALOG catalog_dlg =
{
	5, 5, WIDTH_WORLD-10, HEIGHT_WORLD-10,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0x40,  0, 0, 
	catalog,
	0, &line_cat,
	0, &lab_cat,
	0, &gr_box_cat,
	0, &edit_cat,
	1, &images_cat,
	7, &buttons_cat,
	0, NULL,
	0,NULL,
    0,NULL, //Sliders
	NULL,
	NULL,
	0,
	0,
    NULL,
};

static TDIALOG background_dlg =
{
	5, 5, WIDTH_WORLD - 10, HEIGHT_WORLD - 10,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0x40,  0, 0,
	background,
	0, &line_cat,
	0, &lab_cat,
	0, &gr_box_cat,
	0, &edit_cat,
	1, &images_bkg,
	6, &buttons_bkg,
	0, NULL,
	0,NULL,
    0,NULL, //Sliders
	NULL,
	NULL,
	0,
	0,
    NULL,
};

static TDIALOG patterns_dlg =
{
	5, 5, WIDTH_WORLD - 10, HEIGHT_WORLD - 10,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0x40,  0, 0,
	patterns,
	0, &line_cat,
	0, &lab_cat,
	0, &gr_box_cat,
	0, &edit_cat,
	1, &images_pattern,
	5, &buttons_pattern,
	0, NULL,
	0,NULL,
    0,NULL, //Sliders
	NULL,
	NULL,
	0,
	0,
    NULL,
};

static TDIALOG blocklist_dlg =
        {
                5, 5, WIDTH_WORLD-10, HEIGHT_WORLD-10,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0x40,  0, 0,
                blocklist,
                0, &line_cat,
                0, &lab_cat,
                0, &gr_box_cat,
                0, &edit_cat,
                3, &images_blk,
                9, &buttons_blk,
                0, NULL,
                0,NULL,
                0,NULL, //Sliders
                NULL,
                NULL,
                0,
                0,
                NULL,
        };


static int proc_dlg_catalog(int n)
/*------------------------------*/
{
	int ret = Dlg_Ret_Val_Cancel;

	switch (n)
	{
	case ID_UP:
		ret = Dlg_Ret_Val_UP;
		break;
	case ID_DN:
		ret = Dlg_Ret_Val_DN;
		break;
	case ID_FOLDER:
		ret = Dlg_Ret_Val_Folder;
		break;
	case ID_FOLDER0:
		ret = Dlg_Ret_Val_Folder0;
		break;
	case ID_BIGGER:
		ret = Dlg_Ret_Val_Bigger;
		break;
	case ID_SMALLER:
		ret = Dlg_Ret_Val_Smaller;
		break;
	case ID_ESCAPE:
		ret = Dlg_Ret_Val_Cancel;
		break;
	default:
		ret = n;
		break;
	}
	return ret;
};

static int proc_dlg_blocklist(int n)
/*------------------------------*/
{
    int ret = -1;
    int check1;

    switch (n)
    {
        case ID_UP_END:
            ret = Dlg_Ret_Val_UP_END;
            break;
        case ID_UP:
            ret = Dlg_Ret_Val_UP;
            break;
        case ID_DN:
            ret = Dlg_Ret_Val_DN;
            break;
        case ID_DN_END:
            ret = Dlg_Ret_Val_DN_END;
            break;
        case ID_BIGGER:
            ret = Dlg_Ret_Val_Bigger;
            break;
        case ID_SMALLER:
            ret = Dlg_Ret_Val_Smaller;
            break;
        case ID_ALPHABET:
            if (sorting_type==1) ret = Dlg_Ret_Val_Alphabet;
            break;
        case ID_TIME:
            if (sorting_type==0)  ret = Dlg_Ret_Val_Time;
            break;
        case ID_ESCAPE:
            ret = Dlg_Ret_Val_Cancel;
            break;
        default:
            ret = n;
            break;
    }
    return ret;
};

static int proc_dlg_background(int n)
/*------------------------------*/
{
	int ret = Dlg_Ret_Val_Cancel;

	switch (n)
	{
	case ID_UP_END:
		ret = Dlg_Ret_Val_UP_END;
		break;
	case ID_UP:
		ret = Dlg_Ret_Val_UP;
		break;
	case ID_DN:
		ret = Dlg_Ret_Val_DN;
		break;
	case ID_BIGGER:
		ret = Dlg_Ret_Val_Bigger;
		break;
	case ID_SMALLER:
		ret = Dlg_Ret_Val_Smaller;
		break;
	case ID_ESCAPE:
		ret = Dlg_Ret_Val_Cancel;
		break;
	default:
		ret = n;
		break;
	}
	return ret;
};

static int proc_dlg_patterns(int n)
/*------------------------------*/
{
	int ret = Dlg_Ret_Val_Cancel;

	switch (n)
	{
	case ID_UP:
		ret = Dlg_Ret_Val_UP;
		break;
	case ID_DN:
		ret = Dlg_Ret_Val_DN;
		break;
	case ID_BIGGER:
		ret = Dlg_Ret_Val_Bigger;
		break;
	case ID_SMALLER:
		ret = Dlg_Ret_Val_Smaller;
		break;
	case ID_ESCAPE:
		ret = Dlg_Ret_Val_Cancel;
		break;
	default:
		ret = n;
		break;
	}
	return ret;
};

int GetBackground(char *fn, int Max_Len, int opcja, char* pathname, char *pathname0)
{
	int Ret_Val;
	BOOL ret;
	char **file_list = NULL;
	int n_list;
	char background_file_name[BlockFileNameMax + 1][MAXPATH];
	int i, j;

	int dx_win_cat, dy_win_cat, x_win_cat, y_win_cat, x_win, y_win, x1_win, y1_win;
	double wsp_xxx, wsp_yyy, wsp_x, wsp_y;
	double dx_but, dy_but, ddx_but, ddy_but;
	int nx_but, ny_but, dx, dy;
	int d_n_list, n_list_max;
	BOOL go_break;
	long pXp1, pYp1, pXk1, pYk1;
	long pXp2, pYp2, pXk2, pYk2;
	double XXp, YYp, XXk, YYk;
	BOOL reload = TRUE;
	int max_y;
	int background_dlg_dx;
	int background_dlg_dy;
	char drive[MAXDRIVE];
	char dir[MAXDIR];
	char file[MAXFILE];
	char ext[MAXEXT];
	int flags;
	char* dirptr;
	double X_bak, Y_bak;
	int ret_p;
    static int curr_h, curr_v;

    char title_suffix[32];
    int title_len;
    char *title;

    bMouse = TRUE;
    background_dlg.flags=0x40;

	while (1)
	{

		if (reload) {
            Max_Len = Max_Len;
            param_line_out();

            while (1) {
                ret = My_GetFiles(pathname, &n_list, "\\*.", "\\*.pcx", "\\*.png", "\\*.jpg", FALSE);

                if (ret) {
                    break;
                }
            }
        }
			//building dialog window for catalog

			wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
			wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

			dx_win_cat = getmaxx() - 8; // 10;
			dy_win_cat = getmaxy() - 8; // 10;

			background_dlg.dx = ((float)getmaxx() - 20.0) / wsp_x;
			background_dlg.dy = ((float)getmaxy() - 42.0 *((float)HEIGHT / 12.0) - 20) / wsp_y;

			background_dlg_dx = background_dlg.dx;
			background_dlg_dy = background_dlg.dy;

			background_dlg.x = 10 / wsp_x;
			background_dlg.y = 10 / wsp_y;

			Get_Default_Color_Dlg(&color_dlg);

			x_win_cat = 8;
			y_win_cat = 8;

			for (i = 0; i < FIXED_BUT_BKG; i++)
				buttons_bkg[i].x = background_dlg.dx - DXButCat - dx_BUT;

			buttons_bkg[0].y = dy_BUT;
			buttons_bkg[1].y = 2 * dy_BUT + DYButCat;
			buttons_bkg[2].y = 3 * dy_BUT + 2 * DYButCat;
			buttons_bkg[3].y = 4 * dy_BUT + 3 * DYButCat;
			buttons_bkg[4].y = 5 * dy_BUT + 4 * DYButCat;

			buttons_bkg[5].y = background_dlg.dy - DYButCat - dy_BUT;
			images_bkg[0].y1 = background_dlg.dy - DYButCat*2 - dy_BUT;
			images_bkg[0].x1 = background_dlg.dx - 32 - dx_BUT;

			param_line_out();


		n_list_till = 0;

		n_list_end = n_list;


		while (1)
		{
            ///////////////////////
            Save_Update_flex(0, &curr_h, &curr_v);

            pXp1 = get_pXp(); pYp1 = get_pYp(); pXk1 = get_pXk(); pYk1 = get_pYk();
            XXp = Xp;  YYp = Yp;  XXk = Xk;  YYk = Yk;

            Xp = 0; Yp = 0; Xk = 3000; Yk = 3000;  //to show objects behing drawinbg sheet in Catalog

            pXp2 = pXp1;
            pYp2 = getmaxy();
            pXk2 = getmaxx();
            pYk2 = 0;

            set_World(pXp2, pYp2, pXk2, pYk2);
			//////////////////////
			//calculating number of buttons each direction
			background_dlg.dx = background_dlg_dx;
			background_dlg.dy = background_dlg_dy;

			dx_but = DX_BUT[ButSizeBkg] * wsp_x;
			dy_but = DY_BUT[ButSizeBkg] * wsp_y;

			nx_but = (background_dlg.dx - 20 - DXButCat) / (DX_BUT[ButSizeBkg] + 5);
			ny_but = (background_dlg.dy - 10) / (DY_BUT[ButSizeBkg] + 5);

			if ((nx_but * ny_but) > BlockFileNameMax)
			{
				ny_but = (int)(BlockFileNameMax / nx_but);
			};

			background_dlg.SizeButtonT = FIXED_BUT_BKG + (nx_but*ny_but);

			/////////////////////////

			//we have n_list number of files
			n_list_max = nx_but * ny_but;
            if (n_list_end < n_list_begin_bkg) n_list_begin_blk=max(n_list_end-n_list_max, 0);
			d_n_list = n_list_end - n_list_begin_bkg;
			if (d_n_list > n_list_max) d_n_list = n_list_max;

			if (d_n_list > n_list) d_n_list = n_list;

			background_dlg.SizeButtonT = FIXED_BUT_BKG + d_n_list; // (nx_but*ny_but);

			for (i = 0; i < n_list_max; i++)
			{
				if (i < d_n_list)
				{
					sprintf(background_file_name[i], "\025%s%c%s", pathname, Slash, strarray[i + n_list_begin_bkg]);
				}
				else
					strcpy(background_file_name[i], "");

			}

			go_break = FALSE;
			for (j = 0; j < ny_but; j++)
			{
				for (i = 0; i < nx_but; i++)
				{
					memmove(&buttons_bkg[j*nx_but + i + FIXED_BUT_BKG], &buttons_def, sizeof(BUTTON));
					buttons_bkg[j*nx_but + i + FIXED_BUT_BKG].dx = DX_BUT[ButSizeBkg];
					buttons_bkg[j*nx_but + i + FIXED_BUT_BKG].dy = DY_BUT[ButSizeBkg];

					buttons_bkg[j*nx_but + i + FIXED_BUT_BKG].x = i * (DX_BUT[ButSizeBkg] + 5) + 5;
					buttons_bkg[j*nx_but + i + FIXED_BUT_BKG].y = j * (DY_BUT[ButSizeBkg] + 5) + 5;
					buttons_bkg[j*nx_but + i + FIXED_BUT_BKG].id = (j*nx_but + i + ID_IMPORT);
					buttons_bkg[j*nx_but + i + FIXED_BUT_BKG].txt = background_file_name[j*nx_but + i];
					if ((j*nx_but + i) > d_n_list)
					{
						go_break = TRUE;
						reload = TRUE;
						break;
					}
				}
				if (go_break) break;
			}

			//eventually there are only j rows
			//size of dialog correction 
			
			max_y = (j+1) * (DY_BUT[ButSizeBkg] + 5) + 10;

			if (max_y < (7 * (DYButCat + dy_BUT))) max_y = (7 * (DYButCat + dy_BUT));

			if (max_y < background_dlg.dy) background_dlg.dy = max_y;

			buttons_bkg[5].y = background_dlg.dy - DYButCat - dy_BUT;
			images_bkg[0].y1 = background_dlg.dy - DYButCat * 2 - dy_BUT;

			n_list_till = (n_list_begin_bkg + d_n_list);

            background_dlg.txt=pathname;

            sprintf(title_suffix,"%d...%d (%d)", n_list_begin_bkg+1, n_list_till, n_list);
            title_len=strlen(background_dlg.txt)+32;
            title=(char*)malloc(title_len);
            sprintf(title,"%s  %s", background_dlg.txt, title_suffix);
            background_dlg.txt=title;

            if (n_list_begin_bkg == 0) buttons_bkg[0].enable=buttons_bkg[1].enable=0;
            else buttons_bkg[0].enable=buttons_bkg[1].enable=1;

            if (n_list_till == n_list_end) buttons_bkg[2].enable=0;
            else buttons_bkg[2].enable=1;
			
			Info = 0;
			Ret_Val = Dialog(&background_dlg, &color_dlg, proc_dlg_background, bMouse);

            background_dlg.txt=pathname;
            free(title);

            set_World(pXp1, pYp1, pXk1, pYk1);
            Xp = XXp;  Yp = YYp;  Xk = XXk;  Yk = YYk;
            Save_Update_flex(1, &curr_h, &curr_v);

            bMouse=FALSE;
			Info = 1;
			background_dlg.flags = 0xC0;

			if (Ret_Val == Dlg_Ret_Val_UP_END)
			{
				if (n_list_begin_bkg > 0)
				{
					n_list_begin_bkg = 0;
					background_dlg.flags = 0;

                    reload = FALSE;
                    break;
					//continue;
				}
				else 
				{
					reload = FALSE;
					break;
				}
				
			}

			if (Ret_Val == Dlg_Ret_Val_UP)
			{

				if (n_list_begin_bkg > 0)  n_list_begin_bkg -= n_list_max; // d_n_list;

				if (n_list_begin_bkg < 0)
					n_list_begin_bkg = 0;
				background_dlg.flags = 0;


                reload = FALSE;
                break;
				//continue;
			}

			else if (Ret_Val == Dlg_Ret_Val_DN)
			{

				if (n_list_till < n_list_end) n_list_begin_bkg = n_list_till;
				background_dlg.flags = 0;

                reload = FALSE;
                break;
				//continue;
			}
			
			else if (Ret_Val == Dlg_Ret_Val_Bigger)
			{

				Set_Enable_Button(&background_dlg, ID_SMALLER, 1);
				//need to make immage button bigger
				ButSizeBkg += 1;
				if (ButSizeBkg == ButSizeMax) Set_Enable_Button(&background_dlg, ID_BIGGER, 0);
				n_list_begin_bkg = 0;
				background_dlg.flags = 0;
				reload = FALSE;
				break;
			}
			else if (Ret_Val == Dlg_Ret_Val_Smaller)
			{

				Set_Enable_Button(&background_dlg, ID_BIGGER, 1);
				//need to make immage button smaller
				ButSizeBkg -= 1;
				if (ButSizeBkg == 0) Set_Enable_Button(&background_dlg, ID_SMALLER, 0);
				n_list_begin_bkg = 0;
				background_dlg.flags = 0;
				reload = FALSE;
				break;
			}
			else
			{
				
				if (Ret_Val == Dlg_Ret_Val_Cancel)
				{
					layer_info();
					view_scale();

					return 0;
				}
				else
				{
					background_file_name_ptr = background_file_name[Ret_Val - Dlg_Ret_Val_IMPORT_BLOCK] + 1;
					dirptr = strstr(background_file_name_ptr, "<DIR>");
					if (dirptr != NULL)
					{
						*dirptr = '\0';

						//changing folder
						dirptr = strstr(background_file_name_ptr, "..");
						if (dirptr != NULL)
						{
						    *dirptr = '\0';
							if (strcmp(pathname, pathname0) != 0)
							{
								dirptr= strrchr(background_file_name_ptr, Slash);
								*dirptr = '\0';
								strcpy(pathname, background_file_name_ptr);
							}
							if (strcmp(pathname, pathname0) != 0)
							{
								dirptr = strrchr(background_file_name_ptr, Slash);
								*dirptr = '\0';
								strcpy(pathname, background_file_name_ptr);
							}
						}
						else
						{
							dirptr = strstr(background_file_name_ptr, ".");
							if (dirptr != NULL) strcpy(pathname, pathname0);
							else strcpy(pathname, background_file_name_ptr);
						}

						reload = TRUE;
						X_bak = X;
						Y_bak = Y;

						///////
						n_list_begin_bkg = 0;
						background_dlg.flags = 0;
						///////

						break;
					}
					else
					{
						if (opcja == 0)
						{
							SetBackground(background_file_name_ptr);
						}
						else if (opcja == 1)
						{
							flags = fnsplit(background_file_name_ptr, drive, dir, file, ext);
							ret_p = SetPattern(background_file_name_ptr, file, ext);
							strcpy(SolidPatternName, file);
							SolidPattern = 1;
							//mSolidPattern.max = 7;
						}
						else if (opcja == 2)
						{
							flags = fnsplit(background_file_name_ptr, drive, dir, file, ext);
							ret_p = SetPattern(background_file_name_ptr, file, ext);
							strncpy(SolidHatchPatternName, file, 31);
							SolidHatchPatternName[31] = '\0';
							SolidHatchPattern = 1;
							SolidHatch = 0;
						}
						else if (opcja == 3)
						{
							flags = fnsplit(background_file_name_ptr, drive, dir, file, ext);
							ret_p = SetPattern(background_file_name_ptr, file, ext);
							strcpy(TracePatternName, file);
							TracePattern = 1;
							//mTracePattern.max = 7;
						}
						layer_info();
						view_scale();

						return 1;
					}
				}


				param_line_out();

				bMouse = TRUE;
                background_dlg.flags=0x40;
			}
		}
	}
	return 0;
}


int compare_names(const void *a, const void *b) {
    const char *ia = (const char *)((POLE*)a)->txt;
    const char *ib = (const char *)((POLE*)b)->txt;
    return strcmp(ia, ib);
}

int compare_time(const void *a, const void *b) {
    int ia = ((POLE*)a)->iconno;
    int ib = ((POLE*)b)->iconno;
    return ia - ib;
}

void qseparate(POLE * BlockList, POLE *mBlockList, int mBlockList_max, int *n_list_blk)
{  int i, n_list=0;

    memcpy(&BlockList[0], &mBlockList[0], sizeof(POLE));
    BlockList[0].wcod=1;
    n_list++;

    for (i=1; i<mBlockList_max; i++)
    {
        if (strcmp(mBlockList[i-1].txt, mBlockList[i].txt)==0)
            BlockList[n_list-1].wcod=BUTTON_GROUP; //multiple
        else
        {
            memcpy(&BlockList[n_list], &mBlockList[i], sizeof(POLE));
            BlockList[n_list].wcod=BUTTON_NORMAL;
            n_list++;
        }
    }
   *n_list_blk=n_list;
}

void qselect(POLE * BlockList, POLE *mBlockList, int mBlockList_max, int *n_list_blk, char *group_txt)
{  int i, n_list=0;

    strcpy(BlockList[0].txt,FOLDER);
    BlockList[0].wcod=BUTTON_FOLDER;
    n_list++;

    for (i=0; i<mBlockList_max; i++)
    {
        if (strcmp(mBlockList[i].txt, group_txt)==0)
        {
            memcpy(&BlockList[n_list], &mBlockList[i], sizeof(POLE));
            BlockList[n_list].wcod=BUTTON_NORMAL;
            n_list++;
        }
    }
    *n_list_blk=n_list;
}

void reset_n_list_begin_blk(void)
{
    n_list_begin_blk = 0;
}

int getBlockFromDialog(TMENU *mBlockList0, long *my_ptr__off_block)
{
    int n_list_blk;
    int n_list_till_blk = 0;
    int n_list_end_blk = 1000;
    double wsp_x, wsp_y;
    int dx_win_cat, dy_win_cat, x_win_cat, y_win_cat, x_win, y_win, x1_win, y1_win;
    static int curr_h, curr_v;
    int blocklist_dlg_dx;
    int blocklist_dlg_dy;
    int i, j;
    long pXp1, pYp1, pXk1, pYk1;
    double XXp, YYp, XXk, YYk;
    long pXp2, pYp2, pXk2, pYk2;
    double dx_but, dy_but;
    int nx_but, ny_but;
    int d_n_list, n_list_max;
    BOOL go_break;
    int max_y;
    int kolory_paper;
    int Ret_Val;
    char title_suffix[32];
    int title_len;
    char *title;
    char group_txt[130];
    int n_list_begin_blk_bak=0;

	unsigned char Layers_on[MAX_NUMBER_OF_LAYERS];
	unsigned char view_only_current_layer_bak;

    BOOL re_group=FALSE;
    re_sort=TRUE;

    POLE *mBlockList=(POLE*)malloc(mBlockList0->max*sizeof(POLE));
    memmove(mBlockList, mBlockList0->pola, mBlockList0->max*sizeof(POLE));

    POLE *BlockList=(POLE*)malloc(mBlockList0->max*sizeof(POLE));

    n_list_blk=mBlockList0->max;

    n_list_till_blk = 0;

    while (1)
    {
        //sorting
        if (re_sort) {
            if (sorting_type==0)
            {
                qsort(mBlockList, mBlockList0->max, sizeof(POLE), compare_names);
                qseparate(BlockList,mBlockList,mBlockList0->max,&n_list_blk);
            }
            else
            {
                qsort(mBlockList, mBlockList0->max, sizeof(POLE), compare_time);
                memmove(BlockList, mBlockList, mBlockList0->max*sizeof(POLE));
                n_list_blk=mBlockList0->max;
            }
            re_sort=FALSE;
        }
        else if (re_group)
        {
            qselect(BlockList,mBlockList,mBlockList0->max,&n_list_blk,group_txt);
            re_group=FALSE;
        }

        n_list_end_blk = n_list_blk;
        //building dialog window for blocks list

        Check_Radio_Button (&blocklist_dlg, ID_ALPHABET, ID_TIME, sorting_type == 1 ? ID_TIME: ID_ALPHABET, FALSE) ;

        wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
        wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

        dx_win_cat = getmaxx() - 8; // 10;
        dy_win_cat = getmaxy() - 8; // 10;

        blocklist_dlg.dx = ((float)getmaxx() - 20.0) / wsp_x;
        blocklist_dlg.dy = ((float)getmaxy() - 42.0 *((float)HEIGHT / 12.0) - 20) / wsp_y;

        blocklist_dlg_dx = blocklist_dlg.dx;
        blocklist_dlg_dy = blocklist_dlg.dy;

        blocklist_dlg.x = 10 / wsp_x;
        blocklist_dlg.y = 10 / wsp_y;

        Get_Default_Color_Dlg(&color_dlg);

        x_win_cat = 8; // 10;
        y_win_cat = 8; // 10;

        for (i = 0; i < FIXED_BUT_BLK; i++)
            buttons_blk[i].x = blocklist_dlg.dx - DXButCat - dx_BUT;

        buttons_blk[FIXED_BUT_BLK-3].x = blocklist_dlg.dx - DXButCat - dx_BUT + 4;
        buttons_blk[FIXED_BUT_BLK-2].x = blocklist_dlg.dx - DXButCat/2 - dx_BUT + 5;

        buttons_blk[0].y = dy_BUT;
        buttons_blk[1].y = 2 * dy_BUT + DYButCat;
        buttons_blk[2].y = 3 * dy_BUT + 2 * DYButCat;
        buttons_blk[3].y = 4 * dy_BUT + 3 * DYButCat;
        buttons_blk[4].y = 5 * dy_BUT + 4 * DYButCat;
        buttons_blk[5].y = 6 * dy_BUT + 5 * DYButCat;

        buttons_blk[6].y = 7 * dy_BUT + 6 * DYButCat + 14;
        buttons_blk[7].y = 7 * dy_BUT + 6 * DYButCat  + 14;

        buttons_blk[8].y = blocklist_dlg.dy - DYButCat - dy_BUT;

        images_blk[0].y1 = blocklist_dlg.dy - DYButCat*2 - dy_BUT;
        images_blk[0].x1 = blocklist_dlg.dx - 32 - dx_BUT;

        images_blk[1].y1 = 7 * dy_BUT + 5 * DYButCat  + 27;
        images_blk[1].x1 = blocklist_dlg.dx - DXButCat - dx_BUT;

        images_blk[2].y1 = 7 * dy_BUT + 5 * DYButCat  + 27;
        images_blk[2].x1 = blocklist_dlg.dx - DXButCat/2 - dx_BUT + 2;

        ///////////////////
        Save_Update_flex(0, &curr_h, &curr_v);

        pXp1 = get_pXp(); pYp1 = get_pYp(); pXk1 = get_pXk(); pYk1 = get_pYk();
        XXp = Xp;  YYp = Yp;  XXk = Xk;  YYk = Yk;

        Xp = 0; Yp = 0; Xk = 3000; Yk = 3000;  //to show objects behing drawinbg sheet in Catalog

        pXp2 = pXp1;
        pYp2 = getmaxy();
        pXk2 = getmaxx();
        pYk2 = 0;

        set_World(pXp2, pYp2, pXk2, pYk2);
        ///////////////////
        //calculating number of buttons each direction
        blocklist_dlg.dx = blocklist_dlg_dx;
        blocklist_dlg.dy = blocklist_dlg_dy;

        dx_but = DX_BUT[ButSizeBlk] * wsp_x;
        dy_but = DY_BUT[ButSizeBlk] * wsp_y;

        nx_but = (blocklist_dlg.dx - 20 - DXButCat) / (DX_BUT[ButSizeBlk] + 5);
        ny_but = (blocklist_dlg.dy - 10) / (DY_BUT[ButSizeBlk] + 5);

        if ((nx_but * ny_but) > BlockFileNameMax)
        {
            ny_but = (int)(BlockFileNameMax / nx_but);
        };

        blocklist_dlg.SizeButtonT = FIXED_BUT_BLK + (nx_but*ny_but);
        /////////////////////

        //we have n_list number of files
        n_list_max = nx_but * ny_but;
        if (n_list_end_blk < n_list_begin_blk) n_list_begin_blk=max(n_list_end_blk-n_list_max, 0);
        d_n_list = n_list_end_blk - n_list_begin_blk;
        if (d_n_list > n_list_max) d_n_list = n_list_max;

        if (d_n_list > n_list_blk) d_n_list = n_list_blk;

        blocklist_dlg.SizeButtonT = FIXED_BUT_BLK + d_n_list; // (nx_but*ny_but);


        go_break = FALSE;
        for (j = 0; j < ny_but; j++)
        {
            for (i = 0; i < nx_but; i++)
            {
                memmove(&buttons_blk[j*nx_but + i + FIXED_BUT_BLK], &buttons_def, sizeof(BUTTON));
                buttons_blk[j*nx_but + i + FIXED_BUT_BLK].dx = DX_BUT[ButSizeBlk];
                buttons_blk[j*nx_but + i + FIXED_BUT_BLK].dy = DY_BUT[ButSizeBlk];

                buttons_blk[j*nx_but + i + FIXED_BUT_BLK].x = i * (DX_BUT[ButSizeBlk] + 5) + 5;
                buttons_blk[j*nx_but + i + FIXED_BUT_BLK].y = j * (DY_BUT[ButSizeBlk] + 5) + 5;
                buttons_blk[j*nx_but + i + FIXED_BUT_BLK].id = (j*nx_but + i + ID_INSERT);
                buttons_blk[j*nx_but + i + FIXED_BUT_BLK].txt = BlockList[n_list_begin_blk + j*nx_but + i].txt;
                buttons_blk[j*nx_but + i + FIXED_BUT_BLK].adr = (long)(BlockList[n_list_begin_blk + j*nx_but + i].menu);
                buttons_blk[j*nx_but + i + FIXED_BUT_BLK].flags = (unsigned int)(BlockList[n_list_begin_blk + j*nx_but + i].wcod);
                buttons_blk[j*nx_but + i + FIXED_BUT_BLK].reference=(BlockList[n_list_begin_blk + j*nx_but + i].iconno);
                if ((j*nx_but + i) > d_n_list)
                {
                    go_break = TRUE;
                    break;
                }
            }
            if (go_break) break;
        }

        //eventually there are only j rows
        //size of dialog correction

        max_y = (j + 1) * (DY_BUT[ButSizeBlk] + 5) + 10;

        if (max_y < (FIXED_BUT_BLK * (DYButCat + dy_BUT))) max_y = (FIXED_BUT_BLK * (DYButCat + dy_BUT));

        if (max_y < blocklist_dlg.dy) blocklist_dlg.dy = max_y;

        buttons_blk[8].y = blocklist_dlg.dy - DYButCat - dy_BUT;
        images_blk[0].y1 = blocklist_dlg.dy - DYButCat * 2 - dy_BUT;

        n_list_till_blk = (n_list_begin_blk + d_n_list);

        Info = 0;
        kolory_paper = kolory.paper;
        kolory.paper = 7;

        backup_type_width_kolor();
        backup_hatch_pattern();

        blocklist_dlg.txt=BLOCKLIST;

        sprintf(title_suffix,"%d...%d (%d)", n_list_begin_blk+1, n_list_till_blk, n_list_blk);
        title_len=strlen(blocklist_dlg.txt)+32;
        title=(char*)malloc(title_len);
        sprintf(title,"%s  %s", blocklist_dlg.txt, title_suffix);
        blocklist_dlg.txt=title;

		for (i = 0; i < MAX_NUMBER_OF_LAYERS; i++)
		{
			Layers_on[i] = Layers[i].on;
			Layers[i].on = 1;
		}
		view_only_current_layer_bak = options1.view_only_current_layer;
		options1.view_only_current_layer = 0;

        if (n_list_begin_blk == 0) buttons_blk[0].enable=buttons_blk[1].enable=0;
        else buttons_blk[0].enable=buttons_blk[1].enable=1;

        if (n_list_till_blk == n_list_end_blk) buttons_blk[2].enable=buttons_blk[3].enable=0;
        else buttons_blk[2].enable=buttons_blk[3].enable=1;

        Ret_Val = Dialog(&blocklist_dlg, &color_dlg, proc_dlg_blocklist, bMouse);

		options1.view_only_current_layer = view_only_current_layer_bak;
		for (i = 0; i < MAX_NUMBER_OF_LAYERS; i++)
		{
			Layers[i].on = Layers_on[i];
		}

        blocklist_dlg.txt=BLOCKLIST;
        free(title);

        kolory.paper = kolory_paper;

        set_World(pXp1, pYp1, pXk1, pYk1);
        Xp = XXp;  Yp = YYp;  Xk = XXk;  Yk = YYk;
        Save_Update_flex(1, &curr_h, &curr_v);

        bMouse=FALSE;
        blocklist_dlg.flags=0xC0;
        restore_type_width_kolor();
        restore_hatch_pattern();

        Info = 1;

        if (Ret_Val == Dlg_Ret_Val_UP_END)
        {
            if (n_list_begin_blk > 0)
            {
                n_list_begin_blk = 0;
                blocklist_dlg.flags = 0;

                continue;
            }
            else
            {
                continue;
            }
        }

        else if (Ret_Val == Dlg_Ret_Val_UP)
        {
            if (n_list_begin_blk > 0)  n_list_begin_blk -= n_list_max; // d_n_list;

            if (n_list_begin_blk < 0)
                n_list_begin_blk = 0;
            blocklist_dlg.flags = 0;
            continue;
        }

        else if (Ret_Val == Dlg_Ret_Val_DN)
        {
            if (n_list_till_blk < n_list_end_blk) n_list_begin_blk = n_list_till_blk;
            blocklist_dlg.flags = 0;
            continue;
        }
        else if (Ret_Val == Dlg_Ret_Val_DN_END)
        {
            if (n_list_till_blk < n_list_blk)
            {
                n_list_begin_blk = n_list_blk - d_n_list;
                blocklist_dlg.flags = 0;

                continue;
            }
            else
            {
                continue;
            }
        }
        else if (Ret_Val == Dlg_Ret_Val_Bigger)
        {

            Set_Enable_Button(&blocklist_dlg, ID_SMALLER, 1);
            //need to make immage button bigger
            ButSizeBlk += 1;
            if (ButSizeBlk == ButSizeMax) Set_Enable_Button(&blocklist_dlg, ID_BIGGER, 0);
            n_list_begin_blk = 0;
            blocklist_dlg.flags = 0;
            continue;
        }
        else if (Ret_Val == Dlg_Ret_Val_Smaller)
        {
            Set_Enable_Button(&blocklist_dlg, ID_BIGGER, 1);
            //need to make immage button smaller
            ButSizeBlk -= 1;
            if (ButSizeBlk == 0) Set_Enable_Button(&blocklist_dlg, ID_SMALLER, 0);
            n_list_begin_blk = 0;
            blocklist_dlg.flags = 0;
            continue;
        }
        else if (Ret_Val == Dlg_Ret_Val_Alphabet)
        {
            re_sort=TRUE;
            sorting_type=0;
            n_list_begin_blk=0;
            continue;
        }
        else if (Ret_Val == Dlg_Ret_Val_Time)
        {
            re_sort=TRUE;
            sorting_type=1;
            n_list_begin_blk=0;
            continue;
        }
        else
        {

            if (Ret_Val == Dlg_Ret_Val_Cancel)
            {
                layer_info();
                view_scale();

                free(BlockList);
                free(mBlockList);

                if (get_block_changed()) redraw();

                return -1;
            }
            else
            {
                int btn_no=Ret_Val - Dlg_Ret_Val_INSERT_BLOCK + FIXED_BUT_BLK;
                int blk_no=n_list_begin_blk + Ret_Val - Dlg_Ret_Val_INSERT_BLOCK;

                if (buttons_blk[btn_no].flags==2)  //group
                {
                    n_list_begin_blk_bak = n_list_begin_blk;
                    n_list_begin_blk = 0;
                    re_group=TRUE;
                    strcpy(group_txt, buttons_blk[btn_no].txt);
                    continue;
                }
                else if (buttons_blk[btn_no].flags==4)  //folder
                {
                    n_list_begin_blk = n_list_begin_blk_bak;
                    re_sort=TRUE;
                    continue;
                }

                layer_info();
                view_scale();

                free(BlockList);
                free(mBlockList);

                re_sort=TRUE;

                if (get_block_changed()) redraw();
                return buttons_blk[btn_no].reference;
            }

            param_line_out();

            bMouse = TRUE;
            blocklist_dlg.flags=0x40;
        }
    }

    free(BlockList);
    free(mBlockList);

    return 0;
}


int FileNameOC(char *fn, int Max_Len)
{
	int Ret_Val;
	BOOL ret;
	char **file_list = NULL;
	int n_list;
	char block_file_name[BlockFileNameMax+1][MAXPATH];
	int i, j;

	int dx_win_cat, dy_win_cat, x_win_cat, y_win_cat, x_win, y_win, x1_win, y1_win;
	double wsp_xxx, wsp_yyy, wsp_x, wsp_y;
	double dx_but, dy_but, ddx_but, ddy_but;
	int nx_but, ny_but, dx, dy;
	int d_n_list, n_list_max;
	BOOL go_break;
	long pXp1, pYp1, pXk1, pYk1;
	long pXp2, pYp2, pXk2, pYk2;
	double XXp, YYp, XXk, YYk;
	char *block_file_name_ptr;
	BOOL reload = TRUE;
	int max_y;
	int catalog_dlg_dx;
	int catalog_dlg_dy;

	char pathname[MAXPATH];
	char drive[MAXDRIVE];
	char dir[MAXDIR];
	char file[MAXFILE];
	char ext[MAXEXT];
	int flags;
	int kolory_paper;
	char* dirptr;
	char pathname0[MAXPATH]="Catalog";
	double X_bak, Y_bak;
	int ret_p;
	BOOL folder_choice = FALSE;
    static int curr_h, curr_v;

    char title_suffix[32];
    int title_len;
    char *title;


    bMouse = TRUE;
    catalog_dlg.flags=0x40;

	get_lines_width_dxf_glb();

	while (1)
	{

		if (reload)
        {
            Max_Len = Max_Len;
            param_line_out();

            while (1) {
                if ((folder_choice == TRUE) || (strlen(fn) == 0)) {
                    ret = Load_File(fn, OPEN_CATALOG, FALSE);

                    if (strlen(fn) == 0) {
                        sw_callback_locked_proc();
                        return 0;
                    }

                    if (!ret) {
                        sw_callback_locked_proc();
                        return 0;
                    }

                }
#ifndef LINUX
                ret = My_GetFiles(fn, &n_list, "\\*.", "\\*.a?x", "\\*.dxf",NULL, TRUE);
#else
                ret = My_GetFiles(fn, &n_list, "\\*.", "\\*.alx", "\\*.axx", "\\*.dxf", TRUE);
#endif

                if (ret) {

                    break;
                } else {
                    folder_choice = TRUE;
                }
            }
        }

        param_line_out();

        n_list_begin = 0;
        n_list_till = 0;

        n_list_end = n_list;
			//building dialog window for catalog
        while (1)
        {
			wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
			wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

			dx_win_cat = getmaxx() - 8; // 10;
			dy_win_cat = getmaxy() - 8; // 10;

			catalog_dlg.dx = ((float)getmaxx() - 20.0) / wsp_x;
			catalog_dlg.dy = ((float)getmaxy() - 42.0 *((float)HEIGHT / 12.0) - 20) / wsp_y;

			catalog_dlg_dx = catalog_dlg.dx;
			catalog_dlg_dy = catalog_dlg.dy;

			catalog_dlg.x = 10 / wsp_x;
			catalog_dlg.y = 10 / wsp_y;

			Get_Default_Color_Dlg(&color_dlg);

			x_win_cat = 8; // 10;
			y_win_cat = 8; // 10;

			for (i = 0; i < 7; i++)
				buttons_cat[i].x = catalog_dlg.dx - DXButCat - dx_BUT;

			images_cat[0].x1 = catalog_dlg.dx - 32 - dx_BUT;

			buttons_cat[0].y = dy_BUT;
			buttons_cat[1].y = 2 * dy_BUT + DYButCat;
			buttons_cat[2].y = 3 * dy_BUT + 2 * DYButCat;
			buttons_cat[3].y = 4 * dy_BUT + 3 * DYButCat;
			buttons_cat[4].y = 5 * dy_BUT + 4 * DYButCat;
			buttons_cat[5].y = 6 * dy_BUT + 5 * DYButCat;

			buttons_cat[6].y = catalog_dlg.dy - DYButCat - dy_BUT;

			images_cat[0].y1 = catalog_dlg.dy - DYButCat*2 - dy_BUT;

            ///////////////////
            Save_Update_flex(0, &curr_h, &curr_v);

            pXp1 = get_pXp(); pYp1 = get_pYp(); pXk1 = get_pXk(); pYk1 = get_pYk();
            XXp = Xp;  YYp = Yp;  XXk = Xk;  YYk = Yk;

            Xp = 0; Yp = 0; Xk = 3000; Yk = 3000;  //to show objects behing drawinbg sheet in Catalog

            pXp2 = pXp1;
            pYp2 = getmaxy();
            pXk2 = getmaxx();
            pYk2 = 0; // pYk1;

            set_World(pXp2, pYp2, pXk2, pYk2);
			///////////////////
			//calculating number of buttons each direction
			catalog_dlg.dx = catalog_dlg_dx;
			catalog_dlg.dy = catalog_dlg_dy;

			dx_but = DX_BUT[ButSizeCat] * wsp_x;
			dy_but = DY_BUT[ButSizeCat] * wsp_y;

			nx_but = (catalog_dlg.dx - 20 - DXButCat) / (DX_BUT[ButSizeCat] + 5);
			ny_but = (catalog_dlg.dy - 10) / (DY_BUT[ButSizeCat] + 5);

			if ((nx_but * ny_but) > BlockFileNameMax)
			{
				ny_but = (int)(BlockFileNameMax / nx_but);
			};

			catalog_dlg.SizeButtonT = FIXED_BUT + (nx_but*ny_but);
			/////////////////////

			//we have n_list number of files
			n_list_max = nx_but * ny_but;
            if (n_list_end < n_list_begin) n_list_begin=max(n_list_end-n_list_max, 0);
			d_n_list = n_list_end - n_list_begin;
			if (d_n_list > n_list_max) d_n_list = n_list_max;

			if (d_n_list > n_list) d_n_list = n_list;

			catalog_dlg.SizeButtonT = FIXED_BUT + d_n_list; // (nx_but*ny_but);


			for (i = 0; i < n_list_max; i++)
			{
				if (i < d_n_list)
				{
					sprintf(block_file_name[i], "\026%s%c%s", fn /*pathname*/, Slash, strarray[i + n_list_begin]);
				}
				else
					strcpy(block_file_name[i], "");

			}

			go_break = FALSE;
			for (j = 0; j < ny_but; j++)
			{
				for (i = 0; i < nx_but; i++)
				{
					memmove(&buttons_cat[j*nx_but + i + FIXED_BUT], &buttons_def, sizeof(BUTTON));
					buttons_cat[j*nx_but + i + FIXED_BUT].dx = DX_BUT[ButSizeCat];
					buttons_cat[j*nx_but + i + FIXED_BUT].dy = DY_BUT[ButSizeCat];

					buttons_cat[j*nx_but + i + FIXED_BUT].x = i * (DX_BUT[ButSizeCat] + 5) + 5;
					buttons_cat[j*nx_but + i + FIXED_BUT].y = j * (DY_BUT[ButSizeCat] + 5) + 5;
					buttons_cat[j*nx_but + i + FIXED_BUT].id = (j*nx_but + i + ID_IMPORT);
					buttons_cat[j*nx_but + i + FIXED_BUT].txt = block_file_name[j*nx_but + i];
					if ((j*nx_but + i) > d_n_list)
					{
						go_break = TRUE;
						reload = TRUE;
						break;
					}
				}
				if (go_break) break;
			}

			//eventually there are only j rows
			//size of dialog correction 

			max_y = (j + 1) * (DY_BUT[ButSizeCat] + 5) + 10;

			if (max_y < (8 * (DYButCat + dy_BUT))) max_y = (8 * (DYButCat + dy_BUT));

			if (max_y < catalog_dlg.dy) catalog_dlg.dy = max_y;

			buttons_cat[6].y = catalog_dlg.dy - DYButCat - dy_BUT;
			images_cat[0].y1 = catalog_dlg.dy - DYButCat * 2 - dy_BUT;

			n_list_till = (n_list_begin + d_n_list);
			
			Info = 0;
			kolory_paper = kolory.paper;
			kolory.paper = 7;

            backup_type_width_kolor();
            backup_hatch_pattern();

            catalog_dlg.txt=fn;

            sprintf(title_suffix,"%d...%d (%d)", n_list_begin+1, n_list_till, n_list);
            title_len=strlen(catalog_dlg.txt)+32;
            title=(char*)malloc(title_len);
            sprintf(title,"%s  %s", catalog_dlg.txt, title_suffix);
            catalog_dlg.txt=title;

            if (n_list_begin == 0) buttons_blk[0].enable=0;
            else buttons_blk[0].enable=1;

            if (n_list_till == n_list_end) buttons_blk[1].enable=0;
            else buttons_blk[1].enable=1;

			Ret_Val = Dialog(&catalog_dlg, &color_dlg, proc_dlg_catalog, bMouse);

            catalog_dlg.txt=fn;
            free(title);

            kolory.paper = kolory_paper;

            set_World(pXp1, pYp1, pXk1, pYk1);
            Xp = XXp;  Yp = YYp;  Xk = XXk;  Yk = YYk;
            Save_Update_flex(1, &curr_h, &curr_v);

            bMouse=FALSE;
            catalog_dlg.flags=0xC0;
            restore_type_width_kolor();
            restore_hatch_pattern();

			Info = 1;

			if (Ret_Val == Dlg_Ret_Val_UP)
			{
				if (n_list_begin > 0)  n_list_begin -= n_list_max; // d_n_list;

				if (n_list_begin < 0)
					n_list_begin = 0;
				catalog_dlg.flags = 0;
				continue;
			}

			else if (Ret_Val == Dlg_Ret_Val_DN)
			{
				if (n_list_till < n_list_end) n_list_begin = n_list_till;
				catalog_dlg.flags = 0;
				continue;
			}
			else if (Ret_Val == Dlg_Ret_Val_Folder)
			{
				catalog_dlg.flags = 0;

				strcpy(fn, "");
				reload = TRUE;
				folder_choice = TRUE;
				break;
			}
			else if (Ret_Val == Dlg_Ret_Val_Folder0)
			{
				
				catalog_dlg.flags = 0;


				strcpy(fn, ".");
				reload = TRUE;
				folder_choice = TRUE;
				break;
			}
			else if (Ret_Val == Dlg_Ret_Val_Bigger)
			{

				Set_Enable_Button(&catalog_dlg, ID_SMALLER, 1);
				//need to make immage button bigger
				ButSizeCat += 1;
				if (ButSizeCat == ButSizeMax) Set_Enable_Button(&catalog_dlg, ID_BIGGER, 0);
				n_list_begin = 0;
				catalog_dlg.flags = 0;
				reload = FALSE;
				break;
			}
			else if (Ret_Val == Dlg_Ret_Val_Smaller)
			{
				Set_Enable_Button(&catalog_dlg, ID_BIGGER, 1);
				//need to make immage button smaller
				ButSizeCat -= 1;
				if (ButSizeCat == 0) Set_Enable_Button(&catalog_dlg, ID_SMALLER, 0);
				n_list_begin = 0;
				catalog_dlg.flags = 0;
				reload = FALSE;
				
				break;
			}
			else
			{
				
				if (Ret_Val == Dlg_Ret_Val_Cancel)
				{
					layer_info();
					view_scale();

					return 0;
				}
				else
				{
					block_file_name_ptr = block_file_name[Ret_Val - Dlg_Ret_Val_IMPORT_BLOCK] + 1;
					dirptr = strstr(block_file_name_ptr, "<DIR>");
					if (dirptr != NULL)
					{
						*dirptr = '\0';

						//changing folder
						dirptr = strstr(block_file_name_ptr, "..");
						if (dirptr != NULL)
						{
							*dirptr = '\0';

								dirptr = strrchr(block_file_name_ptr, Slash);
								if (dirptr != NULL)
								{
									*dirptr = '\0';
								}
								
								dirptr = strrchr(block_file_name_ptr, Slash);
								if (dirptr != NULL)
								{
									*dirptr = '\0';
								}
								
								strcpy(fn /*pathname*/, block_file_name_ptr);

						}
						else
						{
							dirptr = strstr(block_file_name_ptr, ".");
							if (dirptr != NULL) 
								strcpy(fn /*pathname*/, "" /*pathname0*/);
							else strcpy(fn /*pathname*/, block_file_name_ptr);
						}

						X_bak = X;
						Y_bak = Y;
						folder_choice = FALSE;
						reload = TRUE;

						///////
						n_list_begin_bkg = 0;
						catalog_dlg.flags = 0;
						///////

						break;
					}
					else
					{

						flags = fnsplit(block_file_name_ptr, drive, dir, file, ext);
						if (strcmp(strlwr2(ext), ".dxf") == 0)
							WezSymbol_DXF(block_file_name_ptr);
						else
							WezSymbol(block_file_name_ptr);


					}
				}

			
				param_line_out();
				
				bMouse = TRUE;
                catalog_dlg.flags=0x40;
			}
		}
	}
	return 0;
}

int FileNamePattern(char **pattern_files, int n_list)
{
	int Ret_Val;
	BOOL ret;
	char **file_list = NULL;
	char pattern_file_name[BlockFileNameMax + 1][MAXPATH]; // [64] ;  //48
	int i, j;

	int dx_win_cat, dy_win_cat, x_win_cat, y_win_cat, x_win, y_win, x1_win, y1_win;
	double wsp_xxx, wsp_yyy, wsp_x, wsp_y;
	double dx_but, dy_but, ddx_but, ddy_but;
	int nx_but, ny_but, dx, dy;
	int d_n_list, n_list_max;
	BOOL go_break;
	long pXp1, pYp1, pXk1, pYk1;
	long pXp2, pYp2, pXk2, pYk2;
	double XXp, YYp, XXk, YYk;
	char *block_file_name_ptr;
	BOOL reload = TRUE;
	int max_y;
	int patterns_dlg_dx;
	int patterns_dlg_dy;
    static int curr_h, curr_v;

    char title_suffix[32];
    int title_len;
    char *title;

	patterns_dlg.pozx = 10;
	patterns_dlg.pozy = 10;

    bMouse = TRUE;
    patterns_dlg.flags=0x40;
	
	while (1)
	{

		if (reload) {
            Cur_ond(X, Y);
            param_line_out();

        }
			//building dialog window for patterns

			wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
			wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

			dx_win_cat = getmaxx() - 8; // 10;
			dy_win_cat = getmaxy() - 8; // 10;

			patterns_dlg.dx = ((float)getmaxx() - 20.0) / wsp_x;
			patterns_dlg.dy = ((float)getmaxy() - 42.0 *((float)HEIGHT / 12.0) - 20) / wsp_y;

			patterns_dlg_dx = patterns_dlg.dx;
			patterns_dlg_dy = patterns_dlg.dy;

			patterns_dlg.x = 10 / wsp_x;
			patterns_dlg.y = 10 / wsp_y;

			Get_Default_Color_Dlg(&color_dlg);

			x_win_cat = 8;
			y_win_cat = 8;

			for (i = 0; i < 5; i++)
				buttons_pattern[i].x = patterns_dlg.dx - DXButCat - dx_BUT;

			buttons_pattern[0].y = dy_BUT;
			buttons_pattern[1].y = 2 * dy_BUT + DYButCat;
			buttons_pattern[2].y = 3 * dy_BUT + 2 * DYButCat;
			buttons_pattern[3].y = 4 * dy_BUT + 3 * DYButCat;
			buttons_pattern[4].y = patterns_dlg.dy - DYButCat - dy_BUT;
			images_pattern[0].y1 = patterns_dlg.dy - DYButCat * 2 - dy_BUT;
			images_pattern[0].x1 = patterns_dlg.dx - 32 - dx_BUT;

			Cur_offd(X, Y);

			param_line_out();




		n_list_till = 0;

		n_list_end = n_list;


		while (1)
		{
            ///////////////////
            Save_Update_flex(0, &curr_h, &curr_v);

            pXp1 = get_pXp(); pYp1 = get_pYp(); pXk1 = get_pXk(); pYk1 = get_pYk();
            XXp = Xp;  YYp = Yp;  XXk = Xk;  YYk = Yk;

            Xp = 0; Yp = 0; Xk = 3000; Yk = 3000;  //to show objects behing drawinbg sheet in Catalog

            pXp2 = pXp1;
            pYp2 = getmaxy();
            pXk2 = getmaxx();
            pYk2 = 0;

            set_World(pXp2, pYp2, pXk2, pYk2);
			///////////////////
			//calculating number of buttons each direction
			patterns_dlg.dx = patterns_dlg_dx;
			patterns_dlg.dy = patterns_dlg_dy;

			dx_but = DX_BUT[ButSizePat] * wsp_x;
			dy_but = DY_BUT[ButSizePat] * wsp_y;

			nx_but = (patterns_dlg.dx - 20 - DXButCat) / (DX_BUT[ButSizePat] + 5);
			ny_but = (patterns_dlg.dy - 10) / (DY_BUT[ButSizePat] + 5);

			if ((nx_but * ny_but) > BlockFileNameMax)
			{
				ny_but = (int)(BlockFileNameMax / nx_but);
			};

			patterns_dlg.SizeButtonT = FIXED_BUT_PATTERN + (nx_but*ny_but);
			/////////////////////

			//we have n_list number of files
			n_list_max = nx_but * ny_but;
            if (n_list_end < n_list_begin_patterns) n_list_begin_blk=max(n_list_end-n_list_max, 0);
			d_n_list = n_list_end - n_list_begin_patterns;
			if (d_n_list > n_list_max) d_n_list = n_list_max;

			if (d_n_list > n_list) d_n_list = n_list;

			patterns_dlg.SizeButtonT = FIXED_BUT_PATTERN + d_n_list; // (nx_but*ny_but);


			for (i = 0; i < n_list_max; i++)
			{
				if (i < d_n_list)
				{
					sprintf(pattern_file_name[i], "\024%s%c%s", __PATTERNS__, Slash, pattern_files[i + n_list_begin_patterns]);
				}
				else
					strcpy(pattern_file_name[i], "");

			}

			go_break = FALSE;
			for (j = 0; j < ny_but; j++)
			{
				for (i = 0; i < nx_but; i++)
				{
					memmove(&buttons_pattern[j*nx_but + i + FIXED_BUT_PATTERN], &buttons_def, sizeof(BUTTON));
					buttons_pattern[j*nx_but + i + FIXED_BUT_PATTERN].dx = DX_BUT[ButSizePat];
					buttons_pattern[j*nx_but + i + FIXED_BUT_PATTERN].dy = DY_BUT[ButSizePat];

					buttons_pattern[j*nx_but + i + FIXED_BUT_PATTERN].x = i * (DX_BUT[ButSizePat] + 5) + 5;
					buttons_pattern[j*nx_but + i + FIXED_BUT_PATTERN].y = j * (DY_BUT[ButSizePat] + 5) + 5;
					buttons_pattern[j*nx_but + i + FIXED_BUT_PATTERN].id = (j*nx_but + i + ID_IMPORT);
					buttons_pattern[j*nx_but + i + FIXED_BUT_PATTERN].txt = pattern_file_name[j*nx_but + i];

                    buttons_pattern[j*nx_but + i + FIXED_BUT_PATTERN].paper=kolory.paper;

                    if ((j*nx_but + i) > d_n_list)
					{
						go_break = TRUE;
						reload = TRUE;
						break;
					}
				}
				if (go_break) break;
			}

			//eventually there are only j rows
			//size of dialog correction 

			max_y = (j + 1) * (DY_BUT[ButSizePat] + 5) + 10;

			if (max_y < (6 * (DYButCat + dy_BUT))) max_y = (6 * (DYButCat + dy_BUT));

			if (max_y < patterns_dlg.dy) patterns_dlg.dy = max_y;

			buttons_pattern[4].y = patterns_dlg.dy - DYButCat - dy_BUT;
			images_pattern[0].y1 = patterns_dlg.dy - DYButCat * 2 - dy_BUT;

			n_list_till = (n_list_begin_patterns + d_n_list);

            if (n_list_begin_patterns == 0) buttons_pattern[0].enable=0;
            else buttons_pattern[0].enable=1;

            if (n_list_till == n_list_end) buttons_pattern[1].enable=0;
            else buttons_pattern[1].enable=1;

            patterns_dlg.txt=patterns;

            sprintf(title_suffix,"%d...%d (%d)", n_list_begin_patterns+1, n_list_till, n_list);
            title_len=strlen(patterns_dlg.txt)+32;
            title=(char*)malloc(title_len);
            sprintf(title,"%s  %s", patterns_dlg.txt, title_suffix);
            patterns_dlg.txt=title;

			Info = 0;
			Ret_Val = Dialog(&patterns_dlg, &color_dlg, proc_dlg_patterns, bMouse);

            patterns_dlg.txt=patterns;
            free(title);

            set_World(pXp1, pYp1, pXk1, pYk1);
            Xp = XXp;  Yp = YYp;  Xk = XXk;  Yk = YYk;
            Save_Update_flex(1, &curr_h, &curr_v);

            bMouse = FALSE;
            patterns_dlg.flags=0xC0;
			Info = 1;

			patterns_dlg.pozx = X;
			patterns_dlg.pozy = Y;

			if (Ret_Val == Dlg_Ret_Val_UP)
			{
				if (n_list_begin_patterns > 0)  n_list_begin_patterns -= n_list_max; // d_n_list;

				if (n_list_begin_patterns < 0)
					n_list_begin_patterns = 0;
				patterns_dlg.flags = 0;
				continue;
			}

			else if (Ret_Val == Dlg_Ret_Val_DN)
			{
				if (n_list_till < n_list_end) n_list_begin_patterns = n_list_till;
				patterns_dlg.flags = 0;
				continue;
			}
			else if (Ret_Val == Dlg_Ret_Val_Bigger)
			{

				Set_Enable_Button(&patterns_dlg, ID_SMALLER, 1);
				//need to make immage button bigger
				ButSizePat += 1;
				if (ButSizePat == ButSizeMax) Set_Enable_Button(&patterns_dlg, ID_BIGGER, 0);
				n_list_begin_patterns = 0;
				patterns_dlg.flags = 0;
				reload = FALSE;
				break;
			}
			else if (Ret_Val == Dlg_Ret_Val_Smaller)
			{
				
				Set_Enable_Button(&patterns_dlg, ID_BIGGER, 1);
				//need to make immage button smaller
				ButSizePat -= 1;
				if (ButSizePat == 0) Set_Enable_Button(&patterns_dlg, ID_SMALLER, 0);
				n_list_begin_patterns = 0;
				patterns_dlg.flags = 0;
				reload = FALSE;

				break;
			}
			else
			{
				
				if (Ret_Val == Dlg_Ret_Val_Cancel)
				{
					layer_info();
					view_scale();

					return -1;
				}
				else
				{
					
					layer_info();
					view_scale();

					return (n_list_begin_patterns + Ret_Val - Dlg_Ret_Val_IMPORT_BLOCK);
				}


				Cur_offd(X, Y);

				param_line_out();

				bMouse = TRUE;
                patterns_dlg.flags=0x40;
			}
		}
	}
	return -1;
}

int FileNamePCX (char *fn,int Max_Len)
{
  BOOL ret;

  Max_Len = Max_Len ;
  ret = Load_File (fn, INI_PCX, FALSE);
  return ret;
}

int FileNamePCX2BMP (char *fn,int Max_Len)
{
    BOOL ret;

    Max_Len = Max_Len ;
    ret = Load_File (fn, INI_PCX2BMP, FALSE);

    return ret;
}

int FileNamePCX_Export (char *fn,int Max_Len)
{
  BOOL ret;

  Max_Len = Max_Len ;
  ret = Load_File (fn, SAVE_PCX, FALSE);

  return ret;
}

int FileNamePNG_Export(char *fn, int Max_Len)
{
	BOOL ret;

	Max_Len = Max_Len;
	ret = Load_File(fn, SAVE_PNG, FALSE);

	return ret;
}

int FileNameJPG_Export(char* fn, int Max_Len)
{
	BOOL ret;

	Max_Len = Max_Len;
	ret = Load_File(fn, SAVE_JPEG, FALSE);

	return ret;
}

int FileNamePRO (char *fn,int Max_Len)
{
  BOOL ret;

  Max_Len = Max_Len ;
  ret = Load_File (fn, INI_PRO, FALSE);
  return ret;
}


int FileNameGEO (char *fn,int Max_Len)
{
  BOOL ret;

  Cur_ond(X,Y);
  Max_Len = Max_Len ;
  ret = Load_File (fn, INI_GEO, FALSE);
  return ret;
}

static void redcrpkh(char typ, int comput_area)
{ static void (*CUR_oN)(double ,double)=nooop_;
  static void (*CUR_oFF)(double ,double)=nooop_;
  static TMENU *menu;
  static int ( *SW[4])();
  static char st[32];
  unsigned short cod = L' ';
  static int iconno=0;

 if ((typ==0) || (typ==10))
   {
     eL.x=maxX-PL266;
     eL.y=ESTR_Y;
     eL.lmax=r22;
     eL.val_no_max = 1 ;
     eL.mode = GV_ANGLE ;
     eL.format = "%#10.5lg" ;
     eL.ESTRF = e_imp_o;
	 eL.extend = 0;

     BlokM=1;
     CUR_oFF=CUR_OFF; 
	 CUR_oN = CUR_ON;
	 if (comput_area == 10)
	 {
		 sel.gor = 1;
		 CUR_OFF = cur_offpkisel;
		 CUR_ON = cur_onpkisel;
	 }
	 else
	 {
		 CUR_OFF = cur_offpki;
		 CUR_ON = cur_onpki;
	 }
  
     SW[0]=SERV[73];
     SW[1]=SERV[81];
     SW[2]=SERV[71];
     SW[3]=SERV[79];

     if (typ==10) komunikat0(Komunikat_R0=122/*37*/);
      else
       {
		 if (SolidHatchPattern == 1)
		 {
			 komunikat0(139);
			 SERV[73] = noooph;  //Home  kat +3
			 SERV[81] = noooph;  //End   kat -3
			 SERV[71] = hatch_pattern_rotate_t45;  //Home  kat +45
			 SERV[79] = hatch_pattern_rotate_t_45;  //End   kat -45
		 }
		 else if (SolidHatch == 1)
		 {
			 komunikat0(140);
			 SERV[73] = noooph;  //Home  kat +3
			 SERV[81] = noooph;  //End   kat -3
			 SERV[71] = noooph;  //Home  kat +45
			 SERV[79] = noooph;  //End   kat -45
		 }
		 else
		 {
			 SERV[73] = hatch_rotate_t3;  //PgUp  kat +3
			 SERV[81] = hatch_rotate_t_3;  //PgDn   kat -3
			 SERV[71] = hatch_rotate_t45;  //Home  kat +45
			 SERV[79] = hatch_rotate_t_45;  //End   kat -45
		 }
       }

     b_check_arc = check_if_obiekt (ADP, ADK, Ablok, Oluk);
  
	 Cur_ond(X,Y);
   }
  else if (typ == 11)
  {
    Cur_offd(X,Y);
    CUR_OFF=CUR_oFF;
    CUR_ON=CUR_oN;

    SERV[73]=SW[0];
    SERV[81]=SW[1];
    SERV[71]=SW[2];
    SERV[79]=SW[3];
    BlokM=0;
    komunikat0(Komunikat_R0=0);
	Cur_ond(X, Y);
  }
 else {
	 Cur_offd(X, Y);
	 CUR_OFF = CUR_oFF;
	 CUR_ON = CUR_oN;

	 SERV[73] = (void*)SW[0];
	 SERV[81] = (void*)SW[1];
	 SERV[71] = (void*)SW[2];
	 SERV[79] = (void*)SW[3];
	 BlokM = 0;
	komunikat0(Komunikat_R0 = 0);
 }
}


int obrot_import_kat_t90 (void)
/*---------------------------*/
{
  obrot_import_kat_rel (90) ;
  return 0;
}

int obrot_import_kat_t_90 (void)
/*----------------------------*/
{
  obrot_import_kat_rel (-90) ;
  return 0;
}

int hatch_rotate_t3(void)
/*----------------------*/
{ char sk [MaxTextLen] = "" ;
  double d ;
  double kos, koc;

  if (i__hatch_pattern_no == 0) return 0;

  Hatch_Rotate_t3();
  new_hatch_angle = s_hatch_param_df_angle - current_hatch_angle;
  kos = sin (new_hatch_angle);
  koc = cos (new_hatch_angle);
  Cur_offd (X, Y) ;
  if ((ADP!=NULL) && (ADK!=NULL))
    transformacja_blok (ADP, ADK, Px + (10*s_hatch_param_df_scale),
                      Py - (10*s_hatch_param_df_scale), kos, koc, Tobrot,0);
  current_hatch_angle = s_hatch_param_df_angle;
  Cur_ond (X, Y) ;
  return 0;
}

int hatch_rotate_t_3(void)
/*----------------------*/
{ char sk [MaxTextLen] = "" ;
  double d ;
  double kos, koc;

  if (i__hatch_pattern_no == 0) return 0;

  Hatch_Rotate_t_3();
  new_hatch_angle = s_hatch_param_df_angle - current_hatch_angle;
  kos = sin (new_hatch_angle);
  koc = cos (new_hatch_angle);
  Cur_offd (X, Y) ;
  if ((ADP!=NULL) && (ADK!=NULL))
    transformacja_blok (ADP, ADK, Px + (10*s_hatch_param_df_scale),
                        Py - (10*s_hatch_param_df_scale), kos, koc, Tobrot,0);
  current_hatch_angle = s_hatch_param_df_angle;
  Cur_ond (X, Y) ;
  return 0;
}

int hatch_rotate_t45(void)
/*----------------------*/
{ char sk [MaxTextLen] = "" ;
  double d ;
  double kos, koc;

  if (i__hatch_pattern_no == 0) return 0;

  Hatch_Rotate_t45();
  new_hatch_angle = s_hatch_param_df_angle - current_hatch_angle;
  kos = sin (new_hatch_angle);
  koc = cos (new_hatch_angle);
  Cur_offd (X, Y) ;
  if ((ADP!=NULL) && (ADK!=NULL))
    transformacja_blok (ADP, ADK, Px + (10*s_hatch_param_df_scale),
      Py - (10*s_hatch_param_df_scale), kos, koc, Tobrot,0);
     current_hatch_angle = s_hatch_param_df_angle;
  Cur_ond (X, Y) ;

  return 0;
}

int hatch_rotate_t_45(void)
/*----------------------*/
{ char sk [MaxTextLen] = "" ;
  double d ;
  double kos, koc;

  if (i__hatch_pattern_no == 0) return 0;

  Hatch_Rotate_t_45();
  new_hatch_angle = s_hatch_param_df_angle - current_hatch_angle;
  kos = sin (new_hatch_angle);
  koc = cos (new_hatch_angle);
  Cur_offd (X, Y) ;
  if ((ADP!=NULL) && (ADK!=NULL))
    transformacja_blok (ADP, ADK, Px + (10*s_hatch_param_df_scale),
      Py - (10*s_hatch_param_df_scale), kos, koc, Tobrot,0);
     current_hatch_angle = s_hatch_param_df_angle;
  Cur_ond (X, Y) ;

  return 0;
}

int noooph(void)
{
	return 0;
}

void rotate_cursor_pattern_bitmap(int i_angle)
{
	if ((cursor_pattern_bitmap!=NULL) && (cursor_pattern_bitmap0!=NULL))
	    rotate_sprite(cursor_pattern_bitmap, cursor_pattern_bitmap0, 0, 0, itofix(i_angle));  //0,0
}

int hatch_pattern_rotate_t45(void)
{
	SolidHatchPatternAngle++;
	if (SolidHatchPatternAngle > 3) SolidHatchPatternAngle = 0;

	CUR_OFF(X, Y);
	rotate_cursor_pattern_bitmap(fixangles[SolidHatchPatternAngle]);
	CUR_ON(X, Y);

	hatch_image_pattern_angle_menu(SolidHatchPatternAngle);
	return 0;
}

int hatch_pattern_rotate_t_45(void)
{
	SolidHatchPatternAngle--;
	if (SolidHatchPatternAngle < 0) SolidHatchPatternAngle = 3;

	CUR_OFF(X, Y);
	rotate_cursor_pattern_bitmap(fixangles[SolidHatchPatternAngle]);
	CUR_ON(X, Y);

	hatch_image_pattern_angle_menu(SolidHatchPatternAngle);
	return 0;
}

static void redcrpki(char typ)
{ static void (*CUR_oN)(double ,double)=nooop_;
  static void (*CUR_oFF)(double ,double)=nooop_;
  static int ( *SW[2])();
  static TMENU *menu;
  static char st[POLE_TXT_MAX]="";
  unsigned short cod = L' ';
  static int iconno=0;
  //static int npA=0;

 if (typ==0)
   {
     eA.x=maxX-PL266;
     eA.y= ESTR_Y;
     eA.lmax=r12;
     eA.val_no_max = 1 ;
     eA.mode = GV_ANGLE ;
     eA.format = "%#8.4lg" ;
     eA.ESTRF = e_imp_o;
	 eA.extend = 0;
     npA=dodajstr(&eA);
     BlokM=1;

     CUR_oFF=CUR_OFF;    CUR_OFF=cur_offpki;
     CUR_oN=CUR_ON;      CUR_ON=cur_onpki;

     SW[0]=SERV[71];  SERV[71]=obrot_import_kat_t90;  //Home  kat 0  t0
     SW[1]=SERV[79];  SERV[79]=obrot_import_kat_t_90;  //End   kat 90 t90

     komunikat0(Komunikat_R0=37);

     sprintf(eA.st, "%#8.4f", block_angle*180/Pi);
     Out_Edited_Draw_Param ((ESTR *)&eA, TRUE) ;

     getmenupini (&menu, st, &cod,&iconno) ;

     menupini (&mBlok_Imp, _BLOCK_, _BLOCK_C_, 1) ;

     if (Check_if_Equal(fixed_block_scale_x, fixed_block_scale_y)==FALSE)
         sprintf(st, "%#8.4f;%#8.4f", fixed_block_scale_x, fixed_block_scale_y);
     else sprintf(st, "%#8.4f", fixed_block_scale_x);
     menu_par_new ((*mBlok_Imp.pola)[1].txt, st) ;
     menu_par_new ((*mBlok_Imp.pola)[0].txt, st) ;

     b_check_arc = check_if_obiekt (ADP, ADK, Ablok, Oluk);
     //Cur_ond(X,Y);
     CUR_ON(X,Y);
   }
  else
  {
    usunstr(npA);
    Cur_offd(X,Y);
    CUR_OFF=CUR_oFF;
    CUR_ON=CUR_oN;
    SERV[71]= (void*)SW[0];
    SERV[79]= (void*)SW[1];
	BlokM = 0;
    menupini (menu, st, cod, iconno) ;
    komunikat0(Komunikat_R0=0);
  }
}


static void redcrpki_GEO(char typ)
{ static void (*CUR_oN)(double ,double)=nooop_;
  static void (*CUR_oFF)(double ,double)=nooop_;
  static int ( *SW[2])();
  static TMENU *menu;
  static char st[POLE_TXT_MAX];
  unsigned short cod = L' ';
  static int iconno=0;

 if (typ==0)
   {
     eL.x=maxX-PL266;
     eL.y= ESTR_Y;
     eL.lmax=r22;
     eL.val_no_max = 1 ;
     eL.mode = GV_ANGLE ;
     eL.format = "%#10.5lg" ;
     eL.ESTRF = e_imp_o;
	 eL.extend = 0;

     BlokM=1;
     CUR_oFF=CUR_OFF;    CUR_OFF=cur_offpki;
     CUR_oN=CUR_ON;      CUR_ON=cur_onpki;

     SW[0]=SERV[71];  SERV[71]=obrot_import_kat_t90;  //Home  kat 0  t0
     SW[1]=SERV[79];  SERV[79]=obrot_import_kat_t_90;  //End   kat 90 t90

     komunikat0(Komunikat_R0=37);

     getmenupini (&menu, st, &cod, &iconno) ;

     menupini (&mBlok_Imp_GEO, _BLOCK_3D_, _BLOCK_C_, 1) ;

     b_check_arc = check_if_obiekt (ADP, ADK, Ablok, Oluk);
     Cur_ond(X,Y);
   }
  else
  {

    Cur_offd(X,Y);
    CUR_OFF=CUR_oFF;
    CUR_ON=CUR_oN;
    SERV[71]= (void*)SW[0];
    SERV[79]= (void*)SW[1];
    BlokM=0;
    menupini (menu, st, cod, iconno) ;
    komunikat0(Komunikat_R0=0);
  }
}


static int PrzesunI(double *X0_, double *Y0_)
/*-----------------------------------------*/
{
  double X0,Y0;
  EVENT *ev;

  redcrpki(0);
  while(1)
  {
      if (Error) { ClearErr(); continue;}
      if (Info) { ClearInfo(); continue;}

    ev=Get_Event_Point(NULL, &X0, &Y0);
    if(ev->What == evKeyDown &&  ev->Number==0)
    {
      redcrpki(1);
      return ESC;
    }
    if (ev->What == evKeyDown &&  ev->Number == ENTER)
    {
      redcrpki(1); 
      return ENTER;
    }
    if (ev->What == evCommandP)
    {
      if (ev->Number == 0)
      {
	skala_import () ;
      }  
      if (ev->Number == 1)
      {
	ustalona_skala_import () ;
      }
      else if (ev->Number == 3)
      {
	lustro_y_y_import () ;
      }
      else if (ev->Number == 4)
      {
	lustro_x_x_import () ;
      }
      else if (ev->Number == 5)
      {
    center_to_cursor_import (X0_, Y0_) ;
      }
      else if (ev->Number == 6)
      {
	Divide_Block (Px, Py) ;
      }
      else if (ev->Number == 7)
      {
	obrot_import () ;
      }  
      else if (ev->Number == 8)
      {
	obrot_import_kat (45) ;
      }
      else if (ev->Number == 9)
      {
	obrot_import_kat (90) ;
      }
      else if (ev->Number == 10)
      {
	obrot_import_kat (135) ;
      }
      else if (ev->Number == 11)
      {
	obrot_import_kat (180) ;
      }
      else if (ev->Number == 12)
      {
	obrot_import_kat (225) ;
      }
      else if (ev->Number == 13)
      {
	obrot_import_kat (270) ;
      }
       else if (ev->Number == 14)
      {
	obrot_import_kat (315) ;
      }
	   else if (ev->Number == 15)
	  {
		  obrot_import_kat(0);
	  }
	   else if (ev->Number == 16)
	  {
		  obrot_import_K();
	  }
	   else if (ev->Number == 17)
	  {
		  obrot_kopiuj_paral(0);
	  }
	   else if (ev->Number == 18)
	  {
		  obrot_kopiuj_paral(Pi/2.0);
	  }
	   else if (ev->Number == 19)
	  {
		  obrot_kopiuj_paralK();
	  }
    }
  }
}


static int PrzesunI_GEO(void)
/*-------------------------*/
{
  double X0,Y0;
  EVENT *ev;

  redcrpki_GEO(0);
  while(1)
  {
    ev=Get_Event_Point(NULL, &X0, &Y0);
    if(ev->What == evKeyDown &&  ev->Number==0)
    {
      redcrpki_GEO(1);
      return ESC;
    }
    if (ev->What == evKeyDown &&  ev->Number == ENTER)
    {
      redcrpki_GEO(1);
      return ENTER;
    }
    if (ev->What == evCommandP)
    {
      if (ev->Number == 0)
      {
	skala_import () ;
      }  

      else if (ev->Number == 2)
      {
	lustro_y_y_import () ;
      }
      else if (ev->Number == 3)
      {
	lustro_x_x_import () ;
      }
      else if (ev->Number == 4)
      {
	Divide_Block (Px, Py) ;
      }
      else if (ev->Number == 5)
      {
	obrot_import () ;
      }  
      else if (ev->Number == 6)
      {
	obrot_import_kat (45) ;
      }
      else if (ev->Number == 7)
      {
	obrot_import_kat (90) ;
      }
      else if (ev->Number == 8)
      {
	obrot_import_kat (135) ;
      }
      else if (ev->Number == 9)
      {
	obrot_import_kat (180) ;
      }
      else if (ev->Number == 10)
      {
	obrot_import_kat (225) ;
      }
      else if (ev->Number == 11)
      {
	obrot_import_kat (270) ;
      }
      else if (ev->Number == 12)
      {
	obrot_import_kat (315) ;
	  }
	  else if (ev->Number == 13)
	  {
		  obrot_kopiuj_paral(0);
	  }
	  else if (ev->Number == 14)
	  {
		  obrot_kopiuj_paral(90);
	  }
	  else if (ev->Number == 15)
	  {
		  obrot_kopiuj_paralK();
	  }

    }
  }
}

void backup_hatch_pattern__(void)
{
    ;
}

void settle_hatch_pattern(int no)
{
    if (no==999)  //defined in DXFin
    {
        insulation_hatch = FALSE;
        set_hatch_pattern(no, 0, FALSE);
        SolidHatchPattern = 0;
        SolidHatch=0;

    }
    else {
        hatch_file_reopen();
        set_menu_hatch_pattern();
        insulation_hatch = FALSE;
        set_hatch_pattern(no, 0, FALSE);
        SolidHatchPattern = 0;
        if (no == 0) SolidHatch = 1;
        else SolidHatch = 0;

        BOOL b__temp1 = free_mem_hatch_pattern_(2);
    }
}


static int PrzesunH(double *X0, double *Y0, int kod_bloku, int comput_area)
/*-----------------------------------------------------------------------*/
{
  EVENT *ev;

  char blok_name[MaxLen] = _BLOCK__;

  static double kos, koc;
  double lastX, lastY;

  void *ad;
  BLOK *adb = NULL;
  double x0b, y0b;

  try_again:
  
  hatching_in_progress = TRUE;

  redcrP (0) ;

  if (kod_bloku == 1) redcrpkh(10, comput_area); else redcrpkh(0, comput_area);

  while(1)
  {
	 
      ev=Get_Event_Point(NULL, X0, Y0);
    if(ev->What == evKeyDown &&  ev->Number==0)
    {
      redcrpkh(1, comput_area);
      redcrP (2) ;
	  hatching_in_progress = FALSE;
      return 0;
    }

    if (ev->What == evKeyDown &&  ev->Number == ENTER)
    {
		if (comput_area == 10) //edit existing 
		{

			global_point_in_solid = TRUE;
            global_any_choice=TRUE;
			if ((ad = obiekt_LlS_wybrany()) != NULL)
			{
				global_point_in_solid = FALSE;
                global_any_choice=FALSE;
				adb = (BLOK*)find_first_hatch_block(ad, &x0b, &y0b);
				if (adb == NULL) continue;
			}
			else
			{
				global_point_in_solid = FALSE;
                global_any_choice=FALSE;
				continue;
			}
		}

      redcrpkh(1, comput_area);
      redcrP (2) ;
	  hatching_in_progress = FALSE;
      return 1;
    }
      if (ev->What == evCommandP)
       {
 	switch (ev->Number)
	{
	  case ID_PATTERN_FOLDER:	/*Temporary*/
		  redcrpkh(1, comput_area);
		  redcrP(1);
		  hatching_in_progress = FALSE;
		  return 20;
		break;
	  case ID_VPATTERN_ANGLE :

		  if (i__hatch_pattern_no == 0) break;

	    hatch_angle_rotate () ;
	    
	    new_hatch_angle = s_hatch_param_df_angle - current_hatch_angle;
	    kos = sin (new_hatch_angle);
            koc = cos (new_hatch_angle);
            Cur_offd (X, Y) ;
            if ((ADP!=NULL) && (ADK!=NULL)) 
               //transformacja_blok (ADP, ADK, Px + 10, Py - 10, kos, koc, Tobrot,0);
				transformacja_blok(ADP, ADK, Px + (10 * s_hatch_param_df_scale),
					Py - (10 * s_hatch_param_df_scale), kos, koc, Tobrot, 0);
			
            current_hatch_angle = s_hatch_param_df_angle;
            Cur_ond (X, Y) ;
	    break ;
	  case ID_VPATTERN_PARALLEL:

		  if (i__hatch_pattern_no == 0) break;

		  hatch_angle_rotate_parallel();

		  new_hatch_angle = s_hatch_param_df_angle - current_hatch_angle;
		  kos = sin(new_hatch_angle);
		  koc = cos(new_hatch_angle);
		  Cur_offd(X, Y);
		  if ((ADP != NULL) && (ADK != NULL))
			  transformacja_blok(ADP, ADK, Px + (10 * s_hatch_param_df_scale),
				  Py - (10 * s_hatch_param_df_scale), kos, koc, Tobrot, 0);

		  current_hatch_angle = s_hatch_param_df_angle;
		  Cur_ond(X, Y);
		  break;

	  case ID_VPATTERN_PERPENDICULAR:

		  if (i__hatch_pattern_no == 0) break;

		  hatch_angle_rotate_perpendicular();

		  new_hatch_angle = s_hatch_param_df_angle - current_hatch_angle;
		  kos = sin(new_hatch_angle);
		  koc = cos(new_hatch_angle);
		  Cur_offd(X, Y);
		  if ((ADP != NULL) && (ADK != NULL))
			  transformacja_blok(ADP, ADK, Px + (10 * s_hatch_param_df_scale),
				  Py - (10 * s_hatch_param_df_scale), kos, koc, Tobrot, 0);

		  current_hatch_angle = s_hatch_param_df_angle;
		  Cur_ond(X, Y);
		  break;
	  case ID_SCALE :
		  if (i__hatch_pattern_no == 0) break;
	    hatch_scale () ;
	    Cur_offd (X, Y) ;
	    if (FALSE == Check_if_Equal(current_hatch_scale,0)) 
  	      new_hatch_scale = s_hatch_param_df_scale / current_hatch_scale; 
  	    if ((ADP!=NULL) && (ADK!=NULL))  
               transformacja_blok (ADP, ADK, Px, Py, new_hatch_scale, new_hatch_scale, Tskala,0) ;
            current_hatch_scale = s_hatch_param_df_scale;
            Cur_ond (X, Y) ;
	    break ;
	  case ID_SNAP_BASE :
		  if (i__hatch_pattern_no == 0) break;
	    hatch_snap_base () ;
	    break ;
	  case ID_SNAP_BASE_POINT :
		  if (i__hatch_pattern_no == 0) break;
	    hatch_snap_base_point () ;
	    break ;  
	  case ID_PATTERN :	/*menu*/
	    break ;
	  case ID_DIST :
		hatch_dist () ;

		if (i__hatch_pattern_no != 0)
		{
			if ((ADP != NULL) && (ADK != NULL))
			{
				if ((strncmp("////////", s_hatch_param_df_name, 8) == 0) ||
					(strncmp("XXXXXXXX", s_hatch_param_df_name, 8) == 0))
				{
					Cur_offd(X, Y);
					if (FALSE == Check_if_Equal(current_hatch_distans, 0))
						new_hatch_distans = s_hatch_param_df_distans / current_hatch_distans;
					transformacja_blok(ADP, ADK, Px, Py, new_hatch_distans, new_hatch_distans, Tskala, 0);
					current_hatch_distans = s_hatch_param_df_distans;
					Cur_ond(X, Y);
				}
			}
		}
			break ;
	      case ID_INSULATION:
			  if (insulation_hatch_pattern[0] > 0)
			  {
				  set_hatch_pattern(insulation_hatch_pattern[0], 1, TRUE);
                  insulation_hatch_no=0;
				  SolidHatchPattern = 0;
				  SolidHatch = 0;
				  insulation_hatch = TRUE;

				  redcrpkh(1, comput_area);
				  redcrP(1);
				  hatching_in_progress = FALSE;
				  return 2;
			  }
			  break;
        case ID_INSULATION+1:
            if (insulation_hatch_pattern[1] > 0)
            {
                set_hatch_pattern(insulation_hatch_pattern[1], 1, TRUE);
                insulation_hatch_no=1;
                SolidHatchPattern = 0;
                SolidHatch = 0;
                insulation_hatch = TRUE;

                redcrpkh(1, comput_area);
                redcrP(1);
                hatching_in_progress = FALSE;
                return 2;
            }
            break;
		  case ID_IMAGE_PATTERN:

			  redcrpkh(11, comput_area);
			  redcrP(1);
			  hatching_in_progress = FALSE;
			  return 21;
			  break;
		  case ID_GRAB_IMAGE_PATTERN:
			  redcrpkh(11, comput_area);
			  redcrP(1);
			  hatching_in_progress = FALSE;
			  return 22;
			  break;
		  case ID_IMAGE_PATTERN_SCALE:
		  case ID_IMAGE_PATTERN_SCALE + 1:
		  case ID_IMAGE_PATTERN_SCALE + 2:
		  case ID_IMAGE_PATTERN_SCALE + 3:
		  case ID_IMAGE_PATTERN_SCALE + 4:
		  case ID_IMAGE_PATTERN_SCALE + 5:
		  case ID_IMAGE_PATTERN_SCALE + 6:
			  hatch_image_pattern_scale(ev->Number);
			  break;
		  case ID_IMAGE_PATTERN_ANGLE:
		  case ID_IMAGE_PATTERN_ANGLE + 1:
		  case ID_IMAGE_PATTERN_ANGLE + 2:
		  case ID_IMAGE_PATTERN_ANGLE + 3:
			  hatch_image_pattern_angle(ev->Number);
			  break;
	  default :
		  if ((ev->Number >= ID_IMAGE_PATTERN_DX) && (ev->Number < (ID_IMAGE_PATTERN_DX + 10)))
		  {
			  hatch_image_pattern_dx(ev->Number);
			  break;
		  }
		  else if ((ev->Number >= ID_IMAGE_PATTERN_DY) && (ev->Number < (ID_IMAGE_PATTERN_DY + 10)))
		  {
			  hatch_image_pattern_dy(ev->Number);
			  break;
		  }
		  else if ((ev->Number >= ID_TRANSLUCENCY) && (ev->Number < (ID_TRANSLUCENCY + 20)))
		  {
			  set_solidhatchtranslucency(ev->Number, 1);

			  redcrpkh(1, comput_area);
			  redcrP(1);
			  hatching_in_progress = FALSE;

              insulation_hatch=FALSE;

			  return 2;
			  break;
		  }

		insulation_hatch = FALSE;
	    set_hatch_pattern (ev->Number - ID_END, 1, FALSE) ;
		SolidHatchPattern = 0;
		if ((ev->Number - ID_END)==0) SolidHatch = 1;
		else SolidHatch = 0;

	    redcrpkh(1, comput_area);
            redcrP (1) ;
		hatching_in_progress = FALSE;
	    return 2;    
	    break ;
	}
     }	
  }
}

static void redcrH(char typ)
{ static void (*CUR_oN)(double ,double)=nooop_;
  static void (*CUR_oFF)(double ,double)=nooop_;
  switch(typ)
   { case 0 :

      komunikat0 (0) ;
      sel_akt=sel.akt;
	  sel_cur = sel.cur;
	  sel_gor = sel.gor;
	  
      ADP=ADK=NULL;


	  CUR_oFF = CUR_OFF;
	  CUR_oN = CUR_ON;

      break;
     case 11 : 
		 CUR_OFF(X, Y);

		 CUR_OFF =out_sel_off;
		 CUR_ON = out_sel_on;
		 sel.akt=1;   

		 sel.gor = 1;
		 CUR_ON(X, Y);

		 return;
		 break;
	 case 1:

		 return;
		 break;
     case 2 :
      if(ADP != NULL)
       { zmien_atrybut(ADP,ADK,Ablok,Ausuniety);
	 usun_blok(ADP,ADK);
       }
      break;
     case 3 :
      zmien_atrybut(ADP,ADK,Ablok,Ausuniety);
      usun_blok(ADP,ADK);

	  CUR_OFF = CUR_oFF;
	  CUR_ON = CUR_oN;

	  sel.akt = sel_akt;
	  sel.cur = sel_cur;
	  sel.gor = sel_gor;

	  return;
      break;
     case 4 :
      memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
      zmien_atrybut_undo(dane, dane + dane_size);
      set_insert_point (X, Y, (void*)ADP) ; /* patrz ReadBlock_ i Get_List_Block*/
      zmien_atrybut(ADP,ADK,Ablok,Aoblok);

      break;
     case 5 :
      memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
      zmien_atrybut_undo(dane, dane + dane_size);
      set_insert_point (X, Y, (void*)ADP) ; /* patrz ReadBlock_ i Get_List_Block*/
      zmien_atrybut(ADP,ADK,Ablok,Aoblok);
      break;
     default : break;
   }
  if (typ == 2 || typ==3 || typ==4 || typ == 5)
   { Cur_offd(X,Y); 

     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;

     sel.akt=sel_akt; 
	 sel.cur = sel_cur;

	 Cur_ond(X, Y);
   }
}

static void redcrI(char typ)
{ static void (*CUR_oN)(double ,double)=nooop_;
  static void (*CUR_oFF)(double ,double)=nooop_;
  static int sel_akt;
  switch(typ)
   { case 0 :
	  CUR_OFF(X, Y);
      komunikat0 (0) ;
      sel_akt=sel.akt;
      sel.akt=1; 
      ADP=ADK=NULL;  
      CUR_oFF=CUR_OFF;  CUR_OFF=cursel_off;
      CUR_oN=CUR_ON;    CUR_ON=cursel_on;

	  CUR_ON(X, Y);
      break;
     case 1 : 
		 CUR_OFF(X, Y);

		 CUR_OFF = CUR_oFF;
		 CUR_ON = CUR_oN;

		 sel.akt = sel_akt;
		 CUR_ON(X, Y);

		 return;
	  break;
	 case 10:
		 CUR_OFF(X, Y);

		 CUR_OFF = CUR_oFF;
		 CUR_ON = CUR_oN;
		 sel.akt=0;   
		 CUR_ON(X, Y);

		 return;
		 break;
     case 2 :
      if(ADP != NULL)
       { zmien_atrybut(ADP,ADK,Ablok,Ausuniety);
	 usun_blok(ADP,ADK);
       }
      break;
     case 3 :
      zmien_atrybut(ADP,ADK,Ablok,Ausuniety);
      usun_blok(ADP,ADK);
	  CUR_OFF(X, Y);
	  CUR_OFF = CUR_oFF;
	  CUR_ON = CUR_oN;
	  sel.akt = sel_akt;
	  CUR_ON(X, Y);
	  return;
      break;
     case 4 :
      memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
      zmien_atrybut_undo(dane, dane + dane_size);
      set_insert_point (X, Y, (void*)ADP) ; /* patrz ReadBlock_ i Get_List_Block*/
      zmien_atrybut(ADP,ADK,Ablok,Aoblok);

      break;
     case 5 :
      memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
      zmien_atrybut_undo(dane, dane + dane_size);
      set_insert_point (X, Y, (void*)ADP) ; /* patrz ReadBlock_ i Get_List_Block*/
      zmien_atrybut(ADP,ADK,Ablok,Aoblok);
      break;
      case 6 :
       memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
       zmien_atrybut_undo(dane, dane + dane_size);
       zmien_atrybut(ADP,ADK,Ablok,Aoblok);

       break;
     default : break;
   }
  if(typ==2 || typ == 5)
   {

	 CUR_OFF(X, Y);
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     sel.akt=sel_akt;
	 CUR_ON(X, Y);
   }
}

int Get_Background_From_Catalog(int opcja)
{

	double X0, Y0;
	int status;

	char blok_name[MaxLen];
	if (opcja==0) strcpy(blok_name, _BACKGROUND_);
	else if (opcja == 1) strcpy(blok_name, _SOLID_IMAGE_);
	else if (opcja == 2) strcpy(blok_name, _HATCH_IMAGE_);
	else if (opcja == 3) strcpy(blok_name, _TRACE_IMAGE_);
	else strcpy(blok_name, _IMAGE_);

	char st[MaxTextLen];
	char fn[MaxLen] = "";
	int l_kr;
	static int(*SW[3])();
	int k;

	dane_size0 = dane_size;

	Error = 0;
	redcrI(0);

	if (!GetBackground(fn, MaxLen, opcja, background_pathname, background_pathname0))
	{
		redcrI(10);
		return 0;
	}
	else
	{
		redcrI(1);
		return 1;
	}

}


void Place_Import_Block_From_Catalog(int opcja)
{

	double X0, Y0;
	int status;

	char blok_name[MaxLen] = _BLOCK__;

	char st[MaxTextLen];
	int l_kr;
	static int(*SW[3])();
	int k;

	dane_size0 = dane_size;

	Error = 0;
	redcrI(0);

	if (!FileNameOC((char *) &catalog_fn, MaxLen))
	{
		redcrI(10);
		return;
	}
	else
	{
		redcrI(1);
		return;
	}
}

void Select_Pattern_From_Folder(int opcja)
{

	double X0, Y0;
	int status;

	char blok_name[MaxLen] = _PATTERN_;

	char st[MaxTextLen];
	char fn[MaxLen] = "";
	int l_kr;
	static int(*SW[3])();
	int k;

	dane_size0 = dane_size;

	Error = 0;
	redcrI(0);

	if (!FileNamePattern((char **) fn, MaxLen))
	{
		Cur_ond(X, Y);
		redcrI(1);
		sel.akt = 0;
		return;
	}
	else
	{
		Cur_ond(X, Y);
		redcrI(1);
		sel.akt = 0;
		return;
	}
}

void Place_Import_Block (int opcja, char *blockfile)
/*------------------------------------------------*/
{
  double X0,Y0;
  int status;

  char blok_name[MaxLen] = _BLOCK__;

  char st[MaxTextLen];
  char fn[MaxLen]=u8"";
  int l_kr;
  static int ( *SW[3])() ;
  int k;

  dane_size0=dane_size;

  Error=0;  
  redcrI(0);
  if (opcja == 0)
  {
      if(!FileNameI(fn,MaxLen))
       {
         redcrI(1);
	      return;
       }
       else
       {
		  ;
       }
     k=ReadBlock_(fn,&X0,&Y0,&ADP,&ADK, &blok_name [strlen(blok_name)], MaxLen - strlen(blok_name)-1, &w__object_no, TRUE);
	 block_angle = 0.0;
     if ((k>0) && (k!=5))
      { 
		 redcrI(2);
	    return;
      }
	 else
	 {
		 CUR_OFF(X, Y);
	 }
  }
  else if (opcja == 4)
    {
        k=ReadBlock_(blockfile,&X0,&Y0,&ADP,&ADK, &blok_name [strlen(blok_name)], MaxLen - strlen(blok_name)-1, &w__object_no, TRUE);
        block_angle = 0.0;
        if ((k>0) && (k!=5))
        {
            redcrI(2);
            return;
        }
        else
        {
            CUR_OFF(X, Y);
        }
    }

  else if (opcja==1)
  {
     w__object_no = 1;
	 block_angle = 0.0;
     if (FALSE == Get_List_Block (&X0, &Y0, &ADP, &ADK, &blok_name [strlen(blok_name)], MaxLen - strlen(blok_name)-1))
     {
       redcrI (1) ;
       return ;
     }
	 else
	 {
		 CUR_OFF(X, Y);
	 }
    if (options1.save_original_layer==0) change_layer (ADP, ADK, Current_Layer);  
      else normalize_layer (ADP, ADK);
  }
  else if (opcja==2) 
  {
    /* sprawdzenie obecnosci bloku normalnie usuwanego na poczatku sesji */
    strcpy(fn,"blk00000.alx");
    status = access(fn,0);
    if (status != 0)
    {
     ErrList(94);
     redcrI(2);
     return;
    }
	block_angle = 0.0;
    if(ReadBlock_(fn,&X0,&Y0,&ADP,&ADK, &blok_name [strlen(blok_name)], MaxLen - strlen(blok_name)-1, &w__object_no, TRUE))
     { redcrI(2);
      return;
     }
	else
	{
		CUR_OFF(X, Y);
	}

  }
  else if (opcja==3) 
  {
    strcpy(fn,symbol_name);
    status = access(fn,0);
    if (status != 0)
    {
     strcpy(st, _NO_BLOCK_IN_CATALOG_);
     strcat(st,fn);
     strcat(st,")");
     komunikat_str (st);
     redcrI(2);
     return;
    }
	block_angle = 0.0;


    if(ReadBlock_(fn,&X0,&Y0,&ADP,&ADK, &blok_name [strlen(blok_name)], MaxLen - strlen(blok_name)-1, &w__object_no, TRUE))
     { redcrI(2);
      return;
     }
	else
	{
		CUR_OFF(X, Y);
	}
  }
  else if (opcja==5)
  {
      BLOK *b;
      b=(BLOK*)dane;
      ADP=dane;
      ADK=(char*)b+b->n+sizeof(NAGLOWEK) -1;
      strcpy(blok_name, blockfile);
	  X0 = 0; Y0 = 0;

  }

  if (Error != 0)
  {
     delay (2000) ;  // ReadBlock_ moze komunikowac obledzie,
		     // komunikat_str wyswietlany jest w tym samym miejscu,
		     // przeniesc docelowo do b_message
     Error = 0;		     
  }  
  if (blok_name [0] != 0)
  {
     blok_name [MaxLen - 1] = '\0';
     komunikat_str (blok_name);
  }
  DX=X-X0; DY=Y-Y0;
  Px=X0;Py=Y0;
      SW[0]=SERV[73];  SERV[73]=sel_t;
      SW[1]=SERV[81];  SERV[81]=sel_d;
     if (opcja==1)
     {
         current_block_scale_x=1.0;
         current_block_scale_y=1.0;
     }
     else {
         current_block_scale_x = fixed_block_scale_x;
         current_block_scale_y = fixed_block_scale_y;

         if ((Check_if_Equal(fixed_block_scale_x, 1.0) == FALSE) ||
             (Check_if_Equal(fixed_block_scale_y, 1.0) == FALSE)) {
             Cur_offd(X, Y);

             transformacja_blok(ADP, ADK, Px, Py, fixed_block_scale_x, fixed_block_scale_y, Tskala, 0);
             if (FALSE == Check_if_Equal (fixed_block_scale_x, fixed_block_scale_y))
             {
                 trans_scale_arcs (Px,Py, fixed_block_scale_x, fixed_block_scale_y, FALSE) ;
             }
             Cur_ond(X, Y);
         }
     }

	  if (PrzesunI(&X0, &Y0) == ESC)
	  {
		  Cur_ond(X, Y);
		  redcrI(3);
	  }
      else
       {
         TTF_redraw = FALSE;
         transformacja_blok(ADP,ADK,X-X0,Y-Y0,0,0,Tprzesuw,0);
         blokzap(ADP,ADK,Ablok,COPY_PUT,1);
         CUR_OFF(X, Y);
         CUR_ON(X, Y);

         if (opcja!=5) redcrI(4);
         else redcrI(6);
         if (TTF_redraw) redraw();

         if (opcja!=5) KopiujM ();
         redcrI(1);
       }
  komunikat (0);

  SERV[73]= (void*)SW[0];
  SERV[81]= (void*)SW[1];
  return;
}


void Place_Import_Block_DXF(int opcja)
/*-----------------------------------*/
{
	double X0, Y0;
	int status;

	char blok_name[MaxLen] = _BLOCK__;

	char st[MaxTextLen];
	char fn[MaxLen] = "";
	int l_kr;
	static int(*SW[3])();
	int k, no_lines;
    double Jednostki_, SkalaF_;

	dane_size0 = dane_size;

	Error = 0;
	redcrI(0);
	if (opcja == 0)
	{

		if (!FileNameIDXF(fn, MaxLen))
		{
			redcrI(1);
			return;
		}
		else
		{
			;
		}

		block_angle = 0.0;
        backup_type_width_kolor();
        backup_hatch_pattern();
        Jednostki_ = Jednostki;
        SkalaF_ = SkalaF;
		no_lines = czytaj_dxf_blok(fn, &X0, &Y0, &ADP, &ADK, &blok_name[strlen(blok_name)], 1.0 /*Jednostki_dxf*/, 1.0 /*SkalaF_dxf*/, 1 /*latin2_dxf*/, 0);
        Jednostki = Jednostki_;
        SkalaF = SkalaF_;
        restore_type_width_kolor();
        restore_hatch_pattern();
		if (no_lines > 0) k = 0; else k = 1;

		if ((ADP == NULL) || (ADK == NULL)) return;
		
		if ((k > 0) && (k != 5))
		{
			redcrI(2);
			return;
		}
		else
		{
			Cur_offd(X, Y);
		}
		if (options1.save_original_layer == 0) change_layer(ADP, ADK, Current_Layer);
		else normalize_layer(ADP, ADK);

	}
	else if (opcja == 3)
	{
		Error = 0;
		Info = 0;
		strcpy(fn, symbol_name);
		status = access(fn, 0);
		if (status != 0)
		{
			strcpy(st, "Brak symbolu w katalogu  (");
			strcat(st, fn);
			strcat(st, ")");
			komunikat_str(st);
			redcrI(1);
			return;
		}
		else
		{
			;
		}

		block_angle = 0.0;
        backup_type_width_kolor();
        backup_hatch_pattern();
        Jednostki_ = Jednostki;
        SkalaF_ = SkalaF;
        no_lines = czytaj_dxf_blok(fn, &X0, &Y0, &ADP, &ADK, &blok_name[strlen(blok_name)], 1.0 /*Jednostki_dxf*/, 1.0 /*SkalaF_dxf*/, 1 /*latin2_dxf*/, 0);
        Jednostki = Jednostki_;
        SkalaF = SkalaF_;
        restore_type_width_kolor();
        restore_hatch_pattern();
		if ((ADP == NULL) || (ADK == NULL)) return;

		if (no_lines > 0) k = 0; else k = 1;
		if ((k > 0) && (k != 5))
		{
			redcrI(2);

			return;
		}
		else
		{
			Cur_offd(X, Y);
		}
		if (options1.save_original_layer == 0) change_layer(ADP, ADK, Current_Layer);
		else normalize_layer(ADP, ADK);

	}
	if (Error != 0)
	{
		delay(3000);  // ReadBlock_ moze komunikowac obledzie,
				// komunikat_str wyswietlany jest w tym samym miejscu,
				// przeniesc docelowo do b_message
		Error = 0;
	}
	if (blok_name[0] != 0)
	{
		blok_name[MaxLen - 1] = '\0';
		komunikat_str(blok_name);
	}
	DX = X - X0; DY = Y - Y0;
	Px = X0; Py = Y0;
	SW[0] = SERV[73];  SERV[73] = sel_t;
	SW[1] = SERV[81];  SERV[81] = sel_d;

    current_block_scale_x=fixed_block_scale_x;
    current_block_scale_y=fixed_block_scale_y;

    if ((Check_if_Equal(fixed_block_scale_x, 1.0)==FALSE) || (Check_if_Equal(fixed_block_scale_y, 1.0)==FALSE)) {
        Cur_offd (X, Y) ;

        transformacja_blok(ADP, ADK, Px, Py, fixed_block_scale_x, fixed_block_scale_y, Tskala, 0);
        if (Check_if_Equal(fixed_block_scale_x, fixed_block_scale_y)==FALSE)
        {
            //changing circles to ellipses and arcs to eliptic arcs
            trans_scale_arcs (Px,Py, fixed_block_scale_x, fixed_block_scale_y, FALSE) ;
        }
        Cur_ond (X, Y) ;
    }
	
	if (PrzesunI(&X0, &Y0) == ESC)
	{
		Cur_ond(X, Y);
		redcrI(3);
	}
	else
	{
		TTF_redraw = FALSE;
		transformacja_blok(ADP, ADK, X - X0, Y - Y0, 0, 0, Tprzesuw, 0);
		blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
        CUR_OFF(X, Y);
		CUR_ON(X, Y);
		redcrI(4);
		if (TTF_redraw) redraw();
		KopiujM();
		redcrI(1);
	}
	komunikat(0);

	SERV[73] = (void*)SW[0];
	SERV[81] = (void*)SW[1];
	return;
}


//////////////
void Place_Block__ (char *adp, char *adk)
/*-----------------------------------*/
{
  int status;

  char blok_name[MaxLen] = _BLOCK__;

  char st[MaxTextLen];
  char fn[MaxLen]="";
  int l_kr;
  double X0=0, Y0=0;
  static int ( *SW[3])() ;

  dane_size0=dane_size;

  Error=0;  
  redcrI(0);
  sel.akt=1;

  ADP=adp; ADK=adk;

  SW[0]=SERV[73];  SERV[73]=sel_t;
  SW[1]=SERV[81];  SERV[81]=sel_d;
  if(PrzesunI(&X0, &Y0)==ESC) redcrI(3);
  else
   { transformacja_blok(ADP,ADK,X,Y,0,0,Tprzesuw,0);
     Cur_offd(X, Y);
     blokzap(ADP,ADK,Ablok,COPY_PUT,1);
	 Cur_ond(X, Y);
     redcrI(4);
     KopiujM ();
   }
  komunikat (0);
  sel.akt = 0;
  SERV[73]= (void*)SW[0];
  SERV[81]= (void*)SW[1];
  return;
}


void Place_Wstaw_PCX (int opcja)
/*-----------------------------*/
{
  double X0,Y0;
  int status;
  char st[MaxTextLen];
  char fn[MaxLen]="";
  int l_kr;
  static int ( *SW[3])() ;
  char blok_type [30] = "";
  char blok_name [60] = "";
  double XX, YY;
  int akt;

  Error=0;  
  redcrI(0);
  if (opcja == 0)
  {
      if(!FileNamePCX(fn,MaxLen))
       { redcrI(1);
	     return;
       }
       else
       {
		  ;

       }
      if(ReadPCX_real(fn,&X0,&Y0,&ADP,&ADK, &blok_name [strlen(blok_name)], MaxLen - 10, &w__object_no, TRUE))
      { 
		  redcrI(2);
       return;
      }
	  else
	  {
		  ;
	  }
  }

  if (Error != 0)
  {
     delay (3000) ;  // ReadBlock_ moze komunikowac obledzie,
		     // komunikat_str wyswietlany jest w tym samym miejscu,
		     // przeniesc docelowo do b_message
     Error = 0;		     
  }  
  if (blok_name [0] != 0)
  {
     blok_name [MaxLen - 1] = '\0';
     komunikat_str (blok_name);
  }

  return;
}

void Place_Wstaw_PNG_JPG(int opcja, int type)
/*-----------------------------------------*/
{
	double X0, Y0;
	int status;
	char st[MaxTextLen];
	char fn[MaxLen] = "";
	int l_kr;
	static int(*SW[3])();
	char blok_type[30] = "";
	char blok_name[60] = "";
	double XX, YY;
	int akt;

	Error = 0;
	redcrI(0);
	if (opcja == 0)
	{
		if (!FileNamePCX(fn, MaxLen))
		{
			redcrI(1);
			return;
		}
		else
		{
			;
		}
		if (Read_PNG_JPG_real(fn, &X0, &Y0, &ADP, &ADK, &blok_name[strlen(blok_name)], MaxLen - 10, &w__object_no, TRUE, type))
		{
			redcrI(2);
			return;
		}
		else
		{
			;
		}
	}

	if (Error != 0)
	{
		delay(3000);  // ReadBlock_ moze komunikowac obledzie,
				// komunikat_str wyswietlany jest w tym samym miejscu,
				// przeniesc docelowo do b_message
		Error = 0;
	}
	if (blok_name[0] != 0)
	{
		blok_name[MaxLen - 1] = '\0';
		komunikat_str(blok_name);
	}
	
	return;
}

void Convert_Wstaw_ALX(void)
{
    char fn[MaxLen] = "";
    int ret;

    Error = 0;
    redcrI(0);

    if (!FileNamePCX2BMP(fn, MaxLen))
    {
        redcrI(1);
        return;
    }
    else
    {
        ;
    }

    ret = Convert_Image_to_ALX(fn);

    if (ret)
    {
        Place_Import_Block (4, "temp.alx");
    }

}

void Export_Block_PCX (char *adr)
/*-----------------------------*/
{
  double X0,Y0;
  int status;
  char st[MaxTextLen];
  char fn[MaxLen]="";
  int l_kr;
  static int ( *SW[3])() ;
  char blok_type [30] = "";
  double XX, YY;
  PCXheader head;
  FILE *fp;
  B_PCX *pcx;

  Error=0;  
  redcrI(0);
  if(!FileNamePCX_Export(fn,MaxLen))
   { redcrI(1);

     return;
   }
    else
     {

      redcrI(1);
     }
    //export bloku PCX
    pcx=(B_PCX *)adr;

    fp=fopen(fn,"w+b");
    if(!fp)
     {
	  strcpy(st, _PCX_WRITE_ERROR_);

      ErrListStr(st);
      return;
     }
    if (pcx->markers==1)
      {
       fwrite(&pcx->pcx, pcx->n - T294 - sizeof(STRIPS), 1, fp);
      }
       else
        {
         fwrite(&pcx->pcx, pcx->n - T294, 1, fp);
        }

    fclose(fp);

    return;
}

void Export_Block_PNG(char *adr)
/*--------------------------------*/
{
	double X0, Y0;
	int status;
	char st[MaxTextLen];
	char fn[MaxLen] = "";
	int l_kr;
	static int(*SW[3])();
	char blok_type[30] = "";
	double XX, YY;
	PCXheader head;
	FILE *fp;
	B_PCX *pcx;
	char *png;

	Error = 0;
	redcrI(0);
	if (!FileNamePNG_Export(fn, MaxLen))
	{
		redcrI(1);
		return;
	}
	else
	{

		redcrI(1);
	}

	//export bloku PNG
	pcx = (B_PCX *)adr;
	png = pcx->pcx + sizeof(PCXheader);

	fp = fopen(fn, "w+b");
	if (!fp)
	{
		strcpy(st, _PNG_WRITE_ERROR_);

		ErrListStr(st);
		return;
	}

	if (pcx->markers == 1)
	{
		fwrite(png, pcx->n - T294 - sizeof(PCXheader) /*- sizeof(STRIPS)*/, 1, fp);
	}
	else
	{
		fwrite(png, pcx->n - T294 - sizeof(PCXheader), 1, fp);
	}

	fclose(fp);

	return;
}

void Export_Block_JPG(char* adr)
/*--------------------------------*/
{
	double X0, Y0;
	int status;
	char st[MaxTextLen];
	char fn[MaxLen] = "";
	int l_kr;
	static int(*SW[3])();
	char blok_type[30] = "";
	double XX, YY;
	PCXheader head;
	FILE* fp;
	B_PCX* pcx;
	char* png;

	Error = 0;
	redcrI(0);
	if (!FileNameJPG_Export(fn, MaxLen))
	{
		redcrI(1);
		return;
	}
	else
	{

		redcrI(1);
	}

	//export bloku JPG
	pcx = (B_PCX*)adr;
	png = pcx->pcx + sizeof(PCXheader);

	fp = fopen(fn, "w+b");
	if (!fp)
	{
		strcpy(st, _JPG_WRITE_ERROR_);
		ErrListStr(st);
		return;
	}

	if (pcx->markers == 1)
	{
		fwrite(png, pcx->n - T294 - sizeof(PCXheader) - sizeof(STRIPS), 1, fp);
	}
	else
	{
		fwrite(png, pcx->n - T294 - sizeof(PCXheader), 1, fp);
	}

	fclose(fp);

	return;
}




int Write_Block_PCX (char *adr)
/*-----------------------------*/
{
  double X0,Y0;
  int status;

  char blok_name[MaxLen] = _BLOCK__;

  char st[MaxTextLen];
  char fn[MaxLen]="";
  int l_kr;
  static int ( *SW[3])() ;
  char blok_type [30] = "";
  double XX, YY;
  PCXheader head;
  FILE *fp;
  B_PCX *pcx;
 // T_Prot_Date * ptrs_prot ;
  char path [MAXPATH], *en ;

  Error=0;  
    //export bloku PCX
    pcx=(B_PCX *)adr;

    strcpy(fn,"$alfpro.pcx");

    fp=fopen(fn,"w+b");
    if(!fp)
     {
      strcpy(st,"Błąd zapisu pliku '$alfpro.pcx'");
      ErrListStr(st);
      return 0;
     }
    if (pcx->markers==1)
      {
       fwrite(&pcx->pcx, pcx->n - T294 - sizeof(STRIPS), 1, fp);
      }
       else
        {
         fwrite(&pcx->pcx, pcx->n - T294, 1, fp);
        }

    fclose(fp);

    return 1;
}

int Import_View_Hatch_Pattern (char *hatch_file, double *ptrdf_x, double *ptrdf_y, int comput_area)
/*-------------------------------------------------------------------------------------------------*/
{
  double X0,Y0;
  int status;

  char blok_name[MaxLen] = _PATTERN_;

  char st[MaxTextLen];
  char fn[MaxLen]="";
  int l_kr;
  static int ( *SW[3])() ;
  char blok_type [30] = "";
  double XX, YY;
  long_long ADP_, ADK_;
  int ret_przesunh;
  double kos, koc;
  double scale_distans;
  static T_change_param s__change_param_h = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
   					     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	                     3, 1} ;
  double add_scale;
  int kod_bloku;
  char hatch_path[MAXPATH];

  sprintf(hatch_path, "%s%c%s", __PATTERNS__, Slash, hatch_file);


  if (!__file_exists((char *) &hatch_path))
  {
	  sprintf(hatch_path, "%s%c%s", __PATTERNS__, Slash, _NO_PATTERN_);
  }

  Error=0;
  redcrP (0);
  
  memmove(&UNDO_REC, &UNDO_REC_255, sizeof(UNDO_TAB_REC));
  zmien_atrybut_undo(dane, dane + dane_size);

  ADP = dane;  ////
  ADK = dane + dane_size; ////
 
  if (blok_xorput == FALSE)
   { 
    blokzap(ADP,ADK,Ablok,XOR_PUT,1); //is not redrawn, however Ablock changing to Aoblok
     blok_xorput = TRUE;
   }
 
  zmien_atrybut(ADP,ADK,Ablok,Aoblok);
  
  redcrH (0);

  redcrH (1);
 

      if(ReadBlock_(hatch_path,&X0,&Y0,&ADP,&ADK, &blok_name [strlen(blok_name)], MaxLen - 12, &w__object_no, TRUE))
      {   
        ADP=NULL;
        ADK=NULL;
        current_hatch_angle = 0.0;
        current_hatch_scale = 1.0;
        current_hatch_distans = 1.0;

      }
       else
        {

		 s__change_param_h.b_layer = 1;
         s__change_param_h.layer=Current_Layer;

		 s__change_param_h.b_drawpoly = 1;

		 if (strcmp(hatch_file, __IMAGE_AXX) != 0)
		 {
          s__change_param_h.b_color = 1;
		  draw_hatch_image_pattern = FALSE;
		 }
		 else
		 {
			 s__change_param_h.b_color = 0;
			 draw_hatch_image_pattern = TRUE;
		 }

		 change_properties_h (ADP, ADK, &s__change_param_h);

		 draw_hatch_pattern = TRUE;
		

          if (((s_hatch_param_df_angle != 0) || (s_hatch_param_df_scale != 1) ||
              (s_hatch_param_df_distans != 1)) 
              && (strcmp(hatch_file,GEOMETRY_AXX)!=0) 
			  && (strcmp(hatch_file, SOLID_AXX) != 0) 
			  && (strcmp(hatch_file, __SOLID_AXX) != 0)
			  && (strcmp(hatch_file, __IMAGE_AXX) != 0))
           {

            kos = sin (s_hatch_param_df_angle);
            koc = cos (s_hatch_param_df_angle);
            
            transformacja_blok (ADP, ADK, Px + 10, Py - 10, kos, koc, Tobrot,0);
            current_hatch_angle = s_hatch_param_df_angle;

  	    new_hatch_scale = s_hatch_param_df_scale;
            current_hatch_scale = s_hatch_param_df_scale;
            
            scale_distans = new_hatch_scale;   
            
            if ((strncmp("////////",s_hatch_param_df_name,8)==0) ||
                (strncmp("XXXXXXXX",s_hatch_param_df_name,8)==0))
                 {
                   new_hatch_distans = s_hatch_param_df_distans;
                   current_hatch_distans = s_hatch_param_df_distans;
                   
                   scale_distans *= new_hatch_distans;   
                 }  
             
            transformacja_blok (ADP, ADK, Px, Py, scale_distans, scale_distans, Tskala,0) ;

           } 
            else 
             {
              current_hatch_angle = 0;
              current_hatch_scale = 1;
              current_hatch_distans = 1;
             }

         if (blok_name [0] != 0)
          {
             if (insulation_hatch==TRUE)  strcat(blok_name, AUTOSCALE);

             blok_name [MaxLen - 1] = '\0';
             komunikat_str_len (blok_name);
          }   
        }           

  
  DX=X-X0; DY=Y-Y0;
  Px=X0;Py=Y0;
  free_mem();
  free_mem();
  free_mem();
  free_mem();

  if (strcmp(hatch_file,GEOMETRY_AXX)==0) kod_bloku = 1; else kod_bloku = 0;

try_again:
  
    ret_przesunh=(PrzesunH(ptrdf_x, ptrdf_y, kod_bloku, comput_area));
  if (ret_przesunh==0) 
    {
      redcrH(3);
      zmien_atrybut(dane,dane + dane_size,Aoblok,Ablok);

      komunikat (0);
      sel.akt = 0;
      draw_hatch_pattern = FALSE;
      return 0;
    }  
    else if (ret_przesunh==2 )
      {
       redcrH(3);
       zmien_atrybut(dane,dane + dane_size,Aoblok,Ablok);
       komunikat (0);
       sel.akt = 0;
       draw_hatch_pattern = FALSE;
       return 2;
      }
	else if (ret_przesunh == 20) //folder
	  {
		  redcrH(3);
		  zmien_atrybut(dane, dane + dane_size, Aoblok, Ablok);
		  komunikat(0);
		  sel.akt = 0;
		  draw_hatch_pattern = FALSE;
		  return 20;
	  }
	else if (ret_przesunh == 21) //image folder
	  {
		  redcrH(3);
		  zmien_atrybut(dane, dane + dane_size, Aoblok, Ablok);
		  komunikat(0);
		  sel.akt = 0;
		  draw_hatch_pattern = FALSE;
		  return 21;
	  }
	else if (ret_przesunh == 22) //grab image
		  {
		  redcrH(3);
		  zmien_atrybut(dane, dane + dane_size, Aoblok, Ablok);
		  komunikat(0);
		  sel.akt = 0;
		  draw_hatch_pattern = FALSE;
		  return 22;
	  }
    else //1
     {
      redcrH(3);
      Cur_ond(X,Y);

      komunikat (0);
     
      zmien_atrybut(dane,dane + dane_size,Aoblok,Ablok);

      draw_hatch_pattern = FALSE;
      return 1;
    }  
}



void Import (void)
/*--------------*/
{
  global_no_pcx = TRUE;
  Place_Import_Block (0, NULL) ;
  global_no_pcx = FALSE;
}

void Import_DXF(void)
/*--------------*/
{
	Place_Import_Block_DXF(0);
}

void Open_Catalog(void)
/*-------------------*/
{
	Place_Import_Block_From_Catalog(0);
}

void Open_Backgrounds(void)
/*-----------------------*/
{
	int ret;
	ret = Get_Background_From_Catalog(0);
}

int Open_SolidPatterns(void)
/*------------------------*/
{
	return Get_Background_From_Catalog(1);
}

int Open_SolidHatchPatterns(void)
/*-----------------------------*/
{
	return Get_Background_From_Catalog(2);
}

int Open_TracePatterns(void)
/*------------------------*/
{
	return Get_Background_From_Catalog(3);
}


void WstawPCX (void)
/*-----------------*/
{
  Place_Wstaw_PCX (0);
}

void WstawPNG(void)
/*-----------------*/
{
	Place_Wstaw_PNG_JPG(0, 0);
}

void WstawJPG(void)
/*-----------------*/
{
	Place_Wstaw_PNG_JPG(0, 1);
}

void VectorizeALX(void)
/*-----------------*/
{
    Convert_Wstaw_ALX();
}

void WezBlok (void)
/*--------------*/
{
  global_no_pcx = TRUE;
  Place_Import_Block (2, NULL) ;
  global_no_pcx = FALSE;
}

void WezSymbol (char * sym_name)
/*-------------------------------*/
{
  global_no_pcx = TRUE;
  strcpy(symbol_name, sym_name);
  Place_Import_Block (3, NULL) ;
  global_no_pcx = FALSE;
}

void WezSymbol_DXF(char * sym_name)
/*-------------------------------*/
{
	strcpy(symbol_name, sym_name);
	Place_Import_Block_DXF(3);
}

void set_block_angle(double angle)
{
    block_angle=angle;
}
double get_block_angle(void)
{
    return block_angle;
}


#undef __O_IMPORT__