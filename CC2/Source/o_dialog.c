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

#define __DIALOG__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<stdlib.h>
#ifndef LINUX
#include<io.h>
#include<dos.h>
#endif
#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include "alffont.h"
#include "bib_e.h"
#include "rysuj_e.h"
#include "b_menu.h"
#include "o_dialog.h"
#include "b_mouse.h"
#include "o_image.h"
#include "bgiext.h"
#include "alf_res.h"

//#include "gui.h"

#include "menu.h"
#include "dialog.h"

#include "leak_detector_c.h"
#ifndef LINUX
#define LONG long

typedef struct tagRECT {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT, * PRECT, * NPRECT, * LPRECT;
#endif

//BITMAP *slbitmap=NULL;

static int (*kls)(void);
static void (*mv)(int x, int y);
static int (*tab_fun)(void);
static void (*CUR)(int ,int);

static PTMENU *cur_tipsmenu=NULL;

static int xmousex, ymousey;
static p_point bar_center;

static BOOL block_changed=FALSE;
static TDIALOG *Dlg=NULL;

static int x001[4], y001[4], x002[4], y002[4];
static int dx_mov, dy_mov;
static int Dlg_xb, Dlg_yb;
static int Typ=0;
static int set_listbox_slider=0;
static char *listbox_address=NULL;

extern void invert_bitmap(BITMAP *bmp);
extern BOOL XORBAR;

extern int d_myslider_proc(int msg, void *d_, int c);

static int listbox_grab_slider(void *dp3, int d2);
static int listbox_init_slider(int *var1, int *var2, int *var3, int *var4);

static SLIDER listbox_slider={d_myslider_proc,  0, 0,  10, 0,  0,  98,    0,   0,  100,   0,    NULL , listbox_grab_slider, listbox_init_slider, NULL};

extern void mycirclefill(int x1, int y1, int radius);
extern void myarc(int x1, int y1, fixed ang1, fixed ang2, int radius);
extern void myline(int x1, int y1, int x2, int y2);
extern void mycircle(int x, int y, int radius);
extern void Test_przycisniec(int *keys);
extern int  Test_przycisniec0(int *keys);
extern int my_poll_mouse(void);
extern void  Odczyt_licznikow(void);
extern void  Odczyt_licznikow_simple(void);
extern void flip_full_screen(BITMAP * the_screen);
extern void putkeepimage(int left, int top, BITMAP  *bitmap, int op);
extern int get_pYk(void);

extern  BITMAP *screenplay;

#ifdef ALLEGRO5
static int mouse_x0, mouse_y0;
#endif

#define LINE myline
#define CIRCLE mycircle
#define CIRCLEFILL mycirclefill
#define ARC myarc

#define ID_CFontName    38  //from o_chprop.c

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

extern char *Colors_Name_txt[MAXCOLOR + 1];

extern int up_layer_(void);
extern int dn_layer_(void);
extern int pgup_layer_(void);
extern int pgdn_layer_(void);

extern int MP_SIZE_TAB[];
extern int ED_INF_HEIGHT_TAB[];
extern int BAR_G_TAB[];
extern int HEIGHT_TAB[];
extern int WIDTH_TAB[];

extern int movex0;
extern int deltax0;
extern int movey0;
extern int deltay0;

extern int i__font_nomax;

extern void outetextxy(int x, int y, int maxlength, int width, char *s,
					int ink, int paper);;
extern void setcolor(int kolor);
//extern void myline(int x1, int y1, int x2, int y2);
extern void setfillstyle_(int pattern, int color);
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);
extern void setwritemode( int mode );
extern int GetColorAC1(int color);
extern void GrMouseEraseCursor(void);
extern void GrMouseUnEraseCursor(void);
extern void getimage(int left, int top, int right, int bottom, void  *bitmap);
extern void putimage(int left, int top,  void  *bitmap, int op);
extern void lock_mouse(void);
extern void free_mouse(void);
extern int get_string1 (char *tekst, char *legal, int maxlength, int width, int kom, int x, int y);
extern int colors1(int old_color);
extern void GrMouseDisplayCursor(void);
extern void settextjustify(int horiz, int vert);
extern void fillpoly(int numpoints, void *polypoints);
extern int getmaxcolor(void);
extern void putpixel_(int x, int y, int color);
extern void delay(int time);
extern void rectangle(int left, int top, int right, int bottom);
extern void setfillpattern_(char *pattern, int color);
extern void setviewport(int left, int top, int right, int bottom, int clip);
extern int get_window_origin_and_size(int *x_win_orig, int *y_win_orig, int *win_width, int *win_height);
extern void set_resized_window(int dx, int dy, int option);
extern int centre_window(void);
extern int set_window_origin(int x_win_orig, int y_win_orig);
extern int __gr_White(void);

extern float ttf_width_w;
extern void draw_font_name(char* name, unsigned char fontno, int x, int y);
extern void draw_font_name_ttf(TEXT *t, char *t_text, BITMAP *bmp, int x0, int y0, int xmax, double kat, float wysokosc_p, COLOR_ kolor, int mode, int t_width, int t_height);
extern int findfpostopxl(char *s, int max_pxl);
extern int utf8len(const char *s);
extern int lineCuncut(long x1, long y1, long x2, long y2);

extern void reset_if_resized(void);
extern int TTF_text_len(char* text);
extern int getcolor(void);
extern int fnsplit(const char* path, char* drive, char* dir, char* name, char* ext);
extern int ViewBlock(char* FileName, BOOL set_color);
extern int ViewInsBlock(long adr, BOOL set_color);
extern int ViewPattern(char* FileName, char* ext);

extern void dimm_dialog_bitmap(BITMAP *src, BITMAP *dst, int dx, int dy, int gray_sat);

extern void TTF_text_UI(const char *text, int x, int y, int *text_h, int *text_v);
extern void TTF_text_UI_(BITMAP *ui_screen, const char *text, int x, int y, int *text_h, int *text_v);

extern void show_x_cursor(void);

extern void disable_F11(void);
extern void enable_F11(void);
extern int get_monitor_dims(int *ret_left_x, int *ret_right_x, int *ret_top_y, int *ret_bottom_y, int mon);
#ifdef LINUX
extern void xwin_set_hints(int dx, int dy);
#endif

extern void save_window_dim(int x0_, int y0_, int dx_, int dy_);
extern void expand_dim(int x0, int y0, int width, int height);
extern void clean_key_buffer(void);
extern BOOL get_close_button_pressed(void);
extern char *Get_instruct_dlg(void);

extern void set_mode_solid(void);
extern int TRANSLUCENCY;

extern BITMAP *alfa_mouse_null;
extern BITMAP *alfa_mouse_sprite;

extern TDIALOG layers_dlg;

extern void Set_Screenplay(BITMAP *ctx_bitmap);
extern BITMAP *Get_Screenplay(void);
extern void  outtext_r_(BITMAP *ui_screen, char  *textstring);
extern int get_palette_color(int color);

extern void _free_mouse(void);

extern int d_myslider_proc(int msg, void *d, int c);
extern int gui_border_dark, gui_border_light;

extern char *BLOCKLIST;
extern BOOL BAR_POINTER;

extern BOOL is_RETINA(void);

#ifdef ALLEGRO5
extern void position_mouse_(int x, int y);
extern void position_mouse_xy(int x, int y);
#ifdef MACOS
extern void MoveOSXCursor(int x, int y);
#endif
#endif

extern int vfv(int v);

BOOL NO_POINTER=FALSE;

extern char *icon_empty_p;

extern char *qmark_p;
extern char *eymark_p;
extern char *ermark_p;
extern char *offmark_p;
extern char *icon_upgrademark_pmem;
extern char *icon_noupgrademark_pmem;
extern char *plotter_p;
extern char *icon_visible_d_p;
extern char *icon_edit_d_p;
extern char *icon_pointfinder_d_p;
extern char *icon_color_d_p;
extern char *icon_line_thickness_d_p;
extern char *icon_line_type_d_p;
extern char *icon_grey_d_p;
extern char *icon_blackwhite_d_p;
extern char *icon_plotter_s_p;
extern char *icon_visible_db_p;
extern char *icon_edit_db_p;
extern char *icon_pointfinder_db_p;
extern char *icon_grey_db_p;
extern char *icon_blackwhite_db_p;
extern char *icon_plotter_64_p;
extern char *icon_saveimage_64_p;
extern char *icon_rotation_d48_p;
extern char *icon_inversion_d48_p;
extern char *icon_reflection_d48_p;
extern char *icon_grey_d48_p;
extern char *icon_blackwhite_d48_p;
extern char *icon_grey_image_d48_p;
extern char *icon_window_d48_p;
extern char *icon_sheet_d48_p;
extern char *icon_scale_d48_p;
extern char *icon_page_d48_p;
extern char *icon_color_d48_p;

extern char *icon_scale_d_p;
extern char *icon_page_d_p;
extern char *icon_page_width_d_p;
extern char *icon_page_height_d_p;
extern char *icon_page_overlap_d_p;

extern char *icon_very_thin_d_p;
extern char *icon_thin_d_p;
extern char *icon_thick_d_p;
extern char *icon_very_thick_d_p;
extern char *icon_extra_thick_d_p;

extern char *icon_cartridge_d48_p;
extern char *icon_color256_d48_p;

extern char *icon_bold_d_p;
extern char *icon_font_type_d_p;
extern char *icon_hidden_text_d_p;
extern char *icon_italic_d_p;
extern char *icon_layers_d_p;
extern char *icon_text_align_d_p;
extern char *icon_text_height_d_p;
extern char *icon_text_type_d_p;
extern char *icon_width_d_p;
extern char *icon_underlined_d_p;
extern char *icon_yes_d_p;
extern char *icon_no_d_p;
extern char *icon_yes_d_pmem;
extern char *icon_no_d_pmem;
extern char *icon_no_d_12_p;
extern char *icon_mark_d_12_p;

extern char *icon_hor_dpi_sphere_blue_d_p;
extern char *icon_ver_dpi_sphere_blue_d_p;
extern char *icon_hor_size_image_d_p;
extern char *icon_ver_size_image_d_p;
extern char *icon_hor_scale_image_d_p;
extern char *icon_ver_scale_image_d_p;
extern char *icon_background_image_d_p;
extern char* icon_background_image_d48_p;
extern char *icon_rotate_angle_image_d_p;

extern char *icon_top_margin_d_p;
extern char *icon_left_margin_d_p;
extern char *icon_bottom_margin_d_p;
extern char *icon_right_margin_d_p;

extern char *icon_actual_size_d48_p;

extern char *icon_arrow_up_d48_p;
extern char* icon_arrow_up_end_d48_p;
extern char* icon_arrow_down_end_d48_p;
extern char *icon_arrow_down_d48_p;
extern char *icon_folder_d48_p;
extern char *icon_folder0_d48_p;
extern char *icon_smaller_d48_p;
extern char *icon_bigger_d48_p;

extern char *icon_arrow_up_d_p;
extern char *icon_arrow_down_d_p;
extern char *icon_pgup_d_p;
extern char *icon_pgdn_d_p;
extern char *icon_arrow_up_left_d_p;
extern char *icon_arrow_down_left_d_p;

extern char *icon_plotter_start_d_64_p;
extern char *icon_printer_start_d_64_p;
extern char *icon_printer_d_48_p;
extern char *icon_escape_d_64_p;
extern char *icon_save_d_p;
extern char *icon_load_d_p;
extern char *icon_return_d_24_p;

extern char *icon_ok_d_64_p;
extern char *icon_new_layer_d_64_p;

extern char *icon_hor_extents_d_p;
extern char *icon_ver_extents_d_p;
extern char *icon_hor_limits_d_p;
extern char *icon_ver_limits_d_p;
extern char *icon_units_d_p;
extern char *icon_paper_size_d_48_p;
extern char *icon_paper_size_d_p;
extern char *icon_paper_dim_d_48_p;

extern char *icon_yes_d_48_p;
extern char *icon_no_d_48_p;
extern char *icon_escape_d_48_p;
extern char *icon_escape_d_p;

extern char *icon_load_d_48_p;
extern char *icon_load_ini_d_p;
extern char *icon_load_ini_d_48_p;

extern char *icon_touch_id_db_64_p;
extern char *icon_left_margin_d_48_p;
extern char *icon_bottom_margin_d_48_p;

extern char *icon_all_layers_db_64_p;

extern char *icon_global_db_48_p;
extern char *icon_local_db_48_p;

extern char *icon_color256_db_48_p;
extern char *icon_all_windows_p;

extern char *icon_question_mark_db_48_p;

extern char *icon_pdf_vector_64_p;

extern char* icon_folder_bd24_p;
extern char* icon_folder_bd32_p;
extern char* icon_folder_bd48_p;
extern char* icon_folder_bd64_p;

extern char* icon_chain_d_p;

extern char* icon_on_top_d_p;

extern char* icon_h_flip_p;
extern char* icon_v_flip_p;

extern char * icon_trans50_d_p;
extern char * icon_refresh_d_p;

extern char *icon_alpha_sorting_d32_p;
extern char *icon_time_lapse_d32_p;
/////////////////////////////
extern char *icon_ctrl_p;
extern char *icon_alt_p;
extern char *icon_shift_p;
extern char *icon_home_p;
extern char *icon_end_p;
extern char *icon_esc_p;
extern char *icon_ins_p;
extern char *icon_pgdn_p;
extern char *icon_pgup_p;
extern char *icon_spc_p;
extern char* icon_del_p;
extern char *icon_tab_p;
extern char* icon_k_right_p;
extern char* icon_k_left_p;
extern char *icon_3_p;
extern char *icon_4_p;
extern char* icon_k_up_p;
extern char* icon_k_down_p;
extern char *icon_0_p;
extern char *icon_1_p;
extern char *icon_2_p;
extern char *icon_5_p;
extern char *icon_6_p;
extern char *icon_7_p;
extern char *icon_8_p;
extern char *icon_9_p;
extern char *icon_plus_p;
extern char *icon_minus_p;
extern char *icon_f1_p;
extern char *icon_f2_p;
extern char *icon_f3_p;
extern char *icon_f4_p;
extern char *icon_f5_p;
extern char *icon_f6_p;
extern char *icon_f7_p;
extern char *icon_f8_p;
extern char *icon_f9_p;
extern char *icon_f10_p;
extern char *icon_f11_p;
extern char *icon_f12_p;
extern char *icon_mouse1b_p;
extern char* icon_mouse2b_p;
extern char* icon_mouse1b2b_p;
extern char* icon_mouse3b_p;
extern char* icon_mouseRb_p;
extern char* icon_space128_p;
extern char* icon_backspace128_p;
extern char* icon_enter128_p;
extern char *icon_A_p;
extern char *icon_B_p;
extern char *icon_C_p;
extern char *icon_D_p;
extern char *icon_E_p;
extern char *icon_F_p;
extern char *icon_G_p;
extern char *icon_H_p;
extern char *icon_I_p;
extern char *icon_J_p;
extern char *icon_K_p;
extern char *icon_L_p;
extern char *icon_M_p;
extern char *icon_N_p;
extern char *icon_O_p;
extern char *icon_P_p;
extern char *icon_Q_p;
extern char *icon_R_p;
extern char *icon_S_p;
extern char *icon_T_p;
extern char *icon_U_p;
extern char *icon_V_p;
extern char *icon_W_p;
extern char *icon_X_p;
extern char *icon_Y_p;
extern char *icon_Z_p;

extern char* icon_UA_B_p;
extern char* icon_UA_D_p;
extern char* icon_UA_E_p;
extern char* icon_UA_J_p;
extern char* icon_UA_K_p;
extern char* icon_UA_N_p;
extern char* icon_UA_P_p;
extern char* icon_UA_R_p;
extern char* icon_UA_S_p;
extern char* icon_UA_T_p;
extern char* icon_UA_C_p;

extern char *icon_tab_p;

extern char *icon_eurocode_d48_p;
extern char *icon_asce_d48_p;
extern char *icon_icc_d48_p;

extern char *icon_combination_d48_p;
extern char *icon_erase_layer_db_64_p;
extern char *icon_mark_layer_db_64_p;
extern char *icon_Pdelta_d48_p;

extern char *icon_vibrations_d48_p;
extern char *icon_inertia_d48_p;

extern char *icon_hourglass_p;
extern char *icon_pin_to_flex_d48_p;

extern char *icon_view_log_d_48_p;

/////////////////////////////

extern FORTIPS_MAP fortips_map;

static void init(char typ, TDIALOG* Dlg, PTMENU* tipsmenu);
static void draw_check_box(BUTTON *Button);

static TDIALOG dlg0;

static int global_dialog_flag = 0;
static char global_dialog_name[16];
static char *global_dialog_ptr=NULL;
static int curr_x0, curr_y0, curr_h, curr_v;
static int ret_left_x, ret_right_x, ret_top_y, ret_bottom_y;

int  extra1 = 1;

int Get_Global_Dialog_Flag(void)
{
	return global_dialog_flag;
}

char *Get_Global_Dialog_Name(void)
{
	return &global_dialog_name;
}

BOOL get_block_changed(void)
{
    return block_changed;
}

static void  nooop(void)
{}

void(*ProcFG)(void) = nooop;

#define WIDTH_DISPLAY 640
#define HEIGHT_DISPLAY 348
#define HEIGHT_CAPTION 16
#define BORDER_DARK DARKGRAY
#define BORDER_LIGHT      15 //WHITE
#define SIZE_RADIOBUTTON0  8 //7
#define SIZE_ROUNDRADIOBUTTON0  4
#define DXGB 5
#define DYGB 5
#define SKOKM 20 //10
#define baron baronoff
#define kolor_w 97  //103

static int SIZE_CHECKBOX0=14;

int SIZE_RADIOBUTTON = SIZE_RADIOBUTTON0;
int SIZE_ROUNDRADIOBUTTON = SIZE_ROUNDRADIOBUTTON0;
int SIZE_CHECKBOX=14;

extern int PozX,PozY;
extern int PozX0,PozY0;
static int krok=1, DX, DY, Xmax_, Ymax_, pocz_x, pocz_y, pocz_x0, pocz_y0;
static int i__x1w, i__x2w, i__y1w, i__y2w;
static DLG_COLOR *dlg_kolory;
static double wsp_x = 1.5, wsp_y = 1.5;
static LISTBOX *listbox_usk;
static TDIALOG *ptrs__dlg = NULL ;

 static int( *SW[123])(void) ;
 static int( *SW01)(void) ;
 extern int( *SW2[13])(void) ;

extern int SkalaZ_Minus(int sclfct);
extern int SkalaZ_Plus(int sclfct);

 BOOL find_groups = FALSE;


 char *get_icon(int iconno)
 {

	 char* icons[] = { qmark_p, plotter_p, icon_visible_d_p, icon_edit_d_p, icon_pointfinder_d_p, icon_color_d_p, icon_line_thickness_d_p, icon_line_type_d_p, icon_grey_d_p, icon_blackwhite_d_p,
		 /*10*/ icon_plotter_s_p, icon_visible_db_p, icon_edit_db_p, icon_pointfinder_db_p, icon_grey_db_p, icon_blackwhite_db_p,
		 /*16*/ icon_plotter_64_p, icon_saveimage_64_p,
		 /*18*/ icon_rotation_d48_p, icon_inversion_d48_p, icon_reflection_d48_p, icon_grey_d48_p, icon_blackwhite_d48_p, icon_grey_image_d48_p,
		 /*24*/ icon_window_d48_p, icon_sheet_d48_p, icon_scale_d48_p, icon_page_d48_p, icon_color_d48_p,
		 /*29*/ icon_scale_d_p, icon_page_d_p, icon_page_width_d_p, icon_page_height_d_p, icon_page_overlap_d_p,
		 /*34*/ icon_very_thin_d_p, icon_thin_d_p, icon_thick_d_p, icon_very_thick_d_p, icon_extra_thick_d_p, icon_cartridge_d48_p,
		 /*40*/ icon_color256_d48_p, icon_bold_d_p, icon_font_type_d_p, icon_hidden_text_d_p, icon_italic_d_p, icon_layers_d_p, icon_text_align_d_p, icon_text_height_d_p, icon_text_type_d_p, icon_width_d_p, icon_underlined_d_p,
		 /*51*/ icon_yes_d_pmem, icon_no_d_pmem,
		 /*53*/ icon_hor_dpi_sphere_blue_d_p, icon_ver_dpi_sphere_blue_d_p, icon_hor_size_image_d_p, icon_ver_size_image_d_p, icon_hor_scale_image_d_p, icon_ver_scale_image_d_p, icon_background_image_d_p, icon_rotate_angle_image_d_p,
		 /*61*/ eymark_p, ermark_p, offmark_p, icon_top_margin_d_p, icon_left_margin_d_p, icon_actual_size_d48_p, icon_bottom_margin_d_p, icon_right_margin_d_p,
		 /*69*/ icon_arrow_up_d48_p, icon_arrow_down_d48_p, icon_folder_d48_p, icon_folder0_d48_p, icon_bigger_d48_p, icon_smaller_d48_p,
		 /*75*/ icon_arrow_up_d_p, icon_arrow_down_d_p, icon_pgup_d_p, icon_pgdn_d_p, icon_arrow_up_left_d_p, icon_arrow_down_left_d_p,
		 /*81*/ icon_plotter_start_d_64_p, icon_printer_start_d_64_p, icon_escape_d_64_p, icon_save_d_p, icon_load_d_p, icon_return_d_24_p,
		 /*87*/ icon_ok_d_64_p, icon_new_layer_d_64_p, icon_printer_d_48_p,
		 /*90*/ icon_hor_extents_d_p, icon_ver_extents_d_p, icon_hor_limits_d_p, icon_ver_limits_d_p, icon_units_d_p, icon_paper_size_d_48_p, icon_paper_dim_d_48_p,
		 /*97*/ icon_yes_d_48_p, icon_no_d_48_p, icon_escape_d_48_p, icon_yes_d_pmem, icon_no_d_pmem, icon_escape_d_p,
		 /*103*/ icon_load_d_48_p, icon_load_ini_d_p, icon_load_ini_d_48_p, icon_touch_id_db_64_p, icon_left_margin_d_48_p, icon_bottom_margin_d_48_p,
		 /*109*/ icon_all_layers_db_64_p, icon_global_db_48_p, icon_local_db_48_p, icon_color256_db_48_p,
		 /*113*/ icon_all_windows_p, icon_question_mark_db_48_p, icon_pdf_vector_64_p, icon_folder_bd24_p, icon_folder_bd32_p, icon_folder_bd48_p, icon_folder_bd64_p,
		 /*120*/ icon_arrow_up_end_d48_p, icon_chain_d_p, icon_on_top_d_p, icon_h_flip_p, icon_v_flip_p, icon_background_image_d48_p,
		 /*126*/ icon_trans50_d_p, icon_refresh_d_p,
         /*128*/ icon_ctrl_p, icon_alt_p, icon_shift_p, icon_f1_p, icon_f2_p, icon_f3_p, icon_f4_p, icon_f5_p, icon_f6_p, icon_f7_p, icon_f8_p, icon_f9_p, icon_f10_p, icon_f11_p,icon_f12_p,
         /*143*/ icon_space128_p, icon_backspace128_p, icon_enter128_p, icon_esc_p, icon_mouse1b_p, icon_mouse2b_p, icon_mouse3b_p, icon_mouseRb_p, icon_ins_p,
         /*152*/ icon_A_p, icon_B_p, icon_C_p, icon_D_p, icon_E_p, icon_F_p, icon_G_p, icon_H_p, icon_I_p, icon_J_p, icon_K_p, icon_L_p,
         /*164*/ icon_M_p, icon_N_p, icon_O_p, icon_P_p, icon_Q_p, icon_R_p, icon_S_p, icon_T_p, icon_U_p, icon_V_p,
         /*174*/ icon_W_p, icon_X_p, icon_Y_p, icon_Z_p,
         /*178*/ icon_UA_B_p, icon_UA_D_p, icon_UA_E_p, icon_UA_J_p, icon_UA_K_p, icon_UA_N_p, icon_UA_P_p, icon_UA_R_p, icon_UA_S_p, icon_UA_T_p, icon_UA_C_p,
         /*189*/ icon_tab_p, icon_arrow_down_end_d48_p, icon_alpha_sorting_d32_p, icon_time_lapse_d32_p, icon_no_d_12_p,
         /*194*/ icon_eurocode_d48_p, icon_asce_d48_p, icon_icc_d48_p, icon_combination_d48_p, icon_erase_layer_db_64_p, icon_mark_layer_db_64_p,
         /*200*/ icon_mark_d_12_p, icon_Pdelta_d48_p, icon_upgrademark_pmem, icon_noupgrademark_pmem, icon_vibrations_d48_p, icon_inertia_d48_p,
         /*206*/ icon_mouse1b2b_p, icon_paper_size_d_p, icon_empty_p, icon_hourglass_p, icon_pin_to_flex_d48_p, icon_view_log_d_48_p,
     };

	 return icons[iconno];
 }

