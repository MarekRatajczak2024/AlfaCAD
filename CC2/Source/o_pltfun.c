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

#include <forwin.h>
#include <math.h>
#include <string.h>
#ifndef LINUX
#include <io.h>
#endif
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdio.h>
#define ALLEGWIN
#include <allegext.h>
#include "bib_e.h"
#include "o_dialog.h"
#include "o_pltprn.h"
#include "alf_res.h"
#include "o_plt.h"
#include "o_lfile.h"
#include "o_image.h"
#include "rysuj_e.h"

extern BOOL draw_rectangle_to_plt ( double x1, double y1, double x2, double y2);
extern BOOL rectangle_relative ( double dx, double dy);
extern BOOL plt_pen_color_ink (int pen,  int cyjan, int magenta, int yellow);
extern BOOL Check_if_Equal (double x, double y);
extern BOOL rr_go ( long dx, long dy);
extern BOOL Draw_Pcx_To_Drive(B_PCX *adr_pcx,int t_drive);
extern void ObiektTok_No_Superblock (long_long *off, long_long offk, long_long *ad, int obiekt);
extern void komunikat_str_short(char *st, BOOL stay, BOOL center);

extern void set_trace_block_begin(char *adr);
extern void set_trace_block_end(char *adr);
extern void set_trace_block(BOOL tb);

/*
   SP;       returns a pen
   SP pen number;
   0 to 8   0 - returns a pen
   VS;            default VS 20;
   VS pen speed;
   2 to 25    	if ps>=25 ps = 20
		if ps < 2 ps = 2
*/

#define def_speed 20
#define def_width_ink 0.1
#define hardware_margin_plt 400  //10 mm * 40 krokow na milimetr


/*

			    LOCAL DECLARATIONS		     		    

*/

char *rr_command="RR";

static T_PTR_Plt_Config ptrs__config ;
static T_PTR_Plt_Ini_Date ptrs__ini_date ;


static BOOL pen_exist [MAXNPEN];
static double dist_min;		/*minimalna odleglosc miedzy punktami plotera
				w jednostkach rysunku */
static BOOL begin_draw;        /*poczatek plotowania, zmiana pisaka*/

#if defined (D_PLT)
static BOOL  DN = FALSE;
#endif


static BOOL draw_line_to_plt_typ1 ( double x1, double y1, double x2, double y2);
/* static BOOL draw_line_to_plt_typ2 ( double x1, double y1, double x2, double y2); */
static BOOL draw_sheet_plt1(int pen);
static BOOL draw_sheet_plt(int pen);
static BOOL draw_to ( double x, double y);
static BOOL draw_to_plt ( long x, long y);
static void find_pen (void);
static BOOL go_to ( double x, double y);
static BOOL go_to_plt ( long x, long y);
static int object_to_pen ( NAGLOWEK * nag );
static BOOL pen_up (void);
static BOOL pen_dn (void);
static BOOL plt_ini (void);
static BOOL plt_end (void);
static BOOL plt_pen_sel (int pen);
static BOOL plt_pen_speed (int speed);
BOOL plt_pen_width_ink (int pen, double width);
BOOL plt_mark_char(unsigned char zn);
static BOOL plt_pens_width_ink (void);
BOOL write_ter (void);


static long JednostkiPltX ( double jednostkiX, double jednostkiY, BOOL relative)   /* jednostki -> jednostki plt*/
/*----------------------------------------------------------------------------*/
{
 double move_FormatY ;

   long j_pltx;
   if ( ptrs__ini_date->rotation)
   {
       if (( ptrs__config->origin==1 ) || (relative==TRUE))
	move_FormatY=0;
	  else
	    move_FormatY=FormatY;

      j_pltx = floor ( 0.5 +  ptrs__config->xmin + ( ptrs__ini_date->dx /*+ ptrs__ini_date->yp*/ +
	       move_FormatY - (( milimetryplt (jednostkiY) - ptrs__config->dy0) * ptrs__ini_date->scale )) * ptrs__config->rozdz );

   }
   else
   {
       j_pltx = floor ( 0.5 +  ptrs__config->xmin + ( ptrs__ini_date->dx /*- ptrs__ini_date->xp*/ +
	      (	( milimetryplt (jednostkiX ) - ptrs__config->dx0) * ptrs__ini_date->scale )) * ptrs__config->rozdz  );
   }
   return j_pltx;
}

static long JednostkiPltY ( double jednostkiX, double jednostkiY)   /* jednostki -> jednostki plt*/
/*--------------------------------------------------------------*/
{

   long j_plty;
   if ( ptrs__ini_date->rotation)
   {
       j_plty = floor ( 0.5 +  ptrs__config->ymin + ( ptrs__ini_date->dy  /*- ptrs__ini_date->xp */ +
		( ( milimetryplt (jednostkiX) - ptrs__config->dx0) * ptrs__ini_date->scale ))  * ptrs__config->rozdz );
   }
   else
   {
       j_plty = floor ( 0.5 +  ptrs__config->ymin + ( ptrs__ini_date->dy /* - ptrs__ini_date->yp */ +
	      (( milimetryplt  (jednostkiY) - ptrs__config->dy0) * ptrs__ini_date->scale )) * ptrs__config->rozdz  );
   }
   return j_plty;
}


static double dist (double x1,double y1, double x2, double y2)
/*------------------------------------------------------------*/
{
    return sqrt ( (x2 - x1) * (x2 - x1)  +  (y2 - y1) * (y2 - y1) );
}

BOOL Draw_Rectangle_To_Plt ( double x1, double y1, double x2, double y2)
/*--------------------------------------------------------------------*/
{
  return draw_rectangle_to_plt ( x1, y1, x2, y2);
}

BOOL Draw_Line_To_Plt ( double x1, double y1, double x2, double y2)
/*----------------------------------------------------------------------*/
{

    if (ptrs__config->type == 1)
    {
	return draw_line_to_plt_typ1 ( x1, y1, x2, y2);
    }
    else
    {
	return draw_line_to_plt_typ1 ( x1, y1, x2, y2);
    }
}

BOOL draw_rectangle_to_plt ( double x1, double y1, double x2, double y2)
/*---------------------------------------------------------------------------*/
{ double dx, dy;
 if (go_to (x1, y2) == FALSE) return FALSE;
 if (write_ter () == FALSE) return FALSE;
 dx=x2-x1;
 dy=y1-y2;
 if (rectangle_relative(dx, dy) == FALSE) return FALSE;
 if (pen_up () == FALSE) return FALSE;
 return TRUE;
}


static BOOL draw_line_to_plt_typ1 ( double x1, double y1, double x2, double y2)
/*----------------------------------------------------------------------*/
{
    static double xlast, ylast, x, y;
    static double d1, d2;

    if (begin_draw == TRUE)
    {
	  if (ptrs__config->type==99)
	      if (pen_up() == FALSE) return FALSE;
      if (go_to (x1, y1) == FALSE) return FALSE;
      if (pen_dn () == FALSE) return FALSE;
      if (go_to (xlast = x2, ylast = y2) == FALSE) return FALSE;
      begin_draw = FALSE;
    }
    else
    {
	d1 = dist (x1, y1, xlast, ylast);
	if ( d1 < dist_min)
	{
	    if (draw_to (xlast = x2, ylast = y2) == FALSE) return FALSE;
	    return TRUE;
	}
	d2 = dist (x2, y2, xlast, ylast);
	if ( d2 < dist_min)
	{
	    if (draw_to (xlast = x1, ylast = y1) == FALSE) return FALSE;
	    return TRUE;
	}
	if ( d1 < d2 )
	{
	    x = x2;  y = y2;
	    x2 = x1; y2 = y1;
	    x1 = x;  y1 = y;
	}
	if (pen_up () == FALSE) return FALSE;
	if (go_to (x2, y2) == FALSE) return FALSE;
	if (pen_dn () == FALSE) return FALSE;
	if (go_to (xlast = x1, ylast = y1) == FALSE) return FALSE;
    }
    return TRUE;
}



