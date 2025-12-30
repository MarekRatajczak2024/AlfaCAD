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

#define __O_MEASURE__
#include <forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_measur.h"
#include "o_libfun.h"

#include "message.h"
   
static int  nooop1(void)
{return 0;}
#define noop (int ( *)(void))nooop1
extern int Get_Current_Color (void) ;
extern int Save_Message0_Image (void) ;
extern void Put_Message0_Image (int) ;
extern BOOL Add_String_To_List (char *) ;
extern void Set_Float_Flag (BOOL) ;
extern void redcrMP (int type, int type1);
extern int Change_Orto (void) ;
extern void setfillstyle_(int pattern, int color);
extern void setcolor(int kolor);
extern BOOL axis_point (double *df_apx, double *df_apy);
extern int Get_Key (void);
extern void standard_func(void);
extern void delay(int time);
extern void orto_l(LINIA *L, int *Orto_Dir);
extern void cursel_off_pcx(double x, double y);
extern void cursel_on_pcx(double x, double y);
extern void cursel_hand_on(double x, double y);
extern int Solid04(char* adr0, char* adr1, int kolor);
extern void Out_Edited_Draw_Param (ESTR *lps_et, BOOL out) ;

extern void Get_Graph_Values_Cur_Pos (double *ptrdf_x, double *ptrdf_y);
extern void Xp_Yp_move(double ddx, double ddy);
extern void draw_keyimage_(int keyimage, BOOL set_timer, BOOL silent, BOOL enforce);

extern void draw_mickey_hand(double x, double y);
extern void del_demo_keys(void);
extern void draw_keyimages(void);
extern void draw_demokeyimages(void);
extern void restore_keyimages(void);
extern void DoneKeyimage(void);
extern BOOL get_DEMO_RECORDING(void);
extern void draw_demokeyimage_(int keyimage, int keyimage1, BOOL set_timer, BOOL silent, BOOL enforce);
extern void	layer_info (void);
extern void flip_shift_screen(LINIA *line_g0);

extern double cut_val(double jXY);
extern double my_round(double x, unsigned int digits);
extern int isometric_vector_to_cartesian(double dx_iso, double dy_iso, double *dx_cart, double *dy_cart);
extern int cartesian_to_isometric(double cx, double cy, double *ix, double *iy);

// Inverse of milimetryobx
extern double milimetryobx_inv(double obj_x);
// Inverse of milimetryoby
extern double milimetryoby_inv(double obj_y);

#define cursor_off cursel_off
#define cursor_on cursel_on

enum DIST_APS {POINT_DIST, ADD_DIST, SUB_DIST} ;
enum DIST_COMND {POINT_DIST_COMND, ADD_SUB_DIST_COMND} ;

enum ANGLE_STATE {ANGLE0 = 0, ANGLE1, ANGLE2 } ;

static double df_apx1, df_apy1, df_apx2, df_apy2 ;
static int angle_state = ANGLE0 ;
static int angle_ok = 0 ;
static LINIA line_g = Ldef  ;
static LINIA line_ge = Ldef, line_ge1 = Ldef ;
static int mode_dist ;
static double df__total_dist ;
static BOOL b__active_dist = FALSE ;
static BOOL b__active_point = FALSE ;
static BOOL b__active_angle = FALSE ;
static double df__xprev, df__yprev ;
static int out_angle_ok1 ;

#define r22 16
static int strwyj;
static double Lx2,Ly2,Lz2 ;
static double kos0,koc0;

int isometric_to_cartesian(double ix, double iy, double *cx, double *cy);

WIELOKAT S4_PCX = S4def;
B_PCX *GlobalPCX;

static void  nooop(void)
{
    return;
}
/*
double my_round(double x, unsigned int digits) {
    double fac = pow(10, digits);
    return round(x*fac)/fac;
}
*/

int Break_Sum_Measure (void)
/*-------------------------*/
{
  if (b__active_dist == FALSE)
  {
    return 0 ;
  }
  setfillstyle_(SOLID_FILL, BKCOLOR) ;
  bar (maxX / 2 + 1, 0, maxX / 2 + 60, 9) ;
  mode_dist = POINT_DIST ;
  df__total_dist = 0 ;
  return 0 ;
}

int Sum_Measure (void)
/*--------------------*/
{
  if (b__active_dist == FALSE)
  {
    return 0 ;
  }
  setfillstyle_(SOLID_FILL, BKCOLOR) ;
  bar (maxX / 2 + 1, 0, maxX / 2 + 60, 9) ;
  moveto (maxX / 2 + 1, 1) ;
  setcolor (kolory.ink) ;
  if(mode_dist == ADD_DIST)
  {
    mode_dist = SUB_DIST ;
    outtext_r (_SUBSTRACT_) ;
  }
  else
  {
    mode_dist = ADD_DIST ;
    outtext_r (_ADD_) ;
  }
  return 0 ;
}


