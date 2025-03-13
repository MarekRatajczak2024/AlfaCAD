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

#define __O_PROF__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_object.h"
#include "bib_blok.h"
#include "o_osnap.h"
#include "o_libfun.h"
#include "bib_edit.h"
#include "o_pline.h"

#include "menu.h"

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))


static unsigned u__objest1_type, u__objest2_type ;
static void *ptr__objest1, *ptr__objest2 ;
static double df__ob1_x, df__ob1_y ;
static double df__ob2_x, df__ob2_y ;
static double df__r_fillet = 10 ;       //jednostki objektu

static int 	przeciecieLLn_	(double *, double *, void *, void *);
static int 	obciecie	(LINIA *, double ,double ,double ,double );
static void set_chamfer_line (LINIA *, LINIA * , LINIA *);


static void 	undo_prof 	(void);
static char* fillet_line_line (double,LINIA*,LINIA *,double ,double ,double , double );
static char* fillet_line_circle (double,LINIA*,OKRAG *,double ,double ,double , double );
static int 	PierwszyObiektWskazany (double, double);
static int 	DrugiObiektWskazany (double, double) ;
static int 	l_r		(BOOL);
static void 	redcr		(char typ);
static void 	set_prof_arc (LUK *, LINIA *, LINIA *);

static void 	redcr_chamfer 	(char typ);
static int  	e_chamfer 	(BOOL);
static BOOL 	second_line 	(void);
static BOOL 	first_line 	(void);
static BOOL 	chamfer 	(double , double , double , double );
static BOOL 	test_chamfer 	(int, LINIA *, double *, double *, double, double, double);
static void 	undo_chamfer 	(void);
static void 	chamfer_distance(void);
static void 	fillet_radius(void);

static LUK *lps__arc_prof;

static LINIA *ptrs__line1, *ptrs__line2;
static LINIA *ptrs__line_chamfer;
static LINIA s__line1_old, s__line2_old;
static double dist1 = 10, dist2 = 10;
static double dist01 = 10, dist02 = 10 ;                //jednostki obiektu
static BOOL b_undo_chamfer = FALSE;
static BOOL b_undo_prof;


TMENU mChamfer = { 2,0,0,7,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmChamfer,NULL,NULL };

TMENU mFillet = {2,0,0,7,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmFillet,NULL,NULL};

static void (*COMNDCh[])(void)={ chamfer_distance, undo_chamfer };
static void (*COMNDProf[])(void)={ fillet_radius, undo_prof };


extern void layer_info (void) ;

#define r20 20
#define r18 12
#define r16 16
#define r14 14


static int przeciecieLLn_(double *x, double *y, void *adr, void *adr1)
{ LINIA  *adrl,*adrl1;
  double t,p,kx;
  adrl=(LINIA*)adr;
  adrl1=(LINIA*)adr1;
  kx=(adrl1->x2-adrl1->x1)*(adrl->y2-adrl->y1)-(adrl1->y2-adrl1->y1)*(adrl->x2-adrl->x1);
  if (fabs (kx) < 0.005)
  {
    ErrList (29);
    return 0 ;
  }

  if(fabs(adrl->x1-adrl->x2)<0.0001)
   { p=(adrl->x1-adrl1->x1)/(adrl1->x2-adrl1->x1);
     t=(adrl1->y1-adrl->y1+p*(adrl1->y2-adrl1->y1))/(adrl->y2-adrl->y1);
   }
   else
    { p=((adrl1->y1-adrl->y1)*(adrl->x2-adrl->x1)-(adrl1->x1-adrl->x1)*(adrl->y2-adrl->y1))/kx;
      t=(adrl1->x1-adrl->x1+p*(adrl1->x2-adrl1->x1))/(adrl->x2-adrl->x1);
    }
  *x=adrl->x1+t*(adrl->x2-adrl->x1);
  *y=adrl->y1+t*(adrl->y2-adrl->y1);
  return 1;
}


static int obciecie(LINIA *L, double x0,double y0,double x,double y)
{ double t;
  int k;

  y = y;
  if(fabs(L->x2-L->x1)<=.0001)
  {
    if (fabs (L->y2 - y) < fabs (L->y1 - y)) k = 1;
    else k = 2;
    return k;
  }
  else t=(x0-L->x1)/(L->x2-L->x1);
  if(0<=t && t<=1)
   if(x0==L->x1 || (x-L->x1)/(x0-L->x1)>=1) k=2;
   else k=1;
  else
   { t=(L->x2-L->x1)/(x0-L->x1);
     if(t>0 && t<1) k=1;
     else k=2;
   }
  return k;
}

static void set_chamfer_line (LINIA *L, LINIA * L1, LINIA *L2)
/*----------------------------------------------------------*/
{
  L->warstwa = Current_Layer;
  L->kolor = Layers [Current_Layer].color;
  L->typ = Layers [Current_Layer].line_type;
  if (L1->warstwa == L2->warstwa)
  {
    L->warstwa = L1->warstwa;
  }
  if (L1->kolor == L2->kolor)
  {
    L->kolor = L1->kolor;
  }
  if (Get_Width_Line (L1->typ) == Get_Width_Line (L2->typ))
  {
    L->typ = Set_WidthL_to_Type (L->typ, Get_Width_Line (L1->typ));
  }
  if (Get_Type_Line (L1->typ) == Get_Type_Line (L2->typ))
  {
    L->typ = Set_TypeL_to_Type (L->typ, Get_Type_Line (L1->typ));
  }
}


/*-----------------------------------------------------------------------*/
/*------ wyznaczenie luku, korekta lini -----------*/

static void set_prof_arc (LUK *l, LINIA * L1, LINIA *L2)
/*----------------------------------------------------*/
{
  LINIA L = Ldef;

  set_chamfer_line (&L, L1, L2);
  l->kolor = L.kolor;
  l->warstwa = L.warstwa;
  l->typ = L.typ;
}

