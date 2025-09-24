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

#define __O_WYJ__
#pragma warning(disable : 4996)
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#ifndef LINUX
#include<dos.h>
#include<io.h>
#else
#include <unistd.h>
#endif
#include<stdio.h>
#include<stdlib.h>
#ifndef LINUX
#include<process.h>
#endif
#include<errno.h>
#include<math.h>
#include<sys/stat.h>

#include "bgiext.h"
#include "bib_e.h"
#include "rysuj_e.h"
#include "b_mouse.h"
#include "o_loadf.h"
#include "o_protec.h"
#include "b_textw.h"
#include "o_lfile.h"
#include "o_spec.h"
#include "alf_res.h"
#include "o_dialog.h"
#include "o_image.h"
#include "o_pltprn.h"
#include "o_plt.h"
#include "o_prn.h"
#include "o_addmnu.h"

#include "b_bibe.h"

#include "menu.h"

#include "forlinux.h"

#define MIN_H 800 //1024
#define MIN_V 540 //640

#define DRV_MASTER 0
#define DRV_SLAVE 1

#define WINSET_FILE "WINSET.DAT"

#define ALFAIMPORTFILE "alfaimport.tmp"

#define cursor_off() out_cur_off(X,Y)
#define cursor_on() out_cur_on(X,Y)
#define TADD 0x04	   /*teksty dodatkowe*/
#define PRN_PCX 0
#define PRN_WINDOWS 1
#define PRN_PDF 2

#define LINE myline

#define YES _YES__
#define NO _NO__

typedef unsigned long       DWORD;

BOOL preview_blocked = FALSE;

BOOL MacOS15=FALSE;

#pragma pack (1)
T_PTR_Prn_Config ptrs_config ;
T_PTR_Prn_Ini_Date ptrs_ini_date;
static char sz_file [MAXPATH] ;
char *prn_file;
char background_name[32];

double x01,y01,x02,y02,dx,dy,dx1,dy1;
static PRIV_PRN_WINDOW priv_prn_window={0,0,0,0};
static BOOL close_button_pressed=FALSE;

extern BOOL wayland;

extern void Check_XNextEvent(void);
extern BOOL hibernate;

extern int DRAWING_NUMBER;
extern BOOL  Change ;
static  OKNO O;
extern BOOL no_break;
extern void Exec(char *patch0, char *params);
extern int ask_question (int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien, int image);
extern int get_graphic_drv(void);
extern BOOL Semaphore;
extern BOOL Cust_Semaphore;
extern void dialog_cursor(int on);

extern void get_posXY(double *pozx, double *pozy);
extern void set_posXY(double pozx, double pozy);
extern int get_menu_level(void);
extern void Ini_Layers_Dlg (void) ;
extern void Set_Auto_Backup (BOOL) ;
extern void DxfOut(int acad_version) ;
extern void Info_Sys (void) ;
extern void Clear_View (void) ;
extern void Convert_Old_File (void) ;
extern void not_active (void) ;
extern void demo_print(int);
extern void demo_plot(int);
extern void Wejscie (void);
extern double SkalaF_ (void);
extern double Jednost_(void);
extern void set_increased(void);
extern int GetPrintPageWidth(void);
extern int GetPrintPageHeight(void);
extern int GetPrintHorzres(void);
extern int GetPrintVertres(void);
extern int GetPrintHorizDensity(void);
extern int GetPrintVertDensity(void);
extern int my_file_exists(char *name);
extern void setcolor(int kolor);
extern void setwritemode( int mode );
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);
extern void myline(int x1, int y1, int x2, int y2);
extern void delay(int time);
extern BOOL Check_if_Equal (double x, double y);
extern BOOL Check_if_Equal2(double x, double y);
extern int Save_Screen(void);
extern int Restore_Screen(void);
extern int Print2Page(int WINPRINT_DEF);
extern char *Get_Sheet (void);
extern void *find_block_sub(char *adrp, char *adrk, unsigned char kod_obiektu, char *opis_obiektu, int l_char);
extern int wszystkie_warstwy1 (void);
extern void Get_Limits (long off, long offk, int atrybut, double *xmin, double *xmax, double *ymin, double *ymax);
extern int getgraphmode(void);
extern void restorecrtmode(void);
extern void set_BGI_mode_whc(int *gd, int *gm, int width, int height, int colors);
extern int czytaj_rysunek1(char *rys, BOOL b_err);
extern void set_scrsave_time (void);
extern void kasuj_nr_pola(void);
extern int demo_plot_to_com(int n);
extern void change_slash(char *nazwa);
extern void delete_instal_tab(void);
extern int czytaj_dxf_file(char *fn, double Jednostki_dxf, double SkalaF_dxf, double offset_dxf_x, double offset_dxf_y, BOOL auto_size, int latin2_dxf);
extern int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext);
extern int getdisk(void);
extern int setdisk(int _drive);
extern void Al_Load_PCX_fade_out(char *pcx_name, int x, int y, int d_x, int d_y);
extern int load_pcx_(B_PCX *b_pcx,int opcja);
extern void Al_Load_PCX_fade(char *pcx_name, int x, int y, int d_x, int d_y);
extern int Al_Load_PNG_fade(char *png_name, char *png_name1, int w, int h, int x, int y, int d_x, int d_y, int fade_out);
extern int get_window_origin_and_size(int *x_win_orig, int *y_win_orig, int *win_width, int *win_height);
extern void DoneBuffers(int dwg_no);
extern void DoneBuffMacro(void);
extern void DoneBuffers1(void);
extern void DoneBuffers2(void);
extern void DoneBuffers3(void);
extern void DoneBuffers4(void);
extern void DoneBuffers5(void);
extern void DoneBuffers6(void);
extern void DoneBuffers7(void);
extern void DoneBuffers8(void);
extern void DoneBackground(void);
extern void DonePatterns(void);
extern void DonePatternBitmapScaled(void);
extern void DoneKeyimage(void);
extern void DoneMickeyHand(void);
extern void DoneCursorimage(void);
extern void w95_setapptitle(const char *tekst);
extern int pkonfig(KOLORY_PROFILI *kolory_profil);
extern int konfig(KOLORY *kolory_pulpitu);
extern void not_still_implemented	(void);
extern int param_conf(void);
extern void cleardevice(void);
extern void DF_to_String (char *ptrsz_buf, char *ptrsz_format, double df_val, int max_size);
extern void change_font(int nr_font);
extern void set_yp(void);
extern void layer_info (void);
extern void Destroy_All_ctx(void);
extern void Test_App(int doit, char *file_name);
extern void Open_Backgrounds(void);

extern T_Prototype  s__prot;

extern int TRANSLUCENCY;
extern BOOL BIGCURSOR;
extern BOOL BAR_POINTER;
extern void set_dialog_cursor(BOOL bigsmall);
extern void set_menu_cursor(BOOL bar_pointer);
extern void free_mem_hatch_def_param(void);
extern void Free_winvar(void);
extern void find_font_face(char *Font_File);
extern void decodingwin(char *text);
extern void decodingwin_true(wchar_t text);

extern long pXp, pYp, pXk, pYk;
extern int get_first_ctx(void);
extern BOOL get_ctx_created(void);
extern int get_i_ctx_created(void);
extern int get_i_ctx(void);
extern BOOL get_ctx_ok(void);
extern BOOL get_regen_ctx(void);
extern void put_ctx_created(BOOL ctx_created_);
extern void put_i_ctx_created(int i_ctx_created_);
extern void put_i_ctx(int i_ctx_);
extern void put_ctx_ok(BOOL ctx_ok_);
extern void put_ctx_ok(BOOL regen_ctx_);
extern void put_ctx_par(int i, Ctx_Par_copy *rev_ctx_par);
extern char *get_ctx_bitmap(int i);
extern void put_ctx_bitmap(int i, char *ctx_bmp);

extern BITMAP *ctx_bitmap[32];
extern void near uaktualnij_pola(void);
extern void null_all_ctx_pointers(void);

extern DWORD RunSilent(char* strFunct, char* strstrParams);
extern int czytaj_dxf_header(char* fn, DXF_Header* dxf_header);
extern int GetDXFParams(DXF_Header* head);
extern BOOL Delete_File(char* ptrsz_fnd);
extern void put_spec_name(int nr, char* st);
extern void put_spec_string(int nr, char* st);
extern void zwolnienie_pamieci_forever(void);
extern void setaspectratio(int xasp, int yasp);
extern void Save_Desktop_font(char* font_name);
extern void Set_Desktop_font(char* font_name);
extern void reset_background(void);
extern void setfillstyle_(int pattern, int color);
extern void put_regen_ctx(BOOL regen_ctx_);
extern void null_ctx_pointers_tail(int start);
extern void get_spec_name(int nr, char* st);
extern void get_spec_string(int nr, char* st);
extern void get_ctx_par(int i, Ctx_Par_copy* rev_ctx_par);
extern void Save_View_Preview(void);
extern void ShowPreview(int x01, int y01, int x02, int y02);
extern void my_blit(BITMAP* ctx_bitmap, int mmx0, int mmy0, int mmx1, int mmy1, int dxx, int dyy);
extern void sw_callback_locked_proc(void);
extern void decodingwin(char* text);
extern void decodingwin_true(wchar_t text);

extern void mouseMove(int dx, int dy);

extern BOOL DEMO_RECORDING;
extern void set_demo_scale(float demo_scale);

extern void Get_change_dlg(T_change_param* s__change_param_);

extern void Set_change_dlg(T_change_param* s__change_param_);

extern void Reset_change_dlg(void);

extern int focus_on_window(void);
extern int set_focus_on_window(void);
extern int my_poll_mouse(void);
extern void my_set_window_title(char* wnd_title);

extern void get_full_screen(int *x0_n, int *y0_n, int *dx_n, int *dy_n);
extern void set_ttf_digits27_len(void);

extern void expand_horizontally(void);
extern void expand_vertically(void);
extern void expand_diagonally(void);
extern void restore_last_window(void);
extern int circle_and_rectangle_proc(void);

extern GrContext *get_second_screen(void);
extern void draw_keyimage_(int keyimage, BOOL set_timer, BOOL silent, BOOL enforce);
extern void draw_demokeyimage_(int keyimage, int keyimage1, BOOL set_timer, BOOL silent, BOOL enforce);
extern void CopyPreview(int x01, int y01, int x02, int y02, int client_number);
extern int return_active_clients(void);
extern int return_and_convert_bitmap(int client, char *dump_file, int *x1, int *y1, int *x2, int *y2, char *drawing_file);
extern char *Get_dane0_DATA(int dwg_no);
extern void Ini_Global_Object (void);
extern int Get_A_Screen(int ret_left_x, int ret_right_x, int ret_top_y, int ret_bottom_y, int mouse_curx, int mouse_cury);
extern int get_window_origin_and_size(int *x_win_orig, int *y_win_orig, int *win_width, int *win_height);
extern int get_window_new_origin_and_size(int *x_win_orig, int *y_win_orig, int *win_width, int *win_height);
extern void expand_dim(int x0, int y0, int width, int height);
extern void save_window_dim(int x0_, int y0_, int dx_, int dy_);
extern int get_monitor_dims(int *ret_left_x, int *ret_right_x, int *ret_top_y, int *ret_bottom_y, int i);
extern void clean_key_buffer(void);
extern void xwin_set_hints(int dx, int dy);
extern PRIV_PRN_WINDOW *get_priv_prn_window(void);
extern void Done_CUPS_Printers(void);
extern int Get_X11_SCREEN_SHIFT(void);

extern BOOL yes_or_no(char *question);
extern void reset_dane0(void);

extern void backup_hatch_pattern(void);
extern void restore_hatch_pattern(void);

extern int opwin;
extern void deactiv_menu(TMENU *menu);

extern int Test_App_Shm_Off(void);
extern int PozX, PozY;
extern int PozX0, PozY0;

extern int alfaplugins_off(void);

extern void report_mem_leak_(void);
extern void report_mem_leak_cpp_(void);
extern void delete_hatch_file (void);

extern void set_st_jedn(void);

extern void lock_mouse(void);
extern void free_mouse(void);

extern void reset_n_list_begin_blk(void);
extern void getcolor_RGB(int *red, int *green, int *blue, int color);
extern void reset_b_layers_bak(void);

extern BOOL test_python_pillow(void);

extern void DoneArgs(void);
extern void Restore_Pointer(void);

extern void enable_F11(void);

extern int show_native_message_box(char *title, char *heading, char *text);
extern void Odczyt_licznikow_simple(void);
extern void  Test_przycisniec(int *key);

extern int edit_text_flag;
extern void reset_editbox_geometry_set(void);
extern int Get_Mouse_Wheel(void);
extern void Set_Mouse_Wheel(int wheel);

extern void  Odczyt_licznikow(void);
#ifdef MACOS
extern int send_AppleScript_Exit(pid_t pid);
extern pid_t get_e_pid(void);
extern void MoveOSXCursor(int x, int y);
#endif
#ifdef ALLEGRO5
extern void position_mouse_xy(int x, int y);
#endif

extern double depth_magnitude; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double thermal_magnitude;
extern double load_magnitude; //units per mm  default 10kN/m load per 1 mm on drawing paper
extern double force_magnitude; //units per mm  default 10kN force per 1 mm on drawing paper
extern double moment_magnitude; //units per mm  default 10kNm force per 1 mm radius on drawing paper
extern double displacement_magnitude; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
extern double rotation_magnitude;

extern double thermal_precision;
extern double force_precision;
extern double moment_precision;
extern double displacement_precision;
extern double rotation_precision;
extern double load_precision;
extern double stress_precision;

extern double n_magnitude;
extern double v_magnitude;
extern double m_magnitude;
extern double d_magnitude;
extern double r_magnitude;
extern double rm_magnitude;
extern double s_magnitude;
extern double q_magnitude;
extern double p_magnitude;

void unredcr_menu(void);
void Koniec(void);
void Restart(void);
void DxfInExe(char* sk, DXF_Header* head);
void Desktop(void);
int Restore_params(void);
int Deposit_params(void);
void standard_func(void);
void view_scale(void);

extern float cups_prn_width_paper;
extern float cups_prn_height_paper;

extern int findfpostopxl(/*const unsigned*/ char *s, int max_pxl);

extern void Check_ConfigureNotify(void);
extern int get_tier(void);
extern int what_tier(int font_size);
extern void Destroy_Bitmaps(void);
extern void Load_Bitmaps(int HEIGHT_);
extern void reload_client_bitmaps(void);
extern void Set_Mem_Bitmaps(int tier_);
extern void Set_Mem_Cursors(int tier_);
extern void save_mouse_wheel(void);

extern BOOL Semaphore;
extern BOOL check_file_buffer(void);


static BITMAP *second_screen_bak_=NULL;

static DRAWING_PARAMS Drawing_Params[MAX_NUMBER_OF_WINDOWS];

BITMAP *qmark;
char *qmark_p;

BITMAP *eymark;
char *eymark_p;

BITMAP *ermark;
char *ermark_p;

BITMAP *offmark;
char *offmark_p;

BITMAP *icon_yes_d_mem;
BITMAP *icon_yes_dx1_5_mem;
BITMAP *icon_yes_dx2_mem;
BITMAP *icon_no_d_mem;
BITMAP *icon_no_dx1_5_mem;
BITMAP *icon_no_dx2_mem;
char *icon_yes_d_pmem;
char *icon_no_d_pmem;
BITMAP* icon_upgrademark_mem;
char* icon_upgrademark_pmem;
BITMAP* icon_noupgrademark_mem;
char* icon_noupgrademark_pmem;
BITMAP* icon_alfacad_mem;

BITMAP *plotter;
char *plotter_p;

BITMAP *icon_draft;
char *icon_draft_p;

BITMAP *icon_block;
char *icon_block_p;

BITMAP *icon_modify;
char *icon_modify_p;

BITMAP *icon_search;
char *icon_search_p;

BITMAP *icon_explode_text_to_letters;
char *icon_explode_text_to_letters_p;

BITMAP *icon_dim_horizontal;
char *icon_dim_horizontal_p;

BITMAP *icon_hatch;
char *icon_hatch_p;

BITMAP *icon_polygonal_aread;
char *icon_polygonal_aread_p;

BITMAP *icon_create_macro;
char *icon_create_macro_p;

BITMAP *icon_drawing_settings;
char *icon_drawing_settings_p;

BITMAP *icon_settings;
char *icon_settings_p;

BITMAP *icon_save;
char *icon_save_p;

BITMAP *icon_quit;
char *icon_quit_p;

BITMAP *icon_line;
BITMAP *icon_circle;
BITMAP *icon_arc;
BITMAP *icon_text;
BITMAP *icon_polygon;
BITMAP *icon_polygon_n;
BITMAP *icon_disc;
BITMAP *icon_ellipses;
BITMAP *icon_polylines;
BITMAP *icon_solid;
BITMAP *icon_line_rectangle;
BITMAP *icon_trace;
BITMAP *icon_2line;
BITMAP *icon_3line;
BITMAP *icon_line_freehand;
BITMAP *icon_point;
BITMAP *icon_chart;
BITMAP *icon_reinforcing;
char *icon_line_p;
char *icon_circle_p;
char *icon_arc_p;
char *icon_text_p;
char *icon_polygon_p;
char *icon_polygon_n_p;
char *icon_disc_p;
char *icon_ellipses_p;
char *icon_polylines_p;
char *icon_solid_p;
char *icon_line_rectangle_p;
char *icon_trace_p;
char *icon_2line_p;
char *icon_3line_p;
char *icon_line_freehand_p;
char *icon_point_p;
char *icon_chart_p;
char *icon_reinforcing_p;


BITMAP *icon_move;
BITMAP *icon_move_copy;
BITMAP *icon_delete;
BITMAP *icon_restore;
BITMAP *icon_rotate;
BITMAP *icon_rotate_block;
BITMAP *icon_scale;
BITMAP *icon_stretch;
BITMAP *icon_edit_text;
BITMAP *icon_mirror;
BITMAP *icon_mirror_leave;
BITMAP *icon_mirror_replace;
BITMAP *icon_mirror_block_x;
BITMAP *icon_mirror_block_y;
BITMAP *icon_array;
BITMAP *icon_change;
BITMAP *icon_trim_segments;
BITMAP *icon_cutout;
BITMAP *icon_extend;
BITMAP *icon_fillet;
BITMAP *icon_chamfer;
BITMAP *icon_offset;
BITMAP *icon_divide;
BITMAP *icon_undo;

BITMAP *icon_pointfinder;
BITMAP *icon_camera;
BITMAP *icon_parameters;
BITMAP *icon_layers;
BITMAP *icon_measure;
BITMAP *icon_empty;


char *icon_move_p;
char *icon_move_copy_p;
char *icon_delete_p;
char *icon_restore_p;
char *icon_rotate_p;
char *icon_rotate_block_p;
char *icon_scale_p;
char *icon_stretch_p;
char *icon_edit_text_p;
char *icon_mirror_p;
char *icon_mirror_leave_p;
char *icon_mirror_replace_p;
char *icon_mirror_block_x_p;
char *icon_mirror_block_y_p;
char *icon_array_p;
char *icon_change_p;
char *icon_trim_segments_p;
char *icon_cutout_p;
char *icon_extend_p;
char *icon_fillet_p;
char *icon_chamfer_p;
char *icon_offset_p;
char *icon_divide_p;
char *icon_undo_p;

char *icon_pointfinder_p;
char *icon_camera_p;
char *icon_parameters_p;
char *icon_layers_p;
char *icon_measure_p;
char *icon_empty_p;

BITMAP *icon_copy;
BITMAP *icon_paste;
BITMAP *icon_import_block;
BITMAP *icon_export_block;
BITMAP* icon_import_image;
BITMAP *icon_import_png;
BITMAP* icon_import_jpg;
BITMAP *icon_edit_image;
BITMAP *icon_export_image;
BITMAP *icon_group;
BITMAP *icon_explode;
BITMAP *icon_block_list;
BITMAP *icon_explode_frozen;
BITMAP *icon_delete_dxf_blocks;
BITMAP *icon_correct_blocks;
BITMAP *icon_fixed_layers;

char *icon_copy_p;
char *icon_paste_p;
char *icon_import_block_p;
char *icon_export_block_p;
char* icon_import_image_p;
char *icon_import_png_p;
char* icon_import_jpg_p;
char *icon_edit_image_p;
char *icon_export_image_p;
char *icon_group_p;
char *icon_explode_p;
char *icon_block_list_p;
char *icon_explode_frozen_p;
char *icon_delete_dxf_blocks_p;
char *icon_correct_blocks_p;
char *icon_fixed_layers_p;

BITMAP *icon_area;
BITMAP *icon_center_of_area;
BITMAP *icon_first_moment_of_area;
BITMAP *icon_area_moment_of_inertia;
BITMAP *icon_centr_area_mom_of_inertia;
BITMAP *icon_test_of_closed_area;


char *icon_area_p;
char *icon_center_of_area_p;
char *icon_first_moment_of_area_p;
char *icon_area_moment_of_inertia_p;
char *icon_centr_area_mom_of_inertia_p;
char *icon_test_of_closed_area_p;


BITMAP *icon_import;
BITMAP *icon_new;
BITMAP *icon_new_window;
BITMAP *icon_save_as;
BITMAP *icon_export_cad;
BITMAP *icon_import_cad;
BITMAP *icon_export_dxf;
BITMAP *icon_import_dxf;
BITMAP *icon_export_dwg;
BITMAP *icon_import_dwg;
BITMAP *icon_edit_title_block;
BITMAP *icon_cmd;
BITMAP *icon_print;
BITMAP *icon_infobox;
BITMAP *icon_last_files;
BITMAP *icon_open;

char *icon_import_p;
char *icon_new_p;
char *icon_new_window_p;
char *icon_save_as_p;
char *icon_export_cad_p;
char *icon_import_cad_p;
char *icon_export_dxf_p;
char *icon_import_dxf_p;
char *icon_export_dwg_p;
char *icon_import_dwg_p;
char *icon_edit_title_block_p;
char *icon_cmd_p;
char *icon_print_p;
char *icon_infobox_p;
char *icon_last_files_p;
char *icon_open_p;

BITMAP *icon_change_texts;
BITMAP *icon_auto_numbering;
BITMAP *icon_record;
BITMAP *icon_play;
BITMAP *icon_load;
BITMAP *icon_save_as;
BITMAP *icon_format;
BITMAP *icon_dscale;
BITMAP *icon_accuracy;
BITMAP *icon_units;
BITMAP *icon_coordinates;
BITMAP *icon_local;
BITMAP *icon_pointer;
BITMAP *icon_edit_ini;
BITMAP *icon_cartesian;
BITMAP *icon_geodetic;
BITMAP *icon_point_origin;
BITMAP *icon_axis_angle;
BITMAP *icon_rotation_angle;
BITMAP *icon_colorwheel;
BITMAP *icon_panoramic;
BITMAP *icon_font_type;


char *icon_change_texts_p;
char *icon_auto_numbering_p;
char *icon_record_p;
char *icon_play_p;
char *icon_load_p;
char *icon_save_as_p;
char *icon_format_p;
char *icon_dscale_p;
char *icon_accuracy_p;
char *icon_units_p;
char *icon_coordinates_p;
char *icon_local_p;
char *icon_pointer_p;
char *icon_edit_ini_p;
char *icon_cartesian_p;
char *icon_geodetic_p;
char *icon_point_origin_p;
char *icon_axis_angle_p;
char *icon_rotation_angle_p;
char *icon_colorwheel_p;
char *icon_panoramic_p;
char *icon_font_type_p;

BITMAP *icon_yes;
BITMAP *icon_no;
BITMAP *icon_dx_dy;
BITMAP *icon_calibrate;
BITMAP *icon_A_rectangular;
BITMAP *icon_A_circular;
BITMAP *icon_A_circular_rot;
BITMAP *icon_A_circular_n;
BITMAP *icon_A_circular_rot_n;
BITMAP *icon_A_circular_angle;
BITMAP *icon_A_circular_rot_angle;

char *icon_yes_p;
char *icon_no_p;
char *icon_dx_dy_p;
char *icon_calibrate_p;
char *icon_A_rectangular_p;
char *icon_A_circular_p;
char *icon_A_circular_rot_p;
char *icon_A_circular_n_p;
char *icon_A_circular_rot_n_p;
char *icon_A_circular_angle_p;
char *icon_A_circular_rot_angle_p;


BITMAP *icon_pointmode;
BITMAP *icon_endpoint;
BITMAP *icon_nearest_end;
BITMAP *icon_middlepoint;
BITMAP *icon_intersection;
BITMAP *icon_center;
BITMAP *icon_perpendicular;
BITMAP *icon_tangential;
BITMAP *icon_nearest;
BITMAP *icon_onpoint;
BITMAP *icon_move2xy;
BITMAP *icon_movedxdy;
BITMAP *icon_move_polar;
BITMAP *icon_nopointfinder;


char *icon_pointmode_p;
char *icon_endpoint_p;
char *icon_nearest_end_p;
char *icon_middlepoint_p;
char *icon_intersection_p;
char *icon_center_p;
char *icon_perpendicular_p;
char *icon_tangential_p;
char *icon_nearest_p;
char *icon_onpoint_p;
char *icon_move2xy_p;
char *icon_movedxdy_p;
char *icon_move_polar_p;
char *icon_nopointfinder_p;

BITMAP *icon_calculator;
BITMAP *icon_point_measure;
BITMAP *icon_distance_point_to_point;
BITMAP *icon_angle_line_to_line;
BITMAP *icon_plusminus;
BITMAP *icon_property_list;
BITMAP *icon_noplusminus;
BITMAP *icon_addfavourite;
BITMAP *icon_findfavourite;
BITMAP *icon_refresh;
BITMAP *icon_refresh_d;
BITMAP *icon_zoom_all;
BITMAP *icon_zoom_center;
BITMAP *icon_auto_panorama;
BITMAP *icon_zoom_1;
BITMAP *icon_zoom_2;
BITMAP *icon_zoom_in;
BITMAP *icon_zoom_out;
BITMAP *icon_zoom_x;
BITMAP *icon_zoom_window;
BITMAP *icon_zoom_drawing;
BITMAP *icon_zoom_previous;


char *icon_calculator_p;
char *icon_point_measure_p;
char *icon_distance_point_to_point_p;
char *icon_angle_line_to_line_p;
char *icon_plusminus_p;
char *icon_property_list_p;
char *icon_noplusminus_p;
char *icon_addfavourite_p;
char *icon_findfavourite_p;
char *icon_refresh_p;
char *icon_refresh_d_p;
char *icon_zoom_all_p;
char *icon_zoom_center_p;
char *icon_auto_panorama_p;
char *icon_zoom_1_p;
char *icon_zoom_2_p;
char *icon_zoom_in_p;
char *icon_zoom_out_p;
char *icon_zoom_x_p;
char *icon_zoom_window_p;
char *icon_zoom_drawing_p;
char *icon_zoom_previous_p;


BITMAP *icon_pointer_step;
BITMAP *icon_grid_density;
BITMAP *icon_grid_points;
BITMAP *icon_restr_ortho;
BITMAP *icon_color;
BITMAP *icon_line_thickness;
BITMAP *icon_line_type;
BITMAP *icon_point_visibility;
BITMAP *icon_point_size;
BITMAP *icon_get_color;
BITMAP *icon_get_type;
BITMAP *icon_get_layer;
BITMAP *icon_floating_text;
BITMAP *icon_floating_block;
BITMAP *icon_dim_scale;
BITMAP *icon_stretch_in_pin;
BITMAP *icon_break_in_pin;
BITMAP *icon_mask;
BITMAP *icon_hidden_text_vis;
BITMAP *icon_image_points;
BITMAP *icon_highlight_layer;
BITMAP *icon_dbuff_image;
BITMAP *icon_text_most_readable;

