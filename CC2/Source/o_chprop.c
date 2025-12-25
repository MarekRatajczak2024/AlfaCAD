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

#define __CHPROP__
#include <forwin.h>
#include <string.h>
#define ALLEGWIN
#include <allegext.h>
#include <stdlib.h>

#include "bib_e.h"

#include "b_bibe.h"

#include "rysuj_e.h"
#include "o_dialog.h"
#include "o_libfun.h"
#include "o_object.h"
#include "o_image.h"
#include "alffont.h"
#include "alf_res.h"
#include "bib_blok.h"

#include "menu.h"

extern PPOLE pmCzcionka [] ;
extern int Set_WidthSolid_to_Type (int type, int width);
extern int Save_Screen(void);
extern int Restore_Screen(void);
extern char digits[16];
extern char numbers[16];
extern int Add_New_Font_TTF_OTF(char *name, int type, int cur);
extern void rysuj_bitmap_outline(char* ad, int mode, BOOL enforce);
extern void blokzap_no_pcx(char* adp, char* adk, int atrybut, int mode, int kolor);
extern TMENU mTTF_OTF;
extern int Simple_Menu_Proc_(TMENU *menu);
extern void lock_mouse(void);
extern void free_mouse(void);
extern void GrMouseEraseCursor(void);
extern void GrMouseUnEraseCursor(void);
extern int PozX0,PozY0;
extern float jednostkiX0_(long pixels);
extern float jednostkiY0_(long pixels);
extern long pXp,pYp,pXk,pYk;
extern double Get_Ds(void);
extern double Get_Ds_x(void);

static T_change_param s__change_param =  {TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE,
					 0, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1} ;
static T_change_param s__change_param0 = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 
					 0, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1} ;
static T_change_param s__change_param00 = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                     0, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1};
static void change_properties (char  *, char  *, T_change_param *) ;

static int m_x, m_y;


#define XpGr	        5 /*10*/
#define YpGr       	5 /*10*/
#define DXGroup 	185 //213//300
#define DYGroup 	260 //280 /*270*/
#define DXName 		40 //60 ////120
#define DYName 		15 //18 //(2 + DYLab) //(3 + DYLab)
#define DXCheckBox 	15
#define DXDist1		6

#define YpGrLayer	(YpGr+12)
#define YpGrColor	(YpGrLayer + DYName)
#define YpGrWidth	(YpGrColor + DYName)
#define YpGrType     (YpGrWidth + DYName)
#define YpGrText 	(YpGrType + DYName)
#define YpGrFontName 	(YpGrText + 0.5*DYName)
#define YpGrFontType    (YpGrFontName + 1*DYName)
#define YpGrFontJust    (YpGrFontName + 3*DYName)
#define YpGrFontHeight  (YpGrFontName + 4*DYName)
#define YpGrFontWidth   (YpGrFontName + 5*DYName)
#define YpGrFontItalics (YpGrFontName + 6*DYName)
#define YpGrFontUnderline (YpGrFontName + 7*DYName)

#define XpYesButton 50
#define XpNoButton 105

#define XpName 		(XpGr + DXDist1)
#define XpComboBox  	(XpGr + DXName)
#define XpButton	(XpComboBox + DXLayerName + DXDist1)

#define XLayButton	XpButton
#define XLayerName      XpComboBox
#define DXLayerName     (14 * 8 + 10) //(17 * 8 + 10)

#define XColButton      XpButton
#define XColor          XpComboBox
#define DXColor  	(11 * 8 + 10)

#define XWidthButton    XpButton
#define XWidthLine	XpComboBox
#define DXWidthLine     (7 * 8 + 10)

#define XTypeButton	XpButton
#define XTypeLine	XpComboBox
#define DXTypeLine	(13 * 8 + 10)

#define XFontButton	XpButton
#define XFontName	XpComboBox
#define DXFontName	(13 * 8 + 10)


#define XpOK    	     (XpGr + DXGroup + 13)
#define YpOK            174
#define XpCANCEL        XpOK
#define YpCANCEL	    221


#define DYChange        DYLab
#define DYComboBox	12 //14 //(ED_INF_HEIGHT + 2)

#define NoLayers 	MAX_NUMBER_OF_LAYERS
#define NoColors	MAXCOLOR
#define NoTranslucency 20
#define BUTTON_SIZE	33 /*17*/
#define COMBOBOX_SIZE	8/*5*/

#define ID_OK		1
#define ID_CANCEL      	2
#define ID_BLayer       3
#define ID_BColor       4
#define ID_BWidth       5
#define ID_BType        6
#define ID_BFontName       7
#define ID_BFontType       8
#define ID_BFontUkryty     9
#define ID_BFontUkrytyYes  10
#define ID_BFontUkrytyNo   11
#define ID_BFontJust       12
#define ID_BFontHeight     13/*8*/
#define ID_BFontWidth      14/*9*/
#define ID_BFontItalics    15/*10*/
#define ID_BFontItalicsYes 16/*11*/
#define ID_BFontItalicsNo  17/*12*/

#define ID_BFontBold    18/*20*/
#define ID_BFontBoldYes 19/*21*/
#define ID_BFontBoldNo  20/*22*/
#define ID_BFontUnderline    21/*20*/
#define ID_BFontUnderlineYes 22/*21*/
#define ID_BFontUnderlineNo  23/*22*/
#define ID_BHFlip    24/*20*/
#define ID_BHFlipYes 25/*21*/
#define ID_BHFlipNo  26/*22*/
#define ID_BVFlip    27/*20*/
#define ID_BVFlipYes 28/*21*/
#define ID_BVFlipNo  29/*22*/
#define ID_BOnTop    30/*20*/
#define ID_BOnTopYes 31/*21*/
#define ID_BOnTopNo  32/*22*/
#define ID_BTranslucency  33/*22*/


#define ID_CLayer           34/*13*/
#define ID_CColor           35/*14*/
#define ID_CWidth           36/*15*/
#define ID_CType            37/*16*/
#define ID_CFontName        38/*17*/
#define ID_CTextType        39/*17*/
#define ID_CTextJust        40/*17*/
#define ID_CTranslucency    41/*19*/
#define ID_ETextHeight      42/*18*/
#define ID_ETextWidth       43/*19*/

#define NO_OK		0	/*numery button'ow*/
#define NO_CANCEL      	1
#define NO_BLayer       2
#define NO_BColor       3
#define NO_BWidth       4
#define NO_BType        5
#define NO_BFontName    6
#define NO_BFontType    7
#define NO_BFontUkryty  8
#define NO_BFontUkrytyYes  9
#define NO_BFontUkrytyNo   10
#define NO_BFontJust       11
#define NO_BFontHeight     12
#define NO_BFontWidth      13
#define NO_BFontItalics    14
#define NO_BFontItalicsYes 15
#define NO_BFontItalicsNo  16
#define NO_BFontBold       17
#define NO_BFontBoldYes    18
#define NO_BFontBoldNo     19
#define NO_BFontUnderline       20
#define NO_BFontUnderlineYes    21
#define NO_BFontUnderlineNo     22
#define NO_BHFlip       23
#define NO_BHFlipYes    24
#define NO_BHFlipNo     25
#define NO_BVFlip       26
#define NO_BVFlipYes    27
#define NO_BVFlipNo     28
#define NO_BOnTop       29
#define NO_BOnTopYes    30
#define NO_BOnTopNo     31
#define NO_BTRanslucency     32

#define NO_CLayer       0	/*numery listbox'ow*/
#define NO_CColor       1
#define NO_CWidth       2
#define NO_CType        3
#define NO_CFontName    4
#define NO_CFontType    5
#define NO_CFontJust    6
#define NO_CTranslucency    7

#define NO_ETextHeight      0
#define NO_ETextWidth       1

#define LAYER_MAXW 10

static char *font_name_txt [MaxNumberTextStyle + 1] ;

static char *layer_name_txt [NoLayers + 1] ;	/*nalezy wstepnie zainicjowac*/

static LISTBOX listbox [COMBOBOX_SIZE] =
{
  {
    0, 0, 0, 0,  	/*wspolrzedne*/
    NoLayers, LAYER_MAXW, LAYER_MAXW, 0, 0, 0, 24,
    COLOR_NULL, COLOR_NULL, COLOR_NULL,
    1, NULL, layer_name_txt, ID_CLayer, 0
  },
  {
    0, 0, 0, 0,
    NoColors, 17 /*MAXWCOLOR*/, 17, 0, 0, 0, 24,  //18
    COLOR_NULL, COLOR_NULL, COLOR_NULL,
    1, NULL, Colors_Name_txt, ID_CColor, 0
  },
  {
    0, 0, 0, 0,
    NoWidthLines, 0, 0, 0, 0, 0, 24,
    COLOR_NULL, COLOR_NULL, COLOR_NULL,
    1, NULL, width_line_txt, ID_CWidth, 0
  },
  {
    0, 0, 0, 0,
    NoTypeLines, 6, 6, 0, 0, 0, 24,
    COLOR_NULL, COLOR_NULL, COLOR_NULL,
    1, NULL, type_line_txt, ID_CType, 0
  },
  {
    0, 0, 0, 0,
    MaxNumberTextStyle, 0, 0, 0, 0, 0, FONTNAMELEN,
    COLOR_NULL, COLOR_NULL, COLOR_NULL,
    1, NULL, font_name_txt, ID_CFontName, 0
  },
  {
    0, 0, 0, 0,
    NoTextType, 5, 5, 0, 0, 0, 16,
    COLOR_NULL, COLOR_NULL, COLOR_NULL,
    1, NULL, tab_type_txt, ID_CTextType, 0
  },
  {
    0, 0, 0, 0,
    NoTextJust, 0, 0, 0, 0, 0, 16,
    COLOR_NULL, COLOR_NULL, COLOR_NULL,
    1, NULL, font_just_txt,  ID_CTextJust, 0
  },
    {
    0, 0, 0, 0,
    NoTranslucency, 0, 0, 0, 0, 0, 16,
    COLOR_NULL, COLOR_NULL, COLOR_NULL,
    1, NULL, translucency_txt,  ID_CTranslucency, 0
  },
};


