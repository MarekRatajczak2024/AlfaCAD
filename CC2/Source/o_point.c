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

#define __O_POINT__
#include <forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include <string.h>
#include <stdio.h>
#include "bib_e.h"
#include "rysuj_e.h"

#include "menu.h"

extern void setwritemode( int mode );
extern void view_point_type(T_Point *P);
extern void rysuj_bitmap_outline(char *ad, int mode, BOOL enforce, int kolor0, int kolor, int gr);
extern int Point_Selected (T_Point *) ;
extern void Draw_Point (T_Point *ptrs_point, int mode, int kolor);
extern void rysuj_punkt_ (T_Point *ad,int mode,int kolor);
extern void setcolor(int kolor);
extern void setfillstyle_(int pattern, int color);
extern void set_mode_solid(void);

enum POINT {POINT_SIZE = 1, POINT_VIEW_ON = 2, POINT_VIEW_OFF = 3} ;
enum POINT_TYP {POINT_NORMAL = 0, POINT_BASE_POINT, POINT_PIN, POINT_JUNCTION, POINT_FIXED, POINT_FIXED_L, POINT_FIXED_R, POINT_FIXED_U, POINT_PINNED, POINT_PINNED_L, POINT_PINNED_R, POINT_PINNED_U,
                POINT_FIXED_ROLLER, POINT_FIXED_ROLLER_L, POINT_FIXED_ROLLER_R, POINT_FIXED_ROLLER_U, POINT_PINNED_ROLLER, POINT_PINNED_ROLLER_L, POINT_PINNED_ROLLER_R, POINT_PINNED_ROLLER_U, POINT_NO_ROTATION,
                POINT_FIXED_ROLLER_X, POINT_FIXED_ROLLER_XU, POINT_FIXED_ROLLER_Y, POINT_FIXED_ROLLER_YU, POINT_PINNED_ROLLER_X, POINT_PINNED_ROLLER_XU, POINT_PINNED_ROLLER_Y, POINT_PINNED_ROLLER_YU,

        } ;

TMENU mTyp_punktu= {29, 0,0, 12,56,4,ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmTyp_punktu, NULL,NULL} ;


BOOL Add_Point (double df_x, double df_y)
/*---------------------------------------*/
{
  BOOL b_ret ;

  PointG.x = df_x ;
  PointG.y = df_y ;
  b_ret = FALSE ;
  if (NULL != dodaj_obiekt (NULL, (void*)&PointG))
  {
    b_ret = TRUE ;
    rysuj_obiekt ((char*)&PointG, COPY_PUT, 1) ;
  }
  return b_ret ;
}

void Redraw_Point (BOOL b_draw)
/*----------------------------*/
{
  T_Point *ptrs_point ;
  NAGLOWEK *nag ;
  long_long ad, off, offk ;

  Set_Second_Screen();
  okno_r_second () ;
  setwritemode (COPY_PUT) ;
  if (b_draw == FALSE)
  {
    off = 0 ;
    offk = dane_size - 1 ;
    ObiektTok (&off, offk, &ad, Opoint) ;
    while (ad != -1)
    {
      nag = (NAGLOWEK*)(dane + ad) ;
      if (nag->widoczny == 1)
      {
	rysuj_obiekt_ ((char *)nag, COPY_PUT, 0) ;
      }
      ObiektTok (&off, offk, &ad, Opoint);
    }
  }
  else
  {
    off = 0 ;
    offk = dane_size - 1 ;
    ObiektTok (&off, offk, &ad, Opoint) ;
    while (ad != -1)
    {
      ptrs_point = (T_Point*)(dane + ad) ;
      if ((1 == (ptrs_point->widoczny = Point_Selected(ptrs_point))))
      {
	rysuj_obiekt_ ((char *)ptrs_point, COPY_PUT, 1) ;
      }
      ObiektTok (&off, offk, &ad, Opoint);
    }
  }
  okno_all_second () ;
  Set_Screen();
}

void Redraw_Line_225(BOOL b_draw)
/*----------------------------*/
{
	LINIA *ptrs_line;
	NAGLOWEK *nag;
	long_long ad, off, offk;

    Set_Second_Screen();
    okno_r_second () ;
	setwritemode(COPY_PUT);
	if (b_draw == FALSE)
	{
		off = 0;
		offk = dane_size - 1;
		ObiektTok(&off, offk, &ad, Olinia);
		while (ad != -1)
		{
			nag = (NAGLOWEK*)(dane + ad);
			if ((((LINIA*)(nag))->typ== HATCH_OUTLINE_TYPE) && (nag->widoczny == 1))
			{
				rysuj_obiekt_((char *)nag, COPY_PUT, 0);
			}
			ObiektTok(&off, offk, &ad, Olinia);
		}
	}
	else
	{
		off = 0;
		offk = dane_size - 1;
		ObiektTok(&off, offk, &ad, Olinia);
		while (ad != -1)
		{
			ptrs_line = (LINIA*)(dane + ad);
			if ((1 == (ptrs_line->widoczny = linia_wybrana(ptrs_line))))
			{
				rysuj_obiekt_((char *)ptrs_line, COPY_PUT, 1);
			}
			ObiektTok(&off, offk, &ad, Olinia);
		}
	}
    okno_all_second () ;
    Set_Screen();
}

