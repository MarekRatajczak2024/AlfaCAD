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

#define __O_LINE__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#ifndef LINUX
#include<process.h>
#else

#endif
#include<ctype.h>

#ifndef LINUX
#include<io.h>
#include<dos.h>
#endif
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<math.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "o_pline.h"
#include "o_libfun.h"
#include "o_object.h"

#include "menu.h"

extern void Get_Graph_Values_Cur_Pos (double *, double *) ;
extern void view_line_type(LINIA *L);
extern void orto_l(LINIA *L, int *Orto_Dir);
extern BOOL Set_Beginning_Pline_Factory (BLOK *blk_adr, double *df_x_pl_beg, double *df_y_pl_beg);
extern void outvectoror (LINIA *L, AVECTOR *V, int mode,int pl);

static void redcr(char);

enum PLINE_MODE {  PL_MODE_CONTINUE = 1, PL_MODE_LINE, PL_MODE_ARC,
	PL_MODE_END, PL_MODE_UNDO, PL_MODE } ;


static void (*cursor_on)(double ,double)=out_cur_on;
static void (*cursor_off)(double ,double)=out_cur_off;
static BOOL add_line (BOOL b_strwyj) ;
static BOOL add_line3D (BOOL b_strwyj) ;
static double Z3D;

TMENU mEdgeType= {3, 0,0, 12,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmEdgeType, NULL,NULL} ;

static TMENU mAxis1000={1,0,0,20,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmAxis1000,NULL,NULL} ;

static TMENU mAxis50={1,0,0,20,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmAxis50,NULL,NULL} ;

static TMENU mLine={3,0,0,20,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmLine,NULL,NULL} ;

static TMENU mPLine={4,0,0,20,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmPLine,NULL,NULL} ;
static TMENU mPLineSlab={8,0,0,20,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmPLine,NULL,NULL} ;

static TMENU mLine_Con={1,0,0,15,56,4,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmLine_Con,NULL,NULL} ;

static TMENU mPLineObrys={6,0,0,20,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmPLineObrys,NULL,NULL} ;

static TMENU mLine_ConObrys={3,0,0,15,56,4,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmLine_ConObrys,NULL,NULL} ;


enum MENU_ID {IDM_CLOSE = 0, IDM_UNDO, IDM_LEN, IDM_ARC, IDM_FREE_EDGE, IDM_PINNED_EDGE, IDM_FIXED_EDGE, IDM_FLIP_SUPPORT /*IDM_CONTINUOUS_LINE, IDM_DASHED_LINE*/ } ;

enum MENU_ID1 {IDM_NEW_LEN = 0};

static BLOK *BLK_ADR;

/*-----------------------------------------------------------------------*/

static void  cur_off (double x,double y)
/*----------------------------------------*/
{
    flip_screen();
}

/*
static void  cur_off__ (double x,double y)
//----------------------------------------
{
    cursor_off(x,y);
    outlineor (&LiniaG, XOR_PUT, 0) ;
}
*/
static void  cur_on(double x,double y)
/*----------------------------------------*/
{
    LiniaG.x2=x; LiniaG.y2=y;
    switch (LiniaG.obiektt2) {
        case 6:
            if (LiniaG.obiektt3==0)  VectorG.style = V_EDGE_SIMPLE; //18;
            else VectorG.style = V_EDGE_SIMPLE_INV; //19;
            VectorG.typ=LiniaG.typ;
            outvectoror(&LiniaG, &VectorG, COPY_PUT, 1);
            break;
        case 7:
            if (LiniaG.obiektt3==0)  VectorG.style = V_EDGE_FIXED; //20;
            else VectorG.style = V_EDGE_FIXED_INV; //21;
            VectorG.typ=LiniaG.typ;
            outvectoror(&LiniaG, &VectorG, COPY_PUT, 1);
            break;
        default:
            outlineor(&LiniaG, COPY_PUT, 1);
        break;
    }
    cursor_on(x, y);
}

/*
static void  cur_on__(double x,double y)
/---------------------------------------
{
  LiniaG.x2=x; LiniaG.y2=y;
  outlineor (&LiniaG, XOR_PUT, 1) ;
  cursor_on(x, y);
}
*/
#define r18 18
#define r22 16 //22
static int strwyj;
static double Lx2,Ly2,Lz2 ;
static BOOL b__pline ;
//static
double df__xbeg=0.0, df__ybeg=0.0 ;
static double df__len = 0.0 ;

static void set_axis_len (void)
/*---------------------------*/
{
  char sk [MaxTextLen], *str ;
  int retval_no = 1 ;
  double buf_ret [1] ;

  if (EOF == sprintf(sk, "%-8.4f", L50))
   {
    strcpy (sk, "");
   }
  if (NULL != (str = strchr (sk, ' ')))
   {
    str [0] = '\0';
   }

  if (!get_string (sk, "", MaxTextLen, 0, 147)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)
	|| retval_no < 1 || buf_ret [0] <= 0)
  {
    return ;
  }
  L50 = buf_ret [0] ;
  if (L50 > 100000l)
  {
    L50 = 100000l ;
  }
  if (EOF == sprintf(sk, "%-6.2f", L50))
  {
    strcpy (sk, "");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  sk [6] = '\0' ;
  menu_par_new ((*mAxis50.pola)[0].txt, sk) ;
  menu_par_new ((*mAxis1000.pola)[0].txt, sk) ;
}

