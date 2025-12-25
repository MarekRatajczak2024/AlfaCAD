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

#define __ASKING__
#include<forwin.h>
#ifndef LINUX
#include<io.h>
#include<dos.h>
#endif
#include<fcntl.h>
#include <string.h>

#include<stdlib.h>
#include <stdio.h>
#include<sys/stat.h>
#define ALLEGWIN
#include <allegext.h>
//#pragma pack(4)  to avoid warning message
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_loadf.h"
#include "o_dialog.h"
#include "o_pltprn.h"
#include "o_libfun.h"
#include "o_inicnf.h"

#include "menu.h"

#define  XpBox1  145
#define  YpBox1  100
#define  XpBoxIcon  145 + 2 //10
#define  YpBoxIcon  100 + 12 //10
#define  Xp1  5
#define  Yp1  5
#define  DXBox1  250 //350
#define  DXBox1as  296 //350
#define  DYBox1  50 //70
#define  DYBox2  54 //70
#define  DXBut1  45 //50 //54 //75
#define  DYBut1  20
#define  XpOK1  Xp1 + 160
#define  YpOK1  Yp1 + 28
#define  XpCANCEL1  Xp1 + DXBox1 - DXBut1 - 75
#define  XpESCAPE1  Xp1 + DXBox1 + DXBox1 - DXBut1 - 75
#define  YpCANCEL1  Yp1 + 28 //40
#define  YpESCAPE1  Yp1 + 28 //40

#define  XpEUROCODE  Xp1 + 32

#define DXShift 0 //20

#define ID_OK             1
#define ID_CANCEL         2
#define ID_ESCAPE         3

#define ID_EUROCODE       4
#define ID_ASCE           5
#define ID_ICC            6
#define ID_GEOM_STIFFNESS 7
#define ID_INERTIA        8
#define ID_PINNABLE       9
#define ID_THETA         10
#define ID_SIGMA_EQ      11
#define ID_EPSILON       12
#define ID_COMBINATION   13
#define ID_MODES_NUMBER  14

#define MaxComLen 120

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

int XpBox1_ = XpBox1;
int YpBox1_ = YpBox1;

extern void getimage(int left, int top, int right, int bottom, void  *bitmap);
extern void putimage(int left, int top,  void  *bitmap, int op);
extern int my_text_length(FONT* font, const char* text);
extern int Dialog_in_dialog(TDIALOG *dlg);
extern void FreeMouse(void);
extern void LockMouse(void);
//extern BITMAP *qmark;
extern char *qmark_p;
extern char *eymark_p;
extern char *ermark_p;
extern char *offmark_p;

int i_image_no ;
static DLG_COLOR color_dlg;

int ask_question(int n_buttons, char* esc_string, char* ok_string, char* cont_string, char* comment_string, int color_comment, char* comment1_string, int color1_comment, int cien, int image);


static LABEL lab[]={
{Xp1 + DXShift, Yp1 + 5, 0, 0, COLOR_NULL ,COLOR_NULL,""},
{Xp1 + DXShift, Yp1 + 15, 0, 0, COLOR_NULL ,COLOR_NULL,""},
{Xp1 + DXShift, Yp1 + 25, 0, 0, COLOR_NULL ,COLOR_NULL,""},
};

static GROUP_BOX gr_box []=
{
  { Xp1, Yp1, DXBox1, DYBox1,	COLOR_NULL ,COLOR_NULL, "", 0, NULL,
  },
};


static BUTTON buttons [] =
        {
                { XpOK1, YpOK1, DXBut1, DYBut1, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, ""/*ok_string1*/, 0,B_PUSHBUTTON, 0, 1,0, ID_OK, 0, 0,
                },
                { XpCANCEL1, YpCANCEL1, DXBut1, DYBut1, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, ""/*esc_string1*/, 0,B_PUSHBUTTON, 0, 1,0, ID_CANCEL, 0, 0,
                },
                { XpESCAPE1, YpESCAPE1, DXBut1, DYBut1, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, ""/*cont_string1*/, 0,B_PUSHBUTTON, 0, 1,0, ID_ESCAPE, 0, 0,
                },
                { Xp1+13, YpESCAPE1, 20, 20, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, ""/*cont_string1*/, 1,B_RADIOBUTTON, 0, 1,0, ID_EUROCODE, 0, 0,
                },
                { Xp1+45, YpESCAPE1, 20, 20, COLOR_NULL,
                          COLOR_NULL, COLOR_NULL, ""/*cont_string1*/, 0,B_RADIOBUTTON, 0, 1,0, ID_ASCE, 0, 0,
                },
                { Xp1+77, YpESCAPE1, 20, 20, COLOR_NULL,
                          COLOR_NULL, COLOR_NULL, ""/*cont_string1*/, 0,B_RADIOBUTTON, 0, 1,0, ID_ICC, 0, 0,
                },
                { XpESCAPE1+50, YpESCAPE1, 20, 20, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, ""/*cont_string1*/, 0,B_CHECKBOX, 0, 1,0, ID_GEOM_STIFFNESS, 0, 0,
                },
                { XpESCAPE1+100, YpESCAPE1, 20, 20, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, ""/*cont_string1*/, 0,B_CHECKBOX, 0, 1,0, ID_INERTIA, 0, 0,
                },
                { XpESCAPE1+500, YpESCAPE1+20, 20, 20, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, ""/*cont_string1*/, 0,B_CHECKBOX, 0, 1,0, ID_PINNABLE, 0, 0,
                },
                { XpESCAPE1+20, YpESCAPE1, 10, 10, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, "θ", 0,B_CHECKBOX, 0, 1,0, ID_THETA, 0, 0,
                },
                { XpESCAPE1+250, YpESCAPE1, 10, 10, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, "Σσ", 0,B_CHECKBOX, 0, 1,0, ID_SIGMA_EQ, 0, 0,
                },
                { XpESCAPE1+300, YpESCAPE1+20, 10, 10, COLOR_NULL,
                        COLOR_NULL, COLOR_NULL, "ε", 0,B_CHECKBOX, 0, 1,0, ID_EPSILON, 0, 0,
                },
        };


