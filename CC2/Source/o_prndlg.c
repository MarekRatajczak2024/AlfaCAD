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

#define __PRNDLG__
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

#include "bib_e.h"
#include "b_bibe.h"
#include "rysuj_e.h"
#include "o_loadf.h"
#include "o_dialog.h"
#include "o_pltprn.h"
#include "o_libfun.h"
#include "o_inicnf.h"
#include "o_prn.h"

#include "o_prn_cups.h"

#include <allegext.h>

#include "dialog.h"

extern void reset_previewed(void);
extern void setviewport(int left, int top, int right, int bottom, int clip);
extern void utf8Upper(char* text);
extern int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext);
extern char *plotter_p;

extern void setcolor(int kolor);
extern int ViewDrawing(int x1, int y1, int x2, int y2, int x0, int y0, BOOL gray, BOOL bw, int gray_sat, int *real_x1, int *real_y1, int *real_x2, int *real_y2, unsigned char adjust, BOOL fill_background, T_Prn_Ini_Date *prn_ini_date);

extern int TRANSLUCENCY;

extern PRIV_PRN_WINDOW *get_priv_prn_window(void);
extern void set_priv_prn_window(double Xmin_, double Ymin_, double Xmax_, double Ymax_);
extern void setfillstyle_(int pattern, int color);
extern void myrectfill(int x1, int y1, int x2, int y2);
extern void myline(int x1, int y1, int x2, int y2);
extern int __gr_White(void);
extern void set_mode_trans();
extern void set_mode_solid();
extern void draw_dlg_prev(TDIALOG *dlg);
extern void Set_Enable_Button(TDIALOG* dlg, int id_button, BOOL enable);
extern void Set_Enable_Edit(TDIALOG *dlg, int id_edit, BOOL enable);
extern void draw_input_line(INPUTLINE *InputLine);
extern void draw_button(BUTTON *Button);

extern void hide_x_cursor(void);

extern int Print2Page(int WINPRINT_DEF);

extern BOOL SetDefaultPrinter_(char* printer_name);
extern int GetPrintPageWidth(void);
extern int GetPrintPageHeight(void);

typedef unsigned long DWORD;

DWORD SystemSilent(char* strFunct, char* strstrParams);

#define LINE myline

#define WIDTH_DOT 0.2

char wydruk_na_drukarce[128];
typedef enum { PRN_PCX = 0, PRN_WINDOWS, PRN_PDF, PRN_9, PRN_24, PRN_48, PRN_LASER, PRN_ESC_P2 } PRN;
typedef enum { PRN_MODE_EPSON = 0, PRN_MODE_IBM = 1} PRN_MODE ;
typedef enum { PRN_COLOR_KCMY = 0, PRN_COLOR_YMCK = 1, PRN_COLOR_MCYK = 2, PRN_COLOR_GRAY = 3} COLOR_MODE ;
typedef enum { D_DRAFT = 1, D_NORMAL, D_DOUBLE, D_HIGH, D_SUPER_HIGH } DENSITY;

int x1_prev=0, y1_prev=0, x2_prev=0, y2_prev=0, x0_prev=0, y0_prev=0;
float cups_prn_width_paper;
float cups_prn_height_paper;

void set_cups_printer_name(int no, CUPS_PRINTERS *cups_printer, BOOL defaultp, int p_counter);

typedef enum
{
  Prn_density_draft = 0,
  Prn_density_normal,
  Prn_density_double,
  Prn_density_high,
  Prn_density_super_high,
  Prn_l_luk,
}BUT_Prn_Config;

typedef enum
{
  Prn_type = 0,
  Prn_mode,
  Prn_condensed,
  Prn_density,
  Prn_rotation,
  Prn_inversion,
  Prn_reflection,
  Prn_out_to_file,
  Prn_margin,
  Prn_line_thinest_width,
  Prn_line_thin_width,
  Prn_line_norm_width,
  Prn_line_thick_width,
  Prn_line_thickest_width,
  Prn_top_margin ,
  Prn_bottom_margin,
  Prn_left_margin ,
  Prn_right_margin,
  Prn_width_paper ,
  Prn_height_paper ,
  Prn_width_paper_printer,
  Prn_height_paper_printer,
  Prn_scale ,
  Prn_actual_size,
  Prn_print_pause ,
  Prn_change_page ,
  Prn_color_print ,
  Prn_PCX_gray,
  Prn_gray_print,
  Prn_color_mode,
  Prn_set_page ,
  Prn_print_window ,
  Prn_print_sheet ,
  Prn_port_num ,
  Prn_text_mode ,
  Prn_buforowanie,
  Prn_kompresja,
  Prn_ploter,
  Prn_oce,
  Prn_fixed_name,
  Prn_plik,
  Prn_copies,
  Prn_thin_point_PCX,
  Prn_line_thick_correction_H,
  Prn_line_thick_correction_V,
  Prn_roll,
  Prn_WINPRINT_DEF,
  Prn_Image_Format,
  Prn_Gray_Saturation,
  Prn_Background,
} BUT_Prn_Ini_Date;



T_Prn_Ini_Date prn_ini_date_old;

/*

                           LOCAL DECLARATIONS                               

*/

static BOOL first = TRUE;               /*pierwsze wejscie do programu*/
static T_Prn_Config     prn_config [PRN_ESC_P2+1] ;
static T_PTR_Prn_Config ptrs__config ;
static float prn_ini_date_width_paper_bak;
static BOOL prn_ini_date_reflection_bak;
static float prn_ini_date_height_paper_bak;
static BOOL prn_ini_date_actual_size_bak;
static BOOL size_bak = FALSE;
static BOOL reflection_bak = FALSE;

static T_Prn_Ini_Date   prn_ini_date=
{
	PRN_WINDOWS,          /*typ drukarki*/
	PRN_MODE_IBM,
	TRUE,           /*kondensacja do rozmirow papieru, wydruk  w skali*/
	D_DRAFT,        /* D_HIGH, D_DOUBLE, D_NORMAL, D_DRAFT*/ /*gestosc drukowania*/
	TRUE,           /*caly rysunek, okno*/
	0.0,              /* okno do plotowania*/
	0.0,              /* okno do plotowania*/
	0.0,              /* okno do plotowania*/
	0.0,              /* okno do plotowania*/
	0.0,             /* h0*/
    0.0,0.0,0.0,0.0,        /*wewnetrzne okno do plotowania*/
	FALSE,          /*obrot rysunku 1 - tak, 0 - nie*/
   FALSE,          /*odwrocenie rysunku*/
   FALSE,           /*reflection*/
	TRUE,           /*wydruk do zbioru*/
	0,              /*numer pierwszej strony*/
	5.0,              /*zakladka*/
	0.10,		/*szerokosc linii bardzo cienkiej*/
	0.18,            /*szerokosc linii cienkiej*/
	0.35,            /*                grubej*/
	0.50,            /*                b. grubej*/
	0.70,		/*		  najgrubszej*/
	0.0,            /*gorny margines*/
	0.0,            /*dolny margines*/
	0.0,            /*lewy margines*/
	0.0,            /*prawy margines*/
	210.0,           /*szerokosc wydruku*/
	297.0,           /*wysokosc wydruku*/
	210.0,           /*szerokosc wydruku drukarki*/
	297.0,           /*wysokosc wydruku drukarki*/
	1.0,             /*skala dla wydruku w skali*/
	0,              /*actual scale*/
	0,              /*pauza na zmiane arkusza*/
	0,              /*zmiama strony*/
	1,              /*wydruk w kolorze*/
   0,              /*PCX gray*/
	0,			 /*grey print*/
	PRN_COLOR_KCMY, /*tryb koloru KCMY / YMCK*/
	0,              /*ustawienie dlugosci strony*/
	0,              /*wydruk okna*/
   0,              /*wydruk arkusza*/
   NULL,           /*adres arkusza*/
	1,              /*numer portu dla drukarki*/
	0,              /*przelaczenie w tryb tekstowy*/
	0,              /*buforowanie*/
   1,              /*kompresja*/
   0,              /*ploter*/
   0,			   /*oce*/
   0,              /*fixed_name*/
   "",
   1,
   2,
   1.0,			   /*korekta gruboci linii poziomej*/
   1.0,			   /*korekta gruboci linii pionowej*/
   0,              /*default roll*/
   0,			   /*WinPrint default*/
   "BMP",              /*image format*/
   0,
   0,
   0,
   3,
   0
};

extern char *prn_file;

static BOOL     add_value (FILE *) ;
static BOOL     get_config_prn (void) ;
static void     get_config0PCX_prn (void);
static void     get_config0PDF_prn(void);
static void     get_config0WINDOWS_prn (void);
static BOOL     get_prn_ini_date (float prn_width_paper, float prn_height_paper);
static BOOL     get_printer_date (T_Fstring, T_Fstring) ;
static BOOL     get_config_param (T_Fstring , T_Fstring) ;


#define DYLab_xx (DYLab - 3)

#define DXButOK 49
#define XpPaper  5 ////10
#define YpPaper  5
#define XpLineWidth  XpPaper
#define YpLineWidth  152
#define XpCondensed  XpPaper //XpPrinterType
#define YpCondensed  (59 - 6)
#define XpRotation   XpPaper //XpPrinterType
#define YpRotation   YpPaper
#define XpDensity    180 ////190 //222
#define YpDensity    152
#define XpFileName   XpDensity
#define YpFileName   5
#define XpPrinterType XpDensity //392 /*398*/
#define YpPrinterType YpFileName
#define DYBut (DYLab -3)   /*-4*/
#define DXBut 140
#define DXRBut 20
#define DXRRBut 8
#define DYRRBut 8
#define DXGroup1  203  /*200*/
#define DXGroup  160
#define DXGroup2 170
#define DXGroup3 122
#define DXGroup4 65
#define XpCANCEL XpDensity + 70//(XpPaper + 105)  // (XpPaper + 95)
#define YpOK 200
#define XpOK XpCANCEL + 51
#define YpCANCEL YpOK
#define DXEdit 36
#define DXEditR 42
#define XpEditPage XpCondensed + 131
#define XpEditThick XpDensity + 25
#define XpEditMargin XpCondensed + 16
#define XpEditMargin1 XpCondensed + 70
#define XpRange XpLineWidth + DXGroup3 + 4

static DLG_COLOR color_dlg;

#define DIST_LIN 4
static DARK_LIGHT_LINE line_d_l [] =
{
	{ XpCondensed+1, YpCondensed+ 4 * DYLab - DYIL - 16, DXGroup2-62, 1, LINE_NULL, COLOR_NULL},
	{ XpCondensed + DXGroup2 - 62, YpCondensed + 4 * DYLab - DYIL - 16, 1, 23, LINE_NULL, COLOR_NULL},
    { XpPrinterType+1, YpPrinterType+ 2 * DYLab_xx + 11, DXGroup2-2, 1, LINE_NULL, COLOR_NULL},
	{ XpPrinterType+1, YpCondensed+ 4 * DYLab - DYIL -16, DXGroup2-2, 1, LINE_NULL, COLOR_NULL},
	
};

static GROUP_BOX gr_box []=
{
  { XpCondensed, YpCondensed -10, DXGroup2, 5 * DYLab - DYIL - 5,
	COLOR_NULL ,COLOR_NULL, prn_comm7, 0, NULL,
  },
  {  XpRotation, YpRotation, DXGroup2, 2 * DYLab - DYIL - 8,
	COLOR_NULL ,COLOR_NULL, prn_comm8, 0, NULL,
  },

  
  { XpCANCEL /*XpDensity*/, YpLineWidth+2, DXGroup2 - (XpCANCEL - XpDensity), 1 * DYLab + 3,
	COLOR_NULL ,COLOR_NULL, prn_comm10, 0, NULL
  },
  
  { XpPrinterType, YpPrinterType, DXGroup2, 5 * DYLab_xx + DYIL + 42,
	COLOR_NULL ,COLOR_NULL, prn_comm11, 0, NULL,
  },
  { XpDensity, YpLineWidth+2, DXGroup4, 3.6 * DYLab -2,
	COLOR_NULL ,COLOR_NULL, prn_comm12, 0, NULL,
  },

  { XpLineWidth + DXGroup3 + 1, YpLineWidth+2, DXGroup4-18, 3.6 * DYLab -2,
          COLOR_NULL ,COLOR_NULL, prn_comm33, 0, NULL,
  },

  { XpLineWidth, YpLineWidth+2, DXGroup3, 3.6 * DYLab-2,
	COLOR_NULL ,COLOR_NULL, prn_comm32, 0, NULL,  /*process needs to be initialized later*/
  },
};

#define EDITTEXTLEN 7
#define EDITTEXTLENR 10
static char sz_copies [EDITTEXTLEN] = "1" ;
static char sz_thin_point_PCX [EDITTEXTLEN]= "0.25";
static char sz_line_thick_correction_H [EDITTEXTLEN]= "1.0";
static char sz_line_thick_correction_V [EDITTEXTLEN]= "1.0";
static char sz_page [EDITTEXTLEN] = "1" ;
static char sz_margin [EDITTEXTLEN] = "0.0" ;             //zakladka
static char sz_line_thinest_width [EDITTEXTLEN] = "0.15" ;    //grubosc linii bardzo cienkiej
static char sz_line_thin_width [EDITTEXTLEN] = "0.25" ;    //grubosc linii cienkiej
static char sz_line_norm_width [EDITTEXTLEN] = "0.35" ;    //grubosc linii grubej
static char sz_line_thick_width [EDITTEXTLEN] = "0.45" ;   //grubosc linii b.grubej
static char sz_line_thickest_width [EDITTEXTLEN] = "0.55" ;    //grubosc linii najgrubszej
static char sz_top_margin [EDITTEXTLEN] = "0.0" ;         //gorny margines
static char sz_left_margin [EDITTEXTLEN] = "0.0" ;        //lewy margines
static char sz_bottom_margin[EDITTEXTLEN] = "0.0";         //dolny margines
static char sz_right_margin[EDITTEXTLEN] = "0.0";        //prawy margines
static char sz_width_paper [EDITTEXTLEN] = "0.0" ;        //szerokosc wydruku
static char sz_height_paper [EDITTEXTLEN] = "0.0" ;       //wysokosc wydruku
static char sz_scale [EDITTEXTLEN] = "1" ;              //skala dla wydruku w skali
static char sz_port_num [EDITTEXTLEN] = "0" ;           //numer portu LPT dla drukarki
static char sz_range_x1[EDITTEXTLENR] = "0.00";
static char sz_range_y1[EDITTEXTLENR] = "0.00";
static char sz_range_x2[EDITTEXTLENR] = "0.00";
static char sz_range_y2[EDITTEXTLENR] = "0.00";

char digits[16] = ".-+0123456789";
char numbers[16] = "0123456789";

#define DISTTMP 120
#define DIST1TMP 120

