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

#define __O_GRAPHF__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "bib_e.h"
#include "bib_edit.h"
#include "rysuj_e.h"
#include "o_graphf.h"
#include "o_libfun.h"
#include "o_object.h"
#include "o_pline.h"

#include "menu.h"

#include "leak_detector_c.h"


enum GRAPH_MNU {PLINE_DX = 0, GRAPH_FUNCTION = 1} ;

static double df__pline_dx = 2.0 ;		/*we wspolrzednych wewnetrznych*/
static double df__gpdx, df__gpdy ;
static int strwyj ;
static BOOL b__line ;
static char  sz__graph_fun [MaxTextLen] = "x" ;
static LINIA L=Ldef;

/*-----------------------------------------------------------*/

typedef struct
{
  char sz_graphf_name [MAX_GRAPHF_NAME_LEN] ;	/*nazwa funkcji*/
  int i_off ;			/*adres wzoru funkcji*/
}
T_Graphf_Name ;

static T_Graphf_Name  s__graphf_names [MAX_GRAPHFNO] ; /*tablica nazw wzorow*/
char *ptrsz__graphf_formula=NULL ;	/*adres wzorow funkcji*/
static int i__graphfno ;		/*liczba wzorow*/
static int i__buf_off ;			/*bierzacy adres wolnego miejsca w buforze*/

static TMENU mGraph = {2,0,0,15,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmGraph,NULL,NULL} ;

void DoneBuffers5(void)
{
  if (ptrsz__graphf_formula!=NULL) free (ptrsz__graphf_formula); 
}

static BOOL
get_graphf_param (T_Fstring key_name, T_Fstring ret_string)
/*--------------------------------------------------------*/
{

  if (i__graphfno >= MAX_GRAPHFNO)
  {
    return FALSE ;
  }
  if (ret_string == NULL)
  {
    return FALSE ;
  }
  strncpy (s__graphf_names [i__graphfno].sz_graphf_name, key_name, MAX_GRAPHF_NAME_LEN) ;
  s__graphf_names [i__graphfno].sz_graphf_name [MAX_GRAPHF_NAME_LEN - 1] = '\0' ;
  s__graphf_names [i__graphfno].i_off = i__buf_off ;
  strcpy (&ptrsz__graphf_formula [i__buf_off], ret_string) ;
  i__buf_off += strlen (ret_string) + 1 ;

  strcpy(pmGraphf [i__graphfno + 1].txt, &(s__graphf_names [i__graphfno].sz_graphf_name [0])) ;
  
  pmGraphf [i__graphfno + 1].wcod = ' ' ;
  pmGraphf [i__graphfno + 1].menu = NULL ;
  i__graphfno++ ;
  return TRUE ;
}


int Read_Graph_Function(void)
/*---------------------------*/
{

    i__graphfno = 0;
    i__buf_off = 0;

    if (NULL == (ptrsz__graphf_formula = (char*)malloc(MAX_GRAPHF_FORMULA_LEN)))	/*tymczasowo i z nadmiarem*/
    {
        return i__graphfno;
    }

    Get_Private_Profile_Strings((T_Fstring)GRAPHF, get_graphf_param);
    if (i__graphfno > 0)
    {
        ptrsz__graphf_formula = realloc(ptrsz__graphf_formula, i__buf_off + 100);
    }
    
    strcpy(pmGraphf[0].txt, _CURRENT_);

    pmGraphf[0].wcod = ' ';
    pmGraphf[0].menu = NULL;
    mGraphf.max = i__graphfno + 1;
    return i__graphfno;
}


/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/