static void set_line_len (void)
/*---------------------------*/
{
  char sk [MaxTextLen], *str ;
  int retval_no = 1 ;
  double buf_ret [1] ;

  if (EOF == sprintf(sk, "%-8.4f", df__len))
   {
    strcpy (sk, "");
   }
  if (NULL != (str = strchr (sk, ' ')))
   {
    str [0] = '\0';
   }

  if (!get_string (sk, "", MaxTextLen, 0, 48)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)
	|| retval_no < 1 || buf_ret [0] <= 0)
  {
    return ;
  }
  df__len = buf_ret [0] ;
  if (df__len > 100000l)
  {
    df__len = 100000l ;
  }
  if (EOF == sprintf(sk, "%-6.2f", df__len))
  {
    strcpy (sk, "");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  sk [6] = '\0' ;
  menu_par_new ((*mLine.pola)[2].txt, sk) ;
}


static BOOL check_if_break_line (void *ptr_data, LINIA **ptrs_begin_line, LINIA **ptrs_end_line, int *ptri_lineno)
/*--------------------------------------------------------------------------------------------------------------*/
{
  long_long off, offk, ad ;
  NAGLOWEK *nag ;
  LINIA *ptrs_line ;

  nag = (NAGLOWEK*)ptr_data ;
  if ((nag->obiekt != Olinia) /*&& (nag->obiekt != Olinia3D)*/)
  {
    return FALSE ;
  }
  *ptri_lineno = 0 ;
  off =  (char  *)nag - dane ;
  offk = dane_size - 1 ;
  ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  while (ad!=-1)
  {
    nag = (NAGLOWEK*)(dane + ad) ;
    if ((nag->obiekt == Olinia /*|| nag->obiekt == Olinia3D*/) &&
	(nag->atrybut == Anormalny || nag->atrybut == Aoblok || nag->atrybut==Apoblok
      || nag->atrybut==Appoblok || nag->atrybut==Apppoblok ))
    {
      ptrs_line = (LINIA*)nag ;
      if (*ptri_lineno == 0 ||
	 (TRUE == Check_if_Equal (ptrs_line->x2, (*ptrs_begin_line)->x1) &&
	   TRUE == Check_if_Equal (ptrs_line->y2, (*ptrs_begin_line)->y1)))
      {
          if (*ptri_lineno == 1)
          {
              *ptrs_begin_line = ptrs_line;
              *ptrs_end_line = ptrs_line ;
          }
          else  *ptrs_begin_line = ptrs_line ;
	(*ptri_lineno)++ ;
      }
      else
      {
	break ;
      }
    }
    else
    {
      break ;
    }
    ObiektTok (&off, offk, &ad, ONieOkreslony) ;
  }
  return (*ptri_lineno > 0) ? TRUE : FALSE ;
}

static int AXIS50_command_proc (int ev_nr)
/*--------------------------------------*/
{
  int ret_val ;
  int orto_temp ;
  LINIA *ptrs_line, *ptrs_begin_line ;
  int i_lineno ;

  switch (ev_nr)
  {
    case IDM_NEW_LEN :
         	CUR_OFF (X, Y) ;
            set_axis_len ();
          	CUR_ON (X, Y) ;
            ret_val = PL_MODE_CONTINUE ;
      break ;
    default :
      ret_val = PL_MODE_CONTINUE ;
      break ;
  }
  return ret_val ;
}

static int AXIS1000_command_proc (int ev_nr)
/*----------------------------------------*/
{
  int ret_val ;
  int orto_temp ;
  LINIA *ptrs_line, *ptrs_begin_line ;
  int i_lineno ;

  switch (ev_nr)
  {
    case IDM_NEW_LEN :
         	CUR_OFF (X, Y) ;
            set_axis_len ();
          	CUR_ON (X, Y) ;
            ret_val = PL_MODE_CONTINUE ;
      break ;
    default :
      ret_val = PL_MODE_CONTINUE ;
      break ;
  }
  return ret_val ;
}

