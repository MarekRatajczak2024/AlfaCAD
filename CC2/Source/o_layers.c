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

#define __LAYERS__
#include <forwin.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_dialog.h"
#include "alf_res.h"
#include "o_libfun.h"

#include "gfx.h"
#include "mouse.h"

//#include "gui.h"

#include "menu.h"

#include "leak_detector_c.h"

extern void layer_info (void);
extern void Image_Free (void) ;
extern int Save_Message0_Image (void);
extern int Save_Screen(void);
extern int Restore_Screen(void);
extern void Put_Message0_Image (int image_no);
extern char *Colors_Name_txt[MAXCOLOR + 1];
extern char digits[16];
extern char numbers[16];
extern int getmaxy(void);
extern void draw_button(BUTTON *Button);
extern void redraw_button(BUTTON* Button);
extern int ask_question (int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien, int image);
extern void delete_all_from_layer_atrybut (int layer_no, int atrybut);
extern void shift_layer_numbers(int layer_no);
extern int ask_question_in_dialog (int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien, int image);

extern void  SetBit( unsigned int A[],  int k );
extern void  ClearBit( unsigned int A[],  int k );
extern int TestBit( unsigned int A[],  int k );

extern KOLORY kolory;

extern int d_myslider_proc(int msg, void *d, int c);
extern void komunikat_str_short(char *st, BOOL stay, BOOL center);

extern void Draw_Slider(SLIDER *Slider);
//extern int slider_function(int *var);
//extern int grab_slider(void *dp3, int d2);
extern int vfv(int v);

#define XpGroup	2
#define YpGroup 4
#define DXGroup (XLayerNo + DXLayerNo +\
	DXCurrentButton+DXLayerName+DXOnOff +\
	DXPointButton+DXEditButton+DXColor+DXWidthLine+DXTypeLine +\
	DXBut+13*DXDist - 13)
int DYGroup = 425;  //12*(32+3) + 5;
#define XpUp    (XTypeLine+DXTypeLine+DXDist+7) 
#define YpUp    (4 * DYLayer - 1 + 6)
#define XpPgUp  XpUp 
#define YpPgUp  (7 * DYLayer - 15 + 4)
#define XpPgDn  XpUp 
#define YpPgDn  (10 * DYLayer - 28 + 2)
#define XpDn    XpUp 
#define YpDn    (13 * DYLayer - 42)
#define XpDel   195
#define YpDel   170
#define XpAdd   307
#define YpAdd   YpDel
#define XpOK    380
#define YpOK    YpDel
#define XpCANCEL  453
#define YpCANCEL  YpDel
#define DXBut 58
#define DYBut 30

#define XLayerNo  	5
#define DXLayerNo  	20 //25 //18
#define XCurrentButton 	(XLayerNo + DXLayerNo + DXDist)
#define DXCurrentButton 15
#define XLayerName      (XCurrentButton + DXCurrentButton + DXDist)
#define DXLayerName     (24 * 8 + 10)        ////(17 * 8 + 10) 
#define XOnOff          (XLayerName + DXLayerName+ DXDist)
#define DXOnOff         DXCurrentButton
#define DXSetAll        12
#define DYSetAll        12 
#define XEditButton     (XOnOff + DXOnOff + DXDist)
#define DXEditButton    DXCurrentButton
#define XPointButton	(XEditButton + DXEditButton + DXDist)
#define DXGreyButton	DXCurrentButton
#define XGreyButton	    (XTypeLine + DXTypeLine + DXDist -25) 
#define XBWButton	    (XGreyButton + DXGreyButton + DXDist) 
#define DXPointButton	DXCurrentButton
#define XColor          (XPointButton+ DXPointButton+ DXDist)
#define DXColor  	    (6 * 8 + 10) + 12 ////(11 * 8 + 10) + 12
#define XWidthLine	(XColor + DXColor + DXDist)
#define DXWidthLine     (7 * 8 + 10) + 3 
#define XTypeLine	(XWidthLine + DXWidthLine + DXDist) 
#define DXTypeLine	   (11 * 8 + 10) + 3 ////(13 * 8 + 10) + 3
#define YLayer		(XTypeLine + DXTypeLine + DXDist)
#define DYLayer     12   // 16 /*DYLab */
#define DXDist		3
#define DYComboBox	12 //14 //(ED_INF_HEIGHT + 2)

#define LABEL_LAYERS_NO 2 /*11*/
#define NoLabels 34
int NoDialogLayers=10;
int NoDialogLayers2=12;
int MaxNoDialogLayers=10;

unsigned int b_layers[8]={0};
unsigned int b_layers_curr[8]={0};
unsigned int b_layers_bak[8]={0};
BOOL b_layers_bak_set=FALSE;

#define MAXNODIALOGLAYERS 32

#define NoColors	MAXCOLOR
#define BUTTON_CURRENT  22 //20 //15 //13 /*7*/

int BUTTON_ONOFF;
int BUTTON_EDIT;
int BUTTON_POINT;
int BUTTON_GREY;
int BUTTON_BW;
int BUTTON_ERASE;
int BUTTON_SIZE=211;
int BUTTON_SIZE2=213;
int COMBOBOX_COLOR=0;
int COMBOBOX_WIDTH;
int COMBOBOX_TYPE;
int COMBOBOX_SIZE=96;

#define ID_ERASE        255
#define ID_MARK         256
#define ID_BACK         257

#define ID_OK		    1
#define ID_CANCEL      	2
#define ID_Up           3
#define ID_Dn           4
#define ID_PgUp         5
#define ID_PgDn         6
#define ID_SET_W_ALL    7
#define ID_SET_E_ALL    8
#define ID_SET_P_ALL    9
#define ID_RESET_W_ALL  10
#define ID_RESET_E_ALL  11
#define ID_RESET_P_ALL  12
#define ID_GoTo         13
#define ID_GoTo1        14
#define ID_SET_G_ALL    15
#define ID_SET_BW_ALL   16
#define ID_RESET_G_ALL  17
#define ID_RESET_BW_ALL 18
#define ID_ADD          19

static int n_first_layer_in_dlg = 0;
static int n_last_layer_in_dlg;
static int n_first_layer_in_dlg_old = 0;
static int no_layers, current_layer;
static char go_to1_txt[17]="1";
static char go_to2_txt[17]="60";

static LISTBOX listbox [MAXNODIALOGLAYERS * 3];
static COMBOBOX combobox [MAXNODIALOGLAYERS * 3];

static int BLayer=-1;
static int ELayer=-1;
static int erase_state=-1;

static int init_slider(int *var1, int *var2, int *var3, int *var4);
static int grab_slider(void *dp3, int d2);

int up_layer_ (void);
int dn_layer_ (void);

LAYER *layers_table ;
DLG_COLOR color_dlg;
BOOL b_redraw ;

#define D20 12

//0
static BUTTON button_GoTo1 =  {
    XpGroup + XpPgUp + 31, YpGroup + YpUp + D20 - 2 * DYComboBox - 5, 16, DYComboBox, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0,B_PUSHBUTTON, 79, 1,0, ID_GoTo1,
  };
//1
static BUTTON button_GoTo =  {
    XpGroup + XpPgUp + 31, YpGroup + YpUp + D20 - DYComboBox - 3, 16, DYComboBox, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0,B_PUSHBUTTON, 80, 1,0, ID_GoTo,
  };
//2
static BUTTON button_Up =  {
    XpGroup + XpUp, YpGroup + YpUp + D20, DXBut - 11, DYBut - 12, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0,B_PUSHBUTTON, 75, 1,0, ID_Up,
  };
//3
static BUTTON button_Dn =  {
  XpGroup + XpDn, YpGroup + YpDn + D20, DXBut - 11, DYBut - 12, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0,B_PUSHBUTTON, 76, 1,0, ID_Dn,
  };
//4
static BUTTON button_PgUp =  {
    XpGroup + XpPgUp, YpGroup + YpPgUp + D20, DXBut - 11, DYBut - 12, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0,B_PUSHBUTTON, 77, 1,0, ID_PgUp,
  };
//5
static BUTTON button_PgDn =  {
  XpGroup + XpPgDn, YpGroup + YpPgDn + D20, DXBut - 11, DYBut - 12, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "", 0,B_PUSHBUTTON, 78, 1,0, ID_PgDn,
  };
//6
static BUTTON button_Add =  {
   XpAdd, YpAdd, DXBut0-20, DYBut0, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, ""/*layer_comm2*/, 0,B_PUSHBUTTON, 88, 1,0, ID_ADD,
  };
//7
static BUTTON button_OK =  {
   XpOK, YpOK, DXBut0-20, DYBut0, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, ""/*layer_comm3*/, 0,B_PUSHBUTTON, 87, 1,0, ID_OK,
  };
//8
static BUTTON button_Cancel =  {
   XpCANCEL, YpCANCEL, DXBut0-20, DYBut0, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, "" /*layer_comm4*/, 0,B_PUSHBUTTON, 83, 1,0, ID_CANCEL,
  };  
//9
static BUTTON button_set_W_all =  {
    XpGroup + XOnOff -1, YpCANCEL, DXSetAll, DYSetAll, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, layer_comm5, 0,B_PUSHBUTTON, 0, 1,0, ID_SET_W_ALL,
  };
 //10
static BUTTON button_set_E_all =  {
    XpGroup + XEditButton-1, YpCANCEL, DXSetAll, DYSetAll, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, layer_comm6, 0,B_PUSHBUTTON, 0, 1,0, ID_SET_E_ALL,
  };
  //11