static void out_pole (double p0)
/*----------------------------------------*/
{
  double jp ;
  char buf [100] ;

  sprintf (buf, "%s = %-12.7lg", measure_comm[0], p0) ;
  Add_String_To_List (buf) ;
  komunikat_str (buf) ;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*-----------    POINT  ------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

static void out_coord0 (double X0, double Y0)
/*----------------------------------------*/
{
  double jX, jY,jX0,jY0;
  double sina1, cosa1;
  char buf [100] ;

  jX = milimetryob (X0);
  jY = milimetryob (Y0);


  if (options1.uklad_geodezyjny==0) sprintf (buf, "%s <0,0>: X = %-16.12lg Y = %-16.12lg", measure_comm[1],jX, jY) ;
    else sprintf (buf, "%s <0,0>: X = %-16.12lg Y = %-16.12lg", measure_comm[1], jY, jX) ;
  Add_String_To_List (buf) ;
  komunikat_str (buf) ;
}


static void out_coord (double X0, double Y0)
/*----------------------------------------*/
{
  double jX, jY,jX0,jY0;
  double sina1, cosa1;
  char buf [100] ;

  jX = milimetryobx (X0);
  jY = milimetryoby (Y0);

    if (get_angle_l()!=0.0)
    {
        jX0=jX;
        jY0=jY;
        sina1=get_sina();
        cosa1=get_cosa();
        jX = (jX0 * cosa1) + (jY0 * sina1);
        jY = (-jX0 * sina1) + (jY0 * cosa1);
        jX=my_round(jX, 6);
        jY=my_round(jY, 6);
    }

  if (options1.uklad_geodezyjny==0) sprintf (buf, "X = %-12.6f Y = %-12.6f", jX, jY) ;
    else sprintf (buf, "X = %-12.6f Y = %-12.6f", jY, jX) ;
  Add_String_To_List (buf) ;
  komunikat_str (buf) ;
}

static void redcrP (char typ)
/*------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[3])(),KR, KR0, akt, sel_cur ;
  static TMENU *menu;
  static char st[POLE_TXT_MAX];
  static unsigned short cod = L' ';
  static int image_no ;
  static void (*RED_Add_M)(void)=nooop;
  static int iconno=0;

  if (typ == 0)
  {
     b__active_point = TRUE ;
     REDRAW_ADD_MENU () ;
     RED_Add_M= REDRAW_ADD_MENU; REDRAW_ADD_MENU = nooop ;
     getmenupini (&menu, st, &cod, &iconno) ;
     KR = Komunikat_R ;
     KR0 = Komunikat_R0 ;
     komunikat (0) ;
     image_no = Save_Message0_Image () ;
     komunikat0 (54) ;
     akt = sel.akt ; sel.akt = ASel ;
     sel_cur = sel.cur ; sel.cur = NCSel ;
     df__xprev = X ;
     df__yprev = Y ;
     Cur_offd (X, Y) ;
     CUR_oFF = CUR_OFF ;  CUR_OFF = cursel_off ;
     CUR_oN = CUR_ON ;   CUR_ON = cursel_on ;
     SW[0] = SERV[73] ;  SERV[73] = sel_t ;
     SW[1] = SERV[81] ;  SERV[81] = sel_d ;
     Cur_ond (X, Y) ;
   }
  else
  {
     Cur_offd (X, Y) ;
     CUR_OFF = CUR_oFF ;
     CUR_ON = CUR_oN ;
     SERV[73] = SW[0] ;
     SERV[81] = SW[1] ;
     sel.akt = akt ;
     sel.cur = sel_cur ;
     komunikat (KR) ;
     Komunikat_R0 = KR0 ;
     Put_Message0_Image (image_no) ;
     menupini (menu, st, cod, iconno) ;
     b__active_point = FALSE ;
     REDRAW_ADD_MENU = RED_Add_M ;
     REDRAW_ADD_MENU () ;
     Cur_ond (X, Y) ;
   }
}

static void redcrP_PCX(char typ)
/*------------------------*/
{
    static void (*CUR_oN)(double, double);
    static void (*CUR_oFF)(double, double);
    static int (*SW[3])(), KR, KR0, akt, sel_cur;
    static TMENU* menu;
    static char st[POLE_TXT_MAX];
    static unsigned short cod = L' ';
    static int image_no;
    static void (*RED_Add_M)(void)=nooop;
    static int iconno = 0;

    if (typ == 0)
    {
        Cur_offd(X, Y);
        b__active_point = TRUE;
        REDRAW_ADD_MENU();
        RED_Add_M = REDRAW_ADD_MENU; REDRAW_ADD_MENU = nooop;
        getmenupini(&menu, st, &cod, &iconno);
        KR = Komunikat_R;
        KR0 = Komunikat_R0;
        komunikat(0);
        image_no = Save_Message0_Image();
        komunikat0(54);
        akt = sel.akt; sel.akt = ASel;
        sel_cur = sel.cur; sel.cur = NCSel;
        df__xprev = X;
        df__yprev = Y;
        CUR_oFF = CUR_OFF;  CUR_OFF = cursel_off_pcx;
        CUR_oN = CUR_ON;   CUR_ON = cursel_on_pcx;
        SW[0] = SERV[73];  SERV[73] = sel_t;
        SW[1] = SERV[81];  SERV[81] = sel_d;
        Cur_ond(X, Y);
    }
    else
    {
        Cur_offd(X, Y);
        CUR_OFF = CUR_oFF;
        CUR_ON = CUR_oN;
        SERV[73] = SW[0];
        SERV[81] = SW[1];
        sel.akt = akt;
        sel.cur = sel_cur;
        komunikat(KR);
        Komunikat_R0 = KR0;
        Put_Message0_Image(image_no);
        menupini(menu, st, cod, iconno);
        b__active_point = FALSE;
        REDRAW_ADD_MENU = RED_Add_M;
        REDRAW_ADD_MENU();
        Cur_ond(X, Y);
    }
}

void Os_odniesienia1 (void)
/*---------------------*/
{

aa:
  redcrMP (0,1) ;
  if (FALSE == axis_point (&df_apx1, &df_apy1))
  {
    redcrMP (1,1) ;
    return ;
  }
  redcrMP (2,1) ;
  if (FALSE == axis_point (&df_apx2, &df_apy2))
  {
    goto aa ;
  }
  redcrMP (3,1) ;
}


static void out_local_angle (void)
/*------------------------------*/
{
  double dX, dY, dl, angle ;
  char buf [100] ;
  int l_kr;
  PLINIA pl ;
  int kkk;

  if (strwyj == 1)
  {
      angle=eL.values[0];
  }
  else {
      parametry_lini(&LiniaG, &pl);

      dX = milimetryob(LiniaG.x2 - LiniaG.x1);
      dY = milimetryob(LiniaG.y2 - LiniaG.y1);
      dl = milimetryob(pl.dl);
      angle = pl.kat;
  }
  sprintf (buf, "%s = %-8.7lg", measure_comm[2], angle) ;
  l_kr=put_angle_l(angle);
  Add_String_To_List (buf) ;
  strcat (buf, measure_comm[3]) ;
  komunikat_str (buf) ;
  kkk=Get_Key () ;
}

double cut_val1(double jXY)
{
 if (fabs(jXY)<0.0000001) return 0;
  else return jXY;
}

void Point_Origin (void)
/*---------------------*/
{
  EVENT *ev;
  double X0, Y0;
  int l_kr ;
  double xd, yd, xd1, yd1 ;
  char sk [MaxTextLen] = "" ;
  int retval_no = 2 ;
  double buf_ret [2] ;
  double jX,jY,jX0,jY0;
  double angle_l, sina, cosa;
  int ret;

  standard_func();
  if (b__active_point == TRUE)
  {
    ErrList (78) ;
    return ;
  }
  redcrP (0) ;
  X0 = X;
  Y0 = Y;
  while(1)
  {
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    switch (ev->What)
    {
       case evKeyDown :
	   if(ev->Number==0)
	   {
	     redcrP (1) ;
	     return ;
	   }
	   if(ev->Number == ENTER)
	   {
         if (options1.uklad_izometryczny)
         {
             cartesian_to_isometric(milimetryobx (X0), milimetryoby (Y0), &jX, &jY);
         }
         else
         {
             jX = milimetryobx (X0) ;
             jY = milimetryoby (Y0) ;

             angle_l = get_angle_l();
             if (angle_l != 0) {
                 jX0 = jX;
                 jY0 = jY;
                 sina = get_sina();
                 cosa = get_cosa();
                 jX = (jX0 * cosa) + (jY0 * sina);
                 jY = (-jX0 * sina) + (jY0 * cosa);
                 jX0 = jX;
                 jY0 = jY;

                 jX = my_round(jX0, 6);
                 jY = my_round(jY0, 6);
             }
         }

         if (options1.uklad_geodezyjny==0) sprintf (sk, "%#16.12lg;%#16.12lg", jX, jY) ;
           else sprintf (sk, "%#16.12lg;%#16.12lg", jY, jX) ;
         if (!get_string (sk, "", MaxTextLen, 0, 99))
          {
            redcrP (1) ;
            return ;
          }
         if (FALSE == calculator (sk, &retval_no, buf_ret) || retval_no < 2)
          {
           redcrP (1) ;
           return ;
          }


         if (options1.uklad_izometryczny)
         {
             xd = jednostkiOb(buf_ret[0]);
             yd = jednostkiOb(buf_ret[1]);
             ret = isometric_vector_to_cartesian(xd, yd, &xd1, &yd1);
         }
         else
         {
             if (options1.uklad_geodezyjny == 0)
             {
                 xd = jednostkiOb(buf_ret[0]);
                 yd = jednostkiOb(buf_ret[1]);
             } else
             {
                 xd = jednostkiOb(buf_ret[1]);
                 yd = jednostkiOb(buf_ret[0]);
             }

             if (angle_l != 0)
             {
                 sina = get_sina();
                 cosa = get_cosa();
                 xd1 = (xd * cosa) - (yd * sina);
                 yd1 = (xd * sina) + (yd * cosa);
             }
             else
             {
                 xd1 = xd;
                 yd1 = yd;
             }
         }

         xd = X0 - xd1;
         yd = Y0 - yd1;

	     l_kr=put_localx(xd) ;
	     l_kr=put_localy(yd) ;

         /*
           double xd_obj = jednostkiOb(buf_ret[0]);   // Cartesian object X
           double yd_obj = jednostkiOb(buf_ret[1]);   // Cartesian object Y

           double xd_drawing, yd_drawing;

// Convert object Cartesian → drawing units (mm)
           xd_drawing = milimetryobx_inv(xd_obj);     // you need inverse function!
           yd_drawing = milimetryoby_inv(yd_obj);     // inverse of milimetryoby

// Now apply rotation if needed (geodetic Cartesian)
           if (!options1.uklad_izometryczny && options1.uklad_geodezyjny != 0 && angle_l != 0) {
               double sina = get_sina();
               double cosa = get_cosa();
               double temp = xd_drawing;
               xd_drawing = temp * cosa - yd_drawing * sina;
               yd_drawing = temp * sina + yd_drawing * cosa;
           }

// For isometric – NO additional transformation here!
           double xd1 = xd_drawing;
           double yd1 = yd_drawing;

// Compute offset in drawing units
           double offset_x = X0 - xd1;
           double offset_y = Y0 - yd1;

// Set local origin
           l_kr=put_localx(offset_x);
           l_kr=put_localy(offset_y);
              */


	     pozycja_kursora();
	     delay(500);
	     redcrP (1) ;
	     return;
	   }
	   break;
    }
  }
}

void Point_Measure (void)
/*---------------------*/
{
  EVENT *ev;
  double X0, Y0;

  standard_func();
  if (b__active_point == TRUE)
  {
    ErrList (78) ;
    return ;
  }
  SERV[66] = Change_Orto ;
  redcrP (0) ;
  while(1)
  {
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    switch (ev->What)
    {
       case evKeyDown :
	   if(ev->Number==0)
	   {
	     redcrP (1) ;
	     SERV[66] = (int (*)(void)) noop;
	     return ;
	   }
	   if(ev->Number == ENTER)
	   {
	     out_coord (X0, Y0) ;
	   }
	   break;
       default :
	   break ;
    }
  }
  SERV[66] = (int (*)(void)) noop;
}

int Get_Point_PCX(double* coord_x, double* coord_y, B_PCX *b_pcx , int opcja)
/*------------------------------------------------------------------------*/
{
    EVENT* ev;
    double X0, Y0;
    int ret;

    GlobalPCX = b_pcx;

    redcrP_PCX(0);
    while (1)
    {
        ev = Get_Event_Point(NULL, &X0, &Y0);

        switch (ev->What)
        {
        case evKeyDown:
            if (ev->Number == 0)
            {
                redcrP_PCX(1);
                return 0;
            }
            if (ev->Number == ENTER)
            {
                redcrP_PCX(1);
                
                *coord_x = X0;
                *coord_y = Y0;
                return 1;
            }
            break;
        default:
            break;
        }
    }
    return 0;
}


int Get_Point (double *coord_x, double *coord_y, int opcja)
/*-------------------------------------------------------*/
{
  EVENT *ev;
  double X0, Y0;

  redcrP (0) ;
  while(1)
  {
    ev = Get_Event_Point (NULL, &X0, &Y0) ;

    switch (ev->What)
    {
       case evKeyDown :
	   if(ev->Number==0)
	   {
	     redcrP (1) ;
	     return 0;
	   }
	   if(ev->Number == ENTER)
	   {
        redcrP (1);
        *coord_x=X0;
        *coord_y=Y0;
        return 1;
	   }
	   break;
       default :
	   break ;
    }
  }
  return 0;
}


int Point_Measure_(void)
{
  Point_Measure();
  return 0;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*------------     ODLEGLOSC     ---------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/


static void out_dist (void)
/*-----------------------*/
{
  double dX, dY, dl, angle ;
  char buf [100] ;
  PLINIA pl ;

  parametry_lini(&line_g, &pl) ;

  dX = milimetryob (line_g.x2 - line_g.x1) ;
  dY = milimetryob (line_g.y2 - line_g.y1) ;
  dl = milimetryob (pl.dl) ;
  angle = pl.kat ;
  if (mode_dist == ADD_DIST)
  {
    df__total_dist += dl ;
  }
  else
  if (mode_dist == SUB_DIST)
  {
    df__total_dist -= dl ;
  }
  if (mode_dist == POINT_DIST)
  {
    sprintf (buf, "L=%-13.9lgDX=%-13.9lgDY=%-13.9lg%s=%-8.7lg",  dl, dX, dY, measure_comm[4], angle) ;
  }
  else
  {
    sprintf (buf, "SumL=%-13.9lgL=%-12.9lgDX=%-13.9lgDY=%-13.9lg%s=%-8.7lg", df__total_dist, dl, dX, dY, measure_comm[4], angle) ;
  }
  Add_String_To_List (buf) ;
  komunikat_str (buf) ;
}

static void  shift_cur_off(double x, double y)
/*--------------------------------------*/
{
    flip_shift_screen(&line_g);
}

static void  cur_off(double x, double y)
/*--------------------------------------*/
{
    flip_screen();
}

/*
static void  cur_off__ (double x, double y)
//---------------------------------------
{
  cursor_off (x, y) ;
  outlineor (&line_g, XOR_PUT, 0) ;
}
*/

static void  shift_cur_on(double x, double y)
/*--------------------------------------*/
{
    float s_dx, s_dy;

    line_g.x2 = x;
    line_g.y2 = y;

    s_dx = line_g.x2 - line_g.x1;
    s_dy = line_g.y2 - line_g.y1;

    //limiting the vector to the size of image

    if (s_dx >= Xp)  //vector is to the right, so image is panning to the left, Xp can be 0 or positive
                    //this case is only if s_dx is positive, so first point is on the right side of second point
    {
        line_g.x2 = line_g.x1 + Xp;
        x = line_g.x2;
    }
    else  //vector is to the left, so image is panning to the right, Xp is zero or negative 
    {
        if (s_dx < (X2d + Xp - Xmax))
        {
            s_dx = (X2d + Xp - Xmax);
            if (s_dx > 0.0) s_dx = 0.0;
            line_g.x2 = line_g.x1 + s_dx;
            x = line_g.x2;
        }
    }


    if (s_dy >= Yp) //vector is to up, so image is moving to the down, Yp can be 0 or positive
                   //this case is only if s_dy is positive, so first point is below the second point
    {
        line_g.y2 = line_g.y1 + Yp;
        y = line_g.y2;
    }
    else  //vector is to down, so image is panning to the up, Yp is zero or negative 
    {
        if (s_dy < (Y2d + Yp - Ymax))
        {
            s_dy = (Y2d + Yp - Ymax);
            if (s_dy > 0.0) s_dy = 0.0;
            line_g.y2 = line_g.y1 + s_dy;
            y = line_g.y2;
        }
    }


    line_g.kolor = Get_Current_Color();
    outlineor(&line_g, XOR_PUT, 0);

    cursel_hand_on(x, y);

    X = x;
    Y = y;
}

static void  cur_on (double x, double y)
/*--------------------------------------*/
{
  line_g.x2 = x ;
  line_g.y2 = y ;
  line_g.kolor = Get_Current_Color () ;
  outlineor (&line_g, XOR_PUT, 0) ;
  cursor_on(x, y);
}

static void  cur_offLA (double x, double y)
/*--------------------------------------*/
{
    flip_screen();
}

static void  cur_offLA__ (double x, double y)
/*--------------------------------------*/
{
  cursor_off (x, y) ;
  outlineor (&LiniaG, XOR_PUT, 0) ;
}

static void  cur_onLA (double x, double y)
/*--------------------------------------*/
{
    double l;
    double kos, koc, kat;

  LiniaG.x2 = x ;
  LiniaG.y2 = y ;
  LiniaG.kolor = Get_Current_Color () ;
  outlineor (&LiniaG, COPY_PUT, 0) ;

    l=sqrt((x-LiniaG.x1)*(x-LiniaG.x1)+(y-LiniaG.y1)*(y-LiniaG.y1));
    if(l) { koc=(x-LiniaG.x1)/l; kos=(y-LiniaG.y1)/l; kat=Atan2(y-LiniaG.y1,x-LiniaG.x1); }
    else  { koc=1; kos=0; kat=0; }
    if(kat<0) kat=2*PI+kat;
    kat*=(180/PI);
    //eL.values=&kat;

      sprintf (eL.st, "%#10.5lf", kat) ;
      Out_Edited_Draw_Param ((ESTR *)&eL, TRUE) ;
      cursor_on(x, y);
}

static void redcrPS1(char typ)
{
    static void (*CUR_oN)(double, double);
    static void (*CUR_oFF)(double, double);
    static int KR0;
    if (typ == 0)
    {
        KR0 = Komunikat_R0;
        komunikat0(144);
        Cur_offd(X, Y);
        CUR_oFF = CUR_OFF;  CUR_OFF = shift_cur_off;
        CUR_oN = CUR_ON;   CUR_ON = shift_cur_on;
        Orto_Dir = I_Orto_NoDir;
        Cur_ond(X, Y);
    }
    else
    {
        CUR_OFF = CUR_oFF;
        CUR_ON = CUR_oN;
        Komunikat_R0 = KR0;
        komunikat0(Komunikat_R0);
    }
}

static void redcrD1 (char typ)
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int KR0 ;
  if (typ == 0)
  {
     KR0 = Komunikat_R0 ;
     komunikat0 (56) ;
     Cur_offd (X, Y) ;
     CUR_oFF = CUR_OFF ;  CUR_OFF = cur_off ;
     CUR_oN = CUR_ON ;   CUR_ON = cur_on ;
     Orto_Dir = I_Orto_NoDir ;
     Cur_ond (X, Y) ;
   }
  else
   {
     Cur_offd (X, Y) ;
     CUR_OFF = CUR_oFF ;
     CUR_ON = CUR_oN ;
     Cur_ond (X, Y) ;
     Komunikat_R0 = KR0 ;
     komunikat0 (Komunikat_R0) ;
   }
}

static int A_p(BOOL b_graph_value)
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

static int el_o (BOOL b_graph_value)
/*--------------------------------*/
{
    double a;

    b_graph_value = b_graph_value ;
    if (eL.val_no < 1) return 0;
    a = eL.values [0] ;
    kos0 = sin (a * Pi / 180) ;
    koc0 = cos (a * Pi / 180) ;
    strwyj = 1 ;
    return 1 ;
}


static void redcrD3 (char typ)
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int KR0 ;
  static int np;

  if (typ == 0)
  {
     KR0 = Komunikat_R0 ;
     komunikat0 (103) ;
     CUR_OFF (X, Y) ;
     CUR_oFF = CUR_OFF ;  CUR_OFF = cur_offLA ;
     CUR_oN = CUR_ON ;   CUR_ON = cur_onLA ;
     Orto_Dir = I_Orto_NoDir ;
      eL.x=maxX-PL266;
      eL.y= ESTR_Y;
      eL.lmax=r22;
      eL.val_no_max	= 2 ;
      eL.mode	= GV_ANGLE	;
      eL.format = "%#10.5lg" ;
      eL.ESTRF=el_o;
      eL.extend = 0;
      np=dodajstr(&eL);
     CUR_ON (X, Y) ;
   }
  else
   {
     CUR_OFF (X, Y) ;
     usunstr(np);
     CUR_OFF = CUR_oFF ;
     CUR_ON = CUR_oN ;
     CUR_ON (X, Y) ;
     Komunikat_R0 = KR0 ;
     komunikat0 (Komunikat_R0) ;
   }
}