static COMBOBOX combobox [COMBOBOX_SIZE] =
{
 { XLayerName, YpGrLayer -1, DXLayerName, DYComboBox, &listbox [0]},
 { XColor, YpGrColor -1 , DXLayerName/*DXColor*/, DYComboBox, &listbox [1]},
 { XWidthLine, YpGrWidth -1 ,DXLayerName /*DXWidthLine*/, DYComboBox, &listbox [2]},
 { XTypeLine, YpGrType -1 , DXLayerName /*DXTypeLine*/, DYComboBox, &listbox [3]},
 { XFontName, YpGrFontName-3 , DXLayerName, DYComboBox+2, &listbox [4]},
 { XFontName, YpGrFontType-1 , DXLayerName, DYComboBox, &listbox [5]},
 { XFontName, YpGrFontJust-1 , DXLayerName, DYComboBox, &listbox [6]},
 { XpGr + DXGroup + 41, YpGrLayer - 1 , 52, DYComboBox, &listbox[7]},
} ;


static BUTTON buttons [BUTTON_SIZE] =
{
/*0*/  { XpOK, YpOK, DXBut0, DYBut0, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "OK", 0,B_PUSHBUTTON, 87, 1,0, ID_OK,
  },
/*1*/  { XpCANCEL, YpCANCEL, DXBut0, DYBut0, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "ESC", 0,B_PUSHBUTTON, 83, 1,0, ID_CANCEL,
  },
/*2*/  { XLayButton, YpGrLayer , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_CHECKBOX, 0, 1,0, ID_BLayer,
  } ,
/*3*/  { XColButton, YpGrColor , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_CHECKBOX, 0, 1,0, ID_BColor,
  },
/*4*/  { XWidthButton, YpGrWidth , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_CHECKBOX, 0, 1,0, ID_BWidth,
  },
/*5*/  { XTypeButton, YpGrType , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_CHECKBOX, 0, 1,0, ID_BType,
  },
/*6*/  { XFontButton, YpGrFontName -1, DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_CHECKBOX, 0, 1,0, ID_BFontName,
  },  
/*7*/  { XFontButton, YpGrFontName + 1*DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_CHECKBOX, 0, 1,0, ID_BFontType,    
  },
/*8*/  { XFontButton, YpGrFontName + 2*DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_CHECKBOX, 0, 1,0, ID_BFontUkryty,
  },
/*9*/  { XpName + XpYesButton, YpGrFontName + 2*DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_RADIOBUTTON, 0, 1,0, ID_BFontUkrytyYes,
  },
/*10*/  { XpName + XpNoButton, YpGrFontName + 2*DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_RADIOBUTTON, 0, 1,0, ID_BFontUkrytyNo,
  },  
/*11*/  { XFontButton, YpGrFontName + 3*DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_CHECKBOX, 0, 1,0, ID_BFontJust,    
  },  
/*12*/  { XFontButton, YpGrFontName + 4*DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_CHECKBOX, 0, 1,0, ID_BFontHeight,
  },
/*13*/  { XFontButton, YpGrFontName + 5*DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_CHECKBOX, 0, 1,0, ID_BFontWidth,
  },
/*14*/  { XFontButton, YpGrFontName + 6*DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_CHECKBOX, 0, 1,0, ID_BFontItalics,
  },
/*15*/  { XpName + XpYesButton, YpGrFontName + 6*DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_RADIOBUTTON, 0, 1,0, ID_BFontItalicsYes,
  },
/*16*/  { XpName + XpNoButton, YpGrFontName + 6*DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_RADIOBUTTON, 0, 1,0, ID_BFontItalicsNo,
  },
/*17*/  { XFontButton, YpGrFontName + 7*DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_CHECKBOX, 0, 1,0, ID_BFontBold,
  },
/*18*/  { XpName + XpYesButton, YpGrFontName + 7*DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_RADIOBUTTON, 0, 1,0, ID_BFontBoldYes,
  },
/*19*/  { XpName + XpNoButton, YpGrFontName + 7*DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_RADIOBUTTON, 0, 1,0, ID_BFontBoldNo,
  },
  /*20*/  { XFontButton, YpGrFontName + 8 * DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
	"", 0, B_CHECKBOX, 0, 1,0, ID_BFontUnderline,
  },
  /*21*/  { XpName + XpYesButton, YpGrFontName + 8 * DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
	  "", 0, B_RADIOBUTTON, 0, 1,0, ID_BFontUnderlineYes,
  },
  /*22*/  { XpName + XpNoButton, YpGrFontName + 8 * DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_RADIOBUTTON, 0, 1,0, ID_BFontUnderlineNo,
  },
    /*23*/  { XFontButton, YpGrFontName + 9 * DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
  "", 0, B_CHECKBOX, 0, 1,0, ID_BHFlip,
},
/*24*/  { XpName + XpYesButton, YpGrFontName + 9 * DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_RADIOBUTTON, 0, 1,0, ID_BHFlipYes,
},
/*25*/  { XpName + XpNoButton, YpGrFontName + 9 * DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
  "", 0, B_RADIOBUTTON, 0, 1,0, ID_BHFlipNo,
  },
  /*26*/  { XFontButton, YpGrFontName + 10 * DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
 "", 0, B_CHECKBOX, 0, 1,0, ID_BVFlip,
},
/*27*/  { XpName + XpYesButton, YpGrFontName + 10 * DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_RADIOBUTTON, 0, 1,0, ID_BVFlipYes,
},
/*28*/  { XpName + XpNoButton, YpGrFontName + 10 * DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
  "", 0, B_RADIOBUTTON, 0, 1,0, ID_BVFlipNo,
  },
  /*29*/  { XFontButton, YpGrFontName + 11 * DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
 "", 0, B_CHECKBOX, 0, 1,0, ID_BOnTop,
},
/*30*/  { XpName + XpYesButton, YpGrFontName + 11 * DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
    "", 0, B_RADIOBUTTON, 0, 1,0, ID_BOnTopYes,
},
/*31*/  { XpName + XpNoButton, YpGrFontName + 11 * DYName , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
  "", 0, B_RADIOBUTTON, 0, 1,0, ID_BOnTopNo,
},
  /*32*/  { 289, YpGrLayer , DXCheckBox, DYComboBox, COLOR_NULL, COLOR_NULL, COLOR_NULL,
  "", 0, B_CHECKBOX, 0, 1,0, ID_BTranslucency,
},
} ;


static GROUP_BOX gr_box [2] =
{
  { XpGr, YpGr, DXGroup, DYGroup,
	COLOR_NULL ,COLOR_NULL, chprop_comm0, 0, NULL,
  },
  { XpGr + DXGroup + 4, YpGr, 310 - DXGroup - 13, 26, //  75,
    COLOR_NULL ,COLOR_NULL, chprop_comm2, 0, NULL,
  },
};

static LABEL labels [19] = {
  { XpName, YpGrLayer, 0, 0, COLOR_NULL,COLOR_NULL,chprop_comm1,0,0},
};

static char sz__height_text [10] = "";
static char sz__width_text [11] = "";

static INPUTLINE s__edit [] =
{
   { XpComboBox, YpGrFontName + 4*DYName, DXLayerName, DYComboBox, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 10,0,1, sz__height_text, -1,&digits,
   },
   { XpComboBox, YpGrFontName + 5*DYName, DXLayerName, DYComboBox, COLOR_NULL,COLOR_NULL,
	COLOR_NULL,COLOR_NULL,COLOR_NULL, 10,0,1, sz__width_text, -1,&digits,
   },
} ;

static DARK_LIGHT_LINE line_d_l [] =
{
   { XpGr + 1, YpGrText+1, DXGroup - 1, 1, LINE_NULL, COLOR_NULL},
   { XpGr + 1,  YpGrFontName + 9 * DYName - 2, DXGroup - 1, 1, LINE_NULL, COLOR_NULL},
} ;