char notice[16] = _NOTICE_;

static IMAGE images_a [] =
{
 { Xp1+5, Yp1 + 5, 64, 64, 0, _AT_NOTICE_},  //YpOK1-4
};

#define _EUROCODE_ u8"EUROCODE"
#define _ASCE_ u8"ASCE"
#define _ICC_ u8"ICC"

static IMAGE images_as [] =
{
            { Xp1+5, Yp1 + 5, 64, 64, 0, _AT_NOTICE_},  //YpOK1-4
            { Xp1+5, Yp1 + 5, 64, 64, 194, _EUROCODE_},
            { Xp1+5+32, Yp1 + 5, 64, 64, 195, _ASCE_},
            { Xp1+5+64, Yp1 + 5, 64, 64, 196, _ICC_},
            { Xp1+5+64, Yp1 + 5, 64, 64, 197, _COMBINATION_},
            { Xp1+5+64, Yp1 + 5, 64, 64, 201, _GEOM_STIFFNESS_},
            { Xp1+5+64, Yp1 + 5, 64, 64, 205, _INERTIA_},
            { Xp1+5+64, Yp1 + 5 - 30, 64, 64, 204, _VIBRATIONS_},
            { Xp1+5+94, Yp1 + 5 - 30, 64, 64, 210, _PINNABLE_},
};

static LISTBOX listbox_as [2] =
        {
                {
                        0, 0, 0, 0,    /*wspolrzedne*/
                        8, 8, 8, 0, 0, 0, 3,
                        0 /*COLOR_NULL*/, COLOR_NULL, 15 /*COLOR_NULL*/,
                        1, NULL, combination_txt, ID_COMBINATION, 0
                },

                {
                        0, 0, 0, 0,    /*wspolrzedne*/
                        8, 8, 8, 0, 0, 0, 3,
                        0 /*COLOR_NULL*/, COLOR_NULL, 15 /*COLOR_NULL*/,
                        1, NULL, modes_number_txt, ID_MODES_NUMBER, 0
                },
        };

static COMBOBOX combobox_as [2] =
        {
                { 0, YpESCAPE1,     12, 8, &listbox_as [0]},
                { 0, YpESCAPE1 - 20, 12 , 8, &listbox_as [1]},
        } ;


static TDIALOG asking_dlg=
{
	XpBox1, YpBox1, DXBox1+10, DYBox1+14,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0,  0,0, //XpBox1+(DXBox1/2),YpBox1+(DYBox1/2),
	notice,
	0, NULL, //&line_d_l,
	2, (LABEL(*)[])&lab,
	1, (GROUP_BOX(*)[])&gr_box,
	0, NULL, //&edit,
	1, (IMAGE(*)[])&images_a,
	2, (BUTTON(*)[])&buttons,
	0, NULL, /*listbox*/
	0,NULL,
    0,NULL, //Sliders
	NULL,
	NULL,
	0,
	0,
    NULL,
};

static TDIALOG asking_dlg_static=
        {
                XpBox1, YpBox1, DXBox1+10, DYBox2+14,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0,  0,0, //XpBox1+(DXBox1/2),YpBox1+(DYBox1/2),
                notice,
                0, NULL, //&line_d_l,
                2, (LABEL(*)[])&lab,
                1, (GROUP_BOX(*)[])&gr_box,
                0, NULL, //&edit,
                9, (IMAGE(*)[])&images_as,
                9, (BUTTON(*)[])&buttons,
                0, NULL, /*listbox*/
                2, (COMBOBOX(*)[])&combobox_as,
                0,NULL, //Sliders
                NULL,
                NULL,
                0,
                0,
                NULL,
        };

char installation[32] = _INSTALATION_;

static TDIALOG asking1_dlg=
{
	XpBox1, YpBox1, DXBox1+10, DYBox1+10,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0, XpBox1+(DXBox1/2),YpBox1+(DYBox1/2),
	installation,
	0, NULL, //&line_d_l,
	2, (LABEL(*)[])&lab,
	1, (GROUP_BOX(*)[])&gr_box,
	0, NULL, //&edit,
	0, NULL,
	2, (BUTTON(*)[])&buttons,
    0,NULL, //Sliders
	0, NULL,
	0,NULL,
	NULL,
    NULL,
    0,
    0,
    NULL,
};

static int proc_dlg_asking( int n)
/*------------------------------*/
{
  int ret = Dlg_Ret_Val_Cancel;

  switch (n)
  {
    case ID_OK:
    	ret = Dlg_Ret_Val_OK;
      break;
    case ID_CANCEL:
      ret = Dlg_Ret_Val_Cancel;
      break;
	case ID_ESCAPE:
      ret = Dlg_Ret_Val_Return;
      break;
    default:
      //ret = Dlg_Ret_Val_Cancel;
      ret = Dlg_Ret_Val_Continue;
      break;
  }
  return ret;
};


