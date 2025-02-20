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

#pragma warning(disable : 4996)
#include<forwin.h>
#include <string.h>
#define ALLEGWIN
#include <allegext.h>
#include <stdio.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "bib_edit.h"
#include "o_libfun.h"


#include "leak_detector_c.h"

#define lpmax 10
static int lp=0, np=0;
static ESTR *et[lpmax];

char *ptr_st_ = NULL;
char *ptr_values_ = NULL;

extern void outetextxy (int x, int y, int maxlength, int width, char *s, int ink, int paper) ;
extern BOOL Get_Graph_Values (ESTR *ptrs_et) ;
extern void setfillstyle_(int pattern, int color);
extern void setcolor(int kolor);
extern int ttf_digits27_len;
extern int ttf_digits13_len;
extern float ttf_width_w;
extern int TTF_text_len(char* text);
extern int utf8len(const char* s);
extern int lenutf8to(const unsigned char* s, int to);
extern void set_strwyj(void);
extern int get_dialog_string(char *tekst, char *legal, int maxlength, int width0, int kom, int *tab);

extern BOOL text_edit_dialog;

enum modes { E_Clear, E_Out, E_Edit, E_Draw } ;


void Set_Edit_Vector (ESTR *lps_et, int (*ptr_fun)(BOOL))
/*------------------------------------------------------*/
{
  lps_et->x=maxX-PL266;
  lps_et->y=0;
#define r18 18
#define r27 27
  lps_et->lmax=r27 /*r18*/;
  lps_et->val_no_max = 2 ;
  lps_et->mode = GV_VECTOR ;
  lps_et->format = "%#10.5lg\0%#10.5lg;%#7.2lf" ;
  lps_et->ESTRF=ptr_fun;
}


#define x15 15

static void uaktualnijs1 (ESTR *lps_et, int n)
/*-------------------------------------------*/
{
  int x,y, x1, x2;
  char buf [MaxTextLen*4] ;
  char *buf1=NULL;
  char *ptr;
  int max_len;
  int text_len_pxl;
  int fwlen;
  int m_len;

  if (lps_et->st==NULL)
      return;

  if (lps_et->lmax != 27)
  {
	  x = lps_et->x;
	  x1 = x + (int)((float)(lps_et->lmax) * ttf_width_w); // width_w;
	  strcpy(buf, lps_et->st);

      text_len_pxl = TTF_text_len(buf);
	  fwlen = utf8len(buf);
	  max_len = (int)(((float)(x1-x) / ((float)text_len_pxl / fwlen)) /*+ 0.5*/);
	  if (max_len < fwlen)
	  {
		  m_len = lenutf8to(lps_et->st, max_len);
		  if (m_len<strlen(buf)) buf[m_len] = '\0';
	  }
  }
  else
  {
      x1 = lps_et->x;
	  x = lps_et->x - ttf_digits27_len - x15;
	  x2 = lps_et->x - ttf_digits13_len -x15;
	  strcpy(buf, lps_et->st);
	  buf[lps_et->lmax] = '\0';

	  ptr = strchr(buf, ';');
	  if (ptr)
	  {
		  *(ptr+1) = '\0';
		  buf1 = ptr + 2;
	  }
  }
  if (x<1) x=1;
  y = lps_et->y;

  moveto(x+x15,y/*+1*/);

  if(n == E_Clear || n == E_Out)
  {
    setfillstyle_(SOLID_FILL, BKCOLOR) ;
    bar(x, y - ESTR_Y, x1+x15 + 1, y + ED_INF_HEIGHT - ESTR_Y - 1);
    if (n == E_Out)
    {
      if ((change_color_wsp==TRUE) && (options1.uklad_geodezyjny==1)) setcolor (11 /*12*/);
        else setcolor (kolory.ink) ;
      outtext_r (buf) ;
	  if (buf1)
	  {
		  moveto(x2 + x15, y /* + 1*/);
		  outtext_r(buf1);
	  }
    }
  }
  else if (n == E_Edit)
  {
    setfillstyle_(SOLID_FILL,kolory.paperk);
    bar (x, 0, x1 + x15, ED_INF_HEIGHT - ESTR_Y);  //5
  }
  return;
}

static void uaktualnijs (int n)
/*----------------------------*/
{
  uaktualnijs1 (et [np],  n) ;
}


void Out_Edited_Draw_Param (ESTR *lps_et, BOOL out)
/*--------------------------------------------------*/
{
  int n ;

  n = E_Clear ;
  if (out == TRUE)
  {
    n = E_Out ;
  }
  uaktualnijs1 (lps_et, n) ;
}

void DoneBuffers6(void)
{

  while (lp>0) usunstr(lp-1);

}

void set_np(int np0)
{
    np=np0;
}

int get_lp(void)
{
   return lp;
}

void redraw_str(int no_)
{
    if (et[no_]->st != NULL) {
        Out_Edited_Draw_Param(et[no_], TRUE);
    }
}

