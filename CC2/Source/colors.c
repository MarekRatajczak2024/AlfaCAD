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

#define __COLORS__
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
#include "alf_res.h"
#include "o_plt.h"
#include "o_lfile.h"
#include "o_image.h"
#include "o_inicnf.h"
#include "b_bibe.h"
#include "rysuj_e.h"

#include "dialog.h"

extern int Save_Screen(void);
extern int Restore_Screen(void);
extern int GetColorACPlt(int color);

int new_color;


#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

#define Button_def {0,0,0,0,0,0,0,"",0,0, 0, 0, 0}

#define nrButtonCancel     0
#define nrButtonCol        1

#define ID_CANCEL 1

#define ID_MOVE -3

#define nof_but  256

#define XpLabC 25/*10*/
#define XpButC 120
#define YpLabC 40 /*30*/

#define XpBox 10
#define YpBox 5

#define XpBut 6
#define YpBut 10

#define DYBut 8 //12 
#define DXBut 12 //16

#define DYROW (DYLab - 3)     

#define DXB 6
#define DYB 6

#define DXCancel 35
#define DYCancel 25

#define DXDistance 5


static int col_state = 7;
											/*numer koloru aktywnego*/
static DLG_COLOR color_dlg;

static GROUP_BOX gr_box1 []=
{
      {	XpBox,	YpBox, DXBut * 16 + XpBut*2, DYBut * 16 + YpBut + 6, //275, 215,
	COLOR_NULL ,COLOR_NULL,     
	KOLORY, 0, NULL,
      },
};

static BUTTON But_Cancel = {
	XpBox + DXBut * 16 + XpBut * 2 + DXDistance, YpBox + DYBut * 16 + YpBut + 6 - DYCancel , DXCancel, DYCancel, //50, 30,
	COLOR_NULL, COLOR_NULL, COLOR_NULL, "Esc",
	0,B_PUSHBUTTON, 99, 1,0,ID_CANCEL, 0, 0};

static BUTTON pdBut[nof_but]=Button_def;

const IMAGE images_col[] =
{
 { XpBox + DXBut * 16 + XpBut * 2 + DXDistance + 6, 5, 48, 48, 112, image_col_tips0},
};

static TDIALOG colors_config={ XpLabC,YpLabC-20, XpBox * 2 + DXBut * 16 + XpBut * 2 + DXCancel + DXDistance, YpBox * 2 + DYBut * 16 + YpBut + 9,  /*350,225,*/  COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0x40,0,0,
	        CHOOSE_COLORS,
			0, NULL,
			0, NULL,
			1/*2*/, (GROUP_BOX(*)[]) &gr_box1,
			0, NULL, /*1, &pdLin, */
			1, (IMAGE(*)[])&images_col,
			nof_but,(BUTTON(*)[]) &pdBut,
			0, NULL,
			0,NULL,
            0,NULL, //Sliders
			NULL,
	        NULL,
	        0,
	        0,
            NULL,
};

static TDIALOG colors_config1 = { XpLabC,YpLabC - 20, XpBox * 2 + DXBut * 16 + XpBut * 2 + DXCancel + DXDistance, YpBox * 2 + DYBut * 16 + YpBut + 9,  /*350,225,*/  COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0x40,0,0,
	        CHOOSE_COLORS,
			0, NULL,
			0, NULL,
			1/*2*/,  (GROUP_BOX(*)[])&gr_box1,
			0, NULL, /*1, &pdLin, */
			1, (IMAGE(*)[])&images_col,
			nof_but,(BUTTON(*)[])&pdBut,
			0, NULL,
			0,NULL,
            0,NULL, //Sliders
	        NULL,
	        NULL,
	        0,
	        0,
            NULL,
};


char empty[1] = "";