static int proc_dlg_asking_static( int n)
/*------------------------------*/
{
    int ret = Dlg_Ret_Val_Cancel;

    switch (n)
    {
        case ID_EUROCODE:
        case ID_ASCE:
        case ID_ICC:
            Check_Radio_Button ( &asking_dlg_static, ID_EUROCODE, ID_ICC, n, TRUE);
            ret = Dlg_Ret_Val_Continue;
            break;
        case ID_OK:
            ret = Dlg_Ret_Val_OK;
            break;
        case ID_CANCEL:
            ret = Dlg_Ret_Val_Cancel;
            break;
        case ID_ESCAPE:
            ret = Dlg_Ret_Val_Return;
            break;
        case ID_COMBINATION:
        case ID_GEOM_STIFFNESS:
        case ID_PINNABLE:
            ret = Dlg_Ret_Val_Continue;
            break;
        default:
            //ret = Dlg_Ret_Val_Cancel;
            ret = Dlg_Ret_Val_Continue;
            break;
    }
    return ret;
};


BOOL yes_or_no(char *question)
{
	int ret;

	ret = ask_question(2, _No_, _Yes_, "", question, 12, "", 11, 1, 63);
	//1 ok; 0 - rezygnuj; 2 - Powrot
	if (ret == 1) return TRUE;
	else if (ret == 2) return FALSE;
	else return FALSE;
}



int ask_question (int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien, int image)
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
{ int Ret_Val;
  char sk[MAXPATH]="";
  char st[MAXPATH]="";
  int DXBox2, DXBox1_;
  char string_null[2]="";
  double wspscrx;
  long image_s;
  int dx_win_ask, dy_win_ask, x_win_ask, y_win_ask;
  double wsp_x, wsp_y;
  double dt,dt1,dt2;
  static int curr_h, curr_v;
  BOOL but_changed=FALSE;

  asking_dlg.SizeLabT=2;
  asking_dlg.dy=DYBox1+14;
  (*asking_dlg.Groups)[0].dy=DYBox1;
  (*asking_dlg.Buttons)[0].y=(*asking_dlg.Buttons)[1].y=(*asking_dlg.Buttons)[2].y=YpOK1;

  dt1 = (double)my_text_length(font, comment_string);
  dt2 = (double)my_text_length(font, comment1_string);

  if (dt2 > dt1) dt = dt2;
  else dt = dt1;

#define WIDTH_DISPLAY 640
#define HEIGHT_DISPLAY 348

  wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
  wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

  DXBox1_ = (dt + 170) / wsp_x;

  if (DXBox1_ < DXBox1) DXBox1_ = DXBox1;
  
  asking_dlg.dx = DXBox1_+10;
  gr_box->dx = DXBox1_;

  DXBox2 = (DXBox1_ / 2);

  asking_dlg.Images= (IMAGE(*)[]) &images_a;

  asking_dlg.SizeButtonT=n_buttons;

  switch (n_buttons)
  {
  case 1:
     buttons[0].x=DXBox1_/2 - (DXBut1/2) + DXShift + Xp1;
	 buttons[0].name2 = 100;
     buttons[0].flags &= ~BUTTON_HIDDEN;
  break;
  case 2:
	 buttons[0].x = DXBox1_ * 6 / 16 - (DXBut1 / 2) + DXShift + Xp1;
	 buttons[0].name2 = 100;
     buttons[0].flags &= ~BUTTON_HIDDEN;
	 buttons[1].x = DXBox1_ * 10 / 16 - (DXBut1 / 2) + DXShift + Xp1;
	 buttons[1].name2 = 101;
     buttons[1].flags &= ~BUTTON_HIDDEN;

     if (strcmp(esc_string,"Log")==0)
     {
         buttons[0].y -= 3;
         buttons[1].y -= 3;
         buttons[0].dy += 3;
         buttons[1].dy += 3;
         buttons[1].name2 = 211;
         but_changed=TRUE;
     }
  break;
  case 3:
     buttons[0].x=DXBox1_ * 7 / 24 - (DXBut1/2) + DXShift + Xp1;
	 buttons[0].name2 = 100;
     buttons[0].flags &= ~BUTTON_HIDDEN;
     buttons[1].x=DXBox1_ * 12 / 24 - (DXBut1/2) + DXShift + Xp1;
	 buttons[1].name2 = 102;
     buttons[1].flags &= ~BUTTON_HIDDEN;
	 buttons[2].x=DXBox1_ * 17 / 24 - (DXBut1/2) + DXShift + Xp1;
	 buttons[2].name2 = 101;
     buttons[2].flags &= ~BUTTON_HIDDEN;
  break;
  default:
  break;
  
  }

  dx_win_ask = getmaxx();
  dy_win_ask = getmaxy();

  asking_dlg.x = 1 + (dx_win_ask - asking_dlg.dx*wsp_x) / (2.0*wsp_x);
  asking_dlg.y = 1 + (dy_win_ask - asking_dlg.dy*wsp_y) / (2.0*wsp_y) - HEIGHT/2;

  asking_dlg.flags = 0x40;

  lab[0].txt=comment_string;
  lab[1].txt=comment1_string;

  if (cien==1)
   {
    lab[0].shadow=1;
    lab[0].s_ink=8;
    lab[1].shadow=1;
    lab[1].s_ink=8;
   }

  wspscrx = 638;
  wspscrx /= getmaxx();

  
  lab[0].ink=color_comment;

  lab[0].x = DXBox2 - dt1 / 2 / wsp_x +DXShift;
 
  lab[1].ink = color1_comment;

  lab[1].x = DXBox2 - dt2 / 2 / wsp_x +DXShift;

  Get_Default_Color_Dlg (&color_dlg);

  image_s=imagesize(125, 200, 675, 400);

  x_win_ask = asking_dlg.x*wsp_x;
  y_win_ask = 26 + asking_dlg.y*wsp_y;

  dx_win_ask = 10 + asking_dlg.dx*wsp_x;
  dy_win_ask = 58 /*52*/ + asking_dlg.dy*wsp_y;

  images_a[0].iconno = image;

  Save_Update_flex(0, &curr_h, &curr_v);
  Ret_Val = Dialog(&asking_dlg, &color_dlg, proc_dlg_asking, bMouse);

  if (but_changed==TRUE)
  {
      buttons[0].y += 3;
      buttons[1].y += 3;
      buttons[0].dy -= 3;
      buttons[1].dy -= 3;
  }

  if (strcmp(cont_string, "Upgrade") == 0)
  {
      FreeMouse();
      LockMouse();
  }
  Save_Update_flex(1, &curr_h, &curr_v);

  if (Ret_Val == Dlg_Ret_Val_OK) return 1;  //TAK
    else if (Ret_Val == Dlg_Ret_Val_Cancel) return 0;  //ESC
	  else if (Ret_Val == Dlg_Ret_Val_Return) return 2;  //NIE
	    else return 0;

}

