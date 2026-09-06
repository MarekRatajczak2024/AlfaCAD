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

#define __O_CONFIG__
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

extern void GrMouseEraseCursor(void);
extern void GrMouseDisplayCursor(void);
extern int colors1(int old_color);
extern int my_getch(void);


#define Button_def {0,0,0,0,0,0,0,"",0,0, 0, 0, 0}

#define nrButtonOK          0
#define nrButtonCancel      1
#define nrButtonLocal       2
#define nrButtonGlobal      3
#define nrButtonSave        4
#define nrButtonMenu        5
#define nrButtonCol (5+3*17)

#define ID_OK nrButtonOK+255
#define ID_CANCEL nrButtonCancel
#define ID_Local  nrButtonLocal
#define ID_Global nrButtonGlobal
#define ID_Save   nrButtonSave
#define ID_COLORS nrButtonCol

#define nof_but  175
#define nof_lab  11 //13
#define nof_lin  1

#define XpLabC 5
#define XpButC 100
#define YpLabCMen 5
#define YpLabCCol 55
#define DYBut 10
#define DXBut 10
#define DYROW 10

#define DXBut01 55
#define DYBut01 32
#define DYBut02 24


#define DXB 6
#define DYB 6

static int i__svgamode,i__svgamode1 = 0;
static int colors256 = 0;
static int old_color = 0;
static int new_color = 0;
static int numer_wiersza = 0;
static int cur_global_lokal;

static int ret;


static int col_state[19] ={15,13,15,12,11,2,12,1,13,15,0,0,0,0,0,0,0,0,0};
static int kolor_pola_tmp[19]={15,13,15,12,11,2,12,1,13,15,0,0,0,0,0,0,0,0,0};
static int kolor_pola[19]={15,13,15,12,11,2,12,1,13,15,0,0,0,0,0,0,0,0,0};
static int kolor_pola_local[19]={15,13,15,12,11,2,12,1,13,15,0,0,0,0,0,0,0,0,0};

static void config_save(void);
static void conf_ini(int opcja);

											/*numer koloru aktywnego*/
static DLG_COLOR color_dlg;
static BOOL local=TRUE;

static LABEL pdLab[nof_lab]={
		   {XpLabC, YpLabCMen + 1 * DYROW + DYL, 0, 0, COLOR_NULL, COLOR_NULL,config_labels0},
		   {XpLabC, YpLabCMen + 2 * DYROW + DYL, 0, 0, COLOR_NULL, COLOR_NULL,config_labels1},
		   {XpLabC, YpLabCMen + 3 * DYROW + DYL, 0, 0, COLOR_NULL, COLOR_NULL,config_labels2},

		   {XpLabC, YpLabCCol + 1 * DYROW + DYL, 0, 0, COLOR_NULL, COLOR_NULL,config_labels3},
		   {XpLabC, YpLabCCol + 2 * DYROW + DYL, 0, 0, COLOR_NULL, COLOR_NULL,config_labels4},
		   {XpLabC, YpLabCCol + 3 * DYROW + DYL, 0, 0, COLOR_NULL, COLOR_NULL,config_labels5},
		   {XpLabC, YpLabCCol + 4 * DYROW + DYL, 0, 0, COLOR_NULL, COLOR_NULL,config_labels6},
		   {XpLabC, YpLabCCol + 5 * DYROW + DYL, 0, 0, COLOR_NULL, COLOR_NULL,config_labels7},
		   {XpLabC, YpLabCCol + 6 * DYROW + DYL, 0, 0, COLOR_NULL, COLOR_NULL,config_labels8},
		   {XpLabC, YpLabCCol + 7 * DYROW + DYL, 0, 0, COLOR_NULL, COLOR_NULL,config_labels9},
		   //{400,212, 0, 0, COLOR_NULL, COLOR_NULL,config_labels10},
		   //{400,224, 0, 0, COLOR_NULL, COLOR_NULL,local_global},
		   {310, YpLabCMen + 1, 0, 0, COLOR_NULL, COLOR_NULL,config_labels11},
		   };


