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

#define __O_ARRAY__
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

#include "menu.h"

extern int Get_Key (void);
extern void Out_Edited_Draw_Param (ESTR *lps_et, BOOL out) ;
extern BOOL Add_Block (char  *,char  *, long *, double, double, double, double, double, double, double, double, int) ;
extern double Grid_to_Rad (double) ;
extern void Rotate_Point (double, double, double, double, double, double, double *,double *) ;
extern void outokno_a(OKNO *O, LINIA *L1);
extern void orto_l(LINIA *L, int *Orto_Dir);
extern int isometric_to_cartesian(double ix, double iy, double *cx, double *cy);

static int np ;

#define MAX_ITEMS_NO  INT_MAX

typedef struct
{
   int rows_no ;
   int columns_no ;
   double df_dist_x ;
   double df_dist_y ;
}
t_array_rect ;

typedef struct
{
   double df_xbp, df_ybp ;
   double df_xcp, df_ycp ;
   BOOL b_rotation ;
   int input_type ;
   int items_no ;
   double df_angle_to_fill ;		/*w stopniach*/
}
t_array_polar ;

enum POLAR_TYPE { IT_AF = 1,	/*licza elementow - kat wypelnienia*/
		  IT_ABI = 2,   /*licza elementow - kat pomiedzy elementami*/
		  ABI_AF = 3 } ;/*kat pomiedzy elementami - kat wypelnienia*/


static t_array_rect s_array_rect = {1, 1, 0, 0} ;
static t_array_polar s_array_polar = {0, 0, 0, 0, TRUE, IT_AF, 1, 360} ;
static  OKNO s_window ;
static ESTR e_items_no, e_angle_to_file ;
#define r18 18
#define r22 16 //22
static int strwyj;

TMENU mArray_polar = { 2,0,0,34,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmArray_polar,NULL,NULL };

TMENU mArray_polar_rot={2,0,0,34,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmArray_polar_rot,NULL,NULL};

//#include <math.h>

/*
 * Transform partial derivatives (gradients) from isometric to Cartesian coordinates.
 *
 * Inputs:
 *   df_dx_iso - Partial derivative df/di_x in isometric system
 *   df_dy_iso - Partial derivative df/di_y in isometric system
 *
 * Outputs:
 *   df_dx_cart - Partial derivative df/dc_x in Cartesian system
 *   df_dy_cart - Partial derivative df/dc_y in Cartesian system
 *
 * Returns: 0 on success, -1 if output pointers are NULL
 */
int isometric_gradient_to_cartesian_(double df_dx_iso, double df_dy_iso, double *df_dx_cart, double *df_dy_cart)
{
    if (!df_dx_cart || !df_dy_cart) return -1;

    double inv_sqrt3 = 1.0 / sqrt(3.0);

    *df_dx_cart = inv_sqrt3 * df_dx_iso - inv_sqrt3 * df_dy_iso;
    *df_dy_cart = df_dx_iso + df_dy_iso;

    return 0;
}

int isometric_vector_to_cartesian(double dx_iso, double dy_iso, double *dx_cart, double *dy_cart)
{
    if (!dx_cart || !dy_cart) return -1;

    double sqrt3_over_2 = sqrt(3.0) / 2.0;
    double half = 0.5;

    *dx_cart = sqrt3_over_2 * (dx_iso - dy_iso);
    *dy_cart = half * (dx_iso + dy_iso);

    return 0;
}

#define ONE_OVER_SQRT3 (1.0 / sqrt(3.0))  // ≈ 0.5773502691896257

/*
 * Convert a vector from Cartesian to Isometric coordinates.
 * This transforms displacement-like vectors (contravariant components)
 * from Cartesian (dx_cart, dy_cart) to isometric components (dx_iso, dy_iso).
 *
 * Inputs:
 *   dx_cart - Vector component along Cartesian x-axis
 *   dy_cart - Vector component along Cartesian y-axis
 *
 * Outputs:
 *   dx_iso  - Component along isometric x-axis
 *   dy_iso  - Component along isometric y-axis
 *
 * Returns: 0 on success, -1 if output pointers are NULL
 */
