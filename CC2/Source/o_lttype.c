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

#define __O_LTTYPE__
#include <forwin.h>
#include <stddef.h>
#ifndef LINUX
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "bib_e.h"
#include "alffont.h"
#include "rysuj_e.h"
#include "b_buf.h"
#include "b_menu.h"
#include "o_lttype.h"
#include "o_libfun.h"
#include "glyph.h"
#include "o_loadf.h"

#include "forlinux.h"

#include "message.h"
#include "menu.h"

////#include "leak_detector_c.h"

GLYPH_FACE *face_ttf[128];
TTF_font_files ttf_font_files;
char *ttf_file[128];
char *ttf_face[128];

char *kochimincho[]={"kochimincho\0"};

extern int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext);
static int alfacad_logo_font=0;
extern void Free_Desktop_font(void);
extern char *winfont;
extern char *otffont;
extern int my_file_exists(char *name);
extern BOOL enforce_redraw;
extern void find_any_font_face(char *Font_File, char *face_name);
extern int do_llf_file(int argc, char* argv[]);
extern int do_aft_file(int argc, char *argv[]);
extern char *alft;

extern void utf8Upper(char* text);
extern void Free_ini_font(void);
extern int findfile_recursive(char *folder,const char *filename, char *fullpath);
extern void Free_alft(void);

#ifdef LINUX
extern off_t tell(int fd);
#endif


#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

/*------------  ReadTypyLinii  ----------------------------------*/

#define FILE_LINE_TYPE "RYSUJ.LIN"   	     /*zbior z typami linii*/
#define MAXLINE 100         /*max. dlugosc linii zbioru konfiguracyjnego*/


int i_font_no = 0;

typedef enum
{
    ID_Lin_Kreskowa = 0,
    ID_Lin_Punktowa,
    ID_Lin_Dwupunktowa,
    ID_Lin_Wielopunktowa,
    ID_Lin_Granica,
    ID_Lin_Srodkowa,
    ID_Lin_Ukryta,
    ID_Lin_Widmo,
    ID_Lin_Kreskowa_2,
    ID_Lin_Punktowa_2,
    ID_Lin_Dwupunktowa_2,
    ID_Lin_Wielopunktowa_2,
    ID_Lin_Granica_2,
    ID_Lin_Srodkowa_2,
    ID_Lin_Ukryta_2,
    ID_Lin_Widmo_2,
    ID_Lin_Kreskowa_x2,
    ID_Lin_Punktowa_x2,
    ID_Lin_Dwupunktowa_x2,
    ID_Lin_Wielopunktowa_x2,
    ID_Lin_Granica_x2,
    ID_Lin_Srodkowa_x2,
    ID_Lin_Ukryta_x2,
    ID_Lin_Widmo_x2,
    ID_Lin_Trzypunktowa,
    ID_Lin_Dwusrodkowa,
    ID_Lin_Trzykreskowa,
    ID_Lin_Widmo_3,
    ID_Lin_Dwusrodkowa_x2,
    ID_Lin_Trzykreskowa_x2,
    ID_Lin_Wielokreskowa,
}ID_Lin_Config;

int no_lin_config_param = sizeof(lin_config_param) / sizeof(lin_config_param[0]);

static void get_config0_lin (void);
static BOOL get_config_lin (void);
static BOOL get_config_param (T_Fstring, T_Fstring);
static T_Font_Header *read_font_TTF_OTF(char *ptrsz_file, int no);
int Add_New_Font_TTF_OTF(char *name, int type, int cur);
static T_Font_Header *read_font_old(char *ptrsz_file);
int Add_New_Font_CHR(char *name);
static T_Font_Header *read_font(char *ptrsz_file);
int Add_New_Font_AFF(char *name);

FontTable PTRS__Font_Table;


static BOOL
get_config_param (T_Fstring key_name, T_Fstring ret_string)
/*-------------------------------------------------------*/
{
   int 		i;
   T_Fstring	p;
   LTYPE	typ_linii;
   int		line_type_number  ;   /* typ 0 linia ciagla*/
   char *mylin_config_param;

   for (i = 0; i < no_lin_config_param; i++)
   {
	   mylin_config_param = &lin_config_param[i];

      if (stricmp (key_name, mylin_config_param) == 0)
      {
	    break;
      }
   }
   if (i >= no_lin_config_param)
   {
      return FALSE;
   }
   line_type_number = i + 1;	/*linia ciagla numer zero*/
   strncpy ( typ_linii.nazwa, key_name, MaxLenNameLine);
   typ_linii.nazwa [MaxLenNameLine] = '\0';

   p = ret_string;
   p = strtok (p, ",");
   if (p == NULL)
   {
      return FALSE;
   }
   for ( i = 0; i < MaxLiczbaKresekLinii ; i++)
   {
       if ( sscanf ( p , "%lf", &(typ_linii.kreski[i]) ) < 1 ) break;
       p = strtok (NULL, ",");
       if (p == NULL)
       {
	   i++;
	   break;
       }
   }
   if (i == 0)
   {
     return FALSE;
   }
   typ_linii.IloscElementow = i;
   typ_linii.dlwzorca = 0;
   for (i = 0; i < typ_linii.IloscElementow; i++)
   {
       typ_linii.dlwzorca += fabs (  typ_linii.kreski [i]  );
   }

   memmove (  (void *)&(TypyLinii[line_type_number]), 
	(void  *)&typ_linii, sizeof(LTYPE));

   return TRUE;
}


static BOOL get_config_lin (void)
/*-------------------------------*/
{
  get_config0_lin ();
  return Get_Private_Profile_Strings ((T_Fstring)LINE, get_config_param);
}


static void get_config0_lin (void)
/*-------------------------------*/
{
}

int ReadTypyLinii (void)
/*---------------------*/
{ int i;

  LiczbaTypowLinii=32;
  LiczbaWzorcowLinii=32; 

  get_config_lin ();

  strcpy (TypyLinii [0].nazwa, "Continuous");  //"CONTINUOUS"
  TypyLinii [0].dlwzorca=0;
  TypyLinii [0].IloscElementow=0;
  
  for (i = 0; i < 32; i++)
  {
    TypyLinii [i].opis=i;
  }

  return 0;
}


/*----------  ReadTextStyle  ------------------------------------*/

#define BUF_FONTS_SIZE 15000

extern PPOLE pmCzcionka [] ;
extern PTMENU mCzcionka ;
extern PTMENU mCzcionkaW;
extern PTMENU mCzcionkaZ;
int i__font_nomax = 0 ;
int i__font_namelenmax = 13;
static int i__font_nomax_DXF = 0;
static char *ptrs__buf_fonts = NULL ;
static unsigned u__buf_fonts_size = 0 ;
static unsigned u__buf_fonts_size_check ;

#define GLYPH_TARGET GLYPH_TARGET_ALLEGRO

char *Get_Ptr_Buf_Fonts (void)
{
  return ptrs__buf_fonts ;
}

