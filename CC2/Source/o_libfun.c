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

#define __O_LIBFUN__
#include <forwin.h>
#include <stdlib.h>
#ifndef LINUX
#include<process.h>
#else

#endif
#define ALLEGWIN
#include <allegext.h>

#include <math.h>
#include <time.h>

#include <limits.h>
#include <string.h>

#include <stdio.h>
#include <float.h>
#ifndef LINUX
#include <dos.h>
#include <io.h>
#endif
#include<fcntl.h>
#include <ctype.h>

#include<sys/stat.h>
#include<signal.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "o_loadf.h"
#include "o_protec.h"
#include "o_libfun.h"
#include "o_graphf.h"
#include "o_lfile.h"


#define LINE myline

#include "noactive.h"

#include "menu.h"

#define PREC_ANGLE  1.0E-3 //1.0E-2
#define PREC_FLOAT  1.0E-5
#define PREC_DOUBLE  1.0E-10
#define PREC_1  1.0E-2 //1.0E-1
#define PREC_2  1.0E-3 //1.0E-2
#define PREC_3  1.0E-4 //1.0E-3
#define PREC_4  1.0E-4

extern void Set_Buf_Mak_Size (unsigned long size) ;
extern void Set_Memory_Size (unsigned long size) ;
extern void Set_Buf_PCX (unsigned long size);
extern void Set_Mouse_Buttons_No (int mb_no) ;
extern void Test_przycisniec(int *keys);
extern void Odczyt_licznikow(void);
extern int  kbrdy(void);
extern void myline(int x1, int y1, int x2, int y2);
extern int getgraphmode(void);
extern void getviewsettings(struct viewporttype  *viewport);
extern void setviewport(int left, int top, int right, int bottom, int clip);
extern void Hide_Mouse (void);
extern void getimage(int left, int top, int right, int bottom, void  *bitmap);
extern void setcolor(int kolor);
extern void setwritemode( int mode );
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);
extern void setrgbdefaults(void);
extern void Show_Mouse (void);
extern int getchp(void);
extern void putimage(int left, int top,  void  *bitmap, int op);
extern void restorecrtmode(void);
extern void setgraphmode(int mode);
extern int Save_Screen(void);
extern int getdisk(void);
extern int setdisk(int _drive);
extern int Restore_Screen(void);
extern int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext);
extern void fnmerge (char *path, const char *drive, const char *dir, const char *name, const char *ext);
extern int BIGCURSOR;

extern void utf8Upper(char* text);
extern int my_getch(void);

static double get_little_val (double df_lv, double df_x, double df_y) ;
static struct viewporttype view_old;
static int Mode0;
extern char default_path_TTF[MAXPATH];
extern char default_path_OTF[MAXPATH];

extern void Set_Dist_Free_mouse(int distx, int disty);
extern void Set_Mouse_Speed(float m_speed);
extern void getcolor_RGB(int *red, int *green, int *blue, int color);
extern int cursor_color0;
extern BOOL BAR_POINTER;

extern void set_sleep_state(BOOL state);
extern void Set_Mouse_Wheel(int val_int);

int check_dim_line(LINIA *L) {
//checking if line is dimensioning line
    if (L->blok == 0) return 0;
    if (L->obiektt1 != 0) return 0;
    if (L->obiektt2 != 1) return 0;
    if (L->obiektt3 != 0) return 0;
    return 1;
}

double Atan2 (double y, double x)
/*------------------------------*/
{
  if (fabs(y) < 1.0E-10 && fabs (x) < 1.0E-10)
  {
/*   Internal_Error (__LINE__,__FILE__) ;*/
   return 0 ;
  }
  return atan2 (y, x) ;
}


void Truncate_Spacing (T_Fstring buf)
/*---------------------------------*/
{
  int i, len;
  T_Fstring s;

  if (buf == NULL)
  {
    return ;
  }
  len = strlen (buf) ;
  if (len == 0)
  {
    return;
  }
  s = buf + len - 1;
  for ( i = 0; i < len; i++, s--)
    if (*s != ' ' && *s != '\t') 	/*'\t' tabulacja*/
      break;
  *++s = '\0';
}

T_Fstring Ignore_Spacing (T_Fstring buf)
/*--------------------------------------*/
{
  T_Fstring s;

  if (buf == NULL)
  {
    return buf ;
  }
  s = buf;
  while (1)
  {
    if (*s == ' ' || *s == '\t')
    {
      s++;
      continue;
    }
    return s;
  }
}


int Menu_No_to_Color_No (int i_menu_poz)
/*-----------------------------------*/
{
  int i_color ;

  i_color = i_menu_poz + 1 ;
  if (i_menu_poz == 15)
  {
    i_color = 0 ;
  }
  return i_color ;
}

int Color_No_to_Menu_No (int i_color)
/*---------------------------------*/
{
  int i_menu_poz ;

  i_menu_poz = i_color - 1 ;
  if (i_color == 0)
  {
    i_menu_poz = 15 ;
  }
  return i_menu_poz ;
}

int Color_to_DXF (int i_color)
/*---------------------------*/
{
  int i_acad_no ;

  i_acad_no = i_color ;
  if (i_color == 0)
  {
    i_acad_no = 15 ;
  }
  return i_acad_no ;
}

void Get_Menu_Param (int i_pos, int i_maxw, int i_max,
			    int *ptri_foff, int *ptri_poz)
/*------------------------------------------------------*/
{
  *ptri_foff = 0 ;
  *ptri_poz = i_pos ;
  if (i_maxw > 0)
  {
    *ptri_foff = i_pos - i_pos % i_maxw ;
    if (*ptri_foff + i_maxw > i_max)
    {
      *ptri_foff = i_max - i_maxw ;
    }
    *ptri_poz = i_pos - *ptri_foff ;
  }
}