int cartesian_vector_to_isometric(
        double dx_cart, double dy_cart,
        double *dx_iso, double *dy_iso)
{
    if (!dx_iso || !dy_iso) return -1;

    // Inverse transformation matrix for vectors:
    // dx_iso = (1/√3) * dx_cart + dy_cart
    // dy_iso = -(1/√3) * dx_cart + dy_cart
    *dx_iso = ONE_OVER_SQRT3 * dx_cart + dy_cart;
    *dy_iso = -ONE_OVER_SQRT3 * dx_cart + dy_cart;

    return 0;
}

static void make_array_rect (void)
/*------------------------------*/
{
  double df_dx, df_dy ;
  int i, j ;
  long l_block_size ;
  double angle_l, sina, cosa ;
  double df_dx1, df_dy1;
  int ret;

  angle_l=get_angle_l();
  if (angle_l!=0)
  {
  sina=get_sina();
  cosa=get_cosa();
  }

  for (i = 0, df_dy = 0 ;
       i < s_array_rect.rows_no;
       i++, df_dy += s_array_rect.df_dist_y )
  {
    for (j = 0, df_dx = ((i == 0) ? s_array_rect.df_dist_x : 0) ;
	 j < ((i == 0) ? s_array_rect.columns_no -1 : s_array_rect.columns_no) ;
	 j++, df_dx += s_array_rect.df_dist_x )
     {
       if (angle_l!=0)
       {
           if (options1.uklad_izometryczny)
           {
               ret = isometric_vector_to_cartesian(df_dx, df_dy, &df_dx1, &df_dy1);
           }
           else
           {
               df_dx1 = (df_dx * cosa) - (df_dy * sina);
               df_dy1 = (df_dx * sina) + (df_dy * cosa);
           }
       }
       else
        {
        df_dx1 = df_dx;
        df_dy1 = df_dy;
        }
       if (FALSE == Add_Block (ADP, ADK, &l_block_size, df_dx1, df_dy1, 0, 0, 0, 0, 0, 0, Tprzesuw))
       {
	 return ;
       }
     }
  }
}


static BOOL get_array_rect_size (void)
/*-----------------------------*/
{
  char st [MaxTextLen] ;
  int retval_no = 2 ;
  double buf_ret [2] ;

aa:
  retval_no = 2 ;
  st [0] = '\0' ;
  if (!get_string (st, "", MaxTextLen - 1 , 0, 47))
  {
    return FALSE ;
  }
  if (0 == calculator (st, &retval_no, buf_ret))
  {
    if (retval_no > 0)
    {
      ErrList (75) ;
    }
    Get_Key () ;
    goto aa ;
  }
  if (retval_no < 2 || buf_ret [0] < 1 || buf_ret [1] < 1)
  {
    ErrList (75) ;
    Get_Key () ;
    goto aa ;
  }
  s_array_rect.rows_no = (int)buf_ret [0] ;
  s_array_rect.columns_no = (int)buf_ret [1] ;
  return TRUE ;
}

static void cur_offo (double x, double y)
{ flip_screen();
    }


static void cur_offo__ (double x, double y)
{ double angle_l;
  LINIA L,L1;
  int temp_dir;

  cursel_off (x, y) ;
  angle_l=get_angle_l();
  if (angle_l==0)
  {
  outokno (&s_window, 0, 0) ;
  }
  else
  {
  temp_dir = Orto_Dir;

  L.x1=s_window.x1;
  L.y1=s_window.y1;
  L.x2=s_window.x2;
  L.y2=s_window.y2;

  Orto_Dir=I_Orto_XDir;
  orto_l(&L,&Orto_Dir);
  L1.x1=L.x2;
  L1.y1=L.y2;

  L.x2=s_window.x2;
  L.y2=s_window.y2;

  Orto_Dir=I_Orto_YDir;
  orto_l(&L,&Orto_Dir);
  L1.x2=L.x2;
  L1.y2=L.y2;

  Orto_Dir=temp_dir;

  outokno_a(&s_window,&L1) ;
  }

}