static int line_command_proc (int ev_nr)
/*--------------------------------------*/
{
  int ret_val ;
  int orto_temp ;
  LINIA *ptrs_line, *ptrs_begin_line, *ptrs_end_line ;
  int i_lineno ;

  switch (ev_nr)
  {
    case IDM_CLOSE :
      if (FALSE == check_if_break_line ((void *)dane,
	    &ptrs_begin_line, &ptrs_end_line, &i_lineno) && i_lineno > 1)
      {
	ret_val = PL_MODE_CONTINUE ;
      }
      else
      {
	CUR_OFF (X, Y) ;
	orto_temp = orto ;
	orto = 0 ;
	Lx2 = ptrs_begin_line->x1 ;
	Ly2 = ptrs_begin_line->y1 ;
	add_line (TRUE) ;
	orto = orto_temp ;
	ret_val = PL_MODE_END ;
	CUR_ON (X, Y) ;
      }
      break ;
    case IDM_UNDO :
      if (FALSE == check_if_break_line ((void *)dane,
	    &ptrs_begin_line, &ptrs_end_line, &i_lineno))
      {
	ret_val = PL_MODE_CONTINUE ;
      }
      else
      {
	ptrs_line = (LINIA*)dane ;
	if (TRUE == Check_if_Equal (ptrs_line->x2, LiniaG.x1) &&
	    TRUE == Check_if_Equal (ptrs_line->y2, LiniaG.y1))
	{
        if (i_lineno > 1)
        {
            LiniaG.x1 = ptrs_end_line->x2 ;
            LiniaG.y1 = ptrs_end_line->y2 ;
            ret_val = PL_MODE_CONTINUE ;
        }
        else
        {
            LiniaG.x1 = LiniaG.x2 ;
            LiniaG.y1 = LiniaG.y2 ;
            ret_val = PL_MODE_END ;
        }

	  rysuj_obiekt ((char*)dane, COPY_PUT, 0) ;
	  Usun_Object ((char*)dane, TRUE) ;

      CUR_OFF (X, Y) ;
	  CUR_ON (X, Y) ;
	}
      }
      break ;
    case IDM_LEN :
      if (TRUE == check_if_break_line ((void *)dane,
	    &ptrs_begin_line, &ptrs_end_line, &i_lineno))
      {
	double x2, y2 ;
	double angle, df_r, df_len  ;

	set_line_len () ;
	df_len = jednostkiOb (df__len) ;
	ptrs_line =  (LINIA *)dane ;
	angle = Atan2 (ptrs_line->y2 - ptrs_line->y1, ptrs_line->x2 - ptrs_line->x1) ;
	df_r = Dist_PP (ptrs_line->x1, ptrs_line->y1, ptrs_line->x2 , ptrs_line->y2) ;
	obrd (sin (angle), cos (angle), df_r + df_len, 0, &x2, &y2) ;
	x2 += ptrs_line->x1 ;
	y2 += ptrs_line->y1 ;
	orto_temp = orto ;
	orto = 0 ;
	Lx2 = x2 ;
	Ly2 = y2 ;
	add_line (TRUE) ;
	orto = orto_temp ;
    CUR_OFF (X, Y) ;
	CUR_ON (X, Y) ;
      }
      ret_val = PL_MODE_CONTINUE ;
      break ;
    default :
      ret_val = PL_MODE_CONTINUE ;
      break ;
  }
  return ret_val ;
}

BOOL Set_Beginning_Pline (BLOK *blk_adr, double *X0, double *Y0)
{
    BOOL ret;
    ret=Set_Beginning_Pline_Factory (blk_adr, &df__xbeg, &df__ybeg);
    BLK_ADR=blk_adr;
    *X0=df__xbeg;
    *Y0=df__ybeg;
    return ret;
}

