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

#define __O_SKETCH__
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

#include "menu.h"

#define ID_DIST 0
#define ID_ERASE 1

extern int global_mode;

extern double dP1P2 (double , double , double , double ) ;
extern void orto_l(LINIA *L, int *Orto_Dir);
extern void set_pattern_add_limit(long_long n);
extern int get_pattern_offset(void);
extern void set_pattern_offset(int patt_offs);
extern void set_pattern_count(BOOL p_count);


static void (*cursor_on)(double ,double)=out_cur_on;
static void (*cursor_off)(double ,double)=out_cur_off;
static BOOL add_block (void) ;
static void dist_sketch (void) ;
static void erase_sketch (void) ;
static BOOL add_sketch_line (void) ;
static void redraw_sketch(void);

/*-----------------------------------------------------------------------*/

TMENU mSketch={2,0,0,25,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr, 0,0,0, &pmSketch, NULL, NULL} ;

static int patt_offs=0;
static int patt_offs_bak=0;

void set_sketch_offset(int patt_o)
{
    patt_offs = patt_o;
}

int get_sketch_offset(void)
{
    return patt_offs;
}

static void  cur_off (double x,double y)
/*---------------------------------------*/
{
    flip_screen();
}

static void  cur_on (double x, double y)
/*---------------------------------------*/
{
    int patt_offs_prev;
   // char st[16];

  LiniaG.x2 = x ;
  LiniaG.y2 = y ;
  if (df__Sketch_Dist_Max  <= dP1P2 (LiniaG.x1, LiniaG.y1, x, y) &&
    TRUE == add_sketch_line ())
  {
    global_mode == 0;
    outline (&LiniaG, COPY_PUT, 0) ;
    LiniaG.x1 = LiniaG.x2 ;
    LiniaG.y1 = LiniaG.y2 ;
    LiniaG.x2 = X ;
    LiniaG.y2 = Y ;
    Orto_Dir = I_Orto_NoDir;
  }
  set_pattern_offset(patt_offs_bak);
  outlineor (&LiniaG, COPY_PUT, 0) ;
  cursor_on(x, y);
}

static BOOL add_sketch_line (void)
/*--------------------------------*/
{
  BOOL b_ret = FALSE ;

  if(orto)
  {
    orto_l(&LiniaG, &Orto_Dir);
    Orto_Dir = I_Orto_NoDir;
  }
  if(LiniaG.x1 != LiniaG.x2 || LiniaG.y1 != LiniaG.y2)
  {
    if (dodaj_obiekt ((BLOK*)dane, (void*)&LiniaG) != NULL)
    {
      b_ret = TRUE ;
    }
    Set_Second_Screen();
      set_pattern_offset(patt_offs_bak);
      rysuj_obiekt((char*)&LiniaG, COPY_PUT,1);
      patt_offs_bak = get_pattern_offset();
    Set_Screen();
    flip_screen();
  }
  return b_ret ;
}

static void dist_sketch (void)
/*---------------------------*/
{
  char sk [MaxTextLen] = "", *str ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d ;
  char *ptr;

  if (EOF == sprintf(sk, "%-6.2lf", jednostki_d_to_plt_d (df__Sketch_Dist_Max )))
  {
    strcpy (sk, "");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  if (!get_string (sk, "", MaxTextLen, 0, 19)) return ;
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
  CUR_OFF (X, Y) ;
  df__Sketch_Dist_Max  = plt_d_to_jednostki_d (d) ;
  CUR_ON (X, Y) ;


  if (EOF == sprintf(sk, u8"%-6.2lf", jednostki_d_to_plt_d (df__Sketch_Dist_Max )))
  {
    strcpy (sk, u8"");
  }
  if (NULL != (str = strchr (sk, ' ')))
  {
    str [0] = '\0';
  }
  strcat (sk, _MM_) ;
  menu_par_new ((*mSketch.pola)[0].txt, sk) ;
}


static void erase_sketch (void)
/*---------------------------*/
{
  BLOK *ptrs_block ;
  NAGLOWEK *nag ;

  redraw_sketch();

  nag = (NAGLOWEK*)dane ;
  ptrs_block = (BLOK*)dane ;
  if (nag->obiekt == OdBLOK &&
      ptrs_block->kod_obiektu == B_PLINE &&
     ptrs_block->dlugosc_opisu_obiektu == 1 &&
     ptrs_block->opis_obiektu [0] == PL_SKETCH)
  {
    CUR_OFF (X, Y) ;
    set_pattern_count(TRUE);
    set_pattern_offset(0);
    patt_offs = 0;
    blokzap(dane, (char  *)(dane + nag->n), Anormalny, COPY_PUT, 0) ;
    usun_obiekt ((void *)dane) ;
    CUR_ON (X, Y) ;
  }
  else
  {
    ErrList (71) ;
  }
}


static void redraw_sketch(void)
/*---------------------------*/
{
    BLOK* ptrs_block;
    NAGLOWEK* nag;

    nag = (NAGLOWEK*)dane;
    ptrs_block = (BLOK*)dane;
    if (nag->obiekt == OdBLOK &&
        ptrs_block->kod_obiektu == B_PLINE &&
        ptrs_block->dlugosc_opisu_obiektu == 1 &&
        ptrs_block->opis_obiektu[0] == PL_SKETCH)
    {
        Set_Second_Screen();
        blokzap(dane, (char*)(dane + nag->n), Anormalny, COPY_PUT, 0);
        set_pattern_offset(0);
        patt_offs = 0;
        set_pattern_count(TRUE);
        blokzap(dane, (char*)(dane + nag->n), Anormalny, COPY_PUT, 1);
        Set_Screen();
    }
    else
    {
        ErrList(71);
    }
}


static BOOL add_block (void)
/*------------------------*/
{
  BLOK s_blockd = Bdef ;  //TO CHECK
  BLOK *ptrs_block ;
  char c_pltype ;

  s_blockd.n = B3 + sizeof (c_pltype) ;
  s_blockd.kod_obiektu = B_PLINE ;
  s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype) ;
  if( (ptrs_block = (BLOK*)dodaj_obiekt (NULL, &s_blockd)) == NULL)
  {
    return FALSE ;
  }
  c_pltype = PL_SKETCH ;
  memmove (&(ptrs_block->opis_obiektu [0]), &c_pltype, sizeof(c_pltype)) ;
  return TRUE ;
}