static void cur_ono (double x, double y)
{
  double df_x, df_y ;
  int temp_dir;
  double angle_l;
  LINIA L, L1;
  double sina, cosa, dxl, dyl;
  double df_x1, df_y1;

  s_window.x2 = x ;
  s_window.y2 = y ;
  angle_l=get_angle_l();
  if (angle_l==0)
  {
  outokno (&s_window, 0, 0) ;
  }
  else
  {
  sina=get_sina();
  cosa=get_cosa();
  temp_dir = Orto_Dir;

  L.x1=s_window.x1;
  L.y1=s_window.y1;
  L.x2=s_window.x2;
  L.y2=s_window.y2;

  Orto_Dir=I_Orto_XDir;
  orto_l(&L,&Orto_Dir);
  L1.x1=L.x2;
  L1.y1=L.y2;

  L.x2=s_window.x2;
  L.y2=s_window.y2;

  Orto_Dir=I_Orto_YDir;
  orto_l(&L,&Orto_Dir);
  L1.x2=L.x2;
  L1.y2=L.y2;

  Orto_Dir=temp_dir;

  outokno_a(&s_window,&L1) ;
  }
  cursel_on (x, y) ;

  df_x1 = milimetryob (fabs (s_window.x2 - s_window.x1)) ;
  df_y1 = milimetryob (fabs (s_window.y2 - s_window.y1)) ;

  if (angle_l==0)
  {
   df_x = df_x1 ;
   df_y = df_y1 ;
  }
  else
   {
    dxl = milimetryob (L1.x1 - s_window.x1);
    dyl = milimetryob (L1.y1 - s_window.y1); 
    df_x = sqrt((dxl * dxl) + (dyl * dyl));
    dxl = milimetryob (L1.x1 - s_window.x2);
    dyl = milimetryob (L1.y1 - s_window.y2); 
    df_y = sqrt((dxl * dxl) + (dyl * dyl));
   }
  sprintf (eL.st, "%#8.3lf;%#8.3lf", df_x, df_y) ;
  Out_Edited_Draw_Param ((ESTR *)&eL, TRUE) ;
}



static int edit_distance_value (BOOL b_graph_value)
/*-----------------------------------*/
{
  int wsx, wsy ;

  b_graph_value = b_graph_value ;
  if (eL.val_no < 2)
  {
    return 0 ;
  }
  wsx = (s_window.x2 - s_window.x1 > 0) ? 1 : -1 ;
  wsy = (s_window.y2 - s_window.y1 > 0) ? 1 : -1 ;
  s_array_rect.df_dist_x = jednostkiOb (wsx * eL.values [0]) ;
  s_array_rect.df_dist_y = jednostkiOb (wsy * eL.values [1]) ;
  strwyj = 1 ;
  return 1 ;
}


static BOOL get_unit_cell (void)
/*------------------------------*/
{
  EVENT *ev;
  double df_x, df_y ;

  strwyj = 0 ;
  while(1)
  {
     ev = Get_Event_Point (NULL, &df_x, &df_y) ;
     if(ev->What == evKeyDown || strwyj == 1)
     {
       if (ev->Number == 0)
       {
	 return FALSE ;
       }
       else
       if (ev->Number == ENTER || strwyj == 1)
       {
	 return TRUE ;
       }
     }
   }
}


