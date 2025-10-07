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

#define __O_POLYGN__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"

#include "menu.h"

extern int Get_Current_Color (void) ;
extern void out_blok2 (double, double, double, double, double, double, double, double, int, double, BOOL, int) ;
void transformacja_blok2 (char  *, char  *, double, double, double, double,
		double, double, double, double, int) ;
extern double Dist_PP (double x1,double y1, double x2, double y2) ;
extern double Grid_to_Rad (double angle) ;
extern char  *Add_Object (BLOK **, void *) ;
extern void out_parametry_lini (LINIA *) ;
extern void Out_Edited_Draw_Param (ESTR *lps_et, BOOL out) ;
extern void orto_l(LINIA *L, int *Orto_Dir);
extern void zmien_kolor_typ_warstwa(char  *adr,char  *adrk, unsigned char new_color, unsigned char new_type, unsigned char new_layer);
extern void view_line_type(LINIA *L);

enum POLY_DRAW_MODE { EDGE, I_SCRIBE, C_SCRIBE, EDGES} ;

typedef struct
{
  int mode ;	/*sposob rysowania : dwa kolejne rogi, opisany lub wpisany w wielokat*/
  int n ;	/*liczba bokow*/
  double x0, y0 ; /*srodek wielokata*/
  double x1, y1, x2, y2 ; /*dwa kolejne rogi*/
}
t_polygon ;

static t_polygon s_polygon = {I_SCRIBE, 4, 0, 0, 0, 0, 0, 0} ;
	/*poza mode i n pozostale parametry powinny byc wstepnie inicjowane*/

static LINIA line_g = Ldef ;

#define r18 18
#define r22 16
static int strwyj;
static double Px, Py, skala1, skala0, kos, koc, kos0, koc0;

static TMENU mPolygon= {4, 0,0,19,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr, 0,0,0,&pmPolygon, NULL,NULL} ;

static void temp_edge_proc (t_polygon *) ;
static void temp_c_scribe_proc (t_polygon *) ;
static void *create_polygon (BLOK *, t_polygon *, int) ;
static void *add_polygon (t_polygon *, int) ;
static int L_p(BOOL) ;
static void  cur_off (double, double) ;
static void  cur_on (double, double) ;
static void set_poly_mode (int) ;
static void out_poly_message0 (int) ;
static void out_poly_message1 (int) ;
static void redcr (char) ;
static void mypolygon (double , double) ;
static void redcr0(char) ;
static int O_n (BOOL) ;

/*-----------------------------------------------------------------------*/

static void temp_edge_proc (t_polygon *ptr_polygon)
/*-------------------------------------------------*/
{
/*wyznaczenie srodka wielokata*/
  double df_dangle, si, co, a ;
  double x, y , x0, y0, x2, y2 ;

  df_dangle = Pi2 / (2 * ptr_polygon->n) ;
  x = (ptr_polygon->x2 + ptr_polygon->x1) / 2 ;
  y = (ptr_polygon->y2 + ptr_polygon->y1) / 2 ;
  x2 = ptr_polygon->x2 - x ;
  y2 = ptr_polygon->y2 - y ;
  a = Atan2 (y2, x2) ;
  si = sin (a) ;
  co = cos (a) ;
  x0 = 0 ;
  y0 = Dist_PP (0, 0, x2, y2) / tan (df_dangle) ;
  obrd (si, co, x0, y0, &x0, &y0) ;
  x0 += x ;
  y0 += y ;
  ptr_polygon->x0 = x0 ;
  ptr_polygon->y0 = y0 ;
}


static void temp_c_scribe_proc (t_polygon *ptr_polygon)
/*-------------------------------------------------*/
{
/*wyznaczenie 1 i 2-go punktu wielokata opisanego na okregu*/
  double df_dangle, a, si, co ;
  double x, y ,x0, y0, x1, y1, r, h ;

  df_dangle = Pi2 / (2 * ptr_polygon->n) ;
  x0 = ptr_polygon->x0 ;
  y0 = ptr_polygon->y0 ;
  x = ptr_polygon->x1 - x0 ;
  y = ptr_polygon->y1 - y0 ;
  a = Atan2 (y, x) ;
  si = sin (a) ;
  co = cos (a) ;
  r = Dist_PP (0, 0, x, y) ;
  h = r * tan (df_dangle) ;
  x1 = r ;
  y1 = -h ;
  obrd (si, co, x1, y1, &x1, &y1) ;
  x1 += x0 ;
  y1 += y0 ;
  ptr_polygon->x1 = x1 ;
  ptr_polygon->y1 = y1 ;
}

