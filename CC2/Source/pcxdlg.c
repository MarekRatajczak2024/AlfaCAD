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

#define __PCXDLG__
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
#include <stdlib.h>
#include <allegext.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "o_dialog.h"

#include "dialog.h"

static DLG_COLOR color_dlg;
static TDIALOG import_PCX_dlg;
extern char digits[16];
extern char numbers[16];

#define Xgroup 5
#define Ygroup 5
#define Xrow (Xgroup + 10)
#define Yrow (Ygroup + 10)
#define YrowI Yrow
#define YrowL (Yrow + 3)
#define DYrow 20
#define DXedit 80
#define ddXedit 24

#define DXunits 18
#define DXcol (DXedit + 32 + 10)
#define Xgroup1 Xgroup
#define Ygroup1 (Ygroup + 2 * DYrow + 10)
#define Xrow1 (Xgroup1 + 10)
#define Yrow1 (Ygroup1 + 10)
#define Yrow1I Yrow1 
#define Yrow1L (Yrow1 + 3)
#define Xunits Xrow + 2 * DXcol -20
#define Xunits1 Xrow1 + 2 * DXcol -20
#define DXgroup (Xrow - Xgroup + 2 * (DXcol) + DXunits)

#define XpCANCEL (Xgroup + DXgroup + 5)
#define YpCANCEL Ygroup
#define XpOK XpCANCEL
#define YpOK (Ygroup1 +  3 * DYrow  -  DYBut0 + 11)

#define Xdialog 10
#define Ydialog 10
#define DXdialog (XpOK + DXBut0 - 5)
#define DYdialog (YpOK + DYBut0)

#define ID_OK			 1
#define ID_CANCEL        2
#define ID_BACKGROUND    3
#define ID_SCALE_X       4
#define ID_SCALE_Y       5
#define ID_IMAGE_FORMAT1 6
#define ID_PRESERVE_ASPECT 7
#define ID_ON_TOP 8
#define ID_H_FLIP 9
#define ID_V_FLIP 10

extern char *image_formats[];
#define NoImageFormat 9

extern char* get_units(void);

static void resize_image(int direction);

//dialog 
char imige_file_params[] = _IMAGE_FILE_IMPORT_;

static BOOL get_PCX_dlg_control(void)
/*------------------------------------*/
{
	return TRUE;
}

static int proc_dlg_PCX_date(int n)
/*-------------------------------------*/
{
	int ret = Dlg_Ret_Val_Continue;
	

	switch (n)
	{
	case ID_OK:
		if (get_PCX_dlg_control() == TRUE)
		{
			ret = Dlg_Ret_Val_OK;
		}
		break;
	case ID_CANCEL:
		ret = Dlg_Ret_Val_Cancel;
		break;
	case ID_SCALE_X:
		resize_image(0);
		ret = Dlg_Ret_Val_Redraw;
		break;
	case ID_SCALE_Y:
		resize_image(1);
		ret = Dlg_Ret_Val_Redraw;
		break;
	case ID_PRESERVE_ASPECT:
		resize_image(0);
		ret = Dlg_Ret_Val_Redraw;
		break;
	default:
		ret = Dlg_Ret_Val_Continue;
		break;
	}
	return ret;
}

static DARK_LIGHT_LINE line_d_l[] =
{
{10, 10, 20, 10, LINE_NULL, COLOR_NULL},
};

char deg[8] = u8"[°]";

#define EDITTEXTLEN 16 //9
static char dpi_x[EDITTEXTLEN] = "100";
static char dpi_y[EDITTEXTLEN] = "100";
static char pxl_x[EDITTEXTLEN] = "";
static char pxl_y[EDITTEXTLEN] = "";
static char mm_x[EDITTEXTLEN] = "";
static char mm_y[EDITTEXTLEN] = "";
static char units_x[EDITTEXTLEN] = "";
static char units_y[EDITTEXTLEN] = "";
static char units[EDITTEXTLEN] = "";
static char scale_x[EDITTEXTLEN] = "1.0";
static char scale_y[EDITTEXTLEN] = "1.0";
static char angle[EDITTEXTLEN] = "0.0";