void  pan_shift_second_point(double X0, double Y0)
/*--------------------------------------------------------*/
{
    EVENT* ev;
    BOOL Auto_Pan_bak;


    Auto_Pan_bak = Auto_Pan;
    Auto_Pan = FALSE;
    line_g.x1 = X0;
    line_g.y1 = Y0;
    line_g.x2 = X;
    line_g.y2 = Y;
    redcrPS1(0);
    while (1)
    {
        do_not_draw_keys = TRUE;
        ev = Get_Event_Point(NULL, &X0, &Y0);
        do_not_draw_keys = FALSE;
        if (ev->What == evKeyDown && ev->Number == 0)
        {
            redcrPS1(1);
            CUR_OFF(X, Y);
            CUR_ON(X, Y);
            Auto_Pan = Auto_Pan_bak;
            return;
        }
        if (ev->What == evKeyDown && ev->Number == ENTER)
        {
            Xp_Yp_move(line_g.x1 - line_g.x2, line_g.y1 - line_g.y2);

            if (orto)
            {
                orto_l(&line_g, &Orto_Dir);
                Orto_Dir = I_Orto_NoDir;
            }
            else
            {
                line_g.x2 = X0;
                line_g.y2 = Y0;
            }
            line_g.x1 = X0;
            line_g.y1 = Y0;

            redcrPS1(1);

            del_demo_keys();

            do_not_draw_cur = TRUE;
            do_not_draw_keys = TRUE;
            redraw();
            do_not_draw_cur = FALSE;
            do_not_draw_keys = FALSE;

            draw_keyimages();
            
            draw_demokeyimages();

            CUR_OFF(X, Y);
            CUR_ON(X, Y);

            Auto_Pan = Auto_Pan_bak;
            return;
        }
    }
    Auto_Pan = Auto_Pan_bak;
}