static GROUP_BOX gr_box1 []=
{
      { XpLabC, 	YpLabCMen ,
	340, 	 4 * DYROW + 2 * DYIL,
	COLOR_NULL ,COLOR_NULL,     	 /*kolory */
	 config_groups0, 0, NULL,
      },
      {	XpLabC, 	YpLabCCol ,
	340, 	8 * DYROW + 2 * DYIL,
	COLOR_NULL ,COLOR_NULL,     	 /*kolory */
	 config_groups1, 0, NULL,
      },
};

#define DIST_LIN 3
static DARK_LIGHT_LINE line_d_l [] =
{
	{ 350, DIST_LIN, 1, 139, COLOR_NULL, COLOR_NULL},
};


static BUTTON But_OK = { 358, 109 /*YpLabCMen + 1*/, DXBut01, DYBut01, COLOR_NULL, COLOR_NULL, COLOR_NULL,
	"",0,B_PUSHBUTTON, 87, 1,0, ID_OK, 0, 0};

static BUTTON But_Cancel = { 358, 73 /*31*/, DXBut01, DYBut01, COLOR_NULL, COLOR_NULL, COLOR_NULL, 
    "", 0,B_PUSHBUTTON, 83, 1,0,ID_CANCEL, 0, 0};


static BUTTON But_Global = { 360, 37, 20, DYBut, COLOR_NULL, COLOR_NULL, COLOR_NULL,
	config_buttons2, 0, B_RADIOBUTTON, 0, 1, 0, ID_Global, 0, 0,
};

static BUTTON But_Local = { 360, 56, 20, DYBut, COLOR_NULL,COLOR_NULL, COLOR_NULL,  
	config_buttons3, 0, B_RADIOBUTTON, 0, 1,0, ID_Local, 0, 0,
};

static BUTTON But_Save = { 358, YpLabCMen + 1 /*120*/, DXBut01, DYBut02, COLOR_NULL, COLOR_NULL, COLOR_NULL,
	"",0,B_PUSHBUTTON, 84, 1,0,ID_Save, 0, 0};
	   	   
static BUTTON pdBut[nof_but];

static IMAGE images_conf[] =
{
 {  375, 32, 48, 48, 110,image_conf_tips0},
 {  375, 52, 48, 48, 111,image_conf_tips1},
};