static void redcrARP (int type)
/*---------------------------*/
{
  static void  (*CUR_oN)(double ,double);
  static void  (*CUR_oFF)(double ,double);
  double angle_l;
  double df_x1, df_y1;
  double sina, cosa;
  int ret;

  switch (type)
  {
    case 0 :
      eL.x = maxX / 2 + 1; /* - PL266 ;*/
      eL.y = ESTR_Y;
      eL.val_no_max = 2 ;
      eL.lmax = 18 ;
      eL.mode = GV_VECTOR_XY ;
      eL.format = "\0%#8.3lf;%#8.3lf" ;
      eL.ESTRF = edit_distance_value ;
	  eL.extend = 0;
      break ;
    case 1 :
      komunikat0 (75) ;
      CUR_OFF (X,Y) ;
      sel.akt = 1 ;
      sel.cur = 0 ;
      CUR_oFF = CUR_OFF ; CUR_OFF = cursel_off ;
      CUR_oN = CUR_ON ;   CUR_ON = cursel_on ;
      CUR_ON (X, Y) ;
      break ;
    case 2 :
      blokzap (ADP, ADK, Ablok, XOR_PUT, 1) ;
      CUR_OFF (X, Y) ;
      return ;
    case 3 :
      CUR_OFF (X, Y) ;
      np = dodajstr (&eL) ;
      s_window.x1 = s_window.x2 = X;
      s_window.y1 = s_window.y2 = Y;
      CUR_oFF = CUR_OFF ; CUR_OFF = cur_offo ;
      CUR_oN = CUR_ON ;   CUR_ON = cur_ono ;
      CUR_ON (X,Y) ;
      break ;
    case 4 :
      usunstr (np) ;
      break ;
    case 5 :
      usunstr (np) ;
      if (strwyj == 0)
      {
        df_x1 = s_window.x2 - s_window.x1 ;
        df_y1 = s_window.y2 - s_window.y1 ;
        angle_l=get_angle_l();
        if (angle_l==0)
        {
         s_array_rect.df_dist_x = df_x1 ;
         s_array_rect.df_dist_y = df_y1 ;
        }
        else
         {
            if (options1.uklad_izometryczny)
            {
                ret= cartesian_vector_to_isometric(df_x1, df_y1, &s_array_rect.df_dist_x, &s_array_rect.df_dist_y);
            }
            else
            {
                sina = get_sina();
                cosa = get_cosa();

                s_array_rect.df_dist_x = (df_x1 * cosa) + (df_y1 * sina);
                s_array_rect.df_dist_y = (-df_x1 * sina) + (df_y1 * cosa);
            }
         }
      }
      CUR_OFF (X, Y) ;
      blokzap (ADP, ADK, Ablok, XOR_PUT, 1) ;
      break ;
    case 6 :
      blokzap (ADP, ADK, Ablok, XOR_PUT, 1) ;
      CUR_OFF (X, Y) ;
      break ;
  }
}


void Array_Rect_Proc (void)
/*------------------------*/
{

  if (FALSE == get_array_rect_size ())
  {
    redcrARP (6) ;
    return ;
  }
  redcrARP (0) ;
aa:
  redcrARP (1) ;
  if (FALSE ==get_unit_cell ())
  {
    redcrARP (2) ;
    return ;
  }
  redcrARP (3) ;
  
  if (FALSE ==get_unit_cell ())
  {
    redcrARP (4) ;
	
    goto aa ;
  }
  redcrARP (5) ;
  make_array_rect () ;
  
}

/*--------------------------------------------------------------*/

