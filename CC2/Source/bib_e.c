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

#define __BIB_E__
#pragma warning(disable : 4996)
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
#ifndef LINUX
#include<dos.h>
#ifdef _CMAKE_
#include <synchapi.h> //needed for CLion
#endif
#else
#include <unistd.h>
#endif
#include<stdio.h>
#ifndef LINUX
#include<process.h>
#endif
#include<errno.h>
#include<math.h>
#include<signal.h>
#ifndef LINUX
#include<io.h>
#endif
#include<fcntl.h>

#include "bib_e.h"
#include "alf_res.h"
#include "rysuj_e.h"
#include "bib_edit.h"
#include "b_libfun.h"
#include "o_libfun.h"
#include "b_mouse.h"
#include "o_addmnu.h"
#include "o_debug.h"
#include "o_lttype.h"
#include "o_protec.h"
#include "o_graphf.h"
#include "o_loadf.h"
#include "o_spec.h"
#include "b_bibe.h"
#include "o_dialog.h"

#include "alffont.h"


#include "bgiext.h"

#include "menu.h"

#include "o_prn.h"

#include "leak_detector_c.h"

#define F32 8

#define MaxNpts 92 //128 //max number of points for bspline

#define PCX_RANGE

#define FADE_OUT myfade_out


#define MY_DOT_FILL  HATCH_FILL //LTSLASH_FILL //HATCH_FILL  //WIDE_DOT_FILL
#define MY_DOT_FILL1 WIDE_DOT_FILL

#define MAX_MAX_N_BLOCKS  1000000 //256000 //128000 //64000  //32000
#define PREC_DOUBLE  1.0E-10
#define r27 27
#define max_licznik_obiektow 10

#define SizeObKon sizeof(NAGLOWEK)
#define PKu130x 210
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

#define LINE myline
//#define GO_TRIANGULATION

#define w1_5 1.5
#define kat0 10

#ifndef isnan
#define isnan(a) (a != a)
#endif

double thermal_axis_size=5; //mm

double node_size=0; //mm TEMPORARY

float BRIGHTNESS_DIFF=80; //72.0; //64.0;

BOOL hatch_cursor = FALSE;

static int x0_bak=-1, y0_bak=-1;
static int KursorS_arch = 0;
BOOL reset_pointer = FALSE;

BITMAP* cursor_pattern = NULL;
BOOL cur_xor = FALSE;

double view_vector_scale=1.0;

#ifdef LINUX

long filelength_l(char *filename);
long filelength(int f);
#endif

void Destroy_All_ctx(void);
static void ramka(int opt);

int DRAWING_NUMBER=0;  //global

int pYk_arch = 0;
int pXk_arch = 9999;

int prev_cur_size = 0;

static int pattern_offset = 0;
static BOOL pattern_count = FALSE;

int MAX_N_BLOCKS;

BOOL no_break = FALSE;

typedef unsigned int GrColor;

#define DEFAULT_SPRITE_W   16
#define DEFAULT_SPRITE_H   16

int BIGCURSOR = 0;
extern BOOL BAR_POINTER;

BOOL enforce_redraw = FALSE;

extern BOOL glb_silent;


extern void rysuj_punkt_ (T_Point *ad,int mode,int kolor);

extern char* punits[];
extern char *upunits[];
extern int(*SW2[13])(void);
extern char readmouse(void);
extern void _free_mouse(void);

extern int get_komunikat_arc_bak(void);
extern BOOL kom_arc_bak(void);

#ifdef ALLEGRO5
extern void position_mouse_(int x, int y);
extern void set_mouse_range(int x1, int y1, int x2, int y2);
extern void set_mouse_speed(int xspeed, int yspeed);
#endif
//extern int get_cursor_info(void);
extern int get_v32(void);

void Scale_Point (double k1, double k2, double x1,double y1,double x2,double y2,double *x,double *y);

const int funits_no=8;
double funits[8] = { 1.0, 10.0, 1000.0, 1000000.0, 25.4, 304.8, 914.4, 1609344};

void Copy_cursor_pattern_bitmap(SOLID_PATTERN_LIBRARY_ITEM* solid_pattern_library_item);
void my_sleep(int sleepMs);

void hide_x_cursor(void);
void show_x_cursor(void);

#ifdef LINUX
#define DEFAULT_SPRITE_NULL_W   16
#define DEFAULT_SPRITE_NULL_H   16

char alfa_mouse_data_null[DEFAULT_SPRITE_NULL_H * DEFAULT_SPRITE_NULL_W] =
        {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

        };
#endif

char alfa_mouse_arrow_data[DEFAULT_SPRITE_H * DEFAULT_SPRITE_W] =
{
   2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0,
   2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0,
   2, 1, 1, 1, 1, 1, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0,
   2, 1, 1, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 1, 2, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 2, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0
};

char alfa_mouse_edit_data[DEFAULT_SPRITE_H * DEFAULT_SPRITE_W] =
{
    0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 2, 2, 1, 2, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 2, 2, 1, 2, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0
};

#define DEFAULT_SPRITE_W32   32
#define DEFAULT_SPRITE_H32   32

char alfa_mouse_arrow_data32[DEFAULT_SPRITE_H32 * DEFAULT_SPRITE_W32] =
{
   0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 2, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   2, 2, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 2, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 2, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   2, 2, 1, 1, 2, 2, 0, 2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   2, 2, 1, 2, 2, 2, 0, 0, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 2, 2, 0, 0, 0, 0, 0, 2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 1, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

char alfa_mouse_edit_data32[DEFAULT_SPRITE_H32 * DEFAULT_SPRITE_W32] =
        {
                0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        };


static GrContext *second_screen;
static GrContext *third_screen;
GrContext *second_screen_back;
ZBUFFER *zbuf;
extern void Write_screen_pcx(void);
extern void getcolor_RGB(int *red, int *green, int *blue, int color);
extern void orto_l(LINIA *L, int *Orto_Dir);
extern void setlinestyle(int linestyle, unsigned upattern, int thickness);
extern int Get_Current_Color (void);
extern void setwritemode( int mode );
extern void setfillstyle_(int pattern, int color);
extern void setviewport(int left, int top, int right, int bottom, int clip);
extern void setviewport_second(BITMAP *second_screen,int left, int top, int right, int bottom, int clip);
extern void setcolor(int kolor);
extern void myline(int x1, int y1, int x2, int y2);
extern void mycircle(int x, int y, int radius);
extern int linestyle_xor(int typ);
extern void putpixel_(int x, int y, int color);
extern void rectangle(int left, int top, int right, int bottom);
extern int getcolor(void);
extern void setrgb2cmyk(int *color_c, int *color_m, int *color_y, int i);
extern int get_palette_color(int color);
extern void setfillpattern_gregre();
extern void _normal_rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
extern void setfillstyle(int pattern, int color);
extern void Get_Limits_PCX (long_long off, long_long offk, int atrybut, double *xmin, double *xmax, double *ymin, double *ymax);
extern unsigned long_long Get_imageBMPsize(void);
extern int Bytes_Per_Pixel(int color_depth);
extern void InfoListStr1(int n, char *st);
extern void Set_Screenplay(BITMAP *ctx_bitmap);
extern void GrClearContext(GrColor bg);
extern void obiekt_tok_all (char  *adr, char  *adrk, char  **ad, int obiekt);
extern int my_kbhit(void);
extern int Draw_pcx_virtual(B_PCX *pcx, int opcje, int ctx_x0, int ctx_y0, int ctx_ww, int ctx_wh);
extern int Draw_pcx(B_PCX *pcx);
extern void GrSetContext(const GrContext *context);
extern long int Wyslij_par_ekranu(int x);
extern void my_stretch_blit(BITMAP *ctx_bitmap,int mmx0,int mmy0,int dxx0, int dyy0,int mmx1,int mmy1,int dxx,int dyy);
extern int get_WhNumberTextStyle(void);
extern void outtextxy_w_v (TEXT *t, int mode);
extern void outtextxy_w_0 (TEXT *t);
extern void set_mode_trans();
extern void set_mode_solid();
extern void my_blit_reverse(BITMAP *ctx_bitmap,int mmx0,int mmy0,int mmx1,int mmy1,int dxx,int dyy);
extern void my_blit(BITMAP *ctx_bitmap,int mmx0,int mmy0,int mmx1,int mmy1,int dxx,int dyy);
extern void my_blit_s(BITMAP *ctx_bitmap,int mmx0,int mmy0,int mmx1,int mmy1,int dxx,int dyy);
#ifdef ALLEGRO5
void my_blit_sd(BITMAP *src_bitmap, BITMAP *dst_bitmap, int mmx0,int mmy0,int mmx1,int mmy1,int dxx,int dyy);
#endif
extern void draw_tools(void);
extern void closegraph(void);
extern int getmaxcolor(void);
extern void setaspectratio( int xasp, int yasp );
extern void set_BGI_mode_whc(int *gd, int *gm, int width, int height, int colors);
extern void setrgbdefaults(void);
extern void setbkcolor(int color);
extern int demo(int Alf_window_number);
extern int ReadTextStyleDXF (void);
extern BOOL get_config_profil (void);
extern int demo_out(int Alf_window_number);
extern void InitImages(void);
extern void setrgbpalette(int color, int red, int green, int blue);
extern void myfade_out(int speed);
extern void calc_bspline(int lp, int n, float points[], int p1, float *out_x, float *out_y);

extern void SkalaZC(void);

extern TDIALOG printer_dlg;

extern int TestRedraw(void);

extern int JumpL_(void);
extern BOOL preview_blocked;

extern void skala_menup_(void);

extern void set_last_window(void);
extern void set_original_window(void);

extern int	global_mode;
extern BOOL hatching_in_progress;

extern char *icon_pointerb_p;
extern void putimage_preserve(int left, int top, BITMAP  *bitmap, int op);
extern void getimage(int left, int top, int right, int bottom, BITMAP  *bitmap);
extern int ask_question(int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien, int image);
extern BOOL Get_Bitmap_Outline(void);
extern void rysuj_bitmap_outline(char *ad, int mode, BOOL enforce, int kolor0, int kolor, int gr);
extern void draw_keyimages(void);
extern void skala_format(int dark);
extern int my_poll_keyboard(void);
extern void Save_View_Preview(void);
extern BOOL get_config_sectors(void);
extern void save_dialog_cursor(void);
extern void save_menu_cursor(void);
extern void convert_and_deposit_bitmap(BITMAP *PREVIEW, int x1, int y1, int x2, int y2, char *drawing_file, int client_number);
extern void reset_special_background(char *file_pcx);
extern BITMAP* load_memory_pcx(AL_CONST void* buffer, PALETTE* pal);
extern double get_skala(void);
extern void DonePatternBitmapScaled(void);
extern void putimage(int left, int top, BITMAP* bitmap, int op);
extern void fillpoly(int numpoints, AL_CONST int* polypoints);
extern void rysuj_spline_(SPLINE* ad, float *ad_xy, int mode, int kolor, BOOL count_mvcurb, BOOL partial, BOOL moving);
extern int GET_CONTROL_KEYS(int key);
extern void ClearInfo0(void);
extern int setfillpattern_gregre_scaled(char* s_pattern, BOOL to_printer, double resolution, long origin_x, long origin_y, int* origin_x_prn, int* origin_y_prn);
extern void my_fillpoly(int numpoints, int* polypoints, int translucency, int kolory_paper);
extern void reset_trans_blender(void);
extern void  Odczyt_licznikow(void);

extern float get_demo_scale(void);
extern void set_pattern_add_limit(long_long n);

extern void  getimage2(int left, int top, int right, int bottom, BITMAP  *bitmap);
extern BOOL dragging_quad_move;

extern void komunikat_str_short_reminder(void);
extern BOOL get_short_notice(void);

extern double radius_magnitude; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double depth_magnitude; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double thermal_magnitude; //units per mm  default 1 Celsius per 1 mm on drawing paper
extern double load_magnitude; //units per mm  default 10kN/m force per 1 mm on drawing paper
extern double flood_magnitude; //units per mm  default 10kN/m² load per 1 mm on drawing paper
extern double shear_magnitude; //units per mm  default 10kN/m shear/reactions per 1 mm on drawing paper
extern double force_magnitude; //units per mm  default 10kN force per 1 mm on drawing paper
extern double moment_magnitude; //units per mm  default 10kNm force per 1 mm radius on drawing paper
extern double displacement_magnitude; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
extern double rotation_magnitude;
extern double Get_TextLen (TEXT *ptrs_text, char *ptrsz_t);

extern double measure_line (LINIA *ptrs_line,
                            BOOL b_first_end,
                            double df_l0, double df_dl,
                            double *df_x, double *df_y);

extern BOOL linia_visible(LINIA *L);
extern BOOL okrag_visible (OKRAG *ad);
extern BOOL wielokat_visible(WIELOKAT *w);

extern double round_to_fraction(double dimbase, double fraction);
extern int cursor_color0;
extern void get_solidarc_ends(SOLIDARC *sa, double *xy);

extern int isometric_vector_to_cartesian(double dx_iso, double dy_iso, double *dx_cart, double *dy_cart);
//extern int isometric_gradient_to_cartesian(double dx_iso, double dy_iso, double *dx_cart, double *dy_cart);
extern double isometric_angle_to_cartesian_angle(double iso_angle_deg);
extern double isometric_angle_to_cartesian_angle_rad(double iso_angle_rad);
extern int isometric_polar_to_cartesian(double x1, double y1, double length, double angle_deg, double *x2, double *y2);
extern double isometric_vector_length_f(float x1, float y1, float x2, float y2);
extern double isometric_vector_angle(double x1, double y1, double x2, double y2);
extern int circle_to_isometric_ellipse(enum PlaneType plane, double x, double y, double r, double *xe, double *ye, double *rx, double *ry, double *a);
extern int circle_to_isometric_ellipse_o_e(enum PlaneType plane, OKRAG *o, ELLIPSE *e);
extern int arc_to_isometric_ellipticalarc_a_ea(enum PlaneType plane, LUK *l, ELLIPTICALARC *ea);
//extern int Get_EllipticalArc_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double kat1, double kat2, double xy[]);
extern void Get_EllipticalArc_EndPoints (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle,double df_sangle0,double df_eangle0,double *x1, double *y1, double *x2, double *y2);
extern int cartesian_vector_to_isometric(double dx_cart, double dy_cart, double *dx_iso, double *dy_iso);
extern void srodekea_(double *x,double *y, double *tangent, void *adr);

static char *format_float="%.9g";
static char *format_floatd="%.9g";

double thermal_precision=0.1;
double force_precision=0.01;
double moment_precision=0.01;
double stress_precision=0.01;
double displacement_precision=0.01;
double rotation_precision=0.0001;
double load_precision=0.01;
extern double dim_precision;

double thermal_precision0=0.1;
double force_precision0=0.01;
double moment_precision0=0.01;
double stress_precision0=0.01;
double displacement_precision0=0.01;
double rotation_precision0=0.0001;
double load_precision0=0.01;

int lineC(long x1, long y1, long x2, long y2);
int lineCuncut(long x1, long y1, long x2, long y2);

int my_getch(void);

static double krok_old;
static BOOL snap_changed=FALSE;
static BOOL redraw_again=FALSE;

static BITMAP *ctx_bitmap[32]={NULL};

int key_buffer=-1;

struct al_ffblk   _al_ffblk;

int(*serv134)(void) = { NULL };
int(*serv135)(void) = { NULL };

static BITMAP *alfa_mouse_null = NULL;
static BITMAP *alfa_mouse_pointer = NULL;/* default mouse pointer */
static BITMAP *alfa_mouse_pointer32 = NULL;/* default mouse pointer */
static BITMAP *alfa_mouse_pointer48 = NULL;/* default mouse pointer */

static BITMAP *alfa_mouse_edit = NULL;
static BITMAP *alfa_mouse_edit32 = NULL;
static BITMAP *alfa_mouse_edit48 = NULL;

static BITMAP *alfa_mouse_sprite = NULL;
static BITMAP *alfa_mouse_edit_sprite = NULL;
static BITMAP *alfa_mouse_busy_sprite = NULL;

extern unsigned long_long imagesizelong(int left, int top, int right, int bottom);

extern BOOL TTF_redraw;
extern int TTF_text_len(char *text);

extern char get_shifts(int key2);

extern float solidpatternscales[7];
extern int solidpatternangles[4];
extern int fixangles[4];
extern void record_keyimages(void);
extern void remove_keyimages(BOOL silent);
extern void restore_keyimages(void);

extern long global_pan_dx, global_pan_dy;
extern int TRANSLUCENCY;
extern int HALFTRANSLUCENCY;

extern int get_points_hatch_outline(char* adr, char* adrk, float* hatch_points, int* vertex_counts, int* h_layer, int* h_kolor);

extern void set_sketch_offset(int patt_o);
extern int get_sketch_offset(void);

extern void set_cur_color(int color);
extern int get_cur_color(void);

extern int Expand_flex();

extern void set_priv_prn_window(double Xmin_, double Ymin_, double Xmax_, double Ymax_);
extern GrContext* get_second_screen_bak_(void);
extern BOOL get_DEMO_RECORDING(void);
extern void draw_keyimage(int keyimage, BOOL set_timer, BOOL silent, BOOL enforce);
extern void draw_demokeyimage(int keyimage, int keyimage1, BOOL set_timer, BOOL silent, BOOL enforce);

extern void outlinetext(TEXT *t, QUAD *outline, double margin);
extern void winf_addf(void);
extern int get_lp(void);
extern void redraw_str(int no_);

extern int get_3p_arc(LUK *l, POINTD *p1, POINTD *p2, POINTD *p3);
extern int get_arc_points(LUK *l, double **xy, int n0, BOOL reversed, BOOL printer);
extern void rysuj_solidarc_(SOLIDARC *ad,int mode,int kolor, BOOL as_redraw, BOOL enforce, BOOL begin_line, BOOL end_line);
extern int solidarc_wybrany(SOLIDARC *sa);
extern BOOL Point_in_SolidArc (SOLIDARC *ptr_sa, double x, double y);

extern BITMAP *icon_hourglass_mem;
extern int vfv(int v);

int solid_pattern_library_no = 0;
#define SOLID_PATTERN_LIBRARY_MAX_ITEMS 512 //1024
char* Solid_Pattern_Library[SOLID_PATTERN_LIBRARY_MAX_ITEMS];
BITMAP* cursor_pattern_bitmap0 = NULL;
BITMAP* cursor_pattern_bitmap = NULL;
BITMAP* cursor_fill_bitmap = NULL;
double copy_cursor_pattern_fx = 0.0;
int copy_cursor_fill_color = -1;
int copy_SolidHatchTranslucency = -1;
int copy_kolory_paper = -1;
double current_cursor_fx = 0.0;
int cursor_icon_size = 128; // 64;

//---------
int scancode0[121]={1,30,48,46,32,18,33,34,35,23,36,   // 0-10
                      37,38,50,49,24,25,16,19,31,20,   // 11-20
                      22,47,17,45,21,44,129,120,121,122,   // 21-30
                      123,124,125,126,127,128,1,79,80,81,   // 31-40
                      75,1,77,71,72,73,59,60,61,62,   // 41-50
                      63,64,65,66,67,68,69,133,1,1,   // 51-60
                      130,131,1,1,1,1,1,1,1,1,   // 61-70
                      1,1,1,1,1,82,83,71,79,73,   // 71-80
                      81,75,77,72,80,1,1,1,1,1,   // 81-90
                      1,1,1,1,1,1,1,1,1,1,   // 91-100
                      1,1,1,1,1,1,1,1,1,1,   // 101-110
                      1,1,1,1,1,1,1,1,1,1};   // 111-120

//Shift
int scancode1[121]={1,1,1,1,1,1,1,1,1,1,1,   // 0-10
                      1,1,1,1,1,1,1,1,1,1,   // 11-20
                      1,1,1,1,1,1,1,1,1,1,   // 21-30
                      1,1,1,1,1,1,1,1,1,1,   // 31-40
                      1,1,1,1,1,1,84,85,86,87,   // 41-50
                      88,89,90,91,92,93,69,133,1,1,   // 51-60
                      1,1,1,1,1,1,1,1,1,1,   // 61-70
                      1,1,1,1,1,1,1,1,1,1,   // 71-80
                      1,115,116,117,118,1,1,1,1,1,   // 81-90    //!!!!!!!!!!!!!!!!!!  RESIZE
                      1,1,1,1,1,1,1,1,1,1,   // 91-100
                      1,1,1,1,1,1,1,1,1,1,   // 101-110
                      1,1,1,1,1,1,1,1,1,1};   // 111-120

//Ctrl
int scancode2[121]={0,1,2,3,4,5,6,7,8,9,10,   // 0-10
                      11,12,13,14,15,16,17,18,19,20,   // 11-20
                      21,22,23,24,25,26,1,1,1,1,   // 21-30
                      1,1,1,1,1,1,1,1,1,1/*118*/,   // 31-40    //!!!!!!!!!!!!!!!!!!   RESIZE
                      1,1,1,1,1,1,94,95,96,97,   // 41-50
                      98,99,100,101,102,103,136/*69*/,137/*133*/,1,1,   // 51-60
                      1,1,1,1,1,1,1,1,1,1,   // 61-70
                      1,1,1,1,1,116,1,119,117,118,   // 71-80
                      115,141,142,143,144,1,1,1,1,1,   // 81-90   //!!!!!!!!!!!!! RESIZE
                      1,1,1,1,1,1,1,1,1,1,   // 91-100
                      1,1,1,1,1,1,1,1,1,1,   // 101-110
                      1,1,1,1,1,1,1,1,1,1};   // 111-120

//Ctrl-Shift
int scancode3[121]={0,1,1,1,1,1,1,1,1,1,1,   // 0-10
                      1,1,1,1,1,1,1,1,1,1,   // 11-20
                      1,1,1,1,1,1,1,1,1,1,   // 21-30
                      1,1,1,1,1,1,1,1,1,1,   // 31-40
                      1,1,1,1,1,1,1,1,1,1,   // 41-50
                      1,1,1,1,1,1,140,133,1,1,   // 51-60
                      1,1,1,1,1,1,1,1,1,1,   // 61-70
                      1,1,1,1,1,1,1,1,1,1,   // 71-80
                      1,1,1,1,1,1,1,1,1,1,   // 81-90
                      1,1,1,1,1,1,1,1,1,1,   // 91-100
                      1,1,1,1,1,1,1,1,1,1,   // 101-110
                      1,1,1,1,1,1,1,1,1,1};   // 111-120

//Alt
int scancode4[256]={1,1,225,1,185,1,232,226,224,243,241,   // 0-10
                      242,1,230,1,1,227,253,252,1,231,   // 11-20
                      137,251,246,1,247,1,129,120,121,122,   // 21-30
                      123,124,125,126,127,128,1,1,1,1,   // 31-40
                      0xB9,1,1,1,1,1,104,105,106,107,   // 41-50    ...Alt-F4
                      108,109,110,111,112,113,114,133,1,1,   // 51-60  Alt-F5...
                      130,131,1,1,1,1,1,1,1,1,   // 61-70
                      1,1,1,1,1,1,1,1,1,1,   // 71-80
                      1,1,1,1,1,1,1,130,131,1,   // 81-90
                      1,1,1,1,1,1,1,1,1,1,   // 91-100
                      1,1,1,1,1,1,1,1,1,1,   // 101-110
                      1,1,1,1,1,1,1,1,1,1,   // 111-120
					  1,1,1,1,1,1,1,1,   // 121-128
					  1,1,1,1,1,1,1,1,  //129-136
					  1,1,1,1,1,1,1,1,  //137-144
					  1,1,1,1,1,1,1,1,  //145-152
					  1,1,1,1,1,1,1,1,  //153-160
					  1,1,1,1,1,1,1,1,  //161-168
					  1,1,1,1,1,1,1,1,  //169-176
					  1,1,1,1,1,1,1,1,  //177-184
					  1,1,1,1,1,1,1,1,  //185-192
					  1,1,1,1,1,1,1,1,  //193-200
					  1,1,1,1,1,1,1,1,  //201-208
					  1,1,1,1,1,1,1,1,  //209-216
					  1,1,1,1,1,1,1,1,  //217-224
					  1,1,1,1,1,1,1,1,  //225-232
					  1,1,1,1,1,1,1,1,  //233-240
					  1,1,1,1,1,1,1,1,  //241-248
					  1,1,1,1,1,1,1};  //249-255

//Alt-Shift
int scancode5[121]={1,1,1,1,1,1,1,1,1,1,1,   // 0-10
                      1,1,1,1,1,1,1,1,1,1,   // 11-20
					  1,1,1,1,1,1,129,120,121,122,   // 21-30
					  123,124,125,126,127,128,1,1,1,1,   // 31-40
                      1,1,1,1,1,1,1,1,1,1,   // 41-50
                      1,1,1,1,1,1,69,133,1,1,   // 51-60
                      1,1,1,1,1,1,1,1,1,1,   // 61-70
                      1,1,1,1,1,1,1,1,1,1,   // 71-80
                      1,1,1,1,1,1,1,1,1,1,   // 81-90
                      1,1,1,1,1,1,1,1,1,1,   // 91-100
                      1,1,1,1,1,1,1,1,1,1,   // 101-110
                      1,1,1,1,1,1,1,1,1,1};   // 111-120

//AltGR-Letter Duze litery
int scancode6[256]={ 1,0x8F,177,0x95,187,0x90,192,180,186,179,184,   // 0-10
                      181,0x9C,189,0xA5,0xA3,178,185,248,0x98,183,   // 11-20
                      191,188,182,0xA0,190,0xA1,129,120,121,122,   // 21-30
                      123,124,125,126,127,128,1,1,1,1,   // 31-40
                      0xB9,1,1,1,1,1,104,105,106,107,   // 41-50
                      108,109,110,111,112,113,114,133,1,1,   // 51-60
                      1,1,1,1,0x8F,225,0x95,185,0x90,232,   // 61-70
                      226,224,243,241,242,0x9C,230,0xA5,0xA3,227,   // 71-80
                      253,252,0x98,231,137,251,246,0xA1,247,0xA0,   // 81-90
                      1,1,1,1,1,1,0x86,225,0x8D,185,   // 91-100
                      0x91,232,226,224,243,241,242,0x92,230,0xA4,   // 101-110
                      0xA2,227,253,252,0x9E,231,137,251,246,0xA7, //111-120
					  247,0xA6,1,1,1,1,1,1,   // 121-128
					  1,1,1,1,1,1,1,1,  //129-136
					  1,1,1,1,1,1,1,1,  //137-144
					  1,1,1,1,1,1,1,1,  //145-152
					  1,1,1,1,1,1,1,1,  //153-160
					  1,1,1,1,1,1,1,1,  //161-168
					  1,1,1,1,1,1,1,1,  //169-176
					  1,1,1,1,1,1,1,1,  //177-184
					  1,1,1,1,1,1,1,1,  //185-192
					  1,1,1,1,1,1,1,1,  //193-200
					  1,1,1,1,1,1,1,1,  //201-208
					  1,1,0xA3,1,1,1,1,1,  //209-216
					  1,1,1,1,1,1,1,1,  //217-224
					  1,1,1,1,1,1,1,1,  //225-232
					  1,1,1,1,1,1,1,1,  //233-240
					  1,1,0xA2,1,1,1,1,1,  //241-248
					  1,1,1,1,1,1,1};  //249-255

//Alt - Letter Duze litery
int scancode6_[256] = { 1,0x8F,177,0x95,187,0x90,192,180,186,179,184,   // 0-10
					  181,0x9C,189,0xA5,234/*0xA3*/,178,185,248,228/*0x98*/,183,   // 11-20
					  191,188,182,0xA0,190,0xA1,129,120,121,122,   // 21-30
					  123,124,125,126,127,128,1,1,1,1,   // 31-40
					  0xB9,1,1,1,1,1,104,105,106,107,   // 41-50
					  108,109,110,111,112,113,114,133,1,1,   // 51-60
					  1,1,1,1,0x8F,225,0x95,185,0x90,232,   // 61-70
					  226,224,243,241,242,0x9C,230,0xA5,0xA3,227,   // 71-80
					  253,252,0x98,231,137,251,246,0xA1,247,0xA0,   // 81-90
					  1,1,1,1,1,1,0x86,225,0x8D,185,   // 91-100
					  0x91,232,226,224,243,241,242,0x92,230,0xA4,   // 101-110
					  0xA2,227,253,252,0x9E,231,137,251,246,0xA7, //111-120
					  247,0xA6,1,1,1,1,1,1,   // 121-128
					  1,1,1,1,1,1,1,1,  //129-136
					  1,1,1,1,1,1,1,1,  //137-144
					  1,1,1,1,1,1,1,1,  //145-152
					  1,1,1,1,1,1,1,1,  //153-160
					  1,1,1,1,1,1,1,1,  //161-168
					  1,1,1,1,1,1,1,1,  //169-176
					  1,1,1,1,1,1,1,1,  //177-184
					  1,1,1,1,1,1,1,1,  //185-192
					  1,1,1,1,1,1,1,1,  //193-200
					  1,1,1,1,1,1,1,1,  //201-208
					  1,1,0xA3,1,1,1,1,1,  //209-216
					  1,1,1,1,1,1,1,1,  //217-224
					  1,1,1,1,1,1,1,1,  //225-232
					  1,1,1,1,1,1,1,1,  //233-240
					  1,1,0xA2,1,1,1,1,1,  //241-248
					  1,1,1,1,1,1,1 };  //249-255
//AltGR male litery
int scancode9[256]={ 1,0x86,225,0x8D,185,0x91,232,226,224,243,241,   // 0-10
                      242,0x92,230,0xA4,0xA2,227,253,252,0x9E,231,   // 11-20
                      137,251,246,0xA6,247,0xA7,129,120,121,122,   // 21-30
                      123,124,125,126,127,128,1,1,1,1,   // 31-40 changed 124 to 137
                      0xB9,1,1,1,1,1,104,105,106,107,   // 41-50
                      108,109,110,111,112,113,114,133,1,1,   // 51-60
                      1,1,1,1,0x8F,225,0x95,185,0x90,232,   // 61-70
                      226,224,243,241,242,0x9C,230,0xA5,0xA3,227,   // 71-80
                      253,252,0x98,231,137,251,246,0xA1,247,0xA0,   // 81-90
                      1,1,1,1,1,1,0x86,225,0x8D,185,   // 91-100
                      0x91,232,226,224,243,241,242,0x92,230,0xA4,   // 101-110
                      0xA2,227,253,252,0x9E,231,137,251,246,0xA7, //111-120
					  247,0xA6,1,1,1,1,1,1,   // 121-128
					  1,1,1,1,1,1,1,1,  //129-136
					  1,1,1,1,1,1,1,1,  //137-144
					  1,1,1,1,1,1,1,1,  //145-152
					  1,1,1,1,1,1,1,1,  //153-160
					  1,1,1,1,1,1,1,1,  //161-168
					  1,1,1,1,1,1,1,1,  //169-176
					  1,1,1,1,1,1,1,1,  //177-184
					  1,1,1,1,1,1,1,1,  //185-192
					  1,1,1,1,1,1,1,1,  //193-200
					  1,1,1,1,1,1,1,1,  //201-208
					  1,1,0xA3,1,1,1,1,1,  //209-216
					  1,1,1,1,1,1,1,1,  //217-224
					  1,1,1,1,1,1,1,1,  //225-232
					  1,1,1,1,1,1,1,1,  //233-240
					  1,1,0xA2,1,1,1,1,1,  //241-248
					  1,1,1,1,1,1,1};  //249-255
//Alt male litery
int scancode9_[256] = { 1,0x86,225,0x8D,235/*185*/,0x91,232,226,171,243,241,   // 0-10
					  242,0x92,230,0xA4,0xA2,227,172,252,229/*0x9E*/,231,   // 11-20
					  137,251,246,0xA6,247,0xA7,129,120,121,122,   // 21-30
					  123,124,125,126,127,128,1,1,1,1,   // 31-40 changed 124 to 137
					  0xB9,1,1,1,1,1,104,105,106,107,   // 41-50
					  108,109,110,111,112,113,114,133,1,1,   // 51-60
					  1,1,1,1,0x8F,225,0x95,185,0x90,232,   // 61-70
					  226,224,243,241,242,0x9C,230,0xA5,0xA3,227,   // 71-80
					  253,252,0x98,231,137,251,246,0xA1,247,0xA0,   // 81-90
					  1,1,1,1,1,1,0x86,225,0x8D,185,   // 91-100
					  0x91,232,226,224,243,241,242,0x92,230,0xA4,   // 101-110
					  0xA2,227,253,252,0x9E,231,137,251,246,0xA7, //111-120
					  247,0xA6,1,1,1,1,1,1,   // 121-128
					  1,1,1,1,1,1,1,1,  //129-136
					  1,1,1,1,1,1,1,1,  //137-144
					  1,1,1,1,1,1,1,1,  //145-152
					  1,1,1,1,1,1,1,1,  //153-160
					  1,1,1,1,1,1,1,1,  //161-168
					  1,1,1,1,1,1,1,1,  //169-176
					  1,1,1,1,1,1,1,1,  //177-184
					  1,1,1,1,1,1,1,1,  //185-192
					  1,1,1,1,1,1,1,1,  //193-200
					  1,1,1,1,1,1,1,1,  //201-208
					  1,1,0xA3,1,1,1,1,1,  //209-216
					  1,1,1,1,1,1,1,1,  //217-224
					  1,1,1,1,1,1,1,1,  //225-232
					  1,1,1,1,1,1,1,1,  //233-240
					  1,1,0xA2,1,1,1,1,1,  //241-248
					  1,1,1,1,1,1,1 };  //249-255

int scancode7[256]={1,1,1,1,1,1,1,1,1,1,1,   // 0-10
                      1,1,1,1,1,1,1,1,1,1,   // 11-20
                      1,1,1,1,1,1,1,1,1,1,   // 21-30
                      1,1,1,1,1,1,1,1,1,1,   // 31-40
                      1,1,1,1,1,1,1,1,1,1,   // 41-50
                      1,1,1,1,1,1,1,1,1,1,   // 51-60
                      1,1,1,1,1,1,1,1,1,1,   // 61-70
                      1,1,1,1,1,1,1,1,1,1,   // 71-80
                      1,1,1,1,1,1,1,1,1,0xA0,   // 81-90
                      1,1,1,1,1,1,1,1,1,1,   // 91-100
                      1,1,1,1,1,1,1,1,1,1,   // 101-110
                      1,1,1,1,1,1,1,1,1,1,  //111-120
					  1,0xA6,1,1,1,1,1,1,   // 111-128
					  1,1,1,1,1,1,1,1,  //129-136
					  1,1,1,1,1,1,1,1,  //137-144
					  1,1,1,1,1,1,1,1,  //145-152
					  1,1,1,1,1,1,1,1,  //153-160
					  1,1,1,1,1,1,1,1,  //161-168
					  1,1,1,1,1,1,1,1,  //169-176
					  1,1,1,1,1,1,1,1,  //177-184
					  1,1,1,1,1,1,1,1,  //185-192
					  1,1,1,1,1,1,1,1,  //193-200
					  1,1,1,1,1,1,1,1,  //201-208
					  1,1,1,1,1,1,1,1,  //209-216
					  1,1,1,1,1,1,1,1,  //217-224
					  1,1,1,1,1,1,1,1,  //225-232
					  1,1,1,1,1,1,1,1,  //233-240
					  1,1,1,1,1,1,1,1,  //241-248
					  1,1,1,1,1,1,1};  //249-255

int scancode8[256]={1,1,1,1,1,1,1,1,1,1,1,   // 0-10
                      1,1,1,1,1,1,1,1,1,1,   // 11-20
                      1,1,1,1,1,1,1,1,1,1,   // 21-30
                      1,1,1,1,1,1,1,1,1,1,   // 31-40
                      1,1,1,1,1,1,1,1,1,1,   // 41-50
                      1,1,1,1,1,1,1,1,1,1,   // 51-60
                      1,1,1,1,1,1,1,1,1,1,   // 61-70
                      1,1,1,1,1,1,1,1,1,1,   // 71-80
                      1,1,1,1,1,1,1,1,1,0xA1,   // 81-90
                      1,1,1,1,1,1,1,1,1,1,   // 91-100
                      1,1,1,1,1,1,1,1,1,1,   // 101-110
                      1,1,1,1,1,1,1,1,1,1,   //111-120
					  1,0xA7,1,1,1,1,1,1,   // 121-128
					  1,1,1,1,1,1,1,1,  //129-136
					  1,1,1,1,1,1,1,1,  //137-144
					  1,1,1,1,1,1,1,1,  //145-152
					  1,1,1,1,1,1,1,1,  //153-160
					  1,1,1,1,1,1,1,1,  //161-168
					  1,1,1,1,1,1,1,1,  //169-176
					  1,1,1,1,1,1,1,1,  //177-184
					  1,1,1,1,1,1,1,1,  //185-192
					  1,1,1,1,1,1,1,1,  //193-200
					  1,1,1,1,1,1,1,1,  //201-208
					  1,1,1,1,1,1,1,1,  //209-216
					  1,1,1,1,1,1,1,1,  //217-224
					  1,1,1,1,1,1,1,1,  //225-232
					  1,1,1,1,1,1,1,1,  //233-240
					  1,1,1,1,1,1,1,1,  //241-248
					  1,1,1,1,1,1,1};  //249-255


static int first_ctx=0;
static BOOL ctx_ok=FALSE;
static BOOL ctx_created=FALSE;
static int i_ctx=0;
static int i_ctx_created;

#define LAST_POLYGONS_NO 4

static p_vector first_polygon_vector = { 0 };
static p_vector last_polygon_vector = { 0 };
static T_PixelTVal last_polygon[LAST_POLYGONS_NO][8] = { 0 };

typedef struct
{
  int x0;
  int y0;   //dla potrzeb wycinkowego buforowania
  int ww;
  int wh;
} Ctx_Par;

static Ctx_Par ctx_par[32];   //maksymalna liczba platow

typedef COLOR_ COLOR[256];
                        
int color256[]={16,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16, /*0..16*/
  		       8,8,8,8,8,8, /*17..22*/
		       15,15,15,15,15,15,15,15,31, /*23..31*/
		       5,5,5, /*32..34*/
		       6,6,6,6, /*35..38*/
		       9,9, /*39..40*/
		       10,10, /*41..42*/
		       2,2,/*43..44*/
		       3,3,3,3,3,3, /*45..50*/
		       4,4,4, /*51..53*/
		       5,5,5, /*54..56*/
		       6,6,6,6,6,6, /*57..62*/
		       1,1,1, /*63..65*/
		       2,2,2,2, /*66..69*/
		       3,3,3,3,3, /*70..74*/
		       4,4,4, /*75..77*/
		       5,5, /*78..79*/
		       6,6,6,6,6,6,6,6,6,6, /*80..89*/
		       2,2,2,2, /*90..93*/
		       3,3,3,3,3, /*94..98*/
		       4,4,4,4,4, /*99..103*/
		       13,13, /*104..105*/
		       14,14,14,14,14, /*106..110*/
		       9,9, /*111,112*/
		       10,10,10, /*113..115*/
		       11,11,11,11,11,11,11, /*116..122*/
		       12,12,12, /*123..125*/
		       13,13, /*126..127*/
		       14,14,14,14,14,14,14,14,14, /*128..136*/
		       10,10,10,10, /*137..140*/
		       11,11,11,11,11,11, /*141..146*/
		       12,12,12,12,12, /*147..151*/
		       14,14,14,14,14,14,14, /*152..158*/
		       10,10,10,10,10,10, /*159..164*/
		       11,11,11,11,11, /*165..169*/
		       12,12,12,12,12,12, /*170..175*/
		       13,13, /*176..177*/
		       14,14,14,14,14, /*178..182*/
		       9,9,9, /*183..185*/
		       10,10,10, /*186..188*/
		       11,11,11,11,11,11, /*189..194*/
		       12,12,12, /*195..197*/
		       13,13, /*198..199*/
		       14,14,14,14,14,14,14,14,14,14, /*200..209*/
		       10,10,10, /*210..212*/
		       11,11,11,11,11,11, /*213..218*/
		       12,12,12,12,12, /*219..223*/
		       14,14,14,14,14,14, /*224..229*/
		       10,10,10,10,10, /*230..234*/
		       12,12,12,12,12,12,12,12,12,12,12,12,12, /*235..247*/
		       8, /*248*/
		       5,3,4,1,6,2,7, }; /*249..255*/

/*bardzo cienka*/
char l_e_bc[32]={6,6, 8, 8, 2,14,10,4,12,   4, 6, 8, 2,10,8, 4,10,  8, 10,10, 4,14,14,6, 12,12,14,14,12,12,14,14,};
char pp0_bc[32]={9,12,16,18,6,24,18,6,23,   6,11,15, 4,18,15,5,18,  16,20,24,10,27,25,12,23,22,25,25,25,24,26,26,};
char pp_bc[32][16]={{1,2,1,2,1,2,0,0,0,0,0,0,0,0,0,0},   /*ciagla*/
        {1,2,1,2,1,0,0,0,0,0,0,0,0,0,0,0,},   /*kreskowa*/
        {1,2,1,2,1,4,1,0,0,0,0,0,0,0,0,0,},   /*punktowa*/
        {1,2,1,4,1,4,1,0,0,0,0,0,0,0,0,0,},   /*dwupunktowa*/
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*wielopunktowa*/
        {1,2,1,2,1,3,1,2,1,2,1,3,1,0,0,0,},   /*granica*/
        {1,2,1,2,1,2,1,3,2,0,0,0,0,0,0,0,},   /*srodkowa*/
        {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*ukryta*/  /*6*/
        {1,2,1,2,1,2,1,3,2,3,2,0,0,0,0,0,},   /*widmo*/
       
        {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*kreskowa_/2*/
        {1,2,1,3,1,0,0,0,0,0,0,0,0,0,0,0,},   /*punktowa_/2*/
        {1,2,1,3,1,3,1,0,0,0,0,0,0,0,0,0,},   /*dwupunktowa_/2*/
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*wielopunktowa_/2*/
        {1,2,1,3,1,2,1,3,1,0,0,0,0,0,0,0,},   /*granica_/2*/
        {1,2,1,2,1,3,2,0,0,0,0,0,0,0,0,0,},   /*srodkowa_/2*/
        {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*ukryta_/2*/  /*5*/
        {1,2,1,2,1,3,1,3,1,0,0,0,0,0,0,0,},   /*widmo_/2*/
       
        {1,2,1,2,1,2,1,0,0,0,0,0,0,0,0,0,},    /*kreskowa_x2*/
        {1,2,1,2,1,2,1,4,1,0,0,0,0,0,0,0,},    /*punktowa_x2*/
        {1,2,1,2,1,5,1,5,1,0,0,0,0,0,0,0,},    /*dwupunktowa_x2*/
        {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*wielopunktowa_x2*/
        {1,2,1,2,1,4,1,2,1,2,1,4,1,0,0,0,},    /*granica_x2*/
        {1,2,1,2,1,2,1,2,1,4,1,2,1,0,0,0,},    /*srodkowa_x2*/
        {1,2,1,2,1,5,0,0,0,0,0,0,0,0,0,0,},    /*ukryta_x2*/  /*6*/
        {1,2,1,2,1,2,1,3,2,3,2,0,0,0,0,0,},    /*widmo_x2*/
       
        {1,2,1,2,1,3,1,3,1,3,1,0,0,0,0,0,},    /*linia1 _ ...*/
        {1,2,1,3,1,2,1,2,1,3,1,3,1,0,0,0,},    /*linia2 _ _ ..*/
        {1,2,1,3,1,2,1,3,1,2,1,3,1,0,0,0,},    /*linia3 _ _ _ .*/
        {1,2,1,2,1,3,2,3,2,3,2,0,0,0,0,0,},    /*linia4 -- - - -*/
        {1,2,1,3,1,2,1,3,2,3,2,0,0,0,0,0,},    /*linia5 __ __ _ _*/
        {1,2,1,3,1,2,1,3,1,2,1,3,2,0,0,0,},    /*linia6 __ __ __ _*/
        {1,3,2,3,1,2,1,3,1,2,1,2,1,0,0,0,},};    /*linia7 _ __ ___ ____*/


char l_e_bc_x4[32] = { 6,12, 14, 8, 2,14,10,4,12,   4, 6, 8, 2,10,8, 4,10,  8, 10,10, 4,14,14,6, 12,12,14,14,12,12,14,14, };
char pp0_bc_x4[32] = { 9,32,32,18,6,24,18,6,23,   6,11,15, 4,18,15,5,18,  16,20,24,10,27,25,12,23,22,25,25,25,24,26,26, };
char pp_bc_x4[32][16] = { {1,2,1,2,1,2,0,0,0,0,0,0,0,0,0,0},   /*ciagla*/
	    {1,2,1,2,1,2,1,2,1,2,1,0,0,0,0,0,},   /*kreskowa*/
	    //{1,2,1,2,1,2,1,2,1,8,1,2,1,0,0,0,},   /*punktowa*/  x2
	    {1,2,1,2,1,2,1,2,1,8,1,2,1,0,0,0,},   /*punktowa*/
		{1,2,1,4,1,4,1,0,0,0,0,0,0,0,0,0,},   /*dwupunktowa*/
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*wielopunktowa*/
		{1,2,1,2,1,3,1,2,1,2,1,3,1,0,0,0,},   /*granica*/
		{1,2,1,2,1,2,1,3,2,0,0,0,0,0,0,0,},   /*srodkowa*/
		{1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*ukryta*/  /*6*/
		{1,2,1,2,1,2,1,3,2,3,2,0,0,0,0,0,},   /*widmo*/

		{1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*kreskowa_/2*/
		{1,2,1,3,1,0,0,0,0,0,0,0,0,0,0,0,},   /*punktowa_/2*/
		{1,2,1,3,1,3,1,0,0,0,0,0,0,0,0,0,},   /*dwupunktowa_/2*/
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*wielopunktowa_/2*/
		{1,2,1,3,1,2,1,3,1,0,0,0,0,0,0,0,},   /*granica_/2*/
		{1,2,1,2,1,3,2,0,0,0,0,0,0,0,0,0,},   /*srodkowa_/2*/
		{1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*ukryta_/2*/  /*5*/
		{1,2,1,2,1,3,1,3,1,0,0,0,0,0,0,0,},   /*widmo_/2*/

		{1,2,1,2,1,2,1,0,0,0,0,0,0,0,0,0,},    /*kreskowa_x2*/
		{1,2,1,2,1,2,1,4,1,0,0,0,0,0,0,0,},    /*punktowa_x2*/
		{1,2,1,2,1,5,1,5,1,0,0,0,0,0,0,0,},    /*dwupunktowa_x2*/
		{1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*wielopunktowa_x2*/
		{1,2,1,2,1,4,1,2,1,2,1,4,1,0,0,0,},    /*granica_x2*/
		{1,2,1,2,1,2,1,2,1,4,1,2,1,0,0,0,},    /*srodkowa_x2*/
		{1,2,1,2,1,5,0,0,0,0,0,0,0,0,0,0,},    /*ukryta_x2*/  /*6*/
		{1,2,1,2,1,2,1,3,2,3,2,0,0,0,0,0,},    /*widmo_x2*/

		{1,2,1,2,1,3,1,3,1,3,1,0,0,0,0,0,},    /*linia1 _ ...*/
		{1,2,1,3,1,2,1,2,1,3,1,3,1,0,0,0,},    /*linia2 _ _ ..*/
		{1,2,1,3,1,2,1,3,1,2,1,3,1,0,0,0,},    /*linia3 _ _ _ .*/
		{1,2,1,2,1,3,2,3,2,3,2,0,0,0,0,0,},    /*linia4 -- - - -*/
		{1,2,1,3,1,2,1,3,2,3,2,0,0,0,0,0,},    /*linia5 __ __ _ _*/
		{1,2,1,3,1,2,1,3,1,2,1,3,2,0,0,0,},    /*linia6 __ __ __ _*/
		{1,3,2,3,1,2,1,3,1,2,1,2,1,0,0,0,}, };    /*linia7 _ __ ___ ____*/


/*cienka*/
char l_e_c[32]={6,6, 8, 8, 2,14,10,4,12,   4, 6, 8, 2,10,8, 4,10,  8, 10,10, 4,14,14,6, 12,12,14,14,12,12,14,14,};
char pp0_c[32]={9,12,16,18,6,24,18,6,23,   6,11,15, 4,18,15,5,18,  16,20,24,10,27,25,12,23,22,25,25,25,24,26,26,};
char pp_c[32][16]={{1,2,1,2,1,2,0,0,0,0,0,0,0,0,0,0,},   /*ciagla*/
       {1,2,1,2,1,0,0,0,0,0,0,0,0,0,0,0,},   /*kreskowa*/
       {1,2,1,2,1,4,1,0,0,0,0,0,0,0,0,0,},   /*punktowa*/
       {1,2,1,4,1,4,1,0,0,0,0,0,0,0,0,0,},   /*dwupunktowa*/
       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*wielopunktowa*/
       {1,2,1,2,1,3,1,2,1,2,1,3,1,0,0,0,},   /*granica*/
       {1,2,1,2,1,2,1,3,2,0,0,0,0,0,0,0,},   /*srodkowa*/
       {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*ukryta*/  /*6*/
       {1,2,1,2,1,2,1,3,2,3,2,0,0,0,0,0,},   /*widmo*/
       
       {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*kreskowa_/2*/
       {1,2,1,3,1,0,0,0,0,0,0,0,0,0,0,0,},   /*punktowa_/2*/
       {1,2,1,3,1,3,1,0,0,0,0,0,0,0,0,0,},   /*dwupunktowa_/2*/
       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*wielopunktowa_/2*/
       {1,2,1,3,1,2,1,3,1,0,0,0,0,0,0,0,},   /*granica_/2*/
       {1,2,1,2,1,3,2,0,0,0,0,0,0,0,0,0,},   /*srodkowa_/2*/
       {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*ukryta_/2*/  /*5*/
       {1,2,1,2,1,3,1,3,1,0,0,0,0,0,0,0,},   /*widmo_/2*/
       
       {1,2,1,2,1,2,1,0,0,0,0,0,0,0,0,0,},    /*kreskowa_x2*/
       {1,2,1,2,1,2,1,4,1,0,0,0,0,0,0,0,},    /*punktowa_x2*/
       {1,2,1,2,1,5,1,5,1,0,0,0,0,0,0,0,},    /*dwupunktowa_x2*/
       {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*wielopunktowa_x2*/
       {1,2,1,2,1,4,1,2,1,2,1,4,1,0,0,0,},    /*granica_x2*/
       {1,2,1,2,1,2,1,2,1,4,1,2,1,0,0,0,},    /*srodkowa_x2*/
       {1,2,1,2,1,5,0,0,0,0,0,0,0,0,0,0,},    /*ukryta_x2*/  /*6*/
       {1,2,1,2,1,2,1,3,2,3,2,0,0,0,0,0,},    /*widmo_x2*/
       
       {1,2,1,2,1,3,1,3,1,3,1,0,0,0,0,0,},    /*linia1 _ ...*/
       {1,2,1,3,1,2,1,2,1,3,1,3,1,0,0,0,},    /*linia2 _ _ ..*/
       {1,2,1,3,1,2,1,3,1,2,1,3,1,0,0,0,},    /*linia3 _ _ _ .*/
       {1,2,1,2,1,3,2,3,2,3,2,0,0,0,0,0,},    /*linia4 -- - - -*/
       {1,2,1,3,1,2,1,3,2,3,2,0,0,0,0,0,},    /*linia5 __ __ _ _*/
       {1,2,1,3,1,2,1,3,1,2,1,3,2,0,0,0,},    /*linia6 __ __ __ _*/
       {1,3,2,3,1,2,1,3,1,2,1,2,1,0,0,0,},};    /*linia7 _ __ ___ ____*/

char l_e_c_x4[32] = { 6,12, 14, 8, 2,14,10,4,12,   4, 6, 8, 2,10,8, 4,10,  8, 10,10, 4,14,14,6, 12,12,14,14,12,12,14,14, };
char pp0_c_x4[32] = { 9,32,32,18,6,24,18,6,23,   6,11,15, 4,18,15,5,18,  16,20,24,10,27,25,12,23,22,25,25,25,24,26,26, };
char pp_c_x4[32][16] = { {1,2,1,2,1,2,0,0,0,0,0,0,0,0,0,0,},   /*ciagla*/
	   {1,2,1,2,1,2,1,2,1,2,1,0,0,0,0,0,},   /*kreskowa*/
	  // {1,2,1,2,1,2,1,2,1,8,1,2,1,0,0,0,},   /*punktowa*/ x2
	   {1,2,1,2,1,2,1,2,1,8,1,2,1,0,0,0,},   /*punktowa*/
	   {1,2,1,4,1,4,1,0,0,0,0,0,0,0,0,0,},   /*dwupunktowa*/
	   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*wielopunktowa*/
	   {1,2,1,2,1,3,1,2,1,2,1,3,1,0,0,0,},   /*granica*/
	   {1,2,1,2,1,2,1,3,2,0,0,0,0,0,0,0,},   /*srodkowa*/
	   {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*ukryta*/  /*6*/
	   {1,2,1,2,1,2,1,3,2,3,2,0,0,0,0,0,},   /*widmo*/

	   {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*kreskowa_/2*/
	   {1,2,1,3,1,0,0,0,0,0,0,0,0,0,0,0,},   /*punktowa_/2*/
	   {1,2,1,3,1,3,1,0,0,0,0,0,0,0,0,0,},   /*dwupunktowa_/2*/
	   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*wielopunktowa_/2*/
	   {1,2,1,3,1,2,1,3,1,0,0,0,0,0,0,0,},   /*granica_/2*/
	   {1,2,1,2,1,3,2,0,0,0,0,0,0,0,0,0,},   /*srodkowa_/2*/
	   {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},   /*ukryta_/2*/  /*5*/
	   {1,2,1,2,1,3,1,3,1,0,0,0,0,0,0,0,},   /*widmo_/2*/

	   {1,2,1,2,1,2,1,0,0,0,0,0,0,0,0,0,},    /*kreskowa_x2*/
	   {1,2,1,2,1,2,1,4,1,0,0,0,0,0,0,0,},    /*punktowa_x2*/
	   {1,2,1,2,1,5,1,5,1,0,0,0,0,0,0,0,},    /*dwupunktowa_x2*/
	   {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*wielopunktowa_x2*/
	   {1,2,1,2,1,4,1,2,1,2,1,4,1,0,0,0,},    /*granica_x2*/
	   {1,2,1,2,1,2,1,2,1,4,1,2,1,0,0,0,},    /*srodkowa_x2*/
	   {1,2,1,2,1,5,0,0,0,0,0,0,0,0,0,0,},    /*ukryta_x2*/  /*6*/
	   {1,2,1,2,1,2,1,3,2,3,2,0,0,0,0,0,},    /*widmo_x2*/

	   {1,2,1,2,1,3,1,3,1,3,1,0,0,0,0,0,},    /*linia1 _ ...*/
	   {1,2,1,3,1,2,1,2,1,3,1,3,1,0,0,0,},    /*linia2 _ _ ..*/
	   {1,2,1,3,1,2,1,3,1,2,1,3,1,0,0,0,},    /*linia3 _ _ _ .*/
	   {1,2,1,2,1,3,2,3,2,3,2,0,0,0,0,0,},    /*linia4 -- - - -*/
	   {1,2,1,3,1,2,1,3,2,3,2,0,0,0,0,0,},    /*linia5 __ __ _ _*/
	   {1,2,1,3,1,2,1,3,1,2,1,3,2,0,0,0,},    /*linia6 __ __ __ _*/
	   {1,3,2,3,1,2,1,3,1,2,1,2,1,0,0,0,}, };    /*linia7 _ __ ___ ____*/

       
/*gruba,bardzo gruba,2gruba*/         /*_/2*/                /*_x2*/                 /*linie1..7*/                
char l_e_g[32]={1,2,4, 6, 2,6, 4, 2,6,   2,4, 6, 2,6, 4, 2,6,   2, 4, 6, 2,6, 4, 2,6,  8, 8, 8, 8, 8, 8, 8,};
char pp0_g[32]={0,8,16,18,4,24,18,4,23,  6,12,13,3,17,12,3,15,  14,21,27,6,36,28,6,36 ,22,28,34,26,32,38,42,};
char pp_g[32][16]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*ciagla*/
       {4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*kreskowa*/
       {7,4,1,4,0,0,0,0,0,0,0,0,0,0,0,0,},    /*punktowa*/
       {7,3,1,3,1,3,0,0,0,0,0,0,0,0,0,0,},    /*dwupunktowa*/
       {1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*wielopunktowa*/
       {7,3,7,3,1,3,0,0,0,0,0,0,0,0,0,0,},    /*granica*/
       {10,3,2,3,0,0,0,0,0,0,0,0,0,0,0,0,},   /*srodkowa*/
       {2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*ukryta*/
       {10,3,2,3,2,5,0,0,0,0,0,0,0,0,0,0,},   /*widmo*/
       
       {3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*kreskowa_/2*/
       {5,3,1,3,0,0,0,0,0,0,0,0,0,0,0,0,},    /*punktowa_/2*/
       {5,2,1,2,1,2,0,0,0,0,0,0,0,0,0,0,},    /*dwupunktowa_/2*/
       {1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*wielopunktowa_/2*/
       {5,2,5,2,1,2,0,0,0,0,0,0,0,0,0,0,},    /*granica_/2*/
       {7,2,1,2,0,0,0,0,0,0,0,0,0,0,0,0,},   /*srodkowa_/2*/
       {1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*ukryta_/2*/
       {7,2,1,2,1,2,0,0,0,0,0,0,0,0,0,0,},   /*widmo_/2*/
       
       {7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*kreskowa_x2*/
       {10,5,1,5,0,0,0,0,0,0,0,0,0,0,0,0,},    /*punktowa_x2*/
       {10,5,1,5,1,5,0,0,0,0,0,0,0,0,0,0,},    /*dwupunktowa_x2*/
       {2,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*wielopunktowa_x2*/
       {10,5,10,5,1,5,0,0,0,0,0,0,0,0,0,0,},    /*granica_x2*/
       {15,5,3,5,0,0,0,0,0,0,0,0,0,0,0,0,},   /*srodkowa_x2*/
       {3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},    /*ukryta_x2*/
       {15,5,3,5,3,5,0,0,0,0,0,0,0,0,0,0,},   /*widmo_x2*/
       
       {7,3,1,3,1,3,1,3,0,0,0,0,0,0,0,0,},    /*linia1 _ ...*/
       {7,3,7,3,1,3,1,3,0,0,0,0,0,0,0,0,},    /*linia2 _ _ ..*/
       {7,3,7,3,7,3,1,3,0,0,0,0,0,0,0,0,},    /*linia3 _ _ _ .*/
       {8,3,2,3,2,3,2,3,0,0,0,0,0,0,0,0,},    /*linia4 -- - - -*/
       {8,3,8,3,2,3,2,3,0,0,0,0,0,0,0,0,},    /*linia5 __ __ _ _*/
       {8,3,8,3,8,3,2,3,0,0,0,0,0,0,0,0,},    /*linia6 __ __ __ _*/
       {2,3,4,3,8,3,16,3,0,0,0,0,0,0,0,0,},};   /*linia7 _ __ ___ ____*/
       
/*******************************************/
BOOL Cur_State_On ;

char *load_symbol[]={"", "DL", "LL", "Lr", "WL", "SL", "EL", "RL", "HL", "FL", "TL"};

int getlanguage(void)
{
#ifdef ENGLISH
#ifdef UKRAINIAN
    return 2;
#else
#ifdef SPANISH
return 3;
#else
    return 0;
#endif
#endif
#else
return 1;
#endif
}

typedef struct {
	int            linestyle;
	unsigned short upattern;    /* Note : BCC defines unsigned ! */
	int            thickness;
} linesettingstype_;

static linesettingstype_ lineinfo0;

GrContext *get_second_screen(void)
{
    return second_screen;
}

GrContext* get_second_screen_back(void)
{
	return second_screen_back;
}

void setlinestyle1(int line_style, unsigned short u_pattern, int thickness)
{
  lineinfo0.linestyle=line_style;
  lineinfo0.upattern=u_pattern;
  lineinfo0.thickness=thickness;
  setlinestyle(SOLID_LINE, 0, 1); // thickness);
}

void setlinestyle3(int line_style, unsigned short u_pattern, int thickness)
{
  lineinfo0.linestyle=line_style;
  lineinfo0.upattern=u_pattern;
  lineinfo0.thickness=thickness;
  setlinestyle(line_style, 0, thickness);
}

static int tab_thickness[8]={1,1,1,1,1,1,1,1};
static int solid_thickness[8]={1,1,1,3,5,1,1,1};

void setlinestyle2(int line_style, unsigned short u_pattern, int thickness)
{
  lineinfo0.linestyle=line_style;
  lineinfo0.upattern=u_pattern;
  if (thickness == 7) thickness = 2; //TEMPORARY for hiddent hatch outline
  else lineinfo0.thickness=thickness;
  setlinestyle(SOLID_LINE, 0, tab_thickness[thickness]);
}

void getlinesettings1(linesettingstype_ *lineinfo)
{
  memmove(lineinfo, &lineinfo0, sizeof(lineinfo0));
}


int getlinestyle(void)
{
	return lineinfo0.linestyle;
}

int getupattern(void)
{
	return lineinfo0.upattern;
}

extern void Clear_View (void) ;
extern void Image_Free (void) ;
extern void Save_View (void) ;
extern void Draw_Solid (int numpoints, T_PixelTVal* polypoints, unsigned pcx_solid, BOOL hatch_solid, double origin_x, double origin_y, char *s_pattern, int translucency, GRADIENT4 *gradient) ;
extern void Draw_Kolo (long_long xr,long_long yr,long_long R) ;
extern BOOL Point_in_Solid (WIELOKAT *ptr_w, double x, double y) ;
extern BOOL Check_DArea_in_Last_Solid (void) ;
extern BOOL Check_Draw_Pieslice (OKRAG *ad) ;
extern int Point_Selected (T_Point *) ;
extern int Vector_Selected (AVECTOR *ptrs_vector);
extern BOOL Ini_String_List (void) ;
extern double Get_Point_Size (void) ;
extern BOOL Get_Point_View (void) ;
extern BOOL Read_Hatch_Pattern (void) ;
extern int Edit_Text_Tab_F2 (void) ;
extern int Edit_Text_F3 (void) ;
extern int Usun_F5 (void) ;
extern int undo_blok_F6 (void) ;
extern int small_large_step(void);
extern int small_step (void);
extern int large_step (void);
extern int circle_and_rectangle_proc(void);
extern int set_orig_window(void);
extern int set_lastest_window(void);

extern int Free_Mouse (void);
extern int Wake_Up(void);
extern void reset_dane0(void);

static double wsp_x = 1, wsp_y = 1;

static char rysuj2[]="RYSUJ.$2";

static char logoalf[]="LOGOALF.LOG";

static void  mvcur(double dx, double dy);

long pXp,pYp,pXk,pYk;
static long pXp__, pYp__, pXk__, pYk__;

int spec_name_length;
double Ds_x ; /*uzyto w b_sel*/
double rpp ;
double skala_p_x ;
double typ_profilu ;
double hektometr_p ;
double hektometr_k ;
int znacznik_aplikacji ;
double dynamic_block ;
double localx ;
double localy ;
double angle_l;
double tana;
double tana1;
double sina;
double cosa;
double sina1;
double cosa1;

MyDane *dane_s;

static double Ds,y_pikseleY0, temp_X0, temp_Y0 ;
#define r23 23
static int skokBW(BOOL);
ESTR e={0,ESTR_Y, 0, -1, 0,r23 ,NULL, NULL, 0, 2, GV_POINT, NULL, skokBW, NULL};
static char  *dane0_Data[MAX_NUMBER_OF_WINDOWS] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static void  nooop(void)
{
 return;
}

#ifdef FORWIN32

void delay(int time)
{
	 rest(time);
}

#endif

void backup_world(void)
{
	pXp__ = pXp;
	pYp__ = pYp;
	pXk__ = pXk;
	pYk__ = pYk;
	pXp = 0;
	pYk = 0;
	pXk = getmaxx();
	pYp = getmaxy();
}

void world_back(void)
{
	pXp = pXp__;
	pYp = pYp__;
	pXk = pXk__;
	pYk = pYk__;
}

double my_round(double x, unsigned int digits) {
    double fac = pow(10, digits);
    return round(x*fac)/fac;
}

void ltoa1 (long wartosc, char * buf, int width)
/*--------------------------------------------*/
{
 sprintf(buf,"%#10ld",wartosc);
}

void dtoa(double wartosc, char * buf, int width, int prec)
/*--------------------------------------------------------*/
{
 if (width==10)
 {
  sprintf(buf,"%#10.3lg",wartosc);
  }
  else if (width==6)
   {
    sprintf(buf,"%#6.3lg",wartosc);
   }
   else
   {
    sprintf(buf,"%#5.2lg",wartosc);
   }
}

void color_bar(void)
{ int a_kolor;
a_kolor = Get_Current_Color () ;
setwritemode(COPY_PUT);
setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
setfillstyle_ (SOLID_FILL, GetColorAC (a_kolor)) ;
bar (0, ED_INF_HEIGHT + 1, vfv(8), 2*ED_INF_HEIGHT - 2 /*3*/) ;
}


void orto_l(LINIA *L, int *Orto_Dir)
{   double tana, tana1, angle_l;
    float Lx3, Ly3, Lx4, Ly4;

    angle_l=get_angle_l();
    if (angle_l<0) angle_l+=360;
    if (angle_l!=0)
    {
     if ((angle_l==90) || (angle_l==270))
     {
     Lx3=L->x1;
     Ly3=L->y2;
     Lx4=L->x2;
     Ly4=L->y1;
     }
     else
     {
     tana=get_tana();
     tana1=get_tana1();
     Lx3 = ((tana * L->x1) - (tana1*L->x2) + (L->y2) - (L->y1))/(tana - tana1);
     Ly3 = L->y2 + ( tana1 * (Lx3 - L->x2) );
     Lx4 =L->x1 - (Lx3 - L->x2);
     Ly4 =L->y1 + (L->y2 - Ly3);
     }
     if (*Orto_Dir == I_Orto_XDir)
     {
      L->x2=Lx3;
      L->y2=Ly3;
     }
     else
      {
       L->x2=Lx4;
       L->y2=Ly4;
      }
    }
    else
     {
      if (*Orto_Dir == I_Orto_XDir)
      {
       L->y2=L->y1;
      }
      else
       {
	L->x2=L->x1;
       }
     }

}

double Get_Ds_x(void)
{
	return Ds_x;
}

double Get_Ds(void)
{
    return Ds;
}

long pikseleX (double jednostki)
{ return xp + Double_to_Long (/*floor*/ (0.5 + (jednostki-Xp)*Ds_x)); }

long pikseleX0 (double jednostki)
{ return   /*Double_to_Long*/ (/*floor*/ (temp_X0 + jednostki * Ds_x)) ;}  

float pikseleX0f(float jednostki)
{
	return   (float)(temp_X0 + jednostki * Ds_x);
}

long pikseleX00 (double jednostki)
{ return   /*Double_to_Long*/ (floor (jednostki * Ds_x)) ;}

long pikseleY00 (double jednostki)
{ return    /*Double_to_Long*/ (floor (jednostki * Ds)) ;}


long pikseleXdpi (double jednostki, int dpi)
{ double piksele;
  piksele=jednostki/25.4;  //cale
  piksele*=dpi;
  piksele*=sk_x;
  return (floor(piksele));
}

long pikseleYdpi (double jednostki, int dpi)
{ double piksele;
  piksele=jednostki/25.4;   //cale
  piksele*=dpi;
  return (floor(piksele));
}

double pikseleX0d (double jednostki)
{ double x_ret;
  x_ret=   /*Double_to_Long*/ (temp_X0 + jednostki * Ds_x) ;
  return x_ret;
}

long pikseleDX (double jednostki)
{ return /*Double_to_Long*/ (/*floor*/ (0.5+ jednostki*Ds_x));}

long pikseleY (double jednostki)
{ return maxY-yk + Double_to_Long (floor (0.5 - (jednostki-Yp)*Ds));} 

long pikseleY0 (double jednostki)
{ return    /*Double_to_Long*/ (/*floor*/ (temp_Y0 - jednostki * Ds)) + 0.5; } //+0.5  03/07/2022

float pikseleY0f(float jednostki)
{
	return    (float)(temp_Y0 - jednostki * Ds);
}

double pikseleY0d (double jednostki)
{ double y_ret;
  y_ret=  /*Double_to_Long*/ (temp_Y0 - jednostki * Ds) ;
  return y_ret;
}

double jednostkiX0 (double xx_)
{
 double x_ret;
 x_ret = (-temp_X0 + pXp + xx_) / Ds_x;
 return x_ret;
}

double jednostkiY0 (double yy_)
{
 double y_ret;
 y_ret = (temp_Y0 - yy_ + pYk) / Ds;
 return y_ret;
}

float jednostkiX0_(long pixels)
{
	float xx;
	xx=(float)(((double)pixels - temp_X0) / Ds_x);
	return xx;
}

float jednostkiY0_(long pixels)
{
	return  (float)((temp_Y0 - (double)pixels) / Ds);
}

void set_tools_window(int x1,int y1,int x2,int y2)
{
  tools_window.x1=jednostkiX0(x1);
  tools_window.y1=jednostkiY0(y1);
  tools_window.x2=jednostkiX0(x2);
  tools_window.y2=jednostkiY0(y2);
}

long pikseleDY (double jednostki)
{ return Double_to_Long (floor (0.5+jednostki*Ds));}


double pikseleFDY(double jednostki)
{
	return (float)(jednostki * Ds);
}

double jednostkiX(long piksele)
{return (piksele/(Ds_x));}

double jednostkiY(long piksele)
{return (piksele/(Ds));}

double jednostkiplt(double mmplt)   /*mm (plt) -> jednostki */
{return  mmplt;}		/* nie uwzgledniono ze (Xmin, Ymin)
					nie zawsz jest w (0,0)*/

double jednostkiOb(double mmob)     /*mm (ob) -> jednostki */
{return  mmob*Jednostki/SkalaF;}

static const double inch_mm = 25.4;

float jednostkiObXm(double mob)     /*m (ob) local -> jednostki global*/
{return  (float)(mob*1000.0/SkalaF + localx);}

float jednostkiObXi(double mob)     /*in (ob) local -> jednostki global*/
{return  (float)(mob*inch_mm/SkalaF + localx);}

float jednostkiObYm(double mob)     /*m (ob) local -> jednostki global*/
{return  (float)(mob*1000.0/SkalaF + localy);}

float jednostkiObYi(double mob)     /*in (ob) local -> jednostki global*/
{return  (float)(mob*inch_mm/SkalaF + localy);}

double get_skala_profilu_x(void)    //mnoznik skali
{ return skala_p_x; }

double get_poziom_pp(void)        //poziom zerowy profilu w m
{ return rpp; }

double get_typ_profilu(void)        //typ profilu: 0 - kanalizacyjny, 1 - technologiczny
{ return typ_profilu; }

double get_hektometr_p(void)        //poczatkowy hektometr
{ return hektometr_p; }

double get_hektometr_k(void)        //koncowy hektometr
{ return hektometr_k; }

int get_znacznik_aplikacji(void)
{ return znacznik_aplikacji; }

int put_znacznik_aplikacji(int znacznik_aplikacji0)
{
znacznik_aplikacji=znacznik_aplikacji0;
return 1;
}

int put_dynamic_block(double dynamic_block_a)
{
dynamic_block=dynamic_block_a;
return 1;
}

double get_dynamic_block(void)
{return dynamic_block;}

int put_skala_profilu_x(double skala_px)    //mnoznik skali
{
skala_p_x=skala_px;
return 1;
}

int put_poziom_pp(double rpp0)        //poziom zerowy profilu w m
{
rpp=rpp0;
return 1;
}

int put_typ_profilu(double typ_profilu0)
{
typ_profilu=typ_profilu0;
return 1;
}

int put_hektometr(double hektometr, int opcja)        //poziom zerowy profilu w m
{
if (opcja==0) hektometr_p=hektometr;
 else hektometr_k=hektometr;
return 1;
}


int put_angle_l(double a_l)
{ double angle_lr;
angle_l=a_l;
angle_lr=(angle_l/360.0) * Pi2;
tana = tan (angle_lr);
if (options1.uklad_izometryczny)
{ if (FALSE == Check_if_Equal (tana, 0)) tana1= tan ( angle_lr + (Pi2/3.));
    sina1=sin (angle_lr+2.094395102);
    cosa1=cos (angle_lr+2.094395102);
}
else { if (FALSE == Check_if_Equal (tana, 0)) tana1= tan ( angle_lr + (Pi2/4.));
    sina1=sin (angle_lr+1.570796327);
    cosa1=cos (angle_lr+1.570796327);
}
sina = sin (angle_lr);
cosa = cos (angle_lr);
return 1;
}

int put_angle_lr(double a_l0, double a_l)
{  int l_kr;
    l_kr = put_angle_l(a_l);
    Set_Second_Screen();
    okno_r_second();
    l_kr = put_angle_l(a_l0);
    ramka(2);
    l_kr = put_angle_l(a_l);
    ramka(3);
    okno_all_second();
    Set_Screen();
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
}

int put_localx(double local_x)
{
localx=local_x;
return 1;
}


int put_localy(double local_y)
{
localy=local_y;
return 1;
}

void set_angle_l(double angle)
{ angle_l=angle; }

double get_angle_l(void)
{ return angle_l; }

double get_tana(void)
{ return tana; }

double get_tana1(void)
{ return tana1; }

double get_sina(void)
{ return sina; }

double get_cosa(void)
{ return cosa; }

double get_cosa1(void)
{ return cosa1; }

double get_sina1(void)
{ return sina1; }

double get_localx(void)
{ return localx; }

double get_localy(void)
{ return localy; }


double milimetryplt(double jednostki)  /* jednostki -> mm (plt) */
{return  jednostki;}                   /* nie uwzgledniono ze (Xmin, Ymin)
					nie zawsz jest w (0,0)*/

/* 'd' oznacza przyrost jednostek,
polorzenie poczatku ukladu wspolrzednych nie ma znaczenia */
double jednostki_d_to_plt_d (double jednostki_d)  /* jednostki -> mm (plt) */
/*----------------------------------------------*/
{
  return jednostki_d;
}

double plt_d_to_jednostki_d (double mmplt)  /* mm (plt) -> jednostki*/
/*----------------------------------------*/
{
  return mmplt ;
}


/*transformacja milimetry --> jednostki  */
/* wyswietlane sa na ekranie jednostki */

double milimetryob(double jednostki)   /* jednostki -> mm (ob) */
{return  (jednostki*SkalaF/Jednostki);}

double milimetryobx(double jednostki)   /* jednostki -> mm (ob) */
{return  ((jednostki - localx) *SkalaF/Jednostki);}

double milimetryoby(double jednostki)   /* jednostki -> mm (ob) */
{return  ((jednostki - localy) *SkalaF/Jednostki);}

// Inverse of milimetryobx
double milimetryobx_inv(double obj_x) {
    return obj_x * Jednostki / SkalaF + localx;  // adjust if localx is already set differently
}

// Inverse of milimetryoby
double milimetryoby_inv(double obj_y) {
    return obj_y * Jednostki / SkalaF + localy;
}

double milimetryobxl (double x0, double y0)
/*-------------------------------------------*/
{
    double x, y;
    double jx,jy,jx0,jy0;
    double sina, cosa;
    double angle_l;

    x = milimetryobx (x0) ;
    y = milimetryoby (y0) ;

    angle_l=get_angle_l();

    if (angle_l!=0)
    {
        jx0=x;
        jy0=y;
        sina=get_sina();
        cosa=get_cosa();
        jx = (jx0 * cosa) + (jy0 * sina);
        jy = (-jx0 * sina) + (jy0 * cosa);
        jx0=jx;
        jy0=jy;

        jx=my_round(jx0, 6);
        jy=my_round(jy0, 6);

    }
    else
    {
        jx=my_round(x, 6);
        jy=my_round(y, 6);
    }

    if (options1.uklad_geodezyjny==0) return jx;
    else return jy;
}

double milimetryobyl (double x0, double y0)
/*------------------------------------------*/
{
    double x, y;
    double jx,jy,jx0,jy0;
    double sina, cosa;
    double angle_l;

    x = milimetryobx (x0) ;
    y = milimetryoby (y0) ;

    angle_l=get_angle_l();

    if (angle_l!=0)
    {
        jx0=x;
        jy0=y;
        sina=get_sina();
        cosa=get_cosa();
        jx = (jx0 * cosa) + (jy0 * sina);
        jy = (-jx0 * sina) + (jy0 * cosa);
        jx0=jx;
        jy0=jy;
        jx=my_round(jx0, 6);
        jy=my_round(jy0, 6);
    }
    else
    {
        jx=my_round(x, 6);
        jy=my_round(y, 6);
    }

    if (options1.uklad_geodezyjny==0) return jy;
    else return jx;
}

double mm_to_inch (double mm)
/*------------------------*/
{
   return    mm / inch_mm ;
}

double inch_to_mm (double inch)
/*------------------------*/
{
   return    inch * inch_mm ;
}

void okno_r(void)
/*---------------------*/
{
 setviewport (pXp,pYk,pXk,pYp,1);
}

void okno_r_second(void)
/*---------------------*/
{
 setviewport_second ((BITMAP *)second_screen,pXp,pYk,pXk,pYp,1);
}

void okno_r_bitmap(BITMAP *bitmap)
/*---------------------*/
{
    setviewport_second (bitmap,pXp,pYk,pXk,pYp,1);
}

void set_World(long pXp1, long pYp1, long pXk1, long pYk1)
/*-------------------------------------------------------*/
{
	pXp = pXp1;
	pYp = pYp1;
	pXk = pXk1;
	pYk = pYk1;
}

void get_World(long *pXp1, long *pYp1, long *pXk1, long *pYk1)
/*-------------------------------------------------------*/
{
	*pXp1 = pXp;
	*pYp1 = pYp;
	*pXk1 = pXk;
	*pYk1 = pYk;
}

int get_pXp(void)
/*------------------*/
{
 return (int)pXp;
}

int get_pXk(void)
/*------------------*/
{
 return (int)pXk;
}

int get_pYp(void)
/*------------------*/
{
 return (int)pYp;
}

int get_pYk(void)
/*------------------*/
{
 return (int)pYk;
}

static int jed_to_piks_x (int jednostki_x)
{
  wsp_x = 1.0 * (getmaxx () + 1) / 640 /*WIDTH_DISPLAY*/;
  return (0.5 + jednostki_x * wsp_x);
}

static int jed_to_piks_y (int jednostki_y)
{
  wsp_y = 1.0 * (getmaxy () + 1 - 37) / (480 /*HEIGHT_DISPLAY*/ - 37) ;
  return (0.5 + jednostki_y * wsp_y);
}

void okno_all(void)
/*---------------------*/
{
  setviewport (0,0,maxX,maxY,0);
}

void okno_all_second(void)
//------------------------
{
  setviewport_second ((BITMAP*)second_screen,0,0,maxX,maxY,0);
}

void okno_all_bitmap(BITMAP *bitmap)
//------------------------
{
    setviewport_second (bitmap,0,0,maxX,maxY,0);
}


void get_global_coords (double *x0, double *y0)
/*-------------------------------------------*/
{
  double x, y, xp, yp;
  double sina, cosa;
  double angle_l;

  if (options1.uklad_geodezyjny==0)
   {
    xp = *x0;
    yp = *y0;
   }
    else
     {
      yp = *x0;
      xp = *y0 ;
     }
  /*jezeli jest to uklad lokalny nalezy przetworzyc lokalne xp,yp na globalne x, y */
  angle_l=get_angle_l();
  if (angle_l!=0)
  {
   sina=get_sina();
   cosa=get_cosa();
   x=(xp*cosa) - (yp*sina);
   y=(xp*sina) + (yp*cosa);
  }
  else
   {
    x=xp;
    y=yp;
   }
  *x0=jednostkiOb(x) + get_localx();
  *y0=jednostkiOb(y) + get_localy();

  return;
}


void get_local_coords (double *x0, double *y0)
/*-------------------------------------------*/
{
    double x, y, xp, yp;
    double jx,jy,jx0,jy0;
    double sina, cosa;
    double angle_l;

    if (options1.uklad_geodezyjny==0)
    {
        xp = *x0;
        yp = *y0;
    }
    else
    {
        yp = *x0;
        xp = *y0 ;
    }

    x = milimetryobx (xp) ;
    y = milimetryoby (yp) ;

    angle_l=get_angle_l();

    if (angle_l!=0)
    {
        jx0=x;
        jy0=y;
        sina=get_sina();
        cosa=get_cosa();
        jx = (jx0 * cosa) + (jy0 * sina);
        jy = (-jx0 * sina) + (jy0 * cosa);
        jx0=jx;
        jy0=jy;

        jx=my_round(jx0, 6);
        jy=my_round(jy0, 6);

    }

    if (options1.uklad_geodezyjny==0) {
       *x0=jx; *y0=jy;
    }
    else
    {
        *y0=jx; *x0=jy;
    }

}

static int skokBW (BOOL b_graph_value)
/*----------------------------------*/
{
  double x, y, xp, yp;
  double sina, cosa;

  b_graph_value = b_graph_value ;

  if (e.val_no < 1 || e.st == NULL) return 0;
  if (options1.uklad_geodezyjny==0)
   {
    xp = e.values [0];
    yp = milimetryob(Y) ;
    if (e.val_no >= 2)
    {
      yp = e.values [1];
    }
   }
    else
     {
      yp = e.values [0];
      xp = milimetryob(X) ;
      if (e.val_no >= 2)
      {
        xp = e.values [1];
      }
     }

  /*jezeli jest to uklad lokalny nalezy przetworzyc lokalne xp,yp na globalne x, y */
  angle_l=get_angle_l();
  if (angle_l!=0)
  {
  sina=get_sina();
  cosa=get_cosa();
  x=(xp*cosa) - (yp*sina);
  y=(xp*sina) + (yp*cosa);
  }
  else
   {
   x=xp;
   y=yp;
   }
  mvcurp(jednostkiOb(x) - ( X - get_localx() ), jednostkiOb(y) - ( Y - get_localy() )) ;
  sel.wyj=1;
  return 1;
}

double cut_val(double jXY)
{
 if (fabs(jXY)<0.0000001) return 0;
  else return jXY;
}


#include <math.h>

#define SQRT3_OVER_2   (sqrt(3.0)/2.0)   // ≈ 0.8660254037844386
#define ONE_OVER_SQRT3 (1.0/sqrt(3.0))   // ≈ 0.5773502691896257
#define HALF           (0.5)

/* Cartesian -> Isometric */
int cartesian_to_isometric(double cx, double cy, double *ix, double *iy) {
    if (!ix || !iy) return -1;

    *ix = ONE_OVER_SQRT3 * cx + cy;
    *iy = -ONE_OVER_SQRT3 * cx + cy;

    return 0;
}

int cartesian_to_isometric_f(float cx, float cy, float *ix, float *iy) {
    if (!ix || !iy) return -1;

    *ix = ONE_OVER_SQRT3 * cx + cy;
    *iy = -ONE_OVER_SQRT3 * cx + cy;

    return 0;
}

/* Isometric -> Cartesian */
int isometric_to_cartesian(double ix, double iy, double *cx, double *cy) {
    if (!cx || !cy) return -1;

    *cx = SQRT3_OVER_2 * (ix - iy);
    *cy = HALF * (ix + iy);

    return 0;
}

int isometric_to_cartesian_f(float ix, float iy, float *cx, float *cy) {
    if (!cx || !cy) return -1;

    *cx = SQRT3_OVER_2 * (ix - iy);
    *cy = HALF * (ix + iy);

    return 0;
}


//#define M_PI 3.14159265358979323846

/*
 * Convert a Cartesian angle to its equivalent isometric angle.
 * Inputs:
 *   cart_angle_deg - Angle in degrees in Cartesian system
 * Returns:
 *   Angle in degrees in isometric system, normalized to [0, 360)
 */
double cartesian_angle_to_isometric_angle__(double cart_angle_deg) {
    double cart_rad = cart_angle_deg * M_PI / 180.0;
    double cart_dx = cos(cart_rad);
    double cart_dy = sin(cart_rad);
    double inv_sqrt3 = 1.0 / sqrt(3.0);
    double iso_dx = inv_sqrt3 * cart_dx + cart_dy;
    double iso_dy = -inv_sqrt3 * cart_dx + cart_dy;
    double iso_rad = atan2(iso_dy, iso_dx);
    double iso_deg = iso_rad * 180.0 / M_PI;
    if (iso_deg < 0.0) iso_deg += 360.0;
    return iso_deg;
}

/*
 * Compute the delta_angle_isometric corresponding to a given delta_angle_cartesian
 * at a specific current angle_cart.
 *
 * Inputs:
 *   angle_cart_deg       - Current angle in degrees in Cartesian system
 *   delta_angle_cart_deg - Delta angle in degrees to add in Cartesian space
 *
 * Returns:
 *   Equivalent delta angle in degrees to add in isometric space, signed and minimized (|delta| <= 180)
 */
double compute_delta_angle_isometric__(double angle_cart_deg, double delta_angle_cart_deg) {
    double old_iso = cartesian_angle_to_isometric_angle__(angle_cart_deg);
    double new_cart = fmod(angle_cart_deg + delta_angle_cart_deg + 360.0, 360.0);  // Normalize to [0, 360)
    double new_iso = cartesian_angle_to_isometric_angle__(new_cart);
    double delta_iso = new_iso - old_iso;
    if (delta_iso < -180.0) delta_iso += 360.0;
    if (delta_iso > 180.0) delta_iso -= 360.0;
    return delta_iso;
}

//#define M_PI 3.14159265358979323846

/*
 * Convert a Cartesian angle (in radians) to its equivalent isometric angle (in radians).
 * Input:  cart_angle_rad - angle in Cartesian system
 * Output: angle in isometric system, normalized to [0, 2π)
 */
//static
double cartesian_angle_to_isometric_angle(double cart_angle_rad)
{
    double cart_dx = cos(cart_angle_rad);
    double cart_dy = sin(cart_angle_rad);

    double inv_sqrt3 = 1.0 / sqrt(3.0);
    double iso_dx = inv_sqrt3 * cart_dx + cart_dy;
    double iso_dy = -inv_sqrt3 * cart_dx + cart_dy;

    double iso_rad = atan2(iso_dy, iso_dx);
    if (iso_rad < 0.0) iso_rad += 2.0 * M_PI;

    return iso_rad;
}

/*
 * Compute the isometric delta angle (in radians) that corresponds to
 * adding a given Cartesian delta angle (in radians) at a specific current
 * Cartesian angle.
 *
 * Inputs:
 *   angle_cart_rad       - Current direction angle in Cartesian (radians)
 *   delta_angle_cart_rad - Delta angle to add in Cartesian space (radians, can be negative)
 *
 * Returns:
 *   Equivalent delta angle in radians to add in isometric space
 *   (shortest direction, |delta| ≤ π)
 */
static double compute_delta_angle_isometric(double angle_cart_rad,
                                            double delta_angle_cart_rad)
{
    double old_iso = cartesian_angle_to_isometric_angle(angle_cart_rad);

    double new_cart_rad = angle_cart_rad + delta_angle_cart_rad;
    // Normalize to [0, 2π)
    new_cart_rad = fmod(new_cart_rad, 2.0 * M_PI);
    if (new_cart_rad < 0.0) new_cart_rad += 2.0 * M_PI;

    double new_iso = cartesian_angle_to_isometric_angle(new_cart_rad);

    double delta_iso = new_iso - old_iso;

    // Keep delta in [-π, π] range
    if (delta_iso > M_PI)  delta_iso -= 2.0 * M_PI;
    if (delta_iso < -M_PI) delta_iso += 2.0 * M_PI;

    return delta_iso;
}

void pozycja_kursora(void)
/*----------------------------*/
{
  double jX,jY,jX0,jY0;
  double sina1, cosa1;
  int ret;


  if (!X_Y_kursora)
  {
    return ;
  }
  e.st [0] = '\0' ;
  e.lmax = r27;
  jX = milimetryobx (X) ;
  jY = milimetryoby (Y) ;
  if (options1.uklad_izometryczny)
  {
      jX0 = jX;
      jY0 = jY;

      ret = cartesian_to_isometric(jX0, jY0, &jX, &jY);
  }
  else {
      if (angle_l != 0) {
          jX0 = jX;
          jY0 = jY;
          sina1 = get_sina();
          cosa1 = get_cosa();
          jX = (jX0 * cosa1) + (jY0 * sina1);
          jY = (-jX0 * sina1) + (jY0 * cosa1);
          jX = cut_val(jX);
          jY = cut_val(jY);
      }
  }

  if (options1.uklad_geodezyjny == 1) sprintf(e.st, "%#12.9lg; %#12.9lg", jY, jX);
  else sprintf(e.st, "%#12.9lg; %#12.9lg", jX, jY);

  e.x = getmaxx();

  change_color_wsp=TRUE;
  Out_Edited_Draw_Param ((ESTR *)&e, TRUE) ;
  change_color_wsp=FALSE;

}

void Reset_Pointer(void)
{

	reset_pointer = TRUE;
}

void DoneCursorimage(void)
{
	if (cursor_pattern != NULL) destroy_bitmap(cursor_pattern);
	if (cursor_pattern_bitmap0 != NULL) destroy_bitmap(cursor_pattern_bitmap0);
	if (cursor_pattern_bitmap != NULL) destroy_bitmap(cursor_pattern_bitmap);
}


void out_cur_off(double X, double Y) {
	flip_screen();
}


#ifndef LINUX
BOOL INTL_CURSOR = TRUE; // FALSE;
#else
BOOL INTL_CURSOR = TRUE;
#endif


//#include <stdint.h>
//#include <float.h>
//#include <math.h>

typedef struct {
    long x, y;
} IPoint;

/**
 * Finds the two intersection points of a line with the rectangle.
 * @param x0, y0: Internal point (integers)
 * @param xmax, ymax: Rectangle bounds (integers)
 * @param cosa, sina: Precomputed cos and sin of the angle
 * @param out: Array of 2 IPoints to be filled
 */
void get_intersections(int x0, int y0, long xmax, long ymax,
                       double cosa, double sina, IPoint out[2]) {
    double t[4];
    int count = 0;

    // Distances to vertical boundaries (x=0 and x=xmax)
    // x = x0 + t*cosa  => t = (x - x0) / cosa
    t[0] = (0 - x0) / cosa;
    t[1] = (double)(xmax - (long)x0) / cosa;

    // Distances to horizontal boundaries (y=0 and y=ymax)
    // y = y0 + t*sina  => t = (y - y0) / sina
    t[2] = (0 - y0) / sina;
    t[3] = (double)(ymax - (long)y0) / sina;

    for (int i = 0; i < 4; i++) {
        double ix = x0 + t[i] * cosa;
        double iy = y0 + t[i] * sina;

        // Check if the point lies on the rectangle's segments
        // We use a small epsilon for floating point comparison against bounds
        if (ix >= -0.001 && ix <= (double)xmax + 0.001 &&
            iy >= -0.001 && iy <= (double)ymax + 0.001) {

            out[count].x = (long)round(ix);
            out[count].y = (long)round(iy);

            // Ensure we don't return the same corner twice
            if (count > 0 && out[0].x == out[1].x && out[0].y == out[1].y) continue;

            count++;
            if (count == 2) break;
        }
    }
}

// To find intersections for both lines:
// Line 1: Use (cosa, sina)
// Line 2: Use (-sina, cosa)  <-- This is the perpendicular vector (rotated 90 deg)


void out_cur_on_factory(double X, double Y, int circle)
{
	int x0, y0, x1, y1, kursor05, kursor01;
	float f_x, f_y;
	int cursor_color;
	int lpoints;
	int ii;

	int xpp, xkk, ypp, ykk;
	float background_r;
    float background_g;
    float background_b;
	int color=0, color_r, color_g, color_b;
	float colord;
	int cur_d, cur_d_a;
	SOLID_PATTERN_LIBRARY_ITEM solid_pattern_library_item;
	int rgb, r,g,b;
    float A, A1;
    float brightness_b, brightness_c;
    int pXp1, pYp1, pXk1, pYk1;
    int w2, h2;
    BITMAP *sec_screen;
	int d30, d10;
	int A5pYk;

#ifdef ALLEGRO5
	A5pYk = (int)pYk;
#else
	A5pYk = 0;
#endif


	d30=30*(RETINA+1);
	d10=10*(RETINA+1);

    pXp1=get_pXp();
    pXk1=get_pXk();
    pYp1=get_pYp();
    pYk1=get_pYk();

    background_r = 0.0f;
    background_g = 0.0f;
    background_b = 0.0f;

	f_x = (float)X;
	f_y = (float)Y;
	//x0 = pikseleX0(f_x);
	//y0 = pikseleY0(f_y);
	x0 = (int)pikseleX0(X);
	y0 = (int)pikseleY0(Y);

    x1 = (int)pikseleX(X);
    y1 = (int)pikseleY(Y);

	setcolor(kolory.cur);
	cursor_color = palette_color[kolory.cur];

	setwritemode(COPY_PUT);

	setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);

	okno_r();

	if ((KursorS == 0) || (KursorS == 100)) cur_d = d30;
	else
	{
		if (KursorS > 100) cur_d = (KursorS - 100)*(RETINA+1);
		else cur_d = KursorS*(RETINA+1);
	}

	if (reset_pointer == TRUE)
	{
		reset_pointer = FALSE;
		if (KursorS_arch > 100) cur_d_a = (KursorS_arch - 100)*(RETINA+1);
		else cur_d_a = KursorS_arch*(RETINA+1);

		KursorS_arch = KursorS;
	}
	else
	{
		if ((x0_bak > -1) && (y0_bak > -1))
		{
			;
		}
		else
		{
			KursorS_arch = KursorS;

		}
	}

	if ((SolidHatchPattern == 1) && (hatch_cursor == TRUE))
	{
		if (cursor_pattern)
		{
			//putimage_preserve(x0_bak - 1, y0_bak - cur_d_a, cursor_bmp_v, COPY_PUT);
			destroy_bitmap(cursor_pattern);
			cursor_pattern = NULL;
		}
		cursor_pattern = create_bitmap(cursor_icon_size, cursor_icon_size);
		if (cursor_pattern_bitmap)
		{
			if (current_cursor_fx != copy_cursor_pattern_fx)
			{
				strncpy(&solid_pattern_library_item.pattern_name, &SolidHatchPatternName, 31);
				solid_pattern_library_item.pattern_name[31] = '\0';
				Copy_cursor_pattern_bitmap(&solid_pattern_library_item);
				copy_cursor_pattern_fx = current_cursor_fx;
			}

			//putimage_preserve(x0 + d10, y0 + d10 + A5pYk , cursor_pattern_bitmap, COPY_PUT);
			//draw_trans_sprite(screen, cursor_pattern_bitmap, x0 + d10, y0 + d10 + A5pYk);  //this could be used when patterns would get transparency, what is possible using SOLID_PATTERN.flag
			draw_sprite(screen, cursor_pattern_bitmap, x0 + d10, y0 + d10 + A5pYk);
		}
	}
	else if ((SolidHatch == 1) && (hatch_cursor == TRUE))
	{
		if (cursor_pattern)
		{
			//putimage_preserve(x0_bak - 1, y0_bak - cur_d_a, cursor_bmp_v, COPY_PUT);
			destroy_bitmap(cursor_pattern);
			cursor_pattern = NULL;
		}
		cursor_pattern = create_bitmap(cursor_icon_size, cursor_icon_size);
		if (cursor_fill_bitmap)
		{
			if ((LiniaG.kolor != copy_cursor_fill_color) || (copy_SolidHatchTranslucency != SolidHatchTranslucency) || (copy_kolory_paper != kolory.paper))
			{
				clear_to_color(cursor_fill_bitmap, palette_color[GetColorAC(LiniaG.kolor)]);

				copy_cursor_fill_color = LiniaG.kolor;
				copy_SolidHatchTranslucency = SolidHatchTranslucency;
				copy_kolory_paper = kolory.paper;
			}
		}
		else
		{
			cursor_fill_bitmap = create_system_bitmap(cursor_icon_size, cursor_icon_size);

			clear_to_color(cursor_fill_bitmap, palette_color[GetColorAC(LiniaG.kolor)]);

			copy_cursor_fill_color = LiniaG.kolor;
			copy_SolidHatchTranslucency = SolidHatchTranslucency;
			copy_kolory_paper = kolory.paper;
		}

		set_trans_blender(0, 0, 0, (int)SolidHatchTranslucency);

		draw_trans_sprite(screen, cursor_fill_bitmap, x0 + d10, y0 + d10 + A5pYk);
		set_mode_solid();
	}

	if (INTL_CURSOR)
	{

        okno_all();

        A=0;
        A1=0;


        for (int xx = x1 - (int)(cur_d*sk_x); xx < x1 + (int)(cur_d*sk_x); xx++)
		{
			int yy = y1;
            //check if bitmap pixel is visible
            if ((xx>pXp1) && (xx<pXk1) && (yy>pYk1) && (yy<pYp1))
            {
                color = _getpixel32(screen, xx, yy);
                r = getr(color);
                g = getg(color);
                b = getb(color);

                A1 += 1.0f;
                background_r = (background_r * A + (float)r) / A1;
                background_g = (background_g * A + (float)g) / A1;
                background_b = (background_b * A + (float)b) / A1;
                A = A1;
            }
		}



        for (int yy = y1 - cur_d; yy < y1 + cur_d; yy++)
		{
			int xx=x1;
            //check if bitmap pixel is visible
            if ((xx>pXp1) && (xx<pXk1) && (yy>pYk1) && (yy<pYp1))
            {
                color = _getpixel32(screen, xx, yy);
                r = getr(color);
                g = getg(color);
                b = getb(color);

                A1 += 1.0f;
                background_r = (background_r * A + (float)r) / A1;
                background_g = (background_g * A + (float)g) / A1;
                background_b = (background_b * A + (float)b) / A1;
                A = A1;
            }
		}
	}

    okno_r();

	x0_bak = x0; y0_bak = y0;

	if ((KursorS == 0) || (KursorS == 100))
	{
		if (INTL_CURSOR)
		{
			brightness_b = 0.299f * background_r + 0.587f * background_g + 0.114f * background_b;
			color_r = getr(cursor_color);
			color_g = getg(cursor_color);
			color_b = getb(cursor_color);
			brightness_c = 0.299f * (float)color_r + 0.587f * (float)color_g + 0.114f * (float)color_b;

			if (fabsf(brightness_c - brightness_b) < BRIGHTNESS_DIFF)
			{

				set_cur_color(0xFFFFFF - palette_color[kolory.cur]);
				//__gr_color = color;  //??????
				cursor_color = cursor_color0 = 0xFFFFFF - palette_color[kolory.cur];
			}
		}


		xpp = x0 - d30;
		xkk = x0 + d30;
		ypp = y0 - d30;
		ykk = y0 + d30;

#ifdef ALLEGRO5  //maybe always ???
        kursor05 = d30;
        kursor01 = d10;
#endif
        //central cross
        //it was for Allegro 4
#ifndef ALLEGRO5
        hline(screen, xpp, y0, xkk, cursor_color);
        vline(screen, x0, ypp, ykk, cursor_color);
#else
        LINE(x0 - kursor05, y0, x0 + kursor05, y0);
        LINE(x0, y0 - kursor05, x0, y0 + kursor05);
#endif

        if (KursorS == 100)
		{
			kursor05 = d30;
			kursor01 = d10;
			//surrounding cross hor
			LINE(x0 - kursor05, y0 - 1, x0 - kursor01, y0 - 1);
			LINE(x0 + kursor01, y0 - 1, x0 + kursor05, y0 - 1);
			LINE(x0 - kursor05, y0 + 1, x0 - kursor01, y0 + 1);
			LINE(x0 + kursor01, y0 + 1, x0 + kursor05, y0 + 1);
			//surrounding cross ver
			LINE(x0 - 1, y0 - kursor05, x0 - 1, y0 - kursor01);
			LINE(x0 - 1, y0 + kursor01, x0 - 1, y0 + kursor05);
			LINE(x0 + 1, y0 - kursor05, x0 + 1, y0 - kursor01);
			LINE(x0 + 1, y0 + kursor01, x0 + 1, y0 + kursor05);
		}
		
		setwritemode(COPY_PUT);

#ifndef ALLEGRO5
        /*
		lpoints = xpp;
		for (ii = 0; ii < lpoints; ii += 3)
			putpixel(screen, ii, y0, cursor_color);
		lpoints = ypp;
		for (ii = 0; ii < lpoints; ii += 3)
			putpixel(screen, x0, ii, cursor_color);

		for (ii = xkk + 1; ii < pXk; ii += 3)
			putpixel(screen, ii, y0, cursor_color);

		for (ii = ykk + 1; ii < pYp; ii += 3)
			putpixel(screen, x0, ii, cursor_color);
        */

        linestyle(32);

        IPoint Ip[2];
        get_intersections(x0, y0, pXk, pYp, cosa, -sina, Ip);
        lineC(Ip[0].x, Ip[0].y, Ip[1].x, Ip[1].y);
        if (options1.uklad_izometryczny)
        {
            get_intersections(x0, y0, pXk, pYp, cosa1, -sina1, Ip);
        }
        else get_intersections(x0, y0, pXk, pYp, sina, cosa, Ip);
        lineC(Ip[0].x, Ip[0].y, Ip[1].x, Ip[1].y);

        ////lineC(0, y0, xpp, y0);
        ////lineC(xkk, y0, pXk, y0);

        ////lineC(x0, 0, x0, ypp);
        ////lineC(x0, ykk, x0, pYp);

#else
        linestyle(32);

        IPoint Ip[2];
        get_intersections(x0, y0, pXk, pYp, cosa, -sina, Ip);
        lineC(Ip[0].x, Ip[0].y, Ip[1].x, Ip[1].y);
        if (options1.uklad_izometryczny)
        {
            get_intersections(x0, y0, pXk, pYp, cosa1, -sina1, Ip);
        }
        else get_intersections(x0, y0, pXk, pYp, sina, cosa, Ip);
        lineC(Ip[0].x, Ip[0].y, Ip[1].x, Ip[1].y);

        ////lineC(0, y0, xpp, y0);
        ////lineC(xkk, y0, pXk, y0);

        ////lineC(x0, 0, x0, ypp);
        ////lineC(x0, ykk, x0, pYp);
#endif
	}
	else
	{
		if (KursorS > 100)
		{
			kursor05 = (KursorS - 100)*(RETINA+1) / 2;
			kursor01 = d10;
		}
		else
		{
			kursor05 = KursorS*(RETINA+1) / 2;
			kursor01 = 0;
		}


		if (INTL_CURSOR)
		{
			brightness_b = 0.299f * background_r + 0.587f * background_g + 0.114f * background_b;
			color_r = getr(cursor_color);
			color_g = getg(cursor_color);
			color_b = getb(cursor_color);
			brightness_c = 0.299f * (float)color_r + 0.587f * color_g + 0.114f * color_b;

			if ((fabs(brightness_c - brightness_b) < BRIGHTNESS_DIFF))
			{

				set_cur_color(0xFFFFFF - palette_color[kolory.cur]);
				//__gr_color = color; //????
				cursor_color = 0xFFFFFF - palette_color[kolory.cur];
			}

		}

		//central cross
		LINE(x0 - kursor05, y0, x0 + kursor05, y0);
		LINE(x0, y0 - kursor05, x0, y0 + kursor05);

		if (kursor01 > 0)
		{
			//surrounding cross hor
			LINE(x0 - kursor05, y0 - 1, x0 - kursor01, y0 - 1);
			LINE(x0 + kursor01, y0 - 1, x0 + kursor05, y0 - 1);
			LINE(x0 - kursor05, y0 + 1, x0 - kursor01, y0 + 1);
			LINE(x0 + kursor01, y0 + 1, x0 + kursor05, y0 + 1);
			//surrounding cross ver
			LINE(x0 - 1, y0 - kursor05, x0 - 1, y0 - kursor01);
			LINE(x0 - 1, y0 + kursor01, x0 - 1, y0 + kursor05);
			LINE(x0 + 1, y0 - kursor05, x0 + 1, y0 - kursor01);
			LINE(x0 + 1, y0 + kursor01, x0 + 1, y0 + kursor05);
		}
		
		setwritemode(COPY_PUT);
	}

    if (circle)  mycircle(x0, y0, 16);

    if (sel.akt && sel.gor) out_sel_on(X,Y);

	okno_all();

 }

void out_cur_on(double X, double Y)
{
    out_cur_on_factory(X, Y, 0);
}

void out_circle_cur_on(double X, double Y)
{
    out_cur_on_factory(X, Y, 1);
}


static void cur_off(double x, double y)
{
  out_cur_off(x, y);
}

static void cur_on(double x, double y)
{
	out_cur_on(x, y);
  pozycja_kursora();
  }

#define MAX_GRID_POINT 6000  //it's good number, even for huge multi-monitor screen
static int *gx, gxk, *gy, gyk ;


void DoneBuffers4(void)
{
  if (gx!=NULL) free (gx);
  if (gy!=NULL) free (gy);

}

static double GetFloatPrecission(double value, double precision)
{
	return (floor((value * pow(10, precision) + 0.5)) / pow(10, precision));
}

// Helper for positive modulo (handles negative values correctly)
static double positive_mod(double a, double b) {
    double m = fmod(a, b);
    return (m < 0) ? m + b : m;
}

static void grid_obl(void) {
    double i, ix0, iy0;
    double XXp, YYp, YYk;

    XXp = GetFloatPrecission(Xp, 10);
    YYp = GetFloatPrecission(Yp, 10);

    // Offsets from local origin (in drawing mm)
    double offset_x = positive_mod(localx, krok_g);
    double offset_y = positive_mod(localy, krok_g);

    if (!options1.uklad_izometryczny)
    {
        // Cartesian: Snap starts to match offset mod krok_g
        double dx_start = XXp - offset_x;
        ix0 = offset_x + krok_g * ceil(dx_start / krok_g);

        double dy_start = YYp - offset_y;
        iy0 = offset_y + krok_g * ceil(dy_start / krok_g);

        // Snap end for Y (largest <= Yk matching offset)
        double dy_end = Yk - offset_y;
        YYk = offset_y + krok_g * floor(dy_end / krok_g);

        // Limit check
        if ((Xk - ix0)/krok_g + 1 >= MAX_GRID_POINT || (YYk - iy0)/krok_g + 1 >= MAX_GRID_POINT) {
            gxk = gyk = -1; //MAX_GRID_POINT exceeded;
            return;
        }

        // X positions (vertical lines)
        for (i = ix0, gxk = 0; i <= Xk + 1e-6; i += krok_g, gxk++) {
            gx[gxk] = pikseleX(i);
        }

        // Y positions (horizontal lines)
        for (i = iy0, gyk = 0; i <= YYk + 1e-6; i += krok_g, gyk++) {
            gy[gyk] = pikseleY(i);
        }
    } else {
        // Isometric: Snap in iso space using local origin
        double local_cx = localx;  // drawing mm
        double local_cy = localy;

        double local_ix, local_iy;
        cartesian_to_isometric(local_cx, local_cy, &local_ix, &local_iy);

        double offset_ix = positive_mod(local_ix, krok_g);
        double offset_iy = positive_mod(local_iy, krok_g);

        // Window corners to iso (as before)
        double corner_cx[4] = {Xp, Xk, Xp, Xk};
        double corner_cy[4] = {Yp, Yp, Yk, Yk};
        double min_ix = INFINITY, max_ix = -INFINITY;
        double min_iy = INFINITY, max_iy = -INFINITY;
        for (int c = 0; c < 4; c++) {
            double ix, iy;
            cartesian_to_isometric(corner_cx[c], corner_cy[c], &ix, &iy);
            min_ix = fmin(min_ix, ix);
            max_ix = fmax(max_ix, ix);
            min_iy = fmin(min_iy, iy);
            max_iy = fmax(max_iy, iy);
        }

        // Snap ix_start >= min_ix, ≡ offset_ix mod krok_g
        double dix_start = min_ix - offset_ix;
        ix0 = offset_ix + krok_g * ceil(dix_start / krok_g);

        // ix_end <= max_ix
        double dix_end = max_ix - offset_ix;
        double ix_end = offset_ix + krok_g * floor(dix_end / krok_g);

        // iy_start >= min_iy, ≡ offset_iy mod krok_g
        double diy_start = min_iy - offset_iy;
        iy0 = offset_iy + krok_g * ceil(diy_start / krok_g);

        // iy_end <= max_iy
        double diy_end = max_iy - offset_iy;
        double iy_end = offset_iy + krok_g * floor(diy_end / krok_g);

        // Limit check (approx points)
        int approx_nx = (ix_end - ix0)/krok_g + 2;
        int approx_ny = (iy_end - iy0)/krok_g + 2;
        if (approx_nx * approx_ny >= MAX_GRID_POINT)
        {
            gxk = gyk = -1; //MAX_GRID_POINT exceeded;
            return;
        }

        // Generate points
        int count = 0;
        int ii=0;
        int jj=0;
        for (i = iy0; i <= iy_end + 1e-6; i += krok_g)
        {
            ii++;
            for (double j = ix0; j <= ix_end + 1e-6; j += krok_g)
            {
                jj++;
                double cx, cy;
                isometric_to_cartesian(j, i, &cx, &cy);
                if (cx >= Xp - 1e-6 && cx <= Xk + 1e-6 && cy >= Yp - 1e-6 && cy <= Yk + 1e-6)
                {
                    gx[count] = pikseleX(cx);
                    gy[count] = pikseleY(cy);
                    count++;
                }
            }
        }
        gxk = gyk = count;
    }
}


static int grid_kom=0;

void  grid_on(void)
{ int ix,iy;
  int *gxa,*gya;
  int gsizex, gsizey;
  
  if (!grid_) return;
  grid_obl();
  if (gxk == 0 || gyk == 0) return;
  //if (((!options1.uklad_izometryczny) && (((pikseleX(Xk)-pikseleX(Xp))/gxk<5) || (abs(pikseleY(Yk)-pikseleY(Yp))/gyk<5))) ||
  //   ((options1.uklad_izometryczny) && (((pikseleX(Xk)-pikseleX(Xp))/gxk<5) || (abs(pikseleY(Yk)-pikseleY(Yp))/gyk<2))))
  if (gxk<0 || gyk<0 || ((gxk > 1) && (fabs(*(gx + 1) - *(gx))<5)) || ((gyk > 2) && (fabs(*(gy + 1) - *(gy + 2))<5)))
  /*grid mesh less than 5 pxl*/
    {
      if (!grid_kom) ErrList(1);
      grid_kom=1;
      Set_Screenplay(second_screen);  //to return to second screen after the message
      return;
    }
  if(grid_kom)
  {
      komunikat(0); grid_kom=0;
      Set_Screenplay(second_screen);  //to return to second screen after the message
  }

  setcolor(kolory.border/*LIGHTGRAY*/);
  setwritemode(COPY_PUT);

  setlinestyle1(USERBIT_LINE, 0, 0);
  //setlinestyle(SOLID_LINE, 0, NORM_WIDTH);

  if (gxk > 1)
	  gsizex = max(min(fabs(*(gx + 1) - *(gx)) / 6 /*3*/, 10), 1);
  else
      gsizex = 10;

  if (gyk > 1)
  {
  	  if (*(gy) == *(gy + 1))
  	  {
	  	  if (gyk > 2)
	  	  	gsizey = max(min(fabs(*(gy + 1) - *(gy + 2)) / 6 /*3*/, 10), 1);
  	  	else gsizey=gsizex;
  	  }
	  else gsizey = max(min(fabs(*(gy) - *(gy + 1)) / 6 /*3*/, 10), 1);
  }
  else
      gsizey = 10;

  for (iy = 0, gya = gy; iy < gyk; gya++, iy++)
  {
      //printf("\ngya[%d]=%d\n",iy,*gya);
	  for (ix = 0, gxa = gx; ix < gxk; gxa++, ix++)
	  {
          //printf(" gxa[%d]=%d",ix,*gxa);
          //if (((ix+1)%10)==0) printf("\n");
		  if ((*gxa + gsizex) > pikseleX(Xk))
			  lineCuncut(*gxa - gsizex, *gya-pYk, *gxa, *gya-pYk);
		  else if ((*gxa + gsizex) < pikseleX(Xp))
			  lineCuncut(*gxa, *gya-pYk, *gxa + gsizex, *gya-pYk);
		  else
			  lineCuncut(*gxa - gsizex, *gya-pYk, *gxa + gsizex, *gya-pYk);
		  if ((*gya - gsizey) < pikseleY(Yk)-pYk)   //pYk
			  lineCuncut(*gxa, *gya-pYk, *gxa, *gya + gsizey-pYk);
		  else if ((*gya + gsizey) > pikseleY(Yp)+pYk)  //-pYk  //???
			  lineCuncut(*gxa, *gya - gsizey-pYk, *gxa, *gya-pYk);
		  else
			  lineCuncut(*gxa, *gya - gsizey-pYk, *gxa, *gya + gsizey-pYk);
	  }
  }
  setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
}

void  grid_off(void)
{
  if (grid_kom) return;
  redraw();
}


static void ramka(int opt)
{ int x1k[13]={-4,4,-4,-4,-6,0,6,14,20,14,-4,-4,-4,};
  int y1k[13]={4,4,-4,4,-14,-20,-14,-6,0,6,2,0,-2,};
  int x2k[13]={-4,4,14,14,0,6,-6,20,14,14,18,20,18,};
  int y2k[13]={-14,-14,-4,4,-20,-14,-14,0,6,-6,2,0,-2,};

  int x1g[13]={ -4,  4,-4,-4, -6,  0,  6,14,20,14, 2, 0,-2,};
  int y1g[13]={  4,  4,-4, 4,-14,-20,-14,-6, 0, 6, 4, 4, 4,};
  int x2g[13]={ -4,  4,14,14,  0,  6,- 6,20,14,14, 2, 0,-2,};
  int y2g[13]={-14,-14,-4, 4,-20,-14,-14, 0, 6,-6,-18,-20,-18,};

  int x1i[18]={0,0,-27,-25,-30,27,25,30,0,0,-17,0,17,-17,0,-17,0,17};
  int y1i[18]={0,0,-14,-16,-17,-14,-16,-17,0,-15,-25,-35,-25,-10,-20,-25,-35,-25};
  int x2i[18]={-26,26,-25,-30,-27,25,30,27,0,-17,0,17,0,0,17,-17,0,17};
  int y2i[18]={-15,-15,-16,-17,-14,-16,-17,-14,-15,-25,-35,-25,-15,-20,-10,-10,-20,-10};


  double angle_l, angle_lr, sinfi, cosfi;
  int ii, x11, y11, x12, y12;

  setcolor(kolory.border/*LIGHTGRAY*/);
  setwritemode(COPY_PUT);
  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);

if (opt==0)
 {
  if (Xp==Xmin && Yp==Ymin && (X_max<=X2d) && (Y_max<=Y2d))
   {
       rectangle(pXp, pYp-pYk, pikseleX(Xmax), pikseleY(Ymax)-pYk  ); ////
   }
  else
   {

       if (Xp==Xmin) LINE(pXp,pYp-pYk,pXp,pYk-pYk);
       if (fabs(Yp - Ymin)< 1.0E-10) LINE(pXp, pYp-pYk, pXk, pYp-pYk);
       if(Xp+X2d>=Xmax - 1.0E-10) LINE(pXk,pYp-pYk,pXk,pYk-pYk);
       if(Yp+Y2d>=Ymax - 1.0E-10) LINE(pXp,pYk-pYk,pXk,pYk-pYk);
   }
 }
  else
  {
   angle_l=get_angle_l();
   if (angle_l<0) angle_l+=360;

   if  (options1.uklad_izometryczny==1)
   {
       if (opt == 2) setcolor(kolory.paper);

       for (ii = 0; ii < 18; ii++)
       {
           LINE(x1i[ii] + 38, y1i[ii] + maxY - 8 /*36*/ - pYk, x2i[ii] + 38, y2i[ii] + maxY - 8 /*36*/ - pYk);
       }
   }
   else {
       if ((Check_if_Equal(0, angle_l) == FALSE) || (options1.uklad_geodezyjny == 1) || opt == 2) {

           if (opt == 2) setcolor(kolory.paper);

           if (Check_if_Equal(angle_l, 90) == TRUE) {
               sinfi = 1;
               cosfi = 0;
           } else if (Check_if_Equal(angle_l, 180) == TRUE) {
               sinfi = 0;
               cosfi = -1;
           } else if (Check_if_Equal(angle_l, 270) == TRUE) {
               sinfi = -1;
               cosfi = 0;
           } else {
               angle_lr = angle_l * 0.017453293;
               sinfi = sin(angle_lr);
               cosfi = cos(angle_lr);
           }

           for (ii = 0; ii < 13; ii++) {
               if (options1.uklad_geodezyjny == 0) {
                   x11 = x1k[ii] * cosfi + y1k[ii] * sinfi;
                   y11 = -x1k[ii] * sinfi + y1k[ii] * cosfi;
                   x12 = x2k[ii] * cosfi + y2k[ii] * sinfi;
                   y12 = -x2k[ii] * sinfi + y2k[ii] * cosfi;
               } else {
                   x11 = x1g[ii] * cosfi + y1g[ii] * sinfi;
                   y11 = -x1g[ii] * sinfi + y1g[ii] * cosfi;
                   x12 = x2g[ii] * cosfi + y2g[ii] * sinfi;
                   y12 = -x2g[ii] * sinfi + y2g[ii] * cosfi;
               }
               LINE(x11 + 22, y11 + maxY - 22 - pYk, x12 + 22, y12 + maxY - 22 - pYk);
           }
       }
   }
  }
}

void change_angle_l(double angle)
{   int ret;
    Set_Screenplay((BITMAP *)second_screen);
    ramka(2);
    ret = put_angle_l(angle);
    ramka(1);
    Set_Screenplay(screen);
    CUR_OFF(X, Y);
    CUR_ON(X, Y);
}

void Xk_Yk(void)
{ Xk=min(X2d+Xp,Xmax);
  Yk=min(Y2d+Yp,Ymax);
  pXk=pikseleX(Xk);
  pYk=pikseleY(Yk);

  temp_X0 = 0.5 - Xp * Ds_x ;
  temp_Y0 = 0.5 + y_pikseleY0 + Yp * Ds ;
}


void Xp_Yp(void)
{ double Xpl,Ypl;
  Xpl=X-Xd_;
  if((Xpl+X2d)>Xmax) Xpl=Xmax-X2d;
  if(Xpl<Xmin) Xpl=Xmin;
  Xp=Xpl;
  if(Xmax<=X2d) Xp=Xmin;
  Ypl=Y-Yd_;
  if((Ypl+Y2d)>Ymax) Ypl=Ymax-Y2d;
  if(Ypl<Ymin) Ypl=Ymin;
  Yp=Ypl;
  if(Ymax<=Y2d) Yp=Ymin;
  pXp=pikseleX(Xp);
  pYp=pikseleY(Yp);

  Xk_Yk();
}

void Xp_Yp_p(double xmin, double ymin)
{ double Xpl,Ypl;
    Xpl=X-Xd_;
    if((Xpl+X2d)>Xmax) Xpl=Xmax-X2d;
    if(Xpl<xmin) Xpl=xmin;
    Xp=Xpl;
    if(Xmax<=X2d) Xp=xmin; //Xmin;
    Ypl=Y-Yd_;
    if((Ypl+Y2d)>Ymax) Ypl=Ymax-Y2d;
    if(Ypl<Ymin) Ypl=Ymin;
    Yp=Ypl;
    if(Ymax<=Y2d) Yp=Ymin;
    pXp=pikseleX(Xp);
    pYp=pikseleY(Yp);
    Xk_Yk();
}

void Xp_Yp_move(double ddx, double ddy)
{
	global_pan_dx = pikseleX00(ddx);
	global_pan_dy = pikseleY00(ddy);
	Xp += ddx;
	Yp += ddy;
	pXp = pikseleX(Xp);
	pYp = pikseleY(Yp);
	X += ddx;
	Y += ddy;
	Xk_Yk();
}

void Xp_Yp_to_left(void)
{
	double Xpl;

  Xpl=X-Xd;
  if(Xpl<Xmin) Xpl=Xmin;
  global_pan_dx = pikseleX00(Xp - Xpl);
  Xp=Xpl;
  pXp=pikseleX(Xp);
  Xk_Yk();
}

void Xp_Yp_to_left_fix(void)
{
	double Xpl;
	double Xp0;

	Xp0 = Xp;
	Xpl = Xp - Xd;
	if (Xpl < Xmin) Xpl = Xmin;
	global_pan_dx = pikseleX00(Xp - Xpl);
	Xp = Xpl;
	X -= (Xp0 - Xp);
	pXp = pikseleX(Xp);
	Xk_Yk();
}

void Xp_Yp_to_right(void)
{
	double Xpl;

  Xpl=X-X2d+Xd;
  if(Xpl>(Xmax-X2d)) Xpl=Xmax-X2d;  
  global_pan_dx = pikseleX00(Xp - Xpl);
  Xp=Xpl;
  pXp=pikseleX(Xp);
  Xk_Yk();
}

void Xp_Yp_to_right_fix(void)
{
	double Xpl;
	double Xp0;

	Xp0 = Xp;
	Xpl = Xk - X2d + Xd;
	if (Xpl > (Xmax - X2d)) Xpl = Xmax - X2d;
	global_pan_dx = pikseleX00(Xp - Xpl);
	Xp = Xpl;
	X -= (Xp0 - Xp);
	pXp = pikseleX(Xp);
	Xk_Yk();
}

void Xp_Yp_to_down(void)
{
	double Ypl;

  Ypl=Y-Yd;
  if(Ypl<Ymin) Ypl=Ymin;
  global_pan_dy = pikseleY00(Yp - Ypl);
  Yp=Ypl;
  pYp=pikseleY(Yp);
  Xk_Yk();
}

void Xp_Yp_to_down_fix(void)
{
	double Ypl;
	double Yp0;

	Yp0 = Yp;
	Ypl = Yp - Yd;
	if (Ypl < Ymin) Ypl = Ymin;
	global_pan_dy = pikseleY00(Yp - Ypl);
	Yp = Ypl;
	Y -= (Yp0 - Yp);
	pYp = pikseleY(Yp);
	Xk_Yk();
}

void Xp_Yp_to_up(void)
{
	double Ypl;

  Ypl=Y-Y2d+Yd;
  if(Ypl>(Ymax-Y2d)) Ypl=Ymax-Y2d;
  global_pan_dy = pikseleY00(Yp - Ypl);
  Yp=Ypl;
  pYp=pikseleY(Yp);
  Xk_Yk();
}

void Xp_Yp_to_up_fix(void)
{
	double Ypl;
	double Yp0;

	Yp0 = Yp;
	Ypl = Yk - Y2d + Yd;
	if (Ypl > (Ymax - Y2d)) Ypl = Ymax - Y2d;
	global_pan_dy = pikseleY00(Yp - Ypl);
	Yp = Ypl;
	Y -= (Yp0 - Yp);
	pYp = pikseleY(Yp);
	Xk_Yk();
}

void Xp0_Yp0(double xp, double yp)
{
  Xp=xp;
  Yp=yp;
  pXp=pikseleX(Xp);
  pYp=pikseleY(Yp);
  Xk_Yk();
}

void X_Y(void)
{
  X = Xp + min (Xd, (Xmax - Xp) / 2);
  Y = Yp + min (Yd, (Ymax - Yp) / 2);
  pXp=pikseleX(Xp);
  pYp=pikseleY(Yp);
  Xk_Yk();
}

void Get_View_Coord (int * x1, int *y1, int *x2, int *y2)
/*-----------------------------------------------------*/
{
  *x1 = 0 ;
  *y1 = 0 ;
  *x2 = pXk - pXp ;
  *y2 = pYp - pYk ;
}

static int Inside(long x,long x1,long x2)
{ int ix;
  ix=0;
  if (x<x1) ix=-1;
  else if (x>x2) ix=1;
  return ix;
}


int lineCuncut(long x1, long y1, long x2, long y2)
{
	long X1 = 0, Y1 = 0, X2 = pXk - pXp, Y2 = pYp - pYk;
	int color_cur;

	int l_e, iii;
	int pp0;
	int pp[14];
	double delxxx, delyyy, delxxxx, delyyyy, delxx, delyy, nlr, lxy;
	int nl;
	float xxp1, yyp1, xxk1, yyk1, xxp2, yyp2, xxk2, yyk2, xxp3, yyp3, xxk3, yyk3, xxp4, yyp4, xxk4, yyk4;
	float xxp5, yyp5, xxk5, yyk5, xxp6, yyp6, xxk6, yyk6, xxp7, yyp7, xxk7, yyk7;
	int xxp, yyp, xxk, yyk;
	linesettingstype_ lineinfo;
	int de_x, de2_x, de_y, de2_y;
	int i;

	getlinesettings1(&lineinfo);
	l_e = 1;
	if (lineinfo.linestyle == USERBIT_LINE)
	{
		switch (lineinfo.thickness)
		{
		case 0: l_e = l_e_bc[lineinfo.upattern];  //bardzo cienka
			for (i = 0; i < (l_e - 1); i++) pp[i] = pp_bc[lineinfo.upattern][i];
			pp0 = pp0_bc[lineinfo.upattern];
			color_cur = palette_color[getcolor()];
			break;
		case 1: l_e = l_e_c[lineinfo.upattern];   //cienka
			for (i = 0; i < (l_e - 1); i++) pp[i] = pp_c[lineinfo.upattern][i];
			pp0 = pp0_c[lineinfo.upattern];
			break;
		case 2: //gruba
		case 3: //bardzo gruba
		case 4: l_e = l_e_g[lineinfo.upattern]; //najgrubsza
			for (i = 0; i < (l_e - 1); i++) pp[i] = pp_g[lineinfo.upattern][i];
			pp0 = pp0_g[lineinfo.upattern];
			break;
		default:l_e = l_e_g[lineinfo.upattern]; //gruba
			for (i = 0; i < (l_e - 1); i++) pp[i] = pp_g[lineinfo.upattern][i];
			pp0 = pp0_g[lineinfo.upattern];
			break;
		}
	}
	else
		if (lineinfo.linestyle == DOTTED_LINE)
		{
			l_e = 2;
			pp[0] = 1;
			pp0 = 4;
		}
		else
		{
			l_e = 1;
		}


	if (lineinfo.thickness >= 3)
	{
		delxxx = fabs(x2 - x1);
		delyyy = fabs(y2 - y1);
		if (delxxx > delyyy)
		{
			de_x = 0;
			de2_x = 0;
			de_y = 1;
			de2_y = 2;
		}
		else
		{
			de_x = 1;
			de2_x = 2;
			de_y = 0;
			de2_y = 0;
		}
	}

	if (l_e == 1)
	{
		LINE(x1, y1, x2, y2);
		if (lineinfo.thickness >= 3)
		{
			LINE(x1 - de_x, y1 - de_y, x2 - de_x, y2 - de_y);
			LINE(x1 + de_x, y1 + de_y, x2 + de_x, y2 + de_y);
			if (lineinfo.thickness == 4)
			{
				LINE(x1 - de2_x, y1 - de2_y, x2 - de2_x, y2 - de2_y);
				LINE(x1 + de2_x, y1 + de2_y, x2 + de2_x, y2 + de2_y);
			}
		}
	}
	else
	{
		delxxx = x2 - x1;
		delyyy = y2 - y1;
		lxy = sqrt((delxxx*delxxx) + (delyyy*delyyy));
		if (lxy < pp0)  //dlugosc odcinka jest mniejsza od wzorca
		{
			l_e = 2;
			pp[0] = 1;
			pp[1] = 3;
			pp0 = 4;
		}
		nlr = lxy / pp0;
		if (fabs(nlr) > 0.0000000001)
		{
			delxx = delxxx / nlr;
			delyy = delyyy / nlr;
		}
		nl = (lxy / pp0);
		if (nl > 0)
		{
			delxxxx = delxxx / lxy;
			delyyyy = delyyy / lxy;
			for (iii = 1; iii <= (nl + 1); iii++)
			{
				xxp1 = x1 + (iii - 1)*delxx;
				yyp1 = y1 + (iii - 1)*delyy;
				xxk1 = xxp1 + delxxxx * (pp[0]);
				yyk1 = yyp1 + delyyyy * (pp[0]);
				xxp = (int)xxp1;
				yyp = (int)yyp1;
				xxk = (int)xxk1;
				yyk = (int)yyk1;

				if (iii > nl) //ostatni odcinek
				{
					//rysowany jest odcinek <xxp,yyp>,<x2,y2>
					x1 = xxp;
					y1 = yyp;
					if (lineinfo.thickness < 2)
					{
						l_e = 2;
						pp[0] = 1;
						pp[1] = 2;
						pp0 = 3;
					}
					else
					{
						l_e = 2;
						pp[0] = 2;
						pp[1] = 3;
						pp0 = 4;
					}
					delxxx = x2 - x1;
					delyyy = y2 - y1;
					lxy = sqrt((delxxx*delxxx) + (delyyy*delyyy));
					if (lxy < pp0)  //dlugosc ostatniego odcinka jest mniejsza od nowego wzorca
					{
						if (lineinfo.thickness == 0)
						{
							putpixel_(xxp, yyp, color_cur);
							putpixel_(x2, y2, color_cur);
						}
						else LINE(xxp, yyp, x2, y2);

						if (lineinfo.thickness >= 3)
						{
							LINE(xxp - de_x, yyp - de_y, x2 - de_x, y2 - de_y);
							LINE(xxp + de_x, yyp + de_y, x2 + de_x, y2 + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, x2 - de2_x, y2 - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, x2 + de2_x, y2 + de2_y);
							}
						}
					}
					else
					{
						nlr = lxy / pp0;
						if (fabs(nlr) > 0.0000000001)
						{
							delxx = delxxx / nlr;
							delyy = delyyy / nlr;
						}
						nl = (lxy / pp0);
						if (nl > 0)
						{
							delxxxx = delxxx / lxy;
							delyyyy = delyyy / lxy;
							for (iii = 1; iii <= (nl + 1); iii++)
							{
								xxp1 = x1 + (iii - 1)*delxx;
								yyp1 = y1 + (iii - 1)*delyy;
								xxk1 = xxp1 + delxxxx * (pp[0]);
								yyk1 = yyp1 + delyyyy * (pp[0]);
								xxp = (int)xxp1;
								yyp = (int)yyp1;
								xxk = (int)xxk1;
								yyk = (int)yyk1;
								if (iii > nl) //ostatni odcinek
								{
									if (lineinfo.thickness == 0)
									{
										putpixel_(xxp, yyp, color_cur);
										putpixel_(x2, y2, color_cur);
									}
									else LINE(xxp, yyp, x2, y2);
									if (lineinfo.thickness >= 3)
									{
										LINE(xxp - de_x, yyp - de_y, x2 - de_x, y2 - de_y);
										LINE(xxp + de_x, yyp + de_y, x2 + de_x, y2 + de_y);
										if (lineinfo.thickness == 4)
										{
											LINE(xxp - de2_x, yyp - de2_y, x2 - de2_x, y2 - de2_y);
											LINE(xxp + de2_x, yyp + de2_y, x2 + de2_x, y2 + de2_y);
										}
									}
								}
								else
								{
									if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
									if (lineinfo.thickness >= 3)
									{
										LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
										LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
										if (lineinfo.thickness == 4)
										{
											LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
											LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
										}
									}
								}
							}
						}
					}
				}
				else
				{
					if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
					if (lineinfo.thickness >= 3)
					{
						LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
						LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
						if (lineinfo.thickness == 4)
						{
							LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
							LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
						}
					}
					if (l_e > 2)
					{
						xxp2 = xxk1 + delxxxx * (pp[1]);
						yyp2 = yyk1 + delyyyy * (pp[1]);
						xxk2 = xxp2 + delxxxx * (pp[2]);
						yyk2 = yyp2 + delyyyy * (pp[2]);
						xxp = (int)xxp2;
						yyp = (int)yyp2;
						xxk = (int)xxk2;
						yyk = (int)yyk2;
						if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
						if (lineinfo.thickness >= 3)
						{
							LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
							LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
							}
						}
					}
					if (l_e > 4)
					{
						xxp3 = xxk2 + delxxxx * (pp[3]);
						yyp3 = yyk2 + delyyyy * (pp[3]);
						xxk3 = xxp3 + delxxxx * (pp[4]);
						yyk3 = yyp3 + delyyyy * (pp[4]);
						xxp = (int)xxp3;
						yyp = (int)yyp3;
						xxk = (int)xxk3;
						yyk = (int)yyk3;
						if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
						if (lineinfo.thickness >= 3)
						{
							LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
							LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
							}
						}
					}
					if (l_e > 6)
					{
						xxp4 = xxk3 + delxxxx * (pp[5]);
						yyp4 = yyk3 + delyyyy * (pp[5]);
						xxk4 = xxp4 + delxxxx * (pp[6]);
						yyk4 = yyp4 + delyyyy * (pp[6]);
						xxp = (int)xxp4;
						yyp = (int)yyp4;
						xxk = (int)xxk4;
						yyk = (int)yyk4;
						if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
						if (lineinfo.thickness >= 3)
						{
							LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
							LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
							}
						}
					}
					if (l_e > 8)
					{
						xxp5 = xxk4 + delxxxx * (pp[7]);
						yyp5 = yyk4 + delyyyy * (pp[7]);
						xxk5 = xxp5 + delxxxx * (pp[8]);
						yyk5 = yyp5 + delyyyy * (pp[8]);
						xxp = (int)xxp5;
						yyp = (int)yyp5;
						xxk = (int)xxk5;
						yyk = (int)yyk5;
						if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
						if (lineinfo.thickness >= 3)
						{
							LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
							LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
							}
						}
					}
					if (l_e > 10)
					{
						xxp6 = xxk5 + delxxxx * (pp[9]);
						yyp6 = yyk5 + delyyyy * (pp[9]);
						xxk6 = xxp6 + delxxxx * (pp[10]);
						yyk6 = yyp6 + delyyyy * (pp[10]);
						xxp = (int)xxp6;
						yyp = (int)yyp6;
						xxk = (int)xxk6;
						yyk = (int)yyk6;
						if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
						if (lineinfo.thickness >= 3)
						{
							LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
							LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
							}
						}
					}
					if (l_e > 12)
					{
						xxp7 = xxk6 + delxxxx * (pp[11]);
						yyp7 = yyk6 + delyyyy * (pp[11]);
						xxk7 = xxp7 + delxxxx * (pp[12]);
						yyk7 = yyp7 + delyyyy * (pp[12]);
						xxp = (int)xxp7;
						yyp = (int)yyp7;
						xxk = (int)xxk7;
						yyk = (int)yyk7;
						if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
						if (lineinfo.thickness >= 3)
						{
							LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
							LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
							}
						}
					}
				}

			}
		}
		else
		{
			LINE(x1, y1, x2, y2);
			if (lineinfo.thickness >= 3)
			{
				LINE(x1 - de_x, y1 - de_y, x2 - de_x, y2 - de_y);
				LINE(x1 + de_x, y1 + de_y, x2 + de_x, y2 + de_y);
				if (lineinfo.thickness == 4)
				{
					LINE(x1 - de2_x, y1 - de2_y, x2 - de2_x, y2 - de2_y);
					LINE(x1 + de2_x, y1 + de2_y, x2 + de2_x, y2 + de2_y);
				}
			}
		}
	}
	return 1;
}


static int insideint(int x, int x1, int x2)
/*----------------------------------------------*/
{
	int ix;
	ix = 0;
	if (x < x1) ix = -1;
	else if (x > x2) ix = 1;
	return ix;
}

int clip_line(int *x1, int *y1, int *x2, int *y2, int xx1, int yy1, int xx2, int yy2)
/*---------------------------------------------------------------------------------*/
{
	int ix1, ix2, iy1, iy2;
	double  xx, yy;
	double a, a1;

	if (TRUE == Check_if_Equal(*x1, *x2))
	{
		*x1 = *x2;		//dla unikniecia "float"
	}
	if (TRUE == Check_if_Equal(*y1, *y2))
	{
		*y1 = *y2;
	}

	ix1 = insideint(*x1, xx1, xx2);
	ix2 = insideint(*x2, xx1, xx2);
	iy1 = insideint(*y1, yy1, yy2);
	iy2 = insideint(*y2, yy1, yy2);
	if (ix1*ix2 == 1 || iy1 * iy2 == 1) return 0;
	if (*x1 != *x2)  a = (float)(*y2 - *y1) / (float)(*x2 - *x1);
	if (*y1 != *y2) a1 = (float)(*x2 - *x1) / (float)(*y2 - *y1);
	if (ix1)
	{
		xx = ix1 > 0 ? xx2 : xx1;
		*y1 = *y1 + a * (xx - *x1);
		*x1 = xx;
		iy1 = insideint(*y1, yy1, yy2);
		if (iy1*iy2 == 1) return 0;
	}
	if (iy1)
	{
		yy = iy1 > 0 ? yy2 : yy1;
		*x1 = *x1 + a1 * (yy - *y1);
		*y1 = yy;
		ix1 = insideint(*x1, xx1, xx2);
		if (ix1) return 0;
	}
	if (ix2)
	{
		xx = ix2 > 0 ? xx2 : xx1;
		*y2 = *y2 + a * (xx - *x2);
		*x2 = xx;
		iy2 = insideint(*y2, yy1, yy2);
	}
	if (iy2)
	{
		yy = iy2 > 0 ? yy2 : yy1;
		*x2 = *x2 + a1 * (yy - *y2);
		*y2 = yy;
	}
	return 1;
}


BOOL get_pattern_count(void)
{
	return pattern_count;
}

int get_pattern_offset(void)
{
	return pattern_offset;
}

void set_pattern_count(BOOL p_count)
{
	pattern_count = p_count;
}

void set_pattern_offset(int offs)
{
	pattern_offset = offs;
}

int lineC(long x1, long y1, long x2, long y2)
{
  long X1=0,Y1=0,X2=pXk-pXp,Y2=pYp-pYk;
  int ix1,ix2,iy1,iy2;
  long xx,yy;
  double a,a1;
  int color_cur;
  
  int l_e,iii, l_e_cc;
  int pp0;
  int pp[256], pp_org[248], pp_org_c[68];
  double delxxx,delyyy,delxxxx,delyyyy,delxx,delyy,nlr,lxy; 
  int nl;
  float xxp1, yyp1, xxk1, yyk1, xxp2, yyp2, xxk2, yyk2;
  int xxp,yyp,xxk,yyk;
  linesettingstype_ lineinfo;
  int de_x, de2_x, de_y, de2_y;
  int i, ip, ch_pattern, ii, j, il;
  int sum, sum0, sum1;
  long xx1, xx2, yy1, yy2;
  int xxp_, yyp_;
  int lx_;

  int scale_factor=1;
  double ds_factor;

  ds_factor = skala * DokladnoscF;

  if (ds_factor < 5) scale_factor = 1;
  else if (ds_factor < 10) scale_factor = 2;
  else  scale_factor = 4;

  ix1=Inside(x1,X1,X2);
  ix2=Inside(x2,X1,X2);
  iy1=Inside(y1,Y1,Y2);
  iy2=Inside(y2,Y1,Y2);
  if (ix1*ix2==1 || iy1*iy2==1) 
	  return 0;
  if(x1!=x2)  a=((double)(y2-y1))/(x2-x1);
  if(y1!=y2) a1=((double)(x2-x1))/(y2-y1);
  if (ix1)
   { xx=ix1>0 ? X2 : X1;
     y1=y1+(long)floor(.5+a*(xx-x1));
     x1=xx;
	 iy1=Inside(y1,Y1,Y2);
	 if (iy1*iy2==1) 
		 return 0;
   }
  if (iy1)
   { yy=iy1>0 ? Y2 : Y1;
     x1=x1+(long)floor(.5+a1*(yy-y1));
     y1=yy;
	 ix1=Inside(x1,X1,X2);
     if (ix1) 
		 return 0;
   }
  if (ix2)
   { xx=ix2>0 ? X2 : X1;
     y2=y2+(long)floor(.5+a*(xx-x2));
     x2=xx;
     iy2=Inside(y2,Y1,Y2);
   }
  if (iy2)
   { yy=iy2>0 ? Y2 : Y1;
     x2=x2+(long)floor(.5+a1*(yy-y2));
	 y2=yy;
   }

  getlinesettings1(&lineinfo);
  
  l_e=1;

  //maybe here was mistake ... ???
#ifndef ALLEGRO5
  color_cur= palette_color[getcolor()];  //////???
#else
  color_cur= get_cur_color();  //////???
#endif

  //RETINA:  lineinfo.thickness could be doubled

  if (lineinfo.linestyle == USERBIT_LINE)
  {
	  switch (lineinfo.thickness)
	  {
	  case 0: 
	  case 1:

		  if (lineinfo.upattern == 0)
		  {
			  l_e = l_e_c[lineinfo.upattern]; //cienka
			  for (i = 0; i < (l_e); i++) pp_org[i] = pp_c[lineinfo.upattern][i];
			  pp0 = pp0_c[lineinfo.upattern];
		  }
		  else
		  {
			  l_e_cc = l_e_g[lineinfo.upattern]; //gruba
			  for (i = 0; i < l_e_cc; i++) pp_org_c[i] = pp_g[lineinfo.upattern][i] * scale_factor;


			  pp0 = 0;

			  //generating very thin and thin pattern
			  l_e = 0;

			  for (i = 0; i < (l_e_cc); i++)
			  {
				  if (!(i % 2))  //odd  - line
				  {
					  j = 0;
					  while (j < pp_org_c[i])
					  {
						  pp_org[l_e++] = 1;
						  pp_org[l_e++] = 2;
						  pp0 += 3;
						  j += 3;
					  }
					  pp_org[l_e++] = 1;
					  pp0 += 1;
				  }
				  else  //even - space
				  {
					  pp_org[l_e++] = pp_org_c[i];
					  pp0 += pp_org_c[i];
				  }

			  }
		  }


		  break;

	  case 2: //gruba//pattern_offset = 0;
	  case 3: //bardzo gruba
	  case 4: l_e = l_e_g[lineinfo.upattern]; //najgrubsza
		  for (i = 0; i < (l_e); i++) pp_org[i] = pp_g[lineinfo.upattern][i] * scale_factor;
		  pp0 = pp0_g[lineinfo.upattern] * scale_factor;
		  break;
	  default:l_e = l_e_g[lineinfo.upattern]; //gruba
		  for (i = 0; i < (l_e); i++) pp_org[i] = pp_g[lineinfo.upattern][i] * scale_factor;
		  pp0 = pp0_g[lineinfo.upattern] * scale_factor;
		  break;
	  }


	  if ((global_mode == 1) || (!pattern_count)) pattern_offset = 0;

	  ////// shifting the pattern, we have dp shifting vector
	  if (pattern_offset > 0)
	  {

		  sum = 0;
		  ip = 0;
		  while ((sum<pattern_offset) && (ip < (l_e)))  //(l_e-1)
		  {
			  sum += pp_org[ip++];
		  }

		  if (ip > 0)
		  {
			  sum0 = 0;
			  for (i = 0; i < (l_e - 1); i++) sum0 += pp_org[i];
			  pp_org[l_e-1] = pp0 - sum0;
			  if ((ip % 2)!=0)  //dash
			  {
				  j = 0;
				  pp[j++] = sum - pattern_offset;
				  for (i = ip; i < l_e; i++) pp[j++] = pp_org[i];
				  
				  for (ii = 0; ii < ip - 1; ii++) pp[j++] = pp_org[ii];
				  pp[j++] = pp_org[ii] - pp[0];
				  pp[j++] = 0;
				  l_e+=2;
			  }
			  else
			  {
				  j = 0;
				  pp[j++] = 0;
				  sum1 = 0;
				  pp[j++] = sum - pattern_offset;
				  for (i = ip; i < l_e; i++) pp[j++] = pp_org[i];
				  for (ii = 0; ii < ip - 1; ii++) pp[j++] = pp_org[ii];
				  l_e += 2;
				  sum1 = 0;
				  for (i = 0; i < (l_e-1); i++) sum1 += pp[i];
				  pp[j++] = pp0 - sum1;
				  
			  }
		  }
		  
		  else  //just regular pattern
			  for (i = 0; i < (l_e); i++)
			  {
				  pp[i] = pp_org[i];
				  sum += pp[i];
			  }
          
	  }
	  
	  else
	  {
		  sum = 0;
		  for (i = 0; i < (l_e); i++)
		  {
			  pp[i] = pp_org[i];
			  sum += pp[i];
		  }
	  }

  }
  else 
  if (lineinfo.linestyle==DOTTED_LINE)
  {
  l_e=2;
  pp[0]=1;
  pp0=4;
  }
  else
  {
   l_e=1;
  }

   if (lineinfo.thickness>=3)
   {
    delxxx=fabs(x2-x1);
    delyyy=fabs(y2-y1);
    if (delxxx>delyyy)
    {
    de_x=0;
    de2_x=0;
    de_y=1;
    de2_y=2;
    }
    else
    {
    de_x=1;
    de2_x=2;
    de_y=0;
    de2_y=0;
    }
   } 
  
   if (l_e==1) 
   {
    LINE(x1,y1,x2,y2);
    if (lineinfo.thickness>=3) 
    {
     LINE(x1-de_x,y1-de_y,x2-de_x,y2-de_y);
     LINE(x1+de_x,y1+de_y,x2+de_x,y2+de_y);
     if (lineinfo.thickness==4)
      {
       LINE(x1-de2_x,y1-de2_y,x2-de2_x,y2-de2_y);
       LINE(x1+de2_x,y1+de2_y,x2+de2_x,y2+de2_y);
      }
    }
   } 
    else
    {
     delxxx=x2-x1;
     delyyy=y2-y1;
     lxy=sqrt((delxxx*delxxx)+(delyyy*delyyy));

     nlr=lxy/pp0;
     if (fabs(nlr)>0.00000001)  //0.0000000001)
       {
         delxx=delxxx/nlr;
         delyy=delyyy/nlr;
		 nl = (lxy / pp0);
       } 
	 else nl = -1;
     if (nl>=0) 
      { 
       delxxxx=delxxx/lxy;
       delyyyy=delyyy/lxy;
       for (iii=1; iii<=(nl+1); iii++)
       {
		xxp1=x1+(iii-1)*delxx;
		yyp1=y1+(iii-1)*delyy;
		xxk1=xxp1+delxxxx*(pp[0]);
		yyk1=yyp1+delyyyy*(pp[0]);
		xxp=(int)xxp1;
		yyp=(int)yyp1;
		xxk=(int)xxk1;
		yyk=(int)yyk1;

		if ((iii > nl) && (!pattern_count)) //last segment
		{
			
			//rysowany jest odcinek <xxp,yyp>,<x2,y2>
			x1 = xxp;
			y1 = yyp;

			ch_pattern = 1;  //14-01-2021

			if (ch_pattern == 1)
			{
				if (lineinfo.thickness < 2)
				{

					l_e = 2;
					pp[0] = 1;
					pp[1] = 2;
					pp0 = 3;


				}
				else
				{

					//continuous line

					l_e = 1;
					pp[0] = 1;
					pp0 = 1;
				}
			}
			delxxx = x2 - x1;
			delyyy = y2 - y1;
			lxy = sqrt((delxxx*delxxx) + (delyyy*delyyy));
			if (lxy < pp0)  //dlugosc ostatniego odcinka jest mniejsza od nowego wzorca
			{
				if (lineinfo.thickness == 0)
				{
					putpixel_(xxp, yyp, color_cur);
					putpixel_(x2, y2, color_cur);
				}
				else LINE(xxp, yyp, x2, y2);

				if (lineinfo.thickness >= 3)
				{
					LINE(xxp - de_x, yyp - de_y, x2 - de_x, y2 - de_y);
					LINE(xxp + de_x, yyp + de_y, x2 + de_x, y2 + de_y);
					if (lineinfo.thickness == 4)
					{
						LINE(xxp - de2_x, yyp - de2_y, x2 - de2_x, y2 - de2_y);
						LINE(xxp + de2_x, yyp + de2_y, x2 + de2_x, y2 + de2_y);
					}
				}
			}
			else
			{
				nlr = lxy / pp0;
				if (fabs(nlr) > 0.0000000001)
				{
					delxx = delxxx / nlr;
					delyy = delyyy / nlr;
				}
				nl = (lxy / pp0);
				if (nl > 0)
				{
					delxxxx = delxxx / lxy;
					delyyyy = delyyy / lxy;
					for (iii = 1; iii <= (nl + 1); iii++)
					{
						xxp1 = x1 + (iii - 1)*delxx;
						yyp1 = y1 + (iii - 1)*delyy;
						xxk1 = xxp1 + delxxxx * (pp[0]);
						yyk1 = yyp1 + delyyyy * (pp[0]);
						xxp = (int)xxp1;
						yyp = (int)yyp1;
						xxk = (int)xxk1;
						yyk = (int)yyk1;
						if ((iii > nl) && (ch_pattern == 1))//ostatni odcinek
						{
							if (lineinfo.thickness == 0)
							{
								putpixel_(xxp, yyp, color_cur);
								putpixel_(x2, y2, color_cur);
							}
							else LINE(xxp, yyp, x2, y2);

							if (lineinfo.thickness >= 3) 
							{
								LINE(xxp - de_x, yyp - de_y, x2 - de_x, y2 - de_y);
								LINE(xxp + de_x, yyp + de_y, x2 + de_x, y2 + de_y);
								if (lineinfo.thickness == 4)
								{
									LINE(xxp - de2_x, yyp - de2_y, x2 - de2_x, y2 - de2_y);
									LINE(xxp + de2_x, yyp + de2_y, x2 + de2_x, y2 + de2_y);
								}
							}
						}
						else
						{
							if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur);
							else LINE(xxp, yyp, xxk, yyk);
							if (lineinfo.thickness >= 3)
							{
								LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
								LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
								if (lineinfo.thickness == 4)
								{
									LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
									LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
								}
							}
						}
					}
				}
			}
			
		}
		else 
		{
		 if (x1 < x2) { xx1 = x1; xx2 = x2; }
		 else { xx1 = x2; xx2 = x1; };
		 if (y1 < y2) { yy1 = y1; yy2 = y2; }
		 else { yy1 = y2; yy2 = y1; };

		 xxp_ = xxp;
		 yyp_ = yyp;

		 if (iii > nl)  //last segment
		 {
			if (!clip_line(&xxp, &yyp, &xxk, &yyk, xx1, yy1, xx2, yy2)) goto break__;
		 }

		 if (((xxp != xxk) || (yyp != yyk)) || (lineinfo.thickness<2)) // (lineinfo.upattern == 0))
		 {
			 if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur);
			 else LINE(xxp, yyp, xxk, yyk);
			 if (lineinfo.thickness >= 3)
			 {
				 LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
				 LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
				 if (lineinfo.thickness == 4)
				 {
					 LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
					 LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
				 }
			 }
		 }

         for (il=1; il<(l_e-1); il+=2)
		 {
			xxp2=xxk1+delxxxx*(pp[il]);
			yyp2=yyk1+delyyyy*(pp[il]);
			xxk2=xxp2+delxxxx*(pp[il+1]);
			yyk2=yyp2+delyyyy*(pp[il+1]);
			xxp=(int)xxp2;
			yyp=(int)yyp2;
			xxk=(int)xxk2;
			yyk=(int)yyk2;
			xxk1 = xxk2;
			yyk1 = yyk2;


				if (iii > nl)
				{
					if (!clip_line(&xxp, &yyp, &xxk, &yyk, xx1, yy1, xx2, yy2)) goto break__;
				}


			if (((xxp != xxk) || (yyp != yyk)) || (lineinfo.thickness < 2)) //(lineinfo.upattern == 0))
			{

				if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur);
				else LINE(xxp, yyp, xxk, yyk);
				if (lineinfo.thickness >= 3)
				{
					LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
					LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
					if (lineinfo.thickness == 4)
					{
						LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
						LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
					}
				}
			}
			
		 }
		 
		 //at the end we know where pattern was finished
		}
	
       }

break__:
		
		if ((iii > nl) && (pattern_count) && (lineinfo.upattern > 0))
		{
			lx_ = (int)(sqrt((x2 - xxp_) * (x2 - xxp_) + (y2 - yyp_) * (y2 - yyp_)));

			pattern_offset = pattern_offset + lx_;
			
			pattern_offset = pattern_offset % pp0;
		}

     }
     else  
     {
      LINE(x1,y1,x2,y2);
      if (lineinfo.thickness>=3) 
      {
       LINE(x1-de_x,y1-de_y,x2-de_x,y2-de_y);
       LINE(x1+de_x,y1+de_y,x2+de_x,y2+de_y);
       if (lineinfo.thickness==4)
        {
         LINE(x1-de2_x,y1-de2_y,x2-de2_x,y2-de2_y);
         LINE(x1+de2_x,y1+de2_y,x2+de2_x,y2+de2_y);
        }
      }
     } 
    }
  return 1;
}



int lineC0(long x1, long y1, long x2, long y2)
{
	long X1 = 0, Y1 = 0, X2 = pXk - pXp, Y2 = pYp - pYk;
	int ix1, ix2, iy1, iy2;
	long xx, yy;
	double a, a1;
	int color_cur;
	int l_e, iii;
	int pp0;
	int pp[14];
	double delxxx, delyyy, delxxxx, delyyyy, delxx, delyy, nlr, lxy;
	int nl;
	float xxp1, yyp1, xxk1, yyk1, xxp2, yyp2, xxk2, yyk2, xxp3, yyp3, xxk3, yyk3, xxp4, yyp4, xxk4, yyk4;
	float xxp5, yyp5, xxk5, yyk5, xxp6, yyp6, xxk6, yyk6, xxp7, yyp7, xxk7, yyk7;
	int xxp, yyp, xxk, yyk;
	linesettingstype_ lineinfo;
	int de_x, de2_x, de_y, de2_y;
	int i;


	ix1 = Inside(x1, X1, X2);
	ix2 = Inside(x2, X1, X2);
	iy1 = Inside(y1, Y1, Y2);
	iy2 = Inside(y2, Y1, Y2);
	if (ix1*ix2 == 1 || iy1 * iy2 == 1)
		return 0;
	if (x1 != x2)  a = ((double)(y2 - y1)) / (x2 - x1);
	if (y1 != y2) a1 = ((double)(x2 - x1)) / (y2 - y1);
	if (ix1)
	{
		xx = ix1 > 0 ? X2 : X1;
		y1 = y1 + (long)floor(.5 + a * (xx - x1));
		x1 = xx;
		iy1 = Inside(y1, Y1, Y2);
		if (iy1*iy2 == 1)
			return 0;
	}
	if (iy1)
	{
		yy = iy1 > 0 ? Y2 : Y1;
		x1 = x1 + (long)floor(.5 + a1 * (yy - y1));
		y1 = yy;
		ix1 = Inside(x1, X1, X2);
		if (ix1)
			return 0;
	}
	if (ix2)
	{
		xx = ix2 > 0 ? X2 : X1;
		y2 = y2 + (long)floor(.5 + a * (xx - x2));
		x2 = xx;
		iy2 = Inside(y2, Y1, Y2);
	}
	if (iy2)
	{
		yy = iy2 > 0 ? Y2 : Y1;
		x2 = x2 + (long)floor(.5 + a1 * (yy - y2));
		y2 = yy;
	}


	getlinesettings1(&lineinfo);

	l_e = 1;

	color_cur = palette_color[getcolor()];  //////???

	if (lineinfo.linestyle == USERBIT_LINE)
	{
		switch (lineinfo.thickness)
		{
		case 0: l_e = l_e_bc[lineinfo.upattern];  //bardzo cienka
			for (i = 0; i < (l_e - 1); i++) pp[i] = pp_bc[lineinfo.upattern][i];
			pp0 = pp0_bc[lineinfo.upattern];
			break;
		case 1: l_e = l_e_c[lineinfo.upattern];   //cienka
			for (i = 0; i < (l_e - 1); i++) pp[i] = pp_c[lineinfo.upattern][i];
			pp0 = pp0_c[lineinfo.upattern];
			break;
		case 2: //gruba
		case 3: //bardzo gruba
		case 4: l_e = l_e_g[lineinfo.upattern]; //najgrubsza
			for (i = 0; i < (l_e - 1); i++) pp[i] = pp_g[lineinfo.upattern][i];
			pp0 = pp0_g[lineinfo.upattern];
			break;
		default:l_e = l_e_g[lineinfo.upattern]; //gruba
			for (i = 0; i < (l_e - 1); i++) pp[i] = pp_g[lineinfo.upattern][i];
			pp0 = pp0_g[lineinfo.upattern];
			break;
		}
	}
	else
		if (lineinfo.linestyle == DOTTED_LINE)
		{
			l_e = 2;
			pp[0] = 1;
			pp0 = 4;
		}
		else
		{
			l_e = 1;
		}


	if (lineinfo.thickness >= 3) 
	{
		delxxx = fabs(x2 - x1);
		delyyy = fabs(y2 - y1);
		if (delxxx > delyyy)
		{
			de_x = 0;
			de2_x = 0;
			de_y = 1;
			de2_y = 2;
		}
		else
		{
			de_x = 1;
			de2_x = 2;
			de_y = 0;
			de2_y = 0;
		}
	}

	if (l_e == 1)
	{
		LINE(x1, y1, x2, y2);
		if (lineinfo.thickness >= 3) 
		{
			LINE(x1 - de_x, y1 - de_y, x2 - de_x, y2 - de_y);
			LINE(x1 + de_x, y1 + de_y, x2 + de_x, y2 + de_y);
			if (lineinfo.thickness == 4)
			{
				LINE(x1 - de2_x, y1 - de2_y, x2 - de2_x, y2 - de2_y);
				LINE(x1 + de2_x, y1 + de2_y, x2 + de2_x, y2 + de2_y);
			}
		}
	}
	else
	{
		delxxx = x2 - x1;
		delyyy = y2 - y1;
		lxy = sqrt((delxxx*delxxx) + (delyyy*delyyy));
		if (lxy < pp0)  //dlugosc odcinka jest mniejsza od wzorca
		{
			l_e = 2;
			pp[0] = 1;
			pp[1] = 3;
			pp0 = 4;
		}
		nlr = lxy / pp0;
		if (fabs(nlr) > 0.0000000001)
		{
			delxx = delxxx / nlr;
			delyy = delyyy / nlr;
		}
		nl = (lxy / pp0);
		if (nl > 0)
		{
			delxxxx = delxxx / lxy;
			delyyyy = delyyy / lxy;
			for (iii = 1; iii <= (nl + 1); iii++)
			{
				xxp1 = x1 + (iii - 1)*delxx;
				yyp1 = y1 + (iii - 1)*delyy;
				xxk1 = xxp1 + delxxxx * (pp[0]);
				yyk1 = yyp1 + delyyyy * (pp[0]);
				xxp = (int)xxp1;
				yyp = (int)yyp1;
				xxk = (int)xxk1;
				yyk = (int)yyk1;

				if (iii > nl) //ostatni odcinek
				{
					//rysowany jest odcinek <xxp,yyp>,<x2,y2>
					x1 = xxp;
					y1 = yyp;
					if (lineinfo.thickness < 2)
					{
						l_e = 2;
						pp[0] = 1;
						pp[1] = 2;
						pp0 = 3;
					}
					else
					{
						l_e = 2;
						pp[0] = 2;
						pp[1] = 3;
						pp0 = 4;
					}
					delxxx = x2 - x1;
					delyyy = y2 - y1;
					lxy = sqrt((delxxx*delxxx) + (delyyy*delyyy));
					if (lxy < pp0)  //dlugosc ostatniego odcinka jest mniejsza od nowego wzorca
					{
						if (lineinfo.thickness == 0)
						{
							putpixel_(xxp, yyp, color_cur);
							putpixel_(x2, y2, color_cur);
						}
						else LINE(xxp, yyp, x2, y2);

						if (lineinfo.thickness >= 3) 
						{
							LINE(xxp - de_x, yyp - de_y, x2 - de_x, y2 - de_y);
							LINE(xxp + de_x, yyp + de_y, x2 + de_x, y2 + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, x2 - de2_x, y2 - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, x2 + de2_x, y2 + de2_y);
							}
						}
					}
					else
					{
						nlr = lxy / pp0;
						if (fabs(nlr) > 0.0000000001)
						{
							delxx = delxxx / nlr;
							delyy = delyyy / nlr;
						}
						nl = (lxy / pp0);
						if (nl > 0)
						{
							delxxxx = delxxx / lxy;
							delyyyy = delyyy / lxy;
							for (iii = 1; iii <= (nl + 1); iii++)
							{
								xxp1 = x1 + (iii - 1)*delxx;
								yyp1 = y1 + (iii - 1)*delyy;
								xxk1 = xxp1 + delxxxx * (pp[0]);
								yyk1 = yyp1 + delyyyy * (pp[0]);
								xxp = (int)xxp1;
								yyp = (int)yyp1;
								xxk = (int)xxk1;
								yyk = (int)yyk1;
								if (iii > nl) //ostatni odcinek
								{
									if (lineinfo.thickness == 0)
									{
										putpixel_(xxp, yyp, color_cur);
										putpixel_(x2, y2, color_cur);
									}
									else LINE(xxp, yyp, x2, y2);
									if (lineinfo.thickness >= 3) 
									{
										LINE(xxp - de_x, yyp - de_y, x2 - de_x, y2 - de_y);
										LINE(xxp + de_x, yyp + de_y, x2 + de_x, y2 + de_y);
										if (lineinfo.thickness == 4)
										{
											LINE(xxp - de2_x, yyp - de2_y, x2 - de2_x, y2 - de2_y);
											LINE(xxp + de2_x, yyp + de2_y, x2 + de2_x, y2 + de2_y);
										}
									}
								}
								else
								{
									if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
									if (lineinfo.thickness >= 3) 
									{
										LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
										LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
										if (lineinfo.thickness == 4)
										{
											LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
											LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
										}
									}
								}
							}
						}
					}
				}
				else
				{
					if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur);
					else LINE(xxp, yyp, xxk, yyk);
					if (lineinfo.thickness >= 3) 
					{
						LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
						LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
						if (lineinfo.thickness == 4)
						{
							LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
							LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
						}
					}
					if (l_e > 2)
					{
						xxp2 = xxk1 + delxxxx * (pp[1]);
						yyp2 = yyk1 + delyyyy * (pp[1]);
						xxk2 = xxp2 + delxxxx * (pp[2]);
						yyk2 = yyp2 + delyyyy * (pp[2]);
						xxp = (int)xxp2;
						yyp = (int)yyp2;
						xxk = (int)xxk2;
						yyk = (int)yyk2;
						if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
						if (lineinfo.thickness >= 3)
						{
							LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
							LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
							}
						}
					}
					if (l_e > 4)
					{
						xxp3 = xxk2 + delxxxx * (pp[3]);
						yyp3 = yyk2 + delyyyy * (pp[3]);
						xxk3 = xxp3 + delxxxx * (pp[4]);
						yyk3 = yyp3 + delyyyy * (pp[4]);
						xxp = (int)xxp3;
						yyp = (int)yyp3;
						xxk = (int)xxk3;
						yyk = (int)yyk3;
						if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
						if (lineinfo.thickness >= 3) 
						{
							LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
							LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
							}
						}
					}
					if (l_e > 6)
					{
						xxp4 = xxk3 + delxxxx * (pp[5]);
						yyp4 = yyk3 + delyyyy * (pp[5]);
						xxk4 = xxp4 + delxxxx * (pp[6]);
						yyk4 = yyp4 + delyyyy * (pp[6]);
						xxp = (int)xxp4;
						yyp = (int)yyp4;
						xxk = (int)xxk4;
						yyk = (int)yyk4;
						if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
						if (lineinfo.thickness >= 3) 
						{
							LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
							LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
							}
						}
					}
					if (l_e > 8)
					{
						xxp5 = xxk4 + delxxxx * (pp[7]);
						yyp5 = yyk4 + delyyyy * (pp[7]);
						xxk5 = xxp5 + delxxxx * (pp[8]);
						yyk5 = yyp5 + delyyyy * (pp[8]);
						xxp = (int)xxp5;
						yyp = (int)yyp5;
						xxk = (int)xxk5;
						yyk = (int)yyk5;
						if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
						if (lineinfo.thickness >= 3) 
						{
							LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
							LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
							}
						}
					}
					if (l_e > 10)
					{
						xxp6 = xxk5 + delxxxx * (pp[9]);
						yyp6 = yyk5 + delyyyy * (pp[9]);
						xxk6 = xxp6 + delxxxx * (pp[10]);
						yyk6 = yyp6 + delyyyy * (pp[10]);
						xxp = (int)xxp6;
						yyp = (int)yyp6;
						xxk = (int)xxk6;
						yyk = (int)yyk6;
						if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
						if (lineinfo.thickness >= 3) 
						{
							LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
							LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
							}
						}
					}
					if (l_e > 12)
					{
						xxp7 = xxk6 + delxxxx * (pp[11]);
						yyp7 = yyk6 + delyyyy * (pp[11]);
						xxk7 = xxp7 + delxxxx * (pp[12]);
						yyk7 = yyp7 + delyyyy * (pp[12]);
						xxp = (int)xxp7;
						yyp = (int)yyp7;
						xxk = (int)xxk7;
						yyk = (int)yyk7;
						if (lineinfo.thickness == 0) putpixel_(xxp, yyp, color_cur); else LINE(xxp, yyp, xxk, yyk);
						if (lineinfo.thickness >= 3) 
						{
							LINE(xxp - de_x, yyp - de_y, xxk - de_x, yyk - de_y);
							LINE(xxp + de_x, yyp + de_y, xxk + de_x, yyk + de_y);
							if (lineinfo.thickness == 4)
							{
								LINE(xxp - de2_x, yyp - de2_y, xxk - de2_x, yyk - de2_y);
								LINE(xxp + de2_x, yyp + de2_y, xxk + de2_x, yyk + de2_y);
							}
						}
					}
				}

			}
		}
		else
		{
			LINE(x1, y1, x2, y2);
			if (lineinfo.thickness >= 3)
			{
				LINE(x1 - de_x, y1 - de_y, x2 - de_x, y2 - de_y);
				LINE(x1 + de_x, y1 + de_y, x2 + de_x, y2 + de_y);
				if (lineinfo.thickness == 4)
				{
					LINE(x1 - de2_x, y1 - de2_y, x2 - de2_x, y2 - de2_y);
					LINE(x1 + de2_x, y1 + de2_y, x2 + de2_x, y2 + de2_y);
				}
			}
		}
	}
	return 1;
}


char *Get_dane0_DATA(int dwg_no)
{
	return dane0_Data[dwg_no];
}

void zwolnienie_pamieci_forever(void)
{
	DelDaneSize = 32768; //16384;
	if (dane0_Data[DRAWING_NUMBER] != NULL)
	{
		DisposeBuffer(dane0_Data[DRAWING_NUMBER]);
		dane0_Data[DRAWING_NUMBER] = NULL;
	}
	Free_Buf_Font_Mem(TRUE);
	Destroy_All_ctx();
}

void zwolnienie_pamieci(void)
{
  DelDaneSize=32768; //16384;
  if (dane0_Data[DRAWING_NUMBER] != NULL)
  {
    DisposeBuffer (dane0_Data[DRAWING_NUMBER]) ;
	dane0_Data[DRAWING_NUMBER] = NULL;
  }
  Free_Buf_Font_Mem (TRUE) ;
  InitDane();
  regen_ctx=TRUE;
  Destroy_All_ctx();
}

void zwolnienie_pamieci_1(void)
{
  DelDaneSize=524288;
  if (dane0_Data[DRAWING_NUMBER] != NULL)
  {
    DisposeBuffer (dane0_Data[DRAWING_NUMBER]) ;
	dane0_Data[DRAWING_NUMBER] = NULL;
  }
  Free_Buf_Font_Mem (TRUE) ;
  InitDane();
}

int linestyle(int typ)
{ int grubosc1,typ1;
  grubosc1 = (typ & 224) / 32;
  if (grubosc1 == 7) grubosc1 = 2; //TEMPORARY Hatch outline
  typ1 = (typ & 31);
  setlinestyle2 (USERBIT_LINE, typ1, grubosc1) ;
  return grubosc1;
}

int linestyle_solid(int typ)
{ int grubosc1,typ1;
  grubosc1 = typ - 1;
  typ1 = 0;
  if (grubosc1 == 5)
   {
    setlinestyle2 (4, 1, 1) ;
    return 1;
   }
  else if (grubosc1 == 6)
    {
     setlinestyle2 (4, 1, 1) ;
     return 1;
    }
    else
     {
       setlinestyle2 (USERBIT_LINE, typ1, grubosc1) ;
       return grubosc1;
     }
}

int linestyle_xor(int typ)
{ int grubosc1,typ1;
  grubosc1 = (typ & 224) / 32;
  if (grubosc1 == 7) grubosc1 = 2; //TEMPORARY Hatch outline

  typ1 = (typ & 31);
  if (grubosc1 == 0) grubosc1 = 1;
  setlinestyle2 (USERBIT_LINE, typ1, grubosc1) ;
  return grubosc1;
}

int GetColorAC(int color)
{ int colorAC;
  int transf[]={  0, 57 /*12*/, 14, 10, 11, 9, 64 /*13*/, 15,  8,  4,  6,  2,  3,  1,  5,  7,  16 /*0*/};
  if (color <= 16)
  {
   colorAC=transf[color];
   if (colorAC == kolory.paper) 
    { 
	   if (colorAC == 15) //white
	   {
		   if (global_mode == XOR_PUT) colorAC = 7;
		   else colorAC = 16;
	   }
	   else  if (colorAC == 16) //black
	   {
		   if (global_mode == XOR_PUT) colorAC = 7;
		   else colorAC = 15;
	   }
	   else  if (colorAC == 0) //black
	   {
		   if (global_mode == XOR_PUT) colorAC = 7;
		   else colorAC = 15;
	   }
	else 
	   if (colorAC < 8) colorAC+=8; 
	   else if (colorAC == 8) colorAC = 7;
	   else if (colorAC > 8) colorAC-=8;
    } 
   return colorAC;
  } 
  if (color > 255)
  {
    color = 15 ;
  }
  if (color == kolory.paper)
   {
     color = ( 255 - color);
   }
  return color;
}

int GetColorACPlt(int color)
{
	int colorAC;
	int transf[] = { 0, 57 /*12*/, 14, 10, 11, 9, 64 /*13*/, 15,  8,  4,  6,  2,  3,  1,  5,  7,  16 /*0*/ };
	if (color <= 16)
	{
		colorAC = transf[color];
		return colorAC;
	}
	return color;
}



int GetColorWin(unsigned char color)
{ int colorAC;
  int transf[]={  0, 12 /*57*/ /*12*/, 14, 10, 11, 9, 64 /*13*/, 16 /*15*/,  8,  4,  6,  2,  3,  1,  5,  7,  16 /*0*/};
  if (color <= 16)
  {
   colorAC=transf[color];
   return colorAC;
  } 
  if (color > 255)
  {
	  color = 0; //15;
  }
  return color;
}

int GetColorWinPCX(unsigned char color)
{ int colorAC;
  int transf[]={  0, 57 /*12*/, 14, 10, 11, 9, 64 /*13*/, 15,  8,  4,  6,  2,  3,  1,  5,  7,  16 /*0*/};
  if (color <= 16)
  {
   colorAC=transf[color];

   return colorAC;
  } 
  if (color > 255)
  {
    color = 15 ;
  }

  return color;
}


int GetColorAC1(int color)
{ int colorAC;
  int transf[]={0, 57 /*12*/, 14, 10, 11, 9, 64/*13*/, 15, 8,4,6,2,3,1,5,7,16 /*0*/};
  if (color <= 16)
  {
   colorAC=transf[color];
   return colorAC;
  } 
  if (color > 255)
  {
    color = 15 ;
  }
  return color;
}

static int KMCY(int kolor0)
//-------------------------
{ int kolor1=0;
  if (kolor0 & 1) kolor1 += 1;
  if (kolor0 & 2) kolor1 += 4;
  if (kolor0 & 4) kolor1 += 2;
  if (kolor0 & 8) kolor1 += 8;
  return kolor1;
}

static int YMCK(int kolor0)
/*----------------------*/
{ int kolor1=0;
  if (kolor0 & 1) kolor1 += 8;
  if (kolor0 & 2) kolor1 += 4;
  if (kolor0 & 4) kolor1 += 2;
  if (kolor0 & 8) kolor1 += 1;
  return kolor1;
}

static int MCYK(int kolor0)
/*----------------------*/
{ int kolor1=0;
  if (kolor0 & 1) kolor1 += 8;
  if (kolor0 & 2) kolor1 += 2;
  if (kolor0 & 4) kolor1 += 1;
  if (kolor0 & 8) kolor1 += 4;
  return kolor1;
}


int GetColorHP0(int color0, int color_mode, int bank_kolorow, int horizontal_bit)
{
 int transf_KCMY[] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                            2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
                            4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
                            6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                            8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
                            10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
                            12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
                            14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
                           };
 int transf_KCMY1[]={0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,1,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,
                            0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,
                            0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,
                            0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,
                            0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,
                            0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,
                            0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,
                            0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,0,2,4,6,8,10,12,14,
                           };
                           
 int transf_KCMY2[]={0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,1,4,4,4,4,4,4,4,8,8,8,8,8,8,8,8,
                            0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,4,4,4,4,4,4,4,4,8,8,8,8,8,8,8,8,
                            0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,4,4,4,4,4,4,4,4,8,8,8,8,8,8,8,8,
                            0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,4,4,4,4,4,4,4,4,8,8,8,8,8,8,8,8,
                            0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,4,4,4,4,4,4,4,4,8,8,8,8,8,8,8,8,
                            0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,4,4,4,4,4,4,4,4,8,8,8,8,8,8,8,8,
                            0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,4,4,4,4,4,4,4,4,8,8,8,8,8,8,8,8,
                            0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,4,4,4,4,4,4,4,4,8,8,8,8,8,8,8,8,
                           };
                           
int nr_colorhp[]={  0,
                         222,152,164,  8, 43, 82, 16,240,214,252,189,  8,113, 80,176, 16,
                         128, 24,  4,132,156,132,132,156,132,156,152, 28,144,192,130,192, 
                          10, 64, 64, 88,216,144,196,220, 68,  2,  2,  2, 64, 66, 18, 48,
                          72, 48, 18, 80, 18, 82, 84,150,214,198, 84, 66, 82, 64, 80, 64,
                          48,112, 83,112, 66, 90, 94,222, 92, 30, 80, 98, 65, 10, 35,106,
                          74, 65, 67,210,202,124, 68, 88,149,135,142,120,138,200, 65, 17,
                          96, 65, 12, 29,188,140,188,157, 29,133,129,133,129,160,  1, 32,
                           8, 12,136,184,160,164, 36, 60,184, 56, 25, 37,  5,  8,  1, 40,
                          42, 73, 97,170,129,160,180,135,176,162,116,100,  3, 96, 11, 35,
                         104, 72, 10,112, 74, 54,138,204,254,221, 70, 80,115, 10, 65, 17,
                         104, 49, 73,105,113,106,110,240, 62,221,  7, 38, 49, 81,105, 35,
                          73, 41, 40,  9, 14,140,164,153, 60, 56, 36, 37,160, 33, 40, 40,
                          32,  8, 44, 36,157,140,185, 60,184, 12, 25, 56,164,  1,  8,  1,
                           5, 25, 13, 12, 36,136, 29,157,  5,188, 56, 56, 32, 32,  9, 40,
                          41, 41, 11, 43, 49,108,109,141,119,107,105,104, 33, 41, 73, 93,
                         255,231,255,127, 53, 39,238,251,231,145,162,131,176,224,  7,  0, 
                        };
int tab_colorhp[]={ 1,  
                           1,  1,  2,  2,  1,  3,  1,  3,  1,  2,  2,  3,  3,  1,  6,  1,
                           6,  6,  6,  6,  6,  4,  3,  3,  1,  1,  2,  3,  3,  4,  4,  6,
                           6,  5,  3,  1,  5,  4,  1,  1,  1,  1,  4,  2,  2,  4,  6,  4,
                           2,  2,  4,  4,  3,  4,  6,  2,  2,  1,  2,  6,  6,  1,  6,  4,
                           1,  6,  4,  4,  5,  5,  1,  5,  2,  2,  3,  4,  2,  4,  2,  4,
                           4,  4,  4,  6,  6,  6,  3,  3,  4,  4,  4,  4,  6,  6,  6,  6,
                           3,  3,  6,  6,  6,  4,  4,  4,  4,  4,  4,  6,  6,  1,  6,  6,
                           1,  5,  6,  6,  4,  3,  4,  3,  3,  3,  3,  2,  6,  6,  4,  4,
                           4,  4,  4,  6,  3,  3,  3,  3,  4,  4,  6,  6,  4,  4,  4,  4, 
                           1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  5,  5,  4,  5,  5,  5,
                           5,  5,  5,  1,  5,  5,  5,  5,  4,  1,  1,  1,  1,  5,  2,  1,
                           1,  1,  1,  1,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  2,  6,
                           2,  5,  2,  2,  2,  2,  2,  4,  4,  4,  4,  4,  4,  2,  4,  1,
                           4,  6,  2,  2,  1,  1,  1,  6,  2,  1,  2,  5,  5,  5,  2,  5,
                           2,  5,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
                           1,  1,  5,  5,  2,  2,  6,  6,  6,  6,  6,  6,  6,  6,  6,  1,
                         };
                           

 static int tab_bit[]={0,0,32,64,96,128,224,0,0};
 
 int color1;
 int color2;
 int tabela1;
 int color;

 if ( color0==0) color=7; else color=color0;

 color1=nr_colorhp[color];

 if ( color_mode==0 )
  {
   tabela1=tab_bit[tab_colorhp[color]];
   if (bank_kolorow==0) 
     {
       return (transf_KCMY[color1] | tabela1); 
     }  
      else if (bank_kolorow==1) 
        {
          return (transf_KCMY1[color1] | tabela1);
        }  
       else 
         {
           return (transf_KCMY2[color1] | tabela1);
         } 
   }
  else
   if ( color_mode==1 )
    {
    tabela1=tab_bit[tab_colorhp[color]];
    if (bank_kolorow==0) 
     {
       color2=YMCK(transf_KCMY[color1]);
       return (color2 | tabela1); 
     }  
      else if (bank_kolorow==1) 
        {
          color2=YMCK(transf_KCMY1[color1]);
          return (color2 | tabela1);
        }  
       else 
         {
           color2=YMCK(transf_KCMY2[color1]);
           return (color2 | tabela1);
         } 
    }
     else
       if ( color_mode==2 )
	{

         tabela1=tab_bit[tab_colorhp[color]];
         if (bank_kolorow==0) 
          {
           color2=MCYK(transf_KCMY[color1]);
           return (color2 | tabela1); 
          }  
           else if (bank_kolorow==1) 
            {
             color2=MCYK(transf_KCMY1[color1]);
             return (color2 | tabela1);
            }  
             else 
              {
               color2=MCYK(transf_KCMY2[color1]);
               return (color2 | tabela1);
              } 
	 }
	 else
	    {

              tabela1=tab_bit[tab_colorhp[color]];
              if (bank_kolorow==0) 
               {
                return (transf_KCMY[color1] | tabela1); 
               }  
                else if (bank_kolorow==1) 
                {
                 return (transf_KCMY1[color1] | tabela1);
                }  
                 else 
                  {
                   return (transf_KCMY2[color1] | tabela1);
                  } 
            }
}


int tab_tab16[17]={0,1,11,3,9,6,16,8,14,2,12,4,10,5,15,7,13};
int tab_tab36[37]={0,1,22,19,4,14,35,32,17,9,30,27,12,2,23,20,5,7,28,25,10,15,36,33,18,8,29,26,11,3,24,21,6,13,34,31,16};

int GetColorHP(int color0, int color_mode, int bank_kolorow, int horizontal_bit, int liczba_pol)
{
 
 int color1;
 int color2;
 int color;
 int c255, m255, y255;
 int c_255_, m_255_, y_255_;
 double c_255_d, m_255_d, y_255_d;
 int _k_, _c_, _m_, _y_;
 int red, green, blue, gray;
 double gray_d;

 if ( color0==0) color=7; else color=color0;

 color2=0;

 if (color==7) return 1;

 if ( color_mode==3 )   //skala szrosci
 {
  _k_=0;
  _c_=0;
  _m_=0;
  _y_=0;

  color1=GetColorAC1(color);

  getcolor_RGB(&red, &green, &blue, GetColorAC1(color));

  gray_d=64-(0.3*(double)red + 0.59*(double)green +0.11*(double)blue);  //0..64

  if (liczba_pol==36)
  {
    gray_d /= 1.78;
    gray_d += 0.5;
  }
   else   //16
    {
      gray_d /= 4.0;
      gray_d += 0.5;
    }

  gray=(int)gray_d;

 if (tab_tab36[((bank_kolorow+1)*(horizontal_bit+1))]<=gray) _k_=1;

 }
 else //kolory
 {
 setrgb2cmyk(&c255, &m255, &y255, GetColorAC1(color));

 if (liczba_pol==36)
  {
    c_255_d=((255.0-c255) / 7.0)+0.5;
    m_255_d=((255.0-m255) / 7.0)+0.5;
    y_255_d=((255.0-y255) / 7.0)+0.5;

    c_255_=(int)c_255_d;
    m_255_=(int)m_255_d;
    y_255_=(int)y_255_d;

    _k_=0;
    _c_=0;
    _m_=0;
    _y_=0;

    if (tab_tab36[((bank_kolorow+1)*(horizontal_bit+1))]<=c_255_) _c_=1;
    if (tab_tab36[((bank_kolorow+1)*(horizontal_bit+1))]<=m_255_) _m_=1;
    if (tab_tab36[((bank_kolorow+1)*(horizontal_bit+1))]<=y_255_) _y_=1;

  }
  else
    {
      c_255_d=((255.0-c255) / 16.0)+0.5;
      m_255_d=((255.0-m255) / 16.0)+0.5;
      y_255_d=((255.0-y255) / 16.0)+0.5;

      c_255_=(int)c_255_d;
      m_255_=(int)m_255_d;
      y_255_=(int)y_255_d;

      _k_=0;
      _c_=0;
      _m_=0;
      _y_=0;

      if (tab_tab16[((bank_kolorow+1)*(horizontal_bit+1))]<=c_255_) _c_=1;
      if (tab_tab16[((bank_kolorow+1)*(horizontal_bit+1))]<=m_255_) _m_=1;
      if (tab_tab16[((bank_kolorow+1)*(horizontal_bit+1))]<=y_255_) _y_=1;
    }
 }

 if (_k_==1) color2+=1;
 if (_c_==1) color2+=2;
 if (_m_==1) color2+=4;
 if (_y_==1) color2+=8;

 if ( color_mode==0 )
  {
    return color2;
  }
  else
   if ( color_mode==1 )
    {
       color2=YMCK(color2);
       return color2;
     }  
     else if ( color_mode==2 )
   	{
        color2=MCYK(color2);
        return color2;
      }
 	  else
	    {
        // kolor=0;
        return color2;
       }
}


void SetColorAC(int color)
{ setcolor(GetColorAC(color));
}


void my_rect_ (int x1, int y1, int x2, int y2)
{
	setwritemode(XOR_PUT);
	SetColorAC(15);
	LINE (x1, y1, x2, y1) ;
	LINE (x2, y1, x2, y2) ;
	LINE (x2, y2, x1, y2) ;
	LINE (x1, y2, x1, y1) ;

	LINE (x1+1, y1+1, x2-1, y1+1) ;
	LINE (x2-1, y1+1, x2-1, y2-1) ;
	LINE (x2-1, y2-1, x1+1, y2-1) ;
	LINE (x1+1, y2-1, x1+1, y1+1) ;

	LINE (x1+2, y1+2, x2-2, y1+2) ;
	LINE (x2-2, y1+2, x2-2, y2-2) ;
	LINE (x2-2, y2-2, x1+2, y2-2) ;
	LINE (x1+2, y2-2, x1+2, y1+2) ;

}

void my_rect (int x1, int y1, int x2, int y2)
{
	setwritemode(XOR_PUT);
	SetColorAC(15);
	LINE (x1, y1, x2, y1) ;
	LINE (x2, y1, x2, y2) ;
	LINE (x2, y2, x1, y2) ;
	LINE (x1, y2, x1, y1) ;

	LINE (x1+1, y1+1, x2-1, y1+1) ;
	LINE (x2-1, y1+1, x2-1, y2-1) ;
	LINE (x2-1, y2-1, x1+1, y2-1) ;
	LINE (x1+1, y2-1, x1+1, y1+1) ;

	LINE (x1+2, y1+2, x2-2, y1+2) ;
	LINE (x2-2, y1+2, x2-2, y2-2) ;
	LINE (x2-2, y2-2, x1+2, y2-2) ;
	LINE (x1+2, y2-2, x1+2, y1+2) ;

}


void Draw_Point (T_Point *ptrs_point, int mode, int kolor)
/*------------------------------------------------------*/
{
  double df_psize ;
  long x1, y1, x2, y2, x12, y12, dxy, dd, dd1;
  double dl ;
  double x1i, y1i, x2i, y2i;
  double kos30=-0.5, koc30=0.866025404, kos210=0.5, koc210=-0.866025404, kos150=0.5, koc150=0.866025404, kos330=-0.5, koc330=-0.866025404;  //inverted

  if (FALSE == Get_Point_View () && (ptrs_point->typ<7))
  {
    return ;
  }

  if (ptrs_point->typ<8) df_psize = (Get_Point_Size()  *view_vector_scale) / 2;
   else df_psize = (Get_Point_Size() *view_vector_scale) / 4 ;

       
  x1 = pikseleX0 (ptrs_point->x - df_psize) ;
  y1 = pikseleY0 (ptrs_point->y - df_psize) ;
  x2 = pikseleX0 (ptrs_point->x + df_psize) ;
  y2 = pikseleY0 (ptrs_point->y + df_psize) ;
  x12 = (x1 + x2)/2;
  y12 = (y1 + y2)/2;

  linestyle(64);
  
  switch (ptrs_point->typ)
   {
   case 0:
    lineC (x1, y1, x2, y2) ;
    lineC (x1, y2, x2, y1) ;
    break;
   case 1:
    lineC (x1, y1, x2, y2) ;
    lineC (x1, y2, x2, y1) ;
    if (mode==COPY_PUT)
    DrawCircle(pikseleX0 (ptrs_point->x), pikseleY0 (ptrs_point->y), fabs(x2-x1)/5, mode);
    break;
   case 7: /*junction*/
	   ////lineC(x1, y1, x2, y2); ////
	   ////lineC(x1, y2, x2, y1); ////
	   if (mode == COPY_PUT)
	   {
		   if (kolor) set_mode_trans();
		   //	   set_fill_color(k->kolor);
		   Draw_Kolo((long_long)pikseleX0(ptrs_point->x), (long_long)pikseleY0(ptrs_point->y), (long_long)fabs(x2 - x1) / 5);
		   if (kolor) set_mode_solid();
	   }
	   else   DrawCircle(pikseleX0(ptrs_point->x), pikseleY0(ptrs_point->y), fabs(x2 - x1) / 5, mode);
	   break;
   case 8: /*pin*/
    lineC (x1, y1, x2, y2) ;
    if (mode==COPY_PUT)
    DrawCircle(pikseleX0 (ptrs_point->x), pikseleY0 (ptrs_point->y), fabs(x2-x1)/3, mode);
     else lineC (x1, y2, x2, y1) ;
    break;
   case 9: /*pin_g*/
    lineC (x12, y12, x12, y2) ;
    lineC (x1, y1, x2, y2) ;
    if (mode==COPY_PUT)
    DrawCircle(pikseleX0 (ptrs_point->x), pikseleY0 (ptrs_point->y), fabs(x2-x1)/3, mode);
     else lineC (x1, y2, x2, y1) ;
    break; 
   case 10: /*pin_d*/
    lineC (x12, y12, x12, y1) ;
    lineC (x1, y1, x2, y2) ;
    if (mode==COPY_PUT)
    DrawCircle(pikseleX0 (ptrs_point->x), pikseleY0 (ptrs_point->y), fabs(x2-x1)/3, mode);
     else lineC (x1, y2, x2, y1) ;
    break;
   case 11: /*pin_s*/
    lineC (x1, y12, x2, y12) ;
    lineC (x1, y1, x2, y2) ;
    if (mode==COPY_PUT)
    DrawCircle(pikseleX0 (ptrs_point->x), pikseleY0 (ptrs_point->y), fabs(x2-x1)/3, mode);
     else lineC (x1, y2, x2, y1) ;
    break;
       case 12: /*fixed D*/
       case 13: /*fixed L*/
       case 14: /*fixed R*/
       case 15: /*fixed U*/
           lineC (x12, y1, x12, y2) ;
           lineC (x1, y12, x2, y12) ;
           if (kolor) set_mode_trans();
           Draw_Kolo((long_long)pikseleX0(ptrs_point->x), (long_long)pikseleY0(ptrs_point->y), (long_long)fabs(x2 - x1) / 1.5);  //2
           if (kolor) set_mode_solid();
           linestyle(128);
           dxy=pikseleDX(df_psize*2.5);  //2
           dd = (x2 - x1) / 1.5;  //2
           dd1=(x2-x1)/2;
           switch (ptrs_point->typ)
           {
               case 12:
                   lineC(x1 - dxy, y12 + dd, x2 + dxy, y12 + dd);
                   break;
               case 13:
                   lineC (x12-dd, y1+dxy, x12-dd, y2-dxy);
                   break;
               case 14:
                   lineC (x12+dd, y1+dxy, x12+dd, y2-dxy);
                   break;
               case 15:
                   lineC(x1 - dxy, y12 - dd, x2 + dxy, y12 - dd);
                   break;
           }
       break;
       case 16: /*pinned D*/
       case 17: /*pinned L*/
       case 18: /*pinned R*/
       case 19: /*pinned U*/
           lineC (x12, y1, x12, y2) ;
           lineC (x1, y12, x2, y12) ;
           DrawCircle(pikseleX0 (ptrs_point->x), pikseleY0 (ptrs_point->y), fabs(x2-x1), mode);
           linestyle(128);
           dxy=pikseleDX(df_psize*3);
           dd = (x2 - x1);
           dd1=(x2-x1)/2;
           switch (ptrs_point->typ)
           {
               case 16:
                   lineC(x1 - dxy, y12 + dd, x2 + dxy, y12 + dd);
                   break;
               case 17:
                   lineC (x12-dd, y1+dxy, x12-dd, y2-dxy);
                   break;
               case 18:
                   lineC (x12+dd, y1+dxy, x12+dd, y2-dxy);
                   break;
               case 19:
                   lineC(x1 - dxy, y12 - dd, x2 + dxy, y12 - dd);
                   break;
           }
           break;
       case 20: /*fixed roller D*/
       case 21: /*fixed roller L*/
       case 22: /*fixed roller R*/
       case 23: /*fixed roller U*/
           lineC (x12, y1, x12, y2) ;
           lineC (x1, y12, x2, y12) ;
           if (kolor) set_mode_trans();
           Draw_Kolo((long_long)pikseleX0(ptrs_point->x), (long_long)pikseleY0(ptrs_point->y), (long_long)fabs(x2 - x1) / 1.5);  //2
           if (kolor) set_mode_solid();
           linestyle(128);
           dxy=pikseleDX(df_psize*2.5);  //2
           dd=(long)((double)(x2-x1)/1.5);  //2
           dd1=(x2-x1)/2;
           switch (ptrs_point->typ)
           {
               case 20:
                   lineC (x1-dxy, y12+dd, x2+dxy, y12+dd);
                   lineC (x1-dxy, y12+dd+dd1, x2+dxy, y12+dd+dd1);
                   break;
               case 21:
                   lineC (x12-dd, y1+dxy, x12-dd, y2-dxy);
                   lineC (x12-dd-dd1, y1+dxy, x12-dd-dd1, y2-dxy);
                   break;
               case 22:
                   lineC (x12+dd, y1+dxy, x12+dd, y2-dxy);
                   lineC (x12+dd+dd1, y1+dxy, x12+dd+dd1, y2-dxy);
                   break;
               case 23:
                   lineC (x1-dxy, y12-dd, x2+dxy, y12-dd);
                   lineC (x1-dxy, y12-dd-dd1, x2+dxy, y12-dd-dd1);
                   break;
           }
           break;
       case 24: /*pinned roller D*/
       case 25: /*pinned roller L*/
       case 26: /*pinned roller R*/
       case 27: /*pinned roller U*/
           lineC (x12, y1, x12, y2) ;
           lineC (x1, y12, x2, y12) ;
           DrawCircle(pikseleX0 (ptrs_point->x), pikseleY0 (ptrs_point->y), fabs(x2-x1), mode);
           linestyle(128);
           dxy=pikseleDX(df_psize*3);
           dd = (x2 - x1);
           dd1=(x2-x1)/2;
           switch (ptrs_point->typ)
           {
               case 24:
                   lineC(x1 - dxy, y12 + dd, x2 + dxy, y12 + dd);
                   lineC (x1-dxy, y12+dd+dd1, x2+dxy, y12+dd+dd1);
                   break;
               case 25:
                   lineC (x12-dd, y1+dxy, x12-dd, y2-dxy);
                   lineC (x12-dd-dd1, y1+dxy, x12-dd-dd1, y2-dxy);
                   break;
               case 26:
                   lineC (x12+dd, y1+dxy, x12+dd, y2-dxy);
                   lineC (x12+dd+dd1, y1+dxy, x12+dd+dd1, y2-dxy);
                   break;
               case 27:
                   lineC(x1 - dxy, y12 - dd, x2 + dxy, y12 - dd);
                   lineC (x1-dxy, y12-dd-dd1, x2+dxy, y12-dd-dd1);
                   break;
           }
           break;
       case 28: /*no rotation Z*/
           lineC (x12, y1, x12, y2) ;
           lineC (x1, y12, x2, y12) ;
           if (kolor) set_mode_trans();
           Draw_Kolo((long_long)pikseleX0(ptrs_point->x), (long_long)pikseleY0(ptrs_point->y), (long_long)fabs(x2 - x1) / 1.5);  //2
           if (kolor) set_mode_solid();
           linestyle(96);
           dxy=pikseleDX(df_psize*2.325);  //2.5
           dd=(long)((double)(x2-x1)*0.25); //  sin(22)/1.5 sin(30)/1.5
           dd1=(long)((double)(x2-x1)*0.618);  // cos(22)/1.5; cos(30)/1.5;

           lineC (x12-dxy, y12+dd, x12-dd1, y12+dd);
           lineC (x12+dd1, y12+dd, x12+dxy, y12+dd);
           lineC (x12-dxy, y12-dd, x12-dd1, y12-dd);
           lineC (x12+dd1, y12-dd, x12+dxy, y12-dd);

           lineC (x12-dd, y12+dxy, x12-dd, y12+dd1);
           lineC (x12-dd, y12-dd1, x12-dd, y12-dxy);
           lineC (x12+dd, y12+dxy, x12+dd, y12+dd1);
           lineC (x12+dd, y12-dd1, x12+dd, y12-dxy);

           break;
       case 29: /*fixed roller X*/
       case 30: /*fixed roller XU*/
           //Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)x12, (double)y1, &x1i, &y1i);
           //Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)x12, (double)y2, &x2i, &y2i);
           lineC (x12, y1, x12, y2) ;
           //lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
           Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)x1, (double)y12, &x1i, &y1i);
           Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)x2, (double)y12, &x2i, &y2i);
           //lineC (x1, y12, x2, y12) ;
           lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
           if (kolor) set_mode_trans();
           Draw_Kolo((long_long)pikseleX0(ptrs_point->x), (long_long)pikseleY0(ptrs_point->y), (long_long)fabs(x2 - x1) / 1.5);  //2
           if (kolor) set_mode_solid();
           linestyle(128);
           dxy=pikseleDX(df_psize*2.5);  //2
           dd=(long)((double)(x2-x1)/1.5);  //2
           dd1=(long)((double)(x2-x1)/2.);
           dl=(double)dd1*koc30;

           switch (ptrs_point->typ)
           {
               case 29:
                   Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd), &x1i, &y1i);
                   Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd), &x2i, &y2i);
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd+dd1), &x1i, &y1i);
                   Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd+dd1), &x2i, &y2i);
                   //shifting line
                   x1i = x1i + dl * koc30;
                   y1i = y1i + dl * kos30;
                   x2i = x2i + dl * koc30;
                   y2i = y2i + dl * kos30;
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   break;
               case 30:
                   Rotate_Point(kos210, koc210, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd), &x1i, &y1i);
                   Rotate_Point(kos210, koc210, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd), &x2i, &y2i);
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   Rotate_Point(kos210, koc210, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd+dd1), &x1i, &y1i);
                   Rotate_Point(kos210, koc210, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd+dd1), &x2i, &y2i);
                   //shifting line
                   x1i = x1i + dl * koc210;
                   y1i = y1i + dl * kos210;
                   x2i = x2i + dl * koc210;
                   y2i = y2i + dl * kos210;
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   break;
           }
            break;
       case 31: /*fixed roller Y*/
       case 32: /*fixed roller YU*/
           //Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)x12, (double)y1, &x1i, &y1i);
           //Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)x12, (double)y2, &x2i, &y2i);
           lineC (x12, y1, x12, y2) ;
           //lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
           Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)x1, (double)y12, &x1i, &y1i);
           Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)x2, (double)y12, &x2i, &y2i);
           //lineC (x1, y12, x2, y12) ;
           lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
           if (kolor) set_mode_trans();
           Draw_Kolo((long_long)pikseleX0(ptrs_point->x), (long_long)pikseleY0(ptrs_point->y), (long_long)fabs(x2 - x1) / 1.5);  //2
           if (kolor) set_mode_solid();
           linestyle(128);
           dxy=pikseleDX(df_psize*2.5);  //2
           dd=(long)((double)(x2-x1)/1.5);  //2
           dd1=(float)((double)(x2-x1)/2.);
           dl=-(double)dd1*koc30;

           switch (ptrs_point->typ)
           {
               case 31:
                   Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd), &x1i, &y1i);
                   Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd), &x2i, &y2i);
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd+dd1), &x1i, &y1i);
                   Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd+dd1), &x2i, &y2i);
                   //shifting line
                   x1i = x1i + dl * koc150;
                   y1i = y1i + dl * kos150;
                   x2i = x2i + dl * koc150;
                   y2i = y2i + dl * kos150;
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   break;
               case 32:
                   Rotate_Point(kos330, koc330, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd), &x1i, &y1i);
                   Rotate_Point(kos330, koc330, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd), &x2i, &y2i);
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   Rotate_Point(kos330, koc330, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd+dd1), &x1i, &y1i);
                   Rotate_Point(kos330, koc330, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd+dd1), &x2i, &y2i);
                   //shifting line
                   x1i = x1i + dl * koc330;
                   y1i = y1i + dl * kos330;
                   x2i = x2i + dl * koc330;
                   y2i = y2i + dl * kos330;
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   break;
           }
           break;
       case 33: /*pinned roller X*/
       case 34: /*pinned roller XU*/
           //Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)x12, (double)y1, &x1i, &y1i);
           //Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)x12, (double)y2, &x2i, &y2i);
           lineC (x12, y1, x12, y2) ;
           //lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
           Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)x1, (double)y12, &x1i, &y1i);
           Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)x2, (double)y12, &x2i, &y2i);
           //lineC (x1, y12, x2, y12) ;
           lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
           DrawCircle(pikseleX0 (ptrs_point->x), pikseleY0 (ptrs_point->y), fabs(x2-x1), mode);
           linestyle(128);
           dxy=pikseleDX(df_psize*3);
           dd=(x2-x1);
           dd1=(long)((double)(x2-x1)/2.);
           dl=(double)dd1*koc30;
           
           switch (ptrs_point->typ)
           {
               case 33:
                   Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd), &x1i, &y1i);
                   Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd), &x2i, &y2i);
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd+dd1), &x1i, &y1i);
                   Rotate_Point(kos30, koc30, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd+dd1), &x2i, &y2i);
                   //shifting line
                   x1i = x1i + dl * koc30;
                   y1i = y1i + dl * kos30;
                   x2i = x2i + dl * koc30;
                   y2i = y2i + dl * kos30;
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   break;
               case 34:
                   Rotate_Point(kos210, koc210, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd), &x1i, &y1i);
                   Rotate_Point(kos210, koc210, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd), &x2i, &y2i);
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   Rotate_Point(kos210, koc210, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd+dd1), &x1i, &y1i);
                   Rotate_Point(kos210, koc210, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd+dd1), &x2i, &y2i);
                   //shifting line
                   x1i = x1i + dl * koc210;
                   y1i = y1i + dl * kos210;
                   x2i = x2i + dl * koc210;
                   y2i = y2i + dl * kos210;
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   break;
           }
           break;
       case 35: /*pinned roller Y*/
       case 36: /*pinned roller YU*/
           //Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)x12, (double)y1, &x1i, &y1i);
           //Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)x12, (double)y2, &x2i, &y2i);
           lineC (x12, y1, x12, y2) ;
           //lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
           Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)x1, (double)y12, &x1i, &y1i);
           Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)x2, (double)y12, &x2i, &y2i);
           //lineC (x1, y12, x2, y12) ;
           lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
           DrawCircle(pikseleX0 (ptrs_point->x), pikseleY0 (ptrs_point->y), fabs(x2-x1), mode);
           linestyle(128);
           dxy=pikseleDX(df_psize*3);
           dd=(x2-x1);
           dd1=(float)((double)(x2-x1)/2.);
           dl=-(double)dd1*koc30;

           switch (ptrs_point->typ)
           {
               case 35:
                   Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd), &x1i, &y1i);
                   Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd), &x2i, &y2i);
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd+dd1), &x1i, &y1i);
                   Rotate_Point(kos150, koc150, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd+dd1), &x2i, &y2i);
                   //shifting line
                   x1i = x1i + dl * koc150;
                   y1i = y1i + dl * kos150;
                   x2i = x2i + dl * koc150;
                   y2i = y2i + dl * kos150;
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   break;
               case 36:
                   Rotate_Point(kos330, koc330, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd), &x1i, &y1i);
                   Rotate_Point(kos330, koc330, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd), &x2i, &y2i);
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   Rotate_Point(kos330, koc330, (double)x12, (double)y12, (double)(x1-dxy), (double)(y12+dd+dd1), &x1i, &y1i);
                   Rotate_Point(kos330, koc330, (double)x12, (double)y12, (double)(x2+dxy), (double)(y12+dd+dd1), &x2i, &y2i);
                   //shifting line
                   x1i = x1i + dl * koc330;
                   y1i = y1i + dl * kos330;
                   x2i = x2i + dl * koc330;
                   y2i = y2i + dl * kos330;
                   lineC ((long)x1i, (long)y1i, (long)x2i, (long)y2i) ;
                   break;
           }
           break;
   default : 
    lineC (x1, y1, x2, y2) ;
    lineC (x1, y2, x2, y1) ;
    break;
   }
}

static void out_v_krz (long x, long y)
{   long ksize;
    linestyle(64);
    ksize=pikseleDX(0.5);
    lineC(x-ksize, y, x+ksize, y);
    lineC(x, y-ksize, x, y+ksize);
}

double normalize_txt_angle(double angle0)
{
    double angle;
    angle=Angle_Normal(angle0);
    if ((angle>(Pi_/2.0)) && (angle<(Pi_*1.5))) angle-=Pi_;
   return angle;
}

void normalize_txt(TEXT *ptrs_text)
{  double xp,yp,X2,Y2,tdl,tsin,tcos,xt1,yt1,xt2,yt2,xt3,yt3,xt4,yt4;
    double del_wys=1.0;

    tdl = Get_TextLen (ptrs_text, NULL) ;

    tsin = sin (ptrs_text->kat) ;
    tcos = cos (ptrs_text->kat) ;
    xt1 = ptrs_text->x ;
    yt1 = ptrs_text->y;
    switch (ptrs_text->justowanie)
    {
        case 0 : xt2 = xt1 + tdl * tcos ;   /*do lewej*/
            yt2 = yt1 - Ky * tdl * tsin ;
            xt3 = xt2 - (ptrs_text->wysokosc * del_wys) * tsin ;
            yt3 = yt2 - Ky * (ptrs_text->wysokosc * del_wys) * tcos ;
            xt4 = xt1 - (ptrs_text->wysokosc * del_wys) * tsin ;
            yt4 = yt1 + (ptrs_text->wysokosc * del_wys) * tcos ;
            break;
        case 1 : xt2 = xt1 - tdl * tcos ;   /*do prawej*/
            yt2 = yt1 + Ky * tdl * tsin ;
            xt3 = xt2 - (ptrs_text->wysokosc * del_wys) * tsin ;
            yt3 = yt2 - Ky * (ptrs_text->wysokosc * del_wys) * tcos ;
            xt4 = xt1 - (ptrs_text->wysokosc * del_wys) * tsin ;
            yt4 = yt1 + (ptrs_text->wysokosc * del_wys) * tcos ;
            break;
        case 2 : xt2 = xt1;   /*srodkowo*/
            yt2 = yt1;
            xt3 = xt2 - (ptrs_text->wysokosc * del_wys) * tsin ;
            yt3 = yt2 - Ky * (ptrs_text->wysokosc * del_wys) * tcos ;
            break;
        case 3 : xt2 = xt1;   /*centralnie*/
            yt2 = yt1;
            xt3 = xt2 - 0.5 * (ptrs_text->wysokosc) * (del_wys -1) * tsin ;
            yt3 = yt2 - 0.5 * Ky * (ptrs_text->wysokosc) * (del_wys -1) * tcos ;
            break;
        default :
            break;
    }

    if (ptrs_text->kat > Pi2) ptrs_text->kat -= Pi2;
    if (ptrs_text->kat < 0) ptrs_text->kat += Pi2;

    if ((ptrs_text->kat > (1.001*Pi2/4)) && (ptrs_text->kat < (1.001*Pi2*0.75)))
    {
        ptrs_text->kat -= (Pi2 / 2);
        if (ptrs_text->kat > Pi2) ptrs_text->kat -= Pi2;
        ptrs_text->x = xt3;
        ptrs_text->y = yt3;

        if (ptrs_text->typ == n_typ_pin_name) {
            if (ptrs_text->justowanie == j_do_lewej) {
                ptrs_text->justowanie = j_do_prawej;
                ptrs_text->x = xt4;
                ptrs_text->y = yt4;
            } else {
                if (ptrs_text->justowanie == j_do_prawej) {
                    ptrs_text->justowanie = j_do_lewej;
                    ptrs_text->x = xt4;
                    ptrs_text->y = yt4;
                }
            }
        }
    }
}

double lineSlope(point a,point b){

    if(a.x-b.x == 0.0)
        return NAN;
    else
        return (a.y-b.y)/(a.x-b.x);
}

point intersectionPoint(point a1,point a2,point b1,point b2){
    point c;

    double slopeA = lineSlope(a1,a2), slopeB = lineSlope(b1,b2);

    if(slopeA==slopeB){
        c.x = NAN;
        c.y = NAN;
    }
    else if(isnan(slopeA) && !isnan(slopeB)){
        c.x = a1.x;
        c.y = (a1.x-b1.x)*slopeB + b1.y;
    }
    else if(isnan(slopeB) && !isnan(slopeA)){
        c.x = b1.x;
        c.y = (b1.x-a1.x)*slopeA + a1.y;
    }
    else{
        c.x = (slopeA*a1.x - slopeB*b1.x + b1.y - a1.y)/(slopeA - slopeB);
        c.y = slopeB*(c.x - b1.x) + b1.y;
    }

    return c;
}


double Dist_PP_f (float x1, float y1, float x2, float y2)
/*-----------------------------------------------------*/
{
    return sqrt ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) ;
}

double measure_arcvector (LUK *ptrs_arc, BOOL b_first_end, double df_l0, double df_dl,double *df_x, double *df_y)
{
    double df_line_len ;
    double df_d_angle, df_angle ;
    double df_angle1, df_angle2 ;

    df_angle1 = Angle_Normal (ptrs_arc->kat1) ;
    df_angle2 = Angle_Normal (ptrs_arc->kat2) ;
    if (df_angle1 > df_angle2)
    {
        df_angle2 += Pi2 ;
    }
    df_line_len = fabs (df_angle2 - df_angle1) * ptrs_arc->r ;
    if (df_line_len >= df_l0 + df_dl &&
        FALSE == Check_if_Equal (df_line_len, 0))
    {
        df_d_angle = (df_l0 + df_dl) / ptrs_arc->r ;
        df_angle = ptrs_arc->kat1 + df_d_angle ;
        if (b_first_end == FALSE)
        {
            df_angle = ptrs_arc->kat2 - df_d_angle ;
        }
        *df_x = ptrs_arc->x + ptrs_arc->r * cos (df_angle) ;
        *df_y = ptrs_arc->y + ptrs_arc->r * sin (df_angle) ;
    }
    return df_line_len - df_l0 - df_dl ;
}

double measure_vector (float x1, float y1, float x2, float y2, BOOL b_first_end, double df_l0, double df_dl, double *df_x, double *df_y)
{
    double df_line_len ;
    double df_t ;
    double df_x1, df_x2, df_y1, df_y2 ;

    df_line_len = Dist_PP_f (x1, y1, x2, y2) ;
    if (df_line_len >= df_l0 + df_dl &&
        FALSE == Check_if_Equal (df_line_len, 0))
    {
        df_t = (df_l0 + df_dl) / df_line_len ;
        df_x1 = x1 ;
        df_y1 = y1 ;
        df_x2 = x2 ;
        df_y2 = y2 ;
        if (FALSE == b_first_end)
        {
            df_x1 = x2 ;
            df_y1 = y2 ;
            df_x2 = x1 ;
            df_y2 = y1 ;
        }
        *df_x = (df_x1 + df_t * (df_x2 - df_x1)) ;
        *df_y = (df_y1 + df_t * (df_y2 - df_y1)) ;
    }
    return (df_line_len - df_l0 - df_dl) ;
}


void draw_arrow(double x0, double y0, double x1, double y1, double x2, double y2, double angle_rad, double katS_rad, double koc1, double kos1, double koc2, double kos2, double psize, AVECTOR *v, int mode, int kolor, int vkolor)
{
    LINIA L=Ldef;
    WIELOKAT w=Stdef;
    OKRAG k=Kdef;
    int ret;

    L.warstwa = w.warstwa = k.warstwa = v->warstwa;
    L.kolor = w.kolor = k.kolor = vkolor; //v->kolor;

    if ((v->style<V_EDGE_SIMPLE) && (v->style>0))
    {
        L.x1 = (float)x1;
        L.y1 = (float)y1;
        L.x2 = (float)x2;
        L.y2 = (float)y2;
        L.typ = 64;

        if (linia_visible(&L))
            rysuj_obiekt_(&L, mode, kolor);
    }

    if ((v->style==V_EDGE_FIXED) || (v->style==V_EDGE_FIXED_INV) || (v->style==V_EDGE_ARC_FIXED) || (v->style==V_EDGE_ARC_FIXED_INV))
    {
       L.typ = V_FIXED_LINE; //128;
       L.x1 = (float)x0;
       L.y1 = (float)y0;
       L.x2 = (float)(x0 + psize * koc1);
       L.y2= (float)(y0 - psize * kos1);

        if (linia_visible(&L))
            rysuj_obiekt_(&L, mode, kolor);
    }
    else if ((v->style==V_EDGE_ROLL) || (v->style==V_EDGE_ROLL_INV) || (v->style==V_EDGE_ARC_ROLL) || (v->style==V_EDGE_ARC_ROLL_INV))
    {

        if (v->style == V_EDGE_ROLL)  //roll edge
            psize *= -1;
        k.x=(float)(x0 + psize * 0.5 * koc1);
        k.y=(float)(y0 - psize * 0.5 * kos1);
        k.r=(float)(fabs(psize * 0.5));

        if (Check_Draw_Pieslice (&k))
            rysuj_obiekt_(&k, mode, kolor);
    }
    else
    {
        w.xy[2] = (float)x0;
        w.xy[3] = (float)y0;

        if ((!(v->cartflags & 1)) || (v->style==17) || (v->style==20))
        {
            w.xy[0] = (float)(x0 + psize * koc1);
            w.xy[1] = (float)(y0 - psize * kos1);
            w.xy[4] = (float)(x0 + psize * koc2);
            w.xy[5] = (float)(y0 - psize * kos2);
        }
        else
        {
            /*
            // Isometric mode
            // Get isometric angle in radians (current vector direction)
            double iso_angle = cartesian_angle_to_isometric_angle(angle_rad);

            // Backward for arrowhead at start (head)
            double iso_backward = fmod(iso_angle + M_PI, 2.0 * M_PI);
            if (iso_backward < 0.0) iso_backward += 2.0 * M_PI;

            // Symmetric opening in isometric space (± katS_rad)
            double katS_rad = 25.0 * M_PI / 180.0;  // or your katS variable

            double iso_angle1 = fmod(iso_backward + katS_rad, 2.0 * M_PI);
            double iso_angle2 = fmod(iso_backward - katS_rad, 2.0 * M_PI);

            // Convert to degrees for polar function
            double iso_angle1_deg = iso_angle1 * 180.0 / M_PI;
            double iso_angle2_deg = iso_angle2 * 180.0 / M_PI;

            // Compute tips using isometric polar — this gives correct X/Y without extra sign flip
            double x_tip1, y_tip1;
            isometric_polar_to_cartesian(x0, y0, psize, iso_angle1_deg, &x_tip1, &y_tip1);

            double x_tip2, y_tip2;
            isometric_polar_to_cartesian(x0, y0, psize, iso_angle2_deg, &x_tip2, &y_tip2);

            // Set vertices (no Y inversion here!)
            w.xy[0] = x_tip1;
            w.xy[1] = y_tip1;
            w.xy[4] = x_tip2;
            w.xy[5] = y_tip2;

             */
            // Isometric mode
            double iso_angle = cartesian_angle_to_isometric_angle(angle_rad);

            double iso_backward = fmod(iso_angle + M_PI/2., 2.0 * M_PI);
            if (iso_backward < 0.0) iso_backward += 2.0 * M_PI;

            double katS_rad = 25.0 * M_PI / 180.0;  // or your variable

            // SWAPPED ORDER TO MATCH MANUAL
            double iso_angle1 = fmod(iso_backward + katS_rad, 2.0 * M_PI);
            double iso_angle2 = fmod(iso_backward - katS_rad, 2.0 * M_PI);

            double iso_angle1_deg = iso_angle1 * 180.0 / M_PI;
            double iso_angle2_deg = iso_angle2 * 180.0 / M_PI;

            double x_tip1, y_tip1, x_tip2, y_tip2;
            isometric_polar_to_cartesian(x0, y0, psize, iso_angle1_deg, &x_tip1, &y_tip1);
            isometric_polar_to_cartesian(x0, y0, psize, iso_angle2_deg, &x_tip2, &y_tip2);

            w.xy[0] = (float)x_tip1;
            w.xy[1] = (float)y_tip1;
            w.xy[2] = (float)x0;
            w.xy[3] = (float)y0;
            w.xy[4] = (float)x_tip2;
            w.xy[5] = (float)y_tip2;
        }

        w.lp = 6;
        w.n = 32;

        if (wielokat_visible(&w))
            rysuj_obiekt_(&w, mode, kolor);
    }
}

void draw_spring(double x0, double y0, double x1, double y1, double x2, double y2, double angle_rad, double katS_rad, double koc1, double kos1, double koc2, double kos2, double psize, AVECTOR *v, int mode, int kolor, int vkolor)
{
    LINIA L=Ldef;
    double angle;
    int ret;
    int i;
    float n=1.f;

    float spring[]={0.f,0.f, 0.45f, -0.1f, -0.45f, -0.3f, 0.45f, -0.5f, -0.45f, -0.7f, 0.45f, -0.9f, -0.45f, -1.1f, 0.45f, -1.3f, -0.45f, -1.3f};

    L.warstwa = v->warstwa;
    L.kolor = vkolor; //v->kolor;

    if (!(v->cartflags & 1))
    {
        angle=M_PI;
    }
    else
    {
        angle=M_PI;
    }

    if (v->y2>v->y1) n=1.f;
    else n=-1.f;

    for (i=0; i<16; i+=2)
    {
        L.x1 = (float)(n*spring[i]*psize+x0);
        L.y1 = (float)(spring[i+1]*psize+y0);
        L.x2 = (float)(n*spring[i+2]*psize+x0);
        L.y2 = (float)(spring[i+3]*psize+y0);
        if (linia_visible(&L))
            rysuj_obiekt_(&L, mode, kolor);
    }
}

void draw_wave(double x0, double y0, double x1, double y1, double x2, double y2, double koc, double kos, double n1, double ra, AVECTOR *v, int mode, int kolor, int vkolor)
{
    SPLINE s=Splinedef;
    LINIA L=Ldef;
    double dl, dlm, dx, dy;
    double  xp, yp;
    int i;
    BOOL cpv;

    cpv=Get_Control_Points_View();
    Set_Control_Points_View(FALSE);

    if ((v->magnitude1==0) && (v->magnitude2==0)) return;

    dx=ra;
    dy=ra/4;

    s.warstwa=v->warstwa;
    //s.kolor=vkolor;
    if (n1==1) s.kolor=1;
    else s.kolor=5;
    s.typ=64;

    dl=n1*sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

    if (fabs(dl)>(3*dx))
    {
        dlm=0;
        s.xy[0]=(float)x1;
        s.xy[1]=(float)y1;
        dlm+=(n1*dx);
        s.xy[2]=(float)(x1+dlm);
        s.xy[3]=(float)(y1+dy);
        i=4;
        dlm+=(2*n1*dx);
        while (fabs(dlm)<fabs(dl))
        {
            dy=-dy;
            s.xy[i]=(float)(x1+dlm);
            s.xy[i+1]=(float)(y1+dy);
            i+=2;
            if (i>=NumSplinePoints*2) break;
            dlm+=(2*n1*dx);
        }

        dlm-=(n1*dx);
        dy=-dy;
        s.xy[i]=(float)(x1+dlm);
        s.xy[i+1]=(float)y1;
        i+=2;

        s.lp=i;
        s.xy[s.lp]=0.75f;
        s.n = 8 + (s.lp + 1 ) * sizeof(float);

        s.npts=5;
        s.closed=0;

        for (i = 0; i < (int)s.lp; i += 2)
        {
            Rotate_Point(kos, koc, x1, y1, s.xy[i], s.xy[i + 1], &xp, &yp);
            s.xy[i] = xp; s.xy[i + 1] = yp;
        }

        //if (spline_visible(&s))
        rysuj_obiekt_(&s, mode, kolor);
    }
    else if (fabs(dl)>0.001)
    {
        L.warstwa=v->warstwa;
        //s.kolor=vkolor;
        if (n1==1) L.kolor=1;
        else L.kolor=5;
        L.typ=64;
        L.x1=x1;
        L.y1=y1;
        L.x2=x2;
        L.y2=y2;
        rysuj_obiekt_(&L, mode, kolor);
    }
    Set_Control_Points_View(cpv);
}


int make_arcarrows(LUK *l, AVECTOR *v, double kat, int mode, int kolor, int vkolor, int redraw_obj)
{
    int i;
    double df_line_rem;
    double df_l0;
    double df_psize;
    double df_seg_len, df_seg_len_dens;
    BOOL b_first_end=TRUE;
    double df_x0, df_y0, df_x, df_y, df_x1, df_y1, df_x2, df_y2;
    point p;
    float n, n1;
    double koc, kos;
    double koc1, kos1, koc2, kos2;
    double ra, ra1;
    PLINIA PL1;
    LINIA Lt1;
    double del_angle;
    double angle;
    double katS=Pi_*25.0/180;
    char keym;
    double shift;
    double THE_END=0. ; /*-df_seg_len/2.*/

    //T_Point P;

    df_psize = Get_Point_Size () *view_vector_scale;
    df_seg_len=df_psize;

    ra=df_seg_len / 2;
    ra1=0.9*ra;

    if ((v->style==V_EDGE_ARC_FIXED) || (v->style==V_EDGE_ARC_FIXED_INV))
    {
        katS = Pi_ * 45.0 / 180;
        df_seg_len_dens = df_seg_len / 2.0;
    }
    else df_seg_len_dens = df_seg_len;

    i = 0 ;

    shift = (v->style<V_EDGE_SIMPLE) ? 2. : 1.;

    df_l0 = -df_seg_len_dens/shift; //1 or 2 ;   //first arrow will start at the beginning of the edge
    do
    {
        if (!redraw_obj)
        {
            keym = readmouse();

            if (mvcurb.akton && mvcurb.mvc)
                goto ending;
            if (mvcurb.aktoff && !mvcurb.L && !mvcurb.T && !mvcurb.K &&
                !mvcurb.O && !mvcurb.l && !mvcurb.W && !mvcurb.P)
                goto ending;
        }

        df_line_rem = measure_arcvector(l, b_first_end, df_l0, df_seg_len_dens, &df_x, &df_y);

        if ((v->style>=V_EDGE_SIMPLE) && (df_line_rem<df_seg_len/2.))
        {
            df_x=v->x1+v->r*cosf(v->angle2);
            df_y=v->y1+v->r*sinf(v->angle2);
        }

        if (TRUE == Check_if_GT (df_line_rem, (v->style<V_EDGE_SIMPLE) ? df_seg_len_dens/4 : -df_seg_len_dens/2))   //or maybe df_seg_len_dens/2 for load
        {

            Lt1.x1 = (float)df_x;
            Lt1.y1 = (float)df_y;
            Lt1.x2 = l->x;
            Lt1.y2 = l->y;
            parametry_lini(&Lt1, &PL1);

            //angle=Angle_Normal(PL1.kat*Pi2/180);
            //angle=Angle_Normal(PL1.kat*Pi2/180);
            //koc=cos(angle);
            //kos=sin(angle);

            if ((v->style==V_EDGE_ARC_SIMPLE_INV) || (v->style==V_EDGE_ARC_FIXED_INV) || (v->style==V_EDGE_ARC_ROLL_INV)) PL1.kat+=180;

            angle = Angle_Normal(-(PL1.kat) * Pi / 180);
            if (fabs(angle - Pi2) < 0.00001) angle = 0;
            if (fabs(angle) < 0.00001) angle = 0;

            if ((v->style==V_EDGE_ARC_ROLL) || (v->style==V_EDGE_ARC_ROLL_INV))
            {
                koc1 = koc2 = cos(angle);
                kos1 = kos2 = sin(angle);
            }
            else
            {
                koc1 = cos(angle - katS);
                koc2 = cos(angle + katS);
                kos1 = sin(angle - katS);
                kos2 = sin(angle + katS);
            }

            n1 = -1;

            draw_arrow(df_x, df_y, df_x, df_y, df_x, df_y, angle, katS, koc1, kos1, koc2, kos2, n1 * ra, v, mode, kolor, vkolor);
        }

        //P.x=df_x;
        //P.y=df_y;
        //P.kolor=7;
        //rysuj_punkt_(&P, COPY_PUT, 1);

        df_l0 += df_seg_len_dens ;
        i++ ;
    }
    while (TRUE == Check_if_GT (df_line_rem, (v->style<V_EDGE_SIMPLE) ? df_seg_len/2. : THE_END /*-df_seg_len/2.*/)) ;

    return 1;
ending:
    return 0;
}

int make_arrows(float x1, float y1, float x2, float y2, float x11, float y11, float x12, float y12, double angle0, AVECTOR *v, double kat, int mode, int kolor, int vkolor, int redraw_obj)
{
    //arrows
    int i;
    double df_l0;
    double df_line_rem;
    BOOL b_first_end=TRUE;
    double df_seg_len, df_seg_len_dens;
    double df_x0, df_y0, df_x, df_y, df_x1, df_y1, df_x2, df_y2;
    double df_psize;
    point a1, a2, b1, b2, p;
    double koc, kos;
    double ra, ra1;
    float n, n1;
    double angle; //, angle_rev;
    double katS=Pi_*25.0/180;
    double koc1, kos1, koc2, kos2;
    PLINIA PL1;
    LINIA Lt1;
    double del_angle;
	char keym;
    double shift;
    double THE_END=0. ; /*-df_seg_len/2.*/

    df_psize = Get_Point_Size () *view_vector_scale;

    if (v->style!=15) df_seg_len=df_psize;
    else df_seg_len=df_psize*0.66;  //THERMAL

    if ((v->style==V_EDGE_FIXED) || (v->style==V_EDGE_FIXED_INV))
        katS = Pi_ * 45.0 / 180.;

    angle=Angle_Normal(angle0);

    koc=cos(angle);
    kos=sin(angle);

    ra=df_seg_len / 2;
    ra1=0.9*ra;

    switch (v->style)
    {
        case 10:
        case 17:  //slab load
        case 20:  //Z load
            if (!(v->cartflags & 1)  || (v->style!=10))
            {
                if (x1 <= x2) angle = Angle_Normal(angle0 + Pi_);
            }
            else
            {
                if (x1 > x2) angle = Angle_Normal(angle0 + 60./180.*Pi_);
                else angle = Angle_Normal(angle0 - 120./180.*Pi_);
            }
            break;
        case 11:
            if (!(v->cartflags & 1)) {
                if (y1 <= y2) angle = Angle_Normal(angle0 + Pi_);
            }
            else
            {
                if (y1 <= y2) angle = Angle_Normal(angle0 + 120./180.*Pi_);
                else angle = Angle_Normal(angle0 - 60./180.*Pi_);
            }
            break;
        case 0:
        case 12:
        case V_EDGE_SIMPLE:  //simple supported edge
        case V_EDGE_SIMPLE_INV:  //simple supported edge reversed
        case V_EDGE_FIXED:  //fixed edge
        case V_EDGE_FIXED_INV:  //fixed edge reversed
        case V_EDGE_ROLL:  //roll edge
        case V_EDGE_ROLL_INV:  //roll edge reversed
            if (!(v->cartflags & 1))
                angle=Angle_Normal(-angle0);
            else
            {
                double iso_angle = cartesian_angle_to_isometric_angle(angle0);
                // Add 180° in isometric space (counter-clockwise perpendicular)
                double iso_perp = fmod(iso_angle - M_PI / 2.0, 2.0 * M_PI);
                if (iso_perp < 0.0) iso_perp += 2.0 * M_PI;
                angle = isometric_angle_to_cartesian_angle_rad(iso_perp);
            }

            if (fabs(angle-Pi2)<0.00001) angle=0.;
            if (fabs(angle)<0.00001) angle=0.;
            break;
        case 13:
            if (!(v->cartflags & 1)) {
                if (x1 <= x2) angle = Angle_Normal(angle0 + Pi_);
            }
            else
            {
                //if (x1 <= x2) angle = Angle_Normal(angle0 + 120./180.*Pi_);
                //else angle = Angle_Normal(angle0 - 60./180.*Pi_);

                double iso_angle = cartesian_angle_to_isometric_angle(angle0);
                // Add 180° in isometric space (counter-clockwise perpendicular)
                double iso_perp = fmod(iso_angle - M_PI / 2.0, 2.0 * M_PI);
                if (iso_perp < 0.0) iso_perp += 2.0 * M_PI;
                angle = isometric_angle_to_cartesian_angle_rad(iso_perp);
            }
            break;
        case 14:
            if (!(v->cartflags & 1)) {
                if (y1 <= y2) angle = Angle_Normal(angle0 + Pi_);
            }
            else
            {
                //if (x1 <= x2) angle = Angle_Normal(angle0 + 120./180.*Pi_);
                //else angle = Angle_Normal(angle0 - 60./180.*Pi_);

                double iso_angle = cartesian_angle_to_isometric_angle(angle0);
                // Add 180° in isometric space (counter-clockwise perpendicular)
                double iso_perp = fmod(iso_angle - M_PI / 2.0, 2.0 * M_PI);
                if (iso_perp < 0.0) iso_perp += 2.0 * M_PI;
                angle = isometric_angle_to_cartesian_angle_rad(iso_perp);
            }
            break;
        case 15:
            angle=Angle_Normal(-angle0);
            if (fabs(angle-Pi2)<0.00001) angle=0;
            if (fabs(angle)<0.00001) angle=0;
            break;
        default:
            break;
    }

    if ((v->style==V_EDGE_ROLL) || (v->style==V_EDGE_ROLL_INV))
    {
        koc1 = koc2 = cos(angle);
        kos1 = kos2 = sin(angle);
    }
    else
    {
        //if (((!(v->cartflags & 1))/* || (v->style==17) || (v->style==20)*/) && (y1 > y2))
        //{
        //    angle = Angle_Normal(angle + -60./180.*Pi_);
        //}

        //if ((v->cartflags & 1) && (y1 > y2))
        //{
        //    angle = Angle_Normal(angle + -60./180.*Pi_);
        //}

        koc1 = cos(angle - katS);
        koc2 = cos(angle + katS);
        kos1 = sin(angle - katS);
        kos2 = sin(angle + katS);
    }

    //angle_rev=Angle_Normal(angle+Pi_);

    i = 0 ;

    if ((v->style==V_EDGE_FIXED) || (v->style==V_EDGE_FIXED_INV))
        df_seg_len_dens=df_seg_len/2.0;
    else df_seg_len_dens=df_seg_len;

    shift = (v->style<V_EDGE_SIMPLE) ? 2. : 1.;

    df_l0 = -df_seg_len_dens / shift; // 1 or 2;  first arrow will start at the beginning of the edge
    do
    {
		if (!redraw_obj)
		{
			keym = readmouse();

			if (mvcurb.akton && mvcurb.mvc)
				goto ending;
			if (mvcurb.aktoff && !mvcurb.L && !mvcurb.T && !mvcurb.K &&
				!mvcurb.O && !mvcurb.l && !mvcurb.W && !mvcurb.P)
				goto ending;
		}

        df_line_rem = measure_vector (x1, y1, x2, y2, b_first_end, df_l0,  df_seg_len_dens, &df_x, &df_y) ;

        if ((v->style>=V_EDGE_SIMPLE) && (df_line_rem<df_seg_len/2.))
        {
            df_x=v->x2;
            df_y=v->y2;
        }

        if (TRUE == Check_if_GT (df_line_rem, (v->style<V_EDGE_SIMPLE) ? df_seg_len_dens/4 : -df_seg_len_dens/2))   //or maybe df_seg_len_dens/2 for load
        {

            if ((Check_if_Equal(angle, Pi_ / 2)) || (Check_if_Equal(angle, Pi_ * 3 / 2))) //vertical
            {
                a1.x = df_x;
                a1.y = -100.0;
                a2.x = df_x;
                a2.y = 100.0;
                b1.x = (double) x11;
                b1.y = (double) y11;
                b2.x = (double) x12;
                b2.y = (double) y12;

                if (b1.x == b2.x) b2.x += 0.0001;

                p = intersectionPoint(a1, a2, b1, b2);

                if (isnan(p.x)) {
                    p.x = b2.x;
                    p.y = b2.y;
                }
            } else if ((Check_if_Equal(angle, 0)) || (Check_if_Equal(angle, Pi_))) //horizontal
            {
                a1.x = -100.0;
                a1.y = df_y;
                a2.x = 100.0;
                a2.y = df_y;
                b1.x = (double) x11;
                b1.y = (double) y11;
                b2.x = (double) x12;
                b2.y = (double) y12;
                p = intersectionPoint(a1, a2, b1, b2);

                if (isnan(p.x)) {
                    p.x = b1.x;
                    p.y = b1.y;
                }
            } else {
                a1.x = df_x - 100.0 * koc;
                a1.y = df_y - 100.0 * kos;
                a2.x = df_x + 100.0 * koc;
                a2.y = df_y + 100.0 * kos;
                b1.x = (double) x11;
                b1.y = (double) y11;
                b2.x = (double) x12;
                b2.y = (double) y12;
                p = intersectionPoint(a1, a2, b1, b2);
            }

            if (isnan(p.x)) return 1;

            n1 = 1;

            switch (v->style) {
                case 10:
                case 17: //slab load
                case 20: //Z load
                    if (v->flags & 1) {
                        df_x0 = p.x;
                        df_y0 = p.y;

                        df_x2 = df_x;
                        df_y2 = df_y;

                        if ((!(v->cartflags & 1))) ////|| (v->style!=10))
                            df_x1 = df_x;
                        df_x1 = p.x - n1 * ra1 * koc;

                        if ((p.y - df_y) > -0.001) {
                            df_y1 = p.y - ra1 * kos;
                            if (x1 > x2) n1 = -1;
                        } else {
                            df_y1 = p.y + ra1 * kos;
                            if (x1 < x2) n1 = -1;
                        }
                        n1 *= -1;
                    } else {
                        df_x0 = df_x;
                        df_y0 = df_y;

                        df_x2 = p.x;
                        df_y2 = p.y;

                        if ((!(v->cartflags & 1)))
                        {
                            df_x1 = df_x;
                        }
                        else
                        {
                            if (x1 < x2)
                            {
                                if (Check_if_GE((p.y - df_y),0.))
                                    df_x1 = df_x + n1 * ra1 * koc;
                                else
                                    df_x1 = df_x - n1 * ra1 * koc;
                            }
                            else
                            {
                                if (Check_if_GE((p.y - df_y),0.))
                                    df_x1 = df_x + n1 * ra1 * koc;
                                else
                                    df_x1 = df_x - n1 * ra1 * koc;
                            }
                        }

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

                        if (!(v->cartflags & 1))
                        {
                            df_y1 = df_y;
                        }
                        else
                        {
                            if (y1 < y2)
                            {
                                df_y1 = p.y - n1 * ra1 * kos;
                            }
                            else
                            {
                                df_y1 = p.y + n1 * ra1 * kos;
                            }
                        }

                        if ((p.x - df_x) > 0.001) {
                            df_x1 = p.x - ra1 * koc;
                            if (y1 < y2) n1 = -1;
                        } else {
                            df_x1 = p.x + ra1 * koc;
                            if (y1 > y2) n1 = -1;
                        }

                        n1 *= -1;
                    } else {
                        df_x0 = df_x;
                        df_y0 = df_y;

                        df_x2 = p.x;
                        df_y2 = p.y;

                        if (!(v->cartflags & 1))
                            df_y1 = df_y;
                        else
                        {
                            if (y1 < y2)
                            {
                                if ((p.x - df_x) > 0.)
                                     df_y1 = df_y + n1 * ra1 * kos;
                                else
                                    df_y1 = df_y - n1 * ra1 * kos;
                            }
                            else
                            {
                                if ((p.x - df_x) > 0.)
                                   df_y1 = df_y + n1 * ra1 * kos;
                                else
                                   df_y1 = df_y - n1 * ra1 * kos;
                            }
                        }

                        if ((p.x - df_x) > 0.001) {
                            df_x1 = df_x + ra1 * koc;
                            if (y1 < y2) n1 = -1;
                        } else {
                            df_x1 = df_x - ra1 * koc;
                            if (y1 > y2) n1 = -1;
                        }

                    }
                    break;
                case 0:
                case 12:
                case V_EDGE_SIMPLE:  //simple supported edge
                case V_EDGE_SIMPLE_INV:  //simple supported edge reversed
                case V_EDGE_FIXED:  //fixed edge
                case V_EDGE_FIXED_INV:  //fixed edge reversed
                case V_EDGE_ROLL:  //roll edge
                case V_EDGE_ROLL_INV:  //roll edge reversed
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

                            if (!(v->cartflags & 1))
                            {
                                if (fabs(del_angle - 90) > 1.0) n1 = -1;
                            }
                            else
                            {
                                if ((fabs(del_angle) > (120.0 + 1)) || (fabs(del_angle)) < (60.0 - 1)) n1 = -1;
                            }
                        }

                        df_x1 = p.x - n1 * ra1 * koc;
                        df_y1 = p.y - n1 * ra1 * kos;

                        n1 *= -1;
                    } else {
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

                            if (!(v->cartflags & 1))
                            {
                                if (fabs(del_angle - 90) > 1.0) n1 = -1;
                            }
                            else
                            {
                                if ((fabs(del_angle) > (120.0 + 1)) || (fabs(del_angle)) < (60.0 - 1)) n1 = -1;
                            }
                        }

                        df_x1 = df_x + n1 * ra1 * koc;
                        df_y1 = df_y + n1 * ra1 * kos;
                    }
                    break;
                case 13:
                    if (v->flags & 1) {
                        df_x0 = p.x;
                        df_y0 = p.y;

                        df_x2 = df_x;
                        df_y2 = df_y;

                        if ((!(v->cartflags & 1)))
                            df_x1 = df_x;

                        if (x1 < x2) {
                            if (p.y < df_y) n1 = -1;
                            df_y1 = p.y - n1 * ra1 * kos;
                        } else {
                            if (p.y > df_y) n1 = -1;
                            df_y1 = p.y + n1 * ra1 * kos;
                        }
                        n1 *= -1;
                    } else {
                        df_x0 = df_x;
                        df_y0 = df_y;

                        df_x2 = p.x;
                        df_y2 = p.y;

                        if ((!(v->cartflags & 1)))
                        {
                            df_x1 = df_x;

                            if (x1 < x2) {
                                if (p.y < df_y) n1 = -1;
                                df_y1 = df_y + n1 * ra1 * kos;
                            } else {
                                if (p.y > df_y) n1 = -1;
                                df_y1 = df_y - n1 * ra1 * kos;
                            }
                        }
                        else
                        {
                            double iso_x1, iso_y1, iso_x2, iso_y2;
                            double iso_df_x, iso_df_y, iso_p_x, iso_p_y;
                            int ret=cartesian_to_isometric(x1, y1, &iso_x1, &iso_y1);
                            ret=cartesian_to_isometric(x2, y2, &iso_x2, &iso_y2);
                            ret=cartesian_to_isometric(df_x, df_y, &iso_df_x, &iso_df_y);
                            ret=cartesian_to_isometric(p.x, p.y, &iso_p_x, &iso_p_y);

                            if (iso_x1 < iso_x2)
                            {
                                if (Check_if_LT(iso_p_y, iso_df_y)) n1 = -1;
                                df_x1 = df_x + n1 * ra1 * koc;
                                df_y1 = df_y + n1 * ra1 * kos;
                            }
                            else
                            {
                                if (Check_if_LT(iso_p_y, iso_df_y)) n1 = -1;
                                df_x1 = df_x + n1 * ra1 * koc;
                                df_y1 = df_y + n1 * ra1 * kos;
                            }

                        }
                    }
                    break;
                case 14:
                    if (v->flags & 1) {
                        df_x0 = p.x;
                        df_y0 = p.y;

                        df_x2 = df_x;
                        df_y2 = df_y;

                        if ((!(v->cartflags & 1)))
                           df_y1 = df_y;

                        if (y1 < y2) {
                            if (p.x > df_x) n1 = -1;
                            df_x1 = p.x + n1 * ra1 * koc;
                        } else {
                            if (p.x < df_x) n1 = -1;
                            df_x1 = p.x - n1 * ra1 * koc;
                        }
                        n1 *= -1;
                    } else {
                        df_x0 = df_x;
                        df_y0 = df_y;

                        df_x2 = p.x;
                        df_y2 = p.y;

                        if ((!(v->cartflags & 1))) {
                            df_y1 = df_y;

                            if (y1 < y2) {
                                if (p.x > df_x) n1 = -1;
                                df_x1 = df_x - n1 * ra1 * koc;
                            } else {
                                if (p.x < df_x) n1 = -1;
                                df_x1 = df_x + n1 * ra1 * koc;
                            }
                        }
                        else
                        {
                            double iso_x1, iso_y1, iso_x2, iso_y2;
                            double iso_df_x, iso_df_y, iso_p_x, iso_p_y;
                            int ret=cartesian_to_isometric(x1, y1, &iso_x1, &iso_y1);
                            ret=cartesian_to_isometric(x2, y2, &iso_x2, &iso_y2);
                            ret=cartesian_to_isometric(df_x, df_y, &iso_df_x, &iso_df_y);
                            ret=cartesian_to_isometric(p.x, p.y, &iso_p_x, &iso_p_y);


                            if (iso_y1 < iso_y2)
                            {
                                if (Check_if_LE(iso_p_x, iso_df_x)) n1 = -1;
                                df_x1 = df_x + n1 * ra1 * koc;
                                df_y1 = df_y + n1 * ra1 * kos;
                            }
                            else
                            {
                                if (Check_if_LE(iso_p_x, iso_df_x)) n1 = -1;
                                df_x1 = df_x + n1 * ra1 * koc;
                                df_y1 = df_y + n1 * ra1 * kos;
                            }
                        }
                    }

                    break;
                case 15:

                    df_x0 = df_x;
                    df_y0 = df_y;

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

                    df_x1 = df_x + n1 * ra1 * 0.5 * koc;
                    df_y1 = df_y + n1 * ra1 * 0.5 * kos;

                    df_x2 = p.x - n1 * ra1 * 0.5 * koc;
                    df_y2 = p.y - n1 * ra1 * 0.5 * kos;

                    break;
                default:
                    df_x0 = df_x;
                    df_y0 = df_y;

                    df_x2 = p.x;
                    df_y2 = p.y;

                    df_x1 = df_x;
                    if (x1 < x2) df_y1 = df_y - ra1 * kos;
                    else df_y1 = df_y + ra1 * kos;
                    break;
            }

            if (v->style != 15)
            {
                if (v->style != 0)
                    draw_arrow(df_x0, df_y0, df_x1, df_y1, df_x2, df_y2, angle, katS, koc1, kos1, koc2, kos2, n1 * ra, v, mode, kolor, vkolor);
                else
                    draw_spring(df_x0, df_y0, df_x1, df_y1, df_x2, df_y2, angle, katS, koc1, kos1, koc2, kos2, /*n1 **/ ra, v, mode, kolor, vkolor);
            }
            else draw_wave(df_x0, df_y0, df_x1, df_y1, df_x2, df_y2, koc, kos, n1, ra, v, mode, kolor, vkolor);
        }

        df_l0 += df_seg_len_dens ;
        i++ ;

    }
    while (TRUE == Check_if_GT (df_line_rem, (v->style<V_EDGE_SIMPLE) ? df_seg_len/2. : THE_END /*-df_seg_len/2.*/)) ;

	return 1;

ending:
	return 0;
}

void create_solid_on_line (LINIA *L, WIELOKAT *s, double width1, double width2, double axis)
/*----------------------------------------------------------------------------------------*/
{
    double dx1, dy1, dx2, dy2, df_l1 ;
    double angle_r, si_r, co_r, angle_b, si_b, co_b ;
    double df_sx1, df_sy1, df_sx2, df_sy2 ;

    dx1 = L->x2 - L->x1 ;
    dy1 = L->y2 - L->y1 ;
    df_l1 = sqrt (dx1 * dx1 + dy1 * dy1) ;
    angle_r = Atan2 (dy1, dx1) ;
    si_r = sin (angle_r) ;
    co_r = cos (angle_r) ;

    df_sx1 = 0 ;
    df_sx2 = 0 ;

    df_sy1 = (width1 / 2) - axis;
    df_sy2 = (-width1 / 2) - axis;

    obrd (si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1) ;
    obrd (si_r, co_r, df_sx2, df_sy2, &df_sx2, &df_sy2) ;
    s->xy [0] = df_sx1 + L->x1 ;
    s->xy [1] = df_sy1 + L->y1 ;
    s->xy [2] = df_sx2 + L->x1 ;
    s->xy [3] = df_sy2 + L->y1 ;

    dx2 = L->x2 - L->x1 ;
    dy2 = L->y2 - L->y1 ;
    obru (si_r, co_r, dx2, dy2, &dx2, &dy2) ;
    angle_b = Atan2 (dy2 - 0, dx2 - df_l1) ;
    if (angle_b > 0)
    {
        angle_b = (Pi2 / 2 - angle_b) / 2 + angle_b ;
    }
    else
    {
        angle_b = (Pi2 / 2 + angle_b) / 2 + Pi2 / 2 ;
    }
    si_b = sin (angle_b) ;
    co_b = cos (angle_b) ;
    if (TRUE == Check_if_Equal (si_b, 0))
    {
        df_sx1 = df_l1 ;
        df_sx2 = df_l1 ;
    }
    else
    {
        df_sx1 =  df_l1 - (co_b / si_b) * ((width2 / 2) + axis);
        df_sx2 = df_l1 + (co_b / si_b) * ((width2 / 2) - axis);
    }

    df_sy1 = (- width2 / 2) - axis;
    df_sy2 = (width2 / 2) - axis;
    obrd (si_r, co_r, df_sx1, df_sy1, &df_sx1, &df_sy1) ;
    obrd (si_r, co_r, df_sx2, df_sy2, &df_sx2, &df_sy2) ;
    s->xy [4] = df_sx1 + L->x1 ;
    s->xy [5] = df_sy1 + L->y1 ;
    s->xy [6] = df_sx2 + L->x1 ;
    s->xy [7] = df_sy2 + L->y1 ;
}

void create_solid_on_line_isometric (LINIA *L, WIELOKAT *s, double width1, double width2, double axis) {
    LINIA L_iso;
    cartesian_to_isometric_f(L->x1, L->y1, &L_iso.x1, &L_iso.y1);
    cartesian_to_isometric_f(L->x2, L->y2, &L_iso.x2, &L_iso.y2);

    create_solid_on_line(&L_iso, s, width1, width2, axis);

    for (int i = 0; i < 4; i++) {
        float cx, cy;
        isometric_to_cartesian_f(s->xy[2*i], s->xy[2*i+1], &cx, &cy);
        s->xy[2*i] = cx;
        s->xy[2*i+1] = cy;
    }
}

void set_decimal_format(char *text, double l, double precision)
{
    int len ;
    int lx,z;
    int lg;
    int d;
    char d_txt[64];


    if (precision<1.0) {
        sprintf(d_txt, "%d", (int) (1 / precision));
        d = strlen(d_txt);
    }
    else d=0;

    l = round_to_fraction(l, precision);
    sprintf(text, "%.*f", d, l);
    lx = strlen(text);

    if (NULL != strchr(text, '.')) {
        len = strlen(text);
        while (text[len - 1] == '0') {
            text[--len] = '\0';
        }
        if (text[len - 1] == '.') {
            text[len - 1] = '\0';
        }
    }
}


int trapezoid_base_isometric_x(float x1, float y1, float x2, float y2,
                               float *base_x1, float *base_y1, float *base_x2, float *base_y2)
{
    double ix1, iy1, ix2, iy2, iy;
    double cx1, cy1, cx2, cy2;
    int ret;

    ret=cartesian_to_isometric(x1,y1,&ix1, &iy1);
    ret=cartesian_to_isometric(x2,y2,&ix2, &iy2);

    if (ix1<ix2) iy=max(iy1, iy2);
    else iy=min(iy1, iy2);

    isometric_to_cartesian(ix1, iy, &cx1, &cy1);
    isometric_to_cartesian(ix2, iy, &cx2, &cy2);
    *base_x1=(float)cx1;
    *base_y1=(float)cy1;
    *base_x2=(float)cx2;
    *base_y2=(float)cy2;

    return 0;
}

int trapezoid_base_isometric_y(float x1, float y1, float x2, float y2,
                               float *base_x1, float *base_y1, float *base_x2, float *base_y2) {
    double ix1, iy1, ix2, iy2, ix;
    double cx1, cy1, cx2, cy2;
    int ret;

    ret=cartesian_to_isometric(x1, y1, &ix1, &iy1);
    ret=cartesian_to_isometric(x2, y2, &ix2, &iy2);

    if (iy1<iy2) ix = min(ix1, ix2);
    else ix = max(ix1, ix2);

    isometric_to_cartesian(ix, iy1, &cx1, &cy1);
    isometric_to_cartesian(ix, iy2, &cx2, &cy2);

    *base_x1 = (float)cx1;
    *base_y1 = (float)cy1;
    *base_x2 = (float)cx2;
    *base_y2 = (float)cy2;

    return 0;
}

int shorten_line_isometric(LINIA *L1, LINIA *L2i, double ra, int ends) {
    double len_iso = isometric_vector_length_f(L1->x1, L1->y1, L1->x2, L1->y2);
   // if (len_iso < 2 * ra) {
   //     L2i->x1 = L1->x1;
   //     L2i->y1 = L1->y1;
    //    L2i->x2 = L1->x2;
   //     L2i->y2 = L1->y2;
   //     return -1;  // Too short to cut
   // }

    double iso_angle_rad = isometric_vector_angle((double) L1->x1, (double) L1->y1, (double) L1->x2, (double) L1->y2)*M_PI/180;

    if (ends & 1) {
        // Cut from start: move forward along the vector by ra in iso space
        double angle_deg = iso_angle_rad * 180.0 / M_PI;
        double x1, y1;
        isometric_polar_to_cartesian((double)L1->x1, (double)L1->y1, ra, angle_deg, &x1, &y1);
        L2i->x1 = (float) x1;
        L2i->y1 = (float) y1;
    } else {
        L2i->x1 = L1->x1;
        L2i->y1 = L1->y1;
    }

    if (ends & 2) {
        // Cut from end: move backward along the vector by ra in iso space
        double opp_angle_rad = fmod(iso_angle_rad + M_PI, 2.0 * M_PI);
        double opp_angle_deg = opp_angle_rad * 180.0 / M_PI;
        double x2, y2;
        isometric_polar_to_cartesian((double)L1->x2, (double)L1->y2, ra, opp_angle_deg, &x2, &y2);
        L2i->x2 = (float) x2;
        L2i->y2 = (float) y2;
    }
    else {
        L2i->x2 = L1->x2;
        L2i->y2 = L1->y2;
    }

    return 0;
}


/*
 * Compute the angle (in radians) of the vector perpendicular to the ellipse at the end point.
 * The perpendicular is +90° CCW from the tangent.
 * Inputs are in radians.
 * Returns angle normalized to [0, 2π).
 */
double ellipse_normal_angle(double x, double y, double xr, double yr, double theta, double end_angle) {
    double phi = end_angle;

    double dx_dphi = -xr * sin(phi) * cos(theta) - yr * cos(phi) * sin(theta);
    double dy_dphi = -xr * sin(phi) * sin(theta) + yr * cos(phi) * cos(theta);

    double tangent_angle = atan2(dy_dphi, dx_dphi);

    double normal_angle = tangent_angle + M_PI / 2.0;

    normal_angle = fmod(normal_angle, 2.0 * M_PI);
    if (normal_angle < 0.0) normal_angle += 2.0 * M_PI;

    return normal_angle;
}

#include <math.h>

/**
 * Calculates the absolute angle of the tangent line at the end of an elliptical arc.
 * @param main_axis_angle Rotation of the ellipse (radians).
 * @param xr Semi-radius along the main axis.
 * @param yr Semi-radius perpendicular to the main axis.
 * @param end_angle Geometric angle from center (relative to main_axis_angle).
 * @return Tangent angle in global coordinates (radians).
 */
double get_elliptical_tangent_angle(double main_axis_angle, double xr, double yr, double end_angle) {
    // Converting geometric vector angle to parametric angle 't'
    double t = atan2(xr * sin(end_angle), yr * cos(end_angle));

    // Finding local tangent angle: atan2(dy/dt, dx/dt)
    // dx/dt = -xr * sin(t), dy/dt = yr * cos(t)
    double local_tan = atan2(yr * cos(t), -xr * sin(t));

    // Adding main axis rotation for global orientation
    return main_axis_angle + local_tan;
}


/*
 * Compute the absolute tangent angle (in radians) at the end point of the ellipse.
 * The tangent is the direction of the curve at phi = end_angle.
 * Inputs are in radians.
 * Returns angle normalized to [0, 2π).
 */
double ellipse_tangent_angle(float x, float y, float rx, float ry, float theta, float end_angle) {
    double phi = end_angle;  // parametric angle relative to rotated axis

    double dx_dphi = -rx * sin(phi) * cosf(theta) - ry * cos(phi) * sinf(theta);
    double dy_dphi = -rx * sin(phi) * sinf(theta) + ry * cos(phi) * cosf(theta);

    double tangent_angle = atan2(dy_dphi, dx_dphi);

    tangent_angle = fmod(tangent_angle, 2.0 * M_PI);
    if (tangent_angle < 0.0) tangent_angle += 2.0 * M_PI;

    return tangent_angle;
}

/*
void rotate_dx_dy(double dx, double dy, double angle_rad, double *dx1, double *dy1) {
    *dx1 = dx * cos(angle_rad) - dy * sin(angle_rad);
    *dy1 = dx * sin(angle_rad) + dy * cos(angle_rad);
}
*/

// General function for arrowhead on elliptical arc (any plane)
// Assumes the elliptical arc endpoint (ea_end_x, ea_end_y) is precomputed
// and tangent_angle_rad = ea.angle + ea.kat2 (absolute screen tangent)
void create_arrowhead_ellipticalarc(ELLIPTICALARC *ea, enum PlaneType plane, double ea_end_x, double ea_end_y, double tangent_angle_rad, double Kp2s, double katS, WIELOKAT *w) {
    // Adjust tangent for plane-specific "forward" direction
    double plane_adjust = 0.0;
    switch (plane) {
        case XY_PLANE:
            plane_adjust = 0.0;
            break;
        case XZ_PLANE:
            plane_adjust = - M_PI / 6.0;  // -30° rad
            break;
        case YZ_PLANE:
            plane_adjust = M_PI / 6.0;  // +30° rad
            break;
    }

    tangent_angle_rad += plane_adjust;

    // Normalize
    tangent_angle_rad = fmod(tangent_angle_rad, 2.0 * M_PI);
    if (tangent_angle_rad < 0.0) tangent_angle_rad += 2.0 * M_PI;

    //tangent_angle_rad = ellipse_normal_angle(ea->x, ea->y, ea->rx, ea->ry, ea->angle, ea->kat2);

    // Convert to isometric angle for symmetric opening
    double iso_tangent = cartesian_angle_to_isometric_angle(tangent_angle_rad);

    // Backward in iso space (for base behind tip)
    //double iso_backward = fmod(iso_tangent + M_PI, 2.0 * M_PI);

    //double iso_backward = fmod(iso_tangent, 2.0 * M_PI);

    double iso_backward = fmod(iso_tangent - M_PI/2., 2.0 * M_PI);
    //double iso_backward = ellipse_normal_angle(ea->x, ea->y, ea->rx, ea->ry, ea->angle, ea->kat2);

    double katS_rad = katS * M_PI / 180.0;

    double iso_angle1 = fmod(iso_backward - katS_rad, 2.0 * M_PI);
    double iso_angle2 = fmod(iso_backward + katS_rad, 2.0 * M_PI);

    double iso_angle1_deg = iso_angle1 * 180.0 / M_PI;
    double iso_angle2_deg = iso_angle2 * 180.0 / M_PI;

    double x_tip1, y_tip1, x_tip2, y_tip2;
    isometric_polar_to_cartesian(ea_end_x, ea_end_y, Kp2s, iso_angle1_deg, &x_tip1, &y_tip1);
    isometric_polar_to_cartesian(ea_end_x, ea_end_y, Kp2s, iso_angle2_deg, &x_tip2, &y_tip2);

    // Shift so midpoint of base is at arc end
    double mid_x = (x_tip1 + x_tip2) / 2.0;
    double mid_y = (y_tip1 + y_tip2) / 2.0;

    double dxi = ea_end_x - mid_x;
    double dyi = ea_end_y - mid_y;

    w->xy[0] = (float)(x_tip1 + dxi);
    w->xy[1] = (float)(y_tip1 + dyi);
    w->xy[2] = (float)(ea_end_x + dxi);  // this will be offset tip
    w->xy[3] = (float)(ea_end_y + dyi);
    w->xy[4] = (float)(x_tip2 + dxi);
    w->xy[5] = (float)(y_tip2 + dyi);
}

void Draw_Vector (AVECTOR *ptrs_vector, int mode, int kolor, int redraw_obj)
/*-----------------------------------------------------------------------*/
{
    double df_psize, df_psize1 ;
    PLINIA PL, PL1, PLth;
    LINIA L1=Ldef, L2=Ldef, L3=Ldef, L4=Ldef, Lt=Ldef, Ls=Ldef, Lth=Ldef, Ln=Ldef, Ln1=Ldef, Lp=Ldef;
    SOLIDARC sa=sadef;
    WIELOKAT w=S4def;
    OKRAG O=Odef;
    ELLIPSE E=Edef;
    OKRAG K=Kdef;
    LUK l=ldef;
    ELLIPTICALARC ea=eldef;
    double kat1, kos, koc, kats, kata2;
    double ra;
    double katS=25.0;
    TEXT Vtxt=Tdef, Vtxt1=Tdef, Vltxt=Tdef;
    double n, dx, dy, dx1, dy1, dx2, dy2, Kps, Kp2s, Kp2sn, K1_5, Ltx, Lty;
    int kolorS;
    double koc1, kos1, koc2, kos2, koc1th, kos1th;
    int grubosc, s;
    double  xs, ys;
    int TRANS=25;
    float xmax, ymax;
    char* translucency_ptr;
    int translucency;
    int vkolor;
    double x2, y2;
    ELLIPSE load_ellipse=FEdef;
    int ret;
    double arrow_angle;
    double h_factor;
    double ea_start_x, ea_start_y, ea_end_x, ea_end_y;

#define arrowf 1.0

    if ((ptrs_vector->style > 9) && ((ptrs_vector->style < 16) || (ptrs_vector->style > 16)) && (kolor)) bitmap_vector_exist = TRUE;

    if (redraw_obj)
    {
        if ((ptrs_vector->warstwa == Current_Layer) || (options1.view_only_current_layer == 0)) {
            if ((ptrs_vector->atrybut == Ablok) ||
                ((dragging_quad_move == TRUE) && (ptrs_vector->atrybut == Aoblok))) {

                vkolor = kolory.blok;
                mode=XOR_PUT;

            } else {
                vkolor = ptrs_vector->kolor;
            }
        } else {
            vkolor = GetColorAC(8);
        }
    }
    else vkolor = ptrs_vector->kolor;

    ra=Get_Point_Size () / arrowf *view_vector_scale;

    df_psize = Get_Point_Size () / 2 *view_vector_scale;   //2
    df_psize1 = Get_Point_Size () / 4 *view_vector_scale ; //2

    L1.warstwa=O.warstwa=E.warstwa=l.warstwa=K.warstwa=w.warstwa=sa.warstwa=ea.warstwa=ptrs_vector->warstwa;
    L1.kolor=O.kolor=E.kolor=l.kolor=K.kolor=w.kolor=sa.kolor=ea.kolor=vkolor; //ptrs_vector->kolor;
    L1.typ=ptrs_vector->typ;
    L1.atrybut=O.atrybut=E.atrybut=l.atrybut=K.atrybut=w.atrybut=sa.atrybut=ea.atrybut=ptrs_vector->atrybut;

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

    if (((ptrs_vector->style==4) || (ptrs_vector->style==7) || (ptrs_vector->style==18)) && (!Check_if_Equal(view_vector_scale, 1.0)))
    {
        Scale_Point (view_vector_scale, view_vector_scale ,ptrs_vector->x1,ptrs_vector->y1,ptrs_vector->x2,ptrs_vector->y2,&x2,&y2);
        L1.x2=(float)x2;  L1.y2=(float)y2;
    }
    else
    {
        L1.x2=ptrs_vector->x2;
        L1.y2=ptrs_vector->y2;
    }

    parametry_lini(&L1, &PL);
    kat1=PL.kat;
    kos=sin(PL.kat*Pi/180);
    koc=cos(PL.kat*Pi/180);

    Vtxt.kat=(float)normalize_txt_angle(PL.kat*Pi/180);
    Vtxt.justowanie=j_srodkowo;

    Vtxt.warstwa=ptrs_vector->warstwa;
    Vtxt.kolor=vkolor; //ptrs_vector->kolor;
    Vtxt.atrybut=ptrs_vector->atrybut;
    Vtxt.czcionka=zmwym.czcionka;
    Vtxt.wysokosc=(float)(zmwym.wysokosc*view_vector_scale);
    Vtxt.width_factor=(float)(zmwym.width_factor);
    memmove(&Vtxt1, &Vtxt, sizeof(TEXT));
    memmove(&Vltxt, &Vtxt, sizeof(TEXT));
    Vltxt.wysokosc=Vltxt.wysokosc/2;

    strcpy(Vtxt.text, "");
    strcpy(Vtxt1.text, "");

    switch (ptrs_vector->style)
    {
        double x1, y1, tangent;

        int plane;

        case 0:
        case 1:
        case 2:
        case 3:
            Vtxt.x=(L1.x1+L1.x2)/2;
            Vtxt.y=(L1.y1+L1.y2)/2;
            if (ptrs_vector->property_no>0) sprintf(Vtxt.text, "#%d", ptrs_vector->property_no);
           break;
        case 4: //force
        case 18:  //slab force
        case 19: //force z
            kos1=sin(Angle_Normal((PL.kat-90)*Pi/180.));
            koc1=cos(Angle_Normal((PL.kat-90)*Pi/180.));

            if (L1.y1>L1.y2) {
                if (L1.x1 < L1.x2) n = 1; else n = -1;  //(L1.x1<=L1.x2)
            }
            else
            {
                if (L1.x1 <= L1.x2) n = 1; else n = -1;
            }

            Vtxt.x=(float)((L1.x1+L1.x2)/2.-((n*ra/4.)*koc1));
            Vtxt.y=(float)((L1.y1+L1.y2)/2.-((n*ra/4.)*kos1));
            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, force_precision);
            normalize_txt(&Vtxt);

            Vltxt.x=(float)((L1.x1+L1.x2)/2.+n*((Vltxt.wysokosc+ra/4.)*koc1));
            Vltxt.y=(float)((L1.y1+L1.y2)/2.+n*((Vltxt.wysokosc+ra/4.)*kos1));

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);

            break;
        case 7:  //displacement
        case 27:  //displacement z
            kos1=sin(Angle_Normal((PL.kat-90)*Pi/180));
            koc1=cos(Angle_Normal((PL.kat-90)*Pi/180));

            //if (L1.x1<=L1.x2) n=1; else n=-1;
            if (L1.y1>L1.y2) {
                if (L1.x1 < L1.x2) n = 1; else n = -1;  //(L1.x1<=L1.x2)
            }
            else
            {
                if (L1.x1 <= L1.x2) n = 1; else n = -1;
            }

            Vtxt.x=(float)((L1.x1+L1.x2)/2.f-((n*ra/4)*koc1));
            Vtxt.y=(float)((L1.y1+L1.y2)/2.f-((n*ra/4)*kos1));
            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, displacement_precision);
            normalize_txt(&Vtxt);

            Vltxt.x=(float)((L1.x1+L1.x2)/2.f+n*((Vltxt.wysokosc+ra/4)*koc1));
            Vltxt.y=(float)((L1.y1+L1.y2)/2.f+n*((Vltxt.wysokosc+ra/4)*kos1));

            if (ptrs_vector->variant>0)
                sprintf((char*)&Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf((char*)&Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 5:  //moment
        case 6:  //-moment
            if (ptrs_vector->angle2<ptrs_vector->angle1)
                kata2=ptrs_vector->angle2+Pi2;
            else kata2=ptrs_vector->angle2;
            kats=Angle_Normal((ptrs_vector->angle1+kata2)/2);
            Vtxt.x=ptrs_vector->x1+(float)((ptrs_vector->r+0.5)*cos(kats));
            Vtxt.y=ptrs_vector->y1+(float)((ptrs_vector->r+0.5)*sin(kats));

            Vltxt.x=ptrs_vector->x1+(float)((ptrs_vector->r - Vltxt.wysokosc - 0.5)*cos(kats));
            Vltxt.y=ptrs_vector->y1+(float)((ptrs_vector->r - Vltxt.wysokosc - 0.5)*sin(kats));

            Vtxt.kat=Vltxt.kat=(float)Angle_Normal(kats-Pi_/2);
            Vtxt.justowanie=Vltxt.justowanie=j_srodkowo;
            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, moment_precision);
            normalize_txt(&Vtxt);

            if (ptrs_vector->variant>0)
               sprintf((char*)&Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf((char*)&Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 21:  //moment x
        case 23:  //moment y
        case 25:  //moment yy

        case 22:  //-moment x
        case 24:  //-moment y
        case 26:  //-moment xy

            switch (ptrs_vector->style) {
                case 21:  // moment x
                    plane = XZ_PLANE;
                    break;
                case 23:
                    plane = YZ_PLANE;
                    break;
                case 25:
                    plane = XY_PLANE;
                    break;
                case 22:  // moment x
                    plane = XZ_PLANE;
                    break;
                case 24:
                    plane = YZ_PLANE;
                    break;
                case 26:
                    plane = XY_PLANE;
                    break;
            }
            l.x=ptrs_vector->x1;
            l.y=ptrs_vector->y1;
            l.r=ptrs_vector->r;
            l.kat1=ptrs_vector->angle1;
            l.kat2=ptrs_vector->angle2;

            if (Check_if_Equal(ptrs_vector->angle1, ptrs_vector->angle2))
            {
                kats=Angle_Normal(ptrs_vector->angle1);
                x1=ptrs_vector->x1+ptrs_vector->r*cos(kats);
                y1=ptrs_vector->y1+ptrs_vector->r*sin(kats);
                tangent=kats - Pi_ / 2.;
                if (tangent < 0) tangent += 2.0 * M_PI;
            }
            else {
                ret = arc_to_isometric_ellipticalarc_a_ea(plane, &l, &ea);
                srodekea_(&x1, &y1, &tangent, &ea);
                tangent = fmod(tangent + M_PI, 2.0 * M_PI);
                kats = tangent + Pi_ / 2.;
                if (kats < 0) kats += 2.0 * M_PI;
            }
            Vtxt.x=(float)(x1+0.5*cos(kats));
            Vtxt.y=(float)(y1+0.5*sin(kats));

            Vltxt.x=(float)(x1+(-Vltxt.wysokosc - 0.5)*cos(kats));
            Vltxt.y=(float)(y1+(-Vltxt.wysokosc - 0.5)*sin(kats));

            Vtxt.kat=Vltxt.kat=(float)tangent;
            Vtxt.justowanie=Vltxt.justowanie=j_srodkowo;
            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, moment_precision);
            normalize_txt(&Vtxt);

            if (ptrs_vector->variant>0)
                sprintf((char*)&Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf((char*)&Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);

            break;
        case V_EDGE_ARC_SIMPLE:  //simple supported edge
        case V_EDGE_ARC_SIMPLE_INV:  //simple supported edge flipped
        case V_EDGE_ARC_FIXED:  //fixed edge
        case V_EDGE_ARC_FIXED_INV:  //fixed edge flipped
        case V_EDGE_ARC_ROLL:  //roll edge
        case V_EDGE_ARC_ROLL_INV:  //roll edge flipped
            if (ptrs_vector->angle2<ptrs_vector->angle1)
                kata2=ptrs_vector->angle2+Pi2;
            else kata2=ptrs_vector->angle2;
            kats=Angle_Normal((ptrs_vector->angle1+kata2)/2);
            break;
        case 8: //rotation
        case 9: //-rotation
        //case 28:  //rotation x
        //case 29:  //-rotation x
        //case 30:  //rotation y
        //case 31:  //-rotation y
        //case 32:  //rotation xy
        //case 33:  //-rotation xy
            if (ptrs_vector->angle2<ptrs_vector->angle1)
            kata2=ptrs_vector->angle2+Pi2;
            else kata2=ptrs_vector->angle2;
            kats=Angle_Normal((ptrs_vector->angle1+kata2)/2);
            Vtxt.x=ptrs_vector->x1+(float)((ptrs_vector->r+0.5)*cos(kats));
            Vtxt.y=ptrs_vector->y1+(float)((ptrs_vector->r+0.5)*sin(kats));

            Vltxt.x=ptrs_vector->x1+(float)((ptrs_vector->r - Vltxt.wysokosc - 0.5)*cos(kats));
            Vltxt.y=ptrs_vector->y1+(float)((ptrs_vector->r - Vltxt.wysokosc - 0.5)*sin(kats));

            Vtxt.kat=Vltxt.kat=(float)Angle_Normal(kats-Pi_/2);
            Vtxt.justowanie=Vltxt.justowanie=j_srodkowo;

            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, rotation_precision);
            normalize_txt(&Vtxt);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 28: //rotation x  //TO DO ISOMETRIC
        case 29: //-rotation x
        case 30: //rotation y
        case 31: //-rotation y
        case 32: //rotation xy
        case 33: //-rotation xy
            switch (ptrs_vector->style) {
                case 28:  // moment x //TO DO ISOMETRIC
                    plane = XZ_PLANE;
                    break;
                case 30:
                    plane = YZ_PLANE;
                    break;
                case 32:
                    plane = XY_PLANE;
                    break;
                case 29:  // moment x //TO DO ISOMETRIC
                    plane = XZ_PLANE;
                    break;
                case 31:
                    plane = YZ_PLANE;
                    break;
                case 33:
                    plane = XY_PLANE;
                    break;
            }
            l.x=ptrs_vector->x1;
            l.y=ptrs_vector->y1;
            l.r=ptrs_vector->r;
            l.kat1=ptrs_vector->angle1;
            l.kat2=ptrs_vector->angle2;

            if (Check_if_Equal(ptrs_vector->angle1, ptrs_vector->angle2))
            {
                kats=Angle_Normal(ptrs_vector->angle1);
                x1=ptrs_vector->x1+ptrs_vector->r*cos(kats);
                y1=ptrs_vector->y1+ptrs_vector->r*sin(kats);
                tangent=kats - Pi_ / 2.;
                if (tangent < 0) tangent += 2.0 * M_PI;
            }
            else {
                ret = arc_to_isometric_ellipticalarc_a_ea(plane, &l, &ea);
                srodekea_(&x1, &y1, &tangent, &ea);
                tangent = fmod(tangent + M_PI, 2.0 * M_PI);
                kats = tangent + Pi_ / 2.;
                if (kats < 0) kats += 2.0 * M_PI;
            }
            Vtxt.x=(float)(x1+0.5*cos(kats));
            Vtxt.y=(float)(y1+0.5*sin(kats));

            Vltxt.x=(float)(x1+(-Vltxt.wysokosc - 0.5)*cos(kats));
            Vltxt.y=(float)(y1+(-Vltxt.wysokosc - 0.5)*sin(kats));

            Vtxt.kat=Vltxt.kat=(float)tangent;
            Vtxt.justowanie=Vltxt.justowanie=j_srodkowo;
            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, moment_precision);
            normalize_txt(&Vtxt);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);

            break;
        case 10:  //trapezium Y
        case 17:  ////trapezium Y slab
        case 20:  ////trapezium Z

            if (Check_if_Equal(L1.x1, L1.x2))
            {
                L1.x2+=0.001f;

                parametry_lini(&L1, &PL);
                kat1=PL.kat;
                kos=sin(PL.kat*Pi/180.);
                koc=cos(PL.kat*Pi/180.);

            }

            if (L1.x1<L1.x2) n=1*view_vector_scale;
            else n=-1*view_vector_scale;

            if (ptrs_vector->flags & 1) n*=-1;

            if ((!(ptrs_vector->cartflags & 1)) || (ptrs_vector->style!=10))
            {
                Lt.x1 = L1.x1;
                Lt.y1 = (float)(L1.y1 + n * (ptrs_vector->magnitude1 / ((ptrs_vector->style == 10) ? load_magnitude : flood_magnitude)));
                Lt.x2 = L1.x2;
                Lt.y2 = (float)(L1.y2 + n * (ptrs_vector->magnitude2 / ((ptrs_vector->style == 10) ? load_magnitude : flood_magnitude)));
            }
            else
            {
                double dx1_cart, dy1_cart, dx2_cart, dy2_cart;
                ret = isometric_vector_to_cartesian(0,  n * (ptrs_vector->magnitude1 / ((ptrs_vector->style == 10) ? load_magnitude : flood_magnitude)),  &dx1_cart, &dy1_cart);
                ret = isometric_vector_to_cartesian(0,  n * (ptrs_vector->magnitude2 / ((ptrs_vector->style == 10) ? load_magnitude : flood_magnitude)),  &dx2_cart, &dy2_cart);
                Lt.x1 = (float)(L1.x1 + dx1_cart);
                Lt.y1 = (float)(L1.y1 + dy1_cart);
                Lt.x2 = (float)(L1.x2 + dx2_cart);
                Lt.y2 = (float)(L1.y2 + dy2_cart);
            }

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            if ((!(ptrs_vector->cartflags & 1)) || (ptrs_vector->style!=10))
            {
                //parametry_lini(&Lt, &PL1);
                kos1 = sin(Pi * (PL1.kat + 90.) / 180.);
                koc1 = cos(Pi * (PL1.kat + 90.) / 180.);
            }
            else {
                //kos1 = sin(Pi * (PL1.kat + 120.) / 180.);
                //koc1 = cos(Pi * (PL1.kat + 120.) / 180.);
                double iso_angle=cartesian_angle_to_isometric_angle(PL1.kat*M_PI/180.);
                double iso_perp = fmod(iso_angle + M_PI / 2.0, 2.0 * M_PI);
                if (iso_perp < 0.0) iso_perp += 2.0 * M_PI;
                double cart_angle=isometric_angle_to_cartesian_angle_rad(iso_perp);
                kos1 = sin(cart_angle);
                koc1 = cos(cart_angle);
            }

            if (ptrs_vector->cartflags & 1)
                h_factor=1.2;
            else h_factor=1.0;

            if (Lt.x1==Lt.x2)
            {

                    dx = 0;
                    dy = 0;

                    dx1 = 0;
                    dy1 = 0;
                    dx2 = 0;
                    dy2 = 0;

            }
            else if (Lt.x1<Lt.x2)
            {   if (Lt.y1<L1.y1)
                {
                    dx=-Vtxt.wysokosc*h_factor*koc1;
                    dy=-Vtxt.wysokosc*h_factor*kos1;
                }
                else
                {
                    dx = 0;
                    dy = 0;
                }
                if (Lt.y2<L1.y2)
                {
                    dx1=-Vtxt.wysokosc*h_factor*koc1;
                    dy1=-Vtxt.wysokosc*h_factor*kos1;

                    dx2=-Vltxt.wysokosc*h_factor*koc1;
                    dy2=-Vltxt.wysokosc*h_factor*kos1;
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
                    dx = Vtxt.wysokosc *h_factor* koc1;
                    dy = Vtxt.wysokosc *h_factor* kos1;
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
                    dx1 = Vtxt.wysokosc *h_factor* koc1;
                    dy1 = Vtxt.wysokosc *h_factor* kos1;
                    dx2 = Vltxt.wysokosc *h_factor* koc1;
                    dy2 = Vltxt.wysokosc *h_factor* kos1;
                }
            }

            Vtxt.x=(float)(Lt.x1+dx);
            Vtxt.y=(float)(Lt.y1+dy);
            Vtxt.kat=(float)normalize_txt_angle(PL1.kat*Pi/180.);
            if (Lt.x1<Lt.x2) Vtxt.justowanie=j_do_lewej;
            else Vtxt.justowanie=j_do_prawej;
            Vtxt1.x=(float)(Lt.x2+dx1);
            Vtxt1.y=(float)(Lt.y2+dy1);
            Vtxt1.kat=Vltxt.kat=(float)normalize_txt_angle(PL1.kat*Pi/180.);
            if (Lt.x1<Lt.x2) Vtxt1.justowanie=j_do_prawej;
            else  Vtxt1.justowanie=j_do_lewej;

            Vltxt.x=(float)(Ltx+dx2);
            Vltxt.y=(float)(Lty+dy2);
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

            if (Check_if_Equal(L1.y1, L1.y2))
            {
                L1.y2+=0.001f;

                parametry_lini(&L1, &PL);
                kat1=PL.kat;
                kos=sin(PL.kat*Pi/180);
                koc=cos(PL.kat*Pi/180);

            }

            if (L1.y1<L1.y2) n=1*view_vector_scale;
            else n=-1*view_vector_scale;

            if (ptrs_vector->flags & 1) n*=-1;

            if (!(ptrs_vector->cartflags & 1))
            {
                Lt.x1 = L1.x1 - (float)(n * (ptrs_vector->magnitude1 / load_magnitude));
                Lt.y1 = L1.y1;
                Lt.x2 = L1.x2 - (float)(n * (ptrs_vector->magnitude2 / load_magnitude));
                Lt.y2 = L1.y2;
            }
            else
            {
                double dx1_cart, dy1_cart, dx2_cart, dy2_cart;
                ret = isometric_vector_to_cartesian(- n * (ptrs_vector->magnitude1 / load_magnitude), 0, &dx1_cart, &dy1_cart);
                ret = isometric_vector_to_cartesian(- n * (ptrs_vector->magnitude2 / load_magnitude), 0, &dx2_cart, &dy2_cart);
                Lt.x1 = L1.x1 + (float)dx1_cart;
                Lt.y1 = L1.y1 + (float)dy1_cart;
                Lt.x2 = L1.x2 + (float)dx2_cart;
                Lt.y2 = L1.y2 + (float)dy2_cart;
            }

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            if (!(ptrs_vector->cartflags & 1))
            {
                kos1=sin(Pi*(PL1.kat+90.)/180.);
                koc1=cos(Pi*(PL1.kat+90.)/180.);
            }
            else {
                //kos1 = sin(Pi * (PL1.kat + 120.) / 180.);
                //koc1 = cos(Pi * (PL1.kat + 120.) / 180.);
                double iso_angle=cartesian_angle_to_isometric_angle(PL1.kat*M_PI/180.);
                double iso_perp = fmod(iso_angle + M_PI / 2.0, 2.0 * M_PI);
                if (iso_perp < 0.0) iso_perp += 2.0 * M_PI;
                double cart_angle=isometric_angle_to_cartesian_angle_rad(iso_perp);
                kos1 = sin(cart_angle);
                koc1 = cos(cart_angle);
            }

            if (ptrs_vector->cartflags & 1)
                h_factor=1.2;
            else h_factor=1.0;

            if (Lt.y1<Lt.y2)
            {   if (Lt.x1>L1.x1)
                {
                    dx=-Vtxt.wysokosc*h_factor*koc1;
                    dy=-Vtxt.wysokosc*h_factor*kos1;
                }
                else
                {
                    dx = 0;
                    dy = 0;
                }
                if (Lt.x2>L1.x2)
                {
                    dx1=-Vtxt.wysokosc*h_factor*koc1;
                    dy1=-Vtxt.wysokosc*h_factor*kos1;
                    dx2=-Vltxt.wysokosc*h_factor*koc1;
                    dy2=-Vltxt.wysokosc*h_factor*kos1;
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
                    dx = -Vtxt.wysokosc *h_factor * koc1;
                    dy = -Vtxt.wysokosc *h_factor * kos1;
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
                    dx1 = -Vtxt.wysokosc *h_factor* koc1;
                    dy1 = -Vtxt.wysokosc *h_factor* kos1;
                    dx2 = -Vltxt.wysokosc *h_factor* koc1;
                    dy2 = -Vltxt.wysokosc *h_factor* kos1;
                }
            }

            Vtxt.x=(float)(Lt.x1+dx);
            Vtxt.y=(float)(Lt.y1+dy);
            Vtxt.kat=(float)(PL1.kat*Pi/180.);
            Vtxt.justowanie=j_do_lewej;
            Vtxt1.x=(float)(Lt.x2+dx1);
            Vtxt1.y=(float)(Lt.y2+dy1);
            Vtxt1.kat=Vltxt.kat=(float)(PL1.kat*Pi/180.);
            Vtxt1.justowanie=j_do_prawej;

            Vltxt.x=(float)(Ltx+dx2);
            Vltxt.y=(float)(Lty+dy2);
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

            if (!(ptrs_vector->cartflags & 1))
            {
                arrow_angle = M_PI * (PL.kat + 90.0) / 180.0;
                kos1 = sin(arrow_angle);
                koc1 = cos(arrow_angle);
            }
            else {
                double iso_angle = cartesian_angle_to_isometric_angle(M_PI * PL.kat / 180.);
                // Add 90° in isometric space (counter-clockwise perpendicular)
                double iso_perp = fmod(iso_angle + M_PI / 2.0, 2.0 * M_PI);
                if (iso_perp < 0.0) iso_perp += 2.0 * M_PI;
                arrow_angle = isometric_angle_to_cartesian_angle_rad(iso_perp);
                kos1 = sin(arrow_angle);
                koc1 = cos(arrow_angle);
            }

            n=view_vector_scale;

            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = L1.x1 + (float)(n*(ptrs_vector->magnitude1/load_magnitude)*koc1);
            Lt.y1 = L1.y1 + (float)(n*(ptrs_vector->magnitude1/load_magnitude)*kos1);
            Lt.x2 = L1.x2 + (float)(n*(ptrs_vector->magnitude2/load_magnitude)*koc1);
            Lt.y2 = L1.y2 + (float)(n*(ptrs_vector->magnitude2/load_magnitude)*kos1);

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);

            if (!(ptrs_vector->cartflags & 1))
            {
                kos2=sin(Pi*(PL1.kat+90.)/180.);
                koc2=cos(Pi*(PL1.kat+90.)/180.);
            }
            else {
                double iso_angle = cartesian_angle_to_isometric_angle(M_PI * PL1.kat / 180.);
                // Add 90° in isometric space (counter-clockwise perpendicular)
                double iso_perp = fmod(iso_angle + M_PI / 2.0, 2.0 * M_PI);
                if (iso_perp < 0.0) iso_perp += 2.0 * M_PI;
                double cart_angle = isometric_angle_to_cartesian_angle_rad(iso_perp);
                kos2 = sin(cart_angle);
                koc2 = cos(cart_angle);
            }

            dx = 0;
            dy = 0;
            dx1 = 0;
            dy1 = 0;
            dx2 = 0;
            dy2 = 0;

            if (ptrs_vector->cartflags & 1)
                h_factor=1.2;
            else h_factor=1.0;

            if (((ptrs_vector->magnitude1<0) && !(ptrs_vector->flags & 1)) ||
                ((ptrs_vector->magnitude1>0) && (ptrs_vector->flags & 1)))
            {
                dx = -Vtxt.wysokosc*h_factor * koc2;
                dy = -Vtxt.wysokosc*h_factor * kos2;
            }

            if (((ptrs_vector->magnitude2<0) && !(ptrs_vector->flags & 1)) ||
                ((ptrs_vector->magnitude2>0) && (ptrs_vector->flags & 1)))
            {
                dx1 = -Vtxt.wysokosc*h_factor * koc2;
                dy1 = -Vtxt.wysokosc*h_factor * kos2;

                dx2 = -Vltxt.wysokosc*h_factor * koc2;
                dy2 = -Vltxt.wysokosc*h_factor * kos2;
            }

            Vtxt.x=Lt.x1+(float)dx;
            Vtxt.y=Lt.y1+(float)dy;
            Vtxt.kat=(float)(PL1.kat*Pi_/180.);
            Vtxt.justowanie=j_do_lewej;
            Vtxt1.x=Lt.x2+(float)dx1;
            Vtxt1.y=Lt.y2+(float)dy1;
            Vtxt1.kat=Vltxt.kat=(float)(PL1.kat*Pi_/180.);
            Vtxt1.justowanie=j_do_prawej;

            Vltxt.x=(float)(Ltx+dx2);
            Vltxt.y=(float)(Lty+dy2);
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
        case V_EDGE_SIMPLE:  //simple supported edge
        case V_EDGE_SIMPLE_INV:  //simple supported edge reversed
        case V_EDGE_FIXED:  //fixed edge
        case V_EDGE_FIXED_INV:  //fixed edge reversed
        case V_EDGE_ROLL:  //roll edge
        case V_EDGE_ROLL_INV:  //roll edge reversed
            if ((ptrs_vector->style==V_EDGE_SIMPLE) || (ptrs_vector->style==V_EDGE_FIXED))
               ptrs_vector->magnitude1=ptrs_vector->magnitude2=0.1f;  //just to simulate
               else ptrs_vector->magnitude1=ptrs_vector->magnitude2=-0.1f;  //just to simulate
            kos1=sin(Pi*(PL.kat+90)/180.);
            koc1=cos(Pi*(PL.kat+90)/180.);

            n=view_vector_scale;

            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = L1.x1 + (float)(n*(ptrs_vector->magnitude1/load_magnitude)*koc1);
            Lt.y1 = L1.y1 + (float)(n*(ptrs_vector->magnitude1/load_magnitude)*kos1);
            Lt.x2 = L1.x2 + (float)(n*(ptrs_vector->magnitude2/load_magnitude)*koc1);
            Lt.y2 = L1.y2 + (float)(n*(ptrs_vector->magnitude2/load_magnitude)*kos1);

            Ltx=(Lt.x1 + Lt.x2)/2.;
            Lty=(Lt.y1 + Lt.y2)/2.;

            parametry_lini(&Lt, &PL1);
            kos2=sin(Pi*(PL1.kat+90)/180.);
            koc2=cos(Pi*(PL1.kat+90)/180.);
            break;

        case 13:  //trapezium H

            if (!(ptrs_vector->cartflags & 1))
            {
                if (L1.x1 < L1.x2) {
                    n = 1 * view_vector_scale;
                    ymax = max(L1.y1, L1.y2);
                } else {
                    n = -1 * view_vector_scale;
                    ymax = min(L1.y1, L1.y2);
                }

                if (ptrs_vector->flags & 1) n *= -1;

                Lt.x1 = L1.x1;
                Lt.y1 = (float)(ymax + n * (ptrs_vector->magnitude1 / load_magnitude));
                Lt.x2 = L1.x2;
                Lt.y2 = (float)(ymax + n * (ptrs_vector->magnitude2 / load_magnitude));
            }
            else
            {
                //projection on x axis
                double iso_L1_x1, iso_L1_y1, iso_L1_x2, iso_L1_y2;
                ret=cartesian_to_isometric(L1.x1, L1.y1, &iso_L1_x1, &iso_L1_y1);
                ret=cartesian_to_isometric(L1.x2, L1.y2, &iso_L1_x2, &iso_L1_y2);

                if (iso_L1_x1<iso_L1_x2) n=1*view_vector_scale;
                else n=-1*view_vector_scale;
                
                ret = trapezoid_base_isometric_x(L1.x1, L1.y1, L1.x2, L1.y2,&Lp.x1, &Lp.y1, &Lp.x2, &Lp.y2);
                
                double dx1_cart, dy1_cart, dx2_cart, dy2_cart;
                ret = isometric_vector_to_cartesian(0.,  n * (ptrs_vector->magnitude1 / load_magnitude),  &dx1_cart, &dy1_cart);
                ret = isometric_vector_to_cartesian(0.,  n * (ptrs_vector->magnitude2 / load_magnitude),  &dx2_cart, &dy2_cart);
                Lt.x1 = Lp.x1 + (float)dx1_cart;
                Lt.y1 = Lp.y1 + (float)dy1_cart;
                Lt.x2 = Lp.x2 + (float)dx2_cart;
                Lt.y2 = Lp.y2 + (float)dy2_cart;
            }

            Ltx=(Lt.x1 + Lt.x2)/2.;
            Lty=(Lt.y1 + Lt.y2)/2.;

            parametry_lini(&Lt, &PL1);
            if (!(ptrs_vector->cartflags & 1))
            {
                kos1 = sin(Pi * (PL1.kat + 90) / 180.);
                koc1 = cos(Pi * (PL1.kat + 90) / 180.);
            }
            else {
                //kos1 = sin(Pi * (PL1.kat + 120.) / 180.);
                //koc1 = cos(Pi * (PL1.kat + 120.) / 180.);
                double iso_angle=cartesian_angle_to_isometric_angle(PL1.kat*M_PI/180.);
                double iso_perp = fmod(iso_angle + M_PI / 2.0, 2.0 * M_PI);
                if (iso_perp < 0.0) iso_perp += 2.0 * M_PI;
                double cart_angle=isometric_angle_to_cartesian_angle_rad(iso_perp);
                kos1 = sin(cart_angle);
                koc1 = cos(cart_angle);
            }

            dx=0;
            dy=0;
            dx1=0;
            dy1=0;
            dx2=0;
            dy2=0;

            if (ptrs_vector->cartflags & 1)
                h_factor=1.2;
            else h_factor=1.0;

            if (Lt.x1<Lt.x2)
            {
                if (((ptrs_vector->magnitude1<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude1>0) && (ptrs_vector->flags & 1)))
                {
                    dx=-Vtxt.wysokosc*h_factor*koc1;
                    dy=-Vtxt.wysokosc*h_factor*kos1;
                }
                if (((ptrs_vector->magnitude2<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude2>0) && (ptrs_vector->flags & 1)))
                {
                    dx1=-Vtxt.wysokosc*h_factor*koc1;
                    dy1=-Vtxt.wysokosc*h_factor*kos1;

                    dx2=-Vltxt.wysokosc*h_factor*koc1;
                    dy2=-Vltxt.wysokosc*h_factor*kos1;
                }
            }
            else
            {

                if (((ptrs_vector->magnitude1>=0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude1<0) && (ptrs_vector->flags & 1)))
                {
                    dx = Vtxt.wysokosc *h_factor* koc1;
                    dy = Vtxt.wysokosc *h_factor* kos1;
                }

                if (((ptrs_vector->magnitude2>=0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude2<0) && (ptrs_vector->flags & 1)))
                {
                    dx1 = Vtxt.wysokosc *h_factor* koc1;
                    dy1 = Vtxt.wysokosc *h_factor* kos1;

                    dx2 = Vltxt.wysokosc *h_factor* koc1;
                    dy2 = Vltxt.wysokosc *h_factor* kos1;
                }
            }

            Vtxt.x=Lt.x1+(float)dx;
            Vtxt.y=Lt.y1+(float)dy;
            Vtxt.kat=(float)normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt.justowanie=j_do_lewej;
            else Vtxt.justowanie=j_do_prawej;
            Vtxt1.x=Lt.x2+(float)dx1;
            Vtxt1.y=Lt.y2+(float)dy1;
            Vtxt1.kat=Vltxt.kat=(float)normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt1.justowanie=j_do_prawej;
            else  Vtxt1.justowanie=j_do_lewej;

            Vltxt.x=(float)(Ltx+dx2);
            Vltxt.y=(float)(Lty+dy2);
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

            if (!(ptrs_vector->cartflags & 1))
            {
                if (L1.y1<L1.y2)
                {
                    n=1*view_vector_scale;
                    xmax=min(L1.x1, L1.x2);
                }
                else
                {
                    n=-1*view_vector_scale;
                    xmax=max(L1.x1, L1.x2);
                }

                if (ptrs_vector->flags & 1) n*=-1;

                Lt.x1 = (float)(xmax - n * (ptrs_vector->magnitude1 / load_magnitude));
                Lt.y1 = L1.y1;
                Lt.x2 = (float)(xmax - n * (ptrs_vector->magnitude2 / load_magnitude));
                Lt.y2 = L1.y2;
            }
                else
                {
                    //projection on x axis
                    double iso_L1_x1, iso_L1_y1, iso_L1_x2, iso_L1_y2;
                    ret=cartesian_to_isometric(L1.x1, L1.y1, &iso_L1_x1, &iso_L1_y1);
                    ret=cartesian_to_isometric(L1.x2, L1.y2, &iso_L1_x2, &iso_L1_y2);

                    if (iso_L1_y1<iso_L1_y2) n=1*view_vector_scale;
                    else n=-1*view_vector_scale;

                    ret = trapezoid_base_isometric_y(L1.x1, L1.y1, L1.x2, L1.y2,&Lp.x1, &Lp.y1, &Lp.x2, &Lp.y2);

                    double dx1_cart, dy1_cart, dx2_cart, dy2_cart;
                    ret = isometric_vector_to_cartesian(-n * (ptrs_vector->magnitude1 / load_magnitude), 0., &dx1_cart, &dy1_cart);
                    ret = isometric_vector_to_cartesian(-n * (ptrs_vector->magnitude2 / load_magnitude), 0., &dx2_cart, &dy2_cart);
                    Lt.x1 = Lp.x1 + (float)dx1_cart;
                    Lt.y1 = Lp.y1 + (float)dy1_cart;
                    Lt.x2 = Lp.x2 + (float)dx2_cart;
                    Lt.y2 = Lp.y2 + (float)dy2_cart;
                }

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            if (!(ptrs_vector->cartflags & 1))
            {
                kos1 = sin(Pi * (PL1.kat + 90) / 180);
                koc1 = cos(Pi * (PL1.kat + 90) / 180);
            }
            else {
                //kos1 = sin(Pi * (PL1.kat + 120.) / 180.);
                //koc1 = cos(Pi * (PL1.kat + 120.) / 180.);
                double iso_angle=cartesian_angle_to_isometric_angle(PL1.kat*M_PI/180.);
                double iso_perp = fmod(iso_angle + M_PI / 2.0, 2.0 * M_PI);
                if (iso_perp < 0.0) iso_perp += 2.0 * M_PI;
                double cart_angle=isometric_angle_to_cartesian_angle_rad(iso_perp);
                kos1 = sin(cart_angle);
                koc1 = cos(cart_angle);
            }

            dx=0;
            dy=0;
            dx1=0;
            dy1=0;
            dx2=0;
            dy2=0;

            if (ptrs_vector->cartflags & 1)
                h_factor=1.2;
            else h_factor=1.0;

            if (Lt.y1<Lt.y2)
            {

                if (((ptrs_vector->magnitude1<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude1>0) && (ptrs_vector->flags & 1)))
                {
                    dx=-Vtxt.wysokosc*h_factor*koc1;
                    dy=-Vtxt.wysokosc*h_factor*kos1;
                }

                if (((ptrs_vector->magnitude2<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude2>0) && (ptrs_vector->flags & 1)))
                {
                    dx1=-Vtxt.wysokosc*h_factor*koc1;
                    dy1=-Vtxt.wysokosc*h_factor*kos1;

                    dx2=-Vltxt.wysokosc*h_factor*koc1;
                    dy2=-Vltxt.wysokosc*h_factor*kos1;
                }
            }
            else
            {

                if (((ptrs_vector->magnitude1<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude1>0) && (ptrs_vector->flags & 1)))
                {
                    dx = -Vtxt.wysokosc *h_factor* koc1;
                    dy = -Vtxt.wysokosc *h_factor* kos1;
                }

                if (((ptrs_vector->magnitude2<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude2>0) && (ptrs_vector->flags & 1)))
                {
                    dx1 = -Vtxt.wysokosc *h_factor* koc1;
                    dy1 = -Vtxt.wysokosc *h_factor* kos1;

                    dx2 = -Vltxt.wysokosc *h_factor* koc1;
                    dy2 = -Vltxt.wysokosc *h_factor* kos1;
                }
            }

            Vtxt.x=Lt.x1+(float)dx;
            Vtxt.y=Lt.y1+(float)dy;
            Vtxt.kat=(float)(PL1.kat*Pi/180);
            Vtxt.justowanie=j_do_lewej;
            Vtxt1.x=Lt.x2+(float)dx1;
            Vtxt1.y=Lt.y2+(float)dy1;
            Vtxt1.kat=Vltxt.kat=(PL1.kat*Pi/180);
            Vtxt1.justowanie=j_do_prawej;

            Vltxt.x=(float)(Ltx+dx2);
            Vltxt.y=(float)(Lty+dy2);
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
        case 15:  //thermal

            kos1=sin(Pi*(PL.kat+90)/180);
            koc1=cos(Pi*(PL.kat+90)/180);

            n=view_vector_scale;

            Lth.x1 = (float)((L1.x1+L1.x2)/2. + n*(ptrs_vector->r/depth_magnitude)*koc1);   //thermal_depth_size
            Lth.y1 = (float)((L1.y1+L1.y2)/2. + n*(ptrs_vector->r/depth_magnitude)*kos1);
            Lth.x2 = (float)((L1.x1+L1.x2)/2. - n*(ptrs_vector->r/depth_magnitude)*koc1);
            Lth.y2 = (float)((L1.y1+L1.y2)/2. - n*(ptrs_vector->r/depth_magnitude)*kos1);

            parametry_lini(&Lth, &PLth);
            kos1th=sin(Pi*(PLth.kat+90)/180);
            koc1th=cos(Pi*(PLth.kat+90)/180);

            Lt.x1 = Lth.x1 + (float)((ptrs_vector->magnitude1/thermal_magnitude)*koc1th);
            Lt.y1 = Lth.y1 + (float)((ptrs_vector->magnitude1/thermal_magnitude)*kos1th);
            Lt.x2 = Lth.x2 + (float)((ptrs_vector->magnitude2/thermal_magnitude)*koc1th);
            Lt.y2 = Lth.y2 + (float)((ptrs_vector->magnitude2/thermal_magnitude)*kos1th);

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos2=sin(Pi*(PL1.kat+90)/180);
            koc2=cos(Pi*(PL1.kat+90)/180);

            dx = 0;
            dy = 0;
            dx1 = 0;
            dy1 = 0;
            dx2=0;
            dy2=0;

            if (ptrs_vector->magnitude1<0)
            {
                dx = -Vtxt.wysokosc * koc2;
                dy = -Vtxt.wysokosc * kos2;
            }

            if (ptrs_vector->magnitude2<0)
            {
                dx1 = -Vtxt.wysokosc * koc2;
                dy1 = -Vtxt.wysokosc * kos2;

                dx2 = -Vltxt.wysokosc * koc2;
                dy2 = -Vltxt.wysokosc * kos2;
            }

            Vtxt.x=Lt.x1+(float)dx;
            Vtxt.y=Lt.y1+(float)dy;
            Vtxt.kat=(float)(PL1.kat*Pi/180);
            Vtxt.justowanie=j_do_lewej;
            Vtxt1.x=Lt.x2+(float)dx1;
            Vtxt1.y=Lt.y2+(float)dy1;
            Vtxt1.kat=Vltxt.kat=(float)(PL1.kat*Pi/180);
            Vtxt1.justowanie=j_do_prawej;

            Vltxt.x=(float)(Ltx+dx2);
            Vltxt.y=(float)(Lty+dy2);
            Vltxt.justowanie=j_srodkowo;

            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, thermal_precision);
            set_decimal_format(Vtxt1.text, ptrs_vector->magnitude2, thermal_precision);

            normalize_txt(&Vtxt);
            normalize_txt(&Vtxt1);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);

            break;
        case 16:  //node radius

            n=view_vector_scale;

            Ln.x1 = L1.x1 + (float)(n*(ptrs_vector->r/radius_magnitude)*koc);   //thermal_depth_size
            Ln.y1 = L1.y1 + (float)(n*(ptrs_vector->r/radius_magnitude)*kos);
            Ln.x2 = L1.x2;
            Ln.y2 = L1.y2;

            Vtxt.x=L1.x2;
            Vtxt.y=L1.y2;
            Vtxt.kat=0;
            Vtxt.wysokosc=(float)(zmwym.wysokosc*view_vector_scale*0.75);
            Vtxt.justowanie=j_do_lewej;

            set_decimal_format(Vtxt1.text, ptrs_vector->magnitude1/view_vector_scale, dim_precision);
            sprintf(Vtxt.text,"R%s",Vtxt1.text);
            //normalize_txt(&Vtxt);
            break;
        default:
            break;
    }

    Vtxt.dl = strlen(Vtxt.text);
    Vtxt1.dl = strlen(Vtxt1.text);
    Vtxt.n = T18 + Vtxt.dl;
    Vtxt1.n = T18 + Vtxt1.dl;

    L2.x1 = L1.x1 + (float)(ra * koc);
    L2.y1 = L1.y1 + (float)(ra * kos);
    L2.x2 = L1.x2 - (float)(ra * koc);
    L2.y2 = L1.y2 - (float)(ra * kos);

    L3.x1 = L1.x1 + (float)(df_psize * koc);
    L3.y1 = L1.y1 + (float)(df_psize * kos);
    L3.x2 = L1.x2 - (float)(df_psize * koc);
    L3.y2 = L1.y2 - (float)(df_psize * kos);

    L4.x1 = L1.x1 + (float)(df_psize1 * koc);
    L4.y1 = L1.y1 + (float)(df_psize1 * kos);
    L4.x2 = L1.x2 - (float)(df_psize1 * koc);
    L4.y2 = L1.y2 - (float)(df_psize1 * kos);

    switch(ptrs_vector->style) {
        case 0:

            rysuj_obiekt_(&L4, mode, kolor);
            if (kolor) set_mode_trans();
            K.x = ptrs_vector->x1;
            K.y = ptrs_vector->y1;
            K.r = (float)df_psize1;

            if (okrag_visible(&K))
                rysuj_obiekt_(&K, mode, kolor);
            K.x = ptrs_vector->x2;
            K.y = ptrs_vector->y2;
            if (okrag_visible(&K))
                rysuj_obiekt_(&K, mode, kolor);

            if (kolor) set_mode_solid();

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw = TRUE;
            } else outtextxy_w(&Vtxt, mode);

            if ((ptrs_vector->cartflags) && (ptrs_vector->foundflags==1))
            {
                w.xy[0]=L4.x1;
                w.xy[1]=L4.y1;
                w.xy[2]=L4.x2;
                w.xy[3]=L4.y2;
                w.xy[6]=L4.x2;
                w.xy[7]=L4.y2;
                w.xy[4]=L4.x1;
                w.xy[5]=L4.x1;
                if (!(ptrs_vector->cartflags & 1))
                {
                    arrow_angle = M_PI * (PL.kat + 90.0) / 180.0;
                    kos1 = sin(arrow_angle);
                    koc1 = cos(arrow_angle);
                }
                else {
                    double iso_angle = cartesian_angle_to_isometric_angle(M_PI * PL.kat / 180.);
                    // Add 90° in isometric space (counter-clockwise perpendicular)
                    double iso_perp = fmod(iso_angle + M_PI / 2.0, 2.0 * M_PI);
                    if (iso_perp < 0.0) iso_perp += 2.0 * M_PI;
                    arrow_angle = isometric_angle_to_cartesian_angle_rad(iso_perp);
                    kos1 = sin(arrow_angle);
                    koc1 = cos(arrow_angle);
                }
                if (!make_arrows(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], arrow_angle, ptrs_vector, PL.kat, mode, kolor, vkolor, redraw_obj))
                    return;
            }
            break;
        case 1:

            L4.x2 = L3.x2;
            L4.y2 = L3.y2;
            rysuj_obiekt_(&L4, mode, kolor);

            if (kolor) set_mode_trans();
            K.x = ptrs_vector->x1;
            K.y = ptrs_vector->y1;
            K.r = (float)df_psize1;
            if (okrag_visible(&K))
                rysuj_obiekt_(&K, mode, kolor);
            if (kolor) set_mode_solid();
            O.x = ptrs_vector->x2;
            O.y = ptrs_vector->y2;
            O.r = (float)df_psize;
            O.typ = 64;
            if (okrag_visible(&O))
                rysuj_obiekt_(&O, mode, kolor);
            out_v_krz(pikseleX0(ptrs_vector->x2), pikseleY0(ptrs_vector->y2));

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw = TRUE;
            } else outtextxy_w(&Vtxt, mode);

            break;
        case 2:

            L4.x1 = L3.x1;
            L4.y1 = L3.y1;
            rysuj_obiekt_(&L4, mode, kolor);

            if (kolor) set_mode_trans();
            K.x = ptrs_vector->x2;
            K.y = ptrs_vector->y2;
            K.r = (float)df_psize1;
            if (okrag_visible(&K))
                rysuj_obiekt_(&K, mode, kolor);
            if (kolor) set_mode_solid();
            O.x = ptrs_vector->x1;
            O.y = ptrs_vector->y1;
            O.r = (float)df_psize;
            O.typ = 64;
            if (okrag_visible(&O))
                rysuj_obiekt_(&O, mode, kolor);
            out_v_krz(pikseleX0(ptrs_vector->x1), pikseleY0(ptrs_vector->y1));

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw = TRUE;
            } else outtextxy_w(&Vtxt, mode);

            break;
        case 3:
            rysuj_obiekt_(&L3, mode, kolor);
            O.x = ptrs_vector->x1;
            O.y = ptrs_vector->y1;
            O.r = (float)df_psize;
            O.typ = 64;
            if (okrag_visible(&O))
                rysuj_obiekt_(&O, mode, kolor);
            O.x = ptrs_vector->x2;
            O.y = ptrs_vector->y2;
            if (okrag_visible(&O))
                rysuj_obiekt_(&O, mode, kolor);
            out_v_krz(pikseleX0(ptrs_vector->x1), pikseleY0(ptrs_vector->y1));
            out_v_krz(pikseleX0(ptrs_vector->x2), pikseleY0(ptrs_vector->y2));

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw = TRUE;
            } else outtextxy_w(&Vtxt, mode);

            break;
        case 4:  //force
        case 18: //slab force
        case 19: //force z
            if (ptrs_vector->style == 18)  //slab force
            {
                load_ellipse.kolor = ptrs_vector->kolor;
                load_ellipse.warstwa = ptrs_vector->warstwa;
                load_ellipse.widoczny = ptrs_vector->widoczny;
                load_ellipse.translucency = 51;
                load_ellipse.typ = 64;
                load_ellipse.x = ptrs_vector->x1;
                load_ellipse.y = ptrs_vector->y1;
                load_ellipse.angle = 0;
                load_ellipse.rx = (float) (df_psize * 3.);
                load_ellipse.ry = (float) (load_ellipse.rx / 3.);
                rysuj_obiekt_((void *) &load_ellipse, mode, kolor);
            }

            w.atrybut = ptrs_vector->atrybut;
            w.warstwa = ptrs_vector->warstwa;
            w.kolor = vkolor; //ptrs_vector->kolor;


            if (PL.dl > ra)
            {
                Ls.x2 = L1.x2;
                Ls.y2 = L1.y2;
                Ls.x1 = L2.x1;
                Ls.y1 = L2.y1;

                if ((!(ptrs_vector->cartflags & 1)) || (ptrs_vector->style != 4))
                    create_solid_on_line(&Ls, &w, ra / 4, ra / 4, 0);
                else {
                    LINIA Lsi;
                    shorten_line_isometric(&L1, &Lsi, ra, 1);
                    create_solid_on_line_isometric(&Lsi, &w, ra / 4., ra / 4., 0);
                }

                rysuj_obiekt_(&w, mode, kolor);
            }

            Kp2s = Get_Point_Size() * view_vector_scale / (arrowf * cos(Pi * katS / 180));

            if ((!(ptrs_vector->cartflags & 1)) || (ptrs_vector->style != 4)) {
                koc1 = cos(Pi * (kat1 - katS) / 180);
                koc2 = cos(Pi * (kat1 + katS) / 180);
                kos1 = sin(Pi * (kat1 - katS) / 180);
                kos2 = sin(Pi * (kat1 + katS) / 180);

                w.xy[2] = ptrs_vector->x1;
                w.xy[3] = ptrs_vector->y1;
                w.xy[0] = ptrs_vector->x1 + (float)(Kp2s * koc1);
                w.xy[1] = ptrs_vector->y1 + (float)(Kp2s * kos1);
                w.xy[4] = ptrs_vector->x1 + (float)(Kp2s * koc2);
                w.xy[5] = ptrs_vector->y1 + (float)(Kp2s * kos2);
            } else {
                // Isometric mode
                double angle_rad = kat1 * M_PI / 180.;
                double iso_angle = cartesian_angle_to_isometric_angle(angle_rad);

                //backward is not necessary
                //double iso_backward = fmod(iso_angle /*+ M_PI*/, 2.0 * M_PI);
                //if (iso_backward < 0.0) iso_backward += 2.0 * M_PI;

                double katS_rad = 25.0 * M_PI / 180.0;  // or your variable

                // SWAPPED ORDER TO MATCH MANUAL
                //double iso_angle1 = fmod(iso_backward + katS_rad, 2.0 * M_PI);
                //double iso_angle2 = fmod(iso_backward - katS_rad, 2.0 * M_PI);
                double iso_angle1 = fmod(iso_angle + katS_rad, 2.0 * M_PI);
                double iso_angle2 = fmod(iso_angle - katS_rad, 2.0 * M_PI);

                double iso_angle1_deg = iso_angle1 * 180.0 / M_PI;
                double iso_angle2_deg = iso_angle2 * 180.0 / M_PI;

                double x_tip1, y_tip1, x_tip2, y_tip2;
                isometric_polar_to_cartesian(ptrs_vector->x1, ptrs_vector->y1, Kp2s, iso_angle1_deg, &x_tip1, &y_tip1);
                isometric_polar_to_cartesian(ptrs_vector->x1, ptrs_vector->y1, Kp2s, iso_angle2_deg, &x_tip2, &y_tip2);

                w.xy[0] = (float)x_tip1;
                w.xy[1] = (float)y_tip1;
                w.xy[2] = ptrs_vector->x1;
                w.xy[3] = ptrs_vector->y1;
                w.xy[4] = (float)x_tip2;
                w.xy[5] = (float)y_tip2;
            }

            w.lp = 6;
            w.n = 32;

            if (wielokat_visible(&w))
                rysuj_obiekt_(&w, mode, kolor);

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                outtextxy_w_0(&Vltxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw = TRUE;
            } else {
                outtextxy_w(&Vtxt, mode);
                outtextxy_w(&Vltxt, mode);
            }
            break;
        case 7:  //displacement
        case 27:  //displacement z
            grubosc = (ptrs_vector->typ & 224) / 32;

            if ((!(ptrs_vector->cartflags & 1)) || (ptrs_vector->style != 7))
            {
                L3.x2 = L2.x2;
                L3.y2 = L2.y2;
                L3.typ = grubosc * 32 + 1;
                rysuj_obiekt_(&L3, mode, kolor);

                O.x = ptrs_vector->x1;
                O.y = ptrs_vector->y1;
                O.r = (float)df_psize;
                O.typ = 64;
                if (okrag_visible(&O))
                    rysuj_obiekt_(&O, mode, kolor);
            }
            else
            {
                LINIA Lsi;
                shorten_line_isometric(&L1, &Lsi, ra, 3);
                L3.x2 = Lsi.x2;
                L3.y2 = Lsi.y2;
                L3.typ = grubosc * 32 + 1;
                rysuj_obiekt_(&L3, mode, kolor);
                O.x = ptrs_vector->x1;
                O.y = ptrs_vector->y1;
                O.r = (float)df_psize;
                E.typ = 64;
                ret=circle_to_isometric_ellipse_o_e(XY_PLANE, &O, &E);
                if (elipsa_wybrana_prec(&E))
                    rysuj_obiekt_(&E, mode, kolor);
            }

            Kp2s = Get_Point_Size () *view_vector_scale / (arrowf*cos(Pi*katS/180)) ;

            if ((!(ptrs_vector->cartflags & 1)) || (ptrs_vector->style!=7))
            {
                koc1 = cos(Pi * (kat1 - katS) / 180);
                koc2 = cos(Pi * (kat1 + katS) / 180);
                kos1 = sin(Pi * (kat1 - katS) / 180);
                kos2 = sin(Pi * (kat1 + katS) / 180);

                w.xy[2] = L1.x2;
                w.xy[3] = L1.y2;
                w.xy[0] = L1.x2 - (float)(Kp2s * koc1);
                w.xy[1] = L1.y2 - (float)(Kp2s * kos1);
                w.xy[4] = L1.x2 - (float)(Kp2s * koc2);
                w.xy[5] = L1.y2 - (float)(Kp2s * kos2);
            }
                else
                {
                    // Isometric mode
                    double angle_rad = kat1*M_PI/180. + M_PI;
                    double iso_angle = cartesian_angle_to_isometric_angle(angle_rad);

                    //backward is not necessary
                    //double iso_backward = fmod(iso_angle /*+ M_PI*/, 2.0 * M_PI);
                    //if (iso_backward < 0.0) iso_backward += 2.0 * M_PI;

                    double katS_rad = katS * M_PI / 180.0;  // or your variable

                    // SWAPPED ORDER TO MATCH MANUAL
                    //double iso_angle1 = fmod(iso_backward + katS_rad, 2.0 * M_PI);
                    //double iso_angle2 = fmod(iso_backward - katS_rad, 2.0 * M_PI);
                    double iso_angle1 = fmod(iso_angle + katS_rad, 2.0 * M_PI);
                    double iso_angle2 = fmod(iso_angle - katS_rad, 2.0 * M_PI);

                    double iso_angle1_deg = iso_angle1 * 180.0 / M_PI;
                    double iso_angle2_deg = iso_angle2 * 180.0 / M_PI;

                    double x_tip1, y_tip1, x_tip2, y_tip2;
                    isometric_polar_to_cartesian(ptrs_vector->x2, ptrs_vector->y2, Kp2s, iso_angle1_deg, &x_tip1, &y_tip1);
                    isometric_polar_to_cartesian(ptrs_vector->x2, ptrs_vector->y2, Kp2s, iso_angle2_deg, &x_tip2, &y_tip2);

                    w.xy[0] = (float)x_tip1;
                    w.xy[1] = (float)y_tip1;
                    w.xy[2] = ptrs_vector->x2;
                    w.xy[3] = ptrs_vector->y2;
                    w.xy[4] = (float)x_tip2;
                    w.xy[5] = (float)y_tip2;
                }

            w.lp=6;
            w.n=32;
            w.empty_typ=3;

            if (wielokat_visible(&w))
                rysuj_obiekt_(&w,mode,kolor);

            out_v_krz (pikseleX0(ptrs_vector->x1), pikseleY0(ptrs_vector->y1));

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                outtextxy_w_0(&Vltxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw=TRUE;
            }
            else
            {
                outtextxy_w(&Vtxt,mode);
                outtextxy_w(&Vltxt,mode);
            }
            break;
        case 5:  //moment
        case 6:
        case 8:  //rotation
        case 9:
        case 21:  //moment x  //TO DO ISOMETRIC
        case 22:
        case 23:  //moment y
        case 24:
        case 25:  //moment xy
        case 26:
        case 28:  //rotation x
        case 29:
        case 30:  //rotation y
        case 31:
        case 32:  //rotation xy
        case 33:
        case V_EDGE_ARC_SIMPLE:  //simple supported edge
        case V_EDGE_ARC_SIMPLE_INV:  //simple supported edge flipped
        case V_EDGE_ARC_FIXED:  //fixed edge
        case V_EDGE_ARC_FIXED_INV:  //fixed edge flipped
        case V_EDGE_ARC_ROLL:  //roll edge
        case V_EDGE_ARC_ROLL_INV:  //roll edge flipped
            switch (ptrs_vector->style)
            {
                int plane;
                case 5:
                    if (Check_if_Equal((double)ptrs_vector->angle1, (double)ptrs_vector->angle2)) break;

                    sa.kat2=ptrs_vector->angle2;
                    linestyle(ptrs_vector->typ);
                    kats=ptrs_vector->angle2-Pi_/2;
                    xs=ptrs_vector->x1+ptrs_vector->r*cosf(ptrs_vector->angle2);
                    ys=ptrs_vector->y1+ptrs_vector->r*sinf(ptrs_vector->angle2);
                    n=+1;
                    s=0;

                    sa.atrybut=ptrs_vector->atrybut;
                    sa.warstwa=ptrs_vector->warstwa;
                    sa.kolor=vkolor; //ptrs_vector->kolor;
                    sa.x=ptrs_vector->x1;
                    sa.y=ptrs_vector->y1;
                    sa.r=ptrs_vector->r;
                    sa.kat1=ptrs_vector->angle1;
                    sa.kat2=ptrs_vector->angle2;
                    sa.width1=(float)ra/4;
                    sa.width2=(float)ra/4;
                    rysuj_solidarc_(&sa, mode, kolor, FALSE, TRUE,1,1);
                break;
                case 6:
                    if (Check_if_Equal((double)ptrs_vector->angle1, (double)ptrs_vector->angle2)) break;

                    linestyle(ptrs_vector->typ);
                    kats=ptrs_vector->angle1+Pi_/2;
                    xs=ptrs_vector->x1+ptrs_vector->r*cosf(ptrs_vector->angle1);
                    ys=ptrs_vector->y1+ptrs_vector->r*sinf(ptrs_vector->angle1);
                    n=-1;
                    s=0;

                    sa.atrybut=ptrs_vector->atrybut;
                    sa.warstwa=ptrs_vector->warstwa;
                    sa.kolor=vkolor; //ptrs_vector->kolor;
                    sa.x=ptrs_vector->x1;
                    sa.y=ptrs_vector->y1;
                    sa.r=ptrs_vector->r;
                    sa.kat1=ptrs_vector->angle1;
                    sa.kat2=ptrs_vector->angle2;
                    sa.width1=(float)ra/4;
                    sa.width2=(float)ra/4;
                    rysuj_solidarc_(&sa, mode, kolor, FALSE, TRUE, 1,1);
                    break;
                case 21:  // moment x //TO DO ISOMETRIC
                case 23:
                case 25:
                    switch (ptrs_vector->style) {
                        case 21:  // moment x //TO DO ISOMETRIC
                            plane = XZ_PLANE;
                            break;
                        case 23:
                            plane = YZ_PLANE;
                            break;
                        case 25:
                            plane = XY_PLANE;
                            break;
                    }
                    if (Check_if_Equal((double)ptrs_vector->angle1, (double)ptrs_vector->angle2))
                    {
                        ea_start_x=ea_end_x=ptrs_vector->x2;
                        ea_start_y=ea_end_y=ptrs_vector->y2;
                        ea.rx=ea.ry=sqrtf((ptrs_vector->x2 - ptrs_vector->x1)*(ptrs_vector->x2 - ptrs_vector->x1) + (ptrs_vector->y2-ptrs_vector->y1)*(ptrs_vector->y2-ptrs_vector->y1));
                        ea.angle=0.0f;
                        ea.kat1=ea.kat2=atan2f(ptrs_vector->y2-ptrs_vector->y1, ptrs_vector->x2 - ptrs_vector->x1);
                        break;
                    }

                    linestyle(ptrs_vector->typ);
                    kats=ptrs_vector->angle2-Pi_/2;
                    xs=ptrs_vector->x1+ptrs_vector->r*cosf(ptrs_vector->angle2);
                    ys=ptrs_vector->y1+ptrs_vector->r*sinf(ptrs_vector->angle2);
                    n=+1;
                    s=0;

                    ////instead of solid arc we use arc converted to elliptical arc in isometric
                    l.x=ptrs_vector->x1;
                    l.y=ptrs_vector->y1;
                    l.r=ptrs_vector->r;
                    l.kat1=ptrs_vector->angle1;
                    l.kat2=ptrs_vector->angle2;
                    ea.typ=128;
                    ret=arc_to_isometric_ellipticalarc_a_ea(plane, &l, &ea);
                    rysuj_obiekt_(&ea, mode, kolor);
                    Get_EllipticalArc_EndPoints (ea.x, ea.y, ea.rx, ea.ry, ea.angle, ea.kat1, ea.kat2, &ea_start_x, &ea_start_y, &ea_end_x, &ea_end_y);

                    break;
                case 22:
                case 24:
                case 26:
                    switch (ptrs_vector->style) {
                        case 22:  // moment x //TO DO ISOMETRIC
                            plane = XZ_PLANE;
                            break;
                        case 24:
                            plane = YZ_PLANE;
                            break;
                        case 26:
                            plane = XY_PLANE;
                            break;
                    }
                    if (Check_if_Equal((double)ptrs_vector->angle1, (double)ptrs_vector->angle2))
                    {
                        ea_start_x=ea_end_x=ptrs_vector->x2;
                        ea_start_y=ea_end_y=ptrs_vector->y2;
                        ea.rx=ea.ry=sqrtf((ptrs_vector->x2 - ptrs_vector->x1)*(ptrs_vector->x2 - ptrs_vector->x1) + (ptrs_vector->y2-ptrs_vector->y1)*(ptrs_vector->y2-ptrs_vector->y1));
                        ea.angle=0.0f;
                        ea.kat1=ea.kat2=atan2f(ptrs_vector->y2-ptrs_vector->y1, ptrs_vector->x2 - ptrs_vector->x1);
                        break;
                    }

                    linestyle(ptrs_vector->typ);
                    kats=ptrs_vector->angle1+Pi_/2;
                    xs=ptrs_vector->x1+ptrs_vector->r*cosf(ptrs_vector->angle1);
                    ys=ptrs_vector->y1+ptrs_vector->r*sinf(ptrs_vector->angle1);
                    n=-1;
                    s=0;

                    /*
                    sa.atrybut=ptrs_vector->atrybut;
                    sa.warstwa=ptrs_vector->warstwa;
                    sa.kolor=vkolor; //ptrs_vector->kolor;
                    sa.x=ptrs_vector->x1;
                    sa.y=ptrs_vector->y1;
                    sa.r=ptrs_vector->r;
                    sa.kat1=ptrs_vector->angle1;
                    sa.kat2=ptrs_vector->angle2;
                    sa.width1=ra/4;
                    sa.width2=ra/4;
                    rysuj_solidarc_(&sa, mode, kolor, FALSE, TRUE, 1,1);
                     */

                    ////instead of solid arc we use arc converted to elliptical arc in isometric
                    l.x=ptrs_vector->x1;
                    l.y=ptrs_vector->y1;
                    l.r=ptrs_vector->r;
                    l.kat1=ptrs_vector->angle1;
                    l.kat2=ptrs_vector->angle2;
                    ea.typ=128;
                    ret=arc_to_isometric_ellipticalarc_a_ea(plane, &l, &ea);
                    rysuj_obiekt_(&ea, mode, kolor);
                    Get_EllipticalArc_EndPoints (ea.x, ea.y, ea.rx, ea.ry, ea.angle, ea.kat1, ea.kat2, &ea_start_x, &ea_start_y, &ea_end_x, &ea_end_y);

                    break;
                case 8:  //displacement
                    if (Check_if_Equal((double)ptrs_vector->angle1, (double)ptrs_vector->angle2)) break;

                    grubosc = (ptrs_vector->typ & 224) / 32;
                    linestyle(grubosc * 32 + 1);
                    kats=ptrs_vector->angle2-Pi_/2;
                    n=+1;
                    s=1;
                    xs=ptrs_vector->x1+ptrs_vector->r*cosf(ptrs_vector->angle2);
                    ys=ptrs_vector->y1+ptrs_vector->r*sinf(ptrs_vector->angle2);

                    l.x=ptrs_vector->x1;
                    l.y=ptrs_vector->y1;
                    l.r=ptrs_vector->r;
                    l.kat1=ptrs_vector->angle1;
                    l.kat2=ptrs_vector->angle2;
                    l.typ=grubosc * 32 + 1;
                    rysuj_obiekt_(&l, mode, kolor);
                    break;
                case 9: //-displacement
                    if (Check_if_Equal((double)ptrs_vector->angle1, (double)ptrs_vector->angle2)) break;

                    grubosc = (ptrs_vector->typ & 224) / 32;
                    linestyle(grubosc * 32 + 1);
                    kats=ptrs_vector->angle1+Pi_/2;
                    n=-1;
                    s=1;
                    xs=ptrs_vector->x1+ptrs_vector->r*cosf(ptrs_vector->angle1);
                    ys=ptrs_vector->y1+ptrs_vector->r*sinf(ptrs_vector->angle1);

                    l.x=ptrs_vector->x1;
                    l.y=ptrs_vector->y1;
                    l.r=ptrs_vector->r;
                    l.kat1=ptrs_vector->angle1;
                    l.kat2=ptrs_vector->angle2;
                    l.typ=grubosc * 32 + 1;
                    rysuj_obiekt_(&l, mode, kolor);
                    break;
                case 28:  //TO DO ISOMETRIC
                case 30:
                case 32:
                    switch (ptrs_vector->style) {
                        case 28:  // moment x //TO DO ISOMETRIC
                            plane = XZ_PLANE;
                            break;
                        case 30:
                            plane = YZ_PLANE;
                            break;
                        case 32:
                            plane = XY_PLANE;
                            break;
                    }
                    if (Check_if_Equal((double)ptrs_vector->angle1, (double)ptrs_vector->angle2))
                    {
                        ea_start_x=ea_end_x=ptrs_vector->x2;
                        ea_start_y=ea_end_y=ptrs_vector->y2;
                        ea.rx=ea.ry=sqrtf((ptrs_vector->x2 - ptrs_vector->x1)*(ptrs_vector->x2 - ptrs_vector->x1) + (ptrs_vector->y2-ptrs_vector->y1)*(ptrs_vector->y2-ptrs_vector->y1));
                        ea.angle=0.0f;
                        ea.kat1=ea.kat2=atan2f(ptrs_vector->y2-ptrs_vector->y1, ptrs_vector->x2 - ptrs_vector->x1);
                        break;
                    }

                    grubosc = (ptrs_vector->typ & 224) / 32;
                    linestyle(grubosc * 32 + 1);
                    kats=ptrs_vector->angle2-Pi_/2;
                    xs=ptrs_vector->x1+ptrs_vector->r*cosf(ptrs_vector->angle2);
                    ys=ptrs_vector->y1+ptrs_vector->r*sinf(ptrs_vector->angle2);
                    n=+1;
                    s=1;

                    l.x=ptrs_vector->x1;
                    l.y=ptrs_vector->y1;
                    l.r=ptrs_vector->r;
                    l.kat1=ptrs_vector->angle1;
                    l.kat2=ptrs_vector->angle2;
                    //l.typ=grubosc * 32 + 1;
                    //rysuj_obiekt_(&l, mode, kolor);
                    ea.typ=grubosc * 32 + 1;
                    ret=arc_to_isometric_ellipticalarc_a_ea(plane, &l, &ea);
                    rysuj_obiekt_(&ea, mode, kolor);
                    Get_EllipticalArc_EndPoints (ea.x, ea.y, ea.rx, ea.ry, ea.angle, ea.kat1, ea.kat2, &ea_start_x, &ea_start_y, &ea_end_x, &ea_end_y);

                    break;
                case 29:
                case 31:
                case 33:
                    switch (ptrs_vector->style) {
                        case 29:  // moment x //TO DO ISOMETRIC
                            plane = XZ_PLANE;
                            break;
                        case 31:
                            plane = YZ_PLANE;
                            break;
                        case 33:
                            plane = XY_PLANE;
                            break;
                    }
                    if (Check_if_Equal((double)ptrs_vector->angle1, (double)ptrs_vector->angle2))
                    {
                        ea_start_x=ea_end_x=ptrs_vector->x2;
                        ea_start_y=ea_end_y=ptrs_vector->y2;
                        ea.rx=ea.ry=sqrtf((ptrs_vector->x2 - ptrs_vector->x1)*(ptrs_vector->x2 - ptrs_vector->x1) + (ptrs_vector->y2-ptrs_vector->y1)*(ptrs_vector->y2-ptrs_vector->y1));
                        ea.angle=0.0f;
                        ea.kat1=ea.kat2=atan2f(ptrs_vector->y2-ptrs_vector->y1, ptrs_vector->x2 - ptrs_vector->x1);
                        break;
                    }

                    grubosc = (ptrs_vector->typ & 224) / 32;
                    linestyle(grubosc * 32 + 1);
                    kats=ptrs_vector->angle1+Pi_/2;
                    xs=ptrs_vector->x1+ptrs_vector->r*cosf(ptrs_vector->angle1);
                    ys=ptrs_vector->y1+ptrs_vector->r*sinf(ptrs_vector->angle1);
                    n=-1;
                    s=1;

                    l.x=ptrs_vector->x1;
                    l.y=ptrs_vector->y1;
                    l.r=ptrs_vector->r;
                    l.kat1=ptrs_vector->angle1;
                    l.kat2=ptrs_vector->angle2;
                    //l.typ=grubosc * 32 + 1;
                    //rysuj_obiekt_(&l, mode, kolor);
                    ea.typ=grubosc * 32 + 1;
                    ret=arc_to_isometric_ellipticalarc_a_ea(plane, &l, &ea);
                    rysuj_obiekt_(&ea, mode, kolor);
                    Get_EllipticalArc_EndPoints (ea.x, ea.y, ea.rx, ea.ry, ea.angle, ea.kat1, ea.kat2, &ea_start_x, &ea_start_y, &ea_end_x, &ea_end_y);
                    break;
                case V_EDGE_ARC_SIMPLE:  //simple supported edge
                case V_EDGE_ARC_SIMPLE_INV:  //simple supported edge flipped
                case V_EDGE_ARC_FIXED:  //fixed edge
                case V_EDGE_ARC_FIXED_INV:  //fixed edge flipped
                case V_EDGE_ARC_ROLL:  //roll edge
                case V_EDGE_ARC_ROLL_INV:  //roll edge flipped
                    grubosc = (ptrs_vector->typ & 224) / 32;
                    linestyle(grubosc * 32 + 1);
                    kats=ptrs_vector->angle2-Pi_/2;
                    n=+1;
                    s=1;
                    xs=ptrs_vector->x1+ptrs_vector->r*cosf(ptrs_vector->angle2);
                    ys=ptrs_vector->y1+ptrs_vector->r*sinf(ptrs_vector->angle2);

                    l.x=ptrs_vector->x1;
                    l.y=ptrs_vector->y1;
                    l.r=ptrs_vector->r;
                    l.kat1=ptrs_vector->angle1;
                    l.kat2=ptrs_vector->angle2;
                    l.typ=ptrs_vector->typ;
                    rysuj_obiekt_(&l, mode, kolor);

                    //triangles or dashes
                    if (!make_arcarrows(&l, ptrs_vector, PL.kat, mode, kolor, vkolor, redraw_obj))
                        return;

                    return;
                    break;
            }

            dx=Get_Point_Size () *view_vector_scale / arrowf * cos(kats);
            dy=Get_Point_Size () *view_vector_scale / arrowf * sin(kats);

            xs-=dx;
            ys-=dy;

            Kp2s = Get_Point_Size () *view_vector_scale / (arrowf*cos(Pi*katS/180.)) ;

            Kps = Get_Point_Size () *view_vector_scale;

            if ((!(ptrs_vector->cartflags & 1)) || (ptrs_vector->style<20))
            {
                koc1 = cos(kats - Pi * (katS) / 180.);
                koc2 = cos(kats + Pi * (katS) / 180.);
                kos1 = sin(kats - Pi * (katS) / 180.);
                kos2 = sin(kats + Pi * (katS) / 180.);

                w.xy[2] = (float)xs;
                w.xy[3] = (float)ys;
                w.xy[0] = (float)(xs + Kp2s * koc1);
                w.xy[1] = (float)(ys + Kp2s * kos1);
                w.xy[4] = (float)(xs + Kp2s * koc2);
                w.xy[5] = (float)(ys + Kp2s * kos2);
            }
            else
            {
                int plane;
                switch (ptrs_vector->style)
                {
                    case 21:
                        plane=XZ_PLANE;
                        break;
                    case 22:
                        plane=XZ_PLANE;
                        break;
                    case 23:
                        plane=YZ_PLANE;
                        break;
                    case 24:
                        plane=YZ_PLANE;
                        break;
                    case 25:
                        plane=XY_PLANE;
                        break;
                    case 26:
                        plane=XY_PLANE;
                        break;
                    case 28:
                        plane=XZ_PLANE;
                        break;
                    case 29:
                        plane=XZ_PLANE;
                        break;
                    case 30:
                        plane=YZ_PLANE;
                        break;
                    case 31:
                        plane=YZ_PLANE;
                        break;
                    case 32:
                        plane=XY_PLANE;
                        break;
                    case 33:
                        plane=XY_PLANE;
                        break;
                    default:
                        plane=XY_PLANE;
                        break;
                }

                // Isometric mode for ellipses tip
                //double angle_rad=ea.angle + ea.kat2;
                //create_arrowhead_ellipticalarc(&ea, plane, ea_end_x, ea_end_y, angle_rad, Kps /*Kp2s*/, katS, &w);

                /*
                  //working a OK
                double angle_rad, angle_rad1;
                double iso_angle;

                if (plane!=XY_PLANE) {
                    //angle_rad = ellipse_tangent_angle(ea.x, ea.y, ea.rx, ea.ry, ea.angle, ea.kat2);
                    angle_rad = get_elliptical_tangent_angle(ea.angle, ea.rx, ea.ry, ea.kat2);
                    angle_rad = fmod(angle_rad - M_PI, 2.0 * M_PI);
                    if (angle_rad < 0.0) angle_rad += 2.0 * M_PI;
                    iso_angle = cartesian_angle_to_isometric_angle(angle_rad);
                }
                else {
                    angle_rad = ea.angle + ea.kat2;
                    iso_angle = cartesian_angle_to_isometric_angle(angle_rad);
                    iso_angle -= M_PI / 2.0;
                }

                double katS_rad = katS * M_PI / 180.0;  // katS is usually 25.0

                double iso_angle1 = fmod(iso_angle + katS_rad, 2.0 * M_PI);
                double iso_angle2 = fmod(iso_angle - katS_rad, 2.0 * M_PI);

                double iso_angle1_deg = iso_angle1 * 180.0 / M_PI;
                double iso_angle2_deg = iso_angle2 * 180.0 / M_PI;

                double x_tip1, y_tip1, x_tip2, y_tip2;
                isometric_polar_to_cartesian(ea_end_x, ea_end_y, Kps, iso_angle1_deg, &x_tip1, &y_tip1);
                isometric_polar_to_cartesian(ea_end_x, ea_end_y, Kps, iso_angle2_deg, &x_tip2, &y_tip2);

                double dxi=ea_end_x-(x_tip1+x_tip2)/2.;
                double dyi=ea_end_y-(y_tip1+y_tip2)/2.;

                w.xy[0] = (float)(x_tip1+dxi);
                w.xy[1] = (float)(y_tip1+dyi);
                w.xy[2] = (float)(ea_end_x+dxi);
                w.xy[3] = (float)(ea_end_y+dyi);
                w.xy[4] = (float)(x_tip2+dxi);
                w.xy[5] = (float)(y_tip2+dyi);
                */

                double ea_end_xx = (n>0) ? ea_end_x : ea_start_x;
                double ea_end_yy = (n>0) ? ea_end_y : ea_start_y;
                double angle_rad = get_elliptical_tangent_angle(ea.angle, ea.rx, ea.ry, (n>0) ?ea.kat2 : ea.kat1);

                // Reverse direction for arrowhead base behind tip (your working flip)
                if (n>0) angle_rad = fmod(angle_rad - M_PI, 2.0 * M_PI);
                if (angle_rad < 0.0) angle_rad += 2.0 * M_PI;

                double iso_angle = cartesian_angle_to_isometric_angle(angle_rad);

                double katS_rad = katS * M_PI / 180.0;

                double iso_angle1 = fmod(iso_angle + katS_rad, 2.0 * M_PI);
                double iso_angle2 = fmod(iso_angle - katS_rad, 2.0 * M_PI);

                double iso_angle1_deg = iso_angle1 * 180.0 / M_PI;
                double iso_angle2_deg = iso_angle2 * 180.0 / M_PI;

                double x_tip1, y_tip1, x_tip2, y_tip2;
                isometric_polar_to_cartesian(ea_end_xx, ea_end_yy, Kp2s, iso_angle1_deg, &x_tip1, &y_tip1);
                isometric_polar_to_cartesian(ea_end_xx, ea_end_yy, Kp2s, iso_angle2_deg, &x_tip2, &y_tip2);

                double dxi = ea_end_xx - (x_tip1 + x_tip2) / 2.0;
                double dyi = ea_end_yy - (y_tip1 + y_tip2) / 2.0;

                w.xy[0] = (float)(x_tip1 + dxi);
                w.xy[1] = (float)(y_tip1 + dyi);
                w.xy[2] = (float)(ea_end_xx + dxi);
                w.xy[3] = (float)(ea_end_yy + dyi);
                w.xy[4] = (float)(x_tip2 + dxi);
                w.xy[5] = (float)(y_tip2 + dyi);

            }
           /////////////////
            w.lp = 6;
            w.n = 32;

            if (s==0)
            {
                w.empty_typ=0;
            }
            else
            {
                w.empty_typ=3;
            }

            if (wielokat_visible(&w))
                rysuj_obiekt_(&w, mode, kolor);

            out_v_krz (pikseleX0(ptrs_vector->x1), pikseleY0(ptrs_vector->y1));

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                outtextxy_w_0(&Vltxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw=TRUE;
            }
            else
            {
                outtextxy_w(&Vtxt,mode);
                outtextxy_w(&Vltxt,mode);
            }
            break;
        case 10:  //trapezium Y
        case 17:  //trapezium Y slab
        case 20:  //trapezium Z
            if (ptrs_vector->style==17)  //slab load
            {
                load_ellipse.kolor=ptrs_vector->kolor;
                load_ellipse.warstwa=ptrs_vector->warstwa;
                load_ellipse.widoczny=ptrs_vector->widoczny;
                load_ellipse.translucency=51;
                load_ellipse.typ=64;
                load_ellipse.x=(float)((ptrs_vector->x1+ptrs_vector->x2)/2.);
                load_ellipse.y=(float)((ptrs_vector->y1+ptrs_vector->y2)/2.);
                load_ellipse.angle=PL.kat*Pi/180;
                load_ellipse.rx=(float)(PL.dl/2.);
                load_ellipse.ry=(float)(load_ellipse.rx/3.);
                rysuj_obiekt_((void*)&load_ellipse, mode, kolor);
            }

            L1.typ=64;
            rysuj_obiekt_(&L1, mode, kolor);

            w.empty_typ=0;

            if ((!(ptrs_vector->cartflags & 1))  || (ptrs_vector->style!=10))  //TUTAJ
            {
                w.xy[0] = L1.x1;
                w.xy[1] = L1.y1;
                w.xy[2] = L1.x2;
                w.xy[3] = L1.y2;
                w.xy[4] = L1.x2;
                w.xy[5] = Lt.y2;
                w.xy[6] = L1.x1;
                w.xy[7] = Lt.y1;
            }
            else
            {
                w.xy[0] = L1.x1;
                w.xy[1] = L1.y1;
                w.xy[2] = L1.x2;
                w.xy[3] = L1.y2;
                w.xy[4] = Lt.x2;
                w.xy[5] = Lt.y2;
                w.xy[6] = Lt.x1;
                w.xy[7] = Lt.y1;
            }

            w.lp=8;

            L1.typ=64;
            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=w.xy[4];
            L1.y2=w.xy[5];
            rysuj_obiekt_(&L1, mode, kolor);

            if ((!Check_if_Equal(PL.kat, 90)) && (!Check_if_Equal(PL.kat, 270)))
            {
                L1.x1 = w.xy[6];
                L1.y1 = w.xy[7];
                rysuj_obiekt_(&L1, mode, kolor);

                L1.x2 = w.xy[0];
                L1.y2 = w.xy[1];
                rysuj_obiekt_(&L1, mode, kolor);
            }

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            rysuj_obiekt_(&w, mode, kolor);

            if (TRANSLUCENCY!=TRANS) set_trans_blender(0, 0, 0, (int)TRANSLUCENCY);  //coming back
            set_mode_solid();

            if ((!(ptrs_vector->cartflags & 1)) || (ptrs_vector->style!=10))
                arrow_angle=Pi_ / 2.;
            else arrow_angle=150./180.*Pi_;

			if (!make_arrows(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], arrow_angle, ptrs_vector, PL.kat, mode, kolor, vkolor, redraw_obj))
				return;

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                outtextxy_w_0(&Vtxt1);
                outtextxy_w_0(&Vltxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw=TRUE;
            }
            else
            {
                outtextxy_w(&Vtxt, mode);
                outtextxy_w(&Vtxt1, mode);
                outtextxy_w(&Vltxt, mode);
            }
            break;

        case 11:  //trapezium X
            L1.typ=64;
            rysuj_obiekt_(&L1, mode, kolor);

            w.empty_typ=0;

            if (!(ptrs_vector->cartflags & 1))
            {
                w.xy[0] = L1.x1;
                w.xy[1] = L1.y1;
                w.xy[2] = L1.x2;
                w.xy[3] = L1.y2;
                w.xy[4] = Lt.x2;
                w.xy[5] = L1.y2;
                w.xy[6] = Lt.x1;
                w.xy[7] = L1.y1;
            }
            else
            {
                w.xy[0] = L1.x1;
                w.xy[1] = L1.y1;
                w.xy[2] = L1.x2;
                w.xy[3] = L1.y2;
                w.xy[4] = Lt.x2;
                w.xy[5] = Lt.y2;
                w.xy[6] = Lt.x1;
                w.xy[7] = Lt.y1;
            }
            
            w.lp=8;

            L1.typ=64;
            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=w.xy[4];
            L1.y2=w.xy[5];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x1=w.xy[6];
            L1.y1=w.xy[7];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x2=w.xy[0];
            L1.y2=w.xy[1];
            rysuj_obiekt_(&L1, mode, kolor);

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            rysuj_obiekt_(&w, mode, kolor);

            if (TRANSLUCENCY!=TRANS) set_trans_blender(0, 0, 0, (int)TRANSLUCENCY);  //coming back
            set_mode_solid();

            if (!(ptrs_vector->cartflags & 1)) arrow_angle=0.;
            else arrow_angle=30./180.*Pi_;

			if (!make_arrows(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], arrow_angle, ptrs_vector, PL.kat, mode, kolor, vkolor, redraw_obj))
				return;

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                outtextxy_w_0(&Vtxt1);
                outtextxy_w_0(&Vltxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw=TRUE;
            }
            else
            {
                outtextxy_w(&Vtxt, mode);
                outtextxy_w(&Vtxt1, mode);
                outtextxy_w(&Vltxt, mode);
            }
            break;
        case 12:  //trapezium N
            L1.typ=64;
            rysuj_obiekt_(&L1, mode, kolor);

            w.empty_typ=0;

            w.xy[0]=L1.x1;
            w.xy[1]=L1.y1;
            w.xy[2]=L1.x2;
            w.xy[3]=L1.y2;
            
            w.xy[4]=Lt.x2;
            w.xy[5]=Lt.y2;
            w.xy[6]=Lt.x1;
            w.xy[7]=Lt.y1;

            w.lp=8;

            L1.typ=64;
            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=w.xy[4];
            L1.y2=w.xy[5];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x1=w.xy[6];
            L1.y1=w.xy[7];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x2=w.xy[0];
            L1.y2=w.xy[1];
            rysuj_obiekt_(&L1, mode, kolor);

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            rysuj_obiekt_(&w, mode, kolor);

            if (TRANSLUCENCY!=TRANS) set_trans_blender(0, 0, 0, (int)TRANSLUCENCY);  //coming back
            set_mode_solid();

			if (!make_arrows(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], /*Pi_*(PL.kat+90.0)/180.0*/ arrow_angle, ptrs_vector, PL.kat, mode, kolor, vkolor, redraw_obj))
				return;

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                outtextxy_w_0(&Vtxt1);
                outtextxy_w_0(&Vltxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw=TRUE;
            }
            else
            {
                outtextxy_w(&Vtxt, mode);
                outtextxy_w(&Vtxt1, mode);
                outtextxy_w(&Vltxt, mode);
            }
            break;

            //////////////////
        case V_EDGE_SIMPLE:  //simple supported edge
        case V_EDGE_SIMPLE_INV:  //simple supported edge reversed
        case V_EDGE_FIXED:  //fixed edge
        case V_EDGE_FIXED_INV:  //fixed edge reversed
        case V_EDGE_ROLL:  //roll edge
        case V_EDGE_ROLL_INV:  //roll edge reversed
            L1.typ=ptrs_vector->typ; //64;
            rysuj_obiekt_(&L1, mode, kolor);

            w.empty_typ=0;

            w.xy[0]=L1.x1;
            w.xy[1]=L1.y1;
            w.xy[2]=L1.x2;
            w.xy[3]=L1.y2;

            w.xy[4]=Lt.x2;
            w.xy[5]=Lt.y2;
            w.xy[6]=Lt.x1;
            w.xy[7]=Lt.y1;
            w.lp=8;
            w.n=40;

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);
            /*
            L1.typ=64;
            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=w.xy[4];
            L1.y2=w.xy[5];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x1=w.xy[6];
            L1.y1=w.xy[7];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x2=w.xy[0];
            L1.y2=w.xy[1];
            rysuj_obiekt_(&L1, mode, kolor);

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            rysuj_obiekt_(&w, mode, kolor);

            if (TRANSLUCENCY!=TRANS) set_trans_blender(0, 0, 0, (int)TRANSLUCENCY);  //coming back
            */
            set_mode_solid();

            if (!make_arrows(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], Pi_ * (PL.kat + 90.0) / 180.0, ptrs_vector, PL.kat, mode, kolor, vkolor, redraw_obj))
                return;

            break;
            //////////////////
        case 13:  //trapezium H
            L1.typ=64;
            rysuj_obiekt_(&L1, mode, kolor);

            if (!(ptrs_vector->cartflags & 1))
            {
                if (L1.x1 < L1.x2) {
                    n = 1;
                    w.xy[1] = w.xy[3] = max(L1.y1, L1.y2);
                } else {
                    n = -1;
                    w.xy[1] = w.xy[3] = min(L1.y1, L1.y2);
                }

                w.xy[0] = L1.x1;
                w.xy[2] = L1.x2;
                w.xy[4] = L1.x2;
                w.xy[5] = Lt.y2;
                w.xy[6] = L1.x1;
                w.xy[7] = Lt.y1;
            }
            else
            {
                w.xy[0] = Lp.x1;
                w.xy[1] = Lp.y1;
                w.xy[2] = Lp.x2;
                w.xy[3] = Lp.y2;
                w.xy[4] = Lt.x2;
                w.xy[5] = Lt.y2;
                w.xy[6] = Lt.x1;
                w.xy[7] = Lt.y1;
            }

            w.lp=8;

            L1.typ=33;
            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=ptrs_vector->x1;
            L1.y2=ptrs_vector->y1;
            rysuj_obiekt_(&L1, mode, kolor);
            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=ptrs_vector->x2;
            L1.y2=ptrs_vector->y2;
            rysuj_obiekt_(&L1, mode, kolor);

            L1.typ=64;
            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=w.xy[2];
            L1.y2=w.xy[3];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x1=w.xy[4];
            L1.y1=w.xy[5];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x2=w.xy[6];
            L1.y2=w.xy[7];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            rysuj_obiekt_(&L1, mode, kolor);

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            rysuj_obiekt_(&w, mode, kolor);

            if (TRANSLUCENCY!=TRANS) set_trans_blender(0, 0, 0, (int)TRANSLUCENCY);  //coming back
            set_mode_solid();

            if (!(ptrs_vector->cartflags & 1))
                arrow_angle=Pi_ / 2.;
            else arrow_angle=150./180.*Pi_;

			if (!make_arrows(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], arrow_angle, ptrs_vector, PL.kat, mode, kolor, vkolor, redraw_obj))
				return;

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                outtextxy_w_0(&Vtxt1);
                outtextxy_w_0(&Vltxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw=TRUE;
            }
            else
            {
                outtextxy_w(&Vtxt, mode);
                outtextxy_w(&Vtxt1, mode);
                outtextxy_w(&Vltxt, mode);
            }
            break;
        case 14:  //trapezium V
            L1.typ=64;
            rysuj_obiekt_(&L1, mode, kolor);

            if (!(ptrs_vector->cartflags & 1))
            {
                if (L1.y1 < L1.y2) {
                    n = 1;
                    w.xy[0] = w.xy[2] = min(L1.x1, L1.x2);
                } else {
                    n = -1;
                    w.xy[0] = w.xy[2] = max(L1.x1, L1.x2);
                }

                w.xy[1] = L1.y1;
                w.xy[3] = L1.y2;
                w.xy[4] = Lt.x2;
                w.xy[5] = L1.y2;
                w.xy[6] = Lt.x1;
                w.xy[7] = L1.y1;
            }
            else
            {
                w.xy[0] = Lp.x1;
                w.xy[1] = Lp.y1;
                w.xy[2] = Lp.x2;
                w.xy[3] = Lp.y2;
                w.xy[4] = Lt.x2;
                w.xy[5] = Lt.y2;
                w.xy[6] = Lt.x1;
                w.xy[7] = Lt.y1;
            }

            w.lp=8;

            L1.typ=33;
            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=ptrs_vector->x1;
            L1.y2=ptrs_vector->y1;
            rysuj_obiekt_(&L1, mode, kolor);
            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=ptrs_vector->x2;
            L1.y2=ptrs_vector->y2;
            rysuj_obiekt_(&L1, mode, kolor);

            L1.typ=64;
            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=w.xy[2];
            L1.y2=w.xy[3];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x1=w.xy[4];
            L1.y1=w.xy[5];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x2=w.xy[6];
            L1.y2=w.xy[7];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            rysuj_obiekt_(&L1, mode, kolor);

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            rysuj_obiekt_(&w, mode, kolor);

            if (TRANSLUCENCY!=TRANS) set_trans_blender(0, 0, 0, (int)TRANSLUCENCY);  //coming back
            set_mode_solid();

            if (!(ptrs_vector->cartflags & 1))
                arrow_angle=0.;
            else arrow_angle=30./180.*Pi_;

			if (!make_arrows(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], arrow_angle, ptrs_vector, PL.kat, mode, kolor, vkolor, redraw_obj))
				return;

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                outtextxy_w_0(&Vtxt1);
                outtextxy_w_0(&Vltxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw=TRUE;
            }
            else
            {
                outtextxy_w(&Vtxt, mode);
                outtextxy_w(&Vtxt1, mode);
                outtextxy_w(&Vltxt, mode);
            }
            break;
        case 15:  //THERMAL
            L1.typ=64;
            rysuj_obiekt_(&L1, mode, kolor);

            Lth.typ=64;
            rysuj_obiekt_(&Lth, mode, kolor);

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

            L1.typ=64;
            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=w.xy[4];
            L1.y2=w.xy[5];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x1=w.xy[6];
            L1.y1=w.xy[7];
            rysuj_obiekt_(&L1, mode, kolor);

            L1.x2=w.xy[0];
            L1.y2=w.xy[1];
            rysuj_obiekt_(&L1, mode, kolor);

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            rysuj_obiekt_(&w, mode, kolor);

            if (TRANSLUCENCY!=TRANS) set_trans_blender(0, 0, 0, (int)TRANSLUCENCY);  //coming back
            set_mode_solid();

			if (!make_arrows(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], Pi_ * (PLth.kat + 90.0) / 180.0, ptrs_vector, PLth.kat, mode, kolor, vkolor, redraw_obj))
				return;

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                outtextxy_w_0(&Vtxt1);
                outtextxy_w_0(&Vltxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw=TRUE;
            }
            else
            {
                outtextxy_w(&Vtxt, mode);
                outtextxy_w(&Vtxt1, mode);
                outtextxy_w(&Vltxt, mode);
            }
            break;
        case 16:  //node radius

            if (kolor) {
                if (TRANSLUCENCY!=TRANS) set_trans_blender(0, 0, 0, (int)TRANS);
                set_mode_trans();
            }
            K.x=ptrs_vector->x1;
            K.y=ptrs_vector->y1;
            K.r=ptrs_vector->r;
            K.typ=64;
            if (okrag_visible(&K))
                rysuj_obiekt_(&K, mode, kolor);
            if (kolor) {
                if (TRANSLUCENCY != TRANS) set_trans_blender(0, 0, 0, (int) TRANSLUCENCY);  //coming back
                set_mode_solid();
            }
            O.x=ptrs_vector->x1;
            O.y=ptrs_vector->y1;
            O.r=ptrs_vector->r;
            O.typ=64;
            if (okrag_visible(&O))
                rysuj_obiekt_(&O, mode, kolor);
            out_v_krz (pikseleX0 (ptrs_vector->x1), pikseleY0(ptrs_vector->y1));

            L1.typ=Ln1.typ=32;
            rysuj_obiekt_(&L1, mode, kolor);

            Ln1.x1=L1.x2;
            Ln1.y1=L1.y2;
            Ln1.y2=Ln1.y1;

            double t_len_mm = Get_TextLen(&Vtxt, Vtxt.text);
            double direction=L1.x2-L1.x1;
            if (direction > 0) //to the right
            {
                Vtxt.x = L1.x2 + Vtxt.wysokosc / 4.0f;
                Vtxt.y = L1.y2 + Vtxt.wysokosc / 4.0f;
                Vtxt.justowanie = j_do_lewej;

                if (t_len_mm == 0.0) Ln1.x2 = L1.x2 + 0.01f;
                else Ln1.x2 = L1.x2 + (float)t_len_mm + Vtxt.wysokosc / 2.5f;
            } else  //to the left
            {
                Vtxt.x = L1.x2 - Vtxt.wysokosc / 4.0;
                Vtxt.y = L1.y2 + Vtxt.wysokosc / 4.0;
                Vtxt.justowanie = j_do_prawej;
                if (t_len_mm == 0.0) Ln1.x2 = L1.x2 - 0.01;
                else Ln1.x2 = L1.x2 - (float)t_len_mm - Vtxt.wysokosc / 2.0f;
            }

            rysuj_obiekt_(&Ln1, mode, kolor);

            K1_5=jednostkiplt(w1_5);
            Kp2sn = 1.686909582 /*(2.5 / 2.47) * (2.5/1.5)*/ * K1_5 * view_vector_scale;

            n=-1;
            w.xy[2]=Ln.x1;
            w.xy[3]=Ln.y1;
            w.xy[0]=Ln.x1-(float)(n*Kp2sn*cos(Pi*(kat1-kat0)/180.));
            w.xy[1]=Ln.y1-(float)(n*Kp2sn*sin(Pi*(kat1-kat0)/180.));
            w.xy[4]=Ln.x1-(float)(n*Kp2sn*cos(Pi*(kat1+kat0)/180.));
            w.xy[5]=Ln.y1-(float)(n*Kp2sn*sin(Pi*(kat1+kat0)/180.));
            w.lp = 6;
            w.n = 8 + w.lp * sizeof (float) ;
            if (wielokat_visible(&w))
                rysuj_obiekt_(&w, mode, kolor);

            if (!kolor) {
                outtextxy_w_0(&Vtxt);
                if (PTRS__Text_Style[Vtxt.czcionka]->type == 2) TTF_redraw=TRUE;
            }
            else
            {
                outtextxy_w(&Vtxt, mode);
            }

            break;
        default:
            break;
    }
}


int get_first_ctx(void)
{
	return first_ctx;
}

BOOL get_ctx_created(void)
{
	return ctx_created;
}

int get_i_ctx_created(void)
{
	return i_ctx_created;
}

int get_i_ctx(void)
{
	return i_ctx;
}

BOOL get_ctx_ok(void)
{
	return ctx_ok;
}

BOOL get_regen_ctx(void)
{
	return regen_ctx;
}


void get_ctx_par(int i, Ctx_Par *rev_ctx_par)
{
	rev_ctx_par->x0 = ctx_par[i].x0;
	rev_ctx_par->y0 = ctx_par[i].y0;
    rev_ctx_par->ww = ctx_par[i].ww;
	rev_ctx_par->wh = ctx_par[i].wh;
}

void put_ctx_par(int i, Ctx_Par *rev_ctx_par)
{
	ctx_par[i].x0 = rev_ctx_par->x0;
	ctx_par[i].y0 = rev_ctx_par->y0;
	ctx_par[i].ww = rev_ctx_par->ww;
	ctx_par[i].wh = rev_ctx_par->wh;
}

char *get_ctx_bitmap(int i)
{
	return (char *)ctx_bitmap[i];
}

void put_ctx_bitmap(int i, char *ctx_bmp)
{
	ctx_bitmap[i] = (BITMAP *)ctx_bmp;
}

void put_ctx_created(BOOL ctx_created_)
{
	ctx_created= ctx_created_;
}

void put_i_ctx_created(int i_ctx_created_)
{
	i_ctx_created = i_ctx_created_;
}

void put_i_ctx(int i_ctx_)
{
	i_ctx = i_ctx_;
}

void put_ctx_ok(BOOL ctx_ok_)
{
	ctx_ok = ctx_ok_;
}

void put_regen_ctx(BOOL regen_ctx_)
{
	regen_ctx = regen_ctx_;
}

void null_ctx_pointers_tail(int start)
{
	int i;
	for (i = start; i < 32; i++)
	{
		ctx_bitmap[i] = NULL;
	}
}

void null_all_ctx_pointers(void)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		ctx_bitmap[i] = NULL;
	}
	i_ctx_created = 0;
	ctx_created = 0;
	imageBMPsize_used = 0;
}

void Destroy_All_ctx(void)
{ int j;

  if (ctx_created==TRUE)
    {
     for (j=0; j<i_ctx_created; j++)
      {
		 if (ctx_bitmap[j])
		 {
			 destroy_bitmap(ctx_bitmap[j]);
			 ctx_bitmap[j] = NULL;
		 }
      }
     ctx_created=FALSE;
     i_ctx_created=0;
    }
  imageBMPsize_total_used -= imageBMPsize_used;
  imageBMPsize_used = 0;
}


void ClearScreen(void)
{ //struct viewporttype viewinfo3;
 
  clear_to_color(screen,get_palette_color(kolory.paper));

}


static void ClearScreen_second(void)
{
	clear_to_color((BITMAP*)second_screen, get_palette_color(kolory.paper));
	clear_zbuffer(zbuf, -0.0001); //-99999);
}

static void ClearScreen_second_preview(void)
{

	clear_to_color((BITMAP*)second_screen, get_palette_color(kolory.paper));
	clear_zbuffer(zbuf, -0.0001); //-99999);
}



void SetBackground(char *background_file_name)
{
	reset_special_background(background_file_name);
	redraw();
	return;
}

void Copy_cursor_pattern_bitmap(SOLID_PATTERN_LIBRARY_ITEM* solid_pattern_library_item)
{

	int width_max = 1024;
	int height_max = 1024;

	int xy[8];
	SOLID_PATTERN solid_pattern;
	int bi;


	if (cursor_pattern_bitmap0 != NULL) destroy_bitmap(cursor_pattern_bitmap0);
	cursor_pattern_bitmap0 = create_system_bitmap(cursor_icon_size, cursor_icon_size);
	if (cursor_pattern_bitmap != NULL) destroy_bitmap(cursor_pattern_bitmap);
	cursor_pattern_bitmap = create_system_bitmap(cursor_icon_size, cursor_icon_size);


	strcpy(solid_pattern.pattern, solid_pattern_library_item->pattern_name);
	solid_pattern.scale = SolidHatchPatternScale;
	solid_pattern.angle = 0; // SolidHatchPatternAngle;
	solid_pattern.dx = SolidHatchPatternDx;
	solid_pattern.dy = SolidHatchPatternDy;
	solid_pattern.flag = 0;

	bi = setfillpattern_gregre_scaled(&solid_pattern, FALSE, 0.0, SolidHatchPatternDx, SolidHatchPatternDy, NULL, NULL);
	xy[0] = 0; xy[1] = 0; xy[2] = cursor_icon_size; xy[3] = 0; xy[4] = cursor_icon_size; xy[5] = cursor_icon_size; xy[6] = 0; xy[7] = cursor_icon_size;
	polygon(cursor_pattern_bitmap0, 4, xy, 0);
	rotate_sprite(cursor_pattern_bitmap, cursor_pattern_bitmap0, 0, 0, itofix(fixangles[SolidHatchPatternAngle]));  //0,0

}


int SetPattern(char* pattern_path_name, char* pattern_file_name, char* ext_)
{
	BOOL found;
	int i;
	SOLID_PATTERN_LIBRARY_ITEM* solid_pattern_library_item;
	int readed = 0;
	int file_size = 0;
	char* buffer;
	char p_type;
    char ext[6];

    strcpy(ext, ext_);
    strupr(ext);

	if (strcmp(ext, ".PCX") == 0) p_type = 1;
	else if (strcmp(ext, ".PNG") == 0) p_type = 2;
	else if ((strcmp(ext, ".JPG") == 0) || (strcmp(ext, ".JPEG") == 0)) p_type = 3;
	else return 0;

	found = FALSE;
	for (i = 0; i < solid_pattern_library_no; i++)
	{
		solid_pattern_library_item = (SOLID_PATTERN_LIBRARY_ITEM*)(Solid_Pattern_Library[i]);
		if (strcmp(pattern_file_name, solid_pattern_library_item->pattern_name) == 0)
		{
			found = TRUE;
			Copy_cursor_pattern_bitmap(solid_pattern_library_item);
			break;
		}
	}
	if (found == FALSE)
	{
		if (solid_pattern_library_no < (SOLID_PATTERN_LIBRARY_MAX_ITEMS-1))
		{
			//loading file
			FILE* input = fopen(pattern_path_name, "rb");
			//get Filesize 
			if (input != NULL)
			{
				fseek(input, 0, SEEK_END);
				file_size = ftell(input);
				rewind(input);
				//Allocate memory for buffer
				buffer = malloc(file_size);
				if (buffer != NULL)
				{
					readed = fread(buffer, file_size, 1, input);
					fclose(input);
					if (readed == 1)
					{
						solid_pattern_library_item = (SOLID_PATTERN_LIBRARY_ITEM*)malloc(sizeof(SOLID_PATTERN_LIBRARY_ITEM));
						if (solid_pattern_library_item != NULL)
						{
							solid_pattern_library_item->type = p_type;
							strncpy(solid_pattern_library_item->pattern_name, pattern_file_name, 32);
							solid_pattern_library_item->size = file_size;
							solid_pattern_library_item->pattern_buffer = buffer;
							Solid_Pattern_Library[solid_pattern_library_no] = (char*)solid_pattern_library_item;
							solid_pattern_library_no++;
							Copy_cursor_pattern_bitmap(solid_pattern_library_item);
							return 1;
						}
						else
						{
							free(buffer);
							return 0;
						}
					}
					else
					{
						free(buffer);
						return 0;
					}
				}
				fclose(input);
			}
			else return 0;
		}
	}
	
	return 1;
}

int get_solid_pattern_library_no(void)
{
	return solid_pattern_library_no;
}

int LoadPattern(SOLID_PATTERN_LIBRARY_ITEM* pattern_mem)
{
	BOOL found;
	int i;
	SOLID_PATTERN_LIBRARY_ITEM* solid_pattern_library_item;
	char* buffer;

	found = FALSE;
	for (i = 0; i < solid_pattern_library_no; i++)
	{
		solid_pattern_library_item = (SOLID_PATTERN_LIBRARY_ITEM*)(Solid_Pattern_Library[i]);
		if (strcmp(pattern_mem->pattern_name, solid_pattern_library_item->pattern_name) == 0)
		{
			found = TRUE;
			break;
		}
	}
	if (found == FALSE)
	{
		if (solid_pattern_library_no < (SOLID_PATTERN_LIBRARY_MAX_ITEMS - 1))
		{
			//Allocate memory for buffer
			buffer = malloc(pattern_mem->size);
			if (buffer != NULL)
			{
				memmove(buffer, pattern_mem->pattern_buffer, pattern_mem->size);
				
				solid_pattern_library_item = (SOLID_PATTERN_LIBRARY_ITEM*)malloc(sizeof(SOLID_PATTERN_LIBRARY_ITEM));
				if (solid_pattern_library_item != NULL)
				{
					solid_pattern_library_item->type = pattern_mem->type;
					solid_pattern_library_item->size = pattern_mem->size;
					strncpy(solid_pattern_library_item->pattern_name, pattern_mem->pattern_name, 32);
					solid_pattern_library_item->pattern_buffer = buffer;
					Solid_Pattern_Library[solid_pattern_library_no] = (char*)solid_pattern_library_item;
					solid_pattern_library_no++;
					return 1;
				}
				else
				{
					free(buffer);
					return 0;
				}
			}
			else return 0;
		}
	}
	return 1;
}


int GetPattern(char* pattern_name, char **pattern_ptr)
{
	int i;
	BOOL found;
	SOLID_PATTERN_LIBRARY_ITEM* solid_pattern_library_item;

	found = FALSE;
	for (i = 0; i < solid_pattern_library_no; i++)
	{
		solid_pattern_library_item = (SOLID_PATTERN_LIBRARY_ITEM*)(Solid_Pattern_Library[i]);
		if (strcmp(pattern_name, solid_pattern_library_item->pattern_name) == 0)
		{
			found = TRUE;
			*pattern_ptr = solid_pattern_library_item->pattern_buffer;
			return solid_pattern_library_item->type;
		}
	}
	return 0;
}

void DonePatterns(void)
{
	int i;
	SOLID_PATTERN_LIBRARY_ITEM* solid_pattern_library_item;

	for (i = 0; i < solid_pattern_library_no; i++)
	{
		solid_pattern_library_item = (SOLID_PATTERN_LIBRARY_ITEM*)(Solid_Pattern_Library[i]);
		free(solid_pattern_library_item->pattern_buffer);
		free((SOLID_PATTERN_LIBRARY_ITEM*)Solid_Pattern_Library[i]);
	}
}

static void rysuj_bitmapy(void)
/*---------------------------*/
{
  NAGLOWEK *nag;
  B_PCX *b_pcx;
  WIELOKAT* w;
  SOLIDARC *sa;
  int j, jj;
  char  *adh;
  BOOL b_break = FALSE, b_info_break ;
  double xs, ys ;
  int mode = COPY_PUT ;
  int kk;
  long ww,wh,ww1,ww0,wh0,ww_alloc,wh_alloc;
  double ww_,wh_;
  long mxx, mxy, mmx, mmy;
  long mmx1, mxx1, mmy1, mxy1, mmx2, mmy2, mxx2, mxy2, dxx2, dyy2, mmx3, mmy3, dxx3, dyy3, mmx4;
  char st[60];
  long req_RAM;
  int ii_loop;
  double d_ctx;
  unsigned long_long req_size, req_size1;
  double pcx_range_xp,pcx_range_yp,pcx_range_xk,pcx_range_yk;
  unsigned long_long imageBMPsize_total;
  BOOL stop_buffering;
  double scale_factor;
  double skala0; // , skala1;
  long xpsk1,xksk1,ypsk1,yksk1;
  int ijk;
  SOLID_PATTERN solid_pattern = { 2,0,0,0,0,""};
  char* scale_ptr;
  char* dx_ptr;
  char* angle_ptr;
  char* dy_ptr;
  char* name_ptr;
  T_PixelTVal PolyPoints[MaxNumPoints];
  int PolyPointsMon[8] ;
  int NumPoints, i;
  char *translucency_ptr, *gradient_ptr=NULL;
  unsigned char translucency;
  BOOL ignore_p=FALSE;


if (options1.ignore_buffering_pcx==0)
   {
	skala0=skala;
    skala=1; 
	d_2d();
   }
  
 else
 {
   if (Check_if_Equal((DokladnoscF*skala),SkalaFA_ctx)==FALSE) redraw_ctx=1;  //!!!!!!!!!!!!!!!!!!!!
 }

 SkalaFA_ctx=DokladnoscF*skala;

 if (redraw_ctx==1)
   {
     if (Check_if_Equal((DokladnoscF*skala),SkalaF_ctx)==TRUE)
      {
       if (ctx_created==TRUE)
         {
          redraw_ctx=0;
          ctx_ok=TRUE;
          first_ctx=TRUE;
         }
      }
   }

 
 if (regen_ctx==TRUE)
   {
     redraw_ctx=1;
     regen_ctx=FALSE;
   }


 if (options1.ignore_buffering_pcx==1)
   {
     redraw_ctx=0;
     ctx_ok=FALSE;
   }

 if (redraw_ctx==1)
   {
   if (bitmap_exist)
   {
#ifdef PCX_RANGE
	 //okreslenie zakresu blokow PCX na widocznych warstwach
     
     Get_Limits_PCX(0,dane_size,ANieOkreslony,&pcx_range_xp,&pcx_range_xk,&pcx_range_yp,&pcx_range_yk);
	 ww_ = (pcx_range_xk - pcx_range_xp) * sk_x + 8;  //uwzgledniono aspect ratio
	 wh_ = (pcx_range_yk - pcx_range_yp) + 8;   //28-07-2021 added 100 to ww_ and wh_

     if (ww_>FormatX)  ww_=FormatX;
	 if (wh_>FormatY)  wh_=FormatY;

	 ww=pikseleX00(ww_);
	 wh=pikseleY00(wh_);

	 ww_alloc=ww*(1+(2*fabs(1-sk_x)));
	 wh_alloc=wh*(1+(2*fabs(1-sk_x)));  

	 ww0=pikseleX00(pcx_range_xp);
	 wh0=pikseleY00(FormatY-pcx_range_yk);


	 if (ww0<0) ww0=0;
	 if (wh0<0) wh0=0;

#else
	 ww0=0;
	 wh0=0;
     ww=pikseleX00(FormatX);
     wh=pikseleY00(FormatY);
#endif


	 req_size = imagesizelong(0, 0, ww + 10, wh + 10);  //z zapasem


	 imageBMPsize=Get_imageBMPsize();
	 imageBMPsize_total=(imageBMPsize+imageBMPsize_used) * 0.9;  //z zapasem

     if (req_size>=imageBMPsize_total)
      {
        redraw_ctx=0;
        ctx_ok=FALSE;

		skala = skala0;
		d_2d();
      }


   ctx_ok=FALSE;
   if (ctx_created==TRUE)
    {
     for (j=0; j<i_ctx_created; j++)
      {

        destroy_bitmap(ctx_bitmap[j]);

      }
     ctx_created=FALSE;
     i_ctx_created=0;
    }

   imageBMPsize_total_used -= imageBMPsize_used;
   imageBMPsize_used=0;

   imageBMPsize=Get_imageBMPsize();
   imageBMPsize_total=(imageBMPsize+imageBMPsize_used) *0.9;

   if ((req_size<Wielkosc_Plata))  // && (ww<=MAX_CTX_WIDTH))
     {
      ctx_ok=TRUE;
      i_ctx=1;

      ctx_bitmap[0] = create_bitmap(ww+10, wh+10);   //z zapasem 10
	 
  
      if (ctx_bitmap[0]==NULL) 
	  {
		  ctx_ok=FALSE;
	  }

       else
        {
         ctx_created=TRUE;

		 imageBMPsize_used+=req_size;
		 imageBMPsize_total_used += req_size;

         i_ctx_created=1;
#ifdef PCX_RANGE
		 ctx_par[0].x0=ww0;
		 ctx_par[0].y0=wh0;
#else
         ctx_par[0].x0=0;
		 ctx_par[0].y0=0;
#endif
         ctx_par[0].ww=ww;
         ctx_par[0].wh=wh;
        }
     }
    else
     {
       //sprawdzenie czy to sie zmiesci w ogole w pamieci
       if (req_size<imageBMPsize)
        {
           //podzial na pod-ekrany wirtualne
           //okreslenie maksymalnej szerokosci paska

           ww1=(Wielkosc_Plata-sizeof(BITMAP)-(sizeof(char*)*wh)-100) /
                (Bytes_Per_Pixel(c_depth)*wh*Bytes_Per_Pixel(c_depth));
		   

		   ww1 = (int)(ww1 / 8) * 8;

           d_ctx=ww;
           d_ctx/=ww1;
           d_ctx=ceil(d_ctx);
           i_ctx=(int)d_ctx;

		   if (i_ctx > 32)
		   {

                ctx_created = FALSE;
                ctx_ok = FALSE;
                i_ctx_created = 0;

                options1.ignore_buffering_pcx = 0;

                skala = skala0;
                d_2d();

                ctx_ok = FALSE;

		   }
		   else
		   {
			   ctx_ok = TRUE;
			   i_ctx_created = 0;
			   ctx_created = FALSE;
			   for (j = 0; j < i_ctx; j++)
			   {
#ifdef PCX_RANGE
				   ctx_par[j].x0 = ww0 + (j*ww1);
				   ctx_par[j].y0 = wh0;
#else
				   ctx_par[j].x0 = j * ww1;
				   ctx_par[j].y0 = 0;
#endif
				   if (j == (i_ctx - 1)) ctx_par[j].ww = ww - (j*ww1);
				   else ctx_par[j].ww = ww1;
				   ctx_par[j].wh = wh;

				   req_size1 = imagesizelong(0, 0, ctx_par[j].ww + 10, wh + 10);  //z zapasem
				   ctx_bitmap[j] = create_bitmap(ctx_par[j].ww + 10 /*ww1*/, wh + 10);   //z zapasem

				   if (ctx_bitmap[j] == NULL)

				   {
					   ctx_ok = FALSE;
					   for (jj = 0; jj < i_ctx_created; jj++)
					   {

						   destroy_bitmap(ctx_bitmap[jj]);

					   }

					   imageBMPsize_total_used -= imageBMPsize_used;
					   imageBMPsize_used = 0;

					   ctx_created = FALSE;
					   i_ctx_created = 0;
					   //przywrocenie skali
					   
					   skala = skala0;
					   d_2d();
					   
					   break;
				   }
				   else
				   {
					   ctx_created = TRUE;
					   i_ctx_created++;
					   imageBMPsize_used += req_size1;
					   imageBMPsize_total_used += req_size1;

				   }
			   }
		   }
        }
         else
          {
            if (ctx_ok==TRUE)
             {
         
				skala = skala0;
				d_2d();

               ctx_ok=FALSE;
             }
          }
     }

   SkalaF_ctx=(DokladnoscF*skala);
   redraw_ctx=0;
   first_ctx=0;
   }
  else redraw_ctx=0;
  }
else
{
	if ((options1.ignore_buffering_pcx == 0) && (ctx_ok==FALSE))
	{
		skala = skala0;
		d_2d();
	}
}


 xs = (Xp + Xk) / 2 ;
 ys = (Yp + Yk) / 2 ;

 oknoS(Xp,Yp,Xk,Yk);
 b_info_break = FALSE ;
 if (dane_size > 1000000L)
 {
    b_info_break = TRUE ;
	komunikat0(137);  //TEMPORARY
 }

 setwritemode (mode);


 if (ctx_ok==FALSE) ii_loop=1;
  else ii_loop=i_ctx;

  stop_buffering=FALSE;

 for (j=0; j<ii_loop; j++)
 {
  if (ctx_ok==TRUE)
  {
   if (first_ctx==0)
    {

     req_RAM=imagesize(0,0,ctx_par[j].ww+8,ctx_par[j].wh+8);  //z zapasem

     req_RAM/=1024;
     req_RAM/=1024;
     //req_RAM+=0.5;
     sprintf(st,"[%d/%d] %ld MB",j+1,ii_loop,max(req_RAM,1));
     InfoListStr1(3,st);

     Set_Screenplay(ctx_bitmap[j]);

     GrClearContext(get_palette_color(kolory.paper));

    }
  }
  else
  {
	  Set_Screenplay((BITMAP*)second_screen);

	  ////GrClearContext(get_palette_color(kolory.paper));
  }

 if ((ctx_ok==FALSE) || (first_ctx==0))
  {
   if (j==0) bitmap_exist=FALSE;


   for (ijk=0; ijk<256; ijk++)  
   {
	   Warstwy_On_PCX[ijk].check=0;
       Warstwy_On_PCX[ijk].on=0;

	   Warstwy_On_Solids[ijk].check = 0;
	   Warstwy_On_Solids[ijk].on = 0;
   }

   adh=dane;
   adh+=dane_size;
   obiekt_tok_all ((char *)dane, adh, (char **) &nag, Opcx);
   while ((nag!=NULL) && ((char *)nag<adh))
   {
    if (FALSE == Test_Object ((void*)nag))
    {
      goto aa ;
    }
    
    if( nag->atrybut!=Ablok || BlokM==0 || (char *)nag < ADP || (char *)nag > ADK )
     {
      if (no_break == FALSE && b_break == FALSE && my_kbhit() )
      {
 	   if (my_getch() == ESC)
	   {
	    while (my_kbhit ())
	    {
	       my_getch();
	    }
	    b_break = TRUE;
	   }
	   }
       if ( b_break == TRUE)
	   {
  	    nag->widoczny = 0;
	    goto aa;
	   }
      if (nag->obiekt==Opcx)
      {
       b_pcx = (B_PCX *)nag;
	   if ((b_pcx->atrybut != Ausuniety) &&
		   (b_pcx->atrybut != Abad))
	   {
		   if (b_pcx->on_front == 0)
		   {
			   //zaznaczenie warstwy jako warstwy PCX
			   Warstwy_On_PCX[b_pcx->warstwa].check = 1;
			   Warstwy_On_PCX[b_pcx->warstwa].on = Layers[b_pcx->warstwa].on;
			   if (Layers[b_pcx->warstwa].on == 0)
			   {
				   goto aa;
			   }


			   if (ctx_ok == TRUE)
			   {
				   if (first_ctx == 0)
				   {
					   kk = Draw_pcx_virtual(b_pcx, 1, ctx_par[j].x0, ctx_par[j].y0, ctx_par[j].ww, ctx_par[j].wh);
					   if (kk == -1)
					   {
						   stop_buffering = TRUE;
						   //usuniecie niewypelnionych ctx
						   for (jj = j + 1; jj < i_ctx; jj++)
						   {
							   destroy_bitmap(ctx_bitmap[jj]);
						   }
						   i_ctx = j + 1;
						   i_ctx_created = j + 1;
						   j = ii_loop;
						   bitmap_exist = TRUE;
						   if (b_pcx->kod_obiektu == 2) bitmap_png_exist = TRUE;
						   break;
					   }

				   }

			   }
			   else
			   {
				   if (Layers[b_pcx->warstwa].on == 1)
				   {
					  //if (pcx_wybrany(b_pcx, FALSE))
                      if (pcx_w_prostokacie(b_pcx))
                               kk = Draw_pcx(b_pcx);
				   }
			   }
			   bitmap_exist = TRUE;
			   if (b_pcx->kod_obiektu == 2) bitmap_png_exist = TRUE;
		   }
		   else
		   {
			   bitmap_on_front_exist = TRUE;
		   }
	   }
      }
     }
aa:
    obiekt_tok_all(NULL,adh,(char **) &nag,Opcx);
   } //while

   /////////////////////
  }  //ctx_ok, first
 }  //for



//KONIEC BUFOROWANIA
//na razie skala pozostaje 1

if (ctx_ok==TRUE)
 {
   if (first_ctx==0)
    {
     GrSetContext(second_screen/*NULL*/);

     komunikat0(0);
    }

   first_ctx=1;

   //ustalenie wspolrzednych okien do kopiowania

   scale_factor=1/skala0;
   
   mmx=Wyslij_par_ekranu(0);
   mxx=Wyslij_par_ekranu(1);
   mxy=Wyslij_par_ekranu(2);
   mmy=Wyslij_par_ekranu(3);


   xpsk1=(pikseleX00(Xp)-pikseleX00(0))+0.5;
   xksk1=(pikseleX00(Xk)-pikseleX00(0))+0.5;
   ypsk1=(pikseleY0(Yk)-pikseleY0(FormatY))+0.5;
   yksk1=(pikseleY0(Yp)-pikseleY0(FormatY))+0.5;
   
   mmx4 = 0;
   
   for (j=0; j<i_ctx; j++)
     {
        //wspolrzedne X
	    mmx1=ctx_par[j].x0/*+pikseleX0(0)*/;
		mxx1=ctx_par[j].x0+ctx_par[j].ww/*+pikseleX0(0)*/;
		mmy1=ctx_par[j].y0;
		mxy1=ctx_par[j].y0+ctx_par[j].wh;

	    
        if (mxx1>xpsk1)  
         {
          if (mmx1<xksk1)
          {
           if (mmx1<xpsk1)
             {
              mmx2=xpsk1/*+pikseleX0(0)*/-ctx_par[j].x0; 
			  
			  if (mxx1>xksk1)
			  {
				  mxx2=ctx_par[j].ww-(mxx1-xksk1);
			  }
			  else
			  {
				  mxx2=ctx_par[j].ww;
			  }
              
              dxx2=mxx2-mmx2;

			  if (j==0)  mmx3=((double)(mmx2+ctx_par[j].x0-xpsk1/*+pikseleX0(0)*/)/scale_factor)+mmx+0.5;
			  else mmx3 = mmx4;

			  dxx3=(((double)dxx2)/scale_factor)+0.5;

			  mmx4 = mmx3 + dxx3;
             }
              else
               {
                 mmx2=0;
				 
				 if (mxx1>xksk1)
				 {
				   
					 mxx2=ctx_par[j].ww-(mxx1-xksk1);					
				 }
			     else
				 {
				  mxx2=ctx_par[j].ww;
				 }

              
                 dxx2=mxx2-mmx2;
                 
				 if (j == 0) mmx3 = ((double)(mmx2 + ctx_par[j].x0 - xpsk1/*+pikseleX0(0)*/) / scale_factor + mmx) +0.5;
				 else mmx3 = mmx4;

			     dxx3=(((double)dxx2)/scale_factor)+0.5;

				 mmx4 = mmx3 + dxx3;

               }

			  //wspolrzedne Y
			  if (mxy1>ypsk1)  
			  {
               if (mmy1<yksk1)
			   {
                if (mmy1<ypsk1)
				{
                  mmy2=ypsk1-ctx_par[j].y0; 
			  
			      if (mxy1>yksk1)
				  {
				    mxy2=yksk1-ctx_par[j].y0;
				  }
			       else
				   {
				    mxy2=ctx_par[j].wh;
				   }
              
                  dyy2=mxy2-mmy2;

                  mmy3=((double)(mmy2+ctx_par[j].y0-ypsk1)/scale_factor+mmy)+0.5;
			      dyy3=(((double)dyy2)/scale_factor)+0.5;
				}
                 else
				 {
                  mmy2=0;
				  if (mxy1>yksk1)
				  {
				   mxy2=yksk1-ctx_par[j].y0;
				  }
			       else
				   {
				    mxy2=ctx_par[j].wh;
				   }
              
                  dyy2=mxy2-mmy2;
                 
				  mmy3=((double)(mmy2+ctx_par[j].y0-ypsk1)/scale_factor+mmy)+0.5;
			      dyy3=(((double)dyy2)/scale_factor)+0.5;

				 }

				  okno_r();
				  okno_r_second();
			      my_stretch_blit(ctx_bitmap[j],mmx2,mmy2,dxx2,dyy2,mmx3,mmy3,dxx3,dyy3);

				}
			   }
          }
         }
     }


 }

 if (options1.ignore_buffering_pcx==0)
 {
    skala=skala0;
    d_2d();
 }

 if (b_info_break == TRUE)
 {
   ClearInfo ();
 }
 int l_solid = 0;

	 Set_Screenplay(second_screen);
	 oknoS(Xp, Yp, Xk, Yk);

	 adh = dane;
	 adh += dane_size;
	 obiekt_tok_all2((char*)dane, adh, (char**)&nag, Owwielokat, Osolidarc);
	 while ((nag != NULL) && ((char*)nag < adh))
	 {
		 if (FALSE == Test_Object((void*)nag))
		 {
			 goto aaa;
		 }

		 if ((nag->atrybut != Ausuniety) && (nag->atrybut != Abad))
		 {
			 l_solid++;
			 if (no_break == FALSE && b_break == FALSE && my_kbhit())
			 {
				 if (my_getch() == ESC)
				 {
					 while (my_kbhit())
					 {
						 my_getch();
					 }
					 b_break = TRUE;
				 }
			 }
			 if (b_break == TRUE)
			 {
				 nag->widoczny = 0;
				 break;
			 }
			 if (nag->obiekt == Owwielokat)
			 {
				  //////////////////
				 w = (WIELOKAT*)nag;

				 if (Layers[w->warstwa].on != 0)
				 {
					 if ((w->atrybut != Ausuniety) &&
						 (w->atrybut != Abad))
					 {
						 if ((w->empty_typ == 0) && (w->pattern == 1))
						 {
							 //zaznaczenie warstwy jako warstwy Solids
							 Warstwy_On_Solids[w->warstwa].check = 1;
							 Warstwy_On_Solids[w->warstwa].on = Layers[w->warstwa].on;
							
							 bitmap_pattern_exist = TRUE;
							 if (!(w->widoczny = wielokat_wybrany(w)))
							 {
								 if //(TRUE == Check_DArea_in_Last_Solid() ||
									 (TRUE == Point_in_Solid(w, xs, ys))
								 {
									 if (w->empty_typ == 0)
									 {
										 if (w->pattern == 0)
										 {
											 if ((w->warstwa == Current_Layer) || (options1.view_only_current_layer == 0)) setfillstyle_(SOLID_FILL, GetColorAC(w->kolor));
											 else setfillstyle_(SOLID_FILL, GetColorAC(8));
											 set_mode_trans();
											 bar(0, 0, pXk - pXp, pYp - pYk);
											 set_mode_solid();
											 obiekt_tok_all2(NULL, adh, (char**)&nag, Owwielokat, Osolidarc);
											 continue;
										 }
									 }
								 }
								 else
								 {
									 obiekt_tok_all2(NULL, adh, (char**)&nag, Owwielokat, Osolidarc);  //TEMPORARY  06-02-2022
									 continue;                                             //TEMPORARY  
								 }
							 }

							 for (i = 0; i < (int)w->lp && i < MaxNumPoints; i++)
								 if ((i % 2) == 0)
								 {
									 j = i / 2;
									 PolyPoints[i] = pikseleX0(w->xy[i]);
								 }
								 else PolyPoints[i] = pikseleY0(w->xy[i]);

							 NumPoints = i / 2;

							 if ((w->warstwa == Current_Layer) || (options1.view_only_current_layer == 0)) SetColorAC(w->kolor);
							 else SetColorAC(8);

							 setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
							 if ((w->warstwa == Current_Layer) || (options1.view_only_current_layer == 0))
							 {
								 solid_pattern.flag = 0;
								 setfillstyle_(SOLID_FILL, GetColorAC(w->kolor));
							 }
							 else
							 {
								 solid_pattern.flag = 1;  //hide
								 setfillstyle_(SOLID_FILL, GetColorAC(8));
							 }
							 set_mode_trans();

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
							 memmove(&solid_pattern.scale, scale_ptr, sizeof(short int));
							 memmove(&solid_pattern.dx, dx_ptr, sizeof(short int));
							 memmove(&solid_pattern.angle, angle_ptr, sizeof(short int));
							 memmove(&solid_pattern.dy, dy_ptr, sizeof(short int));
							 strcpy(&solid_pattern.pattern, name_ptr);
							 Draw_Solid(NumPoints, PolyPoints, w->pcx_solid, w->obiektt3, pikseleX0(0), pikseleY0(0), &solid_pattern, TRANSLUCENCY, NULL);


                             ///////////////  if Ablok
                             if ((w->atrybut==Ablok) ||
                                 ((dragging_quad_move == TRUE) && (w->atrybut == Aoblok)))
                             {
                                 setcolor(kolory.blok);
                                 setfillstyle_(SOLID_FILL,kolory.blok);

                                 set_trans_blender(0, 0, 0, (int) HALFTRANSLUCENCY);
                                 set_mode_trans();

                                 Draw_Solid(NumPoints, PolyPoints, w->pcx_solid, w->obiektt3, pikseleX0(0), pikseleY0(0), NULL, HALFTRANSLUCENCY, NULL);

                                 reset_trans_blender();
                             }
                             ///////////////

							 set_mode_solid();
						 }
                         else if ((w->empty_typ == 0) && (w->gradient == 1))
                         {
                             if (w->n == (w->lp * sizeof(float) + 8))
                                 w->translucent = 0;

                             if (w->translucent == 1) {
                                 translucency_ptr = (char *) w->xy;
                                 translucency_ptr += (w->lp * sizeof(float));
                                 memmove(&translucency, translucency_ptr, sizeof(unsigned char));
                                 if (translucency < 255) solid_translucent_exist = TRUE;
                             }

                             //GRADIENT  - this part now is ommited due to moving to rysuj_bitmapy();
                             if (w->n == (w->lp * sizeof(float) + 8))
                                 w->gradient = 0;

                             if (w->gradient == 1) {

                                 translucency_ptr = (char*)w->xy;
                                 translucency_ptr += (w->lp * sizeof(float));

                                 //gradient_ptr = (char *) w->xy;
                                 //gradient_ptr += (w->lp * sizeof(float));
                                 gradient_ptr=translucency_ptr+sizeof(unsigned char);
                                 //memmove(&gradient, gradient_ptr, sizeof(GRADIENT));
                                 solid_translucent_exist = TRUE;
                             }

                             if(!(w->widoczny=wielokat_wybrany(w)))
                             {
                                 if (TRUE == Point_in_Solid (w, xs, ys))
                                 {
                                     if ((w->empty_typ==0) && (w->pattern == 0))
                                     {

                                         translucency = 255;

                                         if ((w->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
                                         {
                                             if ((w->atrybut == Ablok) ||
                                                 ((dragging_quad_move == TRUE) && (w->atrybut == Aoblok)))
                                             {
                                                 setfillstyle_(SOLID_FILL,kolory.blok);
                                                 setcolor(kolory.blok);
                                             } else {
                                                 setfillstyle_(SOLID_FILL, GetColorAC(w->kolor));
                                                 SetColorAC(w->kolor);
                                             }
                                         }
                                         else setfillstyle_ (SOLID_FILL, GetColorAC (8)) ;


                                         if ((w->translucent == 1) || (TRANSLUCENCY<255))
                                         {
                                             if (w->translucent == 1)
                                             {
                                                 translucency_ptr = w->xy;
                                                 translucency_ptr += (w->lp * sizeof(float));
                                                 memmove(&translucency, translucency_ptr, sizeof(unsigned char));

                                             }
                                             else translucency = 255;
                                             if (translucency > TRANSLUCENCY) translucency = TRANSLUCENCY;
                                             set_trans_blender(0, 0, 0, (int)translucency);
                                             set_mode_trans();
                                         }
                                         else set_mode_solid();

                                         PolyPointsMon[0] = pXp; PolyPointsMon[1] = /*pYk*/ 0; PolyPointsMon[2] = pXk; PolyPointsMon[3] = /*pYk*/ 0;
                                         PolyPointsMon[4] = pXk; PolyPointsMon[5] = pYp; PolyPointsMon[6] = pXp; PolyPointsMon[7] = pYp;

                                         if (!w->gradient) my_fillpoly(4, PolyPointsMon, translucency, kolory.paper);
                                         else {
                                             NumPoints=w->lp/2;
                                             Draw_Solid(NumPoints, PolyPoints, w->pcx_solid, w->obiektt3, pikseleX0(0), pikseleY0(0), NULL, translucency, (GRADIENT4*)gradient_ptr);
                                         }


                                         if (w->translucent == 1)
                                         {
                                             reset_trans_blender();
                                         }

                                         set_mode_solid();

                                         goto aaa;

                                     }
                                 }
                                 goto aaa;
                             }
                             for(i=0; i<(int)w->lp && i<MaxNumPoints; i++) {
                                 if ((i % 2) == 0) {
                                     j = i / 2;
                                     PolyPoints[i] = pikseleX0(w->xy[i]);
                                     //if (nag->obiekt==Owwielokat3D) PolyPoints_Z[j]=w->xy[w->lp+j];
                                 } else PolyPoints[i] = pikseleY0(w->xy[i]);
                             }
                             PolyPoints[8] = PolyPoints[0];
                             PolyPoints[9] = PolyPoints[1];

                             NumPoints=i/2;

                             translucency = 255;

                             if ((w->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
                             {
                                 if ((w->atrybut==Ablok) ||
                                     ((dragging_quad_move == TRUE) && (w->atrybut == Aoblok)))
                                 {
                                     setcolor(kolory.blok);
                                     setfillstyle_(SOLID_FILL,kolory.blok);
                                 }
                                 else
                                 {
                                     SetColorAC(w->kolor);
                                     setfillstyle_(SOLID_FILL,GetColorAC(w->kolor));
                                 }
                             }
                             else
                             {
                                 SetColorAC(8);
                                 setfillstyle_(SOLID_FILL,GetColorAC(8));
                             }
                             if (w->empty_typ==0)
                             {
                                 if ((w->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
                                 {
                                     if ((w->atrybut==Ablok) ||
                                         ((dragging_quad_move == TRUE) && (w->atrybut == Aoblok)))
                                     {
                                         setcolor(kolory.blok);
                                         setfillstyle_(SOLID_FILL,kolory.blok);
                                     }
                                     else
                                     {
                                         SetColorAC(w->kolor);
                                         setfillstyle_(SOLID_FILL,GetColorAC(w->kolor));
                                     }
                                 }
                                 else
                                 {
                                     SetColorAC(8);
                                     setfillstyle_(SOLID_FILL,GetColorAC(8));
                                 }

                                 setlinestyle1(SOLID_LINE,0,NORM_WIDTH);

                                 ignore_p = FALSE;

                                 if ((w->translucent == 1) || (TRANSLUCENCY < 255))
                                 {

                                     if (w->translucent == 1)
                                     {
                                         translucency_ptr = w->xy;
                                         translucency_ptr += (w->lp * sizeof(float));
                                         memmove(&translucency, translucency_ptr, sizeof(unsigned char));

                                     }
                                     else translucency = 255;

                                     if (translucency > TRANSLUCENCY) translucency = TRANSLUCENCY;
                                     set_trans_blender(0, 0, 0, (int)translucency);
                                     set_mode_trans();
                                 }
                                 else set_mode_solid();
                                 //GRADIENT
                                 gradient_ptr=NULL;
                                 if ((w->empty_typ == 0) && (w->gradient == 1)) {
                                     if (w->n == (w->lp*sizeof(float)+8))
                                         w->gradient=0;

                                     if (w->gradient == 1)
                                     {
                                         translucency_ptr = (char*)w->xy;
                                         translucency_ptr += (w->lp * sizeof(float));

                                         gradient_ptr=translucency_ptr+sizeof(unsigned char);
                                         solid_translucent_exist = TRUE;
                                     }
                                 }

                                 if (!ignore_p)  Draw_Solid(NumPoints, PolyPoints, w->pcx_solid, w->obiektt3, pikseleX0(0), pikseleY0(0), NULL, translucency, (GRADIENT4*)gradient_ptr);

                                 if (w->translucent == 1)
                                 {
                                     reset_trans_blender();
                                     set_mode_solid();
                                 }
                             }
                             set_mode_solid();
                             ///////////////////////////
                         }

					 }
				 }
			 }
             else if (nag->obiekt == Osolidarc)
             {
                 sa=(SOLIDARC*)nag;

                 if (Layers[sa->warstwa].on != 0)
                 {
                     if ((sa->atrybut != Ausuniety) &&
                         (sa->atrybut != Abad))
                     {
                         if ((sa->empty_typ == 0) && (sa->pattern == 1))
                         {
                             //zaznaczenie warstwy jako warstwy Solids
                             Warstwy_On_Solids[sa->warstwa].check = 1;
                             Warstwy_On_Solids[sa->warstwa].on = Layers[sa->warstwa].on;

                             bitmap_pattern_exist = TRUE;

                             rysuj_solidarc_(sa, COPY_PUT, 1, TRUE, FALSE,1,1);

                         }

                     }
                 }
             }
		 }
aaa:
		 obiekt_tok_all2(NULL, adh, (char**)&nag, Owwielokat, Osolidarc);
	 } //while

}


void rysuj_skin(void)
{
	char* adh;
	NAGLOWEK* nag;
	BOOL b_break = FALSE;
	B_PCX *b_pcx;
	int kk;
	int mode = COPY_PUT;
    int gr;
    int kolor;

	adh = dane;
	adh += dane_size;
	obiekt_tok_all((char*)dane, adh, (char**)&nag, Opcx);
	while ((nag != NULL) && ((char*)nag < adh))
	{
		if (FALSE == Test_Object((void*)nag))
		{
			goto aa;
		}
		if (nag->atrybut != Ablok || BlokM == 0 || (char*)nag < ADP || (char*)nag > ADK)
		{
			if (no_break == FALSE && b_break == FALSE && my_kbhit())
			{
				if (my_getch() == ESC)
				{
					while (my_kbhit())
					{
						my_getch();
					}
					b_break = TRUE;
				}
			}
			if (b_break == TRUE)
			{
				nag->widoczny = 0;
				//goto aa;
				break;
			}
			if (nag->obiekt == Opcx)
			{
				b_pcx = (B_PCX*)nag;
				if ((b_pcx->atrybut != Ausuniety) &&
					(b_pcx->atrybut != Abad))
				{
					if (b_pcx->on_front == 1)
					{
						//if (pcx_wybrany(b_pcx, FALSE))
                        if (pcx_w_prostokacie(b_pcx))
						{
							//zaznaczenie warstwy jako warstwy PCX
							Warstwy_On_PCX[b_pcx->warstwa].check = 1;
							Warstwy_On_PCX[b_pcx->warstwa].on = Layers[b_pcx->warstwa].on;
							if (Layers[b_pcx->warstwa].on == 0)
							{
								goto aa;
							}
							kk = Draw_pcx(b_pcx);
							bitmap_on_front_exist = TRUE;
							if (b_pcx->kod_obiektu == 2) bitmap_png_exist = TRUE;
							if (Get_Bitmap_Outline() && (b_pcx->widoczny))
							{
                                if (b_pcx->atrybut == Ablok) { gr=5; kolor=kolory.blok;}
                                else { gr=3; kolor=kolory.cur;}

								rysuj_bitmap_outline((char*)nag, COPY_PUT, FALSE, 1, kolor, gr);

							}
						}
					}
					else
					{
						bitmap_exist = TRUE;
					}
				}
			}
		}
	aa:
		obiekt_tok_all(NULL, adh, (char**)&nag, Opcx);
	} //while
  }
 



void zlikwiduj_wszystkie_bloki(void)
/*--------------------------------*/
{ BLOK *b;
  T_Desc_Ex_Block 	*ptrs_desc_bl1 ;
  LINIA *L;
  NAGLOWEK *nag;
  char  *adh;
  BOOL b_break = FALSE;
  double xs, ys ;
  int mode = COPY_PUT ;
  BOOL Koniec1;

 xs = (Xp + Xk) / 2 ;
 ys = (Yp + Yk) / 2 ;


 InfoList(8);


 adh=dane;

 Koniec1=FALSE;

 while (Koniec1==FALSE)
  {
      nag=(NAGLOWEK *)adh;

    
        switch(nag->obiekt)
        {
         case OdBLOK :
              b=(BLOK *)adh;
              ptrs_desc_bl1	= (T_Desc_Ex_Block *)(&b->opis_obiektu	[0]) ;
    	        if ((strncmp(ptrs_desc_bl1->sz_type,"*#",2)==0) || (strncmp(ptrs_desc_bl1->sz_type,"*^",2)==0) || //to jest jakas siec
                  (strncmp(ptrs_desc_bl1->sz_type,"*~",2)==0) || (strncmp(ptrs_desc_bl1->sz_type,"@kol",4)==0)||
                  (strncmp(ptrs_desc_bl1->sz_type,"@SHEET",6)==0)) //to jest sheet
               {
                b->blok=0;
                adh+=sizeof(NAGLOWEK) + b->n;
               }
                else
                 {
                  b->n=(B3 + b->dlugosc_opisu_obiektu);
                  b->blok=0;
                  adh+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
                 }
       break;
       case Okoniec :
              Koniec1=TRUE;
       break;
	 default :
              L=(LINIA *)adh;
              L->blok=0;
              adh+=sizeof(NAGLOWEK) + L->n;
	   break;  
	} /*switch*/
  }

  InfoList(0);

}


void percentage (int percent)
/*-------------------------*/
{
  int x,y;
  char buf [MaxTextLen] ;

  if (glb_silent) return;

  x = maxX - 32 * WIDTH;
  y = 1;
  setfillstyle_(SOLID_FILL, BKCOLOR) ;
  bar(x, y, maxX, y + ED_INF_HEIGHT - 1);
  setcolor (kolory.ink) ;
  sprintf(buf, "  %#ld %s",percent,"%") ;
  buf [18] = '\0' ;
  moveto(x+10*WIDTH, y);
  outtext_r (buf) ;
  return;
}

void percentage_out (void)
/*------------------*/
{
  int x,y;

  x = maxX - 20 * WIDTH;
  y = 1;
  moveto(x,y);
  setfillstyle_(SOLID_FILL, BKCOLOR) ;
  bar(x, y, x + 20 * WIDTH + 2, y + ED_INF_HEIGHT - 1);
  
  return;
}

void param_line_out(void)
/*------------------*/
{
	int x, y;
	
	x = 0;
	y = 1;
	moveto(x, y);
	setfillstyle_(SOLID_FILL, BKCOLOR);
	bar(x, y, maxX-1, y + 2 * ED_INF_HEIGHT - 1);

	return;
}



void korekta_obiektow_blokow(void)
/*------------------------------*/
{ BLOK *b, *b1;
  T_Desc_Ex_Block 	*ptrs_desc_bl ;
  LINIA *L;
  LINIA3D *L3D;
  TEXT *t;
  OKRAG *o,*k;
  LUK *l;
  WIELOKAT  *w;
  SPLINE  *s;
  T_Point *ptrs_point ;
  NAGLOWEK *nag;
  B_PCX *b_pcx;

  int i;
  char  *adh;
  BOOL b_break = FALSE;
  double xs, ys ;
  int mode = COPY_PUT ;
  int n_blocks;
  int n_changes, n_changes1, n_changes2;
  char str[60];
  char str1[60];
  BOOL Koniec;
  BOOL To_many_blocks;
  BOOL no_in_block;
  double percent_d;
  int percent, percent0;
  int ret;


 //calculating possible number of blocks
  MAX_N_BLOCKS = dane_size / sizeof(BLOK);
  if (MAX_N_BLOCKS > MAX_MAX_N_BLOCKS) MAX_N_BLOCKS = MAX_MAX_N_BLOCKS;

 char** end_block = malloc(MAX_N_BLOCKS * sizeof(char *));
 char** adr_block = malloc(MAX_N_BLOCKS * sizeof(char *));

 xs = (Xp + Xk) / 2 ;
 ys = (Yp + Yk) / 2 ;

 InfoList(7);


 n_changes=0;
 n_changes1=0;
 n_changes2=0;

 adh=dane;
 n_blocks=0;

 Koniec=FALSE;
 To_many_blocks=FALSE;

 percent0=0;

 while (Koniec==FALSE)
  {
      percent_d=adh-dane;
      percent_d/=dane_size;
      percent_d*=100;
      percent=(int)percent_d;
      if (percent>=percent0)
       {
         percentage(percent0);  
         percent0+=1;
       }


	  if (my_kbhit())
	  {
		  if (my_getch() == ESC)
		  {
			  while (my_kbhit())
			  {
				  my_getch();
			  }
			  free(end_block);
			  free(adr_block);

			  sprintf(str, _SYSTEM_MESSAGE_);
			  sprintf(str1, _INTERRAPTED_);

			  ClearInfo0();

			  ret = ask_question(1, "", "OK", "", str, 12, str1, 11, 1, 0);

			  return;

		  }
	  }

      nag=(NAGLOWEK *)adh;

      for (i=0; i<n_blocks; i++) //sprawdzenie czy blok nie konczy sie w srodku obiektu
        {
         if (adh<end_block[i])
         {
          //sprawdzenie czy aby blok nie jest za dlugi
          if (((long_long)adh+(long_long)(sizeof(NAGLOWEK)+nag->n)) > (long_long)end_block[i])
           {
            n_changes2++;
            b1=(BLOK *)adr_block[i];
            b1->n=(int)((long_long)adh-(long_long)adr_block[i]-sizeof(NAGLOWEK));
			end_block[i] = adh;
           }
         }
        }

    
        switch(nag->obiekt)
        {
         case OdBLOK :
              b=(BLOK *)adh;
              if (b->dlugosc_opisu_obiektu > 1) ptrs_desc_bl = (T_Desc_Ex_Block *)(&b->opis_obiektu [0]) ;
			  ////!////if (b->n > 671744000) b->n=(B3 + b->dlugosc_opisu_obiektu);
				if (b->n < (unsigned int)(B3 + b->dlugosc_opisu_obiektu))
				{
				   b->n=(B3 + b->dlugosc_opisu_obiektu);
				}
              if (b->blok==0)
               {
                //sprawdzenie czy naglowek bloku wskazuje na vertex V
                if (b->kod_obiektu=='V')  //V
                 {
                  for (i=0; i<n_blocks; i++)
                  {
                      if (adr_block[i]>0) {
                          if (adh < end_block[i]) {  //a jednak lezy przynajmniej w jednym bloku bloku
                              b->blok = 1;
                              n_changes1++;
                              break;
                          }
                      }
                  }
                 }
                 else
                 {
                  //sprawdzenie czy aby poprzedzajace bloki nie obejmuja tego obiektu
                  for (i=0; i<n_blocks; i++)
                  {
                      if (adr_block[i]>0) {
                          if (adh < end_block[i]) {  //ograniczenie zakresu bloku
                              n_changes++;
                              b1 = (BLOK *) adr_block[i];
                              b1->n = (int) ((long_long) adh - (long_long) adr_block[i] - sizeof(NAGLOWEK));
                              end_block[i] = adh;
                          }
                          else adr_block[i] = 0;
                      }
                  }
                 }

				 if (b->n == (B3 + b->dlugosc_opisu_obiektu)) 
		 	         b->atrybut = Ausuniety;  //blocks can stay after deleting splines when shadows were created

               }
                else  //sprawdzenie czy aby jednak element nie powinien byc wylaczony z bloku
                 {
                  no_in_block=FALSE;
                  for (i=0; i<n_blocks; i++)
                  {
                      if (adr_block[i]>0) {
                          if (adh < end_block[i]) {  //a jednak jest w bloku
                              no_in_block = TRUE;
                              break;
                          }
                      }
                  }
                  if (no_in_block==FALSE)  //wykluczenie z bloku
                   {
                     b->blok=0;
                     n_changes1++;
                   }
                 }
               if (n_blocks<MAX_N_BLOCKS)
               {
                n_blocks++;
                end_block[n_blocks-1]=(char *)((long_long)adh+(long_long)(sizeof(NAGLOWEK))+ (long_long)b->n);
                adr_block[n_blocks-1]=adh;
                }
			   else
			   {
				   To_many_blocks = TRUE;

				   sprintf(str, _SYSTEM_MESSAGE_);
				   sprintf(str1, _TOO_MANY_BLOCKS_);
				  
				   free(end_block);
				   free(adr_block);
				   ClearInfo0();
				   ret = ask_question(1, "", "OK", "", str, 12, str1, 11, 1, 0);

				   return;
			   }

          adh+=sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
       break;
        default :
            if (nag->obiekt!=Okoniec) {
                if (nag->blok == 0) {  //sprawdzenie czy aby poprzedzajace bloki nie obejmuja tego obiektu
                    for (i = 0; i < n_blocks; i++)
                    {
                        if (adr_block[i]>0) {
                            if (adh < end_block[i]) {  //ograniczenie zakresu bloku
                                n_changes++;
                                b1 = (BLOK *) adr_block[i];
                                b1->n = (int) ((long_long) adh - (long_long) adr_block[i] - sizeof(NAGLOWEK));
                                end_block[i] = adh;
                            } else adr_block[i] = 0;
                        }
                    }
                }
                else  //sprawdzenie czy aby jednak element nie powinien byc wylaczony z bloku
                {
                    no_in_block = FALSE;
                    for (i = 0; i < n_blocks; i++)
                    {
                        if (adr_block[i]>0) {
                            if (adh < end_block[i]) {  //a jednak jest w bloku
                                no_in_block = TRUE;
                                break;
                            }
                            else adr_block[i] = 0;
                        }
                    }
                    if (no_in_block == FALSE)  //wykluczenie z bloku
                    {
                        nag->blok = 0;
                        n_changes1++;
                    }
                }

                adh += sizeof(NAGLOWEK) + nag->n;
            }
            else
            {
                //sprawdzenie czy aby poprzedzajace bloki nie obejmuja tego obiektu
                for (i=0; i<n_blocks; i++)
                {
                    if (adh<end_block[i])
                    {  //ograniczenie zakresu bloku
                        n_changes++;
                        b1=(BLOK *)adr_block[i];
                        b1->n= (int)((long_long)adh-(long_long)adr_block[i]-sizeof(NAGLOWEK));
                        end_block[i] = adh;
                    }
                }
                Koniec=TRUE;
            }
            break;
	} /*switch*/
   if (adh>=(dane+dane_size))  Koniec=TRUE;
  }

  //percentage(percent0);
  percentage_out();

  free(end_block);
  free(adr_block);

  ClearInfo0();

 if (To_many_blocks==TRUE)
  { 

	 sprintf(str, _SYSTEM_MESSAGE_);
	 sprintf(str1, _TOO_MANY_BLOCKS_);

	ret = ask_question(1, "", "OK", "", str, 12, str1, 11, 1, 0);
  }

 if ((n_changes+n_changes1+n_changes2)>0)
  { 

	 sprintf(str, _SYSTEM_MESSAGE_);
	 sprintf(str1, _BLOCKS_CORRECTED_, n_changes + n_changes2, n_changes1);

	ret = ask_question(1, "", "OK", "", str, 12, str1, 11, 1, 0);
    Change=TRUE;
  }
 else
 {

	 sprintf(str, _SYSTEM_MESSAGE_);
	 sprintf(str1, _BLOCKS_TESTED_, n_blocks);

	 ret = ask_question(1, "", "OK", "", str, 12, str1, 11, 1, 0);
 }

}


void par_line(long x1, long y1, long x2, long y2, PLINIA* PL)
/*--------------------------------------------------------*/
{
	double dx, dy;

	dx = (double)(x2 - x1);
	dy = (double)(y2 - y1);
	if (TRUE == Check_if_Equal(dy, 0))
	{
		PL->kat = (dx >= 0 ? 0 : 180);
		PL->cos = (dx >= 0 ? 1 : -1);
		PL->sin = 0;
		PL->dl = fabs(dx);
	}
	else
		if (TRUE == Check_if_Equal(dx, 0))
		{
			PL->kat = (dy >= 0 ? 90 : 270);
			PL->sin = (dy >= 0 ? 1 : -1);
			PL->cos = 0;
			PL->dl = fabs(dy);
		}
		else
		{
			PL->dl = sqrt(dx * dx + dy * dy);
			PL->sin = dy / PL->dl;
			PL->cos = dx / PL->dl;
			PL->kat = (180 / Pi) * Atan2(dy, dx);
			if ((PL->kat) < 0) PL->kat += 360.0;
		}
}

p_point perpend_intersect(long x1, long y1, long x2, long y2, p_point center)
{
	double a, a1, df_min_prec, int_x, int_y;
	PLINIA PL;

	p_point intersection;

	if (x2==x1)
	{
		intersection.x = x1;
		intersection.y = center.y;
	}
	else
		if (y2==y1)
		{
			intersection.y = y1;
			intersection.x = center.x;
		}
		else
		{
			par_line(x1, y1, x2, y2, &PL);

			df_min_prec = Get_Prec(MPREC_DOUBLE, 1, 1);
			if (fabs(PL.sin) < df_min_prec)
			{
				PL.sin = df_min_prec;
			}
			if (fabs(PL.cos) < df_min_prec)
			{
				PL.cos = df_min_prec;
			}
			a = PL.sin / PL.cos;
			a1 = 1 / a;


			int_x = (((double)center.y - (double)y1 + a * (double)x1 + (double)center.x * a1) / (a + a1));
			int_y = ((double)center.y - a1 * (double)(int_x - center.x));

			intersection.x = (long)int_x;
			intersection.y = (long)int_y;
		}
	return intersection;
}


////////////////////////////

#define DIM     2               /* Dimension of points */
typedef int     tPointi[DIM];   /* type integer point */
typedef double  tPointd[DIM];   /* type double point */

#define PMAX    4            /* Max # of pts in polygon */
typedef tPointi tPolygoni[PMAX];/* type integer polygon */

/*
	Returns twice the signed area of the triangle determined by a,b,c,
	positive if a,b,c are oriented ccw, and negative if cw.
*/
int Area2(tPointi a, tPointi b, tPointi c)
{
	return
		(b[0] - a[0]) * (c[1] - a[1]) -
		(c[0] - a[0]) * (b[1] - a[1]);
}

/*
	Returns three times the centroid.  The factor of 3 is
	left in to permit division to be avoided until later.
*/
void Centroid3(tPointi p1, tPointi p2, tPointi p3, tPointi c)
{
	c[0] = p1[0] + p2[0] + p3[0];
	c[1] = p1[1] + p2[1] + p3[1];
	return;
}

/*
	Returns the cg in CG.  Computes the weighted sum of
	each triangle's area times its centroid.  Twice area
	and three times centroid is used to avoid division
	until the last moment.
*/
void FindCG(int n, tPolygoni P, tPointd CG)
{
	int     i;
	double  A2, Areasum2 = 0;        /* Partial area sum */
	tPointi Cent3;

	CG[0] = 0;
	CG[1] = 0;
	for (i = 1; i < n - 1; i++) {
		Centroid3(P[0], P[i], P[i + 1], Cent3);
		A2 = Area2(P[0], P[i], P[i + 1]);
		CG[0] += A2 * Cent3[0];
		CG[1] += A2 * Cent3[1];
		Areasum2 += A2;
	}
	if (Areasum2 == 0)
	{
		A2 = 0;
		for (i = 0; i < n; i++) A2 += P[i][0];
		CG[0] = A2 / n;
		A2 = 0;
		for (i = 0; i < n; i++) A2 += P[i][1];
		CG[1] = A2 / n;
		return;
	}
	CG[0] /= 3 * Areasum2;
	CG[1] /= 3 * Areasum2;
	return;
}


p_point compute2DPolygonCentroid2(T_PixelTVal* vertices, int vertexCount)
{
	tPointd CG;
	p_point centroid;
	tPolygoni P;

	P[0][0] = vertices[0];
	P[0][1] = vertices[1];
	P[1][0] = vertices[2];
	P[1][1] = vertices[3];
	P[2][0] = vertices[4];
	P[2][1] = vertices[5];
	P[3][0] = vertices[6];
	P[3][1] = vertices[7];

	FindCG(4, P, &CG);
	centroid.x = CG[0];
	centroid.y = CG[1];
	return centroid;
}


///////////////////////////

p_point compute2DPolygonCentroid(T_PixelTVal* vertices, int vertexCount)
{
	p_point centroid = { 0, 0 };
	double signedArea = 0.0;
	double x0 = 0.0; // Current vertex X
	double y0 = 0.0; // Current vertex Y
	double x1 = 0.0; // Next vertex X
	double y1 = 0.0; // Next vertex Y
	double a = 0.0;  // Partial signed area

	// For all vertices
	int i = 0;
	for (i = 0; i < (vertexCount/2); ++i)
	{
		x0 = vertices[i*2];
		y0 = vertices[i*2+1];
		x1 = vertices[(i*2 + 2) % vertexCount];
		y1 = vertices[(i*2 + 3) % vertexCount];
		a = x0 * y1 - x1 * y0;
		signedArea += a;
		centroid.x += (x0 + x1) * a;
		centroid.y += (y0 + y1) * a;
	}

	signedArea *= 0.5;
	centroid.x /= (6.0 * signedArea);
	centroid.y /= (6.0 * signedArea);

	return centroid;
}


int compare_vectors(T_PixelTVal* vector0, p_vector* vector2)
{
	if (((vector0[0] == vector2->x1) && (vector0[1] == vector2->y1) && (vector0[2] == vector2->x2) && (vector0[3] == vector2->y2)) ||
		((vector0[0] == vector2->x2) && (vector0[1] == vector2->y2) && (vector0[2] == vector2->x1) && (vector0[3] == vector2->y1))) return 0;
	
	else if (((vector0[2] == vector2->x1) && (vector0[3] == vector2->y1) && (vector0[4] == vector2->x2) && (vector0[5] == vector2->y2)) ||
		((vector0[2] == vector2->x2) && (vector0[3] == vector2->y2) && (vector0[4] == vector2->x1) && (vector0[5] == vector2->y1))) return 0;

	else if (((vector0[4] == vector2->x1) && (vector0[5] == vector2->y1) && (vector0[6] == vector2->x2) && (vector0[7] == vector2->y2)) ||
		((vector0[4] == vector2->x2) && (vector0[5] == vector2->y2) && (vector0[6] == vector2->x1) && (vector0[7] == vector2->y1))) return 0;

	else if (((vector0[6] == vector2->x1) && (vector0[7] == vector2->y1) && (vector0[0] == vector2->x2) && (vector0[1] == vector2->y2)) ||
		((vector0[6] == vector2->x2) && (vector0[7] == vector2->y2) && (vector0[0] == vector2->x1) && (vector0[1] == vector2->y1))) return 0;

	else return 1;
}

BOOL move_vector_inside(T_PixelTVal *PolyPoints, int v_number, int flag)
{
	p_point center, vector_middle, vector_middle1;

	int dx, dy, fx, fy;

	center = compute2DPolygonCentroid2(PolyPoints, 8);

	switch (v_number)
	{
	case 1: //first vector
		vector_middle = perpend_intersect(PolyPoints[0], PolyPoints[1], PolyPoints[2], PolyPoints[3], center);
		dx = center.x - vector_middle.x;
		dy = center.y - vector_middle.y;

		if ((dx == 0) && (dy == 0))
			return FALSE;

		if (dy > 0) fy = 1;
		else if (dy < 0) fy = -1;
		else fy = 0;

		if (dx > 0) fx = 1;
		else if (dx < 0) fx = -1;
		else fx = 0;

		if ((PolyPoints[6] == PolyPoints[0]) &&
			(PolyPoints[7] == PolyPoints[0]))
			{
				if (flag)
				{
					PolyPoints[6] += fx;
				}
				PolyPoints[7] += fy;
			}

		if (flag)
		{
			PolyPoints[0] += fx;
			PolyPoints[2] += fx;
		}

		PolyPoints[1] += fy;
		PolyPoints[3] += fy;


		break; 
	case 2: //second vector
		vector_middle = perpend_intersect(PolyPoints[2], PolyPoints[3], PolyPoints[4], PolyPoints[5], center);
		dx = center.x - vector_middle.x;
		dy = center.y - vector_middle.y;

		if ((dx == 0) && (dy == 0))
			return FALSE;

		if (dy > 0) fy = 1;
		else if (dy < 0) fy = -1;
		else fy = 0;

		if (dx > 0) fx = 1;
		else if (dx < 0) fx = -1;
		else fx = 0;

		if (flag)
		{
			PolyPoints[2] += fx;
			PolyPoints[4] += fx;
		}

		PolyPoints[3] += fy;
		PolyPoints[5] += fy;

		break;
	case 3: //third vector
		vector_middle = perpend_intersect(PolyPoints[4], PolyPoints[5], PolyPoints[6], PolyPoints[7], center);
		dx = center.x - vector_middle.x;
		dy = center.y - vector_middle.y;

		if ((dx == 0) && (dy == 0))
			return FALSE;

		if (dy > 0) fy = 1;
		else if (dy < 0) fy = -1;
		else fy = 0;

		if (dx > 0) fx = 1;
		else if (dx < 0) fx = -1;
		else fx = 0;

		if (flag)
		{
			PolyPoints[4] += fx;
			PolyPoints[6] += fx;
		}

		PolyPoints[5] += fy;
		PolyPoints[7] += fy;

		break;
	case 4:  //last vector
		vector_middle = perpend_intersect(PolyPoints[6], PolyPoints[7], PolyPoints[0], PolyPoints[1], center);

		dx = center.x - vector_middle.x;
		dy = center.y - vector_middle.y;

		if ((dx == 0) && (dy == 0))
		{
			//check if 1 pxl high
			return FALSE;
		}

		if (dy > 0) fy = 1;
		else if (dy < 0) fy = -1;
		else fy = 0;

		if (dx > 0) fx = 1;
		else if (dx < 0) fx = -1;
		else fx = 0;

		if ((PolyPoints[4] == PolyPoints[6]) &&
			(PolyPoints[5] == PolyPoints[7]))
		{
			if (flag)
			{
				PolyPoints[4] += fx;
			}
			PolyPoints[5] += fy;
		}

		if (flag)
		{
			PolyPoints[6] += fx;
			PolyPoints[0] += fx;
		}

		PolyPoints[7] += fy;
		PolyPoints[1] += fy;
		break;
	default:
		break;
	}

	return TRUE;

}


void reset_last_polygon_vectors_e(void)
{
	int i,j;

	first_polygon_vector = (p_vector){ 0 };
	last_polygon_vector = (p_vector){ 0 };
	for (j = 0; j < LAST_POLYGONS_NO; j++)
	{
		for (i = 0; i < 8; i++) last_polygon[j][i] = 0;
	}
}

typedef struct ALLEGRO_COLOR ALLEGRO_COLOR;

#ifndef ALLEGRO5
struct ALLEGRO_COLOR
{
	float r, g, b, a;
};
#endif

void select_color_type(char *ad)
{
    NAGLOWEK *nag;
    LINIA *L;
    nag = (NAGLOWEK *) ad;
    int grubosc;

    if ((nag->obiekt != OdBLOK) && (nag->obiekt != Opcx))
    {
        L=(LINIA *)nag;
        linestyle(L->typ);
        if ((L->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
        {
            if ((L->atrybut == Ablok) ||
            ((dragging_quad_move==TRUE) && (L->atrybut == Aoblok)))
            {
                setcolor(kolory.blok);
            } else
            {
                SetColorAC(L->kolor);
            }
        }
        else
        {
            SetColorAC(8);
        }
   }
}

void select_color_paper_type(char *ad)
{
    NAGLOWEK *nag;
    LINIA *L;
    nag = (NAGLOWEK *) ad;
    int grubosc;

    if ((nag->obiekt != OdBLOK) && (nag->obiekt != Opcx))
    {
        L=(LINIA *)nag;
        linestyle(L->typ);

        SetColorAC(kolory.paper);

    }
}

static void rysuj_obiekty(void)
/*---------------------------*/
{
  BLOK *b1;
  LINIA *L, *L1;
  LINIA3D *L3D;
  TEXT *t;
  OKRAG *o,*k;
  LUK *l;
  WIELOKAT  *w, *w1;
  SPLINE  *s;
  T_Point *ptrs_point ;
  AVECTOR *ptrs_vector ;
  AVECTOR V=Vdef;
  NAGLOWEK *nag, *nag1;
  B_PCX *b_pcx;
  ELLIPSE *e, *fe;
  ELLIPTICALARC *ea;
  SOLIDARC *sa, *sa1;
  T_PixelTVal PolyPoints[MaxNumPoints] ;
  int PolyPointsMon[8] ;
  T_Float PolyPoints_Z[MaxNumPoints_Z] ;
  int NumPoints,i,j;
  char  *adh;
  BOOL b_break = FALSE, b_info_break ;
  double xs, ys ;
  int mode = COPY_PUT ;
  int grubosc;
  BOOL bitmap_exist_o;
  BOOL bitmap_png_exist_o;
  BOOL bitmap_vector_exist_o;
  BOOL bitmap_pattern_exist_o;
  char *end_block;
  int n_changes;
  int licznik_obiektow;
  int _WhNumberTextStyle_;
  long_long pattern_add_limit = 0;
  //long_long
  char *trace_block_begin = NULL, *trace_block_end=NULL;
  BOOL trace_block=FALSE;
  POINTF last_trace_point[2], next_trace_point[2];
  char *translucency_ptr, *gradient_ptr=NULL;
  unsigned char translucency;
  //GRADIENT gradient;
  int found;
  BOOL ignore_p=FALSE;
  BOOL ret_move;
  int ii;

  int li;
  long_long l_abd;
  char* adbe;
  float hatch_points[4096];  //1024
  int h_kolor, h_layer, kolor256;
  ALLEGRO_COLOR al_kolor;
  int vertex_counts[1024] = { 0 }; //max number of polygones //TEMPORARY
  int gr;
  int kolor;
  QUAD t_outline;
  int ret;
  LINIA L_tmp, L_tmp1;
  LUK l_tmp, l_tmp1;
  ELLIPTICALARC ea_tmp, ea_tmp1;
  int kolor1;

/*    //!!!!!!!!! TEMPORARY OFF

 licz_redraw=(dane_size / 100000) + 1;
 if (licznik_redraw>licz_redraw)
  {
    korekta_obiektow_blokow();  ///////!!!!!!!!!!
    licznik_redraw=0;
  }

  licznik_redraw++;
*/


reset_dane0();

_WhNumberTextStyle_=get_WhNumberTextStyle();

 licznik_obiektow=0;

 reset_last_polygon_vectors_e();

 xs = (Xp + Xk) / 2 ;
 ys = (Yp + Yk) / 2 ;

 n_changes=0;
 end_block=0;

 adh=dane;
 adh+=dane_size;
 oknoS(Xp,Yp,Xk,Yk);

 b_info_break = FALSE ;
 if (dane_size > 1000000L)
 {
    b_info_break = TRUE ;
	komunikat0(137);
 }

 bitmap_exist_o=bitmap_exist || bitmap_pattern_exist || bitmap_pattern_exist;
 bitmap_png_exist_o = bitmap_png_exist || bitmap_exist || bitmap_pattern_exist;
 bitmap_vector_exist_o = bitmap_vector_exist;
 bitmap_pattern_exist_o = bitmap_pattern_exist || bitmap_exist || bitmap_png_exist;
 bitmap_exist = FALSE;
 bitmap_png_exist = FALSE;
 bitmap_vector_exist = FALSE;
 bitmap_pattern_exist = FALSE;
 solid_translucent_exist = FALSE;
 bitmap_on_front_exist = FALSE;

 okno_r_second();

 setwritemode (mode);
 end_block=0;
 pattern_add_limit = 0;

 int i_ob[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

 obiekt_tok ((char *)dane, adh, (char **) &nag, ONieOkreslony);
 while ((nag!=NULL) && ((char *)nag<adh))
  {
	 i_ob[nag->obiekt]++;

   if (FALSE == Test_Object ((void*)nag)) 
   {
     goto aaa ;
   }
    
    if (nag->atrybut!=Ablok || BlokM==0 || (char *)nag < ADP || (char *)nag > ADK )
     {
		if (no_break == FALSE && b_break == FALSE)
		{
			if (my_kbhit())
			{

					licznik_obiektow = 0;
					if (my_getch() == ESC)
					{
						while (my_kbhit())
						{
							my_getch();
						}
						b_break = TRUE;
						bitmap_exist = bitmap_exist_o;
						bitmap_png_exist = bitmap_png_exist_o;
                        bitmap_vector_exist = bitmap_vector_exist_o;
						bitmap_pattern_exist = bitmap_pattern_exist_o;
					}

			}
		}
       if ( b_break == TRUE)
	{
	  nag->widoczny = 0;
	  goto aaa;
	}
       switch(nag->obiekt)
       {
     case OdBLOK :
       b1=(BLOK *)nag;

	   if (((b1->kod_obiektu == B_PLINE) &&
		   ((b1->opis_obiektu[0]== PL_PLINE) ||
		   (b1->opis_obiektu[0] == PL_ELLIPSE) ||
		   (b1->opis_obiektu[0] == PL_SKETCH) ||
		   (b1->opis_obiektu[0] == PL_CURVE) ||
           //(b1->opis_obiektu[0] == PL_TRACE) ||
		   (b1->opis_obiektu[0] == PL_ELLIPSE_ARC))) ||
		   (b1->kod_obiektu == PL_HATCH)  //???
		   )
	   {
		   pattern_count = TRUE;
		   pattern_offset = 0;
		   pattern_add_limit = (long_long)(nag) + b1->n - 1;
	   }

       if ((b1->kod_obiektu == B_PLINE) && (b1->opis_obiektu[0]== PL_TRACE))
       {
           trace_block_begin=(char*)nag + sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu;
           trace_block_end=(char*)nag + sizeof(NAGLOWEK) + b1->n - 1;
           trace_block=TRUE;
       }
       else trace_block=FALSE;

	   first_polygon_vector = (p_vector){ 0 };
	   last_polygon_vector = (p_vector){ 0 };
	   for (j = 0; j < LAST_POLYGONS_NO; j++)
	   {
		   for (i = 0; i < 8; i++) last_polygon[j][i] = 0;
	   }
	   //TEST
#ifdef GO_TRIANGULATION
	   if (b1->kod_obiektu == PL_HATCH)
	   {
		   //searching for borders
		   l_abd = (long_long)b1 - (long_long)dane;

		   adbe = (long_long)b1 + (long)(sizeof(NAGLOWEK) + b1->n - 1);
		   li = get_points_hatch_outline(b1, adbe, &hatch_points, &vertex_counts, &h_layer, &h_kolor);

		   kolor256 = GetColorAC(h_kolor);

		   al_kolor.r = _dac_normal[kolor256][0] << 2;
		   al_kolor.g = _dac_normal[kolor256][1] << 2;
		   al_kolor.b = _dac_normal[kolor256][2] << 2;
		   al_kolor.a = 255 * 0.05; // TRANSLUCENCY;


		   if ((h_layer == Current_Layer) || (options1.view_only_current_layer == 0)) setfillstyle_(SOLID_FILL, GetColorAC(h_kolor));
		   else setfillstyle_(SOLID_FILL, GetColorAC(8));

		   //if (translucency > TRANSLUCENCY)
			 translucency = TRANSLUCENCY;
		   set_trans_blender(0, 0, 0, (int)translucency);
		   //set_trans_blender(al_kolor.r, al_kolor.g, al_kolor.b, translucency);
		   //set_difference_blender(al_kolor.r, al_kolor.g, al_kolor.b, translucency);
		   set_mode_trans();

		   al_draw_filled_polygon_with_holes(&hatch_points, &vertex_counts, al_kolor, translucency, kolory.paper);

		   //my_fillpoly(li/2, hatch_points, translucency, kolory.paper);

		   set_mode_solid();
	   }
#endif
	    //

       break;
	  case Olinia :
 		licznik_obiektow++;
	    L=(LINIA *)nag;
 		if ((L->typ == HATCH_OUTLINE_TYPE) && (Get_Point_View() == FALSE)) break;
        select_color_type(L);

        int typl = (L->typ & 31);
        if (typl==0) pattern_offset = 0;


                ////TEMPORARY
               if ((L->n + sizeof(NAGLOWEK)) !=  sizeof (LINIA) )
               {
                   GRAPH_DATA *graph_data=(char*)L + sizeof(LINIA);
                   int flags = graph_data->flags;
                   int nx = graph_data->nx;
               }

		//checking if line is dimensioning line
		if ((L->blok == 1) && (L->obiektt1 == 0) && (L->obiektt2 == 1) && (L->obiektt3 == 0))
		{
			//check if next object is text or line then text
			nag1 = (char*)nag + sizeof(NAGLOWEK) + L->n;
			if (nag1->obiekt == Otekst)
			{
				t = (TEXT*)nag1;
				if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
				{
                    //checking crossing with text
                    //for (i=0; i<4; i++)
                    //    ret=lineC(pikseleX0(t_outline.xy[i*2]), pikseleY0(t_outline.xy[i*2+1]), pikseleX0(t_outline.xy[(i*2+2)%8]), pikseleY0(t_outline.xy[(i*2+3)%8]));
                    //checking crossing with text
                    outlinetext(t, &t_outline, 0.5);
                    Normalize_Quad (&t_outline);
                    //for (i=0; i<4; i++)
                    //    ret=lineC(pikseleX0(t_outline.xy[i*2]), pikseleY0(t_outline.xy[i*2+1]), pikseleX0(t_outline.xy[(i*2+2)%8]), pikseleY0(t_outline.xy[(i*2+3)%8]));
                    ret=trim_line_to_quad(L, &t_outline, &L_tmp, &L_tmp1);
                    ////
                    switch (ret)
                    {
                        case 0:
                            L->widoczny = lineC(pikseleX0(L->x1), pikseleY0(L->y1), pikseleX0(L->x2),pikseleY0(L->y2));
                            break;
                        case 1: L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                            break;
                        case 2: L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                            L->widoczny = lineC(pikseleX0(L_tmp1.x1), pikseleY0(L_tmp1.y1), pikseleX0(L_tmp1.x2),pikseleY0(L_tmp1.y2));
                            break;
                        default:
                            break;
                    }
                    break;
					//L->widoczny = lineC(pikseleX0(L->x1), pikseleY0(L->y1), pikseleX0(L->x2), pikseleY0(L->y2));
				}
			}
			else if (nag1->obiekt == Olinia)
			{
				nag1 = (char*)nag1 + sizeof(NAGLOWEK) + nag1->n;
				if (nag1->obiekt == Otekst)
				{
					t = (TEXT*)nag1;
					if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
					{
                        //checking crossing with text
                        outlinetext(t, &t_outline, 0.5);
                        Normalize_Quad (&t_outline);
                        //for (i=0; i<4; i++)
                        //    ret=lineC(pikseleX0(t_outline.xy[i*2]), pikseleY0(t_outline.xy[i*2+1]), pikseleX0(t_outline.xy[(i*2+2)%8]), pikseleY0(t_outline.xy[(i*2+3)%8]));
                        ret=trim_line_to_quad(L, &t_outline, &L_tmp, &L_tmp1);
                        ////
                        switch (ret)
                        {
                            case 0:
                                L->widoczny = lineC(pikseleX0(L->x1), pikseleY0(L->y1), pikseleX0(L->x2),pikseleY0(L->y2));
                                break;
                            case 1: L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                            break;
                            case 2: L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                                    L->widoczny = lineC(pikseleX0(L_tmp1.x1), pikseleY0(L_tmp1.y1), pikseleX0(L_tmp1.x2),pikseleY0(L_tmp1.y2));
                                break;
                            default:
                                break;
                        }
						break;
					}
				}
				L->widoczny = lineC(pikseleX0(L->x1), pikseleY0(L->y1), pikseleX0(L->x2), pikseleY0(L->y2));
				break;
			}
		}
        //L->widoczny=lineC (pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L->x2),pikseleY0(L->y2));
        switch (L->obiektt2) {
            case 4:
                memmove(&V, L, sizeof(LINIA));
                if (L->obiektt3==0)  V.style = V_EDGE_ROLL;
                else V.style = V_EDGE_ROLL_INV;
                if ((L->widoczny=(linia_wybrana(L)) ? 1:0))
                    Draw_Vector(&V, COPY_PUT, 1, 0);
                break;
           case 6:
               memmove(&V, L, sizeof(LINIA));
                if (L->obiektt3==0)  V.style = V_EDGE_SIMPLE;
                else V.style = V_EDGE_SIMPLE_INV;
               if ((L->widoczny=(linia_wybrana(L)) ? 1:0))
                   Draw_Vector(&V, COPY_PUT, 1, 0);
               break;
           case 7:
               memmove(&V, L, sizeof(LINIA));
                if (L->obiektt3==0)  V.style = V_EDGE_FIXED;
                else V.style = V_EDGE_FIXED_INV;
               if ((L->widoczny=(linia_wybrana(L)) ? 1:0))
                   Draw_Vector(&V, COPY_PUT, 1, 0);
               break;
           default:
               L->widoczny=lineC (pikseleX0(L->x1),pikseleY0(L->y1),pikseleX0(L->x2),pikseleY0(L->y2));
               break;
       }
	    break;
	  case Otekst :
		licznik_obiektow++;
	    t=(TEXT*)nag;

		if (t->czcionka >= (unsigned int)_WhNumberTextStyle_)
		{
          t->czcionka=0 ;	/*zmiana czcionki na domyslna*/
		}
	    if(!(t->widoczny=tekst_wybrany(t))) break;
        if ((t->warstwa==Current_Layer) || (options1.view_only_current_layer==0)) {
            if ((t->atrybut == Ablok) ||
                ((dragging_quad_move == TRUE) && (t->atrybut == Aoblok)))
                outtextxy_w(t, XOR_PUT);
            else outtextxy_w(t, COPY_PUT);
        }
        else
        {
            outtextxy_w(t, XOR_PUT);
        }
	    break;
	  case Okolo :
	   licznik_obiektow+=2;
	   k=(OKRAG*)nag;

       if ((k->atrybut == Abad) || (k->atrybut == Ausuniety)) break;

	   if (!(k->widoczny = Check_Draw_Pieslice (k)))  break ;

	   setlinestyle1(SOLID_LINE,0,NORM_WIDTH);

      if ((k->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
        {
            if ((k->atrybut == Ablok) ||
            ((dragging_quad_move==TRUE) && (k->atrybut == Aoblok)))
            {
                setfillstyle_(SOLID_FILL,kolory.blok);
                setcolor(kolory.blok);
            } else {
                setfillstyle_(SOLID_FILL, GetColorAC(k->kolor));
                SetColorAC(k->kolor);
            }
        }
          else
           {
            setfillstyle_(SOLID_FILL,GetColorAC(8));
            SetColorAC(8);
           }
       set_mode_trans();

	   Draw_Kolo((long_long)pikseleX0(k->x),(long_long)pikseleY0(k->y),(long_long)pikseleDX(k->r));

	   set_mode_solid();

	   break;
	 case Ookrag :
	   licznik_obiektow+=2;
	   o=(OKRAG*)nag;
	   if ((o->typ == HATCH_OUTLINE_TYPE) && (Get_Point_View() == FALSE)) break;
	   if(!(o->widoczny=okrag_wybrany(o))) break;
       select_color_type(o);
	   pattern_count = TRUE;
	   pattern_offset = 0;
	   DrawCircle(pikseleX0(o->x),pikseleY0(o->y),pikseleDX(o->r), mode);
	   pattern_count = FALSE;
	   pattern_offset = 0;
	   break;

   case Oellipse :
       licznik_obiektow+=2;
       e=(ELLIPSE*)nag;
       if(!(e->widoczny=elipsa_wybrana_prec(e))) break;

       select_color_type(e);

       if ((e->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
       {
           if ((e->atrybut == Ablok) ||
               ((dragging_quad_move==TRUE) && (e->atrybut == Aoblok)))
           {
               kolor1=kolory.blok;
           } else {
               kolor1=e->kolor;
           }
       }
       else
       {
           setfillstyle_(SOLID_FILL,GetColorAC(8));
           kolor1=8;
       }

       SetColorAC(kolor1);

       DrawEllipse(e, mode, kolor1, 1);
       break;
   case Ofilledellipse:
       licznik_obiektow+=2;
       fe=(ELLIPSE*)nag;

       if ((fe->atrybut == Abad) || (fe->atrybut == Ausuniety)) break;

       if(!(fe->widoczny=elipsa_wybrana(fe))) break;

       setlinestyle1(SOLID_LINE,0,NORM_WIDTH);

       if ((fe->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
       {
           if ((fe->atrybut == Ablok) ||
               ((dragging_quad_move==TRUE) && (fe->atrybut == Aoblok)))
           {
               setfillstyle_(SOLID_FILL,kolory.blok);
               kolor1=kolory.blok;
           } else {
               setfillstyle_(SOLID_FILL, GetColorAC(fe->kolor));
               kolor1=fe->kolor;
           }
       }
       else
       {
           setfillstyle_(SOLID_FILL,GetColorAC(8));
           SetColorAC(8);
       }

       SetColorAC(kolor1);

       set_mode_trans();

       DrawFilledEllipse(fe, mode, kolor1, 1);

       set_mode_solid();
       break;
       case Oellipticalarc :
               licznik_obiektow+=2;
               ea=(ELLIPTICALARC*)nag;
               if(!(ea->widoczny=lukeliptyczny_wybrany(ea))) break;

               select_color_type((char*)ea);

               if ((ea->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
               {
                   if ((ea->atrybut == Ablok) ||
                       ((dragging_quad_move==TRUE) && (ea->atrybut == Aoblok)))
                   {
                       kolor1=kolory.blok;
                   } else {
                       kolor1=ea->kolor;
                   }
               }
               else
               {
                   setfillstyle_(SOLID_FILL,GetColorAC(8));
                   kolor1=8;
               }

               SetColorAC(kolor1);
               //checking if arc is dimensioning arc
               if ((ea->blok == 1) && (ea->obiektt1 == 0) && (ea->obiektt2 == 1) && (ea->obiektt3 == 0))
               {
                   //check if next object is text or line then text
                   nag1 = (char*)nag + sizeof(NAGLOWEK) + ea->n;
                   if (nag1->obiekt == Otekst)
                   {
                       t = (TEXT*)nag1;
                       if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
                       {
                           //checking crossing with text
                           outlinetext(t, &t_outline, 0.5);
                           Normalize_Quad (&t_outline);
                           ret=trim_ellipticalarc_to_quad(ea, &t_outline, &ea_tmp, &ea_tmp1);
                           ////
                           switch (ret)
                           {
                               case 0:
                                   DrawEllipticalArc(ea, mode, kolor1, 1);
                                   break;
                               case 1:
                                   DrawEllipticalArc(&ea_tmp, mode, kolor1, 1);
                                   break;
                               case 2:
                                   DrawEllipticalArc(&ea_tmp, mode, kolor1, 1);
                                   DrawEllipticalArc(&ea_tmp1, mode, kolor1, 1);
                                   break;
                               default:
                                   break;
                           }
                           break;
                       }
                   }
                   else if (nag1->obiekt == Olinia)
                   {
                       nag1 = (char*)nag1 + sizeof(NAGLOWEK) + nag1->n;
                       if (nag1->obiekt == Olinia)
                           nag1 = (char*)nag1 + sizeof(NAGLOWEK) + nag1->n;
                       if (nag1->obiekt == Otekst)
                       {
                           t = (TEXT*)nag1;
                           if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
                           {
                               //checking crossing with text
                               outlinetext(t, &t_outline, 0.5);
                               Normalize_Quad (&t_outline);
                               ret=trim_ellipticalarc_to_quad(ea, &t_outline, &ea_tmp, &ea_tmp1);
                               ////
                               switch (ret)
                               {
                                   case 0:
                                       DrawEllipticalArc(ea, mode, kolor1, 1);
                                       break;
                                   case 1:
                                       DrawEllipticalArc(&ea_tmp, mode, kolor1, 1);
                                       break;
                                   case 2:
                                       DrawEllipticalArc(&ea_tmp, mode, kolor1, 1);
                                       DrawEllipticalArc(&ea_tmp1, mode, kolor1, 1);
                                       break;
                                   default:
                                       break;
                               }
                               break;
                           }
                       }
                       DrawEllipticalArc(ea, mode, kolor1, 1);
                       break;
                   }
               }
           DrawEllipticalArc(ea, mode, kolor1, 1);
     break;

     case Osolidarc:
         licznik_obiektow+=2;

         sa=(SOLIDARC*)nag;

           if ((sa->atrybut != Ausuniety) &&
               (sa->atrybut != Abad))
           {
               if ((sa->empty_typ == 0) && (sa->pattern == 1))
               {
                   //checking if it's really pattern
                   if (strlen(sa->patternname)==0)
                       sa->pattern=0;
                   else {
                       if (bitmap_pattern_exist_o == FALSE) {
                           redraw_again = TRUE;
                           bitmap_pattern_exist = TRUE;
                           return;
                       }
                       bitmap_pattern_exist = TRUE;
                       break;
                   }
               }
           }
           else break;

           BOOL begin_line=1, end_line=1;

           if ((sa->empty_typ>0) && (sa->empty_typ<6) && (trace_block==TRUE) && (((char*)sa+sa->n+sizeof(NAGLOWEK)-1)<=trace_block_end))  //is outlined and can be the last one
           {
               double xy[8];
               get_solidarc_ends(sa, xy);

               if (((char*)sa+sa->n+sizeof(NAGLOWEK))>trace_block_begin)  //this is not first element in polyline
               {
                   //checking the last coordinates
                   if ((Check_if_Equal(xy[6],last_trace_point[0].x)) &&
                       (Check_if_Equal(xy[7],last_trace_point[0].y)) &&
                       (Check_if_Equal(xy[4],last_trace_point[1].x)) &&
                       (Check_if_Equal(xy[5],last_trace_point[1].y)))
                   {
                       if (!sa->reversed) end_line=0;
                       else begin_line=0;
                   }
               }

               nag1=(NAGLOWEK*)((char*)sa+sizeof(NAGLOWEK)+sa->n);

               if (nag1->obiekt==Owwielokat)
               {
                   w1=(WIELOKAT*)nag1;
                   if ((Check_if_Equal(w1->xy[2], xy[2])) &&
                       (Check_if_Equal(w1->xy[3], xy[3])) &&
                       (Check_if_Equal(w1->xy[0], xy[0])) &&
                       (Check_if_Equal(w1->xy[1], xy[1])))
                       if (!sa->reversed) begin_line=0;
                       else end_line=0;
               }
               else if (nag1->obiekt==Osolidarc)
               {
                   double xy1[8];
                   sa1=(SOLIDARC*)nag1;
                   get_solidarc_ends(sa1, xy1);

                   if ((Check_if_Equal(xy1[4], xy[2])) &&
                       (Check_if_Equal(xy1[5], xy[3])) &&
                       (Check_if_Equal(xy1[6], xy[0])) &&
                       (Check_if_Equal(xy1[7], xy[1])))
                       if (!sa->reversed) begin_line=0;
                       else end_line=0;
               }

               last_trace_point[0].x=xy[0];
               last_trace_point[0].y=xy[1];
               last_trace_point[1].x=xy[2];
               last_trace_point[1].y=xy[3];
           }

         rysuj_solidarc_(sa, mode, 1, TRUE, FALSE, begin_line, end_line);

     break;

	 case Owwielokat :
	   licznik_obiektow+=2;
	   w=(WIELOKAT*)nag;

       gradient_ptr=NULL;

	   SetColorAC(w->kolor);

	   if ((w->atrybut != Ausuniety) &&
		   (w->atrybut != Abad))
	   {
		   if ((w->empty_typ == 0) && ((w->pattern == 1) || (w->gradient == 1)))
		   {
               //checking if it's really pattern
               if (w->n == (w->lp*sizeof(float)+8))
               {
                   w->pattern = 0;
                   w->gradient = 0;
               }
               else {
                   if (bitmap_pattern_exist_o == FALSE) {
                       redraw_again = TRUE;
                       bitmap_pattern_exist = TRUE;
                       return;
                   }
                   bitmap_pattern_exist = TRUE;
                   break;
               }
		   }
		   else {
               if ((w->empty_typ == 0) && (w->translucent == 1)) {
                   if (w->n == (w->lp * sizeof(float) + 8))
                       w->translucent = 0;

                   if (w->translucent == 1) {
                       translucency_ptr = (char *) w->xy;
                       translucency_ptr += (w->lp * sizeof(float));
                       memmove(&translucency, translucency_ptr, sizeof(unsigned char));
                       if (translucency < 255) solid_translucent_exist = TRUE;
                   }
               }
               //GRADIENT  - this part now is ommited due to moving to rysuj_bitmapy();
               if ((w->empty_typ == 0) && (w->gradient == 1)) {
                   if (w->n == (w->lp * sizeof(float) + 8))
                       w->gradient = 0;

                   if (w->gradient == 1) {

                       translucency_ptr = (char*)w->xy;
                       translucency_ptr += (w->lp * sizeof(float));

                       //gradient_ptr = (char *) w->xy;
                       //gradient_ptr += (w->lp * sizeof(float));
                       gradient_ptr=translucency_ptr+sizeof(unsigned char);
                       //memmove(&gradient, gradient_ptr, sizeof(GRADIENT));
                       solid_translucent_exist = TRUE;
                   }
               }
           }
	   }
	   else break;

	   if(!(w->widoczny=wielokat_wybrany(w)))
	   {
	     if (TRUE == Point_in_Solid (w, xs, ys))
	     {
         if ((w->empty_typ==0) && (w->pattern == 0))
          {

			 translucency = 255;

           if ((w->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
           {
               if ((w->atrybut == Ablok) ||
               ((dragging_quad_move == TRUE) && (w->atrybut == Aoblok)))
               {
                   setfillstyle_(SOLID_FILL,kolory.blok);
                   setcolor(kolory.blok);
               } else {
                   setfillstyle_(SOLID_FILL, GetColorAC(w->kolor));
                   SetColorAC(w->kolor);
               }
           }
             else setfillstyle_ (SOLID_FILL, GetColorAC (8)) ;


		   if ((w->translucent == 1) || (TRANSLUCENCY<255))
		   {
			   if (w->translucent == 1)
			   {
				   translucency_ptr = w->xy;
				   translucency_ptr += (w->lp * sizeof(float));
				   memmove(&translucency, translucency_ptr, sizeof(unsigned char));

			   }
			   else translucency = 255;
			   if (translucency > TRANSLUCENCY) translucency = TRANSLUCENCY;
			   set_trans_blender(0, 0, 0, (int)translucency);
			   set_mode_trans();
		   }
		   else set_mode_solid();

		   PolyPointsMon[0] = pXp; PolyPointsMon[1] = /*pYk*/ 0; PolyPointsMon[2] = pXk; PolyPointsMon[3] = /*pYk*/ 0;
		   PolyPointsMon[4] = pXk; PolyPointsMon[5] = pYp; PolyPointsMon[6] = pXp; PolyPointsMon[7] = pYp;

           if (!w->gradient) my_fillpoly(4, PolyPointsMon, translucency, kolory.paper);
           else {
               NumPoints=w->lp/2;
               Draw_Solid(NumPoints, PolyPoints, w->pcx_solid, w->obiektt3, pikseleX0(0), pikseleY0(0), NULL, translucency, (GRADIENT4*)gradient_ptr);
           }


          if (w->translucent == 1)
          {
              reset_trans_blender();
          }

		   set_mode_solid();

		   break;

          }
          else if (w->empty_typ==6)  //obszar wykasowany z wydruku
          {
           if ((w->warstwa==Current_Layer) || (options1.view_only_current_layer==0)) setfillstyle_ (MY_DOT_FILL, GetColorAC (w->kolor)) ;
             else setfillstyle_ (MY_DOT_FILL, GetColorAC (8)) ;
		     bar (0, 0, pXk - pXp, pYp - pYk) ;
			 break;
          }
          else if (w->empty_typ==7)  //obszar aktywny na wydruku
          {
			 break;
          }
	     }
		 else break ;
	   }
       //if widoczny
	   for(i=0; i<(int)w->lp && i<MaxNumPoints; i++)
		if ((i % 2) == 0)
		{
			j=i/2;
			PolyPoints[i]=pikseleX0(w->xy[i]);
			//if (nag->obiekt==Owwielokat3D) PolyPoints_Z[j]=w->xy[w->lp+j];
		}
		else PolyPoints[i]=pikseleY0(w->xy[i]);

	   PolyPoints[8] = PolyPoints[0];
	   PolyPoints[9] = PolyPoints[1];

	   NumPoints=i/2;

	   translucency = 255;

	   if ((w->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
       {
           if ((w->atrybut==Ablok) ||
           ((dragging_quad_move == TRUE) && (w->atrybut == Aoblok)))
           {
               setcolor(kolory.blok);
               setfillstyle_(SOLID_FILL,kolory.blok);
           }
           else
           {
               SetColorAC(w->kolor);
               setfillstyle_(SOLID_FILL,GetColorAC(w->kolor));
           }
       }
        else
       {
            SetColorAC(8);
           setfillstyle_(SOLID_FILL,GetColorAC(8));
       }
      if (w->empty_typ==0)
        {
            if ((w->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
            {
                if ((w->atrybut==Ablok) ||
                ((dragging_quad_move == TRUE) && (w->atrybut == Aoblok)))
                {
                    setcolor(kolory.blok);
                    setfillstyle_(SOLID_FILL,kolory.blok);
                }
                else
                {
                    SetColorAC(w->kolor);
                    setfillstyle_(SOLID_FILL,GetColorAC(w->kolor));
                }
            }
            else
            {
                SetColorAC(8);
                setfillstyle_(SOLID_FILL,GetColorAC(8));
            }


          setlinestyle1(SOLID_LINE,0,NORM_WIDTH);

		  ignore_p = FALSE;

		  if ((w->translucent == 1) || (TRANSLUCENCY < 255))
		  {

			  if (w->translucent == 1)
			  {
				  translucency_ptr = w->xy;
				  translucency_ptr += (w->lp * sizeof(float));
				  memmove(&translucency, translucency_ptr, sizeof(unsigned char));
				  
			  }
			  else translucency = 255;

			  if (translucency > TRANSLUCENCY) translucency = TRANSLUCENCY;
			  set_trans_blender(0, 0, 0, (int)translucency);
			  set_mode_trans();
		  }
          else set_mode_solid();
          //GRADIENT
          gradient_ptr=NULL;
          if ((w->empty_typ == 0) && (w->gradient == 1)) {
              if (w->n == (w->lp*sizeof(float)+8))
                  w->gradient=0;

              if (w->gradient == 1)
              {
                  translucency_ptr = (char*)w->xy;
                  translucency_ptr += (w->lp * sizeof(float));

                  //gradient_ptr = (char*)w->xy;
                  //gradient_ptr += (w->lp * sizeof(float));
                  gradient_ptr=translucency_ptr+sizeof(unsigned char);
                  //memmove(&gradient, gradient_ptr, sizeof(GRADIENT));
                  solid_translucent_exist = TRUE;
              }
          }

		  if (w->pattern == 0)
		  {
			if (!ignore_p)  Draw_Solid(NumPoints, PolyPoints, w->pcx_solid, w->obiektt3, pikseleX0(0), pikseleY0(0), NULL, translucency, (GRADIENT4*)gradient_ptr);
              //set_mode_solid();
		  }
		   else
		   {
			   ;
			   //this is done in rysuj_bitmapy
		   }
		   if (w->translucent == 1)
		   { 
			   reset_trans_blender();
			   set_mode_solid();
		   }
        }
       else if (w->empty_typ==6)
        {
          setlinestyle3(DASHED_LINE,0,NORM_WIDTH);
            if ((w->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
            {
                if ((w->atrybut==Ablok) ||
                ((dragging_quad_move == TRUE) && (w->atrybut == Aoblok)))
                {
                    setcolor(kolory.blok);
                    setfillstyle_(MY_DOT_FILL,kolory.blok);
                }
                else
                {
                    SetColorAC(w->kolor);
                    setfillstyle_(MY_DOT_FILL,GetColorAC(w->kolor));
                }
            }
            else
            {
                SetColorAC(8);
                setfillstyle_(MY_DOT_FILL,GetColorAC(8));
            }

	       Draw_Solid (NumPoints, PolyPoints, w->pcx_solid, w->obiektt3, pikseleX0(0), pikseleY0(0), NULL, TRANSLUCENCY, NULL);
        }
        else if (w->empty_typ==7)
        {
            if ((w->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
            {
                if (w->atrybut==Ablok) setcolor(kolory.blok);
                else SetColorAC(w->kolor);
            }
            else SetColorAC(8);

            grubosc=linestyle_solid(w->empty_typ);
            for(i=0; i<(int)w->lp && i<MaxNumPoints; i+=2)
             {
              if (i==(w->lp-2)) lineC (PolyPoints[i], PolyPoints[i+1], PolyPoints[0], PolyPoints[1]);
               else lineC (PolyPoints[i], PolyPoints[i+1], PolyPoints[i+2], PolyPoints[i+3]);
             }
        }
         else
          {
              if ((w->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
              {
                  if ((w->atrybut==Ablok) ||
                  ((dragging_quad_move == TRUE) && (w->atrybut == Aoblok)))
                      setcolor(kolory.blok);
                  else SetColorAC(w->kolor);
              }
              else SetColorAC(8);

            grubosc=linestyle_solid(w->empty_typ);
            //for(i=0; i<(int)w->lp && i<MaxNumPoints; i+=2)
            // {
            //  if (i==(w->lp-2)) lineC (PolyPoints[i], PolyPoints[i+1], PolyPoints[0], PolyPoints[1]);
             //  else lineC (PolyPoints[i], PolyPoints[i+1], PolyPoints[i+2], PolyPoints[i+3]);
            // }
              for(i=0; i<(int)w->lp && i<MaxNumPoints; i+=2)
               {
                  if ((w->lp>6) && (trace_block==TRUE) && (((char*)w+w->n+sizeof(NAGLOWEK)-1)<=trace_block_end))
                  {
                      if (i == 4)  //endpoint
                      {
                          nag1=(NAGLOWEK*)((char*)w+sizeof(NAGLOWEK)+w->n);
                          if (nag1->obiekt==Owwielokat)
                          {
                              w1=(WIELOKAT*)nag1;
                              if ((Check_if_Equal(w->xy[4], w1->xy[2])) &&
                                  (Check_if_Equal(w->xy[5], w1->xy[3])) &&
                                  (Check_if_Equal(w->xy[6], w1->xy[0])) &&
                                  (Check_if_Equal(w->xy[7], w1->xy[1])))
                                  continue;
                          }
                          else if (nag1->obiekt==Osolidarc)
                          {
                              double xy[8];
                              sa1=(SOLIDARC*)nag1;
                              get_solidarc_ends(sa1, xy);

                              if ((Check_if_Equal(w->xy[4], xy[4])) &&
                                  (Check_if_Equal(w->xy[5], xy[5])) &&
                                  (Check_if_Equal(w->xy[6], xy[6])) &&
                                  (Check_if_Equal(w->xy[7], xy[7])))
                                  continue;
                          }
                          //it has to be also checked if beginning is not identical with the end of the last element in pline block
                      }
                      else if (i == 0)   //startpoint
                      {
                        if (((char*)w>trace_block_begin) &&
                            (Check_if_Equal(w->xy[0],last_trace_point[0].x)) &&
                            (Check_if_Equal(w->xy[1],last_trace_point[0].y)) &&
                            (Check_if_Equal(w->xy[2],last_trace_point[1].x)) &&
                            (Check_if_Equal(w->xy[3],last_trace_point[1].y)))
                        continue;
                        //it has to be also checked if beginning is not identical with the end of the last element in pline block
                      }
                  }
                if (i==(w->lp-2)) lineC (PolyPoints[i], PolyPoints[i+1], PolyPoints[0], PolyPoints[1]);
                else lineC (PolyPoints[i], PolyPoints[i+1], PolyPoints[i+2], PolyPoints[i+3]);
               }
              last_trace_point[0].x=w->xy[6];
              last_trace_point[0].y=w->xy[7];
              last_trace_point[1].x=w->xy[4];
              last_trace_point[1].y=w->xy[5];
          }
         set_mode_solid();
	   break;

     case Ospline:
		 licznik_obiektow+=2;
		 s = (SPLINE*)nag;

		 rysuj_spline_(s, s->xy, mode, 1, FALSE, FALSE, FALSE);

		 break;
	 case Oluk :
	   licznik_obiektow+=2;
	   l=(LUK*)nag;
	   if ((l->typ == HATCH_OUTLINE_TYPE) && (Get_Point_View() == FALSE)) break;
	   if(!(l->widoczny=luk_wybrany(l))) break;
       select_color_type(l);
	   pattern_count = TRUE;

       //checking if arc is dimensioning arc
       if ((l->blok == 1) && (l->obiektt1 == 0) && (l->obiektt2 == 1) && (l->obiektt3 == 0))
       {
           //check if next object is text or line then text
           nag1 = (char*)nag + sizeof(NAGLOWEK) + l->n;
           if (nag1->obiekt == Otekst)
           {
               t = (TEXT*)nag1;
               if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
               {
                   //checking crossing with text
                   //for (i=0; i<4; i++)
                   //    ret=lineC(pikseleX0(t_outline.xy[i*2]), pikseleY0(t_outline.xy[i*2+1]), pikseleX0(t_outline.xy[(i*2+2)%8]), pikseleY0(t_outline.xy[(i*2+3)%8]));
                   //checking crossing with text
                   outlinetext(t, &t_outline, 0.5);
                   Normalize_Quad (&t_outline);
                   //for (i=0; i<4; i++)
                   //    ret=lineC(pikseleX0(t_outline.xy[i*2]), pikseleY0(t_outline.xy[i*2+1]), pikseleX0(t_outline.xy[(i*2+2)%8]), pikseleY0(t_outline.xy[(i*2+3)%8]));
                   ret=trim_arc_to_quad(l, &t_outline, &l_tmp, &l_tmp1);
                   ////
                   switch (ret)
                   {
                       case 0:
                           //L->widoczny = lineC(pikseleX0(L->x1), pikseleY0(L->y1), pikseleX0(L->x2),pikseleY0(L->y2));
                           DrawArc(pikseleX0(l->x),pikseleY0(l->y),l->kat1,l->kat2,pikseleDX(l->r), mode);
                           break;
                       case 1: //L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                           DrawArc(pikseleX0(l_tmp.x),pikseleY0(l_tmp.y),l_tmp.kat1,l_tmp.kat2,pikseleDX(l_tmp.r), mode);
                           break;
                       case 2:// L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                           DrawArc(pikseleX0(l_tmp.x),pikseleY0(l_tmp.y),l_tmp.kat1,l_tmp.kat2,pikseleDX(l_tmp.r), mode);
                          // L->widoczny = lineC(pikseleX0(L_tmp1.x1), pikseleY0(L_tmp1.y1), pikseleX0(L_tmp1.x2),pikseleY0(L_tmp1.y2));
                           DrawArc(pikseleX0(l_tmp1.x),pikseleY0(l_tmp1.y),l_tmp1.kat1,l_tmp1.kat2,pikseleDX(l_tmp1.r), mode);
                           break;
                       default:
                           break;
                   }
                   break;
               }
           }
           else if (nag1->obiekt == Olinia)
           {
               nag1 = (char*)nag1 + sizeof(NAGLOWEK) + nag1->n;
               if (nag1->obiekt == Olinia)
                   nag1 = (char*)nag1 + sizeof(NAGLOWEK) + nag1->n;
               if (nag1->obiekt == Otekst)
               {
                   t = (TEXT*)nag1;
                   if ((t->blok == 1) && (t->obiektt1 == 0) && (t->obiektt2 == 1) && (t->obiektt3 == 0))
                   {
                       //checking crossing with text
                       outlinetext(t, &t_outline, 0.5);
                       Normalize_Quad (&t_outline);
                       //for (i=0; i<4; i++)
                       //    ret=lineC(pikseleX0(t_outline.xy[i*2]), pikseleY0(t_outline.xy[i*2+1]), pikseleX0(t_outline.xy[(i*2+2)%8]), pikseleY0(t_outline.xy[(i*2+3)%8]));
                       ret=trim_arc_to_quad(l, &t_outline, &l_tmp, &l_tmp1);
                       ////
                       switch (ret)
                       {
                           case 0:
                              // L->widoczny = lineC(pikseleX0(L->x1), pikseleY0(L->y1), pikseleX0(L->x2),pikseleY0(L->y2));
                               DrawArc(pikseleX0(l->x),pikseleY0(l->y),l->kat1,l->kat2,pikseleDX(l->r), mode);
                               break;
                           case 1: //L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                               DrawArc(pikseleX0(l_tmp.x),pikseleY0(l_tmp.y),l_tmp.kat1,l_tmp.kat2,pikseleDX(l_tmp.r), mode);
                               break;
                           case 2: //L->widoczny = lineC(pikseleX0(L_tmp.x1), pikseleY0(L_tmp.y1), pikseleX0(L_tmp.x2),pikseleY0(L_tmp.y2));
                               DrawArc(pikseleX0(l_tmp.x),pikseleY0(l_tmp.y),l_tmp.kat1,l_tmp.kat2,pikseleDX(l_tmp.r), mode);
                               //L->widoczny = lineC(pikseleX0(L_tmp1.x1), pikseleY0(L_tmp1.y1), pikseleX0(L_tmp1.x2),pikseleY0(L_tmp1.y2));
                               DrawArc(pikseleX0(l_tmp1.x),pikseleY0(l_tmp1.y),l_tmp1.kat1,l_tmp1.kat2,pikseleDX(l_tmp1.r), mode);
                               break;
                           default:
                               break;
                       }
                       break;
                   }
               }
               //L->widoczny = lineC(pikseleX0(L->x1), pikseleY0(L->y1), pikseleX0(L->x2), pikseleY0(L->y2));
               DrawArc(pikseleX0(l->x),pikseleY0(l->y),l->kat1,l->kat2,pikseleDX(l->r), mode);
               break;
           }
       }
	   //DrawArc(pikseleX0(l->x),pikseleY0(l->y),l->kat1,l->kat2,pikseleDX(l->r), mode);
       switch (l->obiektt2)
       {
           case 4:
               V.warstwa=l->warstwa;
               V.kolor=l->kolor;
               V.typ=l->typ;
               V.x1=l->x;
               V.y1=l->y;
               V.r=l->r;
               V.angle1=l->kat1;
               V.angle2=l->kat2;
               V.magnitude1=0.0f;
               if (l->obiektt3 == 0) V.style = V_EDGE_ARC_ROLL;
               else V.style = V_EDGE_ARC_ROLL_INV;
               Draw_Vector(&V, COPY_PUT, 1, 0);
               break;
           case 6:
               V.warstwa=l->warstwa;
               V.kolor=l->kolor;
               V.typ=l->typ;
               V.x1=l->x;
               V.y1=l->y;
               V.r=l->r;
               V.angle1=l->kat1;
               V.angle2=l->kat2;
               V.magnitude1=0.0f;
               if (l->obiektt3 == 0) V.style = V_EDGE_ARC_SIMPLE;
               else V.style = V_EDGE_ARC_SIMPLE_INV;
               Draw_Vector(&V, COPY_PUT, 1, 0);
           break;
           case 7:
               V.warstwa=l->warstwa;
               V.kolor=l->kolor;
               V.typ=l->typ;
               V.x1=l->x;
               V.y1=l->y;
               V.r=l->r;
               V.angle1=l->kat1;
               V.angle2=l->kat2;
               V.magnitude1=0.0f;
               if (l->obiektt3 == 0) V.style = V_EDGE_ARC_FIXED;
               else V.style = V_EDGE_ARC_FIXED_INV;
               Draw_Vector(&V, COPY_PUT, 1, 0);
           break;
           default:
               DrawArc(pikseleX0(l->x),pikseleY0(l->y),l->kat1,l->kat2,pikseleDX(l->r), mode);
           break;
       }
	   pattern_count = FALSE;
	   pattern_offset = 0;
	   break;
	 case Opoint :
       licznik_obiektow++;
	   ptrs_point = (T_Point*)nag ;

 	   if (!(ptrs_point->widoczny = Point_Selected (ptrs_point))) break ;

	   if ((ptrs_point->warstwa == Current_Layer) || (options1.view_only_current_layer == 0))
	   {
           if ((ptrs_point->atrybut == Ablok) ||
           ((dragging_quad_move == TRUE) && (ptrs_point->atrybut == Aoblok)))
           {
               setcolor(kolory.blok);
               setfillstyle_(SOLID_FILL,kolory.blok);
           }
           else
           {
               SetColorAC(ptrs_point->kolor);
               setfillstyle_(SOLID_FILL, GetColorAC(ptrs_point->kolor));
           }
	   }
	   else
	   {
		   SetColorAC(8);
		   setfillstyle_(SOLID_FILL, GetColorAC(8));
	   }
	   linestyle(64);

	   Draw_Point (ptrs_point, mode, 1) ;

	   break;
       case Ovector :
               licznik_obiektow++;
               ptrs_vector = (AVECTOR*)nag ;

               if ((ptrs_vector->atrybut != Ausuniety) &&
                   (ptrs_vector->atrybut != Abad))
               {
                   if ((ptrs_vector->style > 9) && ((ptrs_vector->style < 16) || (ptrs_vector->style == 20))) bitmap_vector_exist = TRUE;  // (ptrs_vector->style == 18 and == 19 is not practical due to covering by gradients

                   if (!(ptrs_vector->widoczny = Vector_Selected(ptrs_vector))) break;

                   if ((ptrs_vector->warstwa == Current_Layer) || (options1.view_only_current_layer == 0)) {
                       if ((ptrs_vector->atrybut == Ablok) ||
                           ((dragging_quad_move == TRUE) && (ptrs_vector->atrybut == Aoblok))) {
                           setcolor(kolory.blok);
                           setfillstyle_(SOLID_FILL, kolory.blok);
                       } else {
                           SetColorAC(ptrs_vector->kolor);
                           setfillstyle_(SOLID_FILL, GetColorAC(ptrs_vector->kolor));
                       }
                   } else {
                       SetColorAC(8);
                       setfillstyle_(SOLID_FILL, GetColorAC(8));
                   }

                   select_color_type(ptrs_vector);

                   Draw_Vector(ptrs_vector, mode, 1, 1);
               }

               break;
    case Opcx :
		licznik_obiektow += 11;
      b_pcx = (B_PCX *)nag;
	  if ((b_pcx->atrybut != Ausuniety) &&
		  (b_pcx->atrybut != Abad))
	  {

		  if (b_pcx->on_front == 0)
		  {
			  if (bitmap_exist_o == FALSE)
			  {
				  redraw_again = TRUE;
				  bitmap_exist = TRUE;
				  return;
			  }
			  bitmap_exist = TRUE;
			  if (b_pcx->kod_obiektu == 2) bitmap_png_exist = TRUE;

			  if (Get_Bitmap_Outline() && (b_pcx->widoczny))
			  {
                  if ((b_pcx->atrybut == Ablok) ||
                  ((dragging_quad_move == TRUE) && (b_pcx->atrybut == Aoblok)))
                  { gr=5; kolor=kolory.blok;}
                  else { gr=3; kolor=kolory.cur;}

				  rysuj_bitmap_outline((char*)nag, COPY_PUT, FALSE, 1, kolor, gr);

			  }
		  }
		  else
		  {
			  bitmap_on_front_exist = TRUE;
		  }
	  }
      break;
	 default :
	   break;  
	} /*switch*/
     }
aaa:
    obiekt_tok(NULL,adh,(char **) &nag,ONieOkreslony);
	if ((pattern_count == TRUE) && (pattern_add_limit>0) && ((long_long)nag > pattern_add_limit))
	{
		pattern_count = FALSE;
		pattern_add_limit = 0;
		pattern_offset = 0;
	}
  }

 if (b_info_break == TRUE)
 {
	 komunikat0(0);
 }

 reset_dane0();

}


void  Clear_Client_Screen (void)
/*-------------------------------*/
{
  setfillstyle_ (EMPTY_FILL, 0);
  bar (xp, yp, maxX - xk, maxY - yk);
}

char *get_units(void)
{
	int i;

	for (i = 0; i < 5; i++)
		if (Jednostki == funits[i]) break;
	return upunits[i];
}

void set_st_jedn(void)
{  int i;
    for (i = 0; i<8; i++)
        if (Jednostki == funits [i]) break;
    strcpy(st_jedn,punits[i]);
}

void view_scale(void)
{
  static char st_scale[20];
  int i;


  if (SkalaF>1) sprintf(st_scale,u8"1:%d %s",(int)SkalaF,st_jedn);
    else sprintf(st_scale,u8"1:%4.2f %s",SkalaF,st_jedn);

  if (SkalaF>1) sprintf(st_scale,u8"1:%d %s",(int)SkalaF,st_jedn);
    else sprintf(st_scale,u8"1:%4.2f %s",SkalaF,st_jedn);

  setfillstyle_(SOLID_FILL, BKCOLOR);
  bar(maxX - (int)strlen(st_scale)*WIDTH - 2, ED_INF_HEIGHT + 1, maxX, 2 * ED_INF_HEIGHT); // +1);
  moveto(maxX-(int)strlen(st_scale)*WIDTH-2, ED_INF_HEIGHT + 3);
  setcolor(kolory.ink);
  outtext_r(st_scale);
}

void reset_background(void)
{
	pYk_arch = 0;
	pXk_arch = 9999;
}

void flip_corner(void) {
    int mxxx, myyy;
    int scy0;
    int sc_w, sc_h;
    int x2, y2;
    float demo_scale;
    int v32;

    v32=get_v32();

    demo_scale=get_demo_scale();
    sc_w=((BITMAP*)second_screen)->cr-((BITMAP*)second_screen)->cl;
    sc_h=((BITMAP*)second_screen)->cb-((BITMAP*)second_screen)->ct;

    scy0=((BITMAP*)second_screen)->cb-2-v32*demo_scale;

    x2=3*v32*demo_scale;
    y2=v32*demo_scale;

    if (x2>(sc_w-2)) x2=sc_w-2;
    if (y2>(sc_h-2))
    {
        y2=sc_h-2;
        scy0=0;
    }

    scy0=((BITMAP*)second_screen)->cb-2-v32*demo_scale;
    my_blit((BITMAP *) second_screen, 2, scy0-1, pXp+2, maxY -2 - v32*demo_scale, x2, y2+1);
    get_mouse_mickeys(&mxxx, &myyy);
}

void flip_corner3(void) {
	int mxxx, myyy;
	int scy0;
	int sc_w, sc_h;
	int x2, y2;
	float demo_scale;
	BITMAP* sec_screen;
    int v32;

    v32=get_v32();

	sec_screen = (BITMAP*)get_second_screen_bak_();

	if (sec_screen == NULL) return;

	demo_scale = get_demo_scale();
	sc_w = sec_screen->cr - sec_screen->cl;
	sc_h = sec_screen->cb - sec_screen->ct;

	scy0 = sec_screen->cb - 2 - v32 * demo_scale;

	x2 = 3 * v32 * demo_scale;
	y2 = v32 * demo_scale;

	if (x2 > (sc_w - 2)) x2 = sc_w - 2;
	if (y2 > (sc_h - 2))
	{
		y2 = sc_h - 2;
		scy0 = 0;
	}

	scy0 = sec_screen->cb - 2 - v32 * demo_scale;
	my_blit(sec_screen, 2, scy0 - 1, pXp + 2, maxY - 2 - v32 * demo_scale, x2, y2 + 1);
	get_mouse_mickeys(&mxxx, &myyy);
}

void flip_shift_screen(LINIA *line_g0) {
	int mxxx, myyy;
	double ddx, ddy;
	long ldx, ldy;
	int ssx0, sx0, ssy0, sy0, dxx, dyy;
	int x1, x2, y1, y2, x11, x22, y11, y22;

	ddx = line_g0->x2 - line_g0->x1;
	ddy = line_g0->y1 - line_g0->y2;
	ldx = pikseleDX(ddx);
	ldy = pikseleDY(ddy);

	if (ldx > 0)
	{
		ssx0 = 0;
		sx0 = ldx;
		x1 = 0; x2 = ldx;
	}
	else
	{
		ssx0 = -ldx;
		sx0 = 0;
		x1 = pXk + ldx; x2 = pXk;
	}

	y1 = pYp; y2 = pYk;

	if (ldy > 0)
	{
		ssy0 = 0;
		sy0 = ldy;
		y11 = pYk + ldy; y22 = pYk;
	}
	else
	{
		ssy0 = -ldy;
		sy0 = 0;
		y11 = pYp; y22 = pYp + ldy;
	}

	x11 = pXp; x22 = pXk;

	my_blit((BITMAP*)second_screen, ssx0, ssy0, pXp + sx0, maxY - (pYp - pYk) + sy0, pXk - pXp + 1 - abs(ldx), pYp - pYk + 1 - abs(ldy));

	//filling the void
	setfillstyle_(SOLID_FILL, kolory.paper);
	bar(x1, y1, x2, y2);
	bar(x11, y11, x22, y22);

	get_mouse_mickeys(&mxxx, &myyy);
}


void flip_screen(void) {
    int mxxx, myyy;
    my_blit((BITMAP *) second_screen, 0, 0, pXp, maxY - (pYp - pYk), pXk - pXp + 1, pYp - pYk + 1);
    get_mouse_mickeys(&mxxx, &myyy);
}

#ifdef ALLEGRO5
void flip_screen_sd(BITMAP * src_screen, BITMAP * dst_screen) {
    int mxxx, myyy;
    my_blit_sd(src_screen, dst_screen,0, 0, pXp, maxY - (pYp - pYk), pXk - pXp + 1, pYp - pYk + 1);
    get_mouse_mickeys(&mxxx, &myyy);
}
#endif

void flip_full_screen(BITMAP * the_screen) {
    int mxxx, myyy;
    my_blit(the_screen, 0, 0, 0, 0, getmaxx(), getmaxy());
    get_mouse_mickeys(&mxxx, &myyy);
}

void flip_any_screen(BITMAP *third_screen) {
    int mxxx, myyy;
    my_blit((BITMAP *) third_screen, 0, 0, 0, 0, getmaxx(), getmaxy());
    get_mouse_mickeys(&mxxx, &myyy);
}


void Set_Second_Screen(void)
{
    Set_Screenplay((BITMAP *)second_screen);
}

void Set_Screen(void)
{
    Set_Screenplay(screen);
}

void  redraw_cur(BOOL cur)
/*-----------------------*/
{
	int k = 1;
	int ret = 0;

	static int komunikat_R, komunikat_R0;
	long_long adp_b, adk_b;
	int patt_o, patt_s;
	BOOL patt_c;

	patt_o = get_pattern_offset();
	patt_c = get_pattern_count();
	patt_s = get_sketch_offset();

	adp_b = (long_long)ADP;
	adk_b = (long_long)ADK;

	x0_bak = -1;
	y0_bak = -1;

	if (bitmap_exist == TRUE)
	{
		;
	}

	Set_Screenplay((BITMAP *)second_screen);   ////////////////////

	bRedrow = TRUE;

	okno_all_second();  //////////////////

	ClearScreen_second();   ////////////////

	komunikat_R = Komunikat_R;
	komunikat_R0 = Komunikat_R0;

	okno_r_second();

	if ((bitmap_exist == TRUE) || (bitmap_pattern_exist == TRUE))
	{
		rysuj_bitmapy();
	}
	else
	{
		if (ctx_created == TRUE)  Destroy_All_ctx();   //zwolnienie pamieci jezeli niema bitmap
	}

	if ((options1.ignore_buffering_pcx != 0) && (bitmap_exist == TRUE))
	{
		////my_blit_reverse(second_screen, 0, 0, pXp, maxY - (pYp - pYk), pXk - pXp + 1, pYp - pYk + 1);
	}

	okno_r_second();

	rysuj_obiekty();

	if (redraw_again == TRUE)
	{
		set_pattern_offset(patt_o);
		set_pattern_count(patt_c);
		set_sketch_offset(patt_s);
		set_pattern_add_limit(0);
		return;
	}

	if (bitmap_on_front_exist)
	{
		Set_Screenplay(second_screen);
		rysuj_skin();
		Set_Screenplay(screen);
	}

    okno_all_second();
    grid_on();
    ramka(0);
    ramka(1);

	okno_r();
	okno_r_second();

	my_blit((BITMAP*)second_screen, 0, 0, pXp, maxY - (pYp - pYk), pXk - pXp + 1, pYp - pYk + 1);

	okno_all();
	//uzupelnienie okna pulpitu dla malych skala prezentacji

	if (pYk > (2 * ED_INF_HEIGHT + 3))
	{
		setfillpattern_gregre();
		_normal_rectfill(screen, pXp, 2 * ED_INF_HEIGHT + 1, maxX, pYk  - 1, palette_color[0]);
	}

	if (pXk < maxX)
	{
		setfillpattern_gregre();
		_normal_rectfill(screen, pXk+1, pYk, maxX, pYp, palette_color[0]);
	}

	setfillstyle(1, palette_color[0]);

	if (hatching_in_progress)
	{
		ADP = (char *)adp_b;
		ADK = (char *)adk_b;
	}
	else
	{
		ADP = dane;
		ADK = dane + dane_size;
	}
	REDDOD();
	REDRAW_ADD_MENU();
	mvcurb.akton = 1;
	if (Error == 0) komunikat(komunikat_R);
	if (komunikat_R0>0) komunikat0(komunikat_R0);
	else if (get_komunikat_arc_bak()>=0)
	{
		komunikat0(0);
		ret = kom_arc_bak();
	}

	Komunikat_R0 = komunikat_R0;

	if ((cur == TRUE) && (do_not_draw_cur == FALSE))
	{
		CUR_ON(X, Y);
	}
	mvcurb.akton = 0;

	if (do_not_draw_keys==FALSE) draw_keyimages();

 	pozycja_kursora();

	view_scale();
	bRedrow = FALSE;

    winf_addf();

    int lp_=get_lp();
    if (lp_>1)
    {
        for (int i=1; i<lp_; i++) redraw_str(i);
    }

    if (get_short_notice()) komunikat_str_short_reminder();

	current_cursor_fx = get_skala() * DokladnoscF;

	set_pattern_offset(patt_o);
	set_pattern_count(patt_c);
	set_sketch_offset(patt_s);
	set_pattern_add_limit(0);
}


 void  redraw_cur_preview(BOOL cur, int dark)
	 /*------------------------------------*/
 {
	 int k = 1;
	 int ret = 0;
	 int kolory_paper;
	 static int komunikat_R, komunikat_R0;
	 unsigned int view_only_current_layer;

	 view_only_current_layer = options1.view_only_current_layer;
	 options1.view_only_current_layer = 0;

	 kolory_paper = kolory.paper;
	 if (dark < -1)
	 {
		 if (dark < -2)
		 {
			 int bkgcolor = get_palette_color(kolory.paper);
			 unsigned char red = (bkgcolor & 0xFF0000) >> 16;
			 unsigned char green = (bkgcolor & 0x00FF00) >> 8;
			 unsigned char blue = (bkgcolor & 0x0000FF);
			 unsigned char intensity = 0.2989 * red + 0.5870 * green + 0.1140 * blue;
			 if (intensity < 128) kolory.paper = 15;
		 }
		 else kolory.paper = 15;
	 }
	
	 if (bitmap_exist == TRUE)
	 {
		
	 }

	 Set_Screenplay((BITMAP*)second_screen);

	 okno_all_second();

	 ClearScreen_second_preview();

	 komunikat_R = Komunikat_R;
	 komunikat_R0 = Komunikat_R0;
	 
	 okno_r();
	 okno_r_second();


	 if ((bitmap_exist == TRUE) || (bitmap_pattern_exist == TRUE))
	 {
         if ((bitmap_exist == TRUE) && (dark<-1)) regen_ctx = TRUE;
		 rysuj_bitmapy();
	 }
	 else
	 {
		 if (ctx_created == TRUE)  Destroy_All_ctx();   //zwolnienie pamieci jezeli niema bitmap
	 }


	 okno_r_second();

	 rysuj_obiekty();


	 okno_r_second();

	 if (bitmap_on_front_exist)
	 {
		 Set_Screenplay(second_screen);
		 rysuj_skin();
		 Set_Screenplay(screen);
	 }

	 
	 okno_all();

	 kolory.paper = kolory_paper;

	 options1.view_only_current_layer = view_only_current_layer;

 }

 //buttons[]
#define BUT_CONDENSED_Y 1
#define BUT_CONDENSED_N 2

#define BUT_ROTATION 3
#define BUT_INVERSION 4
#define BUT_REFLECTION 5

#define BUT_GRAY_PRINT 15
#define BUT_BW_PRINT 16
#define BUT_PCX_GRAY 17
#define BUT_PRINT_WINDOW 18
#define BUT_PRINT_SHEET 19
#define BUT_ACTUAL_SIZE 22

 //edit[]
#define IL_WIDTH_PAPER 30
#define IL_HEIGHT_PAPER 31
#define IL_MARGIN 32
#define IL_SCALE 38
#define IL_PAGE 39
#define IL_LEFT_MARGIN 42
#define IL_TOP_MARGIN 43
#define IL_RIGHT_MARGIN 44
#define IL_BOTTOM_MARGIN 45

static float intensity_tab[4] = { 0.2, 1.0, 0.8, 0.6 };

 void gray_bitmap(BITMAP *src, BITMAP *dst, int dx, int dy, int gray_sat)
 {
	 int i, j;
	 int color, red, green, blue;
	 double intensity;
	 byte_ int_intensity;
	 int gray;

	 for (j = 0; j < dy; j++)
	 {
		 for (i = 0; i < dx; i++)
		 {
			 color = _getpixel32(src, i, j);
			 red = getr(color);
			 green = getg(color);
			 blue = getb(color);

			 intensity = 0.2989*red + 0.5870*green + 0.1140*blue;

			 if (intensity<252) intensity *= intensity_tab[gray_sat];

			 int_intensity = (byte_)intensity;
			 gray = makecol(int_intensity, int_intensity, int_intensity); //255
			 _putpixel32(dst, i, j, gray);

		 }
	 }
 }

void dimm_dialog_bitmap(BITMAP *src, BITMAP *dst, int dx, int dy, int gray_sat)
{
    int i, j;
    int color, red, green, blue;
    double intensity;
    byte_ int_intensity;
    int gray;

    for (j = 0; j < dy; j++)
    {
        for (i = 0; i < dx; i++)
        {
            color = _getpixel32(src, i, j);
            red = getr(color);
            green = getg(color);
            blue = getb(color);

            if ((red==0) && (green==0) && (blue==0)) red=green=blue=32;

            if (color!=0xaaaaaa)
            {
                intensity = 0.2989*red + 0.5870*green + 0.1140*blue;
                if (intensity<252) intensity *= gray_sat;
                if (intensity>255) intensity=255;
                int_intensity = (byte_)intensity;
                gray = makecol(int_intensity, int_intensity, int_intensity);
                _putpixel32(dst, i, j, gray);
            }
            else {
                gray = makecol(red, green, blue);
                _putpixel32(dst, i, j, gray);
            }
        }
    }
}

 
 void ShowPreview(int x01, int y01, int x02, int y02)
 {
	 float scalex, scaley;
	 int x1, y1, x2, y2;
	 x1 = x01 + 1; x2 = x02 - 1; y1 = y01 + 1; y2 = y02 - 1;

	 skala_format(-1);

	 scalex = (float)(x2 - x1) / (float)(pXk - pXp);
	 scaley = (float)(y2 - y1) / (float)(pYp - pYk);

	 if (scalex < scaley)
	 {
		 skala = scalex*skala;
	 }
	 else
	 {
		 skala = scaley*skala;
	 }

	 SkalaZC();
	 skala_menup_();

	 redraw();

	 return;
 }


 void CopyPreview(int x01, int y01, int x02, int y02, int client_number)
 {
	 float scalex, scaley;
	 float dx, dy;
	 int x1, y1, x2, y2;
	 int ddx, ddy;
	 BITMAP *PREVIEW;

	 x1 = x01 + 1; x2 = x02 - 1; y1 = y01 + 1; y2 = y02 - 1;

	 scalex = (float)(x2 - x1) / (float)(pXk - pXp);
	 scaley = (float)(y2 - y1) / (float)(pYp - pYk);

	 if (scalex < scaley)
	 {
		 dy = ((float)(y2 - y1)*(1.0 - scalex / scaley) / 2.0) - 1.0;
		 y1 += (int)dy;
		 y2 -= (int)dy;
	 }
	 else
	 {
		 dx = ((float)(x2 - x1)*(1.0 - scaley / scalex) / 2.0) - 1.0;
		 x1 += (int)dx;
		 x2 -= (int)dx;
	 }

	 ddx = x2 - x1;
	 ddy = y2 - y1;


	 if (second_screen != NULL)
	 {

		 PREVIEW = create_bitmap(ddx, ddy);

		 stretch_blit((BITMAP*)second_screen, PREVIEW, 0, 0, pXk - pXp + 1, pYp - pYk + 1, 0, 0, ddx, ddy);
		 //deposition of the bitmap, coordinates and file name in DLL

		 convert_and_deposit_bitmap(PREVIEW, x1, y1, x2, y2, zbior_danych, client_number);

		 destroy_bitmap(PREVIEW);
	 }

	 return;
 }



 void CopyPreview1(int x01, int y01, int x02, int y02, int client_number)
 {

	 int x1, y1, x2, y2;
	 int ddx, ddy;
	 BITMAP *PREVIEW;

	 x1 = x01 + 1; x2 = x02 - 1; y1 = y01 + 1; y2 = y02 - 1;

	 ddx = x2 - x1;
	 ddy = y2 - y1;

	 PREVIEW = create_bitmap(ddx, ddy);

	 stretch_blit((BITMAP*)second_screen, PREVIEW, 0, 0, pXk - pXp + 1, pYp - pYk + 1, 0, 0, ddx, ddy);
	 //deposition of the bitmap, coordinates and file name in DLL
	 convert_and_deposit_bitmap(PREVIEW, x1, y1, x2, y2, zbior_danych, client_number);

	 destroy_bitmap(PREVIEW);

	 return;
 }


 void redrawPreview(int x01, int y01, int x02, int y02, int x0, int y0, BOOL gray, BOOL bw, int gray_sat, int *real_x1, int *real_y1, int *real_x2, int *real_y2, unsigned char adjust, BOOL fill_background, T_Prn_Ini_Date *prn_ini_date)
 /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
 {
	 BITMAP *PREVIEW;
	 BITMAP *PREVIEW1;
	 BITMAP *PREVIEW2;
	 BITMAP *PREVIEW3;

	 float scalex, scaley;
	 float dx, dy;
	 int x1, y1, x2, y2;
	 int ddx, ddy, ddxy, ddyx;
	 BOOL portrate;
	 int dark;
     double fXpk, fYpk;
     int iXpk, iYpk;

	 x1 = x01 + 1; x2 = x02 - 1; y1 = y01 + 1; y2 = y02 - 1;

	 Save_View_Preview();
	 
	 if (fill_background) dark = -3;
	 else dark = -2;

     skala_window_p(prn_ini_date->xp, prn_ini_date->xk, prn_ini_date->yp, prn_ini_date->yk, dark);

	 scalex = (float)(x2 - x1) / (float)(pXk - pXp);
	 scaley = (float)(y2 - y1) / (float)(pYp - pYk);

     //expected window: prn_ini_date->xp, prn_ini_date->xk, prn_ini_date->yp, prn_ini_date->yk
     //effective window: Xp, Xk, Yp, Yk
     //second_screen window:  pXp, pXk, pYk, pYp
     //calculating let, right, top and bottom margins
     //fXpk=((Xk-Xp)-(prn_ini_date->xk-prn_ini_date->xp))/2.0;
     //fYpk=((Yk-Yp)-(prn_ini_date->yk-prn_ini_date->yp))/2.0;

     if (Check_if_Equal(Xk, Xp)) fXpk=0.0;
     else {
         fXpk = fabs(((Xk-Xp)-(prn_ini_date->xk-prn_ini_date->xp)) / (2.0*(Xk - Xp)));
     }

     if (Check_if_Equal(Yk, Yp)) fYpk=0.0;
     else {
         fYpk = fabs(((Yk-Yp)-(prn_ini_date->yk-prn_ini_date->yp)) / (2.0*(Yk - Yp)));
     }

     iXpk=(int)(((double)(pXk - pXp)*fXpk)+0.5);
     iYpk=(int)(((double)(pYp - pYk)*fYpk)+0.5);

	 //portrate
	 if ((Get_Check_Button(&printer_dlg, BUT_ROTATION) && (!Get_Check_Button(&printer_dlg, BUT_INVERSION))) ||
		 (Get_Check_Button(&printer_dlg, BUT_INVERSION) && (Get_Check_Button(&printer_dlg, BUT_ROTATION))))
	 {
		 scalex = (float)(x2 - x1) / (float)(pYp - pYk - 2*iYpk);
		 scaley = (float)(y2 - y1) / (float)(pXk - pXp - 2*iXpk);

		 if (adjust == 0)  //central
		 {
			 if (scalex < scaley)
			 {
				 dy = ((float)(y2 - y1)*(1.0 - scalex / scaley) / 2.0) - 1.0;
				 y1 += (int)(dy+0.5);
				 y2 -= (int)(dy+0.5);
			 }
			 else
			 {
				 dx = ((float)(x2 - x1)*(1.0 - scaley / scalex) / 2.0) - 1.0;
				 x1 += (int)(dx+0.5);
				 x2 -= (int)(dx+0.5);
			 }
		 }
		 else if (adjust & 3)
		 {
			 if (scalex < scaley)
			 {
				 dy = (float)(y2 - y1)*(1.0 - scalex / scaley) - 1.0;
				 y2 -= (int)(dy+0.5);
			 }
			 else
			 {
				 dx = (float)(x2 - x1)*(1.0 - scaley / scalex) - 1.0;
				 x2 -= (int)(dx+0.5);
			 }
		 }
	 }
	 else //landscape
	 {
		 scalex = (float)(x2 - x1) / (float)(pXk - pXp - 2*iXpk);
		 scaley = (float)(y2 - y1) / (float)(pYp - pYk - 2*iYpk);

		 if (adjust == 0) //central
		 {
			 if (scalex < scaley)
			 {
				 dy = ((float)(y2 - y1)*(1.0 - scalex / scaley) / 2.0) - 1.0;
				 y1 += (int)(dy+0.5);
				 y2 -= (int)(dy+0.5);
			 }
			 else
			 {
				 dx = ((float)(x2 - x1)*(1.0 - scaley / scalex) / 2.0) - 1.0;
				 x1 += (int)(dx+0.5);
				 x2 -= (int)(dx+0.5);
			 }
		 }
		 else
		 {
			 if (scalex < scaley)
			 {
				 dy = (float)(y2 - y1)*(1.0 - scalex / scaley) - 1.0;
				 y2 -= (int)(dy+0.5);
			 }
			 else
			 {
				 dx = (float)(x2 - x1)*(1.0 - scaley / scalex) - 1.0;
				 x2 -= (int)(dx+0.5);
			 }
		 }
	 }

	 ddx = x2 - x1;
	 ddy = y2 - y1;


	 if (abs(pXk - pXp - 2*iXpk) > abs(pYk - pYp - 2*iYpk))  //landscape
	 {
		 ddxy = abs(ddy - ddx);
		 ddyx = 0;
		 portrate = FALSE;
	 }
	 else
	 {
		 ddxy = 0;
		 ddyx = abs(ddy - ddx);
		 portrate = TRUE;
	 }

	 ddxy = abs(ddy - ddx);

	 PREVIEW = create_bitmap(ddx, ddy);  // []
	 PREVIEW1 = create_bitmap(max(ddx, ddy), max(ddx,ddy));  //==
	 PREVIEW2 = create_bitmap(ddx, ddy);  // []
	 PREVIEW3 = create_bitmap(max(ddx,ddy), max(ddx,ddy));  //==

     set_clip_state(PREVIEW, 1);
     set_clip_state(PREVIEW1, 1);
     set_clip_state(PREVIEW2, 1);
     set_clip_state(PREVIEW3, 1);


	 if (PREVIEW && PREVIEW1 && PREVIEW2 && PREVIEW3)
	 {	 
		 if (Get_Check_Button(&printer_dlg, BUT_ROTATION) && (!Get_Check_Button(&printer_dlg, BUT_INVERSION)))
		 {
			 stretch_blit((BITMAP*)second_screen, PREVIEW1, 0/*iXpk*/, iYpk, pXk - pXp - 2*iXpk + 1, pYp - pYk - 2*iYpk + 1, 0, 0, y2 - y1, x2 - x1);////
			 if (gray) gray_bitmap(PREVIEW1, PREVIEW1, PREVIEW1->w /*ddx*/, PREVIEW1->h /*ddy*/, gray_sat);
			 else if (bw) gray_bitmap(PREVIEW1, PREVIEW1, PREVIEW1->w /*ddx*/, PREVIEW1->h /* ddy*/, gray_sat /*0*/);  //0 gives black, and it was changed to get always gray background if any
			 rotate_sprite(PREVIEW3, PREVIEW1, 0, 0, itofix(64));  //0,0

			 if (Get_Check_Button(&printer_dlg, BUT_REFLECTION))
			 {
				 draw_sprite_v_flip(PREVIEW1, PREVIEW3, 0, 0);
				 if (portrate) my_blit(PREVIEW1, 0, ddyx, x1, y1, x2 - x1, y2 - y1);  //portate
				 else my_blit(PREVIEW1, ddxy, ddyx, x1, y1, x2 - x1, y2 - y1);  //landscape

			 }
			 else
			 {
				 if (portrate) my_blit(PREVIEW3, 0, 0, x1, y1, x2 - x1, y2 - y1);  //portrate
				 else my_blit(PREVIEW3, ddxy, 0, x1, y1, x2 - x1, y2 - y1);  //landscape
			 }
		 }
		 else if (Get_Check_Button(&printer_dlg, BUT_INVERSION) && (!Get_Check_Button(&printer_dlg, BUT_ROTATION)))
		 {
			 stretch_blit((BITMAP*)second_screen, PREVIEW, 0 /*iXpk*/, iYpk, pXk - pXp - 2*iXpk + 1, pYp - pYk - 2*iYpk + 1, 0, 0, x2 - x1, y2 - y1); ////
			 if (gray) gray_bitmap(PREVIEW, PREVIEW, PREVIEW->w /*ddx*/, PREVIEW->h /*ddy*/, gray_sat);
			 else if (bw) gray_bitmap(PREVIEW, PREVIEW, PREVIEW->w /*ddx*/, PREVIEW->h /*ddy*/, gray_sat /*0*/);
			 rotate_sprite(PREVIEW2, PREVIEW, 0, 0, itofix(128));

			 if (Get_Check_Button(&printer_dlg, BUT_REFLECTION))
			 {
				 draw_sprite_v_flip(PREVIEW, PREVIEW2, 0, 0);
				 my_blit(PREVIEW, 0, 0, x1, y1, x2 - x1, y2 - y1);
			 }
			 else  my_blit(PREVIEW2, 0, 0, x1, y1, x2 - x1, y2 - y1);
		 }
		 else if (Get_Check_Button(&printer_dlg, BUT_INVERSION) && (Get_Check_Button(&printer_dlg, BUT_ROTATION)))
		 {
			 stretch_blit((BITMAP*)second_screen, PREVIEW1, 0 /*iXpk*/, iYpk, pXk - pXp - 2*iXpk + 1, pYp - pYk - 2*iYpk + 1, 0, 0, y2 - y1, x2 - x1); ////
			 if (gray) gray_bitmap(PREVIEW1, PREVIEW1, PREVIEW1->w /*ddx*/, PREVIEW1->h /*ddy*/, gray_sat);
			 else if (bw) gray_bitmap(PREVIEW1, PREVIEW1, PREVIEW1->w /*ddx*/, PREVIEW1->h /*ddy*/, gray_sat /*0*/);
			 rotate_sprite(PREVIEW3, PREVIEW1, 0, 0, itofix(192));

			 if (Get_Check_Button(&printer_dlg, BUT_REFLECTION))
			 {
				 draw_sprite_v_flip(PREVIEW1, PREVIEW3, 0, 0);
				 my_blit(PREVIEW1, 0, 0, x1, y1, x2 - x1, y2 - y1);
			 }
			 else  my_blit(PREVIEW3, 0, ddyx, x1, y1, x2 - x1, y2 - y1);
		 }
		 else //no rotation no inversion
		 {
			 stretch_blit((BITMAP*)second_screen, PREVIEW, 0 /*iXpk*/, iYpk, pXk - pXp - 2*iXpk + 1, pYp - pYk - 2*iYpk + 1, 0, 0, ddx, ddy);
			 if (gray) gray_bitmap(PREVIEW, PREVIEW, PREVIEW->w /*ddx*/, PREVIEW->h /*ddy*/, gray_sat);
			 else if (bw) gray_bitmap(PREVIEW, PREVIEW, PREVIEW->w /*ddx*/, PREVIEW->h /*ddy*/, gray_sat /*0*/);
			
			 if (Get_Check_Button(&printer_dlg, BUT_REFLECTION))
			 {
				 draw_sprite_v_flip(PREVIEW2, PREVIEW, 0, 0);
				 my_blit(PREVIEW2, 0, 0, x1, y1, x2 - x1, y2 - y1);
			 }
			 else
			 {
				 my_blit(PREVIEW, 0, 0, x1, y1, x2 - x1, y2 - y1);
			 }
		 }
		 	
		 *real_x1 = x1; *real_x2 = x2;
		 *real_y1 = y1; *real_y2 = y2;

		 destroy_bitmap(PREVIEW);
		 destroy_bitmap(PREVIEW1);
		 destroy_bitmap(PREVIEW2);
		 destroy_bitmap(PREVIEW3);
	 }

	 Restore_View_Preview(); 
	 if (!snap_)krok_s = jednostkiY(1);

	 return;
 }


 void redraw_preview(int dark)
	 /*-------------------*/
 {
	 was_dialog = TRUE;

	 redraw_again = FALSE;
	 redraw_cur_preview(TRUE, dark);
	 if (redraw_again == TRUE)
	 {
		 redraw_again = FALSE;
		 redraw_cur_preview(TRUE, dark);
	 }
	 komunikat(0);
 }

void redraw(void)
/*-------------------*/
{
  record_keyimages();
  was_dialog=TRUE;
  TTF_redraw=FALSE;
  redraw_again=FALSE;
  redraw_cur (TRUE);
  if (redraw_again==TRUE) 
  {
	  redraw_again=FALSE;
	  redraw_cur (TRUE);
  }
  enforce_redraw = FALSE;
  if (do_not_draw_keys==FALSE) 
 	 restore_keyimages();

    ////??????
    ////set_mouse_range((int)pXp+50,(int)pYk+50,(int)pXk-50,(int)pYp-50);
    ////set_mouse_speed(25, 25);
}


void ClearWindow(void)
{ ClearScreen();
  ramka(0);
  ramka(1);
  grid_on();
  pozycja_kursora();
}

static long mv_sh_ms (double dmv) ;

long l_rX, l_rY, l_krok_s; //, t_krok_s ;	/*drag bloku*/


static void cursor_lrtb(double dx, double dy)
{
  double Xl, Yl ;
  static double rX = 0, rY = 0 ;
  int ret;


  Xl = X + dx + rX ;
  if (X == Xmax)
  {
    if (0 != fmod (Xmax, krok_s))
    {
      if (krok_s == krok_g)
      {
	Xl += -fmod (Xmax, krok_g) + krok_g ;
      }
      else
      {
	Xl += krok_s - fmod (Xmax, krok_s) ;
      }
    }
  }
  rX = fmod (Xl, krok_s) ;
  Xl -= rX ;
  l_rX =  mv_sh_ms (rX) ;
  Yl = Y + dy + rY ;
  if (Y == Ymax)
  {
    if (0 != fmod (Ymax, krok_s))
    {
      if (krok_s == krok_g)
      {
	Yl += -fmod (Ymax, krok_g) + krok_g ;
      }
      else
      {
	Yl += krok_s - fmod (Ymax, krok_s) ;
      }
    }
  }
  rY = fmod (Yl, krok_s) ;
  Yl -= rY ;
  l_rX =  mv_sh_ms (rY) ;

  ////if (!options1.uklad_izometryczny)  //TTO DO FOR ISOMETRIC SYSTEM

  l_krok_s = krok_s * DokladnoscF * skala ;

  ////else  //isometric
  ////{
  ////    double cart_krok_s = krok_s * DokladnoscF * skala;
  ////    double l_krok_d, t_krok_d;
  ////    ret=isometric_vector_to_cartesian(cart_krok_s, cart_krok_s, &l_krok_d, &t_krok_d);
  ////    l_krok_s=(long)l_krok_d;
  ////    t_krok_s=(long)t_krok_d;
  ////}

  if (kasowanie_licznikow==TRUE)
   {
     Xl=X;
     Yl=Y;
     kasowanie_licznikow=FALSE;
   }  
  if (Xl > Xmax) Xl = Xmax ;
  else if (Xl<Xmin) Xl = Xmin ;
  if (Yl > Ymax) Yl = Ymax ;
  else if (Yl < Ymin) Yl = Ymin ;
  X = Xl ; Y = Yl ;

   if (X < Xp - PREC_DOUBLE)
    {
     if (Auto_Pan == TRUE)
     {
       Xp_Yp_to_left () ; //cursor moving to the left
       redraw () ;
     }
      else
       {
        if (X < Xp) X = Xp ;
        else
        if (X > Xp + X2d) X = Xp + X2d ;
        if (Y < Yp) Y = Yp ;
        else
        if (Y > Yp + Y2d) Y = Yp + Y2d ;
       }
    }
   else if (X > Xp + X2d + PREC_DOUBLE)
    {
     if (Auto_Pan == TRUE)
     {
       Xp_Yp_to_right () ;
       redraw () ;
     }
      else
       {
        if (X < Xp) X = Xp ;
        else
        if (X > Xp + X2d) X = Xp + X2d ;
        if (Y < Yp) Y = Yp ;
        else
        if (Y > Yp + Y2d) Y = Yp + Y2d ;
       }
    }
    else if (Y < Yp - PREC_DOUBLE)
     {
      if (Auto_Pan == TRUE)
      {
        Xp_Yp_to_down () ;
        redraw () ;
      }
      else
       {
        if (X < Xp) X = Xp ;
        else
        if (X > Xp + X2d) X = Xp + X2d ;
        if (Y < Yp) Y = Yp ;
        else
        if (Y > Yp + Y2d) Y = Yp + Y2d ;
       }
     }
     else if (Y > Yp + Y2d + PREC_DOUBLE)
     {
      if (Auto_Pan == TRUE)
      {
        Xp_Yp_to_up () ;
        redraw () ;
      }
      else
       {
        if (X < Xp) X = Xp ;
        else
        if (X > Xp + X2d) X = Xp + X2d ;
        if (Y < Yp) Y = Yp ;
        else
        if (Y > Yp + Y2d) Y = Yp + Y2d ;
       }
     }
      else
      {
       if (X < Xp) X = Xp ;
       else
       if (X > Xp + X2d) X = Xp + X2d ;
       if (Y < Yp) Y = Yp ;
       else
       if (Y > Yp + Y2d) Y = Yp + Y2d ;
      }
}


void Cur_offd(double x,double y)
{ mvcurb.aktoff=1;
  CUR_OFF(x,y);
  mvcurb.aktoff=0;
  Cur_State_On = FALSE;
}

void Cur_ond(double x,double y)
{ mvcurb.mvc=0;
  mvcurb.akton=1;
  CUR_ON(x,y);
  mvcurb.akton=0;
  Cur_State_On = TRUE;
}

void get_posXY(double *pozx, double *pozy)
{
    *pozx=X;
    *pozy=Y;
}

void CURON(void)
{
    CUR_ON(X,Y);
}

void set_posXY(double pozx, double pozy)
{    int WspX_, WspY_;
    //Odczyt_licznikow();
    X=pozx;
    Y=pozy;
#ifndef ALLEGRO5
    //Odczyt_licznikow();
#else
    do {
        get_mouse_mickeys(&WspX_, &WspY_);
    } while ((WspX_!=0) || (WspY_!=0));
#endif
}

int get_cursor_posX(void)
{
 return (pikseleX0(X)+pXp);
}

int get_cursor_posY(void)
{
 return (pikseleY0(Y)+pYk);
}


static void  mvcur(double dx, double dy)
{ double X0,Y0,Xp0,Yp0;
  X0=X; Y0=Y;
  Xp0=Xp; Yp0=Yp;


      if (snap_changed==TRUE)
      {
           large_step();
           snap_changed=FALSE;
      }

  cursor_lrtb(dx,dy);
  if ((X0!=X) || (Y0!=Y))
  {
    if ((Xp0==Xp) && (Yp0==Yp))
    {
      Cur_offd(X0,Y0);
      Cur_ond(X,Y);
      pozycja_kursora();
    }
  }
}

void mvcurp(double dx, double dy)
{ double X0=X,Y0=Y,Xp0=Xp,Yp0=Yp;

 
  X+=dx;
  if(X>Xmax)X=Xmax;
  else if(X<Xmin) X=Xmin;
  Y+=dy;
  if(Y>Ymax)Y=Ymax;
  else if(Y<Ymin) Y=Ymin;
  if ((X<Xp) || (X>Xp+X2d) || (Y<Yp) || (Y>Yp+Y2d))
   { Xp_Yp();
	 redraw();
   }
  if ((X0!=X) || (Y0!=Y))
   { if ((Xp0==Xp) && (Yp0==Yp))
      { Cur_offd(X0,Y0);
	Cur_ond(X,Y);
	pozycja_kursora();
      }
   }
}

static int  cursor_l(void)
{ mvcur(-krok_s,0);
  return 0;
}

static int  cursor_r(void)
{ mvcur(krok_s,0);
  return 0;
}

static int  cursor_t(void)
{ mvcur(0,krok_s);
  return 0;
}

static int  cursor_b(void)
{ mvcur(0,-krok_s);
  return 0;
}

static long mv_sh_ms (double dmv)
{
  long lmv ;

  if (krok_s > 10)
  {
    dmv /= krok_s / 10;
  }
  dmv *= DokladnoscF * skala;
  lmv = dmv ;
  return lmv ;
}

double mv_ms_sh (int mv)
{ double dmv ;
  dmv = mv ;
  dmv /= DokladnoscF * skala;

  return dmv ;
}

static void mvcur_ (int dx, int dy)
{
  mvcur (mv_ms_sh (dx), mv_ms_sh (dy)) ;
}

void d_2d(void)
{ Ds_x=DokladnoscF*skala*sk_x;
  Ds=DokladnoscF*skala;
  X2d=jednostkiX(maxXr);
  Y2d=jednostkiY(maxYr);
  y_pikseleY0=min(Y2d,Y_max)*Ds;
  Xd=X2d*pan_dxy;  /*0.5*/ /*!!!!!!!!!!!*/
  Yd=Y2d*pan_dxy;  /*0.5*/ /*!!!!!!!!!!!*/
  Xd_=X2d*0.5;
  Yd_=Y2d*0.5;
}

void d_2d_p(void)
{ Ds_x=DokladnoscF*skala*sk_x;
    Ds=DokladnoscF*skala;
    X2d=jednostkiX(maxXr);
    Y2d=jednostkiY(maxYr);
    y_pikseleY0=min(Y2d,Y_max)*Ds;
    Xd=X2d*pan_dxy;  /*0.5*/ /*!!!!!!!!!!!*/
    Yd=Y2d*pan_dxy;  /*0.5*/ /*!!!!!!!!!!!*/
    Xd_=X2d*0.5;
    Yd_=Y2d*0.5;
}


#ifdef INTER
void User(void)
{ char zb[51]="";
  if (!read_esc(zb,50,44)) return;
  CUR_OFF(X,Y);
  ladujexe(zb,150000l);
  CUR_ON(X,Y);
}
#endif

static void BreakOut(int sig)
{
  sig = sig;
  closegraph();
  printf("break\n");
  exit(0);
}


static void Float(int sig)
{
  sig = sig;
  Test_Obiekts_Report () ;

  closegraph();
  printf("float\n");
  exit(0);
}

#ifdef INTER
int ladujexe(char *nz,long size)
{ char seg[5],off[5],st[50];
  int stat;
  unsigned free,segp;
  long l;

  size = size;
  ltoa1(FP_SEG(c),seg,10);
  ltoa1(FP_OFF(c),off,10);

  if(l<0) { ErrList(18);;return -1; }

  errno=0;
  procesp=1;
  stat=spawnl(P_WAIT,nz,nz,&seg,&off,NULL);
  if(stat==2) Break(0);
  signal(SIGINT,Break);
  signal(SIGFPE,Float);
  procesp=0;

  if(stat==-1 && errno<sys_nerr)
   { ErrListStr(sys_errlist[errno]);
     /*komunikat(14);*/
   }
  return stat;
}
#endif

void dzwonek(void)
{ Dzwonek=0;
  if(!Dzwonek) return;

}

void Bell (void)
/*-------------*/
{
  return;
}


void near uaktualnij_pola(void)
{ char sk[50],st[21];
  double d;
  
  if(grid_) strcpy(sk,"T");
  else strcpy(sk,"N");
  menu_par_new((*mParametrym.pola)[2].txt,sk);
 
  DF_to_String (sk, "%-6.2f", milimetryob (krok_g), 6) ;
  menu_par_new ((*mParametrym.pola)[1].txt, sk) ;

  if(1/skala>999) { strcpy(sk,">999"); }
  else if(1/skala<.01) { strcpy(sk,"<.01");  }
  else
   { d=floor(skala*100.+0.5)*.01;

	dtoa(d,sk,6,3);
   

   }
  menu_par_new((*menup.pola)[1].txt,sk);

  if (!snap_) strcpy(sk, "Min") ;
  else
  {
    DF_to_String (sk, "%-6.2f", milimetryob (krok_s), 6) ;
  }
  menu_par_new((*mParametrym.pola)[0].txt,sk);

  if(orto)
	strcpy(st, "T");
  else
   strcpy (st,"N");
  menu_par_new((*mParametrym.pola)[3].txt,st);
}

#define mpini ( void (*) (int n)) nooop

int read_c(int _f, void *buf, int l_char)
/*------------------------------------------*/
{
char buf_c[512];
if (read(_f,&buf_c,l_char)!=l_char) return l_char;
memmove(buf,&buf_c,l_char); 
return l_char;
}

int read_i(int _f, int *int_buf, int l_char)
{
	unsigned char c1, c2; // c3;
  int int_buf0;
  if (read (_f, &c1, 1) != 1) return 0 ;
  if (read (_f, &c2, 1) != 1) return 1 ;
  int_buf0= c2 * 256 + c1;
  if (int_buf0>32767) 
   {
   int_buf0-=65536;
   }
  memmove(int_buf,&int_buf0,sizeof(int)); 
  return 2;
}    

int param_conf(void)
/*------------------------*/
{
  int f, f1;
  long roz;

  f=open("COLORS.CNF",O_RDONLY|O_BINARY);
  if (f==-1)
   {
	 return 0;
   }
  roz=filelength(f);
   
  read(f, &kolory, sizeof(KOLORY));

  if (getmaxcolor () == 1)   /*tymczasowe*/
  {
    kolory.paperk_ini = 1;
    kolory.inkk_ini = 0;
  }
  else
  {
    kolory.paperk_ini = LIGHTGREEN;
    kolory.inkk_ini =  BLACK;
  }

  f1=close(f);

  Kolory_Paper=kolory.paper;
  return 1;
}

static int param_conf_aster(void)
/*------------------------*/
{
	int f, f1;
  long roz;

  f=open("COLORSA.CNF",O_RDWR|O_BINARY);
  if (f==-1)
   {
     return 0;
   }
  roz=filelength(f);
   
  read(f, &kolorya, sizeof(KOLORY_ASTER));
  
  f1=close(f);
  return 1;
}

static int param_conf_profile(void)
/*------------------------*/
{
	int f, f1;
  long roz;

  f=open("COLORSP.CNF",O_RDWR|O_BINARY);
  if (f==-1)
   {
     return 0;
   }
  roz=filelength(f);
   
  read(f, &koloryp, sizeof(KOLORY_PROFILI));
  
  f1=close(f);
  return 1;
}

void monitor_parametry(int Driver)
/*-------------------------------------*/
{
  Driver = Driver;      	/*!!!!*/
  maxX=getmaxx();
  maxY=getmaxy();
  maxXr=maxX-xp-xk;  
  maxYr=maxY-yp-yk;  

  if ( param_conf () && getmaxcolor () > 1)  return;

  if (getmaxcolor () == 1)
  {
    kolory.paperk_ini = 1;
    kolory.inkk_ini = 0;
  }
  else
  {
    kolory.paperk_ini = LIGHTGREEN;
    kolory.inkk_ini =  BLACK;
  }


 if (getmaxcolor () == 1)
  { kolory.cur=1;		/*WHITE;*/
	kolory.blok=1;		/*WHITE;*/
	kolory.ink=1;		/*//WHITE;*/
    kolory.border=1;		/*WHITE;*/
    kolory.paper=0;		/*BLACK;*/
    kolory.inkk=0;		/*BLACK;*/
    kolory.paperk=1;		/*WHITE;*/
    kolory.inkm=1;		/*WHITE;*/
    kolory.borderm=1;		/*WHITE;*/
    kolory.paperm=0;		/*BLACK;*/
  }
 else
  {
    kolory.cur=3;
	kolory.blok=2;
    kolory.ink=10;
	kolory.border=14;
    kolory.paper=3;
    kolory.inkk=12;
    kolory.paperk=1;
    kolory.inkm=7;
    kolory.borderm=7;
    kolory.paperm=1;
  }
}


void monitor_parametry_again(int Driver)
/*-------------------------------------*/
{
  Driver = Driver;      	/*!!!!*/
  maxX=getmaxx();
  maxY=getmaxy();
  maxXr=maxX-xp-xk;
  maxYr=maxY-yp-yk;
		
  if (getmaxcolor () > 1)  return;

  if (getmaxcolor () == 1)   /*tymczasowe*/
  {
    kolory.paperk_ini = 1;
    kolory.inkk_ini = 0;
  }
  else
  {
    kolory.paperk_ini = LIGHTGREEN;
    kolory.inkk_ini =  BLACK;
  }


 if (getmaxcolor () == 1)
  { kolory.cur=1;		/*WHITE;*/
	kolory.blok=1;		/*WHITE;*/
	kolory.ink=1;		/*//WHITE;*/
    kolory.border=1;		/*WHITE;*/
    kolory.paper=0;		/*BLACK;*/
    kolory.inkk=0;		/*BLACK;*/
    kolory.paperk=1;		/*WHITE;*/
    kolory.inkm=1;		/*WHITE;*/
    kolory.borderm=1;		/*WHITE;*/
    kolory.paperm=0;		/*BLACK;*/
  }
 else
  {
    kolory.cur=3;
	kolory.blok=2;
    kolory.ink=10;
	kolory.border=14;
    kolory.paper=3;
    kolory.inkk=12;
    kolory.paperk=1;
    kolory.inkm=7;
    kolory.borderm=7;
    kolory.paperm=1;
  }
}


static void var_rys(void)
{ ZMIENNE zw=Defzmwym;
  LAYER war=Defwar;
  int i;

  skala=1.0;
  krok_s= .5 ; krok_g = 2 ; grid_=0;snap_=0; X_Y_kursora=1;
  FormatX=210;FormatY=297;SkalaF=1;Jednostki=1;rpp=0;skala_p_x=0.2;typ_profilu=0;
  znacznik_aplikacji=0;
  DokladnoscF=4;
  TextG.wysokosc = plt_d_to_jednostki_d (3) ;
  Xmin=Ymin=0;
  X_max=jednostkiplt(FormatX);
  Y_max=jednostkiplt(FormatY);
  Xmax=X_max+Xmin;
  Ymax=Y_max+Ymin;
  Xp=Xmin; Yp=Ymin;
  X=0;Y=0;
  Current_Layer=0;
  No_Layers = 1;
  df__Point_Size = 2.5 ;
  b__Point_View = TRUE ;
  df__Trace_Width = 1 ;
  df__Sketch_Dist_Max = 1.0 ;

  memmove(&zmwym,&zw,sizeof(ZMIENNE));  /*zmwym*/
  for(i=0; i<MAX_NUMBER_OF_LAYERS;i++)
  {
    memmove(&(Layers[i]),&war,sizeof(LAYER)); /*Layers*/
    strcpy (Layers [i].name, "");
  }
  Layers [0].len_name = 1;
  strcpy (Layers [0].name, "0");
}


//////////////////////////
void re_set_params(void)
{
  setaspectratio(10000, min (32000., 10000. / sk_x));
  d_2d () ;
  if (Xp + X2d > Xmax) Xp = Xmax - X2d ;	/*na wypadek zmiany rozdzielczosci monitora*/
  if (Xp < Xmin) Xp = Xmin ;
  if (Yp + Y2d > Ymax) Yp = Ymax - Y2d ;
  if (Yp < Ymin) Yp = Ymin ;
  Xk_Yk () ;
  pXp=pikseleX(Xp);
  pYp=pikseleY(Yp);
  if(!snap_)krok_s=jednostkiY(1);
  uaktualnij_pola();
  Ini_Global_Object ();
}

//////////////////////////


int czytaj_rysunek0(char *rys, BOOL b_err, int redraw_view)
//--------------------------------------------------------
{
  int k = 1 ;
  int ret = 0 ;
  int kk ;
  int ijk;
  BOOL re_read;


re_read_zb:

  bitmap_exist = FALSE;
  bitmap_png_exist = FALSE;
  bitmap_pattern_exist = FALSE;
  solid_translucent_exist = FALSE;
  bitmap_on_front_exist = FALSE;

  set_priv_prn_window(0.0, 0.0, 0.0, 0.0);

  for (ijk=0; ijk<256; ijk++)  
   {
	   Warstwy_On_PCX[ijk].check=0;
       Warstwy_On_PCX[ijk].on=0;

	   Warstwy_On_Solids[ijk].check = 0;
	   Warstwy_On_Solids[ijk].on = 0;
   }
  bitmap_view=TRUE;
  re_read=FALSE;
  kk = stricmp(rys, RYSUJ$1);
  if (kk==0)
  {
  }
   else
    {
     rpp=1;
     typ_profilu=0;
     skala_p_x=0.2;
     znacznik_aplikacji=0;
    }

  if (rys [0] != '\0')
  {

	komunikat0(128);

    ret = k = czytaj_zbior(rys, b_err, TRUE, TRUE);
    if (k==IO_RET_READ_AGAIN)
     {
      re_read=TRUE;
      k=0;
     } 
    if ((k) && (k!=5))
    {
      if (k == 1 && b_err == TRUE)
      {
	ErrList (47) ;
      }
      if (s__prot.b_prot == TRUE)
      {
	    k = czytaj_zbior (s__prot.sz_prot, FALSE, TRUE, TRUE) ;
	    kolory.paper=Kolory_Paper;
      }
    }
  }
  else
  {
    if (s__prot.b_prot == TRUE)
    {
      k = czytaj_zbior (s__prot.sz_prot, FALSE, TRUE, TRUE) ;
	  kolory.paper=Kolory_Paper;
    }
  }
  if (( k ) && (k!=5))
  {
    Ini_Spec_Param () ;
    var_rys();  		/*inicjacja zmiennych globalnych */
    kolory.paper=Kolory_Paper;
  }
  setaspectratio(10000, min (32000., 10000. / sk_x));
  d_2d () ;
  if (Xp + X2d > Xmax) Xp = Xmax - X2d ;	/*na wypadek zmiany rozdzielczosci monitora*/
  if (Xp < Xmin) Xp = Xmin ;
  if (Yp + Y2d > Ymax) Yp = Ymax - Y2d ;
  if (Yp < Ymin) Yp = Ymin ;
  Xk_Yk () ;
  pXp=pikseleX(Xp);
  pYp=pikseleY(Yp);

  reset_background();

  if(!snap_)krok_s=jednostkiY(1);
  uaktualnij_pola();

  if (redraw_view==1)
   {
    setfillstyle_(EMPTY_FILL,0);
   }
  if (Get_Buf_Mak_Size () > 2)  mbufmx = bufor_makra + Get_Buf_Mak_Size () - 2 ;
  else  mbufmx = NULL;
  Ini_Global_Object ();
  if (re_read==TRUE)
   {
   /*zapis rysunku */
    komunikat0(107);
    k=pisz_zbior(rys, FALSE, 1) ;
    if ( k )  { ErrList(8); }
    zwolnienie_pamieci();
    komunikat0(0);
    goto re_read_zb;
   }
  if (redraw_view==1) redraw();
  Save_View();
  return ret;
}

long int Wyslij_par_ekranu(int x)
 {
  if (!x)   return pXp; else
  if (x==1) return pXk; else
  if (x==2) return pYp; else
  if (x==3) return pYk; else
	        return pXp;
 }


int czytaj_rysunek(char *rys, BOOL b_err)
/*-------------------------------------*/
{ int read_err;
  read_err=czytaj_rysunek0(rys, b_err, 1);
#ifdef ALLEGRO5
  flip_screen();
#endif
  return read_err;
}

int czytaj_rysunek1(char *rys, BOOL b_err)
/*--------------------------------------*/
{ int read_err;
  read_err=czytaj_rysunek0(rys, b_err, 0);
  return read_err;
}

/*zaklada sie ze InitDane dokonuje pierwszej alokacji w buforz pamieci*/
void InitDane(void)
{ NAGLOWEK nag;
  if((dane=NewBuffer(&dane0_Data[DRAWING_NUMBER]))==NULL)
  {
    Quick_Exit (97) ;
  }
  nag.obiekt=Okoniec;
  nag.atrybut=Anormalny;
  nag.n=0;
  dane_size=0;
  if(dodaj_obiekt(NULL,&nag)==NULL)
  {
    Quick_Exit (18) ;
  }
  Change = FALSE;
  Set_Auto_Backup (FALSE);
}

static BOOL b_buff_realloc = FALSE ;

BOOL Buff_Dan_Realloc (void)
/*-------------------------*/
{
  return b_buff_realloc ;
}

int SetBufferDaneSize (long l)
{
  long SizeP,SizeK;
  int ret=1;
  char *new_data;

aaa:

    if (DRAWING_NUMBER>32)
        DRAWING_NUMBER=0;

  SizeP=GetBufferSize(dane0_Data[DRAWING_NUMBER]);
  b_buff_realloc = FALSE ;
  if( SizeP < l || SizeP > l+DelDaneSize )
  {
     b_buff_realloc = TRUE ;
     SizeK=DelDaneSize*(1+l/DelDaneSize);
     if( SizeK > SizeP )
     {

     if(!SetBufferSize(dane0_Data[DRAWING_NUMBER],SizeK))
	{
      new_data=ReInitBuffers (&dane0_Data[DRAWING_NUMBER], SizeK);
      if (new_data!=NULL)
	  {
	    goto aaa ;
	  }
	  ret=0;
	  b_buff_realloc = FALSE ;
	}
	else
	{
	   movmem_((void*)dane,(void*)(dane+(SizeK-SizeP)),dane_size);
	   dane+=SizeK-SizeP;
	   Change_Global_Ptr (SizeK - SizeP) ;
	}
     }
     else
     {
	movmem_((void*)dane,(void*)(dane+(SizeK-SizeP)),dane_size);
	SetBufferSize(dane0_Data[DRAWING_NUMBER],SizeK);
	dane+=SizeK-SizeP;
	Change_Global_Ptr (SizeK - SizeP) ;
     }
  }
  if (ret == 1)
  {
    Set_Error_No (NO_ERR) ;
  }
  else
  {
    Set_Error_No (ERR_NOMEM) ;
  }
  return ret;
}


#define IGNORE  0
#define RETRY   1
#define ABORT   2


int error_win(char *msg)
/*---------------------*/
{
   int retval = IGNORE, key;
   ErrListStr (msg);
   key = Get_Legal_Key ("kKPp");
   if (key == 'k' || key == 'K')
   {
       retval = RETRY;
   }
   else if (key == 'p' || key == 'P')
   {
      retval = IGNORE;
   }
   ClearErr ();
   return retval;
}


//#pragma warn -par

int handler(int errval,int ax,int bp,int si)
/*----------------------------------------*/
{
   static char  msg[80];
   int drive;

   static char *err_msg[] = {
    "write protect",
    "unknown unit",
    "dysk nie gotowy",
    "unknown command",
    "data error (CRC)",
    "bad request",
    "seek error",
    "unknown media type",
    "sector not found",
    "printer out of paper",
    "write fault",
    "read fault",
    "general failure",
    "reserved",
    "reserved",
    "invalid disk change",
};


   if (ax < 0)
   {
      ErrList (43);
    return ABORT;
   }
   drive = ax & 0x00FF;

  sprintf(msg, "Błąd urządzenia %c ! Kontynuować / Przerwać: ", 'A' + drive); 	   

   return ABORT;
}
//#pragma warn +par

static int  nooop1 (void)
{return 0;}

static BOOL initial_sel_test (void* ptr_ob)
/*---------------------------------------*/
{
  ptr_ob = ptr_ob ;
  return TRUE ;
}

static BOOL intersect_add_test (void* ptr_ob1, void* ptr_ob2)
/*----------------------------------------------------------*/
{
  ptr_ob1 = ptr_ob1 ;
  ptr_ob2 = ptr_ob2 ;
  return TRUE ;
}

static int restore_graphics(void)
{

int gdriver, gmode;


set_BGI_mode_whc(&gdriver,&gmode,getmaxx()+1,getmaxy()+1,255);

setrgbdefaults(); 
redraw();
return 0;
}

void serv134_135(BOOL go_nogo)
{
	if (go_nogo == FALSE)
	{
		serv134 = SERV[134];
		serv135 = SERV[135];
		SERV[134] = nooop1;
		SERV[135] = nooop1;
	}
	else
	{
		SERV[134] = serv134;
		SERV[135] = serv135;
	}
}

int push_Xp_Yp_to_left(void)
{
	if (Xp > Xmin)
	{
		Xp_Yp_to_left_fix(); //82
		redraw();
	}
	return 0;
}

int push_Xp_Yp_to_right(void)
{
	if ((Xp + X2d) < Xmax)
	{
		Xp_Yp_to_right_fix(); //83
		redraw();
	}
	return 0;
}

int push_Xp_Yp_to_up(void)
{
	if ((Yp + Y2d) < Ymax)
	{
		Xp_Yp_to_up_fix(); //84
		redraw();
	}
	return 0;
}

int push_Xp_Yp_to_down(void)
{
	if (Yp > Ymin)
	{
		Xp_Yp_to_down_fix(); //85
		redraw();
	}
	return 0;
}

void standard_func(void)
/*---------------------*/
 {
  SERV[1] = NULL;
  
  SERV[47] = restore_graphics ;  /*ALT-V*/
  SERV[72] = cursor_t;
  SERV[75] = cursor_l;
  SERV[77] = cursor_r;
  SERV[80] = cursor_b;
  SERV[82] = menupoz;
  SERV[95] = Test_Obiekts_Report ;

  SERV[4] =  nooop1;  //JumpW_;          //Ctrl-D
  
  SERV[5] = koniecS_;         //E
  SERV[14] = koniec_;         //N
  SERV[13] = srodek_;         //M
  SERV[9] = przeciecie_;     //I
  SERV[3] = centrum_;         //C
  SERV[16] = prostopadle_;    //P
  SERV[20] = stycznie_;        //T
  SERV[2] = najblizszy_;     //A
  SERV[15] = Point_Sel_;       //O
  SERV[24] = JumpB_;    //X
  SERV[25] = JumpW_;    //Y
  SERV[12] = JumpL_;    //L
 
 
  SERV[6] = nooop1;  //JumpB_;           //F

  SERV[57] = nooop1;

  SERV[58] = Kopiuj_Ctrl_R; /*Ctrl_R*/


  SERV[59] = (int(*)(void)) help ;  /*F1*/

  SERV[60] = Edit_Text_Tab_F2;   /*F2*/   //EDYCJA TEKSTU + TAB
  SERV[61] = Edit_Text_F3;   /*F3*/   //EDYCJA TEKSTU
  SERV[62] = Edit_Text_F3;   /*F4*/   //EDYCJA TEKSTU
  SERV[63] = Usun_F5; //delete_object_F5;   /*F5*/   //USUN OBIEKT
  SERV[64] = undo_blok_F6; //undelete_object_F6; /*F6*/   //PRZYWROC OBIEKT

  SERV[65] = delay_1;     	/*F7*/  //delay_2, delay_3
  SERV[66] = Change_Orto ;      /*F8*/

  SERV[67] = nooop1;  /*F9*/
  SERV[68] = nooop1;  /*F10*/
  SERV[69] = (int (*)(void)) Expand_flex; //Free_Mouse; /*F11*/

  SERV[70] = (int (*)(void)) Set_Layers; /*Ctrl-L or Ctrl_W*/

  SERV[84] = small_large_step; //osnap_; //Write_screen_pcx; /*Ctrl PrintScreen*/  //nooop1; //osnap_;		  /*Shift-F1*/
  SERV[85] = koniecS_;  // nooop1;  /*Shift-F2*/
  SERV[86] = koniec_; //small_step; //koniec_;     /*Shift-F3*/
  SERV[87] = srodek_; //large_step; //srodek_;     /*Shift-F4*/
  SERV[88] = przeciecie_; //osnap_; //przeciecie_; /*Shift-F5*/
  SERV[89] = centrum_; //koniec_; //centrum_;    /*Shift-F6*/
  SERV[90] = przeciecie_; //prostopadle_;/*Shift-F7*/
  SERV[91] = prostopadle_; //stycznie_ ;  /*Shift-F8*/
  SERV[92] = najblizszy_; /*Shift-F9*/
  SERV[93] = Point_Sel_;  /*Shift-F10*/
  				
  SERV[104] = kolorL_;			/*Alt-F1*/
  SERV[105] = typ_linii_;		/*Alt-F2*/
  SERV[106] = grubosc_linii_;		/*Alt-F3*/
  SERV[107] = Set_Layers_;		/*Alt-F4*/
  SERV[108] = Point_Measure_;		/*Alt-F5*/
  SERV[109] = Dist_Measure_;		/*Alt-F6*/
  SERV[110] = Angle_Measure1_;		/*Alt-F7*/
  SERV[111] = Wez_kolor_; //Calculator_Measure_; 	/*Alt-F8*/
  SERV[112] = Wez_typ_;			/*Alt-F9*/
  SERV[113] = Wez_warstwe_; 	        /*Alt-F10*/
  SERV[114] = Hide_Layers_; //Ukrycie wskazanych warstw /*Alt-F11

  SERV[94] = SkalaW_      ;	/*Ctrl F1   okno*/
  SERV[95] = last_view_   ;	/*Ctrl F2   ostatni*/
  SERV[96] = SkalaE_      ;	/*Ctrl F3   wszystko*/
  SERV[97] = SkalaZC_     ;	/*Ctrl F4   centrum*/
  SERV[98] = SkalaZ1_     ;	/*Ctrl F5   skala 1*/
  SERV[99] = SkalaZ05_    ;	/*Ctrl F6   skala 0.5*/
  SERV[100] = SkalaZ02_   ;	/*Ctrl F7   skala 0.2*/
  SERV[101] = SkalaZ01_   ;	/*Ctrl F8   skala 0.1*/
  SERV[102] =SkalaR_      ;	/*Ctrl F9   odswiezenie*/
  SERV[103] =skala_format_;	/*Ctrl F10   format*/

  SERV[115] = set_lastest_window; //circle_and_rectangle_proc; ///set_decrease_x;	//nooop1;   //RESIZE
  SERV[116] = (int (*)(void)) Expand_flex;  //Free_Mouse;
  SERV[117] = nooop1; //set_decrease_y;	//nooop1;   //RESIZE
  SERV[118] = set_orig_window; //set_increase_y;	//   //RESIZE
  SERV[119] = circle_and_rectangle_proc;   //Ctrl-Home
  SERV[120] = Choose_Layer_1 ;
  SERV[121] = Choose_Layer_2 ;
  SERV[122] = Choose_Layer_3 ;
  SERV[123] = Choose_Layer_4 ;
  SERV[124] = Choose_Layer_5 ;
  SERV[125] = Choose_Layer_6 ;
  SERV[126] = Choose_Layer_7 ;
  SERV[127] = Choose_Layer_8 ;
  SERV[128] = Choose_Layer_9 ;
  SERV[129] = Choose_Layer_10 ;
  SERV[130] = Dec_Layer ;
  SERV[131] = Inc_Layer ;

#ifdef MACOS
  SERV[133] = (int (*)(void)) Expand_flex; //Free_Mouse; /*F11*/
#else
  SERV[133] = (int(*)(void)) help ;   /*F12*/;
#endif
  SERV[134] = (void*)SkalaZ_Minus; //nooop1;
  SERV[135] = (void*)SkalaZ_Plus; //nooop1;

  serv134 = SERV[134];
  serv135 = SERV[135];
  SERV[136] = All_Layers_On_; //nooop1; Ctrl-F11 
  SERV[137] = Reverse_Layers_; //nooop1; Ctrl-F12  
  SERV[138] = nooop1;
  SERV[139] = nooop1;
  SERV[140] = (void*)set_original_window; // set_last_window; // Free_Mouse;

  SERV[141] = push_Xp_Yp_to_left;
  SERV[142] = push_Xp_Yp_to_right;
  SERV[143] = push_Xp_Yp_to_up;
  SERV[144] = push_Xp_Yp_to_down;

  SW2[0] = SERV[4];   //Ctrl-D
  SW2[1] = SERV[5];          //E
  SW2[2] = SERV[14];      //N
  SW2[3] = SERV[13];         //M
  SW2[4] = SERV[9];     //I
  SW2[5] = SERV[3];         //C
  SW2[6] = SERV[16];     //P
  SW2[7] = SERV[20];         //T
  SW2[8] = SERV[2];      //A
  SW2[9] = SERV[15];        //O
  SW2[10] = SERV[24];    //X
  SW2[11] = SERV[25];     //D
  SW2[12] = SERV[12];    //L

  MEDITFUN = nooop1;

  last_mouse_z=mouse_z;
  
 }

void disable_F11(void)
{
    SERV[69] = SERV[116] = nooop1;
}

void enable_F11(void)
{
    SERV[69] = SERV[116] = (int (*)(void)) Expand_flex;
}

void set_yp(void)
 {
  yp=2*ED_INF_HEIGHT + 1;
  maxYr=maxY-yp-yk;
  d_2d () ;
  Xp_Yp () ;
 }

void Ini_Color_Table()
{
  return;
} ;

void change_second_screen(void)
{
  destroy_bitmap((BITMAP *)second_screen);
  destroy_zbuffer(zbuf);
  second_screen=(GrContext *)create_system_bitmap(getmaxx()+1, getmaxy()+1);
  zbuf = create_zbuffer((BITMAP*)second_screen);
  set_zbuffer(zbuf);
    set_clip_rect((BITMAP*)second_screen, 0, 0, getmaxx(), getmaxx());
}

void destroy_second_screen(void)
{
  destroy_bitmap((BITMAP*)second_screen);
}

void create_second_screen(void)
{
  second_screen=(GrContext *)create_system_bitmap(getmaxx()+1, getmaxy()+1);
  zbuf = create_zbuffer((BITMAP *)second_screen);
  set_zbuffer(zbuf);
}

static BITMAP *create_mouse_pointer(char *data)
{
	BITMAP *bmp;
	int x, y;
	int col;

	bmp = create_bitmap(DEFAULT_SPRITE_W, DEFAULT_SPRITE_H);

	for (y = 0; y < DEFAULT_SPRITE_H; y++) {
		for (x = 0; x < DEFAULT_SPRITE_W; x++) {
			switch (data[x + y * DEFAULT_SPRITE_W]) {
			case 1:  col = makecol(255, 255, 255);  break;
			case 2:  col = makecol(0, 0, 0);        break;
			default: col = bmp->vtable->mask_color; break;
			}
			putpixel(bmp, x, y, col);
		}
	}

	return bmp;
}

static BITMAP *create_mouse_pointer32(char *data)
{
	BITMAP *bmp;
	int x, y;
	int col;

	bmp = create_bitmap(DEFAULT_SPRITE_W32, DEFAULT_SPRITE_H32);

	for (y = 0; y < DEFAULT_SPRITE_H32; y++) {
		for (x = 0; x < DEFAULT_SPRITE_W32; x++) {
			switch (data[x + y * DEFAULT_SPRITE_W32]) {
			case 1:  col = makecol(255, 255, 255);  break;
			case 2:  col = makecol(0, 0, 0);        break;
			default: col = bmp->vtable->mask_color; break;
			}
			putpixel(bmp, x, y, col);
		}
	}

	return bmp;
}

#ifdef LINUX
static BITMAP *create_mouse_null(char *data)
{
    BITMAP *bmp;
    int x, y;
    int col;

    bmp = create_bitmap(DEFAULT_SPRITE_NULL_W, DEFAULT_SPRITE_NULL_H);

    for (y = 0; y < DEFAULT_SPRITE_NULL_H; y++) {
        for (x = 0; x < DEFAULT_SPRITE_NULL_W; x++) {
            switch (data[x + y * DEFAULT_SPRITE_NULL_W]) {
                case 1:  col = makecol(255, 255, 255);  break;
                case 2:  col = makecol(0, 0, 0);        break;
                default: col = bmp->vtable->mask_color; break;
            }
            putpixel(bmp, x, y, col);
        }
    }
    return bmp;
}

void hide_x_cursor(void)
{
    set_mouse_sprite(alfa_mouse_null);
}
#endif

void show_x_cursor(void)
{
    set_mouse_sprite(alfa_mouse_sprite);
}


void set_dialog_cursor(int bigsmall)
{
	BIGCURSOR = bigsmall;
	if (BIGCURSOR==2) alfa_mouse_sprite = alfa_mouse_pointer48;
    else if (BIGCURSOR==1) alfa_mouse_sprite = alfa_mouse_pointer32;
	else  alfa_mouse_sprite = alfa_mouse_pointer;

    if (BIGCURSOR==2) alfa_mouse_edit_sprite = alfa_mouse_edit48;
    else if (BIGCURSOR==1) alfa_mouse_edit_sprite = alfa_mouse_edit32;
    else  alfa_mouse_edit_sprite = alfa_mouse_edit;

    if (BIGCURSOR==2) alfa_mouse_busy_sprite = icon_hourglass_mem;
    else if (BIGCURSOR==1) alfa_mouse_busy_sprite = icon_hourglass_mem;
    else  alfa_mouse_busy_sprite = icon_hourglass_mem;

    set_mouse_sprite(alfa_mouse_sprite);
	save_dialog_cursor();
}

void set_menu_cursor(BOOL bar_pointer)
{
    BAR_POINTER = bar_pointer;
    save_menu_cursor();
}

void DoneBuffMacro(void)
{
    if (bufor_makra != NULL) free(bufor_makra);
}

void my_scare_mouse() 
{
    show_mouse(NULL);  //WARNING
    //select_mouse_cursor(MOUSE_CURSOR_NONE);
    ////scare_mouse();  //WARNING
}

void my_unscare_mouse()
{
    show_mouse(screen);  //WARNING
    //select_mouse_cursor(MOUSE_CURSOR_ALLEGRO);
    ////unscare_mouse();  //WARNING
}

void set_cursor_pointer(void)
{
    scare_mouse();
    set_mouse_sprite(alfa_mouse_sprite);
    set_mouse_sprite_focus(0, 0);
    unscare_mouse();
    show_mouse(screen);
}

void set_cursor_pointer_linux(void)
{
    show_os_cursor(MOUSE_CURSOR_NONE);
    //select_mouse_cursor(MOUSE_CURSOR_NONE);
#ifndef MACOS
    disable_hardware_cursor();
#endif
    select_mouse_cursor(MOUSE_CURSOR_ALLEGRO);
}

void set_cursor_pointer__(void)
{
    scare_mouse();
    show_os_cursor(MOUSE_CURSOR_NONE);
    //int ret = get_cursor_info();
#ifndef MACOS
    disable_hardware_cursor();
#endif
    select_mouse_cursor(MOUSE_CURSOR_ALLEGRO);
    set_mouse_sprite(alfa_mouse_sprite);
    unscare_mouse();

}

void set_cursor_edit(void)
{
    scare_mouse();
    set_mouse_sprite(alfa_mouse_edit_sprite);
    set_mouse_sprite_focus(alfa_mouse_edit_sprite->w/2, alfa_mouse_edit_sprite->h/2);
    //if (BIGCURSOR==2) set_mouse_sprite_focus(alfa_mouse_edit_sprite->w/2, alfa_mouse_edit_sprite->h/2);
    //else if (BIGCURSOR==1) set_mouse_sprite_focus(alfa_mouse_edit_sprite->w/2, alfa_mouse_edit_sprite->h/2);
    //else set_mouse_sprite_focus(alfa_mouse_edit_sprite->w/2, alfa_mouse_edit_sprite->h/2);

    unscare_mouse();
    show_mouse(screen);
}

void set_cursor_busy(void)
{
    scare_mouse();
    set_mouse_sprite(alfa_mouse_busy_sprite);
    set_mouse_sprite_focus(alfa_mouse_busy_sprite->w/2, alfa_mouse_busy_sprite->h/2);
    //if (BIGCURSOR==2) set_mouse_sprite_focus(16, 16);
    //if (BIGCURSOR==1) set_mouse_sprite_focus(16, 16);
    //else set_mouse_sprite_focus(16, 16);

    unscare_mouse();
    show_mouse(screen);
}

void set_cursor_edit_linux(void)
{
    //select_mouse_cursor(MOUSE_CURSOR_NONE);
#ifndef MACOS
    enable_hardware_cursor();
#endif
    select_mouse_cursor(MOUSE_CURSOR_EDIT);
    show_os_cursor(MOUSE_CURSOR_EDIT);
}

void set_cursor_edit__(void)
{
    scare_mouse();
    set_mouse_sprite(NULL);
#ifndef MACOS
    enable_hardware_cursor();
#endif
    select_mouse_cursor(MOUSE_CURSOR_EDIT);
    unscare_mouse();
}

void Restore_Pointer(void)
{
    if (BAR_POINTER)
    {
        _free_mouse();
        set_cursor_pointer();
    }
}

void Set_Pointer(void)
{
        _free_mouse();
        set_cursor_pointer();
}

void reini_cursors(BITMAP *small_one, BITMAP *big_one, BITMAP *huge_one)
{
    alfa_mouse_pointer=small_one;
    alfa_mouse_pointer32=big_one;
    alfa_mouse_pointer48=huge_one;

    if (BIGCURSOR==2) alfa_mouse_sprite = alfa_mouse_pointer48;
    else if (BIGCURSOR==1) alfa_mouse_sprite = alfa_mouse_pointer32;
    else  alfa_mouse_sprite = alfa_mouse_pointer;
}

void reini_edit_cursors(BITMAP *small_one, BITMAP *big_one, BITMAP *huge_one)
{
    alfa_mouse_edit=small_one;
    alfa_mouse_edit32=big_one;
    alfa_mouse_edit48=huge_one;

    if (BIGCURSOR==2) alfa_mouse_edit_sprite = alfa_mouse_edit48;
    else if (BIGCURSOR==1) alfa_mouse_edit_sprite = alfa_mouse_edit32;
    else  alfa_mouse_edit_sprite = alfa_mouse_edit;
}

void ini_cursors(void)
{
    
    #ifdef LINUX
        if (alfa_mouse_null!=NULL) destroy_bitmap(alfa_mouse_null);
        alfa_mouse_null = create_mouse_null(alfa_mouse_data_null);
    #endif

      if (alfa_mouse_pointer32!=NULL) destroy_bitmap(alfa_mouse_pointer32);
      if (alfa_mouse_pointer != NULL) destroy_bitmap(alfa_mouse_pointer);
      alfa_mouse_pointer32 = create_mouse_pointer32(alfa_mouse_arrow_data32);
      alfa_mouse_pointer = create_mouse_pointer(alfa_mouse_arrow_data);

      alfa_mouse_pointer48=alfa_mouse_pointer32;  //there is no huge cursor created so far

      if (BIGCURSOR==2) alfa_mouse_sprite = alfa_mouse_pointer32;
      else if (BIGCURSOR==1) alfa_mouse_sprite = alfa_mouse_pointer32;
      else  alfa_mouse_sprite = alfa_mouse_pointer;

    if (alfa_mouse_edit32!=NULL) destroy_bitmap(alfa_mouse_edit32);
    if (alfa_mouse_edit != NULL) destroy_bitmap(alfa_mouse_edit);
    alfa_mouse_edit32 = create_mouse_pointer32(alfa_mouse_edit_data32);
    alfa_mouse_edit = create_mouse_pointer(alfa_mouse_edit_data);

    alfa_mouse_edit48=alfa_mouse_edit32;

    if (BIGCURSOR==2) alfa_mouse_edit_sprite = alfa_mouse_edit48;
    else if (BIGCURSOR=1) alfa_mouse_edit_sprite = alfa_mouse_edit32;
    else  alfa_mouse_edit_sprite = alfa_mouse_edit;

    if (BIGCURSOR==2) alfa_mouse_busy_sprite = icon_hourglass_mem;
    else if (BIGCURSOR==1) alfa_mouse_busy_sprite = icon_hourglass_mem;
    else  alfa_mouse_busy_sprite = icon_hourglass_mem;

    set_mouse_sprite(alfa_mouse_sprite);

}

void ini_cursor_busy(void)
{
    if (BIGCURSOR==2) alfa_mouse_busy_sprite = icon_hourglass_mem;
    else if (BIGCURSOR==1) alfa_mouse_busy_sprite = icon_hourglass_mem;
    else  alfa_mouse_busy_sprite = icon_hourglass_mem;
}

void reset_cursor(void)
{
    CUR_ON = cur_on;
}

 void ini_e (void)
/*----------------------------------*/
{
  long free_size ;
  int i ;
  FILE *f;
  char previous_file_name [MAXPATH];
  BOOL eof_file;
  int k_style_DXF;

  ini_cursors();

  set_clip_rect((BITMAP*)second_screen, 0, 0, getmaxx(), getmaxy());

  Set_Version (VERSION4_0) ;
  signal(SIGINT,BreakOut);
  signal(SIGFPE,Float);
  
  free_size=-1;

  xp=0; yp=2*ED_INF_HEIGHT + 1; xk=0; yk=0;  //ustalanie wielkosci okna rysunku
  Change = FALSE;
  monitor_parametry(Driver);
  ignoruj_aktywacja=1;
 
  Ini_Place_Marker ();
  for (i = 0 ; i < SVMAX ; i++)
  {
    SERV [i] = nooop1 ;
  }
  setbkcolor(BKCOLOR/*kolory.paper*/);

  e.x=maxX-PKu130x;
  e.format = "%#12.9lg\0%#12.9lg;%#12.9lg" ;
  dodajstr(&e);
  if(get_mem_menu()==NULL) {ErrList(95);my_getch();Koniec();}
  
  demo(Alf_window_number);

  Read_Graph_Function () ;
  ReadTypyLinii();
 
  ReadTextStyle();  
  k_style_DXF=ReadTextStyleDXF();
  
  Read_Hatch_Pattern () ;
  
  Ini_String_List () ;

  Ini_Color_Table();

  get_config_sectors();
  
  /*czytanie zbioru PREFILES.DAT*/
  f = fopen ( "PREFILES.DAT" , "rt" ) ;
  if ( f != NULL )
  {
    i=0;
    eof_file=FALSE;
    while ((i<MAXHISTORYFILES) && (eof_file==FALSE))
    {
      /*czytanie nazw zbiorow*/
       if ( fgets(previous_file_name , MAXPATH , f ) == NULL ) eof_file=TRUE;
         else 
          {
            previous_file_name [ strlen (previous_file_name) - 1 ] = '\0';
            strcpy(Previous_File[i],previous_file_name);
          }  
       i++;  
    }
   fclose (f);  
  }   

    bufor_makra = (char *)malloc (Get_Buf_Mak_Size ());

    gx = (int*) malloc ((MAX_GRID_POINT + 2) * sizeof(int)) ;
    gy = (int*) malloc ((MAX_GRID_POINT + 2) * sizeof(int)) ;

  if (bufor_makra == NULL /*|| Layers == NULL*/ || gx == NULL || gy == NULL)
  {  

    demo_out(Alf_window_number);
    ErrList (96) ;
    my_getch ();
    Koniec ();
  }
  
  bufor_pcx = NULL;
  DRAWING_NUMBER = 0;
  InitBuffers () ;
  InitDane () ;
  INITIAL_SEL_TEST = initial_sel_test ;
  INTERSECT_ADD_TEST = intersect_add_test ;

  CUR_OFF=cur_off;
  CUR_ON=cur_on;
  REDDOD=nooop;
  REDRAW_ADD_MENU = nooop ;
  standard_func();
  KLS=menupoz;
  MVCUR=mvcur_;
  MPINI=mpini;
  TABFUN=edycjastr;
  TABFUN1=edycjastr_zn;
  sel.akt=0;

  demo_out (Alf_window_number);
  zbior_danych [0] = '\0';

  sel = (SEL){.akt=0, .akw=0, .gor=0, .cur=0, .wyj=0, .nr=0, .jump=0, .rez=0, .size=7}; //somehow is not initialized in LINUX ????????

  
}


T_PTR_Prototype Get_Prototype (void)
//-----------------------------------
{
  return &s__prot ;
}

void view_pattern_type(char* pattern_name)
{
	setfillstyle_(SOLID_FILL, BKCOLOR);
	bar(maxX / 2 + (8 * WIDTH), HEIGHT /*+ 1*/, maxX / 2 + (8 * WIDTH) + 5 + 15/*strlen(width_type_name)*/ * WIDTH, 2 * HEIGHT);
	moveto(maxX / 2 + (8 * WIDTH), HEIGHT + 1);
	setcolor(kolory.ink);
	outtext_r(pattern_name);
}


void view_line_type(LINIA *L)
/*---------------------------------*/
{ char width_type_name[40];
  char type_name[32];
  int grubosc_l, typ_l;

  grubosc_l=( L->typ & 224 ) / 32 ;
  typ_l=( L->typ & 31 );
  strcpy(width_type_name,view_width_tab[grubosc_l]);
  strcpy(type_name,view_type_tab[typ_l]);
  strcat(width_type_name,type_name);
  setfillstyle_(SOLID_FILL, BKCOLOR);
  bar(maxX/2+(8*WIDTH), HEIGHT /*+ 1*/,maxX/2+(8*WIDTH)+5+15/*strlen(width_type_name)*/*WIDTH, 2 * HEIGHT);
  moveto(maxX/2+(8*WIDTH),HEIGHT + 1);
  setcolor(kolory.ink);
  outtext_r(width_type_name);
}   

void view_point_type(T_Point *P)
/*---------------------------------*/
{ char type_name[32];

  strcpy(type_name, typ_punktu_inf[P->typ]);
 
  setfillstyle_(SOLID_FILL, BKCOLOR);

  bar(maxX/2+(8*WIDTH), HEIGHT + 1,maxX/2+(8*WIDTH)+5+10/*strlen(type_name)*/*WIDTH, 2 * HEIGHT /*+ 1*/);
  moveto(maxX/2+(8*WIDTH),HEIGHT + 2);

  setcolor(kolory.ink);
  outtext_r(type_name);
}

void change_slash(char *nazwa)
/*--------------------------*/
{ int i;
  if (strlen(nazwa)>0)
   {
   for (i=0; i<strlen(nazwa); i++)
    {
     if (nazwa[i]=='/') nazwa[i]='\\';
    }
   }
}

#pragma pack(1)
  typedef struct MYRGB
  {
   unsigned char r, g, b;
   unsigned char filler;
  } MYRGB;

  #define PAL_SIZE     256

  typedef MYRGB MYPALLETE[PAL_SIZE];
  #pragma pack()


void fade_out2(COLOR pallete, int speed)
{ int i,j;
  for (j=0; j<F32; j++)
   {
    for (i = 0; i < 256; ++i)
       setrgbpalette(i,pallete[i].red*(F32-j)/F32,pallete[i].gre*(F32-j)/F32,pallete[i].blu*(F32-j)/F32);
    delay(50/speed);
   }
}


void fade_in2(COLOR pallete, int speed)
{ int i,j;

  for (j=0; j<(F32+1); j++)
   {
    for (i = 0; i < 256; ++i)
      setrgbpalette(i,pallete[i].red*j/F32,pallete[i].gre*j/F32,pallete[i].blu*j/F32);
    delay(50/speed);
   }
}

#ifdef ALLEGRO
void fade_in1(MYPALLETE paleta, int speed);
{
#ifdef nofade
  return;
#else
  fade_in2(paleta, speed);
#endif
}
#endif

void Set_FONTNUMBER(int font_number)
{
  FONTNUMBER=font_number;
}

int get_FONTNUMBER(void)
{
 return FONTNUMBER;
}

int get_HEIGHT(void)
{
  return HEIGHT;
}

int my_kbhit(void)
{

 return keypressed();

}

void clean_key_buffer(void)
{
    key_buffer=-1;
}

void add_to_buffer(char c)
{
	key_buffer = c;
}

int my_ugetch(void)
{
	int key, scancode;
	key = ureadkey(&scancode);
	return key;
}

//unsigned char my_getch(void)
int my_getch(void)
{
	int key0;
  int key_buffer1;
  int key1, key2;
  unsigned char key_;
  //int k;
  unsigned int scancode;
  int LCTRL,RCTRL,ALT,ALTGR,LSHIFT,RSHIFT;
  char sticky_key;

	//// k=my_poll_keyboard();
 if (key_buffer >= 0)
   {
     if (key_buffer==0) key_buffer1=2;
       else key_buffer1=key_buffer;
     key_buffer=-1;

     return key_buffer1;
   }

 key0 = ureadkey(&scancode);
#ifdef LINUX
 if ((key0==127) && (scancode==77)) key0=0;
#endif

 if (key0 == 0)
 {
	 key1 = key0;
	 key2 = scancode;
 }
 else
 {
	 key1 = key0;
	 key2 = scancode;
 }

    sticky_key=get_shifts(key2);  //alt=1, altgr=2, ctrl=4, shift=8
    
    ALTGR=key[KEY_ALTGR] | (sticky_key & 2);
	ALT=key[KEY_ALT] | (sticky_key & 1);

	LCTRL = key[KEY_LCONTROL] | (sticky_key & 4);
	RCTRL = key[KEY_RCONTROL] | (sticky_key & 4);

	LSHIFT = key[KEY_LSHIFT] | (sticky_key & 8);
	RSHIFT = key[KEY_RSHIFT] | (sticky_key & 8);

	if ((sticky_key==4) && (key1==32) && (key2==75)) return CTRL_SPC;

    if ((ALTGR) && ((key2<27) || (key2>60)))
    {
	
		 {   if (key[KEY_CAPSLOCK] || (key_shifts & KB_CAPSLOCK_FLAG))
				{
					if (LSHIFT || RSHIFT) key1=scancode9[key2];
					  else key1=scancode6[key2];
				}
				else 
				{
					if (LSHIFT || RSHIFT) key1=scancode6[key2];
					  else key1=scancode9[key2];
				}
		 }
	
    }

	
	else if ((ALT) && (key2<27)) 
	{
		 {   if (key[KEY_CAPSLOCK] || (key_shifts & KB_CAPSLOCK_FLAG))
				{
					if (LSHIFT || RSHIFT) key1=scancode9_[key2];
					  else key1=scancode6_[key2];
				}
				else 
				{
					if (LSHIFT || RSHIFT) key1=scancode6_[key2];
					  else key1=scancode9_[key2];
				}
		 }
	}
 else if ((key1==0) && (key2<27))
 {
	key1=scancode4[key2];
 }
 else
	{

		if ((LCTRL) || (RCTRL))
		{   //Ctrl-A  ..... Ctrl-Z
			if (key1 == key2) { if (key1 > 0) return GET_CONTROL_KEYS(key1); }
			else if ((sticky_key & 4) && (key1 > 0) & (key2 > 0))
			{
				return GET_CONTROL_KEYS(key2);
			}
		}
		else if ((ALT) && (((key1 >= 48) && (key1 <= 57)) || (key1 == 43) || (key1 == 45) || (key1 == 61)))  //or 43 or 45 or 61
		{
			key_buffer = scancode4[key2];
			key1 = 0;
			if (ALT) ALT = FALSE;
			if (ALTGR) ALTGR = FALSE;
			return key1;
		}
		else if ((key1 > 0) && (key1 < 32) && (key1 == key2)) return GET_CONTROL_KEYS(key1); //when simulated

 key_=key1;

 if (LSHIFT || RSHIFT) key_+=1;
 
 if (LCTRL || RCTRL) key_+=2;

 if ((ALT) || (ALTGR))  key_+=4;
 
 if ((key_ <= 5) && (key2 > 0))
   {
     switch (key_)
     {
      case 0: key_buffer=scancode0[key2];  //------
      break;
      case 1: key_buffer=scancode1[key2];  // Shift
      break;
      case 2: key_buffer=scancode2[key2];  // Ctrl
      break;
      case 3: key_buffer=scancode3[key2];  // Ctrl-Shift
      break;
      case 4: key_buffer=scancode4[key2];  // Alt
      break;
      case 5: key_buffer=scancode5[key2];  // Alt-Shift
      break;
      default: key_buffer=0;
      break;
     }
     key1=0;
     
   }


 }
 

 return key1;

}


int my_directory_exists(const char *path) {
    struct stat info;

    if (stat(path, &info) != 0)
        return 0; // Failed to get file/directory information
#ifdef LINUX
    return S_ISDIR(info.st_mode); // Check if it's a directory
#else
	return (info.st_mode & S_IFDIR);
#endif
}

int my_file_exists(char *name)
{
	int ret;
	struct stat buffer;
	ret=(stat(name, &buffer) == 0);
	return ret;
}

int my_file_size(char* name)
{
    int ret, size;
    struct stat buffer;
    ret = (stat(name, &buffer) == 0);
    if (ret) return buffer.st_size;
    else return 0;
}


int	getdisk(void)
{
  int cur_di=0;
#ifndef LINUX
  cur_di=_getdrive() -1;
#endif
 
  return cur_di;
}

int	setdisk(int _drive)
{
#ifndef LINUX
  return _chdrive(_drive + 1);
#else
return 0;
#endif
}

int  biosprint(int _cmd, int _byte, int _port)
{
  return 1;
}

long	random(void)
{
	return 1;
}


int	findfirst(const char *_pathname, struct ffblk *_ffblk, int _attrib)
{ int ret;
  

  ret = al_findfirst(_pathname, &_al_ffblk, _attrib);
  if (!ret) 
  {
    _ffblk->ff_attrib=_al_ffblk.attrib;
	strcpy(_ffblk->ff_name,(char *) & _al_ffblk.name);
  }

  return ret;
}

int	findnext(struct ffblk *_ffblk)
{ int ret;
  
    ret=al_findnext(&_al_ffblk);
	if (!ret) 
	{
		_ffblk->ff_attrib=_al_ffblk.attrib;
		strcpy(_ffblk->ff_name,(char *) & _al_ffblk.name);
	}

	return ret;
}

char *w95_getlogon (void)
{  static char logon[60];

	return (char *)logon;
	
}


void __djgpp_set_ctrl_c(int flaga)       //blokowanie Ctrl-C
{
    return;
}

#ifndef MACOS
void getdate(date *aktualna_data)
{
 long lt;
 struct tm *today;
 time_t ltime;


 lt = time( &ltime );
	
 today = localtime( &ltime );

 aktualna_data->da_day=today->tm_mday;
 aktualna_data->da_mon=today->tm_mon+1;
 aktualna_data->da_year=today->tm_year+1900;
 aktualna_data->da_hour=today->tm_hour;
 aktualna_data->da_min=today->tm_min;
 aktualna_data->da_sec=today->tm_sec;
}
#endif

int dialog_active(void)
{
	return now_is_dialog;
}

void my_sleep(int sleepMs)
{
#ifdef LINUX
    usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)
#else
#ifdef BIT64
#ifndef LINUX
    _sleep(sleepMs);
#else
    Sleep(sleepMs);
#endif
#else
	_sleep(sleepMs);
#endif
#endif
}


#ifdef LINUX
long filelength_l(char *filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

long filelength(int f)
{  long sizef, posf;
    posf = lseek(f, 0, SEEK_CUR); // get current file pointer
    sizef = lseek(f, 0, SEEK_END); // seek to end of file and get current file pointer
    lseek(f, posf, SEEK_SET); // seek back to previous position in file
    return sizef;
}
#endif

void report_mem_leak_(void)
{

#ifndef DONTDOTHAT
#ifndef NDEBUG
#ifdef BIT64
	report_mem_leak();
#endif
#endif
#endif
}

#undef __BIB_E__