#ifdef ICON_SIZE_TAB   //is ignored
 int get_icon_size__(int iconno)
 {
	 int icon_size[] = {
	  32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	  /*10*/ 32, 32, 32, 32, 32, 32,
	  /*16*/ 64, 64,
	  /*18*/ 48, 48, 48, 48, 48, 48,
	  /*24*/ 32, 32, 32, 32, 32,
	  /*29*/ 32, 32, 32, 32, 32,
	  /*34*/ 32, 32, 32, 32, 32, 32,
	  /*40*/ 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	  /*51*/ 32, 32,
	  /*53*/ 32, 32, 32, 32, 32, 32, 32, 32,
	  /*61*/ 32, 32, 32, 32, 32, 32, 32, 32,
	  /*69*/ 48, 48, 48, 48, 48, 48,
	  /*75*/ 32, 32, 32, 32, 24, 24,
	  /*81*/ 64, 64, 64, 32, 32, 24,
	  /*87*/ 64, 64, 48,
	  /*90*/ 32, 32, 32, 32, 32, 48, 48,
	  /*97*/ 48, 48, 48, 32, 32, 32,
	  /*103*/ 48, 32, 48, 64, 48, 48,
	  /*109*/ 64, 48, 48, 48, 64, 48, 64, 32, 32, 48, 64, 48, 32, 32, 
	  /*123*/ 32, 32, 48, 32, 32,
      /*128*/ 64, 64,64,64,64,64,64,64,64,64,64,64,64,64,64,
      /*143*/ 64,64,64,64,64,64,64,64, 64,
      /*152*/64,64,64,64,64,64,64,64, 64,
      /*161*/ 64,64,64,64,64,64,64,64, 64,
      /*170*/64,64,64,64,64,64,64,64,
      /*178*/64,64,64,64,64,64,64,64,
      /*186*/64,64,64,64, 48, 32, 32, 14, 48, 48, 48, 48, 64, 64, 14, 48, 64, 64, 48, 48,
      /*206*/64,
     };
	 return icon_size[iconno];
 }
#endif

int jed_to_piks_x(int jednostki_x)
/*--------------------------------------*/
{
  return(0.5 + jednostki_x * wsp_x);
}

int piks_x_to_jed(int piks_x)
/*--------------------------------------*/
{
    return(0.5 + piks_x / wsp_x);
}

int jed_to_piks_y(int jednostki_y)
/*--------------------------------------*/
{
  return(0.5 + jednostki_y * wsp_y);
}

int piks_y_to_jed(int piks_y)
/*--------------------------------------*/
{
    return(0.5 + piks_y / wsp_y);
}

#ifdef aaaa
static int piks_to_jed_x(int piksele_x)
/*--------------------------------------*/
{
  return(0.5 + piksele_x / wsp_x);
}

static int piks_to_jed_y(int piksele_y)
/*--------------------------------------*/
{
  return(0.5 + piksele_y / wsp_y);
}
#endif

static void cur_off(int x,int y);
static void cur_on(int x,int y);
static void cur_move(int x, int y);

static int noop1(void)
{ return 0; }


static int  nooop1(void)
{
 return 0 ;
}

#define noop (int( *)(TMENU  *menu))nooop1

#define noopmv (void( *)(int x,int y))nooop

void Get_Dlg_Rect(TDIALOG *dlg, int *x1, int *y1, int *x2, int *y2)
/*-----------------------------------------------------------------*/
{
  *x1 = jed_to_piks_x(dlg->x);
  *y1 = 2 * ED_INF_HEIGHT + jed_to_piks_y(dlg->y);
  *x2 = *x1 + 3 + jed_to_piks_x(dlg->dx) + 2;
  *y2 = *y1 + 3 + 1 + HEIGHT_CAPTION + jed_to_piks_y(dlg->dy) + 2;

  if (*x1<0)
  {
      *x2-=*x1;
      *x1=0;
  }

  if (*x2>getmaxx())
  {
      *x2=getmaxx();
      *x1=*x2 - 3 - jed_to_piks_x(dlg->dx) - 2;
      if (*x1<0)
      {
          *x1=0;
          *x2=*x1 + 3 + jed_to_piks_x(dlg->dx) + 2;
      }
  }

    if (*y1<0)
    {
        *y2-=*y1;
        *y1=0;
    }

  if (*y2>getmaxy())
  {
     *y2=getmaxy();
     *y1=*y2 - 3 - 1 - HEIGHT_CAPTION - jed_to_piks_y(dlg->dy) - 2;
     if (*y1<0)
     {
        *y1=0;
        *y2 = *y1 + 3 + 1 + HEIGHT_CAPTION + jed_to_piks_y(dlg->dy) + 2;
     }
  }
}

TDIALOG *Get_Dlg(void)
/*--------------------*/
{
  return ptrs__dlg ;
}

static int read_dlg(char  *tekst, int ink, int paper,int ink_ini, int paper_ini, int lmax, int width0, int dx, char *legal)
/*--------------------------------------------------------------------------------------------------------------------------------*/
{
  int zn;
  int inkk, paperk, inkk_ini, paperk_ini;
  void(*CUR)(int ,int);
  float width;

  //int x1, y1, x2, y2;

  paperk = kolory.paperk;
  inkk = kolory.inkk;
  paperk_ini = kolory.paperk_ini;
  inkk_ini = kolory.inkk_ini;
  kolory.paperk = paper ;
  kolory.inkk = ink;
  kolory.paperk_ini = paper_ini;
  kolory.inkk_ini = ink_ini;
  CUR = MVCUR ;
  MVCUR = noopmv ;

  if (lmax < 10)
  {
	  zn = editstring(tekst, legal, lmax, (float)lmax, FALSE, 0, TRUE, 5, 3);
  }
  else
  {
	  ////width = (dx - 4 * DXIL - ttf_width_w) / ttf_width_w + extra1;
      width = dx / ttf_width_w;
	  zn = editstring(tekst, legal, lmax, width, FALSE, 1, TRUE, 5, 3);
      ////zn = editstring(tekst, legal, lmax, dx, FALSE, 1, TRUE, 5, 3);
  }

  _free_mouse();

  MVCUR = CUR ;
  kolory.paperk = paperk;
  kolory.inkk = inkk;
  kolory.paperk_ini = paperk_ini;
  kolory.inkk_ini = inkk_ini;
  return zn;
}

static void draw_lines(DARK_LIGHT_LINE*Lines,int SizeLinT)
/*-----------------------------------------------------*/
{
  int i ;
  int x1, y1, x2, y2;
  char dark, ligth;

  for(i=0;i<SizeLinT;i++)
  {
    dark = Lines[i].dark == COLOR_NULL ? dlg_kolory->dlg_lin_dark : Lines[i].dark;
    ligth = Lines[i].ligth == COLOR_NULL ? dlg_kolory->dlg_lin_ligth : Lines[i].ligth;
    x1 = jed_to_piks_x(Lines[i].x1)+pocz_x;
    y1 = jed_to_piks_y(Lines[i].y1)+pocz_y;
    x2 = jed_to_piks_x(Lines[i].x1 + Lines[i].dx - 1)+pocz_x;
    y2 = jed_to_piks_y(Lines[i].y1 + Lines[i].dy - 1)+pocz_y;
    if( x1 == x2)
    {
      if(dark != LINE_NULL)
      {
	 setcolor(dark);
	 LINE(x1, y1, x1, y2);
      }
      if(ligth != LINE_NULL)
      {
	setcolor(ligth);
	LINE( x1+1, y1, x1+1, y2);
      }
    }
    else
    if(y1 == y2)
    {
      if(dark != LINE_NULL)
      {
	setcolor(dark);
	LINE(x1, y1, x2, y1);
      }
      if(ligth != LINE_NULL)
      {
	setcolor(ligth);
	LINE( x1, y1+1, x2, y1+1);
      }
    }
  }
}


void Draw_Label(LABEL *Label, BOOL backg)
/*---------------------------------------*/
{
  int  ink, paper;
  int x1, y1, x2, y2;

  ink = Label->ink == COLOR_NULL ? dlg_kolory->dlg_label_ink : Label->ink;
  paper = Label->paper == COLOR_NULL ? dlg_kolory->dlg_label_paper : Label->paper;
  x1 = jed_to_piks_x(Label->x)+pocz_x;
  y1 = jed_to_piks_y(Label->y)+pocz_y;
  x2 = jed_to_piks_x(Label->x + Label->dx - 1) + pocz_x;
  y2 = jed_to_piks_y(Label->y + Label->dy - 1)+pocz_y;
  if(backg == TRUE)
  {
    setfillstyle_(SOLID_FILL, paper);
    bar(x1, y1, x2, y2 + 2);
  }
  if(Label->shadow==1)
   {
     setcolor(Label->s_ink);
     moveto( x1 + DXIL + 1, y1 + DYIL - 1);
     outtext_r( Label->txt );
   }
  setcolor(ink);
  moveto( x1 + DXIL, y1 + DYIL - 2);
  outtext_r( Label->txt );
}

static void draw_labels(LABEL *Labels,int SizeLabT)
/*------------------------------------------------*/
{
  int i ;

  for(i=0;i<SizeLabT;i++)
  {
    Draw_Label(&Labels[i], FALSE);
  }
}

static void redraw_labels(LABEL *Labels, int SizeLabT)
/*------------------------------------------------*/
{
	int i;

	for (i = 0; i < SizeLabT; i++)
	{
		Draw_Label(&Labels[i], TRUE);
	}
}

static void Draw_Group_Header(GROUP_BOX  *Group, int push)
/*-------------------------------------------------------*/
{
	int ink, paper;
	int x1, x2, y1, y2;
	double dy1;
	int txtpos;
	void(*ProcF)(void) = nooop;

	setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
	setwritemode(COPY_PUT);

	if (push==1)
	{
		ink = Group->paper == COLOR_NULL ? dlg_kolory->dlg_group_paper : Group->paper;
		paper = Group->ink == COLOR_NULL ? dlg_kolory->dlg_group_ink : Group->ink; 
	}
	else
	{
		paper = Group->paper == COLOR_NULL ? dlg_kolory->dlg_group_paper : Group->paper;
		ink = Group->ink == COLOR_NULL ? dlg_kolory->dlg_group_ink : Group->ink;
	}
	
	dy1 = 19 * WIDTH / 11;

	x1 = jed_to_piks_x(Group->x) + pocz_x;
	x2 = jed_to_piks_x(Group->x + Group->dx - 1) + pocz_x;
	y1 = jed_to_piks_y(Group->y) + pocz_y;
	y2 = y1 + dy1;
	x1 += 1 + x1 % 2;
	x2 += x2 % 2;
	y1 += 1 + y1 % 2;
	y2 += y2 % 2;

	setfillstyle_(SOLID_FILL, paper);
	bar(x1, y1, x2, y2);
	if (Group->txt[0] != '\0')
	{
	    setcolor(ink);
		moveto(x1 + DXGB, y1 + DYGB - 2); //1);
		txtpos = findfpostopxl(Group->txt, x2 - x1 - 2 * DXGB);
		outtext_r(&Group->txt[txtpos]);
		setcolor(BORDER_DARK);
	}

}

static void draw_groups(GROUP_BOX  *Groups, int SizeGroupT, TDIALOG *dlg, BOOL added_function)
/*-------------------------------------------------------------------------------------------*/
{
  int i, ink, paper ;
  int x1,x2, y1, y2;
  double dy1;
  BOOL function_in;

#ifndef LINUX
  //void(*ProcF)(void) = nooop;
  void(*ProcF)(int, int, int, int, int, int, int) = (void (*)(int, int, int, int, int, int, int)) nooop;
#else
  void(*ProcF)(int, int, int, int, int, int, int) = (void (*)(int, int, int, int, int, int, int)) nooop;
#endif
  int txtpos;

  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
  setwritemode(COPY_PUT);
  
  for(i = 0; i < SizeGroupT; i++)
  {
	  if (Groups[i].process == NULL) function_in = FALSE;
	  else function_in = TRUE;
	  if (function_in == added_function)
	  {
		  paper = Groups[i].paper == COLOR_NULL ? dlg_kolory->dlg_group_paper : Groups[i].paper;
		  ink = Groups[i].ink == COLOR_NULL ? dlg_kolory->dlg_group_ink : Groups[i].ink;
		  x1 = jed_to_piks_x(Groups[i].x) + pocz_x;
		  x2 = jed_to_piks_x(Groups[i].x + Groups[i].dx - 1) + pocz_x;
		  y1 = jed_to_piks_y(Groups[i].y) + pocz_y;
		  y2 = jed_to_piks_y(Groups[i].y + Groups[i].dy - 1) + pocz_y;
		  x1 += 1 + x1 % 2;
		  x2 += x2 % 2;
		  y1 += 1 + y1 % 2;
		  y2 += y2 % 2;

		  setfillstyle_(SOLID_FILL, paper);
		  bar(x1, y1, x2, y2);
		  if (Groups[i].txt[0] != '\0')
		  {
			  setcolor(ink);
			  moveto(x1 + DXGB, y1 + DYGB - 2); //1);

			  txtpos = findfpostopxl(Groups[i].txt, x2 - x1 - 2*DXGB);

			  outtext_r(&Groups[i].txt[txtpos]);
			  setcolor(BORDER_DARK);

			  dy1 = 19 * WIDTH / 11;
			  y1 += dy1;
			  LINE(x1, y1, x2, y1);
		  }
		  setcolor(BORDER_DARK);
		  LINE(x1, y1, x2, y1);
		  LINE(x1, y1, x1, y2);
		  setcolor(BORDER_LIGHT);
		  LINE(x1, y2, x2, y2);
		  LINE(x2, y2, x2, y1);

		  if (Groups[i].process != NULL)
		  {
			  ProcF = (void *)Groups[i].process;
			  ProcF(i, x1, y1, x2, y2, (int)((float)dlg->x*wsp_x), (int)((float)dlg->y*wsp_y));
		  }
	  }
  }
}

static int find_group(GROUP_BOX *Groups, int SizeGroupT)
/*------------------------------------------------------*/
{
	int i, ret = -1;
	int x1, y1, x2, y2;

	for (i = SizeGroupT-1; i >= 0; i--)  //reversed
	{
		x1 = jed_to_piks_x(Groups[i].x) + pocz_x;
		x2 = jed_to_piks_x(Groups[i].x + Groups[i].dx - 1) + pocz_x;
		y1 = jed_to_piks_y(Groups[i].y) + pocz_y;
		y2 = jed_to_piks_y(Groups[i].y + Groups[i].dy - 1) + pocz_y;
		if (PozX > x1 && PozX < x2 &&
			PozY > y1 && PozY < y2)
		{
			break;
		}
	}
	if (i >= 0)
	{
		cur_off(PozX, PozY);
		Draw_Group_Header(&(Groups[i]), 1);
		delay(200);
		Draw_Group_Header(&(Groups[i]), 0);
		delay(50);

		moveto(PozX + pocz_x, PozY + pocz_y);
		cur_on(PozX, PozY);
		ret = Groups[i].id;
	}
	return ret;
}

static void mvwindow (int dx, int dy)
{
    int w,h;

    w=x002[Typ]-x001[Typ];
    h=y002[Typ]-y001[Typ];

    if (!mouse_b)
    {

        Dlg->x=piks_x_to_jed(Dlg->xb);
        Dlg->y=piks_y_to_jed(Dlg->yb - 2 * ED_INF_HEIGHT);

        int d_xb = Dlg->xb - Dlg_xb;
        int d_yb = Dlg->yb - Dlg_yb;

        pocz_x+=d_xb;
        pocz_y+=d_yb;

        x001[Typ]+=d_xb;
        x002[Typ]+=d_xb;
        y001[Typ]+=d_yb;
        y002[Typ]+=d_yb;

        if (cur_tipsmenu!=NULL)
        {
            cur_tipsmenu->xpcz += d_xb;
            cur_tipsmenu->ypcz += d_yb;
            cur_tipsmenu=NULL;
        }

        MVCUR=CUR;

    }
    else
    {

        int pos = mouse_pos;
        dx_mov=(pos >> 16) - xmousex;
        dy_mov=(pos & 0x0000ffff) - ymousey;

        xmousex+=dx_mov;
        ymousey+=dy_mov;

        Dlg->xb+=dx_mov;
        Dlg->yb+=dy_mov;

        if (Dlg->xb<0)  Dlg->xb=0;
        if (Dlg->xb+(x002[Typ]-x001[Typ])>getmaxx())  Dlg->xb=getmaxx()-(x002[Typ]-x001[Typ]);
        if (Dlg->yb<yp)  Dlg->yb=yp;
        if (Dlg->yb+(y002[Typ]-y001[Typ])>getmaxy())  Dlg->yb=getmaxy()-(y002[Typ]-y001[Typ]);

		cur_off(PozX, PozY);

        if (Dlg->background!=NULL) flip_full_screen(Dlg->background);

        if (Dlg->back!=NULL) putkeepimage(Dlg->xb, Dlg->yb, Dlg->back, COPY_PUT);

		cur_on(PozX, PozY);
    }
}

static int find_header(TDIALOG *dlg, PTMENU *tipsmenu, int typ)
/*----------------------------------------------------------*/
{
    int ret = -1;
    int y2;
    int k = 0;
    int  rx1, ry1, rx2, ry2;

    int pocz_x_=pocz_x;
    int pocz_y_=pocz_y;

    Dlg=dlg;
    Typ=typ;

    y2 = y001[typ] + 7 + ED_INF_HEIGHT;
    if (PozX > x001[typ]+2 && PozX < x002[typ]-2 &&
        PozY > y001[typ]+3 && PozY < y2)
    {
        destroy_bitmap(dlg->back);
        dlg->back = create_bitmap_ex(32, x002[typ]-x001[typ], y002[typ]-y001[typ]);
        set_clip_rect(screen, 0, 0, getmaxx(), getmaxy());

		cur_off(PozX, PozY);
        acquire_screen();
        getimage(x001[typ], y001[typ], x002[typ], y002[typ], dlg->back);
        release_screen();
		cur_on(PozX, PozY);

        dx_mov=0;
        dy_mov=0;

        Dlg_xb=Dlg->xb;
        Dlg_yb=Dlg->yb;

        int pos = mouse_pos;
        xmousex = pos >> 16;
        ymousey = pos & 0x0000ffff;

        cur_tipsmenu=tipsmenu;

        CUR=MVCUR;
        MVCUR=mvwindow;
        ret = -3;
    }
    return ret;
}

static int find_input_line(INPUTLINE *InputLines,int SizeInLinT)
/*------------------------------------------------------------*/
{ char buf[256];
  int i, ink, paper, ink_ini, paper_ini ;
  int ret = 0;
  int x1, x2, y1, y2;
  int dx;
  int input_width;
  int x01, y01, x02, y02;
  int clip_state;

  
  cur_off(PozX, PozY);  
  for(i=0;i<SizeInLinT;i++)
  {
    if(InputLines [i].enable == FALSE)
    {
	continue;
    }
    x1 = jed_to_piks_x(InputLines[i].x)+pocz_x;
    x2 = jed_to_piks_x(InputLines[i].x + InputLines[i].dx - 1)+pocz_x;
    y1 = jed_to_piks_y(InputLines[i].y)+pocz_y;
    y2 = jed_to_piks_y(InputLines[i].y + InputLines[i].dy - 1)+pocz_y;
    if(PozX > x1 && PozX < x2 && PozY > y1 && PozY < y2)
    {
	break;
    }
  }
  if(i < SizeInLinT)
  {
    ink = InputLines[i].ink == COLOR_NULL ? dlg_kolory->dlg_in_line_ink : InputLines[i].ink;
    paper = InputLines[i].paper == COLOR_NULL ? 254 /*dlg_kolory->dlg_in_line_paper*/ : InputLines[i].paper ;
    ink_ini = InputLines[i].ink_ini == COLOR_NULL ? dlg_kolory->dlg_in_line_ink_ini : InputLines[i].ink_ini;
    paper_ini = InputLines[i].paper_ini == COLOR_NULL ? dlg_kolory->dlg_in_line_paper_ini : InputLines[i].paper_ini ;

	if (InputLines[i].MaxLen<10)
       //moveto( x1 + 0.9 * DXIL, y1 + (y2 - y1 - (HEIGHT - 3))/2 + 1);
       moveto( x1 + 0.9 * DXIL, y1 + (y2 - y1 - (HEIGHT - 3))/2 + 2);
	else
		//moveto(x1 + 2 * DXIL, y1 + (y2 - y1 - (HEIGHT - 3))/2 + 1);
        moveto(x1 + 2 * DXIL, y1 + (y2 - y1 - (HEIGHT - 3))/2 + 2);

    strcpy( buf, InputLines[i].txt);
 
    if(paper < 9) ink = 15;

	dx = (int)((float)InputLines[i].dx*wsp_x);

      dx = jed_to_piks_x (InputLines[i].dx);

    get_clip_rect(screenplay, &x01, &y01, &x02, &y02);
    set_clip_rect(screenplay,  x1+2, y1, x2-2, y2); //+2 and -2 to not overlap the frame
    clip_state=get_clip_state(screenplay);
    set_clip_state(screenplay, 1);

    if(read_dlg(buf, ink, paper, ink_ini, paper_ini,
	InputLines[i].MaxLen, InputLines[i].width, dx, InputLines[i].legal) != ESC)
    {
      strcpy(InputLines[i].txt, buf);
      ret = InputLines[i].id;
    }
    else
    { 
      setfillstyle_(SOLID_FILL, paper);
      bar(x1+1, y1+1, x2-1, y2-1);

	  if (InputLines[i].MaxLen < 10)
	  {
	  	  //input_width = ((x2 - x1) - 3 * DXIL - ttf_width_w) / ttf_width_w + extra1;
          input_width = ((x2 - x1) - ttf_width_w) / ttf_width_w  + extra1;
          //input_width=InputLines[i].width;
	  	  outetextxy(x1 + 0.9 * DXIL, y1 + (y2 - y1 - (HEIGHT - 3)) / 2 + 2, InputLines[i].MaxLen,
            input_width, InputLines[i].txt, ink, paper);
	  }
	  else
	  {
		  input_width = ((x2 - x1) - 4 * DXIL - ttf_width_w ) / ttf_width_w + extra1;
          //input_width=jed_to_piks_x(InputLines[i].width);
		  outetextxy(x1 + 2 * DXIL, y1 + (y2 - y1 - (HEIGHT - 3)) / 2 + 2, InputLines[i].MaxLen,
			  input_width, InputLines[i].txt, ink, paper);
	  }
	  
      ret = -1;
    }
      _free_mouse();
      set_clip_rect(screenplay,  x01, y01, x02, y02);
      set_clip_state(screenplay, clip_state);
  }
  moveto(PozX+pocz_x, PozY+pocz_y);
  cur_on(PozX, PozY); 

  return ret;
}


