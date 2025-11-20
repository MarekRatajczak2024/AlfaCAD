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

#define __DXF_O__
#pragma warning(disable : 4996)
#include "forwin.h"
#include <stdio.h>
#ifdef LINUX
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "bib_e.h"
#include "b_dxf_o.hpp"
#include "rysuj_e.h"
#include "alffont.h"
#include "o_lfile.h"
#include "o_loadf.h"

#include "alfversion.h"

#include "forlinux.h"

#include "message.h"

#include "leak_detector_cpp.hpp"

#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

#define TBYLAYER 17  /*wymaga uzgodnienia*/
#define CBYLAYER 18  /*wymaga uzgodnienia*/

#define MAXLINE 1024 //255
INS_BLOCK ins_b;

#define w1_5 1.5

extern int AcadV;   /*prowizorka*/
static BOOL to_main = TRUE;
static BOOL to_block = FALSE;
static BOOL to_entities = FALSE;
char *STANDARD=(char*)"STANDARD";

typedef unsigned long       DWORD;

char code999[5][8] = { "dimline", "dimarc", "dimlext", "dimhead", "dimtext" };

extern "C" {
extern void GetTextLen (TEXT *t, double *tdl) ;
extern void Get_Limits (long off, long offk, int atrybut,
	double *xmin, double *xmax, double *ymin, double *ymax) ;
extern int my_file_exists(char *name);
extern int32_t utf8_to_ucs2(const uint8_t * input, const uint8_t ** end_ptr);
extern int dxf_encoding;
extern int utf82unicode(unsigned char *utf8text, unsigned char *unicodetext);
extern void unicode2windxf(char *unicodetext, char *wintext, int count);
extern int get_fonts_for_dxf(unsigned char *charfont_numbers);
void utf82none(char * tekst);
void normalize_file_name(char* tekst);
extern int fnsplit(const char *path, char *drive, char *dir, char *name, char *ext);
extern DWORD RunSilent(char* strFunct, char* strstrParams);
extern void Rotate_Point(double si, double co,
	double x1, double y1,  			    /*center point*/
	double x2, double y2, double *x, double *y); /*rotate point*/
extern int get_palette_color(int color);

extern void utf8Upper(char* text);
extern double Get_Point_Size(void);
extern BOOL Check_if_Equal (double x, double y);
extern double Angle_Normal (double angle);
extern void Elliptical_Arc_Ends (void *ea, double *x1, double *y1, double *x2, double *y2);
extern void SolidArc_To_Points (SOLIDARC *sa, double *x1, double *y1, double *x2, double *y2);
extern double normalize_txt_angle(double angle0);
extern void normalize_txt(TEXT *ptrs_text);
extern void set_decimal_format(char *text, double l, double precision);
extern void create_solid_on_line (LINIA *L, WIELOKAT *s, double width1, double width2, double axis);
extern void Normalize_Solid (WIELOKAT *ptr_w);
extern double measure_vector (float x1, float y1, float x2, float y2, BOOL b_first_end, double df_l0, double df_dl, double *df_x, double *df_y);
extern point intersectionPoint(point a1,point a2,point b1,point b2);
extern BOOL Check_if_GT (double x, double y);
extern double Get_TextLen (TEXT *ptrs_text, char *ptrsz_t) ;
extern void set_global_hidden_blocks_visibility(BOOL block_visibility);

extern char *load_symbol[];

extern double dim_precision;
extern double thermal_precision;
extern double force_precision;
extern double moment_precision;
extern double displacement_precision;
extern double rotation_precision;
extern double load_precision;

extern double depth_magnitude; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double radius_magnitude; //units per mm  default 1 Celsius per 1 mm on drawing paper
extern double thermal_magnitude; //units per mm  default 1 Celsius per 1 mm on drawing paper
extern double load_magnitude; //units per mm  default 10kN/m load per 1 mm on drawing paper
extern double flood_magnitude; //units per mm  default 10kN/m² load per 1 mm on drawing paper
extern double shear_magnitude; //units per mm  default 10kN/m shear/reaction per 1 mm on drawing paper
extern double force_magnitude; //units per mm  default 10kN force per 1 mm on drawing paper
extern double moment_magnitude; //units per mm  default 10kNm force per 1 mm radius on drawing paper
extern double displacement_magnitude; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
extern double rotation_magnitude;

}

extern BOOL PIN_IS_PRINTABLE;

extern void mazovia_cyr2acad_cyr (char * tekst) ;
extern int get_width370(unsigned char e_type);
extern BOOL get_width_invisible(unsigned char e_type);
extern void line_widths_dxf(void);

void utf82ISOlatin(char * tekst);
void utf82latin(char * tekst);
void utf82acad(char * tekst);
void encodingdxf(char *text);
void encodingdxf_layer(char *text);

extern int utf82unicode(unsigned char *utf8text, unsigned char *unicodetext);
extern void unicode2win(char *unicodetext, char *wintext, int count);


#define FIRSTHANDLE 48

static int handle_hex= FIRSTHANDLE;
static int image_no = 16;
static int image_no_ = 16;
static   char *Path;
static BOOL EXPORT_TRANSLUCEN_DISC=FALSE;

enum KG { INT,FLOAT,STRING,KOMENTARZ,KODNIEZNANY };
#define MaxLen 100

#define SECTION  "SECTION"
#define HEADER	 "HEADER"
#define CLASSES	 "CLASSES"
#define TABLES	 "TABLES"
#define OBJECTS	 "OBJECTS"
#define DICTIONARY "DICTIONARY"
#define BLOCKS	 "BLOCKS"
#define ENTITIES "ENTITIES"
#define ENDSEC	 "ENDSEC"
#define TABLE	 "TABLE"
#define VPORT	 "VPORT"
#define ENDTAB	 "ENDTAB"
#define LTYPE	 "LTYPE"
#define LAYER	 "LAYER"
#define STYLE	 "STYLE"
#define VIEW	 "VIEW"
#define UCS	     "UCS"
#define APPID	 "APPID"
#define DIMSTYLE "DIMSTYLE"
#define BLOCK_RECORD "BLOCK_RECORD"
#define DWGMGR	 "DWGMGR"
#define ENDTAB	 "ENDTAB"
#define LINE     "LINE"
#define CIRCLE   "CIRCLE"
#define ELLIPSEDXF   "ELLIPSE"
#define HATCH   "HATCH"
#define ARC      "ARC"
#define SOLID    "SOLID"
#define BSPLINE  "SPLINE"
#define POLYLINE "POLYLINE"
#define LWPOLYLINE "LWPOLYLINE"
#define POINT    "POINT"
#define IMAGEDEF "IMAGEDEF"
#define IMAGE    "IMAGE"
#define VERTEX   "VERTEX"
#define SEQEND   "SEQEND"
#define INSERT   "INSERT"
#define DIMENSION "DIMENSION"
#define BLOCK    "BLOCK"
#define ENDBLK   "ENDBLK"


char *strlwr_(char *s) {
    char *tmp = s;

    for (; *tmp; ++tmp) {
        *tmp = tolower((unsigned char) *tmp);
    }

    return s;
}

char *strupr_(char *s) {
    char *tmp = s;

    for (; *tmp; ++tmp) {
        *tmp = toupper((unsigned char) *tmp);
    }

    return s;
}


static int typ_linii(int grubosc_typ)
/*---------------------------------*/
{ 
 return (grubosc_typ & 31);
}

static int grubosc_linii(int grubosc_typ)
/*-------------------------------------*/
{
	return (grubosc_typ & 224) / 32;
}

/*---------------------------------------------------------------*/
static int color_dxf_old (int i_color)
/*-------------------------------*/
{
  int i_acad_no ;

  i_acad_no = i_color ;
  if (i_color == 0)
  {
    i_acad_no = 15 ;
  }
  return i_acad_no ;
}





static int color_dxf (int i_color)
/*-------------------------------*/
{ int i_acad_no ;
  static int tab_alf2acad[]={  0,  1,  2,  3,  4,  5,  6,  7, 8 /*251*/, 14, 34, 94,134,172,214,  9,  0,
                               51, 51, 51, 51, 51, 51, 50, 50, 40, 40, 31, 31, 43, 43,233,233,
                               233,233,243,243,243, 21, 21, 32, 30, 30, 23, 11,223,241, 41,211,
                               211,221,220,230,230,240,240, 20, 20,240,230,230,232,220,210,211,
                               200,205,214,222,224,232,242, 12, 22, 24,244,232,223,212,213,200,
                               191,213,223,223, 13,243, 33, 32, 42, 42, 42, 33, 43,193,193,193,
                               141,141, 53, 53, 53, 55, 55, 52, 52, 60, 61, 61, 71,113,113,133,
                               142,142,110,101, 93, 81, 81, 91, 62, 62, 63,103,113,133,133,141,
                               161,143,153,126, 27, 37, 45, 65, 44, 44, 25, 17,193,203,183,163,
                               181,185,195,202,226,235, 15, 24, 34,234,236,224,206,224,195,192,
                               170,182,175,184,186,187, 17, 16, 38, 35,247,177,175,194,190,180,
                               171,152,144,155,126, 97, 86, 46, 64, 75,105,115,125,134,153,143,
                               140,133,113,113, 83,113,102, 72, 70, 90,110,113,101,111,120,131,
                               130,120,111,101, 91, 91,101, 82, 74, 95,104,105,125,132,142,142,
                               150,152,144,144,118,107,125,132,  7,168,176,156,174,154,162,160,
                               250, 19, 29,251,252,  8,  8,253,253,253,254,254,254,254,255,  7,};

  i_acad_no = tab_alf2acad[i_color] ;
  return i_acad_no ;
}


static char * layer_dxf(char *ptrsz_name, int i_no)
{
	static char sz_buf[MaxLenNameLayer+1]; 	/*bib_e.h*/
	char *text_ptr;
    int ret;

	if (i_no == 0)
	{
		strcpy(sz_buf, "0");
	}

	else if (strlen(ptrsz_name) == 0)  /*pusta nazwa*/
	{
        ret = snprintf(sz_buf, 10, "%d",i_no);
	}
	else
	{

        
		strncpy(sz_buf, ptrsz_name, MaxLenNameLayer);
		sz_buf[MaxLenNameLayer] = '\0';
		text_ptr = sz_buf;

		encodingdxf_layer(text_ptr);

		text_ptr[MaxLenNameLayerDXF] = '\0';

	}
	return sz_buf;
}

static char * layer_dxf_old (char *ptrsz_name, int i_no)
/*-------------------------------------------------*/
{
  static char sz_buf [MaxLenNameLine] ; 	/*bib_e.h*/
  unsigned char *ptrsz_tmp ;
  int ret;

  if (i_no == 0)
  {
    strcpy (sz_buf, "0") ;
  }
  else if (i_no == 254)  //for Aster needs
   {
     strcpy (sz_buf, "0") ;
   }
  else
  {
   if (strlen(ptrsz_name)==0)  /*empty name*/
   {
       ret = snprintf(sz_buf, 10, "%d",i_no);
   }
    else
   {
    strncpy (sz_buf, ptrsz_name, MaxLenNameLine) ;
    sz_buf [MaxLenNameLine - 1] = '\0' ;
    for (ptrsz_tmp = (unsigned char *)sz_buf; *ptrsz_tmp != '\0'; ptrsz_tmp++)
    {
      if ((*ptrsz_tmp < 48) && (*ptrsz_tmp != 36) && (*ptrsz_tmp != 45))
       {
         *ptrsz_tmp = '_' ; 
       }
      else 
       if ((*ptrsz_tmp > 57) && (*ptrsz_tmp < 65))
       {
         *ptrsz_tmp = '_' ;
       }
      else 
       if ((*ptrsz_tmp > 90) && (*ptrsz_tmp < 95))
       {
         *ptrsz_tmp = '_' ;
       } 
      switch (*ptrsz_tmp)
      {
        case 96   : *ptrsz_tmp = '_' ; break ;
	case a_PL : *ptrsz_tmp = 'A' ; break ;
	case c_PL : *ptrsz_tmp = 'C' ; break ;
	case e_PL : *ptrsz_tmp = 'E' ; break ;
	case l_PL : *ptrsz_tmp = 'L' ; break ;
	case n_PL : *ptrsz_tmp = 'N' ; break ;
	case o_PL : *ptrsz_tmp = 'O' ; break ;
	case s_PL : *ptrsz_tmp = 'S' ; break ;
	case z_PL : *ptrsz_tmp = 'Z' ; break ;
	case x_PL : *ptrsz_tmp = 'Z' ; break ;
	case A_PL : *ptrsz_tmp = 'A' ; break ;
	case C_PL : *ptrsz_tmp = 'C' ; break ;
	case E_PL : *ptrsz_tmp = 'E' ; break ;
	case L_PL : *ptrsz_tmp = 'L' ; break ;
	case N_PL : *ptrsz_tmp = 'N' ; break ;
	case O_PL : *ptrsz_tmp = 'O' ; break ;
	case S_PL : *ptrsz_tmp = 'S' ; break ;
	case Z_PL : *ptrsz_tmp = 'Z' ; break ;
	case X_PL : *ptrsz_tmp = 'Z' ; break ;
      }
      if (*ptrsz_tmp > 122) 
       {
         *ptrsz_tmp = '_' ;
       } 
    }
   }
  }
  strupr_ (sz_buf) ;
  return sz_buf;
}

void replace_U00B2(char *text)
{
	char *ptr;
	char tail[MaxMultitextLen];

	ptr = strstr(text, "\302\262");
	while (ptr!=NULL)
	{
		strcpy(tail, ptr + 2);
		strcpy(ptr, "\\U+00B2");
		strcat(ptr + 2, tail);
		ptr += 7;
		
        ptr = strstr(ptr, "\302\262");
	}

}

void encodingdxf(char *text)
{
	int count;
	char unicodetext[MaxMultitextLen * 2] = "";
	
	replace_U00B2(text);

	return;
	
	count = utf82unicode((unsigned char *)text, (unsigned char *)&unicodetext);
	unicode2windxf((char *)&unicodetext, text, count);

}

void encodingdxf_layer(char *text)
{
	char unicodetext[MaxMultitextLen * 2] = "";
	int i;
	int len_i;
	char legal_chars[80] = " 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-";
	unsigned char c;

	replace_U00B2(text);


	len_i = (int)strlen(text);

	for (i = 0; i < len_i; i++)
	{
		c = text[i];
		if ((c < 127) && (strchr(legal_chars, c) == NULL)) text[i] = '_';
	}
	
}
/*------  class PISZ_GRUPA  -------------------------------------*/

//class PISZ_GRUPA
//  public :
//   PISZ_GRUPA(void);
//   PiszGrupa(int kod, int n);
//   PiszGrupa(int kod, double f);
//   PiszGrupa(int kod, char *str);

PISZ_GRUPA::PISZ_GRUPA(void)
{}

int PISZ_GRUPA::PiszGrupa(int kod, int n)
{
	if ((kod == 5) || (kod == 105))
	{
		//return OkWeWy;
		char handle_str[16];
		sprintf(handle_str, "%X", handle_hex++);
		if (PiszKodGrupy(kod) == ErrorWeWy || PiszString(handle_str) == ErrorWeWy) return ErrorWeWy;
		else return OkWeWy;
	}
	if (kod == -5)
	{
		char handle_str[16];
		sprintf(handle_str, "%X", n);
		if (PiszKodGrupy(-kod) == ErrorWeWy || PiszString(handle_str) == ErrorWeWy) return ErrorWeWy;
		else return OkWeWy;
	}
	else if (kod == 340)
	{
		char handle_str[16];
		sprintf(handle_str, "%X", n);
		if (PiszKodGrupy(kod) == ErrorWeWy || PiszString(handle_str) == ErrorWeWy) return ErrorWeWy;
		else return OkWeWy;
	}
	else
	{
		if (PiszKodGrupy(kod) == ErrorWeWy || PiszInt(n) == ErrorWeWy) return ErrorWeWy;
		else return OkWeWy;
	}
}

int PISZ_GRUPA::PiszGrupa(int kod, double f)
{

  /************************** 2.6 ****************************/
  if ( AcadV == ACADV26)
  {
    if (kod == 30 || kod == 31 || kod == 32 || kod == 33) return OkWeWy;
  }
  /**************************************************************/

  if(PiszKodGrupy(kod)==ErrorWeWy || PiszFloat(f)==ErrorWeWy ) return ErrorWeWy;
  else return OkWeWy;
}

int PISZ_GRUPA::PiszGrupa(int kod, char *str)
{
  if(PiszKodGrupy(kod)==ErrorWeWy || PiszString(str)==ErrorWeWy ) return ErrorWeWy;
  else return OkWeWy;
}

/*--------------------------------------------*/

/*----------  class PISZ_HEADER  -----------------------------*/

//class PISZ_HEADER : virtual public PISZ_GRUPA
//  public :
//   PISZ_HEADER(void);
//   int PiszHeader(void);

PISZ_HEADER::PISZ_HEADER(void)
{ }

int PISZ_HEADER::PiszHeader(void)
{ if (PiszGrupa(999, (char*)_ALFACAD_) == ErrorWeWy ||
     PiszGrupa(0,(char*)SECTION)==ErrorWeWy ||
     PiszGrupa(2,(char*)HEADER)==ErrorWeWy  ||
     PiszHeader0()==ErrorWeWy	     ||
     ChangeHeader()==ErrorWeWy	     ||
     PiszGrupa(0,(char*)ENDSEC)==ErrorWeWy ) return ErrorWeWy;
  else return OkWeWy;
}

/*------------------------------------------------------------*/


/*-------  class PISZ_TABLES  -------------------------------------*/

//class PISZ_TABLES : virtual public PISZ_GRUPA
// private :
//   virtual int PiszVport(void);
//   virtual int PiszLtype();
//   virtual int PiszLayer();
//   virtual int PiszStyle();
//   virtual int PiszView();
//   virtual int PiszUcs();
//   virtual int PiszDwgMgr();
//  public :
//   PISZ_TABLES(void);
//   int PiszTables(void);

PISZ_CLASSES::PISZ_CLASSES(void)
{}

int PISZ_CLASSES::PiszClasses(void)
{
	if (PiszGrupa(0, (char*)SECTION) == ErrorWeWy ||
		PiszGrupa(2, (char*)CLASSES) == ErrorWeWy ||
		//PiszGrupa(5, handle_hex) == ErrorWeWy ||
		//PiszImageDef() == ErrorWeWy ||
		PiszGrupa(0, (char*)ENDSEC) == ErrorWeWy) return ErrorWeWy;
	else return OkWeWy;
}

PISZ_OBJECTSDEF::PISZ_OBJECTSDEF(void)
{}

int PISZ_OBJECTSDEF::PiszObjects(void)
{
	if (PiszGrupa(0, (char*)SECTION) == ErrorWeWy ||
		PiszGrupa(2, (char*)OBJECTS) == ErrorWeWy ||
		PiszGrupa(0, (char*)DICTIONARY) == ErrorWeWy ||
		PiszGrupa(5, (char*)"C") == ErrorWeWy ||
		PiszGrupa(330, 0) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbDictionary") ||
		PiszGrupa(281, 1) == ErrorWeWy ||
		PiszGrupa(3, (char*)"ACAD_GROUP") ||
		PiszGrupa(350, (char*)"D") == ErrorWeWy ||
		PiszGrupa(0, (char*)DICTIONARY) == ErrorWeWy ||
		PiszGrupa(5, (char*)"D") == ErrorWeWy ||
		PiszGrupa(330, (char*)"C") == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbDictionary") == ErrorWeWy ||
		PiszGrupa(281, 1) == ErrorWeWy ||
		PiszImageDef() == ErrorWeWy)  return ErrorWeWy;
		//PiszGrupa(0, ENDSEC) == ErrorWeWy) return ErrorWeWy;
	else return OkWeWy;
}

int PISZ_OBJECTSDEF::PiszImageDef()
{
	char image_file[MAXPATH], image_file_pcx[MAXPATH];
	char image_name[MAXPATH];
	char dir__[MAXPATH]="";
	char drive__[MAXDRIVE]="";
	char file__[MAXFILE]="";
	char ext__[MAXEXT]="";
	int flags;
	char ext_[5], ext_1[5];
	FILE *fp;
	char st[MaxTextLen];
	char *png;
	B_PCX *b_pcx;
    int ret;

	NAGLOWEK *nag;
	char *adh;

	char params[128];
	DWORD runcode;

	adh = dane;
	adh += dane_size;
	obiekt_tok_all(dane, adh, (char **)&nag, ONieOkreslony);
	while (nag != NULL)
	{
		switch (nag->obiekt)
		{
		case OdBLOK:
			break;
		case Opcx:
			b_pcx = (B_PCX *)nag;

			if ((b_pcx->atrybut == Ausuniety) || (b_pcx->atrybut == Abad)) break;
			//firstly, image must be exported to the file
		    //image file takes name from dxf file name indexed at the end follow the pattern  %Path
			flags = fnsplit(Path, drive__, dir__, file__, ext__);
			if (b_pcx->kod_obiektu == 1)
			{
				strcpy(ext_, ".pcx");
				strcpy(ext_1, ".png");
				sprintf(image_file_pcx, "%s%s%s_img%d%s", drive__, dir__, file__, image_no, ext_);
				sprintf(image_file, "%s%s%s_img%d%s", drive__, dir__, file__, image_no, ext_1);
				sprintf(image_name, "%s_img%d%s", file__, image_no, ext_1);
				//export bloku PCX
				fp = fopen(image_file_pcx, "w+b");
				if (!fp)
				{
					strcpy(st, _PCX_FILE_ERROR_);
					ErrListStr(st);
					return ErrorWeWy;
				}
				if (b_pcx->markers == 1)
				{
					fwrite(&b_pcx->pcx, b_pcx->n - T294 - sizeof(STRIPS), 1, fp);
				}
				else
				{
					fwrite(&b_pcx->pcx, b_pcx->n - T294, 1, fp);
				}
				fclose(fp);
				//converting to png
				sprintf(params, "%s %s", image_file_pcx, image_file);
				runcode = RunSilent((char*)"image2png.exe", params);
				if (runcode > 0)
				{
					return ErrorWeWy;
				}
				if (my_file_exists(image_file_pcx)) ret=unlink(image_file_pcx);

			}
			else if (b_pcx->kod_obiektu == 2)
			{
				strcpy(ext_, ".png");
				sprintf(image_file, "%s%s%s_img%d%s", drive__, dir__, file__, image_no, ext_);
				sprintf(image_name, "%s_img%d%s", file__, image_no, ext_);
				//export bloku PNG
				png = b_pcx->pcx + sizeof(PCXheader);

				fp = fopen(image_file, "w+b");
				if (!fp)
				{
					strcpy(st, _PNG_FILE_ERROR_);
					ErrListStr(st);
					return ErrorWeWy;
				}
				if (b_pcx->markers == 1)
				{
					fwrite(png, b_pcx->n - T294 - sizeof(PCXheader) /*- sizeof(STRIPS)*/, 1, fp);
				}
				else
				{
					fwrite(png, b_pcx->n - T294 - sizeof(PCXheader), 1, fp);
				}
				fclose(fp);
			}
			else
			{
				strcpy(st, _UNKNOWN_IMAGE_FORMAT_);
				return ErrorWeWy;
			}

			image_no++;

			if (PiszGrupa(0, (char*)IMAGEDEF) == ErrorWeWy ||
				PiszGrupa(-5, image_no) == ErrorWeWy ||
			    PiszGrupa(100, (char*)"AcDbRasterImageDef") == ErrorWeWy) return ErrorWeWy;
			if (PiszGrupa(1, image_name) == ErrorWeWy) return ErrorWeWy;
			if (PiszGrupa(10, (int)(b_pcx->i_x)) == ErrorWeWy ||
				PiszGrupa(20, (int)(b_pcx->i_y)) == ErrorWeWy ||
				PiszGrupa(11, (double)(b_pcx->dx*0.1)) == ErrorWeWy ||   //in 0.1 cm
				PiszGrupa(21, (double)(b_pcx->dy*0.1)) == ErrorWeWy) return ErrorWeWy;  //in 0.1 cm
			if (PiszGrupa(280, (int)1) == ErrorWeWy) return ErrorWeWy;
			if (PiszGrupa(281, (int)2) == ErrorWeWy) return ErrorWeWy;

			break;
		default:
			break;

		}
		obiekt_tok_all(NULL, adh, (char **)&nag, ONieOkreslony);
	}
	return OkWeWy;
}

PISZ_TABLES::PISZ_TABLES(void)
{}

int PISZ_TABLES::PiszTables(void)
{ if(PiszGrupa(0,(char*)SECTION)==ErrorWeWy ||
     PiszGrupa(2,(char*)TABLES)==ErrorWeWy  ||
	 //PiszGrupa(5, handle_hex) == ErrorWeWy ||
     PiszVport()==ErrorWeWy ||
     PiszLtype()==ErrorWeWy ||
     PiszLayer()==ErrorWeWy ||
     PiszStyle()==ErrorWeWy ||
     PiszView()==ErrorWeWy ||
     PiszUcs()==ErrorWeWy ||
     PiszAppid()==ErrorWeWy ||
	 PiszDimstyle() == ErrorWeWy ||
	 PiszBlockRecord() == ErrorWeWy) return ErrorWeWy;
  else return OkWeWy;
}


int PISZ_TABLES::PiszLtype()
{ int i,j;
  if(PiszGrupa(0,(char*)TABLE)==ErrorWeWy ||
     PiszGrupa(2,(char*)LTYPE)==ErrorWeWy  ||
	 PiszGrupa(5, handle_hex) == ErrorWeWy ||
	 PiszGrupa(100, (char*)"AcDbSymbolTable") == ErrorWeWy ||
     PiszGrupa(70, LiczbaWzorcowLinii+2)==ErrorWeWy) return ErrorWeWy;

  if (PiszGrupa(0, (char*)LTYPE) == ErrorWeWy ||
	  PiszGrupa(5, handle_hex) == ErrorWeWy ||
	  PiszGrupa(100, (char*)"AcDbSymbolTableRecord") == ErrorWeWy ||
	  PiszGrupa(100, (char*)"AcDbLinetypeTableRecord") == ErrorWeWy ||
	  PiszGrupa(2, (char*)"ByBlock") == ErrorWeWy ||
	  PiszGrupa(70, 0) == ErrorWeWy ||  //PiszGrupa(70,64)==ErrorWeWy ||
	  PiszGrupa(3, (char*)"") == ErrorWeWy ||  //PiszGrupa(3," ")==ErrorWeWy ||
	  PiszGrupa(72, 65) == ErrorWeWy ||
	  PiszGrupa(73, 0) == ErrorWeWy ||
	  PiszGrupa(40, 0) == ErrorWeWy) return ErrorWeWy;

  if (PiszGrupa(0, (char*)LTYPE) == ErrorWeWy ||
	  PiszGrupa(5, handle_hex) == ErrorWeWy ||
	  PiszGrupa(100, (char*)"AcDbSymbolTableRecord") == ErrorWeWy ||
	  PiszGrupa(100, (char*)"AcDbLinetypeTableRecord") == ErrorWeWy ||
	  PiszGrupa(2, (char*)"ByLayer") == ErrorWeWy ||
	  PiszGrupa(70, 0) == ErrorWeWy ||  //PiszGrupa(70,64)==ErrorWeWy ||
	  PiszGrupa(3, (char*)"") == ErrorWeWy ||  //PiszGrupa(3," ")==ErrorWeWy ||
	  PiszGrupa(72, 65) == ErrorWeWy ||
	  PiszGrupa(73, 0) == ErrorWeWy ||
	  PiszGrupa(40, 0) == ErrorWeWy) return ErrorWeWy;

  for(i=0;i<LiczbaWzorcowLinii;i++)
   { if(PiszGrupa(0,(char*)LTYPE)==ErrorWeWy ||
	PiszGrupa(5, handle_hex) == ErrorWeWy ||
    PiszGrupa(100, (char*)"AcDbSymbolTableRecord")==ErrorWeWy  ||
	PiszGrupa(100, (char*)"AcDbLinetypeTableRecord") == ErrorWeWy ||
	PiszGrupa(2, TypyLinii[i].nazwa) == ErrorWeWy ||
	PiszGrupa(70,0)==ErrorWeWy ||  //PiszGrupa(70,64)==ErrorWeWy ||
	PiszGrupa(3,TypyLinii[i].nazwa)==ErrorWeWy ||  //PiszGrupa(3," ")==ErrorWeWy ||
	PiszGrupa(72,65)==ErrorWeWy ||
	PiszGrupa(73,TypyLinii[i].IloscElementow)==ErrorWeWy ||
	PiszGrupa(40,TypyLinii[i].dlwzorca)==ErrorWeWy ) return ErrorWeWy;
     for(j=0;j<TypyLinii[i].IloscElementow;j++)
      if(PiszGrupa(49,TypyLinii[i].kreski[j])==ErrorWeWy ||
		  PiszGrupa(74, 0) == ErrorWeWy) return ErrorWeWy;
   }
  if(PiszGrupa(0,(char*)ENDTAB)==ErrorWeWy) return ErrorWeWy;
  else return OkWeWy;
}