static void set_prof_arc_LO (LUK *l, LINIA * ptrs_line1, OKRAG *ptrs_c)
/*-----------------------------------------------------------*/
{
  LINIA s_line = Ldef, s_line2 = Ldef ;

  s_line2.warstwa = ptrs_c->warstwa ;
  s_line2.kolor = ptrs_c->kolor ;
  s_line2.typ = ptrs_c->typ ;
  set_chamfer_line (&s_line, ptrs_line1, &s_line2);
  l->kolor = s_line.kolor;
  l->warstwa = s_line.warstwa;
  l->typ = s_line.typ;
}

static void fillet_radius(void)
{
	;
}

static void undo_prof (void)
/*---------------------------*/
{
  if (b_undo_prof == FALSE)
  {
    ErrList (53);
    return;
  }
  rysuj_obiekt ((char *)ptrs__line1, COPY_PUT, 0) ;
  rysuj_obiekt ((char *)lps__arc_prof, COPY_PUT, 0) ;
  memcpy (ptrs__line1, &s__line1_old, sizeof (LINIA));
  rysuj_obiekt ((char *)ptrs__line1, COPY_PUT, 1) ;
  if (NULL != ptrs__line2)
  {
    rysuj_obiekt ((char *)ptrs__line2, COPY_PUT, 0) ;
    memcpy (ptrs__line2, &s__line2_old, sizeof (LINIA));
    rysuj_obiekt ((char *)ptrs__line2, COPY_PUT, 1) ;
  }
  usun_obiekt ((void *)lps__arc_prof);
  CUR_OFF (X, Y) ;
  CUR_ON (X, Y);
  b_undo_prof = FALSE;
  return ;
}

static char *fillet_line_line (double df_r, LINIA*L1,LINIA *L2,double X1,double Y1,double X2, double Y2)
{
  int k1,k2 ;
  double a,a1,a2,b,co,si,cob,sib;
  double stangle,endangle,Xr,Yr,R,e;
  struct P3
   { double x0,y0,x1,y1,x2,y2;
   } p3,p3l;
   LUK s_arc ;
   LINIA Lp1, Lp2;
  double df_l1x, df_l1y, df_l2x, df_l2y ;
  long off1, off2, l_move;
  BLOK *adb;
  int i_fillet_type ;

  memcpy (&s_arc, &LukG, sizeof(LUK)) ;
  s_arc.r = df_r ;
  if (FILLET_PL_NO == (i_fillet_type = Check_Pline_to_Fillet (L1, L2)))
  {
    ErrList (83) ;
    return NULL ;
  }
  b_undo_prof = FALSE;
   memcpy (&s__line1_old, L1, sizeof (LINIA));
   memcpy (&s__line2_old, L2, sizeof (LINIA));

   if (s_arc.r <= 0)
   {
     s_arc.r = 0.0001;
   }
  memcpy (&Lp1, L1, sizeof (LINIA));
  memcpy (&Lp2, L2, sizeof (LINIA));
  if(!przeciecieLLn_ (&p3.x0, &p3.y0, &Lp1, &Lp2))
      return NULL ;
  najblizszyL_ (&X1,&Y1,&Lp1);
  k1=obciecie(&Lp1,p3.x0,p3.y0,X1,Y1);
  df_l1x = Lp1.x1 ;
  df_l1y = Lp1.y1 ;
  if (k1 == 2)
  {
    df_l1x = Lp1.x2 ;
    df_l1y = Lp1.y2 ;
  }
  p3.x1=X1-p3.x0; p3.y1=Y1-p3.y0;
  najblizszyL_ (&X2,&Y2,&Lp2);
  k2=obciecie(&Lp2,p3.x0,p3.y0,X2,Y2);
  df_l2x = Lp2.x1 ;
  df_l2y = Lp2.y1 ;
  if (k2 == 2)
  {
    df_l2x = Lp2.x2 ;
    df_l2y = Lp2.y2 ;
  }
  p3.x2=X2-p3.x0; p3.y2=Y2-p3.y0;
  df_l1x -= p3.x0 ;
  df_l1y -= p3.y0 ;
  df_l2x -= p3.x0 ;
  df_l2y -= p3.y0 ;
  a1=Atan2(p3.y1,p3.x1);
  if(a1<0) a1+=2*Pi;
  a2=Atan2(p3.y2,p3.x2);
  if(a2<0) a2+=2*Pi;
  a=(a1+a2)/2;
  if(fabs(a2-a1)>Pi) a+=Pi;
  if(a>2*Pi) a-=2*Pi;
  b=fabs(a-a1);
  if(b>Pi/2) b=2*Pi-b;
  si=sin(a);
  co=cos(a);
  R=s_arc.r;
  cob=cos(b);
  sib=sin(b);
  if (fabs (sib) < .0001)
  {
    ErrList (29) ;
    return NULL;
  }
  p3.x1=p3.x2=R*((1/sib)-sib);
  e=(sin(a1-a)>=0) ? 1 : -1;
  p3.y1=e*R*cob;
  p3.y2=-p3.y1;
  obru (si, co, df_l1x, df_l1y, &df_l1x, &df_l1y) ;
  obru (si, co, df_l2x, df_l2y, &df_l2x, &df_l2y) ;
  if (p3.x1 > df_l1x || p3.x2 > df_l2x)
  {
    ErrList (32) ;
     return NULL;
  }
  obrd(si,co,p3.x1,p3.y1,&p3.x1,&p3.y1);
  obrd(si,co,p3.x2,p3.y2,&p3.x2,&p3.y2);
  obrd(si,co,R/sib,0,&Xr,&Yr);
  s_arc.x=Xr+p3.x0;
  s_arc.y=Yr+p3.y0;
  p3l.x1=p3.x1-Xr;  p3l.x2=p3.x2-Xr;
  p3l.y1=p3.y1-Yr;  p3l.y2=p3.y2-Yr;
  a1=Atan2(p3l.y1,p3l.x1);
  if(a1<0) a1+=2*Pi;
  a2=Atan2(p3l.y2,p3l.x2);
  if(a2<0) a2+=2*Pi;
  if(fabs(a2-a1)>=Pi)
   { stangle=max(a1,a2);
     endangle=min(a1,a2);
   }
  else
   { stangle=min(a1,a2);
     endangle=max(a1,a2);
   }
  s_arc.kat1=stangle;
  s_arc.kat2=endangle;

  set_prof_arc (&s_arc, L1, L2);
  off1 = (char  *)L1 - dane;
  off2 = (char  *)L2 - dane;
  adb = Get_Common_Blok (L1, L2);
  Set_Block_Change_Mode ((void *)L1, OB1CHANGE_BREAK) ;
  Set_Block_Change_Mode ((void *)L2, OB1CHANGE_BREAK) ;
  if (adb == NULL)
  {
    s_arc.blok = NoElemBlok;
    s_arc.obiektt2 = O2NoBlockS ;
    l_move = 0 ;
    off1 += sizeof (LUK) ;
    off2 += sizeof (LUK) ;
  }
  else
  {
    s_arc.blok = ElemBlok ;
    s_arc.obiektt2 = L1->obiektt2 ;
    if (i_fillet_type == FILLET_PL_YES)
    {
      l_move = min ((char  *)L1 - (char  *)adb,
		  (char  *)L2 - (char  *)adb) ;
      l_move += sizeof(LINIA) ;
      if (off1 > off2)
	off1 += sizeof (LUK) ;
      else
	off2 += sizeof (LUK) ;
    }
    else
    {
      l_move = sizeof(NAGLOWEK) + adb->n ;
    }
  }
  if(NULL == (lps__arc_prof = (LUK*)Add_Block_Object_Ex (&adb, (void *)&s_arc, l_move)))
      return NULL ;
  ptrs__line1 = L1 = (LINIA*)(dane + off1) ;
  ptrs__line2 = L2 = (LINIA*)(dane + off2) ;
  rysuj_obiekt ((char *)L1, COPY_PUT, 0) ;
  rysuj_obiekt ((char *)L2, COPY_PUT, 0) ;
  if(k1==1)
  {
    L1->x2=p3.x1+p3.x0;
    L1->y2=p3.y1+p3.y0;
  }
  else
  {
    L1->x1=p3.x1+p3.x0;
    L1->y1=p3.y1+p3.y0;
  }
  if(k2==1)
  {
    L2->x2=p3.x2+p3.x0;
    L2->y2=p3.y2+p3.y0;
  }
  else
  {
    L2->x1=p3.x2+p3.x0;
    L2->y1=p3.y2+p3.y0;
  }
  rysuj_obiekt ((char *)L1, COPY_PUT, 1) ;
  rysuj_obiekt ((char *)L2, COPY_PUT, 1) ;
  rysuj_obiekt ((char *)&s_arc, COPY_PUT, 1) ;
  CUR_OFF (X, Y) ;
  CUR_ON(X,Y);
  if (adb != NULL && adb->obiektt2 == O2BlockPline)
  {
    Check_Pline (adb) ;
  }
  b_undo_prof = TRUE;
  return lps__arc_prof;
}