void Draw_Input_Line(INPUTLINE *InputLine)
/*----------------------------------------*/
{
  int ink, paper, border;
  int x1, y1, x2, y2;
  int input_width;
    int x01, y01, x02, y02;
    int clip_state;

  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
  setwritemode(COPY_PUT);
  paper = InputLine->paper == COLOR_NULL? 254 : InputLine->paper ;
  ink = InputLine->ink == COLOR_NULL? dlg_kolory->dlg_in_line_ink : InputLine->ink ;
  if (InputLine->enable==0) ink=LIGHTGRAY;
  border = InputLine->border == COLOR_NULL? dlg_kolory->dlg_in_line_border : InputLine->border;
  x1 = jed_to_piks_x(InputLine->x)+pocz_x;
  x2 = jed_to_piks_x(InputLine->x + InputLine->dx - 1)+pocz_x;
  y1 = jed_to_piks_y(InputLine->y)+pocz_y;
  y2 = jed_to_piks_y(InputLine->y + InputLine->dy - 1)+pocz_y;

    get_clip_rect(screenplay, &x01, &y01, &x02, &y02);
    set_clip_rect(screenplay,  x1+2, y1, x2-2, y2); //+2 and -2 to not overlap the frame
    clip_state=get_clip_state(screenplay);
    set_clip_state(screenplay, 1);
  
  setwritemode(COPY_PUT);
  
  setcolor(15);
  LINE(x1,y1,x2,y1);
  LINE(x1,y1,x1,y2);
  
  
  setcolor(border);
  LINE(x1,y2,x2,y2);
  LINE(x2,y2,x2,y1);
  
  setfillstyle_(SOLID_FILL, paper);
  bar(x1+1, y1+1, x2-1, y2-1);

  if(paper < 9 ) ink = 15;
  if (InputLine->enable==1) setcolor(ink);
    else setcolor(LIGHTGRAY);

  if (InputLine->MaxLen < 10)  //10
  {
	  //input_width = InputLine->MaxLen;
      input_width = ((x2 - x1) - ttf_width_w) / ttf_width_w  + extra1;
	  outetextxy(x1 + 0.9 * DXIL, y1 + (y2 - y1 - (HEIGHT - 3)) / 2 + 2, InputLine->MaxLen, input_width, InputLine->txt, ink, paper); //254
  }
  else
  {
      input_width = ((x2 - x1) - 4 * DXIL - ttf_width_w) / ttf_width_w + extra1;
      outetextxy(x1 + 2 * DXIL, y1 + (y2 - y1 - (HEIGHT - 3)) / 2 + 2, InputLine->MaxLen, input_width, InputLine->txt,ink, paper); //254
  }

    set_clip_rect(screenplay,  x01, y01, x02, y02);
    set_clip_state(screenplay, clip_state);

}

static void draw_input_lines(INPUTLINE *InputLines,int SizeInLinT)
/*--------------------------------------------------------------*/
{
  int i;

  for(i=0;i<SizeInLinT;i++)
  {
    Draw_Input_Line(&InputLines[i]);
  }
}

void draw_input_line(INPUTLINE *InputLine)
/*--------------------------------------------------------------*/
{
    Draw_Input_Line(InputLine);
}

void set_proper_color(int kolor_m_)
{
	if (kolor_m_ == 15)
		setcolor(0);
	else if (kolor_m_ == 8)
		setcolor(15);
	else if (kolor_m_ == 16)
		setcolor(15);
	else if (kolor_m_ > 0)
		setcolor(0); //15
	else setcolor(0);
}

/*-------------------------------------------*/
/*-------------------------------------------*/
/*		COMBOBOX		     */
/*-------------------------------------------*/
/*-------------------------------------------*/

static int
get_bar_image_size(LISTBOX  * listbox)
/*---------------------------------------*/
{
  int x1,y1,x2,y2;

  x1 = jed_to_piks_x(listbox->x) + 1+pocz_x;
  x2 = jed_to_piks_x(listbox->x + listbox->dx) - 2+pocz_x;
  y1 = jed_to_piks_y(listbox->y + listbox->dy * listbox->poz) + 1+pocz_y;
  y2 = jed_to_piks_y(listbox->y + listbox->dy *(listbox->poz + 1)) - 1+pocz_y;
  return imagesize(x1, y1 - 2, x2, y2 + 2) ;
}

static void baronoff(LISTBOX  * listbox)
/*-----------------------------------------------*/
{ int x1,y1,x2,y2;
  int bb;
  char *ptr, *ptr1;
  int kolor_m;
  char *numer_koloru_str;
  int numer_koloru;
  int color_bar=FALSE;

  if (BAR_POINTER) show_mouse(NULL);

  x1 = jed_to_piks_x(listbox->x) + 1 +pocz_x;
  x2 = jed_to_piks_x(listbox->x + listbox->dx) - 2+pocz_x;
  y1 = jed_to_piks_y((listbox->y + listbox->dy * listbox->poz) /*+ 1*/)+pocz_y;
  y2 = jed_to_piks_y((listbox->y + listbox->dy *(listbox->poz + 1)) - 1)+pocz_y;

      ptr=strstr(listbox->txt[listbox->foff + listbox->poz],u8"֍");
      if(ptr!=NULL)
       {
          color_bar=TRUE;
		  ptr1 = strstr(listbox->txt[listbox->foff + listbox->poz], _OTHER_);
		  
          if(ptr1!=NULL) 
          {
			numer_koloru_str = ptr1 + 10;
			if (strlen(numer_koloru_str) == 0) kolor_m = 15;
			else
			{
				while (numer_koloru_str[0] == '\040') numer_koloru_str += 1;
				numer_koloru = atoi(numer_koloru_str);
				kolor_m = GetColorAC1(numer_koloru);
			}
          }
          else
          {
           kolor_m=GetColorAC1(listbox->foff + listbox->poz + 1);
           if (kolor_m==0) kolor_m=15;
          }
         setcolor(kolor_m);
       } 
        else setcolor(15);
        
  ////setwritemode(XOR_PUT);
  ////setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
  if (color_bar)
  {
      setwritemode(XOR_PUT);
      setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
      for (int i = 0; i < 3; i++)
      {
          LINE(x1 + i, y1 + i, x2 - i, y1 + i);
          LINE(x2 - i, y1 + i, x2 - i, y2 - i);
          LINE(x2 - i, y2 - i, x1 + i, y2 - i);
          LINE(x1 + i, y2 - i, x1 + i, y1 + i);
      }
  }
  else {
      if (XORBAR)
      {
          setwritemode(XOR_PUT);
          setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
          for (bb = y1; bb <= y2; bb++) {
              LINE(x1, bb, x2, bb);
          }
      }
      else
      {
          setwritemode(COPY_PUT);
          BITMAP *barbitmap;
          barbitmap = create_bitmap(x2-x1, y2-y1);
          getimage(x1, y1, x2, y2, barbitmap);
          invert_bitmap(barbitmap);
          putimage(x1, y1, barbitmap, COPY_PUT);
      }
  }

  if (BAR_POINTER) show_mouse(screen);

  if (!BAR_POINTER)
  {
      PozY = (y1 + y2) / 2;
      moveto(PozX + pocz_x, PozY + pocz_y);
      MVCUR(0, 0);
  }

    bar_center.x=(x1+x2)/2;
    bar_center.y=(y1+y2)/2;
   
}


static void draw_listbox(LISTBOX  * listbox)
/*--------------------------------------*/
{
  int x01, y01, x02, y02, x1, y1, x2, y2, xt, yt, i, size, height, ink, border, paper, w, h;
  unsigned char def_paper, def_ink, def_border;
  char temp [MaxTextLen], *ptr_temp ;

  char sk[MaxTextLen + 1];
  char sk1[MaxTextLen + 1];

  int kolor_m, kolor_rgb;
  char *ptr, *ptr1;
  char *numer_koloru_str;
  int numer_koloru;
  double listbox_max_text_len_;
  int listbox_max_text_len, typ_l, typ_f;
  int ret;
  TEXT fontn= Tdef;
  COLOR_ kolor;
  BOOL draw_poz;
  int color_bak;
  BITMAP *list_screen;

  GrMouseEraseCursor();

  listbox->maxw=listbox->maxw0; //inicjacja

  def_paper =(listbox->flags & FLB_LIST) ? dlg_kolory->dlg_listbox_paper : dlg_kolory->dlg_list_combo_paper;
  def_ink =(listbox->flags & FLB_LIST) ? dlg_kolory->dlg_listbox_ink : dlg_kolory->dlg_list_combo_ink;
  def_border =(listbox->flags & FLB_LIST) ? dlg_kolory->dlg_listbox_border : dlg_kolory->dlg_list_combo_border;
  paper = listbox->paper == COLOR_NULL ? def_paper : listbox->paper ;
  ink = listbox->ink == COLOR_NULL ? def_ink : listbox->ink ;
  border = listbox->border == COLOR_NULL ? def_border : listbox->border;
  size =((listbox->maxw == 0) ? listbox->max : listbox->maxw);

  if(listbox->max < listbox->maxw)
  {
      size = listbox->max;
  }

  x01 = jed_to_piks_x(listbox->x)+pocz_x;
  x02 = jed_to_piks_x(listbox->x + listbox->dx) /*- 1*/ +pocz_x;
  y01 = jed_to_piks_y(listbox->y)+pocz_y-2;
  y02 = jed_to_piks_y(listbox->y + listbox->dy * size) - 1+pocz_y + 2;

  while (y02>getmaxy())
  {
      size--;
      y02 = jed_to_piks_y(listbox->y + listbox->dy * size) - 1+pocz_y + 2;
  }

  listbox->maxw=size;

  w=x02-x01;
  h=y02-y01;

  list_screen= create_bitmap_ex(32, w, h);
  if (list_screen==NULL) return;
  Set_Screenplay(list_screen);

  set_clip_rect(list_screen, 0, 0, x02, y02);

  x1=0;
  x2=x1+w;
  y1=2;
  y2=y1+h-4;

  height = jed_to_piks_y(listbox->dy);
  setfillstyle_(SOLID_FILL,paper);
  bar(x1,y1 - 2,x2,y2 + 2);
  setcolor(border);
  setwritemode(COPY_PUT);
  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
  LINE(x1,y1-2,x2,y1-2);
  LINE(x1,y1-2,x1,y2+2);
  setcolor(15);
  LINE(x1,y2+2,x2,y2+2);
  LINE(x2,y1-2,x2,y2+2);
  
  setcolor(ink);
  
  xt=x1+3;
  //if(listbox->max < listbox->maxw)
  //{
  //  size = listbox->max;
  //}

  listbox_max_text_len_ = listbox->max_text_len * getmaxx() * 8;
  listbox_max_text_len_ /= 638;
  listbox_max_text_len_ /= WIDTH;
  listbox_max_text_len_ += 0.5;
  listbox_max_text_len =(int) listbox_max_text_len_;

  if (listbox->max < listbox->foff)  listbox->foff=0;  //to dla zabezpieczenia przed crashem

  for(i=0;i<size;i++)
  {
    //yt = jed_to_piks_y(listbox->y + listbox->dy * i) +pocz_y +(height - 8) / 2 ;
    yt = jed_to_piks_y(listbox->dy * i) +(height - 8) / 2 ;
    ptr_temp = listbox->txt[listbox->foff+i];
    if(strlen(ptr_temp) > listbox_max_text_len)
    {
      strcpy(temp, ptr_temp) ;
      temp [listbox_max_text_len] = '\0';
      ptr_temp = temp ;
    }
     /**/
      ptr=strstr(ptr_temp,u8"֍");  //ASCII 219
      if(ptr!=NULL)
       { 
		  ptr1 = strstr(listbox->txt[listbox->foff + i], _OTHER_);
		 
		 if (ptr1 != NULL)
		 {
			strcpy(sk, ptr_temp + 2);
			if (listbox->wartosc > 16)
			{
				sprintf(sk1, " %d", listbox->wartosc);
				strncat(sk, sk1, strlen(sk1));
				kolor_m = GetColorAC1(listbox->wartosc);
			}
			   else kolor_m = 15;

         }
         else
         {
          kolor_m=GetColorAC1(listbox->foff+i+1);
		  strcpy(sk, ptr_temp + 2);
         }
        setfillstyle_(SOLID_FILL,kolor_m);
        bar(x1+1, yt -(height / 2) + 6, x2-1, yt +(height / 2) + 3); ///!!!!!!!!!!!  
        if(kolor_m > 16)
         {
          kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
          if(kolor_rgb < 96) setcolor(15); else setcolor(0);
         }
		set_proper_color(kolor_m);

       }
       else
        {

          setcolor(ink);    
		  strcpy(sk, ptr_temp);
        }

	draw_poz = TRUE;
    ptr=strstr(ptr_temp, u8"֏");  //ASCII 218
	if(ptr!=NULL)
	{

		typ_l=listbox->foff+i;

		linestyle(typ_l+64);
	    ret=lineCuncut (x1+1,yt+(height / 2) , x2-1, yt+(height / 2) );

		strcpy(sk, ptr_temp+2);
	}
	else
	{
	    ptr=strstr(ptr_temp, u8"҂");  //ASCII 217
		if(ptr!=NULL)
		{

			typ_l=listbox->foff+i;
			if (typ_l < 5)
			{

				linestyle(typ_l * 32);
				ret = lineCuncut(x1 + 1, yt + (height / 2), x2 - 1, yt + (height / 2));
			}

			strcpy(sk, ptr_temp+2);
		}
		else if (listbox->id==ID_CFontName)
			{
			   typ_f = listbox->foff + i;

			   moveto(xt, yt - 2);

			   if (typ_f < i__font_nomax)
			   {
				   if ((PTRS__Text_Style[typ_f]->type < 2) || (PTRS__Text_Style[typ_f]->type == 3))
					   draw_font_name(ptr_temp, typ_f, xt + 5, yt + HEIGHT / 2 + 4);
				   else
				   {
					   kolor.red = 0;
					   kolor.gre = 0;
					   kolor.blu = 100;
					   strcpy(fontn.text, ptr_temp);
					   fontn.dl = strlen(fontn.text);
					   fontn.n = T18 + fontn.dl;
					   fontn.czcionka = typ_f;
					   fontn.kat = 0.0;
					   fontn.bold = 0;
					   fontn.width_factor = 1;
					   fontn.italics = 0;
					   fontn.justowanie = j_do_lewej;
					   fontn.multiline = 0;
					   fontn.ukryty = 0;
					   fontn.wysokosc = 4;
					   fontn.kolor = 0;
					   fontn.width = 0;
					   fontn.height = 0;
					   fontn.warstwa = Current_Layer;
					   fontn.widoczny = 1;

					   draw_font_name_ttf(&fontn, &fontn.text, list_screen, xt + 5, yt + HEIGHT / 2 + 4, x2, fontn.kat, vfv(20.0), kolor, COPY_PUT, 0, 0);

				   }
			   }
			   else
			   {
				   color_bak = getcolor();
				   setcolor(184);
				   TTF_text_UI_(list_screen, ptr_temp, xt + 5, yt-2, NULL, NULL);
				   setcolor(color_bak);
			   }
			   draw_poz = FALSE;
			}
	}
	
      /**/
	if (draw_poz)
	{
        moveto(xt, yt + 7 - (HEIGHT/2));
		int pos = findfpostopxl(sk, x2 - x1 - 3);
		outtext_r(&sk[pos]);
	}

  }


    blit(list_screen, screen, 0,0,x01,y01,w,h);

    Set_Screenplay(screen);

    destroy_bitmap(list_screen);


}


static int listbox_init_slider(int *var1, int *var2, int *var3, int *var4)
{
    LISTBOX *listbox;
    listbox=(LISTBOX *)listbox_address;

    *var1=listbox->foff;  //  n_first_layer_in_dlg;
    *var2=listbox->foff+listbox->maxw; //  n_last_layer_in_dlg+1;
    //*var3=No_Layers;
    *var3=listbox->max;  // no_layers;
    *var4=listbox->maxw;  //NoDialogLayers;
    return 1;
}

static int listbox_grab_slider(void *dp3, int d2)
{   char slider_var[32];
    int var1, var2, var3, var4, ret;
    int (*SlideFun)(int*, int*, int*, int*);

    LISTBOX *listbox;
    listbox=(LISTBOX *)listbox_address;

    SlideFun = (int(*)(int *, int *, int *, int *))dp3;

    ret = SlideFun(&var1, &var2, &var3, &var4);
    //sprintf(slider_var, "%d-%d", max(0, d2-NoDialogLayers), d2);
    ////sprintf(slider_var, "%d-%d", var3 - d2 - (var2-var1) + 1, var3-d2);
    ////komunikat_str_short(slider_var, FALSE);


    //get_dlg_controls_to_layers ();
    listbox->foff=var3 - d2 - (var2-var1);   //n_first_layer_in_dlg
    //tmenu->foff=var3-d2 - 1 - tmenu->maxw;    //n_last_layer_in_dlg
    //init_layer_dlg_control (FALSE);

    //select_mouse_cursor(MOUSE_CURSOR_NONE);
    //show_mouse(NULL);
    ////Redraw_Dlg(&layers_dlg);
    draw_listbox(listbox);
    //select_mouse_cursor(MOUSE_CURSOR_ALLEGRO);
    //show_mouse(screen);
    return 0;
}


static int find_listbox_slider(LISTBOX *listbox, SLIDER *slider)
/*------------------------------------------------------------*/
{ int i, ret=0, ret1;
    int size, x1, y1, x11, x12, y11, y12, dx, dy; //x1, y1, x2, y2,
    int poz1;
    int mouse_yy;

    SLIDER Slider_;

    memmove(&Slider_, slider, sizeof(SLIDER));

    Slider_.fg = palette_color[Slider_.fg /*kolory.inkm*/];
    Slider_.bg = palette_color[Slider_.bg /*kolory.paperk*/];

    gui_set_screen(screen);

    if (slider->flags & 0xF0) return ret;

    mouse_yy=mouse_y;

    if(mouse_x > slider->x && mouse_x < (slider->x + slider->w) &&
       mouse_y > slider->y && mouse_y < (slider->y + slider->h))
    {
        //set_scrsave_time();
        //show_hide_tip(menu, FALSE);
        baronoff(listbox);
        ret1 = Slider_.proc(MSG_CLICK, &Slider_, '\0');
        slider->d2 = Slider_.d2;
        //baronoff(menu);
        size=listbox->maxw?listbox->maxw:listbox->max;

        x1=listbox->x;
        y1=listbox->y;

        x1 = jed_to_piks_x(listbox->x)+pocz_x;
        y1 = jed_to_piks_y(listbox->y)+pocz_y;

        dy=jed_to_piks_y(listbox->dy);

        for (i = 0; i < size; i++) {
            y11 = y1 + i * dy;
            y12 = y11 + dy;
            //mouse_yy = mouse_y;
            if ((mouse_y >= y11) && (mouse_y <= y12)) break;
        }
        if (i < size) {
            poz1 = i;
            if (poz1 != listbox->poz) {
                //set_scrsave_time();
                //show_hide_tip(menu, FALSE);
                //baronoff(menu);
                listbox->poz = poz1;
                //baron(listbox);
            }
        }
        baron(listbox);
        ret=1;
    }

    return ret;
}


void draw_listbox_slider(SLIDER *slider)
{
    SLIDER Slider_;
    int var1, var2, var3, var4, ret;
    BITMAP *slbitmap;

    gui_fg_color = palette_color[kolory.inkm];
    //gui_mg_color = makecol(128, 128, 128);
    gui_mg_color = palette_color[180 /*kolory.inkk*/];
    //gui_bg_color = makecol(200, 240, 200);
    //gui_bg_color = makecol(200, 200, 200);
    gui_bg_color = palette_color[98 /*kolory.paperk*/];

    gui_border_dark = palette_color[8];
    gui_border_light = palette_color[15];

    slider->flags &= ~0x800;

    int (*SlideFun)(int *, int *, int *, int *);
    SlideFun = (int (*)(int *, int *, int *, int *)) slider->dp3;

    ret = SlideFun(&var1, &var2, &var3, &var4);

    //adjusting slider
    slider->d1 = var3 - (var2 - var1);

    int Slider_h = slider->h;
    double slratio = (double) Slider_h / (double) (var3);
    int slbody;
    if (var3 <= var4) slbody = Slider_h;
    else slbody = (var2 - var1) * slratio;

    if (slider->dp != NULL)
    {
        destroy_bitmap((BITMAP *) slider->dp);
        slider->dp = NULL;
    }

    if (slbody<4) slider->flags |= 0xF0;  //hidden
    else
    {
        slider->flags &= ~0xF0;
        slbitmap = create_bitmap(slider->w, slbody);
        slider->d2 = var3 - var1 - (var2 - var1);

        clear_to_color(slbitmap, get_palette_color(246/*kolory.paperm*/));
        hline(slbitmap, 1, 1, slbitmap->w - 2, BLACK);
        vline(slbitmap, slbitmap->w - 2, 1, slbitmap->h - 1, BLACK);
        hline(slbitmap, slbitmap->w - 2, slbitmap->h - 1, 1, BLACK);
        vline(slbitmap, 1, 1, slbitmap->h - 1, BLACK);

        hline(slbitmap, 2, 2, slbitmap->w - 3, palette_color[15]);
        vline(slbitmap, slbitmap->w - 3, 2, slbitmap->h - 2, palette_color[8]);
        hline(slbitmap, slbitmap->w - 3, slbitmap->h - 2, 2, palette_color[8]);
        vline(slbitmap, 2, 2, slbitmap->h - 2, palette_color[15]);

        slider->dp = slbitmap;
    }

    memmove(&Slider_, slider, sizeof(SLIDER));

    Slider_.flags=0;
    Slider_.fg = palette_color[Slider_.fg /*kolory.inkm*/];
    Slider_.bg = palette_color[Slider_.bg /*kolory.paperk*/];

    //adopt dimensions
    //Slider_.x = jed_to_piks_x(Slider->x) + pocz_x;
    //Slider_.y = jed_to_piks_y(Slider->y) + pocz_y;
    //Slider_.w = jed_to_piks_x(Slider->w);
    //Slider_.h = jed_to_piks_y(Slider->h);
    Slider_.slider = slider;

    ret = slider->proc(MSG_DRAW, &Slider_, '\0');
}

void redraw_listbox_slider(SLIDER *slider)
{
    SLIDER Slider_;
    int var1, var2, var3, var4, ret;

    gui_fg_color = palette_color[kolory.inkm];
    gui_mg_color = palette_color[180];
    gui_bg_color = palette_color[98];

    gui_border_dark = palette_color[8];
    gui_border_light = palette_color[15];

    slider->flags &= ~0x800;

    int (*SlideFun)(int *, int *, int *, int *);
    SlideFun = (int (*)(int *, int *, int *, int *)) slider->dp3;

    ret = SlideFun(&var1, &var2, &var3, &var4);

    //adjusting slider
    slider->d1 = var3 - (var2 - var1);
    slider->d2 = var3 - var1 - (var2 - var1);

    memmove(&Slider_, slider, sizeof(SLIDER));

    Slider_.flags=0;
    Slider_.fg = palette_color[slider->fg];
    Slider_.bg = palette_color[slider->bg];

    Slider_.slider = slider;

    ret = slider->proc(MSG_DRAW, &Slider_, '\0');
}


 /*wyswietlenie okna wraz z trescia*/
static int open_listbox(LISTBOX  * listbox)
/*---------------------------------------*/
{

  BITMAP *listbox_back;
  int x1, y1, x2, y2, size;
  int i_image_no ;

  listbox->maxw=listbox->maxw0;  //initiasion

  size =((listbox->maxw == 0) ? listbox->max : listbox->maxw);
  x1 = jed_to_piks_x(listbox->x)+pocz_x;
  x2 = jed_to_piks_x(listbox->x + listbox->dx) - 1+pocz_x;
  y1 = jed_to_piks_y(listbox->y)+pocz_y;
  y2 = jed_to_piks_y(listbox->y + listbox->dy * size) - 1+pocz_y;

  if(listbox->max < listbox->maxw)
  {
      size = listbox->max;
  }

  while (y2>getmaxy())
  {
      size--;
      y2 = jed_to_piks_y(listbox->y + listbox->dy * size) - 1+pocz_y + 2;
  }

  listbox->maxw=size;

  set_listbox_slider=0;
  if ((BAR_POINTER) && (listbox->maxw>0) && (listbox->maxw<listbox->max))
  {
      x2+=jed_to_piks_y(12);
      set_listbox_slider=1;
      listbox_address=listbox;
  }

  if(!(listbox->flags & FLB_LIST))
  {
     listbox->back = NULL ;
     if(get_free_mem_size(0) <(long)imagesize(x1, y1 - 2, x2, y2 + 2) +
                             (long)get_bar_image_size(listbox) + 100)
     {
       listbox->flags |= FLB_BACK_MEM ;
       i_image_no = Get_Image_Tmp(x1, y1 - 2, x2, y2 + 2) ;
       if(i_image_no == -1)
       {
         return 0 ;
      }
       listbox->back =(void *)(i_image_no + 1) ;
     }
     else
     {

       if ((listbox_back=create_bitmap(x2-x1+1,y2-y1+1+4))==NULL) return 0;
       listbox->back=listbox_back;
       getimage(x1, y1 - 2, x2, y2 + 2,listbox->back);
     }
  }
  draw_listbox(listbox);

  if (set_listbox_slider)
    {

        listbox_slider.x=x2-jed_to_piks_y(10);
        listbox_slider.y=y1;
        listbox_slider.h=y2-y1;
        listbox_slider.w=jed_to_piks_y(8);
        listbox_slider.slider=&listbox_slider;
        gui_set_screen(screen);
        draw_listbox_slider(&listbox_slider);
    }

  baron(listbox);
  return 1;
}