BITMAP *icon_line_color;
BITMAP *icon_text_color;
BITMAP *icon_text_height;
BITMAP *icon_width;
BITMAP *icon_italic;
BITMAP *icon_bold;
BITMAP *icon_font_type;
BITMAP *icon_accuracy;
BITMAP *icon_terminators;
BITMAP *icon_line_offset;
BITMAP *icon_text_direction;
BITMAP *icon_extension_lines;

BITMAP *icon_dim_arrowheads;
BITMAP *icon_dim_ticks;
BITMAP *icon_dim_dots;

BITMAP *icon_text_parallel;
BITMAP *icon_text_horizontal;
BITMAP *icon_fixed_length;
BITMAP *icon_align_to_object;

BITMAP *icon_dim_direction;
BITMAP *icon_dim_new;
BITMAP *icon_dim_remove;
BITMAP *icon_dim_edit;
BITMAP *icon_dim_settings;
BITMAP *icon_dim_arc_RL;
BITMAP *icon_dim_circle_DR;

BITMAP *icon_dim_horizontal;
BITMAP *icon_dim_vertical;
BITMAP *icon_dim_aligned;
BITMAP *icon_dim_linear;
BITMAP *icon_dim_linearX;
BITMAP *icon_dim_angular;

BITMAP *icon_dim_arc_radial;
BITMAP *icon_dim_arc_length;
BITMAP *icon_dim_diametric;
BITMAP *icon_dim_radial;
BITMAP *icon_dim_radial_y;
BITMAP *icon_dim_radial_y_dim;
BITMAP *icon_dim_radial_y_point;
BITMAP *icon_dim_radial_y_tangential;

char *icon_pointer_step_p;
char *icon_grid_density_p;
char *icon_grid_points_p;
char *icon_restr_ortho_p;
char *icon_color_p;
char *icon_line_thickness_p;
char *icon_line_type_p;
char *icon_point_visibility_p;
char *icon_point_size_p;
char *icon_get_color_p;
char *icon_get_type_p;
char *icon_get_layer_p;
char *icon_floating_text_p;
char *icon_floating_block_p;
char *icon_dim_scale_p;
char *icon_stretch_in_pin_p;
char *icon_break_in_pin_p;
char *icon_mask_p;
char *icon_hidden_text_vis_p;
char *icon_image_points_p;
char *icon_highlight_layer_p;
char *icon_dbuff_image_p;
char *icon_text_most_readable_p;

char *icon_line_color_p;
char *icon_text_color_p;
char *icon_text_height_p;
char *icon_width_p;
char *icon_italic_p;
char *icon_bold_p;
char *icon_font_type_p;
char *icon_accuracy_p;
char *icon_terminators_p;
char *icon_line_offset_p;
char *icon_text_direction_p;
char *icon_extension_lines_p;

char *icon_dim_arrowheads_p;
char *icon_dim_ticks_p;
char *icon_dim_dots_p;

char *icon_text_parallel_p;
char *icon_text_horizontal_p;
char *icon_fixed_length_p;
char *icon_align_to_object_p;

char *icon_dim_direction_p;
char *icon_dim_new_p;
char *icon_dim_remove_p;
char *icon_dim_edit_p;
char *icon_dim_settings_p;
char *icon_dim_arc_RL_p;
char *icon_dim_circle_DR_p;

char *icon_dim_horizontal_p;
char *icon_dim_vertical_p;
char *icon_dim_aligned_p;
char *icon_dim_linear_p;
char *icon_dim_linearX_p;
char *icon_dim_angular_p;

char *icon_dim_arc_radial_p;
char *icon_dim_arc_length_p;
char *icon_dim_diametric_p;
char *icon_dim_radial_p;
char *icon_dim_radial_y_p;
char *icon_dim_radial_y_dim_p;
char *icon_dim_radial_y_point_p;
char *icon_dim_radial_y_tangential_p;

BITMAP *icon_circle_center_radius;
BITMAP *icon_circle_center_diameter;
BITMAP *icon_circle_2_points;
BITMAP *icon_circle_3_points;
BITMAP *icon_line_close;
BITMAP *icon_line_remove;
BITMAP *icon_trim_value;
BITMAP *icon_line_continue;
BITMAP *icon_arc_3_points;
BITMAP *icon_arc_beginning_center_end;
BITMAP *icon_arc_beginning_center_angle;
BITMAP *icon_arc_beginning_center_chord;
BITMAP *icon_arc_beginning_end_radius;
BITMAP *icon_arc_beginning_end_angle;
BITMAP *icon_arc_beginning_end_direction;
BITMAP *icon_arc_continuation;
BITMAP *icon_text_angle;
BITMAP *icon_text_width;
BITMAP *icon_text_type;
BITMAP *icon_hidden_text;
BITMAP *icon_text_align;
BITMAP *icon_text_interval;
BITMAP *icon_line_polygon_cor_cor;
BITMAP *icon_line_polygon_cen_tan;
BITMAP *icon_line_polygon_cen_cor;
BITMAP *icon_ellipse_approximation;
BITMAP *icon_ellipse_axis_center;
BITMAP *icon_ellipse_axis_start;
BITMAP *icon_ellipse_center;
BITMAP *icon_solid_quad_triang;
BITMAP *icon_solid_triangular1;
BITMAP *icon_solid_fill_line_width;
BITMAP *icon_solid_quadrangular;
BITMAP *icon_solid_triangular;
BITMAP *icon_arc_close;
BITMAP *icon_arc_remove;
BITMAP *icon_trace_width;
BITMAP *icon_trace_axis_offset;
BITMAP *icon_trace_polyline;
BITMAP *icon_trace_fill_line_width;
BITMAP *icon_trace_remove;
BITMAP *icon_chart_step_dx;
BITMAP *icon_ftfunct;
BITMAP *icon_last_block;
BITMAP *icon_select_all;
BITMAP *icon_all_layers;
BITMAP *icon_select_cross_window;
BITMAP *icon_add_remove;
BITMAP *icon_gear1;
BITMAP *icon_text_align_baselineleft;
BITMAP *icon_text_align_baselineright;
BITMAP *icon_text_align_baselinecenter;
BITMAP *icon_text_align_middlecenter;
BITMAP *icon_circle_tangential;

char *icon_circle_center_radius_p;
char *icon_circle_center_diameter_p;
char *icon_circle_2_points_p;
char *icon_circle_3_points_p;
char *icon_line_close_p;
char *icon_line_remove_p;
char *icon_trim_value_p;
char *icon_line_continue_p;
char *icon_arc_3_points_p;
char *icon_arc_beginning_center_end_p;
char *icon_arc_beginning_center_angle_p;
char *icon_arc_beginning_center_chord_p;
char *icon_arc_beginning_end_radius_p;
char *icon_arc_beginning_end_angle_p;
char *icon_arc_beginning_end_direction_p;
char *icon_arc_continuation_p;
char *icon_text_angle_p;
char *icon_text_width_p;
char *icon_text_type_p;
char *icon_hidden_text_p;
char *icon_text_align_p;
char *icon_text_interval_p;
char *icon_line_polygon_cor_cor_p;
char *icon_line_polygon_cen_tan_p;
char *icon_line_polygon_cen_cor_p;
char *icon_ellipse_approximation_p;
char *icon_ellipse_axis_center_p;
char *icon_ellipse_axis_start_p;
char *icon_ellipse_center_p;
char *icon_solid_quad_triang_p;
char *icon_solid_triangular1_p;
char *icon_solid_fill_line_width_p;
char *icon_solid_quadrangular_p;
char *icon_solid_triangular_p;
char *icon_arc_close_p;
char *icon_arc_remove_p;
char *icon_trace_width_p;
char *icon_trace_axis_offset_p;
char *icon_trace_polyline_p;
char *icon_trace_fill_line_width_p;
char *icon_trace_remove_p;
char *icon_chart_step_dx_p;
char *icon_ftfunct_p;
char *icon_last_block_p;
char *icon_select_all_p;
char *icon_all_layers_p;
char *icon_select_cross_window_p;
char *icon_add_remove_p;
char *icon_gear1_p;
char *icon_text_align_baselineleft_p;
char *icon_text_align_baselineright_p;
char *icon_text_align_baselinecenter_p;
char *icon_text_align_middlecenter_p;
char *icon_circle_tangential_p;

BITMAP *icon_axis_length;
BITMAP *icon_dash_dotted;
BITMAP *icon_dashed;
BITMAP *icon_return_all;
BITMAP *icon_change_mode;
BITMAP *icon_change_property;
BITMAP *icon_all_texts;
BITMAP *icon_select_window;
BITMAP *icon_select_cross;

char *icon_axis_length_p;
char *icon_dash_dotted_p;
char *icon_dashed_p;
char *icon_return_all_p;
char *icon_change_mode_p;
char *icon_change_property_p;
char *icon_all_texts_p;
char *icon_select_window_p;
char *icon_select_cross_p;

BITMAP *icon_solid_filled;
BITMAP *icon_solid_very_thin;
BITMAP *icon_solid_thin;
BITMAP *icon_solid_thick;
BITMAP *icon_solid_very_thick;
BITMAP *icon_solid_extra_thick;
BITMAP *icon_text_angle_0;
BITMAP *icon_text_angle_90;
BITMAP *icon_text_parallel_to_object;
BITMAP *icon_text_perpendicular;
BITMAP *icon_text_angle_to_object;
BITMAP *icon_text_angle_x;

char *icon_solid_filled_p;
char *icon_solid_very_thin_p;
char *icon_solid_thin_p;
char *icon_solid_thick_p;
char *icon_solid_very_thick_p;
char *icon_solid_extra_thick_p;
char *icon_text_angle_0_p;
char *icon_text_angle_90_p;
char *icon_text_parallel_to_object_p;
char *icon_text_perpendicular_p;
char *icon_text_angle_to_object_p;
char *icon_text_angle_x_p;

BITMAP *icon_trace_filled;
BITMAP *icon_trace_very_thin;
BITMAP *icon_trace_thin;
BITMAP *icon_trace_thick;
BITMAP *icon_trace_very_thick;
BITMAP *icon_trace_extra_thick;
BITMAP *icon_point_simple;
BITMAP *icon_point_base;
BITMAP *icon_point_pin;

char *icon_trace_filled_p;
char *icon_trace_very_thin_p;
char *icon_trace_thin_p;
char *icon_trace_thick_p;
char *icon_trace_very_thick_p;
char *icon_trace_extra_thick_p;
char *icon_point_simple_p;
char *icon_point_base_p;
char *icon_point_pin_p;

BITMAP *icon_RAM_for_drawing;
BITMAP *icon_RAM_virtual_image;
BITMAP *icon_RAM_data_size;
BITMAP *icon_RAM_printing_buffer;
BITMAP *icon_RAM_macro_buffer;
BITMAP *icon_RAM_image_buffer;
BITMAP *icon_current_folder;
BITMAP *icon_desktop_size;

char *icon_RAM_for_drawing_p;
char *icon_RAM_virtual_image_p;
char *icon_RAM_data_size_p;
char *icon_RAM_printing_buffer_p;
char *icon_RAM_macro_buffer_p;
char *icon_RAM_image_buffer_p;
char *icon_current_folder_p;
char *icon_desktop_size_p;

BITMAP *icon_layers_info;
BITMAP *icon_x1;
BITMAP *icon_y1;
BITMAP *icon_x2;
BITMAP *icon_y2;
BITMAP *icon_x3;
BITMAP *icon_y3;
BITMAP *icon_x4;
BITMAP *icon_y4;
BITMAP *icon_angle1;
BITMAP *icon_angle2;
BITMAP *icon_dim_dx;
BITMAP *icon_dim_dy;
BITMAP *icon_inner_block_name;
BITMAP *icon_outer_block_name;

char *icon_layers_info_p;
char *icon_x1_p;
char *icon_y1_p;
char *icon_x2_p;
char *icon_y2_p;
char *icon_x3_p;
char *icon_y3_p;
char *icon_x4_p;
char *icon_y4_p;
char *icon_angle1_p;
char *icon_angle2_p;
char *icon_dim_dx_p;
char *icon_dim_dy_p;
char *icon_inner_block_name_p;
char *icon_outer_block_name_p;

BITMAP *icon_qr_code1;
BITMAP *icon_edit;
BITMAP *icon_grey;
BITMAP *icon_blackwhite;
BITMAP *icon_plotter_s;
BITMAP *icon_visible;

char *icon_qr_code1_p;
char *icon_edit_p;
char *icon_grey_p;
char *icon_blackwhite_p;
char *icon_plotter_s_p;
char *icon_visible_p;

BITMAP *icon_visible_d;
BITMAP *icon_edit_d;
BITMAP *icon_pointfinder_d;
BITMAP *icon_color_d;
BITMAP *icon_line_thickness_d;
BITMAP *icon_line_type_d;
BITMAP *icon_grey_d;
BITMAP *icon_blackwhite_d;

char *icon_visible_d_p;
char *icon_edit_d_p;
char *icon_pointfinder_d_p;
char *icon_color_d_p;
char *icon_line_thickness_d_p;
char *icon_line_type_d_p;
char *icon_grey_d_p;
char *icon_blackwhite_d_p;

BITMAP *icon_visible_db;
BITMAP *icon_edit_db;
BITMAP *icon_pointfinder_db;
BITMAP *icon_grey_db;
BITMAP *icon_blackwhite_db;

char *icon_visible_db_p;
char *icon_edit_db_p;
char *icon_pointfinder_db_p;
char *icon_grey_db_p;
char *icon_blackwhite_db_p;

BITMAP *icon_plotter_64;
BITMAP *icon_saveimage_64;

char *icon_plotter_64_p;
char *icon_saveimage_64_p;


BITMAP *icon_rotation_d48;
BITMAP *icon_inversion_d48;
BITMAP *icon_reflection_d48;
BITMAP *icon_grey_d48;
BITMAP *icon_blackwhite_d48;
BITMAP *icon_grey_image_d48;

char *icon_rotation_d48_p;
char *icon_inversion_d48_p;
char *icon_reflection_d48_p;
char *icon_grey_d48_p;
char *icon_blackwhite_d48_p;
char *icon_grey_image_d48_p;

BITMAP *icon_page_d48;
BITMAP *icon_scale_d48;
BITMAP *icon_window_d48;
BITMAP *icon_sheet_d48;
BITMAP *icon_tools;
BITMAP *icon_features;
BITMAP *icon_ctrl;
BITMAP *icon_alt;
BITMAP *icon_shift;
BITMAP *icon_home;
BITMAP *icon_end;
BITMAP *icon_esc;
BITMAP *icon_ins;
BITMAP *icon_pgdn;
BITMAP *icon_pgup;
BITMAP *icon_spc;
BITMAP* icon_del;
BITMAP *icon_tab;
BITMAP* icon_ent;
BITMAP* icon_er;
BITMAP* icon_k_right;
BITMAP* icon_k_left;
BITMAP* icon_k_up;
BITMAP* icon_k_down;
BITMAP *icon_0;
BITMAP *icon_1;
BITMAP *icon_2;
BITMAP *icon_3;
BITMAP *icon_4;
BITMAP *icon_5;
BITMAP *icon_6;
BITMAP *icon_7;
BITMAP *icon_8;
BITMAP *icon_9;
BITMAP *icon_plus;
BITMAP *icon_minus;
BITMAP *icon_f1;
BITMAP *icon_f2;
BITMAP *icon_f3;
BITMAP *icon_f4;
BITMAP *icon_f5;
BITMAP *icon_f6;
BITMAP *icon_f7;
BITMAP *icon_f8;
BITMAP *icon_f9;
BITMAP *icon_f10;
BITMAP *icon_f11;
BITMAP *icon_f12;
BITMAP *icon_mouse3;
BITMAP *icon_mouseR;
BITMAP *icon_measure_tape;
BITMAP *icon_spa;
BITMAP *icon_ce;
BITMAP *icon_look_down;


char *icon_page_d48_p;
char *icon_scale_d48_p;
char *icon_window_d48_p;
char *icon_sheet_d48_p;
char *icon_tools_p;
char *icon_features_p;
char *icon_ctrl_p;
char *icon_alt_p;
char *icon_shift_p;
char *icon_home_p;
char *icon_end_p;
char *icon_esc_p;
char *icon_ins_p;
char *icon_pgdn_p;
char *icon_pgup_p;
char *icon_spc_p;
char* icon_del_p;
char *icon_tab_p;
char* icon_ent_p;
char* icon_er_p;
char* icon_k_right_p;
char* icon_k_left_p;
char* icon_k_up_p;
char* icon_k_down_p;
char *icon_0_p;
char *icon_1_p;
char *icon_2_p;
char *icon_3_p;
char *icon_4_p;
char *icon_5_p;
char *icon_6_p;
char *icon_7_p;
char *icon_8_p;
char *icon_9_p;
char *icon_plus_p;
char *icon_minus_p;
char *icon_f1_p;
char *icon_f2_p;
char *icon_f3_p;
char *icon_f4_p;
char *icon_f5_p;
char *icon_f6_p;
char *icon_f7_p;
char *icon_f8_p;
char *icon_f9_p;
char *icon_f10_p;
char *icon_f11_p;
char *icon_f12_p;
char *icon_mouse3_p;
char *icon_mouseR_p;
char *icon_measure_tape_p;
char *icon_spa_p;
char *icon_ce_p;
char *icon_look_down_p;

BITMAP *icon_plotter_32;
char *icon_plotter_32_p;

BITMAP *icon_cartridge_d48;
BITMAP *icon_very_thin_d;
BITMAP *icon_thin_d;
BITMAP *icon_thick_d;
BITMAP *icon_very_thick_d;
BITMAP *icon_extra_thick_d;
BITMAP *icon_color_d48;

char *icon_cartridge_d48_p;
char *icon_very_thin_d_p;
char *icon_thin_d_p;
char *icon_thick_d_p;
char *icon_very_thick_d_p;
char *icon_extra_thick_d_p;
char *icon_color_d48_p;

BITMAP *icon_scale_d;
BITMAP *icon_page_d;
BITMAP *icon_page_width_d;
BITMAP *icon_page_height_d;
BITMAP *icon_page_overlap_d;

char *icon_scale_d_p;
char *icon_page_d_p;
char *icon_page_width_d_p;
char *icon_page_height_d_p;
char *icon_page_overlap_d_p;

BITMAP *icon_color256_d48;
char *icon_color256_d48_p;

BITMAP *icon_multiline_text;
BITMAP *icon_underlined;
BITMAP *icon_line_spacing;
char *icon_multiline_text_p;
char *icon_underlined_p;
char *icon_line_spacing_p;

BITMAP *icon_bold_d;
BITMAP *icon_font_type_d;
BITMAP *icon_hidden_text_d;
BITMAP *icon_italic_d;
BITMAP *icon_layers_d;
BITMAP *icon_text_align_d;
BITMAP *icon_text_height_d;
BITMAP *icon_text_type_d;
BITMAP *icon_width_d;
BITMAP *icon_underlined_d;
BITMAP *icon_yes_d_mem;
BITMAP *icon_no_d_mem;
BITMAP *icon_no_d_12;
BITMAP *icon_mark_d_12;

char *icon_bold_d_p;
char *icon_font_type_d_p;
char *icon_hidden_text_d_p;
char *icon_italic_d_p;
char *icon_layers_d_p;
char *icon_text_align_d_p;
char *icon_text_height_d_p;
char *icon_text_type_d_p;
char *icon_width_d_p;
char *icon_underlined_d_p;
char *icon_yes_d_pmem;
char *icon_no_d_pmem;
char *icon_no_d_12_p;
char *icon_mark_d_12_p;

BITMAP *icon_hor_dpi_sphere_blue_d;
BITMAP *icon_ver_dpi_sphere_blue_d;
BITMAP *icon_hor_size_image_d;
BITMAP *icon_ver_size_image_d;
BITMAP *icon_hor_scale_image_d;
BITMAP *icon_ver_scale_image_d;
BITMAP *icon_background_image_d;
BITMAP* icon_background_image_d48;
BITMAP *icon_rotate_angle_image_d;

char *icon_hor_dpi_sphere_blue_d_p;
char *icon_ver_dpi_sphere_blue_d_p;
char *icon_hor_size_image_d_p;
char *icon_ver_size_image_d_p;
char *icon_hor_scale_image_d_p;
char *icon_ver_scale_image_d_p;
char *icon_background_image_d_p;
char* icon_background_image_d48_p;
char *icon_rotate_angle_image_d_p;

BITMAP *icon_top_margin_d;
BITMAP *icon_left_margin_d;
BITMAP *icon_bottom_margin_d;
BITMAP *icon_right_margin_d;
BITMAP *icon_actual_size_d48;


char *icon_top_margin_d_p;
char *icon_left_margin_d_p;
char *icon_bottom_margin_d_p;
char *icon_right_margin_d_p;
char *icon_actual_size_d48_p;

BITMAP *icon_pattern;
BITMAP *icon_pattern_angle;
BITMAP *icon_pattern_scale;
BITMAP *icon_edit_point_origin;
BITMAP *icon_set_point_origin;
BITMAP *icon_pattern_line_distance;

char *icon_pattern_p;
char *icon_pattern_angle_p;
char *icon_pattern_scale_p;
char *icon_edit_point_origin_p;
char *icon_set_point_origin_p;
char *icon_pattern_line_distance_p;

BITMAP *icon_anglex;
BITMAP *icon_angle45;
BITMAP *icon_angle90;
BITMAP *icon_angle135;
BITMAP *icon_angle180;
BITMAP *icon_angle225;
BITMAP *icon_angle270;
BITMAP *icon_angle315;

char *icon_anglex_p;
char *icon_angle45_p;
char *icon_angle90_p;
char *icon_angle135_p;
char *icon_angle180_p;
char *icon_angle225_p;
char *icon_angle270_p;
char *icon_angle315_p;

BITMAP *icon_divide_count;
BITMAP *icon_divide_counter;
BITMAP *icon_divide_measure;
BITMAP *icon_divide_segment_length;
BITMAP *icon_divide_counter_max;
BITMAP *icon_divide_polyline_segment;
BITMAP *icon_divide_polyline;
BITMAP *icon_divide_segment;

char *icon_divide_count_p;
char *icon_divide_counter_p;
char *icon_divide_measure_p;
char *icon_divide_segment_length_p;
char *icon_divide_counter_max_p;
char *icon_divide_polyline_segment_p;
char *icon_divide_polyline_p;
char *icon_divide_segment_p;

BITMAP *icon_align_block;
BITMAP *icon_offset_point;
BITMAP *icon_offset_distance;
BITMAP *icon_set_offset_distance;
BITMAP *icon_return;

char *icon_align_block_p;
char *icon_offset_point_p;
char *icon_offset_distance_p;
char *icon_set_offset_distance_p;
char *icon_return_p;

BITMAP *icon_open_file_folder;
char *icon_open_file_folder_p;

BITMAP *icon_arrow_up_d48;
BITMAP* icon_arrow_up_end_d48;
BITMAP* icon_arrow_down_end_d48;
BITMAP *icon_arrow_down_d48;
BITMAP *icon_folder_d48;
BITMAP *icon_folder0_d48;
BITMAP *icon_smaller_d48;
BITMAP *icon_bigger_d48;

char *icon_arrow_up_d48_p;
char* icon_arrow_up_end_d48_p;
char* icon_arrow_down_end_d48_p;
char *icon_arrow_down_d48_p;
char *icon_folder_d48_p;
char *icon_folder0_d48_p;
char *icon_smaller_d48_p;
char *icon_bigger_d48_p;

BITMAP *icon_expand_hor;
BITMAP *icon_expand_ver;
BITMAP *icon_expand_diag;
BITMAP *icon_expand_flex;
BITMAP *icon_expand_last;
BITMAP *icon_expand_win;
BITMAP *icon_arrow_up_d;
BITMAP *icon_arrow_down_d;
BITMAP *icon_pgup_d;
BITMAP *icon_pgdn_d;
BITMAP *icon_arrow_up_left_d;
BITMAP *icon_arrow_down_left_d;

char *icon_expand_hor_p;
char *icon_expand_ver_p;
char *icon_expand_diag_p;
char *icon_expand_flex_p;
char *icon_expand_last_p;
char *icon_expand_win_p;
char *icon_arrow_up_d_p;
char *icon_arrow_down_d_p;
char *icon_pgup_d_p;
char *icon_pgdn_d_p;
char *icon_arrow_up_left_d_p;
char *icon_arrow_down_left_d_p;

BITMAP *icon_plotter_start_d_64;
BITMAP *icon_printer_start_d_64;
BITMAP *icon_printer_d_48;
BITMAP *icon_escape_d_64;
BITMAP *icon_save_d;
BITMAP *icon_load_d;
BITMAP *icon_return_d_24;
BITMAP *icon_frame;
BITMAP *icon_zones_around;
BITMAP *icon_zones_prefix;
BITMAP *icon_zones_first_number;
BITMAP *icon_zones_reversed;
BITMAP *icon_zone_height;
BITMAP *icon_zone_width;
BITMAP *icon_margin_width;
BITMAP *icon_frame_line;
BITMAP *icon_zones_line;
BITMAP *icon_frame_color;
BITMAP *icon_zones_color;
BITMAP *icon_style2_black;
BITMAP *icon_zones_top;
BITMAP *icon_zones_bottom;
BITMAP *icon_zones_top_bottom;
BITMAP *icon_frame_offset;

char *icon_plotter_start_d_64_p;
char *icon_printer_start_d_64_p;
char *icon_printer_d_48_p;
char *icon_escape_d_64_p;
char *icon_save_d_p;
char *icon_load_d_p;
char *icon_return_d_24_p;
char *icon_frame_p;
char *icon_zones_around_p;
char *icon_zones_prefix_p;
char *icon_zones_first_number_p;
char *icon_zones_reversed_p;
char *icon_zone_height_p;
char *icon_zone_width_p;
char *icon_margin_width_p;
char *icon_frame_line_p;
char *icon_zones_line_p;
char *icon_frame_color_p;
char *icon_zones_color_p;
char *icon_style2_black_p;
char *icon_zones_top_p;
char *icon_zones_bottom_p;
char *icon_zones_top_bottom_p;
char *icon_frame_offset_p;

BITMAP *icon_ok_d_64;
BITMAP *icon_new_layer_d_64;

char *icon_ok_d_64_p;
char *icon_new_layer_d_64_p;

BITMAP *icon_hor_extents_d;
BITMAP *icon_ver_extents_d;
BITMAP *icon_hor_limits_d;
BITMAP *icon_ver_limits_d;
BITMAP *icon_units_d;
BITMAP *icon_paper_size_d_48;
BITMAP *icon_paper_size_d;
BITMAP *icon_paper_dim_d_48;

char *icon_hor_extents_d_p;
char *icon_ver_extents_d_p;
char *icon_hor_limits_d_p;
char *icon_ver_limits_d_p;
char *icon_units_d_p;
char *icon_paper_size_d_48_p;
char *icon_paper_size_d_p;
char *icon_paper_dim_d_48_p;

BITMAP *icon_bspline;
BITMAP *icon_bspline34;
BITMAP *icon_bspline3p;
BITMAP *icon_bspline4;
BITMAP *icon_bspline3;

char *icon_bspline_p;
char *icon_bspline34_p;
char *icon_bspline3p_p;
char *icon_bspline4_p;
char *icon_bspline3_p;

BITMAP *icon_yes_d_48;
BITMAP *icon_no_d_48;
BITMAP *icon_escape_d_48;
BITMAP *icon_escape_d;

char *icon_yes_d_48_p;
char *icon_no_d_48_p;
char *icon_escape_d_48_p;
char *icon_escape_d_p;

BITMAP *icon_tartan;
BITMAP *icon_all_windows;

char *icon_tartan_p;
char *icon_all_windows_p;

BITMAP *icon_junction;
BITMAP *icon_save_window;

char *icon_junction_p;
char *icon_save_window_p;

BITMAP *icon_freehand_segment_length;
BITMAP *icon_remove_last_freehand;

char *icon_freehand_segment_length_p;
char *icon_remove_last_freehand_p;

BITMAP *icon_new_from_template;
char *icon_new_from_template_p;

BITMAP *icon_idea;
char *icon_idea_p;