static INPUTLINE edit_p []=
        {
#define IL_WIDTH_PAPER 30
	{ XpEditPage, YpCondensed + 2.79 * DYLab - 21, DXEdit, 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_width_paper, IL_WIDTH_PAPER,&digits, 
   },
#define IL_HEIGHT_PAPER 31
	{ XpEditPage, YpCondensed + 3.41 * DYLab - 21, DXEdit, 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_height_paper, IL_HEIGHT_PAPER,&digits, 
   },
#define IL_MARGIN 32
	{XpEditPage, YpCondensed + 2.17 * DYLab - 21, DXEdit, 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_margin, IL_MARGIN,&digits, 
   },
#define IL_LINE_THINEST_WIDTH 33
/*5*/   { XpEditThick, YpLineWidth+0.5*DYLab+1, DXEdit, 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_line_thinest_width, IL_LINE_THINEST_WIDTH,&digits, 
   },
#define IL_LINE_THIN_WIDTH 34
/*6*/   { XpEditThick, YpLineWidth+1.1*DYLab+1, DXEdit, 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_line_thin_width, IL_LINE_THIN_WIDTH,&digits, 
   },
#define IL_LINE_NORM_WIDTH 35 
/*7*/   { XpEditThick, YpLineWidth+1.7*DYLab+1, DXEdit, 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_line_norm_width, IL_LINE_NORM_WIDTH,&digits, 
   },
#define IL_LINE_THICK_WIDTH 36
/*8*/   { XpEditThick, YpLineWidth+2.31*DYLab+1, DXEdit, 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_line_thick_width, IL_LINE_THICK_WIDTH,&digits, 
   },
#define IL_LINE_THICKEST_WIDTH 37
/*9*/   { XpEditThick, YpLineWidth+2.92*DYLab+1, DXEdit, 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_line_thickest_width, IL_LINE_THICKEST_WIDTH,&digits, 
   },
#define IL_SCALE 38
/*10*/   { XpEditPage, YpCondensed+ 0.93 * DYLab -21, DXEdit , 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_scale, IL_SCALE,&digits, 
   },
#define IL_PAGE 39
/*11*/   { XpEditPage, YpCondensed+ 1.55 * DYLab -21, DXEdit , 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_page, IL_PAGE,&numbers, 
   },
#define IL_COPIES (-1) //14
#define IL_THIN_POINT_PCX (-1) //15

#define IL_LINE_THICK_CORRECT_H 40 //16
   { XpCANCEL + 4 /*XpDensity + 15*/, YpLineWidth+0.5*DYLab+2, 45, 12, COLOR_NULL,COLOR_NULL,
      COLOR_NULL,COLOR_NULL,COLOR_NULL, 6, 0, 1, sz_line_thick_correction_H, IL_LINE_THICK_CORRECT_H,&digits, 
   },
#define IL_LINE_THICK_CORRECT_V 41 //17
   { XpCANCEL + 52 /*XpDensity + 100*/, YpLineWidth+0.5*DYLab+2, 45, 12, COLOR_NULL,COLOR_NULL,
      COLOR_NULL,COLOR_NULL,COLOR_NULL, 6, 0, 1, sz_line_thick_correction_V, IL_LINE_THICK_CORRECT_V,&digits, 
   },

#define IL_LEFT_MARGIN 42

	{ XpEditMargin+4, YpCondensed + 2.79 * DYLab - 21, DXEdit, 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_left_margin, IL_LEFT_MARGIN,&digits, 
   },

 #define IL_TOP_MARGIN 43

	{ XpEditMargin+4, YpCondensed + 3.41 * DYLab - 21, DXEdit, 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_top_margin, IL_TOP_MARGIN,&digits, 
   },

   #define IL_RIGHT_MARGIN 44

	{ XpEditMargin1+4, YpCondensed + 2.79 * DYLab - 21, DXEdit, 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_right_margin, IL_RIGHT_MARGIN,&digits, 
   },

 #define IL_BOTTOM_MARGIN 45

	{ XpEditMargin1+4, YpCondensed + 3.41 * DYLab - 21, DXEdit, 12, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 5,0,1, sz_bottom_margin, IL_BOTTOM_MARGIN,&digits, 
   },

#define IL_RANGE_X1 46
/*5*/   { XpRange, YpLineWidth+0.5*DYLab+1, DXEditR, 12, COLOR_NULL,COLOR_NULL,
          COLOR_NULL,COLOR_NULL,COLOR_NULL, 7,0,0, sz_range_x1, IL_RANGE_X1,&digits,
    },
#define IL_RANGE_Y1 47
/*6*/   { XpRange, YpLineWidth+1.1*DYLab+1, DXEditR, 12, COLOR_NULL,COLOR_NULL,
          COLOR_NULL,COLOR_NULL,COLOR_NULL, 7,0,0, sz_range_y1, IL_RANGE_Y1,&digits,
    },
#define IL_RANGE_X2 48
/*7*/   { XpRange, YpLineWidth+1.7*DYLab+1, DXEditR, 12, COLOR_NULL,COLOR_NULL,
          COLOR_NULL,COLOR_NULL,COLOR_NULL, 7,0,0, sz_range_x2, IL_RANGE_X2,&digits,
    },
#define IL_RANGE_Y2 49
/*8*/   { XpRange, YpLineWidth+2.31*DYLab+1, DXEditR, 12, COLOR_NULL,COLOR_NULL,
          COLOR_NULL,COLOR_NULL,COLOR_NULL, 7,0,0, sz_range_y2, IL_RANGE_Y2,&digits,
    },
};

static BUTTON buttons_p [] =
{
#define BUT_CONDENSED_Y 1
  { XpCondensed + DXIL, YpCondensed -5 , DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm13, 0,B_RADIOBUTTON, 0, 1,0, BUT_CONDENSED_Y,
  },
#define BUT_CONDENSED_N 2
{ XpCondensed + DXIL, YpCondensed + 1 * DYLab -18, DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm14, 0,B_RADIOBUTTON, 0, 1,0, BUT_CONDENSED_N,
  },

#define BUT_ROTATION 3
  { XpRotation+ DXIL, YpRotation + 1 * DYLab - 12, DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm15, 0,B_CHECKBOX, 0, 1,0, BUT_ROTATION,
  },
#define BUT_INVERSION 4
  { XpRotation+ DXIL + 55 , YpRotation + 1 * DYLab - 12, DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm16, 0,B_CHECKBOX, 0, 1,0, BUT_INVERSION,
  },
#define BUT_REFLECTION 5
  { XpRotation + DXIL + 110 , YpRotation + 1 * DYLab - 12, DXRBut, DYBut, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "", 0,B_CHECKBOX, 0, 1,0, BUT_REFLECTION,
  },
#define BUT_DENSITY_DRAFT 6
  { XpDensity+ DXIL, YpCondensed + 5, DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm17, 0,B_RADIOBUTTON, 0, 1,0, BUT_DENSITY_DRAFT,
  },
#define BUT_DENSITY_NORMAL 7
  { XpDensity+ DXIL, YpCondensed+ 17, DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm18, 0,B_RADIOBUTTON, 0, 1,0, BUT_DENSITY_NORMAL,
  },
#define BUT_DENSITY_DOUBLE 8
  { XpDensity+ DXIL, YpCondensed+ 29, DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm19, 0,B_RADIOBUTTON, 0, 1,0, BUT_DENSITY_DOUBLE,
  },
#define BUT_DENSITY_HIGH 9
  { XpDensity+ DXIL, YpCondensed+ 41, DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm20, 0,B_RADIOBUTTON, 0, 1,0, BUT_DENSITY_HIGH,
  },
 #define BUT_DENSITY_SUPER_HIGH 10
  { XpDensity + DXIL, YpCondensed + 52, DXRBut, DYBut, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, prn_comm21, 0,B_RADIOBUTTON, 0, 1,0, BUT_DENSITY_SUPER_HIGH,
  },

#define BUT_PRINTER_PCX 11
  
  { XpPrinterType+ DXIL, YpPrinterType+ 1 * DYLab - 12, DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "" /*prn_comm[22]*/, 0,B_RADIOBUTTON, 0, 1,0, BUT_PRINTER_PCX,
  },

#define BUT_PRINTER_WINDOWS 12
  
  { XpPrinterType+ DXIL + 110, YpPrinterType+ 1 * DYLab - 12, DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "" /*prn_comm[22]*/, 0,B_RADIOBUTTON, 0, 1,0, BUT_PRINTER_WINDOWS,
  },

  #define BUT_PRINTER_PDF 13

  { XpPrinterType + DXIL + 55, YpPrinterType + 1 * DYLab - 12, DXRBut, DYBut, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "" /*prn_comm[22]*/, 0,B_RADIOBUTTON, 0, 1,0, BUT_PRINTER_PDF,
  },
  
#define BUT_OK 14
  { XpOK, YpOK, DXButOK, DYBut0-6, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm23, 0,B_PUSHBUTTON, 82, 1,0, BUT_OK,
  },
#define BUT_CANCEL 15
  { XpCANCEL, YpCANCEL, DXButOK, DYBut0-6, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm24, 0,B_PUSHBUTTON, 83, 1,0, BUT_CANCEL,
  },

#define BUT_GRAY_PRINT 16
  { XpPrinterType + DXIL + 2, YpCondensed + 4 * DYLab -19 , DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm25, 0, B_CHECKBOX, 0, 1,0, BUT_GRAY_PRINT,
  },
#define BUT_BW_PRINT 17
  { XpPrinterType + DXIL + 58, YpCondensed + 4 * DYLab - 19 , DXRBut, DYBut, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "", 0, B_CHECKBOX, 0, 1,0, BUT_BW_PRINT,
  },
#define BUT_PCX_GRAY 18
  { XpPrinterType + DXIL + 111, YpCondensed + 4 * DYLab -19 , DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm26, 0, B_CHECKBOX, 0, 1,0, BUT_PCX_GRAY,
  },

#define BUT_PRINT_WINDOW 19
  { XpLineWidth + DXIL,YpCondensed + 4 * DYLab - 19, DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm27, 0, B_CHECKBOX, 0, 1,0, BUT_PRINT_WINDOW,
  },
#define BUT_PRINT_SHEET 20
  { XpLineWidth + 64,YpCondensed + 4 * DYLab - 19, DXRBut, DYBut, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm28, 0, B_CHECKBOX, 0, 1,0, BUT_PRINT_SHEET,
  },

#define BUT_ZAPIS 21
  { XpCANCEL, YpCANCEL - 19, DXButOK, DYBut - 3, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm29, 0,B_PUSHBUTTON, 84, 1,0, BUT_ZAPIS,
  },
#define BUT_ODCZYT 22
  { XpOK, YpCANCEL - 19, DXButOK, DYBut - 3, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm30, 0,B_PUSHBUTTON, 85, 1,0, BUT_ODCZYT,
  },

#define BUT_ACTUAL_SIZE 23
  { XpCondensed + DXIL, YpCondensed + 2 * DYLab - 25, DXRBut, DYBut, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, prn_comm15, 0,B_CHECKBOX, 0, 1,0, BUT_ACTUAL_SIZE,
  },

#define BUT_GRAY_1 24
{ XpPrinterType + DXIL + 47, YpCondensed + 4 * DYLab - 11 , DXRRBut, DYRRBut, COLOR_NULL,
  COLOR_NULL, COLOR_NULL, prn_comm25, 0, B_ROUNDRADIOBUTTON, 0, 1,0, BUT_GRAY_1,
},
#define BUT_GRAY_2 25
  { XpPrinterType + DXIL + 47, YpCondensed + 4 * DYLab - 5 , DXRRBut, DYRRBut, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, prn_comm25, 0, B_ROUNDRADIOBUTTON, 0, 1,0, BUT_GRAY_2,
  },
#define BUT_GRAY_3 26
  { XpPrinterType + DXIL + 47, YpCondensed + 4 * DYLab + 1, DXRRBut, DYRRBut, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, prn_comm25, 0, B_ROUNDRADIOBUTTON, 0, 1,0, BUT_GRAY_3,
  },
#define BUT_BACKGROUND 27
  { XpPrinterType + DXIL + 111,YpCondensed + 2.57 * DYLab + 1 * DYIL - 13 , DXRBut, DYBut, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, prn_comm26, 0, B_CHECKBOX, 0, 1,0, BUT_BACKGROUND,
  },
#define BUT_SET_RANGE 28
  { XpRange, YpLineWidth+2.92*DYLab -1, DXEditR, 15, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, prn_comm27, 0, B_PUSHBUTTON, 127, 1,0, BUT_SET_RANGE,
  },
};

const IMAGE images_pr [] =
{
 {  XpRotation + DXIL+ 15, YpRotation + 1 * DYLab - 12, 48, 48, 18,image_pr_tips0},
 {  XpRotation + DXIL + 70 , YpRotation + 1 * DYLab - 12, 48, 48, 19,image_pr_tips1},
 {  XpRotation + DXIL + 125 , YpRotation + 1 * DYLab - 12, 48, 48, 20,image_pr_tips2},
 {  XpPrinterType + DXIL + 15, YpCondensed + 4.15 * DYLab - 20 , 48, 48, 21,image_pr_tips3},
 {  XpPrinterType + DXIL + 72, YpCondensed + 4.15 * DYLab - 20, 48, 48, 22,image_pr_tips4},
 {  XpPrinterType + DXIL + 126, YpCondensed + 4.15 * DYLab - 20, 48, 48, 23,image_pr_tips5},

 {  XpPrinterType+133, YpPrinterType + 9, 64, 64, 16,image_pr_tips6},
 {  XpPrinterType+23, YpPrinterType + 9, 64, 64, 17,image_pr_tips7},
 {  XpPrinterType + 78, YpPrinterType + 9, 64, 64, 115,image_pr_tips25},

 {  XpLineWidth + 25, YpCondensed + 4.15 * DYLab - 20, 64, 64, 24,image_pr_tips8},
 {  XpLineWidth + 80, YpCondensed + 4.15 * DYLab - 20, 64, 64, 25,image_pr_tips9},

 {  XpCondensed + DXIL + 103, YpCondensed + 0.70 * DYLab + 1 * DYIL + 2 - 22, 32, 32, 29,image_pr_tips10},
 {  XpCondensed + DXIL + 103, YpCondensed + 1.32 * DYLab + 1 * DYIL + 2 - 22, 32, 32, 30,image_pr_tips11},
 {  XpCondensed + DXIL + 103, YpCondensed + 2.57 * DYLab + 1 * DYIL + 2 - 22, 32, 32, 31,image_pr_tips12},
 {  XpCondensed + DXIL + 103, YpCondensed + 3.18 * DYLab + 1 * DYIL + 2 - 22, 32, 32, 32,image_pr_tips13},
 {  XpCondensed + DXIL + 103, YpCondensed + 1.94 * DYLab + 1 * DYIL + 2 - 22, 32, 32, 33,image_pr_tips14},

 {  XpCondensed + 2, YpCondensed + 2.57 * DYLab + 1 * DYIL + 2 - 22, 32, 32, 65,image_pr_tips15},
 {  XpCondensed + 56, YpCondensed + 2.57 * DYLab + 1 * DYIL + 2 - 22, 32, 32, 68,image_pr_tips16},
 {  XpCondensed + 2, YpCondensed + 3.18 * DYLab + 1 * DYIL + 2 - 22, 32, 32, 64,image_pr_tips17},
 {  XpCondensed + 56, YpCondensed + 3.18 * DYLab + 1 * DYIL + 2 - 22, 32, 32, 67,image_pr_tips18},
 {  XpCondensed + DXIL + 16, YpCondensed + 1.70 * DYLab + 1 * DYIL + 2 - 22, 48, 48, 66,image_pr_tips19},


 {  XpDensity + 5, YpLineWidth + DYIL + 0.35 * DYLab, 32, 32, 34,image_pr_tips20},
 {  XpDensity + 5, YpLineWidth + DYIL + 0.95 * DYLab, 32, 32, 35,image_pr_tips21},
 {  XpDensity + 5, YpLineWidth + DYIL + 1.55 * DYLab, 32, 32, 36,image_pr_tips22},
 {  XpDensity + 5, YpLineWidth + DYIL + 2.15 * DYLab, 32, 32, 37,image_pr_tips23},
 {  XpDensity + 5, YpLineWidth + DYIL + 2.75 * DYLab, 32, 32, 38,image_pr_tips24},

 {  XpCondensed + DXIL + 103, YpCondensed + 4.15 * DYLab - 18, 48, 48, 207,image_pr_tips26},

 {  XpPrinterType + DXIL + 126, YpCondensed + 2.57 * DYLab + 1 * DYIL - 13, 48, 48, 125,image_pr_tips27},
};