static BUTTON button_set_P_all =  {
    XpGroup + XPointButton-1, YpCANCEL, DXSetAll, DYSetAll, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, layer_comm7, 0,B_PUSHBUTTON, 0, 1,0, ID_SET_P_ALL,
  };
    //12
static BUTTON button_reset_W_all =  {
    XpGroup + XOnOff-1, YpCANCEL + 14 * 2, DXSetAll, DYSetAll, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, layer_comm8, 0,B_PUSHBUTTON, 0, 1,0, ID_RESET_W_ALL,
  };
    //13
static BUTTON button_reset_E_all =  {
    XpGroup + XEditButton-2, YpCANCEL + 14 * 2, DXSetAll, DYSetAll, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, layer_comm9, 0,B_PUSHBUTTON, 0, 1,0, ID_RESET_E_ALL,
  };
    //14
static BUTTON button_reset_P_all =  {
    XpGroup + XPointButton-1, YpCANCEL + 14 * 2, DXSetAll, DYSetAll, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, layer_comm10, 0,B_PUSHBUTTON, 0, 1,0, ID_RESET_P_ALL,
  };
//15
static BUTTON button_set_G_all =  {
    XpGroup + XGreyButton -1, YpCANCEL, DXSetAll, DYSetAll, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, layer_comm11, 0,B_PUSHBUTTON, 0, 1,0, ID_SET_G_ALL,
  };
//16
static BUTTON button_set_BW_all =  {
    XpGroup + XBWButton -1, YpCANCEL, DXSetAll, DYSetAll, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, layer_comm12, 0,B_PUSHBUTTON, 0, 1,0, ID_SET_BW_ALL,
  };
//17
static BUTTON button_reset_G_all =  {
    XpGroup + XGreyButton -1, YpCANCEL + 14 * 2, DXSetAll, DYSetAll, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, layer_comm13, 0,B_PUSHBUTTON, 0, 1,0, ID_RESET_G_ALL,
  };
 //18
static BUTTON button_reset_BW_all =  {
    XpGroup + XBWButton -1, YpCANCEL + 14 * 2, DXSetAll, DYSetAll, COLOR_NULL,
    COLOR_NULL, COLOR_NULL, layer_comm14, 0,B_PUSHBUTTON, 0, 1,0, ID_RESET_BW_ALL,
  };
//19
static BUTTON button_erase =  {
        XpGroup + 5, YpCANCEL + 14 * 2, DXSetAll, DYSetAll, COLOR_NULL,
        COLOR_NULL, COLOR_NULL, "", 0,B_CHECKBOX, 0, 1,0, ID_ERASE,
};
//20
static BUTTON button_mark =  {
        XpGroup + 37, YpCANCEL + 14 * 2, DXSetAll, DYSetAll, COLOR_NULL,
        COLOR_NULL, COLOR_NULL, "", 0,B_CHECKBOX, 0, 1,0, ID_MARK,
};
//21
static BUTTON button_back =  {
        XpGroup + XOnOff - 4, YpGroup + 9, 18, DYComboBox+2, COLOR_NULL,
        COLOR_NULL, COLOR_NULL, layer_comm35, 0,B_PUSHBUTTON, 86, 1,0, ID_BACK,
};


//static INPUTLINE edit_table [NoDialogLayers2];
static INPUTLINE edit_table [34];
//static BUTTON buttons [BUTTON_SIZE2];
static BUTTON buttons [246];

static LABEL labels [NoLabels] = {
  { XpGroup+ XCurrentButton -20,  	   YpGroup + 2 * DYLayer + DYIL, 0, 0, COLOR_NULL,COLOR_NULL,layer_comm15,0,0},
  { XpGroup+ DXIL + XLayerName+30, YpGroup + 2 * DYLayer + DYIL, 0, 0, COLOR_NULL,COLOR_NULL,layer_comm16,0,0},
};

static GROUP_BOX gr_box [1]=
{
  { XpGroup, YpGroup, DXGroup, 0,
	COLOR_NULL ,COLOR_NULL, layer_comm32, 0, NULL,
  },
};

static IMAGE images_l[] =
{
 { XpGroup + XOnOff - 3, YpGroup + 2 * DYLayer-2, 32, 32, 2,layer_tip0},  //visible
 { XpGroup + XEditButton - 3, YpGroup + 2 * DYLayer-2, 32, 32, 3,layer_tip1},  //edit
 { XpGroup + XPointButton - 3, YpGroup + 2 * DYLayer-2, 32, 32, 4,layer_tip2},   //point
 { XpGroup + XOnOff - 3, YpCANCEL + 14 * 1 -1, 32, 32, 11,layer_tip3},  //visible+-
 { XpGroup + XEditButton - 3, YpCANCEL + 14 * 1 -1, 32, 32, 12,layer_tip4},  //edit+-
 { XpGroup + XPointButton - 3, YpCANCEL + 14 * 1 -1, 32, 32, 13,layer_tip5},   //point+-
 { XpGroup + DXIL + XColor + 20,  YpGroup + 2 * DYLayer-6, 32, 32, 5,layer_tip6},  //color
 { XpGroup + DXIL + XWidthLine + 19,    YpGroup + 2 * DYLayer-6, 32, 32, 6,layer_tip7},  //thickness
 { XpGroup + DXIL + XTypeLine + 20,  YpGroup + 2 * DYLayer-6, 32, 32, 7,layer_tip8},  //type
 { XpGroup + XBWButton + 10,  YpGroup + 10, 48, 48, 10,layer_tip9},  //printer
 { XpGroup + XBWButton + 35,  YpGroup + 10, 48, 48, 89,layer_tip10},  //plotter
 { XpGroup + XGreyButton-3,  YpGroup + 1 * DYLayer + 6, 32, 32, 8,layer_tip11},  //grey
 { XpGroup + XBWButton-3,  YpGroup + 1 * DYLayer + 6, 32, 32, 9,layer_tip12},  //black & white
 { XpGroup + XGreyButton-3,  YpCANCEL + 14 * 1 -1, 32, 32, 14,layer_tip13},  //grey+-
 { XpGroup + XBWButton-3,  YpCANCEL + 14 * 1 -1, 32, 32, 15,layer_tip14},  //black & white+-
 { XpGroup + DXIL + XLayerName + 120, YpCANCEL + 10, 64, 64, 109,layer_tip15},  //All layers
 { XpGroup, YpCANCEL + 10, 64, 64, 198,layer_tip16},  //Erase layer
 { XpGroup + 32, YpCANCEL + 10, 64, 64, 199,layer_tip17},  //Mark layers
};

static DARK_LIGHT_LINE lines_top[] =
{
	{ XpGroup + XWidthLine, 39, DXWidthLine, 1, LINE_NULL, COLOR_NULL},
	{ XpGroup + XTypeLine, 39, DXTypeLine - 30, 1, LINE_NULL, COLOR_NULL},
};

static SLIDER sliders[] =
        {
                {d_myslider_proc,  XpDn + (DXBut - 11)/2, YpDn + DYBut - 7,  18, 415 - YpDn,  0,  98,    0,   0,  100,   0,    NULL , grab_slider, init_slider, NULL},
        };

static TDIALOG layers_dlg=
{
	2 ,  1, 620, 220,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0x40,0,0,
	layer_comm34,
	2, &lines_top,		/*line*/
    NoLabels, &labels,
	1, &gr_box,
	34 , &edit_table,	/*edit*/  //++numer warstwy
	18, &images_l,		/*image*/
    213, &buttons,
	0, NULL,		/*listbox*/
    96, &combobox,
    1,&sliders, //Sliders
	NULL,
	NULL,
	0,
	0,
    NULL,
};


static void	add_layer (void);
static void disable_dlg_layer_controls (int );
static void draw_dlg_layer_controls (int );
static void get_dlg_controls_to_layer (int , int );
static void get_dlg_controls_to_layers (void);
static void init_layer_dlg_control (BOOL b_draw);
static int  proc_dlg_layer (int );
static void set_layer_to_dlg (int , int );
static void set_struct_dialog_control (int erase_flag);

static int init_slider(int *var1, int *var2, int *var3, int *var4)
{
    *var1=n_first_layer_in_dlg;
    *var2=n_last_layer_in_dlg+1;
    //*var3=No_Layers;
    *var3=no_layers;
    *var4=NoDialogLayers;
    return 1;
}

static int grab_slider(void *dp3, int d2)
{   char slider_var[32];
    int var1, var2, var3, var4, ret;
    int (*SlideFun)(int*, int*, int*, int*);

    SlideFun = (int(*)(int *, int *, int *, int *))dp3;

    ret = SlideFun(&var1, &var2, &var3, &var4);
    //sprintf(slider_var, "%d-%d", max(0, d2-NoDialogLayers), d2);
    sprintf(slider_var, "%d-%d", var3 - d2 - (var2-var1) + 1, var3-d2);
    komunikat_str_short(slider_var, FALSE, FALSE);


    get_dlg_controls_to_layers ();
    n_first_layer_in_dlg=var3 - d2 - (var2-var1);
    n_last_layer_in_dlg=var3-d2 - 1;
    init_layer_dlg_control (FALSE);

    //select_mouse_cursor(MOUSE_CURSOR_NONE);
    //show_mouse(NULL);
    Redraw_Dlg(&layers_dlg);
    //select_mouse_cursor(MOUSE_CURSOR_ALLEGRO);
    //show_mouse(screen);
    return 0;
}


void reset_BLayerELayer(void)
{
    BLayer=-1;
    ELayer=-1;
}

