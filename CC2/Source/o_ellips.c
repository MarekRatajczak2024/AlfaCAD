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

#define __O_ELLIPS__
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
#include "o_object.h"
#include "o_ellip1.h"
#include "bib_edit.h"

#include "menu.h"

#define OZero 1.0E-6
#define OCon  1.0E+6      /*1.0E+6*/

extern void setwritemode( int mode );
extern void setcolor(int kolor);
extern int linestyle_xor(int typ);
extern int linestyle_solid(int typ);
extern void view_line_type(LINIA *L);
extern double dP1P2 (double x1,double y1, double x2, double y2);
extern void out_sel_off(double X, double Y);
extern void out_sel_on(double X, double Y);
extern void najblizszyL_ (double *x, double *y, void *adr);
extern void wielokat_linia_ (double *x, double *y, void *adr, void *adr1, void (*fun)(double*, double*, void*));

void orto_l(LINIA *L, int *Orto_Dir);
static int outline_disc=0;
static int strwyj,ns=0;

static void  (*CUR_oN0)(double ,double)=NULL;
static void  (*CUR_oFF0)(double ,double)=NULL;

static void  (*CUR_oN)(double ,double)=NULL;
static void  (*CUR_oFF)(double ,double)=NULL;
static void  (*CUR_oN1)(double ,double)=NULL;
static void  (*CUR_oFF1)(double ,double)=NULL;
static void  (*CUR_oN2)(double, double)=NULL;
static void  (*CUR_oFF2)(double, double)=NULL;

static void cur_el_on (double df_x, double df_y);
static void cur_el_off (double df_x, double df_y);
static void cur_el_sel_off (double df_x, double df_y);
static void cur_el_sel_on (double df_x, double df_y);
static int np=-1 ;
static int y_axis=0;
static int el_sel_akt=-1,el_sel_cur, el_sel_gor, el_sel_nr;

typedef
struct
{ double xs,ys;
    double xc,yc;
    double xe,ye;
    double x2,y2;
    double a;
    double L;
    double r;
    double d;
    void *ptr_con ; /*kontynuacja, xs, ys : wsp. konca, do kontynuacji*/
    BOOL b_first_end ;	/*ktory koniec objektu jest ostatni*/
    double con_angle ; /*kat kontynuacji*/
} PELUK;

static PELUK pel;

static LINIA L=Ldef;

TMENU m__ellipse = {2,0,0,34,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pm__ellipse,NULL,NULL} ;
TMENU m__ellipse_e = {1,0,0,34,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pm__ellipse,NULL,NULL} ;
TMENU m__ellipse_d = {2,0,0,34,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pm__ellipse_d,NULL,NULL} ;
TMENU m__ellipticalarc = {2,0,0,34,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pm__ellipticalarc,NULL,NULL} ;

/*-----------------------------------------------------------------------*/

enum ELL {ELL_DIS = 0, ELL_POINT = 1, ELL_TAN = 2, ELL_AXIS_END = 3, ELL_CENTER = 4} ;
enum ELL_ANGLE {ELL_LOCAL = 0, ELL_GLOBAL = 1 } ;
#define ELL_TRANS 3

typedef struct
{
  int i_draw_type ;	/*sposob rysowania elipsy*/
  double df_1x, df_1y ; /*1-y wskazany punkt*/
  double df_2x, df_2y ; /*2-y wskazany punkt*/
  double df_3x, df_3y ; /*3-y wskazany punkt*/
  double df_cx, df_cy ; /*3-y wskazany punkt*/
  double df_angle, df_xaxis, df_yaxis ;/*kat obrotu elipsy, dlugosc polosi w kierunku X i Y*/
  double df_sangle, df_eangle;/*start angle, end angle*/
  double df_kos, df_koc; /*sin(df_angle), scos(df_angle)*/
  double df_kos_b, df_koc_b; /*sin(df_angle), scos(df_angle)*/
  int    df_yaxis_method;
}
t_ellipse ;

