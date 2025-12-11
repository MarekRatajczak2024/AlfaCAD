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

#define __O_PRNFUN__

#define old_prn_point_

#define new_color_print

#define logo_alfacad

#define ALLEGRO_PRN_BMP

#include "forwin.h"
#include <math.h>
#include <string.h>

#include <stdio.h>
#ifdef LINUX
#else
#include <conio.h>
#endif
#include <stdlib.h>
#ifndef LINUX
#include<process.h>
#else
#endif
#include <allegro.h>
#include <iostream>
#include "bib_e.h"
#include "rysuj_e.h"
#include "alffont.h"
#include "alf_res.h"
#include "o_dialog.h"
#include "o_pltprn.h"
#include "o_libfun.h"
#include "o_inicnf.h"
#include "o_prn.h"
#include "o_editst.h"
#include "bgiext.h"
#include "o_protec.h"
#include "hpdf.h"
//#include "hpdf_utils.h"
#ifndef LINUX
//#include "hpdf_image.h"
#endif
#include "o_lttype.h"
#include "hpdf_errorcodes.h"
#include "allegro/internal/aintern.h"

#include "message.h"

#include "leak_detector_cpp.hpp"

//#include "ruby.h"
//#include "ruby/io.h"

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

double ST_W_FACTOR = 1.11;

int BITS_PER_DOT = 24;
int BYTES_PER_DOT = BITS_PER_DOT / 8;

unsigned char BMPpallete[256 * 4];

double PDF_TTF_width_factor = 0.87675; // 0.875 1.25;
double PDF_TTF_height_factor = 1.4025;

float angle_italic = 12.5;  //15.0  //in the range of 12.5 to 15.0

static HPDF_STATUS pdf_error = 0;
static HPDF_STATUS pdf_temp_error = 0;

unsigned char *BMPpallete_;

#define WIDTH_DOT 0.05
#define WIDTH_MIN 0.01
typedef unsigned long       DWORD;

#define bitmap_Image "bitmapImage" //"bitmapImage.bmp"

extern "C" {

extern BOOL wielokat_visible_in_frame(WIELOKAT *w, RECTD *rect);

extern int my_kbhit(void);
extern int my_getch(void);
extern int pcx_w_prostokacie(B_PCX *);
extern int Draw_Pcx_To_PDF(B_PCX *);
extern int Draw_Pcx_To_Drive (B_PCX *,int);
extern void Change_Set_Draw_Window (double , double, double,  double);
extern void Change_oknoS(double dx1, double dy1, double dx2, double dy2);
extern void ObiektTok_No_Superblock (long_long *, long_long , long_long *, int );
extern void PutPixelWin(int,int,int,unsigned char);
extern void PutPixelWinRGB(int, int, int, unsigned char, unsigned char, unsigned char);
extern void StartWinPage(void);
extern void EndWinPage(void);
extern void EndWinDoc(void);
extern int StartWinPrintDoc(char *doc_name);
extern int GetPrintHorizDensity(void);
extern int GetPrintPageWidth(void);
extern int GetPrintPageHeight(void);
extern int GetPrintVertDensity(void);
extern int linia_frozen(LINIA *ad0, char frozen);
extern int spline_frozen(SPLINE *ad0, char frozen);
extern int tekst_frozen(TEXT *ad0, char frozen);
extern int okrag_frozen(OKRAG *ad0, char frozen);
extern int point_frozen(T_Point *ad0, char frozen);
extern int Point_Selected(T_Point *ptrs_point);
extern int ellipse_frozen(ELLIPSE *ad0, char frozen);
extern int ellipticalarc_frozen(ELLIPTICALARC *ad0, char frozen);
extern int wielokat_frozen(WIELOKAT *ad0, char frozen);
extern int solidarc_frozen(SOLIDARC *ad0, char frozen);
extern int luk_frozen(LUK *ad0, char frozen);
extern int vector_frozen(AVECTOR *ad0, char frozen);
extern void set_XY(double del_minx, double del_miny, double del_maxx, double del_maxy);
extern BOOL Copy_File(char *ptrsz_fnd, char *ptrsz_fns);
extern BOOL Delete_File(char *ptrsz_fnd);
extern BOOL get_dest_image_file_name(char *fns, char *fnd, int i_pageno, char *image_format);
extern DWORD RunSilent(char* strFunct, char* strstrParams);
#ifdef LINUX
extern DWORD SystemSilent(char* strFunct, char* strstrParams);
#endif
extern int ask_question(int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien, int image);
extern double Get_Point_Size(void);
extern void komunikat_str_short(char *st, BOOL stay, BOOL center);
extern int get_alfacad_logo_font();
extern T_Prot_Date s_prot_date;
extern char *strlwr2(char *_s);
extern int my_file_exists(char *name);
extern void TTF_text(BITMAP *bmp, char *text, int x, int y);
extern void Draw_TTF_text(TEXT *t, char *t_text, BITMAP *bmp, int x, int y, double kat, float wysokosc_p, COLOR_ kolor, int mode);
extern void Init_Draw_TTF_text(TEXT *t, double kat, float wysokosc_p, COLOR_ kolor);
extern void Amend_Draw_TTF_text(TEXT *t, float wysokosc_p);
extern void Draw_TTF_char(TEXT *t, unsigned int unicode, BITMAP *bmp, int x0, int y0);

extern int type__drive;
extern TTF_font_files ttf_font_files;

extern void oknoS_(double x1, double y1, double x2, double y2);
extern BOOL draw_rectangle_to_drive(double* xy, double dist, int kolor0, BOOL bw, BOOL grey, int translucency);
extern void reset_texts_width(long_long off, long_long offk);
extern BOOL TTF_printing;

extern int setfillpattern_gregre_scaled(char* s_pattern, BOOL to_printer, double resolution, int origin_x, int origin_y, int *origin_x_prn, int *origin_y_prn);
#define CURRENT_PATTERN_DEF_MAX 64
extern BITMAP* pattern_bitmap_scaled[CURRENT_PATTERN_DEF_MAX];

extern double Get_Text_Len(TEXT* ptrs_text, char* ptrsz_t, double font_scale, int font_index, int* t_width, int* t_height, double *matrix_d, int *i_matrix_d);
extern void sort_solid_xy_points(int* xy, int* xys, int lp, int *lps);
extern BOOL linen_plt_pattern(double x1, double y1, double x2, double y2);
extern int get_palette_color(int color);

extern void reset_last_polygon_vectors(void);


extern BOOL draw_polygon_to_drive_pattern(int numpoints, double* xy0);
extern BOOL draw_rectangle_to_drive_pattern(double* xy0);
extern BOOL draw_polygon_to_drive_gradient(int numpoints, float *xy0, unsigned char translucency, GRADIENT4 *gradient, Print_Rect *window_to_print);

extern BITMAP* load_memory_pcx(AL_CONST void* buffer, PALETTE* pal);
extern BITMAP* load_memory_png(AL_CONST void* buffer, int bufsize, RGB* pal);
extern BITMAP* load_memory_jpg(void* buffer, int size, RGB* palette);

extern void make_grey32(BITMAP* png, T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_, BOOL grey, BOOL bw);
extern void make_grey24(BITMAP* png, T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_, BOOL grey, BOOL bw);
extern void make_grey8(BITMAP* png, T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_, BOOL grey, BOOL bw);
extern void outlinetext(TEXT *t, QUAD *outline, double margin);

extern int check_dim_line(LINIA *L);
extern HPDF_Image HPDF_LoadPngImageFromFile_(HPDF_Doc  pdf_doc, char* png_file);

extern void set_trace_block_begin(char *adr);
extern void set_trace_block_end(char *adr);
extern void set_trace_block(BOOL tb);

void my_soft_polygon(BITMAP* bmp, int vertices, AL_CONST int* points, int color, int translucency, int colorB);
void SetPDFClip(int pdf_left, int pdf_bottom, int pdf_right, int pdf_top);

extern HPDF_Image
HPDF_Image_LoadRawImageFromMem(HPDF_MMgr          mmgr,
	const HPDF_BYTE* buf,
	HPDF_Xref          xref,
	HPDF_UINT          width,
	HPDF_UINT          height,
	HPDF_ColorSpace    color_space,
	HPDF_UINT          bits_per_component);
}

T_PTR_Prn_Ini_Date *get_ptrs__prn_ini_date(void);

extern double df__width_fill_pcx;
extern int MakeGrayColor(int kolor);
extern int MakeColor(int kolor);
extern int utf82unicode(unsigned char *utf8text, unsigned char *unicodetext);

extern int Get_HDC_Height(void);

byte_ get_grey_intensity(COLOR_ color);
byte_ get_grey_intensity_ttf(COLOR_ color);


double TOP_MARGIN0=0; 
double LEFT_MARGIN0=0; //0.25      

int BMP_rows;

int vertical_i=0;
static char org_density;
static BOOL set_left_margin=FALSE;
int Serial_Mode=0;
char _n_[4]="\n";
char _prz_[4]=",";
char _sr_[4]=";";

HPDF_Doc  pdf_doc;
HPDF_Font pdf_font;
HPDF_Page pdf_page;

static int pdf_left_margin = 0, pdf_top_margin = 0, pdf_right_margin = 0, pdf_bottom_margin = 0;
static float pdf_page_width = 0.0, pdf_page_height = 0.0;
static int pdf_page_no = 0;

HPDF_Image image[32];

BITMAP *allegro_prn_bmp;

void line_prn_pattern(double x01, double y01, double x02, double y02);
void line_prn_translucent(double x01, double y01, double x02, double y02, int color);
void line_prn_trans(double x01, double y01, double x02, double y02, int kolor256, double text_line_width, BOOL bw, BOOL grey);
BOOL Draw_Line_To_Prn_trans(double x1, double y1, double x2, double y2, int kolor0, double text_line_width, BOOL bw, BOOL grey);

void correct_prn_bitmap_colors(BITMAP* png);
int get_Allegro_color(int kolor256, BOOL bw, BOOL grey);

static int max_x_clip = 0;
static int max_x_clip_sheet = 0;
static int left_clip = 0;
static int left_clip_pdf = 0;
static int left_clip_sheet = 0;
static int right_clip = 0;
static int right_clip_pdf = 0;
static int right_clip_sheet = 0;
static int top_clip = 0;
static int top_clip_pdf = 0;
static int top_clip_sheet = 0;
static int bottom_clip = 0;
static int bottom_clip_pdf = 0;
static int bottom_clip_sheet = 0;
static int min_y_clip = 0;
static int min_y_clip_sheet = 0;
static int max_y_clip = 0;
static int max_y_clip_sheet = 0;
static int extra_off_top=0;
static int extra_off_bottom=0;
static int extra_off_left=0;
static int extra_off_right=0;

#pragma pack(1)
typedef
  struct
   { 
    unsigned char count ;
    char znak ;
   } hpiip_record;
   
#pragma pack()

static char null_d[64]={'\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        };

static char null_d_null[64]={'\377','\377','\377','\377','\377','\377','\377','\377',
                        '\377','\377','\377','\377','\377','\377','\377','\377',
                        '\377','\377','\377','\377','\377','\377','\377','\377',
                        '\377','\377','\377','\377','\377','\377','\377','\377',
                        '\377','\377','\377','\377','\377','\377','\377','\377',
                        '\377','\377','\377','\377','\377','\377','\377','\377',
                        '\377','\377','\377','\377','\377','\377','\377','\377',
                        '\377','\377','\377','\377','\377','\377','\377','\377',
                        };

static unsigned char null_d_PCX[64]={
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        '\0','\0','\0','\0','\0','\0','\0','\0',
                        };

unsigned char gray_colors[17]={16,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,255};

typedef enum { PRN_PCX = 0, PRN_WINDOWS, PRN_PDF, PRN_9, PRN_24, PRN_48, PRN_LASER, PRN_ESC_P2} PRN;
typedef enum { PRN_MODE_EPSON = 0, PRN_MODE_IBM = 1} PRN_MODE ;
typedef enum { D_DRAFT = 1, D_NORMAL, D_DOUBLE, D_HIGH, D_SUPER_HIGH} DENSITY;


typedef struct
{
    char command_mode [MaxComandLen];
    int horizontal_density;  	/*liczba punktow na cal*/
    int vertical_density;
    char  *lp_matrix_c;	/*adres matrycy  cyjan*/
    char  *lp_matrix_m;	/*adres matrycy  magenta*/
    char  *lp_matrix_y;	/*adres matrycy  yellow*/
    char  *lp_matrix_k;	/*adres matrycy  black*/
    char  *lp_matrix_c_s;	/*adres matrycy  cyjan dla sheet*/
    char  *lp_matrix_m_s;	/*adres matrycy  magenta dla sheet*/
    char  *lp_matrix_y_s;	/*adres matrycy  yellow dla sheet*/
    char  *lp_matrix_k_s;	/*adres matrycy  black dla sheet*/
    char  *lp_matrix_null; /*adres matrycy null*/
    char  *lp_matrix_enable; /*adres matrycy enable*/
    char  *lp_matrix_PCX; /*adres matycy PCX*/
    long   len;			/*dlugosc bufora*/
    double xp;			/*okno w jednostkach rysunku*/
    double yp;
	double xk;			/*okno w jednostkach rysunku*/
	double yk;
    double condensed_wsp;	/*1 - druk w skali,
				szerokosc papieru / szerokosc rysunku
				- dla druku skondensowanego*/
				/*wymiary matrycy w jednostkach rysunku*/
    int width_byte;		/*szerokosc w bajtach*/
    double width_max;		/*w jednostkach rysunku*/
    double height_max;		/*w jednostkach rysunku*/
    double width;		/*w jednostkach rysunku*/
    double height;		/*w jednostkach rysunku,wielokrotnosc - 8, 3 * 8, 24 lub 48 punktow*/
	double top_margin;  /*offset y w punktach*/
	double left_margin;  /*offset x w punktach*/
	double bottom_margin;  /*offset y w punktach*/
	double right_margin;  /*offset x w punktach*/
} Matrix_Head;

typedef struct
{   int line_thinest_width_h ;
    int line_thin_width_h ;	/*grubosc w punktach linii cienkiej na wydruku*/
    int line_norm_width_h ;	/*w pliku konfiguracyjnym grubosci podane sa*/
    int line_thick_width_h ;   /*w milimetrach na papierze*/
    int line_thickest_width_h ;
    int line_thinest_width_v ;
    int line_thin_width_v ;
    int line_norm_width_v ;
    int line_thick_width_v ;
    int line_thickest_width_v ;
} T_Line_Width ;

/*

	                             	    LOCAL DECLARATIONS		    

*/
static hpiip_record hpiip [3001];
static unsigned char bufor_wydruku [9100];
static char ibmx24mode = 16;
static BOOL x24mode = FALSE;
static int i__page_min = 0 ;
static T_PTR_Prn_Config    ptrs__prn_config ;
static T_PTR_Prn_Ini_Date  ptrs__prn_ini_date ;

static Matrix_Head	matrix_head ;
static /* const */ char Car_Ret  = 0xd ;
static /* const */ char LF  = 0xa ;

int line_width_type ;

static double line_max_h_width ;	/*margines, dla linii o rzeczywistej
				   grubosci, lezacych na granicy dwu okien*/

static int 	get_vertical_density (void) ;
static void 	clear_matrix (BOOL set_background);
static BOOL 	draw_file_begin (void);
static BOOL 	draw_file_end (void);
static void 	set_line_max_width (void) ;
static char 	get_matrix_byte (int, int, int, int);
static int 	get_width_line (BOOL, double) ;
static BOOL 	draw_matrix_to_file (void);
static BOOL 	draw_next_page (void);
static BOOL 	draw_sheet_prn (void);
static BOOL 	draw_sheet_prn_nrot (void);
static BOOL 	draw_sheet_prn_rot (void);
static BOOL 	draw_to_matrix1___(Print_Rect window_to_print);   //dla zbior_danych2  -  ignorowane
static BOOL     draw_to_matrix_entities(Print_Rect window_to_print, int entities, char frozen);
static BOOL 	draw_to_matrix(Print_Rect window_to_print);
static int 		jednostki_to_prn_x (double, double);
static int 		jednostki_to_prn_y (double, double);
static int 		jednostki_y_to_byte_prn (double);
static int 		jednostki_y_to_prn(double);
static int 		jednostki_x_to_prn(double);
static int 		jednostki_x_to_byte_prn (double);
static int 		jednostki_h_to_byte_prn(double);
static int 		jednostki_w_to_byte_prn(double);
int 		jednostki_h_to_bit_prn(double);
int 		jednostki_w_to_bit_prn(double);
static void		line_prn (double, double, double, double, int, double, BOOL, BOOL);
static BOOL		make_matrix_head (void);
static void 	put_dot(int, int, int);
static BOOL 	set_line_feed (int) ;
static BOOL    draw_left_margin1(int) ;
static BOOL    draw_left_margin (char *, int) ;
static BOOL    draw_top_margin (void) ;
static BOOL    draw_header_PCX (double, double, double, double) ;
static void    draw_pallete_PCX (void);
static BOOL 	set_line_spacing (void) ;
static BOOL    write_laser_command (char *, char *, long) ;
static long    inch_to_decipoints (double) ;
static BOOL    draw_matrix_to_file_PCX (void) ;
static BOOL    draw_matrix_to_file_BMP(void);
static BOOL    draw_matrix_to_file_WINDOWS (void) ;
static int 	mm_to_dot (double, int) ;
static void line_pcx_prn(double x01, double y01, double x02, double y02, COLOR_ kolor, double text_line_width);
static BOOL draw_rectangle_pcx_to_drive(PCX_SOLID *pcx_s, double dist, COLOR_ kolor);

const int fileHeaderSize = 14;
const int infoHeaderSize = 40;
int BMPpalleteSize = 0;

FILE* BMPimageFile;
DWORD BMPfilePos;

char imageFileName[MAXDIR];
char imageFileNamePdf[MAXDIR];

void generateBitmapImageHeaders(unsigned char *image, int height, int width, char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int width, int paddingSize);
unsigned char* createBitmapInfoHeader(int height, int width);
unsigned char* createBMPpallete(void);
static BOOL finish_BMP(int BMP_width, int height_to_fill);
static BOOL draw_top_margin_PCX(int BMP_width, int height_to_fill);
static int draw_top_margin_WINDOWS();

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data);
/*

				GLOBAL FUNCTIONS			 

*/

void jednostki_to_prn_xy(int numpoints, double *xy, int* pxy);


#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler(HPDF_STATUS   error_no,
	HPDF_STATUS   detail_no,
	void         *user_data)
{
	char error_str[64];
	char comment_str[64];
	BOOL ret;
	char *ptr;

	sprintf(error_str, "ERROR: error_no=%04X", (HPDF_UINT)error_no);
	strncpy(comment_str, hpdf_error_detail[error_no - 0x1000], 63);
	comment_str[63] = '\0';
	ptr = strchr(comment_str, '\n');
	if (ptr) *ptr = '\0';
	ret = ask_question(1, "", confirm, "", error_str, 12, comment_str, 11, 1, 62);
	pdf_temp_error = error_no;
}


int get_pdf_left_margin(void)
{
    return pdf_left_margin;
}

int get_pdf_top_margin(void)
{
    return pdf_top_margin;
}

int get_pdf_page_height(void)
{
    return pdf_page_height;
}

int Line_Width (int width_style)
/*-----------------------------*/
{
  int width ;
  double df_width ;

  if (type__drive == PLT_DRIVE) return 0;

  df_width = 0 ;
  width = ( width_style & 224 ) / 32 ;
  switch (width)
  {
    case LINE_THINEST_WIDTH :
	  df_width = ptrs__prn_ini_date->line_thinest_width ;
	  break ;
    case LINE_THIN_WIDTH :
	  df_width = ptrs__prn_ini_date->line_thin_width ;
	  break ;
    case LINE_NORM_WIDTH :
	  df_width = ptrs__prn_ini_date->line_norm_width ;
	  break ;
    case LINE_THICK_WIDTH :
	  df_width = ptrs__prn_ini_date->line_thick_width ;
	  break ;
    case LINE_THICKEST_WIDTH :
	  df_width = ptrs__prn_ini_date->line_thickest_width ;
	  break ;	  
    default :
	  break ;
  }
  Set_Line_Width (jednostki_d_to_plt_d (df_width)) ;
  return width ;
}

int Solid_Line_Width (int solid_width_style)
/*----------------------------------------*/
{
  int width, width1 ;
  double df_width ;

 if (type__drive == PLT_DRIVE) return 0;

  df_width = 0 ;
  width = solid_width_style - 1;
  width1=width;
  switch (width)
  {
    case LINE_MIN_WIDTH :   //-1
     return width ;
     break ;
    case LINE_THINEST_WIDTH :   //0
	  df_width = ptrs__prn_ini_date->line_thinest_width ;
	  break ;
    case LINE_THIN_WIDTH :   //1
	  df_width = ptrs__prn_ini_date->line_thin_width ;
	  break ;
    case LINE_NORM_WIDTH :   //2
	  df_width = ptrs__prn_ini_date->line_norm_width ;
	  break ;
    case LINE_THICK_WIDTH :  //3
	  df_width = ptrs__prn_ini_date->line_thick_width ;
	  break ;
    case LINE_THICKEST_WIDTH :  //4
	  df_width = ptrs__prn_ini_date->line_thickest_width ;
	  break ;
    case 5:   //solid_null
	  df_width = ptrs__prn_ini_date->line_thickest_width ;
     width1=-1;
	  break ;
    case 6:   //solid_enable
	  df_width = ptrs__prn_ini_date->line_thickest_width ;
     width1=-1;
	  break ;
    default :
     df_width = ptrs__prn_ini_date->line_norm_width ;
	  break ;
  }
  Set_Line_Width (jednostki_d_to_plt_d (df_width)) ;
  return width1 ;
}

/*dla wszystkich obiektow poza kolem i obszarem*/
BOOL Draw_Line_To_Prn ( double x1, double y1, double x2, double y2, int kolor, double text_line_width, BOOL bw, BOOL grey)
/*---------------------------------------------------------------------------------------------------------------------*/
{
   line_prn (x1, y1, x2, y2, kolor, text_line_width, bw, grey) ;
  return TRUE ;
}
/*TYLKO DLA RYSOWANIA KOLA I OBSZARU*/
BOOL Draw_Line_To_Prn1 ( double x1, double y1, double x2, double y2, int kolor0, double text_line_width, BOOL bw, BOOL grey)
/*------------------------------------------------------------------------------------------------------------------------*/
{  int kolor;
 kolor=kolor0;

  line_prn (x1, y1, x2, y2, kolor, text_line_width, bw, grey) ;	 	      

  return TRUE ;
}

BOOL Draw_Line_To_Prn_trans(double x1, double y1, double x2, double y2, int kolor0, double text_line_width, BOOL bw, BOOL grey)
/*------------------------------------------------------------------------------------------------------------------------*/
{
	
	line_prn_trans(x1, y1, x2, y2, kolor0, text_line_width, bw, grey);

	return TRUE;
}



BOOL Draw_Line_To_Prn_Pattern(double x1, double y1, double x2, double y2)
/*----------------------------------------------------------------------*/
{

	line_prn_pattern(x1, y1, x2, y2);

	return TRUE;
}

/*

			LOCAL FUNCTIONS 			    

*/

static int get_vertical_density (void)
/*-----------------------------------*/
{
  int i_ret ;

  if ((ptrs__prn_ini_date->prn_type == PRN_LASER) ||
      (ptrs__prn_ini_date->prn_type == PRN_PCX) ||
	  (ptrs__prn_ini_date->prn_type == PRN_PDF)
#ifdef LINUX
      || (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)
#endif
      )
  {
   if (D_DRAFT == ptrs__prn_ini_date->density)
    {
     i_ret = ptrs__prn_config->density_draft;
    }
    else
     if (D_NORMAL == ptrs__prn_ini_date->density)
     {
      i_ret = ptrs__prn_config->density_normal;
     }
    else
     if (D_DOUBLE == ptrs__prn_ini_date->density)
     {
      i_ret = ptrs__prn_config->density_double;
     }
    else
     if (D_HIGH == ptrs__prn_ini_date->density)
     {
      i_ret = ptrs__prn_config->density_high;
     }
	 else
	 if (D_SUPER_HIGH == ptrs__prn_ini_date->density)
	 {
		i_ret = ptrs__prn_config->density_super_high;
	 }
  }
  else
  if (PRN_MODE_EPSON == ptrs__prn_ini_date->prn_mode)
   {
    if (D_HIGH == ptrs__prn_ini_date->density)
     {
      i_ret = ptrs__prn_config->vertical_density_high_EPSON ;
     }
	if (D_SUPER_HIGH == ptrs__prn_ini_date->density)
	{
		i_ret = ptrs__prn_config->vertical_density_super_high_EPSON;
	}
     else
     {
      i_ret = ptrs__prn_config->vertical_density_EPSON ;
     }
   }
  else
  {
   if (D_HIGH == ptrs__prn_ini_date->density)
    {
     i_ret = ptrs__prn_config->vertical_density_high_IBM ;
    }
   if (D_SUPER_HIGH == ptrs__prn_ini_date->density)
   {
	   i_ret = ptrs__prn_config->vertical_density_super_high_IBM;
   }
    else
      {
       i_ret = ptrs__prn_config->vertical_density_IBM ;
      }
  }
  return i_ret ;
}


static int mm_to_dot (double df_mm, int i_density, BOOL dot_cor)
/*------------------------------------------------------------*/
{
  double df_inch ;
  double df_dot ;
  int df_dot_int;

  df_inch = mm_to_inch (df_mm) * matrix_head.condensed_wsp ;
  if (dot_cor) df_dot = floor (0.5 + df_inch * i_density * ptrs__prn_ini_date->line_thick_correction_H) ;
     else df_dot = floor (0.5 + df_inch * i_density * ptrs__prn_ini_date->line_thick_correction_V) ;
  if (df_dot < 1)
  {
    df_dot = 1 ;
  }
  df_dot_int = df_dot ;
  return df_dot_int ;
}


static int get_width_line (BOOL b_hor, double text_line_width)
/*----------------------------------------------------------*/
{
  int width, width_h, width_v ;

  switch (line_width_type)
  {
    case LINE_MIN_WIDTH :
      width_v = mm_to_dot (0, matrix_head.vertical_density,1) ;
      width_h = mm_to_dot (0, matrix_head.horizontal_density,0) ;
      break ;

    case LINE_THINEST_WIDTH :
      width_v = mm_to_dot (ptrs__prn_ini_date->line_thinest_width,
					matrix_head.vertical_density,1) ;
      width_h = mm_to_dot (ptrs__prn_ini_date->line_thinest_width,
					matrix_head.horizontal_density,0) ;
      break;

    case LINE_THIN_WIDTH :
      width_v = mm_to_dot (ptrs__prn_ini_date->line_thin_width,
					matrix_head.vertical_density,1) ;
      width_h = mm_to_dot (ptrs__prn_ini_date->line_thin_width,
					matrix_head.horizontal_density,0) ;
      break;

    case LINE_NORM_WIDTH :
      width_v = mm_to_dot (ptrs__prn_ini_date->line_norm_width,
					matrix_head.vertical_density,1) ;
      width_h = mm_to_dot (ptrs__prn_ini_date->line_norm_width,
					matrix_head.horizontal_density,0) ;
      break;

    case LINE_THICK_WIDTH :
      width_v = mm_to_dot (ptrs__prn_ini_date->line_thick_width,
					matrix_head.vertical_density,1) ;
      width_h = mm_to_dot (ptrs__prn_ini_date->line_thick_width,
					matrix_head.horizontal_density,0) ;
      break;
      
    case LINE_THICKEST_WIDTH :
      width_v = mm_to_dot (ptrs__prn_ini_date->line_thickest_width,
					matrix_head.vertical_density,1) ;
      width_h = mm_to_dot (ptrs__prn_ini_date->line_thickest_width,
					matrix_head.horizontal_density,0) ;
      break;  

    case TEXT_LINE_TYPE :
      width_v = mm_to_dot (text_line_width, matrix_head.vertical_density,1) ;

      width_h = mm_to_dot (text_line_width, matrix_head.horizontal_density,0) ;

      break;

  }

   width = width_v ;
  if (TRUE == b_hor)
  {
    width = width_h ;
  }
     
  return width ;
}


int get_width_line_ (BOOL b_hor, double text_line_width)
{
    return get_width_line (b_hor, text_line_width);
}

static void set_line_max_width (void)
/*------------------------------------*/
{
   double width ;

   width = max (ptrs__prn_ini_date->line_thinest_width,
	   max (ptrs__prn_ini_date->line_thin_width,
	   max (ptrs__prn_ini_date->line_norm_width,
	   max (ptrs__prn_ini_date->line_thick_width,
	        ptrs__prn_ini_date->line_thickest_width)))) ;

   width = plt_d_to_jednostki_d (width) / matrix_head.condensed_wsp ;
   line_max_h_width = width ;
}

static BOOL set_line_feed (int n)
/*------------------------------*/
{
  static char *EPSONFeed;
  char EPSONFeed1 = 4;
  char EPSONFeed2 = 0;
  char EPSONFeed3 = 72;
  char EPSONFeed4 = 0;
  unsigned char EPSONFeed5 = 180;
  char EPSONFeed6 = 0;
  static char *LS =ptrs__prn_config->line_feed_n216;    /* "\033J" ; */
  unsigned char chr1, chr2 ;
  int i ;


  EPSONFeed = "\033[\\";
  chr1 = n / 200 ;
  chr2 = n % 200 ;
  if (x24mode)
  {
   if ((D_HIGH == ptrs__prn_ini_date->density) || (D_SUPER_HIGH == ptrs__prn_ini_date->density))
    {
    char EPSONFeed5 = ptrs__prn_config->line_feed_specjal;  /* np. 360 */
    }

  if (Write_To_Device (EPSONFeed, strlen(EPSONFeed)) != strlen(EPSONFeed))
		 return FALSE;
  if (Write_To_Device (&EPSONFeed1, sizeof(char)) != sizeof(char)) return FALSE;
  if (Write_To_Device (&EPSONFeed2, sizeof(char)) != sizeof(char)) return FALSE;
  if (Write_To_Device (&EPSONFeed3, sizeof(char)) != sizeof(char)) return FALSE;
  if (Write_To_Device (&EPSONFeed4, sizeof(char)) != sizeof(char)) return FALSE;
  if (Write_To_Device (&EPSONFeed5, sizeof(char)) != sizeof(char)) return FALSE;
  if (Write_To_Device (&EPSONFeed6, sizeof(char)) != sizeof(char)) return FALSE;
  }
  if ( chr1>0 )
  {
   for (i = 0 ; i < chr1 ; i++)
   {
    if (Write_To_Device (LS, strlen (LS)) != strlen (LS)) return FALSE;
    chr1 = 200 ;
    if (Write_To_Device (&chr1, sizeof(char)) != sizeof(char)) return FALSE;
   }
  }
  if ( chr2>0 )
  {
   if (Write_To_Device (LS, strlen (LS)) != strlen (LS)) return FALSE;
   if (Write_To_Device (&chr2, sizeof(char)) != sizeof(char)) return FALSE;
  }
  if (( chr1>0 ) || ( chr2>0 ))
  {
   if (Write_To_Device (&Car_Ret, sizeof(char)) != sizeof(char)) return FALSE;
  }
  return TRUE;
}


static BOOL draw_file_begin (void)
/* ----------------------------- */
{
  char b ;
  char *LM="                                                                             ";
  double p_hight, ploter_page_length;
  double ploter_page_width;
  unsigned char p_hight_i;
  int page_size, page_width, resolution;
  double page_size_d, page_width_d;
  char s_page_size[12];
  char s_page_width[12];
  char s_resolution[12];

#ifndef LINUX
  /*inicjacja drukarki*/
  if (ptrs__prn_ini_date->prn_type == PRN_LASER)
    {
//	  if (ptrs__prn_ini_date->ploter==FALSE)  
//	  {
//#ifndef RCFFORMAT
	  if (ptrs__prn_ini_date->oce==FALSE)
	  {
        LM = "\033E";   /*reset*/
        if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
        LM = "\033&l0O"; /*orientacja*/
        if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
        LM = "\033&l0E"; /*top margin*/
        if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
	  }
//	  }
//#endif
/*
     if (ptrs__prn_ini_date->ploter==TRUE)
        {
            p_hight=ceil( ptrs__prn_ini_date->height_paper + ptrs__prn_ini_date->top_margin);
            LM = "\033%-12345X@PJL JOB NAME = ";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "\42ALFcad-Pro\42\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET MIRROR = OFF\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET PALETTESOURCE = SOFTWARE\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET RENDERMODE = COLOR\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET RET = OFF\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET PRINTAREA = FULLSIZE\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
//          LM = "@PJL SET RESOLUTION = 300";
//          if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            page_size = (p_hight * 40) + 40;
            itoa ( page_size,  s_page_size, 10);
            sprintf(LM, "@PJL SET PAPERLENGTH = %s\n", s_page_size);
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            p_width=ceil( ptrs__prn_ini_date->width_paper + ptrs__prn_ini_date->left_margin);
            page_width = (p_width * 40) + 40;
            itoa ( page_width,  s_page_width, 10);
            sprintf(LM, "@PJL SET PAPERWIDTH = %s\n", s_page_width);
//            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET ORIENTATION = PORTRAIT\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET MARGINS = NORMAL\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET PAGE_MEMORY = 0\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET NUM_BANDS = 0\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL ENTER LANGUAGE = PCL\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
        }
*/
    }
   else if (ptrs__prn_ini_date->prn_type == PRN_ESC_P2)
   {
    LM = "\033@";
    if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
    LM = "\033(G";
    if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
    b = 1;
    if ((Write_To_Device (&b, sizeof(char))) != sizeof(char)) return FALSE;
    b = 0;
    if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;
    b = 1;
    if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;

//    if (matrix_head.vertical_density==720)
//      LM = "\033(U";
//    if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
//    b = 1;
//    if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;
//    b = 0;
//    if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;
//    b = 5;
//    if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;

      if (matrix_head.horizontal_density==720)
       {
        LM = "\033(i";
        if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
        b = 1;
        if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;
        b = 0;
        if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;
        b = 1;
        if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;
       }

//      LM = "\033(U";
//      if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
//      b = 1;
//      if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;
//      b = 0;
//      if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;
//      b = 5;
//      if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;
//     }
    LM = "\033+";
    if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
    if (matrix_head.vertical_density==720) b = 1;
     else if (matrix_head.vertical_density==180) b = 2;
      else b = 1;
    if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;
//  b = 0;
//  if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;
//  b = 10;
//  if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;
  //LM = "\033r";
  //if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
  //b = 1;
  //if ((Write_To_Device (&b, sizeof(char)))!= sizeof(char)) return FALSE;
   }
  /* ustawienie dlugosci strony */
  if ( ptrs__prn_ini_date->set_page==TRUE )
   {
   if (ptrs__prn_ini_date->prn_type == PRN_LASER)
    {
    /*przyjmuje sie ze albo jest to A4 albo A3*/
    p_hight=ceil( ptrs__prn_ini_date->height_paper + ptrs__prn_ini_date->top_margin);
    if (ptrs__prn_ini_date->ploter==TRUE)
              {
                /*
                //zdefiniowanie dlugosci wydruku dla trybu HP-GL
                LM = "\033*c";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                page_size = (p_hight / 25.4) + 1;  //w calach
                itoa ( page_size,  s_page_size, 10);
                if (Write_To_Device (s_page_size, strlen (s_page_size)) != strlen (s_page_size)) return FALSE;
                if (Write_To_Device ("L", 1) != 1) return FALSE;
                */
                ////////////////
#ifdef POSTSCRIPT
//#ifdef RCFFORMAT
				if (ptrs__prn_ini_date->oce==TRUE)
				{
		        if (ptrs__prn_ini_date->roll>0)
					{
					LM = "BEGMF \042Oce 9400, version_3.2\042;\n";
					if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
					LM = "MFVERSION 1;\n";
					if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
					LM = "MFDESC \042Oce RCF, Version=2.5, Type=Header, Unit=Metric\042;\n";
					if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
					if (ptrs__prn_ini_date->roll==1) 
					{
					  LM = "APPLDATA 021 \042BY=0, RS=0, RN=1\042;\n";
					  if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
					}
					  else if (ptrs__prn_ini_date->roll==2) 
						{
							LM = "APPLDATA 021 \042BY=0, RS=0, RN=2\042;\n";
							if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
						}
					LM = "APPLDATA 029 \042LR=2, TB=2\042;\n";
					if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
					LM = "APPLDATA 003 \042RO=0\042;\n";
					if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
//				LM = "APPLDATA 023 \042CI=0\042;\n";
//                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
//				LM = "APPLDATA 014 \042PM=0\042;\n";
//                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
//				LM = "APPLDATA 059 \042LG=2\042;\n";
//                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
					LM = "ENDMF;";
					if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
					}

/*
				ploter_page_length=p_hight;
                page_size_d = ploter_page_length;
                page_size_d *= (720/25.4);  //w decipoints
                if (D_HIGH == ptrs__prn_ini_date->density) page_size_d *=2;
                page_size = (int) page_size_d;
                itoa ( page_size,  s_page_size, 10);

				ploter_page_width = ceil( ptrs__prn_ini_date->width_paper + ptrs__prn_ini_date->left_margin);
                page_width_d = ploter_page_width;
                page_width_d *= (720/25.4);
                if (D_HIGH == ptrs__prn_ini_date->density) page_width_d *=2;
                page_width = (int) page_width_d;
                itoa ( page_width,  s_page_width, 10);

				LM = "\033%0BINBP5,1;PS";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
				if (Write_To_Device (s_page_size, strlen (s_page_size)) != strlen (s_page_size)) return FALSE ;
                if (Write_To_Device (",", 1) != 1) return FALSE ;
				if (Write_To_Device (s_page_width, strlen (s_page_width)) != strlen (s_page_width)) return FALSE ;
*/	
				}
//#endif
                LM = "\033-12345X@PJL JOB NAME = \"ALFAX\"\n";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
//				LM = "@PJL MEDIASOURCE = TRAY2\n";
//                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                LM = "@PJL SET MIRROR = OFF\n";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                LM = "@PJL SET PALETTESOURCE = SOFTWARE\n";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                LM = "@PJL SET RENDERMODE = COLOR\n";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                LM = "@PJL SET RET = OFF\n";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                LM = "@PJL SET PRINTAREA = INKEDAREA\n"; //FULLAREA\n";  //INKEDAREA\n";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;

                LM = "@PJL SET RESOLUTION = ";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;

                resolution=matrix_head.horizontal_density;
                itoa (resolution, s_resolution, 10);
                if (Write_To_Device (s_resolution, strlen (s_resolution)) != strlen (s_resolution)) return FALSE ;
                if (Write_To_Device (_n_, 1) != 1) return FALSE ;


                ploter_page_length=p_hight;
                page_size_d = ploter_page_length;
                page_size_d *= (720/25.4);  //w decipoints
				page_size_d *= 1.0;
                if (D_HIGH == ptrs__prn_ini_date->density) page_size_d *=2;
				else if (D_SUPER_HIGH == ptrs__prn_ini_date->density) page_size_d *= 4;
                page_size = (int) page_size_d;
                itoa ( page_size,  s_page_size, 10);

                LM = "@PJL SET PAPERLENGTH = ";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                if (Write_To_Device (s_page_size, strlen (s_page_size)) != strlen (s_page_size)) return FALSE ;
                if (Write_To_Device (_n_, 1) != 1) return FALSE ;

                ploter_page_width = ceil( ptrs__prn_ini_date->width_paper + ptrs__prn_ini_date->left_margin);
                page_width_d = ploter_page_width;
                page_width_d *= (720/25.4);
                if (D_HIGH == ptrs__prn_ini_date->density) page_width_d *=2;
				else if (D_SUPER_HIGH == ptrs__prn_ini_date->density) page_width_d *= 4;
                page_width = (int) page_width_d;
                itoa ( page_width,  s_page_width, 10);

                LM = "@PJL SET PAPERWIDTH = ";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                if (Write_To_Device (s_page_width, strlen (s_page_width)) != strlen (s_page_width)) return FALSE ;
                if (Write_To_Device (_n_, 1) != 1) return FALSE ;

              //  LM = "@PJL SET ORIENTATION = PORTRAIT\n";
              //  if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                LM = "@PJL SET MARGINS = NORMAL\n";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                LM = "@PJL SET PAGE_MEMORY = 0\n";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                LM = "@PJL SET NUM_BANDS = 0\n";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                LM = "@PJL ENTER LANGUAGE = HPGL2\n";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
              //  LM = "\33%1BBP5,1,1,\" #1/alf\"INWU0NP256ECQL51PS38640,11600RO90SC;";

                LM = "\33%1BBP5,1,1,\" #1/alf\"INWU0NP256ECQL51";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                LM= "PS";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                page_size = ploter_page_length * 40 * 1.0;
                itoa ( page_size,  s_page_size, 10);

                page_width = ploter_page_width * 40;
                itoa ( page_width,  s_page_width, 10);

                if (Write_To_Device (s_page_size, strlen (s_page_size)) != strlen (s_page_size)) return FALSE ;
                if (Write_To_Device (_prz_, 1) != 1) return FALSE ;
                if (Write_To_Device (s_page_width, strlen (s_page_width)) != strlen (s_page_width)) return FALSE ;
                if (Write_To_Device (_sr_, 1) != 1) return FALSE ;
          
           //     if (Write_To_Device ("SC;", 3) != 3) return FALSE ;
           //     LM = "IP0,38640,1016,37624SC0,300,0,300AC0,0";
           //     if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                ////////////////
#else
                LM = "\033%3B";    //tryb HP-GL
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;


                //ustawienie dlugosci strony
                /////
                page_size = (p_hight * 40);
                itoa ( page_size,  s_page_size, 10);
                p_width=ceil( ptrs__prn_ini_date->width_paper + ptrs__prn_ini_date->left_margin);
                page_width = (p_width * 40);
                itoa ( page_width,  s_page_width, 10);
                if (Write_To_Device ("PS", 2) != 2) return FALSE;
                if (Write_To_Device (s_page_size, strlen (s_page_size)) != strlen (s_page_size)) return FALSE;
                if (Write_To_Device (",",1) != 1) return FALSE;
                if (Write_To_Device (s_page_width, strlen (s_page_width)) != strlen (s_page_width)) return FALSE;
                if (Write_To_Device (";", 1) != 1) return FALSE;
#endif

                /*
                //zdefiniowanie dlugosci wydruku dla trybu HP-GL
                LM = "\033*c";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                page_size = (p_hight / 25.4) + 1;  //w calach
                itoa ( page_size,  s_page_size, 10);
                if (Write_To_Device (s_page_size, strlen (s_page_size)) != strlen (s_page_size)) return FALSE;
                if (Write_To_Device ("L", 1) != 1) return FALSE;
                */

                /////
                LM = "\033%0A";    //powrot do trybu PCL
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;

                /*
                //zdefiniowanie dlugosci wydruku dla trybu HP-GL
                LM = "\033*c";
                if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
                page_size = (p_hight * 720 / 25.4);  //w decipoint
                itoa ( page_size,  s_page_size, 10);
                page_width = (p_width * 720 / 25.4);
                itoa ( page_width,  s_page_width, 10);

                if (Write_To_Device (s_page_width, strlen (s_page_width)) != strlen (s_page_width)) return FALSE;
                if (Write_To_Device ("x", 1) != 1) return FALSE;
                if (Write_To_Device (s_page_size, strlen (s_page_size)) != strlen (s_page_size)) return FALSE;
                if (Write_To_Device ("Y", 1) != 1) return FALSE;
                */

              }
			  else if (p_hight<=297) /*[mm]*/
				{
					LM = "\033&l26A";
					if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
				}
					else if (p_hight<=420)
					{
						LM = "\033&l27A";
						if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
					}
						else
							{
								LM = "\033&l101A";
								if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
							}
/*            //    LM = "@PJL SET SIZE=a0";
            //    if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "\033%-12345X@PJL JOB NAME = ";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "\42ALFcad-Pro\42\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET MIRROR = OFF\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET PALETTESOURCE = SOFTWARE\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET RENDERMODE = COLOR\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET RET = OFF\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET PRINTAREA = FULLSIZE\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
//          LM = "@PJL SET RESOLUTION = 300";
//          if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            page_size = (p_hight * 40) + 40;
            itoa ( page_size,  s_page_size, 10);
            sprintf(LM, "@PJL SET PAPERLENGTH = %s\n", s_page_size);
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            p_width=ceil( ptrs__prn_ini_date->width_paper + ptrs__prn_ini_date->left_margin);
            page_width = (p_width * 40) + 40;
            itoa ( page_width,  s_page_width, 10);
            sprintf(LM, "@PJL SET PAPERWIDTH = %s\n", s_page_width);
//            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET ORIENTATION = PORTRAIT\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET MARGINS = NORMAL\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET PAGE_MEMORY = 0\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL SET NUM_BANDS = 0\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
            LM = "@PJL ENTER LANGUAGE = HPGL\n";
            if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
*/

             //   {
             //    LM = "\033&l101A";
             //    if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
             //   }
            
    }
    else
    {
     p_hight=ceil(( ptrs__prn_ini_date->height_paper + ptrs__prn_ini_date->top_margin ) / 25.3995 );
     p_hight_i=p_hight;
     LM = "\033C";
     if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
     b = 0;
     if (Write_To_Device (&b, sizeof(char)) != sizeof(char)) return FALSE;
     if (Write_To_Device (&p_hight_i, sizeof(char)) != sizeof(char)) return FALSE;
    }
   }

  if (ptrs__prn_ini_date->prn_type == PRN_LASER)
    {
      if ((ptrs__prn_ini_date->color_print==TRUE) && (ptrs__prn_ini_date->color_mode !=3))
      {
	     LM = "\033*rbC\033*r-4U\033*p0N\033*o2D\033*o0Q" ;  /*start raster graphics margin in the current column*/
      }
      else
       {
	      LM = "" ;  /*start raster graphics margin in the current column*/
       }
    }
     else
       {
    	   LM = "" ;
       }
  if ( strlen (LM) > 0)
   {
    if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
   }

#endif

  return TRUE;
}


static BOOL  draw_file_end (void)
/*------------------------------*/
{
  static char *NormalFeed;
  char NormalFeed1 = 4;
  char NormalFeed2 = 0;
  char NormalFeed3 = 72;
  char NormalFeed4 = 0;
  unsigned char NormalFeed5 = 216;
  char NormalFeed6 = 0;
  char b;
  int i ;
  char *LM ;

  NormalFeed = "\033[\\";

   if (ptrs__prn_ini_date->prn_type == PRN_LASER)  /*reset drukarki */
    {
      if (Write_To_Device (&Car_Ret, sizeof(char)) != sizeof(char)) return FALSE;
      LM = "\033*r1U\033*rB";
      if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;

      if (ptrs__prn_ini_date->ploter==TRUE)
        {
		  if (ptrs__prn_ini_date->roll>0)
		  {
			LM = "\033%0BPG;IN;\n";    //tryb HP-GL
			if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
			//  LM = "\033%-12345X@PJL EOJ\n";
			//  if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
		  }
			else
			{
				LM = "\033E";   /*reset*/
				if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
			}
        }
     else
      {
       LM = "\033E";   /*reset*/
       if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
      }
    }
    else if (ptrs__prn_ini_date->prn_type == PRN_ESC_P2)
      {
        if  (( ptrs__prn_ini_date->change_page ) == 1)
          {
           if (Write_To_Device (&Car_Ret, sizeof(char)) != sizeof(char)) return FALSE;
           if (Write_To_Device (&LF, sizeof(char)) != sizeof(char)) return FALSE;
           b = 12;  /*FF = 12*/
           if (Write_To_Device (&b, sizeof(char)) != sizeof(char)) return FALSE;
           LM = "\033@";
           if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
          }
        else
         {
          LM = "\033@";
          if (Write_To_Device (LM, strlen (LM)) != strlen (LM)) return FALSE ;
          if (Write_To_Device (&Car_Ret, sizeof(char)) != sizeof(char)) return FALSE;
          for (i = 0 ; i < 5 ; i++)
            { if (Write_To_Device (&LF, sizeof(char)) != sizeof(char)) return FALSE; }
         }
      }
    else
    {
     if  (( ptrs__prn_ini_date->change_page ) == 1)
      {
        b = 12;  /*FF = 12*/
        if (Write_To_Device (&b, sizeof(char)) != sizeof(char)) return FALSE;
        if (Write_To_Device (&Car_Ret, sizeof(char)) != sizeof(char)) return FALSE;
      }

    if (x24mode)
     {
      if (Write_To_Device (NormalFeed, strlen(NormalFeed)) != strlen(NormalFeed))
	     return FALSE;
      if (Write_To_Device (&NormalFeed1, sizeof(char)) != sizeof(char)) return FALSE;
      if (Write_To_Device (&NormalFeed2, sizeof(char)) != sizeof(char)) return FALSE;
      if (Write_To_Device (&NormalFeed3, sizeof(char)) != sizeof(char)) return FALSE;
      if (Write_To_Device (&NormalFeed4, sizeof(char)) != sizeof(char)) return FALSE;
      if (Write_To_Device (&NormalFeed5, sizeof(char)) != sizeof(char)) return FALSE;
      if (Write_To_Device (&NormalFeed6, sizeof(char)) != sizeof(char)) return FALSE;
     }
     if (ptrs__prn_ini_date->change_page == 0)
      {
       if (Write_To_Device (&Car_Ret, sizeof(char)) != sizeof(char)) return FALSE;
       for (i = 0 ; i < 5 ; i++)
        { if (Write_To_Device (&LF, sizeof(char)) != sizeof(char)) return FALSE; }
      }
    }

  return TRUE;
}

static BOOL draw_top_margin (void)
/*------------------------------*/
{
  int n ;
  long l_ver_margin ;
  int vertical_feed ;

  set_left_margin=TRUE;

  TOP_MARGIN0= (0.5 + 3.5/25.3995);
  LEFT_MARGIN0=0.25;
 
  if (ptrs__prn_ini_date->prn_type == PRN_LASER)
  {
	if (ptrs__prn_ini_date->ploter==TRUE) TOP_MARGIN0 = 0;
	   else TOP_MARGIN0= (0.5 + 3.5/25.3995);
    if (ptrs__prn_ini_date->ploter==TRUE) LEFT_MARGIN0=0;
	   else LEFT_MARGIN0=0.25;
	    
    l_ver_margin = inch_to_decipoints (
		 mm_to_inch (ptrs__prn_ini_date->top_margin) - TOP_MARGIN0) ;
    if (write_laser_command ("\033&a", "V", l_ver_margin) != TRUE) return FALSE ;
  }
  else
  {
   if (ptrs__prn_ini_date->prn_type == PRN_9) { vertical_feed = 216; }
    else if (ptrs__prn_ini_date->prn_type == PRN_24) { vertical_feed = 216; }
     else
      {
	if ((D_HIGH == ptrs__prn_ini_date->density) || (D_SUPER_HIGH == ptrs__prn_ini_date->density))
	  {
	   vertical_feed=ptrs__prn_config->line_feed_specjal;  /* np. 360 */
	   }
	   else
	    {
	    vertical_feed = 180 ;
	    }
       }
    n = mm_to_inch (ptrs__prn_ini_date->top_margin) *
       vertical_feed /* get_vertical_density ()*/ ;
    if (set_line_feed (n) == FALSE) return FALSE ;
  }
  return TRUE ;
}


static BOOL draw_next_page (void)
/*------------------------------*/
{
  char b ;
  int i ;

  //jezeli jest to zapis PCX, nalezy otworzyc kolejny plik i utworzyc nowy PCX

  set_left_margin=TRUE;
    if (ptrs__prn_ini_date->change_page == 1)
  {
    b = 12;  /*FF = 12*/
    if (Write_To_Device (&b, sizeof(char)) != sizeof(char)) return FALSE;
    if (Write_To_Device (&Car_Ret, sizeof(char)) != sizeof(char)) return FALSE;
  }
  if (ptrs__prn_ini_date->change_page == 0)
   {
    if (Write_To_Device (&Car_Ret, sizeof(char)) != sizeof(char)) return FALSE;
    for (i = 0 ; i < 5 ; i++)
     { if (Write_To_Device (&LF, sizeof(char)) != sizeof(char)) return FALSE; }
   }

  if (ptrs__prn_ini_date->out_to_file == FALSE &&
      ptrs__prn_ini_date->print_pause == TRUE)
  {
    komunikat (57);
    Get_Key ();
    komunikat (46);
  }
  return draw_top_margin () ;
}

static BOOL draw_left_margin1(int i_dotno)
/*-----------------------------------------*/
{
char b;
int j;
int l_head;
int i_dotno1;

 b = 0;
 if (ptrs__prn_ini_date->prn_type== PRN_24)
  {
  l_head=3;
  }
  else if (ptrs__prn_ini_date->prn_type== PRN_48)
   {
   l_head=6;
   }
   else l_head=1;

 i_dotno1=i_dotno * l_head;

 if (i_dotno>0)
 { for (j=1; j<=i_dotno1; j++)
  {
   if (Write_To_Device (&b, sizeof(char)) != sizeof(char)) return FALSE;
  }
 }
 return TRUE ;

}


static BOOL draw_left_margin (char *ptrsz_buf, unsigned int i_buflen)
/*----------------------------------------------------------*/

{
  static char *CMIBM = "\033[g";
  char *CM = matrix_head.command_mode;
  unsigned char n1, n2, i_dotnox24n1, i_dotnox24n2 ;
  unsigned int i_dotno, i_dotnox24 ;

  CMIBM = "\033[g";
  i_dotno = mm_to_inch (ptrs__prn_ini_date->left_margin) *
		       matrix_head.horizontal_density /8 ;
  i_dotno = (i_dotno > i_buflen) ? i_buflen : i_dotno ;
  if (x24mode)
  { i_dotnox24 = (i_dotno*6)+1 ;
    i_dotnox24n1 = i_dotnox24 % 256 ;
    i_dotnox24n2 = i_dotnox24 / 256 ;
  }
  n1 = i_dotno % 256 ;
  n2 = i_dotno / 256 ;
  
  memset (ptrsz_buf, i_dotno, 0) ;
  if (x24mode)
 { if (Write_To_Device (CMIBM, strlen (CMIBM)) != strlen (CMIBM)) return FALSE ;
   if (Write_To_Device (&i_dotnox24n1, sizeof (i_dotnox24n1)) != sizeof(i_dotnox24n1)) return FALSE ;
   if (Write_To_Device (&i_dotnox24n2, sizeof (i_dotnox24n2)) != sizeof(i_dotnox24n2)) return FALSE ;
	  
  }
  else
  {
  if (Write_To_Device (CM, strlen (CM)) != strlen (CM)) return FALSE ;
  if (Write_To_Device (&n1, sizeof (n1)) != sizeof(n1)) return FALSE ;
  if (Write_To_Device (&n2, sizeof(n2)) != sizeof(n2)) return FALSE ;
  }
  if (x24mode)
  { if (Write_To_Device (&ibmx24mode, 1) != 1) return FALSE ;
  }
if (Write_To_Device (ptrsz_buf, i_dotno * sizeof(char))!= i_dotno* sizeof(char))
	 return FALSE ;
  return TRUE ;
}

/*-------------------------------------------------*/

static BOOL draw_sheet_prn (void)
/*------------------------------*/
{

    if (ptrs__prn_ini_date->rotation == TRUE)
    {
      return draw_sheet_prn_rot();
    }
    else
    {
	   return draw_sheet_prn_nrot();
    }
}


BOOL Draw_Window (double xmin, double ymin, double xmax, double ymax,
 double width, double height, double left_margin)
/*---------------------------------------------------------------------*/
{
    double xmin_;
   double xmax_, ymin_, ymax_;
  Print_Rect window_to_print;
  int x1, y1, x2, y2;

  double max_x_clip1;
  int max_right, max_bottom;

    byte_ int_intensity;
    byte_ intensity;
    float intensity_tab[4] = { 0.2, 1.0, 0.8, 0.6 };

  max_x_clip = 0;
  max_x_clip_sheet = 0;
  left_clip = 0;
  left_clip_sheet = 0;
  right_clip = 0;
  right_clip_sheet = 0;
  top_clip = 0;
  top_clip_pdf = 0;
  top_clip_sheet = 0;
  bottom_clip = 0;
  bottom_clip_pdf = 0;
  bottom_clip_sheet = 0;
  min_y_clip = 0;
  min_y_clip_sheet = 0;
  max_y_clip = 0;
  max_y_clip_sheet = 0;

  
  if (ptrs__prn_ini_date->inversion == TRUE)
    {
     if (ptrs__prn_ini_date->rotation == TRUE)
     {
	  /*
      xmin_ = FormatX - xmin - width ;
      ymin_ = FormatY - ymin - height;
      xmin = xmin_;
      ymin = ymin_;
      xmax_ = xmin + width;
      ymax_ = ymin + height;
      xmax = xmax_;
      ymax = ymax_; 
	  */
     }
     else
     {

     //// xmin_ = FormatX_S - xmin - width ; ////
      ymin_ = FormatY_S - ymin - height + FormatY_S0;
      ymin = ymin_;
      xmax_ = xmin + width;
      ymax_ = ymin + height;
     //// xmax = xmax_; ////
      ymax = ymax_;

     }
    }

  matrix_head.xp = xmin;
  matrix_head.yp = ymin;

  matrix_head.width = width ;
  matrix_head.height = height ;
  matrix_head.width_byte = jednostki_w_to_byte_prn (matrix_head.width);   ///????  w or x  ?
  window_to_print.x_min = xmin - line_max_h_width; //*40;
  window_to_print.y_min = ymin - line_max_h_width; //*40;
  window_to_print.x_max = xmax + line_max_h_width; //*40;
  window_to_print.y_max = ymax + line_max_h_width; //*40;

  matrix_head.top_margin = jednostki_h_to_bit_prn(ptrs__prn_ini_date->top_margin);
  matrix_head.left_margin = jednostki_w_to_bit_prn(ptrs__prn_ini_date->left_margin);
  matrix_head.bottom_margin = jednostki_h_to_bit_prn(ptrs__prn_ini_date->bottom_margin);
  matrix_head.right_margin = jednostki_w_to_bit_prn(ptrs__prn_ini_date->right_margin);

  if (ptrs__prn_ini_date->prn_type != PRN_PDF)
  {
      max_x_clip = allegro_prn_bmp->w - matrix_head.right_margin;
      max_x_clip_sheet = allegro_prn_bmp->w - matrix_head.right_margin;
      max_y_clip = allegro_prn_bmp->h;
      max_y_clip_sheet = allegro_prn_bmp->h;
  }
  else
  {
      max_x_clip = pdf_page_width - matrix_head.right_margin;
      max_x_clip_sheet = pdf_page_width - matrix_head.right_margin;
      max_y_clip = pdf_page_height - matrix_head.top_margin;
      max_y_clip_sheet = pdf_page_height - matrix_head.top_margin;

      top_clip_pdf = pdf_page_height -  matrix_head.top_margin;

      extra_off_top = jednostki_w_to_byte_prn(5);
      extra_off_left = jednostki_w_to_byte_prn(-1);
      extra_off_bottom = jednostki_w_to_byte_prn(3);
      extra_off_right = jednostki_w_to_byte_prn(1);
  }

  if ((ptrs__prn_ini_date->print_window == TRUE) || (ptrs__prn_ini_date->print_sheet == TRUE))
  {

		if (ptrs__prn_ini_date->inversion == TRUE)
		{
			if (ptrs__prn_ini_date->rotation == TRUE)  //in this mode it has to be shifted by xp, yp
			{
                left_clip = jednostki_w_to_bit_prn((ptrs__prn_ini_date->yp_w - ptrs__prn_ini_date->yp) * matrix_head.condensed_wsp);
                if (left_clip<0) left_clip=0;
				left_clip_pdf = jednostki_w_to_bit_prn((ptrs__prn_ini_date->yp_w - ptrs__prn_ini_date->yp - ymin) * matrix_head.condensed_wsp);
                if (left_clip_pdf<0) left_clip_pdf=0;

                right_clip = jednostki_w_to_bit_prn((ptrs__prn_ini_date->yk - ptrs__prn_ini_date->yk_w) * matrix_head.condensed_wsp);
				right_clip_pdf = jednostki_w_to_bit_prn((ptrs__prn_ini_date->yk - ptrs__prn_ini_date->yk_w) * matrix_head.condensed_wsp);

				max_x_clip = jednostki_w_to_bit_prn(matrix_head.width) - matrix_head.right_margin;


                max_x_clip_sheet = matrix_head.left_margin + jednostki_w_to_bit_prn((ptrs__prn_ini_date->yk) * matrix_head.condensed_wsp);

                max_y_clip_sheet = pdf_page_height - matrix_head.top_margin;

				//max_x_clip cannot exceed the window boundaries
				max_x_clip1 = matrix_head.left_margin + jednostki_w_to_bit_prn((ptrs__prn_ini_date->yk_w - ymin) * matrix_head.condensed_wsp);
				if (max_x_clip1 < max_x_clip) max_x_clip = max_x_clip1;

				top_clip = jednostki_w_to_bit_prn((ptrs__prn_ini_date->xp_w - xmin) * matrix_head.condensed_wsp);
				if (top_clip > 0) min_y_clip = top_clip;

                bottom_clip_pdf = pdf_page_height - jednostki_to_prn_y(ptrs__prn_ini_date->xk_w, 0) - matrix_head.bottom_margin - matrix_head.top_margin ;
                if (bottom_clip_pdf<0)
                    bottom_clip_pdf=0;

				bottom_clip = jednostki_w_to_bit_prn((ptrs__prn_ini_date->xk_w -  xmin) * matrix_head.condensed_wsp);
				if (bottom_clip > 0) max_y_clip = bottom_clip;

                top_clip_pdf = pdf_page_height - jednostki_to_prn_y(ptrs__prn_ini_date->xp_w, 0) - matrix_head.top_margin;
                ////if (top_clip_pdf>(jednostki_w_to_bit_prn(matrix_head.height_max) - matrix_head.top_margin))    ////???????
                ////    top_clip_pdf=(jednostki_w_to_bit_prn(matrix_head.height_max) - matrix_head.top_margin);

			}
			else
			{
                left_clip = jednostki_w_to_bit_prn((ptrs__prn_ini_date->xk - ptrs__prn_ini_date->xk_w + ptrs__prn_ini_date->xp - xmin) * matrix_head.condensed_wsp);
                if (left_clip<0) left_clip=0;
                left_clip_pdf = jednostki_w_to_bit_prn((ptrs__prn_ini_date->xk -  ptrs__prn_ini_date->xk_w  + ptrs__prn_ini_date->xp - xmin) * matrix_head.condensed_wsp);
                if (left_clip_pdf<0) left_clip_pdf=0;


                right_clip = jednostki_w_to_bit_prn((xmax -xmin - (ptrs__prn_ini_date->xp_w - ptrs__prn_ini_date->xp)) * matrix_head.condensed_wsp);
                right_clip_pdf = jednostki_w_to_bit_prn((xmax -xmin - (ptrs__prn_ini_date->xp_w - ptrs__prn_ini_date->xp)) * matrix_head.condensed_wsp);


				max_x_clip = matrix_head.left_margin + jednostki_w_to_bit_prn((xmax  - (ptrs__prn_ini_date->xp_w - ptrs__prn_ini_date->xp) - xmin) * matrix_head.condensed_wsp);

                max_x_clip_sheet = matrix_head.left_margin + jednostki_w_to_bit_prn((xmax /*- ptrs__prn_ini_date->xp*/ - xmin) * matrix_head.condensed_wsp);
                max_y_clip_sheet = pdf_page_height - matrix_head.top_margin;


				top_clip = jednostki_w_to_bit_prn((ymax - ptrs__prn_ini_date->yk_w) * matrix_head.condensed_wsp);
				if (top_clip > 0) min_y_clip = top_clip;


                float alpha = ptrs__prn_ini_date->yp + ptrs__prn_ini_date->yk - ptrs__prn_ini_date->yk_w;
                bottom_clip_pdf = pdf_page_height - jednostki_to_prn_y(0, alpha /*ptrs__prn_ini_date->yp_w*/) - matrix_head.bottom_margin - matrix_head.top_margin ;
                if (bottom_clip_pdf<0)
                    bottom_clip_pdf=0;

                bottom_clip = jednostki_w_to_bit_prn((ymax - ptrs__prn_ini_date->yp_w) * matrix_head.condensed_wsp);
				if (bottom_clip > 0) max_y_clip = bottom_clip;

                alpha = - ptrs__prn_ini_date->yp_w + ptrs__prn_ini_date->yp + ptrs__prn_ini_date->yk;
                top_clip_pdf = pdf_page_height - jednostki_to_prn_y(0, alpha /*ptrs__prn_ini_date->yk_w*/) - matrix_head.top_margin;
                ////if (top_clip_pdf>(jednostki_w_to_bit_prn(matrix_head.height_max) - matrix_head.top_margin))   ////????????
                ////    top_clip_pdf=(jednostki_w_to_bit_prn(matrix_head.height_max) - matrix_head.top_margin);
			}
		}
		else
		{
			if (ptrs__prn_ini_date->rotation == TRUE)
			{
                left_clip = jednostki_w_to_bit_prn((ptrs__prn_ini_date->yp_w - ptrs__prn_ini_date->yp) * matrix_head.condensed_wsp);
                if (left_clip<0) left_clip=0;
				left_clip_pdf = jednostki_w_to_bit_prn((ptrs__prn_ini_date->yp_w /*- ptrs__prn_ini_date->yp*/ - ymin) * matrix_head.condensed_wsp);
                if (left_clip_pdf<0) left_clip_pdf=0;

                right_clip = jednostki_w_to_bit_prn((ptrs__prn_ini_date->yk - ptrs__prn_ini_date->yk_w) * matrix_head.condensed_wsp);
				right_clip_pdf = jednostki_w_to_bit_prn((ptrs__prn_ini_date->yk - ptrs__prn_ini_date->yk_w) * matrix_head.condensed_wsp);

				max_x_clip = jednostki_w_to_bit_prn(matrix_head.width) - matrix_head.right_margin;

				//max_x_clip cannot exceed the window boundaries
				max_x_clip1 = matrix_head.left_margin + jednostki_w_to_bit_prn((ptrs__prn_ini_date->yk_w - ymin) * matrix_head.condensed_wsp);
				if (max_x_clip1 < max_x_clip) max_x_clip = max_x_clip1;

                max_x_clip_sheet = matrix_head.left_margin + jednostki_w_to_bit_prn((ptrs__prn_ini_date->yk - ymin) * matrix_head.condensed_wsp);

                max_y_clip_sheet = pdf_page_height - matrix_head.top_margin;

				top_clip = jednostki_w_to_bit_prn((ptrs__prn_ini_date->xp_w - xmin) * matrix_head.condensed_wsp);
				if (top_clip > 0) min_y_clip = top_clip;

                bottom_clip_pdf = pdf_page_height - jednostki_to_prn_y(ptrs__prn_ini_date->xk_w, 0) - matrix_head.bottom_margin - matrix_head.top_margin ;
                if (bottom_clip_pdf<0)
                    bottom_clip_pdf=0;

				bottom_clip = jednostki_w_to_bit_prn((ptrs__prn_ini_date->xk_w - xmin) * matrix_head.condensed_wsp);
				if (bottom_clip > 0) max_y_clip = bottom_clip;

                top_clip_pdf = pdf_page_height - jednostki_to_prn_y(ptrs__prn_ini_date->xp_w, 0) - matrix_head.top_margin;
                ////if (top_clip_pdf>(jednostki_w_to_bit_prn(matrix_head.height_max) - matrix_head.top_margin))   ////????????
                ////    top_clip_pdf=(jednostki_w_to_bit_prn(matrix_head.height_max) - matrix_head.top_margin);
			}
			else
			{
                left_clip = jednostki_w_to_bit_prn((ptrs__prn_ini_date->xp_w - ptrs__prn_ini_date->xp)*matrix_head.condensed_wsp);
                if (left_clip<0) left_clip=0;
				left_clip_pdf = jednostki_w_to_bit_prn((ptrs__prn_ini_date->xp_w - /*ptrs__prn_ini_date->xp -*/ xmin)*matrix_head.condensed_wsp);
                if (left_clip_pdf<0) left_clip_pdf=0;

                right_clip = jednostki_w_to_bit_prn((ptrs__prn_ini_date->xk - ptrs__prn_ini_date->xk_w)*matrix_head.condensed_wsp);
				right_clip_pdf = jednostki_w_to_bit_prn((ptrs__prn_ini_date->xk - ptrs__prn_ini_date->xk_w)*matrix_head.condensed_wsp);
				  
				max_x_clip = jednostki_w_to_bit_prn(matrix_head.width) - matrix_head.right_margin;
				//max_x_clip cannot exceed the window boundaries
				max_x_clip1 = matrix_head.left_margin + jednostki_w_to_bit_prn((ptrs__prn_ini_date->xk_w - xmin) * matrix_head.condensed_wsp);
				if (max_x_clip1 < max_x_clip) max_x_clip = max_x_clip1;

                max_x_clip_sheet = matrix_head.left_margin + jednostki_w_to_bit_prn((ptrs__prn_ini_date->xk - xmin) * matrix_head.condensed_wsp);
                max_y_clip_sheet = matrix_head.bottom_margin + jednostki_w_to_bit_prn((ptrs__prn_ini_date->yk - ymin) * matrix_head.condensed_wsp);


				top_clip = jednostki_w_to_bit_prn((ymax - ptrs__prn_ini_date->yk_w)*matrix_head.condensed_wsp);
				if (top_clip > 0) min_y_clip = top_clip;

                bottom_clip_pdf = pdf_page_height - jednostki_to_prn_y(0, ptrs__prn_ini_date->yp_w) - matrix_head.bottom_margin - matrix_head.top_margin ;
                if (bottom_clip_pdf<0)
                    bottom_clip_pdf=0;

				bottom_clip = jednostki_w_to_bit_prn((ymax - ptrs__prn_ini_date->yp_w) * matrix_head.condensed_wsp);
				if (bottom_clip > 0) max_y_clip = bottom_clip;

                top_clip_pdf = pdf_page_height - jednostki_to_prn_y(0, ptrs__prn_ini_date->yk_w) - matrix_head.top_margin;
                ////if (top_clip_pdf>(jednostki_w_to_bit_prn(matrix_head.height_max) - matrix_head.top_margin))  ////????????
                ////    top_clip_pdf=(jednostki_w_to_bit_prn(matrix_head.height_max) - matrix_head.top_margin);
				  
			}
		}
  }

  Set_Draw_Window (window_to_print.x_min, window_to_print.y_min, window_to_print.x_max, window_to_print.y_max);
  if ((ptrs__prn_ini_date->prn_type == PRN_PCX) || (ptrs__prn_ini_date->prn_type == PRN_WINDOWS))
  {
      set_clip_rect(allegro_prn_bmp, 0, 0,
                    allegro_prn_bmp->w, allegro_prn_bmp->h);
      clear_matrix(FALSE);

      max_right=jednostki_to_prn_x(xmax-xmin, ymax-ymin) + matrix_head.left_margin;
      if (max_x_clip<max_right) max_right=max_x_clip;
      max_bottom=jednostki_to_prn_y(xmax-xmin, ymin);
      if (max_y_clip<max_bottom) max_bottom=max_y_clip;

	  if (ptrs__prn_ini_date->prn_type != PRN_PDF) {
          if ((ptrs__prn_ini_date->print_window == TRUE) || (ptrs__prn_ini_date->print_sheet == TRUE))
              set_clip_rect(allegro_prn_bmp, matrix_head.left_margin + left_clip_sheet, min_y_clip_sheet,
                            max_x_clip_sheet, max_y_clip_sheet);
          else
              set_clip_rect(allegro_prn_bmp, matrix_head.left_margin + left_clip, min_y_clip, max_right /*max_x_clip*/, max_bottom /*max_y_clip*/);
      }
	  //clear_matrix(TRUE);
      get_clip_rect(allegro_prn_bmp, &x1, &y1, &x2, &y2);
      int b_color24;
#ifdef LINUX
      if (ptrs__prn_ini_date->background) {
          int b_color;
          int tmpR, tmpG, tmpB, tmpA;
          b_color = get_palette_color(kolory.paper);
          //tmpB = b_color & 0xFF; b_color >>= 8;  //it was before, but now is changed
          //tmpG = b_color & 0xFF; b_color >>= 8;
          //tmpR = b_color & 0xFF; b_color >>= 8;
          tmpR = b_color & 0xFF; b_color >>= 8;
          tmpG = b_color & 0xFF; b_color >>= 8;
          tmpB = b_color & 0xFF; b_color >>= 8;

          tmpA = b_color & 0xFF; /* dwColor >>= 8; */

          b_color24 = ((tmpR & 0xff) << 16) + ((tmpG & 0xff) << 8) + (tmpB & 0xff);

          if ((ptrs__prn_ini_date->gray_print) || !(ptrs__prn_ini_date->color_print) || (ptrs__prn_ini_date->PCX_gray))  //|| (ptrs__prn_ini_date->PCX_gray) this is not clear if should be
          {
              intensity = 0.2989 * tmpR + 0.5870 * tmpG + 0.1140 * tmpB;

              if (intensity < 252) intensity *= intensity_tab[ptrs__prn_ini_date->gray_saturation];

              int_intensity = (byte_)intensity;
              b_color24 = makecol(int_intensity, int_intensity, int_intensity);

          }
          else if (!(ptrs__prn_ini_date->color_print))
          {
              if ((tmpB < 252) && (tmpG < 252) && (tmpB < 252))
              {
                  //int_intensity = 0xFF;
                  //int_intensity = 0;
                  //b_color24 = makecol(int_intensity, int_intensity, int_intensity);
                  b_color24=makecol(255, 255, 255);
              }
          }
      }
      else b_color24=makecol(255, 255, 255);
#else
      if (ptrs__prn_ini_date->background)
          {
			  int b_color;
			  int tmpR, tmpG, tmpB, tmpA;

			  b_color = get_palette_color(kolory.paper);

			  tmpR = b_color & 0xFF; b_color >>= 8;
			  tmpG = b_color & 0xFF; b_color >>= 8;
			  tmpB = b_color & 0xFF; b_color >>= 8;
			  tmpA = b_color & 0xFF; /* dwColor >>= 8; */

			  b_color24 = ((tmpR & 0xff) << 16) + ((tmpG & 0xff) << 8) + (tmpB & 0xff);

              if ((ptrs__prn_ini_date->gray_print) || !(ptrs__prn_ini_date->color_print) || (ptrs__prn_ini_date->PCX_gray))  //|| (ptrs__prn_ini_date->PCX_gray) this is not clear if should be
              {
				  intensity = 0.2989 * tmpR + 0.5870 * tmpG + 0.1140 * tmpB;

                  if (intensity < 252) intensity *= intensity_tab[ptrs__prn_ini_date->gray_saturation];

                  int_intensity = (byte_)intensity;
                  b_color24 = makecol(int_intensity, int_intensity, int_intensity);

              }
              else if (!(ptrs__prn_ini_date->color_print))
              {
                  if ((tmpR < 252) && (tmpG < 252) && (tmpB < 252))
                  {
                      //int_intensity = 0xFF;
                      //int_intensity = 0;
                      //b_color24 = makecol(int_intensity, int_intensity, int_intensity);
                      b_color24=makecol(255, 255, 255);
                  }
              }

          }
          else b_color24=makecol(255, 255, 255);
#endif

      rectfill(allegro_prn_bmp, x1, y1, x2, y2, b_color24);
	  
  }
 
  NULL_FOUND=FALSE;

  if (draw_to_matrix (window_to_print) == FALSE ||
      draw_matrix_to_file () == FALSE)
    {
      return FALSE ;
    }

  return TRUE ;
}

unsigned char *GetBMPpallete(void)
{
	return BMPpallete_;
}

void generateBitmapImageHeaders(int height, int width, char* imageFileName) {

	unsigned char padding[3] = { 0, 0, 0 };
	int paddingSize = (4 - (width*BYTES_PER_DOT) % 4) % 4;

	unsigned char* fileHeader = createBitmapFileHeader(height, width, paddingSize);
	unsigned char* infoHeader = createBitmapInfoHeader(height, width);
	static unsigned char* BMPpallete = createBMPpallete();

	BMPpallete_= BMPpallete;

	BMPimageFile = fopen(imageFileName, "wb");

	fwrite(fileHeader, 1, fileHeaderSize, BMPimageFile);
	fwrite(infoHeader, 1, infoHeaderSize, BMPimageFile);
	if (BITS_PER_DOT < 24)
	{
		fwrite(BMPpallete, 1, BMPpalleteSize, BMPimageFile);
	}
	
	BMPfilePos += (infoHeaderSize + BMPpalleteSize);
}


unsigned char* createBMPpallete(void)
{
	int i;
	static unsigned char BMPpallete[256 * 4];

	for (i = 0; i < 256; i++)
	{
		BMPpallete[i * 4] = (unsigned char)(i);
		BMPpallete[i * 4 + 1] = (unsigned char)(i);
		BMPpallete[i * 4 + 2] = (unsigned char)(i);
		BMPpallete[i * 4 + 3] = 0;
	};

	return BMPpallete;
}

unsigned char* createBitmapFileHeader(int height, int width, int paddingSize) {
	int fileSize = fileHeaderSize + infoHeaderSize + BMPpalleteSize + (BYTES_PER_DOT*width + paddingSize) * height;
	int a_offset;

	static unsigned char fileHeader[] = {
		0,0, /// signature
		0,0,0,0, /// image file size in bytes
		0,0,0,0, /// reserved
		0,0,0,0, /// start of pixel array
	};

	fileHeader[0] = (unsigned char)('B');
	fileHeader[1] = (unsigned char)('M');
	memcpy(&fileHeader[2], &fileSize, 4);
	a_offset = fileHeaderSize + infoHeaderSize + BMPpalleteSize;

	fileHeader[10] = (unsigned char)(a_offset);
	fileHeader[11] = (unsigned char)(a_offset >> 8);
	fileHeader[12] = (unsigned char)(a_offset >> 16);
	fileHeader[13] = (unsigned char)(a_offset >> 24);

	return fileHeader;
}


unsigned char* createBitmapInfoHeader(int height, int width) {
	static unsigned char infoHeader[] = {
		0,0,0,0, /// header size
		0,0,0,0, /// image width
		0,0,0,0, /// image height
		0,0, /// number of color planes
		0,0, /// bits per pixel
		0,0,0,0, /// compression
		0,0,0,0, /// image size
		0,0,0,0, /// horizontal resolution
		0,0,0,0, /// vertical resolution
		0,0,0,0, /// colors in color table
		0,0,0,0, /// important color count
	};
	short density;
	//int dpi_x, dpi_y;

	if (ptrs__prn_ini_date->reflection == 0)  height = -height;
	infoHeader[0] = (unsigned char)(infoHeaderSize);

#ifndef LINUX
	infoHeader[4] = (unsigned char)(width);
	infoHeader[5] = (unsigned char)(width >> 8);
	infoHeader[6] = (unsigned char)(width >> 16);
	infoHeader[7] = (unsigned char)(width >> 24);
#else
	memcpy(&infoHeader[4], &width, sizeof(int));
#endif

#ifndef LINUX
	infoHeader[8] = (unsigned char)(height);
	infoHeader[9] = (unsigned char)(height >> 8);
	infoHeader[10] = (unsigned char)(height >> 16);
	infoHeader[11] = (unsigned char)(height >> 24);
#else
	memcpy(&infoHeader[8], &height, sizeof(int));
#endif

	infoHeader[12] = (unsigned char)(1);
	infoHeader[14] = (unsigned char)(BYTES_PER_DOT * 8);
	
	density = int(matrix_head.horizontal_density*1000.0/25.4);  //dots per meter

	memcpy(&infoHeader[24],&density,2);
	memcpy(&infoHeader[28], &density, 2);

	return infoHeader;
}

void show_print_progress(double print_progress)
{
	char progress[32];

	sprintf(progress, "%.0f %%", print_progress*100.0);
	komunikat_str_short(progress, FALSE, FALSE);
	return;
}

int StartPDFDoc(char *doc_name)
{
	int ret;
	int i;

	pdf_error = 0;
	pdf_temp_error = 0;

	pdf_doc = HPDF_New(error_handler, NULL);
	if (!pdf_doc) {
		
		ret = ask_question(1, "", (char*)confirm, "", (char*)_CANNOT_CREATE_PDF_, 12, "", 11, 1, 62);
		return 0;
	}
	HPDF_UseUTFEncodings(pdf_doc);
	HPDF_SetCurrentEncoder(pdf_doc, "UTF-8");
	HPDF_SetCompressionMode(pdf_doc, HPDF_COMP_ALL);

    pdf_page_no = 0;

	//zeroing of embeded fonts
	for (i = 0; i < MaxNumberTextStyle; i++)
	{
		ttf_font_files.PDF_embeded[i] = FALSE;
	}

	return 1;
}

void draw_rect (HPDF_Page page, double x, double y)
{
    HPDF_Page_Rectangle(page, x, y, pdf_page_width-pdf_left_margin-pdf_right_margin, pdf_page_height-pdf_top_margin-pdf_bottom_margin);
}

void draw_sheet_rect (HPDF_Page page, double x1, double y1, double x2, double y2)
{
    HPDF_Page_Rectangle(page, x1, y1, x2-x1, y2-y1);
}

void SetPDFClip(int pdf_left, int pdf_bottom, int pdf_right, int pdf_top) {
    HPDF_Page_GRestore (pdf_page);

    int pdf_left_margin_, pdf_right_margin_, pdf_bottom_margin_, pdf_top_margin_;

    if (pdf_top<pdf_bottom)
        pdf_top=pdf_bottom;

    if (pdf_left<pdf_left_margin + extra_off_left)
        pdf_left=pdf_left_margin + extra_off_left;
    if (pdf_right>(pdf_page_width - pdf_right_margin + extra_off_right))
        pdf_right=pdf_page_width - pdf_right_margin + extra_off_right;
    if (pdf_bottom<pdf_bottom_margin + extra_off_bottom)
        pdf_bottom=pdf_bottom_margin + extra_off_bottom;
    if (pdf_top>(pdf_page_height - pdf_top_margin + extra_off_top))
        pdf_top=pdf_page_height - pdf_top_margin + extra_off_top;

    HPDF_Page_SetLineWidth(pdf_page, 0);
    HPDF_Page_SetRGBStroke(pdf_page, 1.0, 1.0, 1.0);
    HPDF_Page_SetLineCap(pdf_page, HPDF_BUTT_END);

    HPDF_Page_GSave(pdf_page);  /* Save the current graphic state */
    draw_sheet_rect(pdf_page, pdf_left, pdf_bottom, pdf_right, pdf_top);
    HPDF_Page_Clip(pdf_page);
    HPDF_Page_Stroke(pdf_page);
}


BOOL StartPDFPage(void)
{
	HPDF_STATUS hpdf_status;
	char png_file[64];
	HPDF_Image image[32];
	double iw;
	double ih;

	double pdf_x_insert = 0.0; // 3.25; // 2.54;
	double pdf_y_insert;
    double pdf_dy;

    switch (ptrs__prn_ini_date->density)
    {
        case 1:
            pdf_dy = - 1.0;
            break;
        case 2:
            pdf_dy = - 0.825;
            break;
        default:
            pdf_dy = - 0.185;
            break;
    }

    pdf_y_insert = pdf_dy / 25.4f * (float)matrix_head.horizontal_density; // 0.0; //3.25; // 2.54;

	pdf_page = HPDF_AddPage(pdf_doc);
	hpdf_status = HPDF_Page_SetWidth(pdf_page, (float)ptrs__prn_ini_date->width_paper / 25.4f * 72.0f);
    if (hpdf_status) return FALSE;
	hpdf_status = HPDF_Page_SetHeight(pdf_page, (float)ptrs__prn_ini_date->height_paper / 25.4f * 72.f);
    if (hpdf_status) return FALSE;
    hpdf_status = HPDF_Page_Concat(pdf_page, 72.0f / (float)matrix_head.vertical_density, 0, 0, 72.0f / (float)matrix_head.vertical_density, 0, 0);
    if (hpdf_status) return FALSE;
	pdf_left_margin = (int)(((float)ptrs__prn_ini_date->left_margin / 25.4f * (float)matrix_head.horizontal_density)+0.5);
	pdf_top_margin = (int)(((float)ptrs__prn_ini_date->top_margin / 25.4f * (float)matrix_head.vertical_density)+0.5);
    pdf_right_margin = (int)(((float)ptrs__prn_ini_date->right_margin / 25.4f * (float)matrix_head.horizontal_density)+0.5);
    pdf_bottom_margin = (int)(((float)ptrs__prn_ini_date->bottom_margin / 25.4f * (float)matrix_head.vertical_density)+0.5);
	pdf_page_width = ((float)ptrs__prn_ini_date->width_paper / 25.4f * (float)matrix_head.horizontal_density);
	pdf_page_height = ((float)ptrs__prn_ini_date->height_paper / 25.4f * (float)matrix_head.horizontal_density);

	float dx = (ptrs__prn_ini_date->width_paper / 25.4f) - ((int)(ptrs__prn_ini_date->width_paper / 25.4f * 100)) / 100.0;
	float dy = (ptrs__prn_ini_date->height_paper / 25.4f) - ((int)(ptrs__prn_ini_date->height_paper / 25.4f * 100)) / 100.0;
	//import PNG if necessary
#ifndef HPDF_NOPNGLIB
	if (ptrs__prn_ini_date->import_png == TRUE)
	{
		if (pdf_page_no == 0) sprintf(png_file, "Temp/pdfimg%03d_.png", Client_number);
		else sprintf(png_file, "Temp/pdfimg%03d_%03d.png", Client_number, pdf_page_no);

		if (my_file_exists(png_file))
		{
			image[pdf_page_no] = HPDF_LoadPngImageFromFile_(pdf_doc, png_file);
			if (image[pdf_page_no] == NULL) return FALSE;
			iw = HPDF_Image_GetWidth(image[pdf_page_no]);
			ih = HPDF_Image_GetHeight(image[pdf_page_no]);

            hpdf_status = HPDF_Page_DrawImage(pdf_page, image[pdf_page_no], pdf_x_insert, pdf_y_insert /*pdf_page_height*/, iw, ih);
            if (hpdf_status) return FALSE;
			////unlink(png_file);
		}
	}
#endif

    hpdf_status = HPDF_Page_SetLineWidth(pdf_page, 0);
    hpdf_status = HPDF_Page_SetRGBStroke(pdf_page, 1.0, 1.0, 1.0);
    hpdf_status = HPDF_Page_SetLineCap(pdf_page, HPDF_BUTT_END);

    hpdf_status = HPDF_Page_GSave (pdf_page);  /* Save the current graphic state */
    draw_rect (pdf_page, pdf_left_margin, pdf_bottom_margin);
    hpdf_status = HPDF_Page_Clip (pdf_page);
    hpdf_status = HPDF_Page_Stroke (pdf_page);

	pdf_page_no++;
	return TRUE;
}

void EndPDFPage(void)
{
    HPDF_Page_GRestore (pdf_page);
	return;
}

void EndPDFDoc(char *fname)
{
	//int ret;
	/* save the document to a file */
	HPDF_SaveToFile(pdf_doc, fname);
	/* clean up */
	HPDF_Free(pdf_doc);

	if (pdf_error>0)
		error_handler(pdf_error, 0, (char*)"");//  void *user_data)
	return;
}

void LinePDF(int x1, int y1, int x2, int y2, int width, COLOR_ color, HPDF_LineCap cap)
{
	HPDF_Page_SetLineWidth(pdf_page, width);
	HPDF_Page_SetRGBStroke(pdf_page, color.red/255.0, color.gre/255.0, color.blu/255.0);
	HPDF_Page_SetLineCap(pdf_page, cap); // HPDF_ROUND_END);
	HPDF_Page_MoveTo(pdf_page, x1, y1);
	HPDF_Page_LineTo(pdf_page, x2, y2);
	HPDF_Page_Stroke(pdf_page);
}

void CirclePDF(int x0, int y0, float r, int width, COLOR_ color)
{

    HPDF_Page_SetLineWidth(pdf_page, width);
    HPDF_Page_SetRGBStroke(pdf_page, color.red/255.0, color.gre/255.0, color.blu/255.0);

    HPDF_Page_Circle(pdf_page, x0, y0, r);
    HPDF_Page_Stroke(pdf_page);

}


void ArcPDF(int x0, int y0, int r, double a1, double a2, int width, COLOR_ color, HPDF_LineCap cap)
{


    HPDF_Page_SetLineWidth(pdf_page, width);
    HPDF_Page_SetRGBStroke(pdf_page, color.red/255.0, color.gre/255.0, color.blu/255.0);
    HPDF_Page_SetLineCap(pdf_page, cap); // HPDF_ROUND_END);

    HPDF_Page_Arc(pdf_page, x0, y0, r, a1, a2);
    HPDF_Page_Stroke(pdf_page);

}

void EllipsePDF(int x0, int y0, int rx, int ry, double angle, int width, COLOR_ color, HPDF_LineCap cap)
{


    HPDF_Page_SetLineWidth(pdf_page, width);
    HPDF_Page_SetRGBStroke(pdf_page, color.red/255.0, color.gre/255.0, color.blu/255.0);
    HPDF_Page_SetLineCap(pdf_page, cap); // HPDF_ROUND_END);

    HPDF_Page_SetRGBFill(pdf_page, color.red/255.0, color.gre/255.0, color.blu/255.0);


    HPDF_Page_Ellipse(pdf_page, x0, y0, rx, ry);
    HPDF_Page_Stroke(pdf_page);

}

void FilledEllipsePDF(int x0, int y0, int rx, int ry, double angle, COLOR_ color, HPDF_LineCap cap, unsigned char translucency)
{

    HPDF_Page_GSave (pdf_page);  // Save the current graphic state ////

    HPDF_ExtGState gstate{ HPDF_CreateExtGState(pdf_doc) };  ////
    HPDF_ExtGState_SetAlphaStroke(gstate, 0.0 /*((float)translucency)/255.0*/); // Set the opacity  ////

    HPDF_Page_SetLineWidth(pdf_page, 0);
    HPDF_Page_SetRGBStroke(pdf_page, color.red/255.0, color.gre/255.0, color.blu/255.0);
    HPDF_Page_SetLineCap(pdf_page, cap); // HPDF_ROUND_END);

    HPDF_Page_SetRGBFill(pdf_page, color.red/255.0, color.gre/255.0, color.blu/255.0);
    ////HPDF_ExtGState gstate{ HPDF_CreateExtGState(pdf_doc) };  ////
    ////HPDF_ExtGState_SetAlphaStroke(gstate, 0.0 /*((float)translucency)/255.0*/); // Set the opacity  ////
    HPDF_ExtGState_SetAlphaFill(gstate, ((float)translucency)/255.0); // Set the opacity  ////
    HPDF_Page_SetExtGState(pdf_page, gstate); // Apply the gstate property to the page  ////

    HPDF_Page_Ellipse(pdf_page, x0, y0, rx, ry);
    HPDF_Page_FillStroke(pdf_page);

    HPDF_Page_GRestore (pdf_page);  ////
}

///////////////////////////////////
void FilledSolidPDF(int numpoints, int *xy, COLOR_ color, HPDF_LineCap cap, unsigned char translucency)
{

    HPDF_STATUS hpdf_status;

    HPDF_Page_GSave (pdf_page);  // Save the current graphic state ////

    HPDF_ExtGState gstate{ HPDF_CreateExtGState(pdf_doc) };  ////
    HPDF_ExtGState_SetAlphaStroke(gstate, 0.0 /*((float)translucency)/255.0*/); // Set the opacity  ////

    HPDF_Page_SetLineWidth(pdf_page, 0);
    HPDF_Page_SetRGBStroke(pdf_page, color.red/255.0, color.gre/255.0, color.blu/255.0);
    HPDF_Page_SetLineCap(pdf_page, cap); // HPDF_ROUND_END);

    HPDF_Page_SetRGBFill(pdf_page, color.red/255.0, color.gre/255.0, color.blu/255.0);
    ////HPDF_ExtGState gstate{ HPDF_CreateExtGState(pdf_doc) };  ////
    HPDF_ExtGState_SetAlphaFill(gstate, ((float)translucency)/255.0); // Set the opacity  ////
    hpdf_status = HPDF_Page_SetExtGState(pdf_page, gstate); // Apply the gstate property to the page  ////

    HPDF_Page_MoveTo(pdf_page, xy[0], xy[1]);
    for (int i = 2; i < numpoints; i+=2) {
        HPDF_Page_LineTo(pdf_page, xy[i], xy[i + 1]);
    }

    HPDF_Page_ClosePathFillStroke(pdf_page);

    HPDF_Page_GRestore (pdf_page);  ////
}



BOOL Draw_Pcx_To_PDF(B_PCX* adr_pcx)
{
	HPDF_Image image;
	float iw, ih;
	double pdf_x_insert, pdf_y_insert, d_pdf_y_insert, pdf_x_insert_r, pdf_y_insert_r;
	int isize;

	PCXheader * head;
	int width, height;
	float bwidth, bheight;
	char* png_data;
	long png_mem;
	float fWidth, fHeight;
	float x01, y01;
	double kat;
	BITMAP* png1;
	PALETTE pal;
	int i, j;

	T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_;
	BOOL bw, grey;
	int bits_per_component;


	ptrs__prn_ini_date_ = get_ptrs__prn_ini_date();


	bwidth = (adr_pcx->dx * adr_pcx->i_x);
	bheight = (adr_pcx->dy * adr_pcx->i_y);

	x01 = adr_pcx->x;
	y01 = adr_pcx->y;

	kat = adr_pcx->kat;

	if (ptrs__prn_ini_date->rotation == TRUE)
	{
		kat -= PI2;
	}
	if (ptrs__prn_ini_date->inversion == TRUE)
	{
		kat -= PI; //TO DO
	}

	if (print_inversion == TRUE)
	{
		if (print_rotation == TRUE)
		{
			//// if reflection
			x01 = FormatX_S - x01;
			y01 = FormatY_S - y01;
		}
		else
		{
			x01 = FormatX_S - x01 +FormatX_S0;  //TO CHECK, before was without + FormatX_S0
			y01 = FormatY_S + FormatY_S0 - y01;
		}
	}
	else if (print_rotation == TRUE)
	{
		//x01 = y01;
		//y01 = x01;
        ;
	}

	iw = adr_pcx->dx * adr_pcx->i_x;
	ih = adr_pcx->dy * adr_pcx->i_y;
	isize = adr_pcx->i_x * adr_pcx->i_y * 4;

	pdf_x_insert = jednostki_to_prn_x(x01, y01);
	pdf_y_insert = jednostki_to_prn_y(x01, y01);

	d_pdf_y_insert=jednostki_y_to_prn(bheight);

	pdf_x_insert += pdf_left_margin;
	pdf_y_insert = pdf_page_height - pdf_y_insert - pdf_top_margin;

	head = (PCXheader*)adr_pcx->pcx;
	width = (head->xmax - head->xmin);
	height = (head->ymax - head->ymin);

	png_data = adr_pcx->pcx + sizeof(PCXheader);
	png_mem = width * height * 4 + 100;  // 4 BYTES per pixel , 100 is reserve


	if (adr_pcx->kod_obiektu == 1) png1 = load_memory_pcx(adr_pcx->pcx, &pal);
	else if (adr_pcx->kod_obiektu == 2) png1 = load_memory_png(png_data, png_mem, (RGB*)&pal);
	else if (adr_pcx->kod_obiektu == 3) png1 = load_memory_jpg(png_data, png_mem, (RGB*)&pal);

	int color_space= HPDF_CS_DEVICE_RGB;


	bw = Layers[adr_pcx->warstwa].bw;
	grey = Layers[adr_pcx->warstwa].grey;

	if ((*ptrs__prn_ini_date_)->PCX_gray == TRUE)
	{
		grey = TRUE;
		bw = FALSE;
	}
	else if (((*ptrs__prn_ini_date_)->color_print == 1) && (bw == 0))
	{
		if (((*ptrs__prn_ini_date_)->gray_print == 1) || (grey == 1) || (*ptrs__prn_ini_date_)->PCX_gray == 1)
		{
			grey = TRUE;
			bw = FALSE;
		}
	}
	else
	{
		bw = TRUE;
		grey = FALSE;
	}

	if (head->bits_per_pixel == 32)
	{
		if ((grey) || (bw))
		{
			make_grey32(png1, ptrs__prn_ini_date_, grey, bw);
		}
		bits_per_component = 8;
	}
	else if (head->bits_per_pixel == 24)
	{
		if ((grey) || (bw))
		{
			make_grey24(png1, ptrs__prn_ini_date_, grey, bw);
		}
		bits_per_component = 8;
	}
	else if (head->bits_per_pixel == 8)
	{
		if ((grey) || (bw))
		{
			make_grey8(png1, ptrs__prn_ini_date_, grey, bw);
		}
		bits_per_component = 8;
	}

	unsigned char* img_buffer, * rgb_buffer, * alpha_buffer;

	img_buffer = *png1->line;

	rgb_buffer = (unsigned char*)malloc(3 * width * height);
	alpha_buffer = (unsigned char*)malloc(width * height);
	if (rgb_buffer == NULL || alpha_buffer == NULL) {
		free(rgb_buffer);
		free(alpha_buffer);
		return FALSE;
	}
	
	if (head->bits_per_pixel == 8)
		image = HPDF_LoadRawImageFromMem((HPDF_Doc)pdf_doc, img_buffer, width, height, HPDF_CS_INDEXED, 8);

	if (head->bits_per_pixel == 24)
	   image = HPDF_LoadRawImageFromMem((HPDF_Doc) pdf_doc, img_buffer, width, height, HPDF_CS_DEVICE_RGB, 8);

	else if (head->bits_per_pixel == 32) //with alpha channel
	{

		for (i = 0; i < height; i++)
			for (j = 0; j < width; j++) {
				rgb_buffer[((i * width) + j) * 3] =
					img_buffer[((i * width) + j) * 4 + 2];  //+0 corrected Linux, TO CHECK on W&M

				rgb_buffer[(((i * width) + j) * 3) + 1] =
					img_buffer[((i * width) + j) * 4 + 1];

				rgb_buffer[(((i * width) + j) * 3) + 2] =
					img_buffer[((i * width) + j) * 4 + 0];  //+2 corrected  Linux, TO CHECK on W&M

				alpha_buffer[(i * width) + j] =
					img_buffer[((i * width) + j) * 4 + 3];
			}

		HPDF_Image smask = HPDF_LoadRawImageFromMem(pdf_doc, alpha_buffer, width, height, HPDF_CS_DEVICE_GRAY, 8);

		image = HPDF_LoadRawImageFromMem(pdf_doc, rgb_buffer, width, height, HPDF_CS_DEVICE_RGB, 8);

		if (HPDF_Image_AddSMask(image, smask) != HPDF_OK)
			image = NULL;

		free(rgb_buffer);
		free(alpha_buffer);

	}

	else return FALSE;

	if (image == NULL)
	{
		destroy_bitmap(png1);
		return FALSE;
	}

    fWidth = jednostki_x_to_prn(bwidth);
	fHeight = jednostki_y_to_prn(bheight);

	pdf_x_insert_r = pdf_x_insert;
	pdf_y_insert_r = pdf_y_insert - d_pdf_y_insert;


	if (adr_pcx->h_flip)
	{
		fWidth *= -1;
		pdf_x_insert_r -= fWidth;
	}

	if (adr_pcx->v_flip)
	{
		fHeight *= -1;
		pdf_y_insert_r -= fHeight;
	}

	if (Check_if_Equal(kat, 0.0) == FALSE)
	{

		double si = sin(kat);
		double co = cos(kat);

		Rotate_Point(si, co, pdf_x_insert, pdf_y_insert, pdf_x_insert_r, pdf_y_insert_r, &pdf_x_insert_r, &pdf_y_insert_r);

		HPDF_Page_GSave(pdf_page);

		HPDF_Page_Concat(pdf_page, fWidth * cos(kat),
			fWidth * sin(kat),
			fHeight * -sin(kat),
			fHeight * cos(kat),
			pdf_x_insert_r, pdf_y_insert_r);

		HPDF_Page_ExecuteXObject(pdf_page, image);
		HPDF_Page_GRestore(pdf_page);

	}
	else
	{
		HPDF_Page_DrawImage(pdf_page, image, pdf_x_insert_r, pdf_y_insert_r, fWidth, fHeight);
	}

	destroy_bitmap(png1);
	return TRUE;
}



void TextPDF(TEXT *t, char *zn, int x1, int y1, float font_scale, float angle, COLOR_ color, BOOL index_pos)
{
	HPDF_Font font;
	float rad1;
	float rad2;
	int height;
	char zn_b[MaxMultitextLen];
	char *ptrn;
	char *ptrr;
    char *ptr174;
	char *ptr176;
	char *ptr136;
	double t_width_factor;
	double t_wysokosc;

    /* bold text. */
	if (t->bold)
	{
		//What can be done is just increasing 10% the height
		t_wysokosc = t->wysokosc*1.08225;
		t_width_factor = t->width_factor*1.0145;
	}
	else
	{
		
		t_wysokosc = t->wysokosc;
		t_width_factor = t->width_factor;
	}

	height = jednostki_y_to_prn(t_wysokosc * font_scale * PDF_TTF_height_factor);

	//checking font
	if (ttf_font_files.PDF_embeded[t->czcionka] == FALSE)
	{
		ttf_font_files.pdf_font_name[t->czcionka] = HPDF_LoadTTFontFromFile(pdf_doc, ttf_font_files.file[t->czcionka], HPDF_TRUE);
		ttf_font_files.PDF_embeded[t->czcionka] = TRUE;

		if (ttf_font_files.pdf_font_name[t->czcionka] == NULL)
		{
			pdf_error = pdf_temp_error;
			pdf_temp_error = 0;
			return;
		}
	}
	else if (ttf_font_files.pdf_font_name[t->czcionka] == NULL) return;


	if (height > HPDF_MAX_FONTSIZE)
	{
		pdf_error=0x1050;
		pdf_temp_error = 0;
		return;
	}


	//preparing font
	/* create default-font */
	font = HPDF_GetFont(pdf_doc, ttf_font_files.pdf_font_name[t->czcionka], "UTF-8");
	if (pdf_temp_error > 0)
	{
		pdf_error = 0x1050;
		pdf_temp_error = 0;
		return;
	}
	HPDF_Page_SetFontAndSize(pdf_page, font, height);
	if (pdf_temp_error > 0)
	{
		pdf_error = 0x1050;
		pdf_temp_error = 0;
		return;
	}
	HPDF_Page_SetTextRenderingMode(pdf_page, HPDF_FILL);
	HPDF_Page_SetRGBFill(pdf_page, color.red / 255.0, color.gre / 255.0, color.blu / 255.0);
	HPDF_Page_BeginText(pdf_page);

	/* scaling text (X direction)*/
	//if (t->width_factor!=0.0)
	
	rad1 = 0.0;
	rad2 = 0.0;
	/* Skewing text. */
	if (t->italics)  rad2 = angle_italic / 180.0 * 3.141592;

	HPDF_Page_SetTextMatrix(pdf_page, t_width_factor*PDF_TTF_width_factor*cos(angle), t_width_factor*PDF_TTF_width_factor*sin(angle), -sin(angle-rad2), cos(angle-rad2), x1, y1);

    if (index_pos) ptr174 = strchr(zn, '\174');
    else ptr174=NULL;
    ptrn = strchr(zn, '\n');
	ptrr = strchr(zn, '\r');
	ptr176 = strchr(zn, '\176');
	ptr136 = strchr(zn, '\136');
	if (ptrn || ptrr || ptr176 || ptr136 || ptr174)
	{
		strcpy(zn_b, zn);
		ptrn = strchr(zn_b, '\n');
		ptrr = strchr(zn_b, '\r');
		ptr176 = strchr(zn_b, '\176');
		ptr136 = strchr(zn_b, '\136');
        if (index_pos) ptr174 = strchr(zn_b, '\174');
        else ptr174=NULL;
		if (ptrn != NULL) *ptrn = '\0';
		if (ptrr != NULL) *ptrr = '\0';
		if (ptr176 != NULL) *ptr176 = '\0';
		if (ptr136 != NULL) *ptr136 = '\0';
        if (ptr174 != NULL) *ptr174 = '\0';

		if (strlen(zn_b)>0) HPDF_Page_ShowText(pdf_page, zn_b);
	}
	else
	{
		HPDF_Page_ShowText(pdf_page, zn);
	}

	HPDF_Page_EndText(pdf_page);

	return;
}

void RectPDF(int x1, int y1, int x2, int y2, COLOR_ color)
{
	int xx1, yy1;
	HPDF_Page_SetRGBFill(pdf_page, color.red / 255.0, color.gre / 255.0, color.blu / 255.0);
	if (x2 < x1) { xx1 = x1; x1 = x2; x2 = xx1; }
	if (y2 < y1) { yy1 = y1; y1 = y2; y2 = yy1; }
	HPDF_Page_Rectangle(pdf_page, x1, y1, x2 - x1, y2 - y1);
}

static BOOL draw_sheet_prn_nrot (void)  /*bez obrotu*/
/*----------------------------------*/
{
  double xmin, ymin, xmax, ymax; /*okno w jednostkach rysunku*/
  BOOL b_break_x, b_break_y ;
  BOOL b_new_page ;
  int i_pageno ;        /*numer strony*/
  double df_height, df_height_max ;    /*narysowana wysokosc, wysokosc strony*/
  double df_margin ;                   /*margines w jednostkach rysunku*/
  double left_margin, right_margin, top_margin, bottom_margin;
  double df_y0 ;                       /*poczatek strony ;*/
  int k,kk;
  BOOL ret;
  int width, height, top;
  double xp, xk, yp, yk;
  char fnd[MAXPATH]="";
  char *fnd_ptr;
  char params[128];
  DWORD runcode;
  char error_str[64];
  double print_progress;
  double next_print_progress=0.01;
  double total_area, area;
  int i;

  /* tutaj chwilowo przedefiniowano wartosci xp, yp, xk, yk */

  /*-------------*/
  if (strlen(zbior_danych_2)>0)
  {
   k=pisz_zbior("RYSUJ.$2", FALSE, 1) ;
   if ( k )  return FALSE;
  }

  i_pageno = 0 ;
  df_height = 0 ;
  b_new_page = FALSE ;
  df_margin = plt_d_to_jednostki_d(ptrs__prn_ini_date->margin) / matrix_head.condensed_wsp; 
  df_height_max = plt_d_to_jednostki_d(ptrs__prn_ini_date->height_paper) / matrix_head.condensed_wsp; 
  

  left_margin = plt_d_to_jednostki_d(ptrs__prn_ini_date->left_margin) / matrix_head.condensed_wsp;
  top_margin = plt_d_to_jednostki_d(ptrs__prn_ini_date->top_margin) / matrix_head.condensed_wsp;
  right_margin = plt_d_to_jednostki_d(ptrs__prn_ini_date->right_margin) / matrix_head.condensed_wsp;
  bottom_margin = plt_d_to_jednostki_d(ptrs__prn_ini_date->bottom_margin) / matrix_head.condensed_wsp;

  df_height_max -= (top_margin + bottom_margin);

  xmin = ptrs__prn_ini_date->xp ;

  top = abs(jednostki_h_to_byte_prn(ptrs__prn_ini_date->top_margin) * 8);

  if ((ptrs__prn_ini_date->prn_type==PRN_PCX)
      #ifdef LINUX
      || (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)
      #endif
            )
	{

	   sprintf(imageFileName, "%s%03d.bmp",bitmap_Image, Client_number);

	   xp = ptrs__prn_ini_date->xp;
	   xk = ptrs__prn_ini_date->xk;
	   yp = ptrs__prn_ini_date->yp;
	   yk = ptrs__prn_ini_date->yk;


	   width = abs(jednostki_w_to_byte_prn(ptrs__prn_ini_date->width_paper) * 8);
	   height = abs(jednostki_h_to_byte_prn(ptrs__prn_ini_date->height_paper) * 8);

	   generateBitmapImageHeaders(height, width, imageFileName);

	   ret = draw_top_margin_PCX(width, top);

	   BMP_rows = top;

     }
#ifndef LINUX
  else if (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)
  {
      kk=StartWinPrintDoc(zbior_danych);
	  StartWinPage();

	  vertical_i=top;

	  //vertical_i += draw_top_margin_WINDOWS();
  }
#endif
  else if (ptrs__prn_ini_date->prn_type == PRN_PDF)
  {
	  kk = StartPDFDoc(zbior_danych);
	  if (!StartPDFPage()) return FALSE;

	  vertical_i = top;

	  //reseting ttf files flags
	  for (i = 0; i<MaxNumberTextStyle; i++)
	  {
		  ttf_font_files.PDF_embeded[i] = FALSE;
	  }
  }
  else  //inne drukarki DOSOWE
   {
    draw_file_begin () ;

    if (FALSE == draw_top_margin ())
     {
       return FALSE ;
     }
   }

  total_area = fabs((ptrs__prn_ini_date->xk - ptrs__prn_ini_date->xp) * (ptrs__prn_ini_date->yk - ptrs__prn_ini_date->yp));
  area = 0.0;

  while (1)
  {
    b_break_x = FALSE ;
    xmax = xmin + matrix_head.width_max - left_margin - right_margin;
    if (xmax >= ptrs__prn_ini_date->xk)
    {
      b_break_x = TRUE ;
      xmax = ptrs__prn_ini_date->xk ;
    }

    if (ptrs__prn_ini_date->inversion == TRUE)
     {
      ymin = ptrs__prn_ini_date->yp ;
      df_y0 = ymin;
     }
       else
        {
         ymax = ptrs__prn_ini_date->yk ;
         df_y0 = ymax ;
        }
    while (1)
	{
      b_break_y = FALSE ;

      if (ptrs__prn_ini_date->inversion == TRUE)
       {
        ymax = ymin + matrix_head.height_max;
       }
         else ymin = ymax - matrix_head.height_max ;

      df_height += ymax - ymin ;

      if (TRUE == Check_if_GE (df_height, df_height_max ))
      {
	     b_new_page = TRUE ;
        if (ptrs__prn_ini_date->inversion == TRUE)
         ymax = df_y0 + df_height_max;
          else ymin = df_y0 - df_height_max ;  //?????????????
      }
      if (ptrs__prn_ini_date->inversion == TRUE)
       {
        if (ymax >= ptrs__prn_ini_date->yk )
        {
	       ymax = ptrs__prn_ini_date->yk ;
	       b_new_page = TRUE ;
          b_break_y = TRUE ;
        }
       }
       else
       {
        if (ymin <= ptrs__prn_ini_date->yp )
        {
	       ymin = ptrs__prn_ini_date->yp ;
	       b_new_page = TRUE ;
          b_break_y = TRUE ;
        }
       }
      if (i_pageno >= i__page_min)
      {

          if (FALSE == Draw_Window(xmin, ymin, xmax, ymax, //// matrix_head.width_max, ymax - ymin)) return FALSE;
                                   matrix_head.width, ymax - ymin, left_margin))
              return FALSE;

		if (total_area!=0.0)
		{
			
			area += fabs((xmax-xmin) * (ymax-ymin));
			if (total_area > 0)
			{
				print_progress = (area / total_area) + 0.001;
				if (print_progress > 1.0) print_progress = 1.0;
				if (print_progress >= next_print_progress)
				{
					show_print_progress(print_progress);
					next_print_progress = print_progress + 0.01;
				}
			}
		}

		
	    if (b_new_page == TRUE && !(b_break_x == TRUE &&  b_break_y == TRUE))
	    {
	      b_new_page = FALSE ;
          df_height = 0 ;

#ifndef LINUX
		  if (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)
		  {
			EndWinPage();
			StartWinPage();

			vertical_i=top;

			//vertical_i += draw_top_margin_WINDOWS();
		  }
		  else
#endif
              if (ptrs__prn_ini_date->prn_type == PRN_PDF)
		  {
			  EndPDFPage();
			  StartPDFPage();

			  vertical_i = top;

		  }
		  else if ((ptrs__prn_ini_date->prn_type == PRN_PCX)
               #ifdef LINUX
                || (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)  //just for LINUX
               #endif
                  )
		  {

			  correct_prn_bitmap_colors(allegro_prn_bmp);

			  if (BMP_rows < height)
			  {
				  ret = finish_BMP(width, height - BMP_rows);
			  }

			  fclose(BMPimageFile);

              if (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)  //just for LINUX
              {
#ifdef LINUX
                  sprintf(imageFileNamePdf, "%s%03d.pdf",bitmap_Image, Client_number);
                  sprintf(params, "\"%s\" %s %d", imageFileName, imageFileNamePdf, matrix_head.vertical_density);
                  runcode = RunSilent("image2image.exe", params);

                  if (runcode > 0) {
                      sprintf(error_str, "%s", _FILE_WRITING_ERROR_);
                      ErrListStr(error_str);
                  }

                  sprintf(params, "-d %s -o Resolution=%ddpi", ptrs__prn_ini_date->printer, matrix_head.vertical_density); //ptrs__prn_ini_date->density);
                  runcode = SystemSilent("lpoptions",params);

                  sprintf(params, "-c -d %s %s", ptrs__prn_ini_date->printer, imageFileNamePdf); //imageFileName);
                  runcode = SystemSilent("lp",params);
                  if (runcode > 0) {
                      sprintf(error_str, "%s%d", err_message_cups, runcode);
                      ErrListStr(error_str);
                  }
#endif
              }
              else {
                  if (strcmp(ptrs__prn_ini_date->image_format, "BMP") == 0) {
                      ret = get_dest_image_file_name(ptrs__prn_ini_date->plik, fnd, i_pageno,
                                                     ptrs__prn_ini_date->image_format);
                      if (FALSE != Copy_File(fnd, imageFileName)) {
                          ErrListStr((char *) _FILE_WRITING_ERROR_);
                      }
                  } else {
                      ret = get_dest_image_file_name(ptrs__prn_ini_date->plik, fnd, i_pageno,
                                                     ptrs__prn_ini_date->image_format);
                      sprintf(params, "\"%s\" %s %d", imageFileName, fnd, matrix_head.vertical_density);
                      runcode = RunSilent("image2image.exe", params);
                      if (runcode > 0) {
                          sprintf(error_str, "%s%d", err_message, runcode);
                          ErrListStr(error_str);
                      }
                  }
              }

			  generateBitmapImageHeaders(height, width, imageFileName);
			  ret = draw_top_margin_PCX(width, top);

			  BMP_rows = top;

		  }
		  else
		  {
			 if (draw_next_page () == FALSE) return FALSE ;  //inne drukarki
		  }

		  i_pageno++;

          ymin += df_margin ;

            if (ptrs__prn_ini_date->inversion == TRUE)
            {
                df_y0 = ymax;
            }
           else df_y0 = ymin ;
        }
      }
      else {
          if (b_new_page == TRUE && !(b_break_x == TRUE &&  b_break_y == TRUE)) {
              b_new_page = FALSE;
              df_height = 0;
              i_pageno++;
              ymin += df_margin;

              if (ptrs__prn_ini_date->inversion == TRUE)
              {
                  df_y0 = ymax;
              }
              else
                  df_y0 = ymin;
          }
      }

      if (b_break_y == TRUE)
      {
	     break ;
      }
      if (ptrs__prn_ini_date->inversion == TRUE)
        ymin = ymax ;
         else ymax = ymin ;
    }
    if (b_break_x == TRUE)
    {
      break ;
    }
	xmin = xmax - df_margin;
  }

  if ((ptrs__prn_ini_date->prn_type == PRN_PCX)
#ifdef LINUX
    || (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)  //just for LINUX
#endif
    )
  {
	  correct_prn_bitmap_colors(allegro_prn_bmp);

	  if (BMP_rows < height)
	  {
		  ret = finish_BMP(width, height - BMP_rows);
	  }
	 
	  fclose(BMPimageFile);

      if (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)  //just for LINUX
      {
#ifdef LINUX
          sprintf(imageFileNamePdf, "%s%03d.pdf",bitmap_Image, Client_number);
          sprintf(params, "\"%s\" %s %d", imageFileName, imageFileNamePdf, matrix_head.vertical_density);
          runcode = RunSilent("image2image.exe", params);

          if (runcode > 0) {

              sprintf(error_str, "%s", _FILE_WRITING_ERROR_);

              ErrListStr(error_str);
          }

          sprintf(params, "-d %s -o Resolution=%ddpi", ptrs__prn_ini_date->printer, matrix_head.vertical_density); //ptrs__prn_ini_date->density);
          runcode = SystemSilent("lpoptions",params);

          sprintf(params, "-c -d %s %s", ptrs__prn_ini_date->printer, imageFileNamePdf); //imageFileName);
          runcode = SystemSilent("lp",params);
          if (runcode > 0) {
              sprintf(error_str, "%s%d", err_message_cups, runcode);

              ErrListStr(error_str);
          }
#endif
      }
      else {
          if (strcmp(ptrs__prn_ini_date->image_format, "BMP") == 0) {
              ret = get_dest_image_file_name(ptrs__prn_ini_date->plik, fnd, i_pageno, ptrs__prn_ini_date->image_format);
              fnd_ptr = fnd;

              if (FALSE == Copy_File(fnd_ptr, imageFileName)) {
                  ErrListStr((char *) _FILE_WRITING_ERROR_);
              }
          } else {
              ret = get_dest_image_file_name(ptrs__prn_ini_date->plik, fnd, i_pageno, ptrs__prn_ini_date->image_format);

              sprintf(params, "\"%s\" %s %d", imageFileName, fnd, matrix_head.vertical_density);
              runcode = RunSilent("image2image.exe", params);
              if (runcode > 0) {
                  sprintf(error_str, "%s%d", err_message, runcode);
                  ret = ask_question(1, "", confirm, "", error_str, 12, "", 11, 1, 62);

                  CUR_ON(X, Y);
              }
          }
      }

	  ret = Delete_File(imageFileName);
  }
    else if (ptrs__prn_ini_date->prn_type==PRN_WINDOWS) 
	{
		EndWinPage();
		EndWinDoc();
	}
	else if (ptrs__prn_ini_date->prn_type == PRN_PDF)
	  {
		  EndPDFPage();

		 
		  EndPDFDoc(ptrs__prn_ini_date->plik);
		  
	  }
		else draw_file_end () ;
   
  return TRUE ;
}


static BOOL draw_sheet_prn_rot (void)    /* z obrotem */
/*----------------------------------*/
{
  double xmin, ymin, xmax, ymax; /*okno w jednostkach rysunku*/
  BOOL b_break_x, b_break_y ;
  BOOL b_new_page ;
  int i_pageno ;        /*numer strony*/
  double df_height, df_height_max ;    /*narysowana wysokosc, wysokosc strony*/
  double df_margin ;                   /*margines w jednostkach rysunku*/
  double left_margin, right_margin, top_margin, bottom_margin;
  double df_x0 ;                       /*poczatek strony ;*/
  int k,kk;
  BOOL ret;
  int width, height, top;
  double xp, xk, yp, yk;
  char fnd[MAXPATH];
  char params[128];
  DWORD runcode;
  char error_str[64];
  double print_progress;
  double total_area, area;

  if (strlen(zbior_danych_2)>0)
  {
   k=pisz_zbior("RYSUJ.$2", FALSE, 1) ;
   if ( k )  return FALSE;
  }
  i_pageno = 0 ;
  df_height = 0 ;
  b_new_page = FALSE ;
  df_margin = plt_d_to_jednostki_d(ptrs__prn_ini_date->margin) / matrix_head.condensed_wsp;
  df_height_max = plt_d_to_jednostki_d(ptrs__prn_ini_date->height_paper) / matrix_head.condensed_wsp;
  ymin = ptrs__prn_ini_date->yp ;

  left_margin = plt_d_to_jednostki_d(ptrs__prn_ini_date->left_margin) / matrix_head.condensed_wsp;
  top_margin = plt_d_to_jednostki_d(ptrs__prn_ini_date->top_margin) / matrix_head.condensed_wsp;
  right_margin = plt_d_to_jednostki_d(ptrs__prn_ini_date->right_margin) / matrix_head.condensed_wsp;
  bottom_margin = plt_d_to_jednostki_d(ptrs__prn_ini_date->bottom_margin) / matrix_head.condensed_wsp;

  top = abs(jednostki_h_to_byte_prn(ptrs__prn_ini_date->top_margin) * 8);

  if ((ptrs__prn_ini_date->prn_type==PRN_PCX)
      #ifdef LINUX
      || (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)
#endif
            )
	{

	  sprintf(imageFileName, "%s%03d.bmp", bitmap_Image, Client_number);

	  xp = ptrs__prn_ini_date->xp;
	  xk = ptrs__prn_ini_date->xk;
	  yp = ptrs__prn_ini_date->yp;
	  yk = ptrs__prn_ini_date->yk;

	  
	  width = abs(jednostki_w_to_byte_prn(ptrs__prn_ini_date->width_paper) * 8);
	  height = abs(jednostki_h_to_byte_prn(ptrs__prn_ini_date->height_paper) * 8);

	  generateBitmapImageHeaders(height, width, imageFileName);

	  ret = draw_top_margin_PCX(width, top);

	  BMP_rows = top;

     }
#ifndef LINUX
  else if (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)
  {
      kk=StartWinPrintDoc(zbior_danych);
	  StartWinPage();
	  vertical_i=top;

	  //vertical_i += draw_top_margin_WINDOWS();
  }
#endif
  else if (ptrs__prn_ini_date->prn_type == PRN_PDF)
  {
	  kk = StartPDFDoc(zbior_danych);
	  StartPDFPage();
	  vertical_i = top;

  }
  else
   {
  
     draw_file_begin () ;

     if (FALSE == draw_top_margin ())
      {
        return FALSE ;
      }
   }

  total_area = fabs((ptrs__prn_ini_date->xk - ptrs__prn_ini_date->xp) * (ptrs__prn_ini_date->yk - ptrs__prn_ini_date->yp));
  area = 0.0;

  while (1)
  {
    b_break_y = FALSE ;
    ymax = ymin + matrix_head.width_max - left_margin - right_margin;
    if (ymax >= ptrs__prn_ini_date->yk)
    {
      b_break_y = TRUE ;
      ymax = ptrs__prn_ini_date->yk ;
    }

    if (ptrs__prn_ini_date->inversion == TRUE)
     {
      xmin = ptrs__prn_ini_date->xp ;
      df_x0 = xmin;
     }
      else
       {
        xmin = ptrs__prn_ini_date->xp ;
        df_x0 = xmin ;
       }
    while (1)
    {
      b_break_x = FALSE ;

     if (ptrs__prn_ini_date->inversion == TRUE)
      {
       xmax = xmin + matrix_head.height_max;  
      }
        else xmax = xmin + matrix_head.height_max;

      df_height += xmax - xmin ;

		if (TRUE == Check_if_GE (df_height, df_height_max - top_margin - bottom_margin))
		  {
			 b_new_page = TRUE ;

		   if (ptrs__prn_ini_date->inversion == TRUE)
			xmax = df_x0 + df_height_max - top_margin - bottom_margin;
			 else xmax = df_x0 + df_height_max - top_margin - bottom_margin;
		  }

     if (ptrs__prn_ini_date->inversion == TRUE)
      {
       if (xmax >= ptrs__prn_ini_date->xk)
       {
	     xmax = ptrs__prn_ini_date->xk ;
	     b_new_page = TRUE ;
	     b_break_x = TRUE ;
       }
      }
      else
      {
      if (xmax >= ptrs__prn_ini_date->xk)
       {
	     xmax = ptrs__prn_ini_date->xk ;
	     b_new_page = TRUE ;
	     b_break_x = TRUE ;
       }
      }
      if (i_pageno >= i__page_min)
      {
		
		  if (FALSE == Draw_Window(xmin, ymin, xmax, ymax, ////matrix_head.width_max, xmax - xmin)) return FALSE;
			  matrix_head.width, xmax - xmin, left_margin)) return FALSE ;
		  

		  if (total_area != 0.0)
		  {

			  area += fabs((xmax - xmin) * (ymax - ymin));
			  if (total_area > 0)
			  {
				  print_progress = area / total_area;
				  if (print_progress > 1.0) print_progress = 1.0;
				  show_print_progress(print_progress);
			  }
		  }
		
	   if (b_new_page == TRUE && !(b_break_x == TRUE &&  b_break_y == TRUE))
       {
	    b_new_page = FALSE ;
	    df_height = 0 ;
#ifndef LINUX
	    if (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)
		{
			EndWinPage();
			StartWinPage();
			vertical_i=top;

			//vertical_i += draw_top_margin_WINDOWS();
		}
#endif
		if (ptrs__prn_ini_date->prn_type == PRN_PDF)
		{
			EndPDFPage();
			StartPDFPage();
			vertical_i = top;

		}
	    else if ((ptrs__prn_ini_date->prn_type == PRN_PCX)
#ifdef LINUX
           || (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)  //just for LINUX
#endif
           )
	    {
		  correct_prn_bitmap_colors(allegro_prn_bmp);

		  if (BMP_rows < height)
		  {
			  ret = finish_BMP(width, height - BMP_rows);
		  }

		  fclose(BMPimageFile);

          if (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)  //just for LINUX
          {
#ifdef LINUX

              sprintf(imageFileNamePdf, "%s%03d.pdf",bitmap_Image, Client_number);
              sprintf(params, "\"%s\" %s %d", imageFileName, imageFileNamePdf, matrix_head.vertical_density);
              runcode = RunSilent("image2image.exe", params);

              if (runcode > 0) {
                  sprintf(error_str, "%s", _FILE_WRITING_ERROR_);
                  ErrListStr(error_str);
              }

              sprintf(params, "-d %s -o Resolution=%ddpi", ptrs__prn_ini_date->printer, matrix_head.vertical_density); //ptrs__prn_ini_date->density);
              runcode = SystemSilent("lpoptions",params);

              sprintf(params, "-c -d %s %s", ptrs__prn_ini_date->printer, imageFileNamePdf); //imageFileName);
              runcode = SystemSilent("lp",params);
              if (runcode > 0) {
                  sprintf(error_str, "%s%d", err_message_cups, runcode);
                  ErrListStr(error_str);
              }
#endif
          }
          else {
              if (strcmp(ptrs__prn_ini_date->image_format, "BMP") == 0) {
                  ret = get_dest_image_file_name(ptrs__prn_ini_date->plik, fnd, i_pageno,
                                                 ptrs__prn_ini_date->image_format);
                  if (FALSE == Copy_File(fnd, imageFileName)) {
                      ErrListStr((char *) _FILE_WRITING_ERROR_);
                  }
              } else {
                  ret = get_dest_image_file_name(ptrs__prn_ini_date->plik, fnd, i_pageno,
                                                 ptrs__prn_ini_date->image_format);
                  sprintf(params, "\"%s\" %s %d", imageFileName, fnd, matrix_head.vertical_density);
                  runcode = RunSilent("image2image.exe", params);
                  if (runcode > 0) {
                      sprintf(error_str, "%s%d", err_message, runcode);
                      ErrListStr(error_str);
                  }
              }
          }
		  generateBitmapImageHeaders(height, width, imageFileName);

		  ret = draw_top_margin_PCX(width, top);

		  BMP_rows = top;

	    }
	    else 
	    {
		  if (draw_next_page () == FALSE) return FALSE ;
	    }

		  i_pageno++;

		  xmax -= df_margin ;
		  df_x0 = xmax;  //23-01-2021
	   }
      }
      else {
          if (b_new_page == TRUE && !(b_break_x == TRUE &&  b_break_y == TRUE)) {
              b_new_page = FALSE;
              df_height = 0;
              i_pageno++;

              xmax -= df_margin;
              /*df_x0 = xmin ;*/
              df_x0 = xmax;  //23-01-2021
          }
      }

      if (b_break_x == TRUE)
      {
	break ;
		}
      if (ptrs__prn_ini_date->inversion == TRUE)
       xmin = xmax;
        else xmin = xmax ;
    }
    if (b_break_y == TRUE)
    {
      break ;
    }

	ymin = ymax - df_margin;
  }
  
  if ((ptrs__prn_ini_date->prn_type == PRN_PCX)
#ifdef LINUX
    || (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)  //just for LINUX
#endif
    )
  {
	  correct_prn_bitmap_colors(allegro_prn_bmp);

	  if (BMP_rows < height)
	  {
		  ret = finish_BMP(width, height - BMP_rows);
	  }
	  
	  fclose(BMPimageFile);

      if (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)  //just for LINUX
      {
#ifdef LINUX
          sprintf(imageFileNamePdf, "%s%03d.pdf",bitmap_Image, Client_number);

          sprintf(params, "\"%s\" %s %d", imageFileName, imageFileNamePdf, matrix_head.vertical_density);
          runcode = RunSilent("image2image.exe", params);

          if (runcode > 0) {
              sprintf(error_str, "%s", _FILE_WRITING_ERROR_);
              ErrListStr(error_str);
          }

          sprintf(params, "-d %s -o Resolution=%ddpi", ptrs__prn_ini_date->printer, matrix_head.vertical_density); //ptrs__prn_ini_date->density);
          runcode = SystemSilent("lpoptions",params);

          sprintf(params, "-c -d %s %s", ptrs__prn_ini_date->printer, imageFileNamePdf); //imageFileName);
          runcode = SystemSilent("lp",params);
          if (runcode > 0) {
              sprintf(error_str, "%s%d", err_message_cups, runcode);
              ErrListStr(error_str);
          }
#endif
      }
      else {
          if (strcmp(ptrs__prn_ini_date->image_format, "BMP") == 0) {
              ret = get_dest_image_file_name(ptrs__prn_ini_date->plik, fnd, i_pageno, ptrs__prn_ini_date->image_format);
              if (FALSE == Copy_File(fnd, imageFileName)) {
                  ErrListStr((char *) _FILE_WRITING_ERROR_);
              }
          } else {
              ret = get_dest_image_file_name(ptrs__prn_ini_date->plik, fnd, i_pageno, ptrs__prn_ini_date->image_format);
              sprintf(params, "\"%s\" %s %d", imageFileName, fnd, matrix_head.vertical_density);
              runcode = RunSilent("image2image.exe", params);
              if (runcode > 0) {
                  sprintf(error_str, "%s%d", err_message, runcode);
                  ErrListStr(error_str);
              }
          }
      }

	  ret = Delete_File(imageFileName);
  }
    else if (ptrs__prn_ini_date->prn_type==PRN_WINDOWS) 
	{
		EndWinPage();
		EndWinDoc();
	}
	else if (ptrs__prn_ini_date->prn_type == PRN_PDF)
	  {
		  EndPDFPage();
		  EndPDFDoc(ptrs__prn_ini_date->plik);
	  }
		else draw_file_end () ;

  return TRUE ;
}

/*-------------------------------------------------*/
/*-------------------------------------------------*/

static void clear_matrix (BOOL set_background)
/*------------------------------------------*/
{
    byte_ int_intensity;
    byte_ intensity;
    float intensity_tab[4] = { 0.2, 1.0, 0.8, 0.6 };
    //int r,g,b;
    int b_color24, kolor256;

#ifdef ALLEGRO_PRN_BMP
  if ((ptrs__prn_ini_date->prn_type == PRN_PCX) || (ptrs__prn_ini_date->prn_type == PRN_WINDOWS))
	  if (BITS_PER_DOT < 24)
	  {
		  if ((set_background) && (ptrs__prn_ini_date->background)) clear_to_color(allegro_prn_bmp, get_palette_color(kolory.paper));
		  else clear_to_color(allegro_prn_bmp, makecol(255, 255, 255));
	  }
	  else
	  {
#ifdef LINUX
      if ((set_background) && (ptrs__prn_ini_date->background))
      {
          int b_color;
          int tmpR, tmpG, tmpB, tmpA;
          b_color = get_palette_color(kolory.paper);
          tmpB = b_color & 0xFF; b_color >>= 8;
          tmpG = b_color & 0xFF; b_color >>= 8;
          tmpR = b_color & 0xFF; b_color >>= 8;
          tmpA = b_color & 0xFF; /* dwColor >>= 8; */

          b_color24 = ((tmpB & 0xff) << 16) + ((tmpG & 0xff) << 8) + (tmpR & 0xff);

          if ((ptrs__prn_ini_date->gray_print) || !(ptrs__prn_ini_date->color_print) || (ptrs__prn_ini_date->PCX_gray))  //|| (ptrs__prn_ini_date->PCX_gray) this is not clear if should be
          {
              intensity = 0.2989 * tmpR + 0.5870 * tmpG + 0.1140 * tmpB;

              if (intensity < 252) intensity *= intensity_tab[ptrs__prn_ini_date->gray_saturation];

              int_intensity = (byte_) intensity;
              b_color24 = makecol(int_intensity, int_intensity, int_intensity);

          } else if (!(ptrs__prn_ini_date->color_print)) {
              if ((tmpB < 252) && (tmpG < 252) && (tmpB < 252)) {
                  //int_intensity = 0xFF;
                  //int_intensity = 0;
                  //b_color24 = makecol(int_intensity, int_intensity, int_intensity);
                  b_color24 = makecol(255, 255, 255);
              }
          }
      }
      else b_color24=makecol(255, 255, 255);
#else
          if ((set_background) && (ptrs__prn_ini_date->background))
          {
              int b_color;
              int tmpR, tmpG, tmpB, tmpA;

              b_color = get_palette_color(kolory.paper);

              tmpR = b_color & 0xFF; b_color >>= 8;
              tmpG = b_color & 0xFF; b_color >>= 8;
              tmpB = b_color & 0xFF; b_color >>= 8;
              tmpA = b_color & 0xFF; /* dwColor >>= 8; */

              b_color24 = ((tmpR & 0xff) << 16) + ((tmpG & 0xff) << 8) + (tmpB & 0xff);

              if ((ptrs__prn_ini_date->gray_print) || !(ptrs__prn_ini_date->color_print) || (ptrs__prn_ini_date->PCX_gray))  //|| (ptrs__prn_ini_date->PCX_gray) this is not clear if should be
              {
                  intensity = 0.2989 * tmpR + 0.5870 * tmpG + 0.1140 * tmpB;

                  if (intensity < 252) intensity *= intensity_tab[ptrs__prn_ini_date->gray_saturation];

                  int_intensity = (byte_)intensity;
                  b_color24 = makecol(int_intensity, int_intensity, int_intensity);

              }
              else if (!(ptrs__prn_ini_date->color_print))
              {
                  if ((tmpR < 252) && (tmpG < 252) && (tmpB < 252))
                  {
                      //int_intensity = 0xFF;
                      //int_intensity = 0;
                      //b_color24 = makecol(int_intensity, int_intensity, int_intensity);
                      b_color24=makecol(255, 255, 255);
                  }
              }
          }
          else b_color24=makecol(255, 255, 255);
#endif
          clear_to_color(allegro_prn_bmp, b_color24);
	  }
#else

  if ((ptrs__prn_ini_date->prn_type==PRN_PCX) || (ptrs__prn_ini_date->prn_type == PRN_PDF))
   {
    for (j=0; j<64; j++)
     {
       //null_d_PCX[j]=kolory.paper;
	   null_d_PCX[j]=255; //przyjeto ostatecznie zawsze biale tlo
     }
    for (i = 0; i < (matrix_head.len) ; i+=64)
     {
      memmove(&matrix_head.lp_matrix_PCX [i], &null_d_PCX[0], 64);
     }
   }
  else if (ptrs__prn_ini_date->prn_type==PRN_WINDOWS)
   {
    for (j=0; j<64; j++)
     {
       null_d_PCX[j]=255;  //TLO ZAWSZE JEST BIALE DLA DRUKAREK WINDOWS
     }
    for (i = 0; i < (matrix_head.len) ; i+=64)  //*8
     {
      memmove(&matrix_head.lp_matrix_PCX [i], &null_d_PCX[0], 64);
     }
   }
#endif
}


#ifdef logo_alfacad
//AlfaCAD
int logo_dl=7;
int logo_log[8]={19,64,48,1,62,19,4};
//                  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  53  54  55  56  57  58  59  60  61  62  63  64
char logo_str[65]={'Y','a','s','F','D','J','.','*','p','w','S','4','(',')','(','7','3','7','2','A','3','W','(','^','*','&','^','+',')','_','1','5','0','M','*','^','(','G','F','C','B','R','c','f','T','G','J','X','f','g','E','L','o','n','f','W','s','7','=','+','r','F','C',' ','l'};
#endif

#ifndef logo_alfacad
int logo_dl=0;
int logo_log[1]={0};
char logo_str[1]={' '};
#endif


#define ENT_SOLID_NULL 3
#define ENT_PCX 2
#define ENT_SOLID 1
#define ENT_SOLID_TRANSLUCENT 6
#define ENT_SOLID_PATTERN 4
#define ENT_PCX_SKIN 5
#define ENT_ENTITIES 0
#define ALL_LAYERS 0
#define FROZEN 1
#define NOTFROZEN 2


static BOOL draw_to_matrix(Print_Rect window_to_print)
{
	int ret_pcx = 1;
	int ret_solid_pattern = 0;
	int ret_solid = 0;
	int ret_solid_frozen = 0;
	int ret_on_front = 0;
	int ret_entities = 0;

    pdf_error = 0;  //in case there is PDF printing

    if (ptrs__prn_ini_date->print_translucent_solids == TRUE)  //just patterned and translucent
    {
        if ((bitmap_exist) || (ptrs__prn_ini_date->background))
        {
            ret_pcx = draw_to_matrix_entities(window_to_print, ENT_PCX, ALL_LAYERS);
        }
        Set_Draw_Window(window_to_print.x_min, window_to_print.y_min, window_to_print.x_max, window_to_print.y_max);
        if (bitmap_pattern_exist)
        {
            ret_solid_pattern = draw_to_matrix_entities(window_to_print, ENT_SOLID_PATTERN, NOTFROZEN);
        }

	//if (ptrs__prn_ini_date->print_translucent_solids == TRUE)  //just patterned and translucent
	//{
		Set_Draw_Window(window_to_print.x_min, window_to_print.y_min, window_to_print.x_max, window_to_print.y_max);
		ret_solid_frozen = draw_to_matrix_entities(window_to_print, ENT_SOLID_TRANSLUCENT, FROZEN);
		if (!ret_solid_frozen) return FALSE;
		Set_Draw_Window(window_to_print.x_min, window_to_print.y_min, window_to_print.x_max, window_to_print.y_max);
		ret_solid = draw_to_matrix_entities(window_to_print, ENT_SOLID_TRANSLUCENT, NOTFROZEN);


		if (ptrs__prn_ini_date->bitmap_only) return (ret_pcx + ret_solid_pattern + ret_on_front + ret_solid_frozen + ret_solid);
	}


    Set_Draw_Window(window_to_print.x_min, window_to_print.y_min, window_to_print.x_max, window_to_print.y_max);
    ret_solid_frozen = draw_to_matrix_entities(window_to_print, ENT_SOLID, FROZEN);
    if (!ret_solid_frozen) return FALSE;
    Set_Draw_Window(window_to_print.x_min, window_to_print.y_min, window_to_print.x_max, window_to_print.y_max);
    ret_solid = draw_to_matrix_entities(window_to_print, ENT_SOLID, NOTFROZEN);


	Set_Draw_Window(window_to_print.x_min, window_to_print.y_min, window_to_print.x_max, window_to_print.y_max);
	ret_entities = draw_to_matrix_entities(window_to_print, ENT_ENTITIES, FROZEN);
	if (!ret_entities) return FALSE;
	Set_Draw_Window(window_to_print.x_min, window_to_print.y_min, window_to_print.x_max, window_to_print.y_max);
	ret_entities = draw_to_matrix_entities(window_to_print, ENT_ENTITIES, NOTFROZEN);
	if (!ret_entities) return FALSE;
	if (ptrs__prn_ini_date->bitmap_only == FALSE)
	{
		if (bitmap_on_front_exist)
		{
			ret_entities = draw_to_matrix_entities(window_to_print, ENT_PCX_SKIN, ALL_LAYERS);
			if (!ret_entities) return FALSE;
		}
	}


	return (ret_pcx | ret_solid | ret_entities);
}

void Init_TTF_text_to_matrix(TEXT *t, double font_scale, int font_index, int kolor256, BOOL bw, BOOL grey)
{

	COLOR_ kolor;
	byte_ intensity;
	float wysokosc_p;
	double kat;
	
	wysokosc_p = jednostki_y_to_prn(t->wysokosc) * font_scale;

	kat = t->kat;

	if (ptrs__prn_ini_date->rotation == TRUE)
	{
		kat -= PI2;
	}
	if (ptrs__prn_ini_date->inversion == TRUE)
	{
		kat -= PI; //TO DO
	}

	//reflection;
    
	if ((ptrs__prn_ini_date->prn_type == PRN_PCX))
	{

		kolor.red = _dac_normal[kolor256][0] << 2;
		kolor.gre = _dac_normal[kolor256][1] << 2;
		kolor.blu = _dac_normal[kolor256][2] << 2;
	}
	else
	{
		kolor.red = _dac_normal[kolor256][0] << 2;
		kolor.gre = _dac_normal[kolor256][1] << 2;
		kolor.blu = _dac_normal[kolor256][2] << 2;
	}
	

	if ((ptrs__prn_ini_date->color_print == 1) && (bw == 0))
	{
		if ((ptrs__prn_ini_date->gray_print == 1) || (grey == 1))
		{
			intensity = get_grey_intensity_ttf(kolor);

			kolor.red = intensity;
			kolor.gre = intensity;  //254
			kolor.blu = intensity;  //254
		}
	}
	else
	{
		if ((kolor.red < 254) && (kolor.gre < 254) && (kolor.blu < 254))
			kolor = { 0, 0, 0 };
	}

	Init_Draw_TTF_text(t, kat, wysokosc_p, kolor);
}

void Amend_TTF_text_to_matrix(TEXT *t, double font_scale, int font_index)
{
	float wysokosc_p;

	wysokosc_p = jednostki_y_to_prn(t->wysokosc) * font_scale;
	Amend_Draw_TTF_text(t, wysokosc_p);
}

void Draw_TTF_text_to_matrix(TEXT *t, char *zn, double x01, double y01, double font_scale, int font_index, int kolor256, BOOL bw, BOOL grey, BOOL index_pos)
{

	int x1, y1;
	COLOR_ kolor;
	byte_ intensity;
	float wysokosc_p;
	double kat;

	if (ptrs__prn_ini_date->prn_type == PRN_PDF)
	{

		x1 = jednostki_to_prn_x(x01, y01);
		y1 = jednostki_to_prn_y(x01, y01);

		kolor.red = _dac_normal[kolor256][0] << 2;
		kolor.gre = _dac_normal[kolor256][1] << 2;
		kolor.blu = _dac_normal[kolor256][2] << 2;

		if ((ptrs__prn_ini_date->color_print == 1) && (bw == 0))
		{
			if ((ptrs__prn_ini_date->gray_print == 1) || (grey == 1))
			{
				intensity = get_grey_intensity(kolor);
				kolor = { intensity, intensity, intensity };
			}
		}
		else
		{
			if ((kolor.red < 254) && (kolor.gre < 254) && (kolor.blu < 254))
				kolor = { 0, 0, 0 };
		}

		
		x1 += pdf_left_margin;
		
		y1 = pdf_page_height - y1 - pdf_top_margin;


		kat = t->kat;

		if (ptrs__prn_ini_date->rotation == TRUE)
		{
			kat -= PI2;
		}
		if (ptrs__prn_ini_date->inversion == TRUE)
		{
			kat -= PI; //TO DO
		}

		TextPDF(t, zn, x1, y1, font_scale, kat, kolor, index_pos);

		return;
	}

	x1 = jednostki_to_prn_x(x01, y01);
	y1 = jednostki_to_prn_y(x01, y01);

	x1 += matrix_head.left_margin;

	wysokosc_p = jednostki_y_to_prn(t->wysokosc) * font_scale;

	kat = t->kat;

	if (ptrs__prn_ini_date->rotation == TRUE)
	{
		kat -= PI2;
	}
	if (ptrs__prn_ini_date->inversion == TRUE)
	{
		kat -= PI; //TO DO
	}

	//reflection;

	kolor.red = _dac_normal[kolor256][0] << 2;
	kolor.gre = _dac_normal[kolor256][1] << 2;
	kolor.blu = _dac_normal[kolor256][2] << 2;

	if ((ptrs__prn_ini_date->color_print == 1) && (bw == 0))
	{
		if ((ptrs__prn_ini_date->gray_print == 1) || (grey == 1))
		{
			intensity = get_grey_intensity_ttf(kolor);

			kolor.red = intensity;
			kolor.gre = 254;// intensity;
			kolor.blu = 254;// intensity;
		}
	}
	else
	{
		if ((kolor.red < 254) && (kolor.gre < 254) && (kolor.blu < 254))
			kolor = { 0, 0, 0 };
	}
#define COPY_PUT 0
	Draw_TTF_text(t, zn, allegro_prn_bmp, x1, y1, kat, wysokosc_p, kolor, COPY_PUT);
}

void Draw_TTF_char_to_matrix(TEXT *t, unsigned int unicode, double x01, double y01, double font_scale, int font_index, int kolor256, BOOL bw, BOOL grey)
{
	int x1, y1;

	if (type__drive != PRN_DRIVE) return;

	if (ptrs__prn_ini_date->prn_type == PRN_PDF) return;  //it works always as full text

	x1 = jednostki_to_prn_x(x01, y01);
	y1 = jednostki_to_prn_y(x01, y01);

	x1 += matrix_head.left_margin;

	Draw_TTF_char(t, unicode, allegro_prn_bmp, x1, y1);
}



int split_dim_line(LINIA *L, LINIA *L_tmp, LINIA *L_tmp1) {
    TEXT *t;
    QUAD t_outline;
    int ret;
    int ob3;

        t = (TEXT*)((char*)L + sizeof(NAGLOWEK) + L->n);
        if ((t->blok==1) && (t->obiekt == Otekst))
        {
            if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
            {
                //checking crossing with text
                outlinetext(t, &t_outline, 0.5);
                Normalize_Quad (&t_outline);
                ret=trim_line_to_quad(L, &t_outline, L_tmp, L_tmp1);
                return ret;
            }
        }
        else if (t->obiekt == Olinia)
        {
            t = (TEXT*)((char*)t + sizeof(NAGLOWEK) + t->n);
            if ((t->blok==1) && (t->obiekt == Otekst))
            {
                if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
                {
                    //checking crossing with text
                    outlinetext(t, &t_outline, 0.5);
                    Normalize_Quad (&t_outline);
                    ret=trim_line_to_quad(L, &t_outline, L_tmp, L_tmp1);
                    return ret;
                }
            }
            else return 0;
        }
        else return 0;
}

int split_dim_arc(LUK *l, LUK *l_tmp, LUK *l_tmp1) {
    NAGLOWEK *nag, *nag1;
    TEXT *t;
    QUAD t_outline;
    int ret;

    //checking if arc is dimensioning arc
    if ((l->blok == 1) && (l->obiektt1 == 0) && (l->obiektt2 == 1) && (l->obiektt3 == 0))
    {
        //check if next object is text or line then text
        nag1 = (NAGLOWEK*)((char*)l + sizeof(NAGLOWEK) + l->n);
        if (nag1->obiekt == Otekst)
        {
            t = (TEXT*)nag1;
            if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
            {
                //checking crossing with text
                outlinetext(t, &t_outline, 0.5);
                Normalize_Quad (&t_outline);
                ret=trim_arc_to_quad(l, &t_outline, l_tmp, l_tmp1);
                return ret;
            }
        }
        else if (nag1->obiekt == Olinia)
        {
            nag1 = (NAGLOWEK*)((char*)nag1 + sizeof(NAGLOWEK) + nag1->n);
            if (nag1->obiekt == Olinia)
                nag1 = (NAGLOWEK*)((char*)nag1 + sizeof(NAGLOWEK) + nag1->n);
            if (nag1->obiekt == Otekst)
            {
                t = (TEXT*)nag1;
                if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
                {
                    //checking crossing with text
                    outlinetext(t, &t_outline, 0.5);
                    Normalize_Quad (&t_outline);
                    ret=trim_arc_to_quad(l, &t_outline, l_tmp, l_tmp1);
                    return ret;
                }
            }
            else return 0;
        }
        else return 0;
    }
    else return 0;
}

static BOOL draw_to_matrix_entities(Print_Rect window_to_print, int entities, char frozen)
/*--------------------------------------------------------------------------------------*/
{
 TEXT T=Tdef, T1, *T0;
 NAGLOWEK *nag;
 LUK l = ldef, l1, l_tmp, l_tmp1;
 LINIA L = Ldef, L_tmp, L_tmp1;
 SPLINE S = Splinedef;
 AVECTOR V;
 T_Point *point;
 long_long off, offk, ad;
 int k;
 BOOL b_err=TRUE;
 //double dist_1;
 int layer_0_on;
 char st[MaxTextLen];
 int i;
 BOOL draw_logo;
 char *block_adr=NULL;
 BLOK *b1, *b_sheet;
 T_Desc_Ex_Block *ptrs_desc_bl1;
 double del_minx,del_miny,del_maxx,del_maxy;
 double xmin__,width__,ymin__,height__,xmax__,ymax__;
 int t_drive;
 int aaaa;
 double df_width_line;
 double width_s0, width_s1;
 int ret;
    int transluc;
    char* translucency_ptr;

 BLOK_SIEC=FALSE;

 if (ptrs__prn_ini_date->prn_type== PRN_PCX) t_drive=0; 
 else if (ptrs__prn_ini_date->prn_type == PRN_PDF) t_drive = 0;
 else t_drive=1;

 if (ptrs__prn_ini_date->inversion == TRUE) print_inversion=TRUE;
   else print_inversion=FALSE;
 if (ptrs__prn_ini_date->rotation == TRUE) print_rotation = TRUE;
 else print_rotation = FALSE;

  if (strlen(zbior_danych_2)>0)
  {
   zwolnienie_pamieci();
   k = czytaj_zbior("RYSUJ.$2", b_err, TRUE, TRUE);
    if (k)
    {
      if (k == 1 && b_err == TRUE)
      {
	ErrList (101) ;
	return FALSE;
      }
      else
       {
       ErrList (102) ;
       return FALSE;
       }
    }
  }

//jezeli drukowane jest "okno+arkusz":
// 1.wyszukiwany jest blok arkusza (adres bloku jest znany jako parametr)
// 2.

 off=0;
 offk=dane_size ;

#define VARIANT1
 
#ifdef  VARIANT1
 if (ptrs__prn_ini_date->rotation == TRUE)
 {
	 if (ptrs__prn_ini_date->inversion == TRUE)

         oknoS_ (FormatX_S - matrix_head.xp - matrix_head.height - line_max_h_width,
			  FormatY_S - matrix_head.yp - matrix_head.width_max - line_max_h_width,
			  FormatX_S - matrix_head.xp + line_max_h_width,
			  FormatY_S - matrix_head.yp + line_max_h_width) ;


   else
    oknoS_ (matrix_head.xp - line_max_h_width,
	   matrix_head.yp - line_max_h_width,
	   matrix_head.xp + matrix_head.height + line_max_h_width,
	   matrix_head.yp + matrix_head.width_max + line_max_h_width) ;
 }
 else
 {
	 if (ptrs__prn_ini_date->inversion == TRUE)

            oknoS_(
                 FormatX_S + FormatX_S0 - matrix_head.width_max - matrix_head.xp - line_max_h_width,
                 FormatY_S + FormatY_S0 - matrix_head.height- matrix_head.yp - line_max_h_width,
                 FormatX_S + FormatX_S0 - matrix_head.xp + line_max_h_width,
                 FormatY_S + FormatY_S0 - matrix_head.yp + line_max_h_width) ;


  else
   oknoS_ (matrix_head.xp - line_max_h_width,
	  matrix_head.yp - line_max_h_width,
	  matrix_head.xp + FormatX_S  + line_max_h_width, //___max !!!
	  matrix_head.yp + matrix_head.height + line_max_h_width) ;
 }
#else

    del_minx=window_to_print.x_min;
    del_miny=window_to_print.y_min;
    del_maxx=window_to_print.x_max;
    del_maxy=window_to_print.y_max;

    Set_Draw_Window(del_minx, del_miny, del_maxx, del_maxy);
    oknoS_(del_minx, del_miny, del_maxx, del_maxy);
    set_XY(del_minx, del_miny, del_maxx, del_maxy);
#endif
 
 //tutaj mozna dodac drukowanie logo firmy producenta
 char *sz_series;

 sz_series=s_prot_date.sz_series;
 draw_logo = (BOOL) strcmp(sz_series, "01010101");

#ifdef UKRAINIAN
 draw_logo = FALSE;
#else
 draw_logo = FALSE;  //it's finally Donationware ...
#endif

SHEET_IS_PRINTED=FALSE;

if (draw_logo==TRUE)
{

 st[0]='\0';
 for (i=0; i<logo_dl; i++)
  {
    st[i]=logo_str[logo_log[i]];
  }
 st[logo_dl]='\0';

 layer_0_on=Layers[0].on;
 Layers[0].on=1;
 T.warstwa=0;
 T.czcionka = get_alfacad_logo_font(); // 4;

 T.wysokosc= (ptrs__prn_ini_date->xk - ptrs__prn_ini_date->xp) * 0.035;

 T.kat = 0.0;
 T.justowanie= j_centralnie;
 T.typ=n_typ_normal;

 T.x = ptrs__prn_ini_date->xp + (ptrs__prn_ini_date->xk - ptrs__prn_ini_date->xp) / 2.0;
 T.y = ptrs__prn_ini_date->yp + (ptrs__prn_ini_date->yk - ptrs__prn_ini_date->yp) / 2.0;

 if (ptrs__prn_ini_date->gray_print) T.kolor = 249;
 else T.kolor = 254;
 T.bold=0;
 T.obiektt2=O2NoBlockS;
 strcpy(T.text,st);
 T.dl=strlen(T.text);
 T.n=T18+T.dl;
 T.width = 0.0;
 T.height = 0.0;
 if (tekst_wybrany_prn (&T))
  {
	df_width_line = s__Fonts_Style[T.czcionka].df_width_line;
	s__Fonts_Style[T.czcionka].df_width_line *= 0.5;
	line_width_type = TEXT_LINE_TYPE; // LINE_NORM_WIDTH;
	if (Draw_Tekst_To_Drive(&T, 0, 1, 1) == FALSE)
	{
		s__Fonts_Style[T.czcionka].df_width_line = df_width_line;
		return FALSE;
	}
	s__Fonts_Style[T.czcionka].df_width_line = df_width_line;
  }
 Layers[0].on=layer_0_on;
}


 if ((ptrs__prn_ini_date->print_sheet == TRUE)) // &&   //08-12-2020
    // (ptrs__prn_ini_date->print_window == TRUE))
 {
	 SHEET_IS_PRINTED = TRUE;

	 if (ptrs__prn_ini_date->prn_type != PRN_PDF)
		 set_clip_rect(allegro_prn_bmp, matrix_head.left_margin + left_clip_sheet, min_y_clip_sheet, max_x_clip_sheet, max_y_clip_sheet);
     else
        SetPDFClip(matrix_head.left_margin + left_clip_sheet + extra_off_left, matrix_head.bottom_margin + min_y_clip_sheet + extra_off_bottom, max_x_clip_sheet + extra_off_right, /*matrix_head.bottom_margin + */ max_y_clip_sheet + extra_off_top);

   off=ptrs__prn_ini_date->sheet_adr - dane;
   b_sheet=(BLOK *)ptrs__prn_ini_date->sheet_adr;
   offk=off+sizeof(NAGLOWEK)+b_sheet->n-1;

  ObiektTok (&off, offk, &ad, ONieOkreslony);
  while (ad!=-1)
  {
    nag = (NAGLOWEK*)(dane+ad);
    if (my_kbhit () && my_getch() == ESC)
    {
      while (my_kbhit ())
      {
	 my_getch();
      }
      return FALSE;
    }

    if (pdf_error != 0) return FALSE;

    if (BLOK_SIEC==TRUE)
     {
      if ((long_long)nag>(long_long)block_adr) BLOK_SIEC=FALSE;
     }

    if ((nag->obiekt!=OdBLOK) && (nag->widoczny==0))   //// 19-09-2025  due to gradient solids
    {
        ObiektTok(&off, offk, &ad, ONieOkreslony);
        continue;
    }

    switch(nag->obiekt)
    {
       case OdBLOK :
	   reset_last_polygon_vectors();
       b1=(BLOK *)nag;
       ptrs_desc_bl1	= (T_Desc_Ex_Block *)(&b1->opis_obiektu	[0]) ;

    	   if ((strncmp(ptrs_desc_bl1->sz_type,"*#",2)==0) || (strncmp(ptrs_desc_bl1->sz_type,"*^",2)==0) || //to jest jakas siec
             (strncmp(ptrs_desc_bl1->sz_type,"*~",2)==0) || (strncmp(ptrs_desc_bl1->sz_type,"@kol",4)==0)) //to jest opis lub kolizja
          {
            if (BLOK_SIEC==FALSE)
             {
               BLOK_SIEC=TRUE;
               block_adr=(char *)nag + sizeof(NAGLOWEK) + b1->n - 1;
             }
          }

            if ((b1->kod_obiektu == B_PLINE) && (b1->opis_obiektu[0]== PL_TRACE))
            {
                set_trace_block_begin((char*)nag + sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu);
                set_trace_block_end((char*)nag + sizeof(NAGLOWEK) + b1->n - 1);
                set_trace_block(TRUE);
            }
            else set_trace_block(FALSE);

     break;
       case Olinia :
	  if (entities!=ENT_ENTITIES) break;
	  if (frozen) { if (!linia_frozen((LINIA *)nag, frozen-1)) break; }
	  if (!linia_wybrana ((LINIA *)nag)) break ;

      //edges
      if (((LINIA*)nag)->obiektt2==4) {
                memmove(&V, ((LINIA*)nag), sizeof(LINIA));
                if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_ROLL;
                else V.style = V_EDGE_ROLL_INV;
                Draw_Vector_To_Drive(&V, &window_to_print);
                break;
      }
      else if (((LINIA*)nag)->obiektt2==6) {
          memmove(&V, ((LINIA*)nag), sizeof(LINIA));
          if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_SIMPLE;
          else V.style = V_EDGE_SIMPLE_INV;
          Draw_Vector_To_Drive(&V, &window_to_print);
          break;
      }
       else if (((LINIA*)nag)->obiektt2==7) {
          memmove(&V, ((LINIA*)nag), sizeof(LINIA));
          if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_FIXED;
          else V.style = V_EDGE_FIXED_INV;
          Draw_Vector_To_Drive(&V, &window_to_print);
          break;
      }

	  line_width_type = Line_Width (((LINIA*)nag)->typ);
      if (check_dim_line((LINIA*)nag)==0) {if (Draw_Line_To_Drive ((LINIA*)nag) == FALSE) return FALSE;}
      else {
          ret = split_dim_line((LINIA *) nag, &L_tmp, &L_tmp1);
          switch (ret) {
              case 0:
                  if (Draw_Line_To_Drive((LINIA *) nag) == FALSE) return FALSE;
                  break;
              case 1:
                  if (Draw_Line_To_Drive(&L_tmp) == FALSE) return FALSE;
                  break;
              case 2:
                  if (Draw_Line_To_Drive(&L_tmp) == FALSE) return FALSE;
                  if (Draw_Line_To_Drive(&L_tmp1) == FALSE) return FALSE;
                  break;
              default:
                  break;
          }
      }

      if (BLOK_SIEC==TRUE)
      {
        memmove(&L,nag,sizeof(LINIA));
        L.kolor=7;
        L.typ=0;
        line_width_type = Line_Width (L.typ);
	     if (Draw_Line_To_Drive (&L) == FALSE) return FALSE;
      }
	  break;
       case Otekst :
	   if (entities!=ENT_ENTITIES) break;
	  if (!tekst_wybrany_prn ((TEXT*)nag)) break;  
	  if (frozen) { if (!tekst_frozen((TEXT *)nag, frozen-1)) break; }
	  
	 line_width_type = TEXT_LINE_TYPE; /* LINE_NORM_WIDTH; */

	 if (Draw_Tekst_To_Drive((TEXT*)nag, 0, 1, 1/*, 0*/) == FALSE) return FALSE;
	 
     if (BLOK_SIEC==TRUE)
      {
        T0=(TEXT *)nag;
        memmove(&T1,T0,sizeof(TEXT));
        strcpy(T1.text,T0->text);
        T1.kolor=7;
        T1.bold=0;
		T1.width = 0;
		if ((T0->bold == 1) && (PTRS__Text_Style[T0->czcionka]->type == 2))
		{
			width_s0 = Get_Text_Len(T0, T0->text, 1.0, 0, NULL, NULL, NULL,  NULL);
			width_s1 = Get_Text_Len(&T1, T1.text, 1.0, 0, NULL, NULL, NULL,  NULL);
			ST_W_FACTOR = width_s0 / width_s1;
			T1.width = 0;
			T1.width_factor = T0->width_factor * ST_W_FACTOR;
		}
        line_width_type = LINE_THINEST_WIDTH;;
	     if (Draw_Tekst_To_Drive (&T1, 0, 1, 1) == FALSE) return FALSE;
      }
	  

	  break;
       case Okolo :
	  if (entities!=ENT_SOLID) break;
	  if (frozen) { if (!okrag_frozen((OKRAG *)nag, frozen-1)) break; }
	  if (!okrag_wybrany ((OKRAG*)nag)) break ;
	  
	  line_width_type = LINE_MIN_WIDTH ;
	  if (Draw_Fill_Circle_To_Drive ((OKRAG*)nag) == FALSE) return FALSE;
          l.warstwa = ((OKRAG*)nag)->warstwa;
          l.kolor = ((OKRAG*)nag)->kolor;
          //l.typ = ((OKRAG*)nag)->typ;
          l.typ = 0;
          l.x = ((OKRAG*)nag)->x;
          l.y = ((OKRAG*)nag)->y;
          l.kat1 = 0;
          l.kat2 = Pi2;
          l.r = ((OKRAG*)nag)->r;
          if (BLOK_SIEC==TRUE)
           {
             l.kolor=7;
           }
         if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;  //circle around the disk, to make disk more precise, but it should be also translucent: TO DO
	  break;
        case Opoint:
            if (((T_Point *)nag)->typ>=7)  //junction
            {
                point=(T_Point*)nag;
                if (entities != ENT_ENTITIES) break;
                if (frozen) { if (!point_frozen(point, frozen - 1)) break; }
                if (!Point_Selected(point)) break;

                line_width_type = LINE_MIN_WIDTH;
                if (Draw_Point_To_Drive(point) == FALSE) return FALSE;
            }
            break;
       case Ookrag :
      if (entities!=ENT_ENTITIES) break;
	  if (frozen) { if (!okrag_frozen((OKRAG *)nag, frozen-1)) break; }
	  if (!okrag_wybrany ((OKRAG*)nag)) break ;
	  
	  line_width_type = Line_Width (((OKRAG*)nag)->typ);
	  l.warstwa = ((OKRAG*)nag)->warstwa;
	  l.kolor = ((OKRAG*)nag)->kolor;
	  l.typ = ((OKRAG*)nag)->typ;
	  l.x = ((OKRAG*)nag)->x;
	  l.y = ((OKRAG*)nag)->y;
	  l.r = ((OKRAG*)nag)->r;
	  l.kat1 = 0;
	  l.kat2 = Pi2;
	  if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	 break;
     case Oellipse :
        if (entities!=ENT_ENTITIES) break;
        if (frozen) { if (!ellipse_frozen((ELLIPSE *)nag, frozen-1)) break; }
        if (!elipsa_wybrana_prec ((ELLIPSE*)nag)) break ;

        line_width_type = Line_Width (((ELLIPSE*)nag)->typ);

        if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
        break;
    case Ofilledellipse :
            if (entities!=ENT_SOLID) break;
            if (frozen) { if (!ellipse_frozen((ELLIPSE *)nag, frozen-1)) break; }
            if (!elipsa_wybrana_prec ((ELLIPSE*)nag)) break ;

            line_width_type = Line_Width (((ELLIPSE*)nag)->typ);

            if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
        break;
        case Oellipticalarc :
            if (entities!=ENT_ENTITIES) break;
            if (frozen) { if (!ellipticalarc_frozen((ELLIPTICALARC *)nag, frozen-1)) break; }
            if (!lukeliptyczny_wybrany  ((ELLIPTICALARC*)nag)) break ;

            line_width_type = Line_Width (((ELLIPTICALARC*)nag)->typ);

            if (Draw_EllipticalArc_To_Drive ((ELLIPTICALARC*)nag) == FALSE) return FALSE;
            break;
       case Owwielokat :

           transluc=255;
            if ((((WIELOKAT*)nag)->translucent) == 1) {
                translucency_ptr = (char*)((WIELOKAT*)nag)->xy;
                translucency_ptr += (((WIELOKAT*)nag)->lp * sizeof(float));
                memmove(&transluc, translucency_ptr,  sizeof(unsigned char));
            }

		   if ((entities == ENT_SOLID_NULL) && ((((WIELOKAT*)nag)->empty_typ) != 6) && ((((WIELOKAT*)nag)->empty_typ) != 7)) break;
           if ((entities == ENT_SOLID_TRANSLUCENT) && (((((WIELOKAT*)nag)->empty_typ) != 0) || ((((WIELOKAT*)nag)->pattern) == 1) || ((((WIELOKAT*)nag)->translucent) == 0) || (transluc==255))) break;
		   if ((entities == ENT_SOLID) && (((((WIELOKAT*)nag)->empty_typ) != 0) || ((((WIELOKAT*)nag)->pattern) == 1) || (((((WIELOKAT*)nag)->translucent) == 1) && (transluc<255)))) break;
           //if ((entities == ENT_SOLID_PATTERN) && (((((WIELOKAT*)nag)->empty_typ) != 0) || ((((WIELOKAT*)nag)->pattern) != 1))) break;
           if ((entities == ENT_SOLID_PATTERN) && (((((WIELOKAT*)nag)->empty_typ) != 0) || (((((WIELOKAT*)nag)->pattern) != 1) && ((((WIELOKAT*)nag)->gradient) != 1)))) break;
		   if ((entities == ENT_ENTITIES) && (((((WIELOKAT*)nag)->empty_typ) == 0))) break;
		   if ((entities != ENT_ENTITIES) && (entities != ENT_SOLID_NULL) && (entities != ENT_SOLID) && (entities != ENT_SOLID_TRANSLUCENT) && (entities != ENT_SOLID_PATTERN)) break;
           if ((entities != ENT_SOLID_PATTERN) && (((((WIELOKAT*)nag)->empty_typ) == 0) && ((((WIELOKAT*)nag)->gradient) == 1))) break;

	  if (frozen) { if (!wielokat_frozen((WIELOKAT *)nag, frozen-1)) break; }

	  //if (!wielokat_wybrany((WIELOKAT*)nag)) break;
     if (!wielokat_visible_in_frame((WIELOKAT*)nag, (RECTD*)&window_to_print)) break;
	  
	  if (((WIELOKAT*)nag)->empty_typ==0) line_width_type = LINE_MIN_WIDTH ;
       else  line_width_type = Solid_Line_Width (((WIELOKAT*)nag)->empty_typ);

     if ((((WIELOKAT*)nag)->empty_typ)==7) ENABLE_FOUND=TRUE;
     if ((((WIELOKAT*)nag)->empty_typ)==6) NULL_FOUND=TRUE;
	 if ((((WIELOKAT*)nag)->blok) == 0) reset_last_polygon_vectors();
	  if (Draw_Wielokat_To_Drive ((WIELOKAT*)nag, &window_to_print) == FALSE) return FALSE;
	 break;
    case Osolidarc :
            transluc=255;
            if ((((SOLIDARC*)nag)->translucent) == 1) transluc=((SOLIDARC*)nag)->translucency;

            if ((entities == ENT_SOLID_NULL) && ((((SOLIDARC*)nag)->empty_typ) != 6) && ((((SOLIDARC*)nag)->empty_typ) != 7)) break;
            if ((entities == ENT_SOLID_TRANSLUCENT) && (((((SOLIDARC*)nag)->empty_typ) != 0) || ((((SOLIDARC*)nag)->pattern) == 1) || ((((SOLIDARC*)nag)->translucent) == 0) || (transluc==255))) break;
            if ((entities == ENT_SOLID) && (((((SOLIDARC*)nag)->empty_typ) != 0) || ((((SOLIDARC*)nag)->pattern) == 1) || (((((SOLIDARC*)nag)->translucent) == 1) && (transluc<255)))) break;
            if ((entities == ENT_SOLID_PATTERN) && (((((SOLIDARC*)nag)->empty_typ) != 0) || ((((SOLIDARC*)nag)->pattern) != 1))) break;
            if ((entities == ENT_ENTITIES) && (((((SOLIDARC*)nag)->empty_typ) == 0))) break;
            if ((entities != ENT_ENTITIES) && (entities != ENT_SOLID_NULL) && (entities != ENT_SOLID) && (entities != ENT_SOLID_TRANSLUCENT) && (entities != ENT_SOLID_PATTERN)) break;

            if (frozen) { if (!solidarc_frozen((SOLIDARC *)nag, frozen-1)) break; }
            if (!solidarc_wybrany((SOLIDARC*)nag)) break;

            if (((SOLIDARC*)nag)->empty_typ==0) line_width_type = LINE_MIN_WIDTH ;
            else  line_width_type = Solid_Line_Width (((SOLIDARC*)nag)->empty_typ);

            if ((((SOLIDARC*)nag)->empty_typ)==7) ENABLE_FOUND=TRUE;
            if ((((SOLIDARC*)nag)->empty_typ)==6) NULL_FOUND=TRUE;
            if ((((SOLIDARC*)nag)->blok) == 0) reset_last_polygon_vectors();
            if (Draw_Solidarc_To_Drive ((SOLIDARC*)nag, &window_to_print) == FALSE) return FALSE;
            break;
	case Ospline:
		if (entities != ENT_ENTITIES) break;
		if (frozen) { if (!spline_frozen((SPLINE *)nag, frozen - 1)) break; }
		if (!spline_wybrany((SPLINE *)nag)) break;

		line_width_type = Line_Width(((SPLINE*)nag)->typ);
		if (Draw_Spline_To_Drive((SPLINE*)nag) == FALSE) return FALSE;

		if (BLOK_SIEC == TRUE)
		{
			memmove(&S, nag, sizeof(SPLINE));
			S.kolor = 7;
			S.typ = 0;
			line_width_type = Line_Width(S.typ);
			if (Draw_Spline_To_Drive(&S) == FALSE) return FALSE;
		}
		break;

    case Opcx:
	 if ((entities!=ENT_PCX) && (entities != ENT_PCX_SKIN)) break;
	 if ((entities == ENT_PCX) && (((B_PCX*)nag)->on_front==1)) break;
	 if ((entities == ENT_PCX_SKIN) && (((B_PCX*)nag)->on_front == 0)) break;
         //if (!pcx_w_prostokacie((B_PCX *)nag)) break;
	  if (!pcx_wybrany((B_PCX*)nag, TRUE)) break;
     line_width_type = LINE_MIN_WIDTH ;

	 if (ptrs__prn_ini_date->prn_type == PRN_PDF)
	 {
		 if (Draw_Pcx_To_PDF((B_PCX *)nag) == FALSE) return FALSE;
	 }
	 else
	 {
		 if (Draw_Pcx_To_Drive((B_PCX *)nag, t_drive) == FALSE) return FALSE;
	 }
	 break;
     case Oluk :
	  if (entities!=ENT_ENTITIES) break;
	  if (frozen) { if (!luk_frozen((LUK *)nag, frozen-1)) break; }
	  if(!luk_wybrany_prn ((LUK*)nag)) break;

        //edges
        if (((LUK*)nag)->obiektt2==4) {
            V.warstwa=((LUK*)nag)->warstwa;
            V.kolor=((LUK*)nag)->kolor;
            V.typ=((LUK*)nag)->typ;
            V.x1=((LUK*)nag)->x;
            V.y1=((LUK*)nag)->y;
            V.r=((LUK*)nag)->r;
            V.angle1=((LUK*)nag)->kat1;
            V.angle2=((LUK*)nag)->kat2;
            V.magnitude1=0.0f;
            if (((LUK*)nag)->obiektt3 == 0) V.style = V_EDGE_ARC_ROLL;
            else V.style = V_EDGE_ARC_ROLL_INV;
            Draw_Vector_To_Drive(&V, &window_to_print);
            break;
        }
        else if (((LUK*)nag)->obiektt2==6) {
            V.warstwa=((LUK*)nag)->warstwa;
            V.kolor=((LUK*)nag)->kolor;
            V.typ=((LUK*)nag)->typ;
            V.x1=((LUK*)nag)->x;
            V.y1=((LUK*)nag)->y;
            V.r=((LUK*)nag)->r;
            V.angle1=((LUK*)nag)->kat1;
            V.angle2=((LUK*)nag)->kat2;
            V.magnitude1=0.0f;
            if (((LUK*)nag)->obiektt3 == 0) V.style = V_EDGE_ARC_SIMPLE;
            else V.style = V_EDGE_ARC_SIMPLE_INV;
            Draw_Vector_To_Drive(&V, &window_to_print);
            break;
        }
        else if (((LUK*)nag)->obiektt2==7) {
            V.warstwa=((LUK*)nag)->warstwa;
            V.kolor=((LUK*)nag)->kolor;
            V.typ=((LUK*)nag)->typ;
            V.x1=((LUK*)nag)->x;
            V.y1=((LUK*)nag)->y;
            V.r=((LUK*)nag)->r;
            V.angle1=((LUK*)nag)->kat1;
            V.angle2=((LUK*)nag)->kat2;
            V.magnitude1=0.0f;
            if (((LUK*)nag)->obiektt3 == 0) V.style = V_EDGE_ARC_FIXED;
            else V.style = V_EDGE_ARC_FIXED_INV;

            Draw_Vector_To_Drive(&V, &window_to_print);
            break;
        }
	  
	  line_width_type = Line_Width (((LUK*)nag)->typ);

      ret=split_dim_arc((LUK*)nag, &l_tmp, &l_tmp1);
      switch (ret)
      {
        case 0:
            if (Draw_Arc_To_Drive ((LUK*)nag) == FALSE) return FALSE;
            break;
        case 1:
            if (Draw_Arc_To_Drive (&l_tmp) == FALSE) return FALSE;
            break;
        case 2:
            if (Draw_Arc_To_Drive (&l_tmp) == FALSE) return FALSE;
            if (Draw_Arc_To_Drive (&l_tmp1) == FALSE) return FALSE;
            break;
        default:
            break;
      }

     if (BLOK_SIEC==TRUE)
      {
        memmove(&l1,nag,sizeof(LUK));
        l1.kolor=7;
        l1.typ=0;
        line_width_type = Line_Width (l1.typ);
	     if (Draw_Arc_To_Drive (&l1) == FALSE) return FALSE;
      }
	  
	 break;
     case Ovector:
         if (entities != ENT_ENTITIES) break;
            if (frozen) { if (!vector_frozen((AVECTOR *)nag, frozen - 1)) break; }
            if (!vector_wybrany((AVECTOR *)nag)) break;

            if (Draw_Vector_To_Drive((AVECTOR*)nag, &window_to_print) == FALSE) return FALSE;
        break;
       default :
	 break;
      } /*switch*/
    ObiektTok (&off, offk, &ad, ONieOkreslony);
	
  }

  //zmiana okna oknoS

  if (ptrs__prn_ini_date->inversion == TRUE) 
  {
	  if (ptrs__prn_ini_date->rotation == TRUE)
	  {
		  del_minx = ptrs__prn_ini_date->xp_w;
		  del_miny = ptrs__prn_ini_date->yp_w;
		  del_maxx = ptrs__prn_ini_date->xk_w;
		  del_maxy = ptrs__prn_ini_date->yk_w;
	  }
	  else
	  {
		 del_minx = ptrs__prn_ini_date->xp + (ptrs__prn_ini_date->xk - ptrs__prn_ini_date->xk_w);
		 del_miny = ptrs__prn_ini_date->yp + (ptrs__prn_ini_date->yk - ptrs__prn_ini_date->yk_w);
		 del_maxx = ptrs__prn_ini_date->xk - (ptrs__prn_ini_date->xp_w - ptrs__prn_ini_date->xp);
		 del_maxy = ptrs__prn_ini_date->yk - (ptrs__prn_ini_date->yp_w - ptrs__prn_ini_date->yp);

	  }
  }
  else
  {
	  del_minx = ptrs__prn_ini_date->xp_w;
	  del_miny = ptrs__prn_ini_date->yp_w;
	  del_maxx = ptrs__prn_ini_date->xk_w;
	  del_maxy = ptrs__prn_ini_date->yk_w;
  }

  if (del_minx<window_to_print.x_min)  del_minx=window_to_print.x_min;
  if (del_miny<window_to_print.y_min)  del_miny=window_to_print.y_min;
  if (del_maxx>window_to_print.x_max)  del_maxx=window_to_print.x_max;
  if (del_maxy>window_to_print.y_max)  del_maxy=window_to_print.y_max;

  Set_Draw_Window(del_minx, del_miny, del_maxx, del_maxy);
  set_XY(del_minx, del_miny, del_maxx, del_maxy);


// wyszukiwanie obszarow wydruku

  off=ptrs__prn_ini_date->sheet_adr - dane;
  b_sheet=(BLOK *)ptrs__prn_ini_date->sheet_adr;
  offk=off+sizeof(NAGLOWEK)+b_sheet->n-1;


  ObiektTok (&off, offk, &ad, ONieOkreslony);
  while (ad!=-1)
  {
    nag = (NAGLOWEK*)(dane+ad);
    if (my_kbhit () && my_getch() == ESC)
    {
      while (my_kbhit ())
      {
	     my_getch();
      }
      return FALSE;
    }

    if (pdf_error != 0) return FALSE;

    if (BLOK_SIEC==TRUE)
     {
      if ((long_long)nag>(long_long)block_adr) BLOK_SIEC=FALSE;
     }

      if ((nag->obiekt!=OdBLOK) && (nag->widoczny==0))  //// 19-09-2025  due to gradient solids
      {
          ObiektTok(&off, offk, &ad, ONieOkreslony);
          continue;
      }

    switch(nag->obiekt)
    {
       case OdBLOK :
	   reset_last_polygon_vectors();
       b1=(BLOK *)nag;
       ptrs_desc_bl1	= (T_Desc_Ex_Block *)(&b1->opis_obiektu	[0]) ;

    	   if ((strncmp(ptrs_desc_bl1->sz_type,"*#",2)==0) || (strncmp(ptrs_desc_bl1->sz_type,"*^",2)==0) || //to jest jakas siec
             (strncmp(ptrs_desc_bl1->sz_type,"*~",2)==0) || (strncmp(ptrs_desc_bl1->sz_type,"@kol",4)==0)) //to jest opis lub kolizja
          {
            if (BLOK_SIEC==FALSE)
             {
               BLOK_SIEC=TRUE;
               block_adr=(char *)nag + sizeof(NAGLOWEK) + b1->n - 1;
             }
          }

            if ((b1->kod_obiektu == B_PLINE) && (b1->opis_obiektu[0]== PL_TRACE))
            {
                set_trace_block_begin((char*)nag + sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu);
                set_trace_block_end((char*)nag + sizeof(NAGLOWEK) + b1->n - 1);
                set_trace_block(TRUE);
            }
            else set_trace_block(FALSE);

     break;
       case Olinia :
	  if (entities!=ENT_ENTITIES) break;
	  if (frozen) { if (!linia_frozen((LINIA *)nag, frozen-1)) break; }
	  if (!linia_wybrana ((LINIA *)nag)) break ;

        //edges
        if (((LINIA*)nag)->obiektt2==4) {
            memmove(&V, ((LINIA*)nag), sizeof(LINIA));
            if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_ROLL;
            else V.style = V_EDGE_ROLL_INV;
            Draw_Vector_To_Drive(&V, &window_to_print);
            break;
        }
        else if (((LINIA*)nag)->obiektt2==6) {
            memmove(&V, ((LINIA*)nag), sizeof(LINIA));
            if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_SIMPLE;
            else V.style = V_EDGE_SIMPLE_INV;
            Draw_Vector_To_Drive(&V, &window_to_print);
            break;
        }
        else if (((LINIA*)nag)->obiektt2==7) {
            memmove(&V, ((LINIA*)nag), sizeof(LINIA));
            if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_FIXED;
            else V.style = V_EDGE_FIXED_INV;
            Draw_Vector_To_Drive(&V, &window_to_print);
            break;
        }
	  
	  line_width_type = Line_Width (((LINIA*)nag)->typ);

      if (check_dim_line((LINIA*)nag)==0) {if (Draw_Line_To_Drive ((LINIA*)nag) == FALSE) return FALSE;}
      else {
          ret = split_dim_line((LINIA *) nag, &L_tmp, &L_tmp1);
          switch (ret) {
              case 0:
                  if (Draw_Line_To_Drive((LINIA *) nag) == FALSE) return FALSE;
                  break;
              case 1:
                  if (Draw_Line_To_Drive(&L_tmp) == FALSE) return FALSE;
                  break;
              case 2:
                  if (Draw_Line_To_Drive(&L_tmp) == FALSE) return FALSE;
                  if (Draw_Line_To_Drive(&L_tmp1) == FALSE) return FALSE;
                  break;
              default:
                  break;
          }
      }


	  if (BLOK_SIEC==TRUE)
      {
        memmove(&L,nag,sizeof(LINIA));
        L.kolor=7;
        L.typ=0;
        line_width_type = Line_Width (L.typ);
	     if (Draw_Line_To_Drive (&L) == FALSE) return FALSE;
      }


	  break;
       case Otekst :
	  if (entities!=ENT_ENTITIES) break;
	  if (frozen) { if (!tekst_frozen((TEXT *)nag, frozen-1)) break; }
	  if (!tekst_wybrany_prn ((TEXT*)nag)) break;
	  
	 line_width_type = TEXT_LINE_TYPE; /* LINE_NORM_WIDTH; */

	 if (Draw_Tekst_To_Drive((TEXT*)nag, 0, 1, 1/*, 0*/) == FALSE) return FALSE;
	 

	 if (BLOK_SIEC==TRUE)
      {
        T0=(TEXT *)nag;
        memmove(&T1,T0,sizeof(TEXT));
        strcpy(T1.text,T0->text);
		T1.width = 0;
        T1.kolor=7;
        T1.bold=0;
		if ((T0->bold == 1) && (PTRS__Text_Style[T0->czcionka]->type == 2))
		{
			width_s0 = Get_Text_Len(T0, T0->text, 1.0, 0, NULL, NULL, NULL,  NULL);
			width_s1 = Get_Text_Len(&T1, T1.text, 1.0, 0, NULL, NULL, NULL, NULL);
			ST_W_FACTOR = width_s0 / width_s1;
			T1.width = 0;
			T1.width_factor = T0->width_factor * ST_W_FACTOR;
		}
        line_width_type = LINE_THINEST_WIDTH;
	     if (Draw_Tekst_To_Drive (&T1, 0, 1, 1) == FALSE) return FALSE;
      }
	  
	  break;
       case Okolo :
	  if (entities!=ENT_SOLID) break;
	  if (frozen) { if (!okrag_frozen((OKRAG *)nag, frozen-1)) break; }
	  if (!okrag_wybrany ((OKRAG*)nag)) break ;
	  
	  line_width_type = LINE_MIN_WIDTH ;
	  if (Draw_Fill_Circle_To_Drive ((OKRAG*)nag) == FALSE) return FALSE;

          l.warstwa = ((OKRAG*)nag)->warstwa;
          l.kolor = ((OKRAG*)nag)->kolor;
          //l.typ = ((OKRAG*)nag)->typ;
          l.typ = 0;
          l.x = ((OKRAG*)nag)->x;
          l.y = ((OKRAG*)nag)->y;
          l.kat1 = 0;
          l.kat2 = Pi2;
          l.r = ((OKRAG*)nag)->r;
          if (BLOK_SIEC==TRUE)
           {
             l.kolor=7;
           }
          if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	  break;
        case Opoint:
            if (((T_Point *)nag)->typ>=7)  //junction
            {
                point=(T_Point*)nag;
                if (entities != ENT_ENTITIES) break;
                if (frozen) { if (!point_frozen(point, frozen - 1)) break; }
                if (!Point_Selected(point)) break;

                line_width_type = LINE_MIN_WIDTH;
                if (Draw_Point_To_Drive(point) == FALSE) return FALSE;
            }
            break;
       case Ookrag :
	  if (entities!=ENT_ENTITIES) break;
	  if (frozen) { if (!okrag_frozen((OKRAG *)nag, frozen-1)) break; }
	  if (!okrag_wybrany ((OKRAG*)nag)) break ;
	  
	  line_width_type = Line_Width (((OKRAG*)nag)->typ);
	  l.warstwa = ((OKRAG*)nag)->warstwa;
	  l.kolor = ((OKRAG*)nag)->kolor;
	  l.typ = ((OKRAG*)nag)->typ;
	  l.x = ((OKRAG*)nag)->x;
	  l.y = ((OKRAG*)nag)->y;
	  l.r = ((OKRAG*)nag)->r;
	  l.kat1 = 0;
	  l.kat2 = Pi2;
	  if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	 break;
     case Oellipse :
            if (entities!=ENT_ENTITIES) break;
            if (frozen) { if (!ellipse_frozen((ELLIPSE *)nag, frozen-1)) break; }
            if (!elipsa_wybrana_prec ((ELLIPSE*)nag)) break ;

            line_width_type = Line_Width (((ELLIPSE*)nag)->typ);

            if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
        break;
        case Ofilledellipse :
            if (entities!=ENT_SOLID) break;
            if (frozen) { if (!ellipse_frozen((ELLIPSE *)nag, frozen-1)) break; }
            if (!elipsa_wybrana_prec ((ELLIPSE*)nag)) break ;

            line_width_type = Line_Width (((ELLIPSE*)nag)->typ);

            if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
        break;
        case Oellipticalarc :
            if (entities!=ENT_ENTITIES) break;
            if (frozen) { if (!ellipticalarc_frozen((ELLIPTICALARC *)nag, frozen-1)) break; }
            if (!lukeliptyczny_wybrany  ((ELLIPTICALARC*)nag)) break ;

            line_width_type = Line_Width (((ELLIPTICALARC*)nag)->typ);

            if (Draw_EllipticalArc_To_Drive ((ELLIPTICALARC*)nag) == FALSE) return FALSE;
            break;
       case Owwielokat :
           transluc=255;
            if ((((WIELOKAT*)nag)->translucent) == 1) {
                translucency_ptr = (char*)((WIELOKAT*)nag)->xy;
                translucency_ptr += (((WIELOKAT*)nag)->lp * sizeof(float));
                memmove(&transluc, translucency_ptr,  sizeof(unsigned char));
            }

            if ((entities == ENT_SOLID_NULL) && ((((WIELOKAT*)nag)->empty_typ) != 6) && ((((WIELOKAT*)nag)->empty_typ) != 7)) break;
            if ((entities == ENT_SOLID_TRANSLUCENT) && (((((WIELOKAT*)nag)->empty_typ) != 0) || ((((WIELOKAT*)nag)->pattern) == 1) || ((((WIELOKAT*)nag)->translucent) == 0) || (transluc==255))) break;
            if ((entities == ENT_SOLID) && (((((WIELOKAT*)nag)->empty_typ) != 0) || ((((WIELOKAT*)nag)->pattern) == 1) || (((((WIELOKAT*)nag)->translucent) == 1) && (transluc<255)))) break;
            //if ((entities == ENT_SOLID_PATTERN) && (((((WIELOKAT*)nag)->empty_typ) != 0) || ((((WIELOKAT*)nag)->pattern) != 1))) break;
            if ((entities == ENT_SOLID_PATTERN) && (((((WIELOKAT*)nag)->empty_typ) != 0) || (((((WIELOKAT*)nag)->pattern) != 1) && ((((WIELOKAT*)nag)->gradient) != 1)))) break;
            if ((entities == ENT_ENTITIES) && (((((WIELOKAT*)nag)->empty_typ) == 0))) break;
            if ((entities != ENT_ENTITIES) && (entities != ENT_SOLID_NULL) && (entities != ENT_SOLID) && (entities != ENT_SOLID_TRANSLUCENT) && (entities != ENT_SOLID_PATTERN)) break;
            if ((entities != ENT_SOLID_PATTERN) && (((((WIELOKAT*)nag)->empty_typ) == 0) && ((((WIELOKAT*)nag)->gradient) == 1))) break;

            if (frozen) { if (!wielokat_frozen((WIELOKAT *)nag, frozen-1)) break; }

	  //if (!wielokat_wybrany((WIELOKAT*)nag)) break;
      if (!wielokat_visible_in_frame((WIELOKAT*)nag, (RECTD*)&window_to_print)) break;
	  
	  if (((WIELOKAT*)nag)->empty_typ==0) line_width_type = LINE_MIN_WIDTH ;
       else  line_width_type = Solid_Line_Width (((WIELOKAT*)nag)->empty_typ);

     if ((((WIELOKAT*)nag)->empty_typ)==7) ENABLE_FOUND=TRUE;
     if ((((WIELOKAT*)nag)->empty_typ)==6) NULL_FOUND=TRUE;
	 if ((((WIELOKAT*)nag)->blok) == 0) reset_last_polygon_vectors();
     if (Draw_Wielokat_To_Drive ((WIELOKAT*)nag, &window_to_print) == FALSE) return FALSE;
	 break;
    case Osolidarc :
            transluc=255;
            if ((((SOLIDARC*)nag)->translucent) == 1) transluc=((SOLIDARC*)nag)->translucency;

            if ((entities == ENT_SOLID_NULL) && ((((SOLIDARC*)nag)->empty_typ) != 6) && ((((SOLIDARC*)nag)->empty_typ) != 7)) break;
            if ((entities == ENT_SOLID_TRANSLUCENT) && (((((SOLIDARC*)nag)->empty_typ) != 0) || ((((SOLIDARC*)nag)->pattern) == 1) || ((((SOLIDARC*)nag)->translucent) == 0) || (transluc==255))) break;
            if ((entities == ENT_SOLID) && (((((SOLIDARC*)nag)->empty_typ) != 0) || ((((SOLIDARC*)nag)->pattern) == 1) || (((((SOLIDARC*)nag)->translucent) == 1) && (transluc<255)))) break;
            if ((entities == ENT_SOLID_PATTERN) && (((((SOLIDARC*)nag)->empty_typ) != 0) || ((((SOLIDARC*)nag)->pattern) != 1))) break;
            if ((entities == ENT_ENTITIES) && (((((SOLIDARC*)nag)->empty_typ) == 0))) break;
            if ((entities != ENT_ENTITIES) && (entities != ENT_SOLID_NULL) && (entities != ENT_SOLID) && (entities != ENT_SOLID_TRANSLUCENT) && (entities != ENT_SOLID_PATTERN)) break;

            if (frozen) { if (!solidarc_frozen((SOLIDARC *)nag, frozen-1)) break; }
            if (!solidarc_wybrany((SOLIDARC*)nag)) break;

            if (((SOLIDARC*)nag)->empty_typ==0) line_width_type = LINE_MIN_WIDTH ;
            else  line_width_type = Solid_Line_Width (((SOLIDARC*)nag)->empty_typ);

            if ((((SOLIDARC*)nag)->empty_typ)==7) ENABLE_FOUND=TRUE;
            if ((((SOLIDARC*)nag)->empty_typ)==6) NULL_FOUND=TRUE;
            if ((((SOLIDARC*)nag)->blok) == 0) reset_last_polygon_vectors();
            if (Draw_Solidarc_To_Drive ((SOLIDARC*)nag, &window_to_print) == FALSE) return FALSE;
            break;
	case Ospline:
		if (entities != ENT_ENTITIES) break;
		if (frozen) { if (!spline_frozen((SPLINE *)nag, frozen - 1)) break; }
		if (!spline_wybrany((SPLINE *)nag)) break;

		line_width_type = Line_Width(((SPLINE*)nag)->typ);
		if (Draw_Spline_To_Drive((SPLINE*)nag) == FALSE) return FALSE;

		if (BLOK_SIEC == TRUE)
		{
			memmove(&S, nag, sizeof(SPLINE));
			S.kolor = 7;
			S.typ = 0;
			line_width_type = Line_Width(S.typ);
			if (Draw_Spline_To_Drive(&S) == FALSE) return FALSE;
		}
		break;
    case Opcx:
	if ((entities != ENT_PCX) && (entities != ENT_PCX_SKIN)) break;
	if ((entities == ENT_PCX) && (((B_PCX*)nag)->on_front == 1)) break;
	if ((entities == ENT_PCX_SKIN) && (((B_PCX*)nag)->on_front == 0)) break;
	 if (!pcx_wybrany((B_PCX*)nag, TRUE)) break;
     line_width_type = LINE_MIN_WIDTH ;
	 if (ptrs__prn_ini_date->prn_type == PRN_PDF)
	 {
		 if (Draw_Pcx_To_PDF((B_PCX *)nag) == FALSE) return FALSE;
	 }
	 else
	 {
		 if (Draw_Pcx_To_Drive((B_PCX *)nag, t_drive) == FALSE) return FALSE;
	 }
	 break;
    case Oluk :
	  if (entities!=ENT_ENTITIES) break;
	  if (frozen) { if (!luk_frozen((LUK *)nag, frozen-1)) break; }
	  if(!luk_wybrany_prn ((LUK*)nag)) break;

            //edges
            if (((LUK*)nag)->obiektt2==4) {
                V.warstwa=((LUK*)nag)->warstwa;
                V.kolor=((LUK*)nag)->kolor;
                V.typ=((LUK*)nag)->typ;
                V.x1=((LUK*)nag)->x;
                V.y1=((LUK*)nag)->y;
                V.r=((LUK*)nag)->r;
                V.angle1=((LUK*)nag)->kat1;
                V.angle2=((LUK*)nag)->kat2;
                V.magnitude1=0.0f;
                if (((LUK*)nag)->obiektt3 == 0) V.style = V_EDGE_ARC_ROLL;
                else V.style = V_EDGE_ARC_ROLL_INV;
                Draw_Vector_To_Drive(&V, &window_to_print);
                break;
            }
            else if (((LUK*)nag)->obiektt2==6) {
                V.warstwa=((LUK*)nag)->warstwa;
                V.kolor=((LUK*)nag)->kolor;
                V.typ=((LUK*)nag)->typ;
                V.x1=((LUK*)nag)->x;
                V.y1=((LUK*)nag)->y;
                V.r=((LUK*)nag)->r;
                V.angle1=((LUK*)nag)->kat1;
                V.angle2=((LUK*)nag)->kat2;
                V.magnitude1=0.0f;
                if (((LUK*)nag)->obiektt3 == 0) V.style = V_EDGE_ARC_SIMPLE;
                else V.style = V_EDGE_ARC_SIMPLE_INV;
                Draw_Vector_To_Drive(&V, &window_to_print);
                break;
            }
            else if (((LUK*)nag)->obiektt2==7) {
                V.warstwa=((LUK*)nag)->warstwa;
                V.kolor=((LUK*)nag)->kolor;
                V.typ=((LUK*)nag)->typ;
                V.x1=((LUK*)nag)->x;
                V.y1=((LUK*)nag)->y;
                V.r=((LUK*)nag)->r;
                V.angle1=((LUK*)nag)->kat1;
                V.angle2=((LUK*)nag)->kat2;
                V.magnitude1=0.0f;
                if (((LUK*)nag)->obiektt3 == 0) V.style = V_EDGE_ARC_FIXED;
                else V.style = V_EDGE_ARC_FIXED_INV;

                Draw_Vector_To_Drive(&V, &window_to_print);
                break;
            }
	  
	  line_width_type = Line_Width (((LUK*)nag)->typ);

        ret=split_dim_arc((LUK*)nag, &l_tmp, &l_tmp1);
        switch (ret)
        {
            case 0:
                if (Draw_Arc_To_Drive ((LUK*)nag) == FALSE) return FALSE;
                break;
            case 1:
                if (Draw_Arc_To_Drive (&l_tmp) == FALSE) return FALSE;
                break;
            case 2:
                if (Draw_Arc_To_Drive (&l_tmp) == FALSE) return FALSE;
                if (Draw_Arc_To_Drive (&l_tmp1) == FALSE) return FALSE;
                break;
            default:
                break;
        }

     if (BLOK_SIEC==TRUE)
      {
        memmove(&l1,nag,sizeof(LUK));
        l1.kolor=7;
        l1.typ=0;
        line_width_type = Line_Width (l1.typ);
	     if (Draw_Arc_To_Drive (&l1) == FALSE) return FALSE;
      }
	  
	 break;
        case Ovector:
            if (entities != ENT_ENTITIES) break;
            if (frozen) { if (!vector_frozen((AVECTOR *)nag, frozen - 1)) break; }
            if (!vector_wybrany((AVECTOR *)nag)) break;

            if (Draw_Vector_To_Drive((AVECTOR*)nag, &window_to_print) == FALSE) return FALSE;
            break;
       default :
	 break;
      } /*switch*/
    ObiektTok (&off, offk, &ad, ONieOkreslony);
	
  }

 }

 SHEET_IS_PRINTED=FALSE;

 if (ptrs__prn_ini_date->prn_type != PRN_PDF)
	set_clip_rect(allegro_prn_bmp, matrix_head.left_margin + left_clip, min_y_clip, max_x_clip, max_y_clip);
 else
    SetPDFClip(matrix_head.left_margin + left_clip_pdf, matrix_head.bottom_margin + bottom_clip_pdf, /*right_clip_pdf */ max_x_clip, /*matrix_head.bottom_margin + */ top_clip_pdf);

    off=0;
 offk=dane_size ;

 ObiektTok_No_Superblock (&off, offk, &ad, ONieOkreslony);
 while (ad!=-1)
  {
    nag = (NAGLOWEK*)(dane+ad);
    if (my_kbhit () && my_getch() == ESC)
    {
      while (my_kbhit ())
      {
	 my_getch();
      }
      return FALSE;
    }

    if (pdf_error != 0) return FALSE;

    if (BLOK_SIEC==TRUE)
     {
      if ((long_long)nag>(long_long)block_adr) BLOK_SIEC=FALSE;
     }

      if ((nag->obiekt!=OdBLOK) && (nag->widoczny==0))  //// 19-09-2025  due to gradient solids
      {
          ObiektTok(&off, offk, &ad, ONieOkreslony);
          continue;
      }

    switch(nag->obiekt)
    {
       case OdBLOK :
	   reset_last_polygon_vectors();
       b1=(BLOK *)nag;
       ptrs_desc_bl1	= (T_Desc_Ex_Block *)(&b1->opis_obiektu	[0]) ;

    	   if ((strncmp(ptrs_desc_bl1->sz_type,"*#",2)==0) || (strncmp(ptrs_desc_bl1->sz_type,"*^",2)==0) || //to jest jakas siec
             (strncmp(ptrs_desc_bl1->sz_type,"*~",2)==0) || (strncmp(ptrs_desc_bl1->sz_type,"@kol",4)==0)) //to jest opis lub kolizja
          {
           if (BLOK_SIEC==FALSE)
             {
              BLOK_SIEC=TRUE;
              block_adr=(char *)nag + sizeof(NAGLOWEK) + b1->n - 1;
             }
          }

            if ((b1->kod_obiektu == B_PLINE) && (b1->opis_obiektu[0]== PL_TRACE))
            {
                set_trace_block_begin((char*)nag + sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu);
                set_trace_block_end((char*)nag + sizeof(NAGLOWEK) + b1->n - 1);
                set_trace_block(TRUE);
            }
            else set_trace_block(FALSE);

     break;
       case Olinia :
	  if (entities!=ENT_ENTITIES) break;
	  if (frozen) { if (!linia_frozen((LINIA *)nag, frozen-1)) break; }

        //edges
        if (((LINIA*)nag)->obiektt2==4) {
            memmove(&V, ((LINIA*)nag), sizeof(LINIA));
            if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_ROLL;
            else V.style = V_EDGE_ROLL_INV;
            Draw_Vector_To_Drive(&V, &window_to_print);
            break;
        }
        else if (((LINIA*)nag)->obiektt2==6) {
            memmove(&V, ((LINIA*)nag), sizeof(LINIA));
            if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_SIMPLE;
            else V.style = V_EDGE_SIMPLE_INV;
            Draw_Vector_To_Drive(&V, &window_to_print);
            break;
        }
        else if (((LINIA*)nag)->obiektt2==7) {
            memmove(&V, ((LINIA*)nag), sizeof(LINIA));
            if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_FIXED;
            else V.style = V_EDGE_FIXED_INV;
            Draw_Vector_To_Drive(&V, &window_to_print);
            break;
        }

	  if (!linia_wybrana ((LINIA *)nag)) break ;

	  line_width_type = Line_Width (((LINIA*)nag)->typ);
      if (check_dim_line((LINIA*)nag)==0) {if (Draw_Line_To_Drive ((LINIA*)nag) == FALSE) return FALSE;}
      else {
          ret = split_dim_line((LINIA *) nag, &L_tmp, &L_tmp1);
          switch (ret) {
              case 0:
                  if (Draw_Line_To_Drive((LINIA *) nag) == FALSE) return FALSE;
                  break;
              case 1:
                  if (Draw_Line_To_Drive(&L_tmp) == FALSE) return FALSE;
                  break;
              case 2:
                  if (Draw_Line_To_Drive(&L_tmp) == FALSE) return FALSE;
                  if (Draw_Line_To_Drive(&L_tmp1) == FALSE) return FALSE;
                  break;
              default:
                  break;
          }
      }

	  if (BLOK_SIEC==TRUE)
      {
        memmove(&L,nag,sizeof(LINIA));
        L.kolor=7;
        L.typ=0;
        line_width_type = Line_Width (L.typ);
	     if (Draw_Line_To_Drive (&L) == FALSE) return FALSE;
      }
	  break;
       case Otekst :
	  if (entities!=ENT_ENTITIES) break;
	  if (frozen) { if (!tekst_frozen((TEXT *)nag, frozen-1)) break; }

	  if (!tekst_wybrany_prn((TEXT*)nag)) break;  

	  line_width_type = TEXT_LINE_TYPE; /* LINE_NORM_WIDTH; */

	  if (Draw_Tekst_To_Drive((TEXT*)nag, 0, 1, 1/*, 0*/) == FALSE) return FALSE;

	  if (BLOK_SIEC==TRUE)
      {
        T0=(TEXT *)nag;
        memmove(&T1,T0,sizeof(TEXT));
        strcpy(T1.text,T0->text);
		T1.width = 0;
        T1.kolor=7;
        T1.bold=0;
		if ((T0->bold == 1) && (PTRS__Text_Style[T0->czcionka]->type == 2))
		{
			width_s0 = Get_Text_Len(T0, T0->text, 1.0, 0, NULL, NULL, NULL, NULL);
			width_s1 = Get_Text_Len(&T1, T1.text, 1.0, 0, NULL, NULL, NULL, NULL);
			ST_W_FACTOR = width_s0 / width_s1;
			T1.width = 0;
			T1.width_factor = T0->width_factor * ST_W_FACTOR;
		}
        line_width_type = LINE_THINEST_WIDTH;
	     if (Draw_Tekst_To_Drive (&T1, 0, 1, 1) == FALSE) return FALSE;
      }
	  
	  break;
       case Okolo :
	  if (entities!=ENT_SOLID) break;
	  if (frozen) { if (!okrag_frozen((OKRAG *)nag, frozen-1)) break; }
	  if (!okrag_wybrany ((OKRAG*)nag)) break ;
	  
	  line_width_type = LINE_MIN_WIDTH ;
	  if (Draw_Fill_Circle_To_Drive ((OKRAG*)nag) == FALSE) return FALSE;

          l.warstwa = ((OKRAG*)nag)->warstwa;
          l.kolor = ((OKRAG*)nag)->kolor;
          //l.typ = ((OKRAG*)nag)->typ;
          l.typ = 0;
          l.x = ((OKRAG*)nag)->x;
          l.y = ((OKRAG*)nag)->y;
          l.kat1 = 0;
          l.kat2 = Pi2;
          l.r = ((OKRAG*)nag)->r;
          if (BLOK_SIEC==TRUE)
           {
             l.kolor=7;
           }
          if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	  break;
	   case Opoint:
		   if (((T_Point *)nag)->typ>=7)  //junction
		   {
               point=(T_Point*)nag;
			   if (entities != ENT_ENTITIES) break;
			   if (frozen) { if (!point_frozen(point, frozen - 1)) break; }
			   if (!Point_Selected(point)) break;

			   line_width_type = LINE_MIN_WIDTH;
			   if (Draw_Point_To_Drive(point) == FALSE) return FALSE;
		   }
		   break;
       case Ookrag :
	  if (entities!=ENT_ENTITIES) break;
	  if (frozen) { if (!okrag_frozen((OKRAG *)nag, frozen-1)) break; }
	  if (!okrag_wybrany ((OKRAG*)nag)) break ;
	  
	  line_width_type = Line_Width (((OKRAG*)nag)->typ);
	  l.warstwa = ((OKRAG*)nag)->warstwa;
	  l.kolor = ((OKRAG*)nag)->kolor;
	  l.typ = ((OKRAG*)nag)->typ;
	  l.x = ((OKRAG*)nag)->x;
	  l.y = ((OKRAG*)nag)->y;
	  l.r = ((OKRAG*)nag)->r;
	  l.kat1 = 0;
	  l.kat2 = Pi2;
	  if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	 break;
     case Oellipse :
            if (entities!=ENT_ENTITIES) break;
            if (frozen) { if (!ellipse_frozen((ELLIPSE *)nag, frozen-1)) break; }
            if (!elipsa_wybrana_prec ((ELLIPSE*)nag)) break ;

            line_width_type = Line_Width (((ELLIPSE*)nag)->typ);

            if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
        break;
        case Ofilledellipse :
            if (entities!=ENT_SOLID) break;
            if (frozen) { if (!ellipse_frozen((ELLIPSE *)nag, frozen-1)) break; }
            if (!elipsa_wybrana_prec ((ELLIPSE*)nag)) break ;

            line_width_type = Line_Width (((ELLIPSE*)nag)->typ);

            if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
        break;
        case Oellipticalarc :
            if (entities!=ENT_ENTITIES) break;
            if (frozen) { if (!ellipticalarc_frozen((ELLIPTICALARC *)nag, frozen-1)) break; }
            if (!lukeliptyczny_wybrany  ((ELLIPTICALARC*)nag)) break ;

            line_width_type = Line_Width (((ELLIPTICALARC*)nag)->typ);

            if (Draw_EllipticalArc_To_Drive ((ELLIPTICALARC*)nag) == FALSE) return FALSE;
            break;
       case Owwielokat :
           transluc=255;
           if ((((WIELOKAT*)nag)->translucent) == 1) {
               translucency_ptr = (char*)((WIELOKAT*)nag)->xy;
               translucency_ptr += (((WIELOKAT*)nag)->lp * sizeof(float));
               memmove(&transluc, translucency_ptr,  sizeof(unsigned char));
           }

            if ((entities == ENT_SOLID_NULL) && ((((WIELOKAT*)nag)->empty_typ) != 6) && ((((WIELOKAT*)nag)->empty_typ) != 7)) break;
            if ((entities == ENT_SOLID_TRANSLUCENT) && (((((WIELOKAT*)nag)->empty_typ) != 0) || ((((WIELOKAT*)nag)->pattern) == 1) || ((((WIELOKAT*)nag)->translucent) == 0) || (transluc==255))) break;
            if ((entities == ENT_SOLID) && (((((WIELOKAT*)nag)->empty_typ) != 0) || ((((WIELOKAT*)nag)->pattern) == 1) || (((((WIELOKAT*)nag)->translucent) == 1) && (transluc<255)))) break;
            //if ((entities == ENT_SOLID_PATTERN) && (((((WIELOKAT*)nag)->empty_typ) != 0) || ((((WIELOKAT*)nag)->pattern) != 1))) break;
            if ((entities == ENT_SOLID_PATTERN) && (((((WIELOKAT*)nag)->empty_typ) != 0) || (((((WIELOKAT*)nag)->pattern) != 1) && ((((WIELOKAT*)nag)->gradient) != 1)))) break;
            if ((entities == ENT_ENTITIES) && (((((WIELOKAT*)nag)->empty_typ) == 0))) break;
            if ((entities != ENT_ENTITIES) && (entities != ENT_SOLID_NULL) && (entities != ENT_SOLID) && (entities != ENT_SOLID_TRANSLUCENT) && (entities != ENT_SOLID_PATTERN)) break;
            if ((entities != ENT_SOLID_PATTERN) && (((((WIELOKAT*)nag)->empty_typ) == 0) && ((((WIELOKAT*)nag)->gradient) == 1))) break;

            if (frozen) { if (!wielokat_frozen((WIELOKAT *)nag, frozen-1)) break; }

	  //if (!wielokat_wybrany((WIELOKAT*)nag)) break;
      if (!wielokat_visible_in_frame((WIELOKAT*)nag, (RECTD*)&window_to_print)) break;

	  if (((WIELOKAT*)nag)->empty_typ==0) line_width_type = LINE_MIN_WIDTH ;
       else  line_width_type = Solid_Line_Width (((WIELOKAT*)nag)->empty_typ);

     if ((((WIELOKAT*)nag)->empty_typ)!=7) //nie uwzglednia sie obszarow zaznaczonych
                                           //do wydruku jezeli nie sa w bloku sheet
     {
       if ((((WIELOKAT*)nag)->empty_typ)==6) NULL_FOUND=TRUE;
	   if ((((WIELOKAT*)nag)->blok) == 0) reset_last_polygon_vectors();
	    if (Draw_Wielokat_To_Drive ((WIELOKAT*)nag, &window_to_print) == FALSE) return FALSE;
     }
	 break;
        case Osolidarc :
            transluc=255;
            if ((((SOLIDARC*)nag)->translucent) == 1) transluc=((SOLIDARC*)nag)->translucency;

            if ((entities == ENT_SOLID_NULL) && ((((SOLIDARC*)nag)->empty_typ) != 6) && ((((SOLIDARC*)nag)->empty_typ) != 7)) break;
            if ((entities == ENT_SOLID_TRANSLUCENT) && (((((SOLIDARC*)nag)->empty_typ) != 0) || ((((SOLIDARC*)nag)->pattern) == 1) || ((((SOLIDARC*)nag)->translucent) == 0) || (transluc==255))) break;
            if ((entities == ENT_SOLID) && (((((SOLIDARC*)nag)->empty_typ) != 0) || ((((SOLIDARC*)nag)->pattern) == 1) || (((((SOLIDARC*)nag)->translucent) == 1) && (transluc<255)))) break;
            if ((entities == ENT_SOLID_PATTERN) && (((((SOLIDARC*)nag)->empty_typ) != 0) || ((((SOLIDARC*)nag)->pattern) != 1))) break;
            if ((entities == ENT_ENTITIES) && (((((SOLIDARC*)nag)->empty_typ) == 0))) break;
            if ((entities != ENT_ENTITIES) && (entities != ENT_SOLID_NULL) && (entities != ENT_SOLID) && (entities != ENT_SOLID_TRANSLUCENT) && (entities != ENT_SOLID_PATTERN)) break;

            if (frozen) { if (!solidarc_frozen((SOLIDARC *)nag, frozen-1)) break; }
            if (!solidarc_wybrany((SOLIDARC*)nag)) break;

            if (((SOLIDARC*)nag)->empty_typ==0) line_width_type = LINE_MIN_WIDTH ;
            else  line_width_type = Solid_Line_Width (((SOLIDARC*)nag)->empty_typ);

            if ((((SOLIDARC*)nag)->empty_typ)!=7) //nie uwzglednia sie obszarow zaznaczonych
                //do wydruku jezeli nie sa w bloku sheet
            {
                if ((((SOLIDARC*)nag)->empty_typ)==6) NULL_FOUND=TRUE;
                if ((((SOLIDARC*)nag)->blok) == 0) reset_last_polygon_vectors();
                if (Draw_Solidarc_To_Drive ((SOLIDARC*)nag, &window_to_print) == FALSE) return FALSE;
            }
            break;
	case Ospline:
		
		if (entities != ENT_ENTITIES) break;
		if (frozen) { if (!spline_frozen((SPLINE *)nag, frozen - 1)) break; }
		if (!spline_wybrany((SPLINE *)nag)) break;

		line_width_type = Line_Width(((SPLINE*)nag)->typ);
		if (Draw_Spline_To_Drive((SPLINE*)nag) == FALSE) return FALSE;
		
		if (BLOK_SIEC == TRUE)
		{
			memmove(&S, nag, sizeof(SPLINE));
			S.kolor = 7;
			S.typ = 0;
			line_width_type = Line_Width(S.typ);
			if (Draw_Spline_To_Drive(&S) == FALSE) return FALSE;
		}
		
		break;
    case Opcx:
		if ((entities != ENT_PCX) && (entities != ENT_PCX_SKIN)) break;
		if ((entities == ENT_PCX) && (((B_PCX*)nag)->on_front == 1)) break;
		if ((entities == ENT_PCX_SKIN) && (((B_PCX*)nag)->on_front == 0)) break;
     line_width_type = LINE_MIN_WIDTH ;

	 if (ptrs__prn_ini_date->prn_type == PRN_PDF)
	 {
		 if ((entities == ENT_PCX_SKIN) && (((B_PCX*)nag)->on_front == 1))
		 {
			 if (Draw_Pcx_To_PDF((B_PCX*)nag) == FALSE) return FALSE;
		 }
	 }
	 else
	 {
		 if (Draw_Pcx_To_Drive((B_PCX *)nag, t_drive) == FALSE) return FALSE;
	 }
	 break;
       case Oluk :
	  if (entities!=ENT_ENTITIES) break;
	  if (frozen) { if (!luk_frozen((LUK *)nag, frozen-1)) break; }
	  if(!luk_wybrany_prn ((LUK*)nag)) break;

            //edges
            if (((LUK*)nag)->obiektt2==4) {
                V.warstwa=((LUK*)nag)->warstwa;
                V.kolor=((LUK*)nag)->kolor;
                V.typ=((LUK*)nag)->typ;
                V.x1=((LUK*)nag)->x;
                V.y1=((LUK*)nag)->y;
                V.r=((LUK*)nag)->r;
                V.angle1=((LUK*)nag)->kat1;
                V.angle2=((LUK*)nag)->kat2;
                V.magnitude1=0.0f;
                if (((LUK*)nag)->obiektt3 == 0) V.style = V_EDGE_ARC_ROLL;
                else V.style = V_EDGE_ARC_ROLL_INV;
                Draw_Vector_To_Drive(&V, &window_to_print);
                break;
            }
            else if (((LUK*)nag)->obiektt2==6) {
                V.warstwa=((LUK*)nag)->warstwa;
                V.kolor=((LUK*)nag)->kolor;
                V.typ=((LUK*)nag)->typ;
                V.x1=((LUK*)nag)->x;
                V.y1=((LUK*)nag)->y;
                V.r=((LUK*)nag)->r;
                V.angle1=((LUK*)nag)->kat1;
                V.angle2=((LUK*)nag)->kat2;
                V.magnitude1=0.0f;
                if (((LUK*)nag)->obiektt3 == 0) V.style = V_EDGE_ARC_SIMPLE;
                else V.style = V_EDGE_ARC_SIMPLE_INV;
                Draw_Vector_To_Drive(&V, &window_to_print);
                break;
            }
            else if (((LUK*)nag)->obiektt2==7) {
                V.warstwa=((LUK*)nag)->warstwa;
                V.kolor=((LUK*)nag)->kolor;
                V.typ=((LUK*)nag)->typ;
                V.x1=((LUK*)nag)->x;
                V.y1=((LUK*)nag)->y;
                V.r=((LUK*)nag)->r;
                V.angle1=((LUK*)nag)->kat1;
                V.angle2=((LUK*)nag)->kat2;
                V.magnitude1=0.0f;
                if (((LUK*)nag)->obiektt3 == 0) V.style = V_EDGE_ARC_FIXED;
                else V.style = V_EDGE_ARC_FIXED_INV;

                Draw_Vector_To_Drive(&V, &window_to_print);
                break;
            }
	  
	  line_width_type = Line_Width (((LUK*)nag)->typ);

        ret=split_dim_arc((LUK*)nag, &l_tmp, &l_tmp1);
        switch (ret)
        {
            case 0:
                if (Draw_Arc_To_Drive ((LUK*)nag) == FALSE) return FALSE;
                break;
            case 1:
                if (Draw_Arc_To_Drive (&l_tmp) == FALSE) return FALSE;
                break;
            case 2:
                if (Draw_Arc_To_Drive (&l_tmp) == FALSE) return FALSE;
                if (Draw_Arc_To_Drive (&l_tmp1) == FALSE) return FALSE;
                break;
            default:
                break;
        }

     if (BLOK_SIEC==TRUE)
      {
        memmove(&l1,nag,sizeof(LUK));
        l1.kolor=7;
        l1.typ=0;
        line_width_type = Line_Width (l1.typ);
	     if (Draw_Arc_To_Drive (&l1) == FALSE) return FALSE;
      }
	  
	 break;
        case Ovector:
            if (entities != ENT_ENTITIES) break;
            if (frozen) { if (!vector_frozen((AVECTOR *)nag, frozen - 1)) break; }
            if (!vector_wybrany((AVECTOR *)nag)) break;

            if (Draw_Vector_To_Drive((AVECTOR*)nag, &window_to_print) == FALSE) return FALSE;
            break;
       default :
	 break;
      } /*switch*/
    ObiektTok_No_Superblock (&off, offk, &ad, ONieOkreslony);
  }
  return TRUE;
}


static BOOL draw_to_matrix1____(Print_Rect window_to_print)   //TO REMOVE
/*-------------------------------------------------------*/
{
 NAGLOWEK *nag;
 LUK l = ldef, l1;
 LINIA L;
 TEXT T1, *T0;
 ELLIPSE e, fe;
 ELLIPTICALARC ea;
 T_Point *point;
 long_long off, offk, ad;
 int k;
 BOOL b_err=TRUE;
 char *block_adr;
 BLOK *b1;
 T_Desc_Ex_Block *ptrs_desc_bl1;
 double width_s0, width_s1;
 AVECTOR V;

 BLOK_SIEC=FALSE;

 if (ptrs__prn_ini_date->inversion == TRUE) print_inversion=TRUE;
  else print_inversion=FALSE;
 if (ptrs__prn_ini_date->rotation == TRUE) print_rotation = TRUE;
 else print_rotation = FALSE;

 
 if (strlen(zbior_danych_2)==0) return TRUE;
 zwolnienie_pamieci();
 k = czytaj_zbior(zbior_danych_2, b_err, TRUE, FALSE);
  if (k)
   {
    if (k == 1 && b_err == TRUE)
    {
     return FALSE;
    }
   }

 off=0;
 offk=dane_size;

 if (ptrs__prn_ini_date->rotation == TRUE)
 {
    if (ptrs__prn_ini_date->inversion == TRUE)
    oknoS_ (FormatX - matrix_head.xp - matrix_head.height - line_max_h_width,
	        FormatY - matrix_head.yp - matrix_head.width - line_max_h_width,
	        FormatX - matrix_head.xp + line_max_h_width,
	        FormatY - matrix_head.yp + line_max_h_width) ;
    else
     oknoS_ (matrix_head.xp - line_max_h_width,
	   matrix_head.yp - line_max_h_width,
	   matrix_head.xp + matrix_head.height + line_max_h_width,
	   matrix_head.yp + matrix_head.width + line_max_h_width) ;
 }
 else
 {
  if (ptrs__prn_ini_date->inversion == TRUE)
   oknoS_ (FormatX - matrix_head.xp - matrix_head.width - line_max_h_width,
	       FormatY - matrix_head.yp - matrix_head.height - line_max_h_width,
	       FormatX - matrix_head.xp + line_max_h_width,
	       FormatY - matrix_head.yp + line_max_h_width) ;
	   
  else
   oknoS_ (matrix_head.xp - line_max_h_width,
	  matrix_head.yp - line_max_h_width,
	  matrix_head.xp + matrix_head.width + line_max_h_width,
	  matrix_head.yp + matrix_head.height + line_max_h_width) ;
 }
 ObiektTok (&off, offk, &ad, ONieOkreslony);
 while (ad!=-1)
 {
    nag = (NAGLOWEK*)(dane+ad);

    if (BLOK_SIEC==TRUE)
     {
      if ((long_long)nag>(long_long)block_adr) BLOK_SIEC=FALSE;
     }

    if (my_kbhit () && my_getch() == ESC)
    {
      while (my_kbhit ())
      {
	 my_getch();
      }
      return FALSE;
    }

     if ((nag->obiekt!=OdBLOK) && (nag->widoczny==0))  //// 19-09-2025  due to gradient solids
     {
         ObiektTok(&off, offk, &ad, ONieOkreslony);
         continue;
     }

    switch(nag->obiekt)
    {
      case OdBLOK :
       b1=(BLOK *)nag;
       ptrs_desc_bl1	= (T_Desc_Ex_Block *)(&b1->opis_obiektu	[0]) ;

    	   if ((strncmp(ptrs_desc_bl1->sz_type,"*#",2)==0) || (strncmp(ptrs_desc_bl1->sz_type,"*^",2)==0) || //to jest jakas siec
             (strncmp(ptrs_desc_bl1->sz_type,"*~",2)==0) || (strncmp(ptrs_desc_bl1->sz_type,"@kol",4)==0)) //to jest opis lub kolizja
          {
           if (BLOK_SIEC==FALSE)
             {
              BLOK_SIEC=TRUE;
              block_adr=(char *)nag + sizeof(NAGLOWEK) + b1->n - 1;
             }
          }

            if ((b1->kod_obiektu == B_PLINE) && (b1->opis_obiektu[0]== PL_TRACE))
            {
                set_trace_block_begin((char*)nag + sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu);
                set_trace_block_end((char*)nag + sizeof(NAGLOWEK) + b1->n - 1);
                set_trace_block(TRUE);
            }
            else set_trace_block(FALSE);

     break;
       case Olinia :
	   if (!linia_wybrana ((LINIA *)nag)) break ;   //!!!!!!!!!!!!!

        //edges
        if (((LINIA*)nag)->obiektt2==4) {
            memmove(&V, ((LINIA*)nag), sizeof(LINIA));
            if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_ROLL;
            else V.style = V_EDGE_ROLL_INV;
            Draw_Vector_To_Drive(&V, &window_to_print);
            break;
        }
        else if (((LINIA*)nag)->obiektt2==6) {
            memmove(&V, ((LINIA*)nag), sizeof(LINIA));
            if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_SIMPLE;
            else V.style = V_EDGE_SIMPLE_INV;
            Draw_Vector_To_Drive(&V, &window_to_print);
            break;
        }
        else if (((LINIA*)nag)->obiektt2==7) {
            memmove(&V, ((LINIA*)nag), sizeof(LINIA));
            if (((LINIA*)nag)->obiektt3 == 0) V.style = V_EDGE_FIXED;
            else V.style = V_EDGE_FIXED_INV;
            Draw_Vector_To_Drive(&V, &window_to_print);
            break;
        }
	  
	  line_width_type = Line_Width (((LINIA*)nag)->typ);
	  if (Draw_Line_To_Drive ((LINIA*)nag) == FALSE) return FALSE;

	  if (BLOK_SIEC==TRUE)
      {
        memmove(&L,nag,sizeof(LINIA));
        L.kolor=7;
        L.typ=0;
        line_width_type = Line_Width (L.typ);
	     if (Draw_Line_To_Drive (&L) == FALSE) return FALSE;
      }
	  break;
       case Otekst :
	  if (!tekst_wybrany_prn ((TEXT*)nag)) break;
     if (BLOK_SIEC==TRUE)
      {
        T0=(TEXT *)nag;
        memmove(&T1,T0,sizeof(TEXT));
        strcpy(T1.text,T0->text);
		T1.width = 0;
        T1.kolor=7;
        T1.bold=0;
		if ((T0->bold == 1) && (PTRS__Text_Style[T0->czcionka]->type == 2))
		{
			width_s0 = Get_Text_Len(T0, T0->text, 1.0, 0, NULL, NULL, NULL, NULL);
			width_s1 = Get_Text_Len(&T1, T1.text, 1.0, 0, NULL, NULL, NULL, NULL);
			ST_W_FACTOR = width_s0 / width_s1;
			T1.width = 0;
			T1.width_factor = T0->width_factor * ST_W_FACTOR;
		}
        line_width_type = LINE_THINEST_WIDTH;;
	     if (Draw_Tekst_To_Drive (&T1, 0, 1, 1) == FALSE) return FALSE;
      }
	  line_width_type = TEXT_LINE_TYPE; /* LINE_NORM_WIDTH; */

	  if (Draw_Tekst_To_Drive((TEXT*)nag, 0, 1, 1/*, 0*/) == FALSE) return FALSE;
	  
	  break;
       case Okolo :
	  if (!okrag_wybrany ((OKRAG*)nag)) break ;
	  line_width_type = LINE_MIN_WIDTH ;
	  if (Draw_Fill_Circle_To_Drive ((OKRAG*)nag) == FALSE) return FALSE;
	  l.warstwa = ((OKRAG*)nag)->warstwa;
          l.kolor = ((OKRAG*)nag)->kolor;
          l.typ = 0;
          l.x = ((OKRAG*)nag)->x;
          l.y = ((OKRAG*)nag)->y;
          l.kat1 = 0;
          l.kat2 = Pi2;
          l.r = ((OKRAG*)nag)->r;
          if (BLOK_SIEC==TRUE)
           {
             l.kolor=7;
           }
          if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	  break ;
        case Opoint:
            if (((T_Point *)nag)->typ>=7)  //junction
            {
                point=(T_Point*)nag;
                if (!Point_Selected(point)) break;

                line_width_type = LINE_MIN_WIDTH;
                if (Draw_Point_To_Drive(point) == FALSE) return FALSE;
            }
            break;
       case Ookrag :
	  if (!okrag_wybrany ((OKRAG*)nag)) break ;
	  line_width_type = Line_Width (((OKRAG*)nag)->typ);
	  l.warstwa = ((OKRAG*)nag)->warstwa;
	  l.kolor = ((OKRAG*)nag)->kolor;
	  l.typ = ((OKRAG*)nag)->typ;
	  l.x = ((OKRAG*)nag)->x;
	  l.y = ((OKRAG*)nag)->y;
	  l.r = ((OKRAG*)nag)->r;
	  l.kat1 = 0;
	  l.kat2 = Pi2;
	  if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;
	 break;
     case Oellipse :
            if (!elipsa_wybrana_prec ((ELLIPSE*)nag)) break ;

            line_width_type = Line_Width (((ELLIPSE*)nag)->typ);

            if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
        break;
        case Ofilledellipse :
            if (!elipsa_wybrana_prec ((ELLIPSE*)nag)) break ;

            line_width_type = Line_Width (((ELLIPSE*)nag)->typ);

            if (Draw_Ellipse_To_Drive ((ELLIPSE*)nag) == FALSE) return FALSE;
            break;
        case Oellipticalarc :
            if (!lukeliptyczny_wybrany  ((ELLIPTICALARC*)nag)) break ;

            line_width_type = Line_Width (((ELLIPTICALARC*)nag)->typ);

            if (Draw_EllipticalArc_To_Drive ((ELLIPTICALARC*)nag) == FALSE) return FALSE;
            break;
       case Owwielokat :
	  if (!wielokat_wybrany((WIELOKAT*)nag)) break;
	  line_width_type = LINE_MIN_WIDTH ;
	  if (Draw_Wielokat_To_Drive ((WIELOKAT*)nag, &window_to_print) == FALSE) return FALSE;
	 break;
        case Osolidarc :
            if (!solidarc_wybrany((SOLIDARC*)nag)) break;
            line_width_type = LINE_MIN_WIDTH ;
            if (Draw_Solidarc_To_Drive ((SOLIDARC*)nag, &window_to_print) == FALSE) return FALSE;
            break;
       case Oluk :
	  if(!luk_wybrany_prn ((LUK*)nag)) break;

            //edges
            if (((LUK*)nag)->obiektt2==4) {
                V.warstwa=((LUK*)nag)->warstwa;
                V.kolor=((LUK*)nag)->kolor;
                V.typ=((LUK*)nag)->typ;
                V.x1=((LUK*)nag)->x;
                V.y1=((LUK*)nag)->y;
                V.r=((LUK*)nag)->r;
                V.angle1=((LUK*)nag)->kat1;
                V.angle2=((LUK*)nag)->kat2;
                V.magnitude1=0.0f;
                if (((LUK*)nag)->obiektt3 == 0) V.style = V_EDGE_ARC_ROLL;
                else V.style = V_EDGE_ARC_ROLL_INV;
                Draw_Vector_To_Drive(&V, &window_to_print);
                break;
            }
            else if (((LUK*)nag)->obiektt2==6) {
                V.warstwa=((LUK*)nag)->warstwa;
                V.kolor=((LUK*)nag)->kolor;
                V.typ=((LUK*)nag)->typ;
                V.x1=((LUK*)nag)->x;
                V.y1=((LUK*)nag)->y;
                V.r=((LUK*)nag)->r;
                V.angle1=((LUK*)nag)->kat1;
                V.angle2=((LUK*)nag)->kat2;
                V.magnitude1=0.0f;
                if (((LUK*)nag)->obiektt3 == 0) V.style = V_EDGE_ARC_SIMPLE;
                else V.style = V_EDGE_ARC_SIMPLE_INV;
                Draw_Vector_To_Drive(&V, &window_to_print);
                break;
            }
            else if (((LUK*)nag)->obiektt2==7) {
                V.warstwa=((LUK*)nag)->warstwa;
                V.kolor=((LUK*)nag)->kolor;
                V.typ=((LUK*)nag)->typ;
                V.x1=((LUK*)nag)->x;
                V.y1=((LUK*)nag)->y;
                V.r=((LUK*)nag)->r;
                V.angle1=((LUK*)nag)->kat1;
                V.angle2=((LUK*)nag)->kat2;
                V.magnitude1=0.0f;
                if (((LUK*)nag)->obiektt3 == 0) V.style = V_EDGE_ARC_FIXED;
                else V.style = V_EDGE_ARC_FIXED_INV;

                Draw_Vector_To_Drive(&V, &window_to_print);
                break;
            }

     if (BLOK_SIEC==TRUE)
      {
        memmove(&l1,nag,sizeof(LUK));
        l1.kolor=7;
        l1.typ=0;
        line_width_type = Line_Width (l1.typ);
	     if (Draw_Arc_To_Drive (&l1) == FALSE) return FALSE;
      }
	  line_width_type = Line_Width (((LUK*)nag)->typ);
	  if (Draw_Arc_To_Drive ((LUK*)nag) == FALSE) return FALSE;
	 break;
        case Ovector:
            if (!vector_wybrany((AVECTOR *)nag)) break;

            if (Draw_Vector_To_Drive((AVECTOR*)nag, &window_to_print) == FALSE) return FALSE;
            break;
       default :
	 break;
      } /*switch*/
    ObiektTok (&off, offk, &ad, ONieOkreslony);
  }
  return TRUE;
}


static BOOL draw_matrix_to_file (void)
/*-----------------------------------*/
{

  if ((ptrs__prn_ini_date->prn_type== PRN_PCX)
#ifdef LINUX
    || (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)
#endif
    )
  {
	correct_prn_bitmap_colors(allegro_prn_bmp);
	return draw_matrix_to_file_BMP();
  }

  else if (ptrs__prn_ini_date->prn_type== PRN_WINDOWS)
  {
    return draw_matrix_to_file_WINDOWS ();
  }

  else if (ptrs__prn_ini_date->prn_type == PRN_PDF)
  {
	  return TRUE;
  }

  else return FALSE;
}

static BOOL write_laser_command (char *ptrsz_str1, char *ptrsz_str2, long val)
/*---------------------------------------------------------------------------*/
{
  char sz_buf [10] ;

  sprintf(sz_buf,"%#ld",val); 
  if (Write_To_Device (ptrsz_str1, strlen (ptrsz_str1)) != strlen (ptrsz_str1)) return FALSE ;
  if (Write_To_Device (sz_buf, strlen (sz_buf)) != strlen (sz_buf)) return FALSE ;
  if (Write_To_Device (ptrsz_str2, strlen (ptrsz_str2)) != strlen (ptrsz_str2)) return FALSE ;
  return TRUE ;
}

static long inch_to_decipoints (double df_inch)
/*---------------------------------------------*/
{
  return (long) floor (.5 + df_inch * 720) ;
}

static BOOL draw_header_PCX(double xp, double xk, double yp, double yk)
/*------------------------------------------------------------------*/
{

  PCXheader head;
  int width, height;

  width = abs(jednostki_x_to_byte_prn(xk - xp) * 8);
  height = abs(jednostki_y_to_byte_prn(yp- yk) * 8);

  head.manufacturer=10;
  head.version=5;
  head.encoding=1;
  head.bits_per_pixel=8;
  head.xmin=0;
  head.ymin=0;
  head.xmax=width-1;
  head.ymax=height-1;
  head.horz_res=width;
  head.vert_res=height;
  head.num_color_planes=1;
  head.bytes_per_line=width;
  head.palette_type=1;

  if (Write_To_Device (&head, sizeof(PCXheader)) !=  sizeof(PCXheader)) return FALSE ;
  return TRUE;
}


static void draw_pallete_PCX(void)
/*-----------------------------*/
{ int i;
  unsigned char byte1;

  byte1= 0x0C;
  if (Write_To_Device (&byte1, sizeof(char)) != sizeof(char)) return ;

  for(i=0;i<256;i++)			// save the palette
  {
    byte1=_dac_normal[i][0]<<2;
    if (Write_To_Device (&byte1, sizeof(char)) != sizeof(char)) return ;
    byte1=_dac_normal[i][1]<<2;
    if (Write_To_Device (&byte1, sizeof(char)) != sizeof(char)) return ;
    byte1=_dac_normal[i][2]<<2;
    if (Write_To_Device (&byte1, sizeof(char)) != sizeof(char)) return ;
  }
}


static BOOL draw_matrix_to_file_PCX (void)
/*--------------------------------------*/
{
	int nx_byte, ny_byte, n_row; // j;
  char *CM = matrix_head.command_mode;
  char  *ptrhsz_buf_PCX ;
  int nx_byte1, nx_byte_PCX;
  int count=0;
  int count_r=0;
  int i_row;
  unsigned char key;
  unsigned char key2;
  long total;
  int runcount;

  nx_byte = matrix_head.width_byte ;
  nx_byte_PCX = matrix_head.width_byte * 8;
  ny_byte = jednostki_y_to_byte_prn (matrix_head.height) ;
  n_row = ny_byte * 8 ;

   ptrhsz_buf_PCX = matrix_head.lp_matrix_PCX ;  //adres matrycy PCX

  if (set_left_margin==TRUE)
   {
    ;
   }

   total=nx_byte_PCX * n_row ;

   count= 0;
   count_r=0;
   i_row=0;
   nx_byte1=0;

   while(count < total)
   {
    runcount = 0;       // reset some variables...
    key = ptrhsz_buf_PCX[count_r];         //!!!!!!!!!!!!!!!!!!!!!!!
    key2= key;

    while(key == key2 && runcount < 63) // check to see how long this "run"
    {
      runcount++;
      count++;
      count_r++;
      if (count_r==nx_byte_PCX)
       {
         i_row++;
       }

      key2=ptrhsz_buf_PCX[count_r];  //!!!!!!!!!!!!!!!!!!
      if(count % nx_byte_PCX == 0) break;             // "runs" cannot go from one
    }                                           //  row to another.
    if(runcount == 1)   // if it isn't a "run"
     {
      if(key >= 0xC0)   // save either a "run" of length 1...
      {
        bufor_wydruku[nx_byte1]=0xC1 ;
        nx_byte1++;
        bufor_wydruku[nx_byte1]=key ;
        nx_byte1++;
       }
      else
       {
         bufor_wydruku[nx_byte1]=key ;
         nx_byte1++;
        }
     }
    else                // if it is a "run", save the "runlength" and then
    {                   //  save the color index number
      bufor_wydruku[nx_byte1]=runcount + 0xC0 ;
      nx_byte1++;
      bufor_wydruku[nx_byte1]=key ;
      nx_byte1++;
    }
    if (nx_byte1 > 9000)
      {
        if (Write_To_Device (&bufor_wydruku[0], (nx_byte1)) != (nx_byte1)) goto false1 ;
        nx_byte1=0;
      }
   }  //while
   if (nx_byte1 > 0)
    {
     if (Write_To_Device (&bufor_wydruku[0], (nx_byte1)) != (nx_byte1)) goto false1 ;
     nx_byte1=0;
    }
  
  return TRUE ;
false1:
  return FALSE ;
}


static BOOL finish_BMP(int BMP_width, int height_to_fill)
{
	int i;
	char *row_ptr;
	long bytes_width;
    int bkg_color;
    unsigned char r,g,b,a;
    byte_ intensity;
    float intensity_tab[4] = { 0.2, 1.0, 0.8, 0.6 };

	bytes_width = BMP_width * BYTES_PER_DOT;
	if (NULL == (row_ptr = (char *) malloc(bytes_width))) return 0;


    //if (ptrs__prn_ini_date->background) bkg_color=GetColorAC(kolory.paper);
    //        else  bkg_color=GetColorAC(15); //0xffffff;

    ////if (ptrs__prn_ini_date->background) bkg_color=get_palette_color(kolory.paper);
    ////else  
		bkg_color=0xffffff;

    //r = _dac_normal[bkg_color][0] << 2;
    //g = _dac_normal[bkg_color][1] << 2;
    //b = _dac_normal[bkg_color][2] << 2;

	
#ifdef LINUX
    r = bkg_color & 0xFF; bkg_color >>= 8;
#else
	b = bkg_color & 0xFF; bkg_color >>= 8;
#endif

    g = bkg_color & 0xFF; bkg_color >>= 8;

#ifdef LINUX
    b = bkg_color & 0xFF; bkg_color >>= 8;
#else
	r = bkg_color & 0xFF; bkg_color >>= 8;
#endif

    a = bkg_color & 0xFF;

    ////if ((ptrs__prn_ini_date->gray_print) || !(ptrs__prn_ini_date->color_print) || (ptrs__prn_ini_date->PCX_gray)) //|| (ptrs__prn_ini_date->PCX_gray) this is not clear if should be
    ////{
    ////    intensity = 0.2989 * r + 0.5870 * g + 0.1140 * b;

    ////    if (intensity < 252) intensity *= intensity_tab[ptrs__prn_ini_date->gray_saturation];

    ////    r=g=b=intensity;

    ////}

    for (i = 0; i < (bytes_width); i+=3)
    {
        *(row_ptr + i) = r;
        *(row_ptr + i + 1) = g;
        *(row_ptr + i + 2) = b;
    }

	for (i = 0; i < height_to_fill; i++) 
	{
		fwrite((char *)(row_ptr), bytes_width, 1, BMPimageFile);
        BMPfilePos += bytes_width;
	}

	free(row_ptr);
	return 1;

}

int draw_top_margin_PCX(int BMP_width, int height_to_fill)
{
	int i;
	char *row_ptr;
	long bytes_width;

	bytes_width = BMP_width * BYTES_PER_DOT;
	if (NULL == (row_ptr = (char *)malloc(bytes_width))) return 0;

	for (i = 0; i < (bytes_width); i++)  *(row_ptr + i) = '\xff';

	for (i = 0; i < height_to_fill; i++)
	{
		fwrite((char *)(row_ptr), bytes_width, 1, BMPimageFile);
		BMPfilePos += bytes_width;
	}

	free(row_ptr);
	return 1;

}

int  draw_top_margin_WINDOWS(void)
{

	return 0;
}

static BOOL draw_matrix_to_file_BMP(void)
/*-------------------------------------------*/
{
	long i, nx_byte, ny_byte, n_row; // j;
	char *CM = matrix_head.command_mode;
	char  *ptrhsz_buf_PCX;
	int nx_byte1, nx_byte_PCX;
	int count = 0;
	long count_r = 0;
	int count_x = 0;
	int i_row;
	long total;
	unsigned char padding[3] = { 0, 0, 0 };
	int paddingSize;

	nx_byte = matrix_head.width_byte;
	nx_byte_PCX = matrix_head.width_byte * BITS_PER_DOT;
	ny_byte = jednostki_y_to_byte_prn(matrix_head.height);
	n_row = ny_byte * 8;

	paddingSize = (4 - (nx_byte_PCX) % 4) % 4;

	ptrhsz_buf_PCX = matrix_head.lp_matrix_PCX;  //adres matrycy PCX
	
	total = nx_byte_PCX * n_row;

	count_r = 0;
	count = 0;
	i_row = 0;
	nx_byte1 = 0;


	for (i = 0; i < n_row; i++) {
		
		fwrite((char *)(ptrhsz_buf_PCX + (i*nx_byte_PCX)), BYTES_PER_DOT, nx_byte*8, BMPimageFile);
        BMPfilePos += (BYTES_PER_DOT * nx_byte * 8);

		if (paddingSize > 0)
		{
			fwrite(padding, 1, BYTES_PER_DOT /*paddingSize*/, BMPimageFile);
			BMPfilePos += paddingSize;
		}
	}

	BMP_rows += n_row;
	return TRUE;
}
 
static BOOL draw_matrix_to_file_WINDOWS(void)
/*-------------------------------------------*/
{
	long i, nx_byte, ny_byte, n_row, j;
	char *CM = matrix_head.command_mode;
	char  *ptrhsz_buf_PCX;
	int nx_byte1, nx_byte_PCX;
	int count = 0;
	long count_r = 0;
	int count_x = 0;
	int i_row;
	unsigned char keyR, keyG, keyB;
	unsigned char key2R, key2G, key2B;
	long total;
	int runcount;
	int start_j, finish_j;
	int hdc_height;

	hdc_height = Get_HDC_Height();

	nx_byte = matrix_head.width_byte;
	nx_byte_PCX = matrix_head.width_byte * BITS_PER_DOT;
	ny_byte = jednostki_y_to_byte_prn(matrix_head.height);
	n_row = ny_byte * 8;

	ptrhsz_buf_PCX = matrix_head.lp_matrix_PCX;  //adres matrycy PCX

		total = nx_byte_PCX * n_row;

		count_r = 0;
		count = 0;
		i_row = 0;
		nx_byte1 = 0;

		for (i = 0; i < n_row; i++)  //w pionie
		{
			count_x = 0;
			runcount = 0;
			key2R = ptrhsz_buf_PCX[count_r];
			key2G = ptrhsz_buf_PCX[count_r+1];
			key2B = ptrhsz_buf_PCX[count_r+2];
			if ((original_color_print == FALSE) && (key2R != 252) && (key2G != 252) && (key2B != 252))  //wydruk B/W i nie jest to kolor tla
			{
				key2R = 0;
				key2G = 0;
				key2G = 0;
			}
			start_j = 0;
			finish_j = 0;

			for (j = 0; j < nx_byte_PCX; j+=3)  //w poziomie
			{
				keyR = ptrhsz_buf_PCX[count_r];
				keyG = ptrhsz_buf_PCX[count_r + 1];
				keyB = ptrhsz_buf_PCX[count_r + 2];

				finish_j = count_x;
				
				int key_count = 1;

				if ((key2R == keyR) && (key2G == keyG) && (key2B == keyB) && key_count)
				{
					runcount++;
				}
				else
				{
					if ((key2R != 255) || (key2G != 255) || (key2B != 255))
					{
						if (ptrs__prn_ini_date->reflection)
							PutPixelWinRGB(start_j, finish_j, hdc_height - (i + vertical_i), key2R, key2G, key2B);  //nie jest to kolor bialy
						else
						    PutPixelWinRGB(start_j, finish_j, i + vertical_i, key2R, key2G, key2B);  //nie jest to kolor bialy
					}
					key2R = keyR;
					key2G = keyG;
					key2B = keyB;
					start_j = count_x;
					finish_j = count_x;

					runcount = 0;
				}
				count_x++;
				count_r += 3;
			}
			
			if (runcount > 0)
			{
				if ((keyR != 255) || (keyG != 255) || (keyB != 255))
				{
					if (ptrs__prn_ini_date->reflection)
					    PutPixelWinRGB(start_j, finish_j, hdc_height - (i + vertical_i), keyR, keyG, keyB);  //nie jest to kolor bialy
					else
						PutPixelWinRGB(start_j, finish_j, i + vertical_i, keyR, keyG, keyB);  //nie jest to kolor bialy
				}
			}
			
		}


   //na koncu trzeba zapamietac ostatnia pozycje i  (verical)
	vertical_i += i;  //po wyjsciu z petli i jest zwiekszone o 1

	return TRUE;
}


static char get_matrix_byte (int x, int y, int dy, int kolor_ep)
/*-------------------------------------------------------------*/
{
  long dot_off;		/*offset bajtu zawierajacego punkt*/
  char  * lp_dot_byte;
  unsigned char pom, ret_byte;
  int i, m_x_byte_size;
  long aa;

  ret_byte = 0;
  m_x_byte_size = matrix_head.width_byte;
  dot_off = (long)y * m_x_byte_size + (x /*+ 1*/) / 8;
  if (kolor_ep==1)  /*black*/
   {
    if (SHEET_IS_PRINTED==TRUE) lp_dot_byte = matrix_head.lp_matrix_k_s + dot_off;
     else lp_dot_byte = matrix_head.lp_matrix_k + dot_off;
   }
   else
    if (kolor_ep==2)  /*cyjan*/
    {
    if (SHEET_IS_PRINTED==TRUE) lp_dot_byte = matrix_head.lp_matrix_c_s + dot_off;
     else lp_dot_byte = matrix_head.lp_matrix_c + dot_off;
    }
    else
     if (kolor_ep==4)  /*magenta*/
     {
      if (SHEET_IS_PRINTED==TRUE) lp_dot_byte = matrix_head.lp_matrix_m_s + dot_off;
       else lp_dot_byte = matrix_head.lp_matrix_m + dot_off;
     }
     else
      if (kolor_ep==8)  /*yellow*/
      {
      if (SHEET_IS_PRINTED==TRUE) lp_dot_byte = matrix_head.lp_matrix_y_s + dot_off;
       else lp_dot_byte = matrix_head.lp_matrix_y + dot_off;
      }
      else
       if (kolor_ep==128)  /*null*/
       {
       lp_dot_byte = matrix_head.lp_matrix_null + dot_off;
       }
       else
       if (kolor_ep==64)  /*enable*/
       {
       lp_dot_byte = matrix_head.lp_matrix_enable + dot_off;
       }
      else              /*balck*/
       {
       if (SHEET_IS_PRINTED==TRUE) lp_dot_byte = matrix_head.lp_matrix_k_s + dot_off;
        else lp_dot_byte = matrix_head.lp_matrix_k + dot_off;
       }
  aa = m_x_byte_size;
  aa *= dy;
  if (aa > 32000L)
  {
    aa++;
  }

  for ( i = 0;   i < 8;   i++, lp_dot_byte += m_x_byte_size * dy)
  {
    pom = 1;
    pom <<= (7 - (x % 8));
    pom &= *lp_dot_byte ;
    if (pom)
    {
      pom = 1;
      pom <<= (7 - i);
      ret_byte |= pom;
    }
  }
  return ret_byte;
}



static BOOL make_matrix_head(void)
/*-------------------------------*/
{
	long matrix_len;
	long m_y_row_size;
	long m_x_byte_size_max;
	double FormatX_1, FormatY_1;
	BOOL b_err = TRUE, prime_file;
	int lin;

#ifndef LINUX
	if (ptrs__prn_ini_date->prn_type == PRN_WINDOWS)
	{
		matrix_head.horizontal_density = GetPrintHorizDensity();
		matrix_head.vertical_density = GetPrintVertDensity();
		matrix_head.width = GetPrintPageWidth();
		matrix_head.height = GetPrintPageHeight();
	}
	else
	{
		//org_density = ptrs__prn_ini_date->density;
		matrix_head.vertical_density = get_vertical_density();
		matrix_head.horizontal_density = matrix_head.vertical_density;
		matrix_head.width = ptrs__prn_ini_date->width_paper;
		matrix_head.height = ptrs__prn_ini_date->height_paper;
	}
#else
    matrix_head.vertical_density = get_vertical_density();
    matrix_head.horizontal_density = matrix_head.vertical_density;
    matrix_head.width = ptrs__prn_ini_date->width_paper;
    matrix_head.height = ptrs__prn_ini_date->height_paper;
#endif

	FormatX_1 = (ptrs__prn_ini_date->xk - ptrs__prn_ini_date->xp);
	FormatY_1 = (ptrs__prn_ini_date->yk - ptrs__prn_ini_date->yp);

	if (ptrs__prn_ini_date->condensed == TRUE)
	{

		if (ptrs__prn_ini_date->rotation == TRUE)
		{
			matrix_head.condensed_wsp = min(
				jednostkiplt(ptrs__prn_ini_date->width_paper - ptrs__prn_ini_date->left_margin - ptrs__prn_ini_date->right_margin) / FormatY_1,
				jednostkiplt(ptrs__prn_ini_date->height_paper - ptrs__prn_ini_date->top_margin - ptrs__prn_ini_date->bottom_margin) / FormatX_1);
		}
		else
		{
			matrix_head.condensed_wsp = min(
				jednostkiplt(ptrs__prn_ini_date->width_paper - ptrs__prn_ini_date->left_margin - ptrs__prn_ini_date->right_margin) / FormatX_1,
				jednostkiplt(ptrs__prn_ini_date->height_paper - ptrs__prn_ini_date->top_margin - ptrs__prn_ini_date->bottom_margin) / FormatY_1); // *ptrs__prn_ini_date->scale;
		}

		matrix_head.width_max = matrix_head.width / matrix_head.condensed_wsp + 0.5;
		matrix_head.height_max = matrix_head.height / matrix_head.condensed_wsp + 0.5;

	}
	else
	{
		matrix_head.condensed_wsp = ptrs__prn_ini_date->scale;
		matrix_head.width_max = (ptrs__prn_ini_date->rotation == TRUE ? FormatY_1 : FormatX_1);

		matrix_head.width_max = matrix_head.width / matrix_head.condensed_wsp;
		matrix_head.height_max = matrix_head.height / matrix_head.condensed_wsp;

	}

	/*  tutaj nalezy wczytac wiekszy ze zbiorow i sprawdzic wielkosc pamieci */

	prime_file = TRUE;

	if ((ptrs__prn_ini_date->prn_type == PRN_PCX) /*|| (ptrs__prn_ini_date->prn_type == PRN_PDF)*/ || (ptrs__prn_ini_date->prn_type == PRN_WINDOWS))
	{
		if (ptrs__prn_ini_date->color_print == TRUE)
		{

			if ((ptrs__prn_ini_date->print_sheet == TRUE))
				matrix_len = (GetFreeMemSize() / 4) - 30;
			else matrix_len = (GetFreeMemSize() / 2) - 18;
		}
		else
		{
			if ((ptrs__prn_ini_date->print_sheet == TRUE))
				matrix_len = (GetFreeMemSize() / 4) - 30;
			else matrix_len = (GetFreeMemSize() / 2) - 18;
		}

		if (matrix_len > Bufor_Wydruku)
		{
			matrix_len = Bufor_Wydruku;
		}

		matrix_len /= 64;
		matrix_len *= 64;

		m_x_byte_size_max = jednostki_w_to_byte_prn(matrix_head.width);

		if (matrix_len < 48 * m_x_byte_size_max)
		{
			ErrList(211);
			return FALSE;
		}

#ifdef ALLEGRO_PRN_BMP

		m_y_row_size = matrix_len / (m_x_byte_size_max * BITS_PER_DOT); // BYTES_PER_DOT);

		m_y_row_size -= m_y_row_size % 8;

		allegro_prn_bmp = create_bitmap_ex(BITS_PER_DOT, m_x_byte_size_max * 8, m_y_row_size);

		if (allegro_prn_bmp == NULL)
		{
			Internal_Error(__LINE__, __FILE__);
			return FALSE;
		}

		lin = is_linear_bitmap(allegro_prn_bmp);

		matrix_len = m_x_byte_size_max * BYTES_PER_DOT * m_y_row_size;

		matrix_head.lp_matrix_PCX = (char *)allegro_prn_bmp->line[0];

#else
		if (NULL == NewBuffer((char **)&matrix_head.lp_matrix_PCX))
		{
			Internal_Error(__LINE__, __FILE__);
			return FALSE;
		}

		if (FALSE == SetBufferSize((char  *)matrix_head.lp_matrix_PCX, matrix_len + 64))  //*8 or *BITS_PER_DOT
		{
			DisposeBuffer((char  *)matrix_head.lp_matrix_PCX);
			Internal_Error(__LINE__, __FILE__);
			return FALSE;
		}
#endif


		matrix_head.len = matrix_len;
		m_y_row_size = matrix_len / (m_x_byte_size_max * (BITS_PER_DOT));

		m_y_row_size -= m_y_row_size % 8;

		matrix_head.height_max = jednostkiplt(inch_to_mm(1.0 * m_y_row_size / matrix_head.vertical_density) / matrix_head.condensed_wsp);

	}

  return TRUE;
}

static float intensity_tab[4] = {1.0, 1.0, 0.8, 0.6};

byte_ get_grey_intensity(COLOR_ color)
{
	byte_ intensity;

	intensity = (byte_)(0.2989*color.red + 0.5870*color.gre + 0.1140*color.blu);

	intensity *= intensity_tab[ptrs__prn_ini_date->gray_saturation];

	return intensity;
}

byte_ get_grey_intensity_p(COLOR_ *color)
{
	byte_ intensity;

	intensity = (byte_)(0.2989 * color->red + 0.5870 * color->gre + 0.1140 * color->blu);

	intensity *= intensity_tab[ptrs__prn_ini_date->gray_saturation];

	return intensity;
}


byte_ get_grey_intensity_ttf(COLOR_ color)
{
	byte_ intensity;

	intensity = (byte_)(0.2989*color.red + 0.5870*color.gre + 0.1140*color.blu);
	intensity *= intensity_tab[ptrs__prn_ini_date->gray_saturation];

	return intensity;
}

int MakeGrayColor(int kolor)
{   int kolor1;
    double intensity;
	int int_intensity;
	int red, green, blue;

	red=_dac_normal[kolor][0];
	green=_dac_normal[kolor][1];
	blue=_dac_normal[kolor][2];

    intensity = 0.2989*red + 0.5870*green + 0.1140*blue;

	intensity *= intensity_tab[ptrs__prn_ini_date->gray_saturation];

	int_intensity=(int)intensity;

	kolor1=gray_colors[int_intensity/4];

	return kolor1;
}


int MakeColor(int kolor)
{
	int kolor1;
	

	kolor1 = kolor;

	return kolor1;
}


static void put_dot(int x, int y, COLOR_ kolor)
/*--------------------------------------------*/
/*
W tej procedurze dodawany jest punkt do kazdego z 4 buforow koloru
lub do pojedynczego bufora koloru czarnego;
Jezeli x jest parzysty, to dodawany jest kolor z banku 0, jezeli nieparzysty,
to kolor z banku 1;
*/
{
	long dot_off;		/*offset bajtu zawierajacego punkt*/
	long dot_off_PCX;
	char  * lp_dot_byte_PCX;
	unsigned char pom;

	if (ptrs__prn_ini_date->prn_type == PRN_PDF)
	{
		
		x = x + pdf_left_margin;
		y = pdf_page_height - y - pdf_top_margin;

		LinePDF(x, y, x+1, y, 1, kolor, HPDF_BUTT_END);

		return;
	}

	if (x < 0 ||
		y < 0 ||
		x >(matrix_head.width_byte * 8 - matrix_head.left_margin - matrix_head.right_margin) ||
		y > (matrix_head.len / (matrix_head.width_byte * BITS_PER_DOT))) // BYTES_PER_DOT  
	{
		return;   /*rogi linii rzeczywistych (obcinane sa linie o zerowej grubosci)*/
	}

	dot_off = (long)y * matrix_head.width_byte + x / 8 + matrix_head.left_margin;
	dot_off_PCX = (long)(y * matrix_head.width_byte * BITS_PER_DOT) + (long)(x * BITS_PER_DOT / 8) + (long)(matrix_head.left_margin*BYTES_PER_DOT);
	if (dot_off_PCX > matrix_head.len) return;
	lp_dot_byte_PCX = matrix_head.lp_matrix_PCX + dot_off_PCX;


	if (dot_off < 0 || dot_off_PCX < 0 || dot_off >= matrix_head.len)
	{
		Internal_Error(__LINE__, __FILE__);
	}
	/*zapalenie bitu (x % 8) bajtu o adresie (*lp_dot_byte)  */
	pom = 1;
	pom <<= (7 - (x % 8));

	if (ptrs__prn_ini_date->prn_type == PRN_PCX) //PCX
	{

		*lp_dot_byte_PCX = kolor.red;
		if (BITS_PER_DOT == 24)
		{
			*(lp_dot_byte_PCX + 1) = kolor.gre;
			*(lp_dot_byte_PCX + 2) = kolor.blu;
		}
	}
	else //WINDOWS
	{
		*lp_dot_byte_PCX = kolor.red;
		*(lp_dot_byte_PCX + 1) = kolor.gre;
		*(lp_dot_byte_PCX + 2) = kolor.blu;
	}
	
}




static void put_dot_v (int x, int y, int i_width, COLOR_ kolor, double wspsincos)
/*-----------------------------------------------------------------------*/
{
  int y1, y2 ;
  int yy;
  double i_width_d, yy1, yy2;
  
  i_width_d = i_width;

  yy1 = y - ( i_width_d / wspsincos ) + 0.5;
  yy2 = y + ( i_width_d / wspsincos ) + 0.5;
  
  y1=(int) yy1;
  y2=(int) yy2;
  
  if (y2==y1) 
   { 
    if (fabs(yy2-yy1)>0.5) y2+=1;   //!!!!!!!!
   }
  
  for (yy = y1 ; yy < y2 ; yy++)
   {
    put_dot (x, yy, kolor) ; 
   }
}


static void put_dot_h (int x, int y, int i_width, COLOR_ kolor, double wspsincos)
/*-----------------------------------------------------------------------*/
{
  int x1, x2 ;
  int xx;
  double i_width_d, xx1, xx2;
  
  i_width_d = i_width;

  xx1 = x - ( i_width_d / wspsincos ) + 0.5;
  xx2 = x + ( i_width_d / wspsincos ) + 0.5;   
  x1 = (int) xx1;
  x2 = (int) xx2;
   
  for (xx = x1 ; xx < x2 ; xx++)
   {
    put_dot (xx, y, kolor) ;
   }
}

static void put_dot_v_ (int x, int y, int i_width, COLOR_ kolor)
/*----------------------------------------------------------*/
{
  int y1, y2 ;
  int yy;
  double i_width_d, yy1, yy2;
  
  i_width_d = i_width;

  yy1 = y - (i_width_d * 0.5) + 0.5;
  yy2 = y + (i_width_d * 0.5) + 0.5;

  y1 = (int) yy1;
  y2 = (int) yy2;

  if (y2==y1)
   { 
    if (fabs(yy2-yy1)>0.5) y2+=1;   //!!!!!!!!
   }

  
  for (yy = y1 ; yy </*=*/ y2 ; yy++)
   {
    put_dot (x, yy, kolor) ;
   }
   
 return; 
}


static void put_dot_h_ (int x, int y, int i_width, COLOR_ kolor)
/*----------------------------------------------------------*/
{
  int x1, x2 ;
  int xx;
  double i_width_d, xx1, xx2;
  
  i_width_d = i_width;
  
  xx1 = x - (i_width_d * 0.5) + 0.5;
  xx2 = x + (i_width_d * 0.5) + 0.5;
  
  x1 = (int) xx1;
  x2 = (int) xx2;
  
  if (x2==x1) 
   { 
    if (fabs(xx2-xx1)>0.5) x2+=1;   //!!!!!!!!
   }  
  
  for (xx = x1 ; xx </*=*/ x2 ; xx++)
   {
    put_dot (xx, y, kolor) ;
   }
   
 return; 
}

static void put_dot_vh_h (int x, int y, int i_width_v, int i_width_h, COLOR_ kolor, double wspsin, double wspcos)
/*----------------------------------------------------------------------------------------------------------*/
{
  int x0, x1, x2, y0, y1, y2 ;
  int xx, yy;
  double i_width_dv, i_width_dh;
  double xxd, delxd, delyd, delx;
  double dxx, dyy, yyd, xx1, xx2, yy1, yy2;
  
  i_width_dv = i_width_v;
  i_width_dh = i_width_h;

  dxx = i_width_dh;   //v
  dyy = i_width_dv;   //h
   
  dxx = ((dxx * wspsin) / 2) ;
  dyy = ((dyy * wspcos) / 2) ;
  
  xx1 = x - dxx + 0.5;
  x1 = (int) xx1;
  xx2 = x + dxx + 0.5;
  x2 = (int) xx2;
  yy1 = y + dyy + 0.5;
  y1 = (int) yy1;
  yy2 = y - dyy + 0.5;
  y2 = (int) yy2;
  
  
  if (x2 < x1)
   {
    x0 = x1;
    x1 = x2;
    x2 = x0;
    y0 = y1;
    y1 = y2;
    y2 = y0;
   } 
   
   
  if (x2==x1) 
   { 
     if (fabs(xx2-xx1)>0.5) x2+=1;   //!!!!!!!!
   } 

  delxd = x2 - x1;
  delyd = y2 - y1;
  
  if (TRUE == Check_if_Equal (delxd, 0)) return;
  
  for (xx = (x1 + 1) ; xx </*=*/ (x2 + 1); xx++)
  {
    xxd = xx - x1;
    delx = /*fabs*/ (xxd / delxd);
    yyd = y1;
    yyd += ( delx * delyd);
    yy = yyd;
    put_dot (xx, yy, kolor) ;
	put_dot (xx, yy - 1, kolor);
  }
 return;
}

static void put_dot_vh_v (int x, int y, int i_width_v, int i_width_h, COLOR_ kolor, double wspsin, double wspcos)
/*--------------------------------------------------------------------------------------------------------*/
{
  int x0, x1, x2, y0, y1, y2 ;
  int xx, yy;
  double i_width_dv, i_width_dh;
  double yyd, delxd, delyd, dely;
  double dxx, dyy, xxd, xx1, yy1, xx2, yy2;
	
  //wypelnianie w pionie - kat < 45 deg
      
  i_width_dv = i_width_v;
  i_width_dh = i_width_h;

  dxx = i_width_dh;  //v
  dyy = i_width_dv;  //h       //!!!!!!!!!!!!!!!!!!
  
  dxx = ((dxx * wspsin) / 2.0) ;
  dyy = ((dyy * wspcos) / 2.0) ;

  xx1 = x - dxx;
  x1 = round(xx1);
  xx2 = x + dxx;
  x2 = round(xx2);
  yy1 = y + dyy;
  y1 = round(yy1);
  yy2 = y - dyy;
  y2 = round(yy2);
  
  if (y2 < y1)
   {
    x0 = x1;
    x1 = x2;
    x2 = x0;
    y0 = y1;
    y1 = y2;
    y2 = y0;
   }
  
   if (y2==y1)
   { 
    if (fabs(yy2-yy1)>0.5) y2+=1;   //!!!!!!!!
   }

  delxd = x2 - x1;
  delyd = y2 - y1;
  
  if (TRUE == Check_if_Equal (delyd, 0)) 
	  return;

  if (y < 0)
	 int aaa = 0;

  for (yy = y1 ; yy </*=*/ y2 ; yy++) 
  {
    yyd = yy - y1;
    dely = /*fabs*/ (yyd / delyd);
    xxd = x1;
    xxd += ( dely * delxd);
    xx = xxd;
    
    put_dot (xx, yy, kolor) ;
	put_dot (xx /*+ 1*/, yy + 1, kolor) ;
  }  

  return;
}

static void point_(int x, int y, int i_width_v, int i_width_h, COLOR_ kolor)
/*-----------------------------------------------------------------------*/
{
	int x1, x2, y1, y2;
  int xx, yy;
  double d_width_dv, d_width_dh;
  double dxx, dyy, xxd, xx1, xx2, yy1, yy2;

  if ((i_width_v<=0) || (i_width_h<=0)) return;

//stary sposob
#ifdef old_prn_point_

  d_width_dv = i_width_v;
  d_width_dh = i_width_h;
  
  d_width_dv *= 0.5;
  d_width_dh *= 0.5;

//--------------
  dxx = d_width_dh ;
  
  xx1 = x - dxx + 0.5;
  x1 = (int) xx1;
  xx2 = x + dxx + 0.5;
  x2 = (int) xx2;

  for (xx = x1 ; xx < x2 ; xx++)
  {
    //wyznaczenie zakresu y1, y2
    xxd = x - xx;
    dyy = fabs(d_width_dh /*v*/ * d_width_dh) - (xxd*xxd);
    if (dyy>0.00001)
       dyy = sqrt(dyy);
    if (d_width_dh > 0.00001)
     {
      dyy *= (d_width_dv/d_width_dh);
     }
    yy1 = y - dyy + 0.5;
    y1 = (int) yy1;
    yy2 = y + dyy + 0.5;
    y2 = (int) yy2;
    
    for (yy = y1; yy < y2; yy++)
    {
      put_dot (xx, yy, kolor);
    }
  }  

#else
//nowy sposob
  x1 = x - ((i_width_v-1) * 0.5);
//  x2 = x + ((i_width_v-1) * 0.5);
  x2 = x + ((i_width_v-1) - x1);

  d_width_dv = i_width_v;
  d_width_dv *= 0.5;
  d_width_dh = i_width_h;
  d_width_dh *= 0.5;

  for (xx = x1 ; xx <= x2 ; xx++)
  {
    //wyznaczenie zakresu y1, y2
    xxd = x - xx;
    dyy = fabs(d_width_dv*d_width_dh) - (xxd*xxd);

//    if (dyy>0.00001) dyy = sqrt(dyy);
//    if (d_width_dh > 0.00001) dyy *= (d_width_dv/d_width_dh);
    dyy = sqrt(dyy);

//    y1 = y - dyy;
//    y2 = y + dyy;

    y1 = y - (dyy -1);
    y2 = y1 + (dyy + dyy -1);
    
    for (yy = y1; yy <= y2; yy++)
    {
      put_dot (xx, yy, kolor);
    }
  }
#endif
}

static void line_prn0(int x1, int y1, int x2, int y2, int kolor256, double text_line_width)
/*---------------------------------------------------------------------------------------*/


{
	int x, y, dx, dy, xs, ys;
	int dp, dd, d, i;
	int i_width;
	int inwert;
	double wspcos, wspsin, alfa;
	COLOR_ kolor;
	byte_ intensity;

	x = x1;
	y = y1;
	xs = ys = 1;
	if (x1 > x2) xs = -1;
	if (y1 > y2) ys = -1;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	if (dx == 0)
	{
		wspsin = 1;
		wspcos = 0;
	}
	else
	{
		alfa = Atan2(y2 - y1, x2 - x1);
		wspcos = cos(alfa);
		wspsin = sin(alfa);
	};


	if (ptrs__prn_ini_date->prn_type == PRN_PCX)
	{

		kolor.red = _dac_normal[kolor256][0] << 2;
		kolor.gre = _dac_normal[kolor256][1] << 2;
		kolor.blu = _dac_normal[kolor256][2] << 2;

		if (ptrs__prn_ini_date->color_print == 1)
		{
			if (ptrs__prn_ini_date->gray_print == 1)
			{
				intensity = get_grey_intensity(kolor);
				kolor = { intensity, intensity, intensity };
			}
		}
		else
		{
			if ((kolor.red < 252) && (kolor.gre < 252) && (kolor.blu < 252))
				kolor = { 0, 0, 0 };
		}
	}
	else if (ptrs__prn_ini_date->prn_type == PRN_PDF)
	{
		kolor.red = _dac_normal[kolor256][0] << 2;
		kolor.gre = _dac_normal[kolor256][1] << 2;
		kolor.blu = _dac_normal[kolor256][2] << 2;

		if (ptrs__prn_ini_date->color_print == 1)
		{
			if (ptrs__prn_ini_date->gray_print == 1)
			{
				intensity = get_grey_intensity(kolor);
				kolor = { intensity, intensity, intensity };
			}
		}
		else
		{
			if ((kolor.red < 252) && (kolor.gre < 252) && (kolor.blu < 252))
				kolor = { 0, 0, 0 };
		}

		x1 += pdf_left_margin;
		x2 += pdf_left_margin;
		y1 = pdf_page_height - y1 - pdf_top_margin;
		y2 = pdf_page_height - y2 - pdf_top_margin;

	}
	else
	{
		kolor.red = _dac_normal[kolor256][0] << 2;
		kolor.gre = _dac_normal[kolor256][1] << 2;
		kolor.blu = _dac_normal[kolor256][2] << 2;
    }
 

  if (dx >= dy)
  {
      if (( dx * 0.85 ) < dy )
       { inwert = 1;
       }
	else
	{
	inwert = 0 ;
	}
      i_width = get_width_line (FALSE, text_line_width) ;

	  if (ptrs__prn_ini_date->prn_type == PRN_PDF)
	  {
		  LinePDF(x1, y1, x2, y2, i_width, kolor, HPDF_ROUND_END);
	  }
	  else
	  {
		  put_dot_v(x1, y1, i_width, kolor, wspcos);

		  if (inwert == 1) put_dot_h(x1, y1, i_width, kolor, wspsin);

		  dd = 2 * (dy - dx);
		  dp = 2 * dy;
		  d = 2 * dy - dx;
		  for (i = 0; i < dx; i++, x += xs)
		  {
			  if (d >= 0) { d += dd; y += ys; }
			  else d += dp;
			  put_dot_v(x, y, i_width, kolor, wspcos);
			  if (inwert == 1) put_dot_h(x, y, i_width, kolor, wspsin);
		  }
	  }
 
  }
  else
  {
      if (( dy * 0.85 ) < dx )
      {
       inwert = 1;
       }
	else
	{
	inwert = 0;
	}
      i_width = get_width_line (TRUE, text_line_width) ;

	  if (ptrs__prn_ini_date->prn_type == PRN_PDF)
	  {
		  LinePDF(x1, y1, x2, y2, i_width, kolor, HPDF_ROUND_END);
	  }
	  else
	  {
		  put_dot_h(x1, y1, i_width, kolor, wspsin);
		  if (inwert == 1) put_dot_v(x1, y1, i_width, kolor, wspcos);

		  dd = 2 * (dx - dy);
		  dp = 2 * dx;
		  d = 2 * dx - dy;
		  for (i = 0; i < dy; i++, y += ys)
		  {
			  if (d >= 0) { d += dd; x += xs; }
			  else d += dp;
			  put_dot_h(x, y, i_width, kolor, wspsin);
			  if (inwert == 1) put_dot_v(x, y, i_width, kolor, wspcos);
		  }
	  }

  }
}


static void solid_(int x1, int y1, int x2, int y2, COLOR_ kolor)
/*------------------------------------------------------------*/
{
  int xx, yy;
  int x3, y3;
  
  if (x2<x1)
   {
    x3=x2;
    x2=x1;
    x1=x3;
   }

  if (y2<y1)
   {
    y3=y2;
    y2=y1;
    y1=y3;
   }


  if ((x2-x1<0) || (y2-y1<0)) return;

 if (ptrs__prn_ini_date->thin_point_PCX==0)
 {
  x2--;
  y2--;
  if (x2<=x1) x2=x1+1;   //zawsze drukowany jest przynajmniej 1 punkt
  if (y2<=y1) y2=y1+1;

  for (xx = x1 ; xx < x2 ; xx++)
   {
     for (yy = y1; yy < y2; yy++)
      {
        put_dot (xx, yy, kolor);
      }
   }
 }
 else if (ptrs__prn_ini_date->thin_point_PCX==1)
  {
   x2--;
   if (x2<=x1) x2=x1+1;   //zawsze drukowany jest przynajmniej 1 punkt
   if (y2==y1) y2=y1+1;

   for (xx = x1 ; xx < x2 ; xx++)
   {
     for (yy = y1; yy < y2; yy++)
      {
        put_dot (xx, yy, kolor);
      }
   }
  }
 else if (ptrs__prn_ini_date->thin_point_PCX==2)
  {
   if (x1==x2) x2=x1+1;   //zawsze drukowany jest przynajmniej 1 punkt
   if (y1==y2) y2=y1+1;
   for (xx = x1 ; xx < x2 ; xx++)
   {
     for (yy = y1; yy < y2; yy++)
      {
        put_dot (xx, yy, kolor);
      }
   }
  }
 else  //3
 {
  for (xx = x1 ; xx <= x2 ; xx++)
  {
    for (yy = y1; yy <= y2; yy++)
    {
      put_dot (xx, yy, kolor);
    }
  }
 }
}

static void solid___(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, COLOR_ kolor)
{ //int x0 ;
  int xx, yy;
  int xi,yj,yi,xii;
  double a,b,a1,b1;
  int min_x, min_y, max_x, max_y;
  double yj_d;
  double aa, aaa;
  //tymczasowo rysowanie punktu

  aa=(x1+x3)/2+0.5;
  xx=(int)aa;
  aa=(y1+y3)/2+0.5;
  yy=(int)aa;
  aa=sqrtl((x3-x1)*(x3-x1)+(y3-y1)*(y3-y1));
  aa/=1.4142135;
  xi=(int)aa;
  yi=(int)aa;
  if (xi==0) xi=1;
  if (yi==0) yi=1;

  point_(xx, yy, xi, yi, kolor);

  return;

  //rysowanie obszaru nieortogonalnego
  //wyznaczamy rownanie prostej dla odcinka <x1,y1>,<x2,y2>
  if (Check_if_Equal(x1,x2)==FALSE)
   {
    if (abs(x2-x1)>abs(y2-y1))  //lezy prawie poziomo
    {
     a=(y2-y1)/(x2-x1);
     b=y1-a*x1;
     a1=(y4-y1)/(x4-x1);
     b1=y1-a1*x1;
     //pierwsza linia
     for (xi=x1; xi<=x2; xi++)
      {
        yj_d=xi;
        yj_d*=a;
        yj_d+=b+0.5;
        yj=(int)yj_d;
        put_dot (xi, yj, kolor);
        aa=xi;
        //klonowanie punktu w liniach prostopadlych
        for (yi=y1; yi<=y4; yi++)
         {
           aa+=a1;
           aaa=aa+0.5;
           xii=(int)aaa;
           put_dot (xii, yi, kolor);
         }
      }
    }
    else //lezy prawie pionowo
    {
     a=(y4-y1)/(x4-x1);
     b=y1-a*x1;
     a1=(y2-y1)/(x2-x1);
     b1=y1-a1*x1;
     //pierwsza linia
     for (xi=x1; xi<=x4; xi++)
      {
        yj_d=xi;
        yj_d*=a;
        yj_d+=b+0.5;
        yj=(int)yj_d;
        put_dot (xi, yj, kolor);
        aa=xi;
        //klonowanie punktu w liniach prostopadlych
        for (yi=y1; yi<=y2; yi++)
         {
           aa+=a1;
           aaa=aa+0.5;
           xii=(int)aaa;
           put_dot (xii, yi, kolor);
         }
      }
    }
   }
    else
     {
       //rysowanie linii pionowych
       if (x1<x3) {min_x=x1; max_x=x3;}
         else {min_x=x3; max_x=x1;}

       if (y1<y3) {min_y=y1; max_y=y3;}
         else {min_y=y3; max_y=y1;}

       for (xi=min_x; xi<=max_x; xi++)
        {
         for (yj=min_y; yj<=max_y; yj++)
          {
            put_dot (xi, yj, kolor);
          }
        }
     }

  return;
}


void solid04_prn_old(double x01, double y01, double x02, double y02, int kolor256, double kos, double koc, BOOL bw, BOOL grey)
//----------------------------------------------------------------------------------------------------------------------------
{
	int x1, y1, x2, y2, x3, y3, x4, y4;
	double x11, y11, x22, y22, x33, y33, x44, y44;
	double width, height;
	byte_ intensity;
	COLOR_ kolor;


	if (ptrs__prn_ini_date->prn_type == PRN_PCX)
	{

		kolor.red = _dac_normal[kolor256][0] << 2;
		kolor.gre = _dac_normal[kolor256][1] << 2;
		kolor.blu = _dac_normal[kolor256][2] << 2;
	}
	else
	{
		kolor.red = _dac_normal[kolor256][0] << 2;
		kolor.gre = _dac_normal[kolor256][1] << 2;
		kolor.blu = _dac_normal[kolor256][2] << 2;
	}

	if ((ptrs__prn_ini_date->color_print == 1) && (bw == 0))
	{
		if ((ptrs__prn_ini_date->gray_print == 1) || (grey == 1) || ptrs__prn_ini_date->PCX_gray == 1)
		{
			intensity = get_grey_intensity(kolor);
			kolor = { intensity, intensity, intensity };
		}
	}
	else
	{
		if ((kolor.red < 252) && (kolor.gre < 252) && (kolor.blu < 252))
			kolor = { 0, 0, 0 };
	}

	if (Check_if_Equal(kos, 0.0) == TRUE)
	{
		x1 = jednostki_to_prn_x(x01, y01);
		y1 = jednostki_to_prn_y(x01, y01);
		x2 = jednostki_to_prn_x(x02, y02);
		y2 = jednostki_to_prn_y(x02, y02);
		solid_(x1, y1, x2, y2, kolor);
	}
	else
	{
		width = fabs(x02 - x01);
		height = fabs(y02 - y01);

		x11 = x01;
		y11 = y01;

		x22 = x01 + height * kos;
		y22 = y01 - height * koc;

		x33 = x01 + width * koc + height * kos;
		y33 = y01 + width * kos - height * koc;

		x44 = x01 + width * koc;
		y44 = y01 + width * kos;

		x1 = jednostki_to_prn_x(x11, y11);
		y1 = jednostki_to_prn_y(x11, y11);
		x2 = jednostki_to_prn_x(x22, y22);
		y2 = jednostki_to_prn_y(x22, y22);
		x3 = jednostki_to_prn_x(x33, y33);
		y3 = jednostki_to_prn_y(x33, y33);
		x4 = jednostki_to_prn_x(x44, y44);
		y4 = jednostki_to_prn_y(x44, y44);

		solid___(x1, y1, x2, y2, x3, y3, x4, y4, kolor);
	}
}



void solid04_prn(PCX_SOLID *pcx_s, COLOR_ kolor, double kos, double koc, BOOL bw, BOOL grey)
//----------------------------------------------------------------------------------------------
{
	int i;
	double s1, s2, p1, p2;
	BOOL ret;
	byte_ intensity;

	if (ptrs__prn_ini_date->PCX_gray)
	{
		intensity = get_grey_intensity(kolor);
		kolor = { intensity, intensity, intensity };
	}
	else if ((ptrs__prn_ini_date->color_print == 1) && (bw == 0))
	{
		if ((ptrs__prn_ini_date->gray_print == 1) || (grey == 1) || ptrs__prn_ini_date->PCX_gray==1)
		{
			intensity = get_grey_intensity(kolor);
			kolor = { intensity, intensity, intensity };
		}
	}
	else
	{
		if ((kolor.red < 252) && (kolor.gre < 252) && (kolor.blu < 252))
			kolor = { 0, 0, 0 };

	}


	ret= draw_rectangle_pcx_to_drive(pcx_s, df__width_fill_pcx, kolor);

	s1 = pcx_s->xy[6]; //punk S jest ostatnim punktem
	s2 = pcx_s->xy[7];
	for (i = 0; i < 8; i += 2) 	//dla kazdego wierzcholka
	{
		p1 = pcx_s->xy[i];	    	//punkt P jest nastepny
		p2 = pcx_s->xy[i + 1];

		line_pcx_prn(s1, s2, p1, p2, kolor, 0);
		s1 = p1;
		s2 = p2;
	}
	
	return; 
}


static BOOL draw_rectangle_pcx_to_drive(PCX_SOLID *pcx_s, double dist, COLOR_ kolor)
/*-------------------------------------------------------------------------------------*/
{
	
	double dx, dy, dxx, dyy;
	int i, nx, ny, nmax;

	dx = pcx_s->xy[0] - pcx_s->xy[6];
	dy = pcx_s->xy[1] - pcx_s->xy[7];

	nx = abs((int)(dx / dist));
	ny = abs((int)(dy / dist));

	if (nx > ny) nmax = nx;
	else nmax = ny;

	dxx = dx / nmax;
	dyy = dy / nmax;

	if (nmax<1)
	{
		line_pcx_prn(pcx_s->xy[6], pcx_s->xy[7], pcx_s->xy[4], pcx_s->xy[5], kolor, 0);
		return TRUE;
	}


	for (i = 0; i < nmax; i++)
	{
		line_pcx_prn(pcx_s->xy[6]+i*dxx, pcx_s->xy[7]+i*dyy, pcx_s->xy[4]+i*dxx, pcx_s->xy[5]+i*dyy, kolor, 0);
	}

	return TRUE;

}

static void line_pcx_prn(double x01, double y01, double x02, double y02, COLOR_ kolor, double text_line_width)
{
	int x1, y1, x2, y2, x, y, dx, dy, xs, ys, dx0, dy0;
	int i, x__1, y__1, x__2, y__2;
	int i_width_v, i_width_h;
	double wspcos, wspsin, alfa;
	double wspa, wspb;
	double dxs0, dys0, dxs, dys;
	double xy;

	
	x1 = jednostki_to_prn_x(x01, y01);
	y1 = jednostki_to_prn_y(x01, y01);
	x2 = jednostki_to_prn_x(x02, y02);
	y2 = jednostki_to_prn_y(x02, y02);

	i_width_v = get_width_line(FALSE, text_line_width);
	i_width_h = get_width_line(TRUE, text_line_width);

	x = x1;
	y = y1;
	xs = ys = 1;
	if (x1 > x2) xs = -1;
	if (y1 > y2) ys = -1;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);

	if ((dx == 0) || (dy == 0))
	{
		x__1 = jednostki_to_prn_x(x01 * 100, y01 * 100);
		y__1 = jednostki_to_prn_y(x01 * 100, y01 * 100);
		x__2 = jednostki_to_prn_x(x02 * 100, y02 * 100);
		y__2 = jednostki_to_prn_y(x02 * 100, y02 * 100);
		dx0 = abs(x__2 - x__1);
		dy0 = abs(y__2 - y__1);
	}
	else
	{
		dx0 = dx;
		dy0 = dy;
	}

	dxs0 = (x02 - x01);
	dys0 = (y01 - y01);

	dxs = (x2 - x1);
	dys = (y2 - y1);

	wspa = 1000000;

	if (FALSE == Check_if_Equal(dxs, 0))
	{
		wspa = dys / dxs;
	}

	wspb = y1 - (wspa * x1);

	if (TRUE == Check_if_Equal(dx0, 0))
	{
		wspsin = 0;
		wspcos = 1;
	}
	else
	{
		alfa = Atan2(y2 - y1, x2 - x1);
		wspcos = cos(alfa);
		wspsin = sin(alfa);
	}


	if (y2 == y1)
	{
		if (D_HIGH == ptrs__prn_ini_date->density)
		{
			//if (i_width_v < 2) i_width_v = 2; //!!!!!!!!!!!!!!!!!!!!!!! tymczasowo
			//if (i_width_h < 2) i_width_h = 2; //!!!!!!!!!!!!!!!!!!!!!! tymczasowo
			;
		}
	}

	
	if (dx0 >= dy0)
	{
		if (dx == 0) return;

		for (i = 0; i <= dx; i++, x += xs)
		{

			if (TRUE == Check_if_Equal(dy0, 0)) put_dot_v_(x, y, i_width_v, kolor);
			else
			{
				xy = x;
				xy *= wspa;
				xy += wspb;
				y = (int)xy;
				put_dot_vh_v(x, y, i_width_v, i_width_h, kolor, wspsin, wspcos);
			}
		}

	}
	else
	{
		
		if (dy == 0) return;

		for (i = 0; i <= dy; i++, y += ys)
		{
			if (TRUE == Check_if_Equal(dx0, 0)) put_dot_h_(x, y, i_width_h, kolor);
			else
			{
				xy = y;
				xy -= wspb;
				xy /= wspa;
				x = (int)xy;
				put_dot_vh_h(x, y, i_width_v, i_width_h, kolor, wspsin, wspcos);
			}
		}
	}
}

void line_prn_pattern(double x01, double y01, double x02, double y02)
{
	int x1, y1, x2, y2; // x, y; // dx, dy, xs;

	x1 = jednostki_to_prn_x(x01, y01);
	y1 = jednostki_to_prn_y(x01, y01);
	x2 = jednostki_to_prn_x(x02, y02);
	y2 = jednostki_to_prn_y(x02, y02);

	x1 += matrix_head.left_margin; // jednostki_to_prn_x(ptrs__prn_ini_date->left_margin, 0);
	x2 += matrix_head.left_margin; // jednostki_to_prn_x(ptrs__prn_ini_date->left_margin, 0);

	line(allegro_prn_bmp, x1, y1, x2, y2, 0);


}

void line_prn_translucent(double x01, double y01, double x02, double y02, int color)
{
	int x1, y1, x2, y2; // x, y; // dx, dy, xs;

	x1 = jednostki_to_prn_x(x01, y01);
	y1 = jednostki_to_prn_y(x01, y01);
	x2 = jednostki_to_prn_x(x02, y02);
	y2 = jednostki_to_prn_y(x02, y02);

	x1 += matrix_head.left_margin; // jednostki_to_prn_x(ptrs__prn_ini_date->left_margin, 0);
	x2 += matrix_head.left_margin; // jednostki_to_prn_x(ptrs__prn_ini_date->left_margin, 0);

	line(allegro_prn_bmp, x1, y1, x2, y2, color);


}

int get_Allegro_color(int kolor256, BOOL bw, BOOL grey)
{
	COLOR_ kolor;
	byte_ intensity;

	kolor.red = _dac_normal[kolor256][0] << 2;
	kolor.gre = _dac_normal[kolor256][1] << 2;
	kolor.blu = _dac_normal[kolor256][2] << 2;

	if ((ptrs__prn_ini_date->color_print == 1) && (bw == 0))
	{
		if ((ptrs__prn_ini_date->gray_print == 1) || (grey == 1))
		{
			intensity = get_grey_intensity(kolor);
			kolor = { intensity, intensity, intensity };
		}
	}
	else
	{
		if ((kolor.red < 254) && (kolor.gre < 254) && (kolor.blu < 254))
			kolor = { 0, 0, 0 };
	}
	
	return makecol24(kolor.red, kolor.gre, kolor.blu);
}

static void line_prn(double x01, double y01, double x02, double y02, int kolor256, double text_line_width, BOOL bw, BOOL grey)
{
	int x1, y1, x2, y2, x, y, dx, dy, xs, ys, dx0, dy0;
	int i, x__1, y__1, x__2, y__2;
	int i_width_v, i_width_h;
	double wspcos, wspsin, alfa;
	double wspa, wspb;
	double dxs0, dys0, dxs, dys;
	double xy;
	COLOR_ kolor;
	byte_ intensity;

	if (ptrs__prn_ini_date->prn_type == PRN_PDF)
	{
		x1 = jednostki_to_prn_x(x01, y01);
		y1 = jednostki_to_prn_y(x01, y01);
		x2 = jednostki_to_prn_x(x02, y02);
		y2 = jednostki_to_prn_y(x02, y02);

		kolor.red = _dac_normal[kolor256][0] << 2;
		kolor.gre = _dac_normal[kolor256][1] << 2;
		kolor.blu = _dac_normal[kolor256][2] << 2;

		if ((ptrs__prn_ini_date->color_print == 1) && (bw == 0))
		{
			if ((ptrs__prn_ini_date->gray_print == 1) || (grey == 1))
			{
				intensity = get_grey_intensity(kolor);
				kolor = { intensity, intensity, intensity };
			}
		}
		else
		{
			if ((kolor.red < 254) && (kolor.gre < 254) && (kolor.blu < 254))
				kolor = { 0, 0, 0 };
		}

		i_width_v = get_width_line(FALSE, text_line_width);
		i_width_h = get_width_line(TRUE, text_line_width);

		x1 += pdf_left_margin;
		x2 += pdf_left_margin;
		y1 = pdf_page_height - y1 - pdf_top_margin;
		y2 = pdf_page_height - y2 - pdf_top_margin;

		LinePDF(x1, y1, x2, y2, i_width_h, kolor, HPDF_ROUND_END);

		return;
	}


	x1 = jednostki_to_prn_x(x01, y01);
	y1 = jednostki_to_prn_y(x01, y01);
	x2 = jednostki_to_prn_x(x02, y02);
	y2 = jednostki_to_prn_y(x02, y02);


	x = x1;
	y = y1;
	xs = ys = 1;
	if (x1 > x2) xs = -1;
	if (y1 > y2) ys = -1;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);

	if ((dx == 0) || (dy == 0))
	{
		x__1 = jednostki_to_prn_x(x01 * 100, y01 * 100);
		y__1 = jednostki_to_prn_y(x01 * 100, y01 * 100);
		x__2 = jednostki_to_prn_x(x02 * 100, y02 * 100);
		y__2 = jednostki_to_prn_y(x02 * 100, y02 * 100);
		dx0 = abs(x__2 - x__1);
		dy0 = abs(y__2 - y__1);
	}
	else
	{
		dx0 = dx;
		dy0 = dy;
	}

	dxs0 = (x02 - x01);
	dys0 = (y01 - y01);

	dxs = (x2 - x1);
	dys = (y2 - y1);

	wspa = 1000000;

	if (FALSE == Check_if_Equal(dxs, 0))
	{
		wspa = dys / dxs;
	}

	wspb = y1 - (wspa * x1);

	if (TRUE == Check_if_Equal(dx0, 0))
	{
		wspsin = 0;
		wspcos = 1;
	}
	else
	{
		alfa = Atan2(y2 - y1, x2 - x1);
		wspcos = cos(alfa);
		wspsin = sin(alfa);
	}

	i_width_v = get_width_line(FALSE, text_line_width);
	i_width_h = get_width_line(TRUE, text_line_width);

	if (y2 == y1)
	{
		if (D_HIGH == ptrs__prn_ini_date->density)
		{
			//if (i_width_v < 2) i_width_v = 2; //!!!!!!!!!!!!!!!!!!!!!!! tymczasowo
			//if (i_width_h < 2) i_width_h = 2; //!!!!!!!!!!!!!!!!!!!!!! tymczasowo
			;
		}
	}

	if ((ptrs__prn_ini_date->prn_type == PRN_PCX))
	{
		//kolor.red = _dac_normal[kolor256][2] << 2;
		//kolor.gre = _dac_normal[kolor256][1] << 2;
		//kolor.blu = _dac_normal[kolor256][0] << 2;

		kolor.red = _dac_normal[kolor256][0] << 2;
		kolor.gre = _dac_normal[kolor256][1] << 2;
		kolor.blu = _dac_normal[kolor256][2] << 2;
	}
	else
	{
		kolor.red = _dac_normal[kolor256][0] << 2;
		kolor.gre = _dac_normal[kolor256][1] << 2;
		kolor.blu = _dac_normal[kolor256][2] << 2;
    }

	if ((ptrs__prn_ini_date->color_print == 1) && (bw == 0))
	{
		if ((ptrs__prn_ini_date->gray_print == 1) || (grey == 1))
		{
			intensity = get_grey_intensity(kolor);
			kolor = { intensity, intensity, intensity };
		}
	}
	else
	{
		if ((kolor.red < 254) && (kolor.gre < 254) && (kolor.blu < 254))
			kolor = { 0, 0, 0 };
	}


		//punkt poczatkowy
		point_(x1, y1, i_width_v, i_width_h, kolor);  //dla pionowych i poziomych
	//punkt koncowy
		point_(x2, y2, i_width_v, i_width_h, kolor);

		if (dx0 >= dy0)
		{
			if (dx == 0) return;

			for (i = 0; i <= dx; i++, x += xs)
			{

				if (TRUE == Check_if_Equal(dy0, 0)) put_dot_v_(x, y, i_width_v, kolor);
				else
				{
					xy = x;
					xy *= wspa;
					xy += wspb;
					y = round(xy);
					put_dot_vh_v(x, y, i_width_v, i_width_h, kolor, wspsin, wspcos);
				}
			}

		}
		else
		{

			if (dy == 0) return;

			for (i = 0; i <= dy; i++, y += ys)
			{
				if (TRUE == Check_if_Equal(dx0, 0)) put_dot_h_(x, y, i_width_h, kolor);
				else
				{
					xy = y;
					xy -= wspb;
					xy /= wspa;
					x = round(xy);
					put_dot_vh_h(x, y, i_width_v, i_width_h, kolor, wspsin, wspcos);
				}
			}
		}
}

void line_prn_trans(double x01, double y01, double x02, double y02, int kolor256, double text_line_width, BOOL bw, BOOL grey)
{
	int x1, y1, x2, y2, x, y, dx, dy, xs, ys, dx0, dy0;
	int i, x__1, y__1, x__2, y__2;
	int i_width_v, i_width_h;
	double wspcos, wspsin, alfa;
	double wspa, wspb;
	double dxs0, dys0, dxs, dys;
	double xy;
	COLOR_ kolor;
	byte_ intensity;

	int kolorA;

	x1 = jednostki_to_prn_x(x01, y01);
	y1 = jednostki_to_prn_y(x01, y01);
	x2 = jednostki_to_prn_x(x02, y02);
	y2 = jednostki_to_prn_y(x02, y02);


	kolorA = get_Allegro_color(kolor256, bw, grey);

	line(allegro_prn_bmp, x1, y1, x2, y2, kolorA);

	return;

	x = x1;
	y = y1;
	xs = ys = 1;
	if (x1 > x2) xs = -1;
	if (y1 > y2) ys = -1;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);

	if ((dx == 0) || (dy == 0))
	{
		x__1 = jednostki_to_prn_x(x01 * 100, y01 * 100);
		y__1 = jednostki_to_prn_y(x01 * 100, y01 * 100);
		x__2 = jednostki_to_prn_x(x02 * 100, y02 * 100);
		y__2 = jednostki_to_prn_y(x02 * 100, y02 * 100);
		dx0 = abs(x__2 - x__1);
		dy0 = abs(y__2 - y__1);
	}
	else
	{
		dx0 = dx;
		dy0 = dy;
	}

	dxs0 = (x02 - x01);
	dys0 = (y01 - y01);

	dxs = (x2 - x1);
	dys = (y2 - y1);

	wspa = 1000000;

	if (FALSE == Check_if_Equal(dxs, 0))
	{
		wspa = dys / dxs;
	}

	wspb = y1 - (wspa * x1);

	if (TRUE == Check_if_Equal(dx0, 0))
	{
		wspsin = 0;
		wspcos = 1;
	}
	else
	{
		alfa = Atan2(y2 - y1, x2 - x1);
		wspcos = cos(alfa);
		wspsin = sin(alfa);
	}

	i_width_v = get_width_line(FALSE, text_line_width);
	i_width_h = get_width_line(TRUE, text_line_width);

	if (y2 == y1)
	{
		if (D_HIGH == ptrs__prn_ini_date->density)
		{
			//if (i_width_v < 2) i_width_v = 2; //!!!!!!!!!!!!!!!!!!!!!!! tymczasowo
			//if (i_width_h < 2) i_width_h = 2; //!!!!!!!!!!!!!!!!!!!!!! tymczasowo
			;
		}
	}

	if ((ptrs__prn_ini_date->prn_type == PRN_PCX))
	{

		kolor.red = _dac_normal[kolor256][0] << 2;
		kolor.gre = _dac_normal[kolor256][1] << 2;
		kolor.blu = _dac_normal[kolor256][2] << 2;
	}
	else
	{
		kolor.red = _dac_normal[kolor256][0] << 2;
		kolor.gre = _dac_normal[kolor256][1] << 2;
		kolor.blu = _dac_normal[kolor256][2] << 2;
	}

	if ((ptrs__prn_ini_date->color_print == 1) && (bw == 0))
	{
		if ((ptrs__prn_ini_date->gray_print == 1) || (grey == 1))
		{
			intensity = get_grey_intensity(kolor);
			kolor = { intensity, intensity, intensity };
		}
	}
	else
	{
		if ((kolor.red < 254) && (kolor.gre < 254) && (kolor.blu < 254))
			kolor = { 0, 0, 0 };
	}



	point_(x1, y1, i_width_v, i_width_h, kolor);  //dla pionowych i poziomych
//punkt koncowy
	point_(x2, y2, i_width_v, i_width_h, kolor);

	if (dx0 >= dy0)
	{
		if (dx == 0) return;

		for (i = 0; i <= dx; i++, x += xs)
		{

			if (TRUE == Check_if_Equal(dy0, 0)) put_dot_v_(x, y, i_width_v, kolor);
			else
			{

				xy = x;
				xy *= wspa;
				xy += wspb;
				y = round(xy);
				put_dot_vh_v(x, y, i_width_v, i_width_h, kolor, wspsin, wspcos);
			}
		}

	}
	else
	{
		if (dy == 0) return;

		for (i = 0; i <= dy; i++, y += ys)
		{
			if (TRUE == Check_if_Equal(dx0, 0)) put_dot_h_(x, y, i_width_h, kolor);
			else
			{

				xy = y;
				xy -= wspb;
				xy /= wspa;
				x = round(xy);
				put_dot_vh_h(x, y, i_width_v, i_width_h, kolor, wspsin, wspcos);
			}
		}
	}
}


static int jednostki_to_prn_x ( double jednostkiX, double jednostkiY)
/*--------------------------------------------------------------*/
{
  double jx_mm, j_inch;
  int j_dot;
  int aa;

  if (ptrs__prn_ini_date->rotation == TRUE)
  {
  jx_mm = milimetryplt (jednostkiY - matrix_head.yp);
  }
  else
  {
	  if (ptrs__prn_ini_date->inversion == TRUE)
	  {
		  jx_mm = milimetryplt(jednostkiX - matrix_head.xp);
		  if (matrix_head.xp != 0.0)
			  aa = 1;
	  }
	  else jx_mm = milimetryplt(jednostkiX - matrix_head.xp);
  }

  j_inch = mm_to_inch (jx_mm) * matrix_head.condensed_wsp;
  j_dot = (int)/*floor*/(0.5 + j_inch * (double)matrix_head.horizontal_density); /* 0 1-wszy, 1 2-gi,..*/
  if (j_dot > 0)
  {
    j_dot--;
  }
  return j_dot;
}

int units_to_prn_x(double jednostkiX, double jednostkiY)
{
	return jednostki_to_prn_x(jednostkiX, jednostkiY);
}


static int jednostki_to_prn_y ( double jednostkiX, double jednostkiY)
/*--------------------------------------------------------------*/
{
  double jy_mm, j_inch;
  int j_dot;

  if (ptrs__prn_ini_date->rotation == TRUE)
  {
    jy_mm = milimetryplt (jednostkiX - matrix_head.xp);
  }
   else
    {
	  jy_mm = milimetryplt(matrix_head.yp + matrix_head.height - jednostkiY);
	  
    }

  j_inch = mm_to_inch (jy_mm) * matrix_head.condensed_wsp;
  j_dot = (int)/*floor*/(0.5 + j_inch * (double)matrix_head.vertical_density);
  if (j_dot > 0)
  {
    j_dot--;
  }
  return j_dot;
}

int units_to_prn_y(double jednostkiX, double jednostkiY)
{
	return jednostki_to_prn_y(jednostkiX, jednostkiY);
}

static int jednostki_x_to_byte_prn (double jednostkid)
/*---------------------------------------------------*/
{  double j_mm, j_inch;
  int j_dot , no_of_byte;

  j_mm = jednostki_d_to_plt_d( jednostkid );
  j_inch = mm_to_inch(j_mm) * matrix_head.condensed_wsp;
  j_dot = (int)floor (0.5 + j_inch * (double)matrix_head.horizontal_density);
  no_of_byte = j_dot / 8;
  if (j_dot % 8 > 0 )
    no_of_byte++;
  return no_of_byte;
}

static int jednostki_y_to_byte_prn (double jednostkid)
/*---------------------------------------------------*/
{  double j_mm, j_inch;
  int j_dot , no_of_byte;

  j_mm = jednostki_d_to_plt_d( jednostkid );
  j_inch = mm_to_inch(j_mm) * matrix_head.condensed_wsp;
  j_dot = (int)floor (0.5 + j_inch * (double)matrix_head.vertical_density);
  no_of_byte = j_dot / 8;
  if (j_dot % 8 > 0 )                /*liczba rzedow powinna byc*/
    no_of_byte++;      		     /*z zalozenia podzilna przez 24*/
  return no_of_byte;                 /*nie dotyczy to ostatniego rzedu pasa*/
}

static double prn_pxl_to_jednostki_x(int j_dot)
{
	double j_mm, j_inch;
	double jednostkid;

	j_inch=(double)j_dot/ (double)matrix_head.horizontal_density;
	j_mm = inch_to_mm(j_inch) / matrix_head.condensed_wsp;
	jednostkid = plt_d_to_jednostki_d(j_mm);
	return jednostkid;
}

static int jednostki_x_to_prn(double jednostkid)
/*---------------------------------------------*/
{
	double j_mm, j_inch;
	int j_dot;

	j_mm = jednostki_d_to_plt_d(jednostkid);
	j_inch = mm_to_inch(j_mm) * matrix_head.condensed_wsp;
	j_dot = (int)floor(0.5 + j_inch * (double)matrix_head.horizontal_density);

	return j_dot;
}

static int jednostki_y_to_prn(double jednostkid)
/*---------------------------------------------*/
{
	double j_mm, j_inch;
	int j_dot;

	j_mm = jednostki_d_to_plt_d(jednostkid);
	j_inch = mm_to_inch(j_mm) * matrix_head.condensed_wsp;
	j_dot = (int)floor(0.5 + j_inch * (double)matrix_head.vertical_density);
	    		    
	return j_dot;                
}

int units_x_to_prn(double jednostkid)
{
	return jednostki_x_to_prn(jednostkid);
}


int units_y_to_prn(double jednostkid)
{
	return jednostki_y_to_prn(jednostkid);
}

static int jednostki_w_to_byte_prn(double jednostkid)
/*---------------------------------------------------*/
{
	double j_mm, j_inch;
	int j_dot, no_of_byte;

	j_mm = jednostki_d_to_plt_d(jednostkid);
	j_inch = mm_to_inch(j_mm); // *matrix_head.condensed_wsp;
	j_dot = (int)floor(0.5 + j_inch * (double)matrix_head.horizontal_density);
	no_of_byte = j_dot / 8;
	if (j_dot % 8 > 0)
		no_of_byte++;
	return no_of_byte;
}

//static
int jednostki_w_to_bit_prn(double jednostkid)
/*---------------------------------------------------*/
{
	double j_mm, j_inch;
	int j_dot;

	j_mm = jednostki_d_to_plt_d(jednostkid);
	j_inch = mm_to_inch(j_mm); // *matrix_head.condensed_wsp;
	j_dot = (int)floor(0.5 + j_inch * (double)matrix_head.horizontal_density);
	
	return j_dot;
}

static int jednostki_h_to_byte_prn(double jednostkid)
/*---------------------------------------------------*/
{
	double j_mm, j_inch;
	int j_dot, no_of_byte;

	j_mm = jednostki_d_to_plt_d(jednostkid);
	j_inch = mm_to_inch(j_mm); // *matrix_head.condensed_wsp;
	j_dot = (int)floor(0.5 + j_inch * (double)matrix_head.vertical_density);
	no_of_byte = j_dot / 8;
	if (j_dot % 8 > 0)                /*liczba rzedow powinna byc*/
		no_of_byte++;      		     /*z zalozenia podzilna przez 24*/
	return no_of_byte;                 /*nie dotyczy to ostatniego rzedu pasa*/
}

int jednostki_h_to_bit_prn(double jednostkid)
/*---------------------------------------------------*/
{
	double j_mm, j_inch;
	int j_dot;

	j_mm = jednostki_d_to_plt_d(jednostkid);
	j_inch = mm_to_inch(j_mm); // *matrix_head.condensed_wsp;
	j_dot = (int)floor(0.5 + j_inch * (double)matrix_head.vertical_density);
	
	return j_dot; 
}

int units_h_to_byte_prn(double jednostkid)
{
	return jednostki_h_to_byte_prn(jednostkid);
}

void jednostki_to_prn_xy(int numpoints, double *xy, int* pxy)
{
    int i;

    for (i=0; i<numpoints; i+=2)
    {
        pxy[i] = jednostki_to_prn_x(xy[i], xy[i+1]) + matrix_head.left_margin;
        pxy[i+1] = jednostki_to_prn_y(xy[i], xy[i+1]);
    }
}

void jednostki_to_prn_xy_l(int numpoints, double *xy, long* pxy)
{
    int i;

    for (i=0; i<numpoints; i+=2)
    {
        pxy[i] = (long)(jednostki_to_prn_x(xy[i], xy[i+1]) + matrix_head.left_margin);
        pxy[i+1] = (long)(jednostki_to_prn_y(xy[i], xy[i+1]));
    }
}


BOOL Draw_to_Prn_Proc_Exe (char *ptrsz_file,
                       T_PTR_Prn_Config ptrs_config,
		       T_PTR_Prn_Ini_Date ptrs_ini_date, int init_prn_dlg, int serial_mode)
/*-------------------------------------------------------------------------------*/
{
  BOOL b_ret ;
  double df_widthfill ;
  char lpt_no [MAXPATH]; //[10];
  char ptrsz_file1[MAXPATH];
  BOOL ptrs__prn_ini_date_out_to_file;
  BOOL buforowanie_wydruku;
  int i;
  double xp_t, yp_t, xk_t, yk_t;
  int copies;
  static T_Prn_Ini_Date  ptrs__prn_ini_date_archive ;
  double th;


  Serial_Mode=serial_mode;
  set_left_margin=FALSE;
  b_ret=FALSE;
  i__page_min = ptrs_ini_date->first_page ;
  ptrs__prn_config = ptrs_config ;
  ptrs__prn_ini_date = ptrs_ini_date ;
  org_density = ptrs__prn_ini_date->density;
  strcpy(ptrsz_file1, ptrsz_file);
  ptrs__prn_ini_date_out_to_file = ptrs__prn_ini_date->out_to_file;
  
  buforowanie_wydruku = FALSE;
  if ((ptrs__prn_ini_date->prn_type==PRN_PCX) || (ptrs__prn_ini_date->prn_type == PRN_PDF))
   {

     //zapamietanie konfiguracji
     memcpy(&ptrs__prn_ini_date_archive,ptrs__prn_ini_date,sizeof(T_Prn_Ini_Date));

	 if (((ptrs__prn_ini_date->color_print == FALSE) || (ptrs__prn_ini_date->gray_print == TRUE)) &&
		 (!bitmap_png_exist) && (!bitmap_vector_exist))
	 {
		 BITS_PER_DOT = 8;
		 BYTES_PER_DOT = BITS_PER_DOT / 8;
		 BMPpalleteSize = (4 * 256);

	 }
	 else
	 {
		 BITS_PER_DOT = 24;
		 BYTES_PER_DOT = BITS_PER_DOT / 8;
		 BMPpalleteSize = 0;
	 }

     //zmiana konfiguracji
     ptrs__prn_ini_date->copies=1;

     ptrs__prn_ini_date->out_to_file=1;

     ptrs__prn_ini_date->color_mode=0;

	 
     if ((ptrs__prn_ini_date->condensed == FALSE) && (ptrs__prn_ini_date->actual_size==TRUE))
       {
         //ustawienie wielkosci strony
		 
         if (ptrs__prn_ini_date->rotation==FALSE)
           {
			 ptrs__prn_ini_date->width_paper = (fabs((ptrs__prn_ini_date->xk - ptrs__prn_ini_date->xp)) * ptrs__prn_ini_date->scale) + ptrs__prn_ini_date->left_margin + ptrs__prn_ini_date->right_margin + 0.5;
			 ptrs__prn_ini_date->height_paper = (fabs((ptrs__prn_ini_date->yk - ptrs__prn_ini_date->yp)) * ptrs__prn_ini_date->scale) + ptrs__prn_ini_date->top_margin + ptrs__prn_ini_date->bottom_margin + 0.5;
           }
            else
             {
			 ptrs__prn_ini_date->width_paper = (fabs((ptrs__prn_ini_date->yk - ptrs__prn_ini_date->yp)) * ptrs__prn_ini_date->scale) + ptrs__prn_ini_date->left_margin + ptrs__prn_ini_date->right_margin + 0.5; 
			 ptrs__prn_ini_date->height_paper = (fabs((ptrs__prn_ini_date->xk - ptrs__prn_ini_date->xp)) * ptrs__prn_ini_date->scale) + ptrs__prn_ini_date->top_margin + ptrs__prn_ini_date->bottom_margin + 0.5; 
             }
       }
	   
	 
   }

  else if (ptrs__prn_ini_date->prn_type==PRN_WINDOWS)
   {
     //zapamietanie konfiguracji
     memcpy(&ptrs__prn_ini_date_archive,ptrs__prn_ini_date,sizeof(T_Prn_Ini_Date));

     //zmiana konfiguracji
     ptrs__prn_ini_date->copies=1;

#ifndef LINUX
if (NO_REFLECTION)
	 ptrs__prn_ini_date->reflection = 0;  //TEMPORARY
#endif

     ptrs__prn_ini_date->color_mode=0;

	 //TUTAJ NALEZY PRZYJAC WIELKOSC STRONY Z INFORMACJI O DRUKARCE  //TODO!!!
   }

  buforowanie_wydruku = TRUE;
  strcpy(lpt_no, ptrs__prn_ini_date->plik);


  if (((ptrs__prn_ini_date->print_window) || (ptrs__prn_ini_date->print_sheet)) && //08-12-2020
      (ptrs__prn_ini_date->rotation) &&
      (ptrs__prn_ini_date->inversion))
   {
	 
     xp_t = FormatX_S - (double)(ptrs__prn_ini_date->xk);
     xk_t = FormatX_S - (double)(ptrs__prn_ini_date->xp);
     yp_t = FormatY_S - (double)(ptrs__prn_ini_date->yk);  
     yk_t = FormatY_S - (double)(ptrs__prn_ini_date->yp); 

	 ptrs__prn_ini_date->xp = (float)xp_t;
	 ptrs__prn_ini_date->xk = (float)xk_t;
	 ptrs__prn_ini_date->yp = (float)yp_t;
	 ptrs__prn_ini_date->yk = (float)yk_t;

	 xp_t = FormatX_S - (double)(ptrs__prn_ini_date->xk_w);
	 xk_t = FormatX_S - (double)(ptrs__prn_ini_date->xp_w);
	 yp_t = FormatY_S - (double)(ptrs__prn_ini_date->yk_w);
	 yk_t = FormatY_S - (double)(ptrs__prn_ini_date->yp_w);

     ptrs__prn_ini_date->xp_w = (float)xp_t;
     ptrs__prn_ini_date->xk_w = (float)xk_t;
     ptrs__prn_ini_date->yp_w = (float)yp_t;
     ptrs__prn_ini_date->yk_w = (float)yk_t;
	 
   }

	if (make_matrix_head() == FALSE)
	{
		if (ptrs__prn_ini_date->prn_type != PRN_WINDOWS)
		{
			Close_Output_Device();
		}
		goto aa;
	}

	  th = 0.75;

	  df_widthfill = jednostkiplt(inch_to_mm(th / (
		  max(matrix_head.horizontal_density,
			  matrix_head.vertical_density))) / matrix_head.condensed_wsp);

	  Set_Type_Drive(PRN_DRIVE, ptrs__prn_ini_date->prn_type,
		  jednostkiplt(ptrs__prn_config->l_luk) / matrix_head.condensed_wsp,
		  jednostkiplt(ptrs__prn_config->dot_width) / matrix_head.condensed_wsp,
		  df_widthfill);

	  set_line_max_width();
  

  if (buforowanie_wydruku==FALSE) copies=ptrs__prn_ini_date->copies; else copies=1;
  for (i=0; i<copies; i++)
  {
   if (draw_sheet_prn () == FALSE )
   {
    b_ret = FALSE;
    ErrList (41);
    break;
   }
   else
   {
    b_ret = TRUE ;
    
    komunikat (0);
   }
  }


  if ((ptrs__prn_ini_date->prn_type== PRN_PCX) /*|| (ptrs__prn_ini_date->prn_type == PRN_PDF)*/ || (ptrs__prn_ini_date->prn_type== PRN_WINDOWS))
  {
#ifdef ALLEGRO_PRN_BMP
	destroy_bitmap(allegro_prn_bmp);
#else
	  
    DisposeBuffer((char  *) matrix_head.lp_matrix_PCX); 
#endif
   }

  
aa:
  ptrs__prn_ini_date->density = org_density;


  if ((ptrs__prn_ini_date->prn_type==PRN_PCX) || (ptrs__prn_ini_date->prn_type==PRN_WINDOWS) || (ptrs__prn_ini_date->prn_type == PRN_PDF))
   {
     //przywrocenie konfiguracji
     memcpy(ptrs__prn_ini_date,&ptrs__prn_ini_date_archive,sizeof(T_Prn_Ini_Date));
   }
  return b_ret ;
}

BOOL Draw_to_Prn_Proc(char *ptrsz_file,
	T_PTR_Prn_Config ptrs_config,
	T_PTR_Prn_Ini_Date ptrs_ini_date, int init_prn_dlg, int serial_mode)
{
	BOOL ret;
	char plik_bak[MAXPATH];
	char image_format_bak[16];
	int image_format_i_bak;

	reset_texts_width(0, dane_size);
	TTF_printing = TRUE;

	if ((ptrs_ini_date->prn_type == PRN_PDF) && ((bitmap_exist == TRUE) || (bitmap_pattern_exist == TRUE) || 
		                                         (solid_translucent_exist == TRUE) ||  (ptrs_ini_date->background)))
	{
		strcpy(image_format_bak, ptrs_ini_date->image_format);
		image_format_i_bak = ptrs_ini_date->image_format_i;
		ret = TRUE;
		
		ptrs_ini_date->prn_type = PRN_PCX;
		strcpy(ptrs_ini_date->image_format,"PNG");
		ptrs_ini_date->image_format_i = 1;
		strcpy(plik_bak, ptrs_ini_date->plik);
		sprintf(ptrs_ini_date->plik, "Temp/pdfimg%03d_.png", Client_number);
		ptrs_ini_date->bitmap_only = TRUE;
		ptrs_ini_date->import_png = FALSE;
		ptrs_ini_date->print_translucent_solids = TRUE;

		ret = Draw_to_Prn_Proc_Exe(ptrsz_file, ptrs_config, ptrs_ini_date, init_prn_dlg, serial_mode);
		ptrs_ini_date->prn_type = PRN_PDF;
		strcpy(ptrs_ini_date->plik, plik_bak);
		
		ptrs_ini_date->bitmap_only = FALSE;
		ptrs_ini_date->import_png = TRUE;
		ptrs_ini_date->print_translucent_solids = FALSE;
		if (ret) ret = Draw_to_Prn_Proc_Exe(ptrsz_file, ptrs_config, ptrs_ini_date, init_prn_dlg, serial_mode);

		strcpy(ptrs_ini_date->image_format, image_format_bak);
		ptrs_ini_date->image_format_i = image_format_i_bak;
	}
	else
	{
		strcpy(image_format_bak, ptrs_ini_date->image_format);
		image_format_i_bak = ptrs_ini_date->image_format_i;
		ptrs_ini_date->bitmap_only = FALSE;
		ptrs_ini_date->import_png = FALSE;
		ptrs_ini_date->print_translucent_solids = TRUE;
		ret = Draw_to_Prn_Proc_Exe(ptrsz_file, ptrs_config, ptrs_ini_date, init_prn_dlg, serial_mode);
		strcpy(ptrs_ini_date->image_format, image_format_bak);
		ptrs_ini_date->image_format_i = image_format_i_bak;
	}

	reset_texts_width(0, dane_size);
	TTF_printing = FALSE;

	return ret;

}

T_PTR_Prn_Ini_Date *get_ptrs__prn_ini_date(void)
{
	return &ptrs__prn_ini_date;
}

double get_condensed_wsp(void)
{
	return matrix_head.condensed_wsp;
}

double get_matrix_head_xp(void)
{
	return matrix_head.xp;
}

double get_matrix_head_xk(void)
{
	return matrix_head.xk;
}

double get_matrix_head_yp(void)
{
	return matrix_head.yp;
}

double get_matrix_head_yk(void)
{
	return matrix_head.yk;
}


double get_matrix_head_width(void)
{
	return matrix_head.width;
}

double get_matrix_head_height(void)
{
	return matrix_head.height;
}



void correct_prn_bitmap_colors(BITMAP *png)
{
	float intensity_tab[4] = { 0.2, 1.0, 0.8, 0.6 };
	int j, i;
	int blue_;
	COLOR_* rowcolor;

	if (png->vtable->color_depth < 24) return;

	for (j = 0; j < png->h; j++)
	{
		rowcolor = (COLOR_*)(matrix_head.lp_matrix_PCX + (j * png->w * 3));
		for (i = 0; i < png->w; i++)
		{
			blue_ = rowcolor->blu;
			rowcolor->blu = rowcolor->red;
			rowcolor->red = blue_;
			rowcolor += 1;
		}
	}

}


void make_bitmap_grey(BITMAP* png, int t_printer, int bw, int grey)
{
	COLOR_ kolor;
	byte_ intensity;
	float intensity_tab[4] = { 0.2, 1.0, 0.8, 0.6 };
	int j, i;
	int color;
	int colorgray;

	if ((grey) || (bw) || (ptrs__prn_ini_date->gray_print == 1) || (ptrs__prn_ini_date->color_print == 0))
	{
			for (j = 0; j < png->h; j++)
			{
				for (i = 0; i < png->w; i++)
				{
					color = _getpixel24(png, i, j);
					kolor.red = getr(color);
					kolor.gre = getg(color);
					kolor.blu = getb(color);

					intensity = get_grey_intensity(kolor);
					//int_intensity = (byte_)intensity;
					colorgray = makecol(intensity, intensity, intensity);

					_putpixel24(png, i, j, colorgray);
				}
			}

	}
}

int get_rotation_inversion(void)
{
	return (( print_rotation*3 + print_inversion*2) % 4);
}


BOOL draw_lined_solid_to_drive___(int* xy)
/*------------------------------------*/
{
	unsigned w_tmp;
	double xd1, xd2, xd_o, yd_o;
	double cl, cp;
	double xl, xp, y;
	double ymin, ymin0;
	double x1a, x2a, angle_l, angle_p;
	
	double dxp, dxl;
	double x1,y1,x2,y2,x3,y3,x4,y4;

	double dist = 1.0;

	x1 = xy[0]; y1 = xy[1];
	x2 = xy[2]; y2 = xy[3];
	x3 = xy[4]; y3 = xy[5];
	x4 = xy[6]; y4 = xy[7];

	xd_o = yd_o = 0; 	/*to avoid warnings*/

	angle_l = Atan2(y4 - y1, x4 - x1);
	angle_p = Atan2(y3 - y2, x3 - x2);
	dxl = dist * fabs(sin(angle_l));
	dxp = dist * fabs(sin(angle_p));

	if (fabs(y1 - y4) <= dist)
	{
		line(allegro_prn_bmp, (x1 + x4) / 2 + dxl, (y1 + y4) / 2, (x2 + x3) / 2 - dxp, (y1 + y4) / 2, 0);
	}

	x1a = x1 + dxl;
	x2a = x2 - dxp;
	cl = (x4 - x1) / (y4 - y1);
	cp = (x3 - x2) / (y3 - y2);
	ymin = y4;
	ymin0 = y1;
	w_tmp = 0;
	do
	{
		ymin -= dist;
		y = ymin + dist / 2;
		if (ymin < ymin0)
		{
			y = ymin0 + dist / 2;
		}
		xl = x1a + (y - y1) * cl;
		xp = x2a + (y - y2) * cp;

		if (xl > xp)
		{
			xl = (xl + xp) / 2;
			xp = xl;
		}
		xd1 = xl;
		xd2 = xp;
		if (w_tmp % 2 == 1)
		{
			xd1 = xp;
			xd2 = xl;
		}
		if (w_tmp != 0)
		{
			line(allegro_prn_bmp, xd_o, yd_o, xd1, y, 0);
		}
		
		line(allegro_prn_bmp, xd1, y, xd2, y, 0);

		xd_o = xd2;
		yd_o = y;
		w_tmp++;
	} while (ymin > ymin0);
	return TRUE;
}

BOOL draw_lined_solid_to_drive(double* xy)
/*----------------------------------------*/
{
	unsigned w_tmp;
	double xd1, xd2, xd_o, yd_o;
	double cl, cp;
	double xl, xp, y;
	double ymin, ymin0;
	double x1a, x2a, angle_l, angle_p;
	double dxp, dxl;
	double x1 = xy[0], y1 = xy[1],
		x2 = xy[2], y2 = xy[3],
		x3 = xy[4], y3 = xy[5],
		x4 = xy[6], y4 = xy[7];
	double dist;
	

	dist = prn_pxl_to_jednostki_x(1);

	xd_o = yd_o = 0; 	/*to avoid warnings*/

	angle_l = Atan2(y4 - y1, x4 - x1);
	angle_p = Atan2(y3 - y2, x3 - x2);
	dxl = dist * fabs(sin(angle_l));
	dxp = dist * fabs(sin(angle_p));

	if (fabs(y1 - y4) <= dist)
	{
		return linen_plt_pattern((x1 + x4) / 2.0 + dxl, (y1 + y4) / 2.0, (x2 + x3) / 2.0 - dxp, (y1 + y4) / 2.0);
	}

	x1a = x1 + dxl;
	x2a = x2 - dxp;
	cl = (x4 - x1) / (y4 - y1);
	cp = (x3 - x2) / (y3 - y2);
	ymin = y1;
	ymin0 = y4;
	w_tmp = 0;
	do
	{
		ymin -= dist;
		y = ymin + dist / 2.0;
		if (ymin < ymin0)
		{
			y = ymin0 + dist / 2.0;
		}
		xl = x1a + (y - y1) * cl;
		xp = x2a + (y - y2) * cp;
		if (xl > xp)
		{
			xl = (xl + xp) / 2.0;
			xp = xl;
		}
		xd1 = xl;
		xd2 = xp;
		if (w_tmp % 2 == 1)
		{
			xd1 = xp;
			xd2 = xl;
		}
		if (w_tmp != 0)
		{
			if (linen_plt_pattern(xd_o, yd_o, xd1, y) == FALSE) return FALSE;
		}
		if (linen_plt_pattern(xd1, y, xd2, y) == FALSE) return FALSE;

		xd_o = xd2;
		yd_o = y;
		w_tmp++;
	} while (ymin > ymin0);
	return TRUE;
}

void Draw_Solidarc_Pattern(int numpoints, double* xy0, char* s_pattern, int t_printer, int bw, int grey, float origin_x_, float origin_y_)
{
    int i;
    int xxyy[256];
    double xy[256];
    BITMAP* pattern_bitmap_scaled_prn;
    int origin_x, origin_y;
    int origin_x1, origin_y1;
    int lxmax = 0;
    int lymax = 0;
    int bxmax = 0;
    int bymax = 0;
    BOOL ret;
    int bi;

    memcpy(xy, xy0, sizeof(double) * numpoints);

    origin_x = jednostki_to_prn_x(origin_x_, origin_y_);
    origin_y = jednostki_to_prn_y(origin_x_, origin_y_);

    double resolution = (double)matrix_head.horizontal_density / 150.0 * matrix_head.condensed_wsp;

    if ((grey) || (bw) || (ptrs__prn_ini_date->gray_print == 1) || (ptrs__prn_ini_date->color_print == 0)) ((SOLID_PATTERN*)s_pattern)->flag = ptrs__prn_ini_date->gray_saturation;

    bi = setfillpattern_gregre_scaled(s_pattern, TRUE, resolution, origin_x, origin_y, &origin_x1, &origin_y1);
    if (bi == -1) return;
    if (print_inversion)
    {
        if (print_rotation)
        {
            origin_x -= origin_y1;
            origin_y -= origin_x1;
        }
        else
        {
            origin_x -= origin_x1;
            origin_y -= origin_y1;
        }

    }
    else if (print_rotation)
    {
        origin_x += origin_y1;
        origin_y += origin_x1;
    }
    else
    {
        origin_x += origin_x1;
        origin_y += origin_y1;
    }



    pattern_bitmap_scaled_prn = create_bitmap_ex(24, pattern_bitmap_scaled[bi]->w, pattern_bitmap_scaled[bi]->h);
    if (pattern_bitmap_scaled_prn == NULL) return;
    blit(pattern_bitmap_scaled[bi], pattern_bitmap_scaled_prn, 0, 0, 0, 0, pattern_bitmap_scaled[bi]->w, pattern_bitmap_scaled[bi]->h);

    drawing_mode(DRAW_MODE_COPY_PATTERN, pattern_bitmap_scaled_prn, origin_x, origin_y);

    for (i = 0; i < numpoints; i += 2)
    {

        if (abs(xxyy[(i + 2) % numpoints] - xxyy[i]) > lxmax) lxmax = abs(xxyy[(i + 2) % numpoints] - xxyy[i]);
        if (abs(xxyy[(i + 3) % numpoints] - xxyy[i + 1]) > lymax) lymax = abs(xxyy[(i + 3) % numpoints] - xxyy[i + 1]);
    }

    ret = draw_polygon_to_drive_pattern(numpoints, xy0);

    destroy_bitmap(pattern_bitmap_scaled_prn);
}

void Draw_Solid_Pattern(int numpoints, double* xy0, char* s_pattern, int t_printer, int bw, int grey, float origin_x_, float origin_y_)
{
	int i;
	int xxyy[8];
	double xy[8];
	BITMAP* pattern_bitmap_scaled_prn;
	int origin_x, origin_y;
	int origin_x1, origin_y1;
	int lxmax = 0;
	int lymax = 0;
	int bxmax = 0;
	int bymax = 0;
	BOOL ret;
	int bi;

	memcpy(xy, xy0, sizeof(double) * numpoints);

	origin_x = jednostki_to_prn_x(origin_x_, origin_y_);
	origin_y = jednostki_to_prn_y(origin_x_, origin_y_);

	double resolution = (double)matrix_head.horizontal_density / 150.0 * matrix_head.condensed_wsp;

	if ((grey) || (bw) || (ptrs__prn_ini_date->gray_print == 1) || (ptrs__prn_ini_date->color_print == 0)) ((SOLID_PATTERN*)s_pattern)->flag = ptrs__prn_ini_date->gray_saturation;

	bi = setfillpattern_gregre_scaled(s_pattern, TRUE, resolution, origin_x, origin_y, &origin_x1, &origin_y1);
	if (bi == -1) return;
	if (print_inversion)
	{
		if (print_rotation)
		{
			origin_x -= origin_y1;
			origin_y -= origin_x1;
		}
		else
		{
			origin_x -= origin_x1;
			origin_y -= origin_y1;
		}

	}
	else if (print_rotation)
	{
		origin_x += origin_y1;
		origin_y += origin_x1;
	}
	else
	{
		origin_x += origin_x1;
		origin_y += origin_y1;
	}


	
	pattern_bitmap_scaled_prn = create_bitmap_ex(24, pattern_bitmap_scaled[bi]->w, pattern_bitmap_scaled[bi]->h);
	if (pattern_bitmap_scaled_prn == NULL) return;
	blit(pattern_bitmap_scaled[bi], pattern_bitmap_scaled_prn, 0, 0, 0, 0, pattern_bitmap_scaled[bi]->w, pattern_bitmap_scaled[bi]->h);

	drawing_mode(DRAW_MODE_COPY_PATTERN, pattern_bitmap_scaled_prn, origin_x, origin_y);

	for (i = 0; i < numpoints; i += 2)
	{
		
		if (abs(xxyy[(i + 2) % 8] - xxyy[i]) > lxmax) lxmax = abs(xxyy[(i + 2) % 8] - xxyy[i]);
		if (abs(xxyy[(i + 3) % 8] - xxyy[i + 1]) > lymax) lymax = abs(xxyy[(i + 3) % 8] - xxyy[i + 1]);
	}

	ret = draw_rectangle_to_drive_pattern(xy0);
	
	destroy_bitmap(pattern_bitmap_scaled_prn);
}

//void Draw_Solid_Pattern(int numpoints, double* xy0, char* s_pattern, int t_printer, int bw, int grey, float origin_x_, float origin_y_)

void Draw_Solid_Gradient(int numpoints, float *xy, int bw, int grey, unsigned char translucency, GRADIENT4 *gradient, Print_Rect *window_to_print)
{
    int ret;
    int fgray_saturation=0;

    if ((grey) || (bw) || (ptrs__prn_ini_date->gray_print == 1) || (ptrs__prn_ini_date->color_print == 0)) fgray_saturation = ptrs__prn_ini_date->gray_saturation;
    ret=draw_polygon_to_drive_gradient(numpoints, xy, translucency, gradient, window_to_print);

}


#undef __O_PRNFUN__