void menu_par_new (char  *pole, const char *par)
/*------------------------------------------------*/
{ char *st;
  //int len_s;
  //char param_[60];
  //int i;
  int n;

  st=pole + strlen(pole) + 1;

 if ((strlen(pole)+1+strlen(par))<POLE_TXT_MAX)
     strcpy(st, par);
 else
 {
     n = POLE_TXT_MAX - strlen(pole - 2);
     if (n>0) strncpy(st, par, n);
 }
}

void return_menu_par (char *pole, char * par)
/*----------------------------------------*/
{ char *st;
  st=pole + strlen(pole) + 1;
  strcpy(par,st);
}

void return_menu_par0 (char *pole, char * par)
/*-----------------------------------------*/
{ char *st;
  st=pole;
  strcpy(par,st);
}

void obru(double si,double co, double xp, double yp, double *xn,double *yn)
/*------------------------------------------------*/
{ *xn=+xp*co+yp*si;  	/*clockwise*/
  *yn=-xp*si+yp*co;
}

void obrd(double si,double co, double xp, double yp, double *xn, double *yn)
/*------------------------------------------------------------------------*/
{ *xn=+xp*co-yp*si;	/*counterclockwise*/
  *yn= xp*si+yp*co;
}


void DF_to_String (char *ptrsz_buf, char *ptrsz_format, double df_val, int max_size)
/*----------------------------------------------------------------------------------*/
{
  char *str ;
  int len ;

  if (EOF == sprintf (ptrsz_buf, ptrsz_format, df_val))
  {
    ptrsz_buf [0] = '\0';
  }
  if (NULL != (str = strchr (ptrsz_buf, ' ')))
  {
    str [0] = '\0';
  }
  if (NULL != strchr (ptrsz_buf, '.')  &&
      NULL == strchr (ptrsz_buf, 'e') &&
      NULL == strchr (ptrsz_buf, 'E'))
  {
    len = strlen (ptrsz_buf) ;
    while (ptrsz_buf [len - 1] == '0')
    {
      ptrsz_buf [--len] = '\0' ;
    }
    if (ptrsz_buf [len - 1] == '.')
    {
       ptrsz_buf [len - 1] = '\0' ;
    }
  }
  if (max_size > 0 && strlen (ptrsz_buf) > max_size)
  {
    ptrsz_buf [max_size] = '\0' ;
  }
  return;
}


void Rotate_Point (double si, double co,
		double x1, double y1,  			    /*center point*/
		double x2, double y2, double *x, double *y) /*rotate point*/
/*-------------------------------------------------------------------------------------*/
{
  *x = x1 + (x2 - x1) * co - (y2 - y1) * si ;
  *y = y1 + (x2 - x1) * si + (y2 - y1) * co ;
}


void Change_Global_Ptr (long n)
/*----------------------------*/
{
  if (ADP != NULL)
  {
    ADP += n ;
    l__Off_Bl_Beg = (long)(ADP - dane ) ;
  }
  if (ADK != NULL)
  {
    ADK += n ;
    l__Off_Bl_End = (long)(ADK - dane ) ;
  }
  if (PTR__GTMP1 != NULL)
  {
    PTR__GTMP1 += n ;
  }
  if (PTR__GTMP2 != NULL)
  {
    PTR__GTMP2 += n ;
  }
  if (PTR__GTMP3 != NULL)
  {
    PTR__GTMP3 += n ;
  }
  if (PTR__GTMP4 != NULL)
  {
    PTR__GTMP4 += n ;
  }
  if (PTR__GTMP5 != NULL)
  {
    PTR__GTMP5 += n ;
  }
  if (PTR__GTMP6 != NULL)
  {
    PTR__GTMP6 += n ;
  }
  if (PTR__GTMP7 != NULL)
  {
    PTR__GTMP7 += n ;
  }
  if (PTR__GTMPBLOCK != NULL)
  {
    PTR__GTMPBLOCK += n ;
  }
  if (PTR__GTMPFIRST != NULL)
  {
    PTR__GTMPFIRST += n ;
  }
  if (PTR__GTMPLAST != NULL)
  {
    PTR__GTMPLAST += n ;
  }
}


void Shift_Global_Ptr (long_long n)
/*--------------------------------*/
{
    if (ADP != NULL)
    {
        ADP += n ;
        l__Off_Bl_Beg = (long)(ADP - dane ) ;
    }
    if (ADK != NULL)
    {
        ADK += n ;
        l__Off_Bl_End = (long)(ADK - dane ) ;
    }
    if (PTR__GTMP1 != NULL)
    {
        PTR__GTMP1 += n ;
    }
    if (PTR__GTMP2 != NULL)
    {
        PTR__GTMP2 += n ;
    }
    if (PTR__GTMP3 != NULL)
    {
        PTR__GTMP3 += n ;
    }
    if (PTR__GTMP4 != NULL)
    {
        PTR__GTMP4 += n ;
    }
    if (PTR__GTMP5 != NULL)
    {
        PTR__GTMP5 += n ;
    }
    if (PTR__GTMP6 != NULL)
    {
        PTR__GTMP6 += n ;
    }
    if (PTR__GTMP7 != NULL)
    {
        PTR__GTMP7 += n ;
    }
    if (PTR__GTMPBLOCK != NULL)
    {
        PTR__GTMPBLOCK += n ;
    }
    if (PTR__GTMPFIRST != NULL)
    {
        PTR__GTMPFIRST += n ;
    }
    if (PTR__GTMPLAST != NULL)
    {
        PTR__GTMPLAST += n ;
    }
}

char *Get_PTR__GTMPBLOCK(void)
{
    return PTR__GTMPBLOCK;
}

void Set_PTR__GTMPBLOCK(char *block)
{
    PTR__GTMPBLOCK=block;
}

char *Get_PTR__GTMPFIRST(void)
{
    return PTR__GTMPFIRST;
}