char *image_formats[] = {
	 "BMP",
	 "PNG",
	 "JPEG",
	 "TIFF",
	 "PCX",
	 "EPS",
	 "PDF",
};

static char *paper_formats[] = {
	"",
	"ISO A4", // 297*210
	"ISO_A4",  // 210*297
	"ISO A3", // 420*297
	"ISO_A3",  // 297*420
	"ISO A2",  // 594*420
	"ISO_A2",   // 420*594
	"ISO A1",  // 841*594
	"ISO_A1",   // 594*841
	"ISO A0",  //1189*841
	"ISO_A0",   //841*1189
	"ISO B4", // 250*176
	"ISO_B4",  // 176*250
	"ISO B3", // 500*353
	"ISO_B3",  // 353*500
	"ISO B2",  // 707*500
	"ISO_B2",   // 500*707
	"ISO B1",  //1000*707
	"ISO_B1",   //707*1000
	"ISO B0",  //1414*1000
	"ISO_B0",   //1000*1414
	"ANSI A", //279.4*215.9
	"ANSI_A",  //215.9*279.4
	"ANSI B", //431.8*279.4
	"ANSI_B",  //279.4*431.8
	"ANSI C", //559*432
	"ANSI_C",  //432*559
	"ANSI D", //864*559
	"ANSI_D",  //559*864
	"ANSI E", //1118*864
	"ANSI_E",  //864*1118
    "US letter",  //215.9*279.4
	"US legal", //215.9*355.6
};

char *printer_types[32] = {NULL};

#define NoImageFormat 7
#define NoPaperFormat 33
#define NoPrinter 0

#define LS_IMAGE_FORMAT 0
#define LS_PAPER_FORMAT 50
#define LS_PRINTER_TYPE 51

static LISTBOX listbox[3] =
{
	{
	0, 0, 0, 0,
	NoImageFormat, 0, 0, 0, 0, 0, 9,
	COLOR_NULL, COLOR_NULL, COLOR_NULL,
	1, NULL, image_formats, LS_IMAGE_FORMAT, 0
  },
	{
	0, 0, 0, 0,
	NoPaperFormat, 11, 11, 0, 0, 0, 9,
	COLOR_NULL, COLOR_NULL, COLOR_NULL,
	1, NULL, paper_formats, LS_PAPER_FORMAT, 0
  },
    {
    0, 0, 0, 0,
    NoPrinter, 1, 0, 0, 0, 0, 64,
    COLOR_NULL, COLOR_NULL, COLOR_NULL,
    1, NULL, printer_types, LS_PRINTER_TYPE, 0
    }
};

void set_cups_printer_name(int no, CUPS_PRINTERS *cups_printer, BOOL defaultp, int p_counter)
{
    printer_types[no]=cups_printer->printer_name;
	listbox[2].txt[no] = printer_types[no];
    listbox[2].max=no+1;
    listbox[2].maxw=listbox[2].max;
    if (listbox[2].maxw>10) listbox[2].maxw=10;
    if (no==0) {listbox[2].poz=0; listbox[2].foff=0;}
    else
    {
        if ((defaultp) || (p_counter==0))
        {
            if (no<10) {listbox[2].poz=no; listbox[2].foff=0;}
            else {listbox[2].poz=0; listbox[2].foff=no;}
        }
    }

    if ((defaultp) ||  (p_counter==0))
    {
        cups_prn_width_paper=cups_printer->width/100.0;
        cups_prn_height_paper=cups_printer->length/100.0;
    }
}

void set_cups_default_printer_name(CUPS_PRINTERS* cups_printer) {

	int i = 0;
	int no;
	
	while (i < listbox[2].max)
	{
		if (strcmp(cups_printer->printer_name, printer_types[i]) == 0)
		{
			if (i < 10) { listbox[2].poz = i; listbox[2].foff = 0; }
			else { listbox[2].poz = 0; listbox[2].foff = i; }

			cups_prn_width_paper = cups_printer->width / 100.0;
			cups_prn_height_paper = cups_printer->length / 100.0;
			return;
		}
		i++;
	}

	no = listbox[2].max;
	printer_types[listbox[2].max] = cups_printer->printer_name;
	listbox[2].max += 1;
	listbox[2].maxw = listbox[2].max;
	if (listbox[2].maxw > 10) listbox[2].maxw = 10;
	if (no == 0) { listbox[2].poz = 0; listbox[2].foff = 0; }
	else
	{
		
		if (no < 10) { listbox[2].poz = no; listbox[2].foff = 0; }
		else { listbox[2].poz = 0; listbox[2].foff = no; }
		
	}

		cups_prn_width_paper = cups_printer->width / 100.0;
		cups_prn_height_paper = cups_printer->length / 100.0;

}

#define DXComboBox	50
#define DYComboBox	12
static COMBOBOX combobox[3] =
{
 { XpDensity + 12*DXIL, YpCondensed + 10, DXComboBox, DYComboBox, &listbox[0]},
 { XpEditPage, YpCondensed + 4.3 * DYLab - 21, DXEdit, 11, &listbox[1]},
 { XpPrinterType+ DXIL, YpPrinterType + DYLab + 15, 155, 11, &listbox[2]},
};

TDIALOG printer_dlg=
{
	2, 1, WIDTH_WORLD - 275,HEIGHT_WORLD-58,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0x40,0,0,
	wydruk_na_drukarce/*"Wydruk na drukarce"*/,
	4, &line_d_l,
	0, NULL,
	7 , &gr_box,
	20, &edit_p,
	28, &images_pr,
	28 , &buttons_p,
	0, NULL,
	3,&combobox,
    0,NULL, //Sliders
	NULL,
	NULL,
	0,
	0,
    NULL,
};

// `len` number of characters to print
// `len+1` is the size of `dest`

void mysprintf(char *dest, float f) {

    if (f >= 10000) sprintf(dest, "%.0f", f);
    if (f >= 1000) sprintf(dest, "%.1f", f);
    else sprintf(dest,"%.2f", f);
}