int ask_question_static (int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien, int image, int *combination, int *geometric_stiffness, int *inertia, int *st_dynamic_no, BOOL *PINNABLE,
                         int *theta_, int *sigma_eq_, int *epsilon_)
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
{ int Ret_Val;
    char sk[MAXPATH]="";
    char st[MAXPATH]="";
    int DXBox2, DXBox1_;
    char string_null[2]="";
    double wspscrx;
    long image_s;
    int dx_win_ask, dy_win_ask, x_win_ask, y_win_ask;
    double wsp_x, wsp_y;
    double dt,dt1,dt2;
    static int curr_h, curr_v;
    int del, del1;

    asking_dlg_static.SizeLabT=2;
    asking_dlg_static.dy=DYBox2+14;
    (*asking_dlg.Groups)[0].dy=DYBox2;
    (*asking_dlg.Buttons)[0].y=(*asking_dlg.Buttons)[1].y=(*asking_dlg.Buttons)[2].y=YpOK1;

    dt1 = (double)my_text_length(font, comment_string);
    dt2 = (double)my_text_length(font, comment1_string);

    if (dt2 > dt1) dt = dt2;
    else dt = dt1;

#define WIDTH_DISPLAY 640
#define HEIGHT_DISPLAY 348

    wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
    wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

    DXBox1_ = (dt + 170) / wsp_x;

    if (DXBox1_ < DXBox1as) DXBox1_ = DXBox1as;

    asking_dlg_static.dx = DXBox1_+40; //10
    gr_box->dx = DXBox1_+30;  //+0

    DXBox2 = (DXBox1_ / 2);

    asking_dlg_static.Images= (IMAGE(*)[]) &images_as;

    asking_dlg_static.SizeButtonT=n_buttons;

    switch (n_buttons)
    {
        case 1:
            buttons[0].x=DXBox1_/2 - (DXBut1/2) + DXShift + Xp1;
            buttons[0].name2 = 100;
            buttons[0].flags &= ~BUTTON_HIDDEN;
            break;
        case 2:
            buttons[0].x = DXBox1_ * 6 / 16 - (DXBut1 / 2) + DXShift + Xp1;
            buttons[0].name2 = 100;
            buttons[0].flags &= ~BUTTON_HIDDEN;
            buttons[1].x = DXBox1_ * 10 / 16 - (DXBut1 / 2) + DXShift + Xp1;
            buttons[1].name2 = 101;
            buttons[1].flags &= ~BUTTON_HIDDEN;
            break;
        case 3:
            buttons[0].x=DXBox1_ * 7 / 24 - (DXBut1/2) + DXShift + Xp1;
            buttons[0].name2 = 100;
            buttons[0].flags &= ~BUTTON_HIDDEN;
            buttons[1].x=DXBox1_ * 12 / 24 - (DXBut1/2) + DXShift + Xp1;
            buttons[1].name2 = 102;
            buttons[1].flags &= ~BUTTON_HIDDEN;
            buttons[2].x=DXBox1_ * 17 / 24 - (DXBut1/2) + DXShift + Xp1;
            buttons[2].name2 = 101;
            buttons[2].flags &= ~BUTTON_HIDDEN;
            break;

        case 5:
            ////TEMPORARY
            /*
            buttons[3].flags |= BUTTON_HIDDEN;
            buttons[4].flags |= BUTTON_HIDDEN;
            buttons[5].flags |= BUTTON_HIDDEN;
            */

            buttons[3].flags &= ~BUTTON_HIDDEN;
            buttons[4].flags &= ~BUTTON_HIDDEN;
            buttons[5].flags &= ~BUTTON_HIDDEN;

            buttons[6].flags |= BUTTON_HIDDEN;
            buttons[7].flags |= BUTTON_HIDDEN;
            buttons[8].flags |= BUTTON_HIDDEN;

            buttons[9].flags &= ~BUTTON_HIDDEN;
            //buttons[10].flags &= ~BUTTON_HIDDEN;
            buttons[10].flags |= BUTTON_HIDDEN;
            buttons[11].flags &= ~BUTTON_HIDDEN;
            ///////////

            asking_dlg_static.SizeButtonT=12; //6;
            asking_dlg_static.SizeImageT=5;
            asking_dlg_static.SizeComboBoxT=1;

            buttons[0].x = DXBox1_/2 - (DXBut1) + DXShift + Xp1 - 3;
            buttons[0].name2 = 100;
            buttons[0].flags &= ~BUTTON_HIDDEN;
            buttons[1].x = DXBox1_/2 + DXShift + Xp1 + 3;
            buttons[1].name2 = 101;
            buttons[1].flags &= ~BUTTON_HIDDEN;

            buttons[2].flags |= BUTTON_HIDDEN;

            images_as[1].y1=images_as[2].y1=images_as[3].y1=images_as[4].y1=images_as[5].y1=images_as[6].y1=buttons[0].y + (int)((float)(HEIGHT-18)/wsp_y);

            images_as[7].y1=images_as[8].y1=images_as[6].y1-20;


            combobox_as [0].y=images_as[4].y1+HEIGHT/(2*wsp_y);
            combobox_as [1].y=images_as[7].y1+HEIGHT/(wsp_y);


            buttons[3].y=buttons[4].y=buttons[5].y=buttons[6].y=buttons[7].y=buttons[0].y-15;
            buttons[3].x=images_as[1].x1+13/wsp_x;
            buttons[4].x=images_as[2].x1+13/wsp_x;
            buttons[5].x=images_as[3].x1+13/wsp_x;
            images_as[4].x1=images_as[7].x1=DXBox1_-25;
            //images_as[8].x1=DXBox1_+5;
            combobox_as [0].x=DXBox1_-42;
            combobox_as [1].x=DXBox1_-42;

            del=(WIDTH-7)*2;
            if (del>4) del=4;

            del1=WIDTH-9;
            if (del1>0) del1=0;

            /*
            images_as[5].x1=buttons[1].x+DXBut1 + WIDTH/4 + del; // - abs(WIDTH-12)/wsp_x;
            buttons[6].x=images_as[5].x1+15/wsp_x;
            buttons[6].check=*geometric_stiffness;

            images_as[6].x1=buttons[1].x+DXBut1 + WIDTH/4+18 + del; // - abs(WIDTH-12)/wsp_x;
            buttons[7].x=images_as[6].x1+15/wsp_x-del1;
            buttons[7].check=*inertia;

            buttons[8].x=images_as[8].x1+15/wsp_x-del1;
            buttons[8].y=images_as[8].y1+40/wsp_y-del1;
            buttons[8].check=!(*PINNABLE);
            */

            buttons[9].x=buttons[1].x+DXBut1 + WIDTH/4+88 + del + 15/wsp_x-del1;
            buttons[9].y=buttons[0].y + (int)((float)(HEIGHT-18)/wsp_y - 20);
            buttons[9].check=*theta_;

            /*
            buttons[10].x=buttons[1].x+DXBut1 + WIDTH/4+88 + del + 15/wsp_x-del1;
            buttons[10].y=buttons[0].y + (int)((float)(HEIGHT-18)/wsp_y - 8);
            buttons[10].check=*sigma_eq_;
            */

            buttons[11].x=buttons[1].x+DXBut1 + WIDTH/4+88 + del + 15/wsp_x-del1;
            buttons[11].y=buttons[0].y + (int)((float)(HEIGHT-18)/wsp_y + 4);
            buttons[11].check=*epsilon_;

            break;

        case 6:
            ////TEMPORARY
            /*
            buttons[3].flags |= BUTTON_HIDDEN;
            buttons[4].flags |= BUTTON_HIDDEN;
            buttons[5].flags |= BUTTON_HIDDEN;
            */

            buttons[3].flags &= ~BUTTON_HIDDEN;
            buttons[4].flags &= ~BUTTON_HIDDEN;
            buttons[5].flags &= ~BUTTON_HIDDEN;

            buttons[6].flags |= BUTTON_HIDDEN;
            buttons[7].flags |= BUTTON_HIDDEN;
            buttons[8].flags |= BUTTON_HIDDEN;

            buttons[9].flags &= ~BUTTON_HIDDEN;
            buttons[10].flags &= ~BUTTON_HIDDEN;
            buttons[11].flags &= ~BUTTON_HIDDEN;
            ///////////

            asking_dlg_static.SizeButtonT=12; //6;
            asking_dlg_static.SizeImageT=5;
            asking_dlg_static.SizeComboBoxT=1;

            buttons[0].x = DXBox1_/2 - (DXBut1) + DXShift + Xp1 - 3;
            buttons[0].name2 = 100;
            buttons[0].flags &= ~BUTTON_HIDDEN;
            buttons[1].x = DXBox1_/2 + DXShift + Xp1 + 3;
            buttons[1].name2 = 101;
            buttons[1].flags &= ~BUTTON_HIDDEN;

            buttons[2].flags |= BUTTON_HIDDEN;

            images_as[1].y1=images_as[2].y1=images_as[3].y1=images_as[4].y1=images_as[5].y1=images_as[6].y1=buttons[0].y + (int)((float)(HEIGHT-18)/wsp_y);

            images_as[7].y1=images_as[8].y1=images_as[6].y1-20;


            combobox_as [0].y=images_as[4].y1+HEIGHT/(2*wsp_y);
            combobox_as [1].y=images_as[7].y1+HEIGHT/(wsp_y);


            buttons[3].y=buttons[4].y=buttons[5].y=buttons[6].y=buttons[7].y=buttons[0].y-15;
            buttons[3].x=images_as[1].x1+13/wsp_x;
            buttons[4].x=images_as[2].x1+13/wsp_x;
            buttons[5].x=images_as[3].x1+13/wsp_x;
            images_as[4].x1=images_as[7].x1=DXBox1_-25;
            //images_as[8].x1=DXBox1_+5;
            combobox_as [0].x=DXBox1_-42;
            combobox_as [1].x=DXBox1_-42;

            del=(WIDTH-7)*2;
            if (del>4) del=4;

            del1=WIDTH-9;
            if (del1>0) del1=0;

            /*
            images_as[5].x1=buttons[1].x+DXBut1 + WIDTH/4 + del; // - abs(WIDTH-12)/wsp_x;
            buttons[6].x=images_as[5].x1+15/wsp_x;
            buttons[6].check=*geometric_stiffness;

            images_as[6].x1=buttons[1].x+DXBut1 + WIDTH/4+18 + del; // - abs(WIDTH-12)/wsp_x;
            buttons[7].x=images_as[6].x1+15/wsp_x-del1;
            buttons[7].check=*inertia;

            buttons[8].x=images_as[8].x1+15/wsp_x-del1;
            buttons[8].y=images_as[8].y1+40/wsp_y-del1;
            buttons[8].check=!(*PINNABLE);
            */

            buttons[9].x=buttons[1].x+DXBut1 + WIDTH/4+88 + del + 15/wsp_x-del1;
            buttons[9].y=buttons[0].y + (int)((float)(HEIGHT-18)/wsp_y - 20);
            buttons[9].check=*theta_;

            buttons[10].x=buttons[1].x+DXBut1 + WIDTH/4+88 + del + 15/wsp_x-del1;
            buttons[10].y=buttons[0].y + (int)((float)(HEIGHT-18)/wsp_y - 8);
            buttons[10].check=*sigma_eq_;

            buttons[11].x=buttons[1].x+DXBut1 + WIDTH/4+88 + del + 15/wsp_x-del1;
            buttons[11].y=buttons[0].y + (int)((float)(HEIGHT-18)/wsp_y + 4);
            buttons[11].check=*epsilon_;

            break;
        case 9:

            ////TEMPORARY
            buttons[3].flags &= ~BUTTON_HIDDEN;
            buttons[4].flags &= ~BUTTON_HIDDEN;
            buttons[5].flags &= ~BUTTON_HIDDEN;

            buttons[6].flags &= ~BUTTON_HIDDEN;
            buttons[7].flags &= ~BUTTON_HIDDEN;
            buttons[8].flags &= ~BUTTON_HIDDEN;

            buttons[9].flags |= BUTTON_HIDDEN;
            buttons[10].flags |= BUTTON_HIDDEN;
            buttons[11].flags |= BUTTON_HIDDEN;
            ///////////

            asking_dlg_static.SizeButtonT=9;
            asking_dlg_static.SizeImageT=9;
            asking_dlg_static.SizeComboBoxT=2;

            buttons[0].x = DXBox1_/2 - (DXBut1) + DXShift + Xp1 - 3;
            buttons[0].name2 = 100;
            buttons[0].flags &= ~BUTTON_HIDDEN;
            buttons[1].x = DXBox1_/2 + DXShift + Xp1 + 3;
            buttons[1].name2 = 101;
            buttons[1].flags &= ~BUTTON_HIDDEN;

            buttons[2].flags |= BUTTON_HIDDEN;

            images_as[1].y1=images_as[2].y1=images_as[3].y1=images_as[4].y1=images_as[5].y1=images_as[6].y1=buttons[0].y + (int)((float)(HEIGHT-18)/wsp_y);

            images_as[7].y1=images_as[8].y1=images_as[6].y1-20;


            combobox_as [0].y=images_as[4].y1+HEIGHT/(2*wsp_y);
            combobox_as [1].y=images_as[7].y1+HEIGHT/(wsp_y);


            buttons[3].y=buttons[4].y=buttons[5].y=buttons[6].y=buttons[7].y=buttons[0].y-15;
            buttons[3].x=images_as[1].x1+13/wsp_x;
            buttons[4].x=images_as[2].x1+13/wsp_x;
            buttons[5].x=images_as[3].x1+13/wsp_x;
            images_as[4].x1=images_as[7].x1=DXBox1_-25;
            images_as[8].x1=DXBox1_+5;
            combobox_as [0].x=DXBox1_-42;
            combobox_as [1].x=DXBox1_-42;

            del=(WIDTH-7)*2;
            if (del>4) del=4;

            del1=WIDTH-9;
            if (del1>0) del1=0;
            images_as[5].x1=buttons[1].x+DXBut1 + WIDTH/4 + del; // - abs(WIDTH-12)/wsp_x;
            buttons[6].x=images_as[5].x1+15/wsp_x;
            buttons[6].check=*geometric_stiffness;

            images_as[6].x1=buttons[1].x+DXBut1 + WIDTH/4+18 + del; // - abs(WIDTH-12)/wsp_x;
            buttons[7].x=images_as[6].x1+15/wsp_x-del1;
            buttons[7].check=*inertia;

            buttons[8].x=images_as[8].x1+15/wsp_x-del1;
            buttons[8].y=images_as[8].y1+40/wsp_y-del1;
            buttons[8].check=!(*PINNABLE);

            break;
        default:
            break;

    }

    dx_win_ask = getmaxx();
    dy_win_ask = getmaxy();

    asking_dlg_static.x = 1 + (dx_win_ask - asking_dlg_static.dx*wsp_x) / (2.0*wsp_x);
    asking_dlg_static.y = 1 + (dy_win_ask - asking_dlg_static.dy*wsp_y) / (2.0*wsp_y) - HEIGHT/2;

    asking_dlg_static.flags = 0x40;

    lab[0].txt=comment_string;
    lab[1].txt=comment1_string;

    if (cien==1)
    {
        lab[0].shadow=1;
        lab[0].s_ink=8;
        lab[1].shadow=1;
        lab[1].s_ink=8;
    }

    wspscrx = 638;
    wspscrx /= getmaxx();


    lab[0].ink=color_comment;

    lab[0].x = DXBox2 - (int)(dt1 / 2. / wsp_x +DXShift);

    lab[1].ink = color1_comment;

    lab[1].x = DXBox2 - (int)(dt2 / 2. / wsp_x +DXShift);

    Get_Default_Color_Dlg (&color_dlg);

    image_s=imagesize(125, 200, 675, 400);

    x_win_ask = (int)(asking_dlg_static.x*wsp_x);
    y_win_ask = (int)(26 + asking_dlg_static.y*wsp_y);

    dx_win_ask = (int)(10 + asking_dlg_static.dx*wsp_x);
    dy_win_ask = (int)(58 /*52*/ + asking_dlg_static.dy*wsp_y);

    images_as[0].iconno = image;

    Save_Update_flex(0, &curr_h, &curr_v);
    Ret_Val = Dialog(&asking_dlg_static, &color_dlg, proc_dlg_asking_static, bMouse);
    Save_Update_flex(1, &curr_h, &curr_v);

    if (Ret_Val == Dlg_Ret_Val_OK)
    {
        *geometric_stiffness=Get_Check_Button(&asking_dlg_static, ID_GEOM_STIFFNESS);
        *inertia=Get_Check_Button(&asking_dlg_static, ID_INERTIA);
        *PINNABLE=!Get_Check_Button(&asking_dlg_static, ID_PINNABLE);
        *theta_=Get_Check_Button(&asking_dlg_static, ID_THETA);
        *sigma_eq_=Get_Check_Button(&asking_dlg_static, ID_SIGMA_EQ);
        *epsilon_=Get_Check_Button(&asking_dlg_static, ID_EPSILON);
        *combination= listbox_as [0].poz + listbox_as [0].foff + 1;
        *st_dynamic_no= listbox_as [1].poz + listbox_as [1].foff;
        if (Get_Check_Button(&asking_dlg_static, ID_EUROCODE))
            return 1;  //EUROCODE
        else if (Get_Check_Button(&asking_dlg_static, ID_ASCE))
            return 2;  //ASCE
        else if (Get_Check_Button(&asking_dlg_static, ID_ICC))
            return 3;  //ICC
    }
    else if (Ret_Val == Dlg_Ret_Val_Cancel) return 0;  //ESC
    else if (Ret_Val == Dlg_Ret_Val_Return) return 0;  //NIE
    //else
    return 0;
}