int PISZ_TABLES::PiszLayer()
{ int i;  //, i_line_type;
  int l_on;
  int l_flag;
  if(PiszGrupa(0,(char*)TABLE)==ErrorWeWy ||
     PiszGrupa(2,(char*)LAYER)==ErrorWeWy  ||
	 PiszGrupa(5, handle_hex) == ErrorWeWy ||
	 PiszGrupa(100, (char*)"AcDbSymbolTable") == ErrorWeWy ||
     PiszGrupa(70, No_Layers)==ErrorWeWy) return ErrorWeWy;
  for(i = 0 ; i < No_Layers ; i++)
   {
	  l_flag = 0;
	  if (Layers[i].on == 0) l_on = -1; else l_on = 1;
	  if (Layers[i].edit == 0) l_flag+=4;
	if(PiszGrupa(0,(char*)LAYER)==ErrorWeWy ||
	PiszGrupa(5, handle_hex) == ErrorWeWy ||
    PiszGrupa(330, (char*)"2") == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbSymbolTableRecord") == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbLayerTableRecord") == ErrorWeWy ||
	PiszGrupa(2, layer_dxf (Layers[i].name, i))==ErrorWeWy  ||
	PiszGrupa(70,l_flag)==ErrorWeWy ||  /*nie zamrozona (65-zamrozona)*/
	PiszGrupa(62,(int)(color_dxf(Layers[i].color)*l_on))==ErrorWeWy ||
	PiszGrupa(6,TypyLinii[Layers[i].line_type & 31].nazwa) == ErrorWeWy ||
    PiszGrupa(370, (int)get_width370((Layers[i].line_type & 224) / 32)) == ErrorWeWy ||
	PiszGrupa(390, (char*)"F") == ErrorWeWy) return ErrorWeWy;
   }
  if(PiszGrupa(0,(char*)ENDTAB)==ErrorWeWy) return ErrorWeWy;
  else return OkWeWy;
}

int PISZ_TABLES::PiszStyle ()
/*-------------------------*/
{
  int i, i_text_styleno ;
  unsigned char charfont_numbers[MaxNumberTextStyle];
  int ret;

  i_text_styleno  = 0 ;
  for(i = 0; i < WhNumberTextStyle; i++)
  {
    if (NULL != PTRS__Text_Style[i] &&
    '\0' != PTRS__Text_Style[i]->font_acad_name [0])
    {
      i_text_styleno++ ;
    }
  }
  if(PiszGrupa(0,(char*)TABLE)==ErrorWeWy ||
     PiszGrupa(2,(char*)STYLE)==ErrorWeWy  ||
	 PiszGrupa(5, handle_hex) == ErrorWeWy ||
	 PiszGrupa(100, (char*)"AcDbSymbolTable") == ErrorWeWy ||
     PiszGrupa(70, i_text_styleno)==ErrorWeWy) return ErrorWeWy;

  //checking styles
  ret = get_fonts_for_dxf(&charfont_numbers[0]);
  
  for (i = 0 ; i < WhNumberTextStyle; i++)
  {
    if ((NULL == PTRS__Text_Style[i]) ||
	('\0' == PTRS__Text_Style[i]->font_acad_name [0]) ||
	(charfont_numbers[i]==0))
    {
      continue ;
    }
    if(PiszGrupa(0,(char*)STYLE)==ErrorWeWy ||
	PiszGrupa(5, handle_hex) == ErrorWeWy ||
    PiszGrupa(100, (char*)"AcDbSymbolTableRecord") == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbTextStyleTableRecord") == ErrorWeWy ||
	PiszGrupa(2, PTRS__Text_Style[i]->font_name) == ErrorWeWy ||
	PiszGrupa(70,0)==ErrorWeWy ||
	PiszGrupa(40,0.0)==ErrorWeWy ||
	PiszGrupa(41, s__Fonts_Style[i].df_width_factor) == ErrorWeWy ||
	PiszGrupa(50,0.0)==ErrorWeWy ||
	PiszGrupa(71,0)==ErrorWeWy ||
	PiszGrupa(42, TextG.wysokosc)==ErrorWeWy ) return ErrorWeWy;
   if ((dxf_encoding==1250) && ((strcmp(PTRS__Text_Style[i]->font_acad_name,"CYRILLIC")==0) ||
                           (strcmp(PTRS__Text_Style[i]->font_acad_name,"CIRYLLIC")==0)) )
     {
       if (PiszGrupa(3, (char*)"")==ErrorWeWy ||
  	        PiszGrupa(4, (char*)"Courier New Cyr")==ErrorWeWy) return ErrorWeWy;
     }
     else
      {
       if (PiszGrupa(3,PTRS__Text_Style[i]->font_acad_name)==ErrorWeWy ||
	        PiszGrupa(4, (char*)"")==ErrorWeWy ) return ErrorWeWy;
      }
   }
  if(PiszGrupa(0,(char*)ENDTAB)==ErrorWeWy) return ErrorWeWy;
  else return OkWeWy;
}

int PISZ_TABLES::PiszViewPorts()
{

	if (PiszGrupa(0, (char*)VPORT) == ErrorWeWy ||
		PiszGrupa(5, handle_hex) == ErrorWeWy ||
		PiszGrupa(330, 2) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbSymbolTableRecord") == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbViewportTableRecord") == ErrorWeWy) return ErrorWeWy;

	if (PiszGrupa(2, (char*)"*ACTIVE") == ErrorWeWy ||
		PiszGrupa(70, 0) == ErrorWeWy ||
		PiszGrupa(10, Xmin) == ErrorWeWy ||
		PiszGrupa(20, Ymin) == ErrorWeWy ||
		PiszGrupa(11, Xmax) == ErrorWeWy ||
		PiszGrupa(21, Ymax) == ErrorWeWy ||
		PiszGrupa(12, (Xmax-Xmin)/2.0) == ErrorWeWy ||
		PiszGrupa(22, (Ymax - Ymin) / 2.0) == ErrorWeWy ||
		PiszGrupa(13, 0.0) == ErrorWeWy ||
		PiszGrupa(23, 0.0) == ErrorWeWy ||
		PiszGrupa(14, krok_s) == ErrorWeWy ||
		PiszGrupa(24, krok_s) == ErrorWeWy ||
		PiszGrupa(15, krok_g) == ErrorWeWy ||
		PiszGrupa(25, krok_g) == ErrorWeWy ||
		PiszGrupa(16, 0) == ErrorWeWy ||
		PiszGrupa(26, 0) == ErrorWeWy ||
		PiszGrupa(36, 1) == ErrorWeWy ||
		PiszGrupa(17, 0) == ErrorWeWy ||
		PiszGrupa(27, 0) == ErrorWeWy ||
		PiszGrupa(37, 0) == ErrorWeWy ||
		PiszGrupa(40, Xmax-Xmin) == ErrorWeWy ||
		PiszGrupa(41, Ymax-Ymin) == ErrorWeWy ||
		PiszGrupa(42, 50) == ErrorWeWy ||
		PiszGrupa(43, 0) == ErrorWeWy ||
		PiszGrupa(44, 0) == ErrorWeWy ||
		PiszGrupa(50, 0) == ErrorWeWy ||
		PiszGrupa(51, 0) == ErrorWeWy ||
		PiszGrupa(71, 0) == ErrorWeWy ||
		PiszGrupa(72, (Ymax - Ymin) / 2.0) == ErrorWeWy ||
		PiszGrupa(73, 1) == ErrorWeWy ||
		PiszGrupa(74, 3) == ErrorWeWy ||
		PiszGrupa(75, snap_) == ErrorWeWy ||
		PiszGrupa(76, grid_) == ErrorWeWy ||
		PiszGrupa(77, 0) == ErrorWeWy ||
		PiszGrupa(78, 0) == ErrorWeWy) return ErrorWeWy;

	else return OkWeWy;
}


int PISZ_TABLES::PiszVport(void)
{ 
	if (PiszGrupa(0, (char*)TABLE) == ErrorWeWy ||
		PiszGrupa(2, (char*)VPORT) == ErrorWeWy ||
		PiszGrupa(5, handle_hex) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbSymbolTable") == ErrorWeWy ||
		PiszGrupa(70, 1) == ErrorWeWy || //end table def
		PiszViewPorts() == ErrorWeWy ||
		PiszGrupa(0, (char*)ENDTAB) == ErrorWeWy) return ErrorWeWy;
	else return OkWeWy;
}

int PISZ_TABLES::PiszView()
{ 
	if (PiszGrupa(0, (char*)TABLE) == ErrorWeWy ||
		PiszGrupa(2, (char*)VIEW) == ErrorWeWy ||
		PiszGrupa(5, handle_hex) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbSymbolTable") == ErrorWeWy ||
		PiszGrupa(70, 0) == ErrorWeWy ||
		PiszGrupa(0, (char*)"ENDTAB") == ErrorWeWy)  return ErrorWeWy;
	else return OkWeWy;
}
int PISZ_TABLES::PiszUcs()
{ 
	if (PiszGrupa(0, (char*)TABLE) == ErrorWeWy ||
		PiszGrupa(2, (char*)UCS) == ErrorWeWy ||
		PiszGrupa(5, handle_hex) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbSymbolTable") == ErrorWeWy ||
		PiszGrupa(70, 0) == ErrorWeWy ||
		PiszGrupa(0, (char*)"ENDTAB") == ErrorWeWy)  return ErrorWeWy;
	else return OkWeWy;
}
int PISZ_TABLES::PiszAppid()
{ 
	if (PiszGrupa(0, (char*)TABLE) == ErrorWeWy ||
		PiszGrupa(2, (char*)APPID) == ErrorWeWy ||
		PiszGrupa(5, handle_hex) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbSymbolTable") == ErrorWeWy ||
		PiszGrupa(70, 1) == ErrorWeWy) return ErrorWeWy;

	    if (PiszGrupa(0, (char*)APPID) == ErrorWeWy ||
			PiszGrupa(5, handle_hex) == ErrorWeWy ||
			PiszGrupa(100, (char*)"AcDbSymbolTableRecord") == ErrorWeWy ||
			PiszGrupa(100, (char*)"AcDbRegAppTableRecord") == ErrorWeWy ||
			PiszGrupa(2, (char*)"ACAD") == ErrorWeWy ||
			PiszGrupa(70, 0) == ErrorWeWy) return ErrorWeWy;
	   
		if (PiszGrupa(0, (char*)APPID) == ErrorWeWy ||
			PiszGrupa(5, handle_hex) == ErrorWeWy ||
			PiszGrupa(100, (char*)"AcDbSymbolTableRecord") == ErrorWeWy ||
			PiszGrupa(100, (char*)"AcDbRegAppTableRecord") == ErrorWeWy ||
			PiszGrupa(2, (char*)"ALFACAD") == ErrorWeWy ||
			PiszGrupa(70, 0) == ErrorWeWy ||

		PiszGrupa(0, (char*)"ENDTAB") == ErrorWeWy)  return ErrorWeWy;

	return OkWeWy;
}

int PISZ_TABLES::PiszDimstyle()
{
	DRW_Dimstyle ent_;
	DRW_Dimstyle *ent;

	ent = &ent_;

	ent->dimasz = 0.18;
	ent->dimtxt = zmwym.wysokosc;
	ent->dimexe = 0.18;
	ent->dimexo = 0.0625;
	ent->dimgap = ent->dimcen = 0.09;
	ent->dimtxsty = PTRS__Text_Style[zmwym.czcionka] != NULL &&
		PTRS__Text_Style[zmwym.czcionka]->font_acad_name[0] != '\0' ?
		PTRS__Text_Style[zmwym.czcionka]->font_name : STANDARD;
	ent->dimscale = SkalaF;
	ent->dimlfac = SkalaF;
	ent->dimtfac = 1.0;
	ent->dimfxl = 1.0;
	ent->dimdli = 0.38;
	ent->dimrnd = zmwym.dokladnosc;
	ent->dimdle = 0.18;
	ent->dimtp = 0.0;
	ent->dimtm = 0.0;
	ent->dimtsz = 0;
	ent->dimtvp = 0;
	ent->dimaltf = 25.4;
	ent->dimtol = 0;
	ent->dimlim = 0;
	ent->dimse1 = 0;
	ent->dimse2 = 0;
	ent->dimtad = 0;
	ent->dimzin = 0;
	ent->dimtoh = (int)zmwym.kierunek_t;
	ent->dimtolj = 1;
	ent->dimalt = ent->dimtofl = ent->dimsah = ent->dimtix = ent->dimsoxd = ent->dimfxlon = 0;
	ent->dimaltd = ent->dimunit = ent->dimaltu = ent->dimalttd = ent->dimlunit = 2;
	ent->dimclrd = (int)color_dxf(zmwym.Lkolor);
	ent->dimclre = (int)color_dxf(zmwym.Lkolor);
	ent->dimclrt = (int)color_dxf(zmwym.Tkolor);
	ent->dimjust = 0;
	ent->dimupt = 0;
	ent->dimazin = 2;
	ent->dimaltz = 0;
	ent->dimaltttz = 0;
	ent->dimtzin = 0;
	ent->dimfrac = 0;
	ent->dimtih = ent->dimadec = ent->dimaunit = ent->dimsd1 = ent->dimsd2 = ent->dimtmove = 0;
	ent->dimaltrnd = zmwym.dokladnosc;
	ent->dimdec = ent->dimtdec = 4;
	ent->dimfit = ent->dimatfit = 3;
	ent->dimdsep = '.';
	ent->dimlwd = ent->dimlwe = -2;

	if (PiszGrupa(0, (char*)TABLE) == ErrorWeWy ||
		PiszGrupa(2, (char*)DIMSTYLE) == ErrorWeWy ||
		PiszGrupa(5, handle_hex) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbSymbolTable") == ErrorWeWy ||
		PiszGrupa(70, 1) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbDimStyleTable") == ErrorWeWy ||
		PiszGrupa(71, 1) == ErrorWeWy) return ErrorWeWy;

	if (PiszGrupa(0, (char*)"DIMSTYLE") == ErrorWeWy ||
		PiszGrupa(105, handle_hex) == ErrorWeWy ||
		PiszGrupa(330, (char*)"A") == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbSymbolTableRecord") == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbDimStyleTableRecord") == ErrorWeWy ||
		PiszGrupa(2, (char*)"Standard") == ErrorWeWy ||
		PiszGrupa(70, 0) == ErrorWeWy ||
        PiszGrupa(40, 1) == ErrorWeWy ||
		PiszGrupa(41, ent->dimasz) == ErrorWeWy ||
		PiszGrupa(42, ent->dimexo) == ErrorWeWy ||
		PiszGrupa(43, ent->dimdli) == ErrorWeWy ||
		PiszGrupa(44, ent->dimexe) == ErrorWeWy ||
		PiszGrupa(45, ent->dimrnd) == ErrorWeWy ||
		PiszGrupa(46, ent->dimdle) == ErrorWeWy ||
		PiszGrupa(47, ent->dimtp) == ErrorWeWy ||
		PiszGrupa(48, ent->dimtm) == ErrorWeWy) return ErrorWeWy;
	
	if (PiszGrupa(140, ent->dimtxt) == ErrorWeWy ||
		PiszGrupa(141, ent->dimcen) == ErrorWeWy ||
		PiszGrupa(142, ent->dimtsz) == ErrorWeWy ||
		PiszGrupa(143, ent->dimaltf) == ErrorWeWy ||
		PiszGrupa(144, ent->dimlfac) == ErrorWeWy ||
		PiszGrupa(145, ent->dimtvp) == ErrorWeWy ||
		PiszGrupa(146, ent->dimtfac) == ErrorWeWy ||
		PiszGrupa(147, ent->dimgap) == ErrorWeWy)  return ErrorWeWy;
	
	if (PiszGrupa(71, ent->dimtol) == ErrorWeWy ||
		PiszGrupa(72, ent->dimlim) == ErrorWeWy ||
		PiszGrupa(73, ent->dimtih) == ErrorWeWy ||
		PiszGrupa(74, ent->dimtoh) == ErrorWeWy ||
		PiszGrupa(75, ent->dimse1) == ErrorWeWy ||
		PiszGrupa(76, ent->dimse2) == ErrorWeWy ||
		PiszGrupa(77, ent->dimtad) == ErrorWeWy ||
		PiszGrupa(78, ent->dimzin) == ErrorWeWy) return ErrorWeWy;
	
	if (PiszGrupa(170, ent->dimalt) == ErrorWeWy ||
		PiszGrupa(171, ent->dimaltd) == ErrorWeWy ||
		PiszGrupa(172, ent->dimtofl) == ErrorWeWy ||
		PiszGrupa(173, ent->dimsah) == ErrorWeWy ||
		PiszGrupa(174, ent->dimtix) == ErrorWeWy ||
		PiszGrupa(175, ent->dimsoxd) == ErrorWeWy ||
		PiszGrupa(176, ent->dimclrd) == ErrorWeWy ||
		PiszGrupa(177, ent->dimclre) == ErrorWeWy ||
		PiszGrupa(178, ent->dimclrt) == ErrorWeWy) return ErrorWeWy;
	
	if (PiszGrupa(270, ent->dimunit)== ErrorWeWy ||
		PiszGrupa(271, ent->dimdec)== ErrorWeWy ||
		PiszGrupa(272, ent->dimtdec)== ErrorWeWy ||
		PiszGrupa(273, ent->dimaltu)== ErrorWeWy ||
		PiszGrupa(274, ent->dimalttd)== ErrorWeWy ||
		PiszGrupa(275, ent->dimaunit)== ErrorWeWy) return ErrorWeWy;

	if (PiszGrupa(280, ent->dimjust)== ErrorWeWy ||
		PiszGrupa(281, ent->dimsd1)== ErrorWeWy ||
		PiszGrupa(282, ent->dimsd2)== ErrorWeWy ||
		PiszGrupa(283, ent->dimtolj)== ErrorWeWy ||
		PiszGrupa(284, ent->dimtzin)== ErrorWeWy ||
		PiszGrupa(285, ent->dimaltz)== ErrorWeWy ||
		PiszGrupa(286, ent->dimaltttz)== ErrorWeWy ||
		PiszGrupa(287, ent->dimfit)== ErrorWeWy ||
		PiszGrupa(288, ent->dimupt)== ErrorWeWy) return ErrorWeWy;
	
	if (PiszGrupa(340, (char*)ent->dimtxsty.c_str()) == ErrorWeWy ||
		PiszGrupa(0, (char*)"ENDTAB") == ErrorWeWy)  return ErrorWeWy;
	else  return OkWeWy;
}

int PISZ_TABLES::PiszBlockRecord()
{
	if (PiszGrupa(0, (char*)TABLE) == ErrorWeWy ||
		PiszGrupa(2, (char*)BLOCK_RECORD) == ErrorWeWy ||
		PiszGrupa(5, handle_hex) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbSymbolTable") == ErrorWeWy ||
		PiszGrupa(70, 2) == ErrorWeWy) return ErrorWeWy;

	if (PiszGrupa(0, (char*)"BLOCK_RECORD") == ErrorWeWy ||
	PiszGrupa(5, handle_hex) == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbSymbolTableRecord") == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbBlockTableRecord") == ErrorWeWy ||
	PiszGrupa(2, (char*)"*Model_Space") == ErrorWeWy) return ErrorWeWy;

	if (PiszGrupa(0, (char*)"BLOCK_RECORD") == ErrorWeWy ||
		PiszGrupa(5, handle_hex) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbSymbolTableRecord") == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbBlockTableRecord") == ErrorWeWy ||
		PiszGrupa(2, (char*)"*Paper_Space") == ErrorWeWy) return ErrorWeWy;
	
	//if (PiszGrupa(0, "ENDTAB") == ErrorWeWy)  return ErrorWeWy;
	return OkWeWy;
}


/*-------------------------------------------------------*/


/*------  class PISZ_OBJECTS  -------------------------------------*/

//class PISZ_OBJECTS : virtual public PISZ_GRUPA
//  protected :
//   int RysujLinia(LINIA *L);
//   int RysujText(TEXT* t);
//   int RysujKolo(OKRAG* k);
//   int RysujOkrag(OKRAG*o);
//   int RysujLuk(LUK* l);
//   int RysujSolid(WIELOKAT * s);
//  public  :
//   PISZ_OBJECTS(void);

PISZ_OBJECTS::PISZ_OBJECTS(void)
{}


#define NO_DIM TRUE

int PISZ_OBJECTS::InsBlock(INS_BLOCK ins_b1)
{
	if ((ins_b1.kod_obiektu == B_DIM) && (!NO_DIM))
	{
		if (PiszGrupa(0, (char*)DIMENSION) == ErrorWeWy ||
			PiszGrupa(5, handle_hex) == ErrorWeWy ||
			PiszGrupa(8, layer_dxf(Layers[0].name, 0)) == ErrorWeWy) return ErrorWeWy;
		if (ins_b1.angle != 0.0)
		{
			if (PiszGrupa(50, ins_b1.angle) == ErrorWeWy) return ErrorWeWy;
		}
	}
	else
	{
		if (PiszGrupa(0, (char*)INSERT) == ErrorWeWy ||
			PiszGrupa(5, handle_hex) == ErrorWeWy ||
			PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
			PiszGrupa(8, layer_dxf(Layers[0].name, 0)) == ErrorWeWy) return ErrorWeWy;
    }

  if (PiszGrupa(100,(char*)"AcDbBlockReference") == ErrorWeWy) return ErrorWeWy;
  if(PiszGrupa(2,ins_b1.nazwa_bloku)==ErrorWeWy) return ErrorWeWy; 
  if (PiszGrupa(5, handle_hex) == ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(10,(double)(ins_b1.x0))==ErrorWeWy  ||
     PiszGrupa(20,(double)(ins_b1.y0))==ErrorWeWy  ||
     PiszGrupa(30,0.0)==ErrorWeWy  ) return ErrorWeWy;
    if (ins_b1.invisible==1)
    {
        if (PiszGrupa(60, 1) == ErrorWeWy) return ErrorWeWy;
    }
  return OkWeWy;
}


int PISZ_OBJECTS::RecordBlock(INS_BLOCK ins_b1)
{
	if (PiszGrupa(70, 2) == ErrorWeWy ||
		PiszGrupa(0, (char*)"BLOCK_RECORD") == ErrorWeWy ||
		PiszGrupa(5, handle_hex) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbSymbolTableRecord") == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbBlockTableRecord") == ErrorWeWy ||
		PiszGrupa(2, ins_b1.nazwa_bloku) == ErrorWeWy) return ErrorWeWy;
    if (ins_b1.invisible==1)
    {
        if (PiszGrupa(60, 1) == ErrorWeWy) return ErrorWeWy;
    }
	return OkWeWy;
}


int PISZ_OBJECTS::IniBlock(INS_BLOCK ins_b1)
{ if(PiszGrupa(0,(char*)BLOCK)==ErrorWeWy ||
	PiszGrupa(5, handle_hex) == ErrorWeWy ||
	PiszGrupa(100,(char*)"AcDbEntity") == ErrorWeWy ||
     PiszGrupa(8, layer_dxf (Layers[0].name, 0))==ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(100, (char*)"AcDbBlockBegin") == ErrorWeWy) return ErrorWeWy;
  if(PiszGrupa(2,ins_b1.nazwa_bloku)==ErrorWeWy) return ErrorWeWy;

  if (ins_b1.invisible==1)
  {
      if (PiszGrupa(60, 1) == ErrorWeWy) return ErrorWeWy;
  }


  if (PiszGrupa(70, 0) == ErrorWeWy) return ErrorWeWy;
 if(PiszGrupa(10,(double)(ins_b1.x0))==ErrorWeWy  ||
 PiszGrupa(20,(double)(ins_b1.y0))==ErrorWeWy  ||
 PiszGrupa(30,0.0)==ErrorWeWy  ) return ErrorWeWy;
  if(PiszGrupa(3,ins_b1.nazwa_bloku)==ErrorWeWy) return ErrorWeWy; 

  if (((ins_b1.kod_obiektu == B_DIM) || (ins_b1.kod_obiektu == B_DIM1)  || (ins_b1.kod_obiektu == B_DIM2)) && (ins_b1.angle != 0.0))
  {
	  if (PiszGrupa(999, ins_b1.angle) == ErrorWeWy) return ErrorWeWy;
  }
  
  return OkWeWy;
}


