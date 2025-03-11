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

#define __RYSUJ__

#include<forwin.h>

#include<stdlib.h>
#ifndef LINUX
#include<process.h>
#else
#endif

#define ALLEGWIN
#include <allegext.h>

#include <glyph.h>
#ifndef LINUX
#include<dos.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/shm.h>
#endif
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<glyph.h>

#include "leak_detector_c.h"

#ifdef LINUX
#include "res/resource.h"

#else
#include "..\..\source\res\resource.h"
#endif

#ifdef MIDI_ON
#include<b_alleg.h>
#endif

#include "bib_e.h"
#include "alffont.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "alfpro.h"
#include "o_inigfx.h"
#include "alf_res.h"
#include "a4a_animation.h"

#include <glyph.h>
#include "main.h"
#include "o_static.h"
#include "o_animate_dynamics.h"

#include "icon_yes_d.h"
#include "icon_no_d.h"
#include "icon_upgrademark_d.h"
#include "icon_noupgrademark_d.h"
#include "icon_hourglass_d.h"

#include "leak_detector_c.h"

#define GLYPH_TARGET GLYPH_TARGET_ALLEGRO

#define TTF_SIZE_BUFFER FALSE

BOOL go_reset_if_resized=FALSE;

char Window_Name[32]="\x00";

static char *XDG_CURRENT_DESKTOP;

static GLYPH_REND *rend=NULL;
static GLYPH_FACE *face=NULL;

static GLYPH_FACE *face_UI=NULL;
static GLYPH_REND *rend_UI=NULL;
static GLYPH_KEEP *keep_UI=NULL;

static GLYPH_FACE *face_ini=NULL;
static GLYPH_REND *rend_ini=NULL;
static GLYPH_KEEP *keep_ini=NULL;

char** argv_;
int argc_;

char* argv__;
char* argp__;

int poll_kbd, poll_m;

DRIVER_STRUCT drv_cur;

int roz_in = 32000L;
char *alft = NULL;

BOOL ttf_refresh = FALSE;
int TTF_bold_factor = 140;
double TTF_width_factor = 1.25;
double TTF_height_factor = 1.4;

extern FONT *font1, *font2, *font3;
extern GLYPH_FACE *face_ttf[128];
extern BOOL TTF_redraw;
extern int empty_dlg();

extern void Check_ConfigureNotify(void);

extern void _free_mouse(void);
extern void dialog_cursor(int on);
extern void relock_mouse(void);

extern void  QInitialize(int argc, char *argv[]);

extern int slider(void);

char default_path_TTF[MAXPATH]="";
char default_path_OTF[MAXPATH]="";
char *winvar;
char *winfont;
char *otffont;

static int wt_lw_no;
int PL166, PL266, PL366;

#define MAXLINE 255

#define DRV_MASTER 0
#define DRV_SLAVE 1
static int graphic_drv;
static char font_file_name[12]="font.ini";
static char font_name[MAXPATH];
static char font_name1[MAXPATH];
static char sk1[12];
FILE *stru_ini;
static BOOL music_avail;

extern BITMAP *Get_Screenplay(void);
extern void GrMouseEraseCursor(void);

extern void setrgbdefaults(void);
extern void out_file_name (void) ;
extern BOOL Auto_Backup_Proc (void);
extern BOOL ScrSave_Proc (int);
extern void Open_File_Trace (void) ;
extern void Close_File_Trace (void) ;
extern void Hatch (void) ;
extern void Geometria (void) ;
extern int wczytaj_komunikaty (void) ;
extern void re_set_params(void);
extern int first_window_main;
extern void closegraph(void);
extern void change_mode_gr(DRIVER_STRUCT *drv);
extern void lock_mouse(void);
extern void Copy_screen(void);
extern void monitor_parametry_again(int Driver);
extern void mysetrgbdefaults(void);
extern int check_tables(void);
extern void destroy_parent(void);
extern void change_second_screen(void);
extern void cleardevice(void);
extern void increase_window(DRIVER_STRUCT *drv, int x, int y);
extern void free_mouse(void);
extern void set_resized_window_GFX(DRIVER_STRUCT *drv, int dx, int dy);
extern void set_original_window_GFX(DRIVER_STRUCT *drv);
extern void set_last_window_GFX(DRIVER_STRUCT *drv);
extern void Initial_Message(char file_name[255]);
extern void Child_Message(int mode_);
extern int Test_App(int doit, char *file_name);
extern int GrMouseDetect(void);
extern void fade_out1(int speed);
extern int get_desktop_color_depth(void);
extern int get_master_data(DRIVER_STRUCT *drv_data);
extern int get_slave_data(DRIVER_STRUCT *drv_data);
extern void lock_mouse_switch(void);
extern void Load_Data(void);
extern void create_second_screen(void);
extern int alfacad_logo(void);
extern BOOL Copy_File (char *ptrsz_fnd, char *ptrsz_fns);
extern void set_default_background(void);
extern void set_special_background(char *file_pcx);
extern void settextstyle(int font, int direction, int charsize);
extern void setcolor(int kolor);
extern void settextjustify(int horiz, int vert);
extern void __djgpp_set_ctrl_c(int flaga);
extern void Clear_View (void);
extern void Ini_Layers_Dlg (void);
extern void Save_Last_Window_Settings(void);
extern void load_file_to_history(char *sk);

extern int GoRegRedraw(void(*ptr)(void));
extern int TestRedraw(void);
extern int testCall(int val);

extern int key_buffer;
extern BOOL Semaphore;
extern BOOL Cust_Semaphore;

extern void set_pattern_offset(int offs);
extern void set_pattern_count(BOOL p_count);

extern int my_poll_mouse(void);
extern int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext);
extern void getcolor_RGB_char(unsigned char *red, unsigned char *green, unsigned char *blue, int color);
extern int getcolor(void);
extern void  gettextjustify(int *horiz, int *vert);
extern int32_t utf8_to_ucs2 (const uint8_t * input, const uint8_t ** end_ptr);
extern int utf8len(const char *s);

extern int gk_face_character_outline(GLYPH_FACE* const gface, const unsigned code, char *alf, long *lw0, int yMax0);
extern int gk_face_character_ymax(GLYPH_FACE* const gface, int *yMax0);

extern int my_poll_mouse(void);
extern int my_poll_keyboard(void);

extern void hide_x_cursor(void);
extern void show_x_cursor(void);

extern int Set_XWindow_header_height(void);

extern void set_mickey_hand(BITMAP* icon_mickey_s, BITMAP* icon_mickey);

void reset_if_resized(void);
extern void reset_bmp_back(void);

extern void closebutton (void);

#define GLYPH_TARGET_SURFACE    BITMAP

extern void gk_render_char(GLYPH_TARGET_SURFACE* const bmp,
                   GLYPH_REND* const rend,const unsigned unicode,
                   int* const pen_x,int* const pen_y);
extern void gk_render_line_utf8(GLYPH_TARGET_SURFACE* const bmp,
                                GLYPH_REND* const rend,const char* const text,
                                const int pen_x,const int pen_y);

extern void linen(long X1, long Y1, long X2, long Y2, int mode);
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);

extern BOOL Show_Alfa_Instruction(void);
extern void set_menu_level(int menu_l);
extern void XWindow_Name(int client_number);

extern int __file_exists(char *name);

extern void GoRestart(void);

extern int Expand_flex();
extern int get_window_origin_and_size_(int *x_win_orig, int *y_win_orig, int *win_width, int *win_height);
extern void set_editbox_geometry(int x, int y);
extern void set_editbox_geometry_line(int x, int y);
extern void set_editbox_geometry_set(void);
extern void set_editbox_geometry_line_set(void);
extern int Get_X11_SCREEN_SHIFT(void);
extern int Get_WIN_WINDOW_T_B(void);

extern void ini_cursors(void);
extern void ini_cursor_busy(void);

extern void SteelUS(void);
extern void TimberUS(void);
extern void SteelEU(void);
extern void SteelUK(void);

#ifndef LINUX
extern void init_file_dropped_fill_buf(void);
extern int GetShmpPtr_1(struct shmseg **shmp_);
extern void set_editbox_geometry_win(int x, int y);
extern void set_editbox_geometry_line_win(int x, int y);
extern int GoRegtestCall(int(*ptr)(int));
#endif

extern BITMAP *qmark;
extern char *qmark_p;
extern BITMAP *eymark;
extern char *eymark_p;
extern BITMAP *ermark;
extern char *ermark_p;
extern BITMAP *offmark;
extern char *offmark_p;
extern BITMAP *plotter;
extern char *plotter_p;
extern BITMAP *icon_draft;
extern char *icon_draft_p;

extern char *icon_block_p;
extern char *icon_modify_p;
extern char *icon_search_p;
extern char *icon_explode_text_to_letters_p;
extern char *icon_dim_horizontal_p;
extern char *icon_hatch_p;
extern char *icon_polygonal_aread_p;
extern char *icon_create_macro_p;
extern char *icon_drawing_settings_p;
extern char *icon_settings_p;
extern char *icon_save_p;
extern char *icon_quit_p;

extern BITMAP *icon_block;
extern BITMAP *icon_modify;
extern BITMAP *icon_search;
extern BITMAP *icon_explode_text_to_letters;
extern BITMAP *icon_dim_horizontal;
extern BITMAP *icon_hatch;
extern BITMAP *icon_polygonal_aread;
extern BITMAP *icon_create_macro;
extern BITMAP *icon_drawing_settings;
extern BITMAP *icon_settings;
extern BITMAP *icon_save;
extern BITMAP *icon_quit;

extern BITMAP *icon_line;
extern BITMAP *icon_circle;
extern BITMAP *icon_arc;
extern BITMAP *icon_text;
extern BITMAP *icon_polygon;
extern BITMAP *icon_polygon_n;
extern BITMAP *icon_disc;
extern BITMAP *icon_ellipses;
extern BITMAP *icon_polylines;
extern BITMAP *icon_solid;
extern BITMAP *icon_line_rectangle;
extern BITMAP *icon_trace;
extern BITMAP *icon_2line;
extern BITMAP *icon_3line;
extern BITMAP *icon_line_freehand;
extern BITMAP *icon_point;
extern BITMAP *icon_chart;
extern BITMAP *icon_reinforcing;

extern char *icon_line_p;
extern char *icon_circle_p;
extern char *icon_arc_p;
extern char *icon_text_p;
extern char *icon_polygon_p;
extern char *icon_polygon_n_p;
extern char *icon_disc_p;
extern char *icon_ellipses_p;
extern char *icon_polylines_p;
extern char *icon_solid_p;
extern char *icon_line_rectangle_p;
extern char *icon_trace_p;
extern char *icon_2line_p;
extern char *icon_3line_p;
extern char *icon_line_freehand_p;
extern char *icon_point_p;
extern char *icon_chart_p;
extern char *icon_reinforcing_p;

extern BITMAP *icon_move;
extern BITMAP *icon_move_copy;
extern BITMAP *icon_delete;
extern BITMAP *icon_restore;
extern BITMAP *icon_rotate;
extern BITMAP *icon_rotate_block;
extern BITMAP *icon_scale;
extern BITMAP *icon_stretch;
extern BITMAP *icon_edit_text;
extern BITMAP *icon_mirror;
extern BITMAP *icon_mirror_leave;
extern BITMAP *icon_mirror_replace;
extern BITMAP *icon_mirror_block_x;
extern BITMAP *icon_mirror_block_y;
extern BITMAP *icon_array;
extern BITMAP *icon_change;
extern BITMAP *icon_trim_segments;
extern BITMAP *icon_cutout;
extern BITMAP *icon_extend;
extern BITMAP *icon_fillet;
extern BITMAP *icon_chamfer;
extern BITMAP *icon_offset;
extern BITMAP *icon_divide;
extern BITMAP *icon_undo;

extern char *icon_move_p;
extern char *icon_move_copy_p;
extern char *icon_delete_p;
extern char *icon_restore_p;
extern char *icon_rotate_p;
extern char *icon_rotate_block_p;
extern char *icon_scale_p;
extern char *icon_stretch_p;
extern char *icon_edit_text_p;
extern char *icon_mirror_p;
extern char *icon_mirror_leave_p;
extern char *icon_mirror_replace_p;
extern char *icon_mirror_block_x_p;
extern char *icon_mirror_block_y_p;
extern char *icon_array_p;
extern char *icon_change_p;
extern char *icon_trim_segments_p;
extern char *icon_cutout_p;
extern char *icon_extend_p;
extern char *icon_fillet_p;
extern char *icon_chamfer_p;
extern char *icon_offset_p;
extern char *icon_divide_p;
extern char *icon_undo_p;

extern BITMAP *icon_pointfinder;
extern BITMAP *icon_camera;
extern BITMAP *icon_parameters;
extern BITMAP *icon_layers;
extern BITMAP *icon_measure;
extern BITMAP *icon_empty;


extern char *icon_pointfinder_p;
extern char *icon_camera_p;
extern char *icon_parameters_p;
extern char *icon_layers_p;
extern char *icon_measure_p;
extern char *icon_empty_p;


extern BITMAP *icon_copy;
extern BITMAP *icon_paste;
extern BITMAP *icon_import_block;
extern BITMAP *icon_export_block;
extern BITMAP* icon_import_image;
extern BITMAP *icon_import_png;
extern BITMAP* icon_import_jpg;
extern BITMAP *icon_edit_image;
extern BITMAP *icon_export_image;
extern BITMAP *icon_group;
extern BITMAP *icon_explode;
extern BITMAP *icon_block_list;
extern BITMAP *icon_explode_frozen;
extern BITMAP *icon_delete_dxf_blocks;
extern BITMAP *icon_correct_blocks;
extern BITMAP *icon_fixed_layers;

extern char *icon_copy_p;
extern char *icon_paste_p;
extern char *icon_import_block_p;
extern char *icon_export_block_p;
extern char* icon_import_image_p;
extern char *icon_import_png_p;
extern char* icon_import_jpg_p;
extern char *icon_edit_image_p;
extern char *icon_export_image_p;
extern char *icon_group_p;
extern char *icon_explode_p;
extern char *icon_block_list_p;
extern char *icon_explode_frozen_p;
extern char *icon_delete_dxf_blocks_p;
extern char *icon_correct_blocks_p;
extern char *icon_fixed_layers_p;


extern BITMAP *icon_area;
extern BITMAP *icon_center_of_area;
extern BITMAP *icon_first_moment_of_area;
extern BITMAP *icon_area_moment_of_inertia;
extern BITMAP *icon_centr_area_mom_of_inertia;
extern BITMAP *icon_test_of_closed_area;


extern char *icon_area_p;
extern char *icon_center_of_area_p;
extern char *icon_first_moment_of_area_p;
extern char *icon_area_moment_of_inertia_p;
extern char *icon_centr_area_mom_of_inertia_p;
extern char *icon_test_of_closed_area_p;

extern BITMAP *icon_import;
extern BITMAP *icon_new;
extern BITMAP *icon_new_window;
extern BITMAP *icon_save_as;
extern BITMAP *icon_export_cad;
extern BITMAP *icon_import_cad;
extern BITMAP *icon_export_dxf;
extern BITMAP *icon_import_dxf;
extern BITMAP *icon_export_dwg;
extern BITMAP *icon_import_dwg;
extern BITMAP *icon_edit_title_block;
extern BITMAP *icon_cmd;
extern BITMAP *icon_print;
extern BITMAP *icon_infobox;
extern BITMAP *icon_last_files;
extern BITMAP *icon_open;

extern char *icon_import_p;
extern char *icon_new_p;
extern char *icon_new_window_p;
extern char *icon_save_as_p;
extern char *icon_export_cad_p;
extern char *icon_import_cad_p;
extern char *icon_export_dxf_p;
extern char *icon_import_dxf_p;
extern char *icon_export_dwg_p;
extern char *icon_import_dwg_p;
extern char *icon_edit_title_block_p;
extern char *icon_cmd_p;
extern char *icon_print_p;
extern char *icon_infobox_p;
extern char *icon_last_files_p;
extern char *icon_open_p;

extern BITMAP *icon_change_texts;
extern BITMAP *icon_auto_numbering;
extern BITMAP *icon_record;
extern BITMAP *icon_play;
extern BITMAP *icon_load;
extern BITMAP *icon_save_as;
extern BITMAP *icon_format;
extern BITMAP *icon_dscale;
extern BITMAP *icon_accuracy;
extern BITMAP *icon_units;
extern BITMAP *icon_coordinates;
extern BITMAP *icon_local;
extern BITMAP *icon_pointer;
extern BITMAP *icon_edit_ini;
extern BITMAP *icon_cartesian;
extern BITMAP *icon_geodetic;
extern BITMAP *icon_point_origin;
extern BITMAP *icon_axis_angle;
extern BITMAP *icon_rotation_angle;
extern BITMAP *icon_colorwheel;
extern BITMAP *icon_panoramic;
extern BITMAP *icon_font_type;


extern char *icon_change_texts_p;
extern char *icon_auto_numbering_p;
extern char *icon_record_p;
extern char *icon_play_p;
extern char *icon_load_p;
extern char *icon_save_as_p;
extern char *icon_format_p;
extern char *icon_dscale_p;
extern char *icon_accuracy_p;
extern char *icon_units_p;
extern char *icon_coordinates_p;
extern char *icon_local_p;
extern char *icon_pointer_p;
extern char *icon_edit_ini_p;
extern char *icon_cartesian_p;
extern char *icon_geodetic_p;
extern char *icon_point_origin_p;
extern char *icon_axis_angle_p;
extern char *icon_rotation_angle_p;
extern char *icon_colorwheel_p;
extern char *icon_panoramic_p;
extern char *icon_font_type_p;

extern BITMAP *icon_yes;
extern BITMAP *icon_no;
extern BITMAP *icon_dx_dy;
extern BITMAP *icon_calibrate;
extern BITMAP *icon_A_rectangular;
extern BITMAP *icon_A_circular;
extern BITMAP *icon_A_circular_rot;
extern BITMAP *icon_A_circular_n;
extern BITMAP *icon_A_circular_rot_n;
extern BITMAP *icon_A_circular_angle;
extern BITMAP *icon_A_circular_rot_angle;

extern char *icon_yes_p;
extern char *icon_no_p;
extern char *icon_dx_dy_p;
extern char *icon_calibrate_p;
extern char *icon_A_rectangular_p;
extern char *icon_A_circular_p;
extern char *icon_A_circular_rot_p;
extern char *icon_A_circular_n_p;
extern char *icon_A_circular_rot_n_p;
extern char *icon_A_circular_angle_p;
extern char *icon_A_circular_rot_angle_p;

extern BITMAP *icon_pointmode;
extern BITMAP *icon_endpoint;
extern BITMAP *icon_nearest_end;
extern BITMAP *icon_middlepoint;
extern BITMAP *icon_intersection;
extern BITMAP *icon_center;
extern BITMAP *icon_perpendicular;
extern BITMAP *icon_tangential;
extern BITMAP *icon_nearest;
extern BITMAP *icon_onpoint;
extern BITMAP *icon_move2xy;
extern BITMAP *icon_movedxdy;
extern BITMAP *icon_move_polar;
extern BITMAP *icon_nopointfinder;


extern char *icon_pointmode_p;
extern char *icon_endpoint_p;
extern char *icon_nearest_end_p;
extern char *icon_middlepoint_p;
extern char *icon_intersection_p;
extern char *icon_center_p;
extern char *icon_perpendicular_p;
extern char *icon_tangential_p;
extern char *icon_nearest_p;
extern char *icon_onpoint_p;
extern char *icon_move2xy_p;
extern char *icon_movedxdy_p;
extern char *icon_move_polar_p;
extern char *icon_nopointfinder_p;


extern BITMAP *icon_calculator;
extern BITMAP *icon_point_measure;
extern BITMAP *icon_distance_point_to_point;
extern BITMAP *icon_angle_line_to_line;
extern BITMAP *icon_plusminus;
extern BITMAP *icon_property_list;
extern BITMAP *icon_noplusminus;
extern BITMAP *icon_addfavourite;
extern BITMAP *icon_findfavourite;
extern BITMAP *icon_refresh;
extern BITMAP *icon_refresh_d;
extern BITMAP *icon_zoom_all;
extern BITMAP *icon_zoom_center;
extern BITMAP *icon_auto_panorama;
extern BITMAP *icon_zoom_1;
extern BITMAP *icon_zoom_2;
extern BITMAP *icon_zoom_in;
extern BITMAP *icon_zoom_out;
extern BITMAP *icon_zoom_x;
extern BITMAP *icon_zoom_window;
extern BITMAP *icon_zoom_drawing;
extern BITMAP *icon_zoom_previous;


extern char *icon_calculator_p;
extern char *icon_point_measure_p;
extern char *icon_distance_point_to_point_p;
extern char *icon_angle_line_to_line_p;
extern char *icon_plusminus_p;
extern char *icon_property_list_p;
extern char *icon_noplusminus_p;
extern char *icon_addfavourite_p;
extern char *icon_findfavourite_p;
extern char *icon_refresh_p;
extern char *icon_refresh_d_p;
extern char *icon_zoom_all_p;
extern char *icon_zoom_center_p;
extern char *icon_auto_panorama_p;
extern char *icon_zoom_1_p;
extern char *icon_zoom_2_p;
extern char *icon_zoom_in_p;
extern char *icon_zoom_out_p;
extern char *icon_zoom_x_p;
extern char *icon_zoom_window_p;
extern char *icon_zoom_drawing_p;
extern char *icon_zoom_previous_p;

extern BITMAP *icon_pointer_step;
extern BITMAP *icon_grid_density;
extern BITMAP *icon_grid_points;
extern BITMAP *icon_restr_ortho;
extern BITMAP *icon_color;
extern BITMAP *icon_line_thickness;
extern BITMAP *icon_line_type;
extern BITMAP *icon_point_visibility;
extern BITMAP *icon_point_size;
extern BITMAP *icon_get_color;
extern BITMAP *icon_get_type;
extern BITMAP *icon_get_layer;
extern BITMAP *icon_floating_text;
extern BITMAP *icon_floating_block;
extern BITMAP *icon_dim_scale;
extern BITMAP *icon_stretch_in_pin;
extern BITMAP *icon_break_in_pin;
extern BITMAP *icon_mask;
extern BITMAP *icon_hidden_text_vis;
extern BITMAP *icon_image_points;
extern BITMAP *icon_highlight_layer;
extern BITMAP *icon_dbuff_image;
extern BITMAP *icon_text_most_readable;

extern BITMAP *icon_line_color;
extern BITMAP *icon_text_color;
extern BITMAP *icon_text_height;
extern BITMAP *icon_width;
extern BITMAP *icon_italic;
extern BITMAP *icon_bold;
extern BITMAP *icon_font_type;
extern BITMAP *icon_accuracy;
extern BITMAP *icon_terminators;
extern BITMAP *icon_line_offset;
extern BITMAP *icon_text_direction;
extern BITMAP *icon_extension_lines;

extern BITMAP *icon_dim_arrowheads;
extern BITMAP *icon_dim_ticks;
extern BITMAP *icon_dim_dots;

extern BITMAP *icon_text_parallel;
extern BITMAP *icon_text_horizontal;
extern BITMAP *icon_fixed_length;
extern BITMAP *icon_align_to_object;

extern BITMAP *icon_dim_direction;
extern BITMAP *icon_dim_new;
extern BITMAP *icon_dim_remove;
extern BITMAP *icon_dim_edit;
extern BITMAP *icon_dim_settings;
extern BITMAP *icon_dim_arc_RL;
extern BITMAP *icon_dim_circle_DR;

extern BITMAP *icon_dim_horizontal;
extern BITMAP *icon_dim_vertical;
extern BITMAP *icon_dim_aligned;
extern BITMAP *icon_dim_linear;
extern BITMAP *icon_dim_linearX;
extern BITMAP *icon_dim_angular;

extern BITMAP *icon_dim_arc_radial;
extern BITMAP *icon_dim_arc_length;
extern BITMAP *icon_dim_diametric;
extern BITMAP *icon_dim_radial;
extern BITMAP *icon_dim_radial_y;
extern BITMAP *icon_dim_radial_y_dim;
extern BITMAP *icon_dim_radial_y_point;
extern BITMAP *icon_dim_radial_y_tangential;

extern char *icon_pointer_step_p;
extern char *icon_grid_density_p;
extern char *icon_grid_points_p;
extern char *icon_restr_ortho_p;
extern char *icon_color_p;
extern char *icon_line_thickness_p;
extern char *icon_line_type_p;
extern char *icon_point_visibility_p;
extern char *icon_point_size_p;
extern char *icon_get_color_p;
extern char *icon_get_type_p;
extern char *icon_get_layer_p;
extern char *icon_floating_text_p;
extern char *icon_floating_block_p;
extern char *icon_dim_scale_p;
extern char *icon_stretch_in_pin_p;
extern char *icon_break_in_pin_p;
extern char *icon_mask_p;
extern char *icon_hidden_text_vis_p;
extern char *icon_image_points_p;
extern char *icon_highlight_layer_p;
extern char *icon_dbuff_image_p;
extern char *icon_text_most_readable_p;