BITMAP *icon_mirror_block;
char *icon_mirror_block_p;

 BITMAP *icon_tree;
 BITMAP *icon_tree_enter;
 BITMAP *icon_tree_return;

 char *icon_tree_p;
 char *icon_tree_enter_p;
 char *icon_tree_return_p;


 BITMAP *icon_trans30;
 BITMAP *icon_trans40;
 BITMAP *icon_trans50;
 BITMAP *icon_trans60;
 BITMAP *icon_trans70;
 BITMAP *icon_trans80;
 BITMAP *icon_trans90;
 BITMAP *icon_trans100;

 BITMAP* icon_trans05;
 BITMAP* icon_trans10;
 BITMAP* icon_trans15;
 BITMAP* icon_trans20;
 BITMAP* icon_trans25;
 BITMAP* icon_trans35;
 BITMAP* icon_trans45;
 BITMAP* icon_trans55;
 BITMAP* icon_trans65;
 BITMAP* icon_trans75;
 BITMAP* icon_trans85;
 BITMAP* icon_trans95;
 

 char *icon_trans30_p;
 char *icon_trans40_p;
 char *icon_trans50_p;
 char *icon_trans60_p;
 char *icon_trans70_p;
 char *icon_trans80_p;
 char *icon_trans90_p;
 char *icon_trans100_p;

 char* icon_trans05_p;
 char* icon_trans10_p;
 char* icon_trans15_p;
 char* icon_trans20_p;
 char* icon_trans25_p;
 char* icon_trans35_p;
 char* icon_trans45_p;
 char* icon_trans55_p;
 char* icon_trans65_p;
 char* icon_trans75_p;
 char* icon_trans85_p;
 char* icon_trans95_p;

 BITMAP* icon_trans50_d;
 char* icon_trans50_d_p;

 BITMAP *icon_back;
 BITMAP *icon_space;

 char *icon_back_p;
 char *icon_space_p;

 BITMAP *icon_A;
 char *icon_A_p;
 BITMAP *icon_B;
 char *icon_B_p;
 BITMAP *icon_C;
 char *icon_C_p;
 BITMAP *icon_D;
 char *icon_D_p;
 BITMAP *icon_E;
 char *icon_E_p;
 BITMAP *icon_F;
 char *icon_F_p;
 BITMAP *icon_G;
 char *icon_G_p;
 BITMAP *icon_H;
 char *icon_H_p;
 BITMAP *icon_I;
 char *icon_I_p;
 BITMAP *icon_J;
 char *icon_J_p;
 BITMAP *icon_K;
 char *icon_K_p;
 BITMAP *icon_L;
 char *icon_L_p;
 BITMAP *icon_M;
 char *icon_M_p;
 BITMAP *icon_N;
 char *icon_N_p;
 BITMAP *icon_O;
 char *icon_O_p;
 BITMAP *icon_P;
 char *icon_P_p;
 BITMAP *icon_Q;
 char *icon_Q_p;
 BITMAP *icon_R;
 char *icon_R_p;
 BITMAP *icon_S;
 char *icon_S_p;
 BITMAP *icon_T;
 char *icon_T_p;
 BITMAP *icon_U;
 char *icon_U_p;
 BITMAP *icon_V;
 char *icon_V_p;
 BITMAP *icon_W;
 char *icon_W_p;
 BITMAP *icon_X;
 char *icon_X_p;
 BITMAP *icon_Y;
 char *icon_Y_p;
 BITMAP *icon_Z;
 char *icon_Z_p;


 char *icon_A_p;
 char *icon_A_p;
 char *icon_A_p;
 char *icon_A_p;
 char *icon_A_p;

 BITMAP *icon_load_d_48;
 BITMAP *icon_load_ini_d;
 BITMAP *icon_load_ini_d_48;

 char *icon_load_d_48_p;
 char *icon_load_ini_d_p;
 char *icon_load_ini_d_48_p;

 BITMAP *icon_angle0;
 BITMAP *icon_angleRX;
 BITMAP *icon_anglePto;
 BITMAP *icon_angleRto;
 BITMAP *icon_angleXtoP;

 char *icon_angle0_p;
 char *icon_angleRX_p;
 char *icon_anglePto_p;
 char *icon_angleRto_p;
 char *icon_angleXtoP_p;
 
 BITMAP *icon_touch_id_db_64;
 BITMAP *icon_left_margin_d_48;
 BITMAP *icon_bottom_margin_d_48;
 BITMAP *icon_touch_id_db_64;

 char *icon_touch_id_db_64_p;
 char *icon_left_margin_d_48_p;
 char *icon_bottom_margin_d_48_p;

 BITMAP *icon_all_layers_db_64;
 char *icon_all_layers_db_64_p;

 BITMAP *icon_global_db_48;
 BITMAP *icon_local_db_48;
 char *icon_global_db_48_p;
 char *icon_local_db_48_p;

 BITMAP *icon_color256_db_48;
 char *icon_color256_db_48_p;

 BITMAP *icon_cursors;
 BITMAP *icon_cursor_small;
 BITMAP *icon_cursor_big;
 BITMAP *icon_cursor_extrabig;

 char *icon_cursors_p;
 char *icon_cursor_small_p;
 char *icon_cursor_big_p;
 char *icon_cursor_extrabig_p;

 BITMAP *icon_question_mark_db_48;
 char *icon_question_mark_db_48_p;

 BITMAP *icon_pointern;
 BITMAP *icon_pointerb;

 char *icon_pointern_p;
 char *icon_pointerb_p;

 BITMAP *icon_pdf_vector_64;
 char *icon_pdf_vector_64_p;

 BITMAP *icon_pattern_folder;
 char *icon_pattern_folder_p;

 BITMAP *icon_close_window;
 char *icon_close_window_p;

 BITMAP *icon_change_pattern;
 char *icon_change_pattern_p;

 BITMAP *icon_import_map;
 char *icon_import_map_p;

 BITMAP *icon_starAstar;
 BITMAP *icon_starA;
 BITMAP *icon_Astar;
 BITMAP *icon_Aonly;
 BITMAP *icon_gear;
 BITMAP *icon_confirm_or_deny;

 char *icon_starAstar_p;
 char *icon_starA_p;
 char *icon_Astar_p;
 char *icon_Aonly_p;
 char *icon_gear_p;
 char *icon_confirm_or_deny_p;

 BITMAP *icon_stonewall;
 char* icon_stonewall_p;

BITMAP* icon_folder_bd24;
char* icon_folder_bd24_p;
 BITMAP* icon_folder_bd32;
 char* icon_folder_bd32_p;
 BITMAP* icon_folder_bd48;
 char* icon_folder_bd48_p;
 BITMAP* icon_folder_bd64;
 char* icon_folder_bd64_p;

 BITMAP* icon_brickwall_dx;
 char* icon_brickwall_dx_p;

 BITMAP* icon_brickwall_dy;
 char* icon_brickwall_dy_p;

 BITMAP* icon_chain_d;
 char* icon_chain_d_p;

 BITMAP* icon_on_top_d;
 char* icon_on_top_d_p;


 BITMAP* icon_h_flip;
 char* icon_h_flip_p;
 BITMAP* icon_v_flip;
 char* icon_v_flip_p;

 BITMAP* icon_offset_style;
 char* icon_offset_style_p;
 BITMAP* icon_offset_normal;
 char* icon_offset_normal_p;
 BITMAP* icon_offset_smooth;
 char* icon_offset_smooth_p;

 BITMAP* icon_trace_close;
 char* icon_trace_close_p;
 BITMAP* icon_trace_break;
 char* icon_trace_break_p;


 BITMAP* icon_UA_B;
 BITMAP* icon_UA_D;
 BITMAP* icon_UA_E;
 BITMAP* icon_UA_J;
 BITMAP* icon_UA_K;
 BITMAP* icon_UA_N;
 BITMAP* icon_UA_P;
 BITMAP* icon_UA_R;
 BITMAP* icon_UA_S;
 BITMAP* icon_UA_T;
 BITMAP* icon_UA_C;
 BITMAP* icon_UA_JA;
 BITMAP* icon_UA_I;
 BITMAP* icon_UA_O;

 char* icon_UA_B_p;
 char* icon_UA_D_p;
 char* icon_UA_E_p;
 char* icon_UA_J_p;
 char* icon_UA_K_p;
 char* icon_UA_N_p;
 char* icon_UA_P_p;
 char* icon_UA_R_p;
 char* icon_UA_S_p;
 char* icon_UA_T_p;
 char* icon_UA_C_p;
 char* icon_UA_JA_p;
 char* icon_UA_I_p;
 char* icon_UA_O_p;

 BITMAP* icon_spline_points;
 char* icon_spline_points_p;

 BITMAP* icon_spline_control_points;
 char* icon_spline_control_points_p;

 BITMAP* icon_spline_points_close;
 char* icon_spline_points_close_p;

 BITMAP* icon_spline_amend;
 char* icon_spline_amend_p;

 BITMAP* icon_spline_points_end;
 char* icon_spline_points_end_p;

 BITMAP* icon_mouse1b;
 BITMAP* icon_mouse2b;
 BITMAP* icon_mouse1b2b;
 BITMAP* icon_mouse3b;
 BITMAP* icon_mouseRb;

 char* icon_mouse1b_p;
 char* icon_mouse2b_p;
 char* icon_mouse1b2b_p;
 char* icon_mouse3b_p;
 char* icon_mouseRb_p;

 BITMAP* icon_education;
 char* icon_education_p;

 BITMAP* icon_arrows;
 BITMAP* icon_arrows_w;
 char* icon_arrows_p;
 char* icon_arrows_w_p;

 BITMAP* icon_mickey_hand;
 char* icon_mickey_hand_p;
 BITMAP* icon_mickey_hand_s;
 char* icon_mickey_hand_s_p;

 BITMAP* icon_dim_leader;
 char* icon_dim_leader_p;

 BITMAP* icon_insulation;
 char* icon_insulation_p;

 BITMAP* icon_ins_simple;
 char* icon_ins_simple_p;
 BITMAP* icon_ins_complex;
 char* icon_ins_complex_p;
 BITMAP* icon_quad_trans;
 char* icon_quad_trans_p;

BITMAP* icon_rect_angle_opt;
char* icon_rect_angle_opt_p;
BITMAP* icon_move_rectangle;
char* icon_move_rectangle_p;
BITMAP* icon_rect_options;
char* icon_rect_options_p;

BITMAP* icon_object_polyline;
char* icon_object_polyline_p;
BITMAP* icon_entire_polyline;
char* icon_entire_polyline_p;
BITMAP* icon_single_object;
char* icon_single_object_p;

BITMAP* icon_image_dx;
char* icon_image_dx_p;
BITMAP* icon_image_dy;
char* icon_image_dy_p;

BITMAP* icon_break_options;
char* icon_break_options_p;
BITMAP* icon_break_and_leave;
char* icon_break_and_leave_p;
BITMAP* icon_break_and_cut;
char* icon_break_and_cut_p;
BITMAP* icon_break_divide;
char* icon_break_divide_p;

BITMAP* icon_fixed_scale;
char* icon_fixed_scale_p;

BITMAP* icon_text_collineal;
char* icon_text_collineal_p;

BITMAP* icon_decimal_fractional;
char* icon_decimal_fractional_p;

BITMAP* icon_import_vectorize;
char* icon_import_vectorize_p;

BITMAP* icon_space128;
char* icon_space128_p;
BITMAP* icon_backspace128;
char* icon_backspace128_p;
BITMAP* icon_enter128;
char* icon_enter128_p;

BITMAP* icon_ttf;
char* icon_ttf_p;
BITMAP* icon_otf;
char* icon_otf_p;

BITMAP *icon_elliptical;
char *icon_elliptical_p;
BITMAP *icon_ellipticalarc;
char *icon_ellipticalarc_p;
BITMAP *icon_filledellipses;
char *icon_filledellipses_p;

BITMAP *icon_ellipticalarclocal;
char *icon_ellipticalarclocal_p;
BITMAP *icon_ellipticalarcglobal;
char *icon_ellipticalarcglobal_p;

BITMAP *icon_arc_line_continuation;
char *icon_arc_line_continuation_p;
BITMAP *icon_line_arc_continuation;
char *icon_line_arc_continuation_p;

BITMAP *icon_center_block;
char *icon_center_block_p;
BITMAP *icon_alpha_sorting_d32;
char *icon_alpha_sorting_d32_p;
BITMAP *icon_time_lapse_d32;
char *icon_time_lapse_d32_p;

BITMAP *icon_select_entity;
char *icon_select_entity_p;
BITMAP *icon_select;
char *icon_select_p;

BITMAP *icon_vector;
char *icon_vector_p;
BITMAP *icon_rigid_rigid;
char *icon_rigid_rigid_p;
BITMAP *icon_rigid_pin;
char *icon_rigid_pin_p;
BITMAP *icon_pin_rigid;
char *icon_pin_rigid_p;
BITMAP *icon_pin_pin;
char *icon_pin_pin_p;
BITMAP *icon_pin_pin;
char *icon_pin_pin_p;
BITMAP *icon_force;
char *icon_force_p;
BITMAP *icon_moment;
char *icon_moment_p;
BITMAP *icon_moment_rev;
char *icon_moment_rev_p;
BITMAP *icon_displacement;
char *icon_displacement_p;
BITMAP *icon_rotation;
char *icon_rotation_p;
BITMAP *icon_rotation_rev;
char *icon_rotation_rev_p;
BITMAP *icon_trapezium_y;
char *icon_trapezium_y_p;
BITMAP *icon_trapezium_x;
char *icon_trapezium_x_p;
BITMAP *icon_trapezium_n;
char *icon_trapezium_n_p;
BITMAP *icon_trapezium_h;
char *icon_trapezium_h_p;
BITMAP *icon_trapezium_v;
char *icon_trapezium_v_p;

BITMAP *icon_fixed_h;
char *icon_fixed_h_p;
BITMAP *icon_fixed_vl;
char *icon_fixed_vl_p;
BITMAP *icon_fixed_vr;
char *icon_fixed_vr_p;

BITMAP *icon_pinned_h;
char *icon_pinned_h_p;
BITMAP *icon_pinned_vl;
char *icon_pinned_vl_p;
BITMAP *icon_pinned_vr;
char *icon_pinned_vr_p;

BITMAP *icon_fixed_roller_h;
char *icon_fixed_roller_h_p;
BITMAP *icon_fixed_roller_vl;
char *icon_fixed_roller_vl_p;
BITMAP *icon_fixed_roller_vr;
char *icon_fixed_roller_vr_p;

BITMAP *icon_pinned_roller_h;
char *icon_pinned_roller_h_p;
BITMAP *icon_pinned_roller_vl;
char *icon_pinned_roller_vl_p;
BITMAP *icon_pinned_roller_vr;
char *icon_pinned_roller_vr_p;

BITMAP *icon_node;
char *icon_node_p;
BITMAP *icon_magnitude;
char *icon_magnitude_p;

BITMAP *icon_thermal;
char *icon_thermal_p;

BITMAP *icon_gear_plus;
char *icon_gear_plus_p;

BITMAP *icon_fixed_hu;
char *icon_fixed_hu_p;
BITMAP *icon_pinned_hu;
char *icon_pinned_hu_p;
BITMAP *icon_fixed_roller_hu;
char *icon_fixed_roller_hu_p;
BITMAP *icon_pinned_roller_hu;
char *icon_pinned_roller_hu_p;
BITMAP *icon_fixed_rotation;
char *icon_fixed_rotation_p;

BITMAP *icon_compression_mag;
char *icon_compression_mag_p;
BITMAP *icon_tension_mag;
char *icon_tension_mag_p;
BITMAP *icon_shear_mag;
char *icon_shear_mag_p;
BITMAP *icon_moments_mag;
char *icon_moments_mag_p;
BITMAP *icon_displacements_mag;
char *icon_displacements_mag_p;
BITMAP *icon_reactions_xy;
char *icon_reactions_xy_p;
BITMAP *icon_reactions_m;
char *icon_reactions_m_p;

BITMAP *icon_thermal_y;
char *icon_thermal_y_p;
BITMAP *icon_thermal_t;
char *icon_thermal_t_p;

BITMAP *icon_trapezium_y1;
char *icon_trapezium_y1_p;
BITMAP *icon_trapezium_y2;
char *icon_trapezium_y2_p;
BITMAP *icon_thermal_t1;
char *icon_thermal_t1_p;
BITMAP *icon_thermal_t2;
char *icon_thermal_t2_p;

BITMAP *icon_vectors;
char *icon_vectors_p;

BITMAP *icon_US_Flag;
char *icon_US_Flag_p;
BITMAP *icon_EU_Flag;
char *icon_EU_Flag_p;
BITMAP *icon_UK_Flag;
char *icon_UK_Flag_p;

BITMAP *icon_CA_Flag;
char *icon_CA_Flag_p;
BITMAP *icon_AU_Flag;
char *icon_AU_Flag_p;
BITMAP *icon_CN_Flag;
char *icon_CN_Flag_p;

BITMAP *icon_IH_sections;
char *icon_IH_sections_p;
BITMAP *icon_U_sections;
char *icon_U_sections_p;
BITMAP *icon_T_sections;
char *icon_T_sections_p;
BITMAP *icon_L_sections;
char *icon_L_sections_p;
BITMAP *icon_2L_sections;
char *icon_2L_sections_p;
BITMAP *icon_RT_sections;
char *icon_RT_sections_p;
BITMAP *icon_O_sections;
char *icon_O_sections_p;
BITMAP *icon_EO_sections;
char *icon_EO_sections_p;
BITMAP *icon_Z_sections;
char *icon_Z_sections_p;
BITMAP *icon_VJ_sections;
char *icon_VJ_sections_p;

BITMAP *icon_timber;
char *icon_timber_p;

BITMAP *icon_node_size;
char *icon_node_size_p;

BITMAP *icon_dead_load;
char *icon_dead_load_p;
BITMAP *icon_live_load;
char *icon_live_load_p;
BITMAP *icon_roof_load;
char *icon_roof_load_p;
BITMAP *icon_wind_load;
char *icon_wind_load_p;
BITMAP *icon_snow_load;
char *icon_snow_load_p;
BITMAP *icon_earthquake_load;
char *icon_earthquake_load_p;
BITMAP *icon_rain_load;
char *icon_rain_load_p;
BITMAP *icon_soil_load;
char *icon_soil_load_p;
BITMAP *icon_fluid_load;
char *icon_fluid_load_p;
BITMAP *icon_thermal_load;
char *icon_thermal_load_p;

BITMAP *icon_unspecified_load;
char *icon_unspecified_load_p;
BITMAP *icon_unknown_load;
char *icon_unknown_load_p;
BITMAP *icon_number_load;
char *icon_number_load_p;

BITMAP *icon_stress_mag;
char *icon_stress_mag_p;
BITMAP *icon_stress_plus_mag;
char *icon_stress_plus_mag_p;
BITMAP *icon_stress_minus_mag;
char *icon_stress_minus_mag_p;
BITMAP *icon_shear_stress_mag;
char *icon_shear_stress_mag_p;

BITMAP *icon_eurocode_d48;
char *icon_eurocode_d48_p;
BITMAP *icon_asce_d48;
char *icon_asce_d48_p;
BITMAP *icon_icc_d48;
char *icon_icc_d48_p;

BITMAP *icon_combination_d48;
char *icon_combination_d48_p;

BITMAP *icon_erase_layer_db_64;
char *icon_erase_layer_db_64_p;
BITMAP *icon_mark_layer_db_64;
char *icon_mark_layer_db_64_p;

BITMAP *icon_AlfaCAD48;
char *icon_AlfaCAD48_p;

BITMAP *icon_Pdelta_d48;
char *icon_Pdelta_d48_p;

BITMAP *icon_dynamics;
char *icon_dynamics_p;

BITMAP *icon_vibrations_d48;
char *icon_vibrations_d48_p;
BITMAP *icon_inertia_d48;
char *icon_inertia_d48_p;

BITMAP *icon_dynamics_run;
char *icon_dynamics_run_p;

BITMAP *icon_menustyle;
char *icon_menustyle_p;
BITMAP *icon_cursorstyle;
char *icon_cursorstyle_p;
BITMAP *icon_barstyle;
char *icon_barstyle_p;

BITMAP *icon_perc_mag;
char *icon_perc_mag_p;

BITMAP *icon_cross_section_forces;
char *icon_cross_section_forces_p;

BITMAP *icon_resilience;
char *icon_resilience_p;

BITMAP *icon_hourglass;
char *icon_hourglass_p;
BITMAP *icon_hourglass_mem;
BITMAP *icon_hourglassx1_5_mem;
BITMAP *icon_hourglassx2_mem;

BITMAP *icon_cursor_12x16_mem;
BITMAP *icon_cursor_18x24_mem;
BITMAP *icon_cursor_24x32_mem;
BITMAP *icon_cursor_36x48_mem;
BITMAP *icon_cursor_48x64_mem;
BITMAP *icon_cursor_72x96_mem;
BITMAP *icon_cursor_96x128_mem;

BITMAP *icon_edit_cursor_4x16_mem;
BITMAP *icon_edit_cursor_6x24_mem;
BITMAP *icon_edit_cursor_8x32_mem;
BITMAP *icon_edit_cursor_12x48_mem;
BITMAP *icon_edit_cursor_16x64_mem;
BITMAP *icon_edit_cursor_24x96_mem;
BITMAP *icon_edit_cursor_32x128_mem;

BITMAP *icon_ULS;
char *icon_ULS_p;
BITMAP *icon_SLS;
char *icon_SLS_p;
BITMAP *icon_QPSLS;
char *icon_QPSLS_p;

BITMAP *icon_mousewheel;
char *icon_mousewheel_p;
BITMAP *icon_mousewheelnatural;
char *icon_mousewheelnatural_p;
BITMAP *icon_mousewheelregular;
char *icon_mousewheelregular_p;

BITMAP *icon_pin_to_flex_d48;
char *icon_pin_to_flex_d48_p;

BITMAP *icon_slab_zone;
char *icon_slab_zone_p;
BITMAP *icon_slab_wall;
char *icon_slab_wall_p;
BITMAP *icon_slab_space;
char *icon_slab_space_p;
BITMAP *icon_slab_load;
char *icon_slab_load_p;
BITMAP *icon_slab_geo_red;
char *icon_slab_geo_red_p;
BITMAP *icon_slab_geo_black;
char *icon_slab_geo_black_p;
BITMAP *icon_slab_fem;
char *icon_slab_fem_p;
BITMAP *icon_slab_edge_rolled;
char *icon_slab_edge_rolled_p;
BITMAP *icon_slab_edge_hinged;
char *icon_slab_edge_hinged_p;
BITMAP *icon_slab_edge_free;
char *icon_slab_edge_free_p;
BITMAP *icon_slab_edge_fixed;
char *icon_slab_edge_fixed_p;
BITMAP *icon_flip_support;
char *icon_flip_support_p;
BITMAP *icon_static;
char *icon_static_p;
BITMAP *icon_slab_fem_a;
char *icon_slab_fem_a_p;


BITMAP *dump_bitmap[MAX_NUMBER_OF_WINDOWS] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
							/*NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
							NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
							NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,*/
							};
char *dump_bitmap_p[MAX_NUMBER_OF_WINDOWS]= { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
							/*NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
							NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
							NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,*/
};

void out_file_name (void) ;


POLE pmAcadV[]={
	{u8"[1] Acad\0   2.6",'1',0,NULL},
	{u8"[2] Acad\0 10,11",'2',0,NULL},
	};

static TMENU mAcadV={2,0,0,16,20,7,TADD,CMNU,CMBR,CMTX,0,14,0,0,0,(POLE(*)[]) &pmAcadV,NULL,NULL};


TMENU mQuit = {14, 0, 0, 28, 1, 3, ICONS | TADD, CMNU, CMBR, CMTX, 0, 0, 0, 0,0,(POLE(*)[]) &pmQuit, NULL, NULL};


static POLE pmCzcionkaEkran[]={
   {u8"[0] pc8x14",'0',0,NULL},
   {u8"[1] pc8x14t",'1',0,NULL},
   {u8"[2] pc8x16",'2',0,NULL},
   {u8"[3] px8x18",'3',0,NULL},
   {u8"[4] px11x22",'4',0,NULL},
   {u8"[5] px14x28",'5',0,NULL},
   {u8"[6] xm7x12",'6',0,NULL},
   {u8"[7] xm8x12",'7',0,NULL},
   {u8"[8] xm8x12b",'8',0,NULL},
   {u8"[9] xm8x13",'9',0,NULL},
   {u8"[A] xm8x13b",'A',0,NULL},
   {u8"[B] xm8x14",'B',0,NULL},
   {u8"[C] xm8x14b",'C',0,NULL},
   {u8"[D] xm8x15",'D',0,NULL},
   {u8"[E] xm8x15b",'E',0,NULL},
   {u8"[F] xm8x16",'F',0,NULL},
   {u8"[G] xm8x16b",'G',0,NULL},
   {u8"[H] xm8x16i",'H',0,NULL},
   {u8"[I] xm8x19",'I',0,NULL},
   {u8"[J] xm9x15",'J',0,NULL},
   {u8"[K] xm9x15b",'K',0,NULL},
   {u8"[L] xm9x17",'L',0,NULL},
   {u8"[M] xm10x17",'M',0,NULL},
   {u8"[N] xm10x17b",'N',0,NULL},
   {u8"[O] xm10x20",'O',0,NULL},
   {u8"[P] xm10x20b",'P',0,NULL},
   {u8"[Q] xm11x19b",'Q',0,NULL},
   {u8"[R] xm12x15",'R',0,NULL},
   {u8"[S] xm12x20",'S',0,NULL},
   {u8"[T] xm12x20b",'T',0,NULL},
   {u8"[U] xm12x23",'U',0,NULL},
   {u8"[V] xm12x24",'V',0,NULL},
   {u8"[W] xm14x26",'W',0,NULL},
   {u8"[X] xm16x25",'X',0,NULL},
   {u8"[Y] xm16x25b",'Y',0,NULL},
   {u8"[Z] xm16x25i",'Z',0,NULL}
   };

TMENU mCzcionkaEkran={36,18,18,13,20,7,0,CMNU,CMBR,CMTX,0,26,0,0,0,(POLE(*)[]) &pmCzcionkaEkran,NULL,NULL};


TMENU mOpcje = {12, 0, 0, 42, 1, 3, ICONS | TADD, CMNU, CMBR, CMTX, 0, 0, 0, 0,0,(POLE(*)[]) &pmOpcje, NULL, NULL};

typedef enum DWG_ERROR
{
	DWG_NOERR = 0,
	/* sorted by severity */
	DWG_ERR_WRONGCRC = 1,
	DWG_ERR_NOTYETSUPPORTED = 1 << 1, /* 2 */
	DWG_ERR_UNHANDLEDCLASS = 1 << 2, /* 4 */
	DWG_ERR_INVALIDTYPE = 1 << 3, /* 8 */
	DWG_ERR_INVALIDHANDLE = 1 << 4, /* 16 */
	DWG_ERR_INVALIDEED = 1 << 5, /* 32 */
	DWG_ERR_VALUEOUTOFBOUNDS = 1 << 6, /* 64 */
	/* -------- critical errors ------- */
	DWG_ERR_CLASSESNOTFOUND = 1 << 7, /* 128 */
	DWG_ERR_SECTIONNOTFOUND = 1 << 8, /* 256 */
	DWG_ERR_PAGENOTFOUND = 1 << 9, /* 512 */
	DWG_ERR_INTERNALERROR = 1 << 10,/* 1024 */
	DWG_ERR_INVALIDDWG = 1 << 11,/* 2048 */
	DWG_ERR_IOERROR = 1 << 12,/* 4096 */
	DWG_ERR_OUTOFMEM = 1 << 13,/* 8192 */

} Dwg_Error;