void Preview(int box, int x01, int y01, int x02, int y02, int x00, int y00)
{
	int ret_view;
	BOOL gray_prn, bw_prn;
	int gray_sat;
	BOOL fill_background;
	int r_x1, r_y1, r_x2, r_y2;
	BOOL window_prn, sheet_prn, actual_size_prn, rotation_prn, inversion_prn, reflection_prn, condensed_prn, background_prn;
	float scale_prn, left_prn, right_prn, top_prn, bottom_prn, maxx_prn, maxy_prn, margin_prn, width_prn, height_prn, page_prn;
	int dx, dy, ddx, ddy, dddx, dddy, dl, dr, dt, db, dm, dw, dh;
    int x1, y1, x2, y2, x0, y0, xx1, yy1, xx2, yy2, xxx1, yyy1, xxx2, yyy2;
	float scalex, scaley, scalexy, dxx, dyy;
	unsigned char adjust = 0;  //0-center, 1-left, 2-top, 4-right, 8-bottom
	BOOL lineR, lineB, nextR, nextC;
	char page_no[6];
	int pageNo = 0;
	int txt_x, txt_y, txt_w;
	float dscalex, dscaley, dscalexy = 1.0;
	int x1_, x2_, y1_, y2_;
    double X_max_, Y_max_;


	show_mouse(NULL);


    X_max_ = (prn_ini_date.xk-prn_ini_date.xp);
    Y_max_ = (prn_ini_date.yk-prn_ini_date.yp);


	x1 = x01+2;
	y1 = y01+2;
	x2 = x02-2;
	y2 = y02-2;
	x0 = x00;
	y0 = y00;

	xx1 = x1;
	xx2 = x2;
	yy1 = y1;
	yy2 = y2;

	
	x1_prev = x1;
	y1_prev = y1;
	x2_prev = x2;
	y2_prev = y2;
	x0_prev = x0;
	y0_prev = y0;

	dx = x2 - x1;
	dy = y2 - y1;

	dl=dr=dt=db=dm=dw=dh=0;

	window_prn = Get_Check_Button(&printer_dlg, BUT_PRINT_WINDOW);
	sheet_prn = Get_Check_Button(&printer_dlg, BUT_PRINT_SHEET);

    if (!sheet_prn)
	{
		//all in [mm]
		scale_prn = atof(edit_p[IL_SCALE - IL_WIDTH_PAPER].txt);
		left_prn = atof(edit_p[IL_LEFT_MARGIN - IL_WIDTH_PAPER].txt);
		right_prn = atof(edit_p[IL_RIGHT_MARGIN - IL_WIDTH_PAPER].txt);
		top_prn = atof(edit_p[IL_TOP_MARGIN - IL_WIDTH_PAPER].txt);
		bottom_prn = atof(edit_p[IL_BOTTOM_MARGIN - IL_WIDTH_PAPER].txt);
		margin_prn = atof(edit_p[IL_MARGIN - IL_WIDTH_PAPER].txt);
		width_prn = atof(edit_p[IL_WIDTH_PAPER - IL_WIDTH_PAPER].txt);
		height_prn = atof(edit_p[IL_HEIGHT_PAPER - IL_WIDTH_PAPER].txt);

		actual_size_prn = Get_Check_Button(&printer_dlg, BUT_ACTUAL_SIZE);
		rotation_prn = Get_Check_Button(&printer_dlg, BUT_ROTATION);
		inversion_prn = Get_Check_Button(&printer_dlg, BUT_INVERSION);
		reflection_prn = Get_Check_Button(&printer_dlg, BUT_REFLECTION);
		condensed_prn = Get_Check_Button(&printer_dlg, BUT_CONDENSED_N);
		background_prn = Get_Check_Button(&printer_dlg, BUT_BACKGROUND);
		page_prn =atoi(edit_p[IL_PAGE - IL_WIDTH_PAPER].txt);

        maxx_prn = X_max_ * scale_prn; //[mm]
        maxy_prn = Y_max_ * scale_prn; //[mm]

		if (((maxx_prn + left_prn + right_prn) > 0) && ((maxy_prn + top_prn + bottom_prn) > 0))
		{

			if (actual_size_prn)
			{
				if (!rotation_prn)
				{
					scalex = (float)(x2 - x1) / (maxx_prn + left_prn + right_prn);
					scaley = (float)(y2 - y1) / (maxy_prn + top_prn + bottom_prn);

					if (scalex < scaley) scalexy = scalex;
					else scalexy = scaley;
					
					ddx = (int)(((float)dx * maxx_prn / (maxx_prn + left_prn + right_prn))+0.5);
					ddy = (int)(((float)dy * maxy_prn / (maxy_prn + top_prn + bottom_prn))+0.5);


					dddx = dx - ddx;
					dddy = dy - ddy;
					if ((left_prn > 0) || (right_prn > 0))
						dl = (int)(((float)dddx * left_prn / (left_prn + right_prn))+0.5);
					else dl = 0;
					

					if ((top_prn > 0) || (bottom_prn > 0))
						dt = (int)(((float)dddy * top_prn / (top_prn + bottom_prn))+0.5);
					else dt = 0;
					
					dr = dddx - dl;
					db = dddy - dt;
					x1 += dl; x2 -= dr; y1 += dt; y2 -= db;
				}
				else
				{
					ddx = (int)(((float)dx * maxx_prn / (maxx_prn + left_prn + right_prn))+0.5);
					ddy = (int)(((float)dy * maxy_prn / (maxy_prn + top_prn + bottom_prn))+0.5);
					dddx = dx - ddx;
					dddy = dy - ddy;
					if ((left_prn > 0) || (right_prn > 0))
						dl = (int)(((float)dddx * left_prn / (left_prn + right_prn))+0.5);
					else dl = 0;
					if ((top_prn > 0) || (bottom_prn > 0))
						dt = (int)(((float)dddy * top_prn / (top_prn + bottom_prn))+0.5);
					else dt = 0;
					
					dr = dddx - dl;
					db = dddy - dt;
					x1 += dl; x2 -= dr; y1 += dt; y2 -= db;

				}
				adjust = 0;
			}
			else
			{
				if (condensed_prn)  //no scale
				{

					  if (width_prn>0.0)
						  scalex = (float)(x2 - x1) / (width_prn); // +left_prn + right_prn);
					  else scalex = 1.0;
					  if (height_prn>0.0)
						  scaley = (float)(y2 - y1) / (height_prn); // +top_prn + bottom_prn);
					  else scaley = 1.0;

				}
				else
				{
					if (!rotation_prn)
					{

                        maxx_prn = X_max_ * scale_prn; //[mm]
                        maxy_prn = Y_max_ * scale_prn; //[mm]

					}
					else
					{

                        maxx_prn = Y_max_ * scale_prn; //[mm]
                        maxy_prn = X_max_ * scale_prn; //[mm]
					}

					if (maxx_prn > 0.0)
					   scalex = (float)(x2 - x1) / (maxx_prn);
					else scalex = 1.0;
					if (maxy_prn > 0.0)
					    scaley = (float)(y2 - y1) / (maxy_prn);
					else scaley = 1.0;
				}


					//position of the page
					if (scalex < scaley)
					{
						scalexy = scalex;
						dyy = ((float)(y2 - y1)*(1.0 - scalex / scaley) / 2.0); // -1.0;
						xx1 = x1;
						xx2 = x2;
						yy1 = y1 + (int)(dyy+0.5);
						yy2 = y2 - (int)(dyy+0.5);

					}
					else
					{
						scalexy = scaley;
						dxx = ((float)(x2 - x1)*(1.0 - scaley / scalex) / 2.0); // -1.0;
						xx1 = x1 + (int)(dxx+0.5);
						xx2 = x2 - (int)(dxx+0.5);
						yy1 = y1;
						yy2 = y2;
					}

					dl = (int)(left_prn*scalexy + 0.5);
					dr = (int)(right_prn*scalexy + 0.5);
					dt = (int)(top_prn*scalexy + 0.5);
					db = (int)(bottom_prn*scalexy + 0.5);
					dm = (int)(margin_prn*scalexy + 0.5);
					dw = (int)(width_prn * scalexy +0.5);
					dh = (int)(height_prn * scalexy +0.5);

					x1 = xx1 + dl;
					x2 = xx2 - dr;
					y1 = yy1 + dt;
					y2 = yy2 - db;

					adjust = 3;

			}
		}

		setviewport(x1, y1, x2, y2, 1);
		bw_prn = Get_Check_Button(&printer_dlg, BUT_BW_PRINT);
		gray_prn = Get_Check_Button(&printer_dlg, BUT_GRAY_PRINT);

		if (Get_Check_Button(&printer_dlg, BUT_GRAY_1))
			gray_sat = 1;
		else if (Get_Check_Button(&printer_dlg, BUT_GRAY_2))
			gray_sat = 2;
		if (Get_Check_Button(&printer_dlg, BUT_GRAY_3))
			gray_sat = 3;
		fill_background = Get_Check_Button(&printer_dlg, BUT_BACKGROUND);
			

	    ret_view = ViewDrawing(x1, y1, x2, y2, x0, y0, gray_prn, bw_prn, gray_sat, &r_x1, &r_y1, &r_x2, &r_y2, adjust, fill_background, &prn_ini_date);
		setviewport(0, 0, getmaxx(), getmaxy(), 1);


		if (((x2 - x1) > 0.0) && ((y2 - y1) > 0.0))
		{
			if (!actual_size_prn)
			{
				if (!condensed_prn)
				{

					dscalex = (float)(r_x2 - r_x1) / (float)(x2 - x1);
					dscaley = (float)(r_y2 - r_y1) / (float)(y2 - y1);

					if (dscalex < dscaley) dscalexy = dscalex;
					else dscalexy = dscaley;

					if (dscalexy != 1.0)
					{

						dl = (int)(left_prn * scalexy * dscalexy + 0.5);
						dr = (int)(right_prn * scalexy * dscalexy + 0.5);
						dt = (int)(top_prn * scalexy * dscalexy + 0.5);
						db = (int)(bottom_prn * scalexy * dscalexy + 0.5);
						dm = (int)(margin_prn * scalexy * dscalexy + 0.5);
						dw = (int)(width_prn * scalexy * dscalexy + 0.5);
						dh = (int)(height_prn * scalexy * dscalexy + 0.5);
					}
				}
			}
		}
		

		
		setfillstyle_(SOLID_FILL, GetColorAC(15));
		
		//pages
		if (actual_size_prn)
		{
			if (!rotation_prn)
			{
				setcolor(WHITE);
				
				myrectfill(r_x1 - dl, r_y1 - dt, r_x2 + dr, r_y1);  //top strap
				myrectfill(r_x2, r_y1, r_x2 + dr, r_y2 + db);  //right strap
				myrectfill(r_x1 - dl, r_y2, r_x2, r_y2 + db); //bottom strap
				myrectfill(r_x1 - dl, r_y1, r_x1, r_y2); //left strap
				
				setcolor(DARKGRAY);

				LINE(r_x1 - dl, r_y1 - dt, r_x2 + dr, r_y1 - dt);
				LINE(r_x2 + dr, r_y1 - dt, r_x2 + dr, r_y2 + db);
				LINE(r_x2 + dr, r_y2 + db, r_x1 - dl, r_y2 + db);
				LINE(r_x1 - dl, r_y2 + db, r_x1 - dl, r_y1 - dt);
	
				LINE(r_x1, r_y1, r_x2, r_y1);
				LINE(r_x2, r_y1, r_x2, r_y2);
				LINE(r_x2, r_y2, r_x1, r_y2);
				LINE(r_x1, r_y2, r_x1, r_y1);	
			}
			else
			{
				setcolor(WHITE);
				
				myrectfill(r_x1 - dl, r_y1 - dt, r_x2 + dr, r_y1);  //top strap
				myrectfill(r_x2, r_y1, r_x2 + dr, r_y2 + db);  //right strap
				myrectfill(r_x1 - dl, r_y2, r_x2, r_y2 + db); //bottom strap
				myrectfill(r_x1 - dl, r_y1, r_x1, r_y2); //left strap
				
				setcolor(DARKGRAY);

				LINE(r_x1 - dl, r_y1 - dt, r_x2 + dr, r_y1 - dt);
				LINE(r_x2 + dr, r_y1 - dt, r_x2 + dr, r_y2 + db);
				LINE(r_x2 + dr, r_y2 + db, r_x1 - dl, r_y2 + db);
				LINE(r_x1 - dl, r_y2 + db, r_x1 - dl, r_y1 - dt);
				
				LINE(r_x1, r_y1, r_x2, r_y1);
				LINE(r_x2, r_y1, r_x2, r_y2);
				LINE(r_x2, r_y2, r_x1, r_y2);
				LINE(r_x1, r_y2, r_x1, r_y1);
			}
		}
		else
		{
			if (condensed_prn)
			{
				setcolor(WHITE);
				myrectfill(xx1, yy1, xx2, r_y1);  //top strap
				myrectfill(r_x2, r_y1, xx2, yy2);  //right strap
				myrectfill(xx1, r_y2, r_x2, yy2); //bottom strap
				myrectfill(xx1, r_y1, r_x1, r_y2); //left strap
				setcolor(DARKGRAY);

				LINE(xx1, yy1, xx2, yy1);
				LINE(xx2, yy1, xx2, yy2);
				LINE(xx2, yy2, xx1, yy2);
				LINE(xx1, yy2, xx1, yy1);
				
				LINE(r_x1, r_y1, r_x2, r_y1);
				LINE(r_x2, r_y1, r_x2, r_y2);
				LINE(r_x1, r_y2, r_x2, r_y2);
				LINE(r_x1, r_y1, r_x1, r_y2);
			}
			else
			{

				setcolor(DARKGRAY);

				//change_font(1);
				pageNo = 0;
				yyy1 = yy1;
				lineB = TRUE;
				nextR = TRUE;

                set_trans_blender(0, 0, 0, TRANSLUCENCY_PREVIEW);

				xxx1 = xx1;
				lineR = TRUE;
				nextC = TRUE;
                if ((dw>0) && (dh>0))
                {
                    while (nextC) //horizontally
                    {
                        xxx2 = xxx1 + dw;
                        if ((xxx2 - dl) >= (xx2 /* - 1*/))  //dr ?
                        {
                            if (xxx2 > xx2) lineR = FALSE;
                            xxx2 = xx2;
                            nextC = FALSE;
                        }

                        yyy1 = yy1;
                        nextR = TRUE;
                        lineB = TRUE;

                        while (nextR) //vertically
                        {
                            yyy2 = yyy1 + dh;
                            if ((yyy2 - db) >= (yy2 /* - 1*/))   //db ?
                            {
                                if (yyy2 > yy2) lineB = FALSE;
                                yyy2 = yy2;
                                nextR = FALSE;
                            }

                            set_mode_trans();
                            bar(xxx1, yyy1, xxx2, yyy2);
                            set_mode_solid();

                            LINE(xxx1, yyy1, xxx2, yyy1);  //top
                            if (lineR) LINE(xxx2, yyy1, xxx2, yyy2);  //right
                            if (lineB) LINE(xxx2, yyy2, xxx1, yyy2);  //bottom
                            LINE(xxx1, yyy2, xxx1, yyy1);  //left

                            pageNo++;
                            sprintf(page_no, "%d", pageNo);
                            txt_w = (WIDTH - 1) * strlen(page_no);
                            txt_x = xxx1 + 5;
                            txt_y = yyy1 + 5;
                            if ((txt_x + txt_w) > xx2)
                                txt_x = (xx2 - txt_w);
                            if ((txt_y + (HEIGHT - 4)) > yy2)
                                txt_y = (yy2 - (HEIGHT - 4));
                            moveto(txt_x, txt_y); // +(ED_INF_HEIGHT / 2) + 1);
                            outtext_r(page_no);

                            yyy1 = yyy2 - dt - db - dm;
                        }
                        xxx1 = xxx2 - dl - dr - dm;
                    }
                }
                else
                {
                    //error, page too small
                    set_mode_trans();
                    bar(r_x1, r_y1, r_x2, r_y2);
                    set_mode_solid();
                    LINE(r_x1, r_y1, r_x2, r_y1);  //top
                    LINE(r_x2, r_y1, r_x2, r_y2);  //right
                    LINE(r_x2, r_y2, r_x1, r_y2);  //bottom
                    LINE(r_x1, r_y2, r_x1, r_y1);  //left
                    moveto(r_x1+5, r_y1+5);
                    outtext_r("???");
                }

                set_trans_blender(0, 0, 0, TRANSLUCENCY);

			}
		}
	}
	else
	{
		setcolor(DARKGRAY);
		LINE(x1, y2, x2, y1);
	}

	show_mouse(screen);

	return;
}


static BOOL
get_config_param (T_Fstring key_name, T_Fstring ret_string)
/*-------------------------------------------------------*/
{
   int i, val_int;
   double val_double;
   char prn_conf_par[128];


   //strupr (key_name);
   utf8Upper(key_name);

   for (i = 0; i < no_prn_config_param; i++)
   {
      //strupr (&prn_config_param [i]);

      strcpy(prn_conf_par, prn_config_param [i]);
      utf8Upper(&prn_conf_par);

      if (stricmp (key_name, prn_conf_par) == 0)
      {
	   break;
      }
   }
   if (i >= no_prn_config_param)
   {
      return FALSE;
   }
  switch (i)
  {
     case Prn_density_draft:
	if ( sscanf ( ret_string, "%d", &val_int) == 1 )
	{
	  if (val_int > 0)
	    ptrs__config->density_draft = val_int;
	}
	break;
     case Prn_density_normal:
	if ( sscanf ( ret_string, "%d", &val_int) == 1 )
	{
	  if (val_int > 0)
	    ptrs__config->density_normal = val_int;
	}
	break;
     case Prn_density_double:
	if ( sscanf ( ret_string, "%d", &val_int) == 1 )
	{
	  if (val_int > 0)
	    ptrs__config->density_double = val_int;
	}
	break;
     case Prn_density_high:
	if ( sscanf ( ret_string, "%d", &val_int) == 1 )
	{
	  if (val_int > 0)
	    ptrs__config->density_high = val_int;
	}
	break;
	case Prn_density_super_high:
		if (sscanf(ret_string, "%d", &val_int) == 1)
		{
			if (val_int > 0)
				ptrs__config->density_super_high = val_int;
		}
		break;
    case Prn_l_luk:
	if ( sscanf ( ret_string, "%lf", &val_double) == 1 )  //lf
	{
	  if (val_double > 0)
	    ptrs__config->l_luk = val_double;
	}
	break;
     default:
	break;
   }
   return TRUE;
}


static BOOL get_config_prn (void)
/*-------------------------------*/
{
  BOOL retval;

  retval = TRUE;
  if (first == TRUE)
  {
    ptrs__config = &prn_config [0] ;
    get_config0PCX_prn ();
    retval = Get_Private_Profile_Strings ((T_Fstring)PRINTER_PCX, get_config_param);
	
	ptrs__config = &prn_config [1] ;
    get_config0WINDOWS_prn ();
    retval = Get_Private_Profile_Strings ((T_Fstring)PRINTER_WINDOWS, get_config_param);

	ptrs__config = &prn_config[2];
	get_config0PDF_prn();
	retval = Get_Private_Profile_Strings((T_Fstring)PRINTER_PDF, get_config_param);
  }
  return retval;
}


static void get_config0PCX_prn (void)
/*-------------------------------*/
{

  ptrs__config->density_draft = 75 ;

  ptrs__config->density_normal = 150 ;

  ptrs__config->density_double = 300 ;

  ptrs__config->density_high = 600 ;

  ptrs__config->density_super_high = 1200;

  ptrs__config->dot_width = WIDTH_DOT ;
  ptrs__config->l_luk= 0.5;
}

static void get_config0PDF_prn(void)
/*-------------------------------*/
{

	ptrs__config->density_draft = 75;

	ptrs__config->density_normal = 150;

	ptrs__config->density_double = 300;

	ptrs__config->density_high = 600;

	ptrs__config->density_super_high = 1200;

	ptrs__config->dot_width = WIDTH_DOT;
	ptrs__config->l_luk = 0.5;
}

static void get_config0WINDOWS_prn (void)
/*-------------------------------*/
{

  ptrs__config->density_draft = 75 ;

  ptrs__config->density_normal = 150 ;

  ptrs__config->density_double = 300 ;

  ptrs__config->density_high = 600 ;

  ptrs__config->density_super_high = 1200;

  ptrs__config->dot_width = WIDTH_DOT ;
  ptrs__config->l_luk= 0.5;
}