void init_button_colors (int nb, BUTTON *button, int xp, int yp, int dx, int dy)
/*---------------------------------------------------------------------*/
{ int i,j, x=xp, y=yp;
	 
  for (i=0; i < 15; i++, y+=dy )
   {x=xp;
    for (j=0; j < 16; j++, x+=dx )
        {
         button[nb + (i*16) + j].x = x;
         button[nb + (i*16) + j].y = y;
		 button[nb + (i*16) + j].dx = dx;
		 button[nb + (i*16) + j].dy = dy;
		 button[nb + (i*16) + j].paper = GetColorACPlt(1+(i*16) + j);  //GetColorAC
		 button[nb + (i*16) + j].border = BLACK;
		 button[nb + (i*16) + j].ink = 0;
		 button[nb + (i*16) + j].txt = empty;
		 button[nb + (i*16) + j].check = 0  ;
		 button[nb + (i*16) + j].enable = 1  ;
		 button[nb + (i*16) + j].type = B_RADIOBUTTON ;	/*B_CHECKBOX ;*/ /*B_RADIOBUTTON*/
		 button[nb + (i*16) + j].name2 =0  ;
		 button[nb + (i*16) + j].id = nb + (i*16) + j +1;
		} 
   } 
   x=xp;
   for (j=0; j < 15; j++, x+=dx )
        {
         button[nb + 240 + j].x = x;
         button[nb + 240 + j].y = y;
		 button[nb + 240 + j].dx = dx;
		 button[nb + 240 + j].dy = dy;
		 button[nb + 240 + j].paper = GetColorACPlt(1+ 240 + j);  //GetColorAC
		 button[nb + 240 + j].border = BLACK;
		 button[nb + 240 + j].ink = 0;
		 button[nb + 240 + j].txt = empty;
		 button[nb + 240 + j].check = 0  ;
		 button[nb + 240 + j].enable = 1  ;
		 button[nb + 240 + j].type = B_RADIOBUTTON ;	/*B_CHECKBOX ;*/ /*B_RADIOBUTTON*/
		 button[nb + 240 + j].name2 =0  ;
		 button[nb + 240 + j].id = nb + 241 + j;
		} 
}


void init_button_colors_dialog(void)
/*--------------------------------*/
{
  int nb;
  nb = nrButtonCol ;
  init_button_colors( nb,pdBut, XpBox + XpBut, YpBox + YpBut, DXBut, DYBut); /*1..256*/
}


int colors_set(int n)
{
  int ret ;
  int nrBat;

  ret = Dlg_Ret_Val_Continue ;
  switch (n)
   {
	 case ID_CANCEL:  /*0*/
	   ret = Dlg_Ret_Val_Cancel;
	   break;
     case ID_MOVE:  /*-3*/
       ret = Dlg_Ret_Val_Continue;
       break;
	 default :  /*2..256*/
	   nrBat = col_state;
	   if (nrBat != n)
		{
		  scare_mouse();
		  pdBut [nrBat-1].check = 0;
		  Draw_Button (&pdBut [nrBat-1] );
		  pdBut [n-1].check = 1;
		  Draw_Button (&pdBut [n-1] );
		}
	   new_color = n - 1;
   	   ret = Dlg_Ret_Val_OK;
   	   break;
   }
  return ret;
}

int colors_set1(int n)
{
  int ret ;
  //int wiersz;
  int nrBat;
  //double asp;

  ret = Dlg_Ret_Val_Continue ;
  switch (n)
   {
	 case ID_CANCEL:  /*0*/
	   ret = Dlg_Ret_Val_Cancel;
	   break;
     case ID_MOVE:  /*-3*/
       ret = Dlg_Ret_Val_Continue;
       break;
	 default :  /*1..256*/
	   nrBat = col_state;
	   if (nrBat != n)
		{
		  scare_mouse();
		  pdBut [nrBat-1].check = 0;
		  Draw_Button (&pdBut [nrBat-1] );
		  pdBut [n-1].check = 1;
		  Draw_Button (&pdBut [n-1] );
		  unscare_mouse();
		}
	   new_color = n - 1;
   	   ret = Dlg_Ret_Val_OK;
   	   break;
   }
  return ret;
}