char* DwgErrorList[] = { "NOERR",/* sorted by severity */"WRONG CRC","NOT YET SUPPORTED","UNHANDLED CLASS","INVALID TYPE","INVALID HANDLE","INVALIDEED","VALUE OUT OF BOUNDS",
/* -------- critical errors ------- */"CLASSES NOT FOUND","SECTION NOT FOUND","PAGE NOT FOUND","INTERNAL ERROR","INVALID DWG","IO ERROR","OUT OF MEM" };

BOOL get_close_button_pressed(void)
{  int ret=close_button_pressed;
    close_button_pressed=FALSE;
    return ret;
}

void set_background_menu(char *background)
{
	strncpy((char *) &background_name, background, 31);
	menu_par_new((*mOpcje.pola)[5].txt, background_name);
}

void load_file_to_history(char *sk)
/*-------------------------------*/
{ int i;
	if (strlen(sk) > 0)
	{
		i = 0;  //Check for MacOS
		while (i<MAXHISTORYFILES)
		{
			if ((strlen(Previous_File[i]) > 0) && (strcmp(sk, Previous_File[i]) == 0))
			{
#ifdef MACOS
				if (i!=0)
#endif
				 strcpy(Previous_File[i], Previous_File[0]);  //(i!=0) condition has to be on Mac, if not it's crashing, Windows and Linux tolerate it
				strcpy(Previous_File[0], sk);
				mLastFiles.poz = 0;
				return;
			}
			i++;
		}
		/*dopisanie nazwy do zbioru Previous_File[][]*/
		for (i = 0; i < (MAXHISTORYFILES-1); i++)
		{
			strcpy(Previous_File[(MAXHISTORYFILES-1) - i], Previous_File[(MAXHISTORYFILES-2) - i]);
		}
		strcpy(Previous_File[0], sk);
		mLastFiles.poz = 0;
	}
}    

BOOL zapisz_do(void);

BOOL uaktualnij(void)
/*----------------------------*/
{
  BOOL ret = FALSE;
  if(!strlen(zbior_danych))
  {
    ret = zapisz_do();
  }
  else
  {
    cursor_off();
    if (0 == pisz_zbior(zbior_danych, FALSE, 0))
    {
      ret = TRUE;
	  load_file_to_history(zbior_danych);
    }
    cursor_on();
  }
  if (ret == TRUE)
  {
    Change = FALSE;
  }
  return ret;
}

BOOL zapisz_do(void)
/*-------------------------*/
{
  char st1[MaxTextLen];
  char sk[MAXPATH]="";
  BOOL ret = FALSE;

  int key;


  if (Load_File (sk, SAVE_FILE_AS, FALSE) == FALSE)
  {
     return FALSE;
  }
  /*sprawdzenie czy zbior juz istnieje*/

#ifndef LINUX
  /*
  if (my_file_exists(sk))
   {
	strcpy(st1, _FILE_);

    strcat(st1, sk);
	////decodingwin(st1);

	strcat(st1, _FILE_EXISTS_);
    komunikat_str(st1);

	key = Get_Legal_Key(_YES_NO_ESC_);

    komunikat (0);
    //if (key == ESC) return FALSE;
    //if (key == 'N' || key == 'n')

	if (key != _YES_ && key != _yes_)
    {
      return FALSE;
    }
   }
   */
#endif
  if (pisz_zbior(sk, FALSE, 0) == 0)  /*brak bledu*/
  {
     ret = TRUE;
     if (numer_zbioru_hierarchii > 1)
      {
        if (numer_zbioru_hierarchii<255)  numer_zbioru_hierarchii += 1;
        strcpy (nazwa_zbioru_hierarchii [numer_zbioru_hierarchii], sk);
      } 
       else
        {
          numer_zbioru_hierarchii = 1;
          strcpy (nazwa_zbioru_hierarchii [numer_zbioru_hierarchii], sk);
        }
     load_file_to_history(sk);   
     strcpy(zbior_danych,sk); /*zmiana aktualnej nazwy pliku*/

	 out_file_name () ;
  }
  if (ret == TRUE)
  {
    Change = FALSE;
  }
  return ret;
}

void zamien(double *x1, double *x2)
{ double pom;
  pom=*x1;
  *x1=*x2;
  *x2=pom;
}

void  cur_offo(double x,double y)
{
    flip_screen();
}


void  cur_offo__(double x,double y)
{ x = x; y = y;
  outokno(&O,0,0);
}


void  cur_ono(double x,double y)
{ O.x2=x; O.y2=y;
  outokno(&O,0,0);
}

static void redcrok(void)
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  komunikat0(Komunikat_R0=31);
  CUR_OFF(X,Y);
  CUR_oFF=CUR_OFF;  CUR_OFF=cur_offo;
  CUR_oN=CUR_ON;    CUR_ON=cur_ono;
  CUR_ON(X,Y);
}

static BOOL b_active_w = FALSE ;

static void redcr(char typ)
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[3])(void),komunikat_R, komunikat_R0, ss;
  if (typ==0)
   {

     komunikat0(0);
     komunikat0(30);

     Cur_offd (X,Y);
     CUR_oFF=CUR_OFF;   CUR_OFF=cursel_off;
     CUR_oN=CUR_ON;     CUR_ON=cursel_on;
     SW[0]=SERV[73];    SERV[73]=sel_t;
     SW[1]=SERV[81];    SERV[81]=sel_d;
     Cur_ond (X,Y);
     b_active_w = TRUE ;
   }
  else
   {
     Cur_offd(X,Y);
     CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     SERV[73]=SW[0];
     SERV[81]=SW[1];
     Cur_ond (X,Y) ;
     komunikat0(0);
     b_active_w = FALSE ;
   }
}

int oknok_d(void)
{
  double X0, Y0 ;
  EVENT *ev;

  O.x1=x01;
  O.y1=y01;
  redcrok();
  while(1)
   { ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0) return 0;
     if (ev->What == evKeyDown &&  ev->Number==ENTER)
      {
	Cur_offd(X,Y);
	x02=X0; y02=Y0;
	if(x01>x02) zamien(&x01,&x02);
	if(y01>y02) zamien(&y01,&y02);
	Cur_ond (X,Y) ;
	if((dx=x02-x01)>0 && (dy=y02-y01)>0)
	 {
	  return 1;
	 }
	return 0;
      }
   }
}

int Okno_D (void)
/*-------------------------------------------------*/
{ int n;
  EVENT *ev;
  double X0,Y0;

  redcr(0);
  while(1)
   { ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0)
      { redcr(1);
	return 0;
      }
     if (ev->What == evKeyDown &&  ev->Number==ENTER)
      {
	x01=X0;
	y01=Y0;
	n=oknok_d();
	redcr(1);
	return n;
      }
   }
}


void wymazanie_ekranu (void)
//---------------------------------
{ int max_X, max_Y, i;
  max_X = getmaxx();
  max_Y = getmaxy();
  setcolor(0);
  setwritemode(XOR_PUT);
  setlinestyle1(SOLID_LINE,0,NORM_WIDTH);

  for (i=0; i<max_X; i++)
   {
    LINE(0,i,max_X,i); 
    delay(10);   
   }
}


double get_optimal_angle0(char  *adr,char  *adrk)
/*----------------------------------------------------*/
{ LINIA *L;
  BLOK  *b;
  char  *adp,  *adk;
  LINIA L_max;
  int nr_lines;
  double L_l, L_max_l;
  double opt_angle;
  BOOL end_check;

  adp = adr ;
  adk = adrk ;
  nr_lines=0;
  L_max_l=0;

  opt_angle=0;
  end_check=FALSE;

  L = (LINIA*)adp;
  while ((L->obiekt != Okoniec) && (adp < adk) && (end_check==FALSE))
  {
    if (L->obiekt == OdBLOK)
    {
      b=(BLOK *)adp;
      adp+=sizeof(NAGLOWEK)+B3+b->dlugosc_opisu_obiektu;
    }
    else
    {
      if ((L->atrybut != Abad) && (L->atrybut != Ausuniety) &&
          (L->obiekt == Olinia))

      { //to jest linia
        L_l=sqrt((L->x2-L->x1)*(L->x2-L->x1)+(L->y2-L->y1)*(L->y2-L->y1));
		if (L_l > L_max_l)
		{
			L_max_l = L_l;
			memmove(&L_max, L, sizeof(LINIA));
			nr_lines++;
		}

      }
     adp += L->n + sizeof(NAGLOWEK) ;
    }
   L = (LINIA*)adp;
  }if ( ptrs_ini_date->print_sheet )
  if ((nr_lines>0) && (L_max_l>0))
   {
//    L_max.x1=L_max.x2; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  - chwilowo
    if (Check_if_Equal(L_max.x1,L_max.x2)==TRUE) opt_angle=0.0; //Pi2/4;
      else if (Check_if_Equal(L_max.y1,L_max.y2)==TRUE) opt_angle=0.0; //Pi2/4;
        else if (Check_if_Equal(L_max.y1,L_max.y2)==FALSE)
         {
           opt_angle=atan((L_max.y2-L_max.y1)/(L_max.x2-L_max.x1));
         }
   }
  return (opt_angle);
}

GrContext* get_second_screen_bak_(void)
{
	return second_screen_bak_;
}

void backup_second_screen(void)
{   BITMAP *s_screen;
    s_screen=(BITMAP*)get_second_screen();
	if (s_screen == NULL) return;
    if (second_screen_bak_!=NULL) destroy_bitmap(second_screen_bak_);
    second_screen_bak_= create_bitmap_ex(32 /*bitmap_color_depth(s_screen)*/, s_screen->w, s_screen->h);
    second_screen_bak_->clip=s_screen->clip;
	second_screen_bak_->cl = s_screen->cl;
	second_screen_bak_->cr = s_screen->cr;
	second_screen_bak_->ct = s_screen->ct;
	second_screen_bak_->cb = s_screen->cb;
    blit(s_screen, second_screen_bak_, 0,0,0,0,s_screen->w,s_screen->h);
}

void restore_second_screen(void)
{   BITMAP *s_screen;
    s_screen=(BITMAP*)get_second_screen();
	if (s_screen == NULL) return;
    if (second_screen_bak_!=NULL)
    {
        set_clip_state(s_screen, 0);

        s_screen->clip=second_screen_bak_->clip;
        s_screen->cl=second_screen_bak_->cl;
        s_screen->cr=second_screen_bak_->cr;
        s_screen->ct=second_screen_bak_->ct;
        s_screen->cb=second_screen_bak_->cb;

		set_clip_state(second_screen_bak_, 0);
        blit(second_screen_bak_, s_screen, 0,0,0,0,second_screen_bak_->w,second_screen_bak_->h);
        destroy_bitmap(second_screen_bak_);
        second_screen_bak_=NULL;
    }
    CUR_OFF(X,Y);
    CUR_ON(X,Y);
}

BOOL DrawToPrn0 (int serial_mode, int init_prn_dlg)
//-------------------------------------------------
{
  int window_ok ;
  int k;
  int kk;
#ifdef ALLEGRO
  PALLETE paleta;
#endif
  BOOL ret_pre, ret_code;
  double optimal_angle;
  char *sheet_adr, *adk_S, *adp_F, *adk_F, *adp_A, *adk_A;
  BLOK *b_sheet, *b_format, *b_arkusz;
  int Change0;
  double xmin,ymin,xmax,ymax;
  double koc, kos;
  static T_Prn_Ini_Date  ptrs_ini_date_archive ;
  BOOL bitmap_exist0;
  BOOL normalize_text_back;
  PRIV_PRN_WINDOW *priv_prn_window = get_priv_prn_window();

  char format[20]="*FORMAT";
  char format1[20] = "*ФОРМАТ";
  char page[20]="*PAGE";
  char page1[20] = "*ARKUSZ";
  char page2[20] = "*СТОРІНКА";
  char page3[20] = "*АРКУШ";

  char kom[64];
  int ret_prn_param;

  NULL_FOUND=FALSE;
  ENABLE_FOUND=FALSE;
  normalize_text_back=normalize_text;
  normalize_text=FALSE;
  float prn_width_paper=210.0;
  float prn_height_paper=297.0;

  kk = Print2Page(1);
#ifndef LINUX
  prn_width_paper = (float)GetPrintPageWidth();
  prn_height_paper = (float)GetPrintPageHeight();
#else
  //reading from CUPS
    prn_width_paper = cups_prn_width_paper;
    prn_height_paper = cups_prn_height_paper;
#endif


return_to_dialog:

  if (init_prn_dlg==1)
   {
    strcpy (sz_file, zbior_danych) ;
    File_New_Ext (sz_file, ".prn") ;

    backup_second_screen();

    if ((ret_prn_param = Prn_Draw_Param (sz_file, &ptrs_config, &ptrs_ini_date, prn_width_paper, prn_height_paper, serial_mode)) == 0)
    {
      restore_second_screen();
      normalize_text=normalize_text_back;
        ////simulate_keypress(14592);  //to help hiding cursor after restoring second screen (overwalking)
        ////simulate_keypress(ESC);  //return to drawing from transparent dialog;
      return FALSE;
    }

#ifdef LINUX
#ifndef MACOS
       if ((ptrs_ini_date->prn_type==PRN_PCX) || (ptrs_ini_date->prn_type==PRN_WINDOWS))
       {
          if (test_python_pillow()==FALSE) return FALSE;
       }
#endif
#endif


   if (kk)
   {
#ifndef LINUX
	   if (ptrs_ini_date->prn_type == PRN_WINDOWS)
	   {
		   prn_width_paper = GetPrintPageWidth();
		   prn_height_paper = GetPrintPageHeight();
		   ptrs_ini_date->width_paper = prn_width_paper;
		   ptrs_ini_date->height_paper = prn_height_paper;
	   }
	   else
	   {
		   prn_width_paper = ptrs_ini_date->width_paper;
		   prn_height_paper = ptrs_ini_date->height_paper;
	   }
#else
       prn_width_paper = ptrs_ini_date->width_paper;
       prn_height_paper = ptrs_ini_date->height_paper;
#endif
   }

    restore_second_screen();
	original_color_print=ptrs_ini_date->color_print;

#ifndef LINUX
	if ((ptrs_ini_date->prn_type==PRN_WINDOWS) && (ret_prn_param != 2))
	{
		preview_blocked = TRUE;
		sw_callback_locked_proc();
		kk=Print2Page(ptrs_ini_date->WINPRINT_DEF);
		preview_blocked = FALSE;
		if (kk==0)
		{
			normalize_text=normalize_text_back;
			return FALSE;
		}
	}
#endif
   }

  if ((ptrs_ini_date->prn_type==PRN_PCX)
      #ifdef LINUX
      || (ptrs_ini_date->prn_type==PRN_WINDOWS)
      #endif
    )
   {
     //zapamietanie konfiguracji
     memcpy(&ptrs_ini_date_archive,ptrs_ini_date,sizeof(T_Prn_Ini_Date));
     //zmiana konfiguracji
     ptrs_ini_date->copies=1;
     ptrs_ini_date->out_to_file=1;
   }
  else if (ptrs_ini_date->prn_type == PRN_PDF)
  {
	  //zapamietanie konfiguracji
	  memcpy(&ptrs_ini_date_archive, ptrs_ini_date, sizeof(T_Prn_Ini_Date));
	  //zmiana konfiguracji
	  ptrs_ini_date->copies = 1;
	  ptrs_ini_date->out_to_file = 1;
  }
  else if (ptrs_ini_date->prn_type==PRN_WINDOWS)
   {
     //zapamietanie konfiguracji
     memcpy(&ptrs_ini_date_archive,ptrs_ini_date,sizeof(T_Prn_Ini_Date));
     //zmiana konfiguracji
     ptrs_ini_date->copies=1;
     ptrs_ini_date->out_to_file=0;
     ptrs_ini_date->buforowanie=0;
	 ptrs_ini_date->width_paper=GetPrintPageWidth();
	 ptrs_ini_date->height_paper=GetPrintPageHeight();
	 ptrs_ini_date->print_pause=0;
	 ptrs_ini_date->color_mode=0;
   }

  FormatX_S = FormatX;
  FormatY_S = FormatY;

  FormatX_S0 = 0;
  FormatY_S0 = 0;

  if (serial_mode==0)
  {
  if (( ptrs_ini_date->print_window ) || (ptrs_ini_date->print_sheet))
   {
    optimal_angle=0.0;
    Komunikat_R = 0;

    if ( ptrs_ini_date->print_sheet )
     {
       b_active_w = TRUE;
       sel.akt=1;
       sheet_adr = NULL;
       sheet_adr = Get_Sheet();
       sel.akt=0;
       b_active_w = FALSE ;
       if (sheet_adr==NULL)
        {
         if ((ptrs_ini_date->prn_type==PRN_PCX) || (ptrs_ini_date->prn_type==PRN_WINDOWS) || (ptrs_ini_date->prn_type == PRN_PDF))
          {
           //przywrocenie konfiguracji
           memcpy(ptrs_ini_date,&ptrs_ini_date_archive,sizeof(T_Prn_Ini_Date));
          }
		 normalize_text=normalize_text_back;
         return FALSE;
        }

       ptrs_ini_date->sheet_adr=sheet_adr;
       //ustalenie kata ortogonalnego
       b_sheet=(BLOK *)sheet_adr;
       adk_S=sheet_adr+sizeof(NAGLOWEK)+b_sheet->n-1;

       adp_F=find_block_sub(sheet_adr, adk_S, B_NIEOKRESLONY, format,7);
	   if (adp_F == NULL) adp_F = find_block_sub(sheet_adr, adk_S, B_NIEOKRESLONY, format1, 13);
       if (adp_F==NULL)
        {

		 sprintf(kom, _INTERRUPT_, format);
		 komunikat_str(kom);
         if ((ptrs_ini_date->prn_type==PRN_PCX) || (ptrs_ini_date->prn_type==PRN_WINDOWS))
          {
           //przywrocenie konfiguracji
           memcpy(ptrs_ini_date,&ptrs_ini_date_archive,sizeof(T_Prn_Ini_Date));
          }
		 normalize_text=normalize_text_back;
         return FALSE;
        }
       b_format=(BLOK *)adp_F;
       adk_F=adp_F+sizeof(NAGLOWEK)+b_format->n-1;
       adp_A=find_block_sub(sheet_adr, adk_S, B_NIEOKRESLONY, page,5);
	   if (adp_A == NULL) adp_A = find_block_sub(sheet_adr, adk_S, B_NIEOKRESLONY, page1, 7);
	   if (adp_A == NULL) adp_A = find_block_sub(sheet_adr, adk_S, B_NIEOKRESLONY, page2, 17);
	   if (adp_A == NULL) adp_A = find_block_sub(sheet_adr, adk_S, B_NIEOKRESLONY, page3, 11);
       if (adp_A==NULL)
        {

		 sprintf(kom, _INTERRUPT_, page);
         komunikat_str(kom);
         if ((ptrs_ini_date->prn_type==PRN_PCX) || (ptrs_ini_date->prn_type==PRN_WINDOWS))
          {
           //przywrocenie konfiguracji
           memcpy(ptrs_ini_date,&ptrs_ini_date_archive,sizeof(T_Prn_Ini_Date));
          }
		 normalize_text=normalize_text_back;
         return FALSE;
        }
       b_arkusz=(BLOK *)adp_A;
       adk_A=adp_A+sizeof(NAGLOWEK)+b_arkusz->n-1;

       optimal_angle=get_optimal_angle0(adp_F,adk_F);
       if (Check_if_Equal(optimal_angle,0.0)==FALSE)
        {
         Change0=Change;
         k=pisz_zbior(RYSUJ$1, FALSE, 1) ;
         if ( k )
          {
			komunikat_str(_SPOOLED_INTERRUPT_);
            if ((ptrs_ini_date->prn_type==PRN_PCX) || (ptrs_ini_date->prn_type==PRN_WINDOWS))
             {
              //przywrocenie konfiguracji
              memcpy(ptrs_ini_date,&ptrs_ini_date_archive,sizeof(T_Prn_Ini_Date));
             }
			normalize_text=normalize_text_back;
            return 0;
          }

         k=wszystkie_warstwy1();
         kos = sin (-optimal_angle);
         koc = cos (-optimal_angle);
         transformacja_blok (ADP, ADK, 0, 0, kos, koc, Tobrot,0);
		 zmien_atrybut(ADP, ADK, Ablok, Anormalny);

        }

       //ustalenie okna formatu
       Get_Limits (adp_F - dane, (long)(adp_F - dane + sizeof(NAGLOWEK) + b_format->n -1), ANieOkreslony, &xmin, &xmax, &ymin, &ymax) ;
       ptrs_ini_date->xp=xmin;
       ptrs_ini_date->yp=ymin;
       ptrs_ini_date->xk=xmax;
       ptrs_ini_date->yk=ymax;

	   FormatX_S = xmax;
	   FormatY_S = ymax;

	   FormatX_S0 = xmin;
	   FormatY_S0 = ymin;

       //ustalenie okna arkusza
       Get_Limits (adp_A - dane, (long)(adp_A - dane + sizeof(NAGLOWEK) + b_arkusz->n -1), ANieOkreslony, &xmin, &xmax, &ymin, &ymax) ;
       ptrs_ini_date->xp_w=xmin;
       ptrs_ini_date->yp_w=ymin;
       ptrs_ini_date->xk_w=xmax;
       ptrs_ini_date->yk_w=ymax;


     }
     else if (ret_prn_param==2)
      {
       b_active_w = TRUE;
       sel.akt=1;
       window_ok = Okno_D();
       sel.akt=0;
       b_active_w = FALSE ;
       if (window_ok==0)
        {

         if ((ptrs_ini_date->prn_type==PRN_PCX) || (ptrs_ini_date->prn_type==PRN_WINDOWS) || (ptrs_ini_date->prn_type == PRN_PDF))
          {
           //przywrocenie konfiguracji
           memcpy(ptrs_ini_date,&ptrs_ini_date_archive,sizeof(T_Prn_Ini_Date));
          }
		 normalize_text=normalize_text_back;
         return FALSE;
        }
       ptrs_ini_date->xp=x01;
       ptrs_ini_date->yp=y01;
       ptrs_ini_date->xk=x02;
       ptrs_ini_date->yk=y02;

	   FormatX_S = max(x02,x01);
	   FormatY_S = max(y02,y01);

	   FormatX_S0 = min(x01, x02);
	   FormatY_S0 = min(y01, y02);

	   ptrs_ini_date->xp_w = x01;
	   ptrs_ini_date->yp_w = y01;
	   ptrs_ini_date->xk_w = x02;
	   ptrs_ini_date->yk_w = y02;

       priv_prn_window->xp=ptrs_ini_date->xp;
       priv_prn_window->yp=ptrs_ini_date->yp;
       priv_prn_window->xk=ptrs_ini_date->xk;
       priv_prn_window->yk=ptrs_ini_date->yk;

       Change=TRUE;

       goto return_to_dialog;
      }
     else
    {
        ptrs_ini_date->xp=priv_prn_window->xp;
        ptrs_ini_date->yp=priv_prn_window->yp;
        ptrs_ini_date->xk=priv_prn_window->xk;
        ptrs_ini_date->yk=priv_prn_window->yk;

        FormatX_S = max(ptrs_ini_date->xk,ptrs_ini_date->xp);
        FormatY_S = max(ptrs_ini_date->yk,ptrs_ini_date->yp);

        FormatX_S0 = min(ptrs_ini_date->xk,ptrs_ini_date->xp);
        FormatY_S0 = min(ptrs_ini_date->yk,ptrs_ini_date->yp);

        ptrs_ini_date->xp_w = priv_prn_window->xp;
        ptrs_ini_date->yp_w = priv_prn_window->yp;
        ptrs_ini_date->xk_w = priv_prn_window->xk;
        ptrs_ini_date->yk_w = priv_prn_window->yk;
    }
         demo_print(1);
		 if (bitmap_pattern_exist) DonePatternBitmapScaled();

            ret_code=Draw_to_Prn_Proc (sz_file, ptrs_config, ptrs_ini_date, init_prn_dlg, serial_mode);

		 if (bitmap_pattern_exist) DonePatternBitmapScaled();
         demo_print(0);
         if (Check_if_Equal(optimal_angle,0.0)==FALSE)
          {
			bitmap_exist0=bitmap_exist;
            zwolnienie_pamieci();
            czytaj_rysunek1(RYSUJ$1, TRUE);
            Change=Change0;
            Ini_Place_Marker ();
            Ini_Layers_Dlg () ;
            Set_Auto_Backup (FALSE);
			bitmap_exist=bitmap_exist0;
          }

    set_scrsave_time ();
   }
   else
   {

    demo_print(1);
	if (bitmap_pattern_exist) DonePatternBitmapScaled();

        ret_code=Draw_to_Prn_Proc (sz_file, ptrs_config, ptrs_ini_date, init_prn_dlg, serial_mode);

	if (bitmap_pattern_exist) DonePatternBitmapScaled();
    demo_print(0);
    Komunikat_R = 0;

     set_scrsave_time ();
   }
  }
   else //serial_mode==1
    {

       demo_print(1);
	   if (bitmap_pattern_exist) DonePatternBitmapScaled();

           ret_code=Draw_to_Prn_Proc (sz_file, ptrs_config, ptrs_ini_date, init_prn_dlg, serial_mode);

	   if (bitmap_pattern_exist) DonePatternBitmapScaled();
       demo_print(0);
    }

  if ((ptrs_ini_date->prn_type==PRN_PCX) || (ptrs_ini_date->prn_type==PRN_WINDOWS) || (ptrs_ini_date->prn_type == PRN_PDF))
   {
    //przywrocenie konfiguracji
    memcpy(ptrs_ini_date,&ptrs_ini_date_archive,sizeof(T_Prn_Ini_Date));
   }
  normalize_text=normalize_text_back;
  return ret_code;
}


void DrawToPrn(void)
/*---------------------*/
{ BOOL k;

  NULL_FOUND=FALSE;
  ENABLE_FOUND=FALSE;
  kasuj_nr_pola();
  k=DrawToPrn0(0,1);
  set_trans_blender(0, 0, 0, TRANSLUCENCY); //just for a case

  lock_mouse();

  enable_F11();
}

BOOL DrawToPrn_Serial(int first_number, int last_number, int if_init, int if_exit)
/*------------------------------------------------------------------------------*/
{ char zbior_danych0[MAXPATH];
  int k, i;
  BOOL ret_var;
  ret_var=1;

  strcpy(zbior_danych0,zbior_danych);
  k=pisz_zbior(RYSUJ$1, FALSE, 1) ;
  if ( k )
   {
    komunikat_str(u8"Błąd zapisu zbioru ! Pominięto wydruk rysunków warsztatowych");
    return FALSE;
   }
  for (i=first_number; i<=last_number; i++)
  {
    zwolnienie_pamieci();
    sprintf(zbior_danych,"Rysunki/#%ld", i);
    czytaj_rysunek(zbior_danych, TRUE);
    Ini_Place_Marker ();
    Ini_Layers_Dlg () ;
    if ((i==1) && (if_init==1))
    {
      if (DrawToPrn0(1,1)==FALSE)
       { ret_var=0;
         goto error_print;
       }
    }
      else
       {
         if (DrawToPrn0(1,0)==FALSE)
         { ret_var=0;
           goto error_print;
         }
       }
  }

if (if_exit==1)  DrawToPrn0(1,2);

error_print:
  demo_print(0);
  Komunikat_R = 0;
  zwolnienie_pamieci();
  czytaj_rysunek(RYSUJ$1, TRUE);
  strcpy(zbior_danych, zbior_danych0);
  Ini_Place_Marker ();
  Ini_Layers_Dlg () ;
  Change = FALSE;
  Set_Auto_Backup (FALSE);
  return ret_var;
}