int Add_New_Font_TTF_OTF(char *name, int type, int cur)
{
	int i;
	char sk[MAXPATH] = "";
	char dir[MAXDIR]="";
	char drive[MAXDRIVE]="";
	char file[MAXFILE]="";
	char ext[MAXEXT]="";
	int flags;
	GLYPH_FACE *new_face_ttf;
	int font_no;
	int font_namelen;
	char file_ext[32];
	char face_name[128] = "";

	if (name == NULL)
	{
        if (type==0) {
            if (Load_File(sk, INI_TTF_FONT, cur) == FALSE) {
                return -1;
            }
        }
        else
        if (Load_File(sk, INI_OTF_FONT, cur) == FALSE) {
            return -1;
        }

		find_any_font_face(sk, &face_name);

		for (i = 0; i < WhNumberTextStyle; i++)
		{

			if ((ttf_font_files.Alfa_loaded[i] == TRUE) &&
				(strcmp(ttf_font_files.face[i], face_name) == 0))
			{
				//face already exists
				return i;
			}
		}

		//so doesn't exist, font will be added;
	}
	else
	{
	    sprintf(sk, "%s.ttf", name);
		flags = fnsplit(sk, drive, dir, file, ext);
	}

	
	font_no = WhNumberTextStyle;
	if (NULL == (PTRS__Text_Style[font_no] =
		read_font_TTF_OTF(sk, font_no)) ||
		0 == PTRS__Text_Style[font_no]->place_xms)
	{
		//maybe otf ?
        sprintf(sk, "%s.otf", name);
        flags = fnsplit(sk, drive, dir, file, ext);
        if (NULL == (PTRS__Text_Style[font_no] =
                             read_font_TTF_OTF(sk, font_no)) ||
            0 == PTRS__Text_Style[font_no]->place_xms)
            return -1;
	}

	s__Fonts_Style[font_no].df_width_line = 0.075;  //average

	s__Fonts_Style[font_no].df_width_factor = 0.85;  //average

    pmCzcionka[font_no].txt = &(PTRS__Text_Style[WhNumberTextStyle]->font_name[0]);
	font_namelen = strlen(PTRS__Text_Style[WhNumberTextStyle]->font_name);

	WhNumberTextStyle++;
	i__font_nomax++;

	mCzcionka.max = i__font_nomax;

    pmCzcionka[i__font_nomax].txt = add_new_font;
    pmCzcionka[i__font_nomax-1].menu=NULL;
    pmCzcionka[i__font_nomax].menu=&mTTF_OTF;
	mCzcionka.max++;

	mCzcionkaW.max = mCzcionka.max;
	mCzcionkaZ.max = mCzcionka.max;

	if (font_namelen > (i__font_namelenmax + 2))
	{
		i__font_namelenmax = i__font_namelenmax + 2;
		mCzcionka.xdl = mCzcionkaW.xdl = mCzcionkaZ.xdl = i__font_namelenmax;
	}

	return font_no;

}

static T_Font_Header *read_font_TTF_OTF(char *ptrsz_file, int no)
/*-------------------------------------------------------------*/
{
	int flags;
	char drive[MAXDRIVE]="";
	char dir[MAXDIR]="";
	char file[MAXFILE]="";
	char ext[MAXEXT]="", ext_[MAXEXT]="";
	T_Font_Header *ptrs_header, s_header;
	char file_ext[32]="";
	long l_alloc;
	char pathname[MAXPATH]="";
	char ttf_file_path[MAXPATH]="";
	GLYPH_FACE *new_face_ttf;
	int path_len, face_len;
	int i;
	char face_name[128]="";
	int ret_err;
	char lff_name[64]="", aft_name[64]="", aft_font[64]="";
    char *fontdir;


    flags = fnsplit(ptrsz_file, drive, dir, file, ext);

	if (my_file_exists(ptrsz_file)) strcpy(ttf_file_path, ptrsz_file);
	else
	{
        strcpy(ext_, ext);
		/*
#ifndef LINUX
        if (strcmp(_strupr(ext_),".OTF")==0)
#else
        if (strcmp(strupr(ext_),".OTF")==0)
#endif
            fontdir=otffont;
        else fontdir=winfont;
        if (!findfile_recursive(fontdir, ptrsz_file, &ttf_file_path)) return NULL;
		*/
#ifndef LINUX
        if (strcmp(_strupr(ext_),".OTF")==0)
#else
        if (strcmp(strupr(ext_),".OTF")==0)
#endif
		{
			fontdir = otffont;
			if (!findfile_recursive(fontdir, ptrsz_file, &ttf_file_path))
			{
				fontdir = winfont;
				if (!findfile_recursive(fontdir, ptrsz_file, &ttf_file_path)) return NULL;
			}
		}
		else
		{
			fontdir = winfont;
			if (!findfile_recursive(fontdir, ptrsz_file, &ttf_file_path))
			{
				fontdir = otffont;
				if (!findfile_recursive(fontdir, ptrsz_file, &ttf_file_path)) return NULL;
			}
		}
	}

	find_any_font_face(ttf_file_path, &face_name);

    if (strlen(face_name)==0) strcpy(&face_name, &file);  //TO CHANGE

	for (i = 0; i < i__font_nomax /*WhNumberTextStyle*/; i++)
	{

		if ((ttf_font_files.Alfa_loaded[i] == TRUE) &&
			(strcmp(ttf_font_files.face[i], face_name) == 0))
		{
			//face already exists
			return NULL;
		}
	}

	strncpy(s_header.font_name, file, FONTNAMELEN-1);
	s_header.font_name[FONTNAMELEN-1] = '\0';
	s_header.num_char = 0;

	sprintf(file_ext, "%s%s", file, ext);

	pathname[0] = '\0';
	strcat(pathname, file);
#define EXT ".TMP"
	strcat(pathname, EXT);
	ptrs_header = NULL;

	strncpy(s_header.s_alloc_param.file_buf, pathname,13);
	l_alloc = sizeof(T_Font_Header) + s_header.num_char * sizeof(char);
    ptrs_header = (T_Font_Header*)malloc(l_alloc);
	if (ptrs_header == NULL)
	{
		ErrList(12);
		return ptrs_header;
	}
	memcpy((void*)ptrs_header, &s_header, sizeof(T_Font_Header));
	ptrs_header->ptrsz_widths = (unsigned char*)(ptrs_header + 1);

	face_ttf[no] = gk_load_face_from_file(ttf_file_path, 0);  //ptrsz_file
	if (face_ttf[no] == NULL) 
		return NULL;

	//so doesn't exist, font will be added;

	path_len = strlen(ttf_file_path) + 1;
	ttf_file[no] = (char *)malloc(path_len);
	strcpy(ttf_file[no], ttf_file_path);
	ttf_font_files.file[no] = ttf_file[no];

	face_len = strlen(face_name) + 1;
	ttf_face[no] = (char *)malloc(face_len);
	strcpy(ttf_face[no], face_name);
	ttf_font_files.face[no] = ttf_face[no];

	ttf_font_files.Alfa_loaded[no] = TRUE;
	ttf_font_files.PDF_embeded[no] = FALSE;
	
	strncpy(ptrs_header->font_acad_name, file_ext, 33);
	ptrs_header->font_acad_name[33] = '\0';

	ptrs_header->ptrsz_font = NULL;

	ptrs_header->type = 2;
	ptrs_header->place_xms = 1;
	ptrs_header->place_buf = 0;
	ptrs_header->place_buf_font = 0;
	return ptrs_header;
}