void reset_erase_state(void)
{
    erase_state=-1;
}

static void set_struct_dialog_control (int erase_flag)
/*--------------------------------------------------*/
{
  int i;
  static char sz_label [4 * 256] ;
  double wsp_y;
  int dy;
  double alfay=100;

  wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);

  dy = 8 - (int)(((float)HEIGHT / 4.0) + 0.5);

  //Max
  NoDialogLayers = min((getmaxy()/wsp_y - alfay  - 2*DYLayer)/DYLayer, 32);
  NoDialogLayers2=NoDialogLayers+2;

  BUTTON_ONOFF = BUTTON_CURRENT + NoDialogLayers;
  BUTTON_EDIT =	BUTTON_ONOFF  +  NoDialogLayers;
  BUTTON_POINT = BUTTON_EDIT  +  NoDialogLayers;
  BUTTON_GREY = BUTTON_POINT + NoDialogLayers;
  BUTTON_BW = BUTTON_GREY + NoDialogLayers;
  BUTTON_ERASE = BUTTON_BW + NoDialogLayers;
  BUTTON_SIZE =	BUTTON_ERASE  +  NoDialogLayers;
  BUTTON_SIZE2 = BUTTON_ERASE  +  NoDialogLayers2;
  COMBOBOX_COLOR = 0;
  COMBOBOX_WIDTH = COMBOBOX_COLOR + NoDialogLayers;
  COMBOBOX_TYPE = COMBOBOX_WIDTH + NoDialogLayers;
  COMBOBOX_SIZE	= COMBOBOX_TYPE + NoDialogLayers;

    layers_dlg.SizeButtonT=BUTTON_SIZE;
    layers_dlg.SizeComboBoxT=COMBOBOX_SIZE;
    layers_dlg.SizeInLinT=NoDialogLayers2;
    layers_dlg.SizeLabT=NoDialogLayers+2;

    DYGroup=(DYLayer * (NoDialogLayers  + 3) + 5);

    layers_dlg.dy=DYGroup + DYBut0 + 14;  //11

    gr_box[0].dy=DYGroup;

    int ddy=(int)((float)(HEIGHT-18)/7.0 + 0.5);
    int ddx=0;

/*labels*/
  for ( i = LABEL_LAYERS_NO; i < LABEL_LAYERS_NO + NoDialogLayers ; i++)
  {
     labels [i].x = XpGroup + XLayerNo + 7 ;  //-2
     labels [i].y = dy  + YpGroup + (i - LABEL_LAYERS_NO+ 3) * DYLayer  + ddy;
     labels[i].dx = 22;
     labels [i].dy = 8 ;
     labels [i].ink = COLOR_NULL ;
     labels [i].paper = COLOR_NULL ;
     labels [i].txt = &sz_label [4 * (i - LABEL_LAYERS_NO)] ;
     labels [i].txt [3] = '\0' ;
  }

/*buttons*/
  memcpy (buttons + 0, &button_Up, sizeof (BUTTON));
  memcpy (buttons + 1, &button_Dn, sizeof (BUTTON));
  memcpy (buttons + 2, &button_PgUp, sizeof (BUTTON));
  memcpy (buttons + 3, &button_PgDn, sizeof (BUTTON));

  button_Add.y=YpGroup+DYGroup+2;
  button_OK.y=YpGroup+DYGroup+2;
  button_Cancel.y=YpGroup+DYGroup+2;

  sliders[0].x=button_Dn.x + button_Dn.dx/2 - sliders[0].w/2;
  sliders[0].y=button_Dn.y + button_Dn.dy + 4;
  sliders[0].h=gr_box[0].y + gr_box[0].dy - button_Dn.y - button_Dn.dy - 8;

    button_set_W_all.y=button_set_E_all.y=button_set_P_all.y=YpGroup+DYGroup+2;
    button_reset_W_all.y=button_reset_E_all.y=button_reset_P_all.y=YpGroup+DYGroup+14*2 + 2;
    button_set_G_all.y=button_set_BW_all.y=YpGroup+DYGroup+2;
    button_reset_G_all.y=button_reset_BW_all.y=button_erase.y=button_mark.y=YpGroup+DYGroup+14*2 + 2;

    button_erase.check=erase_flag;

    images_l[3].y1=images_l[4].y1=images_l[5].y1=images_l[13].y1=images_l[14].y1=YpGroup+DYGroup+14+1+ddy-vfv(1);
    images_l[15].y1=images_l[16].y1=images_l[17].y1=YpGroup+DYGroup + 10 +ddy - vfv(2);

    images_l[0].x1=XpGroup + XOnOff - 3 + ddx;
    images_l[1].x1=XpGroup + XEditButton - 3 + ddx;
    images_l[2].x1=XpGroup + XPointButton - 3 + ddx;

    images_l[3].x1=XpGroup + XOnOff - 3 + ddx;
    images_l[4].x1=XpGroup + XEditButton - 3 + ddx;
    images_l[5].x1=XpGroup + XPointButton - 3 + ddx;

    images_l[6].x1=XpGroup + DXIL + XColor + 20 + ddx;
    images_l[7].x1=XpGroup + DXIL + XWidthLine + 19 + ddx;
    images_l[8].x1=XpGroup + DXIL + XTypeLine + 20 + ddx;

    images_l[9].x1=XpGroup + XBWButton + 10 + ddx;
    images_l[10].x1=XpGroup + XBWButton + 35 + ddx;
    images_l[11].x1=XpGroup + XGreyButton-4 + ddx;
    images_l[12].x1=XpGroup + XBWButton-3 + ddx;
    images_l[13].x1=XpGroup + XGreyButton-4 + ddx;
    images_l[14].x1=XpGroup + XBWButton-3 + ddx;

    images_l[15].x1=XpGroup + DXIL + XLayerName + 120 + ddx;

  memcpy (buttons + 4, &button_Add, sizeof (BUTTON));
  memcpy (buttons + 5, &button_OK, sizeof (BUTTON));
  memcpy (buttons + 6, &button_Cancel, sizeof (BUTTON));
  memcpy (buttons + 7, &button_set_W_all, sizeof (BUTTON));
  memcpy (buttons + 8, &button_set_E_all, sizeof (BUTTON));
  memcpy (buttons + 9, &button_set_P_all, sizeof (BUTTON));
  memcpy (buttons + 10, &button_reset_W_all, sizeof (BUTTON));
  memcpy (buttons + 11, &button_reset_E_all, sizeof (BUTTON));
  memcpy (buttons + 12, &button_reset_P_all, sizeof (BUTTON));
  memcpy (buttons + 13, &button_GoTo, sizeof (BUTTON));
  memcpy (buttons + 14, &button_GoTo1, sizeof (BUTTON));
  memcpy (buttons + 15, &button_set_G_all, sizeof (BUTTON));
  memcpy (buttons + 16, &button_set_BW_all, sizeof (BUTTON));
  memcpy (buttons + 17, &button_reset_G_all, sizeof (BUTTON));
  memcpy (buttons + 18, &button_reset_BW_all, sizeof (BUTTON));
  memcpy (buttons + 19, &button_erase, sizeof (BUTTON));
  memcpy (buttons + 20, &button_mark, sizeof (BUTTON));
  memcpy (buttons + 21, &button_back, sizeof (BUTTON));


  for ( i = BUTTON_CURRENT; i < BUTTON_CURRENT+ NoDialogLayers ; i++)
  {
     buttons [i].x = XpGroup + XCurrentButton + 6;
     buttons [i].y = YpGroup + (i - BUTTON_CURRENT + 3) * DYLayer;
     buttons [i].dx = DXCurrentButton;	/*nie uzywane*/
     buttons [i].dy = DYComboBox;
     buttons [i].border = buttons [i].ink = buttons [i].paper = COLOR_NULL;
     buttons [i].txt = "";
     buttons [i].check = 0;
     buttons [i].type = B_RADIOBUTTON;
     buttons [i].name2 = 0;
     buttons [i].enable = 0;
     buttons [i].flags = 0;
     buttons [i].id = i;
   }
  for ( i = BUTTON_ONOFF; i < BUTTON_ONOFF+ NoDialogLayers ; i++)
  {
     buttons [i].x = XpGroup + XOnOff;
     buttons [i].y = YpGroup + (i - BUTTON_ONOFF + 3) * DYLayer;
     buttons [i].dx = DXOnOff;	/*nie uzywane*/
     buttons [i].dy = DYComboBox;
     buttons [i].border = buttons [i].ink = buttons [i].paper = COLOR_NULL;
     buttons [i].txt = "";
     buttons [i].check = 0;
     buttons [i].type = B_CHECKBOX;
     buttons [i].name2 = 0;
     buttons [i].enable = 0;
     buttons [i].flags = 0;
     buttons [i].id = i;
   }
  for ( i = BUTTON_EDIT; i < BUTTON_EDIT+ NoDialogLayers ; i++)
  {
     buttons [i].x = XpGroup + XEditButton;
     buttons [i].y = YpGroup + (i - BUTTON_EDIT + 3) * DYLayer;
     buttons [i].dx = DXOnOff;	/*nie uzywane*/
     buttons [i].dy = DYComboBox;
     buttons [i].border = buttons [i].ink = buttons [i].paper = COLOR_NULL;
     buttons [i].txt = "";
     buttons [i].check = 0;
     buttons [i].type = B_CHECKBOX;
     buttons [i].name2 = 0;
     buttons [i].enable = 0;
     buttons [i].flags = 0;
     buttons [i].id = i;
   }
  for ( i = BUTTON_POINT; i < BUTTON_POINT+ NoDialogLayers ; i++)
  {
     buttons [i].x = XpGroup + XPointButton;
     buttons [i].y = YpGroup + (i - BUTTON_POINT + 3) * DYLayer;
     buttons [i].dx = DXOnOff;	/*nie uzywane*/
     buttons [i].dy = DYComboBox;
     buttons [i].border = buttons [i].ink = buttons [i].paper = COLOR_NULL;
     buttons [i].txt = "";
     buttons [i].check = 0;
     buttons [i].type = B_CHECKBOX;
     buttons [i].name2 = 0;
     buttons [i].enable = 0;
     buttons [i].flags = 0;
     buttons [i].id = i;
   }
  for ( i = BUTTON_GREY; i < BUTTON_GREY+ NoDialogLayers ; i++)
  {
     buttons [i].x = XpGroup + XGreyButton;
     buttons [i].y = YpGroup + (i - BUTTON_GREY + 3) * DYLayer;
     buttons [i].dx = DXOnOff;	/*nie uzywane*/
     buttons [i].dy = DYComboBox;
     buttons [i].border = buttons [i].ink = buttons [i].paper = COLOR_NULL;
     buttons [i].txt = "";
     buttons [i].check = 0;
     buttons [i].type = B_CHECKBOX;
     buttons [i].name2 = 0;
     buttons [i].enable = 0;
     buttons [i].flags = 0;
     buttons [i].id = i;
   }
  for ( i = BUTTON_BW; i < BUTTON_BW+ NoDialogLayers ; i++)
  {
     buttons [i].x = XpGroup + XBWButton;
     buttons [i].y = YpGroup + (i - BUTTON_BW + 3) * DYLayer;
     buttons [i].dx = DXOnOff;	/*nie uzywane*/
     buttons [i].dy = DYComboBox;
     buttons [i].border = buttons [i].ink = buttons [i].paper = COLOR_NULL;
     buttons [i].txt = "";
     buttons [i].check = 0;
     buttons [i].type = B_CHECKBOX;
     buttons [i].name2 = 0;
     buttons [i].enable = 0;
     buttons [i].flags = 0;
     buttons [i].id = i;
  }
  for ( i = BUTTON_ERASE; i < BUTTON_ERASE+ NoDialogLayers ; i++)
  {
    buttons [i].x = XpGroup + 2;
    buttons [i].y = YpGroup + 2 + (i - BUTTON_ERASE + 3) * DYLayer;
    buttons [i].dx = 10; //DXOnOff;	/*nie uzywane*/
    buttons [i].dy = 8; //DYComboBox;
    buttons [i].border = buttons [i].ink = buttons [i].paper = COLOR_NULL;
    buttons [i].txt = "";
    buttons [i].check = 0;
    buttons [i].type = B_FLATPUSHBUTTON; //B_PUSHBUTTON;
    buttons [i].name2 = 193;
    buttons [i].enable = erase_flag && buttons [i - BUTTON_ERASE + BUTTON_CURRENT].enable;
    buttons [i].flags = (erase_flag==1) ? 0 : BUTTON_HIDDEN;
    buttons [i].id = i;
  }
