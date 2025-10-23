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

#define __ALLEGROEXT__
#pragma warning(disable : 4996)
#define _WIN32_WINNT 0x0502
#define ALLEGWIN
#ifndef LINUX
#ifndef ALLEGRO_AND_MFC
#define ALLEGRO_AND_MFC //????
#endif
#else

#endif

#define IC_CLOUD "CLOUD"
#define IC_CLOUD_CONNECT "CONNECT"
#define IC_CLOUD_USER "USER"
#define IC_CLOUD_PASSWORD "PASSWORD"
#define IC_CLOUD_SHARE_URL "SHARE_URL"
#define IC_CLOUD_ADS "ADS"
#define IC_CLOUD_ADS_URL "ADS_URL"
#define IC_CLOUD_UPGRADE "UPGRADE"
#define IC_CLOUD_UPGRADE_URL "UPGRADE_URL"

#include "allegext.h"
#include <allegro.h>
#ifdef MACOS
#include <osxalleg.h>
#endif
#include "loadpng.h"
#include "jpgalleg.h"

#ifndef LINUX
#include <winalleg.h>
#include <iostream>
#include <gfx.h>
#include <stdlib.h>
#include <vector>
#else
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#ifndef MACOS
#include <xalleg.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>
#else
//#include <cstdint>
//#include <ApplicationServices/ApplicationServices.h>
#endif
#include "report.h"
#include <curl/curl.h>
#endif

#ifdef LINUX
#include <sys/time.h>
#else
#include <time.h>
#endif
#include <math.h>
#include"forwin.h"

#ifdef LINUX
#ifdef ALLEGRO5
#include "../../Allegro-Legacy-Alfa/include/allegro/font.h"
#include "../../Allegro-Legacy-Alfa/include/allegro/internal/aintern.h"
#ifndef MACOS
#include "../../Allegro-Legacy-Alfa/include/xalleg.h"
#endif
#include "../../Allegro-Legacy-Alfa/include/xdnd.h"
#else
#include "../../allegro5-4.4.3/include/allegro/font.h"
#include "../../allegro5-4.4.3/include/allegro/internal/aintern.h"
#endif

#define LPCTSTR const char *
#define LPCSTR const char *

#define LPTSTR char *
#define LPSTR char *

#define LPCWSTR const uint16_t *  //TO CHECK
#define LPWSTR uint16_t *  //TO CHECK

#define LONG long

typedef unsigned long  DWORD;

#ifdef LINUX
#ifdef ALLEGRO5
#define GFX_GDI GFX_AUTODETECT_WINDOWED
#else
#define GFX_GDI GFX_XWINDOWS
#endif
#endif

#else
#include "allegro/font.h"
#include "allegro/internal/aintern.h"
#endif

timeval tvtimestamp = { 0,0 };
timeval tvtimestamp1;


#include "alfpro.h"
#include "o_inigfx.h"
#include "o_libfun.h"
#include "o_inicnf.h"

#include "menu.h"

#include "alfalogosak.h"

//#include "leak_detector_cpp.hpp"

#ifdef BIT64
#define long_long long long
#else
#define long_long long
#endif

#define TRUE  1
#define FALSE 0

#ifdef LINUX
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

BOOL cloud_connect;
char *cloud_user;
char *cloud_user0;
char *cloud_password;
char *cloud_password0;
char *cloud_share_url;
char *cloud_share0_url;
char *cloud_ads;
char *cloud_ads0;
char *cloud_ads_url;
char *cloud_ads0_url;
char *cloud_upgrade;
char *cloud_upgrade0;
char *cloud_upgrade_url;
char *cloud_upgrade0_url;
//#define BEEP

extern char* encodePassword(char *pass, char *key);
extern char* decodePassword(char *pass, char *key);

extern void set_client_flag(int client_no, unsigned char flag);
extern int deposit_hbitmap(char *dump_file, int real_x1, int real_y1, int real_x2, int real_y2, char *drawing_file);
extern int return_active_clients_no(void);
extern int return_hbitmap(int client, char *dump_file, int *real_x1, int *real_y1, int *real_x2, int *real_y2, char *drawing_file);
extern unsigned char get_client_flag(void);
extern void startup(LPCSTR lpApplicationName, LPSTR lpParams);
extern int get_rotation_inversion(void);

extern BOOL get_editor_on(void);
extern char *get_eTitle(void);

static int last_mouse_b = 0;
static int cur_mouse_b = 0;
static BOOL global_resized=FALSE;
static int attr_x=0,attr_y=0;
static BOOL is_hidden=FALSE;

extern BOOL RETINA;
BOOL wayland=FALSE;

int hidden_dy=3000;

#ifdef LINUX
#ifndef MACOS
static Display *main_display;
static Window main_root_window;
static int X11_SCREEN_SHIFT=36;
Display *display00=NULL;
#else
static int X11_SCREEN_SHIFT=25; //24 point=25 pxl
#endif
#else
extern RECT get_editbox_geometry_win(int opt);
extern HWND get_editor_hWnd(void);
static int X11_SCREEN_SHIFT = 32;  //standard for Windows
static int WIN_WINDOW_T_B = 9; //standard window top margin for Windows
_xdnd_struct *xdnd_buf=NULL;
#endif

static int curr_w=0, curr_w_=0, curr_h=0, curr_h_=0;

static int x_win_orig_, y_win_orig_, win_width_, win_height_;
static BOOL is_in=TRUE;

#define DEFAULT_SPRITE_W   16
#define DEFAULT_SPRITE_H   16

////////////////
#ifdef LINUX
#ifndef MACOS
// State machine structure
typedef struct {
    bool xdndExchangeStarted;
    bool xdndPositionReceived;
    bool xdndStatusReceived;
    bool xdndStatusSent;
    bool xdndDropReceived;
    Time xdndDropTimestamp;
    Time xdndLastPositionTimestamp;
    bool amISource;
    int p_rootX;
    int p_rootY;
    Window otherWindow;
    Atom proposedAction;
    Atom proposedType;
} XDNDStateMachine;
// Atom definitions
static Atom XdndAware, XA_ATOM_, XdndEnter, XdndPosition, XdndActionCopy, XdndLeave, XdndStatus, XdndDrop,
XdndSelection, XDND_DATA, XdndTypeList, XdndFinished, WM_PROTOCOLS, WM_DELETE_WINDOW, typesWeAccept[6];

// XDND global state machine
static XDNDStateMachine xdndState;
#else
typedef struct {
	int x;
	int y;
	int w;
	int h;
} WATTR;
#endif
#endif
////////////////


char alfa_mouse_arrow_data_x[DEFAULT_SPRITE_H * DEFAULT_SPRITE_W] =
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

#ifndef LINUX
HWND win_get_window_(void)
{
    return win_get_window();
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifdef LINUX
#ifndef MACOS
#include "xdndfile.h"
//extern BITMAP *parent_screen;
Window get_allegro_window(void);

static AWIdnd dnd;

    Window *winlist (Display *disp, unsigned long *len);
    char *winame (Display *disp, Window win);
    int getprop (Display *disp, char *name, Window win, Window root_window, XWindowAttributes &attr, int *is_hidden);
    char *atomtype (Atom x);
#else
	extern int send_AppleScript(pid_t pid, int x, int y, BOOL relative);
#endif
#endif

    void Check_ConfigureNotify(void);

    void set_resized_window_GFX(DRIVER_STRUCT *drv, int *dx, int *dy, int option);

    BOOL ConfigureNotifySemaphore=TRUE;

    int GoRegRedraw(void(*ptr)(void));
	int TestRedraw(void);
	int testCall(int val);

    BOOL hibernate=FALSE;

    extern int Komunikat_R0;
    extern void komunikat0(int n);
    extern int get_komunikat_arc_bak(void);
    extern BOOL kom_arc_bak(void);

extern void my_sleep(int sleepMs);
	extern void standard_func(void);

    extern int wmctrl (int argc, char **argv);
    extern int edit_text_flag;

    extern void show__if_DEMO_RECORDING(int newicon);
    extern char _EDIT_TEXT_[];
    extern void get_editbox_size_origin(int *w, int *h, int *x, int *y);
    extern void get_editbox_size_origin_line(int *w, int *h, int *x, int *y);

    extern void utf8Upper(char* text);
    extern void change_bs2s(T_Fstring thestring);
    extern BOOL Get_Private_Profile_Strings_Cloud(T_Fstring lpApplicationName, BOOL (*f)(T_Fstring, T_Fstring));

    extern char *icon_upgrademark_pmem;
    extern char *icon_noupgrademark_pmem;

    extern void okno_all(void);

	extern int my_text_length(FONT *font, const char *text);

	extern void TTF_text_UI(const char *text, int x, int y, int *text_h, int *text_v);
    extern void TTF_text_UI_(BITMAP *ui_screen, const char *text, int x, int y, int *text_h, int *text_v);

	extern FONT* alfa_create_allegro_font(void);

	extern int fnsplit(const char *path, char *drive, char *dir, char *name, char *ext);
	extern void komunikat_str(char *st);
	extern void set_background_menu(char *background);

	extern int WinSize(int x0, int y0, int dx, int dy, int NO); 
	extern int x0_new, y0_new, dx_new, dy_new, dx_back, dy_back, x0_back, y0_back;
	extern void getcolor_RGB(int *red, int *green, int *blue, int color);

	extern int poll_kbd;
	extern int textwidth(const char  *textstring);
	extern void Set_FONTNUMBER(int font_number);
	extern int get_FONTNUMBER(void);
	extern void lock_mouse(void);
	extern void _free_mouse(void);
	extern void Save_Last_Window_Settings(void);
	extern int Load_Last_Window_Settings(int *x_win_orig, int *y_win_orig, int *win_width, int *win_height);
	extern void delay(int time);
	extern int dialog_active(void);
	extern void my_rect(int x1, int y1, int x2, int y2);
	extern void  Odczyt_licznikow_resize(long *dx, long *dy);
	extern void set_resized_window(int dx, int dy, int option);
	extern void redraw(void);
	extern void set_last_window(void);

	extern int MP_SIZE;
	extern int ED_INF_HEIGHT;
	extern int BAR_G;
	extern int HEIGHT;
	extern int WIDTH;

	extern unsigned char _dac_normal[256][3];
	extern BOOL sleep_state;
	extern BOOL mouse_out;
	extern void  Test_przycisniec(int *keys);
	extern char GFX_ID[5];
	extern int GFX_WIN;

	int circle_and_rectangle_proc(void);
	extern char *background_file_name_ptr;
	extern int time_scrsave;
	extern int dynamic_menu;
    extern int desktop_instruction;
	extern float pan_dxy;

	extern void CopyPreview(int x01, int y01, int x02, int y02, int client_number);
#ifndef LINUX
	extern void Set_Focus(HWND HWnd);
#endif
	int Free_Mouse(void);
	extern int Client_number;
	extern BOOL preview_blocked;
	extern void reset_flag(void);
	extern char background_pcx_file[260];

	extern void serv134_135(BOOL go_nogo);

	extern void  Odczyt_licznikow(void);
	extern int last_mouse_z;

	extern BITMAP *alfa_mouse_sprite;
	       /* current mouse pointer */

	extern int now_is_dialog;
	extern int Get_Global_Menu_Flags(void);
	extern int BIGCURSOR;
    extern BOOL BAR_POINTER;

	extern int lineC(long x1, long y1, long x2, long y2);
	extern int DRAWING_NUMBER;

	extern void sw_callback_push_proc(void);

	extern int poll_kbd, poll_m;

	extern double get_skala(void);
	extern double DokladnoscF;

	extern float solidpatternscales[7];

	extern int GetPattern(char* pattern_name, char** pattern_ptr);
	extern BITMAP* load_memory_pcx(AL_CONST void* buffer, PALETTE* pal);

	extern int solid_pattern_library_no;
    #define SOLID_PATTERN_LIBRARY_MAX_ITEMS 512 //1024
	extern char* Solid_Pattern_Library[SOLID_PATTERN_LIBRARY_MAX_ITEMS];

	void sw_callback_locked_proc(void);

	void putimage_preserve(int left, int top, BITMAP  *bitmap, int op);

	extern double PATTERN_SCALE;
	extern double PATTERN_SCALE_PRN;
	extern double FIX_PATTERN_SCALE_PRN;

	extern BOOL bitmap_pattern_exist;
	extern BOOL solid_translucent_exist;

	extern void gray_bitmap(BITMAP* src, BITMAP* dst, int dx, int dy, int gray_sat);

	extern DRIVER_STRUCT drv_cur;

	extern void makro_wine(void);

	extern void restore_focus(void);

	extern void remove_keyimages(BOOL silent);

	extern void my_soft_polygon(BITMAP* bmp, int vertices, AL_CONST int* points, int color, int translucency, int colorB);

	extern int GetColorAC(int color);

    extern void dialog_cursor(int on);

    extern void clean_key_buffer(void);

    extern void show_x_cursor(void);

    extern BOOL go_reset_if_resized;

    extern GrContext *get_second_screen(void);
	extern GrContext* get_second_screen_back(void);
	extern GrContext* get_second_screen_bak_(void);

    extern void flip_screen(void);
    extern void flip_full_screen(BITMAP * the_screen);

#ifdef ALLEGRO5
    extern BITMAP* icon_alfacad_mem;
    extern void set_display_icon(BITMAP *icon);
#endif
	int set_window_origin(int x_win_orig, int y_win_orig);
    void LockMouse(void);

    void get_display_window(void);
    void focus_display_window(void);

extern int Test_App_Shm(int doit, char* file_name);

extern int My_GetFiles(char* szDir, int* n_list, char* pattern, char* pattern1, char* pattern2, char* pattern3, BOOL blocks);
extern char **strarray;

extern void flip_any_screen(BITMAP* third_screen);
extern size_t uri_decode(const char* src, const size_t len, char* dst);
extern int my_file_exists(char* name);
extern int my_file_size(char* name);

extern void extra_logo(int x, int y, int option, char* file_name);
extern int getlanguage(void);
extern int ask_question(int n_buttons, char* esc_string, char* ok_string, char* cont_string, char* comment_string, int color_comment, char* comment1_string, int color1_comment, int cien, int image);
extern void Restart(void);
extern DWORD SystemSilent(char* strFunct, char* strstrParams);
extern DWORD SystemSilentS(char* strstrParams);
extern DWORD RunSilent(char* strFunct, char* strstrParams);

extern int Expand_flex(void);
extern int Get_Mouse_Wheel(void);

extern char** argv_;

#ifdef ALLEGRO5
extern int get_display_width(void);
extern int get_display_height(void);
extern void get_window_position(int *x, int *y);
extern void set_window_position(int x, int y);
#ifdef MACOS
extern void set_osx_window_position(pid_t pid, int x, int y, BOOL relative);
extern pid_t al_getpid(void);
extern bool isRetinaOSXDisplay(void);
#endif
#endif
extern void set_sleep_state(BOOL state);

#ifdef LINUX
extern void _xwin_set_hints(int w, int h);
extern char Window_Name[32];

int get_monitor_dims(int *ret_left_x, int *ret_right_x, int *ret_top_y, int *ret_bottom_y, int i);
#ifdef ALLEGRO5
extern BOOL al_get_monitor_dims( int *ret_left_x, int *ret_right_x, int *ret_top_y, int *ret_bottom_y);
extern int al_get_window_origin_and_size(int *x_win_orig, int *y_win_orig, int *win_width, int *win_height);
extern int al_get_window_new_origin_and_size(int *x_win_orig, int *y_win_orig, int *win_width, int *win_height);
extern bool acknowledge_resize(void);

extern int get_x_window_id(void);
extern int get_display_flags(void);
extern void all_render_screen(void);
extern void set_semaphore(bool sem);
extern void set_timer_speed(int speed);
extern void al_resize_screen(int w, int h);
#endif
#endif

	long global_pan_dx = 0;
	long global_pan_dy = 0;

	void reset_trans_blender(void);
    void save_window_dim(int x0_, int y0_, int dx_, int dy_);
    void save_desktop_instruction(void);

    void Set_Screenplay(BITMAP *ctx_bitmap);

#ifndef LINUX
    int get_monitor_dims(int* ret_left_x, int* ret_right_x, int* ret_top_y, int* ret_bottom_y, int monitor);
#endif

int	global_mode;

void beep(long sound);

BOOL my_rgb_map = FALSE;
COLOR_MAP *trans_map = NULL;
COLOR_MAP *trans_map_preview = NULL;
COLOR_MAP *light_map = NULL;
DATAFILE *data;
FONT *font1, *font2, *font3;
BITMAP *bmp_save=NULL;
BITMAP *background_pcx;
BITMAP* pattern_bitmap = NULL;
BITMAP* pattern_bitmap_middle = NULL;
BITMAP* pattern_bitmap_middle1 = NULL;

#define CURRENT_PATTERN_DEF_MAX 64
BITMAP* pattern_bitmap_scaled[CURRENT_PATTERN_DEF_MAX] = { NULL };
int pattern_bitmap_scaled_no = 0;

typedef struct CURRENT_PATTERN_DEF
{
	float bitmap_scale = 1.0;
	char bitmap_name[32] = "";
	char bitmap_angle = 0;
	float bitmap_dx = 0.0;
	float bitmap_dy = 0.0;
	float bitmap_fx = 0.0;
	int bitmap_flags = 0;
} CURRENT_PATTERN_DEF;

CURRENT_PATTERN_DEF current_pattern_def[CURRENT_PATTERN_DEF_MAX];

double current_fx = -1.0;
BITMAP *sprite_bmp[16]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
int NO_S=0;
static int origin_x_fix=0, origin_y_fix=0;

static int h_increase_arch=800;
static int v_increase_arch=600;  
//static
    int x_increase_arch=0;
//static
    int y_increase_arch=0;
extern BOOL window_was_resized;
static int sprite_bmp_x[16], sprite_bmp_y[16];
static int points_old[8]={0,0,0,0,0,0,0,0};
static int c_old=-1;
static int dx_old=-1;
static int dy_old=-1;

static int FONT_DIRECTION=0;
static int font_charsize;
static int gfx_card = GFX_AUTODETECT;
static int cur_color=0;
static int cur_colors_depth=8;
static int cur_width=640;
static int cur_height=480;
static int cur_colors=256;
static int cur_fillstyle_pattern=0;
static int cur_fillstyle_color=0;
static int cur_linestyle_linestyle=0;
static int cur_linestyle_upattern=0;
static int cur_linestyle_thickness=1;
static int cur_posx=0;
static int cur_posy=0;
static int viewport_x=0;
static int viewport_y=0;
static int viewport_xk=0;
static int viewport_yk=0;
static int cur_text_just_horiz=0;  //left
static int cur_text_just_vert=0;  //bottom
int cursor_color0=0;

static BITMAP *NONCLIPPED_SCREEN;

BITMAP *screenplay;

static BITMAP *parent_screen=NULL;

static BITMAP *parent_second=NULL;

static int dist_just=1;
static int n_h_increase=0;
static int h_increase;
static int v_increase;
static BOOL sw_callback_locked = FALSE;
static BOOL sw_callback_push = FALSE;

int fixangles[4] = { 0, 192, 128, 64 };

int  __gr_color=0;

static int free_mouse = 0;

int TRANSLUCENCY = 179; //128
int HALFTRANSLUCENCY = 179;

#ifndef LINUX
static HDC win_dc;
#endif

#define BYTES_PER_PIXEL(bpp)  (((int)(bpp) + 7) / 8)

void Copy_screen(void);

PALETTE mypal;

typedef unsigned int GrColor;

typedef struct FVECTOR 
{
  float x, y, z;
} FVECTOR;


typedef struct
{
	short int scale;
	short int dx;
	short int angle;
	short int dy;
	short int flags;
	char pattern[32];
} SOLID_PATTERN_COPY;

#ifndef LINUX

void init_file_dropped_fill_buf(void)
{
	char* ptr;
	get_file_dropped_fill_buf(&ptr);
	xdnd_buf = (_xdnd_struct*)ptr;
}

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};