static BOOL
get_printer_date (T_Fstring key_name, T_Fstring ret_string)
/*-------------------------------------------------------*/
{
   int i;
   double double_val;
   int int_val;
   BOOL bool_val;

   //strupr (key_name);
   //utf8Upper(key_name);

   for (i = 0; i < no_prn_ini_date_param; i++)
   {
	  //utf8Upper(prn_ini_date_param[i]);
      if (stricmp (key_name, prn_ini_date_param [i]) == 0)
      {
	 break;
      }
   }
   if (i >= no_prn_ini_date_param)
   {
      return FALSE;
   }
  switch (i)
  {
     case Prn_type:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  if (int_val < PRN_PCX) int_val = PRN_PCX;
	  if (int_val > PRN_PDF) int_val = PRN_PDF;
	  prn_ini_date.prn_type= int_val;
	}
	break;
     case Prn_mode:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  if (int_val < PRN_MODE_EPSON) int_val = PRN_MODE_EPSON ;
	  if (int_val > PRN_MODE_IBM) int_val = PRN_MODE_IBM ;
	  prn_ini_date.prn_mode= int_val;
	}
	break;
	case Prn_roll:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  if (int_val < 0) int_val = 0 ;
	  if (int_val > 2) int_val = 2 ;
	  prn_ini_date.roll= int_val;
	}
	break;
     case Prn_color_mode:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  if (int_val < PRN_COLOR_KCMY) int_val = PRN_COLOR_KCMY ;
	  if (int_val > PRN_COLOR_GRAY) int_val = PRN_COLOR_GRAY ;
	  prn_ini_date.color_mode= int_val;
	}
	break;
     case Prn_condensed:
	if ( sscanf ( ret_string, "%d", &bool_val) == 1 )
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.condensed = !bool_val;
	}
	break;
     case Prn_density:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  if (int_val < D_DRAFT) int_val = D_DRAFT;
	  if (int_val > D_SUPER_HIGH) int_val = D_SUPER_HIGH;
	  prn_ini_date.density= int_val;
	}
	break;
     case Prn_rotation:
	if ( sscanf ( ret_string, "%d", &bool_val) == 1 )
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.rotation= bool_val;
	}
	break;
     case Prn_inversion:
	if ( sscanf ( ret_string, "%d", &bool_val) == 1 )
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.inversion= bool_val;
	}
	break;
	 case Prn_reflection:
	if (sscanf(ret_string, "%d", &bool_val) == 1)
	{
		if (bool_val != 0) bool_val = 1;
		prn_ini_date.reflection = bool_val;
	}
	break;
     case Prn_out_to_file:
	if ( sscanf ( ret_string, "%d", &bool_val) == 1 )
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.out_to_file= !bool_val;
	}
	break;
     case Prn_margin:
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
	{
	  if (double_val>= 0)
	    prn_ini_date.margin = double_val;
	}
	break;
     case Prn_line_thinest_width:
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
	{
	  if (double_val>= 0)
	    prn_ini_date.line_thinest_width= double_val;
	}
	break;	
     case Prn_line_thin_width:
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
	{
	  if (double_val>= 0)
	    prn_ini_date.line_thin_width= double_val;
	}
	break;
     case Prn_line_norm_width:
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
	{
	  if (double_val>= 0)
	    prn_ini_date.line_norm_width= double_val;
	}
	break;
     case Prn_line_thick_width:
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 ) //lf
	{
	  if (double_val>= 0)
	    prn_ini_date.line_thick_width= double_val;
	}
	break;
     case Prn_line_thickest_width:
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
	{
	  if (double_val>= 0)
	    prn_ini_date.line_thickest_width= double_val;
	}
	break;	
     case Prn_top_margin :
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
	{
	  if (double_val < 0) double_val = 0 ;
	  prn_ini_date.top_margin= double_val;
	}
	break;
	 case Prn_bottom_margin:
		 if (sscanf(ret_string, "%lf", &double_val) == 1)  //lf
		 {
			 if (double_val < 0) double_val = 0;
			 prn_ini_date.bottom_margin = double_val;
		 }
		 break;
     case Prn_left_margin :
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
	{
	  if (double_val < 0) double_val = 0 ;
	  prn_ini_date.left_margin = double_val;
	}
	break;
	 case Prn_right_margin:
		 if (sscanf(ret_string, "%lf", &double_val) == 1)  //lf
		 {
			 if (double_val < 0) double_val = 0;
			 prn_ini_date.right_margin = double_val;
		 }
		 break;
     case Prn_width_paper :
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
	{
	  if (double_val < 0) double_val = 0 ;
	  prn_ini_date.width_paper = double_val;
	}
	break;
      case Prn_width_paper_printer :
          if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
          {
              if (double_val < 0) double_val = 0 ;
              prn_ini_date.prn_width_paper = double_val;
          }
          break;
     case Prn_height_paper :
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
	{
	  if (double_val < 0) double_val = 0 ;
	  prn_ini_date.height_paper = double_val;
	}
	break;
      case Prn_height_paper_printer:
          if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
          {
              if (double_val < 0) double_val = 0 ;
              prn_ini_date.prn_height_paper = double_val;
          }
          break;
     case Prn_scale :
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
	{
	  if (double_val < .0001) double_val = .0001 ;
	  prn_ini_date.scale = double_val;
	}
	break;
	 case Prn_actual_size:
		 if (sscanf(ret_string, "%d", &bool_val) == 1)
		 {
			 if (bool_val != 0) bool_val = 1;
			 prn_ini_date.actual_size = bool_val;
		 }
		 break;
     case Prn_print_pause :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.print_pause = bool_val;
	}
	break;
     case Prn_change_page :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.change_page = bool_val;
	}
	break;
     case Prn_color_print :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.color_print = bool_val;
	}
	break;
	 case Prn_gray_print:
		 if (sscanf(ret_string, "%d", &bool_val) == 1)
		 {
			 if (bool_val != 0) bool_val = 1;
			 prn_ini_date.gray_print = bool_val;
		 }
		 break;
     case Prn_PCX_gray :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.PCX_gray = bool_val;
	}
	break;
	 case Prn_WINPRINT_DEF :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.WINPRINT_DEF = bool_val;
	}
	break;
	 case Prn_Image_Format:
		 strcpy(prn_ini_date.image_format, ret_string);
		 break;
     case Prn_set_page :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.set_page = bool_val;
	}
	break;
     case Prn_print_window :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.print_window = bool_val;
	}
	break;
     case Prn_print_sheet :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.print_sheet = bool_val;
	}
	break;
     case Prn_text_mode :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.text_mode = bool_val;
	}
	break;	
     case Prn_buforowanie :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.buforowanie = bool_val;
	}
   break;
     case Prn_kompresja :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.kompresja = bool_val;
	}
	break;
     case Prn_ploter :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.ploter = bool_val;
	}
	break;
	case Prn_oce :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.oce = bool_val;
	}
	break;
     case Prn_fixed_name :
	if ( sscanf ( ret_string, "%d", &bool_val) == 1)
	{
	  if (bool_val != 0) bool_val = 1 ;
	  prn_ini_date.fixed_name = bool_val;
	}
	break;
    case Prn_plik:
        strncpy (prn_ini_date.plik, ret_string, MAXPATH);
        prn_ini_date.plik [MAXPATH -1] = '\0';
   break;
     case Prn_port_num:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  if (int_val <= 0) int_val = 1;
	  if (int_val > 8) int_val = 8 ;
	  prn_ini_date.port_num = int_val;
	}
	break;
     case Prn_copies:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  if (int_val <= 0) int_val = 1;
	  if (int_val > 999) int_val = 999 ;
	  prn_ini_date.copies = int_val;
	}
	break;
   case Prn_thin_point_PCX:
	if ( sscanf ( ret_string, "%d", &int_val) == 1 )
	{
	  if (int_val < 0) int_val = 0;
	  if (int_val > 3) int_val = 3 ;
	  prn_ini_date.thin_point_PCX = int_val;
	}
	break;
     case Prn_Gray_Saturation:
	   if (sscanf(ret_string, "%d", &int_val) == 1)
	   {
		   if (int_val < 1) int_val = 1;
		   if (int_val > 3) int_val = 3;
		   prn_ini_date.gray_saturation = int_val;
	   }
	   break;
	case Prn_line_thick_correction_H:
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
	{
	  if (double_val < 0.0) double_val = 0.1;
	  if (double_val > 10.0) double_val = 10.0;
	  prn_ini_date.line_thick_correction_H = double_val;
	}
	break;
	case Prn_line_thick_correction_V:
	if ( sscanf ( ret_string, "%lf", &double_val) == 1 )  //lf
	{
	  if (double_val < 0.0) double_val = 0.1;
	  if (double_val > 10.0) double_val = 10.0;
	  prn_ini_date.line_thick_correction_V = double_val;
	}
	break;
	case Prn_Background:
		if (sscanf(ret_string, "%d", &bool_val) == 1)
		{
			if (bool_val != 0) bool_val = 1;
			prn_ini_date.background = bool_val;
		}
		break;
  }
  return TRUE;
}

static BOOL add_value (FILE *stru)
/*-------------------------------*/
{
  T_Fstring key_name;
  T_Fstring comment;
  int i;

   for (i = 0; i < no_prn_ini_date_param; i++)
   {
     key_name = prn_ini_date_param [i];
     comment = prn_ini_date_param_comment [i];
     switch (i)
     {
       case Prn_type:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.prn_type, comment))
	    return FALSE;
	  break;
       case Prn_mode:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.prn_mode, comment))
	    return FALSE;
	  break;
	   case Prn_roll:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.roll, comment))
	    return FALSE;
	  break;
       case Prn_color_mode:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.color_mode, comment))
	    return FALSE;
	  break;
       case Prn_condensed:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, !prn_ini_date.condensed, comment))
	    return FALSE;
	  break;
       case Prn_density:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.density, comment))
	    return FALSE;
	  break;
       case Prn_rotation:
	  if (EOF == fprintf (stru, "%s=%d \t\t%s\n",
			key_name, prn_ini_date.rotation, comment))
	    return FALSE;
	  break;
       case Prn_inversion:
	  if (EOF == fprintf (stru, "%s=%d \t\t%s\n",
			key_name, prn_ini_date.inversion, comment))
	    return FALSE;
	  break;
	   case Prn_reflection:
		   if (EOF == fprintf(stru, "%s=%d \t\t%s\n",
			   key_name, prn_ini_date.reflection, comment))
			   return FALSE;
		   break;
       case Prn_out_to_file:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, !prn_ini_date.out_to_file, comment))
	    return FALSE;
	  break;
       case Prn_margin:
	  if (EOF == fprintf (stru, "%s=%.0lf\t\t%s\n",
			key_name, prn_ini_date.margin, comment))
	    return FALSE;
	break;
       case Prn_line_thinest_width:
	  if (EOF == fprintf (stru, "%s=%.2lf\t\t%s\n",
			key_name, prn_ini_date.line_thinest_width, comment))
	    return FALSE;
	break;	
       case Prn_line_thin_width:
	  if (EOF == fprintf (stru, "%s=%.2lf\t\t%s\n",
			key_name, prn_ini_date.line_thin_width, comment))
	    return FALSE;
	break;
     case Prn_line_norm_width:
	  if (EOF == fprintf (stru, "%s=%.2lf\t\t%s\n",
			key_name, prn_ini_date.line_norm_width, comment))
	    return FALSE;
	break;
     case Prn_line_thick_width:
	  if (EOF == fprintf (stru, "%s=%.2lf\t\t%s\n",
			key_name, prn_ini_date.line_thick_width, comment))
	    return FALSE;
	break;
     case Prn_line_thickest_width:
	  if (EOF == fprintf (stru, "%s=%.2lf\t\t%s\n",
			key_name, prn_ini_date.line_thickest_width, comment))
	    return FALSE;
	break;
     case Prn_top_margin:
	  if (EOF == fprintf (stru, "%s=%.1lf\t\t%s\n",
			key_name, prn_ini_date.top_margin, comment))
	    return FALSE;
	break;
	 case Prn_bottom_margin:
		 if (EOF == fprintf(stru, "%s=%.1lf\t\t%s\n",
			 key_name, prn_ini_date.bottom_margin, comment))
			 return FALSE;
		 break;
     case Prn_left_margin:
	  if (EOF == fprintf (stru, "%s=%.1lf\t\t%s\n",
			key_name, prn_ini_date.left_margin, comment))
	    return FALSE;
	break;
	 case Prn_right_margin:
		 if (EOF == fprintf(stru, "%s=%.1lf\t\t%s\n",
			 key_name, prn_ini_date.right_margin, comment))
			 return FALSE;
		 break;
     case Prn_width_paper:
	  if (EOF == fprintf (stru, "%s=%.1lf\t\t%s\n",
			key_name, prn_ini_date.width_paper, comment))
	    return FALSE;
	break;
     case Prn_height_paper:
	  if (EOF == fprintf (stru, "%s=%.1lf\t\t%s\n",
			key_name, prn_ini_date.height_paper, comment))
	    return FALSE;
	break;
	 case Prn_width_paper_printer:
		 if (EOF == fprintf(stru, "%s=%.1lf\t\t%s\n",
			 key_name, prn_ini_date.prn_width_paper, comment))
			 return FALSE;
		 break;
	 case Prn_height_paper_printer:
		 if (EOF == fprintf(stru, "%s=%.1lf\t\t%s\n",
			 key_name, prn_ini_date.prn_height_paper, comment))
			 return FALSE;
		 break;
     case Prn_scale:
	  if (EOF == fprintf (stru, "%s=%.3lf\t\t%s\n",
			key_name, prn_ini_date.scale, comment))
	    return FALSE;
	break;
	 case Prn_actual_size:
		 if (EOF == fprintf(stru, "%s=%d\t\t%s\n",
			 key_name, prn_ini_date.actual_size, comment))
			 return FALSE;
		 break;
     case Prn_print_pause:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.print_pause, comment))
	    return FALSE;
	break;
     case Prn_change_page:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.change_page, comment))
	    return FALSE;
	break;
     case Prn_color_print:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.color_print, comment))
	    return FALSE;
	break;
	 case Prn_gray_print:
		 if (EOF == fprintf(stru, "%s=%d\t\t%s\n",
			 key_name, prn_ini_date.gray_print, comment))
			 return FALSE;
		 break;
    case Prn_PCX_gray:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.PCX_gray, comment))
	    return FALSE;
	break;
	 case Prn_WINPRINT_DEF:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.WINPRINT_DEF, comment))
	    return FALSE;
	break;
	 case Prn_Image_Format:
		 if (EOF == fprintf(stru, "%s=%s\t\t%s\n",
			 key_name, prn_ini_date.image_format, comment))
			 return FALSE;
		 break;
     case Prn_set_page:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.set_page, comment))
	    return FALSE;
	break;
       case Prn_print_window:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.print_window, comment))
	    return FALSE;
	break;
     case Prn_print_sheet:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.print_sheet, comment))
	    return FALSE;
	break;
       case Prn_buforowanie:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.buforowanie, comment))
	    return FALSE;
   break;
     case Prn_kompresja:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.kompresja, comment))
	    return FALSE;
   break;
    case Prn_ploter:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.ploter, comment))
	    return FALSE;
   break;
   case Prn_oce:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.oce, comment))
	    return FALSE;
   break;
     case Prn_fixed_name:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.fixed_name, comment))
	    return FALSE;
	break;
      case Prn_plik:

	  if (EOF == fprintf (stru, "%s=%s\t\t%s\n",
			key_name, prn_ini_date.plik, comment))

	    return FALSE;
	break;
       case Prn_text_mode:
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.text_mode, comment))
	    return FALSE;
	break;	
       case Prn_port_num :
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.port_num, comment))
	    return FALSE;
	  break;
       case Prn_copies :
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.copies, comment))
	    return FALSE;
	  break;
       case Prn_thin_point_PCX :
	  if (EOF == fprintf (stru, "%s=%d\t\t%s\n",
			key_name, prn_ini_date.thin_point_PCX, comment))
	    return FALSE;
	  break;
	   case Prn_Gray_Saturation:
		   if (EOF == fprintf(stru, "%s=%d\t\t%s\n",
			   key_name, prn_ini_date.gray_saturation, comment))
			   return FALSE;
		   break;
	  case Prn_line_thick_correction_H:
	  if (EOF == fprintf (stru, "%s=%.2lf\t\t%s\n",
			key_name, prn_ini_date.line_thick_correction_H, comment))
	    return FALSE;
	break;
	case Prn_line_thick_correction_V:
	  if (EOF == fprintf (stru, "%s=%.2lf\t\t%s\n",
			key_name, prn_ini_date.line_thick_correction_V, comment))
	    return FALSE;
	break;
	case Prn_Background:
		if (EOF == fprintf(stru, "%s=%d \t\t%s\n",
			key_name, prn_ini_date.background, comment))
			return FALSE;
		break;
       default:
	 break;
     }
   }
   if (EOF == fprintf (stru, "\n"))
    return FALSE;
   return TRUE;
}