int Add_New_Font_CHR(char *name)
{
	int i;
	char sk[MAXPATH] = "";
	char dir[MAXDIR]="";
	char drive[MAXDRIVE]="";
	char file[MAXFILE]="";
	char ext[MAXEXT]="";
	int flags;
	int font_no;
	int font_namelen;

#ifndef LINUX
	sprintf(sk, "FONTS\\CHR\\%s.chr",name);
#else
    sprintf(sk, "Fonts/CHR/%s.CHR",name);
#endif
	flags = fnsplit(sk, drive, dir, file, ext);

	font_no = WhNumberTextStyle;
	if (NULL == (PTRS__Text_Style[font_no] =
		read_font_old(sk/*, font_no*/)) ||
		0 == PTRS__Text_Style[font_no]->place_xms)
	{
		return -1;
	}
	s__Fonts_Style[font_no].df_width_line = 0.1;
	strcpy(PTRS__Text_Style[font_no]->font_acad_name, file);
	s__Fonts_Style[font_no].df_width_factor = 1.0;

    pmCzcionka[font_no].txt = &(PTRS__Text_Style[WhNumberTextStyle]->font_name[0]);
	font_namelen = strlen(PTRS__Text_Style[WhNumberTextStyle]->font_name);

	WhNumberTextStyle++;
	i__font_nomax++;

	mCzcionka.max = i__font_nomax;

    pmCzcionka[i__font_nomax].txt =add_new_font;
    pmCzcionka[i__font_nomax-1].menu=NULL;
    pmCzcionka[i__font_nomax].menu=&mTTF_OTF;
	mCzcionka.max++;

	mCzcionkaW.max = mCzcionka.max;
	mCzcionkaZ.max = mCzcionka.max;

	if (font_namelen > (i__font_namelenmax + 2))
	{
		i__font_namelenmax = i__font_namelenmax + 2;
		mCzcionka.xdl = mCzcionkaW.xdl = mCzcionkaZ.xdl = i__font_namelenmax;
	}

	return font_no;

}

static T_Font_Header *read_font_old(char *ptrsz_file)
/*-----------------------------------------------------*/
{
	int f;
	unsigned roz1;
	T_Font_Header_Old s_header_old;
	T_Font_Header s_header;
	T_Font_Header *ptrs_header;
	T_Font_File_Header s_file_header;
	long l_alloc;
	char pathname[MAXPATH]="";
	char drive[MAXDRIVE]="";
	char dir[MAXDIR]="";
	char file[MAXFILE]="";
	char ext[MAXEXT]="";

	char pathname0[MAXPATH]="";
	char drive0[MAXDRIVE]="";
	char dir0[MAXDIR]="";
	char file0[MAXFILE]="";
	char ext0[MAXEXT]="";
	char font_file[MAXPATH]="";
	int flags, flags1;
	long l1, l2;
	char *bufor_testowy;
	unsigned int was_read;
	int poz_widths;
	int  header_size, header_old_size, file_header_size;


	strcpy(font_file, "");

#ifdef NET_VERSION
	flags1 = fnsplit(argv0, drive0, dir0, file0, ext0);
	strcat(font_file, drive0);
	strcat(font_file, dir0);
#endif
	strcat(font_file, ptrsz_file);
	flags = fnsplit(ptrsz_file, drive, dir, file, ext);
	pathname[0] = '\0';
	strcat(pathname, file);
#define EXT ".TMP"
	strcat(pathname, EXT);
	ptrs_header = NULL;
	f = open(font_file, O_RDONLY | O_BINARY);
	if (f == -1)
	{

		strcat(str, font_file /*ptrsz_file*/);
		ErrListStr(str);
		return ptrs_header;
	}

	file_header_size = sizeof(T_Font_File_Header);

	if (read(f, &s_file_header, sizeof(T_Font_File_Header)) != sizeof(T_Font_File_Header))
	{
		goto err;
	}

	if ((0 != strncmp(s_file_header.comment, FONTVER, FONTVER_LEN)) && (0 != strncmp(s_file_header.comment, FONTVERNEW, FONTVER_LEN)))
	{
		goto err;
	}

	if (read(f, &s_header_old, sizeof(T_Font_Header_Old)) != sizeof(T_Font_Header_Old))
	{
		goto err;
	}

	strcpy(&s_header.font_name, &s_header_old.font_name);
	strcpy(&s_header.font_acad_name, &s_header_old.font_acad_name);
	s_header.font_size = s_header_old.font_size;
	s_header.num_char = (unsigned int)s_header_old.num_char;
	s_header.first_char = (unsigned int)s_header_old.first_char;
	s_header.height = s_header_old.height;
	s_header.vert_margin = s_header_old.vert_margin;
	s_header.margin_char = s_header_old.margin_char;
	s_header.last_height = s_header_old.last_height;
	s_header.font_aspect = s_header_old.font_aspect;
	s_header.place_xms = s_header_old.place_xms;
	s_header.place_buf = s_header_old.place_buf;
	s_header.place_buf_font = s_header_old.place_buf_font;
	s_header.type = s_header_old.type;
	s_header.reserve = s_header_old.reserve;
	s_header.s_alloc_param = s_header_old.s_alloc_param;

#ifdef BIT64
#ifndef LINUX
  poz_widths = lseek(f, -12, SEEK_CUR);
#else
#ifdef ARM64
	poz_widths = lseek(f, -24, SEEK_CUR);
#else
    poz_widths = lseek(f, -16, SEEK_CUR);
#endif
#endif
#endif
    header_old_size = sizeof(T_Font_Header_Old);
	header_size = sizeof(T_Font_Header);

	strncpy(s_header.s_alloc_param.file_buf, pathname, 13);
	l_alloc = sizeof(T_Font_Header) + s_header.num_char * sizeof(char);
    ptrs_header = (T_Font_Header*)malloc(l_alloc);
	if (ptrs_header == NULL)
	{
		ErrList(12);
		close(f);
		return ptrs_header;
	}
	memcpy((void*)ptrs_header, &s_header, sizeof(T_Font_Header));
	ptrs_header->ptrsz_widths = (unsigned char*)(ptrs_header + 1);
	ptrs_header->place_xms = 1;
	ptrs_header->ptrsz_font = NULL;

	l_alloc -= sizeof(T_Font_Header);


	if (read(f, (void*)(ptrs_header->ptrsz_widths), (unsigned)l_alloc) != l_alloc)
	{
		goto err;
	}
	l_alloc = (s_header.num_char) * sizeof(T_Font_Off) + s_header.font_size -12;  //64bit  -12  ???
    bufor_testowy = (char *)malloc(l_alloc + 1000);  //(24000);  //16000

	was_read = read(f, (void*)bufor_testowy, (unsigned)l_alloc);
	if (was_read != l_alloc)
	{

		goto err;
	}

	ptrs_header->ptrs_off = (T_Font_Off*)bufor_testowy;

	ptrs_header->ptrsz_font = (char*)ptrs_header->ptrs_off + (s_header.num_char * sizeof(T_Font_Off));

	ptrs_header->place_xms = 1;
	ptrs_header->place_buf = 0;
	ptrs_header->place_buf_font = 0;
	close(f);
	return ptrs_header;
err:
	//My_Free(ptrs_header);
    free(ptrs_header);
	close(f);
	ErrList(11);
	return NULL;
}