static int L_p (BOOL b_graph_value)
/*-------------------------------*/
{
  PLINIA PL ;
  double l, k ;
  double angle_l;

  b_graph_value = b_graph_value ;
  if (eL.val_no < 1)
  {
    return 0 ;
  }
  l = eL.values [0] ;
  if ( orto != 0 || eL.val_no == 1)
  {
    parametry_linior (&L, &PL) ;
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
  k = k * Pi / 180.0;
  df__gpdx = Double_to_Float (l * cos (k));
  df__gpdy = Double_to_Float (l * sin (k));
  strwyj=1;
  return 1;
}

static void cur_off (double x,double y)
{
    flip_screen();
}
/*
static void cur_off__ (double x,double y)
{
  if (b__line == TRUE)
  {
    outlineor (&L, XOR_PUT, 0) ;
  }
  cursel_off (x,y) ;
}
*/
static void cur_on (double x,double y)
{
  cursel_on (x,y) ;
  if (b__line == TRUE)
  {
    L.x2 = x ; L.y2 = y ;
    outlineor (&L, COPY_PUT, 1) ;
  }
}


static void set_pline_dx (void)
/*----------------------------*/
{
  char sk [MaxTextLen] = "" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d;

  DF_to_String (sk, "%-6.2f", milimetryob (df__pline_dx), 0) ;
  if (!get_string (sk, "", MaxTextLen, 0, 54)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  if ( d <= 0 )
  {
    ErrList (30);
    return ;
  }
  df__pline_dx = jednostkiOb (d) ;
  DF_to_String (sk, "%-6.2f", milimetryob (df__pline_dx), 6) ;
  menu_par_new ((*mGraph.pola)[0].txt, sk) ;
}


static void set_graph_function (int i_graphf_no)
/*--------------------------------------------*/
{
  char sk [MaxTextLen] ;
  char *ptrsz_graphf ;

  if (0 == i_graphf_no)
  {
    strcpy (sk, sz__graph_fun) ;
  }
  else
  {
    ptrsz_graphf = &ptrsz__graphf_formula [s__graphf_names [i_graphf_no - 1].i_off] ;
    strcpy (sk, ptrsz_graphf) ;
  }
  if (TRUE == get_string (sk, "", MaxTextLen, 0, 55))
  {
    strcpy (sz__graph_fun, sk) ;
  }
}

#define Bdefx  {Anormalny,OdBLOK,0,0,0,1,0,0,  B3, 0,0, 0}

static BLOK *add_block (void)
/*------------------------*/
{
  BLOK s_block = Bdefx ;
  BLOK *ptrs_block ;

  s_block.n = B3 + sizeof (char) ;
  s_block.kod_obiektu = B_PLINE ;
  s_block.dlugosc_opisu_obiektu = sizeof(char) ;
  s_block.obiektt2 = O2BlockPline ;
  s_block.opis_obiektu [0] = PL_PLINE ;
  ptrs_block = (BLOK*)dodaj_obiekt (NULL, &s_block) ;
  return ptrs_block ;
}

static BOOL calculate (double df_x, double *ptrdf_y)
/*------------------------------------------------*/
{
  char sz_tmp [MaxTextLen] ;
  int retval_no = 1 ;
  BOOL b_ret = TRUE ;

  strcpy (sz_tmp, "x=") ;
  DF_to_String (&sz_tmp [2], "%-8.6f", df_x, 0) ;
  strcat (sz_tmp, ";") ;
  strcat (sz_tmp, sz__graph_fun) ;
  if (FALSE == calculator (sz_tmp, &retval_no, ptrdf_y)  || retval_no < 1)
  {
    b_ret = FALSE ;
  }
  return b_ret ;
}

static void make_graph_function (double df_gpx1,
				 double df_gpy1,
				 double df_gpx2)
/*---------------------------------------------*/
{
/*wykres musi przechodzic przez punkt (df_gpx1, df_gpy1),
  -lokalny poczatek ukladu wspolrzednych jest okreslony przez
   (df_gpx1, df_gpy1) i wartosc funkcji w 0
  -funkcja w 0 musi byc okreslona jednoznacznie,
  -
*/

  BLOK *ptrs_block ;
  double df_x1, df_x2, df_y1, df_y2, df_y0 ;
  double df_len, df_sumlen, df_pline_dx ;

  char  *ptr_ob_begin,  *ptr_ob_end ;

  if (TRUE == Check_if_Equal (df_gpx1, df_gpx2) ||
      strlen (sz__graph_fun) == 0)
  {
    return ;
  }
  df_pline_dx = df__pline_dx ;
  if (df_gpx1 > df_gpx2)
  {
    df_pline_dx = -df__pline_dx ;
  }
  df_x1 = df_gpx1 ;
  df_y1 = df_gpy1 ;
  if (FALSE == calculate (0, &df_y0))
  {
    return ;
  }
  df_sumlen = fabs (df_gpx2 - df_gpx1) ;
  df_len = 0 ;
aa:
  if (NULL == (ptrs_block = add_block ()))
  {
    return ;
  }
  Set_Second_Screen();
  while (TRUE == Check_if_LT (df_len, df_sumlen))
  {
    df_x2 = df_x1 + df_pline_dx ;
    df_len += df__pline_dx ;
    if (df_len > df_sumlen)
    {
      df_x2 = df_gpx2 ;
    }
    if (FALSE == calculate (df_x2 - df_gpx1, &df_y2))
    {
      df_x1 = df_x2 + df_pline_dx ;
      df_len += df__pline_dx ;
      if (TRUE == Check_if_GE (df_len, df_sumlen))
      {
	   return ;
      }
      if (FALSE == calculate (df_x1 - df_gpx1, &df_y1))
      {
	   ErrList (90) ;
	   return ;
      }
      df_y1 += df_gpy1 - df_y0 ;
      goto aa ;
    }
    df_y2 += df_gpy1 - df_y0 ;
    LiniaG.x1 = jednostkiOb(df_x1) ;
    LiniaG.y1 = jednostkiOb(df_y1) ;
    LiniaG.x2 = jednostkiOb(df_x2) ;
    LiniaG.y2 = jednostkiOb(df_y2) ;
    if (NULL == Add_Object (&ptrs_block, (void *)&LiniaG))
    {
        ptr_ob_begin = (char*)ptrs_block ;
        ptr_ob_end = (char*)ptrs_block + sizeof (NAGLOWEK) + ((BLOK*)ptrs_block)->n - 1;
        blokzap (ptr_ob_begin, ptr_ob_end, Anormalny, COPY_PUT, 0) ;

        usun_obiekt (ptrs_block) ;
        Set_Screen();
        flip_screen();
        return;
    }
    rysuj_obiekt ((char *)&LiniaG, COPY_PUT, 1) ;
    Set_Screen();
    df_x1 = df_x2 ;
    df_y1 = df_y2 ;
  }
  flip_screen();
}


static BOOL graphf_point (double *ptrdf_gpx, double *ptrdf_gpy)
/*------------------------------------------------------------*/
{
  EVENT *ev;

  while(1)
  {
     strwyj = 0 ;
     ev = Get_Event_Point (NULL, ptrdf_gpx, ptrdf_gpy) ;
     if (ev->What == evKeyDown && ev->Number == 0)
     {
       return FALSE ;
     }
     else
     if ((ev->What == evKeyDown && ev->Number == ENTER) || strwyj)
     {
       return TRUE ;
     }
     else
     if (ev->What == evCommandP)
     {
       if (ev->Number == PLINE_DX)
       {
	 set_pline_dx () ;
       }
       if (ev->Number > PLINE_DX)
       {
	 set_graph_function (ev->Number - 2) ;
       }
     }
   }
}


static void redcrP (char typ)
/*------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[3])(), akt ;
  char sk [MaxTextLen] ;

  if (typ == 0)
  {
     komunikat (0) ;
     b__line = FALSE ;
     LiniaG.blok= ElemBlok ;
     LiniaG.obiektt2 = O2BlockPline ;
     akt = sel.akt ; sel.akt = ASel ;
     Set_Edit_Vector (&eL, L_p) ;
     CUR_OFF (X, Y) ;
     CUR_oFF = CUR_OFF ;  CUR_OFF = cur_off ;
     CUR_oN = CUR_ON ;   CUR_ON = cur_on ;
     SW[0] = SERV[73] ;  SERV[73] = sel_t ;
     SW[1] = SERV[81] ;  SERV[81] = sel_d ;

     menupini (&mGraph, _GRAPHF_, _GRAPHF_C_, 28) ;

     DF_to_String (sk, "%-6.2f", milimetryob (df__pline_dx), 6) ;
     menu_par_new ((*mGraph.pola)[0].txt, sk) ;
     CUR_ON (X, Y) ;
   }
  else
  {
     CUR_OFF (X, Y) ;
     CUR_OFF = CUR_oFF ;
     CUR_ON = CUR_oN ;
     SERV[73] = SW[0] ;
     SERV[81] = SW[1] ;
     sel.akt = akt ;
     CUR_ON (X, Y) ;
     LiniaG.blok= NoElemBlok ;
     LiniaG.obiektt2 = O2NoBlockS ;
     menupini (NULL, "", ' ',0) ;
     komunikat (0) ;
     komunikat0 (0) ;
   }
}


void Graph_Function (void)
/*-----------------------*/
{
  double df_gpx1, df_gpy1, df_gpx2, df_gpy2 ;
  int np ;
  BOOL b_ret ;

  redcrP (0) ;
  while (1)
  {
    komunikat0 (93) ;
    b__line = FALSE ;
    if (FALSE == graphf_point (&df_gpx1, &df_gpy1))
    {
      redcrP (1) ;
      return ;
    }
    komunikat0 (94) ;
    np = dodajstr (&eL) ;
    CUR_OFF (X,Y) ;
    L.x1 = L.x2 = df_gpx1 ;
    L.y1 = L.y2 = df_gpy1 ;
    b__line = TRUE ;
    CUR_ON (X,Y) ;
    b_ret = graphf_point (&df_gpx2, &df_gpy2) ;
    CUR_OFF (X,Y) ;
    usunstr (np) ;
    b__line = FALSE ;
    if (TRUE == b_ret)
    {
      if (strwyj)
      {
	df_gpx2 = df_gpx1 + df__gpdx ;
      }
      make_graph_function (milimetryob(df_gpx1), milimetryob(df_gpy1), milimetryob(df_gpx2)) ;
    }
    CUR_ON (X,Y) ;
  }
}

#undef __O_GRAPHF__