int ask_question_in_dialog (int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien, int image)
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
{ int Ret_Val;
    char sk[MAXPATH]="";
    char st[MAXPATH]="";
    int DXBox2, DXBox1_;
    char string_null[2]="";
    double wspscrx;
    long image_s;
    int dx_win_ask, dy_win_ask, x_win_ask, y_win_ask;
    double wsp_x, wsp_y;
    double dt,dt0,dt1,dt2;
    static int curr_h, curr_v;
    int lab_n=2;

    asking_dlg.SizeLabT=2;
    asking_dlg.dy=DYBox1+14;
    (*asking_dlg.Groups)[0].dy=DYBox1;
    (*asking_dlg.Buttons)[0].y=(*asking_dlg.Buttons)[1].y=(*asking_dlg.Buttons)[2].y=YpOK1;

    dt0 = (double)my_text_length(font, cont_string);
    dt1 = (double)my_text_length(font, comment_string);
    dt2 = (double)my_text_length(font, comment1_string);

    if (dt0>0)
    {
        lab_n=3;
        asking_dlg.SizeLabT=3;
        asking_dlg.dy+=10; //HEIGHT+5;
        (*asking_dlg.Groups)[0].dy+=10; //HEIGHT+5;
        (*asking_dlg.Buttons)[0].y=(*asking_dlg.Buttons)[1].y=(*asking_dlg.Buttons)[2].y+=10; //HEIGHT+5;
    }

    dt=max(dt0, dt1);
    dt=max(dt, dt2);

#define WIDTH_DISPLAY 640
#define HEIGHT_DISPLAY 348

    wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
    wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

    DXBox1_ = (dt + 170) / wsp_x;

    if (DXBox1_ < DXBox1) DXBox1_ = DXBox1;

    asking_dlg.dx = DXBox1_+10;
    gr_box->dx = DXBox1_;

    DXBox2 = (DXBox1_ / 2);

    asking_dlg.Images= (IMAGE(*)[]) &images_a;

    asking_dlg.SizeButtonT=n_buttons;

    switch (n_buttons)
    {
        case 1:
            buttons[0].x=DXBox1_/2 - (DXBut1/2) + DXShift + Xp1;
            buttons[0].name2 = 100;
            buttons[0].flags &= ~BUTTON_HIDDEN;
            break;
        case 2:
            buttons[0].x = DXBox1_ * 6 / 16 - (DXBut1 / 2) + DXShift + Xp1;
            buttons[0].name2 = 100;
            buttons[1].flags &= ~BUTTON_HIDDEN;
            buttons[1].x = DXBox1_ * 10 / 16 - (DXBut1 / 2) + DXShift + Xp1;
            buttons[1].name2 = 101;
            buttons[1].flags &= ~BUTTON_HIDDEN;
            break;
        case 3:
            buttons[0].x=DXBox1_ * 7 / 24 - (DXBut1/2) + DXShift + Xp1;
            buttons[0].name2 = 100;
            buttons[0].flags &= ~BUTTON_HIDDEN;
            buttons[1].x=DXBox1_ * 12 / 24 - (DXBut1/2) + DXShift + Xp1;
            buttons[1].name2 = 102;
            buttons[1].flags &= ~BUTTON_HIDDEN;
            buttons[2].x=DXBox1_ * 17 / 24 - (DXBut1/2) + DXShift + Xp1;
            buttons[2].name2 = 101;
            buttons[2].flags &= ~BUTTON_HIDDEN;
            break;
        default:
            break;

    }

    dx_win_ask = getmaxx();
    dy_win_ask = getmaxy();

    asking_dlg.x = 1 + (dx_win_ask - asking_dlg.dx*wsp_x) / (2.0*wsp_x);
    asking_dlg.y = 1 + (dy_win_ask - asking_dlg.dy*wsp_y) / (2.0*wsp_y) - HEIGHT/2;

    asking_dlg.flags = 0x40;

    if (lab_n==3) {
        lab[0].txt = cont_string;
        lab[1].txt = comment_string;
        lab[2].txt = comment1_string;
    }
    else
    {
        lab[0].txt = comment_string;
        lab[1].txt = comment1_string;
        lab[2].txt = cont_string;
    }

    if (cien==1)
    {
        lab[0].shadow=1;
        lab[0].s_ink=8;
        lab[1].shadow=1;
        lab[1].s_ink=8;
        lab[2].shadow=1;
        lab[2].s_ink=8;
    }

    wspscrx = 638;
    wspscrx /= getmaxx();

    lab[0].ink=color_comment;

    lab[0].x = DXBox2 - (lab_n==2 ? dt1:dt0) / 2 / wsp_x +DXShift;

    lab[1].ink = color1_comment;

    lab[1].x = DXBox2 - (lab_n==2 ? dt2:dt1) / 2 / wsp_x +DXShift;

    lab[2].ink = color1_comment;

    lab[2].x = DXBox2 - dt2 / 2 / wsp_x +DXShift;

    Get_Default_Color_Dlg (&color_dlg);

    image_s=imagesize(125, 200, 675, 400);

    x_win_ask = asking_dlg.x*wsp_x;
    y_win_ask = 26 + asking_dlg.y*wsp_y;

    dx_win_ask = 10 + asking_dlg.dx*wsp_x;
    dy_win_ask = 58 /*52*/ + asking_dlg.dy*wsp_y;

    images_a[0].iconno = image;

    Ret_Val = Dialog_in_dialog(&asking_dlg);

    return Ret_Val;

}


