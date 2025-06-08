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
#ifndef LINUX
#include <dos.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#define ALLEGWIN
#include <allegext.h>

#include "bib_e.h"
#include "b_mouse.h"
#include "rysuj_e.h"

static int forget_mouse=0;

static int mouse_x_back, mouse_y_back;

extern BOOL MacOS15;

extern int x_increase_arch;
extern int y_increase_arch;

extern void (*MSERV)(int x,int y);
extern volatile char keys;
extern void _MouseClear(void);
extern int  Test_przycisniec0(int *keys);
extern void Odczyt_licznikow0(int *mouse_x, int *mouse_y);
extern void GrMouseGetEvent1(int flags,GrMouseEvent *event);
extern void GrMouseGetKeys(GrMouseEvent* event, int PozX, int PozY);
extern int my_poll_mouse(void);
#ifdef ALLEGRO5
extern void position_mouse_(int x, int y);
extern void position_mouse_xy(int x, int y);
extern void get_mouse_cursor_position(int *ret_x, int *ret_y);
#ifdef MACOS
int MoveOSXCursor(int x, int y);
#endif
extern void move_pointer(int x, int y);
#endif
//extern void get_mouse_cursor_position(int *ret_x, int *ret_y);

extern BOOL hibernate;

extern BOOL get_sw_callback_locked(void);

extern void hide_x_cursor(void);
extern void show_x_cursor(void);
extern int Get_Global_Dialog_Flag(void);
extern void beep(long sound);
void free_mouse(void);

static int mouse_button_no = 2 ;
static BOOL locked_mouse;
static int gmaxx2, gmaxy2;
static int n_int_mouse=0;


static long mousetimestamp=0;
static long currentmousetimestamp=0;
static long breakmousetimestamp=0;
static BOOL breaktime=FALSE;
static int gesturetime=1; //2;  //2 sec
static MVECTOR mvector={0,{0},{0}};
static int K_num=3;
static int distx_free_mouse = 40, disty_free_mouse = 10;

static BOOL hardware_cursor_enabled=FALSE;


int PozX;
int PozY;

int PozX0;
int PozY0;


#define CORRECT_POSITION_MOUSE



#define MOUSE_COUNTER  500000 //2500
#define MOUSE_COUNTER2 1000

typedef struct _RECT { 
  long left; 
  long top; 
  long right; 
  long bottom; 
} RECT_, *PRECT_;


void set_sleep_state(BOOL state) {
    if (sleep_state!=state)
    {
#ifdef ALLEGRO5
#ifdef MACOS
        set_timer_speed((state ? 5 : 30));  //60  //120
#else
        set_timer_speed((state ? 5 : 60));  //60  //120
#endif
#endif
        sleep_state=state;
    }
}

void Set_Dist_Free_mouse(int distx, int disty)
{
    distx_free_mouse = distx;
    disty_free_mouse = disty;
}

void MouseClear(void)
{  
	_MouseClear();
}


void lock_mouse(void)
{  int WspX, WspY;

	locked_mouse=TRUE;
	gmaxx2 = getmaxx() / 2;
    gmaxy2 = getmaxy() / 2;

#ifdef LINUX
    hide_x_cursor();
#ifdef MACOS
    show_mouse(NULL);
#endif
#endif

    if (hardware_cursor_enabled==TRUE)
#ifndef MACOS
        disable_hardware_cursor();
#endif
    hardware_cursor_enabled=FALSE;

	scare_mouse();

    ini_cursor=FALSE;
}



void free_mouse(void)
{
	locked_mouse=FALSE;
#ifndef ALLEGRO5
#ifndef MACOS
	enable_hardware_cursor();
#endif
#endif
    hardware_cursor_enabled=TRUE;

#ifdef LINUX
    show_x_cursor();
#ifdef MACOS
    show_mouse(screen);
#endif
#endif

    ini_cursor = TRUE;

}

void _free_mouse(void)
{

	locked_mouse=FALSE;
#ifndef ALLEGRO5
#ifndef MACOS
	enable_hardware_cursor();
#endif
#endif
    hardware_cursor_enabled=TRUE;

#ifdef LINUX
    show_x_cursor();
#endif
    ////unscare_mouse();

    Show_Mouse() ;
    ini_cursor = TRUE;
}


int  Test_przycisniec1(int *keys)
{
  return Test_przycisniec0(keys);
}

void  Odczyt_licznikow1(void)
{ int mouse_x, mouse_y;

 Odczyt_licznikow0(&mouse_x, &mouse_y);
 MSERV(mouse_x,mouse_y);
 return;
}

void  Set_Mouse_Buttons_No (int mb_no)
/*--------------------------------------*/
{
  mouse_button_no = mb_no ;
}

/*________________________________________________________________________*/