static BOOL plt_ini (void)
/*-----------------------*/
{
  char *INI = ptrs__config->ini;
  char *TER = ptrs__config->ter_kom;
  char *PS = "PS";
  char *LM="                                             ";
  int page_size, page_width;
  char s_page_size[20], s_page_width[20];
  int ret;


  if (INI[0] == '\0') return TRUE;
  if (Write_To_Device (INI, strlen (INI)) != strlen (INI)) return FALSE;
  if (ptrs__config->type==99) return TRUE;
  if (Write_To_Device (TER, strlen (TER)) != strlen (TER)) return FALSE;
  //ustalenie dlugosci strony
  if (ptrs__ini_date->page_size==1)
   {
     page_size = fabs((JednostkiPltX(ptrs__ini_date->xk, ptrs__ini_date->yk, FALSE)
                 - JednostkiPltX(ptrs__ini_date->xp, ptrs__ini_date->yp, FALSE)))
                 + (ptrs__config->rozdz * ptrs__ini_date->dx) + hardware_margin_plt;

       ret = snprintf(s_page_size, 10, "%d",page_size);
     page_width = fabs((JednostkiPltY(ptrs__ini_date->xk, ptrs__ini_date->yk)
                 - JednostkiPltY(ptrs__ini_date->xp, ptrs__ini_date->yp)))
                 + (ptrs__config->rozdz * ptrs__ini_date->dy) + hardware_margin_plt;

       ret = snprintf(s_page_width, 10, "%d",page_width);
     if (Write_To_Device (PS, strlen (PS)) != strlen (PS)) return FALSE;
     if (Write_To_Device (s_page_size, strlen (s_page_size)) != strlen (s_page_size)) return FALSE;
     if (Write_To_Device (",",1) != 1) return FALSE;
     if (Write_To_Device (s_page_width, strlen (s_page_width)) != strlen (s_page_width)) return FALSE;
     if (Write_To_Device (TER, strlen (TER)) != strlen (TER)) return FALSE;
   }
  return TRUE;
}

static BOOL plt_end (void)
/*-----------------------*/
{
  char *END = ptrs__config->end;

  if (pen_up () == FALSE) return FALSE;
  if (Write_To_Device (END, strlen (END)) != strlen (END)) return FALSE;
  if (END[0] == '\n') return TRUE;
  if (write_ter () == FALSE) return FALSE;
  return TRUE;
}


static BOOL plt_pen_sel (int pen0)
/*------------------------------*/
{                               /*SP%;*/
				/* SP0; oddaje pisak*/
				/*SPn; pobiera pisak o numerze n=1,..,8*/
  char sz_pen [20];
  char *SP = ptrs__config->pen_sel;
  char *TER = ptrs__config->ter_kom;
  int k, pen;
  int ret;

  if ((ptrs__ini_date->ink_plotter==1) && (ptrs__ini_date->color_print==1))
         pen=1; else pen=pen0;

  if ( begin_draw == FALSE)  /*pisak podnosimy tylko przy jego zmianie*/
  {                          /*po inicjacji pisak jest podniesiony*/
      if (pen_up () == FALSE) return FALSE;
  }
  begin_draw = TRUE;

    ret = snprintf(sz_pen, 10, "%d",pen);

  if (SP [0] == '\0') return TRUE; /*nie ma to wiekszego sensu */
  if (Write_To_Device (SP, strlen (SP)) != strlen (SP)) return FALSE;
  if (Write_To_Device (sz_pen, strlen (sz_pen)) != strlen (sz_pen)) return FALSE;
  if (Write_To_Device (TER, strlen (TER)) != strlen (TER)) return FALSE;

  akt_pen_width=0;  //zerowanie grubosci pisaka w celu pozniejszego ustawienia

#if defined (D_PLT)
 SetColorAC(pen);      /*pisaki numerujemy od 1*/
#endif

  //ustalanie koloru pisaka
  if (ptrs__ini_date->ink_plotter==1)
   {
   if (ptrs__ini_date->color_print==0)
     k=plt_pen_color_ink (pen, 0, 0, 0);
    else
     {
      if (ptrs__ini_date->color_255==1)
        k=plt_pen_color_ink (1, ptrs__ini_date->tab_pen_color[pen0-1].c255,ptrs__ini_date->tab_pen_color[pen0-1].m255, ptrs__ini_date->tab_pen_color[pen0-1].y255);
      else
        k=plt_pen_color_ink (1, ptrs__ini_date->tab_pen_color[pen0-1].c,ptrs__ini_date->tab_pen_color[pen0-1].m, ptrs__ini_date->tab_pen_color[pen0-1].y);
     }
   }
  return TRUE;
}


static BOOL plt_pen_speed (int speed)
/*----------------------------------*/
{
  char sz_speed [20];
  char *VS = ptrs__config->pen_speed;
  char *TER = ptrs__config->ter_kom;
  int ret;


  if (ptrs__ini_date->ink_plotter==1) return TRUE;

  if (speed == 0)
  {
    speed = def_speed;
  }
  if (VS [0] == '\0') return TRUE;

    ret = snprintf(sz_speed, 10, "%d",speed);
  if (Write_To_Device (VS, strlen (VS)) != strlen (VS)) return FALSE;
  if (Write_To_Device (sz_speed, strlen (sz_speed)) != strlen (sz_speed)) return FALSE;
  if (Write_To_Device (TER, strlen (TER)) != strlen (TER)) return FALSE;
  return TRUE;
}


BOOL plt_mark_char(unsigned char zn)
/*--------------------------------*/
{
	char zndef[16];
	sprintf(zndef, "\n\n[%04X] %c", zn, zn);
	if (Write_To_Device(zndef, strlen(zndef)) != strlen(zndef)) return FALSE;
	return TRUE;
}

BOOL plt_pen_width_ink (int pen, double width)
/*-------------------------------------------------*/
{
  char sz_width_ink [20];
  char sz_pen [5];
  char *PW = ptrs__config->pen_width_ink;
  char *TER = ptrs__config->ter_kom;

  if (width == 0)
  {
    width = def_width_ink;
  }

  width *= ptrs__ini_date->scale;

  if (Check_if_Equal(akt_pen_width, width)==TRUE) return TRUE;
  if (PW [0] == '\0') return TRUE;
  sprintf (sz_width_ink,"%-4.2lf",width);
  if (Write_To_Device (PW, strlen (PW)) != strlen (PW)) return FALSE;
  if (Write_To_Device (sz_width_ink, strlen (sz_width_ink)) != strlen (sz_width_ink)) return FALSE;
  if (Write_To_Device (",",1) != 1) return FALSE;
  sprintf (sz_pen,"%d",pen);
  if (Write_To_Device (sz_pen, strlen (sz_pen)) != strlen (sz_pen)) return FALSE;
  if (Write_To_Device (TER, strlen (TER)) != strlen (TER)) return FALSE;
  akt_pen_width=width;
  return TRUE;
}