int PISZ_OBJECTS::EndBlock(void)
{ if((PiszGrupa(0,(char*)ENDBLK)==ErrorWeWy) ||
    PiszGrupa(5, handle_hex) == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
	PiszGrupa(8, layer_dxf(Layers[0].name, 0)) == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbBlockEnd") == ErrorWeWy) return ErrorWeWy;
  else return OkWeWy;
}

int PISZ_OBJECTS::RysujLinia(LINIA *L)
{
	char code123[10];
	int code123i;

	if(PiszGrupa(0,(char*)LINE)==ErrorWeWy ||
	 PiszGrupa(5, handle_hex) == ErrorWeWy ||
	 PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
     PiszGrupa(8, layer_dxf (Layers[L->warstwa].name, L->warstwa))==ErrorWeWy) return ErrorWeWy;
  if(typ_linii(L->typ)!=TBYLAYER &&
   PiszGrupa(6,TypyLinii[typ_linii(L->typ)].nazwa)==ErrorWeWy) return ErrorWeWy;
  if(L->kolor!=CBYLAYER &&
   PiszGrupa(62,(int)color_dxf (L->kolor))==ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(100,(char*)"AcDbLine")==ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(370, (int)get_width370(L->typ)) == ErrorWeWy) return ErrorWeWy;
  if (get_width_invisible(L->typ))
  {
	  if (PiszGrupa(999, (char*)"invisible") == ErrorWeWy) return ErrorWeWy;
  }
  if(PiszGrupa(10,(double)(L->x1))==ErrorWeWy  ||
     PiszGrupa(20,(double)(L->y1))==ErrorWeWy  ||
     PiszGrupa(30,0.0)==ErrorWeWy	      ||
     PiszGrupa(11,(double)(L->x2))==ErrorWeWy  ||
     PiszGrupa(21,(double)(L->y2))==ErrorWeWy  ||
     PiszGrupa(31,0.0)==ErrorWeWy  ) return ErrorWeWy;

  code123i = L->obiektt1 * 100 + L->obiektt2 * 10 + L->obiektt3;
  switch (code123i)
  {
  case 0:
	  break;
  case 10:
	  strncpy(code123, code999[0], 8);  
	  if (PiszGrupa(999, code123) == ErrorWeWy) return ErrorWeWy;
	  break;
  case 11:
	  strncpy(code123, code999[2], 8);
	  if (PiszGrupa(999, code123) == ErrorWeWy) return ErrorWeWy;
	  break;
  case 310:
	  strncpy(code123, code999[3], 8);
	  if (PiszGrupa(999, code123) == ErrorWeWy) return ErrorWeWy;
	  break;
  default:
	  break;
  }

   return OkWeWy;
}

int PISZ_OBJECTS::RysujLinia3D(LINIA3D *L)
{ if(PiszGrupa(0,(char*)LINE)==ErrorWeWy ||
	PiszGrupa(5, handle_hex) == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
     PiszGrupa(8, layer_dxf (Layers[L->warstwa].name, L->warstwa))==ErrorWeWy) return ErrorWeWy;
  if(typ_linii(L->typ)!=TBYLAYER &&
   PiszGrupa(6,TypyLinii[typ_linii(L->typ)].nazwa)==ErrorWeWy) return ErrorWeWy;
  if(L->kolor!=CBYLAYER &&
   PiszGrupa(62,(int)color_dxf (L->kolor))==ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(100, (char*)"AcDbLine") == ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(370, (int)get_width370(L->typ)) == ErrorWeWy) return ErrorWeWy;
  if(PiszGrupa(10,(double)(L->x1))==ErrorWeWy  ||
     PiszGrupa(20,(double)(L->y1))==ErrorWeWy  ||
     PiszGrupa(30,(double)(L->z1))==ErrorWeWy  ||
     PiszGrupa(11,(double)(L->x2))==ErrorWeWy  ||
     PiszGrupa(21,(double)(L->y2))==ErrorWeWy  ||
     PiszGrupa(31,(double)(L->z2))==ErrorWeWy  ) return ErrorWeWy;
  else return OkWeWy;
}

void slash2underline (char * tekst)
/*-------------------------------*/
{ 
  int dl_tekst;
  unsigned char * ptrsz_tmp;

  dl_tekst=(int)strlen(tekst);
  if (dl_tekst==0) return;
  for (ptrsz_tmp = (unsigned char *)tekst; *ptrsz_tmp != '\0'; ptrsz_tmp++)
   {
    if ((*ptrsz_tmp=='\057') || (*ptrsz_tmp=='\134'))
         *ptrsz_tmp='\137';
   }
}

extern "C" {
void alfpro2acad (char * tekst)
/*---------------------------*/
{
  int dl_tekst;
  unsigned char * ptrsz_tmp;
  unsigned char a;

  dl_tekst=(int)strlen(tekst);
  if (dl_tekst==0) return;
  for (ptrsz_tmp = (unsigned char *)tekst; *ptrsz_tmp != '\0'; ptrsz_tmp++)
    {
	a = *ptrsz_tmp;
    if ((*ptrsz_tmp<'\060') && (*ptrsz_tmp!='\044') && (*ptrsz_tmp!='\055'))
       *ptrsz_tmp='\137';
       else
    if ((*ptrsz_tmp>'\071') && (*ptrsz_tmp<'\101'))
       *ptrsz_tmp='\137';
       else
    if ((*ptrsz_tmp>'\132') && (*ptrsz_tmp<'\137')) 
       *ptrsz_tmp='\137';
       else
    if (*ptrsz_tmp=='\140') *ptrsz_tmp='\137';
       else
    if ((*ptrsz_tmp=='\173') || (*ptrsz_tmp=='\174') || (*ptrsz_tmp=='\175') ||
        (*ptrsz_tmp=='\176') || (*ptrsz_tmp=='\177') || (*ptrsz_tmp=='\200'))
          *ptrsz_tmp='\137';
       else
    if (*ptrsz_tmp=='\361') *ptrsz_tmp='\261';    
       else
      { switch (*ptrsz_tmp)
       { 
   case a_PL : *ptrsz_tmp = 'a' ; break ;
	case c_PL : *ptrsz_tmp = 'c' ; break ;
	case e_PL : *ptrsz_tmp = 'e' ; break ;
	case l_PL : *ptrsz_tmp = 'l' ; break ;
	case n_PL : *ptrsz_tmp = 'n' ; break ;
	case o_PL : *ptrsz_tmp = 'o' ; break ;
	case s_PL : *ptrsz_tmp = 's' ; break ;
	case z_PL : *ptrsz_tmp = 'z' ; break ;
	case x_PL : *ptrsz_tmp = 'z' ; break ;
	case A_PL : *ptrsz_tmp = 'A' ; break ;
	case C_PL : *ptrsz_tmp = 'C' ; break ;
	case E_PL : *ptrsz_tmp = 'E' ; break ;
	case L_PL : *ptrsz_tmp = 'L' ; break ;
	case N_PL : *ptrsz_tmp = 'N' ; break ;
	case O_PL : *ptrsz_tmp = 'O' ; break ;
	case S_PL : *ptrsz_tmp = 'S' ; break ;
	case Z_PL : *ptrsz_tmp = 'Z' ; break ;
	case X_PL : *ptrsz_tmp = 'Z' ; break ;

	default: if (*ptrsz_tmp>'\177') *ptrsz_tmp='\137'; break;
       }	 
      }
       
   }
  strupr_(tekst);
}

}

void mazovia2acad (char * tekst)
/*-----------------------------*/
{ int i, dl_tekst;
  dl_tekst=(int)strlen(tekst);
  if (dl_tekst==0) return;
  for (i=0;i<dl_tekst; i++)
   {
    switch (tekst[i])
    {
    case '\136':tekst[i]='\040';   //^
             break;
	case '\176':tekst[i]='\040';   //~
			 break;
    case '\206':tekst[i]='\271';   //
             break;
    case '\215':tekst[i]='\346';   //
             break;
    case '\221':tekst[i]='\352';   //
             break;
    case '\222':tekst[i]='\263';   //
             break;
    case '\244':tekst[i]='\361';   //
             break;
    case '\242':tekst[i]='\363';   //
             break;
    case '\236':tekst[i]='\234';   //
             break;
    case '\246':tekst[i]='\237';   //
             break;
    case '\247':tekst[i]='\277';   //
             break;
    case '\217':tekst[i]='\245';
             break;
    case '\225':tekst[i]='\306';
             break;
    case '\220':tekst[i]='\312';
             break;
    case '\234':tekst[i]='\243';
             break;
    case '\245':tekst[i]='\321';
             break;
    case '\243':tekst[i]='\323';
             break;
    case '\230':tekst[i]='\214';
             break;
    case '\240':tekst[i]='\217';
             break;
    case '\241':tekst[i]='\257';
             break;
    default: break;             
    }
   }
}

void utf82acad(char * tekst)
/*-----------------------------*/
{

	int dl_tekst;
	char buf[MAXLINE];
	unsigned int unicode;

	uint8_t *ptr;
	const uint8_t *ptr1=NULL;
	char ch;
	

	dl_tekst = (int)strlen(tekst);
	if (dl_tekst == 0) return;

	strcpy(buf, "");

	ptr = (uint8_t *)tekst;

	while (*ptr != '\0')
	{
		unicode = utf8_to_ucs2(ptr, &ptr1);
		ptr = (uint8_t *)ptr1;

		if (unicode < 127)
		{
			ch = unicode;
			strncat(buf, &ch, 1);
		}
		else
		{
			switch (unicode)
			{

			case 0X105:  ch = '\271';   //a
				break;
			case 0X107:ch = '\346';   //c
				break;
			case 0X119:ch = '\352';   //e
				break;
			case 0X142:ch = '\263';   //l
				break;
			case 0X144:ch = '\361';   //n
				break;
			case 0XF3:ch = '\363';   //o
				break;
			case 0X15B:ch = '\234';   //s
				break;
			case 0X17A:ch = '\237';   //z'
				break;
			case 0X17C:ch = '\277';   //z.
				break;
			case 0X104:ch = '\245';  //A
				break;
			case 0X106:ch = '\306';  //C
				break;
			case 0X118:ch = '\312';  //E
				break;
			case 0X141:ch = '\243';  //L
				break;
			case 0X143:ch = '\321';  //N
				break;
			case 0XD3:ch = '\323';  //O
				break;
			case 0X15A:ch = '\214';  //S
				break;
			case 0X179:ch = '\217';  //Z'
				break;
			case 0X17B:ch = '\257';  //Z.
				break;
			case 0xB1:ch = '\360';   //plus minus
				break;
			default: 
				ch = '\40';
				break;
			}
			strncat(buf, &ch, 1);
		}
	}
	strcpy(tekst, buf);
}


void mazovia2latin (char * tekst)
/*-----------------------------*/
{ int i, dl_tekst;
  dl_tekst=(int)strlen(tekst);
  if (dl_tekst==0) return;
  for (i=0;i<dl_tekst; i++)
   {
    switch (tekst[i])
    {
    case '\206':tekst[i]='\245';
             break;
    case '\215':tekst[i]='\206';
             break;
    case '\221':tekst[i]='\251';
             break;
    case '\222':tekst[i]='\210';
             break;
    case '\244':tekst[i]='\344';
             break;
    case '\242':tekst[i]='\242';
             break;
    case '\236':tekst[i]='\230';
             break;
    case '\246':tekst[i]='\253';
             break;
    case '\247':tekst[i]='\276';
             break;
    case '\217':tekst[i]='\244';
             break;
    case '\225':tekst[i]='\217';
             break;
    case '\220':tekst[i]='\250';
             break;
    case '\234':tekst[i]='\235';
             break;
    case '\245':tekst[i]='\343';
             break;
    case '\243':tekst[i]='\340';
             break;
    case '\230':tekst[i]='\227';
             break;
    case '\240':tekst[i]='\215';
             break;
    case '\241':tekst[i]='\275';
             break;
    default: break;             
    }
   }
}

void utf82latin(char * tekst)
/*-----------------------------*/
{
	int dl_tekst;
	char buf[MAXLINE];
	unsigned int unicode;
	uint8_t *ptr;
	const uint8_t *ptr1=NULL;
	char ch;


	dl_tekst = (int)strlen(tekst);
	if (dl_tekst == 0) return;

	strcpy(buf, "");

	ptr = (uint8_t *)tekst;

	while (*ptr != '\0')
	{
		unicode = utf8_to_ucs2(ptr, &ptr1);
		ptr = (uint8_t *)ptr1;

		if (unicode < 127)
		{
			ch = unicode;
			strncat(buf, &ch, 1);
		}
		else
		{
			switch (unicode)
			{
			case 0X105:  ch = '\245';   //a
				break;
			case 0X107:ch = '\206';   //c
				break;
			case 0X119:ch = '\251';   //e
				break;
			case 0X142:ch = '\210';   //l
				break;
			case 0X144:ch = '\344';   //n
				break;
			case 0XF3:ch = '\242';   //o
				break;
			case 0X15B:ch = '\230';   //s
				break;
			case 0X17A:ch = '\253';   //z'
				break;
			case 0X17C:ch = '\276';   //z.
				break;
			case 0X104:ch = '\244';  //A
				break;
			case 0X106:ch = '\217';  //C
				break;
			case 0X118:ch = '\250';  //E
				break;
			case 0X141:ch = '\235';  //L
				break;
			case 0X143:ch = '\343';  //N
				break;
			case 0XD3:ch = '\340';  //O
				break;
			case 0X15A:ch = '\227';  //S
				break;
			case 0X179:ch = '\215';  //Z'
				break;
			case 0X17B:ch = '\275';  //Z.
				break;
			case 0xB1:ch = '\361';   //plus minus
				break;
			default:
				ch = '\40';
				break;
			}
			strncat(buf, &ch, 1);
		}
	}
	strcpy(tekst, buf);
}


void normalize_file_name(char* tekst)
{
	uint8_t* ptr;
	const uint8_t* ptr1 = NULL;
	unsigned int unicode;
	char ch;

	ptr = (uint8_t*)tekst;

	while (*ptr != '\0')
	{
		unicode = utf8_to_ucs2(ptr, &ptr1);
		if (unicode < 127)
		{
			ch = unicode;
			if (ch == '/' || (ch == '@'))
			{
				ch = '_';
				*ptr = ch;
			}
		}
		else if (unicode == 1422)
		{
			ch = '_';
			*ptr = ch;
			*(ptr+1) = ch;
		}

		ptr = (uint8_t*)ptr1;
	}
}

void utf82none(char * tekst)
/*-----------------------------*/
{
	int dl_tekst;
	char buf[MAXLINE];
	unsigned int unicode;
	uint8_t *ptr;
	const uint8_t *ptr1 = NULL;
	char ch;


	dl_tekst = (int)strlen(tekst);
	if (dl_tekst == 0) return;

	strcpy(buf, "");

	ptr = (uint8_t *)tekst;

	while (*ptr != '\0')
	{
		unicode = utf8_to_ucs2(ptr, &ptr1);
		ptr = (uint8_t *)ptr1;

		if (unicode < 127)
		{
            ch = unicode;
			if ((ch == '/') || (ch == '@')) ch = '_';
			strncat(buf, &ch, 1);
		}
		else
		{
			switch (unicode)
			{
			case 0X58E:   ch = '_';  //
				strncat(buf, &ch, 1);
				break;
			case 0X105:  ch = 'a';   //a
				break;
			case 0X107:ch = 'c';   //c
				break;
			case 0X119:ch = 'e';   //e
				break;
			case 0X142:ch = 'l';   //l
				break;
			case 0X144:ch = 'n';   //n
				break;
			case 0XF3:ch = 'o';   //o
				break;
			case 0X15B:ch = 's';   //s
				break;
			case 0X17A:ch = 'z';   //z'
				break;
			case 0X17C:ch = 'z';   //z.
				break;
			case 0X104:ch = 'A';  //A
				break;
			case 0X106:ch = 'C';  //C
				break;
			case 0X118:ch = 'E';  //E
				break;
			case 0X141:ch = 'L';  //L
				break;
			case 0X143:ch = 'N';  //N
				break;
			case 0XD3:ch = 'O';  //O
				break;
			case 0X15A:ch = 'S';  //S
				break;
			case 0X179:ch = 'Z';  //Z'
				break;
			case 0X17B:ch = 'Z';  //Z.
				break;
			case 0xB1:ch = '-';   //plus minus
				break;
			default:
				ch = '_';
				break;
			}
			strncat(buf, &ch, 1);
		}
	}
	strcpy(tekst, buf);
}


void utf82ISOlatin(char * tekst)
/*-----------------------------*/
{
	int dl_tekst;
	char buf[MAXLINE];
	unsigned int unicode;
	uint8_t *ptr;
	const uint8_t *ptr1 = NULL;
	char ch;


	dl_tekst = (int)strlen(tekst);
	if (dl_tekst == 0) return;

	strcpy(buf, "");

	ptr = (uint8_t *)tekst;

	while (*ptr != '\0')
	{
		unicode = utf8_to_ucs2(ptr, &ptr1);
		ptr = (uint8_t *)ptr1;

		if (unicode < 127)
		{
			ch = unicode;
			strncat(buf, &ch, 1);
		}
		else
		{
			switch (unicode)
			{
			case 0X105: ch = '\261';   //a
				break;
			case 0X107:ch = '\346';   //c
				break;
			case 0X119:ch = '\352';   //e
				break;
			case 0X142:ch = '\263';   //l
				break;
			case 0X144:ch = '\361';   //n
				break;
			case 0XF3:ch = '\363';   //o
				break;
			case 0X15B:ch = '\266';   //s
				break;
			case 0X17A:ch = '\274';   //z'
				break;
			case 0X17C:ch = '\277';   //z.
				break;
			case 0X104:ch = '\241';  //A
				break;
			case 0X106:ch = '\306';  //C
				break;
			case 0X118:ch = '\312';  //E
				break;
			case 0X141:ch = '\243';  //L
				break;
			case 0X143:ch = '\321';  //N
				break;
			case 0XD3:ch = '\323';  //O
				break;
			case 0X15A:ch = '\246';  //S
				break;
			case 0X179:ch = '\254';  //Z'
				break;
			case 0X17B:ch = '\257';  //Z.
				break;
			case 0xB1:ch = '\360';   //plus minus
				break;
			default:
				ch = '\40';
				break;
			}
			strncat(buf, &ch, 1);
		}
	}
	strcpy(tekst, buf);
}


void mazovia2none(char * tekst)
/*-----------------------------*/
{
	int i, dl_tekst;
	dl_tekst = (int)strlen(tekst);
	if (dl_tekst == 0) return;
	for (i = 0; i < dl_tekst; i++)
	{
		switch (tekst[i])
		{
		case '\206':tekst[i] = 'a';
			break;
		case '\215':tekst[i] = 'c';
			break;
		case '\221':tekst[i] = 'e';
			break;
		case '\222':tekst[i] = 'l';
			break;
		case '\244':tekst[i] = 'n';
			break;
		case '\242':tekst[i] = 'o';
			break;
		case '\236':tekst[i] = 's';
			break;
		case '\246':tekst[i] = 'z';
			break;
		case '\247':tekst[i] = 'z';
			break;
		case '\217':tekst[i] = 'A';
			break;
		case '\225':tekst[i] = 'C';
			break;
		case '\220':tekst[i] = 'E';
			break;
		case '\234':tekst[i] = 'L';
			break;
		case '\245':tekst[i] = 'N';
			break;
		case '\243':tekst[i] = 'O';
			break;
		case '\230':tekst[i] = 'S';
			break;
		case '\240':tekst[i] = 'Z';
			break;
		case '\241':tekst[i] = 'Z';
			break;
		default: break;
		}
	}
}


void mazovia2w1250(char * tekst)
//-----------------------------
{
	int i, dl_tekst;
	dl_tekst = (int)strlen(tekst);
	if (dl_tekst == 0) return;
	for (i = 0; i < dl_tekst; i++)
	{
		switch (tekst[i])
		{
		case '\206':tekst[i] = '\271';
			break;
		case '\215':tekst[i] = '\346';
			break;
		case '\221':tekst[i] = '\352';
			break;
		case '\222':tekst[i] = '\263';
			break;
		case '\244':tekst[i] = '\361';
			break;
		case '\242':tekst[i] = '\363';
			break;
		case '\236':tekst[i] = '\234';
			break;
		case '\246':tekst[i] = '\237';
			break;
		case '\247':tekst[i] = '\277';
			break;
		case '\217':tekst[i] = '\245';
			break;
		case '\225':tekst[i] = '\306';
			break;
		case '\220':tekst[i] = '\312';
			break;
		case '\234':tekst[i] = '\243';
			break;
		case '\245':tekst[i] = '\321';
			break;
		case '\243':tekst[i] = '\323';
			break;
		case '\230':tekst[i] = '\214';
			break;
		case '\240':tekst[i] = '\217';
			break;
		case '\241':tekst[i] = '\257';
			break;
		default: break;
		}
	}
}

void latin2w1250(char * tekst)
//-----------------------------
{
	int i, dl_tekst;
	dl_tekst = (int)strlen(tekst);
	if (dl_tekst == 0) return;
	for (i = 0; i < dl_tekst; i++)
	{
		switch (tekst[i])
		{
		case '\245':tekst[i] = '\271';
			break;
		case '\206':tekst[i] = '\346';
			break;
		case '\251':tekst[i] = '\352';
			break;
		case '\210':tekst[i] = '\263';
			break;
		case '\344':tekst[i] = '\361';
			break;
		case '\242':tekst[i] = '\363';
			break;
		case '\230':tekst[i] = '\234';
			break;
		case '\253':tekst[i] = '\237';
			break;
		case '\276':tekst[i] = '\277';
			break;
		case '\244':tekst[i] = '\245';
			break;
		case '\217':tekst[i] = '\306';
			break;
		case '\250':tekst[i] = '\312';
			break;
		case '\235':tekst[i] = '\243';
			break;
		case '\343':tekst[i] = '\321';
			break;
		case '\340':tekst[i] = '\323';
			break;
		case '\227':tekst[i] = '\214';
			break;
		case '\215':tekst[i] = '\217';
			break;
		case '\275':tekst[i] = '\257';
			break;
		default: break;
		}
	}
}


int PISZ_OBJECTS::RysujText(TEXT* t)
{ char text_lat[MaxTextLen];
  double t_x, t_y;
  double x01, y01;
  double tdl, tsin, tcos;
  int t72, t73;
  char code123[10];
  int code123i;


  if (t->ukryty==TRUE) return OkWeWy;
  x01 = t->x ;
  y01 = t->y ;
  //jezeli justowanie jest rozne od j_do_lewej nalezy wyznaczyc nowy punkt t_x,t_y
  
  GetTextLen (t, &tdl) ;
  tsin = sin (t->kat) ;
  tcos = cos (t->kat) ; 
 

    switch (t->justowanie)
     {
       case 0: t_x = t->x;  //to the left
               t_y = t->y;
			   t72 = 0;
			   t73 = 0;
               break;
       case 1: t_x = x01 + tdl * tcos ;  //to the right
               t_y = y01 - Ky * tdl * tsin ;
			   t72 = 2;
			   t73 = 0;
               break;
       case 2: t_x = x01 + (tdl/2) * tcos ;  //srodkowo - DXF central
               t_y = y01 - (Ky * tdl)/2 * tsin ;
			   t72 = 1;
			   t73 = 0;
               break;
       case 3: t_x = x01 + (tdl/2) * tcos + (t->wysokosc/2) * tsin;  //centralnie - DXF Middle
               t_y = y01 - (Ky * tdl)/2 * tsin - (t->wysokosc/2) * tcos;
			   t72 = 1;
			   t73 = 2;
               break;
     }  
  
  if(PiszGrupa(0,(char*)"TEXT")==ErrorWeWy ||
	  PiszGrupa(5, handle_hex) == ErrorWeWy ||
	  PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
     PiszGrupa(8,layer_dxf (Layers[t->warstwa].name, t->warstwa))==ErrorWeWy) return ErrorWeWy;
  if(t->kolor!=CBYLAYER &&
   PiszGrupa(62,(int)color_dxf (t->kolor))==ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(100, (char*)"AcDbText") == ErrorWeWy) return ErrorWeWy;
  if(PiszGrupa(10,(double)(t->x))==ErrorWeWy  || 
     PiszGrupa(20,(double)(t->y))==ErrorWeWy  ||  
     PiszGrupa(30,0.0)==ErrorWeWy) return ErrorWeWy;


  if (t72 > 0)
  {
	  if (PiszGrupa(11, (double)(x01)) == ErrorWeWy ||
		  PiszGrupa(21, (double)(y01)) == ErrorWeWy ||
		  PiszGrupa(31, 0.0) == ErrorWeWy)  return ErrorWeWy;
  }

	  if (PiszGrupa(40, (double)(t->wysokosc)) == ErrorWeWy) return ErrorWeWy;

	  strcpy(text_lat, t->text);

	  encodingdxf((char *)&text_lat);
	  
	  if (PiszGrupa(1, (char *)(text_lat)) == ErrorWeWy) return ErrorWeWy;
     
  if(t->kat!=0.0 && PiszGrupa(50,(double)(t->kat)*180/Pi)==ErrorWeWy ) return ErrorWeWy;
  if (PiszGrupa (41, s__Fonts_Style [t->czcionka].df_width_factor * t->width_factor) == ErrorWeWy  ||
     PiszGrupa (51, (double)(t->italics == 1 ? ITALICS_ANGLE : 0)) == ErrorWeWy  ||
     PiszGrupa(7, PTRS__Text_Style[t->czcionka] != NULL &&
		  PTRS__Text_Style[t->czcionka]->font_acad_name [0] != '\0' ?
     PTRS__Text_Style[t->czcionka]->font_name : STANDARD)==ErrorWeWy ) return ErrorWeWy;
   if (PiszGrupa(72, t72) == ErrorWeWy) return ErrorWeWy;
   if (PiszGrupa(73, t73) == ErrorWeWy) return ErrorWeWy;  //always zero

   code123i = t->obiektt1 * 100 + t->obiektt2 * 10 + t->obiektt3;
   switch (code123i)
   {
   case 0:
	   break;
   case 10:
	   strncpy(code123, code999[4], 8);
	   if (PiszGrupa(999, code123) == ErrorWeWy) return ErrorWeWy;
	   break;
   default:
	   break;
   }
   
   if (PiszGrupa(100, (char*)"AcDbText") == ErrorWeWy) return ErrorWeWy;
  return OkWeWy;
}

char *replace_lncr(char *mtext)
{
    int i, l;
    static char new_mtext[2048+1];

    strcpy(new_mtext,"");

    l = (int)strlen(mtext);
    for (i = 0; i < l; i++)
    {
        if (mtext[i] == '\r') { ; }
        else if (mtext[i] == '\n') strncat(new_mtext, "\\P", 2);
        else strncat(new_mtext, &mtext[i], 1);
    }
    return new_mtext;
}

int PISZ_OBJECTS::RysujMText(TEXT* t)
{
	char text_lat[MaxMultitextLen + 1];
	double t_x, t_y;
	double x01, y01;
	double tdl, tdl1, tsin, tcos;
	int t71;
	double t44;
	char *text_ptr;
	char text_250[260];
	int p249;
	double text_box_width;
	TEXT ptr_t;
	char* ptr_n;
	double text_box_width_margin = 1.1;

    double t_x1, t_y1;
    double kos0;
    double koc0;


    if (t->ukryty == TRUE) return OkWeWy;
	x01 = t->x;
	y01 = t->y;
	//jezeli justowanie jest rozne od j_do_lewej nalezy wyznaczyc nowy punkt t_x,t_y
	
	GetTextLen(t, &tdl);

	text_box_width = tdl;

	memcpy(&ptr_t, t, sizeof(TEXT));
	ptr_n = strchr(ptr_t.text, '\n');
	while (ptr_n != NULL)
	{
		memcpy(ptr_t.text, ptr_n + 1, strlen(ptr_n+1)+1);
		ptr_t.width = 0;
		ptr_t.height = 0;
		GetTextLen(&ptr_t, &tdl1);
		if (tdl1 > text_box_width) text_box_width = tdl1;
		ptr_n = strchr(ptr_t.text, '\n');
	}

	tsin = sin(t->kat);
	tcos = cos(t->kat);


	t_x = x01 - t->wysokosc * tsin;
	t_y = y01 + t->wysokosc * tcos;

	switch (t->justowanie)
	{
	case 0: //to the left

		t71 = 1;
		break;
	case 1: //to the right

		t71 = 3;
		break;
	case 2: //srodkowo DXF Central

		t71 = 2;
		break;
	case 3:  //centralnie  -  DXF Middle

		t71 = 2;
		break;
	}
	
	if (PiszGrupa(0, (char*)"MTEXT") == ErrorWeWy ||
		PiszGrupa(5, handle_hex) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
		PiszGrupa(8, layer_dxf(Layers[t->warstwa].name, t->warstwa)) == ErrorWeWy) return ErrorWeWy;

	if (t->kolor != CBYLAYER &&
		PiszGrupa(62, (int)color_dxf(t->kolor)) == ErrorWeWy) return ErrorWeWy;

	if (PiszGrupa(370, -1) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbMText") == ErrorWeWy) return ErrorWeWy;
	
	if (PiszGrupa(10, (double)(t_x)) == ErrorWeWy ||  //t_x
		PiszGrupa(20, (double)(t_y)) == ErrorWeWy ||   //t_y
		PiszGrupa(30, 0.0) == ErrorWeWy ||
		PiszGrupa(40, (double)(t->wysokosc)) == ErrorWeWy) return ErrorWeWy;

	if (PiszGrupa(41, text_box_width* text_box_width_margin) == ErrorWeWy ||
		PiszGrupa(42, text_box_width* text_box_width_margin) == ErrorWeWy) return ErrorWeWy;

	if (PiszGrupa(71, t71) == ErrorWeWy ||
		PiszGrupa(72, 1) == ErrorWeWy) return ErrorWeWy;

	strcpy(text_lat, t->text);

	text_ptr = text_lat;
    text_ptr=replace_lncr(text_ptr);

	encodingdxf(text_ptr);
	
	while (strlen(text_ptr) > 249)
	{
		if ((text_ptr[248] == '\\') || (text_ptr[248]>127)) p249 = 248;  //  "........\", "P....."
		else if ((text_ptr[247] == '\\') || (text_ptr[247]>127)) p249 = 247;   //  "........\P", "......"
		else p249 = 249;

		strncpy(text_250, text_ptr, p249);
		text_250[p249] = '\0';
		if (PiszGrupa(3, (char *)(text_250)) == ErrorWeWy) return ErrorWeWy;
		text_ptr += p249;
	}

	if (t->width_factor != 1.0) sprintf(text_250, "\\W%.4fx;", t->width_factor);
	else strcpy(text_250, "");

	strcat(text_250, text_ptr);

	if (PiszGrupa(1, (char *)(text_250)) == ErrorWeWy) return ErrorWeWy;

    /*center point*//*rotate point*/
    kos0 = sin (t->kat) ;
    koc0 = cos (t->kat) ;
    Rotate_Point( kos0, koc0, 0, 0, 1.0, 0.0, &t_x1, &t_y1);
    if (PiszGrupa(11, (double)(t_x1)) == ErrorWeWy ||  //t_x
        PiszGrupa(21, (double)(t_y1)) == ErrorWeWy ||   //t_y
        PiszGrupa(31, 0.0) == ErrorWeWy) return ErrorWeWy;

	if (PiszGrupa(7, PTRS__Text_Style[t->czcionka] != NULL &&
			PTRS__Text_Style[t->czcionka]->font_acad_name[0] != '\0' ?
			PTRS__Text_Style[t->czcionka]->font_name : STANDARD) == ErrorWeWy ||
		PiszGrupa(210, 0) == ErrorWeWy ||
		PiszGrupa(220, 0) == ErrorWeWy ||
		PiszGrupa(230, 1) == ErrorWeWy)  return ErrorWeWy;


	if (PiszGrupa(73, 2) == ErrorWeWy) return ErrorWeWy;
	t44 = t->spacing / 3.0; //6.25;
	if (t44 < 0.25) t44 = 0.25;
	if (t44 > 4.0) t44 = 4.0;
	if (PiszGrupa(44, t44) == ErrorWeWy) return ErrorWeWy;
	return OkWeWy;
}


int PISZ_OBJECTS::RysujKolo(OKRAG* k)
{
	char code123[10];
	int code123i;

	if(PiszGrupa(0,(char*)POLYLINE)==ErrorWeWy ||
	PiszGrupa(5, handle_hex) == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
     PiszGrupa(8, layer_dxf (Layers[k->warstwa].name, k->warstwa))==ErrorWeWy) return ErrorWeWy;
  if(k->kolor!=CBYLAYER &&
   PiszGrupa(62,(int)color_dxf (k->kolor))==ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(100, (char*)"AcDb2dPolyline") == ErrorWeWy) return ErrorWeWy;
  if(PiszGrupa(66,1)==ErrorWeWy     ||
     PiszGrupa(70,1)==ErrorWeWy     ||
     PiszGrupa(40,k->r)==ErrorWeWy  ||
     PiszGrupa(41,k->r)==ErrorWeWy) return ErrorWeWy;
  if(PiszGrupa(0,(char*)VERTEX)==ErrorWeWy ||
	  PiszGrupa(5, handle_hex) == ErrorWeWy ||
	  PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
	  PiszGrupa(100, (char*)"AcDbVertex") == ErrorWeWy ||
     PiszGrupa(8, layer_dxf (Layers[k->warstwa].name, k->warstwa))==ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(100, (char*)"AcDb2dVertex") == ErrorWeWy) return ErrorWeWy;
  if(k->kolor!=CBYLAYER &&
   PiszGrupa(62,(int)color_dxf (k->kolor))==ErrorWeWy) return ErrorWeWy;
  if(PiszGrupa(10,(double)(k->x-k->r*.5))==ErrorWeWy  ||
     PiszGrupa(20,(double)(k->y))==ErrorWeWy  ||
     PiszGrupa(30,0.0)==ErrorWeWy   ||
     PiszGrupa(42,1.0)==ErrorWeWy ) return ErrorWeWy;
  if(PiszGrupa(0,(char*)VERTEX)==ErrorWeWy ||
	  PiszGrupa(5, handle_hex) == ErrorWeWy ||
	  PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
	  PiszGrupa(100, (char*)"AcDbVertex") == ErrorWeWy||
     PiszGrupa(8, layer_dxf (Layers[k->warstwa].name, k->warstwa))==ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(100, (char*)"AcDb2dVertex") == ErrorWeWy) return ErrorWeWy;
  if(k->kolor!=CBYLAYER &&
   PiszGrupa(62,(int)color_dxf (k->kolor))==ErrorWeWy) return ErrorWeWy;
  if(PiszGrupa(10,(double)(k->x+k->r*.5))==ErrorWeWy  ||
     PiszGrupa(20,(double)(k->y))==ErrorWeWy  ||
     PiszGrupa(30,0.0)==ErrorWeWy   ||
     PiszGrupa(42,1.0)==ErrorWeWy ) return ErrorWeWy;
  code123i = k->obiektt1 * 100 + k->obiektt2 * 10 + k->obiektt3;
  switch (code123i)
  {
  case 0:
	  break;
  case 10:
	  strncpy(code123, code999[3], 8);
	  if (PiszGrupa(999, code123) == ErrorWeWy) return ErrorWeWy;
	  break;
  default:
	  break;
  }
  if(PiszGrupa(0,(char*)SEQEND)==ErrorWeWy ||
	  PiszGrupa(5, handle_hex) == ErrorWeWy ||
	  PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy) return ErrorWeWy;
  return OkWeWy;
}


int PISZ_OBJECTS::RysujTransKolo(OKRAG* k, int translucency)
{
    char code123[10];
    int code123i;

    if(PiszGrupa(0,(char*)POLYLINE)==ErrorWeWy ||
       PiszGrupa(5, handle_hex) == ErrorWeWy ||
       PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
       PiszGrupa(8, layer_dxf (Layers[k->warstwa].name, k->warstwa))==ErrorWeWy) return ErrorWeWy;
    if(k->kolor!=CBYLAYER &&
       PiszGrupa(62,(int)color_dxf (k->kolor))==ErrorWeWy) return ErrorWeWy;

    if (PiszGrupa(440,(int)translucency)==ErrorWeWy) return ErrorWeWy;

    if (PiszGrupa(100, (char*)"AcDb2dPolyline") == ErrorWeWy) return ErrorWeWy;
    if(PiszGrupa(66,1)==ErrorWeWy     ||
       PiszGrupa(70,1)==ErrorWeWy     ||
       PiszGrupa(40,k->r)==ErrorWeWy  ||
       PiszGrupa(41,k->r)==ErrorWeWy) return ErrorWeWy;
    if(PiszGrupa(0,(char*)VERTEX)==ErrorWeWy ||
       PiszGrupa(5, handle_hex) == ErrorWeWy ||
       PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
       PiszGrupa(100, (char*)"AcDbVertex") == ErrorWeWy ||
       PiszGrupa(8, layer_dxf (Layers[k->warstwa].name, k->warstwa))==ErrorWeWy) return ErrorWeWy;
    if (PiszGrupa(100, (char*)"AcDb2dVertex") == ErrorWeWy) return ErrorWeWy;
    if(k->kolor!=CBYLAYER &&
       PiszGrupa(62,(int)color_dxf (k->kolor))==ErrorWeWy) return ErrorWeWy;

    if (PiszGrupa(440,(int)translucency)==ErrorWeWy) return ErrorWeWy;

    if(PiszGrupa(10,(double)(k->x-k->r*.5))==ErrorWeWy  ||
       PiszGrupa(20,(double)(k->y))==ErrorWeWy  ||
       PiszGrupa(30,0.0)==ErrorWeWy   ||
       PiszGrupa(42,1.0)==ErrorWeWy ) return ErrorWeWy;
    if(PiszGrupa(0,(char*)VERTEX)==ErrorWeWy ||
       PiszGrupa(5, handle_hex) == ErrorWeWy ||
       PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
       PiszGrupa(100, (char*)"AcDbVertex") == ErrorWeWy||
       PiszGrupa(8, layer_dxf (Layers[k->warstwa].name, k->warstwa))==ErrorWeWy) return ErrorWeWy;
    if (PiszGrupa(100, (char*)"AcDb2dVertex") == ErrorWeWy) return ErrorWeWy;
    if(k->kolor!=CBYLAYER &&
       PiszGrupa(62,(int)color_dxf (k->kolor))==ErrorWeWy) return ErrorWeWy;

    if (PiszGrupa(440,(int)translucency)==ErrorWeWy) return ErrorWeWy;

    if(PiszGrupa(10,(double)(k->x+k->r*.5))==ErrorWeWy  ||
       PiszGrupa(20,(double)(k->y))==ErrorWeWy  ||
       PiszGrupa(30,0.0)==ErrorWeWy   ||
       PiszGrupa(42,1.0)==ErrorWeWy ) return ErrorWeWy;
    code123i = k->obiektt1 * 100 + k->obiektt2 * 10 + k->obiektt3;
    switch (code123i)
    {
        case 0:
            break;
        case 10:
            strncpy(code123, code999[3], 8);
            if (PiszGrupa(999, code123) == ErrorWeWy) return ErrorWeWy;
            break;
        default:
            break;
    }
    if(PiszGrupa(0,(char*)SEQEND)==ErrorWeWy ||
       PiszGrupa(5, handle_hex) == ErrorWeWy ||
       PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy) return ErrorWeWy;
    return OkWeWy;
}

int PISZ_OBJECTS::RysujOkrag(OKRAG*o)
{ if(PiszGrupa(0,(char*)CIRCLE)==ErrorWeWy ||
	PiszGrupa(5, handle_hex) == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
     PiszGrupa(8, layer_dxf (Layers[o->warstwa].name, o->warstwa))==ErrorWeWy) return ErrorWeWy;
  if(typ_linii(o->typ)!=TBYLAYER &&
   PiszGrupa(6,TypyLinii[typ_linii(o->typ)].nazwa)==ErrorWeWy) return ErrorWeWy;
  if(o->kolor!=CBYLAYER &&
   PiszGrupa(62,(int)color_dxf (o->kolor))==ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(100, (char*)"AcDbCircle") == ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(370, (int)get_width370(o->typ)) == ErrorWeWy) return ErrorWeWy;
  if (get_width_invisible(o->typ))
  {
	  if (PiszGrupa(999, (char*)"invisible") == ErrorWeWy) return ErrorWeWy;
  }
  if(PiszGrupa(10,(double)(o->x))==ErrorWeWy  ||
     PiszGrupa(20,(double)(o->y))==ErrorWeWy  ||
     PiszGrupa(30,0.0)==ErrorWeWy	      ||
     PiszGrupa(40,(double)(o->r))==ErrorWeWy ) return ErrorWeWy;
  else return OkWeWy;
}

int PISZ_OBJECTS::RysujEllipse(ELLIPSE *e)
{   double x1, y1;
    double kos, koc;
    double eratio, eangle;
    double erx, ery;

    if (Check_if_Equal(e->rx, 0.0)) return OkWeWy;
    if(PiszGrupa(0,(char*)ELLIPSEDXF)==ErrorWeWy ||
     PiszGrupa(5, handle_hex) == ErrorWeWy ||
     PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
     PiszGrupa(8, layer_dxf (Layers[e->warstwa].name, e->warstwa))==ErrorWeWy) return ErrorWeWy;
    if(typ_linii(e->typ)!=TBYLAYER &&
       PiszGrupa(6,TypyLinii[typ_linii(e->typ)].nazwa)==ErrorWeWy) return ErrorWeWy;
    if(e->kolor!=CBYLAYER &&
       PiszGrupa(62,(int)color_dxf (e->kolor))==ErrorWeWy) return ErrorWeWy;
    if (PiszGrupa(100, (char*)"AcDbEllipse") == ErrorWeWy) return ErrorWeWy;
    if (PiszGrupa(370, (int)get_width370(e->typ)) == ErrorWeWy) return ErrorWeWy;
    if (get_width_invisible(e->typ))
    {
        if (PiszGrupa(999, (char*)"invisible") == ErrorWeWy) return ErrorWeWy;
    }

    if (e->rx>=e->ry) {
        kos = sin(e->angle);
        koc = cos(e->angle);
        Rotate_Point(kos, koc, 0, 0, e->rx, 0, &x1, &y1);
        eratio = e->ry / e->rx;
    }
    else
    {
        eangle=Angle_Normal(e->angle+Pi/2.0);
        kos = sin(eangle);
        koc = cos(eangle);
        erx=e->ry;
        ery=e->rx;
        Rotate_Point(kos, koc, 0, 0, erx, 0, &x1, &y1);
        eratio = ery / erx;
    }


    if(PiszGrupa(10,(double)(e->x))==ErrorWeWy  ||
       PiszGrupa(20,(double)(e->y))==ErrorWeWy  ||
       PiszGrupa(30,0.0)==ErrorWeWy) return ErrorWeWy;

    if(PiszGrupa(11,(double)(x1))==ErrorWeWy  ||
       PiszGrupa(21,(double)(y1))==ErrorWeWy  ||
       PiszGrupa(31,0.0)==ErrorWeWy) return ErrorWeWy;

    if(PiszGrupa(210,(double)(0.0))==ErrorWeWy  ||
       PiszGrupa(220,(double)(0.0))==ErrorWeWy  ||
       PiszGrupa(230,(double)(1.0))==ErrorWeWy) return ErrorWeWy;

    if (PiszGrupa(40,(double)(eratio))==ErrorWeWy ) return ErrorWeWy;
    if (PiszGrupa(41,(double)(0.0))==ErrorWeWy ) return ErrorWeWy;
    if (PiszGrupa(42,(double)(Pi2))==ErrorWeWy ) return ErrorWeWy;
    else return OkWeWy;
}


int PISZ_OBJECTS::RysujFilledEllipse(ELLIPSE *fe)
{   double x1, y1;
    double kos, koc;
    double eratio, eangle;
    double erx, ery;

    if (Check_if_Equal(fe->rx, 0.0)) return OkWeWy;

    if(PiszGrupa(0,(char*)HATCH)==ErrorWeWy ||
       PiszGrupa(5, handle_hex) == ErrorWeWy ||
       PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy) return ErrorWeWy;

    if(PiszGrupa(330,(int)2)==ErrorWeWy ||
      PiszGrupa(100, (char*)"AcDbEllipse") == ErrorWeWy ||
      PiszGrupa(8, layer_dxf (Layers[fe->warstwa].name, fe->warstwa))==ErrorWeWy) return ErrorWeWy;

    if(fe->kolor!=CBYLAYER && PiszGrupa(62,(int)color_dxf (fe->kolor))==ErrorWeWy) return ErrorWeWy;

    if (PiszGrupa(440,(int)fe->translucency)==ErrorWeWy) return ErrorWeWy;

    if ( PiszGrupa(100, (char*)"AcDbChatch") == ErrorWeWy) return ErrorWeWy;

    if(PiszGrupa(10,0.0)==ErrorWeWy  ||
       PiszGrupa(20,0.0)==ErrorWeWy  ||
       PiszGrupa(30,0.0)==ErrorWeWy) return ErrorWeWy;

    if(PiszGrupa(210,0.0)==ErrorWeWy  ||
       PiszGrupa(220,0.0)==ErrorWeWy  ||
       PiszGrupa(230,1.0)==ErrorWeWy) return ErrorWeWy;

    if(PiszGrupa(2,(char*)SOLID)==ErrorWeWy ||
       PiszGrupa(70, (int)1) == ErrorWeWy ||
       PiszGrupa(71, (int)1) == ErrorWeWy ||
       PiszGrupa(91, (int)1) == ErrorWeWy ||
       PiszGrupa(92, (int)5) == ErrorWeWy ||
       PiszGrupa(93, (int)1) == ErrorWeWy ||
       PiszGrupa(72, (int)3) == ErrorWeWy) return ErrorWeWy;

    if (fe->rx>=fe->ry) {
        kos = sin(fe->angle);
        koc = cos(fe->angle);
        Rotate_Point(kos, koc, 0, 0, fe->rx, 0, &x1, &y1);
        eratio = fe->ry / fe->rx;
    }
    else
    {
        eangle=Angle_Normal(fe->angle+Pi/2.0);
        kos = sin(eangle);
        koc = cos(eangle);
        erx=fe->ry;
        ery=fe->rx;
        Rotate_Point(kos, koc, 0, 0, erx, 0, &x1, &y1);
        eratio = ery / erx;
    }

    if(PiszGrupa(10,(double)(fe->x))==ErrorWeWy  ||
       PiszGrupa(20,(double)(fe->y))==ErrorWeWy  ||
       PiszGrupa(11,(double)x1)==ErrorWeWy  ||
       PiszGrupa(21,(double)y1)==ErrorWeWy) return ErrorWeWy;

    if (PiszGrupa(40,(double)(eratio))==ErrorWeWy ) return ErrorWeWy;
    if (PiszGrupa(50,(double)(0.0))==ErrorWeWy ) return ErrorWeWy;
    if (PiszGrupa(51,(double)(360.0))==ErrorWeWy ) return ErrorWeWy;

    if (PiszGrupa(73,1)==ErrorWeWy ||
        PiszGrupa(75,1)==ErrorWeWy ||
        PiszGrupa(76,1)==ErrorWeWy ||
        PiszGrupa(47,(double)0.794178)==ErrorWeWy ||   ////????
        PiszGrupa(98,1)==ErrorWeWy) return ErrorWeWy;

    if(PiszGrupa(10,(double)(fe->x))==ErrorWeWy  ||
       PiszGrupa(20,(double)(fe->y))==ErrorWeWy) return ErrorWeWy;

    if(PiszGrupa(450,0)==ErrorWeWy ||
       PiszGrupa(451,0)==ErrorWeWy ||
       PiszGrupa(460,0.0)==ErrorWeWy) return ErrorWeWy;
     if(PiszGrupa(453,0)==ErrorWeWy) return ErrorWeWy;

    return OkWeWy;
}

int PISZ_OBJECTS::RysujEllipticalArc(ELLIPTICALARC *ea)
{   double x1, y1;
    double kos, koc, kos_, koc_;
    double eratio, eangle;
    double erx, ery;
    double kat1, kat2, par1, par2;
    double p1, p2;
    double df_x1_, df_y1_, df_x2_, df_y2_, df_x1, df_y1, df_x2, df_y2;

    double vx1, vy1, vx2, vy2;
    double dot,det;

    if (Check_if_Equal(ea->rx, 0.0)) return OkWeWy;
    if(PiszGrupa(0,(char*)ELLIPSEDXF)==ErrorWeWy ||
       PiszGrupa(5, handle_hex) == ErrorWeWy ||
       PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
       PiszGrupa(8, layer_dxf (Layers[ea->warstwa].name, ea->warstwa))==ErrorWeWy) return ErrorWeWy;
    if(typ_linii(ea->typ)!=TBYLAYER &&
       PiszGrupa(6,TypyLinii[typ_linii(ea->typ)].nazwa)==ErrorWeWy) return ErrorWeWy;
    if(ea->kolor!=CBYLAYER &&
       PiszGrupa(62,(int)color_dxf (ea->kolor))==ErrorWeWy) return ErrorWeWy;
    if (PiszGrupa(100, (char*)"AcDbEllipse") == ErrorWeWy) return ErrorWeWy;
    if (PiszGrupa(370, (int)get_width370(ea->typ)) == ErrorWeWy) return ErrorWeWy;
    if (get_width_invisible(ea->typ))
    {
        if (PiszGrupa(999, (char*)"invisible") == ErrorWeWy) return ErrorWeWy;
    }

    if (ea->rx>=ea->ry) {
        kos = sin(ea->angle);
        koc = cos(ea->angle);
        kos_ = sin(-ea->angle);
        koc_ = cos(-ea->angle);
        erx=ea->rx;
        ery=ea->ry;
        Rotate_Point(kos, koc, 0, 0, ea->rx, 0, &x1, &y1);
        eratio = ea->ry / ea->rx;
        kat1=Angle_Normal(ea->kat1);
        kat2=Angle_Normal(ea->kat2);
    }
    else
    {
        eangle=Angle_Normal(ea->angle+Pi/2.0);
        kos = sin(eangle);
        koc = cos(eangle);
        kos_ = sin(eangle);
        koc_ = cos(eangle);
        erx=ea->ry;
        ery=ea->rx;
        Rotate_Point(kos, koc, 0, 0, erx, 0, &x1, &y1);
        eratio = ery / erx;
        kat1=Angle_Normal(ea->kat1+Pi/2.0);
        kat2=Angle_Normal(ea->kat2+Pi/2.0);
    }


    Elliptical_Arc_Ends (ea, &df_x1, &df_y1, &df_x2, &df_y2);
    df_x1-=ea->x;
    df_y1-=ea->y;
    df_x2-=ea->x;
    df_y2-=ea->y;
    Rotate_Point(kos_, koc_, 0, 0, df_x1, df_y1, &df_x1_, &df_y1_);
    Rotate_Point(kos_, koc_, 0, 0, df_x2, df_y2, &df_x2_, &df_y2_);


    //par 1
    if (Check_if_Equal(kat1, 0.0)) p1=0.0;
    else if (Check_if_Equal(kat1, Pi)) p1=Pi;
    else {
        vx1 = df_x1_;
        vy1 = sqrt((ea->rx) * (ea->rx) - df_x1_ * df_x1_);
        if (df_y1 < 0) vy1 *= (-1);
        vx2 = 1.0;
        vy2 = 0.0;

        dot = vx1 * vx2 + vy1 * vy2;  // Dot product between [vx1, vy1] and [vx2, vy2]
        det = vx1 * vy2 - vy1 * vx2;  // Determinant
        p1 = -atan2(det, dot);  // atan2(y, x) or atan2(sin, cos)
        p1 = fmod(p1, Pi2);
        if (p1 < 0) p1 += Pi2;
    }

    //par 2
    if (Check_if_Equal(kat2, 0.0)) p2=0.0;
    else if (Check_if_Equal(kat2, Pi)) p2=Pi;
    else {
        vx1 = df_x2_;
        vy1 = sqrt((ea->rx) * (ea->rx) - df_x2_ * df_x2_);
        if (df_y2 < 0) vy1 *= (-1);
        vx2 = 1.0;
        vy2 = 0.0;

        dot = vx1 * vx2 + vy1 * vy2;  // Dot product between [vx1, vy1] and [vx2, vy2]
        det = vx1 * vy2 - vy1 * vx2;  // Determinant
        p2 = -atan2(det, dot);  // atan2(y, x) or atan2(sin, cos)
        p2 = fmod(p2, Pi2);
        if (p2 < 0) p2 += Pi2;
    }


    if(PiszGrupa(10,(double)(ea->x))==ErrorWeWy  ||
       PiszGrupa(20,(double)(ea->y))==ErrorWeWy  ||
       PiszGrupa(30,0.0)==ErrorWeWy) return ErrorWeWy;

    if(PiszGrupa(11,(double)(x1))==ErrorWeWy  ||
       PiszGrupa(21,(double)(y1))==ErrorWeWy  ||
       PiszGrupa(31,0.0)==ErrorWeWy) return ErrorWeWy;

    if(PiszGrupa(210,(double)(0.0))==ErrorWeWy  ||
       PiszGrupa(220,(double)(0.0))==ErrorWeWy  ||
       PiszGrupa(230,(double)(1.0))==ErrorWeWy) return ErrorWeWy;

    if (PiszGrupa(40,(double)(eratio))==ErrorWeWy ) return ErrorWeWy;
    if (PiszGrupa(41,(double)(p1))==ErrorWeWy ) return ErrorWeWy;
    if (PiszGrupa(42,(double)(p2))==ErrorWeWy ) return ErrorWeWy;
    else return OkWeWy;
}

int PISZ_OBJECTS::RysujLuk(LUK* l)
{
	char code123[10];
	int code123i;

	if(PiszGrupa(0,(char*)ARC)==ErrorWeWy ||
	PiszGrupa(5, handle_hex) == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
     PiszGrupa(8, layer_dxf (Layers[l->warstwa].name, l->warstwa))==ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(100, (char*)"AcDbCircle") == ErrorWeWy) return ErrorWeWy;
  if(typ_linii(l->typ)!=TBYLAYER &&
   PiszGrupa(6,TypyLinii[typ_linii(l->typ)].nazwa)==ErrorWeWy) return ErrorWeWy;
  if(l->kolor!=CBYLAYER &&
   PiszGrupa(62,(int)color_dxf (l->kolor))==ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(370, (int)get_width370(l->typ)) == ErrorWeWy) return ErrorWeWy;
  if (get_width_invisible(l->typ))
  {
	  if (PiszGrupa(999, (char*)"invisible") == ErrorWeWy) return ErrorWeWy;
  }
  if(PiszGrupa(10,(double)(l->x))==ErrorWeWy  ||
     PiszGrupa(20,(double)(l->y))==ErrorWeWy  ||
     PiszGrupa(30,0.0)==ErrorWeWy	      ||
     PiszGrupa(40,(double)(l->r))==ErrorWeWy ) return ErrorWeWy;

  code123i = l->obiektt1 * 100 + l->obiektt2 * 10 + l->obiektt3;
  switch (code123i)
  {
  case 0:
	  break;
  case 10:
	  strncpy(code123, code999[1], 8);
	  if (PiszGrupa(999, code123) == ErrorWeWy) return ErrorWeWy;
	  break;
  default:
	  break;
  }

	 if (PiszGrupa(100, (char*)"AcDbArc") == ErrorWeWy) return ErrorWeWy;
     if (PiszGrupa(50,(double)(l->kat1)*180/Pi)==ErrorWeWy  ||
     PiszGrupa(51,(double)(l->kat2)*180/Pi)==ErrorWeWy ) return ErrorWeWy;
  else return OkWeWy;
}


int PISZ_OBJECTS::RysujSolidArc(SOLIDARC* sa)
{
	char code123[10];
	int code123i;
    double width1, width2;
    double x1, y1, x2, y2, kat1, kat2, dkat, bulge;

	if(PiszGrupa(0,(char*)LWPOLYLINE)==ErrorWeWy ||
	PiszGrupa(5, handle_hex) == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
    PiszGrupa(8, layer_dxf (Layers[sa->warstwa].name, sa->warstwa))==ErrorWeWy) return ErrorWeWy;
    if(sa->kolor!=CBYLAYER && PiszGrupa(62,(int)color_dxf (sa->kolor))==ErrorWeWy) return ErrorWeWy;

    if ((sa->empty_typ == 0) && (sa->pattern == 0) && (sa->translucent == 1))
    {
        if (PiszGrupa(440, (int)sa->translucency) == ErrorWeWy) return ErrorWeWy;
    }
    if (PiszGrupa(100, (char*)"AcDbPolyline") == ErrorWeWy) return ErrorWeWy;

    if (PiszGrupa(370, 0) == ErrorWeWy) return ErrorWeWy;  //TEMPORARY, if sa->empty_typ>0

    //converting solidarc to polyline arc
    SolidArc_To_Points (sa, &x1, &y1, &x2, &y2);
    kat1=sa->kat1;
    kat2=sa->kat2;
    if (kat2<kat1) kat2+=Pi2;
    dkat=kat2-kat1;
    bulge=tan(dkat/4);

        width1 = sa->width2;
        width2 = sa->width1;

    if(PiszGrupa(10,x1)==ErrorWeWy  ||
     PiszGrupa(20,y1)==ErrorWeWy  ||
     PiszGrupa(40,width1)==ErrorWeWy  ||
     PiszGrupa(41,width2)==ErrorWeWy  ||
     PiszGrupa(42,bulge)==ErrorWeWy  ||
     PiszGrupa(10,x2)==ErrorWeWy	      ||
     PiszGrupa(20,y2)==ErrorWeWy ) return ErrorWeWy;
  else return OkWeWy;
}



int PISZ_OBJECTS::RysujSolid(WIELOKAT * s)
{
	char code123[10];
	char code123i;
	unsigned char translucency;
	char* translucency_ptr;

	if(s->lp!=6 && s->lp!=8) return OkWeWy;
  if(PiszGrupa(0,(char*)SOLID)==ErrorWeWy ||
	  PiszGrupa(5, handle_hex) == ErrorWeWy ||
	  PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
     PiszGrupa(8, layer_dxf (Layers[s->warstwa].name, s->warstwa))==ErrorWeWy) return ErrorWeWy;
  if(s->kolor!=CBYLAYER &&
   PiszGrupa(62,(int)color_dxf (s->kolor))==ErrorWeWy) return ErrorWeWy;
  if ((s->empty_typ == 0) && (s->pattern == 0) && (s->translucent == 1))
  {
	  translucency_ptr = (char *)s->xy;
	  translucency_ptr += (s->lp * sizeof(float));
	  memmove(&translucency, translucency_ptr, sizeof(unsigned char));

	  if (PiszGrupa(440, (int)translucency) == ErrorWeWy) return ErrorWeWy;
  }
  if (PiszGrupa(100, (char*)"AcDbTrace") == ErrorWeWy) return ErrorWeWy;

  if (PiszGrupa(370, 0) == ErrorWeWy) return ErrorWeWy;  //TEMPORARY, if s->empty_typ>0

  if(PiszGrupa(10,(double)(s->xy[0]))==ErrorWeWy  ||
     PiszGrupa(20,(double)(s->xy[1]))==ErrorWeWy  ||
     PiszGrupa(30,0.0)==ErrorWeWy       	  ||
     PiszGrupa(11,(double)(s->xy[2]))==ErrorWeWy  ||
     PiszGrupa(21,(double)(s->xy[3]))==ErrorWeWy  ||
     PiszGrupa(31,0.0)==ErrorWeWy)  return ErrorWeWy;
  if(s->lp==8)
  {
   if (PiszGrupa(12,(double)(s->xy[6]))==ErrorWeWy  ||
     PiszGrupa(22,(double)(s->xy[7]))==ErrorWeWy  ||
     PiszGrupa(32,0.0)==ErrorWeWy 		  ||
     PiszGrupa(13,(double)(s->xy[4]))==ErrorWeWy  ||
     PiszGrupa(23,(double)(s->xy[5]))==ErrorWeWy  ||
     PiszGrupa(33,0.0)==ErrorWeWy ) return ErrorWeWy;
  }
  else
  {
    if(PiszGrupa(12,(double)(s->xy[4]))==ErrorWeWy     	||
	  PiszGrupa(22,(double)(s->xy[5]))==ErrorWeWy  	||
	  PiszGrupa(32,0.0)==ErrorWeWy 		       	||
     PiszGrupa(13,(double)(s->xy[4]))==ErrorWeWy       	||
     PiszGrupa(23,(double)(s->xy[5]))==ErrorWeWy  	||
     PiszGrupa(33,0.0)==ErrorWeWy ) return ErrorWeWy;
  }
  code123i = s->obiektt1 * 100 + s->obiektt2 * 10 + s->obiektt3;
  switch (code123i)
  {
  case 0:
	  break;
  case 10:
	  strncpy(code123, code999[3], 8);
	  if (PiszGrupa(999, code123) == ErrorWeWy) return ErrorWeWy;
	  break;
  default:
	  break;
  }
  return OkWeWy;
}

int PISZ_OBJECTS::RysujSolid3D(WIELOKAT * s)
{ if(s->lp!=6 && s->lp!=8) return OkWeWy;
  if(PiszGrupa(0,(char*)SOLID)==ErrorWeWy ||
	  PiszGrupa(5, handle_hex) == ErrorWeWy ||
	  PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
     PiszGrupa(8, layer_dxf (Layers[s->warstwa].name, s->warstwa))==ErrorWeWy) return ErrorWeWy;
  if(s->kolor!=CBYLAYER &&
   PiszGrupa(62,(int)color_dxf (s->kolor))==ErrorWeWy) return ErrorWeWy;
  if (PiszGrupa(100, (char*)"AcDb3dSolid") == ErrorWeWy) return ErrorWeWy;
  if(PiszGrupa(10,(double)(s->xy[0]))==ErrorWeWy  ||
     PiszGrupa(20,(double)(s->xy[1]))==ErrorWeWy  ||
     PiszGrupa(30,(double)(s->xy[s->lp]))==ErrorWeWy  ||
     PiszGrupa(11,(double)(s->xy[2]))==ErrorWeWy  ||
     PiszGrupa(21,(double)(s->xy[3]))==ErrorWeWy  ||
     PiszGrupa(31,(double)(s->xy[s->lp+1]))==ErrorWeWy)  return ErrorWeWy;
  if(s->lp==8)
  {
   if (PiszGrupa(12,(double)(s->xy[6]))==ErrorWeWy  ||
       PiszGrupa(22,(double)(s->xy[7]))==ErrorWeWy  ||
       PiszGrupa(32,(double)(s->xy[s->lp+3]))==ErrorWeWy  ||
       PiszGrupa(13,(double)(s->xy[4]))==ErrorWeWy  ||
       PiszGrupa(23,(double)(s->xy[5]))==ErrorWeWy  ||
       PiszGrupa(33,(double)(s->xy[s->lp+2]))==ErrorWeWy ) return ErrorWeWy;
  }
  else
  {
    if(PiszGrupa(12,(double)(s->xy[4]))==ErrorWeWy  	||
       PiszGrupa(22,(double)(s->xy[5]))==ErrorWeWy  	||
	    PiszGrupa(32,(double)(s->xy[s->lp+2]))==ErrorWeWy ||
       PiszGrupa(13,(double)(s->xy[4]))==ErrorWeWy   	||
       PiszGrupa(23,(double)(s->xy[5]))==ErrorWeWy  	||
       PiszGrupa(33,(double)(s->xy[s->lp+2]))==ErrorWeWy ) return ErrorWeWy;
  }
  return OkWeWy;
}

int PISZ_OBJECTS::RysujSpline(SPLINE *s)
{
	int i,j;
	int spline_flag;
	int s_grade;
	double knot_d, knot_v;

	if (PiszGrupa(0, (char*)BSPLINE) == ErrorWeWy ||
		PiszGrupa(5, handle_hex) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
		PiszGrupa(8, layer_dxf(Layers[s->warstwa].name, s->warstwa)) == ErrorWeWy) return ErrorWeWy;
	if (s->kolor != CBYLAYER &&
		PiszGrupa(62, (int)color_dxf(s->kolor)) == ErrorWeWy) return ErrorWeWy;
	if (PiszGrupa(100, (char*)"AcDbSpline") == ErrorWeWy) return ErrorWeWy;
    if (PiszGrupa(370, (int)get_width370(s->typ)) == ErrorWeWy) return ErrorWeWy;
	if ((s->npts < 5) || (s->npts == 6))
	{
		if (s->closed == 1) if (PiszGrupa(70, 1) == ErrorWeWy) return ErrorWeWy;
		if (s->npts == 3) 
		{ 
			s_grade = 3;
			if (PiszGrupa(71, 2) == ErrorWeWy) return ErrorWeWy; 
		}
		else 
		{ 
			s_grade = 4;
			if (PiszGrupa(71, 3) == ErrorWeWy) return ErrorWeWy; 
		}
		if (PiszGrupa(72, ((int)s->lp / 2) + s_grade) == ErrorWeWy) return ErrorWeWy;
		if (PiszGrupa(73, (int)(s->lp / 2)) == ErrorWeWy) return ErrorWeWy;
		for (i = 0; i < (int)s->lp; i += 2)
		{
			if (PiszGrupa(10, (double)(s->xy[i])) == ErrorWeWy ||
				PiszGrupa(20, (double)(s->xy[i + 1])) == ErrorWeWy ||
				PiszGrupa(30, 0.0) == ErrorWeWy) return ErrorWeWy;
		}
		if (s->lp > 8) knot_d = 1.0 / ((s->lp / 2) - 3);
		else knot_d = 0.0;
		for (i = 0; i < (int)s->lp/2; i ++)
		{		
			if (i < 4) knot_v = 0;
			else knot_v = knot_d * (i - 3);
			if (PiszGrupa(40, knot_v) == ErrorWeWy) return ErrorWeWy;
		}
		for (j = 0; j < s_grade; j++)
		{
			if (PiszGrupa(40, 1) == ErrorWeWy) return ErrorWeWy;
		}
	}
	else //multipoints
	{
		if (s->closed == 1) spline_flag = 1025; else spline_flag = 1024;
		if (PiszGrupa(70, spline_flag) == ErrorWeWy) return ErrorWeWy;
		if (PiszGrupa(71, 3) == ErrorWeWy) return ErrorWeWy;
		if (PiszGrupa(74, (int)(s->lp / 2)) == ErrorWeWy) return ErrorWeWy;
		if (PiszGrupa(999, (double)(s->xy[s->lp])) == ErrorWeWy) return ErrorWeWy; //it's as custom comment defining curviness of spline
		for (i = 0; i < (int)s->lp; i += 2)
		{
			if (PiszGrupa(11, (double)(s->xy[i])) == ErrorWeWy ||
				PiszGrupa(21, (double)(s->xy[i + 1])) == ErrorWeWy ||
				PiszGrupa(31, 0.0) == ErrorWeWy) return ErrorWeWy;
		}

	}

	return OkWeWy;
}

int PISZ_OBJECTS::RysujPoint(T_Point *p, int *p_block_sufix)
{  char sufiks_bloku[10];
    OKRAG o=Odef;
    OKRAG k=Kdef;
    LINIA L=Ldef;
    double df_psize;
    double x1, y1, x2, y2, x12, y12;
    double dxy, dd, dd1 ;

    if (p->typ>1)
    {
        if (((p->typ > 7) && (PIN_IS_PRINTABLE)) || (p->typ == 7) || (p->typ > 11))
        {
            /*przyjecie nazwy domyslnej*/
            strcpy(ins_b.nazwa_bloku, "P");
            ins_b.x0 = p->x;
            ins_b.y0 = p->y;
            ins_b.z0 = 0;
            ins_b.invisible = 0;

            *p_block_sufix += 1;
            sprintf(sufiks_bloku, "%#ld", *p_block_sufix);

            if (strlen(ins_b.nazwa_bloku) > 27) ins_b.nazwa_bloku[26] = '\0';

            strcat(ins_b.nazwa_bloku, sufiks_bloku);
            /*KOREKTA NAZWY BLOKU*/
            encodingdxf((char *) &ins_b.nazwa_bloku);

            /*funkcja RecordBlock zapisuje blok w TABLES*/
            to_block = FALSE;
            to_entities = FALSE;
            if (RecordBlock(ins_b) == ErrorWeWy) return ErrorWeWy;

            /*funkcja IniBlock inicjuje blok w zbiorze blokow*/
            /*i dopisuje Insert do zbioru prymitywow*/
            to_entities = TRUE;
            if (InsBlock(ins_b) == ErrorWeWy) return ErrorWeWy;

            to_block = TRUE;
            to_entities = FALSE;
            if (IniBlock(ins_b) == ErrorWeWy) return ErrorWeWy;
            //point as a block

            if (p->typ < 8) df_psize = Get_Point_Size() / 2;
            else df_psize = Get_Point_Size() / 4;


            x1 = (p->x - df_psize);
            y1 = (p->y - df_psize);
            x2 = (p->x + df_psize);
            y2 = (p->y + df_psize);
            x12 = (x1 + x2) / 2;
            y12 = (y1 + y2) / 2;

            L.warstwa = o.warstwa = k.warstwa = p->warstwa;
            L.kolor = o.kolor = k.kolor = p->kolor;
            L.typ = o.typ = k.typ = 64;

            o.r = df_psize * 2;
            k.r = df_psize * 1.5;

            //actual point
            /*
            if (PiszGrupa(0, (char *) POINT) == ErrorWeWy ||
                PiszGrupa(5, handle_hex) == ErrorWeWy ||
                PiszGrupa(100, (char *) "AcDbEntity") == ErrorWeWy ||
                PiszGrupa(8, layer_dxf(Layers[p->warstwa].name, p->warstwa)) == ErrorWeWy)
                return ErrorWeWy;
            if (p->kolor != CBYLAYER &&
                PiszGrupa(62, (int) color_dxf(p->kolor)) == ErrorWeWy)
                return ErrorWeWy;
            if (PiszGrupa(100, (char *) "AcDbPoint") == ErrorWeWy) return ErrorWeWy;
            //if (PiszGrupa(370, (int)get_width370(p->typ)) == ErrorWeWy) return ErrorWeWy;
            if (PiszGrupa(10, (double) (p->x)) == ErrorWeWy ||
                PiszGrupa(20, (double) (p->y)) == ErrorWeWy ||
                PiszGrupa(30, 0.0) == ErrorWeWy)
                return ErrorWeWy;
            if (PiszGrupa(50, 0.0) == ErrorWeWy) return ErrorWeWy;
            */
            if (PiszGrupa(999, (int) p->typ) == ErrorWeWy) return ErrorWeWy;

            switch (p->typ) {
                //case 1:
                //    o.x=p->x;
                //    o.y=p->y;
                //    o.r = 2*df_psize/5.0;
                //    RysujOkrag(&o);
                //    break;
                case 2: //not existing
                case 3:
                case 4:
                case 5:
                case 6:
                    break;
                case 7: //junction
                    k.x = p->x;
                    k.y = p->y;
                    k.r = 2 * df_psize / 5.0;
                    RysujKolo(&k);
                    break;
                case 8: //pin
                    L.x1 = x1;
                    L.y1 = y1;
                    L.x2 = x2;
                    L.y2 = y2;
                    RysujLinia(&L);

                    o.x = p->x;
                    o.y = p->y;
                    o.r = 2 * df_psize / 3.0;
                    RysujOkrag(&o);
                    break;
                case 9:  //ping g
                    L.x1 = x12;
                    L.y1 = y12;
                    L.x2 = x12;
                    L.y2 = y2;
                    RysujLinia(&L);

                    L.x1 = x1;
                    L.y1 = y1;
                    L.x2 = x2;
                    L.y2 = y2;
                    RysujLinia(&L);

                    o.x = p->x;
                    o.y = p->y;
                    o.r = 2 * df_psize / 3.0;
                    RysujOkrag(&o);
                    break;
                case 10:  //pin d
                    L.x1 = x12;
                    L.y1 = y12;
                    L.x2 = x12;
                    L.y2 = y1;
                    RysujLinia(&L);

                    L.x1 = x1;
                    L.y1 = y1;
                    L.x2 = x2;
                    L.y2 = y2;
                    RysujLinia(&L);

                    o.x = p->x;
                    o.y = p->y;
                    o.r = 2 * df_psize / 3.0;
                    RysujOkrag(&o);
                    break;
                case 11:  //pin s
                    L.x1 = x1;
                    L.y1 = y12;
                    L.x2 = x2;
                    L.y2 = y12;
                    RysujLinia(&L);

                    L.x1 = x1;
                    L.y1 = y1;
                    L.x2 = x2;
                    L.y2 = y2;
                    RysujLinia(&L);

                    o.x = p->x;
                    o.y = p->y;
                    o.r = 2 * df_psize / 3.0;
                    RysujOkrag(&o);
                    break;
                case 12:
                case 13:
                case 14:
                case 15:
                    L.x1 = x12;
                    L.y1 = y1;
                    L.x2 = x12;
                    L.y2 = y2;
                    RysujLinia(&L);

                    L.x1 = x1;
                    L.y1 = y12;
                    L.x2 = x2;
                    L.y2 = y12;
                    RysujLinia(&L);

                    k.x = p->x;
                    k.y = p->y;
                    RysujKolo(&k);

                    L.typ = 128;
                    dxy = df_psize * 2.5;  //2
                    dd = df_psize * 1.5;  //*1.0
                    dd1 = df_psize;
                    switch (p->typ) {
                        case 12:
                            //x1 - dxy, y12 - dd, x2 + dxy, y12 - dd
                            L.x1 = x1 - dxy;
                            L.y1 = y12 - dd;
                            L.x2 = x2 + dxy;
                            L.y2 = y12 - dd;
                            break;
                        case 13:
                            //x12 - dd, y1 - dxy, x12 - dd, y2 + dxy
                            L.x1 = x12 - dd;
                            L.y1 = y1 - dxy;
                            L.x2 = x12 - dd;
                            L.y2 = y2 + dxy;
                            break;
                        case 14:
                            //x12 + dd, y1 - dxy, x12 + dd, y2 + dxy
                            L.x1 = x12 + dd;
                            L.y1 = y1 - dxy;
                            L.x2 = x12 + dd;
                            L.y2 = y2 + dxy;
                            break;
                        case 15:
                            //x1 - dxy, y12 + dd, x2 + dxy, y12 + dd
                            L.x1 = x1 - dxy;
                            L.y1 = y12 + dd;
                            L.x2 = x2 + dxy;
                            L.y2 = y12 + dd;
                            break;
                    }
                    RysujLinia(&L);
                    break;
                case 16:
                case 17:
                case 18:
                case 19:
                    L.x1 = x12;
                    L.y1 = y1;
                    L.x2 = x12;
                    L.y2 = y2;
                    RysujLinia(&L);

                    L.x1 = x1;
                    L.y1 = y12;
                    L.x2 = x2;
                    L.y2 = y12;
                    RysujLinia(&L);

                    o.x = p->x;
                    o.y = p->y;
                    RysujOkrag(&o);

                    L.typ = 128;
                    dxy = df_psize * 3;
                    dd = df_psize * 2;
                    dd1 = df_psize;
                    switch (p->typ) {
                        case 16:
                            //x1 - dxy, y12 - dd, x2 + dxy, y12 - dd
                            L.x1 = x1 - dxy;
                            L.y1 = y12 - dd;
                            L.x2 = x2 + dxy;
                            L.y2 = y12 - dd;
                            break;
                        case 17:
                            //x12 - dd, y1 - dxy, x12 - dd, y2 + dxy
                            L.x1 = x12 - dd;
                            L.y1 = y1 - dxy;
                            L.x2 = x12 - dd;
                            L.y2 = y2 + dxy;
                            break;
                        case 18:
                            //x12 + dd, y1 - dxy, x12 + dd, y2 + dxy
                            L.x1 = x12 + dd;
                            L.y1 = y1 - dxy;
                            L.x2 = x12 + dd;
                            L.y2 = y2 + dxy;
                            break;
                        case 19:
                            //x1 - dxy, y12 + dd, x2 + dxy, y12 + dd
                            L.x1 = x1 - dxy;
                            L.y1 = y12 + dd;
                            L.x2 = x2 + dxy;
                            L.y2 = y12 + dd;
                            break;
                    }
                    RysujLinia(&L);
                    break;
                case 20:
                case 21:
                case 22:
                case 23:
                    L.x1 = x12;
                    L.y1 = y1;
                    L.x2 = x12;
                    L.y2 = y2;
                    RysujLinia(&L);

                    L.x1 = x1;
                    L.y1 = y12;
                    L.x2 = x2;
                    L.y2 = y12;
                    RysujLinia(&L);

                    k.x = p->x;
                    k.y = p->y;
                    RysujKolo(&k);

                    L.typ = 128;
                    dxy = df_psize * 2.5;  //2.0
                    dd = df_psize * 1.5;  //1.0
                    dd1 = df_psize;
                    switch (p->typ) {
                        case 20:
                            //x1 - dxy, y12 - dd, x2 + dxy, y12 - dd
                            L.x1 = x1 - dxy;
                            L.y1 = y12 - dd;
                            L.x2 = x2 + dxy;
                            L.y2 = y12 - dd;
                            RysujLinia(&L);

                            //x1 - dxy, y12 - dd - dd1, x2 + dxy, y12 - dd - dd1
                            L.x1 = x1 - dxy;
                            L.y1 = y12 - dd - dd1;
                            L.x2 = x2 + dxy;
                            L.y2 = y12 - dd - dd1;
                            RysujLinia(&L);
                            break;
                        case 21:
                            //x12 - dd, y1 - dxy, x12 - dd, y2 + dxy
                            L.x1 = x12 - dd;
                            L.y1 = y1 - dxy;
                            L.x2 = x12 - dd;
                            L.y2 = y2 + dxy;
                            RysujLinia(&L);

                            //x12 - dd - dd1, y1 - dxy, x12 - dd - dd1, y2 + dxy
                            L.x1 = x12 - dd - dd1;
                            L.y1 = y1 - dxy;
                            L.x2 = x12 - dd - dd1;
                            L.y2 = y2 + dxy;
                            RysujLinia(&L);

                            break;
                        case 22:
                            //x12 + dd, y1 - dxy, x12 + dd, y2 + dxy
                            L.x1 = x12 + dd;
                            L.y1 = y1 - dxy;
                            L.x2 = x12 + dd;
                            L.y2 = y2 + dxy;
                            RysujLinia(&L);

                            //x12 + dd + dd1, y1 - dxy, x12 + dd + dd1, y2 + dxy
                            L.x1 = x12 + dd + dd1;
                            L.y1 = y1 - dxy;
                            L.x2 = x12 + dd + dd1;
                            L.y2 = y2 + dxy;
                            RysujLinia(&L);

                            break;
                        case 23:
                            //x1 - dxy, y12 + dd, x2 + dxy, y12 + dd
                            L.x1 = x1 - dxy;
                            L.y1 = y12 + dd;
                            L.x2 = x2 + dxy;
                            L.y2 = y12 + dd;
                            RysujLinia(&L);

                            //x1 - dxy, y12 + dd + dd1, x2 + dxy, y12 + dd + dd1
                            L.x1 = x1 - dxy;
                            L.y1 = y12 + dd + dd1;
                            L.x2 = x2 + dxy;
                            L.y2 = y12 + dd + dd1;
                            RysujLinia(&L);

                            break;
                    }
                    break;
                case 24:
                case 25:
                case 26:
                case 27:
                    L.x1 = x12;
                    L.y1 = y1;
                    L.x2 = x12;
                    L.y2 = y2;
                    RysujLinia(&L);

                    L.x1 = x1;
                    L.y1 = y12;
                    L.x2 = x2;
                    L.y2 = y12;
                    RysujLinia(&L);

                    o.x = p->x;
                    o.y = p->y;
                    RysujOkrag(&o);

                    L.typ = 128;
                    dxy = df_psize * 3;
                    dd = df_psize * 2;
                    dd1 = df_psize;
                    switch (p->typ) {
                        case 24:
                            //x1 - dxy, y12 - dd, x2 + dxy, y12 - dd
                            L.x1 = x1 - dxy;
                            L.y1 = y12 - dd;
                            L.x2 = x2 + dxy;
                            L.y2 = y12 - dd;
                            RysujLinia(&L);

                            //x1 - dxy, y12 - dd - dd1, x2 + dxy, y12 - dd - dd1
                            L.x1 = x1 - dxy;
                            L.y1 = y12 - dd - dd1;
                            L.x2 = x2 + dxy;
                            L.y2 = y12 - dd - dd1;
                            RysujLinia(&L);
                            break;
                        case 25:
                            //x12 - dd, y1 - dxy, x12 - dd, y2 + dxy
                            L.x1 = x12 - dd;
                            L.y1 = y1 - dxy;
                            L.x2 = x12 - dd;
                            L.y2 = y2 + dxy;
                            RysujLinia(&L);

                            //x12 - dd - dd1, y1 - dxy, x12 - dd - dd1, y2 + dxy
                            L.x1 = x12 - dd - dd1;
                            L.y1 = y1 - dxy;
                            L.x2 = x12 - dd - dd1;
                            L.y2 = y2 + dxy;
                            RysujLinia(&L);
                            break;
                        case 26:
                            //x12 + dd, y1 - dxy, x12 + dd, y2 + dxy
                            L.x1 = x12 + dd;
                            L.y1 = y1 - dxy;
                            L.x2 = x12 + dd;
                            L.y2 = y2 + dxy;
                            RysujLinia(&L);

                            //x12 + dd + dd1, y1 - dxy, x12 + dd + dd1, y2 + dxy
                            L.x1 = x12 + dd + dd1;
                            L.y1 = y1 - dxy;
                            L.x2 = x12 + dd + dd1;
                            L.y2 = y2 + dxy;
                            RysujLinia(&L);
                            break;
                        case 27:
                            //x1 - dxy, y12 + dd, x2 + dxy, y12 + dd
                            L.x1 = x1 - dxy;
                            L.y1 = y12 + dd;
                            L.x2 = x2 + dxy;
                            L.y2 = y12 + dd;
                            RysujLinia(&L);

                            //x1 - dxy, y12 + dd + dd1, x2 + dxy, y12 + dd + dd1
                            L.x1 = x1 - dxy;
                            L.y1 = y12 + dd + dd1;
                            L.x2 = x2 + dxy;
                            L.y2 = y12 + dd + dd1;
                            RysujLinia(&L);
                            break;
                    }
                    break;
                case 28:
                    L.x1 = x12;
                    L.y1 = y1;
                    L.x2 = x12;
                    L.y2 = y2;
                    RysujLinia(&L);

                    L.x1 = x1;
                    L.y1 = y12;
                    L.x2 = x2;
                    L.y2 = y12;
                    RysujLinia(&L);

                    k.x = p->x;
                    k.y = p->y;
                    RysujKolo(&k);

                    L.typ = 128;
                    dxy = df_psize * 2.235;
                    dd = 2 * df_psize * 0.25;
                    dd1 = 2 * df_psize * 0.618;

                    L.x1 = x12-dxy;
                    L.y1 = y12+dd;
                    L.x2 = x12-dd1;
                    L.y2 = y12+dd;
                    RysujLinia(&L);

                    L.x1 = x12+dd1;
                    L.y1 = y12+dd;
                    L.x2 = x12+dxy;
                    L.y2 = y12+dd;
                    RysujLinia(&L);

                    L.x1 = x12-dxy;
                    L.y1 = y12-dd;
                    L.x2 = x12-dd1;
                    L.y2 = y12-dd;
                    RysujLinia(&L);

                    L.x1 = x12+dd1;
                    L.y1 = y12-dd;
                    L.x2 = x12+dxy;
                    L.y2 = y12-dd;
                    RysujLinia(&L);

                    L.x1 = x12-dd;
                    L.y1 = y12+dxy;
                    L.x2 = x12-dd;
                    L.y2 = y12+dd1;
                    RysujLinia(&L);

                    L.x1 = x12-dd;
                    L.y1 = y12-dd1;
                    L.x2 = x12-dd;
                    L.y2 = y12-dxy;
                    RysujLinia(&L);

                    L.x1 = x12+dd;
                    L.y1 = y12+dxy;
                    L.x2 = x12+dd;
                    L.y2 = y12+dd1;
                    RysujLinia(&L);

                    L.x1 = x12+dd;
                    L.y1 = y12-dd1;
                    L.x2 = x12+dd;
                    L.y2 = y12-dxy;
                    RysujLinia(&L);


                    break;
                default:
                    break;
            }
            return OkWeWy;
        }
        return OkWeWy;
    }
    else {
        //actual point
        if (PiszGrupa(0, (char *) POINT) == ErrorWeWy ||
            PiszGrupa(5, handle_hex) == ErrorWeWy ||
            PiszGrupa(100, (char *) "AcDbEntity") == ErrorWeWy ||
            PiszGrupa(8, layer_dxf(Layers[p->warstwa].name, p->warstwa)) == ErrorWeWy)
            return ErrorWeWy;
        if (p->kolor != CBYLAYER &&
            PiszGrupa(62, (int) color_dxf(p->kolor)) == ErrorWeWy)
            return ErrorWeWy;
        if (PiszGrupa(100, (char *) "AcDbPoint") == ErrorWeWy) return ErrorWeWy;
        //if (PiszGrupa(370, (int)get_width370(p->typ)) == ErrorWeWy) return ErrorWeWy;
        if (PiszGrupa(10, (double) (p->x)) == ErrorWeWy ||
            PiszGrupa(20, (double) (p->y)) == ErrorWeWy ||
            PiszGrupa(30, 0.0) == ErrorWeWy)
            return ErrorWeWy;
        if (PiszGrupa(50, 0.0) == ErrorWeWy) return ErrorWeWy;
        else return OkWeWy;
    }
    return OkWeWy;
}


void PISZ_OBJECTS::draw_arrow_to_DXF(double x0, double y0, double x1, double y1, double x2, double y2, double koc1, double kos1, double koc2, double kos2, double psize, AVECTOR *v)
{
    LINIA L=Ldef;
    WIELOKAT w=Stdef;
    int ret;

    L.x1=x1;
    L.y1=y1;
    L.x2=x2;
    L.y2=y2;
    L.warstwa=w.warstwa=v->warstwa;
    L.kolor=w.kolor=v->kolor;
    L.typ=64;

    ret = RysujLinia(&L);

    w.xy[2]=x0;
    w.xy[3]=y0;
    w.xy[0]=x0+psize*koc1;
    w.xy[1]=y0-psize*kos1;
    w.xy[4]=x0+psize*koc2;
    w.xy[5]=y0-psize*kos2;

    w.lp=6;
    w.n=32;

    ret = RysujSolid(&w);
}

void PISZ_OBJECTS::draw_wave_to_DXF(double x0, double y0, double x1, double y1, double x2, double y2, double koc, double kos, double n1, double ra, AVECTOR *v)
{
    SPLINE s=Splinedef;
    LINIA L=Ldef;
    double dl, dlm, dx, dy;
    double  xp, yp;
    int i;
    int ret;

    if ((v->magnitude1==0) && (v->magnitude2==0)) return;

    dx=ra;
    dy=ra/4;

    s.warstwa=v->warstwa;
    if (n1==1) s.kolor=1;
    else s.kolor=5;
    s.typ=64;

    dl=n1*sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

    if (fabs(dl)>(3*dx))
    {
        dlm=0;
        s.xy[0]=x1;
        s.xy[1]=y1;
        dlm+=(n1*dx);
        s.xy[2]=x1+dlm;
        s.xy[3]=y1+dy;
        i=4;
        dlm+=(2*n1*dx);
        while (fabs(dlm)<fabs(dl))
        {
            dy=-dy;
            s.xy[i]=x1+dlm;
            s.xy[i+1]=y1+dy;
            i+=2;
            if (i>=NumSplinePoints*2) break;
            dlm+=(2*n1*dx);
        }

        dlm-=(n1*dx);
        dy=-dy;
        s.xy[i]=x1+dlm;
        s.xy[i+1]=y1;
        i+=2;

        s.lp=i;
        s.xy[s.lp]=0.75;
        s.n = 8 + (s.lp + 1 ) * sizeof(float);

        s.npts=5;
        s.closed=0;

        for (i = 0; i < (int)s.lp; i += 2)
        {
            Rotate_Point(kos, koc, x1, y1, s.xy[i], s.xy[i + 1], &xp, &yp);
            s.xy[i] = xp; s.xy[i + 1] = yp;
        }

        ret = RysujSpline(&s);
    }
    else if (fabs(dl)>0.001)
    {
        L.warstwa=v->warstwa;
       
        if (n1==1) L.kolor=1;
        else L.kolor=5;
        L.typ=64;
        L.x1=x1;
        L.y1=y1;
        L.x2=x2;
        L.y2=y2;

        ret = RysujLinia(&L);
    }
}


void PISZ_OBJECTS::make_arrows_to_DXF(float x1, float y1, float x2, float y2, float x11, float y11, float x12, float y12, double angle0, AVECTOR *v, double kat)
{
    //arrows
    int i;
    double df_l0;
    double df_line_rem;
    BOOL b_first_end=TRUE;
    double df_seg_len;
    double df_x0, df_y0, df_x, df_y, df_x1, df_y1, df_x2, df_y2;
    double df_psize;
    point a1, a2, b1, b2, p;
    double koc, kos;
    double ra, ra1;
    float n, n1;
    double angle, angle_rev;
    double katS=Pi_*25.0/180;
    double koc1, kos1, koc2, kos2;
    PLINIA PL1;
    LINIA Lt1;
    double del_angle;


    df_psize = Get_Point_Size ();


    if (v->style!=15) df_seg_len=df_psize;
    else df_seg_len=df_psize*0.66;  //THERMAL

    angle=Angle_Normal(angle0);

    koc=cos(angle);
    kos=sin(angle);

    ra=df_seg_len / 2;
    ra1=0.9*ra;

    switch (v->style)
    {
        case 10:
            if (x1<=x2) angle=Angle_Normal(angle0+Pi_);
            break;
        case 11:
            if (y1<=y2) angle=Angle_Normal(angle0+Pi_);
            break;
        case 12:
            angle=Angle_Normal(-angle0);
            if (fabs(angle-Pi2)<0.00001) angle=0;
            if (fabs(angle)<0.00001) angle=0;
            break;
        case 13:
            if (x1<=x2) angle=Angle_Normal(angle0+Pi_);
            break;
        case 14:
            if (y1<=y2) angle=Angle_Normal(angle0+Pi_);
            break;
        case 15:
            angle=Angle_Normal(-angle0);
            if (fabs(angle-Pi2)<0.00001) angle=0;
            if (fabs(angle)<0.00001) angle=0;
            break;
        default:
            break;
    }

    koc1=cos(angle-katS);
    koc2=cos(angle+katS);
    kos1=sin(angle-katS);
    kos2=sin(angle+katS);

    angle_rev=Angle_Normal(angle+Pi_);

    i = 0 ;
    df_l0 = -df_seg_len/2; //0 ;
    do
    {
        df_line_rem = measure_vector (x1, y1, x2, y2, b_first_end, df_l0,  df_seg_len, &df_x, &df_y) ;
        if (TRUE == Check_if_GT (df_line_rem, df_seg_len/2 /*0*/))
        {

            if (Check_if_Equal(angle, Pi_/2))  //vertical
            {
                a1.x=df_x; a1.y=-100.0;
                a2.x=df_x; a2.y=100.0;
                b1.x=(double)x11; b1.y=(double)y11;
                b2.x=(double)x12; b2.y=(double)y12;
                p=intersectionPoint(a1,a2,b1,b2);
            }
            else if (Check_if_Equal(angle, 0))  //horizontal
            {
                a1.x=-100.0; a1.y=df_y;
                a2.x=100.0; a2.y=df_y;
                b1.x=(double)x11; b1.y=(double)y11;
                b2.x=(double)x12; b2.y=(double)y12;
                p=intersectionPoint(a1,a2,b1,b2);
            }
            else
            {
                a1.x=df_x-100.0*koc; a1.y=df_y-100.0*kos;
                a2.x=df_x+100.0*koc; a2.y=df_y+100.0*kos;
                b1.x=(double)x11; b1.y=(double)y11;
                b2.x=(double)x12; b2.y=(double)y12;
                p=intersectionPoint(a1,a2,b1,b2);
            }

            if (isnan(p.x)) return;

            n1=1;

            switch (v->style)
            {
                case 10:
                    if (v->flags & 1)
                    {
                        df_x0 = p.x;
                        df_y0 = p.y;

                        df_x2 = df_x;
                        df_y2 = df_y;

                        df_x1 = df_x;

                        if ((p.y - df_y) > -0.001) {
                            df_y1 = p.y - ra1 * kos;
                            if (x1 > x2) n1 = -1;
                        } else {
                            df_y1 = p.y + ra1 * kos;
                            if (x1 < x2) n1 = -1;
                        }
                        n1*=-1;
                    }
                    else
                    {
                        df_x0=df_x;
                        df_y0=df_y;

                        df_x2 = p.x;
                        df_y2 = p.y;

                        df_x1 = df_x;

                        if ((p.y - df_y) > -0.001) {
                            df_y1 = df_y + ra1 * kos;
                            if (x1 > x2) n1 = -1;
                        } else {
                            df_y1 = df_y - ra1 * kos;
                            if (x1 < x2) n1 = -1;
                        }
                    }
                    break;
                case 11:
                    if (v->flags & 1) {
                        df_x0 = p.x;
                        df_y0 = p.y;

                        df_x2 = df_x;
                        df_y2 = df_y;

                        df_y1 = df_y;

                        if ((p.x - df_x) > 0.001) {
                            df_x1 = p.x - ra1 * koc;
                            if (y1 < y2) n1 = -1;
                        } else {
                            df_x1 = p.x + ra1 * koc;
                            if (y1 > y2) n1 = -1;
                        }
                        n1*=-1;
                    }
                    else {
                        df_x0 = df_x;
                        df_y0 = df_y;

                        df_x2 = p.x;
                        df_y2 = p.y;

                        df_y1 = df_y;

                        if ((p.x - df_x) > 0.001) {
                            df_x1 = df_x + ra1 * koc;
                            if (y1 < y2) n1 = -1;
                        } else {
                            df_x1 = df_x - ra1 * koc;
                            if (y1 > y2) n1 = -1;
                        }
                    }
                    break;
                case 12:
                    if (v->flags & 1) {
                        df_x0 = p.x;
                        df_y0 = p.y;

                        df_x2 = df_x;
                        df_y2 = df_y;

                        n1 = 1;

                        Lt1.x1 = df_x;
                        Lt1.y1 = df_y;
                        Lt1.x2 = p.x;
                        Lt1.y2 = p.y;

                        parametry_lini(&Lt1, &PL1);
                        if (PL1.dl > 0) {
                            if (PL1.kat < kat) PL1.kat += 360;
                            del_angle = PL1.kat - kat;

                            if (fabs(del_angle - 90) > 1.0) n1 = -1;
                        }

                        df_x1 = p.x - n1 * ra1 * koc;
                        df_y1 = p.y - n1 * ra1 * kos;

                        n1*=-1;

                    }
                    else
                    {
                        df_x0 = df_x;
                        df_y0 = df_y;

                        df_x2 = p.x;
                        df_y2 = p.y;

                        n1 = 1;

                        Lt1.x1 = df_x;
                        Lt1.y1 = df_y;
                        Lt1.x2 = p.x;
                        Lt1.y2 = p.y;

                        parametry_lini(&Lt1, &PL1);
                        if (PL1.dl > 0) {
                            if (PL1.kat < kat) PL1.kat += 360;
                            del_angle = PL1.kat - kat;

                            if (fabs(del_angle - 90) > 1.0) n1 = -1;
                        }

                        df_x1 = df_x + n1 * ra1 * koc;
                        df_y1 = df_y + n1 * ra1 * kos;
                    }
                    break;
                case 13:
                    if (v->flags & 1)
                    {
                        df_x0 = p.x;
                        df_y0 = p.y;

                        df_x2 = df_x;
                        df_y2 = df_y;

                        df_x1 = df_x;
                        if (x1 < x2) {
                            if (p.y < df_y) n1 = -1;
                            df_y1 = p.y - n1 * ra1 * kos;
                        } else {
                            if (p.y > df_y) n1 = -1;
                            df_y1 = p.y + n1 * ra1 * kos;
                        }
                        n1*=-1;
                    }
                    else
                    {
                        df_x0 = df_x;
                        df_y0 = df_y;

                        df_x2 = p.x;
                        df_y2 = p.y;

                        df_x1 = df_x;

                        if (x1 < x2) {
                            if (p.y < df_y) n1 = -1;
                            df_y1 = df_y + n1 * ra1 * kos;
                        } else {
                            if (p.y > df_y) n1 = -1;
                            df_y1 = df_y - n1 * ra1 * kos;
                        }
                    }
                    break;
                case 14:
                    if (v->flags & 1)
                    {
                        df_x0 = p.x;
                        df_y0 = p.y;

                        df_x2 = df_x;
                        df_y2 = df_y;

                        df_y1 = df_y;

                        if (y1 < y2) {
                            if (p.x > df_x) n1 = -1;
                            df_x1 = p.x + n1 * ra1 * koc;
                        } else {
                            if (p.x < df_x) n1 = -1;
                            df_x1 = p.x - n1 * ra1 * koc;
                        }
                        n1*=-1;
                    }
                    else
                    {
                        df_x0 = df_x;
                        df_y0 = df_y;

                        df_x2 = p.x;
                        df_y2 = p.y;

                        df_y1 = df_y;

                        if (y1 < y2) {
                            if (p.x > df_x) n1 = -1;
                            df_x1 = df_x - n1 * ra1 * koc;
                        } else {
                            if (p.x < df_x) n1 = -1;
                            df_x1 = df_x + n1 * ra1 * koc;
                        }
                    }

                    break;
                case 15:

                    df_x0=df_x;
                    df_y0=df_y;

                    n1=1;

                    Lt1.x1=df_x;
                    Lt1.y1=df_y;
                    Lt1.x2=p.x;
                    Lt1.y2=p.y;

                    parametry_lini(&Lt1, &PL1);
                    if (PL1.dl>0)
                    {
                        if (PL1.kat < kat) PL1.kat += 360;
                        del_angle = PL1.kat - kat;

                        if (fabs(del_angle - 90) > 1.0) n1 = -1;
                    }

                    df_x1 = df_x + n1*ra1 * koc;
                    df_y1 = df_y + n1*ra1 * kos;

                    df_x2 = p.x - n1*ra1*0.5 * koc;
                    df_y2 = p.y - n1*ra1*0.5 * kos;
                    break;
                default:
                    df_x1 = df_x;
                    if (x1<x2) df_y1 = df_y - ra1 * kos;
                    else df_y1 = df_y + ra1 * kos;
                    break;
            }

            if (v->style!=15) draw_arrow_to_DXF(df_x0, df_y0, df_x1, df_y1, df_x2, df_y2, koc1, kos1, koc2, kos2, n1*ra, v);
            else draw_wave_to_DXF(df_x0, df_y0, df_x1, df_y1, df_x2, df_y2, koc, kos, n1, ra, v);
            df_l0 += df_seg_len ;
            i++ ;
        }
    }
    while (TRUE == Check_if_GT (df_line_rem, df_seg_len/2)) ;
}


int PISZ_OBJECTS::RysujVector(AVECTOR *ptrs_vector, int *v_block_sufix)
{  char sufiks_bloku[10];
    double df_psize, df_psize1 ;
    PLINIA PL, PL1, PLth;
    LINIA L1=Ldef, L2=Ldef, L3=Ldef, L4=Ldef, Lt=Ldef, Ls=Ldef, Lth=Ldef, Ln=Ldef, Ln1=Ldef;
    SOLIDARC sa=sadef;
    WIELOKAT w=S4def;
    OKRAG O=Odef;
    OKRAG K=Kdef;
    LUK l=ldef;
    double kat1, kos, koc, kats, kata2;
    double ra;
    double katS=25.0;
    TEXT Vtxt=Tdef, Vltxt=Tdef, Vtxt1=Tdef;
    double n, dx, dy, dx1, dy1, dx2, dy2, Kp2s, K1_5, Ltx, Lty;
    int kolorS;
    double koc1, kos1, koc2, kos2, koc1th, kos1th;
    int grubosc, s;
    double  xs, ys;
    int TRANS=25;
    float xmax, ymax;
    char* translucency_ptr;
    int translucency;
    BOOL ret;
    double Lcx, Lcy;
    double t_len_mm, direction, Kp2sn;
    double kat0=10;
    int vkolor;

#define arrowf 1.0

    /*przyjecie nazwy domyslnej*/
    strcpy(ins_b.nazwa_bloku, "V");
    ins_b.x0 = ptrs_vector->x1;
    ins_b.y0 = ptrs_vector->y1;
    ins_b.z0 = 0;
    ins_b.invisible = 0;

    *v_block_sufix += 1;
    sprintf(sufiks_bloku, "%#ld", *v_block_sufix);

    if (strlen(ins_b.nazwa_bloku) > 27) ins_b.nazwa_bloku[26] = '\0';

    //if (strcmp(ins_b.nazwa_bloku, "*D") != 0) strcat(ins_b.nazwa_bloku, "-");   //nie moze byc "~"
    strcat(ins_b.nazwa_bloku, sufiks_bloku);
    /*KOREKTA NAZWY BLOKU*/
    encodingdxf((char *) &ins_b.nazwa_bloku);

    /*funkcja RecordBlock zapisuje blok w TABLES*/
    to_block = FALSE;
    to_entities = FALSE;
    if (RecordBlock(ins_b) == ErrorWeWy) return ErrorWeWy;

    /*funkcja IniBlock inicjuje blok w zbiorze blokow*/
    /*i dopisuje Insert do zbioru prymitywow*/
    to_entities = TRUE;
    if (InsBlock(ins_b) == ErrorWeWy) return ErrorWeWy;

    to_block = TRUE;
    to_entities = FALSE;
    if (IniBlock(ins_b) == ErrorWeWy) return ErrorWeWy;

    if (PiszGrupa(999, (int)ptrs_vector->style) == ErrorWeWy) return ErrorWeWy;

    //actual vector

    df_psize = Get_Point_Size () / 2;   //2
    df_psize1 = Get_Point_Size () / 4 ; //2

    if (ptrs_vector->kolor==7) vkolor=0;
    else vkolor=ptrs_vector->kolor;

    L1.warstwa=O.warstwa=l.warstwa=K.warstwa=w.warstwa=sa.warstwa=ptrs_vector->warstwa;
    L1.kolor=O.kolor=l.kolor=K.kolor=w.kolor=sa.kolor=vkolor;

    L1.typ=ptrs_vector->typ;

    memmove(&L2, &L1, sizeof(LINIA));
    memmove(&L3, &L1, sizeof(LINIA));
    memmove(&L4, &L1, sizeof(LINIA));
    memmove(&Lt, &L1, sizeof(LINIA));
    memmove(&Ls, &L1, sizeof(LINIA));
    memmove(&Lth, &L1, sizeof(LINIA));
    memmove(&Ln, &L1, sizeof(LINIA));
    memmove(&Ln1, &L1, sizeof(LINIA));

    L1.x1=ptrs_vector->x1;
    L1.y1=ptrs_vector->y1;
    L1.x2=ptrs_vector->x2;
    L1.y2=ptrs_vector->y2;
    parametry_lini(&L1, &PL);
    kat1=PL.kat;
    kos=sin(PL.kat*Pi/180);
    koc=cos(PL.kat*Pi/180);

    Vtxt.kat=normalize_txt_angle(PL.kat*Pi/180);
    Vtxt.justowanie=j_srodkowo;

    Vtxt.warstwa=ptrs_vector->warstwa;
    Vtxt.kolor=vkolor;
    Vtxt.czcionka=zmwym.czcionka;
    Vtxt.wysokosc=zmwym.wysokosc;
    Vtxt.width_factor=zmwym.width_factor;
    memmove(&Vtxt1, &Vtxt, sizeof(TEXT));
    memmove(&Vltxt, &Vtxt, sizeof(TEXT));
    Vltxt.wysokosc=Vltxt.wysokosc/2;

    strcpy(Vtxt.text, "");
    strcpy(Vtxt1.text, "");

    switch (ptrs_vector->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            Vtxt.x=(L1.x1+L1.x2)/2;
            Vtxt.y=(L1.y1+L1.y2)/2;
            if (ptrs_vector->property_no>0) sprintf(Vtxt.text, "#%d", ptrs_vector->property_no);
            break;
        case 4:

            kos1=sin(Angle_Normal((PL.kat-90)*Pi/180));
            koc1=cos(Angle_Normal((PL.kat-90)*Pi/180));

            if (L1.x1<=L1.x2) n=1; else n=-1;

            Vtxt.x=(L1.x1+L1.x2)/2-((n*ra/4)*koc1);
            Vtxt.y=(L1.y1+L1.y2)/2-((n*ra/4)*kos1);
            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, force_precision);
            normalize_txt(&Vtxt);

            Vltxt.x=(L1.x1+L1.x2)/2+n*((Vltxt.wysokosc+ra/4)*koc1);
            Vltxt.y=(L1.y1+L1.y2)/2+n*((Vltxt.wysokosc+ra/4)*kos1);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 7:

            kos1=sin(Angle_Normal((PL.kat-90)*Pi/180));
            koc1=cos(Angle_Normal((PL.kat-90)*Pi/180));

            if (L1.x1<=L1.x2) n=1; else n=-1;

            Vtxt.x=(L1.x1+L1.x2)/2-((n*ra/4)*koc1);
            Vtxt.y=(L1.y1+L1.y2)/2-((n*ra/4)*kos1);
            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, displacement_precision);
            normalize_txt(&Vtxt);

            Vltxt.x=(L1.x1+L1.x2)/2+n*((Vltxt.wysokosc+ra/4)*koc1);
            Vltxt.y=(L1.y1+L1.y2)/2+n*((Vltxt.wysokosc+ra/4)*kos1);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 5:
        case 6:
            if (ptrs_vector->angle2<ptrs_vector->angle1)
                kata2=ptrs_vector->angle2+Pi2;
            else kata2=ptrs_vector->angle2;
            kats=Angle_Normal((ptrs_vector->angle1+kata2)/2);
            Vtxt.x=ptrs_vector->x1+(ptrs_vector->r+0.5)*cos(kats);
            Vtxt.y=ptrs_vector->y1+(ptrs_vector->r+0.5)*sin(kats);

            Vltxt.x=ptrs_vector->x1+(ptrs_vector->r - Vltxt.wysokosc - 0.5)*cos(kats);
            Vltxt.y=ptrs_vector->y1+(ptrs_vector->r - Vltxt.wysokosc - 0.5)*sin(kats);

            Vtxt.kat=Vltxt.kat=Angle_Normal(kats-Pi_/2);
            Vtxt.justowanie=Vltxt.justowanie=j_srodkowo;
            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, moment_precision);
            normalize_txt(&Vtxt);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 8:
        case 9:
            if (ptrs_vector->angle2<ptrs_vector->angle1)
                kata2=ptrs_vector->angle2+Pi2;
            else kata2=ptrs_vector->angle2;
            kats=Angle_Normal((ptrs_vector->angle1+kata2)/2);
            Vtxt.x=ptrs_vector->x1+(ptrs_vector->r+0.5)*cos(kats);
            Vtxt.y=ptrs_vector->y1+(ptrs_vector->r+0.5)*sin(kats);

            Vltxt.x=ptrs_vector->x1+(ptrs_vector->r - Vltxt.wysokosc - 0.5)*cos(kats);
            Vltxt.y=ptrs_vector->y1+(ptrs_vector->r - Vltxt.wysokosc - 0.5)*sin(kats);

            Vtxt.kat=Vltxt.kat=Angle_Normal(kats-Pi_/2);
            Vtxt.justowanie=Vltxt.justowanie=j_srodkowo;

            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, rotation_precision);
            normalize_txt(&Vtxt);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 10:  //trapezium Y

            if (L1.x1<L1.x2) n=1;
            else n=-1;

            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = L1.x1;
            Lt.y1 = L1.y1 + n*(ptrs_vector->magnitude1/load_magnitude);
            Lt.x2 = L1.x2;
            Lt.y2 = L1.y2 + n*(ptrs_vector->magnitude2/load_magnitude);

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos1=sin(Pi*(PL1.kat+90)/180);
            koc1=cos(Pi*(PL1.kat+90)/180);

            if (Lt.x1<Lt.x2)
            {   if (Lt.y1<L1.y1)
                {
                    dx=-Vtxt.wysokosc*koc1;
                    dy=-Vtxt.wysokosc*kos1;
                }
                else
                {
                    dx = 0;
                    dy = 0;
                }
                if (Lt.y2<L1.y2)
                {
                    dx1=-Vtxt.wysokosc*koc1;
                    dy1=-Vtxt.wysokosc*kos1;

                    dx2=-Vltxt.wysokosc*koc1;
                    dy2=-Vltxt.wysokosc*kos1;
                }
                else {
                    dx1 = 0;
                    dy1 = 0;
                    dx2 = 0;
                    dy2 = 0;
                }
            }
            else
            {   if (Lt.y1>L1.y1)
                {
                    dx = 0;
                    dy = 0;
                }
                else
                {
                    dx = Vtxt.wysokosc * koc1;
                    dy = Vtxt.wysokosc * kos1;
                }
                if (Lt.y2>L1.y2)
                {
                    dx1 = 0;
                    dy1 = 0;
                    dx2 = 0;
                    dy2 = 0;
                }
                else
                {
                    dx1 = Vtxt.wysokosc * koc1;
                    dy1 = Vtxt.wysokosc * kos1;
                    dx2 = Vltxt.wysokosc * koc1;
                    dy2 = Vltxt.wysokosc * kos1;
                }
            }

            Vtxt.x=Lt.x1+dx;
            Vtxt.y=Lt.y1+dy;
            Vtxt.kat=normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt.justowanie=j_do_lewej;
            else Vtxt.justowanie=j_do_prawej;
            Vtxt1.x=Lt.x2+dx1;
            Vtxt1.y=Lt.y2+dy1;
            Vtxt1.kat=Vltxt.kat=normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt1.justowanie=j_do_prawej;
            else  Vtxt1.justowanie=j_do_lewej;

            Vltxt.x=Ltx+dx2;
            Vltxt.y=Lty+dy2;
            Vltxt.justowanie=j_srodkowo;

            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, load_precision);
            set_decimal_format(Vtxt1.text, ptrs_vector->magnitude2, load_precision);

            normalize_txt(&Vtxt);
            normalize_txt(&Vtxt1);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 11: //trapezium X
            if (L1.y1<L1.y2) n=1;
            else n=-1;

            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = L1.x1 - n*(ptrs_vector->magnitude1/load_magnitude);
            Lt.y1 = L1.y1;
            Lt.x2 = L1.x2 - n*(ptrs_vector->magnitude2/load_magnitude);
            Lt.y2 = L1.y2;

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos1=sin(Pi*(PL1.kat+90)/180);
            koc1=cos(Pi*(PL1.kat+90)/180);

            if (Lt.y1<Lt.y2)
            {   if (Lt.x1>L1.x1)
                {
                    dx=-Vtxt.wysokosc*koc1;
                    dy=-Vtxt.wysokosc*kos1;
                }
                else
                {
                    dx = 0;
                    dy = 0;
                }
                if (Lt.x2>L1.x2)
                {
                    dx1=-Vtxt.wysokosc*koc1;
                    dy1=-Vtxt.wysokosc*kos1;
                    dx2=-Vltxt.wysokosc*koc1;
                    dy2=-Vltxt.wysokosc*kos1;
                }
                else {
                    dx1 = 0;
                    dy1 = 0;
                    dx2 = 0;
                    dy2 = 0;
                }
            }
            else
            {   if (Lt.x1>L1.x1)
                {
                    dx = 0;
                    dy = 0;
                }
                else
                {
                    dx = -Vtxt.wysokosc * koc1;
                    dy = -Vtxt.wysokosc * kos1;
                }
                if (Lt.x2>L1.x2)
                {
                    dx1 = 0;
                    dy1 = 0;
                    dx2 = 0;
                    dy2 = 0;
                }
                else
                {
                    dx1 = -Vtxt.wysokosc * koc1;
                    dy1 = -Vtxt.wysokosc * kos1;
                    dx2 = -Vltxt.wysokosc * koc1;
                    dy2 = -Vltxt.wysokosc * kos1;
                }
            }

            Vtxt.x=Lt.x1+dx;
            Vtxt.y=Lt.y1+dy;
            Vtxt.kat=(PL1.kat*Pi/180);
            Vtxt.justowanie=j_do_lewej;
            Vtxt1.x=Lt.x2+dx1;
            Vtxt1.y=Lt.y2+dy1;
            Vtxt1.kat=Vltxt.kat=(PL1.kat*Pi/180);
            Vtxt1.justowanie=j_do_prawej;

            Vltxt.x=Ltx+dx2;
            Vltxt.y=Lty+dy2;
            Vltxt.justowanie=j_srodkowo;

            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, load_precision);
            set_decimal_format(Vtxt1.text, ptrs_vector->magnitude2, load_precision);

            normalize_txt(&Vtxt);
            normalize_txt(&Vtxt1);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 12:  //trapezium N

            kos1=sin(Pi*(PL.kat+90)/180);
            koc1=cos(Pi*(PL.kat+90)/180);

            n=1;
            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = L1.x1 + n*(ptrs_vector->magnitude1/load_magnitude)*koc1;
            Lt.y1 = L1.y1 + n*(ptrs_vector->magnitude1/load_magnitude)*kos1;
            Lt.x2 = L1.x2 + n*(ptrs_vector->magnitude2/load_magnitude)*koc1;
            Lt.y2 = L1.y2 + n*(ptrs_vector->magnitude2/load_magnitude)*kos1;

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos2=sin(Pi*(PL1.kat+90)/180);
            koc2=cos(Pi*(PL1.kat+90)/180);

            dx = 0;
            dy = 0;
            dx1 = 0;
            dy1 = 0;
            dx2 = 0;
            dy2 = 0;

            //if (ptrs_vector->magnitude1<0)
            if (((ptrs_vector->magnitude1<0) && !(ptrs_vector->flags & 1)) ||
                ((ptrs_vector->magnitude1>0) && (ptrs_vector->flags & 1)))
            {
                dx = -Vtxt.wysokosc * koc2;
                dy = -Vtxt.wysokosc * kos2;
            }

            //if (ptrs_vector->magnitude2<0)
            if (((ptrs_vector->magnitude2<0) && !(ptrs_vector->flags & 1)) ||
                ((ptrs_vector->magnitude2>0) && (ptrs_vector->flags & 1)))
            {
                dx1 = -Vtxt.wysokosc * koc2;
                dy1 = -Vtxt.wysokosc * kos2;

                dx2 = -Vltxt.wysokosc * koc2;
                dy2 = -Vltxt.wysokosc * kos2;
            }

            Vtxt.x=Lt.x1+dx;
            Vtxt.y=Lt.y1+dy;
            Vtxt.kat=(PL1.kat*Pi/180);
            Vtxt.justowanie=j_do_lewej;
            Vtxt1.x=Lt.x2+dx1;
            Vtxt1.y=Lt.y2+dy1;
            Vtxt1.kat=Vltxt.kat=(PL1.kat*Pi/180);
            Vtxt1.justowanie=j_do_prawej;

            Vltxt.x=Ltx+dx2;
            Vltxt.y=Lty+dy2;
            Vltxt.justowanie=j_srodkowo;

            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, load_precision);
            set_decimal_format(Vtxt1.text, ptrs_vector->magnitude2, load_precision);

            normalize_txt(&Vtxt);
            normalize_txt(&Vtxt1);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 13:  //trapezium H
            if (L1.x1<L1.x2)
            {
                n=1;
                ymax=max(L1.y1, L1.y2);
            }
            else
            {
                n=-1;
                ymax=min(L1.y1, L1.y2);
            }

            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = L1.x1;
            Lt.y1 = ymax + n*(ptrs_vector->magnitude1/load_magnitude);
            Lt.x2 = L1.x2;
            Lt.y2 = ymax + n*(ptrs_vector->magnitude2/load_magnitude);

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos1=sin(Pi*(PL1.kat+90)/180);
            koc1=cos(Pi*(PL1.kat+90)/180);

            dx=0;
            dy=0;
            dx1=0;
            dy1=0;
            dx2=0;
            dy2=0;

            if (Lt.x1<Lt.x2)
            {
                
                if (((ptrs_vector->magnitude1<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude1>0) && (ptrs_vector->flags & 1)))
                {
                    dx=-Vtxt.wysokosc*koc1;
                    dy=-Vtxt.wysokosc*kos1;
                }
               
                if (((ptrs_vector->magnitude2<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude2>0) && (ptrs_vector->flags & 1)))
                {
                    dx1=-Vtxt.wysokosc*koc1;
                    dy1=-Vtxt.wysokosc*kos1;

                    dx2=-Vltxt.wysokosc*koc1;
                    dy2=-Vltxt.wysokosc*kos1;
                }
            }
            else
            {
               
                if (((ptrs_vector->magnitude1>=0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude1<0) && (ptrs_vector->flags & 1)))
                {
                    dx = Vtxt.wysokosc * koc1;
                    dy = Vtxt.wysokosc * kos1;
                }
               
                if (((ptrs_vector->magnitude2>=0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude2<0) && (ptrs_vector->flags & 1)))
                {
                    dx1 = Vtxt.wysokosc * koc1;
                    dy1 = Vtxt.wysokosc * kos1;

                    dx2 = Vltxt.wysokosc * koc1;
                    dy2 = Vltxt.wysokosc * kos1;
                }
            }

            Vtxt.x=Lt.x1+dx;
            Vtxt.y=Lt.y1+dy;
            Vtxt.kat=normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt.justowanie=j_do_lewej;
            else Vtxt.justowanie=j_do_prawej;
            Vtxt1.x=Lt.x2+dx1;
            Vtxt1.y=Lt.y2+dy1;
            Vtxt1.kat=Vltxt.kat=normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt1.justowanie=j_do_prawej;
            else  Vtxt1.justowanie=j_do_lewej;

            Vltxt.x=Ltx+dx2;
            Vltxt.y=Lty+dy2;
            Vltxt.justowanie=j_srodkowo;

            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, load_precision);
            set_decimal_format(Vtxt1.text, ptrs_vector->magnitude2, load_precision);

            normalize_txt(&Vtxt);
            normalize_txt(&Vtxt1);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 14:  //trapezium V

            if (L1.y1<L1.y2)
            {
                n=1;
                xmax=min(L1.x1, L1.x2);
            }
            else
            {
                n=-1;
                xmax=max(L1.x1, L1.x2);
            }

            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = xmax - n*(ptrs_vector->magnitude1/load_magnitude);
            Lt.y1 = L1.y1;
            Lt.x2 = xmax - n*(ptrs_vector->magnitude2/load_magnitude);
            Lt.y2 = L1.y2;

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos1=sin(Pi*(PL1.kat+90)/180);
            koc1=cos(Pi*(PL1.kat+90)/180);

            dx=0;
            dy=0;
            dx1=0;
            dy1=0;
            dx2=0;
            dy2=0;

            if (Lt.y1<Lt.y2)
            {
                
                if (((ptrs_vector->magnitude1<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude1>0) && (ptrs_vector->flags & 1)))
                {
                    dx=-Vtxt.wysokosc*koc1;
                    dy=-Vtxt.wysokosc*kos1;
                }
               
                if (((ptrs_vector->magnitude2<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude2>0) && (ptrs_vector->flags & 1)))
                {
                    dx1=-Vtxt.wysokosc*koc1;
                    dy1=-Vtxt.wysokosc*kos1;

                    dx2=-Vltxt.wysokosc*koc1;
                    dy2=-Vltxt.wysokosc*kos1;
                }
            }
            else
            {
                
                if (((ptrs_vector->magnitude1<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude1>0) && (ptrs_vector->flags & 1)))
                {
                    dx = -Vtxt.wysokosc * koc1;
                    dy = -Vtxt.wysokosc * kos1;
                }
                
                if (((ptrs_vector->magnitude2<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude2>0) && (ptrs_vector->flags & 1)))
                {
                    dx1 = -Vtxt.wysokosc * koc1;
                    dy1 = -Vtxt.wysokosc * kos1;

                    dx2 = -Vltxt.wysokosc * koc1;
                    dy2 = -Vltxt.wysokosc * kos1;
                }
            }

            Vtxt.x=Lt.x1+dx;
            Vtxt.y=Lt.y1+dy;
            Vtxt.kat=(PL1.kat*Pi/180);
            Vtxt.justowanie=j_do_lewej;
            Vtxt1.x=Lt.x2+dx1;
            Vtxt1.y=Lt.y2+dy1;
            Vtxt1.kat=Vltxt.kat=(PL1.kat*Pi/180);
            Vtxt1.justowanie=j_do_prawej;

            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, load_precision);
            set_decimal_format(Vtxt1.text, ptrs_vector->magnitude2, load_precision);

            normalize_txt(&Vtxt);
            normalize_txt(&Vtxt1);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 15:

            kos1=sin(Pi*(PL.kat+90)/180);
            koc1=cos(Pi*(PL.kat+90)/180);


            Lth.x1 = (L1.x1+L1.x2)/2 + (ptrs_vector->r/depth_magnitude)*koc1;  //thermal_depth_size
            Lth.y1 = (L1.y1+L1.y2)/2 + (ptrs_vector->r/depth_magnitude)*kos1;
            Lth.x2 = (L1.x1+L1.x2)/2 - (ptrs_vector->r/depth_magnitude)*koc1;
            Lth.y2 = (L1.y1+L1.y2)/2 - (ptrs_vector->r/depth_magnitude)*kos1;

            parametry_lini(&Lth, &PLth);
            kos1th=sin(Pi*(PLth.kat+90)/180);
            koc1th=cos(Pi*(PLth.kat+90)/180);

            Lt.x1 = Lth.x1 + (ptrs_vector->magnitude1/thermal_magnitude)*koc1th;
            Lt.y1 = Lth.y1 + (ptrs_vector->magnitude1/thermal_magnitude)*kos1th;
            Lt.x2 = Lth.x2 + (ptrs_vector->magnitude2/thermal_magnitude)*koc1th;
            Lt.y2 = Lth.y2 + (ptrs_vector->magnitude2/thermal_magnitude)*kos1th;

            parametry_lini(&Lt, &PL1);
            kos2=sin(Pi*(PL1.kat+90)/180);
            koc2=cos(Pi*(PL1.kat+90)/180);

            dx = 0;
            dy = 0;
            dx1 = 0;
            dy1 = 0;

            if (ptrs_vector->magnitude1<0)
            {
                dx = -Vtxt.wysokosc * koc2;
                dy = -Vtxt.wysokosc * kos2;
            }

            if (ptrs_vector->magnitude2<0)
            {
                dx1 = -Vtxt.wysokosc * koc2;
                dy1 = -Vtxt.wysokosc * kos2;
            }

            Vtxt.x=Lt.x1+dx;
            Vtxt.y=Lt.y1+dy;
            Vtxt.kat=(PL1.kat*Pi/180);
            Vtxt.justowanie=j_do_lewej;
            Vtxt1.x=Lt.x2+dx1;
            Vtxt1.y=Lt.y2+dy1;
            Vtxt1.kat=(PL1.kat*Pi/180);
            Vtxt1.justowanie=j_do_prawej;

            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, thermal_precision);
            set_decimal_format(Vtxt1.text, ptrs_vector->magnitude2, thermal_precision);

            normalize_txt(&Vtxt);
            normalize_txt(&Vtxt1);
            break;
        case 16:
            Ln.x1 = L1.x1 + (ptrs_vector->r/radius_magnitude)*koc;   //thermal_depth_size
            Ln.y1 = L1.y1 + (ptrs_vector->r/radius_magnitude)*kos;
            Ln.x2 = L1.x2;
            Ln.y2 = L1.y2;

            Vtxt.x=L1.x2;
            Vtxt.y=L1.y2;
            Vtxt.kat=0;
            Vtxt.wysokosc=zmwym.wysokosc*0.75;
            Vtxt.justowanie=j_do_lewej;

            set_decimal_format(Vtxt1.text, ptrs_vector->magnitude1, dim_precision);
            sprintf(Vtxt.text,"R%s",Vtxt1.text);

            break;
        default:
            break;
    }

    Vtxt.dl = strlen(Vtxt.text);
    Vtxt1.dl = strlen(Vtxt1.text);
    Vtxt.n = T18 + Vtxt.dl;
    Vtxt1.n = T18 + Vtxt1.dl;

    L2.x1 = L1.x1 + ra * koc;
    L2.y1 = L1.y1 + ra * kos;
    L2.x2 = L1.x2 - ra * koc;
    L2.y2 = L1.y2 - ra * kos;

    L3.x1 = L1.x1 + df_psize * koc;
    L3.y1 = L1.y1 + df_psize * kos;
    L3.x2 = L1.x2 - df_psize * koc;
    L3.y2 = L1.y2 - df_psize * kos;

    L4.x1 = L1.x1 + df_psize1 * koc;
    L4.y1 = L1.y1 + df_psize1 * kos;
    L4.x2 = L1.x2 - df_psize1 * koc;
    L4.y2 = L1.y2 - df_psize1 * kos;

    switch(ptrs_vector->style)
    {
        case 0:

            if(RysujLinia(&L4)==ErrorWeWy) return ErrorWeWy;

            K.x=ptrs_vector->x1;
            K.y=ptrs_vector->y1;
            K.r=df_psize1;

            if(RysujKolo(&K)==ErrorWeWy) return ErrorWeWy;

            K.x=ptrs_vector->x2;
            K.y=ptrs_vector->y2;
            if(RysujKolo(&K)==ErrorWeWy) return ErrorWeWy;

            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;

            break;
        case 1:

            L4.x2=L3.x2;
            L4.y2=L3.y2;

            if(RysujLinia(&L4)==ErrorWeWy) return ErrorWeWy;

            K.x=ptrs_vector->x1;
            K.y=ptrs_vector->y1;
            K.r=df_psize1;

            if(RysujKolo(&K)==ErrorWeWy) return ErrorWeWy;

            O.typ = 64;
            O.x = ptrs_vector->x2;
            O.y = ptrs_vector->y2;
            O.r = df_psize;

            if(RysujOkrag(&O)==ErrorWeWy) return ErrorWeWy;

            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;

            break;
        case 2:

            L4.x1=L3.x1;
            L4.y1=L3.y1;

            if(RysujLinia(&L4)==ErrorWeWy) return ErrorWeWy;

            K.x=ptrs_vector->x2;
            K.y=ptrs_vector->y2;
            K.r=df_psize1;

            if(RysujKolo(&K)==ErrorWeWy) return ErrorWeWy;

            O.typ = 64;
            O.x = ptrs_vector->x1;
            O.y = ptrs_vector->y1;
            O.r = df_psize;

            if(RysujOkrag(&O)==ErrorWeWy) return ErrorWeWy;


            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;

            break;
        case 3:

            if(RysujLinia(&L3)==ErrorWeWy) return ErrorWeWy;

            O.typ = 64;
            O.x = ptrs_vector->x1;
            O.y = ptrs_vector->y1;
            O.r = df_psize;

            if(RysujOkrag(&O)==ErrorWeWy) return ErrorWeWy;

            O.x = ptrs_vector->x2;
            O.y = ptrs_vector->y2;

            if(RysujOkrag(&O)==ErrorWeWy) return ErrorWeWy;

            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;

            break;
        case 4:  //force
        case 18: //slab force
            w.atrybut=ptrs_vector->atrybut;
            w.warstwa=ptrs_vector->warstwa;
            w.kolor=vkolor;

            if (PL.dl>ra)
            {
                Ls.x2 = ptrs_vector->x2;
                Ls.y2 = ptrs_vector->y2;
                Ls.x1 = L2.x1;
                Ls.y1 = L2.y1;

                create_solid_on_line(&Ls, &w, ra / 4, ra / 4, 0);

                if(RysujSolid(&w)==ErrorWeWy) return ErrorWeWy;
            }

            Kp2s = Get_Point_Size () / (arrowf*cos(Pi*katS/180)) ;

            koc1=cos(Pi*(kat1-katS)/180);
            koc2=cos(Pi*(kat1+katS)/180);
            kos1=sin(Pi*(kat1-katS)/180);
            kos2=sin(Pi*(kat1+katS)/180);

            w.xy[2]=ptrs_vector->x1;
            w.xy[3]=ptrs_vector->y1;
            w.xy[0]=ptrs_vector->x1+Kp2s*koc1;
            w.xy[1]=ptrs_vector->y1+Kp2s*kos1;
            w.xy[4]=ptrs_vector->x1+Kp2s*koc2;
            w.xy[5]=ptrs_vector->y1+Kp2s*kos2;

            w.lp=6;
            w.n=32;

            if(RysujSolid(&w)==ErrorWeWy) return ErrorWeWy;

            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;
            if(RysujText(&Vltxt)==ErrorWeWy) return ErrorWeWy;
            break;
        case 7:  //displacement

            grubosc=(ptrs_vector->typ & 224 ) / 32;

            L3.x2=L2.x2;
            L3.y2=L2.y2;
            L3.typ=grubosc*32 + 1;

            if(RysujLinia(&L3)==ErrorWeWy) return ErrorWeWy;

            O.typ = 64;
            O.x = ptrs_vector->x1;
            O.y = ptrs_vector->y1;
            O.r = df_psize;

            if(RysujOkrag(&O)==ErrorWeWy) return ErrorWeWy;

            Kp2s = Get_Point_Size () / (arrowf*cos(Pi*katS/180)) ;

            koc1=cos(Pi*(kat1-katS)/180);
            koc2=cos(Pi*(kat1+katS)/180);
            kos1=sin(Pi*(kat1-katS)/180);
            kos2=sin(Pi*(kat1+katS)/180);

            w.xy[2]=ptrs_vector->x2;
            w.xy[3]=ptrs_vector->y2;
            w.xy[0]=ptrs_vector->x2-Kp2s*koc1;
            w.xy[1]=ptrs_vector->y2-Kp2s*kos1;
            w.xy[4]=ptrs_vector->x2-Kp2s*koc2;
            w.xy[5]=ptrs_vector->y2-Kp2s*kos2;

            w.lp=6;
            w.n=32;
            w.empty_typ=3;

            if(RysujSolid(&w)==ErrorWeWy) return ErrorWeWy;

            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;
            if(RysujText(&Vltxt)==ErrorWeWy) return ErrorWeWy;
            break;
        case 5:  //moment
        case 6:
        case 8:  //rotation
        case 9:
            if (ptrs_vector->style==5)
            {
                linestyle(ptrs_vector->typ);
                kats = ptrs_vector->angle2 - Pi_ / 2;
                xs = ptrs_vector->x1 + ptrs_vector->r * cos(ptrs_vector->angle2);
                ys = ptrs_vector->y1 + ptrs_vector->r * sin(ptrs_vector->angle2);
                n = +1;
                s = 0;

                sa.atrybut = ptrs_vector->atrybut;
                sa.warstwa = ptrs_vector->warstwa;
                sa.kolor = vkolor;
                sa.x = ptrs_vector->x1;
                sa.y = ptrs_vector->y1;
                sa.r = ptrs_vector->r;
                sa.kat1 = ptrs_vector->angle1;
                sa.kat2 = ptrs_vector->angle2;
                sa.width1 = ra / 4;
                sa.width2 = ra / 4;

                if(RysujSolidArc(&sa)==ErrorWeWy) return ErrorWeWy;
            }
            else if (ptrs_vector->style==6)
            {
                linestyle(ptrs_vector->typ);
                kats = ptrs_vector->angle1 + Pi_ / 2;
                xs = ptrs_vector->x1 + ptrs_vector->r * cos(ptrs_vector->angle1);
                ys = ptrs_vector->y1 + ptrs_vector->r * sin(ptrs_vector->angle1);
                n = -1;
                s = 0;

                sa.atrybut = ptrs_vector->atrybut;
                sa.warstwa = ptrs_vector->warstwa;
                sa.kolor = vkolor;
                sa.x = ptrs_vector->x1;
                sa.y = ptrs_vector->y1;
                sa.r = ptrs_vector->r;
                sa.kat1 = ptrs_vector->angle1;
                sa.kat2 = ptrs_vector->angle2;
                sa.width1 = ra / 4;
                sa.width2 = ra / 4;

                if(RysujSolidArc(&sa)==ErrorWeWy) return ErrorWeWy;
            }
            else if (ptrs_vector->style==8)
            {
                grubosc = (ptrs_vector->typ & 224) / 32;
                linestyle(grubosc * 32 + 1);
                kats = ptrs_vector->angle2 - Pi_ / 2;
                n = +1;
                s = 1;
                xs = ptrs_vector->x1 + ptrs_vector->r * cos(ptrs_vector->angle2);
                ys = ptrs_vector->y1 + ptrs_vector->r * sin(ptrs_vector->angle2);

                l.x = ptrs_vector->x1;
                l.y = ptrs_vector->y1;
                l.r = ptrs_vector->r;
                l.kat1 = ptrs_vector->angle1;
                l.kat2 = ptrs_vector->angle2;
                l.typ = grubosc * 32 + 1;

                if(RysujLuk(&l)==ErrorWeWy) return ErrorWeWy;
            }
            else if (ptrs_vector->style==9)
            {
                grubosc = (ptrs_vector->typ & 224) / 32;
                linestyle(grubosc * 32 + 1);
                kats = ptrs_vector->angle1 + Pi_ / 2;
                n = -1;
                s = 1;
                xs = ptrs_vector->x1 + ptrs_vector->r * cos(ptrs_vector->angle1);
                ys = ptrs_vector->y1 + ptrs_vector->r * sin(ptrs_vector->angle1);

                l.x = ptrs_vector->x1;
                l.y = ptrs_vector->y1;
                l.r = ptrs_vector->r;
                l.kat1 = ptrs_vector->angle1;
                l.kat2 = ptrs_vector->angle2;
                l.typ = grubosc * 32 + 1;

                if(RysujLuk(&l)==ErrorWeWy) return ErrorWeWy;
            }

            dx = Get_Point_Size() / arrowf * cos(kats);
            dy = Get_Point_Size() / arrowf * sin(kats);

            xs -= dx;
            ys -= dy;


            Kp2s = Get_Point_Size() / (arrowf * cos(Pi * katS / 180));

            koc1 = cos(kats - Pi * (katS) / 180);
            koc2 = cos(kats + Pi * (katS) / 180);
            kos1 = sin(kats - Pi * (katS) / 180);
            kos2 = sin(kats + Pi * (katS) / 180);

            w.xy[2] = xs;
            w.xy[3] = ys;
            w.xy[0] = xs + Kp2s * koc1;
            w.xy[1] = ys + Kp2s * kos1;
            w.xy[4] = xs + Kp2s * koc2;
            w.xy[5] = ys + Kp2s * kos2;
            w.lp = 6;
            w.n = 32;

            if (s == 0) {
                w.empty_typ = 0;
            } else {
                w.empty_typ = 3;
            }

            if(RysujSolid(&w)==ErrorWeWy) return ErrorWeWy;

            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;
            if(RysujText(&Vltxt)==ErrorWeWy) return ErrorWeWy;

            break;
        case 10:  //trapezium Y

            L1.typ = 64;

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            w.empty_typ = 0;
            w.xy[0] = ptrs_vector->x1;
            w.xy[1] = ptrs_vector->y1;
            w.xy[2] = ptrs_vector->x2;
            w.xy[3] = ptrs_vector->y2;
            w.xy[4] = ptrs_vector->x2;
            w.xy[5] = Lt.y2;
            w.xy[6] = ptrs_vector->x1;
            w.xy[7] = Lt.y1;
            w.lp = 8;
            w.n = 40;

            Normalize_Solid(&w);

            L1.typ = 64;
            L1.x1 = w.xy[2];
            L1.y1 = w.xy[3];
            L1.x2 = w.xy[4];
            L1.y2 = w.xy[5];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x1 = w.xy[6];
            L1.y1 = w.xy[7];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x2 = w.xy[0];
            L1.y2 = w.xy[1];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            w.translucent = 1;
            translucency = TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            if(RysujSolid(&w)==ErrorWeWy) return ErrorWeWy;

            make_arrows_to_DXF(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], Pi_ / 2, ptrs_vector, PL.kat);

            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;
            if(RysujText(&Vtxt1)==ErrorWeWy) return ErrorWeWy;

            break;
        case 11:  //trapezium X

            L1.typ=64;

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            w.empty_typ=0;
            w.xy[0]=ptrs_vector->x1;
            w.xy[1]=ptrs_vector->y1;
            w.xy[2]=ptrs_vector->x2;
            w.xy[3]=ptrs_vector->y2;
            w.xy[4]=Lt.x2;
            w.xy[5]=ptrs_vector->y2;
            w.xy[6]=Lt.x1;
            w.xy[7]=ptrs_vector->y1;
            w.lp=8;
            w.n=40;

            Normalize_Solid(&w);

            L1.typ=64;
            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=w.xy[4];
            L1.y2=w.xy[5];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x1=w.xy[6];
            L1.y1=w.xy[7];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x2=w.xy[0];
            L1.y2=w.xy[1];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            if(RysujSolid(&w)==ErrorWeWy) return ErrorWeWy;

            make_arrows_to_DXF(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], 0, ptrs_vector, PL.kat);

            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;
            if(RysujText(&Vtxt1)==ErrorWeWy) return ErrorWeWy;

            break;
        case 12:  //trapezium N
            L1.typ=64;

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            w.empty_typ=0;
            w.xy[0]=ptrs_vector->x1;
            w.xy[1]=ptrs_vector->y1;
            w.xy[2]=ptrs_vector->x2;
            w.xy[3]=ptrs_vector->y2;
            w.xy[4]=Lt.x2;
            w.xy[5]=Lt.y2;
            w.xy[6]=Lt.x1;
            w.xy[7]=Lt.y1;
            w.lp=8;
            w.n=40;

            Normalize_Solid(&w);

            L1.typ=64;
            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=w.xy[4];
            L1.y2=w.xy[5];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x1=w.xy[6];
            L1.y1=w.xy[7];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x2=w.xy[0];
            L1.y2=w.xy[1];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            if(RysujSolid(&w)==ErrorWeWy) return ErrorWeWy;

            make_arrows_to_DXF(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], Pi_*(PL.kat+90.0)/180.0, ptrs_vector, PL.kat);

            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;
            if(RysujText(&Vtxt1)==ErrorWeWy) return ErrorWeWy;

            break;
        case 13:  //trapezium H
            L1.typ=64;

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            if (ptrs_vector->x1<ptrs_vector->x2)
            {
                n=1;
                w.xy[1]=w.xy[3]=max(ptrs_vector->y1, ptrs_vector->y2);
            }
            else
            {
                n=-1;
                w.xy[1]=w.xy[3]=min(ptrs_vector->y1, ptrs_vector->y2);
            }
            w.xy[0]=ptrs_vector->x1;
            w.xy[2]=ptrs_vector->x2;
            w.xy[4]=ptrs_vector->x2;
            w.xy[5]=Lt.y2;
            w.xy[6]=ptrs_vector->x1;
            w.xy[7]=Lt.y1;

            Normalize_Solid(&w);

            L1.typ=33;
            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=ptrs_vector->x1;
            L1.y2=ptrs_vector->y1;

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=ptrs_vector->x2;
            L1.y2=ptrs_vector->y2;

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.typ=64;
            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=w.xy[2];
            L1.y2=w.xy[3];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x1=w.xy[4];
            L1.y1=w.xy[5];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x2=w.xy[6];
            L1.y2=w.xy[7];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x1=w.xy[0];
            L1.y1=w.xy[1];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            if(RysujSolid(&w)==ErrorWeWy) return ErrorWeWy;

            make_arrows_to_DXF(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], Pi_/2, ptrs_vector, PL.kat);

            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;
            if(RysujText(&Vtxt1)==ErrorWeWy) return ErrorWeWy;

            break;
        case 14:  //trapezium V

            L1.typ=64;

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            if (ptrs_vector->y1<ptrs_vector->y2)
            {
                n=1;
                w.xy[0]=w.xy[2]=min(ptrs_vector->x1, ptrs_vector->x2);
            }
            else
            {
                n=-1;
                w.xy[0]=w.xy[2]=max(ptrs_vector->x1, ptrs_vector->x2);
            }

            w.xy[1]=ptrs_vector->y1;
            w.xy[3]=ptrs_vector->y2;
            w.xy[4]=Lt.x2;
            w.xy[5]=ptrs_vector->y2;
            w.xy[6]=Lt.x1;
            w.xy[7]=ptrs_vector->y1;

            Normalize_Solid(&w);

            L1.typ=33;
            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=ptrs_vector->x1;
            L1.y2=ptrs_vector->y1;

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=ptrs_vector->x2;
            L1.y2=ptrs_vector->y2;

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.typ=64;
            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=w.xy[2];
            L1.y2=w.xy[3];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x1=w.xy[4];
            L1.y1=w.xy[5];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x2=w.xy[6];
            L1.y2=w.xy[7];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x1=w.xy[0];
            L1.y1=w.xy[1];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            if(RysujSolid(&w)==ErrorWeWy) return ErrorWeWy;

            make_arrows_to_DXF(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], 0, ptrs_vector, PL.kat);

            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;
            if(RysujText(&Vtxt1)==ErrorWeWy) return ErrorWeWy;

            break;

        case 15: //thermal
            L1.typ=64;

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            if(RysujLinia(&Lth)==ErrorWeWy) return ErrorWeWy;

            w.empty_typ=0;
            w.xy[0]=Lth.x1;
            w.xy[1]=Lth.y1;
            w.xy[2]=Lth.x2;
            w.xy[3]=Lth.y2;
            w.xy[4]=Lt.x2;
            w.xy[5]=Lt.y2;
            w.xy[6]=Lt.x1;
            w.xy[7]=Lt.y1;
            w.lp=8;
            w.n=40;

            Normalize_Solid(&w);

            L1.typ=64;
            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=w.xy[4];
            L1.y2=w.xy[5];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x1=w.xy[6];
            L1.y1=w.xy[7];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            L1.x2=w.xy[0];
            L1.y2=w.xy[1];

            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            if(RysujSolid(&w)==ErrorWeWy) return ErrorWeWy;

            make_arrows_to_DXF(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], Pi_*(PLth.kat+90.0)/180.0, ptrs_vector, PLth.kat);

            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;
            if(RysujText(&Vtxt1)==ErrorWeWy) return ErrorWeWy;
            break;
        case 16:
            ////PRINT_TRANSLUCENCY=TRANS;
            K.x=ptrs_vector->x1;
            K.y=ptrs_vector->y1;
            K.r=ptrs_vector->r;

            if (EXPORT_TRANSLUCEN_DISC)
            {
                if (RysujTransKolo(&K, TRANS) == ErrorWeWy) return ErrorWeWy;
            }

            ////PRINT_TRANSLUCENCY=255;

            l.typ = 64;
            l.x=ptrs_vector->x1;
            l.y=ptrs_vector->y1;
            l.r=ptrs_vector->r;
            l.kat1 = 0;
            l.kat2 = Pi2;

            if(RysujLuk(&l)==ErrorWeWy) return ErrorWeWy;

            L1.typ=Ln1.typ=32;
            if(RysujLinia(&L1)==ErrorWeWy) return ErrorWeWy;

            Ln1.x1=L1.x2;
            Ln1.y1=L1.y2;
            Ln1.y2=Ln1.y1;

            t_len_mm = Get_TextLen(&Vtxt, Vtxt.text);
            direction=L1.x2-L1.x1;

            if (direction > 0) //to the right
            {
                Vtxt.x = L1.x2 + Vtxt.wysokosc / 4.0;
                Vtxt.y = L1.y2 + Vtxt.wysokosc / 4.0;
                Vtxt.justowanie = j_do_lewej;

                if (t_len_mm == 0.0) Ln1.x2 = L1.x2 + 0.01;
                else Ln1.x2 = L1.x2 + t_len_mm + Vtxt.wysokosc / 2.5;
            } else  //to the left
            {
                Vtxt.x = L1.x2 - Vtxt.wysokosc / 4.0;
                Vtxt.y = L1.y2 + Vtxt.wysokosc / 4.0;
                Vtxt.justowanie = j_do_prawej;
                if (t_len_mm == 0.0) Ln1.x2 = L1.x2 - 0.01;
                else Ln1.x2 = L1.x2 - t_len_mm - Vtxt.wysokosc / 2.0;
            }

            if (Draw_Line_To_Drive(&Ln1) == FALSE) return FALSE;

            K1_5=jednostkiplt(w1_5);
            Kp2sn = 1.686909582 * K1_5;

            n=-1;
            w.xy[2]=Ln.x1;
            w.xy[3]=Ln.y1;
            w.xy[0]=Ln.x1-n*Kp2sn*cos(Pi*(kat1-kat0)/180);
            w.xy[1]=Ln.y1-n*Kp2sn*sin(Pi*(kat1-kat0)/180);
            w.xy[4]=Ln.x1-n*Kp2sn*cos(Pi*(kat1+kat0)/180);
            w.xy[5]=Ln.y1-n*Kp2sn*sin(Pi*(kat1+kat0)/180);
            w.lp = 6;
            w.n = 8 + w.lp * sizeof (float) ;

            if(RysujSolid(&w)==ErrorWeWy) return ErrorWeWy;

            if(RysujText(&Vtxt)==ErrorWeWy) return ErrorWeWy;

            break;
        default:
            break;
    }

    return OkWeWy;
}