int Add_New_Font_AFF(char *name)
{
	int i;
	char sk[MAXPATH] = "";
	char dir[MAXDIR]="";
	char drive[MAXDRIVE]="";
	char file[MAXFILE]="";
	char ext[MAXEXT]="";
	int flags;
	int font_no;
	int font_namelen;


#ifndef LINUX
	sprintf(sk, "FONTS\\AFF\\%s.aff", name);
#else
    sprintf(sk, "Fonts/AFF/%s.aff", name);
#endif
	flags = fnsplit(sk, drive, dir, file, ext);

	font_no = WhNumberTextStyle;
	if (NULL == (PTRS__Text_Style[font_no] =
		read_font(sk/*, font_no*/)) ||
		0 == PTRS__Text_Style[font_no]->place_xms)
	{
		return -1;
	}
	s__Fonts_Style[font_no].df_width_line = 0.1;
	strcpy(PTRS__Text_Style[font_no]->font_acad_name, file);
	s__Fonts_Style[font_no].df_width_factor = 1.0;

    pmCzcionka[font_no].txt = &(PTRS__Text_Style[WhNumberTextStyle]->font_name[0]);
	font_namelen = strlen(PTRS__Text_Style[WhNumberTextStyle]->font_name);

	WhNumberTextStyle++;
	i__font_nomax++;

	mCzcionka.max = i__font_nomax;

    pmCzcionka[i__font_nomax].txt = add_new_font;
    pmCzcionka[i__font_nomax-1].menu=NULL;
    pmCzcionka[i__font_nomax].menu=&mTTF_OTF;
	mCzcionka.max++;

	mCzcionkaW.max = mCzcionka.max;
	mCzcionkaZ.max = mCzcionka.max;

	if (font_namelen > (i__font_namelenmax + 2))
	{
		i__font_namelenmax = i__font_namelenmax + 2;
		mCzcionka.xdl = mCzcionkaW.xdl = mCzcionkaZ.xdl = i__font_namelenmax;
	}

	return font_no;

}

static T_Font_Header *read_font (char *ptrsz_file)
/*-----------------------------------------------------*/
{
  int f ;
  unsigned roz1 ;
  T_Font_Header *ptrs_header, s_header ;
  T_Font_File_Header s_file_header ;
  long l_alloc ;
  char pathname [MAXPATH]="";
  char drive [MAXDRIVE]="" ;
  char dir [MAXDIR]="" ;
  char file [MAXFILE]="" ;
  char ext [MAXEXT]="" ;
  
  char pathname0 [MAXPATH]="" ;
  char drive0 [MAXDRIVE]="" ;
  char dir0 [MAXDIR]="" ;
  char file0 [MAXFILE]="" ;
  char ext0 [MAXEXT]="" ;
  char font_file [MAXPATH]="";
  int flags, flags1 ;
  long l1,l2;
  char *bufor_testowy;
  unsigned int was_read;
  int poz_widths;
  int  header_size, file_header_size;


  strcpy(font_file, "");
  
#ifdef NET_VERSION
  flags1 = fnsplit (argv0, drive0, dir0, file0, ext0);
  strcat(font_file, drive0);
  strcat(font_file, dir0);
#endif
  strcat(font_file, ptrsz_file);
  flags = fnsplit (ptrsz_file, drive, dir, file, ext) ;
  flags = flags ;
  pathname [0] = '\0' ;
  strcat (pathname, file) ;
#define EXT ".TMP"
  strcat (pathname, EXT) ;
  ptrs_header = NULL ;
  f = open (font_file , O_RDONLY | O_BINARY) ;
  if (f == -1)
  {

     strcat (str, font_file ) ;
     ErrListStr (str) ;
     return ptrs_header ;
   }

  file_header_size = sizeof(T_Font_File_Header);

  if (read (f, &s_file_header, sizeof(T_Font_File_Header)) != sizeof(T_Font_File_Header))
  {
    goto err ;
  }

  if ((0 != strncmp (s_file_header.comment, FONTVER, FONTVER_LEN)) && (0 != strncmp(s_file_header.comment, FONTVERNEW, FONTVER_LEN)))
  {
    goto err ;
  }
   
  if (read (f, &s_header, sizeof(T_Font_Header)) != sizeof(T_Font_Header))
  {
    goto err ;
  }

  //if 64bit we move back 12 bytes
#ifdef BIT64
#ifdef LINUX
#endif
#else
  //poz_widths = lseek(f, +12, SEEK_CUR);  //-12
  poz_widths = lseek(f, +12, SEEK_CUR);  //-12
#endif
  header_size = sizeof(T_Font_Header);

  strncpy (s_header.s_alloc_param.file_buf, pathname, 13) ;
  l_alloc = sizeof(T_Font_Header) + s_header.num_char * sizeof(char) ;
    ptrs_header = (T_Font_Header*)malloc (l_alloc) ;
  if (ptrs_header == NULL)
  {
    ErrList (12) ;
    close (f) ;
    return ptrs_header;
  }
  memcpy ((void*)ptrs_header, &s_header, sizeof(T_Font_Header)) ;
  ptrs_header->ptrsz_widths = (unsigned char*)(ptrs_header + 1) ;
  ptrs_header->place_xms = 1 ;
  ptrs_header->ptrsz_font = NULL ;

  l_alloc -= sizeof(T_Font_Header) ;

#ifdef LINUX
    lseek(f, -2, SEEK_CUR);
#endif
#ifdef ARM64
	lseek(f, -4, SEEK_CUR);
#endif

   
  if (read (f, (void*)(ptrs_header->ptrsz_widths), (unsigned)l_alloc) != l_alloc)
  {

    goto err ;
  }
  l_alloc = (s_header.num_char) * sizeof(T_Font_Off) + s_header.font_size; // -12;  //64bit  -12  ???

  bufor_testowy = (char *)malloc (l_alloc+1000);  //(24000);  //16000

#ifdef LINUX
    //lseek(f, -2, SEEK_CUR);
#endif

  was_read = read(f, (void*)bufor_testowy, (unsigned)l_alloc);
  if (was_read != l_alloc)
  {
    goto err ;
  }


   ptrs_header->ptrs_off=(void*)bufor_testowy;

  ptrs_header->ptrsz_font = (char*)ptrs_header->ptrs_off + (s_header.num_char * sizeof(T_Font_Off));

  ptrs_header->place_xms = 1 ;
  ptrs_header->place_buf = 0 ;
  ptrs_header->place_buf_font = 0 ;
  close (f) ;
  return ptrs_header;
err :
  //My_Free (ptrs_header) ;
  free (ptrs_header) ;
  close (f) ;
  ErrList (11) ;
  return NULL ;
}

void DoneBuffers3(void)
{ int i;
char str[16];
int gk;
  
  for (i=0; i< i__font_nomax; i++)  //MaxNumberTextStyle
	  if ((char *)PTRS__Text_Style[i]!=NULL) 
	  {
		  if ((PTRS__Text_Style[i]->type < 2) || (PTRS__Text_Style[i]->type == 3))
		  {
			  free((char *)PTRS__Text_Style[i]->ptrs_off);
			  free((char *)PTRS__Text_Style[i]);
		  }
		  
		  else
		  {
              ////  !!!!!!!!!!!!!!! NO IDEA IF NEEDED, HAS TO BE CONFIRMED
			  if (face_ttf[i]!=NULL)
              {
                  //   gk_unload_face(face_ttf[i]);  //this can crash after operations in Dialog
                  face_ttf[i] = NULL;
              }

			  free((char *)PTRS__Text_Style[i]);
		  }
		  //freeing ttf_path_names
		  if (ttf_font_files.Alfa_loaded[i] == TRUE)
		  {
			  free(ttf_font_files.file[i]);
			  free(ttf_font_files.face[i]);
		  }
	  }

  Free_Desktop_font();
  Free_ini_font();
  Free_alft();

  size_t gk_bytes=gk_bytes_allocated();

  ////gk_library_cleanup();  //is better to ignore that, after operations in Dialog it crashes at the end
}