BOOL DrawToPrn_SerialS(int first_number, int last_number, int if_init, int if_exit)
/*-------------------------------------------------------------------------------*/
{ char zbior_danych0[MAXPATH];
  int k, i;
  BOOL ret_var;

  ret_var=1;
  strcpy(zbior_danych0,zbior_danych);
  k=pisz_zbior(RYSUJ$1, FALSE, 1) ;
  if ( k )
   {
    komunikat_str(u8"Błąd zapisu zbioru ! Pominięto wydruk specyfikacji");
    return FALSE;
   }

  for (i=first_number; i<=last_number; i++)
  {
    zwolnienie_pamieci();
    sprintf(zbior_danych,"RYSUNKI/#S%ld", i);
    czytaj_rysunek(zbior_danych, TRUE);
    Ini_Place_Marker ();
    Ini_Layers_Dlg () ;
    if ((i==1) && (if_init==1))
    {
      if (DrawToPrn0(2,1)==FALSE)
       { ret_var=0;
         goto error_print;
       }
    }
      else
       {
         if (DrawToPrn0(2,0)==FALSE)
         { ret_var=0;
           goto error_print;
         }
       }
  }
if (if_exit==1)  DrawToPrn0(2,2);
error_print:
  demo_print(0);
  Komunikat_R = 0;
  zwolnienie_pamieci();
  czytaj_rysunek(RYSUJ$1, TRUE);
  strcpy(zbior_danych, zbior_danych0);
  Ini_Place_Marker ();
  Ini_Layers_Dlg () ;
  Change = FALSE;
  Set_Auto_Backup (FALSE);
  return ret_var;
}

BOOL DrawToPrn_SerialZ(int first_number, int last_number, int if_init, int if_exit)
/*-------------------------------------------------------------------------------*/
{ char zbior_danych0[MAXPATH];
  int k, i;
  BOOL ret_var;

  ret_var=1;
  strcpy(zbior_danych0,zbior_danych);
  k=pisz_zbior(RYSUJ$1, FALSE, 1) ;
  if ( k )
   {
    komunikat_str(u8"B³¹d zapisu zbioru ! Pominiêto wydruk specyfikacji");
    return FALSE;
   }

  for (i=first_number; i<=last_number; i++)
  {
    zwolnienie_pamieci();
    sprintf(zbior_danych,"RYSUNKI/#Z%ld", i);
    czytaj_rysunek(zbior_danych, TRUE);
    Ini_Place_Marker ();
    Ini_Layers_Dlg () ;
    if ((i==1) && (if_init==1))
    {
      if (DrawToPrn0(2,1)==FALSE)
       { ret_var=0;
         goto error_print;
       }
    }
      else
       {
         if (DrawToPrn0(2,0)==FALSE)
         { ret_var=0;
           goto error_print;
         }
       }
  }
if (if_exit==1)  DrawToPrn0(2,2);
error_print:
  demo_print(0);
  Komunikat_R = 0;
  zwolnienie_pamieci();
  czytaj_rysunek(RYSUJ$1, TRUE);
  strcpy(zbior_danych, zbior_danych0);
  Ini_Place_Marker ();
  Ini_Layers_Dlg () ;
  Change = FALSE;
  Set_Auto_Backup (FALSE);
  return ret_var;
}

BOOL DrawToPrn_SerialP(int first_number, int last_number, int if_init, int if_exit)
/*-------------------------------------------------------------------------------*/
{ char zbior_danych0[MAXPATH];
  int k, i;
  BOOL ret_var;

  ret_var=1;
  strcpy(zbior_danych0,zbior_danych);
  k=pisz_zbior(RYSUJ$1, FALSE, 1) ;
  if ( k )
   {
    komunikat_str(u8"B³¹d zapisu zbioru ! Pominiêto wydruk rysunków prefabrykatów");
    return FALSE;
   }

  for (i=first_number; i<=last_number; i++)
  {
    zwolnienie_pamieci();
    sprintf(zbior_danych,"RYSUNKI/#P%ld", i);
    czytaj_rysunek(zbior_danych, TRUE);
    Ini_Place_Marker ();
    Ini_Layers_Dlg () ;
    if ((i==1) && (if_init==1))
    {
      if (DrawToPrn0(2,1)==FALSE)
       { ret_var=0;
         goto error_print;
       }
    }
      else
       {
         if (DrawToPrn0(2,0)==FALSE)
         { ret_var=0;
           goto error_print;
         }
       }
  }
if (if_exit==1)  DrawToPrn0(2,2);
error_print:
  demo_print(0);
  Komunikat_R = 0;
  zwolnienie_pamieci();
  czytaj_rysunek(RYSUJ$1, TRUE);
  strcpy(zbior_danych, zbior_danych0);
  Ini_Place_Marker ();
  Ini_Layers_Dlg () ;
  Change = FALSE;
  Set_Auto_Backup (FALSE);
  return ret_var;
}

BOOL DrawToPrn_SerialPT(int numbers, int tab_pref[174], int if_init, int if_exit)
/*-------------------------------------------------------------------------------*/
{ char zbior_danych0[MAXPATH];
  int k, i;
  BOOL ret_var;

  ret_var=1;
  strcpy(zbior_danych0,zbior_danych);
  k=pisz_zbior(RYSUJ$1, FALSE, 1) ;
  if ( k )
   {
    komunikat_str(u8"B³¹d zapisu zbioru ! Pominiêto wydruk rysunków prefabrykatów");
    return FALSE;
   }

  for (i=0; i<=numbers; i++)
  {
    zwolnienie_pamieci();
    sprintf(zbior_danych,"RYSUNKI/#P%ld", tab_pref[i]);
    czytaj_rysunek(zbior_danych, TRUE);
    Ini_Place_Marker ();
    Ini_Layers_Dlg () ;
    if ((i==0) && (if_init==1))
    {
      if (DrawToPrn0(2,1)==FALSE)
       { ret_var=0;
         goto error_print;
       }
    }
      else
       {
         if (DrawToPrn0(2,0)==FALSE)
         { ret_var=0;
           goto error_print;
         }
       }
  }
if (if_exit==1)  DrawToPrn0(2,2);
error_print:
  demo_print(0);
  Komunikat_R = 0;
  zwolnienie_pamieci();
  czytaj_rysunek(RYSUJ$1, TRUE);
  strcpy(zbior_danych, zbior_danych0);
  Ini_Place_Marker ();
  Ini_Layers_Dlg () ;
  Change = FALSE;
  Set_Auto_Backup (FALSE);
  return ret_var;
}


void PltOut (void)
//--------------------------
{
  char sz_file_pl [MAXPATH] ;
  char com_no [10];
  BOOL b_to_file, b_to_com ;
  int port_num ;
  int window_ok ;
  int k;
  double optimal_angle;
  char *sheet_adr, *adk_S, *adp_F, *adk_F, *adp_A, *adk_A;
  BLOK *b_sheet, *b_format, *b_arkusz;
  int Change0;
  double xmin,ymin,xmax,ymax;
  double koc, kos;
  BOOL normalize_text_back;


  T_PTR_Plt_Config ptrs_config ;
  T_PTR_Plt_Ini_Date ptrs_ini_date ;

  NULL_FOUND=FALSE;
  ENABLE_FOUND=FALSE;

  normalize_text_back=normalize_text;
  normalize_text=FALSE;

  if (strlen(plt_file0)==0)
  {
   strcpy (sz_file_pl, zbior_danych) ;
   File_New_Ext (sz_file_pl, ".plt") ;
  }
  else strcpy (sz_file_pl, plt_file0);


  if (FALSE == Plt_Draw_Param (sz_file_pl, &ptrs_config, &ptrs_ini_date ))
  {

	normalize_text=normalize_text_back;
    return ;
  }
  b_to_file = TRUE ;
  b_to_com = FALSE;
  port_num = 1 ;
  strcpy(plt_file0,"");
  if (strnicmp (sz_file_pl, "LPT", 3) == 0)
  {
    if (sscanf (&sz_file_pl [3] , "%d", &port_num) == 1  && (port_num > 0))
    {
      b_to_file = FALSE ;
      strcpy(plt_file0,sz_file_pl);
    }
  }
  else  /*porty szeregowe*/
   {
   if (strnicmp (sz_file_pl, "COM", 3) == 0)
    {
      b_to_file = TRUE ;
      b_to_com = TRUE ;
      strcpy(com_no,sz_file_pl);
      strcpy(plt_file0,com_no);
      strcpy(sz_file_pl, "COM_PLT.TMP");
    }
   }
  if (FALSE == Ini_Output_Device (sz_file_pl, b_to_file, port_num, 0))
  {
    ErrList (8) ;

	normalize_text=normalize_text_back;
    return ;
  }

  if (( ptrs_ini_date->print_window ) || (ptrs_ini_date->print_sheet)) //08-12-2020
   {
    optimal_angle=0.0;
    Komunikat_R = 0;

     if ( ptrs_ini_date->print_sheet )
     {
       b_active_w = TRUE;
       sel.akt=1;
       sheet_adr = NULL;
       sheet_adr = Get_Sheet();
       sel.akt=0;
       b_active_w = FALSE ;
       if (sheet_adr==NULL)
        {
		 normalize_text=normalize_text_back;
         return; // FALSE;
        }

       ptrs_ini_date->sheet_adr=sheet_adr;
       //ustalenie kata ortogonalnego
       b_sheet=(BLOK *)sheet_adr;
       adk_S=sheet_adr+sizeof(NAGLOWEK)+b_sheet->n-1;

       adp_F=find_block_sub(sheet_adr, adk_S, B_NIEOKRESLONY, "*FORMAT",7);
       if (adp_F==NULL)
        {
		 komunikat_str(_FORMAT_INTERRUPT_);
		 normalize_text=normalize_text_back;
         return; // FALSE;
        }
       b_format=(BLOK *)adp_F;
       adk_F=adp_F+sizeof(NAGLOWEK)+b_format->n-1;

       adp_A=find_block_sub(sheet_adr, adk_S, B_NIEOKRESLONY, "*ARKUSZ",7);
       if (adp_A==NULL)
		   adp_A = find_block_sub(sheet_adr, adk_S, B_NIEOKRESLONY, "*PAGE", 7);
	   if (adp_A == NULL)
        {
         komunikat_str(_PAGE_INTERRUPT_);
		 normalize_text=normalize_text_back;
         return; // FALSE;
        }
       b_arkusz=(BLOK *)adp_A;
       adk_A=adp_A+sizeof(NAGLOWEK)+b_arkusz->n-1;

       optimal_angle=get_optimal_angle0(adp_F,adk_F);
       if (Check_if_Equal(optimal_angle,0.0)==FALSE)
        {
         Change0=Change;
         k=pisz_zbior(RYSUJ$1, FALSE, 1) ;
         if ( k )
          {
			komunikat_str(_SPOOLED_INTERRUPT_);
			normalize_text=normalize_text_back;
            return; // 0;
          }

         k=wszystkie_warstwy1();
         kos = sin (-optimal_angle);
         koc = cos (-optimal_angle);
         transformacja_blok (ADP, ADK, 0, 0, kos, koc, Tobrot,0);
        }

       //ustalenie okna formatu
       Get_Limits (adp_F - dane, (long)(adp_F - dane + sizeof(NAGLOWEK) + b_format->n -1), ANieOkreslony, &xmin, &xmax, &ymin, &ymax) ;
       ptrs_ini_date->xp=xmin;
       ptrs_ini_date->yp=ymin;
       ptrs_ini_date->xk=xmax;
       ptrs_ini_date->yk=ymax;
       x01=xmin;
       y01=ymin;
       x02=xmax;
       y02=ymax;
       //ustalenie okna arkusza
       Get_Limits (adp_A - dane, (long)(adp_A - dane + sizeof(NAGLOWEK) + b_arkusz->n -1), ANieOkreslony, &xmin, &xmax, &ymin, &ymax) ;
       ptrs_ini_date->xp_w=xmin;
       ptrs_ini_date->yp_w=ymin;
       ptrs_ini_date->xk_w=xmax;
       ptrs_ini_date->yk_w=ymax;
     }
     else
      {
        b_active_w = TRUE ;
        sel.akt=1;
        window_ok = Okno_D();
        sel.akt=0;
        b_active_w = FALSE ;

        if (window_ok==0)
        {
		 normalize_text=normalize_text_back;
         return ;
        }
        ptrs_ini_date->xp=x01;
        ptrs_ini_date->yp=y01;
        ptrs_ini_date->xk=x02;
        ptrs_ini_date->yk=y02;
      }

    if (ptrs_config->origin==1)
      {
       ptrs_config->dx0 = 0.5 * (x02 - x01); /*Xmax - Xmin*/
       ptrs_config->dy0 = 0.5 * (y02 - y01); /*Ymax - Ymin*/
       }
	   else
	    {
	     ptrs_config->dx0 = 0; //x01;
	     ptrs_config->dy0 = 0; //y01;
	    }
    /******/
   demo_plot(1);
   Set_Draw_Window (x01, y01, x02, y02);
   if (TRUE==Draw_to_Plt_Proc (ptrs_config, ptrs_ini_date))  komunikat(0);
     else b_to_com = FALSE;
   Close_Output_Device () ;
   demo_plot(0);
#ifndef LINUX
   if (b_to_com == TRUE)
    {
    demo_plot_to_com(1);
//    en = getenv ("COMSPEC") ;
//    strcpy(params,"/C COPY COM_PLT.TMP ") ;
//    strcpy(params2,">NUL");
//    k = spawnl (P_WAIT, en, en, &params, &com_no, &params2, NULL) ;
    k = spawnl (P_WAIT, "PLOTUJ.BAT", "PLOTUJ.BAT", &com_no, NULL) ;
    demo_plot_to_com(0);
    }
#endif
    if (Check_if_Equal(optimal_angle,0.0)==FALSE)
      {
        zwolnienie_pamieci();
        czytaj_rysunek(RYSUJ$1, TRUE);
        Change=Change0;
        Ini_Place_Marker ();
        Ini_Layers_Dlg () ;
        Set_Auto_Backup (FALSE);
      }
   set_scrsave_time ();
  }
   else
    {
     demo_plot(1);
     Set_Draw_Window (Xmin, Ymin, Xmax, Ymax);
     if (TRUE==Draw_to_Plt_Proc (ptrs_config, ptrs_ini_date))
       {
        komunikat(0);
       }
        else b_to_com = FALSE;
     Close_Output_Device () ;
     demo_plot(0);
#ifndef LINUX
     if (b_to_com == TRUE)
     {
     demo_plot_to_com(1);
/*     Mode=getgraphmode();
     restorecrtmode(); */
//     en = getenv ("COMSPEC") ;
//     strcpy(params,"/C COPY COM_PLT.TMP ") ;
//     strcpy(params2,">NUL");
//     k = spawnl (P_WAIT, en, en, &params, &com_no, &params2, NULL) ;

     k = spawnl (P_WAIT, "PLOTUJ.BAT", "PLOTUJ.BAT", &com_no, NULL) ;
/*     setgraphmode (Mode) ;  */
     demo_plot_to_com(0);
     }
#endif
     Komunikat_R = 0;

     set_scrsave_time ();
    }
   normalize_text=normalize_text_back;
   enable_F11();
   return;
}

//static
void dxf_out (void)
/*-----------------------*/
{
  int acad_version ;

  acad_version = 2;
  DxfOut (acad_version) ;
  set_scrsave_time ();
}

//BOOL
void Uaktualnij (void);
BOOL Uaktualnij_ (void);

void DwgIn(void)
{
	char sk[MAXPATH] = "";
	char skala_dxf1[MaxTextLen] = "100";
	char parametr[80] = "";
	int key;
	int retval_no = 1;
	char params[128];
	unsigned long runcode;
	char error_str_code[72];
	BOOL ret;
	DXF_Header dxf_header;

	if (Change)
	{
		komunikat(3);
		key = Get_Legal_Key(_YES_NO_ESC_);
		komunikat(0);
		if (key == ESC) return;

		if (key == _YES_ || key == _yes_)
		{
			Uaktualnij();
		}
	}
	strcpy(sk, "");
	if (Load_File(sk, INI_DWG, TRUE) == FALSE)
	{
		return;
	}

    //LibreDWG variant
    sprintf(params, "\"%s\" --as r2013 -y -v0 -o %s", sk, ALFAIMPORTFILE);
	runcode = RunSilent("dwg2dxf.exe", params);
	runcode &= 8191;
	if (runcode > 0)
	{
		CUR_OFF(X, Y);
		sprintf(error_str_code, "%s%u", err_message_code, runcode);
		for (int i = 1; i < 13; i++)
		{
			if (runcode & (int)pow(2, i))
			{
				if (strlen(error_str_code) < 48)
				{
					strcat(error_str_code, "-");
					strcat(error_str_code, DwgErrorList[i]);
				}
			}
		}
//WRONG CRC,NOT YET SUPPORTED,UNHANDLED CLASS,INVALID TYPE,INVALID HANDLE,INVALIDEED,VALUE OUT OF BOUNDS,CLASSES NOT FOUND,SECTION NOT FOUND,PAGE NOT FOUND,INTERNAL ERROR,INVALID DWG, IO ERROR, OUT OF MEM
		ret = ask_question(1, "", confirm, "", err_message, 12, error_str_code, 11, 1, 62);
		CUR_ON(X, Y);
		return;
	}


	if (!czytaj_dxf_header(ALFAIMPORTFILE, &dxf_header))
	{
		CUR_OFF(X, Y);
		ret = ask_question(1, "", confirm, "", err_message1, 12, "", 11, 1, 62);
		CUR_ON(X, Y);

		return;
	}

	if (!GetDXFParams(&dxf_header))
		return;

	DxfInExe(ALFAIMPORTFILE, &dxf_header);
	ret = Delete_File(ALFAIMPORTFILE);
}

void DxfIn(void)
{
	char sk[MAXPATH] = "";
	char skala_dxf1[MaxTextLen] = "100";
	char parametr[80] = "";
	int key;
	int retval_no = 1;
	BOOL ret;
	DXF_Header dxf_header;

	if (Change)
	{
		komunikat(3);
		key = Get_Legal_Key(_YES_NO_ESC_);
		komunikat(0);
		if (key == ESC) return;

		if (key == _YES_ || key == _yes_)
		{
			Uaktualnij();
		}
	}
	strcpy(sk, "");
	if (Load_File(sk, INI_DXF, TRUE) == FALSE)
	{
		return;
	}

	if (!czytaj_dxf_header(sk, &dxf_header))
	{
		CUR_OFF(X, Y);
		ret = ask_question(1, "", confirm, "", err_message1, 12, "", 11, 1, 62);
		CUR_ON(X, Y);

		return;
	}

	if (!GetDXFParams(&dxf_header))
		return;

	preview_blocked = TRUE;

	DxfInExe(sk, &dxf_header);

	preview_blocked = FALSE;


	standard_func();

}

void DxfInExe(char *sk, DXF_Header *head)
{
  int k;
  char skala_dxf1[MaxTextLen]="100";
  double SkalaF_dxf;
  double Jednostki_dxf;
  double offset_dxf_x;
  double offset_dxf_y;
  char parametr[80]="";
  char str1[MAXPATH], str2[MAXPATH];
  int key;
  int retval_no = 1 ;
  BOOL auto_size;

  Jednostki_dxf = head->Jednostki_dxf;
  SkalaF_dxf = head->SkalaF_dxf;

  offset_dxf_x = head->offset_x;
  offset_dxf_y = head->offset_y;

  auto_size = head->auto_size;

  strcpy(zbior_danych_2,"");
  zwolnienie_pamieci();

  bitmap_view=TRUE;

  strcpy (zbior_danych, "") ;
  out_file_name () ;
  Clear_View () ;
  if (0 != czytaj_rysunek(s__prot.sz_prot, TRUE))  //"PROTOTYP.AXF"
   {
    strcpy (zbior_danych, "") ;
   }

  FormatX = jednostkiplt(head->format_x);
  FormatY = jednostkiplt(head->format_y);
  X_max = jednostkiplt(FormatX);
  Y_max = jednostkiplt(FormatY);
  Xmax = X_max + Xmin;
  Ymax = Y_max + Ymin;
  if (X > Xmax) X = Xmax;
  if (Y > Ymax) Y = Ymax;

  Ini_Place_Marker ();
  Ini_Layers_Dlg () ;
  Change = FALSE;
  komunikat0(0); //255
  komunikat(0);

  strcpy(zbior_danych_2,"");
  strcpy(str1, _CONVERSION_TERMINATION_);
  strncat(str1,sk,strlen(sk));
  ErrListStr(str1);
  Error=0;
  backup_hatch_pattern();
  k = czytaj_dxf_file (sk, Jednostki_dxf, SkalaF_dxf, offset_dxf_x, offset_dxf_y, auto_size, dxf_encoding);
  restore_hatch_pattern();
  if (k<0)
    {
    ErrList(170);
    set_scrsave_time ();
    return;
    }
   else
    {
     if (k>1)
     {
      strcpy(str1, _ERROR_DXF_);
	  sprintf(str2, _LINE_NO_, k);

      strncat(str1,sk,strlen(sk));
      strcat(str1, str2);
      ErrListStr(str1);
      key = Get_Legal_Key ("\033\015");
      komunikat (0);
     }
	 else if (k == 1)
	 {
		 komunikat(0);
	 }
     else
      {
        komunikat0(0);
      }
    }
  strcpy(zbior_danych,"");
  Ini_Place_Marker ();
  Ini_Layers_Dlg () ;
  Change = TRUE; //FALSE;
  Set_Auto_Backup (TRUE); //(FALSE);
  skala_format_();
  redraw();
  set_scrsave_time ();
}

void Inicjacja(void)
/*---------------------------*/
{
  char sk[MAXPATH]="";
  int key;
  char dir__[MAXPATH]="";
  char drive__[MAXDRIVE]="";
  char file__[MAXFILE]="";
  char ext__[MAXEXT]="";
  int flags;
  int focus;
  int ret;

  if (Change)
  {
    ret=ask_question(3, "Esc",_Yes_,_No_, _DRAWING_NOT_SAVED_, 12, _SAVE_IT_, 11, 1, 61);
    //1 ok; 0 - rezygnuj; 2 - Powrot
    if (ret==1) key=_YES_;
    else if (ret==2) key=_NO_;
    else return;
    if (key == _YES_ || key == _yes_)
    {
      Uaktualnij();
    }
  }
  strcpy (sk, "");

  if (Load_File (sk, INI_FILE, TRUE) == FALSE)
  {
     return;
  }

  strcpy(zbior_danych_2,""); /*kasowanie rysunku dolaczonego*/
  bitmap_view=TRUE;

  /*pobranie sciezki*/
  flags=fnsplit (sk, drive__, dir__, file__, ext__);
  strcpy(Current_File_Directory,drive__);
  strcat(Current_File_Directory,dir__);

  if (bitmap_pattern_exist) DonePatternBitmapScaled();

  zwolnienie_pamieci();
  if (numer_zbioru_hierarchii > 1)
   {
    if (numer_zbioru_hierarchii<255) numer_zbioru_hierarchii += 1;
    strcpy (nazwa_zbioru_hierarchii [numer_zbioru_hierarchii], sk);
   }
   else
    {
     numer_zbioru_hierarchii = 1;
     strcpy (nazwa_zbioru_hierarchii [numer_zbioru_hierarchii], sk);
    }
  /*sprawdzenie nazwy*/
  load_file_to_history(sk);

  strcpy(zbior_danych,sk);
  out_file_name () ;
  Clear_View () ;

  czytaj_rysunek(sk, TRUE);

  Ini_Place_Marker ();
  Ini_Layers_Dlg () ;
  Change = FALSE;
  Set_Auto_Backup (FALSE);

  focus = set_focus_on_window();

}

void Nowy_rysunek(void)
/*------------------------------*/
{
  char sk[MAXPATH]="";
  int key;

  BOOL ret;

  if (Change)
  {
    komunikat (3);
    key = Get_Legal_Key (_YES_NO_ESC_);
    komunikat (0);
    if (key == ESC) return;
    if (key == _YES_ || key == _yes_)
    {
		ret = Uaktualnij_();
		if (!ret) return;
    }
  }
  strcpy (sk, "");

  strcpy(zbior_danych_2,""); /*kasowanie rysunku dolaczonego*/
  bitmap_view=TRUE;

  if (bitmap_pattern_exist) DonePatternBitmapScaled();

  zwolnienie_pamieci();

  /*sprawdzenie nazwy*/
  load_file_to_history(sk);

  strcpy(zbior_danych,sk);
  out_file_name () ;
  Clear_View () ;
  czytaj_rysunek(sk, TRUE);
  Ini_Place_Marker ();
  Ini_Layers_Dlg () ;
  Change = FALSE;
  Set_Auto_Backup (FALSE);


}

void Prototype(void)
/*-----------------------------*/
{
	char sk[MAXPATH] = "";
	char dir__[MAXPATH]="";
	char drive__[MAXDRIVE]="";
	char file__[MAXFILE]="";
	char ext__[MAXEXT]="";
	int flags;

	strcpy(sk, "");

	if (Load_File(sk, INI_PRO, TRUE) == FALSE)
	{
		return;
	}

	/*pobranie sciezki*/
	flags = fnsplit(sk, drive__, dir__, file__, ext__);

}

void New_from_template(void)
/*---------------------------*/
{
	char sk[MAXPATH] = "";
	int key;
	char dir__[MAXPATH]="";
	char drive__[MAXDRIVE]="";
	char file__[MAXFILE]="";
	char ext__[MAXEXT]="";
	int flags;

	if (Change)
	{
		komunikat(3);
		key = Get_Legal_Key(_YES_NO_ESC_);
		komunikat(0);
		if (key == ESC) return;
		if (key == _YES_ || key == _yes_)
		{
			Uaktualnij();
		}
	}
	strcpy(sk, "");

	if (Load_File(sk, INI_FILE, TRUE) == FALSE)
	{
		return;
	}
	strcpy(zbior_danych_2, ""); /*kasowanie rysunku dolaczonego*/
	bitmap_view = TRUE;


	/*pobranie sciezki*/
	flags = fnsplit(sk, drive__, dir__, file__, ext__);
	strcpy(Current_File_Directory, drive__);
	strcat(Current_File_Directory, dir__);

	if (bitmap_pattern_exist) DonePatternBitmapScaled();

	zwolnienie_pamieci();

	strcpy(zbior_danych, "");
	out_file_name();
	Clear_View();
	czytaj_rysunek(sk, TRUE);
	Ini_Place_Marker();
	Ini_Layers_Dlg();
	Change = FALSE;
	Set_Auto_Backup(FALSE);

}

