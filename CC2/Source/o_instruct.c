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

#define __O_INSTRUCT__
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
#define  DXBox1  250
#define  DYBox1  254 //252
#define  DXBut1  45 //50 //54 //75
#define  DYBut1  20
#define  XpOK1  Xp1 + 160
#define  YpOK1  Yp1 + 228
#define  XpSTAY  Xp1 + 7
#define  YpSTAY  Yp1 + 228

#define DXShift 102
#define DXShiftOr1 40
#define DXShiftOr2 67 //65
#define DXShiftOr3 51 //49
#define DXShiftOr4 24
#define DXShiftOr5 35 //33
#define DXShiftOr6 19 //17

#define ID_OK             1
#define ID_STAY         2

#define MaxComLen 120

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))


extern void getimage(int left, int top, int right, int bottom, void  *bitmap);
extern void putimage(int left, int top,  void  *bitmap, int op);
extern int my_text_length(FONT* font, const char* text);
extern void save_desktop_instruction(void);
extern char *qmark_p;
extern char *eymark_p;
extern char *ermark_p;
extern char *offmark_p;

static DLG_COLOR color_dlg;

int show_instruction (void);
int stay_check0, stay_check;

static LABEL lab[]={
{Xp1 + DXShift, Yp1 + 8, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[0]},
{Xp1 + DXShift, Yp1 + 22, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[1]},
{Xp1 + DXShift, Yp1 + 36, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[2]},
{Xp1 + DXShift, Yp1 + 50, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[3]},
{Xp1 + DXShift, Yp1 + 64, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[4]},
{Xp1 + DXShift, Yp1 + 78, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[5]},
{Xp1 + DXShift, Yp1 + 92, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[6]},
{Xp1 + DXShift, Yp1 + 106, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[7]},
{Xp1 + DXShift, Yp1 + 120, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[8]},
{Xp1 + DXShift, Yp1 + 134, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[9]},
{Xp1 + DXShift, Yp1 + 148, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[10]},
{Xp1 + DXShift, Yp1 + 162, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[11]},
{Xp1 + DXShift, Yp1 + 172, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[12]},
{Xp1 + DXShift, Yp1 + 182, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[13]},
{Xp1 + DXShift, Yp1 + 192, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[14]},
{Xp1 + DXShift, Yp1 + 202, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[15]},
{Xp1 + DXShift, Yp1 + 216, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[16]},

{Xp1 + DXShiftOr1, Yp1 + 8, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[17]},
{Xp1 + DXShiftOr1, Yp1 + 22, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[17]},
{Xp1 + DXShiftOr6, Yp1 + 36, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[17]},
{Xp1 + DXShiftOr2, Yp1 + 36, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[17]},
{Xp1 + DXShiftOr5, Yp1 + 50, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[17]},
{Xp1 + DXShiftOr2, Yp1 + 50, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[17]},
{Xp1 + DXShiftOr3, Yp1 + 64, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[17]},
{Xp1 + DXShiftOr1, Yp1 + 78, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[17]},

{Xp1 + DXShiftOr1, Yp1 + 106, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[17]},

{Xp1 + DXShiftOr4, Yp1 + 148, 0, 0, COLOR_NULL ,COLOR_NULL,instruct_txt[17]},
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
  { XpSTAY, YpSTAY, DXBut1, DYBut1, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, stay_string, 0,B_CHECKBOX, 0, 1,0, ID_STAY, 0, 0,
  },
};

char instruction[32] = _INSTRUCTION_;

static IMAGE images_i [] =
{
{ Xp1+5, Yp1 + 4, 32, 32, 145, _AT_INSTRUCTION_},  //Open menu / Select
{ Xp1+68, Yp1 + 4, 32, 32, 147, ""},

{ Xp1+5, Yp1 + 18, 32, 32, 146, ""},  //Close menu / Escape
{ Xp1+68, Yp1 + 18, 32, 32, 148, ""},

#ifdef MACOS
{ Xp1+5, Yp1 + 32, 32, 32, 142, ""},  //Freeing the mouse
#else
 { Xp1+5, Yp1 + 32, 32, 32, 141, ""},  //Freeing the mouse
#endif
 { Xp1+37, Yp1 + 32, 32, 32, 128, ""},
 { Xp1+53, Yp1 + 32, 32, 32, 209, ""},  //189 //151

 { Xp1+82, Yp1 + 32, 32, 32, 148, ""},  //mouse buttons
 { Xp1+92, Yp1 + 32, 32, 32, 206, ""},

 { Xp1+5, Yp1 + 46, 64, 32, 143, ""},   //Auxiliary menu
 { Xp1+53, Yp1 + 46, 32, 32, 151, ""},
 { Xp1+84, Yp1 + 46, 32, 32, 149, ""},

 { Xp1+5, Yp1 + 60, 32, 32, 128, ""},  //Auxiliary context menu
 { Xp1+21, Yp1 + 60, 32, 32, 143, ""},
 { Xp1+70, Yp1 + 60, 32, 32, 128, ""},
 { Xp1+84, Yp1 + 60, 32, 32, 149, ""},

 { Xp1+5, Yp1 + 74, 32, 32, 128, ""},  //Orthogonality on/off
 { Xp1+21, Yp1 + 74, 32, 32, 168, ""},
 { Xp1+70, Yp1 + 74, 32, 32, 138, ""},

 { Xp1+5, Yp1 + 88, 32, 32, 150, ""},  //Zooming / scrolling

 { Xp1+5, Yp1 + 102, 32, 32, 128, ""},  //Panning (Mickey Mouse hand)
 { Xp1+21, Yp1 + 102, 32, 32, 129, ""},
 { Xp1+70, Yp1 + 102, 32, 32, 128, ""},
 { Xp1+86, Yp1 + 102, 32, 32, 130, ""},

 { Xp1+5, Yp1 + 116, 32, 32, 144, ""},  //Command repetition

 { Xp1+5, Yp1 + 130, 32, 32, osnap_icon0, ""},  //Osnap choice (there are more)
 { Xp1+21, Yp1 + 130, 32, 32, osnap_icon1, ""},
 { Xp1+37, Yp1 + 130, 32, 32, osnap_icon2, ""},
 { Xp1+53, Yp1 + 130, 32, 32, osnap_icon3, ""},
{ Xp1+69, Yp1 + 130, 32, 32, osnap_icon4, ""},
{ Xp1+85, Yp1 + 130, 32, 32, osnap_icon5, ""},

 { Xp1+5, Yp1 + 144, 32, 32, 131, ""},  //Helping hand (all hotkeys)
#ifdef MACOS
{ Xp1+53, Yp1 + 144, 32, 32, 208, ""},  //empty
#else
 { Xp1+53, Yp1 + 144, 32, 32, 142, ""},
#endif

{ Xp1+5, Yp1 + 158, 32, 32, 129, ""},  //Alf shift for Greek

{ Xp1+5, Yp1 + 179, 32, 32, 128, ""},  //Sticking keys
{ Xp1+21, Yp1 + 179, 32, 32, 129, ""},
{ Xp1+37, Yp1 + 179, 32, 32, 130, ""},

{ Xp1+5, Yp1 + 214, 32, 32, 189, ""},  //tab
};