void get_console(void)
{
    AllocConsole();
    ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void free_console(void)
{
    FreeConsole();
}


int gettimeofday(struct timeval* tp, struct timezone* tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    time = ((uint64_t)file_time.dwLowDateTime);
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec = (long)((time - EPOCH) / 10000000L);
    tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
    return 0;
}


struct tm* localtime_r(time_t* _clock, struct tm* _result) {
    _localtime32_s(_result, (const __time32_t*)_clock);  // the _clock is time_t*, but _localtime32_s need ‘const __time32_t*’
    return _result;
}
#endif


typedef int64_t timestamp_t;

static int origin_x=0;
static int origin_y=0;

#ifdef LINUX
#ifdef MACOS

bool isRetinaDisplay(void)
{
	return isRetinaOSXDisplay();
}

#else
//Linux
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>

bool isRetinaDisplay(void) {
    Display* display = XOpenDisplay(NULL);
    if (!display) return false;

    int screen = DefaultScreen(display);
    float dpiX = (float)(DisplayWidth(display, screen) * 25.4) / DisplayWidthMM(display, screen);
    XCloseDisplay(display);

    return dpiX > (96 * 1.25); // Assuming 96 DPI is the standard, adjust as needed
}
#endif
#else
//Windows
//#include <windows.h>

bool isRetinaDisplay(void) {
    HDC hdc = GetDC(NULL);
    int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
    ReleaseDC(NULL, hdc);
    return dpiX > (96 * 1.25); // Assuming 96 DPI is the standard, adjust as needed
}
#endif

#ifdef LINUX
#ifndef MACOS
void _xwin_set_hints(int w, int h)
{
    XSizeHints *hints = XAllocSizeHints();

    //if (origin_x<0) origin_x=0;
    //if (origin_y<0) origin_y=0;
#ifdef ALLEGRO5
    return;
#endif
    /* Set size and position hints for Window Manager.  */
    if (hints) {
        hints->flags = PPosition | PMinSize | PMaxSize | PBaseSize;
        //hints->min_width  = hints->max_width  = hints->base_width  = w; //MAREK
        //hints->min_height = hints->max_height = hints->base_height = h;
        hints->x = origin_x;
        hints->y = origin_y;
        hints->min_width  = w; //100;
        hints->max_width  = 6880; //6000;
        hints->base_width  = w;
        hints->min_height = h; //350;
        hints->max_height = 3000;
        hints->base_height = h;
        XSetWMNormalHints(_xwin.display, _xwin.wm_window, hints);

        XFree(hints);
    }
}
#else
void xwin_set_hints_(int w, int h)
{
	//MACOS TEMPORARY
	return;
}
#endif
#endif

void set_origins(int x, int y)
{
    origin_x=x;
    origin_y=y;
}

timestamp_t currentTimestamp( void )
{
    time_t t;

    t=time(NULL);

    return (timestamp_t)t;

}

/* ----------------------------------------------------------------------------- */

int32_t timestampToStructtm ( timestamp_t timestamp, struct tm* dateStruct)
{

#ifdef LINUX
#ifndef MACOS
    gmtime_r ( &timestamp, dateStruct);
#else
	gmtime_r ( (time_t*)&timestamp, dateStruct);
#endif
#else
    struct tm *dateStruct1;
    dateStruct1=gmtime(&timestamp);
    memmove(dateStruct, dateStruct1, sizeof(tm));
#endif
    return 0;
}

/* ----------------------------------------------------------------------------- */
char *eng_months[]= {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
char *eng_day[]= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

int int_month(char *row_month)
{
    for (int i=1; i<13; i++)
    {
        if (strcmp(row_month, eng_months[i])==0) return i;
    }
    return 1; //just for case
}

int32_t sprintf_timestampAsYYYYMMDDHHMMSS ( char* buf, long *date, timestamp_t timestamp )
{
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
    struct tm timeStruct;
    int weekday = 0;

    if (timestamp==NULL) {
        return sprintf(buf, "NULL_TIMESTAMP");
    }

    memset (&timeStruct, 0, sizeof (struct tm));
    timestampToStructtm(timestamp, &timeStruct);

    year = timeStruct.tm_year + 1900;
    month = timeStruct.tm_mon + 1;
    day = timeStruct.tm_mday;
    hour = timeStruct.tm_hour;
    minute = timeStruct.tm_min;
    second = timeStruct.tm_sec;
    weekday = timeStruct.tm_wday;

    *date=year*10000+month*100+day;
    return sprintf(buf, "%s, %02d %s %04d %02d:%02d:%02d", eng_day[weekday], day, eng_months[month], year, hour, minute, second);
}

void     getviewsettings(struct viewporttype  *viewport)
{
  viewport->left=viewport_x;
  viewport->top=viewport_y;
  viewport->right=viewport_xk;
  viewport->bottom=viewport_yk;
  return;
}

void setviewsettings(struct viewporttype  *viewport, int v_x, int v_y, int v_xk, int v_yk)
{
    viewport->left=v_x;
    viewport->top=v_y;
    viewport->right=v_xk;
    viewport->bottom=v_yk;
    return;
}


int      getmaxx(void)
{
  return cur_width-2; 
}

int      getmaxy(void)
{
  return cur_height-2;
}

int      __getmaxx(void)
{
  return cur_width-2; 
}

int      __getmaxy(void)
{
  return cur_height-2; 
}

void destroy_parent(void)
{
	parent_screen=NULL;
}

void setviewport(int left, int top, int right, int bottom, int clip)
{
    int x_left;
    int y_top;
    int x_right;
    int y_bottom;

    if (parent_screen != NULL) {destroy_bitmap(parent_screen);
        parent_screen = NULL;
    }
#ifndef ALLEGRO5
    screen = NONCLIPPED_SCREEN;
#endif
    parent_screen = create_sub_bitmap((BITMAP*)screen, left, top, right-left+1, bottom-top+1);
#ifndef ALLEGRO5
    screen = parent_screen;
#else
    set_clip_rect(screen,left, top, right-left, bottom);  //bottom-top
#endif
    screenplay = parent_screen;

    if (left<right) { x_left=left; x_right=right;}
    else { x_left=right; x_right=left;}
    if (top<bottom) { y_top=top; y_bottom=bottom;}
    else { y_top=bottom; y_bottom=top;}

    viewport_x=x_left;
    viewport_y=y_top;
    viewport_xk=x_right;
    viewport_yk=y_bottom;
    set_clip_rect(screenplay,0, 0, right-left, bottom-top);
    return;
}

void     setviewport_second(BITMAP *second_screen,int left, int top, int right, int bottom, int clip)
{
  int x_left;
  int y_top;
  int x_right;
  int y_bottom;


  if (left<right) { x_left=left; x_right=right;}
    else { x_left=right; x_right=left;}
  if (top<bottom) { y_top=top; y_bottom=bottom;}
    else { y_top=bottom; y_bottom=top;}

  viewport_x=x_left;
  viewport_y=y_top;
  viewport_xk=x_right;
  viewport_yk=y_bottom;
  screenplay = second_screen;
  set_clip_rect(second_screen,0, 0, right-left, bottom-top);
  return;
}


unsigned imagesize(int left, int top, int right, int bottom)
{ int size;
 int width, height;

 width=right-left;
 height=bottom-top;

 size=sizeof(BITMAP)+(sizeof(char *) * height) +
      width * BYTES_PER_PIXEL(cur_colors_depth) * height * BYTES_PER_PIXEL(cur_colors_depth)+100;     //100 to zapas

  return size;
}

unsigned long_long imagesizelong(int left, int top, int right, int bottom)
{
	unsigned long_long size;
	int width, height;

	width = right - left;
	height = bottom - top;

	size = sizeof(BITMAP) + (sizeof(char *) * height) + 
		(unsigned long) width * BYTES_PER_PIXEL(cur_colors_depth) * (unsigned long) height * BYTES_PER_PIXEL(cur_colors_depth) + 100;     //100 to zapas

	return size;
}

int getClosest(int val1, int val2,
	int target)
{
	if (target - val1 >= val2 - target)
		return val2;
	else
		return val1;
}

int findClosest(int arr[], int n, int target)
{
	// Corner cases
	if (target <= arr[0])
		return arr[0];
	if (target >= arr[n - 1])
		return arr[n - 1];

	// Doing binary search
	int i = 0, j = n, mid = 0;
	while (i < j) {
		mid = (i + j) / 2;

		if (arr[mid] == target)
			return arr[mid];

		/* If target is less than array element,
			then search in left */
		if (target < arr[mid]) {

			// If target is greater than previous
			// to mid, return closest of two
			if (mid > 0 && target > arr[mid - 1])
				return getClosest(arr[mid - 1],
					arr[mid], target);

			/* Repeat for left half */
			j = mid;
		}

		// If target is greater than mid
		else {
			if (mid < n - 1 && target < arr[mid + 1])
				return getClosest(arr[mid],
					arr[mid + 1], target);
			// update i
			i = mid + 1;
		}
	}

	// Only single element left after search
	return arr[mid];
}


int setfillpattern_gregre_scaled(char *s_pattern, BOOL to_printer, double resolution, long origin_x, long origin_y, int * origin_x_prn, int * origin_y_prn)
{
	PALLETE pal;
	float fx;
	int dx, dy, dxx, dyy, dxxfx;
	int dmax = 2048;
	SOLID_PATTERN_COPY* solid_pattern;
	float scale;
	int angle;
	int pattern_dx, pattern_dy;
	BITMAP* pattern_bitmap_0;
	BOOL found;
	int i;
	int p_type;
	char* pattern_ptr;
	long png_mem;
	int width_max = 1024;
	int height_max = 1024;
	int xy[8];
	float multiplex=1.0;
	int flags;
	int bi;
	BOOL create_new;

	solid_pattern = (SOLID_PATTERN_COPY*)s_pattern;
	scale = solidpatternscales[solid_pattern->scale];
	angle = solid_pattern->angle;
	pattern_dx= solid_pattern->dx;
	pattern_dy = solid_pattern->dy;
	flags = solid_pattern->flags;

	float origin_factors[] = { 0, 0.03125, 0.0625, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875 };

	if (to_printer)
	{
		fx = (float)scale * resolution * PATTERN_SCALE_PRN * FIX_PATTERN_SCALE_PRN;
		dmax = 4096;
		angle = get_rotation_inversion();
	}
	else fx = (float)scale * get_skala() * DokladnoscF * 0.1 * PATTERN_SCALE;  //0.125

	//searching for bitmap
	found = FALSE;
	for (i = 0; i < CURRENT_PATTERN_DEF_MAX; i++)
	{
		if (pattern_bitmap_scaled[i] != NULL)
		{
			if ((strcmp(current_pattern_def[i].bitmap_name, solid_pattern->pattern) == 0) &&
				(current_pattern_def[i].bitmap_angle == angle) &&
				(current_pattern_def[i].bitmap_scale == scale) &&
				(current_pattern_def[i].bitmap_dx == pattern_dx) &&
				(current_pattern_def[i].bitmap_dy == pattern_dy))
			{
				found = TRUE;
				if ((current_pattern_def[i].bitmap_flags != flags) ||
					(current_pattern_def[i].bitmap_fx != fx))
					create_new = TRUE;
				else create_new = FALSE;
				bi = i;
				break;
			}
		}
		else break;
	}
	if (found == FALSE)  //need new bitmap
	{
		if (pattern_bitmap_scaled_no < CURRENT_PATTERN_DEF_MAX) pattern_bitmap_scaled_no++;
		else 
			pattern_bitmap_scaled_no = CURRENT_PATTERN_DEF_MAX;
		create_new = TRUE;
		bi = pattern_bitmap_scaled_no - 1;
	}

	if (create_new)
	{
		if (pattern_bitmap != NULL)
		{
			destroy_bitmap(pattern_bitmap);
			pattern_bitmap = NULL;
		}

		//searching for pattern
		p_type = GetPattern(solid_pattern->pattern, &pattern_ptr);
		if (p_type == 0) return -1;

		if (angle == 0)
		{
			if (p_type == 2)
			{
				png_mem = width_max * height_max * 4 + 100;  // 4 BYTES per pixel , 100 is reserve
				pattern_bitmap = load_memory_png(pattern_ptr, png_mem, pal);
			}
			else if (p_type == 1)
			{
				pattern_bitmap = load_memory_pcx(pattern_ptr, &pal);
			}
			else if (p_type == 3)
			{
				png_mem = width_max * height_max * 4 + 100;  // 4 BYTES per pixel , 100 is reserve
				pattern_bitmap = load_memory_jpg(pattern_ptr, png_mem, pal);
			}
			else return -1;

			if (pattern_bitmap == NULL) return -1;
		}
		else
		{
			if (p_type == 2)
			{
				png_mem = width_max * height_max * 4 + 100;  // 4 BYTES per pixel , 100 is reserve
				pattern_bitmap_0 = load_memory_png(pattern_ptr, png_mem, pal);
			}
			else if (p_type == 1)
			{
				pattern_bitmap_0 = load_memory_pcx(pattern_ptr, &pal);
			}
			else if (p_type == 3)
			{
				png_mem = width_max * height_max * 4 + 100;  // 4 BYTES per pixel , 100 is reserve
				pattern_bitmap_0 = load_memory_jpg(pattern_ptr, png_mem, pal);
			}
			else return -1;

			if (pattern_bitmap_0 == NULL) return -1;

			pattern_bitmap = create_system_bitmap(pattern_bitmap_0->w, pattern_bitmap_0->h);
			if (pattern_bitmap == NULL)
			{
				destroy_bitmap(pattern_bitmap_0);
				return -1;
			}
			rotate_sprite(pattern_bitmap, pattern_bitmap_0, 0, 0, itofix(fixangles[angle]));  //0,0
			destroy_bitmap(pattern_bitmap_0);
			pattern_bitmap_0 = NULL;

		}

		dx = pattern_bitmap->w;
		dy = pattern_bitmap->h;
		dxx = dx * fx;
		dyy = dy * fx;

		int arr[] = { 16, 32, 64, 128, 256, 512, 1024, 2048, 4098, 8192};
		int fxarr[] = { 16, 21, 32, 42, 64, 85, 128, 170, 256, 341, 512, 682, 1024, 1365, 2048, 2730, 4098, 5464, 8192};

		int directfx = 0;
		int n = sizeof(arr) / sizeof(arr[0]);
		int target = dxx;
		int nfx = sizeof(fxarr) / sizeof(fxarr[0]);
		int targetfx = dxx;
		dxx = findClosest(arr, n, target);
		dxxfx = findClosest(fxarr, nfx, targetfx);

		if (dxx > dmax)
		{
			dxx = dmax;
			dyy = dmax;
		}
		if (dxxfx > dmax)
		{
			dxxfx = dmax;
		}

		if (dxxfx > dxx)
		{
			if ((to_printer) && (dxx > 1024))
			{
				directfx = 0;
				multiplex = 1.0;
			}
			else
			{
				directfx = 1;
				dxx = dxx * 2;
				multiplex = 0.3333;
			}
		}
		else if (dxxfx < dxx)
		{
			directfx = 2;
			multiplex = 0.3333;
		}
		else
			multiplex = 1.0;

		dyy = dxx;

		if ((directfx==1) || (directfx == 2))
		{
			if (pattern_bitmap_middle != NULL) destroy_bitmap(pattern_bitmap_middle);
			pattern_bitmap_middle = create_system_bitmap(dxx, dyy);

			if (pattern_bitmap_middle == NULL)
				return -1;

			stretch_blit(pattern_bitmap, pattern_bitmap_middle, 0, 0, dx, dy, 0, 0, dxx, dyy);
			destroy_bitmap(pattern_bitmap);
			pattern_bitmap = NULL;

			if (pattern_bitmap_middle1 != NULL) destroy_bitmap(pattern_bitmap_middle1);
			pattern_bitmap_middle1 = create_system_bitmap(dxx * 3, dyy * 3);

			if (pattern_bitmap_middle1 == NULL)
				return -1;

			drawing_mode(DRAW_MODE_COPY_PATTERN, pattern_bitmap_middle, 0, 0);  //x_anchor=0, y_anchor=0
			xy[0] = 0; xy[1] = 0; xy[2] = 0; xy[3] = dyy * 3; xy[4] = dxx * 3; xy[5] = dyy * 3; xy[6] = dxx * 3; xy[7] = 0;
			polygon(pattern_bitmap_middle1, 4, xy, 0);
			destroy_bitmap(pattern_bitmap_middle);
			pattern_bitmap_middle = NULL;

			if (pattern_bitmap_scaled[bi] != NULL) destroy_bitmap(pattern_bitmap_scaled[bi]);
			pattern_bitmap_scaled[bi] = create_system_bitmap(dxx * 2, dyy * 2);

			if (pattern_bitmap_scaled[bi] == NULL)
				return -1;

			stretch_blit(pattern_bitmap_middle1, pattern_bitmap_scaled[bi], 0, 0, dxx * 3, dyy * 3, 0, 0, dxx * 2, dyy * 2);
			
			destroy_bitmap(pattern_bitmap_middle1);
			pattern_bitmap_middle1 = NULL;
		}
		else
		{
			if (pattern_bitmap_scaled[bi] != NULL) destroy_bitmap(pattern_bitmap_scaled[bi]);
			pattern_bitmap_scaled[bi] = create_bitmap_ex(bitmap_color_depth(pattern_bitmap), dxx, dyy);

			if (pattern_bitmap_scaled[bi] == NULL)
				return -1;

			stretch_blit(pattern_bitmap, pattern_bitmap_scaled[bi], 0, 0, dx, dy, 0, 0, dxx, dyy);
			
			destroy_bitmap(pattern_bitmap);
			pattern_bitmap = NULL;
		}


		if (flags >= 1)  //hide or print gray
		{
			gray_bitmap(pattern_bitmap_scaled[bi], pattern_bitmap_scaled[bi], pattern_bitmap_scaled[bi]->w, pattern_bitmap_scaled[bi]->h, flags);
		}
	

		strcpy(current_pattern_def[bi].bitmap_name, solid_pattern->pattern);
		current_pattern_def[bi].bitmap_angle = angle;
		current_pattern_def[bi].bitmap_scale = scale;
		current_pattern_def[bi].bitmap_dx = pattern_dx;
		current_pattern_def[bi].bitmap_dy = pattern_dy;
		current_pattern_def[bi].bitmap_flags = flags;
		current_pattern_def[bi].bitmap_fx = fx;
	}

	
	origin_x_fix = -(int)((origin_factors[pattern_dx] * multiplex * (float)pattern_bitmap_scaled[bi]->w) + 0.5);
	origin_y_fix = -(int)((origin_factors[pattern_dy] * multiplex * (float)pattern_bitmap_scaled[bi]->h) + 0.5);

	if ((origin_x_prn != NULL) && (origin_y_prn != NULL))
	{

		*origin_x_prn = origin_x_fix;
		*origin_y_prn = origin_y_fix;
	}
	
	drawing_mode(DRAW_MODE_COPY_PATTERN, pattern_bitmap_scaled[bi], (int)origin_x + origin_x_fix, (int)origin_y + origin_y_fix);  //x_anchor=0, y_anchor=0
	
	return bi;
}

void DonePatternBitmapScaled(void)
{
	int i;

	for (i = 0; i < CURRENT_PATTERN_DEF_MAX; i++)
	{
		if (pattern_bitmap_scaled[i] != NULL)
		{
			destroy_bitmap(pattern_bitmap_scaled[i]);
			pattern_bitmap_scaled[i] = NULL;
			strcpy(current_pattern_def[i].bitmap_name, "");
		}
		else break;
	}
	pattern_bitmap_scaled_no = 0;
}

void setfillpattern_gregre()
{
	drawing_mode(DRAW_MODE_COPY_PATTERN, background_pcx, 0, 0);
}

void     fillpolypattern(int numpoints, AL_CONST int *polypoints, char *s_pattern, long origin_x, long origin_y)
{
	int bi;

  bitmap_pattern_exist = TRUE;

  bi = setfillpattern_gregre_scaled(s_pattern, FALSE, 0.0, origin_x, origin_y, NULL, NULL); //option for filling solid, trace and hatch with image

  polygon(screenplay, numpoints, polypoints, cur_fillstyle_color);
  
  return;
}

void     fillpoly(int numpoints, AL_CONST int* polypoints)
{
	//int bi;
	////bi = setfillpattern_gregre_scaled(0.5, FALSE, 0.0, origin_x, origin_y, NULL, NULL); //option for filling solid, trace and disc with image
	polygon(screenplay, numpoints, polypoints, cur_fillstyle_color);
	////my_soft_polygon(screenplay, numpoints, polypoints, cur_fillstyle_color, translucency, colorB
}

void     my_fillpoly(int numpoints, AL_CONST int* polypoints, int translucency, int kolory_paper)
{
	int colorB;
	int kolor256, r, b, g;
	//int bi;
	////bi = setfillpattern_gregre_scaled(0.5, FALSE, 0.0, origin_x, origin_y, NULL, NULL); //option for filling solid, trace and disc with image
	//polygon(screenplay, numpoints, polypoints, cur_fillstyle_color);
	kolor256 = GetColorAC(kolory_paper);

	//r = _dac_normal[kolor256][0] << 2;
	//g = _dac_normal[kolor256][1] << 2;
	//b = _dac_normal[kolor256][2] << 2;

    r = 255;
    g = 255;
    b = 255;

	colorB = makeacol32(r, g, b, 255);

    if ((screenplay->w < 0) || (screenplay->w > 9999)) return;  //????????

	//polygon(screenplay, numpoints, polypoints, cur_fillstyle_color);  //this version can be used everywhere but not in hatching solids and trace solids
	my_soft_polygon(screenplay, numpoints, polypoints, cur_fillstyle_color, translucency, colorB);  //this version has to be always used in hatching solids and trace solids

    //if (TRANSLUCENCY!=translucency) set_trans_blender(0, 0, 0, (int)TRANSLUCENCY);  //coming back
	return;
}

void     fillpoly3D(int numpoints,  int *polypoints, float polypoints_z[48], int bg)
{
	V3D_f vt[48], *vtp[48];
	int i;
	int red, green, blue;
	int shade;
	float shade_d;
	FVECTOR v1, v2;
	float normal_x, normal_y, normal_z;
	BITMAP *texture;

	if (numpoints<3) return;


	texture = create_bitmap(32, 32);

	getcolor_RGB(&red, &green, &blue, cur_fillstyle_color);
	
	for (i=0; i<numpoints; i++)
	{ 
	  //0  1  2  3
      vt[i].x=*(polypoints+(i*2));     //0  2  4  6
	  vt[i].y=*(polypoints+(i*2)+1);   //1  3  5  7
      vt[i].z=polypoints_z[i];
	  vt[i].u=0;
	  vt[i].v=0;
	  vt[i].c=cur_fillstyle_color;
	}

	v1.x = vt[1].x - vt[0].x;
    v1.y = vt[1].y - vt[0].y;
    v1.z = vt[1].z - vt[0].z;
    v2.x = vt[2].x - vt[0].x;
    v2.y = vt[2].y - vt[0].y;
    v2.z = vt[2].z - vt[0].z;
    cross_product_f(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, &normal_x, &normal_y, &normal_z);
	normalize_vector_f(&normal_x, &normal_y, &normal_z);

	if (normal_z> 0)   //not perpendicular to the screen
	{
			shade_d=255.0*(normal_z);
			if (shade_d>255) shade=255; else shade=shade_d;
			clear_to_color(texture, makecol_depth(8, red*shade/64, green*shade/64, blue*shade/64));
    }
	else clear_to_color(texture,makecol_depth(8, 0, 0, 0));

	switch (numpoints)
	{
	case 3:
		//if (polygon_z_normal_f(&vt[0], &vt[1], &vt[2]) > 0)
		triangle3d_f(screenplay, POLYTYPE_ATEX_MASK /*POLYTYPE_FLAT*/ | POLYTYPE_ZBUF , texture, &vt[0], &vt[1], &vt[2]);
		break;
	case 4:
		//if (polygon_z_normal_f(&vt[0], &vt[1], &vt[2]) > 0)
        quad3d_f(screenplay, POLYTYPE_ATEX_MASK /*POLYTYPE_FLAT*/ | POLYTYPE_ZBUF , texture, &vt[0], &vt[1], &vt[2], &vt[3]);
		break;
	default:
		//if (polygon_z_normal_f(&vt[0], &vt[1], &vt[2]) > 0)
		//polygon3d_f(screenplay, POLYTYPE_GCOL | POLYTYPE_ZBUF, NULL, numpoints, &vt);
		for (i=0; i<numpoints; i++) vtp[i]=&vt[i];
		polygon3d_f(screenplay, POLYTYPE_ATEX_MASK /*POLYTYPE_FLAT*/ | POLYTYPE_ZBUF , texture, numpoints, vtp);
		break;
	}


destroy_bitmap(texture);

return;
}


void     drawpoly(int numpoints, int *polypoints)
{
	int i;
	int vx[4];
	int vy[4];
	int dy;
	int hy;

	if (numpoints == 4)
	{
		for (i = 0; i < numpoints; i++)
		{
			//0  1  2  3
			vx[i] = *(polypoints + (i * 2));     //0  2  4  6
			vy[i] = *(polypoints + (i * 2) + 1);   //1  3  5  7
		}
		if ((vx[0]==vx[3]) && (vy[0] == vy[1]) &&
			(vx[1] == vx[2]) && (vy[2] == vy[3]))
		{
			dy = abs(vy[0] - vy[3]);
			hy = min(vy[0], vy[3]) + 1;
			for (i = 0; i <= dy; i++)
			{
				hline(screenplay, vx[0], hy, vx[1], cur_color);
				hy += 1;
			}
		}

	}

  return;
}

void     setwritemode( int mode )
{
  if (mode>1) mode=0;
  global_mode = mode;
  drawing_mode(mode, NULL, 0, 0);
  return;
}

void     rectangle(int left, int top, int right, int bottom)
{
  rect(screenplay, left, top, right, bottom, cur_color);
  return;
}

void     bar(int left, int top, int right, int bottom)
{
#ifdef ALLEGRO5
  rectfill(screenplay, left, top, right, bottom, cur_fillstyle_color);
#else
    _normal_rectfill(screenplay, left, top, right, bottom, cur_fillstyle_color);
#endif
  return;
}

void     settextjustify(int horiz, int vert)
{
  cur_text_just_horiz=horiz;
  cur_text_just_vert=vert;
  return;
}

void  gettextjustify(int *horiz, int *vert)
{
	*horiz = cur_text_just_horiz;
	*vert = cur_text_just_vert;
	return;
}

void     moveto(int x, int y)
{
  cur_posx=x;
  cur_posy=y;
  return;
}


void     getimage(int left, int top, int right, int bottom, BITMAP  *bitmap)
{ int width, height;

  BITMAP *new_bitmap;
  BITMAP *old_bitmap;

  if (top < 0)
	  top=0;

  if (left < 0)
	  left = 0;

  width=right-left+1;
  height=bottom-top+1;

  old_bitmap = create_sub_bitmap(screen, left, top, width, height);
  new_bitmap = bitmap;
  clear_bitmap(new_bitmap);
  blit(old_bitmap, new_bitmap, 0, 0, 0, 0, width, height);
  destroy_bitmap(old_bitmap);

  return;
}

void  getimage2(int left, int top, int right, int bottom, BITMAP  *bitmap)
{ int width, height;
    BITMAP *new_bitmap;
    BITMAP *old_bitmap;
    BITMAP *sec_screen;

    sec_screen=(BITMAP*)get_second_screen();

    if (top < 0)
        top=0;

    if (left < 0)
        left = 0;

    width=right-left+1;
    height=bottom-top+1;

    old_bitmap = create_sub_bitmap(sec_screen, left, top, width, height);
    new_bitmap = bitmap;
    clear_bitmap(new_bitmap);
    blit(old_bitmap, new_bitmap, 0, 0, 0, 0, width, height);
    destroy_bitmap(old_bitmap);

    return;
}

void putimage(int left, int top,  BITMAP  *bitmap, int op)
{
  BITMAP *old_bitmap;
  BITMAP *new_bitmap;

  old_bitmap=bitmap;
  new_bitmap = create_sub_bitmap(screen, left, top, old_bitmap->w, old_bitmap->h);
  blit(old_bitmap, new_bitmap, 0, 0, 0, 0, old_bitmap->w, old_bitmap->h);
  destroy_bitmap(old_bitmap);
  destroy_bitmap(new_bitmap);
  return;
}

void putkeepimage(int left, int top,  BITMAP  *bitmap, int op)
{

    blit(bitmap, screen, 0, 0, left, top, bitmap->w, bitmap->h);

    return;
}

void putimage2(int left, int top,  BITMAP  *bitmap, int op)
{
    BITMAP *old_bitmap;
    BITMAP *new_bitmap;
    BITMAP *sec_screen;

    sec_screen=(BITMAP*)get_second_screen();

    old_bitmap=bitmap;
    new_bitmap = create_sub_bitmap(sec_screen, left, top, old_bitmap->w, old_bitmap->h);
    blit(old_bitmap, new_bitmap, 0, 0, 0, 0, old_bitmap->w, old_bitmap->h);
    destroy_bitmap(old_bitmap);
    destroy_bitmap(new_bitmap);
    return;
}

void putimage3(int left, int top, BITMAP* bitmap, int op)
{
	BITMAP* old_bitmap;
	BITMAP* new_bitmap;
	BITMAP* sec_screen;

	sec_screen = (BITMAP*)get_second_screen_bak_();
	if (sec_screen == NULL) return;

	old_bitmap = bitmap;
	new_bitmap = create_sub_bitmap(sec_screen, left, top, old_bitmap->w, old_bitmap->h);
	blit(old_bitmap, new_bitmap, 0, 0, 0, 0, old_bitmap->w, old_bitmap->h);
	destroy_bitmap(old_bitmap);
	destroy_bitmap(new_bitmap);
	return;
}

void putimage_preserve(int left, int top, BITMAP  *bitmap, int op)
{
	BITMAP *old_bitmap;
	BITMAP *new_bitmap;
	int width, height;
	
	old_bitmap = bitmap;

	width = old_bitmap->w;
	height = old_bitmap->h;

	if (top < 0)
	{
		height += top;
		top = 0;
	}

	if (left < 0)
	{
		width += left;
		left = 0;
	}

	new_bitmap = create_sub_bitmap(screen, left, top, width, height);

	//set_clip(new_bitmap, 0, 0, old_bitmap->w, old_bitmap->h);
	
	blit(old_bitmap, new_bitmap, 0, 0, 0, 0, width, height);
	//destroy_bitmap(old_bitmap);
	destroy_bitmap(new_bitmap);
	return;
}

int      getx(void)
{
  return cur_posx;
}

int      gety(void)
{
  return cur_posy;
}

void     GrMouseInitN(int queue_size)
{
  install_mouse();
  return;
}

void     GrMouseSetColors(GrColor fg,GrColor bg)
{
  return;
}

void     GrMouseDisplayCursor(void)
{
    //acquire_bitmap(screen);
    show_mouse((BITMAP*)screen);
}


static void sscare_mouse(void)
{
    show_mouse(NULL);
}

static void uunscare_mouse(void)
{
    acquire_bitmap(screen);
    show_mouse(screen);
    release_bitmap(screen);
}

void     GrMouseEraseCursor(void)
{
  scare_mouse();
}

void     GrMouseUnEraseCursor(void)
{
  unscare_mouse();
}

void     GrMouseWarp(int x,int y)
{
  position_mouse(x-viewport_x, y-viewport_y);
}

void GrMouseEventMode(int dummy)
{
  ;
}

void GrMouseInit(void)
{
  install_mouse();
}

void GrMouseUnInit(void)
{
  remove_mouse();
}


int my_poll_mouse(void)
{
	if (poll_m)
	   return poll_mouse();
	else return 0;
}

int my_poll_keyboard(void)
{
	if (poll_kbd)
       return poll_keyboard();
	else return 0;
}

void GrMouseGetEventT(int flags,GrMouseEvent *event,long timout_msecs)
{
  int p_mouse=my_poll_mouse();
  if (flags & GR_M_BUTTON_DOWN)
    {
      if (mouse_b & 1) event->flags += GR_M_LEFT_DOWN;
      if (mouse_b & 2) event->flags += GR_M_RIGHT_DOWN;
      if (mouse_b & 4) event->flags += GR_M_MIDDLE_DOWN;
    }
  if (flags & GR_M_MOTION)
    {
      event->flags += GR_M_MOTION;
      event->x = mouse_x + viewport_x;
      event->y = mouse_y + viewport_y;
    }
}

int  GrMouseDetect(void)
{
  return 1;   //tymczasowo
}

void GrMouseGetEvent(int flags,GrMouseEvent *event)
{
	int p_mouse = my_poll_mouse();
  if (flags & GR_M_BUTTON_DOWN)
    {
      if (mouse_b & 1) event->flags += GR_M_LEFT_DOWN;
      if (mouse_b & 2) event->flags += GR_M_RIGHT_DOWN;
      if (mouse_b & 4) event->flags += GR_M_MIDDLE_DOWN;
    }
  if (flags & GR_M_MOTION)
    {
      event->flags += GR_M_MOTION;
      event->x = mouse_x+viewport_x;
      event->y = mouse_y+viewport_y;
    }
}

void GrMouseRelease00(void)
{
  while (1)
  {
   int p_mouse = my_poll_mouse();
   if (mouse_b == 0) break;
  }
}

void GrMouseRelease(void)
{
	do
	{
		my_poll_mouse();
	} while (mouse_b > 0);
}

void GrMouseClear(void)
{  int xxx, yyy;
    int p_mouse = my_poll_mouse();
    get_mouse_mickeys(&xxx, &yyy);
	p_mouse = my_poll_mouse();
}

void GrMouseGetKeys(GrMouseEvent *event, int PozX, int PozY)
{   int tvret;

	event->flags=0;

	//int p_mouse = my_poll_mouse();  // WINE

	last_mouse_b = cur_mouse_b;
	cur_mouse_b = mouse_b;

	if ((cur_mouse_b & 1) && !(last_mouse_b & 1)) event->flags += 4;
	if ((cur_mouse_b & 2) && !(last_mouse_b & 2))
    {
        event->flags += 1;
        tvret=gettimeofday(&tvtimestamp, NULL);
    }
    if (tvtimestamp.tv_sec>0)
    {
        if (!(cur_mouse_b & 2))
              tvtimestamp.tv_sec=0;
        else if (cur_mouse_b & 1) {
            //check time again
            tvret=gettimeofday(&tvtimestamp1, NULL);
            if (((tvtimestamp1.tv_sec-tvtimestamp.tv_sec)*1e6 + (tvtimestamp1.tv_usec-tvtimestamp.tv_usec))>500000)
            {
                tvtimestamp.tv_sec=0;
            	while (mouse_b!=0) my_sleep(10);
                show__if_DEMO_RECORDING(819);
                int ret = Expand_flex();
                event->flags = 0;
                return;
            }
        }
    }

	if ((cur_mouse_b & 4) && !(last_mouse_b & 4)) event->flags += 2;

}

void GrMouseGetEvent1(int flags,GrMouseEvent *event)
{
	int p_mouse = my_poll_mouse();
  
  if (flags) // & GR_M_BUTTON_DOWN)
    {
      if (mouse_b & 1) event->flags += 4;
      if (mouse_b & 2) event->flags += 1;
      if (mouse_b & 4) event->flags += 2;

	  //czekanie na podniesienie klawisza
      //
	  if (mouse_b > 0) GrMouseRelease();
	  
    }
  if (flags & GR_M_MOTION)
    {
      event->flags += GR_M_MOTION;
      event->x = mouse_x+viewport_x;
      event->y = mouse_y+viewport_y;
    }
}

void     outtextxy(int x, int y0, const char *textstring)
{ int y;

  text_mode(-1);
  //z uwagi na roznice w wyswietlaniu tekstu w Allegro, nalezy podniesc tekst o ich wysokosc

  if (cur_text_just_vert!=0) { y=y0/*+text_height(font3)*/+dist_just;}
    else y=y0-text_height(font3)+dist_just;
  if (cur_text_just_horiz==0) textout(screen, font3, textstring, x/*-viewport_x*/, y/*-viewport_y*/, cur_color);
    else if (cur_text_just_horiz==1) textout_centre(screen, font3, textstring, x/*-viewport_x*/, y/*-viewport_y*/, cur_color);
      else if (cur_text_just_horiz==2) textout_right(screen, font3, textstring, x/*-viewport_x*/, y/*-viewport_y*/, cur_color);
        else textout(screen, font3, textstring, x/*-viewport_x*/, y/*-viewport_y*/, cur_color);

  moveto(x + textwidth(textstring), y0);
}

void     settextsettings(int font_number, int direction, int charsize, int horiz, int vert)
{
  Set_FONTNUMBER(font_number);
  FONT_DIRECTION=direction;
  font_charsize=charsize;
  cur_text_just_horiz=horiz;
  cur_text_just_vert=vert;
}

void     gettextsettings(struct textsettingstype  *texttypeinfo)
{
  texttypeinfo->font=get_FONTNUMBER();
  texttypeinfo->direction=0;
  texttypeinfo->charsize=font_charsize;  //get_HEIGHT();
  texttypeinfo->horiz=cur_text_just_horiz;
  texttypeinfo->vert=cur_text_just_vert;
}

void change_font(int nr_font)
{
 if (nr_font==0) font3 = font1;
   else font3 = font2;
}

void     settextstyle(int font, int direction, int charsize)
{
	;
}

int      textwidth(const char  *textstring)
{
  return my_text_length(font3, textstring);
}

void     setrgbpalette(int color, int red, int green, int blue)
{
    mypal[color].r=red;
    mypal[color].g=green;
    mypal[color].b=blue;
 
  if (color==255)  set_palette(mypal);
}

void     mysetrgbdefaults(void)
{ int i;

  for (i=0; i<256; i++)
   {
    mypal[i].r=_dac_normal[i][0];
    mypal[i].g=_dac_normal[i][1];
    mypal[i].b=_dac_normal[i][2];
   }
  
  set_palette(mypal);
}


void set_default_background()
{
	background_pcx=(BITMAP *)data[gregre].dat;
	set_background_menu((char*)"greygres");
}

void set_special_background(char *file_pcx)
{  PALLETE pal;
	int flags;
	char dir[MAXDIR];
	char drive[MAXDRIVE];
	char file[MAXFILE];
	char ext[MAXEXT];

	flags = fnsplit(file_pcx, drive, dir, file, ext);
	if (strcmp(strupr(ext), ".PCX") == NULL) background_pcx = load_pcx(file_pcx, pal);
	else if (strcmp(strupr(ext), ".PNG") == NULL) background_pcx = load_png(file_pcx, pal);
	   else if ((strcmp(strupr(ext), ".JPG") == NULL) || (strcmp(strupr(ext), ".JPEG") == NULL)) background_pcx = load_jpg(file_pcx, pal);

	if (background_pcx == NULL)
	{
		set_default_background();
		set_background_menu((char*)"greygres");
	}
	else
	{
		set_background_menu(file);
	}
}

typedef enum
{
	Dtp_wallpaper = 0,
	Dtp_autopanorama,
	Dtp_dynamic_menu,
	Dtp_desktop_cursor,
    Dtp_menu_cursor,
    Dtp_wheel_style,
    Dtp_desktop_instruction,
} Dtp_types;


static BOOL add_desktop(FILE *stru)
/*-------------------------------*/
{
	T_Fstring key_name;
	T_Fstring comment;
	int i;
    int WHEEL_STYLE;

	for (i = 0; i < no_desktop_data_param; i++)
	{
		key_name = desktop_data_param[i];
		comment = desktop_data_param_comment[i];
		switch (i)
		{
		case Dtp_wallpaper:
			if (EOF == fprintf(stru, "%s=%s\t\t%s\n",
				key_name, background_pcx_file, comment))
				return FALSE;
			break;
			/*
		case Dtp_screen_saver:
			if (EOF == fprintf(stru, "%s=%d\t\t%s\n",
				key_name, (int)(time_scrsave/60), comment))
				return FALSE;
			break;
			*/
		case Dtp_autopanorama:
			if (EOF == fprintf(stru, "%s=%g\t\t%s\n",
				key_name, pan_dxy, comment))
				return FALSE;
			break;
		case Dtp_dynamic_menu:
			if (EOF == fprintf(stru, "%s=%d\t\t%s\n",
				key_name, dynamic_menu, comment))
				return FALSE;
			break;
		case Dtp_desktop_cursor:
			if (EOF == fprintf(stru, "%s=%d\t\t%s\n",
				key_name, BIGCURSOR, comment))
				return FALSE;
			break;
        case Dtp_menu_cursor:
                if (EOF == fprintf(stru, "%s=%d\t\t%s\n",
                                   key_name, BAR_POINTER, comment))
                    return FALSE;
                break;
        case Dtp_wheel_style:
                if (Get_Mouse_Wheel()==1) WHEEL_STYLE=0;
                    else WHEEL_STYLE=1;
                if (EOF == fprintf(stru, "%s=%d\t\t%s\n",
                                   key_name, WHEEL_STYLE, comment))
                    return FALSE;
                break;
        case Dtp_desktop_instruction:
                if (EOF == fprintf(stru, "%s=%d\t\t%s\n",
                       key_name, desktop_instruction, comment))
                    return FALSE;
                break;
		default:
			break;
		}
	}
	if (EOF == fprintf(stru, "\n"))
		return FALSE;
	return TRUE;
}

void save_dialog_cursor(void)
{
	Add_Private_Profile_Group((T_Fstring)IC_DESKTOP, add_desktop);
}

void save_menu_cursor(void)
{
    Add_Private_Profile_Group((T_Fstring)IC_DESKTOP, add_desktop);
}

void save_mouse_wheel(void)
{
    Add_Private_Profile_Group((T_Fstring)IC_DESKTOP, add_desktop);
}

void save_desktop_instruction(void)
{
    Add_Private_Profile_Group((T_Fstring)IC_DESKTOP, add_desktop);
}

void reset_special_background(char *file_pcx)
{
	destroy_bitmap(background_pcx);
	set_special_background(file_pcx);
	strcpy(background_pcx_file, background_file_name_ptr);
	Add_Private_Profile_Group((T_Fstring)IC_DESKTOP, add_desktop);
}

void setfillpattern_gregre0(void)
{
 drawing_mode(DRAW_MODE_COPY_PATTERN, background_pcx, 0,0);
}

void set_mode_trans(void)
{
  drawing_mode(DRAW_MODE_TRANS, screenplay, 0, 0);   //TO DO URGENTLY
}

void set_mode_trans_prn(BITMAP *prn_bitmap)
{
	drawing_mode(DRAW_MODE_TRANS, prn_bitmap, 0, 0);
}

void set_mode_solid(void)
{

  drawing_mode(DRAW_MODE_SOLID, screenplay, 0, 0);  // TO DO URGENTLY

}

void set_mode_solid2(void)
{  BITMAP *sec_screen;

    sec_screen=(BITMAP*)get_second_screen();
    drawing_mode(DRAW_MODE_SOLID, sec_screen, 0, 0);  // TO DO URGENTLY
}

void set_mode_solid_prn(BITMAP* prn_bitmap)
{
	drawing_mode(DRAW_MODE_SOLID, prn_bitmap, 0, 0);
}

void set_mode_xor(void)
{
	drawing_mode(DRAW_MODE_XOR, screenplay, 0, 0);
}

void set_fill_color(int color)
{
  cur_fillstyle_color=palette_color[color];
}

void setfillstyle(int pattern, int color)
{
    if (abs(color)>255)
    {
        color=255;  //emergency
    }
  cur_fillstyle_pattern=pattern;
  if (pattern==1) drawing_mode(0, NULL, 0, 0);

  cur_fillstyle_color=palette_color[color];
  return;
}

void     setfillpattern( char  *upattern, int color)
{
  return;
}


int get_palette_color(int color)
{
    int p_color;
    p_color=palette_color[color];
    return p_color;
}


void set_cur_color(int color)
{
    cur_color=color;
}

int get_cur_color(void)
{
    return cur_color;
}

void     setcolor__(int color)
{
    if (color>255) ////!!!!!!
        color=0;
  cur_color=palette_color[color];
  __gr_color = color;
  return;
}

void     setlinestyle(int linestyle, unsigned upattern, int thickness)
{
  cur_linestyle_linestyle=linestyle;
  cur_linestyle_upattern=upattern;
  cur_linestyle_thickness=thickness;
  return;
}


void  GrDestroyContext(GrContext *context)
{
  return;
}



/*
 * Available frame modes (video memory layouts)
 */
typedef enum _GR_frameModes {
	/* ====== video frame buffer modes ====== */
	GR_frameUndef,                      /* undefined */
	GR_frameText,                       /* text modes */
	GR_frameHERC1,                      /* Hercules mono */
	GR_frameEGAVGA1,                    /* EGA VGA mono */
	GR_frameEGA4,                       /* EGA 16 color */
	GR_frameSVGA4,                      /* (Super) VGA 16 color */
	GR_frameSVGA8,                      /* (Super) VGA 256 color */
	GR_frameVGA8X,                      /* VGA 256 color mode X */
	GR_frameSVGA16,                     /* Super VGA 32768/65536 color */
	GR_frameSVGA24,                     /* Super VGA 16M color */
	GR_frameSVGA32L,                    /* Super VGA 16M color padded #1 */
	GR_frameSVGA32H,                    /* Super VGA 16M color padded #2 */
	/* ==== modes provided by the X11 driver ===== */
	GR_frameXWIN1   = GR_frameEGAVGA1,
	GR_frameXWIN4   = GR_frameSVGA4,
	GR_frameXWIN8   = GR_frameSVGA8,
	GR_frameXWIN16  = GR_frameSVGA16,
	GR_frameXWIN24  = GR_frameSVGA24,
	GR_frameXWIN32L = GR_frameSVGA32L,
	GR_frameXWIN32H = GR_frameSVGA32H,
        /* ==== modes provided by the WIN32 driver ===== */
        GR_frameWIN32_1   = GR_frameEGAVGA1,
        GR_frameWIN32_4   = GR_frameSVGA4,
        GR_frameWIN32_8   = GR_frameSVGA8,
        GR_frameWIN32_16  = GR_frameSVGA16,
        GR_frameWIN32_24  = GR_frameSVGA24,
        GR_frameWIN32_32L = GR_frameSVGA32L,
        GR_frameWIN32_32H = GR_frameSVGA32H,
	/* ==== linear frame buffer modes  ====== */
	GR_frameSVGA8_LFB,                  /* (Super) VGA 256 color */
	GR_frameSVGA16_LFB,                 /* Super VGA 32768/65536 color */
	GR_frameSVGA24_LFB,                 /* Super VGA 16M color */
	GR_frameSVGA32L_LFB,                /* Super VGA 16M color padded #1 */
	GR_frameSVGA32H_LFB,                /* Super VGA 16M color padded #2 */
	/* ====== system RAM frame buffer modes ====== */
	GR_frameRAM1,                       /* mono */
	GR_frameRAM4,                       /* 16 color planar */
	GR_frameRAM8,                       /* 256 color */
	GR_frameRAM16,                      /* 32768/65536 color */
	GR_frameRAM24,                      /* 16M color */
	GR_frameRAM32L,                     /* 16M color padded #1 */
	GR_frameRAM32H,                     /* 16M color padded #2 */
	GR_frameRAM3x8,                     /* 16M color planar (image mode) */
	/* ====== markers for scanning modes ====== */
	GR_firstTextFrameMode     = GR_frameText,
	GR_lastTextFrameMode      = GR_frameText,
	GR_firstGraphicsFrameMode = GR_frameHERC1,
	GR_lastGraphicsFrameMode  = GR_frameSVGA32H_LFB,
	GR_firstRAMframeMode      = GR_frameRAM1,
	GR_lastRAMframeMode       = GR_frameRAM3x8
} GrFrameMode;



GrContext *GrCreateFrameContext(GrFrameMode md,int w,int h,char far *memory[4],GrContext *where)
{
  return NULL;
}

GrContext *GrCreateContext(int w,int h,char far *memory[4],GrContext *where)
{
  return NULL;
}



void     GrSetContext(const GrContext *context)
{
  return;
}


void Set_Screenplay(BITMAP *ctx_bitmap)
{
  screenplay=ctx_bitmap;
}

BITMAP *Get_Screenplay(void)
{
    return screenplay;
}

void     GrClearContext(GrColor bg)
{
  clear_to_color(screenplay, bg);
  return;
}


void     GrClearContextMem(BITMAP *ctx_bitmap_mem, GrColor bg)
{
  clear_to_color(ctx_bitmap_mem, bg);
  return;
}


void     GrBitBlt(GrContext *dst,int x,int y,GrContext *src,int x1,int y1,int x2,int y2,GrColor op)
{
  return;
}


void     closegraph(void)
{
  //allegro_exit();
  return;
}


int      getmaxcolor(void)
{
  return 255;
}

void     setaspectratio( int xasp, int yasp )
{
  return;
}

int      getgraphmode(void)
{
  return 0; //get_display_switch_mode();
}

void     setbkcolor(int color)
{
  return;
}


int      __gr_White(void)
{
  return 255;
}


void GrClearScreen(GrColor bg)
{
  clear_to_color(screenplay, bg);
  return;
}

void ClearDevice(void)
{
  clear_bitmap(screen);
}


void     restorecrtmode(void)
{ int k;

 k=set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
 return;
}


int      installuserfont(const char *name)
{
  return 0;
}


void     GrHLine(int x1,int x2,int y,GrColor c)
{
  hline(screenplay, x1, y, x2, c);
  return;
}


void make_sprite_bmp(int dx, int dy, int nr)
{
	//set_color_depth(8);
	sprite_bmp[nr]=create_bitmap_ex(8, dx, dy);
    clear_bitmap(sprite_bmp[nr]);
}

void destroy_sprite_bmp(int nr)
{
	if (sprite_bmp[nr]!=NULL)
	{
	 destroy_bitmap(sprite_bmp[nr]);
	 sprite_bmp[nr]=NULL;
	}
}



void     GrFilledPolygon(int numpts,int minx,int miny,int maxx,int maxy, int *points,GrColor c, int nr)
{
	int i;
     
     polygon(screenplay, numpts, points, c);
	 return;

    
    if ((points[0] == points[2]) && (points[1] == points[3]))
    {
		points[2] = points[0] + 1;  ///?????
		points[3] = points[1] + 1;  ///?????
		i = 0;
    }

    
	if (sprite_bmp[nr] == NULL)
		make_sprite_bmp(maxx - minx + 2, maxy - miny + 2, nr);
	else clear_bitmap(sprite_bmp[nr]);
		//clear_to_color(sprite_bmp[nr], makecol(255, 0, 255));
	 
	sprite_bmp_x[nr]=points[0]-minx;
	sprite_bmp_y[nr]=points[1]-miny;


	polygon(sprite_bmp[nr], numpts, points, 0xFFFFFF);  //0xFFFFFF //255  


	draw_character(screenplay, sprite_bmp[nr], points[0]-sprite_bmp_x[nr], points[1]-sprite_bmp_y[nr], c);

    return;
}


void GrFilledPolygon1(const int *points, int dx, int dy, GrColor c, fixed angle)
{  int x0, y0;
   x0=points[0];
   y0=points[1];

   
   if ((dx_old!=dx) || (dy_old!=dy))
   {
	destroy_sprite_bmp(0);
	make_sprite_bmp(dx, dy, 0);
	clear_to_color(sprite_bmp[0],c);
   }

   else if (c_old!=c) clear_to_color(sprite_bmp[0],c);


	rotate_sprite(screenplay, sprite_bmp[0], x0, y0, angle);
	dx_old=dx;
	dy_old=dy;
	c_old=c;

    return;
}


int my_desktop_color_depth(void)
{
	return 8;
}


int get_desktop_color_depth(void)
{
	return my_desktop_color_depth();
}


void lock_mouse_switch(void)
{
  
 lock_mouse();

 }

void free_mouse_switch(void)
{

  _free_mouse();
	  
}

//#define BEEP

void beep(long sound)
{
#ifdef BEEP
#ifdef LINUX
    printf("\a")
#else
	MessageBeep(sound);
#endif
#endif
	;
}

#ifdef LINUX
/*
Returns the parent window of "window" (i.e. the ancestor of window
that is a direct child of the root, or window itself if it is a direct child).
If window is the root window, returns window.
*/
#ifndef MACOS
Window get_toplevel_parent(Display * display, Window window)
{
    Window wparent;
    Window root;
    Window * children;
    unsigned int num_children;
    char *name = NULL;

    XFetchName(display, window, &name);

    while (1) {
        if (0 == XQueryTree(display, window, &root,
                            &wparent, &children, &num_children)) {
            fprintf(stderr, "XQueryTree error\n");
            abort(); //change to whatever error handling you prefer
        }
        if (children) { //must test for null
            XFree(children);
        }
        if (window == root || wparent == root) {
            return window;
        }
        else {
            window = wparent;
        }
    }
}

Window get_my_window(Display * display, Window current, char *needle)
{

    /*
    Window retval, root, parent, *children;
    unsigned children_count;
    char *name = NULL;
    XTextProperty text_prop_return;

    //// Check if this window has the name we seek
    XGetWMName(display, current, &text_prop_return);

    if(XFetchName(display, current, &name) > 0) {
        int r = strcmp(needle, name);
        XFree(name);
        if(r == 0) {
            return current;
        }
    }

    retval = 0;

    //// If it does not: check all subwindows recursively.
    if(0 != XQueryTree(display, current, &root, &parent, &children, &children_count)) {
        unsigned i;
        for(i = 0; i < children_count; ++i) {
            Window win = get_my_window(display, children[i], needle);

            if(win != 0) {
                retval = win;
                break;
            }
        }
        XFree(children);
    }
    return retval;
    */
#ifdef ALLEGRO5
    return get_x_window_id();
#else
    return _xwin.window;
#endif
}

void MessageBeep(long message_sound)
{
    printf("\a");
}
#else

#endif
#endif

int Get_X11_SCREEN_SHIFT(void)
{
    return X11_SCREEN_SHIFT;
}

#ifndef LINUX
int Get_WIN_WINDOW_T_B(void)
{
	return WIN_WINDOW_T_B;
}
#endif

void move_pointer(int x, int y)
{

#ifndef LINUX

#else
#ifndef MACOS
    Display *display;
    Window focus, toplevel_parent_of_focus, root_window;
    XWindowAttributes attr;
    int ret;

    display = XOpenDisplay(0);

    //root_window=DefaultRootWindow(display);
    //focus=root_window; //ALLEGRO5
#ifndef ALLEGRO5
    focus=_xwin.window;
#else
    focus=get_x_window_id();
#endif

    toplevel_parent_of_focus = get_toplevel_parent(display, focus);
    ret = XGetWindowAttributes(display,  toplevel_parent_of_focus, &attr);

    XWarpPointer(display, focus, focus, 0, 0, 0, 0, x, y);

    XCloseDisplay(display);

#else

#endif
#endif
}

void move_pointer_on_display(int x, int y)
{

#ifndef LINUX

#else
#ifndef MACOS
    Display *display;
    Window focus, toplevel_parent_of_focus, root_window;
    XWindowAttributes attr;
    int ret;

    display = XOpenDisplay(0);

    XWarpPointer(display, None, None, 0, 0, 0, 0, x, y);

    XCloseDisplay(display);

#else

#endif
#endif
}

void reset_if_resized(void)
{

    int w, w_, h, h_;
    int ret1;

#ifndef LINUX
	HWND wnd;
	RECT wndRect, clnRect;

	//checkin window dimensions
	wnd = win_get_window();
	GetWindowRect(wnd, &wndRect);
	GetClientRect(wnd, &clnRect);

#else
#ifndef MACOS
    typedef struct tagRECT
    {
        LONG    left;
        LONG    top;
        LONG    right;
        LONG    bottom;
    } RECT;

    RECT wndRect, clnRect;

    Display *display;
    Window focus, toplevel_parent_of_focus, root_window;
    XWindowAttributes attr;
    int revert;
    int ret;

    display = XOpenDisplay(0);


    root_window=DefaultRootWindow(display);

    XRRScreenResources* sr = XRRGetScreenResources(display, root_window);
    XRRCrtcInfo* ci = XRRGetCrtcInfo(display, sr, sr->crtcs[0]);
    XRROutputInfo* oi = XRRGetOutputInfo(display, sr, sr->outputs[0]);


    //XGetInputFocus(display, &focus, &revert);
#ifndef ALLEGRO5
    focus=_xwin.window;
#else
    focus=get_x_window_id();
#endif
    toplevel_parent_of_focus = get_toplevel_parent(display, focus);
    ////toplevel_parent_of_focus = get_my_window(display, root_window, Window_Name);
    ret = XGetWindowAttributes(display,  toplevel_parent_of_focus, &attr);

    wndRect.left=attr.x;
    wndRect.top=attr.y;
    wndRect.right=attr.x+attr.width;
    wndRect.bottom=attr.y+attr.height;

    //TEMPORARAY // TO DO
    clnRect.left=attr.x;
    clnRect.top=attr.y;
    clnRect.right=attr.x+attr.width;
    clnRect.bottom=attr.y+attr.height;

    XRRFreeOutputInfo(oi);
    XRRFreeCrtcInfo(ci);
    XRRFreeScreenResources(sr);

    XCloseDisplay(display);
#else
    typedef struct tagRECT
	{
		LONG    left;
		LONG    top;
		LONG    right;
		LONG    bottom;
	} RECT;

	RECT wndRect, clnRect;

#endif
#endif

	w = wndRect.right - wndRect.left;
	w_ = clnRect.right - clnRect.left;
	h = wndRect.bottom - wndRect.top;
	h_ = clnRect.bottom - clnRect.top;

	if ((w != curr_w) || (h != curr_h) || (w_ != curr_w_) || (h_ != curr_h_))
	{
		dx_new = w_;
		dy_new = h_-X11_SCREEN_SHIFT;
		x0_new = wndRect.left;
		y0_new = wndRect.top;

		set_resized_window(dx_new, dy_new, 1);
		ret1 = set_window_origin(x0_new, y0_new);
	}

}

#ifndef LINUX
struct MonitorRects
{
    std::vector<RECT>   rcMonitors;

    static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
    {
        MonitorRects* pThis = reinterpret_cast<MonitorRects*>(pData);
        pThis->rcMonitors.push_back(*lprcMonitor);
        return TRUE;
    }

    MonitorRects()
    {
        EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)this);
    }
};
#endif