/*zgaszenie okna z odtworzeniem przeslanianego ekranu*/
static void  close_listbox(LISTBOX  * listbox)
/*-------------------------------------------*/
{
  int x1,y1;
  int i_image_no ;

  if (BAR_POINTER) show_mouse(NULL);

  if((!(listbox->flags & FLB_LIST)) && listbox->back)
  {
    x1 = jed_to_piks_x(listbox->x)+pocz_x;
    y1 = jed_to_piks_y(listbox->y)+pocz_y;
    if(listbox->flags & FLB_BACK_MEM)
    {
      i_image_no =(int)listbox->back - 1 ;
      Put_Image_Tmp(i_image_no, x1, y1 - 2) ;
    }
    else
    {
      putimage( x1, y1 - 2, listbox->back, COPY_PUT) ;
    }
    listbox->back = NULL;
  }
  if ((BAR_POINTER) && (set_listbox_slider))
  {
      if (listbox_slider.dp!=NULL)
      {
          destroy_bitmap((BITMAP *) listbox_slider.dp);
          listbox_slider.dp = NULL;
          set_listbox_slider=0;
      }
  }

  if (BAR_POINTER) show_mouse(screen);
}

static void shift_cursor(LISTBOX  * listbox)
{  float yd, xr;
    int x1,y1,x2,y2,w,h;
    int size;
    int mouse_x_, mouse_y_;
    SLIDER *Slider;

    size=listbox->maxw?listbox->maxw:listbox->max;

    x1 = jed_to_piks_x(listbox->x)+pocz_x;
    x2 = jed_to_piks_x(listbox->x + listbox->dx) /*- 1*/ +pocz_x;
    y1 = jed_to_piks_y(listbox->y)+pocz_y-2;
    y2 = jed_to_piks_y(listbox->y + listbox->dy * size) - 1+pocz_y + 2;

    mouse_x_=mouse_x;
    mouse_y_=mouse_y;
    if ((mouse_x >= x1) && (mouse_x<= x2))
    {
        if ((mouse_y >= y1) && (mouse_y <= y2))
        {
#ifdef ALLEGRO5
#ifdef MACOS
        	MoveOSXCursor(mouse_x_, bar_center.y);
#else
        	position_mouse(mouse_x_, bar_center.y);
#endif
#else
        	position_mouse(mouse_x_, bar_center.y);
#endif
        	set_forget_mouse(mouse_x_, bar_center.y);
        }
    }
    //if (listbox->max > listbox->maxw)
    if (set_listbox_slider)
    {
        Slider=&listbox_slider;
        gui_set_screen(screen);
        setwritemode(COPY_PUT);
        redraw_listbox_slider(Slider);
    }

}


/*funkcja przesuwania kursora o 1 poz. w dol listbox*/
static int  cnext(LISTBOX *listbox)
{
   if(listbox->poz+listbox->foff == listbox->max-1) return 0;
   baronoff(listbox);
   listbox->poz++;
   if(listbox->maxw!=0)
	{
	   if(listbox->poz+listbox->foff >= listbox->max) listbox->poz--;
	   else
		 if(listbox->poz >= listbox->maxw)
		   {
		listbox->foff++;
			listbox->poz--;
		draw_listbox(listbox);
		GrMouseEraseCursor();
		   }
	 }
   baron(listbox);
    if (BAR_POINTER) shift_cursor(listbox);
   return 0;
}

/*funkcja przesuwania kursora o 1 poz. w gore listbox*/
static int  cprev(LISTBOX *listbox)
{
   if(listbox->poz == 0 && listbox->foff==0 ) return 0;
   baronoff(listbox);
   listbox->poz--;
   if(listbox->maxw!=0)
       if(listbox->poz < 0)
	 {
	   if(listbox->foff > 0)
	     {
	       listbox->foff--;
	       draw_listbox(listbox);
		   GrMouseEraseCursor();
	     }
	   listbox->poz=0;
	 }
   baron(listbox);
    if (BAR_POINTER) shift_cursor(listbox);
   return 0;
 }
 

static int  cpgnext(LISTBOX *listbox)
{
 if(listbox->maxw==0) return 0;
 baronoff(listbox);
 if(listbox->foff+listbox->maxw < listbox->max)
   {
    /* nastapi przerysowanie */
     listbox->foff += listbox->maxw;
     if(listbox->foff + listbox->maxw > listbox->max)
       {
	 listbox->foff = listbox->max - listbox->maxw;
	 listbox->poz  = listbox->maxw - 1;
       }
     draw_listbox(listbox);
	 GrMouseEraseCursor();
   }
 else
   {
	 listbox->poz = listbox->maxw - 1;
     if(listbox->poz >= listbox->max) listbox->poz=listbox->max?listbox->max-1 : 0;
   }
 baron(listbox);
 if (BAR_POINTER) shift_cursor(listbox);
 return 0;
}

int  cpgnext1(LISTBOX *listbox)
{
 if(listbox->maxw==0) return 0;
 if(listbox->foff+listbox->maxw < listbox->max)
   {
    /* nastapi przerysowanie */
     listbox->foff += listbox->maxw;
     if(listbox->foff + listbox->maxw > listbox->max)
       {
	 listbox->foff = listbox->max - listbox->maxw;
	 listbox->poz  = listbox->maxw - 1;
       }
     draw_listbox(listbox);
	 GrMouseEraseCursor();
   }
 else
   {
	 listbox->poz = listbox->maxw - 1;
     if(listbox->poz >= listbox->max) listbox->poz=listbox->max?listbox->max-1 : 0;
   }
 return 0;
}

static int  cpgprev(LISTBOX *listbox)
{
 if(listbox->maxw==0) return 0;
 baronoff(listbox);
 if(listbox->foff == 0) listbox->poz=0;
 else
   {
     listbox->foff -= listbox->maxw;
     if(listbox->foff <0)
       {
	 listbox->foff = 0;
	 listbox->poz  = 0;
       }
	 draw_listbox(listbox);
	 GrMouseEraseCursor();
   }
 baron(listbox);
 if (BAR_POINTER) shift_cursor(listbox);
 return 0;
}

int  cpgprev1(LISTBOX *listbox)
{
 if(listbox->maxw==0) return 0;
 if(listbox->foff == 0) listbox->poz=0;
 else
   {
     listbox->foff -= listbox->maxw;
     if(listbox->foff <0)
       {
	 listbox->foff = 0;
	 listbox->poz  = 0;
       }
	 draw_listbox(listbox);
	 GrMouseEraseCursor();
   }
 return 0;
}


static int  cbegin(LISTBOX * listbox)
{
  baronoff(listbox);
  listbox->poz=0;
  if(listbox->foff)
    {
      listbox->foff=0;
      draw_listbox(listbox);
	  GrMouseEraseCursor();
    }
  baron(listbox);
  if (BAR_POINTER) shift_cursor(listbox);
  return 0;
}

int  cbegin1(LISTBOX * listbox)
{
  listbox->poz=0;
  if(listbox->foff)
    {
      listbox->foff=0;
      draw_listbox(listbox);
	  GrMouseEraseCursor();
    }
  return 0;
}

static int  cend(LISTBOX * listbox)
{
  baronoff(listbox);
  if(listbox->maxw==0) listbox->poz=listbox->max - 1;
  else
    {
      listbox->foff=listbox->max - listbox->maxw;
      if(listbox->foff < 0)
	 {
	   listbox->foff=0;
	   listbox->poz=listbox->max-1;
	 }
      else listbox->poz=listbox->maxw - 1;
      draw_listbox(listbox);
	  GrMouseEraseCursor();
    }
  baron(listbox);
  if (BAR_POINTER) shift_cursor(listbox);
  return 0;
}

int  cend1(LISTBOX * listbox)
{
  if(listbox->maxw==0) listbox->poz=listbox->max - 1;
  else
    {
      listbox->foff=listbox->max - listbox->maxw;
      if(listbox->foff < 0)
	 {
	   listbox->foff=0;
	   listbox->poz=listbox->max-1;
	 }
      else listbox->poz=listbox->maxw - 1;
      draw_listbox(listbox);
	  GrMouseEraseCursor();
    }
  return 0;
}

static void  pusk(int x,int y)
{
    int i,x1,y1,x2,y2,size,xr,yd, poz1, dx, dy, x11, x12, y11, y12, w, h;
    int mouse_xx, mouse_yy;
    LISTBOX *listbox;
    listbox=listbox_usk;

    size=listbox->maxw?listbox->maxw:listbox->max;
    x1 = jed_to_piks_x(listbox->x)+pocz_x;
    x2 = jed_to_piks_x(listbox->x + listbox->dx) /*- 1*/ +pocz_x;
    y1 = jed_to_piks_y(listbox->y)+pocz_y-2;
    y2 = jed_to_piks_y(listbox->y + listbox->dy * size) - 1+pocz_y + 2;

    w=x2-x1;
    h=y2-y1;

    dy=jed_to_piks_y(listbox->dy);

    if ((mouse_x >= x1) && (mouse_x<=x2))
    {
        if ((mouse_y >= y1) && (mouse_y<=y2))
        {
            for (i = 0; i < size; i++)
            {
                y11 = y1 + i * dy;
                y12 = y11 + dy;
                mouse_yy = mouse_y;
                if ((mouse_y >= y11) && (mouse_y <= y12)) break;
            }
            if (i < size)
            {
                poz1 = i;
                if (poz1 != listbox->poz)
                {
                    baronoff(listbox);
                    listbox->poz = poz1;
                    baron(listbox);
                }
            }
        }
    }
}

static void  usk(int x, int y)
/*---------------------------------*/
{
  static int dyy=0;
  x = x;

  dyy += y;
  if( dyy > SKOKM )
  {
    dyy = 0;
    cprev(listbox_usk);
  }
  if( dyy <- SKOKM )
  {
    dyy = 0;
    cnext(listbox_usk);
  }
}

static void init_lbox(char typ)
/*--------------------------*/
{ static int( *SW1[8])();
  static void(*mv)(int x,int y);
  static int pozx, pozy;
  static int global_menu_flags;
  switch(typ)
   { case 0 :
	  pozx = PozX;
	  pozy = PozY;
	  global_menu_flags = Get_Global_Menu_Flags();
	  Set_Global_Menu_Flags(NOMENU);
	  SW1[0]=SERV[72];   SERV[72]= (int (*)(void)) cprev;
	  SW1[1]=SERV[80];   SERV[80]= (int (*)(void)) cnext;
	  SW1[2]=SERV[71];   SERV[71]= (int (*)(void)) cbegin;
	  SW1[3]=SERV[73];   SERV[73]= (int (*)(void)) cpgprev;
	  SW1[4]=SERV[79];   SERV[79]= (int (*)(void)) cend;
	  SW1[5]=SERV[81];   SERV[81]= (int (*)(void)) cpgnext;

      SW1[6]=SERV[135];
      SW1[7]=SERV[134];

           SERV[135] = (void*)cnext;
           SERV[134] = (void*)cprev;

	  mv=MVCUR;

           if (!BAR_POINTER) MVCUR=usk;
           else MVCUR=pusk;

	  break;
	 case 1 :
	  Set_Global_Menu_Flags(global_menu_flags);
	  SERV[72] = SW1[0];
	  SERV[80] = SW1[1];
	  SERV[71] = SW1[2];
	  SERV[73] = SW1[3];
	  SERV[79] = SW1[4];
	  SERV[81] = SW1[5];

           SERV[134] = SW1[6];
           SERV[135] = SW1[7];
	  MVCUR=mv;
	  PozX = pozx;
	  PozY = pozy;
	  break;
   }
}

static BOOL edit_combo_box(COMBOBOX *ComboBox)
/*-------------------------------------------*/
{
  LISTBOX *listbox;
  EVENT *ev;
  int poz_xx, poz_yy;
  int ret;
  static int( *SW2[2])();

  if (ComboBox->listbox->max==0) return FALSE;

  if ((!BAR_POINTER) || (NO_POINTER))
  {
      lock_mouse();
      GrMouseEraseCursor();
  }

  SW2[0]=SERV[81];   SERV[81]=sel_d;

  poz_xx=PozX;
  poz_yy=PozY;
  PozX0=mouse_x;
  PozY0=mouse_y;
  listbox = ComboBox->listbox;
  listbox->x = ComboBox->x;
  listbox->y = ComboBox->y + ComboBox->dy;
  listbox->dx = ComboBox->dx;
  listbox->dy = ComboBox->dy;
  listbox_usk = listbox;
  if(FALSE == open_listbox(listbox))
  {
    SERV[81]=SW2[0];

      if ((!BAR_POINTER) || (NO_POINTER))
      {
          free_mouse();
          GrMouseUnEraseCursor();
      }

    return FALSE ;
  }

  init_lbox(0);
  while(1)
  {
    ev = GetEvent((TMENU*)listbox);
    if(ev->What == evKeyDown &&
	(ev->Number == 0 || ev->Number == ENTER))
    {
      if ((set_listbox_slider) && (ev->Number == ENTER))
      {
          ret = find_listbox_slider((LISTBOX*)listbox_address, &listbox_slider);
          if (ret==1) continue;
      }
      init_lbox(1);
      close_listbox(listbox);
      PozX=poz_xx;
      PozY=poz_yy;

#ifdef ALLEGRO5
#ifdef MACOS
    	MoveOSXCursor(PozX0,PozY0);
#else
    	position_mouse(PozX0,PozY0);
#endif
#else
    	position_mouse(PozX0,PozY0);
#endif
    	set_forget_mouse(PozX0,PozY0);

      MVCUR(0,0);
      SERV[81]=SW2[0];

        if ((!BAR_POINTER) || (NO_POINTER))
        {
            free_mouse();
            GrMouseUnEraseCursor();
        }
	   
      return ev->Number == 0 ? FALSE : TRUE;
    }

  }
  SERV[81]=SW2[0];

    if ((!BAR_POINTER) || (NO_POINTER))
    {
        free_mouse();
        GrMouseUnEraseCursor();
    }
   
}

static BOOL edit_list_box(LISTBOX *listbox)
/*------------------------------------------*/
{
  EVENT *ev;

  lock_mouse();
  listbox_usk = listbox;

  init_lbox(0);
  if(listbox->flags & FLB_BAROFF)
  {
     baronoff(listbox);
  }
  while(1)
  {
    ev = GetEvent((TMENU*)listbox);
    if(ev->What == evKeyDown &&
	(ev->Number == 0 || ev->Number == ENTER))
    {
	  init_lbox(1);
      if(listbox->flags & FLB_BAROFF)
      {
	    baronoff(listbox);
      }
	  free_mouse();

      return ev->Number == 0 ? FALSE : TRUE;
    }
	free_mouse();
  }
  free_mouse();

}


static int kolorXL1(int no_layer, int x, int y, int i_color0)
{

	int i_color;

  char sk1[MaxTextLen]="" ;
  int retval_no = 1 ;
  double buf_ret [1] ;

  sk1 [0] = '\0' ;
  i_color=i_color0;
  sprintf(sk1,"%#ld",i_color);
  if(!get_string1(sk1, "", 3, 4, 81,x,y)) return i_color;
  komunikat(0);
  if(FALSE == calculator(sk1, &retval_no, buf_ret)  || retval_no < 0 || retval_no > 255)
  {
  }
  else
  {
   i_color = buf_ret [0] ;
  }  
  return i_color;
}


static int kolorXL(int no_layer, int x, int y, int i_color0)
{ LINIA LiniaGB;

int i_color;

  LiniaGB.kolor = LiniaG.kolor;
  LiniaG.kolor = i_color0;
  i_color = colors1(i_color0);
  LiniaG.kolor = LiniaGB.kolor;
  
  return i_color;
}


static int find_combo_box(COMBOBOX *ComboBoxes,int SizeComboBoxT)
/*------------------------------------------------------------*/
{
  int i, ink, paper, poz;
  int ret = 0;
  int x1, x2, y1, y2;
  char temp [MaxTextLen], *ptr_temp ;
  int color;

  int kolor_m, kolor_rgb;
  char *ptr, *ptr1;
  char *numer_koloru_str;
  int numer_koloru;
  int typ_l, typ_f;
  int ret1;
  COLOR_ kolor;
  TEXT fontn= Tdef;
  BOOL draw_poz;
  int color_bak;
 
  cur_off(PozX, PozY);  
  for(i=0;i<SizeComboBoxT;i++)
  {
    if(ComboBoxes [i].listbox->enable == FALSE)
    {
	continue;
    }
    x1 = jed_to_piks_x(ComboBoxes[i].x)+pocz_x;
    x2 = jed_to_piks_x(ComboBoxes[i].x + ComboBoxes[i].dx) -1 +pocz_x;
    y1 = jed_to_piks_y(ComboBoxes[i].y)+pocz_y;
    y2 = jed_to_piks_y(ComboBoxes[i].y + ComboBoxes[i].dy) -1 +pocz_y;


    if(PozX > x1 && PozX < x2 && PozY > y1 && PozY < y2)
    {
	break;
    }
  }
  if(i < SizeComboBoxT)
  {
    ret = -1;
    ink = ComboBoxes[i].listbox->ink == COLOR_NULL ? dlg_kolory->dlg_combo_box_ink : ComboBoxes[i].listbox->ink;
    paper = ComboBoxes[i].listbox->paper == COLOR_NULL ? dlg_kolory->dlg_combo_box_paper : ComboBoxes[i].listbox->paper ;
    moveto( x1+DXIL, y1 +(y2-y1-8)/2);
	draw_poz = TRUE;
    if(edit_combo_box(ComboBoxes + i) == TRUE)
    {
	  show_mouse(NULL);
      poz = ComboBoxes [i].listbox->foff + ComboBoxes [i].listbox->poz ;
      ptr_temp = ComboBoxes [i].listbox->txt[poz] ;

      if(strlen(ptr_temp) > ComboBoxes[i].listbox->max_text_len)
      {
	 strcpy(temp, ptr_temp) ;
	 temp [ComboBoxes[i].listbox->max_text_len] = '\0' ;
	 ptr_temp = temp ;
      }
      
      ptr=strstr(ptr_temp,u8"֍");
      if(ptr!=NULL)
       { 
		  ptr1 = strstr(ptr_temp, _OTHER_);
		  
         if(ptr1!=NULL) 
         {
           numer_koloru_str=ptr1 + 10;
		   if (strlen(numer_koloru_str) == 0) kolor_m = 15;
		   else
		   {
			   while (numer_koloru_str[0] == '\040') numer_koloru_str += 1;
			   numer_koloru = atoi(numer_koloru_str);
			   kolor_m = GetColorAC1(numer_koloru);
		   }
         }
         else
         {
          kolor_m=GetColorAC1(poz + 1);
		  ptr_temp+=2;
         }
        setfillstyle_(SOLID_FILL,kolor_m);
	
        bar(x1+1, y1+1 - 2, x2-1, y2-1 + 2);
		
        if(kolor_m > 16)
         {
          kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
          if(kolor_rgb < 96) setcolor(15); else setcolor(0);  
         }
		set_proper_color(kolor_m);	
       }
       else
        {
		   setfillstyle_(SOLID_FILL, paper);
		   
		   bar(x1+1, y1+1 - 2, x2-1, y2-1 + 2);
		   
		   setcolor(ink);
		 
           ptr=strstr(ptr_temp,u8"֏");

           if(ptr!=NULL)
            {
			    typ_l = poz;
			    linestyle(typ_l + 64);
			    ret1 = lineCuncut(x1 + 1, y2-2, x2 - 1, y2-2);
				ptr_temp+=2;
		    }
		   else
		   {
			   ptr = strstr(ptr_temp, u8"҂");
			   if (ptr != NULL)
			   {
				   typ_l = poz;
				   if (typ_l < 5)
				   {
					   linestyle(typ_l * 32);
					   ret1 = lineCuncut(x1 + 1, y2 - 2, x2 - 1, y2 - 2);
				   }
				   ptr_temp+=2;
			   }
			   else if (ComboBoxes[i].listbox->id == ID_CFontName)
			   {
				   typ_f = poz;

				   moveto(x1, y1 - 2);

				   if (typ_f < i__font_nomax)
				   {

					   if ((PTRS__Text_Style[typ_f]->type < 2) || (PTRS__Text_Style[typ_f]->type == 3))
					   {
						   
						   draw_font_name(ptr_temp, typ_f, x1 + 5, y1 + HEIGHT + 10);
						   
					   }
					   else
					   {

						   kolor.red = 0;
						   kolor.gre = 0;
						   kolor.blu = 100;
						   strcpy(fontn.text, ptr_temp);
						   fontn.dl = strlen(fontn.text);
						   fontn.n = T18 + fontn.dl;
						   fontn.czcionka = typ_f;
						   fontn.kat = 0.0;
						   fontn.bold = 0;
						   fontn.width_factor = 1;
						   fontn.italics = 0;
						   fontn.justowanie = j_do_lewej;
						   fontn.multiline = 0;
						   fontn.ukryty = 0;
						   fontn.wysokosc = 4;
						   fontn.kolor = 0;
						   fontn.width = 0;
						   fontn.height = 0;
						   fontn.warstwa = Current_Layer;
						   fontn.widoczny = 1;

						   
						   draw_font_name_ttf(&fontn, &fontn.text, screen, x1 + 5, y1 + HEIGHT + vfv(10), x2, fontn.kat, vfv(20.0), kolor, COPY_PUT, 0, 0);
						  

					   }
				   }
				   else
				   {
					   color_bak = getcolor();
					   setcolor(184);
					   TTF_text_UI(ptr_temp, x1 + 5, y1 + 8, NULL, NULL);
					   setcolor(color_bak);
				   }
				   draw_poz = FALSE;
			   }
		   }
        }
      /**/   
	  if (strcmp(ptr_temp, _OTHER_) == 0)
		 
        {

		init(2, &dlg0, NULL);  //closing without restoring background

        color=kolorXL(0,x1+DXIL, y1 +(y2-y1-8)/2,ComboBoxes[i].listbox->wartosc);

        ComboBoxes [i].listbox->wartosc=color;
		if (color <= 16)
		{
			ComboBoxes[i].listbox->poz = color-1;
			if (ComboBoxes[i].listbox->poz>=ComboBoxes[i].listbox->maxw)
			{
				(ComboBoxes[i].listbox->poz = ComboBoxes[i].listbox->maxw-1);
				ComboBoxes[i].listbox->foff = color-1 - ComboBoxes[i].listbox->poz;
			}
			else ComboBoxes[i].listbox->foff = 0;
		}

        ret = -2;

		free_mouse();
		show_mouse(screen);
        return ret;
        }
       else if (draw_poz)
        {
        if (ComboBoxes[i].listbox->enable==1) {;} //setcolor(ink);
        else setcolor(DARKGRAY);
         moveto( x1+DXIL -2, y1 +(y2-y1-8)/2 +  8 - (HEIGHT/2));
		 int pos = findfpostopxl(ptr_temp, x2 - x1 - 3);
		 
         outtext_r(&ptr_temp[pos]);
		 
        } 
      ret = ComboBoxes[i].listbox->id;
	  show_mouse(screen);
    }
  }

  cur_on(PozX, PozY); 

  return ret;
}

static int find_list_box(LISTBOX *ListBoxes,int SizeListBoxT)
/*------------------------------------------------------------*/
{
  int i ;
  int ret = 0;
  int x1, x2, y1, y2, size;

  cur_off(PozX, PozY); 
  for(i=0;i<SizeListBoxT;i++)
  {
    if(ListBoxes [i].enable == FALSE)
    {
	continue;
    }
    size =((ListBoxes[i].maxw == 0) ? ListBoxes[i].max : ListBoxes[i].maxw);
    x1 = jed_to_piks_x(ListBoxes[i].x) + 1+pocz_x;
    x2 = jed_to_piks_x(ListBoxes[i].x + ListBoxes[i].dx) - 2+pocz_x;
    y1 = jed_to_piks_y(ListBoxes[i].y)+pocz_y;
    y2 = jed_to_piks_y(ListBoxes[i].y + ListBoxes[i].dy * size) - 1+pocz_y;
    
    if(PozX > x1 && PozX < x2 && PozY > y1 && PozY < y2)
    {
	break;
    }
  }
  if(i < SizeListBoxT)
  {
    ret = -1;

    ListBoxes[i].poz =(PozY - y1) /(jed_to_piks_y(ListBoxes[i].dy) + 0.5);
    if(ListBoxes[i].max <= ListBoxes[i].poz + ListBoxes[i].foff)
    {
      ListBoxes[i].poz = ListBoxes[i].max - ListBoxes[i].foff - 1;
      if(ListBoxes[i].poz < 0)
      {
	ListBoxes[i].poz = 0;
      }
    }
    GrMouseEraseCursor();
    if(edit_list_box(ListBoxes + i) == TRUE)
    {
      ret = ListBoxes[i].id;
    }
	GrMouseDisplayCursor();
    PozY = jed_to_piks_y(ListBoxes[i].y + ListBoxes[i].dy *(ListBoxes[i].poz + 0.5))+pocz_y;
  }
  cur_on(PozX, PozY);
  return ret;
}