unsigned int colors_zone(unsigned int *color_to_set)
/*-------------------------------------------------*/
{
	int nrBat;
	static int komunikat_R, komunikat_R0;
	int ret;
    static int curr_h, curr_v;
	
	komunikat_R0 = Komunikat_R0;
	memmove(pdBut, &But_Cancel, sizeof(BUTTON));
	Get_Default_Color_Dlg(&color_dlg);
	init_button_colors_dialog();
	col_state = (unsigned int)color_to_set;
	nrBat = col_state+1;
	Set_Check_Button(&colors_config, nrBat, 1);

    Save_Update_flex(0, &curr_h, &curr_v);
	ret = Dialog(&colors_config, &color_dlg, colors_set1, bMouse);
    Save_Update_flex(1, &curr_h, &curr_v);
	komunikat0(komunikat_R0);
	
	return ret;
}



int colors(void)
/*-------------*/
{ int nrBat;
  static int komunikat_R, komunikat_R0;
  static int curr_h, curr_v;

  komunikat_R0 = Komunikat_R0 ;
  memmove (pdBut, &But_Cancel, sizeof(BUTTON));
  Get_Default_Color_Dlg (&color_dlg);
  init_button_colors_dialog();
  col_state = LiniaG.kolor + 1;
  new_color = LiniaG.kolor;
  nrBat = col_state;
  Set_Check_Button(&colors_config, nrBat, 1);

  Save_Update_flex(0, &curr_h, &curr_v);
  Dialog(&colors_config, &color_dlg, colors_set, bMouse);
  Save_Update_flex(1, &curr_h, &curr_v);

  komunikat0(komunikat_R0);
  return new_color;
}


int colors1(int old_color)
/*----------------------*/
{ int nrBat;
  static int komunikat_R, komunikat_R0;

  int dx_win_ask, dy_win_ask, x_win_ask, y_win_ask; // x_win, y_win; // x1_win, y1_win;
  double wsp_x, wsp_y;
  static int curr_h, curr_v;

  komunikat_R0 = Komunikat_R0 ;
  memmove (pdBut, &But_Cancel, sizeof(BUTTON));
  Get_Default_Color_Dlg (&color_dlg);
  init_button_colors_dialog();
  new_color = old_color;
  col_state = old_color + 1;
  nrBat = col_state;
  Set_Check_Button(&colors_config, nrBat, 1);

  wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
  wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

  x_win_ask = colors_config.x*wsp_x;
  y_win_ask = 26 + colors_config.y*wsp_y;

  dx_win_ask = 10 + colors_config.dx*wsp_x;
  dy_win_ask = 58 /*52*/ + colors_config.dy*wsp_y;


  Save_Update_flex(0, &curr_h, &curr_v);
  Dialog(&colors_config, &color_dlg, colors_set1, bMouse);
  Save_Update_flex(1, &curr_h, &curr_v);

  komunikat0(komunikat_R0);
  return new_color;
}

int colors2(int old_color)
/*----------------------*/
{ int nrBat;
  static int komunikat_R, komunikat_R0;
  LINIA LiniaGB ;
  int new_color;
  static int curr_h, curr_v;

  komunikat_R0 = Komunikat_R0 ;
  memmove (pdBut, &But_Cancel, sizeof(BUTTON));
  Get_Default_Color_Dlg (&color_dlg);
  init_button_colors_dialog();
  LiniaGB.kolor = LiniaG.kolor;
  LiniaG.kolor=old_color;
  col_state=LiniaG.kolor + 2;
  nrBat = col_state;
  Set_Check_Button(&colors_config, nrBat, 1);

  Save_Update_flex(0, &curr_h, &curr_v);
  Dialog(&colors_config1, &color_dlg, colors_set1, bMouse);
  Save_Update_flex(1, &curr_h, &curr_v);

  komunikat0(komunikat_R0);

  Error = FALSE;
  Info = FALSE;
  new_color=LiniaG.kolor;
  LiniaG.kolor=LiniaGB.kolor;
 return new_color;
 
}

#undef __COLORS__