void Set_PTR__GTMPFIRST(char *first)
{
    PTR__GTMPFIRST=first;
}

char *Get_PTR__GTMPLAST(void)
{
    return PTR__GTMPLAST;
}

void Set_PTR__GTMPLAST(char *last)
{
    PTR__GTMPLAST=last;
}


double Angle_Normal (double angle)
/*-------------------------------*/
{
  if (fabs (angle) >= Pi2 + o_male)
  {
    angle -= floor (0.5 + angle / Pi2) * Pi2 ;
  }
  if(angle < 0)
  {
    angle += Pi2;
  }
  ////if (TRUE == Check_if_Equal (angle, Pi2))
  ////{
  ////  angle = 0 ;
  ////}
  return angle;
}

double Angle_Simple (double angle)
{
    double angle1=Angle_Normal (angle);
    if (angle1>=Pi) angle1-=Pi;
    return angle1;
}

double Double_Normal (double param)
/*-------------------------------*/
{
    if (fabs (param) <= o_male_male) param = 0.0;
    return param;
}

double Angle_Normal_Grid (double angle)
/*------------------------------------*/
{
   return 360 * Angle_Normal (angle * Pi2 / 360.0) / Pi2 ;
}

double Grid_to_Rad (double angle)
/*------------------------------*/
{
  angle  = angle * Pi2 / 360.0 ;
  return angle ;
}

double Rad_to_Grid (double angle)
/*------------------------------*/
{
  angle  = angle * 360.0 / Pi2 ;
  return angle ;
}

static double get_little_val (double df_lv, double df_x, double df_y)
/*-----------------------------------------------------------------*/
{
  double df_dd ;

  df_dd = fabs (df_x / df_lv) ;
  if (df_dd == 0)
  {
    df_dd = fabs (df_y / df_lv) ;
    if (df_dd == 0)
    {
      df_dd = 1 / df_lv;
    }
  }
  if (df_dd < 1 / df_lv)
  {
    df_dd = 1 / df_lv;
  }
  return df_dd ;
}


double Get_Prec (int i_prec, double df_x, double df_y)
/*---------------------------------------------------*/
{
  double df_prec ;

  switch (i_prec)
  {
    case MPREC_ANGLE :
      df_prec = PREC_ANGLE ;
      break ;
    case MPREC_FLOAT :
      df_prec = PREC_FLOAT ;
      break ;
    case MPREC_DOUBLE :
      df_prec = PREC_DOUBLE ;
      break ;
    case MPREC_1 :
      df_prec = PREC_1 ;
      break ;
    case MPREC_2 :
      df_prec = PREC_2 ;
      break ;
    case MPREC_3 :
      df_prec = PREC_3 ;
      break ;
    case MPREC_4 :
      df_prec = PREC_4 ;
      break ;
    default :
      df_prec = PREC_FLOAT ;
      break ;
  }
  df_prec = get_little_val (1 / df_prec, df_x, df_y) ;
  return df_prec ;
}


BOOL
Check_Angle_Prec (double k, double k1, double k2, int i_prec)
/*----------------------------------------------------------*/
{
  BOOL ret;
  double df_dangle ;

  ret = FALSE;
  df_dangle = Get_Prec (i_prec, 1, 1) ;
  k = Angle_Normal (k);
  k1 = Angle_Normal (k1);
  k2 = Angle_Normal (k2);
  if (fabs (k - Pi2) < df_dangle)
    k = 0 ;
  if (fabs (k1 - Pi2) < df_dangle)
    k1 = 0 ;
  if (fabs (k2 - Pi2) < df_dangle)
    k2 = 0 ;
  if (fabs (k1 - k2) < df_dangle)
  {
    if (fabs (k - k1) < df_dangle)
    {
      return TRUE ;
    }
    else
    {
      return FALSE ;
    }
  }
  if (k1 <= k2)  /*gdy k1 == k2 (w przblizeniu)  to k1 moze byc > k2 np. na 15 miejscu po przecinku*/
  {
    if (k >=  (k1 - df_dangle) && k <= (k2 + df_dangle))
    {
      ret = TRUE;
    }
  }
  else
  {
    if ( k >=  (k1 - df_dangle)  ||  k <=  (k2 + df_dangle) )
    {
      ret = TRUE;
    }
  }
  return ret;
}

BOOL Check_Angle (double k, double k1, double k2)
/*-------------------------------------------------*/
{
  return Check_Angle_Prec (k, k1, k2, MPREC_ANGLE) ;  //MPREC_FLOAT
}

double Get_Angle_Difference (double df_angle1, double df_angle2)
/*-------------------------------------------------------------*/
{
    double df_diff ;

    df_diff = Angle_Normal (df_angle2 - df_angle1) ;
    if (df_diff > Pi2 / 2)
    {
      df_diff = Pi2 - df_diff ;
    }
    return df_diff ;
}


BOOL
Sharp_Check_Angle (double k, double k1, double k2)
/*-------------------------------------------------*/
{
  BOOL ret;
  double df_prec ;

  ret = FALSE;
  k = Angle_Normal (k);
  k1 = Angle_Normal (k1);
  k2 = Angle_Normal (k2);
  if (TRUE == Check_if_Equal (k1, k2))
  {
      return FALSE ;
  }
  df_prec = Get_Prec (MPREC_2, 1, 1) ;
  if (k1 <= k2)
  {
    if (k >= (k1 + df_prec) && k <= (k2 - df_prec))
    {
      ret = TRUE;
    }
  }
  else
  {
    if (k >= (k1 + df_prec)  ||  k <= (k2 - df_prec))
    {
      ret = TRUE;
    }
  }
  return ret;
}

