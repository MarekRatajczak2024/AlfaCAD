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

#define __O_RECTA__
#include<forwin.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#ifndef LINUX
#include<io.h>
#endif
#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<graphics.h>
#define ALLEGWIN
#include <allegext.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_spec.h"

#include "menu.h"

#include "leak_detector_c.h"


#define SEEK_SET        0
#define SEEK_CUR	1
#define SEEK_END	2

#define MaxLen 120

#pragma pack(1)

extern void obrot_import (void);
extern void obrot_import_kat (double kat_obrotu);
extern void obrot_import_K(void);
extern void	obrot_kopiuj_paral(double kat);
extern void	obrot_kopiuj_paralK(void);
extern void set_block_angle(double angle);
extern double get_block_angle(void);
extern double Angle_Normal(double angle);
extern double Angle_Normal_Grid (double angle);
extern void set_Px_Py(double Px_, double Py_);
extern void Rotate_Point (double, double, double, double, double, double, double *,double *) ;
extern BOOL Check_if_Equal (double x, double y);
extern int cartesian_to_isometric(double cx, double cy, double *ix, double *iy);
extern int isometric_to_cartesian(double ix, double iy, double *cx, double *cy);
extern int cartesian_vector_to_isometric(double dx_cart, double dy_cart, double *dx_iso, double *dy_iso);
extern int isometric_vector_to_cartesian(double dx_iso, double dy_iso, double *dx_cart, double *dy_cart);

typedef
  struct
     { int x1;
       int y1;
       int x2;
       int y2;
     } WIRE_ASC_;
typedef WIRE_ASC_ * WIRE_ASC;

typedef
  struct
     { unsigned e:8;
       unsigned f2:16;
       unsigned f1:23;
       unsigned s:1;
     } r_real;
     
typedef
  struct
     { unsigned f:23;
       unsigned e:8;
       unsigned s:1;
     } r_float;     
     
#pragma pack()     

extern void orto_l(LINIA *L, int *Orto_Dir);
extern void outrect(OKNO *O,double dx,double dy);
extern void outrect_a(OKNO *O, LINIA *L1);
extern void Out_Edited_Draw_Param (ESTR *lps_et, BOOL out);


static  OKNO s_window ;
static PRECTANGLE prectangle;
static int strwyj;
static double df_x1, df_y1;
static int np ;
static LINIA L1window ;
static double FormatYY, DELXX, DELYY;
static long ll;
static double angle_l_rect=0.0;
static double angle_l00=0.0;
static int insertion_point=0;
static double base_point_x, base_point_y;

void set_angle_l_rect(double angle)
{
    angle_l_rect=angle;
}

static BOOL add_block_0 (double x_0, double y_0, char *blok_type0)
/*--------------------------------------------------------*/
{
  unsigned size_block = B3 ;
  BLOK s_blockd = Bdef;
  BLOK blokd= Bdef ;  //TO CHECK
  BLOK *ptrs_block, *buf_block ;
  int  len_type, len_desc ;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  char blok_type[31];
  char c_pltype;


  strcpy(blok_type,blok_type0);
  len_type = strlen (blok_type) + 1 ;
  len_desc = sizeof(unsigned) + 2 * sizeof (float) + sizeof(len_type) + len_type ;
  size_block += len_desc ;
  
  if (NULL == (buf_block = (BLOK*) malloc /*getmem*/ (sizeof(NAGLOWEK) + size_block)))
  {
    return FALSE;
  }
  memcpy (buf_block, &blokd, sizeof (blokd)) ;
  buf_block->n = size_block ;
  buf_block->kod_obiektu = 'E' ;
  buf_block->dlugosc_opisu_obiektu = len_desc ;
  ptrs_desc_bl = (T_Desc_Ex_Block *)(&buf_block->opis_obiektu [0]) ;
  ptrs_desc_bl->flags = EBF_IP ;
  ptrs_desc_bl->x = x_0 ;
  ptrs_desc_bl->y = y_0 ;
  ptrs_desc_bl->len = len_type ;
  strcpy (&ptrs_desc_bl->sz_type [0], blok_type) ;
  if ((ptrs_block = (BLOK*)dodaj_obiekt (NULL, buf_block)) == NULL)
  {
      free(buf_block);
    return FALSE;
  }
  c_pltype = PL_RECTANGLE;
  memmove(&(ptrs_block->opis_obiektu[0]), &c_pltype, sizeof(c_pltype));
  free(buf_block);
  return TRUE ;
}

