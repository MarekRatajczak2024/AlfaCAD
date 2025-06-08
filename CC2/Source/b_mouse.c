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
//#include <graphics.h>
#define ALLEGWIN
#include <allegext.h>
#ifndef LINUX
#include <dos.h>
#endif
#include <stdlib.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "b_mouse.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


extern void GrMouseUnInit(void);
extern void GrMouseEraseCursor(void);
extern void GrMouseDisplayCursor(void);
extern void GrMouseWarp(int x,int y);
extern void GrMouseGetEventT(int flags,GrMouseEvent *event,long timout_msecs);
extern void GrMouseClear(void);
extern void GrMouseGetEvent(int flags,GrMouseEvent *event);


char sz_tmp [200] ;

#define CURSIZEX 10
#define CURSIZEY 16
unsigned u__imagesize  ;
static char *ptrsz__screen ;
static char *ptrsz__scmask ;
static char *ptrsz__curmask ;
static GrMouseEvent evt;
static int evt_x, evt_y;
static char  sz__curmask [(CURSIZEX - 1) * (CURSIZEY - 1)] =
{
/*0*/	 0,0,0,0,0,0,0,0,0,
/*1*/	 0,1,0,0,0,0,0,0,0,
/*2*/	 0,1,1,0,0,0,0,0,0,
/*3*/	 0,1,1,1,0,0,0,0,0,
/*4*/	 0,1,1,1,1,0,0,0,0,
/*5*/	 0,1,1,1,1,1,0,0,0,
/*6*/	 0,1,1,1,1,1,1,0,0,
/*7*/	 0,1,1,1,1,1,1,1,0,
/*8*/	 0,1,1,1,1,1,1,1,1,
/*9*/	 0,1,1,1,1,1,0,0,0,
/*10*/   0,1,1,0,1,1,0,0,0,
/*11*/   0,1,0,0,0,1,1,0,0,
/*12*/   0,0,0,0,0,1,1,0,0,
/*13*/   0,0,0,0,0,0,1,1,0,
/*14*/   0,0,0,0,0,0,1,1,0,
///*15*/   0,0,0,0,0,0,0,0,0,
} ;

static char  sz__scmask [CURSIZEX * CURSIZEY] =
{
/*0*/	 1,1,0,0,0,0,0,0,0,0,      /*    0,0,0,0,0,0,0,0,0,0, */
/*1*/	 1,1,1,0,0,0,0,0,0,0,	   /*    0,1,0,0,0,0,0,0,0,0, */
/*2*/	 1,1,1,1,0,0,0,0,0,0,	   /*    0,1,1,0,0,0,0,0,0,0, */
/*3*/	 1,1,1,1,1,0,0,0,0,0,	   /*    0,1,1,1,0,0,0,0,0,0, */
/*4*/	 1,1,1,1,1,1,0,0,0,0,	   /*    0,1,1,1,1,0,0,0,0,0, */
/*5*/	 1,1,1,1,1,1,1,0,0,0,	   /*    0,1,1,1,1,1,0,0,0,0, */
/*6*/	 1,1,1,1,1,1,1,1,0,0,	   /*    0,1,1,1,1,1,1,0,0,0, */
/*7*/	 1,1,1,1,1,1,1,1,1,0,	   /*    0,1,1,1,1,1,1,1,0,0, */
/*8*/	 1,1,1,1,1,1,1,1,1,1,	   /*    0,1,1,1,1,1,1,1,1,0, */
/*9*/	 1,1,1,1,1,1,1,1,1,1,	   /*    0,1,1,1,1,1,0,0,0,0, */
/*10*/   1,1,1,1,1,1,1,0,0,0,	   /*    0,1,1,0,1,1,0,0,0,0, */
/*11*/   1,1,1,0,1,1,1,1,0,0,	   /*    0,1,0,0,0,1,1,0,0,0, */
/*12*/   1,1,0,0,1,1,1,1,0,0,	   /*    0,0,0,0,0,1,1,0,0,0, */
/*13*/   0,0,0,0,0,1,1,1,1,0,	   /*    0,0,0,0,0,0,1,1,0,0, */
/*14*/   0,0,0,0,0,1,1,1,1,0,	   /*    0,0,0,0,0,0,1,1,0,0, */
/*15*/   0,0,0,0,0,0,1,1,0,0,      /*    0,0,0,0,0,0,0,0,0,0, */
} ;