void Draw_ComboBox(COMBOBOX *ComboBox)
/*-----------------------------------*/
{
  int poz, ink, paper, border;
  int x1, y1, x2, y2;

  char *ptr_temp;
  char sk[MaxTextLen + 1];
  char sk1[MaxTextLen + 1];
  int kolor_m, kolor_rgb;
  char *ptr, *ptr1;

  int typ_l, typ_f;
  int ret1;
  int ilen;
  COLOR_ kolor;
  TEXT fontn= Tdef;
  BOOL draw_poz;
  int color_bak;

  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
  setwritemode(COPY_PUT);

    paper = ComboBox->listbox->paper == COLOR_NULL? dlg_kolory->dlg_combo_box_paper : ComboBox->listbox->paper ;
    ink = ComboBox->listbox->ink == COLOR_NULL? dlg_kolory->dlg_combo_box_ink : ComboBox->listbox->ink ;
    border = ComboBox->listbox->border == COLOR_NULL? dlg_kolory->dlg_combo_box_border : ComboBox->listbox->border;
    x1 = jed_to_piks_x(ComboBox->x)+pocz_x;
    x2 = jed_to_piks_x(ComboBox->x + ComboBox->dx) - 1+pocz_x;
    y1 = jed_to_piks_y(ComboBox->y)+pocz_y;
    y2 = jed_to_piks_y(ComboBox->y + ComboBox->dy) - 1+pocz_y;
    setcolor(border);
    setwritemode(COPY_PUT);


	LINE(x1,y1-2,x2,y1-2);
    LINE(x1,y1-2,x1,y2+2);
    setcolor(15);
    LINE(x1,y2+2,x2,y2+2);
    LINE(x2,y1-2,x2,y2+2);
	
    setcolor(border);
    
    if (ComboBox->listbox->max > 0)   //win32todo
	{

		draw_poz = TRUE;
		poz = ComboBox->listbox->poz + ComboBox->listbox->foff;
		ptr_temp = ComboBox->listbox->txt[poz] ;

		ilen = strlen(ptr_temp);
		
		ptr1 = strstr(ptr_temp, _OTHER_);
		
	  if (ptr1!=NULL)
      {
        strcpy(sk,ptr_temp+2);
        sprintf(sk1," %#ld",ComboBox->listbox->wartosc);
        strncat(sk,sk1,strlen(sk1));
        kolor_m=GetColorAC1(ComboBox->listbox->wartosc);
        setfillstyle_(SOLID_FILL,kolor_m);
        bar(x1+1, y1+1 - 2, x2-1, y2-1 + 2);
        if(kolor_m > 16)
         {
          kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
          if(kolor_rgb < 96) setcolor(15); else setcolor(0);
         }

		set_proper_color(kolor_m);
      }
      else 
      {  
		ptr=strstr(ptr_temp,u8"֍");
         if(ptr!=NULL)
         { 
		  ptr_temp+=2;
          strcpy(sk,ptr_temp);
          kolor_m=GetColorAC1(poz + 1);
          setfillstyle_(SOLID_FILL,kolor_m);
		  bar(x1 + 1, y1 + 1 - 2, x2 - 1, y2 - 1);// +2);
          if(kolor_m > 16)
           {
            kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
            if(kolor_rgb < 96) setcolor(15); else setcolor(0);
           }

		  set_proper_color(kolor_m);
         } 
       else
        { 
	     
		 setfillstyle_(SOLID_FILL, paper);

		 bar(x1 + 1, y1 + 1 - 2, x2 - 1, y2 - 1 + 2);
		 setcolor(ink);

		 ptr = strstr(ptr_temp, u8"֏");
		
		 if (ptr != NULL)
		 {

			 if (ComboBox->listbox->id == 666)
			 {
				 setcolor(paper);
				 LINE(x1, y1 - 2, x2, y1 - 2);
				 setcolor(ink);
			}
			
			 typ_l = poz;
			 linestyle(typ_l + 64);
			 ret1 = lineCuncut(x1 + 1, y2 - 2, x2 - 2, y2 - 2);
			 ptr_temp+=2;
		 }
		 else
		 {
			  ptr=strstr(ptr_temp,u8"҂");
			  if (ptr != NULL)
			  {
				  typ_l = poz;
				  if (typ_l < 5)
				  {
					  if (ComboBox->listbox->id == 666)
					  {
						  setcolor(paper);
						  LINE(x1, y1 - 2, x2, y1 - 2);
						  setcolor(ink);
					  }
					  
					  linestyle(typ_l * 32);
					  ret1 = lineCuncut(x1 + 1, y2 - 2, x2 - 2, y2 - 2);
				  }
				  ptr_temp+=2;
			  }
			  else if (ComboBox->listbox->id == ID_CFontName)

			  {
				  typ_f = poz;
				  if (typ_f < i__font_nomax)
				  {

					  moveto(x1, y1 - 2);

					  if (typ_f < i__font_nomax)
					  {
						  if ((PTRS__Text_Style[typ_f]->type < 2) || (PTRS__Text_Style[typ_f]->type == 3))
							  draw_font_name(ptr_temp, typ_f, x1 + 5, y1 + HEIGHT + 10);
						  else
						  {

							  kolor.red = 0;
							  kolor.gre = 0;
							  kolor.blu = 100;
							  strcpy(fontn.text, ptr_temp);
							  fontn.warstwa = Current_Layer;
							  fontn.widoczny = 1;
							  fontn.dl = strlen(fontn.text);
							  fontn.n = T18 + fontn.dl;
							  fontn.czcionka = typ_f;
							  fontn.kat = 0.0;
							  fontn.bold = 0;
							  fontn.width_factor = 1;
							  fontn.italics = 0;
							  fontn.justowanie = j_do_lewej;
							  fontn.multiline = 0;
							  fontn.ukryty = 0;
							  fontn.wysokosc = 4;
							  fontn.kolor = 0;
							  fontn.width = 0;
							  fontn.height = 0;
							  fontn.warstwa = Current_Layer;
							  fontn.widoczny = 1;

							  draw_font_name_ttf(&fontn, &fontn.text, screen, x1 + 5, y1 + HEIGHT + 10, x2, fontn.kat, 20.0, kolor, COPY_PUT, 0, 0);

						  }
					  }
					  else
					  {
						  color_bak = getcolor();
						  setcolor(184);
						  TTF_text_UI(ptr_temp, x1 + 5, y1 + 8, NULL, NULL);
						  setcolor(color_bak);
					  }

					  draw_poz = FALSE;
				  }

			  }
		 }
		 
         strcpy(sk,ptr_temp);
		
        }
      } 
	  if (draw_poz)
		{
            if (ComboBox->listbox->enable==1) {;}
            else setcolor(DARKGRAY);
            moveto(x1 + DXIL - 2, y1 + (y2 - y1 - 8) / 2 +  8 - (HEIGHT/2));
			int pos = findfpostopxl(sk, x2 - x1 - 3);
			outtext_r(&sk[pos]);
		}
	}
}


static void draw_combo_boxes(COMBOBOX *ComboBoxes,int SizeComboBoxT)
/*--------------------------------------------------------------*/
{
  int i;

  for(i=0;i<SizeComboBoxT;i++)
  {
    Draw_ComboBox(&ComboBoxes[i]);
  }
}

void Draw_ListBox(LISTBOX *listbox)
/*---------------------------------*/
{
  draw_listbox(listbox);
  if(!(listbox->flags & FLB_BAROFF))
  {
    baron(listbox);
  }
}

static void draw_list_boxes(LISTBOX *ListBoxes,int SizeListBoxT)
/*--------------------------------------------------------------*/
{
  int i;

  for(i=0;i<SizeListBoxT;i++)
  {
    Draw_ListBox(&ListBoxes[i]);
  }
}


/*-------------------------------------------*/
/*-------------------------------------------*/
/*		  BUTTONS		     */
/*-------------------------------------------*/
/*-------------------------------------------*/

#define DXHBut 14
#define DYHBut 14
#define multigrey 8

void draw_push_button(BUTTON *Button)
/*----------------------------------------*/
{
	int paper, border, ink;
	int x1, x2, y1, y2, x0, y0;
	BITMAP *bmp, *bmp24, *bmp32, *bmp48, *bmp64;
	int ret;
	int flags;
	char dir[MAXDIR]="";
	char drive[MAXDRIVE]="";
	char file[MAXFILE]="";
	char ext[MAXEXT]="";
	int dx, dy, wh;
	int HEIGHT_BAK, WIDTH_BAK;
	int pos_txt;
	char* dirptr;
    int folder_icon_no = 118;
    int folder_icon_no24 = 116;
	int folder_icon_no32 = 117;
    int folder_icon_no48 = 118;
    int folder_icon_no64 = 119;
	char Button_txt[MAXDIR];
	float txt_scale_factor = 0.85;
    int len;
    BITMAP *screenplay=Get_Screenplay();
    int size_checkbox0;
    int fv;

	if (Button->flags & BUTTON_HIDDEN) return;
	setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
	setwritemode(COPY_PUT);

	paper = Button->paper == COLOR_NULL ? dlg_kolory->dlg_button_paper : Button->paper;
	if (Button->enable == 0) ink = DARKGRAY;
	else ink = Button->ink == COLOR_NULL ? dlg_kolory->dlg_button_ink : Button->ink;
	border = Button->border == COLOR_NULL ? dlg_kolory->dlg_button_border : Button->border;
    if ((Button->txt != NULL) && (Button->txt[0] == '\024')) setfillstyle_(SOLID_FILL, paper);  //DARKGRAY
	    else setfillstyle_(SOLID_FILL, paper);
	x1 = jed_to_piks_x(Button->x) + pocz_x;
	x2 = jed_to_piks_x(Button->x + Button->dx - 1) + pocz_x;
	y1 = jed_to_piks_y(Button->y) + pocz_y;
	y2 = jed_to_piks_y(Button->y + Button->dy - 1) + pocz_y;
	bar(x1 + 1, y1 + 1, x2 - 1, y2 - 1);

    //if (is_RETINA())
    //    fv=2;
    //else fv=1;

	//if ((y2-y1) < 54*fv) folder_icon_no = 117;
	//else if ((y2-y1) > 70*fv) folder_icon_no = 119;

    bmp24 = (BITMAP*)get_icon(folder_icon_no24);
    bmp32 = (BITMAP*)get_icon(folder_icon_no32);
    bmp48 = (BITMAP*)get_icon(folder_icon_no48);
    bmp64 = (BITMAP*)get_icon(folder_icon_no64);

    if ((y2-y1)*0.8 < bmp32->h) folder_icon_no = 116;
    else if ((y2-y1)*0.8 < bmp48->h) folder_icon_no = 117;
    else if ((y2-y1)*0.8 < bmp64->h) folder_icon_no = 118;
    else folder_icon_no = 119;

	setcolor(border);
	LINE(x1 + 1, y1, x2 - 1, y1);
	LINE(x1 + 1, y2, x2 - 1, y2);
	LINE(x1, y1 + 1, x1, y2 - 1);
	LINE(x2, y1 + 1, x2, y2 - 1);
	if (!Button->check)
	{
		setcolor(BORDER_LIGHT);
		LINE(x1 + 1, y1 + 1, x2 - 1, y1 + 1);
		LINE(x1 + 1, y1 + 2, x2 - 2, y1 + 2);
		LINE(x1 + 1, y1 + 1, x1 + 1, y2 - 1);
		LINE(x1 + 2, y1 + 2, x1 + 2, y2 - 2);
		setcolor(BORDER_DARK);
		LINE(x1 + 1, y2 - 1, x2 - 1, y2 - 1);
		LINE(x1 + 2, y2 - 2, x2 - 1, y2 - 2);
		LINE(x2 - 1, y1 + 1, x2 - 1, y2 - 1);
		LINE(x2 - 2, y1 + 2, x2 - 2, y2 - 2);
	}
	else
	{
		setcolor(BORDER_DARK);
		LINE(x1 + 1, y1 + 1, x2 - 1, y1 + 1);
		LINE(x1 + 1, y1 + 1, x1 + 1, y2 - 1);
		setcolor(BORDER_LIGHT);
		LINE(x1 + 1, y2 - 1, x2 - 1, y2 - 1);
		LINE(x2 - 1, y1 + 1, x2 - 1, y2 - 1);
	}
    if (Button->flags & BUTTON_GROUP)
    {
        int dy=(y2-y1)/5;
        setfillstyle_(SOLID_FILL, GetColorAC(241));

        bar(x2, y1 + 2, x2 + multigrey, y1 + 2 + dy);
        setfillstyle_(SOLID_FILL, GetColorAC(244));

        bar(x2, y1 + 2 +dy, x2 + multigrey, y1 +2 + 2*dy);
        setfillstyle_(SOLID_FILL, GetColorAC(248));

        bar(x2, y1 + 2 + 2*dy, x2 + multigrey, y1 + 2+ 3*dy);
    }
	setcolor(ink);
	x0 = (x1 + x2) / 2;
	y0 = 1 + (y1 + y2) / 2;

	settextjustify(CENTER_TEXT, CENTER_TEXT);
	if (Button->txt != NULL)
	{
        if ((strlen(Button->txt) > 0) && (Button->txt[0] == '\023'))
        {
            txt_scale_factor = (float) (Button->dx / 72.0 * 0.8);
            if (txt_scale_factor < 0.65) txt_scale_factor = 0.65f;
            if (txt_scale_factor > 0.85) txt_scale_factor = 0.85f;

            ////txt_scale_factor = 1.0; ////temporary

            movex0 = x1 + 6;
            deltax0 = x2 - x1 - 12;
            movey0 = y2 - 6;
            deltay0 = y2 - y1 - 12;

            if (Button->flags & BUTTON_FOLDER)
            {
                //wh = get_icon_size(folder_icon_no);
                //dx = -wh / 2;
                //dy = -wh / 2;

                bmp = (BITMAP*)get_icon(folder_icon_no);
                wh = bmp->w;
                dx = -wh / 2;
                dy = -wh / 2;
                x1 = jed_to_piks_x(Button->x + Button->dx / 2) + pocz_x + dx;
                y1 = jed_to_piks_y(Button->y + Button->dy / 2) + pocz_y + dy;

                if (bmp != NULL)
                {
                    blit(bmp, screenplay, 0, 0, x1, y1, wh, wh);
                }
            }
            else {
                //view block
                if (Button->txt != NULL)
                    ret = ViewInsBlock(Button->adr, FALSE);
            }

            strcpy(&file, (char *)(Button->txt + 1));
            setcolor(ink);
            moveto(x0 + 1, y2 + 3);
            settextjustify(CENTER_TEXT, TOP_TEXT);
            HEIGHT_BAK = HEIGHT;
            WIDTH_BAK = WIDTH;
            HEIGHT = (int)((float)HEIGHT*txt_scale_factor);
            WIDTH = (int)((float)WIDTH*txt_scale_factor);
            len = utf8len(file);
            pos_txt = findfpostopxl(file, (int)((float)deltax0 / 0.95f));
            if (pos_txt < len) outtext_r(&file[pos_txt]);

            HEIGHT = HEIGHT_BAK;
            WIDTH = WIDTH_BAK;
        }
		else if ((strlen(Button->txt) > 0) && (Button->txt[0] == '\024'))
		{
			txt_scale_factor = (float)Button->dx / 72.0f * 0.8f;
			if (txt_scale_factor < 0.65) txt_scale_factor = 0.65f;
			if (txt_scale_factor > 0.85) txt_scale_factor = 0.85f;

			movex0 = x1 + 6;
			deltax0 = x2 - x1 - 12;
			movey0 = y2 - 6;
			deltay0 = y2 - y1 - 12;
 			ret = ViewBlock((char *)(Button->txt + 1), TRUE);
			flags = fnsplit((char *)(Button->txt + 1), drive, dir, file, ext);
			setcolor(ink);
			moveto(x0 + 1, y2 + 3);
			settextjustify(CENTER_TEXT, TOP_TEXT);
			HEIGHT_BAK = HEIGHT;
			WIDTH_BAK = WIDTH;
			HEIGHT = (int)((float)HEIGHT*txt_scale_factor);
			WIDTH = (int)((float)WIDTH*txt_scale_factor);
			pos_txt= findfpostopxl(file, (int)((float)deltax0 / 0.95));
			outtext_r(&file[pos_txt]);
			HEIGHT = HEIGHT_BAK;
			WIDTH = WIDTH_BAK;
		}
		else if ((strlen(Button->txt) > 0) && (Button->txt[0] == '\026'))
		{
			txt_scale_factor = (float)(Button->dx / 72.0 * 0.8);
			if (txt_scale_factor < 0.65) txt_scale_factor = 0.65f;
			if (txt_scale_factor > 0.85) txt_scale_factor = 0.85f;

			movex0 = x1 + 6;
			deltax0 = x2 - x1 - 12;
			movey0 = y2 - 6;
			deltay0 = y2 - y1 - 12;

			strcpy(Button_txt, Button->txt + 1);
			flags = fnsplit(Button_txt, drive, dir, file, ext);
			dirptr = strstr(Button_txt, "<DIR>");

			if (dirptr != NULL)
			{

				//wh = get_icon_size(folder_icon_no);
				//dx = -wh / 2;
				//dy = -wh / 2;

				bmp = (BITMAP*)get_icon(folder_icon_no);
                wh = bmp->w;
                dx = -wh / 2;
                dy = -wh / 2;
				x1 = jed_to_piks_x(Button->x + Button->dx / 2) + pocz_x + dx;
				y1 = jed_to_piks_y(Button->y + Button->dy / 2) + pocz_y + dy;

				if (bmp != NULL)
				{
					blit(bmp, screenplay, 0, 0, x1, y1, wh, wh);
				}
				*dirptr = '\0';
			}
			else
			{
				ret = ViewBlock((char*)(Button->txt + 1), FALSE);
		    }
			flags = fnsplit((char *)(Button->txt + 1), drive, dir, file, ext);
			setcolor(ink);
			moveto(x0 + 1, y2 + 3);
			settextjustify(CENTER_TEXT, TOP_TEXT);
			HEIGHT_BAK = HEIGHT;
			WIDTH_BAK = WIDTH;
			HEIGHT = (int)((float)HEIGHT*txt_scale_factor);
			WIDTH = (int)((float)WIDTH*txt_scale_factor);
			pos_txt = findfpostopxl(file, (int)((float)deltax0 / 0.95f));
			outtext_r(&file[pos_txt]);
			HEIGHT = HEIGHT_BAK;
			WIDTH = WIDTH_BAK;
		}
		else if ((strlen(Button->txt) > 0) && (Button->txt[0] == '\025'))
		{
			txt_scale_factor = (float)((float)Button->dx / 72.0f * 0.8f);
			if (txt_scale_factor < 0.65) txt_scale_factor = 0.65f;
			if (txt_scale_factor > 0.85) txt_scale_factor = 0.85f;

			movex0 = x1 + 6;
			deltax0 = x2 - x1 - 12;
			movey0 = y2 - 6;
			deltay0 = y2 - y1 - 12;

			strcpy(Button_txt, Button->txt + 1);
			flags = fnsplit(Button_txt, drive, dir, file, ext);
			dirptr = strstr(Button_txt, "<DIR>");
			if (dirptr != NULL)
			{

			   //wh = get_icon_size(folder_icon_no);
			   //dx = -wh / 2;
			   //dy = -wh / 2;

			   bmp = (BITMAP*)get_icon(folder_icon_no);
               wh = bmp->w;
               dx = -wh / 2;
               dy = -wh / 2;
			   x1 = jed_to_piks_x(Button->x + Button->dx / 2) + pocz_x + dx;
			   y1 = jed_to_piks_y(Button->y + Button->dy / 2) + pocz_y + dy;

			   if (bmp != NULL)
			   {
				   blit(bmp, screenplay, 0, 0, x1, y1, wh, wh);
			   }
			   *dirptr = '\0';
			}
			else
			{
				ret = ViewPattern(Button_txt, ext);
			}

			flags = fnsplit(Button_txt, drive, dir, file, ext);
			
			setcolor(ink);
			moveto(x0 + 1, y2 + 3);
			settextjustify(CENTER_TEXT, TOP_TEXT);
			HEIGHT_BAK = HEIGHT;
			WIDTH_BAK = WIDTH;
			HEIGHT = (int)((float)HEIGHT*txt_scale_factor);
			WIDTH = (int)((float)WIDTH*txt_scale_factor);
			pos_txt = findfpostopxl(file, (int)((float)deltax0 / 0.95f));
			outtext_r(&file[pos_txt]);
			HEIGHT = HEIGHT_BAK;
			WIDTH = WIDTH_BAK;
		}
		else
		{
			if (Button->name2 == 1)
			{
				char *st = Button->txt;

				if (st == NULL)
					return;

				moveto(x0, y0 - (ED_INF_HEIGHT)-2);
				outtext_r(Button->txt);
				moveto(x0, y0 + 2);
				st += strlen(st) + 1;
				outtext_r(st);
			}
			else if (Button->name2 > 1)
			{

				//wh = get_icon_size(Button->name2);
				//dx = -wh / 2;
				//dy = -wh / 2;

				bmp = (BITMAP *)get_icon(Button->name2);
                wh = bmp->w;
                dx = -wh / 2;
                dy = -wh / 2;
				x1 = jed_to_piks_x(Button->x + Button->dx / 2) + pocz_x +dx;
				y1 = jed_to_piks_y(Button->y + Button->dy / 2) + pocz_y +dy;

				if (bmp != NULL)
				{
                    if (Button->enable)
					   blit(bmp, screenplay, 0, 0, x1, y1, wh, wh);
                    else
                    {
                        BITMAP *bmp1= create_bitmap_ex(32, bmp->w, bmp->h);
                        dimm_dialog_bitmap(bmp, bmp1, bmp->w, bmp->h, 5);
                        blit(bmp1, screenplay, 0, 0, x1, y1, wh, wh);
                        destroy_bitmap(bmp1);
                    }
				}
			}
			else
			{
				moveto(x0 + 1, y0 - (ED_INF_HEIGHT / 2) + 1);
				outtext_r(Button->txt);
			}
		}
	}
  settextjustify(LEFT_TEXT, TOP_TEXT);
}