int PLine_Line_Command_Proc (int ev_nr)
/*-----------------------------------*/
{
  int ret_val ;
  int orto_temp ;
  double df_xend, df_yend ;
  void *ptr_temp, *ptr_ob ;
  BOOL b_first_end ;
  int ret;

  switch (ev_nr)
  {
    case IDM_CLOSE :
      ret=Get_Begin_Pline (BLK_ADR, &ptr_ob, &b_first_end, &df__xbeg, &df__ybeg);

      if (FALSE == Get_End_Pline (BLK_ADR /*(void*)dane*/,  &ptr_ob, &b_first_end,
      df__xbeg, df__ybeg, &df_xend, &df_yend))
      {
	ret_val = PL_MODE_CONTINUE ;
      }
      else
      {
	CUR_OFF (X, Y) ;
	orto_temp = orto ;
	orto = 0 ;
	Lx2 = df__xbeg ;
	Ly2 = df__ybeg ;

    PTR__GTMPBLOCK=(char*)BLK_ADR;
	add_line (TRUE) ;
    BLK_ADR=(BLOK*)PTR__GTMPBLOCK;

	orto = orto_temp ;
	ret_val = PL_MODE_END ;
	CUR_ON (X, Y) ;
      }
      break ;
    case IDM_UNDO :
      if (NULL == (ptr_temp = Get_Pline_Last_Ob (BLK_ADR /*(void*)dane*/)))
      {
	ret_val = PL_MODE_UNDO ;
      }
      else
      {
	rysuj_obiekt ((char*)ptr_temp, COPY_PUT, 0) ;
    CUR_OFF (X, Y) ;
    PTR__GTMPBLOCK=(char*)BLK_ADR;
	Usun_Object (ptr_temp, FALSE) ;
    BLK_ADR=(BLOK*)PTR__GTMPBLOCK;
	Get_End_Pline (BLK_ADR/*(void*)dane*/, &ptr_ob, &b_first_end,
		df__xbeg, df__ybeg, &df_xend, &df_yend) ;
	LiniaG.x1 = df_xend ;
	LiniaG.y1 = df_yend ;

	CUR_ON (X, Y) ;
 	ret_val = PL_MODE_CONTINUE ;
      }
      break ;
    case IDM_LEN :
      PTR__GTMPBLOCK=(char*)BLK_ADR;
      if (FALSE == Get_End_Pline (BLK_ADR/*(void*)dane*/,  &ptr_ob, &b_first_end,
      df__xbeg, df__ybeg, &df_xend, &df_yend))
      {
	ret_val = PL_MODE_CONTINUE ;
      }
      else
      {
	double x2, y2 ;
	double angle, df_r, df_len  ;
	LINIA *ptrs_line ;
	LUK  *ptrs_arc ;

	set_line_len () ;
	df_len = jednostkiOb (df__len) ;
	if ((Olinia == ((NAGLOWEK *)ptr_ob)->obiekt))
	{
	  ptrs_line =  (LINIA *)ptr_ob ;
	  angle = Atan2 (ptrs_line->y2 - ptrs_line->y1, ptrs_line->x2 - ptrs_line->x1) ;
	  df_r = Dist_PP (ptrs_line->x1, ptrs_line->y1, ptrs_line->x2 , ptrs_line->y2) ;
	  obrd (sin (angle), cos (angle), df_r + df_len, 0, &x2, &y2) ;
	  x2 += ptrs_line->x1 ;
	  y2 += ptrs_line->y1 ;
	}
	else
	{
	  ptrs_arc =  (LUK *)ptr_ob ;
	  angle = ((b_first_end == TRUE) ? ptrs_arc->kat1 : ptrs_arc->kat2) ;
	  obrd (sin (angle), cos (angle), ptrs_arc->r,
	       ((b_first_end == TRUE) ? -1 : 1) * df_len, &x2, &y2) ;
	  x2 += ptrs_arc->x ;
	  y2 += ptrs_arc->y ;
	}
	CUR_OFF (X, Y) ;
	orto_temp = orto ;
	orto = 0 ;
	Lx2 = x2 ;
	Ly2 = y2 ;
	add_line (TRUE) ;
    BLK_ADR=(BLOK*)PTR__GTMPBLOCK;
	orto = orto_temp ;
	ret_val = PL_MODE_END ;
	CUR_ON (X, Y) ;
	ret_val = PL_MODE_CONTINUE ;
      }
      break ;
    case IDM_ARC :
      ret_val = PL_MODE_ARC ;
      break ;
      /*
    case IDM_CONTINUOUS_LINE :
      CUR_OFF (X, Y) ;
      LiniaG.typ=LukG.typ=96;
      OBRYS_line_typ=96;
      view_line_type( &LiniaG ) ;
      CUR_ON (X, Y) ;
      ret_val = PL_MODE_CONTINUE ;
      break;
    case IDM_DASHED_LINE :
      CUR_OFF (X, Y) ;
      LiniaG.typ=LukG.typ=65;
      OBRYS_line_typ=65;
      view_line_type( &LiniaG ) ;
      CUR_ON (X, Y) ;
      ret_val = PL_MODE_CONTINUE ;
      break;
       */
      case IDM_FREE_EDGE:
          LiniaG.obiektt2=O2FREE_EDGE;
          LiniaG.obiektt3=O3REGULAR_EDGE;
          LukG.obiektt2=O2FREE_EDGE;
          LukG.obiektt3=O3REGULAR_EDGE;
          ret_val = PL_MODE_CONTINUE ;
          break;
      case IDM_PINNED_EDGE:
          LiniaG.obiektt2=O2HINGED_EDGE;
          LiniaG.obiektt3=O3REGULAR_EDGE;
          LukG.obiektt2=O2HINGED_EDGE;
          LukG.obiektt3=O3REGULAR_EDGE;
          ret_val = PL_MODE_CONTINUE ;
          break;
      case IDM_FIXED_EDGE:
          LiniaG.obiektt2=O2FIXED_EDGE;
          LiniaG.obiektt3=O3REGULAR_EDGE;
          LukG.obiektt2=O2FIXED_EDGE;
          LukG.obiektt3=O3REGULAR_EDGE;
          ret_val = PL_MODE_CONTINUE ;
          break;
      case IDM_FLIP_SUPPORT:
          LiniaG.obiektt3=!LiniaG.obiektt3;
          LukG.obiektt3=!LukG.obiektt3;
          ret_val = PL_MODE_CONTINUE ;
          break;
          /*
      case IDM_ROLLED_EDGE:
          LiniaG.obiektt2=O2FREE_EDGE;
          LiniaG.obiektt3=O3REGULAR_EDGE;
          LukG.obiektt2=O2FREE_EDGE;
          LukG.obiektt3=O3REGULAR_EDGE;
          break;
           */
    default :
      ret_val = PL_MODE_CONTINUE ;
      break ;
  }
  return ret_val ;
}