int get_window_origin_and_size(int *x_win_orig, int *y_win_orig, int *win_width, int *win_height)
{
#ifndef LINUX
    HWND wnd;
	GFX_MARGINS* gfx_margins;
	int ret;
	int margin_v, e_h=0, e_h1=0;  //e_h=0;
	HDC hdc;
	LPCRECT lprcClip;
	//MONITORENUMPROC lpfnEnum;
	LPARAM dwData;
	RECT actualscreen;
	int w1, h1;
#else
    typedef struct tagRECT
    {
        LONG    left;
        LONG    top;
        LONG    right;
        LONG    bottom;
    } RECT;
#endif

    RECT wndRect;
    RECT clnRect;
    int  w,h;

    int ret_ref = 0;


    if (GFX_WIN==1)
    {
#ifndef LINUX
        wnd = win_get_window(); //WINDOWS

    GetWindowRect(wnd, &wndRect);  //WINDOWS
    GetClientRect(wnd, &clnRect);  //WINDOWS

	ret = get_gfx_margins(&gfx_margins);
	ret_ref = gfx_margins->maximize_flag;

	BOOL found_mon = FALSE;
	int i;
	MonitorRects monitors;

	if (gfx_margins->maximize_flag == 2) {

		//check if multiple monitor
		//int sm_monitors = GetSystemMetrics(SM_CMONITORS);

		//MonitorRects monitors;
		//printf("%X\n", monitors.rcMonitors.size());
		for (i = 0; i < monitors.rcMonitors.size(); i++)
		{
			if (((wndRect.left + gfx_margins->l_b) >= monitors.rcMonitors[i].left) && ((wndRect.left + gfx_margins->l_b) <= monitors.rcMonitors[i].right) &&
				((wndRect.top + gfx_margins->t_b) >= monitors.rcMonitors[i].top) && ((wndRect.top + gfx_margins->t_b) <= monitors.rcMonitors[i].bottom))
			{
				found_mon = TRUE;
				if (i > 0) e_h1 = 0; // 1;  //????? check it out !!!
				break;
			}
		}

		margin_v = gfx_margins->t_b; e_h = 1;
	}
	else margin_v = 0;

	w = (int)(clnRect.right - clnRect.left);
	h = (int)(clnRect.bottom - clnRect.top) - margin_v - e_h;

	if (found_mon)
	{
		w1 = (int)(monitors.rcMonitors[i].right - monitors.rcMonitors[i].left);
		h1 = (int)(monitors.rcMonitors[i].bottom - monitors.rcMonitors[i].top) - gfx_margins->t_btb - e_h - e_h1;
		if (w1 < w)
			w = w1;
		if (h1 < h)
			h = h1;
	}


	*x_win_orig = (int)wndRect.left;
	*y_win_orig = (int)wndRect.top + margin_v;
	*win_width = w;  //client
	*win_height = h;  //client

	X11_SCREEN_SHIFT = (wndRect.bottom - wndRect.top) - (clnRect.bottom - clnRect.top);
	WIN_WINDOW_T_B = gfx_margins->t_b;


	gfx_margins->maximize_flag = 0;
#else
#ifndef MACOS
        Display *display;
        Window focus, toplevel_parent_of_focus, root_window;
        XWindowAttributes attr;
        int revert;
        int ret;

        display = XOpenDisplay(0);

        root_window=DefaultRootWindow(display);

        XRRScreenResources* sr = XRRGetScreenResources(display, root_window);
        XRRCrtcInfo* ci = XRRGetCrtcInfo(display, sr, sr->crtcs[0]);
        XRROutputInfo* oi = XRRGetOutputInfo(display, sr, sr->outputs[0]);

#ifndef ALLEGRO5
        focus=_xwin.window;
#else
        focus=get_x_window_id();
#endif
        toplevel_parent_of_focus = get_toplevel_parent(display, focus);
        ////toplevel_parent_of_focus = get_my_window(display, root_window, Window_Name);
        ret = XGetWindowAttributes(display,  toplevel_parent_of_focus, &attr);

        wndRect.left=attr.x;
        wndRect.top=attr.y;
        wndRect.right=attr.x+attr.width;
        wndRect.bottom=attr.y+attr.height;

        //TEMPORARAY // TO DO
        clnRect.left=attr.x;
        clnRect.top=attr.y;
        clnRect.right=attr.x+attr.width;
        clnRect.bottom=attr.y+attr.height;

        XRRFreeOutputInfo(oi);
        XRRFreeCrtcInfo(ci);
        XRRFreeScreenResources(sr);

        XCloseDisplay(display);

        w = (int)(clnRect.right - clnRect.left);
        h = (int)(clnRect.bottom - clnRect.top);

        *x_win_orig = (int)wndRect.left;
        *y_win_orig = (int)wndRect.top;
        *win_width = w;  //client
        *win_height = h;  //client

#ifdef ALLEGRO5
    if (!wayland)
            *y_win_orig+= X11_SCREEN_SHIFT;
#endif
#else
        ret_ref=al_get_window_origin_and_size(x_win_orig, y_win_orig, win_width, win_height);
    	return !ret_ref;
#endif
#endif
        return ret_ref;
    }
    return 0;
}


int get_window_new_origin_and_size(int *x_win_orig, int *y_win_orig, int *win_width, int *win_height)
{
    int ret_ref;
#ifdef MACOS
    ret_ref=al_get_window_new_origin_and_size(x_win_orig, y_win_orig, win_width, win_height);
	return !ret_ref;
#else
    return get_window_origin_and_size(x_win_orig, y_win_orig, win_width, win_height);
#endif
}


void lock_mouse_switch_callback(void)
{ int k;
    int ret;

    //when window is getting focus back
    //ret= get_window_origin_and_size(&x_win_orig_, &y_win_orig_, &win_width_, &win_height_);

    is_in=TRUE;

    hibernate=FALSE;

    printf("hibernate=%d\n", hibernate);

#ifdef MACOS
	//disable_hardware_cursor();
#endif

  return;

 set_sleep_state(FALSE);

 mouse_out = FALSE;

 dialog_cursor(0);
 lock_mouse();

 clean_key_buffer();
 clear_keybuf();
 do {
     Test_przycisniec(&k);
 } while (k!=0);

    free_mouse=0;

    go_reset_if_resized=TRUE;

    return;

 if (sw_callback_push)
	 {
		 simulate_keypress(14592);
		 sw_callback_push = FALSE;
	 }

 }

#ifndef LINUX
void set_focus(void)
{
	HWND HWnd;
	HWnd = win_get_window();
	Set_Focus(HWnd);
}
#endif

void set_myclient_flag(int client_no, unsigned char flag)
{
	set_client_flag(client_no, flag);
}

void convert_and_deposit_bitmap(BITMAP *PREVIEW, int x1, int y1, int x2, int y2, char *drawing_file, int client_number)
{
	PALLETE pal;
	int ret;
	char dump_file[32];
	sprintf(dump_file, "Temp/dump%03d%03d.bmp", Client_number, client_number);
	get_palette(pal);
	save_bitmap(dump_file, PREVIEW, pal);

	if (PREVIEW)
	{
		ret = deposit_hbitmap(dump_file, x1, y1, x2, y2, drawing_file);
	}

	return;
}

int return_active_clients(void)
{
	return return_active_clients_no();
}

int return_and_convert_bitmap(int client, char *dump_file, int *x1, int *y1, int *x2, int *y2, char *drawing_file)
{
	int ret;
	
	ret = return_hbitmap(client, dump_file, x1, y1, x2, y2, drawing_file);
	
	return ret;
}

void free_mouse_switch_callback(void)
{  int ret;
    //when window is loosing focus
    //ret= get_window_origin_and_size(&x_win_orig_, &y_win_orig_, &win_width_, &win_height_);
    is_in=FALSE;

    hibernate=TRUE;

    printf("hibernate=%d\n", hibernate);
#ifdef MACOS
	//enable_hardware_cursor();
#endif
   return;

  //free_mouse_switch();
  //////////////set_sleep_state(TRUE);
//#ifdef BEEP
 //MessageBeep(MB_OK);
  if (!preview_blocked)
  {
	 CopyPreview(0, 0, 320, 320, DRAWING_NUMBER);
  }
    //HWnd = win_get_window();
    //Set_Focus(HWnd);
   mouse_out = TRUE;

    ////serv134_135(FALSE);
    free_mouse = 1;
    _free_mouse();
    ////set_sleep_state(TRUE);
    ////clean_key_buffer();
    dialog_cursor(1);

   ////remove_keyimages(TRUE);


}

#ifdef LINUX
#ifndef MACOS
void xwin_set_hints(int dx, int dy) {
    _xwin_set_hints(dx, dy);
}


void get_display_window(void)
{
    main_display = XOpenDisplay(0);
    main_root_window=DefaultRootWindow(main_display);
}

void focus_display_window(void)
{
    XRaiseWindow(main_display,main_root_window);
}

Window get_allegro_window(void)
{
#ifdef ALLEGRO5
    return get_x_window_id();
#else
    return _xwin.window;
#endif
}

//#define GET_SHADED_STATE   //TO DO

#ifdef GET_SHADED_STATE
/* window states */

typedef enum {
    WINDOW_STATE_NONE               = 0,
    WINDOW_STATE_MODAL              = (1 << 0),
    WINDOW_STATE_STICKY             = (1 << 1),
    WINDOW_STATE_MAXIMIZED_VERT     = (1 << 2),
    WINDOW_STATE_MAXIMIZED_HORZ     = (1 << 3),
    WINDOW_STATE_MAXIMIZED          = (WINDOW_STATE_MAXIMIZED_VERT | WINDOW_STATE_MAXIMIZED_HORZ),
    WINDOW_STATE_SHADED             = (1 << 4),
    WINDOW_STATE_SKIP_TASKBAR       = (1 << 5),
    WINDOW_STATE_SKIP_PAGER         = (1 << 6),
    WINDOW_STATE_HIDDEN             = (1 << 7),
    WINDOW_STATE_FULLSCREEN         = (1 << 8),
    WINDOW_STATE_ABOVE              = (1 << 9),
    WINDOW_STATE_BELOW              = (1 << 10),
    WINDOW_STATE_DEMANDS_ATTENTION  = (1 << 11),
    WINDOW_STATE_FOCUSED            = (1 << 12),
    WINDOW_STATE_SIZE               = 13,
} window_state_t;

static char* WINDOW_STATE_NAMES[] = {
        "_NET_WM_STATE_MODAL",
        "_NET_WM_STATE_STICKY",
        "_NET_WM_STATE_MAXIMIZED_VERT",
        "_NET_WM_STATE_MAXIMIZED_HORZ",
        "_NET_WM_STATE_SHADED",
        "_NET_WM_STATE_SKIP_TASKBAR",
        "_NET_WM_STATE_SKIP_PAGER",
        "_NET_WM_STATE_HIDDEN",
        "_NET_WM_STATE_FULLSCREEN",
        "_NET_WM_STATE_ABOVE",
        "_NET_WM_STATE_BELOW",
        "_NET_WM_STATE_DEMANDS_ATTENTION",
        "_NET_WM_STATE_FOCUSED"
};


typedef struct {

    Display *dpy;
    Window id;

    struct {
        Atom NET_WM_STATE;
        Atom NET_WM_STATES[WINDOW_STATE_SIZE];
    } atoms;

} window_t;

int window_is_minimized(Display *display, Window window) {

    Atom wmState = XInternAtom(display, "_NET_WM_STATE", True);

    Atom actual_type;
    int actual_format;
    unsigned long i, num_items, bytes_after, num_states=0;
    Atom* states = NULL;
    long max_length = 1024;
    unsigned int flags = 0;
    unsigned char *properties = NULL;

    window_t win;
    win.dpy=display;
    win.id=window;

    int status;

    do {
        status = XGetWindowProperty(display, window, wmState, 0, max_length, False, AnyPropertyType, &actual_type, &actual_format, &num_items, &bytes_after, &properties);   //XA_ATOM
        if (status == Success && actual_type == 4 && properties && actual_format == 32 && num_items)  //XA_ATOM
        {
            {
                Atom *atoms = (Atom *) properties;
                int maximized = 0;
                int fullscreen = 0;

#define  ALFA_WINDOW_HIDDEN  0x00000008;             //< window is not visible
#define  ALFA_WINDOW_MINIMIZED  0x00000040;          //< window is minimized
#define  ALFA_WINDOW_MAXIMIZED  0x00000080;          //< window is maximized

                Atom _NET_WM_STATE_HIDDEN = XInternAtom(display, "_NET_WM_STATE_HIDDEN", True);
                Atom _NET_WM_STATE_MAXIMIZED_VERT = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", True);
                Atom _NET_WM_STATE_MAXIMIZED_HORZ = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", True);

                for (i = 0; i < num_items; ++i) {
                    if (atoms[i] == _NET_WM_STATE_HIDDEN) {
                        flags |= ALFA_WINDOW_HIDDEN;
                    } else if (atoms[i] == _NET_WM_STATE_MAXIMIZED_VERT) {
                        maximized |= 1;
                    } else if (atoms[i] == _NET_WM_STATE_MAXIMIZED_HORZ) {
                        maximized |= 2;
                    }
                }
                if (maximized == 3) {
                    flags |= ALFA_WINDOW_MAXIMIZED;
                }

            }
        }
       } while (bytes_after);

    XFree(properties);
    return 0;

}
#endif  //GET_SHADED_STATE

#ifdef ALLEGRO5
_xwin_type _xwin;
#endif

#else
void xwin_set_hints(int dx, int dy) {
		xwin_set_hints_(dx, dy);
	}
#endif
#endif

void set_attr_x_attr_y(int x, int y)
{
    attr_x = x;
    attr_y = y;
}

//#ifdef ALLEGRO5
//   _xwin_type _xwin;
//#endif

#ifdef MACOS
pid_t get_e_pid(void)
{
    FILE *etitle_file;
    char eTitle_file_name[64];
    char buff[16];

    sprintf(eTitle_file_name,"%s.pid",get_eTitle());
    etitle_file=fopen(eTitle_file_name,"r");
    if (etitle_file==NULL) return 0;
    fread(&buff, 1, 16, etitle_file);
    fclose(etitle_file);
    return atoi(buff);
}
#endif