extern char *icon_line_color_p;
extern char *icon_text_color_p;
extern char *icon_text_height_p;
extern char *icon_width_p;
extern char *icon_italic_p;
extern char *icon_bold_p;
extern char *icon_font_type_p;
extern char *icon_accuracy_p;
extern char *icon_terminators_p;
extern char *icon_line_offset_p;
extern char *icon_text_direction_p;
extern char *icon_extension_lines_p;

extern char *icon_dim_arrowheads_p;
extern char *icon_dim_ticks_p;
extern char *icon_dim_dots_p;

extern char *icon_text_parallel_p;
extern char *icon_text_horizontal_p;
extern char *icon_fixed_length_p;
extern char *icon_align_to_object_p;

extern char *icon_dim_direction_p;
extern char *icon_dim_new_p;
extern char *icon_dim_remove_p;
extern char *icon_dim_edit_p;
extern char *icon_dim_settings_p;
extern char *icon_dim_arc_RL_p;
extern char *icon_dim_circle_DR_p;

extern char *icon_dim_horizontal_p;
extern char *icon_dim_vertical_p;
extern char *icon_dim_aligned_p;
extern char *icon_dim_linear_p;
extern char *icon_dim_linearX_p;
extern char *icon_dim_angular_p;

extern char *icon_dim_arc_radial_p;
extern char *icon_dim_arc_length_p;
extern char *icon_dim_diametric_p;
extern char *icon_dim_radial_p;
extern char *icon_dim_radial_y_p;
extern char *icon_dim_radial_y_dim_p;
extern char *icon_dim_radial_y_point_p;
extern char *icon_dim_radial_y_tangential_p;

extern BITMAP *icon_circle_center_radius;
extern BITMAP *icon_circle_center_diameter;
extern BITMAP *icon_circle_2_points;
extern BITMAP *icon_circle_3_points;
extern BITMAP *icon_line_close;
extern BITMAP *icon_line_remove;
extern BITMAP *icon_trim_value;
extern BITMAP *icon_line_continue;
extern BITMAP *icon_arc_3_points;
extern BITMAP *icon_arc_beginning_center_end;
extern BITMAP *icon_arc_beginning_center_angle;
extern BITMAP *icon_arc_beginning_center_chord;
extern BITMAP *icon_arc_beginning_end_radius;
extern BITMAP *icon_arc_beginning_end_angle;
extern BITMAP *icon_arc_beginning_end_direction;
extern BITMAP *icon_arc_continuation;
extern BITMAP *icon_text_angle;
extern BITMAP *icon_text_width;
extern BITMAP *icon_text_type;
extern BITMAP *icon_hidden_text;
extern BITMAP *icon_text_align;
extern BITMAP *icon_text_interval;
extern BITMAP *icon_line_polygon_cor_cor;
extern BITMAP *icon_line_polygon_cen_tan;
extern BITMAP *icon_line_polygon_cen_cor;
extern BITMAP *icon_ellipse_approximation;
extern BITMAP *icon_ellipse_axis_center;
extern BITMAP *icon_ellipse_axis_start;
extern BITMAP *icon_ellipse_center;
extern BITMAP *icon_solid_quad_triang;
extern BITMAP *icon_solid_triangular1;
extern BITMAP *icon_solid_fill_line_width;
extern BITMAP *icon_solid_quadrangular;
extern BITMAP *icon_solid_triangular;
extern BITMAP *icon_arc_close;
extern BITMAP *icon_arc_remove;
extern BITMAP *icon_trace_width;
extern BITMAP *icon_trace_axis_offset;
extern BITMAP *icon_trace_polyline;
extern BITMAP *icon_trace_fill_line_width;
extern BITMAP *icon_trace_remove;
extern BITMAP *icon_chart_step_dx;
extern BITMAP *icon_ftfunct;
extern BITMAP *icon_last_block;
extern BITMAP *icon_select_all;
extern BITMAP *icon_all_layers;
extern BITMAP *icon_select_cross_window;
extern BITMAP *icon_add_remove;
extern BITMAP *icon_gear1;
extern BITMAP *icon_text_align_baselineleft;
extern BITMAP *icon_text_align_baselineright;
extern BITMAP *icon_text_align_baselinecenter;
extern BITMAP *icon_text_align_middlecenter;
extern BITMAP *icon_circle_tangential;


extern char *icon_circle_center_radius_p;
extern char *icon_circle_center_diameter_p;
extern char *icon_circle_2_points_p;
extern char *icon_circle_3_points_p;
extern char *icon_line_close_p;
extern char *icon_line_remove_p;
extern char *icon_trim_value_p;
extern char *icon_line_continue_p;
extern char *icon_arc_3_points_p;
extern char *icon_arc_beginning_center_end_p;
extern char *icon_arc_beginning_center_angle_p;
extern char *icon_arc_beginning_center_chord_p;
extern char *icon_arc_beginning_end_radius_p;
extern char *icon_arc_beginning_end_angle_p;
extern char *icon_arc_beginning_end_direction_p;
extern char *icon_arc_continuation_p;
extern char *icon_text_angle_p;
extern char *icon_text_width_p;
extern char *icon_text_type_p;
extern char *icon_hidden_text_p;
extern char *icon_text_align_p;
extern char *icon_text_interval_p;
extern char *icon_line_polygon_cor_cor_p;
extern char *icon_line_polygon_cen_tan_p;
extern char *icon_line_polygon_cen_cor_p;
extern char *icon_ellipse_approximation_p;
extern char *icon_ellipse_axis_center_p;
extern char *icon_ellipse_axis_start_p;
extern char *icon_ellipse_center_p;
extern char *icon_solid_quad_triang_p;
extern char *icon_solid_triangular1_p;
extern char *icon_solid_fill_line_width_p;
extern char *icon_solid_quadrangular_p;
extern char *icon_solid_triangular_p;
extern char *icon_arc_close_p;
extern char *icon_arc_remove_p;
extern char *icon_trace_width_p;
extern char *icon_trace_axis_offset_p;
extern char *icon_trace_polyline_p;
extern char *icon_trace_fill_line_width_p;
extern char *icon_trace_remove_p;
extern char *icon_chart_step_dx_p;
extern char *icon_ftfunct_p;
extern char *icon_last_block_p;
extern char *icon_select_all_p;
extern char *icon_all_layers_p;
extern char *icon_select_cross_window_p;
extern char *icon_add_remove_p;
extern char *icon_gear1_p;
extern char *icon_text_align_baselineleft_p;
extern char *icon_text_align_baselineright_p;
extern char *icon_text_align_baselinecenter_p;
extern char *icon_text_align_middlecenter_p;
extern char *icon_circle_tangential_p;


extern BITMAP *icon_axis_length;
extern BITMAP *icon_dash_dotted;
extern BITMAP *icon_dashed;
extern BITMAP *icon_return_all;
extern BITMAP *icon_change_mode;
extern BITMAP *icon_change_property;
extern BITMAP *icon_all_texts;
extern BITMAP *icon_select_window;
extern BITMAP *icon_select_cross;

extern char *icon_axis_length_p;
extern char *icon_dash_dotted_p;
extern char *icon_dashed_p;
extern char *icon_return_all_p;
extern char *icon_change_mode_p;
extern char *icon_change_property_p;
extern char *icon_all_texts_p;
extern char *icon_select_window_p;
extern char *icon_select_cross_p;

extern BITMAP *icon_solid_filled;
extern BITMAP *icon_solid_very_thin;
extern BITMAP *icon_solid_thin;
extern BITMAP *icon_solid_thick;
extern BITMAP *icon_solid_very_thick;
extern BITMAP *icon_solid_extra_thick;
extern BITMAP *icon_text_angle_0;
extern BITMAP *icon_text_angle_90;
extern BITMAP *icon_text_parallel_to_object;
extern BITMAP *icon_text_perpendicular;
extern BITMAP *icon_text_angle_to_object;
extern BITMAP *icon_text_angle_x;

extern char *icon_solid_filled_p;
extern char *icon_solid_very_thin_p;
extern char *icon_solid_thin_p;
extern char *icon_solid_thick_p;
extern char *icon_solid_very_thick_p;
extern char *icon_solid_extra_thick_p;
extern char *icon_text_angle_0_p;
extern char *icon_text_angle_90_p;
extern char *icon_text_parallel_to_object_p;
extern char *icon_text_perpendicular_p;
extern char *icon_text_angle_to_object_p;
extern char *icon_text_angle_x_p;

extern BITMAP *icon_trace_filled;
extern BITMAP *icon_trace_very_thin;
extern BITMAP *icon_trace_thin;
extern BITMAP *icon_trace_thick;
extern BITMAP *icon_trace_very_thick;
extern BITMAP *icon_trace_extra_thick;
extern BITMAP *icon_point_simple;
extern BITMAP *icon_point_base;
extern BITMAP *icon_point_pin;

extern char *icon_trace_filled_p;
extern char *icon_trace_very_thin_p;
extern char *icon_trace_thin_p;
extern char *icon_trace_thick_p;
extern char *icon_trace_very_thick_p;
extern char *icon_trace_extra_thick_p;
extern char *icon_point_simple_p;
extern char *icon_point_base_p;
extern char *icon_point_pin_p;

extern BITMAP *icon_RAM_for_drawing;
extern BITMAP *icon_RAM_virtual_image;
extern BITMAP *icon_RAM_data_size;
extern BITMAP *icon_RAM_printing_buffer;
extern BITMAP *icon_RAM_macro_buffer;
extern BITMAP *icon_RAM_image_buffer;
extern BITMAP *icon_current_folder;
extern BITMAP *icon_desktop_size;

extern char *icon_RAM_for_drawing_p;
extern char *icon_RAM_virtual_image_p;
extern char *icon_RAM_data_size_p;
extern char *icon_RAM_printing_buffer_p;
extern char *icon_RAM_macro_buffer_p;
extern char *icon_RAM_image_buffer_p;
extern char *icon_current_folder_p;
extern char *icon_desktop_size_p;

extern BITMAP *icon_layers_info;
extern BITMAP *icon_x1;
extern BITMAP *icon_y1;
extern BITMAP *icon_x2;
extern BITMAP *icon_y2;
extern BITMAP *icon_x3;
extern BITMAP *icon_y3;
extern BITMAP *icon_x4;
extern BITMAP *icon_y4;
extern BITMAP *icon_angle1;
extern BITMAP *icon_angle2;
extern BITMAP *icon_dim_dx;
extern BITMAP *icon_dim_dy;
extern BITMAP *icon_inner_block_name;
extern BITMAP *icon_outer_block_name;

extern char *icon_layers_info_p;
extern char *icon_x1_p;
extern char *icon_y1_p;
extern char *icon_x2_p;
extern char *icon_y2_p;
extern char *icon_x3_p;
extern char *icon_y3_p;
extern char *icon_x4_p;
extern char *icon_y4_p;
extern char *icon_angle1_p;
extern char *icon_angle2_p;
extern char *icon_dim_dx_p;
extern char *icon_dim_dy_p;
extern char *icon_inner_block_name_p;
extern char *icon_outer_block_name_p;

extern BITMAP *icon_qr_code1;
extern BITMAP *icon_edit;
extern BITMAP *icon_grey;
extern BITMAP *icon_blackwhite;
extern BITMAP *icon_plotter_s;
extern BITMAP *icon_visible;

extern char *icon_qr_code1_p;
extern char *icon_edit_p;
extern char *icon_grey_p;
extern char *icon_blackwhite_p;
extern char *icon_plotter_s_p;
extern char *icon_visible_p;

extern BITMAP *icon_visible_d;
extern BITMAP *icon_edit_d;
extern BITMAP *icon_pointfinder_d;
extern BITMAP *icon_color_d;
extern BITMAP *icon_line_thickness_d;
extern BITMAP *icon_line_type_d;
extern BITMAP *icon_grey_d;
extern BITMAP *icon_blackwhite_d;

extern char *icon_visible_d_p;
extern char *icon_edit_d_p;
extern char *icon_pointfinder_d_p;
extern char *icon_color_d_p;
extern char *icon_line_thickness_d_p;
extern char *icon_line_type_d_p;
extern char *icon_grey_d_p;
extern char *icon_blackwhite_d_p;

extern BITMAP *icon_visible_db;
extern BITMAP *icon_edit_db;
extern BITMAP *icon_pointfinder_db;
extern BITMAP *icon_grey_db;
extern BITMAP *icon_blackwhite_db;

extern char *icon_visible_db_p;
extern char *icon_edit_db_p;
extern char *icon_pointfinder_db_p;
extern char *icon_grey_db_p;
extern char *icon_blackwhite_db_p;

extern BITMAP *icon_plotter_64;
extern BITMAP *icon_saveimage_64;

extern char *icon_plotter_64_p;
extern char *icon_saveimage_64_p;

extern BITMAP *icon_rotation_d48;
extern BITMAP *icon_inversion_d48;
extern BITMAP *icon_reflection_d48;
extern BITMAP *icon_grey_d48;
extern BITMAP *icon_blackwhite_d48;
extern BITMAP *icon_grey_image_d48;

extern char *icon_rotation_d48_p;
extern char *icon_inversion_d48_p;
extern char *icon_reflection_d48_p;
extern char *icon_grey_d48_p;
extern char *icon_blackwhite_d48_p;
extern char *icon_grey_image_d48_p;

extern BITMAP *icon_page_d48;
extern BITMAP *icon_scale_d48;
extern BITMAP *icon_window_d48;
extern BITMAP *icon_sheet_d48;
extern BITMAP *icon_tools;
extern BITMAP *icon_features;
extern BITMAP *icon_ctrl;
extern BITMAP *icon_alt;
extern BITMAP *icon_shift;
extern BITMAP *icon_home;
extern BITMAP *icon_end;
extern BITMAP *icon_esc;
extern BITMAP *icon_ins;
extern BITMAP *icon_pgdn;
extern BITMAP *icon_pgup;
extern BITMAP *icon_spc;
extern BITMAP* icon_del;
extern BITMAP* icon_ent;
extern BITMAP* icon_er;
extern BITMAP* icon_k_right;
extern BITMAP* icon_k_left;
extern BITMAP* icon_k_up;
extern BITMAP* icon_k_down;
extern BITMAP *icon_tab;
extern BITMAP *icon_0;
extern BITMAP *icon_1;
extern BITMAP *icon_2;
extern BITMAP *icon_3;
extern BITMAP *icon_4;
extern BITMAP *icon_5;
extern BITMAP *icon_6;
extern BITMAP *icon_7;
extern BITMAP *icon_8;
extern BITMAP *icon_9;
extern BITMAP *icon_plus;
extern BITMAP *icon_minus;
extern BITMAP *icon_f1;
extern BITMAP *icon_f2;
extern BITMAP *icon_f3;
extern BITMAP *icon_f4;
extern BITMAP *icon_f5;
extern BITMAP *icon_f6;
extern BITMAP *icon_f7;
extern BITMAP *icon_f8;
extern BITMAP *icon_f9;
extern BITMAP *icon_f10;
extern BITMAP *icon_f11;
extern BITMAP *icon_f12;
extern BITMAP *icon_mouse3;
extern BITMAP *icon_mouseR;
extern BITMAP *icon_measure_tape;
extern BITMAP *icon_spa;
extern BITMAP *icon_ce;
extern BITMAP *icon_look_down;

extern char *icon_page_d48_p;
extern char *icon_scale_d48_p;
extern char *icon_window_d48_p;
extern char *icon_sheet_d48_p;
extern char *icon_tools_p;
extern char *icon_features_p;
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
extern char* icon_ent_p;
extern char* icon_er_p;
extern char* icon_k_right_p;
extern char* icon_k_left_p;
extern char* icon_k_up_p;
extern char* icon_k_down_p;
extern char *icon_tab_p;
extern char *icon_0_p;
extern char *icon_1_p;
extern char *icon_2_p;
extern char *icon_3_p;
extern char *icon_4_p;
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
extern char *icon_mouse3_p;
extern char *icon_mouseR_p;
extern char *icon_measure_tape_p;
extern char *icon_spa_p;
extern char *icon_ce_p;
extern char *icon_look_down_p;

extern BITMAP *icon_plotter_32;
extern char *icon_plotter_32_p;

extern BITMAP *icon_cartridge_d48;
extern BITMAP *icon_very_thin_d;
extern BITMAP *icon_thin_d;
extern BITMAP *icon_thick_d;
extern BITMAP *icon_very_thick_d;
extern BITMAP *icon_extra_thick_d;
extern BITMAP *icon_color_d48;

extern char *icon_cartridge_d48_p;
extern char *icon_very_thin_d_p;
extern char *icon_thin_d_p;
extern char *icon_thick_d_p;
extern char *icon_very_thick_d_p;
extern char *icon_extra_thick_d_p;
extern char *icon_color_d48_p;

extern BITMAP *icon_scale_d;
extern BITMAP *icon_page_d;
extern BITMAP *icon_page_width_d;
extern BITMAP *icon_page_height_d;
extern BITMAP *icon_page_overlap_d;

extern char *icon_scale_d_p;
extern char *icon_page_d_p;
extern char *icon_page_width_d_p;
extern char *icon_page_height_d_p;
extern char *icon_page_overlap_d_p;

extern BITMAP *icon_color256_d48;
extern char *icon_color256_d48_p;

extern BITMAP *icon_multiline_text;
extern BITMAP *icon_underlined;
extern BITMAP *icon_line_spacing;
extern char *icon_multiline_text_p;
extern char *icon_underlined_p;
extern char *icon_line_spacing_p;

extern BITMAP *icon_bold_d;
extern BITMAP *icon_font_type_d;
extern BITMAP *icon_hidden_text_d;
extern BITMAP *icon_italic_d;
extern BITMAP *icon_layers_d;
extern BITMAP *icon_text_align_d;
extern BITMAP *icon_text_height_d;
extern BITMAP *icon_text_type_d;
extern BITMAP *icon_width_d;
extern BITMAP *icon_underlined_d;
//extern BITMAP *icon_yes_d;
//extern BITMAP *icon_no_d;
extern BITMAP *icon_no_d_12;
extern BITMAP *icon_mark_d_12;

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
//extern char *icon_yes_d_p;
//extern char *icon_no_d_p;
extern char *icon_no_d_12_p;
extern char *icon_mark_d_12_p;

extern BITMAP *icon_hor_dpi_sphere_blue_d;
extern BITMAP *icon_ver_dpi_sphere_blue_d;
extern BITMAP *icon_hor_size_image_d;
extern BITMAP *icon_ver_size_image_d;
extern BITMAP *icon_hor_scale_image_d;
extern BITMAP *icon_ver_scale_image_d;
extern BITMAP *icon_background_image_d;
extern BITMAP* icon_background_image_d48;
extern BITMAP *icon_rotate_angle_image_d;

extern char *icon_hor_dpi_sphere_blue_d_p;
extern char *icon_ver_dpi_sphere_blue_d_p;
extern char *icon_hor_size_image_d_p;
extern char *icon_ver_size_image_d_p;
extern char *icon_hor_scale_image_d_p;
extern char *icon_ver_scale_image_d_p;
extern char *icon_background_image_d_p;
extern char* icon_background_image_d48_p;
extern char *icon_rotate_angle_image_d_p;

extern BITMAP *icon_top_margin_d;
extern BITMAP *icon_left_margin_d;
extern BITMAP *icon_bottom_margin_d;
extern BITMAP *icon_right_margin_d;
extern BITMAP *icon_actual_size_d48;

extern char *icon_top_margin_d_p;
extern char *icon_left_margin_d_p;
extern char *icon_bottom_margin_d_p;
extern char *icon_right_margin_d_p;
extern char *icon_actual_size_d48_p;

extern BITMAP *icon_pattern;
extern BITMAP *icon_pattern_angle;
extern BITMAP *icon_pattern_scale;
extern BITMAP *icon_edit_point_origin;
extern BITMAP *icon_set_point_origin;
extern BITMAP *icon_pattern_line_distance;

extern char *icon_pattern_p;
extern char *icon_pattern_angle_p;
extern char *icon_pattern_scale_p;
extern char *icon_edit_point_origin_p;
extern char *icon_set_point_origin_p;
extern char *icon_pattern_line_distance_p;

extern BITMAP *icon_anglex;
extern BITMAP *icon_angle45;
extern BITMAP *icon_angle90;
extern BITMAP *icon_angle135;
extern BITMAP *icon_angle180;
extern BITMAP *icon_angle225;
extern BITMAP *icon_angle270;
extern BITMAP *icon_angle315;

extern char *icon_anglex_p;
extern char *icon_angle45_p;
extern char *icon_angle90_p;
extern char *icon_angle135_p;
extern char *icon_angle180_p;
extern char *icon_angle225_p;
extern char *icon_angle270_p;
extern char *icon_angle315_p;

extern BITMAP *icon_divide_count;
extern BITMAP *icon_divide_counter;
extern BITMAP *icon_divide_measure;
extern BITMAP *icon_divide_segment_length;
extern BITMAP *icon_divide_counter_max;
extern BITMAP *icon_divide_polyline_segment;
extern BITMAP *icon_divide_polyline;
extern BITMAP *icon_divide_segment;

extern char *icon_divide_count_p;
extern char *icon_divide_counter_p;
extern char *icon_divide_measure_p;
extern char *icon_divide_segment_length_p;
extern char *icon_divide_counter_max_p;
extern char *icon_divide_polyline_segment_p;
extern char *icon_divide_polyline_p;
extern char *icon_divide_segment_p;

extern BITMAP *icon_align_block;
extern BITMAP *icon_offset_point;
extern BITMAP *icon_offset_distance;
extern BITMAP *icon_set_offset_distance;
extern BITMAP *icon_return;

extern char *icon_align_block_p;
extern char *icon_offset_point_p;
extern char *icon_offset_distance_p;
extern char *icon_set_offset_distance_p;
extern char *icon_return_p;

extern BITMAP *icon_open_file_folder;
extern char *icon_open_file_folder_p;

extern BITMAP *icon_arrow_up_d48;
extern BITMAP* icon_arrow_up_end_d48;
extern BITMAP* icon_arrow_down_end_d48;
extern BITMAP *icon_arrow_down_d48;
extern BITMAP *icon_folder_d48;
extern BITMAP *icon_folder0_d48;
extern BITMAP *icon_smaller_d48;
extern BITMAP *icon_bigger_d48;

extern char *icon_arrow_up_d48_p;
extern char* icon_arrow_up_end_d48_p;
extern char* icon_arrow_down_end_d48_p;
extern char *icon_arrow_down_d48_p;
extern char *icon_folder_d48_p;
extern char *icon_folder0_d48_p;
extern char *icon_smaller_d48_p;
extern char *icon_bigger_d48_p;

extern BITMAP *icon_expand_hor;
extern BITMAP *icon_expand_ver;
extern BITMAP *icon_expand_diag;
extern BITMAP *icon_expand_flex;
extern BITMAP *icon_expand_last;
extern BITMAP *icon_expand_win;
extern BITMAP *icon_arrow_up_d;
extern BITMAP *icon_arrow_down_d;
extern BITMAP *icon_pgup_d;
extern BITMAP *icon_pgdn_d;
extern BITMAP *icon_arrow_up_left_d;
extern BITMAP *icon_arrow_down_left_d;

extern char *icon_expand_hor_p;
extern char *icon_expand_ver_p;
extern char *icon_expand_diag_p;
extern char *icon_expand_flex_p;
extern char *icon_expand_last_p;
extern char *icon_expand_win_p;
extern char *icon_arrow_up_d_p;
extern char *icon_arrow_down_d_p;
extern char *icon_pgup_d_p;
extern char *icon_pgdn_d_p;
extern char *icon_arrow_up_left_d_p;
extern char *icon_arrow_down_left_d_p;

extern BITMAP *icon_plotter_start_d_64;
extern BITMAP *icon_printer_start_d_64;
extern BITMAP *icon_printer_d_48;
extern BITMAP *icon_escape_d_64;
extern BITMAP *icon_save_d;
extern BITMAP *icon_load_d;
extern BITMAP *icon_return_d_24;
extern BITMAP *icon_frame;
extern BITMAP *icon_zones_around;
extern BITMAP *icon_zones_prefix;
extern BITMAP *icon_zones_first_number;
extern BITMAP *icon_zones_reversed;
extern BITMAP *icon_zone_height;
extern BITMAP *icon_zone_width;
extern BITMAP *icon_margin_width;
extern BITMAP *icon_frame_line;
extern BITMAP *icon_zones_line;
extern BITMAP *icon_frame_color;
extern BITMAP *icon_zones_color;
extern BITMAP *icon_style2_black;
extern BITMAP *icon_zones_top;
extern BITMAP *icon_zones_bottom;
extern BITMAP *icon_zones_top_bottom;
extern BITMAP *icon_frame_offset;