static TDIALOG instruct_dlg=
{
	XpBox1, YpBox1, DXBox1+10, DYBox1+14,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0,  0,0, //XpBox1+(DXBox1/2),YpBox1+(DYBox1/2),
	instruction,
	0, NULL, //&line_d_l,
	27, (LABEL(*)[])&lab,
	1, (GROUP_BOX(*)[])&gr_box,
	0, NULL, //&edit,
	38, (IMAGE(*)[])&images_i,
	2, (BUTTON(*)[])&buttons,
	0, NULL,
	0,NULL,
    0,NULL, //Sliders
	NULL,
	NULL,
	0,
	0,
    NULL,
};

char *Get_instruct_dlg(void)
{
    return (char*)&instruct_dlg;
}

static int proc_dlg_instruct( int n)
/*------------------------------*/
{
  int ret = Dlg_Ret_Val_Continue;

  switch (n)
  {
    case ID_OK:
    	ret = Dlg_Ret_Val_OK;
        return ret;
      break;
    case ID_STAY:
        stay_check = Get_Check_Button(&instruct_dlg, ID_STAY);
        ret = Dlg_Ret_Val_Continue;
      break;
    default:
      break;
  }
  return ret;
};


BOOL Show_Alfa_Instruction(void)
{
	int ret;

	ret = show_instruction();
	if (ret == 1) return TRUE;
	else if (ret == 2) return FALSE;
	else return FALSE;
}



int show_instruction (void)
/*------------------------*/
{ int Ret_Val;
  char sk[MAXPATH]="";
  char st[MAXPATH]="";
  int DXBox2, DXBox1_;
  char string_null[2]="";
  double wspscrx;
  long image_s;
  int dx_win_ask, dy_win_ask, x_win_ask, y_win_ask;
  double wsp_x, wsp_y;
  double dt=0,dt1,dt2;
  static int curr_h, curr_v;

  for (int i=0; i<instruct_dlg.SizeLabT; i++) {
      dt1 = (double) my_text_length(font, lab[i].txt);
      if (dt1>dt) dt=dt1;
  }

#define WIDTH_DISPLAY 640
#define HEIGHT_DISPLAY 348

  wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
  wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

  DXBox1_ = dt / wsp_x + DXShift + 8;

  if (DXBox1_ < DXBox1) DXBox1_ = DXBox1;
  
  instruct_dlg.dx = DXBox1_+10;
  gr_box->dx = DXBox1_;

  DXBox2 = (DXBox1_ / 2);

  instruct_dlg.Images= (IMAGE(*)[]) &images_i;


    buttons[0].x=DXBox1_ - DXBut1 - 2 ;
    buttons[0].name2 = 100;

  dx_win_ask = getmaxx();
  dy_win_ask = getmaxy();

  instruct_dlg.x = 1 + (dx_win_ask - instruct_dlg.dx*wsp_x) /(2.0*wsp_x);
  instruct_dlg.y = 1 + (dy_win_ask - instruct_dlg.dy*wsp_y) / (2.0*wsp_y) - HEIGHT/2;

  instruct_dlg.flags = 0x40;

  wspscrx = 638;
  wspscrx /= getmaxx();

  Get_Default_Color_Dlg (&color_dlg);

  stay_check0 = stay_check = Get_Check_Button(&instruct_dlg, ID_STAY);

  Save_Update_flex(0, &curr_h, &curr_v);
  Ret_Val = Dialog(&instruct_dlg, &color_dlg, proc_dlg_instruct, bMouse);
  Save_Update_flex(1, &curr_h, &curr_v);

  if (Ret_Val == Dlg_Ret_Val_OK)
  {
      if (stay_check!=stay_check0)
      {
          desktop_instruction=!stay_check;
          save_desktop_instruction();
      }
      return 1;  //OK
  }
	    else return 0;

}

#undef __O_INSTRUCT__