/*edit*/
  for ( i = 0; i < NoDialogLayers ; i++)
  {
    edit_table[i].x =  XpGroup + XLayerName + 2;
    edit_table[i].y =  YpGroup + (i + 3) * DYLayer;
    edit_table[i].dx = DXLayerName - 2;
    edit_table[i].dy = DYComboBox;
    edit_table[i].border = edit_table[i].ink = edit_table[i].ink_ini = edit_table[i].paper_ini = COLOR_NULL;
    edit_table[i].paper = COLOR_NULL;
	edit_table[i].MaxLen = MaxLenNameLayer; //64; //32; // maxlen_w - 1; // 32; // 20; //16
	edit_table[i].width = 32; // maxlen_w - 1; // 32;
    edit_table[i].enable = 0;		/*wazne*/
    edit_table[i].id = -1;
    edit_table[i].txt = ""; /*layer_name_txt [i]; */  /*nie moze byc smieci*/
	edit_table[i].legal = "";
  }
 //numer warstwy
    edit_table[NoDialogLayers].x =  XpGroup + XpPgUp;
    edit_table[NoDialogLayers].y =  YpGroup + YpUp + D20 - DYComboBox - 3;
    edit_table[NoDialogLayers].dx = (DXBut - 8 ) / 5 * 3 ;
    edit_table[NoDialogLayers].dy = DYComboBox;
    edit_table[NoDialogLayers].border = edit_table[NoDialogLayers].ink = edit_table[NoDialogLayers].ink_ini = edit_table[NoDialogLayers].paper_ini = COLOR_NULL;
    edit_table[NoDialogLayers].paper = COLOR_NULL;
    edit_table[NoDialogLayers].MaxLen = 3;
    edit_table[NoDialogLayers].width = 3;
    edit_table[NoDialogLayers].enable = 1;
    edit_table[NoDialogLayers].id = -1;
    edit_table[NoDialogLayers].txt = (char *)go_to2_txt; //"60";
	edit_table[NoDialogLayers].legal = &numbers;

    edit_table[NoDialogLayers+1].x =  XpGroup + XpPgUp;
    edit_table[NoDialogLayers+1].y =  YpGroup + YpUp + D20 - 2 * DYComboBox - 5;
    edit_table[NoDialogLayers+1].dx = (DXBut - 8 ) / 5 * 3 ;
    edit_table[NoDialogLayers+1].dy = DYComboBox;
    edit_table[NoDialogLayers+1].border = edit_table[NoDialogLayers+1].ink = edit_table[NoDialogLayers+1].ink_ini = edit_table[NoDialogLayers+1].paper_ini = COLOR_NULL;
    edit_table[NoDialogLayers+1].paper = COLOR_NULL;
    edit_table[NoDialogLayers+1].MaxLen = 3;
    edit_table[NoDialogLayers+1].width = 3;
    edit_table[NoDialogLayers+1].enable = 1;
    edit_table[NoDialogLayers+1].id = -1;
    edit_table[NoDialogLayers+1].txt = (char *)go_to1_txt; // "1";
	edit_table[NoDialogLayers+1].legal = &numbers;

/*combobox*/
  for ( i = COMBOBOX_COLOR; i < COMBOBOX_COLOR+ NoDialogLayers ; i++)
  {
    listbox[i].x = listbox[i].y = listbox[i].dx = listbox[i].dy = 0;
    listbox[i].max =NoColors;
    listbox[i].maxw = 17 /*MAXWCOLOR*/ ;  //6

    listbox[i].poz = 7 ;
    listbox[i].flags = listbox[i].foff = 0;
    listbox[i].max_text_len = 15;
    listbox[i].border = listbox[i].ink = listbox[i].paper = COLOR_NULL;
    listbox[i].enable = 0;
    listbox[i].id = -1;
    listbox[i].back = NULL;
    listbox[i].txt = Colors_Name_txt_L;
    combobox [i].x = XpGroup + XColor;
    combobox [i].y = YpGroup + (i - COMBOBOX_COLOR + 3) * DYLayer;
    combobox [i].dx = DXColor;
    combobox [i].dy = DYComboBox;
    combobox [i].listbox = &listbox [i];

    listbox[i].maxw = min(17, (getmaxy()/wsp_y - 2 * ED_INF_HEIGHT - layers_dlg.y - combobox [i].y)/combobox [i].dy);
  }
  for ( i = COMBOBOX_WIDTH; i < COMBOBOX_WIDTH+ NoDialogLayers ; i++)
  {
    listbox[i].x = listbox[i].y = listbox[i].dx = listbox[i].dy = 0;
    listbox[i].max =NoWidthLines;
    listbox[i].poz = NoWidthLines; /*pusty lancuch inicjacyjny*/
    listbox[i].maxw = 0;
    listbox[i].flags = listbox[i].foff = 0;
    listbox[i].max_text_len = 15;
    listbox[i].border = listbox[i].ink = listbox[i].paper = COLOR_NULL;
    listbox[i].enable = 0;
    listbox[i].id = 666;
    listbox[i].back = NULL;
    listbox[i].txt = width_line_txt_L;
    combobox [i].x = XpGroup + XWidthLine;
    combobox [i].y = YpGroup + (i - COMBOBOX_WIDTH + 3) * DYLayer;
    combobox [i].dx = DXWidthLine;
    combobox [i].dy = DYComboBox;
    combobox [i].listbox = &listbox [i];
  }
  for ( i = COMBOBOX_TYPE; i < COMBOBOX_TYPE+ NoDialogLayers ; i++)
  {
    listbox[i].x = listbox[i].y = listbox[i].dx = listbox[i].dy = 0;
    listbox[i].max =NoTypeLines;
    listbox[i].poz = NoTypeLines;
    listbox[i].flags = listbox[i].foff = 0;
    listbox[i].max_text_len = 18;
    listbox[i].border = listbox[i].ink = listbox[i].paper = COLOR_NULL;
    listbox[i].enable = 0;
    listbox[i].id = 666;
    listbox[i].back = NULL;
    listbox[i].txt = type_line_txt_L;
    combobox [i].x = XpGroup + XTypeLine;
    combobox [i].y = YpGroup + (i - COMBOBOX_TYPE + 3) * DYLayer;
    combobox [i].dx = DXTypeLine-30;
    combobox [i].dy = DYComboBox;
    combobox [i].listbox = &listbox [i];

    listbox[i].maxw = min(32, (getmaxy()/wsp_y - 2 * ED_INF_HEIGHT - layers_dlg.y - combobox [i].y)/combobox [i].dy);
  }
}