static BOOL add_line_OLD (BOOL b_strwyj)
/*----------------------------------*/
{ float Lx3,Ly3,Lx4,Ly4;
  BOOL b_ret = FALSE ;
  LINIA LiniaGG;
  char sstt0[32];
  char sstt01[32];

  if(b_strwyj)
  {
    LiniaG.x2 = Double_to_Float (Lx2) ;
    LiniaG.y2 = Double_to_Float (Ly2) ;
  }
  else
  {
    LiniaG.x2 = X ;
    LiniaG.y2 = Y ;
  }
  if(orto)
  {
    orto_l(&LiniaG, &Orto_Dir);
    Orto_Dir = I_Orto_NoDir;
  }
  if (LiniaG.x1 == LiniaG.x2 && LiniaG.y1 == LiniaG.y2)
  {
    return TRUE ;
  }
  
  if (NULL != dodaj_obiekt (((b__pline == TRUE) ? (BLOK*)dane : NULL), &LiniaG))
  {
    
    rysuj_obiekt ((char*)&LiniaG, COPY_PUT, 1) ;
    
    LiniaG.x1 = LiniaG.x2 ;
    LiniaG.y1 = LiniaG.y2 ;
    LiniaG.x2 = X ;
    LiniaG.y2 = Y ;
    b_ret = TRUE ;
  }
  
  return b_ret ;
}

static BOOL add_line (BOOL b_strwyj)
/*---------------------------------*/
{ float Lx3,Ly3,Lx4,Ly4;
  BOOL b_ret = FALSE ;
  LINIA LiniaGG;
  char sstt0[32];
  char sstt01[32];
  double angle_l;
  LINIA L1;
  double P_Orto_Dir;
  double L1000, L2000;

  if(b_strwyj)
  {
    LiniaG.x2 = Double_to_Float (Lx2) ;
    LiniaG.y2 = Double_to_Float (Ly2) ;
  }
  else
  {
    LiniaG.x2 = X ; 
    LiniaG.y2 = Y ;
  }

  if(orto)
   {
    orto_l(&LiniaG, &Orto_Dir);
    Orto_Dir = I_Orto_NoDir;
   }

  if ((AXIS==TRUE) || (AXIS_DIV==TRUE))
  {
  if (AXIS==TRUE)
   {
    L1000=1000;
    L2000=2000;
   }
  ///////////////////
  if (orto)
   {
     angle_l=get_angle_l();
     if (Check_if_Equal(angle_l,0)==TRUE)
        {
         if (LiniaG.y1==LiniaG.y2)  //generowanie linii pionowej
          {
            L_axis.x1=LiniaG.x2;
            L_axis.x2=LiniaG.x2;
            if (AXIS==TRUE)
              {
               L_axis.y1=-L1000;
               L_axis.y2=L2000;
              }
               else
                {
                 L_axis.y1=LiniaG.y2-L50;
                 L_axis.y2=LiniaG.y2+L50;
                }
          }
         else //generowanie linii poziomej
          {
            if (AXIS==TRUE)
             {
              L_axis.x1=-L1000;
              L_axis.x2=L2000;
             }
              else
               {
                L_axis.x1=LiniaG.x2-L50;
                L_axis.x2=LiniaG.x2+L50;
               }
            L_axis.y1=LiniaG.y2;
            L_axis.y2=LiniaG.y2;
          }
        }
         else
          {
            //linia prostopadla do L, przechodzaca przez punkt L1.x2, L1.y2
            //o dlugosci 2000
            if (Check_if_Equal(LiniaG.y2, LiniaG.y1)==TRUE)  //generowanie linii pionowej
              {
               L_axis.x1=LiniaG.x2;
               L_axis.x2=LiniaG.x2;
               if (AXIS==TRUE)
                {
                 L_axis.y1=-L1000;
                 L_axis.y2=L2000;
                }
                 else
                  {
                   L_axis.y1=LiniaG.y2-L50;
                   L_axis.y2=LiniaG.y2+L50;
                  }
              }
              else if (Check_if_Equal(LiniaG.x2, LiniaG.x1)==TRUE)  //generowanie linii poziomej
               {
                L_axis.y1=LiniaG.y2;
                L_axis.y2=LiniaG.y2;
                if (AXIS==TRUE)
                 {
                  L_axis.x2=L2000;
                  L_axis.x1=-L1000;
                 }
                  else
                   {
                     L_axis.x1=LiniaG.x2-L50;
                     L_axis.x2=LiniaG.x2+L50;
                   }
               }
               else //linia ukosna
                {
                 P_Orto_Dir = atan((LiniaG.y2-LiniaG.y1)/(LiniaG.x2-LiniaG.x1));
                 if (AXIS==TRUE)
                  {
                   L_axis.x1=LiniaG.x2-L1000*sin(P_Orto_Dir);
                   L_axis.y1=LiniaG.y2+L1000*cos(P_Orto_Dir);
                   L_axis.x2=LiniaG.x2+L1000*sin(P_Orto_Dir);
                   L_axis.y2=LiniaG.y2-L1000*cos(P_Orto_Dir);
                  }
                   else
                    {
                     L_axis.x1=LiniaG.x2-L50*sin(P_Orto_Dir);
                     L_axis.y1=LiniaG.y2+L50*cos(P_Orto_Dir);
                     L_axis.x2=LiniaG.x2+L50*sin(P_Orto_Dir);
                     L_axis.y2=LiniaG.y2-L50*cos(P_Orto_Dir);
                    }
                }
          }
   //////////////////
  
      if (NULL != dodaj_obiekt (NULL, &L_axis))
       {
        rysuj_obiekt ((char*)&L_axis, COPY_PUT, 1) ;
       }
   }
     return TRUE;
  }

  if (LiniaG.x1 == LiniaG.x2 && LiniaG.y1 == LiniaG.y2)
  {
    return TRUE ;
  }
  
  if (NULL != dodaj_obiekt (((b__pline == TRUE) ? BLK_ADR /*(BLOK*)dane*/ : NULL), &LiniaG))
  {
    
    rysuj_obiekt ((char*)&LiniaG, COPY_PUT, 1) ;
    
    LiniaG.x1 = LiniaG.x2 ;
    LiniaG.y1 = LiniaG.y2 ;
    LiniaG.x2 = X ;
    LiniaG.y2 = Y ;
    b_ret = TRUE ;
  }
  
  return b_ret ;
}