static BOOL get_prn_dlg_control (void)
/*------------------------------------*/
{
  char drive [MAXDRIVE]="" ;
  char dir [MAXDIR]="" ;
  char file [MAXFILE]="" ;
  char ext [MAXEXT]="" ;
  int flags ;

  prn_ini_date.condensed = !Get_Check_Button (&printer_dlg, BUT_CONDENSED_Y) ;
  prn_ini_date.rotation = Get_Check_Button ( &printer_dlg, BUT_ROTATION) ;
  prn_ini_date.inversion = Get_Check_Button ( &printer_dlg, BUT_INVERSION) ;
  prn_ini_date.reflection = Get_Check_Button(&printer_dlg, BUT_REFLECTION);
  prn_ini_date.color_print = !Get_Check_Button(&printer_dlg, BUT_BW_PRINT);
  prn_ini_date.gray_print = Get_Check_Button ( &printer_dlg, BUT_GRAY_PRINT) ;
  prn_ini_date.PCX_gray = Get_Check_Button ( &printer_dlg, BUT_PCX_GRAY) ;
  prn_ini_date.print_window = Get_Check_Button ( &printer_dlg, BUT_PRINT_WINDOW) ;
  prn_ini_date.print_sheet = Get_Check_Button ( &printer_dlg, BUT_PRINT_SHEET) ;
  prn_ini_date.actual_size = Get_Check_Button(&printer_dlg, BUT_ACTUAL_SIZE);
  prn_ini_date.background = Get_Check_Button(&printer_dlg, BUT_BACKGROUND);

  
  if (Get_Check_Button ( &printer_dlg, BUT_DENSITY_DRAFT))
  prn_ini_date.density = D_DRAFT;
  else if (Get_Check_Button ( &printer_dlg, BUT_DENSITY_NORMAL))
  prn_ini_date.density = D_NORMAL;
  else if (Get_Check_Button ( &printer_dlg, BUT_DENSITY_DOUBLE))
  prn_ini_date.density = D_DOUBLE;
  else if (Get_Check_Button ( &printer_dlg, BUT_DENSITY_HIGH))
  prn_ini_date.density = D_HIGH;
  else if (Get_Check_Button(&printer_dlg, BUT_DENSITY_SUPER_HIGH))
	  prn_ini_date.density = D_SUPER_HIGH;

  if (Get_Check_Button(&printer_dlg, BUT_GRAY_1))
	  prn_ini_date.gray_saturation = 1;
  else if (Get_Check_Button(&printer_dlg, BUT_GRAY_2))
	  prn_ini_date.gray_saturation = 2;
  if (Get_Check_Button(&printer_dlg, BUT_GRAY_3))
	  prn_ini_date.gray_saturation = 3;

  if (Get_Check_Button ( &printer_dlg, BUT_PRINTER_PCX))
  prn_ini_date.prn_type = PRN_PCX;

  if (Get_Check_Button(&printer_dlg, BUT_PRINTER_PDF))
	  prn_ini_date.prn_type = PRN_PDF;
  
  else if (Get_Check_Button ( &printer_dlg, BUT_PRINTER_WINDOWS))
  prn_ini_date.prn_type = PRN_WINDOWS;
 

  strcpy(prn_ini_date.image_format, listbox[LS_IMAGE_FORMAT].txt[listbox[LS_IMAGE_FORMAT].poz]);
  prn_ini_date.image_format_i= listbox[LS_IMAGE_FORMAT].foff + listbox[LS_IMAGE_FORMAT].poz;

  prn_ini_date.first_page = 1 ;
  if (prn_ini_date.condensed == FALSE)
  {
    if ((sscanf ( sz_page, "%d", &prn_ini_date.first_page) < 1) ||
	(prn_ini_date.first_page < 1))
    {
      ErrList (67) ;
      return FALSE;
    }
  }
  prn_ini_date.first_page-- ;

  if ((sscanf(sz_margin, "%f", &prn_ini_date.margin) < 1) ||
	  (prn_ini_date.margin < 0) ||

	  (sscanf(sz_line_thinest_width, "%f", &prn_ini_date.line_thinest_width) < 1) ||
	  (prn_ini_date.line_thinest_width < 0) ||

	  (sscanf(sz_line_thin_width, "%f", &prn_ini_date.line_thin_width) < 1) ||
	  (prn_ini_date.line_thin_width < 0) ||

	  (sscanf(sz_line_norm_width, "%f", &prn_ini_date.line_norm_width) < 1) ||
	  (prn_ini_date.line_norm_width < 0) ||

	  (sscanf(sz_line_thick_width, "%f", &prn_ini_date.line_thick_width) < 1) ||
	  (prn_ini_date.line_thick_width < 0) ||

	  (sscanf(sz_line_thickest_width, "%f", &prn_ini_date.line_thickest_width) < 1) ||
	  (prn_ini_date.line_thickest_width < 0) ||

	  (sscanf(sz_top_margin, "%f", &prn_ini_date.top_margin) < 1) ||
	  (prn_ini_date.top_margin < 0) ||

	  (sscanf(sz_bottom_margin, "%f", &prn_ini_date.bottom_margin) < 1) ||
	  (prn_ini_date.bottom_margin < 0) ||

	  (sscanf(sz_left_margin, "%f", &prn_ini_date.left_margin) < 1) ||
	  (prn_ini_date.left_margin < 0) ||

	  (sscanf(sz_right_margin, "%f", &prn_ini_date.right_margin) < 1) ||
	  (prn_ini_date.right_margin < 0) ||

	  (sscanf(sz_scale, "%f", &prn_ini_date.scale) < 1) ||
	  (prn_ini_date.scale <= 0) ||

	  (sscanf(sz_port_num, "%d", &prn_ini_date.port_num) < 1) ||
	  (prn_ini_date.port_num <= 0) || (prn_ini_date.port_num > 8) ||

	  (sscanf(sz_copies, "%d", &prn_ini_date.copies) < 1) ||
	  (prn_ini_date.copies <= 0) || (prn_ini_date.copies > 999) ||

	  (sscanf(sz_thin_point_PCX, "%d", &prn_ini_date.thin_point_PCX) < 1) ||
	  (prn_ini_date.thin_point_PCX < 0) || (prn_ini_date.thin_point_PCX > 3) ||

	  (sscanf(sz_line_thick_correction_H, "%f", &prn_ini_date.line_thick_correction_H) < 1) ||
	  (prn_ini_date.line_thick_correction_H <= 0.1) || (prn_ini_date.line_thick_correction_H > 10.0) ||

	  (sscanf(sz_line_thick_correction_V, "%f", &prn_ini_date.line_thick_correction_V) < 1) ||
	  (prn_ini_date.line_thick_correction_V <= 0.1) || (prn_ini_date.line_thick_correction_V > 10.0))

  {
    ErrList (55) ;
    return FALSE;
  }

  if ((prn_ini_date.prn_type != PRN_WINDOWS)
      #ifdef LINUX
  || (prn_ini_date.prn_type == PRN_WINDOWS)
      #endif
          )
  {
	  if ((sscanf(sz_width_paper, "%f", &prn_ini_date.width_paper) < 1) ||
		  (prn_ini_date.width_paper <= 0) ||

		  (sscanf(sz_height_paper, "%f", &prn_ini_date.height_paper) < 1) ||
		  (prn_ini_date.height_paper <= 0))

	  {
		  ErrList(55);
		  return FALSE;
	  }
  }

  if (listbox[2].txt[listbox[2].foff + listbox[2].poz] != NULL)
	  strncpy(&prn_ini_date.printer, listbox[2].txt[listbox[2].foff + listbox[2].poz], 32);
  else strcpy(&prn_ini_date.printer, "");

  flags = fnsplit (prn_file, drive, dir, file, ext);
  if (flags & WILDCARDS)
  {
     ErrList (28) ;
     return FALSE;
  }
  return TRUE;
}

