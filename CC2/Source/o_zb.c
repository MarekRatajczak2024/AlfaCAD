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

#define __O_ZB__
#include<forwin.h>
#ifndef LINUX
#include<io.h>
#include<dos.h>
#include <stdint.h>
#else
#include <unistd.h>
#endif
#include<fcntl.h>
#include<string.h>
#include<math.h>
#include<ctype.h>
#include<limits.h>
#include<stdlib.h>
#include<sys/stat.h>
#ifndef LINUX
#include<process.h>
#include <wtypes.h>
#else
#include <linwtypes.h>
#include <bits/stdint-uintn.h>
#endif

#include <sys/types.h>
#include <stdio.h>
//#include <fileapi.h>
#include <zlib.h>

#include "bib_e.h"
#include "b_bibe.h"
#include "rysuj_e.h"
#include "o_loadf.h"
#include "o_protec.h"
#include "o_spec.h"
#include "o_lfile.h"
#include "o_libfun.h"
#include "alffont.h"

#include "forlinux.h"

#include "message.h"

////#include "leak_detector_c.h"

////#define UPDATE_DATE

#define SEEK_SET 0
#define MaxLen 255

unsigned char  *Change_Block_Descript(BLOK *ptr_block, void *ptr_description, int len_descr);

BOOL test_python(void);
BOOL test_pillow(void);

extern int Export_Upgrade(char *fn, double Px,double Py, char *b_name, char *b_type);

extern void decodingwin_true(char *text);

extern int read_c(int _f, void *buf, int l_char) ;
extern int read_i(int _f, int *int_buf, int l_char);
extern void obiekt_tok_all (char  *, char  *, char  **, int) ;
extern void obiekt_tok_all1_1 (char  *, char  *, char  **, int) ;
extern void not_active (void) ;
extern BOOL Check_Objects (long_long , long_long , BOOL ) ;
extern long_long Get_Size_Block (long_long, long_long) ;
extern void *Change_Object (void *, void *, BOOL) ;
extern void *Change_Object1_1 (void *, void *, BOOL) ;
extern void Object_Tok_All (long_long *, long_long, long_long *, int) ;
extern void InfoList(int n) ;
extern void ClearInfo (void) ;
extern void Set_Version (int) ;
extern double get_skala_profilu_x(void);    //mnoznik skali
extern double get_poziom_pp(void);        //poziom zerowy profilu w m
extern int get_znacznik_aplikacji(void);
extern double get_hektometr_p(void);
extern double get_hektometr_k(void);
extern int put_znacznik_aplikacji(int);
extern void percentage (int percent);
extern void zwolnienie_pamieci_1(void);
extern void korekta_obiektow_blokow(void);
extern void zlikwiduj_wszystkie_bloki(void);
extern int load_pcx__(B_PCX *b_pcx);
extern int GetPCXParams(PCXheader head, B_PCX *b_pcx, double scale_xf, double scale_yf, double angle_f, BOOL background, BOOL preserve_aspect, BOOL on_top, BOOL h_flip, BOOL v_flip);
extern void reset_n_list_begin_blk(void);
extern int fnsplit(const char *path, char *drive, char *dir, char *name, char *ext);

static double df__text_height, df__text_width_factor ;
static BOOL b__italics ;
static BOOL b__bold ;
static char df__text_type ;
static char df__text_justowanie ;
static char df__text_ukryty ;
static char df__text_czcionka ;  
static int w__spec_no ;
static long l__dane_size ;  //TO XCHECK

void change_colors (char  *adr,char  *adrk, int kolor_elementu);

void change_layer (char  *adr,char  *adrk, int new_layer);
void normalize_layer (char *adr, char *adrk);
static void change_types_2_0_to_2_1 (char  *adr,char  *adrk);
static void change_texts_2_1_to_3_0 (char *adp, char *adk);

long czytaj(int f, char  *buf, unsigned long_long nbyte) ;

extern void delay(int time);
extern int kolorL_bitmap(void);
extern int GetColorAC1(int color);
extern void color_bar(void);
extern int Get_Point (double *coord_x, double *coord_y, int opcja);
extern int LoadPCX_real(B_PCX *b_pcx, char *nazwa_pcx);
extern int load_geo__(char *file_geo, char *blok_name);
extern FontTable PTRS__Font_Table;
extern int ReadBlockStyles40(int f);
extern void  SetDefaultStyles(void);
extern int ReadBlockStyles(int f);

extern int TRANSLUCENCY;
extern char *Get_File_Ext(char *fn);

extern int solid_pattern_library_no;

extern void get_spec_name(int nr, char *st);
extern void get_spec_string(int nr, char *st);
extern void put_spec_string(int nr, char *st);
extern void put_spec_name(int nr, char *st);

extern int ReadStyles(int f, int version);
extern int ReadStyles40(int f, int version);
extern int32_t ucs2_to_utf8 (int32_t ucs2, uint8_t * utf8);

#ifdef LINUX
extern long filelength_l(char *filename);
extern long filelength(int f);
extern off_t tell(int fd);
#endif

extern int SaveBlockStyles(int f, unsigned char charfont_numbers[]);
extern int SaveStyles(int f);
extern int get_fonts_for_dxf(unsigned char *charfont_numbers);
extern int get_patterns_for_solids(unsigned char* pattern_numbers);
extern void ErrListCode(int n, int error_code);

extern int __file_exists(char *name);
extern int ask_question(int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien, int image);
extern int LoadPCXinfo(char *filename, PCXheader *head); // , COLOR *palette);
extern int Get_Point_PCX(double* coord_x, double* coord_y, B_PCX *b_pcx , int opcja);
extern void standard_func(void);
extern BOOL CheckJPGExif(char* filename);
extern int LoadPNGinfo(char *filename, unsigned int *width, unsigned int *height, unsigned int *dpiX, unsigned int *dpiY, unsigned char * bits_per_pixel);
extern int LoadJPGinfo(char* filename, unsigned int* width, unsigned int* height, unsigned int* dpiX, unsigned int* dpiY, unsigned char* bits_per_pixel);

#define SOLID_PATTERN_LIBRARY_MAX_ITEMS 512  //1024
extern char* Solid_Pattern_Library[SOLID_PATTERN_LIBRARY_MAX_ITEMS];
extern void set_priv_prn_window(double Xmin_, double Ymin_, double Xmax_, double Ymax_);
extern PRIV_PRN_WINDOW *get_priv_prn_window(void);
extern void reset_trans_blender(void);
extern int Load_PNG_JPG_real(B_PCX *b_pcx, char *nazwa_pcx, size_t width, size_t height, int horz_res, int vert_res, unsigned char bits_per_pixel, int type);
extern int LoadPattern(SOLID_PATTERN_LIBRARY_ITEM* pattern_mem);

extern void utf8Upper(char* text);
extern void set_st_jedn(void);

extern char  *dane0;

extern double depth_magnitude; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double thermal_magnitude; //units per mm  default 1 Celsius per 1 mm on drawing paper
extern double load_magnitude; //units per mm  default 10kN/m load per 1 mm on drawing paper
extern double force_magnitude; //units per mm  default 10kN force per 1 mm on drawing paper
extern double moment_magnitude; //units per mm  default 10kNm force per 1 mm radius on drawing paper
extern double displacement_magnitude; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
extern double rotation_magnitude;

extern double depth_magnitude0; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double thermal_magnitude0; //units per mm  default 1 Celsius per 1 mm on drawing paper
extern double load_magnitude0; //units per mm  default 10kN/m load per 1 mm on drawing paper
extern double force_magnitude0; //units per mm  default 10kN force per 1 mm on drawing paper
extern double moment_magnitude0; //units per mm  default 10kNm force per 1 mm radius on drawing paper
extern double displacement_magnitude0; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
extern double rotation_magnitude0;

extern double thermal_precision;
extern double force_precision;
extern double moment_precision;
extern double stress_precision;
extern double displacement_precision;
extern double rotation_precision;
extern double load_precision;

extern double thermal_precision0;
extern double force_precision0;
extern double moment_precision0;
extern double stress_precision0;
extern double displacement_precision0;
extern double rotation_precision0;
extern double load_precision0;

extern double n_magnitude;
extern double v_magnitude;
extern double m_magnitude;
extern double d_magnitude;
extern double r_magnitude;
extern double rm_magnitude;
extern double s_magnitude;
extern double q_magnitude;
extern double p_magnitude;

extern double n_magnitude0;
extern double v_magnitude0;
extern double m_magnitude0;
extern double d_magnitude0;
extern double r_magnitude0;
extern double rm_magnitude0;
extern double s_magnitude0;
extern double q_magnitude0;
extern double p_magnitude0;

BOOL SAVE_ALL_FONTS = FALSE;

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

typedef enum
{
    PBM_scale = 0,
    PBM_interpolation,
    PBM_threshold,
    PBM_invert,
    PBM_filter,
    PBM_nofilter,

    ALX_scale,
    ALX_blacklevel,
    ALX_turdsize,
    ALX_alphamax,
    ALX_curve_tolerance,
    ALX_quantize
}BUT_Vectorization;

int pbm_scale=1;  //-s
int pbm_interpolation=3;  //-1  linear  or  -3 cubic
float pbm_threshold=0.45; //-t
BOOL pbm_invert=FALSE; // -i
int pbm_filter=4;  //-f
BOOL pbm_nofilter=FALSE;  //-n

float alx_scale=0.1;  //-x
float alx_blacklevel=0.5;  //-k
int alx_turdsize=2;  //-t
int alx_alphamax=1;  //-a
float alx_curve_tolerance=0.2;  //-O
int alx_quantize=10;  //-u


char typ_linii2_0_to_2_1(char typ2_0) 
{ char grubosc, typ;
  char typ2_1;
 grubosc=(typ2_0 / 5) + 1;
 typ=typ2_0 % 5;
 typ2_1=grubosc * 32 + typ;
 return typ2_1;
}


