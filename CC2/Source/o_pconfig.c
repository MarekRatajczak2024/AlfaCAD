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

#define __O_PCONFIG__

#include<forwin.h>
#ifndef LINUX
#include<io.h>
#include<dos.h>
#endif
#include<fcntl.h>
#include <string.h>

#include<stdlib.h>
#include <stdio.h>
#ifdef LINUX
#include <unistd.h>
#endif
#include<sys/stat.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "o_loadf.h"
#include "o_dialog.h"
#include "o_pltprn.h"
#include "o_libfun.h"
#include "o_inicnf.h"

#include "message.h"


extern double Aspect(TDIALOG *dlg, BOOL m, double asp0);
extern void GrMouseEraseCursor(void);
extern void GrMouseDisplayCursor(void);
extern int colors1(int old_color);
extern int my_getch(void);

#define Button_def {0,0,0,0,0,0,0,"",0,0, 0, 0, 0, 0, 0}

#define nrButtonOK          0
#define nrButtonCancel      1
#define nrButtonCol         2

#define ID_OK (nrButtonOK + 1)
#define ID_CANCEL (nrButtonCancel + 1)
#define ID_COLORY (nrButtonCol + 1)

#define nof_but  359 
#define nof_lab  22
#define nof_lin  0

#define XpLabC 5
#define XpButC 140 //150
#define YpLabCCol 5
#define DYBut 10 //(DYLab -3)
#define DXBut 10 //(DYBut - 5)
#define DYROW 10 //(DYLab - 10)

#define DXB 6
#define DYB 6

static int i__svgamode,i__svgamode1 = 0;
static int colors256 = 0;
static int old_color = 0;
static int new_color = 0;
static int numer_wiersza = 0;
static int ret;

static int col_state[24] ={15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15};
static int kolor_pola0[19]={15,13,15,12,11,2,12,1,13,15,0,0,0,0,0,0,0,0,0};
static int kolor_pola_profil[25]={7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};
static int kolor_pola_profil_tmp[25]={7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};

/*numer koloru aktywnego*/

static BOOL mouse;
static DLG_COLOR color_dlg;
static LABEL pdLab[nof_lab]={
         {XpLabC, YpLabCCol + 1 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_measurement_axis_},
		   {XpLabC, YpLabCCol + 2 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_existing_ground_line_},
		   {XpLabC, YpLabCCol + 3 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_designed_ground_line_},
		   {XpLabC, YpLabCCol + 4 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_street_line_},
		   {XpLabC, YpLabCCol + 5 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_existing_canal_line_},
		   {XpLabC, YpLabCCol + 6 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_designed_canal_line_},
		   {XpLabC, YpLabCCol + 7 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_elevation_values_},
		   {XpLabC, YpLabCCol + 8 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_l_material_},
		   {XpLabC, YpLabCCol + 9 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_distances_},
		   {XpLabC, YpLabCCol + 10 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_hectometers_},
		   {XpLabC, YpLabCCol + 11 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_installation_description_},
		   {XpLabC, YpLabCCol + 12 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_T_symbol_},
		   {XpLabC, YpLabCCol + 13 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_e_symbol_},
		   {XpLabC, YpLabCCol + 14 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_G_symbol_},
		   {XpLabC, YpLabCCol + 15 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_W_symbol_},
		   {XpLabC, YpLabCCol + 16 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_K_symbol_},
		   {XpLabC, YpLabCCol + 17 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_hole_axis_},
		   {XpLabC, YpLabCCol + 18 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_hole_number_},
		   {XpLabC, YpLabCCol + 19 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_hole_description_},
		   {XpLabC, YpLabCCol + 20 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_layers_description_},
		   {XpLabC, YpLabCCol + 21 * DYROW +1, 0, 0, COLOR_NULL, COLOR_NULL,_water_symbol_},
		   {350,6, 0, 0, COLOR_NULL, COLOR_NULL,_other_},
		   };


static GROUP_BOX gr_box1 []=
{
      { XpLabC, YpLabCCol ,
	380/*450*/, 22/*19*/ * DYROW + 2 * DYIL,
	COLOR_NULL ,COLOR_NULL,     	
	_OBJECT_COLORS_, 0, NULL,
      },
};