BOOL Check_if_Equal0(double x, double y)
/*---------------------------------------*/
{
    BOOL retval;
    double dd;

    retval = FALSE;
    dd = get_little_val(DF_PRECISION, x, y);  ////WARNING    0.01
    if (fabs(x - y) < dd)
    {
        retval = TRUE;
    }
    return retval;
}


BOOL Check_if_Equal (double x, double y)
/*---------------------------------------*/
{
  BOOL retval ;
  double dd ;

  retval = FALSE ;
  dd = get_little_val (DF_PRECISION, x, y) ;
  if (fabs (x - y) < dd)
  {
    retval = TRUE ;
  }
  return retval ;
}

BOOL Check_if_Equal1(double x, double y)
/*---------------------------------------*/
{
    BOOL retval;
    double dd;

    retval = FALSE;
    dd = get_little_val(DF_PRECISION*0.1, x, y);  ////WARNING    0.1
    if (fabs(x - y) < dd)
    {
        retval = TRUE;
    }
    return retval;
}

BOOL Check_if_Equal2(double x, double y)
/*---------------------------------------*/
{
	BOOL retval;
	double dd;

	retval = FALSE;
	dd = get_little_val(DF_PRECISION*0.01, x, y);  ////WARNING    0.01
	if (fabs(x - y) < dd)
	{
		retval = TRUE;
	}
	return retval;
}

BOOL Check_if_Equal3(double x, double y)
/*---------------------------------------*/
{
    BOOL retval;
    double dd;

    retval = FALSE;
    dd = get_little_val(DF_PRECISION*0.001, x, y);
    if (fabs(x - y) < dd)
    {
        retval = TRUE;
    }
    return retval;
}



BOOL Check_if_GE (double x, double y)
/*----------------------------------*/
{
  BOOL retval ;

  retval = FALSE ;
  if (x >= y - get_little_val (DF_PRECISION, x, y))
  {
    retval = TRUE ;
  }
  return retval ;
}

BOOL Check_if_GE02(double x, double y)
/*----------------------------------*/
{
	BOOL retval;

	retval = FALSE;
	if (x >= y - get_little_val(DF_PRECISION*0.01, x, y))
	{
		retval = TRUE;
	}
	return retval;
}

BOOL Check_if_LE (double x, double y)
/*----------------------------------*/
{
  BOOL retval ;

  retval = FALSE ;
  if (x <= y + get_little_val (DF_PRECISION, x, y))
  {
    retval = TRUE ;
  }
  return retval ;
}

BOOL Check_if_LE2 (double x, double y)
/*----------------------------------*/
{
    BOOL retval ;

    retval = FALSE ;
    if (x <= y + get_little_val (DF_PRECISION*0.01, x, y))
    {
        retval = TRUE ;
    }
    return retval ;
}


BOOL Check_if_GT (double x, double y)
/*----------------------------------*/
{
  BOOL retval ;

  retval = FALSE ;
  if (x > y + get_little_val (DF_PRECISION, x, y))
  {
    retval = TRUE ;
  }
  return retval ;
}

BOOL Check_if_LT (double x, double y)
/*----------------------------------*/
{
  BOOL retval ;

  retval = FALSE ;
  if (x < y - get_little_val (DF_PRECISION, x, y))
  {
    retval = TRUE ;
  }
  return retval ;
}


double Double_to_Float (double x)
/*-----------------------------*/
{
  double retval ;

  retval = x ;
  if (x > FLT_MAX) retval = 1.0E+20 ;
  else
  if (x < -FLT_MAX) retval = - 1.0E+20 ;
  return retval;
}

static double const  long_max = LONG_MAX / 10 ;

long Double_to_Long (double x)
/*-----------------------------*/
{
  if (x > long_max)
    x = long_max ;
  else
  if (x < - long_max)
    x = - long_max ;
  return (long)x ;
}

static double const  int_max = INT_MAX - 10.0 ;
static double const  int_min = INT_MIN + 10.0 ;

int Double_to_Int (double x)
/*-------------------------*/
{
  if (x > int_max)
    x = int_max ;
  else
  if (x < int_min)
    x = int_min ;
  return (int)x ;
}


double Dist_PP (double x1, double y1, double x2, double y2)
/*-----------------------------------------------------*/
{
  return sqrt ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) ;
}


void  Quick_Exit (int errorno)
/*--------------------------*/
{ int k;
  ErrList (errorno) ;
  k=my_getch () ;
  exit(0) ;
}

/*------------------------------------------------------------------*/

void out_krz(double X, double Y)
{ int x0,y0;
  x0 = (int)pikseleX0(X);
  y0 = (int)pikseleY0(Y);
  setcolor(kolory.cur);
    setwritemode(COPY_PUT);
  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
  Set_Second_Screen();
  okno_r_second();
  LINE(x0-2,y0,x0+2,y0);
  LINE(x0,y0-2,x0,y0+2);
  okno_all_second();
  Set_Screen();
}

void out_krz_sc(double X, double Y)
{ int x0,y0;
    x0 = (int)pikseleX0(X);
    y0 = (int)pikseleY0(Y);
    setcolor(kolory.cur);
    setwritemode(COPY_PUT);
    setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
    okno_r();
    LINE(x0-2,y0,x0+2,y0);
    LINE(x0,y0-2,x0,y0+2);
    okno_all();
}

void out_krz_sc_p(int x0, int y0)
{
    setcolor(kolory.cur);
    setwritemode(COPY_PUT);
    setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
    okno_r();
    LINE(x0-2,y0,x0+2,y0);
    LINE(x0,y0-2,x0,y0+2);
    okno_all();
}