int dodajstr(ESTR *e)  	/*zaklada sie, ze dodajstr i usunstr wystepuja parami*/
/*-------------------*/
{
  void *ptr_st = NULL, *ptr_values = NULL ;

  if ( (lp + 1 > lpmax) ||
	(NULL == (ptr_st = malloc (MaxTextLen))) ||
	(e->val_no_max > 0 &&
	(NULL == (ptr_values = malloc (e->val_no_max * sizeof (double))))) )
  {
    return -1 ;
  }
  ptr_st_ = ptr_st;
  ptr_values_ = ptr_values;
  np = lp;
  e->st = (char *)ptr_st;
  e->st [0] = '\0' ;
  e->val_no = 0 ;
  e->values = (double *)ptr_values ;
  et[lp++] = e ;
  e->np = np;
  return np ;
}

/*usunstr musi byc wywolane przed komunikat0 */
void usunstr(int n)			/*musi wystapic po dodajstr*/
/*----------------*/
{
  int i;

  if(!lp || n>=lp || n<0) return;
  if (et[n]->st != NULL)
  {
      uaktualnijs1(et[n], E_Clear);
      free((void*)et[n]->st);
      et[n]->st = NULL;
  }
  if (et[n]->values != NULL)
  {
      free((void*)et[n]->values);
      et[n]->values = NULL;
  }
  lp--;
  for(i=n;i<lp;i++) et[i]=et[i+1];    /*dla n < lp  bedzie zle */
  if(n<np || (n==np && np==lp)) np--;
}

static void  nooop(void)
{}
#define noop (void ( *)(int x,int y))nooop

static int read_estr(void)
/*-----------------------*/
{
  int zn ;
  void (*CUR)(int ,int);
  char *ptr, *ptrsz_format1, *ptrsz_format2 ;
  char buf [MaxTextLen*2+1];
  char buf1 [MaxTextLen*2+1];
  BOOL b_graph_value, b_ret ;
  int ret;
  int tab = 1;

  unsigned bold;
  unsigned italics;
  unsigned justowanie;
  unsigned underline;

  bold=TextG.bold;
  italics=TextG.italics;
  justowanie=TextG.justowanie;
  underline=TextG.underline;

  for (ptr = et[np]->st; ptr [0] == ' '; ptr++) ;
  strcpy (buf, ptr) ;

  if  ((text_edit_dialog) && ((et[np]->mode == GV_STRING) || (et[np]->mode == GV_STRING_D))) uaktualnijs(E_Draw);
      else uaktualnijs(E_Edit);
  CUR=MVCUR; MVCUR=noop;
  b_graph_value = FALSE ;
  if (GV_POINT == et [np]->mode ||
      GV_DIST == et [np]->mode ||
      GV_VECTOR == et [np]->mode ||
      GV_VECTOR_XY == et [np]->mode ||
      GV_ANGLE == et [np]->mode)
  {
    b_graph_value = TRUE ;
  }

  if ((et[np]->mode == GV_STRING_D) && (et[np]->address!=NULL))
  {
      TextG.bold = ((TEXT*)et[np]->address)->bold;
      TextG.italics = ((TEXT*)et[np]->address)->italics;
      TextG.justowanie = ((TEXT*)et[np]->address)->justowanie;
      TextG.underline = ((TEXT*)et[np]->address)->underline;
  }

  if (((et[np]->mode==GV_STRING) || (et[np]->mode == GV_STRING_D)) && (text_edit_dialog==TRUE)) {
      ret=get_dialog_string(buf, "", MaxTextLen*2, 0, 13, &tab);
      if (ret==-1) zn=TAB;
      else if (ret==0) zn=ESC;
      else zn=ENTER;
  }
  else
  {
      zn = editstring(buf, "", MaxTextLen * 2, (float)et[np]->lmax, b_graph_value, et[np]->extend, TRUE, 4, 4);
  }
  MVCUR=CUR;
  uaktualnijs (E_Clear) ;
#define DOWNKEY 336
#define MIDDLE_BUTTON 2
  if (zn == ENTER || zn == DOWNKEY || zn == MIDDLE_BUTTON)
  {
    if (zn == ENTER)
    {
      b_ret = TRUE ;
      b_graph_value = FALSE ;
      if (0 < et[np]->val_no_max)
      {
	et[np]->val_no = et[np]->val_no_max ;
	b_ret = calculator (buf, &et[np]->val_no, et[np]->values) ;
      }
    }
    else
    {
      b_graph_value = TRUE ;
      b_ret = Get_Graph_Values (et[np]) ;
    }
    if (b_ret == TRUE)
    {
      strcpy (buf1, et [np]->st) ;
      ptrsz_format2 = ptrsz_format1 = et [np]->format ;
      ptrsz_format2 += strlen (ptrsz_format2) + 1 ;
      if (et [np]->val_no_max == 0)
      {
	strcpy (et [np]->st, buf) ;
      }
      else
      if (et [np]->val_no == 1)
      {
	if (ptrsz_format1 [0] == '\001')
	{
	  sprintf (et [np]->st, ++ptrsz_format1, (int)et[np]->values [0]);
	}
	else
	{
	  sprintf (et [np]->st, ptrsz_format1, et[np]->values [0]);
	}
      }
      else
      if (et [np]->val_no == 2)
      {
	    sprintf (et [np]->st, ptrsz_format2, et[np]->values [0], et[np]->values [1]);
      }

      if (0 == et [np]->ESTRF (b_graph_value))
      {
	    strcpy (et [np]->st, buf1) ;
	    b_ret = FALSE ;
      }
          zn = ENTER;
      if (b_ret == FALSE)
      {
	    zn = ESC ;
      }
    }
  }
  uaktualnijs(E_Out);

  TextG.bold=bold;
  TextG.italics=italics;
  TextG.justowanie=justowanie;
  TextG.underline=underline;

  return zn;
}