int Restore_params(void)
{
	int l_kr;
	int i;
	Ctx_Par_copy ctx_par;
    char sk[20];

	Xp = Drawing_Params[DRAWING_NUMBER].Xp;
	Yp = Drawing_Params[DRAWING_NUMBER].Yp;
	Xmin = Drawing_Params[DRAWING_NUMBER].Xmin;
	Ymin = Drawing_Params[DRAWING_NUMBER].Ymin;
	Xmax = Drawing_Params[DRAWING_NUMBER].Xmax;
	Ymax = Drawing_Params[DRAWING_NUMBER].Ymax;
	skala = Drawing_Params[DRAWING_NUMBER].skala;
	X_max = Drawing_Params[DRAWING_NUMBER].X_max;
	Y_max = Drawing_Params[DRAWING_NUMBER].Y_max;


	X = Drawing_Params[DRAWING_NUMBER].X;
	Y = Drawing_Params[DRAWING_NUMBER].Y;
	krok_s = Drawing_Params[DRAWING_NUMBER].krok_s;
	krok_g = Drawing_Params[DRAWING_NUMBER].krok_g;
	grid_ = Drawing_Params[DRAWING_NUMBER].grid_;
	snap_ = Drawing_Params[DRAWING_NUMBER].snap_;
	Current_Layer = Drawing_Params[DRAWING_NUMBER].Current_Layer;
	No_Layers = Drawing_Params[DRAWING_NUMBER].No_Layers;
	memcpy(&Layers, &Drawing_Params[DRAWING_NUMBER].Layers, sizeof(LAYER)*MAX_NUMBER_OF_LAYERS);
	FormatX = Drawing_Params[DRAWING_NUMBER].FormatX;
	FormatY = Drawing_Params[DRAWING_NUMBER].FormatY;
	SkalaF = Drawing_Params[DRAWING_NUMBER].SkalaF;
	DokladnoscF = Drawing_Params[DRAWING_NUMBER].DokladnoscF;
	Jednostki = Drawing_Params[DRAWING_NUMBER].Jednostki;
	memcpy(&zmwym, &Drawing_Params[DRAWING_NUMBER].zmwym, sizeof(ZMIENNE));
    zmwym_fraction=Drawing_Params[DRAWING_NUMBER].zmwym_fraction;
    zmwym_reserve=Drawing_Params[DRAWING_NUMBER].zmwym_reserve;
	orto = Drawing_Params[DRAWING_NUMBER].orto;
	df__Point_Size = Drawing_Params[DRAWING_NUMBER].df__Point_Size;
	b__Point_View = Drawing_Params[DRAWING_NUMBER].b__Point_View;
	df__Trace_Width = Drawing_Params[DRAWING_NUMBER].df__Trace_Width;
	df__Sketch_Dist_Max = Drawing_Params[DRAWING_NUMBER].df__Sketch_Dist_Max;

	l_kr = put_localx(Drawing_Params[DRAWING_NUMBER].local_x);
	l_kr = put_localy(Drawing_Params[DRAWING_NUMBER].local_y);
	l_kr = put_angle_l(Drawing_Params[DRAWING_NUMBER].angle_l);

	TRANSLUCENCY = Drawing_Params[DRAWING_NUMBER].TRANSLUCENCY;

	memcpy(&sektory_arkusza_ext, &Drawing_Params[DRAWING_NUMBER].sektory_arkusza_ext, sizeof(SEKTORY_EXT));
	KursorS = Drawing_Params[DRAWING_NUMBER].KursorS;

	ciagniecie_linii = Drawing_Params[DRAWING_NUMBER].ciagniecie_linii;
	rozcinanie_linii = Drawing_Params[DRAWING_NUMBER].rozcinanie_linii;
	ukrywanie_typu = Drawing_Params[DRAWING_NUMBER].ukrywanie_typu;
	type_view = Drawing_Params[DRAWING_NUMBER].type_view;
	swobodny_blok = Drawing_Params[DRAWING_NUMBER].swobodny_blok;
	swobodny_tekst = Drawing_Params[DRAWING_NUMBER].swobodny_tekst;
	memcpy(&options1, &Drawing_Params[DRAWING_NUMBER].options1, sizeof(OPTIONS1));
	normalize_text = Drawing_Params[DRAWING_NUMBER].options1.normalize_text;
	memcpy(&sel, &Drawing_Params[DRAWING_NUMBER].sel, sizeof(SEL));
	dane = Drawing_Params[DRAWING_NUMBER].dane;
	dane_size = Drawing_Params[DRAWING_NUMBER].dane_size;
    reset_dane0();

	for (i = 0; i < Max_No_Spec; i++)
	{
		put_spec_name(i, (char *) & Drawing_Params[DRAWING_NUMBER].s__table_spec[i].spec_name);
		put_spec_string(i, (char *) & Drawing_Params[DRAWING_NUMBER].s__table_spec[i].spec_value);
	}
	kolory.paper = Drawing_Params[DRAWING_NUMBER].kolory_paper;
	memcpy(&TextG, &Drawing_Params[DRAWING_NUMBER].TextG, sizeof(TEXT));
	Change = Drawing_Params[DRAWING_NUMBER].Change;
	bitmap_exist = Drawing_Params[DRAWING_NUMBER].bitmap_exist;
	bitmap_png_exist = Drawing_Params[DRAWING_NUMBER].bitmap_png_exist;
    bitmap_vector_exist = Drawing_Params[DRAWING_NUMBER].bitmap_vector_exist;
	bitmap_pattern_exist = Drawing_Params[DRAWING_NUMBER].bitmap_pattern_exist;
	solid_translucent_exist = Drawing_Params[DRAWING_NUMBER].solid_translucent_exist;
	bitmap_on_front_exist = Drawing_Params[DRAWING_NUMBER].bitmap_on_front_exist;

	strcpy(zbior_danych, Drawing_Params[DRAWING_NUMBER].zbior_danych);

	last_active_layer = -1;
	last_active_layer_arch = -1;

	imageBMPsize_used = Drawing_Params[DRAWING_NUMBER].imageBMPsize_used;
	put_ctx_created(Drawing_Params[DRAWING_NUMBER].ctx_created);
	put_i_ctx_created(Drawing_Params[DRAWING_NUMBER].i_ctx_created);
	put_i_ctx(Drawing_Params[DRAWING_NUMBER].i_ctx);
	put_ctx_ok(Drawing_Params[DRAWING_NUMBER].ctx_ok);
	put_regen_ctx(Drawing_Params[DRAWING_NUMBER].regen_ctx);

    depth_magnitude=Drawing_Params[DRAWING_NUMBER].depth_magnitude;
    thermal_magnitude=Drawing_Params[DRAWING_NUMBER].thermal_magnitude;
    load_magnitude=Drawing_Params[DRAWING_NUMBER].load_magnitude;
    force_magnitude=Drawing_Params[DRAWING_NUMBER].force_magnitude;
    moment_magnitude=Drawing_Params[DRAWING_NUMBER].moment_magnitude;
    displacement_magnitude=Drawing_Params[DRAWING_NUMBER].displacement_magnitude;
    rotation_magnitude=Drawing_Params[DRAWING_NUMBER].rotation_magnitude;

    thermal_precision=Drawing_Params[DRAWING_NUMBER].thermal_precision;
    load_precision=Drawing_Params[DRAWING_NUMBER].load_precision;
    force_precision=Drawing_Params[DRAWING_NUMBER].force_precision;
    moment_precision=Drawing_Params[DRAWING_NUMBER].moment_precision;
    displacement_precision=Drawing_Params[DRAWING_NUMBER].displacement_precision;
    rotation_precision=Drawing_Params[DRAWING_NUMBER].rotation_precision;
    stress_precision=Drawing_Params[DRAWING_NUMBER].stress_precision;

    n_magnitude=Drawing_Params[DRAWING_NUMBER].n_magnitude;
    v_magnitude=Drawing_Params[DRAWING_NUMBER].v_magnitude;
    m_magnitude=Drawing_Params[DRAWING_NUMBER].m_magnitude;
    d_magnitude=Drawing_Params[DRAWING_NUMBER].d_magnitude;
    r_magnitude=Drawing_Params[DRAWING_NUMBER].r_magnitude;
    rm_magnitude=Drawing_Params[DRAWING_NUMBER].rm_magnitude;
    s_magnitude=Drawing_Params[DRAWING_NUMBER].s_magnitude;
    q_magnitude=Drawing_Params[DRAWING_NUMBER].q_magnitude;
    p_magnitude=Drawing_Params[DRAWING_NUMBER].p_magnitude;

    memmove(&static_colors, &Drawing_Params[DRAWING_NUMBER].static_colors, sizeof(STATIC_COLORS));
    memmove(&static_stress_colors, &Drawing_Params[DRAWING_NUMBER].static_stress_colors, sizeof(STATIC_STRESS_COLORS));

    //zeroing
    for (i=0; i<32; i++) put_ctx_bitmap(i, NULL);

    for (i = 0; i < Drawing_Params[DRAWING_NUMBER].i_ctx_created; i++)
	{
		memcpy(&ctx_par, &Drawing_Params[DRAWING_NUMBER].ctx_par[i],sizeof(Ctx_Par_copy));
		put_ctx_par(i, &ctx_par);
		put_ctx_bitmap(i, Drawing_Params[DRAWING_NUMBER].ctx_bitmap[i]);
	}

	Set_change_dlg(&Drawing_Params[DRAWING_NUMBER].s__change_param);

	null_ctx_pointers_tail(Drawing_Params[DRAWING_NUMBER].i_ctx_created);

    priv_prn_window.xp = Drawing_Params[DRAWING_NUMBER].priv_prn_window.xp;
    priv_prn_window.yp = Drawing_Params[DRAWING_NUMBER].priv_prn_window.yp ;
    priv_prn_window.xk = Drawing_Params[DRAWING_NUMBER].priv_prn_window.xk;
    priv_prn_window.yk = Drawing_Params[DRAWING_NUMBER].priv_prn_window.yk;

    set_st_jedn();
    view_scale();

    reset_n_list_begin_blk();
    reset_b_layers_bak();

	return 1;
}



int Deposit_params(void)
{
	int i;
	Ctx_Par_copy ctx_par;

	Drawing_Params[DRAWING_NUMBER].Xp = Xp;
	Drawing_Params[DRAWING_NUMBER].Yp = Yp;
	Drawing_Params[DRAWING_NUMBER].Xmin = Xmin;
	Drawing_Params[DRAWING_NUMBER].Ymin = Ymin;
	Drawing_Params[DRAWING_NUMBER].Xmax = Xmax;
	Drawing_Params[DRAWING_NUMBER].Ymax = Ymax;
	Drawing_Params[DRAWING_NUMBER].skala = skala;
	Drawing_Params[DRAWING_NUMBER].X_max = X_max;
	Drawing_Params[DRAWING_NUMBER].Y_max = Y_max;
	Drawing_Params[DRAWING_NUMBER].X = X;
	Drawing_Params[DRAWING_NUMBER].Y = Y;
	Drawing_Params[DRAWING_NUMBER].krok_s = krok_s;
	Drawing_Params[DRAWING_NUMBER].krok_g = krok_g;
	Drawing_Params[DRAWING_NUMBER].grid_ = grid_;
	Drawing_Params[DRAWING_NUMBER].snap_ = snap_;
	Drawing_Params[DRAWING_NUMBER].Current_Layer = Current_Layer;
	Drawing_Params[DRAWING_NUMBER].No_Layers = No_Layers;
	memcpy(&Drawing_Params[DRAWING_NUMBER].Layers, &Layers, sizeof(LAYER)*MAX_NUMBER_OF_LAYERS);
	Drawing_Params[DRAWING_NUMBER].FormatX = FormatX;
	Drawing_Params[DRAWING_NUMBER].FormatY = FormatY;
	Drawing_Params[DRAWING_NUMBER].SkalaF = SkalaF;
	Drawing_Params[DRAWING_NUMBER].DokladnoscF = DokladnoscF;
	Drawing_Params[DRAWING_NUMBER].Jednostki = Jednostki;
	memcpy(&Drawing_Params[DRAWING_NUMBER].zmwym, &zmwym, sizeof(ZMIENNE));
    Drawing_Params[DRAWING_NUMBER].zmwym_fraction = zmwym_fraction;
    Drawing_Params[DRAWING_NUMBER].zmwym_reserve = zmwym_reserve;
	Drawing_Params[DRAWING_NUMBER].orto = orto;
	Drawing_Params[DRAWING_NUMBER].df__Point_Size = df__Point_Size;
	Drawing_Params[DRAWING_NUMBER].b__Point_View = b__Point_View;
	Drawing_Params[DRAWING_NUMBER].df__Trace_Width = df__Trace_Width;
	Drawing_Params[DRAWING_NUMBER].df__Sketch_Dist_Max = df__Sketch_Dist_Max;

	Drawing_Params[DRAWING_NUMBER].local_x = get_localx();
	Drawing_Params[DRAWING_NUMBER].local_y = get_localy();
	Drawing_Params[DRAWING_NUMBER].angle_l = get_angle_l();

	Drawing_Params[DRAWING_NUMBER].TRANSLUCENCY = TRANSLUCENCY;

	memcpy(&Drawing_Params[DRAWING_NUMBER].sektory_arkusza_ext, &sektory_arkusza_ext, sizeof(SEKTORY_EXT));
	Drawing_Params[DRAWING_NUMBER].KursorS = KursorS;

	Drawing_Params[DRAWING_NUMBER].ciagniecie_linii = ciagniecie_linii;
	Drawing_Params[DRAWING_NUMBER].rozcinanie_linii = rozcinanie_linii;
	Drawing_Params[DRAWING_NUMBER].ukrywanie_typu = ukrywanie_typu;
	Drawing_Params[DRAWING_NUMBER].type_view = type_view;
	Drawing_Params[DRAWING_NUMBER].swobodny_blok = swobodny_blok;
	Drawing_Params[DRAWING_NUMBER].swobodny_tekst = swobodny_tekst;
	memcpy(&Drawing_Params[DRAWING_NUMBER].options1, &options1, sizeof(OPTIONS1));
	Drawing_Params[DRAWING_NUMBER].options1.normalize_text = normalize_text;

	memcpy(&Drawing_Params[DRAWING_NUMBER].sel, &sel, sizeof(SEL));

	Drawing_Params[DRAWING_NUMBER].dane = dane;
	Drawing_Params[DRAWING_NUMBER].dane_size = dane_size;

	for (i = 0; i < Max_No_Spec; i++)
	{
		get_spec_name(i, (char*) & Drawing_Params[DRAWING_NUMBER].s__table_spec[i].spec_name);
		get_spec_string(i,(char*) &Drawing_Params[DRAWING_NUMBER].s__table_spec[i].spec_value);
	}

	Drawing_Params[DRAWING_NUMBER].kolory_paper = kolory.paper;
	memcpy(&Drawing_Params[DRAWING_NUMBER].TextG, &TextG, sizeof(TEXT));
	Drawing_Params[DRAWING_NUMBER].Change = Change;
	Drawing_Params[DRAWING_NUMBER].bitmap_exist= bitmap_exist;
	Drawing_Params[DRAWING_NUMBER].bitmap_png_exist = bitmap_png_exist;
    Drawing_Params[DRAWING_NUMBER].bitmap_vector_exist = bitmap_vector_exist;
	Drawing_Params[DRAWING_NUMBER].bitmap_pattern_exist = bitmap_pattern_exist;
	Drawing_Params[DRAWING_NUMBER].bitmap_on_front_exist = bitmap_on_front_exist;

	//saving CTX if exist
	Drawing_Params[DRAWING_NUMBER].imageBMPsize_used = imageBMPsize_used;
	Drawing_Params[DRAWING_NUMBER].ctx_created = get_ctx_created();
	Drawing_Params[DRAWING_NUMBER].i_ctx_created = get_i_ctx_created();
	Drawing_Params[DRAWING_NUMBER].i_ctx = get_i_ctx();
	Drawing_Params[DRAWING_NUMBER].ctx_ok = get_ctx_ok();
	Drawing_Params[DRAWING_NUMBER].regen_ctx = get_regen_ctx();

	for (i = 0; i < Drawing_Params[DRAWING_NUMBER].i_ctx_created; i++)
	{
		get_ctx_par(i, &ctx_par);
		memcpy(&Drawing_Params[DRAWING_NUMBER].ctx_par[i], &ctx_par, sizeof(Ctx_Par_copy));
		Drawing_Params[DRAWING_NUMBER].ctx_bitmap[i] = get_ctx_bitmap(i);
	}

	strcpy(Drawing_Params[DRAWING_NUMBER].zbior_danych, zbior_danych);

	Get_change_dlg(&Drawing_Params[DRAWING_NUMBER].s__change_param);

    Drawing_Params[DRAWING_NUMBER].priv_prn_window.xp = priv_prn_window.xp;
    Drawing_Params[DRAWING_NUMBER].priv_prn_window.yp = priv_prn_window.yp;
    Drawing_Params[DRAWING_NUMBER].priv_prn_window.xk = priv_prn_window.xk;
    Drawing_Params[DRAWING_NUMBER].priv_prn_window.yk = priv_prn_window.yk;

    Drawing_Params[DRAWING_NUMBER].depth_magnitude=depth_magnitude;
    Drawing_Params[DRAWING_NUMBER].thermal_magnitude=thermal_magnitude;
    Drawing_Params[DRAWING_NUMBER].load_magnitude=load_magnitude;
    Drawing_Params[DRAWING_NUMBER].force_magnitude=force_magnitude;
    Drawing_Params[DRAWING_NUMBER].moment_magnitude=moment_magnitude;
    Drawing_Params[DRAWING_NUMBER].displacement_magnitude=displacement_magnitude;
    Drawing_Params[DRAWING_NUMBER].rotation_magnitude=rotation_magnitude;

    Drawing_Params[DRAWING_NUMBER].thermal_precision=thermal_precision;
    Drawing_Params[DRAWING_NUMBER].load_precision=load_precision;
    Drawing_Params[DRAWING_NUMBER].force_precision=force_precision;
    Drawing_Params[DRAWING_NUMBER].moment_precision=moment_precision;
    Drawing_Params[DRAWING_NUMBER].displacement_precision=displacement_precision;
    Drawing_Params[DRAWING_NUMBER].rotation_precision=rotation_precision;
    Drawing_Params[DRAWING_NUMBER].stress_precision=stress_precision;

    Drawing_Params[DRAWING_NUMBER].n_magnitude=n_magnitude;
    Drawing_Params[DRAWING_NUMBER].v_magnitude=v_magnitude;
    Drawing_Params[DRAWING_NUMBER].m_magnitude=m_magnitude;
    Drawing_Params[DRAWING_NUMBER].d_magnitude=d_magnitude;
    Drawing_Params[DRAWING_NUMBER].r_magnitude=r_magnitude;
    Drawing_Params[DRAWING_NUMBER].rm_magnitude=rm_magnitude;
    Drawing_Params[DRAWING_NUMBER].s_magnitude=s_magnitude;
    Drawing_Params[DRAWING_NUMBER].q_magnitude=q_magnitude;
    Drawing_Params[DRAWING_NUMBER].p_magnitude=p_magnitude;

    memmove(&Drawing_Params[DRAWING_NUMBER].static_colors, &static_colors, sizeof(STATIC_COLORS));
    memmove(&Drawing_Params[DRAWING_NUMBER].static_stress_colors, &static_stress_colors, sizeof(STATIC_STRESS_COLORS));

	return 1;
}

void New_window_factory(int NEW_DRAWING_NUMBER, char *sk) //open another drawing in another window factory
{
	char dir__[MAXPATH]="";
	char drive__[MAXDRIVE]="";
	char file__[MAXFILE]="";
	char ext__[MAXEXT]="";
	int flags;
    int ret;

	strcpy(zbior_danych_2, ""); /*kasowanie rysunku dolaczonego*/
	bitmap_view = TRUE;

	CopyPreview(0, 0, 320, 320, DRAWING_NUMBER);
	ret = Deposit_params();

	Reset_change_dlg();

	DRAWING_NUMBER = NEW_DRAWING_NUMBER;

	InitBuffers();
	null_all_ctx_pointers();

	/*pobranie sciezki*/
	flags = fnsplit(sk, drive__, dir__, file__, ext__);
	strcpy(Current_File_Directory, drive__);
	strcat(Current_File_Directory, dir__);

	zwolnienie_pamieci();

	if (numer_zbioru_hierarchii > 1)
	{
		if (numer_zbioru_hierarchii < 255) numer_zbioru_hierarchii += 1;
		strcpy(nazwa_zbioru_hierarchii[numer_zbioru_hierarchii], sk);
	}
	else
	{
		numer_zbioru_hierarchii = 1;
		strcpy(nazwa_zbioru_hierarchii[numer_zbioru_hierarchii], sk);
	}
	/*sprawdzenie nazwy*/
	load_file_to_history(sk);

	strcpy(zbior_danych, sk);
	out_file_name();
	Clear_View();
	czytaj_rysunek(sk, TRUE);

	Ini_Place_Marker();
	Ini_Layers_Dlg();
	Change = FALSE;
	Set_Auto_Backup(FALSE);
    komunikat0(0);
}

int get_empty_buffer(void)
{
    int i;
    int ret;
    int NEW_DRAWING_NUMBER;

    //finding empty buffer
    i = 0;
    while (i < MAX_NUMBER_OF_WINDOWS)
    {
        if (Get_dane0_DATA(i) == NULL)
        {
            NEW_DRAWING_NUMBER = i;
            break;
        }
        i++;
    }

    if (i == MAX_NUMBER_OF_WINDOWS) return -1;
    else return i;
}

void New_window(void) //open another drawing in another window
{
    char sk[MAXPATH] = "";
    int i;
    int ret;
    int NEW_DRAWING_NUMBER;

    //finding empty buffer
    i = 0;
    while (i < MAX_NUMBER_OF_WINDOWS)
    {
        if (Get_dane0_DATA(i) == NULL)
        {
            NEW_DRAWING_NUMBER = i;
            break;
        }
        i++;
    }

    if (i == MAX_NUMBER_OF_WINDOWS) //to many
    {
        ret = ask_question(1, "", "OK", "", _TO_MANY_WIN_, 12, "", 11, 1, 0);
        return;
    }

    strcpy(sk, "");

    if (Load_File(sk, INI_FILE, TRUE) == FALSE)
    {
        return;
    }

    New_window_factory(NEW_DRAWING_NUMBER, sk);
}

void New_window_from_template(void) //open another drawing in another window
{
	char sk[MAXPATH] = "";
	char dir__[MAXPATH]="";
	char drive__[MAXDRIVE]="";
	char file__[MAXFILE]="";
	char ext__[MAXEXT]="";
	int flags;
	int i;
	int ret;
	int NEW_DRAWING_NUMBER;

	//finding empty buffer
	i = 0;
	while (i < MAX_NUMBER_OF_WINDOWS)
	{
		if (Get_dane0_DATA(i) == NULL)
		{
			NEW_DRAWING_NUMBER = i;
			break;
		}
		i++;
	}

	if (i == MAX_NUMBER_OF_WINDOWS) //to many
	{
		ret = ask_question(1, "", "OK", "", _TO_MANY_WIN_, 12, "", 11, 1, 0);
		return;
	}

	strcpy(sk, "");

	if (Load_File(sk, INI_FILE, TRUE) == FALSE)
	{
		return;
	}

	strcpy(zbior_danych_2, ""); /*kasowanie rysunku dolaczonego*/
	bitmap_view = TRUE;

	CopyPreview(0, 0, 320, 320, DRAWING_NUMBER);
	ret = Deposit_params();

	Reset_change_dlg();

	DRAWING_NUMBER = NEW_DRAWING_NUMBER;

	InitBuffers();
	null_all_ctx_pointers();

	/*pobranie sciezki*/
	flags = fnsplit(sk, drive__, dir__, file__, ext__);
	strcpy(Current_File_Directory, drive__);
	strcat(Current_File_Directory, dir__);

	zwolnienie_pamieci();

	strcpy(zbior_danych, "");
	out_file_name();
	Clear_View();
	czytaj_rysunek(sk, TRUE);

	Ini_Place_Marker();
	Ini_Layers_Dlg();
	Change = FALSE;
	Set_Auto_Backup(FALSE);
    komunikat0(0);
}

void New_window_empty(void) //open another drawing in another window
{
	char sk[MAXPATH] = "";
	int i;
	int ret;
	int NEW_DRAWING_NUMBER;

	//finding empty buffer
	i = 0;
	while (i < MAX_NUMBER_OF_WINDOWS)
	{
		if (Get_dane0_DATA(i) == NULL)
		{
			NEW_DRAWING_NUMBER = i;
			break;
		}
		i++;
	}

	if (i == MAX_NUMBER_OF_WINDOWS) //to many
	{
		ret = ask_question(1, "", "OK", "", _TO_MANY_WIN_, 12, "", 11, 1, 0);
		return;
	}

	strcpy(sk, "");

	strcpy(zbior_danych_2, ""); /*kasowanie rysunku dolaczonego*/
	bitmap_view = TRUE;

	CopyPreview(0, 0, 320, 320, DRAWING_NUMBER);
	ret = Deposit_params();

	Reset_change_dlg();

	DRAWING_NUMBER = NEW_DRAWING_NUMBER;

	InitBuffers();
	null_all_ctx_pointers();

	zwolnienie_pamieci();

	strcpy(zbior_danych, sk);
	out_file_name();
	Clear_View();
	czytaj_rysunek(sk, TRUE);

	Ini_Place_Marker();
	Ini_Layers_Dlg();
	Change = FALSE;
	Set_Auto_Backup(FALSE);
}


void Load_P_File1234(char *previous_file)
/*--------------------------------------------*/
{

  char sk[MAXPATH]="";
  int key;
  char dir__[MAXPATH]="";
  char drive__[MAXDRIVE]="";
  char file__[MAXFILE]="";
  char ext__[MAXEXT]="";
  int flags;

  if (strlen(previous_file)==0) return;

  if (Change)

  {
    komunikat (3);
    key = Get_Legal_Key (_YES_NO_ESC_);
    komunikat (0);

    if (key == ESC) return;
    if (key == _YES_ || key == _yes_)
    {
      Uaktualnij();
    }
  }

  strcpy (sk, previous_file);

  strcpy(zbior_danych_2,""); /*kasowanie rysunku dolaczonego*/

  /*pobranie sciezki*/

  flags=fnsplit (sk, drive__, dir__, file__, ext__);

  strcpy(Current_File_Directory,drive__);

  strcat(Current_File_Directory,dir__);

  if (bitmap_pattern_exist) DonePatternBitmapScaled();

  zwolnienie_pamieci();

  if (numer_zbioru_hierarchii > 1)

   {

    if (numer_zbioru_hierarchii<255) numer_zbioru_hierarchii += 1;

    strcpy (nazwa_zbioru_hierarchii [numer_zbioru_hierarchii], sk);

   }

   else

    {

     numer_zbioru_hierarchii = 1;

     strcpy (nazwa_zbioru_hierarchii [numer_zbioru_hierarchii], sk);

    }

    /*dopisanie nazwy do zbioru Previous_File[][]*/
    /*  for (i=0; i<3; i++)
   {
    strcpy(Previous_File[4-i],Previous_File[3-i]);
   }
    strcpy(Previous_File[0],sk);  */


  strcpy(zbior_danych,sk);

  out_file_name () ;

  Clear_View () ;

  czytaj_rysunek(sk, TRUE);


  Ini_Place_Marker ();

  Ini_Layers_Dlg () ;

  Change = FALSE;

  Set_Auto_Backup (FALSE);


}

void Load_P_File(void)
{ int n;
    n=mLastFiles.foff+mLastFiles.poz;
 Load_P_File1234(Previous_File[n]);
 return;
}


int alfacad_logo(void)
/*--------------*/
{  int ret;
 #ifdef ALLEGRO
  PALLETE paleta;
 #endif
    int w, h, x0, y0;

 w = getmaxx();
 h = getmaxy();

 x0 = getmaxx() / 2;
 y0 = getmaxy() / 2;

 return Al_Load_PNG_fade("alfalogosaktrans.png", "alfalogosak.png", w, h, x0, y0, 640, 480, 0);  //622, 465

}

void alfacad_logo_info(void)

/*--------------*/

{B_PCX b_pcx;
#ifdef ALLEGRO
    PALLETE paleta;
#endif
    int w, h, x0, y0;

    w = getmaxx();
    h = getmaxy();

    x0 = w / 2;
    y0 = h / 2;


    Al_Load_PNG_fade("alfalogosaktrans.png", "alfalogosak.png", w, h, x0, y0, 640, 480, 1);  //622, 465

}


