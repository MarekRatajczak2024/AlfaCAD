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

#include<forwin.h>
#include<graphics.h>
#define ALLEGWIN
#include <allegext.h>
#ifndef LINUX
#include<io.h>
#endif
#include <stdio.h>
#include<wchar.h>
#include<stdlib.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_editst.h"
#include "unicode.h"

#define DXIL 	8 //6  //0   //copy from o_dialog,h

#define LINE myline

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

extern char *get_komunikat_ptr (int n) ;
extern BOOL Add_String_To_List (char *) ;
extern BOOL Get_Str_From_List (char *, int, int, int, int) ;
extern void Get_Current_Pos (int *, int *) ;
extern void Set_Current_Pos (void);
extern void myline(int x1, int y1, int x2, int y2);
extern void setfillstyle_(int pattern, int color);
extern int getx(void);
extern int gety(void);
extern void setwritemode( int mode );
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);
extern void setcolor(int kolor);
extern int keys;
extern char catch (unsigned char zn);
extern float ttf_width_w;
extern int TTF_char_len(unsigned int unicode);

extern int utf82unicode(unsigned char *utf8text, unsigned char *unicodetext);
extern int TTF_text_len(char *text);
extern int TTF_text_len_pos(char *text, int pos);
extern BOOL TTF_text_pos_x0(char *text, int x0, int y0, int width_w, int *pos);

extern int inukeys(TMENU *menu);

extern void take_altkey_away(void);
extern int Expand_flex(void);
extern BOOL Get_Str_From_Clip(char *ptrsz_buf, int i_poz, int i_buflen, int xpcz, int ypcz);
extern void Put_Str_To_Clip(char *ptrsz_buf);

extern void lock_mouse(void);
extern void _free_mouse(void);

extern void my_scare_mouse(void);
extern void my_unscare_mouse(void);


#ifdef LINUX
extern void Put_Str_To_Clip(char *ptrsz_buf);
extern BOOL Get_Str_From_Clip(char *ptrsz_buf,
                       int i_poz,
                       int i_buflen,
                       int xpcz,
                       int ypcz);

extern int Expand_flex();
extern int TTF_char_len(unsigned int unicode);

#endif
extern void set_cursor_pointer(void);
extern void set_cursor_edit(void);

BOOL editing_text = FALSE;

static int mouse_x_=0, mouse_y_=0;


#define MIDDLE_BUTTON 2
#define BS 8
#define FORMFEED 12
#define ENTER 0x0d  /*13*/
#define ESC 27
#define SHTAB 2063 //271
#define HOMEKEY 2119 //327
#define ENDKEY 2127 //335
#define UPKEY 2120 //328
#define DOWNKEY 2128 //336
#define PGUPKEY 2121 //329
#define PGDNKEY 2129 //337
#define LEFTKEY 2123 //331
#define INSKEY 2049 //257 
#define INSKEY1 2130 //338
#define RIGHTKEY 2125 //333
#define DELKEY 2131 //339
#define CTRLLEFTKEY 2163 //371
#define CTRLRIGHTKEY 2164 //372
#define CTRLPGUPKEY 2180 //388
#define CTRLPGDNKEY 2166 //374
#define CTRLHOMEKEY 2167 //375
#define CTRLENDKEY 2165 //373
#define PASTECLIP 2070
#define COPYCLIP 2051
#define F1 2107 //315
#define F2 2108 //316
#define F3 2109 //317
#define F4 2110 //318
#define F5 2111 //319
#define F6 2112 //320
#define F7 2113 //321
#define F8 2114 //322
#define F9 2115 //323
#define F10 2116 //324
#define F11 2117

#define ALT_A  2078 //286
#define ALT_F  2081 //289
#define ALT_C  2094 //302
#define ALT_E  2066 //274
#define ALT_L  2086 //294
#define ALT_N  2097 //305
#define ALT_O  2072 //280
#define ALT_S  2079 //287
#define ALT_Z  2092 //300
#define ALT_X  2093 //301

#define RIGHT  0x01
#define LEFT   0x02
#define CAPSLOCK 0x40

#define CTRLYKEY 25
#define CTRLBCSPKEY 127
#define CTRLSPC 2016 //2051
#define BCSPKEY 2056 //264
#define CTRLXKEY 24
#define CTRLKKEY 11
#define CTRLWKEY 23

#define y_1 0
#define y_1_1 1
#define y_2 2
#define y_3 4
#define y_4 5

static void nooop(void)
{}
#define noop (void ( *)(int x,int y))nooop

typedef struct tedit {
             int x0, y0;                /*polorzenie poczatku tekstu*/
	     int max;			/*maksymalna dlogosc edytowanego tekstu*/
	     int maxw;			/*rozmiar okna ( 0 dla nie scrolowanych )*/
	     int poz;                   /*pozycja kursora i offset*/
	     int foff;			/*pierwsza pozycja w okienku*/
	   } T_EDITST;

static int pol_key (int c)
/*----------------*/
{
  int key_flags;
  BOOL big;

  big = ((key_shifts & KB_SHIFT_FLAG) &&
                   !(key_shifts & KB_CAPSLOCK_FLAG)) ||
       (!(key_shifts & KB_SHIFT_FLAG) &&
                    (key_shifts & KB_CAPSLOCK_FLAG))  ;

#ifdef UTF8
  switch (c)
  {
   case ALT_A :
     return big ? 0xC484 : 0xC485;
   case ALT_C :
     return big ? 0xC486 : 0xC487;
   case ALT_E :
     return big ? 0xC498 : 0xC499;
   case ALT_L :
     return big ? 0xC581 : 0xC582;
   case ALT_N :
     return big ? 0xC583 : 0xC584;
   case ALT_O :
     return big ? 0xC393 : 0xC383;
   case ALT_S :
	 return big ? 0xC59A : 0xC59B;
   case ALT_Z :
     return big ? 0xC5BB : 0x0xC5BC;   //z.
   case ALT_X :
     return big ? 0xC5B9 : 0xC5BA;    //z,
   case ALT_F :
     return big ? 0xCF95 : 0xCF86;  //phi
   default :
     return c;
  }
#else
  switch (c)
  {
  case ALT_A:
	  return big ? 143 : 134;
  case ALT_C:
	  return big ? 149 : 141;
  case ALT_E:
	  return big ? 144 : 145;
  case ALT_L:
	  return big ? 156 : 146;
  case ALT_N:
	  return big ? 165 : 164;
  case ALT_O:
	  return big ? 163 : 162;
  case ALT_S:
	  return big ? 152 : 158;
  case ALT_Z:
	  return big ? 161 : 167; // 160 : 166;
  case ALT_X:
	  return big ? 160 : 166; // 161 : 167;
  case ALT_F:
	  return 232;
  default:
	  return c;
  }
#endif
}