static LABEL lab[] =
{   
	{ Xrow + ddXedit, YrowL, 0, 0, COLOR_NULL,COLOR_NULL, dpi_x, 0, 0,},
	{ Xrow + DXcol + ddXedit, YrowL, 0, 0, COLOR_NULL,COLOR_NULL, dpi_y, 0, 0,},
	{ Xrow + ddXedit, YrowL + DYrow,  0, 0, COLOR_NULL,COLOR_NULL, pxl_x, 0,0,},
	{ Xrow + DXcol + ddXedit, YrowL + DYrow,  0, 0, COLOR_NULL,COLOR_NULL,pxl_y, 0,0,},
	{ Xrow1 + ddXedit, Yrow1L-3,  0,0, COLOR_NULL,COLOR_NULL,mm_x, 0,0,},
	{ Xrow1 + DXcol + ddXedit, Yrow1L-3, 0,0, COLOR_NULL,COLOR_NULL,mm_y, 0,0,},
	{ Xrow1 + ddXedit, Yrow1L+5,  0,0, COLOR_NULL,COLOR_NULL,units_x, 0,0,},
	{ Xrow1 + DXcol + ddXedit, Yrow1L+5, 0,0, COLOR_NULL,COLOR_NULL,units_y, 0,0,},
	
	{Xrow + 0.5*DXcol+20, YrowL, 0, 0, COLOR_NULL,COLOR_NULL,"[dpi]",0,0},
	{Xrow + 0.5*DXcol+20, YrowL + DYrow, 0, 0, COLOR_NULL,COLOR_NULL,"[pxl]",0,0},

	{Xunits1, Yrow1L-3, 0, 0, COLOR_NULL,COLOR_NULL,"[mm]",0,0},
	{Xunits1-3, Yrow1L+5, 0, 0, COLOR_NULL,COLOR_NULL,units,0,0},
	{Xunits1, Yrow1L + 2 * DYrow, 0, 0, COLOR_NULL,COLOR_NULL,deg,0,0},

};


static GROUP_BOX gr_box[] =
{
 { Xgroup, Ygroup, DXgroup-60, 2 * DYrow + 5,
	COLOR_NULL ,COLOR_NULL, header_s, 0, NULL,
  },
  { Xgroup1, Ygroup1, DXgroup, 3 * DYrow + 5,
	COLOR_NULL ,COLOR_NULL, setup_s, 0, NULL,
  },
  { XpCANCEL, YpCANCEL + DYBut0 + 10, DXBut0-10, 20,
	COLOR_NULL ,COLOR_NULL, "", 0, NULL,
  },
   { Xgroup+DXgroup - 55, Ygroup, 55, 2 * DYrow + 5,
	COLOR_NULL ,COLOR_NULL, flip_s, 0, NULL,
  },
};


static void resize_image(int direction)
{
	double size_xf, size_yf;
	int horz_res, vert_res, pxl_xi, pxl_yi;
	double scale_xf, scale_yf;
	BOOL preserve_aspect;
	char i;

	preserve_aspect = Get_Check_Button(&import_PCX_dlg, ID_PRESERVE_ASPECT);
	if (preserve_aspect)
	{
		if (direction == 0)  //X
			strcpy(&scale_y, &scale_x);
		else //Y
			strcpy(&scale_x, &scale_y);
	}


	scale_xf = atof(scale_x);
	scale_yf = atof(scale_y);
	horz_res = atoi(dpi_x);
	vert_res = atoi(dpi_y);
	pxl_xi = atoi(pxl_x);
	pxl_yi = atoi(pxl_y);

	size_xf = 25.4 / horz_res * pxl_xi * scale_xf;
	size_yf = 25.4 / vert_res * pxl_yi * scale_yf;

	sprintf(mm_x, "%.2f", size_xf);
	sprintf(mm_y, "%.2f", size_yf);

	sprintf(units, "(%s)", get_units());
	sprintf(units_x, "(%.4f)", milimetryob(size_xf));
	sprintf(units_y, "(%.4f)", milimetryob(size_yf));

}