static BOOL add_block(void)
/*------------------------*/
{
	BLOK s_blockd = Bdef;  //TO CHECK
	BLOK *ptrs_block;
	char c_pltype;

	s_blockd.n = B3 + sizeof(c_pltype);
	s_blockd.kod_obiektu = B_PLINE;  //'E'
	s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype);
	if ((ptrs_block = (BLOK*)dodaj_obiekt(NULL, &s_blockd)) == NULL)
	{
		return FALSE;
	}
	c_pltype = PL_RECTANGLE;
	memmove(&(ptrs_block->opis_obiektu[0]), &c_pltype, sizeof(c_pltype));
	return TRUE;
}

int LinR(double x1,double y1,double x2,double y2)
{ LINIA L=Ldef;
  L.warstwa=Current_Layer;
  L.kolor=LiniaG.kolor;
  L.x1=(float)x1; L.y1=(float)y1;
  L.x2=(float)x2; L.y2=(float)y2;
  L.typ=LiniaG.typ;
  L.blok= ElemBlok; 
  L.obiektt1 = 0;
  L.obiektt2 = O2BlockPline;
   if (dodaj_obiekt((BLOK*)dane, (void*)&L)==NULL) return 0;
  else return 1;
}
/*
int TexR(double x,double y, int no)
{ TEXT T=Tdef;
    T.warstwa=Current_Layer;
    T.kolor=LiniaG.kolor;
    T.x=(float)x; T.y=(float)y;
    T.blok= ElemBlok;
    T.obiektt1 = 0;
    T.obiektt2 = O2BlockPline;
    sprintf(T.text,"%d",no);
    T.czcionka=zmwym.czcionka;
    T.wysokosc=(float)zmwym.wysokosc;
    T.kat=0;
    T.dl=strlen(&T.text[0]);
    T.n=T18+T.dl;
    T.width = 0;
    T.height = 0;
    if (dodaj_obiekt((BLOK*)dane, (void*)&T)==NULL) return 0;
    else return 1;
}
*/

static int Ins_rectangle(PRECTANGLE *prectangle)
{
  //char tekst_p[60];
  //char tekst_p1[60];
  //int i;
  //int l_kr;
  //int LengthT;
  //TEXT TextGa=Tdef;
  //double angle_l, sina, cosa;
  //double dy_typ = 3;
  //double dy_komentarz = 3;
  //char typ_bloku [30] = "";
  //double xx, yy;

  //xx=prectangle->x1;
  //yy=prectangle->y1;

  if (FALSE == add_block())
   {
    return 0;
   }

  //angle_l=get_angle_l();
  //sina=get_sina();
  //cosa=get_cosa();

    if (!LinR(prectangle->x1, prectangle->y1, prectangle->x2, prectangle->y2)) return 0;
    if (!LinR(prectangle->x2, prectangle->y2, prectangle->x3, prectangle->y3)) return 0;
    if (!LinR(prectangle->x3, prectangle->y3, prectangle->x4, prectangle->y4)) return 0;
    if (!LinR(prectangle->x4, prectangle->y4, prectangle->x1, prectangle->y1)) return 0;

    //for tests only
    //if (!TexR(prectangle->x1, prectangle->y1,1)) return 0;
    //if (!TexR(prectangle->x2, prectangle->y2,2)) return 0;
    //if (!TexR(prectangle->x3, prectangle->y3,3)) return 0;
    //if (!TexR(prectangle->x4, prectangle->y4,4)) return 0;

  return 1;
}

static double Px, Py,DX,DY;

static void  cur_offPB(double x, double y)
{
	flip_screen();
}

static void  cur_offPB__(double x,double y)
{ out_blok1(DX,DY,0,0,Tprzesuw,0);
  cursel_off(x,y);
}

static void  cur_onPB(double x,double y)
{
  DX=x-Px;  DY=y-Py;
  cursel_on(x, y);
  out_blok1(DX,DY,0,0,Tprzesuw,0);
  cursel_on(x, y);
}

