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

#define __O_HATCH__
#include <forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include <string.h>
#include <math.h>
#ifndef LINUX
#include <io.h>
#endif
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_hatch.h"
#include "o_libfun.h"
#include "o_lfile.h"
#include "b_menu.h"

#include "menu.h"

#include "leak_detector_c.h"

extern BOOL Get_Private_Profile_Strings_Hatch(T_Fstring lpApplicationName, BOOL(*f)(T_Fstring, T_Fstring));
extern int Import_View_Hatch_Pattern (char *hatch_file, double *ptrdf_x, double *ptrdf_y, int comput_area);
double current_hatch_scale=1.0, new_hatch_scale, current_hatch_angle=0.0, new_hatch_angle;
double current_hatch_distans=1.0, new_hatch_distans;
extern void alfpro2acad (char *);
extern int hatch_proc (long_long, long_long, int, double[], double[], T_PTR_Hatch_Param, int comput_area,
			double df_apx1, double df_apy1, double df_apx2, double df_apy2, void *adb, int to_block, int draw);
extern int hatch_proc_test (long_long, long_long, double, double, T_PTR_Hatch_Param, int comput_area,
			double df_apx1, double df_apy1, double df_apx2, double df_apy2 );
extern void windows2mazovia(char *tekst);
extern char  *find_first_hatch_block(char *ad1, double *x0b, double *y0b);
extern void zmien_atrybut_preserve_hatch_outline(char  *adr, char  *adrk, int aold, int anew);
extern int zmien_atrybut_hatch_outline(char  *adr, char  *adrk, int aold, int anew, int blok);
extern void *find_hatch_solid_pattern(char* adr, char* adrk);
extern void utf82none(char * tekst);
extern void normalize_file_name(char* tekst);
extern BOOL hatch_cursor;
extern int noooph(void);
extern int hatch_pattern_rotate_t45(void);
extern int hatch_pattern_rotate_t_45(void);
extern void rotate_cursor_pattern_bitmap(int i_angle);
extern int fixangles[4];
extern BOOL global_point_in_solid;
extern BOOL global_any_choice;
extern int FileNamePattern(char **pattern_files, int n_list);
extern int Wez_HatchSolid_Pattern(void);
extern int SetPattern(char* pattern_path_name, char* pattern_file_name, char* ext);
extern void	layer_info (void);
extern void view_scale(void);

extern double get_skala(void);
extern void Copy_cursor_pattern_bitmap(SOLID_PATTERN_LIBRARY_ITEM* solid_pattern_library_item);
extern int Open_SolidHatchPatterns(void);
extern int utf8len(const char* s);
extern T_PTR_Hatch_Def_Param Get_Hatch_Def_Table999_ (void);

int i__file_handle ;
static int comput_area1 ;
static double df_apx1_, df_apy1_, df_apx2_, df_apy2_;

#define HATCH_HEAD_NAME  "ALF HATCH 1.0"

float solidhatchpatternscales[7] = {0.125,0.25,0.5,1.0,2.0,4.0,8.0};
int solidhatchpatternangles[4] = { 0, 90, 180, 270 };
char solidhatchpatterndxy[10][5] = { "0", "1/32", "1/16","1/8","1/4","3/8","1/2","5/8","3/4","7/8" };

extern double copy_cursor_pattern_fx;
void* obiekt_LlS_wybrany(void);


#define MaxMenuWin 32 //10

static BOOL                     wos_area = FALSE ;
static T_Hatch_Param 		    s_hatch_param = {1, 0, 1, 0, 0, 0} ;
static T_Hatch_Param            s_hatch_param2 = { 1, 0, 1, 0, 0, 0 };
static T_Hatch_Param            s_hatch_param1 = {1, 0, 1, 0, 0, 0} ;
static T_Hatch_Param            s_hatch_param0 = {1, 0, 1, 0, 0, 0} ;
int i_a_hatch_pattern_no = 1 ;
int i_a_hatch_pattern_no_bak = 1 ;
static T_Hatch_Param 		    s_hatch_param_bak = {1, 0, 1, 0, 0, 0};
static int                      i_a_hatch_pattern_no1 = 1 ;
static T_Hatch_Def_Param  	    s__def_param = {u8"", 0, 1} ;
static T_Hatch_Def_Param        s__def_param1 = {u8"", 0, 1} ;
static T_Hatch_Def_Param        s__def_param0 = {u8"", 0, 1} ;
static T_PTR_Hatch_Line_Def 	ptrs__hatch_line_def ;
static double 			df__dist = 1.0 ;
static short int SolidHatchPattern_bak=0;
static short int SolidHatch_bak=0;
static T_Hatch_Def_Param        s__def_param_bak= {u8"", 0, 1} ;

TMENU mHatch ={9,0,0,30,56,4, TADD | ICONS,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,(POLE(*)[]) &pmHatch,NULL,NULL};
static BOOL get_point (double *, double *) ;
void redcrP (char) ;
BOOL f (int i_type);
char s_hatch_param_df_name_bak [MAXPATH];

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

#define HATCH_HEAD_NAME_LEN 20
#define MaxNumberSegPattern 24 //6

typedef struct
{
  char sz_name [HATCH_HEAD_NAME_LEN] ;
  long l_off_patterns_name ;
  int i_pattern_namesno ;
} t_file_hatch_pattern_head ;

static t_file_hatch_pattern_head s__head = {HATCH_HEAD_NAME, 0, 0} ;
static T_PTR_Hatch_Def_Param ptrs__hatch_def_param = NULL ;
int insulation_hatch_pattern[2] = {-1, -1};
double insulation_hatch_scale[2] = {1, 1};
double insulation_hatch_scale_div[2] = {1.0, 100.0};
char insulation_hatch_name[HATCH_HEAD_NAME_LEN+4][2] = {"", ""};
int insulation_hatch_no=0;
BOOL insulation_hatch = FALSE;
static int i__pattern_names_maxno ;	/*max. liczba wzorcow*/
static int i__lines_defno ;	/*liczba linii definicji wzorcow*/
static BOOL get_mem_hatch_def_param (void)
/*---------------------------------------*/
{
  unsigned long_long l_free ;

  l_free = get_free_mem_size (0) ;
  i__pattern_names_maxno = (int)((l_free - 1000) / sizeof(T_Hatch_Def_Param)) ;
  if (i__pattern_names_maxno < 1)
  {
    return FALSE ;
  }
  if (i__pattern_names_maxno > PATTERN_NAMES_MAXNO)
	  i__pattern_names_maxno = PATTERN_NAMES_MAXNO;
    ptrs__hatch_def_param = (T_PTR_Hatch_Def_Param) malloc (i__pattern_names_maxno * sizeof(T_Hatch_Def_Param));
  return TRUE ;
}

#ifdef LINUX
off_t tell(int fd)
{
    return lseek(fd, 0, SEEK_CUR);
}
#endif

void set_solidhatch_pattern_name()
{
    menu_par_new((*mSolidHatchPattern.pola)[0].txt, SolidHatchPatternName);
}

void free_mem_hatch_def_param(void)
{
    if (ptrs__hatch_def_param != NULL)
    {
        free(ptrs__hatch_def_param);
        ptrs__hatch_def_param = NULL;
    }
}

void delete_hatch_file (void)
/*---------------------------------*/
{
  unlink (HATCH_TEMP_FILE) ;
}