static TDIALOG Config={ 10,1, 420, 145,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0x40,0,0,
	config_title,
			1, (DARK_LIGHT_LINE(*)[])&line_d_l,
			nof_lab, (LABEL(*)[])&pdLab,
			2, (GROUP_BOX(*)[])&gr_box1,
			0, NULL, /*1, &pdLin, */
			2, (IMAGE(*)[])&images_conf,
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


int GetColorAC_config(int color)
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
   


void init_button (int nb, BUTTON *button, int xp, int yp, int dx, int dy)
/*---------------------------------------------------------------------*/
{ int i, x=xp, y=yp, nr_wiersza;

  for (i=0; i < 17; i++, x+=dx + 4)
   {     button[nb + i].x = x;
	 button[nb + i].y = y;
	 button[nb + i].dx = dx;
	 button[nb + i].dy = dy;
	 if (i<16) button[nb + i].paper = GetColorAC_config(i)/*i*/;
	  else
	   {
	    nr_wiersza=(nb-nrButtonMenu)/17;
	    if (kolor_pola[nr_wiersza]<15) button[nb + i].paper = 15;
	      else button[nb + i].paper = kolor_pola[nr_wiersza];
	   } 
	 
	 button[nb + i].border = 0; //BLACK;
	 button[nb + i].ink = 0;
	 button[nb + i].txt = "";
	 button[nb + i].check = 0  ;
	 button[nb + i].enable = 1  ;
	 button[nb + i].type = B_RADIOBUTTON ;
	 button[nb + i].name2 =0  ;
	 button[nb + i].id = nb + i + 1;
  }
}


void init_button_config_dialog(void)
/*-----------------------------*/
{
  int nb;
  pdBut[0] = But_OK;
  pdBut[1] = But_Cancel;
  pdBut[2] = But_Local;
  pdBut[3] = But_Global;
  pdBut[4] = But_Save;
/*menu*/
  nb = nrButtonMenu;
  init_button( nb,pdBut, XpButC, YpLabCMen+1 * DYROW, DXBut, DYBut); /*ramka*/;
  nb += 17;
  init_button( nb,pdBut, XpButC, YpLabCMen+2 * DYROW, DXBut, DYBut); /*tlo*/;
  nb += 17 ;
  init_button( nb,pdBut, XpButC, YpLabCMen+3 * DYROW, DXBut, DYBut); /*tekst*/;
/*kolory...*/
  nb = nrButtonCol;
  init_button( nb,pdBut, XpButC, YpLabCCol+1 * DYROW, DXBut, DYBut); /*ramka*/;
  nb += 17;
  init_button( nb,pdBut, XpButC, YpLabCCol+2 * DYROW, DXBut, DYBut); /*tekst*/;
  nb += 17 ;
  init_button( nb,pdBut, XpButC, YpLabCCol+3 * DYROW, DXBut, DYBut); /*tekst*/;
  nb += 17;
  init_button( nb,pdBut, XpButC, YpLabCCol+4 * DYROW, DXBut, DYBut); /*tekst*/;
  nb += 17;
  init_button( nb,pdBut, XpButC, YpLabCCol+5 * DYROW, DXBut, DYBut); /*tekst*/;
  nb += 17;
  init_button( nb,pdBut, XpButC, YpLabCCol+6 * DYROW, DXBut, DYBut); /*tekst*/;
  nb += 17;
  init_button( nb,pdBut, XpButC, YpLabCCol+7 * DYROW, DXBut, DYBut); /*tekst*/;

  Check_Radio_Button(&Config, ID_Local, ID_Global, local == TRUE ? ID_Local : ID_Global, FALSE);
  
}


static int config( int n)
{
  int wiersz, nrBat;

  i__svgamode1=0;
  colors256=0;
  ret = Dlg_Ret_Val_Continue ;
  switch (n)
   {
	 case ID_OK:
	   ret = Dlg_Ret_Val_OK ;
	   break;
	 case ID_CANCEL:
	   ret = Dlg_Ret_Val_Cancel ;
	   break;
	 case ID_Local:
       memmove(&kolor_pola,&kolor_pola_local,sizeof(KOLORY));
	   ret = Dlg_Ret_Val_Local;
	   local = TRUE;
	   Check_Radio_Button(&Config, ID_Local, ID_Global, n, TRUE);
	   break;
	 case ID_Global:
       conf_ini(0);
	   ret = Dlg_Ret_Val_Global;
	   local = FALSE;
	   Check_Radio_Button(&Config, ID_Local, ID_Global, n, TRUE);
	   break;                 
	 case ID_Save:
       config_save();
	   ret = Dlg_Ret_Val_Save ;
	   break;   
     default :
	   n-=1;
	   wiersz = (n - nrButtonMenu  ) / 17/*16*/;
	   nrBat = nrButtonMenu  + wiersz*17/*16*/ + col_state[wiersz];
	   if (nrBat != n)
		{
		  GrMouseEraseCursor();
		  pdBut [nrBat].check = 0;
		  Draw_Button (&pdBut [nrBat] );
		  pdBut [n].check = 1;
		  Draw_Button (&pdBut [n] );
		  GrMouseDisplayCursor();
		}
	   col_state [(n - nrButtonMenu  ) / 17] = (n - nrButtonMenu ) % 17 ;
	   if ((n - nrButtonMenu) % 17==16)
	    {
	     ret = Dlg_Ret_Val_Return;
	     colors256 = 1;
	     old_color = kolor_pola [wiersz];
	     numer_wiersza = wiersz;
	     break;
	    } 
	   kolor_pola [(n - nrButtonMenu  ) / 17] = col_state [(n - nrButtonMenu  ) / 17];
	   ret =  Dlg_Ret_Val_Reset ;	
   }
  return ret;
}


int GetColorDJGPP_config(int color)
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



static void conf_ini(int opcja)
/*----------------------------*/
{ int f;
  int nrBat,i;

if (opcja==0)
 {
  f=open("COLORS.CNF",O_RDWR|O_BINARY);
  if (f==-1)
   {
	 goto aa;
   }
  read(f, kolor_pola, sizeof(kolor_pola));
  close(f);

  for (i=0; i<19; i++)
   {
   kolor_pola[i]=GetColorDJGPP_config(kolor_pola[i]);
   }
  
 } 
aa:
  for (i=0; i<10; i++)
   {
         col_state[i]=kolor_pola[i];
         if (col_state[i]>15) col_state[i]=16;
	 nrBat = nrButtonMenu + i*17/*16*/ + col_state[i];
	 pdBut [nrBat].check = 1;
   }

  return;
}

static void config_save(void)
/*-------------------------*/
{ int f;
  int i;

#ifndef LINUX
    f=open("COLORS.CNF", O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
    f=open("COLORS.CNF", O_CREAT|O_TRUNC|O_RDWR, 0666); //S_IRWXU | S_IRWXG | S_IRWXO);
#endif
  if (f!=-1)
   {
          for (i=0; i<19; i++)
          {
           kolor_pola_tmp[i]=GetColorAC_config(kolor_pola[i]);
          }
	 write(f, kolor_pola_tmp, sizeof(kolor_pola_tmp));
	 close(f);
   }
}



int konfig(KOLORY *kolory_pulpitu)
/*-----------------------------*/
{
	int i;
	int k;
    static int curr_h, curr_v;

	memmove(&kolor_pola_tmp, kolory_pulpitu, sizeof(KOLORY));
	for (i = 0; i < 19; i++)
	{
		kolor_pola[i] = GetColorDJGPP_config(kolor_pola_tmp[i]);
	}

	memmove(&kolor_pola_local, kolor_pola, sizeof(KOLORY));

	if (get_mem_menu() == NULL) { ErrList(18);k = my_getch(); goto aa; }
	memmove(pdBut, &But_OK, sizeof(BUTTON));
	memmove(pdBut + 1, &But_Cancel, sizeof(BUTTON));
	memmove(pdBut + 2, &But_Local, sizeof(BUTTON));
	memmove(pdBut + 3, &But_Global, sizeof(BUTTON));
	memmove(pdBut + 4, &But_Save, sizeof(BUTTON));   //as global
	Get_Default_Color_Dlg(&color_dlg);

_Reset1_:

	init_button_config_dialog();

	conf_ini(1);

    Save_Update_flex(0, &curr_h, &curr_v);
	ret = Dialog(&Config, &color_dlg, config, bMouse);
    Save_Update_flex(1, &curr_h, &curr_v);

aa:
	if (colors256 > 0)
	{
		new_color = colors1(old_color);
		kolor_pola[numer_wiersza] = new_color;
		
		goto _Reset1_;
	}
	

  if (ret==Dlg_Ret_Val_OK)  //aktualizacja kolorow
  {
	for (i=0; i<19; i++)
     {
       kolor_pola_tmp[i]=GetColorAC_config(kolor_pola[i]);
     }
	memmove(kolory_pulpitu,&kolor_pola_tmp, sizeof(KOLORY));
	return 1;
  }
    return 0;
}

#undef __O_CONFIG__