static void obrot_import_kat_rect(double kat_obrotu)
/*--------------------------------------------*/
{
	double kos, koc;
	double kat_obrotu_r;
	double block_angle, block_angle0;

	kat_obrotu_r = kat_obrotu * Pi / 180;

	kos = sin(kat_obrotu_r); // -block_angle);
	koc = cos(kat_obrotu_r); // -block_angle);
	Cur_offd(X, Y);
	transformacja_blok(ADP, ADK, Px, Py, kos, koc, Tobrot, 0);
	block_angle = kat_obrotu_r;
    block_angle0 = get_block_angle();
    block_angle = block_angle0 + kat_obrotu_r;
	block_angle = Angle_Normal(block_angle);
    set_block_angle(block_angle);

	Cur_ond(X, Y);
}

int obrot_import_kat_rect90(void)
/*---------------------------*/
{
	obrot_import_kat_rect(90);
	return 0;
}

int obrot_import_kat_rect_90(void)
/*----------------------------*/
{
	obrot_import_kat_rect(-90);
	return 0;
}

static void redcrPB(char typ)
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int(*SW[4])();
 if (typ==0)
   { BlokM=1;
     CUR_oFF=CUR_OFF;    CUR_OFF=cur_offPB;
     CUR_oN=CUR_ON;      CUR_ON=cur_onPB;
     komunikat0(37);

     SW[0] = SERV[71];  SERV[71] = obrot_import_kat_rect90;  //Home  kat 0  t0
	 SW[1] = SERV[79];  SERV[79] = obrot_import_kat_rect_90;  //End   kat 90 t90

     Cur_ond(X,Y);
	 
   }
  else
  { Cur_offd(X,Y);
    CUR_OFF=CUR_oFF;
    CUR_ON=CUR_oN;

	SERV[71] = SW[0];
	SERV[79] = SW[1];

    BlokM=0;
    komunikat0(0);
  }
}


static int PrzesunB(double X0, double Y0)
{
  EVENT *ev;

  Px=X0;Py=Y0;
  DX=X-X0; DY=Y-Y0;
  redcrPB(0);
  while(1) {
      ev = Get_Event_Point(NULL, &X0, &Y0);
      if (ev->What == evKeyDown && ev->Number == 0) {
          redcrPB(1);
          return ESC;
      }
      if (ev->What == evKeyDown && ev->Number == ENTER) {
          redcrPB(1);
          return ENTER;
      }
      if (ev->What == evCommandP) {
          set_Px_Py(X0,Y0);
          if (ev->Number == 0) {
              obrot_import();
          }
          else if (ev->Number == 1)
          {
              obrot_import_kat (45) ;
          }
          else if (ev->Number == 2)
          {
              obrot_import_kat (90) ;
          }
          else if (ev->Number == 3)
          {
              obrot_import_kat (135) ;
          }
          else if (ev->Number == 4)
          {
              obrot_import_kat (180) ;
          }
          else if (ev->Number == 5)
          {
              obrot_import_kat (225) ;
          }
          else if (ev->Number == 6)
          {
              obrot_import_kat (270) ;
          }
          else if (ev->Number == 7)
          {
              obrot_import_kat (315) ;
          }
          else if (ev->Number == 8)
          {
              obrot_import_kat(0);
          }
          else if (ev->Number == 9)
          {
              obrot_import_K();
          }
          else if (ev->Number == 10)
          {
              obrot_kopiuj_paral(0);
          }
          else if (ev->Number == 11)
          {
              obrot_kopiuj_paral(90);
          }
          else if (ev->Number == 12)
          {
              obrot_kopiuj_paralK();
          }
      }
  }
}

static void cur_offo (double x, double y)
{
    flip_screen();
}

static void cur_ono (double x, double y)
{
  double df_x, df_y ;
  int temp_dir;
  double angle_l;
  LINIA L;
  double sina, cosa, dxl, dyl;
  

  s_window.x2 = x ;
  s_window.y2 = y ;
  angle_l=get_angle_l();
  if (angle_l==0)
  {
    outrect (&s_window, 0, 0) ;
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
  L1window.x1=L.x2;
  L1window.y1=L.y2;

  L.x2=s_window.x2;
  L.y2=s_window.y2;

  Orto_Dir=I_Orto_YDir;
  orto_l(&L,&Orto_Dir);
  L1window.x2=L.x2;
  L1window.y2=L.y2;

  Orto_Dir=temp_dir;

  outrect_a(&s_window,&L1window) ;
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
    dxl = milimetryob (L1window.x1 - s_window.x1);
    dyl = milimetryob (L1window.y1 - s_window.y1); 
    df_x = sqrt((dxl * dxl) + (dyl * dyl));
    dxl = milimetryob (L1window.x1 - s_window.x2);
    dyl = milimetryob (L1window.y1 - s_window.y2); 
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
  df_x1 = jednostkiOb(wsx * eL.values[0]);
  df_y1 = jednostkiOb(wsy * eL.values[1]);

  strwyj = 1 ;
  return 1 ;
}

void obrot_axis (void)
/*---------------------------*/
{
    char sk[MaxTextLen];
    int retval_no = 1;
    double buf_ret[1];
    double kos, koc;
    double kat_obrotu;
    double angle_l0;

    angle_l0=angle_l_rect;

    sk[0] = '\0';
    if (!get_string(sk, "", MaxTextLen, 0, 16)) return;
    if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1) {
        return;
    }
    angle_l_rect+=buf_ret[0];
    angle_l_rect=Angle_Normal_Grid(angle_l_rect);
    put_angle_lr(angle_l0, angle_l_rect);
}