char *fillet_line_to_line(double df_r, LINIA* L1, LINIA* L2)
{
    double x1, y1, x2, y2;

    x1 = L1->x1+2*(L1->x2-L1->x1)/3;
    y1 = L1->y1+2*(L1->y2-L1->y1)/3;
    x2 = L2->x1+(L2->x2-L2->x1)/3;
    y2 = L2->y1+(L2->y2-L2->y1)/3;

    return fillet_line_line(df_r, L1, L2, x1, y1, x2, y2);
}

static char* fillet_line_circle (double df_r,
		    LINIA *ptrs_line,
		    OKRAG * ptrs_circle,
		    double df_x1,
		    double df_y1,
		    double df_x2,
		    double df_y2)
{
  LINIA s_line_tmp ;
  OKRAG s_circle_tmp ;
  LUK s_arc ;
  double df_angle, df_si, df_co ;
  double df_liney ;
  double df_int_r ;
  double df_int_x1, df_int_x2 ; /*przeciecie odcinka z okregiem*/
  double df_ax1, df_ay1, df_ax2, df_ay2 ;	/*konce luku*/
  double df_t1, df_xtmp, df_ytmp ;
  double df_xc, df_yc ;
  double df_dd ;
  BOOL b_int_left, b_down, b_inside, b_first_trunc ;
  long off1 ;
  BLOK * adb ;

  b_undo_prof = FALSE ;
  memcpy (&s_arc, &LukG, sizeof(LUK)) ;
  s_arc.r = df_r ;
  memcpy (&s__line1_old, ptrs_line, sizeof (LINIA));
  memcpy (&s_line_tmp, ptrs_line, sizeof (LINIA)) ;

  if (s_arc.r <= 0)
  {
    s_arc.r = 0.00001;
  }
  najblizszyL_ (&df_x1, &df_y1, ptrs_line) ;
  NearestO_ (&df_x2, &df_y2, ptrs_circle) ;
  df_angle = Atan2 (ptrs_line->y2 - ptrs_line->y1,
		    ptrs_line->x2 - ptrs_line->x1) ;
  df_si = sin (-df_angle) ;
  df_co = cos (-df_angle) ;
  Rotate_Line (&s_line_tmp, ptrs_circle->x, ptrs_circle->y, df_si, df_co) ;
  df_liney = s_line_tmp.y1 ;
  Rotate_Point (df_si, df_co, ptrs_circle->x, ptrs_circle->y,
		 df_x1, df_y1, &df_x1, &df_y1) ;
  Rotate_Point (df_si, df_co, ptrs_circle->x, ptrs_circle->y,
		 df_x2, df_y2, &df_x2, &df_y2) ;
  if (TRUE == Check_if_LT (fabs (ptrs_circle->y - df_liney) , ptrs_circle->r))
  {
    if (df_y2 > df_liney)
    {
      df_yc = df_liney + s_arc.r ;
      if (TRUE == Check_if_GT (df_yc + s_arc.r, ptrs_circle->y + ptrs_circle->r))
      {
	ErrList (32) ;
	return FALSE ;
      }
      b_down = FALSE ;
    }
    else
    {
      df_yc = df_liney - s_arc.r ;
      if (TRUE == Check_if_LT (df_yc - s_arc.r, ptrs_circle->y - ptrs_circle->r))
      {
	ErrList (32) ;
	return FALSE ;
      }
      b_down = TRUE ;
    }
    df_dd = ptrs_circle->r * ptrs_circle->r -
	    (ptrs_circle->y - df_liney) * (ptrs_circle->y - df_liney) ;
    if (df_dd < 0)
    {
      df_dd = 0 ;
    }
    df_int_x1 = ptrs_circle->x - sqrt (df_dd) ;
    df_int_x2 = ptrs_circle->x + sqrt (df_dd) ;
    df_int_r = ptrs_circle->r + s_arc.r ;
    b_inside = FALSE ;
    if (df_x1 > df_int_x1 && df_x1 < df_int_x2)
    {
      df_int_r = ptrs_circle->r - s_arc.r ;
      b_inside = TRUE ;
    }
    b_int_left = TRUE ;
    if (fabs (df_x1 - df_int_x1) > fabs (df_x1 - df_int_x2))
    {
      b_int_left = FALSE ;
    }
    df_dd = df_int_r * df_int_r -
	     (ptrs_circle->y - df_yc) * (ptrs_circle->y - df_yc) ;
    if (df_dd < 0)
    {
      df_dd = 0 ;
    }
    df_dd = sqrt (df_dd) ;
    df_xc = ptrs_circle->x + (b_int_left == TRUE ? -df_dd : df_dd) ;
    s_circle_tmp.x = df_xc ;
    s_circle_tmp.y = df_yc ;
    s_circle_tmp.r = s_arc.r ;
    if (0 == przeciecieLO_tt (&df_ax1, &df_ay1, &df_t1,
			      &df_ax1, &df_ay1, &df_t1,
			      &s_line_tmp, &s_circle_tmp) ||
	0 == przeciecieOO_ex (&df_ax2, &df_ay2,
			      &df_ax2, &df_ay2,
			      &s_circle_tmp, ptrs_circle))
    {
      Internal_Error (__LINE__,__FILE__);
      return NULL ;
    }
    if ((b_int_left == TRUE && b_inside == FALSE) ||
	(b_int_left == FALSE && b_inside == TRUE))
    {
      b_first_trunc = FALSE ;
      s_line_tmp.x2 = df_ax1 ;
      s_line_tmp.y2 = df_ay1 ;
    }
    else
    {
      b_first_trunc = TRUE ;
      s_line_tmp.x1 = df_ax1 ;
      s_line_tmp.y1 = df_ay1 ;
    }
    if ((b_first_trunc == TRUE && df_t1 > 1) ||
	(b_first_trunc == FALSE && df_t1 < 0))
    {
      ErrList (32) ;
      return NULL ;
    }
    if ((b_int_left == TRUE && b_inside == FALSE && b_down == TRUE) ||
	(b_int_left == TRUE && b_inside == TRUE && b_down == FALSE) ||
	(b_int_left == FALSE && b_inside == TRUE && b_down == TRUE) ||
	(b_int_left == FALSE && b_inside == FALSE && b_down == FALSE))
    {
      df_xtmp = df_ax1 ;
      df_ytmp = df_ay1 ;
      df_ax1 = df_ax2 ;
      df_ay1 = df_ay2 ;
      df_ax2 = df_xtmp ;
      df_ay2 = df_ytmp ;
    }
    s_arc.x = df_xc ;
    s_arc.y = df_yc ;
    Points_To_Arc (&s_arc, df_ax1, df_ay1, df_ax2, df_ay2) ;
    df_si = sin (df_angle) ;
    df_co = cos (df_angle) ;
    Rotate_Line (&s_line_tmp, ptrs_circle->x, ptrs_circle->y, df_si, df_co) ;
    Rotate_Arc (&s_arc, ptrs_circle->x, ptrs_circle->y, df_si, df_co) ;
  }
  else
  {
    if (TRUE == Check_if_GT (fabs (df_liney - ptrs_circle->y), ptrs_circle->r + 2 * s_arc.r))
    {
      ErrList (84) ;
      return NULL ;
    }
    b_down = TRUE ;
    df_yc = df_liney + s_arc.r ;
    if (df_liney > ptrs_circle->y)
    {
      b_down = FALSE ;
      df_yc = df_liney - s_arc.r ;
    }
    df_int_r = ptrs_circle->r + s_arc.r ;
    b_int_left = TRUE ;
    if (df_x1 > ptrs_circle->x)
    {
      b_int_left = FALSE ;
    }
    df_dd = df_int_r * df_int_r -
	     (ptrs_circle->y - df_yc) * (ptrs_circle->y - df_yc) ;
    if (df_dd < 0)
    {
      df_dd = 0 ;
    }
    df_dd = sqrt (df_dd) ;
    df_xc = ptrs_circle->x + (b_int_left == TRUE ? -df_dd : df_dd) ;
    s_circle_tmp.x = df_xc ;
    s_circle_tmp.y = df_yc ;
    s_circle_tmp.r = s_arc.r ;
    if (0 == przeciecieLO_tt (&df_ax1, &df_ay1, &df_t1,
			      &df_ax1, &df_ay1, &df_t1,
			      &s_line_tmp, &s_circle_tmp) ||
	0 == przeciecieOO_ex (&df_ax2, &df_ay2,
			      &df_ax2, &df_ay2,
			      &s_circle_tmp, ptrs_circle))
    {
      Internal_Error (__LINE__,__FILE__);
      return NULL ;
    }
    if (b_int_left == TRUE)
    {
      b_first_trunc = FALSE ;
      s_line_tmp.x2 = df_ax1 ;
      s_line_tmp.y2 = df_ay1 ;
    }
    else
    {
      b_first_trunc = TRUE ;
      s_line_tmp.x1 = df_ax1 ;
      s_line_tmp.y1 = df_ay1 ;
    }
    if ((b_first_trunc == TRUE && df_t1 > 1) ||
	(b_first_trunc == FALSE && df_t1 < 0))
    {
      ErrList (32) ;
      return NULL ;
    }
    if ((b_int_left == TRUE && b_down == FALSE) ||
	(b_int_left == FALSE && b_down == TRUE))
    {
      df_xtmp = df_ax1 ;
      df_ytmp = df_ay1 ;
      df_ax1 = df_ax2 ;
      df_ay1 = df_ay2 ;
      df_ax2 = df_xtmp ;
      df_ay2 = df_ytmp ;
    }
    s_arc.x = df_xc ;
    s_arc.y = df_yc ;
    Points_To_Arc (&s_arc, df_ax1, df_ay1, df_ax2, df_ay2) ;
    df_si = sin (df_angle) ;
    df_co = cos (df_angle) ;
    Rotate_Line (&s_line_tmp, ptrs_circle->x, ptrs_circle->y, df_si, df_co) ;
    Rotate_Arc (&s_arc, ptrs_circle->x, ptrs_circle->y, df_si, df_co) ;
  }
  if (TRUE == Check_if_Vertex_Inside_Pline (ptrs_line,
		      (b_first_trunc == TRUE ? ptrs_line->x1 : ptrs_line->x2),
		      (b_first_trunc == TRUE ? ptrs_line->y1 : ptrs_line->y2)))
  {
    ErrList (85) ;
    return NULL ;
  }

  set_prof_arc_LO (&s_arc, ptrs_line, ptrs_circle) ;
  off1 = (char  *)ptrs_line - dane ;
  adb = Get_Common_Blok (ptrs_line, ptrs_circle) ;
  Set_Block_Change_Mode ((void *)ptrs_line, OB1CHANGE_BREAK) ;
  Set_Block_Change_Mode ((void *)ptrs_circle, OB1CHANGE_BREAK) ;
  s_arc.blok = NoElemBlok ;
  s_arc.obiektt2 = O2NoBlockS ;
  if (adb != NULL)
  {
    s_arc.blok = ElemBlok ;
    s_arc.obiektt2 = ptrs_line->obiektt2 ;
  }
  if(NULL == (lps__arc_prof = (LUK*) dodaj_obiekt (adb, (void *)&s_arc))) 
      return NULL ;
  ptrs__line1 = ptrs_line = (LINIA*)(dane + off1 + (adb == NULL ? sizeof(LUK) : 0)) ;
  ptrs__line2 = NULL ;
  rysuj_obiekt ((char *)ptrs_line, COPY_PUT, 0) ;
  memcpy (ptrs_line, &s_line_tmp, sizeof(LINIA)) ;
  rysuj_obiekt ((char *)ptrs_circle, COPY_PUT, 1) ;
  rysuj_obiekt ((char *)ptrs_line, COPY_PUT, 1) ;
  rysuj_obiekt ((char *)&s_arc, COPY_PUT, 1) ;
  CUR_OFF (X, Y) ;
  CUR_ON(X,Y);
  b_undo_prof = TRUE ;
  return lps__arc_prof ;
}