void draw_flat_push_button(BUTTON *Button)
/*----------------------------------------*/
{
	int paper, border, ink;
	int x1, x2, y1, y2, x0, y0;
	BITMAP *bmp, *bmp24, *bmp32, *bmp48, *bmp64;
	int ret;
	int flags;
	char dir[MAXDIR]="";
	char drive[MAXDRIVE]="";
	char file[MAXFILE]="";
	char ext[MAXEXT]="";
	int dx, dy, wh;
	int pos_txt;
	char* dirptr;
	int folder_icon_no = 118;
    int folder_icon_no24 = 116;
    int folder_icon_no32 = 117;
    int folder_icon_no48 = 118;
    int folder_icon_no64 = 119;
	char Button_txt[MAXDIR];
    int fv;
    BITMAP *screenplay=Get_Screenplay();

	if (Button->flags & BUTTON_HIDDEN)
    {
        x1 = jed_to_piks_x(Button->x) + pocz_x;
        x2 = jed_to_piks_x(Button->x + Button->dx - 1) + pocz_x;
        y1 = jed_to_piks_y(Button->y) + pocz_y;
        y2 = jed_to_piks_y(Button->y + Button->dy - 1) + pocz_y;
        paper = dlg_kolory->dlg_paper;
        setfillstyle_(SOLID_FILL, paper);
        bar(x1, y1, x2, y2);
        return;
    }
	setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
	setwritemode(COPY_PUT);
	paper = Button->paper == COLOR_NULL ? dlg_kolory->dlg_button_paper : Button->paper;
	if (Button->enable == 0) ink = DARKGRAY;
	else ink = Button->ink == COLOR_NULL ? dlg_kolory->dlg_button_ink : Button->ink;
	border = Button->border == COLOR_NULL ? dlg_kolory->dlg_button_border : Button->border;
	setfillstyle_(SOLID_FILL, paper);
	x1 = jed_to_piks_x(Button->x) + pocz_x;
	x2 = jed_to_piks_x(Button->x + Button->dx - 1) + pocz_x;
	y1 = jed_to_piks_y(Button->y) + pocz_y;
	y2 = jed_to_piks_y(Button->y + Button->dy - 1) + pocz_y;
	bar(x1 + 1, y1 + 1, x2 - 1, y2 - 1);

    //if (is_RETINA())
    //    fv=2;
    //else fv=1;

    //if ((y2-y1) < 54*fv) folder_icon_no = 117;
    //else if ((y2-y1) > 70*fv) folder_icon_no = 119;

    bmp24 = (BITMAP*)get_icon(folder_icon_no24);
    bmp32 = (BITMAP*)get_icon(folder_icon_no32);
    bmp48 = (BITMAP*)get_icon(folder_icon_no48);
    bmp64 = (BITMAP*)get_icon(folder_icon_no64);

    if ((y2-y1)*0.8 < bmp32->h) folder_icon_no = 116;
    else if ((y2-y1)*0.8 < bmp48->h) folder_icon_no = 117;
    else if ((y2-y1)*0.8 < bmp64->h) folder_icon_no = 118;
    else folder_icon_no = 119;

	setcolor(border);
	LINE(x1 + 1, y1, x2 - 1, y1);
	LINE(x1 + 1, y2, x2 - 1, y2);
	LINE(x1, y1 + 1, x1, y2 - 1);
	LINE(x2, y1 + 1, x2, y2 - 1);
	if (!Button->check)
	{
		setcolor(BORDER_LIGHT);
		LINE(x1 + 1, y1 + 1, x2 - 1, y1 + 1);

		LINE(x1 + 1, y1 + 1, x1 + 1, y2 - 1);

		setcolor(BORDER_DARK);
		LINE(x1 + 1, y2 - 1, x2 - 1, y2 - 1);

		LINE(x2 - 1, y1 + 1, x2 - 1, y2 - 1);

	}
	else
	{
		setcolor(BORDER_DARK);
		LINE(x1 + 1, y1 + 1, x2 - 1, y1 + 1);
		LINE(x1 + 1, y1 + 1, x1 + 1, y2 - 1);
		setcolor(BORDER_LIGHT);
	    LINE(x1 + 1, y2 - 1, x2 - 1, y2 - 1);
		LINE(x2 - 1, y1 + 1, x2 - 1, y2 - 1);
	}
	setcolor(ink);
	x0 = (x1 + x2) / 2;
	y0 = 1 + (y1 + y2) / 2;

	settextjustify(CENTER_TEXT, CENTER_TEXT);
	if (Button->txt != NULL)
	{
		if ((strlen(Button->txt) > 0) && (Button->txt[0] == '\026'))
		{
			movex0 = x1 + 6;
			deltax0 = x2 - x1 - 12;
			movey0 = y2 - 6;
			deltay0 = y2 - y1 - 12;
            
			strcpy(Button_txt, Button->txt + 1);
			dirptr = strstr(Button_txt, "<DIR>");

			flags = fnsplit((char *)(Button->txt + 1), drive, dir, file, ext);
			if (dirptr != NULL)
			{

				//wh = get_icon_size(folder_icon_no);
				//dx = -wh / 2;
				//dy = -wh / 2;

				bmp = (BITMAP*)get_icon(folder_icon_no);
                wh = bmp->w;
                dx = -wh / 2;
                dy = -wh / 2;
				x1 = jed_to_piks_x(Button->x + Button->dx / 2) + pocz_x + dx;
				y1 = jed_to_piks_y(Button->y + Button->dy / 2) + pocz_y + dy;

				if (bmp != NULL)
				{
					blit(bmp, screenplay, 0, 0, x1, y1, wh, wh);
				}

				*dirptr = '\0';
			}
			else ret = ViewBlock((char*)(Button->txt + 1), FALSE);

			flags = fnsplit(Button_txt, drive, dir, file, ext);

			setcolor(ink);
			moveto(x0 + 1, y2 + 2);
			settextjustify(CENTER_TEXT, TOP_TEXT);
			pos_txt = findfpostopxl(file, deltax0);
			outtext_r(&file[pos_txt]);
		}
		else if ((strlen(Button->txt) > 0) && (Button->txt[0] == '\025'))
		{
			movex0 = x1 + 6;
			deltax0 = x2 - x1 - 12;
			movey0 = y2 - 6;
			deltay0 = y2 - y1 - 12;

			strcpy(Button_txt, Button->txt + 1);
			dirptr = strstr(Button_txt, "<DIR>");
			flags = fnsplit(Button_txt, drive, dir, file, ext);

			if (dirptr != NULL)
			{
				//wh = get_icon_size(folder_icon_no);
				//dx = -wh / 2;
				//dy = -wh / 2;

				bmp = (BITMAP*)get_icon(folder_icon_no);
                wh = bmp->w;
                dx = -wh / 2;
                dy = -wh / 2;
				x1 = jed_to_piks_x(Button->x + Button->dx / 2) + pocz_x + dx;
				y1 = jed_to_piks_y(Button->y + Button->dy / 2) + pocz_y + dy;

				if (bmp != NULL)
				{
					blit(bmp, screenplay, 0, 0, x1, y1, wh, wh);
				}

				*dirptr = '\0';
			}
			else ret = ViewPattern(Button_txt, ext);
			
			flags = fnsplit(Button_txt, drive, dir, file, ext);

			setcolor(ink);
			moveto(x0 + 1, y2 + 2);
			settextjustify(CENTER_TEXT, TOP_TEXT);
			pos_txt = findfpostopxl(file, deltax0);
			outtext_r(&file[pos_txt]);
		}
		else
		{
			if (Button->name2 == 1)
			{
				char *st = Button->txt;

				if (st == NULL)
					return;

				moveto(x0, y0 - (ED_INF_HEIGHT)-2);
				outtext_r(Button->txt);
				moveto(x0, y0 + 2);
				st += strlen(st) + 1;
				outtext_r(st);
			}
			else if (Button->name2 > 1)
			{

				//wh = get_icon_size(Button->name2);
				//dx = -wh / 2;
				//dy = -wh / 2;

				bmp = (BITMAP *)get_icon(Button->name2);
                wh = bmp->w;
                dx = -wh / 2;
                dy = -wh / 2;
				x1 = jed_to_piks_x(Button->x + Button->dx / 2) + pocz_x + dx;
				y1 = jed_to_piks_y(Button->y + Button->dy / 2) + pocz_y + dy;

				if (bmp != NULL)
				{
                    if (Button->enable)
					   blit(bmp, screenplay, 0, 0, x1, y1, wh, wh);

                    else
                    {
                        BITMAP *bmp1= create_bitmap(bmp->w, bmp->h);
                        dimm_dialog_bitmap(bmp, bmp1, bmp->w, bmp->h, 5);
                        blit(bmp1, screenplay, 0, 0, x1, y1, wh, wh);
                        destroy_bitmap(bmp1);
                    }
				}
			}
			else
			{
				moveto(x0 + 1, y0 - (ED_INF_HEIGHT / 2) + 1);
				outtext_r(Button->txt);
			}
		}
	}
	settextjustify(LEFT_TEXT, TOP_TEXT);
}

static void draw_check_box(BUTTON *Button)
/*---------------------------------------*/
{
  int paper, ink;
  int x1,x2,y1,y2, x0, y0;

    if (Button->flags & BUTTON_HIDDEN)
    {
        x1 = jed_to_piks_x(Button->x) + pocz_x;
        x2 = jed_to_piks_x(Button->x + Button->dx - 1) + pocz_x;
        y1 = jed_to_piks_y(Button->y) + pocz_y;
        y2 = jed_to_piks_y(Button->y + Button->dy - 1) + pocz_y;
        paper = dlg_kolory->dlg_paper;
        setfillstyle_(SOLID_FILL, paper);
        bar(x1, y1, x2, y2);
        return;
    }

  //if (Button->flags & BUTTON_HIDDEN) return;
  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
  setwritemode(COPY_PUT);
  paper = Button->paper == COLOR_NULL ? dlg_kolory->dlg_button_paper : Button->paper ;
  if (Button->enable==0) ink=DARKGRAY;
    else ink = Button->ink == COLOR_NULL ? dlg_kolory->dlg_button_ink : Button->ink;
  if (paper > 255)
	  return;

  setfillstyle_(SOLID_FILL,paper);
  SIZE_CHECKBOX = (int)((float)SIZE_CHECKBOX0*((float)WIDTH / 8.0));
  x1 = jed_to_piks_x(Button->x)+pocz_x;
  x2 = x1 + SIZE_CHECKBOX - 1;
  y1 = jed_to_piks_y(Button->y)+pocz_y;
  y2 = jed_to_piks_y(Button->y + Button->dy) - 1+pocz_y;
  y1 +=(y2 - y1 - SIZE_CHECKBOX - 1) / 2;
  y2 = y1 + SIZE_CHECKBOX - 1;
  bar(x1 + 1, y1 + 1, x2 - 1, y2 - 1);
  if(!Button->check)
  {
    setcolor(BORDER_LIGHT);
    LINE(x1, y1, x2, y1);
    LINE(x1, y1, x1, y2);
    setcolor(BORDER_DARK);
    LINE(x1, y2, x2, y2);
    LINE(x2, y1, x2, y2);
  }
  else
  {
    setcolor(BORDER_DARK);
    LINE(x1, y1, x2, y1);
    LINE(x1, y1, x1, y2);
    setcolor(BORDER_LIGHT);
    LINE(x1, y2, x2, y2);
    LINE(x2, y1, x2, y2);
    setcolor(ink);
    LINE( x1+3,(y1+y2)/2,(x1+x2)/2 - 2, y2-3);
    LINE((x1+x2)/2 - 2, y2-3, x2 - 3, y1 + 4);
  }
  setcolor(ink);
  x0 = x2 +(y2 - y1) / 2;
  y0 = 1 +(y1 + y2) / 2;
  moveto(x0, y0 - 5 /*4*/);
  outtext_r(Button->txt);
}

static void draw_round_radio_button(BUTTON *Button)
{
int paper, ink, col;
int x0, y0;

if (Button->flags & BUTTON_HIDDEN) return;
setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
setwritemode(COPY_PUT);
paper = Button->paper == COLOR_NULL ? dlg_kolory->dlg_button_paper : Button->paper;
if (Button->enable == 0) ink = DARKGRAY;
else ink = Button->ink == COLOR_NULL ? dlg_kolory->dlg_button_ink : Button->ink;

setfillstyle_(SOLID_FILL, paper);


SIZE_ROUNDRADIOBUTTON = (int)((float)SIZE_ROUNDRADIOBUTTON0*((float)WIDTH / 8.0));

x0 = jed_to_piks_x(Button->x) + pocz_x + SIZE_ROUNDRADIOBUTTON - WIDTH * WIDTH / 18;
y0 = jed_to_piks_y(Button->y) + pocz_y - SIZE_ROUNDRADIOBUTTON - HEIGHT * HEIGHT / 120;

setcolor(paper);

CIRCLEFILL(x0, y0, SIZE_ROUNDRADIOBUTTON);
if (!Button->check)
{
	setcolor(BORDER_DARK);
	ARC(x0, y0, itofix(-64), itofix(64), SIZE_ROUNDRADIOBUTTON + 1);
	setcolor(BORDER_LIGHT);
	ARC(x0, y0, itofix(64), itofix(192), SIZE_ROUNDRADIOBUTTON + 1);
}
else
{
	setcolor(BORDER_LIGHT);
	ARC(x0, y0, itofix(-64), itofix(64), SIZE_ROUNDRADIOBUTTON + 1);
	setcolor(BORDER_DARK);
	ARC(x0, y0, itofix(64), itofix(192), SIZE_ROUNDRADIOBUTTON + 1);
	col = ink;
	if (ink == paper)
	{
		col = getmaxcolor() - paper;
	}
	setcolor(col);

	CIRCLEFILL(x0, y0, SIZE_ROUNDRADIOBUTTON/2);
}
setcolor(ink);

moveto(x0 - SIZE_ROUNDRADIOBUTTON -1, y0 - 5);
outtext_r(Button->txt);
settextjustify(LEFT_TEXT, TOP_TEXT);
}


static void draw_radio_button(BUTTON *Button)
/*------------------------------------------*/
{
  int paper, ink, col;
  int x1, x2, y1, y2, x0, y0;
  int polypoints [ 8 ];
  float skl;

  if (Button->flags & BUTTON_HIDDEN) return;
  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
  setwritemode(COPY_PUT);
  paper = Button->paper == COLOR_NULL ? dlg_kolory->dlg_button_paper : Button->paper ;
  if (Button->enable==0) ink=DARKGRAY;
    else ink = Button->ink == COLOR_NULL ? dlg_kolory->dlg_button_ink : Button->ink;

  setfillstyle_(SOLID_FILL,paper);
  x1 = jed_to_piks_x(Button->x)+pocz_x;

  skl = (float)((float)(HEIGHT) / 15.0);
  if (skl < 1.0) skl = 1.0f;

  SIZE_RADIOBUTTON = (int)((float)SIZE_RADIOBUTTON0 *skl);
  x0 = x1 + SIZE_RADIOBUTTON;
  x2 = x0 + SIZE_RADIOBUTTON;
  y1 = jed_to_piks_y(Button->y)+pocz_y;
  y2 = jed_to_piks_y(Button->y + Button->dy - 1)+pocz_y;
  y0 =(y1 + y2) / 2;
  y1 = y0 - SIZE_RADIOBUTTON;
  y2 = y0 + SIZE_RADIOBUTTON;
  polypoints [0] = x0;   polypoints [1] = y2-1;
  polypoints [2] = x1+1;   polypoints [3] = y0;
  polypoints [4] = x0;   polypoints [5] = y1+1;
  polypoints [6] =  x2-1;  polypoints [7] = y0;
  setcolor(paper);

  fillpoly(4, polypoints);
  if(!Button->check)
  {
    setcolor(BORDER_DARK);
    LINE(x0+1, y1+1, x2, y0);
    LINE(x2, y0, x0, y2);
    setcolor(BORDER_LIGHT);
    LINE(x0-1, y2-1, x1, y0);
    LINE(x1, y0, x0, y1);
  }
  else
  {
    setcolor(BORDER_LIGHT);
    LINE(x0+1, y1+1, x2, y0);
    LINE(x2, y0, x0, y2);
    setcolor(BORDER_DARK);
    LINE(x0-1, y2-1, x1, y0);
    LINE(x1, y0, x0, y1);
    col = ink;
    if(ink == paper)
    {
      col = getmaxcolor() - paper;
    }
    setcolor(col);

	
	putpixel_(x0, y0 - 3, col);
	LINE(x0 - 1, y0 - 2, x0 + 1, y0 - 2);
	LINE(x0 - 2, y0 - 1, x0 + 2, y0 - 1);
	LINE(x0 - 3, y0, x0 + 3, y0);
	LINE(x0 - 2, y0 + 1, x0 + 2, y0 + 1);
	LINE(x0 - 1, y0 + 2, x0 + 1, y0 + 2);
	putpixel_(x0, y0 + 3, col);
	
  }
  setcolor(ink);

  x0 = x2 +(y2 - y1) / 2;
  y0 = 1 +(y1 + y2) / 2;
  moveto(x0, y0 - 5);
  if (strlen(Button->txt) > 0)
  {
	  bar(x0, y0, x0 + TTF_text_len(Button->txt), y0 + HEIGHT);
	  outtext_r(Button->txt);
  }
  settextjustify(LEFT_TEXT, TOP_TEXT);
}


void Draw_Button(BUTTON *Button)
/*-----------------------------*/
{
    switch(Button->type)
    {
      case B_PUSHBUTTON:
	 draw_push_button(Button);
	 break;
      case B_CHECKBOX:
	 draw_check_box(Button);
	 break;
      case B_RADIOBUTTON:
	 draw_radio_button(Button);
	 break;
	  case B_FLATPUSHBUTTON:
	 draw_flat_push_button(Button);
	 break;
	  case B_ROUNDRADIOBUTTON:
	 draw_round_radio_button(Button);
	 break;
      default:
	break;
    }
}

static void draw_buttons(BUTTON *Buttons,int SizeButtonT)
/*----------------------------------------------------*/
{
  int i ;

  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
  setwritemode(COPY_PUT);
  for(i=0;i<SizeButtonT;i++)
  {
    Draw_Button(&(Buttons[i]));
  }
}

void draw_button(BUTTON *Button)
/*----------------------------------------------------*/
{
    setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
    setwritemode(COPY_PUT);
    Draw_Button(Button);
}

void redraw_button(BUTTON* Button)
/*----------------------------------------------------*/
{
	show_mouse(NULL);
	setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
	setwritemode(COPY_PUT);
	Draw_Button(Button);
	show_mouse(screen);
}

static int find_button(BUTTON *Buttons,int SizeButtonT, int *ib)
/*------------------------------------------------------------*/
{
  int i, ret=-1;
  int x0, y0, x1, y1, x2, y2;

  *ib=-1;

  for(i=SizeButtonT-1; i>=0; i--)
  {
    if(Buttons [i].enable == FALSE)
    {
	continue;
    }

    if(Buttons [i].flags & BUTTON_HIDDEN)
    {
      continue;
    }
	if (Buttons[i].type == B_ROUNDRADIOBUTTON) 
	{ 
	  SIZE_ROUNDRADIOBUTTON = (int)((float)SIZE_ROUNDRADIOBUTTON0*((float)WIDTH / 8.0));

	  x0 = jed_to_piks_x(Buttons[i].x) + pocz_x + SIZE_ROUNDRADIOBUTTON - WIDTH * WIDTH / 18;
	  y0 = jed_to_piks_y(Buttons[i].y) + pocz_y - SIZE_ROUNDRADIOBUTTON - HEIGHT * HEIGHT / 120;

	  x1 = x0 - SIZE_ROUNDRADIOBUTTON;
	  x2 = x0 + SIZE_ROUNDRADIOBUTTON;

      y1 = y0 - SIZE_ROUNDRADIOBUTTON;
	  y2 = y0 + SIZE_ROUNDRADIOBUTTON;
	}
	else
	{
    x1 = jed_to_piks_x(Buttons[i].x)+pocz_x;
    x2 = jed_to_piks_x(Buttons[i].x + Buttons[i].dx - 1)+pocz_x;
	y1 = jed_to_piks_y(Buttons[i].y)+pocz_y;
    y2 = jed_to_piks_y(Buttons[i].y + Buttons[i].dy - 1)+pocz_y;
	}
    
    if(PozX > x1 && PozX < x2 &&
	 PozY > y1 && PozY < y2)
    {
      break;
    }
  }

  if (i >= 0)
  {
    cur_off(PozX, PozY); 
    switch(Buttons[i].type)
    {
      case B_PUSHBUTTON:
	  case B_FLATPUSHBUTTON:
	   Buttons[i].check = 1;
	   select_mouse_cursor(MOUSE_CURSOR_NONE);
	   Draw_Button(&(Buttons[i]));
	   select_mouse_cursor(MOUSE_CURSOR_ALLEGRO);
	   delay(200);
	   Buttons[i].check = 0;
	   select_mouse_cursor(MOUSE_CURSOR_NONE);
	   Draw_Button(&(Buttons[i]));
	   select_mouse_cursor(MOUSE_CURSOR_ALLEGRO);
	   delay(50);
	   break;
      case B_CHECKBOX:
	   if(Buttons[i].check) Buttons[i].check = 0;
	   else Buttons[i].check = 1;
	   select_mouse_cursor(MOUSE_CURSOR_NONE);
	   Draw_Button(&(Buttons[i]));
	   select_mouse_cursor(MOUSE_CURSOR_ALLEGRO);
	   break;
      case B_RADIOBUTTON:
	  case B_ROUNDRADIOBUTTON:
	   break;
     default:
	   break;
    }
    moveto(PozX+pocz_x, PozY+pocz_y);
    cur_on(PozX, PozY);  
    ret = Buttons[i].id;
    *ib=i;
  }
 return ret;
}

void draw_pushed_button(BUTTON *Button)
 {
  Button->check = 1;
  Draw_Button(Button);
  delay(150);
  Button->check = 0;
  Draw_Button(Button);
  delay(25);
 }


void Draw_Slider(SLIDER *Slider)
{  int ret;
    SLIDER Slider_;
    int x1, y1, x2, y2;

    if (!(Slider->flags & 0xF0)) {

        memmove(&Slider_, Slider, sizeof(SLIDER));

        Slider_.fg = palette_color[Slider_.fg /*kolory.inkm*/];
        Slider_.bg = palette_color[Slider_.bg /*kolory.paperk*/];

        //adopt dimensions
        Slider_.x = jed_to_piks_x(Slider->x) + pocz_x;
        Slider_.y = jed_to_piks_y(Slider->y) + pocz_y;
        Slider_.w = jed_to_piks_x(Slider->w);
        Slider_.h = jed_to_piks_y(Slider->h);
		Slider_.slider = Slider;

        ret = Slider->proc(MSG_DRAW, &Slider_, '\0');
    }
}


static int find_slider(SLIDER *Slider,int SizeSliderT)
/*------------------------------------------------------------*/
{ int i, ret=0;
    SLIDER Slider_;
    SLIDER *slider;

    gui_set_screen(screen);
    for(i=0;i<SizeSliderT;i++)
    {
        slider=&Slider[i];
        if (slider->flags & 0xF0) continue;
        memmove(&Slider_, &Slider[i], sizeof(SLIDER));

        Slider_.fg=palette_color[Slider_.fg /*kolory.inkm*/];
        Slider_.bg=palette_color[Slider_.bg /*kolory.paperk*/];

        //adopt dimensions
        Slider_.x = jed_to_piks_x(Slider->x) + pocz_x;
        Slider_.y = jed_to_piks_y(Slider->y) + pocz_y;
        Slider_.w = jed_to_piks_x(Slider->w);
        Slider_.h = jed_to_piks_y(Slider->h);
		Slider_.slider = Slider;

        if(PozX > Slider_.x && PozX < (Slider_.x + Slider_.w) &&
              PozY > Slider_.y && PozY < (Slider_.y + Slider_.h))
            {
                break;
            }
    }

    if (i<SizeSliderT)
    {
        ret = Slider[i].proc(MSG_CLICK, &Slider_, '\0');
        Slider[i].d2 = Slider_.d2;
    }

    return ret;
}


static void draw_sliders(SLIDER *Slider,int SizeSliderT, BITMAP *dialog_screen) //, int caption)
/*----------------------------------------------------------------------------------------*/
{
    int i ;


    gui_set_screen(dialog_screen);
    setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
    setwritemode(COPY_PUT);

    gui_fg_color = palette_color[kolory.inkm];
    //gui_mg_color = makecol(128, 128, 128);
    gui_mg_color = palette_color[180 /*kolory.inkk*/];
    //gui_bg_color = makecol(200, 240, 200);
    //gui_bg_color = makecol(200, 200, 200);
    gui_bg_color = palette_color[98 /*kolory.paperk*/];

    gui_border_dark = palette_color[8];
    gui_border_light = palette_color[15];

    for(i=0;i<SizeSliderT;i++)
    {
        Draw_Slider(&(Slider[i]));
    }
}