void obrot_axis_K(void)
/*---------------------------*/
{
    char sk[MaxTextLen];
    int retval_no = 1;
    double buf_ret[1];
    double kos, koc;
    double kat_obrotu;
    double angle_l0;

    angle_l0=angle_l_rect;

    sk[0] = '\0';
    if (!get_string(sk, "", MaxTextLen, 0, 16)) return;
    if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
    {
        return;
    }
    angle_l_rect=buf_ret[0]+angle_l00;
    angle_l_rect=Angle_Normal_Grid(angle_l_rect);
    put_angle_lr(angle_l0, angle_l_rect);
}

static void obrot_axis_paral(double kat)
{
    double	kos, koc;
    double kat_linii=0;
    unsigned typ;
    void  *adr;
    PLINIA PL;
    double angle_l0;

    angle_l0=angle_l_rect;

    sel.akt = ASel;
    sel.gor = NGSel;

    typ = Blinia | Btekst;
    if ((adr = select_n(&typ, NULL, 75)) != NULL)
    {
        Cur_offd(X, Y);
        if (typ == Btekst) kat_linii = kat * Pi / 180.0 + ((TEXT*)adr)->kat;
        else
        {
            parametry_lini((LINIA*)adr, &PL);
            kat_linii = (kat + PL.kat) * Pi / 180.0;
        }

        angle_l_rect=kat_linii*180.0/Pi;
        put_angle_lr(angle_l0, angle_l_rect);

        Cur_ond(X, Y);
    }

    sel.akt = NASel;
    sel.gor = NGSel;

    Cur_offd(X, Y);
    Cur_ond(X, Y);
}

static void obrot_axis_paralK(void)
{
    double kat_linii = 0.0;
    unsigned typ;
    double kat;
    void  *adr;
    PLINIA PL;
    char sk[MaxTextLen] = "";
    int retval_no = 1;
    double buf_ret[1];
    double angle_l0;

    angle_l0=angle_l_rect;

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
            kat = kat * Pi / 180.0;
            Cur_offd(X, Y);
            if (typ == Btekst) kat = kat + ((TEXT*)adr)->kat;
            else
            {
                parametry_lini((LINIA*)adr, &PL);
                kat = kat + PL.kat * Pi / 180.0;
            }
            angle_l_rect=kat*180.0/Pi;
            put_angle_lr(angle_l0, angle_l_rect);

            Cur_ond(X, Y);
        }
    }

    sel.akt = NASel;
    sel.gor = NGSel;

    Cur_offd(X, Y);
    Cur_ond(X, Y);
}