static void *create_polygon (BLOK *ptr_blok, t_polygon *ptr_polygon, int atrybut)
/*---------------------------------------------------------------------------*/
/*add_polygon usuwa blok w przypadku braku pamieci*/
{
  double df_dangle, a, si, co, r ;
  double x0, y0, x1, y1, x2, y2, x02, y02 ;
  LINIA * ptr_line ;
  int i ;

  df_dangle = Pi2 / (2 * ptr_polygon->n) ;
  x0 = ptr_polygon->x0 ;
  y0 = ptr_polygon->y0 ;
  x1 = ptr_polygon->x1 - x0 ;
  y1 = ptr_polygon->y1 - y0 ;
  r =   Dist_PP (0, 0, x1, y1) ;
  x02 = r * cos (df_dangle) ;
  y02 = r * sin (df_dangle) ;
  for (i = 0; i < ptr_polygon->n ; i++)
  {
    a = Atan2 (y1, x1) ;
    a += df_dangle ;
    si = sin (a) ;
    co = cos (a) ;
    obrd (si, co, x02, y02, &x2, &y2) ;
    LiniaG.x1 = x1 + x0 ;
    LiniaG.y1 = y1 + y0 ;
    LiniaG.x2 = x2 + x0 ;
    LiniaG.y2 = y2 + y0 ;
    if (NULL == (ptr_line = (LINIA *)Add_Object (&ptr_blok, (void*)&LiniaG)))
    {
       usun_obiekt ((void *)ptr_blok) ;
       return NULL ;
    }
    ptr_line->atrybut = atrybut ;
    ptr_line->blok = ElemBlok ;
    if (linia_wybrana (ptr_line))
      ptr_line->widoczny = 1 ;
    else
      ptr_line->widoczny = 0 ;
    x1 = x2 ;
    y1 = y2 ;
  }
  return ptr_blok ;
}


static void *add_polygon (t_polygon *ptr_polygon, int atrybut)
/*-----------------------------------------------------------*/
{
  BLOK B = Bdef ;  //TO CHECK
  BLOK *nb ;
  char b_pltype ;

  B.n = B3 + sizeof (b_pltype) ;
  B.kod_obiektu = B_PLINE ;
  B.dlugosc_opisu_obiektu = sizeof(b_pltype) ;
  B.obiektt2 = O2BlockPline ;
  if( (nb = (BLOK*)dodaj_obiekt (NULL, &B)) == NULL) return 0 ;
  b_pltype = PL_POLYGON ;
  memmove (&(nb->opis_obiektu [0]), &b_pltype, sizeof(b_pltype)) ;

  switch (ptr_polygon->mode)
  {
    case EDGE :
	temp_edge_proc (ptr_polygon) ;
	break ;
     case I_SCRIBE :
	break ;
     case C_SCRIBE :
	temp_c_scribe_proc (ptr_polygon) ;
	break ;
     default :
	break ;
  }
  return create_polygon (nb, ptr_polygon, atrybut) ; /*add_polygon usuwa blok w przypadku braku pamieci*/
}

static int O_n (BOOL b_graph_value)
/*------------------*/
{
  int n;
  char polygon_n[8];

  b_graph_value = b_graph_value ;
  if (eO.val_no < 1)
  {
    return 0 ;
  }
  n = eO.values [0] ;
  if (n > 2 && n <= 1024)
  {
    s_polygon.n = n ;
	sprintf(polygon_n, "%d", s_polygon.n);
	menu_par_new((*mPolygon.pola)[3].txt, polygon_n);
    return 1 ;
  }
  else
  {
    ErrList (68) ;
    return 0 ;
  }
}