static void draw_images(IMAGE *Images,int SizeImageT, PTMENU *tipsmenu)
/*------------------------------------------------------------------*/
{ 
  int i,x01,y01,x1,y1,x2,y2;

  BITMAP *bmp, *buffer, *sec_screen;

  int x02, y02;
  BITMAP *screenplay=Get_Screenplay();
   
   for(i=0;i<SizeImageT;i++)
   {

      if (Images[i].iconno>0) {

          bmp = (BITMAP *) get_icon(Images[i].iconno);

          x01 = jed_to_piks_x(Images[i].x1) + pocz_x0;
          y01 = jed_to_piks_y(Images[i].y1) + pocz_y0 + 2;

          x1 = jed_to_piks_x(Images[i].x1) + pocz_x;
          y1 = jed_to_piks_y(Images[i].y1) + pocz_y + 2;
          x2 = jed_to_piks_x(Images[i].x2);
          y2 = jed_to_piks_x(Images[i].y2);

          if (bmp != NULL) {
              if (global_dialog_ptr!=Get_instruct_dlg()) {
                  blit(bmp, screenplay, 0, 0, x1, y1, x2, y2);
              }
              else
              {
                  x02 = bmp->w;
                  y02 = bmp->h;

                  float wsp_sk=max(2.0,(float)y02/ 62.0f);
                  x02 = (int)((float)x02*wsp_sk);
                  y02 = (int)((float)y02*wsp_sk);

                  buffer = create_bitmap_ex(32, x2, y2 + 1);

                  if (buffer != NULL)
                  {
                      clear_to_color(buffer, bitmap_mask_color(buffer));
                      rotate_scaled_sprite(buffer, bmp, 0, 0, ftofix(0.0), ftofix((double)y2 / (double)y02));
                      set_alpha_blender();
                      draw_trans_sprite(screenplay, buffer, x1, y1);
                      destroy_bitmap(buffer);
                      set_trans_blender(0, 0, 0, TRANSLUCENCY);
                      set_mode_solid();
                  }
              }
          }
      }
	  else
	  {
		  x01 = 0; y01 = 0;
	  }
	  //filing timpsmenu
	  if (tipsmenu != NULL)
	  {
		  ////sprintf(fortip, "@%d;%d$%s", x1 + Images[i].x2, y1 + Images[i].y2, Images[i].txt);
		  ////strcpy((*(tipsmenu->pola))[i].txt, fortip);

		  //strncpy((*(tipsmenu->pola))[i].txt, Images[i].txt, POLE_TXT_MAX-1);
	  	  //(*(tipsmenu->pola))[i].txt[POLE_TXT_MAX-1]='\0';
          (*(tipsmenu->pola))[i].txt=Images[i].txt;
		  fortips_map.x1[i] = x01;
		  fortips_map.y1[i] = y01;
		  fortips_map.x2[i] = x01 + Images[i].x2;
		  fortips_map.y2[i] = y01 + Images[i].y2;
	  }

   }

} 

 void draw_dlg(TDIALOG *dlg, int typ, PTMENU *tipsmenu, BITMAP *dialog_screen, RECT *dialog_rect)
/*-----------------------------------------------------------------------------------*/
{
	////show_mouse(NULL);
	//select_mouse_cursor(MOUSE_CURSOR_NONE);

    Set_Screenplay(dialog_screen);

  if(dlg->Groups != NULL) draw_groups(*(dlg->Groups),dlg->SizeGroupT, dlg, FALSE);
  if(dlg->Labels != NULL) draw_labels(*(dlg->Labels),dlg->SizeLabT);
  if(dlg->InputLines != NULL) draw_input_lines(*(dlg->InputLines),dlg->SizeInLinT);
  if(dlg->ComboBoxes != NULL) draw_combo_boxes(*(dlg->ComboBoxes),dlg->SizeComboBoxT);
  if(dlg->ListBoxes != NULL) draw_list_boxes(*(dlg->ListBoxes),dlg->SizeListBoxT);
  if(dlg->Lines != NULL) draw_lines(*(dlg->Lines),dlg->SizeLinT);
  if(dlg->Images != NULL) draw_images(*(dlg->Images),dlg->SizeImageT, tipsmenu);
  if (dlg->Buttons != NULL) draw_buttons(*(dlg->Buttons), dlg->SizeButtonT);

  if (dlg->Sliders != NULL) draw_sliders(*(dlg->Sliders), dlg->SizeSliderT, dialog_screen); //, dlg_kolory->dlg_caption);

  show_mouse(NULL); ////

  blit(dialog_screen, screen, 0, 0, (int)dialog_rect->left, (int)dialog_rect->top, (int)(dialog_rect->right-dialog_rect->left), (int)(dialog_rect->bottom-dialog_rect->top));
  destroy_bitmap(dialog_screen);

  Set_Screenplay(screen);

  acquire_screen();
#ifdef LINUX
  show_x_cursor();
#endif
  show_mouse(screen); ////
  release_screen();
  //select_mouse_cursor(MOUSE_CURSOR_ALLEGRO);


  pocz_x=pocz_x0;
  pocz_y=pocz_y0;

  if (dlg->Groups != NULL) draw_groups(*(dlg->Groups), dlg->SizeGroupT, dlg, TRUE);
}

void draw_transparent_dlg(TDIALOG *dlg, int typ, PTMENU *tipsmenu, RECT *dialog_rect)
/*-----------------------------------------------------------------------------------*/
{
    if(dlg->Images != NULL) draw_images(*(dlg->Images),dlg->SizeImageT, tipsmenu);
    if (dlg->Groups != NULL) draw_groups(*(dlg->Groups), dlg->SizeGroupT, dlg, TRUE);
    if (dlg->Buttons != NULL) draw_buttons(*(dlg->Buttons), dlg->SizeButtonT);

    ////form MacOS aa64 22-08-2025
	acquire_screen();
#ifdef LINUX
	show_x_cursor();
#endif
	show_mouse(screen); ////
	release_screen();
}

 void draw_dlg_lab(TDIALOG *dlg)
	 /*---------------------------------*/
 {
	 scare_mouse();
	 if (dlg->Labels != NULL) redraw_labels(*(dlg->Labels), dlg->SizeLabT);
	 unscare_mouse();
	 cur_on(PozX, PozY);
 }

 void draw_dlg_combobox(TDIALOG *dlg)
	 /*---------------------------------*/
 {
	 scare_mouse();
	 if (dlg->ComboBoxes != NULL) draw_combo_boxes(*(dlg->ComboBoxes), dlg->SizeComboBoxT);
	 unscare_mouse();
	 cur_on(PozX, PozY);
 }

 void draw_dlg_prev(TDIALOG *dlg)
	 /*------------------------*/
 {

	 my_scare_mouse();

	 if (dlg->Labels != NULL) draw_labels(*(dlg->Labels), dlg->SizeLabT);
	 if (dlg->InputLines != NULL) draw_input_lines(*(dlg->InputLines), dlg->SizeInLinT);

	 if (dlg->Buttons != NULL) draw_buttons(*(dlg->Buttons), dlg->SizeButtonT);

	 if (dlg->Groups != NULL) draw_groups(*(dlg->Groups), dlg->SizeGroupT, dlg, TRUE);
	 my_unscare_mouse();
	 cur_on(PozX, PozY);
 }


static void open_dlg(TDIALOG *dlg, char typ, BITMAP **dialog_screen, RECT *dialog_rect)
/*----------------------------------------------------------------------------------*/
{
	int x01, y01, x02, y02;
    int x1, y1, x2, y2, border,caption, ink_caption;
	int x1c, x2c, y1c, y2c;
    int x0, y0;
	BITMAP* bitmap_ptr;
	BITMAP* bitmap;
    int w,h;
    BITMAP *dialog_screen_ptr;

	char pattern[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

	int max_field_x, max_field_y;


	border = dlg->border == COLOR_NULL ? dlg_kolory->dlg_border : dlg->border;
	caption = dlg->caption == COLOR_NULL ? dlg_kolory->dlg_caption : dlg->caption;
	ink_caption = dlg->ink_caption == COLOR_NULL ? dlg_kolory->dlg_ink_caption : dlg->ink_caption;

	max_field_x = getmaxx();
	max_field_y = getmaxy();

	wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
	wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);

	Get_Dlg_Rect(dlg, &x01, &y01, &x02, &y02);

	if (x01 < 0) x01 = 0;
	if (y01 < 0) y01 = 0;

	if (x02 > getmaxx()) x02 = getmaxx();
	if (y02 > getmaxx()) y02 = getmaxy();

	if (typ == 0)
    {
		//position_mouse((x02 - x01) / 2, (y02 - y01) / 2);
        dlg->xb = x01;
        dlg->yb = y01;
        bitmap = create_bitmap(x02 - x01, y02 - y01);
        dlg->back = (char *) bitmap;
        getimage(x01, y01, x02, y02, dlg->back);
        if (strcmp(dlg->txt, "^") != 0) {
            bitmap_ptr = create_bitmap(getmaxx(), getmaxy());
            dlg->background = (char *) bitmap_ptr;
            getimage(0, 0, getmaxx(), getmaxy(), dlg->background);
        }
    }
       //if sliders exist
       for (int i=0; i<dlg->SizeSliderT; i++)
       {
           int var1, var2, var3, var4, ret;
           BITMAP *slbitmap;
           SLIDER *slider=dlg->Sliders;

           if ((typ==0) || (slider[i].flags & 0x800))
           {
               slider[i].flags &= ~0x800;

               int (*SlideFun)(int *, int *, int *, int *);
               SlideFun = (int (*)(int *, int *, int *, int *)) slider[i].dp3;
               //int var1=d2;
               ret = SlideFun(&var1, &var2, &var3, &var4);

               //adjusting slider
               slider[i].d1 = var3 - (var2 - var1);

               int Slider_h = jed_to_piks_y(slider[i].h);
               double slratio = (double) Slider_h / (double) (var3); //-(var2-var1));
               int slbody;
               if (var3 <= var4) slbody = Slider_h;
               else slbody = (var2 - var1) * slratio;

               if (slider[i].dp != NULL) destroy_bitmap((BITMAP *) slider[i].dp);

               if (slbody<4) slider[i].flags |= 0xF0;  //hidden
               else
               {
                   slider[i].flags &= ~0xF0;
                   slbitmap = create_bitmap(jed_to_piks_x(slider[i].w), slbody);
                   //slider[i].d2=var3-(var2-var1)/2;
                   slider[i].d2 = var3 - var1 - (var2 - var1);

                   clear_to_color(slbitmap, get_palette_color(246/*kolory.paperm*/));
                   hline(slbitmap, 1, 1, slbitmap->w - 2, BLACK);
                   vline(slbitmap, slbitmap->w - 2, 1, slbitmap->h - 1, BLACK);
                   hline(slbitmap, slbitmap->w - 2, slbitmap->h - 1, 1, BLACK);
                   vline(slbitmap, 1, 1, slbitmap->h - 1, BLACK);

                   hline(slbitmap, 2, 2, slbitmap->w - 3, palette_color[15]);
                   vline(slbitmap, slbitmap->w - 3, 2, slbitmap->h - 2, palette_color[8]);
                   hline(slbitmap, slbitmap->w - 3, slbitmap->h - 2, 2, palette_color[8]);
                   vline(slbitmap, 2, 2, slbitmap->h - 2, palette_color[15]);

                   slider[i].dp = slbitmap;
               }
           }
       }


  dialog_rect->left=x01;
  dialog_rect->top=y01;
  dialog_rect->right=x02;
  dialog_rect->bottom=y02;

  w=x02-x01;
  h=y02-y01;
  x1=0;
  y1=0;
  x2=w;
  y2=h;
  dialog_screen_ptr=create_bitmap_ex(32,w, h);
  if (dialog_screen_ptr==NULL) return;


  clear_to_color(dialog_screen_ptr, get_palette_color(kolory.paper));

  *dialog_screen=dialog_screen_ptr;

  Set_Screenplay(dialog_screen_ptr);

  setcolor(border);
  setwritemode(COPY_PUT);
  setlinestyle1(SOLID_LINE,0,THICK_WIDTH);

  rectangle(x1+2,y1+2, x2-2, y2 -2);
  setfillstyle_(SOLID_FILL,caption);
  x1c = x1+4;
  y1c = y1+4;
  x2c = x2-4;
  y2c = y1+ 7 + ED_INF_HEIGHT ;
  bar(x1c, y1c, x2c, y2c + 1);
  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
  setcolor(BORDER_LIGHT);
  rectangle(x1c-1, y1c-1, x2c+1, y2c+1);
  setcolor(ink_caption);

  x0 =(x1c + x2c) / 2;

  y0 = y1c + 2;

  moveto(x0 - TTF_text_len(dlg->txt) / 2, y0);
 
  settextjustify(LEFT_TEXT, TOP_TEXT);
  i__x1w = x1 + 3;
  i__x2w = x2 - 3;
  i__y1w = y1 + 7 + ED_INF_HEIGHT ;
  i__y2w = y2 - 3;
  
  setfillpattern_(pattern, kolor_w);

  setfillstyle_(SOLID_FILL,kolor_w);

  bar(i__x1w,i__y1w,i__x2w,i__y2w); 

  pocz_x=i__x1w;
  pocz_y=i__y1w;
  Xmax_ = i__x2w - i__x1w;
  Ymax_ = i__y2w - i__y1w;
  DX = i__x1w - 1;
  DY = i__y1w - 1;


    pocz_x0=x01 + 3;
    pocz_y0=y01 + 7 + ED_INF_HEIGHT ;

  outtext_r_(dialog_screen_ptr, dlg->txt);

    x001[typ]=x01;
    y001[typ]=y01;
    x002[typ]=x02;
    y002[typ]=y02;
}

static void open_transparent_dlg(TDIALOG *dlg, char typ, RECT *dialog_rect)
/*-----------------------------------------------------------------------*/
{
	int x1, y1, x2, y2, border,caption, ink_caption;
	static int x1c, x2c, y1c, y2c;
    int x0, y0;
    int w,h;

	char pattern[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

	int max_field_x, max_field_y;

	border = dlg->border == COLOR_NULL ? dlg_kolory->dlg_border : dlg->border;
	caption = dlg->caption == COLOR_NULL ? dlg_kolory->dlg_caption : dlg->caption;
	ink_caption = dlg->ink_caption == COLOR_NULL ? dlg_kolory->dlg_ink_caption : dlg->ink_caption;

	max_field_x = getmaxx();
	max_field_y = getmaxy();

	wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
	wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);

	Get_Dlg_Rect(dlg, &x1, &y1, &x2, &y2);

	dlg->xb = x1;
	dlg->yb = y1;

    if (strcmp(dlg->txt, "Dim")!=0)
    {

        dlg->xb = 0;
        dlg->yb = 0;

        dlg->back = create_bitmap(getmaxx(), getmaxy());
        getimage(0, 0, getmaxx(), getmaxy(), dlg->back);

        dlg->background = create_bitmap(getmaxx(), getmaxy());
        getimage(0, 0, getmaxx(), getmaxy(), dlg->background);

    }

    dialog_rect->left=x1;
    dialog_rect->top=y1;
    dialog_rect->right=x2;
    dialog_rect->bottom=y2;

	x1c = x1 + 4;
	y1c = y1 + 4;
	x2c = x2 - 4;
	y2c = y1 + 7 + ED_INF_HEIGHT;

	x0 = (x1c + x2c) / 2;
	y0 = y1c + 2;

	i__x1w = x1 + 3;
	i__x2w = x2 - 3;
	i__y1w = y1 + 7 + ED_INF_HEIGHT;
	i__y2w = y2 - 3;

    pocz_x=i__x1w;
    pocz_y=i__y1w;
    Xmax_ = i__x2w - i__x1w;
    Ymax_ = i__y2w - i__y1w;
    DX = i__x1w - 1;
    DY = i__y1w - 1;

}

void Close_Dlg(TDIALOG *dlg, char typ)
/*---------------------------------*/
{

  setviewport(0, 0, getmaxx(),getmaxy(), 1);
  if (typ==1) {
      if (strcmp(dlg->txt, "Dim")!=0) {

          if (strcmp(dlg->txt, "^")!=0) {
              if (dlg->background != NULL) putimage(0, 0, dlg->background, COPY_PUT); //restore background
              if (dlg->back != NULL) destroy_bitmap(dlg->back);
              dlg->background = NULL;
          }
          else
          {
              if (dlg->back != NULL) putimage(dlg->xb, dlg->yb, dlg->back, COPY_PUT); //restore background
          }
          dlg->back=NULL;
      }
  }

  okno_r();
  my_sleep(0);
  okno_all();
  lock_functions=FALSE;
}

void Redraw_Dlg(TDIALOG *dlg)
{ RECT dialog_rect;
  BITMAP *dialog_screen;

  open_dlg(dlg, 3, &dialog_screen, &dialog_rect);
  draw_dlg(dlg, 3, NULL, dialog_screen, &dialog_rect);
}

static void cur_off(int x,int y)
/*------------------------------*/
{
  Hide_Mouse() ;
  ini_cursor=FALSE;

}

static void cur_on(int x,int y)
/*------------------------------*/
{
  Show_Mouse() ;
  ini_cursor = TRUE;
}

static void cur_move(int x,int y)
/*------------------------------*/
{
  Move_Mouse(x, y) ; 

}

static void mv_cur( int dx , int dy )
{
	PozX = mouse_x;
	PozY = mouse_y;
}

static int cur_l(void)
/*------------------*/
{ mv_cur(-krok,0);
  return 0;
}

static int cur_r(void)
/*--------------------*/
{ mv_cur(krok,0);
  return 0;
}

static int cur_t(void)
/*------------------*/
{ mv_cur(0,krok);
  return 0;
}

static int cur_b(void)
/*--------------------*/
{ mv_cur(0,-krok);
  return 0;
}

static int next_sel()
/*-------------------*/
{return 0;}

void dialog_cursor(int on) {

    switch (on) {
        case 0:
            KLS=kls;
            MVCUR=mv;
            TABFUN=tab_fun;
            break;
        case 1:
            kls=KLS;          KLS=noop1;
            mv=MVCUR;         MVCUR=mv_cur;
            tab_fun=TABFUN;    TABFUN=next_sel;
            break;
    }
}

static void init(char typ, TDIALOG *Dlg, PTMENU *tipsmenu)
/*-----------------------------------------------------*/
{
  RECT dialog_rect;
  BITMAP *dialog_screen;

  static int(*kls)(void);
  static void(*mv)(int x,int y);
  static int(*tab_fun)(void);

  int n1;

  lock_functions=TRUE;
  switch (typ)
  {
  case 0:
  case 3:
	  //show_mouse(NULL);
	  //disable_hardware_cursor();
	  now_is_dialog = 1;
	  ptrs__dlg = Dlg;
	  if ((Dlg->flags == FLDLG_NOIMAGE) || (Dlg->flags & 0x40))
	  {    /*tylko konwersja starych blokow i zbiorow*/
		  open_dlg(Dlg, typ, &dialog_screen, &dialog_rect);
		  draw_dlg(Dlg, typ, tipsmenu, dialog_screen, &dialog_rect);
		  find_groups = FALSE;
	  }
	  else
	  {
		  open_transparent_dlg(Dlg, typ, &dialog_rect);
		  draw_transparent_dlg(Dlg, typ, tipsmenu, &dialog_rect);
		  find_groups = TRUE;
	  }

	  if ((Dlg->flags & 0x40))
	  {
		  PozX = jed_to_piks_x(Dlg->dx / 2)  + pocz_x;
		  PozY = jed_to_piks_y(Dlg->dy / 2)  + pocz_y;
		  if (Dlg->pozx == 0 && Dlg->pozy == 0)
		  {
			  PozX = jed_to_piks_x(Dlg->dx / 2) + pocz_x;
			  PozY = jed_to_piks_y(Dlg->dy / 2) + pocz_y;
		  }
      }

	  if  (Dlg->flags & 0x80)
	  {
		  PozX = mouse_x;
		  PozY = mouse_y;
	  }
	
         //Ini_Mouse_Cursor(PozX,PozY) ;
		
	    //moveto(PozX, PozY);
        ////cur_on(PozX,PozY);
#ifdef ALLEGRO5
#ifdef MACOS
  		MoveOSXCursor(PozX,PozY);
#else
  		  position_mouse_xy(PozX,PozY);

          Ini_Mouse_Cursor(PozX,PozY) ;

          //moveto(PozX, PozY);
          cur_on(PozX,PozY);

#endif
#else
  		//position_mouse(PozX,PozY);
        Ini_Mouse_Cursor(PozX,PozY) ;

        //moveto(PozX, PozY);
        cur_on(PozX,PozY);
#endif
  		set_forget_mouse(PozX,PozY);
#ifdef LINUX
  		sleep(0);
#else
  	_sleep(0);
#endif

         SW01 = SERV[84];
         for(n1 = 0; n1 < SVMAX-71 ; n1++)
         {
          SW [n1] = SERV [n1+71];
          SERV [n1+71] = noop1;
         }
         for(n1 = 0; n1 <10; n1++)
         {

          SW [110+n1] = SERV [59+n1];
          SERV [59+n1] = noop1;
         }

          SW [120] = SERV [47];

          SERV [47] = noop1;

	  
	  SERV[72]=cur_t;
	  SERV[75]=cur_l;
	  SERV[77]=cur_r;
	  SERV[80]=cur_b;
	  SERV[82]=noop1;  /*ins*/

      SW[121]=SERV[135];
      SW[122]=SERV[134];


     if(strcmp(Dlg->txt, _LAYERS_)==0)
       {
         SERV[72]=up_layer_;
         SERV[80]=dn_layer_;
         SERV[73]=pgup_layer_;
         SERV[81]=pgdn_layer_;

         SERV[135] = (void*)dn_layer_;
         SERV[134] = (void*)up_layer_;
       }

     SERV[84]=SW01;

	  kls=KLS;          KLS=noop1;
	  mv=MVCUR;         MVCUR=mv_cur;
	  tab_fun=TABFUN;    TABFUN=next_sel;
	  break;
	 case 1 :
	 //case 2 :


         //if sliders exist
         for (int i=0; i<Dlg->SizeSliderT; i++)
         {
             //BITMAP *slbitmap;
             SLIDER *slider = Dlg->Sliders;

             if (slider[i].flags & 0xF0) continue;

             if (slider[i].dp!=NULL) destroy_bitmap((BITMAP*)slider[i].dp);
             if (slider[i].dp=NULL);
         }
      case 2 :

	  cur_off(PozX, PozY) ; 
	  ClearErr() ;
	  Close_Dlg(Dlg, typ) ;

	  SERV[4] = SW2[0];   //Ctrl-D
	  SERV[5] = SW2[1];          //E
	  SERV[14] = SW2[2];      //N
	  SERV[13] = SW2[3];         //M
	  SERV[9] = SW2[4];     //I
	  SERV[3] = SW2[5];         //C
	  SERV[16] = SW2[6];     //P
	  SERV[20] = SW2[7];         //T
	  SERV[2] = SW2[8];      //A
	  SERV[15] = SW2[9];        //O
	  SERV[24] = SW2[10];    //X
	  SERV[25] = SW2[11];     //Y
	  SERV[12] = SW2[12];    //L
	 

	  for(n1 = 0 ;n1 < SVMAX-71 ; n1++)
          {
           SERV [n1+71] = SW [n1];
          } 

          SERV [47] = SW [120];
          
	 for(n1 = 0; n1 <10; n1++)
         {

          SERV [59+n1] = SW [110+n1];
         }


          SERV[134] = (void*)SkalaZ_Minus;
          SERV[135] = (void*)SkalaZ_Plus;

      GrMouseEraseCursor();

	  lock_mouse();

	  KLS=kls;
	  MVCUR=mv;
	  TABFUN=tab_fun;
	  now_is_dialog=0;
	  break;
   }
}

void get_size_and_disable_F11(int dim)
{
    int ret;

    disable_F11();
}

void check_size_and_enable_F11(int dim)
{   int curr_x01, curr_y01, curr_h1, curr_v1;
    int ret;

    enable_F11();

    if (get_close_button_pressed()) Koniec();
}

// Function to set the kth bit of n
void setBit(unsigned char *n, int k)
{
    *n |= (1 << (k - 1));
}

// Function to clear the kth bit of n
void clearBit(unsigned char *n, int k)
{
    *n &= (~(1 << (k - 1)));
}