#define y1 1
#define y2 2
static IMAGE images_ch[] =
{
 {  XpName, YpGrLayer-y2, 32, 32, 45,chprop_tips0},
 {  XpName, YpGrColor-y2, 32, 32, 5,chprop_tips1},
 {  XpName, YpGrWidth-y2, 32, 32, 6,chprop_tips2},
 {  XpName, YpGrType-y2, 32, 32, 7,chprop_tips3},

 {  XpName, YpGrFontName -1 -y2, 32, 32, 42,chprop_tips4},
 {  XpName, YpGrFontName + 1*DYName -y2, 32, 32, 48,chprop_tips5},
 {  XpName, YpGrFontName + 2*DYName -y2, 32, 32, 43,chprop_tips6},
 {  XpName, YpGrFontName + 3*DYName -y2, 32, 32, 46,chprop_tips7},
 {  XpName, YpGrFontName + 4*DYName -y2, 32, 32, 47,chprop_tips8},
 {  XpName, YpGrFontName + 5*DYName -y2, 32, 32, 49,chprop_tips9},
 {  XpName, YpGrFontName + 6*DYName -y2, 32, 32, 44,chprop_tips10},
 {  XpName, YpGrFontName + 7*DYName -y2, 32, 32, 41,chprop_tips11},
 {  XpName, YpGrFontName + 8*DYName -y2, 32, 32, 50,chprop_tips12},
 {  XpName, YpGrFontName + 9 * DYName -y1, 32, 32, 123,chprop_tips15},
 {  XpName, YpGrFontName + 10 * DYName -y1, 32, 32, 124,chprop_tips16},
 {  XpName, YpGrFontName + 11 * DYName -y1, 32, 32, 122,chprop_tips17},

 {  XpName+ XpYesButton+20, YpGrFontName + 2*DYName -y2, 32, 32, 51,chprop_tips13},
 {  XpName+ XpNoButton+20, YpGrFontName + 2*DYName -y2, 32, 32, 52,chprop_tips14},

 {  XpName+ XpYesButton+20, YpGrFontName + 6*DYName -y2, 32, 32, 51,chprop_tips13},
 {  XpName+ XpNoButton + 20, YpGrFontName + 6*DYName -y2, 32, 32, 52,chprop_tips14},

 {  XpName+ XpYesButton+20, YpGrFontName + 7*DYName -y2, 32, 32, 51,chprop_tips13},
 {  XpName+ XpNoButton + 20, YpGrFontName + 7*DYName -y2, 32, 32, 52,chprop_tips14},

 {  XpName+ XpYesButton+20, YpGrFontName + 8*DYName -y2, 32, 32, 51,chprop_tips13},
 {  XpName+ XpNoButton + 20, YpGrFontName + 8*DYName -y2, 32, 32, 52,chprop_tips14},

 {  XpName + XpYesButton + 20, YpGrFontName + 9 * DYName -y2, 32, 32, 51,chprop_tips13},
 {  XpName + XpNoButton + 20, YpGrFontName + 9 * DYName -y2, 32, 32, 52,chprop_tips14},

 {  XpName + XpYesButton + 20, YpGrFontName + 10 * DYName -y2, 32, 32, 51,chprop_tips13},
 {  XpName + XpNoButton + 20, YpGrFontName + 10 * DYName -y2, 32, 32, 52,chprop_tips14},

 {  XpName + XpYesButton + 20, YpGrFontName + 11 * DYName -y2, 32, 32, 51,chprop_tips13},
 {  XpName + XpNoButton + 20, YpGrFontName + 11 * DYName -y2, 32, 32, 52,chprop_tips14},

 {  XpGr + DXGroup + 4 + 10, YpGrLayer -y2, 32, 32, 126,chprop_tips18},
};

static TDIALOG change_dlg=
{
	100, 5 /*10*/, 310 /*338*/, DYGroup + 2 * YpGr + 2,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0,0,0, chprop_comm1,
	2, (DARK_LIGHT_LINE(*)[])&line_d_l,		/*line*/
	0, NULL,		/*&labels*/
	2, (GROUP_BOX(*)[])&gr_box,
	2, (INPUTLINE(*)[])&s__edit, 		/*edit*/
	31, (IMAGE(*)[])&images_ch,		/*image*/
	BUTTON_SIZE, (BUTTON(*)[])&buttons,
	0, NULL,		/*listbox*/
	COMBOBOX_SIZE, (COMBOBOX(*)[])&combobox,
    0,NULL, //Sliders
	NULL,
	NULL,
	0,
	0,
    NULL,
};