static BOOL get_rectangle_size (void)
/*------------------------------*/
{
  EVENT *ev;
  double df_x, df_y ;
  double angle_l0;

  strwyj = 0 ;
  while(1)
  {
     ev = Get_Event_Point (NULL, &df_x, &df_y) ;
     if (ev->What == evCommandP) {
          if (ev->Number == 0) {
              obrot_axis();
          }
          else if (ev->Number == 1)
          {
              angle_l0=angle_l_rect;
              angle_l_rect=45.0+angle_l00;
              put_angle_lr (angle_l0, angle_l_rect) ;
          }
          else if (ev->Number == 2)
          {
              angle_l0=angle_l_rect;
              angle_l_rect=90.0+angle_l00;
              put_angle_lr (angle_l0,  angle_l_rect) ;
          }
          else if (ev->Number == 3)
          {
              angle_l0=angle_l_rect;
              angle_l_rect=135.0+angle_l00;
              put_angle_lr (angle_l0, angle_l_rect) ;
          }
          else if (ev->Number == 4)
          {
              angle_l0=angle_l_rect;
              angle_l_rect=180.0+angle_l00;
              put_angle_lr (angle_l0, angle_l_rect) ;
          }
          else if (ev->Number == 5)
          {
              angle_l0=angle_l_rect;
              angle_l_rect=225.0+angle_l00;
              put_angle_lr (angle_l0, angle_l_rect) ;
          }
          else if (ev->Number == 6)
          {
              angle_l0=angle_l_rect;
              angle_l_rect=270.0+angle_l00;
              put_angle_lr (angle_l0, angle_l_rect) ;
          }
          else if (ev->Number == 7)
          {
              angle_l0=angle_l_rect;
              angle_l_rect=315.0+angle_l00;
              put_angle_lr (angle_l0, angle_l_rect) ;
          }
          else if (ev->Number == 8)
          {
              angle_l0=angle_l_rect;
              angle_l_rect=0.0+angle_l00;
              put_angle_lr(angle_l0, angle_l_rect);
          }
          else if (ev->Number == 9)
          {
              obrot_axis_K();
          }
          else if (ev->Number == 10)
          {
              obrot_axis_paral(0.0);
          }
          else if (ev->Number == 11)
          {
              obrot_axis_paral(90.0);
          }
          else if (ev->Number == 12)
          {
              obrot_axis_paralK();
          }
          else if (ev->Number == 13)
          {
              insertion_point=0;
          }
          else if (ev->Number == 14)
          {
              insertion_point=1;
          }
      }
     else if(ev->What == evKeyDown || strwyj == 1)
     {
       if (ev->Number == 0)
       {
	     return FALSE ;
       }
       else if (ev->Number == ENTER || strwyj == 1)
       {
	    return TRUE ;
       }
     }
   }
}