static INPUTLINE edit[] =
{
	{ Xrow1 + ddXedit, Yrow1 + DYrow,  DXedit, 12, COLOR_NULL,COLOR_NULL,
		COLOR_NULL,COLOR_NULL,COLOR_NULL, 12,0,1, scale_x, ID_SCALE_X,&digits,
	},
	{   Xrow1 + DXcol + ddXedit, Yrow1 + DYrow,  DXedit, 12, COLOR_NULL,COLOR_NULL,
		COLOR_NULL,COLOR_NULL,COLOR_NULL, 12,0,1, scale_y, ID_SCALE_Y,&digits,
	},
	{ Xrow1 + DXcol + ddXedit, Yrow1 + 2 * DYrow,  DXedit, 12, COLOR_NULL,COLOR_NULL,
		COLOR_NULL,COLOR_NULL,COLOR_NULL, 12,0,1, angle, -1,&digits,
	},

};

static IMAGE images_p[] =
{
	{ Xrow, YrowI, 32, 32, 53,pcx_tips0},
	{ Xrow + DXcol, YrowI, 32, 32, 54,pcx_tips1},

	{ Xrow, YrowI + DYrow, 32, 32, 55,pcx_tips2},
	{ Xrow + DXcol, YrowI + DYrow, 32, 32, 56,pcx_tips3},

	{ Xrow1, Yrow1I, 32, 32, 55,pcx_tips4},
	{ Xrow1 + DXcol, Yrow1I, 32, 32, 56,pcx_tips5},

	{ Xrow1, Yrow1I + 1 * DYrow, 32, 32, 57,pcx_tips6},
	{ Xrow1 + DXcol, Yrow1I + 1 * DYrow, 32, 32, 58,pcx_tips7},

	{ Xrow1, Yrow1I + 2 * DYrow, 32, 32, 59,pcx_tips8},
	{ Xrow1 + DXcol, Yrow1I + 2 * DYrow, 32, 32, 60,pcx_tips9},

	{ Xrow1 + 0.5 * DXcol, Yrow1I + 2 * DYrow, 32, 32, 121,pcx_tips10},

	{ XpCANCEL + 18 , YpCANCEL + DYBut0 + 13, 32, 32, 122,pcx_tips11},

	{ Xrow + 1.75*DXcol, YrowI, 32, 32, 123,pcx_tips12},
	{ Xrow + 1.75*DXcol, YrowI + DYrow, 32, 32, 124,pcx_tips13},
	
};

static BUTTON buttons[] =
{
	{ XpOK, YpOK, DXBut0 - 10, DYBut0 - 6, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "" /*"OK"*/, 0, B_PUSHBUTTON, 87, 1, 0, ID_OK, 0, 0,
	},
	{ XpCANCEL, YpCANCEL, DXBut0 - 10, DYBut0 - 6, COLOR_NULL,
	  COLOR_NULL, COLOR_NULL, "" /*"ESC"*/, 0,B_PUSHBUTTON, 83, 1,0, ID_CANCEL, 0, 0,
	},
	{ Xrow1 + ddXedit , Yrow1I + 2 * DYrow - 4, 20, 20, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "", 1,B_CHECKBOX, 0, 1,0, ID_BACKGROUND, 0, 0,
	},

	{ Xrow1 + 3.5 * ddXedit , Yrow1I + 2 * DYrow - 4, 20, 20, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "", 1,B_CHECKBOX, 0, 1,0, ID_PRESERVE_ASPECT, 0, 0,
    },

	{ XpCANCEL +46 , YpCANCEL + DYBut0 +10, 20, 20, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "", 0,B_CHECKBOX, 0, 1,0, ID_ON_TOP, 0, 0,
	},

	{Xrow + 1.975 * DXcol, YrowI-3, 20, 20, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "", 0,B_CHECKBOX, 0, 1,0, ID_H_FLIP, 0, 0,
	},

	{  Xrow + 1.975 * DXcol, YrowI + DYrow-3, 20, 20, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "", 0,B_CHECKBOX, 0, 1,0, ID_V_FLIP, 0, 0,
	},
};

static LISTBOX listbox[] =
{
{
	0, 0, 0, 0,
	NoImageFormat, 0, 0, 0, 0, 5, 5,
	COLOR_NULL, COLOR_NULL, COLOR_NULL,
	1, NULL, image_formats, ID_IMAGE_FORMAT1
  }
};