extern char *icon_plotter_start_d_64_p;
extern char *icon_printer_start_d_64_p;
extern char *icon_printer_d_48_p;
extern char *icon_escape_d_64_p;
extern char *icon_save_d_p;
extern char *icon_load_d_p;
extern char *icon_return_d_24_p;
extern char *icon_frame_p;
extern char *icon_zones_around_p;
extern char *icon_zones_prefix_p;
extern char *icon_zones_first_number_p;
extern char *icon_zones_reversed_p;
extern char *icon_zone_height_p;
extern char *icon_zone_width_p;
extern char *icon_margin_width_p;
extern char *icon_frame_line_p;
extern char *icon_zones_line_p;
extern char *icon_frame_color_p;
extern char *icon_zones_color_p;
extern char *icon_style2_black_p;
extern char *icon_zones_top_p;
extern char *icon_zones_bottom_p;
extern char *icon_zones_top_bottom_p;
extern char *icon_frame_offset_p;

extern BITMAP *icon_ok_d_64;
extern BITMAP *icon_new_layer_d_64;

extern char *icon_ok_d_64_p;
extern char *icon_new_layer_d_64_p;

extern BITMAP *icon_hor_extents_d;
extern BITMAP *icon_ver_extents_d;
extern BITMAP *icon_hor_limits_d;
extern BITMAP *icon_ver_limits_d;
extern BITMAP *icon_units_d;
extern BITMAP *icon_paper_size_d_48;
extern BITMAP *icon_paper_size_d;
extern BITMAP *icon_paper_dim_d_48;

extern char *icon_hor_extents_d_p;
extern char *icon_ver_extents_d_p;
extern char *icon_hor_limits_d_p;
extern char *icon_ver_limits_d_p;
extern char *icon_units_d_p;
extern char *icon_paper_size_d_48_p;
extern char *icon_paper_size_d_p;
extern char *icon_paper_dim_d_48_p;

extern BITMAP *icon_bspline;
extern BITMAP *icon_bspline34;
extern BITMAP *icon_bspline3p;
extern BITMAP *icon_bspline4;
extern BITMAP *icon_bspline3;

extern char *icon_bspline_p;
extern char *icon_bspline34_p;
extern char *icon_bspline3p_p;
extern char *icon_bspline4_p;
extern char *icon_bspline3_p;

extern BITMAP *icon_yes_d_48;
extern BITMAP *icon_no_d_48;
extern BITMAP *icon_escape_d_48;
extern BITMAP *icon_escape_d;

extern char *icon_yes_d_48_p;
extern char *icon_no_d_48_p;
extern char *icon_escape_d_48_p;
extern char *icon_escape_d_p;

extern BITMAP *icon_tartan;
extern BITMAP *icon_all_windows;

extern char *icon_tartan_p;
extern char *icon_all_windows_p;

extern BITMAP *icon_junction;
extern BITMAP *icon_save_window;

extern char *icon_junction_p;
extern char *icon_save_window_p;

extern BITMAP *icon_freehand_segment_length;
extern BITMAP *icon_remove_last_freehand;

extern char *icon_freehand_segment_length_p;
extern char *icon_remove_last_freehand_p;

extern BITMAP *icon_new_from_template;
extern char *icon_new_from_template_p;

extern BITMAP *icon_idea;
extern char *icon_idea_p;

extern BITMAP *icon_mirror_block;
extern char *icon_mirror_block_p;

extern BITMAP *icon_tree;
extern BITMAP *icon_tree_enter;
extern BITMAP *icon_tree_return;

extern char *icon_tree_p;
extern char *icon_tree_enter_p;
extern char *icon_tree_return_p;

extern BITMAP *icon_trans30;
extern BITMAP *icon_trans40;
extern BITMAP *icon_trans50;
extern BITMAP *icon_trans60;
extern BITMAP *icon_trans70;
extern BITMAP *icon_trans80;
extern BITMAP *icon_trans90;
extern BITMAP *icon_trans100;

extern char *icon_trans30_p;
extern char *icon_trans40_p;
extern char *icon_trans50_p;
extern char *icon_trans60_p;
extern char *icon_trans70_p;
extern char *icon_trans80_p;
extern char *icon_trans90_p;
extern char *icon_trans100_p;

extern BITMAP* icon_trans05;
extern BITMAP* icon_trans10;
extern BITMAP* icon_trans15;
extern BITMAP* icon_trans20;
extern BITMAP* icon_trans25;
extern BITMAP* icon_trans35;
extern BITMAP* icon_trans45;
extern BITMAP* icon_trans55;
extern BITMAP* icon_trans65;
extern BITMAP* icon_trans75;
extern BITMAP* icon_trans85;
extern BITMAP* icon_trans95;

extern char* icon_trans05_p;
extern char* icon_trans10_p;
extern char* icon_trans15_p;
extern char* icon_trans20_p;
extern char* icon_trans25_p;
extern char* icon_trans35_p;
extern char* icon_trans45_p;
extern char* icon_trans55_p;
extern char* icon_trans65_p;
extern char* icon_trans75_p;
extern char* icon_trans85_p;
extern char* icon_trans95_p;

extern BITMAP* icon_trans50_d;
extern char* icon_trans50_d_p;

extern BITMAP *icon_back;
extern BITMAP *icon_space;

extern char *icon_back_p;
extern char *icon_space_p;

extern BITMAP *icon_A;
extern char *icon_A_p;
extern BITMAP *icon_B;
extern char *icon_B_p;
extern BITMAP *icon_C;
extern char *icon_C_p;
extern BITMAP *icon_D;
extern char *icon_D_p;
extern BITMAP *icon_E;
extern char *icon_E_p;
extern BITMAP *icon_F;
extern char *icon_F_p;
extern BITMAP *icon_G;
extern char *icon_G_p;
extern BITMAP *icon_H;
extern char *icon_H_p;
extern BITMAP *icon_I;
extern char *icon_I_p;
extern BITMAP *icon_J;
extern char *icon_J_p;
extern BITMAP *icon_K;
extern char *icon_K_p;
extern BITMAP *icon_L;
extern char *icon_L_p;
extern BITMAP *icon_M;
extern char *icon_M_p;
extern BITMAP *icon_N;
extern char *icon_N_p;
extern BITMAP *icon_O;
extern char *icon_O_p;
extern BITMAP *icon_P;
extern char *icon_P_p;
extern BITMAP *icon_Q;
extern char *icon_Q_p;
extern BITMAP *icon_R;
extern char *icon_R_p;
extern BITMAP *icon_S;
extern char *icon_S_p;
extern BITMAP *icon_T;
extern char *icon_T_p;
extern BITMAP *icon_U;
extern char *icon_U_p;
extern BITMAP *icon_V;
extern char *icon_V_p;
extern BITMAP *icon_W;
extern char *icon_W_p;
extern BITMAP *icon_X;
extern char *icon_X_p;
extern BITMAP *icon_Y;
extern char *icon_Y_p;
extern BITMAP *icon_Z;
extern char *icon_Z_p;

extern BITMAP *icon_load_d_48;
extern BITMAP *icon_load_ini_d;
extern BITMAP *icon_load_ini_d_48;

extern char *icon_load_d_48_p;
extern char *icon_load_ini_d_p;
extern char *icon_load_ini_d_48_p;

extern BITMAP *icon_angle0;
extern BITMAP *icon_angleRX;
extern BITMAP *icon_anglePto;
extern BITMAP *icon_angleRto;
extern BITMAP *icon_angleXtoP;

extern char *icon_angle0_p;
extern char *icon_angleRX_p;
extern char *icon_anglePto_p;
extern char *icon_angleRto_p;
extern char *icon_angleXtoP_p;

extern BITMAP *icon_touch_id_db_64;
extern char *icon_touch_id_db_64_p;

extern BITMAP *icon_left_margin_d_48;
extern BITMAP *icon_bottom_margin_d_48;

extern char *icon_left_margin_d_48_p;
extern char *icon_bottom_margin_d_48_p;

extern BITMAP *icon_all_layers_db_64;
extern char *icon_all_layers_db_64_p;

extern BITMAP *icon_global_db_48;
extern BITMAP *icon_local_db_48;
extern char *icon_global_db_48_p;
extern char *icon_local_db_48_p;

extern BITMAP *icon_color256_db_48;
extern char *icon_color256_db_48_p;

extern BITMAP *icon_cursors;
extern BITMAP *icon_cursor_small;
extern BITMAP *icon_cursor_big;

extern char *icon_cursors_p;
extern char *icon_cursor_small_p;
extern char *icon_cursor_big_p;

extern BITMAP *icon_question_mark_db_48;
extern char *icon_question_mark_db_48_p;

extern BITMAP *icon_pointern;
extern BITMAP *icon_pointerb;

extern char *icon_pointern_p;
extern char *icon_pointerb_p;

extern BITMAP *icon_pdf_vector_64;
extern char *icon_pdf_vector_64_p;

extern BITMAP *icon_pattern_folder;
extern char *icon_pattern_folder_p;

extern BITMAP *icon_close_window;
extern char *icon_close_window_p;

extern BITMAP *icon_change_pattern;
extern char *icon_change_pattern_p;

extern BITMAP *icon_import_map;
extern char *icon_import_map_p;

extern BITMAP *icon_starAstar;
extern BITMAP *icon_starA;
extern BITMAP *icon_Astar;
extern BITMAP *icon_Aonly;
extern BITMAP *icon_gear;
extern BITMAP *icon_confirm_or_deny;

extern char *icon_starAstar_p;
extern char *icon_starA_p;
extern char *icon_Astar_p;
extern char *icon_Aonly_p;
extern char *icon_gear_p;
extern char *icon_confirm_or_deny_p;

extern BITMAP *icon_stonewall;
extern char* icon_stonewall_p;

extern BITMAP* icon_brickwall_dx;
extern char* icon_brickwall_dx_p;
extern BITMAP* icon_brickwall_dy;
extern char* icon_brickwall_dy_p;

extern BITMAP* icon_folder_bd32;
extern char* icon_folder_bd32_p;
extern BITMAP* icon_folder_bd48;
extern char* icon_folder_bd48_p;
extern BITMAP* icon_folder_bd64;
extern char* icon_folder_bd64_p;

extern BITMAP* icon_chain_d;
extern char* icon_chain_d_p;

extern BITMAP* icon_on_top_d;
extern char* icon_on_top_d_p;

extern BITMAP* icon_h_flip;
extern char* icon_h_flip_p;
extern BITMAP* icon_v_flip;
extern char* icon_v_flip_p;

extern BITMAP* icon_offset_style;
extern char* icon_offset_style_p;
extern BITMAP* icon_offset_normal;
extern char* icon_offset_normal_p;
extern BITMAP* icon_offset_smooth;
extern char* icon_offset_smooth_p;

extern BITMAP* icon_trace_close;
extern char* icon_trace_close_p;
extern BITMAP* icon_trace_break;
extern char* icon_trace_break_p;

extern BITMAP* icon_UA_B;
extern BITMAP* icon_UA_D;
extern BITMAP* icon_UA_E;
extern BITMAP* icon_UA_J;
extern BITMAP* icon_UA_K;
extern BITMAP* icon_UA_N;
extern BITMAP* icon_UA_P;
extern BITMAP* icon_UA_R;
extern BITMAP* icon_UA_S;
extern BITMAP* icon_UA_T;
extern BITMAP* icon_UA_C;

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

extern BITMAP* icon_spline_points;
extern char* icon_spline_points_p;
extern BITMAP* icon_spline_control_points;
extern char* icon_spline_control_points_p;

extern BITMAP* icon_spline_points_close;
extern char* icon_spline_points_close_p;

extern BITMAP* icon_spline_amend;
extern char* icon_spline_amend_p;

extern BITMAP* icon_spline_points_end;
extern char* icon_spline_points_end_p;

extern BITMAP* icon_mouse1b;
extern BITMAP* icon_mouse2b;
extern BITMAP* icon_mouse1b2b;
extern BITMAP* icon_mouse3b;
extern BITMAP* icon_mouseRb;

extern char* icon_mouse1b_p;
extern char* icon_mouse2b_p;
extern char* icon_mouse1b2b_p;
extern char* icon_mouse3b_p;
extern char* icon_mouseRb_p;

extern BITMAP* icon_education;
extern char* icon_education_p;

extern BITMAP* icon_mickey_hand;
extern char* icon_mickey_hand_p;

extern BITMAP* icon_mickey_hand_s;
extern char* icon_mickey_hand_s_p;

extern BITMAP* icon_dim_leader;
extern char* icon_dim_leader_p;

extern BITMAP* icon_insulation;
extern char* icon_insulation_p;

extern BITMAP* icon_ins_simple;
extern char* icon_ins_simple_p;
extern BITMAP* icon_ins_complex;
extern char* icon_ins_complex_p;
extern BITMAP* icon_quad_trans;
extern char* icon_quad_trans_p;

extern BITMAP* icon_rect_angle_opt;
extern char* icon_rect_angle_opt_p;
extern BITMAP* icon_move_rectangle;
extern char* icon_move_rectangle_p;
extern BITMAP* icon_rect_options;
extern char* icon_rect_options_p;

extern BITMAP* icon_object_polyline;
extern char* icon_object_polyline_p;
extern BITMAP* icon_entire_polyline;
extern char* icon_entire_polyline_p;
extern BITMAP* icon_single_object;
extern char* icon_single_object_p;

extern BITMAP* icon_image_dx;
extern char* icon_image_dx_p;
extern BITMAP* icon_image_dy;
extern char* icon_image_dy_p;

extern BITMAP* icon_break_options;
extern char* icon_break_options_p;
extern BITMAP* icon_break_and_leave;
extern char* icon_break_and_leave_p;
extern BITMAP* icon_break_and_cut;
extern char* icon_break_and_cut_p;
extern BITMAP* icon_break_divide;
extern char* icon_break_divide_p;

extern BITMAP* icon_fixed_scale;
extern char* icon_fixed_scale_p;

extern BITMAP* icon_text_collineal;
extern char* icon_text_collineal_p;

extern BITMAP* icon_decimal_fractional;
extern char* icon_decimal_fractional_p;

extern BITMAP* icon_import_vectorize;
extern char* icon_import_vectorize_p;

extern BITMAP* icon_space128;
extern char* icon_space128_p;
extern BITMAP* icon_backspace128;
extern char* icon_backspace128_p;
extern BITMAP* icon_enter128;
extern char* icon_enter128_p;

extern BITMAP* icon_ttf;
extern char* icon_ttf_p;
extern BITMAP* icon_otf;
extern char* icon_otf_p;

extern BITMAP *icon_elliptical;
extern char *icon_elliptical_p;
extern BITMAP *icon_ellipticalarc;
extern char *icon_ellipticalarc_p;
extern BITMAP *icon_filledellipses;
extern char *icon_filledellipses_p;

extern BITMAP *icon_ellipticalarclocal;
extern char *icon_ellipticalarclocal_p;
extern BITMAP *icon_ellipticalarcglobal;
extern char *icon_ellipticalarcglobal_p;

extern BITMAP *icon_arc_line_continuation;
extern char *icon_arc_line_continuation_p;
extern BITMAP *icon_line_arc_continuation;
extern char *icon_line_arc_continuation_p;

extern BITMAP *icon_center_block;
extern char *icon_center_block_p;
extern BITMAP *icon_alpha_sorting_d32;
extern char *icon_alpha_sorting_d32_p;
extern BITMAP *icon_time_lapse_d32;
extern char *icon_time_lapse_d32_p;

extern BITMAP *icon_select_entity;
extern char *icon_select_entity_p;

extern BITMAP *icon_select;
extern char *icon_select_p;

extern BITMAP *icon_vector;
extern char *icon_vector_p;
extern BITMAP *icon_rigid_rigid;
extern char *icon_rigid_rigid_p;
extern BITMAP *icon_rigid_pin;
extern char *icon_rigid_pin_p;
extern BITMAP *icon_pin_rigid;
extern char *icon_pin_rigid_p;
extern BITMAP *icon_pin_pin;
extern char *icon_pin_pin_p;

extern BITMAP *icon_force;
extern char *icon_force_p;
extern BITMAP *icon_moment;
extern char *icon_moment_p;
extern BITMAP *icon_moment_rev;
extern char *icon_moment_rev_p;
extern BITMAP *icon_displacement;
extern char *icon_displacement_p;
extern BITMAP *icon_rotation;
extern char *icon_rotation_p;
extern BITMAP *icon_rotation_rev;
extern char *icon_rotation_rev_p;
extern BITMAP *icon_trapezium_y;
extern char *icon_trapezium_y_p;
extern BITMAP *icon_trapezium_x;
extern char *icon_trapezium_x_p;
extern BITMAP *icon_trapezium_n;
extern char *icon_trapezium_n_p;
extern BITMAP *icon_trapezium_h;
extern char *icon_trapezium_h_p;
extern BITMAP *icon_trapezium_v;
extern char *icon_trapezium_v_p;

extern BITMAP *icon_fixed_h;
extern char *icon_fixed_h_p;
extern BITMAP *icon_fixed_vl;
extern char *icon_fixed_vl_p;
extern BITMAP *icon_fixed_vr;
extern char *icon_fixed_vr_p;

extern BITMAP *icon_pinned_h;
extern char *icon_pinned_h_p;
extern BITMAP *icon_pinned_vl;
extern char *icon_pinned_vl_p;
extern BITMAP *icon_pinned_vr;
extern char *icon_pinned_vr_p;

extern BITMAP *icon_fixed_roller_h;
extern char *icon_fixed_roller_h_p;
extern BITMAP *icon_fixed_roller_vl;
extern char *icon_fixed_roller_vl_p;
extern BITMAP *icon_fixed_roller_vr;
extern char *icon_fixed_roller_vr_p;

extern BITMAP *icon_pinned_roller_h;
extern char *icon_pinned_roller_h_p;
extern BITMAP *icon_pinned_roller_vl;
extern char *icon_pinned_roller_vl_p;
extern BITMAP *icon_pinned_roller_vr;
extern char *icon_pinned_roller_vr_p;

extern BITMAP *icon_node;
extern char *icon_node_p;
extern BITMAP *icon_magnitude;
extern char *icon_magnitude_p;

extern BITMAP *icon_thermal;
extern char *icon_thermal_p;

extern BITMAP *icon_gear_plus;
extern char *icon_gear_plus_p;

extern BITMAP *icon_fixed_hu;
extern char *icon_fixed_hu_p;
extern BITMAP *icon_pinned_hu;
extern char *icon_pinned_hu_p;
extern BITMAP *icon_fixed_roller_hu;
extern char *icon_fixed_roller_hu_p;
extern BITMAP *icon_pinned_roller_hu;
extern char *icon_pinned_roller_hu_p;
extern BITMAP *icon_fixed_rotation;
extern char *icon_fixed_rotation_p;

extern BITMAP *icon_compression_mag;
extern char *icon_compression_mag_p;
extern BITMAP *icon_tension_mag;
extern char *icon_tension_mag_p;
extern BITMAP *icon_shear_mag;
extern char *icon_shear_mag_p;
extern BITMAP *icon_moments_mag;
extern char *icon_moments_mag_p;
extern BITMAP *icon_displacements_mag;
extern char *icon_displacements_mag_p;
extern BITMAP *icon_reactions_xy;
extern char *icon_reactions_xy_p;
extern BITMAP *icon_reactions_m;
extern char *icon_reactions_m_p;

extern BITMAP *icon_thermal_y;
extern char *icon_thermal_y_p;
extern BITMAP *icon_thermal_t;
extern char *icon_thermal_t_p;

extern BITMAP *icon_trapezium_y1;
extern char *icon_trapezium_y1_p;
extern BITMAP *icon_trapezium_y2;
extern char *icon_trapezium_y2_p;
extern BITMAP *icon_thermal_t1;
extern char *icon_thermal_t1_p;
extern BITMAP *icon_thermal_t2;
extern char *icon_thermal_t2_p;

extern BITMAP *icon_vectors;
extern char *icon_vectors_p;

extern BITMAP *icon_US_Flag;
extern char *icon_US_Flag_p;
extern BITMAP *icon_EU_Flag;
extern char *icon_EU_Flag_p;
extern BITMAP *icon_UK_Flag;
extern char *icon_UK_Flag_p;

extern BITMAP *icon_IH_sections;
extern char *icon_IH_sections_p;
extern BITMAP *icon_U_sections;
extern char *icon_U_sections_p;
extern BITMAP *icon_T_sections;
extern char *icon_T_sections_p;
extern BITMAP *icon_L_sections;
extern char *icon_L_sections_p;
extern BITMAP *icon_2L_sections;
extern char *icon_2L_sections_p;
extern BITMAP *icon_RT_sections;
extern char *icon_RT_sections_p;
extern BITMAP *icon_O_sections;
extern char *icon_O_sections_p;
extern BITMAP *icon_EO_sections;
extern char *icon_EO_sections_p;
extern BITMAP *icon_Z_sections;
extern char *icon_Z_sections_p;
extern BITMAP *icon_VJ_sections;
extern char *icon_VJ_sections_p;

extern BITMAP *icon_timber;
extern char *icon_timber_p;

extern BITMAP *icon_node_size;
extern char *icon_node_size_p;

extern BITMAP *icon_dead_load;
extern char *icon_dead_load_p;
extern BITMAP *icon_live_load;
extern char *icon_live_load_p;
extern BITMAP *icon_roof_load;
extern char *icon_roof_load_p;
extern BITMAP *icon_wind_load;
extern char *icon_wind_load_p;
extern BITMAP *icon_snow_load;
extern char *icon_snow_load_p;
extern BITMAP *icon_earthquake_load;
extern char *icon_earthquake_load_p;
extern BITMAP *icon_rain_load;
extern char *icon_rain_load_p;
extern BITMAP *icon_soil_load;
extern char *icon_soil_load_p;
extern BITMAP *icon_fluid_load;
extern char *icon_fluid_load_p;
extern BITMAP *icon_thermal_load;
extern char *icon_thermal_load_p;

extern BITMAP *icon_unspecified_load;
extern char *icon_unspecified_load_p;
extern BITMAP *icon_unknown_load;
extern char *icon_unknown_load_p;
extern BITMAP *icon_number_load;
extern char *icon_number_load_p;

extern BITMAP *icon_stress_mag;
extern char *icon_stress_mag_p;
extern BITMAP *icon_stress_plus_mag;
extern char *icon_stress_plus_mag_p;
extern BITMAP *icon_stress_minus_mag;
extern char *icon_stress_minus_mag_p;
extern BITMAP *icon_shear_stress_mag;
extern char *icon_shear_stress_mag_p;

extern BITMAP *icon_eurocode_d48;
extern char *icon_eurocode_d48_p;
extern BITMAP *icon_asce_d48;
extern char *icon_asce_d48_p;
extern BITMAP *icon_icc_d48;
extern char *icon_icc_d48_p;

extern BITMAP *icon_combination_d48;
extern char *icon_combination_d48_p;

extern BITMAP *icon_erase_layer_db_64;
extern char *icon_erase_layer_db_64_p;
extern BITMAP *icon_mark_layer_db_64;
extern char *icon_mark_layer_db_64_p;

extern BITMAP *icon_AlfaCAD48;
extern char *icon_AlfaCAD48_p;

extern BITMAP *icon_Pdelta_d48;
extern char *icon_Pdelta_d48_p;

extern BITMAP *icon_dynamics;
extern char *icon_dynamics_p;

extern BITMAP *icon_vibrations_d48;
extern char *icon_vibrations_d48_p;
extern BITMAP *icon_inertia_d48;
extern char *icon_inertia_d48_p;

extern BITMAP *icon_dynamics_run;
extern char *icon_dynamics_run_p;

extern BITMAP *icon_menustyle;
extern char *icon_menustyle_p;
extern BITMAP *icon_cursorstyle;
extern char *icon_cursorstyle_p;
extern BITMAP *icon_barstyle;
extern char *icon_barstyle_p;

extern BITMAP *icon_perc_mag;
extern char *icon_perc_mag_p;

extern BITMAP *icon_cross_section_forces;
extern char *icon_cross_section_forces_p;

extern BITMAP *icon_resilience;
extern char *icon_resilience_p;

extern BITMAP *icon_hourglass_mem;

extern BITMAP *icon_ULS;
extern char *icon_ULS_p;
extern BITMAP *icon_SLS;
extern char *icon_SLS_p;
extern BITMAP *icon_QPSLS;
extern char *icon_QPSLS_p;

extern BITMAP *icon_yes_dmem;
extern BITMAP *icon_no_dmem;
extern char *icon_yes_d_pmem;
extern char *icon_no_d_pmem;
extern BITMAP* icon_upgrademark_mem;
extern char* icon_upgrademark_pmem;
extern BITMAP* icon_noupgrademark_mem;
extern char* icon_noupgrademark_pmem;

extern int EditText(char *, int adjust, int nCmdShow, int *single, int *tab);
extern int set_window_icon(void);
extern BOOL preview_blocked;

extern FONT *font1, *font2;


int mynCmdShow;
BITMAP *doubleBuffer = NULL;
SAMPLE *sound;
char textbuff[80] = "Foo";
BITMAP *bg;

int buttonCallback(int id);
int buttonCallback2(int id);
int onThemeChange(int id);

static int colors[3];

static int n_demo=0;