void Redraw_Arc_225(BOOL b_draw)
/*----------------------------*/
{
	LUK *ptrs_arc;
	NAGLOWEK *nag;
	long_long ad, off, offk;

    Set_Second_Screen();
    okno_r_second () ;
	setwritemode(COPY_PUT);
	if (b_draw == FALSE)
	{
		off = 0;
		offk = dane_size - 1;
		ObiektTok(&off, offk, &ad, Oluk);
		while (ad != -1)
		{
			nag = (NAGLOWEK*)(dane + ad);
			if ((((LUK*)(nag))->typ == HATCH_OUTLINE_TYPE) && (nag->widoczny == 1))
			{
				rysuj_obiekt_((char *)nag, COPY_PUT, 0);
			}
			ObiektTok(&off, offk, &ad, Oluk);
		}
	}
	else
	{
		off = 0;
		offk = dane_size - 1;
		ObiektTok(&off, offk, &ad, Oluk);
		while (ad != -1)
		{
			ptrs_arc = (LUK*)(dane + ad);
			if ((1 == (ptrs_arc->widoczny = luk_wybrany(ptrs_arc))))
			{
				rysuj_obiekt_((char *)ptrs_arc, COPY_PUT, 1);
			}
			ObiektTok(&off, offk, &ad, Oluk);
		}
	}
    okno_all_second () ;
    Set_Screen();
}

void Redraw_Circle_225(BOOL b_draw)
/*----------------------------*/
{
	OKRAG *ptrs_circle;
	NAGLOWEK *nag;
	long_long ad, off, offk;

    Set_Second_Screen();
    okno_r_second () ;
	setwritemode(COPY_PUT);
	if (b_draw == FALSE)
	{
		off = 0;
		offk = dane_size - 1;
		ObiektTok(&off, offk, &ad, Ookrag);
		while (ad != -1)
		{
			nag = (NAGLOWEK*)(dane + ad);
			if ((((OKRAG*)(nag))->typ == HATCH_OUTLINE_TYPE) && (nag->widoczny == 1))
			{
				rysuj_obiekt_((char *)nag, COPY_PUT, 0);
			}
			ObiektTok(&off, offk, &ad, Ookrag);
		}
	}
	else
	{
		off = 0;
		offk = dane_size - 1;
		ObiektTok(&off, offk, &ad, Ookrag);
		while (ad != -1)
		{
			ptrs_circle = (OKRAG*)(dane + ad);
			if ((1 == (ptrs_circle->widoczny = okrag_wybrany(ptrs_circle))))
			{
				rysuj_obiekt_((char *)ptrs_circle, COPY_PUT, 1);
			}
			ObiektTok(&off, offk, &ad, Ookrag);
		}
	}
    okno_all_second () ;
    Set_Screen();
}

void Redraw_Spline(BOOL b_draw)
/*----------------------------*/
{
	SPLINE *ptrs_spline;
	NAGLOWEK *nag;
	long_long ad, off, offk;

	Set_Second_Screen();
	okno_r_second();
	setwritemode(COPY_PUT);
	if (b_draw == FALSE)
	{
		off = 0;
		offk = dane_size - 1;
		ObiektTok(&off, offk, &ad, Ospline);
		while (ad != -1)
		{
			nag = (NAGLOWEK*)(dane + ad);
			if (nag->widoczny == 1)
			{
				rysuj_obiekt_((char *)nag, COPY_PUT, 0);
			}
			ObiektTok(&off, offk, &ad, Ospline);
		}
	}
	else
	{
		off = 0;
		offk = dane_size - 1;
		ObiektTok(&off, offk, &ad, Ospline);
		while (ad != -1)
		{
			ptrs_spline = (SPLINE*)(dane + ad);
			if (ptrs_spline->widoczny >= 0)
			{
				rysuj_obiekt_((char *)ptrs_spline, COPY_PUT, 1);
			}
			ObiektTok(&off, offk, &ad, Ospline);
		}
	}
	okno_all_second();
	Set_Screen();
}

