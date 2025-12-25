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

#define __DXFDLG__
#pragma warning(disable : 4996)
#include<forwin.h>
#ifndef LINUX
#include<io.h>
#endif
#include<fcntl.h>
#include <string.h>
#ifndef LINUX
#include<dos.h>
#endif
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

#define Xgroup 5
#define Ygroup 5
#define Xrow (Xgroup + 10)
#define Yrow (Ygroup + 10)
#define YrowI Yrow
#define YrowL (Yrow + 3)
#define DYrow 20
#define DXedit 55
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
#define YpCANCEL Ygroup1 //Ygroup
#define XpOK XpCANCEL
#define YpOK (Ygroup1 +  3 * DYrow  -  DYBut0 + 13)

#define DYComboBox	12

#define Xdialog 10
#define Ydialog 10
#define DXdialog (XpOK + DXBut0 - 5)
#define DYdialog (YpOK + DYBut0)


#define ID_OK			 1
#define ID_CANCEL        2
#define ID_BACKGROUND    3
#define ID_SCALE_X       4
#define ID_SCALE_Y       5
#define ID_UNITS         6
#define ID_SCALES        7
#define ID_FORMATS       8
#define ID_EXTENTS       9
#define ID_LIMITS       10
#define ID_AUTO_SIZE    20


void PreviewDXF(void);

double extents_xf, extents_yf, limits_xf, limits_yf;

static void resize_image(void);
char new_format[32];

extern char digits[16];
extern char numbers[16];
extern void draw_dlg_lab(TDIALOG* dlg);
extern void draw_dlg_combobox(TDIALOG* dlg);

//dialog 
char drawing_file_params[] = _DFX_IMPORT_;

static char x_scale[16]={"X"};

static BOOL get_DXF_dlg_control(void)
/*------------------------------------*/
{
	return TRUE;
}

static DARK_LIGHT_LINE line_d_l[] =
{
{10, 10, 20, 10, LINE_NULL, COLOR_NULL},
};


#define EDITTEXTLEN 16
static char extents_x[EDITTEXTLEN] = "0";
static char extents_y[EDITTEXTLEN] = "0";
static char limits_x[EDITTEXTLEN] = "0";
static char limits_y[EDITTEXTLEN] = "0";
static char mm_x[EDITTEXTLEN] = "0";
static char mm_y[EDITTEXTLEN] = "0";
static char offset_x[EDITTEXTLEN] = "0";
static char offset_y[EDITTEXTLEN] = "0";

static LABEL lab[] =
{   
	{ Xrow + ddXedit, YrowL, 65, 10, COLOR_NULL,COLOR_NULL, extents_x, 0, 0,},
	{ Xrow + DXcol + ddXedit, YrowL, 65, 10, COLOR_NULL,COLOR_NULL, extents_y, 0, 0,},
	{ Xrow + ddXedit, YrowL + DYrow,  65, 10, COLOR_NULL,COLOR_NULL, limits_x, 0,0,},
	{ Xrow + DXcol + ddXedit, YrowL + DYrow,  65, 10, COLOR_NULL,COLOR_NULL,limits_y, 0,0,},
	{ Xrow1 + ddXedit, Yrow1L,  65,10, COLOR_NULL,COLOR_NULL,mm_x, 0,0,},
	{ Xrow1 + DXcol + ddXedit, Yrow1L, 65,10, COLOR_NULL,COLOR_NULL,mm_y, 0,0,},
	
	{Xunits /*-5*/, YrowL, 0, 0, COLOR_NULL,COLOR_NULL,"[U]",0,0},
	{Xunits /*-5*/, YrowL + DYrow, 0, 0, COLOR_NULL,COLOR_NULL,"[U]",0,0},

	{Xunits1, Yrow1L, 0, 0, COLOR_NULL,COLOR_NULL,"[mm]",0,0},

};

static GROUP_BOX gr_box[] =
{
 { Xgroup, Ygroup, DXgroup, 2 * DYrow + 5,
	COLOR_NULL ,COLOR_NULL, header_dxf, 0, NULL,
  },
  { Xgroup1, Ygroup1, DXgroup, 3 * DYrow + 5,
	COLOR_NULL ,COLOR_NULL, setup_dxf, 0, NULL,
  },

  { XpOK, Ygroup, DXBut0 - 10, 2 * DYrow + 5,
	COLOR_NULL ,COLOR_NULL, setup_format, 0, NULL,  /*process needs to be initialized later*/
  },
};