static void set_struct_dialog_control (void)
/*---------------------------------------*/
{
    int i, i_pos;
    double wsp_y;

    wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15.0

    /*warstwy*/
  for (i = 0; i < 6; i++)
  {
	  layer_name_txt[i] = &Layers[i].name;
  }

  for (i = 6; i < No_Layers ; i++)
  {
    layer_name_txt [i] = &Layers [i].name;
  }
  
  layer_name_txt [NoLayers] = NoChange ;
  listbox [NO_CLayer].max =  No_Layers ;
  Set_Check_Button ( &change_dlg, ID_BLayer, s__change_param.b_layer) ;
  listbox [NO_CLayer].enable = s__change_param.b_layer ;
  listbox [NO_CLayer].maxw = 0 ;
  if (No_Layers > LAYER_MAXW)
  {
    listbox [NO_CLayer].maxw = LAYER_MAXW ;
  }
  i_pos = listbox [NO_CLayer].enable ?
    s__change_param.layer : NoLayers ;
  Get_Menu_Param (i_pos, listbox [NO_CLayer].maxw, listbox [NO_CLayer].max,
	    &listbox [NO_CLayer].foff, &listbox [NO_CLayer].poz) ;
/*czcionki*/
  for (i = 0 ; i < WhNumberTextStyle + 1 ; i++)
  {
    font_name_txt [i] = pmCzcionka [i].txt ;
  }
  font_name_txt [WhNumberTextStyle+1] = NoChange ;
  listbox [NO_CFontName].max =  WhNumberTextStyle + 1;
  listbox [NO_CFontName].maxw =  0 ;
  if (WhNumberTextStyle > 7)
  {
    //listbox [NO_CFontName].maxw = 12 ;
    listbox [NO_CFontName].maxw = min(32, (getmaxy()/wsp_y - 2 * ED_INF_HEIGHT - change_dlg.y - combobox [4].y)/combobox [4].dy);
  }
  Set_Check_Button ( &change_dlg, ID_BFontName,  s__change_param.b_text_font) ;
  listbox [NO_CFontName].enable = s__change_param.b_text_font ;
  i_pos = listbox [NO_CFontName].enable ? s__change_param.i_text_font : (WhNumberTextStyle+1) ;
  Get_Menu_Param (i_pos, listbox [NO_CFontName].maxw, listbox [NO_CFontName].max,
	    &listbox [NO_CFontName].foff, &listbox [NO_CFontName].poz) ;

/*typ tekstu*/
  Set_Check_Button ( &change_dlg, ID_BFontType,  s__change_param.b_text_type) ;
  listbox [NO_CFontType].enable = s__change_param.b_text_type ;
  i_pos = listbox [NO_CFontType].enable ? s__change_param.i_text_type : NoTextType ;
  Get_Menu_Param (i_pos, listbox [NO_CFontType].maxw, listbox [NO_CFontType].max,
	    &listbox [NO_CFontType].foff, &listbox [NO_CFontType].poz) ;
	    
	    
/*ukryty*/  
  
  Set_Check_Button ( &change_dlg, ID_BFontUkryty,  s__change_param.b_text_ukryty) ;
  if (TRUE == s__change_param.b_text_ukryty)
  {
    Check_Radio_Button ( &change_dlg, ID_BFontUkrytyYes, ID_BFontUkrytyNo,
	  s__change_param.i_text_ukryty == TRUE ?
	  ID_BFontUkrytyYes : ID_BFontUkrytyNo, FALSE) ;
  }
  else
  {
    Set_Check_Button ( &change_dlg, ID_BFontUkrytyYes,  FALSE) ;
    Set_Check_Button ( &change_dlg, ID_BFontUkrytyNo,  FALSE) ;
  }
  buttons [NO_BFontUkrytyYes].enable = s__change_param.b_text_ukryty ;
  buttons [NO_BFontUkrytyNo].enable = s__change_param.b_text_ukryty ;  	    
	    
/*justowanie tekstu*/
  Set_Check_Button ( &change_dlg, ID_BFontJust,  s__change_param.b_text_just) ;
  listbox [NO_CFontJust].enable = s__change_param.b_text_just ;
  i_pos = listbox [NO_CFontJust].enable ? s__change_param.i_text_just : NoTextJust ;
  Get_Menu_Param (i_pos, listbox [NO_CFontJust].maxw, listbox [NO_CFontJust].max,
	    &listbox [NO_CFontJust].foff, &listbox [NO_CFontJust].poz) ;
		    
	    	    
/*kolory*/
  Colors_Name_txt [MAXCOLOR] = NoChange ;
  Set_Check_Button ( &change_dlg, ID_BColor, s__change_param.b_color) ;
  listbox [NO_CColor].enable = s__change_param.b_color ;
  i_pos = listbox [NO_CColor].enable ?  s__change_param.color : NoColors ;
  if (listbox [NO_CColor].enable)
  {
  if (i_pos>=16) 
   {
    listbox[NO_CColor].wartosc=i_pos + 1;
    i_pos=16;
   }
    else listbox[NO_CColor].wartosc=i_pos + 1;
  }
   else listbox[NO_CColor].wartosc=Layers[Current_Layer].color;
    
  Get_Menu_Param (i_pos, listbox [NO_CColor].maxw, listbox [NO_CColor].max,
	    &listbox [NO_CColor].foff, &listbox [NO_CColor].poz) ;

/*grubosc linii*/
  Set_Check_Button ( &change_dlg, ID_BWidth, s__change_param.b_width) ;
  listbox [NO_CWidth].enable = s__change_param.b_width ;
  listbox [NO_CWidth].poz = listbox [NO_CWidth].enable ?
  s__change_param.width : NoWidthLines;

/*typ linii*/
  Set_Check_Button ( &change_dlg, ID_BType,  s__change_param.b_type) ;
  listbox [NO_CType].enable = s__change_param.b_type ;
  i_pos = listbox [NO_CType].enable ?  s__change_param.type : NoTypeLines ;
  Get_Menu_Param (i_pos, listbox [NO_CType].maxw, listbox [NO_CType].max,
	    &listbox [NO_CType].foff, &listbox [NO_CType].poz) ;

  /*translucency*/
  Set_Check_Button(&change_dlg, ID_BTranslucency, s__change_param.b_translucency);
  listbox[NO_CTranslucency].enable = s__change_param.b_translucency;
    listbox[NO_CTranslucency].poz = listbox[NO_CTranslucency].enable ?
    s__change_param.i_translucency : 20 ; 
  i_pos = listbox[NO_CTranslucency].enable ? s__change_param.i_translucency : 20;
  Get_Menu_Param(i_pos, listbox[NO_CTranslucency].maxw, listbox[NO_CTranslucency].max,
      &listbox[NO_CTranslucency].foff, &listbox[NO_CTranslucency].poz);


/*wysokosc tekstu*/
  Set_Check_Button ( &change_dlg, ID_BFontHeight,  s__change_param.b_text_height) ;
  s__edit [NO_ETextHeight].enable = s__change_param.b_text_height ;
  s__edit [NO_ETextHeight].txt = sz__height_text ;
  if (FALSE == s__edit [NO_ETextHeight].enable)
  {
    s__edit [NO_ETextHeight].txt = NoChange ;
  }

/*szerokosc tekstu*/
  Set_Check_Button ( &change_dlg, ID_BFontWidth,  s__change_param.b_text_width) ;
  s__edit [NO_ETextWidth].enable = s__change_param.b_text_width ;
  s__edit [NO_ETextWidth].txt = sz__width_text ;
  if (FALSE == s__edit [NO_ETextWidth].enable)
  {
    s__edit [NO_ETextWidth].txt = NoChange ;
  }

/*italics*/
  Set_Check_Button ( &change_dlg, ID_BFontItalics,  s__change_param.b_text_italics) ;
  if (TRUE == s__change_param.b_text_italics)
  {
    Check_Radio_Button ( &change_dlg, ID_BFontItalicsYes, ID_BFontItalicsNo,
	  s__change_param.i_text_italics == TRUE ?
	  ID_BFontItalicsYes : ID_BFontItalicsNo, FALSE) ;
  }
  else
  {
    Set_Check_Button ( &change_dlg, ID_BFontItalicsYes,  FALSE) ;
    Set_Check_Button ( &change_dlg, ID_BFontItalicsNo,  FALSE) ;
  }
  buttons [NO_BFontItalicsYes].enable = s__change_param.b_text_italics ;
  buttons [NO_BFontItalicsNo].enable = s__change_param.b_text_italics ;
  
  /*bold*/
  Set_Check_Button ( &change_dlg, ID_BFontBold,  s__change_param.b_text_bold) ;
  if (TRUE == s__change_param.b_text_bold)
  {
    Check_Radio_Button ( &change_dlg, ID_BFontBoldYes, ID_BFontBoldNo,
	  s__change_param.i_text_bold == TRUE ?
	  ID_BFontBoldYes : ID_BFontBoldNo, FALSE) ;
  }
  else
  {
    Set_Check_Button ( &change_dlg, ID_BFontBoldYes,  FALSE) ;
    Set_Check_Button ( &change_dlg, ID_BFontBoldNo,  FALSE) ;
  }
  buttons [NO_BFontBoldYes].enable = s__change_param.b_text_bold ;
  buttons [NO_BFontBoldNo].enable = s__change_param.b_text_bold ;

  /*underline*/
  Set_Check_Button(&change_dlg, ID_BFontUnderline, s__change_param.b_text_underline);
  if (TRUE == s__change_param.b_text_underline)
  {
	  Check_Radio_Button(&change_dlg, ID_BFontUnderlineYes, ID_BFontUnderlineNo,
		  s__change_param.i_text_underline == TRUE ?
		  ID_BFontUnderlineYes : ID_BFontUnderlineNo, FALSE);
  }
  else
  {
	  Set_Check_Button(&change_dlg, ID_BFontUnderlineYes, FALSE);
	  Set_Check_Button(&change_dlg, ID_BFontUnderlineNo, FALSE);
  }
  buttons[NO_BFontUnderlineYes].enable = s__change_param.b_text_underline;
  buttons[NO_BFontUnderlineNo].enable = s__change_param.b_text_underline;

  /*hflip*/
  Set_Check_Button(&change_dlg, ID_BHFlip, s__change_param.b_hflip);
  if (TRUE == s__change_param.b_hflip)
  {
      Check_Radio_Button(&change_dlg, ID_BHFlipYes, ID_BHFlipNo,
          s__change_param.i_hflip == TRUE ?
          ID_BHFlipYes : ID_BHFlipNo, FALSE);
  }
  else
  {
      Set_Check_Button(&change_dlg, ID_BHFlipYes, FALSE);
      Set_Check_Button(&change_dlg, ID_BHFlipNo, FALSE);
  }
  buttons[NO_BHFlipYes].enable = s__change_param.b_hflip;
  buttons[NO_BHFlipNo].enable = s__change_param.b_hflip;

  /*vflip*/
  Set_Check_Button(&change_dlg, ID_BVFlip, s__change_param.b_vflip);
  if (TRUE == s__change_param.b_vflip)
  {
      Check_Radio_Button(&change_dlg, ID_BVFlipYes, ID_BVFlipNo,
          s__change_param.i_vflip == TRUE ?
          ID_BVFlipYes : ID_BVFlipNo, FALSE);
  }
  else
  {
      Set_Check_Button(&change_dlg, ID_BVFlipYes, FALSE);
      Set_Check_Button(&change_dlg, ID_BVFlipNo, FALSE);
  }
  buttons[NO_BVFlipYes].enable = s__change_param.b_vflip;
  buttons[NO_BVFlipNo].enable = s__change_param.b_vflip;

  /*ontop*/
  Set_Check_Button(&change_dlg, ID_BOnTop, s__change_param.b_ontop);
  if (TRUE == s__change_param.b_ontop)
  {
      Check_Radio_Button(&change_dlg, ID_BOnTopYes, ID_BOnTopNo,
          s__change_param.i_ontop == TRUE ?
          ID_BOnTopYes : ID_BOnTopNo, FALSE);
  }
  else
  {
      Set_Check_Button(&change_dlg, ID_BOnTopYes, FALSE);
      Set_Check_Button(&change_dlg, ID_BOnTopNo, FALSE);
  }
  buttons[NO_BOnTopYes].enable = s__change_param.b_ontop;
  buttons[NO_BOnTopNo].enable = s__change_param.b_ontop;
  
}