/*-----------------------------------------------------------------------*/

static int PierwszyObiektWskazany (double df_x, double df_y)
{
  int ret = 0 ;

  if (Error) ClearErr();

  u__objest1_type = Blinia | Bokrag ;  /* | Bluk*/
  if ((ptr__objest1 = select_w(&u__objest1_type, NULL))==NULL) return ret ;
   switch (u__objest1_type)
    {
      case Blinia :
      case Bokrag :
	 df__ob1_x = df_x ;
	 df__ob1_y = df_y ;
     rysuj_obiekt (ptr__objest1, XOR_PUT, 1) ;
     out_krz (df_x, df_y) ;
	 ret = 1 ;
	 break ;
      case Bluk   : break;
      default     : break;
     }
  return ret;
}

static int DrugiObiektWskazany (double df_x, double df_y)
{
  int ret = 0 ;

  u__objest2_type = Blinia | Bokrag ;  /* | Bluk*/
  if ((ptr__objest2 = select_w(&u__objest2_type, NULL))==NULL) return ret ;
   switch (u__objest2_type)
    {
      case Blinia :
      case Bokrag :
	 if (u__objest1_type == Bokrag && u__objest2_type == Bokrag)
	 {
	   return ret ;
	 }
	 df__ob2_x = df_x ;
	 df__ob2_y = df_y ;
     rysuj_obiekt (ptr__objest2, XOR_PUT, 1) ;
	 out_krz (df_x, df_y) ;
	 ret = 1 ;
	 break;
      case Bluk   : break;
      default     : break;
     }
  return ret;
}