int get_alfacad_logo_font(void)
{
	return alfacad_logo_font;
}


static BOOL
get_fonts_param (T_Fstring key_name, T_Fstring ret_string)
/*------------------------------------------------------*/
{
  double df_width_factor, df_width_line ;
  char *ptrsz_font_acad_name, *ptrsz_width_factor, *ptrsz_width_line, *endptr ;
  char drive[MAXDRIVE]="";
  char dir[MAXDIR]="";
  char file[MAXFILE]="";
  char ext[MAXEXT]="";
  int flags;
  char file_ext[32];
  int font_namelen = 0;

  if (key_name[0] == ';') return FALSE;

  if (ret_string == NULL) return FALSE ;

  utf8Upper(key_name);

  ptrsz_font_acad_name = NULL ;
  df_width_line = 0.1 ;
  
  if (NULL != strtok (ret_string, " ,\t"))
  {
    ptrsz_width_line = strtok (NULL, " ,\t") ;
    if (NULL != ptrsz_width_line)
    {
     df_width_line = strtod (ptrsz_width_line, &endptr) ;
     if (df_width_line<0)
     {
       df_width_line = 0.0 ;  /*grubosc minimalna*/
     }
    }

    ptrsz_font_acad_name = strtok (NULL, " ,\t") ;
    ptrsz_font_acad_name = Ignore_Spacing (ptrsz_font_acad_name) ;
    ptrsz_width_factor = strtok (NULL, " ,\t") ;
    df_width_factor = 1.0 ;
    if (NULL != ptrsz_width_factor)
    {
      df_width_factor = strtod (ptrsz_width_factor, &endptr) ;
      if (0.001 > df_width_factor)
      {
        df_width_factor = 1.0 ;
      }
    }
  }

  flags = fnsplit(ret_string, drive, dir, file, ext);
  sprintf(file_ext, "%s%s", file, ext);

#ifndef LINUX
  if ((strcmp(_strupr(ext),".TTF")==0) || (strcmp(_strupr(ext),".OTF")==0))
#else
    if ((strcmp(strupr(ext),".TTF")==0) || (strcmp(strupr(ext),".OTF")==0))
#endif
  {
	  if (NULL == (PTRS__Text_Style[i_font_no] =
		  read_font_TTF_OTF(ret_string, i_font_no)) ||
		  0 == PTRS__Text_Style[i_font_no]->place_xms)
	  {
		  return FALSE;
	  }

	  s__Fonts_Style[i_font_no].df_width_line = df_width_line;

	  if (NULL != ptrsz_font_acad_name)
	  {
		  strcpy(PTRS__Text_Style[i_font_no]->font_acad_name, ptrsz_font_acad_name);
		  s__Fonts_Style[i_font_no].df_width_factor = df_width_factor;
	   }

      pmCzcionka[i_font_no].txt = &(PTRS__Text_Style[i_font_no]->font_name[0]);
      pmCzcionka[i_font_no].menu=NULL;
	  font_namelen = strlen(&PTRS__Text_Style[i_font_no]->font_name[0]);

	  i_font_no++;
  }
#ifndef LINUX
  else if (strcmp(_strupr(ext), ".CHR") == 0)
#else
    else if (strcmp(strupr(ext), ".CHR") == 0)
#endif
  {
	  if (NULL == (PTRS__Text_Style[i_font_no] =
		  read_font_old(ret_string)) ||
		  0 == PTRS__Text_Style[i_font_no]->place_xms)
	  {
		  return FALSE;
	  }

	  s__Fonts_Style[i_font_no].df_width_line = df_width_line;

	  if (NULL != ptrsz_font_acad_name)
	  {
		  strcpy(PTRS__Text_Style[i_font_no]->font_acad_name, ptrsz_font_acad_name);
		  s__Fonts_Style[i_font_no].df_width_factor = df_width_factor;
	  }

      pmCzcionka[i_font_no].txt = &(PTRS__Text_Style[i_font_no]->font_name[0]);
      pmCzcionka[i_font_no].menu=NULL;
	  font_namelen = strlen(&PTRS__Text_Style[i_font_no]->font_name[0]);

	  i_font_no++;

  }
  else
  {
		  if (NULL == (PTRS__Text_Style[i_font_no] =
			  read_font(ret_string)) ||
			  0 == PTRS__Text_Style[i_font_no]->place_xms)
		  {
			  return FALSE;
		  }

		  s__Fonts_Style[i_font_no].df_width_line = df_width_line;

		  if (NULL != ptrsz_font_acad_name)
		  {
			  strcpy(PTRS__Text_Style[i_font_no]->font_acad_name, ptrsz_font_acad_name);
			  s__Fonts_Style[i_font_no].df_width_factor = df_width_factor;
		  }

          pmCzcionka[i_font_no].txt = &(PTRS__Text_Style[i_font_no]->font_name[0]);
          pmCzcionka[i_font_no].menu=NULL;
		  font_namelen = strlen(&PTRS__Text_Style[i_font_no]->font_name[0]);

          int same=(strcmp(pmCzcionka[i_font_no].txt, kochimincho));
          if (same==0)
		  {
			 alfacad_logo_font = i_font_no;
		  }

		  i_font_no++;
  }

	  if (i__font_nomax < (i_font_no))
	  {
		  i__font_nomax = (i_font_no);
	  }
	  
	  if (i__font_namelenmax < (font_namelen + 2))
	  {
		  i__font_namelenmax = (font_namelen + 2);
	  }

  return TRUE;
}

static BOOL
get_fonts_param_DXF (T_Fstring key_name, T_Fstring ret_string)
/*----------------------------------------------------------*/
{
  double df_width_factor, df_width_line ;
  char *ptrsz_width_factor, *endptr ;
  int i, i_font;

  //strupr (ret_string);
  utf8Upper(key_name);
  
  if (NULL != strtok (ret_string, " ,\t"))
  {
    ptrsz_width_factor = strtok (NULL, " ,\t") ;
    df_width_factor = 1.0 ;
    if (NULL != ptrsz_width_factor)
    {
      df_width_factor = strtod (ptrsz_width_factor, &endptr) ;
      if (0.001 > df_width_factor)
      {
        df_width_factor = 1.0 ;
      }
    }
  }
  
  if ((strcmp(key_name, "\r")!=0) && (ret_string!=NULL) && (i__font_nomax_DXF < MaxNumberTextStyleDXF))
  {
	  strcpy(PTRS__Text_Style_DXF[i__font_nomax_DXF].font_acad_name, key_name);
	  strcpy(PTRS__Text_Style_DXF[i__font_nomax_DXF].font_alf_name, ret_string);

	  PTRS__Text_Style_DXF[i__font_nomax_DXF].df_width_factor = df_width_factor;

	  i__font_nomax_DXF += 1;
  }
    
  return TRUE ;
}

static void free_EMB_font_mem (void)
{
  int i ;
}