void  set_items_no (void)
/*--------------------------*/
{
  char sk [MaxTextLen] = "", *str ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  int ret;

  if (EOF == sprintf (sk, "%-10d", s_array_polar.items_no))
  {
    strcpy (sk, "");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  if (!get_string (sk, "", MaxTextLen, 0, 21)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  if ( 1 > buf_ret [0] ||
       MAX_ITEMS_NO < buf_ret [0])
  {
    ErrList (9) ;
    return ;
  }
  s_array_polar.items_no = (int)buf_ret [0] ;
  ret = snprintf(sk, 10, "%d",s_array_polar.items_no);
  sprintf (e_items_no.st, "%#6d", s_array_polar.items_no) ;
  Out_Edited_Draw_Param ((ESTR *)&e_items_no, TRUE) ;
  menu_par_new ((*mArray_polar.pola)[0].txt, sk) ;
  menu_par_new((*mArray_polar_rot.pola)[0].txt, sk);
}

void  set_angle_to_fill (void)
/*------------------------------*/
{
  char sk [MaxTextLen] = "", *str ;
  int retval_no = 1 ;
  double buf_ret [1] ;

  if (EOF == sprintf (sk, "%-6.2lf", s_array_polar.df_angle_to_fill))
  {
    strcpy (sk, "");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  if (!get_string (sk, "", MaxTextLen, 0, 20)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  if (buf_ret [0] == 0 ||
      fabs (buf_ret [0]) > 360)
  {
    ErrList (3) ;
    return ;
  }
  s_array_polar.df_angle_to_fill = buf_ret [0] ;
  sprintf (e_angle_to_file.st, "%#7.2lf", s_array_polar.df_angle_to_fill) ;
  Out_Edited_Draw_Param ((ESTR *)&e_angle_to_file, TRUE) ;
  menu_par_new ((*mArray_polar.pola)[1].txt, e_angle_to_file.st) ;
  menu_par_new((*mArray_polar_rot.pola)[1].txt, e_angle_to_file.st);
}

static void make_array_polar (void)
/*-----------------------------------*/
{
  double df_cos, df_sin, df_dangle, df_angle ;
  double df_xcp, df_ycp, df_xbp, df_ybp, df_xbp1, df_ybp1 ;
  int i ;
  long l_block_size ;

  df_xbp = s_array_polar.df_xbp ;
  df_ybp = s_array_polar.df_ybp ;
  df_xcp = s_array_polar.df_xcp ;
  df_ycp = s_array_polar.df_ycp ;
  df_dangle = Grid_to_Rad (s_array_polar.df_angle_to_fill) / s_array_polar.items_no ;
  for (i = 0, df_angle = df_dangle ;
       i < s_array_polar.items_no - 1 ;
       i++, df_angle += df_dangle)
  {
    df_cos = cos (df_angle) ;
    df_sin = sin (df_angle) ;

    Rotate_Point (df_sin,   df_cos,
		  df_xcp,   df_ycp,
		  df_xbp,   df_ybp,
		 &df_xbp1, &df_ybp1) ;
    if (FALSE == Add_Block (ADP, ADK, &l_block_size, df_xbp1 - df_xbp, df_ybp1 - df_ybp, 0, 0,
		0, 0, 0, 0, Tprzesuw))
   {
     return ;
   }
  }
}

static void make_array_polar_rot (void)
/*-----------------------------------*/
{
  double df_cos, df_sin, df_dangle, df_angle, df_xcp, df_ycp ;
  int i ;
  long l_block_size ;

  df_xcp = s_array_polar.df_xcp ;
  df_ycp = s_array_polar.df_ycp ;
  df_dangle = Grid_to_Rad (s_array_polar.df_angle_to_fill) / s_array_polar.items_no ;
  for (i = 0, df_angle = df_dangle ;
       i < s_array_polar.items_no - 1 ;
       i++, df_angle += df_dangle)
  {
    df_cos = cos (df_angle) ;
    df_sin = sin (df_angle) ;
    if (FALSE == Add_Block (ADP, ADK, &l_block_size, df_xcp, df_ycp, df_sin, df_cos, 0, 0, 0, 0, Tobrot))
    {
      return ;
    }
  }
}

static BOOL array_polar_center_point (double *df_x, double *df_y)
/*-----------------------------------------*/
{
  EVENT *ev;

  while(1)
  {
     ev = Get_Event_Point (NULL, df_x, df_y) ;
     if(ev->What == evKeyDown)
     {
       if (ev->Number == 0)
       {
	 return FALSE ;
       }
       else
       if (ev->Number == ENTER)
       {
	 out_krz(*df_x, *df_y) ;
	 return TRUE ;
       }
     }
     else
     if (ev->What == evCommandP)
     {
       switch (ev->Number)
       {
	 case 0 :
	    set_items_no () ;
	    break ;
	 case 1 :
	    set_angle_to_fill () ;
	    break ;
	 default :
	    break ;
       }
     }
   }
}

static int edit_items_no (BOOL b_graph_value)
/*-----------------------------*/
{ int ret;
  char sk [MaxTextLen] = "" ;


  b_graph_value = b_graph_value ;
  if (e_items_no.val_no < 1 ||
       1 > e_items_no.values [0] ||
       MAX_ITEMS_NO < e_items_no.values [0] )
  {
    ErrList (9) ;
    return 0 ;
  }
  s_array_polar.items_no = (int)e_items_no.values [0] ;
  ret = snprintf(sk, 10, "%d",s_array_polar.items_no);
  menu_par_new ((*mArray_polar.pola)[0].txt, sk) ;
  menu_par_new((*mArray_polar_rot.pola)[0].txt, sk);
  return 1 ;
}


static int edit_angle_to_fill (BOOL b_graph_value)
/*--------------------------------*/
{
  char sk [MaxTextLen] = "", *str ;


  b_graph_value = b_graph_value ;
  if (e_angle_to_file.val_no < 1 ||
      fabs (e_angle_to_file.values [0]) > 360)
  {
    ErrList (3) ;
    return 0 ;
  }
  s_array_polar.df_angle_to_fill  = e_angle_to_file.values [0] ;
  sprintf (sk, "%-9.3lf", s_array_polar.df_angle_to_fill) ;
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  menu_par_new ((*mArray_polar.pola)[1].txt, sk) ;
  menu_par_new((*mArray_polar_rot.pola)[1].txt, sk);
  return 1 ;
}

static void redcrAPP (int type, BOOL b_rot)
/*---------------------------------------*/
{
  static void  (*CUR_oN)(double ,double);
  static void  (*CUR_oFF)(double ,double);
  static int np_items_no, np_angle_to_file ;

  switch (type)
  {
    case 0 :
      CUR_OFF (X,Y) ;
      sel.akt = 1 ;
      sel.cur = 0 ;
      CUR_oFF = CUR_OFF ; CUR_OFF = cursel_off ;
      CUR_oN = CUR_ON ;   CUR_ON = cursel_on ;
	  if (b_rot)
	  {
		  menupini(&mArray_polar_rot, _ARRAY_WITH_ROTATION_, _ARRAY_C_, 118);
	  }
	  else
	  {
		  menupini(&mArray_polar, _ARRAY_, _ARRAY_C_, 117);
	  }

      e_angle_to_file.x = maxX/2+ 4 * WIDTH;
      e_angle_to_file.y= ESTR_Y;
      e_angle_to_file.lmax = 14 ;
      e_angle_to_file.mode = GV_ANGLE ;
      e_angle_to_file.format = "%#8.3lf" ;
      e_angle_to_file.ESTRF = edit_angle_to_fill ;
	  e_angle_to_file.extend = 0;
      e_angle_to_file.val_no_max = 1 ;
      if (-1 != (np_angle_to_file = dodajstr(&e_angle_to_file)))
      {
	    sprintf (e_angle_to_file.st, "%#8.3lf", s_array_polar.df_angle_to_fill) ;
      }

      e_items_no.x = maxX / 2 - 10*WIDTH ;
      e_items_no.y = ESTR_Y;
      e_items_no.lmax = 8 ;
      e_items_no.mode = GV_NUMBER ;
      e_items_no.format = "\01%#6d" ;
      e_items_no.ESTRF = edit_items_no ;
      e_items_no.extend = 0;
      e_items_no.val_no_max = 1 ;
      if (-1 != (np_items_no = dodajstr(&e_items_no)))
      {
          sprintf (e_items_no.st, "%#6d", s_array_polar.items_no) ;
      }

      CUR_ON (X, Y) ;
      break ;
    case 1 :
      komunikat0 (77) ;  /*komunikat niszczy pole edycji liczby elementow*/
      Out_Edited_Draw_Param ((ESTR *)&e_items_no, TRUE) ;
      Out_Edited_Draw_Param ((ESTR *)&e_angle_to_file, TRUE) ;
      break ;
    case 2 :
    case 4 :
    case 5 :
      usunstr (np_items_no) ;
      usunstr (np_angle_to_file) ;
      menupini (NULL, "", ' ', 0) ;
      blokzap (ADP, ADK, Ablok, XOR_PUT, 1) ;
      CUR_OFF (X, Y) ;
      break ;
    case 3 :
      komunikat0 (76) ;	/*komunikat niszczy pole edycji liczby elementow*/
      Out_Edited_Draw_Param ((ESTR *)&e_items_no, TRUE) ;
      Out_Edited_Draw_Param ((ESTR *)&e_angle_to_file, TRUE) ;
      break ;
    case 6 :
      break ;
  }
}


void Array_Polar_Proc (BOOL b_rot)
/*--------------------------------*/
{
  double df_x, df_y ;

  redcrAPP (0, b_rot) ;
  if (FALSE == b_rot)
  {
aa:  redcrAPP (1, b_rot) ;
    if (FALSE == array_polar_center_point (&df_x, &df_y))
    {
      redcrAPP (2, b_rot) ;
      return ;
    }
    s_array_polar.df_xbp = df_x ;
    s_array_polar.df_ybp = df_y ;
  }
  redcrAPP (3, b_rot) ;
  if (FALSE == array_polar_center_point (&df_x, &df_y))
  {
    if (FALSE == b_rot)
    {
      goto aa ;
    }
    redcrAPP (4, b_rot) ;
    return ;
  }
  s_array_polar.df_xcp = df_x ;
  s_array_polar.df_ycp = df_y ;
  redcrAPP (5, b_rot) ;
  if (TRUE == b_rot)
  {
    make_array_polar_rot () ;
  }
  else
  {
    make_array_polar () ;
  }
  redcrAPP (6, b_rot) ;
}

#undef __O_ARRAY__