static INPUTLINE edit[] =
{
	{ Xrow1 + ddXedit, Yrow1 + DYrow,  DXedit, 12, COLOR_NULL,COLOR_NULL,
		COLOR_NULL,COLOR_NULL,COLOR_NULL, 8,0,1, offset_x, ID_SCALE_X,(char *)& digits,
	},
	{   Xrow1 + DXcol + ddXedit, Yrow1 + DYrow,  DXedit, 12, COLOR_NULL,COLOR_NULL,
		COLOR_NULL,COLOR_NULL,COLOR_NULL, 8,0,1, offset_y, ID_SCALE_Y,(char *)& digits
	},

};

static IMAGE images_dxf[] =
{
	{ Xrow, YrowI, 32, 32, 90,images_dxf_tips0},
	{ Xrow + DXcol, YrowI, 32, 32, 91,images_dxf_tips1},

	{ Xrow, YrowI + DYrow, 32, 32, 92,images_dxf_tips2},
	{ Xrow + DXcol, YrowI + DYrow, 32, 32, 93,images_dxf_tips3},

	{ Xrow1, Yrow1I, 32, 32, 55,images_dxf_tips4},
	{ Xrow1 + DXcol, Yrow1I, 32, 32, 56,images_dxf_tips5},

	{ Xrow1, Yrow1I + 0.975 * DYrow, 32, 32, 65,images_dxf_tips6},
	{ Xrow1 + DXcol, Yrow1I + 0.95 * DYrow, 32, 32, 67,images_dxf_tips7},

	{ Xrow1, Yrow1I + 1.95 * DYrow, 32, 32, 94,images_dxf_tips8},
	{ Xrow1 + DXcol, Yrow1I + 1.9 * DYrow, 32, 32, 29,images_dxf_tips9},

	{ XpOK+5,  YrowI, 48, 48, 95,images_dxf_tips10},
	{ XpOK + DXBut0 - 38, YrowI, 48, 48, 96,images_dxf_tips11},
	
};

static BUTTON buttons[] =
{
	{ XpOK, YpOK, DXBut0 - 10, DYBut0 - 8, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "" /*"OK"*/, 0, B_PUSHBUTTON, 87, 1, 0, ID_OK, 0, 0,
	},
	{ XpCANCEL, YpCANCEL, DXBut0 - 10, DYBut0 - 8, COLOR_NULL,
	  COLOR_NULL, COLOR_NULL, "" /*"ESC"*/, 0,B_PUSHBUTTON, 83, 1,0, ID_CANCEL, 0, 0,
	},
	{ Xunits + 22, YrowL -7, 20, 20, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "", 1,B_RADIOBUTTON, 0, 1,0, ID_EXTENTS, 0, 0,
    },
	{Xunits + 22, YrowL + DYrow -7,  20, 20, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "", 1,B_RADIOBUTTON, 0, 1,0, ID_LIMITS, 0, 0,
	},
	{ XpOK + DXBut0 - 48, YrowI, 20, 20, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "", 1,B_CHECKBOX, 0, 1,0, ID_AUTO_SIZE, 0, 0,
  },
};

#define NoUnits 8

#ifdef UKRAINIAN
static char *units[] = {
	u8"мм",
    u8"см",
    u8"м",
    u8"км",
    "\"",
    "'",
    "yd",
    "mi",
};
#else
static char *units[] = {
	"mm",
	"cm",
	"m",
    "km",
	"\"",
	"'",
    "yd",
    "mi",
};
#endif

extern const int funits_no;
extern double funits[];

#define NoScales 15

static char *scales[] = {
 "1 : 1",
 "1 : 2",
 "1 : 5",
 "1 : 10",
 "1 : 20",
 "1 : 25",
 "1 : 50",
 "1 : 100",
 "1 : 500",
 "1 : 1000",
 "1 : 5000",
 "1 : 10000",
 "2 : 1",
 "5 : 1",
 "1 : X",
};

static double fscales[15] = { 1,2,5,10,20,25,50,100,500,1000,5000,10000,0.5,0.2,1 };

#define NoFormats 46