static int fillet_proc (void)
/*-------------------------*/
{
  char *ret_ptr;

  if (u__objest1_type == Blinia && u__objest2_type == Blinia)
  {
    ret_ptr=fillet_line_line (Double_to_Float (jednostkiOb (df__r_fillet)),
		      (LINIA*)ptr__objest1,
		      (LINIA*)ptr__objest2,
		      df__ob1_x,
		      df__ob1_y,
		      df__ob2_x,
		      df__ob2_y) ;
  }
  else
  if (u__objest1_type == Blinia && u__objest2_type == Bokrag)
  {
    ret_ptr=fillet_line_circle (Double_to_Float (jednostkiOb (df__r_fillet)),
		      (LINIA*)ptr__objest1,
		      (OKRAG*)ptr__objest2,
		      df__ob1_x,
		      df__ob1_y,
		      df__ob2_x,
		      df__ob2_y) ;
  }
  else
  if (u__objest2_type == Blinia && u__objest1_type == Bokrag)
  {
    ret_ptr=fillet_line_circle (Double_to_Float (jednostkiOb (df__r_fillet)),
		      (LINIA*)ptr__objest2,
		      (OKRAG*)ptr__objest1,
		      df__ob2_x,
		      df__ob2_y,
		      df__ob1_x,
		      df__ob1_y) ;
  }
  else
  {
    return 0 ;
  }
  if (ret_ptr==NULL) return 0;
  return 1;
}


static int l_r(BOOL b_graph_value)
{
  double r;
  char r_str[16];

  b_graph_value = b_graph_value ;
  if (eL.val_no < 1)
  {
    return 0 ;
  }
  r = eL.values [0] ;
  if (r >= 0)
  {
    df__r_fillet = r ;
	sprintf(r_str, "%g", df__r_fillet);
	menu_par_new((*mFillet.pola)[0].txt, r_str);
    return 1;
  }
  else return 0;
}