static int read_estr_zn(char zn0)
/*-----------------------------*/
{
  int zn ;
  void (*CUR)(int ,int);
  char *ptrsz_format1, *ptrsz_format2 ;
  char buf [MaxTextLen];
  char buf1 [MaxTextLen];
  BOOL b_graph_value, b_ret ;

  //clear if error
  ClearErr();

  strcpy (buf, "") ;
  buf[0]=zn0;
  buf[1]='\0';
  uaktualnijs(E_Edit);
  CUR=MVCUR; MVCUR=noop;
  b_graph_value = FALSE ;
  if (GV_POINT == et [np]->mode ||
      GV_DIST == et [np]->mode ||
      GV_VECTOR == et [np]->mode ||
      GV_VECTOR_XY == et [np]->mode ||
      GV_ANGLE == et [np]->mode)
  {
    b_graph_value = TRUE ;
  }
  zn= editstring (buf, "", MaxTextLen, (float)et[np]->lmax, b_graph_value, 1, FALSE, 4, 4) ;
  MVCUR=CUR;
  uaktualnijs (E_Clear) ;
#define DOWNKEY 336
#define MIDDLE_BUTTON 2
  if (zn == ENTER || zn == DOWNKEY || zn == MIDDLE_BUTTON)
  {
    if (zn == ENTER)
    {
      b_ret = TRUE ;
      b_graph_value = FALSE ;
      if (0 < et[np]->val_no_max)
      {
	    et[np]->val_no = et[np]->val_no_max ;
	    b_ret = calculator (buf, &et[np]->val_no, et[np]->values) ;
      }
    }
    else
    {
      b_graph_value = TRUE ;
      b_ret = Get_Graph_Values (et[np]) ;
    }
    if (b_ret == TRUE)
    {
      strcpy (buf1, et [np]->st) ;
      ptrsz_format2 = ptrsz_format1 = et [np]->format ;
      ptrsz_format2 += strlen (ptrsz_format2) + 1 ;
      if (et [np]->val_no_max == 0)
      {
	    strcpy (et [np]->st, buf) ;
      }
      else if (et [np]->val_no == 1)
      {
	    if (ptrsz_format1 [0] == '\001')
	    {
	      sprintf (et [np]->st, ++ptrsz_format1, (int)et[np]->values [0]);
	    }
	    else
	    {
	      sprintf (et [np]->st, ptrsz_format1, et[np]->values [0]);
	    }
      }
      else if (et [np]->val_no == 2)
      {
	    sprintf (et [np]->st, ptrsz_format2, et[np]->values [0], et[np]->values [1]);
      }
      if (0 == et [np]->ESTRF (b_graph_value))
      {
	    strcpy (et [np]->st, buf1) ;
	    b_ret = FALSE ;
      }
      zn = ENTER;
      if (b_ret == FALSE)
      {
	    zn = ESC ;
      }
    }
  }
  uaktualnijs(E_Out);
  return zn;
}

int edycjastr(void)
/*------------------*/
{
  int zn;

          if(!lp) return 0;
  while(1)
   { zn=read_estr();
	 switch(zn)
	  { case ESC   : return 0;
	case ENTER: {
        set_strwyj();
        return -(np + 1);
    }
	case SHTAB   : if(np<lp-1)np++ ;
			 else np=0;
			 break;
	case TAB: if(np>0)np-- ;
			 else np=lp-1 ;
			 break ;
	default    : break ;
	  }
   }
}

int edycjastr_zn(char zn0)
/*---------------------*/
{
  int zn;

  if(!lp) return 0;
  while(1)
   { zn=read_estr_zn(zn0);
	 switch(zn)
	  { case ESC   : return -1; //0;
	case ENTER: return  -(np+1) ;
	case TAB   : if(np<lp-1)np++ ;
			 else np=0;
			 break;
	case SHTAB: if(np>0)np-- ;
			 else np=lp-1 ;
			 break ;
	default    : break ;
	  }
   }
}
