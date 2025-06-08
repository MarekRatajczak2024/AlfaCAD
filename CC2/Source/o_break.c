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

#define __O_BREAK__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#include<stdlib.h>
#include <math.h>
#include <stdio.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "b_messag.h"
#include "o_libfun.h"
#include "o_object.h"
#include "o_osnap.h"
#include "o_undo.h"
#include "o_pline.h"

#include "menu.h"

#include "leak_detector_c.h"

extern void *PTR__Sel_Adr,*PTR__Sel_Adr1 ;
extern void Break_Line3D (LINIA3D *ptrs_line_s, LINIA3D *ptrs_line_d1,	LINIA3D *ptrs_line_d2, LINIA3D *ptrs_line_d3, double df_x1, double df_y1, double df_x2, double df_y2, double *ptrdf_t1, double *ptrdf_t2);
extern void InfoListStr1(int n, char *st);
extern int make_spline_shadows(long_long l_offb, long_long l_offe, int atrybut, BOOL join, int s_atrybut, int prec, int to_block);
extern void delete_spline_shadows(BOOL make_Ablok);
extern void *check_if_pline(void *ptr_object);
extern void spline_shadows_to_plines(void);

#define OZero 1.0E-5

static BOOL 	break_fun	(void);
static BOOL 	break_object	(void);
static BOOL 	break_arc 	(void);
static BOOL 	break_ellipticalarc 	(void);
static BOOL 	break_circle 	(void);
static BOOL 	break_line 	(void);
static BOOL     break_line_wire (void);
static BOOL 	obiect_to_break (void **ptr_ob_pl);
static void 	redcr_break 	(char);
static void 	object_zap 	(BOOL, int);
static void 	undo_break 	(void);
static void 	undo_break_object (BOOL, BOOL, BOOL, void *, void *, void *, void *);
static void 	podziel 	(void);
static BOOL 	break_pline 	(void) ;

static unsigned		u__type_object, u__last_type_object;
static void 		*ptr__object  ; /*adres wskazanego obiektu, dla polilinii adres polilinii*/
static double 		df__point1X, df__point1Y, df__point2X, df__point2Y;
static int 		i__krok = 0;
static BOOL 		b__pline ;
static BOOL 		b__undo_break;
static BOOL 		b__line_break1, b__line_break2,  b__line_break3,
			b__arc_break1, b__arc_break2, b__arc_break3,
			b__circle_break1, b__circle_break2,
            b__ellipticalarc_break1, b__ellipticalarc_break2, b__ellipticalarc_break3,
            b__ellipse_break1, b__ellipse_break2;
static LINIA 		s__L1 = Ldef, s__L2 =Ldef, s__L3 =Ldef;
static LUK 		s__l1 = ldef, s__l2 = ldef, s__l3 = ldef;
static ELLIPTICALARC 		s__el1 = eldef, s__el2 = eldef, s__el3 = eldef;
static OKRAG 		s__o_old  ;
static ELLIPSE 		s__e_old  ;
static LUK		s__l_old;
static ELLIPTICALARC  s__el_old;
static LINIA		s__L_old;
static int shadows_no = 0;
static BOOL break_and_leave=TRUE;

/*-----------------------------------------------------------------------*/

#define UNDO_FUN 0
#define ONE_BREAK 1
#define BREAK_OPTIONS 2
#define BREAK_AND_LEAVE 3
#define BREAK_AND_CUT 4

TMENU mBreak = {3,0,0,10,56,4,ICONS | TADD,CMNU,CMBR,CMTX, 0, COMNDmnr, 0, 0,0,(POLE(*)[]) &pmBreak, NULL, NULL};

/*-----------------------------------------------------------------------*/

static BOOL initial_sel_test (void* ptr_ob)
/*---------------------------------------*/
{
  NAGLOWEK *nag ;
  BOOL b_ret ;

  b_ret = FALSE ;
  nag = (NAGLOWEK*)ptr_ob ;
  if (nag->atrybut == Ablok)
  {
    b_ret = TRUE ;
  }
  return b_ret ;
}

static BOOL intersect_add_test (void* ptr_ob1, void* ptr_ob2)
/*----------------------------------------------------------*/
{
  NAGLOWEK *nag1, *nag2 ;
  BOOL b_ret ;

  b_ret = TRUE ;
  nag1 = (NAGLOWEK*)ptr_ob1 ;
  nag2 = (NAGLOWEK*)ptr_ob2 ;
  if (nag1->atrybut == Ablok &&
      nag2->atrybut == Ablok)
  {
    b_ret = FALSE ;
  }
  return b_ret ;
}


static void undo_break_object (BOOL b_break1, BOOL b_break2, BOOL b_break3,
				void * ob1, void *ob2, void *ob3, void * ob_old)
/*----------------------------------------------------------------*/
{
  BLOK 	*adb;
  char 	*ado;

  ado = NULL;
  if (TRUE == b_break1) ado = Find_Object (ob1);
  if ( ado == NULL && b_break2 == TRUE) ado = Find_Object (ob2);
  adb  = Parent_Block ((char  *)ado);
  if(NULL == dodaj_obiekt (adb, ob_old))
  {
    return ;
  }
  if (TRUE == b_break1)
  {
    if (NULL == (ado = Find_Object (ob1)))
    {
      Internal_Error (__LINE__,__FILE__);
      return ;
    }
    blokzap ((char  *)ado, (char  *)ado, Anormalny, COPY_PUT, 0);
    usun_obiekt (ado);
  }
  if (TRUE == b_break2)
  {
    if (NULL == (ado = Find_Object ((void *)ob2)))
    {
      Internal_Error (__LINE__,__FILE__);
      return ;
    }
    blokzap ((char  *)ado, (char  *)ado, Anormalny, COPY_PUT, 0);
    usun_obiekt (ado);
  }
  if (TRUE == b_break3)
    {
        if (NULL == (ado = Find_Object (ob3)))
        {
            Internal_Error (__LINE__,__FILE__);
            return ;
        }
        blokzap ((char  *)ado, (char  *)ado, Anormalny, COPY_PUT, 0);
        usun_obiekt (ado);
    }
  rysuj_obiekt ((char *)ob_old, COPY_PUT, 1) ;
}