static BOOL get_dlg_controls (void)
/*---------------------------------*/
{
  char * endptr ;
  double df_text_width, df_text_height ;

  if (TRUE == (s__change_param.b_layer = Get_Check_Button (&change_dlg, ID_BLayer)))
  {
     s__change_param.layer = listbox [NO_CLayer].poz + listbox [NO_CLayer].foff ;
  }
  if (TRUE == (s__change_param.b_color = Get_Check_Button (&change_dlg, ID_BColor)))
  {
    s__change_param.color = listbox [NO_CColor].foff + listbox [NO_CColor].poz ;
    if ((s__change_param.color >=16) && (listbox [NO_CColor].enable))
     {
      s__change_param.color = listbox [NO_CColor].wartosc - 1; 
     }
  }
  if (TRUE == (s__change_param.b_width= Get_Check_Button (&change_dlg, ID_BWidth)))
  {
     s__change_param.width= listbox [NO_CWidth].poz ;
  }
  if (TRUE == (s__change_param.b_type = Get_Check_Button (&change_dlg, ID_BType)))
  {
    s__change_param.type= listbox [NO_CType].foff + listbox [NO_CType].poz ;
  }

  if (TRUE == (s__change_param.b_translucency = Get_Check_Button(&change_dlg, ID_BTranslucency)))
  {
      s__change_param.i_translucency = listbox[NO_CTranslucency].foff + listbox[NO_CTranslucency].poz;
  }

  if (TRUE == (s__change_param.b_text_font = Get_Check_Button (&change_dlg, ID_BFontName)))
  {
    s__change_param.i_text_font = listbox [NO_CFontName].poz + listbox [NO_CFontName].foff ;
  }
  
  if (TRUE == (s__change_param.b_text_type = Get_Check_Button (&change_dlg, ID_BFontType)))
  {
    s__change_param.i_text_type = listbox [NO_CFontType].poz + listbox [NO_CFontType].foff ;
  }
  
   if (TRUE == (s__change_param.b_text_ukryty = Get_Check_Button (&change_dlg, ID_BFontUkryty)))
  {
    s__change_param.i_text_ukryty = Get_Check_Button (&change_dlg, ID_BFontUkrytyYes) ;
  }
  
  if (TRUE == (s__change_param.b_text_just = Get_Check_Button (&change_dlg, ID_BFontJust)))
  {
    s__change_param.i_text_just = listbox [NO_CFontJust].poz + listbox [NO_CFontJust].foff ;
  }
  
  if (TRUE == (s__change_param.b_text_height = Get_Check_Button (&change_dlg, ID_BFontHeight)))
  {
    df_text_height = strtod (sz__height_text, &endptr) ;
    if (df_text_height <=0 )
    {
      ErrList (30);
      return FALSE ;
    }
    s__change_param.df_text_height = df_text_height ;
  }
  if (TRUE == (s__change_param.b_text_width= Get_Check_Button (&change_dlg, ID_BFontWidth)))
  {
    df_text_width = strtod (sz__width_text, &endptr) ;
    if (df_text_width <=0)
    {
      ErrList (30);
      return FALSE ;
    }
    s__change_param.df_text_width= df_text_width ;
  }
  
  if (TRUE == (s__change_param.b_text_italics = Get_Check_Button (&change_dlg, ID_BFontItalics)))
  {
    s__change_param.i_text_italics = Get_Check_Button (&change_dlg, ID_BFontItalicsYes) ;
  }
  
  if (TRUE == (s__change_param.b_text_bold = Get_Check_Button (&change_dlg, ID_BFontBold)))
  {
    s__change_param.i_text_bold = Get_Check_Button (&change_dlg, ID_BFontBoldYes) ;
  }

  if (TRUE == (s__change_param.b_text_underline = Get_Check_Button(&change_dlg, ID_BFontUnderline)))
  {
	  s__change_param.i_text_underline = Get_Check_Button(&change_dlg, ID_BFontUnderlineYes);
  }

  if (TRUE == (s__change_param.b_hflip = Get_Check_Button(&change_dlg, ID_BHFlip)))
  {
      s__change_param.i_hflip = Get_Check_Button(&change_dlg, ID_BHFlipYes);
  }

  if (TRUE == (s__change_param.b_vflip = Get_Check_Button(&change_dlg, ID_BVFlip)))
  {
      s__change_param.i_vflip = Get_Check_Button(&change_dlg, ID_BVFlipYes);
  }
  
  if (TRUE == (s__change_param.b_ontop = Get_Check_Button(&change_dlg, ID_BOnTop)))
  {
      s__change_param.i_ontop = Get_Check_Button(&change_dlg, ID_BOnTopYes);
  }
  
  return TRUE ;
}