static DRIVER_STRUCT drv_master={2,800,600,8,3,287,0,1.0};
static DRIVER_STRUCT drv_slave={1,800,600,8,2,287,0,1.0};

#ifdef __cplusplus
extern "C" {
#endif

    int Rysuj_main(int child, char file_name[255], int nCmdShow, char* application, char* arg_params);

#ifdef __cplusplus
}
#endif

#include "o_protec.h"
#include "b_windem.c"

#include "rysuj.h"

void noopg(void)
{}
static void nooop(void)
{}

#include "bibrys.c"

extern void Biblioteka(void);

extern void Automatic_numbering(void);

extern void Find_Text(void);
extern void Find_and_Change_Text(void);

static void (*COMNDg[])(void)= { Rysuj, Blok, Edycja, nooop, nooop, Wymiarowanie, Hatch, Geometria,
			 Makro,Parametry, Opcje, nooop, Wyjscie, Koniec, Close_window, Find_Text, Find_and_Change_Text, Automatic_numbering, Change_Properties, Spline_Amendment, Change_Vectors, /*21*/ Static_analysis, /*22*/ Cross_section_forces, /*23*/ Animate_dynamics, /*24*/ SteelEU, /*25*/SteelUK, /*26*/SteelUS,  /*27*/TimberUS} ;

//#define WOOD 28 //26   //the number of function for wood choice
int WOOD=sizeof(COMNDg)/sizeof(COMNDg[0]);

TMENU menug={15,0,0,16,1,3, ICONS | TADD,CMNU,CMBR,CMTX,0,0,0,0,0,&pmenug,NULL,NULL};  //flag 7 : icons

typedef  void (*f)(void);
typedef  f * TA;

extern void makro_esc(void);

#define BUF_SIZE 1024
#define SHM_KEY 0x414C4641 //0x1234
#define FILENO 16

void Free_alft(void)
{
    if (alft) free(alft);
}

static int shmid;

struct shmbuf {
    int client_number;
    int mflag;
    int flag[FILENO];
    char file_name[FILENO][MAXPATH];
};

struct shmseg {
    int cnt;
    int complete;
    //char buf[BUF_SIZE];
    struct shmbuf buf;
};

static struct shmseg *shmp;
static struct shmseg shmp_private;

#ifdef LINUX
void Sleep(int miliseconds) {
    sleep(miliseconds);
}
#endif

int Test_App_Shm(int doit, char *file_name)
{

#ifdef LINUX
    int ret_shm;
    int numtimes;
    struct shmbuf *bufptr;
    int spaceavailable;
    struct shmid_ds shmid_buf;
    int i, ii, ret;


    if (doit==1) {

        Number_of_clients=1;
        Client_number=1;

        shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644 | IPC_CREAT);
        if (shmid == -1) {
            perror("Shared memory");
            return 0;
        }

        // Attach to the segment to get a pointer to it.
        shmp = shmat(shmid, NULL, 0);
        if (shmp == (void *) -1) {
            perror("Shared memory attach");
            return 0;
        }

        bufptr = &shmp->buf;

        ret_shm=shmctl(shmid, IPC_STAT, &shmid_buf);
        Number_of_clients=shmid_buf.shm_nattch;
        if (Number_of_clients>1) //another instance exists
        {
            if ((file_name!=NULL) && (strlen(file_name)>0))  //new window inside existing instance
            {
                Client_number=bufptr->client_number;
                ii=0;
                while ((shmp->complete!=1) && (ii<10)) {Sleep(15); ii++;}
                shmp->complete = 0;
                //searching for free slot
                i=0;
                while (i<FILENO)
                {
                    if (bufptr->flag[i]==0) break;
                    i++;
                }
                if (i<FILENO)  //still one of slots is free
                {
                    strcpy(bufptr->file_name[i], file_name);
                    bufptr->flag[i] = 1;
                    bufptr->mflag = 1;
                    shmp->complete = 1;

                    quick_exit(1);  //1 for exit after initiating newe file in existing instance
                }
                else //leaving new instance
                {
                    Client_number=bufptr->client_number;
                    Client_number++;
                    XWindow_Name(Client_number);
                    bufptr->client_number=Client_number;
                    shmp->complete = 1;

                    //private
                    shmp_private.complete=0;
                    shmp_private.buf.client_number=Client_number;
                    //zeroing
                    memset(shmp_private.buf.flag, 0, sizeof(shmp_private.buf.flag));
                    memset(shmp_private.buf.file_name, 0, sizeof(shmp_private.buf.file_name));
                    shmp_private.buf.mflag=0;
                    shmp_private.complete=1;
                }
            }
            else //leaving new instance
            {
                ii=0;
                while ((shmp->complete!=1) && (ii<10)) {sleep(15); ii++;}
                shmp->complete = 0;
                Client_number=bufptr->client_number;
                Client_number++;
                XWindow_Name(Client_number);
                bufptr->client_number=Client_number;
                shmp->complete = 1;

                //private
                shmp_private.complete=0;
                shmp_private.buf.client_number=Client_number;
                //zeroing
                memset(shmp_private.buf.flag, 0, sizeof(shmp_private.buf.flag));
                memset(shmp_private.buf.file_name, 0, sizeof(shmp_private.buf.file_name));
                shmp_private.buf.mflag=0;
                shmp_private.complete=1;
            }
        }
        else
        {
            Client_number = Number_of_clients;
            XWindow_Name(Client_number);
            shmp->complete = 0;
            bufptr->client_number=Client_number;
            //zeroing
            memset(bufptr->flag, 0, sizeof(bufptr->flag));
            memset(bufptr->file_name, 0, sizeof(bufptr->file_name));
            bufptr->mflag=0;
            shmp->complete = 1;

            //private
            shmp_private.complete=0;
            shmp_private.buf.client_number=Client_number;
            //zeroing
            memset(shmp_private.buf.flag, 0, sizeof(shmp_private.buf.flag));
            memset(shmp_private.buf.file_name, 0, sizeof(shmp_private.buf.file_name));
            shmp_private.buf.mflag=0;
            shmp_private.complete=1;
        }
    }
   else if (doit==2) //just adding file in private
    {
        if ((file_name!=NULL) && (strlen(file_name)>0))  //new window inside existing instance
        {
            bufptr = &shmp_private.buf;

            ii=0;
            while ((shmp_private.complete!=1) && (ii<10)) {sleep(15); ii++;}
            shmp_private.complete = 0;
            //searching for free slot
            i=0;
            while (i<FILENO)
            {
                if (bufptr->flag[i]==0) break;
                i++;
            }
            if (i<FILENO)  //still one of slots is free
            {
                strcpy(bufptr->file_name[i], file_name);
                bufptr->flag[i] = 1;
                bufptr->mflag = 1;
                shmp_private.complete = 1;
            }
            else
            {
                shmp_private.complete = 1;
                return 0;
            }
        }
    }
   else //just checking
   {
        ret_shm = shmctl(shmid, IPC_STAT, &shmid_buf);
        Number_of_clients = shmid_buf.shm_nattch;
    }

    return 1;
#else
int ret_shm;
//int shmid;
int numtimes;
//struct shmseg *shmp;
struct shmbuf* bufptr;
//int spaceavailable;
//struct shmid_ds shmid_buf;
int i, ii, ret;

if (doit == 1) {

    // Attach to the segment to get a pointer to it.
    ret = GetShmpPtr_1(&shmp);

    if (ret == 0)
        return 0;

    bufptr = &shmp->buf;
    //shmp->complete = 0;

    if (Number_of_clients > 1) //another instance exists
    {
        if ((file_name != NULL) && (strlen(file_name) > 0))  //new window inside existing instance
        {
            ////Client_number = bufptr->client_number;
            ii = 0;
            while ((shmp->complete != 1) && (ii < 10)) { _sleep(15); ii++; }
            shmp->complete = 0;
            //memcpy(&Client_number, bufptr->client_number, sizeof(Client_number));
            //searching for free slot
            i = 0;
            while (i < FILENO)
            {
                if (bufptr->flag[i] == 0) break;
                i++;
            }
            if (i < FILENO)  //still one of slots is free
            {
                strcpy(bufptr->file_name[i], file_name);
                bufptr->flag[i] = 1;
                bufptr->mflag = 1;
                shmp->complete = 1;

                quick_exit(1);  //1 for exit after initiating newe file in existing instance
            }
            else //leaving new instance
            {
                ////Client_number = bufptr->client_number;
                ////Client_number++;
                ////XWindow_Name(Client_number);
                bufptr->client_number = Client_number;
                shmp->complete = 1;

                //private
                shmp_private.complete = 0;
                shmp_private.buf.client_number = Client_number;
                //zeroing
                memset(shmp_private.buf.flag, 0, sizeof(shmp_private.buf.flag));
                memset(shmp_private.buf.file_name, 0, sizeof(shmp_private.buf.file_name));
                shmp_private.buf.mflag = 0;
                shmp_private.complete = 1;
            }
        }
        else //leaving new instance
        {
            ii = 0;
            while ((shmp->complete != 1) && (ii < 10)) { _sleep(15); ii++; }
            shmp->complete = 0;
            //memcpy(&Client_number, bufptr, sizeof(Client_number));
            ////Client_number = bufptr->client_number;
            ////Client_number++;
            ////XWindow_Name(Client_number);
            //memcpy(bufptr, &Client_number, sizeof(Client_number));
            bufptr->client_number = Client_number;
            shmp->complete = 1;

            //private
            shmp_private.complete = 0;
            shmp_private.buf.client_number = Client_number;
            //zeroing
            memset(shmp_private.buf.flag, 0, sizeof(shmp_private.buf.flag));
            memset(shmp_private.buf.file_name, 0, sizeof(shmp_private.buf.file_name));
            shmp_private.buf.mflag = 0;
            shmp_private.complete = 1;
        }
    }
    else
    {
        ////Client_number = Number_of_clients;
        ////XWindow_Name(Client_number);
        shmp->complete = 0;
        //memcpy( bufptr->client_number, &Client_number, sizeof(Client_number));
        bufptr->client_number = Client_number;
        //zeroing
        memset(bufptr->flag, 0, sizeof(bufptr->flag));
        memset(bufptr->file_name, 0, sizeof(bufptr->file_name));
        bufptr->mflag = 0;
        shmp->complete = 1;

        //private
        shmp_private.complete = 0;
        shmp_private.buf.client_number = Client_number;
        //zeroing
        memset(shmp_private.buf.flag, 0, sizeof(shmp_private.buf.flag));
        memset(shmp_private.buf.file_name, 0, sizeof(shmp_private.buf.file_name));
        shmp_private.buf.mflag = 0;
        shmp_private.complete = 1;
    }
}
else if (doit == 2) //just adding file in private
{
    if ((file_name != NULL) && (strlen(file_name) > 0))  //new window inside existing instance
    {
        bufptr = &shmp_private.buf;

        ii = 0;
        while ((shmp_private.complete != 1) && (ii < 10)) { _sleep(15); ii++; }
        shmp_private.complete = 0;
        //memcpy(&Client_number, bufptr->client_number, sizeof(Client_number));
        //searching for free slot
        i = 0;
        while (i < FILENO)
        {
            if (bufptr->flag[i] == 0) break;
            i++;
        }
        if (i < FILENO)  //still one of slots is free
        {
            strcpy(bufptr->file_name[i], file_name);
            bufptr->flag[i] = 1;
            bufptr->mflag = 1;
            shmp_private.complete = 1;
        }
        else
        {
            shmp_private.complete = 1;
            return 0;
        }
    }
}
else //just checking
{
    ;
}

return 1;
#endif
}

int Test_App_Shm_Off(void)
{
#ifndef LINUX
#else
   int ret_shm;
   struct shmid_ds shmid_buf;

    ret_shm = shmctl(shmid, IPC_STAT, &shmid_buf);
    Number_of_clients = shmid_buf.shm_nattch;
    if (Number_of_clients == 1)
        ret_shm=shmctl(shmid, IPC_RMID, NULL);
#endif
    return 1;
}

int Get_Shm_private_mflag(void)
{
    if (shmp_private.complete==1) return shmp_private.buf.mflag;
    else return 0;
}

int Get_Shm_mflag(void)
{
    if (shmp->complete==1) return shmp->buf.mflag;
    else return 0;
}

int get_graphic_drv(void)
{
	return graphic_drv;
}

void close_graph (void)
/*-------------------*/
{
  closegraph () ;
}


static int i__svgamode = SVGA_WRONG_MODE ;

unsigned _stklen = 5 * 1024 ;
double pole_hatch ;
int i__svga; 

static void out_file_name1 (void)
/*-------------------------------*/
{
  char *str;

  str = strrchr(zbior_danych, '/' /* '\\'*/);
  if (str == NULL)
  {
    str = zbior_danych;
  }
  else
  {
   str++;
  }
  komunikat0_str (9, str) ;
}


void set_increased(int x, int y)
{ int c_256;
  int ret;
  int k;

  if (graphic_drv==DRV_MASTER)
  {
    c_depth=drv_master.gfx_bpp;
    c_256=pow(2,c_depth);

    increase_window(&drv_master, x, y);
  }
  else 
  {
	c_depth=drv_slave.gfx_bpp;
    c_256=pow(2,c_depth);

    increase_window(&drv_slave, x, y);
  }

  Copy_screen();
  mysetrgbdefaults();
  check_tables();
  ggetmaxx=getmaxx();
  ggetmaxy=getmaxy();
  monitor_parametry_again(Driver);
  
  destroy_parent();
  change_second_screen();
  cleardevice();
  re_set_params();

  free_mouse();  
  lock_mouse();  

  redraw();  
  k = my_poll_keyboard();

  return;
}

void set_resized_window(int dx, int dy)
{ int c_256;
  int ret;
  int k;
  int drv_ok;

  drv_ok=get_master_data(&drv_master);

  if (graphic_drv==DRV_MASTER)
  {
    c_depth=drv_master.gfx_bpp;
    c_256=pow(2,c_depth);
    
    set_resized_window_GFX(&drv_master, dx, dy);
  }
  else
  {
	c_depth=drv_slave.gfx_bpp;
    c_256=pow(2,c_depth);
    
    set_resized_window_GFX(&drv_slave, dx, dy);
  }
  lock_mouse();
  Copy_screen();
  mysetrgbdefaults();
  ret=check_tables();
  ggetmaxx=getmaxx();
  ggetmaxy=getmaxy();
  monitor_parametry_again(Driver);
 
  destroy_parent();
  change_second_screen();
  
  cleardevice();
  re_set_params();

  if (dane_size>0)
      redraw();
  k=my_poll_keyboard();

  return;
}

void set_original_window(void)
{ int c_256;
  int ret;
  int k;

  if (graphic_drv==DRV_MASTER)
  {
   c_depth=drv_master.gfx_bpp;
   c_256=pow(2,c_depth);

   set_original_window_GFX(&drv_master);
  }
  else
  {
   c_depth=drv_slave.gfx_bpp;
   c_256=pow(2,c_depth);

   set_original_window_GFX(&drv_slave);
  }
  Copy_screen();
  mysetrgbdefaults();
  check_tables();
  ggetmaxx=getmaxx();
  ggetmaxy=getmaxy();
  monitor_parametry_again(Driver);
 
  destroy_parent();
  change_second_screen();
  cleardevice();
  re_set_params();

  free_mouse();
  lock_mouse();
  
  redraw();
  k=my_poll_keyboard();

  simulate_keypress(21504);
  
  return;
}

void set_last_window(void)
{ int c_256;
  int ret;
  int k;

  if (graphic_drv==DRV_MASTER)
  {
    c_depth=drv_master.gfx_bpp;
    c_256=pow(2,c_depth);

    set_last_window_GFX(&drv_master);
  }
  else
  {
	c_depth=drv_slave.gfx_bpp;
    c_256=pow(2,c_depth);

    set_last_window_GFX(&drv_slave);
  }
  Copy_screen();
  mysetrgbdefaults();
  check_tables();
  ggetmaxx=getmaxx();
  ggetmaxy=getmaxy();
  monitor_parametry_again(Driver);

  destroy_parent();
  change_second_screen();
  cleardevice();
  re_set_params();


  free_mouse();
  lock_mouse();

  redraw();
  k = my_poll_keyboard();

  return;
}

void reset_font(void)
{
	FILE *f_ini;

	strcpy(font_name, "DejaVuSans.ttf");
	strcpy(Czcionka_Pulpitu, font_name);
	MP_SIZE = 19;
	BAR_G = 19;
	HEIGHT = 19;
    ED_INF_HEIGHT = 21; //20;
	WIDTH = 10;
	f_ini = fopen(font_file_name, "wt");

	fprintf(f_ini, "%s\n", font_name);
	fprintf(f_ini, "%ld,%ld,%ld,%ld,%ld\n", MP_SIZE, ED_INF_HEIGHT, BAR_G, HEIGHT, WIDTH);
	fclose(f_ini);
	
}

volatile int ticks = 0;
volatile int total_ticks = 0;
void ticker()
{
	ticks++;
	total_ticks++;
}
END_OF_FUNCTION(ticker)

const int updates_per_second = 60;

void animate(void)
{
	LOCK_VARIABLE(ticks);
	LOCK_VARIABLE(total_ticks);
	LOCK_FUNCTION(ticker);
	install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

	BOOL quit = FALSE;

	a4a_animation_t * haddokken = a4a_animation_createf(
		A4A_SPRITE_TYPE_PNG,
		10,
		5,
		"media/ryu-haddoken%02d.png");

	if (!haddokken)
	{
		allegro_message(
			"Failed to create haddokken a4a_animation. Aborting...");
		exit(1);
	}

	a4a_animation_begin(haddokken, total_ticks);

	while (!quit && !key[KEY_ESC])
	{
		while (ticks == 0)
		{
			rest(100 / updates_per_second);//rest until a full tick has passed
		}

		while (ticks > 0)
		{
			int old_ticks = ticks;

			ticks--;
			if (old_ticks <= ticks)//logic is taking too long: abort and draw a frame
				break;
		}

		BITMAP * haddokken_frame = a4a_animation_frame(
			haddokken,
			total_ticks);

		clear_to_color(haddokken_frame, makecol(255, 0, 255));
		rectfill(haddokken_frame, 10, 10, 50, 50, makecol(255, 255, 255));

		
		draw_sprite(screen, haddokken_frame, 0, 0);
	}

	a4a_animation_destroy(&haddokken);

}

static int noop1(void)
{
	return 0;
}


void mvcurbpTt(unsigned i)      /* ini */
/*-----------------------*/
{
	int wt_lw_no = i;
}


void draw_font_name_ttf(TEXT *t, char *t_text, BITMAP *bmp, int x0, int y0, int xmax, double kat, float wysokosc_p, COLOR_ kolor, int mode, int t_width, int t_height)
{
	GLYPH_FACE *face;
	GLYPH_REND *rend;
	
	unsigned char red, green, blue;
	int text_v, text_h, text_v_, text_h_;
	double text_vd, text_hd;
	int x, y;
	int x1, y1, x2, y2, x3, y3, x4, y4;
	double tsin, tcos;
	char *ptr_n, *ptr_r;

	int i = 0;
	int len_pxl = 0;
	unsigned int unicode;
	char *ptr, *ptr1;
	int max_pxl;

	x = x0;
	y = y0;

	max_pxl = xmax - x0;

	face = face_ttf[t->czcionka];
	if (!face) return;

	rend = gk_create_renderer(face, 0);
	if (!rend) return;

	gk_rend_set_error_char(rend, 0);
	gk_rend_set_undefined_char(rend, 0);

	gk_rend_set_size_subpixel(rend, (int)((wysokosc_p*t->width_factor*TTF_width_factor*64.0) + 0.5), (int)((wysokosc_p*TTF_height_factor*64.0) + 0.5));
	
	if (t->italics) gk_rend_set_italic_angle_in_degrees(rend, 10.5);
	if (t->bold) gk_rend_set_bold_strength(rend, TTF_bold_factor);
	
	gk_rend_set_text_color(rend, kolor.red, kolor.gre, kolor.blu);
	if (t->ukryty) gk_rend_set_text_alpha(rend, 50);

	ptr_n = strchr(t_text, '\n');
	if (ptr_n != NULL) *ptr_n = '\0';
	ptr_r = strchr(t_text, '\r');
	if (ptr_r != NULL) *ptr_r = '\0';

	//finding max length
	ptr = t_text;
	len_pxl = 0;
	while (*ptr != '\0')
	{
		unicode = utf8_to_ucs2(ptr, &ptr1);
		len_pxl += gk_char_width(rend, unicode);
		if (len_pxl > max_pxl) break;
		i += ptr1 - ptr;
		ptr = ptr1;
	}

	*ptr = '\0';

	if (mode == COPY_PUT)
	{
		gk_rend_set_angle_in_radians(rend, kat);
		gk_render_line_utf8(bmp, rend, t_text, x, y);
	}
}

int Get_TTF_Char_Outline(TEXT *t, unsigned int unicode, char *alf, long *lw, int yMax)
{
	GLYPH_FACE *face;

	face = face_ttf[t->czcionka];
	if (!face) return -1;
	return gk_face_character_outline(face, unicode, alf, lw, yMax);
}

void Get_Face_Character_yMax(TEXT *t, int *yMax)
{
	int ret;
	ret = gk_face_character_ymax(face_ttf[t->czcionka], yMax);
}

void Amend_Draw_TTF_text(TEXT *t, float wysokosc_p)
{
	gk_rend_set_size_subpixel(rend_ini, (int)((wysokosc_p*t->width_factor*TTF_width_factor*64.0) + 0.5), (int)((wysokosc_p*TTF_height_factor*64.0) + 0.5));
}

void Init_Draw_TTF_text(TEXT *t, double kat, float wysokosc_p, COLOR_ kolor)
{
	unsigned char red, green, blue;
	
	if (rend_ini) gk_done_renderer(rend_ini);

	face_ini = face_ttf[t->czcionka];
	if (!face_ini) return;

    if (!keep_ini) keep_ini = gk_create_keeper(1890, 8 * 1024 * 1024);

	rend_ini = gk_create_renderer(face_ini, keep_ini);
	if (!rend_ini) return;

	gk_rend_set_error_char(rend_ini, 0);
	gk_rend_set_undefined_char(rend_ini, 0);

	gk_rend_set_size_subpixel(rend_ini, (int)((wysokosc_p*t->width_factor*TTF_width_factor*64.0) + 0.5), (int)((wysokosc_p*TTF_height_factor*64.0) + 0.5));
	
	if (t->italics) gk_rend_set_italic_angle_in_degrees(rend_ini, 10.5);
	if (t->bold) gk_rend_set_bold_strength(rend_ini, TTF_bold_factor);
	
	gk_rend_set_text_color(rend_ini, kolor.red, kolor.gre, kolor.blu);
	if (t->ukryty) gk_rend_set_text_alpha(rend_ini, 50);

	gk_rend_set_angle_in_radians(rend_ini, kat);

}

void Draw_TTF_char(TEXT *t, unsigned int unicode, BITMAP *bmp, int x0, int y0)
{
	int x, y;
	x = x0<<6;
	y = y0<<6;

	gk_render_char(bmp, rend_ini, unicode, &x, &y);
}
//////////////////////////

void Draw_TTF_text(TEXT *t, char *t_text, BITMAP *bmp, int x0, int y0, double kat, float wysokosc_p, COLOR_ kolor, int mode, int t_width, int t_height)
{
	GLYPH_FACE *face;
	GLYPH_REND *rend;
	
	unsigned char red, green, blue;
	int text_v, text_h, text_v_, text_h_;
	double text_vd, text_hd;
	int x, y;
	int x1, y1, x2, y2, x3, y3, x4, y4;
	double tsin, tcos;
	char *ptr_n, *ptr_r;
	int lw;
	char *alf;

	unsigned int unicode;
	char *ptr1;
	
	x = x0;
	y = y0;

	
	face = face_ttf[t->czcionka];
	if (!face) return;


	rend = gk_create_renderer(face, 0 /*keep*/);
	if (!rend) return;

	gk_rend_set_error_char(rend, 0);
	gk_rend_set_undefined_char(rend, 0);

	gk_rend_set_size_subpixel(rend, (int)((wysokosc_p*t->width_factor*TTF_width_factor*64.0)+0.5), (int)((wysokosc_p*TTF_height_factor*64.0)+0.5));
	
	if (t->italics) gk_rend_set_italic_angle_in_degrees(rend, 10.5);
	if (t->bold) gk_rend_set_bold_strength(rend, TTF_bold_factor);
	
	gk_rend_set_text_color(rend, kolor.red, kolor.gre, kolor.blu);
	if (t->ukryty) gk_rend_set_text_alpha(rend, 50);

    ptr_n = strchr(t_text, '\n');
	if (ptr_n!=NULL) *ptr_n='\0';
	ptr_r = strchr(t_text, '\r');
	if (ptr_r != NULL) *ptr_r = '\0';

	
	if (mode == COPY_PUT)
	{
		
		gk_rend_set_angle_in_radians(rend, kat);
		gk_render_line_utf8(bmp, rend, t_text, x, y);

	}
	else
	{
		//calculating the size with angle0
		gk_rend_set_angle_in_radians(rend, 0.0);

		if (!TTF_SIZE_BUFFER || ((t_height == 0) && (t_width == 0)))
		{
			text_v = gk_text_height_utf8(rend, t_text);
			gk_text_advance_subpixel_utf8(rend, t_text, &text_h, &text_v_);
		}
		else
		{
			text_h = t_width;
			text_v = t_height;
		}
		//drawing rectangle rotated about insert point at t->kat
		tsin = sin(t->kat);
		tcos = cos(t->kat);

		text_hd = ((double)(text_h))/64.0;
		text_vd = text_v;
		text_vd *= 0.64;

		switch (t->justowanie)
		{
		case 0: //to the left
		case 1://to the right
		case 2://to the middle
		case 3: //to the centre
			x1 = x;
			y1 = y;

			x2 = x + (int)(text_hd * tcos);
			y2 = y - (int)(text_hd * tsin);

			x3 = x + (int)(text_hd * tcos - text_vd * tsin);
			y3 = y - (int)(text_hd * tsin + text_vd * tcos);

			x4 = x - (int)(text_vd * tsin);
			y4 = y - (int)(text_vd * tcos);
			break;
			
		}

		SetColorAC(t->kolor);
		if (t->bold == 0) setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
		else setlinestyle1(SOLID_LINE, 0, 3/*NORM_WIDTH*/);

		
		linen(x1, y1, x2, y2, mode);
		linen(x2, y2, x3, y3, mode);	
		linen(x3, y3, x4, y4, mode);		
		linen(x4, y4, x1, y1, mode);

		TTF_redraw = TRUE;
	}

    if (ptr_n != NULL) *ptr_n = '\n';
	if (ptr_r != NULL) *ptr_r = '\r';
	
	gk_done_renderer(rend);
	
	return;
}