static int proc_dlg_empty(int n)
/*------------------------------*/
{
	int ret = Dlg_Ret_Val_Cancel;

	switch (n)
	{
	case ID_OK:
		ret = Dlg_Ret_Val_OK;
		break;
	case ID_CANCEL:
		ret = Dlg_Ret_Val_Cancel;
		break;
	case ID_ESCAPE:
		ret = Dlg_Ret_Val_Return;
		break;
	default:
		ret = Dlg_Ret_Val_Cancel;
		break;
	}
	return ret;
};

int empty_dlg()
/*---------------*/
{
	int Ret_Val;
	char string_null[2] = "";
	long image_s;
	int dx_win_ask, dy_win_ask, x_win_ask, y_win_ask; // x_win, y_win; // x1_win, y1_win;
	double wsp_x, wsp_y;
    static int curr_h, curr_v;

	static TDIALOG empty_dlg =
	{
		5, 5, 10, 10,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0,  0,0, 
		"^",
		0, NULL, 
		0, NULL,
		0, NULL,
		0, NULL,
		0, NULL,
		0, NULL,
		0, NULL,
		0,NULL,
        0,NULL, //Sliders
		NULL,
		NULL,
		0,
		0,
        NULL,
	};

	wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
	wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

	asking_dlg.Images = (IMAGE(*)[]) &images_a;

	asking_dlg.SizeButtonT = 0;

	Get_Default_Color_Dlg(&color_dlg);
	
	image_s = imagesize(5, 5, 15, 15);

	x_win_ask = empty_dlg.x*wsp_x;
	y_win_ask = 26 + empty_dlg.y*wsp_y;

	dx_win_ask = 10 + empty_dlg.dx*wsp_x;
	dy_win_ask = 54 /*52*/ + empty_dlg.dy*wsp_y;

	Ret_Val = Dialog(&empty_dlg, &color_dlg, proc_dlg_empty, bMouse);

	return 0;
}

#undef __ASKING__