void  dist_second_point (double X0, double Y0)
/*--------------------------------------------------------*/
{
  EVENT *ev;

  line_g.x1 = X0 ;
  line_g.y1 = Y0 ;
  line_g.x2 = X ;
  line_g.y2 = Y ;
  redcrD1 (0);
  while(1)
  {
     ev=Get_Event_Point (NULL, &X0, &Y0) ;
     if(ev->What == evKeyDown &&  ev->Number==0)
     {
	redcrD1 (1);
	return ;
     }
     if (ev->What == evKeyDown &&  ev->Number== ENTER)
     {
	CUR_OFF (X, Y) ;
	if (orto)
	{
	  orto_l(&line_g, &Orto_Dir);
	  Orto_Dir = I_Orto_NoDir ;
	}
	else
	{
	  line_g.x2 = X0 ;
	  line_g.y2 = Y0 ;
	}
	out_dist ();
	line_g.x1 = X0 ;
	line_g.y1 = Y0 ;
	CUR_ON (X, Y) ;
     }
   }
}

void  axis_second_point (double X0, double Y0)
/*------------------------------------------*/
{
  EVENT *ev;

  LiniaG.x1 = X0 ;
  LiniaG.y1 = Y0 ;
  LiniaG.x2 = X ;
  LiniaG.y2 = Y ;
  redcrD3 (0);
  while(1)
  {
      strwyj = 0 ;
     ev=Get_Event_Point (NULL, &X0, &Y0) ;
     if (ev->What == evKeyDown &&  ev->Number==0)
     {
	redcrD3 (1);
	return ;
     }
     if ((ev->What == evKeyDown &&  ev->Number== ENTER) || (strwyj==1))
     {
	CUR_OFF (X, Y) ;
	LiniaG.x2 = X0 ;
	LiniaG.y2 = Y0 ;
	out_local_angle();
	CUR_ON (X, Y) ;
	ev->Number=0 ;
	return ;
     }
   }
}