//////////////////////////
#define ttf_h 1.6
#define ttf_v 0.9

int TTF_text_len(char *text)
{
	int text_h, text_v;

	if (!rend_UI) return 0;

	gk_rend_set_size_pixels(rend_UI, WIDTH*ttf_h, HEIGHT*ttf_v); //
	gk_rend_set_bold_strength(rend_UI, 50);
	gk_text_size_utf8(rend_UI, text, &text_h, &text_v);
	return text_h;
}

int TTF_char_len(unsigned int unicode)
{
	if (!rend_UI) return 0;

	gk_rend_set_size_pixels(rend_UI, WIDTH*ttf_h, HEIGHT*ttf_v); //
	gk_rend_set_bold_strength(rend_UI, 50);
	return gk_char_width(rend_UI, unicode);
}

//returning length based on pos
int TTF_text_len_pos(char *text, int pos)
{
	char buf[MaxTextLen*2+2];
	int text_h, text_v;

	if (pos < 0)
		return 0;

	if (!rend_UI) return 0;

	if (text == NULL) return 0;
	if (pos == 0) return 0;

	strncpy(buf, text, (MaxTextLen*2));
	buf[pos]='\0';

	gk_rend_set_size_pixels(rend_UI, WIDTH*ttf_h, HEIGHT*ttf_v); //
	gk_rend_set_bold_strength(rend_UI, 50);
	gk_text_size_utf8(rend_UI, buf, &text_h, &text_v);
	return text_h;
}

//returning pos based on cursor position
BOOL TTF_text_pos_x0(char *text, int x0, int y0, int width_w, int *pos)
{   int x_length, x_position, y_position;
    char buf[MaxTextLen*2+2];
    int text_h, text_v;
    int pos1;
    //int x1, y1, x2, y2;
    //get_clip_rect(screen, &x1, &y1, &x2, &y2);

    x_length=mouse_x-x0;
    if (x_length<-width_w) return FALSE;

    if (x_length<0) x_length=0;

    y_position=mouse_y-y0;
    if (y_position<0) return FALSE;

    //pos1 = utf8len(text);
    pos1 = strlen(text);

    if (!rend_UI) return FALSE;

    if (text == NULL) return FALSE;

    strncpy(buf, text, (MaxTextLen*2));

    gk_rend_set_size_pixels(rend_UI, WIDTH*ttf_h, HEIGHT*ttf_v); //
    gk_rend_set_bold_strength(rend_UI, 50);
    gk_text_size_utf8(rend_UI, buf, &text_h, &text_v);

    y_position=y0+text_v-mouse_y;
    if (y_position<0) return FALSE;

    if (mouse_x>(x0+text_h+width_w)) return FALSE;

    while (text_h>x_length)
    {
        if (pos1 > 0)
        {
            if (buf[pos1 - 1] > 127)
                pos1 -= 2;
            else pos1--;
            buf[pos1]='\0';
            gk_text_size_utf8(rend_UI, buf, &text_h, &text_v);
        }
        else
        {
            text_h=0;
        }
    }
    *pos=pos1;
    return TRUE;
}

int my_text_length(FONT *font, const char *text)
{
	return TTF_text_len(text);
}

void TTF_text_UI_(BITMAP *ui_screen, const char *text, int x, int y, int *text_h, int *text_v)
{
	
	int color;
	COLOR_ kolor;
	unsigned char red, green, blue;
	int horiz, vert;

	char *ptr;

	if (!face_UI) return;
	if (!rend_UI) return;
	gk_rend_set_size_pixels(rend_UI, WIDTH*ttf_h, HEIGHT*ttf_v);
	
	getcolor_RGB_char(&red, &green, &blue, getcolor());

	gk_rend_set_text_color(rend_UI, red, green, blue);
	gk_rend_set_bold_strength(rend_UI, 50);
	gk_text_size_utf8(rend_UI, text, text_h, text_v);

	gettextjustify(&horiz, &vert);

	if (horiz == 1) x = x - *text_h / 2;
	else if (horiz == 2) x = x - *text_h;
	
	gk_render_line_utf8(ui_screen, rend_UI, text, x, y + HEIGHT - 3);
}

void TTF_text_UI(const char *text, int x, int y, int *text_h, int *text_v)
{
    TTF_text_UI_(screen, text, x, y, text_h, text_v);
}

void TTF_text_UI_W_H_(BITMAP *ui_screen, const char* text, int x, int y, int* text_h, int* text_v, int WIDTH__, int HEIGHT__)
{
	int color;
	COLOR_ kolor;
	unsigned char red, green, blue;
	int horiz, vert;

	char* ptr;

	if (!face_UI) return;
	if (!rend_UI) return;
	gk_rend_set_size_pixels(rend_UI, WIDTH__ * ttf_h, HEIGHT__ * ttf_v);

	getcolor_RGB_char(&red, &green, &blue, getcolor());

	gk_rend_set_text_color(rend_UI, red, green, blue);
	gk_rend_set_bold_strength(rend_UI, 50);
	gk_text_size_utf8(rend_UI, text, text_h, text_v);

	gettextjustify(&horiz, &vert);

	if (horiz == 1) x = x - *text_h / 2;
	else if (horiz == 2) x = x - *text_h;

	gk_render_line_utf8(ui_screen, rend_UI, text, x, y + HEIGHT__ - 3);
}

void TTF_text_UI_W_H(const char* text, int x, int y, int* text_h, int* text_v, int WIDTH__, int HEIGHT__)
{
   TTF_text_UI_W_H_(screen, text, x, y, text_h, text_v, WIDTH__, HEIGHT__);
}

void TTF_text_test(BITMAP *bmp, char *text, int x, int y)
{
	GLYPH_FACE *face;
	GLYPH_REND *rend;
	int text_v, text_h;

	face = gk_load_face_from_file("DejaVuSans.ttf", 0);
	if (!face) return;
	rend = gk_create_renderer(face, 0);
	if (!rend) return;

	gk_rend_set_size_pixels(rend, 200, 800);
	gk_rend_set_text_color(rend, 252, 0, 0);
	
	gk_text_size_utf8(rend, text, &text_h, &text_v);
	
	gk_render_line_utf8(bmp, rend, text, x, y);

	gk_done_renderer(rend);
	gk_unload_face(face);
}

void TTF_logo(int x, int y)
{
	GLYPH_FACE *face1;
	GLYPH_REND *rend1;
	BITMAP *bmp;
	int text_v, text_h;
	char *logo = "AlfaCAD";

	face1 = gk_load_face_from_file("DejaVuSans.ttf", 0);
	if (!face1) return;
	rend1 = gk_create_renderer(face1, 0);
	if (!rend1) return;

	gk_rend_set_size_pixels(rend1, HEIGHT*4, WIDTH*9);
	gk_rend_set_text_color(rend1, 255,0,0);
	gk_rend_set_text_alpha(rend1, 20);
	gk_rend_set_bold_strength(rend1, 200);
	gk_text_size_utf8(rend1, logo, &text_h, &text_v);
	gk_render_line_utf8(screen, rend1, logo, x- text_h/2, y- text_v/2 - HEIGHT);

	gk_unload_face(face1);
	gk_done_renderer(rend1);
}

void Free_winvar(void)
{
	if (winvar!=NULL) free(winvar);
}

void Free_Desktop_font()
{
	int gk;
	if (face_UI != NULL)
	{
		gk_unload_face(face_UI);
		gk_done_renderer(rend_UI);
		
		rend_UI = NULL;
		face_UI = NULL;
	}
}

void Free_ini_font()
{
	int gk;
	
		if (rend_ini != NULL)
		   gk_done_renderer(rend_ini);
		
		rend_ini = NULL;
}

void Save_Desktop_font(char *font_name)
{
	FILE *f_ini;
	//saving in file font.ini
	FONTNUMBER = 0;

	f_ini = fopen(font_file_name, "wt");
	fprintf(f_ini, "%s\n", font_name);
	fprintf(f_ini, "%d,%d,%d,%d,%d\n", MP_SIZE, ED_INF_HEIGHT, BAR_G, HEIGHT, WIDTH);
	
	fclose(f_ini);
}

void set_ttf_digits27_len(void)
{
	ttf_digits27_len = TTF_text_len("99.999999999; 99.999999999");
	ttf_digits13_len = TTF_text_len("99.999999999 ");
	ttf_width_w = (float)(TTF_text_len("WWWWWWWWWWWWWWWWWWWW")) / 20.0;
    PL266 = 50 * TTF_text_len("0");
    PL366 = 75 * TTF_text_len("0");
}

void Initialize_alft(void)
{
	alft = (char*)malloc(roz_in);
}

void Initialize_Desktop_font(char *font_name)
{
	size_t requiredSize;
	int color;
	COLOR_ kolor;
	unsigned char red, green, blue;
	int horiz, vert;

	char dir[MAXDIR];
	char drive[MAXDRIVE];
	char file[MAXFILE];
	char ext[MAXEXT];
	int flags;

#ifndef LINUX
	getenv_s(&requiredSize, NULL, 0, "WINDIR");
	if (requiredSize == 0)
	{
		winfont = NULL;
	}
#else
    requiredSize=255;
#endif

	winvar = (char*)malloc(requiredSize + 7 * sizeof(char));
	if (!winvar) exit(0);

#ifndef LINUX
	getenv_s(&requiredSize, winvar, requiredSize, "WINDIR");
	strcat(winvar, "\\FONTS\\");
#else
    const char *homedir;

    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    sprintf(winvar, "%s/WinFonts/TTF/", homedir);
#endif

    if (strlen(default_path_TTF)>0) winfont=&default_path_TTF;
    else winfont = winvar;  //if IC_TTF_FONTS doesn't exists

    if (strlen(default_path_OTF)>0) otffont=&default_path_OTF;

	gk_set_font_path(winfont);

	face_UI = gk_load_face_from_file(font_name, 0); 
	if (!face_UI) {
        printf("%s\n", "Cannot find last desktop font file. Setting up default one...");
        face_UI = gk_load_face_from_file("DejaVuSans.ttf", 0);
    }
    if (!face_UI)
    {
        printf("%s\n","Cannot find default desktop font file. Exiting...");
        exit(0);
    }

    if (!keep_UI) keep_UI = gk_create_keeper(1890, 8 * 1024 * 1024);
	
	rend_UI = gk_create_renderer(face_UI, keep_UI);
	if (!rend_UI) return;

	gk_rend_set_size_pixels(rend_UI, HEIGHT*0.85, HEIGHT*0.9);
	getcolor_RGB_char(&red, &green, &blue, getcolor());
	gk_rend_set_text_color(rend_UI, red, green, blue);
	
	gk_rend_set_bold_strength(rend_UI, 50);

	gk_rend_set_text_alpha(rend_UI, 255);

	gk_rend_set_error_char(rend_UI, 0);
	gk_rend_set_undefined_char(rend_UI, 0);

	flags = fnsplit(font_name, drive, dir, file, ext);
	strcat(file, ext);
	strcpy(Czcionka_Pulpitu, file);
	strcpy(Desktop_Font_File, font_name);

	find_font_face(Desktop_Font_File);

	set_ttf_digits27_len();
}

void Set_Desktop_font(char *font_name)
{
	int color;
	COLOR_ kolor;
	unsigned char red, green, blue;
	int horiz, vert;
	
	gk_unload_face(face_UI);
	gk_done_renderer(rend_UI);

	face_UI = gk_load_face_from_file(font_name, 0);  //"DejaVuSans.ttf"
	
	if (!face_UI) return;

	if (!keep_UI) keep_UI = gk_create_keeper(1890, 8 * 1024 * 1024);
	
	rend_UI = gk_create_renderer(face_UI, keep_UI);
	if (!rend_UI) return;

	gk_rend_set_size_pixels(rend_UI, HEIGHT*0.85, HEIGHT*0.9);
	getcolor_RGB_char(&red, &green, &blue, getcolor());
	gk_rend_set_text_color(rend_UI, red, green, blue);
	
	gk_rend_set_bold_strength(rend_UI, 50);
	
	gk_rend_set_text_alpha(rend_UI, 255);

	gk_rend_set_error_char(rend_UI, 0);
	gk_rend_set_undefined_char(rend_UI, 0);
	set_ttf_digits27_len();
}

void extra_logo(int x, int y, int option, char *file_name)
{
	GLYPH_FACE *face;
	GLYPH_REND *rend;
	BITMAP *bmp;
	int text_v, text_h;
    int yextra=0;
    int h_size, v_size;
    char message[MaxTextLen];

    h_size=36*WIDTH/14;
    v_size=54*HEIGHT/24;

	face = gk_load_face_from_file("DejaVuSans.ttf", 0);
	if (!face) return;
	rend = gk_create_renderer(face, 0);
	if (!rend) return;

    sprintf(message,"%s%s", loading_program[option], file_name);

	gk_rend_set_size_pixels(rend, h_size, v_size);
	gk_rend_set_text_alpha_color(rend, 0xFF186DFF); //0x9F186DFF
	gk_text_size_utf8(rend, message, &text_h, &text_v);
    yextra=32-HEIGHT;
	gk_render_line_utf8(screen, rend, message, x - text_h / 2, y - text_v / 2 +(5-option*2)*HEIGHT + yextra);

	gk_unload_face(face);
	gk_done_renderer(rend);
}

typedef struct
{
	BITMAP **png_b;
	char* png_file;
	char** png_p;
} BITMAP_LOAD;

BITMAP *client_bitmap_load[MAX_CLIENT_BITMAP]={NULL};

BITMAP *load_png_(char* png_file, PALETTE pal)
{
	char bitmap_file[MAXPATH];
	sprintf(bitmap_file, "%s/%s", _BITMAPS_, png_file);
	return load_png(bitmap_file, pal);
}

//BITMAP* 
void load_png__(BITMAP_LOAD *png_load)
{
	PALETTE pal;
	char bitmap_file[MAXPATH];
	sprintf(bitmap_file, "%s/%s.png", _BITMAPS_, png_load->png_file);
	png_load->png_b=load_png(bitmap_file, pal);
	png_load->png_p = (char*)png_load->png_b;
}

void load_png___(BITMAP **png_b, char *png_file, char **png_p)
{
    PALETTE pal;
    char bitmap_file[MAXPATH];
    sprintf(bitmap_file, "%s/%s.png", _BITMAPS_, png_file);
    *png_b = load_png(bitmap_file, pal);
    *png_p = &png_b;
}

char *get_XDG_CURRENT_DESKTOP(void)
{
    return XDG_CURRENT_DESKTOP;
}

void Load_New_Files(struct shmbuf *bufptr)
{   int ii, neb;
    if (bufptr->mflag==1)
    {
        //checking other flags
        for (ii=0; ii<FILENO; ii++)
        {
            if (bufptr->flag[ii]==1) break;
        }
        if (ii==FILENO) //no more files
            bufptr->mflag=0;
        else
        {
            neb=get_empty_buffer();
            if (neb>-1)
            {
                bufptr->flag[ii] = 0;
                New_window_factory(neb, bufptr->file_name[ii]);
            }
            else
            {
                bufptr->flag[ii]=0; //ignored
            }
        }
        if (bufptr->mflag==0) Expand_flex();
    }
}

int load_client_bitmap(char *bitmap_file) {
    char client_bitmap_file[MAXPATH];
    BITMAP* cbt;
    PALETTE pal;
    int i;

    //searching for free slot

    for (i=0; i<MAX_CLIENT_BITMAP; i++)
    {
        if (client_bitmap_load[i]==NULL) break;
    }
    if (i==MAX_CLIENT_BITMAP) return 0;  //empty icon

    sprintf(client_bitmap_file, "%s/%s", _PLUGIN_BITMAPS_, bitmap_file);
    cbt = load_png(client_bitmap_file, pal);
    client_bitmap_load[i] = cbt;

    return i+FIRST_CLIENT_BITMAP_NO;
}

int delete_client_bitmap(int iconno) {
    printf("%d\n", iconno);
    if (client_bitmap_load[iconno-FIRST_CLIENT_BITMAP_NO]!=NULL) {
        destroy_bitmap(client_bitmap_load[iconno-FIRST_CLIENT_BITMAP_NO]);
        client_bitmap_load[iconno-FIRST_CLIENT_BITMAP_NO]=NULL;
    }
    return 0;
}

int delete_all_client_bitmaps(void)
{
    for (int i=0; i<MAX_CLIENT_BITMAP; i++)
    {
        if (client_bitmap_load[i]!=NULL)
        {
            destroy_bitmap(client_bitmap_load[i]);
            client_bitmap_load[i]=NULL;
        }
    }
}


void set_geometry(int single)
{   int x_edit;
    int y_edit;
    int curr_x01, curr_y01, curr_h1, curr_v1;
    get_window_origin_and_size_(&curr_x01, &curr_y01, &curr_h1, &curr_v1);
    if (!single) {
        x_edit = curr_x01 + 200;
        y_edit = curr_y01 + 200;
#ifdef LINUX
        set_editbox_geometry(x_edit, y_edit);
#else
        set_editbox_geometry_win(x_edit, y_edit);
#endif
        set_editbox_geometry_set();
    }
else {
        x_edit = curr_x01 + 2;

        y_edit = curr_y01 + Get_X11_SCREEN_SHIFT() + ED_INF_HEIGHT * 2;
#ifdef LINUX
        set_editbox_geometry_line(x_edit, y_edit);
#else
        y_edit -= Get_WIN_WINDOW_T_B();
          set_editbox_geometry_line_win(x_edit, y_edit);
#endif
        set_editbox_geometry_line_set();
    }
}

void DoneArgs(void)
{

#ifndef NDEBUG
#undef free
#endif
    for(int i = argc_-1; i >=0; i--) {
        if (argv_[i] != NULL)
            free(argv_[i]);
    }

    free(argv_);
#ifndef NDEBUG
#define  free(mem_ref) 		  	 	xfree(mem_ref)
#endif
}