static int L_p(BOOL b_graph_value)
/*----------------*/
{
  PLINIA PL ;
  double l, k ;
  double angle_l;


  b_graph_value = b_graph_value ;
  if (eL.val_no < 1) return 0;
  l = eL.values [0] ;
  if ( orto != 0 || eL.val_no == 1)
  {
    angle_l=get_angle_l();
    parametry_linior (&line_g, &PL) ;
    k = angle_l; /*PL.kat*/  ;
    if (k<0) k+=360;
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
  l = jednostkiOb (l) ;
  k = Grid_to_Rad (k) ;
  kos0 = sin (k) ;
  koc0 = cos (k) ;
  skala0 = l ;
  strwyj = 1;
  return 1;
}

static void  cur_off (double x, double y)
/*----------------------------------------*/
{
    flip_screen();
}


static void  cur_on (double x,double y)
/*---------------------------------------*/
{
    double l_new ;
    LINIA L ;
    int temp_Orto_Dir;

    cursel_on (x, y) ;
    line_g.x2 = x ;
    line_g.y2 = y ;

    if (EDGE != s_polygon.mode)
    {
        outlineor (&line_g, COPY_PUT, 1) ;
    }
    else
    {
        out_parametry_lini (&line_g) ;
    }
    if(orto)
    {
        L.x1=Px;
        L.y1=Py;
        L.x2=x;
        L.y2=y;

        temp_Orto_Dir=I_Orto_XDir;
        orto_l(&L, &temp_Orto_Dir);
        x = L.x2;
        y = L.y2;

    }
    l_new = Dist_PP (x, y, Px, Py) ;
    skala1 = l_new ;
    if (l_new)
    {
        koc = (x - Px) / l_new ;
        kos = (y - Py) / l_new ;
    }
    else
    {
        koc = 1 ;
        kos = 0 ;
    }
    out_blok2 (Px, Py, kos, koc, Px, Py, skala1, skala1, Tobrot | Tskala, 0, TRUE, -1) ;
}

static void set_poly_mode (int mode)
/*---------------------------------*/
{
  switch (mode)
  {
     case EDGE :
	s_polygon.mode = EDGE ;
	break ;
     case I_SCRIBE :
	s_polygon.mode = I_SCRIBE ;
	break ;
     case C_SCRIBE :
	s_polygon.mode = C_SCRIBE ;
	break ;
     default :
	break ;
  }
}

static void out_poly_message0 (int mode)
/*------------------------------------*/
{
  switch (mode)
  {
     case EDGE :
	komunikat0 (57) ;
	break ;
     case I_SCRIBE :
	komunikat0 (58) ;
	break ;
     case C_SCRIBE :
	komunikat0 (59) ;
	break ;
     default :
	break ;
  }
}

static void out_poly_message1 (int mode)
/*------------------------------------*/
{
  switch (mode)
  {
     case EDGE :
	komunikat0 (60) ;
	break ;
     case I_SCRIBE :
     case C_SCRIBE :
	komunikat0 (61) ;
	break ;
     default :
	break ;
  }
}

static void redcr (char typ)
/*---------------------------*/
{
  static void  (*CUR_oN)(double ,double) ;
  static void (*CUR_oFF)(double ,double) ;
  static int KR0,np;
  if (typ == 0)
  {
    KR0 = Komunikat_R0 ;
    out_poly_message1 (s_polygon.mode) ;
    CUR_OFF (X, Y) ;
    CUR_oFF = CUR_OFF ;  CUR_OFF = cur_off ;
    CUR_oN = CUR_ON ;    CUR_ON = cur_on ;
    kos=0;
    koc=1;
    np = dodajstr (&eL) ;
    Cur_ond (X, Y) ;
  }
  else
  {
      CUR_OFF (X, Y) ;
    CUR_OFF = CUR_oFF ;
    CUR_ON = CUR_oN ;
    CUR_ON (X, Y) ;
    Komunikat_R0 = KR0 ;
    komunikat0 (Komunikat_R0) ;
    usunstr (np) ;
   }
}


static void mypolygon (double X0, double Y0)
/*----------------------------------------*/
{
  EVENT *ev;
  int size ;
  char  *ptr_bl ;
  unsigned char current_color, current_typ, current_warstwa;

  line_g.x1 = X0 ;
  line_g.y1 = Y0 ;
  line_g.x2 = X0 ;
  line_g.y2 = Y0 ;
  Px = X0 ;
  Py = Y0 ;
  if (s_polygon.mode == EDGE)
  {
    s_polygon.x1 = X0 ;
    s_polygon.y1 = Y0 ;
    s_polygon.x2 = X0 + 1 ;
    s_polygon.y2 = Y0 ;
  }
  else
  {
    s_polygon.x0 = X0 ;
    s_polygon.y0 = Y0 ;
    s_polygon.x1 = X0 + 1 ;
    s_polygon.y1 = Y0 ;
  }
  if(NULL == (ptr_bl = (char  *)add_polygon (&s_polygon, Ablok)))
  {
    return ;
  }
  size = ((BLOK*)ptr_bl)->n + sizeof(NAGLOWEK) ;
  ADP = ptr_bl ;
  ADK = ptr_bl + size ;
  redcr (0) ;
  while (1)
  {
    strwyj = 0 ;
    current_color=LiniaG.kolor;
    current_typ=LiniaG.typ;
    current_warstwa=LiniaG.warstwa;
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    if (ev->What == evKeyDown &&  ev->Number == 0)
    {
      Cur_offd (X, Y) ;
      usun_obiekt ((void *)ADP) ;
      redcr (1) ;
      return ;
    }
    
    if((ev->What == evKeyDown &&  ev->Number==ENTER)  || strwyj)
    {
      Cur_offd (X, Y) ;
      if (strwyj)
      {
	kos = kos0 ;
	koc = koc0 ;
	skala1 = skala0 ;
      }
      /*zmiana_koloru*/
      if ((current_color!=LiniaG.kolor) ||
          (current_typ!=LiniaG.typ) ||
          (current_warstwa!=LiniaG.warstwa))
          zmien_kolor_typ_warstwa(ADP, ADK, LiniaG.kolor, LiniaG.typ, LiniaG.warstwa);
          
      transformacja_blok2 (ADP, ADK, Px, Py, kos, koc,
	     Px, Py, skala1, skala1, Tobrot | Tskala) ;
      Set_Second_Screen();
      blokzap (ADP, ADK, Ablok, COPY_PUT, 1) ;
      Set_Screen();
      flip_screen();
      zmien_atrybut (ADP, ADK, Ablok, Anormalny) ;
      redcr (1) ;
      return ;
    }
  }
}


static void redcr0(char typ)
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[2])(),KR,KR0,akt;
  static TMENU *menu;
  static char st[POLE_TXT_MAX];
  static unsigned short cod = L' ';
  static int iconno=0;

  if (typ == 0)
   {
     CUR_OFF(X, Y);
     KR = Komunikat_R ;
     KR0 = Komunikat_R0 ;
     komunikat (0) ;
     eL.x = maxX - PL266 ;
     eL.y = ESTR_Y;
     eL.val_no_max = 2;
     eL.lmax = r22 ;
     eL.mode = GV_VECTOR ;
     eL.format = "%#12.9lg\0%#12.9lg;%#8.3lf" ;
     eL.ESTRF = L_p ;
	 el.extend = 0;
     eO.x=maxX-PL266 + 5 ;
	 //eO.x = maxX - (22 + 27)*WIDTH; + 5;
     eO.y= ESTR_Y;
     eO.val_no_max = 1;
     eO.mode = GV_NUMBER ;
     eO.format = "\001%#6d" ;
     eO.lmax=r22 - 1 ;
     eO.ESTRF=O_n;
	 eO.extend = 0;
     LiniaG.obiektt2 = O2BlockPline ;
     akt = sel.akt ; sel.akt = ASel ;
     CUR_oFF=CUR_OFF;  CUR_OFF=cursel_off;
     CUR_oN=CUR_ON;   CUR_ON=cursel_on;
     SW[0]=SERV[73];  SERV[73]=sel_t;
     SW[1]=SERV[81];  SERV[81]=sel_d;
     BlokM = 1 ;
     getmenupini (&menu, st, &cod, &iconno) ;
     out_poly_message0 (s_polygon.mode) ;
     CUR_ON (X,Y);
   }
  else
  {
    CUR_OFF(X,Y);
    CUR_OFF=CUR_oFF;
    CUR_ON=CUR_oN;
    SERV[73]=SW[0];
    SERV[81]=SW[1];
    CUR_ON(X,Y);
    BlokM = 0 ;
    sel.akt=akt;
    LiniaG.obiektt2 = O2NoBlockS ;
    Komunikat_R=KR;
    Komunikat_R0=KR0;
    menupini (menu, st, cod, iconno) ;
    komunikat(KR);
    komunikat0(KR0);
  }
}