static BUTTON But_OK = {
			390, YpLabCCol, DXBut0, DYBut0,
			COLOR_NULL, COLOR_NULL, COLOR_NULL, "",
			0,B_PUSHBUTTON, 87, 1,0, ID_OK, 0, 0};

static BUTTON But_Cancel = {
	390, 60, DXBut0, DYBut0,
	COLOR_NULL, COLOR_NULL, COLOR_NULL, "",
	0,B_PUSHBUTTON, 83, 1,0,ID_CANCEL, 0, 0};


static BUTTON pdBut[nof_but];

static IMAGE images_pconf[] =
{
	{  WIDTH_WORLD-210, HEIGHT_WORLD-90, 48, 48, 112,_PROFILE_COLORS_},
};

static TDIALOG Config_profil={ 10,1, WIDTH_WORLD-143,HEIGHT_WORLD - 62,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0x40,0,0,
	_PROFILE_ELEMENT_COLOR_CONFIGURATION_,
			0, NULL,
			nof_lab, (LABEL(*)[])&pdLab,
			1, (GROUP_BOX(*)[])&gr_box1,
			0, NULL,
	       1, (IMAGE(*)[])&images_pconf,
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


static void conf_save_profil(void);
static void conf_ini_profil(int opcja);

   
int GetColorAC_config_profil(int color)
{ int colorAC;
  static int transf[]={0, 12, 14, 10, 11, 9, 13, 15, 8,4,6,2,3,1,5,7,0};
  
  if (color <= 16)
  {
   colorAC=transf[color];
   return colorAC;
  } 
  if (color > 255)
  {
    color = 15 ;
  }
  return color;
}

int GetColorAC1_config_profil(int color)
{ int colorAC;
  static int transf[]={0, 12, 14, 10, 11, 9, 13, 15, 8,4,6,2,3,1,5,7,0};
  if (color <= 16)
  {
   colorAC=transf[color];
   return colorAC;
  } 
  if (color > 255)
  {
    color = 15 ;
  }
  return color;
}

int GetColorDJGPP_config_profil(int color)
{ int colorDJGPP;

  static int transf1[]={0, 13, 11, 12,  9, 14, 10, 15, 8,  5, 3, 4, 1, 6, 2, 7, 0};

  if (color <= 16)
  {
   colorDJGPP=transf1[color];

   return colorDJGPP;
  } 
  if (color > 255)
  {
    color = 15 ;
  }

  return color;
}  

void init_button_profil (int nb, BUTTON *button, int xp, int yp, int dx, int dy)
/*---------------------------------------------------------------------*/
{ int i, x=xp, y=yp, nr_wiersza;
  for (i=0; i < 17/*16*/; i++, x+=dx + 4)
   {     button[nb + i].x = x;
	 button[nb + i].y = y;
	 button[nb + i].dx = dx;
	 button[nb + i].dy = dy;
	 if (i<16) button[nb + i].paper = GetColorAC(i);
	  else
	   {
	    nr_wiersza=(nb-nrButtonCol)/17;
	    if (kolor_pola_profil[nr_wiersza]<15) button[nb + i].paper = 15;
	      else button[nb + i].paper = kolor_pola_profil[nr_wiersza];
	   } 
	 
	 button[nb + i].border = 0;
	 button[nb + i].ink = 0;
	 button[nb + i].txt = "";
	 button[nb + i].check = 0  ;
	 button[nb + i].enable = 1  ;
	 button[nb + i].type = B_RADIOBUTTON ;
	 button[nb + i].name2 =0  ;
	 button[nb + i].id = nb + i + 1;
  }
}


void init_button_config_dialog_profil(void)
/*---------------------------------------*/
{
  int nb;
/*kolory*/
  nb = nrButtonCol;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+1 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+2 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+3 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+4 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+5 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+6 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+7 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+8 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+9 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+10 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+11 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+12 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+13 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+14 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+15 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+16 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+17 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+18 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+19 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+20 * DYROW, DXBut, DYBut);
  nb += 17;
  init_button_profil( nb,pdBut, XpButC, YpLabCCol+21 * DYROW, DXBut, DYBut);
}