void cursorT_on(int x,int y)		/*usunac (rowniez z interface)*/
/*-------------------------*/
{ x = x; y = y; }

void cursorT_off(int x,int y)
/*-------------------------*/
{ x = x; y = y; }

static void cursorT_On(int x,int y, BOOL ins)
/*-----------------------------------------*/
{ int b;
  b= 7 * WIDTH / 8;
  setwritemode(COPY_PUT);
  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
  setcolor(kolory.inkk);
  my_scare_mouse();

  LINE(x+1,y+ED_INF_HEIGHT-y_3,x+b,y+ED_INF_HEIGHT-y_3);
  if ( ins) LINE(x+1,y+ED_INF_HEIGHT-y_4,x+b,y+ED_INF_HEIGHT-y_4);
  my_unscare_mouse();
}
static void cursorT_Off(int x,int y, BOOL ins)
/*------------------------------------------*/
{ int b;
  b= 7 * WIDTH / 8;
  setwritemode(COPY_PUT);
  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
  setcolor(kolory.paperk);
  my_scare_mouse();
  LINE(x+1,y+ED_INF_HEIGHT-y_3,x+b,y+ED_INF_HEIGHT-y_3);
  if (ins) LINE(x+1,y+ED_INF_HEIGHT-y_4,x+b,y+ED_INF_HEIGHT-y_4);
  setcolor(kolory.inkk);
  my_unscare_mouse();
}

static void put_add_char (int x0, int y0, int fpos, int len, int width, int width0, int width_w, int fwlen, int width_pxl, int pos_len_pxl, int expand, int y_4_, int y_3_)
{
  int x1, x2;


  show_mouse(NULL);

  if (expand)
  {
	  x1 = x0 - 1 * WIDTH + 1;
	  x2 = x1 + 5;
	  if (fpos == 0)
	  {
		  setfillstyle_(SOLID_FILL, kolory.paperk);
		  bar(x1, y0 - y_4_, x2, y0 + ED_INF_HEIGHT - y_3_);  //y_3
	  }
	  else
	  {
		  setwritemode(COPY_PUT);
		  setcolor(kolory.inkk);
		  setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
		  LINE(x1, y0 + 3, x1, y0 + 3);
		  LINE(x1 + 1, y0 + 2, x1 + 1, y0 + 4);
		  LINE(x1 + 2, y0 + 1, x1 + 2, y0 + 5);
		  LINE(x1 + 3, y0 + 0, x1 + 3, y0 + 6);
	  }
  }

  if (fwlen > width)
  {
	  if (expand)
	  {
		  x1 = x0 + width_pxl - 7;
		  x2 = x1 + 5;

		  setwritemode(COPY_PUT);
		  setcolor(kolory.inkk);
		  setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
		  LINE(x1, y0, x1, y0 + 6);
		  LINE(x1 + 1, y0 + 1, x1 + 1, y0 + 5);
		  LINE(x1 + 2, y0 + 2, x1 + 2, y0 + 4);
		  LINE(x1 + 3, y0 + 3, x1 + 3, y0 + 3);
	  }
  }
  else
  {
	x1 = x0 + pos_len_pxl;
    x2 = x1 + width_w;
    setfillstyle_(SOLID_FILL,kolory.paperk);
	bar(x1, y0 - y_4_, x2, y0 + ED_INF_HEIGHT - y_3_);  //y_3
  }

  show_mouse(screen);
}


static void put_add_char1 (int x0, int y0, int fpos, int len, int width)
{
  int x1, x2;

  x1 = x0 + width * WIDTH /*8*/ + 1;
  x2 = x1 + 5;
  if (fpos + width < len)
  {
    setwritemode(COPY_PUT);
    setcolor (kolory.inkk);
    setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
    LINE (x1,   y0,  x1,  y0+6);
    LINE (x1+1, y0+1,x1+1,y0+5);
    LINE (x1+2, y0+2,x1+2,y0+4);
    LINE (x1+3, y0+3,x1+3,y0+3);
  }
  else
  {
    setfillstyle_(SOLID_FILL,kolory.paperk);
    bar(x1, y0-2, x2, y0+ED_INF_HEIGHT - y_3);
  }
}

int set_wchar0(unsigned char *buf, int len)
{
	int i = 0;
	int j = 0;

	if (strlen(buf) < len) return len;
	while (i < len)
	{
		if (buf[j] >= 127)
			j++;
		i++;
		j++;
	}
	return j;
}

void outtext_r_e (int x0, int y0, int len, char *s, BOOL ini, int y_4_)
/*-------------------------------------------------------------------*/
{
  unsigned char buf [MaxTextLen*2+2];
  int x;
  int len_ttf;
  int char_len_ttf, b_index;
  int wlen;
  int blen;

  strncpy (buf, s, MaxTextLen*2+1);

  blen = strlen(buf);

  if (len < 0)
  {
	  len = 0;  //  !!!!!!!!!
	  return;
  }

  if (len > blen)
	  return;

  //converting len to wlen
  if (len <= blen)
  {
	  wlen = set_wchar0(buf, len);

	  buf[wlen] = '\0';

  }

  my_scare_mouse();

  if (ini)
  {
    setcolor(kolory.inkk_ini);
    setfillstyle_(SOLID_FILL,kolory.paperk_ini);

	len_ttf = TTF_text_len_pos(&buf, wlen);  //wlen

	x = x0 + len_ttf;

	char_len_ttf = ttf_width_w;

    if (len > 0) bar(x0-1,y0-2,x,y0+ED_INF_HEIGHT - y_3);
    setfillstyle_(SOLID_FILL,kolory.paperk);   /*nad kursorem*/
	bar(x + 1, y0 - y_4_, x + char_len_ttf /*8*/, y0 + ED_INF_HEIGHT - y_3);
  }
  else
  {
    setcolor(kolory.inkk);
    setfillstyle_(SOLID_FILL,kolory.paperk);

	len_ttf = TTF_text_len_pos(&buf, wlen);
	x = x0 + len_ttf;

	char_len_ttf = 0;

	bar(x0 - DXIL + 2, y0 - y_4_, x + char_len_ttf, y0 + ED_INF_HEIGHT - y_3);
  }
  moveto(x0,y0);
  outtext_r (buf);
  my_unscare_mouse();
}