int PISZ_OBJECTS::RysujImage(B_PCX *b_pcx)
{
	
	double ll_x, ll_y, ll_x1, ll_y1, dx1, dy1, dx2, dy2;
	double kos, koc;
	int display_prop;

	image_no_++;


	if (PiszGrupa(0, (char*)IMAGE) == ErrorWeWy ||
		PiszGrupa(5, handle_hex) == ErrorWeWy ||
		PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
		PiszGrupa(8, layer_dxf(Layers[b_pcx->warstwa].name, b_pcx->warstwa)) == ErrorWeWy) return ErrorWeWy;
	
	if (PiszGrupa(100, (char*)"AcDbRasterImage") == ErrorWeWy) return ErrorWeWy;
	//insert point
	//point has to be transferred from upper left (center of rotation) to lower left
	ll_x = b_pcx->x;
	ll_y = b_pcx->y - (b_pcx->i_y*b_pcx->dy);
	//then point <ll_x, ll_y> must be rotated around <b_pcx->x, b_pcx->y> by pcx->kat

	kos = sin(b_pcx->kat);
	koc = cos(b_pcx->kat);
	Rotate_Point(kos, koc, b_pcx->x, b_pcx->y, ll_x, ll_y, &ll_x1, &ll_y1);

	if (PiszGrupa(10, ll_x1) == ErrorWeWy ||
		PiszGrupa(20, ll_y1) == ErrorWeWy ||
		PiszGrupa(30, 0.0) == ErrorWeWy) return ErrorWeWy;

	Rotate_Point(kos, koc, 0, 0,  b_pcx->dx, 0, &dx1, &dy1);
	Rotate_Point(kos, koc, 0, 0,  0, b_pcx->dy, &dx2, &dy2);

	//U-vecxtor
	if (PiszGrupa(11, dx1) == ErrorWeWy ||
		PiszGrupa(21, dy1) == ErrorWeWy ||
		PiszGrupa(31, 0.0) == ErrorWeWy) return ErrorWeWy;

	//V-vector
	if (PiszGrupa(12, dx2) == ErrorWeWy ||
		PiszGrupa(22, dy2) == ErrorWeWy ||
		PiszGrupa(32, 0.0) == ErrorWeWy) return ErrorWeWy;

	if (PiszGrupa(13, (int)(b_pcx->i_x)) == ErrorWeWy ||
		PiszGrupa(23, (int)(b_pcx->i_y)) == ErrorWeWy)  return ErrorWeWy;

	if (PiszGrupa(340, image_no_) == ErrorWeWy) return ErrorWeWy;  //hard reference to imagedef object

	display_prop = 1;
	if (b_pcx->on_front) display_prop += 8;

	if (PiszGrupa(70, display_prop) == ErrorWeWy) return ErrorWeWy;  //image display Show image + Transparency on
	if (PiszGrupa(280, 0) == ErrorWeWy) return ErrorWeWy; //clipping state
	if (PiszGrupa(281, 50) == ErrorWeWy) return ErrorWeWy; //brightness value 0-100
	if (PiszGrupa(282, 50) == ErrorWeWy) return ErrorWeWy; //contrastmvalue 0-100
	if (PiszGrupa(283, 0) == ErrorWeWy) return ErrorWeWy; //fade value 0-100

	return OkWeWy;
}