void Redraw_Bitmap_Outline(int kolor0)
/*-----------------------------------*/
{
	B_PCX *ptrs_pcx;
	NAGLOWEK *nag;
	long_long ad, off, offk;
    int gr, kolor;

    Set_Second_Screen();
	okno_r_second();
	
	off = 0;
	offk = dane_size - 1;
	ObiektTok(&off, offk, &ad, Opcx);
	while (ad != -1)
	{
		nag = (NAGLOWEK*)(dane + ad);
		if (nag->widoczny == 1)
		{
            if (kolor0==0) {gr=5; kolor=kolory.paper;}
            else {
                if (nag->atrybut == Ablok) {
                    gr = 5;
                    kolor = kolory.blok;
                }
                else {
                    gr = 3;
                    kolor = kolory.cur;
                }
            }
			rysuj_bitmap_outline((char *)nag, COPY_PUT, FALSE, kolor0, kolor, gr);
		}
		ObiektTok(&off, offk, &ad, Opcx);
	}
	
	okno_all_second();
    Set_Screen();
}

static void  cur_off (double x,double y)
/*----------------------------------------*/
{
    flip_screen();
}

void outpointor (T_Point *P, int mode,int kolor)
{
    okno_r();
    setwritemode(mode);
    P->x=X;
    P->y=Y;
    if (P->kolor == 0)
    {
        setfillstyle_(SOLID_FILL, kolory.paper);
        setcolor(kolory.paper);
        set_mode_solid();
    }
    else
    {
        SetColorAC(P->kolor);
        setfillstyle_(SOLID_FILL, GetColorAC(P->kolor));
    }
    rysuj_punkt_ (P, mode, kolor);
    okno_all();
}

static void  cur_on(double x,double y)
/*----------------------------------------*/
{
    LiniaG.x2=x; LiniaG.y2=y;
    outpointor (&PointG, COPY_PUT, 1) ;
    cursel_on(x, y);
}

static void redcrP (char typ)
/*------------------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[3])(), akt ;

  if (typ == 0)
  {
	 CUR_OFF(X, Y);
     komunikat (0) ;
     komunikat0 (54) ;
     akt = sel.akt ; sel.akt = ASel ;
     CUR_oFF = CUR_OFF ;  CUR_OFF = cur_off;
     CUR_oN = CUR_ON ;   CUR_ON = cur_on;
     SW[0] = SERV[73] ;  SERV[73] = sel_t ;
     SW[1] = SERV[81] ;  SERV[81] = sel_d ;
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
     komunikat (0) ;
     komunikat0 (0) ;
   }
}


void set_point_normal (void)
 {
  PointG.typ=0;
  view_point_type(&PointG);
 }

void set_point_base_point (void)
 {
  PointG.typ=1;
  view_point_type(&PointG);
 }
 
void set_point_pin (void)
 {
  PointG.typ=8;
  view_point_type(&PointG);
 } 

void set_point_junction(void)
{
	PointG.typ = 7;
	view_point_type(&PointG);
}

void set_point_support(int number)
{
    PointG.typ = number - POINT_FIXED + 12;
}
 
void set_point_pin_g (void)
 {
  PointG.typ=9;
  view_point_type(&PointG);
 }
 
void set_point_pin_d (void)
 {
  PointG.typ=10;
  view_point_type(&PointG);
 }    
 
void set_point_pin_s (void)
 {
  PointG.typ=11;
  view_point_type(&PointG);
 } 

void Point (void)
/*--------------*/
{
  EVENT *ev;
  double X0, Y0;

  redcrP (0) ;
  while (1)
  {
    menupini (&mTyp_punktu, _POINT_TYPE_, _POINT_TYPE_C_, 27) ;
    view_point_type(&PointG);
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    menupini (NULL, "", ' ', 0) ;
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

	     Add_Point (X0, Y0) ;
         CUR_OFF (X,Y) ;
	     CUR_ON (X,Y) ;
	   }
	   break;
	case evCommandP:
	  if (ev->Number == POINT_NORMAL)
            {
              set_point_normal () ;
            }   
            else
            if (ev->Number == POINT_BASE_POINT)
            {
              set_point_base_point () ;
            }   
            else
            {
            if (ev->Number == POINT_PIN)
             {
               set_point_pin () ;
             }
			else if (ev->Number == POINT_JUNCTION)
			{
				set_point_junction();
			}
            else if ((ev->Number >= POINT_FIXED) && (ev->Number <= POINT_PINNED_ROLLER_YU))
            {
                set_point_support(ev->Number);
            }
            /*
            if (ev->Number == POINT_PIN_G)
             {
               set_point_pin_g () ;
             }      
            if (ev->Number == POINT_PIN_D)
             {
               set_point_pin_d () ;
             }   
            if (ev->Number == POINT_PIN_S)
             {
               set_point_pin_s () ;
             }
             */
            }
          break;  
       default :
	   break ;
    }
  }
}

#undef __O_POINT__