static t_ellipse s_ellipse = {ELL_AXIS_END, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;

static void (*cursor_on)(double ,double)=out_cur_on;
static void (*cursor_off)(double ,double)=out_cur_off;

#define yaxis_x_axis_f 0.99

#define r18 18
#define r22 16 //22
static int strwyj;
static double Lx2,Ly2 ;
static double df__axis2_len ;
static double df__angle_a;
static LINIA line_g = Ldef ;


/*------------------------------*/

double cot(double a) { return cos(a) / sin(a); }

static int
L_p (BOOL b_graph_value)
/*----------------------*/
{
  PLINIA PL ;
  double l, k ;
  double angle_l;

  b_graph_value = b_graph_value ;
  if (eL.val_no < 1) return 0;
  l = eL.values [0] ;
  if ( orto != 0 || eL.val_no == 1)
  {
    parametry_linior (&line_g, &PL) ;
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
  l = jednostkiOb (l) ;
  k = Grid_to_Rad (k) ;
  Lx2 = line_g.x1 + l * cos (k) ;
  Ly2 = line_g.y1 + l * sin (k) ;
  strwyj = 1 ;
  return 1 ;
}

static int
L_axis2_len (BOOL b_graph_value)
/*------------------------------*/
{
  double df_len ;

  b_graph_value = b_graph_value ;
  if (eL.val_no < 1)
  {
    return 0 ;
  }
  df_len = eL.values [0] ;
  if (df_len > 0)
  {
    df__axis2_len = jednostkiOb (df_len) ;
    strwyj = 1 ;
    return 1 ;
  }
  else return 0;
}

static int
L_angle (BOOL b_graph_value)
/*-------------------------*/
{
    double df_angle ;

    b_graph_value = b_graph_value ;
    if (eEA.val_no < 1)
    {
        return 0 ;
    }
    df_angle = eEA.values [0] ;

    df__angle_a = df_angle*Pi2/360.0;
    df__angle_a = fmod (df__angle_a, Pi2);
    if (df__angle_a<0) df__angle_a+=Pi2;

    strwyj = 1 ;
    return 1 ;
}


static void set_ellipse_draw_type (int i_type)
/*------------------------------------------*/
{
  int n ;

  s_ellipse.i_draw_type = i_type ;
  if (i_type == ELL_AXIS_END)
  {
    n = 0 ;
    komunikat0 (89) ;
  }
  else
  {
    n = 1 ;
    komunikat0 (91) ;
  }
  menu_par_new ((*m__ellipse.pola)[1].txt, pm_ell__draw_type [n].txt) ;
}

static void set_ellipse_draw_y_method (int i_method)
{  int n;
    n=i_method;

    if (y_axis==1)
    {
        if (s_ellipse.df_yaxis_method!=n)
        {
            if (n == 2) komunikat0(153);
            else if (n == 1) komunikat0(154);
            else komunikat0(92);

            if (s_ellipse.df_yaxis_method == 2)
            {
                eL.val_no_max = 1;
                eL.mode = GV_DIST;
                eL.format = "%#12.9lg";
                eL.ESTRF = L_axis2_len;
                eL.extend = 0;
                np = dodajstr(&eL);

                CUR_OFF = cur_el_off ;
                CUR_ON = cur_el_on ;

                sel.akt = el_sel_akt;
                sel.cur = el_sel_cur;
                sel.gor = el_sel_gor;
                sel.nr = el_sel_nr;
                sel.akt = el_sel_akt;
            }
            else if (n==2)
            {
                usunstr(np);
                el_sel_akt = sel.akt; sel.akt = 1;
                el_sel_cur = sel.cur; sel.cur=1;
                el_sel_gor = sel.gor; sel.gor = 0;
                el_sel_nr = sel.nr;
                el_sel_akt = sel.akt;  sel.akt=1;
                CUR_OFF = cur_el_off ;
                CUR_ON = cur_el_on ;
            }
        }
    }

    s_ellipse.df_yaxis_method=n;
    menu_par_new ((*m__ellipse.pola)[0].txt, pm_ell_y_draw_method [n].txt) ;
}


void set_ellipse_poligon_num (int msg_no)
//--------------------------------------
{
  int i_ell_verno ;
  char sk [MaxTextLen] = "" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d ;
  int ret;

  i_ell_verno = Get_Ell_Verno () ;

  //itoa (i_ell_verno, sk, 10) ;
    ret = snprintf(sk, 10, "%d",i_ell_verno);
  if (!get_string (sk, "", MaxTextLen, 0, msg_no)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = floor (buf_ret [0] + 0.5) ;
  if (d < 10) d=10;
  if (d >120) d=120;

}


static void set_filledellipsetranslucency(int poz0)
{
    char sk[10];
    int poz = poz0;

    float d_trans = 12.75;
    int transluc;

    FilledEllipseTranslucency = 255 - (int)((poz0-ELL_TRANS) * d_trans+0.5);

    transluc = (int)(FilledEllipseTranslucency / d_trans+0.5);
    sprintf(sk, "%d%%", transluc * 5);

    menu_par_new((*m__ellipse_d.pola)[1].txt, sk);
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
}

void set_angle_local (void)
{
    Set_Ell_Angle(0);
}

void set_angle_global (void)
{
    Set_Ell_Angle(1);
}


/*---------------------------------------------------------------------*/
/*---------------------2-2-2-------------------------------------------*/

static BOOL add_block (void)
/*------------------------*/
{
  BLOK s_blockd = Bdef ;
  BLOK *ptrs_block ;
  char c_pltype ;

  s_blockd.n = B3 + sizeof (c_pltype) ;
  s_blockd.kod_obiektu = B_PLINE ;
  s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype) ;
  s_blockd.obiektt2 = O2BlockPline ;
  if( (ptrs_block = (BLOK*)dodaj_obiekt (NULL, &s_blockd)) == NULL)
  {
    return FALSE ;
  }
  c_pltype = PL_ELLIPSE ;
  memmove (&(ptrs_block->opis_obiektu [0]), &c_pltype, sizeof(c_pltype)) ;
  return TRUE ;
}


static BOOL add_ellipse (double df_x, double df_y, BOOL b_strwyj)
/*-------------------------------------------------------------*/
{
    unsigned u_ell_size ;
    ELLIPSE ellipse=Edef;
    double angle;
    double kos;

    if (outline_disc==1)
    {
        ellipse.obiekt=Ofilledellipse;
        ellipse.translucency=FilledEllipseTranslucency;
    }

    Cur_offd (X, Y) ;
    if (b_strwyj)
    {
        s_ellipse.df_yaxis = df__axis2_len ;
    }
    else if (s_ellipse.df_yaxis_method==2)
    {
        ; //do nothing
    }
    else
    {
        if (s_ellipse.df_yaxis_method == 0) {
            s_ellipse.df_yaxis = Dist_PP(s_ellipse.df_cx, s_ellipse.df_cy, df_x, df_y);
        } else if (s_ellipse.df_yaxis_method == 1){
            //option point on ellipse
            Rotate_Point(s_ellipse.df_kos_b, s_ellipse.df_koc_b, s_ellipse.df_cx, s_ellipse.df_cy, df_x, df_y, &df_x,
                         &df_y);
            if (df_x > (s_ellipse.df_cx + s_ellipse.df_xaxis)) df_x = (s_ellipse.df_cx + s_ellipse.df_xaxis);
            else if (df_x < (s_ellipse.df_cx - s_ellipse.df_xaxis)) df_x = (s_ellipse.df_cx - s_ellipse.df_xaxis);

            angle = Angle_Normal(acos((df_x - s_ellipse.df_cx) / s_ellipse.df_xaxis));
            kos = sin(angle);
            if (Check_if_Equal(kos, 0.0)) s_ellipse.df_yaxis = fabs(df_x - s_ellipse.df_cx);
            else s_ellipse.df_yaxis = fabs((df_y - s_ellipse.df_cy) / kos);
        }
        else //2
        {
            //temporary
            s_ellipse.df_yaxis = Dist_PP(s_ellipse.df_cx, s_ellipse.df_cy, df_x, df_y);
        }
    }

    ellipse.kolor=LiniaG.kolor;
    ellipse.warstwa=LiniaG.warstwa;
    ellipse.typ=LiniaG.typ;
    ellipse.atrybut=NoElemBlok;
    ellipse.x=s_ellipse.df_cx;
    ellipse.y=s_ellipse.df_cy;

    ellipse.angle=s_ellipse.df_angle;
    ellipse.rx=s_ellipse.df_xaxis;
    ellipse.ry=s_ellipse.df_yaxis;

    if (dodaj_obiekt (NULL, (void*)&ellipse) == NULL)
    {
        return FALSE ;
    }

    rysuj_obiekt((void*)&ellipse, COPY_PUT, 1);

    return TRUE;
}

static BOOL add_ellipticalarc (void)
/*-------------------------------------------------------------------*/
{
    unsigned u_ell_size ;
    ELLIPTICALARC ellipticalarc=eldef;

    if (outline_disc==1) ellipticalarc.obiekt=Oellipticalarc;

    Cur_offd (X, Y) ;

    ellipticalarc.kolor=LiniaG.kolor;
    ellipticalarc.warstwa=LiniaG.warstwa;
    ellipticalarc.typ=LiniaG.typ;
    ellipticalarc.atrybut=NoElemBlok;
    ellipticalarc.x=s_ellipse.df_cx;
    ellipticalarc.y=s_ellipse.df_cy;

    ellipticalarc.angle=s_ellipse.df_angle;
    ellipticalarc.rx=s_ellipse.df_xaxis;
    ellipticalarc.ry=s_ellipse.df_yaxis;


    //TEMPORARY
    ellipticalarc.kat1=s_ellipse.df_sangle;
    ellipticalarc.kat2=s_ellipse.df_eangle;

    if (dodaj_obiekt (NULL, (void*)&ellipticalarc) == NULL)
    {
        return FALSE ;
    }

    rysuj_obiekt((void*)&ellipticalarc, COPY_PUT, 1);

    return TRUE;
}


static BOOL add_ellipse_convert (double df_x, double df_y, BOOL b_strwyj)
/*-----------------------------------------------------------------*/
{
  unsigned u_ell_size ;

  Cur_offd (X, Y) ;
  s_ellipse.df_yaxis = Dist_PP (s_ellipse.df_cx, s_ellipse.df_cy, df_x, df_y) ;
  if (b_strwyj)
  {
    s_ellipse.df_yaxis = df__axis2_len ;
  }
  if (FALSE == add_block ())
  {
    return FALSE ;
  }
  if (FALSE == Make_Ellipse (s_ellipse.df_cx, s_ellipse.df_cy,
    s_ellipse.df_xaxis, s_ellipse.df_yaxis,
    s_ellipse.df_angle, XOR_PUT, (BLOK**)&dane, NULL, DE_Add,O2BlockPline, outline_disc, 255, 0, 1))  //TO DO kolor, kolo0
  {
    usun_obiekt ((void *)dane) ;
    return FALSE ;
  }
  else
  {
    u_ell_size = sizeof (NAGLOWEK) + ((BLOK*)dane)->n ;
    blokzap (dane, dane + u_ell_size -1, Anormalny, COPY_PUT, 1) ;
    return TRUE ;
  }
}

static void cur_el_off (double x, double y)
{
    flip_screen();
}

static void cur_el1arc_off (double x, double y)
{
    flip_screen();
}

static void cur_el2arc_off (double x, double y)
{
    flip_screen();
}

static void cur_el_off__ (double x, double y)
/*--------------------------------------*/
{
  cursor_off(x, y);
  okno_r () ;
  if (LiniaG.kolor == 0) SetColorAC(15); else SetColorAC(LiniaG.kolor);
  
  setwritemode (XOR_PUT) ;
  linestyle_xor(LiniaG.typ);
  Make_Ellipse (s_ellipse.df_cx, s_ellipse.df_cy,
    s_ellipse.df_xaxis, s_ellipse.df_yaxis,
    s_ellipse.df_angle, XOR_PUT, NULL, NULL, DE_Draw, O2BlockPline, outline_disc, FilledEllipseTranslucency, LiniaG.kolor, 1) ;
  okno_all () ;
  outlineor (&line_g, XOR_PUT, 0) ;
}

static void cur_el_on (double df_x, double df_y)
/*--------------------------------------------*/
{
  double df_yaxis ;
  double angle_l;
  LINIA L1, L_=Ldef;
  double angle;
  double kos;
  double si, co;
  double x1, y1, x2, y2;

  line_g.x2 = df_x ;
  line_g.y2 = df_y ;
  line_g.kolor = Get_Current_Color () ;
  if(orto)
  {
  angle_l=get_angle_l();
  if (angle_l==0)
   {
    if (Orto_Dir == I_Orto_XDir)
    {
      df_y = s_ellipse.df_cy ;
    }
    else
    {
      df_x = s_ellipse.df_cx ;
    }
   }
   else
   {
    L1.x1=s_ellipse.df_cx;
    L1.y1=s_ellipse.df_cy;
    L1.x2=df_x;
    L1.y2=df_y;
    orto_l(&L1, &Orto_Dir);
    df_x=L1.x2;
    df_y=L1.y2;
   }
  }
  if (s_ellipse.df_yaxis_method<2) {

      if (s_ellipse.df_yaxis_method == 0) {
          //option vector
          s_ellipse.df_yaxis = Dist_PP(s_ellipse.df_cx, s_ellipse.df_cy, df_x, df_y);
      } else {
          //option point on ellipse
          Rotate_Point(s_ellipse.df_kos_b, s_ellipse.df_koc_b, s_ellipse.df_cx, s_ellipse.df_cy, df_x, df_y, &df_x,
                       &df_y);
          if (df_x > (s_ellipse.df_cx + s_ellipse.df_xaxis * yaxis_x_axis_f)) {
              df_x = (s_ellipse.df_cx + s_ellipse.df_xaxis * yaxis_x_axis_f);
              Rotate_Point(s_ellipse.df_kos, s_ellipse.df_koc, s_ellipse.df_cx, s_ellipse.df_cy, df_x, df_y, &X, &Y);
          } else if (df_x < (s_ellipse.df_cx - s_ellipse.df_xaxis * yaxis_x_axis_f)) {
              df_x = (s_ellipse.df_cx - s_ellipse.df_xaxis * yaxis_x_axis_f);
              Rotate_Point(s_ellipse.df_kos, s_ellipse.df_koc, s_ellipse.df_cx, s_ellipse.df_cy, df_x, df_y, &X, &Y);
          }

          angle = Angle_Normal(acos((df_x - s_ellipse.df_cx) / s_ellipse.df_xaxis));
          kos = sin(angle);
          if (Check_if_Equal(kos, 0.0)) s_ellipse.df_yaxis = fabs(df_x - s_ellipse.df_cx);
          else s_ellipse.df_yaxis = fabs((df_y - s_ellipse.df_cy) / kos);
      }

      df_yaxis = milimetryob(s_ellipse.df_yaxis);
      if (EOF != sprintf(eL.st, "%#12.9lg", df_yaxis)) {
          Out_Edited_Draw_Param((ESTR *) &eL, TRUE);
      }
      okno_r();
      if (LiniaG.kolor == 0) SetColorAC(15); else SetColorAC(LiniaG.kolor);
      setwritemode(COPY_PUT);
      linestyle(LiniaG.typ);
#ifdef ALLEGRO5
//      acquire_bitmap(screen);
      set_semaphore(0);
#endif
      Make_Ellipse(s_ellipse.df_cx, s_ellipse.df_cy,
                   s_ellipse.df_xaxis, s_ellipse.df_yaxis,
                   s_ellipse.df_angle, COPY_PUT, NULL, NULL, DE_Draw, O2BlockPline, outline_disc,
                   FilledEllipseTranslucency, LiniaG.kolor, 1);
#ifdef ALLEGRO5
//      release_bitmap(screen);
      set_semaphore(1);
#endif
      okno_all();

      outlineor(&line_g, COPY_PUT, 0);
      cursor_on(line_g.x2, line_g.y2);
  }
  else
  {
      if (LiniaG.kolor == 0) SetColorAC(15); else SetColorAC(LiniaG.kolor);
      setwritemode(COPY_PUT);
      linestyle(LiniaG.typ);
      LiniaG.x1=s_ellipse.df_cx-s_ellipse.df_xaxis;
      LiniaG.y1=s_ellipse.df_cy;
      LiniaG.x2=s_ellipse.df_cx+s_ellipse.df_xaxis;
      LiniaG.y2=s_ellipse.df_cy;
      si=sin(s_ellipse.df_angle);
      co=cos(s_ellipse.df_angle);
      Rotate_Point(si,co,s_ellipse.df_cx,s_ellipse.df_cy,LiniaG.x1,LiniaG.y1,&x1,&y1);
      Rotate_Point(si,co,s_ellipse.df_cx,s_ellipse.df_cy,LiniaG.x2,LiniaG.y2,&x2,&y2);
      LiniaG.x1=x1;
      LiniaG.y1=y1;
      LiniaG.x2=x2;
      LiniaG.y2=y2;

      outlineor (&LiniaG, COPY_PUT, 0) ;
      out_krz_sc(LiniaG.x1, LiniaG.y1);
      out_krz_sc(LiniaG.x2, LiniaG.y2);

      out_sel_on(df_x, df_y);
  }
}

void out_el_sel_off(double X, double Y)
{
    out_sel_off(X, Y);
}
void out_el_sel_on(double X, double Y)
{
    out_sel_on(X, Y);
}

static void cur_el1arc_on (double df_x, double df_y)
/*-----------------------------------------------*/
{
    double angle_l;
    LINIA L1;
    double dot, det;
    double df_sangle;

    line_g.x2 = df_x ;
    line_g.y2 = df_y ;
    line_g.kolor = Get_Current_Color () ;

    double vx1 = df_x-s_ellipse.df_cx;
    double vy1 = df_y-s_ellipse.df_cy;
    double vx2 = 1.0;
    double vy2 = 0.0;

    dot=vx1*vx2 + vy1*vy2;  // Dot product between [vx1, vy1] and [vx2, vy2]
    det=vx1*vy2 - vy1*vx2;  // Determinant
    s_ellipse.df_sangle = -atan2(det, dot) - s_ellipse.df_angle;  // atan2(y, x) or atan2(sin, cos)
    s_ellipse.df_sangle=fmod(s_ellipse.df_sangle, Pi2);
    if (s_ellipse.df_sangle<0)  s_ellipse.df_sangle+=Pi2;

    df_sangle=s_ellipse.df_sangle;
    if (Get_Ell_Angle()==1) df_sangle += s_ellipse.df_angle;
    df_sangle=fmod(df_sangle, Pi2);
    if (df_sangle<0)  df_sangle+=Pi2;

    if (EOF != sprintf (eEA.st, "%#12.9lg", df_sangle*360.0/Pi2))
    {
        Out_Edited_Draw_Param ((ESTR *)&eEA, TRUE) ;
    }


    okno_r () ;
    setcolor(kolory.blok);
    setwritemode (COPY_PUT) ;
    linestyle (LiniaG.typ) ;
    linestyle_solid ((LiniaG.typ & 224) / 32) ;
    Make_Ellipse (s_ellipse.df_cx, s_ellipse.df_cy,
                        s_ellipse.df_xaxis, s_ellipse.df_yaxis,
                        s_ellipse.df_angle, COPY_PUT, NULL, NULL, DE_Draw, O2BlockPline, outline_disc, 255, LiniaG.kolor, 1) ;
    okno_all () ;

    if (LiniaG.kolor == 0) SetColorAC(15); else SetColorAC(LiniaG.kolor);
    outlineor (&line_g, COPY_PUT, 0) ;
    cursor_on(line_g.x2, line_g.y2);
}



static void cur_el2arc_on (double df_x, double df_y)
/*-----------------------------------------------*/
{
    double angle_l;
    LINIA L1;
    double dot, det;
    double df_eangle;

    line_g.x2 = df_x ;
    line_g.y2 = df_y ;
    line_g.kolor = Get_Current_Color () ;

    double vx1 = df_x-s_ellipse.df_cx;
    double vy1 = df_y-s_ellipse.df_cy;
    double vx2 = 1.0;
    double vy2 = 0.0;

    dot=vx1*vx2 + vy1*vy2;  // Dot product between [vx1, vy1] and [vx2, vy2]
    det=vx1*vy2 - vy1*vx2;  // Determinant
    s_ellipse.df_eangle = -atan2(det, dot) - s_ellipse.df_angle;  // atan2(y, x) or atan2(sin, cos)
    s_ellipse.df_eangle=fmod(s_ellipse.df_eangle, Pi2);
    if (s_ellipse.df_eangle<0)  s_ellipse.df_eangle+=Pi2;

    df_eangle=s_ellipse.df_eangle;
    if (Get_Ell_Angle()==1) df_eangle += s_ellipse.df_angle;
    df_eangle=fmod(df_eangle, Pi2);
    if (df_eangle<0)  df_eangle+=Pi2;

    if (EOF != sprintf (eEA.st, "%#12.9lg", df_eangle*360.0/Pi2))
    {
        Out_Edited_Draw_Param ((ESTR *)&eEA, TRUE) ;
    }
    okno_r () ;
    if (LiniaG.kolor == 0) SetColorAC(15); else SetColorAC(LiniaG.kolor);
    setwritemode (COPY_PUT) ;
    linestyle (LiniaG.typ) ;
    Make_EllipticalArc (s_ellipse.df_cx, s_ellipse.df_cy,
                  s_ellipse.df_xaxis, s_ellipse.df_yaxis,
                  s_ellipse.df_angle, s_ellipse.df_sangle, s_ellipse.df_eangle, COPY_PUT, NULL, NULL, DE_Draw, Anormalny, Guma, O2BlockPline, outline_disc, LiniaG.kolor, 1) ;
    okno_all () ;

    outlineor (&line_g, COPY_PUT, 0) ;
    cursor_on(line_g.x2, line_g.y2);
}



static void redcr2 (char typ, double df_x , double df_y)
/*-------------------------------------------------------------*/
{

  double angle_l;
  LINIA L1;
  static TMENU *menu ;
  static char st[POLE_TXT_MAX];
  static unsigned short cod = L' ';
  char sz_sk [POLE_TXT_MAX] ;
  static int iconno=0;
  int ret;

  if (typ==0)
  {
      el_sel_akt = sel.akt;
      el_sel_cur = sel.cur;
      el_sel_gor = sel.gor;
      el_sel_nr = sel.nr;
      el_sel_akt=sel.akt;

     y_axis=1;
     getmenupini (&menu, st, &cod, &iconno) ;
     if (s_ellipse.df_yaxis_method==2) komunikat0 (153) ;
     else if (s_ellipse.df_yaxis_method==1) komunikat0 (154) ;
     else komunikat0 (92) ;
     Cur_offd (X, Y) ;
     if(orto)
     {
     angle_l=get_angle_l();
      if (angle_l==0)
      {
       if (Orto_Dir == I_Orto_XDir)
       {
	 df_y = s_ellipse.df_1y ;
       }
       else
       {
	 df_x = s_ellipse.df_1x ;
       }
      }
      else
      {

       L1.x1=s_ellipse.df_1x;
       L1.y1=s_ellipse.df_1y;
       L1.x2=df_x;
       L1.y2=df_y;
       orto_l(&L1, &Orto_Dir);
       df_x=L1.x2;
       df_y=L1.y2;
      }
     }
     s_ellipse.df_2x = df_x ;
     s_ellipse.df_2y = df_y ;
     s_ellipse.df_cx = s_ellipse.df_1x ;
     s_ellipse.df_cy = s_ellipse.df_1y ;
     if (s_ellipse.i_draw_type == ELL_AXIS_END)
     {
       s_ellipse.df_cx = (s_ellipse.df_2x + s_ellipse.df_1x) / 2 ;
       s_ellipse.df_cy = (s_ellipse.df_2y + s_ellipse.df_1y) / 2 ;
     }
     s_ellipse.df_angle =
	   Angle_Normal (Atan2 (s_ellipse.df_2y - s_ellipse.df_1y,
				s_ellipse.df_2x - s_ellipse.df_1x)) ;
     s_ellipse.df_xaxis = Dist_PP (s_ellipse.df_cx, s_ellipse.df_cy,
				 s_ellipse.df_2x, s_ellipse.df_2y) ;

     s_ellipse.df_kos=sin(s_ellipse.df_angle);
     s_ellipse.df_koc=cos(s_ellipse.df_angle);
      s_ellipse.df_kos_b=sin(-s_ellipse.df_angle);
      s_ellipse.df_koc_b=cos(-s_ellipse.df_angle);

     line_g.x1 = s_ellipse.df_cx ;
     line_g.y1 = s_ellipse.df_cy ;
     line_g.x2 = X ;
     line_g.y2 = Y ;
     if (s_ellipse.df_yaxis_method<2) {
         eL.val_no_max = 1;
         eL.mode = GV_DIST;
         eL.format = "%#12.9lg";
         eL.ESTRF = L_axis2_len;
         eL.extend = 0;
         np = dodajstr(&eL);

         CUR_oFF2 = CUR_OFF; CUR_OFF = cur_el_off ;
         CUR_oN2 = CUR_ON ;   CUR_ON = cur_el_on ;
     }
     else
     {
         sel.akt = 1;
         sel.cur=1;
         sel.gor = 0;
         sel.akt=1;
         CUR_oFF2 = CUR_OFF; CUR_OFF = cur_el_off ;
         CUR_oN2 = CUR_ON ;   CUR_ON = cur_el_on ;
     }


     Cur_ond (X, Y) ;
  }
  else if (typ==2)
  {

      ret = snprintf(sz_sk, 32, "%s",EllipticalAngle[Get_Ell_Angle ()]);
      getmenupini (&menu, st, &cod, &iconno) ;

      menupini (&m__ellipticalarc, _ELLIPSE_ANGLE_, _ELLIPSE_ANGLE_C_, 706) ;

      menu_par_new ((*menup.pola)[6].txt, sz_sk) ;

      komunikat0 (151) ;
      Cur_offd (X, Y) ;

      CUR_OFF = CUR_oFF2;
      CUR_ON = CUR_oN2 ;

      eEA.x = maxX - PL266 ;
      eEA.y = ESTR_Y;
      eEA.lmax = r22 ;

      eEA.val_no_max = 1 ;
      eEA.mode = GV_ANGLE ;
      eEA.format = "%#12.9lg" ;
      eEA.ESTRF = L_angle ;
      eEA.extend = 0;
      np = dodajstr (&eEA) ;

      CUR_oFF1 = CUR_OFF ;  CUR_OFF = cur_el1arc_off ;
      CUR_oN1 = CUR_ON ;   CUR_ON = cur_el1arc_on ;
      Cur_ond (X, Y) ;
  }
  else if (typ==3)
  {
      ret = snprintf(sz_sk, 32, "%s",EllipticalAngle[Get_Ell_Angle ()]);
      getmenupini (&menu, st, &cod, &iconno) ;

      menupini (&m__ellipticalarc, _ELLIPSE_ANGLE_, _ELLIPSE_ANGLE_C_, 706) ;

      menu_par_new ((*menup.pola)[6].txt, sz_sk) ;

      komunikat0 (152) ;
      Cur_offd (X, Y) ;

      CUR_OFF = CUR_oFF1 ;
      CUR_ON = CUR_oN1 ;

      eEA.val_no_max = 1 ;
      eEA.mode = GV_ANGLE ;
      eEA.format = "%#12.9lg" ;
      eEA.ESTRF = L_angle ;
      eEA.extend = 0;
      np = dodajstr (&eEA) ;

      CUR_oFF1 = CUR_OFF ;  CUR_OFF = cur_el2arc_off ;
      CUR_oN1 = CUR_ON ;   CUR_ON = cur_el2arc_on ;
      Cur_ond (X, Y) ;
  }
  else
  {
      y_axis=0;
      menupini (menu, st, cod, iconno) ;

      sel.akt = el_sel_akt;
      sel.cur = el_sel_cur;
      sel.gor = el_sel_gor;
      sel.nr = el_sel_nr;
      sel.akt = el_sel_akt;

      CUR_OFF = CUR_oFF2;
      CUR_ON = CUR_oN2 ;

      if (s_ellipse.df_yaxis_method<2) usunstr (np) ;
  }
}

static void *obiekt_wybrany_tangent (void)
/*--------------------------------------*/
{ unsigned typ;
    typ=Blinia | Bwwielokat;
    return select_w(&typ,NULL);
}

BOOL get_ellipse_second_axis(t_ellipse *s_ellipse_, void *ad)
{   LINIA *L, Lw=Ldef;
    PLINIA PL;
    double angle, angle1;
    double cot2;
    double si, co;
    double x1, y1, x2, y2, y3, y4;
    double mm, cc;
    double tkat;

    if (((NAGLOWEK*)ad)->obiekt==Olinia)
    {
        L=(LINIA*)ad;
        memcpy(&Lw, L, sizeof(LINIA));
    }
    else if (((NAGLOWEK*)ad)->obiekt==Owwielokat)
    {
        wielokat_linia_ (&X, &Y, ad, &Lw, najblizszyL_) ;
    }
    else return FALSE;

    si=sin(-s_ellipse_->df_angle);
    co=cos(-s_ellipse_->df_angle);

    Lw.x1-=s_ellipse_->df_cx;
    Lw.y1-=s_ellipse_->df_cy;
    Lw.x2-=s_ellipse_->df_cx;
    Lw.y2-=s_ellipse_->df_cy;

    Rotate_Point(si, co, 0, 0, (double)Lw.x1, (double)Lw.y1, &x1, &y1);
    Rotate_Point(si, co, 0, 0, (double)Lw.x2, (double)Lw.y2, &x2, &y2);

    angle = Angle_Normal(Atan2(y2-y1, x2-x1));

    if (Check_if_Equal(fabs(x2-x1), 0.0)==TRUE) return FALSE;

    mm=(y2-y1)/(x2-x1);
    cc=y1-mm*x1;

    y3=fabs(mm*s_ellipse_->df_xaxis+cc);
    y4=fabs(mm*-s_ellipse_->df_xaxis+cc);

    if (y4<y3) y3=y4;

    tkat=fabs(tan(angle));

    s_ellipse_->df_yaxis=sqrt(fabs(y3*y3+2*y3*s_ellipse_->df_xaxis*tkat));

    return TRUE;
}

static void
ellipse_proc2 (double df_x, double df_y)
/*------------------------------------*/
{
  EVENT *ev;
  void *ad;
  BOOL ret;

  redcr2 (0, df_x, df_y) ;
  while (1)
  {
    strwyj = 0 ;
    ev = Get_Event_Point (NULL, &df_x, &df_y) ;
    if (ev->What == evKeyDown &&  ev->Number == 0)
    {
      Cur_offd (X, Y) ;
      redcr2 (1,0,0) ;
      return ;
    }
    if ((ev->What == evKeyDown &&  ev->Number==ENTER)  || strwyj)
    {
      if (s_ellipse.df_yaxis_method==2)
      {
          if ((ad = obiekt_wybrany_tangent ()) != NULL)
          {
               ret=get_ellipse_second_axis(&s_ellipse, ad);
               if (!ret) continue;
          }
          else continue;
      }
      add_ellipse (df_x, df_y, strwyj) ;
      redcr2 (1,0,0) ;
      flip_screen();
      return ;
    }
    if (ev->What == evCommandP)
    {
        switch (ev->Number)
        {
            case ELL_DIS:
            case ELL_POINT:
            case ELL_TAN:
            set_ellipse_draw_y_method(ev->Number);
            break;
            continue;
            default:
            set_filledellipsetranslucency(ev->Number);
            break;
        }
    }
  }
}

enum PLINE_MODE {PL_MODE_CONTINUE = 1, PL_MODE_LINE , PL_MODE_ARC,
    PL_MODE_END, PL_MODE_UNDO, PL_MODE_ARC_ESC, PL_MODE_ARC_CR,
    PL_MODE_UNDO_CONT, PL_MODE_ARC_CONTINUE} ;

int getwsp_ellipticalarc (double *X0, double *Y0)
/*--------------------------------------------*/
{
    int ret_command ;
    EVENT *ev;

    while (1)
    {
        view_line_type(&LiniaG);
        ev = Get_Event_Point (NULL, X0, Y0) ;
        if (ev->What == evKeyDown &&  ev->Number == 0) {

            ret_command = PL_MODE_ARC_ESC;
            break;
        }
        if ((ev->What == evKeyDown &&  ev->Number==ENTER)  || strwyj)
        {
            ret_command = PL_MODE_ARC_CR ;
            break;
        }

        if( ev->What == evCommandP)
        {
            if (ev->Number == ELL_LOCAL) set_angle_local () ;
            else if (ev->Number == ELL_GLOBAL) set_angle_global () ;

            continue ;
        }

    }
    return ret_command ;
}

static void  cur_off_ea(double x,double y)
{
    flip_screen();
}

static void  cur_on_ea(double x,double y)
{
    cursor_on(x,y);
}

static void redcr_ea(char typ)
{
    static void (*CUR_oN)(double ,double);
    static void (*CUR_oFF)(double ,double);
    if (typ==0)
    {
        mvcurb.EA=1;
        Cur_offd(X,Y);
        CUR_oFF=CUR_OFF;  CUR_OFF=cur_off_ea;
        CUR_oN=CUR_ON;   CUR_ON=cur_on_ea;
        Cur_ond (X, Y) ;
    }
    else
    {
        Cur_offd(X,Y);
        CUR_OFF=CUR_oFF;
        CUR_ON=CUR_oN;
        Cur_ond (X, Y) ;
    }
}

int Getwsp1_ea(void)
{
    double X0,Y0;
    EVENT *ev;

    redcr_ea(0);
    strwyj=0;
    while(1)
    { view_line_type(&LiniaG);
        ev=Get_Event_Point(NULL, &X0, &Y0);
        if(ev->What == evKeyDown && ev->Number==0)
        {
            redcr_ea(1);
            return 0;
        }
        if((ev->What == evKeyDown && ev->Number== ENTER) || strwyj)
        {
            Cur_offd(X,Y);
            ////add_arc (X0, Y0, strwyj) ;
            Cur_ond(X,Y);
            redcr_ea(1);
            return 1;
        }
    }
}

static void near ESCA(double X0, double Y0)  /*poczatek srodek kat*/
{
   ;
}

static void near ESCE(double X0, double Y0)  /* poczatek srodek koniec */
{

    LukG.x=X0; LukG.y=Y0;
    out_krz(LukG.x,LukG.y);
    if(fabs(LukG.x-pel.xs)<OZero && fabs(LukG.y-pel.ys)<OZero) return;
    LukG.r=dP1P2(pel.xs,pel.ys,LukG.x,LukG.y);
    LukG.kat1=LukG.kat2=0;
    L.x1=LukG.x;
    L.y1=LukG.y;
    L.x2=X;
    L.y2=X;
    Getwsp1_ea();
}

static void
ellipticalarc_proc2 (double df_x, double df_y)
/*------------------------------------------*/
{
    EVENT *ev;
    double X0=0, Y0=0 ;
    int ret_command ;
    double dot,det;
    double df_x0, df_y0;
    double vx1, vy1, vx2, vy2;

    df_x0=df_x;
    df_y0=df_y;

    redcr2 (0, df_x, df_y) ;
    while (1)
    {
        strwyj = 0 ;
        ev = Get_Event_Point (NULL, &df_x, &df_y) ;
        if (ev->What == evKeyDown &&  ev->Number == 0)
        {
            Cur_offd (X, Y) ;
            redcr2 (1,0,0) ;
            return ;
        }
        if ((ev->What == evKeyDown &&  ev->Number==ENTER)  || strwyj)
        {
        first_angle:
            redcr2(1, 0, 0);
            s_ellipse.df_yaxis = Dist_PP(s_ellipse.df_cx, s_ellipse.df_cy, df_x, df_y);
            if (strwyj) {
                s_ellipse.df_yaxis = df__axis2_len;
            }

            redcr2(2, X0, Y0);

            strwyj=0;

            if (PL_MODE_ARC_ESC == getwsp_ellipticalarc(&X0, &Y0)) {
                redcr2(1, X0, Y0);
                redcr2(0, df_x0, df_y0);
                continue;
            }

            redcr2(1, X0, Y0);

            if (strwyj) {
                s_ellipse.df_sangle = df__angle_a;
                if (Get_Ell_Angle()==1) s_ellipse.df_sangle -= s_ellipse.df_angle;
                s_ellipse.df_sangle=fmod(s_ellipse.df_sangle, Pi2);
                if (s_ellipse.df_sangle < 0) s_ellipse.df_sangle += Pi2;
            }

            else {

                vx1 = X0 - s_ellipse.df_cx;
                vy1 = Y0 - s_ellipse.df_cy;
                vx2 = 1.0;
                vy2 = 0.0;

                dot = vx1 * vx2 + vy1 * vy2;  // Dot product between [vx1, vy1] and [vx2, vy2]
                det = vx1 * vy2 - vy1 * vx2;  // Determinant
                s_ellipse.df_sangle = -atan2(det, dot) - s_ellipse.df_angle;  // atan2(y, x) or atan2(sin, cos)
                s_ellipse.df_sangle=fmod(s_ellipse.df_sangle, Pi2);
                if (s_ellipse.df_sangle < 0) s_ellipse.df_sangle += Pi2;
            }

            redcr2(3, X0, Y0);

            pel.xs = X0;
            pel.ys = Y0;
            out_krz(pel.xs, pel.ys);

            strwyj=0;

            if (PL_MODE_ARC_ESC == getwsp_ellipticalarc(&X0, &Y0)) {
                goto first_angle;
            }

            if (strwyj) {
                s_ellipse.df_eangle = df__angle_a;
                if (Get_Ell_Angle()==1) s_ellipse.df_eangle -= s_ellipse.df_angle;
                s_ellipse.df_eangle=fmod(s_ellipse.df_eangle, Pi2);
                if (s_ellipse.df_eangle < 0) s_ellipse.df_eangle += Pi2;
            }

            else {

                vx1 = X0 - s_ellipse.df_cx;
                vy1 = Y0 - s_ellipse.df_cy;
                vx2 = 1.0;
                vy2 = 0.0;

                dot = vx1 * vx2 + vy1 * vy2;     // Dot product between [vx1, vy1] and [vx2, vy2]
                det = vx1 * vy2 - vy1 * vx2;     // Determinant
                s_ellipse.df_eangle = -atan2(det, dot) - s_ellipse.df_angle;  // atan2(y, x) or atan2(sin, cos)
                s_ellipse.df_eangle=fmod(s_ellipse.df_eangle, Pi2);
                if (s_ellipse.df_eangle < 0) s_ellipse.df_eangle += Pi2;
            }

            redcr2 (1, df_x, df_y) ;

            add_ellipticalarc () ;
            flip_screen();
            return ;
        }

    }
}


/*---------------------------------------------------------------------*/
/*---------------------1-1-1-------------------------------------------*/

static void  cur_off (double x, double y)
/*--------------------------------------*/
{
    flip_screen();
}
/*
static void  cur_off__ (double x, double y)
//----------------------------------------
{
  cursor_off (x, y) ;
  outlineor (&line_g, XOR_PUT, 0) ;
}
*/
static void  cur_on (double x, double y)
/*--------------------------------------*/
{
  line_g.x2 = x ;
  line_g.y2 = y ;
  line_g.kolor = Get_Current_Color () ;
  outlineor (&line_g, COPY_PUT, 1) ;
  cursor_on(x, y);
}


static void redcr1 (char type)
/*--------------------------*/
{
  static int KR0, np ;
  static TMENU *menu ;
    static char st[POLE_TXT_MAX];
    static unsigned short cod = L' ';
    char sz_sk [10] ;
    static int iconno=0;
    float d_trans = 12.75;
    int transluc;
    static char sk[16];

  switch (type)
  {
    case 0 :

        getmenupini (&menu, st, &cod, &iconno) ;
        if (outline_disc==1)
        {

            menu_par_new ((*m__ellipse.pola)[0].txt, pm_ell_y_draw_method [s_ellipse.df_yaxis_method].txt) ;
            transluc = (int)((float)FilledEllipseTranslucency / d_trans +0.5);
            sprintf(sk, "%d%%", transluc * 5);
            menu_par_new((*m__ellipse_d.pola)[1].txt, sk);
            menupini (&m__ellipse_d, _ELLIPTICALDISC_, _ELLIPTICALDISC_C_, 707) ;
        }
        else {

            menu_par_new ((*m__ellipse.pola)[0].txt, pm_ell_y_draw_method [s_ellipse.df_yaxis_method].txt) ;
            menupini (&m__ellipse_e, _ELLIPSE_, _ELLIPSE_C_, 19) ;
        }
      KR0 = Komunikat_R0 ;
      komunikat0 (90) ;
      eL.ESTRF = L_p ;
	  eL.extend = 0;
      eL.val_no_max = 2 ;
      eL.mode = GV_VECTOR ;
      eL.format = "%#12.9lg\0%#12.9lg;%#8.3lf" ;
      np = dodajstr (&eL) ;
      Cur_offd (X, Y) ;
      CUR_oFF0 = CUR_OFF ;  CUR_OFF = cur_off ;
      CUR_oN0 = CUR_ON ;   CUR_ON = cur_on ;
      Cur_ond (X, Y) ;
      break ;
    case 1 :
      Cur_offd (X, Y) ;
      usunstr (np) ;
    case 3 :
      CUR_OFF = CUR_oFF0 ;
      CUR_ON = CUR_oN0 ;
      komunikat0 (KR0) ;
      Cur_ond (X, Y) ;
      menupini (menu, st, cod, iconno) ;
      break ;
    case 2 :
     usunstr (np) ;
     break ;
  }
}

static void
ellipse_proc1 (double df_x, double df_y)
/*------------------------------------*/
{
  EVENT *ev;

  s_ellipse.df_1x = df_x ;
  s_ellipse.df_1y = df_y ;
  line_g.x1 = df_x ;
  line_g.y1 = df_y ;
  line_g.x2 = X ;
  line_g.y2 = Y ;

  redcr1 (0) ;
  while (1)
  {
    strwyj = 0 ;
    ev = Get_Event_Point (NULL, &df_x, &df_y) ;
    if (ev->What == evKeyDown &&  ev->Number == 0)
    {
      redcr1 (1) ;
      return ;
    }
    if ((ev->What == evKeyDown &&  ev->Number==ENTER)  || strwyj)
    {
      if (strwyj)
      {
	df_x = Lx2 ;
	df_y = Ly2 ;
      }
      if (TRUE == Check_if_Equal (s_ellipse.df_1x, df_x) &&
	  TRUE == Check_if_Equal (s_ellipse.df_1y, df_y))
      {
	continue ;
      }
      redcr1 (2) ;
      ellipse_proc2 (df_x, df_y) ;
      redcr1 (3) ;
      return ;
    }
    if (ev->What == evCommandP )
    {
        switch (ev->Number)
        {
            case ELL_DIS:
            case ELL_POINT:
            case ELL_TAN:
                set_ellipse_draw_y_method(ev->Number);
                break;
            default:
                set_filledellipsetranslucency(ev->Number);
                break;
        }
    }
  }
}

static void
ellipticalarc_proc1 (double df_x, double df_y)
/*------------------------------------*/
{
    EVENT *ev;

    s_ellipse.df_1x = df_x ;
    s_ellipse.df_1y = df_y ;
    line_g.x1 = df_x ;
    line_g.y1 = df_y ;
    line_g.x2 = X ;
    line_g.y2 = Y ;

    redcr1 (0) ;
    while (1)
    {
        strwyj = 0 ;
        ev = Get_Event_Point (NULL, &df_x, &df_y) ;
        if (ev->What == evKeyDown &&  ev->Number == 0)
        {
            redcr1 (1) ;
            return ;
        }
        if ((ev->What == evKeyDown &&  ev->Number==ENTER)  || strwyj)
        {
            if (strwyj)
            {
                df_x = Lx2 ;
                df_y = Ly2 ;
            }
            if (TRUE == Check_if_Equal (s_ellipse.df_1x, df_x) &&
                TRUE == Check_if_Equal (s_ellipse.df_1y, df_y))
            {
                continue ;
            }
            redcr1 (2) ;
            ellipticalarc_proc2 (df_x, df_y) ;
            redcr1 (3) ;
            return ;
        }
        if (ev->What == evCommandP)
        {
            switch (ev->Number)
            {
                case ELL_DIS:
                case ELL_POINT:
                case ELL_TAN:
                    set_ellipse_draw_y_method(ev->Number);
                    break;
                default:
                    break;
            }
        }
    }
}


/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/


static void redcr (char typ)
/*-------------------------*/
{
  static int ( *SW[2])(), akt ;
  static TMENU *menu ;
  static char st[POLE_TXT_MAX];
  static unsigned short cod = L' ';
  char sz_sk [10] ;
  static int iconno=0;
  int ret;

  if (typ==0)
  {
	 CUR_OFF(X, Y);
     komunikat (0) ;
     m__ellipse.max = 2 ;
     LiniaG.blok = 1 ;
     LiniaG.obiektt2 = O2BlockPline ;
    set_ellipse_draw_type (s_ellipse.i_draw_type) ;
      menu_par_new ((*m__ellipse.pola)[0].txt, pm_ell_y_draw_method [s_ellipse.df_yaxis_method].txt) ;
     akt = sel.akt ; sel.akt = ASel ;
     cursor_off = cursel_off;
	 cursor_on = cursel_on ;
     eL.x = maxX - PL266 ;
     eL.y = ESTR_Y ;
     eL.lmax = r22 ;
     getmenupini (&menu, st, &cod, &iconno) ;
     menupini (&m__ellipse, _ELLIPSE_, _ELLIPSE_C_, 19) ;
     CUR_oFF = CUR_OFF ;  CUR_OFF = cursel_off ;
     CUR_oN = CUR_ON ;    CUR_ON = cursel_on ;
     SW [0] = SERV [73] ; SERV [73] = sel_t ;
     SW [1] = SERV [81] ; SERV [81] = sel_d ;
     Cur_ond (X, Y) ;
  }
  else
  {
     Cur_offd (X, Y) ;
     LiniaG.blok = 0 ;
     LiniaG.obiektt2 = O2NoBlockS ;
     CUR_OFF = CUR_oFF ;
     CUR_ON = CUR_oN ;
     SERV [73] = SW [0] ;
     SERV [81] = SW [1] ;
     sel.akt = akt ;
     menupini (menu, st, cod, iconno) ;
     komunikat0 (0) ;
	 CUR_ON(X, Y);
  }
}

void Ellipse_factory (void)
/*-----------------------*/
{
  EVENT *ev;
  double X0, Y0;

  while (1)
  {
    redcr (0) ;
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    switch (ev->What) {
        case evKeyDown :
            if (ev->Number == 0) {
                redcr(1);
                return;
            }
            if (ev->Number == ENTER) {
                ellipse_proc1(X0, Y0);
                redcr(1);
            }
            break;
        case evCommandP :
            switch (ev->Number)
            {
                case ELL_DIS:
                case ELL_POINT:
                case ELL_TAN:
                    set_ellipse_draw_y_method(ev->Number);
                    break;
                case ELL_AXIS_END:
                case ELL_CENTER:
                    set_ellipse_draw_type(ev->Number);
                    break;
                default:
                    break;
            }
            redcr(1);
	break ;
      default :
	break;
    }
  }
}


void EllipticalArc_factory (void)
/*-----------------------*/
{
    EVENT *ev;
    double X0, Y0;

    while (1)
    {
        redcr (0) ;
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
                    ellipticalarc_proc1 (X0, Y0) ;
                    redcr (1) ;
                }
                break;
            case evCommandP :
                switch (ev->Number)
                {
                    case ELL_DIS:
                    case ELL_POINT:
                    case ELL_TAN:
                        set_ellipse_draw_y_method(ev->Number);
                        break;
                    case ELL_AXIS_END:
                    case ELL_CENTER:
                        set_ellipse_draw_type(ev->Number);
                        break;
                    default:
                        break;
                }
                break ;
            default :
                break;
        }
    }
}


void Ellipse (void)
{
    outline_disc=0;
    Ellipse_factory ();
}


void FilledEllipse(void)
{
    outline_disc=1;
    Ellipse_factory ();
}

void EllipticalArc(void)
{
    outline_disc=0;
    EllipticalArc_factory ();
}


#undef __O_ELLIPS__