static char *formats[] ={
 "*   xxx * yyy",
 "A4  210 * 297",
 "A4_ 297 * 210",
 "A3  420 * 297",
 "A3_ 297 * 420",
 "A2  594 * 420",
 "A2_ 420 * 594",
 "A1  841 * 594",
 "A1_ 594 * 841",
 "A0 1189 * 841",
"A0_ 841 *1189",
"a4  185 * 280",
"a4_ 280 * 185",
"a3  395 * 280",
"a2  565 * 400",
"a1  810 * 565",
"a0 1160 * 810",
"3A4 630 * 297",
"4A4 841 * 297",
"5A4 1050* 297",
"6A4 1260* 297",
"3a4 600 * 280",
"4a4  810* 280",
"5a4 1020* 280",
"6a4 1230* 280",
"B4  250 * 176",
"B4_ 176 * 250",
"B3  500 * 353",
"B3_ 353 * 500",
"B2  707 * 500",
"B2_ 500 * 707",
"B1 1000 * 707",
"B1_ 707 *1000",
"B0 1414 *1000",
"B0_ 1000*1414",
"A  279 *  215",
"A_ 215 *  279",
"B  431 *  279",
"B_ 279 *  431",
"C  559 *  432",
"C_ 432 *  559",
"D  864 *  559",
"D_ 559 *  864",
"E 1118 *  864",
"E_ 864 * 1118",
"Legal 215*355"
};

static int fformats[46][2] = FormatyAr;

static LISTBOX listbox[] =
{
{
	0, 0, 0, 0,
	NoUnits, 0, 0, 0, 0, 0, 2,
	COLOR_NULL, COLOR_NULL, COLOR_NULL,
	1, NULL, units, ID_UNITS, 0
  },
	{
	0, 0, 0, 0,
	NoScales, 15, 15, 0, 0, 0, 10,
	COLOR_NULL, COLOR_NULL, COLOR_NULL,
	1, NULL, scales, ID_SCALES, 0
  },
	{
	0, 0, 0, 0,
	NoFormats, 16, 16, 0, 0, 0, 13,
	COLOR_NULL, COLOR_NULL, COLOR_NULL,
	1, NULL, formats, ID_FORMATS, 0
  }
};

static COMBOBOX combobox[] =
{
 {Xrow1 + ddXedit, Yrow1 + 2 * DYrow, DXedit, 10, &listbox[0]},
 {Xrow1 + DXcol + ddXedit, Yrow1 + 2 * DYrow, DXedit, 10, &listbox[1]},
 {XpOK + 5,  YrowL + DYrow - 2,  DXBut0 - 20, 10, &listbox[2]},
};



static TDIALOG import_DXF_dlg =
{
	Xdialog, Ydialog, DXdialog, DYdialog, COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0,0,0,
	drawing_file_params,
	0, (DARK_LIGHT_LINE(*)[])&line_d_l,
	9, (LABEL(*)[])&lab,
	3, (GROUP_BOX(*)[]) &gr_box,
	2, (INPUTLINE(*)[])&edit,
	12, (IMAGE(*)[])&images_dxf,
	5, (BUTTON(*)[])&buttons,
	0, NULL,  //&listbox
	3, (COMBOBOX(*)[])&combobox,
    0,NULL, //Sliders
	PreviewDXF,
	NULL,
	0,
	0,
    NULL,
};

void PreviewDXF(void)
{
	double scale_f = 1.0, units_f = 1.0;
	int units_i, scale_i, format_i;
	double size_xf, size_yf;
	int format_x, format_y;
	BOOL ext_lim;
	double offset_lf, offset_bf;
	int size_xi, size_yi;

	ext_lim = Get_Check_Button(&import_DXF_dlg, ID_EXTENTS);

	units_i = listbox[0].foff + listbox[0].poz;
	units_f = funits[units_i];
	scale_i = listbox[1].foff + listbox[1].poz;
	scale_f = fscales[scale_i];

	if (ext_lim)
	{
		size_xf = extents_xf / scale_f * units_f;
		size_yf = extents_yf / scale_f * units_f;
	}
	else
	{
		size_xf = limits_xf / scale_f * units_f;
		size_yf = limits_yf / scale_f * units_f;
	}

	offset_lf = atof(edit[0].txt);
	offset_bf = atof(edit[1].txt);

	size_xf += offset_lf;
	size_yf += offset_bf;

	sprintf(mm_x, "%.2f", size_xf);
	sprintf(mm_y, "%.2f", size_yf);

	lab[0].txt = extents_x;
	lab[1].txt = extents_y;
	lab[2].txt = limits_x;
	lab[3].txt = limits_y;

	lab[4].txt = mm_x;
	lab[5].txt = mm_y;

	size_xi = (int)(size_xf + 0.5);
	if (size_xi > 99999) size_xi = 99999;
	size_yi = (int)(size_yf + 0.5);
	if (size_yi > 99999) size_yi = 99999;

	sprintf(new_format, "%d; %d", size_xi, size_yi);

	format_i = listbox[2].foff + listbox[2].poz;
	format_x = fformats[format_i][0];
	format_y = fformats[format_i][1];

	listbox[2].txt[0] = new_format;

	draw_dlg_lab(&import_DXF_dlg);
	draw_dlg_combobox(&import_DXF_dlg);
}