static void  set_layer_to_dlg (int no_layer, int no_dlg_line)
/*-----------------------------------------------------------*/
{
  int i_menu_poz ;
  int ret;

  labels [LABEL_LAYERS_NO + no_dlg_line].txt [0] = ' ' ;  //'   ';
    ret = snprintf(&labels [LABEL_LAYERS_NO + no_dlg_line].txt [no_layer > 8 ? 0 : 1], 10, "%d",no_layer + 1);
  buttons [BUTTON_CURRENT + no_dlg_line].enable = 1;
  Set_Check_Button ( &layers_dlg, BUTTON_ONOFF + no_dlg_line,
	layers[no_layer].on);
  buttons [BUTTON_ONOFF + no_dlg_line].enable = 1;
  Set_Check_Button ( &layers_dlg, BUTTON_EDIT + no_dlg_line,
	layers[no_layer].edit);
  buttons [BUTTON_EDIT+ no_dlg_line].enable = 1;
  Set_Check_Button ( &layers_dlg, BUTTON_POINT + no_dlg_line,
	layers[no_layer].point);
  Set_Check_Button ( &layers_dlg, BUTTON_GREY + no_dlg_line,
	layers[no_layer].grey);
  Set_Check_Button ( &layers_dlg, BUTTON_BW + no_dlg_line,
	layers[no_layer].bw);
  buttons [BUTTON_POINT+ no_dlg_line].enable = 1;
  buttons [BUTTON_GREY+ no_dlg_line].enable = 1;
  buttons [BUTTON_BW+ no_dlg_line].enable = 1;

  if (erase_state==-1)
  {
      buttons [BUTTON_ERASE+ no_dlg_line].name2 = 193;
      buttons [BUTTON_ERASE+ no_dlg_line].enable = buttons[19].check;
  }
  else if (erase_state==0) //waiting for BLayer
  {
      buttons [BUTTON_ERASE+ no_dlg_line].enable = 1;
      buttons [BUTTON_ERASE+ no_dlg_line].name2 = 0;
  }
  else if (erase_state==1) //waiting for ELayer
  {
      buttons [BUTTON_ERASE+ no_dlg_line].enable = 1;
      if (no_layer==BLayer) buttons [BUTTON_ERASE+ no_dlg_line].name2 = 200;
      else buttons [BUTTON_ERASE+ no_dlg_line].name2 = 0;
  }
  else if (erase_state==2) //waiting for changing marks
  {
      if ((no_layer>=BLayer) && (no_layer<=ELayer)) buttons [BUTTON_ERASE+ no_dlg_line].name2 = 200;
      else buttons [BUTTON_ERASE+ no_dlg_line].name2 = 0;
      buttons [BUTTON_ERASE+ no_dlg_line].enable = 1;
  }
  else if (erase_state==3) //waiting for erasing or cancelling entire action
  {
      buttons [BUTTON_ERASE+ no_dlg_line].name2 = 193;
      if ((no_layer>=BLayer) && (no_layer<=ELayer)) buttons [BUTTON_ERASE+ no_dlg_line].enable = buttons[19].check;
      else buttons [BUTTON_ERASE+ no_dlg_line].enable = 0;
  }
  else  //something went wrong
  {
      ;
  }

  if (now_is_dialog)
      draw_button(&buttons[BUTTON_ERASE+ no_dlg_line]);

  edit_table [no_dlg_line].txt = layers [no_layer].name;
  edit_table[no_dlg_line].enable = 1;
  i_menu_poz = Color_No_to_Menu_No (layers [no_layer].color) ;
  if (i_menu_poz>=16) 
  {
  listbox[COMBOBOX_COLOR + no_dlg_line].wartosc=i_menu_poz + 1;
  i_menu_poz=16;
  }
  else listbox[COMBOBOX_COLOR + no_dlg_line].wartosc=i_menu_poz + 1;
  Get_Menu_Param (i_menu_poz,
    listbox[COMBOBOX_COLOR + no_dlg_line].maxw,
    listbox[COMBOBOX_COLOR + no_dlg_line].max,
    &listbox[COMBOBOX_COLOR + no_dlg_line].foff,
    &listbox[COMBOBOX_COLOR + no_dlg_line].poz);
  listbox[COMBOBOX_COLOR + no_dlg_line].enable = 1;
  listbox[COMBOBOX_WIDTH+ no_dlg_line].poz = 
    ((layers [no_layer].line_type & 224) / 32) ;
      
  listbox[COMBOBOX_WIDTH+ no_dlg_line].enable = 1;

  i_menu_poz = (layers [no_layer].line_type & 31);
    
  Get_Menu_Param (i_menu_poz,
    listbox[COMBOBOX_TYPE + no_dlg_line].maxw,
    listbox[COMBOBOX_TYPE + no_dlg_line].max,
    &listbox[COMBOBOX_TYPE + no_dlg_line].foff,
    &listbox[COMBOBOX_TYPE + no_dlg_line].poz) ; 

  listbox[COMBOBOX_TYPE+ no_dlg_line].enable = 1;
}

static void  disable_dlg_layer_controls (int no_dlg_line)
/*------------------------------------------------------*/
{
  strcpy (labels [LABEL_LAYERS_NO + no_dlg_line].txt, "   ") ;
  buttons [BUTTON_CURRENT + no_dlg_line].enable = 0;
  Set_Check_Button ( &layers_dlg, BUTTON_ONOFF + no_dlg_line, 0);
  buttons [BUTTON_ONOFF + no_dlg_line].enable = 0;
  Set_Check_Button ( &layers_dlg, BUTTON_EDIT + no_dlg_line, 0);
  buttons [BUTTON_EDIT+ no_dlg_line].enable = 0;
  Set_Check_Button ( &layers_dlg, BUTTON_POINT + no_dlg_line, 0);
  Set_Check_Button ( &layers_dlg, BUTTON_GREY + no_dlg_line, 0);
  Set_Check_Button ( &layers_dlg, BUTTON_BW + no_dlg_line, 0);
  buttons [BUTTON_POINT+ no_dlg_line].enable = 0;
  buttons [BUTTON_GREY+ no_dlg_line].enable = 0;
  buttons [BUTTON_BW+ no_dlg_line].enable = 0;
  buttons [BUTTON_ERASE+ no_dlg_line].enable = 0;
  edit_table [no_dlg_line].txt = "";
  edit_table[no_dlg_line].enable = 0;
  Get_Menu_Param (NoColors,
    listbox[COMBOBOX_COLOR + no_dlg_line].maxw,
    listbox[COMBOBOX_COLOR + no_dlg_line].max,
    &listbox[COMBOBOX_COLOR + no_dlg_line].foff,
    &listbox[COMBOBOX_COLOR + no_dlg_line].poz) ;
  listbox[COMBOBOX_COLOR + no_dlg_line].enable = 0;
  listbox[COMBOBOX_COLOR + no_dlg_line].wartosc = 0;
  listbox[COMBOBOX_WIDTH+ no_dlg_line].poz = NoWidthLines;
  listbox[COMBOBOX_WIDTH+ no_dlg_line].enable = 0;
  
  Get_Menu_Param (NoTypeLines,
    listbox[COMBOBOX_TYPE + no_dlg_line].maxw,
    listbox[COMBOBOX_TYPE + no_dlg_line].max,
    &listbox[COMBOBOX_TYPE + no_dlg_line].foff,
    &listbox[COMBOBOX_TYPE + no_dlg_line].poz) ;
    
  listbox[COMBOBOX_TYPE+ no_dlg_line].enable = 0;
}


static void  get_dlg_controls_to_layer (int no_layer, int no_dlg_line)
/*--------------------------------------------------------------------*/
{
  int i_menu_poz ;

  layers[no_layer].on = Get_Check_Button ( &layers_dlg,
		BUTTON_ONOFF + no_dlg_line);
  layers[no_layer].edit = Get_Check_Button ( &layers_dlg,
		BUTTON_EDIT + no_dlg_line);
  layers[no_layer].point = Get_Check_Button ( &layers_dlg,
		BUTTON_POINT + no_dlg_line);
  layers[no_layer].grey = Get_Check_Button ( &layers_dlg,
		BUTTON_GREY + no_dlg_line);
  layers[no_layer].bw = Get_Check_Button ( &layers_dlg,
		BUTTON_BW + no_dlg_line);
  i_menu_poz = listbox [COMBOBOX_COLOR + no_dlg_line].foff + listbox[COMBOBOX_COLOR + no_dlg_line].poz ;
  if (i_menu_poz>=16)
   {
    i_menu_poz = listbox [COMBOBOX_COLOR + no_dlg_line].wartosc - 1; 
   }
  layers [no_layer].color = Menu_No_to_Color_No (i_menu_poz) ;
  layers [no_layer].line_type = listbox[COMBOBOX_WIDTH+ no_dlg_line].poz * 32
		 + listbox[COMBOBOX_TYPE+ no_dlg_line].poz + listbox[COMBOBOX_TYPE + no_dlg_line].foff;		
}

static void get_dlg_controls_to_layers (void)
/*------------------------------------------*/
{
  int i, k;

  n_last_layer_in_dlg = n_first_layer_in_dlg + NoDialogLayers - 1;
  if (n_last_layer_in_dlg >= no_layers)
  {
    n_last_layer_in_dlg = no_layers - 1;
  }
  for (i = n_first_layer_in_dlg, k = 0; i <= n_last_layer_in_dlg; i++, k++)
  {
    get_dlg_controls_to_layer (i, k);
  }
}