static void redcr(char typ)
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[3])(),KR,KR0,akt,sel_cur,sel_gor,sel_nr,np;
  static TMENU *menu;
  static char st[POLE_TXT_MAX];
  static unsigned short cod = L' ';
  static int iconno=0;
  

  if (typ==0)
   { 
	  CUR_OFF(X, Y);
	  KR=Komunikat_R;
     KR0=Komunikat_R0;
     komunikat(0);
     getmenupini(&menu,st,&cod, &iconno);

     menupini(&mFillet, _FILLET_, _FILLET_C_, 47);

     akt=sel.akt; sel.akt=ASel;
     sel_cur=sel.cur; sel.cur=1;
     sel_gor = sel.gor; sel.gor = 0;
     sel_nr = sel.nr;

     layer_info();

     b_undo_prof = FALSE;
     eL.x = maxX/2 - 10/*+1*/;
     eL.lmax=r14;
     eL.y= ESTR_Y;
     eL.val_no_max = 1 ;
     eL.mode = GV_DIST ;
     eL.format = "%-10.8lg" ;
     eL.ESTRF=l_r;
	 eL.extend = 0;
     if (-1 != (np=dodajstr(&eL)) && (EOF != sprintf(eL.st, "%10.8lg", df__r_fillet)) )
     {
       eL.val_no = 1 ;
     }
     CUR_oFF=CUR_OFF;  CUR_OFF=out_sel_off;
     CUR_oN=CUR_ON;   CUR_ON=out_sel_on;
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;
     CUR_ON(X,Y);
   }
  else
   { CUR_OFF(X,Y);
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     SERV[73]=SW[0];
     SERV[81]=SW[1];
     //CUR_ON(X,Y);
     menupini(menu,st,cod, iconno);
     sel.akt=akt;
     sel.cur=sel_cur;
     sel.gor = sel_gor;
     sel.nr = sel_nr;

     layer_info();
     CUR_ON(X,Y);
     Komunikat_R=KR;
     Komunikat_R0=KR0;
     komunikat(KR);
     komunikat0(KR0);
     usunstr(np);
   }
}

void profilowanie(void)
{
  EVENT *ev;
  double X0, Y0;
  char r_str[16];

    ptr__objest1=NULL;
    ptr__objest2=NULL;

  sprintf(r_str, "%g", df__r_fillet);
  menu_par_new((*mFillet.pola)[0].txt, r_str);
  redcr(0);
  while(1)
aa:{ komunikat0(11);
  Out_Edited_Draw_Param ((ESTR *)&eL, TRUE) ;
     while(1)
      { ev=Get_Event_Point(NULL, &X0, &Y0);
	if ( ev->What == evKeyDown  && ev->Number== 0 )
    {
        if (Error) ClearErr();
        redcr(1);
        return;
    }
	if ( ev->What == evKeyDown  && ev->Number== ENTER &&
	   PierwszyObiektWskazany(X0, Y0))
    {
        CUR_OFF(X0, Y0);
        CUR_ON(X0, Y0);
        break;
    }
	if( ev->What==evCommandP)
	   if (ev->Number==0)
	   {
		   simulate_keypress(TAB);
	   }
	   else (*COMNDProf[ev->Number])();
      }
     komunikat0(12);
     Out_Edited_Draw_Param ((ESTR *)&eL, TRUE) ;
     while(1)
      { ev = Get_Event_Point(NULL, &X0, &Y0) ;
	if ( ev->What == evKeyDown  && ev->Number== 0 )
    {
        rysuj_obiekt(ptr__objest1, COPY_PUT, 1);
        CUR_OFF(X, Y);
        CUR_ON(X, Y);
        goto aa;
    }
	if ( ev->What == evKeyDown  &&
	     ev->Number== ENTER &&
         DrugiObiektWskazany (X0, Y0) )
    {
        if (fillet_proc ())
        {
            CUR_OFF(X0, Y0);
            CUR_ON(X0, Y0);
          break ;
        }
        else
        {
            rysuj_obiekt(ptr__objest1, COPY_PUT, 1);
            if (ptr__objest2) rysuj_obiekt(ptr__objest2, COPY_PUT, 1);
            CUR_OFF(X0, Y0);
            CUR_ON(X0, Y0);
            break;
        }
    }
	if(ev->What == evCommandP)
	{
		if (ev->Number == 0)
		{
			simulate_keypress(TAB);
		}
		else ErrList (52);
	  continue;
	}
      }
   }
}

/*-----------------------------------------------------------------------*/

static void chamfer_distance(void)
{
	;
}

static void undo_chamfer (void)
/*---------------------------*/
{
  char  * adp,  *adk,  *pom;

  if (b_undo_chamfer == FALSE)
  {
    ErrList (53);
    return;
  }
  adp = (char *)ptrs__line1;
  adk = (char *)ptrs__line2;
  if (adp > adk)
  {
    pom = adp;
    adp = adk;
    adk = pom;
  }
  pom = (char  *)ptrs__line_chamfer ;
  if (adp > pom)
    adp = pom;
  else
  if (adk < pom)
   adk = pom;
  ptrs__line1->atrybut = Ablok ;
  ptrs__line2->atrybut = Ablok ;
  ptrs__line_chamfer->atrybut = Ablok ;
  blokzap (adp, adk, Ablok, COPY_PUT, 0);
  memcpy (ptrs__line1, &s__line1_old, sizeof (LINIA));
  memcpy (ptrs__line2, &s__line2_old, sizeof (LINIA));
  rysuj_obiekt(ptrs__line1, COPY_PUT, 1);
  rysuj_obiekt(ptrs__line2, COPY_PUT, 1);
  CUR_OFF (X, Y) ;
  CUR_ON (X, Y);
  b_undo_chamfer = FALSE;
  usun_obiekt ((void *)ptrs__line_chamfer);
  return ;
}


static BOOL   test_chamfer (int k, LINIA *L, double *x_dist, double *y_dist,
		double x0, double y0, double dist)
