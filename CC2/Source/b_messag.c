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

#define __MESSAGE__
#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS

#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<stdio.h>
#ifdef _WIN32
#include <io.h>
#endif
#include<string.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "b_messag.h"

#include "message.h"

static int len_pxl_bak=0;
static char comunics[]="COMUNICS.LIB";
static char errors[]="ERRORS.LIB";

static BOOL short_notice=FALSE;
static char short_notice_str[128]="\0";  //64
static BOOL short_notice_center=FALSE;

extern TMENU mTryb ;
extern void color_bar(void);
extern void getviewsettings(struct viewporttype  *viewport);
extern void setviewport(int left, int top, int right, int bottom, int clip);
extern void setfillstyle_(int pattern, int color);
extern void setcolor(int kolor);
extern void view_scale(void);
extern int getx(void);
extern int gety(void);
extern BOOL Add_String_To_List (char *ptr_string);
extern void DF_to_String (char *ptrsz_buf, char *ptrsz_format, double df_val, int max_size);
extern BOOL glb_silent;
extern void     settextjustify(int horiz, int vert);
extern int TTF_text_len(char* text);
extern int utf8len(const char* s);
extern BOOL get_short_notice(void);
extern int vfv(int v);

#define BUFMAKLEN 20000
#define y1 0
#define y11 1
#define y2 4  //2
#define y3 3

void layer_info (void);

static int i__x, i__y ;
void show_macro(void);
void komunikat_str_short_reminder(void);

void bar2(void)
{
    bar(0, ED_INF_HEIGHT - 1, maxX - 1, 2 * ED_INF_HEIGHT - y1);
}

void bar2_margin(void)
{
    bar(0,ED_INF_HEIGHT /*- 1*/,maxX - /*PL266 -1 */ 27*WIDTH, 2*ED_INF_HEIGHT - y1);
}

void bar2_margin_pxl(int len_pxl)
{
    bar(0,ED_INF_HEIGHT /*- 1*/,len_pxl, 2*ED_INF_HEIGHT - y1);
}

void bar2_tab(void)
{
    bar(vfv(10), ED_INF_HEIGHT, maxX - 1, 2 * ED_INF_HEIGHT - y1); //1);
}

void bar2_tab_half_margin(void)
{
    bar(vfv(10), ED_INF_HEIGHT, (maxX - 15) / 2.0 + 4, 2 * ED_INF_HEIGHT - y1); //1);
}

void bar2_tab_len(int len_pxl)
{
    if (len_pxl > (maxX - 15)) len_pxl = maxX - 11;
    else if (len_pxl < (maxX - 15)/2.0) len_pxl = (maxX - 15)/2.0;

    bar(vfv(10), ED_INF_HEIGHT, len_pxl+4, 2 * ED_INF_HEIGHT - y1); //1);
}

void bar2_short(int len)
{
    bar(maxX/2 - len*WIDTH-5, ED_INF_HEIGHT, maxX/2, 2 * ED_INF_HEIGHT - y1); //1);
}

void bar2_short_pxl(int len, BOOL center)
{
    if (!center) bar(maxX/2 - len, ED_INF_HEIGHT, maxX/2 /*+ len*/, 2 * ED_INF_HEIGHT - y1); //1);
    else bar(maxX/2 - len / 2, ED_INF_HEIGHT, maxX/2 + len / 2, 2 * ED_INF_HEIGHT - y1); //1);
}

void bar1(void)
{
    bar (0, 0, maxX, ED_INF_HEIGHT - 1) ;
}
void bar1_margin(void)
{
    bar (0, 0, maxX - 27 * WIDTH, ED_INF_HEIGHT - 1) ;
}

void bar1_half(void)
{
    bar (0, 0, maxX / 2, ED_INF_HEIGHT - 1);
}


void ClearInfo(void)
{ if(Info)
   { InfoList(0);
	 Info=0;
   }
   //layer_info ();
}

void InfoListStr0(char *st)
{
	char buf[100];
	struct viewporttype viewinfo;

	//  Info=1;
	getviewsettings(&viewinfo);
	setviewport(0, 0, getmaxx(), getmaxy(), 1);
	setfillstyle_(SOLID_FILL, BKCOLOR);
	//bar(0, 0, maxX - 1, ED_INF_HEIGHT - 1);
    bar1();
	//moveto(1,1);
    moveto(1,2);
	setcolor(kolory.ink);
	sprintf(buf, st);
	outtext_r(buf);
	setviewport(viewinfo.left, viewinfo.top, viewinfo.right, viewinfo.bottom, 1);
}