static int L_p(BOOL b_graph_value)
/*-------------------------------*/
{
  PLINIA PL ;
  double l, k ;
  double df_x, df_y ;
  double angle_l ;

  if (eL.val_no < 1) return 0;
  CUR_OFF (X, Y) ;
  l = eL.values [0] ;
  if ( orto == 1 || eL.val_no == 1)
  {
    if (TRUE == b_graph_value)
    {
       Get_Graph_Values_Cur_Pos (&df_x, &df_y) ;
       LiniaG.x2 = df_x ;
       LiniaG.y2 = df_y ;
    }
    parametry_linior (&LiniaG, &PL) ;
    k = PL.kat ;
  }
  else
  {
    angle_l=get_angle_l();
    if (angle_l!=0)
    {
     k = eL.values [1] + angle_l;
     if (k<0) k+=360;
    }
     else
       k = eL.values [1] ;
  }
  l = jednostkiOb(l);
  k = Grid_to_Rad (k) ;
  Lx2 = LiniaG.x1 + l * cos (k) ;
  Ly2 = LiniaG.y1 + l * sin (k) ;
  strwyj = 1;
  CUR_ON (X, Y) ;
  return 1;
}

 int last_pline_delete (void)
/*------------------------*/
{
  if (PL_MODE_CONTINUE != PLine_Line_Command_Proc (1))
	 {
	   redcr (1) ;
    }
  return -83;
}

static int last_line_delete (void)
/*------------------------*/
{
  if (PL_MODE_CONTINUE != line_command_proc (IDM_UNDO))
	 {
	   redcr (1) ;
       return -84;
    }
  return -83;
}

static void redcr(char typ)
/*-------------------------*/
{ static int ( *SW[5])();
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int KR0,np;
  if (typ==0)
   {
     KR0=Komunikat_R0;
     if (b__pline == FALSE)
     {
      SW[4] = SERV [83];  SERV [83] = last_line_delete ;
      if ((AXIS==TRUE) || (AXIS_DIV==TRUE)) Komunikat_R0=127; else Komunikat_R0=8;
     }
      else
       {
        SW[4] = SERV [83];  SERV [83] = last_pline_delete ;
        Komunikat_R0=111;
       }
     komunikat0(Komunikat_R0);
     poczL=1;
     if (b__pline == FALSE)
     {
		 if (AXIS==TRUE) menupini (&mAxis1000, _AXIS_, _AXIS_C_, 0) ;
         else if (AXIS_DIV==TRUE) menupini (&mAxis50, _AXIS_H_, _AXIS_H_C_, 0) ;
           else menupini (&mLine, _LINE_, _LINE_C_, 13) ;
     }
     np=dodajstr(&eL);
     CUR_OFF (X, Y) ;
     CUR_oFF=CUR_OFF;  CUR_OFF=cur_off;
     CUR_oN=CUR_ON;   CUR_ON=cur_on;
     CUR_ON (X, Y) ;
   }
  else if (typ==2)
   {
    SERV [83] = SW [4] ;
   }
  else
   {
     CUR_OFF (X, Y) ;
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     CUR_ON (X, Y) ;
     SERV [83] = SW [4] ;
     Komunikat_R0=KR0;
     poczL=0;
     usunstr(np);
     if (b__pline == TRUE)
     {
       menupini (NULL, "", ' ', 0) ;
     }
     else
     {
     if (AXIS==TRUE) menupini (&mAxis1000, _AXIS_, _AXIS_C_, 0) ;
         else if (AXIS_DIV==TRUE) menupini (&mAxis50, _AXIS_H_, _AXIS_H_C_, 0) ;
           else menupini (&mLine_Con, _LINE_, _LINE_C_, 13) ;
     }
     komunikat0(Komunikat_R0);
   }
}

