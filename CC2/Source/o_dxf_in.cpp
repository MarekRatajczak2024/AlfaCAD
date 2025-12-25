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

#define __O_DXF_IN__
#include"forwin.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#ifndef LINUX
#include<io.h>
#endif

#include <iostream>

#include<fcntl.h>
#include<sys/stat.h>
#include <math.h>
#define ALLEGWIN
#include "allegext.h"
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_spec.h"
#include "alffont.h"
#include "o_libfun.h"

#include <zlib.h>

#include "menu.h"

#include "leak_detector_cpp.hpp"

#define SEEK_SET        0
#define SEEK_CUR	1
#define SEEK_END	2

#define maxlen_w_AC 64
#define MaxLen 120
#define MAXLINE 1024 /*260*/  /*120*/
#define MAXLINE1 1024 //512  in newest vetrsion it is even 2048
#define MaxVarLen 60
#define row_spc 1.5
#define MAXPAR 64
#define MAXPAR1 126


#ifdef NDEBUG
////#define NOSHOWLINES
#endif

std::string utfstr;

extern std::string cp2utf(std::string s);

#pragma pack(1)

#define PATERN_NAME_MAXLEN 36 //32
#define MaxNumberSegPattern 12 //24 //6

#define BOX_H_CORRECTION 0.93


typedef struct
{
    double df_angle ;	/*kat pochylenia linii*/
    double df_startx, df_starty ;	/*przesuniecie punktu startowego linii (w jednostkach rysunku)*/
    double df_move ;	/*przesuniecie (w jednostkach rysunku), dla linii ciaglej nieistotne*/
    double df_dist ;      /*odleglosc miedzy liniamii(w jednostkach rysunku)*/
    double df_len_pattern ;/*dlugosc wzorca(w jednostkach rysunku)*/
    int i_def_size ;      /*ilosc segmentow definicji, 0 dla linii ciaglej*/
    int reserve;
    double df_def_line [MaxNumberSegPattern] ; /*dlugosci poszczegolnych segmentow (w jednostkach rysunku)*/
}
        T_Hatch_Line_Def, *T_PTR_Hatch_Line_Def ;

typedef struct
{
    char sz_name [PATERN_NAME_MAXLEN] ;/*nazwa wzoru wypelnienia*/
    int  i_number_line ;	/*numer w tablicy definicji linii wypelnien*/
    int  i_size ;		/*ilosc linii definiujacych wzor*/
}
        T_Hatch_Def_Param, *T_PTR_Hatch_Def_Param ;

typedef struct
{
    int i_number_hatch ;		/*numer wzoru wypelnienia w tablicy definicji wypelnien*/
    double df_angle ;		/*kat pochylenia linii (w radianach !!!)*/
    double df_scale ;		/*wspolczynnik skali*/
    double df_basex, df_basey ;  	/*wspolrzedne punktu bazowego wypelnien (w jednostkach rysunku)*/
    BOOL insulation_hatch;
}
    T_Hatch_Param, *T_PTR_Hatch_Param ;

typedef struct
{
    char handle[16];
    char name[MAXLINE];
} BLOCK_RECORD;

BLOCK_RECORD *block_record=NULL;

int block_record_no;
int block_record_no_max=100;

int inserts=0;

int leaders_no=0;
int mleaders_no=0;

static double text_annotation_height=3.0, text_annotation_width=1.0, text_annotation_rotation=0.0, text_annotation_spacing=1.0;
static char annotation[MAXLINE]="";
static double et_x, et_y, et_z;
static BOOL mleader_text_location=FALSE;
static char style_file_1000[MAXLINE]="";

double last_mtext_h;

#pragma pack ()
#define DWORD  		unsigned int //long    int for 64 bit, long for 32 bit
#define WORD  		unsigned short
#define BYTE  		unsigned char
extern char code999[5][8];

extern "C" {

extern BOOL is_utf8(const char* string);

extern void Normalize_Solid (WIELOKAT *) ;
extern void POLYARC(double x_s, double y_s, double x_e, double y_e, double wpsc, LUK *l);
extern void Insert_Block(INSERT *I,int to_block1, BOOL block);
extern void zmien_obiektt2_warstwe(char  *adr,char  *adrk,int o_old,int o_new, int new_layer);
extern void usun_bloki_obiektt2(char  *adr, char  *adrk, int o_old);
extern BOOL get_all_ep_space (char *adp0, char *adk0, char  **adp_l,char  **adk_l);
extern void reset_przec_dxf (char  *adr,char  *adrk);
extern void delete_przec_dxf (char  *adr,char  *adrk);
extern void setfillstyle_(int pattern, int color);
extern void setfillpattern_(int pattern, int color);
extern void setcolor(int kolor);
extern int getcolor(void);
extern char *find_obj(char *adrp, char *adrk, int obiekt, int typ_obiektu, int info_p);
extern void korekta_obiektow_blokow(void);
extern void Get_Limits_NO_255 (long_long off, long_long offk, int atrybut, double *xmin, double *xmax, double *ymin, double *ymax);
extern BOOL Set_Block_Proc(double x, double y, char *blok_type);
extern int fnsplit(const char *path, char *drive, char *dir, char *name, char *ext);
extern DWORD RunSilent(char* strFunct, char* strstrParams);
extern int LoadPNGinfo(char *filename, unsigned int *width, unsigned int *height, unsigned int *dpiX, unsigned int *dpiY, unsigned char* bits_per_pixel);
extern int LoadJPGinfo(char* filename, unsigned int* width, unsigned int* height, unsigned int* dpiX, unsigned int* dpiY, unsigned char* bits_per_pixel);
extern int Load_PNG_JPG_real(B_PCX* b_pcx, char* nazwa_pcx, size_t width, size_t height, int horz_res, int vert_res, unsigned char bits_per_pixel, int type);
extern LAYER Layers[MAX_NUMBER_OF_LAYERS];
extern void Get_Limits(long_long off, long_long offk, int atrybut, double *xmin, double *xmax, double *ymin, double *ymax);
extern int my_kbhit(void);
extern unsigned char my_getch(void);
extern int ask_question(int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien, int image);
BOOL glb_silent = FALSE;
extern int32_t ucs2_to_utf8(int32_t ucs2, uint8_t * utf8);

extern int Add_New_Font_TTF_OTF(char *name, int type, int cur);
extern int find_font_ALFA(char *stylefile, double *e_wspx1, int type);
void decodingwin(char *text);
void decodingwin_true(char *text);

extern int hatch_proc (long_long l_offb, long_long l_offe, int seeds_no,
                 double df_pointx[], double df_pointy[],
                 T_PTR_Hatch_Param ptrs_hatch_param, int comput_area,
                 double df_apx1, double df_apy1, double df_apx2, double df_apy2, void *adb, int to_block, int draw);  //to_block=0, int draw=TRUE
extern void settle_hatch_pattern(int no);

extern void set_solidhatchtranslucency(int poz0, int opcja);

extern void set_st_jedn(void);

T_PTR_Hatch_Def_Param Get_Hatch_Def_Table999_ (void);
extern void generate_leader(POINTD *vertices, int vertex_no, LINIA *sample_line, TEXT *sample_text, int to_block, int line_no, BOOL mleader_text_location, double et_x, double et_y, double mtext_h);

extern double Get_Text_Len (TEXT *ptrs_text, char *ptrsz_t, double font_scale, int font_index, int *t_width, int *t_height, double *matrix_d, int *i_matrix_d);
}

extern int win2unicode(char *wintext, char *unicodetext);
extern int win2unicodedxf(char *wintext, char *unicodetext);
extern void unicode2windxf(char *unicodetext, char *wintext, int count);
extern void unicode2utf8(char *unicodetext, unsigned char *utf8text);

extern char *add_block_hatch(BOOL hatchoutline);
extern char *add_block_hatch_in_block(BOOL hatchoutline);

void unicode2utf8char(char *unicode, unsigned short *utf8);
void add_font_DXF(char *fontname);
void scan_text_style_(char *text_style_, char *font_, char *bold_, char *italic_, char *codepage_, char *points_);


T_PTR_Hatch_Def_Param Get_Hatch_Def_Table999 (void);
T_PTR_Hatch_Line_Def Get_Hatch_Line_Def_Table999 (void);

double df_x[4], df_y[4]; //TEMPORARY, it must be array of variables
T_Hatch_Def_Param ptrs__hatch_def_param999={"",999,0};
T_Hatch_Line_Def *s_hatch_line_def999=NULL;

typedef struct
{
	double thinest;
	double thin;
	double normal;
	double thick;
	double thickest;
} Line_Width;

                      //b.c.  c.  g.  b.g.  ng.
Line_Width line_width={0.18,0.25,0.35,0.50,0.70};

char Layers_name[256][maxlen_w+2];
char Layers_name_DXF[256][maxlen_w+2];
#define max_pcx_number 64
int pcx_number = 0;
B_PCX_DEF *dxf_pcx_def_p[max_pcx_number];

static int no_line_width_dxf= sizeof(line_width_dxf_param) / sizeof(line_width_dxf_param[0]);
static int solid_type=0;
static WIELOKAT solid3_hatch=Stdef;
static WIELOKAT solid4_hatch=S4def;
static WIELOKAT *solid_hatch;
static BOOL solid_hatch_ok;

typedef struct
{
  BOOL b_line, b_solid ; 	//narysowano linie, narysowano obszar
  double width1 ;		//szerokosc poczatkowa pierwszego segmentu
  double width ;		//szerokosc biezaca
  double wypuklosc ;
  LINIA line ;
  int color_line ;
  WIELOKAT solid ;
}
t_pline ;


static t_pline  s_pline = {FALSE, FALSE, 0, 0, 0, Ldef, 7, S4def};
static LINIA line_g = Ldef ;

long_long global_block;
int acad_ver=14;
int nw;
int nr_linii;
int zamkniecie;
double start_x, start_y, start_width;
double wypuklosc, wypuklosc0;
double znacznik_wierzcholka;
BOOL drugi_wezel, trzeci_wezel, first_arc, fit_curve, myspline, planarspline, linearspline;
double width_p_standard,width_k_standard;
int numer_polilinii;
double epsilon=0.00001;
double x_rp,y_rp,z_rp,width_p,width_k,kierunek_stycznej;
double width_p0, width_k0;
double x_rp0,y_rp0,z_rp0;
double r_parc,x_r_parc,y_r_parc;
double k_parc,k_karc;
unsigned char ep_layer=0;
unsigned char ep_space=0;
unsigned char ep_novisibility = 0;
unsigned char ep_type=64;
double ep_width;
int ep_color=7;
int ep_translucency=255;
double line_g_x, line_g_y;
    
static int No_Styles_DXF=0;
char ACADVER[MaxVarLen]="AC1011";
char ACCOMMENT[MaxVarLen] = "";
double INSBASE_X=0.0,INSBASE_Y=0.0,INSBASE_Z=0.0;
double EXTMIN_X=0,EXTMIN_Y=0,EXTMIN_Z=0;
double EXTMIN_X0=0, EXTMIN_Y0=0,EXTMIN_Z0=0;
double EXTMAX_X0 = 10, EXTMAX_Y0 = 10, EXTMAX_Z0 = 0;
double EXTMIN_XE=0, EXTMIN_YE=0, EXTMIN_ZE=0;
double EXTMAX_X=10,EXTMAX_Y=10,EXTMAX_Z=0;
double LIMMIN_X=0.0,LIMMIN_Y=0.0;
double LIMMAX_X=10,LIMMAX_Y=10;
int ORTHOMODE=0,REGENMODE=1,FILLMODE=1,QTEXTMODE=0,MIRRTEXT=1,DRAGMODE=2;
double LTSCALE=1.0;
int OSMODE=0,ATTMODE=1;
double TEXTSIZE=0.2,TRACEWID=0.05;
char TEXTSTYLE[MaxVarLen]="STANDARD", CLAYER[MaxVarLen]="0",CELTYPE[MaxVarLen]="BYLAYER";
int CECOLOR=256;
int INSUNITS = 4;
double DIMSCALE=1.0,DIMASZ=0.18,DIMEXO=0.0625,DIMDLI=0.38,DIMRND=0.01,DIMDLE=0.0;
char DIMTXSTY[MaxVarLen] = "STANDARD";
double DIMEXE=0.18,DIMTP=0.0,DIMTM=0.0,DIMTXT=0.18,DIMCEN=0.09,DIMTSZ=0.0;
int DIMTOL=0,DIMLIM=0,DIMTIH=1,DIMTOH=1,DIMSE1=0,DIMSE2=0,DIMTAD=0,DIMZIN=0;
char DIMBLK[MaxVarLen]="";
int DIMASO=1,DIMSHO=1;
char DIMPOST[MaxVarLen]="",DIMAPOST[MaxVarLen]="";
int DIMALT=0,DIMALTD=2;
double DIMALTF=25.4,DIMLFAC=1.0;
int DIMTOFL=0;
double DIMTVP=0.0;
int DIMTIX=0,DIMSOXD=0,DIMSAH=0;
char DIMBLK1[MaxVarLen]="",DIMBLK2[MaxVarLen]="",DIMSTYLE[MaxVarLen]="*UNNAMED";
int DIMCLRD=0,DIMCLRE=0,DIMCLRT=0;
double DIMTFAC=1.0,DIMGAP=0.09;
int LUNITS=2,LUPREC=4;
double SKETCHINC=0.1,FILLETRAD=0.0;
int AUNITS=0,AUPREC=0;
char MYMENU[MaxVarLen]="acad";
double ELEVATION=0.0,PELEVATION=0.0,THICKNESS=0.0;
int LIMCHECK=0,BLIPMODE=1;
double CHAMFERA=0.0,CHAMFERB=0.0;
int SKPOLY=0;
double TDCREATE=2450901.381882175,TDUPDATE=2450901.381882175,TDINDWG=0.0000000000;
double TDUSRTIMER=0.0000000000;
int USRTIMER=1;
double ANGBASE=0.0;
int ANGDIR=0,PDMODE=0;
double PDSIZE=0.0,PLINEWID=0.0;
int COORDS=1,SPLFRAME=0,SPLINETYPE=6,SPLINESEGS=8,ATTDIA=0,ATTREQ=1,HANDLING=0;
char HANDSEED[MaxVarLen]="0";
int SURFTAB1=6,SURFTAB2=6,SURFTYPE=6,SURFU=6,SURFV=6;
char UCSNAME[MaxVarLen]="";
double UCSORG_X=0.0,UCSORG_Y=0.0,UCSORG_Z=0.0,UCSXDIR_X=1.0,UCSXDIR_Y=0.0,UCSXDIR_Z=0.0;
double UCSYDIR_X=0.0,UCSYDIR_Y=1.0,UCSYDIR_Z=0.0;
char PUCSNAME[MaxVarLen]="";
double PUCSORG_X=0.0,PUCSORG_Y=0.0,PUCSORG_Z=0.0,PUCSXDIR_X=1.0,PUCSXDIR_Y=0.0,PUCSXDIR_Z=0.0;
double PUCSYDIR_X=0.0,PUCSYDIR_Y=1.0,PUCSYDIR_Z=0.0;
int USERI1=0,USERI2=0,USERI3=0,USERI4=0,USERI5=0;
double USERR1=0.0,USERR2=0.0,USERR3=0.0,USERR4=0.0,USERR5=0.0;
int WORLDVIEW=1,SHADEDGE=3,SHADEDIF=70,TILEMODE=1,MAXACTVP=16;
double PINSBASE_X=0.0,PINSBASE_Y=0.0,PINSBASE_Z=0.0;
int PLIMCHECK=0;
double PEXTMIN_X=1.000000E+20,PEXTMIN_Y=1.000000E+20,PEXTMIN_Z=1.000000E+20;
double PEXTMAX_X=-1.000000E+20,PEXTMAX_Y=-1.000000E+20,PEXTMAX_Z=-1.000000E+20;
double PLIMMIN_X=0.0,PLIMMIN_Y=0.0,PLIMMAX_X=12.0,PLIMMAX_Y=9.0;
int UNITMODE=0,VISRETAIN=0,PLINEGEN=0,PSLTSCALE=1,TREEDEPTH=3020;
char DWGCODEPAGE[MaxVarLen]="ascii";
static double move_x, move_y, move_z;
int surface_type;

static double FormatYY, DELXX, DELYY;

#define acad_line_nr 26
static
 char * acad_line[]={
	"Continuous",//solid line"CONTINUOUS",//solid line
    "BORDER", //__ __ . __ __ . __ __ . __ __ . __ __ . __ __ . __ __ . __ __ .
    "BORDER2", //__.__.__.__.__.__.__.__.__.__.__.__.__.__.__.__.__.__.__.__.__
    "BORDERX2",//____  ____  .  ____  ____  .  ____  ____  .  ____  ____  .
    "CENTER",//____ _ ____ _ ____ _ ____ _ ____ _ ____ _ ____ _ ____ _ ____ _
    "CENTER2",//___ _ ___ _ ___ _ ___ _ ___ _ ___ _ ___ _ ___ _ ___ _ ___ _ _
    "CENTERX2",//________  __  ________  __  ________  __  ________  __  ____
    "DASHDOT",//__ . __ . __ . __ . __ . __ . __ . __ . __ . __ . __ . __ . _
    "DASHDOT2",//_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.
    "DASHDOTX2",//____  .  ____  .  ____  .  ____  .  ____  .  ____  .  ____
    "DASHED",//__ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
    "DASHED2",//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    "DASHEDX2",//____  ____  ____  ____  ____  ____  ____  ____  ____  ____
    "DIVIDE",//____ . . ____ . . ____ . . ____ . . ____ . . ____ . . ____ . .
    "DIVIDE2",//__..__..__..__..__..__..__..__..__..__..__..__..__..__..__..__..
    "DIVIDEX2",//________  .  .  ________  .  .  ________  .  .  ________  .  .
    "DOT",//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    "DOT2",//................................................................
    "DOTX2",//.  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    "HIDDEN",//__ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
    "HIDDEN2",//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    "HIDDENX2",//____ ____ ____ ____ ____ ____ ____ ____ ____ ____ ____ ____
    "PHANTOM",//______  __  __  ______  __  __  ______  __  __  ______  __  __
    "PHANTOM2",//___ _ _ ___ _ _ ___ _ _ ___ _ _ ___ _ _ ___ _ _ ___ _ _ ___ _ _
    "PHANTOMX2",//____________    ____    ____    ____________    ____    ____
    "AUSGEZOGEN"//solid line
    };
unsigned char acad_ltype [acad_line_nr] ={64,69,77,85,70,78,86,66,74,82,
                          65,73,81,67,75,83,68,76,84,71,
                          79,87,72,80,88,64};
                          
#define quasi_acad_line_nr 8
static char * quasi_acad_line[]={
    "BORDER", //__ __ . __ __ . __ __ . __ __ . __ __ . __ __ . __ __ . __ __ .
    "CENTER",//____ _ ____ _ ____ _ ____ _ ____ _ ____ _ ____ _ ____ _ ____ _
    "DASHDOT",//__ . __ . __ . __ . __ . __ . __ . __ . __ . __ . __ . __ . _
    "DASHED",//__ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
    "DIVIDE",//____ . . ____ . . ____ . . ____ . . ____ . . ____ . . ____ . .
    "DOT",//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    "HIDDEN",//__ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
    "PHANTOM",//______  __  __  ______  __  __  ______  __  __  ______  __  __
    };
unsigned char quasi_acad_ltype [quasi_acad_line_nr] ={69,70,66,65,67,68,71,72};

#define alf_line_nr  32

static char * alf_line_en [] = {
   "Continuous",
   "Dashed",
	"Dashdotted",
	"2-dotted",
	"Dotted",
	"Border",
	"Center",
	"Hidden",
	"Phantom",
	"Dashed_2",
	"Dashdotted_2",
	"2-dotted_2",
	"Dotted_2",
	"Border_2",
	"Center_2",
	"Hidden_2",
	"Phantom_2",
	"Dashed_x2",
	"Dashdotted_x2",
	"2-dotted_x2",
	"Dotted_x2",
	"Border_x2",
	"Center_x2",
	"Hidden_x2",
	"Phantom_x2",
	"3-dotted",
	"2-center",
	"3-dashed",
	"Phantom_3",
	"2-center_x2",
	"3-dashed_x2",
	"Multidashed",
   };  

static char * alf_line_pl[] = {
   "Continuous",
	"Kreskowa",
	"Punktowa",
	"Dwupunktowa",
	"Wielopunktowa",
	"Granica",
	"Srodkowa",
	"Ukryta",
	"Widmo",
	"Kreskowa_2",
	"Punktowa_2",
	"Dwupunktowa_2",
	"Wielopunktowa_2",
	"Granica_2",
	"Srodkowa_2",
	"Ukryta_2",
	"Widmo_2",
	"Kreskowa_x2",
	"Punktowa_x2",
	"Dwupunktowa_x2",
	"Wielopunktowa_x2",
	"Granica_x2",
	"Srodkowa_x2",
	"Ukryta_x2",
	"Widmo_x2",
	"Trzypunktowa",
	"Dwusrodkowa",
	"Trzykreskowa",
	"Widmo_3",
	"Dwusrodkowa_x2",
	"Trzykreskowa_x2",
	"Wielokreskowa",
};


unsigned char alf_ltype[alf_line_nr] = { 64,65,66,67,68,69,70,71,72,73,
                                        74,75,76,77,78,79,80,81,82,83,
                                        84,85,86,87,88,89,90,91,92,93,
                                        94,95 };
typedef enum
{
    IDthinest_width = 0,
    IDthin_width,
    IDnor_width,
    IDthick_width,
    IDthickest_width,
}ID_Line_width;


#ifndef LINUX
#define LONG long
typedef signed int ssize_t;

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#endif

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;


typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD          bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;


struct pPoint {
    double x, y;
};

struct pline {
    pPoint p1, p2;
};

void insertString(char* destination, int pos, char* seed)
{
    char * strC;

    strC = (char*)malloc(strlen(destination)+strlen(seed)+1);
    strncpy(strC,destination,pos);
    strC[pos] = '\0';
    strcat(strC,seed);
    strcat(strC,destination+pos);
    strcpy(destination,strC);
    free(strC);
}

bool onLine(pline l1, pPoint p)
{
    // Check whether p is on the line or not
    if (p.x <= max(l1.p1.x, l1.p2.x)
        && p.x <= min(l1.p1.x, l1.p2.x)
        && (p.y <= max(l1.p1.y, l1.p2.y)
            && p.y <= min(l1.p1.y, l1.p2.y)))
        return true;

    return false;
}

int direction(pPoint a, pPoint b, pPoint c)
{
    double val = (b.y - a.y) * (c.x - b.x)
              - (b.x - a.x) * (c.y - b.y);

    if (Check_if_Equal(val, 0))

        // Collinear
        return 0;

    else if (Check_if_LT(val, 0.0))

        // Anti-clockwise direction
        return 2;

    // Clockwise direction
    return 1;
}

bool isIntersect(pline l1, pline l2)
{
    // Four direction for two lines and points of other line
    int dir1 = direction(l1.p1, l1.p2, l2.p1);
    int dir2 = direction(l1.p1, l1.p2, l2.p2);
    int dir3 = direction(l2.p1, l2.p2, l1.p1);
    int dir4 = direction(l2.p1, l2.p2, l1.p2);

    // When intersecting
    if (dir1 != dir2 && dir3 != dir4)
        return true;

    // When p2 of line2 are on the line1
    if (dir1 == 0 && onLine(l1, l2.p1))
        return true;

    // When p1 of line2 are on the line1
    if (dir2 == 0 && onLine(l1, l2.p2))
        return true;

    // When p2 of line1 are on the line2
    if (dir3 == 0 && onLine(l2, l1.p1))
        return true;

    // When p1 of line1 are on the line2
    if (dir4 == 0 && onLine(l2, l1.p2))
        return true;

    return false;
}

bool checkInside(pPoint poly[], int n, pPoint p)
{

    // When polygon has less than 3 edge, it is not polygon
    if (n < 3)
        return false;

    // Create a point at infinity, y is same as point p
    pline exline = { p, { 999999., p.y } };
    int count = 0;
    int i = 0;
    do {

        // Forming a line from two consecutive points of
        // poly
        pline side = { poly[i], poly[(i + 1) % n] };
        if (isIntersect(side, exline)) {

            // If side is intersects exline
            if (direction(side.p1, p, side.p2) == 0)
                return onLine(side, p);
            count++;
        }
        i = (i + 1) % n;
    } while (i != 0);

    // When count is odd
    return count & 1;
}

/*
    // Function call
    Point polygon[]={,,,,,,}

    if (checkInside(polygon, n, p))
        cout << "Point is inside.";
    else
        cout << "Point is outside.";
*/
////

int GetColorALF(int color, int layer_no)
{ int colorALF;
  static int transfalf[]={  0,  1,  2,  3,  4,  5,  6,  7,  /*246*/ 8, 15,  1, 44, 72, 85,  9,151,168, //0-16
                              167,168,242, 56, 38, 73, 85,152, 10,152,134,168,242, 41, 27, 88, //17-32
                               87,153, 10,152,134,169,242, 25, 27, 89, 87, 10,135,184,134,169, //33-48
                              242, 24, 22,104,101,104,135,184,134,169,242,106,107,121,123,185, //49-64
                              136,184,134,169,242,201,107,200,123,217,186,232,134,169,242,201, //65-80
                              118,216,197,217,186,232,134,232,242,216,214,216,197,217,218,232, //81-96
                              231,232,242,215,116,199,197,219,220,231,230,232,242,215,211,197, //97-112
                              196,219,220,231,230,229,242,210,211,196,196,220,189,230,230,229, //113-128
                              242,209,208,222,194,222,222,230,230,229,242,193,193,224,192,228, //129-144
                              222,230,230,229,242,225,193,226,191,238,228,236,230,229,242,240, //145-160
                              192,239,144,238,228,236,230,234,242,240,177,239,144,237,163,235, //161-176
                              172,234,242,240,145,162,144,163,146,165,166,234,242,175, 81,160, //177-192
                               81,163,146,165,166,166,242, 65, 81, 65, 81,165, 66,157,166,166, //193-208
                              242, 63, 64, 64, 65, 67,157,166,166,166,242, 62, 50, 68, 77, 69, //209-224
                               66,166,166,166,242, 60, 47, 69, 77,154,150,167,166,166,242, 58, //225-240
                               46, 71, 85,154,150,167,166,168,242,242,244,246,249,252,  7,  0,}; //241-256

  if (color<17)
   {
     if (acad_ver<14)
      colorALF=color;
       else
        colorALF=transfalf[color];
   }
    else
     {
      if (color<256) colorALF=transfalf[color];
       else  //bylayer
      {
           colorALF=Layers[layer_no].color;
      }
     }

  return colorALF;
}

char *myfgets(char *buf, int maxline, FILE * f) {
    if (fgets(buf, maxline, f) == NULL) return NULL;
    if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
    if (buf[strlen(buf) - 1] == '\r') buf[strlen(buf) - 1] = '\0';
    return buf;
}

static BOOL get_lines_width_dxf (T_Fstring key_name, T_Fstring ret_string)
/*---------------------------------------------------------------------*/
{
   int i;

   for (i = 0; i < no_line_width_dxf; i++)
   {

      if (stricmp (key_name, line_width_dxf_param [i]) == 0)
      {
	    break;
      }
   }
   if (i >= no_line_width_dxf)
   {
		return TRUE ;
   }
  switch (i)
  {
    case IDthinest_width:
      if ( sscanf ( ret_string, "%f", &line_width.thinest ) < 1 ) goto aa;
      if ( line_width.thinest <= 0 ) goto aa;
    break;
    case IDthin_width:
      if ( sscanf ( ret_string, "%f", &line_width.thin ) < 1 ) goto aa;
      if ( line_width.thin <= 0 ) goto aa;
    break;
    case IDnor_width:
      if ( sscanf ( ret_string, "%f", &line_width.normal ) < 1 ) goto aa;
      if ( line_width.normal <= 0 ) goto aa;
    break;
    case IDthick_width:
      if ( sscanf ( ret_string, "%f", &line_width.thick ) < 1 ) goto aa;
      if ( line_width.thick <= 0 ) goto aa;
    break;
    case IDthickest_width:
      if ( sscanf ( ret_string, "%f", &line_width.thickest ) < 1 ) goto aa;
      if ( line_width.thickest <= 0 ) goto aa;
    break;
    default:
    break;
  }
  return TRUE;
aa:
  return FALSE;
}



static void show_nr_linii (void)
/*----------------------------*/
{
  
  if (!glb_silent)
  {
#ifdef NOSHOWLINES
	  ;
#else

      int x,y;
      char buf [MaxTextLen] ;
      int nn;

	  x = maxX - 30 * WIDTH;  //209
	  y = 1;
	  moveto(x, y);
	  setfillstyle_(SOLID_FILL, BKCOLOR);
	  bar(x, y, maxX, y + ED_INF_HEIGHT - 2);
	  setcolor(kolory.ink);

	  strcpy(buf, _LINE_NO_);

	  outtext_r(buf);
#endif
  }
  return;
}

static int inc_nr_linii (void)
/*----------------------------*/
{

  int nn;
  int ret;
  char str[32], str1[32];

  nr_linii+=1;
  nn=(nr_linii/1000)*1000;
  if (nr_linii!=nn) return 1;

  if (!glb_silent)
  {
#ifdef NOSHOWLINES
	  ;
#else
      int x,y;
      char buf [MaxTextLen] ;

	  x = maxX - 20 * WIDTH;  //150
	  y = 1;
	  setfillstyle_(SOLID_FILL, BKCOLOR);
	  bar(x, y, maxX, y + ED_INF_HEIGHT - 2);
	  setcolor(kolory.ink);
	  moveto(x + 5, y + 1);
	  sprintf(buf, "%#ld", nr_linii);
	  buf[18] = '\0';
	  outtext_r(buf);
#endif
  }
  if (my_kbhit())
  {
	  if (my_getch() == ESC)
	  {
		  while (my_kbhit())
		  {
			  my_getch();
		  }
		  
		  sprintf(str, _SYSTEM_MESSAGE_);
		  sprintf(str1, _INTERRAPTED_);

		  InfoList(0);
		  ret = ask_question(1, "", "OK", "", str, 12, str1, 11, 1, 0);

		  return 0;

	  }
  }

  return 1;
}


#ifdef LINUX

static wchar_t* charToWChar(const char* text)
{
    const size_t size = strlen(text) + 1;
    wchar_t* wText = new wchar_t[size];
    mbstowcs(wText, text, size);
    return wText;
}

void decodingwin(char *text)
{

    return; //TEMPORARY
}

void decodingwin_true(char *text)
{

   return; //TEMPORARY
}

void decodingdxf(char *text)
{

    return; //TEMPORARY
}

#else
void decodingwin(char *text)
{
	int ret;
	char unicodetext[MaxMultitextLen * 2];

	ret = win2unicode(text, (char *)&unicodetext);
	unicode2utf8(unicodetext, (unsigned char *)text);

}

void decodingwin_true(char* text)
{
    int ret;
    char unicodetext[MaxMultitextLen * 2];

    ret = win2unicode(text, (char*)&unicodetext);
    unicode2utf8(unicodetext, (unsigned char*)text);
}


void decodingdxf(char *text)
{
	int ret;
	char unicodetext[MaxMultitextLen * 2];
    std::string cp_text(text);

    /*
    if (strcmp(DWGCODEPAGE,"ANSI_1251")==0)
    {
        utfstr=cp2utf(cp_text);
        const char *cstr = utfstr.c_str();
        strcpy(text, cstr);
    }
    */

	ret = win2unicodedxf(text, (char *)&unicodetext);
	unicode2utf8(unicodetext, (unsigned char *)text);

}

#endif

static BOOL add_block(double x, double y, char kod_obiektu, char *blok_type0, BOOL set_obiektt2, double angle)
/*----------------------------------------------------------------------------------------------------------*/
{
	unsigned size_block = B3;
	BLOK s_blockd = Bdef;
#ifndef LINUX
	//BLOKD blokd = BDdef;
    BLOK blokd = Bdef;
#else
    BLOK blokd = Bdef;
#endif
	BLOK *ptrs_block, *buf_block;
	int  len_type, len_desc;
	T_Desc_Ex_Block 	*ptrs_desc_bl;
	char blok_type[61];
	float kat;


	kat = angle;

	strcpy(blok_type, blok_type0);

    len_type = (int) strlen(blok_type) + 1;
    if ((kod_obiektu == B_DIM) || (kod_obiektu == B_DIM1) || (kod_obiektu == B_DIM2) || (kod_obiektu == B_DIM3))
        len_type += sizeof(float);  //added angle float
    len_desc = sizeof(unsigned) + 2 * sizeof(float) + sizeof(len_type) + len_type;
    size_block += len_desc;

	if (NULL == (buf_block = (BLOK*)malloc /*getmem*/(sizeof(NAGLOWEK) + size_block)))
	{
		return FALSE;
	}
	memcpy(buf_block, &blokd, sizeof(blokd));
	buf_block->n = size_block;
	buf_block->kod_obiektu = kod_obiektu;
	buf_block->dlugosc_opisu_obiektu = len_desc;
	if (set_obiektt2) buf_block->obiektt2 = O2BlockDXF;  /*dla blokow pierwotnych*/

    ptrs_desc_bl = (T_Desc_Ex_Block *) (&buf_block->opis_obiektu[0]);
    ptrs_desc_bl->flags = EBF_IP;
    ptrs_desc_bl->x = x;
    ptrs_desc_bl->y = y;
    ptrs_desc_bl->len = len_type;
    strcpy(&ptrs_desc_bl->sz_type[0], blok_type);

	if ((kod_obiektu == B_DIM) || (kod_obiektu == B_DIM1) || (kod_obiektu == B_DIM2) || (kod_obiektu == B_DIM3))
	{
		memmove((char *)(buf_block->opis_obiektu + buf_block->dlugosc_opisu_obiektu - sizeof(float)), &kat, sizeof(float));
	}

	if ((ptrs_block = (BLOK*)dodaj_obiekt(NULL, buf_block)) == NULL)
	{
        free(buf_block);
		return FALSE;
	}

    free(buf_block);
  return TRUE ;
}

static BOOL add_block_pline (void)
/*------------------------------*/
{
#ifndef LINUX
  BLOKD s_blockd = BDdef ;
#else
    BLOK s_blockd = Bdef ;
#endif
  BLOK *ptrs_block ;
  char c_pltype ;

  s_blockd.n = B3 + sizeof (c_pltype) ;
  s_blockd.kod_obiektu = B_PLINE ;
  s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype) ;
  if( (ptrs_block = (BLOK*)dodaj_obiekt (NULL, &s_blockd)) == NULL)
  {
    return FALSE ;
  }
  c_pltype = PL_PLINE ;
  memmove ( &(ptrs_block->opis_obiektu [0]), &c_pltype, sizeof(c_pltype)) ;
  return TRUE ;
}

static BOOL add_block_in_block (char kod_obiektu, char c_pltype0)
/*--------------------------------------------------------------*/
{
#ifndef LINUX
  BLOKD s_blockd = BDdef ;
#else
    BLOK s_blockd = Bdef ;
#endif
  BLOK *ptrs_block ;
  char c_pltype ;

  s_blockd.n = B3 + sizeof (c_pltype) ;
  s_blockd.kod_obiektu = kod_obiektu ;
  s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype) ;
  s_blockd.blok=1;
  if( (ptrs_block = (BLOK*)dodaj_obiekt ((BLOK*)dane, &s_blockd)) == NULL)
  {
    return FALSE ;
  }
  c_pltype = c_pltype0;
  memmove ( &(ptrs_block->opis_obiektu [0]), &c_pltype, sizeof(c_pltype)) ;
  global_block=(char *) ptrs_block - dane;
  return TRUE ;
}

static BOOL add_block_pline_in_block(void)
/*--------------------------------------*/
{
#ifndef LINUX
	BLOKD s_blockd = BDdef;
#else
    BLOK s_blockd = Bdef;
#endif
	BLOK *ptrs_block;
	char c_pltype;

	s_blockd.n = B3 + sizeof(c_pltype);
	s_blockd.kod_obiektu = B_PLINE;
	s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype);
	s_blockd.blok = 1;
	if ((ptrs_block = (BLOK*)dodaj_obiekt((BLOK*)dane, &s_blockd)) == NULL)
	{
		return FALSE;
	}
	c_pltype = PL_PLINE;
	memmove(&(ptrs_block->opis_obiektu[0]), &c_pltype, sizeof(c_pltype));
	global_block = (char *)ptrs_block - dane;
	return TRUE;
}

unsigned short acad_cyr_tab[70]={L'¸',L'¨',L'ą',L'é',L'ö',L'ó',L'ę',L'ĺ',L'í',L'ă',L'°',L'ů',L'ç',L'ő',L'ú',L'É',L'Ö',L'Ó',L'Ę',L'Ĺ',L'Í',L'Ă',L'Ř',L'҂',L'Ç',L'Ő',L'֏',L'ô',L'ű',L'â',L'ŕ',L'ď',L'đ',L'î',L'ë',L'ä',L'ć', L'ý',L'Ô',L'֍',L'Â',L'Ŕ',L'Ď',L'Đ',L'Î',L'Ë',L'Ä',L'Ć',L'Ý',L'˙',L'÷',L'ń',L'ě',L'č',L'ň',L'ü',L'á',L'ţ',L'.',L'ß',L'×',L'Ń',L'Ě',L'Č',L'Ň',L'Ţ',L'Á',L'Ţ',L',',L' ',};
char  mazovia_cyr_tab[70]={'`','~','#','q','w','e','r','t','y','u','i','o','p','[',']','Q','W','E','R','T','Y','U','I','O','P','{','}','a','s','d','f','g','h','j','k','l',';','\'','A','S','D','F','G','H','J','K','L',':','"','z','x','c','v','b','n','m',',','.','/','Z','X','C','V','B','N','M','<','>','?','^'};


void mazovia_cyr2acad_cyr (char * tekst)
/*-----------------------------*/
{ int i,j, dl_tekst;
  BOOL found_char;

  dl_tekst=(int)strlen(tekst);
  if (dl_tekst==0) return;
  for (i=0;i<dl_tekst; i++)
   {
    found_char=FALSE;
    j=0;
    while ((j<70) && (found_char==FALSE))
     {
       if (tekst[i]==mazovia_cyr_tab[j])
        {
          found_char=TRUE;
          tekst[i]=acad_cyr_tab[j];
        }
       j++;
     }
   }
  return;
}

void acad_cyr2mazovia_cyr (char * tekst)
/*------------------------------------*/
{ int i,j, dl_tekst;
  BOOL found_char;

  dl_tekst=(int)strlen(tekst);
  if (dl_tekst==0) return;
  for (i=0;i<dl_tekst; i++)
   {
    found_char=FALSE;
    j=0;
    while ((j<69) && (found_char==FALSE))
     {
       if (tekst[i]==acad_cyr_tab[j])
        {
          found_char=TRUE;
          tekst[i]=mazovia_cyr_tab[j];
        }
       j++;
     }
   }
  return;
}

void acad2mazovia (char * tekst)
/*-----------------------------*/
{ int i, dl_tekst;
  dl_tekst=(int)strlen(tekst);
  if (dl_tekst==0) return;
  for (i=0;i<dl_tekst; i++)
   {
    switch (tekst[i])
    {
    case '\360':tekst[i]='\361';
             break;
    case '\271':tekst[i]='\206';
             break;
    case '\346':tekst[i]='\215';
             break;
    case '\352':tekst[i]='\221';
             break;
    case '\263':tekst[i]='\222';
             break;
    case '\361':tekst[i]='\244';
             break;
    case '\363':tekst[i]='\242';
             break;
    case '\234':tekst[i]='\236';
             break;
    case '\237':tekst[i]='\246';
             break;
    case '\277':tekst[i]='\247';
             break;
    case '\245':tekst[i]='\217';
             break;
    case '\306':tekst[i]='\225';
             break;
    case '\312':tekst[i]='\220';
             break;
    case '\243':tekst[i]='\234';
             break;
    case '\321':tekst[i]='\245';
             break;
    case '\323':tekst[i]='\243';
             break;
    case '\214':tekst[i]='\230';
             break;
    case '\217':tekst[i]='\240';
             break;
    case '\257':tekst[i]='\241';
             break;
    case '\261':tekst[i]='\361';  //ń
             break;
    default: break;             
    }
   }
}

char *unicode2mazovia(char *unicode)
{
	int ucode;
	
	ucode = (int)strtol(unicode, NULL, 0);
	switch (ucode)
	{
	case 0x00b0:
		return "\370";
		break;
	case 0x00b1:
		return "\361";
		break;
	case 0x00b2:
		return "\375";
	case 0x00b3:
		return "\374";
		break;
	case 0x00bc:
		return "\254";
		break;
	case 0x00bd:
		return "\253";
		break;
	case 0x00d3:
		return "\243";
		break;
	case 0x00f3:
		return "\242";
		break;
	case 0x0104:
		return "\217";
		break;
	case 0x0105:
		return "\206";
		break;
	case 0x0106:
		return "\225";
		break;
	case 0x0107:
		return "\215";
		break;
	case 0x0118:
		return "\220";
		break;
	case 0x0119:
		return "\221";
		break;
	case 0x0141:
		return "\234";
		break;
	case 0x0142:
		return "\222";
		break;
	case 0x0143:
		return "\245";
		break;
	case 0x0144:
		return "\244";
		break;
	case 0x015a:
		return "\230";
		break;
	case 0x015b:
		return "\236";
		break;
	case 0x0179:
		return "\240";
		break;
	case 0x017a:
		return "\247";
		break;
	case 0x017b:
		return "\241";
		break;
	case 0x017c:
		return "\246";
		break;
	case 0x03a3:
		return "\344";
		break;
	case 0x03a9:
		return "\352";
		break;
	case 0x03b1:
		return "\340";
		break;
	case 0x03b2:
		return "\341";
		break;
	case 0x03b4:
		return "\353";
		break;
	case 0x03bc:
		return "\346";
		break;
	case 0x03c0:
		return "\343";
		break;
	case 0x03c3:
		return "\354";
		break;
	case 0x03c4:
		return "\347";
		break;
	case 0x2205:
		return "\350";
		break;
	case 0x220a:
		return "\356";
		break;
	case 0x2211:
		return "\344";
		break;
	case 0x2213:
		return "\361";
		break;
	case 0x221a:
		return "\373";
		break;
	case 0x2248:
		return "\367";
		break;
	case 0x2264:
		return "\363";
		break;
	case 0x2265:
		return "\362";
		break;
	default:
		return " ";
		break;
	}
}

void unicode2utf8char(char *unicode, unsigned short *utf8)
{
	int ucode;
	unsigned char utf8c[4];
	int bytes_n;
	
	ucode = (int)strtol(unicode, NULL, 0);

	bytes_n = ucs2_to_utf8(ucode, utf8c);

	if ((ucode > 1920) || (bytes_n > 2))
	{	
		*utf8= 32; 
	}
	else
	{
		*utf8 = (utf8c[1] << 8) + utf8c[0];
	}
}


void special_chars (char * tekst)
/*-----------------------------*/
{
  int dl_tekst;
  char *adr_char;
  int i_char;

  dl_tekst=(int)strlen(tekst);
  if (dl_tekst==0) return;

  while (1)
   {
    i_char=0;
	//polskie znaki
	adr_char=strstr(tekst,"%%165");  //A
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\245';  
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%198");  //C
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\306';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%202");  //E
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\312';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%163");  //L
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\243';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%209");  //N
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\321';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%211");  //O
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\323';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%140");  //S
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\314';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%175");  //Z
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\257';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%143");  //Z'
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\217';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }

	 adr_char=strstr(tekst,"%%185");  //a
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\271';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%230");  //c
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\346';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%234");  //e
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\352';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%179");  //l
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\263';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%241");  //n
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\361';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%243");  //o
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\363';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%156");  //s
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\234';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%191");  //Z
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\277';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 adr_char=strstr(tekst,"%%159");  //Z'
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\237';
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
	 //////////////////////////////
	 adr_char=strstr(tekst,"%%177");  //+-'
	 if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\261';  
       i_char++;
       memmove(adr_char+1,adr_char+5,dl_tekst-(adr_char-tekst)-4);
       dl_tekst-=4;
	 }
    //////////////////
    //stopnie
    adr_char=strstr(tekst,"%%d");
    if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\260';
       i_char++;
       memmove(adr_char+1,adr_char+3,dl_tekst-(adr_char-tekst)-2);
       dl_tekst-=2;
     }
	adr_char = strstr(tekst, "%%D");
	if (adr_char != NULL)
	{
		tekst[adr_char - tekst] = '\260';
		i_char++;
		memmove(adr_char + 1, adr_char + 3, dl_tekst - (adr_char - tekst) - 2);
		dl_tekst -= 3;
	}
    //plus minus
    adr_char=strstr(tekst, "%%p");
    if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\261'; 
       i_char++;
       memmove(adr_char+1,adr_char+3,dl_tekst-(adr_char-tekst)-2);
       dl_tekst-=2;
     }
    //fi
    adr_char=strstr(tekst, "%%c");
    if (adr_char!=NULL)
     {
       tekst[adr_char-tekst]='\330';
       i_char++;
       memmove(adr_char+1,adr_char+3,dl_tekst-(adr_char-tekst)-2);
       dl_tekst-=2;
     }
    //procent
    adr_char=strstr(tekst, "%%%");
    if (adr_char!=NULL)
     {
	   tekst[adr_char - tekst] = '%';
       i_char++;
       memmove(adr_char+1,adr_char+3,dl_tekst-(adr_char-tekst)-2);
       dl_tekst-=2;
     }
    //nadkreslenie - chwilowo usuniete
    adr_char=strstr(tekst, "%%o");
    if (adr_char!=NULL)
     {
       i_char++;
       memmove(adr_char,adr_char+3,dl_tekst-(adr_char-tekst)-1);
       dl_tekst-=3;
     }
     
    //podkreslenie u - chwilowo usuniete
    adr_char=strstr(tekst, "%%u");
    if (adr_char!=NULL)
     {
       i_char++;
       memmove(adr_char,adr_char+3,dl_tekst-(adr_char-tekst)-1);
       dl_tekst-=3;
     }

     //podkreslenie U - chwilowo usuniete
    adr_char=strstr(tekst, "%%U");
    if (adr_char!=NULL)
     {
       i_char++;
       memmove(adr_char,adr_char+3,dl_tekst-(adr_char-tekst)-1);
       dl_tekst-=3;
     }
     
    if (i_char==0) break;
   }
  return;
}



void latin2mazovia (char * tekst)
/*-----------------------------*/
{ int i, dl_tekst;
  dl_tekst=(int)strlen(tekst);
  if (dl_tekst==0) return;
  for (i=0;i<dl_tekst; i++)
   {
    switch (tekst[i])
    {
    case '\360':tekst[i]='\361';
             break;
    case '\245':tekst[i]='\206'; //a
             break;
    case '\206':tekst[i]='\215';  //c
             break;
    case '\251':tekst[i]='\221';  //e
             break;
    case '\210':tekst[i]='\222';  //l
             break;
    case '\344':tekst[i]='\244';  //n
             break;
    case '\242':tekst[i]='\242';  //o
             break;
    case '\230':tekst[i]='\236';  //s
             break;
    case '\253':tekst[i]='\246';  //z'
             break;
    case '\276':tekst[i]='\247';  //z.
             break;
    case '\244':tekst[i]='\217';  //A
             break;
    case '\217':tekst[i]='\225';  //C
             break;
    case '\250':tekst[i]='\220';  //E
             break;
    case '\235':tekst[i]='\234';  //L
             break;
    case '\343':tekst[i]='\245';  //N
             break;
    case '\340':tekst[i]='\243';  //O
             break;
    case '\227':tekst[i]='\230';  //S
             break;
    case '\215':tekst[i]='\240';  //Z'
             break;
    case '\275':tekst[i]='\241';  //Z.
             break;
    default: break;             
    }
   }
}

void latin2utf8(char * tekst)  //DOS Latin II
/*-----------------------------*/
{
	int i, dl_tekst;
	unsigned int int_code;
	char ubuf[MAXLINE];
	char lo, hi;
	uint8_t utf8c[4];
	int bytes_n;

	strcpy(ubuf, "");

	dl_tekst = (int)strlen(tekst);
	if (dl_tekst == 0) return;

	for (i = 0; i < dl_tekst; i++)
	{
		if ((unsigned char)tekst[i] < 127) strncat(ubuf, &tekst[i], 1);
		else
		{
			switch (tekst[i])
			{
			case '\360':int_code = 0xB1; //// plus minus
				break;
			case '\245':int_code = 261;  ////a  
				break;
			case '\206':int_code = 263;  ////c
				break;
			case '\251':int_code = 281;  ////e
				break;
			case '\210':int_code = 322;  ////l
				break;
			case '\344':int_code = 324;  ////n
				break;
			case '\242':int_code = 243;  ////o
				break;
			case '\230':int_code = 347;  ////s
				break;
			case '\253':int_code = 378;  ////z.
				break;
			case '\276':int_code = 380;  ////z'
				break;
			case '\244':int_code = 260;  ////A
				break;
			case '\217':int_code = 262;  ////C
				break;
			case '\250':int_code = 280;  ////E
				break;
			case '\235':int_code = 321;  ////L
				break;
			case '\343':int_code = 323;  ////N
				break;
			case '\340':int_code = 211;  ////O
				break;
			case '\227':int_code = 346;  ////S
				break;
			case '\215':int_code = 377;  ////Z'
				break;
			case '\275':int_code = 379;  ////Z.
				break;
			default:
				if (tekst[i] < 127) int_code = tekst[i];
				else int_code = 32;
				break;
			}

			//we got Unicode, now we need to convert to UTF8

			bytes_n = ucs2_to_utf8(int_code, utf8c);

			if ((int_code > 1920) || (bytes_n > 2))
			{
				strncat(ubuf, " ", 1);
			}
			else
			{

				lo = utf8c[0];
				hi = utf8c[1];
				strncat(ubuf, &lo, 1);
				strncat(ubuf, &hi, 1);
			}
		}
	}
	strcpy(tekst, ubuf);
}

void ISOlatin2utf8(char * tekst)
/*-----------------------------*/
{
	int i, dl_tekst;
	unsigned int int_code;
	char ubuf[MAXLINE];
	char lo, hi;
	uint8_t utf8c[4];
	int bytes_n;

	strcpy(ubuf, "");

	dl_tekst = (int)strlen(tekst);
	if (dl_tekst == 0) return;
	for (i = 0; i < dl_tekst; i++)
	{
		if ((unsigned char)tekst[i]<127) strncat(ubuf, &tekst[i], 1);
		else
		{
			switch (tekst[i])
			{
			case '\360':int_code = 0xB1; //// plus minus
				break;
			case '\261':int_code = 261;  ////a  
				break;
			case '\346':int_code = 263;  ////c
				break;
			case '\352':int_code = 281;  ////e
				break;
			case '\263':int_code = 322;  ////l
				break;
			case '\361':int_code = 324;  ////n
				break;
			case '\363':int_code = 243;  ////o
				break;
			case '\266':int_code = 347;  ////s
				break;
			case '\274':int_code = 378;  ////z'
				break;
			case '\277':int_code = 380;  ////z.
				break;
			case '\241':int_code = 260;  ////A
				break;
			case '\306':int_code = 262;  ////C
				break;
			case '\312':int_code = 280;  ////E
				break;
			case '\243':int_code = 321;  ////L
				break;
			case '\321':int_code = 323;  ////N
				break;
			case '\323':int_code = 211;  ////O
				break;
			case '\246':int_code = 346;  ////S
				break;
			case '\254':int_code = 377;  ////Z'
				break;
			case '\257':int_code = 379;  ////Z.
				break;
			default:
				if (tekst[i] < 127) int_code = tekst[i];
				else int_code = 32;
				break;
			}

			//we got Unicode, now we need to convert to UTF8
			bytes_n = ucs2_to_utf8(int_code, utf8c);

			if ((int_code > 1920) || (bytes_n > 2))
			{
				strncat(ubuf, " ", 1);
			}
			else
			{

				lo = utf8c[0];
				hi = utf8c[1];
				strncat(ubuf, &lo, 1);
				strncat(ubuf, &hi, 1);
			}

		}
	}
	strcpy(tekst, ubuf);
}

int read_var1(FILE *f,int kod, void * ZMIENNA)
{
  char buf[MAXLINE], *p;
  char dxf_code[MAXLINE];
   
  if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_code , MAXLINE , f ) == NULL ) return 0;
   if (!inc_nr_linii()) return 0;
    if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

if (((kod>=0) && (kod<10)) ||
    ((kod>=999) && (kod<1010))) /*string*/
 {
  strcpy((char *)ZMIENNA, buf);
  return 1;
 }
 else if (((kod>=10) && (kod<60)) ||
          ((kod>=140) && (kod<148)) ||
          ((kod>=210) && (kod<40)) ||
          ((kod>=1010) && (kod<1060))) /*double*/
  {
    p=buf;
    if ( sscanf ( p , "%lf", ZMIENNA)  < 1 ) return 0;
  }    
  else if (((kod>=60) && (kod<80)) ||
          ((kod>=170) && (kod<176)) ||
          ((kod>=1060) && (kod<1080))) /*int*/
  {
    p=buf;
    if ( sscanf ( p , "%d", ZMIENNA)  < 1 ) return 0;
  }                
 return 1; 
}


int read_var2(FILE *f,int kod1, void *ZMIENNA1, int kod2, void  *ZMIENNA2)
{
  char buf[MAXLINE], *p;
  char dxf_code[MAXLINE];

  if (!inc_nr_linii()) return 0;
 if ( myfgets(dxf_code , MAXLINE , f ) == NULL ) return 0;
 if (!inc_nr_linii()) return 0;
 if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
    
if (((kod1>=0) && (kod1<10)) ||
    ((kod1>=999) && (kod1<1010))) /*string*/
 {
  strcpy((char *)ZMIENNA1, buf);
  return 1;
 }
 else if (((kod1>=10) && (kod1<60)) ||
          ((kod1>=140) && (kod1<148)) ||
          ((kod1>=210) && (kod1<40)) ||
          ((kod1>=1010) && (kod1<1060))) /*double*/
  {
    p=buf;
    if ( sscanf ( p , "%lf", ZMIENNA1)  < 1 ) return 0;
  }    
  else if (((kod1>=60) && (kod1<80)) ||
          ((kod1>=170) && (kod1<176)) ||
          ((kod1>=1060) && (kod1<1080))) /*int*/
  {
    p=buf;
    if ( sscanf ( p , "%d", ZMIENNA1)  < 1 ) return 0;
  }                
 /*2*/ 
if (!inc_nr_linii()) return 0;
  if ( myfgets(dxf_code , MAXLINE , f ) == NULL ) return 0;
  if (!inc_nr_linii()) return 0;
 if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
    
if (((kod2>=0) && (kod2<10)) ||
    ((kod2>=999) && (kod2<1010))) /*string*/
 {
  strcpy((char *)ZMIENNA2, buf);
  return 1;
 }
 else if (((kod2>=10) && (kod2<60)) ||
          ((kod2>=140) && (kod2<148)) ||
          ((kod2>=210) && (kod2<40)) ||
          ((kod2>=1010) && (kod2<1060))) /*double*/
  {
    p=buf;
    if ( sscanf ( p , "%lf", ZMIENNA2)  < 1 ) return 0;
  }    
  else if (((kod2>=60) && (kod2<80)) ||
          ((kod2>=170) && (kod2<176)) ||
          ((kod2>=1060) && (kod2<1080))) /*int*/
  {
    p=buf;
    if ( sscanf ( p , "%d", ZMIENNA2)  < 1 ) return 0;
  }    
  
 return 1; 
}

int read_var3(FILE *f,int kod1, void *ZMIENNA1, int kod2, void *ZMIENNA2, int kod3, void *ZMIENNA3)
{ 
  char buf[MAXLINE], *p;
  char dxf_code[MAXLINE];

  if (!inc_nr_linii()) return 0;
 if ( myfgets(dxf_code , MAXLINE , f ) == NULL ) return 0;
 if (!inc_nr_linii()) return 0;
 if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
    
if (((kod1>=0) && (kod1<10)) ||
    ((kod1>=999) && (kod1<1010))) /*string*/
 {
  strcpy((char *)ZMIENNA1, buf);
  return 1;
 }
 else if (((kod1>=10) && (kod1<60)) ||
          ((kod1>=140) && (kod1<148)) ||
          ((kod1>=210) && (kod1<40)) ||
          ((kod1>=1010) && (kod1<1060))) /*double*/
  {
    p=buf;
    if ( sscanf ( p , "%lf", ZMIENNA1)  < 1 ) return 0;
  }    
  else if (((kod1>=60) && (kod1<80)) ||
          ((kod1>=170) && (kod1<176)) ||
          ((kod1>=1060) && (kod1<1080))) /*int*/
  {
    p=buf;
    if ( sscanf ( p , "%d", ZMIENNA1)  < 1 ) return 0;
  }                
 /*2*/ 
if (!inc_nr_linii()) return 0;
  if ( myfgets(dxf_code , MAXLINE , f ) == NULL ) return 0;
  if (!inc_nr_linii()) return 0;
 if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
    
if (((kod2>=0) && (kod2<10)) ||
    ((kod2>=999) && (kod2<1010))) /*string*/
 {
  strcpy((char *)ZMIENNA2, buf);
  return 1;
 }
 else if (((kod2>=10) && (kod2<60)) ||
          ((kod2>=140) && (kod2<148)) ||
          ((kod2>=210) && (kod2<40)) ||
          ((kod2>=1010) && (kod2<1060))) /*double*/
  {
    p=buf;
    if ( sscanf ( p , "%lf", ZMIENNA2)  < 1 ) return 0;
  }    
  else if (((kod2>=60) && (kod2<80)) ||
          ((kod2>=170) && (kod2<176)) ||
          ((kod2>=1060) && (kod2<1080))) /*int*/
  {
    p=buf;
    if ( sscanf ( p , "%d", ZMIENNA2)  < 1 ) return 0;
  }    
 /*3*/
 /*na razie zrezygnowano*/
 return 1; 
}

/*prymitywy nalezy czytac az do wystapienia kodu "0"*/

int find_layer(char *buf1)
{ int ii;
  if (No_Layers==1) return 0;
  for (ii=0; ii<No_Layers; ii++)
   {
     if (strcmp(buf1,Layers_name[ii])==0)
      {
        return ii;
      }
   }
  return 0;   /*jezeli nie zostanie znaleziona nazwa warstwy,
                to obiekt naklada sie na warstwe 0*/ 
}

int find_layer_DXF(char *buf1)
{
	int ii;
	if (No_Layers_DXF == 1) return 0;
	for (ii = 0; ii < No_Layers_DXF; ii++)
	{
		if (strcmp(buf1, Layers_name_DXF[ii]) == 0)
		{
			return ii;
		}
	}
	return 0;   /*jezeli nie zostanie znaleziona nazwa warstwy,
				  to obiekt naklada sie na warstwe 0*/
}

int find_type(char *buf1, int e_type)
{ int ii;
  char *ptr;
  int return_var;
  int grubosc;


  grubosc = (e_type & 224 );  /*grubosc*/

  for (ii=0; ii<acad_line_nr; ii++)
   {
     if (strcmp(buf1,acad_line[ii])==0)
      {
       return_var=grubosc + acad_ltype[ii]-64;
       return return_var;
      }
   }
  for (ii=0; ii<alf_line_nr; ii++)
   {
     if (strcmp(buf1,alf_line_en[ii])==0)
      {
       return_var=grubosc + alf_ltype[ii]-64;
       return return_var;
      }
   } 
  for (ii = 0; ii < alf_line_nr; ii++)
  {
	  if (strcmp(buf1, alf_line_pl[ii]) == 0)
	  {
		  return_var = grubosc + alf_ltype[ii] - 64;
		  return return_var;
	  }
  }
  for (ii=0; ii<quasi_acad_line_nr; ii++)
   {
      ptr=strstr(buf1,quasi_acad_line[ii]);
      if (ptr!=NULL)
      {
       return_var=grubosc + quasi_acad_ltype[ii]-64;
       return return_var;
      }
   }  
  return_var=grubosc;
  return return_var;
}

void add_font_DXF(char *fontname)
{
	if (WhNumberTextStyleDXF < MaxNumberTextStyleDXF)
	{
		strcpy(PTRS__Text_Style_DXF[WhNumberTextStyleDXF].font_acad_name, fontname);
		strcpy(PTRS__Text_Style_DXF[WhNumberTextStyleDXF].font_alf_name, fontname);
		PTRS__Text_Style_DXF[WhNumberTextStyleDXF].df_width_factor = 1.0;
		WhNumberTextStyleDXF++;
	}
}


int find_font_DXF(char *buf1, double *e_wspx, int type)
{ int ii, iii;
  char buf01[64];
  char font_alf_name[64];
  char font_acad_name[64];
  char font_name[64];

  /*nalezy szukac wsrod PTRS__Text_Style_DXF->font_acad_name */
  strcpy(buf01, buf1);
  strupr(buf01);

  *e_wspx=1;
  if (WhNumberTextStyleDXF==0) return -1;
  for (ii = 0; ii < WhNumberTextStyleDXF; ii++)
   {
	 strcpy(font_acad_name, PTRS__Text_Style_DXF[ii].font_acad_name);
	 strupr(font_acad_name);
	 strcpy(font_alf_name, PTRS__Text_Style_DXF[ii].font_alf_name);
	 strupr(font_alf_name);

     if (strcmp(buf01,font_acad_name)==0)
      {
       /*poszukiwanie numeru czcionki*/
       if (WhNumberTextStyle==0) return -1;
       for (iii = 0; iii < WhNumberTextStyle; iii++)
        {
		 strcpy(font_name, PTRS__Text_Style[iii]->font_name);
		 strupr(font_name);
         if ((strcmp(font_alf_name,font_name)==0) && ((PTRS__Text_Style[iii]->type==type) || (type==-1)))
          {
            *e_wspx=PTRS__Text_Style_DXF[ii].df_width_factor;
            return iii;
          }
        }
      }  
   }
  return -1;  /*jezeli nie znalazl*/
}




int find_font_DXF_Alfa_name(char *stylefile, char *stylealfa)
{
    int ii; // , iii;
    char buf01[64];
    char font_acad_name[64];
    char font_name[64];

    strcpy(buf01, stylefile);
    strupr(buf01);

    if (WhNumberTextStyleDXF==0) return 0;
    for (ii = 0; ii < WhNumberTextStyleDXF; ii++)
    {
        strcpy(font_acad_name, PTRS__Text_Style_DXF[ii].font_acad_name);
        strupr(font_acad_name);
        strcpy(font_name, PTRS__Text_Style_DXF[ii].font_alf_name);
        strupr(font_name);

        if (strcmp(buf01,font_acad_name)==0)
        {
            strcpy(stylealfa, PTRS__Text_Style_DXF[ii].font_alf_name);
            return 1;
        }
    }
    return 0;  /*jezeli nie znalazl*/
}


unsigned char find_font(char *font_style, double *e_wspx, double *e_wspx0, BOOL e_wspx_ok,
                        double &e_h, BOOL e_h_ok, double &e_obk1, BOOL e_obk_ok)
{ int ii;
  double e_wspx1;
  int font_number0;
  char font_acad_name[64];
  char style_file[MAXLINE];
  char *ptr;
  char dir[MAXDIR];
  char drive[MAXDRIVE];
  char file[MAXFILE];
  char ext[MAXEXT];
  int flags;

  font_number0=0;

  strcpy(style_file, font_style);

  e_wspx1=*e_wspx0;

  font_number0 = find_font_ALFA(style_file, &e_wspx1, 3);
  if (font_number0 > -1)
  {
      *e_wspx0=s__Fonts_Style[font_number0].df_width_factor;
      return font_number0;
  }
  
  if (No_Styles_DXF==0) goto fontDXF;
  for (ii=0; ii<No_Styles_DXF; ii++)
    {
     if (strcmp(style_file,PTRS__Text_Style_DXF_TABLE[ii].font_acad_name)==0)
      {
        if (PTRS__Text_Style_DXF_TABLE[ii].fixed_height!=0)
         {
           if (e_h_ok==FALSE) e_h=PTRS__Text_Style_DXF_TABLE[ii].fixed_height;
         }
        if (e_wspx_ok==FALSE) *e_wspx = PTRS__Text_Style_DXF_TABLE[ii].width_factor;
		*e_wspx0 = PTRS__Text_Style_DXF_TABLE[ii].width_factor;
        if (PTRS__Text_Style_DXF_TABLE[ii].italic==1) e_obk1=1;
        return PTRS__Text_Style_DXF_TABLE[ii].font_number;
      }
    }

fontDXF:
  
	ptr = strstr(style_file, ".TTF");
	if (ptr != NULL)
	{
		flags = fnsplit(font_style, drive, dir, file, ext);
		font_number0 = find_font_DXF(file, &e_wspx1, 2);
		if (font_number0 == -1)  //not found
		{
			strcat(file, ext);
			font_number0 = Add_New_Font_TTF_OTF(file, 0, 1);
			if (font_number0 > -1)
			{
				add_font_DXF(file);
			}
		}
	}
	else  //all others including .shx
	{
		flags = fnsplit(style_file, drive, dir, file, ext);
		font_number0 = find_font_DXF(file, &e_wspx1, 1);
	}
  

	if (font_number0 > -1)
	{
		PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].font_number = font_number0;
		PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].width_factor *= e_wspx1;
	}
	else
		font_number0 = 0;

	return font_number0;

}

BOOL read_viewport(FILE *f,int to_block, BOOL block)
{
  return 1;
}
 
double kat_p(double x, double y)
 { double kat0;
   if (fabs(x)<epsilon) 
     {
      if (y>0) kat0=0.25*Pi2;
        else kat0=0.75*Pi2;
     }
      else
        {
	  kat0=atan((y)/(x));
	  if ((x<0) && (y>=0)) kat0=kat0+(Pi2/2);
	   else if ((x<0) && (y<0)) kat0=kat0+(Pi2/2);
	    else if ((x>0) && (y<0)) kat0=kat0+(Pi2/2);
	}
  return kat0;	
 } 


unsigned char get_typ(unsigned char eep_type, double eep_width)
 { int typ_l;
   double width;
   int width_l;

   typ_l = (eep_type & 31);  /*typ*/

   width_l=0;
   width=eep_width/100;

   if (width < 0.0) width_l = 7;
   else if (width > (line_width.thick+0.01)) width_l=4;
   else if (width > (line_width.normal+0.01)) width_l=3;
   else if (width > (line_width.thin+0.01)) width_l=2;
   else if (width > (line_width.thinest+0.01)) width_l=1;
   else width_l = 0;

   typ_l += (width_l*32);

   return typ_l;
 }

BOOL check_width_7(char *p1, double *ep_width)
{
	if (strcmp(p1, "invisible") == 0) { *ep_width = -1.0; return TRUE; }
	else return FALSE;
}

BOOL check_angle(char *p1, double *angle)
{
	if (sscanf(p1, "%lf", angle) < 1) return FALSE;
	else return TRUE;
}

BOOL check_dim_line(char *p1, char *obiektt1, char *obiektt2, char *obiektt3)
{
	if (strcmp(p1, code999[0]) == 0) { *obiektt1 = 0; *obiektt2 = 1; *obiektt3 = 0; return TRUE; }
	else if (strcmp(p1, code999[2]) == 0) { *obiektt1 = 0; *obiektt2 = 1; *obiektt3 = 1; return TRUE; }
	else if (strcmp(p1, code999[3]) == 0) { *obiektt1 = 3; *obiektt2 = 1; *obiektt3 = 0; return TRUE; }
	else return FALSE;
}

BOOL check_dim_arc(char *p1, char *obiektt1, char *obiektt2, char *obiektt3)
{
	if (strcmp(p1, code999[1]) == 0) { *obiektt1 = 0; *obiektt2 = 1; *obiektt3 = 0; return TRUE; }
	else return FALSE;
}

BOOL check_dim_head(char *p1, char *obiektt1, char *obiektt2, char *obiektt3)  //kolo/ solid
{
	if (strcmp(p1, code999[3]) == 0) { *obiektt1 = 0; *obiektt2 = 1; *obiektt3 = 0; return TRUE; }
	else return FALSE;
}

BOOL check_dim_text(char *p1, char *obiektt1, char *obiektt2, char *obiektt3)  //text
{
	if (strcmp(p1, code999[4]) == 0) { *obiektt1 = 0; *obiektt2 = 1; *obiektt3 = 0; return TRUE; }
	else return FALSE;
}

BOOL get_width_invisible(unsigned char e_type)
{
	int width_l;
	width_l = (e_type & 224) / 32;
	if (width_l == 7) return TRUE;
	else return FALSE;
}

int get_width370(unsigned char e_type)
{
	int width;
	int width_l;


	width_l = (e_type & 224) / 32;

	if (width_l == 0) width = int(line_width.thinest*100.0 + 0.5);
	else if (width_l == 1) width = int(line_width.thin*100.0 + 0.5);
	else if (width_l == 2) width = int(line_width.normal*100.0 + 0.5);
	else if (width_l == 3) width = int(line_width.thick*100.0 + 0.5);
	else if (width_l == 4) width = int(line_width.thickest*100.0 + 0.5);

	else if (width_l == 5) width = int(line_width.normal * 100.0 + 0.5);
	else if (width_l == 6) width = int(line_width.normal * 100.0 + 0.5);
	else if (width_l == 7) width = int(line_width.normal * 100.0 + 0.5);  //invisible with code 999

	else width = int(line_width.normal*100.0 + 0.5);

	return width;
}

double get_width(unsigned char e_type)
 { double width;
   int width_l;


   width_l = (e_type & 224 ) / 32;

   if (width_l==0) width=line_width.thinest;
     else if (width_l==1) width=line_width.thin;
       else if (width_l==2) width=line_width.normal;
         else if (width_l==3) width=line_width.thick;
           else if (width_l==4) width=line_width.thickest;
              else width=line_width.normal;

   return (width*100);
 }

void pisz_line(double e_xp, double e_yp, double e_xk, double e_yk)
{  double e_xp1, e_yp1, e_xk1, e_yk1;
   LINIA L=Ldef;
   
    e_xp1=jednostkiOb(e_xp - EXTMIN_X);
    e_yp1=jednostkiOb(e_yp - EXTMIN_Y);
    e_xk1=jednostkiOb(e_xk - EXTMIN_X);
    e_yk1=jednostkiOb(e_yk - EXTMIN_Y);
    
    L.x1=e_xp1;
    L.y1=e_yp1;
    L.x2=e_xk1;
    L.y2=e_yk1;

    L.typ=get_typ(ep_type, ep_width);

    L.kolor=ep_color; 
    L.warstwa=ep_layer;
    L.przec=ep_space;
  
    if (NULL == dodaj_obiekt ( NULL, &L)) return ;
    
    return;

}

static void set_next_pline_solid (BOOL b_solid, BOOL b_end, int to_block1)
/*----------------------------------------------------------------------*/
{
  double dx1, dy1, dx2, dy2, df_l1 ;
  double angle_r, si_r, co_r, angle_b, si_b, co_b ;
  double df_sx1, df_sy1, df_sx2, df_sy2 ;

  dx1 = s_pline.line.x2 - s_pline.line.x1 ;
  dy1 = s_pline.line.y2 - s_pline.line.y1 ;
  df_l1 = sqrt (dx1 * dx1 + dy1 * dy1) ;
  angle_r = Atan2 (dy1, dx1) ;
  si_r = sin (angle_r) ;
  co_r = cos (angle_r) ;
  if ((FALSE == b_solid))
  {
    df_sx1 = 0 ;
    df_sy1 = s_pline.width1 / 2 ;
    df_sx2 = 0 ;
    df_sy2 = -s_pline.width1 / 2 ;
    obrd (si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1) ;
    obrd (si_r, co_r, df_sx2, df_sy2, &df_sx2, &df_sy2) ;
    s_pline.solid.xy [0] = df_sx1 + s_pline.line.x1 ;
    s_pline.solid.xy [1] = df_sy1 + s_pline.line.y1 ;
    s_pline.solid.xy [2] = df_sx2 + s_pline.line.x1 ;
    s_pline.solid.xy [3] = df_sy2 + s_pline.line.y1 ;
  }
  else
  {
    s_pline.solid.xy [0] = s_pline.solid.xy [6] ;
    s_pline.solid.xy [1] = s_pline.solid.xy [7] ;
    s_pline.solid.xy [2] = s_pline.solid.xy [4] ;
    s_pline.solid.xy [3] = s_pline.solid.xy [5] ;
  }
  if ((FALSE == b_end) && (wypuklosc0==0) && (wypuklosc == 0))
  { 
    dx2 = line_g.x2 - s_pline.line.x1 ;
    dy2 = line_g.y2 - s_pline.line.y1 ;
    obru (si_r, co_r, dx2, dy2, &dx2, &dy2) ;
    angle_b = Atan2 (dy2 - 0, dx2 - df_l1) ;
    if (angle_b > 0)
    {
      angle_b = (Pi2 / 2 - angle_b) / 2 + angle_b ;
    }
    else
    {
      angle_b = (Pi2 / 2 + angle_b) / 2 + Pi2 / 2 ;
    }
    si_b = sin (angle_b) ;
    co_b = cos (angle_b) ;
    if (TRUE == Check_if_Equal (si_b, 0))
    {
      df_sx1 = df_l1 ;
      df_sx2 = df_l1 ;
    }
    else
    {
      df_sx1 =  df_l1 - (co_b / si_b) * s_pline.width / 2 ;
      df_sx2 = df_l1 + (co_b / si_b) * s_pline.width / 2 ;
    }
  }
  else
  {
    df_sx1 = df_l1 ;
    df_sx2 = df_l1 ;
  }
  df_sy1 = - s_pline.width / 2 ;
  df_sy2 = - df_sy1 ;
  obrd (si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1) ;
  obrd (si_r, co_r, df_sx2, df_sy2, &df_sx2, &df_sy2) ;
  s_pline.solid.xy [4] = df_sx1 + s_pline.line.x1 ;
  s_pline.solid.xy [5] = df_sy1 + s_pline.line.y1 ;
  s_pline.solid.xy [6] = df_sx2 + s_pline.line.x1 ;
  s_pline.solid.xy [7] = df_sy2 + s_pline.line.y1 ;
  s_pline.solid.kolor = s_pline.color_line ;
  s_pline.solid.warstwa = ep_layer;
}


void zaznacz_blok(char *adr)
{ BLOK *b;
  BOOL again;
  NAGLOWEK *nag;
  char *adp, *adk, *adp1;

  b=(BLOK *)adr;
  zmien_atrybut(adr, adr + b->n + sizeof(NAGLOWEK) - 1, ANieOkreslony, Ablok);
  //eliminacja tekstow
  again=TRUE;
  adp=adr;
  adk=adp+b->n+sizeof(NAGLOWEK)-1;
  while ((adp<adk) && (again==TRUE))
   {
     adp1=find_obj(adp, adk, Otekst, ONieOkreslony, 0);
     if (adp1!=NULL)
      {
        nag=(NAGLOWEK *)adp1;
        if (nag->atrybut==Ablok) nag->atrybut=Anormalny;
        adp=adp1+nag->n+sizeof(NAGLOWEK);
      }
       else again=FALSE;
   }
  return;
}

void odznacz_blok(char *adr)
{ BLOK *b;
  b=(BLOK *)adr;
  zmien_atrybut(adr, adr+b->n+sizeof(NAGLOWEK), Ablok, ANieOkreslony);
  return;
}

	      
static int add_pline_line_end (int to_block, int e_extrusion_z, int atrybut)
/*------------------------------------------------------------------------*/
{
  int to_block1;
  BOOL b_ret = FALSE ;
  WIELOKAT solid_temp = S4def ;
  SOLIDARC sa=sadef;
  LINIA L=Ldef;
  LUK l=ldef;
  BLOK *b;
  char *base_adr;
  BLOK *base_block,*base_block1;
  long_long base_adr_dane;
  unsigned char translucency;
  char* translucency_ptr;
  float temp_x1, temp_y1, temp_x2, temp_y2;
  BOOL first_point;
  double l_r;

    L.atrybut=atrybut;
    l.atrybut=atrybut;

  to_block1 = to_block;
  if (to_block1==1)  //zagniezdzenie blokow
  {
     base_block=(BLOK *)dane;
	 base_adr=dane+sizeof(NAGLOWEK)+B3+base_block->dlugosc_opisu_obiektu;
	 base_block1=(BLOK *)base_adr;
	 if (base_block1->obiekt!=OdBLOK)  //brak potwierdzenie
		 base_adr=dane;  //na wszelki wypadek
  }
   else base_adr=dane;
  
  base_adr_dane=base_adr-dane;
  if ((s_pline.line.x1 == s_pline.line.x2) && (s_pline.line.y1 == s_pline.line.y2))
  {
    return 1 ;
  } 
  if (s_pline.b_line == TRUE)
  {
    s_pline.color_line=ep_color;
    s_pline.width1 = jednostkiOb (width_p) ;
    s_pline.width = jednostkiOb (width_k) ;
    set_next_pline_solid (s_pline.b_solid, TRUE, to_block1) ;
    s_pline.solid.lp= 8;
    s_pline.solid.n = 8 + s_pline.solid.lp * sizeof (float) ;
    s_pline.solid.blok= 1;
    s_pline.solid.przec=ep_space;
    memcpy (&solid_temp, &s_pline.solid, sizeof (solid_temp)) ;
    Normalize_Solid (&solid_temp) ;

	if (ep_translucency < 255)
	{
		translucency = (unsigned char)ep_translucency;
		solid_temp.translucent = 1;
		translucency_ptr = (char *)solid_temp.xy;
		translucency_ptr += (solid_temp.lp * sizeof(float));
		memmove(translucency_ptr,&translucency, sizeof(unsigned char));

		solid_temp.n = 8 + solid_temp.lp * sizeof(float) + sizeof(unsigned char);
	}
	else
	{
		solid_temp.translucent = 0;
	}

    solid_temp.warstwa=ep_layer;
    solid_temp.przec=ep_space;
    if ((s_pline.width1!=0) || (s_pline.width!=0))
     {
      if (s_pline.wypuklosc==0)
       {
        if (NULL == dodaj_obiekt ((BLOK*)base_adr, &solid_temp)) return 0;
       } 
       else
        {
          POLYARC(s_pline.line.x1, s_pline.line.y1, s_pline.line.x2, s_pline.line.y2, s_pline.wypuklosc, &l);

            temp_x2=l.x+l.r*cos(l.kat2);
            temp_y2=l.y+l.r*sin(l.kat2);

            if ((Check_if_Equal2(temp_x2, s_pline.line.x2)==TRUE) && (Check_if_Equal(temp_y2, s_pline.line.y2)==TRUE))
                first_point=FALSE;
            else if ((Check_if_Equal2(temp_x2, s_pline.line.x1)==TRUE) && (Check_if_Equal(temp_y2, s_pline.line.y1)==TRUE))
                first_point=TRUE;
            else
            {
                first_point=TRUE;
            }

          l.warstwa=ep_layer;
          l.obiektt2 = O2BlockPline;
          l.kolor=ep_color;

          l.typ=get_typ(ep_type, ep_width);
          l.przec=ep_space;
          l.blok=1;
          l_r=l.r + (s_pline.width/2);

            ////correction of solid
            if (first_point)
            {
                s_pline.solid.xy[6] = l.x + l_r * cos(l.kat1);
                s_pline.solid.xy[7] = l.y + l_r * sin(l.kat1);
            }
            else
            {
                s_pline.solid.xy[4] = l.x + l_r * cos(l.kat2);
                s_pline.solid.xy[5] = l.y + l_r * sin(l.kat2);
            }
            ////

            l_r=l.r - (s_pline.width/2);

            ////correction of solid
            if (first_point)
            {
                s_pline.solid.xy[4] = l.x + l_r * cos(l.kat1);
                s_pline.solid.xy[5] = l.y + l_r * sin(l.kat1);
            }
            else
            {
                s_pline.solid.xy[6] = l.x + l_r * cos(l.kat2);
                s_pline.solid.xy[7] = l.y + l_r * sin(l.kat2);
            }
            ////adding solidarc
            sa.warstwa=ep_layer;
            sa.obiektt2 = O2BlockPline;
            sa.kolor=ep_color;
            l.typ=get_typ(ep_type, ep_width);
            sa.przec=ep_space;
            sa.blok=1;
            if (ep_translucency < 255) {
                sa.translucent = 1;
                sa.translucency = ep_translucency;
            }
            else
            {
                sa.translucent = 0;
                sa.translucency = 255;
            }

            sa.x=l.x;
            sa.y=l.y;
            sa.r=l.r;
            sa.kat1=l.kat1;
            sa.kat2=l.kat2;

            if (first_point) {
                sa.width1 = s_pline.width1;
                sa.width2 = s_pline.width;
                sa.axis1=0.0;
                sa.axis2=0.0;
                sa.reversed=0;
            }
            else
            {
                sa.width1 = s_pline.width;
                sa.width2 = s_pline.width1;
                sa.axis1=0.0;
                sa.axis2=0.0;
                sa.reversed=1;
            }
            if (sa.r>0.00001)
            {
                if (NULL == dodaj_obiekt ((BLOK*)base_adr, &sa)) return 0;
                base_adr=dane+base_adr_dane;
            }
         } 
     }
     else
      {
       if (s_pline.wypuklosc==0)
        {
         L.x1=s_pline.line.x1;
         L.y1=s_pline.line.y1;
         L.x2=s_pline.line.x2;
         L.y2=s_pline.line.y2;  
         L.warstwa=ep_layer;
         L.obiektt2 = O2BlockPline;
         L.kolor=ep_color;
         L.typ=get_typ(ep_type, ep_width);
         L.przec=ep_space;
         L.blok=1;
         if (NULL == dodaj_obiekt ((BLOK*)base_adr, &L)) return 0;
        }
         else
         {
          POLYARC(s_pline.line.x1, s_pline.line.y1, s_pline.line.x2, s_pline.line.y2, s_pline.wypuklosc, &l);
          l.warstwa=ep_layer;
          l.obiektt2 = O2BlockPline;
          l.kolor=ep_color;
          l.typ=get_typ(ep_type, ep_width);
          l.blok=1;
          l.przec=ep_space;
          if (l.r>0.00001) {if (NULL == dodaj_obiekt ((BLOK*)base_adr, &l)) return 0;}
         }
          
      } 
       
  }
  if (Check_if_Equal(e_extrusion_z,-1.0)==TRUE)
   {
     //zaznaczenie bloku
	 base_adr=dane;
     b=(BLOK *)base_adr;
     zaznacz_blok(base_adr);  //to sie wali !!!!!!!!!!
     transformacja_blok (base_adr,base_adr+sizeof(NAGLOWEK)+b->n-1, jednostkiOb(- EXTMIN_X), 0, jednostkiOb(- EXTMIN_X), 100, Tmirror,0) ; //!!!!!!!!!!!!!!!!!!!!!
     odznacz_blok(base_adr);
   }
  return 1 ;
}

static int add_bspline(double xp[NumSplinePoints], double yp[NumSplinePoints], double zp[NumSplinePoints], int to_block, int lp, int spline_type, int zamkniecie, double bcurviness, int atrybut)
{
    SPLINE bspline = Splinedef;
    int i;

    bspline.atrybut=atrybut;

    bspline.kolor= ep_color;
    bspline.typ = get_typ(ep_type, ep_width);
    bspline.kolor = ep_color;
    bspline.warstwa = ep_layer;
    bspline.przec = ep_space;
    bspline.lp = lp*2;

    start_x=xp[0];
    start_y=yp[0];

    for (i = 0; i < (int)bspline.lp; i += 2)
    {
        bspline.xy[i] = jednostkiOb(xp[i / 2] - EXTMIN_X);
        bspline.xy[i + 1] = jednostkiOb(yp[i / 2] - EXTMIN_Y);
    }

    if (spline_type > 4)
    {
        bspline.npts = spline_type;
        bspline.closed = zamkniecie;
        bspline.xy[bspline.lp] = bcurviness;
        bspline.n = 8 + (bspline.lp +1 )* sizeof(float);
    }
    else
    {
        bspline.npts = spline_type;
        bspline.closed = 0;
        bspline.n = 8 + bspline.lp * sizeof(float);
    }

    bspline.obiekt = Ospline;

    bspline.temp1 = 0;

    if (to_block==1) {
        bspline.blok = ElemBlok;
        if (NULL == dodaj_obiekt((BLOK *) dane, &bspline)) return 0;
    }
    else
    {
        bspline.blok = NoElemBlok;
        if (NULL == dodaj_obiekt(NULL, &bspline)) return 0;
    }

    return 1;
}


static int add_bspline___(double xp[NumSplinePoints], double yp[NumSplinePoints], double zp[NumSplinePoints], int to_block, int lp, int spline_type, int zamkniecie, double bcurviness, int atrybut)
{
	SPLINE bspline = Splinedef;
	int to_block1;
	char *base_adr;
	BLOK *base_block, *base_block1;
	long_long base_adr_dane;
	int i;

	to_block1 = to_block;
	if (to_block1 == 1)  //zagniezdzenie blokow
	{
		base_block = (BLOK *)dane;
		base_adr = dane + sizeof(NAGLOWEK) + B3 + base_block->dlugosc_opisu_obiektu;
		base_block1 = (BLOK *)base_adr;
		if (base_block1->obiekt != OdBLOK)  //brak potwierdzenie
			base_adr = dane;  //na wszelki wypadek

	}
	else base_adr = dane;

	base_adr_dane = base_adr - dane;

    bspline.atrybut=atrybut;

	bspline.blok=1;
	bspline.kolor= ep_color;
	bspline.typ = get_typ(ep_type, ep_width);   
	bspline.kolor = ep_color;
	bspline.warstwa = ep_layer;
	bspline.przec = ep_space;
	bspline.lp = lp*2;

    start_x=xp[0];
    start_y=yp[0];

	for (i = 0; i < (int)bspline.lp; i += 2)
	{
		bspline.xy[i] = jednostkiOb(xp[i / 2] - EXTMIN_X);
		bspline.xy[i + 1] = jednostkiOb(yp[i / 2] - EXTMIN_Y);
	}

	if (spline_type > 4)
	{
		bspline.npts = spline_type;
		bspline.closed = zamkniecie;
		bspline.xy[bspline.lp] = bcurviness;
		bspline.n = 8 + (bspline.lp +1 )* sizeof(float);
	}
	else
	{
		bspline.npts = spline_type;
		bspline.closed = 0;
		bspline.n = 8 + bspline.lp * sizeof(float);
	}

	bspline.obiekt = Ospline;
	
	bspline.blok = ElemBlok;
	bspline.temp1 = 0;

	if (NULL == dodaj_obiekt((BLOK*)base_adr, &bspline)) return 0;
	base_adr = dane + base_adr_dane;

	return 1;
}

static int add_pline_line (double X_, double Y_, int to_block, int atrybut)
/*-----------------------------------------------------------------------*/
{
  int to_block1;
  BOOL b_ret = FALSE ;
  WIELOKAT solid_temp  = S4def ;
  SOLIDARC sa=sadef;
  LINIA L=Ldef;
  LUK l=ldef, l1=ldef;
  char *base_adr;
  BLOK *base_block1;
  long_long base_adr_dane;
  unsigned char translucency;
  char* translucency_ptr;
  float temp_x1, temp_y1, temp_x2, temp_y2;
  BOOL first_point;
  double l_r;

  L.atrybut=atrybut;
  l.atrybut=atrybut;

  to_block1 = to_block;
  if (to_block1==1)  //zagniezdzenie blokow
  {
     base_adr=dane+global_block;
	 base_block1=(BLOK *)base_adr;
	 if (base_block1->obiekt!=OdBLOK)  //brak potwierdzenie
		 base_adr=dane;  //na wszelki wypadek
  }
   else base_adr=dane;

  base_adr_dane=base_adr-dane;
  line_g.x2 = jednostkiOb(X_ - EXTMIN_X) ;
  line_g.y2 = jednostkiOb(Y_ - EXTMIN_Y) ;

  s_pline.wypuklosc = wypuklosc0;
  wypuklosc0 = wypuklosc;

  if (s_pline.wypuklosc!=0) s_pline.b_line = TRUE;  ////  !!!!

  if (s_pline.b_line == TRUE)
  {
    s_pline.solid.kolor=ep_color;
	if (ep_translucency < 255)
	{

	}
	else
	{
		s_pline.solid.translucent = 0;
	}
    set_next_pline_solid (s_pline.b_solid, FALSE, to_block1) ;
    s_pline.solid.blok=1;
    memcpy (&solid_temp, &s_pline.solid, sizeof (solid_temp)) ;
    Normalize_Solid (&solid_temp) ;
    solid_temp.warstwa=ep_layer;
    solid_temp.przec=ep_space;
    solid_temp.kolor=ep_color;

	if (ep_translucency < 255)
	{
		translucency = (unsigned char)ep_translucency;
		solid_temp.translucent = 1;
		translucency_ptr = (char*)solid_temp.xy;
		translucency_ptr += (solid_temp.lp * sizeof(float));
		memmove(translucency_ptr, &translucency, sizeof(unsigned char));

		solid_temp.n = 8 + solid_temp.lp * sizeof(float) + sizeof(unsigned char);
	}
	else
	{
		solid_temp.translucent = 0;
	}

    if ((s_pline.line.x1 != s_pline.line.x2) || (s_pline.line.y1 != s_pline.line.y2))
    {
    if ((s_pline.width1!=0) || (s_pline.width!=0))
     {
      if (s_pline.wypuklosc==0)
        {
         if (NULL == dodaj_obiekt ((BLOK*)base_adr, &solid_temp)) return 0;
        }
        else
         {
          POLYARC(s_pline.line.x1, s_pline.line.y1, s_pline.line.x2, s_pline.line.y2, s_pline.wypuklosc, &l);

         temp_x2=l.x+l.r*cos(l.kat2);
         temp_y2=l.y+l.r*sin(l.kat2);

         if ((Check_if_Equal2(temp_x2, s_pline.line.x2)==TRUE) && (Check_if_Equal(temp_y2, s_pline.line.y2)==TRUE))
             first_point=FALSE;
         else if ((Check_if_Equal2(temp_x2, s_pline.line.x1)==TRUE) && (Check_if_Equal(temp_y2, s_pline.line.y1)==TRUE))
            first_point=TRUE;
         else
         {
             first_point=TRUE;
         }

          //// fake arc, replaced by solidarc
          l.warstwa=ep_layer;
		  l.obiektt2 = O2BlockPline;
          l.kolor=ep_color;
          l.typ=get_typ(ep_type, ep_width);
          l.przec=ep_space;
          l.blok=1;
          l_r=l.r + (s_pline.width/2);

          ////correction of solid
          if (first_point)
             {
                 s_pline.solid.xy[6] = l.x + l_r * cos(l.kat1);
                 s_pline.solid.xy[7] = l.y + l_r * sin(l.kat1);
             }
          else
          {
              s_pline.solid.xy[4] = l.x + l_r * cos(l.kat2);
              s_pline.solid.xy[5] = l.y + l_r * sin(l.kat2);
          }
          ////
          l_r=l.r - (s_pline.width/2);
         ////correction of solid
             if (first_point)
             {
                 s_pline.solid.xy[4] = l.x + l_r * cos(l.kat1);
                 s_pline.solid.xy[5] = l.y + l_r * sin(l.kat1);
             }
             else
             {
                 s_pline.solid.xy[6] = l.x + l_r * cos(l.kat2);
                 s_pline.solid.xy[7] = l.y + l_r * sin(l.kat2);
             }
           ////adding solidarc
             sa.warstwa=ep_layer;
             sa.obiektt2 = O2BlockPline;
             sa.kolor=ep_color;
             //l.typ=ep_type;
             l.typ=get_typ(ep_type, ep_width);
             sa.przec=ep_space;
             sa.blok=1;
             if (ep_translucency < 255) {
                 sa.translucent = 1;
                 sa.translucency = ep_translucency;
             }
             else
             {
                 sa.translucent = 0;
                 sa.translucency = 255;
             }

             sa.x=l.x;
             sa.y=l.y;
             sa.r=l.r;
             sa.kat1=l.kat1;
             sa.kat2=l.kat2;

             if (first_point) {
                 sa.width1 = s_pline.width1;
                 sa.width2 = s_pline.width;
                 sa.axis1=0.0;
                 sa.axis2=0.0;
                 sa.reversed=0;
             }
             else
             {
                 sa.width1 = s_pline.width;
                 sa.width2 = s_pline.width1;
                 sa.axis1=0.0;
                 sa.axis2=0.0;
                 sa.reversed=1;
             }
             if ((sa.r>0.00001) && (atrybut!=Ausuniety))
             {
                 if (NULL == dodaj_obiekt ((BLOK*)base_adr, &sa)) return 0;
                 base_adr=dane+base_adr_dane;
             }

         } 
     } 
      else
      {
       if (s_pline.wypuklosc==0)
        {
         L.x1=s_pline.line.x1;
         L.y1=s_pline.line.y1;
         L.x2=s_pline.line.x2;
         L.y2=s_pline.line.y2;
         L.warstwa=ep_layer;
         L.obiektt2 = O2BlockPline;
         L.kolor=ep_color;
         //L.typ=ep_type;
         L.typ=get_typ(ep_type, ep_width);
         L.przec=ep_space;
         L.blok=1;
         if (atrybut!=Ausuniety) {
             if (NULL == dodaj_obiekt((BLOK *) base_adr, &L)) return 0;
         }
        }
        else
         {
          POLYARC(s_pline.line.x1, s_pline.line.y1, s_pline.line.x2, s_pline.line.y2, s_pline.wypuklosc, &l);
          l.warstwa=ep_layer;
		  l.obiektt2 = O2BlockPline;
          l.kolor=ep_color;
          l.typ=get_typ(ep_type, ep_width);
          l.przec=ep_space;
          l.blok=1;
          if (atrybut!=Ausuniety) {
                 if (l.r > 0.00001) { if (NULL == dodaj_obiekt((BLOK *) base_adr, &l)) return 0; }
             }
         }
          
      } 
    }  
    s_pline.b_solid = TRUE ;

    /*ustalenie nowych wartosci szerokosci polilinii*/

	s_pline.width1 = jednostkiOb(width_p);
	s_pline.width = jednostkiOb(width_k);
  }
  memcpy (&s_pline.line, &line_g, sizeof(line_g)) ;
  width_p = width_p0;
  width_k = width_k0;
  s_pline.wypuklosc = wypuklosc0;
  wypuklosc0 = wypuklosc;
  s_pline.color_line = line_g.kolor ;
  line_g_x=line_g.x1;
  line_g_y=line_g.y1;
  line_g.x1=line_g.x2;
  line_g.y1=line_g.y2;
  s_pline.b_line = TRUE ;
  return 1 ;
}


void start_pline (double X_, double Y_, int to_block1)
/*--------------------------------------------------*/
{ WIELOKAT W = S4def;

  memcpy (&s_pline.solid, &W, sizeof(W)) ;
  start_x = X_;
  start_y = Y_;
  line_g.x1 = jednostkiOb (X_ - EXTMIN_X) ;
  line_g.y1 = jednostkiOb (Y_ - EXTMIN_Y) ;
	  if (to_block1 == 0)
	  {
		  if (FALSE == add_block_pline())
		  {
			  return;
		  }
	  }
	  else
	  {
		  if (FALSE == add_block_pline_in_block())  //!!!!!!!!!!!!!!!!!
		  {
			  return;
		  }
	  }
  width_p = width_p0;
  width_k = width_k0;
  s_pline.width1 = jednostkiOb (width_p) ;
  s_pline.width = jednostkiOb (width_k) ;
  start_width=s_pline.width1;
  wypuklosc0 = wypuklosc;
  
}

void start_pline_hatch (double X_, double Y_)
/*-------------------------------------------*/
{ WIELOKAT W = S4def;

    memcpy (&s_pline.solid, &W, sizeof(W)) ;
    start_x = X_;
    start_y = Y_;
    line_g.x1 = jednostkiOb (X_ - EXTMIN_X) ;
    line_g.y1 = jednostkiOb (Y_ - EXTMIN_Y) ;

    width_p = width_p0;
    width_k = width_k0;
    s_pline.width1 = jednostkiOb (width_p) ;
    s_pline.width = jednostkiOb (width_k) ;
    start_width=s_pline.width1;
    wypuklosc0 = wypuklosc;
}

void start_spline(int to_block1)
/*--------------------------------------------------*/
{
	
	if (to_block1 == 0)
	{
		if (FALSE == add_block_pline())
		{
			return;
		}
	}
	else
	{
		if (FALSE == add_block_pline_in_block())  //!!!!!!!!!!!!!!!!!
		{
			return;
		}
	}
	
}


int read_lines_hatch(FILE *f, int to_block, BOOL block, int object_no)
/*------------------------------------------------------------------*/
{
	
	BOOL endentitie;
	char *p, *p1;
	int entitie_code;
	double e_xp=0, e_yp=0, e_zp=0, e_xk=0, e_yk=0, e_zk=0;
	double e_xp1, e_yp1;
	double e_xk1, e_yk1;
	char buf[MAXLINE], buf1[MAXLINE];
	LINIA L = Ldef;
	int i_segm = 0;

	endentitie = FALSE;
	while (endentitie == FALSE)
	{
		if (!inc_nr_linii()) return -1;
		if (myfgets(buf, MAXLINE, f) == NULL) return -1;

		p = buf;

		if (sscanf(p, "%d", &entitie_code) < 1) return -1;


		if ((entitie_code != 0) && (entitie_code != 72) && (entitie_code != 92) && (entitie_code != 93)
			&& (entitie_code != 97) && (entitie_code != 75) && (entitie_code != 76) && (entitie_code != 98)
			&& (entitie_code != 53) && (entitie_code != 43) && (entitie_code != 44) && (entitie_code != 46)
			&& (entitie_code != 79) && (entitie_code != 49))
		{
			if (!inc_nr_linii()) return -1;
			if (myfgets(buf1, MAXLINE, f) == NULL) return -1;

			p1 = buf1;
		}

		switch (entitie_code)
		{
		case 0:
		case 72:
		case 92:
		case 93:
		case 97:
		case 75:
		case 76:
		case 98:
		case 53:
		case 43:
		case 44:
		case 45:
		case 46:
		case 79:
		case 49:
			
			/*generowanie linii*/
			e_xp1 = jednostkiOb(e_xp - EXTMIN_X);
			e_yp1 = jednostkiOb(e_yp - EXTMIN_Y);
			e_xk1 = jednostkiOb(e_xk - EXTMIN_X);
			e_yk1 = jednostkiOb(e_yk - EXTMIN_Y);

			L.x1 = e_xp1;
			L.y1 = e_yp1;
			L.x2 = e_xk1;
			L.y2 = e_yk1;
			L.typ = ep_type;

            L.atrybut=Ablok;
		
			L.kolor = ep_color;
			L.warstwa = ep_layer;
			L.przec = ep_space;
			L.obiektt2 = O2BlockPline;

            if (solid_hatch_ok)
            {
                switch (object_no) {
                    case 0:
                        solid_hatch->xy[0] = L.x1;
                        solid_hatch->xy[1] = L.y1;
                        solid_hatch->xy[2] = L.x2;
                        solid_hatch->xy[3] = L.y2;
                    break;
                    case 1:
                        if (Check_if_Equal(L.x1, solid_hatch->xy[2])==FALSE) {solid_hatch_ok=FALSE; break;}
                        if (Check_if_Equal(L.y1, solid_hatch->xy[3])==FALSE) {solid_hatch_ok=FALSE; break;}
                        solid_hatch->xy[4] = L.x2;
                        solid_hatch->xy[5] = L.y2;
                        break;
                    case 2:
                        if (Check_if_Equal(L.x1, solid_hatch->xy[4])==FALSE) {solid_hatch_ok=FALSE; break;}
                        if (Check_if_Equal(L.y1, solid_hatch->xy[5])==FALSE) {solid_hatch_ok=FALSE; break;}

                        if (solid_hatch->lp==6) {
                            if (Check_if_Equal(L.x2, solid_hatch->xy[0])==FALSE) {solid_hatch_ok=FALSE; break;}
                            if (Check_if_Equal(L.y2, solid_hatch->xy[1])==FALSE) {solid_hatch_ok=FALSE; break;}
                        }
                        else {
                            solid_hatch->xy[6] = L.x2;
                            solid_hatch->xy[7] = L.y2;
                        }
                    break;
                    case 3:
                        if (Check_if_Equal(L.x1, solid_hatch->xy[6])==FALSE) {solid_hatch_ok=FALSE; break;}
                        if (Check_if_Equal(L.y1, solid_hatch->xy[7])==FALSE) {solid_hatch_ok=FALSE; break;}
                        if (Check_if_Equal(L.x2, solid_hatch->xy[0])==FALSE) {solid_hatch_ok=FALSE; break;}
                        if (Check_if_Equal(L.y2, solid_hatch->xy[1])==FALSE) {solid_hatch_ok=FALSE; break;}
                    break;
                    default:
                        solid_hatch_ok=FALSE;
                    break;
                }
            }

			if (to_block == 1)
			{
				L.blok = 1;
				if (NULL == dodaj_obiekt((BLOK*)dane, &L)) return -1;
			}
			else
			{
                //hatch always creates block
                L.blok = 1;
                if (NULL == dodaj_obiekt((BLOK*)dane, &L)) return -1;
			}

            if (object_no==0)
            {
                start_x=e_xp;
                start_y=e_yp;
            }

			endentitie = TRUE;
			return entitie_code;
			break;
		case 10:  /*start point X - double*/
			if (sscanf(p1, "%lf", &e_xp) < 1) return -1;
			break;
		case 20:  /*start point Y - double*/
			if (sscanf(p1, "%lf", &e_yp) < 1) return -1;
			break;
		case 11:  /*end point X - double*/
			if (sscanf(p1, "%lf", &e_xk) < 1) return -1;
			break;
		case 21:  /*end point Y - double*/
			if (sscanf(p1, "%lf", &e_yk) < 1) return -1;
			break;
		default:  break;
		} /*switch*/
	}
	return 1;
}

int read_lwpolyline_hatch(FILE *f, int to_block, BOOL block, int number_of_paths, int path_type_flag, int path_no, int solid_flag)
/*------------------------------------------------------------------------------------------------------------------------------*/
{
    BOOL endentitie;
    char *p, *p1;
    int entitie_code;
    double e_extrusion_z=0;
    double e_xp, e_yp;
    int zamkniecie=0;
    int bulge_flag=0;
    int baundry_objects=0;
    int edges_vertices=0;
    char buf[MAXLINE], buf1[MAXLINE];
    LINIA L = Ldef;
    int nw_max=100;
    double wypuklosc1;
    int nw42;

    s_pline = {FALSE, FALSE, 0, 0, 0, Ldef, 7, S4def};
    line_g = Ldef ;

    endentitie = FALSE;
    wypuklosc0 = 0.0;
    wypuklosc = 0.0;
    wypuklosc1 = 0.0;
    width_p0 = 0;
    width_k0 = 0;
    nw42=0;

    ep_type = HATCH_OUTLINE_TYPE & 31;

    ep_width = -1;

    nw = 0;
    while (endentitie == FALSE)
    {
        if (!inc_nr_linii()) return -1;
        if (myfgets(buf, MAXLINE, f) == NULL) return -1;

        p = buf;

        if (sscanf(p, "%d", &entitie_code) < 1) return -1;

        if ((entitie_code != 0) && (entitie_code != 92)
            && (entitie_code != 75) && (entitie_code != 76) && (entitie_code != 98)
            && (entitie_code != 53) && (entitie_code != 43) && (entitie_code != 44) && (entitie_code != 46)
            && (entitie_code != 79) && (entitie_code != 49))
        {
            if (!inc_nr_linii()) return -1;
            if (myfgets(buf1, MAXLINE, f) == NULL) return -1;

            p1 = buf1;
        }

        switch (entitie_code)
        {
            case 0:
            case 92:
            case 75:
            case 76:
            case 98:
            case 53:
            case 52:
            case 41:
            case 43:
            case 44:
            case 45:
            case 46:
            case 79:
            case 49:
                if (nw>0) {
                    if (solid_type==0) {
                        add_pline_line(e_xp, e_yp, to_block, Ablok);
                        wypuklosc = wypuklosc1;
                        if (zamkniecie == 0)
                        {
                            add_pline_line_end(to_block, e_extrusion_z, Ablok);
                        }
                        else {
                            add_pline_line(start_x, start_y, to_block, Ablok);

                            s_pline.width1 = start_width;
                            s_pline.wypuklosc=wypuklosc0;
                            add_pline_line_end(to_block, e_extrusion_z, Ablok);
                        }
                    }
                    else
                    {
                        solid_hatch->xy[(nw-1)*2]=jednostkiOb(e_xp - EXTMIN_X) ;
                        solid_hatch->xy[(nw-1)*2+1]=jednostkiOb(e_yp - EXTMIN_Y) ;
                    }

                    baundry_objects = 0;
                    edges_vertices = 0;
                    nw = 0;

                }

                endentitie = TRUE;
                return entitie_code;
                break;

            case 10:  /*start point X - double*/
                if (nw > 0)
                {
                    if (nw == 1)
                    {
                        if (solid_type==0)
                            start_pline_hatch(e_xp, e_yp);
                        else
                        {
                            solid_hatch->xy[(nw-1)*2]=jednostkiOb(e_xp - EXTMIN_X) ;
                            solid_hatch->xy[(nw-1)*2+1]=jednostkiOb(e_yp - EXTMIN_Y) ;
                            start_x=solid_hatch->xy[(nw-1)*2];
                            start_y=solid_hatch->xy[(nw-1)*2+1];
                        }
                    }
                    else
                    {
                        if (solid_type==0)
                            add_pline_line(e_xp, e_yp, to_block, Ablok);
                        else
                        {
                            solid_hatch->xy[(nw-1)*2]=jednostkiOb(e_xp - EXTMIN_X) ;
                            solid_hatch->xy[(nw-1)*2+1]=jednostkiOb(e_yp - EXTMIN_Y) ;
                        }
                    }
                }
                wypuklosc = wypuklosc1;
                if (sscanf(p1, "%lf", &e_xp) < 1) return -1;
                break;
            case 20:  /*start point Y - double*/
                nw++;
                if (sscanf(p1, "%lf", &e_yp) < 1) return -1;
                /*dopisanie linii*/
                break;
            case 42: /*wypuklosc*/
                if ( sscanf (p1 , "%lf", &wypuklosc1) < 1 ) return 0;
                if (nw42==0) wypuklosc0=wypuklosc1;
                nw42++;
                break;
            case 72: /*Has bulge flag*/
                if (sscanf(p1, "%d", &bulge_flag) < 1) return -1;
                break;
            case 73: /*close*/
                if (sscanf(p1, "%d", &zamkniecie) < 1) return -1;
                break;
            case 93: /*Number of edges in this boundary path (only if boundary is not a polyline)  or  Number of polyline vertices*/
                if (sscanf(p1, "%d", &edges_vertices) < 1) return -1;

                if ((path_no==0) && ((!(path_type_flag & 2)) || (edges_vertices>4) || (solid_flag==0) || (edges_vertices<3) || (zamkniecie==0) || (bulge_flag>0) || (number_of_paths!=1))) //no SOLID
                {
                    solid_type=0;
                    if (to_block == 0) {
                        if (add_block_hatch(TRUE) == NULL) {
                            return 0;
                        }
                    } else {
                        if (add_block_hatch_in_block(TRUE) == NULL) {
                            return 0;
                        }
                    }
                }
                else {
                    if ((edges_vertices>2) && (edges_vertices <= 4) && (zamkniecie==1) && (solid_flag==1)) {
                        solid_type = edges_vertices;
                        if (edges_vertices==3) solid_hatch=&solid3_hatch;
                        else solid_hatch=&solid4_hatch;
                    }
                }

                if (nw>0) {

                    if (solid_type==0) {
                        add_pline_line(e_xp, e_yp, to_block, Ablok);

                        if (zamkniecie == 1) {
                            add_pline_line(start_x, start_y, to_block, Ablok);
                        }
                    }
                    else
                    {
                        solid_hatch->xy[(nw-1)*2]=jednostkiOb(e_xp - EXTMIN_X) ;
                        solid_hatch->xy[(nw-1)*2+1]=jednostkiOb(e_yp - EXTMIN_Y) ;
                    }

                    baundry_objects=0;
                    edges_vertices=0;
                    nw=0;

                }

                break;
            case 97:
                if (sscanf(p1, "%d", &baundry_objects) < 1) return -1;
                break;
            default:  break;
        } /*switch*/
    }
    return entitie_code;
}

int read_arcs_hatch(FILE *f, int to_block, BOOL block, int object_no)
/*-----------------------------------------------------------------*/
{
	BOOL endentitie;
	char *p, *p1;
	int entitie_code;
	
	double e_x, e_y;
	double e_r = 0, e_kp = 0, e_kk = 0;
	double e_x1, e_y1;
	double e_r1, e_kp1, e_kk1;
	char buf[MAXLINE], buf1[MAXLINE];
	LUK l = ldef;
	int counterclockwise_flag = 1;

    int a;

	endentitie = FALSE;
	while (endentitie == FALSE)
	{
		if (!inc_nr_linii()) return -1;
		if (myfgets(buf, MAXLINE, f) == NULL) return -1;

		p = buf;

		if (sscanf(p, "%d", &entitie_code) < 1) return -1;


		if ((entitie_code != 0) && (entitie_code != 72) && (entitie_code != 92) && (entitie_code != 93)
			&& (entitie_code != 97) && (entitie_code != 75) && (entitie_code != 76) && (entitie_code != 98)
			&& (entitie_code != 53) && (entitie_code != 43) && (entitie_code != 44) && (entitie_code != 46)
			&& (entitie_code != 79) && (entitie_code != 49))
		{
			if (!inc_nr_linii()) return -1;
			if (myfgets(buf1, MAXLINE, f) == NULL) return -1;

			p1 = buf1;
		}


		switch (entitie_code)
		{
		case 0:
		case 72:
		case 92:
		case 93:
		case 97:
		case 75:
		case 76:
		case 98:
		case 53:
		case 43:
		case 44:
		case 45:
		case 46:
		case 79:
		case 49:

			/*generowanie luku*/
			e_x1 = jednostkiOb(e_x - EXTMIN_X);
			e_y1 = jednostkiOb(e_y - EXTMIN_Y);
			e_r1 = jednostkiOb(e_r);
			if (Check_if_LE(e_r1, 0) == TRUE)
			{
                endentitie = TRUE;
                return entitie_code;
				break; /*zignorowanie luku*/
			}

            if (counterclockwise_flag)
            {
                e_kp1 = e_kp * Pi2 / 360;
                e_kk1 = e_kk * Pi2 / 360;
            }
            else
            {
                e_kk1 = e_kp * Pi2 / 360;
                e_kp1 = (360-e_kk) * Pi2 / 360;
            }

			if (TRUE == Check_if_Equal(e_kp1, e_kk1))
			{
                endentitie = TRUE;
                return entitie_code;
				break;  /*zignorowanie luku*/
			}

			l.x = e_x1;
			l.y = e_y1;
			l.r = e_r1;
            l.kat1 = e_kp1;
            l.kat2 = e_kk1;

			//l.typ=e_type;
			l.typ = ep_type;
			l.kolor = ep_color;
			l.warstwa = ep_layer;
			l.przec = ep_space;
            l.obiektt2 = O2BlockPline;
            l.atrybut=Ablok;

			if (to_block == 1)
			{
				l.blok = 1;
				if (NULL == dodaj_obiekt((BLOK*)dane, &l)) return -1;
			}
			else
			{
                //hatch always creates block
                l.blok = 1;
                if (NULL == dodaj_obiekt((BLOK*)dane, &l)) return -1;
			}

            if (object_no==0)
            {
                start_x=e_x+e_r*cos(e_kp1);
                start_y=e_y+e_r*sin(e_kp1);
            }

			endentitie = TRUE;
			return entitie_code;
			break;
		case 10:  /*start point X - double*/
			if (sscanf(p1, "%lf", &e_x) < 1) return -1;
			break;
		case 20:  /*start point Y - double*/
			if (sscanf(p1, "%lf", &e_y) < 1) return -1;
			break;
		case 40:  /*radius - double*/
			if (sscanf(p1, "%lf", &e_r) < 1) return -1;
			break;
		case 50:  /*start angle - double*/
			if (sscanf(p1, "%lf", &e_kp) < 1) return -1;
			break;
		case 51:  /*end angle - double*/
			if (sscanf(p1, "%lf", &e_kk) < 1) return -1;
			break;
		case 73:  /*counterclockwise flag*/
			if (sscanf(p1, "%d", &counterclockwise_flag) < 1) return -1;
			break;
		default:  break;
		} /*switch*/
	}
	return 1;
 }

int read_ellipses_hatch(FILE *f, int to_block, BOOL block, int object_no, int number_of_paths, int solid_flag, char *pattern_name, int e_layer, int e_color, int e_space, BOOL *done)
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
{
	BOOL endentitie;
	char *p, *p1;
	int entitie_code;
	double e_x, e_y;
	double e_r = 0, e_kp = 0, e_kk = 0, e_kk1;
	double ek_x, ek_y; // ek_z;
	char buf[MAXLINE], buf1[MAXLINE];
	LINIA L = Ldef;
	int i;
	double pa, pb, pu; // puu, 
	double dpu, px[37], py[37];
	double kos, koc;
    int counterclockwise_flag = 1;

    ELLIPSE e=FEdef;
    double angle;
    int e_type=0;
    char *base_adr;
    BLOK* base_block;
	
	endentitie = FALSE;
	while (endentitie == FALSE)
	{
		if (!inc_nr_linii()) return -1;
		if (myfgets(buf, MAXLINE, f) == NULL) return -1;

		p = buf;

		if (sscanf(p, "%d", &entitie_code) < 1) return -1;


		if ((entitie_code != 0) && (entitie_code != 72) && (entitie_code != 92) && (entitie_code != 93)
			&& (entitie_code != 97) && (entitie_code != 75) && (entitie_code != 76) && (entitie_code != 98)
			&& (entitie_code != 53) && (entitie_code != 43) && (entitie_code != 44) && (entitie_code != 46)
			&& (entitie_code != 79) && (entitie_code != 49))
		{
			if (!inc_nr_linii()) return -1;
			if (myfgets(buf1, MAXLINE, f) == NULL) return -1;

			p1 = buf1;
		}

		switch (entitie_code) {
            case 0:
            case 72:
            case 92:
            case 93:
            case 97:
            case 75:
            case 76:
            case 98:
            case 53:
            case 43:
            case 44:
            case 45:
            case 46:
            case 79:
            case 49:

                if (((Check_if_Equal(e_kp, 0.0)) && (Check_if_Equal(e_kk, Pi2)) ||
                      (Check_if_Equal((e_kk-e_kp), 2*Pi))) && (number_of_paths == 1) && (solid_flag == 1)) {

                    e_kp=Angle_Normal(e_kp);
                    e_kk=Angle_Normal(e_kk);

                    //Generowanie elipsy / luku eliptycznego

                    pa=(sqrt((ek_x * ek_x) + (ek_y * ek_y)));  //dlugosc wiekszej polosi
                    pb=e_r*pa;  //dlugosc mniejszej polosi

                    //wyznaczenie kata
                    kos=ek_y/(-pa);
                    koc=ek_x/(-pa);

                    angle=Atan2(ek_y, ek_x);
                    angle=Angle_Normal(angle);

                    e.x = jednostkiOb(e_x - EXTMIN_X);
                    e.y = jednostkiOb(e_y - EXTMIN_Y);

                    e.angle = angle;
                    e.rx = jednostkiOb(pa);
                    e.ry = jednostkiOb(pb);

                    e.typ = get_typ(e_type, ep_width);
                    e.kolor = e_color;
                    e.warstwa = e_layer;
                    e.przec = e_space;
                    e.translucency = 255;


                    if (to_block == 1) {
                        e.blok = ElemBlok;
                        if (NULL == dodaj_obiekt((BLOK*)dane, &e)) return 0;
                    }
                    else
                    {
                        e.blok = NoElemBlok;
                        if (NULL == dodaj_obiekt(NULL, &e)) return 0;
                    }

                    *done=TRUE;
                }
                else
                {
                    //Generowanie elipsy / luku eliptycznego
                    pa = (sqrt((ek_x * ek_x) + (ek_y * ek_y)));  //dlugosc wiekszej polosi
                    pb = e_r * pa;  //dlugosc mniejszej polosi

                if (!counterclockwise_flag) {
                    e_kk1 = e_kk;
                    e_kk = e_kp;
                    e_kp = (Pi2 - e_kk1);
                }

                //wyznaczenie kata
                kos = ek_y / (-pa);
                koc = ek_x / (-pa);

                pu = e_kp;
                //dzielimy luk na 36 czesci
                dpu = (e_kk - e_kp) / 36;

                for (i = 0; i <= 36; i++) {
                    pu = e_kp + (i * dpu);

                    px[i] = pa * cos(pu) + e_x;
                    py[i] = pb * sin(pu) + e_y;

                    if (i == 0) {
                        start_x = px[0];
                        start_y = py[0];
                    }

                    px[i] = jednostkiOb(px[i] - EXTMIN_X);
                    py[i] = jednostkiOb(py[i] - EXTMIN_Y);

                }

                if (to_block == 0) {

                    //hatch always creates block
                    if (FALSE == add_block_pline_in_block()) {
                        return -1;
                    }
                } else {
                    if (FALSE == add_block_pline_in_block()) {
                        return -1;
                    }
                }

                //a wiec mamy wszystkie 36 punktow ktore teraz laczymy liniami
                for (i = 0; i < 36; i++) {
                    L.x1 = px[i];
                    L.y1 = py[i];
                    L.x2 = px[i + 1];
                    L.y2 = py[i + 1];
                    L.typ = ep_type;
                    L.kolor = ep_color;
                    L.warstwa = ep_layer;
                    L.przec = ep_space;
                    L.obiektt2 = O2BlockPline;

                    L.atrybut = Ablok;

                    if (kos != 0.0) {

                        transformacja_blok((char *) &L, ((char *) &L) + 1, jednostkiOb(e_x - EXTMIN_X),
                                           jednostkiOb(e_y - EXTMIN_Y), kos, koc, Tobrot, 0);

                    }

                    L.blok = 1;

                    if (NULL == dodaj_obiekt((BLOK *) dane, &L)) return -1;

                    }
                }

			endentitie = TRUE;
			return entitie_code;
			break;
		
		case 10:  /*start point X - double*/
			if (sscanf(p1, "%lf", &e_x) < 1) return -1;
			break;
		case 20:  /*start point Y - double*/
			if (sscanf(p1, "%lf", &e_y) < 1) return -1;
			break;
		case 11:  /*end point X - double*/
			if (sscanf(p1, "%lf", &ek_x) < 1) return -1;
			break;
		case 21:  /*end point Y - double*/
			if (sscanf(p1, "%lf", &ek_y) < 1) return -1;
			break;
		case 40:  /*radius ratio - double*/
			if (sscanf(p1, "%lf", &e_r) < 1) return -1;
			break;
		case 41:  /*start angle - double*/
			if (sscanf(p1, "%lf", &e_kp) < 1) return -1;
			e_kp += Pi_;
			break;
		case 42:  /*end angle - double*/
			if (sscanf(p1, "%lf", &e_kk) < 1) return -1;
			e_kk += Pi_;
			break;
		case 50:  /*start angle - double*/
			if (sscanf(p1, "%lf", &e_kp) < 1) return -1;
            e_kp *= (Pi_/180);
			break;
		case 51:  /*end angle - double*/
			if (sscanf(p1, "%lf", &e_kk) < 1) return -1;
			e_kk *= (Pi_/180);
			break;
        case 73:  /*counterclockwise flag*/
            if (sscanf(p1, "%d", &counterclockwise_flag) < 1) return -1;
            break;
		default:  break;
		} /*switch*/
	}
	return 1;
}

int read_splines_hatch(FILE *f, int to_block, BOOL block, int object_no)
/*--------------------------------------------------------------------*/
{
	BOOL endentitie;
	char *p, *p1;
	int entitie_code;
	double e_elevation = 0;
	unsigned int e_flags = 0;
	double e_xp[NumSplinePoints];
	double e_yp[NumSplinePoints];
	double e_zp[NumSplinePoints];
	double e_xfit[NumSplinePoints];
	double e_yfit[NumSplinePoints];
	double e_zfit[NumSplinePoints];
	double e_xstangent;
	double e_ystangent;
	double e_zstangent;
	double e_xetangent;
	double e_yetangent;
	double e_zetangent;
	double weight[4];
	int e_lpoints = 2;
	char buf[MAXLINE], buf1[MAXLINE];
	LINIA L = Ldef;
	int to_block1=FALSE;
	int s_degree = 3; 
	int s_n_knots = 0;
	int s_n_ctrlpoints = 0;
	int s_n_fitpoints = 0;
	double s_fittolerance = 0.0000001;
	int snw, snwz, smnw, smnwz;
	int ns;
	double start_sx = 0, start_sy = 0, start_sz = 0;
	int ret;

	int s_knots;
	int s_control_points;
	int s_rational;
	int s_periodic;

	double bcurviness = 0.75;
	int spline_type;

    int a;

    to_block1 = TRUE;

	zamkniecie = 0;
	wypuklosc = 0;
	wypuklosc0 = 0;
	drugi_wezel = FALSE;
	trzeci_wezel = FALSE;
	first_arc = FALSE;
	fit_curve = FALSE;
	myspline = FALSE;
	planarspline = FALSE;
	linearspline = FALSE;
	width_p_standard = 0;
	width_k_standard = 0;
	snw = 0;
	snwz = 0;
	smnw = 0;
	smnwz = 0;
	ns = 0;
	width_p = 0;
	width_k = 0;

	endentitie = FALSE;
	while (endentitie == FALSE)
	{
		if (!inc_nr_linii()) return -1;
		if (myfgets(buf, MAXLINE, f) == NULL) return -1;

		p = buf;

		if (sscanf(p, "%d", &entitie_code) < 1) return -1;


		if ((entitie_code != 0) && (entitie_code != 72) && (entitie_code != 92) && (entitie_code != 93)
			&& (entitie_code != 75) && (entitie_code != 76) && (entitie_code != 98)
			&& (entitie_code != 53) && (entitie_code != 43) && (entitie_code != 44) && (entitie_code != 46)
			&& (entitie_code != 79) && (entitie_code != 49))
		{
			if (!inc_nr_linii()) return -1;
			if (myfgets(buf1, MAXLINE, f) == NULL) return -1;

			p1 = buf1;
		}

		switch (entitie_code)
		{
		case 0:
		case 72:
		case 92:
		case 93:
		case 75:
		case 76:
		case 98:
		case 53:
		case 43:
		case 44:
		case 45:
		case 46:
		case 79:
		case 49:
			
			endentitie = TRUE;
			//fit points have priority
			if (smnw > 0)
			{
				if (smnw < 2) return -1;

				spline_type = 5;
				ret = add_bspline(e_xfit, e_yfit, e_zfit, to_block1, smnw, spline_type, zamkniecie, bcurviness, Ablok);
			}
			if (snw > 0)
			{

				if (snw == 2) spline_type = 3;
				else if (snw == 3) spline_type = 4;
				else spline_type = 6;
				ret = add_bspline(e_xp, e_yp, e_zp, to_block1, snw, spline_type, zamkniecie, bcurviness, Ablok);
			}

			endentitie = TRUE;
			return entitie_code;
			break;
		
		case 94: /*degree of spline*/
			if (sscanf(p1, "%d", &s_degree) < 1) return -1;
			break;
		case 95: /*number of knots*/
			if (sscanf(p1, "%d", &s_knots) < 1) return -1;
			break;
		case 96: /*number of control points*/
			if (sscanf(p1, "%d", &s_control_points) < 1) return -1;
			break;
		case 97: /*no of fit points*/
			if (sscanf(p1, "%d", &s_n_fitpoints) < 1) return -1;
			break;
		case 73: /**/
			if (sscanf(p1, "%d", &s_rational) < 1) return -1;
			break;
		case 74: /**/
			if (sscanf(p1, "%d", &s_periodic) < 1) return -1;
			break;
		case 10:  /*start point X - double*/
			//s_degree determines number of control points
			//if s_degree==2  we take 3 control point (snw=3) and we create spline x0,y0; x1,y1; x3,y3=x2,y2
			//if s_degree==3  we take 4 control point (snw=3) and we create spline x0,y0; x1,y1; x2,y2; x3,x3
			//if s_degree>3 we divide spline for s_debree=3 spline
			//always x0,y0=x3,y3 for next segment
			//we do not care about any other parameters

			wypuklosc = 0;
			if (sscanf(p1, "%lf", &e_xp[snw]) < 1) return -1;
			if (ns == 0) start_sx = e_xp[snw];
			break;
		case 20:  /*start point Y - double*/
			if (sscanf(p1, "%lf", &e_yp[snw]) < 1) return -1;
			if (ns == 0) start_sy = e_yp[snw];
			if (snw < NumSplinePoints - 1) snw++;
			break;
		case 30:  /*start point Z - double*/
			if (sscanf(p1, "%lf", &e_zp[snwz]) < 1) return -1;
			if (ns == 0) start_sz = e_zp[snwz];
			if (snwz < NumSplinePoints - 1) snwz++;
			break;

		case 11:  /*fit point X - double*/
			if (sscanf(p1, "%lf", &e_xfit[smnw]) < 1) return -1;
			break;
		case 21:  /*fit point Y - double*/
			if (sscanf(p1, "%lf", &e_yfit[smnw]) < 1) return -1;
			if (smnw < NumSplinePoints - 1) smnw++;
			break;
		case 31:  /*fit point Z - double*/
			if (sscanf(p1, "%lf", &e_zfit[smnwz]) < 1) return -1;
			if (smnwz < NumSplinePoints - 1) smnwz++;
			break;

		case 12:  /*x start tangent - double*/
			if (sscanf(p1, "%lf", &e_xstangent) < 1) return -1;
			break;
		case 13:  /*y end tangent - double*/
			if (sscanf(p1, "%lf", &e_xetangent) < 1) return -1;
			break;
		case 22:  /*fit datum Y - double*/
			if (sscanf(p1, "%lf", &e_ystangent) < 1) return -1;
			break;
		case 32:  /*fit datum Y - double*/
			if (sscanf(p1, "%lf", &e_yetangent) < 1) return -1;
			break;
		case 23:  /*fit datum Y - double*/
			if (sscanf(p1, "%lf", &e_zstangent) < 1) return -1;
			break;
		case 33:  /*fit datum Y - double*/
			if (sscanf(p1, "%lf", &e_zetangent) < 1) return -1;
			break;
		case 42: /*weightss*/
			if (sscanf(p1, "%lf", &weight[snw]) < 1) return -1;
			break;
		default:  break;
		} /*switch*/
	}
	return 1;
}

T_PTR_Hatch_Def_Param Get_Hatch_Def_Table999 (void)
{
    return &ptrs__hatch_def_param999;
}

T_PTR_Hatch_Def_Param Get_Hatch_Def_Table999_ (void)
{
    return Get_Hatch_Def_Table999();
}

T_PTR_Hatch_Line_Def Get_Hatch_Line_Def_Table999 (void)
{
    return s_hatch_line_def999;
}

BOOL read_hatch(FILE *f, int to_block, BOOL block, int draw)
/*--------------------------------------------------------*/
{
	BOOL endentitie;
	char *p, *p1;
	char buf[MAXLINE], buf1[MAXLINE];
	int entitie_code;
	char pattern_name[64];
	int solid_flag, associativity_flag, path_type_flag, edge_type, is_closed;
	int number_of_paths, number_of_edges, number_of_vertices, number_of_seed_points, number_of_dash_length_items;
	double e_xp0, e_yp0, e_zp0;
	BOOL color_defined;
	LINIA L = Ldef;
	int nw;
    double pattern_line_angle;
    int hatch_style, hatch_type;
	double pattern_base_x;
	double pattern_base_y;
	double pattern_offset_x;
	double pattern_offset_y;
    double dash_length;
    int dash_no;
	int ret;
    void *base_block;
    unsigned char ep_type0;
    int ret_h;
    BOOL go_ahead;
    int path_no=0;
    double start_x0, start_y0;
    int i_number_hatch_arch;
    double df_angle_rev0;

    T_Hatch_Param s_hatch_param999={999,0,1,0,0,FALSE};
    int number_of_pattern_definition_lines;
    int def_line_no;
    int object_no;
    double kos, koc;
    BOOL is_polyline;
    BOOL seed_points_no=0;
    POINTD *seed_points=NULL;
    double seed_x, seed_y;
    int number_of_source_boundary_objects;
    double dx0;
    double dy0;
    BOOL done_ellipse;

	solid_flag = 1;
	ep_color = 7;
	ep_type = 0;
	associativity_flag = 0;
	edge_type = 0;
	number_of_edges = 0;
	number_of_vertices = 0;
	is_closed = 0;
	ep_type = 0;
    ep_width=line_width.normal*100;
    hatch_type=-1;
    number_of_pattern_definition_lines=0;
    def_line_no=-1;
    dash_no=0;
    is_polyline=FALSE;
    number_of_seed_points=0;

    solid_type=0;
    object_no=0;
    solid_hatch_ok=TRUE;

	endentitie = FALSE;
	while (endentitie == FALSE)
	{
		if (!inc_nr_linii()) goto error; //return 0;
		if (myfgets(buf, MAXLINE, f) == NULL) goto error; //return 0;

		p = buf;

		if (sscanf(p, "%d", &entitie_code) < 1) goto error; //return 0;

		if (entitie_code != 0)
		{
			if (!inc_nr_linii()) goto error; //return 0;
			if (myfgets(buf1, MAXLINE, f) == NULL) goto error; //return 0;

			p1 = buf1;
		}
go_repeat:
		switch (entitie_code)
		{
		case 0:
			endentitie = TRUE;

			break;
        case 8:  /*layer - sring*/
            if (!block)
            {
                ep_layer = find_layer(buf1);
                ep_type = Layers[ep_layer].line_type;
                ep_color = Layers[ep_layer].color;
            }
            else
            {
                ep_layer = find_layer_DXF(buf1);
                ep_type = Layers_DXF[ep_layer].line_type;
                ep_color = Layers_DXF[ep_layer].color;
            }
            ep_type0=ep_type;
            break;
        case 6:  /*line type - string*/
            ep_type=find_type(buf1,ep_type);
            ep_type0=ep_type;
            break;
        case 62:  /*color number - int*/
            if ( sscanf ( p1 , "%d", &ep_color)  < 1 ) goto error; //return 0;
            ep_color=GetColorALF(ep_color, ep_layer);
            color_defined = TRUE;
            break;
		case 10:  /*start point X - double*/
            if (seed_points_no>0) {
                if (sscanf(p1, "%lf", &seed_x) < 1) goto error;
                seed_points[number_of_seed_points-seed_points_no].x=seed_x;
            }
            else {if (sscanf(p1, "%lf", &e_xp0) < 1) goto error; //return 0;
                s_hatch_param999.df_basex=e_xp0;}
			break;
		case 20:  /*start point Y - double*/
            if (seed_points_no>0) {
                if (sscanf(p1, "%lf", &seed_y) < 1) goto error;
                seed_points[number_of_seed_points-seed_points_no].y=seed_y;
                seed_points_no--;
            }
			else {if (sscanf(p1, "%lf", &e_yp0) < 1) goto error; //return 0;
                s_hatch_param999.df_basey=e_yp0;}
			break;
		case 30:  /*start point Z - double*/
			if (sscanf(p1, "%lf", &e_zp0) < 1) goto error; //return 0;
			break;
		case 2: strcpy(pattern_name, buf1);
            if (!is_utf8(pattern_name)) decodingdxf(pattern_name);
            strncpy(ptrs__hatch_def_param999.sz_name, pattern_name,PATERN_NAME_MAXLEN-1);
			break;
		case 70:
			if (sscanf(p1, "%d", &solid_flag) < 1) goto error; //return 0;
			break;
		case 71:
			if (sscanf(p1, "%d", &associativity_flag) < 1) goto error; //return 0;
			break;
		case 91:

			if (sscanf(p1, "%d", &number_of_paths) < 1) goto error; //return 0;
			/*rozpoczecie polilinii*/
			is_closed = 0;
			nw = 0;

            if (number_of_paths>1)//no Polyline
            {
                 if (to_block == 0) {
                    if (add_block_hatch(TRUE) == NULL) {
                        goto error; //return 0;
                    }
                } else {
                    if (add_block_hatch_in_block(TRUE) == NULL) {
                        goto error; //return 0;
                    }
                }
            }

			break;
		case 92: //new path begins

			if (sscanf(p1, "%d", &path_type_flag) < 1) goto error; //return 0;

            solid_type=0;
			if (path_type_flag & 2) //Polyline
			{
                solid_hatch_ok=FALSE;
                is_polyline=TRUE;

				ret = read_lwpolyline_hatch(f, to_block, block, number_of_paths, path_type_flag, path_no, solid_flag);

				if (ret == -1) goto error; //return 0;

				else if (ret == 0) endentitie = TRUE;
				else
				{
					entitie_code = ret;
					if (!inc_nr_linii()) goto error; //return 0;
					if (myfgets(buf1, MAXLINE, f) == NULL) goto error; //return 0;

                    if (path_no==0)
                    {
                        start_x0=start_x;
                        start_y0=start_y;
                    }
					goto go_repeat;
				}

                if (path_no==0)
                {
                    start_x0=start_x;
                    start_y0=start_y;
                }

                path_no++;
			}

            else
            {
                if (to_block == 0) {
                    if (add_block_hatch(TRUE) == NULL) {
                        goto error; //return 0;
                    }
                } else {
                    if (add_block_hatch_in_block(TRUE) == NULL) {
                        goto error; //return 0;
                    }
                }
            }

			break;
		case 93: //new path begins
			if (path_type_flag == 0)
			{
				if (sscanf(p1, "%d", &number_of_edges) < 1) goto error; //return 0;
			}
			else
			{
				if (sscanf(p1, "%d", &number_of_vertices) < 1) goto error; //return 0;
			}
			break;
		case 72:
			if (sscanf(p1, "%d", &edge_type) < 1) goto error; //return 0;

            switch (edge_type) {
                case 0:
                    solid_hatch_ok=FALSE;
                    is_polyline=TRUE;
                    ret = read_lwpolyline_hatch(f, to_block, block, number_of_paths, path_type_flag, path_no, solid_flag);     //polyline
                break;
                case 1:
                    if (number_of_edges==3) solid_hatch=&solid3_hatch;
                    else if (number_of_edges==4) solid_hatch=&solid4_hatch;
                    else solid_hatch_ok=FALSE;
                    ret = read_lines_hatch(f, to_block, block, object_no);     //Line
                break;
                case 2:
                    solid_hatch_ok=FALSE;
                    ret = read_arcs_hatch(f, to_block, block, object_no); //Circular arc
                break;
                case 3:
                    solid_hatch_ok=FALSE;
                    done_ellipse=FALSE;
                    ret = read_ellipses_hatch(f, to_block, block, object_no, number_of_paths, solid_flag, pattern_name, ep_layer, ep_color, ep_space, &done_ellipse);   //elliptic arc
                    if (done_ellipse) number_of_paths=0;
                break;
                case 4:
                    solid_hatch_ok=FALSE;
                    ret = read_splines_hatch(f, to_block, block, object_no);  //Splines
                break;
                default:
                    solid_hatch_ok=FALSE;
                break;
            }

			if (ret == -1) goto error; //return 0;

            object_no++;

            if ((path_no==0) && (object_no==1))
            {
                start_x0=start_x;
                start_y0=start_y;
            }

			if (ret == 0) endentitie = TRUE;
			else
			{
				entitie_code = ret;
				if (!inc_nr_linii()) goto error; //return 0;
				if (myfgets(buf1, MAXLINE, f) == NULL) goto error; //return 0;

				goto go_repeat;
			}
			break;
        case 97:
                if (sscanf(p1, "%d", &number_of_source_boundary_objects) < 1) goto error; //return 0;

                break;
		case 73:
			if (sscanf(p1, "%d", &is_closed) < 1) goto error; //return 0;
			break;
        case 78:
            if (sscanf(p1, "%d", &number_of_pattern_definition_lines) < 1) goto error; //return 0;

            ptrs__hatch_def_param999.i_size=number_of_pattern_definition_lines;
            if  (s_hatch_line_def999!=NULL) free(s_hatch_line_def999);
            s_hatch_line_def999=(T_Hatch_Line_Def*)malloc(sizeof(T_Hatch_Line_Def)*number_of_pattern_definition_lines);
            break;
		case 98:
			if (sscanf(p1, "%d", &number_of_seed_points) < 1) goto error; //return 0;
            seed_points_no=number_of_seed_points;
            if (number_of_seed_points>0)
                seed_points=(POINTD*)malloc(number_of_seed_points*sizeof(POINTD));
			break;
        //Hatch pattern data group codes
		case 53:  /*pattern line angle*/
			if (sscanf(p1, "%lf", &pattern_line_angle) < 1) goto error; //return 0;
            def_line_no++;
            s_hatch_line_def999[def_line_no].df_len_pattern = 0.0 ;
            if (pattern_line_angle<0) pattern_line_angle+=360;
            s_hatch_line_def999[def_line_no].df_angle = Angle_Normal (Grid_to_Rad (pattern_line_angle)) ;
            df_angle_rev0 = Angle_Normal (Grid_to_Rad (pattern_line_angle)) ;
            break;
		case 43:  /*pattern base point X*/
			if (sscanf(p1, "%lf", &pattern_base_x) < 1) goto error; //return 0;
            s_hatch_line_def999[def_line_no].df_startx=pattern_base_x;
			break;
		case 44:  /*pattern base point Y*/
			if (sscanf(p1, "%lf", &pattern_base_y) < 1) goto error; //return 0;
            s_hatch_line_def999[def_line_no].df_starty=pattern_base_y;
			break;
		case 45:  /*pattern offset X*/
			if (sscanf(p1, "%lf", &pattern_offset_x) < 1) goto error; //return 0;
			break;
		case 46:  /*pattern offset Y*/
			if (sscanf(p1, "%lf", &pattern_offset_y) < 1) goto error; //return 0;

            kos = sin(df_angle_rev0);
            koc = cos(df_angle_rev0);

                s_hatch_line_def999[def_line_no].df_move=pattern_offset_x*koc+pattern_offset_y*kos;
                s_hatch_line_def999[def_line_no].df_dist=pattern_offset_y*koc-pattern_offset_x*kos;

                if (s_hatch_line_def999[def_line_no].df_dist < 0) {
                s_hatch_line_def999[def_line_no].df_move = - s_hatch_line_def999[def_line_no].df_move;
                s_hatch_line_def999[def_line_no].df_dist = - s_hatch_line_def999[def_line_no].df_dist;
            }

            break;
        case 79: /*Number of dash length items */
            if (sscanf(p1, "%d", &number_of_dash_length_items) < 1) goto error; //return 0;
            s_hatch_line_def999[def_line_no].i_def_size=number_of_dash_length_items;
            dash_no=0;
            break;
        case 49:  /*dash length (multiple entries)*/
                if (sscanf(p1, "%lf", &dash_length) < 1) goto error; //return 0;
                if (dash_no<MaxNumberSegPattern) {
                    s_hatch_line_def999[def_line_no].df_def_line[dash_no] = dash_length;
                    s_hatch_line_def999[def_line_no].df_len_pattern += fabs(dash_length);
                    dash_no++;
                }
            break;
        ////
        case 370: /*line width*/
            if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) goto error; //return 0;
            if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
            else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
            else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
            break;
        case 41:  /*pattern scale*/
                if (sscanf(p1, "%lf", &s_hatch_param999.df_scale) < 1) goto error; //return 0;
                //s_hatch_param999.df_scale*=25.4;
                //s_hatch_param999.df_scale=1.0; //pattern is scaled anyway
                s_hatch_param999.df_scale=jednostkiOb(1.0); //taking into acount transformation scale
            break;
        case 52:  /*pattern angle */
                if (sscanf(p1, "%lf", &s_hatch_param999.df_angle) < 1) goto error; //return 0;
            break;
        case 75: /*Hatch style*/
                if (sscanf(p1, "%d", &hatch_style) < 1) goto error; //return 0;
            break;
        case 76: /*Hatch pattern type:  0 = User-defined; 1 = Predefined; 2 = Custom*/
            if (sscanf(p1, "%d", &hatch_type) < 1) goto error; //return 0;
                break;
		default:  break;
		}
	}

    //setting hatch parameters

    //hatching - it has to be done for all polylines (based of set of df_x, df_y)
    i_number_hatch_arch=s_hatch_param999.i_number_hatch;

    if (to_block==1)
        base_block=(BLOK *)dane;
    else base_block=NULL;

       glb_silent=TRUE;

       ep_type=ep_type0;
       LiniaG.kolor=ep_color;
       LiniaG.typ=ep_type;
       LiniaG.warstwa=ep_layer;

       if (solid_flag==1)
       {
           //solid fill; it's crazy, people using solid fill quite often instead of solid enitity.
           // It's not maybe problem for Autocad, which doesn't provide opacity / transparency over DXF at all,
           //but it's a problem for AlfaCAD to generate drawing based on DXF in reasonable time.
           //So AlfaCAD for 3 or 4 vertices replaces HATCH witch SOLID

           if ((number_of_paths==1) && (((solid_type>0) && (is_polyline)) || (solid_hatch_ok)))
           {
              //creation a SOLID based of solid3_hatch or solid4_hatch
               solid_hatch->warstwa=ep_layer;
               solid_hatch->kolor=ep_color;
               solid_hatch->przec=ep_space;
               if (to_block == 1)
               {
                   solid_hatch->blok=1;
                   if (NULL == dodaj_obiekt ((BLOK*)dane, solid_hatch)) goto error; //return 0;
               }
               else
               {
                   solid_hatch->blok=0;
                   if (NULL == dodaj_obiekt ( NULL, solid_hatch)) goto error; //return 0;
               }

               go_ahead=FALSE;
           }
           else if (number_of_paths==0)
           {
               go_ahead=FALSE;
           }
           else
           {
               set_solidhatchtranslucency(561, 0);
               s_hatch_param999.i_number_hatch = 0;
               go_ahead = TRUE;
           }
       }
       else {
           switch (hatch_type)
           {
               case 0:  //user defined
                   settle_hatch_pattern(999);
                   s_hatch_param999.i_number_hatch=999;
                   go_ahead=TRUE;
               break;
               case 1: //predefined
                   go_ahead=FALSE;
               break;
               default:
                   go_ahead=FALSE;
               break;
           }
       }

       if (go_ahead==TRUE) {

           if (number_of_seed_points>0)
           {
               zmien_atrybut(dane, dane + dane_size, Apppoblok, Anormalny);
               for (int i=0; i<number_of_seed_points; i++)
               {
                   df_x[0] = jednostkiOb(seed_points[i].x - EXTMIN_X);
                   df_y[0] = jednostkiOb(seed_points[i].y - EXTMIN_Y);
                   ret_h = hatch_proc(0, dane_size, 1, df_x, df_y, &s_hatch_param999, 0, 0, 0, 0, 0, base_block, to_block, draw);
                   zmien_atrybut(dane, dane + dane_size, Apppoblok, Ablok);

                   if (ret_h==0)  //if area is not closed it doesn't make any sense to try another seed points
                       break;
               }
           }
           else {
               zmien_atrybut(dane, dane + dane_size, Apppoblok, Anormalny);
               dx0=2.0;
               dy0=2.0;

               df_x[0] = jednostkiOb(start_x0 - dx0 - EXTMIN_X);
               df_y[0] = jednostkiOb(start_y0 - dy0 - EXTMIN_Y);

               df_x[1] = jednostkiOb(start_x0 - dx0 - EXTMIN_X);
               df_y[1] = jednostkiOb(start_y0 + dy0 - EXTMIN_Y);

               df_x[2] = jednostkiOb(start_x0 + dx0 - EXTMIN_X);
               df_y[2] = jednostkiOb(start_y0 + dy0 - EXTMIN_Y);

               df_x[3] = jednostkiOb(start_x0 + dx0 - EXTMIN_X);
               df_y[3] = jednostkiOb(start_y0 - dy0 - EXTMIN_Y);


               ret_h = hatch_proc(0, dane_size, 4, df_x, df_y, &s_hatch_param999, 0, 0, 0, 0, 0, base_block, to_block, draw);
               zmien_atrybut(dane, dane + dane_size, Apppoblok, Ablok);

           }
       }

       s_hatch_param999.i_number_hatch=i_number_hatch_arch;
       glb_silent=FALSE;
       //resetting atrybuts
       if (solid_type==0)
           zmien_atrybut(dane, dane + dane_size, Ablok, Ausuniety); //Ausuniety);


    //if (s_hatch_line_def999 != NULL) free(s_hatch_line_def999);
    if (seed_points!=NULL) free(seed_points);
	return 1;


 error:
    if  (s_hatch_line_def999!=NULL) free(s_hatch_line_def999);
    if (seed_points!=NULL) free(seed_points);
    return 0;
}

BOOL read_seqend(FILE *f)  //reading until 0
{
    int entitie_code;
    BOOL endentitie;
    char buf[MAXLINE], buf1[MAXLINE];
    char *p, *p1;

    endentitie=FALSE;
    while (endentitie==FALSE) {
        if (!inc_nr_linii()) return 0;
        if (myfgets(buf, MAXLINE, f) == NULL) return 0;

        p = buf;

        if (sscanf(p, "%d", &entitie_code) < 1) return 0;

        if (entitie_code != 0)
        {
            if (!inc_nr_linii()) return 0;
            if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

            p1=buf1;

        }

        switch (entitie_code) {
                case 0:
                    endentitie = TRUE;
                    break;
                default:
                    break;
            }
    }
    return 1;
}

BOOL read_pline(FILE *f,int to_block, BOOL block)
/*---------------------------------------------*/
{ BOOL endentitie, vertex, seqend, endvertex;
  char *p, *p1;
  int entitie_code;
  unsigned int e_flags=0;
  double e_elevation=0;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_xp, e_yp, e_zp, e_xk, e_yk, e_zk;
  char buf[MAXLINE], buf1[MAXLINE];
  LINIA L=Ldef;
  LUK l=ldef;
  OKRAG K=Kdef;
  BOOL color_defined;
  BOOL type_defined;
  BOOL polyface_mesh;
  BOOL Entities_Follow;
  int entities_follow;
  
  int to_block1;
  int vertex_enable;
  BOOL Vertex_Enable;
  BOOL ret123;
  BOOL ret_seq;
  char obiektt1=0, obiektt2=0, obiektt3=0;

  ep_space=0;
  ep_novisibility = 0;
  ep_layer=0;
  ep_type=64;
  ep_width=line_width.normal*100;
  ep_color=7;
  to_block1 = to_block;
  s_pline.b_line=FALSE;
  s_pline.b_solid=FALSE; 	/*narysowano linie, narysowano obszar*/
  s_pline.width1=0;		/*szerokosc poczatkowa pierwszego segmentu*/
  s_pline.width=0;
 /*czytanie naglowka polilinii*/
 zamkniecie=0;
 wypuklosc=0;
 wypuklosc0=0;
 drugi_wezel=FALSE;
 trzeci_wezel=FALSE;
 first_arc=FALSE;
 fit_curve=FALSE;
 myspline=FALSE;
 width_p_standard=0;
 width_k_standard=0;
 nw=0;
 e_extrusion_x=0;
 e_extrusion_y=0;
 e_extrusion_z=1;

 color_defined=FALSE;
 type_defined=FALSE;
 polyface_mesh=FALSE;
 Entities_Follow=FALSE;

 
 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
  
  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
  
    p1=buf1;
  }  
  
  
  switch (entitie_code)
  {
  case 0:
    endentitie=TRUE;
    break;
  case 8:  /*layer - string*/
	  if (!block)
	  {
		  ep_layer = find_layer(buf1);
		  ep_type = Layers[ep_layer].line_type;
		  ep_color = Layers[ep_layer].color;
	  }
	  else
	  {
		  ep_layer = find_layer_DXF(buf1);
		  ep_type = Layers_DXF[ep_layer].line_type;
		  ep_color = Layers_DXF[ep_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    ep_type=find_type(buf1,ep_type);
    type_defined=TRUE;
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &ep_color)  < 1 ) return 0;
    ep_color=GetColorALF(ep_color, ep_layer);
    color_defined=TRUE;
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  ret123 = check_width_7(p1, &ep_width);
	  if (!ret123)
	  {
	    ret123 = check_dim_head(p1, &obiektt1, &obiektt2, &obiektt3);
	  }
	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_xp)  < 1 ) return 0;
   break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_yp)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_zp)  < 1 ) return 0;
    break;
  case 11:  /*end point X - double*/
    if ( sscanf ( p1 , "%lf", &e_xk)  < 1 ) return 0;
    break;
  case 21:  /*end point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_yk)  < 1 ) return 0;
    break;
  case 31:  /*end point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_zk)  < 1 ) return 0;
    break;
  case 70: /*polyline flag - optional 0*/
    if ( sscanf (p1 , "%d", &e_flags) < 1 ) return 0;
    if (e_flags & 1) zamkniecie=1;
    if (e_flags & 2) fit_curve=TRUE;
    if (e_flags & 4) myspline=TRUE;
    if (e_flags & 64)
      { polyface_mesh=TRUE;
        zamkniecie=1;
      }
    break;
    case 75: /*surface type*/
          if ( sscanf (p1 , "%d", &surface_type) < 1 ) return 0;
          break;
  case 40: /*default starting width - optional 0*/
    if ( sscanf (p1 , "%lf", &width_p_standard) < 1 ) return 0;
    break;
  case 41: /*default ending width - optional 0*/
    if ( sscanf (p1 , "%lf", &width_k_standard) < 1 ) return 0;
    break;
  case 66: /*default ending width - optional 0*/
    if ( sscanf (p1 , "%d", &entities_follow) < 1 ) return 0;
    if (entities_follow==1) Entities_Follow=TRUE;
    break;
  default:  break;  
  } /*switch*/
 }  /*koniec naglowka*/
 seqend=FALSE;
 vertex=FALSE;
 /*nalezy oczekiwac VERTEX lub SEQEND*/
 if (!inc_nr_linii()) return 0;
 if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
 
 if (strcmp (buf1, "SEQEND") == 0) 
  {
    ret_seq=read_seqend(f);
    return 1;
  }  
 if (strcmp (buf1, "VERTEX") == 0) vertex=TRUE;
 while ((vertex==FALSE) && (seqend==FALSE))
  {
	 if (!inc_nr_linii()) return 0;
    if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
    
    p=buf;
    
    
    if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
    
	if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
    
    if (entitie_code==0)
     {
      if (strcmp (buf1, "SEQEND") == 0) return 1;
      if (strcmp (buf1, "VERTEX") == 0) vertex=TRUE;
     } 
  }  
 /*jest pierwszy vertex*/
 
 while (seqend==FALSE)
 {
 
  endvertex=FALSE;
  nw+=1;
  width_p0=width_p_standard;
  width_k0=width_k_standard;
  wypuklosc=0;

  Vertex_Enable=TRUE;
  
  while (endvertex==FALSE)
  {
 /*VERTEX*/
	  if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
  
  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
    
    p1=buf1;
  }  
 
  switch (entitie_code)
   {
  case 0:

	  if (ep_novisibility)
	  {
		  return 1;
	  }
    endvertex=TRUE;
    vertex=FALSE;

	if (Vertex_Enable == TRUE)
	{
        /*generowanie odcinka polilinii*/
        if (!ep_novisibility) {

            if (nw == 1) {
                start_pline(x_rp, y_rp, to_block1);
            } else {
                if ((nw!=2) || (zamkniecie!=1) || (wypuklosc0!=1) || (wypuklosc!=1) || (s_pline.width1!=s_pline.width) || (s_pline.width1==0))
                    add_pline_line(x_rp, y_rp, to_block1, Anormalny);
                else add_pline_line(x_rp, y_rp, to_block1, Ausuniety);
            }
        }
    }
    break;
  case 8:  /*layer - string*/
	  if (!block)
	  {
		  ep_layer = find_layer(buf1);
		  if (type_defined == FALSE) ep_type = Layers[ep_layer].line_type;
		  if (color_defined == FALSE) ep_color = Layers[ep_layer].color;
	  }
	  else
	  {
		  ep_layer = find_layer_DXF(buf1);
		  if (type_defined == FALSE) ep_type = Layers_DXF[ep_layer].line_type;
		  if (color_defined == FALSE) ep_color = Layers_DXF[ep_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    ep_type=find_type(buf1,ep_type);
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &ep_color)  < 1 ) return 0;
    ep_color=GetColorALF(ep_color, ep_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  check_width_7(p1, &ep_width);
	  break;
  case 10:  /**/
    if ( sscanf ( p1 , "%lf", &x_rp)  < 1 ) return 0;
    break;
  case 20:  /**/
    if ( sscanf ( p1 , "%lf", &y_rp)  < 1 ) return 0;
    break;
  case 30:  /**/
    if ( sscanf ( p1 , "%lf", &z_rp)  < 1 ) return 0;
    break;
  case 70: /**/
    if ( sscanf (p1 , "%d", &znacznik_wierzcholka) < 1 ) return 0;
    break;
  case 40: /**/
    if ( sscanf (p1 , "%lf", &width_p0) < 1 ) return 0;
    break;
  case 41: /**/
    if ( sscanf (p1 , "%lf", &width_k0) < 1 ) return 0;
    break;  
  case 42: /*wypuklosc*/  
    if ( sscanf (p1 , "%lf", &wypuklosc) < 1 ) return 0;
    break;
  case 50: /**/
    if ( sscanf (p1 , "%lf", &kierunek_stycznej) < 1 ) return 0;
    break;
  case 71: /*Optional, present only in nonzero*/
  case 72:
  case 73:
  case 75: if ( sscanf (p1 , "%d", &vertex_enable) < 1 ) return 0;
           if (vertex_enable < 0)  Vertex_Enable=FALSE;
    break;
  default:  break;  
   } /*switch*/
  } /*endvertex*/
  
  if (!inc_nr_linii()) return 0;
   if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
    
   if (strcmp (buf1, "SEQEND") == 0) 
     {
      seqend=TRUE;
     } 
   if (strcmp (buf1, "VERTEX") == 0) vertex=TRUE;
  
   /*szukanie kolejnego vertex lub seqend*/
    while ((vertex==FALSE) && (seqend==FALSE))
    {
		if (!inc_nr_linii()) return 0;
    if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
    
    p=buf;
    if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
    
	if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
    
    if (entitie_code==0)
     {
      if (strcmp (buf1, "SEQEND") == 0) 
       {
        seqend=TRUE;
       } 
      if (strcmp (buf1, "VERTEX") == 0) vertex=TRUE;
     } 
   }  
 }  /*seqend*/
 /*poszukiwanie kolejnego 0*/
 entitie_code=2;
 while (entitie_code!=0)
  {
	 if (!inc_nr_linii()) return 0;
    if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
    
    p=buf;
    if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
    
    if (entitie_code!=0)
    {
		if (!inc_nr_linii()) return 0;
      if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
    }  
  }  
 
 /******/
 /*tutaj mozna sprawdzic, czy polilinia nie jest po prostu kolem*/
 
 if ((nw==2) && (zamkniecie==1) && (wypuklosc0==1) && (wypuklosc==1) && (s_pline.width1==s_pline.width) && (s_pline.width1>0))
 {
    K.x=(s_pline.line.x1 + s_pline.line.x2)/2;
    K.y=(s_pline.line.y1 + s_pline.line.y2)/2;
    K.r=s_pline.width1;
    //K.typ=ep_type;
    K.typ=get_typ(ep_type, ep_width);
    K.kolor=ep_color; 
    K.warstwa=ep_layer;
    K.przec=ep_space;

	if (!ep_novisibility)
	{

		if (to_block1 == 1)
		{
			K.blok = 1;
			if (NULL == dodaj_obiekt((BLOK*)dane, &K)) return 0;
		}
		else
		{
			K.blok = 0;
			if (NULL == dodaj_obiekt(NULL, &K)) return 0;
		}
	}
 }
  else
   {   
 /*jezeli polilinia nie jest zamknieta, nalezy ja zakonczyc*/
 /*w przeciwnym razie nalezy zamknac*/
	 if (!ep_novisibility)
	 {
		 if (zamkniecie == 0) add_pline_line_end(to_block1, e_extrusion_z, Anormalny);
		 else
		 {
 			 add_pline_line(start_x, start_y, to_block1, Anormalny);
			 s_pline.width1 = start_width;
			 add_pline_line_end(to_block1, e_extrusion_z, Anormalny);
		 }
	 }
   }

 return 1;
}


BOOL read_attrib_insert(FILE *f, BOOL block)
/*----------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned int e_layer=0, e_font=0, e_flags=0, e_hj=0, e_vj=0;
  double e_elevation=0;
  int e_color=7;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_x, e_y, e_z, e_h=2, e_wspx=1, e_wspx0 = 1, e_wspxk=1, e_k=0, e_obk=0, e_ax, e_ay, e_az;
  double e_x1, e_y1;
  double e_h1;
  double e_k1, e_obk1;
  char buf[MAXLINE], buf1[MAXLINE], buf1_1[MAXLINE], buf1_2[MAXLINE];
  char attribute_tag[MAXLINE];
  TEXT t=Tdef;
  long_long ll0, ll1, ll2;
  char *adp, *adk; 
  long_long ADP1, dane0, dane_size0;
  LINIA *L;
  BLOK  *b ;
  TEXT *T ;
  BOOL e_x_ok=FALSE, e_y_ok=FALSE, e_h_ok=FALSE, e_k_ok=FALSE;
  BOOL e_wspx_ok=FALSE, e_color_ok=FALSE, e_font_ok=FALSE, e_obk_ok=FALSE;

  e_extrusion_x=0;
  e_extrusion_y=0;
  e_extrusion_z=1;

  ep_space=0;
  ep_novisibility = 0;
  ep_width=line_width.normal*100;
 strcpy(buf1, "STANDARD");
 e_font = 0;

 e_h=TEXTSIZE;
 
 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
  
  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
  
    p1=buf1;
  }  

  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
    endentitie=TRUE;
    /*generowanie linii*/
    e_x1=jednostkiOb(e_x - EXTMIN_X);
    e_y1=jednostkiOb(e_y - EXTMIN_Y);
    e_h1=jednostkiOb(e_h);
    e_k1=e_k*Pi2/360;
    e_obk1=e_obk*Pi2/360;

    special_chars(buf1_1);

          if (!is_utf8(buf1_1)) decodingdxf(buf1_1);
       
    strcpy(t.text,buf1_1);

    special_chars(buf1_2);

    if (!is_utf8(buf1_2)) decodingdxf(buf1_2);

    strcpy(attribute_tag,buf1_2);
    
    t.x=e_x1;
    t.y=e_y1;
    t.wysokosc=e_h1;
    t.kat=e_k1;
    t.width_factor=e_wspx;
    t.kolor=e_color;
    t.warstwa=e_layer;
    t.czcionka=e_font;
    t.blok=1;
    t.n=T18+strlen(t.text); 
	t.multiline=0;
	t.underline = 0;
	t.encoding = 0;
	t.spacing=6;
    t.justowanie=j_do_lewej;
    t.ukryty=0;
    t.dl=strlen(t.text);
    if (e_obk1>0) t.italics=1; 
    /*szukanie tekstu attribute_tag w adresach ADP, ADK*/
    /**/
    adp = ADP ;
    adk = ADK ;

    L = (LINIA*)adp;
    while (L->obiekt != Okoniec && adp <= adk)
    {
    if (L->obiekt == OdBLOK)
    {
	b=(BLOK *)adp;
	adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
      {
      if ((L->obiekt == Otekst) && (L->atrybut != Abad) && (L->atrybut != Ausuniety))
       {
        T = (TEXT*)adp;
        if (T->typ==n_typ_zmienna)
        {
         if (strlen(T->text)>0)
          {
           if (strcmp(T->text,attribute_tag)==0)
            {
             dane0=(long_long)dane;
             dane_size0=dane_size;
             ADP1=(long_long)ADP;
             ll0=ADK-ADP;
             
             if (!e_x_ok) t.x=T->x;
             if (!e_y_ok) t.y=T->y;
             if (!e_h_ok) t.wysokosc=T->wysokosc;
             if (!e_k_ok) t.kat=T->kat;
             if (!e_wspx_ok) t.width_factor=T->width_factor;
			 else
			 {
				 t.width_factor = e_wspx;
				 if ((USERI1 == 211) && (e_wspx0 > 0.0)) t.width_factor /= e_wspx0;
			 }
             if (!e_color_ok) t.kolor=T->kolor;
             t.warstwa=T->warstwa;
             if (!e_font_ok) t.czcionka=T->czcionka;
             if (!e_obk_ok) t.italics=T->italics;
      
             t.blok=1;
             t.justowanie=j_do_lewej;
             t.ukryty=0;
             
    	     if (korekta_obiekt((void *)adp, (void *)&t) == NULL)
	     ErrList (15);
	     ll1=(long_long)dane-dane0;
	     ll2=dane_size-dane_size0;
	     ADP1+=ll1;
	     ADP=(char *)ADP1;
	     ADK=ADP+ll0+ll2;
             return 1;
            } 
          }
        }  
       }
      adp += L->n + sizeof(NAGLOWEK) ; 
      } 
      L = (LINIA*)adp;
    }
    /**/
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    e_color_ok=TRUE;
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  check_width_7(p1, &ep_width);
	  break;
  case 10:  /*start point X - double*/
    e_x_ok=TRUE;
    if ( sscanf ( p1 , "%lf", &e_x)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    e_y_ok=TRUE;
    if ( sscanf ( p1 , "%lf", &e_y)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z)  < 1 ) return 0;
    break;
  case 40:  /*height - double*/
    e_h_ok=TRUE;
    if ( sscanf ( p1 , "%lf", &e_h)  < 1 ) return 0;
    break;
  case 1 :  /*text value*/  
	 
    strcpy(buf1_1,buf1);
    break;
  case 2 : /*Attribute tag*/ 
	 
    strcpy(buf1_2,buf1);
    break;
  case 41:  /*relative X-scale factor - double*/
    e_wspx_ok=TRUE;
    if ( sscanf ( p1 , "%lf", &e_wspx)  < 1 ) return 0;
    break;  
  case 50:  /*rotation angle - double*/
    e_k_ok=TRUE;
    if ( sscanf ( p1 , "%lf", &e_k)  < 1 ) return 0;
    break; 
  case 51:  /*oblique angle - double*/
    e_obk_ok=TRUE;
    if ( sscanf ( p1 , "%lf", &e_obk)  < 1 ) return 0;
    break;    
  case 7 :  /*text style name*/
    e_font_ok=TRUE;
    e_font=find_font(buf1,&e_wspx, &e_wspx0, e_wspx_ok,e_h,e_h_ok,e_obk,e_obk_ok);
    break;   
  case 71:  /*text generation flags*/  
    if ( sscanf( p1 , "%d", &e_flags) < 1 ) return 0;
    break;
  case 72:  /*horizontal justification type*/  
    if ( sscanf( p1 , "%d", &e_hj) < 1 ) return 0;
    break;  
  case 73:  /*vertical justification type*/  
    if ( sscanf( p1 , "%d", &e_vj) < 1 ) return 0;
    break;    
  case 11:  /*alignment point X - double*/
    if ( sscanf ( p1 , "%lf", &e_ax)  < 1 ) return 0;
    break;
  case 21:  /*alignment point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_ay)  < 1 ) return 0;
    break;
  case 31:  /*alignment point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_az)  < 1 ) return 0;
    break;  
  default:  break;  
  } /*switch*/
 }
 return 1;
}


BOOL read_attrib(FILE *f,int to_block, BOOL block)
/*----------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned int e_layer=0, e_font=0, e_flags=0, e_hj=0, e_vj=0;
  double e_elevation=0;
  int e_color=7;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_x, e_y, e_z, e_h=2, e_wspx=1, e_wspx0 = 1, e_wspxk=1, e_k=0, e_obk=0, e_ax, e_ay, e_az;
  double e_x1, e_y1;
  double e_h1;
  double e_k1, e_obk1;
  char buf[MAXLINE], buf1[MAXLINE], buf1_1[MAXLINE]; // e_text[MAXLINE];
  TEXT t=Tdef;
  BOOL e_wspx_ok=FALSE, e_h_ok=FALSE, e_obk_ok=FALSE;
  double del_xt, del_yt, t_x, t_y;
  double e_ax1, e_ay1;
  BOOL e_ax_exists=FALSE;

 e_extrusion_x=0;
 e_extrusion_y=0;
 e_extrusion_z=1;
 ep_space=0;
 ep_novisibility = 0;
 ep_width=line_width.normal*100;
 strcpy(buf1,"STANDARD");
 e_font = 0;
 e_h=TEXTSIZE;
 
 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
  
  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
  
    p1=buf1;
  }  
  
  
  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
    endentitie=TRUE;
    /*generowanie linii*/
    e_x1=jednostkiOb(e_x - EXTMIN_X);
    e_y1=jednostkiOb(e_y - EXTMIN_Y);
    e_h1=jednostkiOb(e_h);
    e_k1=e_k*Pi2/360;
    e_obk1=e_obk*Pi2/360;

    special_chars(buf1_1);

      if (!is_utf8(buf1_1))  decodingdxf(buf1_1);

    strcpy(t.text,buf1_1);

    if ((((e_vj!=0) || (e_hj!=0))) && (e_ax_exists))
          {
              e_ax1 = jednostkiOb(e_ax - EXTMIN_X);
              e_ay1 = jednostkiOb(e_ay - EXTMIN_Y);

              t.x = e_ax1;
              t.y = e_ay1;
          }
          else {
              t.x = e_x1;
              t.y = e_y1;
          }
    t.wysokosc=e_h1;
    t.kat=e_k1;
    t.width_factor=e_wspx;
	if ((USERI1 == 211) && (e_wspx0 > 0.0)) t.width_factor /= e_wspx0;
    t.kolor=e_color;
    t.warstwa=e_layer;
    t.czcionka=e_font;
	t.multiline=0;
	t.underline = 0;
	t.encoding = 0;
	t.spacing=6;
    t.n=T18+strlen(t.text); 
    t.justowanie=j_do_lewej;
    t.ukryty=0;
    t.dl=strlen(t.text);
    t.przec=ep_space;
    if (e_obk1>0) t.italics=1;
	del_xt = (e_h1 * sin(e_k1));
	del_yt = (e_h1 * cos(e_k1));
	t.justowanie = j_do_lewej;
	t_x = t.x;
	t_y = t.y;

	switch (e_hj)
	{
	case 0: //normal
		switch (e_vj)
		{
		case 0: //baseline
			break;
		case 1: //bottom
			t.y += del_yt / 4.0;
			break;
		case 2: //middle
			t.y -= (del_yt / 2);
			break;
		case 3: //top
			t.y -= (del_yt);
			break;
		}
		t.justowanie = j_do_lewej;
		break;
	case 1: //center
		switch (e_vj)
		{
		case 0: //baseline
			t.justowanie = j_srodkowo;
			break;
		case 1: //bottom
			t.justowanie = j_srodkowo;
			t.y += del_yt / 4.0;
			break;
		case 2: //middle
			t.justowanie = j_centralnie;
			break;
		case 3: //top
			t.justowanie = j_centralnie;
			t.y -= (del_yt / 2);
			break;
		}
		break;
	case 2: //right
		switch (e_vj)
		{
		case 0: //baseline
			break;
		case 1: //bottom
			t.y += del_yt / 4.0;
			break;
		case 2: //middle
			t.y -= (del_yt / 2);
			break;
		case 3: //top
			t.y -= (del_yt);
			break;
		}
		t.justowanie = j_do_prawej;
		break;
	case 3: //aligned if vertical aligment=0
		switch (e_vj)
		{
		case 0: //baseline
			break;
		case 1: //bottom
			t.y += del_yt / 4.0;
			break;
		case 2: //middle
			t.y -= (del_yt / 2);
			break;
		case 3: //top
			t.y -= (del_yt);
			break;
		}
		t.justowanie = j_do_lewej;
		break;
	case 4: //middle if vertical alignment=0
		switch (e_vj)
		{
		case 0: //baseline
			t.justowanie = j_srodkowo;
			break;
		case 1: //bottom
			t.justowanie = j_srodkowo;
			t.y += del_yt / 4.0;
			break;
		case 2: //middle
			t.justowanie = j_centralnie;
			break;
		case 3: //top
			t.justowanie = j_centralnie;
			t.y -= (del_yt / 2);
			break;
		}
		break;
	case 5: //fit if vertical aligment=0
		switch (e_vj)
		{
		case 0: //baseline
			break;
		case 1: //bottom
			t.y += del_yt / 4.0;
			break;
		case 2: //middle
			t.y -= (del_yt / 2);
			break;
		case 3: //top
			t.y -= (del_yt);
			break;
		}
		t.justowanie = j_do_lewej;
		break;
	default://normal
		t.justowanie = j_do_lewej;
		break;
	}
  
    if (to_block == 1) 
     {
      t.blok=1;
      if (NULL == dodaj_obiekt ((BLOK*)dane, &t)) return 0; 
     }
     else
      {
       t.blok=0;
       if (NULL == dodaj_obiekt ( NULL, &t)) return 0; 
      } 
    
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  check_width_7(p1, &ep_width);
	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z)  < 1 ) return 0;
    break;
  case 40:  /*height - double*/
    e_h_ok=TRUE;
    if ( sscanf ( p1 , "%lf", &e_h)  < 1 ) return 0;
    break;
  case 1 :  /*text value*/ 
	  
    strcpy(buf1_1,buf1);
    break;
  case 41:  /*relative X-scale factor - double*/
    e_wspx_ok=TRUE;
    if ( sscanf ( p1 , "%lf", &e_wspx)  < 1 ) return 0;
    break;  
  case 50:  /*rotation angle - double*/
    if ( sscanf ( p1 , "%lf", &e_k)  < 1 ) return 0;
    break; 
  case 51:  /*oblique angle - double*/
    e_obk_ok=TRUE;
    if ( sscanf ( p1 , "%lf", &e_obk)  < 1 ) return 0;
    break;    
  case 7 :  /*text style name*/
    e_font=find_font(buf1,&e_wspx, &e_wspx0, e_wspx_ok,e_h,e_h_ok,e_obk,e_obk_ok);
    break;   
  case 71:  /*text generation flags*/  
    if ( sscanf( p1 , "%d", &e_flags) < 1 ) return 0;
    break;
  case 72:  /*horizontal justification type*/  
    if ( sscanf( p1 , "%d", &e_hj) < 1 ) return 0;
    break;  
  case 73:  /*vertical justification type*/  
    if ( sscanf( p1 , "%d", &e_vj) < 1 ) return 0;
    break;    
  case 11:  /*alignment point X - double*/
    if ( sscanf ( p1 , "%lf", &e_ax)  < 1 ) return 0;
    e_ax_exists=TRUE;
    break;
  case 21:  /*alignment point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_ay)  < 1 ) return 0;
    break;
  case 31:  /*alignment point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_az)  < 1 ) return 0;
    break;  
  default:  break;  
  } /*switch*/
 }
 return 1;
}


BOOL read_insert(FILE *f,int to_block, BOOL block)
/*----------------------------------------------*/
{ BOOL endentitie, seqend, attrib;
  char *p, *p1;
  int entitie_code;
  unsigned char e_layer=0;
  unsigned char e_type=64;
  //char e_line_type[30];
  double e_elevation=0;
  int e_color=7;
  int e_attrib_flag=0;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_xp, e_yp, e_zp=0;
  double e_xp1, e_yp1, e_zp1;
  double e_angle=0;
  double e_column_spacing=0, e_row_spacing=0;
  char buf[MAXLINE], buf1[MAXLINE];
  INSERT I=InsertDef;

 e_extrusion_x=0;
 e_extrusion_y=0;
 e_extrusion_z=1;
 ep_space=0;
 ep_novisibility = 0;
 ep_width=line_width.normal*100;
 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

    p1=buf1;
  }  
  
  
  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
    endentitie=TRUE;
    /*generowanie linii*/
    e_xp1=jednostkiOb(e_xp - EXTMIN_X);
    e_yp1=jednostkiOb(e_yp - EXTMIN_Y);
    e_zp1=jednostkiOb(e_zp);
    
    I.x=e_xp1;
    I.y=e_yp1;
    I.z=e_zp1;
    
    I.type=e_type;
    I.color=e_color; 
    I.layer=e_layer;
    I.rotation_angle=(e_angle / 360) * Pi2;
    I.point_x_x=jednostkiOb(-EXTMIN_X);
    I.point_y_y=jednostkiOb(-EXTMIN_Y);
    I.m_x=0;
    I.m_y=0;
    I.m_z=0;  
    I.typ=0;
    I.useri1=USERI1;
    I.column_spacing=jednostkiOb(e_column_spacing);
    I.row_spacing=jednostkiOb(e_row_spacing);
    I.ep_space=ep_space;
    /*kopiowanie bloku*/
    Insert_Block (&I,to_block, block);
    /*jezeli flaga 66 == 1 to czytane sa ATTRIB az do SEQEND*/
    if ((e_attrib_flag==1) && (ADP!=NULL) && (ADK!=NULL))
     {
     seqend=FALSE;
     attrib=FALSE;
     
     while (seqend==FALSE)
      {
       if (entitie_code!=0)
        {
          endentitie=FALSE;
          while (endentitie==FALSE)
          {
			  if (!inc_nr_linii()) return 0;
           if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
           p=buf;
           if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
           if (entitie_code!=0)
            {
			   if (!inc_nr_linii()) return 0;
             if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
             p1=buf1;
            }  
            else endentitie=TRUE;
          }  
        }
	   if (!inc_nr_linii()) return 0;
       if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
       p1=buf1; 
       
       if (strcmp(buf1,"SEQEND")==0)
        {
          seqend=TRUE;
        }  
       
       if (strcmp(buf1,"ATTRIB")==0) 
        {
          if (!read_attrib_insert(f, block)) return 0;
          entitie_code=0;
          goto next_entitie2;
        }
	   if (!inc_nr_linii()) return 0;
        if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
        p=buf;
        if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
        next_entitie2:
        ;
      }
     }
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_type = Layers[e_layer].line_type;
		  ep_width = get_width(Layers[e_layer].line_type);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_type = Layers_DXF[e_layer].line_type;
		  ep_width = get_width(Layers_DXF[e_layer].line_type);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    e_type=find_type(buf1,e_type);
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  check_width_7(p1, &ep_width);
	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_xp)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_yp)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_zp)  < 1 ) return 0;
    break;
  case 41: if ( sscanf ( p1 , "%lf", &I.x_scale)  < 1 ) return 0; 
    break;
  case 42: if ( sscanf ( p1 , "%lf", &I.y_scale)  < 1 ) return 0; 
    break;
  case 43: if ( sscanf ( p1 , "%lf", &I.z_scale)  < 1 ) return 0; 
    break;
  case 50: if ( sscanf ( p1 , "%lf", &e_angle)  < 1 ) return 0; 
    break;
  case 70: if ( sscanf ( p1 , "%d", &I.column_count)  < 1 ) return 0;
    break;
  case 71: if ( sscanf ( p1 , "%d", &I.row_count)  < 1 ) return 0;
    break;
  case 44: if ( sscanf ( p1 , "%lf", &e_column_spacing)  < 1 ) return 0; 
    break;
  case 45: if ( sscanf ( p1 , "%lf", &e_row_spacing)  < 1 ) return 0; 
    break;
  case 66 : /*attributes follow flag*/ 	    
           if ( sscanf ( p1 , "%d", &e_attrib_flag)  < 1 ) return 0;
    break;
  case 2: if (USERI1==211) strcpy(I.block_name,""); 
             else  strcpy(I.block_name," ");
          strcat(I.block_name,buf1);
          if (!is_utf8(I.block_name))  decodingdxf(I.block_name);
    break;
  default:  break;  
  } /*switch*/
 }
 return 1;
}

BOOL read_dimension(FILE *f,int to_block, BOOL block)
/*-------------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned char e_layer=0;
  unsigned char e_type=64;
  double e_elevation=0;
  int e_color=7;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_xp, e_yp, e_zp;
  double move_x1, move_y1, move_z1;
  double e_xp1, e_yp1, e_zp1;
  double e_angle=0;
  char buf[MAXLINE], buf1[MAXLINE];
  char dimension_text[MAXLINE];
  int dimension_type;
  INSERT I=InsertDef;
  int to_block1;

 
  I.dim = TRUE;

 e_extrusion_x=0;
 e_extrusion_y=0;
 e_extrusion_z=1;
 ep_space=0;
 ep_novisibility = 0;
 ep_width=line_width.normal*100;
 move_x = 0;
 move_y = 0; 
 move_z = 0;
 to_block1=to_block;
 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

    p1=buf1;
  }  
  
  
  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
    endentitie=TRUE;
    /*generowanie linii*/
    
    e_xp1=jednostkiOb(e_xp - EXTMIN_X);
    e_yp1=jednostkiOb(e_yp - EXTMIN_Y);
    e_zp1=jednostkiOb(e_zp); 
    
    I.x=e_xp1;
    I.y=e_yp1;
    I.z=e_zp1; 
    
    I.type=e_type;
    I.color=e_color; 
    I.layer=e_layer; 
    I.rotation_angle=(e_angle / 360) * Pi2;
    
    I.point_x_x=jednostkiOb(-EXTMIN_X);
    I.point_y_y=jednostkiOb(-EXTMIN_Y);
    
    move_x1=jednostkiOb(move_x);
    move_y1=jednostkiOb(move_y);
    move_z1=jednostkiOb(move_z); 
    
    I.m_x=move_x1;
    I.m_y=move_y1;
    I.m_z=move_z1;

    I.typ=1;
    /*kopiowanie bloku*/
    I.useri1=USERI1;
    I.ep_space=ep_space;
    Insert_Block (&I,to_block1, block);
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_type = Layers[e_layer].line_type;
		  ep_width = get_width(Layers[e_layer].line_type);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_type = Layers_DXF[e_layer].line_type;
		  ep_width = get_width(Layers_DXF[e_layer].line_type);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    e_type=find_type(buf1,e_type);
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  check_width_7(p1, &ep_width);
	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_xp)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_yp)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_zp)  < 1 ) return 0;
    break;  
  case 12:  /*move vector X - double*/
    if ( sscanf ( p1 , "%lf", &move_x)  < 1 ) return 0;
    break;
  case 22:  /*move vector Y - double*/
    if ( sscanf ( p1 , "%lf", &move_y)  < 1 ) return 0;
    break;
  case 32:  /*move vector Z - double*/
    if ( sscanf ( p1 , "%lf", &move_z)  < 1 ) return 0;
    break;
  case 41: if ( sscanf ( p1 , "%lf", &I.x_scale)  < 1 ) return 0; 
    break;
  case 42: if ( sscanf ( p1 , "%lf", &I.y_scale)  < 1 ) return 0; 
    break;
  case 43: if ( sscanf ( p1 , "%lf", &I.z_scale)  < 1 ) return 0; 
    break;
  case 50: if ( sscanf ( p1 , "%lf", &e_angle)  < 1 ) return 0; 
    break;
  case 70: if ( sscanf ( p1 , "%d", &dimension_type)  < 1 ) return 0;
    break;
  case  1: strcpy(dimension_text,buf1);
    break;
  case 44: if ( sscanf ( p1 , "%lf", &I.column_spacing)  < 1 ) return 0; 
    break;
  case 45: if ( sscanf ( p1 , "%lf", &I.row_spacing)  < 1 ) return 0; 
    break;
  case 2: if (USERI1==211) strcpy(I.block_name,""); 
            else strcpy(I.block_name," ");
            strcat(I.block_name,buf1);
          if (!is_utf8(I.block_name))  decodingdxf(I.block_name);
    break;
  default:  break;  
  } /*switch*/
 }
 return 1;
}

BOOL read_leader(FILE *f,int to_block, BOOL block)
/*----------------------------------------------*/
{ BOOL endentitie;
    char *p, *p1;
    int entitie_code;
    unsigned char e_layer=0;
    unsigned char e_type=64;
    double e_elevation=0;
    int e_color=7;
    double e_extrusion_x, e_extrusion_y, e_extrusion_z;
    double e_xp, e_yp, e_zp;
    char buf[MAXLINE], buf1[MAXLINE];
    char dim_style[MAXLINE];
    LINIA L=Ldef;
    TEXT T=Tdef;
    BOOL ret123;
    char obiektt1 = 0, obiektt2 = 0, obiektt3 = 0;
    int arrow_flag, path_type, creation_flag, direction_flag, hookline_flag;
    double horizontal_direction_x, horizontal_direction_y, horizontal_direction_z, insert_offset_x, insert_offset_y, insert_offset_z, annotation_offset_x, annotation_offset_y, annotation_offset_z, vertices_in_leader=0;
    char hard_reference[MAXLINE];
    POINTD *vertices;
    int vertex_no_max=10;
    int vertex_no=0;
    char annotation[MAXLINE]="";

    leaders_no++;

    //primary allocation
    vertices=(POINTD*)malloc(sizeof(POINTD)*vertex_no_max);

    e_extrusion_x=0;
    e_extrusion_y=0;
    e_extrusion_z=1;
    ep_space=0;
    ep_layer=0;
    e_layer=0;
    ep_type=64;
    e_type=64;
    ep_color=7;
    ep_novisibility = 0;
    ep_width=line_width.normal*100;
    endentitie=FALSE;
    while (endentitie==FALSE)
    {
        if (!inc_nr_linii()) return 0;
        if ( myfgets(buf , MAXLINE , f ) == NULL ) goto error_l;

        p=buf;

        if ( sscanf ( p , "%d", &entitie_code)  < 1 ) goto error_l;

        if (entitie_code!=0)
        {
            if (!inc_nr_linii()) return 0;
            if ( myfgets(buf1 , MAXLINE , f ) == NULL ) goto error_l;

            p1=buf1;
        }

        switch (entitie_code)
        {
            case 0:

                if (ep_novisibility)
                {
                    return 1;
                }

                endentitie=TRUE;
                /*generating of the leader*/

                L.typ=get_typ(e_type, ep_width);

                L.kolor=e_color;
                L.warstwa=e_layer;
                L.przec=ep_space;

                T.kolor=e_color;
                T.warstwa=e_layer;
                T.przec=ep_space;
                if (text_annotation_height>0.0) T.wysokosc=text_annotation_height;
                if (text_annotation_width>0.0) T.width_factor=text_annotation_width;

                strcpy(T.text, annotation);

                generate_leader(vertices, vertex_no, &L, &T, to_block, -1, FALSE, 0,0, last_mtext_h);  //line_no=-1 is always for leader

                free(vertices);
                return 1;
                break;
            case 8:  /*layer - sring*/
                if (!block)
                {
                    e_layer = find_layer(buf1);
                    e_type = Layers[e_layer].line_type;
                    ep_width = get_width(Layers[e_layer].line_type);
                    e_color = Layers[e_layer].color;
                }
                else
                {
                    e_layer = find_layer_DXF(buf1);
                    e_type = Layers_DXF[e_layer].line_type;
                    ep_width = get_width(Layers_DXF[e_layer].line_type);
                    e_color = Layers_DXF[e_layer].color;
                }
                break;
            case 6:  /*line type - string*/
                e_type=find_type(buf1,e_type);
                break;
            case 38:  /*elevation - double*/
                if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) goto error_l;
                break;
            case 62:  /*color number - int*/
                if ( sscanf ( p1 , "%d", &e_color)  < 1 ) goto error_l;
                e_color=GetColorALF(e_color, e_layer);
                break;
            case 67:  /*model (0) /paper (1) space - int*/
                if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) goto error_l;
                break;
            case 60:  /*novisibility*/
                if (sscanf(p1, "%d", &ep_novisibility) < 1) goto error_l;
                break;
            case 370: /*line width*/
                if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) goto error_l;
                if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
                else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
                else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
                break;
            case 999: /*comment*/
                ret123=check_width_7(p1, &ep_width);
                if (!ret123)
                {
                    ret123 = check_dim_line(p1, &obiektt1, &obiektt2, &obiektt3);
                }
                break;
            case  3: strcpy(dim_style,buf1);
                break;
            case 71:  /*arrow flag*/
                if (sscanf(p1, "%d", &arrow_flag) < 1) goto error_l;
                break;
            case 72:  /*path_type*/
                if (sscanf(p1, "%d", &path_type) < 1) goto error_l;
                break;
            case 73:  /*creation flag*/
                if (sscanf(p1, "%d", &creation_flag) < 1) goto error_l;
                break;
            case 74:  /*direction flag*/
                if (sscanf(p1, "%d", &direction_flag) < 1) goto error_l;
                break;
            case 75:  /*hookline flag*/
                if (sscanf(p1, "%d", &hookline_flag) < 1) goto error_l;
                break;
            case 76:  /*vertices in leader*/
                if (sscanf(p1, "%d", &vertices_in_leader) < 1) goto error_l;
                break;
            case 40:  /*text annotation height*/
                if ( sscanf ( p1 , "%lf", &text_annotation_height)  < 1 ) goto error_l;
                break;
            case 41:  /*text annotation width*/
                if ( sscanf ( p1 , "%lf", &text_annotation_width)  < 1 ) goto error_l;
                break;
            case 10:  /*vertex X - double*/
                if ( sscanf ( p1 , "%lf", &e_xp)  < 1 ) goto error_l;
                vertex_no++;
                if (vertex_no>=vertex_no_max)
                {
                    vertex_no_max+=10;
                    vertices=(POINTD*)realloc(vertices, sizeof(POINTD)*vertex_no_max);
                }
                vertices[vertex_no-1].x=jednostkiOb(e_xp - EXTMIN_X);
                break;
            case 20:  /*vertex Y - double*/
                if ( sscanf ( p1 , "%lf", &e_yp)  < 1 ) goto error_l;
                vertices[vertex_no-1].y=jednostkiOb(e_yp - EXTMIN_Y);
                break;
            case 30:  /*vertex Z - ignored - double*/
                if ( sscanf ( p1 , "%lf", &e_zp)  < 1 ) goto error_l;
                break;
            case 77:  /*color number - int   if DIMCLRD == BYBLOCK*/
                if ( sscanf ( p1 , "%d", &e_color)  < 1 ) goto error_l;
                e_color=GetColorALF(e_color, e_layer);
                break;
            case  340:
                strcpy(hard_reference,buf1);
                break;
            case 210:  /*extrusion direction X - double*/
                if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) goto error_l;
                break;
            case 220:  /*extrusion direction Y - double*/
                if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) goto error_l;
                break;
            case 230:  /*extrusion direction Z - double*/
                if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) goto error_l;
                break;
            case 211:  /*horizontal direction X - double*/
                if ( sscanf ( p1 , "%lf", &horizontal_direction_x)  < 1 ) goto error_l;
                break;
            case 221:  /*horizontal direction Y - double*/
                if ( sscanf ( p1 , "%lf", &horizontal_direction_y)  < 1 ) goto error_l;
                break;
            case 231:  /*horizontal direction Z - double*/
                if ( sscanf ( p1 , "%lf", &horizontal_direction_z)  < 1 ) goto error_l;
                break;
            case 212:  /*insert offset X - double*/
                if ( sscanf ( p1 , "%lf", &insert_offset_x)  < 1 ) goto error_l;
                break;
            case 222:  /*insert offset Y - double*/
                if ( sscanf ( p1 , "%lf", &insert_offset_y)  < 1 ) goto error_l;
                break;
            case 232:  /*insert offset Z - double*/
                if ( sscanf ( p1 , "%lf", &insert_offset_z)  < 1 ) goto error_l;
                break;
            case 213:  /*annotation offset X - double*/
                if ( sscanf ( p1 , "%lf", &annotation_offset_x)  < 1 ) goto error_l;
                break;
            case 223:  /*annotation offset Y - double*/
                if ( sscanf ( p1 , "%lf", &annotation_offset_y)  < 1 ) goto error_l;
                break;
            case 233:  /*annotation offset Z - double*/
                if ( sscanf ( p1 , "%lf", &annotation_offset_z)  < 1 ) goto error_l;
                break;
            default:  break;
        } /*switch*/
    }
    free(vertices);
    return 1;

error_l:
    free(vertices);
    return 0;
}

BOOL read_context_leader_line(FILE *f,int to_block, BOOL block, int line_no, BOOL has_set_ll, double ll_xp, double ll_yp)
{
    BOOL endcontextleaderline;
    char *p, *p1;
    int context_code;
    char buf[MAXLINE], buf1[MAXLINE];
    POINTD *vertices;
    int vertex_no_max=10;
    int vertex_no=0;
    LINIA L=Ldef;
    TEXT T=Tdef;
    double e_xp, e_yp, e_zp;

    //primary allocation
    vertices=(POINTD*)malloc(sizeof(POINTD)*vertex_no_max);

    if (has_set_ll==TRUE)
    {
        vertex_no++;
        vertices[vertex_no-1].x=jednostkiOb(ll_xp - EXTMIN_X);
        vertices[vertex_no-1].y=jednostkiOb(ll_yp - EXTMIN_Y);
    }

    endcontextleaderline=FALSE;
    while (endcontextleaderline==FALSE) {
        if (!inc_nr_linii()) return 0;
        if (myfgets(buf, MAXLINE, f) == NULL) goto error_cll;

        p = buf;

        if (sscanf(p, "%d", &context_code) < 1) goto error_cll;

        if (!inc_nr_linii()) return 0;
        if (myfgets(buf1, MAXLINE, f) == NULL) goto error_cll;

        p1 = buf1;

        switch (context_code) {
            case 305:
                /*generating of the leader*/

                L.typ=get_typ(ep_type, ep_width);

                L.kolor=ep_color;
                L.warstwa=ep_layer;
                L.przec=ep_space;

                T.kolor=ep_color;
                T.warstwa=ep_layer;
                T.przec=ep_space;
                if (line_no==0) strcpy(T.text, annotation);
                else strcpy(T.text, "");

                if (text_annotation_height>0.0) T.wysokosc=text_annotation_height;
                if (text_annotation_width>0.0) T.width_factor=text_annotation_width;

                generate_leader(vertices, vertex_no, &L, &T, to_block, line_no, mleader_text_location, et_x, et_y, last_mtext_h);

                free(vertices);

                endcontextleaderline = TRUE;
                return 1;
                break;
            case 10:  /*vertex X - double*/
                if ( sscanf ( p1 , "%lf", &e_xp)  < 1 ) goto error_cll;
                vertex_no++;
                if (vertex_no>=vertex_no_max)
                {
                    vertex_no_max+=10;
                    vertices=(POINTD*)realloc(vertices, sizeof(POINTD)*vertex_no_max);
                }
                vertices[vertex_no-1].x=jednostkiOb(e_xp - EXTMIN_X);
                break;
            case 20:  /*vertex Y - double*/
                if ( sscanf ( p1 , "%lf", &e_yp)  < 1 ) goto error_cll;
                vertices[vertex_no-1].y=jednostkiOb(e_yp - EXTMIN_Y);
                break;
            case 30:  /*vertex Z - ignored - double*/
                if ( sscanf ( p1 , "%lf", &e_zp)  < 1 ) goto error_cll;
                break;
            default:
                break;
        }
    }

    free(vertices);
    return 1;
    error_cll:
    free(vertices);
    return 0;
}


BOOL read_context_leader(FILE *f,int to_block, BOOL block)
{
    BOOL endcontextleader;
    char *p, *p1;
    int context_code;
    char buf[MAXLINE], buf1[MAXLINE];
    int line_no=0;
    BOOL has_set_ll=FALSE;
    double ll_xp=0.0, ll_yp=0.0, ll_zp=0.0;


    endcontextleader=FALSE;
    while (endcontextleader==FALSE) {
        if (!inc_nr_linii()) return 0;
        if (myfgets(buf, MAXLINE, f) == NULL) goto error_cl;

        p = buf;

        if (sscanf(p, "%d", &context_code) < 1) goto error_cl;

        if (!inc_nr_linii()) return 0;
        if (myfgets(buf1, MAXLINE, f) == NULL) goto error_cl;

        p1 = buf1;

        switch (context_code) {
            case 90: /*Has Set Last Leader Line Point*/
                if ( sscanf ( p1 , "%d", &has_set_ll)  < 1 ) goto error_cl;
                break;
            case 10:  /*last leader line point X*/
                if ( sscanf ( p1 , "%lf", &ll_xp)  < 1 ) goto error_cl;
                break;
            case 20:  /*last leader line point Y*/
                if ( sscanf ( p1 , "%lf", &ll_yp)  < 1 ) goto error_cl;
                break;
            case 30:  /*last leader line point Z*/
                if ( sscanf ( p1 , "%lf", &ll_zp)  < 1 ) goto error_cl;
                break;
            case 303:
                endcontextleader = TRUE;
                break;
            case 304:  //leader_line
                if (strncmp(p1, "LEADER_LINE", 11) == 0) {
                    read_context_leader_line(f, to_block, block, line_no, has_set_ll, ll_xp, ll_yp);
                    line_no++;
                }
                break;
            default:
                break;
        }
    }

    return 1;
    error_cl:
    return 0;
}

BOOL read_context_data(FILE *f,int to_block, BOOL block)
{
    BOOL endcontext;
    char *p, *p1;
    int context_code;
    char buf[MAXLINE], buf1[MAXLINE];
    double context_scale;

    strcpy(annotation, "");

    text_annotation_height=3.0;
    text_annotation_width=1.0;
    text_annotation_rotation=0.0;
    text_annotation_spacing=1.0;

    mleader_text_location=FALSE;

    endcontext=FALSE;
    while (endcontext==FALSE) {
        if (!inc_nr_linii()) return 0;
        if (myfgets(buf, MAXLINE, f) == NULL) goto error_c;

        p = buf;

        if (sscanf(p, "%d", &context_code) < 1) goto error_c;

        if (!inc_nr_linii()) return 0;
        if (myfgets(buf1, MAXLINE, f) == NULL) goto error_c;

        p1 = buf1;

        switch (context_code) {
            case 301:
                endcontext = TRUE;
                break;
            case 40:  /*context scale*/
                if ( sscanf ( p1 , "%lf", &context_scale)  < 1 ) goto error_c;
                break;
            case 41:  /*text annotation height*/
                if ( sscanf ( p1 , "%lf", &text_annotation_height)  < 1 ) goto error_c;
                break;
            case 42:  /*text annotation rotation*/
                if ( sscanf ( p1 , "%lf", &text_annotation_rotation)  < 1 ) goto error_c;
                break;
            case 43:  /*text annotation width*/
                if ( sscanf ( p1 , "%lf", &text_annotation_width)  < 1 ) goto error_c;
                break;
            case 45:  /*text annotation spacing factor*/
                if ( sscanf ( p1 , "%lf", &text_annotation_spacing)  < 1 ) goto error_c;
                break;
            case 302:  //leader
                if (strncmp(p1, "LEADER", 6) == 0)
                    read_context_leader(f, to_block, block);
                break;
            case 304:
                strcpy(annotation, p1);
                break;
            case 12:
                if ( sscanf ( p1 , "%lf", &et_x)  < 1 ) goto error_c;
                et_x=jednostkiOb(et_x - EXTMIN_X);
                break;
            case 22:
                if ( sscanf ( p1 , "%lf", &et_y)  < 1 ) goto error_c;
                et_y=jednostkiOb(et_y - EXTMIN_Y);
                mleader_text_location=TRUE;
                break;
            case 32:
                if ( sscanf ( p1 , "%lf", &et_z)  < 1 ) goto error_c;
                break;
            default:
                break;
        }
    }

    return 1;
error_c:
    return 0;
}

BOOL read_multileader(FILE *f,int to_block, BOOL block)
/*----------------------------------------------------*/
{ BOOL endentitie;
    char *p, *p1;
    int entitie_code;
    double e_elevation=0;
    double e_extrusion_x, e_extrusion_y, e_extrusion_z;
    char buf[MAXLINE], buf1[MAXLINE];
    BOOL ret123;
    char obiektt1 = 0, obiektt2 = 0, obiektt3 = 0;
    double vertices_in_leader = 0;

    int ret;

    mleaders_no++;

    e_extrusion_x=0;
    e_extrusion_y=0;
    e_extrusion_z=1;
    ep_space=0;
    ep_layer=0;
    ep_type=64;
    ep_color=7;
    ep_novisibility = 0;
    ep_width=line_width.normal*100;
    endentitie=FALSE;
    while (endentitie==FALSE)
    {
        if (!inc_nr_linii()) return 0;
        if ( myfgets(buf , MAXLINE , f ) == NULL ) goto error_l;

        p=buf;

        if ( sscanf ( p , "%d", &entitie_code)  < 1 ) goto error_l;


        if (entitie_code!=0)
        {
            if (!inc_nr_linii()) return 0;
            if ( myfgets(buf1 , MAXLINE , f ) == NULL ) goto error_l;

            p1=buf1;
        }

        switch (entitie_code)
        {
            case 0:

                if (ep_novisibility)
                {
                    return 1;
                }

                endentitie=TRUE;

                return 1;
                break;
            case 8:  /*layer - sring*/
                if (!block)
                {
                    ep_layer = find_layer(buf1);
                    ep_type = Layers[ep_layer].line_type;
                    ep_width = get_width(Layers[ep_layer].line_type);
                    ep_color = Layers[ep_layer].color;
                }
                else
                {
                    ep_layer = find_layer_DXF(buf1);
                    ep_type = Layers_DXF[ep_layer].line_type;
                    ep_width = get_width(Layers_DXF[ep_layer].line_type);
                    ep_color = Layers_DXF[ep_layer].color;
                }
                break;
            case 6:  /*line type - string*/
                ep_type=find_type(buf1,ep_type);
                break;
            case 38:  /*elevation - double*/
                if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) goto error_l;
                break;
            case 62:  /*color number - int*/
                if ( sscanf ( p1 , "%d", &ep_color)  < 1 ) goto error_l;
                ep_color=GetColorALF(ep_color, ep_layer);
                break;
            case 67:  /*model (0) /paper (1) space - int*/
                if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) goto error_l;
                break;
            case 60:  /*novisibility*/
                if (sscanf(p1, "%d", &ep_novisibility) < 1) goto error_l;
                break;
            case 370: /*line width*/
                if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) goto error_l;
                if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
                else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
                else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
                break;
            case 999: /*comment*/
                ret123=check_width_7(p1, &ep_width);
                if (!ret123)
                {
                    ret123 = check_dim_line(p1, &obiektt1, &obiektt2, &obiektt3);
                }
                break;
            case  300:
                if (strncmp(p1,"CONTEXT_DATA",12)==0)
                    ret=read_context_data(f, to_block, block);
                break;
#ifdef BLAHBLAHBLAH
            case  3: strcpy(dim_style,buf1);
                break;
            case 71:  /*arrow flag*/
                if (sscanf(p1, "%d", &arrow_flag) < 1) goto error_l;
                break;
            case 72:  /*path_type*/
                if (sscanf(p1, "%d", &path_type) < 1) goto error_l;
                break;
            case 73:  /*creation flag*/
                if (sscanf(p1, "%d", &creation_flag) < 1) goto error_l;
                break;
            case 74:  /*direction flag*/
                if (sscanf(p1, "%d", &direction_flag) < 1) goto error_l;
                break;
            case 75:  /*hookline flag*/
                if (sscanf(p1, "%d", &hookline_flag) < 1) goto error_l;
                break;
            case 76:  /*vertices in leader*/
                if (sscanf(p1, "%d", &vertices_in_leader) < 1) goto error_l;
                break;
            case 40:  /*text annotation height*/
                if ( sscanf ( p1 , "%lf", &text_annotation_height)  < 1 ) goto error_l;
                break;
            case 41:  /*text annotation width*/
                if ( sscanf ( p1 , "%lf", &text_annotation_width)  < 1 ) goto error_l;
                break;
            case 10:  /*vertex X - double*/
                if ( sscanf ( p1 , "%lf", &e_xp)  < 1 ) goto error_l;
                vertex_no++;
                if (vertex_no>=vertex_no_max)
                {
                    vertex_no_max+=10;
                    vertices=(POINTD*)realloc(vertices, sizeof(POINTD)*vertex_no_max);
                }
                vertices[vertex_no-1].x=jednostkiOb(e_xp - EXTMIN_X);
                break;
            case 20:  /*vertex Y - double*/
                if ( sscanf ( p1 , "%lf", &e_yp)  < 1 ) goto error_l;
                vertices[vertex_no-1].y=jednostkiOb(e_yp - EXTMIN_Y);
                break;
            case 30:  /*vertex Z - ignored - double*/
                if ( sscanf ( p1 , "%lf", &e_zp)  < 1 ) goto error_l;
                break;
            case 77:  /*color number - int   if DIMCLRD == BYBLOCK*/
                if ( sscanf ( p1 , "%d", &e_color)  < 1 ) goto error_l;
                e_color=GetColorALF(e_color, e_layer);
                break;
            case  340:
                strcpy(hard_reference,buf1);
                break;
            case 210:  /*extrusion direction X - double*/
                if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) goto error_l;
                break;
            case 220:  /*extrusion direction Y - double*/
                if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) goto error_l;
                break;
            case 230:  /*extrusion direction Z - double*/
                if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) goto error_l;
                break;
            case 211:  /*horizontal direction X - double*/
                if ( sscanf ( p1 , "%lf", &horizontal_direction_x)  < 1 ) goto error_l;
                break;
            case 221:  /*horizontal direction Y - double*/
                if ( sscanf ( p1 , "%lf", &horizontal_direction_y)  < 1 ) goto error_l;
                break;
            case 231:  /*horizontal direction Z - double*/
                if ( sscanf ( p1 , "%lf", &horizontal_direction_z)  < 1 ) goto error_l;
                break;
            case 212:  /*insert offset X - double*/
                if ( sscanf ( p1 , "%lf", &insert_offset_x)  < 1 ) goto error_l;
                break;
            case 222:  /*insert offset Y - double*/
                if ( sscanf ( p1 , "%lf", &insert_offset_y)  < 1 ) goto error_l;
                break;
            case 232:  /*insert offset Z - double*/
                if ( sscanf ( p1 , "%lf", &insert_offset_z)  < 1 ) goto error_l;
                break;
            case 213:  /*annotation offset X - double*/
                if ( sscanf ( p1 , "%lf", &annotation_offset_x)  < 1 ) goto error_l;
                break;
            case 223:  /*annotation offset Y - double*/
                if ( sscanf ( p1 , "%lf", &annotation_offset_y)  < 1 ) goto error_l;
                break;
            case 233:  /*annotation offset Z - double*/
                if ( sscanf ( p1 , "%lf", &annotation_offset_z)  < 1 ) goto error_l;
                break;
#endif
            default:  break;
        } /*switch*/
    }
    return 1;

    error_l:
    return 0;
}


BOOL read_shape(FILE *f,int to_block, BOOL block)
{
 ep_space=0;
 return 1;   //win32   return
}

int datahex(char* string, char *data_buffer, int data_index) {

    if(string == NULL)
        return NULL;

    size_t slength = strlen(string);
    if((slength % 2) != 0) // must be even
        return 0;

    size_t dlength = slength / 2;

    size_t index = 0;
    while (index < slength) {
        char c = string[index];
        int value = 0;
        if(c >= '0' && c <= '9')
            value = (c - '0');
        else if (c >= 'A' && c <= 'F')
            value = (10 + (c - 'A'));
        else if (c >= 'a' && c <= 'f')
            value = (10 + (c - 'a'));
        else {
            return 0;
        }

        data_buffer[data_index+(index/2)] += value << (((index + 1) % 2) * 4);

        index++;
    }

    return data_index+dlength;
}

typedef unsigned char ByteData;

ByteData HexChar (char c)
{
    if ('0' <= c && c <= '9') return (ByteData)(c - '0');
    if ('A' <= c && c <= 'F') return (ByteData)(c - 'A' + 10);
    if ('a' <= c && c <= 'f') return (ByteData)(c - 'a' + 10);
    return (ByteData)(-1);
}

ssize_t HexToBin (const char* s, ByteData * buff, ssize_t length)
{
    ssize_t result = 0;
    if (!s || !buff || length <= 0) return -2;

    while (*s)
    {
        ByteData nib1 = HexChar(*s++);
        if ((signed)nib1 < 0) return -3;
        ByteData nib2 = HexChar(*s++);
        if ((signed)nib2 < 0) return -4;

        ByteData bin = (nib1 << 4) + nib2;

        if (length-- <= 0) return -5;
        *buff++ = bin;
        ++result;
    }
    return result;
}


BOOL read_ole2frame(FILE *f,int to_block, BOOL block)
{
    BOOL endentitie;
    char *p, *p1;
    int entitie_code;
    char buf[MAXLINE], buf1[MAXLINE];
    int binary_length=0;
    double ULx, ULy, BRx, BRy;
    int ole_object_type;
    int ole_object_space;
    BOOL end_of_ole=FALSE;
    char ole_name[MAXLINE];
    char *ole=NULL;
    int ole_index=0;
    ssize_t ret;

    ep_space=0;

    endentitie=FALSE;
    while ((endentitie==FALSE) && (end_of_ole==FALSE))
    {
        if (!inc_nr_linii()) return 0;
        if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

        p=buf;

        if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;


        if (entitie_code!=0)
        {
            if (!inc_nr_linii()) return 0;
            if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

            p1=buf1;
        }


        switch (entitie_code)
        {
            case 0:
                if (ep_novisibility)
                {
                    return 1;
                }
                endentitie=TRUE;
                //checking binaries and maybe building pcx object

            break;
            case 3:
                strcpy(ole_name,p1);
                break;
            case 10:
                if ( sscanf ( p1 , "%lf", &ULx)  < 1 ) return 0;
                break;
            case 20:
                if ( sscanf ( p1 , "%lf", &ULy)  < 1 ) return 0;
                break;
            case 11:
                if ( sscanf ( p1 , "%lf", &BRx)  < 1 ) return 0;
                break;
            case 21:
                if ( sscanf ( p1 , "%lf", &BRy)  < 1 ) return 0;
                break;
            case 71:
                if ( sscanf ( p1 , "%ld", &ole_object_type)  < 1 ) return 0;
                break;
            case 72:
                if ( sscanf ( p1 , "%ld", &ole_object_space)  < 1 ) return 0;
                break;
            case 90:
                if ( sscanf ( p1 , "%d", &binary_length)  < 1 ) return 0;
                ole=(char *)malloc(binary_length);
                break;
            case 310:
                ret = HexToBin (p1, (ByteData *)(ole+ole_index), strlen(p1));
                if (ret<0) end_of_ole=TRUE;
                else ole_index+=ret;
                break;
            case 1:
                end_of_ole=TRUE;
                break;
            default:  break;
        } /*switch*/
    }

    if (ole!=NULL) {
        FILE *fole;
        fole = fopen("ole.xxx", "wb");
        if (fole!=NULL)
        {
            fwrite(ole, binary_length, 1, fole);
            fclose(fole);
        }
        free(ole);
    }
    return entitie_code;
}


BOOL read_attdef(FILE *f,int to_block, BOOL block)
/*----------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned int e_layer=0, e_font=0, e_flags=0, e_hj=0, e_vj=0;
  double e_elevation=0;
  int e_color=7;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_x, e_y, e_z, e_h=2, e_wspx=1, e_wspx0 = 1, e_wspxk=1, e_k=0, e_obk=0, e_ax, e_ay, e_az;
  double e_x1, e_y1;
  double e_h1;
  double e_k1, e_obk1;
  char buf[MAXLINE], buf1[MAXLINE], buf1_1[MAXLINE];
  TEXT t=Tdef;
  BOOL e_wspx_ok=FALSE, e_h_ok=FALSE, e_obk_ok=FALSE;
  double del_xt, del_yt, t_x, t_y;
  double e_ax1, e_ay1;
  BOOL e_ax_exists=FALSE;

 e_extrusion_x=0;
 e_extrusion_y=0;
 e_extrusion_z=1;
 ep_space=0;
 ep_novisibility = 0;
 ep_width=line_width.normal*100;
 strcpy(buf1,"STANDARD");
 e_font = 0;
 e_h=TEXTSIZE;
 
 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

    p1=buf1;
  }  

  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
    endentitie=TRUE;
    /*generowanie linii*/
    e_x1=jednostkiOb(e_x - EXTMIN_X);
    e_y1=jednostkiOb(e_y - EXTMIN_Y);
    e_h1=jednostkiOb(e_h);
    e_k1=e_k*Pi2/360;
    e_obk1=e_obk*Pi2/360;

    special_chars(buf1_1);

          if (!is_utf8(buf1_1))  decodingdxf(buf1_1);

    strcpy(t.text,buf1_1);

    if ((((e_vj!=0) || (e_hj!=0))) && (e_ax_exists))
          {
        
              e_ax1 = jednostkiOb(e_ax - EXTMIN_X);
              e_ay1 = jednostkiOb(e_ay - EXTMIN_Y);

              t.x = e_ax1;
              t.y = e_ay1;
          }
          else {
              t.x = e_x1;
              t.y = e_y1;
          }
    t.wysokosc=e_h1;
    t.kat=e_k1;
    t.width_factor=e_wspx;
	if ((USERI1 == 211) && (e_wspx0 > 0.0)) t.width_factor /= e_wspx0;
    t.kolor=e_color;
    t.warstwa=e_layer;
    t.czcionka=e_font;
	t.multiline=0;
	t.underline = 0;
	t.encoding = 0;
	t.spacing=6;
    t.n=T18+strlen(t.text); 
    t.typ=n_typ_zmienna;
    t.justowanie=j_do_lewej;
    t.ukryty=1;
    t.dl=strlen(t.text);
    t.przec=ep_space;
    if (e_obk1>0) t.italics=1;
	del_xt = (e_h1 * sin(e_k1));
	del_yt = (e_h1 * cos(e_k1));
	t.justowanie = j_do_lewej;
	t_x = t.x;
	t_y = t.y;

	switch (e_hj)
	{
	case 0: //normal
		switch (e_vj)
		{
		case 0: //baseline
			break;
		case 1: //bottom
			t.y += del_yt / 4.0;
			break;
		case 2: //middle
			t.y -= (del_yt / 2);
			break;
		case 3: //top
			t.y -= (del_yt);
			break;
		}
		t.justowanie = j_do_lewej;
		break;
	case 1: //center
		switch (e_vj)
		{
		case 0: //baseline
			t.justowanie = j_srodkowo;
			break;
		case 1: //bottom
			t.justowanie = j_srodkowo;
			t.y += del_yt / 4.0;
			break;
		case 2: //middle
			t.justowanie = j_centralnie;
			break;
		case 3: //top
			t.justowanie = j_centralnie;
			t.y -= (del_yt / 2);
			break;
		}
		break;
	case 2: //right
		switch (e_vj)
		{
		case 0: //baseline
			break;
		case 1: //bottom
			t.y += del_yt / 4.0;
			break;
		case 2: //middle
			t.y -= (del_yt / 2);
			break;
		case 3: //top
			t.y -= (del_yt);
			break;
		}
		t.justowanie = j_do_prawej;
		break;
	case 3: //aligned if vertical aligment=0
		switch (e_vj)
		{
		case 0: //baseline
			break;
		case 1: //bottom
			t.y += del_yt / 4.0;
			break;
		case 2: //middle
			t.y -= (del_yt / 2);
			break;
		case 3: //top
			t.y -= (del_yt);
			break;
		}
		t.justowanie = j_do_lewej;
		break;
	case 4: //middle if vertical alignment=0
		switch (e_vj)
		{
		case 0: //baseline
			t.justowanie = j_srodkowo;
			break;
		case 1: //bottom
			t.justowanie = j_srodkowo;
			t.y += del_yt / 4.0;
			break;
		case 2: //middle
			t.justowanie = j_centralnie;
			break;
		case 3: //top
			t.justowanie = j_centralnie;
			t.y -= (del_yt / 2);
			break;
		}
		break;
	case 5: //fit if vertical aligment=0
		switch (e_vj)
		{
		case 0: //baseline
			break;
		case 1: //bottom
			t.y += del_yt / 4.0;
			break;
		case 2: //middle
			t.y -= (del_yt / 2);
			break;
		case 3: //top
			t.y -= (del_yt);
			break;
		}
		t.justowanie = j_do_lewej;
		break;
	default://normal
		t.justowanie = j_do_lewej;
		break;
	}
  
    if (to_block == 1) 
     {
      t.blok=1;
      if (NULL == dodaj_obiekt ((BLOK*)dane, &t)) return 0; 
     }
     else
      {
       t.blok=0;
       if (NULL == dodaj_obiekt ( NULL, &t)) return 0; 
      } 
    
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  check_width_7(p1, &ep_width);
	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z)  < 1 ) return 0;
    break;
  case 40:  /*height - double*/
    e_h_ok=TRUE;
    if ( sscanf ( p1 , "%lf", &e_h)  < 1 ) return 0;
    break;
  case 2 :  /*tag string - identyficator*/  
	 
    strcpy(buf1_1,buf1);
    break;
  case 41:  /*relative X-scale factor - double*/
    e_wspx_ok=TRUE;
    if ( sscanf ( p1 , "%lf", &e_wspx)  < 1 ) return 0;
    break;  
  case 50:  /*rotation angle - double*/
    if ( sscanf ( p1 , "%lf", &e_k)  < 1 ) return 0;
    break; 
  case 51:  /*oblique angle - double*/
    e_obk_ok=TRUE;
    if ( sscanf ( p1 , "%lf", &e_obk)  < 1 ) return 0;
    break;    
  case 7 :  /*text style name*/
    e_font=find_font(buf1,&e_wspx, &e_wspx0, e_wspx_ok,e_h,e_h_ok,e_obk,e_obk_ok);
    break;   
  case 71:  /*text generation flags*/  
    if ( sscanf( p1 , "%d", &e_flags) < 1 ) return 0;
    break;
  case 72:  /*horizontal justification type*/  
    if ( sscanf( p1 , "%d", &e_hj) < 1 ) return 0;
    break;  
  case 73:  /*vertical justification type*/  
    if ( sscanf( p1 , "%d", &e_vj) < 1 ) return 0;
    break;    
  case 11:  /*alignment point X - double*/
    if ( sscanf ( p1 , "%lf", &e_ax)  < 1 ) return 0;
    e_ax_exists=TRUE;
    break;
  case 21:  /*alignment point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_ay)  < 1 ) return 0;
    break;
  case 31:  /*alignment point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_az)  < 1 ) return 0;
    break;  
  default:  break;  
  } /*switch*/
 }
 return 1;
}

float points2mm(float points)
{
    return (points * 0.3527777778);
}

BOOL read_text(FILE *f, int to_block, BOOL block)
/*--------------------------------------------*/
{
	BOOL endentitie;
	char *p, *p1;
	int entitie_code;
	unsigned int e_layer = 0, e_font = 0, e_flags = 0, e_hj = 0, e_vj = 0;
	double e_elevation = 0;
	int e_color = 7;
	double e_extrusion_x, e_extrusion_y, e_extrusion_z;
	double e_x, e_y, e_z, e_h = 2, e_w = 2, e_wspx = 1, e_wspx0 = 1, e_wspxk = 1, e_k = 0, e_obk = 0, e_ax, e_ay, e_az;
	double e_x1, e_y1;
    double e_ax1, e_ay1;
	double e_h1 = -1, e_w1 = -1;
	double e_k1, e_obk1;
	double pi_1;
	char buf[MAXLINE], buf1[MAXLINE];
	char buf1_1[MAXLINE];
	TEXT t = Tdef;
	BOOL e_wspx_ok = FALSE, e_h_ok = FALSE, e_obk_ok = FALSE;
	int kod_310 = 0;
	char buf11[MAXLINE * 2];

	int i, j, i_buf1;
	char codepage_[MAXPAR] = { u8"" }, color_[MAXPAR] = { u8"" }, font_[MAXPAR] = { u8"" }, hight_[MAXPAR] = { u8"" }, width_[MAXPAR] = { "" }, pi_[MAXPAR] = { "" }, alignment_[MAXPAR] = { u8"" }, text_style_[MAXPAR1] = { u8"" };
	double del_xt, del_yt, t_x, t_y;
	int alignment;
	char bold_[MAXPAR] = { u8"" }, italic_[MAXPAR] = { u8"" }, points_[MAXPAR] = { u8"" };
	float points_f;
	char Unicode_[8];
	int ii;
	int ulen;
	unsigned short utf8char;
	char lo, hi;
	BOOL No_Unicode = TRUE;
	int ret123;
	char obiektt1 = 0, obiektt2 = 0, obiektt3 = 0;
    BOOL e_ax_exists=FALSE;

	e_extrusion_x = 0;
	e_extrusion_y = 0;
	e_extrusion_z = 1;
	ep_space = 0;
	ep_novisibility = 0;
	ep_width = line_width.normal * 100;
	strcpy(buf11, "");
	strcpy(buf1, "STANDARD");
	e_font = 0;
	e_h = TEXTSIZE;

	endentitie = FALSE;
	while (endentitie == FALSE)
	{
		if (!inc_nr_linii()) return 0;
		if (myfgets(buf, MAXLINE, f) == NULL) return 0;

		p = buf;

		if (sscanf(p, "%d", &entitie_code) < 1) return 0;

		if (entitie_code != 0)
		{
			if (!inc_nr_linii()) return 0;
			if (myfgets(buf1, MAXLINE, f) == NULL) return 0;

			p1 = buf1;
		}

		switch (entitie_code)
		{
		case 310:
			kod_310++;
			break;
		case 0:
			if (ep_novisibility)
			{
				return 1;
			}
			endentitie = TRUE;
			/*generowanie linii*/
			t.multiline = 0;

			e_x1 = jednostkiOb(e_x - EXTMIN_X);
			e_y1 = jednostkiOb(e_y - EXTMIN_Y);

            if (e_ax_exists) {
                e_ax1 = jednostkiOb(e_ax - EXTMIN_X);
                e_ay1 = jednostkiOb(e_ay - EXTMIN_Y);
            }

			e_h1 = jednostkiOb(e_h);
			e_k1 = e_k * Pi2 / 360;
			e_obk1 = e_obk * Pi2 / 360;

			i_buf1 = strlen(buf1_1);
			alignment = 0;
			i = 0;
			while (i < i_buf1)
			{
				if (buf1_1[i] == '\134') //poszukiwanie opcji
				{
					i++;
					if (buf1_1[i] == 'F')   //odczytanie fontu
					{
						i++; j = 0;
						while (buf1_1[i] != ';')
						{
							if (j < MAXPAR)
							{
								font_[j] = buf1_1[i];
								j++;
							}
							i++;
						}
						font_[j] = '\0';
						strupr(font_);
						e_font = find_font(font_, &e_wspx, &e_wspx0, e_wspx_ok, e_h, e_h_ok, e_obk, e_obk_ok);
						e_h1 = jednostkiOb(e_h);
						i++;
					}
					else if (buf1_1[i] == 'A')   //odczytanie alignmentalignment
					{
						i++; j = 0;
						while (buf1_1[i] != ';')
						{
							if (j < MAXPAR)
							{
                                alignment_[j] = buf1_1[i];
								j++;
							}
							i++;
						}
                        alignment_[j] = '\0';
                        alignment = atoi(alignment_);
						i++;
					}
					else if (buf1_1[i] == 'f')   //odczytanie pozostalych parametrow - zignorowano
					{
						i++;
						text_style_[0] = 'f';
						j = 1;
						while (buf1_1[i] != ';')
						{
							if (j < MAXPAR1)
							{
								text_style_[j] = buf1_1[i];
								j++;
							}
							i++;
						}
						text_style_[j] = ';';
						text_style_[j + 1] = '\0';
						if (t.multiline >= 0) //single and multiline
						{
							//rozbior tekst_style_
							scan_text_style_(text_style_, font_, bold_, italic_, codepage_, points_);
							if (strlen(font_) > 0)
							{
								strupr(font_);
								e_font = find_font(font_, &e_wspx, &e_wspx0, e_wspx_ok, e_h, e_h_ok, e_obk, e_obk_ok);
							}
							if (atoi(bold_) > 0)  t.bold = 1;
							if (atoi(italic_) > 0) t.italics = 1;
							points_f = atof(points_);
							if (points_f > 0) e_h = points2mm(points_f);
						}
						i++;
					}
					else if (buf1_1[i] == 'H')   //odczytanie wysokosci
					{
						i++; j = 0;
						while (buf1_1[i] != ';')
						{
							if (j < MAXPAR)
							{
								hight_[j] = buf1_1[i];
								j++;
							}
							i++;
						}
						if (hight_[j - 1] == 'x')
						{
							hight_[j - 1] = '\0';
							if (t.multiline == 0)
							{
								e_h = atof(hight_);
								e_h1 = e_h1 * atof(hight_);
							}
						}
						else
						{
							hight_[j] = '\0';
							if (t.multiline == 0)
							{
								e_h = atof(hight_);
								e_h1 = jednostkiOb(e_h);   //???????????????
							}
						}
						i++;
					}
					else if (buf1_1[i] == 'W')   //odczytanie width
					{
						i++; j = 0;
						while (buf1_1[i] != ';')
						{
							if (j < MAXPAR)
							{
								width_[j] = buf1_1[i];
								j++;
							}
							i++;
						}
						if (width_[j - 1] == 'x')
						{
							width_[j - 1] = '\0';
							e_h1 = e_h1 * atof(width_);
						}
						else
						{
							width_[j] = '\0';
							if (t.multiline == 0)
							{
								e_w = atof(width_);
								e_w1 = jednostkiOb(e_w);   //???????????????
							}

						}
						i++;
					}
					else if (buf1_1[i] == 'p')   //odczytanie width
					{
						i++; j = 0;
						while (buf1_1[i] != ';')
						{
							if (j < MAXPAR)
							{
								pi_[j] = buf1_1[i];
								j++;
							}
							i++;
						}

						pi_[j] = '\0';
						if (t.multiline == 0)
						{
							pi_1 = atof(pi_ + 1);  //whatever it is
						}

						i++;
					}

					else if (buf1_1[i] == 'C')   //odczytanie koloru
					{
						i++; j = 0;
						while (buf1_1[i] != ';')
						{
							if (j < MAXPAR)
							{
								color_[j] = buf1_1[i];
								j++;
							}
							i++;
						}
						color_[j] = '\0';
						e_color = atoi(color_);
						e_color = GetColorALF(e_color, e_layer);
						i++;
					}

					else if (buf1_1[i] == 'L')   //underlined
					{
						i++;
					}
					else if (buf1_1[i] == 'l')   //underlined
					{
						i++;
					}
					else if (buf1_1[i] == 'O')   //Overlined
					{
						i++;
					}
					else if (buf1_1[i] == 'U')   //Unicode  \U+0080
					{
						i++;
						strcpy(Unicode_, "0x");

						for (ii = 0; ii < 5; ii++)
						{
							if (buf1_1[i] != '+')
							{
								ulen = strlen(Unicode_);
								Unicode_[ulen] = buf1_1[i];
								Unicode_[ulen + 1] = '\0';  //0x0080
							}
							i++;
						}

						unicode2utf8char(Unicode_, &utf8char);

						lo = utf8char & 0X00FF;
						hi = (utf8char & 0XFF00) >> 8;

						strncat(buf11, &lo, 1);
						strncat(buf11, &hi, 1);

						No_Unicode = FALSE;
					}
					else if (buf1_1[i] == 'P')   //nowy wiersz
					{
						strncat(buf11, "\r\n", 2);  //"\n\r"
						t.multiline = 1;
						i++;
					}
					else //dopisanie do bufora
					{
						strncat(buf11, &buf1_1[i], 1);
						i++;
					}
				}
				else //dopisanie do bufora
				{
					if ((buf1_1[i] == '{') || (buf1_1[i] == '}'))
					{
						;
					}
					else
					{
						strncat(buf11, &buf1_1[i], 1);
					}
					i++;
				}
			}

			if (strlen(buf11) > 0)
			{
				special_chars(buf11);

				if (No_Unicode)
                {
                    if (!is_utf8(buf11))  decodingdxf(buf11);
                }

				strcpy(t.text, buf11);

                if ((((e_vj!=0) || (e_hj!=0))) && (e_ax_exists))
                {
                    t.x = e_ax1;
                    t.y = e_ay1;
                }
                else {
                    t.x = e_x1;
                    t.y = e_y1;
                }
				t.wysokosc = e_h1;
				t.kat = e_k1;
				t.width_factor = e_wspx;
				if ((USERI1 == 211) && (e_wspx0 > 0.0)) t.width_factor /= e_wspx0;
				t.kolor = e_color;
				t.warstwa = e_layer;
				t.czcionka = e_font;
				t.multiline = 0;
				t.underline = 0;
				t.encoding = 0;
				t.spacing = 6;
				t.n = T18 + strlen(t.text);
				t.typ = n_typ_normal;
				t.justowanie = j_do_lewej;
				t.ukryty = 0;
				t.dl = strlen(t.text);
				t.obiektt1 = obiektt1;
				t.obiektt2 = obiektt2;
				t.obiektt3 = obiektt3;
				t.przec = ep_space;
				if (e_obk1 > 0) t.italics = 1;

				del_xt = (e_h1 * sin(e_k1));
				del_yt = (e_h1 * cos(e_k1));   //-
				t.justowanie = j_do_lewej;
				t_x = t.x;
				t_y = t.y;

				switch (e_hj)
				{
				case 0: //left
					switch (e_vj)
					{
					case 0: //baseline
						break;
					case 1: //bottom
						t.y += del_yt/4.0;
						break;
					case 2: //middle
						t.y -= (del_yt / 2.0);
						break;
					case 3: //top
						t.y -= (del_yt);
						break;
					}
					t.justowanie = j_do_lewej;
					break;
				case 1: //center
					switch (e_vj)
					{
					case 0: //baseline
						t.justowanie = j_srodkowo;
						break;
					case 1: //bottom
						t.justowanie = j_srodkowo;
						t.y += del_yt / 4.0;
						break;
					case 2: //middle
						t.justowanie = j_centralnie;
						break;
					case 3: //top
						t.justowanie = j_centralnie;
						t.y -= (del_yt/2.0);
						break;
					}
					break;
				case 2: //right
					switch (e_vj)
					{
					case 0: //baseline
						break;
					case 1: //bottom
						t.y += del_yt / 4.0;
						break;
					case 2: //middle
						t.y -= (del_yt / 2.0);
						break;
					case 3: //top
						t.y -= (del_yt);
						break;
					}
					t.justowanie = j_do_prawej;
					break;
				case 3: //aligned if vertical aligment=0
					switch (e_vj)
					{
					case 0: //baseline
						break;
					case 1: //bottom
						t.y += del_yt / 4.0;
						break;
					case 2: //middle
						t.y -= (del_yt / 2.0);
						break;
					case 3: //top
						t.y -= (del_yt);
						break;
					}
					t.justowanie = j_do_lewej;
					break;
				case 4: //middle if vertical alignment=0
					switch (e_vj)
					{
					case 0: //baseline
						t.justowanie = j_srodkowo;
						break;
					case 1: //bottom
						t.justowanie = j_srodkowo;
						t.y += del_yt / 4.0;
						break;
					case 2: //middle
						t.justowanie = j_centralnie;
						break;
					case 3: //top
						t.justowanie = j_centralnie;
						t.y -= (del_yt / 2.0);
						break;
					}
					break;
				case 5: //fit if vertical aligment=0
					switch (e_vj)
					{
					case 0: //baseline
						break;
					case 1: //bottom
						t.y += del_yt / 4.0;
						break;
					case 2: //middle
						t.y -= (del_yt / 2.0);
						break;
					case 3: //top
						t.y -= (del_yt);
						break;
					}
					t.justowanie = j_do_lewej;
					break;
				default://normal
					t.justowanie = j_do_lewej;
					break;
				}

				if (to_block == 1)
				{
					t.blok = 1;
					if (NULL == dodaj_obiekt((BLOK*)dane, &t)) return 0;
				}
				else
				{
					t.blok = 0;
					if (NULL == dodaj_obiekt(NULL, &t)) return 0;
				}

				return 1;
			}
			break;
		case 8:  /*layer - sring*/
			if (!block)
			{
				e_layer = find_layer(buf1);
				e_color = Layers[e_layer].color;
			}
			else
			{
				e_layer = find_layer_DXF(buf1);
				e_color = Layers_DXF[e_layer].color;
			}
			break;
		case 6:  /*line type - string*/
			break;
		case 38:  /*elevation - double*/
			if (sscanf(p1, "%lf", &e_elevation) < 1) return 0;
			break;
		case 62:  /*color number - int*/
			if (sscanf(p1, "%d", &e_color) < 1) return 0;
			e_color = GetColorALF(e_color, e_layer);
			break;
		case 67:  /*model (0) /paper (1) space - int*/
			if (sscanf(p1, "%d", &ep_space) < 1) return 0;
			break;
		case 60:  /*novisibility*/
			if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
			break;
		case 210:  /*extrusion direction X - double*/
			if (sscanf(p1, "%lf", &e_extrusion_x) < 1) return 0;
			break;
		case 220:  /*extrusion direction Y - double*/
			if (sscanf(p1, "%lf", &e_extrusion_y) < 1) return 0;
			break;
		case 230:  /*extrusion direction Z - double*/
			if (sscanf(p1, "%lf", &e_extrusion_z) < 1) return 0;
			break;
		case 370: /*line width*/
			if (sscanf(p1, "%lf", &ep_width) < 1) return 0;
			if (ep_width == -3) ep_width = line_width.normal * 100;  //standard 0.3 mm
			else if (ep_width == -1) ep_width = line_width.normal * 100;  //standard 0.3 mm  jak warstwa
			else if (ep_width == -2) ep_width = line_width.normal * 100;  //standard 0.3 mm  jak blok
			break;
		case 999: /*comment*/
			ret123 = check_width_7(p1, &ep_width);
			if (!ret123)
			{
				ret123 = check_dim_text(p1, &obiektt1, &obiektt2, &obiektt3);
			}
			break;
		case 10:  /*start point X - double*/
			if (sscanf(p1, "%lf", &e_x) < 1) return 0;
			break;
		case 20:  /*start point Y - double*/
			if (sscanf(p1, "%lf", &e_y) < 1) return 0;
			break;
		case 30:  /*start point Z - double*/
			if (sscanf(p1, "%lf", &e_z) < 1) return 0;
			break;
		case 40:  /*height - double*/
			e_h_ok = TRUE;
			if (sscanf(p1, "%lf", &e_h) < 1) return 0;
			break;
		case 1:  /*text value*/
			strcpy(buf1_1, buf1);

			break;
		case 41:  /*relative X-scale factor - double*/
			e_wspx_ok = TRUE;
			if (sscanf(p1, "%lf", &e_wspx) < 1) return 0;
			break;
		case 50:  /*rotation angle - double*/
			if (sscanf(p1, "%lf", &e_k) < 1) return 0;
			break;
		case 51:  /*oblique angle - double*/
			e_obk_ok = TRUE;
			if (sscanf(p1, "%lf", &e_obk) < 1) return 0;
			break;
		case 7:  /*text style name*/
			e_font = find_font(buf1, &e_wspx, &e_wspx0, e_wspx_ok, e_h, e_h_ok, e_obk, e_obk_ok);
			break;
		case 71:  /*text generation flags*/
			if (sscanf(p1, "%d", &e_flags) < 1) return 0;
			break;
		case 72:  /*horizontal justification type*/
			if (sscanf(p1, "%d", &e_hj) < 1) return 0;
			break;
		case 73:  /*vertical justification type*/
			if (sscanf(p1, "%d", &e_vj) < 1) return 0;
			break;
		case 11:  /*alignment point X - double*/
			if (sscanf(p1, "%lf", &e_ax) < 1) return 0;
            e_ax_exists=TRUE;
			break;
		case 21:  /*alignment point Y - double*/
			if (sscanf(p1, "%lf", &e_ay) < 1) return 0;
			break;
		case 31:  /*alignment point Z - double*/
			if (sscanf(p1, "%lf", &e_az) < 1) return 0;
			break;
		default:  break;
		} /*switch*/
	}
 return 1;
}


void scan_text_style_(char *text_style_,char *font_,char *bold_,char *italic_,char *codepage_,char *points_)
{
	int i,j,i_buf2;

	strcpy(font_,"");
    strcpy(bold_,"");
	strcpy(italic_,"");
	strcpy(codepage_,"");
	strcpy(points_,"");

	i_buf2=strlen(text_style_);
    i=0;
    while (i<i_buf2)
     {
        if (text_style_[i]=='f')   //read fontu
          {
            i++; j=0;
            while ((text_style_[i]!='|') && (text_style_[i]!=';'))
             {
               if (j<MAXPAR)
                {font_[j]=text_style_[i];
                 j++;}
               i++;
             }
            font_[j]='\0';
            strupr(font_);
            i++;
          }
          else if (text_style_[i]=='b')   //read bold
          {
            i++; j=0;
            while ((text_style_[i]!='|') && (text_style_[i]!=';'))
             {
               if (j<MAXPAR)
               {bold_[j]=text_style_[i];
               j++;}
               i++;
             }
            bold_[j]='\0';
            i++;
          }
		  else if (text_style_[i]=='i')   //read italic
          {
            i++; j=0;
            while ((text_style_[i]!='|') && (text_style_[i]!=';'))
             {
               if (j<MAXPAR)
               {italic_[j]=text_style_[i];
               j++;}
               i++;
             }
            italic_[j]='\0';
            i++;
          }
		  else if (text_style_[i]=='c')   //reading codepage
          {
            i++; j=0;
            while ((text_style_[i]!='|') && (text_style_[i]!=';'))
             {
               if (j<MAXPAR)
               {codepage_[j]=text_style_[i];
               j++;}
               i++;
             }
            codepage_[j]='\0';
            i++;
          }
		  else if (text_style_[i]=='p')   //odczytanie points
          {
            i++; j=0;
            while ((text_style_[i]!='|') && (text_style_[i]!=';'))
             {
               if (j<MAXPAR)
               {points_[j]=text_style_[i];
               j++;}
               i++;
             }
            points_[j]='\0';
            i++;
          }
          

	}
}


BOOL read_mtext(FILE *f,int to_block, BOOL block)
/*---------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned int /*char*/ e_layer=0, e_font=0, e_attachment_point=0, e_hj=-1, e_vj=-1;
  double e_elevation=0;
  int e_color=7;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_x, e_y, e_z, e_h = 2, e_w = 2, e_wspx = 1, e_wspx0 = 1, e_wspxk = 1, e_k = 0, e_obk = 0;// e_ax, e_ay, e_az;
  double e_x1, e_y1;
  double e_h1 = -1, e_w1 = -1;
  double e_k1; // , e_obk1; //e_ax1, e_ay1, e_az1;
  BOOL e_wspx_ok=FALSE, e_h_ok=FALSE, e_obk_ok=FALSE;
  char buf[MAXLINE], buf10[MAXLINE*2], buf1[MAXLINE]; // e_text[MAXLINE];
  char buf11[MAXLINE*2];
  TEXT t=Tdef;
  double rectangle_width, horizontal_width;
  BOOL  b_rectangle_width=FALSE;
  double x_vector_x=1.0, x_vector_y=0.0, x_vector_z=0.0;
  char codepage_[MAXPAR]={u8""}, color_[MAXPAR]={u8""}, font_[MAXPAR]={u8""}, hight_[MAXPAR]={u8""}, width_[MAXPAR] = { "" }, pi_[MAXPAR] = { "" }, alignment_[MAXPAR]={u8""}, text_style_[MAXPAR1]={u8""};
  int i, j, i_buf1;
  double del_xt, del_yt, del_xtb, del_ytb, t_x, t_y;
  int alignment;
  float points_f;
  char bold_[MAXPAR]={u8""},italic_[MAXPAR]={u8""},points_[MAXPAR]={u8""};
  char Unicode_[8];
  int ii;
  int ulen;
  unsigned short utf8char;
  char lo, hi;
  BOOL No_Unicode = TRUE;
  double tsin, tcos;
  unsigned int e_line_spacing=6;
  double e_spacing=1.0;
  int e_spacing_style=1;
  double box_h;
  char *ptr;
  int n_i=0;
  BOOL e_ax_exists=FALSE;
  double addl;

  float pi_i=0.0;
  float pi_l=0.0;
  float pi_r=0.0;
  float pi_x=0.0;
  int pi_q=0; //left paragraph alignment

  e_extrusion_x=0;
  e_extrusion_y=0;
  e_extrusion_z=1;
  ep_space=0;
  ep_novisibility = 0;
 ep_width=line_width.normal*100;
 strcpy(buf11,"");
 strcpy(buf1,"STANDARD");
 e_font = 0;
 e_h=TEXTSIZE;
 strcpy(buf10, "");
 
 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

    p1=buf1;
  }  
  
  
  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
    endentitie=TRUE;
    /*generowanie tekstu/ tekstow*/
	t.multiline = 0;
    /**************/
    i_buf1=strlen(buf10);

    alignment=0;
    i=0;
    while (i<i_buf1)
     {
      if (buf10[i]=='\134') //poszukiwanie opcji
       {
         i++;
         if (buf10[i]=='F')   //odczytanie fontu
          {
            i++; j=0;
            while (buf10[i]!=';')
             {
               if (j<MAXPAR)
                {font_[j]=buf10[i];
                 j++;}
               i++;
             }
            font_[j]='\0';
            strupr(font_);
            e_font=find_font(font_,&e_wspx, &e_wspx0, e_wspx_ok,e_h,e_h_ok,e_obk,e_obk_ok);
            e_h1=jednostkiOb(e_h);
            i++;
          }
          else if (buf10[i]=='A')   //odczytanie alignment
          {
            i++; j=0;
            while (buf10[i]!=';')
             {
               if (j<MAXPAR)
                {alignment_[j]=buf10[i];
                 j++;}
               i++;
             }
              alignment_[j]='\0';
              alignment=atoi(alignment_);
            i++;
          }
          else if (buf10[i]=='f')   //odczytanie pozostalych parametrow - zignorowano
          {
            i++; 
			text_style_[0]='f';
			j=1;
            while (buf10[i]!=';')
             {
               if (j<MAXPAR1)
               {text_style_[j]=buf10[i];
               j++;}
               i++;
             }
            text_style_[j]=';';
            text_style_[j+1]='\0';
			if (t.multiline >= 0)  //so single or multiline
			{
				//rozbior tekst_style_
				scan_text_style_(text_style_, font_, bold_, italic_, codepage_, points_);
				if (strlen(font_) > 0)
				{
					strupr(font_);
					e_font = find_font(font_, &e_wspx, &e_wspx0, e_wspx_ok, e_h, e_h_ok, e_obk, e_obk_ok);
				}
				if (atoi(bold_) > 0)  t.bold = 1;
				if (atoi(italic_) > 0) t.italics = 1;
				points_f = atof(points_);
				if (points_f > 0) e_h = points2mm(points_f);
			}
            i++;
          }
          else if (buf10[i]=='H')   //odczytanie wysokosci
          {
            i++; j=0;
            while (buf10[i]!=';')
             {
               if (j<MAXPAR)
               {hight_[j]=buf10[i];
               j++;}
               i++;
             }
			if (hight_[j-1]=='x')
			{
			 hight_[j-1]='\0';
			 if (t.multiline >= 0)
			 {
				 e_h = atof(hight_);
				 e_h1 = e_h1 * atof(hight_);
			 }
			}
			 else
			 {
              hight_[j]='\0';
			  if (t.multiline >= 0)
			  {
				  e_h = atof(hight_);
				  e_h1 = jednostkiOb(e_h);   //???????????????
			  }
			 }
            i++;
          }
		  else if (buf10[i] == 'W')   //odczytanie width
		 {
			 e_wspx_ok = TRUE;
			 i++; j = 0;
			 while (buf10[i] != ';')
			 {
				 if (j < MAXPAR)
				 {
					 width_[j] = buf10[i];
					 j++;
				 }
				 i++;
			 }
			 if (width_[j - 1] == 'x')
			 {
				 width_[j - 1] = '\0';
				 e_h1 = e_h1 * atof(width_);
			 }
			 else
			 {
				 width_[j] = '\0';
				 if (t.multiline >= 0)
				 {
					 e_w = atof(width_);
					 e_w1 = jednostkiOb(e_w);   //???????????????
				 }
			 
			 }
			 i++;
		 }
		  else if (buf10[i] == 'p')   //odczytanie width
		 {
		 i++; j = 0;
		 while (buf10[i] != ';')
		 {
             while ((buf10[i] != ';') && (buf10[i] != ','))
             {
                 if (j < MAXPAR) {
                     pi_[j] = buf10[i];
                     j++;
                 }
                 i++;
             }

             pi_[j] = '\0';
             if (t.multiline >= 0)
             {
                 if (pi_[0]=='i')
                     pi_i = atof(pi_+1);  //identation of first line relative to "l" argument
                 else if (pi_[0]=='l')
                     pi_l = atof(pi_+1);  //left paragraph identation
                 else if (pi_[0]=='r')
                     pi_r = atof(pi_+1);  //right paragraph identation
                 else if (pi_[0]=='x')
                     pi_x = atof(pi_+1);  //is required if a “q” or a “t” argument is present, the placement of the “x” has no obvious rules
                 else if (pi_[0]=='q') {
                     if (pi_[1]=='l') pi_q=0; //left paragraph alignment
                     else if (pi_[1]=='r') pi_q=1; //right paragraph alignment
                     else if (pi_[1]=='c') pi_q=2; //center paragraph alignment
                     else if (pi_[1]=='j') pi_q=3; //justified paragraph alignment
                     else if (pi_[1]=='d') pi_q=4; //distributed paragraph alignment
                 }
             }
             if (buf10[i]==';') break;
             else i++;
		 }

		 i++;
		 }

          else if (buf10[i]=='C')   //odczytanie koloru
          {
            i++; j=0;
            while (buf10[i]!=';')
             {
               if (j<MAXPAR)
               {color_[j]=buf10[i];
               j++;}
               i++;
             }
            color_[j]='\0';
            e_color=atoi(color_);
			e_color=GetColorALF(e_color, e_layer);
            i++;
          }
		  
		  else if (buf10[i] == 'L')   //underlined
		  {
			i++;
		  }
		  else if (buf10[i] == 'l')   //underlined
		  {
			i++;
		  }
		  else if (buf10[i] == 'O')   //Overlined
		  {
			i++;
		  }
		  else if (buf10[i] == 'U')   //Unicode  \U+0080
		  {
		  i++;
		  strcpy(Unicode_, "0x");

		  for (ii=0; ii<5; ii++)
			  {
				if (buf10[i] != '+')
				  {
					  ulen = strlen(Unicode_);
					  Unicode_[ulen] = buf10[i];
					  Unicode_[ulen+1] = '\0';  //0x0080
				  }
				i++;
			  }

		  unicode2utf8char(Unicode_, &utf8char);

		  lo = utf8char & 0X00FF;
		  hi = (utf8char & 0XFF00) >> 8;
		  
		  strncat(buf11, &lo, 1);
		  strncat(buf11, &hi, 1);

		  No_Unicode = FALSE;

		  }
          else if (buf10[i]=='P')   //nowy wiersz
          {
           strncat(buf11, "\r\n", 2);  //"\n\r"
		   t.multiline = 1;
           i++;
          }
          else //dopisanie do bufora
           {
            strncat(buf11,&buf10[i],1);
            i++;
           }
       }
        else //dopisanie do bufora
         {
		   if ((buf10[i] == '{') || (buf10[i] == '}'))
		   {
			   ;
		   }
		    else
			{
                strncat(buf11,&buf10[i],1);
			}
           i++;
        }
     }

    /**************/
   if (strlen(buf11)>0)
   {
     special_chars(buf11);
     
	 if (No_Unicode)
     {
         if (!is_utf8(buf11))  decodingdxf(buf11);
     }

    e_k1=e_k;

    t.kat = e_k1;
    t.wysokosc = e_h1;
    t.width_factor=e_wspx*e_wspx0;
    t.czcionka=e_font;
    if (USERI1 == 211)
       t.width_factor /= e_wspx0;

     //wrapping
     if (b_rectangle_width)
     {
         char *ptr_rn=buf11;
         char *ptr_rn1;
         char *ptr_spc1;
         char *ptr_spc2;

         ptr_spc1 = strchr(ptr_rn, ' ');

         while (1)
         {
             if (*ptr_rn=='\0') break;

             ptr_rn1=strchr(ptr_rn, '\n');
             if ((ptr_rn1!=NULL) && (ptr_spc1!=NULL))
             {
                 if (ptr_spc1>ptr_rn1)
                 {
                     ptr_rn = ptr_rn1 + 1;
                     ptr_spc1 = strchr(ptr_rn, ' ');
                     continue;
                 }
             }

             if (ptr_spc1!=NULL) ptr_spc2 = strchr(ptr_spc1+1, ' ');
             else ptr_spc2=NULL;

             if (ptr_spc2!=NULL)
             {
                 //temporary
                 *ptr_spc2 = '\0';
                 addl = Get_Text_Len(&t, ptr_rn, 1.0, 0, NULL, NULL, NULL, NULL);
                 //return to original value
                 *ptr_spc2 = ' ';
                 if (addl > rectangle_width)
                 {
                     //inserting line break
                     insertString(ptr_rn, ptr_spc1-ptr_rn+1, "\r\n");
                     t.multiline=1;
                     ptr_rn=ptr_spc1+3;
                     ptr_spc1 = strchr(ptr_rn, ' ');
                 }
                 else
                 {
                     ptr_spc1 = strchr(ptr_spc1+1, ' ');
                 }
                 if (ptr_spc1!=NULL) ptr_spc2 = strchr(ptr_spc1+1, ' ');
                 else ptr_spc2=NULL;
             }
             else if (ptr_spc1!=NULL)
             {
                 //temporary
                 *ptr_spc1 = '\0';
                 addl = Get_Text_Len(&t, ptr_rn, 1.0, 0, NULL, NULL, NULL, NULL);
                 //return to original value
                 *ptr_spc1 = ' ';
                 if (addl > rectangle_width)
                 {
                     //inserting line break
                     insertString(ptr_rn, ptr_spc1-ptr_rn+1, "\r\n");
                     t.multiline=1;
                 }
                 break; //there are no more spaces
             }
             else break; //no more spaces
         }
     }

    strcpy(t.text,buf11);

	tsin = sin(t.kat);
	tcos = cos(t.kat);

    t.x = e_x1;
    t.y = e_y1;

    //using paragraph properties
    t.x+=jednostkiOb(pi_l+pi_i);  //left paragraph indentation + indentation of the first line

    t.kolor=e_color;
    t.warstwa=e_layer;
    t.czcionka=e_font;
	t.underline = 0;
	t.spacing=e_line_spacing;
    t.n=T18+strlen(t.text); 
    t.typ=n_typ_normal;
    t.justowanie=j_do_lewej;
    t_x=t.x;
    t_y=t.y;

    box_h=t.wysokosc;

     if (t.multiline==1)    //calculating rectangle dimensions
     {
         //number of \n
         char *t_text=t.text;
         n_i=0;
         char *ptr_n=strchr(t_text, '\n');
         while (ptr_n!=NULL)
         {
             n_i++;
             t_text=ptr_n+1;
             ptr_n=strchr(t_text, '\n');
         }
         if (n_i==0)
             box_h=t.wysokosc;
         else box_h=(n_i+1)*t.wysokosc*((float)(t.spacing) / 10.0 + 1.0) * BOX_H_CORRECTION;
     }

       del_xt =   (e_h1 * sin(e_k1));
       del_yt =   (e_h1 * cos(e_k1));

       del_xtb =   (box_h * sin(e_k1));
       del_ytb =   (box_h * cos(e_k1));

           switch (e_attachment_point)
            {
             case 0: //normal
                     t.justowanie=j_do_lewej;
             break;
             case 1: //top left
                     t.justowanie=j_do_lewej;
                     t.x += del_xt;
                     t.y -= del_yt;
             break;
             case 2: //top center
                     t.justowanie=j_centralnie;
                     t.x += (del_xt/2);
                     t.y -= (del_yt/2);
             break;
             case 3: //top right
                     t.justowanie=j_do_prawej;
                     t.x += (del_xt);  //it should be more to the left by space between letters
                     t.y -= (del_yt);
             break;
             case 4: //middle left
                     t.justowanie=j_do_lewej;
                     t.x += (del_xt-(del_xtb/2.0));
                     t.y += (-del_yt+(del_ytb/2.0));
             break;
             case 5: //middle center
                     t.justowanie=j_centralnie;
                     t.x += ((del_xt/2.0) - (del_xtb/2.0));
                     t.y += ((-del_yt/2.0) + (del_ytb/2.0));
             break;
             case 6: //middle right
                     t.justowanie=j_do_prawej;
                     t.x += (del_xt-(del_xtb)/2.0);
                     t.y += (-del_yt+(del_ytb)/2.0);
             break;
             case 7: //bottom left
                     t.justowanie=j_do_lewej;
                     t.x += ((del_xt)-(del_xtb));
                     t.y += ((-del_yt)+(del_ytb));
             break;
             case 8: //bottom center
                     t.justowanie=j_srodkowo;
                     t.x += ((del_xt/2.0)-(del_xtb));
                     t.y += ((-del_yt) +(del_ytb));
             break;
             case 9: //bottom right
                     t.justowanie=j_do_prawej;
                     t.x += ((del_xt)-(del_xtb));
                     t.y += ((-del_yt)+(del_ytb));
             break;
             default://normal
                     t.justowanie=j_do_lewej;
             break;
            }


    t.ukryty=0;
    t.dl=strlen(t.text);
    t.przec=ep_space;

    last_mtext_h=t.wysokosc;
  
    if (to_block == 1) 
     {
      t.blok=1;
      if (NULL == dodaj_obiekt ((BLOK*)dane, &t)) return 0; 
     }
     else
      {
       t.blok=0;
       if (NULL == dodaj_obiekt ( NULL, &t)) return 0; 
      } 
   }
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  check_width_7(p1, &ep_width);
	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x)  < 1 ) return 0;
    e_x1=jednostkiOb(e_x - EXTMIN_X);
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y)  < 1 ) return 0;
    e_y1=jednostkiOb(e_y - EXTMIN_Y);
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z)  < 1 ) return 0;
    break;
  case 11:  /*X-axis direction vector X*/
    if ( sscanf ( p1 , "%lf", &x_vector_x)  < 1 ) return 0;
    e_ax_exists=TRUE;
    //e_k = (acos(x_vector_x)/Pi2)*360;
    break;
  case 21:  /*X-axis direction vector Y*/
    if ( sscanf ( p1 , "%lf", &x_vector_y)  < 1 ) return 0;
    //e_k = (asin(x_vector_y)/Pi2)*360;
    e_k=atan2(x_vector_y, x_vector_x);
    break;
  case 31:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &x_vector_z)  < 1 ) return 0;
    break;  
  case 40:  /*height - double*/
    e_h_ok=TRUE;
    if ( sscanf ( p1 , "%lf", &e_h)  < 1 ) return 0;
	e_h1=jednostkiOb(e_h);
    break;
  case 1 :  /*text value*/
    /*odszukanie \Ffont; \Aattachmet;*/
    /* \Hhight; \Ccolor; \P nowy wiersz*/
    /* f... | b... | c... | p...;*/
    //przepisywanie bufora buf1 do bufora buf11 z poszukiwaniem znaku 134
	//if (strlen(buf10)==0) strcpy(buf10, buf1);
	//else 
	strcat(buf10, buf1);

    break;
  case 3:  /*text value*/
	  strcat(buf10, buf1);
	  break;
  case 41:  /*relative X-scale factor - double*/
    if ( sscanf ( p1 , "%lf", &rectangle_width)  < 1 ) return 0;
    rectangle_width=jednostkiOb(rectangle_width);
    if (Check_if_Equal(rectangle_width, 0.0)==TRUE) b_rectangle_width=FALSE;
    else b_rectangle_width=TRUE;
    break;  
  case 42:  /*relative X-scale factor - double*/
	  if (sscanf(p1, "%lf", &horizontal_width) < 1) return 0;
	  break;
  case 50:  /*rotation angle - double*/
    if ( sscanf ( p1 , "%lf", &e_k)  < 1 ) return 0;
    break;
  case 7 :  /*text style name*/
    e_font=find_font(buf1,&e_wspx, &e_wspx0, e_wspx_ok,e_h,e_h_ok,e_obk,e_obk_ok);
	e_h1=jednostkiOb(e_h);
    break;
  case 73:  /*Mtext line spacing style*/
      if ( sscanf ( p1 , "%d", &e_spacing_style)  < 1 ) return 0;
      break;
  case 44:  /*Mtext line spacing factor*/
          e_obk_ok=TRUE;
          if ( sscanf ( p1 , "%lf", &e_spacing)  < 1 ) return 0;
          e_line_spacing=(int)(((e_spacing*1.4)-1.0)*10);   //or 1.3
          if (e_line_spacing<0) e_line_spacing=0;
          if (e_line_spacing>31) e_line_spacing=31;
          break;
  case 71:  /*text generation flags*/
    if ( sscanf( p1 , "%d", &e_attachment_point) < 1 ) return 0;
    break;
  default:  break;  
  } /*switch*/
 }
 return 1;
}

BOOL read_trace(FILE *f,int to_block, BOOL block)
/*---------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned char e_layer=0;
  unsigned char e_type=64;
  double e_elevation=0;
  int e_color=7;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_x1, e_y1, e_z1, e_x2, e_y2, e_z2, e_x3, e_y3, e_z3, e_x4, e_y4, e_z4 ;
  char buf[MAXLINE], buf1[MAXLINE];
  WIELOKAT S=S4def;
  unsigned char translucency;
  char* translucency_ptr;

  ep_translucency = 255;
  e_extrusion_x=0;
  e_extrusion_y=0;
  e_extrusion_z=1;
  ep_space=0;
  ep_novisibility = 0;
 ep_width=line_width.normal*100;
 endentitie=FALSE;
 S.lp=8;
 S.n=40;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

    p1=buf1;
  }
  
  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
    endentitie=TRUE;
    /*generowanie linii*/
    S.xy[0]=jednostkiOb(e_x1 - EXTMIN_X);
    S.xy[1]=jednostkiOb(e_y1 - EXTMIN_Y);
    S.xy[2]=jednostkiOb(e_x2 - EXTMIN_X);
    S.xy[3]=jednostkiOb(e_y2 - EXTMIN_Y);
    if (S.lp==8)
     {
      S.xy[6]=jednostkiOb(e_x3 - EXTMIN_X);
      S.xy[7]=jednostkiOb(e_y3 - EXTMIN_Y);
      S.xy[4]=jednostkiOb(e_x4 - EXTMIN_X);
      S.xy[5]=jednostkiOb(e_y4 - EXTMIN_Y);
     } 
     else 
      {
       S.xy[4]=jednostkiOb(e_x3 - EXTMIN_X);
       S.xy[5]=jednostkiOb(e_y3 - EXTMIN_Y);
      } 
    S.kolor=e_color; 
    S.warstwa=e_layer;
    S.przec=ep_space;
    S.empty_typ=0;
	if (ep_translucency < 255)
	{
		translucency = (unsigned char)ep_translucency;
		S.translucent = 1;
		translucency_ptr = (char*)S.xy;
		translucency_ptr += (S.lp * sizeof(float));
		memmove(translucency_ptr, &translucency, sizeof(unsigned char));

		S.n = 8 + S.lp * sizeof(float) + sizeof(unsigned char);
	}
	else
	{
		S.translucent = 0;
	}

    if (to_block == 1) 
     {
      S.blok=1;
      if (NULL == dodaj_obiekt ((BLOK*)dane, &S)) return 0; 
     }
     else
      {
       S.blok=0;
       if (NULL == dodaj_obiekt ( NULL, &S)) return 0; 
      } 
    
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_type = Layers[e_layer].line_type;
		  ep_width = get_width(Layers[e_layer].line_type);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_type = Layers_DXF[e_layer].line_type;
		  ep_width = get_width(Layers_DXF[e_layer].line_type);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    e_type=find_type(buf1,e_type);
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 440:  /*transparency*/
	  if (sscanf(p1, "%d", &ep_translucency) < 1) return 0;
	  break;
  case 999: /*comment*/
	  check_width_7(p1, &ep_width);
	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x1)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y1)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z1)  < 1 ) return 0;
    break;
  case 11:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x2)  < 1 ) return 0;
    break;
  case 21:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y2)  < 1 ) return 0;
    break;
  case 31:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z2)  < 1 ) return 0;
    break;
  case 12:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x3)  < 1 ) return 0;
    break;
  case 22:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y3)  < 1 ) return 0;
    break;
  case 32:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z3)  < 1 ) return 0;
    break;  
  case 13:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x4)  < 1 ) return 0;
    break;
  case 23:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y4)  < 1 ) return 0;
    break;
  case 33:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z4)  < 1 ) return 0;
    break;  
  default:  break;  
  } /*switch*/
 }
 return 1;
}

BOOL read_solid(FILE *f,int to_block, BOOL block)
/*----------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned char e_layer=0;
  unsigned char e_type=64;
  double e_elevation=0;
  int e_color=7;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_x1, e_y1, e_z1, e_x2, e_y2, e_z2, e_x3, e_y3, e_z3, e_x4, e_y4, e_z4 ;
  char buf[MAXLINE], buf1[MAXLINE];
  WIELOKAT S=S4def;
  int ret123;
  char obiektt1=0, obiektt2=0, obiektt3=0;
  unsigned char translucency;
  char* translucency_ptr;

  ep_translucency = 255;
  e_extrusion_x=0;
  e_extrusion_y=0;
  e_extrusion_z=1;
  ep_space=0;
  ep_novisibility = 0;
 ep_width=line_width.normal*100;
 endentitie=FALSE;
 S.lp=6;
 S.n=32;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

    p1=buf1;
  }  
  
  
  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
    endentitie=TRUE;
    /*generowanie linii*/
    S.xy[0]=jednostkiOb(e_x1 - EXTMIN_X);
    S.xy[1]=jednostkiOb(e_y1 - EXTMIN_Y);
    S.xy[2]=jednostkiOb(e_x2 - EXTMIN_X);
    S.xy[3]=jednostkiOb(e_y2 - EXTMIN_Y);
    if (S.lp==8)
     {
      S.xy[6]=jednostkiOb(e_x3 - EXTMIN_X);
      S.xy[7]=jednostkiOb(e_y3 - EXTMIN_Y);
      S.xy[4]=jednostkiOb(e_x4 - EXTMIN_X);
      S.xy[5]=jednostkiOb(e_y4 - EXTMIN_Y);
     } 
      else
     {
      S.xy[4]=jednostkiOb(e_x3 - EXTMIN_X);
      S.xy[5]=jednostkiOb(e_y3 - EXTMIN_Y);
     }
    S.kolor=e_color; 
    S.warstwa=e_layer;
    S.przec=ep_space;
    S.empty_typ=0;

      S.obiektt1 = obiektt1;
      S.obiektt2 = obiektt2;
      S.obiektt3 = obiektt3;

	if (ep_translucency < 255)
	{
		translucency = (unsigned char)ep_translucency;
		S.translucent = 1;
		translucency_ptr = (char*)S.xy;
		translucency_ptr += (S.lp * sizeof(float));
		memmove(translucency_ptr, &translucency, sizeof(unsigned char));

		S.n = 8 + S.lp * sizeof(float) + sizeof(unsigned char);
	}
	else
	{
		S.translucent = 0;
	}
  
    if (to_block == 1) 
     {
      S.blok=1;
      if (NULL == dodaj_obiekt ((BLOK*)dane, &S)) return 0; 
     }
     else
      {
       S.blok=0;
       if (NULL == dodaj_obiekt ( NULL, &S)) return 0; 
      }  
    
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_type = Layers[e_layer].line_type;
		  ep_width = get_width(Layers[e_layer].line_type);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_type = Layers_DXF[e_layer].line_type;
		  ep_width = get_width(Layers_DXF[e_layer].line_type);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    e_type=find_type(buf1,e_type);
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 440:  /*transparency*/
	  if (sscanf(p1, "%d", &ep_translucency) < 1) return 0;
	  break;
  case 999: /*comment*/
	  ret123 = check_width_7(p1, &ep_width);
	  if (!ret123)
	  {
		  ret123 = check_dim_head(p1, &obiektt1, &obiektt2, &obiektt3);
	  }
	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x1)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y1)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z1)  < 1 ) return 0;
    break;
  case 11:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x2)  < 1 ) return 0;
    break;
  case 21:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y2)  < 1 ) return 0;
    break;
  case 31:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z2)  < 1 ) return 0;
    break;
  case 12:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x3)  < 1 ) return 0;
    break;
  case 22:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y3)  < 1 ) return 0;
    break;
  case 32:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z3)  < 1 ) return 0;
    break;  
  case 13:  /*start point X - double*/
    S.lp=8;
    S.n=40;
    if ( sscanf ( p1 , "%lf", &e_x4)  < 1 ) return 0;
    break;
  case 23:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y4)  < 1 ) return 0;
    break;
  case 33:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z4)  < 1 ) return 0;
    break;  
  default:  break;  
  } /*switch*/
 }
 return 1;
}


BOOL read_3dface(FILE *f,int to_block, BOOL block)
/*-----------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned char e_layer=0;
  unsigned char e_type=64;
  unsigned int e_flags=0;
  double e_elevation=0;
  int e_color=7;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_x1, e_y1, e_z1, e_x2, e_y2, e_z2, e_x3, e_y3, e_z3, e_x4, e_y4, e_z4 ;
  char buf[MAXLINE], buf1[MAXLINE];
  WIELOKAT S=S4def;
  LINIA L=Ldef;
  char *base_adr;
  BLOK *base_block,*base_block1;
  long_long base_adr_dane=0;

  ep_translucency = 255;
  e_extrusion_x=0;
  e_extrusion_y=0;
  e_extrusion_z=1;
  ep_space=0;
  ep_novisibility = 0;
 ep_width=line_width.normal*100;
 endentitie=FALSE;
 S.lp=6;
 S.n=32;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

    p1=buf1;
  }  

  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }

    endentitie=TRUE;
    /*generowanie linii*/
    S.xy[0]=jednostkiOb(e_x1 - EXTMIN_X);
    S.xy[1]=jednostkiOb(e_y1 - EXTMIN_Y);
    S.xy[2]=jednostkiOb(e_x2 - EXTMIN_X);
    S.xy[3]=jednostkiOb(e_y2 - EXTMIN_Y);
    S.xy[4]=jednostkiOb(e_x3 - EXTMIN_X);
    S.xy[5]=jednostkiOb(e_y3 - EXTMIN_Y);
    
    if (S.lp==8)
     {
      S.xy[6]=jednostkiOb(e_x4 - EXTMIN_X);
      S.xy[7]=jednostkiOb(e_y4 - EXTMIN_Y);
     } 
      else
     {
      S.xy[6]=jednostkiOb(e_x1 - EXTMIN_X);
      S.xy[7]=jednostkiOb(e_y1 - EXTMIN_Y);
     }
    S.kolor=e_color; 
    S.warstwa=e_layer;
    S.przec=ep_space;
    S.empty_typ=0;
    
    /*rozpoczecie polilinii*/
    if (to_block == 0) 
    {
      if (FALSE == add_block_pline ())
       {
         return 0;
       }
    }
     else 
      {
        if (FALSE == add_block_pline_in_block ())
         {
          return 0;
         }  
      }  


  if (to_block==1)  //zagniezdzenie blokow
  {
     base_block=(BLOK *)dane;
	 base_adr=dane+sizeof(NAGLOWEK)+B3+base_block->dlugosc_opisu_obiektu;
	 base_block1=(BLOK *)base_adr;
	 base_adr_dane=base_adr-dane;
	 if (base_block1->obiekt!=OdBLOK)  //brak potwierdzenie
		 base_adr=dane;  //na wszelki wypadek
  }
   else base_adr=dane;
    
    
    if (to_block == 1) 
     {
      S.blok=1;
     }
     else
      {
       S.blok=0;
      } 

    L.kolor=S.kolor;
    L.warstwa=S.warstwa;
    L.blok=S.blok;
    L.obiektt2=S.obiektt2;
    L.obiektt3=S.obiektt3;
    L.przec=S.przec;
    L.typ=64;

      L.x1=S.xy[0];
      L.y1=S.xy[1];
      L.x2=S.xy[2];
      L.y2=S.xy[3];
      if (NULL == dodaj_obiekt ((BLOK*)base_adr, &L)) return 0; 
	  base_adr=dane+base_adr_dane;

      L.x1=S.xy[2];
      L.y1=S.xy[3];
      L.x2=S.xy[4];
      L.y2=S.xy[5];
      if (NULL == dodaj_obiekt ((BLOK*)base_adr, &L)) return 0;
	  base_adr=dane+base_adr_dane;

      L.x1=S.xy[4];
      L.y1=S.xy[5];
      L.x2=S.xy[6];
      L.y2=S.xy[7];
      if (NULL == dodaj_obiekt ((BLOK*)base_adr, &L)) return 0;
	  base_adr=dane+base_adr_dane;

    if (S.lp==8 /*&& (e_flags & 8 == 0)*/)
    {
      L.x1=S.xy[6];
      L.y1=S.xy[7];
      L.x2=S.xy[0];
      L.y2=S.xy[1];
      if (NULL == dodaj_obiekt ((BLOK*)base_adr, &L)) return 0;
	  base_adr=dane+base_adr_dane;
    }  

    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_type = Layers[e_layer].line_type;
		  ep_width = get_width(Layers[e_layer].line_type);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_type = Layers_DXF[e_layer].line_type;
		  ep_width = get_width(Layers_DXF[e_layer].line_type);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    e_type=find_type(buf1,e_type);
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 440:  /*transparency*/
	  if (sscanf(p1, "%d", &ep_translucency) < 1) return 0;
	  break;
  case 999: /*comment*/
	  check_width_7(p1, &ep_width);
	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x1)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y1)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z1)  < 1 ) return 0;
    break;
  case 11:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x2)  < 1 ) return 0;
    break;
  case 21:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y2)  < 1 ) return 0;
    break;
  case 31:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z2)  < 1 ) return 0;
    break;
  case 12:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x3)  < 1 ) return 0;
    break;
  case 22:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y3)  < 1 ) return 0;
    break;
  case 32:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z3)  < 1 ) return 0;
    break;  
  case 13:  /*start point X - double*/
    S.lp=8;
    S.n=40;
    if ( sscanf ( p1 , "%lf", &e_x4)  < 1 ) return 0;
    break;
  case 23:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y4)  < 1 ) return 0;
    break;
  case 33:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z4)  < 1 ) return 0;
    break;  
  case 70: /*3dface flag - optional 0*/
    if ( sscanf (p1 , "%d", &e_flags) < 1 ) return 0;
    break;
    
  default:  break;  
  } /*switch*/
 }
 return 1;
}



BOOL read_arc(FILE *f,int to_block, BOOL block)
/*-------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned char e_layer=0;
  unsigned char e_type=64;
  double e_elevation=0;
  int e_color=7;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_x, e_y, e_z, e_r=0, e_kp=0, e_kk=0;
  double e_x1, e_y1;
  double e_r1, e_kp1, e_kk1;
  char buf[MAXLINE], buf1[MAXLINE];
  LUK l=ldef;
  int ret123;
  char obiektt1 = 0, obiektt2 = 0, obiektt3 = 0;
  int counterclockwise_flag = 1;
  

  e_extrusion_x=0;
  e_extrusion_y=0;
  e_extrusion_z=1;
  ep_space=0;
  ep_novisibility = 0;
 ep_width=line_width.normal*100;
 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

    p1=buf1;
  }  
  
  
  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
    endentitie=TRUE;
    /*generowanie linii*/
    e_x1=jednostkiOb(e_x - EXTMIN_X);
    e_y1=jednostkiOb(e_y - EXTMIN_Y);
    e_r1=jednostkiOb(e_r);
    if (Check_if_LE(e_r1,0)==TRUE)
     {
      return 1; /*zignorowanie luku*/
     }

      if (counterclockwise_flag)
      {
          e_kp1 = e_kp * Pi2 / 360;
          e_kk1 = e_kk * Pi2 / 360;
      }
      else
      {
          e_kk1 = e_kp * Pi2 / 360;
          e_kp1 = (360-e_kk) * Pi2 / 360;
      }

    if (TRUE==Check_if_Equal(e_kp1, e_kk1))
     {
      return 1;  /*zignorowanie luku*/
     }
    
    l.x=e_x1;
    l.y=e_y1;
    l.r=e_r1;

    l.kat1 = e_kp1;
    l.kat2 = e_kk1;

    l.typ=get_typ(e_type, ep_width);
    l.kolor=e_color; 
    l.warstwa=e_layer;
    l.przec=ep_space;

	l.obiektt1 = obiektt1;
	l.obiektt2 = obiektt2;
	l.obiektt3 = obiektt3;

    if (Check_if_Equal(e_extrusion_z,-1.0)==TRUE)
     {
      l.atrybut=Ablok;
      transformacja_blok ((char *)&l, ((char *)&l)+1, jednostkiOb(- EXTMIN_X), 0, jednostkiOb(- EXTMIN_X), 100, Tmirror,0) ; //!!!!!!!!!!!!!!!!!!!!!
      l.atrybut=Anormalny;
     }
  
    if (to_block == 1) 
     {
      l.blok=1;
      if (NULL == dodaj_obiekt ((BLOK*)dane, &l)) return 0; 
     }
     else
      {
       l.blok=0;
       if (NULL == dodaj_obiekt ( NULL, &l)) return 0; 
      } 
    
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_type = Layers[e_layer].line_type;
		  ep_width = get_width(Layers[e_layer].line_type);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_type = Layers_DXF[e_layer].line_type;
		  ep_width = get_width(Layers_DXF[e_layer].line_type);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    e_type=find_type(buf1,e_type);
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  ret123 = check_width_7(p1, &ep_width);
	  if (!ret123)
	  {
		  ret123 = check_dim_arc(p1, &obiektt1, &obiektt2, &obiektt3);
	  }
	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z)  < 1 ) return 0;
    break;
  case 40:  /*radius - double*/
    if ( sscanf ( p1 , "%lf", &e_r)  < 1 ) return 0;
    break;
  case 50:  /*start angle - double*/
    if ( sscanf ( p1 , "%lf", &e_kp)  < 1 ) return 0;
    break; 
  case 51:  /*end angle - double*/
    if ( sscanf ( p1 , "%lf", &e_kk)  < 1 ) return 0;
    break;
  case 73:  /*counterclockwise flag*/
      if (sscanf(p1, "%d", &counterclockwise_flag) < 1) return -1;
  break;
  default:  break;  
  } /*switch*/
 }
 return 1;
}



BOOL read_ellipse(FILE *f,int to_block, BOOL block)
/*-----------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned char e_layer=0;
  unsigned char e_type=64;
  double e_elevation=0;
  int e_color=7;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_x, e_y, e_z, e_r=0, e_kp=0, e_kk=0;
  double ek_x, ek_y, ek_z;
  char buf[MAXLINE], buf1[MAXLINE];
  LINIA L=Ldef;
  ELLIPSE e=Edef;
  ELLIPTICALARC ea=eldef;
  double pa, pb; // , pu;
  double kos, koc;
  int to_block1;

  char *base_adr;
  BLOK* base_block;
  double angle;
  char *e_ea;
  double vx1, vy1, vx2, vy2;
  double dot,det;
  double kat1, kat2;

  to_block1=to_block;
  e_extrusion_x=0;
  e_extrusion_y=0;
  e_extrusion_z=1;
  ep_space=0;
  ep_novisibility = 0;
 ep_width=line_width.normal*100;
 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

    p1=buf1;
  }  
  
  
  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
    endentitie=TRUE;

	pa=(sqrt((ek_x * ek_x) + (ek_y * ek_y)));  //dlugosc wiekszej polosi
	pb=e_r*pa;  //dlugosc mniejszej polosi

	//wyznaczenie kata
	kos=ek_y/(-pa);
	koc=ek_x/(-pa);

    angle=Atan2(ek_y, ek_x);
    angle=Angle_Normal(angle);

    if (((Check_if_Equal(e_kp, 0.0)) && (Check_if_Equal(e_kk, 2*Pi))) || //ellipse
        (Check_if_Equal((e_kk-e_kp), 2*Pi))) {
        e.x = jednostkiOb(e_x - EXTMIN_X);
        e.y = jednostkiOb(e_y - EXTMIN_Y);
        e.angle = angle;
        e.rx = jednostkiOb(pa);
        e.ry = jednostkiOb(pb);

        e.typ = get_typ(e_type, ep_width);
        e.kolor = e_color;
        e.warstwa = e_layer;
        e.przec = ep_space;
        e.translucency = 255;

        e_ea=(char*)&e;
    }
    else
    {
        e_kp=Angle_Normal(e_kp);
        e_kk=Angle_Normal(e_kk);

        if (Check_if_Equal(pa, 0.0)) return 1;

        ea.x = jednostkiOb(e_x - EXTMIN_X);
        ea.y = jednostkiOb(e_y - EXTMIN_Y);
        ea.angle = angle;

        ea.rx = jednostkiOb(pa);
        ea.ry = jednostkiOb(pb);

        //kat1
        if (Check_if_Equal(e_kp, 0.0)) kat1=0.0;
        else if (Check_if_Equal(e_kp, Pi)) kat1=Pi;
        else {
            kos = sin(e_kp);
            koc = cos(e_kp);

            vx1 = ea.rx * koc;
            vy1 = sqrt(fabs(ea.ry * ea.ry * (1.0 - ((vx1 * vx1) / (ea.rx * ea.rx)))));

            if (e_kp > Pi) vy1 *= (-1);

            vx2 = 1.0;
            vy2 = 0.0;

            dot = vx1 * vx2 + vy1 * vy2;  // Dot product between [vx1, vy1] and [vx2, vy2]
            det = vx1 * vy2 - vy1 * vx2;  // Determinant
            kat1 = -atan2(det, dot);
        }

        //kat2
        if (Check_if_Equal(e_kk, 0.0)) kat2=0.0;
        else if (Check_if_Equal(e_kk, Pi)) kat2=Pi;
        else {
            kos = sin(e_kk);
            koc = cos(e_kk);

            vx1 = ea.rx * koc;
            vy1 = sqrt(fabs(ea.ry * ea.ry * (1.0 - ((vx1 * vx1) / (ea.rx * ea.rx)))));

            if (e_kk > Pi) vy1 *= (-1);

            vx2 = 1.0;
            vy2 = 0.0;

            dot = vx1 * vx2 + vy1 * vy2;  // Dot product between [vx1, vy1] and [vx2, vy2]
            det = vx1 * vy2 - vy1 * vx2;  // Determinant
            kat2 = -atan2(det, dot);
        }

        ea.kat1=Angle_Normal(kat1);
        ea.kat2=Angle_Normal(kat2);

        ea.typ = get_typ(e_type, ep_width);
        ea.kolor = e_color;
        ea.warstwa = e_layer;
        ea.przec = ep_space;

        e_ea=(char*)&ea;
    }

        if (to_block == 1) {
            ((NAGLOWEK*)e_ea)->blok = ElemBlok;
            if (NULL == dodaj_obiekt((BLOK*)dane, e_ea)) return 0;
        }
        else
        {
            ((NAGLOWEK*)e_ea)->blok = NoElemBlok;
            if (NULL == dodaj_obiekt(NULL, e_ea)) return 0;
        }


    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_type = Layers[e_layer].line_type;
		  ep_width = get_width(Layers[e_layer].line_type);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_type = Layers_DXF[e_layer].line_type;
		  ep_width = get_width(Layers_DXF[e_layer].line_type);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    e_type=find_type(buf1,e_type);
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  check_width_7(p1, &ep_width);
	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z)  < 1 ) return 0;
    break;
  case 11:  /*end point X - double*/
    if ( sscanf ( p1 , "%lf", &ek_x)  < 1 ) return 0;
    break;
  case 21:  /*end point Y - double*/
    if ( sscanf ( p1 , "%lf", &ek_y)  < 1 ) return 0;
    break;
  case 31:  /*end point Z - double*/
    if ( sscanf ( p1 , "%lf", &ek_z)  < 1 ) return 0;
    break;
  case 40:  /*radius - double*/
    if ( sscanf ( p1 , "%lf", &e_r)  < 1 ) return 0;
    break;
  case 41:  /*start parameter - double*/
    if ( sscanf ( p1 , "%lf", &e_kp)  < 1 ) return 0;
    break; 
  case 42:  /*end parameter - double*/
    if ( sscanf ( p1 , "%lf", &e_kk)  < 1 ) return 0;
    break;     
  default:  break;  
  } /*switch*/
 }
 return 1;
}


BOOL read_circle(FILE *f,int to_block, BOOL block)
/*----------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned char e_layer=0;
  unsigned char e_type=64;
  double e_elevation=0;
  int e_color=7;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_x, e_y, e_z, e_r=0;
  double e_x1, e_y1;
  double e_r1;
  char buf[MAXLINE], buf1[MAXLINE];
  OKRAG O=Odef;

  e_extrusion_x=0;
  e_extrusion_y=0;
  e_extrusion_z=1;
  ep_space=0;
  ep_novisibility = 0;
 ep_width=line_width.normal*100;
 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
  
  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 )
  {
	  return 0;
  }

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
  
    p1=buf1;
  }  
  
  
  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
    endentitie=TRUE;
    /*generowanie okregu*/
    e_x1=jednostkiOb(e_x - EXTMIN_X);
    e_y1=jednostkiOb(e_y - EXTMIN_Y);
    e_r1=jednostkiOb(e_r);
	if (TRUE == Check_if_LE (e_r1, 0)) return 1;
    O.x=e_x1;
    O.y=e_y1;
    O.r=e_r1;

	if (TRUE == Check_if_LE (O.r, 0)) return 1;  //zignorowanie blednego okregu
    O.typ=get_typ(e_type, ep_width);
    O.kolor=e_color; 
    O.warstwa=e_layer;
    O.przec=ep_space;
  
    if (to_block == 1) 
     {
      O.blok=1;
      if (NULL == dodaj_obiekt ((BLOK*)dane, &O)) return 0;
     }
     else
      {
       O.blok=0;
       if (NULL == dodaj_obiekt ( NULL, &O)) return 0; 
      }
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_type = Layers[e_layer].line_type;
		  ep_width = get_width(Layers[e_layer].line_type);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_type = Layers_DXF[e_layer].line_type;
		  ep_width = get_width(Layers_DXF[e_layer].line_type);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    e_type=find_type(buf1,e_type);
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  check_width_7(p1, &ep_width);
	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_x)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_y)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_z)  < 1 ) return 0;
    break;
  case 40:  /*radius - double*/
    if ( sscanf ( p1 , "%lf", &e_r)  < 1 ) 
	{
		return 0;
	}
    break;
  default:  break;  
  } /*switch*/
 }
 return 1;
}


BOOL read_point(FILE *f,int to_block, BOOL block)
/*---------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned char e_layer=0;
  unsigned char e_type=64;
  double e_elevation=0;
  int e_color=7;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_xp, e_yp, e_zp;
  double e_xp1, e_yp1;
  char buf[MAXLINE], buf1[MAXLINE];
  T_Point P=PointDef;

  e_extrusion_x=0;
  e_extrusion_y=0;
  e_extrusion_z=1;
  ep_space=0;
  ep_novisibility = 0;
  ep_novisibility = 0;
 ep_width=line_width.normal*100;
 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

    p1=buf1;
  }  

  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
    endentitie=TRUE;
    /*generowanie linii*/
    e_xp1=jednostkiOb(e_xp - EXTMIN_X);
    e_yp1=jednostkiOb(e_yp - EXTMIN_Y);
    
    P.x=e_xp1;
    P.y=e_yp1;
    P.kolor=e_color; 
    P.warstwa=e_layer;
    P.przec=ep_space;
    if (to_block == 1) 
     {
      P.blok=1;
      if (NULL == dodaj_obiekt ((BLOK*)dane, &P)) return 0;
     }
     else
      {
       P.blok=0;
       if (NULL == dodaj_obiekt ( NULL, &P)) return 0;
      } 
    
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_type = Layers[e_layer].line_type;
		  ep_width = get_width(Layers[e_layer].line_type);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_type = Layers_DXF[e_layer].line_type;
		  ep_width = get_width(Layers_DXF[e_layer].line_type);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string - nieistotne*/
    e_type=find_type(buf1,e_type);
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_xp)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_yp)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_zp)  < 1 ) return 0;
    break;
  default:  break;  
  } /*switch*/
 }
 return 1;
}

BOOL read_line(FILE *f,int to_block, BOOL block)
/*--------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned char e_layer=0;
  unsigned char e_type=64;
  double e_elevation=0;
  int e_color=7;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_xp, e_yp, e_zp, e_xk, e_yk, e_zk;
  double e_xp1, e_yp1;
  double e_xk1, e_yk1;
  char buf[MAXLINE], buf1[MAXLINE];
  LINIA L=Ldef;
  BOOL ret123;
  char obiektt1 = 0, obiektt2 = 0, obiektt3 = 0;
  

 e_extrusion_x=0;
 e_extrusion_y=0;
 e_extrusion_z=1;
 ep_space=0;
 ep_layer=0;
 e_layer=0;
 ep_type=64;
 e_type=64;
 ep_color=7;
 ep_novisibility = 0;
 ep_width=line_width.normal*100;
 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

    p1=buf1;
  }  
  
  
  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
	 
    endentitie=TRUE;
    /*generowanie linii*/
    e_xp1=jednostkiOb(e_xp - EXTMIN_X);
    e_yp1=jednostkiOb(e_yp - EXTMIN_Y);
    e_xk1=jednostkiOb(e_xk - EXTMIN_X);
    e_yk1=jednostkiOb(e_yk - EXTMIN_Y);
    
    L.x1=e_xp1;
    L.y1=e_yp1;
    L.x2=e_xk1;
    L.y2=e_yk1;
    L.typ=get_typ(e_type, ep_width);   //!!!!!!!!!!!!!!!!!!!!
    L.kolor=e_color; 
    L.warstwa=e_layer;
    L.przec=ep_space;

	L.obiektt1 = obiektt1;
	L.obiektt2 = obiektt2;
	L.obiektt3 = obiektt3;

    if (to_block == 1) 
     {
      L.blok=1;
      if (NULL == dodaj_obiekt ((BLOK*)dane, &L)) return 0; 
     }
     else
      {
       L.blok=0;
       if (NULL == dodaj_obiekt ( NULL, &L)) return 0; 
      } 
    
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_type = Layers[e_layer].line_type;
		  ep_width = get_width(Layers[e_layer].line_type);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_type = Layers_DXF[e_layer].line_type;
		  ep_width = get_width(Layers_DXF[e_layer].line_type);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    e_type=find_type(buf1,e_type);
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  ret123=check_width_7(p1, &ep_width);
	  if (!ret123)
	  {
		  ret123 = check_dim_line(p1, &obiektt1, &obiektt2, &obiektt3);
	  }

	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_xp)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_yp)  < 1 ) return 0;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_zp)  < 1 ) return 0;
    break;
  case 11:  /*end point X - double*/
    if ( sscanf ( p1 , "%lf", &e_xk)  < 1 ) return 0;
    break;
  case 21:  /*end point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_yk)  < 1 ) return 0;
    break;
  case 31:  /*end point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_zk)  < 1 ) return 0;
    break;
  default:  break;  
  } /*switch*/
 }
 return 1;
}

BOOL read_mline(FILE *f,int to_block, BOOL block)
/*----------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  unsigned char e_layer=0;
  unsigned char e_type=64;
  double e_elevation=0;
  int e_color=7;
  int e_close=1;            //1=open, 3=close
  int e_justification=0;    //0=top, 1=zero, 2=bottom
  int e_number_of_v=0;
  int e_number_of_el=0;
  double e_scale_factor=1;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_xp, e_yp, e_zp, e_xk, e_yk, e_zk;
  double e_xk0, e_yk0, e_zk0;
  double e_xp1, e_yp1;
  double e_xk1, e_yk1;
  char buf[MAXLINE], buf1[MAXLINE];
  LINIA L=Ldef;
  BOOL first_mline=FALSE;
  

  e_extrusion_x=0;
 e_extrusion_y=0;
 e_extrusion_z=1;
  ep_space=0;
  ep_novisibility = 0;
 ep_width=line_width.normal*100;
 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  

  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

    p1=buf1;
  }  
  
  
  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
	  
    endentitie=TRUE;
    /*generowanie linii*/
    if (first_mline==TRUE)
     {
      e_xp1=jednostkiOb(e_xp - EXTMIN_X);
      e_yp1=jednostkiOb(e_yp - EXTMIN_Y);
      e_xk1=jednostkiOb(e_xk - EXTMIN_X);
      e_yk1=jednostkiOb(e_yk - EXTMIN_Y);
    
      L.x1=e_xp1;
      L.y1=e_yp1;
      L.x2=e_xk1;
      L.y2=e_yk1;
      L.typ=get_typ(e_type, ep_width);
      L.kolor=e_color;
      L.warstwa=e_layer;
      L.przec=ep_space;
      if (to_block == 1)
       {
        L.blok=1;
        if (NULL == dodaj_obiekt ((BLOK*)dane, &L)) return 0;
       }
       else
        {
         L.blok=0;
         if (NULL == dodaj_obiekt ( NULL, &L)) return 0;
        }
    /*generowanie linii zamykajacej*/

     if (e_close==3)
      {
       e_xp1=jednostkiOb(e_xk - EXTMIN_X);
       e_yp1=jednostkiOb(e_yk - EXTMIN_Y);
       e_xk1=jednostkiOb(e_xk0 - EXTMIN_X);
       e_yk1=jednostkiOb(e_yk0 - EXTMIN_Y);
    
       L.x1=e_xp1;
       L.y1=e_yp1;
       L.x2=e_xk1;
       L.y2=e_yk1;
       L.typ=get_typ(e_type, ep_width);
       L.kolor=e_color;
       L.warstwa=e_layer;
       L.przec=ep_space;
       if (to_block == 1)
        {
         L.blok=1;
         if (NULL == dodaj_obiekt ((BLOK*)dane, &L)) return 0;
        }
        else
         {
          L.blok=0;
          if (NULL == dodaj_obiekt ( NULL, &L)) return 0;
         }
      }
     }
    
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  e_layer = find_layer(buf1);
		  e_type = Layers[e_layer].line_type;
		  ep_width = get_width(Layers[e_layer].line_type);
		  e_color = Layers[e_layer].color;
	  }
	  else
	  {
		  e_layer = find_layer_DXF(buf1);
		  e_type = Layers_DXF[e_layer].line_type;
		  ep_width = get_width(Layers_DXF[e_layer].line_type);
		  e_color = Layers_DXF[e_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    e_type=find_type(buf1,e_type);
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &e_color)  < 1 ) return 0;
    e_color=GetColorALF(e_color, e_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 40:
    if ( sscanf ( p1 , "%lf", &e_scale_factor)  < 1 ) return 0;
    break;
  case 70:
    if ( sscanf ( p1 , "%d", &e_justification)  < 1 ) return 0;
    break;
  case 71:
    if ( sscanf ( p1 , "%d", &e_close)  < 1 ) return 0;
    break;
  case 72:
    if ( sscanf ( p1 , "%d", &e_number_of_v)  < 1 ) return 0;
    break;
  case 73:
    if ( sscanf ( p1 , "%d", &e_number_of_el)  < 1 ) return 0;
    break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  check_width_7(p1, &ep_width);
	  break;
  case 10:  /*start point X - double*/
    if ( sscanf ( p1 , "%lf", &e_xp)  < 1 ) return 0;
    e_xk0=e_xp;
    break;
  case 20:  /*start point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_yp)  < 1 ) return 0;
    e_yk0=e_yp;
    break;
  case 30:  /*start point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_zp)  < 1 ) return 0;
    e_zk0=e_zp;
    break;
  case 11:  /*end point X - double*/
    if (first_mline==TRUE)
     {
      e_xp1=jednostkiOb(e_xp - EXTMIN_X);
      e_yp1=jednostkiOb(e_yp - EXTMIN_Y);
      e_xk1=jednostkiOb(e_xk - EXTMIN_X);
      e_yk1=jednostkiOb(e_yk - EXTMIN_Y);
    
      L.x1=e_xp1;
      L.y1=e_yp1;
      L.x2=e_xk1;
      L.y2=e_yk1;
      L.typ=get_typ(e_type, ep_width);
      L.kolor=e_color;
      L.warstwa=e_layer;
      L.przec=ep_space;
	  if (!ep_novisibility)
	  {
		  if (to_block == 1)
		  {
			  L.blok = 1;
			  if (NULL == dodaj_obiekt((BLOK*)dane, &L)) return 0;
		  }
		  else
		  {
			  L.blok = 0;
			  if (NULL == dodaj_obiekt(NULL, &L)) return 0;
		  }
	  }
      e_xp=e_xk;
      e_yp=e_yk;
      e_zp=e_zk;
     }
      else first_mline=TRUE;

    if ( sscanf ( p1 , "%lf", &e_xk)  < 1 ) return 0;
    break;
  case 21:  /*end point Y - double*/
    if ( sscanf ( p1 , "%lf", &e_yk)  < 1 ) return 0;
    break;
  case 31:  /*end point Z - double*/
    if ( sscanf ( p1 , "%lf", &e_zk)  < 1 ) return 0;
    break;
  default:  break;  
  } /*switch*/
 }
 return 1;
}

BOOL read_lwpolyline(FILE *f,int to_block, BOOL block)
/*---------------------------------------------------*/
{ BOOL endentitie;
    char *p, *p1;
    int entitie_code;
    double e_elevation=0;
    unsigned int e_flags=0;
    double e_extrusion_x, e_extrusion_y, e_extrusion_z;
    double e_xp = 0;
    double e_yp = 0;
    int e_lpoints = 2;
    char buf[MAXLINE], buf1[MAXLINE];
    LINIA L=Ldef;
    int to_block1;
    double wypuklosc1;
    double width_p1, width_k1;
    int nw42, nw40, nw41, nw43;

    e_extrusion_x=0;
    e_extrusion_y=0;
    e_extrusion_z=1;
    ep_space=0;
    ep_novisibility = 0;
    ep_layer=0;
    ep_type=64;
    ep_width=line_width.normal*100;
    ep_color=7;
    to_block1 = to_block;
    s_pline.b_line=FALSE;
    s_pline.b_solid=FALSE; 	/*narysowano linie, narysowano obszar*/
    s_pline.width1=0;		/*szerokosc poczatkowa pierwszego segmentu*/
    s_pline.width=0;
    /*czytanie naglowka polilinii*/
    zamkniecie=0;
    wypuklosc=0.0;
    wypuklosc0=0.0;
    wypuklosc1 = 0.0;
    drugi_wezel=FALSE;
    trzeci_wezel=FALSE;
    first_arc=FALSE;
    fit_curve=FALSE;
    myspline=FALSE;
    width_p_standard=0;
    width_k_standard=0;
    nw=0;
    width_p1=0;
    width_k1=0;
    width_p=0;
    width_k=0;
    nw40=0;
    nw41=0;
    nw42=0;
    nw43=0;

    endentitie=FALSE;
    while (endentitie==FALSE)
    {
        if (!inc_nr_linii()) return 0;
        if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

        p=buf;

        if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;


        if (entitie_code!=0)
        {
            if (!inc_nr_linii()) return 0;
            if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

            p1=buf1;
        }


        switch (entitie_code)
        {
            case 0:
                if (ep_novisibility)
                {
                    return 1;
                }
                add_pline_line(e_xp, e_yp, to_block1, Anormalny);
                endentitie=TRUE;
                if (zamkniecie==0) add_pline_line_end(to_block1, e_extrusion_z, Anormalny);
                else
                {
                    add_pline_line (start_x, start_y, to_block1, Anormalny);
                    s_pline.width1=start_width;
                    s_pline.wypuklosc=wypuklosc1;  ////!!!!!
                    add_pline_line_end (to_block1, e_extrusion_z, Anormalny);
                }
                return 1;
                break;
            case 8:  /*layer - sring*/
                if (!block)
                {
                    ep_layer = find_layer(buf1);
                    ep_type = Layers[ep_layer].line_type;
                    ep_color = Layers[ep_layer].color;
                }
                else
                {
                    ep_layer = find_layer_DXF(buf1);
                    ep_type = Layers_DXF[ep_layer].line_type;
                    ep_color = Layers_DXF[ep_layer].color;
                }
                break;
            case 6:  /*line type - string*/
                ep_type=find_type(buf1,ep_type);
                break;
            case 38:  /*elevation - double*/
                if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
                break;
            case 62:  /*color number - int*/
                if ( sscanf ( p1 , "%d", &ep_color)  < 1 ) return 0;
                ep_color=GetColorALF(ep_color, ep_layer);
                break;
            case 67:  /*model (0) /paper (1) space - int*/
                if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
                break;
            case 60:  /*novisibility*/
                if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
                break;
            case 90: /*lwpolyline liczba wierzcholkow*/
                if ( sscanf (p1 , "%d", &e_lpoints) < 1 ) return 0;
                break;
            case 70: /*polyline flag - optional 0*/
                if ( sscanf (p1 , "%d", &e_flags) < 1 ) return 0;
                if (e_flags & 1) zamkniecie=1;
                if (e_flags & 2) fit_curve=TRUE;
                if (e_flags & 4) myspline=TRUE;
                break;
            case 40: /**/
                if (sscanf(p1, "%lf", &width_p1) < 1) return 0;
                if ((nw40==0) && (nw<2))
                {
                    width_p0=width_p1;
                }
                nw40++;
                break;
            case 41: /**/
                if (sscanf(p1, "%lf", &width_k1) < 1) return 0;
                if ((nw41==0) && (nw<2))
                {
                    width_k0=width_k1;
                }
                nw41++;
                break;
            case 43: /**/
                if ( sscanf (p1 , "%lf", &width_p1) < 1 ) return 0;
                if ( sscanf (p1 , "%lf", &width_k1) < 1 ) return 0;
                if ((nw43==0) && (nw<2))
                {
                    width_p0=width_p1;
                    width_k0=width_k1;
                }
                nw43++;
                nw40++;
                nw41++;
                break;
            case 210:  /*extrusion direction X - double*/
                if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
                break;
            case 220:  /*extrusion direction Y - double*/
                if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
                break;
            case 230:  /*extrusion direction Z - double*/
                if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
                break;
            case 370: /*line width*/
                if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
                if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
                else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
                else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
                break;
            case 999: /*comment*/
                check_width_7(p1, &ep_width);
                break;
            case 10:  /*start point X - double*/
                if (!ep_novisibility)
                {
                    if (nw > 0)
                    {
                        if (nw == 1)
                        {
                            start_pline(e_xp, e_yp, to_block1);
                        }
                        else
                        {
                            add_pline_line(e_xp, e_yp, to_block1, Anormalny);
                        }
                    }
                }
                wypuklosc = wypuklosc1;
                wypuklosc1 = 0.0;
                if ( sscanf ( p1 , "%lf", &e_xp)  < 1 ) return 0;
                break;
            case 20:  /*start point Y - double*/
                nw++;
                if ( sscanf ( p1 , "%lf", &e_yp)  < 1 ) return 0;
                /*dopisanie linii*/
                break;
            case 42: /*wypuklosc*/
                if ( sscanf (p1 , "%lf", &wypuklosc1) < 1 ) return 0;
                if ((nw42==0) && (nw<2)) wypuklosc0=wypuklosc1;
                nw42++;
                break;
            default:  break;
        } /*switch*/
    }
    return 1;
}


BOOL read_lwpolyline___old(FILE *f,int to_block, BOOL block)
/*---------------------------------------------------*/
{ BOOL endentitie;
  char *p, *p1;
  int entitie_code;
  double e_elevation=0;
  unsigned int e_flags=0;
  double e_extrusion_x, e_extrusion_y, e_extrusion_z;
  double e_xp = 0;
  double e_yp = 0;
  int e_lpoints = 2;
  char buf[MAXLINE], buf1[MAXLINE];
  LINIA L=Ldef;
  int to_block1;

 e_extrusion_x=0;
 e_extrusion_y=0;
 e_extrusion_z=1;
 ep_space=0;
 ep_novisibility = 0;
 ep_layer=0;
 ep_type=64;
 ep_width=line_width.normal*100;
 ep_color=7;
 to_block1 = to_block;
 s_pline.b_line=FALSE;
 s_pline.b_solid=FALSE; 	/*narysowano linie, narysowano obszar*/
 s_pline.width1=0;		/*szerokosc poczatkowa pierwszego segmentu*/
 s_pline.width=0;
 /*czytanie naglowka polilinii*/
 zamkniecie=0;
 wypuklosc=0;
 wypuklosc0=0;
 drugi_wezel=FALSE;
 trzeci_wezel=FALSE;
 first_arc=FALSE;
 fit_curve=FALSE;
 myspline=FALSE;
 width_p_standard=0;
 width_k_standard=0;
 nw=0;
 width_p=0;
 width_k=0;

 endentitie=FALSE;
 while (endentitie==FALSE)
 {
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;

  p=buf;

  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;


  if (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
    if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

    p1=buf1;
  }


  switch (entitie_code)
  {
  case 0:
	  if (ep_novisibility)
	  {
		  return 1;
	  }
    add_pline_line(e_xp, e_yp, to_block1, Anormalny);
    endentitie=TRUE;
    if (zamkniecie==0) add_pline_line_end (to_block1, e_extrusion_z, Anormalny);
     else
      {
       add_pline_line (start_x, start_y, to_block1, Anormalny);
       s_pline.width1=start_width;
       add_pline_line_end (to_block1, e_extrusion_z, Anormalny);
      }
    return 1;
    break;
  case 8:  /*layer - sring*/
	  if (!block)
	  {
		  ep_layer = find_layer(buf1);
		  ep_type = Layers[ep_layer].line_type;
		  ep_color = Layers[ep_layer].color;
	  }
	  else
	  {
		  ep_layer = find_layer_DXF(buf1);
		  ep_type = Layers_DXF[ep_layer].line_type;
		  ep_color = Layers_DXF[ep_layer].color;
	  }
    break;
  case 6:  /*line type - string*/
    ep_type=find_type(buf1,ep_type);
    break;
  case 38:  /*elevation - double*/
    if ( sscanf ( p1 , "%lf", &e_elevation)  < 1 ) return 0;
    break;
  case 62:  /*color number - int*/
    if ( sscanf ( p1 , "%d", &ep_color)  < 1 ) return 0;
    ep_color=GetColorALF(ep_color, ep_layer);
    break;
  case 67:  /*model (0) /paper (1) space - int*/
    if ( sscanf ( p1 , "%d", &ep_space)  < 1 ) return 0;
    break;
  case 60:  /*novisibility*/
	  if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
	  break;
  case 90: /*lwpolyline liczba wierzcholkow*/
    if ( sscanf (p1 , "%d", &e_lpoints) < 1 ) return 0;
    break;
  case 70: /*polyline flag - optional 0*/
    if ( sscanf (p1 , "%d", &e_flags) < 1 ) return 0;
    if (e_flags & 1) zamkniecie=1;
    if (e_flags & 2) fit_curve=TRUE;
    if (e_flags & 4) myspline=TRUE;
    break;
  case 40: /**/
	  if (sscanf(p1, "%lf", &width_p0) < 1) return 0;
	  break;
  case 41: /**/
	  if (sscanf(p1, "%lf", &width_k0) < 1) return 0;
	  break;
  case 43: /**/
    if ( sscanf (p1 , "%lf", &width_p0) < 1 ) return 0;
    if ( sscanf (p1 , "%lf", &width_k0) < 1 ) return 0;
  //  printf("wp=%#lf wk=%#lf \n",width_p, width_k); delay(400);
    break;
  case 210:  /*extrusion direction X - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_x)  < 1 ) return 0;
    break;
  case 220:  /*extrusion direction Y - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_y)  < 1 ) return 0;
    break;
  case 230:  /*extrusion direction Z - double*/
    if ( sscanf ( p1 , "%lf", &e_extrusion_z)  < 1 ) return 0;
	//tymczasowo  !!!!!!!!!!!!!!
	//e_extrusion_z=0;
    break;
  case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
  case 999: /*comment*/
	  check_width_7(p1, &ep_width);
	  break;
  case 10:  /*start point X - double*/
	  if (!ep_novisibility)
	  {
		  if (nw > 0)
		  {
			  if (nw == 1)
			  {
				  start_pline(e_xp, e_yp, to_block1);
			  }
			  else
			  {
				  add_pline_line(e_xp, e_yp, to_block1, Anormalny);
			  }
		  }
	  }
    wypuklosc = 0.0;
    if ( sscanf ( p1 , "%lf", &e_xp)  < 1 ) return 0;
    break;
  case 20:  /*start point Y - double*/
    nw++;
    if ( sscanf ( p1 , "%lf", &e_yp)  < 1 ) return 0;
    /*dopisanie linii*/
    break;
  case 42: /*wypuklosc*/
    if ( sscanf (p1 , "%lf", &wypuklosc) < 1 ) return 0;
    break;
  default:  break;
  } /*switch*/
 }
 return 1;
}


BOOL read_spline(FILE *f, int to_block, BOOL block)
/*---------------------------------------------------*/
{
	BOOL endentitie;
	char *p, *p1;
	int entitie_code;
	//char e_line_type[30];
	double e_elevation = 0;
	unsigned /*char*/ int e_flags = 0;
	double e_extrusion_x, e_extrusion_y, e_extrusion_z;
	double e_xp[NumSplinePoints];
	double e_yp[NumSplinePoints];
	double e_zp[NumSplinePoints];

	double e_xfit[NumSplinePoints];
	double e_yfit[NumSplinePoints];
	double e_zfit[NumSplinePoints];
	double e_xstangent;
	double e_ystangent;
	double e_zstangent;
	double e_xetangent;
	double e_yetangent;
	double e_zetangent;

	int e_lpoints = 2;
	char buf[MAXLINE], buf1[MAXLINE];
	LINIA L = Ldef;
	int s_degree = 3;  //by default is cubic spline
	int s_n_knots = 0;
	int s_n_ctrlpoints = 0;
	int s_n_fitpoints = 0;
	double s_fittolerance = 0.0000001;
	int snw, snwz, smnw, smnwz;
	int ns;
	double start_sx=0, start_sy=0, start_sz=0;
	int ret;
	int spline_type;

	double bcurviness = 0.75;

	e_extrusion_x = 0;
	e_extrusion_y = 0;
	e_extrusion_z = 1;
	ep_space = 0;
	ep_novisibility = 0;
	ep_layer = 0;
	ep_type = 64;
	ep_width = line_width.normal * 100;
	ep_color = 7;
	s_pline.b_line = FALSE;
	s_pline.b_solid = FALSE; 	
	s_pline.width1 = 0;		
	s_pline.width = 0;
	
	zamkniecie = 0;
	wypuklosc = 0;
	wypuklosc0 = 0;
	drugi_wezel = FALSE;
	trzeci_wezel = FALSE;
	first_arc = FALSE;
	fit_curve = FALSE;
	myspline = FALSE;
	planarspline = FALSE;
	linearspline = FALSE;
	width_p_standard = 0;
	width_k_standard = 0;
	snw = 0;
	snwz = 0;
	ns = 0;
	smnw = 0;
	smnwz = 0;
	width_p = 0;
	width_k = 0;

	endentitie = FALSE;
	while (endentitie == FALSE)
	{
		if (!inc_nr_linii()) return 0;
		if (myfgets(buf, MAXLINE, f) == NULL) return 0;

		p = buf;

		if (sscanf(p, "%d", &entitie_code) < 1) return 0;


		if (entitie_code != 0)
		{
			if (!inc_nr_linii()) return 0;
			if (myfgets(buf1, MAXLINE, f) == NULL) return 0;

			p1 = buf1;
		}

		switch (entitie_code)
		{
		case 0:
			if (ep_novisibility)
			{
				return 1;
			}
			endentitie = TRUE;
			//fit points have priority
			if (smnw > 0)
			{
				if (smnw < 2) return 1;
				spline_type = 5;
				ret = add_bspline(e_xfit, e_yfit, e_zfit, to_block, smnw, spline_type, zamkniecie, bcurviness, Anormalny);
			}
			else if (snw > 0)
			{
				if (snw == 2) spline_type = 3;
				else if (snw == 3) spline_type = 4;
			    else spline_type = 6;
				ret = add_bspline(e_xp, e_yp, e_zp, to_block, snw, spline_type, zamkniecie, bcurviness, Anormalny);

			}
			return 1;
			break;
		case 8:  /*layer - sring*/
			if (!block)
			{
				ep_layer = find_layer(buf1);
				ep_type = Layers[ep_layer].line_type;
				ep_color = Layers[ep_layer].color;
			}
			else
			{
				ep_layer = find_layer_DXF(buf1);
				ep_type = Layers_DXF[ep_layer].line_type;
				ep_color = Layers_DXF[ep_layer].color;
			}
			break;
		case 6:  /*line type - string*/
			ep_type = find_type(buf1, ep_type);
			break;
		case 38:  /*elevation - double*/
			if (sscanf(p1, "%lf", &e_elevation) < 1) return 0;
			break;
		case 62:  /*color number - int*/
			if (sscanf(p1, "%d", &ep_color) < 1) return 0;
			ep_color = GetColorALF(ep_color, ep_layer);
			break;
		case 67:  /*model (0) /paper (1) space - int*/
			if (sscanf(p1, "%d", &ep_space) < 1) return 0;
			break;
		case 60:  /*novisibility*/
			if (sscanf(p1, "%d", &ep_novisibility) < 1) return 0;
			break;
		case 90: /*lwpolyline liczba wierzcholkow*/
			if (sscanf(p1, "%d", &e_lpoints) < 1) return 0;
			break;
		case 70: /*polyline flag - optional 0*/
			if (sscanf(p1, "%d", &e_flags) < 1) return 0;
			if (e_flags & 1) zamkniecie = 1;
			if (e_flags & 2) fit_curve = TRUE;
			if (e_flags & 4) myspline = TRUE;
			if (e_flags & 8) planarspline = TRUE;
			if (e_flags & 16) linearspline = TRUE;
			if (e_flags & 1024) linearspline = TRUE;
			break;
		case 71: /*degree of spline*/
			if (sscanf(p1, "%d", &s_degree) < 1) return 0;
			break;
		case 72: /*no of knots*/
			if (sscanf(p1, "%d", &s_n_knots) < 1) return 0;
			break;
		case 73: /*no of control points*/
			if (sscanf(p1, "%d", &s_n_ctrlpoints) < 1) return 0;
			break;
		case 74: /*no of fit points*/
			if (sscanf(p1, "%d", &s_n_fitpoints) < 1) return 0;
			break;
		case 44: /**/
			if (sscanf(p1, "%lf", &s_fittolerance) < 1) return 0;
			break;
		case 43: /**/
			if (sscanf(p1, "%lf", &width_p) < 1) return 0;
			if (sscanf(p1, "%lf", &width_k) < 1) return 0;
			//  printf("wp=%#lf wk=%#lf \n",width_p, width_k); delay(400);
			break;
		case 210:  /*extrusion direction X - double*/
			if (sscanf(p1, "%lf", &e_extrusion_x) < 1) return 0;
			break;
		case 220:  /*extrusion direction Y - double*/
			if (sscanf(p1, "%lf", &e_extrusion_y) < 1) return 0;
			break;
		case 230:  /*extrusion direction Z - double*/
			if (sscanf(p1, "%lf", &e_extrusion_z) < 1) return 0;
			break;
		case 370: /*line width*/
			if (sscanf(p1, "%lf", &ep_width) < 1) return 0;
			if (ep_width == -3) ep_width = line_width.normal * 100;  //standard 0.3 mm
			else if (ep_width == -1) ep_width = line_width.normal * 100;  //standard 0.3 mm  jak warstwa
			else if (ep_width == -2) ep_width = line_width.normal * 100;  //standard 0.3 mm  jak blok
			break;
		case 999: /*comment*/
			if (!check_width_7(p1, &ep_width))
			{
				/*curviness*/
				if (sscanf(p1, "%lf", &bcurviness) < 1) bcurviness=0.25;
			}
			break;
		case 10:  /*start point X - double*/
			//s_degree determines number of control points
			//if s_degree==2  we take 3 control point (snw=3) and we create spline x0,y0; x1,y1; x3,y3=x2,y2
			//if s_degree==3  we take 4 control point (snw=3) and we create spline x0,y0; x1,y1; x2,y2; x3,x3
			//if s_degree>3 we divide spline for s_debree=3 spline
			//always x0,y0=x3,y3 for next segment
			//we do not care about any other parameters
			wypuklosc = 0;
			if (sscanf(p1, "%lf", &e_xp[snw]) < 1) return 0;
			if (ns == 0) start_sx = e_xp[snw];
			break;
		case 20:  /*start point Y - double*/
			if (sscanf(p1, "%lf", &e_yp[snw]) < 1) return 0;
			if (ns == 0) start_sy = e_yp[snw];
			if (snw < NumSplinePoints-1) snw++;
			break;
		case 30:  /*start point Z - double*/
			if (sscanf(p1, "%lf", &e_zp[snwz]) < 1) return 0;
			if (ns == 0) start_sz = e_zp[snwz];
			if (snwz < NumSplinePoints-1) snwz++;
			break;

		case 11:  /*start point X - double*/
			if (sscanf(p1, "%lf", &e_xfit[smnw]) < 1) return 0;
			break;
		case 21:  /*start point Y - double*/
			if (sscanf(p1, "%lf", &e_yfit[smnw]) < 1) return 0;
			if (smnw < NumSplinePoints-1) smnw++;
			break;
		case 31:  /*start point Z - double*/
			if (sscanf(p1, "%lf", &e_zfit[smnwz]) < 1) return 0;
			if (smnwz < NumSplinePoints-1) smnwz++;
			break;

		case 12:  /*x start tangent - double*/
			if (sscanf(p1, "%lf", &e_xstangent) < 1) return -1;
			break;
		case 13:  /*y end tangent - double*/
			if (sscanf(p1, "%lf", &e_xetangent) < 1) return -1;
			break;
		case 22:  /*fit datum Y - double*/
			if (sscanf(p1, "%lf", &e_ystangent) < 1) return -1;
			break;
		case 32:  /*fit datum Y - double*/
			if (sscanf(p1, "%lf", &e_yetangent) < 1) return -1;
			break;
		case 23:  /*fit datum Y - double*/
			if (sscanf(p1, "%lf", &e_zstangent) < 1) return -1;
			break;
		case 33:  /*fit datum Y - double*/
			if (sscanf(p1, "%lf", &e_zetangent) < 1) return -1;
			break;
		case 42: /*wypuklosc*/
			if (sscanf(p1, "%lf", &wypuklosc) < 1) return 0;
			break;
		default:  break;
		} /*switch*/
	}
	return 1;
}

BOOL read_image(FILE *f, int to_block, BOOL block)
/*-----------------------------------------------*/
{
	BOOL endentitie;
	char *p, *p1;
	int entitie_code;
	unsigned char e_layer = 0;
	unsigned char e_type = 64;
	double e_elevation = 0;
	int e_color = 7;
	double e_extrusion_x, e_extrusion_y, e_extrusion_z;
	double e_xp, e_yp, e_zp, u_xp, u_yp, u_zp, v_xp, v_yp, v_zp;
	double e_xp1, e_yp1;
	char buf[MAXLINE], buf1[MAXLINE];
	int i_xp, i_yp;
	int imagedef_ref;
	int image_display = 1;
	int image_clipping_state, image_brightness, image_contrast, image_fade;
	double kos, koc;
	double ll_x, ll_y, ll_x1, ll_y1, dx, dy;
	B_PCX_DEF *dxf_pcx_def;
	B_PCX_DEF b_pcx_def = {0, 1, 0.0, 0.0, 0.0 ,0.0, 0.0, 0, 0, 0, 1, 2, ""};

	e_extrusion_x = 0;
	e_extrusion_y = 0;
	e_extrusion_z = 1;
	ep_space = 0;
	ep_novisibility = 0;
	ep_novisibility = 0;
	ep_width = line_width.normal * 100;
	endentitie = FALSE;
	while (endentitie == FALSE)
	{
		if (!inc_nr_linii()) return 0;
		if (myfgets(buf, MAXLINE, f) == NULL) return 0;

		p = buf;

		if (sscanf(p, "%d", &entitie_code) < 1) return 0;


		if (entitie_code != 0)
		{
			if (!inc_nr_linii()) return 0;
			if (myfgets(buf1, MAXLINE, f) == NULL) return 0;

			p1 = buf1;
		}


		switch (entitie_code)
		{
		case 0:
			if (ep_novisibility)
			{
				return 1;
			}
			endentitie = TRUE;
			/*generowanie linii*/
			e_xp1 = jednostkiOb(e_xp - EXTMIN_X);
			e_yp1 = jednostkiOb(e_yp - EXTMIN_Y);

			//recording image parameters
			//allocation of B_PCX dxf_pcx[pcx_number]
			b_pcx_def.warstwa = e_layer;
			b_pcx_def.kat = atan2(u_yp, u_xp);
			kos = sin(b_pcx_def.kat);
			koc = cos(b_pcx_def.kat);
			dx = sqrt(u_xp*u_xp + u_yp * u_yp);
			dy = sqrt(v_xp*v_xp + v_yp * v_yp);
			ll_x = e_xp1;
			ll_y = e_yp1 + (i_yp*dy);
			Rotate_Point(kos, koc, e_xp1, e_yp1, ll_x, ll_y, &ll_x1, &ll_y1);

			b_pcx_def.x = ll_x1;
			b_pcx_def.y = ll_y1;
			b_pcx_def.i_x = i_xp;
			b_pcx_def.i_x = i_yp;

			b_pcx_def.dx = dx;
			b_pcx_def.dy = dy;

			b_pcx_def.image_ref = imagedef_ref;
			b_pcx_def.display = image_display;

			dxf_pcx_def = (B_PCX_DEF *)malloc(sizeof(B_PCX_DEF));
			memmove(dxf_pcx_def, &b_pcx_def, sizeof(B_PCX_DEF));
			dxf_pcx_def_p[pcx_number] = dxf_pcx_def;
			pcx_number++;

			return 1;
			break;
		case 8:  /*layer - sring*/
			if (!block)
			{
				e_layer = find_layer(buf1);
				e_type = Layers[e_layer].line_type;
				ep_width = get_width(Layers[e_layer].line_type);
				e_color = Layers[e_layer].color;
			}
			else
			{
				e_layer = find_layer_DXF(buf1);
				e_type = Layers_DXF[e_layer].line_type;
				ep_width = get_width(Layers_DXF[e_layer].line_type);
				e_color = Layers_DXF[e_layer].color;
			}
			break;
		case 10:  /*start point X - double*/
			if (sscanf(p1, "%lf", &e_xp) < 1) return 0;
			break;
		case 20:  /*start point Y - double*/
			if (sscanf(p1, "%lf", &e_yp) < 1) return 0;
			break;
		case 30:  /*start point Z - double*/
			if (sscanf(p1, "%lf", &e_zp) < 1) return 0;
			break;
		case 11:  /*U vector x - double*/
			if (sscanf(p1, "%lf", &u_xp) < 1) return 0;
			break;
		case 21:  /*U vector y - double*/
			if (sscanf(p1, "%lf", &u_yp) < 1) return 0;
			break;
		case 31:  /*U vector z - double*/
			if (sscanf(p1, "%lf", &u_zp) < 1) return 0;
			break;
		case 12:  /*V vector x - double*/
			if (sscanf(p1, "%lf", &v_xp) < 1) return 0;
			break;
		case 22:  /*V vector y - double*/
			if (sscanf(p1, "%lf", &v_yp) < 1) return 0;
			break;
		case 32:  /*V vector z - double*/
			if (sscanf(p1, "%lf", &v_zp) < 1) return 0;
			break;
		case 13:  /*number of points x - double*/
			if (sscanf(p1, "%d", &i_xp) < 1) return 0;
			break;
		case 23:  /*number of points y - double*/
			if (sscanf(p1, "%d", &i_yp) < 1) return 0;
			break;
		case 340:  /*hard reference to imagedef object*/
			if (sscanf(p1, "%X", &imagedef_ref) < 1) return 0;
			break;
		case 70:  /*image display*/
			if (sscanf(p1, "%d", &image_display) < 1) return 0;
			break;
		case 280:  /*image clipping state*/
			if (sscanf(p1, "%d", &image_clipping_state) < 1) return 0;
			break;
		case 281:  /*brightness value 0-100*/
			if (sscanf(p1, "%d", &image_brightness) < 1) return 0;
			break;
		case 282:  /*contrast value 0-100*/
			if (sscanf(p1, "%d", &image_contrast) < 1) return 0;
			break;
		case 283:  /*fade value 0-100*/
			if (sscanf(p1, "%d", &image_fade) < 1) return 0;
			break;
		default:  break;
		} /*switch*/
	}
	return 1;
}

BOOL read_imagedef(FILE *f, char *fn, int to_block, BOOL block)
/*----------------------------------------------------------*/
{
	BOOL endobject;
	char *p, *p1;
	int object_code;
	char buf[MAXLINE], buf1[MAXLINE], image_file_name[MAXLINE], image_file_name_png[MAXPATH];
	int i_xp, i_yp;
	double u_p, v_p;
	int handle;
	int image_is_loaded = 1;
	int resolution_units = 2;  //cm
	int i;
	char dir[MAXDIR];
	char drive[MAXDRIVE];
	char file[MAXFILE], file_[MAXFILE];
	char ext[MAXEXT], ext_[MAXEXT];
	int flags;
	char ext_1[5];
	char params[128];
	DWORD runcode;
	B_PCX b_pcx=PCXdef;
	PCXheader head;
	unsigned int width, height, horz_res, vert_res;
	int info_ret;
	int type;
	unsigned char bits_per_pixel;

	//TEMPORARY
	type = 0;

	endobject = FALSE;

	while (endobject == FALSE)
	{
		if (!inc_nr_linii()) return 0;
		if (myfgets(buf, MAXLINE, f) == NULL) return 0;

		p = buf;

		if (sscanf(p, "%d", &object_code) < 1) return 0;

		if (object_code != 0)
		{
			if (!inc_nr_linii()) return 0;
			if (myfgets(buf1, MAXLINE, f) == NULL) return 0;

			p1 = buf1;
		}

		switch (object_code)
		{
		case 0:
			if (ep_novisibility)
			{
				return 1;
			}
			endobject = TRUE;
			/*searching for b_pcx*/
			i = 0;
			while (i < pcx_number)
			{
				if (dxf_pcx_def_p[i]->image_ref == handle)
				{
					strcpy(dxf_pcx_def_p[i]->file_name, image_file_name);
					dxf_pcx_def_p[i]->resolution_units = resolution_units;
					dxf_pcx_def_p[i]->kod_obiektu = 2;  //png

					flags = fnsplit(image_file_name, drive, dir, file, ext);
					if (strlen(drive) == 0)
					{
						flags = fnsplit(fn, drive, dir, file_, ext_);
					}

					strlwr(ext);
					if ((strcmp(ext, ".png") != 0) && (strcmp(ext, ".jpg")))
					{
						strcpy(ext_1, ".png");  //can be also jpg
						sprintf(image_file_name_png, "%s%s%s%s", drive, dir, file, ext_1);

						sprintf(params, "%s %s", image_file_name, image_file_name_png);
						runcode = RunSilent((char*)"image2png.exe", params);  //can be also image2jpg.exe
						if (runcode > 0)
						{
							break;
						}
						strcpy(dxf_pcx_def_p[i]->file_name, image_file_name_png);
						type = 0;
					}
					else
					{
						sprintf(image_file_name_png, "%s%s%s%s", drive, dir, file, ext);
						strcpy(dxf_pcx_def_p[i]->file_name, image_file_name_png);

						if (strcmp(strupr(ext), ".PNG") == 0) type = 0;
						else type = 1;
					}
                    //generating image
					b_pcx.x = dxf_pcx_def_p[i]->x;
					b_pcx.y = dxf_pcx_def_p[i]->y;
					b_pcx.kat = dxf_pcx_def_p[i]->kat;
					b_pcx.dx = dxf_pcx_def_p[i]->dx;
					b_pcx.dy = dxf_pcx_def_p[i]->dy;
					b_pcx.inwersja = 0;
					strcpy(b_pcx.pcx, dxf_pcx_def_p[i]->file_name);
					b_pcx.ignore_print_background = 1; // 0;
					if (dxf_pcx_def_p[i]->display & 8) b_pcx.on_front = 1;
					else b_pcx.on_front = 0;
					b_pcx.set_foreground = 0;
					b_pcx.background = 15; // 255;
					b_pcx.foreground = 255;

					b_pcx.widoczny = 1;
					b_pcx.kod_obiektu = type + 2;

					if (type==0) info_ret = LoadPNGinfo(dxf_pcx_def_p[i]->file_name, &width, &height, &horz_res, &vert_res, &bits_per_pixel);
					else info_ret = LoadJPGinfo(dxf_pcx_def_p[i]->file_name, &width, &height, &horz_res, &vert_res, &bits_per_pixel);
					if (info_ret == 0) break;

					b_pcx.i_x = width;
					b_pcx.i_y = height;

					if (horz_res == 0) horz_res = 150;
					if (vert_res == 0) vert_res = 150;

					head.xmin = 0;
					head.xmax = width;
					head.ymin = 0;
					head.ymax = height;

					head.bits_per_pixel = bits_per_pixel;
					head.horz_res = horz_res;
					head.vert_res = vert_res;

					if (!Load_PNG_JPG_real(&b_pcx, dxf_pcx_def_p[i]->file_name, width, height, horz_res, vert_res, bits_per_pixel, type)) break;

					bitmap_exist = TRUE;
					bitmap_png_exist = TRUE;
					bitmap_view = TRUE;

					break;
				}
				i++;
			}


			return 1;
			break;
		case 1:
			strcpy(image_file_name, buf1);
			break;
		case 5:  /*handle*/
			if (sscanf(p1, "%X", &handle) < 1) return 0;
			break;
		case 10:  /*start point X - double*/
			if (sscanf(p1, "%d", &i_xp) < 1) return 0;
			break;
		case 20:  /*start point Y - double*/
			if (sscanf(p1, "%d", &i_yp) < 1) return 0;
			break;
		case 11:  /*U vector x - double*/
			if (sscanf(p1, "%lf", &u_p) < 1) return 0;
			break;
		case 21:  /*U vector y - double*/
			if (sscanf(p1, "%lf", &v_p) < 1) return 0;
			break;
		case 280:  /*image-is-loaded*/
			if (sscanf(p1, "%d", &image_is_loaded) < 1) return 0;
			break;
		case 281:  /*resolution unit*/
			if (sscanf(p1, "%d", &resolution_units) < 1) return 0;
			break;
		default:  break;
		} /*switch*/
	}
	return 1;

}


BOOL read_entities_dxf(FILE *f, BOOL block, int block_view, int draw_hatch)
/*-----------------------------------------------------------------------*/
{ char dxf_entitie [MAXLINE1], dxf_entitie_code [MAXLINE1] ;
  BOOL endsec;
  char *p;
  int entitie_code;

  ep_space=0;
  ep_width=line_width.normal*100;
  endsec=FALSE;
  if (!inc_nr_linii()) return 0;
  if ( myfgets(dxf_entitie_code , MAXLINE1 , f ) == NULL ) return 0;
  p=dxf_entitie_code;
  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;

  if (!inc_nr_linii()) return 0;

  if ( myfgets(dxf_entitie , MAXLINE1 , f ) == NULL ) return 0;
  
  while (endsec==FALSE)
  {
#ifndef NDEBUG
      printf(dxf_entitie);printf("\n");//delay(400);
#endif

      /*tu nalezy sprawdzic czy dxf_entitie_code ==0*/
   if (entitie_code==0)
   {
   if (strcmp (dxf_entitie, "LINE") == 0)
    { 
     if (!read_line(f,0, block)) return 0;
     goto next_entitie;
    }
   else if (strcmp (dxf_entitie, "MLINE") == 0)
    { 
     if (!read_mline(f,0, block)) return 0;
     goto next_entitie;
    }
   else if (strcmp (dxf_entitie, "POINT") == 0)
    { 
     if (!read_point(f,0, block)) return 0;
     goto next_entitie;
    } 
   else if (strcmp (dxf_entitie, "CIRCLE") == 0)
    { 
     if (!read_circle(f,0, block)) return 0;
     goto next_entitie;
    } 
   else if (strcmp (dxf_entitie, "ARC") == 0)
    { 
     if (!read_arc(f,0, block)) return 0;
     goto next_entitie;
    }
   else if (strcmp (dxf_entitie, "ELLIPSE") == 0)
    {
     if (!read_ellipse(f,0, block)) return 0;
     goto next_entitie;
    }
   else if (strcmp (dxf_entitie, "TRACE") == 0)
    { 
     if (!read_trace(f,0, block)) return 0;
     goto next_entitie;
    } 
   else if (strcmp (dxf_entitie, "SOLID") == 0)
    { 
     if (!read_solid(f,0, block)) return 0;
     goto next_entitie;
    } 
   else if (strcmp (dxf_entitie, "TEXT") == 0)
    { 
     if (!read_text(f,0, block)) return 0;
     goto next_entitie;
    }
   else if (strcmp (dxf_entitie, "MTEXT") == 0)
    { 
     if (!read_mtext(f,0, block)) return 0;
     goto next_entitie;
    } 
   else if (strcmp (dxf_entitie, "XSHAPE") == 0)
    { 
     if (!read_shape(f,0, block)) return 0;
     goto next_entitie;
    }
   else if (strcmp (dxf_entitie, "INSERT") == 0)
    { 
     if (!read_insert(f,0, block)) return 0;
     goto next_entitie;
    }
   else if (strcmp (dxf_entitie, "ATTDEF") == 0)
    { 
     if (!read_attdef(f,0, block)) return 0;
     goto next_entitie;
    }    
   else if (strcmp (dxf_entitie, "ATTRIB") == 0)
    { 
     if (!read_attrib(f,0, block)) return 0;
     goto next_entitie;
    }
   
   else if (strcmp (dxf_entitie, "POLYLINE") == 0)
    { 
     if (!read_pline(f,0, block)) return 0;
     goto next_entitie;
    }
   
   else if (strcmp (dxf_entitie, "LWPOLYLINE") == 0)
    { 
     if (!read_lwpolyline(f,0, block)) return 0;
     goto next_entitie;
    }
   else if (strcmp (dxf_entitie, "SPLINE") == 0)
    { 
     if (!read_spline(f,0, block)) return 0;
     goto next_entitie;
    }  
   else if (strcmp (dxf_entitie, "3DFACE") == 0)
    { 
     if (!read_3dface(f,0, block)) return 0;
     goto next_entitie;
    }   
   else if (strcmp (dxf_entitie, "XVIEWPORT") == 0)
    { 
     if (!read_viewport(f,0, block)) return 0;
     goto next_entitie;
    } 
   else if (strcmp (dxf_entitie, "DIMENSION") == 0)
    { 
     if (!read_dimension(f,0, block)) return 0;
     goto next_entitie;
    } 
   else if (strcmp(dxf_entitie, "ARC_DIMENSION") == 0)  //// ???????????
   {
	   if (!read_dimension(f, 0, block)) return 0;
	   goto next_entitie;
   }
   else if (strcmp(dxf_entitie, "IMAGE") == 0)  //// ???????????
   {
   if (!read_image(f, 0, block)) return 0;
   goto next_entitie;
   }
   else if (strcmp(dxf_entitie, "XXXOLE2FRAME") == 0)  //TO FINISH
   {
       if (!read_ole2frame(f, 0, block)) return 0;
       goto next_entitie;
   }
   else if (strcmp (dxf_entitie, "ENDSEC") == 0)
    {
     endsec=TRUE;
     return 1;
    }
   else if (strcmp (dxf_entitie, "HATCH") == 0)
    {
       if (block_view==0) {
           if (!read_hatch(f, 0, block, draw_hatch)) return 0;
           goto next_entitie;
       }
    }
   else if (strcmp (dxf_entitie, "LEADER") == 0)
    {
       if (!read_leader(f, 0, block)) return 0;
       goto next_entitie;
    }
   else if (strcmp (dxf_entitie, "MULTILEADER") == 0)
   {
       if (!read_multileader(f, 0, block)) return 0;
       goto next_entitie;
   }
  }
        if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_entitie_code , MAXLINE1 , f ) == NULL ) return 0;
   p=dxf_entitie_code;
   if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
   /*tutaj juz trzeba czytac kody kolejne wartosci*/
   
 next_entitie:
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_entitie , MAXLINE1 , f ) == NULL ) return 0;
  } /*while*/    
 return 1; 
}  


BOOL read_objects_dxf(FILE *f, char *fn, BOOL block)
/*------------------------------------------------*/
{
	char dxf_object[MAXLINE1], dxf_object_code[MAXLINE1];
	BOOL endsec;
	char *p;
	int object_code;

	ep_space = 0;
	ep_width = line_width.normal * 100;
	endsec = FALSE;
	if (!inc_nr_linii()) return 0;
	if (myfgets(dxf_object_code, MAXLINE1, f) == NULL) return 0;
	p = dxf_object_code;
	if (sscanf(p, "%d", &object_code) < 1) return 0;


	if (!inc_nr_linii()) return 0;

	if (myfgets(dxf_object, MAXLINE1, f) == NULL) return 0;

	while (endsec == FALSE)
	{
		/*tu nalezy sprawdzic czy dxf_entitie_code ==0*/
		if (object_code == 0)
		{
			if (strcmp(dxf_object, "IMAGEDEF") == 0)
			{
				if (!read_imagedef(f, fn, 0, block)) return 0;
				goto next_object;
			}
			else if (strcmp(dxf_object, "ENDSEC") == 0)
			{
				endsec = TRUE;
				return 1;
			}
		}
		if (!inc_nr_linii()) return 0;
		if (myfgets(dxf_object_code, MAXLINE1, f) == NULL) return 0;
		p = dxf_object_code;
		if (sscanf(p, "%d", &object_code) < 1) return 0;
		/*tutaj juz trzeba czytac kody kolejne wartosci*/

	next_object:
		if (!inc_nr_linii()) return 0;
		if (myfgets(dxf_object, MAXLINE1, f) == NULL) return 0;
	} /*while*/
	return 1;
}


BOOL read_block_dxf(FILE *f, BOOL block, int block_view, int draw_hatch)
/*--------------------------------------------------------------------*/
{ char dxf_entitie [MAXLINE], dxf_entitie_code [MAXLINE] ;
  char *p;
  char *p1;
  BOOL endblk;
  int entitie_code;
  char typ_bloku [60];
  char styl_bloku [60];
  BOOL block_added;
  double e_xp=0, e_yp=0, e_zp=0;
  double e_xp1, e_yp1;
  unsigned int e_layer=0, e_flags=0;
  double e_elevation=0;
  int e_color=7, e_type=64;
  BOOL color_def = FALSE;
  double distance0x, distance0y, distance1x, distance1y;
  double distance0, distance1;
  char kod_obiektu;
  double angle=0.0;
  int ret123;
  char handle[MAXLINE];

  ep_space=0;
  ep_width=line_width.normal*100;
  block_added=FALSE;
  strcpy(typ_bloku,""); 
  endblk=FALSE;
  if (!inc_nr_linii()) return 0;
  if ( myfgets(dxf_entitie_code , MAXLINE , f ) == NULL ) return 0;
  p=dxf_entitie_code;
  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  
  if (!inc_nr_linii()) return 0;
  if ( myfgets(dxf_entitie , MAXLINE , f ) == NULL ) return 0;
  p1 = dxf_entitie;
  
  while (endblk==FALSE)
  {
   if (entitie_code == 0)
   {
   if (block_added == FALSE)
    {
     distance0x=fabs(e_xp - EXTMIN_X0);
     distance0y=fabs(e_yp - EXTMIN_Y0);

     //szukamy najwiekszego

     if (distance0y>distance0x) distance0=distance0y;
       else distance0=distance0x;

     distance1x=fabs(e_xp);
     distance1y=fabs(e_yp);

     //szukamy najwiekszego

     if (distance1y>distance1x) distance1=distance1y;
       else distance1=distance1x;

     e_xp1=jednostkiOb(e_xp - EXTMIN_X);
     e_yp1=jednostkiOb(e_yp - EXTMIN_Y);
	 if (strstr(typ_bloku, "B_HATCH") != NULL) kod_obiektu = B_HATCH;
     else if (strstr(typ_bloku, "B_DIM3") != NULL) kod_obiektu = B_DIM3;
	 else if (strstr(typ_bloku, "B_DIM2") != NULL) kod_obiektu = B_DIM2;
	 else if (strstr(typ_bloku, "B_DIM1") != NULL) kod_obiektu = B_DIM1;
	 else if (strstr(typ_bloku, "B_DIM") != NULL) kod_obiektu = B_DIM;
	 else if (strstr(typ_bloku, "*D") != NULL) kod_obiektu = B_DIM;
	 else if (strstr(typ_bloku, "B_PLINE") != NULL) kod_obiektu = B_PLINE;
	 else if (strstr(typ_bloku, "B_EXPORT") != NULL) kod_obiektu = B_EXPORT;
     else if (strstr(typ_bloku, "B_GRAPH") != NULL) kod_obiektu = B_GRAPH;
	 else  kod_obiektu = B_EXPORT;

	 if (strstr(typ_bloku, "*Model_Space") != NULL)
	 {
		 if (color_def == TRUE)
		 {
			 if (e_color==7) kolory.paper = 15;
			 else kolory.paper = GetColorALF(e_color, e_layer);
		 }
	 }
	 
     if (FALSE == add_block (e_xp1,e_yp1,kod_obiektu, typ_bloku, TRUE, angle))  /*powinno_byc*/
       {
         return 0;
       } 
     block_added=TRUE;  
    }   
   if (strcmp (dxf_entitie, "LINE") == 0)
    { 
     if (!read_line(f,1, block)) return 0;
     goto next_b_entitie;
    }
   else if (strcmp (dxf_entitie, "MLINE") == 0)
    { 
     if (!read_mline(f,1, block)) return 0;
     goto next_b_entitie;
    }
   else if (strcmp (dxf_entitie, "POINT") == 0)
    { 
     if (!read_point(f,1, block)) return 0;
     goto next_b_entitie;
    } 
   else if (strcmp (dxf_entitie, "CIRCLE") == 0)
    { 
     if (!read_circle(f,1, block)) return 0;
     goto next_b_entitie;
    } 
   else if (strcmp (dxf_entitie, "ARC") == 0)
    {
     if (!read_arc(f,1, block)) return 0;
     goto next_b_entitie;
    }
   else if (strcmp (dxf_entitie, "ELLIPSE") == 0)
    {
     if (!read_ellipse(f,1, block)) return 0;
     goto next_b_entitie;
    }
   else if (strcmp (dxf_entitie, "TRACE") == 0)
    { 
     if (!read_trace(f,1, block)) return 0;
     goto next_b_entitie;
    } 
   else if (strcmp (dxf_entitie, "SOLID") == 0)
    { 
     if (!read_solid(f,1, block)) return 0;
     goto next_b_entitie;
    } 
   else if (strcmp (dxf_entitie, "TEXT") == 0)
    { 
     if (!read_text(f,1, block)) return 0;
     goto next_b_entitie;
    }
   else if (strcmp (dxf_entitie, "MTEXT") == 0)
    { 
     if (!read_mtext(f,1, block)) return 0;
     goto next_b_entitie;
    } 
   else if (strcmp (dxf_entitie, "XSHAPE") == 0)
    { 
     if (!read_shape(f,1, block)) return 0;
     goto next_b_entitie;
    }
   else if (strcmp (dxf_entitie, "INSERT") == 0)
    {
       inserts++;
     if (!read_insert(f,1, block)) return 0;
     goto next_b_entitie;
    }
   else if (strcmp (dxf_entitie, "ATTDEF") == 0)
    { 
     if (!read_attdef(f,1, block)) return 0;
     goto next_b_entitie;
    }    
   else if (strcmp (dxf_entitie, "ATTRIB") == 0)
    { 
     if (!read_attrib(f,1, block)) return 0;
     goto next_b_entitie;
    }
   
   else if (strcmp (dxf_entitie, "POLYLINE") == 0)
    { 
     if (!read_pline(f,1, block)) return 0;
     goto next_b_entitie;
    }

   else if (strcmp (dxf_entitie, "LWPOLYLINE") == 0)
    { 
     if (!read_lwpolyline(f,1, block)) return 0;
     goto next_b_entitie;
    } 
   else if (strcmp(dxf_entitie, "SPLINE") == 0)
   {
	   if (!read_spline(f, 1, block)) return 0;
	   goto next_b_entitie;
   }
   else if (strcmp (dxf_entitie, "3DFACE") == 0)
    { 
     if (!read_3dface(f,1, block)) return 0;
     goto next_b_entitie;
    }   
   else if (strcmp (dxf_entitie, "XVIEWPORT") == 0)
    { 
     if (!read_viewport(f,1, block)) return 0;
     goto next_b_entitie;
    } 
   else if (strcmp (dxf_entitie, "DIMENSION") == 0)
    { 
     if (!read_dimension(f,1, block)) return 0;
     goto next_b_entitie;
    } 
   else if (strcmp(dxf_entitie, "ARC_DIMENSION") == 0)  //// ???????????
   {
   if (!read_dimension(f, 1, block)) return 0;
   goto next_b_entitie;
   }
   else if (strcmp(dxf_entitie, "IMAGE") == 0)  //// ???????????
   {
   if (!read_image(f, 1, block)) return 0;
   goto next_b_entitie;
   }
   else if (strcmp(dxf_entitie, "OLE2FRAME") == 0)
   {
       if (!read_ole2frame(f, 1, block)) return 0;
       goto next_b_entitie;
   }
   else if (strcmp(dxf_entitie, "HATCH") == 0)
	{
       if (block_view==0) {
           if (!read_hatch(f, 1, block, draw_hatch)) return 0;
           goto next_b_entitie;
       }
	}
   else if (strcmp (dxf_entitie, "LEADER") == 0)
    {
       if (!read_leader(f, 1, block)) return 0;
       goto next_b_entitie;
      }
   else if (strcmp (dxf_entitie, "MULTILEADER") == 0)
   {
       if (!read_multileader(f, 1, block)) return 0;
       goto next_b_entitie;
   }
   else if (strcmp (dxf_entitie, "ENDBLK") == 0)
    {
     endblk=TRUE;
     return 1;
    }
   }
   else
   { 
   switch (entitie_code)
    {
   case 8:  
	   if (!block)
	   {
		   e_layer = find_layer(dxf_entitie);
		   e_type = Layers[e_layer].line_type;
		   ep_width = get_width(Layers[e_layer].line_type);
		   e_color = Layers[e_layer].color;
	   }
	   else
	   {
		   e_layer = find_layer_DXF(dxf_entitie);
		   e_type = Layers_DXF[e_layer].line_type;
		   ep_width = get_width(Layers_DXF[e_layer].line_type);
		   e_color = Layers_DXF[e_layer].color;
	   }
      break;
   case 2:  if (USERI1==211) strcpy(typ_bloku,""); 
             else strcpy(typ_bloku," ");
            if (!is_utf8(dxf_entitie))  decodingdxf(dxf_entitie);
            strcat(typ_bloku, dxf_entitie);
      break;
    case 330:
            strcpy(handle,dxf_entitie);
            //searching name in block_record array
            if (USERI1==211) strcpy(typ_bloku,"");
            else strcpy(typ_bloku," ");

            for (int i=0; i<block_record_no_max; i++)
            {
                if (strcmp(handle, block_record[i].handle)==0)
                {
                    strcat(typ_bloku,block_record[i].name);
                    break;
                }
            }
            if (!is_utf8(typ_bloku)) decodingdxf(typ_bloku);
            break;
   case 70: /*flag - optional 0*/
      if ( sscanf (p1 , "%d", &e_flags) < 1 ) return 0;
      break;
   case 370: /*line width*/
    if ( sscanf ( p1 , "%lf", &ep_width)  < 1 ) return 0;
    if (ep_width==-3) ep_width=line_width.normal*100;  //standard 0.3 mm
      else if (ep_width==-1) ep_width=line_width.normal*100;  //standard 0.3 mm  jak warstwa
        else if (ep_width==-2) ep_width=line_width.normal*100;  //standard 0.3 mm  jak blok
    break;
   case 999: /*comment*/
	   ret123=check_width_7(p1, &ep_width);
	   if (!ret123)
	   {
		   ret123 = check_angle(p1, &angle);
	   }
	   break;
   case 10: if ( sscanf ( p1 , "%lf", &e_xp)  < 1 ) return 0;
      break;
   case 20: if ( sscanf ( p1 , "%lf", &e_yp)  < 1 ) return 0;  
      break;
   case 30: if ( sscanf ( p1 , "%lf", &e_zp)  < 1 ) return 0;
      break;
   case 3:  if (USERI1==211) strcpy(styl_bloku,""); 
             else strcpy(styl_bloku," ");
            strcat(styl_bloku, dxf_entitie);
            if (!is_utf8(styl_bloku)) decodingdxf(styl_bloku);
      break;
   case 62:  /*color number - int*/
	   if (sscanf(p1, "%d", &e_color) < 1) return 0;
	   e_color = GetColorALF(e_color, e_layer);
	   color_def = TRUE;
	   break;
    /*tutaj mozna odczytac warstwe (8), nazwe (2), flagi (70), nazwe (3)*/
        /*punkt bazowy (10, 20, 30)*/
    } /*switch*/ 
   } 
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_entitie_code , MAXLINE , f ) == NULL ) return 0;
   p=dxf_entitie_code;
   if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
   
 next_b_entitie:
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_entitie , MAXLINE , f ) == NULL ) return 0;
   p1 = dxf_entitie;

  } /*while*/

  //lastbchecking of th block
  BLOK *new_block;
  new_block=(BLOK*)dane;

  if (new_block->n == (new_block->dlugosc_opisu_obiektu + B3))
    {
        new_block->atrybut=Ausuniety;
        usun_obiekt(new_block);
    }

 return 1; 
}


BOOL read_layer(FILE *f, BOOL block)
/*--------------------------------*/
{ char buf [MAXLINE], buf1 [MAXLINE], *p, *p1;
  BOOL endtab, endlayer;
  int entitie_code, liczba_warstw;
  int layer_flags, layer_color, layer_line_type;
  LAYER layer=Defwar;
  unsigned char e_type=64;

  if (!block) No_Layers = 0;
  else No_Layers_DXF = 0;

  endtab=FALSE;
  if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
  p=buf;
  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  
  while (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
     if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
     if (entitie_code==70)
      {
       p1=buf1;
       if ( sscanf ( p1 , "%d", &liczba_warstw)  < 1 ) return 0;
      } 
	 if (!inc_nr_linii()) return 0;
     if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
     p=buf;
     if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
   }  
    
  while (endtab==FALSE)
  {
   if (entitie_code==0)
    {
	   if (!inc_nr_linii()) return 0;
     if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
     if (strcmp (buf1, "LAYER") == 0)
      {
      /*czytanie az do kodu 0*/
		 if (!block)
		 {
			 if (No_Layers < MAX_NUMBER_OF_LAYERS) No_Layers++;
			 memmove(&(Layers[No_Layers - 1]), &layer, sizeof(LAYER));
		 }
		 else
		 {
			 if (No_Layers_DXF < MAX_NUMBER_OF_LAYERS_DXF) No_Layers_DXF++;
			 memmove(&(Layers_DXF[No_Layers_DXF - 1]), &layer, sizeof(LAYER));
		 }
       endlayer=FALSE;
       while (endlayer==FALSE)
        {
		   if (!inc_nr_linii()) return 0;
          if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
          p=buf;
          if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
          if (entitie_code!=0)
            {
			  if (!inc_nr_linii()) return 0;
             if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;

             p1=buf1;
            }
          switch (entitie_code)
           {
           case 0: endlayer=TRUE;
                break;
           case 2: 
			   if (!block)
			   {
                   if (!is_utf8(buf1)) decodingdxf(buf1);
				   strncpy(Layers[No_Layers - 1].name, buf1, maxlen_w); //nazwa
				   Layers[No_Layers - 1].name[maxlen_w] = '\0';
				   strncpy(Layers_name[No_Layers - 1], buf1, maxlen_w_AC);
				   Layers_name[No_Layers - 1][maxlen_w_AC] = '\0';
			   }
			   else
			   {
                   if (!is_utf8(buf1)) decodingdxf(buf1);
				   strncpy(Layers_DXF[No_Layers_DXF - 1].name, buf1, maxlen_w); //nazwa
				   Layers[No_Layers - 1].name[maxlen_w] = '\0';
				   strncpy(Layers_name_DXF[No_Layers_DXF - 1], buf1, maxlen_w_AC);
				   Layers_name[No_Layers - 1][maxlen_w_AC] = '\0';
			   }
                break;
           case 70:  //standard flag values
			   p1 = buf1;
			   if (sscanf(p1, "%d", &layer_flags) < 1) return 0;
			   if (!block)
			   {
				   if (layer_flags & 1) Layers[No_Layers - 1].point = 0;  //freez
				   if (layer_flags & 4) Layers[No_Layers - 1].edit = 0;  //lock
			   }
			   else
			   {
				   if (layer_flags & 1) Layers_DXF[No_Layers_DXF - 1].point = 0; //freez
				   if (layer_flags & 4) Layers_DXF[No_Layers_DXF - 1].edit = 0;  //lock
			   }
                break;
           case 62:  //color number
			   p1 = buf1;
			   if (sscanf(p1, "%d", &layer_color) < 1) return 0;
			   if (!block)
			   {
				   if (layer_color < 0)
				   {
					   Layers[No_Layers - 1].on = 0;  //on
					   layer_color *= -1;
					   if (layer_color > 255) layer_color = 255;
					   layer_color = GetColorALF(layer_color, 0);
				   }
				   else
				   {
					   if (layer_color > 255) layer_color = 255;
					   layer_color = GetColorALF(layer_color, 0);
				   }
				   //  if (layer_color<0) layer_color = 0;
				   Layers[No_Layers - 1].color = layer_color;
			   }
			   else
			   {
				   if (layer_color < 0)
				   {
					   Layers_DXF[No_Layers_DXF - 1].on = 0;
					   layer_color *= -1;
					   if (layer_color > 255) layer_color = 255;
					   layer_color = GetColorALF(layer_color, 0);
				   }
				   else
				   {
					   if (layer_color > 255) layer_color = 255;
					   layer_color = GetColorALF(layer_color, 0);
				   }
				   //  if (layer_color<0) layer_color = 0;
				   Layers_DXF[No_Layers_DXF - 1].color = layer_color;
			   }
                break;
           case 6:  //linetype name
			   e_type = find_type(buf1, e_type);
			   layer_line_type = find_type(buf1, e_type);
			   if (!block)
			   {
				   Layers[No_Layers - 1].line_type = layer_line_type;
			   }
			   else
			   {
				   Layers_DXF[No_Layers_DXF - 1].line_type = layer_line_type;
			   }
                break;
           case 370:  //line width
			   if (sscanf(p1, "%lf", &ep_width) < 1) return 0;
			   if (ep_width == -3) ep_width = line_width.normal * 100;  //standard 0.3 mm
			   else if (ep_width == -1) ep_width = line_width.normal * 100;  //standard 0.3 mm  jak warstwa
			   else if (ep_width == -2) ep_width = line_width.normal * 100;  //standard 0.3 mm  jak blok
			   layer_line_type = get_typ(ep_type, ep_width);
			   if (!block)
			   {
				   Layers[No_Layers - 1].line_type = layer_line_type;
			   }
			   else
			   {
				   Layers_DXF[No_Layers_DXF - 1].line_type = layer_line_type;
			   }
                break;
           default: break;     
           } 
        }
      }
      else if (strcmp (buf1, "ENDTAB") == 0)
      {
       endtab=TRUE;
      }
    }  
     else
      {
	  if (!inc_nr_linii()) return 0;
       if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
      }
   //next_layer:  ;
  } /*while*/  
  if (!block)
  {
	  if (No_Layers == 0)
	  {
		  No_Layers = 1;
		  memmove(&(Layers[0]), &layer, sizeof(LAYER));
	  }
  }

  /*dodatkowa warstwa 255 dla potrzeb blokow*/
  memmove(&(Layers[255]), &layer, sizeof(LAYER));
  Layers[255].on = 0;
 
 
 return 1; 
}    


BOOL read_appid(FILE *f)
/*------------------------*/
{ char dxf_table [MAXLINE], dxf_table_code [MAXLINE] ;
  BOOL endtab;
  endtab=FALSE;
  while (endtab==FALSE)
  {
	  if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table_code , MAXLINE , f ) == NULL ) return 0;
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table , MAXLINE , f ) == NULL ) return 0;
   if (strcmp (dxf_table, "ENDTAB") == 0)
    {
     endtab=TRUE;
    }
   //next_appid:  ;
  } /*while*/    
 return 1; 
}  

BOOL read_dimstyle(FILE *f)
/*------------------------*/
{ char dxf_table [MAXLINE], dxf_table_code [MAXLINE] ;
  BOOL endtab;
  endtab=FALSE;
  while (endtab==FALSE)
  {
	  if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table_code , MAXLINE , f ) == NULL ) return 0;
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table , MAXLINE , f ) == NULL ) return 0;
   if (strcmp (dxf_table, "ENDTAB") == 0)
    {
     endtab=TRUE;
    }
   //next_dimstyle:  ;
  } /*while*/    
 return 1; 
}

BOOL read_block_record(FILE *f)
/*------------------------*/
{
    char buf [MAXLINE], *p;
    char buf1 [MAXLINE], *p1;
    int table_code;
    BOOL endtab;

    endtab=FALSE;

    if (!inc_nr_linii()) return 0;
    if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
    p=buf;
    if ( sscanf ( p , "%d", &table_code)  < 1 ) return 0;

    if (!inc_nr_linii()) return 0;
    if (myfgets(buf1, MAXLINE, f) == NULL) return 0;
    p1 = buf1;

    while (endtab==FALSE)
    {
        switch (table_code) {
            case 0:
                if (strcmp (buf1, "ENDTAB") == 0)
                {
                    return 1;
                }
            break;
            case 5:
                block_record_no++;
                if (block_record_no==block_record_no_max)
                {
                    block_record_no_max+=100;
                    block_record=(BLOCK_RECORD*)realloc(block_record, block_record_no_max*sizeof(BLOCK_RECORD));
                }
                strcpy(block_record[block_record_no-1].handle, buf1);
                strcpy(block_record[block_record_no-1].name, "");
            break;
            case 2:
                strcpy(block_record[block_record_no-1].name, buf1);
            break;
            default:  break;
        }

        if (!inc_nr_linii()) return 0;
        if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
        p=buf;
        if ( sscanf ( p , "%d", &table_code)  < 1 ) return 0;

        if (!inc_nr_linii()) return 0;
        if (myfgets(buf1, MAXLINE, f) == NULL) return 0;
        p1 = buf1;
    }

    return 1;
}

BOOL read_ltype(FILE *f)
/*------------------------*/
{ char dxf_table [MAXLINE], dxf_table_code [MAXLINE] ;
  BOOL endtab;
  endtab=FALSE;
  while (endtab==FALSE)
  {
	  if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table_code , MAXLINE , f ) == NULL ) return 0;
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table , MAXLINE , f ) == NULL ) return 0;
   if (strcmp (dxf_table, "ENDTAB") == 0)
    {
     endtab=TRUE;
    }
   //next_ltype:  ;
  } /*while*/    
 return 1; 
}

BOOL read_style(FILE *f)
/*------------------------*/
{ char buf [MAXLINE], buf1 [MAXLINE], style_file[MAXLINE], style_file_[MAXLINE], *p, *p1;
  BOOL endtab, endstyle;
  int entitie_code, liczba_styli;
  int style_flags, style_generation_flags;
  double fixed_height, width_factor, oblique_angle;
  LAYER layer=Defwar;
  int font_number0;
  double e_wspx1=1.0;
  char *ptr, *ptr1;
  char dir[MAXDIR];
  char drive[MAXDRIVE];
  char file[MAXFILE];
  char ext[MAXEXT];
  int flags;
  BOOL ignore1000;
  char style_alfa[34];

  No_Styles_DXF=0;  
  endtab=FALSE;
  if (!inc_nr_linii()) return 0;
  if ( myfgets(buf , MAXLINE , f ) == NULL ) return 0;
  p=buf;
  if ( sscanf ( p , "%d", &entitie_code)  < 1 ) return 0;
  
  while (entitie_code!=0)
   {
	  if (!inc_nr_linii()) return 0;
     if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
     if (entitie_code==70)
	  {
	  p1 = buf1;
	  if (sscanf(p1, "%d", &liczba_styli) < 1) return 0;
	  }
	  if (!inc_nr_linii()) return 0;
	  if (myfgets(buf, MAXLINE, f) == NULL) return 0;
	  p = buf;
	  if (sscanf(p, "%d", &entitie_code) < 1) return 0;
   }

   while (endtab == FALSE)
   {
	   if (entitie_code == 0)
	   {
		   if (!inc_nr_linii()) return 0;
		   if (myfgets(buf1, MAXLINE, f) == NULL) return 0;
		   if (strcmp(buf1, "STYLE") == 0)
		   {
			   /*czytanie az do kodu 0*/
			   if (No_Styles_DXF < MaxNumberTextStyleDXF) No_Styles_DXF++;
			   /*predefinicja stylu*/
			   strcpy(PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].font_acad_name, "");
			   PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].font_number = 0;
			   PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].flags = 0;
			   PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].fixed_height = 0;
			   PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].width_factor = 1;
			   PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].italic = 0;
			   PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].generation_flags = 0;

               ignore1000=FALSE;
			   endstyle = FALSE;
			   while (endstyle == FALSE)
			   {
				   if (!inc_nr_linii()) return 0;
				   if (myfgets(buf, MAXLINE, f) == NULL) return 0;
				   p = buf;
				   if (sscanf(p, "%d", &entitie_code) < 1) return 0;
				   if (entitie_code != 0)
				   {
					   if (!inc_nr_linii()) return 0;
					   if (myfgets(buf1, MAXLINE, f) == NULL) return 0;
				   }

				   switch (entitie_code)
				   {
				   case 0: endstyle = TRUE;
					   break;
				   case 2:
					   strcpy(PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].font_acad_name, buf1); //nazwa
					   break;
				   case 70:  //standard flag values
					   p1 = buf1;
					   if (sscanf(p1, "%d", &style_flags) < 1) return 0;
					   PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].flags = style_flags;
					   break;
				   case 40:  //fixed text height
					   p1 = buf1;
					   if (sscanf(p1, "%lf", &fixed_height) < 1) return 0;
					   PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].fixed_height = fixed_height;
					   break;
				   case 41:  //width factor;
					   p1 = buf1;
					   if (sscanf(p1, "%lf", &width_factor) < 1) return 0;
					   PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].width_factor = width_factor;
					   break;
				   case 50:  //oblique angle;
					   p1 = buf1;
					   if (sscanf(p1, "%lf", &oblique_angle) < 1) return 0;
					   if (oblique_angle != 0) PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].italic = 1;
					   break;
				   case 71:  //style generation flag values
					   p1 = buf1;
					   if (sscanf(p1, "%d", &style_generation_flags) < 1) return 0;
					   PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].flags = style_generation_flags;
					   break;
				   case 3:
                   case 1000:
                       if (strlen(buf1)==0) break;
                       if (ignore1000==TRUE) break;

					   strcpy(style_file, buf1);
                       if (entitie_code==1000)
                       {

                           if (find_font_DXF_Alfa_name((char*)style_file, (char*)&style_alfa)==1)
                               strcpy(style_file, style_alfa);
                           else {
                               strcat(style_file, ".ttf");
                               strcpy(style_file_1000, style_file);
                           }
                       }
                       else if ((strlen(style_file)==0) && (strlen(style_file_1000)>0))
                           strcpy(style_file, style_file_1000);  //the last one from flag 1000

                       strcpy(style_file_, style_file);
                       strupr(style_file_);

					   if (strlen(style_file) == 0)
					   {
						   font_number0 = find_font_DXF(PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].font_acad_name, &e_wspx1, -1);
					   }
                       else font_number0 = find_font_DXF(style_file, &e_wspx1, -1);

                       if (font_number0==-1)
					   {
						   ptr = strstr(style_file_, ".TTF");
                           ptr1 = strstr(style_file_, ".OTF");
						   if ((ptr != NULL) || (ptr1 != NULL))
						   {
							   flags = fnsplit(style_file, drive, dir, file, ext);
							   font_number0 = find_font_DXF(buf1, &e_wspx1, 2);
							   if (font_number0 == -1)  //not found but may can find in fonts, if heyes, they will be added to dxf fonts
                               {
                                   font_number0 = find_font_ALFA(style_file, &e_wspx1, 2);
                                   if (font_number0 > -1) {
                                       add_font_DXF(style_file);
                                   }
                                   else
                                   {
                                       font_number0 = Add_New_Font_TTF_OTF(file, 0, 1);
                                       if (font_number0 == -1) font_number0 = Add_New_Font_TTF_OTF(strlwr(file), 0, 1);
                                       if (font_number0 > -1) add_font_DXF(file);
                                       int a=0;  //// strange, but helps to not confuse compiler/debugger
                                   }
                               }
						   }
                            else  //all others including .shx
                            {
                                flags = fnsplit(style_file, drive, dir, file, ext);
                                font_number0 = find_font_DXF(file, &e_wspx1, 1);
                            }
				        }

                        if (font_number0>-1)
                          {
                            PTRS__Text_Style_DXF_TABLE[No_Styles_DXF-1].font_number=font_number0;
                            PTRS__Text_Style_DXF_TABLE[No_Styles_DXF-1].width_factor *= e_wspx1;
                          }
                          else
                            PTRS__Text_Style_DXF_TABLE[No_Styles_DXF - 1].font_number = 0;  //first from the list as substitute

                            if ((entitie_code==3) && (font_number0>0)) ignore1000=TRUE;
                        break;
           
                    default:
                        break;
                   }
               }
           }
      else if (strcmp (buf1, "ENDTAB") == 0)
      {
       endtab=TRUE;
      }
    }  
     else
      {
	  if (!inc_nr_linii()) return 0;
       if ( myfgets(buf1 , MAXLINE , f ) == NULL ) return 0;
      }
  // next_style:  ;
  } /*while*/    
 return 1; 
}    



BOOL read_ucs(FILE *f)
/*------------------------*/
{ char dxf_table [MAXLINE], dxf_table_code [MAXLINE] ;
  BOOL endtab;
  endtab=FALSE;
  while (endtab==FALSE)
  {
	  if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table_code , MAXLINE , f ) == NULL ) return 0;
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table , MAXLINE , f ) == NULL ) return 0;
   if (strcmp (dxf_table, "ENDTAB") == 0)
    {
     endtab=TRUE;
    }
  // next_ucs:  ;
  } /*while*/    
 return 1; 
} 

BOOL read_view(FILE *f)
/*------------------------*/
{ char dxf_table [MAXLINE], dxf_table_code [MAXLINE] ;
  BOOL endtab;
  endtab=FALSE;
  while (endtab==FALSE)
  {
	  if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table_code , MAXLINE , f ) == NULL ) return 0;
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table , MAXLINE , f ) == NULL ) return 0;
   if (strcmp (dxf_table, "ENDTAB") == 0)
    {
     endtab=TRUE;
    }
   //next_view:  ;
  } /*while*/    
 return 1; 
} 

BOOL read_vport(FILE *f)
/*------------------------*/
{ char dxf_table [MAXLINE], dxf_table_code [MAXLINE] ;
  BOOL endtab;
  endtab=FALSE;
  while (endtab==FALSE)
  {
	  if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table_code , MAXLINE , f ) == NULL ) return 0;
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table , MAXLINE , f ) == NULL ) return 0;
   if (strcmp (dxf_table, "ENDTAB") == 0)
    {
     endtab=TRUE;
    }
   //next_vport:  ;
  } /*while*/    
 return 1; 
} 

BOOL read_table_dxf(FILE *f, BOOL block)
/*------------------------------------*/
{ char dxf_table [MAXLINE], dxf_table_code [MAXLINE] ;
  char dxf_tables_table [MAXLINE];
  BOOL endtab;
  endtab=FALSE;
  while (endtab==FALSE)
  {
	  if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table_code , MAXLINE , f ) == NULL ) return 0;
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_table , MAXLINE , f ) == NULL ) return 0;
   dxf_table[32]='\0';
   if (strlen(dxf_table) == 0) return 0;

   strcpy(dxf_tables_table,"TABLE: ");
   strcat(dxf_tables_table,dxf_table);

   strcpy(style_file_1000, "");

   if (strcmp (dxf_table, "LAYER") == 0)
    {    
     if (!read_layer(f, block)) return 0;
     return 1;
    }
   if (strcmp (dxf_table, "APPID") == 0)
    {
     if (!read_appid(f)) return 0;
     return 1;
    }
   if (strcmp (dxf_table, "DIMSTYLE") == 0)
    { 
     if (!read_dimstyle(f)) return 0;
     return 1;
    }
    if (strcmp (dxf_table, "BLOCK_RECORD") == 0)
    {
      if (!read_block_record(f)) return 0;
      return 1;
    }
      if (strcmp (dxf_table, "LTYPE") == 0)
    { 
     if (!read_ltype(f)) return 0;
     return 1;
    }
   if (strcmp (dxf_table, "STYLE") == 0)
    {
     if (!read_style(f)) return 0;
     return 1;
    }  
   if (strcmp (dxf_table, "UCS") == 0)
    {
     if (!read_ucs(f)) return 0;
     return 1;
    }
   if (strcmp (dxf_table, "VIEW") == 0)
    {
     if (!read_view(f)) return 0;
     return 1;
    }  
   if (strcmp (dxf_table, "VPORT") == 0)
    { 
     if (!read_vport(f)) return 0;
     return 1;
    } 
   if (strcmp (dxf_table, "ENDTAB") == 0)
    {
     endtab=TRUE;
    }
  } /*while*/    
 return 1; 
}  


BOOL read_blocks_dxf(FILE *f, BOOL block, int block_view, int draw_hatch)
/*-----------------------------------------------------*/
{ char dxf_blocks [MAXLINE], dxf_blocks_code [MAXLINE] ;
  BOOL endsec;
  int ret;
  endsec=FALSE;

  ep_space=0;
  ep_width=line_width.normal*100;
  while (endsec==FALSE)
  {
   ret=1;  /*1=O.K.*/
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_blocks_code , MAXLINE , f ) == NULL ) return 0;
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_blocks , MAXLINE , f ) == NULL ) return 0;
   if (strcmp (dxf_blocks, "BLOCK") == 0)
    { 
     ret=read_block_dxf(f, block, block_view, draw_hatch);
     goto next_blocks;
    }
   if (strcmp (dxf_blocks, "ENDSEC") == 0)
    {
     endsec=TRUE;
    }
 next_blocks: 
   if (ret==0) return 0;
  } /*while*/    
 return 1; 
}

BOOL read_tables_dxf(FILE *f, BOOL block)
/*------------------------------------*/
{ char dxf_tables [MAXLINE], dxf_tables_code [MAXLINE] ;
  BOOL endsec;
  int ret;
  endsec=FALSE;

  block_record_no=0;
  block_record_no_max=100;
  block_record=(BLOCK_RECORD*)malloc(block_record_no_max*sizeof(BLOCK_RECORD));

  while (endsec==FALSE)
  {
   ret=1;  /*1=O.K.*/
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_tables_code , MAXLINE , f ) == NULL ) return 0;
   if (!inc_nr_linii()) return 0;
   if ( myfgets(dxf_tables , MAXLINE , f ) == NULL ) return 0;
   if (strcmp (dxf_tables, "TABLE") == 0)
    { 
     ret=read_table_dxf(f, block);
     goto next_tables;
    }
   if (strcmp (dxf_tables, "ENDSEC") == 0)
    {
     endsec=TRUE;
    }
 next_tables:
   if (ret==0) return 0;
  } /*while*/    
 return 1; 
}  
  

/******************************/
BOOL read_header_dxf(FILE *f)
/*------------------------*/
{ char dxf_var [MAXLINE], dxf_var_code [MAXLINE] ;
  BOOL endsec;
  int ret;

 endsec=FALSE;
 while (endsec==FALSE)
 {
  ret=1;  /*1=O.K.*/
  if (!inc_nr_linii()) return 0;
  if ( myfgets(dxf_var_code , MAXLINE , f ) == NULL ) return 0;
  if (!inc_nr_linii()) return 0;
  if ( myfgets(dxf_var , MAXLINE , f ) == NULL ) return 0;
  if (dxf_var[0]=='$')  /*to jest zmienna*/
   {
    if (strcmp (dxf_var, "$ACADVER") == 0)
     { ret=read_var1(f,1,&ACADVER);
	   if (!glb_silent) komunikat0_str(23,ACADVER);
      if (strcmp(ACADVER, "AC1.2") == 0) acad_ver = 1;     /*R1.2*/
      else if (strcmp(ACADVER, "AC1.4") == 0) acad_ver = 1;     /*R1.4*/
      else if (strcmp(ACADVER, "AC1.50") == 0) acad_ver = 2;     /*R2.0*/
      else if (strcmp(ACADVER, "AC2.10") == 0) acad_ver = 2;     /*R2.10*/
	   else if (strcmp(ACADVER, "AC1002") == 0) acad_ver = 2;     /*R2.5*/
		else if (strcmp(ACADVER,"AC1003")==0) acad_ver=3;     /*R2.6*/
		 else if (strcmp(ACADVER,"AC1004") == 0) acad_ver = 9;  /*R9*/
		  else if (strcmp(ACADVER,"AC1006")==0) acad_ver=10;     /*R10*/
		   else if (strcmp(ACADVER,"AC1009")==0) acad_ver=12;     /*R11, R12, LT R1/R2*/
			else if (strcmp(ACADVER, "AC1010")==0) acad_ver = 12;     /*R12*/
			 else if (strcmp(ACADVER,"AC1011")==0) acad_ver=12;     /*R13*/
			  else if (strcmp(ACADVER,"AC1012")==0) acad_ver=13;     /*R11, R12*/
			   else if (strcmp(ACADVER,"AC1013")==0) acad_ver=13;     /*R11, R12*/
				else if (strcmp(ACADVER,"AC1014")==0) acad_ver=14;     /*R14*/
				 else if (strcmp(ACADVER,"AC1015")==0) acad_ver=2000;     /*R2000, 2000i, 2002*/
				  else if (strcmp(ACADVER, "AC1016")==0) acad_ver = 20001;     /*R20002*/
				   else if (strcmp(ACADVER, "AC1017")==0) acad_ver = 2002;     /*R20002*/
					else if (strcmp(ACADVER, "AC1018")==0) acad_ver=2004;     /*R2004, R2005, R2006*/
					 else if (strcmp(ACADVER, "AC1019") == 0) acad_ver = 2005;     /*R2005*/
					  else if (strcmp(ACADVER, "AC1020") == 0) acad_ver = 2006;     /*R2006*/
					   else if (strcmp(ACADVER, "AC1021") == 0) acad_ver = 2007;     /*R2007, R2008, R2009*/
						else if (strcmp(ACADVER, "AC1022") == 0) acad_ver = 2008;     /*R2008*/
						 else if (strcmp(ACADVER, "AC1023") == 0) acad_ver = 2009;     /*R2009*/
						  else if (strcmp(ACADVER, "AC1024") == 0) acad_ver = 2010;     /*R2010, R2011, R2012*/
						   else if (strcmp(ACADVER, "AC1025") == 0) acad_ver = 2011;     /*R2011*/
							else if (strcmp(ACADVER, "AC1026") == 0) acad_ver = 2012;     /*R2012*/
							 else if (strcmp(ACADVER, "AC1027") == 0) acad_ver = 2013;     /*R2013, R2014, R2015, R2016, R2017*/
							  else if (strcmp(ACADVER, "AC1028") == 0) acad_ver = 2014;     /*R2014*/
							   else if (strcmp(ACADVER, "AC1029") == 0) acad_ver = 2015;     /*R2015*/
								else if (strcmp(ACADVER, "AC1030") == 0) acad_ver = 2016;     /*R2016*/
								 else if (strcmp(ACADVER, "AC1031") == 0) acad_ver = 2017;     /*R2017*/
								  else if (strcmp(ACADVER, "AC1032") == 0) acad_ver = 2018;     /*R2018, R2019, R2020, R2021, R2022, R2023*/
								   else if (strcmp(ACADVER, "AC1033") == 0) acad_ver = 2019;     /*R2019*/
									else if (strcmp(ACADVER, "AC1034") == 0) acad_ver = 2020;    /*R2020*/
                                     else if (strcmp(ACADVER, "AC1035") == 0) acad_ver = 2021;    /*R2021*/
                                      else if (strcmp(ACADVER, "AC1036") == 0) acad_ver = 2022;    /*R2022*/
                                       else if (strcmp(ACADVER, "AC1037") == 0) acad_ver = 2023;    /*R2023*/
				   else acad_ver=14;  //na wszelki wypadek
      goto next_var;
     }
    if (strcmp (dxf_var, "$INSBASE") == 0)
     { ret=read_var3(f,10,&INSBASE_X,20,&INSBASE_Y,30,&INSBASE_Y); //0,0,0
     goto next_var;
     } 
    if (strcmp (dxf_var, "$EXTMIN") == 0)
    { ret=read_var3(f,10,&EXTMIN_X0,20,&EXTMIN_Y0,30,&EXTMIN_Z0);

	if (USERI1==211)  //TO POCHODZI OD ALFA
	 {
		EXTMIN_X0=0.0;
		EXTMIN_Y0=0.0;
		EXTMIN_Z0=0.0;
	 }
    else if ((EXTMIN_X0>1E+10) || (EXTMIN_Y0>1E+10))
     {
		EXTMIN_X0=0.0;
		EXTMIN_Y0=0.0;
		EXTMIN_Z0=0.0;
	 }
    else
    {
         EXTMIN_X=EXTMIN_X0;
         EXTMIN_Y=EXTMIN_Y0;
         EXTMIN_Z=EXTMIN_Z0;
     }
     goto next_var;
    }
    if (strcmp (dxf_var, "$EXTMAX") == 0)
    { ret=read_var3(f,10,&EXTMAX_X0,20,&EXTMAX_Y0,30,&EXTMAX_Z0); 
	EXTMAX_X = EXTMAX_X0;
	EXTMAX_Y = EXTMAX_Y0;
	EXTMAX_Z = EXTMAX_Z0;

    goto next_var;
    }
    if (strcmp (dxf_var, "$LIMMIN") == 0)
    { ret=read_var2(f,10,&LIMMIN_X,20,&LIMMIN_Y);
	    if (LIMMIN_X > EXTMIN_X) EXTMIN_X = LIMMIN_X;
	    if (LIMMIN_Y > EXTMIN_Y) EXTMIN_Y = LIMMIN_Y;
    goto next_var;
    }
    if (strcmp (dxf_var, "$LIMMAX") == 0)
    {ret=read_var2(f,10,&LIMMAX_X,20,&LIMMAX_Y);
	    if (LIMMAX_X < EXTMAX_X) EXTMAX_X = LIMMAX_X;
	    if (LIMMAX_Y < EXTMAX_Y) EXTMAX_Y = LIMMAX_Y;
    goto next_var;
    }
   if (strcmp (dxf_var, "$ORTHOMODE") == 0)
    {ret=read_var1(f,70,&ORTHOMODE); //0
     goto next_var;
    } 
   if (strcmp (dxf_var, "$REGENMODE") == 0)
    {ret=read_var1(f,70,&REGENMODE); //1
     goto next_var;
    } 
    if (strcmp (dxf_var, "$FILLMODE") == 0)
    {ret=read_var1(f,70,&FILLMODE); //1
     goto next_var;
    } 
    if (strcmp (dxf_var, "$QTEXTMODE") == 0)
    {ret=read_var1(f,70,&QTEXTMODE); //0
    } 
    if (strcmp (dxf_var, "$MIRRTEXT") == 0)
    {ret=read_var1(f,70,&MIRRTEXT); //1
     goto next_var;
    } 
    if (strcmp (dxf_var, "$DRAGMODE") == 0)
    {ret=read_var1(f,70,&DRAGMODE); //2
     goto next_var;
    } 
    if (strcmp (dxf_var, "$LTSCALE") == 0)
    {ret=read_var1(f,40,&LTSCALE); //1.0
     goto next_var;
    }
	if (strcmp(dxf_var, "$INSUNITS") == 0)
	{
		ret = read_var1(f, 70, &INSUNITS); //4
		goto next_var;
	}
    if (strcmp (dxf_var, "$OSMODE") == 0)
    {ret=read_var1(f,70,&OSMODE); //0
     goto next_var;
    } 
    if (strcmp (dxf_var, "$ATTMODE") == 0)
    {ret=read_var1(f,70,&ATTMODE); //1
     goto next_var;
    }  
    if (strcmp (dxf_var, "$TEXTSIZE") == 0)
    {ret=read_var1(f,40,&TEXTSIZE); //0.2
    goto next_var;
    }
    if (strcmp (dxf_var, "$TRACEWID") == 0)
    {ret=read_var1(f,40,&TRACEWID); //0.05
    goto next_var;
    }
   if (strcmp (dxf_var, "$TEXTSTYLE") == 0)
    {ret=read_var1(f,7,&TEXTSTYLE); //STANDARD
     goto next_var;
    }
   if (strcmp (dxf_var, "$CLAYER") == 0)
    {ret=read_var1(f,8,&CLAYER); //0
    goto next_var;
    }
  if (strcmp (dxf_var, "$CELTYPE") == 0)
    {ret=read_var1(f,6,&CELTYPE); //BYLAYER
    goto next_var;
    }
  if (strcmp (dxf_var, "$CECOLOR") == 0)
    {ret=read_var1(f,62,&CECOLOR); //256
    goto next_var;
    }   
  if (strcmp (dxf_var, "$DIMSCALE") == 0)
    {ret=read_var1(f,40,&DIMSCALE); //1.0
    goto next_var;
    }
  if (strcmp(dxf_var, "$DIMTXSTY") == 0)
  {
	  ret = read_var1(f, 7, &DIMTXSTY); //"STANDARD"
	  goto next_var;
  }
  if (strcmp (dxf_var, "$DIMASZ") == 0)
    {ret=read_var1(f,40,&DIMASZ); //0.18
    goto next_var;
    }
  if (strcmp (dxf_var, "$DIMEXO") == 0)
    {ret=read_var1(f,40,&DIMEXO); //0.0625
    goto next_var;
    }
  if (strcmp (dxf_var, "$DIMDLI") == 0)
    {ret=read_var1(f,40,&DIMDLI); //0.38
    goto next_var;
    }
  if (strcmp (dxf_var, "$DIMRND") == 0)
    {ret=read_var1(f,40,&DIMRND); //0.01
    goto next_var;
    }
  if (strcmp (dxf_var, "$DIMDLE") == 0)
    {ret=read_var1(f,40,&DIMDLE); //0.0
    goto next_var;
    }
  if (strcmp (dxf_var, "$DIMEXE") == 0)
    {ret=read_var1(f,40,&DIMEXE); //0.18
    goto next_var;
   }
  if (strcmp (dxf_var, "$DIMTP") == 0)
    {ret=read_var1(f,40,&DIMTP); //0.0
    goto next_var;
   }
  if (strcmp (dxf_var, "$DIMTM") == 0)
    {ret=read_var1(f,40,&DIMTM); //0.0
    goto next_var;
   }
 if (strcmp (dxf_var, "$DIMTXT") == 0)
    {ret=read_var1(f,40,&DIMTXT); //0.18
    goto next_var;
   }
 if (strcmp (dxf_var, "$DIMCEN") == 0)
    {ret=read_var1(f,40,&DIMCEN); //0.09
    goto next_var;
    }
 if (strcmp (dxf_var, "$DIMTSZ") == 0)
    {ret=read_var1(f,40,&DIMTSZ); //0.0
    goto next_var;
    }
 if (strcmp (dxf_var, "$DIMTOL") == 0)
    {ret=read_var1(f,70,&DIMTOL); //0
    goto next_var;     
    } 
 if (strcmp (dxf_var, "$DIMLIM") == 0)
    {ret=read_var1(f,70,&DIMLIM); //0
    goto next_var;     
   }  
 if (strcmp (dxf_var, "$DIMTIH") == 0)
    {ret=read_var1(f,70,&DIMTIH); //1
    goto next_var;     
    } 
 if (strcmp (dxf_var, "$DIMTOH") == 0)
    {ret=read_var1(f,70,&DIMTOH); //1
    goto next_var;     
    } 
 if (strcmp (dxf_var, "$DIMSE1") == 0)
    {ret=read_var1(f,70,&DIMSE1); //0
    goto next_var;     
    } 
 if (strcmp (dxf_var, "$DIMSE2") == 0)
    {ret=read_var1(f,70,&DIMSE2); //0
    goto next_var;     
    } 
 if (strcmp (dxf_var, "$DIMTAD") == 0)
    {ret=read_var1(f,70,&DIMTAD); //0
    goto next_var;     
    } 
 if (strcmp (dxf_var, "$DIMZIN") == 0)
    {ret=read_var1(f,70,&DIMZIN); //0
    goto next_var;     
    }  
 if (strcmp (dxf_var, "$DIMBLK") == 0)
    {ret=read_var1(f,1,&DIMBLK); //""
    goto next_var;
    }
 if (strcmp (dxf_var, "$DIMASO") == 0)
    {ret=read_var1(f,70,&DIMASO); //1    
    goto next_var;     
    } 
 if (strcmp (dxf_var, "$DIMSHO") == 0)
    {ret=read_var1(f,70,&DIMSHO); //1
    goto next_var;     
    } 
 if (strcmp (dxf_var, "$DIMPOST") == 0)
    {ret=read_var1(f,1,&DIMPOST); //""
    goto next_var;
    }
 if (strcmp (dxf_var, "$DIMAPOST") == 0)
    {ret=read_var1(f,1,&DIMAPOST); //""
    goto next_var;
   }
 if (strcmp (dxf_var, "$DIMALT") == 0)
    {ret=read_var1(f,70,&DIMALT); //0
    goto next_var;     
    } 
 if (strcmp (dxf_var, "$DIMALTD") == 0)
    {ret=read_var1(f,70,&DIMALTD); //2
    goto next_var;     
    } 
 if (strcmp (dxf_var, "$DIMALTF") == 0)
    {ret=read_var1(f,40,&DIMALTF); //25.4
    goto next_var;
    }
 if (strcmp (dxf_var, "$DIMLFAC") == 0)
    {ret=read_var1(f,40,&DIMLFAC); //1.0
    goto next_var;
   }
 if (strcmp (dxf_var, "$DIMTOFL") == 0)
    {ret=read_var1(f,70,&DIMTOFL); //0
    goto next_var;     
    } 
 if (strcmp (dxf_var, "$DIMTVP") == 0)
    {ret=read_var1(f,40,&DIMTVP); //0.0
    goto next_var;
    }
 if (strcmp (dxf_var, "$DIMTIX") == 0)
    {ret=read_var1(f,70,&DIMTIX); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$DIMSOXD") == 0)
    {ret=read_var1(f,70,&DIMSOXD); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$DIMSAH") == 0)
    {ret=read_var1(f,70,&DIMSAH); //0
    goto next_var;     
    } 
 if (strcmp (dxf_var, "$DIMBLK1") == 0)
    {ret=read_var1(f,1,&DIMBLK1); //""
    goto next_var;
   }
 if (strcmp (dxf_var, "$DIMBLK2") == 0)
    {ret=read_var1(f,1,&DIMBLK2); //""
    goto next_var;
   }
 if (strcmp (dxf_var, "$DIMSTYLE") == 0)
    {ret=read_var1(f,2,&DIMSTYLE); //*UNNAMED
    goto next_var;
    }
 if (strcmp (dxf_var, "$DIMCLRD") == 0)
    {ret=read_var1(f,70,&DIMCLRD); //0
    goto next_var;     
    }  
 if (strcmp (dxf_var, "$DIMCLRE") == 0)
    {ret=read_var1(f,70,&DIMCLRE); //0
    goto next_var;     
    }  
 if (strcmp (dxf_var, "$DIMCLRT") == 0)
    {ret=read_var1(f,70,&DIMCLRT); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$DIMTFAC") == 0)
    {ret=read_var1(f,40,&DIMTFAC); //1.0
    goto next_var;
   }
if (strcmp (dxf_var, "$DIMGAP") == 0)
    {ret=read_var1(f,40,&DIMGAP); //0.09
    goto next_var;
    }
if (strcmp (dxf_var, "$LUNITS") == 0)
    {ret=read_var1(f,70,&LUNITS); //2
    goto next_var;     
    } 
if (strcmp (dxf_var, "$LUPREC") == 0)
    {ret=read_var1(f,70,&LUPREC); //4
    goto next_var;     
    }  
if (strcmp (dxf_var, "$SKETCHINC") == 0)
    {ret=read_var1(f,40,&SKETCHINC); //0.1
    goto next_var;
    }
if (strcmp (dxf_var, "$FILLETRAD") == 0)
    {ret=read_var1(f,40,&FILLETRAD); //0.0
    goto next_var;
    }
if (strcmp (dxf_var, "$AUNITS") == 0)
    {ret=read_var1(f,70,&AUNITS); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$AUPREC") == 0)
    {ret=read_var1(f,70,&AUPREC); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$MENU") == 0)
    {ret=read_var1(f,1,&MYMENU); //acad
    goto next_var;
   }
if (strcmp (dxf_var, "$ELEVATION") == 0)
    {ret=read_var1(f,40,&ELEVATION); //0.0
    goto next_var;
    }
if (strcmp (dxf_var, "$PELEVATION") == 0)
    {ret=read_var1(f,40,&PELEVATION); //0.0
    goto next_var;
   }
if (strcmp (dxf_var, "$THICKNESS") == 0)
    {ret=read_var1(f,40,&THICKNESS); //0.0
    goto next_var;
  }
if (strcmp (dxf_var, "$LIMCHECK") == 0)
    {ret=read_var1(f,70,&LIMCHECK); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$BLIPMODE") == 0)
    {ret=read_var1(f,70,&BLIPMODE); //1
    goto next_var;     
    } 
if (strcmp (dxf_var, "$CHAMFERA") == 0)
    {ret=read_var1(f,40,&CHAMFERA); //0.0
    goto next_var;
   }
if (strcmp (dxf_var, "$CHAMFERB") == 0)
    {ret=read_var1(f,40,&CHAMFERB); //0.0
    goto next_var;
   }
if (strcmp (dxf_var, "$SKPOLY") == 0)
    {ret=read_var1(f,70,&SKPOLY); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$TDCREATE") == 0)
    {ret=read_var1(f,40,&TDCREATE); //2450901.381882175
    goto next_var;
    }
if (strcmp (dxf_var, "$TDUPDATE") == 0)
    {ret=read_var1(f,40,&TDUPDATE); //2450901.381882175
    goto next_var;
   }
if (strcmp (dxf_var, "$TDINDWG") == 0)
   {ret=read_var1(f,40,&TDINDWG); //0.0000000
    goto next_var;
   }
if (strcmp (dxf_var, "$TDUSRTIMER") == 0)
    {ret=read_var1(f,40,&TDUSRTIMER); //0.000000
    goto next_var;
    }
if (strcmp (dxf_var, "$USRTIMER") == 0)
    {ret=read_var1(f,70,&USRTIMER); //1
    goto next_var;     
    } 
if (strcmp (dxf_var, "$ANGBASE") == 0)
    {ret=read_var1(f,50,&ANGBASE); //0.0
    goto next_var;
    }
if (strcmp (dxf_var, "$ANGDIR") == 0)
    {ret=read_var1(f,70,&ANGDIR); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$PDMODE") == 0)
    {ret=read_var1(f,70,&PDMODE); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$PDSIZE") == 0)
    {ret=read_var1(f,40,&PDSIZE); //0
    goto next_var;
   }
if (strcmp (dxf_var, "$PLINEWID") == 0)
    {ret=read_var1(f,40,&PLINEWID); //0.0
    goto next_var;
   }
if (strcmp (dxf_var, "$COORDS") == 0)
    {ret=read_var1(f,70,&COORDS); //1
    goto next_var;     
    } 
if (strcmp (dxf_var, "$SPLFRAME") == 0)
    {ret=read_var1(f,70,&SPLFRAME); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$SPLINETYPE") == 0)
    {ret=read_var1(f,70,&SPLINETYPE); //6
    goto next_var;     
    } 
if (strcmp (dxf_var, "$SPLINESEGS") == 0)
    {ret=read_var1(f,70,&SPLINESEGS); //8
    goto next_var;     
    } 
if (strcmp (dxf_var, "$ATTDIA") == 0)
    {ret=read_var1(f,70,&ATTDIA); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$ATTREQ") == 0)
    {ret=read_var1(f,70,&ATTREQ); //1
    goto next_var;     
    } 
if (strcmp (dxf_var, "$HANDLING") == 0)
    {ret=read_var1(f,70,&HANDLING); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$HANDSEED") == 0)
    {ret=read_var1(f,5,&HANDSEED); //0
    goto next_var;
   }
if (strcmp (dxf_var, "$SURFTAB1") == 0)
    {ret=read_var1(f,70,&SURFTAB1); //6
    goto next_var;     
    } 
if (strcmp (dxf_var, "$SURFTAB2") == 0)
    {ret=read_var1(f,70,&SURFTAB2); //6
    goto next_var;     
    } 
if (strcmp (dxf_var, "$SURFTYPE") == 0)
    {ret=read_var1(f,70,&SURFTYPE); //6
    goto next_var;     
    } 
if (strcmp (dxf_var, "$SURFU") == 0)
    {ret=read_var1(f,70,&SURFU); //6
    goto next_var;     
    } 
if (strcmp (dxf_var, "$SURFV") == 0)
    {ret=read_var1(f,70,&SURFV); //6
    goto next_var;     
    } 
if (strcmp (dxf_var, "$UCSNAME") == 0)
    {ret=read_var1(f,2,&UCSNAME); //""
    goto next_var;
   }
if (strcmp (dxf_var, "$UCSORG") == 0)
    {ret=read_var3(f,10,&UCSORG_X,20,&UCSORG_Y,30,&UCSORG_Z); //0,0,0
    goto next_var;
    }
if (strcmp (dxf_var, "$UCSXDIR") == 0)
    {ret=read_var3(f,10,&UCSXDIR_X,20,&UCSXDIR_Y,30,&UCSXDIR_Z); //1,0,0
    goto next_var;
   }
if (strcmp (dxf_var, "$UCSYDIR") == 0)
    {ret=read_var3(f,10,&UCSYDIR_X,20,&UCSYDIR_Y,30,&UCSYDIR_Z); //0,1,0
    goto next_var;
   }
if (strcmp (dxf_var, "$PUCSNAME") == 0)
    {ret=read_var1(f,2,&PUCSNAME); //""
    goto next_var;
   }
if (strcmp (dxf_var, "$PUCSORG") == 0)
    {ret=read_var3(f,10,&PUCSORG_X,20,&PUCSORG_Y,30,&PUCSORG_Z); //0,0,0
    goto next_var;
    }
if (strcmp (dxf_var, "$PUCSXDIR") == 0)
    {ret=read_var3(f,10,&PUCSXDIR_X,20,&PUCSXDIR_Y,30,&PUCSXDIR_Z); //1,0,0    
    goto next_var;
    }
if (strcmp (dxf_var, "$PUCSYDIR") == 0)
    {ret=read_var3(f,10,&PUCSYDIR_X,20,&PUCSYDIR_Y,30,&PUCSYDIR_Z); //0,1,0    
    goto next_var;
   }
if (strcmp (dxf_var, "$USERI1") == 0)
    {ret=read_var1(f,70,&USERI1); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$USERI2") == 0)
    {ret=read_var1(f,70,&USERI2); //0    
    goto next_var;     
    } 
if (strcmp (dxf_var, "$USERI3") == 0)
    {ret=read_var1(f,70,&USERI3); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$USERI4") == 0)
    {ret=read_var1(f,70,&USERI4); //0
    goto next_var;     
    }
if (strcmp (dxf_var, "$USERI5") == 0)
    {ret=read_var1(f,70,&USERI5); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$USERR1") == 0)
    {ret=read_var1(f,40,&USERR1); //0.0
    goto next_var;
   }
if (strcmp (dxf_var, "$USERR2") == 0)
    {ret=read_var1(f,40,&USERR2); //0.0
    goto next_var;
   }
if (strcmp (dxf_var, "$USERR3") == 0)
    {ret=read_var1(f,70,&USERR3); //0.0
    goto next_var;
   }
if (strcmp (dxf_var, "$USERR4") == 0)
    {ret=read_var1(f,70,&USERR4); //0.0
    goto next_var;
   }
if (strcmp (dxf_var, "$USERR5") == 0)
    {ret=read_var1(f,70,&USERR5); //0.0
    goto next_var;
    }
if (strcmp (dxf_var, "$WORLDVIEW") == 0)
    {ret=read_var1(f,70,&WORLDVIEW); //1
    goto next_var;     
    } 
if (strcmp (dxf_var, "$SHADEDGE") == 0)
    {ret=read_var1(f,70,&SHADEDGE); //3
    goto next_var;     
    }  
if (strcmp (dxf_var, "$SHADEDIF") == 0)
    {ret=read_var1(f,70,&SHADEDIF); //70
    goto next_var;    
    }
if (strcmp (dxf_var, "$TILEMODE") == 0)
    {ret=read_var1(f,70,&TILEMODE); //1
    goto next_var;     
    } 
if (strcmp (dxf_var, "$MAXACTVP") == 0)
    {ret=read_var1(f,70,&MAXACTVP); //16
    goto next_var;    
    }
if (strcmp (dxf_var, "$PINSBASE") == 0)
    {ret=read_var3(f,10,&PINSBASE_X,20,&PINSBASE_Y,30,&PINSBASE_Z); //0,0,0
    goto next_var;
   }
if (strcmp (dxf_var, "$PLIMCHECK") == 0)
    {ret=read_var1(f,70,&PLIMCHECK); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$PEXTMIN") == 0)
    {ret=read_var3(f,10,&PEXTMIN_X,20,&PEXTMIN_Y,30,&PEXTMIN_Z); //1E+20,1E+20,1E+20

    goto next_var;
  }
if (strcmp (dxf_var, "$PEXTMAX") == 0)
    {ret=read_var3(f,10,&PEXTMAX_X,20,&PEXTMAX_Y,30,&PEXTMAX_Z); //-1E+20,-E+20,-1E+20

    goto next_var;
  }
if (strcmp (dxf_var, "$PLIMMIN") == 0)
    {ret=read_var2(f,10,&PLIMMIN_X,20,&PLIMMIN_Y); //0.0,0.0
    goto next_var;
   }
if (strcmp (dxf_var, "$PLIMMAX") == 0)
    {ret=read_var2(f,10,&PLIMMAX_X,20,&PLIMMAX_Y); //12.0,9.0
    goto next_var;
   }
if (strcmp (dxf_var, "$UNITMODE") == 0)
    {ret=read_var1(f,70,&UNITMODE); //0   
    goto next_var;     
    } 
if (strcmp (dxf_var, "$VISRETAIN") == 0)
    {ret=read_var1(f,70,&VISRETAIN); //0
    goto next_var;     
    } 
if (strcmp (dxf_var, "$PLINEGEN") == 0)
    {ret=read_var1(f,70,&PLINEGEN); //0
    goto next_var;     
    }
if (strcmp (dxf_var, "$PSLTSCALE") == 0)
    {ret=read_var1(f,70,&PSLTSCALE); //1
    goto next_var;     
    } 
if (strcmp (dxf_var, "$TREEDEPTH") == 0)
    {ret=read_var1(f,70,&TREEDEPTH); //3020
    goto next_var;  
    }
if (strcmp (dxf_var, "$DWGCODEPAGE") == 0)
    {ret=read_var1(f,3,&DWGCODEPAGE); //"ascii"
    goto next_var;
    }
  }
  else 
   {
   if (strcmp (dxf_var, "ENDSEC") == 0)
    {
     endsec=TRUE;
    }
   } 
    
next_var:
if (ret==0) return 0;
 } /*while*/    
return 1; 
}

static void chooselayer(int nr_warstwy)
{ int w_color, w_typ;

  if ((nr_warstwy >= 0) && (nr_warstwy<256)) Current_Layer=nr_warstwy;
   else Current_Layer=0;
  w_color=Layers[Current_Layer].color;
  w_typ=Layers[Current_Layer].line_type;
  
  OkragG.kolor=PointG.kolor=TextG.kolor=LukG.kolor = LiniaG.kolor= w_color ; 
  LiniaG.typ=OkragG.typ=LukG.typ=w_typ ;
  OkragG.warstwa=PointG.warstwa=TextG.warstwa=LukG.warstwa = LiniaG.warstwa= nr_warstwy ;
}

extern "C" { 
	extern int czytaj_dxf_header(char *fn, DXF_Header *dxf_header);
	extern int czytaj_dxf_file(char *fn, double Jednostki_dxf, double SkalaF_dxf, double offset_dxf_x, double offset_dxf_y, BOOL auto_size, int latin2_dxf); 
}

void line_widths_dxf(void)
{
  Get_Private_Profile_Strings((T_Fstring)Linie_DXF, get_lines_width_dxf);
}


int czytaj_dxf_header(char *fn, DXF_Header *dxf_header)
/*-----------------------------------------------------------------------------------------------------*/
//searching for Comment: 999 "", Acad version: 1  "AC1021", $EXTMIN 10, 20, $EXTMAX 10, 20,  $LIMMIN 10, 20, $LIMMAX 10, 20
//AC1002 = R2.5
//AC1003 = R2.6
//AC1004 = R9
//AC1006 = R10
//AC1009 = R11/R12
//AC1012 = R13
//AC1014 = R14
//AC1015 = AutoCAD 2000/2002
//AC1018 = AutoCAD 2004/2005/2006
//AC1021 = AutoCAD 2007/2008/2009
//AC1024 = AutoCAD 2010/2011/2012
//AC1027 = AutoCAD 2013/2014/2015/2016/2017
//AC1032 = AutoCAD 2018/2019/2020

//$EXTMIN drawing extents lower left corner, $EXTMAX drawing extents upper right corner
//$LIMMIN drawing limits lower left corner, $LIMMAX drawing limits upper right corner
//$PEXTMIN paper space lower left corner, $PEXTMAX paper space upper right corner
//$PLIMMIN paper space lower left corner, $PLIMMAX paper space upper right corner

{
	FILE *f;
	char dxf_section[MAXLINE], dxf_section_code[MAXLINE];
	BOOL endoffile;
	BOOL entities_ok;

	Get_Private_Profile_Strings((T_Fstring)Linie_DXF, get_lines_width_dxf);

	entities_ok = FALSE;


	f = fopen(fn, "rt");
	if (f == NULL)
	{   /*komunikat*/
		return -1;
	}

	nr_linii = 0;
	show_nr_linii();

	endoffile = FALSE;
	while (endoffile == FALSE)
	{
		if (!inc_nr_linii()) return 0;
		if (myfgets(dxf_section_code, MAXLINE, f) == NULL) return nr_linii;
		if (!inc_nr_linii()) return 0;
		if (myfgets(dxf_section, MAXLINE, f) == NULL)
		{
			if (entities_ok == FALSE) return nr_linii;
			else
			{
				endoffile = TRUE;
				goto next_section;
			}
		}
		if (strcmp(dxf_section, "SECTION") == 0)
		{
			goto next_section;
		}
		dxf_section[32] /*[80]*/ = '\0';
		if (!glb_silent) komunikat0_str(23, dxf_section);/*delay(50);*/

		if (strcmp(dxf_section, "HEADER") == 0)
		{
			if (read_header_dxf(f))
			{  
				if ((EXTMAX_X0 < EXTMIN_X0) || (EXTMAX_Y0 < EXTMIN_Y0))
				{
					EXTMIN_X0 = 0.0;
					EXTMIN_Y0 = 0.0;
					EXTMAX_X0 = 1189;
					EXTMAX_Y0 = 841;

					EXTMIN_X = EXTMIN_X0;
					EXTMIN_Y = EXTMIN_Y0;
					EXTMAX_X = EXTMAX_X0;
					EXTMAX_Y = EXTMAX_Y0;
				}


				strcpy(dxf_header->Comment, ACCOMMENT);
				dxf_header->EXTMIN_X = EXTMIN_X0;
				dxf_header->EXTMIN_Y = EXTMIN_Y0;
				dxf_header->EXTMAX_X = EXTMAX_X0;
				dxf_header->EXTMAX_Y = EXTMAX_Y0;

				dxf_header->LIMMIN_X = LIMMIN_X;
				dxf_header->LIMMIN_Y = LIMMIN_Y;
				dxf_header->LIMMAX_X = LIMMAX_X;
				dxf_header->LIMMAX_Y = LIMMAX_Y;

				dxf_header->PEXTMIN_X = PEXTMIN_X;
				dxf_header->PEXTMIN_X = PEXTMIN_Y;
				dxf_header->PLIMMAX_X = PLIMMAX_X;
				dxf_header->PLIMMAX_Y = PLIMMAX_Y;

				fclose(f);
				return 1;
			}
			else
			{
				fclose(f);
				return 0;
			}
		}

		if (strcmp(dxf_section, "EOF") == 0)
		{
			endoffile = TRUE;
			fclose(f);
			return 0;
			//goto next_section;
		}
	next_section:;
	}

	fclose(f);
	return 0;
}




int czytaj_dxf_file(char *fn, double Jednostki_dxf, double SkalaF_dxf, double offset_dxf_x, double offset_dxf_y, BOOL auto_size, int latin2_dxf)
/*--------------------------------------------------------------------------------------------------------------------------------------------*/
{ FILE *f;
  char dxf_section [MAXLINE], dxf_section_code [MAXLINE] ;
  BOOL endoffile;
  BOOL entities_ok;
  BOOL objects_ok;
  int cur_layer;
  char  *adr,  *adrk;
  double x_space, y_space;
  double x_space_factor, y_space_factor;
  double e_wspx1 = 1.0;
  int zmwym_czcionka = 0;


  last_mtext_h=zmwym.wysokosc;

 Jednostki=Jednostki_dxf;
 SkalaF=SkalaF_dxf;
 set_st_jedn();

 pcx_number = 0;

 Get_Private_Profile_Strings ((T_Fstring)Linie_DXF, get_lines_width_dxf);

 entities_ok = FALSE;
 objects_ok = FALSE;

 leaders_no=0;
 mleaders_no=0;
  
 
 f = fopen ( fn , "rt" ) ;
    if ( f == NULL )
    {   /*komunikat*/
   	return -1;
    }
 
 nr_linii=0;
 show_nr_linii();
 
 endoffile=FALSE;
 while (endoffile==FALSE)
 {    
	 if (!inc_nr_linii()) return 0;
  if ( myfgets(dxf_section_code , MAXLINE , f ) == NULL ) return nr_linii;
  if (!inc_nr_linii()) return 0;
  if ( myfgets(dxf_section , MAXLINE , f ) == NULL )
    {
     if (entities_ok==FALSE) return nr_linii;
       else 
         {
           endoffile=TRUE;
           goto next_section;
         }  
    }
  if (strcmp (dxf_section, "SECTION") == 0)
     { 
       goto next_section;
     }
  dxf_section[32] ='\0';
   
  if (strcmp (dxf_section, "HEADER") == 0)
     {
       if (!glb_silent) komunikat0_str(23,dxf_section);/*delay(50);*/
	   if (read_header_dxf(f))
	   {
		   if ((EXTMAX_X0 < EXTMIN_X0) || (EXTMAX_Y0 < EXTMIN_Y0))
		   {
			   EXTMIN_X0 = 0.0;
			   EXTMIN_Y0 = 0.0;
			   EXTMAX_X0 = 1189;
			   EXTMAX_Y0 = 841;

			   EXTMIN_X = EXTMIN_X0;
			   EXTMIN_Y = EXTMIN_Y0;
			   EXTMAX_X = EXTMAX_X0;
			   EXTMAX_Y = EXTMAX_Y0;
		   }
	   }
	   else return nr_linii;
       goto next_section;
     }
       
  else if (strcmp (dxf_section, "TABLES") == 0)
     {
         if (!glb_silent) komunikat0_str(23,dxf_section);/*delay(50);*/
       if (!read_tables_dxf(f, FALSE)) return nr_linii;
       goto next_section;
     }  
  else if (strcmp (dxf_section, "BLOCKS") == 0)
     {
         if (!glb_silent) komunikat0_str(23,dxf_section);/*delay(50);*/
       if (!read_blocks_dxf(f, FALSE, 0, TRUE)) return nr_linii;
       goto next_section;
     }    
  else if (strcmp (dxf_section, "ENTITIES") == 0)
     {
         if (!glb_silent) komunikat0_str(23,dxf_section);/*delay(50);*/
       if (!read_entities_dxf(f, NULL, 0, TRUE)) return nr_linii;
       entities_ok = TRUE;
       goto next_section;
     }   
  else if (strcmp(dxf_section, "OBJECTS") == 0)
  {
      if (!glb_silent) komunikat0_str(23,dxf_section);/*delay(50);*/
	  if (!read_objects_dxf(f, fn, FALSE)) return nr_linii;
	  objects_ok = TRUE;
	  goto next_section;
  }
  else if (strcmp (dxf_section, "EOF") == 0)
     {
         if (!glb_silent) komunikat0_str(23,dxf_section);/*delay(50);*/
       endoffile=TRUE;
       goto next_section;
     }    

 next_section:   ;
 } 

fclose(f);

//freeing pcx_def
for (int i = 0; i < pcx_number; i++)   free(dxf_pcx_def_p[pcx_number]);

cur_layer=find_layer(CLAYER);

chooselayer(cur_layer);
/*zmiana atrybutow O2BlockDXF na NoBlockS oraz warstwy na 255*/
adr=dane;
adrk=dane + dane_size;
 
if (USERI1 == 211) usun_bloki_obiektt2(adr, adrk, O2BlockDXF);
else zmien_obiektt2_warstwe(adr, adrk, O2BlockDXF, O2BlockDXF, 255);

//zmiana skali elementow ep_space
x_space=-jednostkiOb(EXTMIN_X);
y_space=-jednostkiOb(EXTMIN_Y);
x_space_factor=milimetryob(1);
y_space_factor=milimetryob(1);

get_all_ep_space(NULL, NULL, &ADP, &ADK);
if ((ADP!=NULL) && (ADK!=NULL))
{
    transformacja_blok(ADP,ADK,x_space,y_space,0,0,Tprzesuw,0);
    transformacja_blok(ADP,ADK,x_space,y_space,x_space_factor,y_space_factor,Tskala,0);
    double xminp, xmaxp, yminp, ymaxp;
    Get_Limits(ADP-dane, ADK-dane, Ablok, &xminp, &xmaxp, &yminp, &ymaxp);
    Set_Block_Proc (xminp, yminp, (char*)"*Paper Space Block");
}

ADP = dane;
ADK = dane + dane_size;

if (auto_size == TRUE)
{
	double xmin, xmax, ymin, ymax;
	long_long off = 0, offk = dane_size;

	Get_Limits(off, offk, ANieOkreslony, &xmin, &xmax, &ymin, &ymax);

	//modifying format
	FormatX = max((jednostkiplt(xmax - xmin + 2*offset_dxf_x)+0.5), 50);
	FormatY = max((jednostkiplt(ymax - ymin + 2*offset_dxf_y)+0.5), 50);
	X_max = jednostkiplt(FormatX);
	Y_max = jednostkiplt(FormatY);
	Xmax = X_max + Xmin;
	Ymax = Y_max + Ymin;
	if (X > Xmax) X = Xmax;
	if (Y > Ymax) Y = Ymax;

	Ini_Place_Marker();

    offset_dxf_x -= xmin;
	offset_dxf_y -= ymin;
}
if ((ADP!=NULL) && (ADK!=NULL))
 {
   zmien_atrybut(ADP, ADK, Anormalny, Ablok);
   transformacja_blok(ADP,ADK, offset_dxf_x, offset_dxf_y,0,0,Tprzesuw,0);
   zmien_atrybut(ADP, ADK, Ablok, Anormalny);
 }


SkalaF = SkalaF_dxf;
OkragG.kolor = PointG.kolor = TextG.kolor = LukG.kolor = LiniaG.kolor = GetColorALF(CECOLOR, 0);
switch (INSUNITS)
{

case 4: Jednostki = 1.0;       //mm
	break;
case 5: Jednostki = 10.0;      //cm
	break;
case 6: Jednostki = 1000.0;    //m
	break;
case 7: Jednostki = 1000000.0; //km
        break;
case 1: Jednostki = 25.4;      //"
	break;
case 2: Jednostki = 304.8;     //'
	break;
case 3: Jednostki = 1609344;    //mi
        break;
case 10: Jednostki = 914.4;    //yd
        break;
default: Jednostki = 1.0;
	break;
}

zmwym.wysokosc = DIMTXT;
zmwym.kierunek_t = DIMTOH;
if (DIMCLRD == 0) zmwym.Lkolor = 7;
else zmwym.Lkolor = GetColorALF(DIMCLRD, 0);
if (DIMCLRT == 0) zmwym.Tkolor = 7;
else zmwym.Tkolor = GetColorALF(DIMCLRT, 0);
if (DIMRND == 0.0) zmwym.dokladnosc = 0.01;
else zmwym.dokladnosc = DIMRND;


if (block_record!=NULL) free(block_record);

int l_no=leaders_no;
int ml_no=mleaders_no;

return 0;

fclose(f);
adr=dane;
adrk=dane + dane_size;

if (USERI1 == 211) usun_bloki_obiektt2(adr, adrk, O2BlockDXF);
else zmien_obiektt2_warstwe(adr, adrk, O2BlockDXF, O2NoBlockS, 255);

x_space=-jednostkiOb(EXTMIN_X);
y_space=-jednostkiOb(EXTMIN_Y);
x_space_factor=milimetryob(1);
y_space_factor=milimetryob(1);
offset_dxf_x=0.0;
offset_dxf_y=0.0;

get_all_ep_space(NULL, NULL, &ADP, &ADK);

if ((ADP!=NULL) && (ADK!=NULL))
 {
   transformacja_blok(ADP,ADK,x_space,y_space,0,0,Tprzesuw,0);
   transformacja_blok(ADP,ADK,x_space,y_space,x_space_factor,y_space_factor,Tskala,0);
   reset_przec_dxf(ADP,ADK);
 }

if (block_record!=NULL) free(block_record);
return nr_linii;
}

extern "C" {void get_lines_width_dxf_glb(void); }
void get_lines_width_dxf_glb(void)
{   
	Get_Private_Profile_Strings((T_Fstring)Linie_DXF, get_lines_width_dxf);
}

extern "C" { extern int czytaj_dxf_blok(char *fn, double *X0, double *Y0, RYSPOZ *adp, RYSPOZ *adk, char *buf, double Jednostki_dxf, double SkalaF_dxf, int latin2_dxft, int block_view); }
int czytaj_dxf_blok(char *fn, double *X0, double *Y0, RYSPOZ *adp, RYSPOZ *adk, char *buf, double Jednostki_dxf, double SkalaF_dxf, int latin2_dxf, int block_view)
/*---------------------------------------------------------------------------------------------------------------------*/
{
	FILE *f;
	char dxf_section[MAXLINE], dxf_section_code[MAXLINE];
	BOOL endoffile;
	BOOL entities_ok;
	BOOL objects_ok;
	int cur_layer;
	char  *adr, *adrk;
	double x_space, y_space;
	double x_space_factor, y_space_factor;
	char dir[MAXDIR];
	char drive[MAXDRIVE];
	char file[MAXFILE];
	char ext[MAXEXT];
	int flags;
	char *ADP, *ADK;
	long_long dane_size00;

    last_mtext_h=zmwym.wysokosc;

	dane_size00 = dane_size;
	pcx_number = 0;

    leaders_no=0;

	*adp = NULL;
	*adk = NULL;

	flags = fnsplit(fn, drive, dir, file, ext);

	Jednostki = Jednostki_dxf;
	SkalaF = SkalaF_dxf;

	if (!glb_silent) Get_Private_Profile_Strings((T_Fstring)Linie_DXF, get_lines_width_dxf);

	entities_ok = FALSE;
	objects_ok = FALSE;

	f = fopen(fn, "rt");
	if (f == NULL)
	{   /*komunikat*/
		return -1;
	}

	glb_silent = TRUE;

	nr_linii = 0;
	if (!glb_silent) show_nr_linii();

	endoffile = FALSE;
	while (endoffile == FALSE)
	{
		if (!glb_silent) { if (!inc_nr_linii()) return 0; }
		if (myfgets(dxf_section_code, MAXLINE, f) == NULL) return nr_linii;
		if (!glb_silent) { if (!inc_nr_linii()) return 0; }
		if (myfgets(dxf_section, MAXLINE, f) == NULL)
		{
			if (entities_ok == FALSE)
				return nr_linii;
			else
			{
				endoffile = TRUE;
				goto next_section;
			}
		}
		if (strcmp(dxf_section, "SECTION") == 0)
		{
			goto next_section;
		}
		dxf_section[32] = '\0';
		if (!glb_silent) komunikat0_str(23, dxf_section);/*delay(50);*/

		if (strcmp(dxf_section, "HEADER") == 0)
		{
            if (read_header_dxf(f))
            {
                if ((EXTMAX_X0 < EXTMIN_X0) || (EXTMAX_Y0 < EXTMIN_Y0))
                {
                    EXTMIN_X0 = 0.0;
                    EXTMIN_Y0 = 0.0;
                    EXTMAX_X0 = 1189;
                    EXTMAX_Y0 = 841;

                    EXTMIN_X = EXTMIN_X0;
                    EXTMIN_Y = EXTMIN_Y0;
                    EXTMAX_X = EXTMAX_X0;
                    EXTMAX_Y = EXTMAX_Y0;
                }
            }
            else return nr_linii;
			goto next_section;
		}

		if (strcmp(dxf_section, "TABLES") == 0)
		{
			if (!read_tables_dxf(f, TRUE)) return nr_linii;
			goto next_section;
		}
		if (strcmp(dxf_section, "BLOCKS") == 0)
		{
			if (!read_blocks_dxf(f, FALSE /*TRUE*/, block_view, FALSE)) return nr_linii;
			goto next_section;
		}
		if (strcmp(dxf_section, "ENTITIES") == 0)
		{
			if (!read_entities_dxf(f, TRUE, block_view, FALSE)) return nr_linii;
			entities_ok = TRUE;
			goto next_section;
		}
		if (strcmp(dxf_section, "OBJECTS") == 0)
		{
			if (!read_objects_dxf(f, fn, TRUE)) return nr_linii;
			objects_ok = TRUE;
			goto next_section;
		}
		if (strcmp(dxf_section, "EOF") == 0)
		{
			endoffile = TRUE;
			goto next_section;
		}
	next_section:;
	}

	fclose(f);
	//ustalenie aktualnej warstwy
	cur_layer = Current_Layer;

	chooselayer(cur_layer);
	/*zmiana atrybutow O2BlockDXF na NoBlockS oraz warstwy na 255*/
	adr = dane;
	adrk = dane + dane_size - dane_size00 -1;

	//if (USERI1 == 211)
    usun_bloki_obiektt2(adr, adrk, O2BlockDXF);
	//else zmien_obiektt2_warstwe(adr, adrk, O2BlockDXF, O2NoBlockS, 255);

    adr = dane;
    adrk = dane + dane_size - dane_size00 -1;

    x_space = -jednostkiOb(EXTMIN_X);
	y_space = -jednostkiOb(EXTMIN_Y);
	x_space_factor = milimetryob(1);
	y_space_factor = milimetryob(1);

    ADP = adr; ADK = adrk;

    *adp = adr;
    *adk = adrk;


    if ((ADP != NULL) && (ADK != NULL))
    {
        zmien_atrybut(ADP, ADK, Anormalny, Ablok);
        transformacja_blok(ADP, ADK, x_space, y_space, x_space_factor, y_space_factor, Tskala, 0);
        Set_Block_Proc(0, 0, file);   //blok_type
        ((NAGLOWEK *)dane)->atrybut = Ablok;

    }

	////////korekta_obiektow_blokow();

	adr = dane;
	adrk = dane + dane_size - dane_size00 - 1;

	*adp = adr;
	*adk = adrk;


	*X0 = LIMMIN_X;
	*Y0 = LIMMIN_Y;

	strcpy(buf, file);

    if (block_record!=NULL) free(block_record);

	return nr_linii;

	fclose(f);

	//freeing pcx_def
	for (int i = 0; i < pcx_number; i++)   free(dxf_pcx_def_p[pcx_number]);
	//ClearInfo();
	adr = dane;
	adrk = dane + dane_size - dane_size0 -1;

	if (USERI1 == 211) usun_bloki_obiektt2(adr, adrk, O2BlockDXF);
	zmien_obiektt2_warstwe(adr, adrk, O2BlockDXF, O2NoBlockS, 255);


	x_space = -jednostkiOb(EXTMIN_X);
	y_space = -jednostkiOb(EXTMIN_Y);
	x_space_factor = milimetryob(1);
	y_space_factor = milimetryob(1);
	
	ADP = adr; ADK = adrk;

	if ((ADP != NULL) && (ADK != NULL))
	{
		zmien_atrybut(ADP,ADK,Anormalny,Ablok);

		Set_Block_Proc(0, 0, file /*blok_type*/);

		((NAGLOWEK *)dane)->atrybut = Ablok;

		zmien_atrybut(ADP, ADK, Anormalny, Ablok);
		transformacja_blok(ADP, ADK, x_space, y_space, x_space_factor, y_space_factor, Tskala, 0);
	}

	adr = dane;
	adrk = dane + dane_size - dane_size00 - 1;

	*adp = adr; *adk = adrk;

	///////korekta_obiektow_blokow();
	*X0 = LIMMIN_X;
	*Y0 = LIMMIN_Y;

	strcpy(buf, file);

    if (block_record!=NULL) free(block_record);

	return nr_linii;
}

#undef __O_DXF_IN__