void Polygon1 (void)
/*---------------*/
{
  EVENT *ev ;
  double X0, Y0 ;
  int np ;
  char polygon_n[8];

  sprintf(polygon_n, "%d", s_polygon.n);
  menu_par_new((*mPolygon.pola)[3].txt, polygon_n);
  redcr0 (0) ;
  while(1)
  {
    menupini (&mPolygon, _POLYGON_, _POLYGON_C_, 17) ;
    if (-1 != (np = dodajstr(&eO)) &&
	(EOF != sprintf (eO.st, "%#6d", s_polygon.n)) )
    {
      Out_Edited_Draw_Param ((ESTR *)&eO, TRUE) ;
    }
    view_line_type(&LiniaG); 
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    menupini (NULL, "", ' ', 0) ;
    usunstr (np) ;
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
	     mypolygon (X0, Y0) ;
	     out_poly_message0 (s_polygon.mode) ;
	   }
	   break ;
	case evCommandP :
	   switch (ev->Number)
	   {
	      case EDGE :
	      case I_SCRIBE :
	      case C_SCRIBE :
		 set_poly_mode (ev->Number) ;
		 out_poly_message0 (s_polygon.mode) ;
		 break ;
		  case EDGES:
		  simulate_keypress(TAB);
		  break;
	      default :
		 break ;
	   }
	   break ;
	default :
	   break ;
    }
  }
}

#undef __O_POLYGN__