BOOL plt_pen_color_ink (int pen,  int cyjan, int magenta, int yellow)
/*-----------------------------------------------------------------*/
{
  char sz_color_pen [32];
  char *PC = "PC";
  char *TER = ptrs__config->ter_kom;

  if (Write_To_Device (PC, strlen (PC)) != strlen (PC)) return FALSE;
  sprintf (sz_color_pen,"%d,%d,%d,%d",pen, cyjan, magenta, yellow);
  if (Write_To_Device (sz_color_pen, strlen (sz_color_pen)) != strlen (sz_color_pen)) return FALSE;
  if (Write_To_Device (TER, strlen (TER)) != strlen (TER)) return FALSE;
  return TRUE;
}

BOOL plt_number_of_pen (int number_of_pen)
/*--------------------------------------*/
{
  char sz_number_of_pen [5];
  char *NP = "NP";
  char *TER = ptrs__config->ter_kom;

  if (Write_To_Device (NP, strlen (NP)) != strlen (NP)) return FALSE;
  sprintf (sz_number_of_pen,"%d",number_of_pen);
  if (Write_To_Device (sz_number_of_pen, strlen (sz_number_of_pen)) != strlen (sz_number_of_pen)) return FALSE;
  if (Write_To_Device (TER, strlen (TER)) != strlen (TER)) return FALSE;
  return TRUE;
}

static BOOL plt_pens_color_ink (void)
{ int i;
  BOOL k;
  //liczba pisakow
  k=plt_number_of_pen (ptrs__ini_date->number_of_pen);
  if (k==FALSE) return FALSE;
  //kolory
  for (i=0; i<ptrs__ini_date->number_of_pen; i++)
  {
   if (ptrs__ini_date->color_print==0)
     k=plt_pen_color_ink (i+1, 0, 0, 0);
    else
     {
      if (ptrs__ini_date->color_255==1)
        k=plt_pen_color_ink (i+1, ptrs__ini_date->tab_pen_color[i].c255,ptrs__ini_date->tab_pen_color[i].m255, ptrs__ini_date->tab_pen_color[i].y255);
      else
        k=plt_pen_color_ink (i+1, ptrs__ini_date->tab_pen_color[i].c,ptrs__ini_date->tab_pen_color[i].m, ptrs__ini_date->tab_pen_color[i].y);
     }
   if (k==FALSE) return FALSE;
  }
  return TRUE;
}

static BOOL go_to ( double x, double y)
/*-----------------------------------*/
{					/*PA x,y*/
  long xl, yl;

  xl = JednostkiPltX (x, y, FALSE) ;
  yl = JednostkiPltY (x, y) ;
  return go_to_plt (xl, yl);
}

BOOL rectangle_relative ( double dx, double dy)
/*--------------------------------------------------*/
{					/*PA x,y*/
  long dxl, dyl;

  dxl = JednostkiPltX (dx, dy, TRUE) ;
  dyl = JednostkiPltY (dx, dy) ;
  return rr_go (dxl, dyl);
}


static BOOL go_to_plt ( long x, long y)
/*---------------------------------*/
{					/*PA x,y*/
   char sx [20], sy[20];
   char *PA , *PRZ ;

   int a;

   PA =  ptrs__config->pen_abs;
   PRZ = ptrs__config->ter_wsp;

   if (y<0)
       a=0;

  if (Write_To_Device (PA, strlen (PA))  != strlen (PA)) return FALSE;

  if (ptrs__config->type == 99)
  {
	  sprintf(sx, "%.4f", ((double)x - 80000.0) / 10000.0);
	  sprintf(sy, "%.4f", ((double)y - 80000.0) / 10000.0);
  }
  else
  {
	  sprintf(sx, "%#ld", x);
	  sprintf(sy, "%#ld", y);
  }
  if (Write_To_Device (sx, strlen(sx)) != strlen (sx)) return FALSE;
  if (Write_To_Device (PRZ, strlen (PRZ))  != strlen (PRZ)) return FALSE;
  if (Write_To_Device (sy, strlen(sy)) != strlen (sy)) return FALSE;

#if defined (D_PLT)
  if (DN==TRUE)
   lineto ((int)x, (int)(479-y));
  else moveto
   ((int)x,(int)(479-y));
#endif

  return TRUE;
}


BOOL rr_go ( long dx, long dy)
/*---------------------------------*/
{					/*PA x,y*/
   char sx [20], sy[20];
   char *PA , *PRZ ;
   char *RR , *TER;

     PRZ = ptrs__config->ter_wsp;
     RR =  rr_command;

  if (Write_To_Device (RR, strlen (RR))  != strlen (RR)) return FALSE;
  if (ptrs__config->type == 99)
  {
	  sprintf(sx, "%.4f", ((double)dx - 80000.0) / 10000.0);
	  sprintf(sy, "%.4f", ((double)dy - 80000.0) / 10000.0);
  }
  else
  {
	  sprintf(sx, "%#ld", dx);
	  sprintf(sy, "%#ld", dy);
  }
  if (Write_To_Device (sx, strlen(sx)) != strlen (sx)) return FALSE;
  if (Write_To_Device (PRZ, strlen (PRZ))  != strlen (PRZ)) return FALSE;
  if (Write_To_Device (sy, strlen(sy)) != strlen (sy)) return FALSE;

#if defined (D_PLT)
  if (DN==TRUE)
   lineto ((int)x, (int)(479-y));
  else moveto
   ((int)x,(int)(479-y));
#endif

  return TRUE;
}

static BOOL draw_to ( double x, double y)
/*-------------------------------------*/
{					/*,x,y*/
  long xl, yl;

  xl = JednostkiPltX (x, y, FALSE) ;
  yl = JednostkiPltY (x, y) ;
  return draw_to_plt (xl, yl);
}


static BOOL draw_to_plt(long x, long y)
/*------------------------------------*/
{					/*,x,y*/
	char sx[20], sy[20];
	char *TER = ptrs__config->ter_kom;
	char *PRZ = ptrs__config->ter_wsp;

	if (ptrs__config->type == 99)
	{
		sprintf(sx, "%.4f", ((double)x - 80000.0) /10000.0);
		sprintf(sy, "%.4f", ((double)y - 80000.0) /10000.0);
		if (Write_To_Device(TER, strlen(TER)) != strlen(TER)) return FALSE;
	}
	else
	{
		sprintf(sx, "%#ld", x);
		sprintf(sy, "%#ld", y);
		if (Write_To_Device(PRZ, strlen(PRZ)) != strlen(PRZ)) return FALSE;
	}
  if (Write_To_Device (sx, strlen(sx)) != strlen (sx)) return FALSE;
  if (Write_To_Device (PRZ, strlen (PRZ))  != strlen (PRZ)) return FALSE;
  if (Write_To_Device (sy, strlen(sy)) != strlen (sy)) return FALSE;

#if defined (D_PLT)
  if (DN==TRUE)
   lineto ((int)x, (int)(479-y));
  else moveto
   ((int)x,(int)(479-y));
#endif

  return TRUE;
}

static BOOL pen_up (void)
/*------------------------------------*/
{                                       /*;PU;*/
  char *TER = ptrs__config->ter_kom;
  char *PU  = ptrs__config->pen_up;

  if (PU[0] != '\n') 
      if (Write_To_Device (TER, strlen (TER)) != strlen (TER)) return FALSE;
  if (PU [0] == '\0') return TRUE;  /*dla typu 2*/
  if (Write_To_Device (PU, strlen (PU)) != strlen (PU)) return FALSE;
  if (PU[0] == '\n') return TRUE;
  if (Write_To_Device (TER, strlen (TER)) != strlen (TER)) return FALSE;

#if defined (D_PLT)
  DN = FALSE;
#endif

  return TRUE;
}