void  SetDefaultStyles(void)
{
	int i, j;
	int new_font;
	char Default_Table[15][FONTNAMELEN] = { "POLTECH","POLTECH1","ROMAN","ROMANS","ROMANC","ROMAND","ROMANT",
										  "ITALICC","ITALICT","BLOK","FUTURA","SCRIPTC","SCRIPTS","SYSTEM","SYSTEM1"};

	for (i = 0; i < MaxNumberTextStyle; i++) PTRS__Font_Table.new_no[i] = 0;
	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < i__font_nomax; j++)
		{
			if (strcmp(Default_Table[i], PTRS__Text_Style[j]->font_name) == 0)
			{
				PTRS__Font_Table.new_no[i] = j;
				PTRS__Font_Table.type[i] = 0;
				break;
			}
		}
		if (j == i__font_nomax)
		{
			new_font = Add_New_Font_CHR(Default_Table[i]);
			if (new_font > -1) PTRS__Font_Table.new_no[i] = new_font;
			else
			{
				PTRS__Font_Table.new_no[i] = 0;  //default
				enforce_redraw = TRUE;
			}
		}
	}
}

int find_font_ALFA(char *stylefile, double *e_wspx1, int type)
{
	int j;
	char fontstyle_font_name[34];
	char Style_font_name_ACAD[34];

	strncpy(fontstyle_font_name, stylefile, 33);
	fontstyle_font_name[33] = '\0';
	strupr(fontstyle_font_name);

	for (j = 0; j < i__font_nomax; j++)
	{
		strncpy(Style_font_name_ACAD, PTRS__Text_Style[j]->font_acad_name, 33);
		Style_font_name_ACAD[33] = '\0';
		strupr(Style_font_name_ACAD);

		if ((strcmp(fontstyle_font_name, Style_font_name_ACAD) == 0) &&
			PTRS__Text_Style[j]->type == type) return j;

        //alternatively
        strncpy(Style_font_name_ACAD, PTRS__Text_Style[j]->font_name, 33);
        Style_font_name_ACAD[33] = '\0';
        strupr(Style_font_name_ACAD);

        if ((strcmp(fontstyle_font_name, Style_font_name_ACAD) == 0) &&
            PTRS__Text_Style[j]->type & type) return j;
	}
	return -1;
}


int ReadStyles40(int f, int version)
{
	FontStyle40 fontstyle[MaxNumberTextStyle];
	int i, j;
	int nfonts;
	int new_font;
	char fontstyle_font_name[36];
	char Style_font_name[36];

	if (read(f, &nfonts, sizeof(int)) != sizeof(int)) return FALSE;
	if (nfonts > MaxNumberTextStyle || nfonts < 1) return FALSE;
	if (read(f, &fontstyle, nfonts * sizeof(FontStyle40)) != (nfonts * sizeof(FontStyle40))) return FALSE;

	//zeroing//
	for (i = 0; i < MaxNumberTextStyle; i++)
	{
		PTRS__Font_Table.new_no[i] = 0;
		PTRS__Font_Table.type[i] = 255;  //unknown
		PTRS__Font_Table.zeroing[i] = FALSE;
	}

	for (i = 0; i < nfonts; i++)
	{
		strncpy(fontstyle_font_name, fontstyle[i].font_name, 33);
		fontstyle_font_name[33] = '\0';
		strupr(fontstyle_font_name);

		for (j = 0; j < i__font_nomax; j++)
		{
			strncpy(Style_font_name, PTRS__Text_Style[j]->font_name, 33);
			Style_font_name[33] = '\0';
			strupr(Style_font_name);

			if (strcmp(fontstyle_font_name, Style_font_name) == 0)
			{
				PTRS__Font_Table.new_no[i] = (unsigned char)j;
				if (fontstyle[i].font_name[0] < 91) PTRS__Font_Table.type[i] = 0; //capital letters
				else PTRS__Font_Table.type[i] = 1;
				break;
			}
		}
		if (j == i__font_nomax)
		{
			if (fontstyle[i].font_name[0] < 91)
			{
				new_font = Add_New_Font_CHR(fontstyle[i].font_name);
				if (new_font>-1) PTRS__Font_Table.new_no[i] = new_font;
				else
				{
					PTRS__Font_Table.new_no[i] = 0;  //default
					PTRS__Font_Table.type[i] = PTRS__Text_Style[0]->type;
					PTRS__Font_Table.zeroing[i] = TRUE;
					enforce_redraw = TRUE;
				}
			}
			else
			{
				new_font = Add_New_Font_AFF(fontstyle[i].font_name);
				if (new_font > -1) PTRS__Font_Table.new_no[i] = new_font;
				else
				{
					PTRS__Font_Table.new_no[i] = 0;  //default
					PTRS__Font_Table.type[i] = PTRS__Text_Style[0]->type;
					PTRS__Font_Table.zeroing[i] = TRUE;
					enforce_redraw = TRUE;
				}
			}
		}
	}
	 
	return TRUE;
}

int ReadStyles(int f, int version)
{
	FontStyle fontstyle[MaxNumberTextStyle];
	char fontbuffer[MaxNumberTextStyle*(FONTNAMELEN+1)];
	int i, j;
	int nfonts;
	char *ptr;
	int len;
	int new_font;
	char fontstyle_font_name[36];
	char Style_font_name[36];
	long position;

	if (read(f, &nfonts, sizeof(int)) != sizeof(int)) return FALSE;
	//check file position
	position = tell(f);
	if (nfonts > MaxNumberTextStyle || nfonts < 1) return FALSE;
	if (read(f, &fontbuffer, sizeof(fontbuffer)) ==0 ) return FALSE;

	ptr = fontbuffer;
	for (i = 0; i < nfonts; i++)
	{   //scanning fonts
		strcpy(&fontstyle[i].font_name, ptr);
		len = strlen(fontstyle[i].font_name) + 1;
		ptr += len;
		fontstyle[i].number = (unsigned char)*ptr;
		ptr++;
		fontstyle[i].type = (unsigned char)*ptr;
		ptr++;
	}
	position += (ptr - fontbuffer);
	lseek(f, position, SEEK_SET);
	//zeroing//
	for (i = 0; i < MaxNumberTextStyle; i++)
	{
		PTRS__Font_Table.new_no[i] = 0;
		PTRS__Font_Table.type[i] = 255;  //unknown
		PTRS__Font_Table.zeroing[i] = FALSE;
	}

	for (i = 0; i < nfonts; i++)
	{
		strncpy(fontstyle_font_name, fontstyle[i].font_name, 33);
		fontstyle_font_name[33] = '\0';
		strupr(fontstyle_font_name);

		for (j = 0; j < i__font_nomax; j++)
		{
			strncpy(Style_font_name, PTRS__Text_Style[j]->font_name, 33);
			Style_font_name[33] = '\0';
			strupr(Style_font_name);

			if (strcmp(fontstyle_font_name, Style_font_name) == 0)
			{
				PTRS__Font_Table.new_no[fontstyle[i].number] = j;
				PTRS__Font_Table.type[fontstyle[i].number] = fontstyle[i].type;
				break;
			}
		}
		if (j == i__font_nomax)
		{
			if (fontstyle[i].type==0)
			{
				new_font = Add_New_Font_CHR(fontstyle[i].font_name);
				if (new_font > -1)
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = new_font;
					PTRS__Font_Table.type[fontstyle[i].number] = 0;
				}
				else
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = 0;  //default
					PTRS__Font_Table.type[fontstyle[i].number] = PTRS__Text_Style[0]->type;
					PTRS__Font_Table.zeroing[fontstyle[i].number] = TRUE;
					enforce_redraw = TRUE;
				}
			}
			else if (fontstyle[i].type == 1)
			{
				new_font = Add_New_Font_AFF(fontstyle[i].font_name);
				if (new_font > -1)
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = new_font;
					PTRS__Font_Table.type[fontstyle[i].number] = 1;
				}
				else
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = 0;  //default
					PTRS__Font_Table.type[fontstyle[i].number] = PTRS__Text_Style[0]->type;
					PTRS__Font_Table.zeroing[fontstyle[i].number] = TRUE;
					enforce_redraw = TRUE;
				}
			}
			else
			{
				new_font = Add_New_Font_TTF_OTF(fontstyle[i].font_name, 0, 1);
				if (new_font > -1)
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = new_font;
					PTRS__Font_Table.type[fontstyle[i].number] = 2;
				}
				else
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = 0;  //default
					PTRS__Font_Table.type[fontstyle[i].number] = PTRS__Text_Style[0]->type;
					PTRS__Font_Table.zeroing[fontstyle[i].number] = TRUE;
					enforce_redraw = TRUE;
				}
			}
		}
	}

	return TRUE;
}