void set_forget_mouse(int exp_x, int exp_y)
{  int WspX_, WspY_;
    int mouse_cur_x, mouse_cur_y;
    //forget_mouse=1;
    //do {
    //get_mouse_mickeys(&WspX_, &WspY_);
    //} while ((WspX_!=0) || (WspY_!=0));
    //forget_mouse=0;
    //return;
//#ifndef MACOS
    //forget_mouse=1;
    //return;
//#endif
    //sleep(0);
    int n=0;

    /*
    get_mouse_cursor_position(&mouse_cur_x, &mouse_cur_y);
    //while ((mouse_cur_x!=exp_x) && (mouse_cur_y!=exp_y) && (n<100))
    while ((mouse_cur_x!=exp_x) && (mouse_cur_y!=exp_y) && (n<500))
    {
        printf("X  >>> mouse_cur_x=%d  exp_x=%d   mouse_cur_y=%d  exp_y=%d\n", mouse_cur_x, exp_x, mouse_cur_y, exp_y);
        //get_mouse_mickeys(&WspX, &WspY);
        position_mouse(exp_x, exp_y);
        my_sleep(0);
        get_mouse_cursor_position(&mouse_cur_x, &mouse_cur_y);
        n++;
    }
     */
    forget_mouse=1;
    /*
    if (n==5)
    {
        int a=1;
    }
     */
    return;


    if ((mouse_x==exp_x) && (mouse_y==exp_y))
        forget_mouse=1;
    else if ((mouse_x!=exp_x) || (mouse_y!=exp_y))
    {
        mouse_x_back=mouse_x;
        mouse_y_back=mouse_y;
        printf(">>> mouse_x=%d  exp_x=%d   mouse_y=%d  exp_y=%d\n", mouse_x, exp_x, mouse_y, exp_y);
        forget_mouse=2;
    }
    else forget_mouse=1;
}

void  Test_przycisniec(int *keys)
{ static GrMouseEvent event;

  GrMouseGetKeys(&event, PozX, PozY);
  *keys=event.flags;
}