void InfoList(int n)
{
  struct viewporttype viewinfo;

 // int a;

  getviewsettings (&viewinfo);
  setviewport(0, 0, getmaxx(),getmaxy(), 1);

  Info=1;
  setfillstyle_(SOLID_FILL, BKCOLOR) ;
  bar2();
  if(n)
   { moveto(vfv(10),ED_INF_HEIGHT + y2);
	 setcolor(kolory.ink);
	 outtext_r(komunikaty[n]);
   }
  setviewport(viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1);
}

void InfoListStr1(int n, char *st)
{
  char buf [255] ;
  struct viewporttype viewinfo;

  Info=1;
  getviewsettings (&viewinfo);
  setviewport(0, 0, getmaxx(),getmaxy(), 1);
   setfillstyle_(SOLID_FILL, BKCOLOR) ;
   bar1();
   //moveto(1,1);
   moveto(1,2);
   setcolor(kolory.ink);
   sprintf (buf, messages [n], st) ;
   outtext_r(buf);
  setviewport(viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1);
}

void InfoListStr(int n, char *st)
{
  char buf [255 /*100*/] ;
  struct viewporttype viewinfo;

  //if (glb_silent == TRUE) return;

  settextjustify(LEFT_TEXT, TOP_TEXT);
  Info=1;
  getviewsettings (&viewinfo);
  setviewport(0, 0, getmaxx(),getmaxy(), 1);
   setfillstyle_(SOLID_FILL, BKCOLOR) ;
   bar2_tab();
   moveto(vfv(11),ED_INF_HEIGHT + y2);
   setcolor(kolory.ink);
   sprintf (buf, messages1 [n], st) ;
   outtext_r(buf);
   setviewport(viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1);
}

void InfoListStr_Inv(int n, char *st)
{
  char buf [100] ;
  struct viewporttype viewinfo;

  Info=1;
  getviewsettings (&viewinfo);
  setviewport(0, 0, getmaxx(),getmaxy(), 1);
   setfillstyle_(SOLID_FILL, kolory.ink /*BKCOLOR*/) ;
   //bar(vfv(10),ED_INF_HEIGHT,maxX-1,2*ED_INF_HEIGHT - 1);
   bar2_tab();
   moveto(vfv(11),ED_INF_HEIGHT + y2);
   setcolor(BKCOLOR /*kolory.ink*/);
   sprintf (buf, messages1 [n], st) ;
   outtext_r(buf);
  setviewport(viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1);
}


void ClearInfo0(void)
{ char st[20];

  strcpy(st,"");
  InfoListStr0(st);
  layer_info ();
}

void ClearErr(void)
{ if(Error)
   { ErrList(0);
	 Error=0;
    if (get_short_notice()) komunikat_str_short_reminder();
   }
}


void ErrListFactory(int n, char *error_str)
{ struct viewporttype viewinfo;
  int nn,kom_len;
  static char komunikat[255];
  int len_pxl;

  if (glb_silent == TRUE) return;

  getviewsettings (&viewinfo);
  setviewport(0, 0, getmaxx(),getmaxy(), 1);
  Error=1;
  nn=n;
  if (n) setfillstyle_(SOLID_FILL,kolory.paperk);
  else
   setfillstyle_(SOLID_FILL, BKCOLOR) ;
   bar2_tab_half_margin();
  if(n)
   {
     moveto(vfv(11), ED_INF_HEIGHT + y2);
 	  setcolor(kolory.inkk);

	  sprintf(komunikat, "%s %s", errors_kom[n], error_str);

      len_pxl = TTF_text_len(komunikat);

       bar2_tab_len(len_pxl);

	  komunikat[90] = '\0';
	  kom_len = (int)strlen (komunikat) - 1;
	  if (komunikat [kom_len] == '\n')
	   {
	    komunikat [kom_len] = '\0';
	   }
	 outtext_r(komunikat);
   }
  setviewport(viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1);
 
}


void ErrListCode(int n, int error_code)
{
	static char error_str[32];
	sprintf(error_str, "%s: %d", errors_kom[212], error_code);
	ErrListFactory(n, error_str);
}

void ErrList(int n)
{
 	ErrListFactory(n, "");
    if (get_short_notice()) komunikat_str_short_reminder();
}

void ErrListStr(char *st)
{
  struct viewporttype viewinfo;

  if (glb_silent == TRUE) return;

  getviewsettings (&viewinfo);
  setviewport(0, 0, getmaxx(),getmaxy(), 1);
  color_bar();
  Error=1;
  setfillstyle_(SOLID_FILL,kolory.paperk);
  bar2_tab();
  moveto(vfv(11)/*1*/,ED_INF_HEIGHT + y2);  //+1
  setcolor(kolory.inkk);
  outtext_r(st);
  setviewport(viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1);
}