/*---------------------------------------------------------------*/
{
  double si,co, a;

  if (k == 1)
  {
    *x_dist = L->x1 - x0;
    *y_dist = L->y1 - y0;
  }
  else
  {
    *x_dist = L->x2 - x0;
    *y_dist = L->y2 - y0;
  }
  a = Atan2(*y_dist, *x_dist);
  si = sin(a);
  co = cos(a);
  obru (si, co, *x_dist, *y_dist, x_dist, y_dist);
  if (*x_dist < dist)
  {
    return  FALSE;
  }
  obrd (si, co, dist, 0, x_dist, y_dist);
  *x_dist += x0;
  *y_dist += y0;
  if (k == 1)
  {
    L->x2 = *x_dist ;
    L->y2 = *y_dist ;
  }
  else
  {
    L->x1 = *x_dist ;
    L->y1 = *y_dist ;
  }
  return TRUE;
}

static BOOL chamfer (double X1, double Y1, double X2, double Y2)
/*---------------------------------------------------------*/
{
  int k1,k2;
  double x0, y0, Lx, Ly;
  LINIA Lp1, Lp2, L;
  char * adp,  *adk;
  long off1, off2, l_move ;
  BLOK *adb;
  int i_fillet_type ;
  float L_x1, L_y1;
  BOOL inversion = FALSE;

  if (FILLET_PL_NO == (i_fillet_type = Check_Pline_to_Fillet (ptrs__line1, ptrs__line2)))
  {
    ErrList (86) ;
    return 0 ;
  }
  b_undo_chamfer = FALSE;
  memcpy (&L, &LiniaG, sizeof (LINIA));
  memcpy (&s__line1_old, ptrs__line1, sizeof (LINIA));
  memcpy (&s__line2_old, ptrs__line2, sizeof (LINIA));
  memcpy (&Lp1, ptrs__line1, sizeof (LINIA));
  memcpy (&Lp2, ptrs__line2, sizeof (LINIA));

  if(!przeciecieLLn_(&x0, &y0, &Lp1, &Lp2))
  {
    ErrList(29);
    return FALSE;
  }
  najblizszyL_ (&X1, &Y1, &Lp1);
  k1 = obciecie (&Lp1, x0, y0, X1, Y1);
  if (FALSE == test_chamfer (k1, &Lp1, &Lx, &Ly,
	 x0, y0, dist1))
  {
    ErrList (52);
    return FALSE;
  }
  L.x1 = Lx;
  L.y1 = Ly;
  najblizszyL_ (&X2, &Y2, &Lp2);
  k2 = obciecie (&Lp2, x0, y0, X2, Y2);
  if (FALSE == test_chamfer (k2, &Lp2, &Lx, &Ly,
	 x0, y0, dist2))
  {
    ErrList (52);
    return FALSE;
  }
  L.x2 = Lx;
  L.y2 = Ly;

  
  set_chamfer_line (&L, &Lp1, &Lp2);
  adp = (char  *)ptrs__line1;
  adk = (char  *)ptrs__line2;
  off1 = adp - dane;
  off2 = adk - dane;
  adb = Get_Common_Blok (ptrs__line1, ptrs__line2);

  //checking if within poliline ends should be inverted
  if ((adb!=NULL) && (adb->kod_obiektu = B_PLINE))
  {
      if (adp > adk)
      {
          //chack if one by one
          if (adp == adk+sizeof(NAGLOWEK)+((LINIA*)adk)->n) inversion = TRUE;
      }
      else
      {
          if (adk != adp + sizeof(NAGLOWEK) + ((LINIA*)adp)->n) inversion = TRUE;
      }

      if (inversion==TRUE)
      {
          L_x1 = L.x1;
          L_y1 = L.y1;
          L.x1 = L.x2;
          L.y1 = L.y2;
          L.x1 = L.x2;
          L.x2 = L_x1;
          L.y2 = L_y1;
      }
  }

  Set_Block_Change_Mode ((void *)ptrs__line1, OB1CHANGE_BREAK) ;
  Set_Block_Change_Mode ((void *)ptrs__line2, OB1CHANGE_BREAK) ;
  if (adb == NULL)
  {
    L.blok = NoElemBlok;
    L.obiektt2 = O2NoBlockS ;
    l_move = 0 ;
    off1 += sizeof (LINIA) ;
    off2 += sizeof (LINIA) ;
  }
  else
  {
    L.blok = ElemBlok ;
    L.obiektt2 = ptrs__line1->obiektt2 ;
    if (i_fillet_type == FILLET_PL_YES)
    {
      l_move = min ((char  *)ptrs__line1 - (char  *)adb,
		  (char  *)ptrs__line2 - (char  *)adb) ;
      l_move += sizeof(LINIA) ;
      if (off1 > off2)
	off1 += sizeof (LINIA) ;
      else
	off2 += sizeof (LINIA) ;
    }
    else
    {
      l_move = sizeof(NAGLOWEK) + adb->n ;
    }
  }
  if(NULL == (ptrs__line_chamfer = (LINIA*) Add_Block_Object_Ex (&adb, (void *)&L, l_move))) 
      return 0 ;
  ptrs__line1 = (LINIA*)(dane + off1) ;
  ptrs__line2 = (LINIA*)(dane + off2) ;
  CUR_OFF (X, Y) ;
  rysuj_obiekt ((char *)ptrs__line1, COPY_PUT, 0) ;
  rysuj_obiekt ((char *)ptrs__line2, COPY_PUT, 0) ;
  memcpy (ptrs__line1, &Lp1,sizeof (LINIA));
  memcpy (ptrs__line2, &Lp2,sizeof (LINIA));
  rysuj_obiekt ((char *)ptrs__line1, COPY_PUT, 1) ;
  rysuj_obiekt ((char *)ptrs__line2, COPY_PUT, 1) ;
  rysuj_obiekt ((char *)ptrs__line_chamfer, COPY_PUT, 1) ;
  CUR_ON (X, Y);
  b_undo_chamfer = TRUE;
  if (adb != NULL && adb->obiektt2 == O2BlockPline)
  {
    Check_Pline (adb) ;
  }
  return TRUE;
}


static BOOL first_line (void)
/*-------------------------*/
{
  BOOL ret = FALSE;
  static unsigned type;

    if (Error) ClearErr();

  type = Blinia ;
  if ((ptrs__line1 = (LINIA*)select_w(&type, NULL)) != NULL)
  {
    df__ob1_x=X;
    df__ob1_y=Y;
    rysuj_obiekt (ptrs__line1, XOR_PUT, 1) ;
    out_krz(X,Y);
    ret = TRUE;
  }
  return ret;
}