static int proc_dlg (int n)
/*-------------------------*/
{
  int i_pos ;
  int ret = Dlg_Ret_Val_Continue;
  int font_no, ttf_otf;
  int Xm, Ym;
  int numbertextstyle;

  switch (n)
  {
    case ID_OK:
      if (get_dlg_controls () == TRUE)
      {
	ret = Dlg_Ret_Val_OK;
      }
      break;
    case ID_CANCEL:
      ret = Dlg_Ret_Val_Cancel;
      break;
    case ID_BLayer :
      s__change_param.b_layer = Get_Check_Button (&change_dlg, ID_BLayer) ;
      listbox [NO_CLayer].enable = s__change_param.b_layer ;
      if (FALSE == s__change_param.b_layer)
      {
	s__change_param.layer= listbox [NO_CLayer].poz + listbox [NO_CLayer].foff ;
      }
      i_pos = s__change_param.b_layer ? s__change_param.layer : NoLayers ;
      Get_Menu_Param (i_pos, listbox [NO_CLayer].maxw, listbox [NO_CLayer].max,
		&listbox [NO_CLayer].foff, &listbox [NO_CLayer].poz) ;
      Draw_ComboBox (&combobox [NO_CLayer]);
      break ;
    case ID_BColor :
      s__change_param.b_color = Get_Check_Button (&change_dlg, ID_BColor) ;
      listbox [NO_CColor].enable = s__change_param.b_color;
      if (TRUE == s__change_param.b_color)
      {
	Get_Menu_Param (s__change_param.color,
			listbox [NO_CColor].maxw,
			listbox [NO_CColor].max,
			&listbox [NO_CColor].foff,
			&listbox [NO_CColor].poz) ;
      }
      else
      {
	s__change_param.color = listbox [NO_CColor].foff + listbox [NO_CColor].poz ;
	Get_Menu_Param (NoColors,
			listbox [NO_CColor].maxw,
			listbox [NO_CColor].max,
			&listbox [NO_CColor].foff,
			&listbox [NO_CColor].poz) ;
      }
      Draw_ComboBox (&combobox [NO_CColor]);
      break ;
    case ID_BWidth :
      s__change_param.b_width= Get_Check_Button (&change_dlg, ID_BWidth) ;
      listbox [NO_CWidth].enable = s__change_param.b_width;
      if (TRUE == s__change_param.b_width)
      {
	listbox [NO_CWidth].poz = s__change_param.width;
      }
      else
      {
	s__change_param.width= listbox [NO_CWidth].poz ;
	listbox [NO_CWidth].poz = NoWidthLines ;

      }
      Draw_ComboBox (&combobox [NO_CWidth]);
      break ;
    case ID_BType  :
      s__change_param.b_type = Get_Check_Button (&change_dlg, ID_BType) ;
      listbox [NO_CType].enable = s__change_param.b_type;
      if (TRUE == s__change_param.b_type)
      {
	Get_Menu_Param (s__change_param.type,
			listbox [NO_CType].maxw,
			listbox [NO_CType].max,
			&listbox [NO_CType].foff,
			&listbox [NO_CType].poz) ;
      }
      else
      {
          s__change_param.type = listbox[NO_CType].foff + listbox[NO_CType].poz;
          Get_Menu_Param(NoTypeLines,
              listbox[NO_CType].maxw,
              listbox[NO_CType].max,
              &listbox[NO_CType].foff,
              &listbox[NO_CType].poz);
      }
      Draw_ComboBox(&combobox[NO_CType]);
      break;
    case ID_BTranslucency:
        s__change_param.b_translucency = Get_Check_Button(&change_dlg, ID_BTranslucency);
        listbox[NO_CTranslucency].enable = s__change_param.b_translucency;
        if (TRUE == s__change_param.b_translucency)
        {
            Get_Menu_Param(s__change_param.i_translucency,
                listbox[NO_CTranslucency].maxw,
                listbox[NO_CTranslucency].max,
                &listbox[NO_CTranslucency].foff,
                &listbox[NO_CTranslucency].poz);
        }
      else
      {
	s__change_param.i_translucency = listbox [NO_CTranslucency].foff + listbox [NO_CTranslucency].poz ;
	Get_Menu_Param (NoTranslucency,
			listbox [NO_CTranslucency].maxw,
			listbox [NO_CTranslucency].max,
			&listbox [NO_CTranslucency].foff,
			&listbox [NO_CTranslucency].poz) ;
      }
      Draw_ComboBox (&combobox [NO_CTranslucency]);
      break ;

    case ID_BFontName :
      s__change_param.b_text_font = Get_Check_Button (&change_dlg, ID_BFontName) ;
      listbox [NO_CFontName].enable = s__change_param.b_text_font ;
      if (FALSE == s__change_param.b_text_font)
      {
	s__change_param.i_text_font= listbox [NO_CFontName].poz + listbox [NO_CFontName].foff ;
      }
      i_pos = s__change_param.b_text_font? s__change_param.i_text_font : WhNumberTextStyle + 1;
      Get_Menu_Param (i_pos, listbox [NO_CFontName].maxw, listbox [NO_CFontName].max,
		&listbox [NO_CFontName].foff, &listbox [NO_CFontName].poz) ;
      Draw_ComboBox (&combobox [NO_CFontName]) ;
      break ;
      
   case ID_BFontType  :
      s__change_param.b_text_type = Get_Check_Button (&change_dlg, ID_BFontType) ;
      listbox [NO_CFontType].enable = s__change_param.b_text_type;
      if (TRUE == s__change_param.b_text_type)
      {
	Get_Menu_Param (s__change_param.i_text_type,
			listbox [NO_CFontType].maxw,
			listbox [NO_CFontType].max,
			&listbox [NO_CFontType].foff,
			&listbox [NO_CFontType].poz) ;
      }
      else
      {
	s__change_param.i_text_type = listbox [NO_CFontType].foff + listbox [NO_CFontType].poz ;
	Get_Menu_Param (NoTextType,
			listbox [NO_CFontType].maxw,
			listbox [NO_CFontType].max,
			&listbox [NO_CFontType].foff,
			&listbox [NO_CFontType].poz) ;
      }
      Draw_ComboBox (&combobox [NO_CFontType]);
      break ;      
      
   case ID_BFontUkrytyYes :
   case ID_BFontUkrytyNo :
      if (TRUE == s__change_param.b_text_ukryty)
      {
	s__change_param.i_text_ukryty = (n == ID_BFontUkrytyYes ? 1 : 0) ;
	Check_Radio_Button (&change_dlg,
			    ID_BFontUkrytyYes,
			    ID_BFontUkrytyNo, n, TRUE) ;
      }
      break ;
      
   case ID_BFontUkryty :
      s__change_param.b_text_ukryty = Get_Check_Button (&change_dlg, ID_BFontUkryty) ;
      buttons [NO_BFontUkrytyYes].enable = s__change_param.b_text_ukryty ;
      buttons [NO_BFontUkrytyNo].enable = s__change_param.b_text_ukryty ;
      if (TRUE == s__change_param.b_text_ukryty)
      {
	Check_Radio_Button (&change_dlg,
			    ID_BFontUkrytyYes,
			    ID_BFontUkrytyNo,
			    s__change_param.i_text_ukryty == 1 ?
			    ID_BFontUkrytyYes: ID_BFontUkrytyNo, FALSE) ;
      }
      else
      {
	s__change_param.i_text_ukryty =
       (Get_Check_Button (&change_dlg, ID_BFontUkrytyYes) == TRUE ? 1: 0) ;
	Set_Check_Button (&change_dlg, ID_BFontUkrytyYes, FALSE) ;
	Set_Check_Button (&change_dlg, ID_BFontUkrytyNo, FALSE) ;
      }
      Draw_Button (&buttons [NO_BFontUkrytyYes]);
      Draw_Button (&buttons [NO_BFontUkrytyNo]);
      break ;    
   case ID_BFontJust :
      s__change_param.b_text_just = Get_Check_Button (&change_dlg, ID_BFontJust) ;
      listbox [NO_CFontJust].enable = s__change_param.b_text_just ;
      if (TRUE == s__change_param.b_text_just)
      {
        listbox [NO_CFontJust].poz = s__change_param.i_text_just;	
      }
        else
          {
	   s__change_param.i_text_just= listbox [NO_CFontJust].poz ;
	   listbox [NO_CFontJust].poz = NoTextJust ;
          }
      Draw_ComboBox (&combobox [NO_CFontJust]) ;
      break ; 
      
   case ID_BFontHeight:
      s__change_param.b_text_height = Get_Check_Button (&change_dlg, ID_BFontHeight) ;
      s__edit [NO_ETextHeight].enable = s__change_param.b_text_height ;
      if (TRUE == s__change_param.b_text_height)
      {
	s__edit [NO_ETextHeight].txt = sz__height_text ;
      }
      else
      {
	s__edit [NO_ETextHeight].txt = NoChange ;
      }
      Draw_Input_Line (&s__edit [NO_ETextHeight]) ;
      break ;
   case ID_BFontWidth:
      s__change_param.b_text_width= Get_Check_Button (&change_dlg, ID_BFontWidth) ;
      s__edit [NO_ETextWidth].enable = s__change_param.b_text_width ;
      if (TRUE == s__change_param.b_text_width)
      {
	s__edit [NO_ETextWidth].txt = sz__width_text ;
      }
      else
      {
	s__edit [NO_ETextWidth].txt = NoChange ;
      }
      Draw_Input_Line (&s__edit [NO_ETextWidth]) ;
      break ;
      
   case ID_BFontItalicsYes :
   case ID_BFontItalicsNo :
      if (TRUE == s__change_param.b_text_italics)
      {
	s__change_param.i_text_italics = (n == ID_BFontItalicsYes ? 1 : 0) ;
	Check_Radio_Button (&change_dlg,
			    ID_BFontItalicsYes,
			    ID_BFontItalicsNo, n, TRUE) ;
      }
      break ;
   case ID_BFontItalics :
      s__change_param.b_text_italics = Get_Check_Button (&change_dlg, ID_BFontItalics) ;
      buttons [NO_BFontItalicsYes].enable = s__change_param.b_text_italics ;
      buttons [NO_BFontItalicsNo].enable = s__change_param.b_text_italics ;
      if (TRUE == s__change_param.b_text_italics)
      {
	Check_Radio_Button (&change_dlg,
			    ID_BFontItalicsYes,
			    ID_BFontItalicsNo,
			    s__change_param.i_text_italics == 1 ?
			    ID_BFontItalicsYes: ID_BFontItalicsNo, FALSE) ;
      }
      else
      {
	s__change_param.i_text_italics =
       (Get_Check_Button (&change_dlg, ID_BFontItalicsYes) == TRUE ? 1: 0) ;
	Set_Check_Button (&change_dlg, ID_BFontItalicsYes, FALSE) ;
	Set_Check_Button (&change_dlg, ID_BFontItalicsNo, FALSE) ;
      }
      Draw_Button (&buttons [NO_BFontItalicsYes]);
      Draw_Button (&buttons [NO_BFontItalicsNo]);
      break ;
      
   case ID_BFontBoldYes :
   case ID_BFontBoldNo :
      if (TRUE == s__change_param.b_text_bold)
      {
	s__change_param.i_text_bold = (n == ID_BFontBoldYes ? 1 : 0) ;
	Check_Radio_Button (&change_dlg,
			    ID_BFontBoldYes,
			    ID_BFontBoldNo, n, TRUE) ;
      }
      break ;
   case ID_BFontBold :
      s__change_param.b_text_bold = Get_Check_Button (&change_dlg, ID_BFontBold) ;
      buttons [NO_BFontBoldYes].enable = s__change_param.b_text_bold ;
      buttons [NO_BFontBoldNo].enable = s__change_param.b_text_bold ;
      if (TRUE == s__change_param.b_text_bold)
      {
	Check_Radio_Button (&change_dlg,
			    ID_BFontBoldYes,
			    ID_BFontBoldNo,
			    s__change_param.i_text_bold == 1 ?
			    ID_BFontBoldYes: ID_BFontBoldNo, FALSE) ;
      }
      else
      {
	s__change_param.i_text_bold =
       (Get_Check_Button (&change_dlg, ID_BFontBoldYes) == TRUE ? 1: 0) ;
	Set_Check_Button (&change_dlg, ID_BFontBoldYes, FALSE) ;
	Set_Check_Button (&change_dlg, ID_BFontBoldNo, FALSE) ;
      }
      Draw_Button (&buttons [NO_BFontBoldYes]);
      Draw_Button (&buttons [NO_BFontBoldNo]);
      break ;  

   case ID_BFontUnderlineYes:
   case ID_BFontUnderlineNo:
	   if (TRUE == s__change_param.b_text_underline)
	   {
		   s__change_param.i_text_underline = (n == ID_BFontUnderlineYes ? 1 : 0);
		   Check_Radio_Button(&change_dlg,
			   ID_BFontUnderlineYes,
			   ID_BFontUnderlineNo, n, TRUE);
	   }
	   break;
   case ID_BFontUnderline:
	   s__change_param.b_text_underline = Get_Check_Button(&change_dlg, ID_BFontUnderline);
	   buttons[NO_BFontUnderlineYes].enable = s__change_param.b_text_underline;
	   buttons[NO_BFontUnderlineNo].enable = s__change_param.b_text_underline;
	   if (TRUE == s__change_param.b_text_underline)
	   {
		   Check_Radio_Button(&change_dlg,
			   ID_BFontUnderlineYes,
			   ID_BFontUnderlineNo,
			   s__change_param.i_text_underline == 1 ?
			   ID_BFontUnderlineYes : ID_BFontUnderlineNo, FALSE);
	   }
	   else
	   {
		   s__change_param.i_text_underline =
			   (Get_Check_Button(&change_dlg, ID_BFontUnderlineYes) == TRUE ? 1 : 0);
		   Set_Check_Button(&change_dlg, ID_BFontUnderlineYes, FALSE);
		   Set_Check_Button(&change_dlg, ID_BFontUnderlineNo, FALSE);
	   }
	   Draw_Button(&buttons[NO_BFontUnderlineYes]);
	   Draw_Button(&buttons[NO_BFontUnderlineNo]);
	   break;

   case ID_BHFlipYes:
   case ID_BHFlipNo:
       if (TRUE == s__change_param.b_hflip)
       {
           s__change_param.i_hflip = (n == ID_BHFlipYes ? 1 : 0);
           Check_Radio_Button(&change_dlg,
               ID_BHFlipYes,
               ID_BHFlipNo, n, TRUE);
       }
       break;
   case ID_BHFlip:
       s__change_param.b_hflip = Get_Check_Button(&change_dlg, ID_BHFlip);
       buttons[NO_BHFlipYes].enable = s__change_param.b_hflip;
       buttons[NO_BHFlipNo].enable = s__change_param.b_hflip;
       if (TRUE == s__change_param.b_hflip)
       {
           Check_Radio_Button(&change_dlg,
               ID_BHFlipYes,
               ID_BHFlipNo,
               s__change_param.i_hflip == 1 ?
               ID_BHFlipYes : ID_BHFlipNo, FALSE);
       }
       else
       {
           s__change_param.i_hflip =
               (Get_Check_Button(&change_dlg, ID_BHFlipYes) == TRUE ? 1 : 0);
           Set_Check_Button(&change_dlg, ID_BHFlipYes, FALSE);
           Set_Check_Button(&change_dlg, ID_BHFlipNo, FALSE);
       }
       Draw_Button(&buttons[NO_BHFlipYes]);
       Draw_Button(&buttons[NO_BHFlipNo]);
       break;

   case ID_BVFlipYes:
   case ID_BVFlipNo:
       if (TRUE == s__change_param.b_vflip)
       {
           s__change_param.i_vflip = (n == ID_BVFlipYes ? 1 : 0);
           Check_Radio_Button(&change_dlg,
               ID_BVFlipYes,
               ID_BVFlipNo, n, TRUE);
       }
       break;
   case ID_BVFlip:
       s__change_param.b_vflip = Get_Check_Button(&change_dlg, ID_BVFlip);
       buttons[NO_BVFlipYes].enable = s__change_param.b_vflip;
       buttons[NO_BVFlipNo].enable = s__change_param.b_vflip;
       if (TRUE == s__change_param.b_vflip)
       {
           Check_Radio_Button(&change_dlg,
               ID_BVFlipYes,
               ID_BVFlipNo,
               s__change_param.i_vflip == 1 ?
               ID_BVFlipYes : ID_BVFlipNo, FALSE);
       }
       else
       {
           s__change_param.i_vflip =
               (Get_Check_Button(&change_dlg, ID_BVFlipYes) == TRUE ? 1 : 0);
           Set_Check_Button(&change_dlg, ID_BVFlipYes, FALSE);
           Set_Check_Button(&change_dlg, ID_BVFlipNo, FALSE);
       }
       Draw_Button(&buttons[NO_BVFlipYes]);
       Draw_Button(&buttons[NO_BVFlipNo]);
       break;

   case ID_BOnTopYes:
   case ID_BOnTopNo:
       if (TRUE == s__change_param.b_ontop)
       {
           s__change_param.i_ontop = (n == ID_BOnTopYes ? 1 : 0);
           Check_Radio_Button(&change_dlg,
               ID_BOnTopYes,
               ID_BOnTopNo, n, TRUE);
       }
       break;
   case ID_BOnTop:
       s__change_param.b_ontop = Get_Check_Button(&change_dlg, ID_BOnTop);
       buttons[NO_BOnTopYes].enable = s__change_param.b_ontop;
       buttons[NO_BOnTopNo].enable = s__change_param.b_ontop;
       if (TRUE == s__change_param.b_ontop)
       {
           Check_Radio_Button(&change_dlg,
               ID_BOnTopYes,
               ID_BOnTopNo,
               s__change_param.i_ontop == 1 ?
               ID_BOnTopYes : ID_BOnTopNo, FALSE);
       }
       else
       {
           s__change_param.i_ontop =
               (Get_Check_Button(&change_dlg, ID_BOnTopYes) == TRUE ? 1 : 0);
           Set_Check_Button(&change_dlg, ID_BOnTopYes, FALSE);
           Set_Check_Button(&change_dlg, ID_BOnTopNo, FALSE);
       }
       Draw_Button(&buttons[NO_BOnTopYes]);
       Draw_Button(&buttons[NO_BOnTopNo]);
       break;
    case ID_CFontName:
          font_no= combobox[4].listbox->poz + combobox[4].listbox->foff;
          if (font_no>=WhNumberTextStyle)
          {
              Xm=X;
              Ym=Y;
              X=jednostkiX0_(PozX0) + 250.0/Get_Ds_x();
              Y=jednostkiY0_(PozY0) + pYk/Get_Ds();
              lock_mouse();
              GrMouseEraseCursor();
              ttf_otf = Simple_Menu_Proc_(&mTTF_OTF) - 1;
              X=Xm;
              Y=Ym;
              if (ttf_otf>-1)
              {
                  numbertextstyle=WhNumberTextStyle;
                  font_no = Add_New_Font_TTF_OTF(NULL, ttf_otf, 0);
                  if (font_no > -1)
                  {
                      if (numbertextstyle<WhNumberTextStyle) {
                          combobox[4].listbox->max++;
                          font_name_txt [WhNumberTextStyle-1] = pmCzcionka [WhNumberTextStyle-1].txt ;
                          font_name_txt [WhNumberTextStyle] = add_new_font_c ;
                          //combobox[4].listbox->txt[WhNumberTextStyle]=add_new_font_c;
                          font_name_txt [WhNumberTextStyle+1] = NoChange ;
                          //combobox[4].listbox->txt[WhNumberTextStyle+1]=NoChange;
                      }
                      Draw_ComboBox(&combobox[4]);
                      s__change_param.i_text_font = font_no;
                  }
              }

              free_mouse();
              GrMouseUnEraseCursor();
              position_mouse(PozX0, PozY0);
              show_mouse(screen);
          }
        break;
      
    default:
      ret = Dlg_Ret_Val_Continue;
      break;
  }
  return ret;
}