char *get_komunikat_ptr (int n)
{
  return komunikaty_kom [n];
}

void Get_Current_Pos (int *x, int *y)
/*---------------------------------*/
{
  *x = i__x ;
  *y = i__y ;
}

void Set_Current_Pos(void)
{
    i__x = getx () ;
    i__y = gety () ;
}

void komunikat(int n)
/*-------------------*/
{ char *komunikat1;
  char komunikat01[256];

  struct viewporttype viewinfo;

  Komunikat_R=n;

  getviewsettings (&viewinfo);
  setviewport(0, 0, getmaxx(),getmaxy(), 1);
  if (n) setfillstyle_(SOLID_FILL,kolory.paperk);
  else
   setfillstyle_(SOLID_FILL, BKCOLOR) ;

  bar2_margin();
  if (n)
  {
    moveto(1,ED_INF_HEIGHT + y3 /*1*/);  //y2
    setcolor(kolory.inkk);

   if (n==255) outtext_r (global_kom);
    else
     {
       komunikat1=get_komunikat_ptr(n);
       strcpy(komunikat01,komunikat1);
       outtext_r (komunikat01);
     }
  }
  if (!n)
   {
    layer_info ();
    view_scale ();
   }
  i__x = getx () ;
  i__y = gety () ;
  setviewport(viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1);
  
}

void komunikat_len(int n)
/*-------------------*/
{ char *komunikat1;
    char komunikat01[256];

    struct viewporttype viewinfo;
    int len_pxl;

    Komunikat_R=n;

    if (n==255) len_pxl=TTF_text_len(global_kom);
    else len_pxl=TTF_text_len(get_komunikat_ptr(n));

    getviewsettings (&viewinfo);
    setviewport(0, 0, getmaxx(),getmaxy(), 1);
    if (n) setfillstyle_(SOLID_FILL,kolory.paperk);
    else
        setfillstyle_(SOLID_FILL, BKCOLOR) ;

    //bar2_margin();
    bar2_margin_pxl(min(max(len_pxl+20,maxX/3) , maxX/2 - 10*WIDTH));
    if (n)
    {
        moveto(1,ED_INF_HEIGHT + y3 /*1*/);  //y2
        setcolor(kolory.inkk);

        if (n==255) outtext_r (global_kom);
        else
        {
            komunikat1=get_komunikat_ptr(n);
            strcpy(komunikat01,komunikat1);
            outtext_r (komunikat01);
        }
    }
    if (!n)
    {
        layer_info ();
        view_scale ();
    }
    i__x = getx () ;
    i__y = gety () ;
    setviewport(viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1);

}

BOOL Add_String_To_List1 (char *ptr_string)
{
Add_String_To_List ( ptr_string ) ;
return TRUE;
}

void komunikat_str(char *st)
{
  struct viewporttype viewinfo;

  getviewsettings (&viewinfo);
  setviewport(0, 0, getmaxx(),getmaxy(), 1);
  setfillstyle_(SOLID_FILL,kolory.paperk);

  bar2_margin();
  moveto(vfv(10),ED_INF_HEIGHT + y3);
  setcolor(kolory.inkk);
  outtext_r(st);
  setviewport(viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1);
}

void komunikat_str_len(char *st)
{
    struct viewporttype viewinfo;
    int len, len_pxl;

    getviewsettings (&viewinfo);
    setviewport(0, 0, getmaxx(),getmaxy(), 1);
    setfillstyle_(SOLID_FILL,kolory.paperk);

    len_pxl=TTF_text_len(st);

    bar2_margin_pxl(min(max(len_pxl+20,maxX/3) , maxX/2 - 10*WIDTH));
    moveto(vfv(10),ED_INF_HEIGHT + y3);
    setcolor(kolory.inkk);
    outtext_r(st);
    setviewport(viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1);
}

void remove_short_notice(void)
{
    short_notice=FALSE;
    layer_info();
}

void komunikat_str_short_reset(void)
{
    len_pxl_bak=0;
}

void komunikat_str_short_mask(BOOL center)
{
    struct viewporttype viewinfo;

    getviewsettings(&viewinfo);
    setviewport(0, 0, getmaxx(), getmaxy(), 1);
    setfillstyle_(SOLID_FILL, kolory.paperk);
    bar2_short_pxl(len_pxl_bak, center);
}