void outtext_r_e1 (int x0, int y0, int len, char *s, BOOL ini)
/*-------------------------*/
{
  char buf [256];
  int x;

  strcpy (buf, s);

  if (ini)
  {
    setcolor(kolory.inkk_ini);
    setfillstyle_(SOLID_FILL,kolory.paperk_ini);
    x = x0 + len * WIDTH /*8*/;
    if (len > 0) bar(x0-1,y0-2,x,y0+ED_INF_HEIGHT - y_3);
    setfillstyle_(SOLID_FILL,kolory.paperk);   /*nad kursorem*/
    bar(x+1,y0-2,x+WIDTH /*8*/,y0+ED_INF_HEIGHT - y_3);
  }
  else
  {
    setcolor(0);
    setfillstyle_(SOLID_FILL,kolory.paperk);
    x = x0 + (len + 1) * WIDTH /*8*/;
    bar(x0-1,y0-2,x,y0+ED_INF_HEIGHT - y_3);
  }
  moveto(x0,y0);
  outtext_r (buf);
}

extern int delay_1(void);
extern int delay_2(void);
extern int delay_3(void);


int Get_Key (void)
/*---------------*/
{
   int ret ;
   void (*CUR)(int ,int);

   CUR=MVCUR;
   MVCUR=noop;
   ret = getkey ();
   MVCUR=CUR;
   return ret;
}


int Get_Legal_Key (char *legal)
/*-----------------------------*/
{
    int retval;
    int length;
    char *wc_legal_ptr;
    int i;

#ifndef LINUX
    wchar_t wc_legal[64];
#else
    unsigned short wc_legal[64];
    wchar_t wc_legal_32[64];
#endif

    length = utf82unicode((unsigned char*)legal, (unsigned char*)wc_legal);

#ifdef LINUX
    for (i=0; i<6; i++)
    {
        wc_legal_32[i] = (wchar_t)wc_legal[i];
    }
    wc_legal_ptr=wc_legal_32;
#else
    wc_legal_ptr=wc_legal;
#endif

    while (1)
    {
        retval = Get_Key ();
        if (wcschr (wc_legal_ptr, retval) != NULL)
        {
            break;
        }
    }
    return retval;
}


int getkey(void)
/*-------------------------*/
/* Uses the BIOS to read the next keyboard character */
{
	int  lo, ret_key;

	lo = inkeys(NULL, FALSE);
	sleep_state = FALSE;
	if (lo) return lo;
	lo = inkeys(NULL, FALSE);
	if (lo == 63) delay_1();
	if (lo == 64) delay_2();
	if (lo == 65) delay_3();
	ret_key = lo + 2048;
	return ret_key;
} /* getkey */

int getukey(void)
/*-------------------------*/
/* Uses the BIOS to read the next keyboard character */
{
	int  lo, ret_key;

	lo = inukeys(NULL);
	sleep_state = FALSE;

	return lo;

	if (lo) return lo;
	//lo = (unsigned char)inkeys(NULL);
	lo = inukeys(NULL);
	if (lo == 63) delay_1();
	if (lo == 64) delay_2();
	if (lo == 65) delay_3();
	ret_key = lo + 2048; // 256;
	return ret_key;
} /* getkey */


int utf8len(const char *s)
{
	int len = 0;
	while (*s)
		len += (*(s++) & 0xC0) != 0x80;
	return len;
}

int utf8lento(const char *s, int to)
{
	int len = 0;
	while ((*s) && (len<to))
		len += (*(s++) & 0xC0) != 0x80;
	return len;
}

int lenutf8to(const unsigned char *s, int to)
{
	int len = 0;
	int i = 0;
	while ((*s) && (i < to))
	{
		if (s[len] >= 127) len += 2;
		else len++;
		i++;
	}
	return len;
}

int posutf8tomaxend(const unsigned char *s, int maxend)
{
	int len = 0;
	int i = 0;
	int max_len = utf8len(s);  //e.g. 15
	int to = max_len - maxend;  //e.g. 15 - 10 = 5  marks the position

	while ((*s) && (i < to))
	{
		if (s[len] >= 127) len += 2;
		else len++;
		i++;
	}
	return len;
}

int findlentopxl(const char *s, int max_pxl)
{
	int i=0;
	int len_pxl=0;
	unsigned int unicode;
	char *ptr, *ptr1;

	ptr = s;

	while ((len_pxl < max_pxl) && (*ptr!='\0'))
	{
		unicode = utf8_to_ucs2(ptr, &ptr1);
		len_pxl+=TTF_char_len(unicode);
		i += ptr1 - ptr;
		ptr = ptr1;
        if (unicode==UTF8_BAD_LEADING_BYTE)
            break;
	}
	return i;
}

int findfpostopxl(/*const unsigned*/ char *s, int max_pxl)
{
	int i = 0;
	int len_pxl = 0;
	unsigned int unicode;
	char *ptr0, *ptr, *ptr1;
	int full_length;

	ptr = s;
	len_pxl= TTF_text_len(s);
	if (len_pxl == 0) return 0;

	while ((len_pxl > max_pxl) && (*ptr != '\0'))
	{
		unicode = utf8_to_ucs2(ptr, &ptr1);
		len_pxl -= TTF_char_len(unicode);
        if (ptr1==ptr) return i; //possibly error in encoding
		i += ptr1 - ptr;
		ptr = ptr1;
	}
	return i;
}