static void draw_dlg_layer_controls (int no_dlg_line)
/*---------------------------------------------------*/
{
  Draw_Label (&labels [LABEL_LAYERS_NO + no_dlg_line], TRUE) ;
  Draw_Button (&buttons [BUTTON_ONOFF + no_dlg_line]);
  Draw_Button (&buttons [BUTTON_EDIT + no_dlg_line]);
  Draw_Button (&buttons [BUTTON_POINT + no_dlg_line]);
  Draw_Button (&buttons [BUTTON_GREY + no_dlg_line]);
  Draw_Button (&buttons [BUTTON_BW + no_dlg_line]);


  Draw_Input_Line (&edit_table [no_dlg_line]);
  Draw_Input_Line (&edit_table [NoDialogLayers]);
  Draw_Input_Line (&edit_table [NoDialogLayers+1]);
  Draw_ComboBox (&combobox [COMBOBOX_COLOR + no_dlg_line]);
  Draw_ComboBox (&combobox [COMBOBOX_WIDTH+ no_dlg_line]);
  Draw_ComboBox (&combobox [COMBOBOX_TYPE+ no_dlg_line]);
}


static void  init_layer_dlg_control (BOOL b_draw)
/*--------------------------------------------*/
{
  int current, i, k;

  n_last_layer_in_dlg = n_first_layer_in_dlg + NoDialogLayers - 1;
  if (n_last_layer_in_dlg >= no_layers)
  {
    n_last_layer_in_dlg = no_layers - 1;
    n_first_layer_in_dlg = max(n_last_layer_in_dlg - NoDialogLayers + 1, 0);
  }
  current = current_layer - n_first_layer_in_dlg;
  Check_Radio_Button ( &layers_dlg,BUTTON_CURRENT,
      BUTTON_CURRENT+ NoDialogLayers  -1, BUTTON_CURRENT + current, b_draw);
  for (i = n_first_layer_in_dlg, k = 0; i <= n_last_layer_in_dlg; i++, k++)
  {
    set_layer_to_dlg (i, k);
    if (b_draw == TRUE)
    {
      draw_dlg_layer_controls (k);
    }
  }
  for (i = k ; i < NoDialogLayers; i++)
  {
    disable_dlg_layer_controls (i);
    if (b_draw == TRUE)
    {
      draw_dlg_layer_controls (i);
    }
  }
}


static BOOL get_layer_dlg_control (void)
/*-----------------------------------*/
{
  BOOL ret_val;

  ret_val = TRUE;
  get_dlg_controls_to_layers ();
  if (layers [current_layer].on == 0)
  {
    ErrList (37);
    ret_val = FALSE;
  }
  return ret_val;
}


static void set_slide_flags(void)
{
    SLIDER *slider=layers_dlg.Sliders;
    slider[0].flags |= 0x800;
}

static void update_slide(void)
{
    int var1, var2, var3, var4, ret;
    SLIDER *slider=layers_dlg.Sliders;

    int (*SlideFun)(int *, int *, int *, int *);
    SlideFun = (int (*)(int *, int *, int *, int *)) slider[0].dp3;
    ret = SlideFun(&var1, &var2, &var3, &var4);
    slider[0].d2 = var3 - var1 - (var2 - var1);
    komunikat_str_short("", FALSE, FALSE);
}

static void add_layer (void)
/*-------------------------*/
{
  LAYER layer=Defwar;
  int no_dlg_line;

  if (no_layers >= MAX_NUMBER_OF_LAYERS)
  {
    ErrList (36);
    return;
  }
  memmove( &(layers[no_layers]), &layer, sizeof(LAYER));
  strcpy (layers [no_layers].name, "");
  no_layers++;

  //No_Layers++;

  if (no_layers <= NoDialogLayers)
  {
    no_dlg_line = no_layers -1;
    n_last_layer_in_dlg = no_dlg_line;
    set_layer_to_dlg (no_layers -1, no_dlg_line);

      update_slide();
      set_slide_flags();

    draw_dlg_layer_controls (no_dlg_line);
    Draw_Slider(layers_dlg.Sliders);
  }
  else
  {
    get_dlg_controls_to_layers ();
    n_first_layer_in_dlg = no_layers - NoDialogLayers;
    init_layer_dlg_control (FALSE);
      update_slide();
      set_slide_flags();
      Redraw_Dlg(&layers_dlg);
  }
}

static void up_layer (void)
/*-------------------------*/
{

  if (n_first_layer_in_dlg == 0)
  {
    return;
  }
  get_dlg_controls_to_layers ();
  n_first_layer_in_dlg--;
  init_layer_dlg_control (FALSE);
    update_slide();
  Redraw_Dlg(&layers_dlg);
}

static void dn_layer (void)
/*-------------------------*/
{

  if (no_layers <= NoDialogLayers ||
      n_first_layer_in_dlg + NoDialogLayers >= no_layers)
  {
    return;
  }
  get_dlg_controls_to_layers ();
  n_first_layer_in_dlg++;
  init_layer_dlg_control (FALSE);
    update_slide();
  Redraw_Dlg(&layers_dlg);
}

static void goto_layer (int to_which)
/*---------------------------------*/
{ int which_layer;


  get_dlg_controls_to_layers ();
  if (to_which==0) which_layer=atoi(edit_table[NoDialogLayers].txt)-1;
    else which_layer=atoi(edit_table[NoDialogLayers+1].txt)-1;

  if (which_layer<0) which_layer=0;
  if (which_layer + NoDialogLayers > no_layers)
  {
    which_layer =  no_layers - NoDialogLayers ;
  }
  if (no_layers <= NoDialogLayers ||
       which_layer >= no_layers)
  {
    return ;
  }
  n_first_layer_in_dlg = which_layer ;
  init_layer_dlg_control (FALSE);
    update_slide();
  Redraw_Dlg(&layers_dlg);
}

static void pgup_layer (void)
/*-------------------------*/
{

  if (n_first_layer_in_dlg == 0)
  {
    return;
  }
  get_dlg_controls_to_layers ();
  n_first_layer_in_dlg -= NoDialogLayers ;
  if (n_first_layer_in_dlg < 0)
  {
    n_first_layer_in_dlg = 0 ;
  }
  init_layer_dlg_control (FALSE);
    update_slide();
  Redraw_Dlg(&layers_dlg);
}


static void pgdn_layer (void)
/*-------------------------*/
{

  if (no_layers <= NoDialogLayers ||
      n_first_layer_in_dlg + NoDialogLayers >= no_layers)
  {
    return ;
  }
  get_dlg_controls_to_layers () ;
  n_first_layer_in_dlg += NoDialogLayers ;
  if (n_first_layer_in_dlg + NoDialogLayers > no_layers)
  {
    n_first_layer_in_dlg =  no_layers - NoDialogLayers ;
  }
  init_layer_dlg_control (FALSE) ;
    update_slide();
  Redraw_Dlg(&layers_dlg);
}

int up_layer_ (void)
{
  up_layer();
  return 0;
}

int dn_layer_ (void)
{
  dn_layer();
  return 0;
}

int pgup_layer_ (void)
{
  pgup_layer();
  return 0;
}

int pgdn_layer_ (void)
{
  pgdn_layer();
  return 0;
}

static void set_WEP_all (int typ)
/*-----------------------------*/
{
 int i, k;
 
 for (i = n_first_layer_in_dlg, k = 0; i <= n_last_layer_in_dlg; i++, k++)
  {
  layers[i].on = Get_Check_Button ( &layers_dlg, BUTTON_ONOFF + k);
  layers[i].edit = Get_Check_Button ( &layers_dlg, BUTTON_EDIT + k);
  layers[i].point = Get_Check_Button ( &layers_dlg, BUTTON_POINT + k);
  layers[i].grey = Get_Check_Button ( &layers_dlg, BUTTON_GREY + k);
  layers[i].bw = Get_Check_Button ( &layers_dlg, BUTTON_BW + k);
  }		
 
 for (i=0; i<no_layers; i++)
 {
  switch (typ)
  {
   case 0:layers[i].on=1;
          break;
   case 1:layers[i].edit=1;
          break; 
   case 2:layers[i].point=1;
          break;         
   case 3:layers[i].on=0;
          break;        
   case 4:layers[i].edit=0;
          break; 
   case 5:layers[i].point=0;
          break;  
   case 6:layers[i].grey=1; 
	      layers[i].bw=0;
          break;  
   case 7:layers[i].bw=1;
	      layers[i].grey=0;
          break;  
   case 8:layers[i].grey=0;
          break;  
   case 9:layers[i].bw=0;
          break;  
   default: 
          break;       
  }
 }
 for (i = n_first_layer_in_dlg, k = 0; i <= n_last_layer_in_dlg; i++, k++)
  {
    Set_Check_Button ( &layers_dlg, BUTTON_ONOFF + k,
	layers[i].on);
    buttons [BUTTON_ONOFF + k].enable = 1;
    Set_Check_Button ( &layers_dlg, BUTTON_EDIT + k, layers[i].edit);
    buttons [BUTTON_EDIT+ k].enable = 1;
    Set_Check_Button ( &layers_dlg, BUTTON_POINT + k, layers[i].point);
	Set_Check_Button ( &layers_dlg, BUTTON_GREY + k, layers[i].grey);
	Set_Check_Button ( &layers_dlg, BUTTON_BW + k, layers[i].bw);
    buttons [BUTTON_POINT+ k].enable = 1;
	buttons [BUTTON_GREY+ k].enable = 1;
	buttons [BUTTON_BW+ k].enable = 1;
    
    Draw_Button (&buttons [BUTTON_ONOFF + k]);
    Draw_Button (&buttons [BUTTON_EDIT + k]);
    Draw_Button (&buttons [BUTTON_POINT + k]);
	Draw_Button (&buttons [BUTTON_GREY + k]);
	Draw_Button (&buttons [BUTTON_BW + k]);
  }
 return;
}