static BOOL second_line (void)
/*-------------------------*/
{
  unsigned type ;
  BOOL ret = FALSE ;
  type = Blinia ;
  if ((ptrs__line2 = (LINIA*)select_w(&type, NULL)) != NULL)
  {
      rysuj_obiekt (ptrs__line2, XOR_PUT, 1) ;
    out_krz(X,Y);
    ret = chamfer (df__ob1_x,df__ob1_y,X,Y);
  }
  return ret;
}

static int e_chamfer (BOOL b_graph_value)
/*-------------------------------------*/
{
  double l,k;
  char d_str[24];

  b_graph_value = b_graph_value ;
  if (eL.val_no < 1)
  {
    return 0 ;
  }
  k = l = eL.values [0] ;
  if (eL.val_no > 1)
  {
    k = eL.values [1] ;
  }
  if (k < 0 || l < 0)
  {
    return 0;
  }
  dist01 = l ;
  dist02 = k ;
  l = jednostkiOb (l);
  k = jednostkiOb (k);
  dist1 = (double)Double_to_Float (l) ;
  dist2 = (double)Double_to_Float (k) ;
  sprintf(d_str, "%g;%g", dist01, dist02);
  menu_par_new((*mChamfer.pola)[0].txt, d_str);
  return 1;
}

static void redcr_chamfer (char typ)
/*---------------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[3])(),KR,KR0,akt,sel_cur,sel_gor,sel_nr,np;
  static TMENU *menu;
  static char st[POLE_TXT_MAX];
  unsigned short cod = L' ';
  static int iconno=0;

  if (typ==0)
   { 
	  CUR_OFF(X, Y);
	  KR=Komunikat_R;
     KR0=Komunikat_R0;
     komunikat(0);
     getmenupini(&menu,st,&cod, &iconno);

     menupini(&mChamfer, _CHAMFER_, _CHAMFER_C_, 48);

     akt=sel.akt; sel.akt=ASel;
     sel_cur=sel.cur; sel.cur=1;
     sel_gor = sel.gor; sel.gor = 0;
     sel_nr = sel.nr;

     layer_info();

     b_undo_chamfer = FALSE;
     eL.x = maxX/2-40/*+1*/;
     eL.y= ESTR_Y;
     eL.lmax = r20;
     eL.val_no_max = 2 ;
     eL.mode = GV_DIST ;
     eL.format= "%-8.6lg\0%-8.6lg;%-8.6lg" ;
     eL.ESTRF = e_chamfer;
	 eL.extend = 0;
     dist1 = jednostkiOb (dist01) ;
     dist2 = jednostkiOb (dist02) ;
     if (-1 != (np=dodajstr(&eL)))
     {
       sprintf (eL.st, "%8.6lg", dist01) ;
       eL.val_no = 1 ;
       if (dist01 != dist02)
       {
	 sprintf (eL.st, "%8.6lg;%8.6lg", dist01, dist02) ;
	 eL.val_no = 2 ;
       }
     }
     
     CUR_oFF=CUR_OFF;  CUR_OFF=out_sel_off;
     CUR_oN=CUR_ON;   CUR_ON=out_sel_on;
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;
     CUR_ON(X,Y);
   }
  else
   {
     CUR_OFF(X,Y);
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     SERV[73]=SW[0];
     SERV[81]=SW[1];
     menupini(menu,st,cod, iconno);
     sel.akt=akt;
     sel.cur=sel_cur;
     sel.gor = sel_gor;
     sel.nr = sel_nr;

     layer_info();
	 CUR_ON(X, Y);
     komunikat(Komunikat_R = KR);
     komunikat0(Komunikat_R0 = KR0);
     usunstr(np);
   }
}

void Chamfer (void)
/*----------------*/
{
  EVENT *ev;
  double X0, Y0;
  char d_str[24];

    ptrs__line1=NULL;
    ptrs__line2=NULL;

  sprintf(d_str, "%g;%g", dist1, dist2);
  menu_par_new((*mChamfer.pola)[0].txt, d_str);

  redcr_chamfer(0);
  while(1)
  {
aa: komunikat0 (44);
    Out_Edited_Draw_Param ((ESTR *)&eL, TRUE) ;
    while(1)
    {
      ev=Get_Event_Point(NULL, &X0, &Y0);
      if ( ev->What == evKeyDown  && ev->Number== 0 ) {
          if (Error) ClearErr();
          redcr_chamfer(1);
          return; }
      if ( ev->What == evKeyDown  && ev->Number== ENTER &&
	first_line ())
      {
          CUR_OFF(X0, Y0);
          CUR_ON(X0, Y0);
          break;
      }
      if( ev->What==evCommandP)
	  if (ev->Number == 0)
	  {
		  simulate_keypress(TAB);
	  }
	  else (*COMNDCh[ev->Number])();
    }
    komunikat0(45);
    Out_Edited_Draw_Param ((ESTR *)&eL, TRUE) ;
    while(1) {
        ev = Get_Event_Point(NULL, &X0, &Y0);
        if (ev->What == evKeyDown && ev->Number == 0) {
            rysuj_obiekt(ptrs__line1, COPY_PUT, 1);
            CUR_OFF(X, Y);
            CUR_ON(X, Y);
            goto aa;
        }
        if (ev->What == evKeyDown && ev->Number == ENTER &&
            second_line())
        {
            CUR_OFF(X0, Y0);
            CUR_ON(X0, Y0);
            break;
        }
        else
        {
            rysuj_obiekt(ptrs__line1, COPY_PUT, 1);
            if (ptrs__line2) rysuj_obiekt(ptrs__line2, COPY_PUT, 1);
            break;
        }
      if( ev->What==evCommandP)
      {
		  if (ev->Number == 0)
		  {
			  simulate_keypress(TAB);
		  }
	     else ErrList (52);
	continue;
      }
    }
  }
}
/*-----------------------------------------------------------------------*/
#undef __O_PROF__