void my_show_mouse(BITMAP* sc)
{
	if (sc == NULL)
	{
		show_mouse(NULL);
		mouse_x_ = mouse_x;
		mouse_y_ = mouse_y;
		position_mouse(0, 0);
	}
	else
	{
		position_mouse(mouse_x_, mouse_y_);
		show_mouse(sc);
	}
}

int editstring(unsigned char *s, char *legal, int maxlength, float width0, BOOL b_graph_value, int expand, BOOL last_edit, int y_4_, int y_3_)
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/* Allows the user to edit a string with only certain characters allowed -
   Returns TRUE if ESC was not pressed, FALSE is ESC was pressed.
*/
{
	int c, len, wlen, fwlen, lpos_cur, pos, pos1, fpos;
	uint8_t utf8c[4];
	int scancode;
	BOOL insert = TRUE;
	BOOL b_add;
	int x, y, x0, y0, x00;
	unsigned char bytes[2], c1, c2;
	int bytes_n;
	int len_ttf;
	int width_w;
	int width_pxl;
	int text_len_pxl;
	int m_len;
	int len_pos, len_fpos;
	int pos_len_pxl;
	int width;
    int width_pxl_a;
    BOOL ret;
	int posm_x;
    unsigned char back_s[MaxMultitextLen];
    int x1, y1, x2, y2;
	//int mouse_x_, mouse_y_;

    _free_mouse();
    set_cursor_edit();

	b_add = TRUE;

	editing_text = TRUE;
	
	width_w= ttf_width_w;
	
	x00 = getx();
	y0 = gety() + y_1;

aa:
    width_pxl = width0 * width_w;

    width_pxl_a=getmaxx()-x00-27*WIDTH;

    width_pxl_a = (int)(width0) * ttf_width_w;

	if (width0 == 0) width = maxlength;
	else width = width0;
	x0 = x00;
	y = y0;
	len = strlen(s);
	s[len] = '\0';
	
	wlen = utf8len(s);
	fwlen = wlen;

	if (wlen > maxlength)
	{
		if (s[maxlength] >= 127)
			s[maxlength - 1] = '\0';
		else
			s[maxlength] = '\0';
	}
  
  pos = len;
  lpos_cur = len;
  fpos = 0;

  text_len_pxl = TTF_text_len(s);


  if (!now_is_dialog)
  {
	  posm_x = x00 + text_len_pxl + WIDTH;
	  if (posm_x > getmaxx()) posm_x = x00 - WIDTH;
	  position_mouse(posm_x, y0 + HEIGHT);
  }

  if (text_len_pxl > 0)  width = (int)(((float)width_pxl / ((float)text_len_pxl / fwlen)) /*+ 0.5*/);
  
  fpos = findfpostopxl(s, width_pxl);
  m_len = len - fpos;

  pos_len_pxl = TTF_text_len_pos(&s[fpos], m_len);
 
  if (text_len_pxl > 0)  width = (int)(((float)width_pxl / ((float)text_len_pxl / fwlen)) /*+ 0.5*/);
  wlen = utf8len(s);

  if (wlen >= width)
 
  {
    lpos_cur = width - 1;
    fpos = len - width + 1;

	fpos = lenutf8to(s, wlen - width);
	
  }

if (last_edit==TRUE)
 {
	m_len = findlentopxl(s, width_pxl);

   //  if (fwlen < (width - 1)) m_len = (len - fpos);
   //  else m_len = (width - 1);

   if (fwlen < (width)) m_len = (len - fpos);
   else m_len = (width);

#ifndef LINUX 
   my_show_mouse(NULL);
#endif

	outtext_r_e(x0, y0, m_len, &s[fpos], TRUE, y_4_);
	pos_len_pxl = TTF_text_len_pos(&s[fpos], m_len);

    if (b_add == TRUE) put_add_char (x0, y0, fpos, len, width, width0, width_w, fwlen, width_pxl_a, pos_len_pxl, expand, y_4_, y_3_);
   
  len_ttf = TTF_text_len_pos(&s[fpos], pos-fpos);
  
  x = x0 + len_ttf;
  cursorT_On (x, y, insert);

#ifndef LINUX 
  my_show_mouse(screen);
#endif
  
  c = getukey();

  if (c > 127 && c < 2048)
  {
	  bytes_n = ucs2_to_utf8(c, &utf8c);
	  if (bytes_n == 2) c = utf8c[0] * 256 + utf8c[1];
  }
  else if ((c==F11) || (c==CTRLSPC))
  {
      do_not_draw_cur=TRUE;
      c=Expand_flex();
      do_not_draw_cur=FALSE;
  }

  if (c == F2) c = ESC ;
#ifndef LINUX 
  my_show_mouse(NULL);
#endif
  cursorT_Off (x, y, insert);

      if((c == BS) || (c == ENTER) || (c == F11) || (c == F10) || (c == F9) || (c == ESC) || (c == SHTAB) || (c == HOMEKEY) ||
     (c == ENDKEY) || (c == LEFTKEY) || (c == INSKEY) || (c == INSKEY1) || (c == RIGHTKEY) ||
     (c == MIDDLE_BUTTON) || (c == DOWNKEY)  || (c == COPYCLIP)  || (c == MOUSEENTER))
   {
	 if (fwlen < (width - 1)) m_len = (len - fpos);
	 else m_len = (width - 1);

	 outtext_r_e(x0, y0, m_len, &s[fpos], FALSE, y_4_);
	 pos_len_pxl = TTF_text_len_pos(&s[fpos], m_len);
	 
     if (b_add == TRUE) put_add_char (x0, y0, fpos, len, width, width0, width_w, fwlen, width_pxl_a, pos_len_pxl, expand, y_4_, y_3_);

   }
  else
  {
	 setfillstyle_(SOLID_FILL,kolory.paperk);
	 len_ttf = TTF_text_len_pos(&s[fpos], len - fpos);
	
	 x = x0 + len_ttf;


	 bar(x0-1,y0-2,x,y0+ED_INF_HEIGHT - y_3);


     strcpy(back_s, s);

	 strcpy (s,"");
     len = fpos = pos = lpos_cur = 0;

	 if (fwlen < (width - 1)) m_len = (len - fpos);
	 else m_len = (width - 1);

	  outtext_r_e(x0, y0, m_len, &s[fpos], FALSE, y_4_);
	  pos_len_pxl = TTF_text_len_pos(&s[fpos], m_len);
	 
     if (b_add == TRUE) put_add_char (x0, y0, fpos, len, width, width0, width_w, fwlen, width_pxl_a, pos_len_pxl, expand, y_4_, y_3_);

  }
#ifndef LINUX 
	  my_show_mouse(screen);
#endif
 }
  else
   {
	if (fwlen < (width - 1)) m_len = (len - fpos);
	else m_len = (width - 1);
#ifndef LINUX 
	my_show_mouse(NULL);
#endif
	 outtext_r_e(x0, y0, m_len, &s[fpos], FALSE, y_4_);
	 pos_len_pxl = TTF_text_len_pos(&s[fpos], m_len);
     
      if (b_add == TRUE) put_add_char (x0, y0, fpos, len, width, width0, width_w, fwlen, width_pxl_a, pos_len_pxl, expand, y_4_, y_3_);

	 len_ttf = TTF_text_len_pos(&s[fpos], pos-fpos);
     
	 x = x0 + len_ttf;
     cursorT_On (x, y, insert);

#ifndef LINUX  
	 my_show_mouse(screen);
#endif

     c = getukey();

#ifndef LINUX 
	 my_show_mouse(NULL);
#endif

	 if ((c > 127) && (c < 2048))
	 {
		 bytes_n = ucs2_to_utf8(c, &utf8c);
		 if (bytes_n==2) c = utf8c[0] * 256 + utf8c[1];
	 }
     else if ((c==F11) || (c==CTRLSPC))
     {
         do_not_draw_cur=TRUE;
         c=Expand_flex();
         do_not_draw_cur=FALSE;
     }

     if (c == F2) c = ESC ;
     cursorT_Off (x, y, insert);
#ifndef LINUX  
	 my_show_mouse(screen);
#endif
   }
do
{
	
	switch (c)
	{
	case PGDNKEY:
        get_clip_rect(screen,&x1, &y1, &x2, &y2);
        set_clip_rect(screen, 0,0, getmaxx(), getmaxy());
		if (TRUE == Get_Str_From_List(s, pos, maxlength, x0, y0))
		{
			setfillstyle_(SOLID_FILL, kolory.paperk);
			len_ttf = TTF_text_len_pos(&s[fpos], len - fpos);
	
			x = x0 + len_ttf;
#ifndef LINUX 
			my_show_mouse(NULL);
#endif
			bar(x0 - 1, y0 - 2, x, y0 + ED_INF_HEIGHT - y_3);
#ifndef LINUX 
			my_show_mouse(screen);
#endif

            _free_mouse();
            set_cursor_edit();
            set_clip_rect(screen,x1, y1, x2, y2);
			goto aa;
		}
        else
        {
            strcpy(s, back_s);
            _free_mouse();
            set_cursor_edit();
            set_clip_rect(screen,x1, y1, x2, y2);
            goto aa;
        }
		break;
	case PASTECLIP:
		if (TRUE == Get_Str_From_Clip(s, pos, maxlength, x0, y0 - (y0 * 0.25)))
		{
			goto aa;
		}
		break;
	case COPYCLIP:
		Put_Str_To_Clip(s);
		break;
	case HOMEKEY:
		pos = 0;
		break;
	case ENDKEY:
		if (s[strlen(s)] > 127)
			pos = strlen(s) - 1;
		else pos = strlen(s);
		break;
	case INSKEY:
	case INSKEY1:
		cursorT_Off(x, y, insert);
		insert = !insert;
		cursorT_On(x, y, insert);
		break;
	case LEFTKEY:

		if (pos > 0)
		{
			if (s[pos - 1] > 127)
				pos -= 2;
			else pos--;
		}

		break;
	case RIGHTKEY:

		if (pos < len)
		{
			if (s[pos] > 127)
				pos += 2;
			else pos++;
		}

		break;
	case BS:
		if (pos > 0)
		{
			if (s[pos - 1] > 127)
			{
				memmove(&s[pos - 2], &s[pos], len - pos + 1);
				pos -= 2;
				len -= 2;
			}
			else
			{
				memmove(&s[pos - 1], &s[pos], len - pos + 1);
				pos--;
				len--;
			}
		}
		break;
	case DELKEY:
		if (pos < len)
		{
			if (s[pos] > 127)
			{
				memmove(&s[pos], &s[pos + 2], len - pos - 1);
				len -= 2;
			}
			else
			{
				memmove(&s[pos], &s[pos + 1], len - pos);
				len--;
			}
		}

		break;
	case DOWNKEY:
	case MIDDLE_BUTTON:
		if (TRUE == b_graph_value)
		{
			break;
		}
	case ENTER:
		break;
    case MOUSEENTER:
        pos1=0;
        ret=TTF_text_pos_x0(&s[fpos], x0, y0, width_w, &pos1);
        if (ret) pos=fpos+pos1;
        break;
	case F11:
		break;
	case F10:
		break;
	case F9:
		break;
	case TAB:
		break;
	case SHTAB:
		break;
	case ESC:
		break;
	default:

        if (((legal[0] == 0) || (strchr(legal, c) != NULL)) &&
            (c >= ' ') && ((wlen < maxlength) || ((wlen == maxlength) && (last_edit == TRUE))))
		{
            
			if (last_edit == TRUE)
			{
				show_mouse(NULL);
				setfillstyle_(SOLID_FILL, kolory.paperk);
				bar(x0 - 1, y0 - 2, x, y0 + ED_INF_HEIGHT - y_3);
				show_mouse(screen);
			}
			
				
			if (insert)
			{
				if (c <= 256)
				{
					memmove(&s[pos + 1], &s[pos], len - pos + 1);
					len++;
				}
				else
				{
					memmove(&s[pos + 2], &s[pos], len - pos + 1);
					len += 2;
				}
			}
			else if (pos >= len)
			{
				if (c <= 256) len++;
				else len += 2;
			}
			if (c <= 256) s[pos++] = c;
			else
			{

				c1 = (unsigned char)(c >> 8);
				c2 = (unsigned char)(c & 0xFF);
				s[pos++] = c1;
				s[pos++] = c2;

			}
		}
		break;
	} /* switch */

	

	s[len] = '\0';
	if (len > 508)
	{
	   int a = 0;
    }
  if ((c != ENTER) && (c != F11) && (c != F10) && (c != F9) && (c != ESC) && (c != TAB) && (c !=SHTAB) &&
	((c != DOWNKEY && c != MIDDLE_BUTTON) || FALSE == b_graph_value) )
  {
    int kor = 0;

	 text_len_pxl = TTF_text_len(&s[fpos]);
	 fwlen=utf8len(&s[fpos]);

	 len_pos = utf8lento(s, pos);
	 len_fpos = utf8lento(s, fpos);

	 //normalize width
	 if (text_len_pxl > 0)  width = (int)(((float)width_pxl / ((float)text_len_pxl/fwlen))/*+0.5*/);
	 wlen = utf8len(s);

	 if (wlen <= width)
	 {
	   fpos = 0;
	 }
	 else
	 {
		
	    if (pos < fpos)
	    {
	      fpos = pos;
	    }
	    else
	    if (len_pos >= len_fpos + width)
        {
			fpos = lenutf8to(s, len_pos - width +1);
	    }
		else
	    if ((len_fpos + width) > wlen)
	    {
			fpos = lenutf8to(s, wlen - width +1);
              kor = 1;
        }
		

     }

      if (fpos < 0)
      {
       fpos = 0;
      }

	  if (pos < fpos) pos = fpos;  //EMERGENCY

	  fwlen = utf8len(&s[fpos]);

     //need to find limit in len whatever is shorter: fwlen or  width - kor
	 if (fwlen < (width - kor)) m_len = (len - fpos);
	 else m_len = lenutf8to(&s[fpos], width - kor);

     pos_len_pxl = TTF_text_len_pos(&s[fpos],  m_len);


	 outtext_r_e (x0, y0, m_len, &s[fpos], FALSE, y_4_);

     if (b_add == TRUE) put_add_char (x0, y0, fpos, len, width, width0, width_w, fwlen, width_pxl_a, pos_len_pxl, expand, y_4_, y_3_);
     
	 len_ttf = TTF_text_len_pos(&s[fpos], pos-fpos);
	 x = x0 + len_ttf;
	 cursorT_On (x, y, insert);

	 

	 c = getukey();

	 if ((c > 127) && (c < 2048))
	 {
		 bytes_n = ucs2_to_utf8(c, &utf8c);
		 if (bytes_n == 2) c = utf8c[0] * 256 + utf8c[1];
	 }
     else if ((c==F11) || (c==CTRLSPC))
     {
         do_not_draw_cur=TRUE;
         c=Expand_flex();
         do_not_draw_cur=FALSE;
     }

    if (c == F2) c = ESC ;
	
	 cursorT_Off (x, y, insert);
	
   }
  
 }
 while ((c != ENTER) && (c != F11) && (c != F10) && (c != F9) && (c != ESC) && (c != TAB) && (c !=SHTAB) &&
	 ((c != DOWNKEY && c != MIDDLE_BUTTON) || FALSE == b_graph_value) ) ;

 //re-evaluation of width


   m_len = findlentopxl(s, width_pxl);


   outtext_r_e(x0, y0, m_len, &s[fpos], FALSE, y_4_);


 if ((c == ENTER) || (c == F10) || (c == F9))
 {
   Add_String_To_List (s) ;
 }
 else if (c == F11)
 {
	 simulate_ukeypress(14592, 14592);
 }
 editing_text = FALSE;


 take_altkey_away();

 set_cursor_pointer();
 lock_mouse();

 return c;
} /* editstring */