static void redcr (char typ)
/*--------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int KR0 ;

  switch (typ)
  {
    case 0 :
      KR0 = Komunikat_R0 ;
      komunikat0 (63) ;
      CUR_OFF (X, Y) ;
      CUR_oFF = CUR_OFF ;  CUR_OFF = cur_off ;
      CUR_oN = CUR_ON ;    CUR_ON = cur_on ;
      CUR_ON (X, Y) ;
      set_pattern_count(TRUE);
      set_pattern_offset(0);
      patt_offs_bak=0;
      set_pattern_add_limit(0);
      patt_offs = 0;
      break ;
    case 1 :
      erase_sketch () ;
    case 2 :
      CUR_OFF (X, Y) ;
      CUR_OFF = CUR_oFF ;
      CUR_ON = CUR_oN ;
      CUR_ON (X, Y) ;
      komunikat0 (KR0) ;
      set_pattern_count(FALSE);
      set_pattern_offset(0);
      patt_offs_bak=0;
      set_pattern_add_limit(0);
      patt_offs = 0;
      break ;
  }
}

void start_sketch (double X0, double Y0)
/*-------------------------------------*/
{
  EVENT *ev;

  LiniaG.x1=X0;
  LiniaG.y1=Y0;
  LiniaG.x2=X;
  LiniaG.y2=Y;
  Orto_Dir = I_Orto_NoDir;
  if (FALSE == add_block ())
  {
    return ;
  }
  redcr (0) ;
  while(1)
  {
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    switch (ev->What)
    {
      case evKeyDown :
	 if(ev->Number == 0)
	 {
	   redcr (1) ;
	   return ;
	 }
	 if(ev->Number == ENTER)
	 {
	   redcr (2) ;
	   return ;
	 }
      case evCommandP :
	 switch (ev->Number)
	 {
	   case ID_DIST :
	      dist_sketch () ;
	      break ;
	   case ID_ERASE :
	      redcr (1) ;
	      return ;
	   default :
	      break ;
	 }
	 break ;
      default :
	 break ;
    }
  }
}

static void set_menu_param (void)
/*------------------------------*/
{
  char sk [MaxTextLen] = "" ;

  if (EOF == sprintf(sk, "%#6.2lf", jednostki_d_to_plt_d (df__Sketch_Dist_Max )))
  {
    strcpy (sk, "");
  }
  if (strlen (sk) > 4) sk [4] = '\0' ;
  strcat (sk, "mm") ;
  menu_par_new ((*mSketch.pola)[0].txt, sk) ;
}


static void redcr0 (char typ)
{
  static void (*CUR_oN)(double ,double) ;
  static void (*CUR_oFF)(double ,double) ;
  static TMENU *menu ;
  static char st[POLE_TXT_MAX];
  unsigned short cod = L' ';
  static int iconno=0;
  int akt=0 ;

  if (typ==0)
  {
	 CUR_OFF(X, Y);
     komunikat (0) ;
     komunikat0 (62) ;
     set_menu_param () ;
     LiniaG.blok = 1 ;
     LiniaG.obiektt2 = O2BlockPline ;
     akt = sel.akt ; sel.akt = NASel ;
	 cursor_off = cursel_off;
	 cursor_on = cursel_on;
     getmenupini (&menu, st, &cod, &iconno) ;
     menupini (&mSketch, _SKETCH_, _SKETCH_C_, 26) ;

     CUR_oFF = CUR_OFF ;  CUR_OFF = cursor_off ;
     CUR_oN = CUR_ON ;   CUR_ON = cursor_on ;
     CUR_ON (X, Y) ;
  }
  else
  {
     CUR_OFF (X, Y) ;
     sel.akt = akt ;
     LiniaG.blok = 0 ;
     LiniaG.obiektt2 = O2NoBlockS ;
     menupini (menu, st, cod, iconno) ;
     CUR_OFF = CUR_oFF ;
     CUR_ON = CUR_oN ;
     CUR_ON (X, Y) ;
     komunikat (0);
     komunikat0 (0);
   }
}


void Sketch (void)
/*----------------*/
{
  EVENT *ev;
  double X0, Y0;

  redcr0 (0);
  while(1)
  {
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    switch (ev->What)
    {
      case evKeyDown :
	 if(ev->Number == 0)
	 {
	   redcr0 (1) ;
	   return ;
	 }
	 if(ev->Number == ENTER)
	 {
	   start_sketch (X0, Y0) ;
	 }
	 break ;
      case evCommandP :
	 switch (ev->Number)
	 {
	    case ID_DIST :
	       dist_sketch () ;
	       break ;
	    case ID_ERASE :
	       erase_sketch () ;
	       break ;
	    default :
	       break ;
	 }
	 break ;
      default :
	 break ;
    }
  }
 
}

#undef __O_SKETCH__