void  Odczyt_licznikow_factory(BOOL trig) {
    int WspX, WspY, WspX1, WspY1, WspX_, WspY_;
    int d_mouse_x;
    int d_mouse_y;
    int top = MAXINT, bottom = -MAXINT, left = MAXINT, right = -MAXINT, sum, i;
    int dlx, dly, per;
    int midx, midy;
    int mv_max;
    int ret;
    int mouse_cur_x, mouse_cur_y;

    if (hibernate) return;

    get_mouse_mickeys(&WspX, &WspY);
#ifdef ALLEGRO5
    get_mouse_cursor_position(&mouse_cur_x, &mouse_cur_y);
#else
    mouse_cur_x=mouse_x;
    mouse_cur_y=mouse_y;
#endif
    if (forget_mouse>0)
    {
        if (forget_mouse==2)
            {
            int n=0;
            while ((mouse_cur_x==mouse_x_back) && (mouse_cur_y==mouse_y_back) && (mouse_b==0) && (n<10))
            {
               //// printf("W");
#ifdef LINUX
                sleep(0);
#else
                _sleep(0);
#endif
                n++;
            }
           //// printf("<<< mouse_x=%d  mouse_y=%d\n", mouse_x, mouse_y);
        }
        forget_mouse=0;
       //// printf("FORGOTEN\n");

        return;
    }

    WspX1 = WspX;
    WspY1 = -WspY;

    MSERV(WspX1, WspY1);

    d_mouse_x = abs(mouse_cur_x - gmaxx2);
    d_mouse_y = abs(mouse_cur_y - gmaxy2);

    ////if ((WspX!=0) || (WspY!=0))
    ////    printf("d_mouse_x=%d  d_mouse_y=%d\n", d_mouse_x, d_mouse_y);

    ///// shaking
#ifndef ALLEGRO5
    if (!Get_Global_Dialog_Flag())
    {

        currentmousetimestamp = time(NULL);
        if ((!breaktime) || (currentmousetimestamp > breakmousetimestamp)) {
            breaktime = FALSE;
            if ((WspX != 0) || (WspY != 0)) {
                //continuouing

                mvector.x[mvector.no] = WspX1;
                mvector.y[mvector.no] = WspY1;

                midx = 0;
                midy = 0;
                top = MAXINT, bottom = -MAXINT, left = MAXINT, right = -MAXINT;
                mv_max = 0;
                for (i = 0; i < MV_MAX; i++) {

                    if ((mvector.x[i] != 0) && (mvector.y[i] != 0)) {

                        if (mvector.x[i] < left) left = mvector.x[i];
                        if (mvector.y[i] < top) top = mvector.y[i];
                        if (mvector.x[i] > right) right = mvector.x[i];
                        if (mvector.y[i] > bottom) bottom = mvector.y[i];

                        mv_max++;
                    }
                }
                if (mv_max > (MV_MAX / 2)) {

                    //printf("(%d) %d   %d   %d | %d   %d   %d\n", mv_max, left, midx, right, top, midy, bottom);

                    if (((right - midx > distx_free_mouse) && (midx - left > distx_free_mouse)) &&
                        ((bottom - midy < disty_free_mouse) && (midy - top < disty_free_mouse))) {
                        //beep(0x00000000L);
                        breaktime = TRUE;
                        breakmousetimestamp = time(NULL) + 2;
                        mvector.no = 0;
                        memset(mvector.x, 0, sizeof(mvector.x));
                        memset(mvector.y, 0, sizeof(mvector.y));
                        simulate_keypress(14592);
                    }
                }
                mvector.no++;
                if (mvector.no == MV_MAX) {
                    mvector.no = 0;
                    //printf("*\n");
                }

                mousetimestamp = currentmousetimestamp;
            } else if (currentmousetimestamp > (mousetimestamp + gesturetime)) {
                //resetting
                mvector.no = 0;
                memset(mvector.x, 0, sizeof(mvector.x));
                memset(mvector.y, 0, sizeof(mvector.y));
                mousetimestamp = currentmousetimestamp;
            }
        }
    }
#endif
    /////
	  if ((mouse_out == FALSE) && (sleep_state == TRUE) && ((abs(WspX) > 0) || (abs(WspY) > 0)))  //20
	  {
		  set_sleep_state(FALSE);
	  }

#ifndef LINUX
	  if ((locked_mouse) && ((d_mouse_x > gmaxx2 / 3) || (d_mouse_y > gmaxy2 / 3)))  //  /2
	  {
		  n_int_mouse = 0;
#ifndef CORRECT_POSITION_MOUSE
		  position_mouse(gmaxx2, gmaxy2);
		  get_mouse_mickeys(&WspX, &WspY);
#else
		  position_mouse(gmaxx2, gmaxy2);
#endif
	  }
#else
#ifdef ALLEGRO5
    if ((trig) && (locked_mouse) && ((d_mouse_x > gmaxx2 / 3) || (d_mouse_y > gmaxy2 / 3)))  //  /2
    {
        n_int_mouse = 0;
       //// printf("d_mouse_x=%d  gmaxx2/3=%d   d_mouse_y=%d  gmaxy2/3=%d\n", d_mouse_x, gmaxx2 / 3, d_mouse_y, gmaxy2 / 3);
#ifndef CORRECT_POSITION_MOUSE
        position_mouse(gmaxx2, gmaxy2);
		  get_mouse_mickeys(&WspX, &WspY);
#else
#ifdef MACOS
        //printf(">>> mouse_x=%d   mouse_y=%d\n",mouse_x,mouse_y);
        ret=MoveOSXCursor(gmaxx2, gmaxy2);
#else
        move_pointer(gmaxx2, gmaxy2);
        sleep(0);
#endif
        set_forget_mouse(gmaxx2, gmaxy2);
#endif
    }
#endif
#endif
}

void Odczyt_licznikow(void)
{
    Odczyt_licznikow_factory(TRUE);
}

void Odczyt_licznikow_simple(void)
{
    Odczyt_licznikow_factory(FALSE);
}

void  Odczyt_licznikow_resize(long *dx, long *dy)
{ int WspX,WspY,WspX1,WspY1;
  long msxL, msyL;
  int d_mouse_x, d_mouse_y;

  int p_mouse = poll_mouse();
  get_mouse_mickeys(&WspX, &WspY);
  if (WspX>32767) msxL=(WspX-65535); else msxL=WspX;
  if (WspY>32767) msyL=-(WspY-65535); else msyL=-WspY;
  WspX1=msxL;
  WspY1=msyL;


  d_mouse_x=abs(mouse_x-gmaxx2);
  d_mouse_y=abs(mouse_y-gmaxy2);

  if ((locked_mouse) && ((d_mouse_x>gmaxx2/2) || (d_mouse_y>gmaxy2/2)))
  {
	  n_int_mouse=0;
#ifndef CORRECT_POSITION_MOUSE
	  //odczytanie licznikow
      get_mouse_mickeys(&WspX, &WspY);
	  position_mouse(gmaxx2, gmaxy2);
	  //odczytanie licznikow
      get_mouse_mickeys(&WspX, &WspY);
#else
	 position_mouse(gmaxx2, gmaxy2);
#endif
  }

  *dx=WspX1;
  *dy=WspY1;

}

void  Set_Mouse_Buttons_No1 (int mb_no)
/*--------------------------------------*/
{
  mouse_button_no = mb_no ;
}

void  deinstal_ms(void)
{
 remove_mouse();
}