void outrect(OKNO *O,double dx,double dy)
{
    if (O == NULL )
    {
        Internal_Error (__LINE__,__FILE__);
        return;
    }
    setwritemode(COPY_PUT);
    linestyle(LiniaG.typ);
    if (LiniaG.kolor == 0) SetColorAC(15);
    else SetColorAC(LiniaG.kolor);
    okno_r();
    lineC ((int)pikseleX0(O->x1+dx),(int)pikseleY0(O->y1+dy),(int)pikseleX0(O->x2+dx),(int)pikseleY0(O->y1+dy));
    lineC ((int)pikseleX0(O->x1+dx),(int)pikseleY0(O->y1+dy),(int)pikseleX0(O->x1+dx),(int)pikseleY0(O->y2+dy));
    lineC ((int)pikseleX0(O->x2+dx),(int)pikseleY0(O->y1+dy),(int)pikseleX0(O->x2+dx),(int)pikseleY0(O->y2+dy));
    lineC ((int)pikseleX0(O->x1+dx),(int)pikseleY0(O->y2+dy),(int)pikseleX0(O->x2+dx),(int)pikseleY0(O->y2+dy));
    okno_all();
}

/*
static int shift_grey(int kolory_blok)
{  int ret;
    ret=((kolory_blok+5)%256);
    return ret;
}

static double ColourDistance(RGB e1, RGB e2)
{
    long rmean = ( (long)e1.r + (long)e2.r ) / 2;
    long r = (long)e1.r - (long)e2.r;
    long g = (long)e1.g - (long)e2.g;
    long b = (long)e1.b - (long)e2.b;
    return sqrt((((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8));
}
 */

void outokno(OKNO *O,double dx,double dy)
{
    ////RGB c1, c2;
    ////double cdistance;

  if (O == NULL )
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
    setwritemode(COPY_PUT);
  setlinestyle1(DOTTED_LINE,0,NORM_WIDTH);

  //determine colours similarity
  ////getcolor_RGB(&c1.r, &c1.g, &c1.b, kolory.paper);
  ////getcolor_RGB(&c2.r, &c2.g, &c2.b, kolory.blok);
  ////cdistance=(ColourDistance(c1, c2));
  ////if (cdistance > 1000)  setcolor(kolory.blok);
  ////else setcolor(shift_grey(kolory.blok));
  setcolor(kolory.blok);

  okno_r();
  lineC ((int)pikseleX0(O->x1+dx),(int)pikseleY0(O->y1+dy),(int)pikseleX0(O->x2+dx),(int)pikseleY0(O->y1+dy));
  lineC ((int)pikseleX0(O->x1+dx),(int)pikseleY0(O->y1+dy),(int)pikseleX0(O->x1+dx),(int)pikseleY0(O->y2+dy));
  lineC ((int)pikseleX0(O->x2+dx),(int)pikseleY0(O->y1+dy),(int)pikseleX0(O->x2+dx),(int)pikseleY0(O->y2+dy));
  lineC ((int)pikseleX0(O->x1+dx),(int)pikseleY0(O->y2+dy),(int)pikseleX0(O->x2+dx),(int)pikseleY0(O->y2+dy));
  okno_all();
}

void outrect_a(OKNO *O, LINIA *L1)
{

    if ((O == NULL ) || (L1 == NULL))
    {
        Internal_Error (__LINE__,__FILE__);
        return;
    }
    setwritemode(COPY_PUT);
    linestyle(LiniaG.typ);
    if (LiniaG.kolor == 0) SetColorAC(15);
    else SetColorAC(LiniaG.kolor);
    okno_r();
    lineC ((int)pikseleX0(O->x1),(int)pikseleY0(O->y1),(int)pikseleX0(L1->x1),(int)pikseleY0(L1->y1));
    lineC ((int)pikseleX0(L1->x1),(int)pikseleY0(L1->y1),(int)pikseleX0(O->x2),(int)pikseleY0(O->y2));
    lineC ((int)pikseleX0(O->x2),(int)pikseleY0(O->y2),(int)pikseleX0(L1->x2),(int)pikseleY0(L1->y2));
    lineC ((int)pikseleX0(L1->x2),(int)pikseleY0(L1->y2),(int)pikseleX0(O->x1),(int)pikseleY0(O->y1));
    okno_all();
}

void outokno_a(OKNO *O, LINIA *L1)
{

  if ((O == NULL ) || (L1 == NULL))
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
    setwritemode(COPY_PUT);
  setlinestyle1(DOTTED_LINE,0,NORM_WIDTH);
  setcolor(kolory.blok);
  okno_r();
  lineC ((int)pikseleX0(O->x1),(int)pikseleY0(O->y1),(int)pikseleX0(L1->x1),(int)pikseleY0(L1->y1));
  lineC ((int)pikseleX0(L1->x1),(int)pikseleY0(L1->y1),(int)pikseleX0(O->x2),(int)pikseleY0(O->y2));
  lineC ((int)pikseleX0(O->x2),(int)pikseleY0(O->y2),(int)pikseleX0(L1->x2),(int)pikseleY0(L1->y2));
  lineC ((int)pikseleX0(L1->x2),(int)pikseleY0(L1->y2),(int)pikseleX0(O->x1),(int)pikseleY0(O->y1));
  okno_all();
}

#define ORTO_DIST 20