static void resize_image(void)
{
	double scale_f = 1.0, units_f = 1.0;
	int units_i, scale_i, format_i;
	int format_xf, format_yf;

	units_i = listbox[0].foff + listbox[0].poz;
	units_f = funits[units_i];
	scale_i = listbox[1].foff + listbox[1].poz;
	scale_f = fscales[scale_i];

	format_i = listbox[2].foff + listbox[2].poz;
	format_xf = fformats[format_i][0];
	format_yf = fformats[format_i][1];
}



static int proc_dlg_DXF_date(int n)
/*-------------------------------------*/
{
	int ret = Dlg_Ret_Val_Continue;
    int scale_pos;
    char sk[MaxTextLen]="" ;
    int retval_no = 1 ;
    double buf_ret [1] ;
    double d;
    char *pos_colon;

    switch (n)
	{
	case ID_OK:
		if (get_DXF_dlg_control() == TRUE)
		{
			ret = Dlg_Ret_Val_OK;
		}
		break;
	case ID_CANCEL:
		ret = Dlg_Ret_Val_Cancel;
		break;
	case ID_SCALE_X:
		resize_image();
		ret = Dlg_Ret_Val_Redraw;
		break;
	case ID_SCALE_Y:
		resize_image();
		ret = Dlg_Ret_Val_Redraw;
		break;
	case ID_EXTENTS:
	case ID_LIMITS:
		Check_Radio_Button(&import_DXF_dlg, ID_EXTENTS, ID_LIMITS, n == ID_EXTENTS ? ID_EXTENTS : ID_LIMITS, TRUE);
		PreviewDXF();
        ret = Dlg_Ret_Val_Continue;
        break;
    case ID_SCALES:

        scale_pos=(listbox[1].foff+listbox[1].poz);
        if (scale_pos==14)
        {
            pos_colon=strchr(listbox[1].txt[14],':');
            if (pos_colon!=NULL) strcpy(sk, pos_colon + 1);
            else strcpy(sk, listbox[1].txt[14]);

            if (!get_string (sk, "", MaxTextLen, 0, 6)) {
                ret = Dlg_Ret_Val_Continue;
                strcpy(x_scale, "1 : X");
                listbox[1].txt[14]=x_scale;
                break;
            }
            if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
            {
                ret = Dlg_Ret_Val_Continue;
                strcpy(x_scale, "1 : X");
                listbox[1].txt[14]=x_scale;
                break;
            }
            d = buf_ret [0] ;
            if ( d <= 0 )
            {
                ErrList (30);
                strcpy(x_scale, "1 : X");
                listbox[1].txt[14]=x_scale;
                ret = Dlg_Ret_Val_Continue;
                break;
            }
            sprintf(x_scale,"1 : %-6.2f",d);
            listbox[1].txt[14]=x_scale;
            fscales[14]=d;
            resize_image();
            PreviewDXF();
        }
        ret = Dlg_Ret_Val_Redraw;
        break;
        default:
		ret = Dlg_Ret_Val_Continue;
		break;
	}
	return ret;
}

