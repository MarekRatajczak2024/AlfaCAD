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
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#include "bib_e.h"
#include "rysuj_e.h"

#ifdef  ALLEGWIN
#define LINE myline
#define CIRCLE mycircle
#else
#define LINE line
#define CIRCLE circle
#endif


#define y_21 19
#define y_3 0

extern void setcolor(int kolor);
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);
extern void setwritemode( int mode );
extern void myline(int x1, int y1, int x2, int y2);
extern void putpixel_(int x, int y, int color);
extern void mycircle(int x, int y, int radius);
extern void outtextxy(int x, int y0, const char *textstring);
extern void gettextsettings(struct textsettingstype  *texttypeinfo);
extern void change_font(int nr_font);
extern int getx(void);
extern int gety(void);
extern int getcolor(void);
extern void outtext(const char *textstring);
extern void outtext_(BITMAP *ui_screen, const char *textstring);
extern int textwidth(const char  *textstring);
extern int my_text_length(FONT *font, const char *text);

extern BITMAP *Get_Screenplay(void);

void outtext_r(char *textstring);


/*173-127*/
static char  PL[256-127]=
  "       a      c AEel  C  S   L s ZZoOnNzz                                                                       "
  "                 " ;

static int kolor;

int text_width(char *textstring) ;

void outtext_rs(char *textstring);

int iround(double val)
{
	return (int)(val + 0.5);
}

void outtext_r(char *textstring)
/*---------------------------------------*/
{
    outtext_rs (textstring) ;
}

void outtext_rs_ (BITMAP *ui_screen, char *textstring)
/*--------------------------------------------------*/
{
  int x, y;

  if (textstring == NULL)
    return;

  if (strlen(textstring) > MaxMultitextLen)  //must be something wrong
  {
    *textstring = '\0';
    return;
  }

  x = getx();
  y = gety() - 3;
  moveto(x, y);
  outtext_(ui_screen, textstring);
}

void outtext_rs (char *textstring)
{
    outtext_rs_ (Get_Screenplay(), textstring);
}

void outtext_r_(BITMAP *ui_screen, char *textstring)
/*------------------------------------------------*/
{
    outtext_rs_ (ui_screen, textstring) ;
}

int text_width(char *textstring)
/*-------------------------------*/
{
  return textwidth(textstring);
}