static BOOL get_hatch_param (T_Fstring key_name, T_Fstring ret_string)
/*-----------------------------------------------------------------*/
{ int 		i;
  T_Fstring	p;
  int 		i_namesno ;
  static double df_scale ;
  T_Hatch_Line_Def s_hatch_line_def ;
  //char key_name_utf8[65];
  int a;

   if (ret_string != NULL)
   {
     i_namesno = s__head.i_pattern_namesno ;
     if (i__pattern_names_maxno <= i_namesno)
     {
       return FALSE ;
     }

       if (strcmp(key_name,"TEST")==0)
           a=0;

     strncpy (ptrs__hatch_def_param [i_namesno].sz_name, key_name, PATERN_NAME_MAXLEN) ;
     ptrs__hatch_def_param [i_namesno].sz_name [PATERN_NAME_MAXLEN-1] = '\0' ;
     ptrs__hatch_def_param [i_namesno].i_number_line = i__lines_defno ;
     ptrs__hatch_def_param [i_namesno].i_size = 0 ;
     if ( sscanf (ret_string, "%lf", &df_scale) < 1  || df_scale <= 0.0001)
     {
       df_scale = 1.0 ;
     }

       if ((strcmp(ptrs__hatch_def_param[i_namesno].sz_name, "BINSULATION") == 0) ||
           (strcmp(ptrs__hatch_def_param[i_namesno].sz_name, u8"БІЗОЛЯЦІЯ") == 0))
       {
           strcpy(insulation_hatch_name[0], ptrs__hatch_def_param[i_namesno].sz_name);
           strcat(insulation_hatch_name[0], ".axx");

           insulation_hatch_pattern[0] = i_namesno+3;
           insulation_hatch_scale[0] = df_scale;

           insulation_hatch_scale_div[0]=1.0;
       }

       if ((strcmp(ptrs__hatch_def_param[i_namesno].sz_name, "H1INSUN1") == 0) ||
           (strcmp(ptrs__hatch_def_param[i_namesno].sz_name, u8"ІЗОЛЯЦІЯ_X") == 0))
       {
           strcpy(insulation_hatch_name[1], ptrs__hatch_def_param[i_namesno].sz_name);
           strcat(insulation_hatch_name[1], ".axx");

           insulation_hatch_pattern[1] = i_namesno+3;
           insulation_hatch_scale[1] = df_scale;

           insulation_hatch_scale_div[1]=100.0;

       }

     s__head.i_pattern_namesno++ ;
   }
   else
   {
     if (0 > (i_namesno = s__head.i_pattern_namesno - 1))
     {
       return TRUE ;
     }
     if (-1 == ptrs__hatch_def_param [i_namesno].i_number_line)
     {
       return TRUE ;
     }
     if (strlen (key_name) == 0)
     {
       return TRUE ;
     }
     p = key_name ;
     p = strtok (p, ",");
     if (p == NULL)
     {
       goto error ;
     }
     if ( sscanf (p , "%lf", &s_hatch_line_def.df_angle) < 1)
     {
       goto error ;
     }
     s_hatch_line_def.df_angle = Angle_Normal (Grid_to_Rad (s_hatch_line_def.df_angle)) ;
     p = strtok (NULL, ",") ;
     if ( sscanf (p , "%lf", &s_hatch_line_def.df_startx) < 1)
     {
       goto error ;
     }
     s_hatch_line_def.df_startx *= df_scale ;
     p = strtok (NULL, ",") ;
     if ( sscanf (p , "%lf", &s_hatch_line_def.df_starty) < 1)
     {
       goto error ;
     }
     s_hatch_line_def.df_starty *= df_scale ;
     p = strtok (NULL, ",") ;
     if ( sscanf (p , "%lf", &s_hatch_line_def.df_move) < 1)
     {
       goto error ;
     }
     s_hatch_line_def.df_move *= df_scale ;
     p = strtok (NULL, ",") ;
     if ( sscanf (p , "%lf", &s_hatch_line_def.df_dist) < 1  ||
	 TRUE == Check_if_Equal (s_hatch_line_def.df_dist * df_scale, 0))
     {
       goto error ;
     }
     s_hatch_line_def.df_dist *= df_scale ;
     if (s_hatch_line_def.df_dist < 0)
     {
       s_hatch_line_def.df_dist = - s_hatch_line_def.df_dist ;
       s_hatch_line_def.df_move = - s_hatch_line_def.df_move ;
     }
     p = strtok (NULL, ",") ;
     s_hatch_line_def.df_len_pattern = 0 ;
     if (p == NULL)
     {
      i = 0 ;
     }
     else
     {
      for ( i = 0; i < MaxNumberSegPattern ; i++)
      {
	 if ( sscanf ( p , "%lf", &(s_hatch_line_def.df_def_line [i])) < 1 ) break;
	 s_hatch_line_def.df_def_line [i] *= df_scale ;
	 s_hatch_line_def.df_len_pattern += fabs (s_hatch_line_def.df_def_line [i]) ;
	 p = strtok (NULL, ",");
	 if (p == NULL)
	 {
	     i++;
	     break;
	 }
      }
     }
     s_hatch_line_def.i_def_size = i ;
     if (write (i__file_handle, &s_hatch_line_def, sizeof(s_hatch_line_def)) !=
	sizeof(s_hatch_line_def)) goto error ;
     ptrs__hatch_def_param [i_namesno].i_size += 1 ;
     i__lines_defno++ ;
   }
   return TRUE ;
error :
   ptrs__hatch_def_param [i_namesno].i_number_line = -1 ;
   s__head.i_pattern_namesno-- ;
   return TRUE ;
}