int Set_Orto_Dir (LINIA *L, int orto_dir)
/*--------------------------------------*/
{
  double dx,dy,dxprim,dyprim;
  double angle_l,angle_lr,sinfi,cosfi;
  long l_temp, l_temp1 ;

  if (L == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return I_Orto_NoDir;
  }
  angle_l=get_angle_l();
		      /* uklad globalny */
  dx = L->x2 - L->x1;
  dy = L->y2 - L->y1;

		      /* uklad lokalny */
  if (angle_l!=0)
  {
   angle_lr=angle_l*0.017453293;
   sinfi=sin(angle_lr);
   cosfi=cos(angle_lr);
   dxprim=dx*cosfi+dy*sinfi;
   dyprim=-dx*sinfi+dy*cosfi;
   dx=dxprim;
   dy=dyprim;
  }
  if (TRUE == Check_if_Equal (dx, 0) && TRUE == Check_if_Equal (dy, 0))
  {
    return I_Orto_NoDir ;
  }
  l_temp1 = 3 * pikseleDX (krok_s) / 2 ;
  l_temp = pikseleDX (fabs (dx)) + pikseleDY (fabs (dy)) ;
  if (l_temp < ORTO_DIST   || l_temp < l_temp1)
  {
     orto_dir = I_Orto_YDir;
     if (fabs (dx) >= fabs (dy))
     {
	orto_dir = I_Orto_XDir;
     }
     return orto_dir;
  }
  if ( orto_dir == I_Orto_NoDir)
  {
    orto_dir = I_Orto_YDir;
    if ( fabs(dx) > fabs(dy) )
    {
       orto_dir = I_Orto_XDir;
    }
     return orto_dir;
  }
  return orto_dir;
}


/*--------  Do_Auto_Backup --------------*/
/*------------------------------------------*/

#define FILE_AUTO_BACKUP "BACKUP"

static double time_auto_backup = 60 * 5 ;	/*w sekundach*/
double time_scrsave = 10; // 10; // 60 * 5; /*w sekundach*/
double time_scrsave_ad = 60; // 60 * 5; /*w sekundach*/
static BOOL  do_auto_back ;
static time_t   l_time_1 = -1 ;
static time_t scrsave_time = -1 ;

static BOOL check_auto_backup (void)
/*--------------------------------*/
{
  time_t l_time_2 ;
  BOOL b_retval ;
  double d_timer ;

  b_retval = FALSE ;
  l_time_2 = time (NULL) ;
  d_timer = difftime (l_time_2, l_time_1) ;
  if (d_timer >= time_auto_backup)
  {
    l_time_1 = l_time_2 ;
    b_retval = TRUE ;
  }
  return b_retval;
}

void show_time(void)
{
  struct tm t;
  char time_str[20];
  BOOL showtime=TRUE;

  if (showtime==TRUE)
   {
    //wyswietlenie czasu na ekranie
    strftime(time_str,20,"%X",&t);
    komunikat_str(time_str);
   }
}

void set_scrsave_time (void)
/*------------------------*/
{
  scrsave_time = time (NULL);

  noactive_time = scrsave_time;
  
  return;
}

time_t get_scrsave_time(void)
{
	return scrsave_time;
}

BOOL ScrSave_Proc (int opcja)
/*-------------------------*/
{ time_t current_time;
  double scrsave_d_time;

  double noactive_save_d_time;
  double noactive_send_d_time;
  double noactive_d_time;
  
  char prog_saver[MAXPATH]="SCRSAVE.EXE";
  char param_saver[80]="";
  int k, f, f1, f2, f3, i;
  int Mode;
  int keys;
  char row_screen_buf[1500];
  struct viewporttype view_old1;
  unsigned char lastk='\0';
  unsigned char lk;
  int image_s;
  char path [MAXPATH];
  int disk;


  if (hold_scrsaver == TRUE) return 0;
  if (opcja==0)
   {
    if (time_scrsave<=0) return 0;
   }

  if (sleep_state == TRUE)
  {
	  return 0;
  }

 /*sprawdzenie czasu savera*/  
  current_time = time (NULL) + 1;
  scrsave_d_time = difftime (current_time, scrsave_time) ;


  if ((scrsave_d_time>= time_scrsave) || (opcja==1))
   {
	  set_sleep_state(TRUE);  //SLEEP_STATE
	  set_scrsave_time();

     return 0;
   }   

   return 0;
}


BOOL Auto_Backup_Proc (void)
/*--------------------------*/
{
  BOOL b_retval ;
  char cur_path [MAXPATH] ;
  char file_auto_backup[MAXPATH];
  int disk;
  int flags;
  char  path [MAXPATH], drive [MAXDRIVE], dir [MAXDIR], name [MAXFILE], ext [MAXEXT];
  int k;

#if defined (DEMO_VVV)
  return TRUE ;
#endif

#if defined (DEMO_VVVV)
  return TRUE ;
#endif

  b_retval = FALSE ;
  if (do_auto_back == FALSE || aktmakro != NULL)
  {
    return b_retval;
  }
  if (check_auto_backup () == TRUE)
  {
    Cur_offd(X, Y);
    komunikat0 (49);
	disk = getdisk();  
    Current_Directory (disk + 1, cur_path) ;  
    strncat(cur_path,SSlash,1);

    flags = fnsplit (cur_path, drive, dir, name, ext);
    flags = flags ;  

	sprintf(file_auto_backup, "%s%03d", FILE_AUTO_BACKUP, Client_number);
  
    fnmerge (path, drive, dir, file_auto_backup, EXT__FILE);

    k=pisz_zbior(path, FALSE, 0) ;
    Cur_ond (X, Y);
    komunikat0 (0);
    if ( !k )
    { 
      do_auto_back = FALSE;
      b_retval = TRUE ;
    }
  }
  return b_retval ;
}

/*-------------------------------------------------------*/

static int i__svgamode = SVGA_WRONG_MODE ;

int Get_Svga_Mode (void)
/*--------------------*/
{
  return i__svgamode ;
}


//void change_bs2s(T_Fstring thestring)
void change_bs2s(char *thestring)
{ int i;
#ifndef LINUX
	for (i=0; i<strlen(thestring); i++)
	{
	  if (thestring[i]=='/') thestring[i]='\\';
	}
#endif
    return;
}