int erase_layer(int layer_to_erase)
{
    int ret;
    int key;

    char layer_name[72];

    sprintf(layer_name,"%d  %s", layer_to_erase+1, layers_table[layer_to_erase].name);  //new layer can be erased before creation, just nothing will happen

    ret=ask_question_in_dialog(2, _No_,_Yes_,"", _ERASE_LAYER_, 12, layer_name, 11, 1, 61);
    if (ret==1) key=_YES_;
    else if (ret==2) key=_NO_;
    else return 0;
    if (key == _YES_ || key == _yes_)
    {
        delete_all_from_layer_atrybut (layer_to_erase, ANieOkreslony);
        shift_layer_numbers(layer_to_erase);

        for (int i=layer_to_erase+1; i<No_Layers; i++)
            memcpy ( &Layers[i-1],  &Layers[i], sizeof (LAYER));

        if (Current_Layer>layer_to_erase) Current_Layer-=1;
        if (Current_Layer<0) Current_Layer=0;

        No_Layers-=1;
    }
    return ret;
}

int erase_multiple_layers(int first_layer_to_erase, int last_layer_to_erase)
{
    int ret;
    int key;

    char b_layer_name[72];
    char e_layer_name[72];

    sprintf(b_layer_name,"%s: %d  %s", _FROM_, first_layer_to_erase+1, layers_table[first_layer_to_erase].name);  //new layer can be erased before creation, just nothing will happen
    sprintf(e_layer_name,"%s: %d  %s", _TO_, last_layer_to_erase+1, layers_table[last_layer_to_erase].name);  //new layer can be erased before creation, just nothing will happen

    ret=ask_question_in_dialog(2, _No_,_Yes_,_ERASE_LAYER_, b_layer_name, 12, e_layer_name, 11, 1, 61);
    if (ret==1) key=_YES_;
    else if (ret==2) key=_NO_;
    else return 0;
    if (key == _YES_ || key == _yes_)
    {
        for (int il=first_layer_to_erase; il<=last_layer_to_erase; il++)
        {
            delete_all_from_layer_atrybut(first_layer_to_erase, ANieOkreslony);
            shift_layer_numbers(first_layer_to_erase);

            for (int i = first_layer_to_erase + 1; i < No_Layers; i++)
                memcpy(&Layers[i - 1], &Layers[i], sizeof(LAYER));

            if (Current_Layer > first_layer_to_erase) Current_Layer -= 1;
            if (Current_Layer < 0) Current_Layer = 0;

            No_Layers -= 1;
        }
    }
    return ret;
}


static int proc_dlg_layer (int n)
/*------------------------------*/
{ int ret = Dlg_Ret_Val_Continue, ret1;
  int layer_to_erase;

  //SLIDER *slider=layers_dlg.Sliders;

    if (n==ID_OK) {
        if (get_layer_dlg_control() == TRUE) {
            ret = Dlg_Ret_Val_OK;
        }
    }
    else if (n==ID_CANCEL) {
        ret = Dlg_Ret_Val_Cancel;
    }
    else if ((n>=BUTTON_CURRENT) && (n<(BUTTON_CURRENT + NoDialogLayers))) {
        current_layer = (n - BUTTON_CURRENT) + n_first_layer_in_dlg;
        Check_Radio_Button(&layers_dlg, BUTTON_CURRENT,
                           BUTTON_CURRENT + NoDialogLayers - 1, n, TRUE);
    }
    else if ((n>=BUTTON_GREY) && (n<(BUTTON_GREY + NoDialogLayers))) {
        if (layers[n_first_layer_in_dlg + n - BUTTON_GREY].grey == 0) {
            layers[n_first_layer_in_dlg + n - BUTTON_GREY].grey = 1;
            layers[n_first_layer_in_dlg + n - BUTTON_GREY].bw = 0;
            Set_Check_Button(&layers_dlg, BUTTON_BW + n - BUTTON_GREY, 0);
            //buttons [BUTTON_BW + n - BUTTON_GREY].enable = 0;
            Draw_Button(&buttons[BUTTON_BW + n - BUTTON_GREY]);
        } else layers[n_first_layer_in_dlg + n - BUTTON_GREY].grey = 0;
    }
    else if ((n>=BUTTON_BW) && (n<(BUTTON_BW + NoDialogLayers))) {
        if (layers[n_first_layer_in_dlg + n - BUTTON_BW].bw == 0) {
            layers[n_first_layer_in_dlg + n - BUTTON_BW].bw = 1;
            layers[n_first_layer_in_dlg + n - BUTTON_BW].grey = 0;
            Set_Check_Button(&layers_dlg, BUTTON_GREY + n - BUTTON_BW, 0);
            Draw_Button(&buttons[BUTTON_GREY + n - BUTTON_BW]);
        } else layers[n_first_layer_in_dlg + n - BUTTON_BW].bw = 0;
    }
    else if (n==ID_ADD)
    add_layer ();
    else if (n==ID_GoTo)
      goto_layer (0);
    else if (n==ID_GoTo1)
      goto_layer (1);
    else if (n==ID_Up)
      up_layer ();
    else if (n==ID_Dn)
      dn_layer ();
    else if (n==ID_PgUp)
      pgup_layer ();
    else if (n==ID_PgDn)
      pgdn_layer ();
    else if (n==ID_SET_W_ALL)
      set_WEP_all (0);
    else if (n==ID_SET_E_ALL)
      set_WEP_all (1);
    else if (n==ID_SET_P_ALL)
      set_WEP_all (2);
    else if (n==ID_RESET_W_ALL)
      set_WEP_all (3);
    else if (n==ID_RESET_E_ALL)
      set_WEP_all (4);
    else if (n==ID_RESET_P_ALL)
      set_WEP_all (5);
    else if (n==ID_SET_G_ALL)
	  set_WEP_all (6);
    else if (n==ID_SET_BW_ALL)
	  set_WEP_all (7);
    else if (n==ID_RESET_G_ALL)
	  set_WEP_all (8);
    else if (n==ID_RESET_BW_ALL)
	  set_WEP_all (9);
    else if (n==ID_BACK)
    {
          get_dlg_controls_to_layers ();

          for (int ii=0; ii<8; ii++) b_layers[ii]=0;
          BOOL is_changed=FALSE;
          BOOL layers_on;
          for (int i=0; i<no_layers; i++)
          {
              layers_on=layers[i].on;

              if (layers[i].on) SetBit(b_layers, i);

              unsigned int bb=TestBit(b_layers_bak, i);

              layers[i].on=(bb>0) ? 1:0;

              if (layers_on!=layers[i].on)
                  is_changed=TRUE;
          }

          if (is_changed)
          {
              init_layer_dlg_control (FALSE);
              Redraw_Dlg(&layers_dlg);
          }

          //saving
          for (int ii=0; ii<8; ii++) b_layers_bak[ii]=b_layers[ii];

    }
    else if (n==ID_ERASE)
      {
      if (buttons[19].check==1)
      {
          if (buttons[20].check==1)
          {
              buttons[20].check = 0;
              redraw_button(&buttons[20]);
          }
          if (erase_state<2) erase_state=-1;
          else erase_state=3;  //ready to block deletion
      }
      else
      {
          reset_erase_state();
          reset_BLayerELayer();
      }
      if (erase_state==-1)
        {
            for (int i = BUTTON_ERASE; i < BUTTON_ERASE + NoDialogLayers; i++) {
                buttons[i].enable = buttons[19].check && buttons[i - BUTTON_ERASE + BUTTON_CURRENT].enable;
                buttons[i].name2=193;
                buttons[i].enable = buttons[19].check && buttons[i - BUTTON_ERASE + BUTTON_CURRENT].enable;
                buttons[i].flags = (buttons[19].check == 1) ? 0 : BUTTON_HIDDEN;
                redraw_button(&buttons[i]);
            }
        }
        else if (erase_state==3)
          {
              for (int i = BUTTON_ERASE; i < BUTTON_ERASE + NoDialogLayers; i++) {
                  buttons[i].enable = buttons[19].check && buttons[i - BUTTON_ERASE + BUTTON_CURRENT].enable;
                  buttons[i].name2=193;
                  int layer_in_row = (i - BUTTON_ERASE) + n_first_layer_in_dlg;
                  if ((layer_in_row >= BLayer) && (layer_in_row <= ELayer)) buttons[i].enable = buttons[19].check && buttons[i - BUTTON_ERASE + BUTTON_CURRENT].enable;
                  else buttons[i].enable = 0;
                  buttons[i].flags = (buttons[19].check == 1) ? 0 : BUTTON_HIDDEN;
                  redraw_button(&buttons[i]);
              }
          }
      }
    else if (n==ID_MARK)
    {
        if (buttons[20].check==1)
        {
            if (buttons[19].check==1)
            {
                buttons[19].check = 0;
                redraw_button(&buttons[19]);
            }
            if (erase_state<3)
                 erase_state=0;
            else erase_state=2;  //back to edit
        }
        else
        {
            reset_erase_state();
            reset_BLayerELayer();
        }
        for ( int i = BUTTON_ERASE; i < BUTTON_ERASE+ NoDialogLayers ; i++)
        {
            buttons [i].enable = buttons[20].check && buttons [i - BUTTON_ERASE + BUTTON_CURRENT].enable;
            int layer_in_row = (i - BUTTON_ERASE) + n_first_layer_in_dlg;
            if ((layer_in_row==BLayer) || (layer_in_row<=ELayer)) buttons[i].name2=200;
            buttons[i].name2=0;
            buttons [i].flags = (buttons[20].check==1) ? 0 : BUTTON_HIDDEN;
            redraw_button(&buttons[i]);
        }
    }
    else if ((n>=BUTTON_ERASE) && (n<(BUTTON_ERASE + NoDialogLayers)))
    {
        if (erase_state==-1)  //sigle delete
        {
            //saving changes
            get_dlg_controls_to_layers();
            Current_Layer = current_layer;
            layer_to_erase = (n - BUTTON_ERASE) + n_first_layer_in_dlg;

            if (current_layer == layer_to_erase) {
                ErrList(196);
            } else {
                //////////////////////
                n_first_layer_in_dlg_old = n_first_layer_in_dlg;
                Current_Layer = current_layer;
                if ((No_Layers != no_layers) ||
                    (0 != memcmp(layers_table, Layers, No_Layers * sizeof(LAYER)))) {
                    No_Layers = no_layers;
                    memcpy(Layers, layers_table, No_Layers * sizeof(LAYER));
                    Change = TRUE;
                }

                //////////////////////
                ret1 = erase_layer(layer_to_erase);
                if (ret1 == 1) {
                    layers = layers_table;
                    no_layers = No_Layers;
                    current_layer = Current_Layer;
                    set_struct_dialog_control(1);
                    memcpy(layers_table, Layers, No_Layers * sizeof(LAYER));
                    init_layer_dlg_control(FALSE);

                    Get_Default_Color_Dlg(&color_dlg);
                    b_redraw = TRUE;

                    update_slide();
                    set_slide_flags();

                    Redraw_Dlg(&layers_dlg);
                }
            }
        }
        else if (erase_state==0)  //BLayer selection
        {
            int layer_to_mark = (n - BUTTON_ERASE) + n_first_layer_in_dlg;

            BLayer=layer_to_mark;
            buttons [n].name2=200;
            redraw_button(&buttons[n]);
            erase_state=1;
        }
        else if (erase_state==1) //BLayer selected, ELayer selection
        {
            int layer_to_mark = (n - BUTTON_ERASE) + n_first_layer_in_dlg;
            if (layer_to_mark==BLayer)  //untag and get back to previous step
            {
                buttons [n].name2=0;
                redraw_button(&buttons[n]);
                erase_state=0;
            }
            else
            {
                ELayer=layer_to_mark;
                if (ELayer<BLayer)
                {
                    int blayer=BLayer;
                    BLayer=ELayer;
                    ELayer=blayer;
                }
                int nb= max(BLayer + BUTTON_ERASE - n_first_layer_in_dlg, BUTTON_ERASE);
                int ne= min(ELayer + BUTTON_ERASE - n_first_layer_in_dlg, BUTTON_ERASE + NoDialogLayers - 1);

                for (int i=nb; i<=ne; i++)
                {
                    buttons[i].name2 = 200;
                    redraw_button(&buttons[i]);
                }
                erase_state=2;
            }
        }
        else if (erase_state==2) //ELayer selected, so now all is to be untab, and start again
        {
            int nb= max(BLayer + BUTTON_ERASE - n_first_layer_in_dlg, BUTTON_ERASE);
            int ne= min(ELayer + BUTTON_ERASE - n_first_layer_in_dlg, BUTTON_ERASE + NoDialogLayers);

            for (int i=nb; i<=ne; i++)
            {
                buttons[i].name2 = 0;
                redraw_button(&buttons[i]);
            }
            int layer_to_mark = (n - BUTTON_ERASE) + n_first_layer_in_dlg;

            ELayer=-1;

            BLayer=layer_to_mark;
            buttons [n].name2=200;
            redraw_button(&buttons[n]);
            erase_state=1;
        }
        else if (erase_state==3) //Erase block
        {
            //saving changes
            get_dlg_controls_to_layers();
            Current_Layer = current_layer;
            layer_to_erase = (n - BUTTON_ERASE) + n_first_layer_in_dlg;

            if ((current_layer >= BLayer) &&  (current_layer <= ELayer)) {
                ErrList(196);
            } else {
                //////////////////////
                n_first_layer_in_dlg_old = n_first_layer_in_dlg;
                Current_Layer = current_layer;
                if ((No_Layers != no_layers) ||
                    (0 != memcmp(layers_table, Layers, No_Layers * sizeof(LAYER)))) {
                    No_Layers = no_layers;
                    memcpy(Layers, layers_table, No_Layers * sizeof(LAYER));
                    Change = TRUE;
                }

                //////////////////////
                ret1 = erase_multiple_layers(BLayer, ELayer);
                if (ret1 == 1) {
                    reset_erase_state();
                    reset_BLayerELayer();

                    layers = layers_table;
                    no_layers = No_Layers;
                    current_layer = Current_Layer;
                    set_struct_dialog_control(1);
                    memcpy(layers_table, Layers, No_Layers * sizeof(LAYER));
                    init_layer_dlg_control(FALSE);

                    Get_Default_Color_Dlg(&color_dlg);
                    b_redraw = TRUE;

                    update_slide();
                    set_slide_flags();

                    Redraw_Dlg(&layers_dlg);
                }
            }

        }
    }
  return ret;
}