static BOOL pen_dn (void)
/*------------------------------------*/
{                                       /*;PD;*/
  char *TER = ptrs__config->ter_kom;
  char *PD  = ptrs__config->pen_dn;


  if (Write_To_Device (TER, strlen (TER)) != strlen (TER)) return FALSE;
  if (PD [0] == '\0') return TRUE;  /*dla typu 2*/
  if (Write_To_Device (PD, strlen (PD)) != strlen (PD)) return FALSE;
  if (Write_To_Device (TER, strlen (TER)) != strlen (TER)) return FALSE;

#if defined (D_PLT)
  DN = TRUE;
#endif

  return TRUE;
}

BOOL write_ter (void)
/*---------------------------*/
{                                       /*;*/
  char *TER = ptrs__config->ter_kom;

  if (Write_To_Device (TER, strlen (TER)) != strlen (TER)) return FALSE;
  return TRUE;
}


void show_plot_progress(int pen, double plot_progress)
{
	char progress[32];
	sprintf(progress, "[%3d] %3.0f %%", pen+1, plot_progress*100.0);
	komunikat_str_short(progress, FALSE, FALSE);
	return;
}

static BOOL draw_sheet_plt(int pen0)
/*-------------------------------*/
{
 NAGLOWEK *nag;
 LUK l = ldef;
 ELLIPSE e = Edef;
 ELLIPSE fe =FEdef;
 ELLIPTICALARC ae=eldef;
 AVECTOR V;
 long_long off, offk, ad;
 int pen;

 int k;
 BOOL b_err=TRUE;
 int grubosc;
 char *block_adr=NULL;
 BLOK *b1, *b_sheet;
 T_Desc_Ex_Block *ptrs_desc_bl1;
 double plot_progress;
 double next_plot_progress = 0.01;
 int counter=0;

 BLOK_SIEC=FALSE;

 if (ptrs__ini_date->ink_plotter==1)
   {
     pen=0;
   }
    else pen=pen0;

  if (strlen(zbior_danych_2)>0)
  {
   zwolnienie_pamieci();
   k = czytaj_zbior("RYSUJ.$2", b_err, TRUE, TRUE);
    if (k)
    {
      if (k == 1 && b_err == TRUE)
      {
	ErrList (101) ;
	return FALSE;
      }
      else
       {
       ErrList (102) ;
       return FALSE;
       }
    }
  }

 if (ptrs__ini_date->print_sheet == TRUE)
 {
   off=ptrs__ini_date->sheet_adr - dane;
   b_sheet=(BLOK *)ptrs__ini_date->sheet_adr;
   offk=off+sizeof(NAGLOWEK)+b_sheet->n-1;

  ObiektTok (&off, offk, &ad, ONieOkreslony);
  while (ad!=-1)
  {

    /////////////////////////////Progress
	  counter++;
	  if (counter > 100)
	  {
		  if (dane_size > 0)
		  {
			  plot_progress = ((double)ad / (double)dane_size) + 0.001;
			  if (plot_progress > 1.0) plot_progress = 1.0;
			  if (plot_progress >= next_plot_progress)
			  {
				  show_plot_progress(pen0, plot_progress);
				  next_plot_progress = plot_progress + 0.01;
			  }
		  }
		  counter = 0;
	  }

	/////////////////////////////


    nag = (NAGLOWEK*)(dane+ad);

    if (BLOK_SIEC==TRUE)
     {
      if (nag>block_adr) BLOK_SIEC=FALSE;
     }

    if (( object_to_pen ( nag ) == pen0) || ( object_to_pen ( nag ) == 256))
     {
       switch(nag->obiekt)
	 {
         case OdBLOK :
             b1=(BLOK *)nag;
             if ((b1->kod_obiektu == B_PLINE) && (b1->opis_obiektu[0]== PL_TRACE))
             {
                 set_trace_block_begin((char*)nag + sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu);
                 set_trace_block_end((char*)nag + sizeof(NAGLOWEK) + b1->n - 1);
                 set_trace_block(TRUE);
             }
             else set_trace_block(FALSE);
             break;
           case Olinia :
		  if (((LINIA*)nag)->typ == HATCH_OUTLINE_TYPE) break;
        //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
         if (ptrs__ini_date->ink_plotter==1)
          {
            grubosc = ( ((LINIA*)nag)->typ & 224 ) / 32;
            plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
          }

         //edges
         if (((LINIA*)nag)->obiektt2==4) {
             memmove(&V, ((LINIA*)nag), sizeof(LINIA));
             if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_ROLL;
             else V.style = V_EDGE_ROLL_INV;
             Draw_Vector_To_Drive(&V, NULL);
             break;
         }
         else if (((LINIA*)nag)->obiektt2==6) {
             memmove(&V, ((LINIA*)nag), sizeof(LINIA));
             if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_SIMPLE;
             else V.style = V_EDGE_SIMPLE_INV;
             Draw_Vector_To_Drive(&V, NULL);
             break;
         }
         else if (((LINIA*)nag)->obiektt2==7) {
             memmove(&V, ((LINIA*)nag), sizeof(LINIA));
             if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_FIXED;
             else V.style = V_EDGE_FIXED_INV;
             Draw_Vector_To_Drive(&V, NULL);
             break;
         }

	     if (Draw_Line_To_Drive ((LINIA*)nag) == FALSE) return FALSE;
	     break;
	   case Otekst :
	     if (Draw_Tekst_To_Drive ((TEXT*)nag, ptrs__ini_date->ink_plotter, pen + 1 , ptrs__config->type/*, ptrs__ini_date->tab_pen_width_ink[pen]*/) == FALSE) return FALSE;
	     break;
	   case Okolo :
         //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
         if (ptrs__ini_date->ink_plotter==1)
          {
            grubosc = ( ((OKRAG*)nag)->typ & 224 ) / 32;
            plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
          }
	      if (((OKRAG*)nag)->r>2)
	       {
	         if (Draw_Fill_Circle_To_Drive ((OKRAG*)nag) == FALSE) return FALSE;
	       }
	       else
	        {
	         /*generowanie okregow wspolsrodkowych */
	         l.warstwa = ((OKRAG*)nag)->warstwa;
	         l.kolor = ((OKRAG*)nag)->kolor;
   	         l.typ = 64;  //ciagla gruba
	         l.x = ((OKRAG*)nag)->x;
	         l.y = ((OKRAG*)nag)->y;
	         l.kat1 = 0;
	         l.kat2 = Pi2;
	         l.r = ((OKRAG*)nag)->r;
	         l.r -= ptrs__ini_date->pen_width;
	         while (l.r>0)
	         {
	          if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	          l.r -= ptrs__ini_date->pen_width;
	         }
	         l.r = ptrs__ini_date->pen_width / 2 ;
	         if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	        }
           break;
         case Ofilledellipse :
             //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
             if (ptrs__ini_date->ink_plotter==1)
             {
                 plt_pen_width_ink (pen + 1, ptrs__ini_date->pen_width);
             }
             if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
         break;
         case Ookrag :
        //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
         if (ptrs__ini_date->ink_plotter==1)
          {
            grubosc = ( ((OKRAG*)nag)->typ & 224 ) / 32;
            plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
          }
	     l.warstwa = ((OKRAG*)nag)->warstwa;
	     l.kolor = ((OKRAG*)nag)->kolor;
	     if (nag->obiekt == Okolo) l.typ = 64;
	       else l.typ = ((OKRAG*)nag)->typ;
	     l.x = ((OKRAG*)nag)->x;
	     l.y = ((OKRAG*)nag)->y;
	     l.r = ((OKRAG*)nag)->r;
	     l.kat1 = 0;
	     l.kat2 = Pi2;
	     if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	    break;
        case Oellipse :
             //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
             if (ptrs__ini_date->ink_plotter==1)
             {
                 grubosc = ( ((ELLIPSE*)nag)->typ & 224 ) / 32;
                 plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
             }
             if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
       break;
	   case Oluk :
		  if (((LUK*)nag)->typ == HATCH_OUTLINE_TYPE) break;
        //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
         if (ptrs__ini_date->ink_plotter==1)
          {
            grubosc = ( ((LUK*)nag)->typ & 224 ) / 32;
            plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
          }
	     if (Draw_Arc_To_Drive ((LUK*)nag) == FALSE) return FALSE;
	    break;
         case Oellipticalarc :
             //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
             if (ptrs__ini_date->ink_plotter==1)
             {
                 grubosc = ( ((ELLIPTICALARC *)nag)->typ & 224 ) / 32;
                 plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
             }
             if (Draw_EllipticalArc_To_Drive((ELLIPTICALARC*)nag) == FALSE) return FALSE;
             break;
	  case Owwielokat :
        //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
        if (((WIELOKAT*)nag)->empty_typ==0)
         {
          if (ptrs__ini_date->ink_plotter==1)
          {
            plt_pen_width_ink (pen + 1, ptrs__ini_date->pen_width);
          }
         }
          else
          {
           if (ptrs__ini_date->ink_plotter==1)
            {
              grubosc = (((WIELOKAT*)nag)->empty_typ-1);
              plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
            }
          }

	     if (Draw_Wielokat_To_Drive ((WIELOKAT*)nag, NULL) == FALSE) return FALSE;
	    break;
     case Osolidarc :
             //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
             if (((SOLIDARC*)nag)->empty_typ==0)
             {
                 if (ptrs__ini_date->ink_plotter==1)
                 {
                     plt_pen_width_ink (pen + 1, ptrs__ini_date->pen_width);
                 }
             }
             else
             {
                 if (ptrs__ini_date->ink_plotter==1)
                 {
                     grubosc = (((SOLIDARC*)nag)->empty_typ-1);
                     plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
                 }
             }
             if (Draw_Solidarc_To_Drive ((SOLIDARC*)nag, NULL) == FALSE) return FALSE;
             break;
	 case Ospline:
		 //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
		 if (ptrs__ini_date->ink_plotter == 1)
		 {
			 grubosc = (((SPLINE*)nag)->typ & 224) / 32;
			 plt_pen_width_ink(pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
		 }
		 if (Draw_Spline_To_Drive((SPLINE*)nag) == FALSE) return FALSE;
		 break;
     case Opcx:
     //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
       if (pen==0)
        {
         if (ptrs__ini_date->ink_plotter==1)
          {
            plt_pen_width_ink (pen + 1, 0.1 /*ptrs__ini_date->pen_width*/);
          }
         if (Draw_Pcx_To_Drive ((B_PCX *)nag,1) == FALSE) return FALSE;
        }
     break;
         case Ovector:
             //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
             if (ptrs__ini_date->ink_plotter == 1)
             {
                 switch (((AVECTOR*)nag)->style<10)
                 {
                     case 0:
                     case 1:
                     case 2:
                     case 3:
                     case 7:
                     case 8:
                     case 9:
                         grubosc = (((AVECTOR*)nag)->typ & 224) / 32;
                     break;
                     default:
                         grubosc = 1;
                         break;
                 }
                 plt_pen_width_ink(pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
             }
             if (Draw_Vector_To_Drive((AVECTOR*)nag, NULL) == FALSE) return FALSE;
             break;
         case Opoint:
             grubosc = 1;
             plt_pen_width_ink(pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
             break;
	  default :
	    break;
	 } /*switch*/
     }
    ObiektTok (&off, offk, &ad, ONieOkreslony);
  }

   Set_Draw_Window (ptrs__ini_date->xp_w, ptrs__ini_date->yp_w, ptrs__ini_date->xk_w, ptrs__ini_date->yk_w);
 }

 off=0;
 offk=dane_size ;

 ObiektTok_No_Superblock (&off, offk, &ad, ONieOkreslony);
 while (ad!=-1)
  { 
	 /////////////////////////////Progress
	 counter++;
	 if (counter > 100)
	 {
		 if (dane_size > 0)
		 {
			 plot_progress = ((double)ad / (double)dane_size) + 0.001;
			 if (plot_progress > 1.0) plot_progress = 1.0;
			 if (plot_progress >= next_plot_progress)
			 {
				 show_plot_progress(pen0, plot_progress);
				 next_plot_progress = plot_progress + 0.01;
			 }
		 }
		 counter = 0;
	 }

	 /////////////////////////////
	 
	 nag = (NAGLOWEK*)(dane+ad);


    if (( object_to_pen ( nag ) == pen0) || ( object_to_pen ( nag ) == 256))
     {
       switch(nag->obiekt)
	 {  case OdBLOK :
       b1=(BLOK *)nag;
       ptrs_desc_bl1	= (T_Desc_Ex_Block *)(&b1->opis_obiektu	[0]) ;

    	   if ((strncmp(ptrs_desc_bl1->sz_type,"*#",2)==0) || (strncmp(ptrs_desc_bl1->sz_type,"*^",2)==0) || //to jest jakas siec
             (strncmp(ptrs_desc_bl1->sz_type,"*~",2)==0) || (strncmp(ptrs_desc_bl1->sz_type,"@kol",4)==0)) //to jest opis lub kolizja
          {
           if (BLOK_SIEC==FALSE)
             {
              BLOK_SIEC=TRUE;
              block_adr=(char *)nag + sizeof(NAGLOWEK) + b1->n - 1;
             }
          }

         if ((b1->kod_obiektu == B_PLINE) && (b1->opis_obiektu[0]== PL_TRACE))
         {
             set_trace_block_begin((char*)nag + sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu);
             set_trace_block_end((char*)nag + sizeof(NAGLOWEK) + b1->n - 1);
             set_trace_block(TRUE);
         }
         else set_trace_block(FALSE);

      break;
	   case Olinia :
		  if (((LINIA*)nag)->typ == HATCH_OUTLINE_TYPE) break;
        //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
         if (ptrs__ini_date->ink_plotter==1)
          {
            grubosc = ( ((LINIA*)nag)->typ & 224 ) / 32;
            plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
          }

         //edges
         if (((LINIA*)nag)->obiektt2==4) {
             memmove(&V, ((LINIA*)nag), sizeof(LINIA));
             if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_ROLL;
             else V.style = V_EDGE_ROLL_INV;
             Draw_Vector_To_Drive(&V, NULL);
             break;
         }
         else if (((LINIA*)nag)->obiektt2==6) {
             memmove(&V, ((LINIA*)nag), sizeof(LINIA));
             if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_SIMPLE;
             else V.style = V_EDGE_SIMPLE_INV;
             Draw_Vector_To_Drive(&V, NULL);
             break;
         }
         else if (((LINIA*)nag)->obiektt2==7) {
             memmove(&V, ((LINIA*)nag), sizeof(LINIA));
             if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_FIXED;
             else V.style = V_EDGE_FIXED_INV;
             Draw_Vector_To_Drive(&V, NULL);
             break;
         }

	     if (Draw_Line_To_Drive ((LINIA*)nag) == FALSE) return FALSE;
	     break;
	   case Otekst :
        if (strlen(((TEXT*)nag)->text)>0)
          {
	        if (Draw_Tekst_To_Drive ((TEXT*)nag, ptrs__ini_date->ink_plotter, pen + 1 , ptrs__config->type/*, ptrs__ini_date->tab_pen_width_ink[pen]*/) == FALSE) return FALSE;
          }
	     break;
	   case Okolo :
         //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
         if (ptrs__ini_date->ink_plotter==1)
          {
            grubosc = ( ((OKRAG*)nag)->typ & 224 ) / 32;
            plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
          }
	      if (((OKRAG*)nag)->r>2)
	       {
	         if (Draw_Fill_Circle_To_Drive ((OKRAG*)nag) == FALSE) return FALSE;
	       }
	       else
	        {
	         /*generowanie okregow wspolsrodkowych */
	         l.warstwa = ((OKRAG*)nag)->warstwa;
	         l.kolor = ((OKRAG*)nag)->kolor;
   	         l.typ = 64;  //ciagla gruba
	         l.x = ((OKRAG*)nag)->x;
	         l.y = ((OKRAG*)nag)->y;
	         l.kat1 = 0;
	         l.kat2 = Pi2;
	         l.r = ((OKRAG*)nag)->r;
	         l.r -= ptrs__ini_date->pen_width;
	         while (l.r>0)
	         {
	          if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	          l.r -= ptrs__ini_date->pen_width;
	         }
	         l.r = ptrs__ini_date->pen_width / 2 ;
	         if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	        }
       break;
       case Ofilledellipse :
               //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
               if (ptrs__ini_date->ink_plotter==1)
               {
                   plt_pen_width_ink (pen + 1, ptrs__ini_date->pen_width);
               }
               if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
       break;
	  case Ookrag :
        //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
         if (ptrs__ini_date->ink_plotter==1)
          {
            grubosc = ( ((OKRAG*)nag)->typ & 224 ) / 32;
            plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
          }
	     l.warstwa = ((OKRAG*)nag)->warstwa;
	     l.kolor = ((OKRAG*)nag)->kolor;
	     if (nag->obiekt == Okolo) l.typ = 64;
	       else l.typ = ((OKRAG*)nag)->typ;
	     l.x = ((OKRAG*)nag)->x;
	     l.y = ((OKRAG*)nag)->y;
	     l.r = ((OKRAG*)nag)->r;
	     l.kat1 = 0;
	     l.kat2 = Pi2;
	     if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	    break;
       case Oellipse :
               //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
               if (ptrs__ini_date->ink_plotter==1)
               {
                   grubosc = ( ((ELLIPSE*)nag)->typ & 224 ) / 32;
                   plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
               }
               if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
       break;
	  case Oluk :
		  if (((LUK*)nag)->typ == HATCH_OUTLINE_TYPE) break;
        //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
         if (ptrs__ini_date->ink_plotter==1)
          {
            grubosc = ( ((LUK*)nag)->typ & 224 ) / 32;
            plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
          }
	     if (Draw_Arc_To_Drive ((LUK*)nag) == FALSE) return FALSE;
	    break;
       case Oellipticalarc :
               //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
               if (ptrs__ini_date->ink_plotter==1)
               {
                   grubosc = ( ((ELLIPTICALARC *)nag)->typ & 224 ) / 32;
                   plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
               }
               if (Draw_EllipticalArc_To_Drive((ELLIPTICALARC*)nag) == FALSE) return FALSE;
           break;
	  case Owwielokat :
        //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
        if (((WIELOKAT*)nag)->empty_typ==0)
         {
          if (ptrs__ini_date->ink_plotter==1)
          {
            plt_pen_width_ink (pen + 1, ptrs__ini_date->pen_width);
          }
         }
          else
          {
           if (ptrs__ini_date->ink_plotter==1)
            {
              grubosc = (((WIELOKAT*)nag)->empty_typ-1);
              plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
            }
          }

	     if (Draw_Wielokat_To_Drive ((WIELOKAT*)nag, NULL) == FALSE) return FALSE;
	    break;
       case Osolidarc :
               //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
               if (((SOLIDARC*)nag)->empty_typ==0)
               {
                   if (ptrs__ini_date->ink_plotter==1)
                   {
                       plt_pen_width_ink (pen + 1, ptrs__ini_date->pen_width);
                   }
               }
               else
               {
                   if (ptrs__ini_date->ink_plotter==1)
                   {
                       grubosc = (((SOLIDARC*)nag)->empty_typ-1);
                       plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
                   }
               }
               if (Draw_Solidarc_To_Drive ((SOLIDARC*)nag, NULL) == FALSE) return FALSE;
               break;
	 case Ospline:
		 //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
		 if (ptrs__ini_date->ink_plotter == 1)
		 {
			 grubosc = (((SPLINE*)nag)->typ & 224) / 32;
			 plt_pen_width_ink(pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
		 }
		 if (Draw_Spline_To_Drive((SPLINE*)nag) == FALSE) return FALSE;
		 break;
     case Opcx:
     //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
       if (pen==0)
        {
         if (ptrs__ini_date->ink_plotter==1)
          {
            plt_pen_width_ink (pen + 1, 0.1 /*ptrs__ini_date->pen_width*/);
          }
         if (Draw_Pcx_To_Drive ((B_PCX *)nag,1) == FALSE) return FALSE;
        }
     break;
       case Ovector:
           //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
           if (ptrs__ini_date->ink_plotter == 1)
           {
               switch (((AVECTOR*)nag)->style<10)
               {
                   case 0:
                   case 1:
                   case 2:
                   case 3:
                   case 7:
                   case 8:
                   case 9:
                       grubosc = (((AVECTOR*)nag)->typ & 224) / 32;
                       break;
                   case 16:
                       grubosc = 0;
                       break;
                   default:
                       grubosc = 1;
                       break;
               }
               plt_pen_width_ink(pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
           }
           if (Draw_Vector_To_Drive((AVECTOR*)nag, NULL) == FALSE) return FALSE;
           break;
         case Opoint:
             if (ptrs__ini_date->ink_plotter == 1) {
                 grubosc = 2;
                 plt_pen_width_ink(pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
             }
             if (Draw_Point_To_Drive((T_Point *)nag) == FALSE) return FALSE;
             break;
	  default :
	    break;
	 } /*switch*/
     }
    ObiektTok_No_Superblock (&off, offk, &ad, ONieOkreslony);
  }

 if (ptrs__ini_date->print_sheet == TRUE)
  {
   Set_Draw_Window (ptrs__ini_date->xp, ptrs__ini_date->yp, ptrs__ini_date->xk, ptrs__ini_date->yk);
  }

  return TRUE;
}

static BOOL draw_sheet_plt1(int pen0)
/*-------------------------------*/
{
 NAGLOWEK *nag;
 AVECTOR V;
 LUK l = ldef;
 long_long off, offk, ad;
 int k;
 BOOL b_err=TRUE;
 int grubosc;
 int pen;

 if (ptrs__ini_date->ink_plotter==1)
   {
     pen=0;
   }
    else pen=pen0;

 if (strlen(zbior_danych_2)==0) return TRUE;
 zwolnienie_pamieci();
 k = czytaj_zbior(zbior_danych_2, b_err, TRUE, FALSE);
  if (k)
   {
    if (k == 1 && b_err == TRUE)
    {
     return FALSE;
    }
   }

 off=0;
 offk=dane_size;

 ObiektTok (&off, offk, &ad, ONieOkreslony);
 while (ad!=-1)
  { nag = (NAGLOWEK*)(dane+ad);

    if (( object_to_pen ( nag ) == pen0) || ( object_to_pen ( nag ) == 256))
     {
       switch(nag->obiekt)
	 { case Olinia :
		  if (((LINIA*)nag)->typ == HATCH_OUTLINE_TYPE) break;
        //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
         if (ptrs__ini_date->ink_plotter==1)
          {
            grubosc = ( ((LINIA*)nag)->typ & 224 ) / 32;
            plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
          }

         //edges
         if (((LINIA*)nag)->obiektt2==4) {
             memmove(&V, ((LINIA*)nag), sizeof(LINIA));
             if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_ROLL;
             else V.style = V_EDGE_ROLL_INV;
             Draw_Vector_To_Drive(&V, NULL);
             break;
         }
         else if (((LINIA*)nag)->obiektt2==6) {
             memmove(&V, ((LINIA*)nag), sizeof(LINIA));
             if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_SIMPLE;
             else V.style = V_EDGE_SIMPLE_INV;
             Draw_Vector_To_Drive(&V, NULL);
             break;
         }
         else if (((LINIA*)nag)->obiektt2==7) {
             memmove(&V, ((LINIA*)nag), sizeof(LINIA));
             if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_FIXED;
             else V.style = V_EDGE_FIXED_INV;
             Draw_Vector_To_Drive(&V, NULL);
             break;
         }

	     if (Draw_Line_To_Drive ((LINIA*)nag) == FALSE) return FALSE;
	     break;
	   case Otekst :
	     if (Draw_Tekst_To_Drive ((TEXT*)nag, ptrs__ini_date->ink_plotter, pen + 1, ptrs__config->type/*, ptrs__ini_date->tab_pen_width_ink[pen]*/) == FALSE) return FALSE;
	     break;
	   case Okolo :
         //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
         if (ptrs__ini_date->ink_plotter==1)
          {
            grubosc = ( ((OKRAG*)nag)->typ & 224 ) / 32;
            plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
          }
	      //if (Draw_Fill_Circle_To_Drive ((OKRAG*)nag) == FALSE) return FALSE;
	      if (((OKRAG*)nag)->r>2)
	       {
	         if (Draw_Fill_Circle_To_Drive ((OKRAG*)nag) == FALSE) return FALSE;
	       }
	       else
	        {
	         /*generowanie okregow wspolsrodkowych */
	         l.warstwa = ((OKRAG*)nag)->warstwa;
	         l.kolor = ((OKRAG*)nag)->kolor;
   	         l.typ = 64;  //ciagla gruba
	         l.x = ((OKRAG*)nag)->x;
	         l.y = ((OKRAG*)nag)->y;
	         l.kat1 = 0;
	         l.kat2 = Pi2;
	         l.r = ((OKRAG*)nag)->r;
	         l.r -= ptrs__ini_date->pen_width;
	         while (l.r>0)
	         {
	          if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	          l.r -= ptrs__ini_date->pen_width;
	         }
	         l.r = ptrs__ini_date->pen_width / 2 ;
	         if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	        }
           break;
     case Ofilledellipse :
             //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
             if (ptrs__ini_date->ink_plotter==1)
             {
                 plt_pen_width_ink (pen + 1, ptrs__ini_date->pen_width);
             }
             if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
         break;
	  case Ookrag :
        //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
         if (ptrs__ini_date->ink_plotter==1)
          {
            grubosc = ( ((OKRAG*)nag)->typ & 224 ) / 32;
            plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
          }
	     l.warstwa = ((OKRAG*)nag)->warstwa;
	     l.kolor = ((OKRAG*)nag)->kolor;
	     if (nag->obiekt == Okolo) l.typ = 64;
	       else l.typ = ((OKRAG*)nag)->typ;
	     l.x = ((OKRAG*)nag)->x;
	     l.y = ((OKRAG*)nag)->y;
	     l.r = ((OKRAG*)nag)->r;
	     l.kat1 = 0;
	     l.kat2 = Pi2;
	     if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	    break;
     case Oellipse :
             //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
             if (ptrs__ini_date->ink_plotter==1)
             {
                 grubosc = ( ((ELLIPSE*)nag)->typ & 224 ) / 32;
                 plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
             }
             if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
     break;
	  case Oluk :
		  if (((LUK*)nag)->typ == HATCH_OUTLINE_TYPE) break;
        //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
         if (ptrs__ini_date->ink_plotter==1)
          {
            grubosc = ( ((LUK*)nag)->typ & 224 ) / 32;
            plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
          }
	     if (Draw_Arc_To_Drive ((LUK*)nag) == FALSE) return FALSE;
	    break;
     case Oellipticalarc :
             //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
             if (ptrs__ini_date->ink_plotter==1)
             {
                 grubosc = ( ((ELLIPTICALARC *)nag)->typ & 224 ) / 32;
                 plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
             }
             if (Draw_EllipticalArc_To_Drive((ELLIPTICALARC*)nag) == FALSE) return FALSE;
         break;
     case Owwielokat :
        //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
        if (((WIELOKAT*)nag)->empty_typ==0)
         {
          if (ptrs__ini_date->ink_plotter==1)
          {
            plt_pen_width_ink (pen + 1, ptrs__ini_date->pen_width);
          }
         }
          else
          {
           if (ptrs__ini_date->ink_plotter==1)
            {
              grubosc = (((WIELOKAT*)nag)->empty_typ-1);
              plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
            }
          }

	     if (Draw_Wielokat_To_Drive ((WIELOKAT*)nag, NULL) == FALSE) return FALSE;
	    break;
     case Osolidarc :
             //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
             if (((SOLIDARC*)nag)->empty_typ==0)
             {
                 if (ptrs__ini_date->ink_plotter==1)
                 {
                     plt_pen_width_ink (pen + 1, ptrs__ini_date->pen_width);
                 }
             }
             else
             {
                 if (ptrs__ini_date->ink_plotter==1)
                 {
                     grubosc = (((SOLIDARC*)nag)->empty_typ-1);
                     plt_pen_width_ink (pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
                 }
             }
             if (Draw_Solidarc_To_Drive ((SOLIDARC*)nag, NULL) == FALSE) return FALSE;
             break;
	 case Ospline:
		 //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
		 if (ptrs__ini_date->ink_plotter == 1)
		 {
			 grubosc = (((SPLINE*)nag)->typ & 224) / 32;
			 plt_pen_width_ink(pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
		 }
		 if (Draw_Spline_To_Drive((SPLINE*)nag) == FALSE) return FALSE;
		 break;
       case Opcx:
         if (pen0==0)
          {
           if (ptrs__ini_date->ink_plotter==1)
            {
              plt_pen_width_ink (pen + 1, 0.1 /*ptrs__ini_date->pen_width*/);
            }
           if (Draw_Pcx_To_Drive ((B_PCX *)nag,1) == FALSE) return FALSE;
          }
	    break;
         case Ovector:
             //dla plotera atramentoweg nalezy dodatkowo ustawic grubosc
             if (ptrs__ini_date->ink_plotter == 1)
             {
                 switch (((AVECTOR*)nag)->style<10)
                 {
                     case 0:
                     case 1:
                     case 2:
                     case 3:
                     case 7:
                     case 8:
                     case 9:
                         grubosc = (((AVECTOR*)nag)->typ & 224) / 32;
                         break;
                     case 16:
                         grubosc = 0;
                         break;
                     default:
                         grubosc = 1;
                         break;
                 }
                 plt_pen_width_ink(pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
             }
             if (Draw_Vector_To_Drive((AVECTOR*)nag, NULL) == FALSE) return FALSE;
             break;
         case Opoint:
             if (ptrs__ini_date->ink_plotter == 1) {
                 grubosc = 2;
                 plt_pen_width_ink(pen + 1, ptrs__ini_date->tab_line_width_ink[grubosc]);
             }
             if (Draw_Point_To_Drive((T_Point *)nag) == FALSE) return FALSE;
             break;
	  default :
	    break;
	 } /*switch*/
     }
    ObiektTok (&off, offk, &ad, ONieOkreslony);
  }
  return TRUE;
}


static int object_to_pen ( NAGLOWEK * nag )
/*-----------------------------------------*/
{
    LINIA * L;
    AVECTOR *V;
    TEXT * T;
    int pen_number , grubosc;
    double grubosc_t;

     L = (LINIA*)nag;
     pen_number = 0 ;
     switch (nag->obiekt)
     {
     case OdBLOK:
         return 256;
         break;
	 case Olinia:
	 case Oluk:
	 case Ookrag:
	 case Ospline:
     case Oellipse:
      if ((ptrs__ini_date->color_print==1) && (ptrs__ini_date->ink_plotter==1))
         pen_number=L->kolor;
        else
       {
  	     grubosc = ( L->typ & 224 ) / 32;
	     if ( grubosc == 0)			/*kolory sa numerowane od 1*/
	       pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_bc ;
	     else if (grubosc == 1)
	       pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_c ;
	     else if (grubosc == 2)
	       pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_g ;
	     else if (grubosc == 3)
	       pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_bg ;
	     else if (grubosc == 4)
	       pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_ng ;
       }
	   break;
         case Ovector:
             V = (AVECTOR*)nag;
             switch (V->style<10)
             {
                 case 0:
                 case 1:
                 case 2:
                 case 3:
                 case 7:
                 case 8:
                 case 9:
                     grubosc = ( V->typ & 224 ) / 32;
                     if ( grubosc == 0)			/*kolory sa numerowane od 1*/
                         pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_bc ;
                     else if (grubosc == 1)
                         pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_c ;
                     else if (grubosc == 2)
                         pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_g ;
                     else if (grubosc == 3)
                         pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_bg ;
                     else if (grubosc == 4)
                         pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_ng ;
                     break;
                 default:
                     pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_bc ;
                     break;
             }
             break;
         case Opoint:
             pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_g ;
             break;
	case Otekst:
      if ((ptrs__ini_date->color_print==1) && (ptrs__ini_date->ink_plotter==1))
         pen_number=L->kolor;
        else
        {
	      T = (TEXT*)nag;
	        grubosc_t = T->wysokosc  * ptrs__ini_date->scale;
	      if (T->bold ==0)
	      {
 	       if ( grubosc_t < 2)			/*kolory sa numerowane od 1*/
	        pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_bc ;
	       else if (grubosc_t <= 5)
	        pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_c ;
	       else if (grubosc_t <= 10)
	        pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_g ;
	       else if (grubosc_t <= 14)
	        pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_bg ;
	       else
	        pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_ng ;

	      }
	       else
	        {
              if ( grubosc_t < 1)			/*kolory sa numerowane od 1*/
  	          pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_bc ;
	         else if (grubosc_t < 2)
	          pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_c ;
	         else if (grubosc_t <= 5)
	          pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_g ;
	         else if (grubosc_t <= 10)
	          pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_bg ;
	         else
	          pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_ng ;
	        }
        }
	   break;
	case Owwielokat:
        if ((ptrs__ini_date->color_print==1) && (ptrs__ini_date->ink_plotter==1))
         pen_number=L->kolor;
        else
        {
  	      grubosc = ((WIELOKAT*)nag)->empty_typ - 1;
 	      if ( grubosc == 0)			/*kolory sa numerowane od 1*/
	       pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_bc ;
	      else if (grubosc == 1)
	       pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_c ;
	      else if (grubosc == 2)
	       pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_g ;
	      else if (grubosc == 3)
	       pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_bg ;
	      else if (grubosc == 4)
	       pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_ng ;
           else  pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] - 1] . line_g  ;
        }
	   break;
     case Okolo:
     case Ofilledellipse:
         if ((ptrs__ini_date->color_print==1) && (ptrs__ini_date->ink_plotter==1))
             pen_number=L->kolor;
         else
             pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_g ;
         break;
     case Osolidarc:
             //case Owwielokat3D:
             if ((ptrs__ini_date->color_print==1) && (ptrs__ini_date->ink_plotter==1))
                 pen_number=L->kolor;
             else
             {
                 grubosc = ((SOLIDARC*)nag)->empty_typ - 1;
                 if ( grubosc == 0)			/*kolory sa numerowane od 1*/
                     pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_bc ;
                 else if (grubosc == 1)
                     pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_c ;
                 else if (grubosc == 2)
                     pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_g ;
                 else if (grubosc == 3)
                     pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_bg ;
                 else if (grubosc == 4)
                     pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] -1] . line_ng ;
                 else  pen_number = ptrs__ini_date->tab_pen_number [ color256[L->kolor] - 1] . line_g  ;
             }
             break;
      case Opcx:
        return 256;
	  break;
	default:
	   break;
     }
     if ((ptrs__ini_date->color_print==1) && (ptrs__ini_date->ink_plotter==1))
      {
        if (pen_number==0) return 14;  else return (pen_number - 1);
      }
        else return (pen_number - 1);
}

static void find_pen (void)
/*--------------------------*/
{
  NAGLOWEK *nag;
  long_long off=0, offk=dane_size, ad;
  int i;
  int pen_index;

  for ( i=0; i < MAXNPEN; i++)
   {
     pen_exist [i] = FALSE;
   }

  ObiektTok (&off, offk, &ad, ONieOkreslony);
  while (ad!=-1)
   { nag = (NAGLOWEK*)(dane+ad);
     pen_index = object_to_pen ( nag );
     if (pen_index != -1)
     {
      if (pen_index == 256)
         {
           for ( i=0; i < MAXNPEN; i++)
             {
               pen_exist [i] = TRUE;
             }
         }
         else  pen_exist [ object_to_pen ( nag ) ] = TRUE ;
     }
     ObiektTok (&off, offk, &ad, ONieOkreslony);
   }
}



BOOL Draw_to_Plt_Proc (T_PTR_Plt_Config ptrs_config,
                       T_PTR_Plt_Ini_Date ptrs_ini_date)
/*--------------------------------*/
{
  int pen, err = 0;
  int k;
  BOOL b_err;

#if defined (D_PLT)
  setviewport(0, 0, 639, 479, 1);
  setwritemode(COPY_PUT);
#endif

  if (strlen(zbior_danych_2)>0)
   {
    k=pisz_zbior("RYSUJ.$2", FALSE, 1) ;
    if ( k )  return FALSE;
   }
  ptrs__config = ptrs_config ;
  ptrs__ini_date = ptrs_ini_date ;
  begin_draw = TRUE;
  dist_min = jednostkiplt ( 1.0 / ptrs__config->rozdz);
  Set_Line_Width (0) ;
  Set_Type_Drive (PLT_DRIVE, 0, jednostkiplt (ptrs__config->l_luk),
			 1 / jednostkiplt (ptrs__config->rozdz),
			 plt_d_to_jednostki_d (ptrs__ini_date->pen_width)) ;
  if ( plt_ini () == FALSE)
  {
    err = 8;
    goto err;
  }
 /* jezeli xp>0 lub yp>0 to caly rysunek musi byc przesuniety o xp, yp
   (przed centrowaniem)
 */
  find_pen ();
  for (pen = 0; pen < MAXNPEN; pen++)
  {
    if (pen_exist [pen] == FALSE) continue;
    if ( plt_pen_sel ( pen + 1) == FALSE ||
	 plt_pen_speed  (  ptrs__ini_date->tab_pen_speed [ pen ]  )   == FALSE ||
	 draw_sheet_plt1 (pen) == FALSE  ||  draw_sheet_plt (pen) == FALSE )
    {
      err = 8;
      goto err;
    }
  }
  plt_end ();
  return TRUE;
err:
  ErrList ( err ) ;
  return FALSE;
}