static BOOL
get_autobackup (T_Fstring key_name, T_Fstring ret_string)
/*-------------------------------------------------------*/
{  
   int val_int ;
   long val_long ;
   float val_float ;
   //strupr (key_name);
   utf8Upper(key_name);

//   strupr (ret_string);
   if (ret_string==NULL) return TRUE;

   if (stricmp (key_name, IC_SHEET) == 0 )
   {

	 change_bs2s(ret_string);
     strcpy(Current_File_Directory, ret_string);
     if (Current_File_Directory [strlen (Current_File_Directory) - 1] != '\\')
      {
       strcat (Current_File_Directory, "\\");
      }

     strupr(Current_File_Directory);
     
   }
   else
 
   if (stricmp (key_name, IC_EXPLORER_EXE) == 0 )
    {
 
	 change_bs2s(ret_string);

     strcpy(EXPLORER_EXE, ret_string);
     strupr(EXPLORER_EXE);
    }
   else
   if (stricmp (key_name, IC_LOG) == 0 )
    {
	 
	 change_bs2s(ret_string);

      strcpy(log_dir, ret_string);
      strupr(log_dir);
    }
   else
   if (stricmp (key_name, IC_BLOK) == 0 )
    {
 
	 change_bs2s(ret_string);

      strcpy(bloki_dir, ret_string);
      strupr(bloki_dir);
    }
   else
   if (stricmp (key_name, IC_KATALOG) == 0 )
    {
       change_bs2s(ret_string);

      strcpy(katalogi_dir, ret_string);
      strupr(katalogi_dir);
    }
   else
   if (stricmp (key_name, IC_TTF_FONTS) == 0 )
   {
       change_bs2s(ret_string);
       strcpy(default_path_TTF, ret_string);
       if (default_path_TTF[strlen(default_path_TTF)]!='/')
           strcat(&default_path_TTF,"/");
   }
   else
   if (stricmp (key_name, IC_OTF_FONTS) == 0 )
   {
       change_bs2s(ret_string);
       strcpy(default_path_OTF, ret_string);
       if (default_path_OTF[strlen(default_path_OTF)]!='/')
           strcat(&default_path_OTF,"/");
   }
   else
   if (stricmp (key_name, IC_BIBLIOTEKA) == 0 )
    {

	 change_bs2s(ret_string);

     strcpy(biblioteka_cfg, ret_string);
     strupr(biblioteka_cfg);
    }
    else
   if (stricmp (key_name, IC_AUTOBACKUP) == 0 )
   {
      if ( sscanf ( ret_string, "%d", &val_int) == 1  && val_int > 0)
      {
	time_auto_backup = val_int * 60;
      }
   }
   else
   if (stricmp (key_name, IC_KOREKTA_BLOKOW) == 0 )
   {
      if (( sscanf (ret_string, "%d", &val_int) == 1)  &&
          ((val_int == 0) || (val_int == 1)))
      {
 	     auto_korekta_blokow = val_int;
      }
   }
   /*
   else
   if (stricmp (key_name, IC_TOOLS) == 0 )
   {
      if (( sscanf (ret_string, "%d", &val_int) == 1)  &&
          ((val_int >= 0) && (val_int < 5)))
      {
 	     tools_buttons = val_int;
      }
   }
    */
   else
   if (stricmp (key_name, IC_AUTOZOOMBLOCK) == 0 )
   {
      if (( sscanf (ret_string, "%d", &val_int) == 1)  &&
          ((val_int == 0) || (val_int == 1)))
      {
 	     auto_zoom_block = val_int;
      }
   }
   else
   if (stricmp (key_name, IC_BUFMAKSIZE) == 0 )
   {
      if ( sscanf ( ret_string, "%ld", &val_long) == 1  &&
		(val_long*1024) > 99  && (val_long*1024) < 1025000L)
      {
	    Set_Buf_Mak_Size (val_long*1024) ;
      }
   }
   else
   if (stricmp (key_name, IC_BUFMEMSIZE) == 0 )
   {
      if ( sscanf ( ret_string, "%ld", &val_long) == 1  &&
		(val_long*1024) > 1048576 && (val_long*1024) < 2048000000L)
      {
	     Set_Memory_Size (val_long*1024) ;
      }
   }
   else
   if (stricmp (key_name, IC_BUFPRINTMEMSIZE) == 0 )
   {
      if ( sscanf ( ret_string, "%ld", &val_long) == 1  &&
		(val_long*1024) > 1024000L  && (val_long*1024) < 671088640L)
      {
	     Bufor_Wydruku = (val_long*1024) ;
      }
   }
   else
   if (stricmp (key_name, IC_PLATMEMSIZE) == 0 )
   {
      if ( sscanf ( ret_string, "%ld", &val_long) == 1  &&
		(val_long*1024) > 15999999  && (val_long*1024) < 536870912L)  //262144001L)
      {
	     Wielkosc_Plata = (val_long*1024) ;
      }
   }
   else
   if (stricmp (key_name, IC_PCXBUFFERSIZE) == 0 )
   {
      if ( sscanf ( ret_string, "%ld", &val_long) == 1 &&
		  (val_long * 1024) > 1024000L && (val_long * 1024) < 131072000L)
       {
         Set_Buf_PCX (val_long*1024);
       }
   }
   else
   if (stricmp (key_name, IC_PL_ASTER3) == 0 )
   {
    if ( sscanf ( ret_string, "%d", &val_int) == 1  &&
	   (val_int == 0  || val_int == 1))
      {
     	 pznaki_aster3 = val_int ;
      }
   }
   else
   if (stricmp (key_name, IC_CODE_PAGE) == 0 )
   {
      if ( sscanf ( ret_string, "%d", &val_int) == 1  &&
		(val_int > 0))
      {
		  code_page = val_int ;
      }
   }
   else
   if (stricmp (key_name, IC_DXF_ENCODING) == 0 )
   {
      if ( sscanf ( ret_string, "%d", &val_int) == 1  &&
		(val_int > 0))
      {
	   dxf_encoding = val_int ;
      }
   }
   else
    if (stricmp(key_name, IC_BITMAP_PATTERNS_SCALE) == 0)
    {
        if (sscanf(ret_string, "%f", &val_float) == 1 && (val_float > 0))
        {
            PATTERN_SCALE = val_float;
        }
    }
    else
    if (stricmp(key_name, IC_BITMAP_PATTERNS_SCALE_PRN) == 0)
    {
        if (sscanf(ret_string, "%f", &val_float) == 1 && (val_float > 0))
        {
            PATTERN_SCALE_PRN = val_float;
        }
    }
    else
    if (stricmp (key_name, IC_V_MOUSE) == 0 )
    {
        if ( sscanf ( ret_string, "%f", &val_float) == 1  &&
             (val_float > 0.0  && val_float < 100.0))
        {
            Set_Mouse_Speed(val_float) ;
        }
    }
   else
   if (stricmp (key_name, IC_V_MOUSE_MENU) == 0 )
   {
      if ( sscanf ( ret_string, "%d", &val_int) == 1  &&
		(val_int > 0  && val_int < 100))
      {
	SKOKM_VER = val_int ;
      }
   }
   else
   if (stricmp (key_name, IC_V_MOUSE_MENUP) == 0 )
   {
      if ( sscanf ( ret_string, "%d", &val_int) == 1  &&
		(val_int > 0  && val_int < 100))
      {
	SKOKM_HOR = val_int ;
      }
   }
   else
   if (stricmp (key_name, IC_MOUSEBUTTONNO) == 0 )
   {
      if ( sscanf ( ret_string, "%d", &val_int) == 1  &&
	   (val_int == 2  || val_int == 3))
      {
	Set_Mouse_Buttons_No (val_int) ;
      }
   }
   else
   if (stricmp(key_name, IC_MOUSE_SHAKE_FREE) == 0)
   {
       int distx, disty;
       if (sscanf(ret_string, "%d%d", &distx, &disty) < 2)
       {
           ;
       }
       else
       {
           Set_Dist_Free_mouse(distx, disty);
       }

   }

   return TRUE;
}