int editstring1(unsigned char *s, char *legal, int maxlength, int width, BOOL b_graph_value, int expand, int *lpos_cur0)
/*-----------------------------------------------------------------------------------------------------------*/
/* Allows the user to edit a string with only certain characters allowed -
   Returns TRUE if ESC was not pressed, FALSE is ESC was pressed.
*/
{
 int c, len, lpos_cur, pos, fpos, width0 ;
 BOOL insert = TRUE;
 BOOL b_add;
 int x,y,x0,y0, x00;

  width0 = width ;
  x00 = getx () ;
  y0 = gety ();
aa:
  width = width0 ;
  x0 = x00 ;
  y = y0 ;
  len = strlen(s);
  if (len > maxlength)
  {
    s [maxlength] = '\0';
    len = maxlength;
  }

  lpos_cur = *lpos_cur0;
  if (lpos_cur > len)
   {
    lpos_cur = len;
   }
   
  pos = lpos_cur; 
   
  fpos = 0;
  b_add = TRUE;
  if (width == 0)
  {
    b_add = FALSE;
    width = maxlength + 1;
  }
  if (maxlength >= width)
  {
    x0 += WIDTH ;
    width -= 2;
  }
  
  if (lpos_cur >= width)
   {
    lpos_cur = width - 1;
   }
   
  outtext_r_e1 (x0, y0, min (len - fpos, width /*-1*/), &s[fpos], FALSE /*TRUE*/);
  if (expand==1)
    {
    if (b_add == TRUE) put_add_char1 (x0, y0, fpos, len, width);
    }
    
  x = x0 + lpos_cur * WIDTH /*8*/;
  cursorT_On (x, y, insert);
  c = getkey();
  if (c == F2) c = ESC ;
  cursorT_Off (x, y, insert);

 do
 {
  switch(c)
  {
   case HOMEKEY :
	pos = 0;
	break;
   case ENDKEY :
	pos = len;
	break;
   case INSKEY :
   case INSKEY1:
	 cursorT_Off (x, y, insert);
	insert = !insert;
	 cursorT_On (x, y, insert);
	break;
   case LEFTKEY :
	if (pos > 0)
	 pos--;
	break;
   case RIGHTKEY :
	if (pos < len)
	 pos++;
	break;
   case BS :
	if (pos > 0)
	{
	 memmove( &s[pos - 1], &s[pos], len - pos + 1);
         pos--;
	 len--;
	}
	else
	 {
 	  c = BCSPKEY;
	 }
	break;
   case DELKEY :
	if (pos < len)
	{
	 memmove( &s[pos], &s[pos + 1], len - pos);
	 len--;
	}
	break;
   case DOWNKEY :
   case MIDDLE_BUTTON :
	if (TRUE == b_graph_value)
	{
	  break ;
	}
   case ENTER:
	break;
   case F10 :
        break;
   case F9 :
        break;     	
   case TAB :
	break;
   case SHTAB :
	break;
   case ESC :
	//len = 0;
	break;
   case ALT_A :
   case ALT_C :
   case ALT_E :
   case ALT_L :
   case ALT_N :
   case ALT_O :
   case ALT_S :
   case ALT_Z :
   case ALT_X :
   case ALT_F :
	c = pol_key (c);
   default :
	if (((legal[0] == 0) || (strchr(legal, c) != NULL)) &&
		(c >= ' ') &&  (c <= 256)    &&
		(len < maxlength))
	{
	 if (insert)
	 {
	  memmove( &s[pos + 1], &s[pos], len - pos + 1);
	  len++;
	 }
	 else if (pos >= len)
	  len++;
	 s[pos++] = c;
	}
	break;
  } /* switch */
  s[len] = 0;
  if ((c != ENTER) && (c != F10) && (c != F9) && (c != ESC) && (c != TAB) && (c !=SHTAB) &&
       (c != DOWNKEY) && (c != UPKEY) && (c != CTRLYKEY) && (c != CTRLBCSPKEY) && (c != CTRLPGUPKEY) && (c != CTRLPGDNKEY) &&
       (c != CTRLXKEY) && (c != CTRLKKEY) && (c != CTRLWKEY) &&
       (c != CTRLHOMEKEY) && (c != CTRLENDKEY) && (c != BCSPKEY) && (c != PGUPKEY) && (c != PGDNKEY) &&
	((c != DOWNKEY && c != MIDDLE_BUTTON) || FALSE == b_graph_value) )
  {
    int kor = 0;

	 if (len < width)
	 {
	   fpos = 0;
	 }
	 else
	 {
	    if (pos < fpos)
	    {
	      fpos = pos;
	    }
	    else
	    if (pos >= fpos + width)
            {
              fpos = pos - width + 1;
	    }
	    else
	    if (fpos + width - 1 > len - 1)
	    {
              fpos = len - width + 1;
              kor = 1;
            }
         }
         if (fpos < 0)
         {
          fpos = 0;
         }
         lpos_cur = pos - fpos;
	 outtext_r_e1 (x0, y0, min (len - fpos, width - kor), &s[fpos], FALSE);
	 if (expand==1)
         {
          if (b_add == TRUE) put_add_char1 (x0, y0, fpos, len, width);
         } 
	 x = x0 + lpos_cur * WIDTH /*8*/;
	 cursorT_On (x, y, insert);
	 c = getkey();
    if (c == F2) c = ESC ;
	 cursorT_Off (x, y, insert);
   }
 }
 while ((c != ENTER) && (c != F10) && (c != F9) && (c != ESC) && (c != TAB) && (c !=SHTAB) &&
        (c != DOWNKEY) && (c != UPKEY) && (c != CTRLYKEY) && (c != CTRLBCSPKEY) && (c != CTRLPGUPKEY) && (c != CTRLPGDNKEY) &&
        (c != CTRLXKEY) && (c != CTRLKKEY) && (c != CTRLWKEY) &&
        (c != CTRLHOMEKEY) && (c != CTRLENDKEY) && (c != BCSPKEY) &&  (c != PGUPKEY) && (c != PGDNKEY) &&
	 ((c != DOWNKEY && c != MIDDLE_BUTTON) || FALSE == b_graph_value) ) ;
  outtext_r_e1 (x0, y0, min (len, width), s, FALSE);
  if (expand==1)
    {
     if (b_add == TRUE) put_add_char1 (x0, y0, 0, len, width);
    } 

 if ((c == ENTER) || (c == F10) || (c == F9))
 {
   Add_String_To_List (s) ;
 }
 *lpos_cur0 = lpos_cur + fpos;
 return c;
} /* editstring1 */