void reset_texts_width(long_long off, long_long offk)
{
	//LINIA *ob;
	TEXT *text;
	NAGLOWEK *nag;
	BLOK *adb;
	long_long ad, size;

	size = dane_size;
	Object_Tok_All(&off, offk, &ad, ONieOkreslony);
	while (ad != -1)
	{
		nag = (NAGLOWEK*)(dane + ad);
		switch (nag->obiekt)
		{
		case Otekst:
			text = (TEXT *)(dane + ad);
			text->width = 0;
			text->height = 0;
			off = ad + sizeof(NAGLOWEK) + ((NAGLOWEK*)(dane + ad))->n;
			break;
		case OdBLOK:  adb = (BLOK *)(dane + ad);
			off = ad + sizeof(NAGLOWEK) + B3 + adb->dlugosc_opisu_obiektu;
			break;
		default:  off = ad + sizeof(NAGLOWEK) + ((NAGLOWEK*)(dane + ad))->n;
			break;
		}
		offk += dane_size - size;
		Object_Tok_All(&off, offk, &ad, ONieOkreslony);
	}
}

static void change_properties
		(char  	  *adp,
		 char  	  *adk,
		 T_change_param   *lps_change_param)
/*-----------------------------------------------------*/
{
  LINIA *ob;
  WIELOKAT *ob1 ;
  ELLIPSE *ob2;
  SOLIDARC *ob3;
  TEXT *ptrs_t ;
  BOOL is_TTF;
  BOOL TTF_redraw;
  BOOL hflip, vflip, ontop;
  float d_trans = 12.75;
  int transluc;
  char* translucency_ptr;
  WIELOKAT solid_temp;
  BOOL changed_ptr = FALSE;

  if (adp == NULL || adk == NULL)
  {
    return ;
  }

  CUR_OFF (X,Y) ;
  blokzap(ADP,ADK,Ablok,XOR_PUT,1);
  blokzap_no_pcx(ADP,ADK,Ablok,COPY_PUT,0);
  obiekt_tok_all ((char*)adp, adk, (char **)&ob, ONieOkreslony) ;
  TTF_redraw = FALSE;
  
  while (ob != NULL)
  {
    if (ob->atrybut == Ablok && ob->obiekt != OdBLOK)
    {  
	   if (ob->obiekt == Opcx) 
	   {
           if (lps_change_param->b_hflip)
           {
               hflip = ((B_PCX*)ob)->h_flip;
               if (hflip != lps_change_param->i_hflip)
               {
                   ((B_PCX*)ob)->h_flip = lps_change_param->i_hflip;
                   TTF_redraw = TRUE;
                   regen_ctx = TRUE;
               }
           }

           if (lps_change_param->b_vflip)
           {
               vflip = ((B_PCX*)ob)->v_flip;
               if (vflip != lps_change_param->i_vflip)
               {mTypyLinii.off=
                   ((B_PCX*)ob)->v_flip = lps_change_param->i_vflip;
                   TTF_redraw = TRUE;
                   regen_ctx = TRUE;
               }
           }

           if (lps_change_param->b_ontop)
           {
               ontop = ((B_PCX*)ob)->on_front;
               if (ontop != lps_change_param->i_ontop)
               {
                   ((B_PCX*)ob)->on_front = lps_change_param->i_ontop;
                   TTF_redraw = TRUE;
                   regen_ctx = TRUE;
               }
           }
	   }
       if (lps_change_param->b_layer)
       {
	     ob->warstwa = lps_change_param->layer ;
		 if (ob->obiekt == Opcx)
		 {
			 Warstwy_On_PCX[ob->warstwa].check = 1;
			 Warstwy_On_PCX[ob->warstwa].on = Layers[ob->warstwa].on;
			 regen_ctx = 1;
		 }
         else if (ob->obiekt == Owwielokat) 
             {
                 ob1 = (WIELOKAT*)ob;
                 if ((ob1->empty_typ == 0) && (ob1->pattern == 1))
                 {
                     Warstwy_On_Solids[ob1->warstwa].check = 1;
                     Warstwy_On_Solids[ob1->warstwa].on = Layers[ob1->warstwa].on;
                 }
             }
         else if (ob->obiekt == Osolidarc)
         {
             ob3 = (SOLIDARC*)ob;
             if ((ob3->empty_typ == 0) && (ob3->pattern == 1))
             {
                 Warstwy_On_Solids[ob3->warstwa].check = 1;
                 Warstwy_On_Solids[ob3->warstwa].on = Layers[ob3->warstwa].on;
             }
         }
       }
       if (lps_change_param->b_color)
       {
	     ob->kolor = Menu_No_to_Color_No (lps_change_param->color) ;
       }
       if (ob->obiekt == Olinia || ob->obiekt == Ovector || ob->obiekt == Oluk || ob->obiekt == Ookrag || ob->obiekt == Ospline || ob->obiekt == Oellipse || ob->obiekt == Oellipticalarc)
       {
	     if (lps_change_param->b_width)
	      {
		   if (ob->typ!= HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
	          ob->typ = Set_WidthL_to_Type (ob->typ, lps_change_param->width) ;
	      }
	     if (lps_change_param->b_type)
	      {
			if (ob->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
	         ob->typ = Set_TypeL_to_Type (ob->typ,  lps_change_param->type) ;
	      }
       }
       if (ob->obiekt == Owwielokat)
       {
	     if (lps_change_param->b_width)
	      {
          ob1=(WIELOKAT *) ob;
          if (ob1->pattern==0) ob1->empty_typ = Set_WidthSolid_to_Type (ob1->empty_typ, lps_change_param->width) ;  //protecting image patterns
	      }
       }
       if (ob->obiekt == Osolidarc)
        {
            if (lps_change_param->b_width)
            {
                ob3=(SOLIDARC *) ob;
                if (ob3->pattern==0) ob3->empty_typ = Set_WidthSolid_to_Type (ob3->empty_typ, lps_change_param->width) ;  //protecting image patterns
            }
        }
       if (ob->obiekt == Otekst)
       {
	     ptrs_t = (TEXT*)ob ;
		 
		 if ((PTRS__Text_Style[ptrs_t->czcionka]->type == 2) || (PTRS__Text_Style[lps_change_param->i_text_font]->type == 2)) is_TTF = TRUE;
		    else is_TTF = FALSE;
	     if (lps_change_param->b_text_font)
	     {
	      ptrs_t->czcionka = lps_change_param->i_text_font ;
		  ptrs_t->width = 0;
		  ptrs_t->height = 0;
		  if (is_TTF) TTF_redraw = TRUE;
	     }
	     if (lps_change_param->b_text_type)
	     {
	      ptrs_t->typ = lps_change_param->i_text_type ;
	     }
	 
	     if (lps_change_param->b_text_ukryty)
	     {
        ptrs_t->ukryty = lps_change_param->i_text_ukryty ;
		if (is_TTF) TTF_redraw = TRUE;
	     }
	 
	     if (lps_change_param->b_text_just)
	     {
	      ptrs_t->justowanie = lps_change_param->i_text_just ;
		  if (is_TTF) TTF_redraw = TRUE;
	     }
	     if (lps_change_param->b_text_height)
	     {
	      ptrs_t->wysokosc= lps_change_param->df_text_height ;
		  ptrs_t->width = 0;
		  ptrs_t->height = 0;
		  if (is_TTF) TTF_redraw = TRUE;
	     }
	     if (lps_change_param->b_text_width)
	     {
	      ptrs_t->width_factor = lps_change_param->df_text_width ;
		  ptrs_t->width = 0;
		  ptrs_t->height = 0;
		  if (is_TTF) TTF_redraw = TRUE;
	     }
	     if (lps_change_param->b_text_italics)
	     {
	      ptrs_t->italics = lps_change_param->i_text_italics ;
		  ptrs_t->width = 0;
		  ptrs_t->height = 0;
		  if (is_TTF) TTF_redraw = TRUE;
	     }
	     if (lps_change_param->b_text_bold)
	     {
	      ptrs_t->bold = lps_change_param->i_text_bold ;
		  ptrs_t->width = 0;
		  ptrs_t->height = 0;
		  if (is_TTF) TTF_redraw = TRUE;
	     }
		 if (lps_change_param->b_text_underline)
		 {
			 ptrs_t->underline = lps_change_param->i_text_underline;
			 if (is_TTF) TTF_redraw = TRUE;
		 }
       }
       if (lps_change_param->b_translucency)
       {
           if (ob->obiekt == Owwielokat /*|| ob->obiekt == Owwielokat3D*/)
           {
               //calculating translucency
               transluc = 255 -  (int)((float)lps_change_param->i_translucency * d_trans+0.5);
               ob1 = (WIELOKAT*)ob;
               //elimination patterned solids
               if ((ob1->empty_typ == 0) && (ob1->pattern == 0))
               {
                   //for already translucent solid, even if transluc==255
                   if (ob1->translucent == 1)
                   {
                       translucency_ptr = ob1->xy;
                       translucency_ptr += (ob1->lp * sizeof(float));
                       memmove(translucency_ptr, &transluc, sizeof(unsigned char));
                   }
                   else //needs correction
                   {
                       memcpy(&solid_temp, ob1, sizeof(WIELOKAT));
                       solid_temp.translucent = 1;
                       solid_temp.n = 8 + solid_temp.lp * sizeof(float) + sizeof(unsigned char);

                       translucency_ptr = solid_temp.xy;
                       translucency_ptr += (solid_temp.lp * sizeof(float));
                       memmove(translucency_ptr, &transluc, sizeof(unsigned char));

                       if ((ob = korekta_obiekt((void*)ob, (void*)&solid_temp)) == NULL)
                               ErrList(15);
                       changed_ptr = TRUE;
                   }
               }
      
           }
           else if (ob->obiekt == Osolidarc)
           {
               //calculating translucency
               transluc = 255 -  (int)((float)lps_change_param->i_translucency * d_trans+0.5);
               ob3 = (SOLIDARC*)ob;
               //elimination patterned solids
               if ((ob3->empty_typ == 0) && (ob3->pattern == 0))
               {
                   //for already translucent solid, even if transluc==255
                   ob3->translucent = 1;
                   ob3->translucency = (unsigned char) transluc;
               }
           }
           else if (ob->obiekt == Ofilledellipse)
           {
               //calculating translucency
               ob2 = (ELLIPSE*)ob;
               transluc = 255 -  (int)((float)lps_change_param->i_translucency * d_trans+0.5);
               ob2->translucency=transluc;
           }
       }
    }
    obiekt_tok_all (NULL, adk, (char **)&ob, ONieOkreslony) ;
  }

  if (changed_ptr == TRUE)
  {
      ADP = dane;
      ADK = dane + dane_size;
      adp = ADP;
      adk = ADK;
  }
  blokzap_no_pcx(ADP,ADK,Ablok,COPY_PUT,1);
  zmien_atrybut (dane, dane + dane_size, Aoblok, Anormalny) ;
  zmien_atrybut (adp, adk, Ablok, Aoblok) ;
  ADP = ADK = NULL ;
  Change = TRUE;
  Set_Auto_Backup (TRUE) ;
  CUR_OFF (X,Y) ;
  CUR_ON (X,Y) ;
  if ((TTF_redraw) || (regen_ctx)) redraw();
  return ;
}

void Change_Properties_dlg (void)
/*-----------------------------*/
{
  DLG_COLOR color_dlg ;
  int dlg_ret ;
  //int k;
  int font_no, i_poz1;
  static int curr_h, curr_v;

  memcpy (&s__change_param, &s__change_param0, sizeof (T_change_param)) ;
  set_struct_dialog_control () ;
  Get_Default_Color_Dlg (&color_dlg) ;

  Save_Update_flex(0, &curr_h, &curr_v);
  dlg_ret = Dialog(&change_dlg, &color_dlg, proc_dlg, bMouse) ;
  Save_Update_flex(1, &curr_h, &curr_v);

  Colors_Name_txt [MAXCOLOR] = "" ;

  if (dlg_ret == Dlg_Ret_Val_OK)
  {
    memcpy (&s__change_param0, &s__change_param, sizeof (T_change_param)) ;

	if ((s__change_param.b_text_font) && (s__change_param.i_text_font == WhNumberTextStyle))
	{
        i_poz1 = mTTF_OTF.poz + mTTF_OTF.foff;
		font_no = Add_New_Font_TTF_OTF(NULL, i_poz1, 1);
		if (font_no == -1) return;
		s__change_param.i_text_font = font_no;
	}

    change_properties (ADP, ADK, &s__change_param0) ;
  }
  return ;
}

void Reset_change_dlg(void)
{
    memcpy(&s__change_param0, &s__change_param00, sizeof(T_change_param));
}


void Get_change_dlg(T_change_param *s__change_param_)
{
    memcpy(s__change_param_, &s__change_param0, sizeof(T_change_param));
}

void Set_change_dlg(T_change_param* s__change_param_)
{
    memcpy(&s__change_param0, s__change_param_, sizeof(T_change_param));
}

#undef __CHPROP__