void Check_ConfigureNotify(void)
{
    int ret;
#ifdef LINUX
#ifndef MACOS
    ///////////////////////
    XWindowAttributes attr;
    Window focus, toplevel_parent_of_focus;
    char params[128];
    char params1[128];
    char params2[128];
    int e_w, e_h, e_x, e_y;
    Display *display;

    _xwin_type *_xwin_ = &_xwin;

    if (edit_text_flag==1)  //text editor is open and no skip_taskbar
    {

        sprintf(params1, "%s(%d) — KDialog4alfa", _EDIT_TEXT_, Client_number);
        char *args[] = {
                "wmctrl",
                (char*)"-r",
                params1,
                (char*)"-b",
                (char*)"add,skip_taskbar",
                NULL
        };
        ret=wmctrl(5, args);

        edit_text_flag=0;
    }


if (ConfigureNotifySemaphore) {
#ifdef ALLEGRO5
    display = XOpenDisplay(0);
    focus = get_x_window_id();
    toplevel_parent_of_focus = get_toplevel_parent(display, focus);  //_xwin.display, _xwin.window
    ret = XGetWindowAttributes(display, toplevel_parent_of_focus, &attr);  //_xwin.display
    XCloseDisplay(display);
#else
    toplevel_parent_of_focus = get_toplevel_parent(_xwin.display, _xwin.window);
    ret = XGetWindowAttributes(_xwin.display,  toplevel_parent_of_focus, &attr);
#endif
}

    if ((attr.x!=attr_x) || (attr.y!=attr_y)) {
        printf("origin %d %d\n",attr.x-attr_x,attr.y-attr_y);

        get_editbox_size_origin(&e_w, &e_h, &e_x, &e_y);

        sprintf(params1, "%s(%d) — KDialog4alfa", _EDIT_TEXT_, Client_number);
        sprintf(params2, "0,%d,%d,-1,-1", attr.x-attr_x, attr.y-attr_y);
        char *args[] = {
                "wmctrl",
                (char*)"-r",
                params1,
                (char*)"-e",
                params2,
                NULL
        };
        ret=wmctrl(5, args);
        attr_x=attr.x;
        attr_y=attr.y;
    }


//#ifdef GET_SHADED_STATE
    //if (window_is_minimized(_xwin.display, toplevel_parent_of_focus))
#ifdef ALLEGRO5
        bool is_hidden_now=get_display_flags() & ALLEGRO_MINIMIZED;
        if (is_hidden_now != is_hidden)
        {
            // state is set
            if (is_hidden == FALSE)
            {
                sprintf(params1, "%s(%d) — KDialog4alfa", _EDIT_TEXT_, Client_number);
                sprintf(params2, "0,0,%d,-1,-1", hidden_dy);
                char *args[] = {
                        "wmctrl",
                        (char *) "-r",
                        params1,
                        (char *) "-e",
                        params2,
                        NULL
                };
                ret = wmctrl(5, args);
                is_hidden = TRUE;
            }
            else
            {
                sprintf(params1, "%s(%d) — KDialog4alfa", _EDIT_TEXT_, Client_number);
                sprintf(params2, "0,0,%d,-1,-1", -hidden_dy);
                char *args[] = {
                        "wmctrl",
                        (char *) "-r",
                        params1,
                        (char *) "-e",
                        params2,
                        NULL
                };
                ret = wmctrl(5, args);
                is_hidden = FALSE;
            }
        }
#else
    if (_xwin_->state_change_flag==1)
        { // state is set
            if (is_hidden==FALSE)
            {
                sprintf(params1, "%s(%d) — KDialog4alfa", _EDIT_TEXT_, Client_number);
                sprintf(params2, "0,0,%d,-1,-1", hidden_dy);
                char *args[] = {
                        "wmctrl",
                        (char*)"-r",
                        params1,
                        (char*)"-e",
                        params2,
                        NULL
                };
                ret=wmctrl(5, args);
                is_hidden=TRUE;
            }
        }
        else
        {
            if (is_hidden==TRUE)
            {
                sprintf(params1, "%s(%d) — KDialog4alfa", _EDIT_TEXT_, Client_number);
                sprintf(params2, "0,0,%d,-1,-1", -hidden_dy);
                char *args[] = {
                        "wmctrl",
                        (char*)"-r",
                        params1,
                        (char*)"-e",
                        params2,
                        NULL
                };
                ret=wmctrl(5, args);
                is_hidden=FALSE;
            }
        }
#endif

    ///////////////////
#else
    //ret=send_AppleScript(NULL);
	WATTR attr;
	int e_w, e_h, e_x, e_y;
	pid_t e_pid;

	if (edit_text_flag==1)
	{
		get_window_origin_and_size(&attr.x, &attr.y, &attr.w, &attr.h);
		attr_x=attr.x;
		attr_y=attr.y;
		edit_text_flag=2;
	}
	else if (edit_text_flag==2)  //text editor is open
	{
		e_pid=get_e_pid();
		if (e_pid==0) return;

		get_window_origin_and_size(&attr.x, &attr.y, &attr.w, &attr.h);
		if ((attr.x!=attr_x) || (attr.y!=attr_y))
			{
				printf("origin %d %d\n",attr.x-attr_x,attr.y-attr_y);

				get_editbox_size_origin(&e_w, &e_h, &e_x, &e_y);

				//version with ApplicationServices by Allegro-Legacy
				set_osx_window_position(e_pid, (attr.x-attr_x)/(RETINA+1), (attr.y-attr_y)/(RETINA+1), TRUE);
			    //version with Apple script
				////ret=send_AppleScript(e_pid, (attr.x-attr_x)/(RETINA+1), (attr.y-attr_y)/(RETINA+1), TRUE);

				attr_x=attr.x;
				attr_y=attr.y;
			}
	}

#endif
#else

HWND wnd;

if ((GFX_WIN == 1)  && (get_editor_on()))
{
    int curr_x0, curr_y0, curr_h, curr_v;
    int ret_ref;
    int ret_left_x, ret_right_x, ret_top_y, ret_bottom_y;
    int x, y;
    int width, height;
    int e_w, e_h, e_x, e_y;
    BOOL ret = FALSE;

    wnd = win_get_window();


    BOOL is_iconic = IsIconic(wnd);
    if (is_iconic)
    {
        if (is_hidden == FALSE)
        {
            if (get_editor_on())
            {
                SendMessage(get_editor_hWnd(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
                ret = TRUE;
            }
        }
        is_hidden = TRUE;
    }
    else
    {
        if (is_hidden == TRUE)
        {
            if (get_editor_on())
            {
                //SendMessage(get_editor_hWnd(), WM_SYSCOMMAND, SW_RESTORE, 0);
                ShowWindow(get_editor_hWnd(), TRUE);
                ret = TRUE;
            }
        }
        is_hidden = FALSE;
    
    }

    if (ret == TRUE) return;

    ret = FALSE;
    is_iconic = IsIconic(get_editor_hWnd());
    if (is_iconic)
    {
        if (is_hidden == FALSE)
        {
            if (get_editor_on())
            {
                SendMessage(wnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
                ret = TRUE;
            }
        }
        is_hidden = TRUE;
    }
    else
    {
        if (is_hidden == TRUE)
        {
            if (get_editor_on())
            {
                //SendMessage(get_editor_hWnd(), WM_SYSCOMMAND, SW_RESTORE, 0);
                ShowWindow(wnd, TRUE);
                ret = TRUE;
            }
        }
        is_hidden = FALSE;
    }

    if (ret == TRUE) return;

    ret_ref = get_window_origin_and_size(&curr_x0, &curr_y0, &curr_h, &curr_v);

    if ((curr_x0 == -32000) || (curr_y0 == -32000) || (curr_h == 0) || (curr_v == 0)) return;


    if ((curr_x0 != attr_x) || (curr_y0 != attr_y)) {

        x = curr_x0 - attr_x;
        y = curr_y0 - attr_y;

        printf("origin %d %d\n", x, y);

        //get_editbox_size_origin(&e_w, &w_h, &e_x, &e_y);


        int opt = 0;
        RECT lpRect = get_editbox_geometry_win(opt);
        e_x = lpRect.left;
        e_y = lpRect.top;

        HWND editor = get_editor_hWnd();

        ret = get_monitor_dims(&ret_left_x, &ret_right_x, &ret_top_y, &ret_bottom_y, -1);

        RECT rect;
        
        GetWindowRect(get_editor_hWnd(), &rect);

        int x_ = rect.left + x;
        int y_ = rect.top + y;
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        //char rap[64];

        //sprintf(rap,"%d, %d\n",x_,y_);
        //fputs(rap, stderr);

        if ((x_ + width) > ret_right_x) x_ = ret_right_x - width;
        if (x_ < ret_left_x) x_ = ret_left_x;
        if ((y_ + height) > ret_bottom_y) y_ = ret_bottom_y - height;
        if (y_ < ret_top_y) y_ = ret_top_y;


        ret_ref = MoveWindow(get_editor_hWnd(), x_, y_, width, height, TRUE);
         

        attr_x = curr_x0;
        attr_y = curr_y0;

    }

    
}

#endif
}


void trim_trailing_new_row(char *str) {
	if (str == NULL || *str == '\0') {
		return; // Handle null or empty strings
	}

	int i = strlen(str) - 1; // Start from the end of the string

	// Iterate backwards, identifying trailing new row characters
	while (i >= 0 && ((unsigned char)str[i])=='\n') {
		i--;
	}

	// If 'i' is less than the original last index,
	// it means trailing new row was found and skipped.
	// Terminate the string at the new end.
	if (i < strlen(str) - 1) {
		str[i + 1] = '\0';
	}
}

#define ALLEGRO5DND
void Check_XNextEvent(void) {

    char *fptr, *iptr, *bptr, *eptr;;
    int ret;
	int len;
	int n;

#ifdef LINUX
//#ifndef MACOS
    if (xdnd_buf.mflag==1)  //there is something new
    {
        CURL *curl = curl_easy_init();
        if (xdnd_buf.iflag==0)  //ready to read
        {
            xdnd_buf.iflag=1;
            fptr=xdnd_buf.filebuf;
        	len=strlen(xdnd_buf.filebuf);
        	eptr=fptr+len;
            iptr=strchr(fptr, '\r');
        	n=0;
#ifdef ALLEGRO5
            while ((iptr!=NULL) && (iptr<eptr) && (n<xdnd_buf.rows))
#else
            while ((iptr!=NULL) && (iptr<eptr))
#endif
            {
                *iptr='\0';

                bptr=strstr(fptr, "file://");
                if (bptr!=NULL) fptr+=7;

                char *decoded = curl_unescape(fptr, 0);

            	trim_trailing_new_row(decoded);

            	if (strlen(decoded) > 0)
            	{
            		ret=Test_App_Shm(2, decoded);
            		n++;
            	}
                curl_free(decoded);

                fptr=iptr+2;
                iptr=strchr(fptr, '\r');

            	//n++;
            }
            xdnd_buf.iflag=0;
        }
        xdnd_buf.mflag=0;

        curl_easy_cleanup(curl);

    	////simulate_keypress(27);  //to push
    }

//#else
//#endif
#else
	if ((xdnd_buf!=NULL) && (xdnd_buf->mflag == 1))  //there is something new
	{
		//CURL* curl = curl_easy_init();
		if (xdnd_buf->iflag == 0)  //ready to read
		{
			xdnd_buf->iflag = 1;
			fptr = xdnd_buf->filebuf;
			iptr = strchr(fptr, '\r');
			while (iptr != NULL)
			{
				*iptr = '\0';

				bptr = strstr(fptr, "file://");
				if (bptr != NULL) fptr += 7;

				//char* decoded = curl_unescape(fptr, 0);
				char* decoded = fptr;

				ret = Test_App_Shm(2, decoded);
				//printf("File: %s\n", decoded);
				//curl_free(decoded);

				fptr = iptr + 2;
				iptr = strchr(fptr, '\r');
			}
			xdnd_buf->iflag = 0;
		}
		xdnd_buf->mflag = 0;

		//curl_easy_cleanup(curl);
	}

#endif
}

int set_window_origin(int x_win_orig, int y_win_orig)
{
 #ifndef LINUX
    HWND wnd;
	
	RECT wndRect;
	RECT clnRect;
	
    int  w,h;

    set_attr_x_attr_y(x_win_orig, y_win_orig);


  if (GFX_WIN==1)
  {
    wnd = win_get_window(); 
    

    GetWindowRect(wnd, &wndRect); 
	GetClientRect(wnd, &clnRect);

	x_increase_arch = wndRect.left; 
    y_increase_arch = wndRect.top; 

   
    w  = wndRect.right      - wndRect.left; 
    h  = wndRect.bottom     - wndRect.top; 
   
 
    MoveWindow(wnd, x_win_orig, y_win_orig, w, h, TRUE); 

	return 1;
  }
 #else
#ifndef MACOS
    Display *display;
    Window focus, toplevel_parent_of_focus, root_window;
    int revert;
    int ret;

    char params1[128];
    char params2[128];

    XWindowAttributes attr;

    display = XOpenDisplay(0);

    //root_window=DefaultRootWindow(display);
    //focus=root_window;
 #ifndef ALLEGRO5

    root_window=DefaultRootWindow(display);

    focus=_xwin.window;

    int x, y;
    Window child;
    XWindowAttributes xwa;
    XTranslateCoordinates( display, focus, root_window, 0, 0, &x, &y, &child );
    XGetWindowAttributes( display, focus, &xwa );
    printf( "%d %d\n", x - xwa.x, y - xwa.y );

    toplevel_parent_of_focus = get_toplevel_parent(display, focus);

    ret = XGetWindowAttributes(display,  toplevel_parent_of_focus, &attr);

    ////this doesn't work since X.Org version: 21.1.18
    ////ret = XMoveWindow(display,toplevel_parent_of_focus, x_win_orig,  y_win_orig);
    ////XFlush(display); // or XSync(display, False);

    Window root, parent, *children = NULL;
    unsigned int num_children;

    if(!XQueryTree(display, focus, &root, &parent, &children, &num_children))
        parent = focus-2;

    if (children)
        XFree((char *)children);

    ////iot actually works since X.Org version: 21.1.18 but with parent not with focuse
    ret = XMoveWindow(display,parent, x_win_orig + 1,  y_win_orig + 1 - (wayland==1 ? X11_SCREEN_SHIFT : 0));   //added 1 due to frame of 1 pxl each side
    XFlush(display); // or XSync(display, False);

    XCloseDisplay(display);

    ////////////////////////
    my_sleep(10);

    /*  ////Alternative using wmctrl

    if ((attr.x!=x_win_orig) || (attr.y!=y_win_orig)) {
        printf("origin %d %d\n",attr.x-attr_x,attr.y-attr_y);

        sprintf(params1, "%d", parent);  //recuded by 2 to catch proper window  focus-2
        sprintf(params2, "0,%d,%d,-1,-1", x_win_orig-attr.x, y_win_orig-attr.y);  //added 1 to mode to make space for frame
        char *args[] = {
                "wmctrl",
                //(char*)"-r",
                //params1,
                (char*)"-ir",
                params1,
                (char*)"-e",
                params2,
                NULL
        };
        ret=wmctrl(5, args);

        my_sleep(100);

        attr.x=x_win_orig;
        attr.y=y_win_orig;

        attr_x=attr.x;
        attr_y=attr.y;
    }
     */
    /////////////////////////
 #else
    focus=get_x_window_id();
    set_window_position(x_win_orig, y_win_orig);
 #endif
#else
    pid_t pid=al_getpid();
#ifdef ARM64
	send_AppleScript(pid, x_win_orig/(RETINA+1), y_win_orig/(RETINA+1), FALSE);
#else
	set_osx_window_position(pid, x_win_orig/(RETINA+1), y_win_orig/(RETINA+1), FALSE);
#endif


	//set_window_origin(x_win_orig, y_win_orig);
#endif
#endif
  return 0;
} 

void get_cur_widt_height(int *cur_width, int *cur_height)
{
#ifndef LINUX
    HWND wnd;
   RECT wndRect; 

  if (GFX_WIN==1)
  {
    wnd = win_get_window(); 
	
	GetClientRect(wnd        , &wndRect); 
  
    *cur_width = (wndRect.right - wndRect.left); 
    *cur_height = (wndRect.bottom - wndRect.top);  

  }
#else
#ifndef MACOS
    Display *display;
    Window focus, toplevel_parent_of_focus, root_window;
    XWindowAttributes attr;
    int revert;
    int ret;

    display = XOpenDisplay(0);
    root_window=DefaultRootWindow(display);

#ifndef ALLEGRO5
    focus=_xwin.window;
#else
    focus=get_x_window_id();
#endif
    toplevel_parent_of_focus = get_toplevel_parent(display, focus);

    ret = XGetWindowAttributes(display,  toplevel_parent_of_focus, &attr);

    *cur_width=attr.width;
    *cur_height=attr.height;

    XSizeHints size_hints;
    size_hints.min_width = 640;
    size_hints.min_height = 480;
    size_hints.max_width = 6880; //6000; //2560;
    size_hints.max_height = 3000;//1440;
    XSetWMNormalHints(display, toplevel_parent_of_focus, &size_hints);

    XCloseDisplay(display);
#else

#endif
#endif

  return;
}

int centre_window(void)
{
#ifndef LINUX
    HWND wnd;
	HWND desktop;
	RECT wndRect, desktopRect, clnRect; 
    int  w,w_,h,h_,dw,dh;
	//int ddh;

  if (GFX_WIN==1)
  {
    wnd = win_get_window(); 
    desktop = GetDesktopWindow(); 
 
    GetWindowRect(wnd, &wndRect);
	GetClientRect(wnd, &clnRect); 
    GetWindowRect(desktop, &desktopRect); 
    
	
    w  = wndRect.right      - wndRect.left; 
	w_ = clnRect.right      - clnRect.left; 
    h  = wndRect.bottom     - wndRect.top; 
	h_ = clnRect.bottom     - clnRect.top;
	//ddh= 72-(h-h_);
    dw = desktopRect.right  - desktopRect.left; 
    dh = desktopRect.bottom - desktopRect.top; // - ddh; 
 
    MoveWindow(wnd, (dw-w)/2, (dh-h)/2, w, h, TRUE); 

	return 1;
  }
#else
#endif
  return 0;
}

void graphic_error(char *allerror)
{
	allegro_message("%s\n%s\n", _UNABLE_INSTALL_GRAPHIC_, allerror);
}

void increase_window(DRIVER_STRUCT *drv, int x, int y)
{
    #ifndef LINUX
    int draw_mode;
   int switch_callback_out, switch_callback_in;
  // int c;
  // int ret;
   int gfx_width;
   int gfx_height;
   int cur_width_d;
   int cur_height_d;
   int res_ret;
   HWND wnd;
   RECT wndRect, clnRect;
   int h,h_;
   int w,w_;

   
   
   res_ret=get_desktop_resolution(&cur_width_d, &cur_height_d);
   wnd = win_get_window();
   GetWindowRect(wnd, &wndRect);
   GetClientRect(wnd, &clnRect);

   w  = wndRect.right      - wndRect.left; 
   w_ = clnRect.right      - clnRect.left;
   h  = wndRect.bottom     - wndRect.top; 
   h_ = clnRect.bottom     - clnRect.top;
  
   cur_width_d-=(w-w_);
   cur_height_d-=(h-h_);
   
   set_color_depth(drv->gfx_bpp);
   request_refresh_rate(drv->gfx_refresh);

   h_increase+=(x*32);
   if (h_increase<192) h_increase=192;
   if (h_increase>cur_width_d) h_increase=cur_width_d;
 

   v_increase+=(y*32);
   if (v_increase<96) v_increase=96;
   if (v_increase>cur_height_d) v_increase=cur_height_d;
   
   gfx_width=h_increase;
   gfx_height=v_increase;

   
   window_was_resized=1;

 
   
   if (set_gfx_mode(GFX_GDI, drv->gfx_width, drv->gfx_height, 0, 0) != 0)
   {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	graphic_error(allegro_error);
    abort();
   }


   cur_colors_depth=drv->gfx_bpp;
    
   set_color_depth(drv->gfx_bpp);
	 


   draw_mode=set_display_switch_mode(drv->gfx_display_switch);

   switch_callback_out=set_display_switch_callback(SWITCH_OUT, free_mouse_switch_callback);    ///probably not necessary
   switch_callback_in=set_display_switch_callback(SWITCH_IN , lock_mouse_switch_callback);

   if (GFX_WIN==1) get_cur_widt_height(&cur_width, &cur_height);
	 else
	{
		cur_width=gfx_width;
		cur_height=gfx_height;
	}
   
   cur_colors=pow(2,drv->gfx_bpp);
   viewport_x=0;
   viewport_y=0;
   viewport_xk=cur_width;
   viewport_yk=cur_height;
  
 
   set_clip_rect(screen,0,0,getmaxx(),getmaxy());


   Copy_screen();
   screenplay=NONCLIPPED_SCREEN;

#else
#endif

   return;
}

void set_original_window_GFX(DRIVER_STRUCT *drv)
{
    int dx, dy;
    dx=drv->gfx_width;
    dy=drv->gfx_height;
    set_resized_window_GFX(drv, &dx, &dy, 1) ;

    return;

   int draw_mode;
   int switch_callback_out, switch_callback_in;
  
   int gfx_width;
   int gfx_height;
   int ret_ref;

   ret_ref = get_window_origin_and_size(&x_increase_arch, &y_increase_arch, &h_increase_arch, &v_increase_arch);
   
   set_color_depth(drv->gfx_bpp);
   request_refresh_rate(drv->gfx_refresh);

   h_increase=drv->gfx_width;
   v_increase=drv->gfx_height;  

   gfx_width=h_increase;
   gfx_height=v_increase;
   

   if (set_gfx_mode(GFX_GDI, drv->gfx_width, drv->gfx_height, 0, 0) != 0)
   {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	graphic_error(allegro_error);
    abort();
   }

   cur_colors_depth=drv->gfx_bpp;
    
   set_color_depth(drv->gfx_bpp);

   draw_mode=set_display_switch_mode(drv->gfx_display_switch);

   switch_callback_out=set_display_switch_callback(SWITCH_OUT, free_mouse_switch_callback);    ///probably not necessary
   switch_callback_in=set_display_switch_callback(SWITCH_IN , lock_mouse_switch_callback);

	if (GFX_WIN==1) get_cur_widt_height(&cur_width, &cur_height);
	 else
	{
		cur_width=drv->gfx_width;
		cur_height=drv->gfx_height;
	}

   cur_colors=pow(2,drv->gfx_bpp);
   viewport_x=0;
   viewport_y=0;
   viewport_xk=cur_width;
   viewport_yk=cur_height;

    set_clip_rect(screen,0,0,getmaxx(),getmaxy());

   Copy_screen();
   screenplay=NONCLIPPED_SCREEN;
  

   return;
}

void set_last_window_GFX(DRIVER_STRUCT *drv)
{  int dx, dy;
    dx=h_increase_arch;
    dy=v_increase_arch;
    set_resized_window_GFX(drv, &dx, &dy, 1) ;

   return;

   int draw_mode;
   int switch_callback_out, switch_callback_in;
 
   int gfx_width;
   int gfx_height;

   set_color_depth(drv->gfx_bpp);
   request_refresh_rate(drv->gfx_refresh);

   h_increase=h_increase_arch;
   v_increase=v_increase_arch;  
   window_was_resized=1;

   gfx_width=h_increase;
   gfx_height=v_increase;
   

   if (set_gfx_mode(GFX_GDI, drv->gfx_width, drv->gfx_height, 0, 0) != 0)
   {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	graphic_error(allegro_error);
    abort();
   }
   cur_colors_depth=drv->gfx_bpp;
    
   set_color_depth(drv->gfx_bpp);

   draw_mode=set_display_switch_mode(drv->gfx_display_switch);

   switch_callback_out=set_display_switch_callback(SWITCH_OUT, free_mouse_switch_callback);   ///probably not necessary
   switch_callback_in=set_display_switch_callback(SWITCH_IN , lock_mouse_switch_callback);

   if (GFX_WIN==1) get_cur_widt_height(&cur_width, &cur_height);
	 else
	{
		cur_width=drv->gfx_width;
		cur_height=drv->gfx_height;
	}

   cur_colors=pow(2,drv->gfx_bpp);
   viewport_x=0;
   viewport_y=0;
   viewport_xk=cur_width;
   viewport_yk=cur_height;
  

    set_clip_rect(screen,0,0,getmaxx(),getmaxy());

   Copy_screen();
   screenplay=NONCLIPPED_SCREEN;


   set_window_origin(x_increase_arch, y_increase_arch);

   return;
}

void set_resized_window_GFX(DRIVER_STRUCT *drv, int *dx, int *dy, int option)
{
    int draw_mode;
   int switch_callback_out, switch_callback_in;

   int gfx_width;
   int gfx_height;
   int res_ret;
   int cur_width_win;
   int cur_height_win;
   int w,w_,h,h_;
   int ret, ret2, ret_ref;

#ifndef LINUX
   HWND wnd;
   RECT wndRect, clnRect;

   res_ret=get_desktop_resolution(&cur_width_win, &cur_height_win);
   wnd = win_get_window();
   GetWindowRect(wnd, &wndRect);
   GetClientRect(wnd, &clnRect);
#else

   /*
    typedef struct tagRECT
    {
        LONG    left;
        LONG    top;
        LONG    right;
        LONG    bottom;
    } RECT;

    RECT wndRect, clnRect;

    Display *display;
    Window focus, toplevel_parent_of_focus;
    XWindowAttributes attr;
    int revert;
    int ret;

    display = XOpenDisplay(0);


    XRRScreenResources* sr = XRRGetScreenResources(display, DefaultRootWindow(display));
    XRRCrtcInfo* ci = XRRGetCrtcInfo(display, sr, sr->crtcs[0]);
    XRROutputInfo* oi = XRRGetOutputInfo(display, sr, sr->outputs[0]);


    //XGetInputFocus(display, &focus, &revert);
#ifndef ALLEGRO5
    focus=_xwin.window;
#else
    focus=get_x_window_id();
#endif
    toplevel_parent_of_focus = get_toplevel_parent(display, focus);
    ret = XGetWindowAttributes(display,  toplevel_parent_of_focus, &attr);

    wndRect.left=attr.x;
    wndRect.top=attr.y;
    wndRect.right=attr.x+attr.width;
    wndRect.bottom=attr.y+attr.height;

    //TEMPORARAY // TO DO
    clnRect.left=attr.x;
    clnRect.top=attr.y;
    clnRect.right=attr.x+attr.width;
    clnRect.bottom=attr.y+attr.height;

    XRRFreeOutputInfo(oi);
    XRRFreeCrtcInfo(ci);
    XRRFreeScreenResources(sr);

    XCloseDisplay(display);
*/
#endif


   h_increase=*dx;
   v_increase=*dy;

   gfx_width=*dx;
   gfx_height=*dy;

   if ((gfx_width == 0) || (gfx_height) == 0)  //it was minimalized
	   return;

   window_was_resized=1;

#ifndef ALLEGRO5
   if (set_gfx_mode(GFX_GDI, gfx_width, gfx_height, 0, 0) != 0)
   {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	graphic_error(allegro_error);
    abort();
   }
#else
    if (option==1)
    {
#ifdef ALLEGRO5
        set_semaphore(0);
        my_sleep(100);
#endif
        if (set_gfx_mode(GFX_GDI, gfx_width, gfx_height, 0, 0) != 0) {
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
            graphic_error(allegro_error);
            abort();
        }

        BOOL CORRECT_GFX=TRUE;

#ifdef ALLEGRO5
        int x0m, y0m, dxm, dym;
#ifdef MACOS
        BOOL ret3=acknowledge_resize();
#endif
        my_sleep(100);
        ret=get_window_origin_and_size(&x0m, &y0m, &dxm, &dym);
#ifndef MACOS
        dym-=X11_SCREEN_SHIFT;
#endif
        if (((drv->gfx_width!=dxm) || (drv->gfx_height!=dym))   && (CORRECT_GFX))
        {
            drv->gfx_width=dxm;
            drv->gfx_height=dym;
            if (set_gfx_mode(GFX_GDI, drv->gfx_width, drv->gfx_height, 0, 0) != 0)
            {
                set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
                graphic_error(allegro_error);

                exit(1);
            }
        }

        *dx=drv->gfx_width;
        *dy=drv->gfx_height;
#endif

#ifdef ALLEGRO5
        set_semaphore(1);
#endif
    }
    else {
        ////set_semaphore(0);
        my_sleep(10);
        al_resize_screen(*dx, *dy);
        drv->gfx_width = *dx;
        drv->gfx_height = *dy;
    }
#endif

#ifndef LINUX
        _sleep(0);
#endif

    BOOL CORRECT_GFX=TRUE;

#ifdef ALLEGRO5
   //set_display_icon(icon_alfacad_mem);
#endif


/*
#ifdef ALLEGRO5
    int x0m, y0m, dxm, dym;
#ifdef MACOS
    BOOL ret3=acknowledge_resize();
#endif
    ret=get_window_origin_and_size(&x0m, &y0m, &dxm, &dym);
#ifndef MACOS
    dym-=X11_SCREEN_SHIFT;
#endif
    if (((drv->gfx_width!=dxm) || (drv->gfx_height!=dym))   && (CORRECT_GFX))
    {
        drv->gfx_width=dxm;
        drv->gfx_height=dym;
        if (set_gfx_mode(GFX_GDI, drv->gfx_width, drv->gfx_height, 0, 0) != 0)
        {
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
            graphic_error(allegro_error);

            exit(1);
        }
    }
#endif
*/

    //if (gfx_capabilities & GFX_HW_MEM_BLIT)
    //    blit_hw = TRUE;

    cur_colors_depth=drv->gfx_bpp;
    set_color_depth(drv->gfx_bpp);

    h_increase=drv->gfx_width;
    v_increase=drv->gfx_height;

   draw_mode=set_display_switch_mode(drv->gfx_display_switch);

   switch_callback_out=set_display_switch_callback(SWITCH_OUT, free_mouse_switch_callback);   ///probably not necessary
   switch_callback_in=set_display_switch_callback(SWITCH_IN , lock_mouse_switch_callback);

   my_sleep(10);

    if (GFX_WIN==1) get_cur_widt_height(&cur_width, &cur_height);
	 else
	{
		cur_width=drv->gfx_width;
		cur_height=drv->gfx_height;
	}

   cur_width=*dx;
   cur_height=*dy;

   cur_colors=pow(2,drv->gfx_bpp);
   viewport_x=0;
   viewport_y=0;
   viewport_xk=cur_width;
   viewport_yk=cur_height;


   set_clip_rect(screen,0,0,getmaxx(),getmaxy());

   Copy_screen();
   screenplay=NONCLIPPED_SCREEN;

#ifndef LINUX
    HWND allegro_wnd = win_get_window();
   win_dc =  GetDC(allegro_wnd);
#endif


    //ret2=set_window_origin(X_WIN_ORIG_, Y_WIN_ORIG_);
    //ret2=centre_window();
    //ret_ref = get_window_origin_and_size(&x_increase_arch, &y_increase_arch, &h_increase_arch, &v_increase_arch);

    //saving window dimensions
#ifndef LINUX
    wnd = win_get_window();  //WINDOWS
   GetWindowRect(wnd, &wndRect); //WINDOWS
   GetClientRect(wnd, &clnRect); //WINDOWS

   curr_w = wndRect.right - wndRect.left;
   curr_w_ = clnRect.right - clnRect.left;
   curr_h = wndRect.bottom - wndRect.top;
   curr_h_ = clnRect.bottom - clnRect.top;
#else

    curr_w = h_increase_arch;
    curr_w_ = *dx;
    curr_h = v_increase_arch;
    curr_h_ = *dy;

#endif

#ifdef LINUX
   sleep(0);
#else
    _sleep(0);
#endif

#ifdef ALLEGRO5
    //restoring the very first row
    if (Komunikat_R0>0) komunikat0(Komunikat_R0);
      else if (get_komunikat_arc_bak()>=0)
      {
        komunikat0(0);
        ret = kom_arc_bak();
      }
        else komunikat0(0);
#endif

   return;
}

#define ERR_COULDNT_OPEN_X_DISPLAY 1
#define ERR_MONITOR_DNE 2

#ifdef LINUX
#ifndef MACOS
void XWindow_Name(int client_number) {
    sprintf(Window_Name, "AlfaCAD%d", client_number);
    strcpy(_xwin.application_name, Window_Name);
    strcpy(_xwin.window_title, Window_Name);
}
#else
void XWindow_Name(int client_number)
{
	sprintf(Window_Name, "AlfaCAD%d", client_number);
}
#endif
#endif


#ifdef LINUX
#ifndef MACOS
/*
int get_screen_dims(void)
        {
        Display *disp;
        XRRScreenResources *screen0;
        XRROutputInfo *info;
        XRRCrtcInfo *crtc_info;
        int iscres;
        int icrtc;

        disp = XOpenDisplay(0);
        //screen
        for (iscres = disp->noutput; iscres > 0; ) {
            --iscres;

            info = XRRGetOutputInfo (disp, screen0, screen0->outputs[iscres]);
            if (info->connection == RR_Connected) {
                for (icrtc = info->ncrtc; icrtc > 0;) {
                    --icrtc;

                    crtc_info = XRRGetCrtcInfo (disp, screen0, screen0->crtcs[icrtc]);
                    fprintf(stderr, "==> %dx%d+%dx%d\n", crtc_info->x, crtc_info->y, crtc_info->width, crtc_info->height);

                    XRRFreeCrtcInfo(crtc_info);
                }
            }
            XRRFreeOutputInfo (info);
        }
        XRRFreeScreenResources(screen0);

        XCloseDisplay(disp);

        return 0;
        }
*/

int get_monitor_dims(int *ret_left_x, int *ret_right_x, int *ret_top_y, int *ret_bottom_y, int mon) {
    int ret_left_x_ = INT_MAX;
    int ret_right_x_ = INT_MIN;
    int ret_top_y_ = INT_MAX;
    int ret_bottom_y_ = INT_MIN;
    _XDisplay *display;
    int i,j, output;
    Window *list;
    char *name;
    unsigned long len;
    int ret;
    XWindowAttributes attr;
    int w_is_hidden;

    typedef struct CrtRECT
    {
        int		    x, y;
        unsigned int    width, height;
    } CrtRECT;
    CrtRECT **crt_0=NULL;
    int crt_0_n=0;

    if (!(display = XOpenDisplay(0))) { return ERR_COULDNT_OPEN_X_DISPLAY; }

    ret_left_x_ = 99999;
    ret_right_x_ = 0;
    ret_top_y_ = 99999;
    ret_bottom_y_ = 0;

    Window window = DefaultRootWindow(display);
    XRRScreenResources *screenr = XRRGetScreenResources(display, window);

    // This is the key right here. Use XRRScreenResources::noutput
    output = screenr->noutput;

    if (mon<0) printf("All display:\n");
    else printf("Display %d: \n",mon);
    j=0;

    crt_0=(CrtRECT **)malloc(output*sizeof(char*));  //it's oversized
    
    for (i = 0; i < output; ++i) {
        XRROutputInfo* out_info = XRRGetOutputInfo(display, screenr, screenr->outputs[i]);

        if (NULL != out_info && out_info->connection == RR_Connected) {
            if ((j==mon) || (mon<0))
            {
                XRRCrtcInfo *crt_info = XRRGetCrtcInfo(display, screenr, out_info->crtc);
                //printf("%s\t%dx%d+%d+%d\n", out_info->name, crt_info->width, crt_info->height - X11_SCREEN_SHIFT, crt_info->x, crt_info->y);
                printf("%s\t<%d,%d><%d,%d>\n", out_info->name, crt_info->x, crt_info->y, crt_info->width, crt_info->height);
                if (crt_info->x < ret_left_x_)
                    ret_left_x_ = crt_info->x;
                if ((crt_info->x + crt_info->width) > ret_right_x_)
                    ret_right_x_ = (crt_info->x + crt_info->width);
                if (crt_info->y < ret_top_y_)
                    ret_top_y_ = crt_info->y;
                if ((crt_info->y + crt_info->height - X11_SCREEN_SHIFT) > ret_bottom_y_)
                    ret_bottom_y_ = (crt_info->y + crt_info->height - X11_SCREEN_SHIFT);
                //if (mon==0)
                //{
                    crt_0[crt_0_n]=(CrtRECT*)malloc(sizeof(CrtRECT));
                    crt_0[crt_0_n]->x=crt_info->x;
                    crt_0[crt_0_n]->y=crt_info->y;
                    crt_0[crt_0_n]->width=crt_info->width;
                    crt_0[crt_0_n]->height=crt_info->height;
                    crt_0_n++;
                //}
                XRRFreeCrtcInfo(crt_info);
            }
            j++;
        }

        XRRFreeOutputInfo(out_info);
    }

    list = (Window*)winlist(display,&len);

    for (i=0;i<(int)len;i++) {
        name = winame(display,list[i]);
        ret = XGetWindowAttributes(display,  list[i], &attr);
        ret = getprop (display, name, list[i], window, attr, &w_is_hidden);
        printf("-->%s<--   <%d,%d><%d,%d>\n",name,attr.x,attr.y,attr.x+attr.width,attr.y+attr.height);
        for (j=0; j<crt_0_n; j++) 
        {
            if ((mon == 0) || (mon < 0)) //default and any
            {
                if ((!w_is_hidden) && (
                        (strcmp(name, "plasmashell") == 0) ||
                        (strcmp(name, "gnome-shell") == 0) ||
                        (strcmp(name, "xfce4-panel") == 0) ||
                        (strcmp(name, "lxpanel") == 0) ||
                        (strcmp(name, "lxqt-panel") == 0) ||
                        (strcmp(name, "mate-panel") == 0) ||
                        (strcmp(name, "cinnamon") == 0) ||
                        (strcmp(name, "i3bar") == 0) ||
                        (strcmp(name, "tint2") == 0) ||
                        (strcmp(name, "lxpanel") == 0) ||
                        (strcmp(name, "polybar") == 0) ||
                        (strcmp(name, "lemonbar") == 0) ||
                        (strcmp(name, "dwmblocks") == 0))) {
                    //// DisplayPort-0	    <0,0><2560x1440>
                    ////-->plasmashell<--   <0,0><3440,1440>
                    ////-->plasmashell<--   <0,0><2560,44>
                    ////-->plasmashell<--   <0,0><2560,1440>
                    if ((attr.x >= crt_0[j]->x) && ((attr.x + attr.width) <= (crt_0[j]->x + crt_0[j]->width)) &&
                        (attr.y >= crt_0[j]->y) && ((attr.y + attr.height) <= (crt_0[j]->y + crt_0[j]->height))) {
                        ////if at the bottom
                        if ((attr.x >= crt_0[j]->x) && ((attr.x + attr.width) <= (crt_0[j]->x + crt_0[j]->width)) &&
                            (attr.y > crt_0[j]->y) && ((attr.y + attr.height) == (crt_0[j]->y + crt_0[j]->height)))
                            ret_bottom_y_ -= attr.height;
                        else ////if at the top
                        if ((attr.x >= crt_0[j]->x) && ((attr.x + attr.width) <= (crt_0[j]->x + crt_0[j]->width)) &&
                            (attr.y == crt_0[j]->y) && ((attr.y + attr.height) < (crt_0[j]->y + crt_0[j]->height)))
                            ret_top_y_ += attr.height;
                        else ////if on the left
                        if ((attr.x == crt_0[j]->x) && ((attr.x + attr.width) < (crt_0[j]->x + crt_0[j]->width)) &&
                            (attr.y >= crt_0[j]->y) && ((attr.y + attr.height) <= (crt_0[j]->y + crt_0[j]->height)))
                            ret_left_x_ += attr.width;
                        else ////if on the right
                        if ((attr.x > crt_0[j]->x) && ((attr.x + attr.width) == (crt_0[j]->x + crt_0[j]->width)) &&
                            (attr.y >= crt_0[j]->y) && ((attr.y + attr.height) <= (crt_0[j]->y + crt_0[j]->height)))
                            ret_right_x_ -= attr.width;
                    }
                }
            }
        }
        free(name);
    }

    for (j=0; j<crt_0_n; j++) free(crt_0[j]);
    if (crt_0) free(crt_0);

    XFree(list);

    XRRFreeScreenResources(screenr);
    XCloseDisplay(display);

    *ret_left_x = ret_left_x_;
    *ret_right_x = ret_right_x_;
    *ret_top_y = ret_top_y_;
    *ret_bottom_y = ret_bottom_y_;

    //printf(">>> %d, %d, %d, %d\n", ret_left_x_, ret_top_y_, ret_right_x_, ret_bottom_y_);
    return 0;
}

Window *winlist (Display *disp, unsigned long *len) {
    Atom prop = XInternAtom(disp,"_NET_CLIENT_LIST",False), type;
    int form;
    unsigned long remain;
    unsigned char *list;

    errno = 0;
    if (XGetWindowProperty(disp,XDefaultRootWindow(disp),prop,0,1024,False,XA_WINDOW,
                           &type,&form,len,&remain,&list) != Success) {
        perror("winlist() -- GetWinProp");
        return 0;
    }

    return (Window*)list;
}

char *winame (Display *disp, Window win) {
    Atom prop = XInternAtom(disp,"WM_CLASS",False), type;   //WM_CLASS WM_NAME
    int form;
    unsigned long remain, len;
    unsigned char *list;

    errno = 0;
    if (XGetWindowProperty(disp,win,prop,0,1024,False,XA_STRING,
                           &type,&form,&len,&remain,&list) != Success) {
        perror("winlist() -- GetWinProp");
        return NULL;
    }

    return (char*)list;
}

int getprop (Display *disp, char *name, Window win, Window root_window, XWindowAttributes &attr, int *is_hidden) {
    Atom prop = XInternAtom(disp,name,False), type;
    int form, r = 0;
    unsigned long len, remain;
    unsigned char *list;
    char *tname;
    XWindowAttributes attr0;
    int ret;
    int x, y;
    Window child;
    Atom actual_type;
    int actual_format;
    unsigned long num_items;
    unsigned long bytes_after;
    unsigned char *prop_data = NULL;


    ret = XTranslateCoordinates( disp, win, root_window, 0, 0, &x, &y, &child);
    ret = XGetWindowAttributes(disp,  win, &attr0);
    attr.x=x - attr0.x;
    attr.y=y - attr0.y;
    attr.width=attr0.width;
    attr.height=attr0.height;

    ////printf("%dx%d+%d+%d\n", attr.width, attr.height, x - attr.x, y - attr.y);

    Atom wm_state_atom = XInternAtom(disp, "_NET_WM_STATE", False);
    Atom wm_hidden_atom = XInternAtom(disp, "_NET_WM_STATE_HIDDEN", False);

    *is_hidden = 0;

    if (XGetWindowProperty(disp, win, wm_state_atom, 0L, ~0L, False, XA_ATOM,
                           &actual_type, &actual_format, &num_items, &bytes_after,
                           &prop_data) == Success && prop_data != NULL)
    {
        Atom *states = (Atom *)prop_data;
        for (unsigned long i = 0; i < num_items; ++i) {
            if (states[i] == wm_hidden_atom) {
                *is_hidden = 1;
                break;
            }
        }
        XFree(prop_data);

        ////if (is_hidden) printf("Window is hidden (minimized).\n");
        ////else printf("Window is not hidden.\n");
    }
    ////else fprintf(stderr, "Failed to get _NET_WM_STATE property or no data.\n");

    return r;
}

char *atomtype (Atom x) {
    char *type_ = (char *)malloc(32);
    switch (x) {
        case XA_PRIMARY:
            strcpy(type_,"XA_PRIMARY");
            break;
        case XA_SECONDARY:
            strcpy(type_,"XA_SECONDARY");
            break;
        case XA_ARC:
            strcpy(type_,"XA_ARC");
            break;
        case XA_ATOM:
            strcpy(type_,"XA_ATOM");
            break;
        case XA_CARDINAL:
            strcpy(type_,"XA_CARDINAL");
            break;
        case XA_INTEGER:
            strcpy(type_,"XA_INTEGER");
            break;
        case XA_STRING:
            strcpy(type_,"XA_STRING");
            break;
        case XA_WINDOW:
            strcpy(type_,"XA_WINDOW");
            break;
        case XA_WM_HINTS:
            strcpy(type_,"XA_WM_HINTS");
            break;
        default:
            sprintf(type_,"unlisted (%lu), see Xatom.h",x);
            break;
    }
    return type_;
}
#else
int get_monitor_dims(int *ret_left_x, int *ret_right_x, int *ret_top_y, int *ret_bottom_y, int mon)
	{  BOOL ret;

	   ret= al_get_monitor_dims( ret_left_x, ret_right_x, ret_top_y, ret_bottom_y);

	   //printf(">>> %d, %d, %d, %d\n", *ret_left_x, *ret_top_y, *ret_right_x, *ret_bottom_y);

	   return !ret;
	}
#endif
#endif

void change_mode_gr(DRIVER_STRUCT *drv)
{  int draw_mode;
   int switch_callback_out, switch_callback_in;
   int ret1=0;
   int ret2;
   int X_WIN_ORIG_, Y_WIN_ORIG_, WIN_WIDTH_, WIN_HEIGHT_;
   int dx_new0, dy_new0, x0_new0, y0_new0 ;
#ifndef LINUX
   HWND wnd, Dwnd;
   RECT wndRect, clnRect, DwndRect, DclnRect;
   RECT DskRect;
#endif
   int w, w_, h, h_, Dw, Dw_, Dh, Dh_, dw, dh, dh1, e_h;
   BOOL blit_hw = FALSE;
   int ret_ref;

   if (GFX_WIN==1)
   {
     Save_Last_Window_Settings();
   }

   GFX_WIN = 1;

#ifndef LINUX
   wnd = win_get_window(); //WINDOWS

   GetWindowRect(wnd, &wndRect); //WINDOWS
   GetClientRect(wnd, &clnRect); //WINDOWS

   int S_W, S_H;
   int ret = get_desktop_resolution(&S_W, &S_H);  //ALLEGRO

   Dwnd = GetDesktopWindow();  //WINDOWS
   GetWindowRect(Dwnd, &DwndRect); //WINDOWS
   GetClientRect(Dwnd, &DclnRect); //WINDOWS

   SystemParametersInfo(SPI_GETWORKAREA, NULL, &DskRect, NULL); //WINDOWS

   int sm_monitors = GetSystemMetrics(SM_CMONITORS);
   if (sm_monitors > 1)
   {
	   DskRect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	   DskRect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	   DskRect.right = DskRect.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
	   DskRect.bottom = DskRect.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
   }
   
   w = wndRect.right - wndRect.left;
   w_ = clnRect.right - clnRect.left;
   Dw = DwndRect.right - DwndRect.left;

   Dw_ = DskRect.right - DskRect.left;

   h = wndRect.bottom - wndRect.top;
   h_ = clnRect.bottom - clnRect.top;
   Dh = DwndRect.bottom - DwndRect.top;

   Dh_ = DskRect.bottom - DskRect.top;

   dx_back = w_;
   dy_back = h_;
   x0_back = wndRect.left;
   y0_back = wndRect.top;


   dw = (w - w_);
   dh = (h - h_);
   dh1 = (Dh - Dh_);

   if (dh1 == 0) e_h = 1;
   else e_h = - 1;

   x0_new = DskRect.left;
   y0_new = DskRect.top;

   x0_new -= dw / 2;

   dx_new = DskRect.right - DskRect.left;
   dy_new = DskRect.bottom - DskRect.top - dh + dw / 2 - e_h;
#else

    int ret_left_x, ret_right_x, ret_top_y, ret_bottom_y;
    int ret_left_x0, ret_right_x0, ret_top_y0, ret_bottom_y0;
    int monitor=0;
    int ret;

    ret = get_monitor_dims(&ret_left_x0, &ret_right_x0, &ret_top_y0, &ret_bottom_y0, monitor);
    ret = get_monitor_dims(&ret_left_x, &ret_right_x, &ret_top_y, &ret_bottom_y, -1);

    x0_new = ret_left_x;
    y0_new = 0;

    x0_new0 = ret_left_x0;
    y0_new0 = 0;

    dx_new = ret_right_x - ret_left_x;
    dy_new = ret_bottom_y - ret_top_y; // - dh + dw / 2 - e_h;

    dx_new0 = ret_right_x0 - ret_left_x0;
    dy_new0 = ret_bottom_y0 - ret_top_y0; // - dh + dw / 2 - e_h;

    dx_new -= 2;  //due to the frame
    dy_new -=1;

#endif
   if (GFX_WIN==1) 
   {
	   ret1=Load_Last_Window_Settings(&X_WIN_ORIG_, &Y_WIN_ORIG_, &WIN_WIDTH_, &WIN_HEIGHT_);
       if (ret1)
	   {
           WIN_WIDTH_-=2;  //It's necessary due to 1 pxl frame each side
           WIN_HEIGHT_-=1;

		   drv->gfx_width=WIN_WIDTH_;
		   drv->gfx_height=WIN_HEIGHT_;
           //dx_new = X_WIN_ORIG_;
           //dy_new = Y_WIN_ORIG_;
       }
       else
       {
           /*
           X_WIN_ORIG_ = x0_new0;
           Y_WIN_ORIG_ = y0_new0;
           WIN_WIDTH_ = dx_new0;
           WIN_HEIGHT_ = dy_new0;

           dx_new=dx_new0;
           dy_new=dy_new0;
           x0_new=x0_new0;
           y0_new=y0_new0;
            */

           ////this is for all monitors
           X_WIN_ORIG_ = x0_new;
           Y_WIN_ORIG_ = y0_new;
           WIN_WIDTH_ = dx_new;
           WIN_HEIGHT_ = dy_new;

           dx_new=dx_new;
           dy_new=dy_new;
           x0_new=x0_new;
           y0_new=y0_new;
       }
   }
   else
   {
	   X_WIN_ORIG_ = x0_new;
	   Y_WIN_ORIG_ = y0_new;
	   WIN_WIDTH_ = dx_new;
	   WIN_HEIGHT_ = dy_new;
   }

   if (drv->gfx_width > dx_new) 
	   drv->gfx_width = dx_new;

   if (drv->gfx_height > dy_new) 
	   drv->gfx_height = dy_new;

   if (X_WIN_ORIG_ < x0_new)  
	   X_WIN_ORIG_ = x0_new;
   if (Y_WIN_ORIG_ < y0_new)  
	   Y_WIN_ORIG_ = y0_new;

   if ((X_WIN_ORIG_ + drv->gfx_width) > (dx_new - x0_new)) 
	   X_WIN_ORIG_ -= ((X_WIN_ORIG_ + drv->gfx_width) - dx_new - x0_new);
   if ((Y_WIN_ORIG_ + drv->gfx_height) > (dy_new - y0_new)) 
	   Y_WIN_ORIG_ -= ((Y_WIN_ORIG_ + drv->gfx_height) - dy_new - y0_new);

   set_color_depth(drv->gfx_bpp);
   request_refresh_rate(drv->gfx_refresh);

#ifdef LINUX
#ifdef ALLEGRO5
#endif
    set_origins(X_WIN_ORIG_, Y_WIN_ORIG_);
#endif

   if (set_gfx_mode(GFX_GDI, drv->gfx_width, drv->gfx_height, 0, 0) != 0)
   {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	graphic_error(allegro_error);
	
	exit(1);
   }

#ifndef LINUX
    _sleep(0);
#endif

BOOL CORRECT_GFX=TRUE;

#ifdef ALLEGRO5
    int x0m, y0m, dxm, dym;
#ifdef MACOS
    BOOL ret3=acknowledge_resize();
#endif
    ret=get_window_origin_and_size(&x0m, &y0m, &dxm, &dym);
#ifndef MACOS
    dym-=X11_SCREEN_SHIFT;
#endif
    if (((drv->gfx_width!=dxm) || (drv->gfx_height!=dym))   && (CORRECT_GFX))
    {
        drv->gfx_width=dxm;
        drv->gfx_height=dym;
            if (set_gfx_mode(GFX_GDI, drv->gfx_width, drv->gfx_height, 0, 0) != 0)
        {
            set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
            graphic_error(allegro_error);

            exit(1);
        }
    }
#endif

   if (gfx_capabilities & GFX_HW_MEM_BLIT)
	   blit_hw = TRUE;

    x0_new=X_WIN_ORIG_;
    y0_new=Y_WIN_ORIG_;
    dx_new=drv->gfx_width;
    dy_new=drv->gfx_height;

#ifndef LINUX
    save_window_dim(x0_new, y0_new, dx_new, dy_new);
#else
	save_window_dim(x0_new, y0_new, dx_new, dy_new + Get_X11_SCREEN_SHIFT());
#endif

    drv->gfx_driver=GFX_GDI;
    
   cur_colors_depth=drv->gfx_bpp;
   set_color_depth(drv->gfx_bpp);

   h_increase=drv->gfx_width;
   v_increase=drv->gfx_height;

   window_was_resized=0;

   draw_mode=set_display_switch_mode(drv->gfx_display_switch);

   switch_callback_out=set_display_switch_callback(SWITCH_OUT, free_mouse_switch_callback);
   switch_callback_in=set_display_switch_callback(SWITCH_IN , lock_mouse_switch_callback);

#ifndef LINUX
   if (GFX_WIN==1) get_cur_widt_height(&cur_width, &cur_height);
	 else
	{
		cur_width=drv->gfx_width;
		cur_height=drv->gfx_height;
	}
#else
    cur_width=drv->gfx_width;
    cur_height=drv->gfx_height;
#endif

   cur_colors=pow(2,drv->gfx_bpp);
   viewport_x=0;
   viewport_y=0;
   viewport_xk=cur_width;
   viewport_yk=cur_height;

    set_clip_rect(screen,0,0,getmaxx(),getmaxy());

   Copy_screen();
   screenplay=NONCLIPPED_SCREEN;

#ifndef LINUX
    HWND allegro_wnd = win_get_window();
   win_dc =  GetDC(allegro_wnd);
#endif

   GFX_WIN = 1;

   if (GFX_WIN==1)
   {
	   if (ret1) ret2=set_window_origin(X_WIN_ORIG_, Y_WIN_ORIG_);
	     else ret2=centre_window();
	   ret_ref = get_window_origin_and_size(&x_increase_arch, &y_increase_arch, &h_increase_arch, &v_increase_arch);
   }

   //saving window dimensions
#ifndef LINUX
   wnd = win_get_window();  //WINDOWS
   GetWindowRect(wnd, &wndRect); //WINDOWS
   GetClientRect(wnd, &clnRect); //WINDOWS

   curr_w = wndRect.right - wndRect.left;
   curr_w_ = clnRect.right - clnRect.left;
   curr_h = wndRect.bottom - wndRect.top;
   curr_h_ = clnRect.bottom - clnRect.top;
#else

    curr_w = h_increase_arch;
    curr_w_ = dx_new;
    curr_h = v_increase_arch;
    curr_h_ = dy_new;

#endif
#ifdef LINUX
    sleep(0);
#else
    _sleep(0);
#endif

   return;
}

void     set_BGI_mode_whc(int *gd, int *gm, int width, int height, int colors)
{
   int switch_callback_out, switch_callback_in;


#ifdef fullcolor
  cur_colors_depth=16; //8;
#else
	 cur_colors_depth=my_desktop_color_depth();
  
#endif
     
  set_color_depth(cur_colors_depth);

  if (set_gfx_mode(GFX_GDI, width, height, 0, 0)!=0) {
     set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 graphic_error(allegro_error);
     exit(0);}

  switch_callback_out=set_display_switch_callback(SWITCH_OUT, free_mouse_switch_callback);    ///probably not necessary
  switch_callback_in=set_display_switch_callback(SWITCH_IN , lock_mouse_switch_callback);

  if (GFX_WIN==1) get_cur_widt_height(&cur_width, &cur_height);
	 else
	{
		cur_width=width;
		cur_height=height;
	}

  cur_colors=colors;
  viewport_x=0;
  viewport_y=0;
  viewport_xk=cur_width;
  viewport_yk=cur_height;


    set_clip_rect(screen,0,0,getmaxx(),getmaxy());

  Copy_screen();
  screenplay=NONCLIPPED_SCREEN;
 
  return;
}


void     cleardevice(void)
{
  clear_to_color(screenplay, 0);
  return;
}

void     mycirclefill(int x1, int y1, int radius)
{
	circlefill(screenplay, x1/*+viewport_x*/, y1/*+viewport_y*/, radius, cur_color);
	return;
}

void     myarc(int x1, int y1, fixed ang1, fixed ang2, int radius)
{
	arc(screenplay, x1/*+viewport_x*/, y1/*+viewport_y*/, ang1, ang2, radius, cur_color);
	return;
}

void     myline(int x1, int y1, int x2, int y2)
{
  line(screenplay, x1/*+viewport_x*/, y1/*+viewport_y*/, x2/*+viewport_x*/, y2/*+viewport_y*/, cur_color);
  return;
}

void     myrectfill(int x1, int y1, int x2, int y2)
{
  rectfill(screenplay, x1/*+viewport_x*/, y1/*+viewport_y*/, x2/*+viewport_x*/, y2/*+viewport_y*/, cur_color);
  return;
}

void     mycircle(int x, int y, int radius)
{
  circle(screenplay, x/*+viewport_x*/, y/*+viewport_y*/, radius, cur_color);
  return;
}

void     myputpixel(int x, int y, int color)
{
  putpixel(screenplay, x/*+viewport_x*/, y/*+viewport_y*/, color);
  return;
}

void     outtext(const char *textstring)
{ int y;
  int text_h, text_v;

 TTF_text_UI(textstring, cur_posx, cur_posy, &text_h, &text_v);

 moveto(cur_posx + text_h, cur_posy);

 return;
}

void outtext_(BITMAP *ui_screen, const char *textstring)
{ int y;
    int text_h, text_v;

    TTF_text_UI_(ui_screen, textstring, cur_posx, cur_posy, &text_h, &text_v);

    moveto(cur_posx + text_h, cur_posy);

    return;
}

void     setgraphmode(int mode)
{  int gfx_w, gfx_h, gfx_bpp;
   int MY_GFX;

   cur_colors_depth=my_desktop_color_depth();
     
   set_color_depth(cur_colors_depth);
   
      gfx_w = cur_width;
      gfx_h = cur_height;
      gfx_bpp = bitmap_color_depth(screen);
      if (!gfx_mode_select_ex(&MY_GFX/*GFX_SAFE*/, &gfx_w, &gfx_h, &gfx_bpp))
        exit(0);

     if (set_gfx_mode(GFX_GDI /*GFX_SAFE*/, cur_width, cur_height, 0, 0)!=0) {
		 graphic_error(allegro_error);
         exit(0);}

     viewport_x=0;
     viewport_y=0;
     viewport_xk=cur_width;
     viewport_yk=cur_height;

    set_clip_rect(screen,0,0,getmaxx(),getmaxy());

  Copy_screen();
  screenplay=NONCLIPPED_SCREEN;

}

void     myfade_out(int speed)
{
  return;
}

GrColor GrBlack(void)
{
  return 0;
}

void    GrSetColor(GrColor c,int r,int g,int b)
{
  return;
}

GrColor GrAllocColor(int r,int g,int b)   /* shared, read-only */
{
  return 0;
}

int setrgbcolor(int r, int g, int b)
{
 
  return 0;
  //return __gr_setrgbcolor(r,g,b);
}

void Copy_screen(void)
{
  NONCLIPPED_SCREEN=screen;
}

void switch_screen(void)
{
	//screenplay = screen;
	screenplay = NULL;
}

void my_blit(BITMAP *ctx_bitmap,int mmx0,int mmy0,int mmx1,int mmy1,int dxx,int dyy)
{
  if (ctx_bitmap == NULL) return;
#ifdef ALLEGRO5
  blit(ctx_bitmap,screen,mmx0,mmy0,mmx1,mmy1,dxx,dyy);
#else
    blit(ctx_bitmap,screen,mmx0,mmy0,mmx1,mmy1-viewport_y,dxx,dyy);
#endif
}

void my_blit_s(BITMAP *ctx_bitmap,int mmx0,int mmy0,int mmx1,int mmy1,int dxx,int dyy)
{
    if (ctx_bitmap == NULL) return;
    blit(ctx_bitmap,screenplay,mmx0,mmy0,mmx1,mmy1-viewport_y,dxx,dyy);
}

void my_blit_sd(BITMAP *src_bitmap, BITMAP *dst_bitmap, int mmx0,int mmy0,int mmx1,int mmy1,int dxx,int dyy)
{
    if ((src_bitmap == NULL) || (dst_bitmap == NULL)) return;
    blit(src_bitmap,dst_bitmap,mmx0,mmy0,mmx1,mmy1-viewport_y,dxx,dyy);
}

void my_blit_anim(BITMAP* ctx_bitmap, int mmx0, int mmy0, int mmx1, int mmy1, int dxx, int dyy)
{
	int i,j;
	if (ctx_bitmap == NULL) return;
	global_pan_dx = -global_pan_dx;

	do
	{
		i = global_pan_dx;
		j = global_pan_dy;
		
		blit(ctx_bitmap, screen, mmx0, mmy0, mmx1 + i, mmy1 - viewport_y + j, dxx - i, dyy - j);
		
		if (global_pan_dx !=0 ) global_pan_dx += global_pan_dx < 0 ? 1 : -1;
		if (global_pan_dy != 0) global_pan_dy += global_pan_dy < 0 ? 1 : -1;
		
	} while ((global_pan_dx != 0) || (global_pan_dy != 0));
}

void my_blit_reverse(BITMAP *ctx_bitmap,int mmx0,int mmy0,int mmx1,int mmy1,int dxx,int dyy)
{
  if (ctx_bitmap == NULL) return;
  blit(screen,ctx_bitmap,mmx0,mmy0,mmx1,mmy1-viewport_y,dxx,dyy);
}

void my_blit_mem(BITMAP *ctx_bitmap,BITMAP *ctx_bitmap_mem, int mmx0,int mmy0,int mmx1,int mmy1,int dxx,int dyy)
{
  if (ctx_bitmap == NULL) return;
  blit(ctx_bitmap,ctx_bitmap_mem,mmx0,mmy0,mmx1,mmy1/*-viewport_y*/,dxx,dyy);
}

void my_stretch_blit(BITMAP *ctx_bitmap,int mmx0,int mmy0,int dxx0, int dyy0,int mmx1,int mmy1,int dxx,int dyy)
{
  if (ctx_bitmap == NULL) return;
  stretch_blit(ctx_bitmap,screenplay,mmx0,mmy0,dxx0,dyy0,mmx1,mmy1-viewport_y,dxx,dyy);
}

int Bytes_Per_Pixel(int color_depth)
{
  return BYTES_PER_PIXEL(color_depth);
}

int Save_Screen(void)
{

  if (bmp_save!=NULL) destroy_bitmap(bmp_save);
  bmp_save = create_bitmap(getmaxx(), getmaxy());
  blit(screen, bmp_save, 0, 0, 0, 0, getmaxx()+1, getmaxy()+1);

  return 0;
}

int Restore_Screen(void)
{

  blit(bmp_save, screen, 0, 0, 0, 0, getmaxx()+1, getmaxy()+1);
  destroy_bitmap(bmp_save);
  bmp_save=NULL;
  return 0;
}

void Al_Load_PCX_fade(char *pcx_name, int x, int y, int d_x, int d_y)
{
  BITMAP *bmp;
  RGB pal;

  bmp = load_bitmap(pcx_name, &pal);
  if (bmp != NULL)
  {
   fade_out(64);
   blit(bmp, screen, 0, 0, x, y, d_x, d_y);
  
   fade_in(&pal, 8);
   delay(500);

   destroy_bitmap(bmp);
   fade_out(16);
   GrClearContext(0);
  }
}

char *lang_sufix[]={"","PL","UA","ES"};

#define DAYS_DELAY  1 //2
#define CHECK_DELAY  7

static BOOL get_cloud(T_Fstring key_name, T_Fstring ret_string)
/*-----------------------------------------------------------*/
{
    int val_int;
    long val_long;
    float val_float;
    char *encoded_password;
    char *decoded_password;
    char cloud_password_encoded[64]="";

    //strupr(key_name);
    utf8Upper(key_name);
    //   strupr (ret_string);
    if (ret_string == NULL) return TRUE;

    if (stricmp(key_name, IC_CLOUD_CONNECT) == 0)
    {
        if ( sscanf ( ret_string, "%d", &val_int) == 1 )
        {
            cloud_connect = val_int;
        }
    }
    else
    if (stricmp(key_name, IC_CLOUD_USER) == 0)
    {
        strcpy(cloud_user, ret_string);
        strcpy(cloud_user0, ret_string+1);
        cloud_user0[strlen(cloud_user0)-1]='\0';
    }
    else
    if (stricmp(key_name, IC_CLOUD_PASSWORD) == 0)
    {
        strcpy(cloud_password, ret_string);

        strcpy(cloud_password0, ret_string+1);
        cloud_password0[strlen(cloud_password0)-1]='\0';

        /*encrypted version, if would be needed*/
        //encoded_password=encodePassword(cloud_password0, NULL);
        //printf("%s\n",encoded_password);
        //decoded_password=decodePassword(encoded_password, NULL);
        //printf("%s\n",decoded_password);
    }
    if (stricmp(key_name, IC_CLOUD_SHARE_URL) == 0)
    {
        strcpy(cloud_share_url, ret_string);
        strcpy(cloud_share0_url, ret_string+1);
        cloud_share0_url[strlen(cloud_share0_url)-1]='\0';
    }
    if (stricmp(key_name, IC_CLOUD_ADS) == 0)
    {
        strcpy(cloud_ads, ret_string);
        strcpy(cloud_ads0, ret_string+1);
        cloud_ads0[strlen(cloud_ads0)-1]='\0';
    }
    if (stricmp(key_name, IC_CLOUD_ADS_URL) == 0)
    {
        strcpy(cloud_ads_url, ret_string);
        strcpy(cloud_ads0_url, ret_string+1);
        cloud_ads0_url[strlen(cloud_ads0_url)-1]='\0';
    }
    if (stricmp(key_name, IC_CLOUD_UPGRADE) == 0)
    {
        strcpy(cloud_upgrade, ret_string);
        strcpy(cloud_upgrade0, ret_string+1);
        cloud_upgrade0[strlen(cloud_upgrade0)-1]='\0';
    }
    if (stricmp(key_name, IC_CLOUD_UPGRADE_URL) == 0)
    {
        strcpy(cloud_upgrade_url, ret_string);
        strcpy(cloud_upgrade0_url, ret_string+1);
        cloud_upgrade0_url[strlen(cloud_upgrade0_url)-1]='\0';
    }
    return TRUE;
}

int extract_files_from_stream(char *file_in, char *file_out, char *markers[], char *file_prefix)
{   FILE *f_in, *f_out;
	struct stat st_ads = {0};
	char *ptr1, *ptr2, *ptr3;
    long size;
	int ret;
	char *filemem;
	size_t result;
	char *file_name;
	char *file_date;
	char *file_size;
	char *file_type;
	char row[255]="";

	ret = stat(file_in,  &st_ads);

	if (ret==-1) return 0;
	size=st_ads.st_size;

	filemem=(char*)malloc(size);
	if (filemem==NULL) return 0;

	f_in=fopen(file_in,"r");
	result=fread(filemem, 1, size, f_in);
	fclose(f_in);
	if (result!=size) return 0;

	f_out=fopen(file_out, "w");


    ptr1=strstr(filemem,markers[0]);
	while (ptr1!=NULL)
	{
		ptr2=strstr(ptr1,markers[1]);
		if (ptr2!=NULL)
		{
			*ptr2='\0';
			file_name=ptr1+strlen(markers[0]);
			ptr3=strstr(file_name, file_prefix);
			if (ptr3!=NULL)
				file_name=ptr3+strlen(file_prefix);
			ptr1=ptr2+strlen(markers[1]);
		}
		else break;

		if (strlen(file_name)==0)
		{
			ptr1=strstr(ptr1,markers[0]);
			continue;
		}

		ptr1=strstr(ptr1,markers[2]);
		if (ptr1==NULL) break;
		ptr2=strstr(ptr1,markers[3]);
		if (ptr2!=NULL)
		{
			*ptr2='\0';
			file_date=ptr1+strlen(markers[2]);
			ptr1=ptr2+strlen(markers[3]);
		}
		else break;

		ptr1=strstr(ptr1,markers[4]);
		if (ptr1==NULL) break;
		ptr2=strstr(ptr1,markers[5]);
		if (ptr2!=NULL)
		{
			*ptr2='\0';
			file_size=ptr1+strlen(markers[4]);
			ptr1=ptr2+strlen(markers[5]);
		}
		else break;

		ptr1=strstr(ptr1,markers[6]);
		if (ptr1==NULL) break;
		ptr2=strstr(ptr1,markers[7]);
		if (ptr2!=NULL)
		{
			*ptr2='\0';
			file_type=ptr1+strlen(markers[6]);
			ptr1=ptr2+strlen(markers[7]);
		}
		else break;

		if (strlen(file_name)>0)
		{
			sprintf(row,"%s\t%s\t%s\t%s\n",file_name,file_date,file_size,file_type);
			fwrite(&row, 1, strlen(row), f_out);
		}

		ptr1=strstr(ptr1,markers[0]);
	}

	fclose(f_out);
	free(filemem);
	return 1;
}

int Al_Load_PNG_fade(char *png_name, char *png_name1, int w, int h, int x, int y, int d_x, int d_y, int fade_out)
{
    BITMAP* bmp_add = NULL, *bmp, * bmp0, * bmp1, * buffer, *buffer_f = NULL;
    PALETTE pal;
    int i;
    BITMAP *second_screen, *third_screen;
    char ad_name[MAXPATH]="", ad_name_zip[MAXPATH]="";
    double scale_x, scale_y;
    BOOL ret;
    int n_list;
    int runcode;
    char params[384]="";
    static int strcount1 = 0;
    int FILENAMEMAXC;
    long png_mem;
    struct stat file_info;
    struct timeval stop, start;
    int delta_t;

#define MaxTextLen  254  //512

    int imgno;
#ifndef LINUX
    struct stat st_ads = {0};
    struct stat st_upgds = { 0 };

    char** strarray1 = NULL;

    //strarray1 = malloc(100 * sizeof(char*));  //just to initialize

    if (stat("ads", &st_ads) == -1) {
        mkdir("ads");
    }

     if (stat("upgds", &st_upgds) == -1) {
        mkdir("upgds");
    }

     n_list = 0;

    ret = My_GetFiles("ads", &n_list, "\\*.png", "\\*.jpg", NULL, NULL, -1);
#endif
#ifdef LINUX
    struct stat st_ads = {0};
    struct stat st_upgds = {0};

    char** strarray1 = NULL;

    if (stat("ads", &st_ads) == -1) {
        mkdir("ads", 0700);
    }

    if (stat("upgds", &st_upgds) == -1) {
        mkdir("upgds", 0700);
    }
    n_list = 0;

    ret = My_GetFiles("ads", &n_list, "\\*.png", "\\*.jpg", NULL, NULL, TRUE);
#endif

    if (n_list > 2)
    {
        int rand_result;

        rand_result = (rand() % n_list);
        if (strstr(strarray[rand_result], ".<DIR>") != NULL)
            rand_result = (rand_result + 1) % n_list;
        if (strstr(strarray[rand_result], ".<DIR>") != NULL)
            rand_result = (rand_result + 1) % n_list;

        sprintf(ad_name, "ads/%s", strarray[rand_result]);

        if (strstr(ad_name, ".png")) {
            bmp_add = load_png(ad_name, pal);
        } else {
            bmp_add = load_jpg(ad_name, pal);
        }
        if (bmp_add == NULL) {
            bmp_add = load_jpg("wooden_bridge.jpg", pal);
        }
    }
    else bmp_add = load_jpg("tree.jpg", pal);

    second_screen = create_system_bitmap(w + 1, h + 1);
    third_screen = create_system_bitmap(w + 1, h + 1);
    set_clip_rect(third_screen, 0, 0, w, h);
    clear_to_color((BITMAP *) third_screen, 0x000000);

    if (bmp_add != NULL)
    {
        scale_x = (double)w / (double)bmp_add->w;
        scale_y = (double)h / (double)bmp_add->h;

        if (scale_y > scale_x) scale_x = scale_y;

        buffer_f = create_bitmap_ex(32, w + 1, h + 1);

        stretch_blit(bmp_add, third_screen, 0, 0, bmp_add->w, bmp_add->h, 0, 0, (int)((double)bmp_add->w * scale_x),
            (int)((double)bmp_add->h * scale_x));


        blit(screen, buffer_f, 0, 0, 0, 0, w, h);

        for (int i = 0; i < 26; i++)
        {
            int alpha = 255 - (i * 10); //128; //arbitrary value for demonstration purposes

            blit(third_screen, second_screen, 0, 0, 0, 0, w, h);

            drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
            set_trans_blender(0, 0, 0, alpha);

            draw_trans_sprite(second_screen, buffer_f, 0, 0);

            flip_full_screen(second_screen);
            delay(5);  //10
        }
    }

    drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

    if (fade_in) {
        png_mem = 622 * 465 * 4 + 100;
        bmp1 = load_memory_png(alfalogosak, png_mem, (RGB *)&pal);
    }
    else bmp1 = load_png(png_name1, pal);

    if (bmp1 == NULL) return 0;

    bmp0 = create_bitmap(d_x, d_y);
    bmp = create_bitmap(d_x, d_y);

    clear_to_color(bmp0, 0xffffff);
    stretch_blit(bmp1, bmp0, 0, 0, bmp1->w, bmp1->h, 0, 0, d_x, d_y);

    clear_to_color(bmp, 0xffffff);

    flip_full_screen(second_screen);

    imgno = 30;

    if (bmp != NULL)
    {
        for (i = 1; i < imgno; i++) {
            //if (i==1)
                gettimeofday(&start,NULL);
            buffer = create_bitmap_ex(32, d_x, d_y);
            if (buffer != NULL) {
                double scale = (double(d_x * i) / (double) imgno) / (double) d_x;
                clear_to_color(buffer, bitmap_mask_color(buffer));
                rotate_scaled_sprite(buffer, bmp0, 0, 0,
                                     ftofix(PI * (float) (i - 29) * 0.16 / 29.0f) * 256.0f / (2.0 * PI), ftofix(scale));

                set_alpha_blender();
                draw_trans_sprite(second_screen, buffer, x - (int) (d_x / 2 * i / imgno),
                                  y - (int) (d_y / 2 * i / imgno));
                flip_full_screen(second_screen);
                destroy_bitmap(buffer);
                set_trans_blender(0, 0, 0, TRANSLUCENCY);
                set_mode_solid2();
                set_mode_solid();
            }

            //delay(5);  //10
            blit(third_screen, second_screen, 0, 0, 0, 0, w, h);
            //if (i==1)
            //{
                gettimeofday(&stop,NULL);
                delta_t=(int)(((((long long)stop.tv_sec)*1000)+(stop.tv_usec/1000))-((((long long)start.tv_sec)*1000)+(start.tv_usec/1000)));
                if (delta_t>20) delta_t=20;
            //}
            delay(20-delta_t);  //10
        }

        delay(500);

#ifdef LINUX
        imgno=60;
#endif
#ifndef LINUX
        imgno = 30;
#endif
        flip_full_screen(second_screen);

        for (i = imgno; i >= 1; i--)
        {
            //if (i==imgno)
                gettimeofday(&start,NULL);
            buffer = create_bitmap_ex(32, d_x, d_y);
            if (buffer != NULL)
            {
                double scale = (double(d_x * i) / (double) imgno) / (double) d_x;
                clear_to_color(buffer, bitmap_mask_color(buffer));
                rotate_scaled_sprite(buffer, bmp0, 0, 0, ftofix(0), ftofix(scale));
                set_alpha_blender();
                draw_trans_sprite(second_screen, buffer, x - (int) (d_x / 2 * i / imgno),
                                  y - (int) (d_y / 2 * i / imgno));
                flip_full_screen(second_screen);
                destroy_bitmap(buffer);
                set_trans_blender(0, 0, 0, TRANSLUCENCY);
                set_mode_solid2();
                set_mode_solid();
            }
            blit(third_screen, second_screen, 0, 0, 0, 0, w, h);
            //if (i==imgno)
            //{
                gettimeofday(&stop,NULL);
                delta_t=(int)(((((long long)stop.tv_sec)*1000)+(stop.tv_usec/1000))-((((long long)start.tv_sec)*1000)+(start.tv_usec/1000)));
                if (delta_t>16) delta_t=16;
            //}
            delay(16-delta_t); //0
        }

        if ((fade_out) && (buffer_f!=NULL))
        {
            for (int i = 0; i < 26; i++) {
                int alpha = 5 + (i * 10); //128; //arbitrary value for demonstration purposes
                blit(third_screen, second_screen, 0, 0, 0, 0, w, h);
                drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
                set_trans_blender(0, 0, 0, alpha);
                draw_trans_sprite(second_screen, buffer_f, 0, 0);
                flip_full_screen(second_screen);
                delay(5); //10
            }
            drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
        }
        destroy_bitmap(bmp0);
        destroy_bitmap(bmp);
        destroy_bitmap(bmp1);
        if (buffer_f != NULL) destroy_bitmap(buffer_f);
        destroy_bitmap(third_screen);

        BOOL tools_ok=TRUE;

        if (!fade_out)  //stare 1
        {
            FILE *pp;
            int curl = 0, unzip = 0, sed = 0, tar=0;
            BOOL time_to_update=FALSE;
            char *last_upgd_date_file=(char*)"last_upgd_date.dat";
            long last_upgd_date_num;

            //allocating credentials
            cloud_connect=1;

            cloud_user=(char*)malloc(32);
            strcpy(cloud_user,"\"alfacad\"");
            cloud_user0=(char*)malloc(32);
            strcpy(cloud_user0,"alfacad");

            cloud_password=(char*)malloc(32);
            strcpy(cloud_password,"\"engineer4engineers\"");
            cloud_password0=(char*)malloc(32);
            strcpy(cloud_password0,"engineer4engineers");

            cloud_share_url=(char*)malloc(MAXPATH);
            strcpy(cloud_share_url,"\"https://nextcloud.vurplex.com\"");
            cloud_share0_url=(char*)malloc(MAXPATH);
            strcpy(cloud_share0_url,"https://nextcloud.vurplex.com");

            cloud_ads=(char*)malloc(MAXPATH);
            strcpy(cloud_ads,"\"ads\"");
            cloud_ads0=(char*)malloc(MAXPATH);
            strcpy(cloud_ads0,"ads");

            cloud_ads_url=(char*)malloc(MAXPATH);
            strcpy(cloud_ads_url,"\"https://nextcloud.vurplex.com/remote.php/webdav/ads\"");
            cloud_ads0_url=(char*)malloc(MAXPATH);
            strcpy(cloud_ads0_url,"https://nextcloud.vurplex.com/remote.php/webdav/ads");

            cloud_upgrade=(char*)malloc(MAXPATH);
            strcpy(cloud_upgrade,"\"upgds\"");
            cloud_upgrade0=(char*)malloc(MAXPATH);
            strcpy(cloud_upgrade0,"upgds");

            cloud_upgrade_url=(char*)malloc(MAXPATH);
            strcpy(cloud_upgrade_url,"\"https://nextcloud.vurplex.com/remote.php/webdav/upgds\"");
            cloud_upgrade0_url=(char*)malloc(MAXPATH);
            strcpy(cloud_upgrade0_url,"https://nextcloud.vurplex.com/remote.php/webdav/upgds");

            //getting credentials
            Get_Private_Profile_Strings_Cloud((T_Fstring)IC_CLOUD, get_cloud);

            if (cloud_connect)  //user wants to connect, to get upgrades etc
            {
                if (my_file_exists(last_upgd_date_file)) {
                    char list_row[MaxTextLen]="";
                    FILE *lupgdf = fopen(last_upgd_date_file, "rt");
                    if (lupgdf != NULL) {
                        strcount1 = 0;
                        while (fgets(list_row, MaxTextLen, lupgdf)) {
                            ret = sscanf(list_row, "%ld", &last_upgd_date_num);
                            if ((ret == 1) && (last_upgd_date_num > 20240000) && (last_upgd_date_num < 20300000))
                                break;
                            else last_upgd_date_num = 0;
                        }
                        fclose(lupgdf);
                    } else last_upgd_date_num = 0;

                } else last_upgd_date_num = 0;

                char time_str[32]="";
                long date_num_current, date_num;
                int32_t ts;
                //current time
                timestamp_t current_time = currentTimestamp();
                ts = sprintf_timestampAsYYYYMMDDHHMMSS((char *) &time_str, &date_num_current, current_time);
                printf("%s  %ld\n", time_str, date_num_current);

                if (date_num_current > last_upgd_date_num + (long) CHECK_DELAY) time_to_update = TRUE;

                if ((!fade_out) && (time_to_update))  //stage 2
                {
#ifdef LINUX
                    //checking curl
                    pp = popen("curl -V", "r");
                    if (pp != NULL) {
                        while (1) {
                            char *line;
                            char buf[1000]="";
                            line = fgets(buf, sizeof(buf), pp);
                            if (line == NULL) break;
                            if ((strstr(strlwr(line), "curl") != NULL) && (strstr(strlwr(line), "not found") == NULL)) {
                                curl = 1;
                                //break;
                            }
                        }
                        pclose(pp);
                    }
                    //checking unzip
                    pp = popen("unzip -v", "r");
                    if (pp != NULL) {
                        while (1) {
                            char *line;
                            char buf[1000]="";
                            line = fgets(buf, sizeof(buf), pp);
                            if (line == NULL) break;
                            if ((strstr(strlwr(line), "unzip") != NULL) &&
                                (strstr(strlwr(line), "not found") == NULL)) {
                                unzip = 1;
                                //break;
                            }
                        }
                        pclose(pp);
                    }
                    //checking sed
#ifndef MACOS
                    pp = popen("sed --version", "r");
                    if (pp != NULL) {
                        while (1) {
                            char *line;
                            char buf[1000]="";
                            line = fgets(buf, sizeof(buf), pp);
                            if (line == NULL) break;
                            if ((strstr(strlwr(line), "sed") != NULL) && (strstr(strlwr(line), "not found") == NULL)) {
                                sed = 1;
                                //break;
                            }
                        }
                        pclose(pp);
                    }
#else
                	/*  //we don't need that
                	pp = popen("which -a sed", "r");
                	if (pp != NULL) {
                		while (1) {
                			char *line;
                			char buf[1000]="";
                			line = fgets(buf, sizeof(buf), pp);
                			if (line == NULL) break;
                			if ((strstr(strlwr(line), "sed") != NULL) && (strstr(strlwr(line), "not found") == NULL)) {
                				sed = 1;
                				//break;
                			}
                		}
                		pclose(pp);
                	}
                	*/
                	sed = 1;
#endif
                    if ((!curl) || (!unzip) || (!sed)) {
                        char tools_to_install[72]="";
                        tools_ok = FALSE;
                        snprintf(tools_to_install, sizeof(tools_to_install), "%s %s %s %s", ((curl == 0) ? "curl" : ""),
                                ((unzip == 0) ? "unzip" : ""), ((sed == 0) ? "sed" : ""), _TOOLS_TO_INSTALL_);
                        ret = ask_question(1, (char *) _No_, (char *) _Yes_, "Upgrade", tools_to_install, 12,
                                           (char *) _INSTALL_TOOLS_, 11, 1, 203);
                    }
#endif
#ifndef LINUX
                    AllocConsole();
                    ShowWindow(GetConsoleWindow(), SW_HIDE);
                    //checking curl
                    pp = _popen("curl -V", "r");
                    if (pp != NULL)
                    {
                        while (1)
                        {
                            char *line;
                            char buf[1000]="";
                            line = fgets(buf, sizeof(buf), pp);
                            if (line == NULL) break;
                            if ((strstr(strlwr(line), "curl") != NULL) && (strstr(strlwr(line), "not recognized") == NULL))
                            {
                                curl = 1;
                                //break;
                            }
                            //if (line[0] == 'd') printf("%s\n", line); /* line includes '\n' */
                        }
                        _pclose(pp);
                    }

                    //checking tar
                    pp = _popen("tar --help", "r");
                    if (pp != NULL)
                    {
                        while (1)
                        {
                            char *line;
                            char buf[1000]="";
                            line = fgets(buf, sizeof(buf), pp);
                            if (line == NULL) break;

                            if ((strstr(strlwr(line), "tar") != NULL) && (strstr(strlwr(line), "not recognized") == NULL))
                            {
                                tar = 1;
                                //break;
                            }
                            //if (line[0] == 'd') printf("%s\n", line); /* line includes '\n' */
                        }
                        _pclose(pp);
                    }
#ifdef USESED
                    //checking sed
                    pp = _popen("sed --version", "r");
                    if (pp != NULL)
                    {
                        while (1)
                        {
                            char *line;
                            char buf[1000]="";
                            line = fgets(buf, sizeof(buf), pp);
                            if (line == NULL) break;
                            if ((strstr(strlwr(line), "sed") != NULL) && (strstr(strlwr(line), "not recognized") == NULL))
                            {
                                sed = 1;
                                //break;
                            }
                            //if (line[0] == 'd') printf("%s\n", line); /* line includes '\n' */
                        }
                        _pclose(pp);
                    }
#else
                	sed = 1;
#endif

                    FreeConsole();


                    if ((!curl) || (!tar) || (!sed))
                    {
                        BITMAP* screen_ = screen;
                        global_resized = FALSE;

                        char tools_to_install[72]="";
                        tools_ok = FALSE;
                        sprintf(tools_to_install, "%s %s %s %s", ((curl == 0) ? "curl" : ""), ((tar == 0) ? "tar" : ""),((sed == 0) ? "sed" : ""),_TOOLS_TO_INSTALL_);
                        ret = ask_question(1, _No_, _Yes_, (char*)"Upgrade", tools_to_install, 12, (char *) _INSTALL_TOOLS_, 11, 1,203);

                        if (global_resized) {
                            destroy_bitmap(second_screen);
                            second_screen = create_system_bitmap(getmaxx() + 1, getmaxy() + 1);
                            if (bmp_add != NULL)
                                stretch_blit(bmp_add, second_screen, 0, 0, bmp_add->w, bmp_add->h, 0, 0,
                                    (int)((double)bmp_add->w * scale_x),
                                    (int)((double)bmp_add->h * scale_x));
                            else clear_to_color(second_screen, 0x000000);
                            flip_full_screen(second_screen);
                        }
                        else screen = screen_;
                    }
#endif
                }  //((!fade_out) && (time_to_update))  //stage 2

                if ((!fade_out) && (tools_ok) && (time_to_update))  //stage 3
                {
                    //this should be done when image on the screen
#ifdef LINUX
                    ret = My_GetFiles((char*)"ads", &n_list, (char*)"\\*.png", (char*)"\\*.jpg", NULL, NULL, TRUE);
#endif
#ifndef LINUX
                    ret = My_GetFiles((char*)"ads", &n_list, (char*)"\\*.png", (char*)"\\*.jpg", NULL, NULL, -1);
#endif
                    if (my_file_exists((char*)"list.out"))
                        unlink("list.out");
                    flip_full_screen(second_screen);
                    extra_logo(getmaxx() / 2, getmaxy() / 2 + 6 * HEIGHT, 3, (char*)"");
#ifdef LINUX
//#ifndef MACOS
#ifdef USESCRIPT
                    pp = popen("test -x adslist.sh && echo true || echo false", "r");
                    if (pp != NULL) {
                        while (1) {
                            char *line;
                            char buf[1000]="";
                            line = fgets(buf, sizeof(buf), pp);
                            if (line == NULL) break;
                            if (strstr(strlwr(line), "false") != NULL) {
                                sprintf(params, "+x adslist.sh");
                                runcode = (int)SystemSilent("chmod", params);
                                //break;
                            }
                        }
                        pclose(pp);
                    }

                    sprintf(params, "%s %s %s %s", cloud_share0_url, cloud_ads0, cloud_user0, cloud_password0);
                    runcode = SystemSilent("./adslist.sh", params);
#else
                	//sprintf(params, "\"https://nextcloud.vurplex.com/remote.php/webdav/ads\" --connect-timeout 5 -m 5 --user \"alfacad\":\"engineer4engineers\" --request PROPFIND --data \"<?xml version='1.0' encoding='UTF-8'?> <d:propfind xmlns:d='DAV:'> <d:prop xmlns:oc='http://owncloud.org/ns'> <d:getlastmodified/> <d:getcontentlength/> <d:getcontenttype/> </d:prop> </d:propfind>\" > list.out");
                	snprintf(params, sizeof(params), "curl %s --connect-timeout 5 --max-time 5 --user %s:%s --request PROPFIND --data \"<?xml version='1.0' encoding='UTF-8'?> <d:propfind xmlns:d='DAV:'> <d:prop xmlns:oc='http://owncloud.org/ns'> <d:getlastmodified/> <d:getcontentlength/> <d:getcontenttype/> </d:prop> </d:propfind>\" > list.out", cloud_ads_url, cloud_user, cloud_password);

                	printf("%s\n", params);
                	//runcode = RunSilent("curl", params);
                	runcode = (int)SystemSilentS(params);
                	if (my_file_size((char*)"list.out") > 0)
                	{
                        char *markers[]={(char*)"<d:href>",(char*)"</d:href>",(char*)"<d:getlastmodified>",(char*)"</d:getlastmodified>",(char*)"<d:getcontentlength>",(char*)"</d:getcontentlength>",(char*)"<d:getcontenttype>",(char*)"</d:getcontenttype>"};
                		char file_prefix[MaxTextLen]="";
                		snprintf(file_prefix, sizeof(file_prefix), "/remote.php/webdav/%s/", cloud_ads0);

                		ret=extract_files_from_stream((char*)"list.out", (char*)"list.out", markers, file_prefix);
                	}
#endif
#endif
#ifndef LINUX
                    //sprintf(params, "\"https://nextcloud.vurplex.com/remote.php/webdav/ads\" --connect-timeout 5 -m 5 --user \"alfacad\":\"engineer4engineers\" --request PROPFIND --data \"<?xml version='1.0' encoding='UTF-8'?> <d:propfind xmlns:d='DAV:'> <d:prop xmlns:oc='http://owncloud.org/ns'> <d:getlastmodified/> <d:getcontentlength/> <d:getcontenttype/> </d:prop> </d:propfind>\" > list.out");
                    sprintf(params, "%s --connect-timeout 5 -m 5 --user %s:%s --request PROPFIND --data \"<?xml version='1.0' encoding='UTF-8'?> <d:propfind xmlns:d='DAV:'> <d:prop xmlns:oc='http://owncloud.org/ns'> <d:getlastmodified/> <d:getcontentlength/> <d:getcontenttype/> </d:prop> </d:propfind>\" > list.out",
                            cloud_ads_url, cloud_user, cloud_password);
                    //printf("%s\n", params);
                    runcode = RunSilent((char*)"curl", params);
#ifdef USESED
                    if (my_file_size("list.out") > 0)
                    {
                        sprintf(params, "-e \"/^<?xml version='1.0'?>/d\" -e \"s@</\\?d:response>@\\n@g\" list.out > b.out");
                        //sed - e "/^<?xml version='1.0'?>/d" - e "s@</\?d:response>@\n@g" list.out > b.out
                        runcode = RunSilent("sed", params);
                        sprintf(params, "-e \"/^<d:multistatus/d\" -e \"s@<d:[a-z]\\+/>@@g\" -e \"s@&quot;@@g\" -e \"s@</\\?\\(d\\|oc\\):[a-z]\\+>@\\t@g\" b.out > bb.out");
                        runcode = RunSilent("sed", params);
                        sprintf(params, "-e \"s@.*/remote.php/webdav/%s/@@\" bb.out > list.out", cloud_ads0);
                        runcode = RunSilent("sed", params);
                    }
#else
                	if (my_file_size((char*)"list.out") > 0)
                	{
                		char *markers[]={(char*)"<d:href>",(char*)"</d:href>",(char*)"<d:getlastmodified>",(char*)"</d:getlastmodified>",(char*)"<d:getcontentlength>",(char*)"</d:getcontentlength>",(char*)"<d:getcontenttype>",(char*)"</d:getcontenttype>"};
                		char file_prefix[MaxTextLen]="";
                		sprintf(file_prefix, "/remote.php/webdav/%s/", cloud_ads0);

                		ret=extract_files_from_stream((char*)"list.out", (char*)"list.out", markers, file_prefix);
                	}
#endif
#endif
                    if (my_file_exists((char*)"list.out") && (my_file_size((char*)"list.out") > 0)) {
                        FILE *lf;
                        char list_row[MaxTextLen]="";
                        int ret;
                        char row_file[MaxTextLen]="";
                        char row_file_name[MaxTextLen]="";
                        char row_weekday[6]="";
                        int row_day;
                        char row_month[6]="";
                        int row_year;
                        char row_time[12]="";
                        char row_zone[6]="";
                        int row_size;
                        char row_type[16]="";
                        size_t it;
                        BOOL go_download;
                        BOOL was_download = FALSE;
                        int n_ads = 0;


                        lf = fopen("list.out", "rt");
                        if (lf != NULL) {
                            strcount1 = 0;
                            while (fgets(list_row, MaxTextLen, lf)) {

                                if ((strlen(list_row) < 32) || (list_row[0] == '#') ||
                                    (list_row[0] == '\t'))
                                    continue;
#ifndef LINUX
                                char* ptr = list_row;
                                ptr = strchr(list_row, '\t');
                                while (ptr != NULL)
                                {
                                    *ptr = ' ';
                                    ptr++;
                                    ptr = strchr(ptr, '\t');
                                }
#endif
                                ret = sscanf(list_row, "%s %s %d %s %d %s %s %d %s ", &row_file, &row_weekday,
                                             &row_day,
                                             &row_month, &row_year, &row_time, &row_zone, &row_size, &row_type);

                                if ((strcmp(row_file, "filelist.txt") == 0) || (ret < 9)) continue;
                                //checking if file exists


                                row_file_name[0] = '\0';
                                const size_t len = strlen(row_file);
                                it = uri_decode(row_file, len, row_file_name);

                                ///////////////////////
                                FILENAMEMAXC = sizeof(row_file_name) + 6;
                                strarray1 = (char **) realloc(strarray1, (strcount1 + 1) * sizeof(char *));  //LINUX
                                strarray1[strcount1] = (char *) malloc(FILENAMEMAXC * sizeof(char));
                                strcpy(strarray1[strcount1], row_file_name);
                                strcount1++;

                                //////////////////////
                                n_ads++;

                                go_download = TRUE;
                                for (int i = 0; i < n_list; i++) {
                                    if (strcmp(strarray[i], row_file_name) == 0) //file exists
                                    {
                                        snprintf(ad_name, sizeof(ad_name), "ads/%s", strarray[i]);
                                        stat(ad_name, &file_info);
                                        if (file_info.st_size == row_size) {
                                            go_download = FALSE;
                                            break;
                                        }
                                    }
                                }
                                if (go_download) {
                                    was_download = TRUE;
                                    flip_full_screen(second_screen);
                                    extra_logo(getmaxx() / 2, getmaxy() / 2 + 6 * HEIGHT, 2, row_file_name);

                                    snprintf(params, sizeof(params), "--connect-timeout 5 -m 20 -u %s:%s \"%s/%s\" -o \"%s/%s\"",
                                            cloud_user, cloud_password, cloud_ads0_url, row_file, cloud_ads0,
                                            row_file_name);
#ifdef LINUX
                                    runcode = (int)SystemSilent((char*)"curl", params);
#endif
#ifndef LINUX
                                    runcode = RunSilent("curl", params);
#endif
                                }

                            }
                            fclose(lf);
                            //eliminating old files
                            if (n_ads > 0) {
                                for (i = 0; i < n_list; i++) {
                                    if ((strcmp(strarray[i], "filelist.txt") == 0) ||
                                        (strstr(strarray[i], ".<DIR>") != NULL))
                                        continue;
                                    //checking if file exists
                                    int j;
                                    for (j = 0; j < strcount1; j++) {
                                        //if (strcmp(strarray[i], strarray1[j]) == 0) //file exists  //LINUX
                                        if (strcmp(strarray[i], strarray1[j]) == 0) //file exists
                                        {
                                            break;
                                        }
                                    }
                                    if (j == strcount1) {
                                        snprintf(row_file_name, sizeof(row_file_name), "ads/%s", strarray[i]);
                                        unlink(row_file_name);
                                    }
                                }
                            }
                            for (i = 0; i < strcount1; i++)
                                free(strarray1[i]);
                            free(strarray1);

                            //saving time
                            FILE *lupgdf = fopen(last_upgd_date_file, "wt");
                            fprintf(lupgdf, "%ld\n", date_num_current);
                            fclose(lupgdf);

                        }

                        if (was_download) flip_full_screen(second_screen);
                    } //my_file_exists("list.out")

                    //Checking upgrades

                    BOOL go_download_upgds = FALSE;
                    BOOL was_download_upgds = FALSE;

                    char row_file_upgds[MaxTextLen]="";
                    char row_file_name_upgds[MaxTextLen]="";

                    int language = getlanguage();
#ifdef LINUX
#ifndef MACOS
#ifndef ALLEGRO5
                    sprintf(ad_name, "AlfaCAD%s", lang_sufix[language]);
                    sprintf(ad_name_zip, "AlfaCAD%s.zip", lang_sufix[language]);
#else
                	sprintf(ad_name, "AlfaCAD3%s", lang_sufix[language]);
                	sprintf(ad_name_zip, "AlfaCAD3%s.zip", lang_sufix[language]);
#endif
#else
#if defined(__x86_64__) || defined(_M_X64)
    				// Code for x64 architecture
    				printf("x64 Architecture\n");
                	snprintf(ad_name, sizeof(ad_name), "AlfaCAD3%sx.app/Contents/MacOS/AlfaCAD3%sx", lang_sufix[language],lang_sufix[language]);
                	snprintf(ad_name_zip, sizeof(ad_name_zip), "AlfaCAD3%sx.zip", lang_sufix[language]);
#elif defined(__aarch64__) || defined(_M_ARM64)
    				// Code for arm64 architecture
					printf("ARM64 Architecture\n");
					sprintf(ad_name, "AlfaCAD3%sa.app/Contents/MacOS/AlfaCAD3%sa", lang_sufix[language],lang_sufix[language]);
                	sprintf(ad_name_zip, "AlfaCAD3%sa.zip", lang_sufix[language]);
#else
					// Code for other architectures or when the architecture is unknown
					printf("Unknown Architecture\n");
                	sprintf(ad_name, "AlfaCAD3%sx.app/Contents/MacOS/AlfaCAD3%sx", lang_sufix[language],lang_sufix[language]);
                	sprintf(ad_name_zip, "AlfaCAD3%sx.zip", lang_sufix[language]);
#endif
#endif
#endif
#ifndef LINUX
#ifdef BIT64
                    sprintf(ad_name, "AlfaCAD%s4Win64.exe", lang_sufix[language]);
                    sprintf(ad_name_zip, "AlfaCAD%s4Win64.zip", lang_sufix[language]);
#endif
#ifndef BIT64
                    sprintf(ad_name, "AlfaCAD%s4Win.exe", lang_sufix[language]);
                    sprintf(ad_name_zip, "AlfaCAD%s4Win.zip", lang_sufix[language]);
#endif
#endif

                    stat(ad_name, &file_info);
                    //file time

                    timestamp_t file_time = file_info.st_mtime;
                    ts = sprintf_timestampAsYYYYMMDDHHMMSS((char *) &time_str, &date_num, file_time);
                    printf("%s  %ld\n", time_str, date_num);

                    //downloading zip list
                    ////////////////////////////////////////
                    if (my_file_exists((char*)"upgdslist.out"))
                        unlink("upgdslist.out");
                    flip_full_screen(second_screen);
                    extra_logo(getmaxx() / 2, getmaxy() / 2 + 6 * HEIGHT, 3, "");
#ifdef LINUX
//#ifndef MACOS
#ifdef USESCRIPT
                    //checking adslist.sh
                    pp = popen("test -x upgdslist.sh && echo true || echo false", "r");
                    if (pp != NULL) {
                        while (1) {
                            char *line;
                            char buf[1000]="";
                            line = fgets(buf, sizeof(buf), pp);
                            if (line == NULL) break;
                            if (strstr(strlwr(line), "false") != NULL) {
                                sprintf(params, "+x upgdslist.sh");
                                runcode = (int)SystemSilent("chmod", params);
                                //break;
                            }
                        }
                        pclose(pp);
                    }

                    sprintf(params, "%s %s %s %s", cloud_share0_url, cloud_upgrade0, cloud_user0, cloud_password0);
                    runcode = SystemSilent("./upgdslist.sh", params);
#else

                	snprintf(params, sizeof(params), "curl --connect-timeout 5 --max-time 5 --user %s:%s --request PROPFIND --data \"<?xml version='1.0' encoding='UTF-8'?> <d:propfind xmlns:d='DAV:'> <d:prop xmlns:oc='http://owncloud.org/ns'> <d:getlastmodified/> <d:getcontentlength/> <d:getcontenttype/> </d:prop> </d:propfind>\" %s/remote.php/webdav/%s > upgdslist.out", cloud_user, cloud_password, cloud_share0_url, cloud_upgrade0);

                	printf("%s\n", params);
                	//runcode = RunSilent("curl", params);
                	runcode = (int)SystemSilentS(params);
                	if (my_file_size((char*)"upgdslist.out") > 0)
                	{
                		char *markers[]={(char*)"<d:href>",(char*)"</d:href>",(char*)"<d:getlastmodified>",(char*)"</d:getlastmodified>",(char*)"<d:getcontentlength>",(char*)"</d:getcontentlength>",(char*)"<d:getcontenttype>",(char*)"</d:getcontenttype>"};
                		char file_prefix[MaxTextLen]="";
                		snprintf(file_prefix, sizeof(file_prefix), "/remote.php/webdav/%s/", cloud_upgrade0);

                		ret=extract_files_from_stream((char*)"upgdslist.out", (char*)"upgdslist.out", markers, file_prefix);
                	}



#endif
#endif
#ifndef LINUX
                    //sprintf(params, "\"https://nextcloud.vurplex.com/remote.php/webdav/upgds\" --connect-timeout 5 -m 5 -user \"alfacad\":\"engineer4engineers\" --request PROPFIND --data \"<?xml version='1.0' encoding='UTF-8'?> <d:propfind xmlns:d='DAV:'> <d:prop xmlns:oc='http://owncloud.org/ns'> <d:getlastmodified/> <d:getcontentlength/> <d:getcontenttype/> </d:prop> </d:propfind>\" > upgdslist.out");
                    sprintf(params, "%s --connect-timeout 5 -m 5 --user %s:%s --request PROPFIND --data \"<?xml version='1.0' encoding='UTF-8'?> <d:propfind xmlns:d='DAV:'> <d:prop xmlns:oc='http://owncloud.org/ns'> <d:getlastmodified/> <d:getcontentlength/> <d:getcontenttype/> </d:prop> </d:propfind>\" > upgdslist.out",
                        cloud_upgrade_url, cloud_user, cloud_password);
                    runcode = RunSilent((char*)"curl", params);

#ifdef USESED
                    if (my_file_size("upgdslist.out") > 0)
                    {
                        sprintf(params, "-e \"/^<?xml version='1.0'?>/d\" -e \"s@</\\?d:response>@\\n@g\" upgdslist.out > b.out");
                        //sed - e "/^<?xml version='1.0'?>/d" - e "s@</\?d:response>@\n@g" list.out > b.out
                        runcode = RunSilent("sed", params);
                        sprintf(params, "-e \"/^<d:multistatus/d\" -e \"s@<d:[a-z]\\+/>@@g\" -e \"s@&quot;@@g\" -e \"s@</\\?\\(d\\|oc\\):[a-z]\\+>@\\t@g\" b.out > bb.out");
                        runcode = RunSilent("sed", params);
                        sprintf(params, "-e \"s@.*/remote.php/webdav/%s/@@\" bb.out > upgdslist.out", cloud_upgrade0);
                        runcode = RunSilent("sed", params);
                    }
#else
                	if (my_file_size((char*)"upgdslist.out") > 0)
                	{
                		char *markers[]={(char*)"<d:href>",(char*)"</d:href>",(char*)"<d:getlastmodified>",(char*)"</d:getlastmodified>",(char*)"<d:getcontentlength>",(char*)"</d:getcontentlength>",(char*)"<d:getcontenttype>",(char*)"</d:getcontenttype>"};
                		char file_prefix[MaxTextLen]="";
                		sprintf(file_prefix, "/remote.php/webdav/%s/", cloud_upgrade0);

                		ret=extract_files_from_stream((char*)"upgdslist.out", (char*)"upgdslist.out", markers, file_prefix);
                	}
#endif
#endif
                    if (my_file_exists((char*)"upgdslist.out") && (my_file_size((char*)"upgdslist.out") > 0)) {
                        FILE *lf;
                        char list_row[MaxTextLen];
                        int ret;
                        char row_weekday[6];
                        int row_day;
                        char row_month[6];
                        int row_year;
                        char row_time[12];
                        char row_zone[6];
                        int row_size;
                        char row_type[16];
                        size_t it;

                        lf = fopen("upgdslist.out", "rt");
                        if (lf != NULL) {
                            strcount1 = 0;
                            while (fgets(list_row, MaxTextLen, lf)) {
                                if ((strlen(list_row) < 32) || (list_row[0] == '#') ||
                                    (list_row[0] == '\t'))
                                    continue;
#ifndef LINUX
                                char* ptr = list_row;
                                ptr = strchr(list_row, '\t');
                                while (ptr != NULL)
                                {
                                    *ptr = ' ';
                                    ptr++;
                                    ptr = strchr(ptr, '\t');
                                }
#endif
                                ret = sscanf(list_row, "%s %s %d %s %d %s %s %d %s ", &row_file_upgds, &row_weekday,
                                             &row_day,
                                             &row_month, &row_year, &row_time, &row_zone, &row_size, &row_type);

                                if ((strcmp(row_file_upgds, "upgdslist.txt") == 0) || (ret < 9)) continue;
                                //checking if file exists

                                row_file_name_upgds[0] = '\0';
                                const size_t len = strlen(row_file_upgds);
                                it = uri_decode(row_file_upgds, len, row_file_name_upgds);

                                go_download_upgds = FALSE;
                                if (strcmp(ad_name_zip, row_file_name_upgds) == 0) //new file potentially exists
                                {
                                    //checking time
                                    long upgd_date_num = row_year * 10000 + int_month(row_month) * 100 + row_day;
                                    //sprintf(buf, "%s, %02d %s %04d %02d:%02d:%02d", eng_day[weekday], day, eng_months[month], year, hour, minute, second);
                                    printf("%s    %s, %02d %s %04d %s\n", time_str, row_weekday, row_day, row_month,
                                           row_year, row_time);
                                    printf("%ld   %ld\n", date_num, upgd_date_num);

                                    //if (date_num < (upgd_date_num + DAYS_DELAY)) go_download_upgds = TRUE;  //if zip time (upgd_date_num) is at least 1 day after date of modifying (date_num)
                                    if ((upgd_date_num - date_num) > DAYS_DELAY)
                                        go_download_upgds = TRUE;  //if zip time (upgd_date_num) is at least 1 day after date of modifying (date_num)

                                    break;  //anyway
                                }

                            }
                            fclose(lf);

                        }
                        //saving time
                        FILE *lupgdf = fopen(last_upgd_date_file, "wt");
                        fprintf(lupgdf, "%ld\n", date_num_current);
                        fclose(lupgdf);
                    }  //my_file_exists("upgdslist.out")
                    ////////////////////////////////////////
                    if (go_download_upgds) {
                        int key;
                        BITMAP *screen_ = screen;
                        global_resized = FALSE;

                        drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

                        ret = ask_question(2, (char *) _No_, (char *) _Yes_, (char*)"Upgrade",
                                           (char *) _UPGRADE_READY_ /*exit_str*/, 12, (char *) _UPGRADE_RESTART_,
                                           11, 1,
                                           202);

                        if (global_resized) {
                            destroy_bitmap(second_screen);
                            second_screen = create_system_bitmap(getmaxx() + 1, getmaxy() + 1);
                            if (bmp_add != NULL)
                                stretch_blit(bmp_add, second_screen, 0, 0, bmp_add->w, bmp_add->h, 0, 0,
                                             (int) ((double) bmp_add->w * scale_x),
                                             (int) ((double) bmp_add->h * scale_x));
                            else clear_to_color(second_screen, 0x000000);
                            flip_full_screen(second_screen);
                        } else screen = screen_;

                        if (ret == 1) key = _YES_;
                        else key = _NO_;

                        if (key == _YES_ || key == _yes_) {
                            //downloading
                            was_download_upgds = TRUE;
                            flip_full_screen(second_screen);
                            extra_logo(getmaxx() / 2, getmaxy() / 2 + 6 * HEIGHT, 2, row_file_name_upgds);
                            snprintf(params, sizeof(params), "--connect-timeout 5 -m 120 -u %s:%s \"%s/%s\" -o \"%s/%s\"",
                                    cloud_user, cloud_password, cloud_upgrade0_url, row_file_upgds, cloud_upgrade0,
                                    row_file_name_upgds);
#ifdef LINUX
                            runcode = (int)SystemSilent((char*)"curl", params);
#endif
#ifndef LINUX
                            runcode = RunSilent("curl", params);
#endif
                            //upgrading
#ifdef LINUX
#ifndef MACOS
                            sprintf(params, "-o upgds/%s", row_file_name_upgds);
                            runcode = (int)SystemSilent((char*)"unzip", params);

                        	sprintf(params, "+x %s", ad_name);
                        	runcode = (int)SystemSilent((char*)"chmod", params);
#else
                        	char *cwd;
							cwd = getcwd(NULL, 0);
						    if (cwd != NULL)
						    {
								printf("Current working directory: %s\n", cwd);

						    	runcode=mkdir("/tmp/ALFACAD3XXx", S_IWUSR);
						    	snprintf(params,sizeof(params), "chmod 700 /tmp/ALFACAD3XXx");
						    	runcode = (int)SystemSilentS(params);

						    	if (runcode==0)
						    		{
						    			snprintf(params, sizeof(params), "-o upgds/%s -d %s", row_file_name_upgds, "/tmp/ALFACAD3XXx");
						    			runcode = (int)SystemSilent((char*)"unzip", params);

						    			// sudo chown -R $USER ~/tmp/ALFACAD3XXx
						    			snprintf(params, sizeof(params), "chown -R $USER /tmp/ALFACAD3XXx");  //sudo
						    			runcode = (int)SystemSilentS(params);
						    			if (runcode==0)
						    			{
						    				snprintf(params, sizeof(params), "chgrp -R staff /tmp/ALFACAD3XXx");  //sudo
						    				runcode = (int)SystemSilentS(params);
						    				if (runcode==0)
						    					{
						    					// sudo chmod -R go-w ~/tmp/ALFACAD3XXx
						    					snprintf(params,sizeof(params), "chmod -R 755 /tmp/ALFACAD3XXx");  //sudo
						    					runcode = (int)SystemSilentS(params);

						    					if (runcode==0)
						    					{
						    						// rsync -av /tmp/ALFACAD3XXx/ ~/ALFACAD3
						    						snprintf(params,sizeof(params), "rsync -av /tmp/ALFACAD3XXx/ %s",cwd);
						    						runcode = (int)SystemSilentS(params);
						    					}
						    				}
						    			}

						    			snprintf(params, sizeof(params), "-r /tmp/ALFACAD3XXx");
						    			runcode = (int)SystemSilent((char*)"rm", params);
						    		}

						      free(cwd);
						    }

#endif
#endif
#ifndef LINUX
                            //sprintf(params, "-xf upgds/%s > abc.log", row_file_name_upgds);
                            //RunSilent("tar", params);
                            //sprintf(params, "tar -xf upgds/% s > abc.log", row_file_name_upgds);
                            FILE* gotar;
                            gotar=fopen("gotar.bat", "wt");
                            fprintf(gotar, "@echo off\n");
                            fprintf(gotar, "echo %%1\n");
                            fprintf(gotar, "echo %%2\n");
                            fprintf(gotar, "echo %%3\n");
                            fprintf(gotar, "echo %%4\n");
                            fprintf(gotar, "echo %%5\n");
                            fprintf(gotar, "timeout /t 2 /nobreak\n");
                            fprintf(gotar, "tar -xf upgds/%%1\n");
                            fprintf(gotar, "start %%2 %%4 %%5\n");
                            fclose(gotar);

                            sprintf(params, "gotar.bat %s", row_file_name_upgds);
                            _execlp("gotar.bat", "gotar.bat", row_file_name_upgds, ad_name, *(argv_), *(argv_+1), *(argv_+2), nullptr);
                            quick_exit(0);
#endif

#ifdef LINUX
                            sprintf(params, "+x %s", ad_name);
                            runcode = (int)SystemSilent((char*)"chmod", params);
#endif
#ifndef LINUX
                            /////////////////////////////  TO DO ON WINDOWS
                           ;
                           //// RunSilent("chmod", params);
#endif
                            if (bmp_add != NULL) destroy_bitmap(bmp_add);
                            destroy_bitmap(second_screen);
                            return 255;  //restart
                        }
                    } //(go_download_upgds)

                    flip_full_screen(second_screen);

                    //bitmap not needed anymore
                    destroy_bitmap((BITMAP *) icon_upgrademark_pmem);
                    destroy_bitmap((BITMAP *) icon_noupgrademark_pmem);

                    free(cloud_user);
                    free(cloud_user0);
                    free(cloud_password);
                    free(cloud_password0);
                    free(cloud_share_url);
                    free(cloud_share0_url);
                    free(cloud_ads);
                    free(cloud_ads0);
                    free(cloud_ads_url);
                    free(cloud_ads0_url);
                    free(cloud_upgrade);
                    free(cloud_upgrade0);
                    free(cloud_upgrade_url);
                    free(cloud_upgrade0_url);

                  
                } //(!fade_out) && (tools_ok) ........  //stage 3
            }  //cloud connection

        }  //fade_out
    } //bmp
    if (bmp_add != NULL) destroy_bitmap(bmp_add);
    destroy_bitmap(second_screen);
    return 0;
} //end of function


void Al_Load_PCX_fade_out(char *pcx_name, int x, int y, int d_x, int d_y)
{
  BITMAP *bmp;
  //ALETTE pal;
  RGB pal;

  bmp = load_bitmap(pcx_name, &pal);
 
  fade_out(16);
  GrClearContext(0);

  if (bmp != NULL)
  {
    blit(bmp, screen, 0, 0, x, y, d_x, d_y);
  
    fade_in(&pal, 8);
    delay(1000);

    destroy_bitmap(bmp);
    fade_out(16);
    GrClearContext(0);
  }
}

void Al_Load_PNG_fade_out(char *png_name, int x, int y, int d_x, int d_y)
{
  BITMAP *bmp;
  //PALETTE pal;
  RGB pal;

  bmp = load_png(png_name, &pal);
  
  fade_out(16);
  GrClearContext(0);

  if (bmp != NULL)
  {
    blit(bmp, screen, 0, 0, x, y, d_x, d_y);
  
    fade_in(&pal, 8);
    delay(1000);

    destroy_bitmap(bmp);
    fade_out(16);
    GrClearContext(0);
  }
}

void Load_Data(void)
{
 data = load_datafile("alfacad.dat");
 //data = load_datafile("alfprof.dat");
 if (!data) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	  allegro_message(_ERROR_READING_DAT_);
      exit(1);
   }
}

void FreeMouse(void)
{
	
	serv134_135(FALSE);
	free_mouse = 1;
	_free_mouse();
	remove_mouse();
	Odczyt_licznikow();
	last_mouse_z = mouse_z;
}

void LockMouse(void)
{
	
	serv134_135(TRUE);
	free_mouse = 0;
	lock_mouse();
	install_mouse();
    show_x_cursor();

	if (dialog_active() == 1)
	{
		_free_mouse();
        show_x_cursor();
		show_mouse(screen);
	}
}

int Wake_Up(void)
{
	if (free_mouse == 0)
	{
		serv134_135(FALSE);
		free_mouse = 1;
		_free_mouse();
		remove_mouse();
		set_sleep_state(TRUE);  //SLEEP_STATE

#ifdef BEEP
		MessageBeep(0x00000000L);
#endif
	}
	else
	{
		serv134_135(TRUE);
		free_mouse = 0;
		lock_mouse();
		install_mouse();
		install_keyboard();
		set_sleep_state(FALSE);

        show_x_cursor();

	}

	int p_keyboard = my_poll_keyboard();

	return 0;
}

int Free_Mouse(void)
{
#ifndef LINUX
	HWND wnd;
	RECT wndRect, clnRect;
#endif

	if ((now_is_dialog) && (Get_Global_Menu_Flags() == 0)) return 0;

	if (free_mouse == 0)
	{
		serv134_135(FALSE);
		free_mouse = 1;
		_free_mouse();

		set_sleep_state(TRUE);  //SLEEP_STATE

        clean_key_buffer();

        dialog_cursor(1);

//#ifdef BEEP
		//MessageBeep(0xFFFFFFFF)x
//#endif
		//saving window dimensions
#ifndef LINUX
		wnd = win_get_window();
		GetWindowRect(wnd, &wndRect);
		GetClientRect(wnd, &clnRect);

		curr_w = wndRect.right - wndRect.left;
		curr_w_ = clnRect.right - clnRect.left;
		curr_h = wndRect.bottom - wndRect.top;
		curr_h_ = clnRect.bottom - clnRect.top;
#endif
#ifdef LINUX
#ifndef MACOS
        Display *display;
        Window focus, toplevel_parent_of_focus, root_window;
        XWindowAttributes attr;
        int revert;
        int ret;

        display = XOpenDisplay(0);

        root_window=DefaultRootWindow(display);

#ifndef ALLEGRO5
        focus=_xwin.window;
#else
        focus=get_x_window_id();
#endif
        toplevel_parent_of_focus = get_toplevel_parent(display, focus);
        ////toplevel_parent_of_focus = get_my_window(display, root_window, Window_Name);
        ret = XGetWindowAttributes(display,  toplevel_parent_of_focus, &attr);

        curr_w = attr.width;
        curr_w_ = attr.width;
        curr_h = attr.height;
        curr_h_ = attr.height;

        XCloseDisplay(display);
#else

#endif
#endif
	}
	else
	{
		serv134_135(TRUE);
		free_mouse = 0;
		lock_mouse();
		set_sleep_state(FALSE);

        dialog_cursor(0);

//#ifdef BEEP
//		MessageBeep(MB_OK);
//#endif
//
		reset_if_resized();
	}

	int p_keyboard = my_poll_keyboard();

  return 0;
}

void Free_Mouse_Window (void)
{
	free_mouse=1;

}

void Lock_Mouse_Window (void)
{
	free_mouse=0;
}


void table_callback(int pos)
{
   rect(screen, (SCREEN_W-256)/2-1, 128, (SCREEN_W+256)/2+1, 140, palette_color[255]);
   rectfill(screen, (SCREEN_W-256)/2, 129, (SCREEN_W-256)/2+pos, 139, palette_color[1]);
   delay(500);
}

void DoneBuffers1(void)
{
  if (my_rgb_map==TRUE) free (rgb_map);
  komunikat_str((char*)"@ 1-1");
  if (trans_map!=nullptr) free (trans_map);  //NULL
  komunikat_str((char*)"@ 1-2");
  if (trans_map_preview != nullptr) free(trans_map_preview); //NULL
  komunikat_str((char*)"@ 1-3");
  if (light_map!=nullptr) free (light_map); //NULL
}

void reset_trans_blender(void)
{
	set_trans_blender(0, 0, 0, TRANSLUCENCY);
}
 
int check_tables(void)
{
   if (bitmap_color_depth(screen) > 8) {
      set_trans_blender(0, 0, 0, TRANSLUCENCY);
   }
   else if ((!my_rgb_map) || (!trans_map) || (!light_map))
   {
      scare_mouse();


      if (!my_rgb_map) {
	 rgb_map = (RGB_MAP *)malloc(sizeof(RGB_MAP));
	 create_rgb_table(rgb_map, mypal, table_callback);
	 my_rgb_map = TRUE;
      }

      if (!trans_map) {

	 trans_map = (COLOR_MAP *)malloc(sizeof(COLOR_MAP));
	 create_trans_table(trans_map, mypal, 208 /*192*/, 208/*192*/, 208 /*192*/, table_callback);

      }

	  if (!trans_map_preview) {

		  trans_map_preview = (COLOR_MAP *)malloc(sizeof(COLOR_MAP));
		  create_trans_table(trans_map_preview, mypal, 128 /*192*/, 128/*192*/, 128 /*192*/, table_callback);

	  }


      if (!light_map) {

	 light_map = (COLOR_MAP *)malloc(sizeof(COLOR_MAP));
	 create_light_table(light_map, mypal, 0, 0, 0, table_callback);

      }


      unscare_mouse();
	  return 1;
   }

   return 0;
}

void save_window_dim(int x0_, int y0_, int dx_, int dy_) {
    dx_back=dx_;
    dy_back=dy_;
    x0_back=x0_;
    y0_back=y0_;
}

void restore_window_dim(void) {
    dx_new=dx_back;
    dy_new=dy_back;
    x0_new=x0_back;
    y0_new=y0_back;
}

#ifndef LINUX
int get_monitor_dims(int* ret_left_x, int* ret_right_x, int* ret_top_y, int* ret_bottom_y, int monitor)
{
	RECT DskRect;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &DskRect, NULL); //WINDOWS

	int sm_monitors = GetSystemMetrics(SM_CMONITORS);
	if (sm_monitors > 1)
	{
		DskRect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
		DskRect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
		DskRect.right = DskRect.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
		DskRect.bottom = DskRect.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
	}
	
	*ret_left_x = DskRect.left;
	*ret_right_x = DskRect.right;
	*ret_top_y = DskRect.top;
	*ret_bottom_y = DskRect.bottom;

	return 1;
}
#endif

void expand_horizontally(void)
{
#ifndef LINUX

	//int res_ret, ret1;
	//int cur_width;
	//int cur_height;
	HWND wnd, Dwnd;
	RECT wndRect, clnRect, DwndRect, DclnRect;
	RECT DskRect;
	int w, w_, h, h_, Dw, Dw_, Dh, Dh_, dw, dh,dh1, e_h;
	GFX_MARGINS* gfx_margins;
	int ret_ref;

	wnd = win_get_window();

	GetWindowRect(wnd, &wndRect); //WINDOWS
	GetClientRect(wnd, &clnRect); //WINDOWS

	int S_W, S_H;
	int ret = get_desktop_resolution(&S_W, &S_H);  //ALLEGRO

	Dwnd = GetDesktopWindow(); //WINDOWS
	GetWindowRect(Dwnd, &DwndRect); //WINDOWS
	GetClientRect(Dwnd, &DclnRect); //WINDOWS

	SystemParametersInfo(SPI_GETWORKAREA, NULL, &DskRect, NULL); //WINDOWS

	ret = get_gfx_margins(&gfx_margins);

	/*
	int sm_monitors = GetSystemMetrics(SM_CMONITORS);
	if (sm_monitors > 1)
	{
		DskRect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
		DskRect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
		DskRect.right = DskRect.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
		DskRect.bottom = DskRect.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
	}
	*/

	/*
	w = wndRect.right - wndRect.left;
	w_ = clnRect.right - clnRect.left;
	Dw = DwndRect.right - DwndRect.left;

	Dw_ = DskRect.right - DskRect.left;

	h = wndRect.bottom - wndRect.top;
	h_ = clnRect.bottom - clnRect.top;
	Dh = DwndRect.bottom - DwndRect.top;

	Dh_ = DskRect.bottom - DskRect.top;

	dx_back = w_;
	dy_back = h_;
	x0_back = wndRect.left;
	y0_back = wndRect.top;

	dw = (w - w_);
	dh = (h - h_);
	dh1 = (Dh - Dh_);

	if (dh1 == 0) e_h = 1;
	else e_h = -1;

	x0_new = DskRect.left;
	//y0_new = DskRect.top;
	y0_new = y0_back;

	dx_new = DskRect.right - DskRect.left;
	//dy_new = DskRect.bottom - DskRect.top - dh + dw / 2 - e_h;
	dy_new = dy_back;

	set_resized_window(dx_new, dy_new, 1);

	wnd = win_get_window(); //WINDOWS

	GetWindowRect(wnd, &wndRect); //WINDOWS
	GetClientRect(wnd, &clnRect); //WINDOWS

	w = wndRect.right - wndRect.left;
	w_ = clnRect.right - clnRect.left;

	h = wndRect.bottom - wndRect.top;
	h_ = clnRect.bottom - clnRect.top;

	dw = (w - w_);
	dh = (h - h_);

	x0_new -= dw / 2;
	
	dx_new = wndRect.right - wndRect.left;
	dy_new = wndRect.bottom - wndRect.top;
	*/

	w_ = clnRect.right - clnRect.left;
	h_ = clnRect.bottom - clnRect.top;

	dx_back = w_;
	dy_back = h_;
	x0_back = wndRect.left;
	y0_back = wndRect.top;

	Dh = DwndRect.bottom - DwndRect.top;
	Dh_ = DskRect.bottom - DskRect.top;
	dh1 = (Dh - Dh_);

	if (dh1 == 0) e_h = 1;
	else e_h = -1;

	x0_new = DskRect.left - gfx_margins->l_b;
	y0_new = wndRect.top;

	dx_new = DskRect.right - DskRect.left;
	dy_new = h_;

	set_resized_window(dx_new, dy_new, 1);
	wnd = win_get_window(); //WINDOWS

	//GetClientRect(wnd, &clnRect); //WINDOWS
	GetWindowRect(wnd, &wndRect); //WINDOWS
	dx_new = wndRect.right - wndRect.left;
	dy_new = wndRect.bottom - wndRect.top;

	MoveWindow(wnd, x0_new, y0_new, dx_new, dy_new, TRUE); //WINDOWS

	//int p_mouse = my_poll_mouse();
	//int p_keyboard = my_poll_keyboard();
	//moveto(x0_new + (dx_new / 2), y0_new + (dy_new / 2));
	//position_mouse(x0_new + (dx_new / 2), y0_new + (dy_new / 2));
#endif
#ifdef LINUX
    int ret_left_x, ret_right_x, ret_top_y, ret_bottom_y;
    int monitor=0;
    int ret;
    int curr_x0, curr_y0, curr_h, curr_v;
	int ret_ref;

    ret_ref = get_window_origin_and_size(&curr_x0, &curr_y0, &curr_h, &curr_v);

    save_window_dim(curr_x0, curr_y0, curr_h, curr_v);

    ret = get_monitor_dims(&ret_left_x, &ret_right_x, &ret_top_y, &ret_bottom_y, -1);

    x0_new = ret_left_x;
    y0_new = curr_y0-ret_top_y; //ret_top_y;

    dx_new = ret_right_x - ret_left_x;

    dy_new = min(curr_v - ret_top_y, ret_bottom_y - ret_top_y);

    if (dx_new>5980) dx_new-=8;  //TEMPORARY

    set_origins(x0_new, y0_new);
    set_resized_window(dx_new, dy_new, 1);
    ret = set_window_origin(x0_new, y0_new);

#ifdef MACOS
	enable_hardware_cursor();
	show_os_cursor(MOUSE_CURSOR_ARROW);
	disable_hardware_cursor();
#endif

    //int p_mouse = my_poll_mouse();
    //int p_keyboard = my_poll_keyboard();
    //moveto(x0_new + (dx_new / 2), y0_new + (dy_new / 2));

    return;
#endif
}

void expand_vertically(void)
{
#ifndef LINUX
	//int res_ret, ret1;
	//int cur_width;
	//int cur_height;
	HWND wnd, Dwnd;
	RECT wndRect, clnRect, DwndRect, DclnRect;
	RECT DskRect;
	int w, w_, h, h_, Dw, Dw_, Dh, Dh_, dw, dh, dh1, e_h;
	GFX_MARGINS* gfx_margins;

	wnd = win_get_window();

	GetWindowRect(wnd, &wndRect); //WINDOWS
	GetClientRect(wnd, &clnRect); //WINDOWS

	int S_W, S_H;
	int ret = get_desktop_resolution(&S_W, &S_H);  //ALLEGRO

	Dwnd = GetDesktopWindow(); //WINDOWS
	GetWindowRect(Dwnd, &DwndRect); //WINDOWS
	GetClientRect(Dwnd, &DclnRect); //WINDOWS

	SystemParametersInfo(SPI_GETWORKAREA, NULL, &DskRect, NULL); //WINDOWS

	/*

	int sm_monitors = GetSystemMetrics(SM_CMONITORS);
	if (sm_monitors > 1)
	{
		DskRect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
		DskRect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
		DskRect.right = DskRect.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
		DskRect.bottom = DskRect.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
	}
	*/

	ret = get_gfx_margins(&gfx_margins);

	/*

	w = wndRect.right - wndRect.left;
	w_ = clnRect.right - clnRect.left;
	Dw = DwndRect.right - DwndRect.left;

	Dw_ = DskRect.right - DskRect.left;

	h = wndRect.bottom - wndRect.top;
	h_ = clnRect.bottom - clnRect.top;
	Dh = DwndRect.bottom - DwndRect.top;

	Dh_ = DskRect.bottom - DskRect.top;

	dx_back = w_;
	dy_back = h_;
	x0_back = wndRect.left;
	y0_back = wndRect.top;

	dw = (w - w_);
	dh = (h - h_);
	dh1 = (Dh - Dh_);

	if (dh1 == 0) e_h = 1;
	else e_h = -1;

	//x0_new = DskRect.left;
	x0_new = x0_back;
	y0_new = DskRect.top;
	//y0_new = y0_back;

	//dx_new = DskRect.right - DskRect.left;
	dx_new = dx_back;
	dy_new = DskRect.bottom - DskRect.top - dh + dw / 2 - e_h;
	//dy_new = dy_back;


	set_resized_window(dx_new, dy_new, 1);

	wnd = win_get_window(); //WINDOWS

	GetWindowRect(wnd, &wndRect); //WINDOWS
	GetClientRect(wnd, &clnRect); //WINDOWS

	w = wndRect.right - wndRect.left;
	w_ = clnRect.right - clnRect.left;

	h = wndRect.bottom - wndRect.top;
	h_ = clnRect.bottom - clnRect.top;

	dw = (w - w_);
	dh = (h - h_);

	x0_new -= dw / 2;

	*/

	w_ = clnRect.right - clnRect.left;
	h_ = clnRect.bottom - clnRect.top;

	dx_back = w_;
	dy_back = h_;
	x0_back = wndRect.left;
	y0_back = wndRect.top;

	Dh = DwndRect.bottom - DwndRect.top;
	Dh_ = DskRect.bottom - DskRect.top;
	dh1 = (Dh - Dh_);

	if (dh1 == 0) e_h = 1;
	else e_h = -1;

	x0_new = wndRect.left - gfx_margins->l_b;
	y0_new = DskRect.top;

	dx_new = w_;
	dy_new = DskRect.bottom - DskRect.top - gfx_margins->t_btb - e_h;

	set_resized_window(dx_new, dy_new, 1);
	wnd = win_get_window(); //WINDOWS

	//GetClientRect(wnd, &clnRect); //WINDOWS
	GetWindowRect(wnd, &wndRect); //WINDOWS
	dx_new = wndRect.right - wndRect.left;
	dy_new = wndRect.bottom - wndRect.top;

	MoveWindow(wnd, x0_new, y0_new, dx_new, dy_new, TRUE); //WINDOWS

	//int p_mouse = my_poll_mouse();
	//int p_keyboard = my_poll_keyboard();
	//moveto(x0_new + (dx_new / 2), y0_new + (dy_new / 2));
	//position_mouse(x0_new + (dx_new / 2), y0_new + (dy_new / 2));
#else
    int ret_left_x, ret_right_x, ret_top_y, ret_bottom_y;
    int monitor=0;
    int ret;
    int curr_x0, curr_y0, curr_h, curr_v;
	int ret_ref;

    ret_ref = get_window_origin_and_size(&curr_x0, &curr_y0, &curr_h, &curr_v);

    save_window_dim(curr_x0, curr_y0, curr_h, curr_v);

    ret = get_monitor_dims(&ret_left_x, &ret_right_x, &ret_top_y, &ret_bottom_y, -1);

    x0_new = curr_x0-ret_left_x; //ret_left_x;
    y0_new = ret_top_y;

    dx_new = curr_h-ret_left_x;

    dy_new = ret_bottom_y - ret_top_y;

    set_origins(x0_new, y0_new);
    set_resized_window(dx_new, dy_new, 1);
    ret = set_window_origin(x0_new, y0_new);

#ifdef MACOS
	enable_hardware_cursor();
	show_os_cursor(MOUSE_CURSOR_ARROW);
	disable_hardware_cursor();
#endif

    //int p_mouse = my_poll_mouse();
    //int p_keyboard = my_poll_keyboard();
    //moveto(x0_new + (dx_new / 2), y0_new + (dy_new / 2));
	//position_mouse(x0_new + (dx_new / 2), y0_new + (dy_new / 2));
    return;
#endif
}

void get_full_screen(int *x0_n, int *y0_n, int *dx_n, int *dy_n)
{
#ifndef LINUX
	HWND wnd, Dwnd;
	RECT wndRect, clnRect, DwndRect, DclnRect;
	RECT DskRect;
	int w, w_, h, h_, Dw, Dw_, Dh, Dh_, dw, dh, dh1, e_h;
	GFX_MARGINS *gfx_margins;

	wnd = win_get_window(); //WINDOWS

	GetWindowRect(wnd, &wndRect); //WINDOWS
	GetClientRect(wnd, &clnRect); //WINDOWS

	int S_W, S_H;
	int ret = get_desktop_resolution(&S_W, &S_H);  //ALLEGRO

	Dwnd = GetDesktopWindow(); //WINDOWS
	GetWindowRect(Dwnd, &DwndRect); //WINDOWS
	GetClientRect(Dwnd, &DclnRect); //WINDOWS

	SystemParametersInfo(SPI_GETWORKAREA, NULL, &DskRect, NULL); //WINDOWS


	/*
	int sm_monitors = GetSystemMetrics(SM_CMONITORS);
	if (sm_monitors > 1)
	{
		DskRect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
		DskRect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
		DskRect.right = DskRect.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
		DskRect.bottom = DskRect.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
	}
	*/

	ret = get_gfx_margins(&gfx_margins);


	/*

	w = wndRect.right - wndRect.left;
	w_ = clnRect.right - clnRect.left;
	Dw = DwndRect.right - DwndRect.left;

	Dw_ = DskRect.right - DskRect.left;

	h = wndRect.bottom - wndRect.top;
	h_ = clnRect.bottom - clnRect.top;
	Dh = DwndRect.bottom - DwndRect.top;

	Dh_ = DskRect.bottom - DskRect.top;

	dx_back = w_;
	dy_back = h_;
	x0_back = wndRect.left;
	y0_back = wndRect.top;


	dw = (w - w_);
	dh = (h - h_);
	dh1 = (Dh - Dh_);

	if (dh1 == 0) e_h = 1;
	else e_h = -1;

	*x0_n = DskRect.left;
	*y0_n = DskRect.top;

	*dx_n = DskRect.right - DskRect.left;
	*dy_n = DskRect.bottom - DskRect.top - dh + dw / 2 - e_h;

	wnd = win_get_window(); //WINDOWS

	GetWindowRect(wnd, &wndRect); //WINDOWS
	GetClientRect(wnd, &clnRect); //WINDOWS

	w = wndRect.right - wndRect.left;
	w_ = clnRect.right - clnRect.left;

	h = wndRect.bottom - wndRect.top;
	h_ = clnRect.bottom - clnRect.top;

	dw = (w - w_);
	dh = (h - h_);

	*x0_n -= dw / 2;
	* */


	w_ = clnRect.right - clnRect.left;
	h_ = clnRect.bottom - clnRect.top;

	dx_back = w_;
	dy_back = h_;
	x0_back = wndRect.left;
	y0_back = wndRect.top;

	Dh = DwndRect.bottom - DwndRect.top;
	Dh_ = DskRect.bottom - DskRect.top;
	dh1 = (Dh - Dh_);
	if (dh1 == 0) e_h = 1;
	else e_h = -1;

	*x0_n = DskRect.left-gfx_margins->l_b;
	*y0_n = DskRect.top;

	*dx_n = DskRect.right - DskRect.left;
	*dy_n = DskRect.bottom - DskRect.top - gfx_margins->t_btb - e_h;

#else
    int ret_left_x, ret_right_x, ret_top_y, ret_bottom_y;
    int monitor=0;
    int ret;

    ret = get_monitor_dims(&ret_left_x, &ret_right_x, &ret_top_y, &ret_bottom_y, monitor);

    *x0_n = ret_left_x;
    *y0_n = ret_top_y;

    *dx_n = ret_right_x - ret_left_x;
    *dy_n = ret_bottom_y - ret_top_y; // - dh + dw / 2 - e_h;

#endif
}

void expand_diagonally(void)
{
#ifndef LINUX
	//int res_ret, ret1;
	//int cur_width;
	//int cur_height;
	HWND wnd, Dwnd;
	RECT wndRect, clnRect, DwndRect, DclnRect;
	RECT DskRect;
	int w, w_, h, h_, Dw, Dw_, Dh, Dh_, dw, dh, dh1, e_h;
	GFX_MARGINS* gfx_margins;

	wnd = win_get_window();  //WINDOWS
	
	GetWindowRect(wnd, &wndRect);  //WINDOWS
	GetClientRect(wnd, &clnRect);  //WINDOWS

	int S_W, S_H;
	int ret = get_desktop_resolution(&S_W, &S_H);  //ALLEGRO

	Dwnd = GetDesktopWindow();  //WINDOWS
	GetWindowRect(Dwnd, &DwndRect);  //WINDOWS
	GetClientRect(Dwnd, &DclnRect);  //WINDOWS

	SystemParametersInfo(SPI_GETWORKAREA, NULL, &DskRect, NULL);  //WINDOWS

	/*
	int sm_monitors = GetSystemMetrics(SM_CMONITORS);
	if (sm_monitors > 1)
	{
		DskRect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
		DskRect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
		DskRect.right = DskRect.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
		DskRect.bottom = DskRect.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
	}
	*/

	ret = get_gfx_margins(&gfx_margins);

	/*

	w = wndRect.right - wndRect.left;
	w_ = clnRect.right - clnRect.left;
	Dw = DwndRect.right - DwndRect.left;

	Dw_ = DskRect.right - DskRect.left;

	h = wndRect.bottom - wndRect.top;
	h_ = clnRect.bottom - clnRect.top;
	Dh = DwndRect.bottom - DwndRect.top;

	Dh_ = DskRect.bottom - DskRect.top;
	
	dx_back = w_;
	dy_back = h_;
	x0_back = wndRect.left;
	y0_back = wndRect.top;

	
	dw = (w - w_);
	dh = (h - h_);
	dh1 = (Dh - Dh_);

	if (dh1 == 0) e_h = 1;
	else e_h = -1;

	x0_new = DskRect.left;
	y0_new = DskRect.top;

	dx_new = DskRect.right - DskRect.left;
	dy_new = DskRect.bottom - DskRect.top - dh + dw/2 - e_h;

	set_resized_window(dx_new, dy_new, 1);

	wnd = win_get_window(); //WINDOWS

	GetWindowRect(wnd, &wndRect); //WINDOWS
	GetClientRect(wnd, &clnRect); //WINDOWS

	w = wndRect.right - wndRect.left;
	w_ = clnRect.right - clnRect.left;

	h = wndRect.bottom - wndRect.top;
	h_ = clnRect.bottom - clnRect.top;

	dw = (w - w_);
	dh= (h - h_);

	x0_new -= dw / 2;
	
	dx_new = wndRect.right - wndRect.left;
	dy_new = wndRect.bottom - wndRect.top;
	*/

	w_ = clnRect.right - clnRect.left;
	h_ = clnRect.bottom - clnRect.top;

	dx_back = w_;
	dy_back = h_;
	x0_back = wndRect.left;
	y0_back = wndRect.top;

	Dh = DwndRect.bottom - DwndRect.top;
	Dh_ = DskRect.bottom - DskRect.top;
	dh1 = (Dh - Dh_);
	if (dh1 == 0) e_h = 1;
	else e_h = -1;

	x0_new = DskRect.left - gfx_margins->l_b;
	y0_new = DskRect.top;

	dx_new = DskRect.right - DskRect.left;
	dy_new = DskRect.bottom - DskRect.top - gfx_margins->t_btb - e_h;

	set_resized_window(dx_new, dy_new, 1);
	wnd = win_get_window(); //WINDOWS

	//GetClientRect(wnd, &clnRect); //WINDOWS
	GetWindowRect(wnd, &wndRect); //WINDOWS
	dx_new = wndRect.right - wndRect.left;
	dy_new = wndRect.bottom - wndRect.top;

	MoveWindow(wnd, x0_new, y0_new, dx_new, dy_new, TRUE); //WINDOWS

	//int p_mouse = my_poll_mouse();
	//int p_keyboard = my_poll_keyboard();
	
	//moveto(x0_new + (dx_new / 2), y0_new + (dy_new / 2));
	//position_mouse(x0_new + (dx_new / 2), y0_new + (dy_new / 2));
#else
    int ret_left_x, ret_right_x, ret_top_y, ret_bottom_y;
    int monitor=0;
    int ret, ret2;
    int curr_x0, curr_y0, curr_h, curr_v;
	int ret_ref;

    ret_ref = get_window_origin_and_size(&curr_x0, &curr_y0, &curr_h, &curr_v);

    save_window_dim(curr_x0, curr_y0, curr_h, curr_v);

    ret = get_monitor_dims(&ret_left_x, &ret_right_x, &ret_top_y, &ret_bottom_y, -1);

    x0_new = ret_left_x;
    y0_new = ret_top_y;

    dx_new = ret_right_x - ret_left_x;

    dy_new = ret_bottom_y - ret_top_y;

    if (dx_new>5980) dx_new-=8;  //TEMPORARY

    set_origins(x0_new, y0_new);
    set_resized_window(dx_new, dy_new, 1);

    ret = set_window_origin(0, 0);

#ifdef MACOS
	enable_hardware_cursor();
	show_os_cursor(MOUSE_CURSOR_ARROW);
	disable_hardware_cursor();
#endif

    //int p_mouse = my_poll_mouse();
    //int p_keyboard = my_poll_keyboard();
    //moveto(x0_new + (dx_new / 2), y0_new + (dy_new / 2));
	//position_mouse(x0_new + (dx_new / 2), y0_new + (dy_new / 2));
    return;
#endif
}

void expand_dim(int w_x0, int w_y0, int w_width, int w_height, BOOL redraw_screen) {
    int ret_left_x, ret_right_x, ret_top_y, ret_bottom_y;
    int monitor=0;
	int ret, ret_ref;
	int keys_;

//#ifdef ALLEGRO5
//    set_semaphore(0);
//    my_sleep(100);
//#endif

	enable_hardware_cursor();

    //checking if doesn't exceed screen baundaries
    ret = get_monitor_dims(&ret_left_x, &ret_right_x, &ret_top_y, &ret_bottom_y, -1);

#ifdef LINUX
    //dx_new = w_width;
    //dy_new = w_height - X11_SCREEN_SHIFT;

	x0_new = max(w_x0, ret_left_x);
	y0_new = max(w_y0, ret_top_y);

	dx_new = w_width;

    if (dx_new>5980) dx_new-=8;  //TEMPORARY

#ifdef MACOS //ARM64
	dy_new = w_height;
#else


    /*
    char *session_type = getenv("XDG_SESSION_TYPE");
    //BOOL wayland=FALSE;

    if (session_type != NULL) {
        if (strcmp(session_type, "wayland") == 0) {
            printf("Running under Wayland\n");
            wayland=TRUE;
        } else if (strcmp(session_type, "x11") == 0) {
            printf("Running under Xorg (X11)\n");
        } else {
            printf("Unknown session type: %s\n", session_type);
        }
    } else {
        printf("XDG_SESSION_TYPE environment variable not set. Cannot determine session type.\n");
    }
     */

    if (wayland)  dy_new = w_height;
    else dy_new = w_height - X11_SCREEN_SHIFT;
#endif

    set_origins(x0_new, y0_new);
	set_resized_window(dx_new, dy_new, 0);
    ret = set_window_origin(x0_new, y0_new);  ////arm64
    ////my_sleep(10);
#else
    dx_new = w_width;
    dy_new = w_height; // -X11_SCREEN_SHIFT;

	GFX_MARGINS* gfx_margins;

	ret = get_gfx_margins(&gfx_margins);

	x0_new = max(w_x0, ret_left_x-gfx_margins->l_b);
	y0_new = max(w_y0, ret_top_y);

	set_resized_window(dx_new, dy_new, 0);
	ret = set_window_origin(x0_new, y0_new);
#endif


    //int p_mouse = my_poll_mouse();
    //int p_keyboard = my_poll_keyboard();
    //moveto(x0_new + (dx_new / 2), y0_new + (dy_new / 2));
	//position_mouse(x0_new + (dx_new / 2), y0_new + (dy_new / 2));

   ////global_resized=TRUE;
	//simulate_keypress(27);
	enable_hardware_cursor();

    flip_screen();  ////

	////my_sleep(10);
	disable_hardware_cursor();
	standard_func();
	//Test_przycisniec(&keys_);
	//cur_mouse_b=-1;
	//mouse_b=0;

//#ifdef ALLEGRO5
//    set_semaphore(1);
//#endif
} 

void restore_last_window(void)
{
#ifndef LINUX
	//int ret1;
	
	HWND wnd;
	RECT wndRect, clnRect;
	int w, w_, h, h_, dw, dh;


	if ((dx_back) > 0 && (dy_back > 0))
	{

		dx_new = dx_back;
		dy_new = dy_back;

		x0_new = x0_back;
		y0_new = y0_back;

		wnd = win_get_window();  //WINDOWS

		GetWindowRect(wnd, &wndRect);  //WINDOWS
		GetClientRect(wnd, &clnRect);  //WINDOWS


		w = wndRect.right - wndRect.left;
		w_ = clnRect.right - clnRect.left;
		

		h = wndRect.bottom - wndRect.top;
		h_ = clnRect.bottom - clnRect.top;
		

		dx_back = w_;
		dy_back = h_;
		x0_back = wndRect.left;
		y0_back = wndRect.top;

		set_resized_window(dx_new, dy_new, 1);

		wnd = win_get_window();

		GetWindowRect(wnd, &wndRect); //WINDOWS
		GetClientRect(wnd, &clnRect); //WINDOWS

		w = wndRect.right - wndRect.left;
		w_ = clnRect.right - clnRect.left;

		h = wndRect.bottom - wndRect.top;
		h_ = clnRect.bottom - clnRect.top;

		dw = (w - w_);
		dh = (h - h_);

		dx_new = wndRect.right - wndRect.left;
		dy_new = wndRect.bottom - wndRect.top;

		MoveWindow(wnd, x0_new, y0_new, dx_new, dy_new, TRUE); //WINDOWS

		//int p_mouse = my_poll_mouse();
		//int p_keyboard = my_poll_keyboard();
		//moveto(x0_back + (dx_back / 2), y0_back + (dy_back / 2));
		//position_mouse(x0_new + (dx_new / 2), y0_new + (dy_new / 2));

	}
#else
    int ret_left_x, ret_right_x, ret_top_y, ret_bottom_y;
    int monitor=0;
    int ret;
    int curr_x0, curr_y0, curr_h, curr_v;
	int ret_ref;

    ret_ref = get_window_origin_and_size(&curr_x0, &curr_y0, &curr_h, &curr_v);

    ret = get_monitor_dims(&ret_left_x, &ret_right_x, &ret_top_y, &ret_bottom_y, -1);

    restore_window_dim();

    if ((dx_new!=curr_h) || (dy_new!=curr_v) || (x0_new!=curr_x0) || (y0_new!=curr_y0)) {

        save_window_dim(curr_x0, curr_y0, curr_h, curr_v);

        dy_new -= X11_SCREEN_SHIFT;

        x0_new = max(x0_new, ret_left_x);
        y0_new = max(y0_new, ret_top_y);

        dx_new = min(dx_new, ret_right_x - ret_left_x);
        dy_new = min(dy_new, ret_bottom_y - ret_top_y - X11_SCREEN_SHIFT);


        set_origins(x0_new, y0_new);
        set_resized_window(dx_new, dy_new, 1);
        ret = set_window_origin(x0_new, y0_new);

#ifdef MACOS
    	enable_hardware_cursor();
    	show_os_cursor(MOUSE_CURSOR_ARROW);
    	disable_hardware_cursor();
#endif

        //int p_mouse = my_poll_mouse();
        //int p_keyboard = my_poll_keyboard();
        //moveto(x0_new + (dx_new / 2), y0_new + (dy_new / 2));
		//position_mouse(x0_new + (dx_new / 2), y0_new + (dy_new / 2));
    }

#endif
}

#ifndef LINUX
void Startup(LPCSTR lpApplicationName, LPSTR lpParams)
{
	startup(lpApplicationName, lpParams);
}
#endif


BOOL get_sw_callback_locked(void)
{
    return sw_callback_locked;
}

void sw_callback_locked_proc(void)
{
	sw_callback_locked = TRUE;
}

void sw_callback_push_proc(void)
{
	sw_callback_push = TRUE;
}

int circle_and_rectangle_proc0(int untrap_mouse)
{
   int ox, oy, nx, ny, ox_, oy_;
   int xx, yy;
   int res_ret;
   int cur_width;
   int cur_height;
   int w,w_,h,h_;
   int ret, ret1;
   int a;

#ifdef LINUX
   return 0;
#else
   HWND wnd;
   RECT wndRect, clnRect;

   preview_blocked = TRUE;
  
   if (GFX_WIN!=1) return 0;

   xx = 0; 
   yy = 33;
   ox=SCREEN_W;
   oy=SCREEN_H;
   ox_=ox;
   oy_=oy;
   nx=ox;
   ny=oy;
   
   res_ret=get_desktop_resolution(&cur_width, &cur_height);
   wnd = win_get_window();
   GetWindowRect(wnd, &wndRect);
   GetClientRect(wnd, &clnRect);

   w  = wndRect.right      - wndRect.left; 
   w_ = clnRect.right      - clnRect.left;
   h  = wndRect.bottom     - wndRect.top; 
   h_ = clnRect.bottom     - clnRect.top;

   cur_width-=(w-w_);
   cur_height-=(h-h_);

   x0_new = wndRect.left;
   y0_new= wndRect.top;

   dx_new = cur_width;
   dy_new = cur_height;

   if (untrap_mouse == 1)
   {
	   sw_callback_locked = TRUE;
	   Free_Mouse();
   }
   else sw_callback_locked = FALSE;

   NO_S++;
   /*
   dx_back = w;
   dy_back = h;
   x0_back = wndRect.left;
   y0_back = wndRect.top;
   */
   ret = WinSize(wndRect.left /*- 5*/, wndRect.top /*- 5*/, w /*+ 10*/, h /*+ 10*/, NO_S); 

   set_sleep_state(FALSE);

   //if (x0_new < 0) x0_new = 0;
   if (y0_new < 0)
   {
	   dy_new += y0_new;
	   y0_new = 0;

   }

   //x0_new -= 3;  //???????????????

   if (ret==1)
   {
	   /*
	dx_new = w_;
	dy_new = h_;
	x0_new = wndRect.left;
	y0_new = wndRect.top;
	*/
	 set_resized_window(dx_new, dy_new, 1);
	
	 ret1=set_window_origin(x0_new, y0_new);
     //cleardevice();
     redraw();
	 
   }
   else
   {
	   a = 1;
   }


	LockMouse();


   ////simulate_keypress(14592);

   preview_blocked = FALSE;

   //restore_focus();

  return 1;
#endif
}

int circle_and_rectangle_proc(void)
{
	return circle_and_rectangle_proc0(0);  //1
	return 0;
}

void restore_focus(void)
{
#ifdef LINUX
    return;
#else
	HWND f_hwnd;

	f_hwnd = GetForegroundWindow();

	HWND thiswnd = win_get_window();

	if (f_hwnd != thiswnd)
		simulate_keypress(14592);
		//SetFocus(thiswnd);
#endif
}

int set_focus_on_window(void)
{
#ifdef LINUX
return 0;
#else
	HWND thiswnd = win_get_window();
	SetFocus(thiswnd);
	return 0;
#endif
}

int focus_on_window(void)
{
#ifdef LINUX
    return 0;
#else
	HWND thiswnd=win_get_window();
	SetFocus(thiswnd);
	//BOOL ret=SetKeyboardState(VK_LSHIFT);
	INPUT inputs[2] = {};
	ZeroMemory(inputs, sizeof(inputs));
	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_ESCAPE;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VK_ESCAPE;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
	UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	BOOL uUpdate = UpdateWindow(thiswnd);
	//thiswnd=SetActiveWindow(thiswnd);

	//uUpdate = RedrawWindow(thiswnd, NULL, NULL, RDW_UPDATENOW);

	//int ret = request_scroll(0, 0);
	//simulate_keypress(27);

	return 0;
#endif
}

void DoneBackground(void)
{
	destroy_bitmap(background_pcx);
}

#ifdef LINUX
#ifndef MACOS
static void
set_title(Display* d, Window w, char* name)
{
    XEvent e;
    XSync(d, False);
    int ret=XChangeProperty(d, w,
                            XInternAtom(d, "WM_NAME", False),
                            XInternAtom(d, "STRING", False), 8,
                            PropModeReplace, (unsigned char*)name, strlen(name)+1);
    if(ret==0)
        return;
    ret=XChangeProperty(d, w,
                        XInternAtom(d, "_NET_WM_NAME", False),
                        XInternAtom(d, "STRING", False), 8,
                        PropModeReplace, (unsigned char*)name, strlen(name)+1);
}
#else

#endif
#endif

void my_set_window_title(char* wnd_title)
{
#ifndef LINUX
	HWND allegro_wnd = win_get_window();
	//do_uconvert(name, U_CURRENT, wnd_title, U_ASCII, WND_TITLE_SIZE);
	SetWindowText(allegro_wnd, wnd_title);
#else
#ifndef MACOS
    Display *display;
    Window focus, root_window;;
    int revert;
    display = XOpenDisplay(0);

#ifndef ALLEGRO5
    focus=_xwin.window;
#else
    focus=get_x_window_id();
#endif

    set_title(display, focus, wnd_title);
#else

#endif
#endif
}

#ifdef LINUX
#ifndef MACOS
void Set_XWindow_header_height(void) {
    Display* d;
    Window w, root;
    Atom a, t;
    int s;
    long fg, bg;
    int f;
    unsigned long n, b;
    unsigned char *data = 0;
    long* extents;
    XEvent e;

    d = XOpenDisplay(nullptr);
    s = DefaultScreen(d);
    root = DefaultRootWindow(d);
    fg = BlackPixel(d, s);
    bg = BlackPixel(d, s);
    w = XCreateSimpleWindow(d, root, 0, 0, 100, 100, 0, fg, bg);
    XSelectInput(d, w, ExposureMask|ButtonPressMask|KeyPressMask|PropertyChangeMask);

    XMapWindow(d,w);

    a = XInternAtom(d, "_NET_FRAME_EXTENTS", True); /* Property to check */

    /* Window manager doesn't set up the extents immediately */
    /* Wait until they are set up and there are 4 of them */
    while (XGetWindowProperty(d, w, a,
                              0, 4, False, AnyPropertyType,
                              &t, &f,
                              &n, &b, &data) != Success || n != 4 || b != 0)
    {
        XNextEvent(d, &e);
    }

    /* OK got extents */
    extents = (long*) data;
    ////printf ("Frame extents: left %ld right %ld top %ld bottom %ld\n", extents[0], extents[1], extents[2], extents[3]);

    X11_SCREEN_SHIFT=extents[2];  //added 2 due to invisible bottom line
    XDestroyWindow(d, w);
    XCloseDisplay(d);
}
#else
    void Set_XWindow_header_height(void) {


	X11_SCREEN_SHIFT=25;  //MACOS TEMPORARY  24points=25pxl
}


#endif
#endif

#ifdef __cplusplus
}
#endif

#undef __ALLEGROEXT__