BOOL Read_Hatch_Pattern (void)
/*---------------------------*/
{
  if (10000 > Get_Disk_Free_Space (""))
  {
    return FALSE ;
  }
  s__head.i_pattern_namesno = 0 ;
  i__lines_defno = 0 ;
  if (FALSE == get_mem_hatch_def_param ())
  {
    return FALSE ;
  }
#ifndef LINUX
    i__file_handle=open(HATCH_TEMP_FILE, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
    i__file_handle=open(HATCH_TEMP_FILE, O_CREAT|O_TRUNC|O_RDWR, 0666); //S_IRWXU | S_IRWXG | S_IRWXO);
#endif
  if (i__file_handle == -1)
  {
    if (ptrs__hatch_def_param!=NULL) {
        free(ptrs__hatch_def_param);
        ptrs__hatch_def_param=NULL;
    }
    return FALSE ;
  }
  if (write (i__file_handle, &s__head, sizeof(s__head)) != sizeof(s__head)) goto error ;
  if (FALSE == Get_Private_Profile_Strings_Hatch ((T_Fstring)HATCH_PATTERN, get_hatch_param)) goto error ;
  if (-1 == (s__head.l_off_patterns_name = tell (i__file_handle))) goto error ;
  if (write (i__file_handle, ptrs__hatch_def_param,
      s__head.i_pattern_namesno * sizeof (T_Hatch_Def_Param)) !=
      s__head.i_pattern_namesno * sizeof (T_Hatch_Def_Param)) goto error ;
  if (0 != lseek (i__file_handle, 0L, SEEK_SET)) goto error ;
  if (write (i__file_handle, &s__head, sizeof(s__head)) != sizeof(s__head)) goto error ;
  close (i__file_handle) ;
  atexit (delete_hatch_file) ;
    if (ptrs__hatch_def_param!=NULL) {
        free(ptrs__hatch_def_param);
        ptrs__hatch_def_param=NULL;
    }
  return TRUE ;
error :
  close (i__file_handle) ;
  delete_hatch_file () ;
  unlink (HATCH_TEMP_FILE) ;
    if (ptrs__hatch_def_param!=NULL) {
        free(ptrs__hatch_def_param);
        ptrs__hatch_def_param=NULL;
    }
  return FALSE ;
}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

static BOOL check_mem_list (int i_pattern_namesno)
/*------------------------------------*/
{
  unsigned long_long l_free, l_size=0 ;  //16.08.2007
  BOOL b_ret ;

  b_ret = FALSE ;
  l_free = get_free_mem_size (0) ;
  l_size += i_pattern_namesno * sizeof(T_Hatch_Def_Param) ;
  m__list_hatch.maxw = 0; //MaxMenuWin ;
  m__list_hatch.maxw0 = 0; //MaxMenuWin;
  m__list_hatch.max = i_pattern_namesno + 3 ;
  l_size += Get_Menu_Image_Size (&m__list_hatch) ;
  if (l_size < l_free - 500) /*500 - rezerwa na dalsze alokacje */
  {
    ptrs__hatch_def_param = (T_PTR_Hatch_Def_Param)malloc(i_pattern_namesno * sizeof(T_Hatch_Def_Param));
    b_ret = TRUE ;
  }
  return b_ret ;
}

static void set_list_string (int i_pattern_namesno)

/*-----------------------------------------------*/

{
  int max_len, len, i ;
  char *ptr_s ;

  max_len = 10 ; 	/*dla wzorcow predefiniowanych*/

  if (i_pattern_namesno > 0)
  {
    pm__list_hatch [0].txt = pm__list_hatch_predef [0].txt ;

    pm__list_hatch [0].wcod = pm__list_hatch_predef [0].wcod ;

    pm__list_hatch [0].menu = pm__list_hatch_predef [0].menu ;

     pm__list_hatch [1].txt = pm__list_hatch_predef [1].txt ;  //win32todo

    pm__list_hatch [1].wcod = pm__list_hatch_predef [1].wcod ;

     pm__list_hatch [1].menu = pm__list_hatch_predef [1].menu ;

	    pm__list_hatch [2].txt = pm__list_hatch_predef [2].txt ;  //win32todo

	pm__list_hatch[2].wcod = pm__list_hatch_predef[2].wcod;

	pm__list_hatch[2].menu = pm__list_hatch_predef[2].menu;

    for (i = 0 ; i < i_pattern_namesno ; i++)
    {

      ptr_s = ptrs__hatch_def_param [i].sz_name ;

      pm__list_hatch [i + 3].txt = ptr_s ;   //win32todo

      pm__list_hatch [i + 3].wcod = '\1' ;

      pm__list_hatch [i + 3].menu = NULL ;

      len = utf8len(ptr_s);

      if (len > max_len)

      {

	max_len = len ;

      }

    }

  }


  m__list_hatch.max = i_pattern_namesno + 3 ;

  m__list_hatch.xdl = PATERN_NAME_MAXLEN - 1 ;

  if (max_len + 3 < PATERN_NAME_MAXLEN - 1)

  {

    m__list_hatch.xdl = max_len + 3 ;

  }

  if (m__list_hatch.max <= m__list_hatch.maxw)

  {

    m__list_hatch.maxw = 0 ;
	m__list_hatch.maxw0 = 0;

  }

  m__list_hatch.pola = &pm__list_hatch_predef ;

  if (i_pattern_namesno > 0)

  {

    m__list_hatch.pola = (POLE ( *)[])(pm__list_hatch) ;

  }

}

void set_menu_hatch_pattern (void)
/*------------------------------*/
{
  t_file_hatch_pattern_head s_head ;

  int i_temp ;

  int i_pattern_namesno ;

  char st1[30];

   i_pattern_namesno = 0 ;


   free_mem_hatch_def_param();

  lseek(i__file_handle, 0L, SEEK_SET);

  if (i__file_handle != -1)
  {
    if (read (i__file_handle, &s_head, sizeof(s_head)) != sizeof(s_head))
    {
      close(i__file_handle);

      goto set_list ;
    }
    if (0 != strcmp (HATCH_HEAD_NAME, s_head.sz_name))
    {
      goto set_list ;
    }
    if (FALSE == check_mem_list (s_head.i_pattern_namesno))
    {
      goto set_list ;
    }
    if (s_head.l_off_patterns_name !=

	lseek (i__file_handle, s_head.l_off_patterns_name, SEEK_SET))
    {
      goto set_list ;
    }
    i_temp = s_head.i_pattern_namesno * sizeof(T_Hatch_Def_Param) ;

    if (read (i__file_handle, ptrs__hatch_def_param, i_temp) != i_temp)
    {
      goto set_list ;
    }
    i_pattern_namesno = s_head.i_pattern_namesno;
  }

set_list :

  set_list_string (i_pattern_namesno) ;

}

void set_hatch_pattern (int i_ev, int opcja, BOOL insulation_h)
/*---------------------------------------------------------*/
{
  char sz_buf [PATERN_NAME_MAXLEN] ;

  if (opcja==1)
  {
   strncpy (sz_buf, &(*m__list_hatch.pola)[i_ev].txt[0], PATERN_NAME_MAXLEN) ;

   sz_buf [PATERN_NAME_MAXLEN - 1] = '\0' ;

   menu_par_new ((*mHatch.pola)[0].txt, sz_buf) ;

   strcpy(s_hatch_param_df_name, sz_buf); 

  }

  i_a_hatch_pattern_no = i_ev ;
  s_hatch_param.i_number_hatch = i_ev;
  s_hatch_param.insulation_hatch = insulation_h;

  if (i_a_hatch_pattern_no > 2) {
      if (i_a_hatch_pattern_no==999)
      {
          memcpy(&s__def_param, Get_Hatch_Def_Table999_(), sizeof(s__def_param));
          s__def_param1 = s__def_param;
      }
      else
      {
          memcpy(&s__def_param, &ptrs__hatch_def_param[i_a_hatch_pattern_no - 3], sizeof(s__def_param));
          s__def_param1 = s__def_param;
      }
  }
}


void backup_hatch_pattern(void)
{
    i_a_hatch_pattern_no_bak=i_a_hatch_pattern_no;
    memmove(&s_hatch_param_bak, &s_hatch_param, sizeof(T_Hatch_Param));
    memmove(&s__def_param_bak, &s__def_param, sizeof(T_Hatch_Def_Param));
    SolidHatchPattern_bak = SolidHatchPattern;
    SolidHatch_bak=SolidHatch;
    strcpy(s_hatch_param_df_name_bak, s_hatch_param_df_name);
}

void restore_hatch_pattern(void)
{
    i_a_hatch_pattern_no=i_a_hatch_pattern_no_bak;
    memmove(&s_hatch_param, &s_hatch_param_bak, sizeof(T_Hatch_Param));
    memmove(&s__def_param, &s__def_param_bak, sizeof(T_Hatch_Def_Param));
    SolidHatchPattern = SolidHatchPattern_bak;
    SolidHatch=SolidHatch_bak;
    strcpy(s_hatch_param_df_name, s_hatch_param_df_name_bak);
}

void change_hatch_pattern(int hatch_type)
/*-------------------------------------*/
{
switch (hatch_type)
 {
  case 0:
  case 1: if (i_a_hatch_pattern_no != 2)
           {
            strcpy(s_hatch_param_df_name,"TERMAT24");
            s_hatch_param_df_name [PATERN_NAME_MAXLEN - 1] = '\0' ;
            m__list_hatch.poz = 2 ;
            m__list_hatch.off = 6 ;
            m__list_hatch.foff = 0 ;
            menu_par_new ((*mHatch.pola)[0].txt, s_hatch_param_df_name) ;
            set_hatch_pattern(2,0,FALSE);
            s__def_param.i_number_line=0;
            s__def_param.i_size=4;
           }
          break;
  case 2:
  case 4: if (i_a_hatch_pattern_no != 3)
           {
            strcpy(s_hatch_param_df_name,"TERMAT49");
            s_hatch_param_df_name [PATERN_NAME_MAXLEN - 1] = '\0' ;
            m__list_hatch.poz = 3 ;
            m__list_hatch.off = 6 ;
            m__list_hatch.foff = 0 ;
            menu_par_new ((*mHatch.pola)[0].txt, s_hatch_param_df_name) ;
            set_hatch_pattern(3,0,FALSE);
            s__def_param.i_number_line=4;
            s__def_param.i_size=4;
           }
          break;
  case 3: if (i_a_hatch_pattern_no != 4)
           {
            strcpy(s_hatch_param_df_name,"TERMAT37");
            s_hatch_param_df_name [PATERN_NAME_MAXLEN - 1] = '\0' ;
            m__list_hatch.poz = 4 ;
            m__list_hatch.off = 6 ;
            m__list_hatch.foff = 0 ;
            menu_par_new ((*mHatch.pola)[0].txt, s_hatch_param_df_name) ;
            set_hatch_pattern(4,0,FALSE);
            s__def_param.i_number_line=8;
            s__def_param.i_size=4;
           }
          break;
  case 5: if (i_a_hatch_pattern_no != 8)
           {
            strcpy(s_hatch_param_df_name,"MTHERM36");
            s_hatch_param_df_name [PATERN_NAME_MAXLEN - 1] = '\0' ;
            m__list_hatch.poz = 8 ;
            m__list_hatch.off = 6 ;
            m__list_hatch.foff = 0 ;
            menu_par_new ((*mHatch.pola)[0].txt, s_hatch_param_df_name) ;
            set_hatch_pattern(8,0,FALSE);
            s__def_param.i_number_line=24;
            s__def_param.i_size=4;
           }
          break;
  case 6: if (i_a_hatch_pattern_no != 9)
           {
            strcpy(s_hatch_param_df_name,"MTHERM24");
            s_hatch_param_df_name [PATERN_NAME_MAXLEN - 1] = '\0' ;
            m__list_hatch.poz = 9 ;
            m__list_hatch.off = 6 ;
            m__list_hatch.foff = 0 ;
            menu_par_new ((*mHatch.pola)[0].txt, s_hatch_param_df_name) ;
            set_hatch_pattern(9,0,FALSE);
            s__def_param.i_number_line=28;
            s__def_param.i_size=4;
           }
          break;
  case 7: if (i_a_hatch_pattern_no != 10)
           {
            strcpy(s_hatch_param_df_name,"MTHERM11");
            s_hatch_param_df_name [PATERN_NAME_MAXLEN - 1] = '\0' ;
            m__list_hatch.poz = 10 ;
            m__list_hatch.off = 6 ;
            m__list_hatch.foff = 0 ;
            menu_par_new ((*mHatch.pola)[0].txt, s_hatch_param_df_name) ;
            set_hatch_pattern(10,0, FALSE);
            s__def_param.i_number_line=32;
            s__def_param.i_size=4;
           }
          break;
 case -1 :  //linia caigla
          strcpy(s_hatch_param_df_name,"////////");
          s_hatch_param_df_name [PATERN_NAME_MAXLEN - 1] = '\0' ;
          m__list_hatch.poz = 0 ;
          m__list_hatch.off = 2 ;
          m__list_hatch.foff = 0 ;
          menu_par_new ((*mHatch.pola)[0].txt, s_hatch_param_df_name) ;
          set_hatch_pattern(0,0,FALSE);
          s__def_param.i_number_line=0;
          s__def_param.i_size=0;
          break;
 default: if (i_a_hatch_pattern_no != 2)
           {
            strcpy(s_hatch_param_df_name,"TERMAT24");
            s_hatch_param_df_name [PATERN_NAME_MAXLEN - 1] = '\0' ;
            m__list_hatch.poz = 2 ;
            m__list_hatch.off = 6 ;
            m__list_hatch.foff = 0 ;
            menu_par_new ((*mHatch.pola)[0].txt, s_hatch_param_df_name) ;
            set_hatch_pattern(2,0,FALSE);
            s__def_param.i_number_line=0;
            s__def_param.i_size=4;
           }
          break;
 }
}

static BOOL free_mem_hatch_pattern (int i_type)
/*---------------------------------*/
{
  BOOL b_ret ;
  int i_size ;
  long_long l_free, l_off ;

  b_ret = FALSE ;
  //ptrs__hatch_line_def = NULL ;
  if (pm__list_hatch != NULL)
  {
     if (ptrs__hatch_line_def!=NULL)
     {
         free(ptrs__hatch_line_def);
         ptrs__hatch_line_def=NULL;
     }
  }

  free_mem_hatch_def_param();

  if (i__file_handle == -1)
  {
    if (i_a_hatch_pattern_no <= 2)
      return TRUE ;
    else
      return FALSE ;
  }
  if (i_type == 1 || i_a_hatch_pattern_no <= 2)
  {
    close (i__file_handle) ;
    return TRUE ;
  }
  l_free = get_free_mem_size (1) ;
  i_size = s__def_param.i_size * sizeof (T_Hatch_Line_Def) ;
  if (l_free < 5000 + i_size)
  {
    close (i__file_handle) ;
    return b_ret ;
  }
  ptrs__hatch_line_def = (T_PTR_Hatch_Line_Def)malloc/*getmem*/ (i_size) ;  //MAREK 19-08-2024
  l_off = sizeof (t_file_hatch_pattern_head) +
		s__def_param.i_number_line * sizeof (T_Hatch_Line_Def) ;
  if (l_off !=
      lseek (i__file_handle, l_off, SEEK_SET))
  {
      if (ptrs__hatch_line_def!=NULL) {
          free(ptrs__hatch_line_def);
          ptrs__hatch_line_def=NULL;
      }
    close (i__file_handle) ;
    return b_ret ;
  }
  if (read (i__file_handle, ptrs__hatch_line_def, i_size) != i_size)
  {
      if (ptrs__hatch_line_def!=NULL) {
          free(ptrs__hatch_line_def);
          ptrs__hatch_line_def=NULL;
      }
    close (i__file_handle) ;
    return FALSE ;
  }
  close (i__file_handle) ;
  return TRUE ;
}

BOOL free_mem_hatch_pattern_ (int i_type)
{
    return free_mem_hatch_pattern(i_type);
}

int katHsel(double kat, double *angle)
{
    unsigned typ;
    void* adr;
    PLINIA PL;
    char sk[MaxTextLen];

    typ = Blinia;
    if ((adr = select_n(&typ, NULL, 75)) != NULL)
    {
        Cur_offd(X, Y);
        
        parametry_lini((LINIA*)adr, &PL);
        *angle = kat + PL.kat * Pi / 180;
        Cur_ond(X, Y);
        return 1;
    }
    return 0;
}


void hatch_angle_rotate_parallel(void)
/*----------------------------------*/
{
    char sk[MaxTextLen] = "";
    int retval_no = 1;
    double buf_ret[1];
    double d;
    int ret;


    sel.akt = ASel;
    sel.gor = NGSel;
    ret=katHsel(0, &d);
    sel.akt = NASel;
    sel.gor = NGSel;

    if (ret == 1)
    {
        DF_to_String(sk, "%-9.4f", Rad_to_Grid(d), 8);
        menu_par_new((*mHatch.pola)[2].txt, sk);
        s_hatch_param.df_angle = d;
        s_hatch_param_df_angle = d;
    }
}

void hatch_angle_rotate_perpendicular(void)
/*--------------------------------------*/
{
    char sk[MaxTextLen] = "";
    int retval_no = 1;
    double buf_ret[1];
    double d;
    int ret;

    sel.akt = ASel;
    sel.gor = NGSel;
    ret = katHsel(-Pi / 2, &d);
    sel.akt = NASel;
    sel.gor = NGSel;

    if (ret == 1)
    {
        DF_to_String(sk, "%-9.4f", Rad_to_Grid(d), 8);
        menu_par_new((*mHatch.pola)[2].txt, sk);
        s_hatch_param.df_angle = d;
        s_hatch_param_df_angle = d;
    }
}

void hatch_angle_rotate (void)
/*----------------------*/
{
  char sk [MaxTextLen] = "" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d ;

  DF_to_String (sk, "%-9.4f", Rad_to_Grid (s_hatch_param.df_angle), 0) ;
  if (!get_string (sk, "", MaxTextLen, 0, 32)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  DF_to_String (sk, "%-9.4f", d, 8) ;
  menu_par_new ((*mHatch.pola)[2].txt, sk) ;
  d = Angle_Normal (Grid_to_Rad (d)) ;
  s_hatch_param.df_angle = d ;
  s_hatch_param_df_angle = d ;
}


void Hatch_Rotate_t3(void)
/*----------------------*/
{ char sk [MaxTextLen] = "" ;
  double d ;
  d = s_hatch_param.df_angle + (Pi/60);
  d = Angle_Normal (d) ;
  s_hatch_param.df_angle = d ;
  s_hatch_param_df_angle = d ;
  DF_to_String (sk, "%-9.4f", Rad_to_Grid(d), 8) ;
  menu_par_new ((*mHatch.pola)[2].txt, sk) ;
  return;
}

void Hatch_Rotate_t_3(void)
/*----------------------*/
{ char sk [MaxTextLen] = "" ;
  double d ;
  d = s_hatch_param.df_angle - (Pi/60);
  d = Angle_Normal (d) ;
  s_hatch_param.df_angle = d ;
  s_hatch_param_df_angle = d ;
  DF_to_String (sk, "%-9.4f", Rad_to_Grid(d), 8) ;
  menu_par_new ((*mHatch.pola)[2].txt, sk) ;
  return;
}

void Hatch_Rotate_t45(void)
/*----------------------*/
{ char sk [MaxTextLen] = "" ;
  double d ;
  d = s_hatch_param.df_angle + (Pi/4);
  d = Angle_Normal (d) ;
  s_hatch_param.df_angle = d ;
  s_hatch_param_df_angle = d ;
  DF_to_String (sk, "%-9.4f", Rad_to_Grid(d), 8) ;
  menu_par_new ((*mHatch.pola)[2].txt, sk) ;
  return;
}

void Hatch_Rotate_t_45(void)
/*----------------------*/
{ char sk [MaxTextLen] = "" ;
  double d ;

  d = s_hatch_param.df_angle - (Pi/4);
  d = Angle_Normal (d) ;
  s_hatch_param.df_angle = d ;
  s_hatch_param_df_angle = d ;
  DF_to_String (sk, "%-9.4f", Rad_to_Grid(d), 8) ;
  menu_par_new ((*mHatch.pola)[2].txt, sk) ;
  return;
}

void hatch_scale (void)
/*----------------------*/
{
  char sk [MaxTextLen] = "" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d ;

  DF_to_String (sk, "%-9.4f", s_hatch_param.df_scale, 0) ;
  if (!get_string (sk, "", MaxTextLen, 0, 31)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  if ( d <= 0 )
  {
    ErrList (30);
    return ;
  }
  if (d > 1000)
  {
    d = 1000 ;
  }
  DF_to_String (sk, "%-9.4f", d, 8) ;
  menu_par_new ((*mHatch.pola)[3].txt, sk) ;
  s_hatch_param.df_scale = d ;
  s_hatch_param_df_scale = d ;
}

int hatch_image_pattern(void)
{
    int ret;
    ret = Open_SolidHatchPatterns();
    if (ret == 1)
    {
        menu_par_new((*mSolidHatchPattern.pola)[0].txt, SolidPatternName);
        komunikat0(139);
        return 0;
    }
    return -1;
}

void hatch_image_pattern_scale(int poz)
{
    char sk[16];
    SOLID_PATTERN_LIBRARY_ITEM solid_pattern_library_item;

    SolidHatchPatternScale = poz - ID_IMAGE_PATTERN_SCALE;

    if (strlen(SolidHatchPatternName) > 0)
    {
        strncpy(&solid_pattern_library_item.pattern_name, &SolidHatchPatternName, 31);
        solid_pattern_library_item.pattern_name[31] = '\0';
        CUR_OFF(X, Y);
        Copy_cursor_pattern_bitmap(&solid_pattern_library_item);
        copy_cursor_pattern_fx = get_skala() * DokladnoscF;
        CUR_ON(X, Y);
    }

    sprintf(sk, "%5.3f", solidhatchpatternscales[SolidHatchPatternScale]);

    menu_par_new((*mSolidHatchPattern.pola)[1].txt, sk);
}

void hatch_image_pattern_angle_menu(int angle)
{
    char sk[16];
   sprintf(sk, "%d", solidhatchpatternangles[angle]);
   menu_par_new((*mSolidHatchPattern.pola)[2].txt, sk);
}

void hatch_image_pattern_angle(int poz)
{
    char sk[16];

    SolidHatchPatternAngle = poz - ID_IMAGE_PATTERN_ANGLE;

    CUR_OFF(X, Y);
    rotate_cursor_pattern_bitmap(fixangles[SolidHatchPatternAngle]);
    CUR_ON(X, Y);

    hatch_image_pattern_angle_menu(SolidHatchPatternAngle);

}

void hatch_image_pattern_dx(int poz)
{
    char sk[16];

    SolidHatchPatternDx = poz - ID_IMAGE_PATTERN_DX;

    sprintf(sk, "%s", solidhatchpatterndxy[SolidHatchPatternDx]);

    menu_par_new((*mSolidHatchPattern.pola)[3].txt, sk);
}

void hatch_image_pattern_dy(int poz)
{
    char sk[16];

    SolidHatchPatternDy = poz - ID_IMAGE_PATTERN_DY;

    sprintf(sk, "%s", solidhatchpatterndxy[SolidHatchPatternDy]);

    menu_par_new((*mSolidHatchPattern.pola)[4].txt, sk);
}

void set_solidhatchtranslucency(int poz0, int opcja)
{
    char sk[10];
    int poz = poz0;

    float d_trans = 12.75;
    int transluc;

    SolidHatchPattern = 0;
    SolidHatch = 1;

    if (poz0 == 50) SolidHatchTranslucent = 0;
    else SolidHatchTranslucent = 1;

    SolidHatchTranslucency = 255 - (poz0 - 559) * d_trans;

    transluc = (int)((float)SolidHatchTranslucency / d_trans +0.5);
    sprintf(sk, "%d%%", transluc * 5);

    set_hatch_pattern(0, opcja, FALSE);

	if (ptrs__hatch_def_param!=NULL)
        menu_par_new(&(*m__list_hatch.pola)[0].txt[0], sk);
}


void hatch_dist (void)
/*----------------*/
{
  char sk [MaxTextLen] = "" ;
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d ;

  DF_to_String (sk, "%-8.3f", jednostki_d_to_plt_d (df__dist), 0) ;
  if (!get_string (sk, "", MaxTextLen, 0, 51)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  if ( d <= 0 )
  {
    ErrList (30);
    return ;
  }
  if (d > 1000)
  {
    d = 1000 ;
  }
  DF_to_String (sk, "%-8.3f", d, 8) ;
  strcat (sk, "mm") ;
  menu_par_new ((*mHatch.pola)[6].txt, sk) ;
  df__dist = plt_d_to_jednostki_d (d) ;
  s_hatch_param_df_distans = d ;
}

void hatch_snap_base (void)
/*--------------------*/
{
  char sk [MaxTextLen] = "" ;
  int retval_no = 2 ;
  double buf_ret [2] ;

  DF_to_String (sk, "%-6.3f", milimetryobx (s_hatch_param.df_basex), 0) ;
  strcat (sk, " ; ") ;
  DF_to_String (&sk [strlen (sk)], "%-6.3f", milimetryoby (s_hatch_param.df_basey), 0) ;
  if (!get_string (sk , "", MaxTextLen, 0, 52)) return ;
  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 2)
  {
    return ;
  }
  DF_to_String (sk, "%-6.3f", buf_ret [0], 6) ;
  strcat (sk, ";") ;
  DF_to_String (&sk [strlen (sk)], "%-6.3f", buf_ret [1], 6) ;
  menu_par_new ((*mHatch.pola) [4].txt, sk) ;
  s_hatch_param.df_basex = jednostkiOb (buf_ret [0]) + get_localx();
  s_hatch_param.df_basey = jednostkiOb (buf_ret [1]) + get_localy();
}

void hatch_snap_base_point (void)
/*------------------------*/
{
  char sk [MaxTextLen] = "" ;
  int retval_no = 2 ;
  double buf_ret [2] ;
  double df_xp, df_yp ;
  EVENT *ev ;

  komunikat (112) ;
  ev = Get_Event_Point (NULL, &df_xp, &df_yp) ;
    switch (ev->What)
    {
      case evKeyDown :
	if(ev->Number == 0)
	{
	  komunikat(0);
	  return;
	}
	if(ev->Number == ENTER)
	{
  	  s_hatch_param.df_basex = df_xp ;
          s_hatch_param.df_basey = df_yp ;

          DF_to_String (sk, "%-6.3f", milimetryobx (s_hatch_param.df_basex), 0) ;
          strcat (sk, " ; ") ;
          DF_to_String (&sk [strlen (sk)], "%-6.3f", milimetryoby (s_hatch_param.df_basey), 0) ;
          menu_par_new ((*mHatch.pola) [4].txt, sk) ;
	}
	break;
     }
  komunikat (0);
}

BOOL b__temp ;

void get_base_point(double *x_origin, double *y_origin)
/*---------------------------------------------------*/
{
  *x_origin=s_hatch_param.df_basex;
  *y_origin=s_hatch_param.df_basey;
}

void put_base_point(double *x_origin, double *y_origin)
/*---------------------------------------------------*/
{
  s_hatch_param.df_basex=*x_origin;
  s_hatch_param.df_basey=*y_origin;
}

void put_hatch_scale(double *hatch_scale)
/*-------------------------------------*/
{
  s_hatch_param.df_scale=*hatch_scale;
}

void put_hatch_distance(double hatch_distance)
/*------------------------------------------*/
{
  df__dist = plt_d_to_jednostki_d (hatch_distance) ;
  s_hatch_param_df_distans = hatch_distance ;
}

void put_hatch_angle(double *hatch_angle)
/*------------------------------------*/
{
  s_hatch_param.df_angle=*hatch_angle;
}

void redcrP (char typ)
/*--------------*/
{
  static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int(*SW[3])(), akt, cur, gor;
  static char st[POLE_TXT_MAX];
  unsigned short cod = L' ';
  static TMENU *menu;
  static int iconno=0;
  char sk [MaxTextLen] = "" ;
  int  	i_namesno ;
  float d_trans = 12.75;
  int transluc;

  if (typ == 0)
  {
     if ( comput_area1 == 0 )
     {
         if (SolidHatchPattern == 1)
         {
             komunikat0(139);
             SERV[73] = noooph;  //PgUp  kat +3
             SERV[81] = noooph;  //PgDn   kat -3
             SERV[71] = hatch_pattern_rotate_t45;  //Home  kat +45
             SERV[79] = hatch_pattern_rotate_t_45;  //End   kat -45
         }
         else if (i_a_hatch_pattern_no == 0)
         {
             komunikat0(140);
             SERV[73] = noooph;  //PgUp  kat +3
             SERV[81] = noooph;  //PgDn   kat -3
             SERV[71] = noooph;  //Home  kat +45
             SERV[79] = noooph;  //End   kat -45
         }
         else komunikat0 (88) ;
     if ( wos_area )
      {
      i_a_hatch_pattern_no = i_a_hatch_pattern_no1 ;
      s_hatch_param = s_hatch_param1 ;
      s__def_param = s__def_param1 ;
      i_namesno = s__head.i_pattern_namesno ;
      set_menu_hatch_pattern ();
      set_hatch_pattern (i_a_hatch_pattern_no, 1,FALSE) ;
      wos_area = FALSE ;
      }
     }
	 else if (comput_area1 == 10)
	 {
		 komunikat0(138);
		 if (wos_area)
		 {
			 i_a_hatch_pattern_no = i_a_hatch_pattern_no1;
			 s_hatch_param = s_hatch_param1;
			 s__def_param = s__def_param1;
			 i_namesno = s__head.i_pattern_namesno;
			 set_menu_hatch_pattern();
			 set_hatch_pattern(i_a_hatch_pattern_no, 1,FALSE);
			 wos_area = FALSE;
		 }
	 }
     else
     {
     komunikat0 (98);
     if ( wos_area == FALSE )
      {
      i_a_hatch_pattern_no1 = i_a_hatch_pattern_no ;
      s_hatch_param1 = s_hatch_param ;
      s__def_param1 = s__def_param ;
      s__def_param = s__def_param0 ;
      s_hatch_param = s_hatch_param0 ;
      i_a_hatch_pattern_no = 1 ;  //0  GEOMETRY
      set_hatch_pattern ( i_a_hatch_pattern_no, 0,FALSE ) ;
      wos_area = TRUE ;
      }
     }
	 if (comput_area1 == 10)  //supposed to be 10
	 {
		 akt = sel.akt; sel.akt = 0;
		 cur = sel.cur;	sel.cur = 1;
		 CUR_oFF = CUR_OFF; CUR_OFF = out_sel_off;
		 CUR_oN = CUR_ON; CUR_ON = out_sel_on;
	 }
	 else
	 {
		 akt = sel.akt; sel.akt = ASel;
		 cur = sel.cur; sel.cur = 0;
	 }
		 SW[0] = SERV[73];  SERV[73] = sel_t;
		 SW[1] = SERV[81];  SERV[81] = sel_d;
	 
     getmenupini (&menu,st,&cod, &iconno) ;
     
     set_menu_hatch_pattern ();
     DF_to_String (sk, "%-8.3f", jednostki_d_to_plt_d (df__dist), 8) ;
     strcat (sk, "mm") ;
     menu_par_new ((*mHatch.pola)[6].txt, sk) ;
     DF_to_String (sk, "%-6.3f", milimetryob (s_hatch_param.df_basex), 6) ;
     strcat (sk, ";") ;
     DF_to_String (&sk [strlen (sk)], "%-6.3f", milimetryob (s_hatch_param.df_basey), 6) ;
     menu_par_new ((*mHatch.pola) [4].txt, sk) ;

     transluc = (int)((float)SolidHatchTranslucency / d_trans +0.5);
     sprintf(sk, "%d%%", transluc * 5);

  	if (ptrs__hatch_def_param!=NULL)
        menu_par_new(&(*m__list_hatch.pola)[0].txt[0], sk);

     if ((comput_area1 == 0) || (comput_area1 == 10))
     {
         menupini(&mHatch, _HATCHING_, _HATCHING_C_, 6);
     }

   }
  else if (typ == 1)
  {
     SERV[73] = SW[0] ;
     SERV[81] = SW[1] ;
     menupini (menu, st, cod, iconno) ;
     sel.akt = akt ;
     komunikat0 (0) ;
   }
  else
  {
     SERV[73] = SW[0] ;
     SERV[81] = SW[1] ;
     menupini (menu, st, cod, iconno) ;
     sel.akt = akt ;
	 sel.cur = cur;
     b__temp = free_mem_hatch_pattern (typ) ;
     komunikat0 (0) ;
   }
}

static BOOL get_point (double *ptrdf_x, double *ptrdf_y)
/*-----------------------------------------------------*/
{
  EVENT *ev ;
  redcrP (0) ;
  while (1)
  {
    ev = Get_Event_Point (NULL, ptrdf_x, ptrdf_y) ;
    switch (ev->What)
    {
      case evKeyDown :
	if(ev->Number == 0)
	{
	  redcrP (1) ;
	  return FALSE ;
	}
	if(ev->Number == ENTER)
	{
	  redcrP (2) ;
	  return b__temp ;
	}
	break;
      case evCommandP :
	switch (ev->Number) 
	{
	  case ID_PATTERN_FOLDER:
		//TEMPORARY
		break;
	  case ID_VPATTERN_ANGLE :
	    hatch_angle_rotate () ;
	    break ;
      case ID_VPATTERN_PARALLEL:
          hatch_angle_rotate_parallel();
          break;
      case ID_VPATTERN_PERPENDICULAR:
          hatch_angle_rotate_perpendicular();
          break;
	  case ID_SCALE :
	    hatch_scale () ;
	    break ;
	  case ID_SNAP_BASE :
	    hatch_snap_base () ;
	    break ;
	  case ID_SNAP_BASE_POINT :
	    hatch_snap_base_point () ;
	    break ;
	  case ID_PATTERN :	/*menu*/
	    break ;
	  case ID_DIST :
	    hatch_dist () ;
	    break ;
      case ID_INSULATION:
          if (insulation_hatch_pattern[0] > 0)
          {
              set_hatch_pattern(insulation_hatch_pattern[0], 1, TRUE);
              insulation_hatch_no=0;
              SolidHatchPattern = 0;
              SolidHatch = 0;
              insulation_hatch = TRUE;
          }
        break;
     case ID_INSULATION+1:
            if (insulation_hatch_pattern[1] > 0)
            {
                set_hatch_pattern(insulation_hatch_pattern[1], 1, TRUE);
                insulation_hatch_no=1;
                SolidHatchPattern = 0;
                SolidHatch = 0;
                insulation_hatch = TRUE;
            }
        break;
      case ID_IMAGE_PATTERN:
        //hatch_image_pattern();
        //  TEMPORARY
        break;
      case ID_IMAGE_PATTERN_SCALE:
      case ID_IMAGE_PATTERN_SCALE + 1:
      case ID_IMAGE_PATTERN_SCALE + 2:
      case ID_IMAGE_PATTERN_SCALE + 3:
      case ID_IMAGE_PATTERN_SCALE + 4:
      case ID_IMAGE_PATTERN_SCALE + 5:
      case ID_IMAGE_PATTERN_SCALE + 6:
        hatch_image_pattern_scale(ev->Number);
        break;
      case ID_IMAGE_PATTERN_ANGLE:
      case ID_IMAGE_PATTERN_ANGLE + 1:
      case ID_IMAGE_PATTERN_ANGLE + 2:
      case ID_IMAGE_PATTERN_ANGLE + 3:
        hatch_image_pattern_angle(ev->Number);
        break;
	  default :
          if ((ev->Number >= ID_IMAGE_PATTERN_DX) && (ev->Number < (ID_IMAGE_PATTERN_DX + 10)))
          {
              hatch_image_pattern_dx(ev->Number);
              break;
          }
          else if ((ev->Number >= ID_IMAGE_PATTERN_DY) && (ev->Number < (ID_IMAGE_PATTERN_DY + 10)))
          {
              hatch_image_pattern_dy(ev->Number);
              break;
          }
          else if ((ev->Number >= ID_TRANSLUCENCY) && (ev->Number < (ID_TRANSLUCENCY + 20)))
          {
              set_solidhatchtranslucency(ev->Number, 1);
              break;
          }
        insulation_hatch = FALSE;
	    set_hatch_pattern (ev->Number - ID_END, 1, FALSE) ;
	    break ;
	}
      default :
	break ;
    }
  }
}

T_PTR_Hatch_Def_Param Get_Hatch_Def_Table (void)
/*--------------------------------------------*/
{
  static T_Hatch_Def_Param  s_def_param = {u8"", 0, 1} ;
  if (i_a_hatch_pattern_no > 2)
  {
    s_def_param.i_size = s__def_param.i_size ;
  }
  else
  {
	  s_def_param.i_size = max(1, i_a_hatch_pattern_no);
  }
  return &s_def_param ;
}

T_PTR_Hatch_Line_Def Get_Hatch_Line_Def_Table (void)
/*-----------------------------------------------*/
{
  static T_Hatch_Line_Def s_line_def [2] =
  {  {0, 0,0, 0,.125, 0, 0, 0, {0,0,0,0,0,0,0,0,0,0,0,0}},
    {Pi2 / 4, 0, 0, 0,.125, 0, 0, 0, {0,0,0,0,0,0,0,0,0,0,0,0}}  } ;
  if (i_a_hatch_pattern_no > 2)
  {
    return ptrs__hatch_line_def ;
  }
  else
  {
	s_line_def [0].df_dist = df__dist;
    s_line_def [1].df_dist = df__dist ;
    return &s_line_def [0] ;
  }
}

void* obiekt_LlS_wybrany(void)
/*----------------------------*/
{
    unsigned typ;
    typ = Blinia | Bluk | Bwwielokat;
    return select_w(&typ, NULL);
}


void *obiekt_Ll_wybrany(void)
/*----------------------------*/
{
	unsigned typ;
	typ = Blinia | Bluk;
	return select_w(&typ, NULL);
}


BOOL grab_hatch_pattern(void)
{
    void* ad;
    char* adbe;
    char* adbs;
    BLOK* adb = NULL;
    WIELOKAT *w;
    double x0b, y0b;
    long_long l_abd;
    int li;
    char* scale_ptr;
    char* dx_ptr;
    char* angle_ptr;
    char* dy_ptr;
    char* name_ptr;

    global_point_in_solid = TRUE;
    global_any_choice = TRUE;
    if ((ad = obiekt_LlS_wybrany()) != NULL)
    {
        global_point_in_solid = FALSE;
        global_any_choice = FALSE;
        adb = find_first_hatch_block(ad, &x0b, &y0b);

        if (adb != NULL)
        {
            
            l_abd = (long_long)adb - (long_long)dane;

            adbe = (char*)adb + (long)(sizeof(NAGLOWEK) + adb->n - 1);  //MODIFIED!!! long_long
            adbs = find_hatch_solid_pattern(adb, adbe);

            if (adbs != NULL)
            {
                w = (WIELOKAT*)adbs;
                if ((w->empty_typ == 0) && (w->pattern == 1))
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
                    memmove(&SolidHatchPatternScale, scale_ptr, sizeof(short int));
                    memmove(&SolidHatchPatternDx, dx_ptr, sizeof(short int));
                    memmove(&SolidHatchPatternAngle, angle_ptr, sizeof(short int));
                    memmove(&SolidHatchPatternDy, dy_ptr, sizeof(short int));
                    memmove(&SolidHatchPatternName, name_ptr, strlen(name_ptr) + 1);

                    return TRUE;
                }
            }
        }
    }
    global_point_in_solid = FALSE;
    global_any_choice = FALSE;
    return FALSE;
}

void hatch_file_reopen(void) {
    i__file_handle = open(HATCH_TEMP_FILE, O_RDWR | O_BINARY);
}

int Hatch_Proc ( int comput_area, double df_apx1, double df_apy1, double df_apx2, double df_apy2 )   /* (void) */
/*----------------------------------------------------------------------------------------------*/
{
  double df_x, df_y;
  int ret_View;
  char pattern_file[MAXPATH], pattern_file_new[MAXPATH];
  char znak7, znak8;
  int i;
  char **pattern_list;
  int pattern_i;
  void *ad;
  BLOK *adb = NULL;
  char *adbe;
  double x0b, y0b;
  int li;
  long_long l_abd;
  LINIA Linia_G;
  int ret, ret_end=0, ret_h;
  
  hatch_cursor = TRUE;
  blok_xorput = FALSE;

    //zmiana dokladnosci
  comput_area1 = comput_area ;

  if (comput_area==1) hatch_cursor = FALSE;

  if (comput_area1 == 10)  comput_area1 = 0;
  df_apx1_=df_apx1;
  df_apy1_=df_apy1;
  df_apx2_=df_apx2;
  df_apy2_=df_apy2;

  i__file_handle = open(HATCH_TEMP_FILE, O_RDWR | O_BINARY);

  /*odczytanie bloku wzoru hatch*/

  if ((comput_area==0) || (comput_area == 10))
   {
     strcpy(pattern_file,s_hatch_param_df_name);

     normalize_file_name(&pattern_file);
     if ((strcmp(pattern_file, "________") == 0) || (strcmp(pattern_file, "XXXXXXXX") == 0))
         strcat(pattern_file, _SOLID_SUFFIX_);

     strcat(pattern_file,".axx");
   }
    else strcpy(pattern_file,GEOMETRY_AXX);
once_again_view:
  if (out_pole_ignore==FALSE)
    ret_View=Import_View_Hatch_Pattern (pattern_file, &df_x, &df_y, comput_area);
     else
     {
       df_x=area_df_x;
       df_y=area_df_y;
       ret_View=1;
     }
  
  if (ret_View == 1)
  {

	Cur_offd(X, Y);
    InfoList (2) ;

	adb = NULL;

	if (comput_area == 10) //edit existing 
	{   
		li = 0;
        global_point_in_solid = TRUE;
        global_any_choice =  TRUE;
		if ((ad = obiekt_LlS_wybrany()) != NULL)
		{
            global_point_in_solid = FALSE;
            global_any_choice =  FALSE;
			memcpy(&Linia_G, &LiniaG, sizeof(LINIA));
            if (((NAGLOWEK *)ad)->obiekt==Olinia) memcpy(&LiniaG, (LINIA*)ad, sizeof(LINIA));  //for vector patterns only
			adb = find_first_hatch_block(ad, &x0b, &y0b);
			
			if (adb != NULL)
			{
				//we got hatch block address and insertpoint
				//1. deleting lines of previous hatch, leaving outline (typ=225)
				//2. changing Atrybut to Ablok of outline (typ=225)
				//3. setting insert point

				PTR__GTMP7=(char*)adb;

				adbe = /*(long_long)*/ (char*)adb + (long)(sizeof(NAGLOWEK) + adb->n - 1);  //MODIFIED!!!  long_long

				li = zmien_atrybut_hatch_outline(adb, adbe, ANieOkreslony, Ablok, 1);
				okno_r();
				zmien_atrybut_preserve_hatch_outline(adb, adbe, ANieOkreslony, Ausuniety);
				okno_all();
				usun_blok(adb, adbe);

                adb=(BLOK*)PTR__GTMP7;

                NAGLOWEK *nag=(char*)adb+sizeof(NAGLOWEK)+B3+adb->dlugosc_opisu_obiektu;
                NAGLOWEK *nag1=(char*)nag+sizeof(NAGLOWEK)+nag->n;

				df_x = x0b;
				df_y = y0b;
			}	

		}
        global_point_in_solid = FALSE;
        global_any_choice =  FALSE;

	}

    if (blok_xorput==FALSE)
     { blokzap (ADP, ADK, Ablok, XOR_PUT, 1) ;
       blok_xorput = TRUE;
     }
    if ((ADP == NULL) || (ADK == NULL))  //pattern sample doesn't exists, even not NOPATTAERN.axx
    {ClearInfo () ;
     close(i__file_handle);
     hatch_cursor = FALSE;
     flip_screen();
     return 0;
    }
    Error = 0;

	LiniaG.blok = 1;

    ret_h = hatch_proc ((long_long) 0, (long_long) dane_size, 1, &df_x, &df_y, &s_hatch_param, comput_area, df_apx1_, df_apy1_, df_apx2_, df_apy2_, adb, 0, 1) ;

	if (comput_area == 10) //edit existing 
		memcpy(&LiniaG, &Linia_G, sizeof(LINIA));

    LiniaG.blok = 0 ;

    if(ptrs__hatch_line_def != NULL)
    {
      free(ptrs__hatch_line_def);
      ptrs__hatch_line_def = NULL;
    }
    if (Error == 0)
    {
      ClearInfo () ;
	  layer_info();
	  view_scale();
    }

    if (comput_area == 10) ret_end = 1;
    //    goto once_again_hatch;
  }
  else if (ret_View == 2) //zmiana pliku
   {
     if (blok_xorput==FALSE)
      { blokzap (ADP, ADK, Ablok, XOR_PUT, 1) ;
        blok_xorput=TRUE;
      }
     strcpy(pattern_file,s_hatch_param_df_name);

      normalize_file_name(&pattern_file);
     if ((strcmp(pattern_file, "________") == 0) || (strcmp(pattern_file, "XXXXXXXX") == 0))
         strcat(pattern_file, _SOLID_SUFFIX_);

     strcat(pattern_file,".axx");

     goto once_again_view;
   }
  else if (ret_View == 20) //from folder
  {
	  ////CUR_OFF(X, Y);

	  if (blok_xorput == FALSE)
	  {
		  blokzap(ADP, ADK, Ablok, XOR_PUT, 1);
		  blok_xorput = TRUE;
	  }

	  //memory allocation
	  pattern_list = malloc(PATTERN_NAMES_MAXNO * sizeof(char*));
	  if (pattern_list == NULL) return 0;
	  //preparing file list
	  
	  for (i = 0; i < m__list_hatch.max; i++)
	  {
		  pattern_list[i] = (char *)malloc(PATERN_NAME_MAXLEN+16);
		  if (pattern_list[i] == NULL)
		  {
			  free(pattern_list);
			  return 0;
		  }
	  	  if (ptrs__hatch_def_param!=NULL)
		     strncpy(pattern_file_new, &(*m__list_hatch.pola)[i].txt[0], PATERN_NAME_MAXLEN);
		  pattern_file_new[PATERN_NAME_MAXLEN - 1] = '\0';

		  ////utf82none(pattern_file_new);  //UTF8 file name
          normalize_file_name(&pattern_file_new);

          if ((strcmp(pattern_file_new, "________") == 0) || (strcmp(pattern_file_new, "XXXXXXXX") == 0))
              strcat(pattern_file_new, _SOLID_SUFFIX_);

		  strcat(pattern_file_new, ".axx");
		  sprintf(pattern_list[i],"%s", pattern_file_new);
	  }
	  
	  //opening folder
	  zmien_atrybut(dane, dane + dane_size, Ablok, Aoblok);

	  pattern_i = FileNamePattern(pattern_list, m__list_hatch.max);

	  zmien_atrybut(dane, dane + dane_size, Aoblok, Ablok);

	  //closing folder
	  for (i = 0; i < m__list_hatch.max; i++) 
		  free(pattern_list[i]);
	  free(pattern_list);

	  if (pattern_i >= 0)
	  {
          insulation_hatch=FALSE;

		  i_a_hatch_pattern_no = pattern_i;
		  m__list_hatch.poz = pattern_i;
		  if (m__list_hatch.poz > m__list_hatch.maxw)
		  {
			  m__list_hatch.foff = m__list_hatch.poz - m__list_hatch.maxw + 1;
			  m__list_hatch.poz = m__list_hatch.maxw - 1;
		  }
		  set_hatch_pattern(pattern_i, 1, FALSE);
          SolidHatchPattern = 0;
          if (pattern_i == 0) SolidHatch = 1;
          else SolidHatch = 0;
	  }

  	if (ptrs__hatch_def_param!=NULL)
	  strcpy(pattern_file, &(*m__list_hatch.pola)[i_a_hatch_pattern_no].txt[0]);

      normalize_file_name(&pattern_file);
      if ((strcmp(pattern_file, "________") == 0) || (strcmp(pattern_file, "XXXXXXXX") == 0))
          strcat(pattern_file, _SOLID_SUFFIX_);

	  strcat(pattern_file, ".axx");
	  
	  goto once_again_view; //Temporary
  }
  else if (ret_View == 21) //from image folder
  {

      Cur_offd(X, Y);
  
      if (blok_xorput == FALSE)
      {
          blokzap(ADP, ADK, Ablok, XOR_PUT, 1);
          blok_xorput = TRUE;
      }

      //opening folder

      zmien_atrybut(dane, dane + dane_size, Ablok, Aoblok);

      Cur_offd(X, Y);

      pattern_i = hatch_image_pattern();

      zmien_atrybut(dane, dane + dane_size, Aoblok, Ablok);

      if (pattern_i >= 0)
      {
          Cur_offd(X, Y);
          set_hatch_pattern(pattern_i, 1, FALSE);
          SolidHatchPattern = 1;
          SolidHatch = 0;

          insulation_hatch=FALSE;
         
          strcpy(pattern_file, __IMAGE);

          strcpy(s__def_param.sz_name, __IMAGE);
          strcpy(s_hatch_param_df_name, __IMAGE);

          normalize_file_name(&pattern_file);
          if ((strcmp(pattern_file, "________") == 0) || (strcmp(pattern_file, "XXXXXXXX") == 0))
              strcat(pattern_file, _SOLID_SUFFIX_);

          strcat(pattern_file, ".axx");
      }
      else Cur_offd(X, Y);

     
      goto once_again_view; //Temporary
  }
  else if (ret_View == 22) //grab image pattern
  {

      if (blok_xorput == FALSE)
      {
          blokzap(ADP, ADK, Ablok, XOR_PUT, 1);
          blok_xorput = TRUE;
      }

      //opening folder

      zmien_atrybut(dane, dane + dane_size, Ablok, Aoblok);


      //pattern_i = hatch_image_pattern();
      pattern_i = Wez_HatchSolid_Pattern();

      zmien_atrybut(dane, dane + dane_size, Aoblok, Ablok);

      if (pattern_i == 1)  //pattern or solid
      {
          insulation_hatch=FALSE;

          if (SolidHatchPattern == 1) //pattern
          {
              Cur_offd(X, Y);
              set_hatch_pattern(0, 1, FALSE);

              SolidHatch = 0;

              ret = SetPattern("", SolidHatchPatternName, ".PNG");

              strcpy(pattern_file, "__IMAGE");

              strcpy(s__def_param.sz_name, "__IMAGE");
              strcpy(s_hatch_param_df_name, "__IMAGE");

              normalize_file_name(&pattern_file);
              if ((strcmp(pattern_file, "________") == 0) || (strcmp(pattern_file, "XXXXXXXX") == 0))
                  strcat(pattern_file, _SOLID_SUFFIX_);

              strcat(pattern_file, ".axx");
          }
          else //solid
          {
              Cur_offd(X, Y);
              set_hatch_pattern(0, 1, FALSE);

              SolidHatch = 1;

              strcpy(pattern_file, "__SOLID");

              strcpy(s__def_param.sz_name, "__SOLID");
              strcpy(s_hatch_param_df_name, "__SOLID");

              normalize_file_name(&pattern_file);

              strcat(pattern_file, ".axx");
          }
      }
      else Cur_offd(X, Y);

      goto once_again_view; //Temporary
  }
  else
  {
    if (blok_xorput==FALSE)
     {
      blokzap (ADP, ADK, Ablok, XOR_PUT, 1) ;
      blok_xorput=TRUE;
     }
    ret_end = 0;
  }

 hatch_cursor = FALSE;


  flip_screen();

 return ret_end;

}

int Hatch_Proc_Test1 ( int comput_area, double df_apx1, double df_apy1, double df_apx2, double df_apy2 )   /* (void) */
/*--------------------------------------------------------------------------------------------------------*/
{
  double df_x, df_y ;
  int ret_View;
  char pattern_file [MAXPATH];
  char znak7, znak8;
  int ret=-1;
  blok_xorput = FALSE;
  i__file_handle = open (HATCH_TEMP_FILE, O_RDWR|O_BINARY) ;
  //zmiana dokladnosci
  comput_area1 = comput_area ;
  df_apx1_=df_apx1;
  df_apy1_=df_apy1;
  df_apx2_=df_apx2;
  df_apy2_=df_apy2;

  strcpy(pattern_file,GEOMETRY_AXX);
once_again_view:
  ret_View=Import_View_Hatch_Pattern (pattern_file, &df_x, &df_y, comput_area);

  if (ret_View == 1)
  {

	Cur_offd(X, Y);
    InfoList (2) ;
    LiniaG.blok = 1 ;

    if (blok_xorput==FALSE)
     { blokzap (ADP, ADK, Ablok, XOR_PUT, 1) ;
       blok_xorput = TRUE;
     }
    if ((ADP == NULL) || (ADK == NULL))
    {ClearInfo () ;

     close(i__file_handle);
     return FALSE;
    }
    Error = 0;

	ret = 	hatch_proc_test ((long_long)0, (long_long)dane_size,
		df_x, df_y, &s_hatch_param, comput_area1, df_apx1_, df_apy1_, df_apx2_, df_apy2_) ;
    LiniaG.blok = 0 ;
    if(ptrs__hatch_line_def != NULL)
    {

      free(ptrs__hatch_line_def);
      ptrs__hatch_line_def = NULL;
    }
    if (Error == 0)
    {
      ClearInfo () ;
    }

  }

  return ret;
}

#undef __O_HATCH__