int get_string_str (char *tekst, char *legal, int maxlength, int width0, char *kom_str)
/*-----------------------------------------------------------------------------------*/
{
    int zn, k;
    void (*CUR)(int ,int);
    int x, y ;
    int maxwidth;
    int width_str;
    float width;

    k=Komunikat_R;
    komunikat_str(kom_str);
    Set_Current_Pos();
    Get_Current_Pos (&x, &y) ;

    width_str=TTF_text_len(kom_str);

    moveto (x + 10, y + 2) ;

    CUR=MVCUR; MVCUR=noop;
    if (width0 == 0)
    {
        width = (float)(getmaxx() - TTF_text_len(kom_str) - ttf_width_w) / ttf_width_w;

    }
    else width = width0;

    zn = editstring(tekst, legal, maxlength, width, FALSE, 1, TRUE, 4, 3) ;

    komunikat(k);
    MVCUR=CUR;

    return zn==27 ? 0 : 1;
}

int get_string (char *tekst, char *legal, int maxlength, int width0, int kom)
/*------------------------------------------------------------------------*/
{
   int zn, k;
   void (*CUR)(int ,int);
   int x, y ;
   int maxwidth;
   int width_w;
   float width;

   k=Komunikat_R;
   komunikat(kom);
   Get_Current_Pos (&x, &y) ;
   moveto (x + 5 , y + 2) ;
   CUR=MVCUR; MVCUR=noop;
   if (width0 == 0)
   {
	   width = (float)(getmaxx() - 27*WIDTH - TTF_text_len(get_komunikat_ptr(kom)) - ttf_width_w) / ttf_width_w; // width_w;
   }
   else width = width0;

   zn = editstring(tekst, legal, maxlength, width, FALSE, 1, TRUE, 2, 4) ;
   
   komunikat(k);
   MVCUR=CUR;

   return zn==27 ? 0 : 1;
}