int GetDXFParams(DXF_Header *head)
{
	int dx_win_dlg, dy_win_dlg, x_win_dlg, y_win_dlg;
	double wsp_x, wsp_y, size_xf, size_yf;
	int Ret_Val;
	BOOL ext_lim = 0;
	double scale_f = 1.0, units_f = 1.0;
	int units_i, scale_i, format_i;
	int size_xi, size_yi;
	double offset_lf, offset_bf;
	char *ptr;
	char format_xy[32];
	BOOL auto_size;
    static int curr_h, curr_v;
	

	Set_Check_Button(&import_DXF_dlg, ID_EXTENTS, 1);
	Set_Check_Button(&import_DXF_dlg, ID_LIMITS, 0);

	extents_xf = head->EXTMAX_X - head->EXTMIN_X;
	extents_yf = head->EXTMAX_Y - head->EXTMIN_Y;

	if (extents_xf > 999999)
	{
		extents_xf = 999999;
		sprintf(extents_x, "%d***", (int)extents_xf);
		Set_Check_Button(&import_DXF_dlg, ID_EXTENTS, 0);
		Set_Check_Button(&import_DXF_dlg, ID_LIMITS, 1);
		ext_lim = TRUE;
	}
	else sprintf(extents_x, "%.4f", extents_xf);

	if (extents_yf > 999999)
	{
		extents_yf = 999999;
		sprintf(extents_y, "%d***", (int)extents_yf);
		Set_Check_Button(&import_DXF_dlg, ID_EXTENTS, 0);
		Set_Check_Button(&import_DXF_dlg, ID_LIMITS, 1);
		ext_lim = TRUE;
	}
	else sprintf(extents_y, "%.4f", extents_yf);

	limits_xf = head->LIMMAX_X - head->LIMMIN_X;
	limits_yf = head->LIMMAX_Y - head->LIMMIN_Y;

	sprintf(limits_x, "%.4f", limits_xf);
	sprintf(limits_y, "%.4f", limits_yf);

	units_i = listbox[0].foff + listbox[0].poz;
	units_f = funits[units_i];
	scale_i = listbox[1].foff + listbox[1].poz;
	scale_f = fscales[scale_i];

	if (!ext_lim)
	{
		size_xf = extents_xf / scale_f * units_f;
		size_yf = extents_yf / scale_f * units_f;
	}
	else
	{
		size_xf = limits_xf / scale_f * units_f;
		size_yf = limits_yf / scale_f * units_f;
	}

	offset_lf = atof(edit[0].txt);
	offset_bf = atof(edit[1].txt);

	size_xf += offset_lf;
	size_yf += offset_bf;

	sprintf(mm_x, "%.2f", size_xf);
	sprintf(mm_y, "%.2f", size_yf);

	lab[0].txt = extents_x;
	lab[1].txt = extents_y;
	lab[2].txt = limits_x;
	lab[3].txt = limits_y;

	lab[4].txt = mm_x;
	lab[5].txt = mm_y;

	size_xi = (int)(size_xf + 0.5);
	if (size_xi > 99999) size_xi = 99999;
	size_yi = (int)(size_yf + 0.5);
	if (size_yi > 99999) size_yi = 99999;

	sprintf(new_format, "%d; %d", size_xi, size_yi);
	
	listbox[2].txt[0] = new_format;

	listbox[2].foff = 0;
	listbox[2].poz = 0;

	Get_Default_Color_Dlg(&color_dlg);

	wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
	wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

	x_win_dlg = import_DXF_dlg.x*wsp_x;
	y_win_dlg = 26 + import_DXF_dlg.y*wsp_y;

	dx_win_dlg = 10 + import_DXF_dlg.dx*wsp_x;
	dy_win_dlg = 54 + import_DXF_dlg.dy*wsp_y;

    Save_Update_flex(0, &curr_h, &curr_v);
	Ret_Val = Dialog(&import_DXF_dlg, &color_dlg, proc_dlg_DXF_date, TRUE);
    Save_Update_flex(1, &curr_h, &curr_v);

	if (Ret_Val == Dlg_Ret_Val_OK)
	{
		units_i = listbox[0].foff + listbox[0].poz;
		units_f = funits[units_i];
		scale_i = listbox[1].foff + listbox[1].poz;
		scale_f = fscales[scale_i];
		auto_size = Get_Check_Button(&import_DXF_dlg, ID_AUTO_SIZE);

		head->Jednostki_dxf = units_f;
		head->SkalaF_dxf = scale_f;

		head->offset_x = atof(offset_x);
		head->offset_y = atof(offset_y);

		head->auto_size = auto_size;

		format_i = listbox[2].foff + listbox[2].poz;
		if (format_i == 0)
		{
			strcpy((char *)&format_xy, (char*)listbox[2].txt[0]);
			ptr = strrchr(format_xy, ';');
			if (ptr != NULL)
			{
				*ptr = '\0';
				head->format_x = atoi(format_xy);
				head->format_y = atoi(ptr + 1);
			}
			else
			{
				head->format_x = 1189;  //just A0  1189,841
				head->format_y = 841;
			}
		}
		else
		{
			head->format_x = fformats[format_i][0];
			head->format_y = fformats[format_i][1];
		}

		return 1;  //OK
	}
	else if (Ret_Val == Dlg_Ret_Val_Cancel) return 0;  //ESC
	else return 0;
}


#undef __DXFDLG__