#ifndef LINUX
int Rysuj_main(int child, char file_name[255], int nCmdShow, char *application, char *arg_params)
{
#else
int main(int argc, char *argv[])
{
    int child=0;
    char file_name[255]="";
    int nCmdShow=0;

    XDG_CURRENT_DESKTOP = getenv("XDG_CURRENT_DESKTOP");
   
#endif
  struct shmbuf* bufptr;
  int uformat;
  BOOL drv_ok;
  int n1, schowek;
  TMENU *menu=&menug;
  TA *COMND=&COMNDg;
  void (*FF)(void)=nooop;
  int gdriver, gmode;
  int err;
  int lomode, himode;

  int i,ff;
  int col1;
  
  int x_cent, y_cent;
 
  int kk;
  
  int disk;
  
  int p_mouse;
  PALETTE pal;
  int nbuttons;

  char buf[MAXLINE];
  char *p;
  FILE *f_ini;
  
  int c_256;
  
  int ret;
  int number_of_clients;

  static int(*SW[SVMAX])(void);
  BITMAP *bitmap_ptr;
  BOOL NOCHDIR;
  int ii, neb;


#ifndef FORWIN32  
 _crt0_startup_flags = _crt0_startup_flags | _CRT0_FLAG_FILL_SBRK_MEMORY;
#endif


#ifndef NDEBUG
#undef malloc
#endif

#ifdef LINUX
    // allocate memory and copy strings
    argv_ = malloc((argc+1) * sizeof *argv_);
    for(int i = 0; i < argc; ++i)
    {
        size_t length = strlen(argv[i])+1;
        argv_[i] = malloc(length);
        memcpy(argv_[i], argv[i], length);
    }
    argv_[argc] = NULL;
    argc_=argc;
#else
 
 char* ptr0;
 char* ptr;
 argc_ = 1;
 ptr = strchr(arg_params, ' ');
 while (ptr != NULL)
 {
     argc_++;
     ptr= strchr(ptr+1, ' ');
 }

 argv_ = malloc((argc_ + 2) * sizeof * argv_);  //argv_[0] is reserved for application name
 i = 0;
 size_t length = strlen(application) + 1;
 argv_[i] = malloc(length);
 memcpy(argv_[i], application, length);
 i++;
 ptr0 = arg_params;
 ptr = strchr(arg_params, ' ');
 if (ptr == NULL)
 {
     size_t length = strlen(ptr0) + 1;
     argv_[i] = malloc(length);
     memcpy(argv_[i], ptr0, length);
     i++;
 }
 else
 {
     *ptr = '\0';
     while (ptr0 != NULL)
     {
         size_t length = strlen(ptr0) + 1;
         argv_[i] = malloc(length);
         memcpy(argv_[i], ptr0, length);
         if (ptr != NULL)
         {
             ptr0 = ptr + 1;
             ptr = strchr(ptr0, ' ');
         }
         else ptr0 = NULL;
         i++;
     }
 }
 argv_[i] = NULL;
 argc_ = i;

 argp__ = malloc(strlen(application) + 10);
 strcpy(argp__, application);

#endif

#ifndef NDEBUG
#define  malloc(size) 	    	   	xmalloc (size, __FILE__, __LINE__)
#endif

#ifdef LINUX
 NOCHDIR = FALSE;
 for (int i=1; i<argc; i++)
 {
     if (strcmp(argv[i], "--NOCHDIR")==0) NOCHDIR = TRUE;
 }

    char strAppPath[MAXPATH];
    char *ptr_s;

#ifdef NDEBUG
    //changing folder - for file type association program purpose
    if (NOCHDIR == FALSE)
    {
        strcpy(strAppPath, argv[0]);
        ptr_s = strrchr(&strAppPath,'/');
        if (ptr_s!=NULL) *ptr_s='\0';
        printf("%s\n",strAppPath);
        int ret1 = chdir(&strAppPath);
        if (ret1==-1)
        {
            printf("%s\n","Wrong name of AlfaCAD folder");
            exit(0);
        }
    }
#endif
   Set_XWindow_header_height();
#endif

mynCmdShow = nCmdShow;

disk = getdisk();
start_disk = disk;

strcpy(argv0,"");

 srand((unsigned int)time(0));

#ifdef LINUX
    for (i=1; i<argc; i++) {
        if (strcmp(argv[i], "--NOCHDIR")!=0)
        {
            strcpy(file_name, argv[i]);
            break;
        }
    }

#endif

Test_App(1, file_name);  //reading Client_number

number_of_clients = Number_of_clients;
if (number_of_clients > 1) child = 1;

#ifndef ALF_MAIN2
if (child == 0) Initial_Message(file_name);
else if ((child == 1) || (child == 3)) Child_Message(0);
else if ((child == 2) || (child == 4)) Child_Message(1);
#endif

#ifndef LINUX
ret = GoRegtestCall(testCall);
#endif

sprintf(RYSUJ$1,"RYSUJ$.%03ld",Client_number);
sprintf(HATCH_TEMP_FILE,"ALFHATCH.%03ld",Client_number);

music_avail=FALSE; 

set_uformat(U_UTF8);

int errno;

allegro_init();

 ret= set_window_icon();

 //install_timer();
 nbuttons = install_mouse();

 three_finger_flag=FALSE;

 char const** sections = NULL;
 char const** entries = NULL;
 char konf[10][10][32];
 
 install_keyboard(); 
 install_timer();
 
 poll_kbd=keyboard_needs_poll(); 
 poll_m = mouse_needs_poll();

 p_mouse = my_poll_mouse();

 if (!GrMouseDetect()) 
  {

  printf("\n");printf(_NO_MOUSE_);

  exit(1);
  }

 if (__file_exists("blk00000.alx")) schowek=unlink("blk00000.alx");

  Ini_Sys_Param () ;	/*ustawia bufor makra, typ myszy i tryb SVGA z pliku alfacad.ini*/

  gdriver = DETECT;
  gmode = VGAHI;
  ff=open("COLORS.CNF",O_RDONLY|O_BINARY);  //O_RDWR
  if (ff==-1)
   {
     ;
   }
  else 
  {
    for (i=0; i<19; i++) 
	{
     read(ff, &col1, sizeof(int));
	}

    close(ff);
  }
  
#ifdef fullcolor
  c_256=65535; //16536;
  c_depth=16;
#else
  c_depth=get_desktop_color_depth(); 
  c_256=pow(2,c_depth);
  
#endif

drv_ok=get_master_data(&drv_cur);
if (drv_ok==TRUE)  memcpy(&drv_master, &drv_cur, sizeof(DRIVER_STRUCT));
drv_ok=get_slave_data(&drv_cur);
if (drv_ok==TRUE)  memcpy(&drv_slave, &drv_cur, sizeof(DRIVER_STRUCT));


sk_x_drv_master=drv_master.gfx_aspect;
sk_x_drv_slave=drv_slave.gfx_aspect;

if (sk_x_drv_master <= .01)
  {
    sk_x_drv_master = 1 ;
  }

if (sk_x_drv_slave <= .01)
  {
    sk_x_drv_slave = 1 ;
  }

if ((child==3) || (child==4))  //slave
{
  sk_x=sk_x_drv_slave;
  graphic_drv=DRV_SLAVE;   //0

  c_depth=drv_slave.gfx_bpp;
  c_256=pow(2,c_depth);

  change_mode_gr(&drv_slave);
}
else //master
{
  sk_x=sk_x_drv_master;
  graphic_drv=DRV_MASTER;   //0

  c_depth=drv_master.gfx_bpp;
  c_256=pow(2,c_depth);

  change_mode_gr(&drv_master);
}

  lock_mouse_switch();  //IT MUST BE IN WINDOWS, NEED TO BE CHECKED IN LINUX
  
  Copy_screen();

  Load_Data();

  create_second_screen();

  setrgbdefaults();

  mysetrgbdefaults();
  check_tables();

  set_default_background();
  if (strlen(background_pcx_file)>0)  
	   set_special_background(background_pcx_file);
  

  f_ini = fopen ( font_file_name , "rt" ) ;
   if ( f_ini == NULL )
    {
      strcpy(font_name,"DejaVuSans.ttf");
	  strcpy(Czcionka_Pulpitu, font_name);
      MP_SIZE=BAR_G=HEIGHT=19;
      ED_INF_HEIGHT = HEIGHT + 2; //1;
      WIDTH=10;
      f_ini = fopen ( font_file_name , "wt" ) ;
      fprintf(f_ini,"%s\n",font_name);
#ifdef LINUX
       fprintf(f_ini,"%d,%d,%d,%d,%d\n",MP_SIZE,ED_INF_HEIGHT,BAR_G,HEIGHT,WIDTH);
#else
      fprintf(f_ini,"%ld,%ld,%ld,%ld,%ld\n",MP_SIZE,ED_INF_HEIGHT,BAR_G,HEIGHT,WIDTH);
#endif
      fclose(f_ini);
    }
   else
   {
	   FONTNUMBER = 0;
	   if (fgets(font_name, MAXLINE, f_ini) == NULL)
	   {
		  fclose(f_ini);
          reset_font();
	   }
	   else
	   {
		   font_name[strlen(font_name) - 1] = '\0';
           if (font_name[strlen(font_name) - 1]=='\r')
               font_name[strlen(font_name) - 1] = '\0';
		   if (fgets(buf, MAXLINE, f_ini) == NULL)
		   {
			   fclose(f_ini);
			   reset_font();
		   }
		   else
		   {
			   buf[strlen(buf) - 1] = '\0';
			   p = buf;
#ifdef LINUX
               if (sscanf(p, "%d,%d,%d,%d,%d", &MP_SIZE, &ED_INF_HEIGHT, &BAR_G, &HEIGHT, &WIDTH) < 5)
#else
			   if (sscanf(p, "%ld,%ld,%ld,%ld,%ld", &MP_SIZE, &ED_INF_HEIGHT, &BAR_G, &HEIGHT, &WIDTH) < 5)
#endif
			   {
				   MP_SIZE = BAR_G = HEIGHT = 19;
                   ED_INF_HEIGHT = HEIGHT + 2; //1;
				   WIDTH = 10;
			   }
			   if (HEIGHT==0) HEIGHT=ED_INF_HEIGHT-2; //1;
			}
	   }
      }
 

    Initialize_Desktop_font(font_name);
    
	Initialize_alft();

	char dir[MAXDIR];
	char drive[MAXDRIVE];
	char file[MAXFILE];
	char ext[MAXEXT];
	int flags;

	flags = fnsplit(font_name, drive, dir, file, ext);
	strcat(file, ext);
	strcpy(Czcionka_Pulpitu, file); // font_name);


    int png_mem = 32 * 32 * 4 + 100;
    int png_mem64 = 64 * 64 * 4 + 100;
    icon_no_dmem = load_memory_png(icon_no_d_pm, png_mem, (RGB *)&pal);
    icon_yes_dmem = load_memory_png(icon_yes_d_pm, png_mem, (RGB *)&pal);
    icon_upgrademark_mem = load_memory_png(icon_upgrademark_d_pm, png_mem64, (RGB*)&pal);
    icon_noupgrademark_mem = load_memory_png(icon_noupgrademark_d_pm, png_mem64, (RGB*)&pal);

    icon_hourglass_mem = load_memory_png(icon_hourglass_d_pm, png_mem, (RGB*)&pal);

    icon_no_d_pmem = icon_no_dmem;
    icon_yes_d_pmem = icon_yes_dmem;
    icon_upgrademark_pmem = icon_upgrademark_mem;
    icon_noupgrademark_pmem = icon_noupgrademark_mem;

if (child==0)
{
    ret=alfacad_logo();

    if (ret==255)
    {
        GoRestart();
        allegro_exit();
#ifdef LINUX
        execv(argv_[0], argv_);
#else
        _execv(argp__, argv_);
        return 1;
#endif
        DoneArgs();
        return 1;
    }

  Alf_window_number=0;

  setcolor(129);

  x_cent=(getmaxx()/2);
  y_cent=(getmaxy()/2) + 6*HEIGHT;
  ggetmaxx=getmaxx();
  ggetmaxy=getmaxy();
  moveto(x_cent, y_cent);
  position_mouse(x_cent, y_cent);
  settextjustify (CENTER_TEXT, TOP_TEXT) ;

  extra_logo(x_cent, y_cent, 0, "");

}
  else 
{
	Alf_window_number = 1;
	delay(500);

	setcolor(129);

	x_cent = (getmaxx() / 2);
	y_cent = (getmaxy() / 2) + 7 + 100;
	ggetmaxx = getmaxx();
	ggetmaxy = getmaxy();
	
	moveto(x_cent, y_cent);
    position_mouse(x_cent, y_cent);
	settextjustify(CENTER_TEXT, TOP_TEXT);


    Set_Screen();
	extra_logo(x_cent, y_cent, 1, "");

}
  
  ini_e();

#ifndef LINUX
  init_file_dropped_fill_buf();
#endif;

  //reseting client bitmaps
  for (i = 0; i < MAX_CLIENT_BITMAP; i++) client_bitmap_load[i]=NULL;

  ////PLUGINS
  ret=alfaplugins();

#ifdef LINUX
  hide_x_cursor();
#endif

  uformat=get_uformat();

  __djgpp_set_ctrl_c(0);       //blokowanie Ctrl-C

  b_first_open = TRUE;
  
  makro_esc();

  char bitmap_file[MAXPATH];

  BITMAP_LOAD bitmap_load[] = {
	  {&qmark,"qmark64",&qmark_p },
	  {&eymark,"eymark64",&eymark_p},
	  {&ermark,"ermark64",&ermark_p},
	  {&offmark,"offmark64",&offmark_p},
	  {&plotter,"plotter",&plotter_p},
	  {&icon_draft,"draft",&icon_draft_p},
	  {&icon_block,"create_block",&icon_block_p},  
	  {&icon_modify,"modify",&icon_modify_p},
	  {&icon_search,"search",&icon_search_p},
	  {&icon_explode_text_to_letters,"explode_text_to_letters",&icon_explode_text_to_letters_p},
	  {&icon_dim_horizontal,"dim_horizontal",&icon_dim_horizontal_p},
	  {&icon_hatch,"hatch",&icon_hatch_p},
	  {&icon_polygonal_aread,"polygonal_area",&icon_polygonal_aread_p},
	  {&icon_create_macro,"create_macro",&icon_create_macro_p},
	  {&icon_drawing_settings,"drawing_settings",&icon_drawing_settings_p},
	  {&icon_settings,"settings",&icon_settings_p},
	  {&icon_open,"open",&icon_open_p},
	  {&icon_quit,"quit",&icon_quit_p},
	  {&icon_line,"line",&icon_line_p},
      {&icon_circle,"circle",&icon_circle_p},
	  {&icon_arc,"arc",&icon_arc_p},
	  {&icon_text,"text",&icon_text_p},
	  {&icon_polygon,"polygon",&icon_polygon_p},
	  {&icon_polygon_n,"polygon_n",&icon_polygon_n_p},
	  {&icon_disc,"disc",&icon_disc_p},
	  {&icon_ellipses,"ellipses",&icon_ellipses_p},
	  {&icon_polylines,"polylines",&icon_polylines_p},
	  {&icon_solid,"solid",&icon_solid_p},
	  {&icon_line_rectangle,"line_rectangle",&icon_line_rectangle_p},
	  {&icon_trace,"trace",&icon_trace_p},
	  {&icon_2line,"2line",&icon_2line_p},
	  {&icon_3line,"3line",&icon_3line_p},
	  {&icon_line_freehand,"line_freehand",&icon_line_freehand_p},
	  {&icon_point,"point",&icon_point_p},
	  {&icon_chart,"chart",&icon_chart_p},
	  {&icon_reinforcing,"reinforcing",&icon_reinforcing_p},
	  {&icon_move,"move",&icon_move_p},
	  {&icon_move_copy,"move_copy",&icon_move_copy_p},
	  {&icon_delete,"delete",&icon_delete_p},
	  {&icon_restore,"restore",&icon_restore_p},
	  {&icon_rotate,"rotate",&icon_rotate_p},
	  {&icon_rotate_block,"rotate_block",&icon_rotate_block_p},
	  {&icon_scale,"scale",&icon_scale_p},
	  {&icon_stretch,"stretch",&icon_stretch_p},
	  {&icon_edit_text,"edit_text",&icon_edit_text_p},
	  {&icon_mirror,"mirror",&icon_mirror_p},
	  {&icon_mirror_leave,"mirror_leave",&icon_mirror_leave_p},
	  {&icon_mirror_replace,"mirror_replace",&icon_mirror_replace_p},
	  {&icon_mirror_block_x,"mirror_block_x",&icon_mirror_block_x_p},
	  {&icon_mirror_block_y,"mirror_block_y",&icon_mirror_block_y_p},
	  {&icon_array,"array",&icon_array_p},
	  {&icon_change,"change",&icon_change_p},
	  {&icon_trim_segments,"trim_segments",&icon_trim_segments_p},
	  {&icon_cutout,"cutout",&icon_cutout_p},
	  {&icon_extend,"extend",&icon_extend_p},
	  {&icon_fillet,"fillet",&icon_fillet_p},
	  {&icon_chamfer,"chamfer",&icon_chamfer_p},
	  {&icon_offset,"offset",&icon_offset_p},
	  {&icon_divide,"divide",&icon_divide_p},
	  {&icon_undo,"undo",&icon_undo_p},
      {&icon_pointfinder,"pointfinder",&icon_pointfinder_p},
      {&icon_camera,"camera",&icon_camera_p},
      {&icon_parameters,"parameters",&icon_parameters_p},
      {&icon_layers,"layers",&icon_layers_p},
      {&icon_measure,"measure",&icon_measure_p},
      {&icon_empty,"empty",&icon_empty_p},
      {&icon_copy,"copy",&icon_copy_p},
      {&icon_paste,"paste",&icon_paste_p},
      {&icon_import_block,"import_block",&icon_import_block_p},
      {&icon_export_block,"export_block",&icon_export_block_p},
      {&icon_import_image,"import_image",&icon_import_image_p},
      {&icon_import_png,"import_png",&icon_import_png_p},
      {&icon_import_jpg,"import_jpg",&icon_import_jpg_p},
      {&icon_edit_image,"edit_image",&icon_edit_image_p},
      {&icon_export_image,"export_image",&icon_export_image_p},
      {&icon_group,"create_block",&icon_group_p},
      {&icon_explode,"explode",&icon_explode_p},
      {&icon_block_list,"block_list",&icon_block_list_p},
      {&icon_explode_frozen,"explode_frozen",&icon_explode_frozen_p},
      {&icon_delete_dxf_blocks,"delete_dxf_blocks",&icon_delete_dxf_blocks_p},
      {&icon_correct_blocks,"correct_blocks",&icon_correct_blocks_p},
      {&icon_fixed_layers,"fixed_layers",&icon_fixed_layers_p},
      {&icon_area,"area",&icon_area_p},
      {&icon_center_of_area,"center_of_area",&icon_center_of_area_p},
      {&icon_first_moment_of_area,"first_moment_of_area",&icon_first_moment_of_area_p},
      {&icon_area_moment_of_inertia,"area_moment_of_inertia",&icon_area_moment_of_inertia_p},
      {&icon_centr_area_mom_of_inertia,"centr_area_mom_of_inertia",&icon_centr_area_mom_of_inertia_p},
      {&icon_test_of_closed_area,"test_of_closed_area",&icon_test_of_closed_area_p},
      {&icon_import,"import",&icon_import_p},
      {&icon_new,"new",&icon_new_p},
      {&icon_new_window,"new_window",&icon_new_window_p},
      {&icon_save,"save",&icon_save_p},
      {&icon_save_as,"save_as",&icon_save_as_p},
      {&icon_export_dxf,"export_dxf",&icon_export_dxf_p},
      {&icon_import_dxf,"import_dxf",&icon_import_dxf_p},
      {&icon_export_cad,"export_cad",&icon_export_cad_p},
      {&icon_import_cad,"import_cad",&icon_import_cad_p},
      {&icon_export_dwg,"export_dwg",&icon_export_dwg_p},
      {&icon_import_dwg,"import_dwg",&icon_import_dwg_p},
      {&icon_edit_title_block,"edit_title_block",&icon_edit_title_block_p},
      {&icon_cmd,"cmd",&icon_cmd_p},
      {&icon_print,"print",&icon_print_p},
      {&icon_infobox,"Infobox",&icon_infobox_p},
      {&icon_last_files,"last_files",&icon_last_files_p},
      {&icon_change_texts,"change_texts",&icon_change_texts_p},
      {&icon_auto_numbering,"auto_numbering",&icon_auto_numbering_p},
      {&icon_record,"record",&icon_record_p},
      {&icon_play,"play",&icon_play_p},
      {&icon_load,"load",&icon_load_p},
      {&icon_save_as,"save_as",&icon_save_as_p},
      {&icon_format,"format",&icon_format_p},
      {&icon_dscale,"dscale",&icon_dscale_p},
      {&icon_accuracy,"accuracy",&icon_accuracy_p},
      {&icon_units,"units",&icon_units_p},
      {&icon_coordinates,"coordinates",&icon_coordinates_p},
      {&icon_local,"local",&icon_local_p},
      {&icon_pointer,"pointer",&icon_pointer_p},
      {&icon_edit_ini,"edit_ini",&icon_edit_ini_p},
      {&icon_cartesian,"cartesian",&icon_cartesian_p},
      {&icon_geodetic,"geodetic",&icon_geodetic_p},
      {&icon_point_origin,"point_origin",&icon_point_origin_p},
      {&icon_axis_angle,"axis_angle",&icon_axis_angle_p},
      {&icon_rotation_angle,"rotation_angle",&icon_rotation_angle_p},
      {&icon_colorwheel,"Colorwheel",&icon_colorwheel_p},
      {&icon_panoramic,"panoramic",&icon_panoramic_p},
      {&icon_font_type,"font_type",&icon_font_type_p},
      {&icon_yes,"yes",&icon_yes_p},
      {&icon_no,"no",&icon_no_p},
      {&icon_dx_dy,"dx_dy",&icon_dx_dy_p},
      {&icon_calibrate,"calibrate",&icon_calibrate_p},
      {&icon_A_rectangular,"A_rectangular",&icon_A_rectangular_p},
      {&icon_A_circular,"A_circular",&icon_A_circular_p},
      {&icon_A_circular_rot,"A_circular_rot",&icon_A_circular_rot_p},
      {&icon_A_circular_n,"A_circular_n",&icon_A_circular_n_p},
      {&icon_A_circular_rot_n,"A_circular_rot_n",&icon_A_circular_rot_n_p},
      {&icon_A_circular_angle,"A_circular_angle",&icon_A_circular_angle_p},
      {&icon_A_circular_rot_angle,"A_circular_rot_angle",&icon_A_circular_rot_angle_p},
      {&icon_pointmode,"pointmode",&icon_pointmode_p},
      {&icon_endpoint,"endpoint",&icon_endpoint_p},
      {&icon_nearest_end,"nearest_end",&icon_nearest_end_p},
      {&icon_middlepoint,"middlepoint",&icon_middlepoint_p},
      {&icon_intersection,"intersection",&icon_intersection_p},
      {&icon_center,"center",&icon_center_p},
      {&icon_perpendicular,"perpendicular",&icon_perpendicular_p},
      {&icon_tangential,"tangential",&icon_tangential_p},
      {&icon_nearest,"nearest",&icon_nearest_p},
      {&icon_onpoint,"onpoint",&icon_onpoint_p},
      {&icon_move2xy,"move2xy",&icon_move2xy_p},
      {&icon_movedxdy,"movedxdy",&icon_movedxdy_p},
      {&icon_move_polar,"move_polar",&icon_move_polar_p},
      {&icon_nopointfinder,"nopointfinder",&icon_nopointfinder_p},
      {&icon_calculator,"calculator",&icon_calculator_p},
      {&icon_point_measure,"point_measure",&icon_point_measure_p},
      {&icon_distance_point_to_point,"distance_point_to_point",&icon_distance_point_to_point_p},
      {&icon_angle_line_to_line,"angle_line_to_line",&icon_angle_line_to_line_p},
      {&icon_plusminus,"plusminus",&icon_plusminus_p},
      {&icon_property_list,"property_list",&icon_property_list_p},
      {&icon_noplusminus,"noplusminus",&icon_noplusminus_p},
      {&icon_addfavourite,"addfavourite",&icon_addfavourite_p},
      {&icon_findfavourite,"findfavourite",&icon_findfavourite_p},
      {&icon_refresh,"refresh",&icon_refresh_p},
      {&icon_refresh_d,"refresh_d",&icon_refresh_d_p},
      {&icon_zoom_all,"zoom_all",&icon_zoom_all_p},
      {&icon_zoom_center,"zoom_center",&icon_zoom_center_p},
      {&icon_auto_panorama,"auto_panorama",&icon_auto_panorama_p},
      {&icon_zoom_1,"zoom_1",&icon_zoom_1_p},
      {&icon_zoom_2,"zoom_2",&icon_zoom_2_p},
      {&icon_zoom_in,"zoom_in",&icon_zoom_in_p},
      {&icon_zoom_out,"zoom_out",&icon_zoom_out_p},
      {&icon_zoom_x,"zoom_x",&icon_zoom_x_p},
      {&icon_zoom_window,"zoom_window",&icon_zoom_window_p},
      {&icon_zoom_drawing,"zoom_drawing",&icon_zoom_drawing_p},
      {&icon_zoom_previous,"zoom_previous",&icon_zoom_previous_p},
      {&icon_pointer_step,"pointer_step",&icon_pointer_step_p},
      {&icon_grid_density,"grid_density",&icon_grid_density_p},
      {&icon_grid_points,"grid_points",&icon_grid_points_p},
      {&icon_restr_ortho,"restr_ortho",&icon_restr_ortho_p},
      {&icon_color,"Color",&icon_color_p},
      {&icon_line_thickness,"line_thickness",&icon_line_thickness_p},
      {&icon_line_type,"line_type",&icon_line_type_p},
      {&icon_point_visibility,"point_visibility",&icon_point_visibility_p},
      {&icon_point_size,"point_size",&icon_point_size_p},
      {&icon_get_color,"get_color",&icon_get_color_p},
      {&icon_get_type,"get_type",&icon_get_type_p},
      {&icon_get_layer,"get_layer",&icon_get_layer_p},
      {&icon_floating_text,"floating_text",&icon_floating_text_p},
      {&icon_floating_block,"floating_block",&icon_floating_block_p},
      {&icon_dim_scale,"dim_scale",&icon_dim_scale_p},
      {&icon_stretch_in_pin,"stretch_in_pin",&icon_stretch_in_pin_p},
      {&icon_break_in_pin,"break_in_pin",&icon_break_in_pin_p},
      {&icon_mask,"mask",&icon_mask_p},
      {&icon_hidden_text_vis,"hidden_text_vis",&icon_hidden_text_vis_p},
      {&icon_image_points,"image_points",&icon_image_points_p},
      {&icon_highlight_layer,"highlight_layer",&icon_highlight_layer_p},
      {&icon_dbuff_image,"dbuff_image",&icon_dbuff_image_p},
      {&icon_text_most_readable,"text_most_readable",&icon_text_most_readable_p},
      {&icon_line_color,"line_color",&icon_line_color_p},
      {&icon_text_color,"text_color",&icon_text_color_p},
      {&icon_text_height,"text_height",&icon_text_height_p},
      {&icon_width,"text_width",&icon_width_p},
      {&icon_italic,"italic",&icon_italic_p},
      {&icon_bold,"bold",&icon_bold_p},
      {&icon_font_type,"font_type",&icon_font_type_p},
      {&icon_accuracy,"accuracy",&icon_accuracy_p},
      {&icon_terminators,"terminators",&icon_terminators_p},
      {&icon_line_offset,"line_offset",&icon_line_offset_p},
      {&icon_text_direction,"text_direction",&icon_text_direction_p},
      {&icon_extension_lines,"extension_lines",&icon_extension_lines_p},
      {&icon_dim_arrowheads,"dim_arrowheads",&icon_dim_arrowheads_p},
      {&icon_dim_ticks,"dim_ticks",&icon_dim_ticks_p},
      {&icon_dim_dots,"dim_dots",&icon_dim_dots_p},
      {&icon_text_parallel,"text_parallel",&icon_text_parallel_p},
      {&icon_text_horizontal,"text_horizontal",&icon_text_horizontal_p},
      {&icon_fixed_length,"fixed_length",&icon_fixed_length_p},
      {&icon_align_to_object,"align_to_object",&icon_align_to_object_p},
      {&icon_dim_direction,"dim_direction",&icon_dim_direction_p},
      {&icon_dim_new,"dim_new",&icon_dim_new_p},
      {&icon_dim_remove,"dim_remove",&icon_dim_remove_p},
      {&icon_dim_edit,"dim_edit",&icon_dim_edit_p},
      {&icon_dim_settings,"dim_settings",&icon_dim_settings_p},
      {&icon_dim_arc_RL,"dim_arc_RL",&icon_dim_arc_RL_p},
      {&icon_dim_circle_DR,"dim_circle_DR",&icon_dim_circle_DR_p},
      {&icon_dim_horizontal,"dim_horizontal",&icon_dim_horizontal_p},
      {&icon_dim_vertical,"dim_vertical",&icon_dim_vertical_p},
      {&icon_dim_aligned,"dim_aligned",&icon_dim_aligned_p},
      {&icon_dim_linear,"dim_linear",&icon_dim_linear_p},
      {&icon_dim_linearX,"dim_linearX",&icon_dim_linearX_p},
      {&icon_dim_angular,"dim_angular",&icon_dim_angular_p},
      {&icon_dim_arc_radial,"dim_arc_radial",&icon_dim_arc_radial_p},
      {&icon_dim_arc_length,"dim_arc_length",&icon_dim_arc_length_p},
      {&icon_dim_diametric,"dim_diametric",&icon_dim_diametric_p},
      {&icon_dim_radial,"dim_radial",&icon_dim_radial_p},
      {&icon_dim_radial_y,"dim_radial_y",&icon_dim_radial_y_p},
      {&icon_dim_radial_y_dim,"dim_radial_y_dim",&icon_dim_radial_y_dim_p},
      {&icon_dim_radial_y_point,"dim_radial_y_point",&icon_dim_radial_y_point_p},
      {&icon_dim_radial_y_tangential,"dim_radial_y_tangential",&icon_dim_radial_y_tangential_p},
      {&icon_circle_center_radius,"circle_center_radius",&icon_circle_center_radius_p},
      {&icon_circle_center_diameter,"circle_center_diameter",&icon_circle_center_diameter_p},
      {&icon_circle_2_points,"circle_2_points",&icon_circle_2_points_p},
      {&icon_circle_3_points,"circle_3_points",&icon_circle_3_points_p},
      {&icon_line_close,"line_close",&icon_line_close_p},
      {&icon_line_remove,"line_remove",&icon_line_remove_p},
      {&icon_trim_value,"trim_value",&icon_trim_value_p},
      {&icon_line_continue,"line_continue",&icon_line_continue_p},
      {&icon_arc_3_points,"arc_3_points",&icon_arc_3_points_p},
      {&icon_arc_beginning_center_end,"arc_beginning_center_end",&icon_arc_beginning_center_end_p},
      {&icon_arc_beginning_center_angle,"arc_beginning_center_angle",&icon_arc_beginning_center_angle_p},
      {&icon_arc_beginning_center_chord,"arc_beginning_center_chord",&icon_arc_beginning_center_chord_p},
      {&icon_arc_beginning_end_radius,"arc_beginning_end_radius",&icon_arc_beginning_end_radius_p},
      {&icon_arc_beginning_end_angle,"arc_beginning_end_angle",&icon_arc_beginning_end_angle_p},
      {&icon_arc_beginning_end_direction,"arc_beginning_end_direction",&icon_arc_beginning_end_direction_p},
      {&icon_arc_continuation,"arc_continuation",&icon_arc_continuation_p},
      {&icon_text_angle,"text_angle",&icon_text_angle_p},
      {&icon_text_width,"text_width",&icon_text_width_p},
      {&icon_text_type,"text_type",&icon_text_type_p},
      {&icon_hidden_text,"hidden_text",&icon_hidden_text_p},
      {&icon_text_align,"text_align",&icon_text_align_p},
      {&icon_text_interval,"text_interval",&icon_text_interval_p},
      {&icon_line_polygon_cor_cor,"line_polygon_cor_cor",&icon_line_polygon_cor_cor_p},
      {&icon_line_polygon_cen_tan,"line_polygon_cen_cor",&icon_line_polygon_cen_tan_p},
      {&icon_line_polygon_cen_cor,"line_polygon_cen_tan",&icon_line_polygon_cen_cor_p},
      {&icon_ellipse_approximation,"ellipse_approximation",&icon_ellipse_approximation_p},
      {&icon_ellipse_axis_center,"ellipse_axis_center",&icon_ellipse_axis_center_p},
      {&icon_ellipse_axis_start,"ellipse_axis_start",&icon_ellipse_axis_start_p},
      {&icon_ellipse_center,"ellipse_center",&icon_ellipse_center_p},
      {&icon_solid_quad_triang,"solid_quad_triang",&icon_solid_quad_triang_p},
      {&icon_solid_triangular1,"solid_triangular1",&icon_solid_triangular1_p},
      {&icon_solid_fill_line_width,"solid_fill_line_width",&icon_solid_fill_line_width_p},
      {&icon_solid_quadrangular,"solid_quadrangular",&icon_solid_quadrangular_p},
      {&icon_solid_triangular,"solid_triangular",&icon_solid_triangular_p},
      {&icon_arc_close,"arc_close",&icon_arc_close_p},
      {&icon_arc_remove,"arc_remove",&icon_arc_remove_p},
      {&icon_trace_width,"trace_width",&icon_trace_width_p},
      {&icon_trace_axis_offset,"trace_axis_offset",&icon_trace_axis_offset_p},
      {&icon_trace_polyline,"trace_polyline",&icon_trace_polyline_p},
      {&icon_trace_fill_line_width,"trace_fill_line_width",&icon_trace_fill_line_width_p},
      {&icon_trace_remove,"trace_remove",&icon_trace_remove_p},
      {&icon_chart_step_dx,"chart_step_dx",&icon_chart_step_dx_p},
      {&icon_ftfunct,"ftfunct",&icon_ftfunct_p},
      {&icon_last_block,"last_block",&icon_last_block_p},
      {&icon_select_all,"select_all",&icon_select_all_p},
      {&icon_all_layers,"all_layers",&icon_all_layers_p},
      {&icon_select_cross_window,"select_cross_window",&icon_select_cross_window_p},
      {&icon_add_remove,"add_remove",&icon_add_remove_p},
      {&icon_gear1,"gear1",&icon_gear1_p},
      {&icon_text_align_baselineleft,"text_align_baselineleft",&icon_text_align_baselineleft_p},
      {&icon_text_align_baselineright,"text_align_baselineright",&icon_text_align_baselineright_p},
      {&icon_text_align_baselinecenter,"text_align_baselinecenter",&icon_text_align_baselinecenter_p},
      {&icon_text_align_middlecenter,"text_align_middlecenter",&icon_text_align_middlecenter_p},
      {&icon_circle_tangential,"circle_tangential",&icon_circle_tangential_p},
      {&icon_axis_length,"axis_length",&icon_axis_length_p},
      {&icon_dash_dotted,"dash_dotted",&icon_dash_dotted_p},
      {&icon_dashed,"dashed",&icon_dashed_p},
      {&icon_return_all,"return_all",&icon_return_all_p},
      {&icon_change_mode,"change_mode",&icon_change_mode_p},
      {&icon_change_property,"change_property",&icon_change_property_p},
      {&icon_all_texts,"all_texts",&icon_all_texts_p},
      {&icon_select_window,"select_window",&icon_select_window_p},
      {&icon_select_cross,"select_cross",&icon_select_cross_p},
      {&icon_solid_filled,"solid_filled",&icon_solid_filled_p},
      {&icon_solid_very_thin,"solid_very_thin",&icon_solid_very_thin_p},
      {&icon_solid_thin,"solid_thin",&icon_solid_thin_p},
      {&icon_solid_thick,"solid_thick",&icon_solid_thick_p},
      {&icon_solid_very_thick,"solid_very_thick",&icon_solid_very_thick_p},
      {&icon_solid_extra_thick,"solid_extra_thick",&icon_solid_extra_thick_p},
      {&icon_text_angle_0,"text_angle_0",&icon_text_angle_0_p},
      {&icon_text_angle_90,"text_angle_90",&icon_text_angle_90_p},
      {&icon_text_parallel_to_object,"text_parallel_to_object",&icon_text_parallel_to_object_p},
      {&icon_text_perpendicular,"text_perpendicular",&icon_text_perpendicular_p},
      {&icon_text_angle_to_object,"text_angle_to_object",&icon_text_angle_to_object_p},
      {&icon_text_angle_x,"text_angle_X",&icon_text_angle_x_p},
      {&icon_trace_filled,"trace_filled",&icon_trace_filled_p},
      {&icon_trace_very_thin,"trace_very_thin",&icon_trace_very_thin_p},
      {&icon_trace_thin,"trace_thin",&icon_trace_thin_p},
      {&icon_trace_thick,"trace_thick",&icon_trace_thick_p},
      {&icon_trace_very_thick,"trace_very_thick",&icon_trace_very_thick_p},
      {&icon_trace_extra_thick,"trace_extra_thick",&icon_trace_extra_thick_p},
      {&icon_point_simple,"point_simple",&icon_point_simple_p},
      {&icon_point_base,"point_base",&icon_point_base_p},
      {&icon_point_pin,"point_pin",&icon_point_pin_p},
      {&icon_RAM_for_drawing,"RAM_for_drawing",&icon_RAM_for_drawing_p},
      {&icon_RAM_virtual_image,"RAM_virtual_image",&icon_RAM_virtual_image_p},
      {&icon_RAM_data_size,"RAM_data_size",&icon_RAM_data_size_p},
      {&icon_RAM_printing_buffer,"RAM_printing_buffer",&icon_RAM_printing_buffer_p},
      {&icon_RAM_macro_buffer,"RAM_macro_buffer",&icon_RAM_macro_buffer_p},
      {&icon_RAM_image_buffer,"RAM_image_buffer",&icon_RAM_image_buffer_p},
      {&icon_current_folder,"current_folder",&icon_current_folder_p},
      {&icon_desktop_size,"desktop_size",&icon_desktop_size_p},
      {&icon_layers_info,"layers_info",&icon_layers_info_p},
      {&icon_x1,"x1",&icon_x1_p},
      {&icon_y1,"y1",&icon_y1_p},
      {&icon_x2,"x2",&icon_x2_p},
      {&icon_y2,"y2",&icon_y2_p},
      {&icon_x3,"x3",&icon_x3_p},
      {&icon_y3,"y3",&icon_y3_p},
      {&icon_x4,"x4",&icon_x4_p},
      {&icon_y4,"y4",&icon_y4_p},
      {&icon_angle1,"angle1",&icon_angle1_p},
      {&icon_angle2,"angle2",&icon_angle2_p},
      {&icon_dim_dx,"dim_dx",&icon_dim_dx_p},
      {&icon_dim_dy,"dim_dy",&icon_dim_dy_p},
      {&icon_inner_block_name,"inner_block_name",&icon_inner_block_name_p},
      {&icon_outer_block_name,"outer_block_name",&icon_outer_block_name_p},
      {&icon_qr_code1,"qr_code1",&icon_qr_code1_p},
      {&icon_edit,"edit",&icon_edit_p},
      {&icon_grey,"grey",&icon_grey_p},
      {&icon_blackwhite,"blackwhite",&icon_blackwhite_p},
      {&icon_plotter_s,"plotter_s",&icon_plotter_s_p},
      {&icon_visible,"visible",&icon_visible_p},
      {&icon_visible_d,"visible_d",&icon_visible_d_p},
      {&icon_edit_d,"edit_d",&icon_edit_d_p},
      {&icon_pointfinder_d,"pointfinder_d",&icon_pointfinder_d_p},
      {&icon_color_d,"Color_d",&icon_color_d_p},
      {&icon_line_thickness_d,"line_thickness_d",&icon_line_thickness_d_p},
      {&icon_line_type_d,"line_type_d",&icon_line_type_d_p},
      {&icon_grey_d,"grey_d",&icon_grey_d_p},
      {&icon_blackwhite_d,"blackwhite_d",&icon_blackwhite_d_p},
      {&icon_visible_db,"visible_db",&icon_visible_db_p},
      {&icon_edit_db,"edit_db",&icon_edit_db_p},
      {&icon_pointfinder_db,"pointfinder_db",&icon_pointfinder_db_p},
      {&icon_grey_db,"grey_db",&icon_grey_db_p},
      {&icon_blackwhite_db,"blackwhite_db",&icon_blackwhite_db_p},
      {&icon_plotter_64,"plotter_64",&icon_plotter_64_p},
      {&icon_saveimage_64,"saveimage_64",&icon_saveimage_64_p},
      {&icon_rotation_d48,"rotation_d48",&icon_rotation_d48_p},
      {&icon_inversion_d48,"inversion_d48",&icon_inversion_d48_p},
      {&icon_reflection_d48,"reflection_d48",&icon_reflection_d48_p},
      {&icon_grey_d48,"grey_d48",&icon_grey_d48_p},
      {&icon_blackwhite_d48,"blackwhite_d48",&icon_blackwhite_d48_p},
      {&icon_grey_image_d48,"grey_image_d48",&icon_grey_image_d48_p},
      {&icon_page_d48,"page_d48",&icon_page_d48_p},
      {&icon_scale_d48,"scale_d48",&icon_scale_d48_p},
      {&icon_window_d48,"window_d48",&icon_window_d48_p},
      {&icon_sheet_d48,"sheet_d48",&icon_sheet_d48_p},
      {&icon_tools,"tools",&icon_tools_p},
      {&icon_features,"features",&icon_features_p},
      {&icon_ctrl,"Ctrl",&icon_ctrl_p},
      {&icon_alt,"Alt",&icon_alt_p},
      {&icon_shift,"Shift",&icon_shift_p},
      {&icon_home,"Home",&icon_home_p},
      {&icon_end,"End",&icon_end_p},
      {&icon_esc,"Esc",&icon_esc_p},
      {&icon_ins,"Ins",&icon_ins_p},
      {&icon_pgdn,"PgDn",&icon_pgdn_p},
      {&icon_pgup,"PgUp",&icon_pgup_p},
      {&icon_spc,"Spc",&icon_spc_p},
      {&icon_del,"del",&icon_del_p },
      {&icon_tab,"Tab",&icon_tab_p},
      {&icon_ent,"Ent",&icon_ent_p },
      {&icon_er,"er",&icon_er_p },
      {&icon_k_right,"k_right",&icon_k_right_p },
      {&icon_k_left,"k_left",&icon_k_left_p },
      {&icon_k_up,"k_up",&icon_k_up_p },
      {&icon_k_down,"k_down",&icon_k_down_p },
      {&icon_0,"0",&icon_0_p},
      {&icon_1,"1",&icon_1_p},
      {&icon_2,"2",&icon_2_p},
      {&icon_3,"3",&icon_3_p},
      {&icon_4,"4",&icon_4_p},
      {&icon_5,"5",&icon_5_p},
      {&icon_6,"6",&icon_6_p},
      {&icon_7,"7",&icon_7_p},
      {&icon_8,"8",&icon_8_p},
      {&icon_9,"9",&icon_9_p},
      {&icon_plus,"plus",&icon_plus_p},
      {&icon_minus,"minus",&icon_minus_p},
      {&icon_f1,"F1",&icon_f1_p},
      {&icon_f2,"F2",&icon_f2_p},
      {&icon_f3,"F3",&icon_f3_p},
      {&icon_f4,"F4",&icon_f4_p},
      {&icon_f5,"F5",&icon_f5_p},
      {&icon_f6,"F6",&icon_f6_p},
      {&icon_f7,"F7",&icon_f7_p},
      {&icon_f8,"F8",&icon_f8_p},
      {&icon_f9,"F9",&icon_f9_p},
      {&icon_f10,"F10",&icon_f10_p},
      {&icon_f11,"F11",&icon_f11_p},
      {&icon_f12,"F12",&icon_f12_p},
      {&icon_mouse3,"mouse3",&icon_mouse3_p},
      {&icon_mouseR,"mouseR",&icon_mouseR_p},
      {&icon_measure_tape,"measure_tape",&icon_measure_tape_p},
      {&icon_spa,"Spa",&icon_spa_p},
      {&icon_ce,"ce",&icon_ce_p},
      {&icon_look_down,"look_down",&icon_look_down_p},
      {&icon_plotter_32,"plotter_32",&icon_plotter_32_p},
      {&icon_cartridge_d48,"ink_cartridge_d48",&icon_cartridge_d48_p},
      {&icon_very_thin_d,"very_thin_d",&icon_very_thin_d_p},
      {&icon_thin_d,"thin_d",&icon_thin_d_p},
      {&icon_thick_d,"thick_d",&icon_thick_d_p},
      {&icon_very_thick_d,"very_thick_d",&icon_very_thick_d_p},
      {&icon_extra_thick_d,"extra_thick_d",&icon_extra_thick_d_p},
      {&icon_color_d48,"Color_d48",&icon_color_d48_p},
      {&icon_scale_d,"scale_d",&icon_scale_d_p},
      {&icon_page_d,"page_d",&icon_page_d_p},
      {&icon_page_width_d,"page_width_d",&icon_page_width_d_p},
      {&icon_page_height_d,"page_height_d",&icon_page_height_d_p},
      {&icon_page_overlap_d,"page_overlap_d",&icon_page_overlap_d_p},
      {&icon_color256_d48,"Color256_d48",&icon_color256_d48_p},
      {&icon_multiline_text,"multiline_text",&icon_multiline_text_p},
      {&icon_underlined,"underlined",&icon_underlined_p},
      {&icon_line_spacing,"line_spacing",&icon_line_spacing_p},
      {&icon_bold_d,"bold_d",&icon_bold_d_p},
      {&icon_font_type_d,"font_type_d",&icon_font_type_d_p},
      {&icon_hidden_text_d,"hidden_text_d",&icon_hidden_text_d_p},
      {&icon_italic_d,"italic_d",&icon_italic_d_p},
      {&icon_layers_d,"layers_d",&icon_layers_d_p},
      {&icon_text_align_d,"text_align_d",&icon_text_align_d_p},
      {&icon_text_height_d,"text_height_d",&icon_text_height_d_p},
      {&icon_text_type_d,"text_type_d",&icon_text_type_d_p},
      {&icon_width_d,"text_width_d",&icon_width_d_p},
      {&icon_underlined_d,"underlined_d",&icon_underlined_d_p},
      {&icon_no_d_12,"no_d_12",&icon_no_d_12_p},
      {&icon_mark_d_12,"mark_d_12",&icon_mark_d_12_p},
      {&icon_hor_dpi_sphere_blue_d,"hor_dpi_sphere_blue_d",&icon_hor_dpi_sphere_blue_d_p},
      {&icon_ver_dpi_sphere_blue_d,"ver_dpi_sphere_blue_d",&icon_ver_dpi_sphere_blue_d_p},
      {&icon_hor_size_image_d,"hor_size_image_d",&icon_hor_size_image_d_p},
      {&icon_ver_size_image_d,"ver_size_image_d",&icon_ver_size_image_d_p},
      {&icon_hor_scale_image_d,"hor_scale_image_d",&icon_hor_scale_image_d_p},
      {&icon_ver_scale_image_d,"ver_scale_image_d",&icon_ver_scale_image_d_p},
      {&icon_background_image_d,"background_image_d",&icon_background_image_d_p},
      {&icon_background_image_d48,"background_image_d48",&icon_background_image_d48_p},
      {&icon_rotate_angle_image_d,"rotate_angle_image_d",&icon_rotate_angle_image_d_p},
      {&icon_top_margin_d,"top_margin_d",&icon_top_margin_d_p},
      {&icon_left_margin_d,"left_margin_d",&icon_left_margin_d_p},
      {&icon_bottom_margin_d,"bottom_margin_d",&icon_bottom_margin_d_p},
      {&icon_right_margin_d,"right_margin_d",&icon_right_margin_d_p},
      {&icon_actual_size_d48,"actual_size_d48",&icon_actual_size_d48_p},
      {&icon_pattern,"pattern",&icon_pattern_p},
      {&icon_pattern_angle,"pattern_angle",&icon_pattern_angle_p},
      {&icon_pattern_scale,"pattern_scale",&icon_pattern_scale_p},
      {&icon_edit_point_origin,"edit_point_origin",&icon_edit_point_origin_p},
      {&icon_set_point_origin,"set_point_origin",&icon_set_point_origin_p},
      {&icon_pattern_line_distance,"pattern_line_distance",&icon_pattern_line_distance_p},
      {&icon_anglex,"angleX",&icon_anglex_p},
      {&icon_angle45,"angle45",&icon_angle45_p},
      {&icon_angle90,"angle90",&icon_angle90_p},
      {&icon_angle135,"angle135",&icon_angle135_p},
      {&icon_angle180,"angle180",&icon_angle180_p},
      {&icon_angle225,"angle225",&icon_angle225_p},
      {&icon_angle270,"angle270",&icon_angle270_p},
      {&icon_angle315,"angle315",&icon_angle315_p},
      {&icon_divide_count,"divide_count",&icon_divide_count_p},
      {&icon_divide_counter,"divide_counter",&icon_divide_counter_p},
      {&icon_divide_measure,"divide_measure",&icon_divide_measure_p},
      {&icon_divide_segment_length,"divide_segment_length",&icon_divide_segment_length_p},
      {&icon_divide_counter_max,"divide_counter_max",&icon_divide_counter_max_p},
      {&icon_divide_polyline_segment,"divide_polyline_segment",&icon_divide_polyline_segment_p},
      {&icon_divide_polyline,"divide_polyline",&icon_divide_polyline_p},
      {&icon_divide_segment,"divide_segment",&icon_divide_segment_p},
      {&icon_align_block,"align_block",&icon_align_block_p},
      {&icon_offset_point,"offset_point",&icon_offset_point_p},
      {&icon_offset_distance,"offset_distance",&icon_offset_distance_p},
      {&icon_set_offset_distance,"set_offset_distance",&icon_set_offset_distance_p},
      {&icon_return,"return",&icon_return_p},
      {&icon_open_file_folder,"open_file_folder",&icon_open_file_folder_p},
      {&icon_arrow_up_d48,"arrow_up_d48",&icon_arrow_up_d48_p},
      {&icon_arrow_up_end_d48,"arrow_up_end_d48",&icon_arrow_up_end_d48_p},
      {&icon_arrow_down_end_d48,"arrow_down_end_d48",&icon_arrow_down_end_d48_p},
      {&icon_arrow_down_d48,"arrow_down_d48",&icon_arrow_down_d48_p},
      {&icon_folder_d48,"folder_d48",&icon_folder_d48_p},
      {&icon_folder0_d48,"folder0_d48",&icon_folder0_d48_p},
      {&icon_bigger_d48,"bigger_d48",&icon_bigger_d48_p},
      {&icon_smaller_d48,"smaller_d48",&icon_smaller_d48_p},
      {&icon_expand_hor,"expand_hor",&icon_expand_hor_p},
      {&icon_expand_ver,"expand_ver",&icon_expand_ver_p},
      {&icon_expand_diag,"expand_diag",&icon_expand_diag_p},
      {&icon_expand_flex,"expand_flex",&icon_expand_flex_p},
      {&icon_expand_last,"expand_last",&icon_expand_last_p},
      {&icon_expand_win,"expand_win",&icon_expand_win_p},
      {&icon_arrow_up_d,"arrow_up_d",&icon_arrow_up_d_p},
      {&icon_arrow_down_d,"arrow_down_d",&icon_arrow_down_d_p},
      {&icon_pgup_d,"pgup_d",&icon_pgup_d_p},
      {&icon_pgdn_d,"pgdn_d",&icon_pgdn_d_p},
      {&icon_arrow_up_left_d,"arrow_up_left_d",&icon_arrow_up_left_d_p},
      {&icon_arrow_down_left_d,"arrow_down_left_d",&icon_arrow_down_left_d_p},
      {&icon_plotter_start_d_64,"plotter_start_d_64",&icon_plotter_start_d_64_p},
      {&icon_printer_start_d_64,"printer_start_d_64",&icon_printer_start_d_64_p},
      {&icon_escape_d_64,"escape_d_64",&icon_escape_d_64_p},
      {&icon_save_d,"save_d",&icon_save_d_p},
      {&icon_load_d,"load_d",&icon_load_d_p},
      {&icon_return_d_24,"return_d_24",&icon_return_d_24_p},
      {&icon_frame,"frame",&icon_frame_p},
      {&icon_zones_around,"zones_around",&icon_zones_around_p},
      {&icon_zones_prefix,"zones_prefix",&icon_zones_prefix_p},
      {&icon_zones_first_number,"zones_first_number",&icon_zones_first_number_p},
      {&icon_zones_reversed,"zones_reversed",&icon_zones_reversed_p},
      {&icon_zone_height,"zone_height",&icon_zone_height_p},
      {&icon_zone_width,"zone_width",&icon_zone_width_p},
      {&icon_margin_width,"margin_width",&icon_margin_width_p},
      {&icon_frame_line,"frame_line",&icon_frame_line_p},
      {&icon_zones_line,"zones_line",&icon_zones_line_p},
      {&icon_frame_color,"frame_color",&icon_frame_color_p},
      {&icon_zones_color,"zones_color",&icon_zones_color_p},
      {&icon_style2_black,"style2_black",&icon_style2_black_p},
      {&icon_zones_top,"zones_top",&icon_zones_top_p},
      {&icon_zones_bottom,"zones_bottom",&icon_zones_bottom_p},
      {&icon_zones_top_bottom,"zones_top_bottom",&icon_zones_top_bottom_p},
      {&icon_frame_offset,"frame_offset",&icon_frame_offset_p},
      {&icon_ok_d_64,"ok_d_64",&icon_ok_d_64_p},
      {&icon_new_layer_d_64,"new_layer_d_64",&icon_new_layer_d_64_p},
      {&icon_printer_d_48,"printer_d_48",&icon_printer_d_48_p},
      {&icon_hor_extents_d,"hor_extents_d",&icon_hor_extents_d_p},
      {&icon_ver_extents_d,"ver_extents_d",&icon_ver_extents_d_p},
      {&icon_hor_limits_d,"hor_limits_d",&icon_hor_limits_d_p},
      {&icon_ver_limits_d,"ver_limits_d",&icon_ver_limits_d_p},
      {&icon_units_d,"units_d",&icon_units_d_p},
      {&icon_paper_size_d_48,"paper_size_d_48",&icon_paper_size_d_48_p},
      {&icon_paper_size_d,"paper_size_d",&icon_paper_size_d_p},
      {&icon_paper_dim_d_48,"paper_dim_d_48",&icon_paper_dim_d_48_p},
      {&icon_bspline,"bspline",&icon_bspline_p},
      {&icon_bspline34,"bspline34",&icon_bspline34_p},
      {&icon_bspline3p,"bspline3p",&icon_bspline3p_p},
      {&icon_bspline4,"bspline4",&icon_bspline4_p},
      {&icon_bspline3,"bspline3",&icon_bspline3_p},
      {&icon_yes_d_48,"yes_d_48",&icon_yes_d_48_p},
      {&icon_no_d_48,"no_d_48",&icon_no_d_48_p},
      {&icon_escape_d_48,"escape_d_48",&icon_escape_d_48_p},
      {&icon_escape_d,"escape_d",&icon_escape_d_p},
      {&icon_tartan,"tartan",&icon_tartan_p},
      {&icon_all_windows,"all_windows",&icon_all_windows_p},
      {&icon_junction,"junction",&icon_junction_p},
      {&icon_save_window,"save_window",&icon_save_window_p},
      {&icon_freehand_segment_length,"freehand_segment_length",&icon_freehand_segment_length_p},
      {&icon_remove_last_freehand,"remove_last_freehand",&icon_remove_last_freehand_p},
      {&icon_new_from_template,"new_from_template",&icon_new_from_template_p},
      {&icon_idea,"idea",&icon_idea_p},
      {&icon_mirror_block,"mirror_block",&icon_mirror_block_p},
      {&icon_tree,"tree",&icon_tree_p},
      {&icon_tree_enter,"tree_enter",&icon_tree_enter_p},
      {&icon_tree_return,"tree_return",&icon_tree_return_p},
      {&icon_trans30,"trans30",&icon_trans30_p},
      {&icon_trans40,"trans40",&icon_trans40_p},
      {&icon_trans50,"trans50",&icon_trans50_p},
      {&icon_trans60,"trans70",&icon_trans60_p},
      {&icon_trans70,"trans70",&icon_trans70_p},
      {&icon_trans80,"trans80",&icon_trans80_p},
      {&icon_trans90,"trans90",&icon_trans90_p},
      {&icon_trans100,"trans100",&icon_trans100_p},

      { &icon_trans05,"trans05",&icon_trans05_p },
      { &icon_trans10,"trans10",&icon_trans10_p },
      { &icon_trans15,"trans15",&icon_trans15_p },
      { &icon_trans20,"trans20",&icon_trans20_p },
      { &icon_trans25,"trans25",&icon_trans25_p },
      { &icon_trans35,"trans35",&icon_trans35_p },
      { &icon_trans45,"trans45",&icon_trans45_p },
      { &icon_trans55,"trans55",&icon_trans55_p },
      { &icon_trans65,"trans65",&icon_trans65_p },
      { &icon_trans75,"trans75",&icon_trans75_p },
      { &icon_trans85,"trans85",&icon_trans85_p },
      { &icon_trans95,"trans95",&icon_trans95_p },
      { &icon_trans50_d,"trans50_d",&icon_trans50_d_p },

      {&icon_back,"Back",&icon_back_p},
      {&icon_space,"space",&icon_space_p},	
      {&icon_A,"A",&icon_A_p},
      {&icon_B,"B",&icon_B_p},
      {&icon_C,"C",&icon_C_p},
      {&icon_D,"D",&icon_D_p},
      {&icon_E,"E",&icon_E_p},
      {&icon_F,"F",&icon_F_p},
      {&icon_G,"G",&icon_G_p},
      {&icon_H,"H",&icon_H_p},
      {&icon_I,"I",&icon_I_p},
      {&icon_J,"J",&icon_J_p},
      {&icon_K,"K",&icon_K_p},
      {&icon_L,"L",&icon_L_p},
      {&icon_M,"M",&icon_M_p},
      {&icon_N,"N",&icon_N_p},
      {&icon_O,"O",&icon_O_p},
      {&icon_P,"P",&icon_P_p},
      {&icon_Q,"Q",&icon_Q_p},
      {&icon_R,"R",&icon_R_p},
      {&icon_S,"S",&icon_S_p},
      {&icon_T,"T",&icon_T_p},
      {&icon_U,"U",&icon_U_p},
      {&icon_V,"V",&icon_V_p},
      {&icon_W,"W",&icon_W_p},
      {&icon_X,"X",&icon_X_p},
      {&icon_Y,"Y",&icon_Y_p},
      {&icon_Z,"Z",&icon_Z_p},
      {&icon_load_d_48,"load_d_48",&icon_load_d_48_p},
      {&icon_load_ini_d,"load_ini_d",&icon_load_ini_d_p},
      {&icon_load_ini_d_48,"load_ini_d_48",&icon_load_ini_d_48_p},
      {&icon_angle0,"angle0",&icon_angle0_p},
      {&icon_angleRX,"angleRX",&icon_angleRX_p},
      {&icon_anglePto,"anglePto",&icon_anglePto_p},
      {&icon_angleRto,"angleRto",&icon_angleRto_p},
      {&icon_angleXtoP,"angleXtoP",&icon_angleXtoP_p},
      {&icon_touch_id_db_64,"touch_id_db_64",&icon_touch_id_db_64_p},
      {&icon_left_margin_d_48,"left_margin_d_48",&icon_left_margin_d_48_p},
      {&icon_bottom_margin_d_48,"bottom_margin_d_48",&icon_bottom_margin_d_48_p},
      {&icon_all_layers_db_64,"all_layers_db_64",&icon_all_layers_db_64_p},
      {&icon_global_db_48,"global_db_48",&icon_global_db_48_p},
      {&icon_local_db_48,"local_db_48",&icon_local_db_48_p},
      {&icon_color256_db_48,"Color256_db_48",&icon_color256_db_48_p},
      {&icon_cursors,"cursors",&icon_cursors_p},
      {&icon_cursor_small,"cursor_small",&icon_cursor_small_p},
      {&icon_cursor_big,"cursor_big",&icon_cursor_big_p},
      {&icon_question_mark_db_48,"question_mark_db_48",&icon_question_mark_db_48_p},
      {&icon_pointern,"pointerN",&icon_pointern_p},
      {&icon_pointerb,"pointerB",&icon_pointerb_p},
      {&icon_pdf_vector_64,"pdf_vector_64",&icon_pdf_vector_64_p},
      {&icon_pattern_folder,"pattern_folder",&icon_pattern_folder_p},
      {&icon_close_window,"close_window",&icon_close_window_p},
      {&icon_change_pattern,"change_pattern",&icon_change_pattern_p},
      {&icon_import_map,"import_map",&icon_import_map_p},
      {&icon_starAstar,"starAstar",&icon_starAstar_p},
      {&icon_starA,"starA",&icon_starA_p},
      {&icon_Astar,"Astar",&icon_Astar_p},
      {&icon_Aonly,"Aonly",&icon_Aonly_p},
      {&icon_gear,"gear",&icon_gear_p},
      {&icon_confirm_or_deny,"confirm_or_deny",&icon_confirm_or_deny_p},
      {&icon_stonewall,"StoneWall",&icon_stonewall_p},
      {&icon_brickwall_dx,"brickwall_dx",&icon_brickwall_dx_p},
      {&icon_brickwall_dy,"brickwall_dy",&icon_brickwall_dy_p},
      {&icon_folder_bd32,"folder_bd32",&icon_folder_bd32_p},
      {&icon_folder_bd48,"folder_bd48",&icon_folder_bd48_p},
      {&icon_folder_bd64,"folder_bd64",&icon_folder_bd64_p},
      {&icon_chain_d,"chain_d",&icon_chain_d_p},
      {&icon_on_top_d,"on_top_d",&icon_on_top_d_p},
      {&icon_h_flip,"h_flip",&icon_h_flip_p},
      {&icon_v_flip,"v_flip",&icon_v_flip_p},
      {&icon_offset_style,"offset_style",&icon_offset_style_p},
      {&icon_offset_normal,"offset_normal",&icon_offset_normal_p},
      {&icon_offset_smooth,"offset_smooth",&icon_offset_smooth_p},
      {&icon_trace_close,"trace_close",&icon_trace_close_p},
      {&icon_trace_break,"trace_break",&icon_trace_break_p},
      {&icon_UA_B,u8"Б",&icon_UA_B_p},
      {&icon_UA_D,u8"Д",&icon_UA_D_p},
      {&icon_UA_E,u8"Е",&icon_UA_E_p},
      {&icon_UA_J,u8"Й",&icon_UA_J_p},
      {&icon_UA_K,u8"К",&icon_UA_K_p},
      {&icon_UA_N,u8"Н",&icon_UA_N_p},
      {&icon_UA_P,u8"П",&icon_UA_P_p},
      {&icon_UA_R,u8"Р",&icon_UA_R_p},
      {&icon_UA_S,u8"С",&icon_UA_S_p},
      {&icon_UA_T,u8"Т",&icon_UA_T_p},
      {&icon_UA_C,u8"Ц",&icon_UA_C_p},
      {&icon_spline_points,"spline_points",&icon_spline_points_p},
      {&icon_spline_control_points,"spline_control_points",&icon_spline_control_points_p },
      {&icon_spline_points_close,"spline_points_close",&icon_spline_points_close_p },
      {&icon_spline_amend,"spline_amend",&icon_spline_amend_p },
      {&icon_spline_points_end,"spline_points_end",&icon_spline_points_end_p },
      {&icon_mouse1b,"mouse1b",&icon_mouse1b_p },
      {&icon_mouse2b,"mouse2b",&icon_mouse2b_p },
      {&icon_mouse1b2b,"mouse1b2b",&icon_mouse1b2b_p },
      {&icon_mouse3b,"mouse3b",&icon_mouse3b_p },
      {&icon_mouseRb,"mouseRb",&icon_mouseRb_p },
      {&icon_education,"education",&icon_education_p },
      {&icon_mickey_hand,"mickey_hand",&icon_mickey_hand_p },
      {&icon_mickey_hand_s,"mickey_hand_s",&icon_mickey_hand_s_p },
      {&icon_dim_leader,"dim_leader",&icon_dim_leader_p},
      {&icon_insulation,"insulation",&icon_insulation_p },
      {&icon_ins_simple,"ins-simple",&icon_ins_simple_p },
      {&icon_ins_complex,"ins-complex",&icon_ins_complex_p },
      {&icon_quad_trans,"quad-trans",&icon_quad_trans_p },
      {&icon_rect_angle_opt,"rect_angle_opt",&icon_rect_angle_opt_p },
      {&icon_move_rectangle,"move_rectangle",&icon_move_rectangle_p },
      {&icon_rect_options,"rect_options",&icon_rect_options_p },
      {&icon_object_polyline,"object_polyline",&icon_object_polyline_p },
      {&icon_entire_polyline,"entire_polyline",&icon_entire_polyline_p },
      {&icon_single_object,"single_object",&icon_single_object_p },
      {&icon_image_dx,"image_dx",&icon_image_dx_p },
      {&icon_image_dy,"image_dy",&icon_image_dy_p },
      {&icon_break_options,"break_options",&icon_break_options_p },
      {&icon_break_and_leave,"break_and_leave",&icon_break_and_leave_p },
      {&icon_break_and_cut,"break_and_cut",&icon_break_and_cut_p },
      {&icon_break_divide,"break_divide",&icon_break_divide_p },
      {&icon_fixed_scale,"fixed_scale",&icon_fixed_scale_p },
      {&icon_text_collineal,"text_collineal",&icon_text_collineal_p },
      {&icon_decimal_fractional,"decimal_fractional",&icon_decimal_fractional_p },
      {&icon_import_vectorize,"import_vectorize",&icon_import_vectorize_p },
      {&icon_space128,"space128",&icon_space128_p },
      {&icon_backspace128,"backspace128",&icon_backspace128_p },
      {&icon_enter128,"enter128",&icon_enter128_p },
      {&icon_ttf,"ttf",&icon_ttf_p },
      {&icon_otf,"otf",&icon_otf_p },
      {&icon_elliptical,"elliptical",&icon_elliptical_p },
      {&icon_ellipticalarc,"ellipticalarc",&icon_ellipticalarc_p },
      {&icon_filledellipses,"filledellipses",&icon_filledellipses_p },
      {&icon_ellipticalarclocal,"ellipticalarclocal",&icon_ellipticalarclocal_p },
      {&icon_ellipticalarcglobal,"ellipticalarcglobal",&icon_ellipticalarcglobal_p },
      {&icon_arc_line_continuation,"arc_line_continuation",&icon_arc_line_continuation_p },
      {&icon_line_arc_continuation,"line_arc_continuation",&icon_line_arc_continuation_p },
      {&icon_center_block,"center_block",&icon_center_block_p },
      {&icon_alpha_sorting_d32,"alpha_sorting_d32",&icon_alpha_sorting_d32_p },
      {&icon_time_lapse_d32,"time_lapse_d32",&icon_time_lapse_d32_p },
      {&icon_select_entity,"select_entity",&icon_select_entity_p },
      {&icon_select,"select",&icon_select_p },
      {&icon_vector,"vector",&icon_vector_p },
      {&icon_rigid_rigid,"rigid_rigid",&icon_rigid_rigid_p },
      {&icon_rigid_pin,"rigid_pin",&icon_rigid_pin_p },
      {&icon_pin_rigid,"pin_rigid",&icon_pin_rigid_p },
      {&icon_pin_pin,"pin_pin",&icon_pin_pin_p },
      {&icon_force,"force",&icon_force_p },
      {&icon_moment,"moment",&icon_moment_p },
      {&icon_moment_rev,"moment_rev",&icon_moment_rev_p },
      {&icon_displacement,"displacement",&icon_displacement_p },
      {&icon_rotation,"rotation",&icon_rotation_p },
      {&icon_rotation_rev,"rotation_rev",&icon_rotation_rev_p },
      {&icon_trapezium_y,"trapezium_y",&icon_trapezium_y_p },
      {&icon_trapezium_x,"trapezium_x",&icon_trapezium_x_p },
      {&icon_trapezium_n,"trapezium_n",&icon_trapezium_n_p },
      {&icon_trapezium_x,"trapezium_h",&icon_trapezium_h_p },
      {&icon_trapezium_n,"trapezium_v",&icon_trapezium_v_p },
      {&icon_fixed_h,"fixed_h",&icon_fixed_h_p },
      {&icon_fixed_vl,"fixed_vl",&icon_fixed_vl_p },
      {&icon_fixed_vr,"fixed_vr",&icon_fixed_vr_p },
      {&icon_pinned_h,"pinned_h",&icon_pinned_h_p },
      {&icon_pinned_vl,"pinned_vl",&icon_pinned_vl_p },
      {&icon_pinned_vr,"pinned_vr",&icon_pinned_vr_p },
      {&icon_fixed_roller_h,"fixed_roller_h",&icon_fixed_roller_h_p },
      {&icon_fixed_roller_vl,"fixed_roller_vl",&icon_fixed_roller_vl_p },
      {&icon_fixed_roller_vr,"fixed_roller_vr",&icon_fixed_roller_vr_p },
      {&icon_pinned_roller_h,"pinned_roller_h",&icon_pinned_roller_h_p },
      {&icon_pinned_roller_vl,"pinned_roller_vl",&icon_pinned_roller_vl_p },
      {&icon_pinned_roller_vr,"pinned_roller_vr",&icon_pinned_roller_vr_p },
      {&icon_node,"node",&icon_node_p },
      {&icon_magnitude,"magnitude",&icon_magnitude_p },
      {&icon_thermal,"thermal",&icon_thermal_p },
      {&icon_gear_plus,"gear_plus",&icon_gear_plus_p },
      {&icon_fixed_hu,"fixed_hu",&icon_fixed_hu_p },
      {&icon_pinned_hu,"pinned_hu",&icon_pinned_hu_p },
      {&icon_fixed_roller_hu,"fixed_roller_hu",&icon_fixed_roller_hu_p },
      {&icon_pinned_roller_hu,"pinned_roller_hu",&icon_pinned_roller_hu_p },
      {&icon_fixed_rotation,"fixed_rotation",&icon_fixed_rotation_p },
      {&icon_compression_mag,"compression_mag",&icon_compression_mag_p },
      {&icon_tension_mag,"tension_mag",&icon_tension_mag_p },
      {&icon_shear_mag,"shear_mag",&icon_shear_mag_p },
      {&icon_moments_mag,"moments_mag",&icon_moments_mag_p },
      {&icon_displacements_mag,"displacements_mag",&icon_displacements_mag_p },
      {&icon_reactions_xy,"reactions_xy",&icon_reactions_xy_p },
      {&icon_reactions_m,"reactions_m",&icon_reactions_m_p },
      {&icon_thermal_y,"thermal_y",&icon_thermal_y_p },
      {&icon_thermal_t,"thermal_t",&icon_thermal_t_p },
      {&icon_trapezium_y1,"trapezium_y1",&icon_trapezium_y1_p },
      {&icon_trapezium_y2,"trapezium_y2",&icon_trapezium_y2_p },
      {&icon_thermal_t1,"thermal_t1",&icon_thermal_t1_p },
      {&icon_thermal_t2,"thermal_t2",&icon_thermal_t2_p },
      {&icon_vectors,"vectors",&icon_vectors_p },
      {&icon_US_Flag,"US-Flag",&icon_US_Flag_p },
      {&icon_EU_Flag,"EU-Flag",&icon_EU_Flag_p },
      {&icon_US_Flag,"UK-Flag",&icon_UK_Flag_p },
      {&icon_IH_sections,"IH-sections",&icon_IH_sections_p },
      {&icon_U_sections,"U-sections",&icon_U_sections_p },
      {&icon_T_sections,"T-sections",&icon_T_sections_p },
      {&icon_L_sections,"L-sections",&icon_L_sections_p },
      {&icon_2L_sections,"2L-sections",&icon_2L_sections_p },
      {&icon_RT_sections,"RT-sections",&icon_RT_sections_p },
      {&icon_O_sections,"O-sections",&icon_O_sections_p },
      {&icon_EO_sections,"EO-sections",&icon_EO_sections_p },
      {&icon_Z_sections,"Z-sections",&icon_Z_sections_p },
      {&icon_VJ_sections,"VJ-sections",&icon_VJ_sections_p },
      {&icon_timber,"timber",&icon_timber_p },
      {&icon_node_size,"node_size",&icon_node_size_p },
      {&icon_dead_load,"dead_load2",&icon_dead_load_p },
      {&icon_live_load,"live_load",&icon_live_load_p },
      {&icon_roof_load,"roof_load",&icon_roof_load_p },
      {&icon_wind_load,"wind_load",&icon_wind_load_p },
      {&icon_snow_load,"snow_load",&icon_snow_load_p },
      {&icon_earthquake_load,"earthquake_load",&icon_earthquake_load_p },
      {&icon_rain_load,"rain_load",&icon_rain_load_p },
      {&icon_soil_load,"soil_load",&icon_soil_load_p },
      {&icon_fluid_load,"fluid_load",&icon_fluid_load_p },
      {&icon_thermal_load,"thermal_load",&icon_thermal_load_p },
      {&icon_unspecified_load,"unspecified_load",&icon_unspecified_load_p },
      {&icon_unknown_load,"unknown_load",&icon_unknown_load_p },
      {&icon_number_load,"unspecified_load",&icon_number_load_p },
      {&icon_stress_mag,"stress_mag",&icon_stress_mag_p },
      {&icon_stress_plus_mag,"stress_plus_mag",&icon_stress_plus_mag_p },
      {&icon_stress_minus_mag,"stress_minus_mag",&icon_stress_minus_mag_p },
      {&icon_shear_stress_mag,"shear_stress_mag",&icon_shear_stress_mag_p },
      {&icon_eurocode_d48,"eurocode_d48",&icon_eurocode_d48_p },
      {&icon_asce_d48,"asce_d48",&icon_asce_d48_p },
      {&icon_icc_d48,"icc_d48",&icon_icc_d48_p },
      {&icon_combination_d48,"combination_d48",&icon_combination_d48_p },
      {&icon_erase_layer_db_64,"erase_layer_db_64",&icon_erase_layer_db_64_p },
      {&icon_mark_layer_db_64,"mark_layer_db_64",&icon_mark_layer_db_64_p },
      {&icon_AlfaCAD48,"AlfaCAD48",&icon_AlfaCAD48_p },
      {&icon_Pdelta_d48,"Pdelta_d48",&icon_Pdelta_d48_p },
      {&icon_dynamics,"dynamics",&icon_dynamics_p },
      {&icon_vibrations_d48,"vibrations_d48",&icon_vibrations_d48_p },
      {&icon_inertia_d48,"inertia_d48",&icon_inertia_d48_p },
      {&icon_dynamics_run,"dynamics_run",&icon_dynamics_run_p },
      {&icon_menustyle,"menustyle",&icon_menustyle_p },
      {&icon_cursorstyle,"cursorstyle",&icon_cursorstyle_p },
      {&icon_barstyle,"barstyle",&icon_barstyle_p },
      {&icon_perc_mag,"perc_mag",&icon_perc_mag_p },
      {&icon_cross_section_forces,"cross_section_forces",&icon_cross_section_forces_p },
      {&icon_ULS,"ULS",&icon_ULS_p },
      {&icon_SLS,"SLS",&icon_SLS_p },
      {&icon_QPSLS,"QPSLS",&icon_QPSLS_p },
      {&icon_resilience,"resilience",&icon_resilience_p },
  };

    int bitmaps_size = sizeof(bitmap_load) / sizeof(bitmap_load[0]);
BITMAP* bt;

for (int i = 0; i < bitmaps_size; i++)
{
    sprintf(bitmap_file, "%s/%s.png", _BITMAPS_, bitmap_load[i].png_file);
    bt=load_png(bitmap_file, pal);
    *bitmap_load[i].png_b = bt;
    *bitmap_load[i].png_p = *bitmap_load[i].png_b;
}

  //ini_cursor_busy();  //based on bitmap

  set_mickey_hand(icon_mickey_hand_s, icon_mickey_hand);

  strcpy(zbior_danych, file_name);

  zwolnienie_pamieci();
  
  out_file_name1 () ;

  Clear_View () ;
  
  if (0 != czytaj_rysunek(file_name, TRUE))
   {
    strcpy (zbior_danych, "") ;
   }


  if (strlen(zbior_danych)>0) load_file_to_history(file_name);
 
  out_file_name();

  Ini_Place_Marker ();

  Ini_Layers_Dlg () ;

  Change = FALSE;
  Set_Auto_Backup (FALSE);

  set_scrsave_time ();

  komunikat0(0);

  last_mouse_z=mouse_z;

  int curr_x01, curr_y01, curr_h1, curr_v1;
  get_window_origin_and_size_(&curr_x01, &curr_y01, &curr_h1, &curr_v1);
  int x_edit=curr_x01+200;
  int y_edit=curr_y01+200;
#ifdef LINUX
  set_editbox_geometry(x_edit, y_edit);
#else
  set_editbox_geometry_win(x_edit, y_edit);
#endif

  x_edit=curr_x01+2;

  y_edit=curr_y01+Get_X11_SCREEN_SHIFT()+ED_INF_HEIGHT*2;
#ifdef LINUX
  set_editbox_geometry_line(x_edit, y_edit);
#else
  y_edit -= Get_WIN_WINDOW_T_B();
  set_editbox_geometry_line_win(x_edit, y_edit);
#endif

  position_mouse(getmaxx()/2, getmaxy()/2);
  scare_mouse();

  reset_bmp_back();

  ////reseting client bitmaps
  ////for (i = 0; i < MAX_CLIENT_BITMAP; i++) client_bitmap_load[i]=NULL;

  kasowanie_licznikow = TRUE;

  set_close_button_callback (closebutton);

  if (desktop_instruction) ret=Show_Alfa_Instruction();


  //it's just for test of slider
  /*
  _free_mouse();
  dialog_cursor(1);
  int ret2=slider();
  dialog_cursor(0);
  lock_mouse();
   */

  install_int(Check_ConfigureNotify, 1000);

  _free_mouse();
  lock_mouse();

  //MAIN LOOP

  while(1)
   {

       if (go_reset_if_resized==TRUE) reset_if_resized();

       //checking file buffer
      bufptr = &shmp_private.buf;
      if (bufptr->mflag==1) {
          Load_New_Files(bufptr);
      }
      else {
          bufptr = &shmp->buf;
          if (bufptr->mflag == 1) {
              Load_New_Files(bufptr);
          }
      }

	  if (sleep_state == TRUE) 
		  my_sleep(1);
	 Semaphore = TRUE;
     Auto_Backup_Proc ();
     ScrSave_Proc (0);
     set_menu_level(0);
     set_pattern_offset(0);
     set_pattern_count(FALSE);
     n1 = getwsp(menu) ;
     Auto_Backup_Proc ();
     ScrSave_Proc (0);

     if (n1>WOOD) n1=WOOD;  //wood sections

     if(n1 == 0)
     {
       Error=0;
       komunikat(0);
       if (menu->flags ^ 0x44) menu->flags &= 0x44;

         if ((mouse_b & 2) && (key[KEY_DOWN]))
         {
             ret=empty_dlg();
         }
     }
     else if ((n1>0) && (n1<((sizeof(COMNDg)/sizeof(COMNDg[0]))+1)))
     {
	   FF=COMND[n1-1];
       LASTFUN=FF;
       FF();
       menu->flags &= 0x44;
	   if (exit_alf==TRUE) 
	   {
		  return 0; 
	   }
     }
   }
   return 0;
}

#ifdef LINUX
END_OF_MAIN()
#endif

#undef __RYSUJ__