int  vector_second_point (double X0, double Y0)
/*--------------------------------------------*/
{
  EVENT *ev;

  LiniaG.x1 = X0 ;
  LiniaG.y1 = Y0 ;
  LiniaG.x2 = X ;
  LiniaG.y2 = Y ;
  redcrD3 (0);
  while(1)
  {
     ev=Get_Event_Point (NULL, &X0, &Y0) ;
     if(ev->What == evKeyDown &&  ev->Number==0)
     {
	redcrD3 (1);
	return 0;
     }
     if (ev->What == evKeyDown &&  ev->Number== ENTER)
     {
	CUR_OFF (X, Y) ;
	LiniaG.x2 = X0 ;
	LiniaG.y2 = Y0 ;
	CUR_ON (X, Y) ;
	ev->Number=0 ;
	return 1;
     }
   }
}


static void redcrPS(char typ)
{
    static void (*CUR_oN)(double, double);
    static void (*CUR_oFF)(double, double);
    static int (*SW[4]) (), KR, KR0, akt, sel_cur;
    static int image_no;
    static void (*RED_Add_M)(void);

    if (typ == 0)
    {
        Cur_offd(X, Y);
        b__active_dist = TRUE;
        REDRAW_ADD_MENU();
        RED_Add_M = REDRAW_ADD_MENU; REDRAW_ADD_MENU = nooop;
        KR = Komunikat_R;
        KR0 = Komunikat_R0;
        komunikat(0);
        image_no = Save_Message0_Image();
        komunikat0(143);
        Break_Sum_Measure();
        akt = sel.akt; sel.akt = ASel;
        sel_cur = sel.cur; sel.cur = NCSel;
        df__xprev = X;
        df__yprev = Y;
        CUR_oFF = CUR_OFF;     CUR_OFF = cursel_off;
        CUR_oN = CUR_ON;       CUR_ON = cursel_hand_on;
        SW[0] = SERV[73];    SERV[73] = sel_t;
        SW[1] = SERV[81];    SERV[81] = sel_d;
        SW[2] = SERV[67]; SERV[67] = Sum_Measure;
        SW[3] = SERV[68]; SERV[68] = Break_Sum_Measure;
        Cur_ond(X, Y);
    }
    else
    {
        if (typ == 1) Cur_offd(X, Y);
        CUR_OFF = CUR_oFF;
        CUR_ON = CUR_oN;
        SERV[73] = SW[0];
        SERV[81] = SW[1];
        SERV[67] = SW[2];
        SERV[68] = SW[3];
        sel.akt = akt;
        sel.cur = sel_cur;
        Break_Sum_Measure();
        komunikat(KR);
        Komunikat_R0 = KR0;
        Put_Message0_Image(image_no);
        b__active_dist = FALSE;
        REDRAW_ADD_MENU = RED_Add_M;
        REDRAW_ADD_MENU();
        Cur_ond(X, Y);
    }
}

static void redcrD (char typ)
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[4]) (), KR, KR0, akt, sel_cur ;
  static int image_no ;
  static void (*RED_Add_M)(void);

  if (typ == 0)
  {
	 Cur_offd(X, Y);
     b__active_dist = TRUE ;
     REDRAW_ADD_MENU () ;
     RED_Add_M= REDRAW_ADD_MENU; REDRAW_ADD_MENU = nooop ;
     KR = Komunikat_R ;
     KR0 = Komunikat_R0 ;
     komunikat (0) ;
     image_no = Save_Message0_Image () ;
     komunikat0 (55) ;
     Break_Sum_Measure () ;
     akt = sel.akt ; sel.akt = ASel ;
     sel_cur = sel.cur ; sel.cur = NCSel ;
     df__xprev = X ;
     df__yprev = Y ;
     CUR_oFF = CUR_OFF ;     CUR_OFF = cursel_off ;
     CUR_oN = CUR_ON ;       CUR_ON = cursel_on ;
     SW [0] = SERV [73] ;    SERV [73] = sel_t ;
     SW [1] = SERV [81] ;    SERV [81] = sel_d ;
     SW [2] = SERV [67] ; SERV [67] =  Sum_Measure ;
     SW [3] = SERV [68] ; SERV [68] = Break_Sum_Measure ;
     Cur_ond (X, Y) ;
  }
  else
  {
     if (typ == 1) Cur_offd (X, Y) ;
     CUR_OFF = CUR_oFF ;
     CUR_ON = CUR_oN ;
     SERV [73] = SW [0] ;
     SERV [81] = SW [1] ;
     SERV [67] = SW [2] ;
     SERV [68] = SW [3] ;
     sel.akt = akt ;
     sel.cur = sel_cur ;
     Break_Sum_Measure () ;
     komunikat (KR) ;
     Komunikat_R0 = KR0 ;
     Put_Message0_Image (image_no) ;
     b__active_dist = FALSE ;
     REDRAW_ADD_MENU = RED_Add_M ;
     REDRAW_ADD_MENU () ;
     Cur_ond (X, Y) ;
   }
}