static int proc_dlg_prn_ini_date( int n)
/*-------------------------------------*/
{
  int ret = Dlg_Ret_Val_Continue;
  int check1, check2, check3, check4, prn_change;
  char image_file_name[MAXPATH];
  int paper_format;
  PRIV_PRN_WINDOW  *priv_prn_window;
  BOOL pro_set, pro_set1;


 int img_formats[] = { SAVE_BMP, SAVE_PNG, SAVE_JPEG, SAVE_TIFF, SAVE_PCX, SAVE_EPS, SAVE_PDF };
 char paper_format_x[NoPaperFormat][6] = {"0","297","210","420","297","594","420","841","594","1189","841","250","176","500","353","707","500","1000","707","1414","1000","279.4","215.9","431.8","279.4","559","432","864","559","1118","864","215.9","215.9"};
 char paper_format_y[NoPaperFormat][6]= {"0","210","297","297","420","420","594","594","841","841","1189","176","250","353","500","500","707","707","1000","1000","1414","215.9","279.4","279.4","431.8","432","559","559","864","864","1118","279.4","355.6"};

  switch (n)
  {
    case BUT_OK:
		if (get_prn_dlg_control() == TRUE)
		{
			if ((prn_ini_date.prn_type == PRN_PCX) || (prn_ini_date.prn_type == PRN_PDF))
			{
				if (prn_ini_date.prn_type == PRN_PDF)
				{
					if (Load_File(&image_file_name, img_formats[6], TRUE) == FALSE)
					{
						ret = Dlg_Ret_Val_Cancel;
						break;
					}
				}
				else if (Load_File(&image_file_name, img_formats[prn_ini_date.image_format_i], TRUE) == FALSE)
				{
					ret = Dlg_Ret_Val_Cancel;
					break;
				}
				strcpy(prn_file, image_file_name);
				ret = Dlg_Ret_Val_OK;
			}
			else ret = Dlg_Ret_Val_OK;
		}
      break;
    case BUT_CANCEL:
      ret = Dlg_Ret_Val_Cancel;
      break;
    case BUT_ZAPIS:
      if (get_prn_dlg_control () == TRUE)
      {
		ret = Dlg_Ret_Val_Zapis_Cfg;
      }
      break; 
    case BUT_ODCZYT:
      ret = Dlg_Ret_Val_Odczyt_Cfg;
      break;
    case BUT_PRINT_WINDOW:
          check1 = Get_Check_Button(&printer_dlg, BUT_PRINT_WINDOW);
          if (check1)
          {
              priv_prn_window=get_priv_prn_window();
              prn_ini_date.all_sheet = FALSE;
              prn_ini_date.xp =(float)priv_prn_window->xp;               /* okno do plotowania*/
              prn_ini_date.yp =(float)priv_prn_window->yp;               /* okno do plotowania*/
              prn_ini_date.xk =(float)priv_prn_window->xk;               /* okno do plotowania*/
              prn_ini_date.yk =(float)priv_prn_window->yk;
          }
          else
          {
              prn_ini_date.all_sheet = TRUE;
              prn_ini_date.xp = (float) Xmin;               /* okno do plotowania*/
              prn_ini_date.yp = (float) Ymin;               /* okno do plotowania*/
              prn_ini_date.xk = (float) Xmax;               /* okno do plotowania*/
              prn_ini_date.yk = (float) Ymax;
          }
          mysprintf(&sz_range_x1,prn_ini_date.xp);
          mysprintf(&sz_range_y1,prn_ini_date.yp);
          mysprintf(&sz_range_x2,prn_ini_date.xk);
          mysprintf(&sz_range_y2,prn_ini_date.yk);

          edit_p[16].txt=sz_range_x1;
          edit_p[17].txt=sz_range_y1;
          edit_p[18].txt=sz_range_x2;
          edit_p[19].txt=sz_range_y2;

          draw_input_line(&edit_p[16]);
          draw_input_line(&edit_p[17]);
          draw_input_line(&edit_p[18]);
          draw_input_line(&edit_p[19]);

          Set_Enable_Button(&printer_dlg, BUT_SET_RANGE, check1);
          draw_button(&buttons_p[BUT_SET_RANGE-1]);
          //Preview(x1_prev, y1_prev, x2_prev, y2_prev, x0_prev, y0_prev);
          draw_dlg_prev(&printer_dlg);
          break;
    case BUT_PRINT_SHEET:

          draw_dlg_prev(&printer_dlg);
          break;
	case BUT_ROTATION:
	case BUT_INVERSION:
	case BUT_REFLECTION:
	case BUT_BACKGROUND:
		draw_dlg_prev(&printer_dlg);
		break;
	case BUT_BW_PRINT:
		check1 = Get_Check_Button(&printer_dlg, BUT_GRAY_PRINT);
		check2 = Get_Check_Button(&printer_dlg, BUT_BW_PRINT);
		if (check1 && check2)
		{
			Set_Check_Button(&printer_dlg, BUT_GRAY_PRINT, 0);
			////draw_dlg(&printer_dlg);
		}
		draw_dlg_prev(&printer_dlg);
		break;
	case BUT_GRAY_PRINT:
		check1 = Get_Check_Button(&printer_dlg, BUT_GRAY_PRINT);
		check2 = Get_Check_Button(&printer_dlg, BUT_BW_PRINT);
		if (check1 && check2)
		{
			Set_Check_Button(&printer_dlg, BUT_BW_PRINT, 0);
			////draw_dlg(&printer_dlg);
		}
		draw_dlg_prev(&printer_dlg);
		break;
    case BUT_CONDENSED_Y:
    case BUT_CONDENSED_N:
	  check1 = Get_Check_Button(&printer_dlg, BUT_CONDENSED_Y);
      Check_Radio_Button ( &printer_dlg, BUT_CONDENSED_Y, BUT_CONDENSED_N,
	  n == BUT_CONDENSED_Y ? BUT_CONDENSED_Y : BUT_CONDENSED_N, TRUE);
	 
      if (n == BUT_CONDENSED_Y)
      {
		  Set_Enable_Edit(&printer_dlg, IL_SCALE, 1);
		  Set_Enable_Edit(&printer_dlg, IL_PAGE, 1);
		  Set_Enable_Edit(&printer_dlg, IL_MARGIN, 1);
      }
	  else
	  {
		  Set_Enable_Edit(&printer_dlg, IL_SCALE, 0);
		  Set_Enable_Edit(&printer_dlg, IL_PAGE, 0);
		  Set_Enable_Edit(&printer_dlg, IL_MARGIN, 0);
	  }

	  check2 = Get_Check_Button(&printer_dlg, BUT_CONDENSED_Y);
	  draw_dlg_prev(&printer_dlg);
      ret = Dlg_Ret_Val_Continue;
      break;
	case BUT_ACTUAL_SIZE:
		check1 = Get_Check_Button(&printer_dlg, BUT_ACTUAL_SIZE);
        prn_ini_date_actual_size_bak = check1;
		
		Set_Enable_Edit(&printer_dlg, IL_PAGE, !check1);
		Set_Enable_Edit(&printer_dlg, IL_MARGIN, !check1);
		Set_Enable_Edit(&printer_dlg, IL_WIDTH_PAPER, !check1);
		Set_Enable_Edit(&printer_dlg, IL_HEIGHT_PAPER, !check1);

		draw_dlg_prev(&printer_dlg);
		ret = Dlg_Ret_Val_Continue;
		break;
 
    case BUT_DENSITY_DRAFT:
    case BUT_DENSITY_NORMAL:
    case BUT_DENSITY_DOUBLE:
    case BUT_DENSITY_HIGH:
	case BUT_DENSITY_SUPER_HIGH:
      Check_Radio_Button ( &printer_dlg, BUT_DENSITY_DRAFT, BUT_DENSITY_SUPER_HIGH,
	n , TRUE);
      ret = Dlg_Ret_Val_Continue;
      break;
    case BUT_PRINTER_PCX:
	case BUT_PRINTER_PDF:
    case BUT_PRINTER_WINDOWS:
	  check1 = Get_Check_Button(&printer_dlg, BUT_PRINTER_WINDOWS);
	  check3 = Get_Check_Button(&printer_dlg, BUT_PRINTER_PDF);
      Check_Radio_Button ( &printer_dlg, BUT_PRINTER_PCX, BUT_PRINTER_PDF, n , TRUE);

	  if (n == BUT_PRINTER_WINDOWS)
	  {

#ifndef LINUX
          pro_set=0;
		  pro_set1 = 1;
#else
          pro_set=1;
		  pro_set1 = 1;
#endif
		  sprintf(edit_p[0].txt, "%.1f", prn_ini_date.prn_width_paper);
		  sprintf(edit_p[1].txt, "%.1f", prn_ini_date.prn_height_paper);

		Set_Enable_Button(&printer_dlg, BUT_DENSITY_DRAFT, pro_set);
		Set_Enable_Button(&printer_dlg, BUT_DENSITY_NORMAL, pro_set);
		Set_Enable_Button(&printer_dlg, BUT_DENSITY_DOUBLE, pro_set);
		Set_Enable_Button(&printer_dlg, BUT_DENSITY_HIGH, pro_set);
		Set_Enable_Button(&printer_dlg, BUT_DENSITY_SUPER_HIGH, pro_set);

		Set_Enable_Edit(&printer_dlg, IL_PAGE, pro_set);
		Set_Enable_Edit(&printer_dlg, IL_MARGIN, 1);
		Set_Enable_Edit(&printer_dlg, IL_WIDTH_PAPER, pro_set);
		Set_Enable_Edit(&printer_dlg, IL_HEIGHT_PAPER, pro_set);

		listbox[LS_IMAGE_FORMAT].enable = 0;

		Set_Enable_Button(&printer_dlg, BUT_BW_PRINT, 1);
		Set_Enable_Button(&printer_dlg, BUT_GRAY_PRINT, 1);
		Set_Enable_Button(&printer_dlg, BUT_PCX_GRAY, 1);

		Set_Enable_Button(&printer_dlg, BUT_ACTUAL_SIZE, 0);
        prn_ini_date_actual_size_bak = Get_Check_Button(&printer_dlg, BUT_ACTUAL_SIZE);
        Set_Check_Button(&printer_dlg, BUT_ACTUAL_SIZE, 0);
        prn_ini_date.actual_size=0;

		listbox[1].enable = pro_set;
        listbox[2].enable = pro_set1;
	  }	  
      else if (n != BUT_PRINTER_WINDOWS)
      {

		sprintf(edit_p[0].txt, "%.1f", prn_ini_date.width_paper);
		sprintf(edit_p[1].txt, "%.1f", prn_ini_date.height_paper);

		Set_Enable_Button(&printer_dlg, BUT_DENSITY_DRAFT, 1);
		Set_Enable_Button(&printer_dlg, BUT_DENSITY_NORMAL, 1);
		Set_Enable_Button(&printer_dlg, BUT_DENSITY_DOUBLE, 1);
		Set_Enable_Button(&printer_dlg, BUT_DENSITY_HIGH, 1);
		Set_Enable_Button(&printer_dlg, BUT_DENSITY_SUPER_HIGH, 1);

		Set_Enable_Edit(&printer_dlg, IL_PAGE, 1);
		Set_Enable_Edit(&printer_dlg, IL_MARGIN, 1);
		Set_Enable_Edit(&printer_dlg, IL_WIDTH_PAPER, 1);
		Set_Enable_Edit(&printer_dlg, IL_HEIGHT_PAPER, 1);

		if (n == BUT_PRINTER_PDF) listbox[LS_IMAGE_FORMAT].enable = 0;
		else listbox[LS_IMAGE_FORMAT].enable = 1;

		Set_Enable_Button(&printer_dlg, BUT_BW_PRINT, 1);
		Set_Enable_Button(&printer_dlg, BUT_GRAY_PRINT, 1);
		Set_Enable_Button(&printer_dlg, BUT_PCX_GRAY, 1);

		Set_Enable_Button(&printer_dlg, BUT_ACTUAL_SIZE, 1);
        Set_Check_Button(&printer_dlg, BUT_ACTUAL_SIZE, prn_ini_date_actual_size_bak);

		listbox[1].enable = 1;
        listbox[2].enable = 0;
		
      }
	  
	  //if wWindows printer changed 
	  check2 = Get_Check_Button(&printer_dlg, BUT_PRINTER_WINDOWS);
	  check4 = Get_Check_Button(&printer_dlg, BUT_PRINTER_PDF);

      check4 = Get_Check_Button(&printer_dlg, BUT_ACTUAL_SIZE);

      Draw_ComboBox(&combobox[2]);
      Draw_ComboBox(&combobox[0]);

	  draw_dlg_prev(&printer_dlg);

      ret = Dlg_Ret_Val_Continue;
      break;

	case IL_WIDTH_PAPER:
	case IL_HEIGHT_PAPER:
	case IL_MARGIN:
	case IL_SCALE:
	case IL_LEFT_MARGIN:
	case IL_TOP_MARGIN:
	case IL_RIGHT_MARGIN:
	case IL_BOTTOM_MARGIN:
		draw_dlg_prev(&printer_dlg);
		ret = Dlg_Ret_Val_Continue;
		break;
	case BUT_GRAY_1:
	case BUT_GRAY_2:
	case BUT_GRAY_3:
		Check_Radio_Button(&printer_dlg, BUT_GRAY_1, BUT_GRAY_3, n, TRUE);
		check1 = Get_Check_Button(&printer_dlg, BUT_GRAY_PRINT);
        check2 = Get_Check_Button(&printer_dlg, BUT_BW_PRINT);
		if ((check1) || (check2)) draw_dlg_prev(&printer_dlg);
		ret = Dlg_Ret_Val_Continue;
		break;
	case LS_PAPER_FORMAT:
		paper_format = listbox[1].foff + listbox[1].poz;
		if (paper_format > 0)
		{
			strcpy(edit_p[0].txt, paper_format_x[paper_format]);
			strcpy(edit_p[1].txt, paper_format_y[paper_format]);
            if (Get_Check_Button(&printer_dlg, BUT_PRINTER_WINDOWS))
            {
                sscanf ( paper_format_x[paper_format], "%f", &prn_ini_date.prn_width_paper);
                sscanf ( paper_format_y[paper_format], "%f", &prn_ini_date.prn_height_paper);
            }
            else
            {
                sscanf ( paper_format_x[paper_format], "%f", &prn_ini_date.width_paper);
                sscanf ( paper_format_y[paper_format], "%f", &prn_ini_date.height_paper);
            }
			draw_dlg_prev(&printer_dlg);
			ret = Dlg_Ret_Val_Continue;
		}
		break;
      case BUT_SET_RANGE:
          if (get_prn_dlg_control() == TRUE) {
              ret = Dlg_Ret_Set_Range;
          }
          break;
      case LS_PRINTER_TYPE:
          {
            char params[128];
            DWORD runcode;
            int poz = combobox[2].listbox->poz + combobox[2].listbox->foff;
            char *printer_name = combobox[2].listbox->txt[poz];

            //setting printer as default
            //getting page size of default

#ifndef LINUX
          //TO DO
			prn_change = SetDefaultPrinter_(printer_name);
		  
#else
            //lpoptions -p printer_name
          sprintf(params, "-p %s -d %s", printer_name, printer_name);
          runcode = SystemSilent("lpoptions",params);
#endif
            int kk = Print2Page(1);
#ifndef LINUX
          sprintf(edit_p[0].txt, "%.1f", (float)GetPrintPageWidth());
		  sprintf(edit_p[1].txt, "%.1f", (float)GetPrintPageHeight());
#else
          sprintf(edit_p[0].txt, "%.1f", cups_prn_width_paper);
          sprintf(edit_p[1].txt, "%.1f", cups_prn_height_paper);
#endif
          }
		  Draw_Input_Line(&edit_p[0]);
		  Draw_Input_Line(&edit_p[1]);
		  draw_dlg_prev(&printer_dlg);
		  ret = Dlg_Ret_Val_Continue;
          break;
    default:
      ret = Dlg_Ret_Val_Continue;
      break;
  }
  return ret;
}


static void init_printer_dlg_control (void)
/*----------------------------------------*/
{
	int i;
    BOOL pro_set, pro_set1;

  Check_Radio_Button ( &printer_dlg, BUT_CONDENSED_Y, BUT_CONDENSED_N,
    prn_ini_date.condensed == TRUE ? BUT_CONDENSED_N : BUT_CONDENSED_Y, FALSE);


  Set_Check_Button (&printer_dlg, BUT_ROTATION, prn_ini_date.rotation) ;
  Set_Check_Button (&printer_dlg, BUT_INVERSION, prn_ini_date.inversion) ;
  Set_Check_Button(&printer_dlg, BUT_REFLECTION, prn_ini_date.reflection);

  Check_Radio_Button ( &printer_dlg, BUT_DENSITY_DRAFT, BUT_DENSITY_SUPER_HIGH,
    BUT_DENSITY_DRAFT + prn_ini_date.density - 1, FALSE);
  Check_Radio_Button ( &printer_dlg, BUT_PRINTER_PCX, BUT_PRINTER_PDF,
    BUT_PRINTER_PCX + prn_ini_date.prn_type, FALSE);

  Check_Radio_Button(&printer_dlg, BUT_GRAY_1, BUT_GRAY_3,
	  BUT_GRAY_1 + prn_ini_date.gray_saturation - 1, FALSE);

  Set_Check_Button (&printer_dlg, BUT_BW_PRINT, !prn_ini_date.color_print) ;
  Set_Check_Button(&printer_dlg, BUT_GRAY_PRINT, prn_ini_date.gray_print);
  Set_Check_Button (&printer_dlg, BUT_PCX_GRAY, prn_ini_date.PCX_gray) ;

  Set_Check_Button (&printer_dlg, BUT_PRINT_WINDOW, prn_ini_date.print_window) ;
  Set_Check_Button (&printer_dlg, BUT_PRINT_SHEET, prn_ini_date.print_sheet) ;
  Set_Check_Button (&printer_dlg,  BUT_ACTUAL_SIZE, prn_ini_date.actual_size);

  Set_Check_Button(&printer_dlg, BUT_BACKGROUND, prn_ini_date.background);

  Set_Enable_Button(&printer_dlg, BUT_SET_RANGE, prn_ini_date.print_window);

  sprintf(sz_copies, "%d", prn_ini_date.copies);
  sprintf(sz_thin_point_PCX, "%d", prn_ini_date.thin_point_PCX);
  sprintf(sz_line_thick_correction_H, "%.3f", prn_ini_date.line_thick_correction_H);
  sprintf(sz_line_thick_correction_V, "%.3f", prn_ini_date.line_thick_correction_V);
  sprintf(sz_page, "%d", prn_ini_date.first_page + 1);
  sprintf(sz_margin, "%.0f", prn_ini_date.margin);
  sprintf(sz_line_thinest_width, "%.2f", prn_ini_date.line_thinest_width);
  sprintf(sz_line_thin_width, "%.2f", prn_ini_date.line_thin_width);
  sprintf(sz_line_norm_width, "%.2f", prn_ini_date.line_norm_width);
  sprintf(sz_line_thick_width, "%.2f", prn_ini_date.line_thick_width);
  sprintf(sz_line_thickest_width, "%.2f", prn_ini_date.line_thickest_width);
  sprintf(sz_top_margin, "%.1f", prn_ini_date.top_margin);
  sprintf(sz_bottom_margin, "%.1f", prn_ini_date.bottom_margin);
  sprintf(sz_left_margin, "%.1f", prn_ini_date.left_margin);
  sprintf(sz_right_margin, "%.1f", prn_ini_date.right_margin);
  sprintf(sz_width_paper, "%.1f", prn_ini_date.width_paper);
  sprintf(sz_height_paper, "%.1f", prn_ini_date.height_paper);
  sprintf(sz_scale, "%.2f", prn_ini_date.scale);
  sprintf(sz_port_num, "%d", prn_ini_date.port_num);
 
  
  Set_Enable_Edit(&printer_dlg, IL_SCALE, 0);
  Set_Enable_Edit(&printer_dlg, IL_PAGE, 0) ;
  if (prn_ini_date.condensed != TRUE)
  {
	  Set_Enable_Edit(&printer_dlg, IL_SCALE, 1) ;
	  Set_Enable_Edit(&printer_dlg, IL_PAGE, 1) ;
  }
  
  if (prn_ini_date.prn_type == PRN_WINDOWS)
  {
#ifndef LINUX
      pro_set=0;
	  pro_set1 = 1;
#else
      pro_set=1;
	  pro_set1 = 1;
#endif
	  Set_Enable_Button(&printer_dlg, BUT_DENSITY_DRAFT, pro_set);
	  Set_Enable_Button(&printer_dlg, BUT_DENSITY_NORMAL, pro_set);
	  Set_Enable_Button(&printer_dlg, BUT_DENSITY_DOUBLE, pro_set);
	  Set_Enable_Button(&printer_dlg, BUT_DENSITY_HIGH, pro_set);
	  Set_Enable_Button(&printer_dlg, BUT_DENSITY_SUPER_HIGH, pro_set);

	  Set_Enable_Edit(&printer_dlg, IL_PAGE, pro_set);
	  Set_Enable_Edit(&printer_dlg, IL_MARGIN, 1);
	  Set_Enable_Edit(&printer_dlg, IL_WIDTH_PAPER, pro_set);
	  Set_Enable_Edit(&printer_dlg, IL_HEIGHT_PAPER, pro_set);

	  listbox[LS_IMAGE_FORMAT].enable = pro_set;

	  Set_Enable_Button(&printer_dlg, BUT_BW_PRINT, 1);
	  Set_Enable_Button(&printer_dlg, BUT_GRAY_PRINT, 1);
	  Set_Enable_Button(&printer_dlg, BUT_PCX_GRAY, 1);

	  Set_Enable_Button(&printer_dlg, BUT_ACTUAL_SIZE, 0);
      prn_ini_date_actual_size_bak = prn_ini_date.actual_size;
      Set_Check_Button(&printer_dlg, BUT_ACTUAL_SIZE, 0);
      prn_ini_date.actual_size=0;

	  sprintf(edit_p[0].txt, "%.1f", prn_ini_date.prn_width_paper);
	  sprintf(edit_p[1].txt, "%.1f", prn_ini_date.prn_height_paper);

	  listbox[1].enable = pro_set;
      listbox[2].enable = pro_set1;

  }
  else if (prn_ini_date.prn_type != PRN_WINDOWS)
  {
	
	  Set_Enable_Button(&printer_dlg, BUT_DENSITY_DRAFT, 1);
	  Set_Enable_Button(&printer_dlg, BUT_DENSITY_NORMAL, 1);
	  Set_Enable_Button(&printer_dlg, BUT_DENSITY_DOUBLE, 1);
	  Set_Enable_Button(&printer_dlg, BUT_DENSITY_HIGH, 1);
	  Set_Enable_Button(&printer_dlg, BUT_DENSITY_SUPER_HIGH, 1);

	  Set_Enable_Edit(&printer_dlg, IL_PAGE, 1);
	  Set_Enable_Edit(&printer_dlg, IL_MARGIN, 1);
	  Set_Enable_Edit(&printer_dlg, IL_WIDTH_PAPER, 1);
	  Set_Enable_Edit(&printer_dlg, IL_HEIGHT_PAPER, 1);
	
	  if (prn_ini_date.prn_type == PRN_PDF) listbox[LS_IMAGE_FORMAT].enable = 1;   //0  ???
	  else listbox[LS_IMAGE_FORMAT].enable = 1;
	
	 Set_Enable_Button(&printer_dlg, BUT_BW_PRINT, 1);
	 Set_Enable_Button(&printer_dlg, BUT_GRAY_PRINT, 1);
	 Set_Enable_Button(&printer_dlg, BUT_PCX_GRAY, 1);

     Set_Enable_Button(&printer_dlg, BUT_ACTUAL_SIZE, 1);
     prn_ini_date_actual_size_bak = prn_ini_date.actual_size;

	 sprintf(edit_p[0].txt, "%.1f", prn_ini_date.width_paper);
	 sprintf(edit_p[1].txt, "%.1f", prn_ini_date.height_paper);

	 listbox[1].enable = 1;
     listbox[2].enable = 0;

  }
  for (i = 0; i < listbox[LS_IMAGE_FORMAT].max; i++)
  {
	  if (strcmp(listbox[LS_IMAGE_FORMAT].txt[i], prn_ini_date.image_format) == 0)
	  {
		 listbox[LS_IMAGE_FORMAT].poz = i;
		 prn_ini_date.image_format_i = i;
		 break;
      }
  }
     
  
}