static void undo_pline (void)
/*-------------------------*/
{
  char  *ptrh_beg,  *ptrh_end ;

  if (PTR__GTMP4!= NULL)
  {
    ptrh_beg = PTR__GTMP4;
    ptrh_end = ptrh_beg + sizeof (NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1 ;
    blokzap (ptrh_beg, ptrh_end, Anormalny, COPY_PUT, 0) ;
    usun_obiekt ((char*)ptrh_beg);
  }
  if (PTR__GTMP5 != NULL)
  {
    ptrh_beg = PTR__GTMP5 ;
    ptrh_end = ptrh_beg + sizeof (NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1 ;
    blokzap (ptrh_beg, ptrh_end, Anormalny, COPY_PUT, 0) ;
    usun_obiekt ((char*)ptrh_beg);
  }
  if (PTR__GTMP7 != NULL)
  {
    ptrh_beg = PTR__GTMP7 ;
    ptrh_end = ptrh_beg + sizeof (NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1 ;
    blokzap (ptrh_beg, ptrh_end, Anormalny, COPY_PUT, 0) ;
    usun_obiekt ((char*)ptrh_beg);
  }
  ptrh_beg = PTR__GTMP6 ;
  ptrh_end = ptrh_beg + sizeof (NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1 ;
  zmien_atrybut (ptrh_beg, ptrh_end, Abad, Anormalny) ;
  blokzap (ptrh_beg, ptrh_end, Anormalny, COPY_PUT, 1) ;

  CUR_OFF (X, Y);
  CUR_ON (X, Y);
}


static void undo_break (void)
/*--------------------------*/
{

  if (b__undo_break == FALSE)
  {
    return;
  }
  b__undo_break = FALSE;
  switch (u__last_type_object)
  {
    case Blinia :
     undo_break_object (b__line_break1, b__line_break2, b__line_break3,
				(void *)&s__L1, (void *)&s__L2, (void *)&s__L3,
				(void *)&s__L_old);
     break;
    case Bluk   :
     undo_break_object (b__arc_break1, b__arc_break2, b__arc_break3,
				(void *)&s__l1, (void *)&s__l2, (void *)&s__l3,
				(void *)&s__l_old);
       break;
     case Bellipticalarc   :
          undo_break_object (b__ellipticalarc_break1, b__ellipticalarc_break2, b__ellipticalarc_break3,
                             (void *)&s__el1, (void *)&s__el2, (void *)&s__el3,
                             (void *)&s__el_old);
          break;
    case Bokrag :
     undo_break_object (b__circle_break1, b__circle_break2, FALSE,
				(void *)&s__l1, (void *)&s__l2, NULL,
				(void *)&s__o_old );
       break;
     case Bellipse :
          undo_break_object (b__ellipse_break1, b__ellipse_break2, FALSE,
                             (void *)&s__el1, (void *)&s__el2, NULL,
                             (void *)&s__e_old );
          break;
    case Blinia | Bluk :
       undo_pline () ;
       break ;
    default     :
       break;
  }
  b__undo_break=FALSE;
  flip_screen();
}

static void podziel (void)
/*-----------------------*/
{

  if (i__krok != 2)
  {
    ErrList (52);
    return;
  }
  df__point2X = df__point1X;
  df__point2Y = df__point1Y;
  PTR__GTMP2 = PTR__GTMP1 ;
  break_object ();
}


static BOOL break_line0 (void)
/*--------------------------*/
{
  LINIA 	*L;
  LINIA 	*lps_line_break1 ;
  LINIA3D 	*L3D;
  LINIA3D 	*lps_line_break13D ;
  double 	t1, t2 ;
  BLOK	 	*adb;
  void 		*lps_old;

  b__line_break1 = FALSE;
  b__line_break2 = FALSE;
  L = (LINIA*)ptr__object  ;

       memcpy ((void *)&s__L_old, (void *)L, sizeof (LINIA)) ;
       Break_Line (L, &s__L1, &s__L2, &s__L3,
	         df__point1X, df__point1Y,
	         df__point2X, df__point2Y,
    	      &t1, &t2) ;

      if ((t1 <= 0 && t2 <= 0) || (t1 >= 1 && t2 >= 1))
      {
        return FALSE ;
      }
      if ((t1 <= 0 && t2 >= 1) || (t1 >= 1 && t2 <= 0))
      {
        object_zap (TRUE, COPY_PUT);
        usun_obiekt (ptr__object );
        return TRUE;
      }
      adb = Parent_Block((char  *)L);
      s__L1.blok = NoElemBlok ;
      s__L2.blok = NoElemBlok ;
      if (adb != NULL)
      {
        s__L1.blok = ElemBlok ;
        s__L2.blok = ElemBlok ;
      }
      object_zap (TRUE, COPY_PUT);
      if (s__L1.x1 != s__L1.x2 || s__L1.y1 != s__L1.y2)
      {
        if(NULL == (lps_line_break1 = (LINIA*)dodaj_obiekt(adb, (void*)&s__L1)))
        {
          return 0;
        }
        b__line_break1 = TRUE;
      }
      if (s__L2.x1 != s__L2.x2 || s__L2.y1 != s__L2.y2)
      {
        if (adb != NULL) adb = Parent_Block((char  *)lps_line_break1);
        if (NULL == (LINIA*)dodaj_obiekt(adb, (void*)&s__L2))
        {
          return 0;
        }
        b__line_break2 = TRUE;
      }
      if (NULL == (lps_old = Find_Object (&s__L_old)))
      {
        Internal_Error (__LINE__,__FILE__);
        return FALSE;
      }
      Set_Block_Change_Mode ((void *)lps_old, OB1CHANGE_BREAK) ;
      usun_obiekt (lps_old);
        CUR_OFF (X, Y);
        if (b__line_break1)
        {
          outline (&s__L1, COPY_PUT, 0);
        }
        if (b__line_break2)
        {
          outline (&s__L2, COPY_PUT, 0);
        }
        CUR_ON (X, Y);
      return TRUE;
}


static BOOL break_line (void)
/*-------------------------*/
{
  LINIA 	*L, *L1, *L2;
  DWIE_LINIE L_2, *L_2_;
  LINIA3D 	*L3D;
  LINIA3D 	*lps_line_break13D ;
  double 	t1, t2 ;
  BLOK	 	*adb;
  void 		*lps_old;
  char *ptr_pline, *ptr_parent;

  b__line_break1 = FALSE;
  b__line_break2 = FALSE;
  b__line_break3 = FALSE;

  L = (LINIA*)ptr__object  ;
       memcpy ((void *)&s__L_old, (void *)L, sizeof (LINIA)) ;
       Break_Line (L, &s__L1, &s__L2, &s__L3,
	         df__point1X, df__point1Y,
	         df__point2X, df__point2Y,
    	      &t1, &t2) ;

      if ((t1 <= 0 && t2 <= 0) || (t1 >= 1 && t2 >= 1))
      {
        CUR_OFF (X, Y);
        CUR_ON (X, Y);
        return FALSE ;
      }
      if ((t1 <= 0 && t2 >= 1) || (t1 >= 1 && t2 <= 0))
      {
        object_zap (TRUE, COPY_PUT);
        rysuj_obiekt((char*)ptr__object, COPY_PUT, 0);
        usun_obiekt (ptr__object );
        CUR_OFF (X, Y);
        CUR_ON (X, Y);
        return TRUE;
      }
      //zamiana linii na dwie linie
      //przerysowanie linii kolorem tla
      rysuj_obiekt((char*)ptr__object,COPY_PUT,0);
      if (((Check_if_Equal(s__L1.x1,s__L1.x2)==FALSE) || (Check_if_Equal(s__L1.y1,s__L1.y2)==FALSE)) &&
          ((Check_if_Equal(s__L2.x1,s__L2.x2)==FALSE) || (Check_if_Equal(s__L2.y1,s__L2.y2)==FALSE)))
          //dwie linie
      {
        rysuj_obiekt((char *) &s__L1, COPY_PUT, 1);
        rysuj_obiekt((char *) &s__L2, COPY_PUT, 1);

        //utworzenie dwu_linii
        memmove(&L_2, &s__L1, sizeof(NAGLOWEK) + s__L1.n);
        L_2.n=48;
        L_2_=&L_2;
        memcpy((void *)((char *)L_2_ + sizeof(NAGLOWEK) + s__L1.n), &s__L2, sizeof(NAGLOWEK) + s__L2.n);
        L_2_=(DWIE_LINIE*)korekta_obiekt((void *)ptr__object,(void*)&L_2);
        //modyfikacja dwu-linii na dwie linie
        L1=(LINIA *)L_2_;
        L1->n=20;

        ptr_pline= FIRSTB(L_2_);

          b__line_break1 = TRUE;
          b__line_break2 = TRUE;
      }
       else if ((Check_if_Equal(s__L1.x1,s__L1.x2)==FALSE) || (Check_if_Equal(s__L1.y1,s__L1.y2)==FALSE))
        {
          rysuj_obiekt((char *)& s__L1, COPY_PUT, 1);
          L2=(LINIA*)korekta_obiekt((void *)ptr__object,(void*)&s__L1);
          ptr_pline= FIRSTB(L2);

            b__line_break1 = TRUE;
        }
         else if ((Check_if_Equal(s__L2.x1,s__L2.x2)==FALSE) || (Check_if_Equal(s__L2.y1,s__L2.y2)==FALSE))
          {
            rysuj_obiekt((char *)& s__L2, COPY_PUT, 1);
            L2=(LINIA*)korekta_obiekt((void *)ptr__object,(void*)&s__L2);
            ptr_pline= FIRSTB(L2);

              b__line_break2 = TRUE;
          }
           else
            {
              ptr_pline = FIRSTB(ptr__object);
            }

     //adding cut part
     if (break_and_leave==TRUE) {
         if ((Check_if_Equal(s__L3.x1, s__L3.x2) == FALSE) || (Check_if_Equal(s__L3.y1, s__L3.y2) == FALSE)) {
             if (ptr_pline != NULL) {
                 if ((((BLOK *) ptr_pline)->kod_obiektu == B_PLINE) &&
                     ((((BLOK *) ptr_pline)->opis_obiektu[0] == PL_PLINE) ||
                      (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_ELLIPSE) ||
                      (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_POLYGON) ||
                      (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_RECTANGLE)
                     )) {
                     ptr_parent = FIRSTB(ptr_pline);
                 } else ptr_parent = ptr_pline;
             } else ptr_parent = NULL;


             dodaj_obiekt(ptr_parent, &s__L3);
             rysuj_obiekt((char *) &s__L3, COPY_PUT, 1);

             b__line_break3 = TRUE;

         }
     }

      CUR_OFF (X, Y);
      CUR_ON (X, Y);
      return TRUE;

}


static BOOL break_line_wire (void)
/*-------------------------------------*/
{
  LINIA 	*L;
  double 	t1, t2 ;
  char          sz_tmp [100] ;
  int ret;

  b__line_break1 = FALSE;
  b__line_break2 = FALSE;
  L = (LINIA*)ptr__object  ;
  memcpy ((void *)&s__L_old, (void *)L, sizeof (LINIA)) ;
  Break_Line (L, &s__L1, &s__L2, &s__L3,
	      df__point1X, df__point1Y,
	      df__point2X, df__point2Y,
	      &t1, &t2) ;
  if ((t1 <= 0 && t2 <= 0) || (t1 >= 1 && t2 >= 1))
  {
    return FALSE ;
  }
  s__L1.blok = NoElemBlok ;
  s__L2.blok = NoElemBlok ;
  /*zmiana linii*/
  L->x1=s__L1.x1;
  L->y1=s__L1.y1;
  L->x2=s__L1.x2;
  L->y2=s__L1.y2;
  /*dopisanie nowej linii do bufora*/
  if (s__L2.x1 != s__L2.x2 || s__L2.y1 != s__L2.y2)
   {
    dodatkowe_linie+=1;
    dodatkowe_linie_all+=1;
    memcpy ( &LINE_TAB[dodatkowe_linie], (void *)&s__L2, sizeof (LINIA)) ;
    ret = snprintf(sz_tmp, 10, "%d",dodatkowe_linie_all);
    InfoListStr1 (2, sz_tmp) ;
   } 
  return TRUE;
}

void break_wire (void * ad, double xjunction, double yjunction)
/*------------------------------------------------------------*/
{
 df__point1X=xjunction;
 df__point1Y=yjunction;
 df__point2X=xjunction;
 df__point2Y=yjunction;
 ptr__object=ad;
 break_line_wire();
}


static BOOL break_arc (void)
/*---------------------------*/
{
  LUK * l;
  DWA_LUKI l_2, *l_2_;
  LUK *l1;
  LINIA *L1;
  LUK s__l0;
  double l0_x1, l0_y1, l0_x2, l0_y2;
  double l1_x1, l1_y1, l1_x2, l1_y2;
  double l2_x1, l2_y1, l2_x2, l2_y2;
  char *ptr_pline, *ptr_parent;

  b__arc_break1 = FALSE;
  b__arc_break2 = FALSE;
  b__arc_break3 = FALSE;
  l = (LUK*)ptr__object  ;
  memcpy ((void *)&s__l_old, (void *)l, sizeof (LUK));
  Break_Arc (l, &s__l1, &s__l2, &s__l3,
	     df__point1X, df__point1Y,
	     df__point2X, df__point2Y) ;
  //przerysowanie luku kolorem tla
  rysuj_obiekt((char*)ptr__object,COPY_PUT,0);
  if ((fabs (s__l1.kat1 - s__l1.kat2) <= OZero) && (fabs (s__l2.kat1 - s__l2.kat2) <= OZero))
   {
    usun_obiekt (ptr__object);
       CUR_OFF (X, Y);
       CUR_ON (X, Y);
    return 0;
   }
  
  if ((fabs (s__l1.kat1 - s__l1.kat2) > OZero) && (fabs (s__l2.kat1 - s__l2.kat2) > OZero))
   {
    //dodatkowa analiza kolejnosci lukow
    //wiemy ze jest to polilinia, a wiec przed i za lukiem l mozna spodziewac sie
    //rowniez luku lub linii
    //wyznaczmy najpierw punkty koncowe lukow
     l1_x1=s__l1.x+s__l1.r*cos(s__l1.kat1);
     l1_y1=s__l1.y+s__l1.r*sin(s__l1.kat1);
     l1_x2=s__l1.x+s__l1.r*cos(s__l1.kat2);
     l1_y2=s__l1.y+s__l1.r*sin(s__l1.kat2);

     l2_x1=s__l2.x+s__l2.r*cos(s__l2.kat1);
     l2_y1=s__l2.y+s__l2.r*sin(s__l2.kat1);
     l2_x2=s__l2.x+s__l2.r*cos(s__l2.kat2);
     l2_y2=s__l2.y+s__l2.r*sin(s__l2.kat2);
    //
    //teraz poszukajmy linii nastepujacej po luku
    L1=(LINIA *) ((char*)ptr__object+sizeof(NAGLOWEK)+l->n);  //win32 (char*)
    if (L1->obiekt==Olinia)
     {
      l0_x2=L1->x1;
      l0_y2=L1->y1;
      //sprawdzenie czy pierwszy luk ma punkt wspolny z nastepujaca linia
      if (((Check_if_Equal(l1_x1,l0_x2)==TRUE) && (Check_if_Equal(l1_y1,l0_y2)==TRUE)) ||
          ((Check_if_Equal(l1_x2,l0_x2)==TRUE) && (Check_if_Equal(l1_y2,l0_y2)==TRUE)))
       {  //to wystarczajacy powod zeby zamienic linie
         memcpy ((void *)&s__l0, (void *)&s__l1, sizeof (LUK));
         memcpy ((void *)&s__l1, (void *)&s__l2, sizeof (LUK));
         memcpy ((void *)&s__l2, (void *)&s__l0, sizeof (LUK));
       }
     }
      else if (L1->obiekt==Oluk)
       {
         l1=(LUK *) ((char*)ptr__object+sizeof(NAGLOWEK)+l->n);  //win32 (char*)
         l0_x1=l1->x+l1->r*cos(l1->kat1);
         l0_y1=l1->y+l1->r*sin(l1->kat1);
         l0_x2=l1->x+l1->r*cos(l1->kat2);
         l0_y2=l1->y+l1->r*sin(l1->kat2);
         //sprawdzenie czy pierwszy luk ma punkt wspolny z nastepujacym lukiem
         if (((Check_if_Equal(l1_x1,l0_x2)==TRUE) && (Check_if_Equal(l1_y1,l0_y2)==TRUE)) ||
             ((Check_if_Equal(l1_x1,l0_x1)==TRUE) && (Check_if_Equal(l1_y1,l0_y1)==TRUE)))
          {  //to wystarczajacy powod zeby zamienic linie
            memcpy ((void *)&s__l0, (void *)&s__l1, sizeof (LUK));
            memcpy ((void *)&s__l1, (void *)&s__l2, sizeof (LUK));
            memcpy ((void *)&s__l2, (void *)&s__l0, sizeof (LUK));
          }
           else if (((Check_if_Equal(l1_x2,l0_x2)==TRUE) && (Check_if_Equal(l1_y2,l0_y2)==TRUE)) ||
                    ((Check_if_Equal(l1_x2,l0_x1)==TRUE) && (Check_if_Equal(l1_y2,l0_y1)==TRUE)))
             {  //to rowniez wystarczajacy powod zeby zamienic linie
               memcpy ((void *)&s__l0, (void *)&s__l1, sizeof (LUK));
               memcpy ((void *)&s__l1, (void *)&s__l2, sizeof (LUK));
               memcpy ((void *)&s__l2, (void *)&s__l0, sizeof (LUK));
             }
       }
        else //na razie nie znaleziono nic po luku, poszukajmy przed lukiem
         {
           L1=(LINIA *) ((char*)ptr__object-sizeof(NAGLOWEK)-28);  //moze to linia   //win32 (char*)
           if ((L1->obiekt==Olinia) && (L1->n==20))  //to jest napewno linia
            {
              l0_x1=L1->x2;
              l0_y1=L1->y2;
              //sprawdzenie czy drugi luk ma punkt wspolny z nastepujaca linia
              if (((Check_if_Equal(l2_x1,l0_x1)==TRUE) && (Check_if_Equal(l2_y1,l0_y1)==TRUE)) ||
                  ((Check_if_Equal(l2_x2,l0_x1)==TRUE) && (Check_if_Equal(l2_y2,l0_y1)==TRUE)))
               {  //to wystarczajacy powod zeby zamienic linie
                memcpy ((void *)&s__l0, (void *)&s__l1, sizeof (LUK));
                memcpy ((void *)&s__l1, (void *)&s__l2, sizeof (LUK));
                memcpy ((void *)&s__l2, (void *)&s__l0, sizeof (LUK));
               }
            }
            else
             {
              l1=(LUK *) ((char*)ptr__object-sizeof(NAGLOWEK)-32);  //moze to luk  //win32 (char*)
              if ((l1->obiekt==Oluk) && (l1->n==24))  //to jest napewno luk
               {
                 l0_x1=l1->x+l1->r*cos(l1->kat1);
                 l0_y1=l1->y+l1->r*sin(l1->kat1);
                 l0_x2=l1->x+l1->r*cos(l1->kat2);
                 l0_y2=l1->y+l1->r*sin(l1->kat2);
                 //sprawdzenie czy drugi luk ma punkt wspolny z nastepujacym lukiem
                 if (((Check_if_Equal(l2_x1,l0_x2)==TRUE) && (Check_if_Equal(l2_y1,l0_y2)==TRUE)) ||
                     ((Check_if_Equal(l2_x1,l0_x1)==TRUE) && (Check_if_Equal(l2_y1,l0_y1)==TRUE)))
                  {  //to wystarczajacy powod zeby zamienic linie
                    memcpy ((void *)&s__l0, (void *)&s__l1, sizeof (LUK));
                    memcpy ((void *)&s__l1, (void *)&s__l2, sizeof (LUK));
                    memcpy ((void *)&s__l2, (void *)&s__l0, sizeof (LUK));
                  }
                   else if (((Check_if_Equal(l2_x2,l0_x2)==TRUE) && (Check_if_Equal(l2_y2,l0_y2)==TRUE)) ||
                            ((Check_if_Equal(l2_x2,l0_x1)==TRUE) && (Check_if_Equal(l2_y2,l0_y1)==TRUE)))
                     {  //to rowniez wystarczajacy powod zeby zamienic linie
                       memcpy ((void *)&s__l0, (void *)&s__l1, sizeof (LUK));
                       memcpy ((void *)&s__l1, (void *)&s__l2, sizeof (LUK));
                       memcpy ((void *)&s__l2, (void *)&s__l0, sizeof (LUK));
                     }
               }
             }
         }

    rysuj_obiekt((char *)& s__l1, COPY_PUT, 1);
    rysuj_obiekt((char *) & s__l2, COPY_PUT, 1);

    memmove(&l_2, &s__l1, sizeof(NAGLOWEK) + s__l1.n);
    l_2.n=2*(sizeof(NAGLOWEK)+24)-sizeof(NAGLOWEK); //56;
    l_2_=&l_2;

   memcpy((void *)((char *)l_2_ + sizeof(NAGLOWEK) + s__l1.n), &s__l2, sizeof(NAGLOWEK) + s__l2.n);

    l_2_=(DWA_LUKI*)korekta_obiekt((void *)ptr__object,(void*)&l_2);

       l_2_->shadowed=0;
       l_2_->rezerwa=0;
       l_2_->shadowed_=0;
       l_2_->rezerwa_=0;
    //modyfikacja dwu-luku na dwa luki
    l1=(LUK *)l_2_;
    l1->n=24;

    ptr_pline= FIRSTB(l_2_);
    b__arc_break1 = TRUE;
    b__arc_break2 = TRUE;
   }
   else if (fabs (s__l1.kat1 - s__l1.kat2) > OZero)
    {
      rysuj_obiekt((char *)& s__l1, COPY_PUT, 1);
      l->shadowed=0;
      l->rezerwa=0;
      l->x=s__l1.x;
      l->y=s__l1.y;
      l->r=s__l1.r;
      l->kat1=s__l1.kat1;
      l->kat2=s__l1.kat2;

      ptr_pline= FIRSTB(l);
      b__arc_break1 = TRUE;
    }
    else if (fabs (s__l2.kat1 - s__l2.kat2) > OZero)
    {
      rysuj_obiekt((char *)& s__l2, COPY_PUT, 1);
      l->shadowed=0;
      l->rezerwa=0;
      l->x=s__l2.x;
      l->y=s__l2.y;
      l->r=s__l2.r;
      l->kat1=s__l2.kat1;
      l->kat2=s__l2.kat2;

      ptr_pline= FIRSTB(l);
      b__arc_break2 = TRUE;
    }
      else
      {
          ptr_pline = FIRSTB(ptr__object);
      }

    //adding cut part
    if (break_and_leave==TRUE) {
        if (Check_if_Equal(s__l3.kat1, s__l3.kat2) == FALSE) {
            if (ptr_pline != NULL) {
                if ((((BLOK *) ptr_pline)->kod_obiektu == B_PLINE) &&
                    ((((BLOK *) ptr_pline)->opis_obiektu[0] == PL_PLINE) ||
                     (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_ELLIPSE) ||
                     (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_POLYGON) ||
                     (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_RECTANGLE)
                    )) {
                    ptr_parent = FIRSTB(ptr_pline);
                } else ptr_parent = ptr_pline;
            } else ptr_parent = NULL;

            s__l3.shadowed=0;
            s__l3.rezerwa=0;
            dodaj_obiekt(ptr_parent, &s__l3);
            rysuj_obiekt((char *) &s__l3, COPY_PUT, 1);

            b__arc_break3 = TRUE;
        }
    }


    CUR_OFF (X, Y);
    CUR_ON (X, Y);
  return TRUE;
}


static BOOL break_ellipticalarc (void)
/*---------------------------*/
{
    ELLIPTICALARC * el;
    TWO_ELLIPTICAL_ARCS el_2, *el_2_;
    ELLIPTICALARC *el1;
    LINIA *L1;
    ELLIPTICALARC s__el0;
    double el0_x1, el0_y1, el0_x2, el0_y2;
    double el1_x1, el1_y1, el1_x2, el1_y2;
    double el2_x1, el2_y1, el2_x2, el2_y2;
    char *ptr_pline, *ptr_parent;

    b__ellipticalarc_break1 = FALSE;
    b__ellipticalarc_break2 = FALSE;
    b__ellipticalarc_break3 = FALSE;
    el = (ELLIPTICALARC*)ptr__object  ;
    memcpy ((void *)&s__el_old, (void *)el, sizeof (ELLIPTICALARC));
    Break_Elliptical_Arc (el, &s__el1, &s__el2, &s__el3,
               df__point1X, df__point1Y,
               df__point2X, df__point2Y) ;
    //przerysowanie luku kolorem tla
    rysuj_obiekt((char*)ptr__object,COPY_PUT,0);
    if ((fabs (s__el1.kat1 - s__el1.kat2) <= OZero) && (fabs (s__el2.kat1 - s__el2.kat2) <= OZero))
    {
        usun_obiekt (ptr__object);
        CUR_OFF (X, Y);
        CUR_ON (X, Y);
        return 0;
    }

    if ((fabs (s__el1.kat1 - s__el1.kat2) > OZero) && (fabs (s__el2.kat1 - s__el2.kat2) > OZero))
    {
        //dodatkowa analiza kolejnosci lukow
        //wiemy ze jest to polilinia, a wiec przed i za lukiem l mozna spodziewac sie
        //rowniez luku lub linii
        //wyznaczmy najpierw punkty koncowe lukow

        rysuj_obiekt((char *)& s__el1, COPY_PUT, 1);
        rysuj_obiekt((char *)& s__el2, COPY_PUT, 1);

        memmove(&el_2, &s__el1, sizeof(NAGLOWEK) + s__el1.n);
        el_2.n=2*(sizeof(NAGLOWEK)+32)-sizeof(NAGLOWEK);
        el_2_=&el_2;

        memcpy((void *)((char *)el_2_ + sizeof(NAGLOWEK) + s__el1.n), &s__el2, sizeof(NAGLOWEK) + s__el2.n);

        el_2_=(TWO_ELLIPTICAL_ARCS*)korekta_obiekt((void *)ptr__object,(void*)&el_2);

        el_2_->shadowed=0;
        el_2_->rezerwa=0;
        el_2_->shadowed_=0;
        el_2_->rezerwa_=0;

        //modyfikacja dwu-luku na dwa luki
        el1=(ELLIPTICALARC *)el_2_;
        el1->n=32;

        ptr_pline= FIRSTB(el_2_);
        b__ellipticalarc_break1 = TRUE;
        b__ellipticalarc_break2 = TRUE;
    }
    else if (fabs (s__el1.kat1 - s__el1.kat2) > OZero)
    {
        rysuj_obiekt((char *)& s__el1, COPY_PUT, 1);
        el->shadowed=0;
        el->rezerwa=0;
        el->x=s__el1.x;
        el->y=s__el1.y;
        el->rx=s__el1.rx;
        el->ry=s__el1.ry;
        el->angle=s__el1.angle;
        el->kat1=s__el1.kat1;
        el->kat2=s__el1.kat2;

        ptr_pline= FIRSTB(el);
        b__ellipticalarc_break1 = TRUE;
    }
    else if (fabs (s__el2.kat1 - s__el2.kat2) > OZero)
    {
        rysuj_obiekt((char *)& s__el2, COPY_PUT, 1);
        el->shadowed=0;
        el->rezerwa=0;
        el->x=s__el2.x;
        el->y=s__el2.y;
        el->rx=s__el2.rx;
        el->ry=s__el2.ry;
        el->angle=s__el2.angle;
        el->kat1=s__el2.kat1;
        el->kat2=s__el2.kat2;

        ptr_pline= FIRSTB(el);
        b__ellipticalarc_break2 = TRUE;
    }
    else
    {
        ptr_pline = FIRSTB(ptr__object);
    }
    //adding cut part
    if (break_and_leave==TRUE) {
        if (Check_if_Equal(s__el3.kat1, s__el3.kat2) == FALSE) {
            if (ptr_pline != NULL) {
                if ((((BLOK *) ptr_pline)->kod_obiektu == B_PLINE) &&
                    ((((BLOK *) ptr_pline)->opis_obiektu[0] == PL_PLINE) ||
                     (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_ELLIPSE) ||
                     (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_POLYGON) ||
                     (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_RECTANGLE)
                    )) {
                    ptr_parent = FIRSTB(ptr_pline);
                } else ptr_parent = ptr_pline;
            } else ptr_parent = NULL;

            s__el3.shadowed=0;
            s__el3.rezerwa=0;
            dodaj_obiekt(ptr_parent, &s__el3);
            rysuj_obiekt((char *) &s__el3, COPY_PUT, 1);

            b__ellipticalarc_break3 = TRUE;
        }
    }

    CUR_OFF (X, Y);
    CUR_ON (X, Y);
    return TRUE;
}


static BOOL break_circle (void)
/*---------------------------*/
{
  double x1, y1, x2, y2;
  double k1, k2;
  OKRAG * o;
  BLOK *adb;
  void * lps_old;
  char *ptr_pline=NULL, *ptr_parent;

  if (fabs (df__point1X - df__point2X) < OZero   && fabs (df__point1Y - df__point2Y) < OZero)
  {
    ErrList (52);
    return FALSE;
  }
  b__circle_break1 = FALSE;
  b__circle_break2 = FALSE;
  o = (OKRAG*)ptr__object  ;
  memcpy ((void *)&s__o_old , (void *)o, sizeof (OKRAG));
  memcpy ((void *)&s__l1, (void *)o, sizeof (NAGLOWEK) - 4 /*2*/);
  memcpy ((void *)&s__l2, (void *)o, sizeof (NAGLOWEK) - 4 /*2*/);
  s__l1.obiekt = Oluk;
  s__l1.warstwa = o->warstwa;
  s__l1.kolor = o->kolor;
  s__l1.typ = o->typ;
  s__l1.shadowed=0;
  s__l1.rezerwa=0;
  s__l1.x = o->x;
  s__l1.y = o->y;
  s__l1.r = o->r;

  s__l2.obiekt = Oluk;
  s__l2.warstwa = o->warstwa;
  s__l2.kolor = o->kolor;
  s__l2.typ = o->typ;
  s__l2.shadowed=0;
  s__l2.rezerwa=0;
  s__l2.x = o->x;
  s__l2.y = o->y;
  s__l2.r = o->r;

  x1 = df__point1X ;
  y1 = df__point1Y ;
  najblizszyO (&x1, &y1, (void *)o);
  k1 = Atan2 (y1 - o->y, x1 - o->x);
  x2 = df__point2X ;
  y2 = df__point2Y ;
  najblizszyO (&x2, &y2, (void *)o);
  k2 = Atan2 (y2 - o->y, x2 - o->x);
  k1 = Angle_Normal (k1);
  k2 = Angle_Normal (k2);
  s__l1.kat1 = k2;
  s__l1.kat2 = k1;

  s__l2.kat1 = k1;
  s__l2.kat2 = k2;

  adb = Parent_Block((char  *)o);
  s__l1.blok = NoElemBlok ;
  s__l2.blok = NoElemBlok ;
  if (adb != NULL)
  {
    s__l1.blok = ElemBlok ;
  }
  object_zap (TRUE, COPY_PUT);
  if (fabs (s__l1.kat1 - s__l1.kat2) > OZero)
  {
    if(NULL == (LUK*)dodaj_obiekt(adb, (void*)&s__l1))
    {
        CUR_OFF (X, Y);
        CUR_ON (X, Y);
      return 0;
    }
    b__circle_break1 = TRUE;
  }
  if (NULL == (lps_old = Find_Object (&s__o_old)))
  {
      CUR_OFF (X, Y);
      CUR_ON (X, Y);
    Internal_Error (__LINE__,__FILE__);
    return FALSE;
  }
  Set_Block_Change_Mode ((void *)lps_old, OB1CHANGE_BREAK) ;
  rysuj_obiekt((char*)lps_old,COPY_PUT,0);
  usun_obiekt (lps_old);
  if (b__circle_break1)
  {
    rysuj_obiekt ((char *)&s__l1, COPY_PUT, 1) ;
    ptr_pline= FIRSTB(&s__l1);
  }

    //adding cut part
    if (break_and_leave==TRUE) {
        if (Check_if_Equal(s__l2.kat1, s__l2.kat2) == FALSE) {
            if (ptr_pline != NULL) {
                if ((((BLOK *) ptr_pline)->kod_obiektu == B_PLINE) &&
                    ((((BLOK *) ptr_pline)->opis_obiektu[0] == PL_PLINE) ||
                     (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_ELLIPSE) ||
                     (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_POLYGON) ||
                     (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_RECTANGLE)
                    )) {
                    ptr_parent = FIRSTB(ptr_pline);
                } else ptr_parent = ptr_pline;
            } else ptr_parent = NULL;

            dodaj_obiekt(ptr_parent, &s__l2);
            rysuj_obiekt((char *) &s__l2, COPY_PUT, 1);

            b__circle_break2 = TRUE;
        }
    }

  CUR_OFF (X, Y);
  CUR_ON (X, Y);
  return TRUE;
}

static BOOL break_ellipse (void)
/*---------------------------*/
{
    double x1, y1, x2, y2;
    double k1, k2;
    ELLIPSE * e;
    BLOK *adb;
    void * lps_old;
    char *ptr_pline=NULL, *ptr_parent;

    if (fabs (df__point1X - df__point2X) < OZero   && fabs (df__point1Y - df__point2Y) < OZero)
    {
        ErrList (52);
        return FALSE;
    }
    b__ellipse_break1 = FALSE;
    b__ellipse_break2 = FALSE;
    e = (ELLIPSE*)ptr__object  ;
    memcpy ((void *)&s__e_old , (void *)e, sizeof (ELLIPSE));
    memcpy ((void *)&s__el1, (void *)e, sizeof (NAGLOWEK) - 4 /*2*/);
    memcpy ((void *)&s__el2, (void *)e, sizeof (NAGLOWEK) - 4 /*2*/);
    s__el1.obiekt = Oellipticalarc;
    s__el1.warstwa = e->warstwa;
    s__el1.kolor = e->kolor;
    s__el1.typ = e->typ;
    s__el1.shadowed=0;
    s__el1.rezerwa=0;
    s__el1.x = e->x;
    s__el1.y = e->y;
    s__el1.rx = e->rx;
    s__el1.ry = e->ry;
    s__el1.angle = e->angle;

    s__el2.obiekt = Oellipticalarc;
    s__el2.warstwa = e->warstwa;
    s__el2.kolor = e->kolor;
    s__el2.typ = e->typ;
    s__el2.shadowed=0;
    s__el2.rezerwa=0;
    s__el2.x = e->x;
    s__el2.y = e->y;
    s__el2.rx = e->rx;
    s__el2.ry = e->ry;
    s__el2.angle = e->angle;

    x1 = df__point1X ;
    y1 = df__point1Y ;
    najblizszyE (&x1, &y1, (void *)e);
    k1 = Atan2 (y1 - e->y, x1 - e->x);
    x2 = df__point2X ;
    y2 = df__point2Y ;
    najblizszyE (&x2, &y2, (void *)e);
    k2 = Atan2 (y2 - e->y, x2 - e->x);
    k1 = Angle_Normal (k1);
    k2 = Angle_Normal (k2);
    s__el1.kat1 = k2 - e->angle;
    s__el1.kat2 = k1 - e->angle;

    s__el2.kat1 = k1 - e->angle;
    s__el2.kat2 = k2 - e->angle;

    s__el1.kat1 = Angle_Normal (s__el1.kat1);
    s__el1.kat2 = Angle_Normal (s__el1.kat2);

    s__el2.kat1 = Angle_Normal (s__el2.kat1);
    s__el2.kat2 = Angle_Normal (s__el2.kat2);

    adb = Parent_Block((char  *)e);
    s__el1.blok = NoElemBlok ;
    s__el2.blok = NoElemBlok ;
    if (adb != NULL)
    {
        s__el1.blok = ElemBlok ;
    }
    object_zap (TRUE, COPY_PUT);
    if (fabs (s__el1.kat1 - s__el1.kat2) > OZero)
    {
        if(NULL == (ELLIPTICALARC *)dodaj_obiekt(adb, (void*)&s__el1))
        {
            CUR_OFF (X, Y);
            CUR_ON (X, Y);
            return 0;
        }
        b__ellipse_break1 = TRUE;
    }
    if (NULL == (lps_old = Find_Object (&s__e_old)))
    {
        CUR_OFF (X, Y);
        CUR_ON (X, Y);
        Internal_Error (__LINE__,__FILE__);
        return FALSE;
    }
    Set_Block_Change_Mode ((void *)lps_old, OB1CHANGE_BREAK) ;
    rysuj_obiekt((char*)lps_old,COPY_PUT,0);
    usun_obiekt (lps_old);
    if (b__ellipse_break1)
    {
        rysuj_obiekt ((char *)&s__el1, COPY_PUT, 1) ;
        ptr_pline= FIRSTB(&s__el1);
    }

    //adding cut part
    if (break_and_leave==TRUE) {
        if (Check_if_Equal(s__el2.kat1, s__el2.kat2) == FALSE) {
            if (ptr_pline != NULL) {
                if ((((BLOK *) ptr_pline)->kod_obiektu == B_PLINE) &&
                    ((((BLOK *) ptr_pline)->opis_obiektu[0] == PL_PLINE) ||
                     (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_ELLIPSE) ||
                     (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_POLYGON) ||
                     (((BLOK *) ptr_pline)->opis_obiektu[0] == PL_RECTANGLE)
                    )) {
                    ptr_parent = FIRSTB(ptr_pline);
                } else ptr_parent = ptr_pline;
            } else ptr_parent = NULL;

            s__el2.shadowed=0;
            s__el2.rezerwa=0;

            dodaj_obiekt(ptr_parent, &s__el2);
            rysuj_obiekt((char *) &s__el2, COPY_PUT, 1);

            b__ellipse_break2 = TRUE;
        }
    }

    CUR_OFF (X, Y);
    CUR_ON (X, Y);
    return TRUE;
}

static void redrawb(void)
/*--------------------*/
{
  char  *ptr_ob_begin,  *ptr_ob_end ;

  ptr_ob_begin = ptr_ob_end = ptr__object ;
  if (b__pline == TRUE)
  {
    ptr_ob_begin = (char  *)PTR__GTMP3 ;
    ptr_ob_end = ptr_ob_begin + sizeof (NAGLOWEK) +
		((BLOK*)ptr__object)->n - 1 ;
  }
  blokzap (ptr_ob_begin, ptr_ob_end, Ablok, XOR_PUT, 1) ;
}

static void  nooop(void)
{}

static void object_zap (int zap, int mode)
/*----------------------------------------*/
{
  static void (*REDdod)(void)=nooop ;
  char  *ptr_ob_begin,  *ptr_ob_end ;

  ptr_ob_begin = ptr_ob_end = ptr__object ;
  if (b__pline == TRUE)
  {
    ptr_ob_begin = PTR__GTMP3 ;
    ptr_ob_end = ptr_ob_begin + sizeof (NAGLOWEK) +
		((BLOK*)ptr_ob_begin)->n - 1 ;
  }
  if (zap == TRUE )
  {
    zmien_atrybut (ptr_ob_begin, ptr_ob_end, Anormalny, Ablok) ;
    if (mode == XOR_PUT)
    {
      REDdod = REDDOD; REDDOD = redrawb ;
    }
  }
  blokzap (ptr_ob_begin, ptr_ob_end, Ablok, mode, 1) ;
  CUR_OFF(X, Y);
  CUR_ON (X, Y) ;
  if (zap == FALSE && mode == XOR_PUT)
  {
    zmien_atrybut (ptr_ob_begin, ptr_ob_end, Ablok, Anormalny) ;
    REDDOD = REDdod;
  }
  if (zap == -1 && mode == OR_PUT)
  {
	  zmien_atrybut(ptr_ob_begin, ptr_ob_end, Ablok, Ausuniety);
	  REDDOD = REDdod;
  }
  if (zap == TRUE && mode == COPY_PUT)
  {
    zmien_atrybut (ptr_ob_begin, ptr_ob_end, Ablok, Anormalny) ;
  }
}

void break_leave(void)
{
    break_and_leave=TRUE;
    menu_par_new	((*mBreak.pola)[2].txt,	_break_and_leave_);
}

void break_cut(void)
{
    break_and_leave=FALSE;
    menu_par_new	((*mBreak.pola)[2].txt,	_break_and_cut_);
}

static BOOL obiect_to_break (void **ptr_ob_pl)
/*------------------------------------------*/
{
  EVENT *ev;
  double X0, Y0;
  BLOK *ptrs_temp ;
  BLOK *ptrs_pl;
  BLOK* ptrs_b = NULL;
  char* buff_ptrs_b;
  long_long l_offb, l_offe;
  long_long ptrc_b_offs;
  long_long ptrs_pl_n;

   komunikat0(46);
  u__last_type_object = u__type_object;
  while(1)
  {
    ev=Get_Event_Point (NULL, &X0, &Y0);
    if ( ev->What == evKeyDown  && ev->Number== 0 )
    {
      return FALSE;
    }
    if( ev->What==evCommandP)
    {
      if (ev->Number == UNDO_FUN)
      {
	    undo_break ();
      }
      else if (ev->Number == BREAK_AND_LEAVE) break_leave();
      else if (ev->Number == BREAK_AND_CUT) break_cut();
      continue;
    }
    if ( ev->What == evKeyDown  && ev->Number == ENTER)
    {
      u__type_object = Blinia  | Bluk | Bokrag | Bspline | Bellipse | Bellipticalarc;
      if (NULL != (ptr__object  = select_w (&u__type_object, NULL)))
      {
	   if (((NAGLOWEK*)ptr__object)->obiekt==Ospline)
		   {
		     //generatine spline shadow(s)
		     ((NAGLOWEK*)ptr__object)->atrybut = Ablok;
		     l_offb = (long_long)ptr__object - (long_long)dane;
			 l_offe = l_offb + sizeof(SPLINE) - 1;

			 ptrs_pl = (BLOK*)check_if_pline(ptr__object);
			 if (ptrs_pl != NULL)
			 {
				 ptr__object = ptrs_pl;
			 }

             l_offb = (long_long)ptr__object;
             l_offe = l_offb + sizeof(NAGLOWEK) + ((NAGLOWEK*)ptr__object)->n - 1;
             zmien_atrybut((char*)l_offb, (char*)l_offe, Anormalny, Ablok);

            // rysuj_obiekt(ptr__object, COPY_PUT, 0);

			 l_offb -= (long_long)dane;
			 l_offe -= (long_long)dane;

             if (((NAGLOWEK*)ptr__object)->blok == 1)
             {
                 ptrs_b = Parent_Block((char*)ptr__object);
                 ptrc_b_offs = (long_long)ptrs_b - (long_long)dane;
             }

			 shadows_no = make_spline_shadows(l_offb, l_offe, Anormalny, TRUE, Abad, 0, 0);
             
             if (shadows_no > 0)
             {
                 if (ptrs_b != NULL)
                 {
                     //moving block
                     ptrs_pl_n = ((NAGLOWEK*)dane)->n + sizeof(NAGLOWEK);
                     buff_ptrs_b = malloc(ptrs_pl_n);
                     if (buff_ptrs_b != NULL)
                     {
                         memmove(buff_ptrs_b, dane, ptrs_pl_n);
                         zmien_atrybut(buff_ptrs_b, buff_ptrs_b + ptrs_pl_n - 1, Anormalny, Ablok);
                         usun_obiekt(dane);
                         ptr__object = dodaj_obiekt(ptrs_b, buff_ptrs_b);
                         ((NAGLOWEK*)ptr__object)->blok = 1;
                         obiekt_tok_all(ptr__object, (char*)ptr__object + ptrs_pl_n, &ptr__object, Blinia);
                         free(buff_ptrs_b);
                     }
                 }
                 else
                 {
                     ptrs_temp = (BLOK*)dane;
                     ptr__object = dane + sizeof(NAGLOWEK) + B3 + ptrs_temp->dlugosc_opisu_obiektu;
                 }
             }
		   }


	   b__pline = FALSE ;
	   if (((NAGLOWEK*)ptr__object)->obiektt2 == O2BlockPline &&
	       NULL != (ptrs_temp= Parent_Block ((char  *)ptr__object)) &&
	       TRUE == Check_Pline (ptrs_temp))
		{
		  u__type_object = Blinia  | Bluk ;
		  b__pline = TRUE ;
		  ptr__object = ptrs_temp ;
          *ptr_ob_pl = (char *)ptrs_temp ;
		}
			return TRUE;
      }
    }
  }
}


static BOOL break_object (void)
/*----------------------------*/
{
  BOOL ret = FALSE;

  if (FALSE == b__pline)
  {
    object_zap (FALSE, XOR_PUT);
  }
  switch (u__type_object)
  { case Blinia :
       ret = break_line ();
       break;
    case Bluk   :
       ret = break_arc ();
       break;
    case Bellipticalarc :
          ret = break_ellipticalarc ();
       break;
    case Bokrag :
       ret = break_circle ();
       break;
    case Bellipse :
          ret = break_ellipse ();
       break;
    case Blinia | Bluk :
       ret = break_pline () ;
       break ;
    default     :
       break;
  }
  if (ret == TRUE)
  {
    b__undo_break = TRUE;
  }
  CUR_OFF(X, Y);
  CUR_ON(X, Y);
  return ret;
}


static BOOL break_pline___ (void)
/*---------------------------*/
{
  long l_temp, l_off1, l_off2, l_off3 ;	/*wzgledne polozenie wskazanych obiektow w bloku*/
  double df_x1, df_y1, df_x2, df_y2 ;
  double df_xtemp, df_ytemp ;
  char  *ptrh_beg,  *ptrh_end ;
  BOOL b_close ;
  BLOK *ptrs_temp ;
  char* PTR__GTMP1_, * PTR__GTMP2_;
  char* ptr_parent;

  PTR__GTMP4 = PTR__GTMP5 = NULL ;
  l_off1 = (char *)PTR__GTMP1 - PTR__GTMP3 ;
  l_off2 = (char *)PTR__GTMP2 - PTR__GTMP3 ;
  b_close = Check_if_Close_Pline ((BLOK *)PTR__GTMP3) ;

  l_temp = sizeof(NAGLOWEK) + ((BLOK*)PTR__GTMP3)->n;

  PTR__GTMP1 = malloc(l_temp);
  PTR__GTMP2 = malloc(l_temp);
  if (b_close ==TRUE) PTR__GTMP4 = malloc(l_temp);

  if ((NULL == PTR__GTMP1) ||
      (NULL == PTR__GTMP2) ||
      ((b_close == TRUE) && (NULL == PTR__GTMP4)))
  {
    if (PTR__GTMP1 != NULL)
    {
      free(PTR__GTMP1) ;
    }
    if (PTR__GTMP2 != NULL)
    {
      free(PTR__GTMP2) ;
    }
    if (PTR__GTMP4 != NULL)
    {
        free(PTR__GTMP4);
    }
    ErrList (18) ;
    return FALSE ;
  }
 
  movmem_ ((void*)PTR__GTMP3, (void*)PTR__GTMP1, l_temp) ; /*dodaj_obiekt nie musi dobrze dzialac*/
  movmem_ ((void*)PTR__GTMP3, (void*)PTR__GTMP2, l_temp) ;
  if (b_close == TRUE)
  {
    movmem_ ((void*)PTR__GTMP3, (void*)PTR__GTMP4, l_temp) ;
  }
  if (l_off1 > l_off2)
  {
    l_temp = l_off1 ;
    l_off1 = l_off2 ;
    l_off2 = l_temp ;
    df_xtemp = df__point1X ;
    df_ytemp = df__point1Y ;
    df__point1X = df__point2X ;
    df__point1Y = df__point2Y ;
    df__point2X = df_xtemp ;
    df__point2Y = df_ytemp ;
  }
  df_x1 = df__point1X ;
  df_y1 = df__point1Y ;
  df_x2 = df__point2X ;
  df_y2 = df__point2Y ;
  if (l_off1 == l_off2)
  {
    df_x1 = df__point2X ;
    df_y1 = df__point2Y ;
    df_x2 = df__point1X ;
    df_y2 = df__point1Y ;
  }
  if (TRUE != Truncate_End_Pline ((BLOK **) &PTR__GTMP1, l_off1,
		      df__point1X, df__point1Y, df_x1, df_y1) ||
      TRUE != Truncate_Begin_Pline ((BLOK **) &PTR__GTMP2, l_off2,
		      df__point2X, df__point2Y, df_x2, df_y2) ||
      (b_close == TRUE &&
      TRUE != Truncate_End_Pline ((BLOK **)&PTR__GTMP4, l_off2,
		      df__point2X, df__point2Y, df_x2, df_y2)) ||
      (b_close == TRUE &&
      TRUE != Truncate_Begin_Pline ((BLOK **)&PTR__GTMP4, l_off1,
		      df__point1X, df__point1Y, df_x1, df_y1)))
  {
    free (PTR__GTMP1) ;
    free (PTR__GTMP2) ;
    if (b_close == TRUE)
    {
      free (PTR__GTMP4) ;
    }
    return FALSE ;
  }
  if (TRUE == b_close)
  {
    if (NULL == (ptrs_temp = Pline_Cat ((BLOK *)PTR__GTMP2,
						(BLOK **)&PTR__GTMP1)))
    {
      free (PTR__GTMP1) ;
      free (PTR__GTMP2) ;
      free (PTR__GTMP4) ;
      return FALSE ;
    }
    PTR__GTMP2 = (char *)ptrs_temp ;
    free (PTR__GTMP1) ;
    PTR__GTMP1 = PTR__GTMP2 ;
    PTR__GTMP2 = NULL ;
    if (Get_Len_Pline ((BLOK *)PTR__GTMP1) >
	Get_Len_Pline ((BLOK *)PTR__GTMP4))
    {
      free (PTR__GTMP4) ;
      PTR__GTMP4 = NULL ;
    }
    else
    {
      free (PTR__GTMP1) ;
      PTR__GTMP1 = PTR__GTMP4 ;
      PTR__GTMP4 = NULL ;
    }
  }
  object_zap (FALSE, XOR_PUT) ;
  CUR_OFF (X, Y);
  ptrh_beg = PTR__GTMP3 ;
  ptrh_end = ptrh_beg + sizeof (NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1 ;
  blokzap (ptrh_beg, ptrh_end, Anormalny, COPY_PUT, 0) ;
  

  l_off1 = (char*)PTR__GTMP1 - dane;
  l_off2 = (char*)PTR__GTMP2 - dane;

  ptr_parent = FIRSTB(PTR__GTMP3);
  if (ptr_parent!=NULL) l_off3 = (char*)ptr_parent - dane;

  zmien_atrybut(ptrh_beg, ptrh_end, Anormalny, Abad);


  if (PTR__GTMP1 != NULL)
  {
    ptrh_beg = PTR__GTMP1 ;
    ptrh_end = ptrh_beg + sizeof (NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1 ;
    blokzap (ptrh_beg, ptrh_end, Ablok, COPY_PUT, 1) ;

    if (ptr_parent != NULL)
    {
        zmien_atrybut(ptrh_beg, ptrh_end, Ablok, Anormalny);
        PTR__GTMP1_ = (char*)dodaj_obiekt((void*)ptr_parent, (void*)PTR__GTMP1);
        PTR__GTMP1 = dane + l_off1;
        ptrh_beg = PTR__GTMP1;
        ptrh_end = ptrh_beg + sizeof(NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1;
        zmien_atrybut(ptrh_beg, ptrh_end, Anormalny, Ausuniety);
        ptr_parent = dane + l_off3;
    }
    else zmien_atrybut (ptrh_beg, ptrh_end, Ablok, Anormalny) ;
  }

  if (PTR__GTMP2 != NULL)
  {

    ptrh_beg = PTR__GTMP2 ;
    ptrh_end = ptrh_beg + sizeof (NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1 ;
    blokzap (ptrh_beg, ptrh_end, Ablok, COPY_PUT, 1) ;

    if (ptr_parent != NULL)
    {
        PTR__GTMP2_ = (char*)dodaj_obiekt((void*)ptr_parent, (void*)PTR__GTMP2);
        PTR__GTMP2 = dane + l_off2;
        ptrh_beg = PTR__GTMP2;
        ptrh_end = ptrh_beg + sizeof(NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1;
        zmien_atrybut(ptrh_beg, ptrh_end, Anormalny, Ausuniety);
        ptr_parent = dane + l_off3;
    }
    else zmien_atrybut (ptrh_beg, ptrh_end, Ablok, Anormalny) ;
  }
  CUR_ON (X, Y);
  PTR__GTMP4 = PTR__GTMP1 ;
  PTR__GTMP5 = PTR__GTMP2 ;
  PTR__GTMP6 = PTR__GTMP3 ;
  b__undo_break = TRUE;
  return TRUE ;
}

void deduct_objects(void *b_src, void *b_dest)
{   char *ptrs_beg, *ptrs_end, *ptrd_beg, *ptrd_end;
    LINIA *L1, *L2;
    LUK *l1, *l2;
    BOOL found;

    ptrs_beg = (char*)b_src + sizeof(NAGLOWEK) + B3 + ((BLOK*)b_src)->dlugosc_opisu_obiektu;
    ptrs_end = (char*)b_src + sizeof(NAGLOWEK) + ((BLOK*)b_src)->n - 1;
    while (ptrs_beg<ptrs_end)
    {
        ptrd_beg = (char*)b_dest + sizeof(NAGLOWEK) + B3 + ((BLOK*)b_dest)->dlugosc_opisu_obiektu;
        ptrd_end = (char*)b_dest + sizeof(NAGLOWEK) + ((BLOK*)b_dest)->n - 1;
        found=FALSE;
        while ((ptrd_beg<ptrd_end) && found==FALSE)
        {
            if ((((NAGLOWEK*)ptrd_beg)->atrybut!=Abad) && (((NAGLOWEK*)ptrs_beg)->obiekt == ((NAGLOWEK*)ptrd_beg)->obiekt))
            {
                switch (((NAGLOWEK *) ptrs_beg)->obiekt) {
                    case Olinia:
                        L1=(LINIA*)ptrs_beg;
                        L2=(LINIA*)ptrd_beg;
                        if ((L1->x1==L2->x1) && (L1->y1==L2->y1) && (L1->x2==L2->x2) && (L1->y2==L2->y2))
                        {
                            L2->atrybut=Abad;
                            found=TRUE;
                        }
                        break;
                    case Oluk:
                        l1=(LUK*)ptrs_beg;
                        l2=(LUK*)ptrd_beg;
                        if ((l1->x==l2->x) && (l1->y==l2->y) && (l1->r==l2->r) && (l1->kat1==l2->kat1) && (l1->kat2==l2->kat2))
                        {
                            l2->atrybut=Abad;
                            found=TRUE;
                        }
                        break;
                    default:
                        break;
                }
            }
            ptrd_beg += (sizeof(NAGLOWEK) + ((NAGLOWEK*)ptrd_beg)->n);
        }

        if (found==FALSE)  //searching for partial
        {
            ptrd_beg = (char*)b_dest + sizeof(NAGLOWEK) + B3 + ((BLOK*)b_dest)->dlugosc_opisu_obiektu;
            ptrd_end = (char*)b_dest + sizeof(NAGLOWEK) + ((BLOK*)b_dest)->n - 1;
            while ((ptrd_beg<ptrd_end) && found==FALSE)
            {
                if ((((NAGLOWEK*)ptrd_beg)->atrybut!=Abad) && (((NAGLOWEK*)ptrs_beg)->obiekt == ((NAGLOWEK*)ptrd_beg)->obiekt))
                {
                    switch (((NAGLOWEK *) ptrs_beg)->obiekt) {
                        case Olinia:
                            L1=(LINIA*)ptrs_beg;
                            L2=(LINIA*)ptrd_beg;
                            if ((L1->x1==L2->x1) && (L1->y1==L2->y1)) // && (L1->x2==L2->x2) && (L1->y2==L2->y2))
                            {
                                //cutting second point
                                L2->x1=L1->x2;
                                L2->y1=L1->y2;
                                found=TRUE;
                            }
                            else if ((L1->x2==L2->x2) && (L1->y2==L2->y2)) // && (L1->x2==L2->x2) && (L1->y2==L2->y2))
                            {
                                //cutting second point
                                L2->x2=L1->x1;
                                L2->y2=L1->y1;
                                found=TRUE;
                            }
                            break;
                        case Oluk:
                            l1=(LUK*)ptrs_beg;
                            l2=(LUK*)ptrd_beg;
                            if ((l1->r==l2->r) && (l1->x==l2->x) && (l1->y==l2->y))
                            {
                                if (l2->kat2==l2->kat1) //&& (l1->kat2==l2->kat2))
                                {
                                    //cutting arc
                                    l2->kat2=l1->kat1;
                                    found=TRUE;
                                }
                                else if (l1->kat1==l2->kat1)
                                {
                                    //cutting arc
                                    l2->kat1=l1->kat2;
                                    found=TRUE;
                                }
                                else if (l1->kat2==l2->kat2)
                                {
                                    //cutting arc
                                    l2->kat2=l1->kat1;  //??? WARNING
                                    found=TRUE;
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
                ptrd_beg += (sizeof(NAGLOWEK) + ((NAGLOWEK*)ptrd_beg)->n);
            }
        }

        ptrs_beg += (sizeof(NAGLOWEK) + ((NAGLOWEK*)ptrs_beg)->n);
    }
}


BOOL break_pline(void)
/*---------------------------*/
{
    long_long l_temp, l_off1, l_off2, l_off3, l_off4;	/*wzgledne polozenie wskazanych obiektow w bloku*/
    double df_x1, df_y1, df_x2, df_y2;
    double df_xtemp, df_ytemp;
    char* ptrh_beg, * ptrh_end;
    BOOL b_close;
    BLOK* ptrs_temp;
    char* PTR__GTMP1_=NULL, * PTR__GTMP2_=NULL;
    char* ptr_parent;
    char *ptr_buffer=NULL;
    size_t buff_l;

    PTR__GTMP4 = PTR__GTMP5 = PTR__GTMP7 = NULL;
    l_off1 = (char*)PTR__GTMP1 - PTR__GTMP3;
    l_off2 = (char*)PTR__GTMP2 - PTR__GTMP3;
    b_close = Check_if_Close_Pline((BLOK*)PTR__GTMP3);
    if (break_and_leave)
    {
        buff_l=sizeof(NAGLOWEK) + ((NAGLOWEK*)PTR__GTMP3)->n;
        ptr_buffer=malloc(buff_l);
        memmove(ptr_buffer, PTR__GTMP3, buff_l);
    }
    if (NULL == (PTR__GTMP1 = (char*)dodaj_obiekt(NULL, (void*)PTR__GTMP3)) ||
        NULL == (PTR__GTMP2 = (char*)dodaj_obiekt(NULL, (void*)PTR__GTMP3)) ||
        (b_close == TRUE &&
            NULL == (PTR__GTMP4 = (char*)dodaj_obiekt(NULL, (void*)PTR__GTMP3))))
    {
        if (PTR__GTMP1 != NULL)
        {
            usun_obiekt((void*)PTR__GTMP1);
        }
        if (PTR__GTMP2 != NULL)
        {
            usun_obiekt((void*)PTR__GTMP2);
        }
        ErrList(18);
        if ((break_and_leave) && (ptr_buffer!=NULL))
        {
            free(ptr_buffer);
        }
        return FALSE;
    }
    l_temp = sizeof(NAGLOWEK) + ((BLOK*)PTR__GTMP3)->n;
    movmem_((void*)PTR__GTMP3, (void*)PTR__GTMP1, l_temp); /*dodaj_obiekt nie musi dobrze dzialac*/
    movmem_((void*)PTR__GTMP3, (void*)PTR__GTMP2, l_temp);
    if (b_close == TRUE)
    {
        movmem_((void*)PTR__GTMP3, (void*)PTR__GTMP4, l_temp);
    }
    if (l_off1 > l_off2)
    {
        l_temp = l_off1;
        l_off1 = l_off2;
        l_off2 = l_temp;
        df_xtemp = df__point1X;
        df_ytemp = df__point1Y;
        df__point1X = df__point2X;
        df__point1Y = df__point2Y;
        df__point2X = df_xtemp;
        df__point2Y = df_ytemp;
    }
    df_x1 = df__point1X;
    df_y1 = df__point1Y;
    df_x2 = df__point2X;
    df_y2 = df__point2Y;
    if (l_off1 == l_off2)
    {
        df_x1 = df__point2X;
        df_y1 = df__point2Y;
        df_x2 = df__point1X;
        df_y2 = df__point1Y;
    }
    if (TRUE != Truncate_End_Pline((BLOK**)&PTR__GTMP1, l_off1,
        df__point1X, df__point1Y, df_x1, df_y1) ||
        TRUE != Truncate_Begin_Pline((BLOK**)&PTR__GTMP2, l_off2,
            df__point2X, df__point2Y, df_x2, df_y2) ||
        (b_close == TRUE &&
            TRUE != Truncate_End_Pline((BLOK**)&PTR__GTMP4, l_off2,
                df__point2X, df__point2Y, df_x2, df_y2)) ||
        (b_close == TRUE &&
            TRUE != Truncate_Begin_Pline((BLOK**)&PTR__GTMP4, l_off1,
                df__point1X, df__point1Y, df_x1, df_y1)))
    {
        usun_obiekt((void*)PTR__GTMP1);
        usun_obiekt((void*)PTR__GTMP2);
        if (b_close == TRUE)
        {
            usun_obiekt((void*)PTR__GTMP4);
        }
        if ((break_and_leave) && (ptr_buffer!=NULL))
        {
            free(ptr_buffer);
        }
        return FALSE;
    }
    if (TRUE == b_close)
    {
        if (NULL == (ptrs_temp = Pline_Cat((BLOK*)PTR__GTMP2,
            (BLOK**)&PTR__GTMP1)))
        {
            usun_obiekt((void*)PTR__GTMP1);
            usun_obiekt((void*)PTR__GTMP2);
            usun_obiekt((void*)PTR__GTMP4);
            if ((break_and_leave) && (ptr_buffer!=NULL))
            {
                free(ptr_buffer);
            }
            return FALSE;
        }
        PTR__GTMP2 = (char*)ptrs_temp;
        usun_obiekt((void*)PTR__GTMP1);
        PTR__GTMP1 = PTR__GTMP2;
        PTR__GTMP2 = NULL;
        if (Get_Len_Pline((BLOK*)PTR__GTMP1) >
            Get_Len_Pline((BLOK*)PTR__GTMP4))
        {
            usun_obiekt((void*)PTR__GTMP4);
            PTR__GTMP4 = NULL;
        }
        else
        {
            usun_obiekt((void*)PTR__GTMP1);
            PTR__GTMP1 = PTR__GTMP4;
            PTR__GTMP4 = NULL;
        }
    }
    object_zap(FALSE, XOR_PUT);
    CUR_OFF(X, Y);
    ptrh_beg = PTR__GTMP3;
    ptrh_end = ptrh_beg + sizeof(NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1;
    blokzap(ptrh_beg, ptrh_end, Anormalny, COPY_PUT, 0);

    l_off1 = (char*)PTR__GTMP1 - dane;
    l_off2 = (char*)PTR__GTMP2 - dane;

    ptr_parent = FIRSTB(PTR__GTMP3);
    if (ptr_parent != NULL) l_off3 = dane + dane_size - (char*)ptr_parent;

    zmien_atrybut(ptrh_beg, ptrh_end, Anormalny, Abad);

    if (ptr_parent == NULL)
    {
        if (PTR__GTMP1 != NULL)
        {
            ptrh_beg = PTR__GTMP1;
            ptrh_end = ptrh_beg + sizeof(NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1;
            blokzap(ptrh_beg, ptrh_end, Ablok, COPY_PUT, 1);

            zmien_atrybut(ptrh_beg, ptrh_end, Ablok, Anormalny);

            if (break_and_leave) deduct_objects(PTR__GTMP1, ptr_buffer);
        }

        if (PTR__GTMP2 != NULL)
        {
            ptrh_beg = PTR__GTMP2;
            ptrh_end = ptrh_beg + sizeof(NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1;
            blokzap(ptrh_beg, ptrh_end, Ablok, COPY_PUT, 1);

            zmien_atrybut(ptrh_beg, ptrh_end, Ablok, Anormalny);

            if (break_and_leave) deduct_objects(PTR__GTMP2, ptr_buffer);
        }

        if ((break_and_leave) && (ptr_buffer!=NULL))
        {
            PTR__GTMP7 = (char*)dodaj_obiekt(NULL, (void*)ptr_buffer);
            free(ptr_buffer);

            ptrh_beg = PTR__GTMP7;
            ptrh_end = ptrh_beg + sizeof(NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1;
            blokzap(ptrh_beg, ptrh_end, Ablok, COPY_PUT, 1);
            zmien_atrybut(ptrh_beg, ptrh_end, Ablok, Anormalny);
        }

        PTR__GTMP4 = PTR__GTMP1;
        PTR__GTMP5 = PTR__GTMP2;
        PTR__GTMP6 = PTR__GTMP3;
        b__undo_break = TRUE;
    }
    else
    {
        if (PTR__GTMP1 != NULL)
        {
            ptrh_beg = PTR__GTMP1;
            ptrh_end = ptrh_beg + sizeof(NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1;
            blokzap(ptrh_beg, ptrh_end, Ablok, COPY_PUT, 1);
            zmien_atrybut(ptrh_beg, ptrh_end, Ablok, Anormalny);

            if (break_and_leave) deduct_objects(PTR__GTMP1, ptr_buffer);

            l_temp = ((NAGLOWEK*)PTR__GTMP1)->n + sizeof(NAGLOWEK);
            PTR__GTMP1_ = malloc(l_temp);
            if (PTR__GTMP1_!=NULL) movmem_((void*)PTR__GTMP1, (void*)PTR__GTMP1_, l_temp);
        }

        if (PTR__GTMP2 != NULL)
        {
            ptrh_beg = PTR__GTMP2;
            ptrh_end = ptrh_beg + sizeof(NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1;
            blokzap(ptrh_beg, ptrh_end, Ablok, COPY_PUT, 1);
            zmien_atrybut(ptrh_beg, ptrh_end, Ablok, Anormalny);

            if (break_and_leave) deduct_objects(PTR__GTMP2, ptr_buffer);

            l_temp = ((NAGLOWEK*)PTR__GTMP2)->n + sizeof(NAGLOWEK);
            PTR__GTMP2_ = malloc(l_temp);
            if (PTR__GTMP2_ != NULL) movmem_((void*)PTR__GTMP2, (void*)PTR__GTMP2_, l_temp);
        }

        if (PTR__GTMP2 != NULL) usun_obiekt((void*)PTR__GTMP2);
        if (PTR__GTMP1 != NULL) usun_obiekt((void*)PTR__GTMP1);

        ptr_parent = dane + dane_size - l_off3;

        if (PTR__GTMP1_ != NULL)
        {

           l_off3 = ptr_parent - dane;
           PTR__GTMP1 = (char*)dodaj_obiekt((void*)ptr_parent, (void*)PTR__GTMP1_);

           if (break_and_leave) deduct_objects(PTR__GTMP1_, ptr_buffer);

           ptr_parent = dane + l_off3;
           free(PTR__GTMP1_);
        }

        if (PTR__GTMP2_ != NULL)
        {
            l_off4 = ptr_parent - dane;
            PTR__GTMP2 = (char*)dodaj_obiekt((void*)ptr_parent, (void*)PTR__GTMP2_);

            if (break_and_leave) deduct_objects(PTR__GTMP2_, ptr_buffer);

            ptr_parent = dane + l_off4;
            free(PTR__GTMP2_);
        }

        if ((break_and_leave) && (ptr_buffer!=NULL))
        {
            PTR__GTMP7 = (char*)dodaj_obiekt((void*)ptr_parent, (void*)ptr_buffer);
            free(ptr_buffer);

            ptrh_beg = PTR__GTMP7;
            ptrh_end = ptrh_beg + sizeof(NAGLOWEK) + ((BLOK*)ptrh_beg)->n - 1;
            blokzap(ptrh_beg, ptrh_end, Ablok, COPY_PUT, 1);
            zmien_atrybut(ptrh_beg, ptrh_end, Ablok, Anormalny);
        }

        PTR__GTMP4 = PTR__GTMP1;
        PTR__GTMP5 = PTR__GTMP2;
        PTR__GTMP6 = PTR__GTMP3;
        b__undo_break = TRUE;
    }
    CUR_ON(X, Y);
   
    return TRUE;
}


static BOOL break_fun (void)
/*----------------------*/
{
  BOOL ret = FALSE;

  ret = break_object () ;
  return ret;
}

static BOOL object_sel (void **ptr_ob_pl)
/*------------------------------------*/
{
  unsigned type ;
  void *ptr_ob ;
  BOOL b_ret ;
  static BOOL (*INItial_sel_test)(void*) ;

  b_ret = TRUE ;
  if (b__pline == TRUE)
  {
    if (NULL != PTR__Sel_Adr && ((NAGLOWEK*)PTR__Sel_Adr)->atrybut == Ablok)
    {
      *ptr_ob_pl = PTR__Sel_Adr ;
      return b_ret ;
    }
    else
    if (NULL != PTR__Sel_Adr1 && ((NAGLOWEK*)PTR__Sel_Adr1)->atrybut == Ablok)
    {
      *ptr_ob_pl = PTR__Sel_Adr1 ;
      return b_ret ;
    }
    INItial_sel_test = INITIAL_SEL_TEST ;
    INITIAL_SEL_TEST = initial_sel_test ;
    type = Blinia | Bluk ;
    if (NULL != (ptr_ob = select_w (&type, NULL)) &&
	((NAGLOWEK*)ptr_ob)->atrybut == Ablok)
    {
      *ptr_ob_pl = ptr_ob ;
    }
    else
    {
      b_ret = FALSE ;
      ErrList (82) ;
    }
  INITIAL_SEL_TEST = INItial_sel_test;
  }
  return b_ret ;
}

static void redcr_break (char type)
/*---------------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[3])(),KR,KR0,akt,sel_cur, sel_gor, sel_nr;
  static TMENU *menu;
  static char st[POLE_TXT_MAX];
  unsigned short cod = L' ';
  static BOOL (*INTersect_add_test)(void*, void*) ;
  static int iconno=0;

  switch (type)
  {
    case 0 :
	   CUR_OFF(X, Y);
       KR=Komunikat_R;
       KR0=Komunikat_R0;
       komunikat(0);
       getmenupini(&menu,st,&cod,&iconno);

       menupini(&mBreak, _BREAK_, _BREAK_C_, 44);
       if (break_and_leave==TRUE) menu_par_new((*mBreak.pola)[2].txt,	_break_and_leave_);
       else menu_par_new((*mBreak.pola)[2].txt,	_break_and_cut_);

       PTR__GTMP3 = PTR__GTMP1 = PTR__GTMP2 = NULL ;
       PTR__GTMP4 = PTR__GTMP5 = PTR__GTMP6 = NULL ;
       akt=sel.akt; sel.akt=ASel;
       sel_cur=sel.cur; sel.cur=1;
       sel_gor = sel.gor; sel.gor = 0;
       sel_nr = sel.nr;

       layer_info();

       b__undo_break = FALSE;
       i__krok = 0;
       u__type_object = Bnic;
	  
       CUR_oFF=CUR_OFF;  CUR_OFF=out_sel_off;
       CUR_oN=CUR_ON;   CUR_ON=out_sel_on;
       SW[0]=SERV[73];  SERV[73]=sel_t;
       SW[1]=SERV[81];  SERV[81]=sel_d;
       CUR_ON(X,Y);
	   
       break ;
    case 1 :
       CUR_OFF(X,Y);
       CUR_OFF=CUR_oFF;
       CUR_ON=CUR_oN;
       SERV[73]=(void*)SW[0];
       SERV[81]= (void*)SW[1];
       menupini(menu,st,cod, iconno);
       PTR__GTMP3 = PTR__GTMP1 = PTR__GTMP2 = NULL ;
       PTR__GTMP4 = PTR__GTMP5 = PTR__GTMP6 = NULL ;
       sel.akt=akt;
       sel.cur=sel_cur;
       sel.gor = sel_gor;
       sel.nr = sel_nr;

       layer_info();

	   CUR_ON(X, Y);
       komunikat(Komunikat_R = KR);
       komunikat0(Komunikat_R0 = KR0);
       break ;
    case 2 :
        
        CUR_OFF(X, Y);
        CUR_OFF = CUR_oFF;
        CUR_ON = CUR_oN;
        SERV[73] = (void*)SW[0];
        SERV[81] = (void*)SW[1];

       INTersect_add_test = INTERSECT_ADD_TEST ;
       INTERSECT_ADD_TEST = intersect_add_test ;

       //sel.akt = akt;
       sel.cur = sel_cur;
       sel.gor = sel_gor;
       sel.nr = sel_nr;
       CUR_ON(X, Y);

       break ;
    case 3 :
       INTERSECT_ADD_TEST = INTersect_add_test ;
       break ;
   case  4:
          CUR_OFF(X, Y);
          komunikat(0);

          akt=sel.akt; sel.akt=ASel;
          sel_cur=sel.cur; sel.cur=1;
          sel_gor = sel.gor; sel.gor = 0;
          sel_nr = sel.nr;

          layer_info();

          CUR_oFF=CUR_OFF;  CUR_OFF=out_sel_off;
          CUR_oN=CUR_ON;   CUR_ON=out_sel_on;
          SW[0]=SERV[73];  SERV[73]=sel_t;
          SW[1]=SERV[81];  SERV[81]=sel_d;
          CUR_ON(X,Y);
          break ;
    default :
       break ;
  }
}


void Break (void)
/*--------------*/
{
  EVENT *ev;

  shadows_no = 0;
  redcr_break(0);
  while(1)
  {
  aa:

	if (FALSE == obiect_to_break ((void**)&PTR__GTMP3))
    {
      redcr_break (1);
      return;
    }

    object_zap (TRUE, XOR_PUT) ;

bb: komunikat0 (47) ;
    i__krok = 1;
    while(1)
    {
      redcr_break (2) ;
      ev=Get_Event_Point (NULL, &df__point1X, &df__point1Y);
      redcr_break (3) ;
      if ( ev->What == evKeyDown  && ev->Number== 0 )
      {
        object_zap(TRUE, COPY_PUT);

		if (shadows_no > 0)
		{
			delete_spline_shadows(FALSE);
		}
        redcr_break(4);
	    goto aa;
      }
      if ( ev->What == evKeyDown  && ev->Number== ENTER)
      {
		if (object_sel ((void**)&PTR__GTMP1) == TRUE)
		{
		  out_krz (X, Y);
		  break ;
		}
      }
      if ( ev->What==evCommandP)
      {
		if (ev->Number == UNDO_FUN && b__undo_break == TRUE)
		  {
            object_zap(TRUE, COPY_PUT);
			undo_break ();
            redcr_break(4);
			goto aa;
		  }
        else if (ev->Number == BREAK_AND_LEAVE) break_leave();
        else if (ev->Number == BREAK_AND_CUT) break_cut();
        continue;
      }
    }
    komunikat0 (48) ;
    i__krok = 2;
    b__undo_break = FALSE;
    while(1)
    {
      redcr_break (2) ;
      ev=Get_Event_Point (NULL, &df__point2X, &df__point2Y);
      redcr_break (3) ;
      if ( ev->What == evKeyDown  && ev->Number== 0 )
      {
		goto bb;
      }
      if ( ev->What == evKeyDown  && ev->Number== ENTER)
      {
		if (object_sel ((void**)&PTR__GTMP2) == TRUE)
		{
		  out_krz (X, Y);
		  break ;
		}
      }
      if( ev->What==evCommandP)
      {
		if (ev->Number == ONE_BREAK && u__type_object != Bokrag)
		{
		  podziel ();
          redcr_break(4);
		  goto aa;
		}
        else if (ev->Number == BREAK_AND_LEAVE) break_leave();
        else if (ev->Number == BREAK_AND_CUT) break_cut();
        continue;
      }
    }

     break_fun ();
    zmien_atrybut(dane, dane + dane_size - 1, Ablok, Ausuniety);
	spline_shadows_to_plines();
	
    if (shadows_no > 0) redraw();

    redcr_break(4);
	
  }
}
/*-----------------------------------------------------------------------*/
#undef __O_BREAK__