static BOOL get_desktop(T_Fstring key_name, T_Fstring ret_string)
/*-------------------------------------------------------*/
{
	int val_int;
	long val_long;
	float val_float;

	//strupr(key_name);
    utf8Upper(key_name);
	//   strupr (ret_string);
	if (ret_string == NULL) return TRUE;

	if (stricmp(key_name, IC_BACKGROUNDPCX) == 0)
	{
		change_bs2s(ret_string);
		strcpy(background_pcx_file, ret_string);

	}
	else
	if (stricmp (key_name, IC_SCRSAVE) == 0 )
	{
	   if ( sscanf ( ret_string, "%d", &val_int) == 1  && val_int >= 0)
	   {
	 time_scrsave = val_int * 60;
	   }
	}
	else
		if (stricmp(key_name, IC_PAN) == 0)
		{
			if (sscanf(ret_string, "%f", &val_float) == 1 && (val_float > 0) && (val_float <= 1))
			{
				pan_dxy = val_float;
			}
		}
		else
			if (stricmp(key_name, IC_AUTOPANBLOCK) == 0)
			{
				if ((sscanf(ret_string, "%d", &val_int) == 1) &&
					((val_int == 0) || (val_int == 1)))
				{
					auto_pan_block = val_int;
				}
			}
			else
				if (stricmp(key_name, IC_DYNAMIC_MENU) == 0)
				{
					if ((sscanf(ret_string, "%d", &val_int) == 1) &&
						((val_int == 0) || (val_int == 1)))
					{
						dynamic_menu = val_int;
					}
				}
            else
                if (stricmp(key_name, IC_DIALOG_CURSOR) == 0)
                {
                    if ((sscanf(ret_string, "%d", &val_int) == 1) &&
                        ((val_int == 0) || (val_int == 1) || (val_int == 2)))
                    {
                        BIGCURSOR = val_int;
                    }
                }
            else
                if (stricmp(key_name, IC_MENU_STYLE) == 0)
                {
                    if ((sscanf(ret_string, "%d", &val_int) == 1) &&
                        ((val_int == 0) || (val_int == 1)))
                    {
                        BAR_POINTER = val_int;
                    }
                }
            else
            if (stricmp(key_name, IC_WHEEL_STYLE) == 0)
                {
                    if ((sscanf(ret_string, "%d", &val_int) == 1) &&
                        ((val_int == 0) || (val_int == 1)))
                    {
                        Set_Mouse_Wheel(val_int);
                    }

                }
            else
                if (stricmp(key_name, IC_INSTRUCTION) == 0)
                {
                    if ((sscanf(ret_string, "%d", &val_int) == 1) &&
                        ((val_int == 0) || (val_int == 1)))
                    {
                        desktop_instruction = val_int;
                    }
                }

	else
		if (stricmp(key_name, IC_MOUSEBUTTONNO) == 0)
		{
			if (sscanf(ret_string, "%d", &val_int) == 1 &&
				(val_int == 2 || val_int == 3))
			{
				Set_Mouse_Buttons_No(val_int);
			}
		}

	return TRUE;
}


void Ini_Sys_Param (void)
/*------------------------*/
{
  do_auto_back = FALSE ;

  Get_Private_Profile_Strings ((T_Fstring)IC_DIRECTORY, get_autobackup);
  Get_Private_Profile_Strings((T_Fstring)IC_DESKTOP, get_desktop);
}

void Set_Auto_Backup (BOOL change)
/*---------------------------------*/
{
  if (aktmakro != NULL)
  {
    do_auto_back = FALSE;
    return;
  }
  if (do_auto_back == FALSE && change == TRUE)
  {
     do_auto_back = TRUE ;
     l_time_1 = time (NULL) ;
  }
  if (do_auto_back == TRUE && change == FALSE)
  {
    do_auto_back = FALSE;
  }
}

/*------------------------------------------*/
#undef __O_LIBFUN__

