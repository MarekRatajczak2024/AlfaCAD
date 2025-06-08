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

#include <forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include <string.h>
#include <stdlib.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "b_menu.h"

#include "leak_detector_c.h"

extern int Simple_Menu_Proc (TMENU *) ;
extern char *GetStringFromClipboard(void);
extern void PutStringToClipboard(char *ptrsz_sz_tmp);

extern int utf82unicode(unsigned char *utf8text, unsigned char *unicodetext);
extern void unicode2win(char *unicodetext, char *wintext, int count);
extern int32_t valid_utf8 (const uint8_t * input, int32_t input_length);
extern void decodingwin(char *text);
extern float jednostkiX0_(long pixels);
extern float jednostkiY0_(long pixels);

static POLE *pmList_String ;
static POLE pm_tmp [] = { {u8"",' ',0,NULL}, } ;
extern int utf8len(const char *s);
#ifdef LINUX
extern void Put_Str_To_Clip(char *ptrsz_buf);
#else
extern void Put_Str_To_Clip(char* ptrsz_buf);
#endif

#if defined (MENU_B)
#define MaxMenuWin 10
#define MaxTextLength 15
#define SKALA 2
static TMENU mList_String = {0, 0,0, MaxTextLength , 1, 3,0, CMNU,CMBR,CMTX,0,0,0,0,0,&pmList_String,NULL,NULL};
#else
#define MaxMenuWin 15
#define MaxTextLength 25
#define SKALA 1
static TMENU mList_String = {0, 0,0, MaxTextLength , 1, 3,0, CMNU,CMBR,CMTX,0,0,0, 0,0,&pm_tmp, NULL,NULL};
#endif

#define BUF_LIST_SIZE 4096 //1000

typedef struct
{
   int string_no ;        /*liczba tekstow*/
   unsigned buf_size ;    /*rozmiar bufora*/
   unsigned first_free ;  /*offset do wolnej pamieci w buforze*/
   char *ptrsz_list ;      /*adres bufora*/
}
t_string_list ;

t_string_list s_string_list = { 0, 0, 0, NULL } ;

void DoneBuffers7(void)
{
  if (s_string_list.ptrsz_list != NULL)  free (s_string_list.ptrsz_list);
}

BOOL Ini_String_List (void)
/*------------------------*/
{
  BOOL b_ret ;
  b_ret = FALSE ;

  if (NULL != (s_string_list.ptrsz_list = (char*)malloc (BUF_LIST_SIZE)))
  {
     b_ret = TRUE ;
     s_string_list.buf_size = BUF_LIST_SIZE ;
  }
  return b_ret ;
}

static BOOL delete_if_equel (char *ptr_string)
/*------------------------------------------*/
{
  int len_str, len, i ;
  char *ptr_s ;

  len_str = 0 ;
  for (i = 0 ; i < s_string_list.string_no ; i++)
  {
    ptr_s = s_string_list.ptrsz_list + len_str ;
    if (0 == stricmp (ptr_s, ptr_string))
    {
      len = strlen (ptr_s) + 1 ;
      memmove (ptr_s, ptr_s + len, s_string_list.first_free - (len_str + len)) ;
      s_string_list.first_free -= len ;
      s_string_list.string_no-- ;
      return TRUE ;
    }
    len_str += strlen (ptr_s) + 1 ;
  }
  return FALSE ;
}