int ReadBlockStyles40(int f)
{
	FontStyle40 fontstyle[MaxNumberTextStyle];
	int i, j;
	int nfonts;
	int new_font;
	char fontstyle_font_name[34];
	char Style_font_name[34];

	if (read(f, &nfonts, sizeof(int)) != sizeof(int)) return FALSE;
	if (nfonts > MaxNumberTextStyle || nfonts < 1) return FALSE;
	if (read(f, &fontstyle, nfonts * sizeof(FontStyle40)) != (nfonts * sizeof(FontStyle40))) return FALSE;

	//zeroing//
	for (i = 0; i < MaxNumberTextStyle; i++)
	{
		PTRS__Font_Table.new_no[i] = 0;
		PTRS__Font_Table.type[i] = 255;  //unknown
		PTRS__Font_Table.zeroing[i] = FALSE;
	}

	for (i = 0; i < nfonts; i++)
	{
		strncpy(fontstyle_font_name, fontstyle[i].font_name, 33);
		fontstyle_font_name[33]='\0';
		strupr(fontstyle_font_name);

		for (j = 0; j < i__font_nomax; j++)
		{
			strncpy(Style_font_name, PTRS__Text_Style[j]->font_name, 33);
			Style_font_name[33] = '\0';
			strupr(Style_font_name);

			if (strcmp(fontstyle_font_name, Style_font_name) == 0)
			{
				PTRS__Font_Table.new_no[i] = j;
				if (PTRS__Text_Style[j]->font_name[0]<91) PTRS__Font_Table.type[i] = 0; //capital letters
				else PTRS__Font_Table.type[i] = 1;
				break;
			}
		}
		if (j == i__font_nomax)
		{
			if (fontstyle[i].font_name[0] < 91)
			{
				new_font = Add_New_Font_CHR(fontstyle[i].font_name);
				if (new_font > -1)
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = new_font;
					PTRS__Font_Table.type[fontstyle[i].number] = 0;
				}
				else
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = 0;  //default
					PTRS__Font_Table.type[fontstyle[i].number] = PTRS__Text_Style[0]->type;
					PTRS__Font_Table.zeroing[fontstyle[i].number] = TRUE;
					enforce_redraw = TRUE;
				}
			}
			else
			{
				new_font = Add_New_Font_AFF(fontstyle[i].font_name);
				if (new_font > -1)
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = new_font;
					PTRS__Font_Table.type[fontstyle[i].number] = 1;
				}
				else
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = 0;  //default
					PTRS__Font_Table.type[fontstyle[i].number] = PTRS__Text_Style[0]->type;
					PTRS__Font_Table.zeroing[fontstyle[i].number] = TRUE;
					enforce_redraw = TRUE;
				}
			}
		}
	}

	return TRUE;
}

int ReadBlockStyles(int f)
{
	FontStyle fontstyle[MaxNumberTextStyle];
	int i, j;
	int nfonts;
	char fontbuffer[MaxNumberTextStyle*(FONTNAMELEN+1)];
	char *ptr;
	char len;
	int new_font;
	char fontstyle_font_name[34];
	char Style_font_name[34];
	long position;

	if (read(f, &nfonts, sizeof(int)) != sizeof(int)) return FALSE;
	if (nfonts > MaxNumberTextStyle || nfonts < 1) return FALSE;
	//check file position
	position = tell(f);
	if (read(f, &fontbuffer, sizeof(fontbuffer)) == 0) return FALSE;

	ptr = fontbuffer;
	for (i = 0; i < nfonts; i++)
	{   //scanning fonts
		strcpy(&fontstyle[i].font_name, ptr);
		len = strlen(fontstyle[i].font_name) + 1;
		ptr += len;
		fontstyle[i].number = (unsigned char)*ptr;
		ptr++;
		fontstyle[i].type = (unsigned char)*ptr;
		ptr++;
	}
	position += (ptr - fontbuffer);
	lseek(f, position, SEEK_SET);
	//zeroing//
	for (i = 0; i < MaxNumberTextStyle; i++)
	{
		PTRS__Font_Table.new_no[i] = 0;
		PTRS__Font_Table.type[i] = 255;  //unknown
		PTRS__Font_Table.zeroing[i] = FALSE;
	}

	for (i = 0; i < nfonts; i++)
	{
		strncpy(fontstyle_font_name, fontstyle[i].font_name, 33);
		fontstyle_font_name[33] = '\0';
		////strupr(fontstyle_font_name);

		for (j = 0; j < i__font_nomax; j++)
		{
			strncpy(Style_font_name, PTRS__Text_Style[j]->font_name, 33);
			Style_font_name[33] = '\0';
			////strupr(Style_font_name);

			if (strcmp(fontstyle_font_name, Style_font_name) == 0)
			{
				PTRS__Font_Table.new_no[fontstyle[i].number] = j;
				PTRS__Font_Table.type[fontstyle[i].number] = fontstyle[i].type;
				break;
			}
		}
		if (j == i__font_nomax)
		{
			if (fontstyle[i].type == 0)
			{
				new_font = Add_New_Font_CHR(fontstyle[i].font_name);
				if (new_font > -1)
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = new_font;
					PTRS__Font_Table.type[fontstyle[i].number] = 0;
				}
				else
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = 0;  //default
					PTRS__Font_Table.type[fontstyle[i].number] = PTRS__Text_Style[0]->type;
					PTRS__Font_Table.zeroing[fontstyle[i].number] = TRUE;
					enforce_redraw = TRUE;
				}
			}
			else if (fontstyle[i].type == 1)
			{
				new_font = Add_New_Font_AFF(fontstyle[i].font_name);
				if (new_font > -1)
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = new_font;
					PTRS__Font_Table.type[fontstyle[i].number] = 1;
				}
				else
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = 0;  //default
					PTRS__Font_Table.type[fontstyle[i].number] = PTRS__Text_Style[0]->type;
					PTRS__Font_Table.zeroing[fontstyle[i].number] = TRUE;
					enforce_redraw = TRUE;
				}
			}
			else
			{
				new_font = Add_New_Font_TTF_OTF(fontstyle[i].font_name, 0, 1);
				if (new_font > -1)
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = new_font;
					PTRS__Font_Table.type[fontstyle[i].number] = 2;
				}
				else
				{
					PTRS__Font_Table.new_no[fontstyle[i].number] = 0;  //default
					PTRS__Font_Table.type[fontstyle[i].number] = PTRS__Text_Style[0]->type;
					PTRS__Font_Table.zeroing[fontstyle[i].number] = TRUE;
					enforce_redraw = TRUE;
				}
			}
		}
	}

	return TRUE;
}