BOOL is_utf8(const char* string)
{
    if (!string)
        return 0;

    const unsigned char* bytes = (const unsigned char*)string;
    while (*bytes)
    {
        if ((// ASCII
            // use bytes[0] <= 0x7F to allow ASCII control characters
            bytes[0] == 0x09 ||
            bytes[0] == 0x0A ||
            bytes[0] == 0x0D ||
            (0x20 <= bytes[0] && bytes[0] <= 0x7E)
            )
            ) {
            bytes += 1;
            continue;
        }

        if ((// non-overlong 2-byte
            (0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
            (0x80 <= bytes[1] && bytes[1] <= 0xBF)
            )
            ) {
            bytes += 2;
            continue;
        }

        if ((// excluding overlongs
            bytes[0] == 0xE0 &&
            (0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
            (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            ) ||
            (// straight 3-byte
                ((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
                    bytes[0] == 0xEE ||
                    bytes[0] == 0xEF) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
                ) ||
            (// excluding surrogates
                bytes[0] == 0xED &&
                (0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
                )
            ) {
            bytes += 3;
            continue;
        }

        if ((// planes 1-3
            bytes[0] == 0xF0 &&
            (0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
            (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
            (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            ) ||
            (// planes 4-15
                (0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
                ) ||
            (// plane 16
                bytes[0] == 0xF4 &&
                (0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
                )
            ) {
            bytes += 4;
            continue;
        }

        return 0;
    }

    return 1;
}


static BOOL ver1_1_to_2_0_text (long off, BOOL b_simple)
/*----------------------------------------------------*/
{
  TEXT_OLD1_1 *ptrs_told ;
  TEXT_OLD1_1_F *ptrs_told_f ;
  TEXT_OLD1_1_FF *ptrs_told_ff ;
  TEXT_OLD1_1_FF1 *ptrs_told_ff1 ;
  TEXT_OLD1_1_FFF *ptrs_told_fff ;
  TEXT *ptrs_t, s_text = Tdef ;
  

  ptrs_t = &s_text ;
  ptrs_told = (TEXT_OLD1_1 *)(dane + off) ;
  ptrs_told_f = (TEXT_OLD1_1_F *)(dane + off + 6) ;
  ptrs_told_ff = (TEXT_OLD1_1_FF *)(dane + off + 26) ;
  ptrs_told_ff1 = (TEXT_OLD1_1_FF1 *)(dane + off + 27) ;
  ptrs_told_fff = (TEXT_OLD1_1_FFF *)(dane + off + 28) ;
  ptrs_t->atrybut = ptrs_told->atrybut ;
  ptrs_t->obiekt = ptrs_told->obiekt ; 
  ptrs_t->obiektt1 = 0;//ptrs_told->obiektt1 ;
  ptrs_t->obiektt2 = 0;//ptrs_told->obiektt2 ;
  ptrs_t->obiektt3 = 0;//ptrs_told->obiektt3 ; 
  ptrs_t->widoczny = ptrs_told->widoczny ; 
  ptrs_t->przec = ptrs_told->przec ; 
  if (b_simple == FALSE) ptrs_t->blok = 0 /*ptrs_told->blok*/ ; 
    else ptrs_t->blok = 1;
  ptrs_t->n = ptrs_told->n + 4 ;  
  ptrs_t->warstwa = ptrs_told->warstwa ; 
  ptrs_t->kolor = ptrs_told->kolor ; 
  ptrs_t->czcionka = ptrs_told->czcionka ;
  ptrs_t->bold = 0;
  ptrs_t->kat = ptrs_told_f->kat ;
  ptrs_t->wysokosc = ptrs_told_f->wysokosc ;
  ptrs_t->width_factor = ptrs_told_f->width_factor ;
  ptrs_t->x = ptrs_told_f->x ;
  ptrs_t->y = ptrs_told_f->y ;
  ptrs_t->italics = ptrs_told_ff->italics ;
  ptrs_t->typ = 0 ;
  ptrs_t->justowanie = 0 ;
  ptrs_t->ukryty = 0;
  ptrs_t->dl = ptrs_told_ff1->dl;
  ptrs_t->multiline=0;
  ptrs_t->underline = 0;
  ptrs_t->encoding = 0;
  ptrs_t->spacing=6;
  
  strncpy (ptrs_t->text, ptrs_told_fff->text, MaxTextLen + 1) ;
  ptrs_t->text [MaxTextLen] = '\0' ;
  ptrs_t->text [ptrs_t->dl] = '\0' ;
 
  
  b_simple = ((b_simple == TRUE && ptrs_t->blok == 0) ? TRUE : FALSE) ;
  

  return ((NULL != Change_Object1_1 ((void *)ptrs_told, (void *)ptrs_t,
		b_simple)) ? TRUE : FALSE) ;
		

}

static BOOL ver1_1_to_2_0_linia (long off, BOOL b_simple )
/*----------------------------------------------------*/
{
  LINIA1_1 *ptrs_lold ;
  LINIAEND1_1 *ptrs_loldend;

  LINIA *ptrs_l, s_linia = Ldef ;
  int size_l;

  size_l=sizeof(LINIA1_1);
  ptrs_l = &s_linia ;
  ptrs_lold = (LINIA1_1 *)(dane + off) ;
  ptrs_loldend = (LINIAEND1_1 *)(dane + off+6) ;
  ptrs_l->atrybut = ptrs_lold->atrybut ;
  ptrs_l->obiekt = ptrs_lold->obiekt ; 
  ptrs_l->obiektt1 = 0;//ptrs_lold->obiektt1 ;
  ptrs_l->obiektt2 = 0;//ptrs_lold->obiektt2 ;
  ptrs_l->obiektt3 = 0;//ptrs_lold->obiektt3 ; 
  ptrs_l->widoczny = ptrs_lold->widoczny ; 
  ptrs_l->przec = ptrs_lold->przec ; 
  if (b_simple == FALSE) ptrs_l->blok = 0 /*ptrs_lold->blok*/ ; 
   else ptrs_l->blok = 1;
  ptrs_l->n = ptrs_lold->n + 2 ;  
  ptrs_l->warstwa = ptrs_lold->warstwa ; 
  ptrs_l->kolor = ptrs_lold->kolor ; 
  ptrs_l->typ = typ_linii2_0_to_2_1(ptrs_lold->typ) ;
  ptrs_l->x1 = ptrs_loldend->x1 ;
  ptrs_l->y1 = ptrs_loldend->y1 ;
  ptrs_l->x2 = ptrs_loldend->x2 ;
  ptrs_l->y2 = ptrs_loldend->y2 ;
  
  b_simple = ((b_simple == TRUE && ptrs_l->blok == 0) ? TRUE : FALSE) ;
  
  return ((NULL != Change_Object1_1 ((void *)ptrs_lold, (void *)ptrs_l,
		b_simple)) ? TRUE : FALSE) ;
}

static BOOL ver1_1_to_2_0_luk (long off, BOOL b_simple)
/*----------------------------------------------------*/
{
  LUK1_1 *ptrs_lold ;
  LUKEND1_1 *ptrs_loldend ;
  LUK *ptrs_l, s_luk = ldef ;

  ptrs_l = &s_luk ;
  ptrs_lold = (LUK1_1 *)(dane + off) ;
  ptrs_loldend = (LUKEND1_1 *)(dane + off +6 ) ;
  ptrs_l->atrybut = ptrs_lold->atrybut ;
  ptrs_l->obiekt = ptrs_lold->obiekt ; 
  ptrs_l->obiektt1 = 0;//ptrs_lold->obiektt1 ;
  ptrs_l->obiektt2 = 0;//ptrs_lold->obiektt2 ;
  ptrs_l->obiektt3 = 0;//ptrs_lold->obiektt3 ;
  ptrs_l->widoczny = ptrs_lold->widoczny ; 
  ptrs_l->przec = ptrs_lold->przec ; 
  if (b_simple == FALSE) ptrs_l->blok = 0 /*ptrs_lold->blok*/ ;
    else ptrs_l->blok = 1;
  ptrs_l->n = ptrs_lold->n + 2 ;  
  ptrs_l->warstwa = ptrs_lold->warstwa ; 
  ptrs_l->kolor = ptrs_lold->kolor ; 
  ptrs_l->typ = typ_linii2_0_to_2_1(ptrs_lold->typ) ;
  ptrs_l->x = ptrs_loldend->x ;
  ptrs_l->y = ptrs_loldend->y ;
  ptrs_l->r = ptrs_loldend->r ;
  ptrs_l->kat1 = ptrs_loldend->kat1 ;
  ptrs_l->kat2 = ptrs_loldend->kat2 ;
  
  b_simple = ((b_simple == TRUE && ptrs_l->blok == 0) ? TRUE : FALSE) ;
  
  return ((NULL != Change_Object1_1 ((void *)ptrs_lold, (void *)ptrs_l,
		b_simple)) ? TRUE : FALSE) ;
		
}

static BOOL ver1_1_to_2_0_okrag (long off, BOOL b_simple)
/*----------------------------------------------------*/
{
  OKRAG1_1 *ptrs_lold ;
  OKRAGEND1_1 *ptrs_loldend ;
  OKRAG *ptrs_l, s_okrag = Odef ;

  ptrs_l = &s_okrag ;
  ptrs_lold = (OKRAG1_1 *)(dane + off) ;
  ptrs_loldend = (OKRAGEND1_1 *)(dane + off +6 ) ;
  ptrs_l->atrybut = ptrs_lold->atrybut ;
  ptrs_l->obiekt = ptrs_lold->obiekt ; 
  ptrs_l->obiektt1 = 0;//ptrs_lold->obiektt1 ;
  ptrs_l->obiektt2 = 0;//ptrs_lold->obiektt2 ;
  ptrs_l->obiektt3 = 0;//ptrs_lold->obiektt3 ;
  ptrs_l->widoczny = ptrs_lold->widoczny ; 
  ptrs_l->przec = ptrs_lold->przec ; 
  if (b_simple == FALSE) ptrs_l->blok = 0 /*ptrs_lold->blok*/ ;
    else ptrs_l->blok = 1;
  ptrs_l->n = ptrs_lold->n + 2 ;  
  ptrs_l->warstwa = ptrs_lold->warstwa ; 
  ptrs_l->kolor = ptrs_lold->kolor ; 
  ptrs_l->typ = typ_linii2_0_to_2_1(ptrs_lold->typ) ;
  ptrs_l->x = ptrs_loldend->x ;
  ptrs_l->y = ptrs_loldend->y ;
  ptrs_l->r = ptrs_loldend->r ;
  
  b_simple = ((b_simple == TRUE && ptrs_l->blok == 0) ? TRUE : FALSE) ;
  
  return ((NULL != Change_Object1_1 ((void *)ptrs_lold, (void *)ptrs_l,
		b_simple)) ? TRUE : FALSE) ;
		
}

static BOOL ver1_1_to_2_0_wielokat (long off, BOOL b_simple)
/*----------------------------------------------------*/
{
  WIELOKAT1_1 *ptrs_lold ;
  VERTEX81_1 *ptrs_loldvertex8;
  WIELOKAT *ptrs_l, st_wielokat = Stdef, s4_wielokat = S4def;

  ptrs_lold = (WIELOKAT1_1 *)(dane + off) ;
  ptrs_loldvertex8 = (VERTEX81_1 *)(dane + off + 8 ) ;
  if (ptrs_lold->lp==6) 
   { 
     ptrs_l = &st_wielokat ;
    }
     else 
     {
      ptrs_l = &s4_wielokat ;
     }
  ptrs_l->atrybut = ptrs_lold->atrybut ;
  ptrs_l->obiekt = ptrs_lold->obiekt ; 
  ptrs_l->obiektt1 = 0;//ptrs_lold->obiektt1 ;
  ptrs_l->obiektt2 = 0;//ptrs_lold->obiektt2 ;
  ptrs_l->obiektt3 = 0;//ptrs_lold->obiektt3 ;
  ptrs_l->widoczny = ptrs_lold->widoczny ; 
  ptrs_l->przec = ptrs_lold->przec ; 
  if (b_simple == FALSE) ptrs_l->blok = 0 /*ptrs_lold->blok*/ ;
    else ptrs_l->blok = 1;
  ptrs_l->n = ptrs_lold->n + 4 /*2*/ ;
  ptrs_l->warstwa = ptrs_lold->warstwa ; 
  ptrs_l->kolor = ptrs_lold->kolor ; 
  ptrs_l->temp1 = ptrs_lold->temp1 ;
  ptrs_l->translucent = 0;
  ptrs_l->empty_typ= ptrs_lold->rezerwa1 ;
  ptrs_l->lp = ptrs_lold->lp ;
  ptrs_l->xy[0] = ptrs_loldvertex8->xy[0] ;
  ptrs_l->xy[1] = ptrs_loldvertex8->xy[1] ;
  ptrs_l->xy[2] = ptrs_loldvertex8->xy[2] ; 
  ptrs_l->xy[3] = ptrs_loldvertex8->xy[3] ;
  ptrs_l->xy[4] = ptrs_loldvertex8->xy[4] ;
  ptrs_l->xy[5] = ptrs_loldvertex8->xy[5] ;
  if (ptrs_lold->lp==8) 
  {
   ptrs_l->xy[6] = ptrs_loldvertex8->xy[6] ; 
   ptrs_l->xy[7] = ptrs_loldvertex8->xy[7] ;
  } 
 
  
  b_simple = ((b_simple == TRUE && ptrs_l->blok == 0) ? TRUE : FALSE) ;
  
  return ((NULL != Change_Object1_1 ((void *)ptrs_lold, (void *)ptrs_l,
		b_simple)) ? TRUE : FALSE) ;
		
}

static BOOL ver1_1_to_2_0_point (long off, BOOL b_simple)
/*----------------------------------------------------*/
{
  T_Point1_1 *ptrs_lold ;
  T_PointEND1_1 *ptrs_loldend ;
  T_Point *ptrs_l, s_point = PointDef ;

  ptrs_l = &s_point ;
  ptrs_lold = (T_Point1_1 *)(dane + off) ;
  ptrs_loldend = (T_PointEND1_1 *)(dane + off +6) ;
  ptrs_l->atrybut = ptrs_lold->atrybut ;
  ptrs_l->obiekt = ptrs_lold->obiekt ; 
  ptrs_l->obiektt1 = 0;//ptrs_lold->obiektt1 ;
  ptrs_l->obiektt2 = 0;//ptrs_lold->obiektt2 ;
  ptrs_l->obiektt3 = 0;//ptrs_lold->obiektt3 ; 
  ptrs_l->widoczny = ptrs_lold->widoczny ; 
  ptrs_l->przec = ptrs_lold->przec ; 
  if (b_simple == FALSE) ptrs_l->blok = 0 /*ptrs_lold->blok*/ ; 
    else ptrs_l->blok = 1;
  ptrs_l->n = ptrs_lold->n + 2 ;  
  ptrs_l->warstwa = ptrs_lold->warstwa ; 
  ptrs_l->kolor = ptrs_lold->kolor ; 
  ptrs_l->x = ptrs_loldend->x ;
  ptrs_l->y = ptrs_loldend->y ;
  
  b_simple = ((b_simple == TRUE && ptrs_l->blok == 0) ? TRUE : FALSE) ;
  
  return ((NULL != Change_Object1_1 ((void *)ptrs_lold, (void *)ptrs_l,
		b_simple)) ? TRUE : FALSE) ;
		
}

static BOOL ver1_1_to_2_0_block (long off, BOOL b_simple)
/*----------------------------------------------------*/
{
  BLOK1_1 *ptrs_lold ;
  BLOK1_1F *ptrs_loldf ;
  BLOK1_1FF *ptrs_loldff ;
  BLOK1_1FFF *ptrs_loldfff ;
  T_Point *ptrs_l, s_point = PointDef ;

 
  ptrs_l = &s_point ;
  ptrs_lold = (BLOK1_1 *)(dane + off) ;
  ptrs_loldf = (BLOK1_1F *)(dane + off +4) ;
  ptrs_loldff = (BLOK1_1FF *)(dane + off +5) ;
  ptrs_loldfff = (BLOK1_1FFF *)(dane + off +7) ;

  ptrs_l->atrybut = Ausuniety /*ptrs_lold->atrybut*/ ;
  ptrs_l->obiekt = Opoint ; 
  ptrs_l->obiektt1 = 0;//ptrs_lold->obiektt1 ;
  ptrs_l->obiektt2 = 0;//ptrs_lold->obiektt2 ;
  ptrs_l->obiektt3 = 0;//ptrs_lold->obiektt3 ; 
  ptrs_l->widoczny = ptrs_lold->widoczny ; 
  ptrs_l->przec = ptrs_lold->przec ; 
  ptrs_l->blok = 0 ; 
  ptrs_l->n = 12 ;  
  ptrs_l->warstwa = 0 ; 
  ptrs_l->kolor = 7 ; 
  ptrs_l->x = 0 ;
  ptrs_l->y = 0 ;
  /*zmiana dlugosci bloku */
  ptrs_lold->blok=0;
  ptrs_lold->n = 3 + ptrs_loldff->dlugosc_opisu_obiektu;
  
 /* b_simple = ((b_simple == TRUE && ptrs_l->blok == 0) ? TRUE : FALSE) ; */
   b_simple = TRUE;
  
 return ((NULL != Change_Object1_1 ((void *)ptrs_lold, (void *)ptrs_l,
		b_simple)) ? TRUE : FALSE) ;
		
}



static BOOL ver1_1_to_2_0 (long_long off, long_long offk)
/*-----------------------------------------------*/
{
  NAGLOWEK1_1 *nag ;
  long_long ad, size ;
  BOOL b_ret ;
/*  int ii; */

  b_ret = TRUE ;
  Object_Tok_All (&off, offk, &ad, ONieOkreslony) ;
  while (ad != -1)
  {
    size = dane_size ;
    nag = (NAGLOWEK1_1*)(dane + ad) ;
    switch (nag->obiekt)
    {
      case Otekst :
	b_ret = ver1_1_to_2_0_text (ad, FALSE /*TRUE*/) ;
	break ;
      case Olinia :
         b_ret = ver1_1_to_2_0_linia (ad, FALSE /*TRUE*/ ) ;
	break ;
      case Oluk :
	b_ret = ver1_1_to_2_0_luk (ad, FALSE /*TRUE*/) ;
	break ;			
      case Ookrag :
	b_ret = ver1_1_to_2_0_okrag (ad, FALSE /*TRUE*/) ;
	break ;	
      case Okolo :
	b_ret = ver1_1_to_2_0_okrag (ad, FALSE /*TRUE*/) ;
	break ;		
      case Owwielokat :
	b_ret = ver1_1_to_2_0_wielokat (ad, FALSE /*TRUE*/) ;
	break ;	
      case Opoint :
	b_ret = ver1_1_to_2_0_point (ad, FALSE /*TRUE*/) ;
	break ;	
      case OdBLOK :
	b_ret = ver1_1_to_2_0_block (ad, FALSE /*TRUE*/) ;
/*	Usun_Object (ad, TRUE) ;  */
	break ;
      default :
	break ;
    }
    if (b_ret == FALSE)
    {
      break ;
    }
    off = ad + sizeof(NAGLOWEK) + ((NAGLOWEK*)(dane + ad))->n ;
    offk += dane_size - size ;
    Object_Tok_All (&off, offk, &ad, ONieOkreslony) ;
  }
  dane_size += 4;
  offk += dane_size - size ;
  return b_ret ;
}


static BOOL ver1_0_to_1_1_text (long off, BOOL b_simple)
/*----------------------------------------------------*/
{
  TEXT_OLD1_0 *ptrs_told ;
  TEXT_OLD1_1 *ptrs_t, s_text = Tdef_1_1 ;

  ptrs_t = &s_text ;
  ptrs_told = (TEXT_OLD1_0 *)(dane + off) ;
  memcpy (ptrs_t, ptrs_told, sizeof(NAGLOWEK1_1)) ;
  ptrs_t->n += 5 ; 	/*zmiana rozmiaru tekstu*/
  ptrs_t->warstwa = ptrs_told->warstwa ;
  ptrs_t->kolor	 = ptrs_told->kolor ;
  ptrs_t->czcionka = ptrs_told->czcionka ;
  ptrs_t->kat = (float) (ptrs_told->kat * Pi / 180) ;
  ptrs_t->wysokosc = ptrs_told->wysokosc ;
  ptrs_t->width_factor = 1 ;
  ptrs_t->x = ptrs_told->x ;
  ptrs_t->y = ptrs_told->y ;
  ptrs_t->italics  = 0 ;
  ptrs_t->reserve  = 0 ;
  ptrs_t->dl = ptrs_told->dl ;
  strncpy (ptrs_t->text, ptrs_told->text, MaxTextLen + 1) ;
  ptrs_t->text [MaxTextLen] = '\0' ;
  b_simple = ((b_simple == TRUE && ptrs_t->blok == 0) ? TRUE : FALSE) ;
  return ((NULL != Change_Object ((void *)ptrs_told, (void *)ptrs_t,
		b_simple)) ? TRUE : FALSE) ;
}

static BOOL ver1_0_to_1_1_block (long_long off)
/*----------------------------------------*/
{
  NAGLOWEK1_1 *nag ;
  long_long offk, ad, size ;
  BOOL b_ret ;

  b_ret = TRUE ;
  offk = off + sizeof(NAGLOWEK1_1) + ((BLOK1_1*)(dane + off))->n - 1 ;
  Object_Tok_All (&off, offk, &ad, ONieOkreslony) ;
  while (ad != -1)
  {
    size = dane_size ;
    nag = (NAGLOWEK1_1*)(dane + ad) ;
    switch (nag->obiekt)
    {
      case Otekst :
	b_ret = ver1_0_to_1_1_text (ad, FALSE) ;
	off = ad + sizeof(NAGLOWEK1_1) + ((NAGLOWEK1_1*)(dane + ad))->n ;
	offk += dane_size - size ;
	break ;
      case OdBLOK :
	break ;
      default :
	break ;
    }
    if (b_ret == FALSE)
    {
      break ;
    }
    Object_Tok_All (&off, offk, &ad, ONieOkreslony) ;
  }
  return b_ret ;
}

static BOOL ver2_0_to_2_1 (long_long off, long_long offk)
/*-----------------------------------------------*/
{
  NAGLOWEK *nag ;
  LINIA *L2_0;
  long_long ad, size ;
  BOOL b_ret ;
  char typ2_1;
  BLOK *adb;
  char  *adh;

  b_ret=TRUE;
  Object_Tok_All (&off, offk, &ad, ONieOkreslony) ;
  while (ad != -1)
  {
    size = dane_size ;
    nag = (NAGLOWEK*)(dane + ad) ;
    switch (nag->obiekt)
    {
      case Olinia :
      case Oluk   :
      case Ookrag : L2_0= (LINIA*)(dane + ad) ;
                    typ2_1=typ_linii2_0_to_2_1(L2_0->typ);
                    L2_0->typ=typ2_1; 
	            break ;
      case OdBLOK : adb = (BLOK*)(dane + ad);
	            adh=(char *)adb+sizeof(NAGLOWEK)+adb->n-1;
     	            change_types_2_0_to_2_1 (adb, adh);
           	    break ;        
      default     :
	            break ;
    }
    off = ad + sizeof(NAGLOWEK) + ((NAGLOWEK*)(dane + ad))->n ;
    offk += dane_size - size ;
    Object_Tok_All (&off, offk, &ad, ONieOkreslony) ;
  }
  return b_ret ;
}


static BOOL ver2_1_to_3_0 (long_long off, long_long offk)
/*-----------------------------------------------*/
{
  NAGLOWEK *nag ;
  long_long ad, size ;
  BOOL b_ret ;
  BLOK *adb;
  TEXT3 *old_text;
  TEXT3 new_text;
  char  *adr;
  int ii, i_t, d_dl;
  double percent_d;
  int percent, percent0;
  unsigned char znacznik1, znacznik2;
  int a;


  percent0=0;

  b_ret=TRUE;
  Object_Tok_All (&off, offk, &ad, ONieOkreslony) ;
  if (now_is_dialog==0) komunikat0(106);
  while (ad != -1)
  {

    percent_d=ad; //-dane;
    percent_d/=dane_size;
    percent_d*=100;
    percent=(int)percent_d;
    if (percent>=percent0)
	{
      ///////////////percentage(percent0);  //!!!!!!
      percent0+=5;
    }


    size = dane_size ;
    nag = (NAGLOWEK*)(dane + ad) ;
    switch (nag->obiekt)
    {
      case Otekst : //zmiana tekstow
                     
		              adr = dane + ad;
					  old_text=(TEXT3 *) adr;
					  //korekta tekstu
				      memmove(&new_text, old_text, sizeof(TEXT3));
                      strcpy(new_text.text,"");
					  new_text.multiline=0;
					  new_text.underline = 0;
					  new_text.encoding = 0;
					  new_text.spacing=6;
					  new_text.n += 2;
					  i_t=0;
					  ii=0;
					  znacznik1 = ((old_text->dl >> 8) & 0xFF);
					  old_text->dl = (old_text->dl & 0xFF);
                      if (old_text->dl>0)
					  {
						new_text.text[0] = znacznik1;
						i_t=1;
						ii=1;
					  }
					  if (old_text->dl>1)
					  {
						  znacznik2 = old_text->multiline + old_text->underline * 2 + old_text->encoding * 4 + old_text->spacing * 8;
						  new_text.text[1] = znacznik2;
						  i_t = 2;
						  ii = 2;
					  }
					  if (old_text->dl>2)
					  {
						d_dl=old_text->dl;
						for (ii=i_t; ii<d_dl; ii++)
						{
							new_text.text[ii]=old_text->text[ii-i_t];
						}
					    
					  }

					  if (old_text->dl > 1000)
						  a = 0;

					  new_text.text[ii]='\0';
					  
                      adr = korekta_obiekt (adr, &new_text);
					  off = adr - dane + sizeof(NAGLOWEK) + new_text.n ;
					 
	            break ;
      case OdBLOK :  adb = (BLOK *) (dane + ad) ;
		             off = ad + sizeof(NAGLOWEK)+B3+adb->dlugosc_opisu_obiektu;
           	    break ;        
      default     :  off = ad + sizeof(NAGLOWEK) + ((NAGLOWEK*)(dane + ad))->n ;
	            break ;
    }
    offk += dane_size - size ;
    Object_Tok_All (&off, offk, &ad, ONieOkreslony) ;
  }
  percentage(percent0);

  if (now_is_dialog==0) komunikat0(0);
  return b_ret ;
}


unsigned int mazovia2utf8char(unsigned char mazovia_code)
{
	unsigned int int_code;

	switch (mazovia_code)
	{
	case 134: int_code = 261;  ////a  
		break;
	case 141: int_code = 263;  ////c
		break;
	case 143: int_code = 260;  ////A
		break;
	case 144: int_code = 280;  ////E
		break;
	case 145: int_code = 281;  ////e
		break;
	case 146: int_code = 322;  ////l
		break;
	case 149: int_code = 262;  ////C
		break;
	case 152: int_code = 346;  ////S
		break;
	case 156: int_code = 321;  ////L
		break;
	case 158: int_code = 347;  ////s
		break;
	case 160: int_code = 377;  ////Z'
		break;
	case 161: int_code = 379;  ////Z.
		break;
	case 162: int_code = 243;  ////o
		break;
	case 163: int_code = 211;  ////O
		break;
	case 164: int_code = 324;  ////n
		break;
	case 165: int_code = 323;  ////N
		break;
	case 166: int_code = 378;  ////z.
		break;
	case 167: int_code = 380;  ////z'
		break;
	case 248: int_code = 176;  //deg
		break;
	case 249: int_code = 183;  ////middle dot
		break;
	case 253: int_code = 0xB2;  ////square
		break;
	case 171: int_code = 189;  ////1/2
		break;
	case 172: int_code = 188;  ////1/4
		break;
	case 0xE0: int_code = 0x3B1; //// alpha
		break;
	case 0xE1: int_code = 0x3B2; //// beta
		break;
	case 0xE2: int_code = 0x3B3; //// gamma
		break;
	case 0xE3: int_code = 0x3A0; //// Pi
		break;
	case 0xE4: int_code = 0x3A3; //// Sigma
		break;
	case 0xE5: int_code = 0x3C3; //// sigma
		break;
	case 0xE6: int_code = 0x3BC; //// mi
		break;
	case 0xE7: int_code = 0x3C4; // tal
		break;
	case 0xE8: int_code = 0xD8; //// fi  or F8 small
		break;
	case 0xEA: int_code = 0x3A9; //// Omega
		break;
	case 0xEB: int_code = 0x3B4; //// delta
		break;
	case 0xEE: int_code = 0x3B5; //// sigma
		break;
	case 0xF1: int_code = 0xB1; //// plus minus
		break;
	case 0xFC: int_code = 0xB3; // cubic
		break;
	case 0xF6: int_code = 0xF7; // division
		break;
	default:
		if (mazovia_code >= 127) int_code = 32; //returns space for unknown / not supported letters
		else int_code = mazovia_code;
		break;
	}
	return int_code;
}

int mazovia2utf8(char *mazoviatext, char *utf8text, int maxlen)
{
	unsigned char *zn;
	unsigned char *utf8ptr;
	int bytes_n;
	unsigned char utf8c[4];
	int len;

	zn = mazoviatext;
	utf8ptr = utf8text;
	len = 0;

	while (*zn != '\0')
	{
		if (*zn >= 127)
		{
			bytes_n = ucs2_to_utf8(mazovia2utf8char(*zn), utf8c);
			if ((*zn < 1920) && (bytes_n < 3))
			{
				if ((len + 2) <= maxlen)
				{
					*utf8ptr = utf8c[0];  //0xC4  196
					utf8ptr++;
					*utf8ptr = utf8c[1]; // 0x84  132
					utf8ptr++;
					len += 2;
				}
			}
			else
			{
				if ((len + 1) <= maxlen)
				{
					*utf8ptr = 32;
					utf8ptr++;
					len++;
				}
			}
		}
		else
		{
			if ((len + 1) <= maxlen)
			{
				*utf8ptr = zn[0];
				utf8ptr++;
				len++;
			}
		}
		zn++;
	}
	*utf8ptr = '\0';
	return len;
}

static BOOL ver4_0_to_4_1(long_long off, long_long offk, char *block_type)
/*-----------------------------------------------------------------------*/
{
	NAGLOWEK *nag;
	long_long ad, size;
	BOOL b_ret;
	BLOK *adb;
	char *adb_address;
	TEXT3 *old_text;
	TEXT new_text;
	BLOK new_block;
	char  *adr;
	int ii, i_t, d_dl;
	double percent_d;
    int percent, percent0;
	unsigned char znacznik1, znacznik2;
	char nameUTF8[maxlen_w];
	int len;
    T_Desc_Ex_Block *ptrs_desc_bl;
    T_Desc_Ex_Block *new_ptrs_desc_bl;
	char new_sz_type[MaxLen];
	int len_desc_old_text;
	int len_desc_new_text;
	int len_desc_new;
	int len_text_old;
    int blok_no=0;

	int a;

	percent0 = 0;

	//converting texts

	b_ret = TRUE;
	Object_Tok_All(&off, offk, &ad, ONieOkreslony);
	while (ad != -1)
	{
		
		percent_d = ad; //-dane;
		percent_d /= dane_size;
		percent_d *= 100;
		percent = (int)percent_d;
		if (percent >= percent0)
		{
			if (dane_size>1000000L) percentage(percent0);
			percent0 += 1;
		}
		

		size = dane_size;
		nag = (NAGLOWEK*)(dane + ad);
		switch (nag->obiekt)
		{
		case Otekst: //changing teksts to UTF-8

			adr = dane + ad;
			old_text = (TEXT3 *)adr;
			//text correction and adding sizes
			
			len_text_old = strlen(old_text->text);

			memmove(&new_text, old_text, sizeof(TEXT3));

			len=mazovia2utf8(old_text->text, &new_text.text, MaxMultitextLen + 1);
			
			new_text.dl = strlen(new_text.text);

			if (new_text.dl > 1000)
				a = 0;
			new_text.n = T18 + new_text.dl;

			new_text.width = 0;
			new_text.height = 0;
			
			adr = korekta_obiekt(adr, &new_text);
			off = (long_long)adr - (long_long)dane + sizeof(NAGLOWEK) + new_text.n;

			break;
		case OdBLOK:  adb = (BLOK *)(dane + ad);

            new_sz_type[0]='\0';

			if (adb->dlugosc_opisu_obiektu > 0)
			{
				int len_desc_old = adb->dlugosc_opisu_obiektu;
				if (len_desc_old > 17)  //int + float + int + \0
				{
                    ptrs_desc_bl = (T_Desc_Ex_Block *)(&adb->opis_obiektu[0]);
					len_desc_old_text = ptrs_desc_bl->len;

					memmove(&new_block, adb, sizeof(BLOK));
					len_desc_new_text = mazovia2utf8(ptrs_desc_bl->sz_type, &new_sz_type, MaxLen) + 1;
					len_desc_new = len_desc_old + (len_desc_new_text - len_desc_old_text);
					if (len_desc_new_text == len_desc_old_text)
					{
						strncpy(ptrs_desc_bl->sz_type, &new_sz_type, len_desc_old_text);
						off = ad + sizeof(NAGLOWEK) + B3 + adb->dlugosc_opisu_obiektu;
					}
					else
					{
                        if ((new_ptrs_desc_bl = (T_Desc_Ex_Block*)malloc(sizeof(T_Desc_Ex_Block_4_1) + len_desc_new_text + 1)) != NULL)
                        {
							new_ptrs_desc_bl->flags = ptrs_desc_bl->flags;
							new_ptrs_desc_bl->x = ptrs_desc_bl->x;
							new_ptrs_desc_bl->y = ptrs_desc_bl->y;
							new_ptrs_desc_bl->len = len_desc_new_text;
							strcpy(new_ptrs_desc_bl->sz_type, &new_sz_type);

							adb_address = Change_Block_Descript(adb, (void *)new_ptrs_desc_bl, len_desc_new);
							free(new_ptrs_desc_bl);
						}
						off = adb_address - dane + sizeof(NAGLOWEK) + B3 + ((BLOK *)(adb_address))->dlugosc_opisu_obiektu;
					}
				}
				else off = ad + sizeof(NAGLOWEK) + B3 + adb->dlugosc_opisu_obiektu;
			}
			else off = ad + sizeof(NAGLOWEK) + B3 + adb->dlugosc_opisu_obiektu;
            if ((blok_no==0) && (block_type!=NULL)) strcpy(block_type, &new_sz_type);
                blok_no++;
			break;
		default:  off = ad + sizeof(NAGLOWEK) + ((NAGLOWEK*)(dane + ad))->n;
			break;
		}
		offk += dane_size - size;
		Object_Tok_All(&off, offk, &ad, ONieOkreslony);
	}

	if (dane_size > 1000000L) percentage(percent0);

	return TRUE;
}


static BOOL ver4_1_to_4_2(long_long off, long_long offk)
{
    NAGLOWEK *nag;
    long_long ad, size;
    BOOL b_ret;
    BLOK *adb;
    char *adb_address;
    TEXT3 *old_text;
    TEXT new_text;
    BLOK new_block;
    char  *adr;
    int ii, i_t, d_dl;
    double percent_d;
    int percent, percent0;
    unsigned char znacznik1, znacznik2;
    char nameUTF8[maxlen_w];
    int len;
    T_Desc_Ex_Block *ptrs_desc_bl;
    T_Desc_Ex_Block *new_ptrs_desc_bl;
    char new_sz_type[MaxLen];
    int len_desc_old_text;
    int len_desc_new_text;
    int len_desc_new;
    int len_text_old;

    return TRUE;

    percent0 = 0;

    //converting texts

    b_ret = TRUE;
    Object_Tok_All(&off, offk, &ad, ONieOkreslony);
    while (ad != -1)
    {

        percent_d = ad; //-dane;
        percent_d /= dane_size;
        percent_d *= 100;
        percent = (int)percent_d;
        if (percent >= percent0)
        {
            if (dane_size>1000000L) percentage(percent0);
            percent0 += 1;
        }


        size = dane_size;
        nag = (NAGLOWEK*)(dane + ad);
        switch (nag->obiekt)
        {
            case OdBLOK:  adb = (BLOK *)(dane + ad);

                if (adb->dlugosc_opisu_obiektu > 0)
                {
                    int len_desc_old = adb->dlugosc_opisu_obiektu;
                    if (len_desc_old > 17)  //int + float + int + \0
                    {
                        ptrs_desc_bl = (T_Desc_Ex_Block *)(&adb->opis_obiektu[0]);
                        len_desc_old_text = ptrs_desc_bl->len;

                        len_desc_new = len_desc_old + (sizeof(float)*3);  //added scalex, scaley and angle

                        if ((new_ptrs_desc_bl = (T_Desc_Ex_Block*)malloc(sizeof(T_Desc_Ex_Block_4_1) + len_desc_new_text + 1)) != NULL)
                        {
                            new_ptrs_desc_bl->flags = ptrs_desc_bl->flags;
                            new_ptrs_desc_bl->x = ptrs_desc_bl->x;
                            new_ptrs_desc_bl->y = ptrs_desc_bl->y;
                            new_ptrs_desc_bl->len = len_desc_new_text;
                            strcpy(new_ptrs_desc_bl->sz_type, &new_sz_type);

                            adb_address = Change_Block_Descript(adb, (void *)new_ptrs_desc_bl, len_desc_new);
                            free(new_ptrs_desc_bl);
                        }
                        off = adb_address - dane + sizeof(NAGLOWEK) + B3 + ((BLOK *)(adb_address))->dlugosc_opisu_obiektu;
                    }
                    else off = ad + sizeof(NAGLOWEK) + B3 + adb->dlugosc_opisu_obiektu;
                }
                else off = ad + sizeof(NAGLOWEK) + B3 + adb->dlugosc_opisu_obiektu;
                break;
            default:  off = ad + sizeof(NAGLOWEK) + ((NAGLOWEK*)(dane + ad))->n;
                break;
        }
        offk += dane_size - size;
        Object_Tok_All(&off, offk, &ad, ONieOkreslony);
    }

    if (dane_size > 1000000L) percentage(percent0);

    return TRUE;
}


static void ver4_0_to_4_1_Layer(void)
{
	int i, len;
	char nameUTF8[maxlen_w];

	//converting Layers
	for (i = 0; i < No_Layers; i++)
	{
		len = mazovia2utf8(Layers[i].name, &nameUTF8, maxlen_w);
		strncpy(Layers[i].name, &nameUTF8, maxlen_w);
		Layers[i].len_name = strlen(Layers[i].name);
	}
}

void ver4_0_to_4_1_Spec(T_spec_name *ptrs_specs, int i_spec_no)
{
	int i, len;
	T_spec_name *table_spec;
	char old_spec_value[Max_Spec_Value];
	char old_spec_name[Max_Spec_Name];
	char new_spec_value[Max_Spec_Value];
	char new_spec_name[Max_Spec_Name];

	for (i = 0; i < i_spec_no; i++)
	{
		
		strcpy(&old_spec_name, ptrs_specs[i].spec_name);
		len = mazovia2utf8(&old_spec_name, &new_spec_name, Max_Spec_Name);
		strcpy(ptrs_specs[i].spec_name, &new_spec_name);

		strcpy(&old_spec_value, ptrs_specs[i].spec_value);
		len = mazovia2utf8(&old_spec_value, &new_spec_value, Max_Spec_Value);
		strcpy(ptrs_specs[i].spec_value, &new_spec_value);
	}
}

static void ver4_0_to_4_1_Table(void)
{
	int i, len;
	T_spec_name *table_spec;
	char old_spec_value[Max_Spec_Value];
	char old_spec_name[Max_Spec_Name];
	char new_spec_value[Max_Spec_Value];
	char new_spec_name[Max_Spec_Name];
	
	//converting nameplate

	for (i = 0; i < Max_No_Spec; i++)
	{
		get_spec_name(i, &old_spec_name);
		len = mazovia2utf8(&old_spec_name, &new_spec_name, Max_Spec_Name);
		put_spec_name(i, &new_spec_name);

		get_spec_string(i, &old_spec_value);
		len = mazovia2utf8(&old_spec_value, &new_spec_value, Max_Spec_Value);
		put_spec_string(i, &new_spec_value);
	}

}

static void change_textstyle_global(char *def_text_font)
{
	*def_text_font = PTRS__Font_Table.new_no[(int)def_text_font[0]];
}

static BOOL change_textstyle_indexes(long_long off, long_long offk)
/*---------------------------------------------------------------*/
{
	NAGLOWEK *nag;
	long_long ad, size;
	BOOL b_ret;
	BLOK *adb;
	TEXT *text;
	char  *adr;
	int ii, i_t, d_dl;
	double percent_d;
	int percent, percent0;
	unsigned char znacznik1, znacznik2;


	percent0 = 0;

	b_ret = TRUE;
	Object_Tok_All(&off, offk, &ad, ONieOkreslony);
	while (ad != -1)
	{

		size = dane_size;
		nag = (NAGLOWEK*)(dane + ad);
		switch (nag->obiekt)
		{
		case Otekst: //zmiana tekstow
			adr = dane + ad;
			text = (TEXT *)adr;
			//korekta indeksu tekstu
			if ((PTRS__Font_Table.zeroing[text->czcionka] == TRUE) || (PTRS__Font_Table.type[text->czcionka] == 255))
			{
				text->width = 0;
				text->height = 0;
			}
			text->czcionka = PTRS__Font_Table.new_no[text->czcionka];
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
	return b_ret;
}


static BOOL ver1_0_to_1_1 (long_long off, long_long offk)
/*-----------------------------------------------*/
{
  NAGLOWEK1_1 *nag ;
  long_long ad, size ;
  BOOL b_ret ;

  b_ret = TRUE ;
  Object_Tok_All (&off, offk, &ad, ONieOkreslony) ;
  while (ad != -1)
  {
    size = dane_size ;
    nag = (NAGLOWEK1_1*)(dane + ad) ;
    switch (nag->obiekt)
    {
      case Otekst :
	b_ret = ver1_0_to_1_1_text (ad, TRUE) ;
	break ;
      case OdBLOK :
	b_ret = ver1_0_to_1_1_block (ad) ;
	break ;
      default :
	break ;
    }
    if (b_ret == FALSE)
    {
      break ;
    }
    off = ad + sizeof(NAGLOWEK1_1) + ((NAGLOWEK1_1*)(dane + ad))->n ;
    offk += dane_size - size ;
    Object_Tok_All (&off, offk, &ad, ONieOkreslony) ;
  }
  return b_ret ;
}


static int read_file_ver1_0 (int f, BOOL err_message)
/*---------------------------------------------------*/
{
  long_long l;
  double sk_xx ;
  int skalaf ;
  ZMIENNE_OLD1_0 s_dim_vars_old ;
  BOOL b_ret ;
  int error_code = 50;

    long_long del_dane0;
    char *dane00;

    dane0 = dane,
    dane00 = dane0;

  if (read(f,&Xp,8)!=8) goto error;
  if (read(f,&Yp,8)!=8) goto error;
  if (read(f,&Xmin,8)!=8) goto error;
      if (read(f,&Ymin,8)!=8) goto error;
  if (read(f,&skala,8)!=8) goto error;
  if(skala<=0) goto error;
  if (read(f,&X_max,8)!=8) goto error;
      if (read(f,&Y_max,8)!=8) goto error;
  Xmax=X_max+Xmin;
  Ymax=Y_max+Ymin;
  if (read(f,&X,8)!=8) goto error;
  if (read(f,&Y,8)!=8) goto error;
  if (read(f,&sk_xx,8)!=8) goto error;
  if (read(f,&krok_s,8)!=8) goto error;
  if (read(f,&krok_g,8)!=8) goto error;
  if (krok_s<=0 || krok_g<=0 || X_max<=0 || Y_max<=0) goto error;
  if (read(f,&grid_,2)!=2) goto error;
  if (read(f,&snap_,2)!=2) goto error;
  if (read(f,&X_Y_kursora,2)!=2) goto error;
  if (read(f,&No_Layers,1)!=1) goto error;
  if (read(f,Layers,MAX_OLD_NUMBER_OF_LAYERS*sizeof(LAYER))!=
    MAX_OLD_NUMBER_OF_LAYERS*sizeof(LAYER)) goto error;
  Current_Layer=0;
  Layers [0].on = 1;
  if (No_Layers == 0)
  {
    No_Layers = 1;
    Layers [0].on = 1;
    Layers [0].edit = 1;
    Layers [0].point = 1;
    Layers [0].color = 7;  /*bialy*/
    Layers [0].line_type = 5;
    Layers [0].len_name = 1;
    strcpy (Layers [0].name, "0");
    No_Layers = 2;
    Layers [1].on = 1;
    Layers [1].edit = 1;
    Layers [1].point = 1;
    Layers [1].color = 7;  /*bialy*/
    Layers [1].line_type = 5;
    Layers [1].len_name = 1;
    strcpy (Layers [1].name, "1");
  }
  if (read(f,&FormatX,2)!=2) goto error;
  if (read(f,&FormatY,2)!=2) goto error;
  if (read(f,&skalaf,2)!=2) goto error;
  SkalaF = (double)skalaf ;
  if (read(f,&DokladnoscF,8)!=8) goto error;
  if (read(f,&Jednostki,8)!=8) goto error;
  if (read(f,&df__text_height,8)!=8) goto error;

  df__text_width_factor = 1 ;
  b__italics = 0 ;

  if (read(f,&s_dim_vars_old,sizeof(s_dim_vars_old))!=sizeof(s_dim_vars_old)) goto error;
    zmwym.Lkolor = s_dim_vars_old.Lkolor ;
    zmwym.Tkolor = s_dim_vars_old.Tkolor  ;
    zmwym.wysokosc = s_dim_vars_old.wysokosc ;
    zmwym.width_factor = 1 ;
    zmwym.dokladnosc = s_dim_vars_old.dokladnosc ;
    zmwym.linia_ob = s_dim_vars_old.linia_ob ;
    zmwym.strzalka = s_dim_vars_old.strzalka ;
    zmwym.kierunek_t = s_dim_vars_old.kierunek_t ;
    zmwym.b_add_line_const = 1 ;
    zmwym.czcionka = 0 ;
    zmwym.bold = 0;
    zmwym.italics = 0 ;
    zmwym.collinear=0;
    zmwym.format = 0 ;
  if(FormatX<=0 || FormatY<=0 || SkalaF<=0 || DokladnoscF<=0 ||
     Jednostki<=0 || df__text_height <=0) goto error;  /*sprawdz zmwym !!*/
    if((l=filelength(f))==-1)goto error;
  l-=tell(f);
  if(!SetBufferDaneSize(l)) { error_code = 51; goto error18; }

    del_dane0=dane0-dane00;
    if (del_dane0!=0)
    {
        ;
    }


  dane_size=l;
  dane-=l-sizeof(NAGLOWEK);    /*bardzo nie eleganckie, poprawic!!!!*/
  if (czytaj(f,(char  *)(dane),l)==-1) goto error;
  Set_Version (VERSION1_0) ;
  b_ret = Check_Objects (0, dane_size - 1, TRUE) ;
  Set_Version (VERSION1_1) ;
  if (b_ret == FALSE) goto error;
  return IO_RET_OK ;
  error:
    { zwolnienie_pamieci();
      if (err_message == TRUE)
	 ErrListCode(45, error_code);
      return IO_RET_RW_ERR ;
    }
  error18:
    {
      if (err_message == TRUE)
	 ErrListCode(18, error_code);
      return IO_RET_MEM_ERR ;
    }
}

static int read_old_ver_file_1_0 (int f, int alfa_version /*char *blok_naglowka*/, BOOL err_message)
/*------------------------------------------------------------------------*/
{
  int i_ret ;

  i_ret = IO_RET_OK ;
  if (alfa_version==ver1_0)
  {
    InfoList (4) ;
    i_ret = read_file_ver1_0 (f, err_message) ;
    if (i_ret == IO_RET_OK)
    {
      if (FALSE == ver1_0_to_1_1 (0, dane_size - 1))
      {
	i_ret = IO_RET_MEM_ERR ;
      }
      
      if (FALSE == ver1_1_to_2_0 (0, dane_size - 1))
      {
	i_ret = IO_RET_MEM_ERR ;
      }
      
    }
    
    if (Error == 0)
     ClearInfo () ;
  }
  else
  {
    i_ret = IO_RET_FILE_TYPE_ERR ;
  }
  return i_ret ;
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

long czytaj(int f, char  *buf, unsigned long_long nbyte)
{
 char  *buf1;
 unsigned l;
 int i,k,k1;
 l=32*1024;
 buf1=buf;
 k = (int) (nbyte / l);
 k1 = (int) (nbyte % l);

 for(i=1;i<=k;i++)
	{if(read(f,(char*)buf1,l)==-1) return-1;
	 buf1+=l;}

 if(read(f,(char*)buf1,k1)==-1) return -1;
 return nbyte ;
}

long pisz(int f, char  *buf, unsigned long_long nbyte)
{
 unsigned l;
 int i,k,k1;
 char  *buf1;
 l=32*1024;
 buf1=buf;
 k = (int) (nbyte / l);
 k1 = (int) (nbyte % l);
 for(i=1;i<=k;i++)
	{if(write(f,(char*)buf1,l)==-1) return -1;
	 buf1+=l;}
 if(write(f,(char*)buf1,k1)==-1) return -1;
 return nbyte ;
}

static BOOL read_write_param (int f, int (*proc_io) (int, void*, unsigned), BOOL read_Xp_Yp, int opcja_warstw, int *numer_bledu, int *pattern_library_no_var)
/*---------------------------------------------------------------------------------------------------------------------------------------------------------*/
{
#define RES_LEN 4 //12
  const char PROFIL_TEMP_FILE [] = "ALFPROF.TMP" ;
  int i_f_handle ;
  char sz_reserve [RES_LEN] ;
  int i_layersno, i, iin ;
  float skala_p_x_f;
  float rpp_f;
  int l_kr;
  int zn_b1, zn_b2 ;
  double zn_b1d;
  double local_x, local_y;
  double dl_z_b_2;
  MyDane dane_profs;
  MyDane dane_profs0={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  double nul_Xp, nul_Yp ;
  double rezerwa;
  int rezerwa_int;
  char rezerwa_char;
  double angle_l;
  LAYER2_0 Layer2_0;
  LAYER3 Layer3;

  int s_len, s_ext_len;
  int null_var_int;
  float null_var_float;
  int pattern_library_no_var_int;
  PRIV_PRN_WINDOW prn_window;


  memset (sz_reserve, 0, RES_LEN) ;
  KursorS = 0;
  df__text_type = 0;
  df__text_justowanie = 0;
  df__text_ukryty = 0;
  df__text_czcionka = 0;
  *numer_bledu = 181;
  if (read_Xp_Yp==TRUE)
   {
    if (proc_io (f, &Xp, sizeof(double)) != sizeof(double)) return FALSE ;
    if (proc_io (f, &Yp, sizeof(double)) != sizeof(double)) return FALSE ;
   }
   else
    {
    if (proc_io (f, &nul_Xp, sizeof(double)) != sizeof(double)) return FALSE ;
    if (proc_io (f, &nul_Yp, sizeof(double)) != sizeof(double)) return FALSE ;
    }
  *numer_bledu = 182;
  if (proc_io (f, &Xmin, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &Ymin, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &skala, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &X_max, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &Y_max, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &X, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &Y, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &krok_s, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &krok_g, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &grid_, sizeof(int)) != sizeof(int)) return FALSE ;
  if (proc_io (f, &snap_, sizeof(int)) != sizeof(int)) return FALSE ;
  if (proc_io (f, &Current_Layer, sizeof(int)) != sizeof(int)) return FALSE ;
  if (proc_io (f, &No_Layers, sizeof(int)) != sizeof(int)) return FALSE ;
  *numer_bledu = 183;
  i_layersno = (No_Layers <= 16) ? MAX_OLD_NUMBER_OF_LAYERS : No_Layers ;
  if (opcja_warstw==0)
  {
   for (i=0; i<i_layersno; i++)
   {
    if (proc_io (f, &Layer2_0, sizeof(LAYER2_0)) != sizeof(LAYER2_0)) return FALSE ;
    Layers [i].on    = Layer2_0.on;
    Layers [i].edit  = Layer2_0.edit;
    Layers [i].point = Layer2_0.point;
    Layers [i].grey = Layer2_0.grey;
	Layers [i].bw = Layer2_0.bw;
	Layers [i].reserve = Layer2_0.reserve;
    Layers [i].color = Layer2_0.color; 
    Layers [i].line_type = typ_linii2_0_to_2_1(Layer2_0.line_type); /*tu nalezy zmienic typ linii*/
    Layers [i].len_name = Layer2_0.len_name;
    strcpy (Layers [i].name, Layer2_0.name);
   }
  }
  else if (opcja_warstw==1)
   {
	  *numer_bledu = 184;
	  for (i = 0; i < i_layersno; i++)
	  {
		  if (proc_io(f, &Layer3, sizeof(LAYER3)) != sizeof(LAYER3)) return FALSE;
		  Layers[i].on = Layer3.on;
		  Layers[i].edit = Layer3.edit;
		  Layers[i].point = Layer3.point;
		  Layers[i].grey = Layer3.grey;
		  Layers[i].bw = Layer3.bw;
		  Layers[i].reserve = Layer3.reserve;
		  Layers[i].color = Layer3.color;
		  Layers[i].line_type = Layer3.line_type; 
		  Layers[i].len_name = Layer3.len_name;
		  strcpy(Layers[i].name, Layer3.name);
	  }
   }
  else  //only for ver4_1
  {
	  char *layersBuf = (Bytef *)malloc(sizeof(LAYER)*MAX_NUMBER_OF_LAYERS + 1024);
	  uLongf destLen;
	  int destLenInt;
	  int ret;
	  uLong destLenIntlong;
	  uLong sourceLen;

	  *numer_bledu = 185;
	  if (proc_io == write)
	  {	  
		  i_layersno = (No_Layers <= 16) ? MAX_OLD_NUMBER_OF_LAYERS : No_Layers;
		  sourceLen = sizeof(LAYER)*i_layersno;
		  destLen = compressBound(sourceLen);

		  ret = compress(layersBuf, &destLen, &Layers, (uLong)sizeof(LAYER));
		  if (ret != Z_OK)
		  {
			  if (ret == Z_MEM_ERROR) *numer_bledu = 186;
			  else if (ret == Z_BUF_ERROR) *numer_bledu = 187;
			  return FALSE;
		  }
          destLenInt = (int)destLen;
	  }
	  *numer_bledu = 188;
	  if (proc_io(f, &destLenInt, sizeof(int)) != sizeof(int)) { free(layersBuf); return FALSE; }
	  if (proc_io(f, layersBuf, destLenInt) != destLenInt) { free(layersBuf); return FALSE; }
	  if (proc_io == write)
          free(layersBuf);
	  *numer_bledu = 189;
	  if (proc_io == read)
	  {

		  destLenIntlong = destLenInt;
		  destLen= sizeof(LAYER)*MAX_NUMBER_OF_LAYERS + 1024;
		  ret = uncompress(&Layers, &destLen, layersBuf, destLenIntlong);
		  //memmove(&Layers, layersBuf, destLen);
		  free(layersBuf);
		  if (ret != Z_OK)
		  {
			  if (ret == Z_MEM_ERROR) { *numer_bledu = 190; return FALSE; }
			  else if (ret == Z_BUF_ERROR) { *numer_bledu = 191; return FALSE; }
			  else if (ret == Z_DATA_ERROR) { *numer_bledu = 192; return FALSE; }
		  }
		  *numer_bledu = 193;
		  if ((int)destLen != sizeof(LAYER)*i_layersno) return FALSE;
	  }
  }

  for (int li = 0; li < i_layersno; li++)
  {
      if (!is_utf8(Layers[li].name))
          decodingwin_true(Layers[li].name);
  }
    
  *numer_bledu = 194;
  if (proc_io (f, &FormatX, sizeof(int)) != sizeof(int)) return FALSE ;
  if (proc_io (f, &FormatY, sizeof(int)) != sizeof(int)) return FALSE ;
  if (proc_io (f, &SkalaF,sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &DokladnoscF, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &Jednostki, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &df__text_height, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &df__text_width_factor, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &b__italics, sizeof(int)) != sizeof(int)) return FALSE ;
    b__bold = (b__italics & 2);
    b__italics &= 1;
  *numer_bledu = 195;
  if (proc_io (f, &zmwym, sizeof(zmwym)) != sizeof(zmwym)) return FALSE ;
  if (proc_io (f, &orto, sizeof(int)) != sizeof(int)) return FALSE ;
  if (proc_io (f, &df__Point_Size, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &b__Point_View, sizeof(int)) != sizeof(int)) return FALSE ;
  if (proc_io (f, &df__Trace_Width, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &df__Sketch_Dist_Max, sizeof(double)) != sizeof(double)) return FALSE ;

    long l_off_ = lseek (f, 0, SEEK_CUR);

  if (proc_io (f, &w__spec_no, sizeof(int)) != sizeof(int)) return FALSE ;
  if (proc_io (f, &l__dane_size, 4) != 4) return FALSE ;
  skala_p_x_f=(float)get_skala_profilu_x();
  rpp_f=(float)get_poziom_pp();
  *numer_bledu = 196;
  if (proc_io (f, &skala_p_x_f, sizeof(float)) != sizeof(float)) return FALSE ;
  if (proc_io (f, &rpp_f, sizeof(float)) != sizeof(float)) return FALSE ;
  l_kr=put_skala_profilu_x(skala_p_x_f);
  l_kr=put_poziom_pp(rpp_f);

  if (proc_io (f, &zn_b1, sizeof(int)) != sizeof(int)) return FALSE ;
  if (proc_io (f, &zn_b2, sizeof(int)) != sizeof(int)) return FALSE ;

  local_x=0;
  local_y=0;
  angle_l=0;
  l_kr=put_angle_l(angle_l);
  *numer_bledu = 197;

  if ((zn_b1==1) && ((zn_b2==2) || (zn_b2==3)))
  {
  /* nowa wersja */
  if (proc_io (f, &local_x, sizeof(double)) != sizeof(double)) return FALSE ;
  if (proc_io (f, &local_y, sizeof(double)) != sizeof(double)) return FALSE ;

  l_kr=put_znacznik_aplikacji(0);

  
  if (proc_io(f, &TRANSLUCENCY, sizeof(int)) != sizeof(int)) return FALSE;

  if ((TRANSLUCENCY < 77) || (TRANSLUCENCY > 255))  TRANSLUCENCY = 179;

  reset_trans_blender();
  
  if (proc_io(f, &pattern_library_no_var_int, sizeof(int)) != sizeof(int)) return FALSE;

  *pattern_library_no_var = pattern_library_no_var_int;


  if (opcja_warstw == 2) {
      if (proc_io(f, &prn_window.xp, sizeof(double)) != sizeof(double)) return FALSE;
      if (proc_io(f, &prn_window.yp, sizeof(double)) != sizeof(double)) return FALSE;
      if (proc_io(f, &prn_window.xk, sizeof(double)) != sizeof(double)) return FALSE;
      if (proc_io(f, &prn_window.yk, sizeof(double)) != sizeof(double)) return FALSE;

      set_priv_prn_window(prn_window.xp, prn_window.yp, prn_window.xk, prn_window.yk);
      iin = 0;
  }
  else
  {
      set_priv_prn_window(0.0, 0.0, 0.0, 0.0);
      iin = 6;
  }

     for (i = 0; i < iin; i++)
     {
        if (proc_io(f, &null_var[i], sizeof(double)) != sizeof(double)) return FALSE;
     }

      float magnitude;

      int marker;

      long l_off___ = lseek (f, 0, SEEK_CUR);
      if (proc_io(f, &marker, sizeof(int)) != sizeof(int)) return FALSE;
      if (proc_io(f, &null_var_float, sizeof(float)) != sizeof(float)) return FALSE;

      if (marker==1234567)
      {
          if ((null_var_float>0) && (null_var_float<=10000)) q_magnitude=null_var_float;
          else q_magnitude=q_magnitude0;

          if (proc_io(f, &static_stress_colors, sizeof(STATIC_STRESS_COLORS)) != sizeof(STATIC_STRESS_COLORS)) return FALSE;

          if (proc_io(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
          if (Check_if_LE(magnitude, 0.0)) s_magnitude=s_magnitude0; else s_magnitude=(double)magnitude;

          if (proc_io(f, &stress_precision, sizeof(double)) != sizeof(double)) return FALSE;

          if (Check_if_LE(stress_precision, 0.0)) stress_precision=stress_precision0;
      }
      else
      {
          memmove(&static_stress_colors, &static_stress_colors0, sizeof(STATIC_STRESS_COLORS));
          s_magnitude=s_magnitude0;
          q_magnitude=q_magnitude0;
          stress_precision=stress_precision0;
      }

      if (proc_io(f, &static_colors, sizeof(STATIC_COLORS)) != sizeof(STATIC_COLORS)) return FALSE;
      if (static_colors.node_element_color==0) static_colors.node_element_color=static_colors0.node_element_color;
      if (static_colors.tension_color==0) static_colors.tension_color=static_colors0.tension_color;
      if (static_colors.compression_color==0) static_colors.compression_color=static_colors0.compression_color;
      if (static_colors.shear_color==0) static_colors.shear_color=static_colors0.shear_color;
      if (static_colors.moment_color==0) static_colors.moment_color=static_colors0.moment_color;
      if (static_colors.deformation_color==0) static_colors.deformation_color=static_colors0.deformation_color;
      if (static_colors.reaction_color==0) static_colors.reaction_color=static_colors0.reaction_color;
      if (static_colors.dynamic_color==0) static_colors.dynamic_color=static_colors0.dynamic_color;

      if (proc_io(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
      if (Check_if_LE(magnitude, 0.0)) n_magnitude=n_magnitude0; else n_magnitude=(double)magnitude;

      if (proc_io(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
      if (Check_if_LE(magnitude, 0.0)) v_magnitude=v_magnitude0; else v_magnitude=(double)magnitude;

      if (proc_io(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
      if (Check_if_LE(magnitude, 0.0)) m_magnitude=m_magnitude0; else m_magnitude=(double)magnitude;

      if (proc_io(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
      if (Check_if_LE(magnitude, 0.0)) d_magnitude=d_magnitude0; else d_magnitude=(double)magnitude;

      if (proc_io(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
      if (Check_if_LE(magnitude, 0.0)) r_magnitude=r_magnitude0; else r_magnitude=(double)magnitude;

      if (proc_io(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
      if (Check_if_LE(magnitude, 0.0)) rm_magnitude=rm_magnitude0; else rm_magnitude=(double)magnitude;

      if (proc_io(f, &thermal_precision, sizeof(double)) != sizeof(double)) return FALSE;
      if (Check_if_LE(thermal_precision, 0.0)) thermal_precision=thermal_precision0;
      if (proc_io(f, &load_precision, sizeof(double)) != sizeof(double)) return FALSE;
      if (Check_if_LE(load_precision, 0.0)) load_precision=load_precision0;
      if (proc_io(f, &force_precision, sizeof(double)) != sizeof(double)) return FALSE;
      if (Check_if_LE(force_precision, 0.0)) force_precision=force_precision0;
      if (proc_io(f, &moment_precision, sizeof(double)) != sizeof(double)) return FALSE;
      if (Check_if_LE(moment_precision, 0.0)) moment_precision=moment_precision0;
      if (proc_io(f, &displacement_precision, sizeof(double)) != sizeof(double)) return FALSE;
      if (Check_if_LE(displacement_precision, 0.0)) displacement_precision=displacement_precision0;
      if (proc_io(f, &rotation_precision, sizeof(double)) != sizeof(double)) return FALSE;
      if (Check_if_LE(rotation_precision, 0.0)) rotation_precision=rotation_precision0;

    if (proc_io(f, &thermal_magnitude, sizeof(double)) != sizeof(double)) return FALSE;
    if (Check_if_LE(thermal_magnitude, 0.0)) thermal_magnitude=thermal_magnitude0;
    if (proc_io(f, &load_magnitude, sizeof(double)) != sizeof(double)) return FALSE;
    if (Check_if_LE(load_magnitude, 0.0)) load_magnitude=load_magnitude0;
    if (proc_io(f, &force_magnitude, sizeof(double)) != sizeof(double)) return FALSE;
    if (Check_if_LE(force_magnitude, 0.0)) force_magnitude=force_magnitude0;
    if (proc_io(f, &moment_magnitude, sizeof(double)) != sizeof(double)) return FALSE;
    if (Check_if_LE(moment_magnitude, 0.0)) moment_magnitude=moment_magnitude0;
    if (proc_io(f, &displacement_magnitude, sizeof(double)) != sizeof(double)) return FALSE;
    if (Check_if_LE(displacement_magnitude, 0.0)) displacement_magnitude=displacement_magnitude0;
    if (proc_io(f, &rotation_magnitude, sizeof(double)) != sizeof(double)) return FALSE;
    if (Check_if_LE(rotation_magnitude, 0.0)) rotation_magnitude=rotation_magnitude0;

    if (proc_io(f, &zmwym_fraction, sizeof(int)) != sizeof(int)) return FALSE;
    //if (proc_io(f, &zmwym_reserve, sizeof(int)) != sizeof(int)) return FALSE;  //ignored
      if (proc_io(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
      if (Check_if_LE(magnitude, 0.0)) p_magnitude=p_magnitude0; else p_magnitude=(double)magnitude;

    if (zmwym_fraction<1) zmwym_fraction=64;   //initiation if not set

    *numer_bledu = 198;
    s_ext_len = sizeof(SEKTORY_EXT);

    if (proc_io(f, &sektory_arkusza_ext, sizeof(SEKTORY_EXT)) != sizeof(SEKTORY_EXT)) return FALSE;
    if (sektory_arkusza_ext.dy == 0)
    {
        sektory_arkusza_ext.dy = 75;
        sektory_arkusza_ext.margin_width = 2.5;
        sektory_arkusza_ext.frame_line = 1; // 32;
        sektory_arkusza_ext.sector_line = 2; // 64;
        sektory_arkusza_ext.reversed = 0;
        sektory_arkusza_ext.frame_color = 7;
        sektory_arkusza_ext.sector_color = 7;
        sektory_arkusza_ext.font = 0;
        sektory_arkusza_ext.reserve = 0;
        sektory_arkusza_ext.frame_offset = 0.25;
    }
    s_len = sizeof(SEKTORY);
    if (proc_io(f, &sektory_arkusza, sizeof(SEKTORY)) != sizeof(SEKTORY)) return FALSE;
    if (sektory_arkusza.dx == 0)
    {
        sektory_arkusza.dx = 75;
        sektory_arkusza.first_number = 1;
        sektory_arkusza.prefix = 0;
        sektory_arkusza.draw = 0;
        sektory_arkusza.ramka = 0;
        sektory_arkusza.styl = 0;
    }

    *numer_bledu = 199;

  if (zn_b2==3)
   {
   if (proc_io (f, &angle_l, sizeof(double)) != sizeof(double)) return FALSE ;    /* odczyt kata obrotu ukladu lokalnego */
   l_kr=put_angle_l(angle_l);
   }
    else
     {
     if (proc_io (f, &rezerwa, sizeof(double)) != sizeof(double)) return FALSE ;
     }
 

  if (proc_io (f, &KursorS, sizeof(int)) != sizeof(int)) return FALSE ;
  if (proc_io (f, &df__text_czcionka, sizeof(char)) != sizeof(char)) return FALSE ;
  if (proc_io (f, &df__text_type, sizeof(char)) != sizeof(char)) return FALSE ;
  if (proc_io (f, &df__text_justowanie, sizeof(char)) != sizeof(char)) return FALSE ;
  if (proc_io (f, &df__text_ukryty, sizeof(char)) != sizeof(char)) return FALSE ;


  if (proc_io (f, &rezerwa_char, sizeof(char)) != sizeof(char)) return FALSE ;
  if (rezerwa_char==1) ciagniecie_linii=TRUE; else ciagniecie_linii=FALSE;
  
  if (proc_io (f, &rezerwa_char, sizeof(char)) != sizeof(char)) return FALSE ;
  if (rezerwa_char==1) rozcinanie_linii=TRUE; else rozcinanie_linii=FALSE;
  
  if (proc_io (f, &rezerwa_char, sizeof(char)) != sizeof(char)) return FALSE ;
  if (rezerwa_char==1) ukrywanie_typu=TRUE; else ukrywanie_typu=FALSE;
  
  if (proc_io (f, &rezerwa_char, sizeof(char)) != sizeof(char)) return FALSE ;
  if (rezerwa_char==1) type_view=TRUE; else type_view=FALSE;
  /*swobodny blok*/
  if (proc_io (f, &rezerwa_char, sizeof(char)) != sizeof(char)) return FALSE ;
  if (rezerwa_char==1) swobodny_blok=TRUE; else swobodny_blok=FALSE;
  /*swobodny tekst*/
  if (proc_io (f, &rezerwa_char, sizeof(char)) != sizeof(char)) return FALSE ;
  if (rezerwa_char==1) swobodny_tekst=TRUE; else swobodny_tekst=FALSE;
  /*tryb lokalizacji*/
  if (proc_io (f, &rezerwa_char, sizeof(char)) != sizeof(char)) return FALSE ;
  if (rezerwa_char>0) 
    {
     sel.gor=1;
     sel.nr=rezerwa_char-1;
    }
    else
     {
      sel.gor=0;
      sel.nr=0;
     }
  /*rezerwa*/
  
  normalize_text=options1.normalize_text;
  if (proc_io (f, &options1, sizeof(char)) != sizeof(char)) return FALSE ;

  long l_off__ = lseek (f, 0, SEEK_CUR);

  if (proc_io (f, &dl_z_b_2, sizeof(double)) != sizeof(double)) return FALSE ;
  if (dl_z_b_2>=1)
   {
   if (proc_io (f, &zbior_danych_2, dl_z_b_2+1) != (dl_z_b_2+1)) return FALSE ;
   }
  }

  l_kr=put_localx(local_x);
  l_kr=put_localy(local_y);

  add_zbior_2=1;


  return TRUE ;
}

static BOOL read_write_param1_1 (int f, int (*proc_io) (int, void*, unsigned), BOOL read_Xp_Yp )
/*----------------------------------------------------------------------*/
{
#define RES_LEN 4 //12
  const char PROFIL_TEMP_FILE [] = "ALFPROF.TMP" ;
  int i_f_handle ;
  char sz_reserve [RES_LEN] ;
  int i_layersno, i, iin ;
  float skala_p_x_f;
  float rpp_f;
  int l_kr;
  int zn_b1, zn_b2 ;
  double zn_b1d;
  double local_x, local_y;
  double dl_z_b_2;
  MyDane1_1 dane_profs;
  MyDane1_1 dane_profs0={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  double nul_Xp, nul_Yp ;
  double rezerwa;
  double angle_l;
  ZMIENNE_OLD1_1 zmwym1_1;
  LAYER1_1 Layers1_1;
  LAYER_name_1_1 Layers_name_1_1;
   
  KursorS = 0;
  grid_=0;
  snap_=0;
  Current_Layer=0;
  No_Layers=0 ;
  FormatX=0;
  FormatY=0;
  b__italics=0;
  b__bold=0;
  orto=0;
  b__Point_View=0;
  w__spec_no=0;
  zn_b1=0;
  zn_b2=0;


  memset (sz_reserve, 0, RES_LEN) ;
  if (read_Xp_Yp==TRUE)
   {
    if (read_c (f, &Xp, 8) != 8) return FALSE ;
    if (read_c (f, &Yp, 8) != 8) return FALSE ;
   }
   else
    {
    if (read_c (f, &nul_Xp, 8) != 8) return FALSE ;
    if (read_c (f, &nul_Yp, 8) != 8) return FALSE ;
    }
  if (read_c (f, &Xmin, 8) != 8) return FALSE ;
  if (read_c (f, &Ymin, 8) != 8) return FALSE ;
  if (read_c (f, &skala, 8) != 8) return FALSE ;
  if (read_c (f, &X_max, 8) != 8) return FALSE ;
  if (read_c (f, &Y_max, 8) != 8) return FALSE ;
  if (read_c (f, &X, 8) != 8) return FALSE ;
  if (read_c (f, &Y, 8) != 8) return FALSE ;
  if (read_c (f, &krok_s, 8) != 8) return FALSE ;
  if (read_c (f, &krok_g, 8) != 8) return FALSE ;
  if (read_i (f, &grid_, 2) != 2) return FALSE ;
  if (read_i (f, &snap_, 2) != 2) return FALSE ;
  if (read_i (f, &Current_Layer, 2) != 2) return FALSE ;
  if (read_i (f, &No_Layers, 2) != 2) return FALSE ;
  i_layersno = (No_Layers <= 16) ? MAX_OLD_NUMBER_OF_LAYERS : No_Layers ;

  if (i_layersno>0)
  {  
   for (i=0;i<i_layersno;i++)
   {
	if (read (f, &Layers1_1, 3) != 3) return FALSE ;
	if (read (f, &Layers_name_1_1, 20) != 20) return FALSE ;
    Layers[i].on=Layers1_1.on;
    Layers[i].edit=Layers1_1.edit;
    Layers[i].point=Layers1_1.point;
    Layers[i].color=Layers1_1.color;
    Layers[i].line_type=typ_linii2_0_to_2_1(Layers1_1.line_type);
    Layers[i].grey=Layers1_1.grey;
	Layers[i].bw=Layers1_1.bw;
	Layers[i].reserve=Layers1_1.reserve;
    Layers[i].len_name=Layers1_1.len_name;
    strcpy(Layers[i].name,Layers_name_1_1.name);
   }  
  } 

  if (read_i (f, &FormatX, 2) != 2) return FALSE ;
  if (read_i (f, &FormatY, 2) != 2) return FALSE ;
  if (read_c (f, &SkalaF,8) != 8) return FALSE ;
  if (read_c (f, &DokladnoscF, 8) != 8) return FALSE ;
  if (read_c (f, &Jednostki, 8) != 8) return FALSE ;
  if (read_c (f, &df__text_height, 8) != 8) return FALSE ;
  if (read_c (f, &df__text_width_factor, 8) != 8) return FALSE ;
  if (read_i (f, &b__italics, 2) != 2) return FALSE ;
    b__bold = (b__italics & 2);
    b__italics &= 1;
	if (read_c (f, &zmwym1_1, 36) != 36) return FALSE ;
  
      zmwym.Lkolor=zmwym1_1.Lkolor;;
      zmwym.Tkolor=zmwym1_1.Tkolor;
      zmwym.wysokosc=zmwym1_1.wysokosc; 
      zmwym.width_factor=zmwym1_1.width_factor ; 
      zmwym.dokladnosc=zmwym1_1.dokladnosc;
      zmwym.linia_ob=zmwym1_1.linia_ob;
      zmwym.strzalka=zmwym1_1.strzalka; 
      zmwym.kierunek_t=zmwym1_1.kierunek_t;  
      zmwym.b_add_line_const=zmwym1_1.b_add_line_const;  
      zmwym.italics=zmwym1_1.italics;   
      zmwym.bold=0;
      zmwym.czcionka=zmwym1_1.czcionka;
      zmwym.collinear=0;
      zmwym.format=0;
  
  if (read_i (f, &orto, 2) != 2) return FALSE ;
  if (read_c (f, &df__Point_Size, 8) != 8) return FALSE ;
  if (read_i (f, &b__Point_View, 2) != 2) return FALSE ;
  if (read_c (f, &df__Trace_Width, 8) != 8) return FALSE ;
  if (read_c (f, &df__Sketch_Dist_Max, 8) != 8) return FALSE ;
  if (read_i (f, &w__spec_no, 2) != 2) return FALSE ;
  if (read_c (f, &l__dane_size, 4) != 4) return FALSE ;
  skala_p_x_f=(float)get_skala_profilu_x();
  rpp_f=(float)get_poziom_pp();

  if (read_c (f, &skala_p_x_f, 4) != 4) return FALSE ;
  if (read_c (f, &rpp_f, 4) != 4) return FALSE ;
  l_kr=put_skala_profilu_x(skala_p_x_f);
  l_kr=put_poziom_pp(rpp_f);

  if (read_i (f, &zn_b1, 2) != 2) return FALSE ;
  if (read_i (f, &zn_b2, 2) != 2) return FALSE ;

  local_x=0;
  local_y=0;
  angle_l=0;
  l_kr=put_angle_l(angle_l);


  if ((zn_b1==1) && ((zn_b2==2) || (zn_b2==3)))
  {
  /* nowa wersja */
  if (read_c (f, &local_x, 8) != 8) return FALSE ;
  if (read_c (f, &local_y, 8) != 8) return FALSE ;

  l_kr=put_znacznik_aplikacji(0);


  iin=/*25*/ 26;
  for (i=0; i<iin; i++)
   {
    if (read_c (f, &null_var[i], 8) != 8) return FALSE ;
   }

  if (zn_b2==3)
   {
   if (read_c (f, &angle_l, 8) != 8) return FALSE ;    /* odczyt kata obrotu ukladu lokalnego */
   l_kr=put_angle_l(angle_l);
   }
    else
     {
     if (read_c (f, &rezerwa, 8) != 8) return FALSE ;
     }

  if (read_c (f, &rezerwa, 8) != 8) return FALSE ;
  if (read_c (f, &rezerwa, 8) != 8) return FALSE ;



  if (read_c (f, &dl_z_b_2, 8) != 8) return FALSE ;
  if (dl_z_b_2>=1)
   {
   if (read_c (f, &zbior_danych_2, dl_z_b_2+1) != (dl_z_b_2+1)) return FALSE ;
   }
  }

  l_kr=put_localx(local_x);
  l_kr=put_localy(local_y);

  add_zbior_2=1;

  return TRUE ;
}



static BOOL write_param (int f, int *error_code1)
/*----------------------------------------------------------------------*/
{
#define RES_LEN 4 //12
  const char PROFIL_TEMP_FILE [] = "ALFPROF.TMP" ;
  int i_f_handle ;
  char sz_reserve [RES_LEN] ;
  int i_layersno, i ;
  float skala_p_x_f;
  float rpp_f;
  double typ_profilu0;
  int zn_b1, zn_b2, iin, l_kr ;
  double dl_z_b_2;
  double zn_b1d ;
  int null_var_int;
  float null_var_float;
  int  pattern_library_no_var_int;
  char null_var_char;
  double local_x, local_y;
  double angle_l;
  MyDane dane_profs;
  MyDane dane_profs0={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 /*,0*/};
  uLong sourceLen;
  unsigned int l__dane_size_uint;
  PRIV_PRN_WINDOW *prn_window;
  
  Bytef *layersBuf;
  uLongf destLen;
  int destLenInt=0;
  int ret;

  memset (sz_reserve, 0, RES_LEN) ;
  df__text_czcionka = TextG.czcionka;
  df__text_type = TextG.typ;
  df__text_justowanie = TextG.justowanie;
  df__text_ukryty = TextG.ukryty;
  if (write (f, &Xp, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &Yp, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &Xmin, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &Ymin, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &skala, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &X_max, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &Y_max, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &X, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &Y, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &krok_s, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &krok_g, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &grid_, sizeof(int)) != sizeof(int)) return FALSE ;
  if (write (f, &snap_, sizeof(int)) != sizeof(int)) return FALSE ;
  if (write (f, &Current_Layer, sizeof(int)) != sizeof(int)) return FALSE ;

  if (write (f, &No_Layers, sizeof(int)) != sizeof(int)) return FALSE ;

  layersBuf = (Bytef *)malloc(sizeof(LAYER)*MAX_NUMBER_OF_LAYERS + 1024);

  i_layersno = (No_Layers <= 16) ? MAX_OLD_NUMBER_OF_LAYERS : No_Layers ;
  sourceLen = sizeof(LAYER)*i_layersno;

  destLen = compressBound(sourceLen);
  
  ret = compress(layersBuf, &destLen, (const Bytef*)&Layers, sourceLen);
  
  if (ret != Z_OK) {*error_code1 = 50;  free(layersBuf); return FALSE;}
  destLenInt = (int)destLen;

  if (destLenInt==0) { *error_code1 = 501; free(layersBuf); return FALSE; }

 
  if (write (f, &destLenInt, sizeof(int)) != sizeof(int)) { *error_code1 = 51; free(layersBuf); return FALSE; }
  if (write (f, layersBuf, destLenInt) != destLenInt) { *error_code1 = 52; free(layersBuf); return FALSE; }
  free(layersBuf);
  if (write (f, &FormatX, sizeof(int)) != sizeof(int)) return FALSE ;
  if (write (f, &FormatY, sizeof(int)) != sizeof(int)) return FALSE ;
  if (write (f, &SkalaF,sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &DokladnoscF, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &Jednostki, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &df__text_height, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &df__text_width_factor, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &b__italics, sizeof(int)) != sizeof(int)) return FALSE ;
  if (write (f, &zmwym, sizeof(zmwym)) != sizeof(zmwym)) return FALSE ;
  if (write (f, &orto, sizeof(int)) != sizeof(int)) return FALSE ;
  if (write (f, &df__Point_Size, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &b__Point_View, sizeof(int)) != sizeof(int)) return FALSE ;
  if (write (f, &df__Trace_Width, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &df__Sketch_Dist_Max, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &w__spec_no, sizeof(int)) != sizeof(int)) return FALSE ;
#ifndef LINUX
  if (write (f, &l__dane_size, sizeof(long)) != sizeof(long)) return FALSE ;
#else
    l__dane_size_uint = (unsigned int)l__dane_size;
    if (write (f, &l__dane_size_uint, sizeof(int)) != sizeof(int)) return FALSE ;
#endif
  skala_p_x_f=(float)get_skala_profilu_x();
  rpp_f=(float)get_poziom_pp();

  if (write (f, &skala_p_x_f, sizeof(float)) != sizeof(float)) return FALSE ;
  if (write (f, &rpp_f, sizeof(float)) != sizeof(float)) return FALSE ;


  /* w nowej wersji mozna dolozyc dodatkowy blok parametrow, np.
  32 liczby 8 -bajtowe. W tym celu nalezy zapisac e sz_reserve tzw. znacznik
  bloku danych np. zn_b1=1; zn_b2=2 ; jezeli jest zn_b2=3 to dodatkowo angle_l*/
  zn_b1=1;
  zn_b2=3;

  if (write (f, &zn_b1, sizeof(int)) != sizeof(int)) return FALSE ;
  if (write (f, &zn_b2, sizeof(int)) != sizeof(int)) return FALSE ;

  null_var_int = 0;  
  null_var_char = 0;

  local_x=get_localx();
  local_y=get_localy();
  angle_l=get_angle_l();

  if (write (f, &local_x, sizeof(double)) != sizeof(double)) return FALSE ;
  if (write (f, &local_y, sizeof(double)) != sizeof(double)) return FALSE ;

  if (write(f, &TRANSLUCENCY, sizeof(int)) != sizeof(int)) return FALSE;

  //saving the number of embeded patterns
  pattern_library_no_var_int = 1;

  if (write(f, &pattern_library_no_var_int, sizeof(int)) != sizeof(int)) return FALSE;


  prn_window = get_priv_prn_window();

  if (write(f, &prn_window->xp, sizeof(double)) != sizeof(double)) return FALSE;
  if (write(f, &prn_window->yp, sizeof(double)) != sizeof(double)) return FALSE;
  if (write(f, &prn_window->xk, sizeof(double)) != sizeof(double)) return FALSE;
  if (write(f, &prn_window->yk, sizeof(double)) != sizeof(double)) return FALSE;

    float magnitude;

    int marker=1234567;

    if (write(f, &marker, sizeof(int)) != sizeof(int)) return FALSE;

    null_var_float=(float)q_magnitude;
    if (write(f, &null_var_float, sizeof(float)) != sizeof(float)) return FALSE;

    if (write(f, &static_stress_colors, sizeof(STATIC_STRESS_COLORS)) != sizeof(STATIC_STRESS_COLORS)) return FALSE;
    magnitude=(float)s_magnitude;
    if (write(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
    if (write(f, &stress_precision, sizeof(double)) != sizeof(double)) return FALSE;

    if (write(f, &static_colors, sizeof(STATIC_COLORS)) != sizeof(STATIC_COLORS)) return FALSE;

    magnitude=(float)n_magnitude;
    if (write(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
    magnitude=(float)v_magnitude;
    if (write(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
    magnitude=(float)m_magnitude;
    if (write(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
    magnitude=(float)d_magnitude;
    if (write(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
    magnitude=(float)r_magnitude;
    if (write(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;
    magnitude=(float)rm_magnitude;
    if (write(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;

    if (write(f, &thermal_precision, sizeof(double)) != sizeof(double)) return FALSE;
    if (write(f, &load_precision, sizeof(double)) != sizeof(double)) return FALSE;
    if (write(f, &force_precision, sizeof(double)) != sizeof(double)) return FALSE;
    if (write(f, &moment_precision, sizeof(double)) != sizeof(double)) return FALSE;
    if (write(f, &displacement_precision, sizeof(double)) != sizeof(double)) return FALSE;
    if (write(f, &rotation_precision, sizeof(double)) != sizeof(double)) return FALSE;

  if (write(f, &thermal_magnitude, sizeof(double)) != sizeof(double)) return FALSE;
  if (write(f, &load_magnitude, sizeof(double)) != sizeof(double)) return FALSE;
  if (write(f, &force_magnitude, sizeof(double)) != sizeof(double)) return FALSE;
  if (write(f, &moment_magnitude, sizeof(double)) != sizeof(double)) return FALSE;
  if (write(f, &displacement_magnitude, sizeof(double)) != sizeof(double)) return FALSE;
  if (write(f, &rotation_magnitude, sizeof(double)) != sizeof(double)) return FALSE;

  if (write(f, &zmwym_fraction, sizeof(int)) != sizeof(int)) return FALSE;
  // if (write(f, &zmwym_reserve, sizeof(int)) != sizeof(int)) return FALSE;  //ignored
    magnitude=(float)p_magnitude;
    if (write(f, &magnitude, sizeof(float)) != sizeof(float)) return FALSE;

  if (write (f, &sektory_arkusza_ext, sizeof(SEKTORY_EXT)) != sizeof(SEKTORY_EXT)) return FALSE ;
  if (write (f, &sektory_arkusza, sizeof(SEKTORY)) != sizeof(SEKTORY)) return FALSE ;

  if (write (f, &angle_l, sizeof(double)) != sizeof(double)) return FALSE ;

  if (write (f, &KursorS, sizeof(int)) != sizeof(int)) return FALSE ;
  if (write (f, &df__text_czcionka, sizeof(char)) != sizeof(char)) return FALSE ;
  if (write (f, &df__text_type, sizeof(char)) != sizeof(char)) return FALSE ;
  if (write (f, &df__text_justowanie, sizeof(char)) != sizeof(char)) return FALSE ;
  if (write (f, &df__text_ukryty, sizeof(char)) != sizeof(char)) return FALSE ;
  
  if (ciagniecie_linii==TRUE) null_var_char=1; else null_var_char=0;
  if (write (f, &null_var_char, sizeof(char)) != sizeof(char)) return FALSE ; 
  
  if (rozcinanie_linii==TRUE) null_var_char=1; else null_var_char=0;
  if (write (f, &null_var_char, sizeof(char)) != sizeof(char)) return FALSE ;
  
  if (ukrywanie_typu==TRUE) null_var_char=1; else null_var_char=0;
  if (write (f, &null_var_char, sizeof(char)) != sizeof(char)) return FALSE ;
  
  if (type_view==TRUE) null_var_char=1; else null_var_char=0;
  if (write (f, &null_var_char, sizeof(char)) != sizeof(char)) return FALSE ;

  /*swobodny blok*/
  if (swobodny_blok==TRUE) null_var_char=1; else null_var_char=0;
  if (write (f, &null_var_char, sizeof(char)) != sizeof(char)) return FALSE ;
  /*swobodny tekst*/
  if (swobodny_tekst==TRUE) null_var_char=1; else null_var_char=0;
  if (write (f, &null_var_char, sizeof(char)) != sizeof(char)) return FALSE ;
  /*tryb lokalizacji*/
  if (sel.gor==1) null_var_char=sel.nr+1; else null_var_char=0;
  if (write (f, &null_var_char, sizeof(char)) != sizeof(char)) return FALSE ;
  /*rezerwa*/
  options1.normalize_text=normalize_text;
  if (write (f, &options1, sizeof(char)) != sizeof(char)) return FALSE ;

  dl_z_b_2=strlen(zbior_danych_2);
  if (write (f, &dl_z_b_2, sizeof(double)) != sizeof(double)) return FALSE ;
  if (dl_z_b_2>=1)
   {
   if (write (f, &zbior_danych_2, dl_z_b_2+1) != (dl_z_b_2+1)) return FALSE ;
   }

  return 0 ;
}

BOOL ReadPatterns(int f, int *numer_bledu)
{
int pattern_library_no;
int i;
char* pattern_buffer;
SOLID_PATTERN_LIBRARY_ITEM solid_pattern_library_item;
int ret;
int a;
//READING EMBEDED PATTERNS EXTENSION
if (read(f, &pattern_library_no, sizeof(int)) != sizeof(int)) { *numer_bledu = 213; return FALSE; }
for (i = 0; i < pattern_library_no; i++)
	{
		if (read(f, &solid_pattern_library_item, sizeof(SOLID_PATTERN_LIBRARY_ITEM)) != sizeof(SOLID_PATTERN_LIBRARY_ITEM)) { *numer_bledu = 214;  return FALSE; }
		pattern_buffer = malloc(solid_pattern_library_item.size);
		if (pattern_buffer==NULL) { *numer_bledu = 215;  return FALSE; }
		if (read(f, pattern_buffer, solid_pattern_library_item.size) != solid_pattern_library_item.size) { *numer_bledu = 216; return FALSE;}
		solid_pattern_library_item.pattern_buffer = pattern_buffer;

		ret = LoadPattern(&solid_pattern_library_item);

		free(pattern_buffer);
		
	}
  return TRUE;
}

int czytaj_zbior (char *argv, BOOL err_message, BOOL b_current_ver, BOOL read_Xp_Yp)
/*--------------------------------------------------------------------------------*/
{
  char blok_naglowka [VER_LEN + 1] ;
  int alfa_version;
  int f;
  int i_ret ;
  char fn [MAXPATH] ;
  BOOL read1_Xp_Yp ;
  char st[80];
  int numer_bledu;
  int background_log;
  unsigned char background_color;
  int opcja_warstw;
  int error_code=0;
  char df__zones_font;
  char df__dim_font;
  int pattern_library_no_var, pattern_library_no;
  int i;

    long_long del_dane0;
    char *dane00;

    dane0 = dane,
    dane00 = dane0;

#ifdef LINUX
#define my_open open
#define my_close close
#else
#define my_open _open
#define my_close _close
#endif

  reset_n_list_begin_blk();

  zmwym_fraction=64;
  zmwym_reserve=0;
  Error=0;
  numer_bledu = 0;
  read1_Xp_Yp=read_Xp_Yp;
  strcpy (fn, argv) ;
  File_Ext (fn, EXT__FILE);
  f = my_open (fn, O_RDONLY|O_BINARY, NULL) ;
  if (f != -1)
  {
	  if (read(f, blok_naglowka, VER_LEN) != VER_LEN) {error_code = 0; goto error;}
    blok_naglowka [VER_LEN] = '\0' ;   /*pisz_zbior nie konczy blok_naglowka NULL'em*/

    char *endptr;
    float num;
    num = strtof(&blok_naglowka[7], &endptr);
    alfa_version=FLOAT_TO_INT(num*10);
    
    /* wersja 1.0 */
    if (alfa_version == ver1_0)
      {
      my_close(f);

	  strcpy(st, _DRAWING_V_1_);

      ErrListStr(st);
      delay(200);
      return IO_RET_OPEN_FILE_ERR;
    }
    else if (alfa_version == ver1_1) //if (strcmp (blok_naglowka, VER1_1) == 0)
    {
      komunikat0(106);
	  if (FALSE == read_write_param1_1(f, (int (*)(int, void *, unsigned int)) read, read1_Xp_Yp)) { error_code = 1; goto error;}
      zwolnienie_pamieci_1();
	  if (!SetBufferDaneSize(l__dane_size)) { error_code = 100; goto error18;}

        del_dane0=dane0-dane00;
        if (del_dane0!=0)
        {
            ;
        }

        dane_size = l__dane_size ;
      dane   -=   dane_size - sizeof(NAGLOWEK1_1) ;    /*bardzo nie eleganckie, poprawic!!!!*/
      if (czytaj (f, dane, dane_size) == -1) { error_code = 2; goto error; }
      if (FALSE == Read_Spec1_1 (f, Get_Local_Spec (), w__spec_no)) 
         {
			 {error_code = 3; goto error; }
         } 
      if (-1 == my_close (f)) { error_code = 4; goto error; }
      if (FALSE == ver1_1_to_2_0 (0, dane_size - 1))
       {
		   {error_code = 5; goto error; } /*!!! to nalezy poprawic !!!*/
       }
      i_ret=IO_RET_READ_AGAIN;
    }

    else if (alfa_version == ver2_0) //if (strcmp (blok_naglowka, VER2_0) == 0)
    {
      if (FALSE == read_write_param (f, (int (*)(int, void *, unsigned int)) read, read1_Xp_Yp, 0, 0, &pattern_library_no_var)) { error_code = 6; goto error; }
      if (!SetBufferDaneSize (l__dane_size)) { error_code = 101; goto error18; }

        del_dane0=dane0-dane00;
        if (del_dane0!=0)
        {
            ;
        }

      dane_size = l__dane_size ;
      dane   -=   dane_size - sizeof(NAGLOWEK) ;    /*bardzo nie eleganckie, poprawic!!!!*/
      if (czytaj (f, dane, dane_size) == -1) { error_code = 7; goto error; }
      if (FALSE == Read_Spec (f, Get_Local_Spec (), w__spec_no)) { error_code = 8; goto error; }
      if (-1 == my_close (f)) { error_code = 9; goto error; }
	  /*ZMIANA STRUKTURY TEKSTOW*/

      if (FALSE == ver2_1_to_3_0 (0, dane_size - 1)) { error_code = 10; goto error; }
      
      /*ZMIANA TYPU I GRUBOSCI*/
      if (FALSE == ver2_0_to_2_1 (0, dane_size - 1)) { error_code = 11; goto error; }

	  SetDefaultStyles();

      i_ret=IO_RET_OK;
    }
    else if (alfa_version == ver2_1)  //if (strcmp (blok_naglowka, VER2_1) == 0)
    {
      if (FALSE == read_write_param (f, (int (*)(int, void *, unsigned int)) read, read1_Xp_Yp, 1, &numer_bledu, &pattern_library_no_var)) { error_code = numer_bledu; goto error; }
      if (!SetBufferDaneSize (l__dane_size)) { error_code = 102; goto error18; }

        del_dane0=dane0-dane00;
        if (del_dane0!=0)
        {
            ;
        }

      dane_size = l__dane_size ;
      dane   -=   dane_size - sizeof(NAGLOWEK) ;    /*bardzo nie eleganckie, poprawic!!!!*/
      if (czytaj (f, dane, dane_size) == -1) { error_code = 14; goto error; }
      if (FALSE == Read_Spec (f, Get_Local_Spec (), w__spec_no)) { error_code = 15; goto error; }
      //proba odczytu koloru tla
      background_log=0;
      kolory.paper=Kolory_Paper;
      if (read(f,&background_log,sizeof(int))==sizeof(int))
       {
         if (background_log==54321)
          {
           if (read(f,&background_color,1)==1)
            {
             kolory.paper=background_color;
            }
          }
       }
      if (-1 == my_close (f)) { error_code = 16; goto error; }
	  /*ZMIANA STRUKTURY TEKSTOW*/
	  
      if (FALSE == ver2_1_to_3_0 (0, dane_size - 1)) { error_code = 17; goto error; }

	  SetDefaultStyles();

      i_ret=IO_RET_OK;
    }
    else if (alfa_version >= ver3_0)
    {
      if (alfa_version >= ver4_1)
          opcja_warstw = 2;
	  else opcja_warstw = 1;

      if (FALSE == read_write_param (f, (int (*)(int, void *, unsigned int)) read, read1_Xp_Yp, opcja_warstw, &numer_bledu, &pattern_library_no_var)) { error_code = numer_bledu; goto error; }

      if (alfa_version < ver4_1) lseek(f, -16, SEEK_CUR);

      if (!SetBufferDaneSize (l__dane_size)) { error_code = 19; goto error; }

        del_dane0=dane0-dane00;
        if (del_dane0!=0)
        {
            ;
        }

      dane_size = l__dane_size ;
      dane   -=   dane_size - sizeof(NAGLOWEK) ;    /*bardzo nie eleganckie, poprawic!!!!*/
      if (czytaj (f, dane, dane_size) == -1) { error_code = 20; goto error; }

      if (FALSE == Read_Spec (f, Get_Local_Spec (), w__spec_no)) { error_code = 21; goto error; }
      //proba odczytu koloru tla
      background_log=0;
      kolory.paper=Kolory_Paper;
      if (read(f,&background_log,sizeof(int))==sizeof(int))
       {
         if (background_log==54321)
          {
           if (read(f,&background_color,1)==1)
            {
             kolory.paper=background_color;
            }
          }
       }
	  if (alfa_version == ver4_0)
	  {
		  if (FALSE == ReadStyles40(f,0)) SetDefaultStyles();
	  }
	  else if (alfa_version >= ver4_1)
	  {
		  if (FALSE == ReadStyles(f,0)) SetDefaultStyles();
	  }
	  else SetDefaultStyles();

	  if (alfa_version >= ver4_1)
	  {
		  if (pattern_library_no_var > 0)
		  {
			  if (FALSE == ReadPatterns(f, &numer_bledu)) { error_code = numer_bledu; goto error; }
		  }
	  }

      if (-1 == my_close (f)) { error_code = 22; goto error; }

      i_ret=IO_RET_OK;

    }
    else 
    { 
      /* nieznany naglowek */
		{ error_code = 13; goto error; }
    }  

	if (alfa_version < ver4_1) //(strcmp(blok_naglowka, VER4_1) != 0)
	{
		/*CHANGING TEXTS TO UTF8*/

		if (FALSE == ver4_0_to_4_1(0, dane_size - 1, NULL)) { error_code = 23; goto error; }

		ver4_0_to_4_1_Layer();
		ver4_0_to_4_1_Table();
	}
    else if (alfa_version < ver4_2)
    {
        if (FALSE == ver4_1_to_4_2(0, dane_size - 1)) { error_code = 23; goto error; }
    }

	//changinh text styles indexes, has to be done always
	if (FALSE == change_textstyle_indexes(0, dane_size - 1))
	{
		;
	}

	change_textstyle_global(&df__text_czcionka);
	df__zones_font = sektory_arkusza_ext.font;
	change_textstyle_global(&df__zones_font);
	sektory_arkusza_ext.font = df__zones_font;
	df__dim_font = zmwym.czcionka;
	change_textstyle_global(&df__dim_font);
	zmwym.czcionka = df__dim_font;
	
/*kontrola wartosci parametrow*/
    if (b__Point_View != TRUE && b__Point_View != FALSE) { error_code = 24; goto error; }

    if (TRUE == Check_if_LE (skala, 0) ||
	TRUE == Check_if_LE (df__Point_Size, 0) ||
	TRUE == Check_if_LE (df__Trace_Width, 0) ||
	TRUE == Check_if_LE (df__Sketch_Dist_Max, 0)) {error_code = 25; goto error;}
    Xmax=X_max+Xmin;
    Ymax=Y_max+Ymin;
    if (TRUE == Check_if_LE (krok_s, 0)) krok_s=1;
    if (TRUE == Check_if_LE (krok_g, 0)) krok_g=1;
    if(TRUE == Check_if_LE (krok_s, 0) ||
       TRUE == Check_if_LE (krok_g, 0) ||
       TRUE == Check_if_LE (X_max, 0) ||
       TRUE == Check_if_LE (Y_max, 0)) {error_code = 26; goto error;}
	if (FormatX<=0) { error_code = 27; goto error; }
	if (FormatY<=0) { error_code = 28; goto error; }
	if (TRUE == Check_if_LE (SkalaF, 0)) { error_code = 29; goto error; }
	if (TRUE == Check_if_LE (DokladnoscF, 0)) { error_code = 30; goto error; }
	if (TRUE == Check_if_LE (Jednostki, 0)) { error_code = 31; goto error; }

    set_st_jedn();

	if (TRUE == Check_if_LE (df__text_height, 0)) { error_code = 32; goto error; }
	if (TRUE == Check_if_LE (df__text_width_factor, 0)) { error_code = 33; goto error; }
		//sprawdz zmwym !!

    TextG.wysokosc = df__text_height;
    TextG.width_factor =  df__text_width_factor ;
    TextG.italics = b__italics ;
    TextG.bold = b__bold ;
    TextG.czcionka = df__text_czcionka;
    TextG.typ = df__text_type;
    TextG.justowanie = df__text_justowanie;
    TextG.ukryty = df__text_ukryty;
    if (No_Layers == 0)
    {
      No_Layers = 1;
      Current_Layer=0;
      Layers [0].on = 1;
      Layers [0].edit = 1;
      Layers [0].point = 1;
      Layers [0].color = 7;  /*bialy*/
      Layers [0].line_type = 64;
      Layers [0].len_name = 1;
      strcpy (Layers [0].name, "0");
    }
    
    Layers [255].on = 0;
    Layers [255].edit = 1;
    Layers [255].point = 1;
    Layers [255].color = 7;  /*bialy*/
    Layers [255].line_type = 64;
    Layers [255].len_name = 0;
    strcpy (Layers [255].name, "");

    Layers [254].on = 1;

    Layers [254].edit = 1;

    Layers [254].point = 1;
    Layers [254].color = 7;  /*bialy*/
    Layers [254].line_type = 64;
    Layers [254].len_name = 0;
    strcpy (Layers [254].name, "");

    zmien_atrybut(dane, dane + dane_size, Ablok, Anormalny);

    licznik_redraw=0;

	last_active_layer=-1;
    last_active_layer_arch=-1;

    if ((alfa_version < ver3_1) || (auto_korekta_blokow==TRUE)) //dla starszych wersji lub wymuszone
	{
      ////korekta_obiektow_blokow();  //64bit
      if (killblock==1) zlikwiduj_wszystkie_bloki();
	}

    return i_ret;
  }
  else
  {
    zwolnienie_pamieci();
    return IO_RET_OPEN_FILE_ERR ;
  }
  error:
  {
    zwolnienie_pamieci();
    if (err_message == TRUE)
    {
      ErrListCode(45, error_code);
    }
    my_close(f);
    delay(3000);
    return IO_RET_RW_ERR ;
  }
  error18:
  {
    my_close(f);
    if (err_message == TRUE)
    {
      ErrListCode(18, error_code);
    }
    delay(3000);
    return IO_RET_MEM_ERR ;
  }
}

int pisz_zbior(char *argv, BOOL b_erase, int every)
/*------------------------------------------------*/
{
	T_Prot_Date * ptrs_prot;
	char blok_naglowka[VER_LEN + 1] = VER4_1; //VER4_1; //VER4_0; // VER3_1 /*VER2_0 ;*/ /*VER2_1*/;  //VER3_1 uznawana jest za wiarygodna, i pomijana jest standardowo kontrola blokow
	int f;
	char fn[MAXPATH];
	BOOL b_close = FALSE;
	int background_log;
	int error_code = 200;
	int error_code1 = 0;
	unsigned char charfont_numbers[MaxNumberTextStyle];
	unsigned char pattern_numbers[SOLID_PATTERN_LIBRARY_MAX_ITEMS];
	int ret;
	int i;
	int pattern_library_no;
	SOLID_PATTERN_LIBRARY_ITEM* solid_pattern_library_item;

	if (every == 0)
	{

		if (Check_Prot() == FALSE)
		{
			not_active();
			return IO_RET_PROT_ERR;
		}
	}

		if (dane_size + 1000 > Get_Disk_Free_Space(&argv0))      /*1000 +- na zmienne globalne*/
		{
			{error_code = 201;  goto error; }
		}

		strcpy(fn, argv);
		file_bak(fn, EXT__FILE);
		File_Ext(fn, EXT__FILE);

#ifndef LINUX
    f=open(fn, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
    f=open(fn, O_CREAT|O_TRUNC|O_RDWR, 0666); //S_IRWXU | S_IRWXG | S_IRWXO);
#endif
		if (f != -1)
		{
			b_close = TRUE;
			if (write(f, blok_naglowka, VER_LEN) != VER_LEN) { error_code = 202;  goto error; }
			df__text_height = TextG.wysokosc;
			df__text_width_factor = TextG.width_factor;
			b__italics = TextG.italics | (TextG.bold * 2);
			l__dane_size = dane_size;
			w__spec_no = Max_No_Spec;

			if (write_param(f, &error_code1)) { error_code = 203 + error_code1;  goto error; }

			if (b_erase == TRUE)
			{
				char  *adh;
				adh = dane;
				adh += dane_size;
				usun_blok(dane, adh);
			}

			if (pisz(f, (char  *)(dane), dane_size) == -1) { error_code = 204;  goto error; }
			if (FALSE == Save_Spec(f, Get_Local_Spec())) { error_code = 205;  goto error; }

			background_log = 54321;
			if (write(f, &background_log, sizeof(int)) != sizeof(int)) { error_code = 206;  goto error; }
			if (write(f, &kolory.paper, 1) != 1) { error_code = 207;  goto error; }

			if (SAVE_ALL_FONTS)
			{
				if (FALSE == SaveStyles(f)) { error_code = 208;  goto error; }
			}
			else
			{
				ret = get_fonts_for_dxf(&charfont_numbers);
				//adding the current font
				charfont_numbers[TextG.czcionka] = 1;
				//adding dimensioning font
				charfont_numbers[zmwym.czcionka] = 1;
				//adding zones font
				charfont_numbers[sektory_arkusza_ext.font] = 1;
				if (FALSE == SaveBlockStyles(f, charfont_numbers)) { error_code = 209;  goto error; }
			}

			//EMBEDED PATTERNS EXTENSION
			pattern_library_no = get_patterns_for_solids(&pattern_numbers);
			if (write(f, &pattern_library_no, sizeof(int)) != sizeof(int)) { error_code = 210;  goto error; }
			for (i = 0; i < solid_pattern_library_no; i++)
			{
				if (pattern_numbers[i] == 1)
				{
					solid_pattern_library_item = (SOLID_PATTERN_LIBRARY_ITEM*)(Solid_Pattern_Library[i]);
					if (write(f, solid_pattern_library_item, sizeof(SOLID_PATTERN_LIBRARY_ITEM)) != sizeof(SOLID_PATTERN_LIBRARY_ITEM)) { error_code = 211;  goto error; }
					if (write(f, solid_pattern_library_item->pattern_buffer, solid_pattern_library_item->size) != solid_pattern_library_item->size) { error_code = 212;  goto error; }
				}
			}

			if (-1 == close(f)) { error_code = 212;  goto error; }
			return IO_RET_OK;
		}
	error:
		{
			ErrListCode(8, error_code);
			if (b_close == TRUE)
			{
				close(f);
			}
			return IO_RET_RW_ERR;
		}
}
 

/*-----------------------------------------------*/
/*
na dysku elementy bloku w naglowku nie maja zapalone flagi "blok",
nalezy to zrobic przy imporcie,
tylko piersze 64k elementow bedzie w obrebie bloku, pozostale beda "wolne",
wszystkie zapisane elementy maja atrybut Ablok,
*/

int get_fonts_for_dxf(unsigned char *charfont_numbers)
{
	int i;
	char *adh;
	char *ad_b, *ad_e;
	NAGLOWEK *nag;
	TEXT *adt;
	BLOK  *b;
	unsigned char font_index;
	long_long l;

	for (i = 0; i < MaxNumberTextStyle; i++) charfont_numbers[i] = 0;

	adh = dane;
	adh += dane_size;

	obiekt_tok(dane, adh, (char **)&nag, ONieOkreslony);
	while (nag != NULL)
	{
		switch (nag->obiekt)
		{
		case OdBLOK:
			break;
		case Otekst:
			adt = nag;
			font_index = adt->czcionka;
			charfont_numbers[font_index] = 1;
			break;
		default:
			break;
		}
		obiekt_tok(NULL, adh, (char **)&nag, ONieOkreslony);
	}
	return 1;
}

int get_patterns_for_solids(unsigned char* pattern_numbers)
{
	int i;
	char* adh;
	char* ad_b, * ad_e;
	NAGLOWEK* nag;
	WIELOKAT* w;
    SOLIDARC *sa;
	BLOK* b;
	unsigned char font_index;
	int patterns_no = 0;
	long_long l;
	char* scale_ptr;
	char* dx_ptr;
	char* angle_ptr;
	char* dy_ptr;
	char* name_ptr;
	SOLID_PATTERN_LIBRARY_ITEM* solid_pattern_library_item;

	for (i = 0; i < SOLID_PATTERN_LIBRARY_MAX_ITEMS; i++) pattern_numbers[i] = 0;

	adh = dane;
	adh += dane_size;

	obiekt_tok(dane, adh, (char**)&nag, ONieOkreslony);
	while (nag != NULL)
	{
		switch (nag->obiekt)
		{
		case OdBLOK:
			break;
		case Owwielokat:
			w = (WIELOKAT *)nag;
			if (w->pattern == 1)
			{   
				scale_ptr = w->xy;
				scale_ptr += (w->lp * sizeof(float));
				dx_ptr = scale_ptr;
				dx_ptr += sizeof(short int);
				angle_ptr = dx_ptr;
				angle_ptr += sizeof(short int);
				dy_ptr = angle_ptr;
				dy_ptr += sizeof(short int);
				name_ptr = dy_ptr;
				name_ptr += sizeof(short int);
				
				for (i = 0; i < solid_pattern_library_no; i++)
				{
					solid_pattern_library_item = (SOLID_PATTERN_LIBRARY_ITEM*)(Solid_Pattern_Library[i]);
					if (strcmp(name_ptr, solid_pattern_library_item->pattern_name) == NULL)
					{
						if (pattern_numbers[i] == 0)
						{
							pattern_numbers[i] = 1;
							patterns_no++;
						}
						break;
					}
				}
			}
			break;
            case Osolidarc:
                sa = (SOLIDARC *)nag;
                if (sa->pattern == 1)
                {

                    name_ptr=sa->patternname;

                    for (i = 0; i < solid_pattern_library_no; i++)
                    {
                        solid_pattern_library_item = (SOLID_PATTERN_LIBRARY_ITEM*)(Solid_Pattern_Library[i]);
                        if (strcmp(name_ptr, solid_pattern_library_item->pattern_name) == NULL)
                        {
                            if (pattern_numbers[i] == 0)
                            {
                                pattern_numbers[i] = 1;
                                patterns_no++;
                            }
                            break;
                        }
                    }
                }
                break;
		default:
			break;
		}
		obiekt_tok(NULL, adh, (char**)&nag, ONieOkreslony);
	}
	return patterns_no;
}


int WriteBlock(char *fn, double Px, double Py, char *buf_name, char *buf_type, int every, int bak)
//-------------------------------------------------------------------------------------------------
{
	char blok_naglowka[VERB_LEN] = VERB4_1; /*VERB4_1*/ /*VERB4_0*/ /*VERB3_0*/ /*VERB2_0*/ /*VERB2_1*/
	int f;
	long_long l, poz_blok, size_blok = B3;
	NAGLOWEK *ad;
	char *ad_b, *ad_e;
	TEXT *adt;
	WIELOKAT* w;
    SOLIDARC *sa;
#ifndef LINUX
	BLOKD blok = BDdef;
#else
    BLOK blok = Bdef;
#endif
	BLOK  *b;
	int len_name, len_type;
	int flags = EBF_IP;
	long_long disk_free_space, l_size_block;
	char fn1[MAXPATH];
	BOOL b_errsize = FALSE;
	float f_xip, f_yip;
	T_Prot_Date * ptrs_prot;
	unsigned char font_index;
	FontStyle fontstyle[MaxNumberTextStyle];
	unsigned char charfont_numbers[MaxNumberTextStyle];
	unsigned char pattern_numbers[SOLID_PATTERN_LIBRARY_MAX_ITEMS];
	BOOL have_texts;
	int i;
	int error_code=300;
	char* scale_ptr;
	char* dx_ptr;
	char* angle_ptr;
	char* dy_ptr;
	char* name_ptr;
	SOLID_PATTERN_LIBRARY_ITEM* solid_pattern_library_item;
	BOOL patterns_exist = FALSE;
    int ex_block_pos;

	for (i = 0; i < MaxNumberTextStyle; i++) charfont_numbers[i] = 0;
	for (i = 0; i < SOLID_PATTERN_LIBRARY_MAX_ITEMS; i++) pattern_numbers[i] = 0;

	if (ADP == NULL || ADK == NULL)  //we let save empty block
	{
		ADP = dane;
		ADK = dane;
	}

	if (every == 0)
	{

		if (Check_Prot() == FALSE)
		{
			not_active();
			return IO_RET_PROT_ERR;
		}

		strcpy(fn1, fn);

		if (bak) file_bak(fn1, EXT__BLOK);

		l_size_block = Get_Size_Block(ADP - dane, ADK - dane);
		if (l_size_block > INT_MAX)
		{
			ErrList(21);
			b_errsize = TRUE;
		}
		disk_free_space = Get_Disk_Free_Space(fn1);
		if (l_size_block > disk_free_space - 1000)
		{
			ErrList(8);
			return IO_RET_RW_ERR;
		}
		File_Ext(fn1, EXT__BLOK);
		f_xip = Px;
		f_yip = Py;
		len_name = strlen(buf_name) + 1;
		len_type = strlen(buf_type) + 1;
		blok.kod_obiektu = 'E';
		blok.dlugosc_opisu_obiektu = sizeof(flags) + 2 * sizeof(f_xip) +
			sizeof(len_type) + len_type;
		size_blok += blok.dlugosc_opisu_obiektu;
		blok.atrybut = Ablok;


#ifndef LINUX
        f=open(fn1, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
        f=open(fn1, O_CREAT|O_TRUNC|O_RDWR, 0666); //S_IRWXU | S_IRWXG | S_IRWXO);
#endif
		if (f != -1)
		{
			error_code = 301;
			if (write(f, blok_naglowka, VERB_LEN) != VERB_LEN) goto error;
			if (write(f, &Px, sizeof(double)) != sizeof(double)) goto error;
			if (write(f, &Py, sizeof(double)) != sizeof(double)) goto error;
			if (write(f, &len_name, sizeof(int)) != sizeof(int)) goto error;
			if (write(f, buf_name, len_name) != len_name) goto error;
			if (b_errsize == FALSE)
			{
				poz_blok = lseek(f, 0, 1);
				error_code = 302;
				if (write(f, &blok, sizeof(blok)) != sizeof(blok)) goto error;
#ifdef LINUX
                ex_block_pos=lseek(f, -4, SEEK_CUR);
#endif
				if (write(f, &flags, sizeof(int)) != sizeof(int))goto error;
				if (write(f, &f_xip, sizeof(f_xip)) != sizeof(f_xip))goto error;
				if (write(f, &f_yip, sizeof(f_yip)) != sizeof(f_yip))goto error;
				if (write(f, &len_type, sizeof(len_type)) != sizeof(len_type))goto error;
				if (write(f, buf_type, len_type) != len_type) goto error;
			}
			obiekt_tok((char*)ADP, ADK, (char **)&ad, ONoInBlock); // ONieOkreslony);
			while (ad != NULL)
			{
				if (ad->atrybut == Ablok)
				{
					if (ad->obiekt == Otekst)
					{
						adt = ad;
						font_index = adt->czcionka;
						charfont_numbers[font_index] = 1;
						have_texts = TRUE;
					}
					else if (((NAGLOWEK*)ad)->obiekt == Owwielokat)
					{
						w = (WIELOKAT*)ad;
						if (w->pattern == 1)
						{
							scale_ptr = w->xy;
							scale_ptr += (w->lp * sizeof(float));
							dx_ptr = scale_ptr;
							dx_ptr += sizeof(short int);
							angle_ptr = dx_ptr;
							angle_ptr += sizeof(short int);
							dy_ptr = angle_ptr;
							dy_ptr += sizeof(short int);
							name_ptr = dy_ptr;
							name_ptr += sizeof(short int);

							for (i = 0; i < solid_pattern_library_no; i++)
							{
								solid_pattern_library_item = (SOLID_PATTERN_LIBRARY_ITEM*)(Solid_Pattern_Library[i]);
								if (strcmp(name_ptr, solid_pattern_library_item->pattern_name) == NULL)
								{
									if (pattern_numbers[i] == 0)
									{
										pattern_numbers[i] = 1;
										patterns_exist=TRUE;
									}
									break;
								}
							}
						}
					}
                    else if (((NAGLOWEK*)ad)->obiekt == Osolidarc)
                    {
                        sa = (SOLIDARC*)ad;
                        if (sa->pattern == 1)
                        {
                            name_ptr=sa->patternname;

                            for (i = 0; i < solid_pattern_library_no; i++)
                            {
                                solid_pattern_library_item = (SOLID_PATTERN_LIBRARY_ITEM*)(Solid_Pattern_Library[i]);
                                if (strcmp(name_ptr, solid_pattern_library_item->pattern_name) == NULL)
                                {
                                    if (pattern_numbers[i] == 0)
                                    {
                                        pattern_numbers[i] = 1;
                                        patterns_exist=TRUE;
                                    }
                                    break;
                                }
                            }
                        }
                    }
					l = sizeof(NAGLOWEK) + ad->n;
					if (ad->obiekt == OdBLOK)
					{
						ad_b = ad;
						b = (BLOK *)ad_b;
						
						ad_e = ad_b + l;
						ad_b += sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
						
						while (ad_b < ad_e)
						{
							if (((NAGLOWEK *)ad_b)->obiekt == OdBLOK)
							{
								b = (BLOK *)ad_b;
								ad_b += (sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu);
							}
							else
							{
								if ((((NAGLOWEK *)ad_b)->obiekt == Otekst) /*|| (((NAGLOWEK *)ad_b)->obiekt == Otekst3D)*/)
								{
									adt = ad_b;
									font_index = adt->czcionka;
									charfont_numbers[font_index] = 1;
									have_texts = TRUE;
								}
                                else if (((NAGLOWEK*)ad_b)->obiekt == Olinia)
                                {
                                    LINIA *L=(NAGLOWEK*)ad_b;
                                    if (L->n + sizeof(NAGLOWEK) !=  sizeof (LINIA) )
                                    {
                                      int a=0;
                                    }
                                }
								else if (((NAGLOWEK*)ad_b)->obiekt == Owwielokat)
								{
									w = (WIELOKAT*)ad_b;
									if (w->pattern == 1)
									{
										scale_ptr = w->xy;
										scale_ptr += (w->lp * sizeof(float));
										dx_ptr = scale_ptr;
										dx_ptr += sizeof(short int);
										angle_ptr = dx_ptr;
										angle_ptr += sizeof(short int);
										dy_ptr = angle_ptr;
										dy_ptr += sizeof(short int);
										name_ptr = dy_ptr;
										name_ptr += sizeof(short int);

										for (i = 0; i < solid_pattern_library_no; i++)
										{
											solid_pattern_library_item = (SOLID_PATTERN_LIBRARY_ITEM*)(Solid_Pattern_Library[i]);
											if (strcmp(name_ptr, solid_pattern_library_item->pattern_name) == NULL)
											{
												pattern_numbers[i] = 1;
												patterns_exist=TRUE;
												break;
											}
										}
									}
								}
								ad_b += sizeof(NAGLOWEK) + ((NAGLOWEK *)ad_b)->n;
							}
						}
					}
					
					if (pisz(f, (char *)(ad), l) == -1) { error_code = 303;  goto error; }
					size_blok += l;
				}
				obiekt_tok(NULL, ADK, (char **)&ad, ONoInBlock);
			}
			if (b_errsize == FALSE)
			{
				
				if (FALSE == SaveBlockStyles(f, charfont_numbers)) { error_code = 304; goto error; }

				//EMBEDED PATTERNS EXTENSION
				if (patterns_exist == TRUE)
				{
					int patterns_block_no = 0;
					for (i = 0; i < solid_pattern_library_no; i++)
					{
						if (pattern_numbers[i] == 1)
							patterns_block_no++;
					}
					if (write(f, &patterns_block_no, sizeof(int)) != sizeof(int)) { error_code = 305;  goto error; }
					for (i = 0; i < solid_pattern_library_no; i++)
					{
						if (pattern_numbers[i] == 1)
						{
							solid_pattern_library_item = (SOLID_PATTERN_LIBRARY_ITEM*)(Solid_Pattern_Library[i]);
							if (write(f, solid_pattern_library_item, sizeof(SOLID_PATTERN_LIBRARY_ITEM)) != sizeof(SOLID_PATTERN_LIBRARY_ITEM)) { error_code = 306;  goto error; }
							if (write(f, solid_pattern_library_item->pattern_buffer, solid_pattern_library_item->size) != solid_pattern_library_item->size) { error_code = 307;  goto error; }
						}
					}
				}

				blok.n = (unsigned long)size_blok;
				lseek(f, poz_blok, SEEK_SET);

                if (write(f, &blok, sizeof(blok)-4) != sizeof(blok)-4) { error_code = 308;  goto error; }

			}
			
			if (-1 == close(f)) { error_code = 309;  goto error; }
			return IO_RET_OK;
		}
	error:
		{
			ErrListCode(8, error_code);
			close(f);
			return IO_RET_RW_ERR;
		}
	}

	return IO_RET_OK;
}

/*--------------------------------------*/

static int read_old_ver_block_param (int f, int alfab_version /*char *blok_naglowka*/, double *Px, double *Py, char *buf, int lenmax)
/*---------------------------------------------------------------------------------------------------------------------------------*/
{
  int len ;

  if (alfab_version == verb1_0)
  {
    if (read(f,Px,sizeof(double))!=sizeof(double)) goto error;
    if (read(f,Py,sizeof(double))!=sizeof(double)) goto error;
    if (read(f,&len,sizeof(int))!=sizeof(int)) goto error;
    if (len > lenmax) goto error;
    if (read(f, buf,len)!=len) goto error;
    return IO_RET_OK ;
  error :
    return IO_RET_RW_ERR ;
  }
  else
  {
    return IO_RET_FILE_TYPE_ERR ;
  }
}

static TMENU mNie_Tak={2,0,0,7,52,8,ICONS,CMNU,CMBR,CMTX,0,0,0,0,0,&pmNie_Tak,NULL,NULL};

#ifndef LINUX
DWORD RunSilent(char* strFunct, char* strstrParams)
{
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
	char Args[4096];
	char *pEnvCMD = NULL;
	char *pDefaultCMD = "CMD.EXE";
	ULONG rc;

	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_HIDE;

	Args[0] = 0;

	pEnvCMD = getenv("COMSPEC");

	if (pEnvCMD) {

		strcpy(Args, pEnvCMD);
	}
	else {
		strcpy(Args, pDefaultCMD);
	}

	// "/c" option - Do the command then terminate the command window
	strcat(Args, " /c ");
	//the application you would like to run from the command window
	strcat(Args, strFunct);
	strcat(Args, " ");
	//the parameters passed to the application being run from the command window.
	strcat(Args, strstrParams);

	if (!CreateProcess(NULL, Args, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&StartupInfo,
		&ProcessInfo))
	{
		return GetLastError();
	}

	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
	if (!GetExitCodeProcess(ProcessInfo.hProcess, &rc))
		rc = 0;

	CloseHandle(ProcessInfo.hThread);
	CloseHandle(ProcessInfo.hProcess);

	return rc;

}
#else
#include <stdlib.h>
#endif

#ifdef LINUX

DWORD RunSilent(char* strFunct, char* strstrParams)
{
    int ret;
    char script[MAXPATH];
    char command[MAXPATH];
    char *ptr_str;

    strcpy(&script, strFunct);
    ptr_str=strstr(script, ".exe");
    if ((strcmp(strFunct, "dwg2dxf.exe")==0) || (strcmp(strFunct, "mkbitmap.exe")==0) || (strcmp(strFunct, "potrace.exe")==0) || (strcmp(strFunct, "frame3dd.exe")==0) || (strcmp(strFunct, "gnuplot.exe")==0))
    {
        if (ptr_str != NULL) {
            *ptr_str = '\0';
        }
        sprintf(command, "./%s %s", script, strstrParams);
    }
    else {
        if (ptr_str != NULL) {
            *ptr_str = '\0';
            strcat(script, ".py");
        }
        sprintf(command, "python %s %s", script, strstrParams);
    }
    printf(command, "\n");
    ret = system(command);

    return ret;  //TO CHECK
}

DWORD SystemSilent(char* strFunct, char* strstrParams)
{
    int ret;
    char command[MAXPATH];
    char *ptr_str;

    sprintf(command, "%s %s", strFunct, strstrParams);

    printf("%s\n",command);
    ret = system(command);

    return ret;  //TO CHECK
}

BOOL test_python(void)
{
    FILE *pp;
    BOOL python=0;
    //checking python
    pp = popen("python --version", "r");
    if (pp != NULL)
    {
        while (1)
        {
            char *line;
            char buf[1000];
            line = fgets(buf, sizeof buf, pp);
            if (line == NULL) break;
            if ((strstr(strlwr(line), "python") != NULL) && (strstr(strlwr(line), "not found") == NULL))
            {
                python = 1;
                break;
            }
            if (line[0] == 'd') printf("%s\n", line); /* line includes '\n' */
        }
        pclose(pp);
    }
  return python;
}

BOOL test_pillow(void)
{
    FILE *pp;
    BOOL pillow=0;
    //checking pillow
    pp = popen("python -m pip show pillow", "r");
    if (pp != NULL)
    {
        while (1)
        {
            char *line;
            char buf[1000];
            line = fgets(buf, sizeof buf, pp);
            if (line == NULL) break;
            if (strstr(strlwr(line), "version:") != NULL)
            {
                pillow = 1;
                break;
            }
            if (line[0] == 'd') printf("%s\n", line); /* line includes '\n' */
        }
        pclose(pp);
    }
    return pillow;
}


BOOL test_python_pillow(void)
{
    if (test_python()==FALSE)
    {
        int ret;
        char tools_to_install[72];
        sprintf(tools_to_install, "python + Pillow %s",  _TOOLS_TO_INSTALL_);
        ret = ask_question(1, _No_, _Yes_, "Error", tools_to_install, 12, (char *) _INSTALL_TOOLS_, 11, 1, 203);
        return FALSE;
    }
    if (test_pillow()==FALSE)
    {
        int ret;
        char tools_to_install[72];
        sprintf(tools_to_install, "Pillow for python %s",  _TOOLS_TO_INSTALL_);
        ret = ask_question(1, _No_, _Yes_, "Error", tools_to_install, 12, (char *) _INSTALL_TOOLS_, 11, 1, 203);
        return FALSE;
    }
    return TRUE;
}

BOOL Copy_File(char *ptrsz_fnd, char *ptrsz_fns)
{
    char command[FILENAME_MAX];
    sprintf(command,"/bin/cp -f %s %s",ptrsz_fns,ptrsz_fnd);
    return system(command);
}
#endif

int ReadPCX_real(char *fn,double *Px,double *Py,RYSPOZ *adp,RYSPOZ *adk, char *buf, int lenmax, int *object_no, BOOL b_current_ver)
/*-------------------------------------------------------------------------------------------------------------------------------*/
{
  char blok_naglowka [VERB_LEN + 1] ;
  char st [100] ;
  char *ad;
  char *ad1;
  int f,f_temp; 
  long_long l=0, size, size1, size0 ;
  unsigned len;
  char fn1 [MAXPATH] ;
  char fn_temp [MAXPATH] ;
  int i_ret = IO_RET_OK ;
  long_long poz_akt, poz_off, poz_blok;
  long_long dl_blok;
  NAGLOWEK1_1 nag1_1;
  NAGLOWEK nag;
  BLOKD1_1 blok1_1;
  T_Desc_Ex_Block1_1korekt ExBlok1_1;
#ifndef LINUX
  BLOKD blok;
#else
    BLOK blok;
#endif
  T_Desc_Ex_Block ExBlok;
  LINIA1_1 linia1_1;
  LINIA linia;
  OKRAG1_1 okrag1_1;
  OKRAG okrag;
  LUK1_1 luk1_1;
  LUK luk;
  TEXT_OLD1_1 tekst1_1;
  TEXT tekst;
  WIELOKAT1_1 wielokat1_1;
  WIELOKAT wielokat;
  WIELOKATX1_1 wielokatx1_1;
  VERTEX61_1 vertex61_1;
  VERTEX81_1 vertex81_1;
  TEXT_NAG1_1 tekst_nag1_1;
  TEXT_NAG tekst_nag;
  T_Point1_1 point1_1;
  T_Point point;
  char tekst_tekst[250/*80*/]; 
  int no_blok;
  unsigned long_long dl_file;
  char typ2_1;
  int aktywacja_kolorow ;
  int kolor_elementu = 0;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  BLOK 			*ptrs_block ;
  char nazwa_bloku[12];
  long_long dane_size1, dane_size2;
  B_PCX b_pcx = PCXdef;
  double b_pcx_x, b_pcx_y;
  int kolor_g, n;
  
  char sk[30]="";
  char *endptr;
  double d;
  int info_ret;
  int dlg_ret;
  char *extension;
  int k;
  char params[128];
  char image2pcx[128];
  char *comspec;
  DWORD runcode;
  int ret;
  char error_str[64];


  COLOR palette;
  PCXheader head;

#ifdef LINUX
    if (test_python_pillow()==FALSE)
        return 0;
#endif
  
  *adp = NULL ;
  *adk = NULL ;

  extension = Get_File_Ext(fn);
  if (strcmp(extension, ".PCX") != 0)  //needs conversion
  {
	  //deleting temp.pcx
	  if (__file_exists("temp.pcx")) unlink("temp.pcx");

	  sprintf(params, "\"%s\" temp.pcx", fn);
	  runcode = RunSilent("image2pcx.exe", params);
	  if (runcode > 0)
	  {
		  sprintf(error_str, "%s%d", zb_err_message, runcode);
		  ret = ask_question(1, "", zb_confirm, "", error_str, 12, "", 11, 1, 62);
		  return 0;
	  }
	  if (__file_exists("temp.pcx")) strcpy(fn, "temp.pcx");
	  else
	  {
		  sprintf(error_str, "%s%s", zb_err_message, zb_unknown);
		  ret = ask_question(1, "", zb_confirm, "", error_str, 12, "", 11, 1, 62);
		  return 0;
	  }
  }

  info_ret = LoadPCXinfo(fn, &head); // , &palette);

  //dialog 
  if (head.horz_res == 0) head.horz_res = 150;
  if (head.vert_res == 0) head.vert_res = 150;

  dlg_ret = GetPCXParams(head, &b_pcx, 1.0, 1.0, 0.0, TRUE,TRUE, FALSE, FALSE, FALSE);

  if (dlg_ret == 0)
  {
	  return 0;
  }

  strcpy (fn1, fn) ;
  File_Ext (fn1, EXT__BLOK);
  size1=0;
  size0=0;
  no_blok=0;


       strcpy(b_pcx.pcx,fn1);
       b_pcx.len_pcx=strlen(b_pcx.pcx);
       
       b_pcx.x= X;
       b_pcx.y= Y; 

	   b_pcx.kod_obiektu = 0;
	   b_pcx.warstwa = Current_Layer;
	   b_pcx.widoczny = 1;
	   b_pcx.blok = 0;
	   b_pcx.atrybut = Ablok;
       
       b_pcx.inwersja=0;
	   b_pcx.ignore_print_background = 1; // 0;
       b_pcx.set_foreground=0;
	   b_pcx.background = 15; // 255;
       b_pcx.foreground = 255;

	   b_pcx.i_x = head.xmax-head.xmin;
	   b_pcx.i_y = head.ymax - head.ymin;

	   B_PCX* pcx = (B_PCX*)&b_pcx;
	   REAL_PCX* rpcx = (REAL_PCX*)pcx->pcx;
	   memmove((char *) & rpcx->header, &head, sizeof(head));

       kolor_g=LiniaG.kolor;

       OkragG.kolor=PointG.kolor=TextG.kolor=LukG.kolor = LiniaG.kolor=kolor_g ;
       Layers[Current_Layer].color=kolor_g;
       color_bar();

       komunikat(0);

	   sel.akt = 1;
	   sel.akw = 0;

	   standard_func();

       if (!Get_Point_PCX(&b_pcx_x, &b_pcx_y, pcx, 1))
         {
           komunikat0(0);
           komunikat(0);
           return 0;
         }

       komunikat0(0);
       komunikat(0);

       b_pcx.x=b_pcx_x;
       b_pcx.y=b_pcx_y;

       if (!LoadPCX_real(&b_pcx, fn)) return 0;

       bitmap_exist=TRUE;
       bitmap_view=TRUE;

       redraw();
       
  
	   return 1;
}
/***********/

int Read_PNG_JPG_real(char *fn, double *Px, double *Py, RYSPOZ *adp, RYSPOZ *adk, char *buf, int lenmax, int *object_no, BOOL b_current_ver, int type)
/*--------------------------------------------------------------------------------------------------------------------------------------------------*/
{
	char blok_naglowka[VERB_LEN + 1];
	char st[100];
	char *ad;
	char *ad1;
	int f, f_temp;
	long_long l = 0, size, size1, size0;
	unsigned len;
	char fn1[MAXPATH];
	char fn_temp[MAXPATH];
	int i_ret = IO_RET_OK;
	long_long poz_akt, poz_off, poz_blok;
	long_long dl_blok;
	NAGLOWEK1_1 nag1_1;
	NAGLOWEK nag;
	BLOKD1_1 blok1_1;
	T_Desc_Ex_Block1_1korekt ExBlok1_1;
#ifndef LINUX
	BLOKD blok;
#else
    BLOK blok;
#endif
	T_Desc_Ex_Block ExBlok;
	LINIA1_1 linia1_1;
	LINIA linia;
	OKRAG1_1 okrag1_1;
	OKRAG okrag;
	LUK1_1 luk1_1;
	LUK luk;
	TEXT_OLD1_1 tekst1_1;
	TEXT tekst;
	WIELOKAT1_1 wielokat1_1;
	WIELOKAT wielokat;
	WIELOKATX1_1 wielokatx1_1;
	VERTEX61_1 vertex61_1;
	VERTEX81_1 vertex81_1;
	TEXT_NAG1_1 tekst_nag1_1;
	TEXT_NAG tekst_nag;
	T_Point1_1 point1_1;
	T_Point point;
	char tekst_tekst[250];
	int no_blok;
	unsigned long_long dl_file;
	char typ2_1;
	int aktywacja_kolorow;
	int kolor_elementu = 0;
	T_Desc_Ex_Block 	*ptrs_desc_bl;
	BLOK 			*ptrs_block;
	char nazwa_bloku[12];
	long_long dane_size1, dane_size2;
	B_PCX b_pcx=PCXdef;
	double b_pcx_x, b_pcx_y;
	int kolor_g, n;
	char sk[30] = "";
	char *endptr;
	double d;
	int info_ret;
	int dlg_ret;
	char *extension;
	int k;
	char params[128];
	char image2pcx[128];
	char *comspec;
	DWORD runcode;
	int ret;
	char error_str[64];

	COLOR palette;
	PCXheader head;

	unsigned int width, height, horz_res, vert_res;
	unsigned char bits_per_pixel;

#ifdef LINUX
    if (test_python_pillow()==FALSE)
        return 0;
#endif

	*adp = NULL;
	*adk = NULL;

	extension = Get_File_Ext(fn);
	if ((type==0) && (strcmp(extension, ".PNG") != 0))  //needs conversion
	{
		
		//deleting temp.png
		if (__file_exists("temp.png")) unlink("temp.png");

		sprintf(params, "\"%s\" temp.png", fn);
		runcode = RunSilent("image2png.exe", params);
		if (runcode > 0)
		{
			sprintf(error_str, "%s%d", zb_err_message, runcode);
			ret = ask_question(1, "", zb_confirm, "", error_str, 12, "", 11, 1, 62);
			return 0;
		}
		if (__file_exists("temp.png")) strcpy(fn, "temp.png");
		else
		{
			sprintf(error_str, "%s%s", zb_err_message, zb_unknown);
			ret = ask_question(1, "", zb_confirm, "", error_str, 12, "", 11, 1, 62);
			return 0;
		}
	}
	else if ((type == 1) && ((strcmp(extension, ".JPG") != 0) && (strcmp(extension, ".JPEG") != 0)))  //needs conversion
	{

		//deleting temp.png
		if (__file_exists("temp.jpg")) unlink("temp.jpg");

		sprintf(params, "\"%s\" temp.jpg", fn);
		runcode = RunSilent("image2jpg.exe", params);
		if (runcode > 0)
		{
			sprintf(error_str, "%s%d", zb_err_message, runcode);
			ret = ask_question(1, "", zb_confirm, "", error_str, 12, "", 11, 1, 62);
			return 0;
		}
		if (__file_exists("temp.jpg")) strcpy(fn, "temp.jpg");
		else
		{
			sprintf(error_str, "%s%s", zb_err_message, zb_unknown);
			ret = ask_question(1, "", zb_confirm, "", error_str, 12, "", 11, 1, 62);
			//CUR_ON(X, Y);
			return 0;
		}
	}
	else if ((type == 1) && (CheckJPGExif(fn)==TRUE)) //  needs de-exification
	{

		//deleting temp.png
		if (__file_exists("temp.jpg")) unlink("temp.jpg");

		sprintf(params, "\"%s\" temp.jpg", fn);
		runcode = RunSilent("jpg2noexif.exe", params);
		if (runcode > 0)
		{
			sprintf(error_str, "%s%d", zb_err_message, runcode);
			ret = ask_question(1, "", zb_confirm, "", error_str, 12, "", 11, 1, 62);
			return 0;
		}
		if (__file_exists("temp.jpg")) strcpy(fn, "temp.jpg");
		else
		{
			sprintf(error_str, "%s%s", zb_err_message, zb_unknown);
			ret = ask_question(1, "", zb_confirm, "", error_str, 12, "", 11, 1, 62);
			return 0;
		}
	}

	if (type==0)  info_ret = LoadPNGinfo(fn, &width, &height, &horz_res, &vert_res, &bits_per_pixel); // , &palette);
	else info_ret = LoadJPGinfo(fn, &width, &height, &horz_res, &vert_res, &bits_per_pixel); // , &palette);

	if (info_ret == 0)
	{
		sprintf(error_str, "%s", zb_err_message1);
		ret = ask_question(1, "", zb_confirm, "", error_str, 12, "", 11, 1, 62);
		return 0;
	}

	if (horz_res == 0) horz_res = 150;
	if (vert_res == 0) vert_res = 150;

	head.xmin = 0;
	head.xmax = width;
	head.ymin = 0;
	head.ymax = height;

	head.bits_per_pixel = bits_per_pixel; // 24 (fixed) or 32 for alpha channel
	head.horz_res = horz_res;
	head.vert_res = vert_res;

	//dialog 

	dlg_ret = GetPCXParams(head, &b_pcx, 1.0, 1.0, 0.0, TRUE,TRUE,FALSE, FALSE, FALSE);

	if (dlg_ret == 0)
	{
		return 0;
	}

	strcpy(fn1, fn);
	File_Ext(fn1, EXT__BLOK);
	size1 = 0;
	size0 = 0;
	no_blok = 0;

	strcpy(b_pcx.pcx, fn1);
	b_pcx.len_pcx = strlen(b_pcx.pcx);

	b_pcx.x = X;
	b_pcx.y = Y;

	b_pcx.kod_obiektu = type+2;
	b_pcx.warstwa = Current_Layer;
	b_pcx.widoczny = 1;
	b_pcx.blok = 0;
	b_pcx.atrybut = Ablok;
	
	b_pcx.inwersja = 0;
	b_pcx.ignore_print_background = 1; // 0;
	b_pcx.set_foreground = 0;
	b_pcx.background = 15; // 255;
	b_pcx.foreground = 255;
	b_pcx.i_x = width;
	b_pcx.i_y = height;

	B_PCX* pcx = (B_PCX*)&b_pcx;
	REAL_PCX* rpcx = (REAL_PCX*)pcx->pcx;

	memmove((char *) & rpcx->header, &head, sizeof(head));

	kolor_g = LiniaG.kolor;
	
	OkragG.kolor = PointG.kolor = TextG.kolor = LukG.kolor = LiniaG.kolor = kolor_g;
	Layers[Current_Layer].color = kolor_g;
	color_bar();

	komunikat(0);

	sel.akt = 1;
	sel.akw = 0;

	standard_func();

	if (!Get_Point_PCX(&b_pcx_x, &b_pcx_y, pcx, 1))
	{
		komunikat0(0);
		komunikat(0);
		return 0;
	}

	komunikat0(0);
	komunikat(0);


	b_pcx.x = b_pcx_x;
	b_pcx.y = b_pcx_y;

	
    if (!Load_PNG_JPG_real(&b_pcx, fn, width, height, horz_res, vert_res, bits_per_pixel, type)) return 0;
	

	if (b_pcx.on_front) bitmap_on_front_exist=TRUE;

	bitmap_exist = TRUE;
	bitmap_png_exist = TRUE;
	bitmap_view = TRUE;

	redraw();

	return 1;
}

static BOOL
get_vectorization_param (T_Fstring key_name, T_Fstring ret_string)
/*--------------------------------------------------------------*/
{
    int i, val_int;
    double val_double;
    char vectorization_par[128];

    //strupr (key_name);
    utf8Upper(key_name);

    for (i = 0; i < no_vectorization_param; i++)
    {
        //strupr (&prn_config_param [i]);

        strcpy(vectorization_par, vectorization_param [i]);
        utf8Upper(&vectorization_par);

        if (stricmp (key_name, vectorization_par) == 0)
        {
            break;
        }
    }
    if (i >= no_vectorization_param)
    {
        return FALSE;
    }

    switch (i)
    {
        case PBM_scale:
            if ( sscanf ( ret_string, "%d", &val_int) == 1 )
            {
                if (val_int > 0)
                    pbm_scale = val_int;
            }
            break;
        case PBM_interpolation:
            if ( sscanf ( ret_string, "%d", &val_int) == 1 )
            {
                if ((val_int == 1) || (val_int == 3))
                    pbm_interpolation = val_int;
            }
            break;
        case PBM_threshold:
            if ( sscanf ( ret_string, "%lf", &val_double) == 1 )  //lf
            {
                if (val_double > 0)
                    pbm_threshold = val_double;
            }
            break;
        case PBM_invert:
            if ( sscanf ( ret_string, "%d", &val_int) == 1 )
            {
                if ((val_int == 0) || (val_int == 1))
                    pbm_invert = val_int;
               // printf("inversion %d",pbm_invert);
            }
            break;
        case PBM_filter:
            if ( sscanf ( ret_string, "%d", &val_int) == 1 )
            {
                if (val_int > 0)
                    pbm_filter = val_int;
            }
            break;
        case PBM_nofilter:
            if (sscanf(ret_string, "%d", &val_int) == 1)
            {
                if ((val_int == 0) || (val_int == 1))
                    pbm_nofilter = val_int;
                //printf("nofilter %d",pbm_nofilter);
            }
            break;
        case ALX_scale:
            if ( sscanf ( ret_string, "%lf", &val_double) == 1 )  //lf
            {
                if (val_double > 0)
                    alx_scale = val_double;
            }
            break;
        case ALX_blacklevel:
            if ( sscanf ( ret_string, "%lf", &val_double) == 1 )  //lf
            {
                if (val_double > 0)
                    alx_blacklevel = val_double;
            }
            break;
        case ALX_turdsize:
            if (sscanf(ret_string, "%d", &val_int) == 1)
            {
                if (val_int > 0)
                    alx_turdsize = val_int;
            }
        break;
        case ALX_alphamax:
            if (sscanf(ret_string, "%d", &val_int) == 1)
            {
                if (val_int > 0)
                    alx_alphamax = val_int;
            }
        break;
        case ALX_curve_tolerance:
            if ( sscanf ( ret_string, "%lf", &val_double) == 1 )  //lf
            {
                if (val_double > 0)
                    alx_curve_tolerance = val_double;
            }
        break;
        case ALX_quantize:
            if (sscanf(ret_string, "%d", &val_int) == 1)
            {
                if (val_int > 0)
                    alx_quantize = val_int;
            }
        break;
        default:
            break;
    }
    return TRUE;
}


int Convert_Image_to_ALX(char *fn) {
    char *extension;
    char params[256];
    DWORD runcode;
    int ret;
    char error_str[64];
    char fn_temp[MAXPATH];
    char known_image_formats[][6]={".PNG",".JPG",".JPEG",".PCX", ".TIFF"};
    char known_bitmap_formats[][6]={".BMP",".PBM",".PGM",".PPM"};
    int f_size, i;
    BOOL found;
    BOOL retval;
    char PBM_invert_[6], PBM_nofilter_[6];

	char drive[MAXDRIVE];
	char dir[MAXDIR];
	char file[MAXFILE];
	char ext[MAXEXT];
	int flags;


    retval = Get_Private_Profile_Strings ((T_Fstring)VECTORIZATION, get_vectorization_param);
    printf("\n");

    extension = Get_File_Ext(fn);

    f_size=sizeof(known_image_formats)/sizeof(known_image_formats[0]);
    found=FALSE;
    for (i=0; i<f_size; i++) if (strcmp(extension, known_image_formats[i]) == 0)
        {
           found=TRUE;
           break;
        }

    if (found)
    {

//deleting temp.png
        if (__file_exists("temp.bmp")) unlink("temp.bmp");

        komunikat0(148);

        sprintf(params, "\"%s\" temp.bmp 72", fn);
        runcode = RunSilent("image2image.exe", params);
        if (runcode > 0) {
            sprintf(error_str, "%s%d", zb_err_message, runcode);
            ret = ask_question(1, "", zb_confirm, "", error_str, 12, "", 11, 1, 62);

            komunikat0(0);
            return 0;
        }
        strcpy(fn_temp, "temp.bmp");
    }
    else
    {
        f_size=sizeof(known_bitmap_formats)/sizeof(known_bitmap_formats[0]);
        found=FALSE;
        for (i=0; i<f_size; i++) if (strcmp(extension, known_bitmap_formats[i]) == 0)
            {
                found=TRUE;
                break;
            }
        if (found) strcpy(fn_temp, fn);
        else
        {
            sprintf(error_str, "%s", zb_err_message1);
            ret = ask_question(1, "", zb_confirm, "", error_str, 12, "", 11, 1, 62);

            return 0;
        }
    }

    if (__file_exists(fn_temp))
    {
        if (__file_exists("temp.pbm")) unlink("temp.pbm");

        komunikat0(149);

        if (pbm_invert==1) strcpy(PBM_invert_,"-i "); else strcpy(PBM_invert_,"");
        if (pbm_nofilter==1) strcpy(PBM_nofilter_,"-n "); else strcpy(PBM_nofilter_,"");

        sprintf(params, "-f %d -s %d -%d -t %f %s%s-o temp.pbm \"%s\"", pbm_filter, pbm_scale, pbm_interpolation, pbm_threshold, PBM_invert_,PBM_nofilter_, fn_temp);
        runcode = RunSilent("mkbitmap.exe", params);
    }
    else {
        sprintf(error_str, "%s%s", zb_err_message, zb_unknown);
        ret = ask_question(1, "", zb_confirm, "", error_str, 12, "", 11, 1, 62);

        komunikat0(0);
        return 0;
    }

    if (__file_exists("temp.pbm"))
    {
        if (__file_exists("temp.alx")) unlink("temp.alx");

        komunikat0(150);

		flags = fnsplit(fn, drive, dir, file, ext);

        sprintf(params, "-x %f -k %f -t %d -a %d -O %f -u %d -b alx -f %s temp.pbm", alx_scale, alx_blacklevel, alx_turdsize, alx_alphamax, alx_curve_tolerance, alx_quantize, file);
        runcode = RunSilent("potrace.exe", params);
    }
    else {
        sprintf(error_str, "%s%s", zb_err_message, zb_unknown);
        ret = ask_question(1, "", zb_confirm, "", error_str, 12, "", 11, 1, 62);

        komunikat0(0);
        return 0;
    }

    if (__file_exists("temp.alx"))
    {
        komunikat0(0);
        return 1;
    }

    komunikat0(0);
    return 0;
}


int ReadBlock_ (char *fn,double *Px,double *Py,RYSPOZ *adp,RYSPOZ *adk,	char *buf, int lenmax, int *object_no, BOOL b_current_ver)
/*------------------------------------------------------------------------------------------------------------------------------*/
{
  char blok_naglowka [VERB_LEN + 1] ;
  int alfab_version;
  char st [100] ;
  char *ad;
  int f,f_temp; 
  long_long l=0, size, size1, size0 ;
  unsigned len;
  char fn1 [MAXPATH] ;
  char fn_temp [MAXPATH] ;
  int i_ret = IO_RET_OK ;
  long_long poz_akt, poz_off, poz_blok;
  long_long dl_blok;
  NAGLOWEK1_1 nag1_1;
  NAGLOWEK nag;
  BLOKD1_1 blok1_1;
  T_Desc_Ex_Block1_1korekt ExBlok1_1;
#ifndef LINUX
  BLOKD blok = BDdef;
#else
    BLOK blok = Bdef;
#endif
  T_Desc_Ex_Block ExBlok;
  LINIA1_1 linia1_1;
  LINIAEND1_1 liniaend1_1;
  LINIA linia;
  OKRAG1_1 okrag1_1;
  OKRAGEND1_1 okragend1_1;
  OKRAG okrag;
  LUK1_1 luk1_1;
  LUKEND1_1 lukend1_1;
  LUK luk;
  TEXT_OLD1_1 tekst1_1;
  TEXT3 tekst;
  WIELOKAT1_1 wielokat1_1;
  WIELOKAT wielokat;
  WIELOKATX1_1 wielokatx1_1;
  VERTEX61_1 vertex61_1;
  VERTEX81_1 vertex81_1;
  TEXT_NAG1_1 tekst_nag1_1;
  TEXT_NAGEND1_1 tekst_nagend1_1;
  TEXT_NAG3 tekst_nag;
  T_Point1_1 point1_1;
  T_PointEND1_1 pointend1_1;
  T_Point point;
  char tekst_tekst[MaxLen];
  int no_blok;
  unsigned long_long dl_file;
  char typ2_1;
  int aktywacja_kolorow ;
  int kolor_elementu = 0;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  BLOK 			*ptrs_block ;
  char nazwa_bloku[12];
  long_long dane_size1, dane_size2, dane_size00;
  BLOK *adb;
  char  *adh;
  char buf_utf8[MaxLen];
  int numer_bledu;
  BOOL e_of_f=FALSE;
  off_t new_position;
  int ex_block_pos;
  char block_type [Max_Spec_Block] ;
 
  *adp = NULL ;
  *adk = NULL ;

  char* ptr;
  int a;
  ptr = strchr(fn, '\024');

  if (ptr != NULL)
	  a = 0;


  strcpy (fn1, fn) ;
  File_Ext (fn1, EXT__BLOK);
  size1=0;
  size0=0;
  no_blok=0;
  
read_again:  

  f= open (fn1, O_RDONLY|O_BINARY);
  
  if (f!=-1)
   {
    if (read (f, blok_naglowka, VERB_LEN) != (VERB_LEN))
         {
	   goto error ;
	 } 
    blok_naglowka[VERB_LEN]='\0';

    if (strcmp (blok_naglowka, VERB2_1) == 0)  alfab_version=verb2_1;
    else if (strcmp (blok_naglowka, VERB3_0) == 0)  alfab_version=verb3_0;
    else if (strcmp (blok_naglowka, VERB_ASTER) == 0)  alfab_version=verb1_0;
    else {
        char *endptr;
        float num;
        blok_naglowka[VERB_LEN-2]='\0';
        //num = strtof(&blok_naglowka[7], &endptr);
        num = atof(&blok_naglowka[7]);
        alfab_version = FLOAT_TO_INT(num * 100);
    }

    if (alfab_version == verb1_0)
    {
      close(f);
      return IO_RET_RW_ERR ;
    }  
    if (alfab_version == verb1_1)  //(strcmp (blok_naglowka, VERB1_1) == 0)
    {
#ifdef LINUX
        close(f);
        return IO_RET_RW_ERR ;
#endif

      if (now_is_dialog==0) komunikat0(108);
      
      if (read_c(f,Px,8)!=8) 
         {
	   goto error ;
	 }
      if (read_c(f,Py,8)!=8)
         {
	   goto error ;
	 }
      if (read_i(f,&len,2)!=2)
         {
	   goto error ;
	 }

      if (read_c(f,buf,len)!=len) 
         {
	   goto error ;
	 }

      strcpy (fn_temp, fn1) ;
      File_Tmp (fn_temp, EXT__TMP);

#ifndef LINUX
        f_temp=open(fn_temp, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
        f_temp=open(fn_temp, O_CREAT|O_TRUNC|O_RDWR, 0666); //S_IRWXU | S_IRWXG | S_IRWXO);
#endif

      strncpy(blok_naglowka,VERB2_1,VERB_LEN);
      if (write(f_temp, &blok_naglowka, VERB_LEN) != VERB_LEN) goto error1;
      if (write(f_temp,Px,sizeof(double))!=sizeof(double)) goto error1;
      if (write(f_temp,Py,sizeof(double))!=sizeof(double)) goto error1;
      if (write(f_temp,&len,sizeof(int))!=sizeof(int)) goto error1;
      if (write(f_temp,buf,len)!=len) goto error1;	 
      
      dl_file=filelength(f);
 
      *object_no = 0 ; 	/*licznik obiektow*/
      while (1)
        { 
          /*ODCZYT - KONWERSJA - ZAPIS  - NAJPROSCIEJ*/
          poz_off = lseek (f, 0, 1);
     
          if (poz_off>=dl_file) break;
          if (read(f,&nag1_1,sizeof(nag1_1))!=sizeof(nag1_1)) goto error1;
          poz_off = lseek (f, 0, 1);
          poz_off -= sizeof(nag1_1);
          if (poz_off != lseek (f, poz_off, SEEK_SET)) goto error1;
          
          switch (nag1_1.obiekt)
          {
          case Okoniec:
       
            nag.atrybut=nag1_1.atrybut;
            nag.obiekt=nag1_1.obiekt;
            nag.obiektt1=0; //nag1_1.obiektt1;
            nag.obiektt2=0; //nag.obiektt2;
            nag.obiektt3=0; //nag.obiektt3;
            nag.widoczny=nag1_1.widoczny;
            nag.przec=nag1_1.przec;
            nag.blok=nag1_1.blok;
            nag.n=nag1_1.n;   
            if (write(f_temp, &nag, sizeof(nag))!=(sizeof(nag))) goto error1;
            goto go_koniec;
            break ;           
          case Otekst :
        
            if (read(f, &tekst_nag1_1, 6)!=6) goto error1;
            if (read(f, &tekst_nagend1_1, 22)!=22) goto error1;
            if (read(f, &tekst_tekst, tekst_nagend1_1.dl + 1)!=tekst_nagend1_1.dl + 1) goto error1;
            tekst_nag.atrybut = tekst_nag1_1.atrybut ;
            tekst_nag.obiekt = tekst_nag1_1.obiekt ; 
            tekst_nag.obiektt1 = 0; //tekst_nag1_1.obiektt1 ;
            tekst_nag.obiektt2 = 0; //tekst_nag1_1.obiektt2 ;
            tekst_nag.obiektt3 = 0; //tekst_nag1_1.obiektt3 ; 
            tekst_nag.widoczny = tekst_nag1_1.widoczny ; 
            tekst_nag.przec = tekst_nag1_1.przec ; 
            tekst_nag.blok = 1;
            tekst_nag.n = tekst_nag1_1.n + 2 ;  
  	    tekst_nag.warstwa = tekst_nag1_1.warstwa ; 
            tekst_nag.kolor = tekst_nag1_1.kolor ; 
            tekst_nag.czcionka = tekst_nag1_1.czcionka ;
            tekst_nag.kat = tekst_nagend1_1.kat ;
            tekst_nag.wysokosc = tekst_nagend1_1.wysokosc ;
            tekst_nag.width = tekst_nagend1_1.width ;
            tekst_nag.x = tekst_nagend1_1.x ;
            tekst_nag.y = tekst_nagend1_1.y ;
            tekst_nag.italic = tekst_nagend1_1.italic ;
			tekst_nag.bold=0;
            tekst_nag.typ = 0;
            tekst_nag.justowanie = 0;
            tekst_nag.ukryty = 0;
            tekst_nag.dl = tekst_nagend1_1.dl ;
            tekst_tekst[tekst_nag.dl + 1]='\0';
            if (write(f_temp, &tekst_nag, sizeof(tekst_nag) -2)!=(sizeof(tekst_nag) - 2)) goto error1;
            if (write(f_temp, &tekst_tekst, tekst_nag.dl + 1)!=(tekst_nag.dl + 1)) goto error1;
	   break ;
         case Olinia :
       
           if (read(f, &linia1_1, 6)!=6) goto error1;
           if (read(f, &liniaend1_1, 16)!=16) goto error1;
           linia.atrybut = linia1_1.atrybut ;
           linia.obiekt = linia1_1.obiekt ; 
           linia.obiektt1 = 0;//linia1_1.obiektt1 ;
           linia.obiektt2 = 0;//linia1_1.obiektt2 ;
           linia.obiektt3 = 0;//linia1_1.obiektt3 ; 
           linia.widoczny = linia1_1.widoczny ; 
           linia.przec = linia1_1.przec ; 
           linia.blok = 1 ; 
           linia.n = linia1_1.n + 2;
           linia.warstwa = linia1_1.warstwa ; 
           linia.kolor = linia1_1.kolor ; 
           linia.typ = typ_linii2_0_to_2_1(linia1_1.typ) ;
           linia.x1 = liniaend1_1.x1 ;
           linia.y1 = liniaend1_1.y1 ;
           linia.x2 = liniaend1_1.x2 ;
           linia.y2 = liniaend1_1.y2 ;
           if (write(f_temp, &linia, sizeof(linia))!= sizeof(linia)) goto error1;
	   break ;
         case Oluk :
       
		   if (read(f, &luk1_1, 6)!=6) goto error1;
           if (read(f, &lukend1_1, 20)!=20) goto error1;
           luk.atrybut = luk1_1.atrybut ;
           luk.obiekt = luk1_1.obiekt ; 
           luk.obiektt1 = 0;//luk1_1.obiektt1 ;
           luk.obiektt2 = 0;//luk1_1.obiektt2 ;
           luk.obiektt3 = 0;//luk1_1.obiektt3 ; 
           luk.widoczny = luk1_1.widoczny ; 
           luk.przec = luk1_1.przec ; 
           luk.blok = 1;
           luk.n = luk1_1.n + 2 ;  
           luk.warstwa = luk1_1.warstwa ; 
           luk.kolor = luk1_1.kolor ; 
           luk.typ = typ_linii2_0_to_2_1(luk1_1.typ) ;
           luk.x = lukend1_1.x ;
           luk.y = lukend1_1.y ;
           luk.r = lukend1_1.r ;
           luk.kat1 = lukend1_1.kat1 ;
           luk.kat2 = lukend1_1.kat2 ;
           if (write(f_temp, &luk, sizeof(luk))!=sizeof(luk)) goto error1;
	   break ;			
         case Ookrag :
     
           if (read(f, &okrag1_1, 6)!=6) goto error1;
		   if (read(f, &okragend1_1, 12)!=12) goto error1;
           okrag.atrybut = okrag1_1.atrybut ;
           okrag.obiekt = okrag1_1.obiekt ; 
           okrag.obiektt1 = 0;//okrag1_1.obiektt1 ;
           okrag.obiektt2 = 0;//okrag1_1.obiektt2 ;
           okrag.obiektt3 = 0;//okrag1_1.obiektt3 ; 
           okrag.widoczny = okrag1_1.widoczny ; 
           okrag.przec = okrag1_1.przec ; 
           okrag.blok = 1;
           okrag.n = okrag1_1.n + 2 ;  
           okrag.warstwa = okrag1_1.warstwa ; 
           okrag.kolor = okrag1_1.kolor ; 
           okrag.typ = typ_linii2_0_to_2_1(okrag1_1.typ) ;
           okrag.x = okragend1_1.x ;
           okrag.y = okragend1_1.y ;
           okrag.r = okragend1_1.r ;
           if (write(f_temp, &okrag, sizeof(okrag))!=sizeof(okrag)) goto error1;
	   break ;	
         case Okolo :
     
           if (read(f, &okrag1_1, 6)!=6) goto error1;
		   if (read(f, &okragend1_1, 12)!=12) goto error1;
           okrag.atrybut = okrag1_1.atrybut ;
           okrag.obiekt = okrag1_1.obiekt ; 
           okrag.obiektt1 = 0;//okrag1_1.obiektt1 ;
           okrag.obiektt2 = 0;//okrag1_1.obiektt2 ;
           okrag.obiektt3 = 0;//okrag1_1.obiektt3 ; 
           okrag.widoczny = okrag1_1.widoczny ; 
           okrag.przec = okrag1_1.przec ; 
           okrag.blok = 1;
           okrag.n = okrag1_1.n + 2 ;  
           okrag.warstwa = okrag1_1.warstwa ; 
           okrag.kolor = okrag1_1.kolor ; 
           okrag.typ = typ_linii2_0_to_2_1(okrag1_1.typ);
           okrag.x = okragend1_1.x ;
           okrag.y = okragend1_1.y ;
           okrag.r = okragend1_1.r ;
           if (write(f_temp, &okrag, sizeof(okrag))!=sizeof(okrag)) goto error1;
	   break ;		
         case Owwielokat :
    
           if (read(f, &wielokatx1_1, sizeof(wielokatx1_1))!=sizeof(wielokatx1_1)) goto error1;
           if (wielokatx1_1.lp==6)
            { 
             if (read(f, &vertex61_1, sizeof(vertex61_1))!=sizeof(vertex61_1)) goto error1;
            }
             else 
             {
              if (read(f, &vertex81_1, sizeof(vertex81_1))!=sizeof(vertex81_1)) goto error1;
             }
           wielokat.atrybut = wielokatx1_1.atrybut ;
           wielokat.obiekt = wielokatx1_1.obiekt ; 
           wielokat.obiektt1 = 0;//wielokatx1_1.obiektt1 ;
           wielokat.obiektt2 = 0;//wielokatx1_1.obiektt2 ;
           wielokat.obiektt3 = 0;//wielokatx1_1.obiektt3 ; 
           wielokat.widoczny = wielokatx1_1.widoczny ; 
           wielokat.przec = wielokatx1_1.przec ; 
           wielokat.blok = 1;
           wielokat.n = wielokatx1_1.n + 4 /*2*/ ;  
           wielokat.warstwa = wielokatx1_1.warstwa ; 
           wielokat.kolor = wielokatx1_1.kolor ; 
           wielokat.temp1 = wielokatx1_1.temp1 ;
		   wielokat.translucent = 0;
           wielokat.empty_typ = wielokatx1_1.rezerwa1 ;
           wielokat.lp = wielokatx1_1.lp ;
           if (wielokat.lp==6) 
           {
            wielokat.xy[0] = vertex61_1.xy[0] ;
            wielokat.xy[1] = vertex61_1.xy[1] ;
            wielokat.xy[2] = vertex61_1.xy[2] ; 
            wielokat.xy[3] = vertex61_1.xy[3] ;
            wielokat.xy[4] = vertex61_1.xy[4] ;
            wielokat.xy[5] = vertex61_1.xy[5] ;
           } 
           else
            if (wielokat.lp==8) 
            {
             wielokat.xy[0] = vertex81_1.xy[0] ;
             wielokat.xy[1] = vertex81_1.xy[1] ;
             wielokat.xy[2] = vertex81_1.xy[2] ; 
             wielokat.xy[3] = vertex81_1.xy[3] ;
             wielokat.xy[4] = vertex81_1.xy[4] ;
             wielokat.xy[5] = vertex81_1.xy[5] ;
             wielokat.xy[6] = vertex81_1.xy[6] ; 
             wielokat.xy[7] = vertex81_1.xy[7] ;
            }
            else
            {
             wielokat.xy[0] = 0;
             wielokat.xy[1] = 0;
             wielokat.xy[2] = 0; 
             wielokat.xy[3] = 0;
             wielokat.xy[4] = 0;
             wielokat.xy[5] = 0;
             wielokat.xy[6] = 0; 
             wielokat.xy[7] = 0;
             }
             
            if (write(f_temp, &wielokat, wielokat.n+sizeof(NAGLOWEK))!=(wielokat.n+sizeof(NAGLOWEK))) goto error1;
	   break ;	
         case Opoint :
      
           if (read(f, &point1_1, 6)!=6) goto error1;
           if (read(f, &pointend1_1, 8)!=8) goto error1;
           point.atrybut = point1_1.atrybut ;
           point.obiekt = point1_1.obiekt ; 
           point.obiektt1 = 0;//point1_1.obiektt1 ;
           point.obiektt2 = 0;//point1_1.obiektt2 ;
           point.obiektt3 = 0;//point1_1.obiektt3 ; 
           point.widoczny = point1_1.widoczny ; 
           point.przec = point1_1.przec ; 
           point.blok = 1 ; 
           point.n = point1_1.n + 2 ;  
           point.warstwa = point1_1.warstwa ; 
           point.kolor = point1_1.kolor ; 
		   point.typ = 0;
           point.x = pointend1_1.x ;
           point.y = pointend1_1.y ;
           if (write(f_temp, &point, sizeof(point))!=sizeof(point)) goto error1;
	   break ;	
      case OdBLOK :
        /*tuta moga byc 2 modyfikacje*/ 
        /*blok normalny - bedzie ignorowany*/
        /*blok rozszerzony ze wspolrzednymi - bedzi przepisywany */
       
          no_blok++;
          if (read(f, &blok1_1, 7 /*sizeof(blok1_1)*/)!= 7/*sizeof(blok1_1)*/) goto error1;
          poz_off = lseek (f, 0, 1);
          if (blok1_1.dlugosc_opisu_obiektu>0)
          {
          if (blok1_1.kod_obiektu=='E')
           {
             if (read_i(f, &ExBlok1_1.flags, 2)!=2 ) goto error1;
             if (ExBlok1_1.flags==2)
               {
                if (read(f, &ExBlok1_1.x, 4)!=4 ) goto error1;
                if (read(f, &ExBlok1_1.y, 4)!=4 ) goto error1;
               } 
             if (read_i(f, &ExBlok1_1.len, 2)!=2 ) goto error1;  
             if (read(f, &tekst_tekst, ExBlok1_1.len)!=ExBlok1_1.len) goto error1;
           }
           else
            {
            if (read(f, &tekst_tekst, blok1_1.dlugosc_opisu_obiektu)!=blok1_1.dlugosc_opisu_obiektu) goto error1;
        
            }
          }  
          if (no_blok<2)
          { 
           (*object_no)++ ;
           blok.atrybut = blok1_1.atrybut ;
           blok.obiekt = blok1_1.obiekt ; 
           blok.obiektt1 = 0;//blok1_1.obiektt1 ;
           blok.obiektt2 = 0;//blok1_1.obiektt2 ;
           blok.obiektt3 = 0;//blok1_1.obiektt3 ; 
           blok.widoczny = blok1_1.widoczny ; 
           blok.przec = blok1_1.przec ; 
           blok.blok = 0 ; 
           blok.n = blok1_1.n;    /*na razie....*/
           blok.kod_obiektu = blok1_1.kod_obiektu; 
           blok.flag = 0;  //rezerwa
           blok.dlugosc_opisu_obiektu = blok1_1.dlugosc_opisu_obiektu + 4; 
           ExBlok.flags = ExBlok1_1.flags;
           ExBlok.x = ExBlok1_1.x;
           ExBlok.y = ExBlok1_1.y;
           ExBlok.len = ExBlok1_1.len;
         
           poz_akt = lseek (f_temp, 0, 1);
           poz_blok = poz_akt + 4;

           if (write(f_temp, &blok, sizeof(blok))!= sizeof(blok)) goto error1;
#ifdef LINUX
           ex_block_pos=lseek(f_temp, -4, SEEK_CUR);
#endif
           if (write(f_temp, &ExBlok.flags, sizeof(ExBlok.flags))!= sizeof(ExBlok.flags)) goto error1; 
           if (ExBlok.flags==2)
           {
            if (write(f_temp, &ExBlok.x, sizeof(ExBlok.x))!= sizeof(ExBlok.x)) goto error1; 
            if (write(f_temp, &ExBlok.y, sizeof(ExBlok.y))!= sizeof(ExBlok.y)) goto error1; 
           } 
           if (write(f_temp, &ExBlok.len, sizeof(ExBlok.len))!= sizeof(ExBlok.len)) goto error1; 
           if (write(f_temp, &tekst_tekst, ExBlok.len)!= ExBlok.len) goto error1; 
	  }
	break ;
      default :
  
	break ;
        }	  /*switch*/
      }  /*while*/  
      /*uaktualnienie dlugosci bloku*/  
go_koniec:
      close(f);  
      poz_akt = lseek (f_temp, 0, 1);
      dl_blok = poz_akt - poz_blok - sizeof(long);
      if (lseek(f_temp, poz_blok, SEEK_SET) !=poz_blok) goto error1;
      poz_akt = lseek (f_temp, 0, 1);
      if (write(f_temp, &dl_blok, sizeof(long))!= sizeof(long)) goto error1;
      close(f_temp); 
	  if (now_is_dialog==0) komunikat0(109);
      /*zapis zbioru *.bak*/
      file_bak (fn1, EXT__BLOK);
      /*******************/     
      /* ZAMIANA PLIKOW */
      if (rename (fn_temp, fn1))
      {

		strcpy(st, _ERROR_WREITING_AUX_FILE_);

        ErrListStr(st);
        return IO_RET_RW_ERR ;
      }  
      /******************/
      if (now_is_dialog==0) komunikat0(0);
      goto read_again;
      /*****************/
error1:      
      close(f);
      close(f_temp);

      strcpy(st, _FILE_CONVERSION_ERROR_);

      strcat(st,fn);
      ErrListStr(st);
      //delay(200);
      return IO_RET_RW_ERR ;
    }

    else if (alfab_version >= verb2_0)
    {
      if (read(f,Px,sizeof(double))!=sizeof(double)) 
         {
	   goto error ;
	      }
      if (read(f,Py,sizeof(double))!=sizeof(double))
         {
	   goto error ;
	      }
      if (read(f,&len,sizeof(int))!=sizeof(int))
         {
	   goto error ;
	      }
     
      if (read(f,buf,len)!=len) 
         {
	   goto error ;
	      }
	   buf[lenmax]='\0';

	   if (alfab_version < verb4_1) //(strcmp(blok_naglowka, VERB4_1) != 0)
	   {
		   len = mazovia2utf8(buf, &buf_utf8, lenmax-10);
		   strncpy(buf, buf_utf8, lenmax - 10);
	   }

      *object_no = 0 ; 	/*licznik obiektow*/
      aktywacja_kolorow = 0;

   	  if((ad = dodaj_obiektFD (NULL, f)) == NULL)
	  {
	    i_ret = IO_RET_MEM_ERR ;
	    goto aa ;
	  }
	  if (alfab_version == verb4_0) //(strcmp(blok_naglowka, VERB4_0) == 0)
	  {
		  if (FALSE == ReadBlockStyles40(f)) SetDefaultStyles();
	  }
	  else if (alfab_version >= verb4_1) //(strcmp(blok_naglowka, VERB4_1) == 0)
	  {
	  if (FALSE == ReadBlockStyles(f)) SetDefaultStyles();
	  }
	  else SetDefaultStyles();

	  if (alfab_version >= verb4_1) //(strcmp(blok_naglowka, VERB4_1) == 0)
	  {
#ifdef LINUX
          new_position = lseek(f, 0, SEEK_CUR);
          if (new_position >= dl_file) e_of_f = TRUE;
          if (!e_of_f)
#else
		  if (!eof(f))
#endif
		  {
			  if (FALSE == ReadPatterns(f, &numer_bledu))
			  {

				  ;
			  }
		  }
	  }

	  (*object_no)++ ;
	  l+=((NAGLOWEK*)ad)->n+sizeof(NAGLOWEK);
	  if ( ((LINIA*)ad)->obiekt != OdBLOK )
 	  {
	    ((LINIA*)ad)->warstwa = Current_Layer;
	    ((LINIA*)ad)->blok = NoElemBlok ;
		
	      
	    if (alfab_version == verb2_0)
	    {
	     /*zmiana typu linii*/
	     /****/
	     switch (((LINIA*)ad)->obiekt )
             {
              case Olinia :
              case Oluk   :
              case Ookrag : typ2_1=typ_linii2_0_to_2_1(((LINIA*)ad)->typ);
                            ((LINIA*)ad)->typ=typ2_1;  
	                    break ;
              default     :
	                    break ;
	     }              
	    } 
	  }
	  else
	  {
	    
	    adb = (BLOK*)ad;
	    adh=(char *)adb+sizeof(NAGLOWEK)+adb->n-1;
	    /*sprawdzenie aktywacji kolorow*/
	   if (ignoruj_aktywacja==0)
	   {
	    ptrs_block = (BLOK*)ad;
	    ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu [0]) ;
            strncpy(nazwa_bloku,&ptrs_desc_bl->sz_type[0],6) ;
	    if (strlen(nazwa_bloku)>0)
             { 
               if ((strncmp ( nazwa_bloku,"$J",2 ) == 0)) 
                  {/*wezel*/
                   aktywacja_kolorow=2;
                   kolor_elementu=kolorya.wezel;
                  }
                 else 
                 { if ((strncmp ( nazwa_bloku,"$X",2 ) == 0)) 
                  {/*zacisk listwy +*/
                   aktywacja_kolorow=3;
                   kolor_elementu=kolorya.zacisk;
                  }
                  else
                   { if ((strncmp ( nazwa_bloku,"$PWR",4 ) == 0)) 
                     {/*zasilanie +*/
                      aktywacja_kolorow=4;
                      kolor_elementu=kolorya.zasilanie;
                     }
                     else
                      { if ((strncmp ( nazwa_bloku,"$PORT",5 ) == 0)) 
                       {/*port +*/
                         aktywacja_kolorow=5;
                         kolor_elementu=kolorya.port;
                       }
                       else
                        {  if ((strncmp ( nazwa_bloku,"$SCHEM",6 ) == 0)) 
                          {/*schemat+*/
                           aktywacja_kolorow=6;
                           kolor_elementu=kolorya.schemat;
                          }
                         else
                          { if ((strncmp ( nazwa_bloku,"$ENTER",6 ) == 0)) 
                            {/*aparat+*/
                             aktywacja_kolorow=7;
                             kolor_elementu=LiniaG.kolor;
                            }
                            else
                             {
                              if ((strncmp ( nazwa_bloku,"$",1 ) == 0)) 
                              {/*aparat+*/
                               aktywacja_kolorow=1;
                               kolor_elementu=kolorya.aparat;
                              }
                             }
                          } 
                        }
                      }
                   }
                 }
                  
             } 
           }  
	    if (options1.save_original_layer==0) change_layer ((char  *)ad, adh, Current_Layer); 
	      else normalize_layer ((char *)ad, adh);
	    if (alfab_version == verb2_0) //(strcmp (blok_naglowka, VERB2_0) == 0)
	     {
	      change_types_2_0_to_2_1 ((char *)ad, adh);
	     }
	    if ((aktywacja_kolorow>0) && (ignoruj_aktywacja==0))
	       change_colors((char *)ad, adh, kolor_elementu); /*dla astera*/ 
	  }

        if (alfab_version < verb3_0)
		{
             
          dane_size00=dane_size;
          //zmiana struktury tekstow w wersjach <> 3_0
		  ad=dane;
		  adh=dane+l-1;
	      change_texts_2_1_to_3_0 (ad, adh); 
		  l += (dane_size-dane_size00);
		}

		//converting to utf8 
		if (alfab_version < verb4_1) //(strcmp(blok_naglowka, VERB4_1) != 0)
		{
			dane_size00 = dane_size;
			ver4_0_to_4_1(0, l - 1, (char*)&block_type);
			l += (dane_size - dane_size00);
		}
        else if (alfab_version < verb4_2)
        {
            dane_size00 = dane_size;
            if (FALSE == ver4_1_to_4_2(0, l - 1));
            l += (dane_size - dane_size00);
        }

		//changin text styles indexes, has to be done always
		if (FALSE == change_textstyle_indexes(0, l-1))
		{
			;
		}


#ifdef UPDATE_DATE		
		SYSTEMTIME thesystemtime;
		FILETIME thefiletime;
		GetSystemTime(&thesystemtime);
		SystemTimeToFileTime(&thesystemtime, &thefiletime);
		//if ((strcmp(blok_naglowka, VERB2_1) == 0) || (strcmp(blok_naglowka, VERB2_1_) == 0))
        if (alfab_version == verb2_1)
		{
			LPWIN32_FIND_DATAA data1;
			HANDLE filehandle = CreateFile(fn1, FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			SetFileTime(filehandle, (LPFILETIME) NULL, (LPFILETIME) NULL, &thefiletime);
			CloseHandle(filehandle);
		}
#endif

    }  /*ver 2_0, 2_1, 3_0 and 4_0 and 4_1*/
     else
      {
       goto error;
      }
aa:
    if (l > 0)
    {
       *adp=dane;
       *adk=dane+l-1;
	   
       if (FALSE == Check_Objects (0, l - 1, FALSE))
       {
	     goto error ;
       } 
	   
	   zmien_atrybut (dane, dane + l - 1, ANieOkreslony /*Anormalny*/, Ablok) ;

#define UPDAT_BLOCK
#ifdef UPDAT_BLOCK
        if (alfab_version < verb4_1)
        {
            ADP = dane;
            ADK = dane + l -1;
            int ret=Export_Upgrade(fn, *Px, *Py, buf, block_type);
        }
#endif
    }

     return i_ret ;
   }
  else
    {

      strcpy(st, _FILE_NOT_FOUND_);

      strcat(st,fn1);
	  ErrListStr(st);
      return IO_RET_OPEN_FILE_ERR ;
    }
  error:
    
      dane += l ;
      dane_size -= l ;
      *adp = NULL ;
      *adk = NULL ;
      close(f);

      strcpy(st, _INCORRECT_FILE_FORMAT_);

      strcat(st,fn);
	  ErrListStr(st);
      delay(1000);
      return IO_RET_RW_ERR ;
    
}


void change_colors_dxf (char  *adr,char  *adrk, int kolor_elementu)
/*------------------------------------------------------------------*/
{
 NAGLOWEK *nag;
 char *ad;
 char *ad1;
 char typ2_1;

  obiekt_tok_all ((char *)adr, adrk, &ad,ONieOkreslony);
  while (ad!=NULL)
  {
     nag = (NAGLOWEK*)ad;
     if (nag->obiekt != OdBLOK)
     {
       if (((LINIA*)nag)->kolor==0 ) ((LINIA*)nag)->kolor=kolor_elementu;
     }
     obiekt_tok_all (NULL, adrk, &ad, ONieOkreslony);
  }
}


void change_types_dxf (char  *adr,char  *adrk, int typ_linii)
/*---------------------------------------------------------*/
{
 NAGLOWEK *nag;
 char *ad;
 char *ad1;
 char typ2_1;

  obiekt_tok_all ((char *)adr, adrk, &ad,ONieOkreslony);
  while (ad!=NULL)
  {
     nag = (NAGLOWEK*)ad;
     if (nag->obiekt != OdBLOK)
     {
       if (((LINIA*)nag)->typ==255) ((LINIA*)nag)->typ=typ_linii;
     }
     obiekt_tok_all (NULL, adrk, &ad, ONieOkreslony);
  }
}


void change_colors (char  *adr,char  *adrk, int kolor_elementu)
/*-----------------------------------------------------------*/
{
 NAGLOWEK *nag;
 char *ad;
 char *ad1;
 char typ2_1;

  obiekt_tok_all ((char *)adr, adrk, &ad,ONieOkreslony);
  while (ad!=NULL)
  {
     nag = (NAGLOWEK*)ad;
     if (nag->obiekt != OdBLOK)
     {
       switch (nag->obiekt )
        {
        /***********/
                case Otekst : switch (((TEXT*)nag)->typ)
                              {
                               case n_typ_symbol: ((TEXT*)nag)->kolor=kolorya.t_symbol;
                                                  /*dodatkowo zmiana wysokosci czcionki na 2.5*/
                                                  /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
                                                  // ((TEXT*)nag)->wysokosc=2.5;
                                                  // ((TEXT*)nag)->czcionka=13;
                                                  // na razie zrezygnowano
                                                  break;
                               case n_typ_typ:    ((TEXT*)nag)->kolor=kolorya.t_typ;
                                                  break;    
                               case n_typ_pin_name: ((TEXT*)nag)->kolor=kolorya.t_pin_name;
                                                  break;                                
                               case n_typ_zasilanie: ((TEXT*)nag)->kolor=kolorya.t_zasilanie;
                                                  break;                    
                               case n_typ_port: ((TEXT*)nag)->kolor=kolorya.t_port;
                                                  break;                   
                               case n_typ_schemat: ((TEXT*)nag)->kolor=kolorya.t_schemat;
                                                  break;                   
                               case n_typ_plik: ((TEXT*)nag)->kolor=kolorya.t_plik;
                                                  break;                   
                               case n_typ_siec: ((TEXT*)nag)->kolor=kolorya.t_siec;
                                                  break;                   
                               case n_typ_komentarz: ((TEXT*)nag)->kolor=kolorya.t_komentarz;
                                                  break;                         
                               case n_typ_zmienna: ((TEXT*)nag)->kolor=kolorya.t_zmienna;
                                                  break;                   
                               case n_typ_symbol_drabinki: ((TEXT*)nag)->kolor=kolorya.t_symbol_drabinki;
                                                  break;                   
                               case n_typ_opis_drabinki: ((TEXT*)nag)->kolor=kolorya.t_opis_drabinki;
                                                  break;                   
                              }
                     break; 
                case Opoint : ((LINIA*)nag)->kolor=kolorya.pin;
                     break;
                default: ((LINIA*)nag)->kolor=kolor_elementu;
                     break;
                     
        }       
     }

     obiekt_tok_all (NULL, adrk, &ad, ONieOkreslony);
  }
}


void change_color(char  *adr, char  *adrk, int kolor_elementu)
/*-----------------------------------------------------------*/
{
	NAGLOWEK *nag;
	char *ad;
	char *ad1;
	char typ2_1;

	obiekt_tok_all((char *)adr, adrk, &ad, ONieOkreslony);
	while (ad != NULL)
	{
		nag = (NAGLOWEK*)ad;
		if (nag->obiekt != OdBLOK)
		{	
		  if (nag->atrybut==Ablok) ((LINIA*)nag)->kolor = kolor_elementu;	
		}

		obiekt_tok_all(NULL, adrk, &ad, ONieOkreslony);
	}
}

void change_layer (char  *adr,char  *adrk, int new_layer)
/*-------------------------------------------------------------*/
{
 NAGLOWEK *nag;
 char *ad;

  obiekt_tok_all ((char *)adr, adrk, &ad,ONieOkreslony);
  while (ad!=NULL)
  {
     nag = (NAGLOWEK*)ad;
     if (nag->obiekt != OdBLOK)
     {
       ((LINIA*)nag)->warstwa = new_layer;
     }
     nag->blok = ElemBlok;
     obiekt_tok_all (NULL, adrk, &ad, ONieOkreslony);
  }
  ((NAGLOWEK*)adr)->blok = NoElemBlok ;
}

void normalize_layer (char  *adr,char  *adrk)
/*-----------------------------------------*/
{
 NAGLOWEK *nag;
 char *ad;
 int max_layer_number;
 int i;
 
 max_layer_number=0;
  
  obiekt_tok_all ((char *)adr, adrk, &ad,ONieOkreslony);
  while (ad!=NULL)
  {
     nag = (NAGLOWEK*)ad;
     if (nag->obiekt != OdBLOK)
     {
       if ((((LINIA*)nag)->warstwa > max_layer_number) && ((((LINIA*)nag)->warstwa)!=255))
         max_layer_number = ((LINIA*)nag)->warstwa;
     }
     nag->blok = ElemBlok;
     obiekt_tok_all (NULL, adrk, &ad, ONieOkreslony);
  }
  ((NAGLOWEK*)adr)->blok = NoElemBlok ;
  /*korekta warstw*/
  if ((max_layer_number + 1)>No_Layers)
  {
  for (i=No_Layers; i<=(max_layer_number+1); i++)
   {
    Layers [i].on    = 1;
    Layers [i].edit  = 1;
    Layers [i].point = 1;
    Layers [i].grey  = 0;
	Layers [i].bw    = 0;
	Layers [i].reserve = 0;
    Layers [i].color = 7; 
    Layers [i].line_type = 64; 
    Layers [i].len_name = 0;
    strcpy (Layers [i].name, "");
   }
   No_Layers=max_layer_number + 1;  
  } 
}


static void change_types_2_0_to_2_1 (char  *adr,char  *adrk)
/*-------------------------------------------------------------*/
{
 NAGLOWEK *nag;
 char *ad;
 char typ2_1;

  obiekt_tok_all ((char *)adr, adrk, &ad,ONieOkreslony);
  while (ad!=NULL)
  {
     nag = (NAGLOWEK*)ad;
     if (nag->obiekt != OdBLOK)
     {
       switch (nag->obiekt )
        {
         case Olinia :
         case Oluk   :
         case Ookrag : typ2_1=typ_linii2_0_to_2_1(((LINIA*)nag)->typ);
                       ((LINIA*)nag)->typ=typ2_1;  
	               break ;
         default     :
	               break ;
        }       
     }
     nag->blok = ElemBlok;
     obiekt_tok_all (NULL, adrk, &ad, ONieOkreslony);
  }
}


static void change_texts_2_1_to_3_0 (char *adp, char *adk)
/*-------------------------------------------------------*/
{
  NAGLOWEK *nag ;
  long_long block_size ;
  BOOL b_ret ;
  char typ2_1;
  BLOK *adb;
  char  *adh;
  TEXT3 *old_text;
  TEXT3 new_text;
  char  *adr;
  char *ad;
  int ii, i_t, d_dl;
  unsigned char znacznik1, znacznik2;

  ad = adp;
  block_size= adk - adp;
  nag = (NAGLOWEK*) ad ;
  while ((ad < adk) && (nag->obiekt!=Okoniec))
  {
    switch (nag->obiekt)
    {
      case Otekst : //zmiana tekstow
                     
		              old_text=(TEXT3 *) ad;
					  //korekta tekstu
				      memmove(&new_text, old_text, sizeof(TEXT3));
                      strcpy(new_text.text,"");
					  new_text.multiline=0;
					  new_text.spacing=6;
					  new_text.underline = 0;
					  new_text.encoding = 0;
					  new_text.n += 2;
					  i_t=0;
					  ii=0;
					  znacznik1 = ((old_text->dl >> 8) & 0xFF);
					  old_text->dl = (old_text->dl & 0xFF) ;
                      if (old_text->dl>0)
					  {
						new_text.text[0]=znacznik1;
						
						i_t=1;
						ii=1;
					  }
					  if (old_text->dl>1)
					  {
						znacznik2 = old_text->multiline + old_text->underline*2 + old_text->encoding*4+ old_text->spacing*8;
						new_text.text[1]=znacznik2;
						
						i_t=2;
						ii=2;
					  }
					  if (old_text->dl>2)
					  {
						d_dl=old_text->dl;
						for (ii=i_t; ii<d_dl; ii++)
						{
							new_text.text[ii]=old_text->text[ii-i_t];
						}
					    
					  }

					  new_text.text[ii]='\0';

					  block_size += 2;  //zawsze dodaje sie dwa bajty
					  
                      ad = korekta_obiekt (ad, &new_text);
					  ad +=( sizeof(NAGLOWEK) + new_text.n) ;

					 
	            break ;
      case OdBLOK :  adb = (BLOK *) ad;
		             ad += (sizeof(NAGLOWEK)+B3+adb->dlugosc_opisu_obiektu);
           	    break ;        
      default     :  ad += (sizeof(NAGLOWEK) + ((NAGLOWEK*) ad)->n);
	            break ;
    }
    adk = dane + block_size;
	nag = (NAGLOWEK*) ad ;
  }

  adp=dane;
  adk=ad;

}


static void change_layer1_1 (char  *adr,char  *adrk, int new_layer)
/*-------------------------------------------------------------*/
{
 NAGLOWEK1_1 *nag;
 char *ad;

  obiekt_tok_all1_1 ((char *)adr, adrk, &ad,ONieOkreslony);
  while (ad!=NULL)
  {
     nag = (NAGLOWEK1_1*)ad;
     if (nag->obiekt != OdBLOK)
     {
       ((LINIA1_1*)nag)->warstwa = new_layer;
     }
     nag->blok = ElemBlok;
     obiekt_tok_all1_1 (NULL, adrk, &ad, ONieOkreslony);
  }
  ((NAGLOWEK1_1*)adr)->blok = NoElemBlok ;
}

#undef __O_ZB__