int pre_get_prn_ini_date(void)
{
	Get_Private_Profile_Strings((T_Fstring)PRINTER_DATA, get_printer_date);
	return prn_ini_date.prn_type;
}


static int get_prn_ini_date (float prn_width_paper, float prn_height_paper)
/*-----------------------------------------------------------------------*/
{ int Ret_Val;
  char sk[MAXPATH]="";
  char st[MAXPATH]="";
  int i_f_handle ;
  int key;
  int identyfikator;
  BOOL error_cfg;
  int k;
  int kolory_paper=0;
  PRIV_PRN_WINDOW  *priv_prn_window=get_priv_prn_window();
  static int curr_h, curr_v;
  int keys;

  T_Prn_Ini_Date prn_ini_date_bak;

  if (first == TRUE)
  {
    prn_ini_date.print_sheet = 0;   //tej zmiennej moze brakowac w ALFACAD.INI
    Get_Private_Profile_Strings ((T_Fstring)PRINTER_DATA, get_printer_date);
  }
  if (size_bak)
  {
	  prn_ini_date.width_paper = prn_ini_date_width_paper_bak;
	  prn_ini_date.height_paper = prn_ini_date_height_paper_bak;
  }

  if (reflection_bak)
  {
	  prn_ini_date.reflection = prn_ini_date_reflection_bak;
  }

    if (Check_if_Equal(priv_prn_window->xp,0.0) && Check_if_Equal(priv_prn_window->yp, 0.0) &&
        Check_if_Equal(priv_prn_window->xk,0.0) && Check_if_Equal(priv_prn_window->yk, 0.0))
    {
        set_priv_prn_window(Xmin, Ymin, Xmax, Ymax);
    }

	if (prn_ini_date.print_window==TRUE) {
		prn_ini_date.all_sheet = FALSE;                             /*okno*/
		prn_ini_date.xp = (float)priv_prn_window->xp;               /* okno do plotowania*/
		prn_ini_date.yp = (float)priv_prn_window->yp;               /* okno do plotowania*/
		prn_ini_date.xk = (float)priv_prn_window->xk;               /* okno do plotowania*/
		prn_ini_date.yk = (float)priv_prn_window->yk;

        if (prn_ini_date.xp < (float)Xmin) prn_ini_date.xp = (float)Xmin;
        if (prn_ini_date.yp < (float)Ymin) prn_ini_date.yp = (float)Ymin;
        if (prn_ini_date.xk > (float)Xmax) prn_ini_date.xk = (float)Xmax;
        if (prn_ini_date.yk > (float)Ymax) prn_ini_date.yk = (float)Ymax;
    }
    else
    {
		prn_ini_date.all_sheet = TRUE;               /*caly rysunek*/
		prn_ini_date.xp = (float)Xmin;               /* okno do plotowania*/
		prn_ini_date.yp = (float)Ymin;               /* okno do plotowania*/
		prn_ini_date.xk = (float)Xmax;               /* okno do plotowania*/
		prn_ini_date.yk = (float)Ymax;
    }

	prn_ini_date.prn_width_paper = prn_width_paper;
	prn_ini_date.prn_height_paper = prn_height_paper;

    mysprintf(&sz_range_x1,prn_ini_date.xp);
    mysprintf(&sz_range_y1,prn_ini_date.yp);
    mysprintf(&sz_range_x2,prn_ini_date.xk);
    mysprintf(&sz_range_y2,prn_ini_date.yk);

  Get_Default_Color_Dlg (&color_dlg);
  init_printer_dlg_control ();
  memcpy ( &prn_ini_date_old, &prn_ini_date,  sizeof (T_Prn_Ini_Date));
  
okno_dial:  

  gr_box[6].process = (void (*)(void)) Preview;
  reset_previewed();


  Save_Update_flex(0, &curr_h, &curr_v);
  Ret_Val = Dialog(&printer_dlg, &color_dlg, proc_dlg_prn_ini_date, bMouse);
  Save_Update_flex(1, &curr_h, &curr_v);


  my_scare_mouse();

  ////lock_mouse();

  if (Ret_Val == Dlg_Ret_Val_Zapis_Cfg)
    {
      /*zapis konfiguracji i powrot do okna dialogowego*/
      
      strcpy (sk, "");
      if (Load_File (sk, ZAPIS_CFG, TRUE) == FALSE)
       {   
        goto okno_dial;
       }

#ifndef LINUX
      i_f_handle=open(sk, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
      i_f_handle=open(sk, O_CREAT|O_TRUNC|O_RDWR, 0666); //S_IRWXU | S_IRWXG | S_IRWXO);
#endif
      if (i_f_handle == -1)
      { 
       strcpy(st, _FILE_OPENING_ERROR_);
       strcat(st,sk);
       komunikat_str (st);
       key = Get_Legal_Key ("\033\015");
       komunikat (0);
       goto okno_dial;   
      }
       else
        {
         error_cfg=FALSE;
         identyfikator=123456789;
         
         if (write (i_f_handle, &identyfikator, sizeof(int)) != sizeof(int)) error_cfg=TRUE;
          else if (write (i_f_handle, &prn_ini_date, sizeof(T_Prn_Ini_Date)) != sizeof(T_Prn_Ini_Date)) error_cfg=TRUE;
           
         if (error_cfg==TRUE) 
          {
           strcpy(st, _FILE_WRITING_ERROR_);
           strcat(st,sk);
           komunikat_str (st);
           key = Get_Legal_Key ("\033\015");
           komunikat (0);
          }
         close (i_f_handle) ;
        } 
      goto okno_dial;       
    }
    else if (Ret_Val == Dlg_Ret_Val_Odczyt_Cfg)
     {
       /*odczyt konfiguracji i powrot do okna dialogowego*/
      memcpy ( &prn_ini_date_bak, &prn_ini_date,  sizeof (T_Prn_Ini_Date)); 
      
      strcpy (sk, "");
      if (Load_File (sk, ODCZYT_CFG, TRUE) == FALSE)
       {   
        goto okno_dial;
       }
      i_f_handle = open (sk, O_BINARY | O_RDONLY, S_IREAD) ;
      if (i_f_handle == -1)
      { 
	   strcpy(st, _FILE_OPENING_ERROR_);
       strcat(st,sk);
       komunikat_str (st);
       key = Get_Legal_Key ("\033\015");
       komunikat (0);
       goto okno_dial;   
      }
       else
        {
         error_cfg=FALSE;
         identyfikator=0; //123456789;
         
         if (read (i_f_handle, &identyfikator, sizeof(int)) != sizeof(int)) error_cfg=TRUE;
          {
           if (identyfikator!=123456789) error_cfg=TRUE;
             else if (read (i_f_handle, &prn_ini_date, sizeof(T_Prn_Ini_Date)) != sizeof(T_Prn_Ini_Date)) error_cfg=TRUE;
          } 
          
        if (error_cfg==TRUE)
          {
           strcpy(st, _FILE_READING_ERROR_);
           strcat(st,sk);
           komunikat_str (st);
           key = Get_Legal_Key ("\033\015");
           komunikat (0);
           
           memcpy ( &prn_ini_date, &prn_ini_date_bak, sizeof (T_Prn_Ini_Date)); 
          }
           else init_printer_dlg_control ();
           
         close (i_f_handle) ;
        } 
      goto okno_dial;      
     }
     else
      if ((Ret_Val != Dlg_Ret_Val_OK) &&  (Ret_Val != Dlg_Ret_Set_Range)) /*mouse ???????*/
       {
         return 0;
       }


	  strcpy(prn_ini_date.plik, prn_file);


  if ( 0 != memcmp ( &prn_ini_date_old, &prn_ini_date,  sizeof (T_Prn_Ini_Date)))
  {
   Add_Private_Profile_Group ((T_Fstring)PRINTER_DATA, add_value); /*?????????*/
  }

  if (prn_ini_date.prn_type == PRN_WINDOWS)  //this is not really necessary, printer driver will be initialized anyway
  {

	  prn_ini_date_width_paper_bak = prn_ini_date.width_paper;
	  prn_ini_date_height_paper_bak = prn_ini_date.height_paper;
	  size_bak = TRUE;
	  prn_ini_date.width_paper = prn_ini_date.prn_width_paper;
	  prn_ini_date.height_paper = prn_ini_date.prn_height_paper;
  }

#ifndef LINUX
  if ((prn_ini_date.prn_type == PRN_WINDOWS) && (NO_REFLECTION))
  {
	  prn_ini_date_reflection_bak = prn_ini_date.reflection;
	  reflection_bak = TRUE;
	  prn_ini_date.reflection = FALSE;
  }
#endif

    if (prn_ini_date.prn_type == PRN_PDF)
    {
        prn_ini_date_reflection_bak = prn_ini_date.reflection;
        reflection_bak = TRUE;
        prn_ini_date.reflection = FALSE;
    }

  if (Ret_Val == Dlg_Ret_Set_Range) return 2;
  else return 1;
}

/*

                              GLOBAL FUNCTIONS                

*/

int Prn_Draw_Param (char *ptrsz_file_name,
		     T_PTR_Prn_Config *ptrs_config,
	         T_PTR_Prn_Ini_Date *ptrs_ini_date, float prn_width_paper, float prn_height_paper, int serial_mode)  ///*T_PTR_Prn_Ini_Date *ptrs_ini_date,*/
//---------------------------------------------------------------------------------------------------------
{ int ret_date;

  prn_file = ptrsz_file_name ;

  if (serial_mode == 0) strcpy(wydruk_na_drukarce, _PRINTING_);
  else if (serial_mode == 1) strcpy(wydruk_na_drukarce, _PRINTING_SERIAL_);
  else if (serial_mode == 2) strcpy(wydruk_na_drukarce, _PRINTING_SPEC_);

  if (get_config_prn () == FALSE)
  {
     ErrList ( 26) ;
      return 0 ;
  }
  prn_ini_date.roll = 1;

  if ((ret_date = get_prn_ini_date (prn_width_paper, prn_height_paper)) == 0)
  {
    return 0 ;
  }
  prn_ini_date.roll = 1;
  first = FALSE ;
  *ptrs_config = prn_config ;

  if (prn_ini_date.prn_type == PRN_9)
  {
    *ptrs_config = prn_config + 1 ;
  }
  if (prn_ini_date.prn_type == PRN_24)
  {
    *ptrs_config = prn_config + 2 ;
  }
  if (prn_ini_date.prn_type == PRN_48)
  {
    *ptrs_config = prn_config + 3 ;
  }
  if (prn_ini_date.prn_type == PRN_LASER)
  {
    *ptrs_config = prn_config + 4 ;
  }
  if (prn_ini_date.prn_type == PRN_ESC_P2)
  {
    *ptrs_config = prn_config + 5 ;
  }

  ////////  TEMPORARY  ///////////
  
  prn_ini_date.buforowanie=1;
  prn_ini_date.text_mode=0;
  /////////////////////////////////

  *ptrs_ini_date = &prn_ini_date;

  return ret_date ;
}


#undef __PRNDLG__