static int config_profil( int n)
{
  int ret ;
  int wiersz, nrBat;
  double asp;

  i__svgamode1=0;
  colors256=0;
  ret = Dlg_Ret_Val_Continue ;
  switch (n)
   {
	 case ID_OK:
	   conf_save_profil();
	   ret = Dlg_Ret_Val_OK ;
	   break;
	 case ID_CANCEL:
	   ret = Dlg_Ret_Val_Cancel ;
	   break;
	 default :
	   n-=1;
	   wiersz = (n - nrButtonCol  ) / 17;
	   nrBat = nrButtonCol  + wiersz*17 + col_state[wiersz];
	   if (nrBat != n)
		{
	   	  GrMouseEraseCursor();
		  pdBut [nrBat].check = 0;
		  Draw_Button (&pdBut [nrBat] );
		  pdBut [n].check = 1;
		  Draw_Button (&pdBut [n] );
          GrMouseDisplayCursor();
		}
	   col_state [(n - nrButtonCol  ) / 17] = (n - nrButtonCol ) % 17 ;
	   if ((n - nrButtonCol) % 17==16)
	    {
	     ret = Dlg_Ret_Val_Return;
	     colors256 = 1;
	     old_color = kolor_pola_profil [wiersz];
	     numer_wiersza = wiersz;
	     break;
	    } 
	   kolor_pola_profil [(n - nrButtonCol  ) / 17] = col_state [(n - nrButtonCol ) / 17];
       ret =  Dlg_Ret_Val_Reset ;	
   }
  return ret;
}

static void conf_ini_profil(int opcja)
/*----------------------------------*/
{ int f;
  int nrBat,i;
  unsigned roz;

if (opcja==0)
 {
    
  f=open("COLORSP.CNF",O_RDWR|O_BINARY);
  if (f==-1)
   {
     goto aa;
   }
  read(f, kolor_pola_profil, sizeof(kolor_pola_profil));
  close(f);
  
 }
aa:
  for (i=0; i<21; i++)
   {
         col_state[i]=kolor_pola_profil[i];
         if (col_state[i]>15) col_state[i]=16;
	 nrBat = nrButtonCol + i*17/*16*/ + col_state[i];
	 pdBut [nrBat].check = 1;
   }

  return;
}

static void conf_save_profil(void)
/*------------------------------*/
{ int f;
  int i;

#ifndef LINUX
    f=open("COLORSP.CNF", O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
    f=open("COLORSP.CNF", O_CREAT|O_TRUNC|O_RDWR, 0666); //S_IRWXU | S_IRWXG | S_IRWXO);
#endif
  if (f!=-1)
   {
	 write(f, kolor_pola_profil, sizeof(kolor_pola_profil));
	 close(f);
   }
}



int pkonfig(KOLORY_PROFILI *kolory_profil)
/*-------------------------------------*/
{
  int err;
  int Mode;
  BOOL first_time;
  int i;
  int k;
    static int curr_h, curr_v;

  
  memmove(&kolor_pola_profil,kolory_profil,sizeof(KOLORY_PROFILI));

 if(get_mem_menu()==NULL) {ErrList(18);k=my_getch(); goto aa;}

  memmove (pdBut, &But_OK, sizeof(BUTTON));
  memmove (pdBut+1, &But_Cancel, sizeof(BUTTON));
  Get_Default_Color_Dlg (&color_dlg); 
  
  
_Reset1_ :  

  init_button_config_dialog_profil();
  conf_ini_profil(1);
  Save_Update_flex(0, &curr_h, &curr_v);
  ret = Dialog(&Config_profil, &color_dlg, config_profil, bMouse);
  Save_Update_flex(1, &curr_h, &curr_v);
aa:
  if (colors256 > 0) 
   {
    new_color=colors1(old_color);
    kolor_pola_profil [numer_wiersza] = new_color;
    goto _Reset1_;
   } 

 if (ret==Dlg_Ret_Val_OK)  //aktualizacja kolorow
  {

	memmove(kolory_profil,&kolor_pola_profil, sizeof(KOLORY_PROFILI));
	return 1;
  }
	return 0;
}

#undef __O_PCONFIG__