BOOL Add_String_To_List (char *ptr_string)
/*--------------------------------------*/
{
  int len, len_str, dl ;
  char *ptr_s ;

  if (NULL == s_string_list.ptrsz_list && ptr_string [0] == '\0')
  {
    return FALSE ;
  }
  delete_if_equel (ptr_string) ;
  len = strlen (ptr_string) ;

  if (len<s_string_list.buf_size)  //string is not too long
  {
   if (s_string_list.buf_size <= s_string_list.first_free + len)
   {
      dl = s_string_list.first_free + len + 1 - s_string_list.buf_size;
      len_str = 0;
      ptr_s = s_string_list.ptrsz_list;
      do
      {
          len_str += strlen(ptr_s) + 1;
          ptr_s = s_string_list.ptrsz_list + len_str;
          s_string_list.string_no--;
      } while (len_str < dl);
      s_string_list.first_free -= len_str;
      memmove(s_string_list.ptrsz_list, ptr_s, s_string_list.first_free);
   }
   strcpy(&s_string_list.ptrsz_list[s_string_list.first_free], ptr_string);
   s_string_list.string_no++;
   s_string_list.first_free += len + 1;
  }
#ifdef LINUX
  //copy to clipboard
  Put_Str_To_Clip(ptr_string);
#else
  //copy to clipboard
  Put_Str_To_Clip(ptr_string);
#endif

  return TRUE ;
}


static BOOL check_mem_list (void)
/*------------------------------*/
{
    unsigned long_long l_free;
    unsigned int l_size;
  BOOL b_ret ;

  b_ret = FALSE ;
  l_free = get_free_mem_size (0) ;
  l_size = s_string_list.string_no * sizeof(POLE) ;
  mList_String.maxw = MaxMenuWin ;
  mList_String.max = MaxMenuWin ;
  l_size += Get_Menu_Image_Size (&mList_String) ;
  if (l_size < l_free - 500) /*500 - rezerwa na dalsze alokacje */
  {
    pmList_String = (POLE*) malloc /*getmem*/ (s_string_list.string_no * sizeof(POLE));
    b_ret = TRUE ;
  }
  return b_ret ;
}

static void free_mem_list (void)
/*----------------------------*/
{
    free(pmList_String);
}


void get_strings_list(int *string_no, long_long *ptrsz_long)
{
	*string_no = s_string_list.string_no;
	*ptrsz_long = (long_long)s_string_list.ptrsz_list;  //MODIFIED!!!
}

static void set_list_string (void)
/*------------------------------*/
{
  int k, max_len, len, i, len_str ;
  char *ptr_s ;
  int l, l1;

  int maxtextlength=0;

  k = s_string_list.string_no - 1 ;
  max_len = 0 ;
  len_str = 0 ;
  for (i = 0 ; i < s_string_list.string_no ; i++, k--)
  {
    ptr_s = s_string_list.ptrsz_list + len_str ;

    l=sizeof(pmList_String[k].txt);

    strncpy(pmList_String [k].txt , ptr_s, l-1) ;  //128

    if ((unsigned char)pmList_String[k].txt[l-2] > 127)   //126
        pmList_String[k].txt[l-2] = '\0';  //126
	pmList_String [k].txt[l-1]='\0';  //127

    l1=utf8len(pmList_String [k].txt);

    if (l1>maxtextlength) maxtextlength=l1;

    pmList_String [k].wcod = '\1' ;
    pmList_String [k].menu = NULL ;
    len_str += strlen (ptr_s) + 1 ;
    len = strlen (ptr_s)  ;
    if (len > max_len)
    {
      max_len = len ;
    }
  }
  mList_String.poz = 0 ;
  mList_String.foff = 0 ;
  mList_String.max = s_string_list.string_no ;
  mList_String.xdl = maxtextlength  ;
  if (s_string_list.string_no < mList_String.maxw)
  {
    mList_String.maxw = 0 ;
  }
  mList_String.pola = pmList_String ;
}


BOOL Get_Str_From_List (char *ptrsz_buf,
                        int i_poz,
                        int i_buflen,
                        int xpcz,
                        int ypcz)