int Dialog_in_dialog(TDIALOG *dlg)
{  EVENT *ev;
    int id, ib, id_h;
    int x1, y1, x2, y2;
    BITMAP *bitmap_ptr;
    BITMAP* bitmap;
    int pocz_x1, pocz_y1;
    int ret=0;
	int mx, my;

    pocz_x1=pocz_x;
    pocz_y1=pocz_y;


    PPOLE	pmTipsMenu[] = {
            {u8"@",' ',0,NULL},
            {u8"",' ',0,NULL},
    };

    PTMENU tipsmenu = { 1, 0, 0, 32, 0, 0, 0x100, CMNU, CMBR, CMTX, 0, 0, 0,0,0,(PPOLE(*)[])&pmTipsMenu, NULL, NULL };

    tipsmenu.poz = 0;
    tipsmenu.max = dlg->SizeImageT;


	mx = mouse_x;
	my = mouse_y;
	
#ifndef LINUX
    select_mouse_cursor(MOUSE_CURSOR_NONE);
#endif
	show_mouse(NULL);

    Get_Dlg_Rect(dlg, &x1, &y1, &x2, &y2);

    dlg->xb = x1; //0
    dlg->yb = y1; //0;
    bitmap = create_bitmap(x2 - x1, y2 - y1);
    dlg->back = (char *)bitmap;
    getimage(x1, y1, x2, y2, dlg->back);

    bitmap_ptr = create_bitmap(getmaxx(), getmaxy());
    dlg->background = (char *)bitmap_ptr;
    getimage(0, 0, getmaxx(), getmaxy(), dlg->background);

    Redraw_Dlg(dlg);

#ifndef LINUX
    select_mouse_cursor(MOUSE_CURSOR_ALLEGRO);
#endif
	show_mouse(screen);

    int ended=0;
    while (!ended)
    {
        ev = GetEvent(&tipsmenu);
        switch (ev->What)
        {
            case evKeyDown  :
                if ((ev->Number == 0) ||
                (((strcmp(dlg->txt, u8"Uwaga")==0) && ((ev->Number == 'N') || (ev->Number == 'n'))) ||
                 ((strcmp(dlg->txt, u8"Notice") == 0) && ((ev->Number == 'N') || (ev->Number == 'n'))) ||
                 ((strcmp(dlg->txt, u8"La noticia") == 0) && ((ev->Number == 'N') || (ev->Number == 'n'))) ||
                 ((strcmp(dlg->txt, u8"Увага") == 0) && ((ev->Number == L'Н') || (ev->Number == L'н')))))
                {
                    ret=0;
                    ended=1;
                    break;
                }
                else if (ev->Number == ENTER) //&& (-1 != (id = find_button(*(dlg->Buttons), dlg->SizeButtonT, &ib))))
                {
                    if (-1 != (id = find_button(*(dlg->Buttons), dlg->SizeButtonT, &ib)))
                    {
                        ret = id;
                        ended = 1;
                        break;
                    }
                    else if (-1 != (id_h = find_header(dlg, &tipsmenu, 3)))
                    {
                        ;
                    }
                }
                else if  (((strcmp(dlg->txt, u8"Uwaga")==0) && ((ev->Number == 'T') || (ev->Number == 't'))) ||
                          ((strcmp(dlg->txt, u8"Notice") == 0) && ((ev->Number == 'Y') || (ev->Number == 'y'))) ||
                          ((strcmp(dlg->txt, u8"La noticia") == 0) && ((ev->Number == 'S') || (ev->Number == 's'))) ||
                          ((strcmp(dlg->txt, u8"Увага") == 0) && ((ev->Number == L'Т') || (ev->Number == L'т'))))
                {
                    ret=1;
                    ended=1;
                    break;
                }
            break;
        }
    }


    select_mouse_cursor(MOUSE_CURSOR_NONE);
	show_mouse(NULL);

    if (dlg->background!=NULL)
        putimage(0, 0, dlg->background, COPY_PUT);
    //if (dlg->back!=NULL)
    //    putimage(0, 0, dlg->back, COPY_PUT);
    destroy_bitmap(dlg->back);
    dlg->back=NULL;
    dlg->background=NULL;

    select_mouse_cursor(MOUSE_CURSOR_ALLEGRO);
	show_mouse(screen);

    pocz_x=pocz_x1;
    pocz_y=pocz_y1;
    return ret;
}

void reset_cursor_pos(void) {
#ifdef ALLEGRO5
    int WspX_, WspY_;

    return;

#ifdef ALLEGRO5
#ifdef MACOS
  		//MoveOSXCursor(mouse_x0, mouse_y0);
        MoveOSXCursor(getmaxx()/2, getmaxy()/2);
#else
  		//position_mouse(mouse_x0, mouse_y0);
        position_mouse(getmaxx()/2, getmaxy()/2);
#endif
#else
  		//position_mouse(mouse_x0, mouse_y0);
          position_mouse(getmaxx()/2, getmaxy()/2);
#endif
    //do {
    //    get_mouse_mickeys(&WspX_, &WspY_);
    //} while ((WspX_ != 0) || (WspY_ != 0));
	//set_forget_mouse(mouse_x0, mouse_y0);
    set_forget_mouse(getmaxx()/2, getmaxy()/2);
#endif
    my_sleep(10);
}

int Dialog(TDIALOG *dlg, DLG_COLOR *kolory, int(*fun)(int), BOOL m)
/*-------------------------------------------------------------------*/
{
	int n, id, id_b, id_lb, id_cb, id_sl, id_lsl, id_il, id_gb, id_h;
	EVENT *ev;
	BOOL ret_LMB = TRUE;
	int kk, i;
	int ret, res_ret;
	BOOL dialog_window_was_resized;
	int x_win_orig, y_win_orig, win_width, win_height;
	int cur_width, cur_height;
	int init_option = 0;
    int dim=1;
    int ib;
    BUTTON block_flag_button={ 0, 0, DXHBut, DYHBut, COLOR_NULL, COLOR_NULL, COLOR_NULL, "", 0,B_CHECKBOX, 0, 1,0, 0};

    block_changed=FALSE;

#ifdef ALLEGRO5
    mouse_x0=mouse_x;
    mouse_y0=mouse_y;
#endif


    if (strcmp(dlg->txt,"Dim")==0) dim=0;

    get_size_and_disable_F11(dim);

	global_dialog_flag = 1;
	strncpy(&global_dialog_name, dlg->txt, 15);
    global_dialog_name[15]='\0';

    global_dialog_ptr=(char*)dlg;

	if (dlg->process != NULL)
	{
		ProcFG = dlg->process;

	}
	else ProcFG = NULL;

	dialog_window_was_resized = FALSE;
	//_free_mouse();
    free_mouse();


	if (GFX_WIN == 1)
	{
		ret = get_window_origin_and_size(&x_win_orig, &y_win_orig, &win_width, &win_height);
		res_ret = get_desktop_resolution(&cur_width, &cur_height);
	}

	was_dialog = TRUE;

continue2:
	//m = m;
	dlg_kolory = kolory;

	//POLE	pmTipsMenu[] = {
    PPOLE pmTipsMenu[] = {
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
	{u8"",' ',0,NULL},
    {u8"",' ',0,NULL},
    {u8"",' ',0,NULL},
    {u8"",' ',0,NULL},
    {u8"",' ',0,NULL},
    {u8"",' ',0,NULL},
    {u8"",' ',0,NULL},
	};

	////TMENU tipsmenu = { 1, 0, 0, 32, 0, 0, 0, CMNU, CMBR, CMTX, 0, 0, 0,0,0,(POLE(*)[])&pmTipsMenu, NULL, NULL };
    PTMENU tipsmenu = { 1, 0, 0, 32, 0, 0, 0x100, CMNU, CMBR, CMTX, 0, 0, 0,0,0,(PPOLE(*)[])&pmTipsMenu, NULL, NULL };

	tipsmenu.poz = 0;
	tipsmenu.max = dlg->SizeImageT;

    tipsmenu.xpcz=0; //this is an offset x and is initialized as 0
     tipsmenu.ypcz=0; //this is an offset y and is initialized as 0

    if (strstr(dlg->txt, BLOCKLIST)!=NULL)
    {
        int hbi = 0;
        //checking buttons
        for (i = 0; i < dlg->SizeButtonT; i++) {
            if (((*(dlg->Buttons))[i].txt != NULL) &&
                ((strlen((*(dlg->Buttons))[i].txt) > 0) && ((*(dlg->Buttons))[i].txt[0] == '\023')) &&
                (((BLOK *) ((*(dlg->Buttons))[i].adr + dane))->flag & 1) &&
                ((((*(dlg->Buttons))[i].flags & BUTTON_GROUP) == FALSE) &&
                 (((*(dlg->Buttons))[i].flags & BUTTON_FOLDER) == FALSE))) {

                memmove(&(*(dlg->Buttons))[dlg->SizeButtonT], &block_flag_button, sizeof(BUTTON));
                (*(dlg->Buttons))[dlg->SizeButtonT].x = (*(dlg->Buttons))[i].x + piks_x_to_jed(5);
                (*(dlg->Buttons))[dlg->SizeButtonT].y = (*(dlg->Buttons))[i].y + piks_y_to_jed(
                        4 - (int) ((((float) SIZE_CHECKBOX0 * 8.0 / (float) WIDTH) / 2.0) + 0.5)); // * wsp_y;
                (*(dlg->Buttons))[dlg->SizeButtonT].id = i + 1000; //(*(dlg->Buttons))[i].id+1000;
                if (((BLOK *) ((*(dlg->Buttons))[i].adr + dane))->flag & 2)
                    (*(dlg->Buttons))[dlg->SizeButtonT].check = 0;
                else (*(dlg->Buttons))[dlg->SizeButtonT].check = 1;


                dlg->SizeButtonT++;
                hbi++;
            }
        }
    }

	show_mouse(NULL);

	init(init_option, dlg, &tipsmenu);

	if (strcmp(dlg->txt, "^") == 0)
	{
		dlg->flags |= 0x40;
		init(1, dlg, NULL);
		dlg->flags &= 0x40;

		global_dialog_flag = 0;
		strncpy(global_dialog_name, "", 15);
        global_dialog_name[15]='\0';
        check_size_and_enable_F11(dim);
        reset_cursor_pos();
		return 0;
	}


  while(1)
  {
	ev = GetEvent((TMENU*)&tipsmenu); //NULL &dialog
    switch(ev->What)
    {
      case evKeyDown  :
      if(ev->Number == 0)
      { 
       if((dlg->SizeGroupT == 1) &&(dlg->SizeInLinT == 0))
       {
        if(ret_LMB==TRUE)
         {
          init(1,dlg, NULL);   //umozliwiono powrot po lewym klawiszu myszki
		  //przywrocenie trybu graficznego

		  if (GFX_WIN==1) 
		  {
		    if (dialog_window_was_resized==TRUE)
				{
					set_resized_window(win_width, win_height, 1);
					kk=set_window_origin(x_win_orig, y_win_orig);
				}
		  }
		  ////////////////////////////////

		  global_dialog_flag = 0;
		  strncpy(global_dialog_name, "",15);
          global_dialog_name[15]='\0';
          check_size_and_enable_F11(dim);
          reset_cursor_pos();
		  return 0;
         }
       }
	   else
         {
  	     init(1,dlg, NULL);
		 //przywrocenie trybu graficznego

		 ////////////////////////////////

		 global_dialog_flag = 0;
		 strncpy(global_dialog_name, "",15);
         global_dialog_name[15]='\0';
         check_size_and_enable_F11(dim);
         reset_cursor_pos();
       	 return 0;
         }	
      }
	  //Tak Yes
	   else if (((strcmp(dlg->txt, u8"Uwaga")==0) && ((ev->Number == 'T') || (ev->Number == 't'))) ||
		       ((strcmp(dlg->txt, u8"Notice") == 0) && ((ev->Number == 'Y') || (ev->Number == 'y'))) ||
		       ((strcmp(dlg->txt, u8"La noticia") == 0) && ((ev->Number == 'S') || (ev->Number == 's'))) ||
		       ((strcmp(dlg->txt, u8"Увага") == 0) && ((ev->Number == L'Т') || (ev->Number == L'т'))))
       {
        if((dlg->SizeGroupT == 1) &&(dlg->SizeInLinT == 0))
        {
         if(ret_LMB==TRUE)
         {
          init(1,dlg, NULL);   //umozliwiono powrot po lewym klawiszu myszki
		  //przywrocenie trybu graficznego

		  if (GFX_WIN==1) 
		  {
		    if (dialog_window_was_resized==TRUE)
				{
					set_resized_window(win_width, win_height, 1);
					kk=set_window_origin(x_win_orig, y_win_orig);
				}
		  }
		  ////////////////////////////////

		  global_dialog_flag = 0;
		  strncpy(global_dialog_name, "",15);
          global_dialog_name[15]='\0';
          check_size_and_enable_F11(dim);
          reset_cursor_pos();
		  return 2;
         }
        }
		else
         {
  	     init(1,dlg, NULL);
		 //przywrocenie trybu graficznego

		 ////////////////////////////////

		 global_dialog_flag = 0;
		 strncpy(global_dialog_name, "", 15);
         global_dialog_name[15]='\0';
         check_size_and_enable_F11(dim);
         reset_cursor_pos();
       	 return 0;
         }	
      }
		else if (((strcmp(dlg->txt, u8"Uwaga")==0) && ((ev->Number == 'N') || (ev->Number == 'n'))) ||
			     ((strcmp(dlg->txt, u8"Notice") == 0) && ((ev->Number == 'N') || (ev->Number == 'n'))) ||
			     ((strcmp(dlg->txt, u8"La noticia") == 0) && ((ev->Number == 'N') || (ev->Number == 'n'))) ||
			     ((strcmp(dlg->txt, u8"Увага") == 0) && ((ev->Number == L'Н') || (ev->Number == L'н'))) ||
			     ((strcmp(dlg->txt, u8"Dim") == 0) && ((ev->Number == 0) || (ev->Number == ENTER))))
        { 
        if((dlg->SizeGroupT == 1) &&(dlg->SizeInLinT == 0))
        {
         if(ret_LMB==TRUE)
         {
          init(1,dlg, NULL);   //umozliwiono powrot po lewym klawiszu myszki
		  //przywrocenie trybu graficznego

		  if (GFX_WIN==1) 
		  {
		    if (dialog_window_was_resized==TRUE)
				{
					set_resized_window(win_width, win_height, 1);
					kk=set_window_origin(x_win_orig, y_win_orig);
				}
		  }
		  ////////////////////////////////

		  global_dialog_flag = 0;
		  strncpy(global_dialog_name, "",15);
          global_dialog_name[15]='\0';
          check_size_and_enable_F11(dim);
          reset_cursor_pos();
		  return 10;
         }
        }
        else
         {
  	     init(1,dlg, NULL);
		 //przywrocenie trybu graficznego

		 ////////////////////////////////

		 global_dialog_flag = 0;
		 strncpy(global_dialog_name, "",15);
         global_dialog_name[15]='\0';
         check_size_and_enable_F11(dim);
         reset_cursor_pos();
       	 return 0;
         }	
      }
      if(ev->Number == ENTER)
       {
        if(strcmp(dlg->txt, "Edycja")==0) ret_LMB=FALSE;
       }
continue_lb:
      memmove(&dlg0,dlg,sizeof(TDIALOG));
	  id_b = 0;
	  if ((ev->Number == ENTER) &&
		  (
           (0 != (id = id_sl = find_slider(*(dlg->Sliders), dlg->SizeSliderT))) ||
		   (0 != (id = id_il = find_input_line(*(dlg->InputLines), dlg->SizeInLinT))) ||
		   (0 != (id = id_cb = find_combo_box(*(dlg->ComboBoxes), dlg->SizeComboBoxT))) ||
		   (0 != (id = id_lb = find_list_box(*(dlg->ListBoxes), dlg->SizeListBoxT))) ||
		   (-1 != (id = id_b = find_button(*(dlg->Buttons), dlg->SizeButtonT, &ib))) ||
		   ((find_groups) && (-1 != (id = id_gb = find_group(*(dlg->Groups), dlg->SizeGroupT)))) ||
           (0 != (id = id_h = find_header(dlg, &tipsmenu, 0)))
           ))
	  {
		  if (id == -2)  /* zmiana koloru >16 */
		  {
			  Error = FALSE;
			  Info = FALSE;
			  init_option = 3;
			  goto continue2;
		  }

          if (id == -3)  /* moving */
          {
            ;
          }


          if (id_b > 1000)
          {
              if ((*(dlg->Buttons))[ib].check) clearBit(&((BLOK *)((*(dlg->Buttons))[id_b-1000].adr+dane))->flag,2);
              else setBit(&((BLOK *)((*(dlg->Buttons))[id_b-1000].adr+dane))->flag,2);
              block_changed=TRUE;
              continue;
          }


		  if ((ProcFG != NULL) && ((id_il) || (id_cb) || (id_lb) || (id_b)))
		  {
			  ProcFG();
		  }
	      
		if(id != -1)
		{
	 
		  n = fun(id);

	  
		  if(n == Dlg_Ret_Val_Continue_LB)
		  {
			goto continue_lb;
		  }

		  if (n == Dlg_Ret_Val_Redraw)
		  {
			  Redraw_Dlg(dlg);
		  }

	  
		  if(n == Dlg_Ret_Val_Cancel ||
			  n == Dlg_Ret_Val_Err ||
			  n==Dlg_Ret_Val_OK ||
			  n==Dlg_Ret_Val_Zapis_Cfg ||
			  n==Dlg_Ret_Val_Odczyt_Cfg ||
			  n==Dlg_Ret_Val_Save ||
			  n==Dlg_Ret_Val_Return ||
			  n == Dlg_Ret_Val_UP ||
			  n == Dlg_Ret_Val_UP_END ||
              n == Dlg_Ret_Val_DN_END ||
			  n == Dlg_Ret_Val_DN ||
			  n == Dlg_Ret_Val_Folder ||
			  n == Dlg_Ret_Val_Folder0 ||
			  n == Dlg_Ret_Val_Bigger ||
			  n == Dlg_Ret_Val_Smaller ||
              n == Dlg_Ret_Set_Range ||
              n == Dlg_Ret_Val_Alphabet ||
              n == Dlg_Ret_Val_Time ||
			  n>=Dlg_Ret_Val_IMPORT_BLOCK)
		  {
        
			dlg->flags |= 0x40;
			init(1,dlg, NULL);
			dlg->flags &= 0x40;
		
			//przywrocenie trybu graficznego

			if (GFX_WIN==1) 
			  {
				if (dialog_window_was_resized==TRUE)
					{
						set_resized_window(win_width, win_height, 1);
						kk=set_window_origin(x_win_orig, y_win_orig);
					}
			  }
			////////////////////////////////
			global_dialog_flag = 0;
			strncpy(global_dialog_name, "", 15);
            global_dialog_name[15]='\0';
            check_size_and_enable_F11(dim);
            reset_cursor_pos();
			return n;
		  }
		}
      }
    }
  }
  global_dialog_flag = 0;
  strncpy(global_dialog_name, "", 15);
  global_dialog_name[15]='\0';

  check_size_and_enable_F11(dim);
  reset_cursor_pos();
  return 0;
}

void Get_Default_Color_Dlg(DLG_COLOR *color_dlg)
/*--------------------------------------------*/
{
  if( getmaxcolor() == 1)
  {
    color_dlg->dlg_border 	= 1;
    color_dlg->dlg_paper  	= 0;
    color_dlg->dlg_caption	= 0;
    color_dlg->dlg_ink_caption	= 1;
    color_dlg->dlg_label_ink	= 1;
    color_dlg->dlg_label_paper	= 0;
    color_dlg->dlg_lin_ligth	= 1;
    color_dlg->dlg_lin_dark	= 0;
    color_dlg->dlg_group_ink	= 1;
    color_dlg->dlg_group_paper 	= 0;
    color_dlg->dlg_in_line_border= 1;
    color_dlg->dlg_in_line_ink	= 1;
    color_dlg->dlg_in_line_paper= 0;
    color_dlg->dlg_in_line_ink_ini  = 0 ;
    color_dlg->dlg_in_line_paper_ini= 1 ;
    color_dlg->dlg_button_border= 1;
    color_dlg->dlg_button_ink	= 1;
    color_dlg->dlg_button_paper	= 0;
    color_dlg->dlg_listbox_paper = 0;
    color_dlg->dlg_listbox_ink =  1;
    color_dlg->dlg_listbox_border = 1;
    color_dlg->dlg_list_combo_paper = 0;
    color_dlg->dlg_list_combo_ink =  1;
    color_dlg->dlg_list_combo_border = 1;
    color_dlg->dlg_combo_box_paper = 0;
    color_dlg->dlg_combo_box_ink = 1;
    color_dlg->dlg_combo_box_border = 1;
  }
  else
  {
    color_dlg->dlg_border 	= BLUE;
    color_dlg->dlg_paper  	= LIGHTGRAY;
    color_dlg->dlg_caption	= BLUE;
    color_dlg->dlg_ink_caption	= WHITE;
    color_dlg->dlg_label_ink	= BLACK;
    color_dlg->dlg_label_paper	= LIGHTGRAY;
    color_dlg->dlg_lin_ligth	= WHITE;
    color_dlg->dlg_lin_dark	= BLACK;
    color_dlg->dlg_group_ink	= BLACK;
    color_dlg->dlg_group_paper 	= LIGHTGRAY;
    color_dlg->dlg_in_line_border= BLACK;
    color_dlg->dlg_in_line_ink	= BLACK;
    color_dlg->dlg_in_line_paper= WHITE;
    color_dlg->dlg_in_line_ink_ini  = BLACK;
    color_dlg->dlg_in_line_paper_ini= LIGHTGREEN;
    color_dlg->dlg_button_border= BLACK;
    color_dlg->dlg_button_ink	= BLACK;
    color_dlg->dlg_button_paper	= LIGHTGRAY;
    color_dlg->dlg_listbox_paper = LIGHTGRAY;
    color_dlg->dlg_listbox_ink =  BLACK;
    color_dlg->dlg_listbox_border = BLACK;
    color_dlg->dlg_list_combo_paper = 254 ;
    color_dlg->dlg_list_combo_ink =  BLACK;
    color_dlg->dlg_list_combo_border = BLACK;
    color_dlg->dlg_combo_box_paper = LIGHTGRAY;
    color_dlg->dlg_combo_box_ink = BLACK;
    color_dlg->dlg_combo_box_border = BLACK;

  }
}

void Check_Radio_Button( TDIALOG *dlg,
			int id_first_button,
			int id_last_button,
			int id_check_button,
			BOOL draw)
/*----------------------------------------------*/
{
  int i, id;
  BUTTON *buttons;

  buttons =  *(dlg->Buttons);
  for(i = 0; i < dlg->SizeButtonT ; i++)
  {
    if(buttons[i].id == id_first_button)
    {
      break;
    }
  }
  for( id = id_first_button;
	id <= id_last_button;
	id++, i++)
  {
    buttons[i].check = 0;
    if(buttons[i].id == id_check_button)
    {
      buttons[i].check = 1;
    }
    if(draw == TRUE)
    {
      GrMouseEraseCursor();
      Draw_Button(&(buttons [i]));
      GrMouseDisplayCursor();
    }
  }
}

BOOL Get_Check_Button( TDIALOG *dlg, int id_button)
/*----------------------------------------------*/
{
  int i, ret;
  BUTTON *buttons;

  buttons =  *(dlg->Buttons);
  for(i = 0; i < dlg->SizeButtonT ; i++)
  {
    if(buttons[i].id == id_button)
    {
      break;
    }
  }
  ret = FALSE;
  if(i < dlg->SizeButtonT)
  {
     ret = buttons[i].check;
  }
  return ret;
}

void Set_Check_Button( TDIALOG *dlg, int id_button, BOOL check)
/*-------------------------------------------------------------*/
{
  int i;
  BUTTON *buttons;

  buttons =  *(dlg->Buttons);
  for(i = 0; i < dlg->SizeButtonT ; i++)
  {
    if (buttons[i].id == id_button)
    {
      break;
    }
  }
  if (i < dlg->SizeButtonT)
  {
     buttons[i].check = check;
  }
}

void Set_Enable_Button(TDIALOG *dlg, int id_button, BOOL enable)
/*-------------------------------------------------------------*/
{
	int i;
	BUTTON *buttons;

	if (id_button == -1) return;
	buttons = *(dlg->Buttons);
	for (i = 0; i < dlg->SizeButtonT; i++)
	{
		if (buttons[i].id == id_button)
		{
			break;
		}
	}
	if (i < dlg->SizeButtonT)
	{
		buttons[i].enable = enable;
	}
}

void Set_Enable_Edit(TDIALOG *dlg, int id_edit, BOOL enable)
/*---------------------------------------------------------*/
{
	int i;
	INPUTLINE *edits;

	if (id_edit == -1) return;
	edits = *(dlg->InputLines);
	for (i = 0; i < dlg->SizeInLinT; i++)
	{
		if (edits[i].id == id_edit)
		{
			break;
		}
	}
	if (i < dlg->SizeInLinT)
	{
		edits[i].enable = enable;
	}
}

void Set_Enable_Listbox(TDIALOG *dlg, int id_listbox, BOOL enable)
/*--------------------------------------------------------------*/
{
	int i;
	LISTBOX *listboxes;

	if (id_listbox == -1) return;
    listboxes = *(dlg->ListBoxes);
	for (i = 0; i < dlg->SizeListBoxT; i++)
	{
		if (listboxes[i].id == id_listbox)
		{
			break;
		}
	}
	if (i < dlg->SizeListBoxT)
	{
		listboxes[i].enable = enable;
	}
}

void Dlg_Kolory(DLG_COLOR *kolory)
{
  dlg_kolory = kolory;
  wsp_x = 1.0 ;
  wsp_y = 1.0 ;
}

#undef __DIALOG__