void Save_Last_Window_Settings0(void)
{ int x_win_orig, y_win_orig, win_width, win_height;
  int ret;
  int f_set_handle;

  if (GFX_WIN==0) return;

  ret=get_window_origin_and_size(&x_win_orig, &y_win_orig, &win_width, &win_height);
  if (ret==1)
  {
    /*zapis pliku WINSET.DAT*/

#ifndef LINUX
      f_set_handle =open(WINSET_FILE, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
      f_set_handle =open(WINSET_FILE, O_CREAT|O_TRUNC|O_RDWR, 0666); //S_IRWXU | S_IRWXG | S_IRWXO);
#endif
    if (f_set_handle == -1)
    {
     ErrList(103);
    }
	else
	{
      if (write (f_set_handle, &x_win_orig, sizeof(int)) != sizeof(int)) goto error_set;
	  if (write (f_set_handle, &y_win_orig, sizeof(int)) != sizeof(int)) goto error_set;
	  if (write (f_set_handle, &win_width, sizeof(int)) != sizeof(int)) goto error_set;
	  if (write (f_set_handle, &win_height, sizeof(int)) != sizeof(int)) goto error_set;
error_set:
	  close (f_set_handle) ;
	}
  }
  return;
}

void Save_Last_Window_Settings(void)
{
	int x_win_orig, y_win_orig, win_width, win_height;
	int ret;
	FILE *f_set_handle;

	if (GFX_WIN == 0) return;

	ret = get_window_origin_and_size(&x_win_orig, &y_win_orig, &win_width, &win_height);
#ifdef LINUX
#ifndef MACOS //ARM64
    if (!wayland)
        win_height -= Get_X11_SCREEN_SHIFT();
#endif
#endif

		/*zapis pliku WINSET.DAT*/

		f_set_handle = fopen(WINSET_FILE, "wb");

		if (f_set_handle == NULL)
		{
			ErrList(103);

		}
		else
		{
			fwrite(&x_win_orig, sizeof(x_win_orig), 1, f_set_handle);
			fwrite(&y_win_orig, sizeof(y_win_orig), 1, f_set_handle);
		    fwrite(&win_width, sizeof(win_width), 1, f_set_handle);
			fwrite(&win_height, sizeof(win_height), 1, f_set_handle) ;

			fclose(f_set_handle);

			InfoList(9);
			delay(1000);

			ClearInfo();

		}

	return;
}

int Load_Last_Window_Settings(int *x_win_orig, int *y_win_orig, int *win_width, int *win_height)
{
  int f_set_handle;


  if (GFX_WIN==0) return 0;

  f_set_handle = open (WINSET_FILE, O_BINARY | O_RDONLY, S_IREAD) ;
  if (f_set_handle == -1)
    {

	  get_full_screen(x_win_orig, y_win_orig, win_width, win_height);

	  return 1;
    }
	else
	{
      if (read (f_set_handle, x_win_orig, sizeof(int)) != sizeof(int)) goto error_set1;
	  if (read (f_set_handle, y_win_orig, sizeof(int)) != sizeof(int)) goto error_set1;
	  if (read (f_set_handle, win_width, sizeof(int)) != sizeof(int)) goto error_set1;
	  if (read (f_set_handle, win_height, sizeof(int)) != sizeof(int)) goto error_set1;

      printf("LAST: x=%d, y=%d, w=%d, h=%d\n",*x_win_orig, *y_win_orig, *win_width, *win_height);

	  close (f_set_handle) ;
	  return 1;

error_set1:
	  close (f_set_handle) ;
      get_full_screen(x_win_orig, y_win_orig, win_width, win_height);

	  return 1;
	}
  return 0;
}

void Delete_All_Dump_bitmap(void)
{
	int i;
	for (i = 0; i < MAX_NUMBER_OF_WINDOWS; i++)
	{
		if (dump_bitmap[i] != NULL) destroy_bitmap(dump_bitmap[i]);
	}
}
void Delete_Patterns(void)
{
	free_mem_hatch_def_param();
}

void Close_window(void)
{
	int ret;
	int key;
	int i, j;
	char exit_str[72];
	int i_drawings;

 Semaphore=FALSE;
	//checking how many drawings are opened
	i_drawings = 0;
	for (i = 0; i < MAX_NUMBER_OF_WINDOWS; i++)
	{
		if (Get_dane0_DATA(i) != NULL) i_drawings++;
	}
	if (i_drawings == 1) return;

	if (!Change)
	{
		strcpy(exit_str, _CLOSE_WINDOW_);
		ret = ask_question(2, _No_, _Yes_, "", exit_str, 12, "", 11, 1, 63);
		//1 ok; 0 - rezygnuj; 2 - Powrot
        if (ret==1) key=_YES_;
        else if (ret==2) key=_NO_;
        else return;
        if (key == _NO_ || key == _no_) return;
	}
	else if (Change) //current drawing is not saved
	{
		ret = ask_question(3, "Esc", _Yes_, _No_, _DRAWING_NOT_SAVED_, 12, u8"Save it ? ", 11, 1, 61);
		//1 ok; 0 - rezygnuj; 2 - Powrot
        if (ret==1) key=_YES_;
        else if (ret==2) key=_NO_;
        else return;
		if (key == _YES_ || key == _yes_)
		{
			if (uaktualnij() == FALSE) return;
		}
	}

	i = DRAWING_NUMBER+1;
	if (i == MAX_NUMBER_OF_WINDOWS) i = 0;
	j = 0;
	while (1)
	{
		if ((i!= DRAWING_NUMBER) && (Get_dane0_DATA(i) != NULL))
		{
			break;
		}
		i++;
		j++;
		if (i == MAX_NUMBER_OF_WINDOWS) i = 0;
		if (j == 16)  //something went wrong, no drawing exists
		{
			ret = ask_question(1, "", "OK", "", _NO_MORE_DRAWINGS_, 12, _NEW_DRAWING_, 11, 1, 61);
			Change = FALSE;
			Nowy_rysunek();
			return;
		}
	}
	//closing of DRAWING_NUMBER and freeing memory

	zwolnienie_pamieci_forever();
	DoneBuffers(DRAWING_NUMBER);

	DRAWING_NUMBER = i;
	ret = Restore_params();
	out_file_name();
	Clear_View();  //TEMPORARY

	////////////////////
	setaspectratio(10000, min(32000., 10000. / sk_x));
	d_2d();
	if (Xp + X2d > Xmax) Xp = Xmax - X2d;	//na wypadek zmiany rozdzielczosci monitora
	if (Xp < Xmin) Xp = Xmin;
	if (Yp + Y2d > Ymax) Yp = Ymax - Y2d;
	if (Yp < Ymin) Yp = Ymin;
	Xk_Yk();
	pXp = pikseleX(Xp);
	pYp = pikseleY(Yp);

	reset_background();

	if (!snap_)krok_s = jednostkiY(1);
	uaktualnij_pola();

	setfillstyle_(EMPTY_FILL, 0);
	bar(xp, yp, maxX - xk, maxY - yk);

	Ini_Global_Object();

	redraw();

	Ini_Place_Marker();
	Ini_Layers_Dlg();

    flip_screen();

}

void Koniec(void)

/*--------------*/
{ int ret;
  int key;
  FILE *f;
  int i, j;
  char exit_str[72];
  BOOL changes[MAX_NUMBER_OF_WINDOWS] = {FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE };
  int i_changes = 0;
  char win_no[4];
  char byebye[80];

  char str_zapis[48]= _SAVING_CONF_;

  Semaphore = FALSE;

  exit_alf=FALSE;

#ifdef MACOS  //maybe not only
//closing alfaedit if exists
 if (edit_text_flag>0)  //editor exists
 {
  pid_t e_pid;
  int ret;
  e_pid=get_e_pid();
  if (e_pid==0) return;
  ret=send_AppleScript_Exit(e_pid);

  edit_text_flag=0;
 }
#endif

  //checking if all drawings are saved after edition
  for (i = 0; i < MAX_NUMBER_OF_WINDOWS; i++)
  {
	  if (Get_dane0_DATA(i) != NULL)
	  {
		  if (i == DRAWING_NUMBER) changes[i] = Change;
		  else changes[i] = Drawing_Params[i].Change;
		  if (changes[i] == TRUE) i_changes++;
	  }
  }

  if ((Change) && (i_changes<=1)) //only current drawing is not saved
   {
	   ret=ask_question(3, "Esc",_Yes_,_No_, _DRAWING_NOT_SAVED_, 12, _SAVE_IT_, 11, 1, 61);
	   //1 ok; 0 - rezygnuj; 2 - Powrot
	   if (ret==1) key=_YES_;
	   else if (ret==2) key=_NO_;
	     else return;

	 if (key == _YES_ || key == _yes_)
	 {
	   if (uaktualnij() == FALSE) return;
	 }

   }
  else if (i_changes > 0)  //other drawing is not saved
  {
	  ret = ask_question(3, "Esc", _Yes_, _No_, _SOME_OPEN_DRAWING_NOT_SAVED_, 12, _SAVE_THEM_, 11, 1, 61);
	  //1 ok; 0 - rezygnuj; 2 - Powrot
	  if (ret == 1) key = _YES_;
	  else if (ret == 2) key = _NO_;
	  else return;

	  if (key == _YES_ || key == _yes_)
	  {
		  if (Change)
		  {
			  if (uaktualnij() == FALSE) return;
			  changes[DRAWING_NUMBER] = FALSE;
			  Drawing_Params[DRAWING_NUMBER].Change = FALSE;
		  }
		  for (i = 0; i < MAX_NUMBER_OF_WINDOWS; i++)
		  {
			  if (changes[i] == TRUE)
			  {
				  DRAWING_NUMBER = i;
				  ret = Restore_params();
				  out_file_name();
				  Clear_View();  //TEMPORARY

				  ////////////////////
				  setaspectratio(10000, min(32000., 10000. / sk_x));
				  d_2d();
				  if (Xp + X2d > Xmax) Xp = Xmax - X2d;	//na wypadek zmiany rozdzielczosci monitora
				  if (Xp < Xmin) Xp = Xmin;
				  if (Yp + Y2d > Ymax) Yp = Ymax - Y2d;
				  if (Yp < Ymin) Yp = Ymin;
				  Xk_Yk();
				  pXp = pikseleX(Xp);
				  pYp = pikseleY(Yp);

				  reset_background();

				  if (!snap_)krok_s = jednostkiY(1);
				  uaktualnij_pola();

				  setfillstyle_(EMPTY_FILL, 0);
				  bar(xp, yp, maxX - xk, maxY - yk);

				  Ini_Global_Object();

				  redraw();

				  Ini_Place_Marker();
				  Ini_Layers_Dlg();

				  if (uaktualnij() == FALSE) return;
				  changes[i] = FALSE;
				  Drawing_Params[i].Change = FALSE;
			  }
		  }
	  }

  }

  Test_App(0, NULL);
  if (Number_of_clients == 1)
  {
	  strcpy(exit_str, _WANT_EXIT_);
  }
  else
  {
	  strcpy(exit_str, _WANT_CLOSE_WINDOW_);
  }

  if (yes_or_no(exit_str))
 {
	 komunikat_str(_SAVING_PREFILES_);
	 preview_blocked = TRUE;
	 //zapis pliku PREFILES.DAT
	 f = fopen("PREFILES.DAT", "wt");
	 if (f != NULL)
	 {
		 for (i = 0; i < MAXHISTORYFILES; i++)
		 {
			 //zapis nazw zbiorow
			 fprintf(f, "%s\n", Previous_File[i]);
		 }
		 fclose(f);
	 }

	 komunikat_str("@ D");

	 for (i = 0; i < MAX_NUMBER_OF_WINDOWS; i++)
	 {
		 if (Get_dane0_DATA(i) != NULL)
		 {
			 put_ctx_created(Drawing_Params[i].ctx_created);
			 put_i_ctx_created(Drawing_Params[i].i_ctx_created);
			 if (Drawing_Params[i].i_ctx_created > 0)
			 {
				 for (j = 0; j < Drawing_Params[i].i_ctx_created; i++)
				 {
					 put_ctx_bitmap(i, Drawing_Params[i].ctx_bitmap[j]);
				 }
				 Destroy_All_ctx();
			 }
		 }
	 }

     komunikat_str("@ F");

	 if (Number_of_clients == 1)
	 {
		 komunikat_str("Goodbye");
	 }

   remove_int(Check_ConfigureNotify);

   DoneBuffers1();
   komunikat_str("@ 2");  // !!!!!!!!!
   DoneBuffers2();
   komunikat_str("@ 4");
   DoneBuffers4();
   komunikat_str("@ 5");
   DoneBuffers5();
   komunikat_str("@ 6");
   DoneBuffers6();
   komunikat_str("@ 7");
   DoneBuffers7();
   komunikat_str("@ 8");
   DoneBuffers8();
   komunikat_str("@ 9");
   Done_CUPS_Printers();
   komunikat_str("@ 10");
   Delete_All_Dump_bitmap();
   strcpy(byebye, u8"See you soon! Na razie! Побачимося ! Hasta luego !");
   komunikat_str(byebye);

#ifdef ALLEGRO5
   my_sleep(100);  ////  15-09-2025
   set_semaphore(0);  ////  15-09-2025
#endif

   ret = alfaplugins_off();

   Delete_Patterns();  ////
   Free_winvar();

   for (i = 0; i < MAX_NUMBER_OF_WINDOWS; i++)
   {
		   DoneBuffers(i);
   }

   //strcat(byebye, "!");  //See you soon! Na razie! Hasta luego!!
   //komunikat_str(byebye);
   //strcat(byebye, "!");  //See you soon!Na razie!Hasta luego!!!
   //komunikat_str(byebye);

   DoneBackground();

   //strcat(byebye, "!");  //See you soon! Na razie! Hasta luego!!!!
   //komunikat_str(byebye);

   DonePatterns();

   //strcat(byebye, ".");  //See you soon! Na razie! Hasta luego!!!!.
   //komunikat_str(byebye);

   DonePatternBitmapScaled();

   //strcat(byebye, ".");  //See you soon! Na razie! Hasta luego!!!!..
   //komunikat_str(byebye);

   DoneKeyimage();
   DoneMickeyHand();

   //strcat(byebye, ".");  //See you soon! Na razie! Hasta luego!!!!...
   //komunikat_str(byebye);
   DoneCursorimage();

   //strcat(byebye, ".");  //See you soon! Na razie! Hasta luego!!!!....
   //komunikat_str(byebye);

   DoneBuffers3();

   exit_alf=TRUE;

   ret = Test_App_Shm_Off();

   delete_hatch_file () ;

   report_mem_leak_();
   report_mem_leak_cpp_();

   DoneArgs();

#ifndef MACOS
   quick_exit(0);
#else
      exit(0);
#endif

   return;
  }
 else
  {
     lock_mouse();
     return;
  }

}


void GoRestart(void)

/*--------------*/
{ int ret;

        DoneBuffers1();
        DoneBuffers2();
        DoneBuffers4();
        DoneBuffers5();
        DoneBuffers6();
        DoneBuffers7();
        DoneBuffers8();
        Done_CUPS_Printers();
        Delete_All_Dump_bitmap();
        ret = alfaplugins_off();
        Delete_Patterns();
        Free_winvar();

        DoneBuffers(0);

        DoneBuffMacro();
        DoneBackground();

        DonePatterns();

        DonePatternBitmapScaled();

        DoneKeyimage();
        DoneMickeyHand();

        DoneCursorimage();

        DoneBuffers3();

        exit_alf=TRUE;

        ret = Test_App_Shm_Off();

        delete_hatch_file () ;

        report_mem_leak_();
        report_mem_leak_cpp_();

        return;
}

void Uaktualnij (void)
{
  BOOL ret=uaktualnij();
}

BOOL Uaktualnij_ (void)
{
    return uaktualnij();
}

void Zapisz_do (void)

{

  zapisz_do ();

}

void nooop(void)
{}


void out_file_name (void)
/*---------------------*/
{
 char *str;
 static char sz_file_out [MAXPATH] ;
 char str_utf8[MAXPATH];
 char *adres1;

 if (strlen(zbior_danych)!=0)
 {
  strcpy(sz_file_out,zbior_danych);
  str = strrchr(sz_file_out, '/' /*'\\'*/);
  if (strlen(zbior_danych_2)>0)
   {
    strcat(sz_file_out," + ");
    adres1 = strrchr(zbior_danych_2, '/' /*'\\'*/);
    if (adres1 == NULL)
     {
      strcpy(sz_file_out,zbior_danych_2);
     }
      else
       {
        adres1++;
        strcat(sz_file_out,adres1);
       }
   }
  if (str == NULL)
  {
    str = sz_file_out;
  }
  else
  {
   str++;
  }

  strcpy(str_utf8, str);
  komunikat0_str (9, str_utf8) ;
  my_set_window_title(zbior_danych);
 }
 else
 {
	 strcpy(sz_file_out,"");
	 komunikat0_str (9, sz_file_out) ;
     my_set_window_title(ALF_TITLE);
 }
}


void Powrot(void)

{ char sk[MAXPATH]="";
  int key;
  if (numer_zbioru_hierarchii>1)
   {
    if (Change)
     {
      komunikat (3);
      key = Get_Legal_Key (_YES_NO_ESC_);
      komunikat (0);
      if (key == ESC) return;
      if (key == _YES_ || key == _yes_)
      {
       uaktualnij();
      }
     }
    numer_zbioru_hierarchii -= 1;
    strcpy (sk, nazwa_zbioru_hierarchii [numer_zbioru_hierarchii]);
    strcpy(zbior_danych_2,""); //kasowanie rysunku dolaczonego

	if (bitmap_pattern_exist) DonePatternBitmapScaled();

    zwolnienie_pamieci();
    bitmap_view=TRUE;

    strcpy(zbior_danych,sk);
    Clear_View () ;
    czytaj_rysunek(sk, TRUE);
    Ini_Place_Marker ();
    Ini_Layers_Dlg () ;
    Change = FALSE;
    Set_Auto_Backup (FALSE);
   }
    else  //pien struktury
     {
       komunikat(87);
       key = Get_Legal_Key ("TNtn\033");
       komunikat(0);
     }
}

 static void (*COMND[])(void)=
{
  Inicjacja, nooop, nooop, Uaktualnij, Zapisz_do, nooop, nooop, nooop, Edit_Spec,
   DrawToPrn, PltOut, Info_Sys, Desktop, nooop, dxf_out, dxf_out, DxfIn, DwgIn, Nowy_rysunek, New_from_template, Wejscie, Powrot,
  New_window_empty, New_window_from_template, New_window,
  Load_P_File, Load_P_File, Load_P_File, Load_P_File, Load_P_File,Load_P_File, Load_P_File, Load_P_File,
  Load_P_File, Load_P_File, Load_P_File, Load_P_File,Load_P_File, Load_P_File, Load_P_File, Load_P_File,
} ;


static int shift_grey(int kolory_blok)
{  int ret;
    ret=((kolory_blok+8)%16);
    return ret;
}

static double ColourDistance(RGBA e1, RGBA e2)
{
    long rmean = ( (long)e1.r + (long)e2.r ) / 2;
    long r = (long)e1.r - (long)e2.r;
    long g = (long)e1.g - (long)e2.g;
    long b = (long)e1.b - (long)e2.b;
    return sqrt((((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8));
}

void Konfig(void)
{
	int k1;
    int kolor_tla;
    RGBA c1, c2;
    double cdistance;

    kolor_tla=kolory.paper;

    k1=konfig(&kolory);

	Kolory_Paper=kolory.paper;  //TO BEDZIE UZYWANE PRZY NOWYCH RYSUNKACH

	if (kolory.paper!=kolor_tla)
	{
		regen_ctx=TRUE;
		redraw();
#ifdef ALLEGRO5
        flip_screen();
#endif
	}

  return;

}

void ClearWholeScreen(void)
{
cleardevice();
}

static char font_name_tab[][13]={u8"pc8x14.fnt","pc8x14t.fnt","pc8x16.fnt",
								 "pc8x18.fnt","pc11x22.fnt","pc14x28.fnt",
                                 "xm7x12.fnt","xm8x12.fnt","xm8x12b.fnt",
                                 "xm8x13.fnt","xm8x13b.fnt","xm8x14.fnt",
                                 "xm8x14b.fnt","xm8x15.fnt","xm8x15b.fnt",
                                 "xm8x16.fnt","xm8x16b.fnt","xm8x16i.fnt",
                                 "xm8x19.fnt","xm9x15.fnt","xm9x15b.fnt",
                                 "xm9x17.fnt","xm10x17.fnt","xm10x17b.fnt",
                                 "xm10x20.fnt","xm10x20b.fnt","xm11x19b.fnt",
                                 "xm12x15.fnt","xm12x20.fnt","xm12x20b.fnt",
                                 "xm12x23.fnt","xm12x24.fnt","xm14x26.fnt",
                                 "xm16x25.fnt","xm16x25b.fnt","xm16x25i.fnt",};

 char font_name1_tab[][13]={u8"pc8x14.fnt","pc8x14t.fnt","pc8x16.fnt", //2
								  "px8x18.fnt","px11x22.fnt","px14x28.fnt",  //5
                                  "pc8x14.fnt","pc8x14.fnt","pc8x14.fnt",  //8
                                  "pc8x14.fnt","pc8x14.fnt","pc8x14.fnt",  //11
                                  "pc8x14.fnt","pc8x16.fnt","pc8x16.fnt",  //14
                                  "pc8x16.fnt","pc8x16.fnt","pc8x16.fnt",  //17
                                  "px8x18.fnt","pc8x16.fnt","pc8x16.fnt",  //20
                                  "pc8x16.fnt","pc8x16.fnt","pc8x16.fnt",  //23
                                  "px11x22.fnt","px11x22.fnt","px11x22.fnt", //26
                                  "pc8x16.fnt","pc8x16.fnt","px11x22.fnt",  //29
                                  "px11x22.fnt","px11x22.fnt","px14x28.fnt", //32
                                  "px14x28.fnt","px14x28.fnt","px14x28.fnt",};  //35
                                                     //
								//	 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43
int MP_SIZE_TAB[]=      {13,13,15,17,21,27,12,12,12,13,13,13,13,14,14,15,15,15,18,15,15,16,17,17,20,20,18,14,19,19,22,24,25,26,26,26, 0, 0, 0, 0, 0, 0, 0, 3,



};
int ED_INF_HEIGHT_TAB[]={13,13,15,17,21,25,12,12,12,13,13,13,13,14,14,15,15,15,18,15,15,16,17,17,20,20,18,14,19,19,22,24,25,27,27,27,0,0,0,0,0,0,0,4,

};
int BAR_G_TAB[]=        {12,12,14,16,20,24,10,10,10,11,11,11,11,12,12,13,13,13,16,13,13,14,15,15,18,18,16,12,17,17,20,22,23,23,23,23,0,0,0,0,0,0,0,4,

};
int HEIGHT_TAB[]=       {12,12,14,16,21,26,12,11,11,12,12,12,12,13,13,14,14,14,17,14,14,15,16,16,19,19,17,13,18,18,21,23,24,25,25,25,0,0,0,0,0,0,0,4,

};
int WIDTH_TAB[]=        { 8, 8, 8, 8, 11, 14, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9,10,10,10,10,11,12,12,12,12,12,14,16,16,16,0,0,0,0,0,0,0,5,

};
static char font_file_name[12]="font.ini";
static char font_name[36];
static char font_name1[36];

void closebutton (void)
{
    close_button_pressed=TRUE;
    simulate_keypress ((KEY_ESC << 8) | scancode_to_ascii (KEY_ESC));
    return;

}

void Wsp_Autopan (void)
/*--------------------------*/
{
  char sk[MaxTextLen] = "";
  int retval_no = 1 ;
  double buf_ret [1] ;
  double d ;


  sk [0] = '\0' ;
  DF_to_String (sk, "%-6.2f", pan_dxy, 0) ;

  if (!read_esc(sk,60,154))  return;

  if (FALSE == calculator (sk, &retval_no, buf_ret)  || retval_no < 1)
  {
    return ;
  }
  d = buf_ret [0] ;
  if (d > 1) d = 1;
  if (d < 0.01) d=0.01;
  pan_dxy=d;
  d_2d () ;
  sprintf (sk, "%6.2lf", d) ;
  if (strlen (sk) > 6) sk [6] = '\0' ;
  menu_par_new ((*mOpcje.pola)[2].txt, sk) ;
}

void Expand_hor(void)
{
	expand_horizontally();
};

void Expand_ver(void)
{
	expand_vertically();
};

void Expand_diag(void)
{
	expand_diagonally();
};

int get_window_origin_and_size_(int *x_win_orig, int *y_win_orig, int *win_width, int *win_height) {
    return get_window_origin_and_size(x_win_orig,y_win_orig,win_width,win_height);
}

int Save_Update_flex(int save_upd, int *curr_h, int *curr_v)
{
    int ret;
    static int ret_left_x, ret_right_x, ret_top_y, ret_bottom_y;
    static int curr_x0, curr_y0;
    int curr_x01, curr_y01, curr_h1, curr_v1;
    int monitor=0;
    int ret_menu=0;
	int ret_ref;

    if (save_upd==0) {
        ret_ref = get_window_origin_and_size(&curr_x0, &curr_y0, curr_h, curr_v);

#ifdef LINUX
        ret = get_monitor_dims(&ret_left_x, &ret_right_x, &ret_top_y, &ret_bottom_y, -1);
        xwin_set_hints(320, 200);
#else
        ret = get_monitor_dims(&ret_left_x, &ret_right_x, &ret_top_y, &ret_bottom_y, -1);
#endif
    }
    else
    {
        ret_ref = get_window_origin_and_size(&curr_x01, &curr_y01, &curr_h1, &curr_v1);

        if ((curr_h1!=(*curr_h)) || (curr_v1!=(*curr_v)) || (ret_ref))
        {
            if (get_menu_level()>0) {
                while (get_menu_level()>0)
                    if (menu_address[get_menu_level()-1]!=NULL)
                    {

                        if (!(((TMENU*)(menu_address[get_menu_level()-1]))->flags & NOCLOSE))
                        {
                            if(opwin) closew((TMENU*)menu_address[get_menu_level()-1]);
                            else deactiv_menu((TMENU*)menu_address[get_menu_level()-1]);
                        }
                    }
                unredcr_menu();
                ret_menu=-999;
            }
            save_window_dim(curr_x0, curr_y0, *curr_h, *curr_v);
#ifdef LINUX
            expand_dim(curr_x01, curr_y01, curr_h1, curr_v1);
#else
            expand_dim(curr_x01, curr_y01, curr_h1, curr_v1);
#endif
        }

        //cleaning the keyboard
        clean_key_buffer();
        clear_keybuf();

        close_button_pressed=FALSE;

        return ret_menu;
    }
}

int Expand_flex__(void)
{  int keys_;

    free_mouse();
    while (1) {
        Test_przycisniec(&keys_);
        if (keys_>0)
            break;
        my_sleep(10);
    }
    lock_mouse();
    return 0;
}

int Expand_flex(void)
{
	int ret;

    int ret_left_x, ret_right_x, ret_top_y, ret_bottom_y;
    int curr_x0, curr_y0, curr_h, curr_v;
    int curr_x01, curr_y01, curr_h1, curr_v1;
    int monitor=0;
    int ret_menu=0;
	int ret_ref;
	long mouse_curx, mouse_cury;
    int WspX_, WspY_;
    double X_,Y_;
    BOOL go_A_Screen=FALSE;

    struct shmbuf* bufptr;

    get_posXY(&X_, &Y_);

    ret_ref = get_window_origin_and_size(&curr_x0, &curr_y0, &curr_h, &curr_v);
	mouse_curx = pikseleX(X);
	mouse_cury = pikseleY(Y);


#ifdef LINUX
    ret = get_monitor_dims(&ret_left_x, &ret_right_x, &ret_top_y, &ret_bottom_y, -1);
    xwin_set_hints(320, 200);
#ifdef MACOS
    enable_hardware_cursor();
    show_os_cursor(MOUSE_CURSOR_ARROW);

 if (MacOS15==TRUE) go_A_Screen=FALSE; //TRUE;
 #ifdef ARM64
 go_A_Screen=FALSE;
 #endif

    //opening transparent dialogbox
    if (go_A_Screen==TRUE) {
        ret=Get_A_Screen(ret_left_x, ret_right_x, ret_top_y, ret_bottom_y, (int)mouse_curx, (int)mouse_cury);
    }
    else {
        //or just waiting

        hibernate = TRUE;
#ifdef MACOS  //ARM64
        set_semaphore(0); ////arm64
#endif

        position_mouse_xy((int)mouse_curx, (int)mouse_cury);

        int keys_;

        mouse_b=0;

        while (1)
            {
            keys_=mouse_b;
            if (keys_>0)
            {
                while (mouse_b) my_sleep(10);
                hibernate = FALSE;
                break;
            }
            Check_XNextEvent();
            /////
            if (check_file_buffer())
            {
#ifdef MACOS
             enable_hardware_cursor();
             show_os_cursor(MOUSE_CURSOR_ARROW);
#endif
            }
            /////
        }
#ifdef MACOS //ARM64
        set_semaphore(1); ////arm64
#endif

        lock_mouse();
        position_mouse(getmaxx()/2, getmaxy()/2);
        set_forget_mouse(getmaxx()/2, getmaxy()/2);
    }

    disable_hardware_cursor();
#else
#ifdef ALLEGRO5
    enable_hardware_cursor();
    show_os_cursor(MOUSE_CURSOR_ARROW);

    //or just waiting
    //enable_hardware_cursor();
    hibernate = TRUE;
    set_semaphore(0);
    position_mouse_xy((int)mouse_curx, (int)mouse_cury);

    free_mouse();
    int keys_;

    mouse_b=0;

    while (1)
    {
        keys_=mouse_b;
        if (keys_>0)
        {
            while (mouse_b) my_sleep(10);
            hibernate = FALSE;
            break;
        }
        Check_XNextEvent();
        /////
        if (check_file_buffer())
        {
#ifdef MACOS
            enable_hardware_cursor();
             show_os_cursor(MOUSE_CURSOR_ARROW);
#endif
        }
        /////
    }

    set_semaphore(1);

    lock_mouse();
    position_mouse(getmaxx()/2, getmaxy()/2);
    set_forget_mouse(getmaxx()/2, getmaxy()/2);

    disable_hardware_cursor();
#else
    ret=Get_A_Screen(ret_left_x, ret_right_x, ret_top_y, ret_bottom_y, (int)mouse_curx, (int)mouse_cury);
#endif
#endif

#else
    ret = get_monitor_dims(&ret_left_x, &ret_right_x, &ret_top_y, &ret_bottom_y, -1);

    ret=Get_A_Screen(ret_left_x, ret_right_x, ret_top_y, ret_bottom_y, (int)mouse_curx, (int)mouse_cury);
#endif

    ret_ref = get_window_origin_and_size(&curr_x01, &curr_y01, &curr_h1, &curr_v1);

    if ((curr_h1!=curr_h) || (curr_v1!=curr_v) || (ret_ref))
    {
        if (get_menu_level()>0) {
            while (get_menu_level()>0)
                if (menu_address[get_menu_level()-1]!=NULL)
                {

                    if (!(((TMENU*)(menu_address[get_menu_level()-1]))->flags & NOCLOSE))
                    {
                        if(opwin) closew((TMENU*)menu_address[get_menu_level()-1]);
                        else deactiv_menu((TMENU*)menu_address[get_menu_level()-1]);
                    }
                }
            unredcr_menu();
            ret_menu=-999;
        }
        save_window_dim(curr_x0, curr_y0, curr_h, curr_v);

		if ((curr_h1 > 0) && (curr_h1 < MIN_H)) curr_h1 = MIN_H;
		if ((curr_v1 > 0) && (curr_v1 < MIN_V)) curr_v1 = MIN_V;
#ifdef LINUX

        expand_dim(curr_x01, curr_y01, curr_h1, curr_v1);
#else
		expand_dim(curr_x01, curr_y01, curr_h1, curr_v1);
#endif
    }
    //cleaning the keyboard
    clean_key_buffer();
    clear_keybuf();

    if (close_button_pressed) Koniec();
    close_button_pressed=FALSE;

    if ((BAR_POINTER) && (get_menu_level()>0))
    {
        Restore_Pointer();
    }

#ifdef ALLEGRO5
#ifdef MACOS
    MoveOSXCursor(getmaxx()/2, getmaxy()/2);
#else
    position_mouse_xy(getmaxx()/2, getmaxy()/2);
#endif
#else
    position_mouse(getmaxx()/2, getmaxy()/2);
#endif
    set_forget_mouse(getmaxx()/2, getmaxy()/2);
    my_sleep(10);

    //// ????
    do {
        get_mouse_mickeys(&WspX_, &WspY_);
    } while ((WspX_!=0) || (WspY_!=0));
    ////
    set_posXY(X_, Y_);
    my_sleep(10);
    CUR_ON(X,Y);
    my_sleep(10);

    return ret_menu;
};

void Expand_flex0(void)
{
	int ret;

#ifndef LINUX
	ret = circle_and_rectangle_proc();
	//cleaning the keyboard
	clean_key_buffer();
	clear_keybuf();
#else
	Expand_flex();
#endif
	}


void Expand_last(void)
{
	restore_last_window();
};

void Translucency(void)
{
	int n;
    char sk [MaxTextLen] = "";
	int translucency_tab[] = { 255, 230, 204, 179, 153, 128, 102, 77 };

	n = mTranslucency.poz + mTranslucency.foff;
	TRANSLUCENCY = translucency_tab[n];
	set_trans_blender(0, 0, 0, TRANSLUCENCY);

    sprintf(sk, "%d%%", (int)((TRANSLUCENCY*100/255)+0.5));
    menu_par_new((*mOpcje.pola)[3].txt, sk);

	redraw();
}

void DemoModeOn(void)
{
	int ret;
	draw_keyimage_(-1, FALSE, FALSE, FALSE);

	DEMO_RECORDING = TRUE;
	set_demo_scale(2.0);
	menu_par_new((*mOpcje.pola)[9].txt, _YES__);
	ret = ask_question(1, "", "OK", "", _DEMO_MODE_, 1, _DEMO_MODE1_, 1, 1, 61);
}

void DemoModeOff(void)
{
	draw_demokeyimage_(-1, -1, FALSE, FALSE, FALSE);

	DEMO_RECORDING = FALSE;
	set_demo_scale(1.0);
	menu_par_new((*mOpcje.pola)[9].txt, _NO__);

}

void DialogCursorS(void)
{
	if (BIGCURSOR==0) return;
	set_dialog_cursor(0);
}

void DialogCursorB(void)
{
	if (BIGCURSOR==1) return;
	set_dialog_cursor(1);
}

void DialogCursorEB(void)
{
    if (BIGCURSOR==2) return;
    set_dialog_cursor(2);
}


void MenuCursorB(void)
{
    if (!BAR_POINTER) return;
    set_menu_cursor(FALSE);
}

void MenuCursorP(void)
{
    if (BAR_POINTER) return;
    set_menu_cursor(TRUE);
}


void  wysokosc_znaku_TTF(void)
{
	char sk[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];
	double d;
	int ret;
	double dwt;
    int old_tier, new_tier;

	DF_to_String(sk, "%-6.2f", (double)HEIGHT, 0);
	if (!get_string(sk, "", MaxTextLen, 0, 10)) return;
	if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
	{
		return;
	}
	d = buf_ret[0];
	if (d <= 0)
	{
		ErrList(30);
		return;
	}
	if (d < 12) d = 12.0;
	//if (d > 32) d = 32.0;
    if (d > 64) d = 64.0;


	dwt = ((float)WIDTH / (float)HEIGHT)*1.9;

	MP_SIZE = BAR_G = HEIGHT = (int)(d+0.5);
	ED_INF_HEIGHT = HEIGHT + 2*(RETINA+1); //1;

	WIDTH = (int)((dwt * (float)HEIGHT / 1.9)+0.5);

	menu_par_new((*mCzcionkaEkranTTF.pola)[1].txt, sk);
	Save_Desktop_font(Czcionka_Pulpitu);
	set_ttf_digits27_len();

    /*
    //checking tier
    old_tier=get_tier();
    new_tier=what_tier(HEIGHT);
    if (new_tier!=old_tier)  //changing icons
    {
        Destroy_Bitmaps();
        Load_Bitmaps(HEIGHT);
        Set_Mem_Bitmaps(new_tier);
    }
    */
	redraw();
	set_yp();
	okno_all();
	ClearWholeScreen();
	layer_info();
	regen_ctx = TRUE;
	redraw();
    flip_screen();

    //checking tier
    old_tier=get_tier();
    new_tier=what_tier(HEIGHT);
    if (new_tier!=old_tier)  //changing icons
    {
        Destroy_Bitmaps();
        Load_Bitmaps(HEIGHT);
        reload_client_bitmaps();
        Set_Mem_Bitmaps(new_tier);
        Set_Mem_Cursors(new_tier);
    }

    reset_editbox_geometry_set();

	ret = ask_question(1, "", "OK", "", _FONT_CHANGED_, 12, "", 11, 1, 0);

}

static void width_factor_TTF(void)
/*----------------------------*/
{
	char sk[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];
	double d;
	int ret;

	DF_to_String(&sk, "%-6.4f", ((float)WIDTH/(float)HEIGHT)*1.9, 0);
	if (!get_string(sk, "", MaxTextLen, 0, 33)) return;
	if (FALSE == calculator(sk, &retval_no, buf_ret) || retval_no < 1)
	{
		return;
	}
	d = buf_ret[0];
	if (d <= 0)
	{
		ErrList(30);
		return;
	}
	if (d > 2)
	{
		d = 2;
	}
	if (d < 0.5)
	{
		d = 0.5;
	}

	WIDTH = (int)((d * (float)HEIGHT / 1.9)+0.5);
	menu_par_new((*mCzcionkaEkranTTF.pola)[2].txt, sk);

	Save_Desktop_font(Czcionka_Pulpitu);
	set_ttf_digits27_len();
	redraw();
	set_yp();
	okno_all();
	ClearWholeScreen();
	layer_info();
	regen_ctx = TRUE;
	redraw();
    flip_screen();
	ret = ask_question(1, "", "OK", "", _FONT_CHANGED_, 12, "", 11, 1, 0);
}

void DesktopFontTTF(void)
{
	char sk[MAXPATH] = "";
	int ret;
	char dir[MAXDIR]="";
	char drive[MAXDRIVE]="";
	char file[MAXFILE]="";
	char ext[MAXEXT]="";
	int flags;

	if (Load_File(sk, INI_TTF_FONT, TRUE) == FALSE)
	{
		return;
	}

	//font setup
	Save_Desktop_font(sk);
	Set_Desktop_font(sk);

	flags = fnsplit(sk, drive, dir, file, ext);
	strcat(file, ext);
	strcpy(Czcionka_Pulpitu, file); // font_name);
	strcpy(Desktop_Font_File, sk); // font_name);
	find_font_face(Desktop_Font_File);
	menu_par_new((*mCzcionkaEkranTTF.pola)[0].txt, Czcionka_Pulpitu);

	redraw();
	set_yp();
	okno_all();
	ClearWholeScreen();
	layer_info();
	regen_ctx = TRUE;
	redraw();
	ret = ask_question(1, "", "OK", "", _FONT_CHANGED_, 12, "", 11, 1, 0);

}

void DesktopFontOTF(void)
{
    char sk[MAXPATH] = "";
    int ret;
    char dir[MAXDIR]="";
    char drive[MAXDRIVE]="";
    char file[MAXFILE]="";
    char ext[MAXEXT]="";
    int flags;

    if (Load_File(sk, INI_OTF_FONT, TRUE) == FALSE)
    {
        return;
    }

    //font setup
    Save_Desktop_font(sk);
    Set_Desktop_font(sk);

    flags = fnsplit(sk, drive, dir, file, ext);
    strcat(file, ext);
    strcpy(Czcionka_Pulpitu, file); // font_name);
    strcpy(Desktop_Font_File, sk); // font_name);
    find_font_face(Desktop_Font_File);
    menu_par_new((*mCzcionkaEkranTTF.pola)[0].txt, Czcionka_Pulpitu);

    redraw();
    set_yp();
    okno_all();
    ClearWholeScreen();
    layer_info();
    regen_ctx = TRUE;
    redraw();
    ret = ask_question(1, "", "OK", "", _FONT_CHANGED_, 12, "", 11, 1, 0);

}

void CzcionkaE(void)
{ int n;
  FILE *f_ini;
  int ret;

  n = mCzcionkaEkran.poz + mCzcionkaEkran.foff;
  strcpy(font_name,"");
  strcat(font_name,font_name_tab[n]);
  strcpy(font_name1,"");
  strcat(font_name1,font_name1_tab[n]);

	FONTNUMBER=n;

    f_ini = fopen ( font_file_name , "wt" ) ;
    fprintf(f_ini,"%s\n",font_name);
    fprintf(f_ini,"%ld,%ld,%ld,%ld,%ld,%ld\n",MP_SIZE,ED_INF_HEIGHT,BAR_G,HEIGHT,WIDTH,FONTNUMBER);
    fprintf(f_ini,"%s\n",font_name1);
    fclose(f_ini);

	strcpy(Czcionka_Pulpitu,font_name);
	redraw();
    set_yp();
    okno_all();
    ClearWholeScreen();
    layer_info ();
	regen_ctx=TRUE;
    redraw();

	 ret=ask_question(1, "","OK","", _FONT_CHANGED_, 12, "", 11, 1, 0);

}


static void auto_pan_on(void)
/*-------------------------*/
{
	if (Auto_Pan) return;
	Auto_Pan = TRUE;
	menu_par_new((*mOpcje.pola)[1].txt, YES);
}

static  void auto_pan_off(void)
/*---------------------------*/
{
	if (!Auto_Pan) return;
	Auto_Pan = FALSE;
	menu_par_new((*mOpcje.pola)[1].txt, NO);
}

static void WheelNatural(void)
{
    if (Get_Mouse_Wheel()==1) return;
    Set_Mouse_Wheel(0);
    menu_par_new((*mOpcje.pola)[8].txt, _NATURAL__);
    save_mouse_wheel();
}

static void WheelRegular(void)
{
    if (Get_Mouse_Wheel()==-1) return;
    Set_Mouse_Wheel(1);
    menu_par_new((*mOpcje.pola)[8].txt, _REGULAR__);
    save_mouse_wheel();
}

static void (*COMNDO[])(void)=
{
  Konfig, nooop, Wsp_Autopan, nooop, nooop, Open_Backgrounds, nooop, nooop, nooop, nooop, Save_Last_Window_Settings, nooop, Expand_hor, Expand_ver, Expand_diag, Expand_flex0, Expand_last,
  Translucency,Translucency,Translucency,Translucency,Translucency,Translucency,Translucency,Translucency, DialogCursorS, DialogCursorB, DialogCursorEB,
  DemoModeOn, DemoModeOff,
  nooop, wysokosc_znaku_TTF, width_factor_TTF,
  auto_pan_on, auto_pan_off, MenuCursorB, MenuCursorP, DesktopFontTTF,DesktopFontOTF, WheelNatural, WheelRegular
} ;

void uaktualnij_pola_file (void)
/*-----------------------------------*/
{
  char sk1[MAXPATH];
  char * st ;
  char *ptrsz_lab ;
  int i;
  int max_xdl;
  int pos;
  int max_n;

  max_xdl=33;
  max_n = 0;
  for (i=0; i<MAXHISTORYFILES; i++)
  {
    strcpy(sk1, Previous_File[i]);
	if (strlen(sk1) == 0) continue;
	max_n++;
    ptrsz_lab = sk1;

	pos = findfpostopxl(ptrsz_lab, 24 * WIDTH);

	st = &ptrsz_lab[pos];
	memmove(&ptrsz_lab[6], st, strlen(st) + 1); //z '\0' na koncu
	ptrsz_lab[3] = '.';
	ptrsz_lab[4] = '.';
	ptrsz_lab[5] = '.';

	menu_par_new((*mLastFiles.pola)[i].txt, ptrsz_lab) ;
  }
  mLastFiles.xdl=max_xdl;
  mLastFiles.max = max_n;
  if (mLastFiles.max == 0) mQuit.max = 13;
  else mQuit.max = 14;
}

void Wyjscie(void)
/*-----------------*/
{
  int n;

  uaktualnij_pola_file();    //win32todo
  out_file_name () ;
  Semaphore = FALSE;
  if((n=getwsp1(&mQuit)-1)>=0) (*COMND[n])();    /* n=-1 -> esc */
  if ((n<25) &&  (n!=11)) LASTFUN=COMND[n]; //do not record loading file from history as a last function, so n<25, and do not overwrite demo logo function)
  komunikat0(0);
}

void Opcje(void)
/*-----------------*/
{ char sk [MaxTextLen] = "";
  int n;
  char dir[MAXDIR]="";
  char drive[MAXDRIVE]="";
  char file[MAXFILE]="";
  char ext[MAXEXT]="";
  int flags;

  Semaphore = FALSE;

  if (Auto_Pan) menu_par_new((*mOpcje.pola)[1].txt, YES);
  else menu_par_new((*mOpcje.pola)[1].txt, NO);

  sprintf (sk, "%#6.2lf", pan_dxy) ;
  if (strlen (sk) > 6) sk [6] = '\0' ;
  menu_par_new ((*mOpcje.pola)[2].txt, sk) ;

  sprintf(sk, "%d%%", (int)((TRANSLUCENCY*100/255)+0.5));
  menu_par_new((*mOpcje.pola)[3].txt, sk);

  sprintf(sk, "%s", Czcionka_Pulpitu);
  menu_par_new((*mOpcje.pola)[4].txt, sk);
  menu_par_new((*mCzcionkaEkranTTF.pola)[0].txt, sk);

  flags = fnsplit(background_pcx_file, drive, dir, file, ext);

  sprintf(sk, "%s", file);
  menu_par_new((*mOpcje.pola)[5].txt, sk);

  if (BIGCURSOR==2) strcpy(sk, hugecursor);
  else if (BIGCURSOR==1) strcpy(sk, bigcursor);
  else strcpy(sk, smallcursor);
  menu_par_new((*mOpcje.pola)[6].txt, sk);

  if (!BAR_POINTER) strcpy(sk, barstyle);
  else strcpy(sk, cursorstyle);
  menu_par_new((*mOpcje.pola)[7].txt, sk);

  if (Get_Mouse_Wheel() == 1) menu_par_new((*mOpcje.pola)[8].txt, _NATURAL__);
  else menu_par_new((*mOpcje.pola)[8].txt, _REGULAR__);

  if (DEMO_RECORDING == TRUE) menu_par_new((*mOpcje.pola)[9].txt, _YES__);
  else menu_par_new((*mOpcje.pola)[9].txt, _NO__);

  sprintf(sk, "%d", HEIGHT);
  menu_par_new((*mCzcionkaEkranTTF.pola)[1].txt, sk);
  sprintf(sk, "%4.2f", ((float)WIDTH/(float)HEIGHT)*1.9);
  menu_par_new((*mCzcionkaEkranTTF.pola)[2].txt, sk);
  Semaphore = FALSE;
  if((n=getwsp1(&mOpcje)-1)>=0)
      (*COMNDO[n])();    /* n=-1 -> esc */
  //LASTFUN=COMNDO[n];  //is not recorded

  komunikat0(0);
}

#define DXButDsk 40 //48
#define DYButDsk 28 //32
#define dx_BUT 5
#define dy_BUT 5

#define ID_ESCAPE         1
#define ID_SWITCH  Dlg_Ret_Val_IMPORT_BLOCK

#define nof_box_dsk 65
#define nof_but_dsk 1


static GROUP_BOX gr_box_desktop[nof_box_dsk] =
{
	{ 100, 100, 200, 200,
	COLOR_NULL ,COLOR_NULL, "Desktop", 0, NULL,
  },
};

static GROUP_BOX gr_box_desktop_def = { 0, 0, 100, 100, COLOR_NULL ,COLOR_NULL, "", 0, NULL};

static BUTTON buttons_desktop[nof_but_dsk] =
{
{ 100, 100, DXButDsk, DYButDsk, COLOR_NULL,
	COLOR_NULL, COLOR_NULL, "ESC", 0,B_PUSHBUTTON, 99, 1,0, ID_ESCAPE, 0, 0,
  },
};

static BUTTON buttons_desktop_def = { 100, 100, DXButDsk, DYButDsk, COLOR_NULL,
COLOR_NULL, COLOR_NULL, "", 0, B_FLATPUSHBUTTON, 99, 1, 0, -1, 0, 0,
};

static IMAGE images_desk[] =
{
	{ 20,20, 64, 64, 113, desktop_tip},
};

static TDIALOG desktop_dlg =
{
	5, 5, WIDTH_WORLD - 10, HEIGHT_WORLD - 10,COLOR_NULL,COLOR_NULL,COLOR_NULL, COLOR_NULL, 0x20,  0, 0,
	"",
	0, NULL,
	0, NULL,
	0, &gr_box_desktop,
	0, NULL,
	1, &images_desk,
	1, &buttons_desktop,
	0, NULL,
	0,NULL,
    0,NULL, //Sliders
	NULL,
	NULL,
	0,
	0,
    NULL,
};

static int proc_dlg_desktop(int n)
/*------------------------------*/
{
	int ret = Dlg_Ret_Val_Cancel;

	switch (n)
	{
	case ID_ESCAPE:
		ret = Dlg_Ret_Val_Cancel;
		break;
	default:
		ret = n;
		break;
	}
	return ret;
};


void Dump_View(int box, int x01, int y01, int x02, int y02, int x00, int y00)
{
	my_blit(dump_bitmap_p[box], 0, 0, x01, y01, x02 - x01, y02 - y01);
	return;
}


void Desktop(void)
/*-----------------*/
{
	int client;
	PALETTE palette;
	int real_x1, real_y1, real_x2, real_y2;
	int ret;
	char drawing_file[MAXPATH];
	char dump_file[MAXPATH];
	int Ret_Val;
	static DLG_COLOR color_dlg;
	double wsp_x, wsp_y;
	int i_file;
	int i;
	char dir__[MAXPATH]="";
	char drive__[MAXDRIVE]="";
	char file__[MAX_NUMBER_OF_WINDOWS][MAXFILE]={""};
	char ext__[MAXEXT]="";
	int flags;
	int client_no[64];
	int active_clients;
	int row_no, col_no, row, col, offs;
	int width_, height_;
	int NEW_DRAWING_NUMBER;
    static int curr_h, curr_v;

	CopyPreview(0, 0, 320, 320, DRAWING_NUMBER);
	ret = Deposit_params();

	Save_View_Preview();

	for (i = 0; i < MAX_NUMBER_OF_WINDOWS; i++)  //64
	{
		if (dump_bitmap[i] != NULL)
		{
			destroy_bitmap(dump_bitmap[i]);
			dump_bitmap[i] = NULL;
		}
	}

	desktop_dlg.flags = 0x20;
	desktop_dlg.SizeGroupT = 0;
	i_file = 0;

	wsp_x = max(1.75 / 9.0 * (float)WIDTH, 1.75);
	wsp_y = max(2.5 / 19.0 * (float)HEIGHT, 2.5);  //15

	ShowPreview(0, 0, (int)(getmaxx() / 2), (int)(getmaxy() - (2 * HEIGHT) + 1));
#ifdef ALLEGRO5
    flip_screen();
#endif
	active_clients = return_active_clients() - 1;  //deducted by itself

	height_ = (int)(getmaxy());
	width_ = (int)(getmaxx() / 2);

	//counting number of possible rows
	row_no = (int)(((float)height_ / 325.0) + 0.5);
	col_no = (int)(((float)width_/ 325.0) + 0.5);

	row = 0; col = 0; offs = 0;

	for (client = 0; client < MAX_NUMBER_OF_WINDOWS; client++)
	{
		if (client != DRAWING_NUMBER)
		{
			if (Get_dane0_DATA(client) != NULL)
			{
				ret = return_and_convert_bitmap(client, &dump_file, &real_x1, &real_y1, &real_x2, &real_y2, &drawing_file);
				if ((ret) && (strlen(dump_file) > 0))
				{

					memmove(&gr_box_desktop[i_file], &gr_box_desktop_def, sizeof(GROUP_BOX));

					gr_box_desktop[i_file].x = (int)(((getmaxx() / 2) + col * 325 + offs * 100) / wsp_x);
					gr_box_desktop[i_file].y = (int)((row * 325 + offs * 100) / wsp_y);
					gr_box_desktop[i_file].dx = (real_x2 - real_x1) / wsp_x;
					gr_box_desktop[i_file].dy = (real_y2 - real_y1) / wsp_y;

					if (strlen(drawing_file) > 0)  flags = fnsplit(drawing_file, drive__, dir__, file__[i_file], ext__);
					else strcpy(file__[i_file], "");


					if (strlen(file__[i_file]) == 0)
						strcat(file__[i_file], "***");

					if (Drawing_Params[client].Change)
					{
						gr_box_desktop[i_file].ink = GetColorAC(5);
					}

					gr_box_desktop[i_file].txt = file__[i_file];
					gr_box_desktop[i_file].id = client + ID_SWITCH;
					desktop_dlg.SizeGroupT++;

					dump_bitmap[i_file] = load_bitmap(dump_file, palette);
					dump_bitmap_p[i_file] = (char *)dump_bitmap[i_file];

					gr_box_desktop[i_file].process = (void (*)(void)) Dump_View;

					client_no[i_file] = client;

					i_file++;
					row++;
					if ((row) == row_no)
					{
						row = 0;
						col++;
						if ((col) == col_no)
						{
							col = 0;
							offs++;
						}
					}
				}
			}
		}
	}


	desktop_dlg.dx = ((float)getmaxx() - 20.0) / wsp_x;
	desktop_dlg.dy = ((float)getmaxy() - 42.0 *((float)HEIGHT / 12.0) - 20) / wsp_y;

	desktop_dlg.x = 10 / wsp_x;
	desktop_dlg.y = 10 / wsp_y;

	Get_Default_Color_Dlg(&color_dlg);

	buttons_desktop[0].x = desktop_dlg.dx - DXButDsk - dx_BUT;
	buttons_desktop[0].y = desktop_dlg.dy - DYButDsk - dy_BUT;

	images_desk[0].x1 = desktop_dlg.dx - DXButDsk - dx_BUT;
	images_desk[0].x2 = DXButDsk*wsp_x;
	images_desk[0].y1 = desktop_dlg.dy - DYButDsk - dy_BUT;
	images_desk[0].y2 = DYButDsk*wsp_y;

    Save_Update_flex(0, &curr_h, &curr_v);
	Ret_Val = Dialog(&desktop_dlg, &color_dlg, proc_dlg_desktop, bMouse);
    Save_Update_flex(1, &curr_h, &curr_v);

	preview_blocked = TRUE;
	no_break = TRUE;

	Restore_View_Preview();
	redraw();
	if (!snap_)krok_s = jednostkiY(1);

	layer_info();
	view_scale();
	no_break = FALSE;
	preview_blocked = FALSE;

#ifdef ALLEGRO5
 flip_screen();
#endif

	if (Ret_Val != Dlg_Ret_Val_Cancel)
	{
		NEW_DRAWING_NUMBER = Ret_Val - Dlg_Ret_Val_IMPORT_BLOCK;
		DRAWING_NUMBER = NEW_DRAWING_NUMBER;
		ret = Restore_params();
		out_file_name();
		Clear_View();

		setaspectratio(10000, min(32000., 10000. / sk_x));
		d_2d();
		if (Xp + X2d > Xmax) Xp = Xmax - X2d;	//na wypadek zmiany rozdzielczosci monitora
		if (Xp < Xmin) Xp = Xmin;
		if (Yp + Y2d > Ymax) Yp = Ymax - Y2d;
		if (Yp < Ymin) Yp = Ymin;
		Xk_Yk();
		pXp = pikseleX(Xp);
		pYp = pikseleY(Yp);

		reset_background();

		if (!snap_)krok_s = jednostkiY(1);
		uaktualnij_pola();

		setfillstyle_(EMPTY_FILL, 0);
		bar(xp, yp, maxX - xk, maxY - yk);

		Ini_Global_Object();

		redraw();

		Ini_Place_Marker();
		Ini_Layers_Dlg();
#ifdef ALLEGRO5
        flip_screen();
#endif
	}

	return;
}

PRIV_PRN_WINDOW *get_priv_prn_window(void)
{
    return &priv_prn_window;
}

void set_priv_prn_window(double Xmin_, double Ymin_, double Xmax_, double Ymax_)
{
    priv_prn_window.xp=Xmin_;
    priv_prn_window.yp=Ymin_;
    priv_prn_window.xk=Xmax_;
    priv_prn_window.yk=Ymax_;
}
#undef __O_WYJ__