static void redcrD2 (char typ)
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[4]) (), KR, KR0, sel_akt, sel_cur ;
  static int image_no ;
  static void (*RED_Add_M)(void);

  if (typ == 0)
  {
	 CUR_OFF(X, Y);
     REDRAW_ADD_MENU () ;
     RED_Add_M= REDRAW_ADD_MENU; REDRAW_ADD_MENU = nooop ;
     KR = Komunikat_R ;
     KR0 = Komunikat_R0 ;
     image_no = Save_Message0_Image () ;
     komunikat0 (102) ;
     sel_akt = sel.akt ; sel.akt = ASel ;
     sel_cur = sel.cur ; sel.cur = NCSel ;
     df__xprev = X ;
     df__yprev = Y ;
     CUR_oFF = CUR_OFF ;     CUR_OFF = cursel_off ;
     CUR_oN = CUR_ON ;       CUR_ON = cursel_on ;
     CUR_ON (X, Y) ;
  }
  else
  {
     CUR_OFF (X, Y) ;
     CUR_OFF = CUR_oFF ;
     CUR_ON = CUR_oN ;
     sel.akt = sel_akt ;
     sel.cur = sel_cur ;
     Komunikat_R0 = KR0 ;
     Put_Message0_Image (image_no) ;
     REDRAW_ADD_MENU = RED_Add_M ;
     REDRAW_ADD_MENU () ;
     CUR_ON (X, Y) ;
   }
}