static void poczatekL (double X0, double Y0)
/*-----------------------------------------*/
{ double LiniaG_x2, LiniaG_y2;
  EVENT *ev ;

  df__xbeg = X0 ;
  df__ybeg = Y0 ;
  LiniaG.x1 = X0 ;
  LiniaG.y1 = Y0 ;
  LiniaG.x2 = X ;
  LiniaG.y2 = Y ;
  Orto_Dir = I_Orto_NoDir ;
  redcr (0) ;
  while (1)
  {
    strwyj = 0 ;
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    if (ev->Number == -83)  continue;
    else if (ev->Number == -84) return;
    if (strwyj == 1)
    {
      if ((AXIS==TRUE) || (AXIS_DIV==TRUE))
       {
        LiniaG.x1=Last_GX;
        LiniaG.y1=Last_GY;
       }
      add_line (TRUE) ;
      if ((AXIS==TRUE) || (AXIS_DIV==TRUE))
       {
        LiniaG.x1=LiniaG.x2;
        LiniaG.y1=LiniaG.y2;
        Last_GX=LiniaG.x2;
        Last_GY=LiniaG.y2;
       }
      CUR_OFF (X, Y) ;
      CUR_ON (X, Y) ;
      continue ;
    }
    switch (ev->What)
    {
      case evKeyDown :
	 if(ev->Number == 0)
	 {
	   redcr (1) ;
	   return ;
	 }
	 if (ev->Number == ENTER)
	 {
	  add_line (FALSE) ;
      if ((AXIS==TRUE) || (AXIS_DIV==TRUE))
       {
        LiniaG.x1=X;
        LiniaG.y1=Y;
        LiniaG.x2=X;
        LiniaG.y2=Y;
       }
       CUR_OFF (X, Y) ;
	   CUR_ON (X, Y) ;
	   break ;
	 }
      case evCommandP :
    if (AXIS==TRUE)
      {
  	    if (PL_MODE_CONTINUE != AXIS1000_command_proc (ev->Number))
	     {
	       redcr (1) ;
	       return ;
	     }
      }
    else if (AXIS_DIV==TRUE)
      {
  	    if (PL_MODE_CONTINUE != AXIS50_command_proc (ev->Number))
	     {
	       redcr (1) ;
	       return ;
	     }
      }
     else
      {
  	    if (PL_MODE_CONTINUE != line_command_proc (ev->Number))
	     {
	       redcr (1) ;
	       return ;
	     }
      }
	 break ;
      default :
	 break ;
    }
   if ((AXIS==TRUE) || (AXIS_DIV==TRUE))
    {
     LiniaG.x1=LiniaG.x2;
     LiniaG.y1=LiniaG.y2;
    }
  }
}

static int poczatekPL (double X0, double Y0, BLOK **blk_adr)
/*-------------------------------------------------------*/
{
  EVENT *ev ;
  int ret_command ;

  LiniaG.x1 = X0 ;
  LiniaG.y1 = Y0 ;
  LiniaG.x2 = X ;
  LiniaG.y2 = Y ;
  Orto_Dir = I_Orto_NoDir ;
  redcr (0) ;
  while (1)
  {
    strwyj = 0 ;
      ////PTR__GTMPBLOCK=(char*)*blk_adr; ////
      BLK_ADR = *blk_adr; ////
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    if (ev -> Number == -83)
     {
      return 2;
     }
    if (strwyj == 1)
    {
      PTR__GTMPBLOCK=(char*)*blk_adr;
      BLK_ADR = *blk_adr;
      add_line (TRUE) ;
      *blk_adr=(BLOK*)PTR__GTMPBLOCK;
      BLK_ADR = *blk_adr;
      CUR_OFF (X, Y) ;
      CUR_ON (X, Y) ;
      continue ;
    }
    switch (ev->What)
    {
      case evKeyDown :
	 if(ev->Number == 0)
	 {
         /*
      if ((PLATE==TRUE) || (HOLE==TRUE) || (WALL==TRUE) || (ZONE==TRUE))
       {
        if (PL_MODE_CONTINUE != (ret_command = PLine_Line_Command_Proc (IDM_CLOSE)))
	      {
	       redcr (1) ;
	       return PL_MODE_END; //ret_command ;
	      }
          else
            {
	           redcr (1) ;
              return PL_MODE_END; //ret_command ;
	         }
       }
        else
          */
          //{
      	   redcr (1) ;
	         return PL_MODE_END ;
          //}
	 }
	 if (ev->Number == ENTER)
	 {
       PTR__GTMPBLOCK=(char*)*blk_adr;
       BLK_ADR = *blk_adr;
	   add_line (FALSE) ;
       *blk_adr = (BLOK*)PTR__GTMPBLOCK;
       BLK_ADR = *blk_adr;
       CUR_OFF (X, Y) ;
	   CUR_ON (X, Y) ;
	   break ;
	 }
      case evCommandP :
	 if (PL_MODE_CONTINUE != (ret_command = PLine_Line_Command_Proc (ev->Number)))
	 {
	   redcr (1) ;
	   return ret_command ;
	 }
        *blk_adr = BLK_ADR;
	 break ;
      default :
	 break ;
    }
  }
}