void Ini_Layers_Dlg (void)
/*------------------------*/
{
  n_first_layer_in_dlg_old = 0 ;
}

void reset_b_layers_bak(void)
{
    b_layers_bak_set=TRUE;
}

void Set_Layers (void)
/*----------------*/
{
  int i, image_no ;
  static int komunikat_R, komunikat_R0;
  int k;
  static int curr_h, curr_v;
    
  komunikat_R=Komunikat_R;
  komunikat_R0 = Komunikat_R0 ;
  image_no = Save_Message0_Image () ; 
  komunikat(0);
  
  if (NULL == (layers_table = malloc /*getmem*/ (MAX_NUMBER_OF_LAYERS * sizeof(LAYER))))
  {
    ErrList (18) ;
    return ;
  }
  
  n_first_layer_in_dlg = n_first_layer_in_dlg_old;
  layers = layers_table;
  no_layers = No_Layers;
  current_layer = Current_Layer;
  set_struct_dialog_control (0);
  memcpy ( layers_table, Layers,  No_Layers * sizeof (LAYER));
  Get_Default_Color_Dlg (&color_dlg);
  init_layer_dlg_control (FALSE);
  b_redraw = FALSE ;

  for (int ii=0; ii<8; ii++) b_layers_curr[ii]=0;
  for (int i=0; i<No_Layers; i++) if (Layers[i].on) SetBit(b_layers_curr, i);
  if (b_layers_bak_set==FALSE)
  {
      for (int ii=0; ii<8; ii++) b_layers_bak[ii]=b_layers_curr[ii];
      b_layers_bak_set=TRUE;
  }

  Save_Update_flex(0, &curr_h, &curr_v);
  if (Dialog(&layers_dlg, &color_dlg, proc_dlg_layer, bMouse) == Dlg_Ret_Val_OK ) 
  {
	if ((options1.view_only_current_layer == 1) && (current_layer != Current_Layer)) b_redraw = TRUE;

    n_first_layer_in_dlg_old = n_first_layer_in_dlg;
    Current_Layer = current_layer;
    if ((No_Layers != no_layers) ||
	    (0 != memcmp (layers_table, Layers, No_Layers * sizeof (LAYER))))
    {
      for (i = 0 ; i < No_Layers ; i++)
      {
	  if (Layers [i].on != layers_table [i].on)
	   {
	     b_redraw = TRUE ;
	   }
      if (Warstwy_On_Solids[i].check == 1)
      {
          if (Warstwy_On_Solids[i].on != Layers[i].on)
          {
              b_redraw = TRUE;
          }
      }
      }
      No_Layers = no_layers;
      memcpy (Layers, layers_table, No_Layers * sizeof (LAYER));

      for (int ii=0; ii<8; ii++) b_layers_bak[ii]=b_layers_curr[ii];

	  Change=TRUE;
    }

  }

  Save_Update_flex(1, &curr_h, &curr_v);

  if (b_redraw == TRUE)
  {
    Image_Free () ;
    ////Clear_Client_Screen ();  //21-08-2021
    Ini_Global_Object ();
	//sprawdzenie czy potrzeba regenerowac bitmapy
	for (i=0; i<No_Layers; i++)  //ewentualnie 256
    {
	   if (Warstwy_On_PCX[i].check==1)
	   {
         if (Warstwy_On_PCX[i].on!=Layers[i].on) 
		 {
			 regen_ctx=TRUE;
			 break;
		 }
	   }
    }
    redraw ();
  }
  else
  {
    Cur_offd (X, Y) ;
    Ini_Global_Object ();
    Cur_ond (X,Y) ;
  }
  free(layers_table);
  
  komunikat(komunikat_R);
  Komunikat_R0 = komunikat_R0 ;
  Put_Message0_Image (image_no) ;
    ClearErr ();
    ClearInfo ();
  return;
}


int Set_Layers_ (void)
/*----------------*/
{
 Set_Layers ();
 return 0;
}

#undef __LAYERS__