static void redcrRECT (int type)
/*----------------------------*/
{
  static void  (*CUR_oN)(double ,double);
  static void  (*CUR_oFF)(double ,double);
  static int(*SW[3])(), akt;
 
  double angle_l;
  int temp_dir;
  LINIA L, L1;
  
  double sina, cosa;
  int ret;

  switch (type)
  {
    case 0 :
      eL.x = maxX / 2 + 1;
      eL.y = ESTR_Y;
      eL.val_no_max = 2 ;
      eL.lmax = 18 ;
      eL.mode = GV_VECTOR_XY ;
      eL.format = "\0%#8.3lf;%#8.3lf" ;
      eL.ESTRF = edit_distance_value ;
	  eL.extend = 0;
      break ;
    case 1 :
      komunikat0 (117) ;
      CUR_OFF (X,Y) ;
      sel.akt = 1 ;
      sel.cur = 0 ;
      CUR_oFF = CUR_OFF ; CUR_OFF = cursel_off ;
      CUR_oN = CUR_ON ;   CUR_ON = cursel_on ;
	  SW[0] = SERV[73];  SERV[73] = sel_t;
	  SW[1] = SERV[81];  SERV[81] = sel_d;

      menupini(&mKatOptR, _RECTA_A_, _RECTA_C_, 683);

      CUR_ON (X, Y) ;
      break ;
    case 2 :
      blokzap (ADP, ADK, Ablok, XOR_PUT, 1) ;
      komunikat0 (0) ;
	  CUR_OFF(X, Y);
	  sel.akt = 0;
	  sel.cur = 0;
	  SERV[73] = SW[0];
	  SERV[81] = SW[1];
      menupini(NULL, "", ' ', 0);
	  CUR_ON(X, Y);
      return ;
    case 3 :
      CUR_OFF (X, Y) ;
      np = dodajstr (&eL) ;
      s_window.x1 = s_window.x2 = X ;
      s_window.y1 = s_window.y2 = Y ;
      CUR_oFF = CUR_OFF ; CUR_OFF = cur_offo ;
      CUR_oN = CUR_ON ;   CUR_ON = cur_ono ;
      CUR_ON (X,Y) ;
      break ;
    case 4 :
      usunstr (np) ;
      komunikat0(0);
      CUR_OFF (X,Y) ;
      CUR_OFF = CUR_oFF ;
      CUR_ON = CUR_oN ;
	  sel.akt = 0;
	  sel.cur = 0;
	  CUR_ON(X, Y);
      break ;
    case 5 :
      usunstr (np) ;
      komunikat0(0);
      CUR_OFF (X,Y) ;
      CUR_OFF = CUR_oFF ;
      CUR_ON = CUR_oN ; 
      
      angle_l=get_angle_l();
      
      if (strwyj == 0)
      {
	     df_x1 = s_window.x2 - s_window.x1 ;
	     df_y1 = s_window.y2 - s_window.y1 ;
      }

	if (angle_l==0)
	{
	/*nalezy uzaleznic kierunek generowania od sposobu definiowania okna*/
	/*przypadek 1 punkt 3*/
	  prectangle.x1=0;			prectangle.y1=0;
	  prectangle.x2=fabs(df_x1);	prectangle.y2=0;
	  prectangle.x3=fabs(df_x1);	prectangle.y3=-fabs(df_y1);
	  prectangle.x4=0;			prectangle.y4=-fabs(df_y1);
	  /*przesuniecie prectangle*/
           base_point_x=prectangle.x3;
           base_point_y=prectangle.y3;
	  if (df_x1>0)
	   {
	   prectangle.x1-=df_x1;
	   prectangle.x2-=df_x1;
	   prectangle.x3-=df_x1;
	   prectangle.x4-=df_x1;
           base_point_x=prectangle.x1;
	   }
	  if (df_y1<0)
	   {
	   prectangle.y1-=df_y1;
	   prectangle.y2-=df_y1;
	   prectangle.y3-=df_y1;
	   prectangle.y4-=df_y1;

           base_point_y=prectangle.y1;
	   }

	}
	else
	 {
	  sina=get_sina();
	  cosa=get_cosa();
	  if (strwyj==1)
	  {
        if (!options1.uklad_izometryczny)
        {
            prectangle.x1 = 0;
            prectangle.y1 = 0;
            prectangle.x2 = -df_x1 * cosa;
            prectangle.y2 = -df_x1 * sina;
            prectangle.x3 = -(df_x1 * cosa) + (df_y1 * sina);
            prectangle.y3 = -(df_x1 * sina) - (df_y1 * cosa);
            prectangle.x4 = df_y1 * sina;
            prectangle.y4 = -df_y1 * cosa;
        }
        else
        {
            prectangle.x1 = 0;
            prectangle.y1 = 0;
            ret = isometric_to_cartesian(-df_x1, 0., &prectangle.x2, &prectangle.y2);
            ret = isometric_to_cartesian(-df_x1, -df_y1, &prectangle.x3, &prectangle.y3);
            ret = isometric_to_cartesian(0., -df_y1, &prectangle.x4, &prectangle.y4);
        }

          base_point_x=prectangle.x1;
          base_point_y=prectangle.y1;

          if (s_window.x2>s_window.x1)
          {
              if (s_window.y2 < s_window.y1)  /*1-3*/
              {
                  base_point_x=prectangle.x4;  //done
                  base_point_y=prectangle.y4;
              }
              else
              {
                  base_point_x=prectangle.x2;  //done //???
                  base_point_y=prectangle.y2;
              }
          }
          else
          {
              if (s_window.y2>s_window.y1)  /*3-1*/
              {
                  //4 or 1
                  double l2=(X-prectangle.x2)*(X-prectangle.x2)+(Y-prectangle.y2)*(Y-prectangle.y2);
                  double l3=(X-prectangle.x3)*(X-prectangle.x3)+(Y-prectangle.y3)*(Y-prectangle.y3);
                  if (l2<l3)
                  {
                      base_point_x = prectangle.x1;
                      base_point_y = prectangle.y1;
                  }
                  else
                  {
                      base_point_x = prectangle.x4;
                      base_point_y = prectangle.y4;
                  }
              }
              else
              {
                  base_point_x=prectangle.x2;
                  base_point_y=prectangle.y2;
              }
          }
	  }
	  else
	  {
	  if (s_window.x2>s_window.x1)
	   {
	    if (s_window.y2<s_window.y1)  /*1-3*/
	     {
	      prectangle.x1=s_window.x1-s_window.x2; 	prectangle.y1=s_window.y1-s_window.y2;
	      prectangle.x2=L1window.x1-s_window.x2;	prectangle.y2=L1window.y1-s_window.y2;
	      prectangle.x3=0;				prectangle.y3=0;
	      prectangle.x4=L1window.x2-s_window.x2;	prectangle.y4=L1window.y2-s_window.y2;
	     }
	     else  /*4-2*/
	      {
	      prectangle.x1=L1window.x2-s_window.x2; 	prectangle.y1=L1window.y2-s_window.y2;
	      prectangle.x2=0;				prectangle.y2=0;
	      prectangle.x3=L1window.x1-s_window.x2;	prectangle.y3=L1window.y1-s_window.y2;
	      prectangle.x4=s_window.x1-s_window.x2;	prectangle.y4=s_window.y1-s_window.y2;
	      }
	   }
	    else
	    {
	     if (s_window.y2>s_window.y1)  /*3-1*/
	     {
  	      prectangle.x1=0;  			prectangle.y1=0;
	      prectangle.x2=L1window.x2-s_window.x2;	prectangle.y2=L1window.y2-s_window.y2;
	      prectangle.x3=s_window.x1-s_window.x2;	prectangle.y3=s_window.y1-s_window.y2;
	      prectangle.x4=L1window.x1-s_window.x2;	prectangle.y4=L1window.y1-s_window.y2;
	     }
	     else  /*2-4*/
	      {
	       prectangle.x1=L1window.x1-s_window.x2; 	prectangle.y1=L1window.y1-s_window.y2;
	       prectangle.x4=0;				prectangle.y4=0;
	       prectangle.x3=L1window.x2-s_window.x2;	prectangle.y3=L1window.y2-s_window.y2;
	       prectangle.x2=-s_window.x2+s_window.x1;	prectangle.y2=-s_window.y2+s_window.y1;
	      }
	    }
	  }
	 }
      break ;
    case 7 :
        menupini(&mKatR, _RECTA_R_, _RECTA_C_, 35);
        set_block_angle(angle_l_rect*Pi/180.0);  //0.0
        
      break ;
  case 8 :
        menupini(NULL, "", ' ', 0);
      break ;
  }
}