/*--------------------------------------------*/


/*-------- class PISZ_BLOCKS ------------------------------------*/

//class PISZ_BLOCKS : virtual public PISZ_GRUPA
//  public :
//  int PiszBlocks(void);
//  PISZ_BLOCKS(void);

PISZ_BLOCKS::PISZ_BLOCKS(void)
{}

int PISZ_BLOCKS::PiszBlocks(void)
{
	int kolory_paper;

	if(PiszGrupa(0,(char*)SECTION)==ErrorWeWy ||
     PiszGrupa(2,(char*)BLOCKS)==ErrorWeWy) return ErrorWeWy;

if (kolory.paper == 15) kolory_paper = 7;
else kolory_paper = kolory.paper;

if (PiszGrupa(0, (char*)BLOCK) == ErrorWeWy ||
	PiszGrupa(5, handle_hex) == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
	PiszGrupa(8, (char*)"0") == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbBlockBegin") == ErrorWeWy ||
	PiszGrupa(2,(char*)"*Model_Space") == ErrorWeWy ||
	PiszGrupa(70, 0) == ErrorWeWy ||
	PiszGrupa(10, 0) == ErrorWeWy ||
	PiszGrupa(20, 0) == ErrorWeWy ||
	PiszGrupa(30, 0) == ErrorWeWy ||
	PiszGrupa(62, (int)color_dxf(kolory_paper)) == ErrorWeWy ||
	PiszGrupa(3,(char*)"*Model_Space") == ErrorWeWy ||
	PiszGrupa(1, (char*)"") == ErrorWeWy ||
	PiszGrupa(0, (char*)ENDBLK) == ErrorWeWy) return ErrorWeWy;
	if (PiszGrupa(5, handle_hex) == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
	PiszGrupa(8, (char*)"0") == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbBlockEnd") == ErrorWeWy) return ErrorWeWy;

if (PiszGrupa(0, (char*)BLOCK) == ErrorWeWy ||
	PiszGrupa(5, handle_hex) == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
	PiszGrupa(8, (char*)"0") == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbBlockBegin") == ErrorWeWy ||
	PiszGrupa(2, (char*)"*Paper_Space") == ErrorWeWy ||
	PiszGrupa(70, 0) == ErrorWeWy ||
	PiszGrupa(10, 0) == ErrorWeWy ||
	PiszGrupa(20, 0) == ErrorWeWy ||
	PiszGrupa(30, 0) == ErrorWeWy ||
	PiszGrupa(3, (char*)"*Paper_Space") == ErrorWeWy ||
	PiszGrupa(1, (char*)"") == ErrorWeWy ||
	PiszGrupa(0, (char*)ENDBLK) == ErrorWeWy) return ErrorWeWy;
	if (PiszGrupa(5, handle_hex) == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbEntity") == ErrorWeWy ||
	PiszGrupa(8, (char*)"0") == ErrorWeWy ||
	PiszGrupa(100, (char*)"AcDbBlockEnd") == ErrorWeWy) return ErrorWeWy;

  else return OkWeWy;
}

int PISZ_BLOCKS::PiszEndSec(void)
{ if (PiszGrupa(0,(char*)ENDSEC)==ErrorWeWy ) return ErrorWeWy;
   else return OkWeWy;
}

/*----------------------------------------------------------------*/


/*------  class PISZ_ENTITIES  -------------------------------------*/

/*-----------------------------------------------------*/
//class PISZ_ENTITIES : virtual public PISZ_GRUPA      //
//  public  :                                          //
//   PISZ_ENTITIES(void);                              //
//   int PiszEntities(void);                           //
/*-----------------------------------------------------*/

PISZ_ENTITIES::PISZ_ENTITIES(void)
{}

int PISZ_ENTITIES::PiszEntities(void)
{ if(PiszGrupa(0,(char*)SECTION)==ErrorWeWy   ||
     PiszGrupa(2,(char*)ENTITIES)==ErrorWeWy  ||
     MyEntities()==ErrorWeWy ) return ErrorWeWy;
     to_block=FALSE;
     if (PiszGrupa(0,(char*)ENDSEC)==ErrorWeWy ) return ErrorWeWy;
  else return OkWeWy;
}
/*--------------------------------------------*/

/*----------  class PISZ_DXF  ----------------------------*/

/*---------------------------------------------------------*/
//class PISZ_DXF : public PISZ_HEADER,public PISZ_TABLES,  //
//		 public PISZ_BLOCKS,public PISZ_ENTITIES   //
//  public :                                               //
//   int Pisz_Dxf(void);                                   //
//   PISZ_DXF(void);                                       //
/*---------------------------------------------------------*/

PISZ_DXF::PISZ_DXF(void)
{}

int PISZ_DXF::Pisz_Dxf(void)
{ 
	line_widths_dxf();
	
	if(PiszHeader()==ErrorWeWy ||
	 PiszClasses() == ErrorWeWy ||
     PiszTables()==ErrorWeWy) return ErrorWeWy;
	 to_block = TRUE;
     if (PiszBlocks()==ErrorWeWy) return ErrorWeWy;
     to_block=FALSE;
	 to_entities = TRUE;
     if (PiszEntities()==ErrorWeWy) return ErrorWeWy;

	 to_block = FALSE;
	 to_entities = FALSE;
	 if (PiszGrupa(0, (char*)"ENDTAB") == ErrorWeWy)  return ErrorWeWy;
	 if (PiszEndSec() == ErrorWeWy) return ErrorWeWy;

     to_block=TRUE;
	 to_entities = FALSE;
     if (PiszEndSec()==ErrorWeWy) return ErrorWeWy;
     to_block=FALSE;
	 to_entities = TRUE;
	 if (PiszObjects() == ErrorWeWy) return ErrorWeWy;
	 if (PiszEndSec() == ErrorWeWy) return ErrorWeWy;
     if (PiszGrupa(0,(char*)"EOF")==ErrorWeWy ) return ErrorWeWy;
  else return OkWeWy;
}

/*------------------------------------------------------------*/

static   FILE *stream1; //main
static   FILE *stream2; //blocks
static   FILE *stream3; //entities

/*----------  class PISZ_DXF_ASCI  ----------------------------*/

//class PISZ_DXF_ASCII : public PISZ_DXF
// private :
//  virtual int PiszKodGrupy(int );
//   virtual int PiszInt(int );
//   virtual int PiszFloat(double );
//   virtual int PiszString(char *);
//   virtual int PiszHeader0(void);
//  public :
//   PISZ_DXF_ASCII(char *path);
//   ~PISZ_DXF_ASCII(void);
//   int Pisz_Dxf_ASCII(void);

/*--------*/

int PISZ_DXF_ASCII::PiszHeader0(void)
{ char path26[]="HEADER26.DXT", string[MaxLen];
  char path10[]="HEADER10.DXT", *path;
  FILE *in;
  BOOL END = FALSE;
  //int n;
  if ( AcadV == ACADV26)
     path = path26;
  else
     path = path10;
  if((in=fopen(path,"rt"))==NULL) return ErrorWeWy;
  strcpy(string," ");

   while (!END)
   {
    fgets(string,MaxLen,in);
	if (strncmp(string, "$EOF", 4) != 0) fputs(string, stream1);
	else END = TRUE;
   } 
  fclose(in);

  return OkWeWy;
}

PISZ_DXF_ASCII::PISZ_DXF_ASCII(char *path)
{ Path=path;
  stream1=fopen(Path,"wt");
  stream2 = fopen("$DXF_BLK.TMP", "wt");
  stream3 = fopen("$DXF_ENT.TMP","wt");
}

PISZ_DXF_ASCII::~PISZ_DXF_ASCII(void)
{ BOOL endoffile;
  char dxf_line[MAXLINE];
  int f;
  
  fclose(stream2);
  fclose(stream3);
  stream2=fopen("$DXF_BLK.TMP","rt");
  /*przepisywanie stream2 do stream1*/
  endoffile=FALSE;
  while (endoffile==FALSE)
  {    
  if ( fgets(dxf_line , MAXLINE , stream2 ) == NULL ) endoffile=TRUE;
    else
     {
       fputs(dxf_line,stream1);
     }  
  }     
  fclose(stream2);

  stream3 = fopen("$DXF_ENT.TMP", "rt");
  /*przepisywanie stream2 do stream1*/
  endoffile = FALSE;
  while (endoffile == FALSE)
  {
	  if (fgets(dxf_line, MAXLINE, stream2) == NULL) endoffile = TRUE;
	  else
	  {
		  fputs(dxf_line, stream1);
	  }
  }
  fclose(stream3);

  fclose(stream1);
  /*usuniecie stread2*/
  f = remove("$DXF_BLK.TMP");
  f = remove("$DXF_ENT.TMP");
}

int PISZ_DXF_ASCII::Pisz_Dxf_ASCII(void)
{ if(stream1==NULL) return ErrorWeWy;
  else return Pisz_Dxf();
}

/*-------  virtual  --------------*/

int PISZ_DXF_ASCII::PiszKodGrupy( int n)
{ if (to_block==TRUE) return fprintf(stream2,"%3d\n",n);
  else if (to_entities == TRUE) return fprintf(stream3, "%3d\n", n);
  else return fprintf(stream1,"%3d\n",n);
}

int PISZ_DXF_ASCII::PiszInt( int n)
{ if (to_block==TRUE) return fprintf(stream2,"%6d\n",n);
  else if (to_entities == TRUE) return fprintf(stream3, "%6d\n", n);
   else return fprintf(stream1,"%6d\n",n);
}

int PISZ_DXF_ASCII::PiszFloat( double f)
{ if (to_block==TRUE) return fprintf(stream2,"%#.6lf\n",f);
 else if (to_entities == TRUE) return fprintf(stream3, "%#.6lf\n", f);
   else return fprintf(stream1,"%#.6lf\n",f);
}

int PISZ_DXF_ASCII::PiszString(char *str)
{ if (to_block==TRUE) return fprintf(stream2,"%s\n",str);
  else if (to_entities == TRUE) return fprintf(stream3, "%s\n", str);
    else return fprintf(stream1,"%s\n",str);
}

/*------------------------------------------------------------*/

int AcadV;   /*prowizorka*/
#define cursor_off(X,Y) out_cur_off(X,Y)
#define cursor_on(X,Y) out_cur_on(X,Y)


/*----------  class My_App_PISZ_DXF_ASCII  ----------------------------*/

class My_App_PISZ_DXF_ASCII : public PISZ_DXF_ASCII
{ private :
   virtual int ChangeHeader(void);
   virtual int MyEntities(void);
   virtual int MyBlocks(void);
  public :
   My_App_PISZ_DXF_ASCII(char *path);
};

/*--------*/

My_App_PISZ_DXF_ASCII::My_App_PISZ_DXF_ASCII(char *path) : PISZ_DXF_ASCII(path)
{}

/*---  virtu+al  ---------*/

int My_App_PISZ_DXF_ASCII::ChangeHeader(void)
{
 double df_xemin, df_yemin, df_xemax, df_yemax ;
 int pdmode=3;

 if(PiszGrupa(9,(char*)"$USERI1")==ErrorWeWy ||
     PiszGrupa(70,211)==ErrorWeWy ) return ErrorWeWy;   /*211=65+76+70*/
 if(PiszGrupa(9,(char*)"$USERI2")==ErrorWeWy ||
     PiszGrupa(70,0)==ErrorWeWy ) return ErrorWeWy;   
 if(PiszGrupa(9,(char*)"$USERI3")==ErrorWeWy ||
     PiszGrupa(70,0)==ErrorWeWy ) return ErrorWeWy; 
 if(PiszGrupa(9,(char*)"$USERI4")==ErrorWeWy ||
     PiszGrupa(70,0)==ErrorWeWy ) return ErrorWeWy; 
 if(PiszGrupa(9,(char*)"$USERI5")==ErrorWeWy ||
     PiszGrupa(70,0)==ErrorWeWy ) return ErrorWeWy;
 if(PiszGrupa(9,(char*)"$USERR1")==ErrorWeWy ||
     PiszGrupa(40,0.0)==ErrorWeWy ) return ErrorWeWy;                  
 if(PiszGrupa(9,(char*)"$USERR2")==ErrorWeWy ||
     PiszGrupa(40,0.0)==ErrorWeWy ) return ErrorWeWy; 
 if(PiszGrupa(9,(char*)"$USERR3")==ErrorWeWy ||
     PiszGrupa(40,0.0)==ErrorWeWy ) return ErrorWeWy; 
 if(PiszGrupa(9,(char*)"$USERR4")==ErrorWeWy ||
     PiszGrupa(40,0.0)==ErrorWeWy ) return ErrorWeWy;
 if(PiszGrupa(9,(char*)"$USERR5")==ErrorWeWy ||
     PiszGrupa(40,0.0)==ErrorWeWy ) return ErrorWeWy;  
                     
 if(PiszGrupa(9,(char*)"$INSBASE")==ErrorWeWy ||
     PiszGrupa(10,Xmin)==ErrorWeWy      ||
     PiszGrupa(20,Ymin)==ErrorWeWy      ||
     PiszGrupa(30,0.0)==ErrorWeWy ) return ErrorWeWy;

  Get_Limits (0, dane_size, ANieOkreslony,
     &df_xemin, &df_xemax, &df_yemin, &df_yemax) ;
  if (df_xemin >= df_xemax && df_yemin >= df_yemax)
  {
    df_xemin = df_yemin = 0 ;
    df_xemax = 210 ;
    df_yemax = 297 ;
  }

 if(PiszGrupa(9,(char*)"$EXTMIN")==ErrorWeWy ||
     PiszGrupa(10, df_xemin)==ErrorWeWy      ||
	 PiszGrupa(20, df_yemin)==ErrorWeWy )  return ErrorWeWy;
 if(PiszGrupa(9,(char*)"$EXTMAX")==ErrorWeWy ||
     PiszGrupa(10, df_xemax)==ErrorWeWy      ||
	 PiszGrupa(20, df_yemax)==ErrorWeWy )  return ErrorWeWy;
 if(PiszGrupa(9,(char*)"$LIMMIN")==ErrorWeWy ||
     PiszGrupa(10,Xmin)==ErrorWeWy      ||
     PiszGrupa(20,Ymin)==ErrorWeWy )  return ErrorWeWy;
 if(PiszGrupa(9,(char*)"$LIMMAX")==ErrorWeWy ||
     PiszGrupa(10,Xmax)==ErrorWeWy      ||
     PiszGrupa(20,Ymax)==ErrorWeWy )  return ErrorWeWy;
 if(PiszGrupa(9,(char*)"$ORTHOMODE")==ErrorWeWy ||
     PiszGrupa(70,orto)==ErrorWeWy )  return ErrorWeWy;
 if(PiszGrupa(9,(char*)"$TEXTSIZE")==ErrorWeWy ||
     PiszGrupa(40,TextG.height)==ErrorWeWy )  return ErrorWeWy;
 if(PiszGrupa(9,(char*)"$COORDS")==ErrorWeWy ||
     PiszGrupa(70,1)==ErrorWeWy )  return ErrorWeWy;
 if(PiszGrupa(9,(char*)"$GRIDMODE")==ErrorWeWy ||
     PiszGrupa(70,grid_)==ErrorWeWy )  return ErrorWeWy;
 if(PiszGrupa(9,(char*)"$GRIDUNIT")==ErrorWeWy ||
	 PiszGrupa(10,krok_g)==ErrorWeWy      ||
     PiszGrupa(20,krok_g)==ErrorWeWy )  return ErrorWeWy;
 if(PiszGrupa(9,(char*)"$SNAPMODE")==ErrorWeWy ||
	 PiszGrupa(70,snap_)==ErrorWeWy )  return ErrorWeWy;
 if(PiszGrupa(9,(char*)"$SNAPUNIT")==ErrorWeWy ||
     PiszGrupa(10,krok_s)==ErrorWeWy      ||
     PiszGrupa(20,krok_s)==ErrorWeWy )  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$DIMLUNIT") == ErrorWeWy ||
	 PiszGrupa(70, 2) == ErrorWeWy)  return ErrorWeWy;
 if (PiszGrupa(9, (char*)"$DIMAUNIT") == ErrorWeWy ||
	 PiszGrupa(70, 0) == ErrorWeWy)  return ErrorWeWy;

 int units;
 if (Jednostki == 1.0) units = 4;
 else if (Jednostki == 10.0) units = 5;
 else if (Jednostki == 1000.0) units = 6;
 else if (Jednostki == 1000000.0) units = 7;
 else if (Jednostki == 25.4) units = 1;
 else if (Jednostki == 304.8) units = 2;
 else if (Jednostki == 914.4) units = 10;
 else if (Jednostki == 1609344) units = 3;
 else units = 4;
 if (PiszGrupa(9, (char*)"$INSUNITS") == ErrorWeWy ||
	 PiszGrupa(70, units) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$LUNITS") == ErrorWeWy ||
	 PiszGrupa(70, 2) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$AUNITS") == ErrorWeWy ||
	 PiszGrupa(70, 0) == ErrorWeWy)  return ErrorWeWy;

 //SkalaF
 //double fscales[14] = { 1,2,5,10,20,25,50,100,500,1000,5000,10000,0.5,0.2 };
 if (PiszGrupa(9, (char*)"$DIMSCALE") == ErrorWeWy ||
	 PiszGrupa(40, SkalaF) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$DIMLFAC") == ErrorWeWy ||
	 PiszGrupa(40, SkalaF) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$LTSCALE") == ErrorWeWy ||
	 PiszGrupa(40, 1.0) == ErrorWeWy)  return ErrorWeWy;  //?????

 if (PiszGrupa(9, (char*)"$CECOLOR") == ErrorWeWy ||
	 PiszGrupa(62, (int)color_dxf(LiniaG.kolor)) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$CELTYPE") == ErrorWeWy ||
	 PiszGrupa(6, TypyLinii[typ_linii(LiniaG.typ)].nazwa) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$CELWEIGHT") == ErrorWeWy ||
	 PiszGrupa(370, (int)get_width370(LiniaG.typ)) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$DIMTXSTY") == ErrorWeWy ||
	 PiszGrupa(7, PTRS__Text_Style[zmwym.czcionka] != NULL &&
		 PTRS__Text_Style[zmwym.czcionka]->font_acad_name[0] != '\0' ?
		 PTRS__Text_Style[zmwym.czcionka]->font_name : STANDARD) == ErrorWeWy) return ErrorWeWy;
 
 if (PiszGrupa(9, (char*)"$DIMTXT") == ErrorWeWy ||
	 PiszGrupa(40, zmwym.wysokosc) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$DIMTOH") == ErrorWeWy ||
	 PiszGrupa(70, (int)zmwym.kierunek_t) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$DIMCLRD") == ErrorWeWy ||
	 PiszGrupa(70, (int)color_dxf(zmwym.Lkolor)) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$DIMCLRE") == ErrorWeWy ||
	 PiszGrupa(70, (int)color_dxf(zmwym.Lkolor)) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$DIMCLRT") == ErrorWeWy ||
	 PiszGrupa(70, (int)color_dxf(zmwym.Tkolor)) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$DIMRND") == ErrorWeWy ||
	 PiszGrupa(40, zmwym.dokladnosc) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$PDMODE") == ErrorWeWy ||
     PiszGrupa(70, (int)pdmode) == ErrorWeWy)  return ErrorWeWy;

 if (PiszGrupa(9, (char*)"$PDSIZE") == ErrorWeWy ||
	 PiszGrupa(40, (double)jednostki_d_to_plt_d(Get_Point_Size()/2.0)) == ErrorWeWy)  return ErrorWeWy;


return OkWeWy;
}