static void redcr0(char typ, int mode)
/*----------------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[3])(), akt ;

  if (typ==0)
   {
     if (b__pline == FALSE)
     {
       if ((AXIS==TRUE) || (AXIS_DIV==TRUE)) komunikat0 (126); else komunikat0 (7) ;
       if (AXIS==TRUE) menupini (&mAxis1000, _AXIS_, _AXIS_C_, 0) ;
         else if (AXIS_DIV==TRUE) menupini (&mAxis50, _AXIS_H_, _AXIS_H_C_, 0) ;
           else menupini (&mLine_Con, _LINE_, _LINE_C_, 13);
       LiniaG.blok = NoElemBlok ;
     }
     else
     {
       komunikat0 (110) ;
       if (OBRYS==TRUE) menupini (&mPLineObrys, _POLYLINE_, _POLYLINE_C_,20) ;
         else
       {
             if (mode==1) menupini (&mPLineSlab, _POLYLINE_, _POLYLINE_C_,20) ;
             else menupini (&mPLine, _POLYLINE_, _POLYLINE_C_,20) ;
       }
       LiniaG.blok = ElemBlok ;
     }

	 CUR_OFF(X, Y);

     akt=sel.akt; sel.akt=ASel;

	 cursor_off = cursel_off;
	 cursor_on=cursel_on;
     eL.x=maxX-PL266;
     eL.y = ESTR_Y; // 0;
     eL.val_no_max = 2;
     eL.lmax=r22;
     eL.ESTRF=L_p;
	 eL.extend = 0;
     eL.mode = GV_VECTOR ;
     eL.format = "%#13.9lg\0%#13.9lg;%#7.2lf" ;
     CUR_oFF=CUR_OFF;  CUR_OFF=cursel_off;
     CUR_oN=CUR_ON;   CUR_ON=cursel_on;
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
     
     menupini (NULL, "", ' ',0) ;

     sel.akt=akt;

	 CUR_ON(X, Y);

     LiniaG.blok = NoElemBlok ;
     komunikat0 (0) ;
   }
}

void Linia (void)
/*------------*/
{
  EVENT *ev;
  double X0, Y0;
  long dane_size0 ;
  char sk [MaxTextLen], *str ;

  if (AXIS==TRUE)
   {
     L_axis.warstwa=Current_Layer;
     L_axis.blok=0;
     L_axis.typ=34;
     L_axis.kolor=kolorys.osie;
     L_axis.obiektt2=O2NoBlockS;
   }
    else
      if (AXIS_DIV==TRUE)
       {
        L_axis.warstwa=Current_Layer;
        L_axis.blok=0;
        L_axis.typ=67;
        L_axis.kolor=kolorys.linie_podzialu;
        L_axis.obiektt2=O2BlockHatch25;
      }
  if ((AXIS==TRUE) || (AXIS_DIV==TRUE))
   {
    if (EOF == sprintf(sk, "%-6.2f", L50))
    {
     strcpy (sk, "");
    }
    if (NULL != (str = strchr (sk, ' ')))
    {
     str [0] = '\0';
    }
     sk [6] = '\0' ;
    menu_par_new ((*mAxis50.pola)[0].txt, sk) ;
    menu_par_new ((*mAxis1000.pola)[0].txt, sk) ;
   }
  b__pline = FALSE ;
  dane_size0 = dane_size ;
  redcr0 (0, 0) ;

  while (1)
  {
     view_line_type(&LiniaG);
     ev=Get_Event_Point(NULL, &X0, &Y0);
     Last_GX=X0;
     Last_GY=Y0;
     switch(ev->What)
      {
	case evKeyDown  :
	     if(ev->Number==0)
	      {
		redcr0(1, 0);
		return;
	      }
	     if(ev->Number== ENTER)
	      { 
		poczatekL (X0, Y0);
	      }
	     break;
	case evCommandP :
	   if (0 == ev->Number && dane_size > dane_size0)
	   {
	     poczatekL (((LINIA*)dane)->x2, ((LINIA*)dane)->y2) ;
	   }
	   break;
	default :
	     break;
      }
   }
}

int Pline_Line (double df_xbeg, double df_ybeg, BLOK **blk_adr, int mode)
/*---------------------------------------------------------------------*/
{
  int ret ;
  double df_xend, df_yend ;
  BOOL b_first_end ;
  void *ptr_ob ;

  b__pline = TRUE ;
  if (*blk_adr==(BLOK*)dane)
  {
      df__xbeg = df_xbeg;
      df__ybeg = df_ybeg;
  }
  redcr0 (0, mode) ;
  Get_End_Pline (*blk_adr /*(void*)dane*/, &ptr_ob, &b_first_end,
	df_xbeg, df_ybeg, &df_xend, &df_yend) ;
  ret = poczatekPL (df_xend, df_yend, blk_adr) ;
  redcr0 (1, mode) ;
  redcr(2);
  return ret ;
}

#undef __O_LINE__