void Rectangle1(void)
{ int n;
  int l_kr;
  long ds;
  int dynamic_block_x;
  char symbol_ap[30];
  char typ_ap[30];
  char komentarz_ap[30];
  int punkt = 3;

  angle_l00=get_angle_l();

  angle_l_rect+=angle_l00;

  put_angle_lr (angle_l00,  angle_l_rect) ;

aa:
  set_block_angle(0.0);

  l_kr=put_angle_lr(angle_l00, angle_l_rect);
   
  redcrRECT (0) ;
  redcrRECT (1) ;

  if (FALSE ==get_rectangle_size ())
  {
    redcrRECT (2) ;
    l_kr=put_angle_lr(angle_l_rect, angle_l00);
    angle_l_rect-=angle_l00;
    return ;
  }
  redcrRECT (3) ;
  
  if (FALSE ==get_rectangle_size ())
  {
    redcrRECT (4) ;
    goto aa ;
  }
  redcrRECT (5) ;

  ds=dane_size;
  CUR_ON(X, Y);
  Ins_rectangle(&prectangle);
  ADP = dane; ADK = dane + (dane_size - ds -1);
  zmien_atrybut(ADP,ADK,Anormalny,Ablok);
  CUR_OFF(X, Y);

  redcrRECT (7) ;
  l_kr = put_angle_lr(angle_l_rect, angle_l00);


  if (insertion_point==1)
  {
      if (PrzesunB(0, 0) == ESC) {
          zmien_atrybut(ADP, ADK, Ablok, Ausuniety);
          usun_blok(ADP, ADK);
          CUR_ON(X, Y);
          redcrRECT(8);
          goto aa;
      } else {
          transformacja_blok(ADP, ADK, X - 0, Y - 0, 0, 0, Tprzesuw, 0);
      }
  }
  else
  {
      if (strwyj == 1)
          transformacja_blok(ADP, ADK, s_window.x1-base_point_x, s_window.y1-base_point_y, 0, 0, Tprzesuw, 0);
      else transformacja_blok(ADP, ADK, X - 0, Y - 0, 0, 0, Tprzesuw, 0);
  }


     blokzap(ADP,ADK,Ablok,COPY_PUT,1);
     zmien_atrybut(dane, dane + dane_size, Aoblok, Anormalny);
     zmien_atrybut(ADP,ADK,Ablok,Aoblok);
     CUR_ON(X,Y);
     redcrRECT (8) ;
     goto aa;


}

#undef __O_RECTA__