int get_string1 (char *tekst, char *legal, int maxlength, int width0, int kom, int x, int y)
/*---------------------------------------------------------------------------------------*/
{
   int zn, k;
   void (*CUR)(int ,int);
   float width;

   k=Komunikat_R;
   komunikat(kom);
   moveto (x, y) ;
   CUR=MVCUR; MVCUR=noop;
   width=width0;
   zn = editstring(tekst, legal, maxlength, width, FALSE,0, TRUE, 4, 3) ;
   komunikat(k);
   MVCUR=CUR;
   return zn==27 ? 0 : 1;
}

int get_string2 (char *tekst, char *legal, int maxlength, int width, int kom, int x, int y, int *lpos_cur0)
/*-------------------------------------------------------------------------------------------------------*/
{
   int zn, k;
   void (*CUR)(int ,int);
   int lpos_cur;

   k=Komunikat_R;
   komunikat(kom);
   moveto (x, y) ;
   CUR=MVCUR; MVCUR=noop;
   lpos_cur = *lpos_cur0;
   zn = editstring1(tekst, legal, maxlength, width, FALSE, 1, &lpos_cur) ;
   *lpos_cur0 = lpos_cur;
   komunikat(k);
   MVCUR=CUR;
   return zn;
}

int  read_esc1 (char  *tekst,int lmax,int kom)
/*---------------------------------------------*/
{
	float width;

	width = (float)lmax;
  return get_string (tekst, "", lmax, width, kom) ;
}