static COMBOBOX combobox[] =
{

 {60, 60, 100, 24, &listbox[0]},
};

static TDIALOG import_PCX_dlg =
{
	Xdialog, Ydialog, DXdialog, DYdialog+2, COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0,0,0,
	imige_file_params,
	0 , &line_d_l,
	13, &lab,
	4 , &gr_box,
	3,  &edit,
	14, &images_p,
	7, &buttons,
	0, NULL,
	0,&combobox,
    0,NULL, //Sliders
	NULL,
	NULL,
	0,
	0,
    NULL,
};

int GetPCXParams(PCXheader head, B_PCX *b_pcx, double scale_xf, double scale_yf, double angle_f, BOOL background, BOOL preserve_aspect, BOOL on_top, BOOL h_flip, BOOL v_flip)
{
	long image_s;
	int dx_win_dlg, dy_win_dlg, x_win_dlg, y_win_dlg;
	double wsp_x, wsp_y, size_xf, size_yf;
	int Ret_Val;
	int pxl_xi, pxl_yi;
	char i;
    static int curr_h, curr_v;
	
	sprintf(dpi_x, "%d", head.horz_res);
	sprintf(dpi_y, "%d", head.vert_res);

	pxl_xi = head.xmax - head.xmin;
	pxl_yi = head.ymax - head.ymin;

	sprintf(pxl_x, "%d", pxl_xi);
	sprintf(pxl_y, "%d", pxl_yi);

	sprintf(scale_x, "%.6f", scale_xf);
	sprintf(scale_y, "%.6f", scale_yf);

	size_xf = 25.4 / head.horz_res * pxl_xi * scale_xf;
	size_yf = 25.4 / head.vert_res * pxl_yi * scale_yf;

	sprintf(mm_x, "%.4f", size_xf);
	sprintf(mm_y, "%.4f", size_yf);

	sprintf(units, "(%s)", get_units());
	sprintf(units_x, "(%.4f)", milimetryob(size_xf));
	sprintf(units_y, "(%.4f)", milimetryob(size_yf));

	sprintf(angle, "%.6f", angle_f);

	Set_Check_Button(&import_PCX_dlg, ID_BACKGROUND, background);

	Set_Check_Button(&import_PCX_dlg, ID_PRESERVE_ASPECT, preserve_aspect);

	Set_Check_Button(&import_PCX_dlg, ID_ON_TOP, on_top);
	Set_Check_Button(&import_PCX_dlg, ID_H_FLIP, h_flip);
	Set_Check_Button(&import_PCX_dlg, ID_V_FLIP, v_flip);


	Get_Default_Color_Dlg(&color_dlg);

	wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
	wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

	x_win_dlg = import_PCX_dlg.x*wsp_x;
	y_win_dlg = 26 + import_PCX_dlg.y*wsp_y;

	dx_win_dlg = 10 + import_PCX_dlg.dx*wsp_x;
	dy_win_dlg = 54 + import_PCX_dlg.dy*wsp_y;

    Save_Update_flex(0, &curr_h, &curr_v);
	Ret_Val = Dialog(&import_PCX_dlg, &color_dlg, proc_dlg_PCX_date, TRUE);
    Save_Update_flex(1, &curr_h, &curr_v);

	if (Ret_Val == Dlg_Ret_Val_OK)
	{
		scale_xf = atof(scale_x);
		scale_yf = atof(scale_y);

		b_pcx->dx = 25.4 / (double)head.horz_res * scale_xf;
		b_pcx->dy = 25.4 / (double)head.vert_res * scale_yf;

		b_pcx->kat = atof(angle) * Pi / 180.0;
		b_pcx->ignore_background = !Get_Check_Button(&import_PCX_dlg, ID_BACKGROUND);
		b_pcx->on_front = Get_Check_Button(&import_PCX_dlg, ID_ON_TOP);
		b_pcx->h_flip = Get_Check_Button(&import_PCX_dlg, ID_H_FLIP);
		b_pcx->v_flip = Get_Check_Button(&import_PCX_dlg, ID_V_FLIP);

		return 1;  //OK
	}
	else if (Ret_Val == Dlg_Ret_Val_Cancel) return 0;  //ESC
	else return 0;
}

#undef __PCXDLG__