int My_App_PISZ_DXF_ASCII::MyBlocks(void)
{ return OkWeWy;
}

int My_App_PISZ_DXF_ASCII::MyEntities(void)
{ NAGLOWEK *nag;
  BLOK 	*ptrs_block ;
  TEXT *ptrs_text;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;


 char sufiks_bloku[10];
 int block_sufix, p_block_sufix, v_block_sufix;
 //char **nag1;
 char *adh;
 
 block_sufix=p_block_sufix=v_block_sufix=0;
 adh=dane;
 adh+=dane_size;
 obiekt_tok_all(dane,adh,   (char **) &nag,ONieOkreslony);
 while (nag!=NULL)
  { if (nag->blok == NoElemBlok) 
    {
     if (to_block==TRUE)
      {
        if(EndBlock()==ErrorWeWy) return ErrorWeWy;
        to_block=FALSE;
		to_entities = TRUE;
      } 
    } 
    switch(nag->obiekt)
	  { 
        /*jezeli obiekt jest blokiem, to odczytywana jest nazwa bloku. */
        /*Do nazway dodawany jest sufiks wg. wzorca  ~nr gdzie :*/
        /*nr stanowi kolejny numer bloku*/
        /*wykonywana jest funkcja IniBlock*/
        /*w funkcjach RysujLinia... itd. sprawdzane jest, czy np. l->blok==1*/
        /*Jezeli tak (i dodatkowo to_block==TRUE), zapis nastepuje do zbioru */
        /*blokow. Jezeli nie, zapis nastepuje do zbioru prymitywow i dodatkowo*/
        /*jezeli to_block==TRUE to wykonywana jest funkcja EndBlock i to_block=FALSE*/

          /*if the object is a block, the block name is read. */
          /*A suffix is added to the name with the pattern ~no where :*/
          /*no is the sequential block number*/
          /*IniBlock function is executed*/
          /*in the RysujLinia... etc. functions it is checked whether, e.g. l->blok==1*/
          /*If yes (and additionally to_block==TRUE), the record is written to the blocks file.*/
          /*If not, the record is made to the primitives file and additionally */
          /*if to_block==TRUE, the EndBlock function is executed and to_block=FALSE*/
	    case OdBLOK :
          ptrs_block = (BLOK*)nag ;
          strcpy(ins_b.nazwa_bloku,"");
		  ins_b.kod_obiektu = ptrs_block->kod_obiektu;

          
          if (ptrs_block->blok==NoElemBlok)  /*ignorowane sa bloki wewnetrzne*/
          {
         // if (ptrs_block->dlugosc_opisu_obiektu <= 1)
           if ((ptrs_block->kod_obiektu == B_DIM) || (ptrs_block->kod_obiektu == B_DIM1) || (ptrs_block->kod_obiektu == B_DIM2) || (ptrs_block->kod_obiektu == B_DIM3))
           {
            /*przyjecie nazwy domyslnej*/
            if (NO_DIM) 
				switch (ptrs_block->kod_obiektu)
				{
				case B_DIM: strcpy(ins_b.nazwa_bloku, "B_DIM");
					break;
				case B_DIM1: strcpy(ins_b.nazwa_bloku, "B_DIM1");
					break;
				case B_DIM2: strcpy(ins_b.nazwa_bloku, "B_DIM2");
					break;
                case B_DIM3: strcpy(ins_b.nazwa_bloku, "B_DIM3");
                        break;
				default: strcpy(ins_b.nazwa_bloku, "B_DIM");
					break;
				}
			else strcpy(ins_b.nazwa_bloku, "*D");
            ins_b.x0=0;
            ins_b.y0=0;
            ins_b.z0=0;
			if (ptrs_block->dlugosc_opisu_obiektu >= 4) memmove(&ins_b.angle, &ptrs_block->opis_obiektu[ptrs_block->dlugosc_opisu_obiektu - sizeof(float)], sizeof(float));
			else memmove(&ins_b.angle, &ptrs_block->opis_obiektu[0], sizeof(float));
           }
           else
           if (ptrs_block->kod_obiektu == B_PLINE)
           {
            /*przyjecie nazwy domyslnej*/
            strcpy(ins_b.nazwa_bloku,"B_PLINE");
            ins_b.x0=0;
            ins_b.y0=0;
            ins_b.z0=0;
           }
           else
           if (ptrs_block->kod_obiektu == B_HATCH)
           {
            /*przyjecie nazwy domyslnej*/
            strcpy(ins_b.nazwa_bloku,"B_HATCH");
			if (ptrs_block->dlugosc_opisu_obiektu > 1)
			{
			  ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu[0]);
			  ins_b.x0 = ptrs_desc_bl->x;
			  ins_b.y0 = ptrs_desc_bl->y;
			  ins_b.z0 = 0;
			}
			else
			{
		      ins_b.x0=0;
			  ins_b.y0=0;
			  ins_b.z0=0;
			}
           }
           else 
			   if (ptrs_block->kod_obiektu == B_EXPORT)
			   {
				   if (ptrs_block->dlugosc_opisu_obiektu > 1)
				   {
					   ptrs_desc_bl = (T_Desc_Ex_Block *)(&ptrs_block->opis_obiektu[0]);
					   if (ptrs_desc_bl->len > 1)
					   {
						   strcpy(ins_b.nazwa_bloku, ptrs_desc_bl->sz_type);
					   }
					   else
					   {
						   /*przyjecie nazwy domyslnej*/
						   strcpy(ins_b.nazwa_bloku, "B_EXPORT");
					   }
					   ins_b.x0 = ptrs_desc_bl->x;
					   ins_b.y0 = ptrs_desc_bl->y;
					   ins_b.z0 = 0;
				   }
				   else
				   {
					   /*przyjecie nazwy domyslnej*/
					   strcpy(ins_b.nazwa_bloku, "B");
					   ins_b.x0 = 0;
					   ins_b.y0 = 0;
					   ins_b.z0 = 0;
				   }

			   }
           else
            {
             /*przyjecie nazwy domyslnej*/
             strcpy(ins_b.nazwa_bloku,"B");
             ins_b.x0=0;
             ins_b.y0=0;
             ins_b.z0=0;
            }
           block_sufix+=1;
           sprintf(sufiks_bloku, "%#ld", block_sufix);
           
           if (strlen(ins_b.nazwa_bloku)>27) ins_b.nazwa_bloku[26]='\0';
           
		   if (strcmp(ins_b.nazwa_bloku, "*D")!=0) strcat(ins_b.nazwa_bloku,"-");   //nie moze byc "~"
           strcat(ins_b.nazwa_bloku,sufiks_bloku);  
           /*KOREKTA NAZWY BLOKU*/
		   encodingdxf((char *)&ins_b.nazwa_bloku);

           if (ptrs_block->flag & 2) ins_b.invisible=1;
           else ins_b.invisible=0;

		   /*funkcja RecordBlock zapisuje blok w TABLES*/
		   to_block = FALSE;
		   to_entities = FALSE;
		   if (RecordBlock(ins_b) == ErrorWeWy) return ErrorWeWy;

           /*funkcja IniBlock inicjuje blok w zbiorze blokow*/
           /*i dopisuje Insert do zbioru prymitywow*/
		   to_entities = TRUE;
           if(InsBlock(ins_b)==ErrorWeWy) return ErrorWeWy;

           to_block=TRUE;
		   to_entities = FALSE;
           if(IniBlock(ins_b)==ErrorWeWy) return ErrorWeWy;
          }
	  break;
	    case Olinia :
	  if(RysujLinia((LINIA *)nag)==ErrorWeWy) return ErrorWeWy;
	  break;
	    case Otekst :
		ptrs_text = (TEXT*)nag;
		if (ptrs_text->multiline)
		{
			if (RysujMText((TEXT*)nag) == ErrorWeWy) return ErrorWeWy;
		}
		else
		{
			if(RysujText((TEXT*)nag)==ErrorWeWy) return ErrorWeWy;
		}
	  break;
	    case Okolo :
	 if(RysujKolo((OKRAG*)nag)==ErrorWeWy) return ErrorWeWy;
	 break;
        case Ofilledellipse :
      if(RysujFilledEllipse((ELLIPSE*)nag)==ErrorWeWy) return ErrorWeWy;
      break;
        case Ookrag :
	 if(RysujOkrag((OKRAG*)nag)==ErrorWeWy) return ErrorWeWy;
	 break;
        case Oellipse :
      if(RysujEllipse((ELLIPSE*)nag)==ErrorWeWy) return ErrorWeWy;
      break;
        case Oluk :
		 if(RysujLuk((LUK*)nag)==ErrorWeWy) return ErrorWeWy;
	 break;
        case Oellipticalarc :
         if(RysujEllipticalArc((ELLIPTICALARC*) nag)==ErrorWeWy) return ErrorWeWy;
      break;
        case Owwielokat :
		 if(RysujSolid((WIELOKAT*)nag)==ErrorWeWy) return ErrorWeWy;
    break;
     case Osolidarc :
		 if(RysujSolidArc((SOLIDARC*)nag)==ErrorWeWy) return ErrorWeWy;
    break;
	   case Ospline:
		 if (RysujSpline((SPLINE*)nag) == ErrorWeWy) return ErrorWeWy;
	break; 
       case Opoint :
		 if(RysujPoint((T_Point*)nag, &p_block_sufix) == ErrorWeWy) return ErrorWeWy;
    break;
       case Ovector :
         if(RysujVector((AVECTOR*)nag, &v_block_sufix) == ErrorWeWy) return ErrorWeWy;
    break;
          case Opcx:
		 if (RysujImage((B_PCX*)nag) == ErrorWeWy) return ErrorWeWy;
	 break;
       default :
	 break;
      } /*switch*/
    obiekt_tok_all(NULL,adh, (char ** ) &nag,ONieOkreslony);
  }
  if (to_block==TRUE)
   {
     if(EndBlock()==ErrorWeWy) return ErrorWeWy;
     to_block=FALSE;
	 to_entities = TRUE;
   } 