void komunikat_str_short(char *st, BOOL stay, BOOL center)
{
	struct viewporttype viewinfo;
    int len, len_pxl;

	getviewsettings(&viewinfo);
	setviewport(0, 0, getmaxx(), getmaxy(), 1);
	setfillstyle_(SOLID_FILL, kolory.paperk);
    len_pxl=TTF_text_len(st);

    setfillstyle_(SOLID_FILL, BKCOLOR);
    bar2_short_pxl(len_pxl_bak, center);

    if (strlen(st)>0)
    {
        setfillstyle_(SOLID_FILL, kolory.paperk);
        bar2_short_pxl(len_pxl, center);
        len_pxl_bak = len_pxl;

        if (!center)
          moveto(maxX / 2 - len_pxl, ED_INF_HEIGHT + y3);
        else moveto(maxX / 2 - len_pxl / 2, ED_INF_HEIGHT + y3 + 1);

        setcolor(kolory.inkk);
        outtext_r(st);

        if (stay) {
            short_notice = TRUE;
            strncpy(short_notice_str, st, 127);  //64
            short_notice_str[127] = '\0';  //63
            short_notice_center=center;
        }
    }
    setviewport(viewinfo.left, viewinfo.top, viewinfo.right, viewinfo.bottom, 1);
    len_pxl_bak = len_pxl;
}

void komunikat_str_short_reminder(void) {
    komunikat_str_short(short_notice_str, FALSE, short_notice_center);
}

BOOL get_short_notice(void)
{
    return short_notice;
}


void komunikat0(int n)
{
  struct viewporttype viewinfo;

  getviewsettings (&viewinfo);
  setviewport(0, 0, getmaxx(),getmaxy(), 1);
  Komunikat_R0=n;
   setfillstyle_(SOLID_FILL, BKCOLOR) ;
  if (n ==255)
   {
     n = 0;
     bar1();
   }
     else bar1_margin();
  if(n && n != -1)
   {
      //moveto(1,1);
      moveto(1,2);
	 setcolor(kolory.ink);
	 outtext_r(komunikaty0[n]);
   }

  if (mkbuf) show_macro();
  setviewport(viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1);
}

void set_komunikat0(int n, char *message)
{
    komunikaty0[n]=message;
}

void komunikat0_str (int n, char *st)
/*--------------------------------------*/
{
  struct viewporttype viewinfo;
  char buf [MAXPATH * 2] ;

  getviewsettings (&viewinfo) ;
  setviewport(0, 0, getmaxx(), getmaxy(), 1) ;
   setfillstyle_(SOLID_FILL, BKCOLOR) ;
  if ((n == 9) || (n == 23)) bar1();
    else bar1_half();
  //moveto(1,1) ;
  moveto(1,2) ;
  setcolor (kolory.ink) ;
  sprintf (buf, messages_str [n], st) ;
  outtext_r (buf) ;
  setviewport(viewinfo.left, viewinfo.top, viewinfo.right,viewinfo.bottom, 1);
}

void  Internal_Error (int line, char * file)
/*-----------------------------------------*/
{
  char message [128] ;

  sprintf (message, u8"System message %s, %d", file, line);
  ErrListStr (message);
}


void layer_info (void)
/*--------------------*/
{
  char message [128], *st ;
  char c_on, c_edit, c_point ;
  char krok_kur[8];

  settextjustify(LEFT_TEXT, TOP_TEXT);

  if (Current_Layer>No_Layers) return;
  c_on = (Layers [Current_Layer].on == 1) ? '+' : '-' ;
  c_edit = (Layers [Current_Layer].edit == 1) ? '+' : '-' ;
  c_point = (Layers [Current_Layer].point == 1) ? '+' : '-' ;
  if (snap_ == 1) DF_to_String (krok_kur, "%-6.2f", milimetryob (krok_s), 6) ;
    else strcpy(krok_kur,"min");

  if (sel.gor)
  {
	st = gg[sel.nr];

	sprintf (message, u8"%s  %d  '%s' %c%s %c%s %c%s   %s:%s  k=%s%s",
      _LAYER_,
      Current_Layer + 1,
      Layers [Current_Layer].name,
      c_on,
        _V_,
      c_edit,
        _E_,
      c_point,
        _P_,
        _MODE_,
      st,
      krok_kur,
      orto ? ORTHO : "") ;
  }
  else
  {
	sprintf (message, u8"%s  %d  '%s' %c%s %c%s %c%s  k=%s%s",
        _LAYER_,
      Current_Layer + 1,
      Layers [Current_Layer].name,
      c_on,
        _V_,
      c_edit,
        _E_,
      c_point,
        _P_,
      krok_kur,
      orto ? ORTHO : "") ;
  }
  if (options1.view_only_current_layer==1) strcat(message,u8"  ~~~~~~~");

  color_bar();

  InfoListStr (0, message);

  if (get_short_notice()) komunikat_str_short_reminder();
  Info=0;
}

#undef __MESSAGE__