/*---------------------------------*/
{
  int n, i_len ;
  BOOL b_ret ;
  char sz_tmp [MaxMultitextLen] ;  //MaxTextLen
  char *ptrsz_tmp ;
  double X__, Y__;

  if (NULL == s_string_list.ptrsz_list || s_string_list.string_no == 0)
  {
    return FALSE ;
  }
  if (FALSE == check_mem_list ())
  {
     return FALSE ;
  }
  set_list_string () ;
  b_ret = FALSE ;
  //mList_String.xpcz = xpcz / (8 * SKALA) ;
  //mList_String.ypcz = 2 + ypcz / (8 * SKALA) ;
    mList_String.xpcz = xpcz;
    mList_String.ypcz = ypcz;
    X__=X;
    Y__=Y;
    X=jednostkiX0_(xpcz);
    Y=jednostkiY0_(ypcz);
  if ((n = Simple_Menu_Proc (&mList_String) - 1) >= 0)
  {
    b_ret = TRUE ;
    ptrsz_tmp = (*mList_String.pola)[n].txt ;
    i_len = strlen (ptrsz_tmp) ;
    strcpy (sz_tmp, &ptrsz_buf [i_poz]) ;
    ptrsz_buf [i_poz] = '\0' ;
    strncat (ptrsz_buf, ptrsz_tmp, i_buflen - i_poz) ;
    strncat (ptrsz_buf, sz_tmp, i_buflen - i_poz - i_len) ;
    ptrsz_buf [i_buflen - 1] = '\0' ;
  }
    X=X__;
    Y=Y__;
  free_mem_list () ;  
  return b_ret ;
}

#ifndef LINUX
void encodewin0(char *utf8text, char *wintext)
{
	int ret;
	int count;
	char unicodetext[MaxMultitextLen]; //MaxTextLen

	count = utf82unicode(utf8text, &unicodetext);
    unicode2win(unicodetext, wintext, count);

}
/*
void encodewin(char* utf8text, char* wintext)
{
    int ret;
    int count;
    char unicodetext[MaxMultitextLen]; //MaxTextLen

    //count = utf82unicode(utf8text, &unicodetext);
    //unicode2win(unicodetext, wintext, count);

    utf82win(utf8text, wintext);
}

void Put_Str_To_Clip(char *ptrsz_buf)
{
	char ptrsz_sz_tmp[MaxMultitextLen];  //MaxTextLen

	//utf82win(ptrsz_buf, &ptrsz_sz_tmp);
	//PutStringToClipboard(&ptrsz_sz_tmp);
	
	encodewin(ptrsz_buf, &ptrsz_sz_tmp);  //UTF-8 original
	PutStringToClipboard(ptrsz_sz_tmp);

    ////PutStringToClipboard(ptrsz_buf);
}
*/

BOOL Get_Str_From_Clip(char *ptrsz_buf,
	int i_poz,
	int i_buflen,
	int xpcz,
	int ypcz)
	/*---------------------------------*/
{
	int n, i_len;
	BOOL b_ret;
	char sz_tmp[MaxMultitextLen];  //MaxTextLen
	char *ptrsz_tmp;
	char ptrsz_sz_tmp[MaxMultitextLen];  //MaxTextLen
    int valid;

	b_ret = FALSE;
	
	ptrsz_tmp= GetStringFromClipboard();

	strcpy(&ptrsz_sz_tmp, ptrsz_tmp);

    //if (IsTextUnicode(&ptrsz_sz_tmp))
    valid = valid_utf8(ptrsz_sz_tmp, strlen((char*)ptrsz_sz_tmp));

    if (!valid) decodingwin(&ptrsz_sz_tmp);  //UTF-8 original

	i_len = strlen(ptrsz_sz_tmp);
	strcpy(sz_tmp, &ptrsz_buf[i_poz]);
	ptrsz_buf[i_poz] = '\0';

	strncat(ptrsz_buf, ptrsz_sz_tmp, i_buflen - i_poz);
	strncat(ptrsz_buf, sz_tmp, i_buflen - i_poz - i_len);
	ptrsz_buf[i_buflen - 1] = '\0';

	b_ret = TRUE;
	
	return b_ret;
}
#else

void encodewin(char *utf8text, char *wintext)  //TO DO
{
    wintext=utf8text;
}

#endif