return OkWeWy;
}

/*------------------------------------------------------------*/

extern "C" {

void DxfOut(int acad_version)
/*-------------------------*/
{
  char sk[MAXPATH] ;
  char st1[MaxTextLen];
  int key;

  strcpy (sk, "");
  image_no = 16;
  image_no_ = 16;
  if (Load_File (sk, SAVE_DXF, TRUE) == FALSE)
  {
     return;
  }
  File_Ext (sk, (char*)EXT__DXF) ;

#ifndef LINUX
  /*
  //sprawdzenie czy zbior juz istnieje
  if (my_file_exists(sk))
  {
	  strcpy(st1, _FILE_);
	  strcat(st1, sk);
	  strcat(st1, _FILE_EXISTS_);
	  komunikat_str(st1);
	  key = Get_Legal_Key((char*)_YES_NO_ESC_);
	  komunikat(0);
	  if (key != _YES_ && key != _yes_)
	  {
		  return;
	  }
  }
  */
#endif
  to_block=FALSE;
  to_entities = FALSE;

  if (acad_version == 0)
  {
    AcadV = ACADV26;
  }
  else
  {
    AcadV = ACADV10;  /*na razie*/
  }

  set_global_hidden_blocks_visibility(TRUE);
  My_App_PISZ_DXF_ASCII dxf(sk);
  dxf.Pisz_Dxf_ASCII();
  set_global_hidden_blocks_visibility(FALSE);
}

}


#undef __DXF_O__