int  read_esc(char  *tekst,int lmax,int kom)
/*---------------------------------------------*/
{  int zn,k;
   void (*CUR)(int ,int);
   int x, y ;
   float width;

   k=Komunikat_R;
   komunikat (kom) ;
   Get_Current_Pos (&x, &y) ;
   moveto (x, y + 3 ) ; //2
   CUR=MVCUR; MVCUR=noop;
   width = (float)lmax;
   zn = editstring(tekst, "", lmax, width, FALSE,1, TRUE, 2, 4); //4, 3);
   komunikat(k);
   MVCUR=CUR;
   if (zn==324) return 2;
    else return zn==27 ? 0 : 1;
}


int  read_esc_legal (char *tekst, char *legal, int lmax,int kom)
/*---------------------------------------------*/
{  int zn,k;
   void (*CUR)(int ,int);
   int x, y ;
   int fwlen;
   float width;

   k=Komunikat_R;
   komunikat (kom);
   Get_Current_Pos (&x, &y) ;
   moveto (x, y + 3) ;
   CUR=MVCUR; MVCUR=noop;

   width = (float)lmax;

   zn = editstring(tekst, legal, lmax, width, FALSE,1, TRUE, 2, 4); //4, 3);
   komunikat(k);
   MVCUR=CUR;
   return zn==27 ? 0 : 1;
}


void outetextxy (int x, int y, int maxlength, int width, char *s,
					int ink, int paper)
/*-----------------------------------------------------------------*/
{
  BOOL b_add;
  int len, wlen, max_len, m_len;
  int inkk, paperk;
  int text_len_pxl;
  int width_w, width_pxl;

  if (width == 0)
  {
	  b_add = FALSE;
	  width = maxlength + 1;
  }

  len = strlen(s);

  text_len_pxl = TTF_text_len(s);
  width_pxl = width * ttf_width_w; // width_w;

  wlen = utf8len(s);
  if (text_len_pxl > 0)  width = (int)(((float)width_pxl / ((float)text_len_pxl / wlen)) /*+ 0.5*/);

  b_add = FALSE;

  if (paper != COLOR_DEF)
  {
    paperk = kolory.paperk;
    kolory.paperk = paper;
  }
  if (ink != COLOR_DEF)
  {
    inkk = kolory.inkk;
    kolory.inkk = ink;
  }
  len = strlen(s);

  if (wlen > 20)
  {
	  int a = 0;
  }

  if (wlen < width) m_len = len;
  else m_len = lenutf8to(s, width);

  m_len=findlentopxl(s, width_pxl);

  outtext_r_e (x/*+DXIL*/, y, m_len, s, FALSE, 2);  //2 is y_4_


  if (b_add == TRUE) put_add_char (x, y, 0, len, width, width, width_w, wlen, width_pxl, width_pxl, 0, 4, 4);
  if (paper != COLOR_DEF)
  {
    kolory.paperk = paperk;
  }
  if (ink != COLOR_DEF)
  {
    kolory.inkk = inkk;
  }
}


void outetextxy_s(int x, int y, int maxlength, int width, char *s,
	int ink, int paper)
	/*-----------------------------------------------------------------*/
{
	BOOL b_add;
	int len;
	int inkk, paperk;

	if (s < 255)
		return;

	b_add = FALSE;

	if (paper != COLOR_DEF)
	{
		paperk = kolory.paperk;
		kolory.paperk = paper;
	}
	if (ink != COLOR_DEF)
	{
		inkk = kolory.inkk;
		kolory.inkk = ink;
	}
	len = strlen(s);
	if (len > maxlength)
	{
		s[maxlength] = '\0';
		len = maxlength;
	}
	if (width == 0)
	{
		b_add = FALSE;
		width = maxlength + 1;
	}
	if (maxlength >= width)
	{
		x += WIDTH ;
		width -= 2;
		b_add = TRUE;
	}
	outtext_r_e(x, y, min(len, width), s, FALSE, 2);  //2 is y_4_
	int width_w = ttf_width_w;
	int wlen = utf8len(s);

	if (paper != COLOR_DEF)
	{
		kolory.paperk = paperk;
	}
	if (ink != COLOR_DEF)
	{
		kolory.inkk = inkk;
	}
}