int SaveStyles(int f)
{
	FontStyle fontstyle[MaxNumberTextStyle];
	int i;
	char fontbuffer[MaxNumberTextStyle*(FONTNAMELEN+2)];
	int fontbufferlen=0;
	char *ptr;

	ptr = fontbuffer;

	for (i = 0; i < i__font_nomax; i++)
	{
		if ((PTRS__Text_Style[i]) != NULL)
		{
			strncpy(fontstyle[i].font_name, PTRS__Text_Style[i]->font_name, FONTNAMELEN);
			fontstyle[i].font_name[FONTNAMELEN - 1] = '\0';
			fontstyle[i].number = i;
			fontstyle[i].type = PTRS__Text_Style[i]->type;
		}
		else //unlikely
		{
			strcpy(fontstyle[i].font_name, "");
			fontstyle[i].number = 255;
			fontstyle[i].type = 0;
		}
		memmove(ptr, fontstyle[i].font_name, strlen(fontstyle[i].font_name)+1);
		ptr += (strlen(fontstyle[i].font_name) + 1);
		*ptr = fontstyle[i].number;
		ptr++;
		*ptr = fontstyle[i].type;
		ptr++;
		fontbufferlen += (strlen(fontstyle[i].font_name) + 3);
	}
	if (write(f, &i__font_nomax, sizeof(i__font_nomax)) != sizeof(i__font_nomax)) return FALSE;
	if (write(f, fontbuffer, fontbufferlen) != (fontbufferlen)) return FALSE;
	
	return TRUE;
}

int SaveBlockStyles(int f, unsigned char charfont_numbers[])
{
	FontStyle fontstyle[MaxNumberTextStyle];
	int i;
	int font_nomax = 0;
	char fontbuffer[MaxNumberTextStyle*(FONTNAMELEN+2)];
	int fontbufferlen = 0;
	char *ptr;

    int fn_pos;

	ptr = fontbuffer;

	for (i = 0; i < MaxNumberTextStyle; i++)
	{
		if (charfont_numbers[i] == 1)
		{
			if ((PTRS__Text_Style[i]) != NULL)
			{
				strncpy(fontstyle[font_nomax].font_name, PTRS__Text_Style[i]->font_name, FONTNAMELEN);
				fontstyle[font_nomax].font_name[FONTNAMELEN-1] = '\0';
				fontstyle[font_nomax].number = i;
				fontstyle[font_nomax].type = PTRS__Text_Style[i]->type;
				memmove(ptr, fontstyle[font_nomax].font_name, strlen(fontstyle[font_nomax].font_name)+1);
				ptr += (strlen(fontstyle[font_nomax].font_name) + 1);
				*ptr = fontstyle[font_nomax].number;
				ptr++;
				*ptr = fontstyle[font_nomax].type;
				ptr++;
				fontbufferlen += (strlen(fontstyle[font_nomax].font_name) + 3);

				font_nomax++;
			}
			
		}
	}
    fn_pos=lseek(f, 0, SEEK_CUR);
#ifdef LINUX
    //fn_pos=lseek(f, 12, SEEK_CUR);
#endif
	if (write(f, &font_nomax, sizeof(font_nomax)) != sizeof(font_nomax)) return FALSE;
	if (font_nomax > 0)
	    if (write(f, fontbuffer, fontbufferlen) != (fontbufferlen)) return FALSE;
	return TRUE;
}

int ReadTextStyle (void)
/*---------------------*/
{
  int i, i_font ;
  char *ptrsz_fn ;
  char st1[30];
  long l_alloc ;

  WhNumberTextStyle  = 0 ;
  for (i = 0 ; i < MaxNumberTextStyle; i++)
  {
    PTRS__Text_Style [i] = NULL ;
    s__Fonts_Style [i].df_width_factor = 1.0 ;
  }
  Get_Private_Profile_Strings ((T_Fstring)CZCIONKI, get_fonts_param) ;

  //adding extra position
 
  i_font = TextG.czcionka ;
  if (NULL == PTRS__Text_Style [i_font]) i_font = 0;

  if (NULL == PTRS__Text_Style[i_font])
  {
    ptrsz_fn = PTRS__Text_Style [i_font]->font_name ;

    menu_par_new ((*mTekstm.pola)[10].txt, &ptrsz_fn) ;
    
    mCzcionka.poz = i_font;
    TextG.czcionka = i_font;
  }
  mCzcionka.max = i__font_nomax ;

  pmCzcionka[i__font_nomax].txt = add_new_font;
  pmCzcionka[i__font_nomax].menu=&mTTF_OTF;

  mCzcionka.max++;

  mCzcionkaW.max = mCzcionka.max;
  mCzcionkaZ.max = mCzcionka.max;


	  mCzcionka.maxw = mCzcionka.maxw0 = 0;
	  mCzcionkaW.maxw = mCzcionkaW.maxw0 = 0;
	  mCzcionkaZ.maxw = mCzcionkaZ.maxw0 = 0;


  if (i__font_nomax == 0)
  {
    mCzcionka.max = 1 ; 	/*z powodu obslugi menu*/
  }

  mCzcionka.xdl = mCzcionkaW.xdl = mCzcionkaZ.xdl =i__font_namelenmax;
  
  
  WhNumberTextStyle = i__font_nomax;
  
  /*zdublowanie czcionek - utworzenie czcionek BOLD*/
  
  WhNumberTextStyle0 = i__font_nomax;
  
  /*++++++++++++++++++++++++++++*/
   
  atexit (free_EMB_font_mem) ;

  return WhNumberTextStyle;
}

int ReadTextStyleDXF (void)
/*---------------------*/
{
  int i, i_font ;
  char *ptrsz_fn ;

  WhNumberTextStyleDXF  = 0 ;

  Get_Private_Profile_Strings ((T_Fstring)CZCIONKI_DXF, get_fonts_param_DXF) ;
  
  WhNumberTextStyleDXF = i__font_nomax_DXF ;
  
  return WhNumberTextStyleDXF;
}

BOOL Free_Buf_Font_Mem (BOOL b_all)
/*--------------------------------*/
{
  int i ;
  BOOL b_ret ;

  return TRUE;

  b_ret = FALSE ;
  for (i = 0 ; i < MaxNumberTextStyle ; i++)
  {
    if (NULL != PTRS__Text_Style [i] &&
	PTRS__Text_Style [i]->place_xms == 1 &&
	PTRS__Text_Style [i]->place_buf == 1)
    {
      DisposeBuffer ((char *)PTRS__Text_Style [i]->ptrs_off) ;
      DisposeBuffer ((char *)PTRS__Text_Style [i]->ptrsz_font) ;
      PTRS__Text_Style [i]->place_buf = 0 ;
      b_ret = TRUE ;
      if (b_all == FALSE)
      {
	break ;
      }
    }
  }
  return b_ret ;
}

#undef __O_LTTYPE__