BOOL Ini_Mouse_Cursor(int x, int y)
{
  position_mouse(x, y);
  return TRUE;
}

void Free_Mem_Mouse_Cur (void)
/*---------------------------*/
{
  GrMouseUnInit();	      
}

void Hide_Mouse ()
/*---------------------------*/
{
  GrMouseEraseCursor();
}

void Show_Mouse (void)
/*-------------------------*/
{
  GrMouseDisplayCursor();
}

void Move_Mouse (int x, int y)
/*-------------------------*/
{
  GrMouseWarp(x,y);
}



int  Test_przycisniec0(int *keys)
{ //GrMouseEvent evt;
  /* najpierw nalezy poczekac na zwolnienie wszystkich przyciskow */
  /* standard PC MOUSE  1 - left, 2 - right, 4 - middle */
  /* standard Microsoft 1 - esc, 4 - right */
 

  GrMouseGetEventT(GR_M_BUTTON_DOWN,&evt,1L);
  if(evt.flags & GR_M_LEFT_DOWN) *keys=4;
  if(evt.flags & GR_M_MIDDLE_DOWN) *keys=2;
  if(evt.flags & GR_M_RIGHT_DOWN) *keys=1;

  return 0;
}

void Odczyt_licznikow0(int *mouse_x, int *mouse_y)
{ //GrMouseEvent evt;
  /* najpierw nalezy poczekac na zwolnienie wszystkich przyciskow */
  /* standard PC MOUSE  1 - left, 2 - right, 4 - middle */
  /* standard Microsoft 1 - esc, 4 - right */

  GrMouseGetEventT(GR_M_MOTION,&evt,1L);
  *mouse_x=evt.x-evt_x;
  *mouse_y=evt_y-evt.y;
  evt_x=evt.x;
  evt_y=evt.y;
  return;
}


void _MouseClear(void)
{  
	GrMouseClear();
}

void Odczyt_przycisniec_licznikow(int *keys, int *mouse_x, int *mouse_y)
{ GrMouseEvent evt;
  /* najpierw nalezy poczekac na zwolnienie wszystkich przyciskow */
  /* standard PC MOUSE  1 - left, 2 - right, 4 - middle */
  /* standard Microsoft 1 - esc, 4 - right */

  evt.flags=0;
  GrMouseGetEvent(GR_M_EVENT,&evt);
//  if(evt.flags & GR_M_LEFT_DOWN) *keys=4;
//  if(evt.flags & GR_M_MIDDLE_DOWN) *keys=2;
//  if(evt.flags & GR_M_RIGHT_DOWN) *keys=1;
  *mouse_x=evt.x;
  *mouse_y=evt.y;
  return;
}

void Odczyt_przycisniec_licznikow0(int *keys, int *mouse_x, int *mouse_y)
{ GrMouseEvent evt;
  /* najpierw nalezy poczekac na zwolnienie wszystkich przyciskow */
  /* standard PC MOUSE  1 - left, 2 - right, 4 - middle */
  /* standard Microsoft 1 - esc, 4 - right */

  evt.flags=0;
  GrMouseGetEventT(GR_M_BUTTON_DOWN | GR_M_MOTION, &evt, 0L);
  if(evt.flags & GR_M_LEFT_DOWN) *keys=4;
  if(evt.flags & GR_M_MIDDLE_DOWN) *keys=2;
  if(evt.flags & GR_M_RIGHT_DOWN) *keys=1;
  if(evt.flags & GR_M_MOTION)
   { *mouse_x=evt.x-evt_x;
     *mouse_y=evt_y-evt.y;
     evt_x=evt.x;
     evt_y=evt.y;
   }
    else
     { evt_x=0;
       evt_y=0;
     }
  return;
}