void Local_Angle_Num(void)
/* --------------------*/
{
  char sk[MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d;
  double angle_l;
  char buf [100];
  int l_kr;

  angle_l=get_angle_l();

  DF_to_String (sk, "%-7.3f", angle_l, 0) ;
  if (!get_string (sk, "", MaxTextLen, 0, 63)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  angle_l = buf_ret [0] ;
  if (angle_l>0)
  {
   while (angle_l>=360) angle_l-=360;
  }
  else if (angle_l<0)
   {
     while (angle_l<=-360) angle_l+=360;
   }
  l_kr=put_angle_l(angle_l);
  komunikat (0);
  redraw ();
}


void Local_Angle (void)
/*---------------------*/
{
  EVENT *ev;
  double X0, Y0;
  double angle_l;
  char buf [100];
  int orto_temp;

  angle_l=get_angle_l();
  sprintf (buf, "%s = %-10.7lg", measure_comm[5],angle_l) ;
  komunikat_str (buf) ;

  orto_temp = orto ;
  orto = 0;
  CUR_ON(X, Y);
  redcrD2 (0);
  while(1)
  {
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    switch (ev->What)
    {
	case evKeyDown :
	   if(ev->Number == 0)
	   {
	     redcrD2 (1) ;
	     orto=orto_temp;
	     komunikat (0);
	     return ;
	   }
	   if(ev->Number == ENTER)
	   {
	     axis_second_point (X0, Y0) ;
	     redcrD2 (1) ;
	     orto=orto_temp;
	     komunikat (0);
	     redraw ();
	     return ;
	   }
	   break ;
	default :
	   break ;
    }
  }
  orto=orto_temp;
}

int  Get_Vector (int i)   //parametry przekazywane sa przez LiniaG
/*-------------------*/
{
  EVENT *ev;
  double X0, Y0;
  char buf [100];
  int orto_temp;
  LINIA linia__g;
  int ret;

  linia__g.kolor=LiniaG.kolor;
  linia__g.typ=LiniaG.typ;
  linia__g.warstwa=LiniaG.warstwa;
  LiniaG.kolor=15;
  LiniaG.warstwa=Current_Layer;
  LiniaG.typ=64;
  if (i==0) sprintf (buf, "%s", measure_comm[6]) ;
   else sprintf (buf, "%s", measure_comm[7]) ;
  komunikat_str (buf) ;

  orto_temp = orto ;
  orto = 0;
  SERV[66] = nooop1;

  redcrD2 (0);
  while(1)
  {
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    switch (ev->What)
    {
	case evKeyDown :
	   if(ev->Number == 0)
	   {
	     redcrD2 (1) ;
		 SERV[66] = Change_Orto;
	     orto=orto_temp;
		 
	     komunikat (0);
        LiniaG.kolor=linia__g.kolor;
        LiniaG.typ=linia__g.typ;
        LiniaG.warstwa=linia__g.warstwa;
	     return 0;
	   }
	   if(ev->Number == ENTER)
	   {
	     ret=vector_second_point (X0, Y0) ;
	     redcrD2 (1) ;
		 SERV[66] = Change_Orto;
	     orto=orto_temp;
	     komunikat (0);
        LiniaG.kolor=linia__g.kolor;
        LiniaG.typ=linia__g.typ;
        LiniaG.warstwa=linia__g.warstwa;
	     return ret;
	   }
	   break ;
	default :
	   break ;
    }
  }
  SERV[66] = Change_Orto;
  orto=orto_temp;
  LiniaG.kolor=linia__g.kolor;
  LiniaG.typ=linia__g.typ;
  LiniaG.warstwa=linia__g.warstwa;
  return 0;
}


void Dist_Measure (void)
/*---------------------*/
{
  EVENT *ev;
  double X0, Y0;

  standard_func();
  if (b__active_dist == TRUE)
  {
    ErrList (78) ;
    return ;
  }
  SERV[66] = Change_Orto ;
  redcrD (0);
  while(1)
  {
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    switch (ev->What)
    {
	case evKeyDown :
	   if(ev->Number == 0)
	   {
	     redcrD (1) ;
	     SERV[66] = noop ;
	     return ;
	   }
	   if(ev->Number == ENTER)
	   {
	     dist_second_point (X0, Y0) ;
	   }
	   break ;
	default :
	   break ;
    }
  }
  SERV[66] = noop ;
}

int Dist_Measure_(void)
{
  Dist_Measure();
  return 0;
}


int Pan_Shift()
/*---------------------*/
{
    EVENT* ev;
    double X0, Y0;
    int orto_bak;

    standard_func();

    if (b__active_dist == TRUE)
    {
        ErrList(78);
        return 0;
    }
    SERV[66] = noop;
    orto_bak = orto;
    orto = 0;
    redcrPS(0);
    while (1)
    {
        ev = Get_Event_Point(NULL, &X0, &Y0);
        draw_keyimage_(-1, TRUE, TRUE, FALSE);
        if (get_DEMO_RECORDING()) draw_demokeyimage_(-1, -1, TRUE, TRUE, FALSE);

        switch (ev->What)
        {
        case evKeyDown:
            if (ev->Number == 0)
            {
                redcrPS(1);
                SERV[66] = Change_Orto;
                orto = orto_bak;
                layer_info();
                //draw_keyimage_(-1, TRUE, FALSE, FALSE);
                return 0;
            }
            if (ev->Number == ENTER)
            {
                pan_shift_second_point(X0, Y0);
            }
            break;
        default:
            break;
        }
    }
    SERV[66] = Change_Orto;
    orto = orto_bak;
    layer_info();
    return 0;
}

int Pan_Shift_(void)
{
    Pan_Shift();
    return 0;
}



/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*-----------    ANGLE  ------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

static void out_angle (void) ;

static void redraw_angle (void)
/*----------------------------*/
{

    line_ge.kolor = Get_Current_Color () ;
    outline (&line_ge, XOR_PUT, 0) ;

}

static void redraw_angle1 (void)
/*----------------------------*/
{

    line_ge.kolor = Get_Current_Color () ;
    outline (&line_ge1, XOR_PUT, 0) ;

}

static BOOL previous_point_proc (void)
/*--------------------------------*/
{
  BOOL b_ret ;

  b_ret = TRUE ;
  switch (angle_state)
  {
    case ANGLE0 :
	b_ret = FALSE ;
	break ;
    case ANGLE1 :
	komunikat0 (85) ;
	CUR_OFF (X, Y) ;
	angle_state = ANGLE0 ;
	CUR_ON (X, Y) ;
	break ;
    case ANGLE2 :
	komunikat0 (86) ;
	CUR_OFF (X, Y) ;
	redraw_angle () ;
	angle_state = ANGLE1 ;
	CUR_ON (X, Y) ;
	break ;
  }
  return b_ret ;
}


static void next_point_proc (void)
/*--------------------------------*/
{
  switch (angle_state)
  {
    case ANGLE0 :
	komunikat0 (86) ;
	CUR_OFF (X, Y) ;
	line_ge.x1 = line_ge.x2 = X ;
	line_ge.y1 = line_ge.y2 = Y ;
	angle_state = ANGLE1 ;
	CUR_ON (X, Y) ;
	break ;
    case ANGLE1 :
	komunikat0 (84) ;
	CUR_OFF (X, Y) ;
	line_ge1.x1 = line_ge1.x2 = X ;
	line_ge1.y1 = line_ge1.y2 = Y ;
	angle_state = ANGLE2 ;
	redraw_angle () ;
	CUR_ON (X, Y) ;
	break ;
    case ANGLE2 :
	if (out_angle_ok1 != 1)
	{
	 angle_state = ANGLE0 ;
	 out_angle () ;
	 }
	 else angle_state = 3 ;
	angle_ok = 1 ;
	komunikat0 (85) ;
	redraw_angle () ;
	redraw_angle1 () ;

	CUR_OFF (X, Y) ;

	CUR_ON (X, Y) ;
	break ;
  }
}


static void  cur_off_angle(double x, double y)
/*----------------------------------------------*/
{
    flip_screen();
}

static void  cur_off_angle__ (double x, double y)
/*----------------------------------------------*/
{
  cursor_off (x, y) ;
  if (angle_state == ANGLE1)
  {
    outline (&line_ge, XOR_PUT, 0) ;
  }
  else
  if (angle_state == ANGLE2)
  {
    outline (&line_ge1, XOR_PUT, 0) ;
  }
}

static void  cur_on_angle (double x, double y)
/*----------------------------------------------*/
{
 if (angle_state == ANGLE0)
  {
    cursor_on (x, y) ;
  }
  else
  if (angle_state == ANGLE1)
  {
    line_ge.x2 = x ;
    line_ge.y2 = y ;
    line_ge.kolor = Get_Current_Color () ;
    outline (&line_ge, XOR_PUT, 0) ;
	cursor_on(x, y);
  }
  else
  if (angle_state == ANGLE2)
  {
    line_ge1.x2 = x ;
    line_ge1.y2 = y ;

    line_ge1.kolor = Get_Current_Color () ;
    outline (&line_ge1, XOR_PUT, 0) ;
	cursor_on(x, y);
  }
}


static double set_angle (void)
/*---------------------------*/
{
  double angle, df_xtemp, df_ytemp ;

  angle = Atan2 (line_ge.y1 - line_ge.y2, line_ge.x1 - line_ge.x2) ;
  obru (sin (angle), cos (angle),
	line_ge1.x2 - line_ge1.x1, line_ge1.y2 - line_ge1.y1,
	&df_xtemp, &df_ytemp) ;
  angle = Atan2 (df_ytemp, df_xtemp) ;
  if (angle < 0)
  {
    angle = Pi2 + angle ;
  }
  return angle * 180 / Pi ;
}

static void out_angle (void)
/*-----------------------*/
{
  double angle ;
  char buf [100] ;

  angle = set_angle () ;
  sprintf (buf, "%s=%-8.7lg", measure_comm[8], angle) ;
  Add_String_To_List (buf) ;
  komunikat_str (buf) ;
}


static void redcrA (char typ)
/*--------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[2]) (), akt, sel_cur, KR, KR0 ;
  static void (*RED_Add_M)(void);
  static TMENU *menu;
  static char st[POLE_TXT_MAX];
  static unsigned short cod = L' ';
  static int image_no ;
  static int iconno=0;

  if (typ == 0)
  {
	 Cur_offd(X, Y);
     b__active_angle = TRUE ;
     getmenupini (&menu, st, &cod, &iconno) ;
     KR = Komunikat_R ;
     KR0 = Komunikat_R0 ;
     komunikat (0) ;
     image_no = Save_Message0_Image () ;
     komunikat0 (85) ;
     angle_state = ANGLE0 ;
     RED_Add_M= REDRAW_ADD_MENU ; REDRAW_ADD_MENU = redraw_angle ;
     akt = sel.akt ; sel.akt = ASel ;
     sel_cur = sel.cur ; sel.cur = NCSel ;
     df__xprev = X ;
     df__yprev = Y ;
     CUR_oFF = CUR_OFF ;     CUR_OFF = cur_off_angle ;
     CUR_oN = CUR_ON ;       CUR_ON = cur_on_angle ;
     SW [0] = SERV [73] ;    SERV [73] = sel_t ;
     SW [1] = SERV [81] ;    SERV [81] = sel_d ;
     Cur_ond (X, Y) ;
  }
  else
  {
     Cur_offd (X, Y) ;
     CUR_OFF = CUR_oFF ;
     CUR_ON = CUR_oN ;
     SERV [73] = SW [0] ;
     SERV [81] = SW [1] ;
     REDRAW_ADD_MENU = RED_Add_M ;
     sel.akt = akt ;
     sel.cur = sel_cur ;
     menupini (menu, st, cod, iconno) ;
     komunikat (KR) ;
     Komunikat_R0 = KR0 ;
     b__active_angle = FALSE ;
     Put_Message0_Image (image_no) ;
     Cur_ond (X, Y) ;
   }
}


  void Angle_Measure1 (void)
/*---------------------------*/
{  LINIA line_geee ;
   LINIA line_geee1 ;
   int out_kat ;


  standard_func();
 out_kat==0;
 Angle_Measure(&line_geee, &line_geee1, &out_kat, 0) ;
}


void Angle_Measure (LINIA *line_gee, LINIA *line_gee1, int *out_kat, int out_angle_ok)
{
  EVENT *ev;
  double X0, Y0;

  *out_kat = 0;
  angle_ok = 0;
  out_angle_ok1 = out_angle_ok;
  if (b__active_angle == TRUE)
  {
    ErrList (78) ;
    return ;
  }
  SERV[66] = Change_Orto ;
  redcrA (0);
  while (1)
  {
    if (angle_state != 3)
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    if ((ev->What == evKeyDown && ev->Number == 0)||(angle_state == 3))
    {
     if ((FALSE == previous_point_proc ())||(angle_state == 3))
     {
   	angle_state = ANGLE0;
	   redcrA (1) ;
	   *line_gee = line_ge;
	   *line_gee1 = line_ge1;
	   if (angle_ok == 1) *out_kat = 1;
	     else *out_kat = 0;
	   SERV[66] = noop ;
	   return ;
     }
      else
       {
        if (angle_state>0) outlineor (&line_ge, XOR_PUT, 0) ;
        if (angle_state>1) outlineor (&line_ge1, XOR_PUT, 0) ;
        angle_state = ANGLE0;
	     redcrA (1) ;
  	     *line_gee = line_ge;
	     *line_gee1 = line_ge1;
	     if (angle_ok == 1) *out_kat = 1;
	       else *out_kat = 0;
	     SERV[66] = noop ;
	     return ;
     }

    }
    else
    if(ev->What == evKeyDown && ev->Number == ENTER)
    {
      next_point_proc () ;
    }
  }
  
  SERV[66] = Change_Orto ;

}

int Angle_Measure1_(void)
{
  Angle_Measure1();
  return 0;
}

/*--------------------------------------------*/
/*--------------------------------------------*/
/*------------Get_Graph_Values----------------*/
/*--------------------------------------------*/
/*--------------------------------------------*/


static BOOL get_graph_values_point (ESTR *ptrs_et)
/*-----------------------------------------------*/
{
  EVENT *ev;
  double X0, Y0;

  if (b__active_point == TRUE)
  {
    ErrList (78) ;
    return FALSE ;
  }
  redcrP (0) ;
  while(1)
  {
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    switch (ev->What)
    {
       case evKeyDown :
	   if(ev->Number==0)
	   {
	     redcrP (1) ;
	     return FALSE ;
	   }
	   if(ev->Number == ENTER)
	   {
	     ptrs_et->val_no = 2 ;
	     ptrs_et->values [0] = X0 ;
	     ptrs_et->values [1] = Y0 ;
	     redcrP (1) ;
	     return TRUE ;
	   }
	   break;
       default :
	   break ;
    }
  }
}

/*---------------------------------------------*/


static BOOL get_graph_values_dist_2_point (double X0, double Y0)
/*-----------------------------------------------------------*/
{
  EVENT *ev;

  line_g.x1 = X0 ;
  line_g.y1 = Y0 ;
  line_g.x2 = X ;
  line_g.y2 = Y ;
  redcrD1 (0) ;
  while(1)
  {
     ev=Get_Event_Point (NULL, &X0, &Y0) ;
     if(ev->What == evKeyDown &&  ev->Number==0)
     {
	redcrD1 (1) ;
	return FALSE ;
     }
     if (ev->What == evKeyDown &&  ev->Number== ENTER)
     {
	CUR_OFF (X, Y) ;
	line_g.x2 = X0 ;
	line_g.y2 = Y0 ;
	CUR_ON (X, Y) ;
	redcrD1 (1) ;
	return TRUE ;
     }
   }
}


static BOOL get_graph_values_dist (ESTR *ptrs_et, BOOL b_dist)
/*-----------------------------------------------------------*/
{
  EVENT *ev;
  double X0, Y0;
  PLINIA pl ;

  if (b__active_dist == TRUE)
  {
    ErrList (78) ;
    return FALSE ;
  }
  redcrD (0);
  while(1)
  {
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    switch (ev->What)
    {
	case evKeyDown :
	   if(ev->Number == 0)
	   {
	     redcrD (1) ;
	     return FALSE ;
	   }
	   if(ev->Number == ENTER)
	   {
	     if (TRUE == get_graph_values_dist_2_point (X0, Y0))
	     {
	       parametry_lini(&line_g, &pl) ;
	       ptrs_et->val_no = 1 ;
	       ptrs_et->values [0] = milimetryob (pl.dl) ;
	       if (FALSE == b_dist)
	       {
		 ptrs_et->val_no = 2 ;
		 ptrs_et->values [1] = pl.kat ;
	       }
	       redcrD (1) ;
	       return TRUE ;
	     }
	   }
	   break ;
	default :
	   break ;
    }
  }
}


/*---------------------------------------------------*/

static BOOL get_graph_values_angle (ESTR *ptrs_et)
/*------------------------------------------------*/
{
  EVENT *ev;
  double X0, Y0;

  if (b__active_angle == TRUE)
  {
    ErrList (78) ;
    return FALSE ;
  }
  redcrA (0);
  while (1)
  {
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    if (ev->What == evKeyDown && ev->Number == 0)
    {
      if (FALSE == previous_point_proc ())
      {
	redcrA (1) ;
	return FALSE ;
      }
    }
    else
    if(ev->What == evKeyDown && ev->Number == ENTER)
    {
      if (angle_state != ANGLE2)
      {
	next_point_proc () ;
      }
      else
      {
	redraw_angle () ;
	ptrs_et->val_no = 1 ;
	ptrs_et->values [0] = set_angle () ;
	redcrA (1) ;
	return TRUE ;
      }
    }
  }
}

/*---------------------------------------------------*/


BOOL Get_Graph_Values (ESTR *ptrs_et)
/*-----------------------------------*/
{
  BOOL b_ret ;

  b_ret = FALSE ;
  switch (ptrs_et->mode)
  {
    case GV_POINT :
	 b_ret = get_graph_values_point (ptrs_et) ;
	 break ;
    case GV_DIST :
	 b_ret = get_graph_values_dist (ptrs_et, TRUE) ;
	 break ;
    case GV_VECTOR :
	 b_ret = get_graph_values_dist (ptrs_et, FALSE) ;
	 break ;
    case GV_ANGLE :
	 b_ret = get_graph_values_angle (ptrs_et) ;
	 break ;
    case GV_NUMBER :
	 break ;
    default :
	 break ;
  }
  return b_ret ;
}

void Get_Graph_Values_Cur_Pos (double *ptrdf_x, double *ptrdf_y)
/*--------------------------------------------------------------*/
{
  *ptrdf_x = df__xprev ;
  *ptrdf_y = df__yprev ;
}

void Calculator_Measure (void)
//----------------------------
{
  char sk[MaxTextLen]="" ;
  int retval_no ;
  double buf_ret [1] ;
  double d;

aa:
  if (!get_string (sk, "", MaxTextLen, 0, 56)) return ;
  Set_Float_Flag (FALSE) ;
  retval_no = 1 ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    strcpy (sk, "") ;
  }
  else
  {
    d = buf_ret [0] ;
    if (fabs (d) < 1000000.0)
    {
      DF_to_String (sk, "%-25f", d, 0) ;
    }
    else
    {
      DF_to_String (sk, "%-25.15e", d, 0) ;
    }
  }
  Set_Float_Flag (TRUE) ;
  goto aa ;
}

int Calculator_Measure_(void)
{
  Calculator_Measure();
  return 0;
}


#undef __O_MEASURE__