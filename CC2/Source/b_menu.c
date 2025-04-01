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

#define __B_MENU__
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include<forwin.h>
#include<stdlib.h>
#ifndef LINUX
#include<conio.h>
#else
#include <unistd.h>
#endif
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#define ALLEGWIN
#include <allegext.h>
#include "alffont.h"
#include <time.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "b_menu.h"
#include "bgiext.h"

#include "menu.h"
#include "o_dialog.h"

#include "leak_detector_c.h"

#define LINE myline
#define RECTFILL myrectfill

#define spline_CommandP_close -9
#define spline_CommandP_end -7
#define spline_CommandP_del -5

#define TEMPN 200

#define GMENU_MAX 15

#ifdef LINUX
static float mouse_speed = 2.5;
#else
static float mouse_speed = 1.0;
#endif

static int curr_h, curr_v;

static p_point bar_center;

BOOL XORBAR=FALSE;

BOOL altkey = FALSE;
BOOL altgrkey = FALSE;

BOOL ignore_altkey = FALSE;

BOOL altkey_ = FALSE;
BOOL altgrkey_ = FALSE;
BOOL shiftkey_ = FALSE;
BOOL ctrlkey_ = FALSE;

static int dyy=0;
static int dyy_min = 0;
static int lastk = '\0';

static int menu_level=0;
static char search_str[MaxTextLen]={""};

BITMAP* bmp_back = NULL;
BITMAP* demo_bmp_back = NULL;
BITMAP* demo_bmp_back1 = NULL;
BITMAP* mickey_hand_s = NULL;
BITMAP* mickey_hand = NULL;
static int keyimage_back = 0;
static int shiftkeyimage_back = 0;
static int keyimage_back_back = 0;
static int shiftkeyimage_back_back = 0;
static long keytimestamp, shiftkeytimestamp = 0;

static int MP_SIZE_, ED_INF_HEIGHT_, BAR_G_, HEIGHT_, WIDTH_;

BOOL DEMO_RECORDING = FALSE;
static BOOL withdraw_keyimage_permit = FALSE;
static float demo_scale = 1.0;
static char *object_info_ad=NULL;
static char sk_info [128], *sk1_info;
static char sk0_info [64];
static int menu_n=0;

#define Max_Menu_Level 6

static struct viewporttype viewinfo[Max_Menu_Level];

int TTF_OFF=0;

int typ_f_org;

FORTIPS_MAP fortips_map;

int CONTROL_KEYS[32] = { _CTRL_0_,_CTRL_1_,_CTRL_2_,_CTRL_3_,_CTRL_4_,_CTRL_5_,_CTRL_6_,_CTRL_7_,_CTRL_8_,_CTRL_9_,
_CTRL_10_,_CTRL_11_,_CTRL_12_,_CTRL_13_,_CTRL_14_,_CTRL_15_,_CTRL_16_,_CTRL_17_,_CTRL_18_,_CTRL_19_,
_CTRL_20_,_CTRL_21_,_CTRL_22_,_CTRL_23_,_CTRL_24_,_CTRL_25_,_CTRL_26_,_CTRL_27_,_CTRL_28_,_CTRL_29_,
_CTRL_30_,_CTRL_31_ };


char  readmouse(void);

extern TMENU manimation;

extern BITMAP *client_bitmap_load[MAX_CLIENT_BITMAP];

extern TMENU mBlok_Imp;

extern int PozX, PozY;
extern int i__font_nomax;

extern void Move_Mouse (int, int) ;

extern int my_kbhit(void);
extern int my_getch(void);
extern void setwritemode( int mode );
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);
extern void setcolor(int kolor);
extern void myline(int x1, int y1, int x2, int y2);
extern void myrectfill(int x1, int y1, int x2, int y2);
extern void rectangle(int left, int top, int right, int bottom);
extern int GetColorAC1(int color);
extern void setfillstyle_(int pattern, int color);
extern void getviewsettings(struct viewporttype  *viewport);
extern void setviewport(int left, int top, int right, int bottom, int clip);
extern void setviewsettings(struct viewporttype  *viewport, int v_x, int v_y, int v_xk, int v_yk);
extern void settextjustify(int horiz, int vert);
extern void getimage(int left, int top, int right, int bottom, void  *bitmap);
extern void putimage(int left, int top,  void  *bitmap, int op);
extern void getimage2(int left, int top, int right, int bottom, void  *bitmap);
extern void putimage2(int left, int top,  void  *bitmap, int op);
extern void putimage3(int left, int top, void* bitmap, int op);
extern int get_pXp(void);
extern int get_pYk(void);
extern void set_scrsave_time (void);
extern int Write_screen_pcx(void);
extern BOOL ScrSave_Proc (int opcja);
extern int get_cursor_posX(void);
extern int get_cursor_posY(void);
extern int Skala_In_(void);
extern int Skala_Out_(void);
extern int getx (void) ;
extern int gety (void) ;
extern time_t get_scrsave_time(void);
extern void beep(long sound);

extern int SkalaZ_Minus(int sclfct);
extern int SkalaZ_Plus(int sclfct);

extern int empty_dlg(void);
extern float jednostkiX0_(long pixels);
extern float jednostkiY0_(long pixels);

extern void draw_font_name_ttf(TEXT *t, char *t_text, BITMAP *bmp, int x0, int y0, int xmax, double kat, float wysokosc_p, COLOR_ kolor, int mode, int t_width, int t_height);
extern unsigned char check_client_flag(void);

extern PTMENU mCzcionka;
extern PTMENU mCzcionkaZ;
extern PTMENU mCzcionkaW;
extern TMENU mKolorAC;
extern TMENU mGruboscLinii;
extern TMENU mGruboscLiniiS;
extern TMENU mTypyLinii;
extern TMENU mLineSpacing;
extern TMENU mRegion;

extern TMENU mLoad_Char_Thermal;
extern TMENU mLoad_Char;
extern TMENU mLoadVariant;

extern void(*CUR_OFF)(double, double);
extern void(*CUR_ON)(double, double);

extern int lenutf8to(const unsigned char *s, int to);
extern char *Get_dane0_DATA(int dwg_no);
extern int my_ugetch(void);
extern int TTF_text_len(char *text);
extern int getcolor(void);
extern void backup_world(void);
extern void outtextxy_w_menu(TEXT *t);
extern void world_back(void);
extern int lineCuncut(long x1, long y1, long x2, long y2);
extern void TTF_text_UI_(BITMAP *ui_screen, const char *text, int x, int y, int *text_h, int *text_v);
extern int my_poll_keyboard(void);
extern int Get_Global_Dialog_Flag(void);

extern void set_mode_solid(void);
extern void set_mode_solid2(void);
extern int32_t ucs2_to_utf8(int32_t ucs2, uint8_t* utf8);
extern void TTF_text_UI_W_H_(BITMAP *ui_screen, const char* text, int x, int y, int* text_h, int* text_v, int WIDTH__, int HEIGHT__);
extern void Set_TrybS(int poz);

extern int TRANSLUCENCY;

extern GrContext *get_second_screen(void);
extern void flip_corner(void);
extern void flip_corner3(void);
extern char* Get_Global_Dialog_Name(void);

static int  cprev_(TMENU *menu);
static int  cnext_(TMENU *menu);
int Simple_Menu_Proc_(TMENU *menu);
int Simple_Menu_Proc(TMENU *menu);

extern void return_menu_par (char *pole, char * par);
extern void return_menu_par0 (char *pole, char * par);

extern void get_global_coords (double *x0, double *y0);

extern BOOL TTF_redraw;
extern char* tab_typ_tekstu[];
extern char* typ_punktu_tab[];
extern char* vector_style_tab[];
extern char* tab_justowanie[];
extern char _YES__[4];
extern char _NO__[4];

extern char* view_width_tab1[];
extern char* view_type_tab[];

extern int Menu_No_to_Color_No (int i_menu_poz);
extern int Add_New_Font_TTF_OTF(char *name, int type, int cur);

extern int Set_TypeL_to_Type (int type, int type_line);
extern int colors(void);
extern void nazwa_koloru(char *ad1, char *sk);

extern TMENU mTTF_OTF;

extern BOOL BIGCURSOR;

extern double Get_Ds(void);
extern int Set_WidthL_to_Type(int type, int width);
extern int Get_Shm_private_mflag(void);
extern int Get_Shm_mflag(void);
extern void Check_XNextEvent(void);

extern unsigned long_long Get_Free_Physical_Memory(void);

extern void lock_mouse(void);
extern void _free_mouse(void);

extern void Set_Screenplay(BITMAP *ctx_bitmap);
extern  BITMAP *screenplay;
extern void magnitude2angle(AVECTOR *V, double factor);
extern double depth_magnitude; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
extern double thermal_magnitude; //units per mm  default 1 Celsius per 1 mm on drawing paper
extern double load_magnitude; //units per mm  default 10kN/m force per 1 mm on drawing paper
extern double force_magnitude; //units per mm  default 10kN force per 1 mm on drawing paper
extern double moment_magnitude; //units per mm  default 10kNm force per 1 mm radius on drawing paper
extern double displacement_magnitude; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
extern double rotation_magnitude;

extern void magnitude2line(AVECTOR *V, double factor);
extern void set_decimal_format(char *text, double l, double precision);
extern double Angle_Normal (double angle);
extern unsigned short vector_wcod[];
extern unsigned short point_wcod[];

extern char* typ_punktu_inf[];
extern void komunikat_str_short(char *st, BOOL stay, BOOL center);
extern char *load_symbol[];

extern char* typ_punktu_inf[];
extern int d_myslider_proc(int msg, void *d_, int c);
extern void Draw_Slider(SLIDER *Slider);
extern int get_palette_color(int color);

extern int gui_border_dark, gui_border_light;

extern void set_cursor_pointer(void);
extern void set_cursor_edit(void);


static int menu_grab_slider(void *dp3, int d2);
static int menu_init_slider(int *var1, int *var2, int *var3, int *var4);

extern void disable_F11(void);
extern void enable_F11(void);
extern void Restore_Pointer(void);

#define MAXMENULEVEL 8
static  int set_slider[MAXMENULEVEL]={0,0,0,0,0,0,0,0};
SLIDER slider[MAXMENULEVEL]={{d_myslider_proc,  0, 0,  18, 0,  0,  98,    0,   0,  100,   0,    NULL , menu_grab_slider, menu_init_slider, NULL},
                             {d_myslider_proc,  0, 0,  18, 0,  0,  98,    0,   0,  100,   0,    NULL , menu_grab_slider, menu_init_slider, NULL},
                             {d_myslider_proc,  0, 0,  18, 0,  0,  98,    0,   0,  100,   0,    NULL , menu_grab_slider, menu_init_slider, NULL},
                             {d_myslider_proc,  0, 0,  18, 0,  0,  98,    0,   0,  100,   0,    NULL , menu_grab_slider, menu_init_slider, NULL},
                             {d_myslider_proc,  0, 0,  18, 0,  0,  98,    0,   0,  100,   0,    NULL , menu_grab_slider, menu_init_slider, NULL},
                             {d_myslider_proc,  0, 0,  18, 0,  0,  98,    0,   0,  100,   0,    NULL , menu_grab_slider, menu_init_slider, NULL},
                             {d_myslider_proc,  0, 0,  18, 0,  0,  98,    0,   0,  100,   0,    NULL , menu_grab_slider, menu_init_slider, NULL},
                             {d_myslider_proc,  0, 0,  18, 0,  0,  98,    0,   0,  100,   0,    NULL , menu_grab_slider, menu_init_slider, NULL},
                             };

extern double thermal_precision;
extern double force_precision;
extern double moment_precision;
extern double stress_precision;
extern double displacement_precision;
extern double rotation_precision;
extern double load_precision;

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

extern char *icon_pointfinder_p;
extern char *icon_camera_p;
extern char *icon_parameters_p;
extern char *icon_layers_p;
extern char *icon_measure_p;
extern char *icon_empty_p;

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


extern char *icon_area_p;
extern char *icon_center_of_area_p;
extern char *icon_first_moment_of_area_p;
extern char *icon_area_moment_of_inertia_p;
extern char *icon_centr_area_mom_of_inertia_p;
extern char *icon_test_of_closed_area_p;

extern char *qmark_p;
extern char *eymark_p;
extern char *ermark_p;
extern char *offmark_p;
extern char *plotter_p;

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
extern char *icon_axis_length_p;
extern char *icon_dash_dotted_p;
extern char *icon_dashed_p;
extern char *icon_return_all_p;
extern char *icon_change_mode_p;
extern char *icon_change_property_p;
extern char *icon_all_texts_p;
extern char *icon_select_window_p;
extern char *icon_select_cross_p;

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

extern char *icon_trace_filled_p;
extern char *icon_trace_very_thin_p;
extern char *icon_trace_thin_p;
extern char *icon_trace_thick_p;
extern char *icon_trace_very_thick_p;
extern char *icon_trace_extra_thick_p;
extern char *icon_point_simple_p;
extern char *icon_point_base_p;
extern char *icon_point_pin_p;

extern char *icon_RAM_for_drawing_p;
extern char *icon_RAM_virtual_image_p;
extern char *icon_RAM_data_size_p;
extern char *icon_RAM_printing_buffer_p;
extern char *icon_RAM_macro_buffer_p;
extern char *icon_RAM_image_buffer_p;
extern char *icon_current_folder_p;
extern char *icon_desktop_size_p;

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

extern char *icon_qr_code1_p;
extern char *icon_edit_p;
extern char *icon_grey_p;
extern char *icon_blackwhite_p;
extern char *icon_plotter_s_p;
extern char *icon_visible_p;

extern char *icon_visible_d_p;
extern char *icon_edit_d_p;
extern char *icon_pointfinder_d_p;
extern char *icon_color_d_p;
extern char *icon_line_thickness_d_p;
extern char *icon_line_type_d_p;
extern char *icon_grey_d_p;
extern char *icon_blackwhite_d_p;

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
extern char *icon_tab_p;
extern char* icon_ent_p;
extern char* icon_er_p;
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
extern char *icon_mouse3_p;
extern char *icon_mouseR_p;
extern char *icon_measure_tape_p;
extern char *icon_spa_p;
extern char *icon_ce_p;
extern char *icon_look_down_p;

extern char *icon_plotter_32_p;

extern char *icon_cartridge_d48_p;
extern char *icon_very_thin_d_p;
extern char *icon_thin_d_p;
extern char *icon_thick_d_p;
extern char *icon_very_thick_d_p;
extern char *icon_extra_thick_d_p;
extern char *icon_color_d48_p;

extern char *icon_scale_d_p;
extern char *icon_page_d_p;
extern char *icon_page_width_d_p;
extern char *icon_page_height_d_p;
extern char *icon_page_overlap_d_p;

extern char *icon_color256_d48_p;

extern char *icon_multiline_text_p;
extern char *icon_underlined_p;
extern char *icon_line_spacing_p;

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
extern char *icon_yes_d_pmem;
extern char *icon_no_d_pmem;

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

extern char *icon_pattern_p;
extern char *icon_pattern_angle_p;
extern char *icon_pattern_scale_p;
extern char *icon_edit_point_origin_p;
extern char *icon_set_point_origin_p;
extern char *icon_pattern_line_distance_p;

extern char *icon_anglex_p;
extern char *icon_angle45_p;
extern char *icon_angle90_p;
extern char *icon_angle135_p;
extern char *icon_angle180_p;
extern char *icon_angle225_p;
extern char *icon_angle270_p;
extern char *icon_angle315_p;

extern char *icon_divide_count_p;
extern char *icon_divide_counter_p;
extern char *icon_divide_measure_p;
extern char *icon_divide_segment_length_p;
extern char *icon_divide_counter_max_p;
extern char *icon_divide_polyline_segment_p;
extern char *icon_divide_polyline_p;
extern char *icon_divide_segment_p;

extern char *icon_align_block_p;
extern char *icon_offset_point_p;
extern char *icon_offset_distance_p;
extern char *icon_set_offset_distance_p;
extern char *icon_return_p;

extern char *icon_open_file_folder_p;

extern char *icon_arrow_up_d48_p;
extern char* icon_arrow_up_end_d48_p;
extern char* icon_arrow_down_end_d48_p;
extern char *icon_arrow_down_d48_p;
extern char *icon_folder_d48_p;
extern char *icon_folder0_d48_p;
extern char *icon_smaller_d48_p;
extern char *icon_bigger_d48_p;

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

extern char *icon_ok_d_64_p;
extern char *icon_new_layer_d_64_p;

extern char *icon_hor_extents_d_p;
extern char *icon_ver_extents_d_p;
extern char *icon_hor_limits_d_p;
extern char *icon_ver_limits_d_p;
extern char *icon_units_d_p;
extern char *icon_paper_size_d_48_p;
extern char *icon_paper_dim_d_48_p;

extern char *icon_bspline_p;
extern char *icon_bspline34_p;
extern char *icon_bspline3p_p;
extern char *icon_bspline4_p;
extern char *icon_bspline3_p;

extern char *icon_yes_d_48_p;
extern char *icon_no_d_48_p;
extern char *icon_escape_d_48_p;
extern char *icon_no_d_12_p;
extern char *icon_mark_d_12_p;
extern char *icon_escape_d_p;

extern char *icon_tartan_p;
extern char *icon_all_windows_p;

extern char *icon_junction_p;
extern char *icon_save_window_p;

extern char *icon_freehand_segment_length_p;
extern char *icon_remove_last_freehand_p;

extern char *icon_new_from_template_p;

extern char *icon_idea_p;

extern char *icon_mirror_block_p;

extern char *icon_tree_p;
extern char *icon_tree_enter_p;
extern char *icon_tree_return_p;

extern char *icon_trans30_p;
extern char *icon_trans40_p;
extern char *icon_trans50_p;
extern char *icon_trans60_p;
extern char *icon_trans70_p;
extern char *icon_trans80_p;
extern char *icon_trans90_p;
extern char *icon_trans100_p;

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

extern char* icon_trans50_d_p;

extern char *icon_back_p;
extern char *icon_space_p;

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

extern char *icon_load_d_48_p;
extern char *icon_load_ini_d_p;
extern char *icon_load_ini_d_48_p;

extern char *icon_angle0_p;
extern char *icon_angleRX_p;
extern char *icon_anglePto_p;
extern char *icon_angleRto_p;
extern char *icon_angleXtoP_p;

extern char *icon_touch_id_db_64_p;
extern char *icon_left_margin_d_48_p;
extern char *icon_bottom_margin_d_48_p;

extern char *icon_all_layers_db_64_p;

extern char *icon_global_db_48_p;
extern char *icon_local_db_48_p;

extern BITMAP *icon_color256_db_48;
extern char *icon_color256_db_48_p;

extern char *icon_cursors_p;
extern char *icon_cursor_small_p;
extern char *icon_cursor_big_p;

extern char *icon_pointern_p;
extern char *icon_pointerb_p;

extern char *icon_question_mark_db_48_p;

extern char *icon_pdf_vector_64_p;

extern char *icon_pattern_folder_p;

extern char *icon_close_window_p;

extern char *icon_change_pattern_p;

extern char *icon_import_map_p;

extern char *icon_paper_size_d_p;

extern char *icon_starAstar_p;
extern char *icon_starA_p;
extern char *icon_Astar_p;
extern char *icon_Aonly_p;
extern char *icon_gear_p;
extern char *icon_confirm_or_deny_p;

extern char *icon_stonewall_p;

extern char* icon_brickwall_dx_p;
extern char* icon_brickwall_dy_p;

extern char* icon_folder_bd32_p;
extern char* icon_folder_bd48_p;
extern char* icon_folder_bd64_p;

extern char* icon_chain_d_p;

extern char* icon_on_top_d_p;

extern char* icon_h_flip_p;
extern char* icon_v_flip_p;

extern char* icon_offset_style_p;
extern char* icon_offset_normal_p;
extern char* icon_offset_smooth;

extern char* icon_trace_close_p;
extern char* icon_trace_break_p;

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

extern char* icon_spline_points_p;
extern char* icon_spline_control_points_p;
extern char* icon_spline_points_close_p;
extern char* icon_spline_amend_p;
extern char* icon_spline_points_end_p;

extern char *icon_mouse1b_p; 
extern char* icon_mouse2b_p;
extern char* icon_mouse1b2b_p;
extern char* icon_mouse3b_p;
extern char* icon_mouseRb_p;

extern char* icon_education_p;

extern char* icon_mickey_hand_s_p;

extern char* icon_dim_leader_p;

extern char* icon_insulation_p;

extern char* icon_ins_simple_p;
extern char* icon_ins_complex_p;
extern char* icon_quad_trans_p;

extern char* icon_rect_angle_opt_p;
extern char* icon_move_rectangle_p;
extern char* icon_rect_options_p;

extern char* icon_object_polyline_p;
extern char* icon_entire_polyline_p;
extern char* icon_single_object_p;

extern char* icon_image_dx_p;
extern char* icon_image_dy_p;

extern char* icon_break_options_p;
extern char* icon_break_and_leave_p;
extern char* icon_break_and_cut_p;
extern char* icon_break_divide_p;

extern char* icon_fixed_scale_p;

extern char* icon_text_collineal_p;
extern char* icon_decimal_fractional_p;

extern char* icon_import_vectorize_p;

extern char* icon_space128_p;
extern char* icon_backspace128_p;
extern char* icon_enter128_p;

extern char* icon_ttf_p;
extern char* icon_otf_p;

extern char *icon_elliptical_p;
extern char *icon_ellipticalarc_p;
extern char *icon_filledellipses_p;

extern char *icon_ellipticalarclocal_p;
extern char *icon_ellipticalarcglobal_p;

extern char *icon_arc_line_continuation_p;
extern char *icon_line_arc_continuation_p;

extern char *icon_center_block_p;
extern char *icon_alpha_sorting_d32_p;
extern char *icon_time_lapse_d32_p;

extern char *icon_select_entity_p;
extern char *icon_select_p;

extern char *icon_vector_p;
extern char *icon_rigid_rigid_p;
extern char *icon_rigid_pin_p;
extern char *icon_pin_rigid_p;
extern char *icon_pin_pin_p;
extern char *icon_force_p;
extern char *icon_moment_p;
extern char *icon_moment_rev_p;
extern char *icon_displacement_p;
extern char *icon_rotation_p;
extern char *icon_rotation_rev_p;
extern char *icon_trapezium_y_p;
extern char *icon_trapezium_x_p;
extern char *icon_trapezium_n_p;
extern char *icon_trapezium_h_p;
extern char *icon_trapezium_v_p;

extern char *icon_fixed_h_p;
extern char *icon_fixed_vl_p;
extern char *icon_fixed_vr_p;
extern char *icon_pinned_h_p;
extern char *icon_pinned_vl_p;
extern char *icon_pinned_vr_p;
extern char *icon_fixed_roller_h_p;
extern char *icon_fixed_roller_vl_p;
extern char *icon_fixed_roller_vr_p;
extern char *icon_pinned_roller_h_p;
extern char *icon_pinned_roller_vl_p;
extern char *icon_pinned_roller_vr_p;
extern char *icon_node_p;
extern char *icon_magnitude_p;
extern char *icon_thermal_p;
extern char *icon_gear_plus_p;
extern char *icon_fixed_hu_p;
extern char *icon_pinned_hu_p;
extern char *icon_fixed_roller_hu_p;
extern char *icon_pinned_roller_hu_p;
extern char *icon_fixed_rotation_p;

extern char *icon_compression_mag_p;
extern char *icon_tension_mag_p;
extern char *icon_shear_mag_p;
extern char *icon_moments_mag_p;
extern char *icon_displacements_mag_p;
extern char *icon_reactions_xy_p;
extern char *icon_reactions_m_p;

extern char *icon_thermal_y_p;
extern char *icon_thermal_t_p;

extern char *icon_trapezium_y1_p;
extern char *icon_trapezium_y2_p;
extern char *icon_thermal_t1_p;
extern char *icon_thermal_t2_p;

extern char *icon_vectors_p;

extern char *icon_US_Flag_p;
extern char *icon_EU_Flag_p;
extern char *icon_UK_Flag_p;

extern char *icon_CA_Flag_p;
extern char *icon_AU_Flag_p;

extern char *icon_IH_sections_p;
extern char *icon_U_sections_p;
extern char *icon_T_sections_p;
extern char *icon_L_sections_p;
extern char *icon_2L_sections_p;
extern char *icon_RT_sections_p;
extern char *icon_O_sections_p;
extern char *icon_EO_sections_p;
extern char *icon_Z_sections_p;
extern char *icon_VJ_sections_p;

extern char *icon_timber_p;

extern char *icon_node_size_p;

extern char *icon_dead_load_p;
extern char *icon_live_load_p;
extern char *icon_roof_load_p;
extern char *icon_wind_load_p;
extern char *icon_snow_load_p;
extern char *icon_earthquake_load_p;
extern char *icon_rain_load_p;
extern char *icon_soil_load_p;
extern char *icon_fluid_load_p;
extern char *icon_thermal_load_p;

extern char *icon_unspecified_load_p;
extern char *icon_unknown_load_p;
extern char *icon_number_load_p;

extern char *icon_stress_mag_p;
extern char *icon_stress_plus_mag_p;
extern char *icon_stress_minus_mag_p;
extern char *icon_shear_stress_mag_p;

extern char *icon_eurocode_d48_p;
extern char *icon_asce_d48_p;
extern char *icon_icc_d48_p;

extern char *icon_combination_d48_p;
extern char *icon_erase_layer_db_64_p;
extern char *icon_mark_layer_db_64_p;

extern char *icon_AlfaCAD48_p;
extern char *icon_Pdelta_d48_p;

extern char *icon_dynamics_p;
extern char *icon_vibrations_d48_p;
extern char *icon_inertia_d48_p;

extern char *icon_dynamics_run_p;

extern char *icon_menustyle_p;
extern char *icon_cursorstyle_p;
extern char *icon_barstyle_p;

extern char *icon_perc_mag_p;
extern char *icon_cross_section_forces_p;

extern char *icon_resilience_p;

extern char *icon_ULS_p;
extern char *icon_SLS_p;
extern char *icon_QPSLS_p;

extern TMENU mInfo;
extern TMENU mInfoAbout;
extern TMENU mInfoAboutA;

extern BOOL editing_text;

/////Functions for HELP/////
extern int Free_Mouse(void);

static int  cpgprev(TMENU *menu);
static int  cpgnext(TMENU *menu);

void baronoff(TMENU  * menu);
void show_hide_tip(TMENU * menu, BOOL show);

extern void add_to_buffer(char c);
extern int small_large_step(void);

extern int  Choose_Layer_1(void);
extern int  Choose_Layer_2(void);
extern int  Choose_Layer_3(void);
extern int  Choose_Layer_4(void);
extern int  Choose_Layer_5(void);
extern int  Choose_Layer_6(void);
extern int  Choose_Layer_7(void);
extern int  Choose_Layer_8(void);
extern int  Choose_Layer_9(void);
extern int  Choose_Layer_10(void);
extern int  Dec_Layer(void);
extern int  Inc_Layer(void);
extern int  Change_Orto(void);
extern int  Edit_Text_Tab_F2(void);
extern int  Edit_Text_F3(void);
extern int  Usun_F5(void);
extern int  undo_blok_F6(void);

extern int SkalaW_(void);
extern int last_view_(void);
extern int SkalaE_(void);
extern int SkalaZC_(void);
extern int SkalaZ1_(void);
extern int SkalaZ05_(void);
extern int SkalaZ02_(void);
extern int SkalaZ01_(void);
extern int SkalaR_(void);
extern int skala_format_(void);

extern int kolorL_(void);
extern int typ_linii_(void);
extern int grubosc_linii_(void);
extern int Set_Layers_(void);
extern int Point_Measure_(void);
extern int Dist_Measure_(void);
extern int Pan_Shift_(void);
extern int Angle_Measure1_(void);
extern int Wez_kolor_(void);
extern int Wez_typ_(void);
extern int Wez_warstwe_(void);
extern int Hide_Layers_(void);
extern int All_Layers_On_(void);
extern int Reverse_Layers_(void);

extern int myCharUpperW(int zn);


/////

#define MB_ABORTRETRYIGNORE         0x00000002L

#define tip_delay 3
#define baron baronoff
#define F1 '\59'
#define NOCLOSE 0x01       /*nigdy nie zamykane*/
#define NOWCLOSE 0x02      /*nie zamykane po wyborze*/
#define NAWOPEN 0x08;  /*wlasnie otwarte*/
#define CENTER_T 0x10
#define TADD 0x04 
#define ICONS 0x40
#define NVERT 0x80	   /*okno poziome*/
#define TIP 0x20
#define FIXED 0x100

	#define SKALA 1  //TO CHECK
	#define GR 4
	#define MDY 4
	#define MDY1 1
	#define YP  2
    #define YP0 0
    #define YP1 1
	#define DEFFONT DEFAULT_FONT


#define step 2
#define nsteps 16
#define z_step 16

static POLE pmTranslucencyCH[] = {
        {u8"100%",'0',546,NULL},
        {u8"95%",'1',674,NULL},
        {u8"90%",'2',545,NULL},
        {u8"85%",'3',673,NULL},
        {u8"80%",'4',544,NULL},
        {u8"75%",'5',672,NULL},
        {u8"70%",'6',543,NULL},
        {u8"65%",'7',671,NULL},
        {u8"60%",'8',542,NULL},
        {u8"55%",'9',670,NULL},
        {u8"50%",'A',541,NULL},
        {u8"45%",'B',669,NULL},
        {u8"40%",'C',540,NULL},
        {u8"35%",'D',668,NULL},
        {u8"30%",'E',539,NULL},
        {u8"25%",'F',667,NULL},
        {u8"20%",'G',666,NULL},
        {u8"15%",'H',665,NULL},
        {u8"10%",'I',664,NULL},
        {u8"5%",'J',663,NULL},

};

TMENU mTranslucencyCH = { 20,0,0,8,22,9,ICONS,CMNU,CMBR,CMTX,0,1,0,0,0,(POLE(*)[]) & pmTranslucencyCH,NULL,NULL };


typedef  void (*f)(void);
typedef  f * TA1;

static time_t current_time=0;
static int suma_msx=0, suma_msy=0, suma_abs_msx=0, suma_abs_msy=0;
static int frame_counter = 0;
FRAME_COORDS xy;
FRAMES frames;

int frame_count = 0;
int mouse_dz = 2;

BOOL BAR_POINTER=1;


static TMENU mItalicsI = { 2,0,0,7,79,9,ICONS,CMNU,CMBR,CMTX,0,13,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mUkrytyI = { 2,0,0,7,79,11,ICONS,CMNU,CMBR,CMTX,0,4,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mBoldI = { 2,0,0,7,79,12,ICONS,CMNU,CMBR,CMTX,0,37,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mUnderlinedI = { 2,0,0,7,79,12,ICONS,CMNU,CMBR,CMTX,0,39,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };
static TMENU mAutoInterwalI = { 2,0,0,7,79,12,ICONS,CMNU,CMBR,CMTX,0,6,0,0,0,(POLE(*)[]) &pmTak_Nie,NULL,NULL };

POLE pmJustowanieI[] = {
        {u8"to Left",'L',262,NULL},
        {u8"to Right",'R',263,NULL},
        {u8"Middle",'M',264,NULL},
        {u8"Central",'C',265,NULL},
};

TMENU mJustowanieI = { 4,0,0,13,74,6,ICONS,CMNU,CMBR,CMTX,0,31,0,0,0,(POLE(*)[]) &pmJustowanieI,NULL,NULL };

POLE pmTypTekstuI[] = {
        {u8"Normal",'N',0,NULL},
        {u8"Internal label",'I',0,NULL},
        {u8"component Label",'L',0,NULL},
        {u8"Symbol",'S',0,NULL},
        {u8"Type",'T',0,NULL},
        {u8"*pin name",'*',0,NULL},
        {u8"power supply",'V',0,NULL},
        {u8"Port",'P',0,NULL},
        {u8"Attribute",'A',0,NULL},
        {u8"+Scheme",'+',0,NULL},
        {u8"File name",'F',0,NULL},
        {u8"netWork",'W',0,NULL},
        {u8"Comment",'C',0,NULL},
        {u8"%variable",'%',0,NULL},
        {u8"aUx.symbol",'U',0,NULL},
        {u8"auX.description",'X',0,NULL},
};

TMENU mTypTekstuI = { 16,0,0,16,74,6,0,CMNU,CMBR,CMTX,0,15,0,0,0,(POLE(*)[]) &pmTypTekstuI,NULL,NULL };

extern TMENU mTyp_punktu;
extern TMENU mMember_style;
extern TMENU mForce_Displacement_style;
extern TMENU mMoment_Rotation_style;
extern TMENU mLoad_style;


#ifndef LINUX
#ifndef BIT64
char* StrStrIA(const char* haystack, const char* needle) {
    int c = tolower((unsigned char)*needle);
    if (c == '\0')
        return (char*)haystack;
    for (; *haystack; haystack++) {
        if (tolower((unsigned char)*haystack) == c) {
            for (size_t i = 0;;) {
                if (needle[++i] == '\0')
                    return (char*)haystack;
                if (tolower((unsigned char)haystack[i]) != tolower((unsigned char)needle[i]))
                    break;
            }
        }
    }
    return NULL;
}
#endif

char *strcasestr(const char *str, const char *pattern) {
	size_t i;
	unsigned char c0 = *pattern, c1, c2;

	if (c0 == '\0')
		return (char *)str;

	c0 = toupper(c0);
	for (; (c1 = *str) != '\0'; str++) {
		if (toupper(c1) == c0) {
			for (i = 1;; i++) {
				c2 = pattern[i];
				if (c2 != '\0')
					return (char *)str;
				c1 = str[i];
				if (toupper(c1) != toupper(c2))
					break;
			}
		}
	}
	return NULL;
}
#endif


void Set_Mouse_Speed(float m_speed)
{
    mouse_speed=m_speed;
}

int get_menu_level(void) {
  return menu_level;
}

void inc_menu_level(TMENU * menu) {
    menu_level++;
    menu_address[menu_level-1]=(char*)menu;
}

void dec_menu_level(void) {
    menu_level--;
}

void set_menu_level(int menu_l) {
    menu_level=menu_l;
}

float get_demo_scale(void)
{
    return demo_scale;
}

void set_demo_scale(float demo_scale_)
{
    demo_scale=demo_scale_;
}

static int menu_len (char *ptrsz_len, int i_menu_len)
/*-------------------------------------------------*/
{
  int max_len;

  max_len = lenutf8to(ptrsz_len, i_menu_len);

  return max_len;
}

void Set_HEIGHT_high(void)
{
	MP_SIZE_ = MP_SIZE;
	ED_INF_HEIGHT_ = ED_INF_HEIGHT;
    BAR_G_ = BAR_G;
    HEIGHT_= HEIGHT;
    WIDTH_ = WIDTH;

	MP_SIZE = 27;
	ED_INF_HEIGHT = 27;
	BAR_G = 27;
	HEIGHT = 27;
	WIDTH = 17;
}

void Set_HEIGHT_back(void)
{
	MP_SIZE = MP_SIZE_;
	ED_INF_HEIGHT = ED_INF_HEIGHT_;
	BAR_G = BAR_G_;
	HEIGHT = HEIGHT_;
	WIDTH = WIDTH_;
}


void Test_PMenu (PTMENU *menu)
//-----------------------------------
{
    int xr,yd,size,x1,y1,x2,y2;
    int maxw;
    int i, i_drawings;
    int xs=0, ys=0;

    menu->maxw=menu->maxw0; //inicjacja

    if (menu == &menug)
    {
        //checking how many drawings are opened
        i_drawings = 0;
        for (i = 0; i < MAX_NUMBER_OF_WINDOWS; i++)
        {
            if (Get_dane0_DATA(i) != NULL) i_drawings++;
        }
        if (i_drawings > 1) menu->max = GMENU_MAX;
        else menu->max = GMENU_MAX - 1;
        if ((menu->poz + menu->off) == (GMENU_MAX - 1)) menu->poz -= 1;
    }

    if ((BAR_POINTER) && (menu->maxw>0) && (menu->maxw<menu->max))
    {
        if (menu->flags&NVERT) ys=20;
        else xs=20;
    }

    if (menu->flags&ICONS)
    {
        if (menu->flags&NVERT)
        {
            if ((WIDTH /*8*/ * SKALA * menu->xpcz + menu->xdl * menu->max) > maxX)
                menu->xpcz = (maxX - menu->xdl * menu->max) / (WIDTH /*8*/ * SKALA);
        }
        else if ((WIDTH /*8*/ * SKALA * menu->xpcz + menu->xdl + xs) > maxX)
            menu->xpcz = (maxX - menu->xdl - xs) / (WIDTH /*8*/ * SKALA);

        size = menu->maxw ? menu->maxw : menu->max;
        xr = ((menu->flags&NVERT) ? size * menu->xdl : menu->xdl);
        yd = ((menu->flags&NVERT) ? 1 : size);
        x1 = (menu->xpcz - 1)*WIDTH /*8*/ * SKALA;
        y1 = (menu->ypcz - 1) * 32 + YP;

        x2 = x1 + xr * SKALA + 2 * (GR + 1);
        y2 = y1 + yd * 32 + 2 * GR;

        x2 += xs;
        y2 += ys;

        if (y2 > maxY)
        {
            menu->maxw = (maxY - y1) / (32);
        }
        else
        {
            if (menu->maxw != menu->maxw0)
            {
                maxw = (maxY - y1) / (32);
                if (menu->maxw0 > 0)
                {
                    if (maxw > menu->maxw0)
                    {
                        menu->maxw = menu->maxw0;
                    }
                    else menu->maxw = maxw;
                }
                else
                {
                    if (maxw > menu->max)
                    {
                        menu->maxw = 0;
                    }
                    else menu->maxw = maxw;
                }
            }
        }

        if (menu->flags&NVERT)
        {
            if (x2 > maxX)
            {
                menu->maxw = (maxX - 2 * (GR + 1)) / (menu->xdl * SKALA);

                menu->xpcz = 1;
            }
            else
            {
                if (menu->maxw != menu->maxw0)
                {
                    maxw = (maxX - 2 * (GR + 1)) / (menu->xdl * SKALA);
                    if (menu->maxw0 > 0)
                    {
                        if (maxw > menu->maxw0)
                        {
                            menu->maxw = menu->maxw0;
                        }
                        else menu->maxw = maxw;
                    }
                    else
                    {
                        if (maxw > menu->max)
                        {
                            menu->maxw = 0;
                        }
                        else menu->maxw = maxw;
                    }
                }
            }
        }

    }

    else
    {
        if (menu->flags&NVERT)
        {
            if (WIDTH /*8*/ * SKALA * (menu->xpcz + menu->xdl * menu->max) > maxX)
                menu->xpcz = maxX / (WIDTH /*8*/ * SKALA) - menu->xdl * menu->max;

        }
        else if (WIDTH /*8*/ * SKALA * (menu->xpcz + menu->xdl) > maxX)
            menu->xpcz = maxX / (WIDTH /*8*/ * SKALA) - menu->xdl;


        size=menu->maxw?menu->maxw:menu->max;
        xr=((menu->flags&NVERT)?size*menu->xdl:menu->xdl);
        yd=((menu->flags&NVERT)?1:size);
        x1=(menu->xpcz-1)*WIDTH /*8*/ * SKALA;
        y1=(menu->ypcz-1)*HEIGHT * SKALA+YP;
        x2=x1+xr*WIDTH /*8*/ * SKALA +2*(GR+1);
        y2=y1+yd*HEIGHT * SKALA +2*GR;

        x2 += xs;
        y2 += ys;

        if (y2 > maxY)
        {
            menu->maxw = (maxY - y1) / (HEIGHT * SKALA) -1 ;
        }
        else
        {

            if (menu->maxw0 < ((maxY - y1) / (HEIGHT * SKALA) -1))
                menu->maxw = menu->maxw0;

        }

        if (x2 >maxX && (menu->flags&NVERT))
        {
            menu->maxw = (maxX - 2 * (GR + 1)) / (menu->xdl * WIDTH /*8*/* SKALA) ;

            menu->xpcz = 1;
        }
    }

    if (menu->maxw==0) {
        menu->poz+=menu->foff;
        menu->foff = 0;
    }
    else if (menu->flags&NVERT)
    {
        if (menu->foff > (menu->max - menu->maxw)) {
            menu->poz += (menu->foff - (menu->max - menu->maxw));
            menu->foff = (menu->max - menu->maxw);
        }
    }
    else
    {
        if (menu->poz > menu->maxw) {
            menu->foff += (menu->poz - menu->maxw + 1);
            menu->poz -= (menu->poz - menu->maxw + 1);
        }
    }

}


void Test_Menu (TMENU *menu)
//-----------------------------------
{
  int xr,yd,size,x1,y1,x2,y2;
  int maxw;
  int i, i_drawings;
  int xs=0, ys=0;

    if (menu->flags&ICONS) {

        menu->xdl = (int) (64 * SKALA);
    }

    if (menu->flags & FIXED)
    {
        Test_PMenu((PTMENU*)menu);
        return;
    }

    menu->maxw=menu->maxw0; //inicjacja

  if (menu == &menug)
  {
	  //checking how many drawings are opened
	  i_drawings = 0;
	  for (i = 0; i < MAX_NUMBER_OF_WINDOWS; i++)
	  {
		  if (Get_dane0_DATA(i) != NULL) i_drawings++;
	  }
	  if (i_drawings > 1) menu->max = GMENU_MAX;
	  else menu->max = GMENU_MAX - 1;
	  if ((menu->poz + menu->off) == (GMENU_MAX - 1)) menu->poz -= 1;
  }

    if ((BAR_POINTER) && (menu->maxw>0) && (menu->maxw<menu->max))
    {
        if (menu->flags&NVERT) ys=20;
        else xs=20;
    }

  if (menu->flags&ICONS)
  {
	  if (menu->flags&NVERT)
	  {
		  if ((WIDTH  * SKALA * menu->xpcz + menu->xdl * menu->max) > maxX)
			  menu->xpcz = (maxX - menu->xdl * menu->max) / (WIDTH  * SKALA);
	  }
	  else if ((WIDTH  * SKALA * menu->xpcz + menu->xdl + xs) > maxX)
		  menu->xpcz = (maxX - menu->xdl - xs) / (WIDTH /*8*/ * SKALA);

	  size = menu->maxw ? menu->maxw : menu->max;
	  xr = ((menu->flags&NVERT) ? size * menu->xdl : menu->xdl);
	  yd = ((menu->flags&NVERT) ? 1 : size);
	  x1 = (menu->xpcz - 1)*WIDTH  * SKALA;
	  y1 = (menu->ypcz - 1) * 32 + YP;

	  x2 = x1 + xr * SKALA + 2 * (GR + 1);
	  y2 = y1 + yd * 32 + 2 * GR;

      x2 += xs;
      y2 += ys;

	  if (y2 > maxY)
	  {
		  menu->maxw = (maxY - y1) / (32);
	  }
	  else
	  {
		  if (menu->maxw != menu->maxw0)
		  {
			  maxw = (maxY - y1) / (32);
			  if (menu->maxw0 > 0)
			  {
				  if (maxw > menu->maxw0)
				  {
					  menu->maxw = menu->maxw0;
				  }
				  else menu->maxw = maxw;
			  }
			  else
			  {
				  if (maxw > menu->max)
				  {
					  menu->maxw = 0;
				  }
				  else menu->maxw = maxw;
			  }
		  }
	  }

	  if (menu->flags&NVERT)
	  {
		  if (x2 > maxX)
		  {
			  menu->maxw = (maxX - 2 * (GR + 1)) / (menu->xdl * SKALA);

			  menu->xpcz = 1;
		  }
		  else
		  {
			  if (menu->maxw != menu->maxw0)
			  {
				  maxw = (maxX - 2 * (GR + 1)) / (menu->xdl * SKALA);
				  if (menu->maxw0 > 0)
				  {
					  if (maxw > menu->maxw0)
					  {
						  menu->maxw = menu->maxw0;
					  }
					  else menu->maxw = maxw;
				  }
				  else
				  {
					  if (maxw > menu->max)
					  {
						  menu->maxw = 0;
					  }
					  else menu->maxw = maxw;
				  }
			  }
		  }
	  }
	
  }

  else
  {
 	  if (menu->flags&NVERT)
	  {
		  if (WIDTH  * SKALA * (menu->xpcz + menu->xdl * menu->max) > maxX)
			  menu->xpcz = maxX / (WIDTH  * SKALA) - menu->xdl * menu->max;

	  }
		  else if (WIDTH  * SKALA * (menu->xpcz + menu->xdl) > maxX)
			  menu->xpcz = maxX / (WIDTH  * SKALA) - menu->xdl;
	  
	  
	  size=menu->maxw?menu->maxw:menu->max;
	  xr=((menu->flags&NVERT)?size*menu->xdl:menu->xdl);
	  yd=((menu->flags&NVERT)?1:size);
	  x1=(menu->xpcz-1)*WIDTH  * SKALA;
	  y1=(menu->ypcz-1)*HEIGHT * SKALA+YP;
	  x2=x1+xr*WIDTH  * SKALA +2*(GR+1);
	  y2=y1+yd*HEIGHT * SKALA +2*GR;

      x2 += xs;
      y2 += ys;

	  if (y2 > maxY)
	  {
		menu->maxw = (maxY - y1) / (HEIGHT * SKALA) -1 ;
	  }
	  else
      {

          if (menu->maxw0 < ((maxY - y1) / (HEIGHT * SKALA) -1))
          menu->maxw = menu->maxw0;

      }

	  if (x2 >maxX && (menu->flags&NVERT))
	  {
		menu->maxw = (maxX - 2 * (GR + 1)) / (menu->xdl * WIDTH /*8*/* SKALA) ;
		
		menu->xpcz = 1;
	  }
  }

  if (menu->maxw==0) {
      menu->poz+=menu->foff;
      menu->foff = 0;
  }
  else if (menu->flags&NVERT)
      {
          if (menu->foff > (menu->max - menu->maxw)) {
              menu->poz += (menu->foff - (menu->max - menu->maxw));
              menu->foff = (menu->max - menu->maxw);
          }
      }
      else
      {
          if (menu->poz > menu->maxw) {
              menu->foff += (menu->poz - menu->maxw + 1);
              menu->poz -= (menu->poz - menu->maxw + 1);
          }
      }

}


static int keys=0;
static int keys_int=0;
static int msx, msy;
static char msud;
static char msud_min;
static int typokn=1;
static int global_menu_flags = 0;
#ifndef LINUX
static int skokm;
#else
int skokm;
#endif

extern int text_width(char *textstring);
extern void  outtext_r(char  *textstring);
extern void  outtext_r_(BITMAP *ui_screen, char  *textstring);
extern void  Test_przycisniec(int *keys);
extern void  Odczyt_licznikow(void);
extern int osnap_(void);
extern int menupoz(void);
extern int kolorL_(void);
extern int grubosc_linii_(void);
extern int typ_linii_(void);
extern int circle_and_rectangle_proc(void);
extern int set_orig_window(void);
extern int set_lastest_window(void);



void mskp(void)
{ msx=msy=0;}

extern long l_rX, l_rY, l_krok_s ;

static void  msk (int x, int y)
{
#ifndef LINUX
  if (abs(x)>100) return;
  if (abs(y)>100) return;
#else
#endif

  if (bRedrow == TRUE) return ;
  if (mvcurb.aktoff == 1) return ;
  msx += ((int)((float)x * mouse_speed)) ;
  msy += ((int)((float)y * mouse_speed)) ;
  if (labs (l_rX + msx) > l_krok_s ||
      labs (l_rY + msy) > l_krok_s)
  {
    mvcurb.mvc = 1 ;
  }
}

 void (*MSERV)(int x,int y)=msk;

static void  pusk(int x,int y)
{
    int i,x1,y1,x2,y2,size,xr,yd, poz1, dx, dy, x11, x12, y11, y12;
    int mouse_xx, mouse_yy;
    TMENU *tmenu;

    if (menu_level==0)  return;  //just for a case

    tmenu=menu_address[menu_level-1];

    size=tmenu->maxw?tmenu->maxw:tmenu->max;
    xr=((tmenu->flags&NVERT)?size*tmenu->xdl:tmenu->xdl);
    yd=((tmenu->flags&NVERT)?1:size);
    if (tmenu->flags&ICONS)
    {
        xr = ((tmenu->flags&NVERT) ? size * 2 : 2);
        x1=(tmenu->xpcz-1)*WIDTH * SKALA;
        y1=(tmenu->ypcz-1)*32+YP;

        x2 = x1 + xr *32 * SKALA + 2 * (GR + 1);
        y2=y1+yd*32 +2*GR;

        dx = 64;
        dy = 32;
    }
    else
    {
        x1=(tmenu->xpcz-1)*WIDTH * SKALA;
        y1=(tmenu->ypcz-1)*HEIGHT * SKALA+YP;
        x2=x1+xr*WIDTH * SKALA +2*(GR+1);
        y2=y1+yd*HEIGHT * SKALA +2*GR;

        dx = 64;
        dy=HEIGHT;
    }

    msud_min = 0;
    msud = 0;

    if ((mouse_x >= x1) && (mouse_x<=x2))
    {
        if ((mouse_y >= y1) && (mouse_y<=y2))
        {
            if (tmenu->flags&NVERT)
            {
                for (i = 0; i < size; i++) {
                    x11 = x1 + i * dx;
                    x12 = x11 + dx;
                    mouse_xx = mouse_x;
                    if ((mouse_x >= x11) && (mouse_x <= x12)) break;
                }
                if (i < size) {
                    poz1 = tmenu->foff + i;
                    if (poz1 != tmenu->poz) {
                        set_scrsave_time();
                        show_hide_tip(tmenu, FALSE);
                        baronoff(tmenu);
                        tmenu->poz = poz1;
                        baron(tmenu);
                    }
                }
            }
            else
            {
                for (i = 0; i < size; i++) {
                    y11 = y1 + i * dy;
                    y12 = y11 + dy;
                    mouse_yy = mouse_y;
                    if ((mouse_y >= y11) && (mouse_y <= y12)) break;
                }
                if (i < size) {
                    poz1 = i;
                    if (poz1 != tmenu->poz) {
                        set_scrsave_time();
                        show_hide_tip(tmenu, FALSE);
                        baronoff(tmenu);
                        tmenu->poz = poz1;
                        baron(tmenu);
                    }
                }
            }
        }
    }
}

static void  usk(int x,int y)
{
    int skokmm = skokm / nsteps;  /*8*/

    dyy += (typokn ? y : x);
    dyy_min += (typokn ? y : x);

    if (dyy > skokm)
    {

        dyy = 0;

        msud = 1;

    } else if (dyy_min > skokmm)
    {

        dyy_min = 0;

        msud_min = 1;

    }

    if (dyy < -skokm)
    {

        dyy = 0;

        msud = -1;

    } else if (dyy_min < -skokmm)
    {

        dyy_min = 0;

        msud_min = -1;

    }
}

void
Set_Global_Menu_Flags (int flags)
/*------------------------------*/
{
  global_menu_flags = flags;
}

int Get_Global_Menu_Flags (void)
/*-------------------------*/
{
  return global_menu_flags;
}

static char  addpressb[20];
static char *addpress=NULL,*addpresse;

int  kbrdy(void)
{   if(addpress!=NULL) return 1;
    ////if(aktmakro!=NULL) return 1;
    return my_kbhit();
}


int getchp(void)
{ int k;
     if(addpress!=NULL)
     { k=*addpress++;
       if((char *)addpress==(char *)addpresse)
	 addpress=NULL;
	   return k;
     }
	return my_getch();
}

int getuchp(void)
{
	int k;
	if (addpress != NULL)
	{
		k = *addpress;
		addpress+=4;
		if ((char *)addpress == (char *)addpresse)
			addpress = NULL;
		return k;
	}
	return (int)my_ugetch();
}

int kls(void)
{ addpressb[0]=ENTER ;
  addpressb[1]=0;
  addpress=addpressb;
  addpresse=addpress+1;
  return 0;
}

void  kbflush(void)
{
 
	;
 }



static char  *ad;
static char  *admin=NULL,  *admax=NULL;

void DoneBuffers2(void)
{
	if (admin!=NULL) free (admin);
}

void  *get_mem_menu(void)
{
  int i_maxx ;

  i_maxx = getmaxx () ;

buf_mem_menu = imagesize(0,0,getmaxx(),getmaxy());
buf_mem_menu *= 2.0;  //TO TAKA PROTEZA, z uwagi na Liste blokow

    ad=(char*)malloc (buf_mem_menu) ;
  if (ad==NULL) return NULL;
  admax = ad + buf_mem_menu - 1 ;
  admin=ad;
  return (void*)ad;
}

void  *getmem(unsigned l)
{
  char  *ad2;
  unsigned *ad1;
  char  *adx;

  if (l < 0)
  {
    Internal_Error (__LINE__,__FILE__);
    return NULL ;
  }

  ad2=(char*)ad;

  ad1=(unsigned*) (ad+l);    //(char*)

  *ad1=l;

  adx=ad;

  adx+=l+4 ;


  if (adx>admax)
  {
    ErrList (18) ;

    return NULL;
  }
  else
  {
    ad=adx;

    return ad2;
  }
}

void free_mem(void)
{
  unsigned  *ad1;
  
  ad1=(unsigned  *)(ad-4/*2*/);


  if (ad1 <= (unsigned *) admin)  return;
  
  ad-=*ad1+4 ;

  
  if (ad < admin)
  {
//    Internal_Error (__LINE__,__FILE__) ;
    ad = admin;  /*to the rescue*/
  }
}


unsigned long_long get_free_mem_size (int way)
{
  unsigned long_long ret;
  if (way == 0)
  {
#ifdef LINUX
	  long pages = sysconf(_SC_PHYS_PAGES);
	  long page_size = sysconf(_SC_PAGE_SIZE);
	  ret= pages * page_size;
#else
	  /*
#ifdef BIT64
	  MEMORYSTATUSEX statusex;
	  statusex.dwLength = sizeof(statusex);
	  GlobalMemoryStatusEx(&statusex);
	  ret = statusex.ullTotalPhys;
#else
	  MEMORYSTATUS status;
	  GlobalMemoryStatus(&status);
	  ret= status.dwTotalPhys;
#endif
*/
	  return Get_Free_Physical_Memory();
#endif

  }
  else ret = (admax - ad) - 10;
  return ret;
}

static void ramka_m(int x1,int y1,int x2,int y2)
{  int i;
   setwritemode(COPY_PUT);
   setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
   /*podswietlona krawedz gorna zewn*/
   if (kolory.paper==15) setcolor(7); else setcolor(15);
   LINE(x1, y1, x2, y1);
   LINE(x1, y1, x1, y2);
   /*podswietlona krawedz dolna wewn*/
   if (kolory.paperm==15) setcolor(7); else setcolor(15);
   LINE(x1+GR, y2-GR+1, x2-GR, y2-GR+1);
   LINE(x2-GR+1, y1+GR, x2-GR+1, y2-GR+1);
   /*zacieniona krawedz gorna wewn*/
   if (kolory.paperm==8) setcolor(0); else setcolor(8);
   LINE(x1+GR-1, y1+GR-1, x2-GR, y1+GR-1);
   LINE(x1+GR-1, y1+GR, x1+GR-1, y2-GR);
   /*zacieniona krawedz dolna zewn*/
   if (kolory.paper==8) setcolor(0); else setcolor(8);
   LINE(x1, y2, x2, y2);
   LINE(x2, y1, x2, y2);
   setcolor(kolory.borderm);
   for (i = 1; i<(GR-1); i++)
    rectangle(x1+i,y1+i,x2-i,y2-i); 
   setcolor(kolory.paper);
   LINE(x1, y1, x1+GR-1, y1+GR-1);
   LINE(x2, y1, x2-GR+1, y1+GR-1);
   LINE(x1, y2, x1+GR-1, y2-GR+1);
   LINE(x2, y2, x2-GR+1, y2-GR+1);
}


void invert_bitmap(BITMAP *bmp)
{
    int x, y;
    int k, r, g, b;
    for (y = 0; y < bmp->h; y++) {
        for (x = 0; x < bmp->w; x++) {
            k=getpixel(bmp, x, y);
            r = k & 0xFF;
            g = (k >> 8) & 0xFF;
            b = (k >> 16) & 0xFF;
            // Invert color values
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;
            putpixel(bmp, x, y, makecol(r, g, b));
        }
    }
}

void show_hide_tip(TMENU * menu, BOOL show)
{
  int x0, x1, y1, x2, y2, a, b;
  int i, sp;
  char *ptrsz_temp0;
  char *ptrsz_temp_tadd;
  char ptrsz_temp_[128];
  int s_len, i_len;
  BITMAP *menu_tip_back;
#define TEMPN 200
  char sz_temp[TEMPN];
  int old_color;
  char *ptr;
  char *ptrsz_temp;
  int len;
  char *txt;
  char txt_[2]=" ";
  char txt__[2]="";
  PTMENU *menu_;

    if (menu!=NULL) {
        if (menu->flags & 0x100)
        {
            menu_ = menu;
            txt = &((*(menu_->pola))[0].txt[0]);
            ptrsz_temp0 = (*(menu_->pola))[menu_->foff + menu_->poz].txt;
        }
        else
        {
            txt = &((*(menu->pola))[0].txt[0]);
            ptrsz_temp0 = (*(menu->pola))[menu->foff + menu->poz].txt;
        }
    }
    else return;

	strcpy(ptrsz_temp_, ptrsz_temp0);
	ptrsz_temp = ptrsz_temp_;

	if (strlen(ptrsz_temp) == 0) return;

	len = (int)strlen(ptrsz_temp_);
	if (len == 0) return;


	ptr = strstr(ptrsz_temp, u8"֍");
	if (ptr != NULL) ptrsz_temp += 2;
	len = (int)strlen(ptrsz_temp_);
	ptr = strstr(ptrsz_temp, u8"֎");
	if (ptr != NULL) ptrsz_temp += 2;
	len = (int)strlen(ptrsz_temp_);
	ptr = strstr(ptrsz_temp, u8"҂");
	if (ptr != NULL) ptrsz_temp += 2;
	ptr = strstr(ptrsz_temp, u8"֏");
	len = (int)strlen(ptrsz_temp_);
	if (ptr != NULL) ptrsz_temp += 2;

    ptr = strstr(ptrsz_temp, u8"֊");
    len = (int)strlen(ptrsz_temp_);
    if (ptr != NULL) ptrsz_temp += 5;

	//֍ - color  ->  ֍
	//֎ - color  ->  ֎
	// Ú - line type -> ֏
	//Ů- line thickness -> ҂
    //֊005  object color xxx

	if (menu->flags&TADD)
	{
		ptrsz_temp_tadd = ptrsz_temp0 + strlen(ptrsz_temp0)+1;
		if (strlen(ptrsz_temp_tadd) > 0)
		{
			ptr = strstr(ptrsz_temp_tadd, u8"֍");
			if (ptr != NULL) ptrsz_temp_tadd += 2;
			ptr = strstr(ptrsz_temp_tadd, u8"֎");
			if (ptr != NULL) ptrsz_temp_tadd += 2;
			ptr = strstr(ptrsz_temp_tadd, u8"҂");  //formar Ů
			if (ptr != NULL) ptrsz_temp_tadd += 2;
			ptr = strstr(ptrsz_temp_tadd, u8"֏");  //former Ú
			if (ptr != NULL) ptrsz_temp_tadd += 2;

            ptr = strstr(ptrsz_temp_tadd, u8"֊");
            len = (int)strlen(ptrsz_temp_);
            if (ptr != NULL) ptrsz_temp_tadd += 5;

			strcat(ptrsz_temp, ": ");
			strcat(ptrsz_temp, ptrsz_temp_tadd);
		}
	}
    else {
        int a=0;
    }


	s_len = (int)strlen(ptrsz_temp);
	if (s_len == 0) return;
	sp = 0;
	for (i=0; i<s_len; i++)
	{
		if (*(ptrsz_temp+i) == ' ')  sp++;
		else break;
	}
	if (sp == s_len)  return; 

	i_len = TTF_text_len(ptrsz_temp);

	if (menu->flags&ICONS)
	{
		x1 = (menu->xpcz - 1)*WIDTH /*8*/ * SKALA + GR + 1;
		y1 = (menu->ypcz - 1) * 32 + GR + 1 + YP - 1;
		if (menu->flags&NVERT)
		{
			x1 += menu->poz * 64 * SKALA;
			a = 6; 
			y1 += 32 + 6;
		}
		else
		{
			y1 += menu->poz * 32;
		    a = 63;
		}

		if (menu->flags&NVERT) a -= WIDTH /*8*/ * SKALA;
		b = 32;
	}
	else if ((txt[0] == '@') && (menu->poz>=0))
	{
		//"@Rotation"
		if (*ptrsz_temp == '@')
		{
			ptrsz_temp++;

			s_len = (int)strlen(ptrsz_temp);
		}
		
		if (s_len > 0)
		{

			i_len = TTF_text_len(ptrsz_temp);
			x1 = fortips_map.x2[menu->poz]+menu->xpcz;
			y1 = fortips_map.y2[menu->poz]+menu->ypcz;
			a = 0;
			b = 0;
		}
		else return;
	}
	else
	{
		return; 

		x1 = (menu->xpcz - 1)*WIDTH  * SKALA + GR + 1;
		y1 = (menu->ypcz - 1)*(HEIGHT-2) * SKALA + GR + 1 + YP - 1;
		if (menu->flags&NVERT) x1 += menu->xdl*menu->poz*WIDTH * SKALA;
		else y1 += menu->poz*(HEIGHT-2) * SKALA;
		a = (menu->xdl) * WIDTH * SKALA;
		if (menu->flags&NVERT) a -= WIDTH /*8*/ * SKALA;
		b = (BAR_G)* SKALA;
	}

	
	x0 = x1;

	x1 = x1 + a + 8;
	y2 = y1 + 2 * (HEIGHT-2) * SKALA;
	x2 = x1 + i_len + 6;

	if (x2 > maxX)
	{
		x1 = x0 - i_len - 14;
		x2 = x0 - 8;
	}

    if (BAR_POINTER) show_mouse(NULL);

	if (show == TRUE)
	{
		if ((menu_tip_back = create_bitmap(x2 - x1 + 1, y2 - y1 + 1)) == NULL) return;
		menu->tip_back = menu_tip_back;

		getimage(x1, y1, x2, y2, menu->tip_back);
		menu->xtip = x1;
		menu->ytip = y1;

		old_color = getcolor();
		setwritemode(COPY_PUT);
		setcolor(18);  //inkk
		setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
		
		RECTFILL(x1, y1, x2, y2);
		setcolor(113);
		LINE(x1 + 1, y1 + 1, x2 - 1, y1 + 1);
		LINE(x2 - 1, y1 + 1, x2 - 1, y2 -1);
		LINE(x2 - 1, y2 -1, x1 + 1, y2 -1);
		LINE(x1 + 1, y2 -1 , x1 + 1, y1 + 1);

		moveto(x1 + 3, y1 + 0.6  * (HEIGHT-2) * SKALA);
		setcolor(kolory.inkm);
		strcpy(sz_temp, ptrsz_temp);
		outtext_r(sz_temp);
		setcolor(old_color);
	}
	else
	{
		if (menu->flags & TIP)
		{
			menu->flags &= ~TIP;
		}
		if (menu->tip_back != NULL)
		{
		putimage(menu->xtip, menu->ytip, menu->tip_back, COPY_PUT);
		menu->tip_back = NULL;
	    }
	}

    if (BAR_POINTER) show_mouse(screen);

}


void tip_frame(int x1, int y1, int x2, int y2)
{
    setwritemode(COPY_PUT);
    setcolor(18);  //inkk
    setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);

    RECTFILL(x1, y1, x2, y2);
    setcolor(113);
    LINE(x1 + 1, y1 + 1, x2 - 1, y1 + 1);
    LINE(x2 - 1, y1 + 1, x2 - 1, y2 -1);
    LINE(x2 - 1, y2 -1, x1 + 1, y2 -1);
    LINE(x1 + 1, y2 -1 , x1 + 1, y1 + 1);
    setcolor(kolory.inkm);
}

int frame_up(TMENU * menu)
{
	int x0, y0, x1, y1, a, b;
    int nn;
	int dys;
   
	if (menu->back == NULL) return 1;

	nn = -nsteps;
	
	if ((menu->flags&ICONS) && ((menu->poz + menu->foff) >=0))
	{
		x1 = (menu->xpcz - 1)*WIDTH /*8*/ * SKALA + GR + 1;
		x0 = x1;
		y1 = (menu->ypcz - 1) * 32 + GR + 1 + YP - 1;
		y0 = y1;
		if (menu->flags&NVERT)
		{
	
			x1 += menu->poz * 64 * SKALA;

			a = 64;
		}
		else
		{
			y1 += menu->poz * 32;
			
			a = 63;
		}

		b = 32;

		if (menu->flags&NVERT) {
			if  ((x1 + (step *2 * frame_counter)) <= x0)
			{
				return 1;
			}
		}
		
		else { 
		       
				if (menu->foff > 0) dys = step*nsteps;
				else dys = 0;
				if ((y1 + (step * frame_counter)) <= (y0-dys))  //<=
				{
					return 1;
				}
			}

			setwritemode(XOR_PUT);
			setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
		 
			memcpy(&xy, &frames.frame[menu_level], sizeof(xy));
			
			LINE(xy.x1, xy.y1, xy.x2, xy.y1);
			LINE(xy.x2, xy.y1, xy.x2, xy.y2);
			LINE(xy.x2, xy.y2, xy.x1, xy.y2);
			LINE(xy.x1, xy.y2, xy.x1, xy.y1);
		
			frame_counter -= 1;
			if (menu->flags&NVERT)
			{
				xy.x1 = x1 + (step * 2/*8*/ * frame_counter); 
				xy.y1 = y1; 
				xy.x2 = xy.x1 + a;
				xy.y2 = y1 + b;
			}
			else
			{
				xy.x1 = x1; 
				xy.y1 = y1 + (step /*4*/ * frame_counter); 
				xy.x2 = x1 + a; 
				xy.y2 = xy.y1 + b;
			}

			LINE(xy.x1, xy.y1, xy.x2, xy.y1);
			LINE(xy.x2, xy.y1, xy.x2, xy.y2);
			LINE(xy.x2, xy.y2, xy.x1, xy.y2);
			LINE(xy.x1, xy.y2, xy.x1, xy.y1);
			
			memcpy(&frames.frame[menu_level], &xy, sizeof(xy));

			frame_count -= 1;
			if (frame_counter <= -(nsteps)) //4)
			{
				set_scrsave_time();
				frame_count = nsteps; // 0;
				frame_counter = nsteps; // 0;
				cprev_(menu);
				return 1;
			}
	}
	else if (menu->poz + menu->off >= 0)
	{

			frame_counter -= 1;
			frame_count -= 1;
			if (frame_counter <= -(nsteps)) //4)
			{
				set_scrsave_time();
				frame_count = nsteps; // 0;
				frame_counter = nsteps; // 0;
				cprev_(menu);
				return 1;
			}
	}
	return 0;
}

void frame_off(TMENU * menu)
{
	int x1, y1, a, b;
	
	if (menu->back == NULL)
        return;


    if (BAR_POINTER) return;

    if ((menu->flags&ICONS) && (menu->poz < (menu->max)))
	{
		x1 = (menu->xpcz - 1)*WIDTH /*8*/ * SKALA + GR + 1;
		y1 = (menu->ypcz - 1) * 32 + GR + 1 + YP - 1;
		
		if (menu->flags&NVERT)
		{
			x1 += menu->poz * 64 * SKALA;
			
			a = 64;
		}
		else
		{
			y1 += menu->poz * 32;
			
			a = 63;
		}

		b = 32;

		setwritemode(XOR_PUT);
		setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
		
		memcpy(&xy, &frames.frame[menu_level], sizeof(xy));

		LINE(xy.x1, xy.y1, xy.x2, xy.y1);
		LINE(xy.x2, xy.y1, xy.x2, xy.y2);
		LINE(xy.x2, xy.y2, xy.x1, xy.y2);
		LINE(xy.x1, xy.y2, xy.x1, xy.y1);

	}

}

void pointer_on(TMENU * menu)
{
    int x1, y1, a, b;

    if (menu->back == NULL) return;
    if ((menu->flags&ICONS) && (menu->poz < menu->max))
    {
        x1 = (menu->xpcz - 1)*WIDTH /*8*/ * SKALA + GR + 1;
        y1 = (menu->ypcz - 1) * 32 + GR + 1 + YP - 1;

        if (menu->flags&NVERT)
        {
            x1 += menu->poz * 64 * SKALA;

            a = 64;
        }
        else
        {
            y1 += menu->poz * 32;

            a = 63;
        }

        b = 32;

        position_mouse(x1 + a/2, y1 + b/2);
    }
}

void frame_on(TMENU * menu)
{
	int x1, y1, a, b;
	
 	if (menu->back == NULL) return;

    if (BAR_POINTER) return;
	
	if ((menu->flags&ICONS) && (menu->poz < menu->max))
	{
		x1 = (menu->xpcz - 1)*WIDTH /*8*/ * SKALA + GR + 1;
		y1 = (menu->ypcz - 1) * 32 + GR + 1 + YP - 1;

		if (menu->flags&NVERT)
		{
			x1 += menu->poz * 64 * SKALA;
			
			a = 64;
		}
		else
		{
			y1 += menu->poz * 32;
			
			a = 63;
		}

		b = 32;

		setwritemode(XOR_PUT);
		setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);

		xy.x1 = x1; 
		xy.y1 = y1; 
		xy.x2 = x1 + a; 
		xy.y2 = y1 + b ;

		LINE(xy.x1, xy.y1, xy.x2, xy.y1);
		LINE(xy.x2, xy.y1, xy.x2, xy.y2);
		LINE(xy.x2, xy.y2, xy.x1, xy.y2);
		LINE(xy.x1, xy.y2, xy.x1, xy.y1);

		memcpy(&frames.frame[menu_level], &xy, sizeof(xy));
		
		frame_count = 0;
	}

}

int frame_down(TMENU * menu)
{
	int x0, y0, xm, ym, x1, y1, x2, y2, a, b;
    int nn;
    int cn;
    int mouse_x_, mouse_y_;
    float yd, xr;
    int size;

    nn = nsteps;

    size=menu->maxw?menu->maxw:menu->max;

	if ((menu->flags&ICONS) && (menu->poz < (menu->max)))
	{
        xr = ((menu->flags&NVERT) ? size * 2 : 2);
        yd=((menu->flags&NVERT)?1:size);

		x1 = (menu->xpcz - 1)*WIDTH /*8*/ * SKALA + GR + 1;
		x0 = x1;
		y1 = (menu->ypcz - 1) * 32 + GR + 1 + YP - 1;
		y0 = y1;

        x2 = x1 + xr * 32 * SKALA + 2 * (GR + 1);
        y2=  y1+yd*32  +2*GR;
		
		if (menu->flags&NVERT)
		{
			x1 += menu->poz * 64 * SKALA;
			xm = x0 + 64*menu->max* SKALA - GR; 
			a = 64;
		}
		else
		{
			y1 += menu->poz * 32;
			ym = y0 + 32*menu->max;  
			a = 63;
		}

		b = 32;

		if (menu->flags&NVERT) {
			if ((x1 + a + (step * 2 * frame_counter)) > xm)
			{
				return 1;
			}
		}
		else {
			if ((y1 + (step * frame_counter) + b)>= ym)
			{
				return 1;
			}
			
			if (menu->poz + menu->foff == menu->max - 1)
			{
				return 1;
			}
		}

			setwritemode(XOR_PUT);
			setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
			
			memcpy(&xy, &frames.frame[menu_level], sizeof(xy));

			LINE(xy.x1, xy.y1, xy.x2, xy.y1);
			LINE(xy.x2, xy.y1, xy.x2, xy.y2);
			LINE(xy.x2, xy.y2, xy.x1, xy.y2);
			LINE(xy.x1, xy.y2, xy.x1, xy.y1);

			frame_counter += 1;
			if (menu->flags&NVERT)
			{
				xy.x1 = x1 + (step *2 /*8*/ * frame_counter); 
				xy.y1 = y1; 
				xy.x2 = xy.x1 + a;
				xy.y2 = y1 + b;
			}
			else
			{
				xy.x1 = x1; 
				xy.y1 = y1 + (step /*4*/ * frame_counter); 
				xy.x2 = x1 + a; 
				xy.y2 = xy.y1 + b;
			}
			
			LINE(xy.x1, xy.y1, xy.x2, xy.y1);
			LINE(xy.x2, xy.y1, xy.x2, xy.y2);
			LINE(xy.x2, xy.y2, xy.x1, xy.y2);
			LINE(xy.x1, xy.y2, xy.x1, xy.y1);

			memcpy(&frames.frame[menu_level], &xy, sizeof(xy));

			frame_count += 1;
			if (frame_counter >= (nsteps)) //4)
			{
				set_scrsave_time();
				frame_count = 0;
				frame_counter = 0;
				cn=cnext_(menu);

                /*
                if (BAR_POINTER)
                {
                    mouse_x_=mouse_x;
                    mouse_y_=mouse_y;
                    if ((mouse_x >= x1) && (mouse_x<= x2))
                    {
                        if ((mouse_y >= y1) && (mouse_y <= y2))
                        {
                             position_mouse(mouse_x_, (xy.y2-xy.y1)/2);
                        }
                    }
                }
                 */

				return 1;
			}
			
	}
	else if (menu->poz < (menu->max))
	{
			frame_counter += 1;
			frame_count += 1;
			if (frame_counter >= (nsteps)) //4)
			{
				set_scrsave_time();
				frame_count = 0;
				frame_counter = 0;
				cn=cnext_(menu);
				return 1;
			}

	}
	return 0;
}

void baronoff_(TMENU  * menu)
{
    int x1, y1, a, b;
    char *ptrsz_temp ;
    char *ptr;
    int kolor_m;
    BOOL color_bar=FALSE;
    int l_no=3;
    char *txt;
    char txt_[2]=" ";
    char txt__[2]="";
    PTMENU *menu_;

    if (menu!=NULL) {
        if (menu->flags & 0x100)
        {
            menu_ = menu;
            txt = &((*(menu_->pola))[0].txt[0]);
            ptrsz_temp = (*(menu_->pola))[menu_->foff + menu_->poz].txt;
        }
        else
        {
            txt = &((*(menu->pola))[0].txt[0]);
            ptrsz_temp = (*(menu->pola))[menu->foff + menu->poz].txt;
        }
    }
    else return;

    if  ((menu->poz + menu->foff) >= (menu->max))
    {
        if (menu->maxw>0) {
            menu->poz = menu->maxw-1;
            menu->foff = menu->max-menu->maxw;
        }
        else {
            menu->poz = menu->max-1;
            menu->foff = 0;
        }

    }

    frame_counter = 0;

    ptrsz_temp = (*(menu->pola))[menu->foff + menu->poz].txt;  //???

    if (menu->flags&ICONS)
    {
        x1=(menu->xpcz-1)*WIDTH /*8*/ * SKALA+GR+1;
        y1=(menu->ypcz-1)*32+GR+1+YP -1;
        if (menu->flags&NVERT)
        {
            x1 += menu->poz*64 * SKALA;

            a = 64;
        }
        else
        {
            y1 += menu->poz * 32;

            a = 63;
        }

        b=32;
        l_no=4;
    }
    else
    {
        x1=(menu->xpcz-1)*WIDTH /*8*/ * SKALA+GR+1;
        y1=(menu->ypcz-1)*HEIGHT * SKALA+GR+1+YP -1;
        if(menu->flags&NVERT) x1+=menu->xdl*menu->poz*WIDTH/*8*/ * SKALA;
        else y1+=menu->poz*HEIGHT * SKALA;
        a=(menu->xdl) * WIDTH * SKALA;
        if(menu->flags&NVERT) a-=WIDTH /*8*/ * SKALA;
        b=(BAR_G/*+1*/) * SKALA;
    }

    ptr=strstr(ptrsz_temp,u8"֎");
    if ((ptr!=NULL) || (menu==&mRegion))
    {
        setcolor(15);
        color_bar=TRUE;
    }
    else
    {
        ptr=strstr(ptrsz_temp,u8"֍");
        if ((ptr!=NULL))
        {

            setcolor(15);
            color_bar=TRUE;
        }
        else
        {
            ptr = strstr(ptrsz_temp, u8"֊");
            if ((ptr != NULL))
            {

                setcolor(15);
                color_bar = TRUE;
            }

            else setcolor(15);
        }
    }

    ////setwritemode(XOR_PUT);
    ////setlinestyle1(SOLID_LINE,0,NORM_WIDTH);

    if (color_bar)
    {
        setwritemode(XOR_PUT);
        setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
        for (int i = 0; i < l_no; i++)
        {
            LINE(x1 + i, y1 + i, x1 + a - i, y1 + i);
            LINE(x1 + a - i, y1 + i, x1 + a - i, y1 + b - i);
            LINE(x1 + a - i, y1 + b - i, x1 + i, y1 + b - i);
            LINE(x1 +i, y1 + b - i, x1 + i, y1 + i);
        }
    }
    else
    {
        if (XORBAR)
        {
            setwritemode(XOR_PUT);
            setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
            RECTFILL(x1, y1, x1 + a, y1 + b);
        }
        else {
            setwritemode(COPY_PUT);
            BITMAP *barbitmap;
            barbitmap = create_bitmap(a, b);
            getimage(x1, y1, x1 + a, y1 + b, barbitmap);
            invert_bitmap(barbitmap);
            putimage(x1, y1, barbitmap, COPY_PUT);
        }
    }

    bar_center.x=x1+a/2;
    bar_center.y=y1+b/2;

    frame_on(menu);
}

void baronoff(TMENU  * menu)
{
  frame_counter = 0;
  frame_off(menu);

  if (BAR_POINTER) show_mouse(NULL);

  baronoff_(menu);

  if (BAR_POINTER) show_mouse(screen);

}

static char *get_icons_p(int number)
{

	char* icons[] = { icon_empty_p,icon_block_p,icon_modify_p,icon_search_p,icon_explode_text_to_letters_p,icon_dim_horizontal_p,icon_hatch_p,icon_polygonal_aread_p,icon_create_macro_p,icon_drawing_settings_p,icon_settings_p,icon_open_p,icon_quit_p,
		/*13*/    icon_line_p,icon_circle_p,icon_arc_p,icon_text_p,icon_polygon_p,icon_disc_p,icon_ellipses_p,icon_polylines_p,icon_solid_p,icon_line_rectangle_p,icon_trace_p,icon_2line_p,icon_3line_p,icon_line_freehand_p,icon_point_p,icon_chart_p,icon_reinforcing_p,
		/*30*/	  icon_move_p,icon_move_copy_p,icon_delete_p,icon_restore_p,icon_rotate_p,icon_rotate_block_p,icon_scale_p,icon_stretch_p,icon_edit_text_p,
		/*39*/ 	  icon_mirror_p,icon_mirror_block_x_p,icon_mirror_block_y_p,icon_array_p,icon_change_p,icon_trim_segments_p,icon_cutout_p,icon_extend_p,icon_fillet_p,icon_chamfer_p,icon_offset_p,icon_divide_p,icon_undo_p,
		/*52*/    icon_pointfinder_p,icon_camera_p,icon_parameters_p,icon_layers_p,icon_measure_p,icon_draft_p,
		/*58*/    icon_copy_p,icon_paste_p,icon_import_block_p,icon_export_block_p,icon_import_image_p,icon_edit_image_p,icon_export_image_p,icon_group_p,icon_explode_p,icon_block_list_p,icon_explode_frozen_p,icon_delete_dxf_blocks_p,icon_correct_blocks_p,icon_fixed_layers_p,
		/*72*/    icon_area_p,icon_center_of_area_p,icon_first_moment_of_area_p,icon_area_moment_of_inertia_p,icon_centr_area_mom_of_inertia_p,icon_test_of_closed_area_p,
		/*78*/    icon_import_p, icon_new_p, icon_new_window_p, icon_save_p, icon_save_as_p, icon_export_dxf_p, icon_import_dxf_p, icon_edit_title_block_p, icon_cmd_p, icon_print_p, icon_infobox_p, icon_last_files_p,
		/*90*/    icon_change_texts_p, icon_auto_numbering_p, icon_record_p, icon_play_p, icon_load_p, icon_save_as_p, icon_format_p, icon_dscale_p, icon_accuracy_p, icon_units_p, icon_coordinates_p,
		/*101*/   icon_local_p, icon_pointer_p, icon_edit_ini_p, icon_cartesian_p, icon_geodetic_p, icon_point_origin_p, icon_axis_angle_p, icon_rotation_angle_p, icon_colorwheel_p, icon_panoramic_p, icon_font_type_p,
		/*112*/   icon_yes_p, icon_no_p, icon_dx_dy_p, icon_calibrate_p, icon_A_rectangular_p, icon_A_circular_p, icon_A_circular_rot_p,
		/*119*/   icon_pointmode_p, icon_endpoint_p, icon_nearest_end_p, icon_middlepoint_p, icon_intersection_p, icon_center_p, icon_perpendicular_p, icon_tangential_p, icon_nearest_p, icon_onpoint_p, icon_move2xy_p, icon_movedxdy_p, icon_move_polar_p, icon_nopointfinder_p,
		/*133*/   icon_calculator_p, icon_point_measure_p, icon_distance_point_to_point_p, icon_angle_line_to_line_p, icon_plusminus_p, icon_property_list_p, icon_noplusminus_p,
		/*140*/   icon_addfavourite_p, icon_findfavourite_p, icon_refresh_p, icon_zoom_all_p, icon_zoom_center_p, icon_auto_panorama_p, icon_zoom_1_p, icon_zoom_2_p, icon_zoom_in_p, icon_zoom_out_p, icon_zoom_x_p, icon_zoom_window_p, icon_zoom_drawing_p, icon_zoom_previous_p,
		/*154*/   icon_pointer_step_p, icon_grid_density_p, icon_grid_points_p, icon_restr_ortho_p, icon_color_p, icon_line_thickness_p, icon_line_type_p,
		/*161*/   icon_point_visibility_p, icon_point_size_p, icon_get_color_p, icon_get_type_p, icon_get_layer_p, icon_floating_text_p, icon_floating_block_p, icon_dim_scale_p, icon_stretch_in_pin_p, icon_break_in_pin_p,
		/*171*/   icon_mask_p, icon_hidden_text_vis_p, icon_image_points_p, icon_highlight_layer_p, icon_dbuff_image_p, icon_text_most_readable_p,
		/*177*/   icon_line_color_p, icon_text_color_p, icon_text_height_p, icon_width_p, icon_italic_p, icon_bold_p, icon_font_type_p, icon_accuracy_p, icon_terminators_p, icon_line_offset_p, icon_text_direction_p, icon_extension_lines_p,
		/*189*/   icon_dim_arrowheads_p, icon_dim_ticks_p, icon_dim_dots_p, icon_text_parallel_p, icon_text_horizontal_p, icon_fixed_length_p, icon_align_to_object_p,
		/*196*/   icon_dim_direction_p, icon_dim_new_p, icon_dim_remove_p, icon_dim_edit_p, icon_dim_settings_p, icon_dim_arc_RL_p, icon_dim_circle_DR_p,
		/*203*/   icon_dim_horizontal_p, icon_dim_vertical_p, icon_dim_aligned_p, icon_dim_linear_p, icon_dim_linearX_p, icon_dim_angular_p, icon_dim_arc_radial_p, icon_dim_arc_length_p, icon_dim_diametric_p, icon_dim_radial_p,
		/*213*/   icon_circle_center_radius_p, icon_circle_center_diameter_p, icon_circle_2_points_p, icon_circle_3_points_p,
		/*217*/   icon_line_close_p, icon_line_remove_p, icon_trim_value_p, icon_line_continue_p,
		/*221*/   icon_arc_3_points_p, icon_arc_beginning_center_end_p, icon_arc_beginning_center_angle_p, icon_arc_beginning_center_chord_p, icon_arc_beginning_end_radius_p, icon_arc_beginning_end_angle_p, icon_arc_beginning_end_direction_p, icon_arc_continuation_p,
		/*229*/   icon_text_angle_p, icon_text_width_p, icon_text_type_p, icon_hidden_text_p, icon_text_align_p, icon_text_interval_p,
		/*235*/   icon_line_polygon_cor_cor_p, icon_line_polygon_cen_cor_p, icon_line_polygon_cen_tan_p,
		/*238*/   icon_ellipse_approximation_p, icon_ellipse_axis_center_p,
		/*240*/   icon_ellipse_axis_start_p, icon_ellipse_center_p,
		/*242*/   icon_solid_quad_triang_p, icon_solid_triangular1_p, icon_solid_fill_line_width_p, icon_solid_quadrangular_p, icon_solid_triangular_p,
		/*247*/   icon_arc_close_p, icon_arc_remove_p,
		/*249*/   icon_trace_width_p, icon_trace_axis_offset_p, icon_trace_polyline_p, icon_trace_fill_line_width_p, icon_trace_remove_p,
		/*254*/   icon_chart_step_dx_p, icon_ftfunct_p,
		/*256*/   icon_last_block_p, icon_select_all_p, icon_all_layers_p, icon_select_cross_window_p, icon_add_remove_p, icon_gear1_p,
		/*262*/   icon_text_align_baselineleft_p, icon_text_align_baselineright_p, icon_text_align_baselinecenter_p, icon_text_align_middlecenter_p,
		/*266*/   icon_circle_tangential_p, icon_axis_length_p, icon_dash_dotted_p, icon_dashed_p, icon_return_all_p, icon_change_mode_p, icon_change_property_p, icon_all_texts_p, icon_select_window_p, icon_select_cross_p,
		/*276*/   icon_solid_filled_p, icon_solid_very_thin_p, icon_solid_thin_p, icon_solid_thick_p, icon_solid_very_thick_p, icon_solid_extra_thick_p,
		/*282*/   icon_text_angle_0_p, icon_text_angle_90_p, icon_text_parallel_to_object_p, icon_text_perpendicular_p, icon_text_angle_to_object_p, icon_text_angle_x_p,
		/*288*/   icon_trace_filled_p, icon_trace_very_thin_p, icon_trace_thin_p, icon_trace_thick_p, icon_trace_very_thick_p, icon_trace_extra_thick_p,
		/*294*/   icon_point_simple_p, icon_point_base_p, icon_point_pin_p,
		/*297*/   icon_RAM_for_drawing_p, icon_RAM_virtual_image_p, icon_RAM_data_size_p, icon_RAM_printing_buffer_p, icon_RAM_macro_buffer_p, icon_RAM_image_buffer_p, icon_current_folder_p, icon_desktop_size_p,
		/*305*/   icon_layers_info_p, icon_x1_p, icon_y1_p, icon_x2_p, icon_y2_p, icon_x3_p, icon_y3_p, icon_x4_p, icon_y4_p,
		/*314*/   icon_angle1_p, icon_angle2_p, icon_dim_dx_p, icon_dim_dy_p, icon_inner_block_name_p, icon_outer_block_name_p,
		/*320*/   icon_qr_code1_p, icon_edit_p, icon_grey_p, icon_blackwhite_p, icon_plotter_s_p, icon_visible_p,
		/*326*/   icon_visible_d_p, icon_edit_d_p, icon_pointfinder_d_p, icon_color_d_p, icon_line_thickness_d_p, icon_line_type_d_p, icon_grey_d_p, icon_blackwhite_d_p,
		/*334*/   icon_visible_db_p, icon_edit_db_p, icon_pointfinder_db_p, icon_grey_db_p, icon_blackwhite_db_p,
		/*339*/   icon_plotter_64_p, icon_saveimage_64_p, icon_rotation_d48_p, icon_inversion_d48_p, icon_reflection_d48_p, icon_grey_d48_p, icon_blackwhite_d48_p,  icon_grey_image_d48_p,
		/*347*/   icon_page_d48_p, icon_scale_d48_p, icon_window_d48_p, icon_sheet_d48_p,
		/*351*/   icon_tools_p, icon_features_p, icon_ctrl_p, icon_alt_p, icon_shift_p, icon_home_p, icon_end_p, icon_esc_p, icon_ins_p, icon_pgdn_p, icon_pgup_p, icon_spc_p, icon_tab_p,
		/*364*/   icon_0_p,  icon_1_p, icon_2_p, icon_3_p, icon_4_p, icon_5_p, icon_6_p, icon_7_p, icon_8_p, icon_9_p, icon_plus_p, icon_minus_p,
		/*376*/   icon_f1_p, icon_f2_p, icon_f3_p, icon_f4_p, icon_f5_p, icon_f6_p ,icon_f7_p, icon_f8_p, icon_f9_p, icon_f10_p, icon_f11_p, icon_f12_p,
		/*388*/   icon_mouse3_p, icon_mouseR_p, icon_measure_tape_p, icon_spa_p, icon_ce_p, icon_look_down_p,
		/*394*/   icon_plotter_32_p, icon_cartridge_d48_p, icon_very_thin_d_p, icon_thin_d_p, icon_thick_d_p, icon_very_thick_d_p, icon_extra_thick_d_p,
		/*401*/   icon_color_d48_p, icon_scale_d_p, icon_page_d_p, icon_page_width_d_p, icon_page_height_d_p, icon_page_overlap_d_p,
		/*407*/   icon_color256_d48_p, icon_multiline_text_p, icon_underlined_p, icon_line_spacing_p,
		/*411*/   icon_bold_d_p,icon_font_type_d_p, icon_hidden_text_d_p, icon_italic_d_p, icon_layers_d_p, icon_text_align_d_p, icon_text_height_d_p, icon_text_type_d_p, icon_width_d_p, icon_underlined_d_p,
		/*421*/   icon_hor_dpi_sphere_blue_d_p, icon_ver_dpi_sphere_blue_d_p, icon_hor_size_image_d_p, icon_ver_size_image_d_p, icon_hor_scale_image_d_p, icon_ver_scale_image_d_p, icon_background_image_d_p, icon_rotate_angle_image_d_p,
		/*429*/   icon_top_margin_d_p, icon_left_margin_d_p, icon_actual_size_d48_p, icon_bottom_margin_d_p, icon_right_margin_d_p,
		/*434*/   icon_pattern_p, icon_pattern_angle_p, icon_pattern_scale_p, icon_edit_point_origin_p, icon_set_point_origin_p, icon_pattern_line_distance_p,
		/*440*/   icon_anglex_p, icon_angle45_p, icon_angle90_p, icon_angle135_p, icon_angle180_p, icon_angle225_p, icon_angle270_p, icon_angle315_p,
		/*448*/   icon_divide_count_p, icon_divide_counter_p, icon_divide_measure_p, icon_divide_segment_length_p, icon_divide_counter_max_p, icon_divide_polyline_segment_p, icon_divide_polyline_p, icon_divide_segment_p,
		/*456*/   icon_align_block_p, icon_offset_point_p, icon_offset_distance_p, icon_return_p,
		/*460*/   icon_open_file_folder_p, icon_arrow_up_d48_p, icon_arrow_down_d48_p, icon_folder_d48_p, icon_folder0_d48_p, icon_bigger_d48_p, icon_smaller_d48_p,
		/*467*/   icon_expand_hor_p, icon_expand_ver_p, icon_expand_diag_p, icon_expand_flex_p, icon_expand_last_p,
		/*472*/   icon_arrow_up_d_p, icon_arrow_down_d_p, icon_pgup_d_p, icon_pgdn_d_p, icon_arrow_up_left_d_p, icon_arrow_down_left_d_p, icon_expand_win_p,
		/*479*/   icon_plotter_start_d_64_p, icon_printer_start_d_64_p, icon_escape_d_64_p, icon_save_d_p, icon_load_d_p, icon_return_d_24_p,
		/*485*/   icon_frame_p, icon_zones_around_p, icon_zones_prefix_p, icon_zones_first_number_p, icon_zones_reversed_p, icon_zone_height_p,
		/*491*/   icon_zone_width_p, icon_margin_width_p, icon_frame_line_p, icon_zones_line_p, icon_frame_color_p, icon_zones_color_p, icon_style2_black_p, icon_zones_top_p, icon_zones_bottom_p, icon_zones_top_bottom_p, icon_frame_offset_p,
		/*502*/   icon_ok_d_64_p, icon_new_layer_d_64_p,
		/*504*/   icon_export_cad_p, icon_import_cad_p, icon_export_dwg_p, icon_import_dwg_p,
		/*508*/   icon_printer_d_48_p, icon_hor_extents_d_p, icon_ver_extents_d_p, icon_hor_limits_d_p, icon_ver_limits_d_p, icon_units_d_p, icon_paper_size_d_48_p, icon_paper_dim_d_48_p,
		/*516*/   icon_bspline_p, icon_bspline34_p, icon_bspline3p_p, icon_bspline4_p, icon_bspline3_p,
		/*521*/   icon_yes_d_48_p, icon_no_d_48_p, icon_escape_d_48_p, icon_yes_d_pmem, icon_no_d_pmem, icon_escape_d_p,
		/*527*/   icon_tartan_p, icon_all_windows_p, icon_junction_p, icon_save_window_p, icon_freehand_segment_length_p, icon_remove_last_freehand_p,
		/*533*/   icon_new_from_template_p, icon_idea_p, icon_mirror_block_p, icon_tree_p, icon_tree_enter_p, icon_tree_return_p,
		/*539*/   icon_trans30_p, icon_trans40_p, icon_trans50_p, icon_trans60_p, icon_trans70_p, icon_trans80_p, icon_trans90_p, icon_trans100_p,
		/*547*/   icon_back_p, icon_space_p,
		/*549*/   icon_A_p, icon_B_p, icon_C_p, icon_D_p, icon_E_p, icon_F_p, icon_G_p, icon_H_p, icon_I_p, icon_J_p, icon_K_p, icon_L_p,
		/*561*/   icon_M_p, icon_N_p, icon_O_p, icon_P_p, icon_Q_p, icon_R_p, icon_S_p, icon_T_p, icon_U_p, icon_V_p,
		/*571*/	  icon_W_p, icon_X_p, icon_Y_p, icon_Z_p, icon_load_d_48_p, icon_load_ini_d_p, icon_load_ini_d_48_p,
		/*578*/   icon_angle0_p, icon_angleRX_p, icon_anglePto_p, icon_angleRto_p, icon_angleXtoP_p, icon_touch_id_db_64_p,
		/*584*/   icon_left_margin_d_48_p, icon_bottom_margin_d_48_p, icon_all_layers_db_64_p, icon_global_db_48_p, icon_local_db_48_p, icon_color256_db_48_p,
		/*590*/   icon_cursors_p, icon_cursor_small_p, icon_cursor_big_p, icon_question_mark_db_48_p, icon_pointern_p, icon_pointerb_p, icon_pdf_vector_64_p,
		/*597*/   icon_pattern_folder_p, icon_close_window_p, icon_change_pattern_p, icon_import_map_p, icon_import_png_p, icon_paper_size_d_p,
		/*603*/   icon_mirror_leave_p, icon_mirror_replace_p, icon_set_offset_distance_p, icon_polygon_n_p, icon_A_circular_n_p, icon_A_circular_angle_p, icon_A_circular_rot_n_p, icon_A_circular_rot_angle_p,
		/*611*/   icon_starAstar_p, icon_starA_p, icon_Astar_p, icon_Aonly_p, icon_gear_p, icon_confirm_or_deny_p, icon_stonewall_p, icon_folder_bd32_p, icon_folder_bd48_p, icon_folder_bd64_p,icon_arrow_up_end_d48_p,
		/*622*/   icon_brickwall_dx_p, icon_brickwall_dy_p, icon_import_jpg_p, icon_chain_d_p, icon_on_top_d_p, icon_h_flip_p, icon_v_flip_p,
		/*629*/   icon_offset_style_p, icon_offset_normal_p, icon_offset_smooth, icon_trace_close_p, icon_trace_break_p, icon_background_image_d48_p,
		/*635*/   icon_UA_B_p, icon_UA_D_p, icon_UA_E_p, icon_UA_J_p, icon_UA_K_p, icon_UA_N_p, icon_UA_P_p, icon_UA_R_p, icon_UA_S_p, icon_UA_T_p, icon_UA_C_p,
		/*646*/   icon_spline_points_p, icon_spline_points_close_p, icon_spline_amend_p, icon_spline_points_end_p,
		/*650*/   icon_del_p, icon_ent_p, icon_er_p, icon_k_right_p, icon_k_left_p, icon_k_up_p, icon_k_down_p,
		/*657*/   icon_mouse1b_p, icon_mouse2b_p, icon_mouse3b_p, icon_mouseRb_p, icon_education_p,
        /*662*/   icon_spline_control_points_p,
		/*663*/   icon_trans05_p, icon_trans10_p, icon_trans15_p, icon_trans20_p, icon_trans25_p, icon_trans35_p, icon_trans45_p,
		/*670*/   icon_trans55_p, icon_trans65_p, icon_trans75_p, icon_trans85_p, icon_trans95_p, icon_trans50_d_p, icon_refresh_d_p,
		/*677*/   icon_mickey_hand_s_p,
        /*678*/   icon_dim_leader_p, icon_insulation_p, icon_ins_simple_p, icon_ins_complex_p, icon_quad_trans_p, icon_rect_angle_opt_p, icon_move_rectangle_p, icon_rect_options_p,
        /*686*/   icon_object_polyline_p, icon_entire_polyline_p, icon_single_object_p, icon_image_dx_p, icon_image_dy_p, icon_break_options_p, icon_break_and_leave_p, icon_break_and_cut_p, icon_break_divide_p,
        /*695*/   icon_fixed_scale_p, icon_text_collineal_p, icon_decimal_fractional_p, icon_import_vectorize_p,
        /*699*/   icon_space128_p, icon_backspace128_p, icon_enter128_p, qmark_p, icon_ttf_p, icon_otf_p,
        /*705*/   icon_elliptical_p, icon_ellipticalarc_p, icon_filledellipses_p, icon_ellipticalarclocal_p, icon_ellipticalarcglobal_p, icon_dim_radial_y_p,
        /*711*/   icon_dim_radial_y_dim_p, icon_dim_radial_y_point_p, icon_dim_radial_y_tangential_p, icon_arc_line_continuation_p, icon_line_arc_continuation_p, icon_arrow_down_end_d48_p,
        /*717*/   icon_center_block_p, icon_alpha_sorting_d32_p, icon_time_lapse_d32_p, icon_select_entity_p, icon_select_p,
        /*722*/   icon_vector_p, icon_rigid_rigid_p, icon_rigid_pin_p, icon_pin_rigid_p, icon_pin_pin_p,
        /*727*/   icon_force_p, icon_moment_p, icon_moment_rev_p, icon_displacement_p, icon_rotation_p, icon_rotation_rev_p, icon_trapezium_y_p, icon_trapezium_x_p, icon_trapezium_n_p, icon_trapezium_h_p, icon_trapezium_v_p,
        /*738*/   icon_fixed_h_p, icon_fixed_vl_p, icon_fixed_vr_p, icon_pinned_h_p, icon_pinned_vl_p, icon_pinned_vr_p, icon_fixed_roller_h_p, icon_fixed_roller_vl_p, icon_fixed_roller_vr_p, icon_pinned_roller_h_p, icon_pinned_roller_vl_p, icon_pinned_roller_vr_p,
        /*750*/   icon_node_p, icon_magnitude_p, icon_thermal_p, icon_gear_plus_p,
        /*754*/   icon_fixed_hu_p, icon_pinned_hu_p, icon_fixed_roller_hu_p, icon_pinned_roller_hu_p,
        /*758*/   icon_compression_mag_p, icon_shear_mag_p, icon_moments_mag_p, icon_displacements_mag_p, icon_reactions_xy_p, icon_reactions_m_p,
        /*764*/   icon_thermal_y_p, icon_thermal_t_p, icon_trapezium_y1_p, icon_trapezium_y2_p, icon_thermal_t1_p, icon_thermal_t2_p,
        /*770*/   icon_vectors_p, icon_tension_mag_p, icon_US_Flag_p, icon_EU_Flag_p, icon_UK_Flag_p,
        /*775*/   icon_IH_sections_p, icon_U_sections_p, icon_T_sections_p, icon_L_sections_p, icon_2L_sections_p, icon_RT_sections_p, icon_O_sections_p, icon_EO_sections_p, icon_Z_sections_p, icon_VJ_sections_p,
        /*785*/   icon_timber_p, icon_node_size_p, icon_dead_load_p, icon_live_load_p, icon_roof_load_p, icon_wind_load_p, icon_snow_load_p, icon_earthquake_load_p, icon_rain_load_p, icon_soil_load_p, icon_fluid_load_p, icon_thermal_load_p,
        /*797*/   icon_unspecified_load_p, icon_unknown_load_p, icon_number_load_p,
        /*800*/   icon_stress_mag_p, icon_stress_plus_mag_p, icon_stress_minus_mag_p, icon_shear_stress_mag_p, icon_no_d_12_p,
        /*805*/   icon_eurocode_d48_p, icon_asce_d48_p, icon_icc_d48_p, icon_combination_d48_p, icon_erase_layer_db_64_p, icon_mark_layer_db_64_p, icon_mark_d_12_p,
        /*812*/   icon_AlfaCAD48_p, icon_Pdelta_d48_p, icon_dynamics_p, icon_vibrations_d48_p, icon_inertia_d48_p, icon_dynamics_run_p, icon_fixed_rotation_p,
        /*819*/   icon_mouse1b2b_p, icon_menustyle_p, icon_barstyle_p, icon_cursorstyle_p, icon_perc_mag_p, icon_cross_section_forces_p,
        /*825*/   icon_ULS_p, icon_SLS_p, icon_QPSLS_p, icon_resilience_p, icon_CA_Flag_p, icon_AU_Flag_p,
    };
   
	if (number>1999)
	   	return icons[0];  //just for a case, when no literal L for utf8
return icons[number];
}

static BITMAP *get_icons_aux_p(int number)
{
	int number_aux[] = {364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,359,356,388,389,357,390,392,393};

	return  (BITMAP *)get_icons_p(number_aux[number]);
}

void draw_font_name(char *name, unsigned char fontno, int x, int y)
{
	TEXT T=Tdef;
	int dy;
	
	T.warstwa = Current_Layer;
	T.czcionka = fontno;
	T.atrybut = 0;
	T.italics = 0;
	T.ukryty = 0;
	T.multiline = 0;
	T.underline = 0;
	T.width_factor = 1.0;
	T.widoczny = 1;
	T.kat = 0.0;
	T.justowanie = j_do_lewej;
	T.typ = n_typ_normal;

	T.kolor = 0;
	T.bold = 0;
	T.obiektt2 = O2NoBlockS;
	T.width = 0;
	T.height = 0;

    //changing world
	backup_world();

	if (strcmp(name, "symbolmisc1") == 0)
	{
		strcpy(T.text, "abcdefgh");
		T.wysokosc = 45;
		dy = 4;
	}
	else if (strcmp(name, "symbolmisc2") == 0)
	{
		strcpy(T.text, "ABCDEFGHIJK");
		T.wysokosc = 40;
		dy = -4;
	}
	else if (strcmp(name, "symap") == 0)
	{
		strcpy(T.text, "ABCDEFGHIJK");
		T.wysokosc = 20;
		dy = 0;
	}
	else if (strcmp(name, "symath") == 0)
	{
		strcpy(T.text, "abcdefghijk");
		T.wysokosc = 18;
		dy = 0;
	}
	else
	{
		strcpy(T.text, name);
		T.wysokosc = 20;
		dy = 0;
	}
	T.dl = (int)strlen(T.text);
	T.n = T18 + T.dl;

	T.x = x;
	T.y = y+dy;
	
	outtextxy_w_menu(&T);

	world_back();

}

void pdraww(PTMENU  *menu)
{
    int xt,yt;
    float yd, xr;
    float xtt, ytt;
    int i,size,x1,y1,x2,y2,x3,ink,border,paper;
    int x01,y01,x02,y02,w,h;
    char sz_temp [TEMPN] ;
    char *ptrsz_temp;
    char *ptrsz_tmn;
    unsigned short ptrsz_tcod;
    int ptrsz_iconno;
    int i_len ;
    char *ptr, *ptr1;
    int kolor_m, kolor_rgb;
    int offset_st;
    BITMAP *bmp, *buffer;
    BITMAP *bmp_aux;
    int typ_l, ret, ret1;
    int b;
    int x, y, ys, b2;
    int typ_f;
    int HEIGHT1;
    TEXT fontn= Tdef;
    COLOR_ kolor;
    int color_bak;
    BITMAP *menu_screen;
    int kolor1;

    if (menu == &mBlok_Imp)
        menu->flags |= (ICONS | TADD);

    paper=kolory.paperm;
    ink=kolory.inkm;
    border=kolory.borderm;
    size=menu->maxw?menu->maxw:menu->max;
    if (menu->flags&ICONS)
    {

        menu->xdl = (int)(64 * SKALA);

        xr = ((menu->flags&NVERT) ? size * 2 : 2);
        yd=((menu->flags&NVERT)?1:size);
        x01=(menu->xpcz-1)*WIDTH /*8*/ * SKALA;
        y01=(menu->ypcz-1)*32 + YP;

        x02 = x01 + xr * 32 * SKALA + 2 * (GR + 1);
        y02=y01+yd*32  +2*GR;
        b2 = 32 * SKALA/ 3;
    }
    else
    {
        xr=((menu->flags&NVERT)?size*menu->xdl:menu->xdl);
        yd=((menu->flags&NVERT)?1:size);
        x01=(menu->xpcz-1)*WIDTH /*8*/ * SKALA;
        y01=(menu->ypcz-1)*HEIGHT * SKALA + YP;
        x02=x01+xr*WIDTH /*8*/ * SKALA  +2*(GR+1);
        y02=y01+yd*HEIGHT * SKALA  +2*GR;
        b2 = HEIGHT * SKALA / 3;
    }

    getviewsettings (&viewinfo[menu_level-1]);
    setviewsettings (&viewinfo[menu_level], x01 + GR + viewinfo[menu_level-1].left, y01 + GR + viewinfo[menu_level-1].top, x02 - GR + viewinfo[menu_level-1].left, y02 - GR + viewinfo[menu_level-1].top) ;

    w=x2=x02-x01;
    h=y2=y02-y01;
    x1=0;
    y1=0;
    menu_screen= create_bitmap_ex(32, w, h);
    if (menu_screen==NULL) return;
    Set_Screenplay(menu_screen);

    setfillstyle_(SOLID_FILL,paper);
    bar(x1,y1,x2,y2);

    ramka_m(x1,y1,x2,y2);
    setcolor(ink);

    set_clip_rect(menu_screen, GR, GR, x2-GR, y2-GR);


    xtt = x1 + GR + 2.0 ;
    ytt = y1 + GR - 2.0 * SKALA + MDY ;

    if (menu->max < size) size=menu->max;

    if (menu->flags & CENTER_T)
    {
        settextjustify (CENTER_TEXT, TOP_TEXT) ;
    }
    for(i=0;i<size;i++)
    {
        xt=(int) (xtt+0.5);
        yt=(int)( ytt+0.5);
        if (menu->flags & CENTER_T)
        {
            moveto ((x2 - x1) / 2, (yt + SKALA * WIDTH /*8*/ / 2)-YP0) ;
        }
        else
        {
            if (menu->flags & ICONS) moveto (xt, yt-YP1) ;
            else moveto (xt, yt-YP0) ;
        }
        ptrsz_temp = (char *)(*(menu->pola))[menu->foff+i].txt ;
        ptrsz_tmn = (char *)(*(menu->pola))[menu->foff+i].menu ;
        ptrsz_tcod = (*(menu->pola))[menu->foff+i].wcod ;
        ptrsz_iconno = (*(menu->pola))[menu->foff+i].iconno ;

        ptr=strstr(ptrsz_temp,u8"֍");
        if (ptr!=NULL)
        {
            ptrsz_temp+=2;
            if ((menu->foff + i) >= 16)
            {
                if (LiniaG.kolor > 16) kolor_m=LiniaG.kolor;
                else kolor_m = 254;
            }
            else
            {
                kolor_m=GetColorAC1(menu->foff+i+1);
            }

            setfillstyle_(SOLID_FILL,kolor_m);

            if (menu->flags&ICONS)
            {
                bar(xt,yt - 2,w-(GR+2),yt + 32 - 2);
            }
            else
            {
                bar(xt,yt - 2,w-(GR+2),yt + HEIGHT * SKALA - 2);
            }

            if (kolor_m > 16)
            {
                kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
                if (kolor_rgb < 96) setcolor(15); else setcolor(0);
            }
            else if ((kolor_m == 0) || (kolor_m == 8) || (kolor_m == 16)) setcolor(15);
            else setcolor(0);

        }
        else
        {
            ptr=strstr(ptrsz_temp,u8"֎");
            ptr1=strstr(ptrsz_temp,u8"֊");
            if (ptr!=NULL)
            {
                ptrsz_temp+=2;
                if (ptrsz_iconno==158) kolor_m=GetColorAC1(LiniaG.kolor);
                else if (ptrsz_iconno == 177) kolor_m = GetColorAC1(zmwym.Lkolor);
                else if (ptrsz_iconno == 178) kolor_m = GetColorAC1(zmwym.Tkolor);
                else if (ptrsz_iconno == 495) kolor_m = GetColorAC1(sektory_arkusza_ext.frame_color);
                else if (ptrsz_iconno == 496) kolor_m = GetColorAC1(sektory_arkusza_ext.sector_color);

                else if (ptrsz_iconno == 723) kolor_m = GetColorAC1(static_colors.node_element_color);
                else if (ptrsz_iconno == 771) kolor_m = GetColorAC1(static_colors.tension_color);
                else if (ptrsz_iconno == 758) kolor_m = GetColorAC1(static_colors.compression_color);
                else if (ptrsz_iconno == 759) kolor_m = GetColorAC1(static_colors.shear_color);
                else if (ptrsz_iconno == 760) kolor_m = GetColorAC1(static_colors.moment_color);
                else if (ptrsz_iconno == 761) kolor_m = GetColorAC1(static_colors.deformation_color);
                else if (ptrsz_iconno == 763) kolor_m = GetColorAC1(static_colors.reaction_color);
                else if (ptrsz_iconno == 801) kolor_m = GetColorAC1(static_stress_colors.axial_stress_plus_color);
                else if (ptrsz_iconno == 802) kolor_m = GetColorAC1(static_stress_colors.axial_stress_minus_color);
                else if (ptrsz_iconno == 803) kolor_m = GetColorAC1(static_stress_colors.shear_stress_color);
                else if (ptrsz_iconno == 814) kolor_m = GetColorAC1(static_colors.dynamic_color);
                else if (ptrsz_iconno > 1000) kolor_m = GetColorAC1(ptrsz_iconno-1000);

                else kolor_m = GetColorAC1(LiniaG.kolor); //temporary

                setfillstyle_(SOLID_FILL,kolor_m);

                if (menu->flags&ICONS)
                {
                    bar(xt,yt - 2,w-(GR+2),yt + 32 - 2);
                }
                else
                {
                    bar(xt,yt - 2,w-(GR+2),yt + HEIGHT * SKALA - 2);
                }
                if (kolor_m > 16)
                {
                    kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
                    if (kolor_rgb < 96) setcolor(15); else setcolor(0);
                }
                else if (kolor_m == 0) setcolor(15);
                else setcolor(0);

            }
            else if (ptr1!=NULL)
            {
                ptrsz_temp+=2;
                ret1=sscanf(ptrsz_temp, "%03d", &kolor1);
                kolor_m = GetColorAC1(kolor1);
                ptrsz_temp+=3;
            }
            else
            {
                setcolor(ink);
                ptr=strstr(ptrsz_temp,u8"֏");
                if(ptr!=NULL)
                {
                    typ_l=menu->foff + i;
                    linestyle(typ_l+64);
                    ret=lineCuncut ((w)/2,yt+(HEIGHT * SKALA / 2) - 1, w-(GR+2), yt+(HEIGHT * SKALA / 2) - 1);

                    ptrsz_temp+=2;
                }
                else
                {
                    ptr=strstr(ptrsz_temp,u8"҂");
                    if(ptr!=NULL)
                    {

                        typ_l=menu->foff+i;
                        linestyle(typ_l*32);

                        ret=lineCuncut ((w)/2,yt+(HEIGHT * SKALA / 2) - 1, w-(GR+2), yt+(HEIGHT * SKALA / 2) - 1);

                        ptrsz_temp+=2;
                    }
                    else
                    {
                        if ((menu == &mCzcionka) || (menu == &mCzcionkaZ) || (menu == &mCzcionkaW))
                        {
                            typ_f = menu->foff + i;
                        }

                    }
                }
            }
        }

        //instead of texts we can put images
        //images can occupy space 32x32 pxl, if total width would be some 1
        //total with should be 64pxl, to show extra options or so

        if (menu->flags&ICONS)
        {
            if (ptrsz_iconno>=FIRST_CLIENT_BITMAP_NO) bmp = (BITMAP *)client_bitmap_load[ptrsz_iconno-FIRST_CLIENT_BITMAP_NO];
            else bmp = (BITMAP *)get_icons_p(ptrsz_iconno);
            if (bmp != NULL)
            {
                if (bmp->w == 32) blit(bmp, screenplay, 0, 0, xt - 1, yt - 2, 32, 32);
                else
                {
                    buffer = create_bitmap_ex(32, 32, 32);
                    if (buffer != NULL)
                    {
                        clear_to_color(buffer, bitmap_mask_color(buffer));
                        rotate_scaled_sprite(buffer, bmp, 0, 0, ftofix(0.0), ftofix(32.0 / (double)bmp->w));
                        set_alpha_blender();
                        draw_trans_sprite(screenplay, buffer, xt - 1, yt - 2);
                        destroy_bitmap(buffer);
                        set_trans_blender(0, 0, 0, TRANSLUCENCY);
                        set_mode_solid();
                    }
                }
            }

            if (ptrsz_tcod < 32)
            {
                bmp_aux = (BITMAP *)get_icons_aux_p(ptrsz_tcod);
                if (bmp_aux != NULL)
                {
                    if (bmp_aux->w == 32)  blit(bmp_aux, screenplay, 0, 0, xt - 1 + 32, yt - 2, 32, 32);
                    else
                    {
                        buffer = create_bitmap_ex(32, 32, 32);
                        if (buffer != NULL)
                        {
                            clear_to_color(buffer, bitmap_mask_color(buffer));
                            rotate_scaled_sprite(buffer, bmp_aux, 0, 0, ftofix(0.0), ftofix(32.0 / (double)bmp_aux->w));
                            set_alpha_blender();
                            draw_trans_sprite(screenplay, buffer, xt - 1 + 32, yt - 2);
                            destroy_bitmap(buffer);
                            set_trans_blender(0, 0, 0, TRANSLUCENCY);
                            set_mode_solid();
                        }
                    }
                }
            }
            else if (ptrsz_tcod > 1920)  //255
            {
                bmp_aux = (BITMAP *)get_icons_p(ptrsz_tcod-1920);
                if (bmp_aux != NULL)
                {
                    if (bmp_aux->w==32) blit(bmp_aux, screenplay, 0, 0, xt - 1 + 32, yt - 2, 32, 32);
                    else
                    {
                        buffer = create_bitmap_ex(32, 32, 32);
                        if (buffer != NULL)
                        {
                            clear_to_color(buffer, bitmap_mask_color(buffer));
                            rotate_scaled_sprite(buffer, bmp_aux, 0, 0, ftofix(0.0), ftofix(32.0 / (double)bmp_aux->w));
                            set_alpha_blender();
                            draw_trans_sprite(screenplay, buffer, xt - 1 + 32, yt - 2);
                            destroy_bitmap(buffer);
                            set_trans_blender(0, 0, 0, TRANSLUCENCY);
                            set_mode_solid();
                        }
                    }
                }
            }
            else
            {
                int bytes_n;
                uint8_t utf8c[4];

                bytes_n = ucs2_to_utf8(ptrsz_tcod, (uint8_t*)utf8c);
                if ((ptrsz_tcod < 1920) && (bytes_n < 3))
                {
                    strncpy(sz_temp, utf8c, 2);
                    sz_temp[2] = '\0';
                }
                else
                {
                    strcpy(sz_temp, " ");
                }

                x = getx();
                y = gety();

                b = (int)(-HEIGHT*0.357 + 15);
                moveto(x + 36, y + b);
                outtext_r_(menu_screen, sz_temp);
            }
        }
        else
        {
            if ((menu == &mCzcionka) || (menu == &mCzcionkaZ) || (menu == &mCzcionkaW))
            {
                x = getx();
                y = gety();
                HEIGHT1 = 28;
                b = (int)(HEIGHT - (HEIGHT - HEIGHT1) / 2);
                moveto(x, y + b);

                i_len = menu_len(ptrsz_temp, xr);
                strncpy(sz_temp, ptrsz_temp, i_len);
                sz_temp[i_len] = '\0';

                if (typ_f < i__font_nomax)
                {
                    if ((PTRS__Text_Style[typ_f]->type < 2) || (PTRS__Text_Style[typ_f]->type==3))
                        draw_font_name(ptrsz_temp, typ_f, x, y + 21); //22
                    else
                    {
                        kolor.red = 0;
                        kolor.gre = 0;
                        kolor.blu = 100;
                        strcpy(fontn.text, ptrsz_temp);
                        fontn.dl = (int)strlen(fontn.text);
                        fontn.n = T18 + fontn.dl;
                        fontn.czcionka = typ_f;
                        fontn.kat = 0.0;
                        fontn.bold = 0;
                        fontn.width_factor = 1.0;
                        fontn.italics = 0;
                        fontn.justowanie = j_do_lewej;
                        fontn.multiline = 0;
                        fontn.ukryty = 0;
                        fontn.wysokosc = 4;
                        fontn.kolor = 0;
                        fontn.width = 0;
                        fontn.height = 0;

                        draw_font_name_ttf(&fontn, (char *)&fontn.text, screenplay, x, y + 21, x2, fontn.kat, 20.0, kolor, COPY_PUT, 0, 0);  //22
                    }
                }
                else
                {
                    color_bak = getcolor();
                    setcolor(184);
                    TTF_text_UI_W_H_(screenplay, ptrsz_temp, x, y - 2, NULL, NULL, WIDTH_, HEIGHT_);
                    setcolor(color_bak);
                }

                yt=y;
            }
            else
            {

                i_len = menu_len(ptrsz_temp, xr);
                strncpy(sz_temp, ptrsz_temp, i_len);
                sz_temp[i_len] = '\0';
                outtext_r_(menu_screen, sz_temp);
            }
        }

        offset_st=0;
        if ((ptrsz_tmn!=NULL) && (menu->flags & TADD))   //istnieje podmenu
        {

            setwritemode(COPY_PUT);
            setcolor (kolory.inkk);  //inkk
            setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
            x3=x2-x1-10;
            ys = yt + b2;
            LINE (x3-3,   ys,   x3-3,  ys+6);
            LINE (x3-2,   ys+1, x3-2,  ys+5);
            LINE (x3-1,   ys+2, x3-1,  ys+4);
            LINE (x3,     ys+3, x3,    ys+3);
            offset_st=8;
        }

        setcolor (kolory.inkm);

        if((menu->flags&TADD) && !(menu->flags&ICONS))
        {
            char * st = (*(menu->pola))[menu->foff+i].txt;
            int a;
            ptr = strstr(ptrsz_temp, u8"֎");
            ptr1=strstr(ptrsz_temp,u8"֊");
            if (ptr == NULL)
            {
                ptrsz_temp+=2;
                st += strlen(st) + 1;
                a = (menu->xdl) * WIDTH * SKALA;
                a -= (text_width(st) + offset_st);
                if (menu->flags&NVERT) a -= WIDTH /*8*/ * SKALA;
                moveto(xt + a, yt - YP0);
                outtext_r_(menu_screen, st);
            }
            else if (ptr1 == NULL)
            {
                ptrsz_temp+=5;
                st += strlen(st) + 1;
                a = (menu->xdl) * WIDTH * SKALA;
                a -= (text_width(st) + offset_st);
                if (menu->flags&NVERT) a -= WIDTH /*8*/ * SKALA;
                moveto(xt + a, yt - YP0);
                outtext_r_(menu_screen, st);
            }
        }
        if (menu->flags&NVERT)
        {
            if (menu->flags&ICONS)
                xtt += 64 * SKALA;
            else
                xtt += menu->xdl*WIDTH /*8*/ * SKALA;
        }
        else
        {
            if (menu->flags&ICONS)
            {
                ytt+=32;
            }
            else
            {
                ytt+=HEIGHT * SKALA;
            }
        }
    }
    if (menu->flags & CENTER_T)
    {
        settextjustify (LEFT_TEXT, TOP_TEXT) ;
    }

    blit(menu_screen, screen, 0,0,x01,y01,w,h);

    Set_Screenplay(screen);

    destroy_bitmap(menu_screen);

}


void draww(TMENU  *menu)
{
 int xt,yt;
 float yd, xr;
 float xtt, ytt;
 int i,size,x1,y1,x2,y2,x3,ink,border,paper;
 int x01,y01,x02,y02,w,h;
 char sz_temp [TEMPN] ;
 char *ptrsz_temp;
 char *ptrsz_tmn;
 unsigned short ptrsz_tcod;
 int ptrsz_iconno;
 int i_len ;
 char *ptr, *ptr1;
 int kolor_m, kolor_rgb;
 int offset_st;
 BITMAP *bmp, *buffer;
 BITMAP *bmp_aux;
 int typ_l, ret, ret1;
 int b;
 int x, y, ys, b2;
 int typ_f;
 int HEIGHT1;
 TEXT fontn= Tdef;
 COLOR_ kolor;
 int color_bak;
 BITMAP *menu_screen;
 int kolor1;

 if (menu->flags & FIXED)
 {
     pdraww((PTMENU*)menu);
     return;
 }

 if (menu == &mBlok_Imp) 
	 menu->flags |= (ICONS | TADD);

   paper=kolory.paperm;
   ink=kolory.inkm;
   border=kolory.borderm;
   size=menu->maxw?menu->maxw:menu->max;
   if (menu->flags&ICONS)
   {  

	   menu->xdl = (int)(64 * SKALA);

	   xr = ((menu->flags&NVERT) ? size * 2 : 2);
	   yd=((menu->flags&NVERT)?1:size);
	   x01=(menu->xpcz-1)*WIDTH /*8*/ * SKALA;
	   y01=(menu->ypcz-1)*32 + YP;
	   
	   x02 = x01 + xr * 32 * SKALA + 2 * (GR + 1);
	   y02=y01+yd*32  +2*GR;
	   b2 = 32 * SKALA/ 3;
   }
   else
   {
	   xr=((menu->flags&NVERT)?size*menu->xdl:menu->xdl);
	   yd=((menu->flags&NVERT)?1:size);
	   x01=(menu->xpcz-1)*WIDTH /*8*/ * SKALA;
	   y01=(menu->ypcz-1)*HEIGHT * SKALA + YP;
	   x02=x01+xr*WIDTH /*8*/ * SKALA  +2*(GR+1);
	   y02=y01+yd*HEIGHT * SKALA  +2*GR;
	   b2 = HEIGHT * SKALA / 3;
   }

    getviewsettings (&viewinfo[menu_level-1]);
    setviewsettings (&viewinfo[menu_level], x01 + GR + viewinfo[menu_level-1].left, y01 + GR + viewinfo[menu_level-1].top, x02 - GR + viewinfo[menu_level-1].left, y02 - GR + viewinfo[menu_level-1].top) ;

    w=x2=x02-x01;
    h=y2=y02-y01;
    x1=0;
    y1=0;
    menu_screen= create_bitmap_ex(32, w, h);
    if (menu_screen==NULL) return;

    Set_Screenplay(menu_screen);

   setfillstyle_(SOLID_FILL,paper);
   bar(x1,y1,x2,y2);

   ramka_m(x1,y1,x2,y2);
   setcolor(ink);

   set_clip_rect(menu_screen, GR, GR, x2-GR, y2-GR);

    xtt = x1 + GR + 2.0 ;
    ytt = y1 + GR - 2.0 * SKALA + MDY ;

   if (menu->max < size) size=menu->max;

   if (menu->flags & CENTER_T)
   {
      settextjustify (CENTER_TEXT, TOP_TEXT) ;
   }
   for(i=0;i<size;i++)
   {
     xt=(int) (xtt+0.5);
     yt=(int)( ytt+0.5);
     if (menu->flags & CENTER_T)
     {
    	moveto ((x2 - x1) / 2, (yt + SKALA * WIDTH /*8*/ / 2)-YP0) ;
     }
     else
     {
         if (menu->flags & ICONS) moveto (xt, yt-YP1) ;
    	else moveto (xt, yt-YP0) ;
     }
     ptrsz_temp = (char *)(*(menu->pola))[menu->foff+i].txt ;

#ifdef UKRAINIAN
     if (strlen(ptrsz_temp)>POLE_TXT_MAX)
     {
       printf("ALARM strlen>POLE_TXT_MAX:  %d\n", strlen(ptrsz_temp));
     }
#endif

     ptrsz_tmn = (char *)(*(menu->pola))[menu->foff+i].menu ;
	 ptrsz_tcod = (*(menu->pola))[menu->foff+i].wcod ;
	 ptrsz_iconno = (*(menu->pola))[menu->foff+i].iconno ;
     
     ptr=strstr(ptrsz_temp,u8"֍");
     if (ptr!=NULL)
      {
		 ptrsz_temp+=2;
        if ((menu->foff + i) >= 16)
         {
          if (LiniaG.kolor > 16) kolor_m=LiniaG.kolor;
            else kolor_m = 254;
         }
          else
           {
            kolor_m=GetColorAC1(menu->foff+i+1);
           } 
     
	   setfillstyle_(SOLID_FILL,kolor_m);

		  if (menu->flags&ICONS)
		  {
              bar(xt,yt - 2,w-(GR+2),yt + 32 - 2);
		  }
		  else
		  {
              bar(xt,yt - 2,w-(GR+2),yt + HEIGHT * SKALA - 2);
		  }
       if (kolor_m > 16)
       {
         kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
         if (kolor_rgb < 96) setcolor(15); else setcolor(0);
       }
        else if ((kolor_m == 0) || (kolor_m == 8) || (kolor_m == 16)) setcolor(15);
           else setcolor(0);  
       
      }       
       else
         {
            ptr=strstr(ptrsz_temp,u8"֎");
            ptr1=strstr(ptrsz_temp,u8"֊");
            if (ptr!=NULL)
            {
			   ptrsz_temp+=2;
			  if (ptrsz_iconno==158) kolor_m=GetColorAC1(LiniaG.kolor);
			  else if (ptrsz_iconno == 177) kolor_m = GetColorAC1(zmwym.Lkolor);
			  else if (ptrsz_iconno == 178) kolor_m = GetColorAC1(zmwym.Tkolor);
			  else if (ptrsz_iconno == 495) kolor_m = GetColorAC1(sektory_arkusza_ext.frame_color);
			  else if (ptrsz_iconno == 496) kolor_m = GetColorAC1(sektory_arkusza_ext.sector_color);

              else if (ptrsz_iconno == 723) kolor_m = GetColorAC1(static_colors.node_element_color);
              else if (ptrsz_iconno == 771) kolor_m = GetColorAC1(static_colors.tension_color);
              else if (ptrsz_iconno == 758) kolor_m = GetColorAC1(static_colors.compression_color);
              else if (ptrsz_iconno == 759) kolor_m = GetColorAC1(static_colors.shear_color);
              else if (ptrsz_iconno == 760) kolor_m = GetColorAC1(static_colors.moment_color);
              else if (ptrsz_iconno == 761) kolor_m = GetColorAC1(static_colors.deformation_color);
              else if (ptrsz_iconno == 763) kolor_m = GetColorAC1(static_colors.reaction_color);
              else if (ptrsz_iconno == 801) kolor_m = GetColorAC1(static_stress_colors.axial_stress_plus_color);
              else if (ptrsz_iconno == 802) kolor_m = GetColorAC1(static_stress_colors.axial_stress_minus_color);
              else if (ptrsz_iconno == 803) kolor_m = GetColorAC1(static_stress_colors.shear_stress_color);
              else if (ptrsz_iconno == 814) kolor_m = GetColorAC1(static_colors.dynamic_color);
              else if (ptrsz_iconno > 1000) kolor_m = GetColorAC1(ptrsz_iconno-1000);

			  else kolor_m = GetColorAC1(LiniaG.kolor); //temporary
              
              setfillstyle_(SOLID_FILL,kolor_m);
             
			  if (menu->flags&ICONS)
			  {
                  bar(xt,yt - 2,w-(GR+2),yt + 32 - 2);
			  }
			  else
              {
                  bar(xt,yt - 2,w-(GR+2),yt + HEIGHT * SKALA - 2);
			  }
              if (kolor_m > 16)
               {
                kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
                if (kolor_rgb < 96) setcolor(15); else setcolor(0);
               }
                else if (kolor_m == 0) setcolor(15);
                 else setcolor(0); 
			  
            }
            else if (ptr1!=NULL)
            {
                ptrsz_temp+=2;
                ret1=sscanf(ptrsz_temp, "%03d", &kolor1);
                kolor_m = GetColorAC1(kolor1);
                ptrsz_temp+=3;

                setfillstyle_(SOLID_FILL,kolor_m);

                if (menu->flags&ICONS)
                {
                    bar(xt,yt - 2,w-(GR+2),yt + 32 - 2);
                }
                else
                {
                    bar(xt,yt - 2,w-(GR+2),yt + HEIGHT * SKALA - 2);
                }
                if (kolor_m > 16)
                {
                    kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
                    if (kolor_rgb < 96) setcolor(15); else setcolor(0);
                }
                else if (kolor_m == 0) setcolor(15);
                else setcolor(0);
            }
           else 
		   {
			  setcolor(ink); 
			  ptr=strstr(ptrsz_temp,u8"֏");
			  if(ptr!=NULL)
				{   
					typ_l=menu->foff + i;
					linestyle(typ_l+64);
					ret=lineCuncut ((w)/2,yt+(HEIGHT * SKALA / 2) - 1, w-(GR+2), yt+(HEIGHT * SKALA / 2) - 1);

					ptrsz_temp+=2;
				}
			   else
			   {
					ptr=strstr(ptrsz_temp,u8"҂");
					if(ptr!=NULL)
					{
                      
					   typ_l=menu->foff+i;
					   linestyle(typ_l*32);
					  
					   ret=lineCuncut ((w)/2,yt+(HEIGHT * SKALA / 2) - 1, w-(GR+2), yt+(HEIGHT * SKALA / 2) - 1);

					   ptrsz_temp+=2;
					}
					else
					{
						if ((menu == &mCzcionka) || (menu == &mCzcionkaZ) || (menu == &mCzcionkaW))
						{
							typ_f = menu->foff + i;
						}
						
					}
			   }
		   }
         }
          

	 //instead of texts we can put images
	 //images can occupy space 32x32 pxl, if total width would be some 1
	 //total with should be 64pxl, to show extra options or so
	 
     if (menu->flags&ICONS)
	 {
         if (ptrsz_iconno>=FIRST_CLIENT_BITMAP_NO) bmp = (BITMAP *)client_bitmap_load[ptrsz_iconno-FIRST_CLIENT_BITMAP_NO];
		 else bmp = (BITMAP *)get_icons_p(ptrsz_iconno);
		 if (bmp != NULL)
		 {
			 if (bmp->w == 32) blit(bmp, screenplay, 0, 0, xt - 1, yt - 2, 32, 32);
			 else
			 {
				 buffer = create_bitmap_ex(32, 32, 32);
				 if (buffer != NULL)
				 {
					 clear_to_color(buffer, bitmap_mask_color(buffer));			 
					 rotate_scaled_sprite(buffer, bmp, 0, 0, ftofix(0.0), ftofix(32.0 / (double)bmp->w));
					 set_alpha_blender();
					 draw_trans_sprite(screenplay, buffer, xt - 1, yt - 2);
					 destroy_bitmap(buffer);
					 set_trans_blender(0, 0, 0, TRANSLUCENCY);
					 set_mode_solid();
				 }
			 }
		 }

		 if (ptrsz_tcod < 32)
		 {
			 bmp_aux = (BITMAP *)get_icons_aux_p(ptrsz_tcod);
			 if (bmp_aux != NULL)
			 {		 
			  if (bmp_aux->w == 32)  blit(bmp_aux, screenplay, 0, 0, xt - 1 + 32, yt - 2, 32, 32);
			  else
			  {
				  buffer = create_bitmap_ex(32, 32, 32);
				  if (buffer != NULL)
				  {
					  clear_to_color(buffer, bitmap_mask_color(buffer));
					  rotate_scaled_sprite(buffer, bmp_aux, 0, 0, ftofix(0.0), ftofix(32.0 / (double)bmp_aux->w));
					  set_alpha_blender();
					  draw_trans_sprite(screenplay, buffer, xt - 1 + 32, yt - 2);
					  destroy_bitmap(buffer);
					  set_trans_blender(0, 0, 0, TRANSLUCENCY);
					  set_mode_solid();
				  }
			  }
			 }
		 }
		 else if (ptrsz_tcod > 1920)  //255
		 {
			 bmp_aux = (BITMAP *)get_icons_p(ptrsz_tcod-1920);
			 if (bmp_aux != NULL)
			 {
				 if (bmp_aux->w==32) blit(bmp_aux, screenplay, 0, 0, xt - 1 + 32, yt - 2, 32, 32);
				 else
				 {
					 buffer = create_bitmap_ex(32, 32, 32);
					 if (buffer != NULL)
					 {
						 clear_to_color(buffer, bitmap_mask_color(buffer));
						 rotate_scaled_sprite(buffer, bmp_aux, 0, 0, ftofix(0.0), ftofix(32.0 / (double)bmp_aux->w));
						 set_alpha_blender();
						 draw_trans_sprite(screenplay, buffer, xt - 1 + 32, yt - 2);
						 destroy_bitmap(buffer);
						 set_trans_blender(0, 0, 0, TRANSLUCENCY);
						 set_mode_solid();
					 }
				 }
			 }
		 }
		 else
		 {
			 int bytes_n;
			 uint8_t utf8c[4];

			 bytes_n = ucs2_to_utf8(ptrsz_tcod, (uint8_t*)utf8c);
			 if ((ptrsz_tcod < 1920) && (bytes_n < 3))
			 {
				 strncpy(sz_temp, utf8c, 2);
				 sz_temp[2] = '\0';
			 }
			 else
			 {
				 strcpy(sz_temp, " ");
			 }

			 x = getx();
			 y = gety();

			 b = (int)(-HEIGHT*0.357 + 15);
			 moveto(x + 36, y + b);
			 outtext_r_(menu_screen, sz_temp);
		 }
	 }
	 else 
	 {
		if ((menu == &mCzcionka) || (menu == &mCzcionkaZ) || (menu == &mCzcionkaW))
		{
			x = getx();
			y = gety();
			HEIGHT1 = 28;
			b = (int)(HEIGHT - (HEIGHT - HEIGHT1) / 2);
			moveto(x, y + b);

			i_len = menu_len(ptrsz_temp, xr);
			strncpy(sz_temp, ptrsz_temp, i_len);
			sz_temp[i_len] = '\0';
			
			if (typ_f < i__font_nomax)
			{
				if ((PTRS__Text_Style[typ_f]->type < 2) || (PTRS__Text_Style[typ_f]->type==3))
					draw_font_name(ptrsz_temp, typ_f, x, y + 21);
				else
				{
					kolor.red = 0;
					kolor.gre = 0;
					kolor.blu = 100;
					strcpy(fontn.text, ptrsz_temp);
					fontn.dl = (int)strlen(fontn.text);
					fontn.n = T18 + fontn.dl;
					fontn.czcionka = typ_f;
					fontn.kat = 0.0;
					fontn.bold = 0;
					fontn.width_factor = 1.0;
					fontn.italics = 0;
					fontn.justowanie = j_do_lewej;
					fontn.multiline = 0;
					fontn.ukryty = 0;
					fontn.wysokosc = 4;
					fontn.kolor = 0;
					fontn.width = 0;
					fontn.height = 0;

					draw_font_name_ttf(&fontn, (char *)&fontn.text, screenplay, x, y + 21, x2, fontn.kat, 20.0, kolor, COPY_PUT, 0, 0);  //22
				}
			}
			else
			{
				color_bak = getcolor();
				setcolor(184);
				TTF_text_UI_W_H_(screenplay, ptrsz_temp, x, y - 2, NULL, NULL, WIDTH_, HEIGHT_);
				setcolor(color_bak);
			}

            yt=y;
		}
		else
		{
			
			i_len = menu_len(ptrsz_temp, xr);
			strncpy(sz_temp, ptrsz_temp, i_len);
			sz_temp[i_len] = '\0';
			outtext_r_(menu_screen, sz_temp);
		}
	 }

     offset_st=0;
     if ((ptrsz_tmn!=NULL) && (menu->flags & TADD))   //istnieje podmenu
       {
        
         setwritemode(COPY_PUT);
         setcolor (kolory.inkk);  //inkk
         setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
         x3=x2-x1-10;
		 ys = yt + b2;
         LINE (x3-3,   ys,   x3-3,  ys+6);
         LINE (x3-2,   ys+1, x3-2,  ys+5);
         LINE (x3-1,   ys+2, x3-1,  ys+4);
         LINE (x3,     ys+3, x3,    ys+3);
         offset_st=8;
       }

     setcolor (kolory.inkm);

     if((menu->flags&TADD) && !(menu->flags&ICONS))
	  {
	    char * st = (*(menu->pola))[menu->foff+i].txt;
	    int a;
		ptr = strstr(ptrsz_temp, u8"֎");
        ptr1=strstr(ptrsz_temp,u8"֊");
		if (ptr == NULL)
		{
			ptrsz_temp+=2;
			st += strlen(st) + 1;
			a = (menu->xdl) * WIDTH * SKALA;
			a -= (text_width(st) + offset_st);
			if (menu->flags&NVERT) a -= WIDTH /*8*/ * SKALA;
			moveto(xt + a, yt - YP0);
			outtext_r_(menu_screen, st);
		}
        else if (ptr == NULL)
          {
              ptrsz_temp+=5;
              st += strlen(st) + 1;
              a = (menu->xdl) * WIDTH * SKALA;
              a -= (text_width(st) + offset_st);
              if (menu->flags&NVERT) a -= WIDTH /*8*/ * SKALA;
              moveto(xt + a, yt - YP0);
              outtext_r_(menu_screen, st);
          }
	  }
	 if (menu->flags&NVERT)
	 {
		 if (menu->flags&ICONS)
			 xtt += 64 * SKALA;
		 else
			 xtt += menu->xdl*WIDTH /*8*/ * SKALA;
	 }
     else 
	 {
		 if (menu->flags&ICONS)
		 {
		   ytt+=32;
		 }
		 else
         {
		   ytt+=HEIGHT * SKALA;
		 }
	 }
   }
   if (menu->flags & CENTER_T)
   {
     settextjustify (LEFT_TEXT, TOP_TEXT) ;
   }

    if (BAR_POINTER) show_mouse(NULL);
    blit(menu_screen, screen, 0,0,x01,y01,w,h);


    Set_Screenplay(screen);
    if (BAR_POINTER) show_mouse(screen);

   destroy_bitmap(menu_screen);
}

void drawp(TMENU *menu)
{  int x1,y1,x2,y2,a,b,c,ys,b2, ink,paper;
    int x01,y01,x02,y02,w,h;
   char ptrsz_tcod;
   char *ptrsz_temp=NULL;
   int ptrsz_iconno;
   char sz_temp[8];
   char *ptr, *ptr1;
   int kolor_m, kolor_rgb;
   char *ptrsz_tmn=NULL;
   int d = 0;
   int ret1;
   BITMAP *menu_screen;
   int kolor1;

   paper=kolory.paperm;
   ink=kolory.inkm;
   
   if (menu->flags&ICONS)
   {

	   return;  //it's not necessary in ICONS menu

	   baronoff(menu);
	   frame_on(menu);

	   x1=(menu->xpcz-1)*WIDTH /*8*/ * SKALA +GR+1;
	   y1=(menu->ypcz-1)*32 +GR+1+YP; y1+=menu->poz*32;
       y2=y1+32;
	   c= 32;
	   a = 64;
	   b=32;
	   b2 = 32 * SKALA / 3;

       w=x2=x02-x01;
       h=y2=y02-y01;
       x1=0;
       y1=0;
       menu_screen= create_bitmap_ex(32, w, h);
       if (menu_screen==NULL) return;
       Set_Screenplay(menu_screen);

       setfillstyle_(SOLID_FILL, paper);
	   bar(x1+c+d, y1+d, x1 + a-d, y1 + b-d);
	   setwritemode(COPY_PUT);
	   setcolor(ink);
	  
	   //writing shortcut again
	   ptrsz_temp = (char *)(*(menu->pola))[menu->foff + menu->poz].txt;
	   ptrsz_tcod = (*(menu->pola))[menu->foff + menu->poz].wcod;
	   ptrsz_iconno = (*(menu->pola))[menu->foff + menu->poz].iconno;
	   ptrsz_tmn = (char *)(*(menu->pola))[menu->foff + menu->poz].menu;

	   sz_temp[0] = ptrsz_tcod;
	   sz_temp[1] = '\0';

	   ptr = strstr(ptrsz_temp, u8"֎");
       ptr1=strstr(ptrsz_temp,u8"֊");
	   if (ptr != NULL)
	   {
		   ptrsz_temp+=2;
		   if (ptrsz_iconno == 158) kolor_m = GetColorAC1(LiniaG.kolor);
		   else if (ptrsz_iconno == 177) kolor_m = GetColorAC1(zmwym.Lkolor);
		   else if (ptrsz_iconno == 178) kolor_m = GetColorAC1(zmwym.Tkolor);
           else if (ptrsz_iconno == 495) kolor_m = GetColorAC1(sektory_arkusza_ext.frame_color);
           else if (ptrsz_iconno == 496) kolor_m = GetColorAC1(sektory_arkusza_ext.sector_color);

           else if (ptrsz_iconno == 723) kolor_m = GetColorAC1(static_colors.node_element_color);
           else if (ptrsz_iconno == 771) kolor_m = GetColorAC1(static_colors.tension_color);
           else if (ptrsz_iconno == 758) kolor_m = GetColorAC1(static_colors.compression_color);
           else if (ptrsz_iconno == 759) kolor_m = GetColorAC1(static_colors.shear_color);
           else if (ptrsz_iconno == 760) kolor_m = GetColorAC1(static_colors.moment_color);
           else if (ptrsz_iconno == 761) kolor_m = GetColorAC1(static_colors.deformation_color);
           else if (ptrsz_iconno == 763) kolor_m = GetColorAC1(static_colors.reaction_color);
           else if (ptrsz_iconno == 801) kolor_m = GetColorAC1(static_stress_colors.axial_stress_plus_color);
           else if (ptrsz_iconno == 802) kolor_m = GetColorAC1(static_stress_colors.axial_stress_minus_color);
           else if (ptrsz_iconno == 803) kolor_m = GetColorAC1(static_stress_colors.shear_stress_color);
           else if (ptrsz_iconno == 814) kolor_m = GetColorAC1(static_colors.dynamic_color);
           else if (ptrsz_iconno > 1000) kolor_m = GetColorAC1(ptrsz_iconno-1000);


           else kolor_m = GetColorAC1(LiniaG.kolor); //temporary


		   setfillstyle_(SOLID_FILL, kolor_m);
		  
		   bar(x1+32+d, y1+d, x1 + a-d, y1 + b-d);

		   if (kolor_m > 16)
		   {
			   kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
			   if (kolor_rgb < 96) setcolor(15); else setcolor(0);
		   }
		   else if (kolor_m == 0) setcolor(15);
		   else setcolor(0);
		 
	   }
       else if (ptr1!=NULL)
       {
           ptrsz_temp+=2;
           ret1=sscanf(ptrsz_temp, "%03d", &kolor1);
           kolor_m = GetColorAC1(kolor1);
           ptrsz_temp+=3;

           setfillstyle_(SOLID_FILL, kolor_m);

           bar(x1+32+d, y1+d, x1 + a-d, y1 + b-d);

           if (kolor_m > 16)
           {
               kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
               if (kolor_rgb < 96) setcolor(15); else setcolor(0);
           }
           else if (kolor_m == 0) setcolor(15);
           else setcolor(0);
       }

	   moveto(x1 + 37, y1 + 8);
	   outtext_r_(menu_screen, sz_temp);

   }
   else
   {
	   baronoff(menu);
	   frame_on(menu);

	   x1 = (menu->xpcz - 1)*WIDTH /*8*/ * SKALA + GR + 1;
	   y1 = (menu->ypcz - 1)*HEIGHT * SKALA + GR + 1 + YP; y1 += menu->poz*HEIGHT * SKALA;
       y2= y1 + HEIGHT * SKALA;
	   a = (menu->xdl) * WIDTH * SKALA;
	   b = BAR_G * SKALA;
	   b2 = HEIGHT * SKALA / 3;

       w=x2=x02-x01;
       h=y2=y02-y01;
       x1=0;
       y1=0;
       menu_screen= create_bitmap_ex(32, w, h);
       if (menu_screen==NULL) return;
       Set_Screenplay(menu_screen);

	   setfillstyle_(SOLID_FILL, paper);
	   bar(x1+d, y1+d, x1 + a-d, y1 + b-d);
	   setwritemode(COPY_PUT);
	   setcolor(ink);
	   y1 += MDY1 ;
	   moveto(x1, y1 - YP0);
	   
	   outtext_r_(menu_screen, (*(menu->pola))[menu->foff + menu->poz].txt);
   }

   if ((ptrsz_tmn!=NULL) && (menu->flags & TADD))   //istnieje podmenu
       {
        
         setwritemode(COPY_PUT);
         setcolor (kolory.inkk);  //inkk
         setlinestyle1(SOLID_LINE,0,NORM_WIDTH);
         x2=x1+a-1;
		 ys = y1 + b2 +1;
         LINE (x2-3,   ys,   x2-3,  ys+6);
         LINE (x2-2,   ys+1, x2-2,  ys+5);
         LINE (x2-1,   ys+2, x2-1,  ys+4);
         LINE (x2,     ys+3, x2,    ys+3);
       }

   if((menu->flags&TADD) && !(menu->flags&ICONS))
	{
	  char * st = (*(menu->pola))[menu->poz].txt;
	  int a;

	  ptr = strstr(ptrsz_temp, u8"֎");
      ptr1=strstr(ptrsz_temp,u8"֊");
	  if (ptr == NULL)
	  {
		  ptrsz_temp+=2;
		  st += strlen(st) + 1;
		  a = (menu->xdl) * WIDTH * SKALA;
		  a -= text_width(st);
		  if (menu->flags&NVERT) a -= WIDTH /*8*/ * SKALA;
		  moveto(x1 + a, y1 - YP0);
		  outtext_r_(menu_screen, st);
	  }
       else if (ptr == NULL)
        {
            ptrsz_temp+=5;
            st += strlen(st) + 1;
            a = (menu->xdl) * WIDTH * SKALA;
            a -= text_width(st);
            if (menu->flags&NVERT) a -= WIDTH /*8*/ * SKALA;
            moveto(x1 + a, y1 - YP0);
            outtext_r_(menu_screen, st);
        }
	}

    blit(menu_screen, screen, 0,0,x01,y01,w,h);

    Set_Screenplay(screen);

    destroy_bitmap(menu_screen);

   baronoff(menu);
   frame_on(menu);
}

static TMENU *ACTW=NULL;

static void activate(TMENU *menu)
{
 TMENU **adr;
   menu->next=NULL;
   for(adr=&ACTW;*adr!=NULL;adr=&((*adr)->next));
   *adr=menu;
}

static void deactiv(TMENU *menu)
{
 TMENU **adr;
   for(adr=&ACTW;*adr!=NULL && *adr!=menu;adr=&((*adr)->next));
   *adr=NULL;
}

void deactiv_menu(TMENU *menu) {
    deactiv(menu);
}

void activate_menu(TMENU *menu) {
    activate(menu);
}

static int menu_init_slider(int *var1, int *var2, int *var3, int *var4)
{
    TMENU *tmenu;
    tmenu=menu_address[menu_level-1];

    *var1=tmenu->foff;  //  n_first_layer_in_dlg;
    *var2=tmenu->foff+tmenu->maxw; //  n_last_layer_in_dlg+1;
    //*var3=No_Layers;
    *var3=tmenu->max;  // no_layers;
    *var4=tmenu->maxw;  //NoDialogLayers;
    return 1;
}

static int menu_grab_slider(void *dp3, int d2)
{   char slider_var[32];
    int var1, var2, var3, var4, ret;
    int (*SlideFun)(int*, int*, int*, int*);

    TMENU *tmenu;
    tmenu=menu_address[menu_level-1];

    SlideFun = (int(*)(int *, int *, int *, int *))dp3;

    ret = SlideFun(&var1, &var2, &var3, &var4);

    tmenu->foff=var3 - d2 - (var2-var1);   //n_first_layer_in_dlg
    draww(tmenu);

    return 0;
}

static int find_menu_slider(TMENU *menu, SLIDER *slider)
/*----------------------------------------------------*/
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
        set_scrsave_time();
        show_hide_tip(menu, FALSE);
        baronoff(menu);
        ret1 = Slider_.proc(MSG_CLICK, &Slider_, '\0');
        slider->d2 = Slider_.d2;
        //baronoff(menu);
        size=menu->maxw?menu->maxw:menu->max;

        if (menu->flags&NVERT)
        {
            x1=(menu->xpcz-1)*WIDTH * SKALA;
            y1=(menu->ypcz-1)*32+YP;

            dx = 64;
            dy = 32;
            for (i = 0; i < size; i++) {
                x11 = x1 + i * dx;
                x12 = x11 + dx;
                if ((mouse_x >= x11) && (mouse_x <= x12)) break;
            }
            if (i < size)
            {
                poz1 = menu->foff + i;
                if (poz1 != menu->poz) {
                    set_scrsave_time();
                    menu->poz = poz1;
                }
            }
        }
        else
        {
            if (menu->flags&ICONS)
            {
                x1=(menu->xpcz-1)*WIDTH  * SKALA;
                y1=(menu->ypcz-1)*32 + YP;
                dy=32;
            }
            else {
                x1 = (menu->xpcz - 1) * WIDTH * SKALA;
                y1 = (menu->ypcz - 1) * HEIGHT * SKALA + YP;
                dy=HEIGHT;
            }
            dx = 64;

            for (i = 0; i < size; i++) {
                y11 = y1 + i * dy;
                y12 = y11 + dy;
                if ((mouse_y >= y11) && (mouse_y <= y12)) break;
            }
            if (i < size) {
                poz1 = i;
                if (poz1 != menu->poz) {
                    set_scrsave_time();
                    menu->poz = poz1;
                }
            }
        }
        baron(menu);
        ret=1;
    }

    return ret;
}

void draw_menu_slider(SLIDER *slider)
{
    SLIDER Slider_;
    int var1, var2, var3, var4, ret;
    BITMAP *slbitmap;

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

    int Slider_h = slider->h;
    double slratio = (double) Slider_h / (double) (var3);
    int slbody;
    if (var3 <= var4) slbody = Slider_h;
    else slbody = (var2 - var1) * slratio;

    if (slider->dp != NULL) destroy_bitmap((BITMAP *) slider->dp);

    if (slbody < 4) slider->flags |= 0xF0;  //hidden
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

    Slider_.slider = slider;

    disable_hardware_cursor();

    ret = slider->proc(MSG_DRAW, &Slider_, '\0');

    enable_hardware_cursor();
}

void redraw_menu_slider(SLIDER *slider)
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
int openwh(TMENU *menu)
/*---------------------*/
{

 BITMAP *menu_back;
  SLIDER *Slider;

 float xr,yd;
 int size,x1,y1,x2,y2;

 Test_Menu (menu) ;

 if(menu->back) return 1;
 if(menu->poz+menu->foff>menu->max) { menu->poz=menu->foff=0;}
 size=menu->maxw?menu->maxw:menu->max;
 xr=((menu->flags&NVERT)?size*menu->xdl:menu->xdl);
 yd=((menu->flags&NVERT)?1:size);
 if (menu->flags&ICONS)
 {
	 xr = ((menu->flags&NVERT) ? size * 2 : 2);
	 x1=(menu->xpcz-1)*WIDTH /*8*/ * SKALA;
	 y1=(menu->ypcz-1)*32+YP;
	 
	 x2 = x1 + xr *32 * SKALA + 2 * (GR + 1);
	 y2=y1+yd*32 +2*GR;
 }
 else
 {
	 x1=(menu->xpcz-1)*WIDTH /*8*/ * SKALA;
	 y1=(menu->ypcz-1)*HEIGHT * SKALA+YP;
	 x2=x1+xr*WIDTH /*8*/ * SKALA +2*(GR+1);
	 y2=y1+yd*HEIGHT * SKALA +2*GR;
 }

 set_slider[menu_level-1]=0;
 if ((BAR_POINTER) && (menu->maxw>0) && (menu->maxw<menu->max))
 {
     if (menu->flags&NVERT) y2+=20;
     else x2+=20;
     set_slider[menu_level-1]=1;
 }

 if ((menu_back = create_bitmap(x2 - x1 + 1, y2 - y1 + 36)) == NULL) return 0;
 menu->back=menu_back;

 if (BAR_POINTER) show_mouse(NULL);
getimage(x1,y1-18,x2,y2+18,menu->back);
if (BAR_POINTER) show_mouse(screen);

if (BAR_POINTER) disable_F11();

draww(menu);
if (set_slider[menu_level-1])
{
    Slider=&slider[menu_level-1];
    if (menu->flags&NVERT)
    {
        Slider->x = x1;
        Slider->y = y2-20;
        Slider->h = x2 - x1;
    }
    else
    {
        Slider->x = x2 - 20;
        Slider->y = y1;
        Slider->h = y2 - y1;
    }
    Slider->slider=Slider;
    gui_set_screen(screen);
    draw_menu_slider(Slider);
}
frame_on(menu);

    if ((BAR_POINTER) && (menu_level==1))
    {
        Save_Update_flex(0, &curr_h, &curr_v);
        _free_mouse();
        pointer_on(menu);
    }

 return 1;
}

unsigned Get_Menu_Image_Size (TMENU *menu)
/*-------------------------------------------*/
{
  int xr,yd,size,x1,y1,x2,y2;

 Test_Menu (menu) ;
 if(menu->poz+menu->foff>menu->max) { menu->poz=menu->foff=0;}
 size=menu->maxw?menu->maxw:menu->max;
 xr=((menu->flags&NVERT)?size*menu->xdl:menu->xdl);
 yd=((menu->flags&NVERT)?1:size);
 if (menu->flags&ICONS)
 {
	 x1=(menu->xpcz-1)*WIDTH  * SKALA;
	 y1=(menu->ypcz-1)*32+YP;
	 x2=x1+xr*WIDTH  * SKALA +2*(GR+1);
	 y2=y1+yd*32 +2*GR;
 }
 else
 {
	 x1=(menu->xpcz-1)*WIDTH  * SKALA;
	 y1=(menu->ypcz-1)*HEIGHT * SKALA+YP;
	 x2=x1+xr*WIDTH  * SKALA +2*(GR+1);
	 y2=y1+yd*HEIGHT * SKALA +2*GR;
 }
 return imagesize(x1,y1,x2,y2) ;
}

/*zgaszenie okna z odtworzeniem przeslanianego ekranu*/
void  closewh(TMENU *menu)
{ int x1,y1;
if (menu->flags & TIP)
{
	show_hide_tip(menu, FALSE);
}

if (BAR_POINTER) show_mouse(NULL);
 if(menu->back)
   {
	 if (menu->flags&ICONS)
	 {
	 x1=(menu->xpcz-1)*WIDTH /*8*/ * SKALA;
	 y1=(menu->ypcz-1)*32+YP;
	 }
	 else
	 {
	 x1=(menu->xpcz-1)*WIDTH /*8*/ * SKALA;
	 y1=(menu->ypcz-1)*HEIGHT * SKALA+YP;
	 }
	 putimage(x1, y1-18, menu->back, COPY_PUT);
	 menu->back=NULL;
   }
 if (BAR_POINTER) show_mouse(screen);

}

 /*wyswietlenie okna wraz z trescia*/

 void  popenw(PTMENU *menu)
 { int x0, y0, x00, y00;

     if ((menu == &mCzcionka) || (menu == &mCzcionkaZ) || (menu == &mCzcionkaW))
     {
         Set_HEIGHT_high();
     }

     menu_level++;
     menu_address[menu_level-1]=(char*)menu;

     if (dynamic_menu==TRUE)
     {
         x00 = pikseleX0(X) +20;
         y00 = pikseleY0(Y) +get_pYk() - 24;

         if (menu->flags>=0)
         {
             x0=x00/(WIDTH /*8*/*SKALA);

             if (x0<1) x0=1;
             if (menu_level>1)
                 menu->xpcz=x0+(menu_level*4);
             else menu->xpcz=x0;
         }
         if (menu->flags&ICONS)
         {
             if (menu->maxw==0)
                 y0 = (y00 - menu->max*(32))/32;
             else
                 y0 = (y00 - menu->maxw*(32))/32;
         }
         else
         {
             if (menu->maxw==0) y0=(y00/(HEIGHT*SKALA)) - menu->max - 1;
             else y0=(y00/(HEIGHT*SKALA)) - menu->maxw - 1;
         }
         if (y0<3) y0=3;
         if (menu_level>1) menu->ypcz=y0+1; else menu->ypcz=y0;
     }

     menu->flags = menu->flags | NAWOPEN;
     if(openwh (menu))
     {
         baron (menu) ;
         activate (menu) ;
     }
 }

void  openw(TMENU *menu)
{ int x0, y0, x00, y00;

    if (menu->flags & FIXED)
    {
        popenw((PTMENU *)menu);
        return;
    }

 if ((menu == &mCzcionka) || (menu == &mCzcionkaZ) || (menu == &mCzcionkaW))
 {
	Set_HEIGHT_high();
 }

  menu_level++;
  menu_address[menu_level-1]=(char*)menu;
 
  if (dynamic_menu==TRUE)
   {
	  x00 = pikseleX0(X) +20;
      y00 = pikseleY0(Y) +get_pYk() - 24;

    if (menu->flags>=0)
     {
      x0=x00/(WIDTH *SKALA);
	  
      if (x0<1) x0=1;
      if (menu_level>1) 
		  menu->xpcz=x0+(menu_level*4);
	  else menu->xpcz=x0;
     }
	if (menu->flags&ICONS)
	{
    if (menu->maxw==0)
           y0 = (y00 - menu->max*(32))/32;
       else
           y0 = (y00 - menu->maxw*(32))/32;
	}
	else
    {
    if (menu->maxw==0) y0=(y00/(HEIGHT*SKALA)) - menu->max - 1;
       else y0=(y00/(HEIGHT*SKALA)) - menu->maxw - 1;
	}
    if (y0<3) y0=3;
     if (menu_level>1) menu->ypcz=y0+1; else menu->ypcz=y0;
   }

  menu->flags = menu->flags | NAWOPEN;

  if(openwh (menu))
  {
    baron (menu) ;
    activate (menu) ;
  }
}

/*zgaszenie okna z odtworzeniem przeslanianego ekranu*/
void  closew(TMENU *menu)
{
  int ret;

  if (set_slider[menu_level-1])
  {
      if (slider[menu_level-1].dp!=NULL)
      {
          destroy_bitmap(slider[menu_level - 1].dp);
          slider[menu_level-1].dp=NULL;
      }
      set_slider[menu_level-1]=0;
  }

  menu_level--;

  if(menu->back)
  {
    closewh(menu);
    deactiv(menu);
  }
  if ((menu == &mCzcionka) || (menu == &mCzcionkaZ) || (menu == &mCzcionkaW))
  {
	  Set_HEIGHT_back();
  }

  if ((BAR_POINTER) && (menu_level == 0))
    {
          lock_mouse();
          Save_Update_flex(1, &curr_h, &curr_v);
    }

    if (BAR_POINTER) enable_F11();
}

#include "b_hlp.c"

static void pdraww_scroll (PTMENU *menu, BOOL b_next)
/*---------------------------------------------------*/
{
    int xt, yt, xr;
    float yd;
    int size, x1, y1, x2, y2, x3, ink, paper ;
    char sz_temp [TEMPN] ;
    char *ptrsz_temp ;
    char *ptrsz_tmn;
    unsigned short ptrsz_tcod;
    int ptrsz_iconno;
    int i_len ;
    int xp, yp, x_reg, y_reg ;
    char *sz_ptr ;
    char *ptr, *ptr1;
    int kolor_m, kolor_rgb;
    int ret1, kolor1;

    BITMAP *bitmap_ptr, *buffer;

    int ret, typ_l;
    int x0;
    int offset_st;
    BITMAP *bmp;
    BITMAP *bmp_aux;
    int x, y, ys, b2;
    int typ_f;
    int HEIGHT1, b;
    TEXT fontn= Tdef;
    COLOR_ kolor;
    int color_bak;

    if (menu->flags & NVERT)
    {
        pdraww (menu) ;
        return ;
    }

    paper=kolory.paperm;
    ink = kolory.inkm ;
    size = menu->maxw - 1 ;
    xr = menu->xdl ;
    yd = size ;
    xp = menu->xpcz - 1 ;
    yp = menu->ypcz - 1 ;
    if (b_next == TRUE)
    {
        yp += 1 ;
    }
    if (menu->flags&ICONS)
    {
        x1 = xp * WIDTH /*8*/ * SKALA + GR + 1 ;
        y1 = yp *32 + GR + YP ;
        x2 = x1 + 64;
        y2 = y1 + yd * 32 + 1;
        b2 = 32 * SKALA / 3;

    }
    else
    {
        x1 = xp * WIDTH /*8*/ * SKALA + GR + 1 ;
        y1 = yp * HEIGHT * SKALA + GR + YP ;
        x2 = x1 + xr * WIDTH /*8*/ * SKALA ;
        y2 = y1 + yd * HEIGHT * SKALA + 1;
        b2 = HEIGHT * SKALA / 3;
    }
    bitmap_ptr = create_bitmap(x2 - x1 + 1, y2 - y1 + 1);
    if (bitmap_ptr!=NULL)
    {

        setviewport(viewinfo[menu_level].left, viewinfo[menu_level].top, viewinfo[menu_level].right,viewinfo[menu_level].bottom, 1);

        x1-=viewinfo[menu_level].left;
        y1-=viewinfo[menu_level].top;
        x2-=viewinfo[menu_level].left;
        y2-=viewinfo[menu_level].top;

        sz_ptr = (char *)bitmap_ptr;

        getimage(x1, y1, x2, y2, sz_ptr);

        if (menu->flags&ICONS)
        {
            putimage(x1, y1 + ((b_next == TRUE) ? -32 : +32), sz_ptr, COPY_PUT);
        }
        else
        {
            putimage(x1, y1 + ((b_next == TRUE) ? -HEIGHT * SKALA : +HEIGHT * SKALA), sz_ptr, COPY_PUT);
        }

        setfillstyle_(SOLID_FILL, paper);
        x_reg = x1;
        y_reg = y1;
        if (b_next == TRUE)
        {
            if (menu->flags&ICONS)
            {
                y_reg = y2 - 32;
            }
            else
            {
                y_reg = y2 - HEIGHT * SKALA;
            }
        }
        if (menu->flags&ICONS)
        {
            bar(x_reg, y_reg, x_reg + 32 * 2, y_reg + 32);
        }
        else
        {
            bar(x_reg, y_reg, x_reg + WIDTH /*8*/ * SKALA * menu->xdl, y_reg + HEIGHT * SKALA);
        }
        setcolor(ink);
        xt = x_reg + 2;
        yt = y_reg - 2 * SKALA + MDY;

        if (menu->flags & ICONS) moveto(xt, yt - YP1);
        else moveto(xt, yt - YP0);
        ptrsz_temp = (char *)(*(menu->pola))[menu->foff + menu->poz].txt;
        ptrsz_tmn = (char *)(*(menu->pola))[menu->foff + menu->poz].menu;
        ptrsz_tcod = (*(menu->pola))[menu->foff + menu->poz].wcod;
        ptrsz_iconno = (*(menu->pola))[menu->foff + menu->poz].iconno;

        ptr = strstr(ptrsz_temp, u8"֍");
        if (ptr != NULL)
        {
            ptrsz_temp+=2;
            if ((menu->foff + menu->poz) >= 16)
            {
                if (LiniaG.kolor > 16) kolor_m = LiniaG.kolor;
                else kolor_m = 254;
            }
            else
            {
                kolor_m = GetColorAC1(menu->foff + menu->poz + 1);
            }
            setfillstyle_(SOLID_FILL, kolor_m);

            if (menu->flags&ICONS)
            {
                bar(x_reg, yt - 2, x_reg + (x2 - x1), yt + 32 - 2);
            }
            else
            {
                bar(x_reg, yt - 2, x_reg+(x2 - x1), yt + HEIGHT * SKALA - 2);
            }
            if (kolor_m > 16)
            {
                kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
                if (kolor_rgb < 96) setcolor(15); else setcolor(0);
            }
            else if (kolor_m == 0) setcolor(15);
            else setcolor(0);
        }
        else
        {
            ptr = strstr(ptrsz_temp, u8"֎");
            ptr1=strstr(ptrsz_temp,u8"֊");
            if (ptr != NULL)
            {
                ptrsz_temp+=2;

                if (ptrsz_iconno == 158) kolor_m = GetColorAC1(LiniaG.kolor);
                else if (ptrsz_iconno == 177) kolor_m = GetColorAC1(zmwym.Lkolor);
                else if (ptrsz_iconno == 178) kolor_m = GetColorAC1(zmwym.Tkolor);
                else if (ptrsz_iconno == 495) kolor_m = GetColorAC1(sektory_arkusza_ext.frame_color);
                else if (ptrsz_iconno == 496) kolor_m = GetColorAC1(sektory_arkusza_ext.sector_color);

                else if (ptrsz_iconno == 723) kolor_m = GetColorAC1(static_colors.node_element_color);
                else if (ptrsz_iconno == 771) kolor_m = GetColorAC1(static_colors.tension_color);
                else if (ptrsz_iconno == 758) kolor_m = GetColorAC1(static_colors.compression_color);
                else if (ptrsz_iconno == 759) kolor_m = GetColorAC1(static_colors.shear_color);
                else if (ptrsz_iconno == 760) kolor_m = GetColorAC1(static_colors.moment_color);
                else if (ptrsz_iconno == 761) kolor_m = GetColorAC1(static_colors.deformation_color);
                else if (ptrsz_iconno == 763) kolor_m = GetColorAC1(static_colors.reaction_color);
                else if (ptrsz_iconno == 801) kolor_m = GetColorAC1(static_stress_colors.axial_stress_plus_color);
                else if (ptrsz_iconno == 802) kolor_m = GetColorAC1(static_stress_colors.axial_stress_minus_color);
                else if (ptrsz_iconno == 803) kolor_m = GetColorAC1(static_stress_colors.shear_stress_color);
                else if (ptrsz_iconno == 814) kolor_m = GetColorAC1(static_colors.dynamic_color);
                else if (ptrsz_iconno > 1000) kolor_m = GetColorAC1(ptrsz_iconno-1000);

                else kolor_m = GetColorAC1(LiniaG.kolor); //temporary

                setfillstyle_(SOLID_FILL, kolor_m);

                if (menu->flags&ICONS)
                {
                    bar(x_reg, yt - 2, x_reg + (x2 - x1), yt + 32 - 2);
                }
                else
                {
                    bar(x_reg, yt - 2, x_reg + (x2 - x1), yt + HEIGHT * SKALA - 2);
                }
                if (kolor_m > 16)
                {
                    kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
                    if (kolor_rgb < 96) setcolor(15); else setcolor(0);
                }
                else if (kolor_m == 0) setcolor(15);
                else setcolor(0);
            }
            else if (ptr1!=NULL)
            {
                ptrsz_temp += 2;
                ret1 = sscanf(ptrsz_temp, "%03d", &kolor1);
                kolor_m = GetColorAC1(kolor1);
                ptrsz_temp += 3;

                setfillstyle_(SOLID_FILL, kolor_m);

                if (menu->flags&ICONS)
                {
                    bar(x_reg, yt - 2, x_reg + (x2 - x1), yt + 32 - 2);
                }
                else
                {
                    bar(x_reg, yt - 2, x_reg + (x2 - x1), yt + HEIGHT * SKALA - 2);
                }
                if (kolor_m > 16)
                {
                    kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
                    if (kolor_rgb < 96) setcolor(15); else setcolor(0);
                }
                else if (kolor_m == 0) setcolor(15);
                else setcolor(0);
            }
            else
            {
                setcolor(ink);
                ptr = strstr(ptrsz_temp, u8"֏");
                if (ptr != NULL)
                {
                    typ_l = menu->foff + menu->poz;
                    linestyle(typ_l + 64);

                    x0=0;
                    ret = lineCuncut (x0+ (x2 - x1) / 2,yt + (HEIGHT * SKALA / 2) - 1, x0 + (x2 - x1)-2, yt + (HEIGHT * SKALA / 2) - 1);

                    ptrsz_temp+=2;
                }
                else
                {
                    ptr = strstr(ptrsz_temp, u8"҂");
                    if (ptr != NULL)
                    {

                        typ_l = menu->foff + menu->poz;
                        linestyle(typ_l * 32);

                        x0=0;
                        ret=lineCuncut (x0+(x2-x1)/2,yt+(HEIGHT * SKALA / 2) - 1, x0 + (x2-x1)-2, yt+(HEIGHT * SKALA / 2) - 1);

                        ptrsz_temp+=2;
                    }
                    else
                    {
                        if ((menu == &mCzcionka) || (menu == &mCzcionkaZ) || (menu == &mCzcionkaW))
                        {
                            typ_f = menu->foff + menu->poz;
                        }

                    }

                }

            }
        }

        //instead of texts we can put images
        //images can occupy space 32x32 pxl, if total width would be some 1
        //total with should be 64pxl, to show extra options or so

        if (menu->flags&ICONS)
        {
            if (ptrsz_iconno>=FIRST_CLIENT_BITMAP_NO) bmp = (BITMAP *)client_bitmap_load[ptrsz_iconno-FIRST_CLIENT_BITMAP_NO];
            else bmp = (BITMAP *)get_icons_p(ptrsz_iconno);
            if (bmp != NULL)
            {
                if (bmp->w==32) blit(bmp, screen, 0, 0, xt - 1, yt - 2, 32, 32);
                else
                {
                    buffer = create_bitmap_ex(32, 32, 32);
                    if (buffer != NULL)
                    {
                        clear_to_color(buffer, bitmap_mask_color(buffer));
                        rotate_scaled_sprite(buffer, bmp, 0, 0, ftofix(0.0), ftofix(32.0 / (double)bmp->w));
                        set_alpha_blender();
                        draw_trans_sprite(screen, buffer, xt - 1, yt - 2);
                        destroy_bitmap(buffer);
                        set_trans_blender(0, 0, 0, TRANSLUCENCY);
                        set_mode_solid();
                    }
                }
            }
            if (ptrsz_tcod < 32)
            {
                bmp_aux = (BITMAP *)get_icons_aux_p(ptrsz_tcod);
                if (bmp_aux != NULL)
                {
                    if (bmp_aux->w==32) blit(bmp_aux, screen, 0, 0, xt - 1 + 32, yt - 2, 32, 32);
                    else
                    {
                        buffer = create_bitmap_ex(32, 32, 32);
                        if (buffer != NULL)
                        {
                            clear_to_color(buffer, bitmap_mask_color(buffer));
                            rotate_scaled_sprite(buffer, bmp_aux, 0, 0, ftofix(0.0), ftofix(32.0 / (double)bmp_aux->w));
                            set_alpha_blender();
                            draw_trans_sprite(screen, buffer, xt - 1 + 32, yt - 2);
                            destroy_bitmap(buffer);
                            set_trans_blender(0, 0, 0, TRANSLUCENCY);
                            set_mode_solid();
                        }
                    }
                }
            }
            else if (ptrsz_tcod > 1920)
            {
                bmp_aux = (BITMAP *)get_icons_p(ptrsz_tcod-1920);
                if (bmp_aux != NULL)
                {
                    if (bmp_aux->w == 32) blit(bmp_aux, screen, 0, 0, xt - 1 + 32, yt - 2, 32, 32);
                    else
                    {
                        buffer = create_bitmap_ex(32, 32, 32);
                        if (buffer != NULL)
                        {
                            clear_to_color(buffer, bitmap_mask_color(buffer));
                            rotate_scaled_sprite(buffer, bmp_aux, 0, 0, ftofix(0.0), ftofix(32.0 / (double)bmp_aux->w));
                            set_alpha_blender();
                            draw_trans_sprite(screen, buffer, xt - 1 + 32, yt - 2);
                            destroy_bitmap(buffer);
                            set_trans_blender(0, 0, 0, TRANSLUCENCY);
                            set_mode_solid();
                        }
                    }
                }
            }
            else
            {
                sz_temp[0] = ptrsz_tcod;
                sz_temp[1] = '\0';
                x = getx();
                y = gety();

                b = (int)(-HEIGHT*0.357 + 15);
                moveto(x + 34, y + b);

                outtext_r_(screen, sz_temp);
            }
        }
        else
        {
            if ((menu == &mCzcionka) || (menu == &mCzcionkaZ) || (menu == &mCzcionkaW))
            {
                x = getx();
                y = gety();
                HEIGHT1 = 28;
                b = (int)(HEIGHT - (HEIGHT - HEIGHT1) / 2);
                moveto(x, y + b);

                i_len = menu_len(ptrsz_temp, xr);
                strncpy(sz_temp, ptrsz_temp, i_len);
                sz_temp[i_len] = '\0';

                if (typ_f < i__font_nomax)
                {

                    if ((PTRS__Text_Style[typ_f]->type < 2) || (PTRS__Text_Style[typ_f]->type == 3))
                        draw_font_name(ptrsz_temp, typ_f, x, y + 21);
                    else
                    {

                        kolor.red = 0;
                        kolor.gre = 0;
                        kolor.blu = 100;
                        strcpy(fontn.text, ptrsz_temp);
                        fontn.dl = (int)strlen(fontn.text);
                        fontn.n = T18 + fontn.dl;
                        fontn.czcionka = typ_f;
                        fontn.kat = 0.0;
                        fontn.bold = 0;
                        fontn.width_factor = 1.0;
                        fontn.italics = 0;
                        fontn.justowanie = j_do_lewej;
                        fontn.multiline = 0;
                        fontn.ukryty = 0;
                        fontn.wysokosc = 4;
                        fontn.kolor = 0;
                        fontn.width = 0;
                        fontn.height = 0;

                        draw_font_name_ttf(&fontn, (char *)&fontn.text, screen, x, y + 21, x2, fontn.kat, 20.0, kolor, COPY_PUT, 0, 0);  //22
                    }
                }
                else
                {
                    color_bak = getcolor();
                    setcolor(184);
                    TTF_text_UI_W_H_(screenplay, ptrsz_temp, x, y-2, NULL, NULL, WIDTH_, HEIGHT_);
                    setcolor(color_bak);
                }

                y1=y;
            }
            else
            {
                i_len = menu_len(ptrsz_temp, xr);
                strncpy(sz_temp, ptrsz_temp, i_len);
                sz_temp[i_len] = '\0';
                outtext_r_(screen, sz_temp);
            }
        }

        offset_st = 0;
        if ((ptrsz_tmn != NULL) && (menu->flags & TADD))   //istnieje podmenu
        {
            setwritemode(COPY_PUT);
            setcolor(kolory.inkk);  //inkk
            setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
            x3 = x2 - x1 - 1;
            ys = y1 + b2;
            LINE(x_reg + x3 - 3, ys, x_reg + x3 - 3, ys + 6);
            LINE(x_reg + x3 - 2, ys + 1, x_reg + x3 - 2, ys + 5);
            LINE(x_reg + x3 - 1, ys + 2, x_reg + x3 - 1, ys + 4);
            LINE(x_reg + x3, ys + 3, x_reg + x3, ys + 3);
            offset_st = 8;
        }

        setcolor(kolory.inkm);

        if ((menu->flags&TADD) && !(menu->flags&ICONS))
        {
            char * st = (*(menu->pola))[menu->foff+menu->poz].txt;
            int a;

            st += strlen(st) + 1;
            a = (menu->xdl) * WIDTH * SKALA;
            a -= text_width (st);
            moveto(xt+a,yt-YP0);
            outtext_r_(screen, st);
        }

        setviewport(viewinfo[menu_level-1].left, viewinfo[menu_level-1].top, viewinfo[menu_level-1].right,viewinfo[menu_level-1].bottom, 1);
    }
    else
    {
        pdraww (menu) ;
    }
}

static void draww_scroll (TMENU *menu, BOOL b_next)
/*---------------------------------------------------*/
{
  int xt, yt, xr;
  float yd;
  int size, x1, y1, x2, y2, x3, ink, paper ;
  char sz_temp [TEMPN] ;
  char *ptrsz_temp ;
  char *ptrsz_tmn;
  unsigned short ptrsz_tcod;
  int ptrsz_iconno;
  int i_len ;
  int xp, yp, x_reg, y_reg ;
  char *sz_ptr ;
  char *ptr, *ptr1;
  int kolor_m, kolor_rgb;
  int ret1, kolor1;

  BITMAP *bitmap_ptr, *buffer;

  int ret, typ_l;
  int x0;
  int offset_st;
  BITMAP *bmp;
  BITMAP *bmp_aux;
  int x, y, ys, b2;
  int typ_f;
  int HEIGHT1, b;
  TEXT fontn= Tdef;
  COLOR_ kolor;
  int color_bak;

    if (menu->flags & FIXED)
    {
        pdraww_scroll((PTMENU*)menu, b_next);
        return;
    }

   if (menu->flags & NVERT)
   {
      draww (menu) ;
      return ;
   }
 
   paper=kolory.paperm;
   ink = kolory.inkm ;
   size = menu->maxw - 1 ;
   xr = menu->xdl ;
   yd = size ;
   xp = menu->xpcz - 1 ;
   yp = menu->ypcz - 1 ;
   if (b_next == TRUE)
   {
     yp += 1 ;
   }
   if (menu->flags&ICONS)
   {
	   x1 = xp * WIDTH  * SKALA + GR + 1 ;
	   y1 = yp *32 + GR + YP ;
	   x2 = x1 + 64;
	   y2 = y1 + yd * 32 + 1;
	   b2 = 32 * SKALA / 3;

   }
   else
   {
	   x1 = xp * WIDTH  * SKALA + GR + 1 ;
	   y1 = yp * HEIGHT * SKALA + GR + YP ;
	   x2 = x1 + xr * WIDTH  * SKALA ;
	   y2 = y1 + yd * HEIGHT * SKALA + 1;
	   b2 = HEIGHT * SKALA / 3;
   }

   bitmap_ptr = create_bitmap(x2 - x1 + 1, y2 - y1 + 1);
   if (bitmap_ptr!=NULL)
   {
       setviewport(viewinfo[menu_level].left, viewinfo[menu_level].top, viewinfo[menu_level].right,viewinfo[menu_level].bottom, 1);

       x1-=viewinfo[menu_level].left;
       y1-=viewinfo[menu_level].top;
       x2-=viewinfo[menu_level].left;
       y2-=viewinfo[menu_level].top;

	   sz_ptr = (char *)bitmap_ptr;

	   getimage(x1, y1, x2, y2, sz_ptr);

	   if (menu->flags&ICONS)
	   {
		   putimage(x1, y1 + ((b_next == TRUE) ? -32 : +32), sz_ptr, COPY_PUT);
	   }
	   else
	   {
		   putimage(x1, y1 + ((b_next == TRUE) ? -HEIGHT * SKALA : +HEIGHT * SKALA), sz_ptr, COPY_PUT);
	   }

	   setfillstyle_(SOLID_FILL, paper);
	   x_reg = x1;
	   y_reg = y1;
	   if (b_next == TRUE)
	   {
		   if (menu->flags&ICONS)
		   {
			   y_reg = y2 - 32;
		   }
		   else
		   {
			   y_reg = y2 - HEIGHT * SKALA;
		   }
	   }
	   if (menu->flags&ICONS)
	   {
		   bar(x_reg, y_reg, x_reg + 32 * 2, y_reg + 32);
	   }
	   else
	   {
		   bar(x_reg, y_reg, x_reg + WIDTH /*8*/ * SKALA * menu->xdl, y_reg + HEIGHT * SKALA);
	   }
	   setcolor(ink);
	   xt = x_reg + 2;
	   yt = y_reg - 2 * SKALA + MDY;


       if (menu->flags & ICONS) moveto(xt, yt - YP1);
	   else moveto(xt, yt - YP0);
	   ptrsz_temp = (char *)(*(menu->pola))[menu->foff + menu->poz].txt;
	   ptrsz_tmn = (char *)(*(menu->pola))[menu->foff + menu->poz].menu;
	   ptrsz_tcod = (*(menu->pola))[menu->foff + menu->poz].wcod;
	   ptrsz_iconno = (*(menu->pola))[menu->foff + menu->poz].iconno;

	   ptr = strstr(ptrsz_temp, u8"֍");
	   if (ptr != NULL)
	   {
		   ptrsz_temp+=2;
		   if ((menu->foff + menu->poz) >= 16)
		   {
			   if (LiniaG.kolor > 16) kolor_m = LiniaG.kolor;
			   else kolor_m = 254;
		   }
		   else
		   {
			   kolor_m = GetColorAC1(menu->foff + menu->poz + 1);
		   }
		   setfillstyle_(SOLID_FILL, kolor_m);
		  
		   if (menu->flags&ICONS)
		   {
			   bar(x_reg, yt - 2, x_reg + (x2 - x1), yt + 32 - 2);
		   }
		   else
		   {
			   bar(x_reg, yt - 2, x_reg+(x2 - x1), yt + HEIGHT * SKALA - 2);
		   }
		   if (kolor_m > 16)
		   {
			   kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
			   if (kolor_rgb < 96) setcolor(15); else setcolor(0);
		   }
		   else if (kolor_m == 0) setcolor(15);
		   else setcolor(0);
	   }
	   else
	   {
		   ptr = strstr(ptrsz_temp, u8"֎");
           ptr1=strstr(ptrsz_temp,u8"֊");
		   if (ptr != NULL)
		   {
			   ptrsz_temp+=2;

               if (ptrsz_iconno == 158) kolor_m = GetColorAC1(LiniaG.kolor);
               else if (ptrsz_iconno == 177) kolor_m = GetColorAC1(zmwym.Lkolor);
               else if (ptrsz_iconno == 178) kolor_m = GetColorAC1(zmwym.Tkolor);
               else if (ptrsz_iconno == 495) kolor_m = GetColorAC1(sektory_arkusza_ext.frame_color);
               else if (ptrsz_iconno == 496) kolor_m = GetColorAC1(sektory_arkusza_ext.sector_color);

               else if (ptrsz_iconno == 723) kolor_m = GetColorAC1(static_colors.node_element_color);
               else if (ptrsz_iconno == 771) kolor_m = GetColorAC1(static_colors.tension_color);
               else if (ptrsz_iconno == 758) kolor_m = GetColorAC1(static_colors.compression_color);
               else if (ptrsz_iconno == 759) kolor_m = GetColorAC1(static_colors.shear_color);
               else if (ptrsz_iconno == 760) kolor_m = GetColorAC1(static_colors.moment_color);
               else if (ptrsz_iconno == 761) kolor_m = GetColorAC1(static_colors.deformation_color);
               else if (ptrsz_iconno == 763) kolor_m = GetColorAC1(static_colors.reaction_color);
               else if (ptrsz_iconno == 801) kolor_m = GetColorAC1(static_stress_colors.axial_stress_plus_color);
               else if (ptrsz_iconno == 802) kolor_m = GetColorAC1(static_stress_colors.axial_stress_minus_color);
               else if (ptrsz_iconno == 803) kolor_m = GetColorAC1(static_stress_colors.shear_stress_color);
               else if (ptrsz_iconno == 814) kolor_m = GetColorAC1(static_colors.dynamic_color);
               else if (ptrsz_iconno > 1000) kolor_m = GetColorAC1(ptrsz_iconno-1000);

               else kolor_m = GetColorAC1(LiniaG.kolor); //temporary

			   setfillstyle_(SOLID_FILL, kolor_m);
			   
			   if (menu->flags&ICONS)
			   {
				   bar(x_reg, yt - 2, x_reg + (x2 - x1), yt + 32 - 2);
			   }
			   else
			   {
				   bar(x_reg, yt - 2, x_reg + (x2 - x1), yt + HEIGHT * SKALA - 2);
			   }
			   if (kolor_m > 16)
			   {
				   kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
				   if (kolor_rgb < 96) setcolor(15); else setcolor(0);
			   }
			   else if (kolor_m == 0) setcolor(15);
			   else setcolor(0);
		   }
           else if (ptr1!=NULL)
           {
               ptrsz_temp += 2;
               ret1 = sscanf(ptrsz_temp, "%03d", &kolor1);
               kolor_m = GetColorAC1(kolor1);
               ptrsz_temp += 3;

               setfillstyle_(SOLID_FILL, kolor_m);

               if (menu->flags&ICONS)
               {
                   bar(x_reg, yt - 2, x_reg + (x2 - x1), yt + 32 - 2);
               }
               else
               {
                   bar(x_reg, yt - 2, x_reg + (x2 - x1), yt + HEIGHT * SKALA - 2);
               }
               if (kolor_m > 16)
               {
                   kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
                   if (kolor_rgb < 96) setcolor(15); else setcolor(0);
               }
               else if (kolor_m == 0) setcolor(15);
               else setcolor(0);
           }
		   else
		   {
			   setcolor(ink);
			   ptr = strstr(ptrsz_temp, u8"֏");
			   if (ptr != NULL)
			   {
				   typ_l = menu->foff + menu->poz;
				   linestyle(typ_l + 64);

                   x0=0;
                   ret = lineCuncut (x0+ (x2 - x1) / 2,yt + (HEIGHT * SKALA / 2) - 1, x0 + (x2 - x1)-2, yt + (HEIGHT * SKALA / 2) - 1);

                   ptrsz_temp+=2;
			   }
			   else
			   {
				   ptr = strstr(ptrsz_temp, u8"҂");
				   if (ptr != NULL)
				   {
					   
					   typ_l = menu->foff + menu->poz;
					   linestyle(typ_l * 32);

                       x0=0;
                       ret=lineCuncut (x0+(x2-x1)/2,yt+(HEIGHT * SKALA / 2) - 1, x0 + (x2-x1)-2, yt+(HEIGHT * SKALA / 2) - 1);

                       ptrsz_temp+=2;
				   }
				   else
				   {  

						if ((menu == &mCzcionka) || (menu == &mCzcionkaZ) || (menu == &mCzcionkaW))
						{
							typ_f = menu->foff + menu->poz;
						}
					   
				   }

			   }

		   }
	   }

	   //instead of texts we can put images
	   //images can occupy space 32x32 pxl, if total width would be some 1
	   //total with should be 64pxl, to show extra options or so

	   if (menu->flags&ICONS)
	   {
           if (ptrsz_iconno>=FIRST_CLIENT_BITMAP_NO) bmp = (BITMAP *)client_bitmap_load[ptrsz_iconno-FIRST_CLIENT_BITMAP_NO];
		   else bmp = (BITMAP *)get_icons_p(ptrsz_iconno);
		   if (bmp != NULL)
		   {   
		    if (bmp->w==32) blit(bmp, screen, 0, 0, xt - 1, yt - 2, 32, 32);
			else
			{
				buffer = create_bitmap_ex(32, 32, 32);
				if (buffer != NULL)
				{
					clear_to_color(buffer, bitmap_mask_color(buffer));
					rotate_scaled_sprite(buffer, bmp, 0, 0, ftofix(0.0), ftofix(32.0 / (double)bmp->w));
					set_alpha_blender();
					draw_trans_sprite(screen, buffer, xt - 1, yt - 2);
					destroy_bitmap(buffer);
					set_trans_blender(0, 0, 0, TRANSLUCENCY);
					set_mode_solid();
				}
			}   
		   }
		   if (ptrsz_tcod < 32)
		   {
			   bmp_aux = (BITMAP *)get_icons_aux_p(ptrsz_tcod);
			   if (bmp_aux != NULL)
			   {	  
				if (bmp_aux->w==32) blit(bmp_aux, screen, 0, 0, xt - 1 + 32, yt - 2, 32, 32);
				else
				{
					buffer = create_bitmap_ex(32, 32, 32);
					if (buffer != NULL)
					{
						clear_to_color(buffer, bitmap_mask_color(buffer));
						rotate_scaled_sprite(buffer, bmp_aux, 0, 0, ftofix(0.0), ftofix(32.0 / (double)bmp_aux->w));
						set_alpha_blender();
						draw_trans_sprite(screen, buffer, xt - 1 + 32, yt - 2);
						destroy_bitmap(buffer);
						set_trans_blender(0, 0, 0, TRANSLUCENCY);
						set_mode_solid();
					}
				}
			   }
		   }
		   else if (ptrsz_tcod > 1920)
		   {
			   bmp_aux = (BITMAP *)get_icons_p(ptrsz_tcod-1920);
			   if (bmp_aux != NULL)
			   {
				if (bmp_aux->w == 32) blit(bmp_aux, screen, 0, 0, xt - 1 + 32, yt - 2, 32, 32);
				else
				{
					buffer = create_bitmap_ex(32, 32, 32);
					if (buffer != NULL)
					{
						clear_to_color(buffer, bitmap_mask_color(buffer));
						rotate_scaled_sprite(buffer, bmp_aux, 0, 0, ftofix(0.0), ftofix(32.0 / (double)bmp_aux->w));
						set_alpha_blender();
						draw_trans_sprite(screen, buffer, xt - 1 + 32, yt - 2);
						destroy_bitmap(buffer);
						set_trans_blender(0, 0, 0, TRANSLUCENCY);
						set_mode_solid();
					}
				}
			   }
		   }
		   else
		   {
			   sz_temp[0] = ptrsz_tcod;
			   sz_temp[1] = '\0';
			   x = getx();
			   y = gety();

               b = (int)(-HEIGHT*0.357 + 15);
               moveto(x + 34, y + b);

			   outtext_r_(screen, sz_temp);
		   }
	   }
	   else
	   {
           if ((menu == &mCzcionka) || (menu == &mCzcionkaZ) || (menu == &mCzcionkaW))
		   {
			   x = getx();
			   y = gety();
			   HEIGHT1 = 28;
			   b = (int)(HEIGHT - (HEIGHT - HEIGHT1) / 2);
			   moveto(x, y + b);

			   i_len = menu_len(ptrsz_temp, xr);
			   strncpy(sz_temp, ptrsz_temp, i_len);
			   sz_temp[i_len] = '\0';

			   if (typ_f < i__font_nomax)
			   {

				   if ((PTRS__Text_Style[typ_f]->type < 2) || (PTRS__Text_Style[typ_f]->type == 3))
					   draw_font_name(ptrsz_temp, typ_f, x, y + 21);
				   else
				   {
					   
					   kolor.red = 0;
					   kolor.gre = 0;
					   kolor.blu = 100;
					   strcpy(fontn.text, ptrsz_temp);
					   fontn.dl = (int)strlen(fontn.text);
					   fontn.n = T18 + fontn.dl;
					   fontn.czcionka = typ_f;
					   fontn.kat = 0.0;
					   fontn.bold = 0;
					   fontn.width_factor = 1.0;
					   fontn.italics = 0;
					   fontn.justowanie = j_do_lewej;
					   fontn.multiline = 0;
					   fontn.ukryty = 0;
					   fontn.wysokosc = 4;
					   fontn.kolor = 0; 
					   fontn.width = 0;
					   fontn.height = 0;

					   draw_font_name_ttf(&fontn, (char *)&fontn.text, screen, x, y + 21, x2, fontn.kat, 20.0, kolor, COPY_PUT, 0, 0);  //22
				   }
			   }
			   else
			   {
				   color_bak = getcolor();
				   setcolor(184);
				   TTF_text_UI_W_H_(screenplay, ptrsz_temp, x, y-2, NULL, NULL, WIDTH_, HEIGHT_);
				   setcolor(color_bak);
			   }

               y1=y;
		   }
		   else
		   {
		
			   i_len = menu_len(ptrsz_temp, xr);
			   strncpy(sz_temp, ptrsz_temp, i_len);
			   sz_temp[i_len] = '\0';
			   outtext_r_(screen, sz_temp);
		   }
	   }

	   offset_st = 0;
	   if ((ptrsz_tmn != NULL) && (menu->flags & TADD))   //istnieje podmenu
	   {
		   
		   setwritemode(COPY_PUT);
		   setcolor(kolory.inkk);  //inkk
		   setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
		   x3 = x2 - x1 - 1; // 10;
		   ys = y1 + b2;
		   LINE(x_reg + x3 - 3, ys, x_reg + x3 - 3, ys + 6);
		   LINE(x_reg + x3 - 2, ys + 1, x_reg + x3 - 2, ys + 5);
		   LINE(x_reg + x3 - 1, ys + 2, x_reg + x3 - 1, ys + 4);
		   LINE(x_reg + x3, ys + 3, x_reg + x3, ys + 3);
		   offset_st = 8;
	   }

	   setcolor(kolory.inkm);

      if ((menu->flags&TADD) && !(menu->flags&ICONS))
      {
	  char * st = (*(menu->pola))[menu->foff+menu->poz].txt;
	  int a;

	  st += strlen(st) + 1;
	  a = (menu->xdl) * WIDTH * SKALA;
	  a -= text_width (st);
	  moveto(xt+a,yt-YP0);
	  outtext_r_(screen, st);
      }

      setviewport(viewinfo[menu_level-1].left, viewinfo[menu_level-1].top, viewinfo[menu_level-1].right,viewinfo[menu_level-1].bottom, 1);
   }
   else
   {
     draww (menu) ;
   }
}

static int  cnext_min(TMENU *menu)
{
	int ret;
	ret = frame_down(menu);
	return 0;
}

static int  cnext_min_count(TMENU *menu, int count)
{
int i, cn;

    if (BAR_POINTER) {
        cn = cnext_(menu);
        return 0;
    }
    for (i = 0; i < abs(count); i++)
	    if (frame_down(menu)) break;
	return 0;
}
/*funkcja przesuwania kursora o 1 poz. w dol menu*/

static int  cnext(TMENU *menu)
{
	int cnm;
	cnm=cnext_min_count(menu, step*nsteps);
	return 0;
}


static void shift_cursor(TMENU *menu)
{  float yd, xr;
    int x01,y01,x02,y02,w,h;
    int size;
    int mouse_x_, mouse_y_;
    SLIDER *Slider;

    size=menu->maxw?menu->maxw:menu->max;
    if (menu->flags&ICONS)
    {

        menu->xdl = (int)(64 * SKALA);

        xr = ((menu->flags&NVERT) ? size * 2 : 2);
        yd=((menu->flags&NVERT)?1:size);
        x01=(menu->xpcz-1)*WIDTH /*8*/ * SKALA;
        y01=(menu->ypcz-1)*32 + YP;

        x02 = x01 + xr * 32 * SKALA + 2 * (GR + 1);
        y02=y01+yd*32  +2*GR;
        //b2 = 32 * SKALA/ 3;
    }
    else
    {
        xr=((menu->flags&NVERT)?size*menu->xdl:menu->xdl);
        yd=((menu->flags&NVERT)?1:size);
        x01=(menu->xpcz-1)*WIDTH /*8*/ * SKALA;
        y01=(menu->ypcz-1)*HEIGHT * SKALA + YP;
        x02=x01+xr*WIDTH /*8*/ * SKALA  +2*(GR+1);
        y02=y01+yd*HEIGHT * SKALA  +2*GR;
        //b2 = HEIGHT * SKALA / 3;
    }

    mouse_x_=mouse_x;
    mouse_y_=mouse_y;
    if ((mouse_x_ >= x01) && (mouse_x_ <= x02))
    {
        if ((mouse_y_ >= y01) && (mouse_y_ <= y02))
        {
            if (menu->flags&NVERT)
                position_mouse(bar_center.x, mouse_y_);
            else position_mouse(mouse_x_, bar_center.y);
        }
    }
    //if (menu->max > menu->maxw)
    if (set_slider[menu_level-1])
    {
        Slider=&slider[menu_level-1];
        gui_set_screen(screen);
        setwritemode(COPY_PUT);
        redraw_menu_slider(Slider);
    }

}


static int  cnext_(TMENU *menu)
{
	int mickeyx, mickeyy;

    if (menu==NULL) return 0;


	if (menu->flags & TIP)
	{
		show_hide_tip(menu, FALSE);
	}

   if (menu->poz + menu->foff == menu->max - 1)
   {
     return 0 ;
   }

   baronoff (menu) ;
   menu->poz++ ;
   if (menu->maxw != 0)
   {
     if (menu->poz + menu->foff >= menu->max)
     {
       menu->poz-- ;
     }
     else
     if (menu->poz >= menu->maxw)
     {
       menu->foff++ ;
       menu->poz-- ;
	  frame_off(menu);
      show_mouse(NULL);
	  draww_scroll (menu, TRUE) ;
	  frame_on(menu);
      show_mouse(screen);

     }
   }
   baron (menu) ;
   if (BAR_POINTER) shift_cursor(menu);
   get_mouse_mickeys(&mickeyx, &mickeyy);
   return 0 ;
}

static int  cprev_min(TMENU *menu)
{
	int ret;
	ret = frame_up(menu);
	return 0;
}

static int  cprev_min_count(TMENU *menu, int count)
{
	int i, cn;
    if (BAR_POINTER) {
        cn = cprev_(menu);
        return 0;
    }
	for (i = 0; i < abs(count); i++)
		if (frame_up(menu)) break;
	return 0;
}
/*funkcja przesuwania kursora o 1 poz. w gore menu*/
static int  cprev(TMENU *menu)
{
	cprev_min_count(menu, step*nsteps);
	return 0;
}

static int  cprev_(TMENU *menu)
{
	int mickeyx, mickeyy;

    if (menu==NULL) return 0;

	if (menu->flags & TIP)
	{
		show_hide_tip(menu, FALSE);
	}

   if (menu->poz == 0 && menu->foff==0 )
   {
     return 0 ;
   }
   baronoff (menu) ;
   menu->poz-- ;
   if (menu->maxw != 0)
   {
     if (menu->poz < 0)
     {
       if(menu->foff > 0)
       {
	 menu->foff-- ;

	 menu->poz = 0 ;  /*wykorzystane w draww_scroll*/
     show_mouse(NULL);
	 frame_off(menu);
	 draww_scroll (menu, FALSE) ;
	 frame_on(menu);
     show_mouse(screen);
       }
       menu->poz=0;
     }
  }
  baron(menu);
   if (BAR_POINTER) shift_cursor(menu);
  get_mouse_mickeys(&mickeyx, &mickeyy);

  return 0;
 }

static int  cpgnext(TMENU *menu)
{  if (menu==NULL) return 0;
	if (menu->flags & TIP)
	{
		show_hide_tip(menu, FALSE);
	}
 if (menu->maxw==0) return 0;
 baronoff(menu);
 if (menu->foff+menu->maxw < menu->max)
   {
    /* nastapi przerysowanie */
     menu->foff += menu->maxw;
     if (menu->foff + menu->maxw > menu->max)
       {
         menu->foff = menu->max - menu->maxw;
         menu->poz  = menu->maxw - 1;
       }
     draww(menu);
   }
 else
   {
	 menu->poz = menu->maxw - 1;
     if (menu->poz >= menu->max) menu->poz=menu->max?menu->max-1 : 0;
   }
 baron(menu);
 if (BAR_POINTER) shift_cursor(menu);
 return 0;
}

static int  cpgprev(TMENU *menu)
{    if (menu==NULL) return 0;
	if (menu->flags & TIP)
	{
		show_hide_tip(menu, FALSE);
	}
 if (menu->maxw==0) return 0;
 baronoff(menu);
 if (menu->foff == 0) menu->poz=0;
 else
   {
     menu->foff -= menu->maxw;
     if (menu->foff <0)
       {
         menu->foff = 0;
         menu->poz  = 0;
       }
	 draww(menu);
   }
 baron(menu);
 if (BAR_POINTER) shift_cursor(menu);
 return 0;
}

static int  cbegin(TMENU * menu)
{
    if (menu==NULL) return 0;

	if (menu->flags & TIP)
	{
		show_hide_tip(menu, FALSE);
	}
  baronoff(menu);
  menu->poz=0;
  if (menu->foff)
    {
      menu->foff=0;
      draww(menu);
    }
  baron(menu);
  if (BAR_POINTER) shift_cursor(menu);
  return 0;
}

static int  cend(TMENU * menu)
{
    if (menu==NULL) return 0;

	if (menu->flags & TIP)
	{
		show_hide_tip(menu, FALSE);
	}
  baronoff(menu);
  if (menu->maxw==0) menu->poz=menu->max - 1;
  else
    {
      menu->foff=menu->max - menu->maxw;
      if (menu->foff < 0)
         {
           menu->foff=0;
           menu->poz=menu->max-1;
         }
      else menu->poz=menu->maxw - 1;
      draww(menu);
    }
  baron(menu);
  if (BAR_POINTER) shift_cursor(menu);
  return 0;
}

static int opwin_s = 0 ;        //simple menu

/*opwin - zmienna wskazujaca czy przeprogr. tablica klawiszy f-cji*/
int opwin=0;
/*SW - tablica zawiera adresy sprzed przeprogramowania*/
static int ( *SW[134])(void);
static int ( *SW0[134])(void);

static int ( *SW01)(void);
int ( *SW2[16])(void);

/*tablica klawiszy funkcyjnych*/
/*SERV - przeprogramowanie tablicy klawiszy funkcyjnych*/
static void (*MSW)(int x,int y);
static int (*KLs)(void);
static int  nooop(void)
{return 0;}

static void nooop_(void)
{}

#define noop (int ( *)(void))nooop

static void redcr(char typ)
{ int n1;
 if (!opwin)
   {
     typokn=(typ=='\0');
	 skokm = (typokn ? SKOKM_VER : SKOKM_HOR);
	 
	 SW2[0]= SERV[4];   //Ctrl-D
	 SW2[1]= SERV[5];          //E
	 SW2[2]= SERV[14];      //N
	 SW2[3]= SERV[13];         //M
	 SW2[4]= SERV[9];     //I
	 SW2[5]= SERV[3];         //C
	 SW2[6]= SERV[16];     //P
	 SW2[7]= SERV[20];         //T
	 SW2[8]= SERV[2];      //A
	 SW2[9]= SERV[15];        //O
	 SW2[10]= SERV[24];    //X
	 SW2[11]= SERV[25];     //D
	 SW2[12]= SERV[12];    //L
    
    SERV [4] = noop;
    SERV [5] = noop;
    SERV [14] = noop;
    SERV [13] = noop;
    SERV [9] = noop;
    SERV [3] = noop;
    SERV [16] = noop;
    SERV [20] = noop;
    SERV [2] = noop;
    SERV [15] = noop;
	SERV [24] = noop;
	SERV [23] = noop;
	SERV [12] = noop;
	

	SW0[0]=SERV[71]; SERV[71]= (void *)cbegin;
	SW0[1]=SERV[72]; SERV[72]= (typokn?(void*)cprev:noop);
	SW0[2]=SERV[73]; SERV[73]= (void*)cpgprev;
	SW0[3]=SERV[75]; SERV[75]= (typokn?noop:(void*)cprev);
	SW0[4]=SERV[77]; SERV[77]= (typokn?noop:(void*)cnext);
	SW0[5]=SERV[79]; SERV[79]= (void*)cend;
	SW0[6]=SERV[80]; SERV[80]= (typokn?(void*)cnext:noop);
	SW0[7]=SERV[81]; SERV[81]= (void*)cpgnext;
	SW0[8]=SERV[82]; SERV[82]=/*noop;*/ (void*)kls;
    SW01=SERV[84];

	SERV[172] = (typokn ? (void*)cprev_min : noop);
	SERV[175] = (typokn ? noop : (void*)cprev_min);
	SERV[177] = (typokn ? noop : (void*)cnext_min);
	SERV[180] = (typokn ? (void*)cnext_min : noop);

         for (n1 = 0; n1 < SVMAX-84 ; n1++)
         {
			 if (((n1 + 84) != 172) && ((n1 + 84) != 175) && ((n1 + 84) != 177) & ((n1 + 84) != 180) && ((n1 + 84) != 117))
			 {
				 SW0[n1 + 9] = SERV[n1 + 84];
				 SERV[n1 + 84] = noop;
			 }
         }
         SW0[100] = SERV [47];
         SERV [47] = noop;

         SW0[101] = SERV [58];
         SW0[102] = SERV [59];
         SW0[103] = SERV [60];
         SW0[104] = SERV [61];
         SW0[105] = SERV [62];
         SW0[106] = SERV [63];
         SW0[107] = SERV [64];

         SERV [58] = noop;
         SERV [59] = noop;
         SERV [60] = noop;
         SERV [61] = noop;
         SERV [62] = noop;
         SERV [63] = noop;
         SERV [64] = noop;

		 SERV[135] = (void*)cnext_min_count;
		 SERV[134] = (void*)cprev_min_count;
		 mouse_dz = 1;

     SERV[84]=SW01;

     MSW=MSERV;
     if (!BAR_POINTER) MSERV=usk;
     else MSERV=pusk;
     KLs=KLS;        KLS = kls ;
     opwin=1;
   }
 else if(typokn!=(typ=='\0'))
   {
     typokn=(typ=='\0');
	 skokm = (typokn ? SKOKM_VER : SKOKM_HOR);
	 SERV[72]=(typokn? (void*)cprev:noop);
	 SERV[75]=(typokn?noop: (void*)cprev);
	 SERV[77]=(typokn?noop: (void*)cnext);
	 SERV[80]=(typokn? (void*)cnext:noop);

	 SERV[172] = (typokn ? (void*)cprev_min : noop);
	 SERV[175] = (typokn ? noop : (void*)cprev_min);
	 SERV[177] = (typokn ? noop : (void*)cnext_min);
	 SERV[180] = (typokn ? (void*)cnext_min : noop);

	 SERV[135] = (void*)cnext_min_count;
	 SERV[134] = (void*)cprev_min_count;
	 mouse_dz = 1;

	 KLS = kls ;
   }
}

/*zamkniecie okna menu*/
static void unredcr(void)
{ int n1;
  if(opwin)
     {
      SERV[71]=SW0[0];
      SERV[72]=SW0[1];
      SERV[73]=SW0[2];
      SERV[75]=SW0[3];
      SERV[77]=SW0[4];
      SERV[79]=SW0[5];
      SERV[80]=SW0[6];
      SERV[81]=SW0[7];
      SERV[82]=SW0[8];

      for(n1 = 0 ;n1 < SVMAX-84 ; n1++)
        {
          if ((n1 + 84) != 117)
              SERV [n1+84] = SW0[n1+9];
        }
        
      SERV [47] = SW0[100];

      SERV [58] = SW0[101];
      SERV [59] = SW0[102];
      SERV [60] = SW0[103];
      SERV [61] = SW0[104];
      SERV [62] = SW0[105];
      SERV [63] = SW0[106];
      SERV [64] = SW0[107];
	  
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
	  SERV[25] = SW2[11];     //D
	  SERV[12] = SW2[12];    //L

	  SERV[172] = noop;
	  SERV[175] = noop;
	  SERV[177] = noop;
	  SERV[180] = noop;

	  SERV[134] = (void*)SkalaZ_Minus; //nooop1;
	  SERV[135] = (void*)SkalaZ_Plus; //nooop1;
	  mouse_dz = 2;
      
      MSERV=MSW;
      KLS=KLs;
      opwin=0;
	 }
}

void unredcr_menu(void) {
    unredcr();
}

/*otworzenie okna menu*/

void  openmenu(TMENU *menu)
{
 register TMENU *rmenu;
 //przesuniecie okna do pozycji kursora
   if(!opwin)
     { rmenu=ACTW;
       ACTW=NULL;
       while(rmenu!=NULL)
	 {openw(rmenu);
	  rmenu=rmenu->next;
	 }
     }
   openw(menu);
   redcr(menu->flags &NVERT);
}

static char catch(unsigned char zn)
{
  if(mkbuf!=NULL)
	{ *mkbuf++=zn;
	  if(mkbuf==mbufmx)
		{ErrList(27);
		 mkbuf=NULL;
	
		}
	}
  return zn;
}

int ucatch(int zn)
{
	unsigned char b1, b2; // , b3, b4;

	
	if (mkbuf != NULL)
	{
		if (zn > 127)
		{
			b1 = (zn & 0xFF); //extract first byte
			b2 = ((zn >> 8) & 0xFF); //extract second byte
			*mkbuf++ = 255;
			
			*mkbuf++ = b2;
			*mkbuf++ = b1;
		}
		else
		{
			*mkbuf++ = zn;
		}
		if (mkbuf == mbufmx)
		{
			ErrList(27);
			mkbuf = NULL;
		}
	}
	return zn;
}

static void ctwsp___(int xy)
{ struct ZZ 
	{ char c1; char c2; char c3; char c4;};
  union AA
  { int n;
    struct ZZ zz;
  } XY;
  XY.n=xy;
  catch(XY.zz.c1);
  catch(XY.zz.c2);
  catch(XY.zz.c3);
  catch(XY.zz.c4);

}

static void ctwsp(int xy)
{
    catch((unsigned char)(xy >> (8 * 0)));
    catch((unsigned char)(xy >> (8 * 1)));
    catch((unsigned char)(xy >> (8 * 2)));
    catch((unsigned char)(xy >> (8 * 3)));
}

static void mkrcur(void)
{
   int x=*((int *)aktmakro);
   aktmakro+=4;
   int y=*((int *)aktmakro);
   aktmakro+=4;
   MVCUR(x,y);
   test_aktmakro();
}

static void mkrcur__(void)
{
    int x=*((int *)aktmakro++);
    int y=*((int *)aktmakro++);
    MVCUR(x,y);
    test_aktmakro();
}

#define UP  72
#define DWN 80
#define LT 75
#define RT 77

#define UP_MIN  172
#define DWN_MIN 180
#define LT_MIN 175
#define RT_MIN 177

static int sprawdzenie_ruchu(int msx, int msy)
/*------------------------------------------*/
{ time_t new_time;


  new_time = time (NULL);
  if (difftime (new_time, current_time) > 0.25)
    {
     current_time=time(NULL);
     suma_msx=suma_msy=suma_abs_msx=suma_abs_msy=0;
     return 0;
    }
  suma_msx += msx;
  suma_msy += msy;
  suma_abs_msx += abs(msx);
  suma_abs_msy += abs(msy);
  if ((suma_msx<7) && (suma_msy<7) &&
      (suma_abs_msx>140) && (suma_abs_msx<240) &&

      (suma_abs_msy<20))
   {
     current_time=time (NULL);
     suma_msx=suma_msy=suma_abs_msx=suma_abs_msy=0;
     return 1;
   }
  return 0;
}

void reset_bmp_back(void)
{
	bmp_back = NULL;
}

void DoneKeyimage(void)
{
	if (bmp_back != NULL) { destroy_bitmap(bmp_back); bmp_back=NULL;}
	if (demo_bmp_back != NULL) { destroy_bitmap(demo_bmp_back); demo_bmp_back=NULL; }
}

void DoneMickeyHand(void)
{
	if (mickey_hand_s != NULL) { destroy_bitmap(mickey_hand_s); mickey_hand_s = NULL; }
	if (mickey_hand != NULL) { destroy_bitmap(mickey_hand); mickey_hand = NULL; }
}

int Get_Dialog_Menu_Level(void)
{
	return (Get_Global_Dialog_Flag() + menu_level);
}

void set_mickey_hand(BITMAP *icon_mickey_s, BITMAP* icon_mickey)
{
	mickey_hand_s = create_bitmap_ex(32, 64, 64);
	if (mickey_hand_s != NULL)
	{
		clear_to_color(mickey_hand_s, bitmap_mask_color(mickey_hand_s));
		rotate_scaled_sprite(mickey_hand_s, icon_mickey_s, 0, 0, ftofix(0.0), ftofix(1.0));
	}

	mickey_hand = create_bitmap_ex(32, 96, 96);
	if (mickey_hand != NULL)
	{
		clear_to_color(mickey_hand, bitmap_mask_color(mickey_hand));
		rotate_scaled_sprite(mickey_hand, icon_mickey, 0, 0, ftofix(0.0), ftofix(1.0));
	}
}

void draw_mickey_hand(double x, double y)
{
	int x_h, y_h;

    set_alpha_blender();

	if (BIGCURSOR)
	{
		x_h = pikseleX(x) - 57;
		y_h = pikseleY(y) - 1;
		draw_trans_sprite(screen, mickey_hand, x_h, y_h);
	}
	else
	{
		x_h = pikseleX(x) - 38;
		y_h = pikseleY(y);
		draw_trans_sprite(screen, mickey_hand_s, x_h, y_h);
	}

	set_trans_blender(0, 0, 0, TRANSLUCENCY);
	set_mode_solid();
}

static void draw_keyimage(int keyimage, BOOL set_timer, BOOL silent, BOOL enforce)
{
	BITMAP *bmp, *buffer, *sec_screen;
	int x1, x2, y1, y2, x02, y02;
    int sc_w, sc_h;

    sec_screen=(BITMAP*)get_second_screen();
    sc_w=sec_screen->cr-sec_screen->cl;
    sc_h=sec_screen->cb-sec_screen->ct;

    if ((keyimage == shiftkeyimage_back) && (!enforce))
	{
		shiftkeytimestamp = time(NULL);
		return;
	}

	shiftkeyimage_back = keyimage;

    x1 = sec_screen->cl+2;
    y1 = sec_screen->cb - 2 - 32 * demo_scale;

    if (x1>sec_screen->cr) return;

	x2 = 32*demo_scale;  //+pocz_x;
	y2 = 32*demo_scale;  //+pocz_y;

    if (x2>sc_w - 2) x2=sc_w-2;
    if (x2<(32*demo_scale)) return;
    if (y2>sc_h - 2) y2=sc_h-2;
    if (y2<(32*demo_scale)) return;


	if (keyimage > -1)
	{
        if (do_not_draw_keys==FALSE)
        {
            bmp = (BITMAP *) get_icons_p(keyimage);

            if (bmp != NULL) {
                x02 = bmp->w;
                y02 = bmp->h;

                if (!Get_Dialog_Menu_Level() && !silent) CUR_OFF(X, Y);
                //back image image

                if (bmp_back != NULL) {
                    putimage2(x1, y1, bmp_back, COPY_PUT);
                    flip_corner();
                    bmp_back = NULL;
                }
                //getting image
                if (bmp_back == NULL) bmp_back = create_bitmap(x2, y2 + 1);
                if (bmp_back == NULL) {
                    if (!Get_Dialog_Menu_Level() && !silent) CUR_ON(X, Y);
                    return;
                }

                getimage2(x1, y1, x1 + x2, y1 + y2 + 1, bmp_back);

                //beep(MB_ABORTRETRYIGNORE);

                if (bmp->w == 32) {
                    blit(bmp, sec_screen, 0, 0, x1, y1, x2, y2);
                }
                else {
                    buffer = create_bitmap_ex(32, x2, y2 + 1);
                    if (buffer != NULL) {
                        clear_to_color(buffer, bitmap_mask_color(buffer));
                        rotate_scaled_sprite(buffer, bmp, 0, 0, ftofix(0.0), ftofix((double) x2 / (double) x02));
                        set_alpha_blender();
                        draw_trans_sprite(sec_screen, buffer, x1, y1);
                        destroy_bitmap(buffer);
                        set_trans_blender(0, 0, 0, TRANSLUCENCY);
                        set_mode_solid2();
                        set_mode_solid();
                    }
                }
                flip_corner();
                if (!Get_Dialog_Menu_Level() && !silent) CUR_ON(X, Y);

                if (set_timer) shiftkeytimestamp = time(NULL);

            }
        }
	}
	else
	{
		if (bmp_back != NULL)
		{
			if (!Get_Dialog_Menu_Level() && !silent) CUR_OFF(X, Y);
			putimage2(x1, y1, bmp_back, COPY_PUT);
            flip_corner();
			bmp_back = NULL;
			if (!Get_Dialog_Menu_Level() && !silent) CUR_ON(X, Y);
		}
		shiftkeyimage_back = 0;
	}
}

void draw_keyimage_(int keyimage, BOOL set_timer, BOOL silent, BOOL enforce)
{
	draw_keyimage(keyimage, set_timer, silent, enforce);
}

void del_demo_keys(void)
{
	if (demo_bmp_back != NULL)
	{
		destroy_bitmap(demo_bmp_back);
		demo_bmp_back = NULL;
	}
	if (demo_bmp_back1 != NULL)
	{
		destroy_bitmap(demo_bmp_back1);
		demo_bmp_back1 = NULL;
	}
}

static void draw_demokeyimage(int keyimage, int keyimage1, BOOL set_timer, BOOL silent, BOOL enforce)
{
	BITMAP* bmp, *buffer, *sec_screen;
	int x1, x2, y1, y2, x02, y02;
    int sc_w, sc_h;

    sec_screen=(BITMAP*)get_second_screen();
    sc_w=sec_screen->cr-sec_screen->cl;
    sc_h=sec_screen->cb-sec_screen->ct;

    if ((keyimage == keyimage_back) && (!enforce))
	{
		keytimestamp = time(NULL);
		return;
	}

	keyimage_back = keyimage;

	x1 = sec_screen->cl + 2 + 32 * demo_scale;
    y1 = sec_screen->cb - 2 - 32 * demo_scale;

    if (x1>sec_screen->cr) return;

	x2 = 32*demo_scale;  //+pocz_x;
	y2 = 32*demo_scale;  //+pocz_y;

    if (x2>sc_w - 2 - 32*demo_scale) x2=sc_w-2-32*demo_scale;
    if (x2<(32*demo_scale)) return;
    if (y2>sc_h - 2) y2=sc_h-2;
    if (y2<(32*demo_scale)) return;


	if (keyimage > -1)
    {
        if (do_not_draw_keys==FALSE)
        {
		if ((!Get_Dialog_Menu_Level()) || (strncmp(Get_Global_Dialog_Name(), _PRINTING__, 5) != 0))
            {
                bmp = (BITMAP *) get_icons_p(keyimage);

                if (bmp != NULL) {
                    x02 = bmp->w;
                    y02 = bmp->h;

                    if (!Get_Dialog_Menu_Level() && !silent) CUR_OFF(X, Y);
                    //back image image
                    if (demo_bmp_back != NULL) {
                        putimage2(x1, y1, demo_bmp_back, COPY_PUT);
                        flip_corner();
                        demo_bmp_back = NULL;
                    }
                    //getting image
                    if (demo_bmp_back == NULL) demo_bmp_back = create_bitmap(x2, y2 + 1);
                    if (demo_bmp_back == NULL) {
                        if (!Get_Dialog_Menu_Level() && !silent) CUR_ON(X, Y);
                        return;
                    }
                    getimage2(x1, y1, x1 + x2, y1 + y2 + 1, demo_bmp_back);

                    buffer = create_bitmap_ex(32, x2, y2 + 1);
                    if (buffer != NULL) {
                        clear_to_color(buffer, bitmap_mask_color(buffer));
                        rotate_scaled_sprite(buffer, bmp, 0, 0, ftofix(0.0), ftofix((double) x2 / (double) x02));
                        set_alpha_blender();
                        draw_trans_sprite(sec_screen, buffer, x1, y1);
                        destroy_bitmap(buffer);
                        set_trans_blender(0, 0, 0, TRANSLUCENCY);
                        set_mode_solid2();
                        set_mode_solid();
                        flip_corner();
                    }


                    if (!Get_Dialog_Menu_Level() && !silent) CUR_ON(X, Y);

                    if (set_timer) keytimestamp = time(NULL);
                }
            }
		}
		else
		{
			keyimage_back_back = 0;
			shiftkeyimage_back_back = 0;
		}
	}
	else
	{
		
		if (demo_bmp_back != NULL)
		{
			if (!Get_Dialog_Menu_Level() && !silent) CUR_OFF(X, Y);

			if ((!Get_Dialog_Menu_Level()) || (strncmp(Get_Global_Dialog_Name(), _PRINTING__, 5) != 0))
			{
				putimage2(x1, y1, demo_bmp_back, COPY_PUT);
				flip_corner();
			}
			else
			{
				putimage3(x1, y1, demo_bmp_back, COPY_PUT);
				flip_corner3();
			}
			demo_bmp_back = NULL;

			if (!Get_Dialog_Menu_Level() && !silent) CUR_ON(X, Y);
		}
		keyimage_back = 0;

		keyimage_back_back = 0;
		shiftkeyimage_back_back = 0;
	}

	x1 += 32 * demo_scale;

	if (keyimage1 > -1)
	{
		if ((!Get_Dialog_Menu_Level()) || (strncmp(Get_Global_Dialog_Name(), _PRINTING__, 5) != 0))
		{
			bmp = (BITMAP*)get_icons_p(keyimage1);

			if (bmp != NULL)
			{
				x02 = bmp->w;
				y02 = bmp->h;

				if (!Get_Dialog_Menu_Level() && !silent) CUR_OFF(X, Y);
				//back image image
				if (demo_bmp_back1 != NULL)
				{
					putimage2(x1, y1, demo_bmp_back1, COPY_PUT);
					flip_corner();
					demo_bmp_back1 = NULL;
				}
				//getting image
				if (demo_bmp_back1 == NULL) demo_bmp_back1 = create_bitmap(x2, y2 + 1);
				if (demo_bmp_back1 == NULL)
				{
					if (!Get_Dialog_Menu_Level() && !silent) CUR_ON(X, Y);
					return;
				}
				getimage2(x1, y1, x1 + x2, y1 + y2 + 1, demo_bmp_back1);

				buffer = create_bitmap_ex(32, x2, y2 + 1);
				if (buffer != NULL)
				{
					clear_to_color(buffer, bitmap_mask_color(buffer));
					rotate_scaled_sprite(buffer, bmp, 0, 0, ftofix(0.0), ftofix((double)x2 / (double)x02));
					set_alpha_blender();
					draw_trans_sprite(sec_screen, buffer, x1, y1);
					destroy_bitmap(buffer);
					set_trans_blender(0, 0, 0, TRANSLUCENCY);
					set_mode_solid2();
					set_mode_solid();
					flip_corner();
				}

				if (!Get_Dialog_Menu_Level() && !silent) CUR_ON(X, Y);
			}
		}
		else
		{
			keyimage_back_back = 0;
			shiftkeyimage_back_back = 0;
		}
	}
	else
	{
		if (demo_bmp_back1 != NULL)
		{
			if (!Get_Dialog_Menu_Level() && !silent) CUR_OFF(X, Y);

			if ((!Get_Dialog_Menu_Level()) || (strncmp(Get_Global_Dialog_Name(), _PRINTING__, 5) != 0))
			{
				putimage2(x1, y1, demo_bmp_back1, COPY_PUT);
				flip_corner();
			}
			else
			{
				putimage3(x1, y1, demo_bmp_back1, COPY_PUT);
				flip_corner3();
			}
			demo_bmp_back1 = NULL;

			if (!Get_Dialog_Menu_Level() && !silent) CUR_ON(X, Y);
		}
	}
}

void draw_demokeyimage_(int keyimage, int keyimage1, BOOL set_timer, BOOL silent, BOOL enforce)
{
	draw_demokeyimage(keyimage, keyimage1, set_timer, silent, enforce);
}

BOOL get_DEMO_RECORDING(void)
{
	return DEMO_RECORDING;
}

void record_keyimages(void)
{
	if ((DEMO_RECORDING) && (keyimage_back > 0)) keyimage_back_back = keyimage_back;
	if (shiftkeyimage_back > 0) shiftkeyimage_back_back = shiftkeyimage_back;
}

void remove_keyimages(BOOL silent)
{
	if ((DEMO_RECORDING) && (keyimage_back > 0)) draw_demokeyimage(-1, -1, FALSE, silent, TRUE);
	if (shiftkeyimage_back > 0) draw_keyimage(-1, FALSE, silent, TRUE);
}

void restore_keyimages(void)
{
    draw_keyimage(-1, FALSE, FALSE, TRUE);
	if ((DEMO_RECORDING) && (keyimage_back_back > 0))
	{
		if ((keyimage_back_back> 656) && (keyimage_back_back<661)) draw_demokeyimage(keyimage_back_back, -1, FALSE, FALSE, TRUE);
		else
		{
			if (keyimage_back_back == 391) draw_demokeyimage(keyimage_back_back, 392, FALSE, FALSE, TRUE);
			else if (keyimage_back_back == 547) draw_demokeyimage(keyimage_back_back, 548, FALSE, FALSE, TRUE);
			else if (keyimage_back_back == 651) draw_demokeyimage(keyimage_back_back, 652, FALSE, FALSE, TRUE);
			else draw_demokeyimage(keyimage_back_back, -1, FALSE, FALSE, TRUE);
		}
		keyimage_back_back = 0;
	}
	if (shiftkeyimage_back_back > 0)
	{
		draw_keyimage(shiftkeyimage_back_back, FALSE, FALSE, TRUE);
		shiftkeyimage_back_back = 0;
	}
}

char get_shifts(int key2)  //alt=1, ctrl=2, shift=4
{
	char ret = 0;
	if (altkey_) ret += 1;
	if (altgrkey_) ret += 2;
	if (ctrlkey_) ret += 4;
	if (shiftkey_) ret += 8;

	altkey_ = altgrkey_ = shiftkey_ = FALSE;
	if ((key2 < 82) || (key2 > 85))
	{
		ctrlkey_ = FALSE;
		//now icon has to be removed
		if ((!DEMO_RECORDING) && (ret)) draw_keyimage(-1, FALSE, FALSE, TRUE); //FALSE);
		else
		{
			shiftkeytimestamp = time(NULL);
			withdraw_keyimage_permit = TRUE;
		}
	}
	while (((key_shifts & KB_ALT_FLAG) || (key_shifts & KB_CTRL_FLAG) || (key_shifts & KB_SHIFT_FLAG)) && (!keypressed())) my_sleep(0); // Sleep(0);
	return ret;
}

void draw_keyimages(void)
{
	if (altkey_ || altgrkey_) draw_keyimage(354, TRUE, TRUE, TRUE); //FALSE);
	else if (shiftkey_) draw_keyimage(355, TRUE, TRUE, TRUE); //FALSE);
	else if (ctrlkey_) draw_keyimage(353, TRUE, TRUE, TRUE); //FALSE);
}

void draw_demokeyimages(void)
{
	if ((DEMO_RECORDING) && (keyimage_back_back > 0))
	{
		if ((keyimage_back_back > 656) && (keyimage_back_back < 661)) draw_demokeyimage(keyimage_back_back, -1, FALSE, FALSE, TRUE);
		else
		{
			if (keyimage_back_back == 391) draw_demokeyimage(keyimage_back_back, 392, FALSE, FALSE, TRUE);
			else if (keyimage_back_back == 547) draw_demokeyimage(keyimage_back_back, 548, FALSE, FALSE, TRUE);
			else if (keyimage_back_back == 651) draw_demokeyimage(keyimage_back_back, 652, FALSE, FALSE, TRUE);
			else draw_demokeyimage(keyimage_back_back, -1, FALSE, FALSE, TRUE);
		}
		keyimage_back_back = 0;
	}
}

void move_cursor(void)
{
	int msx0, msy0;
	Odczyt_licznikow();
	if ((msx != 0 || msy != 0) && MVCUR != NULL)
	{
		msx0 = msx; msy0 = msy;
		msx = 0; msy = 0;

		MVCUR(msx0, msy0);
	}
}
///*547*/   icon_back_p, icon_space_p,
///*549*/   icon_A_p, icon_B_p, icon_C_p, icon_D_p, icon_E_p, icon_F_p, icon_G_p, icon_H_p, icon_I_p, icon_J_p, icon_K_p, icon_L_p,
///*561*/   icon_M_p, icon_N_p, icon_O_p, icon_P_p, icon_Q_p, icon_R_p, icon_S_p, icon_T_p, icon_U_p, icon_V_p,
///*571*/	  icon_W_p, icon_X_p, icon_Y_p, icon_Z_p,
///                        A																								   Z
int keydemoimages[] = {-1,549,550,551,552,553,554,555,556,557,558,559,560,561,562,563,564,565,566,567,568,569,570,571,572,573,574,
364,365,366,367,368,369,370,371,372,373,  //KEY_0....KEY_9
364,365,366,367,368,369,370,371,372,373, //KEY_0_PAD....KEY_9_PAD
376,377,378,379,380,381,382,383,384,385,386,387, //KEY_F1...KEY_F12
358, //KEY_ESC
-1, //KEY_TILDE
375,//KEY_MINUS
-1, //KEY_EQUALS
547, // + 548  //KEY_BACKSPACE
363,//KEY_TAB
-1,//KEY_OPENBRACE
-1,//KEY_CLOSEBRACE
651,//KEY_ENTER
-1,//KEY_COLON 
-1,//KEY_QUOTE
-1,//KEY_BACKSLASH 
-1,//KEY_BACKSLASH2
-1,//KEY_COMMA
-1,//KEY_STOP
-1,//KEY_SLASH
391,//KEY_SPACE
359, //KEY_INSERT
650, //KEY_DEL
356, //KEY_HOME 
357, //KEY_END
361, //KEY_PGUP
360, //KEY_PGDN
654,//KEY_LEFT
653,//KEY_RIGHT
655,//KEY_UP
656,//KEY_DOWN
-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
//__allegro_KEY_DOWN 85
//__allegro_KEY_MAX  127
};

void clean_kbd() {
    lastk='\0';
    Odczyt_licznikow();
    msud=0;
    msud_min=0;
}

void show__if_DEMO_RECORDING(int newicon)
{
    if (DEMO_RECORDING)
    {
        draw_demokeyimage(newicon, -1, TRUE, FALSE, FALSE);
    }
}
//unsigned char 
int inkeys(TMENU *menu, BOOL search_ok)
{
  static int mkeys;
  register int lk;
  unsigned char zn;
  int msx0,msy0;
  int kk;
  int sclchg;
  int d_mouse_z;
  BOOL scrkk;
  int (*KeyFun)(TMENU *);
  double tip_d_time;
  int i;
  int keyimage = -1;
  int keydemo = -1;
  time_t new_time;
  PTMENU *menu_;
  char *txt;
  char txt_[2]=" ";
  char *ptr;
  static int a=0;

    if (menu!=NULL) {
        if (menu->flags & 0x100)
        {
            menu_ = menu;
            txt = &((*(menu_->pola))[0].txt[0]);
        }
        else txt = &((*(menu->pola))[0].txt[0]);
    }
    else txt=txt_;

  my_poll_keyboard();

  if (sleep_state == TRUE)
  {
      if (menu!=&manimation)
	     my_sleep(10);
  }

  if(aktmakro!=NULL)
	 { if(my_kbhit() && my_ugetch()==ESC) brkmakro();
   	else
	  { zn=*aktmakro++;
	   test_aktmakro();
       ////my_sleep(50);
	   return zn;
	  }
    }
   if((lk=lastk)!='\0')
   { lastk='\0';
       return ucatch(lk);
   }
   //else ucatch(lk);

#ifdef LINUX
    ////Odczyt_licznikow();
#endif
   msud='\0';
   msud_min = '\0';
   msx=0;msy=0;
   while (1)
   {
       if (Get_Shm_private_mflag()) return ESC;
       if (Get_Shm_mflag()) return ESC;

       Check_XNextEvent();

       if (sleep_state == TRUE)
	   {
           if (menu!=&manimation)
		       my_sleep(10);
	   }

	   if (DEMO_RECORDING)
	   {  
		   if (shiftkeyimage_back > 0)
		   {
			   if (withdraw_keyimage_permit)
			   {
				   new_time = time(NULL);
				   if (difftime(new_time, shiftkeytimestamp) > 2.0)
				   {
					   draw_keyimage(-1, FALSE, FALSE, FALSE);
					   withdraw_keyimage_permit = FALSE;
				   }
			   }
		   }
	   }

       if (key_shifts & (KB_CTRL_FLAG) && (key[KEY_END]))
           return 9998;
	   
	   if ((now_is_dialog==0) && ((menu == NULL) || (menu_level == 0)) && (key_shifts & (KB_ALT_FLAG | KB_SHIFT_FLAG | KB_CTRL_FLAG)) && (!keypressed()))
	   {
		   sleep_state = FALSE;
		   if (key[KEY_ALT])
		   {   
			   altkey_ = !altkey_;
			   shiftkey_ = FALSE;
			   ctrlkey_ = FALSE;
			   keyimage = (altkey_) ? 354 : -1;
			   draw_keyimage(keyimage, TRUE, FALSE, FALSE);
			   if (key_shifts)
				   //while ((key_shifts  & KB_ALT_FLAG) && (!keypressed()))
				   while ((key[KEY_ALT]) && (!keypressed()))
			   {
				   my_sleep(0);
				   my_poll_keyboard();
			   }
		   }
		   else if (key[KEY_ALTGR])
		   {
			   altgrkey_ = !altgrkey_;
			   shiftkey_ = FALSE;
			   ctrlkey_ = FALSE;
			   keyimage = (altgrkey_) ? 354 : -1;
			   draw_keyimage(keyimage, TRUE, FALSE, FALSE);
			   if (key_shifts)
				   while ((key[KEY_ALTGR]) && (!keypressed()))
			   {
				   my_sleep(0);
				   my_poll_keyboard();
			   }
		   }
		   else if (key[KEY_LSHIFT] || key[KEY_RSHIFT])
		   {
				   shiftkey_ = !shiftkey_;
				   altkey_ = FALSE;
				   ctrlkey_ = FALSE;
				   keyimage = (shiftkey_) ? 355 : -1;
				   draw_keyimage(keyimage, TRUE, FALSE, FALSE);
				   if (key_shifts)
					   while ((key[KEY_LSHIFT] || key[KEY_RSHIFT]) && (!keypressed()))
				   {
					   my_sleep(0);
					   my_poll_keyboard();
				   }

		   }
		   else if ((key[KEY_LCONTROL] || key[KEY_RCONTROL]))
		   {
			   ctrlkey_ = !ctrlkey_;
			   shiftkey_ = FALSE;
			   altkey_ = FALSE;
			   keyimage = (ctrlkey_) ? 353 : -1;
			   draw_keyimage(keyimage, TRUE, FALSE, FALSE);
			   if (key_shifts)
				   //while ((key_shifts & KB_CTRL_FLAG) && (key_shifts & !KB_SHIFT_FLAG) && (!keypressed()))
				   while ((key[KEY_LCONTROL] || key[KEY_RCONTROL]) && (!key[KEY_LSHIFT] && !key[KEY_RSHIFT] && !key[KEY_ALT])  && (!keypressed()))
			   {
				   my_sleep(0);
				   my_poll_keyboard();
			   }
			   else  my_poll_keyboard();
			   if ((ctrlkey_) && (key[KEY_LSHIFT] || key[KEY_RSHIFT]))
				{
					ctrlkey_ = FALSE;
					shiftkey_ = FALSE;
					keyimage = 677;
					draw_keyimage(keyimage, TRUE, FALSE, FALSE);
					if (key_shifts)
						while ((key[KEY_LCONTROL] || key[KEY_RCONTROL]) || (key[KEY_LSHIFT] || key[KEY_RSHIFT]) && (!keypressed()))
					{
						my_sleep(0);
						my_poll_keyboard();
					}
					return 9999;
				}
			   else if ((ctrlkey_) && (key[KEY_ALT]))
			   {
				   ctrlkey_ = FALSE;
				   altkey_ = FALSE;
				   keyimage = 677;
				   draw_keyimage(keyimage, TRUE, FALSE, FALSE);
				   if (key_shifts)
					   while ((key[KEY_LCONTROL] || key[KEY_RCONTROL]) || (key[KEY_ALT]) && (!keypressed()))
					   {
						   my_sleep(0);
						   my_poll_keyboard();
					   }
				   return 9999;
			   }
		   }
	   }

	   if (kbrdy())
	   { /*kasowanie czasu savera*/

		   set_scrsave_time();

		   lk = ucatch (getchp());
		   if (lk == '\0')
		   {
			   //lastk = ucatch (getchp());
               lastk = getchp();
		   }
		   kbflush();
           if (search_ok) {
               if ((lk==ENTER) || (lk==ESC))
               {
                   if ((menu_ == &mCzcionka) || (menu_ == &mCzcionkaZ) || (menu_ == &mCzcionkaW)) Set_HEIGHT_back();
                   komunikat_str_short("", FALSE, FALSE);
                   if ((menu_ == &mCzcionka) || (menu_ == &mCzcionkaZ) || (menu_ == &mCzcionkaW)) Set_HEIGHT_high();
               }
               else if (((lk > 0x1F) && (lk < 0x80)) || (lk == 8))
               {
                   if ((menu_ == &mCzcionka) || (menu_ == &mCzcionkaZ) || (menu_ == &mCzcionkaW)) Set_HEIGHT_back();
                   if (lk == 8) {
                       if (strlen(search_str) > 0) search_str[strlen(search_str) - 1] = '\0';
                       komunikat_str_short(search_str, FALSE, FALSE);
                   } else {
                       if (strlen(search_str) < MaxTextLen - 1) {
                           size_t len = strlen(search_str);
                           search_str[len++] = lk;
                           search_str[len] = '\0';
                           komunikat_str_short(search_str, FALSE, FALSE);
                       }
                   }
                   if ((menu_ == &mCzcionka) || (menu_ == &mCzcionkaZ) || (menu_ == &mCzcionkaW)) Set_HEIGHT_high();
                   //search in menu
                   for (i = 0; i < menu_->max; i++) {
                       txt = &((*(menu_->pola))[i].txt[0]);
#ifdef LINUX
                       ptr = strcasestr(txt, search_str);
#else
                       //ptr = StrStrIA(txt, search_str);
                   	   ptr = strcasestr(txt, search_str);
#endif
                       if (ptr != NULL) break;
                   }
                   if (i < menu_->max) {
                       if (menu_->max == menu_->maxw) {
                           menu_->foff = 0;
                           menu_->poz = i;
                       }
                       else if (i < menu_->maxw) {
                           menu_->foff = 0;
                           menu_->poz = i;
                       }
                       else {
                           if ((i > menu_->foff) && (i < (menu_->foff + menu_->maxw))) menu_->poz = i - menu_->foff;
                           else {
                               menu_->foff = min(i, menu_->max - menu_->maxw);
                               menu_->poz = i - menu_->foff;
                           }
                       }
                       closew(menu_);
                       openw(menu_);
                   }
                   lk = 1;
               }
           }

		   if (DEMO_RECORDING)
		   {
			   for (i = 1; i < 86; i++)
			   {
				   if (key[i])
				   {
					   keydemo = keydemoimages[i];
					   if (i== KEY_SPACE) draw_demokeyimage(keydemo, 392, TRUE, FALSE, FALSE);
					   else if (i == KEY_BACKSPACE) draw_demokeyimage(keydemo, 548, TRUE, FALSE, FALSE);
					   else if (i == KEY_ENTER) draw_demokeyimage(keydemo, 652, TRUE, FALSE, FALSE);
					   else draw_demokeyimage(keydemo, -1, TRUE, FALSE, FALSE);
					   break;
				   }
			   }
		   }
		   return lk;
	   }
	   else if (DEMO_RECORDING)
		   {
			   if (keyimage_back > 0)
			   {
				   new_time = time(NULL);
				   if (difftime(new_time, keytimestamp) > 2.0)
				   {
					   draw_demokeyimage(-1, -1, TRUE, FALSE, FALSE);
				   }
			   }
		   }

	   if (keys_int > 0)
	   {
		   keys = keys_int;
		   keys_int = 0;
	   }
	   else Test_przycisniec(&keys); 


	   if (keys > 0)
	   { /*kasowanie czasu savera*/

		   set_scrsave_time();

		   sleep_state=FALSE;

		   mkeys = keys; keys = 0;
           if (((mkeys) & '\01') &&  ((mkeys) & '\02'))
           {
               if (DEMO_RECORDING)
               {
                   draw_demokeyimage(819, -1, TRUE, FALSE, FALSE);
               }
               if (search_ok) komunikat_str_short("", FALSE, FALSE);
               return ucatch(ESC);
           }
		   if ((mkeys) & '\01')
		   {
			   if (DEMO_RECORDING)
			   {
				   draw_demokeyimage(658, -1, TRUE, FALSE, FALSE);
			   }
               if (search_ok) komunikat_str_short("", FALSE, FALSE);
			   return ucatch(ESC);
		   }
		   if ((mkeys) & '\02')
		   {
			   if (DEMO_RECORDING)
			   {
				   draw_demokeyimage(659, -1, TRUE, FALSE, FALSE);
			   }
               if (search_ok) komunikat_str_short("", FALSE, FALSE);
			   return ucatch('\02');
		   }
		   if ((mkeys) & '\04')
		   {
			   if (DEMO_RECORDING)
			   {
				   draw_demokeyimage(657, -1, TRUE, FALSE, FALSE);
			   }
               if (search_ok) komunikat_str_short("", FALSE, FALSE);
			   return ucatch(ENTER);
		   }
	   }

	   Odczyt_licznikow();

	   //odczytanie polozenia kolka myszy
	   d_mouse_z = mouse_z - last_mouse_z;
	   if (d_mouse_z > mouse_dz)
	   {
		   if (DEMO_RECORDING)
		   {
			   draw_demokeyimage(660, -1, TRUE, FALSE, FALSE);
		   }
		   last_mouse_z = mouse_z;
		   
		   KeyFun = (int(*)(TMENU *))SERV[134];
		   if (KeyFun == (void*)SkalaZ_Minus) sclchg = KeyFun((void*)d_mouse_z);  //MODIFIED!!!
		   else if (KeyFun == (void*)cprev_min_count) sclchg = cprev_min_count(menu, d_mouse_z*z_step);
           else KeyFun(menu);

	   }
	   else if (d_mouse_z < -mouse_dz)
	   {
		   
		   if (DEMO_RECORDING)
		   {
			   draw_demokeyimage(660, -1, TRUE, FALSE, FALSE);
		   }
		   last_mouse_z = mouse_z;
		   
		   KeyFun = (int(*)(TMENU *))SERV[135];
		   if (KeyFun == (void*)SkalaZ_Plus) sclchg = KeyFun((void*)-d_mouse_z);  //MODIFIED!!!
		   else if (KeyFun == (void*)cnext_min_count) sclchg = cnext_min_count(menu, -d_mouse_z*z_step);
           else KeyFun(menu);
	   }
	  
	   if ((msx != 0 || msy != 0) && MVCUR != NULL)
	   {

		   set_scrsave_time();

		   if ((menu != NULL) && (global_menu_flags != NOMENU) && (txt[0] == '@') && ((menu->flags & TIP)))
		   {
			    show_hide_tip(menu, FALSE);
		   }

		   mvcurb.mvc = 1;

		   msx0 = msx; msy0 = msy;
		   msx = 0; msy = 0;

		   MVCUR(msx0, msy0);

           if(mkbuf!=NULL)
           {
               if (!editing_text) {
                   ucatch('\01');
                   ctwsp(msx0);
                   ctwsp(msy0);
               }
           }
	   }

	   if (msud > 0) {

		   set_scrsave_time();

		   /*kasowanie czasu savera*/  lastk = typokn ? UP : RT; break;
	   }
	   else if (msud_min > 0) {
		   lastk = typokn ? UP_MIN : RT_MIN; break;
	   }
	   else if (msud < 0) {

		   set_scrsave_time();

		   /*kasowanie czasu savera*/lastk = typokn ? DWN : LT;break;
	   }
	   else if (msud_min < 0) {
		   lastk = typokn ? DWN_MIN : LT_MIN;break;
	   }

	   if (MVCUR != NULL)
	   {
	      scrkk = ScrSave_Proc(0);
		  if ((menu != NULL) && (global_menu_flags != NOMENU) && ((menu->back) || (txt[0] =='@')) && (!(menu->flags & TIP)))
		  {	 
			current_time = time(NULL) + 1;
			tip_d_time = difftime(current_time, get_scrsave_time());

			if ((tip_d_time >= tip_delay) || (menu == &mInfo) || (menu == &mInfoAbout) || (menu == &mInfoAboutA) || (menu == &mHelp))
			{
				menu->flags += TIP;
				if (txt[0] == '@')
				{
                    //the change due to everlapping images, so that one on top is taken first
					//checking if mpouse hovering over the image
					i=(menu->max)-1;
                    while (i>=0)
					{
						if ((PozX >= fortips_map.x1[i]+menu->xpcz) && (PozX <= fortips_map.x2[i]+menu->xpcz) &&
							(PozY >= fortips_map.y1[i]+menu->ypcz) && (PozY <= fortips_map.y2[i]+menu->ypcz))
						{
							menu->poz = i;
							show_hide_tip(menu, TRUE);
							break;
						}
						i--;
					}
				}
				else show_hide_tip(menu, TRUE);
			}
		  }
       }

       if (menu==&manimation)
           return 0;
    }
  return ucatch('\0');
}

void take_altkey_away(void) {
    if (altkey == TRUE) {
        altkey = FALSE;
        ignore_altkey = FALSE;
        draw_keyimage(-1, TRUE, TRUE, FALSE);
    }
}

void clear_keybuff(void)
{
	clear_keybuf();
	altkey = FALSE;
    ignore_altkey = TRUE;
    draw_keyimage(-1, TRUE, TRUE, FALSE);
}

int inukeys(TMENU *menu)
{
	static int mkeys;
	unsigned int lastk = '\0';
	unsigned int lastk1 = '\0';
	unsigned char zn, zn1;
	int scancode;
	int newkey;
	time_t new_time;
    int keyimage = -1;

    if (sleep_state == TRUE)
	{
		my_sleep(10);
	}

	if (DEMO_RECORDING)
	{
		if (shiftkeyimage_back > 0)
		{
			if (withdraw_keyimage_permit)
			{
				new_time = time(NULL);
				if (difftime(new_time, shiftkeytimestamp) > 2.0)
				{
					draw_keyimage(-1, FALSE, FALSE, FALSE);
					withdraw_keyimage_permit = FALSE;
				}
			}
		}
	}
	if (aktmakro != NULL)
	{
		if (my_kbhit() && my_ugetch() == ESC) brkmakro();
		else
		{
			zn = *aktmakro++;
			test_aktmakro();
			if (zn != 255)
			{
				return (int)zn;
			}
			else
			{
				zn  = *aktmakro++;
				zn1 = *aktmakro++;
				newkey = zn1 + (zn << 8);
				return newkey;
			}
		}
	}

#define MOUSEENTER 2132
#define MOUSEESC ESC  //27
#define LEFTKEY 2123 //331
#define INSKEY 2049 //257 
#define INSKEY1 2130 //338
#define RIGHTKEY 2125 //333
#define HOMEKEY 2119 //327
#define ENDKEY 2127 //335
#define PGDNKEY 2129 //337
#define DELKEY 2131 //339
#define F2 2108 //316
#define F9 2115
#define F10 2116
#define F11 2117
#define PASTECLIP 2070
#define COPYCLIP 2051
#define DEGREESIGN 176
#define SUPERSCRIPT4 0x2074
#define ROOTSIGN 0x221A
#define ROOT3SIGN 0x221B
#define EUROSIGN 0x20AC
#define SUPERSCRIPT3 179
#define SUPERSCRIPT2 178
#define PLUSMINUS 177
#define DIVISION 247
#define MIDDLEDOT 183
//#define PHISIGN 216
#define ONEHALF 0xBD
#define ONEQUARTER 0xBC
#define THREEQUARTERS 0xBE
#define DIAMETERSIGN 0xD8

#define ALPHASIGN 0x3B1
#define BETASIGN 0x3B2
#define GAMMASIGN 0x3B3
#define DELTASIGN 0x3B4
#define EPSILONSIGN 0x3B5
#define ZETASIGN 0x3B6
#define ETASIGN 0x3B7
#define THETASIGN 0x3B8
#define IOTASIGN 0x3B9
#define KAPPASIGN 0x3BA
#define LAMBDASIGN 0x3BB
#define MUSIGN 0x3BC
#define NUSIGN 0x3BD
#define XISIGN 0x3BE
#define OMICRONSIGN 0x3BF
#define PISIGN 0x3C0
#define RHOSIGN 0x3C1
#define FINALSIGMASIGN 0x3C2
#define SIGMASIGN 0x3C3
#define TAUSIGN 0x3C4
#define UPSYLONSIGN 0x3C5
#define PHISIGN 0x3C6
#define CHISIGN 0x3C7
#define PSISIGN 0x3C8
#define OMEGASIGN 0x3C9

#define CAPITALALPHASIGN 0x391
#define CAPITALBETASIGN 0x392
#define CAPITALGAMMASIGN 0x393
#define CAPITALDELTASIGN 0x394
#define CAPITALEPSILONSIGN 0x395
#define CAPITALZETASIGN 0x396
#define CAPITALETASIGN 0x397
#define CAPITALTHETASIGN 0x398
#define CAPITALIOTASIGN 0x399
#define CAPITALKAPPASIGN 0x39A
#define CAPITALLAMBDASIGN 0x39B
#define CAPITALMUSIGN 0x39C
#define CAPITALNUSIGN 0x39D
#define CAPITALXISIGN 0x39E
#define CAPITALOMICRONSIGN 0x39F
#define CAPITALPISIGN 0x3A0
#define CAPITALRHOSIGN 0x3A1
#define CAPITALSIGMASIGN 0x3A3
#define CAPITALTAUSIGN 0x3A4
#define CAPITALUPSYLONSIGN 0x3A5
#define CAPITALPHISIGN 0x3A6
#define CAPITALCHISIGN 0x3A7
#define CAPITALPSISIGN 0x3A8
#define CAPITALOMEGASIGN 0x3A9

#define PL_a 0x105
#define PL_A 0x104
#define PL_c 0x107
#define PL_C 0x106

    msud = '\0';
	msud_min = '\0';
	msx = 0; msy = 0;
	while (1)
	{
		if (sleep_state == TRUE)
		{
			my_sleep(10);
		}

		if ((key_shifts & KB_ALT_FLAG) && (!keypressed()))
		{
			if (key[KEY_ALT])
			{
				while ((key_shifts & KB_ALT_FLAG) && (!keypressed())) my_sleep(0); //Sleep(0);
                if (ignore_altkey==FALSE) altkey = !altkey;
                else altkey=FALSE;
                ignore_altkey=FALSE;

                keyimage = (altkey) ? 354 : -1;
                draw_keyimage(keyimage, TRUE, TRUE, FALSE);
			}
		}

		if (keypressed()) //(kbrdy())
		{ /*kasowanie czasu savera*/

			set_scrsave_time();

			newkey = ureadkey(&scancode);

			if (key[KEY_ALTGR])
			{
				altkey = FALSE;
			}

			if (key[KEY_LEFT]) 
			{
				clear_keybuf();  
				return ucatch(LEFTKEY);
			};

			if (key[KEY_RIGHT])
			{
				clear_keybuf();
				return ucatch(RIGHTKEY);
			};
			if (key[KEY_HOME]) {clear_keybuff();  return ucatch(HOMEKEY);}
			if (key[KEY_END]) {clear_keybuff(); return ucatch(ENDKEY);}

			if (key[KEY_PGDN]) { clear_keybuff(); return ucatch(PGDNKEY); }
			if (key[KEY_INSERT]) { clear_keybuff(); return ucatch(INSKEY1); }

			if (key[KEY_DEL]) { clear_keybuff(); return  ucatch(DELKEY); }
			if (key[KEY_F2]) { clear_keybuff();  return ucatch(F2); }
			if (key[KEY_F9]) { clear_keybuff();  return ucatch(F9); }
			if (key[KEY_F10]) { clear_keybuff();  return ucatch(F10); }
			if (key[KEY_F11]) { clear_keybuff();  return ucatch(F11); }

			if ((key_shifts & KB_CTRL_FLAG) && (key_shifts ^ KB_ALT_FLAG))
			{
				if ((key[KEY_LCONTROL] || key[KEY_RCONTROL]) && (!key[KEY_ALT]) && (!key[KEY_ALTGR]) && key[KEY_V] ) {
					clear_keybuff();
					return ucatch(PASTECLIP);
				}
				if ((key[KEY_LCONTROL] || key[KEY_RCONTROL]) && (!key[KEY_ALT]) && (!key[KEY_ALTGR]) && key[KEY_C]) {
					clear_keybuff();
					return ucatch(COPYCLIP);
				}
			}

			if ((key[KEY_ALT] || altkey) && key[KEY_8]) { clear_keybuff();  return ucatch(DEGREESIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_3]) { clear_keybuff();  return ucatch(SUPERSCRIPT3); }
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && ((key[KEY_ALT] || altkey) && key[KEY_4])) { clear_keybuff(); return ucatch(EUROSIGN); }
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && ((key[KEY_ALT] || altkey) && key[KEY_BACKSLASH])) { clear_keybuff(); return ucatch(ROOT3SIGN); }
            if ((key[KEY_ALT] || altkey) && key[KEY_4]) { clear_keybuff(); return ucatch(SUPERSCRIPT4); }
            if ((key[KEY_ALT] || altkey) && key[KEY_BACKSLASH]) { clear_keybuff(); return ucatch(ROOTSIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_2]) { clear_keybuff();  return ucatch(SUPERSCRIPT2); }
			if ((key[KEY_ALT] || altkey) && key[KEY_MINUS]) { clear_keybuff();  return ucatch(PLUSMINUS); }
			if ((key[KEY_ALT] || altkey) && key[KEY_SLASH]) { clear_keybuff();  return ucatch(DIVISION); }
			if ((key[KEY_ALT] || altkey) && key[KEY_STOP]) { clear_keybuff();  return ucatch(MIDDLEDOT); }
			if ((key[KEY_ALT] || altkey) && key[KEY_1]) { clear_keybuff();  return ucatch(ONEHALF); }
			if ((key[KEY_ALT] || altkey) && key[KEY_5]) { clear_keybuff();  return ucatch(ONEQUARTER); }
            if ((key[KEY_ALT] || altkey) && key[KEY_6]) { clear_keybuff();  return ucatch(THREEQUARTERS);}
			if ((key[KEY_ALT] || altkey) && key[KEY_0]) { clear_keybuff();  return ucatch(DIAMETERSIGN); }

            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_A]) { clear_keybuff();  return ucatch(CAPITALALPHASIGN); } ////
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_B]) { clear_keybuff();  return ucatch(CAPITALBETASIGN); }  ////
			if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_G]) { clear_keybuff();  return ucatch(CAPITALGAMMASIGN); }
			if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_D]) { clear_keybuff();  return ucatch(CAPITALDELTASIGN); }
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_E]) { clear_keybuff();  return ucatch(CAPITALEPSILONSIGN); }  ////
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_Z]) { clear_keybuff();  return ucatch(CAPITALZETASIGN); }   ////
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_Q]) { clear_keybuff();  return ucatch(CAPITALETASIGN); }   ////
			if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_H]) { clear_keybuff();  return ucatch(CAPITALTHETASIGN); }
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_I]) { clear_keybuff();  return ucatch(CAPITALIOTASIGN); }  ////
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_K]) { clear_keybuff();  return ucatch(CAPITALKAPPASIGN); }  ////
			if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_L]) { clear_keybuff();  return ucatch(CAPITALLAMBDASIGN); }
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_M]) { clear_keybuff();  return ucatch(CAPITALMUSIGN); }  ////
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_N]) { clear_keybuff();  return ucatch(CAPITALNUSIGN); }  ////
			if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_X]) { clear_keybuff();  return ucatch(CAPITALXISIGN); }
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_O]) { clear_keybuff();  return ucatch(CAPITALOMICRONSIGN); } ////
			if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_P]) { clear_keybuff();  return ucatch(CAPITALPISIGN); }
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_R]) { clear_keybuff();  return ucatch(CAPITALRHOSIGN); } ////
			if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_S]) { clear_keybuff();  return ucatch(CAPITALSIGMASIGN); }
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_T]) { clear_keybuff();  return ucatch(CAPITALTAUSIGN); } ////
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_Y]) { clear_keybuff();  return ucatch(CAPITALUPSYLONSIGN); }  ////
			if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_F]) { clear_keybuff();  return ucatch(CAPITALPHISIGN); }
            if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_C]) { clear_keybuff();  return ucatch(CAPITALCHISIGN); } ////
			if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_U]) { clear_keybuff();  return ucatch(CAPITALPSISIGN); }
			if ((key[KEY_LSHIFT] || (key_shifts & KB_CAPSLOCK_FLAG)) && (key[KEY_ALT] || altkey) && key[KEY_W]) { clear_keybuff();  return ucatch(CAPITALOMEGASIGN); }

			if ((key[KEY_ALT] || altkey) && key[KEY_A]) { clear_keybuff();
                return ucatch(ALPHASIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_B]) { clear_keybuff();  return ucatch(BETASIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_G]) { clear_keybuff();  return ucatch(GAMMASIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_D]) { clear_keybuff();  return ucatch(DELTASIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_E]) { clear_keybuff();  return ucatch(EPSILONSIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_Z]) { clear_keybuff();  return ucatch(ZETASIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_Q]) { clear_keybuff();  return ucatch(ETASIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_H]) { clear_keybuff();  return ucatch(THETASIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_I]) { clear_keybuff();  return ucatch(IOTASIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_K]) { clear_keybuff();  return ucatch(KAPPASIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_L]) { clear_keybuff();  return ucatch(LAMBDASIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_M]) { clear_keybuff();  return ucatch(MUSIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_N]) { clear_keybuff();  return ucatch(NUSIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_X]) { clear_keybuff();  return ucatch(XISIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_O]) { clear_keybuff();  return ucatch(OMICRONSIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_P]) { clear_keybuff();  return ucatch(PISIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_R]) { clear_keybuff();  return ucatch(RHOSIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_S]) { clear_keybuff();  return ucatch(SIGMASIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_T]) { clear_keybuff();  return ucatch(TAUSIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_Y]) { clear_keybuff();  return ucatch(UPSYLONSIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_F]) { clear_keybuff();  return ucatch(PHISIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_C]) { clear_keybuff();  return ucatch(CHISIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_U]) { clear_keybuff();  return ucatch(PSISIGN); }
			if ((key[KEY_ALT] || altkey) && key[KEY_W]) { clear_keybuff();  return ucatch(OMEGASIGN); }

			return ucatch(newkey);
			
		}

		if (!keys)
		{
			Test_przycisniec(&keys);
		}

		if (keys > 0)
		{ /*kasowanie czasu savera*/

			set_scrsave_time();
			sleep_state = FALSE;

			mkeys = keys; keys = 0;
			if ((mkeys)&'\01') return ucatch (MOUSEESC);
			if ((mkeys)&'\02') return ucatch ('\02');
			if ((mkeys)&'\04')  return ucatch (MOUSEENTER);  //ENTER
		}

		Odczyt_licznikow();

	}
	return ucatch ('\0');
}

char  readmouse(void)
{
  Test_przycisniec(&keys_int);
  if (keys_int > 0)
  {
	  mvcurb.mvc = 1;
	  return 0;
  }
  msx=0;msy=0;
  Odczyt_licznikow();
  if (msx!=0 || msy!=0) 
    { 
      if (labs (l_rX + msx) > l_krok_s ||
      labs (l_rY + msy) > l_krok_s)
       {
        mvcurb.mvc = 1 ;
       }
   
    }
  return 0;
}

char  readmouse_move(void)
{
	msx = 0; msy = 0;
	Odczyt_licznikow();
	if (msx != 0 || msy != 0)
	{
		if (labs(l_rX + msx) > l_krok_s ||
			labs(l_rY + msy) > l_krok_s)
		{
			mvcurb.mvc = 1;
		}

	}
	return 0;
}

 static int  commenu(TMENU *menu);
 static char ENTHME='\0';
#define SP 32
#define TAB 9


static EVENT Event;
/*funkcja wyboru z menu-wersja bez wyswietlania menu*/
int  getcom(TMENU *menu)
{  int n;
  int opp,m;
  TMENU *menu2;
  int zn;
  int (*KeyFun)(TMENU *);
  int x_cur, y_cur;
  int n_tools;
  int zn0;
  static TMENU *menua;
  static char st[POLE_TXT_MAX];
  unsigned short cod = L' ';
  static int iconno = 0;
  char *txt;
  char txt_[2]=" ";
  PTMENU *menu_;
  BOOL search_ok=FALSE;
  
  Event.What=evNothing;
 
  if (menu!=NULL && global_menu_flags != NOMENU && menu->back) redcr(menu->flags&NVERT);

  if (menu!=NULL) {
      if (menu->flags & 0x100)
      {
          menu_ = menu;
          txt = &((*(menu_->pola))[0].txt[0]);
          search_ok=TRUE;
          strcpy(search_str,"");
      }
      else
      {
          txt = &((*(menu->pola))[0].txt[0]);
      }
  }
  else txt=txt_;
  
  while (1)
   {

    zn = inkeys(menu, search_ok);

    if (search_ok)
    {
        if (zn=='A') {
            int a=0;
        }
    }
	
	sleep_state=FALSE;

	switch (zn)
	{
	case '\0': if ((zn = inkeys(NULL, FALSE)) >= 0 && zn < SVMAX)
		KeyFun = (int(*)(TMENU *))SERV[zn];
		if (KeyFun != NULL) 
			n = KeyFun(menu);
		else if (zn==1) {
            if (KLS == menupoz)
            {
                getmenupini(&menua, st, &cod, &iconno);
                n = -Simple_Menu_Proc_(menua);
                if (n)
                {
                    n = n + COMNDmnr - 1;
                    Event.What = evCommandP; break;
                }
                continue;
            }
            else break;
        }
        else n = 0;
		if (n == -83) break;     //funkcja usuwania ostatniej linii lub odcinka polilinii
		if (n == -84) break;     //funkcja usuwania ostatniej i jedynej linii i lub odcinka polilinii
		if (n == -183) break;     //funkcja zamkniecia trace lub 2-l 3-l
		if (n == -283)
		{
			Event.What = evCommandP;
			Event.Number = n = spline_CommandP_close;
			break;     //go_close_spline
		}
		if (n == -284)
		{
			Event.What = evCommandP;
			Event.Number = n = spline_CommandP_end;
			break;     //go_end_spline
		}
        if (n == -285)
        {
            Event.What = evCommandP;
            Event.Number = n = spline_CommandP_del;
            break;     //go_del_spline
        }
		if (n)
		{
			if ((zn == 82) || ((zn >= 84) && (zn <= 93))) Event.What = evCommandP;
			else Event.What = evCommandKF;
			break;
		}
		else continue;

	case 9999:
		Pan_Shift();
		n = 0;
		continue;
    case 9998:
        KeyFun = (int(*)(TMENU *))SERV[117];
        if (KeyFun != NULL)
            n = KeyFun(menu);
        n = 0;
        continue;
	case '\10':  //Backspace
		if ((menu_level == 0) && (Get_Global_Dialog_Flag() == 0))
		{
			KeyFun = (void*)(void(*)(TMENU*))LASTFUN;
			if (KeyFun != NULL)
            {
                if (Semaphore == TRUE) KeyFun(menu);
                else if (Cust_Semaphore == TRUE) {
                    Cust_Semaphore = FALSE;
                    KeyFun(menu);
                    Cust_Semaphore = TRUE;
                }
            }
			n = 0;
		}

		continue;
	case '\21':  //Ctrl-Q
        if ((menu_level == 0) && (Get_Global_Dialog_Flag() == 0))
        {
            KeyFun = (int (*)(TMENU *)) SERV[66];
            if (KeyFun != NULL) n = KeyFun(menu);
            else n = 0;
            if (n) {
                Event.What = evCommandP;
                break;
            } else continue;
        }
	case '\22':  //Ctrl-R
        if ((menu_level == 0) && (Get_Global_Dialog_Flag() == 0))
        {
            KeyFun = (int (*)(TMENU *)) Kopiuj_Ctrl_R;
            if (KeyFun != NULL) n = KeyFun(menu);
            else n = 0;
            n=Kopiuj_Ctrl_R();
            if (n) {
                Event.What = evCommandP;
                n = 0;
                break;
            } else {
                n = 0;
                continue;
            }
        }

    case '\27':  //Ctrl-W
            if ((menu_level == 0) && (Get_Global_Dialog_Flag() == 0))
            {
                KeyFun = (int (*)(TMENU *)) SERV[70];
                if (KeyFun != NULL) n = KeyFun(menu);
                else n = 0;
                if (n) {
                    Event.What = evCommandP;
                    n = 0;
                    break;
                } else {
                    n = 0;
                    continue;
                }
            }
	case '\1':if (aktmakro != NULL) mkrcur();
		continue;
	case SP:
	case '\2':
        if ((ctrlkey_) && (zn=='\2')) {
            ctrlkey_=FALSE;
            draw_keyimage(-1, TRUE, FALSE, FALSE);
            if (KLS == menupoz)
            {
                getmenupini(&menua, st, &cod, &iconno);
                n = -Simple_Menu_Proc_(menua);
                if (n)
                {
                    n = n + COMNDmnr - 1;
                    Event.What = evCommandP; break;
                }
                continue;
            }
            else break;
        }
        else {
            n = KLS();
            if (n) {
                Event.What = evCommandP;
                break;
            }
            continue;
        }
	case CTRL_SPC:
		if (KLS == menupoz)
		{
			getmenupini(&menua, st, &cod, &iconno);
			n = -Simple_Menu_Proc_(menua);
			if (n)
			{
				n = n + COMNDmnr - 1;
				Event.What = evCommandP; break;
			}
			continue;
		}
		else break;
	case ESC: n = 0;
		break;

	case Shift_F5:
		if ((opwin == 0) && (opwin_s == 0) && (0 != (n = osnap_())))
		{
			Event.What = evCommandP;
			break;
		}
		else continue;

            case TAB: if ((opwin == 0) && (opwin_s == 0) && (0 != (n = TABFUN())))
		break;
		continue;
	case ENTER:
		if (menu == NULL || global_menu_flags == NOMENU || (txt[0] == '@')) //(((*(menu->pola))[0].txt[0]) == '@'))
		{

			x_cur = get_cursor_posX();
			y_cur = get_cursor_posY();

            Event.What = evKeyDown;
            return zn;
		}
		test_aktmakro();
		if (opwin || aktmakro != NULL)
		{
            if (set_slider[menu_level-1])
            {
                if (find_menu_slider(menu, &slider[menu_level-1]))
                {
                    zn='\0';
                    n=-1;
                    continue;
                }
            }
			n = (menu->max ? menu->poz + menu->foff + 1 : 0);
            ////TEMPORARY
			////if (mkbuf != NULL && (zn = (*(menu->pola))[n - 1].wcod) != '\0')
			////	*(mkbuf - 1) = zn;
			break;
		}
		else
		{

			x_cur = get_cursor_posX();
			y_cur = get_cursor_posY();

            if (ENTHME || ACTW != NULL) { wyj = 0; openmenu(menu); } /* openw, redcr */
		}
		if (mkbuf != NULL) mkbuf--;
		continue;
	case '\60':  //0
	case '\61':  //1
	case '\62':
	case '\63':
	case '\64':
	case '\65':
	case '\66':
	case '\67':
	case '\70':
	case '\71':  //9
	case '\53':  //+
	case '\55':  //-
	case '\56':  //.
		if ((lock_functions == FALSE) && (opwin == 0) && (opwin_s == 0) && (0 != (n = TABFUN1(zn)))) break;
	case '\32':  //Ctrl-Z
	case '\30':  //Ctrl-X
		if (zn == '\32')
		{
			if ((lock_functions == FALSE) && (opwin == 0) && (opwin_s == 0) && (0 != (n = Skala_In_()))) break;
		}
		else { if ((lock_functions == FALSE) && (opwin == 0) && (opwin_s == 0) && (0 != (n = Skala_Out_()))) break; }
		
	case _EDIT_:
	case _edit_: 
	
		if ((edit_functions == TRUE) && (opwin == 0) && (opwin_s == 0) && (0 != (n = MEDITFUN())))
			break;
		else if (edit_functions == TRUE)
		{
			if (menu == NULL || global_menu_flags == NOMENU)
			{
				Event.What = evKeyDown;
				return zn;
			}
		}
#ifdef ENGLISH
#ifdef UKRAINIAN
	case _ENDPOINT_:
	case _endpoint_:
#endif
#else
	case _ENDPOINT_: 
	case _endpoint_:
#endif
	
#ifndef SPANISH
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			KeyFun = (int(*)(TMENU *))SERV[5];
			if (KeyFun != NULL) n = KeyFun(menu);
			else n = 0;
			if (n)
			{
				Event.What = evCommandP;
				break;
			}
			else continue;
		}
#else
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			KeyFun = (int(*)(TMENU*))SERV[14];
			if (KeyFun != NULL) n = KeyFun(menu);
			else n = 0;
			if (n)
			{
				Event.What = evCommandP;
				break;
			}
			else continue;
		}
#endif

	case _CTRL_ENDPOINT_:
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			Set_TrybS(0);
			n = 0;
			continue;
		}

#ifndef SPANISH
	case _NEAREST_: 
	case _nearest_: 

		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			KeyFun = (int(*)(TMENU *))SERV[14];
			if (KeyFun != NULL) n = KeyFun(menu);
			else n = 0;
			if (n)
			{
				Event.What = evCommandP;
				break;
			}
			else continue;
		}
#endif
	case _CTRL_NEAREST_:
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			Set_TrybS(1);
			n = 0;
			continue;
		}

	case _MIDDLE_:
	case _middle_:  

		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			KeyFun = (int(*)(TMENU *))SERV[13];
			if (KeyFun != NULL) n = KeyFun(menu);
			else n = 0;
			if (n)
			{
				Event.What = evCommandP;
				break;
			}
			else continue;
		}

	case _CTRL_MIDDLE_:
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			Set_TrybS(2);
			n = 0;
			continue;
		}
		
	case _INTERSECTION_: 
	case _intersection_:

		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			KeyFun = (int(*)(TMENU *))SERV[9];
			if (KeyFun != NULL) n = KeyFun(menu);
			else n = 0;
			if (n)
			{
				Event.What = evCommandP;
				break;
			}
			else continue;
		}

	case _CTRL_INTERSECTION_:
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			Set_TrybS(3);
			n = 0;
			continue;
		}
		
	case _CENTER_:
	case _center_:
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			KeyFun = (int(*)(TMENU *))SERV[3];
			if (KeyFun != NULL) n = KeyFun(menu);
			else n = 0;
			if (n)
			{
				Event.What = evCommandP;
				break;
			}
			else continue;
		}

	case _CTRL_CENTER_:
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			Set_TrybS(4);
			n = 0;
			continue;
		}
		
	case _PERPENDICULAR_:  //P
	case _perpendicular_:   //p

		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			KeyFun = (int(*)(TMENU *))SERV[16];
			if (KeyFun != NULL) n = KeyFun(menu);
			else n = 0;
			if (n)
			{
				Event.What = evCommandP;
				break;
			}
			else continue;
		}

	case _CTRL_PERPENDICULAR_:
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			Set_TrybS(5);
			n = 0;
			continue;
		}
		
	case _TANGENTIAL_:  //T
	case _tangential_:   //t
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			KeyFun = (int(*)(TMENU *))SERV[20];
			if (KeyFun != NULL) n = KeyFun(menu);
			else n = 0;
			if (n)
			{
				Event.What = evCommandP;
				break;
			}
			else continue;
		}

	case _CTRL_TANGENTIAL_:
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			Set_TrybS(6);
			n = 0;
			continue;
		}

	case _ADJACENT_:  
	case _adjacent_:   

		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			KeyFun = (int(*)(TMENU *))SERV[2];
			if (KeyFun != NULL) n = KeyFun(menu);
			else n = 0;
			if (n)
			{
				Event.What = evCommandP;
				break;
			}
			else continue;
		}

	case _CTRL_ADJACENT_:
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			Set_TrybS(7);
			n = 0;
			continue;
		}

	case _POINT_:  //O
	case _point_:   //o

		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			KeyFun = (int(*)(TMENU *))SERV[15];
			if (KeyFun != NULL) n = KeyFun(menu);
			else n = 0;
			if (n)
			{
				Event.What = evCommandP;
				break;
			}
			else continue;
		}

	case _CTRL_POINT_:
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			Set_TrybS(8);
			n = 0;
			continue;
		}
		
	case _CTRL_NOPE_: 
		if ((sel.akt && !sel.akw) && (menu == NULL))
		{
			Set_TrybS(9);
			n = 0;
			continue;
		}

	
	default:  if (menu == NULL || (txt[0] == '@') /*(((*(menu->pola))[0].txt[0]) == '@')*/ || global_menu_flags == NOMENU)
	{
		Event.What = evKeyDown;
		return zn;
	}

			  zn0 = zn;
#ifdef BIT64
			  zn = myCharUpperW(zn);
#else
			  zn = myCharUpperW(zn);
#endif
			  if (zn != zn0)
			  {
				  for (n = menu->max - 1; n >= 0; n--)
					  if ((*(menu->pola))[n].wcod == zn) break;
				  if (n < 0)
				  {
					  if (mkbuf != NULL) mkbuf--;
					  continue;
				  }
			  }
			  else
			  {
				  for (n = 0; n < menu->max; n++)
					  if ((*(menu->pola))[n].wcod == zn) break;
				  if (n >= menu->max)
				  {
					  if (mkbuf != NULL) mkbuf--;
					  continue;
				  }
			  }
		if(opwin)
		     baronoff(menu);
		if ((menu->maxw) && (n<menu->foff || n>=menu->foff+menu->maxw))
		   {
		     /* wybrana pozycja nie jest w okienku*/
		     menu->foff=n - menu->maxw + 1 ;
		     if (menu->foff<0) menu->foff=0;
		     menu->poz = n - menu->foff;
		     if (opwin) draww(menu);
		   }
		else menu->poz=n - menu->foff;
		if(opwin)
		     baron(menu);
		n++;
    } /* switch */
    if (menu!=NULL)
   {
   if (n<1000 && n>0 /*&&((menu->flags & FIXED)==0) && (menu2=(*(menu->pola))[n-1].menu)!=NULL*/)
     {
       if (menu->flags & FIXED)
       {
           PTMENU *menu_=menu;
           menu2=(*(menu_->pola))[n-1].menu;
       }
       else menu2=(*(menu->pola))[n-1].menu;
       if (menu2!=NULL)
       {
           if (menu->flags & TIP) {
               show_hide_tip(menu, FALSE);
           }
           if (!kbrdy() && opwin == 0) openmenu(menu);
           opp = opwin;
           m = commenu(menu2);
           if (opp)redcr(menu->flags & NVERT);
           if (!m) continue;
           n = m;
       }
     }
   }
   break;

  }
  /*okno nie zamykane gdy NOCLOSE lub wybrano pozycje i flaga NOWCLOSE*/

  if (menu!=NULL)
  {

   if (!((menu->flags & NOCLOSE) ||
	    (n && (menu->flags & NOWCLOSE)) ))
	      { 
 	         if(opwin) closew(menu);
	          else deactiv(menu);
	       }
  } 
  unredcr();
  if (!n)
  {
	  Event.What = evKeyDown;   /*ESC*/
  }
  else if (n > 0)
  {
	  if (menu != NULL)
	  {   
		  
		  if (menu->flags & TIP) { show_hide_tip(menu, FALSE); }
	      Event.What=evCommand; n=menu->off+n;
		 

	  }
  }
  return n;
}


/*funkcja wyboru z menu- wersja z wyswietl. menu*/
static int  commenu(TMENU *menu)
{
 if (menu->max==0) return 0; //empty menu

 if(!kbrdy() || opwin) openw(menu);
 else activate(menu);
 return getcom(menu);
}

/*funkcja wyboru wspolrzednych poprzez getcom*/
int  getwsp(TMENU *menu)
{ char flag,opwinr;
  int n;

   flag=ENTHME;
   opwinr=opwin;
   opwin=0;
   ENTHME='\1';
   n=getcom(menu);
   opwin=opwinr;
   ENTHME=flag;
   return n;
}

int getwsp3(TMENU *menu)
{ int n;
  while(1)
   { if((n=getwsp(menu))==0)  /* n=0 -> esc */
      if(Error) { ClearErr(); continue; }
      if(Info) { ClearInfo(); continue; }
      break;
   }
  return n;
}

static int getwsp1_(TMENU *menu)
{ char flag,opwinr;
  int n;
   flag=ENTHME;
   ENTHME='\1';
   opwinr=opwin;
   opwin=0;
   if (ACTW==NULL)  openmenu(menu);
   n=getcom(menu);
   ENTHME=flag;
   opwin=opwinr;
   return n;
}

int  getwsp1(TMENU *menu)
{
  int n;
   while(1)
    { if((n=getwsp1_(menu))==0)
      if(Error) { ClearErr(); continue; }
      if(Info) { ClearInfo(); continue; }
      break;
    }
   return n;
}


static int getwsp2_(TMENU *menu)
{ char flag,opwinr;
    int n;
    flag=ENTHME;
    ENTHME='\1';
    opwinr=opwin;
    opwin=0;
    if(ACTW==NULL) openmenu(menu);
    n=getcom(menu);
    ENTHME=flag;
    opwin=opwinr;
    return n;
}


void ch_layer (void)
{   int n;
    int i_layer;
    int object_type;
    char sk [128];
    //creating layers menu
    TMENU all_layers;
    POLE (*pola)[];
    char *adbp, *adbk;
    BOOL found;

    frame_off(&mInfoAboutA);

    memcpy(&all_layers, &mKolorAC, sizeof(TMENU));

    pola=malloc(sizeof(POLE)*No_Layers);

    all_layers.max=No_Layers;
    all_layers.maxw=(No_Layers<=16? No_Layers:16);
    all_layers.flags=0;
    all_layers.maxw0=(No_Layers<=16? No_Layers:16);
    all_layers.pola=pola;
    all_layers.xdl=32;

    for (int i = 0; i < No_Layers ; i++)
    {
        sprintf (sk, u8"%#ld", i + 1) ;
        strcat(sk,u8" '");
        strcat(sk,Layers[i].name);
        strcat(sk,u8"'");
        strncpy((*all_layers.pola)[i].txt, sk, 64 /*127*/);
        (*all_layers.pola)[i].wcod=' ';
        (*all_layers.pola)[i].iconno=0;
        (*all_layers.pola)[i].menu=NULL;
    }

    Test_Menu(&all_layers);

    i_layer = ((LINIA *) object_info_ad)->warstwa;

    if ((all_layers.maxw==0) || (all_layers.maxw>i_layer))
    {
        all_layers.foff=0;
        all_layers.poz=i_layer;
    }
    else
    {
        all_layers.foff=i_layer - all_layers.maxw + 1;
        all_layers.poz=all_layers.maxw-1;
    }

    menu_level++;
    menu_address[menu_level-1]=(char*)&all_layers;
    global_menu_flags=0;
    n = Simple_Menu_Proc(&all_layers);

    if (n)
    {
        object_type=((NAGLOWEK *)object_info_ad)->obiekt;

        i_layer=n-1;

        switch (object_type) {
            case OdBLOK:
                adbp = (char *)object_info_ad + sizeof(NAGLOWEK)+B3+((BLOK*)object_info_ad)->dlugosc_opisu_obiektu;
                adbk = (char *)object_info_ad + sizeof(NAGLOWEK) + ((BLOK*)object_info_ad)->n - 1;
                while (adbp<adbk)
                {

                    if ((((NAGLOWEK*)adbp)->atrybut!=Abad) && (((NAGLOWEK*)adbp)->obiekt!=OdBLOK)) {
                        rysuj_obiekt(adbp, COPY_PUT, 0);
                        ((LINIA *) adbp)->warstwa = i_layer;
                        rysuj_obiekt(adbp, COPY_PUT, 1);
                    }
                    adbp+=(sizeof(NAGLOWEK) + ((NAGLOWEK*)adbp)->n);
                }
                break;
            default:
                rysuj_obiekt(object_info_ad, COPY_PUT,0);
                ((LINIA *) object_info_ad)->warstwa = i_layer;
                rysuj_obiekt(object_info_ad, COPY_PUT,1);
                break;
        }

        sprintf (sk, u8"%#ld", ((LINIA *)object_info_ad)->warstwa + 1  ) ;
        strcat(sk,u8" '");
        strcat(sk,Layers[((LINIA *)object_info_ad)->warstwa].name);
        strcat(sk,u8"'");
        menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk);

        Change = TRUE;

        if ((all_layers.maxw==0) || (all_layers.maxw>i_layer))
        {
            all_layers.foff=0;
            all_layers.poz=i_layer;
        }
                else
        {
            all_layers.foff=i_layer - all_layers.maxw + 1;
            all_layers.poz=all_layers.maxw-1;
        }

        if ((object_type==Otekst) && (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2)) TTF_redraw=TRUE;
    }

    free(pola);
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

#define YP  4
#define XP  6
#define MDY 4
#define GR 4

void ch_color (void)
{
    int n;
    int object_type;
    int i_color, i_color0;
    int LiniaG_kolor;
    static char sk[64];
    float ytt;
    int xt,yt;
    int x1,y1,x2;
    int kolor_m, kolor_rgb;
    float xr;
    int size;
    unsigned short ptrsz_tcod;
    char *adbp, *adbk;
    BOOL found;

    frame_off(&mInfoAboutA);
    baronoff_(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;

    switch (object_type) {
        case Olinia:
            i_color0 = ((LINIA *) object_info_ad)->kolor;
            break;
        case Ookrag:
        case Okolo:
            i_color0 = ((OKRAG *) object_info_ad)->kolor;
            break;
        case Oellipse:
        case Ofilledellipse:
            i_color0 = ((ELLIPSE *) object_info_ad)->kolor;
            break;
        case Oluk:
            i_color0 = ((LUK *) object_info_ad)->kolor;
            break;
        case Oellipticalarc:
            i_color0 = ((ELLIPTICALARC *) object_info_ad)->kolor;
            break;
        case Ospline:
            i_color0 = ((SPLINE *) object_info_ad)->kolor;
            break;
        case Otekst:
            i_color0 = ((TEXT *) object_info_ad)->kolor;
            break;
        case Owwielokat:
            i_color0 = ((WIELOKAT *) object_info_ad)->kolor;
            break;
        case Osolidarc:
            i_color0 = ((SOLIDARC *) object_info_ad)->kolor;
            break;
        case Opoint:
            i_color0 = ((T_Point *) object_info_ad)->kolor;
            break;
        case Ovector:
            i_color0 = ((AVECTOR *) object_info_ad)->kolor;
            break;
        case OdBLOK:
            adbp = (char *)object_info_ad + sizeof(NAGLOWEK)+B3+((BLOK*)object_info_ad)->dlugosc_opisu_obiektu;
            adbk = (char *)object_info_ad + sizeof(NAGLOWEK) + ((BLOK*)object_info_ad)->n - 1;
            found=FALSE;
            while (adbp<adbk)
            {
                if (((NAGLOWEK*)adbp)->atrybut!=Abad) {
                    switch (((NAGLOWEK *) adbp)->obiekt) {
                        case Olinia:
                            i_color0 = ((LINIA *) adbp)->kolor;
                            found = TRUE;
                            break;
                        case Oluk:
                            i_color0 = ((LUK *) adbp)->kolor;
                            found = TRUE;
                            break;
                        case Oellipticalarc:
                            i_color0 = ((ELLIPTICALARC *) adbp)->kolor;
                            found = TRUE;
                            break;
                        case Ookrag:
                            i_color0 = ((LUK *) adbp)->kolor;
                            found = TRUE;
                            break;
                        case Oellipse:
                        case Ofilledellipse:
                            i_color0 = ((ELLIPSE *) adbp)->kolor;
                            found = TRUE;
                            break;
                        case Owwielokat:
                            i_color0 = ((WIELOKAT *) adbp)->kolor;
                            found = TRUE;
                            break;
                        case Osolidarc:
                            i_color0 = ((SOLIDARC *) adbp)->kolor;
                            found = TRUE;
                            break;
                        case Ospline:
                            i_color0 = ((SPLINE *) adbp)->kolor;
                            found = TRUE;
                            break;
                        default:
                            break;
                    }
                    if (found == TRUE) break;
                }
                adbp+=sizeof(NAGLOWEK) + ((NAGLOWEK*)adbp)->n;
            }
            break;
        default:
            baronoff(&mInfoAboutA);
            return;
            break;
    }

    if (i_color0>16) i_color0=16;

    Test_Menu (&mKolorAC);

    if ((mKolorAC.maxw==0) || (mKolorAC.maxw>i_color0))
    {
        mKolorAC.foff=0;
        mKolorAC.poz=i_color0;
    }
    else
    {
        mKolorAC.foff=i_color0 - mKolorAC.maxw + 1;
        mKolorAC.poz=mKolorAC.maxw-1;
    }

    menu_level++;
    menu_address[menu_level-1]=&mKolorAC;
    n = Simple_Menu_Proc(&mKolorAC);

    if (n)
    {
        i_color=n-1;
        if (i_color==16) {
            LiniaG_kolor=LiniaG.kolor;
            LiniaG.kolor=i_color0;
            i_color = colors() - 1;
            LiniaG.kolor=LiniaG_kolor;
        }
        if (i_color>=0) {

            rysuj_obiekt(object_info_ad, COPY_PUT, 0);
            switch (object_type) {
                case Olinia:
                    ((LINIA *) object_info_ad)->kolor = Menu_No_to_Color_No(i_color);
                    break;
                case Ookrag:
                case Okolo:
                    ((OKRAG *) object_info_ad)->kolor = Menu_No_to_Color_No(i_color);
                    break;
                case Oellipse:
                case Ofilledellipse:
                    ((ELLIPSE *) object_info_ad)->kolor = Menu_No_to_Color_No(i_color);
                    break;
                case Oluk:
                    ((LUK *) object_info_ad)->kolor = Menu_No_to_Color_No(i_color);
                    break;
                case Oellipticalarc:
                    ((ELLIPTICALARC *) object_info_ad)->kolor = Menu_No_to_Color_No(i_color);
                    break;
                case Ospline:
                    ((SPLINE *) object_info_ad)->kolor = Menu_No_to_Color_No(i_color);
                    break;
                case Otekst:
                    ((TEXT *) object_info_ad)->kolor = Menu_No_to_Color_No(i_color);
                    if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
                    break;
                case Owwielokat:
                    ((WIELOKAT *) object_info_ad)->kolor = Menu_No_to_Color_No(i_color);
                    break;
                case Osolidarc:
                    ((SOLIDARC *) object_info_ad)->kolor = Menu_No_to_Color_No(i_color);
                    break;
                case Opoint:
                    ((T_Point *) object_info_ad)->kolor = Menu_No_to_Color_No(i_color);
                    break;
                case Ovector:
                    ((AVECTOR *) object_info_ad)->kolor = Menu_No_to_Color_No(i_color);
                    break;
                case OdBLOK:
                    adbp = (char *)object_info_ad + sizeof(NAGLOWEK)+B3+((BLOK*)object_info_ad)->dlugosc_opisu_obiektu;
                    adbk = (char *)object_info_ad + sizeof(NAGLOWEK) + ((BLOK*)object_info_ad)->n - 1;
                    while (adbp<adbk)
                    {
                        if (((NAGLOWEK*)adbp)->atrybut!=Abad) {
                            switch (((NAGLOWEK *) adbp)->obiekt) {
                                case Olinia:
                                    rysuj_obiekt(adbp, COPY_PUT, 0);
                                    ((LINIA *) adbp)->kolor = Menu_No_to_Color_No(i_color);
                                    rysuj_obiekt(adbp, COPY_PUT, 1);
                                    break;
                                case Oluk:
                                    rysuj_obiekt(adbp, COPY_PUT, 0);
                                    ((LUK *) adbp)->kolor = Menu_No_to_Color_No(i_color);
                                    rysuj_obiekt(adbp, COPY_PUT, 1);
                                    break;
                                case Ookrag:
                                    rysuj_obiekt(adbp, COPY_PUT, 0);
                                    ((OKRAG *) adbp)->kolor = Menu_No_to_Color_No(i_color);
                                    rysuj_obiekt(adbp, COPY_PUT, 1);
                                    break;
                                case Oellipse:
                                case Ofilledellipse:
                                    rysuj_obiekt(adbp, COPY_PUT, 0);
                                    ((ELLIPSE *) adbp)->kolor = Menu_No_to_Color_No(i_color);
                                    rysuj_obiekt(adbp, COPY_PUT, 1);
                                    break;
                                case Owwielokat:
                                    rysuj_obiekt(adbp, COPY_PUT, 0);
                                    ((WIELOKAT *) adbp)->kolor = Menu_No_to_Color_No(i_color);
                                    rysuj_obiekt(adbp, COPY_PUT, 1);
                                    break;
                                case Osolidarc:
                                    rysuj_obiekt(adbp, COPY_PUT, 0);
                                    ((SOLIDARC *) adbp)->kolor = Menu_No_to_Color_No(i_color);
                                    rysuj_obiekt(adbp, COPY_PUT, 1);
                                    break;
                                case Ospline:
                                    rysuj_obiekt(adbp, COPY_PUT, 0);
                                    ((SPLINE *) adbp)->kolor = Menu_No_to_Color_No(i_color);
                                    rysuj_obiekt(adbp, COPY_PUT, 1);
                                    break;
                                default:
                                    break;
                            }
                        }
                        adbp+=(sizeof(NAGLOWEK) + ((NAGLOWEK*)adbp)->n);
                    }
                    break;
                default:
                    break;
            }

            rysuj_obiekt(object_info_ad, COPY_PUT, 1);

            nazwa_koloru(object_info_ad, &sk);
            LiniaG.kolor=Menu_No_to_Color_No(i_color);
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk);

            kolor_m=GetColorAC1(LiniaG.kolor);
            setfillstyle_(SOLID_FILL,kolor_m);

            size=mInfoAboutA.maxw?mInfoAboutA.maxw:mInfoAboutA.max;

            x1=(mInfoAboutA.xpcz-1) * WIDTH * SKALA + XP;
            y1=(mInfoAboutA.ypcz-1)*32 + YP;

            ytt = /*y1 + GR*/ - 2 * SKALA + MDY + (2 * 32);
            yt=(int) ytt;

            bar(x1 + 32, y1 + yt, x1 + 64, y1 + yt + 32);

            if (kolor_m > 16)
            {
                kolor_rgb = _dac_normal[kolor_m][0] + _dac_normal[kolor_m][1] + _dac_normal[kolor_m][2];
                if (kolor_rgb < 96) setcolor(15); else setcolor(0);
            }
            else if (kolor_m == 0) setcolor(15);
            else setcolor(0);

            int bytes_n;
            uint8_t utf8c[4];
            char sz_temp [TEMPN];
            int b;

            ptrsz_tcod = (*(mInfoAboutA.pola))[2].wcod ;
            bytes_n = ucs2_to_utf8(ptrsz_tcod, (uint8_t*)utf8c);
            if ((ptrsz_tcod < 1920) && (bytes_n < 3))
            {
                strncpy(sz_temp, utf8c, 2);
                sz_temp[2] = '\0';
            }
            else
            {
                strcpy(sz_temp, " ");
            }

            b = (int)(-HEIGHT*0.357 + 16);
            moveto(x1 + 36, y1 + yt + b);
            outtext_r_(screen, sz_temp);

            baronoff(&mInfoAboutA);

            Change = TRUE;

        }
    }
    else baronoff(&mInfoAboutA);

    return;
}

void ch_lwidth (void)
{
    int n;
    int object_type;
    int i_width, i_width0, i_typ;
    static char sk[64];
    char *adbp, *adbk;
    BOOL found;
    BOOL Wielokat=FALSE;

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;

    switch (object_type)
    {
        case Olinia:
            if (((LINIA *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                i_width0 = ((LINIA *)object_info_ad)->typ ;
            else return;
            break;
        case Ookrag:
        case Okolo:
            if (((OKRAG *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                i_width0 = ((OKRAG *)object_info_ad)->typ;
            else return;
            break;
        case Oellipse:
        case Ofilledellipse:
            if (((ELLIPSE *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                i_width0 = ((ELLIPSE *)object_info_ad)->typ;
            else return;
            break;
        case Oluk:
            if (((LUK *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                i_width0 = ((LUK *)object_info_ad)->typ;
            else return;
            break;
        case Oellipticalarc:
            if (((ELLIPTICALARC *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                i_width0 = ((ELLIPTICALARC *)object_info_ad)->typ;
            else return;
            break;
        case Ospline:
            if (((SPLINE *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                i_width0 = ((SPLINE *)object_info_ad)->typ;
            else return;
            break;
        case Owwielokat:
                i_width0 = ((WIELOKAT *)object_info_ad)->empty_typ;
            break;
        case Osolidarc:
            i_width0 = ((SOLIDARC *)object_info_ad)->empty_typ;
            break;
        case Ovector:
            i_width0 = ((AVECTOR *)object_info_ad)->typ ;
            break;
        case OdBLOK:
            adbp = (char *)object_info_ad + sizeof(NAGLOWEK)+B3+((BLOK*)object_info_ad)->dlugosc_opisu_obiektu;
            adbk = (char *)object_info_ad + sizeof(NAGLOWEK) + ((BLOK*)object_info_ad)->n - 1;
            found=FALSE;
            while (adbp<adbk)
            {
                if (((NAGLOWEK*)adbp)->atrybut!=Abad) {
                    switch (((NAGLOWEK *) adbp)->obiekt) {
                        case Olinia:
                            if (((LINIA *) adbp)->typ !=
                                HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                            {
                                i_width0 = ((LINIA *) adbp)->typ;
                                found = TRUE;
                            }
                            break;
                        case Oluk:
                            if (((LUK *) adbp)->typ !=
                                HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                            {
                                i_width0 = ((LUK *) adbp)->typ;
                                found = TRUE;
                            }
                        case Oellipticalarc:
                            if (((ELLIPTICALARC *) adbp)->typ !=
                                HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                            {
                                i_width0 = ((ELLIPTICALARC *) adbp)->typ;
                                found = TRUE;
                            }
                        case Owwielokat:
                            i_width0 = ((WIELOKAT *) adbp)->empty_typ;
                            Wielokat = TRUE;
                            found = TRUE;
                            break;
                        case Osolidarc:
                            i_width0 = ((SOLIDARC *) adbp)->empty_typ;
                            Wielokat = TRUE;
                            found = TRUE;
                            break;
                        case Ospline:
                            i_width0 = ((SPLINE *) adbp)->typ;
                            found = TRUE;
                            break;
                        default:
                            break;
                    }
                    if (found == TRUE) break;
                }
                adbp+=sizeof(NAGLOWEK) + ((NAGLOWEK*)adbp)->n;
            }
            break;
        default:
            break;
    }

    frame_off(&mInfoAboutA);

    if ((object_type==Owwielokat) || (object_type==Osolidarc) || (Wielokat==TRUE))
    {
        mGruboscLinii.poz = i_width0;

        menu_level++;
        menu_address[menu_level-1]=&mGruboscLiniiS;
        n = Simple_Menu_Proc(&mGruboscLiniiS);
    }
    else {
        i_typ = (i_width0 & 31);
        i_width = (i_width0 & 224) / 32;
        mGruboscLinii.poz = i_width;

        menu_level++;
        menu_address[menu_level-1]=&mGruboscLinii;
        n = Simple_Menu_Proc(&mGruboscLinii);
    }

    if (n)
    {
        i_width=n-1;
        switch (object_type)
        {
            case Olinia:
                if (((LINIA *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                {
                    rysuj_obiekt(object_info_ad, COPY_PUT, 0);
                    ((LINIA *) object_info_ad)->typ = Set_WidthL_to_Type(((LINIA *) object_info_ad)->typ, i_width);
                    rysuj_obiekt(object_info_ad, COPY_PUT,1);
                }
                break;
            case Ookrag:
            case Okolo:
                if (((OKRAG *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                {
                    rysuj_obiekt(object_info_ad, COPY_PUT,0);
                    ((OKRAG *) object_info_ad)->typ = Set_WidthL_to_Type(((OKRAG *) object_info_ad)->typ, i_width);
                    rysuj_obiekt(object_info_ad, COPY_PUT,1);
                }
                break;
            case Oellipse:
            case Ofilledellipse:
                if (((ELLIPSE *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                {
                    rysuj_obiekt(object_info_ad, COPY_PUT,0);
                    ((ELLIPSE *) object_info_ad)->typ = Set_WidthL_to_Type(((ELLIPSE *) object_info_ad)->typ, i_width);
                    rysuj_obiekt(object_info_ad, COPY_PUT,1);
                }
                break;
            case Oluk:
                if (((LUK *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                {
                    rysuj_obiekt(object_info_ad, COPY_PUT,0);
                    ((LUK *) object_info_ad)->typ = Set_WidthL_to_Type(((LUK *) object_info_ad)->typ, i_width);
                    rysuj_obiekt(object_info_ad, COPY_PUT,1);
                }
                break;
            case Oellipticalarc:
                if (((ELLIPTICALARC *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                {
                    rysuj_obiekt(object_info_ad, COPY_PUT,0);
                    ((ELLIPTICALARC *) object_info_ad)->typ = Set_WidthL_to_Type(((ELLIPTICALARC *) object_info_ad)->typ, i_width);
                    rysuj_obiekt(object_info_ad, COPY_PUT,1);
                }
                break;
            case Ospline:
                if (((SPLINE *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                {
                    rysuj_obiekt(object_info_ad, COPY_PUT,0);
                    ((SPLINE *) object_info_ad)->typ = Set_WidthL_to_Type(((SPLINE *) object_info_ad)->typ, i_width);
                    rysuj_obiekt(object_info_ad, COPY_PUT,1);
                }
                break;
            case Owwielokat:
                    rysuj_obiekt(object_info_ad, COPY_PUT,0);
                    ((WIELOKAT *) object_info_ad)->empty_typ = i_width;
                    rysuj_obiekt(object_info_ad, COPY_PUT,1);
                break;
            case Osolidarc:
                rysuj_obiekt(object_info_ad, COPY_PUT,0);
                ((SOLIDARC *) object_info_ad)->empty_typ = i_width;
                rysuj_obiekt(object_info_ad, COPY_PUT,1);
                break;
            case Ovector:
                rysuj_obiekt(object_info_ad, COPY_PUT, 0);
                ((AVECTOR *) object_info_ad)->typ = Set_WidthL_to_Type(((AVECTOR *) object_info_ad)->typ, i_width);
                rysuj_obiekt(object_info_ad, COPY_PUT,1);
                break;
            case OdBLOK:
                adbp = (char *)object_info_ad + sizeof(NAGLOWEK)+B3+((BLOK*)object_info_ad)->dlugosc_opisu_obiektu;
                adbk = (char *)object_info_ad + sizeof(NAGLOWEK) + ((BLOK*)object_info_ad)->n - 1;
                while (adbp<adbk)
                {
                    if (((NAGLOWEK*)adbp)->atrybut!=Abad) {
                        switch (((NAGLOWEK *) adbp)->obiekt) {
                            case Olinia:

                                if (((LINIA *) adbp)->typ !=
                                    HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                                {
                                    rysuj_obiekt(adbp, COPY_PUT, 0);
                                    ((LINIA *) adbp)->typ = Set_WidthL_to_Type(((LINIA *) adbp)->typ, i_width);
                                    rysuj_obiekt(adbp, COPY_PUT, 1);
                                }
                                break;
                            case Oluk:
                                if (((LUK *) adbp)->typ !=
                                    HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                                {
                                    rysuj_obiekt(adbp, COPY_PUT, 0);
                                    ((LUK *) adbp)->typ = Set_WidthL_to_Type(((LUK *) adbp)->typ, i_width);
                                    rysuj_obiekt(adbp, COPY_PUT, 1);
                                }
                                break;
                            case Oellipticalarc:
                                if (((ELLIPTICALARC *) adbp)->typ !=
                                    HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                                {
                                    rysuj_obiekt(adbp, COPY_PUT, 0);
                                    ((ELLIPTICALARC *) adbp)->typ = Set_WidthL_to_Type(((ELLIPTICALARC *) adbp)->typ, i_width);
                                    rysuj_obiekt(adbp, COPY_PUT, 1);
                                }
                                break;
                            case Owwielokat:
                                rysuj_obiekt(adbp, COPY_PUT, 0);
                                ((WIELOKAT *) adbp)->empty_typ = i_width;
                                rysuj_obiekt(adbp, COPY_PUT, 1);
                                break;
                            case Osolidarc:
                                rysuj_obiekt(adbp, COPY_PUT, 0);
                                ((SOLIDARC *) adbp)->empty_typ = i_width;
                                rysuj_obiekt(adbp, COPY_PUT, 1);
                                break;
                            case Ospline:
                                rysuj_obiekt(adbp, COPY_PUT, 0);
                                ((SPLINE *) adbp)->typ = Set_WidthL_to_Type(((LUK *) adbp)->typ, i_width);
                                rysuj_obiekt(adbp, COPY_PUT, 1);
                            default:
                                break;
                        }
                    }
                    adbp+=(sizeof(NAGLOWEK) + ((NAGLOWEK*)adbp)->n);
                }
                break;
            default:
                break;
        }

        if ((object_type==Owwielokat) || (object_type==Osolidarc))
        {
            if (i_width==0) i_width=5;
            else i_width--;
        }
        strcpy(sk,view_width_tab1[i_width]);
        menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk);

        Change = TRUE;
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

int point_type[]={0, 1, 8, 7, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void ch_ptype (void) {
    int n;

    frame_off(&mInfoAboutA);

    menu_level++;
    menu_address[menu_level-1]=&mTyp_punktu;
    n = Simple_Menu_Proc(&mTyp_punktu);
    if (n)
    {
        rysuj_obiekt(object_info_ad, COPY_PUT, 0);
        ((T_Point *) object_info_ad)->typ = point_type[n-1];
        rysuj_obiekt(object_info_ad, COPY_PUT, 1);
        //menu update
        menu_par_new((*mInfoAboutA.pola)[menu_n].txt, typ_punktu_inf[((T_Point *)object_info_ad)->typ]);
        Change = TRUE;
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;

}

void ch_stype (void) {
    int n;

    frame_off(&mInfoAboutA);

    menu_level++;
    switch (((AVECTOR *)object_info_ad)->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            menu_address[menu_level-1]=&mMember_style;
            n = Simple_Menu_Proc(&mMember_style);
            break;
        case 4:
        case 7:
            menu_address[menu_level-1]=&mForce_Displacement_style;
            n = Simple_Menu_Proc(&mForce_Displacement_style);
            switch (n)
            {
                case 1:
                    n=5;
                    break;
                case 2:
                    n=8;
                    break;
            }
            break;
        case 5:
        case 6:
        case 8:
        case 9:
            menu_address[menu_level-1]=&mMoment_Rotation_style;
            n = Simple_Menu_Proc(&mMoment_Rotation_style);
            switch (n)
            {
                case 1:
                    n=6;
                    break;
                case 2:
                    n=7;
                    break;
                case 3:
                    n=9;
                    break;
                case 4:
                    n=10;
                    break;
            }
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:menu_address[menu_level-1]=&mLoad_style;
            n = Simple_Menu_Proc(&mLoad_style);
            switch (n)
            {
                case 1:
                    n=11;
                    break;
                case 2:
                    n=12;
                    break;
                case 3:
                    n=13;
                    break;
                case 4:
                    n=14;
                    break;
                case 5:
                    n=15;
                    break;
            }
            break;
        default:
            n=0;
            break;
    }

    if (n)
    {
        rysuj_obiekt(object_info_ad, COPY_PUT, 0);
        ((AVECTOR *) object_info_ad)->style = n-1;
        rysuj_obiekt(object_info_ad, COPY_PUT, 1);
        //menu update
        menu_par_new((*mInfoAboutA.pola)[menu_n].txt, vector_style_tab[((AVECTOR *)object_info_ad)->style]);
        Change = TRUE;
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;

}

static POLE pmObjectSelected[] = {
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
        {u8"",     L' ',0,NULL},
};

static TMENU mObjectSelected = { 0,0,0,10,30,7,ICONS,CMNU,CMBR,CMTX,0,19,0,0,0,(POLE(*)[]) &pmObjectSelected,NULL,NULL };

int vector_icon[]={723, 724, 725, 726, 727, 728, 729, 730, 731, 732, 733, 734, 735, 736, 737, 752, 786};

int point_icon[]={294, 295, 0, 0, 0, 0, 0, 529, 296, 0, 0, 0, 738, 739, 740, 754, 741, 742, 743, 755, 744, 745, 746, 756, 747, 748, 749, 757, 818};
extern char *vector_txt[];
extern char *point_txt[];

int object_icon[]={63, 13, 16, 15, 14, 18, 21, 27, 1, 15 /*TO CHANGE FOR SOLIDARC ICON*/, 706, 19, 707, 516, 722, 63, 0};
extern unsigned short object_wcod[];
extern char *object_txt[];

int choose_object(int type_address_no, TYPE_ADDRESS *type_address)
{
    int n, i;
    int menu_size=0;
    int iconno;
    unsigned short wcod;
    char txt[MaxTextLen];
    void *adr;
    char o_layer_s[12];

    menu_level++;
    menu_address[menu_level-1]=&mObjectSelected;

    mObjectSelected.max=0;

    // Olinia = 1, Otekst = 2,
    //	      Oluk = 3,Ookrag = 4,Okolo = 5, Owwielokat = 6, Opoint = 7, OdBLOK = 8, Osolidarc=9,
    //         Oellipticalarc = 10, Oellipse = 11, Ofilledellipse = 12, Ospline = 13,
    //         Ovector = 14, Opcx = 0,
    //         ONoObiect = 16, Okoniec=0xf};

    for (i=0; i<type_address_no; i++)
    {
        sprintf(txt,"֊%03d",type_address[i].color);

        if (type_address[i].type==Ovector)
        {
            iconno=vector_icon[type_address[i].style];
            wcod=vector_wcod[type_address[i].style];
            strcat(txt,vector_txt[type_address[i].style]);

        }
        else if (type_address[i].type==Opoint)
        {
            iconno=point_icon[type_address[i].style];
            wcod=point_wcod[type_address[i].style];
            strcat(txt,typ_punktu_inf[type_address[i].style]);

        }
        else {
            iconno = object_icon[type_address[i].type];
            wcod=object_wcod[type_address[i].type];
            strcat(txt,object_txt[type_address[i].type]);
        }

        (*mObjectSelected.pola)[i].iconno=iconno;
        (*mObjectSelected.pola)[i].wcod=wcod;

        if (type_address[i].layer>0) {
            sprintf(o_layer_s, " (%d)", type_address[i].layer);
            strcat(txt, o_layer_s);
        }
        strcpy(&(*mObjectSelected.pola)[i].txt, txt);
        mObjectSelected.max++;
    }

    mObjectSelected.maxw=0;
    mObjectSelected.maxw0=mObjectSelected.max;

    n = Simple_Menu_Proc(&mObjectSelected);

    return n-1;
}

void ch_ltype (void)
{
    int n;
    int object_type;
    int i_typ, i_typ0, i_width;
    static char sk[64];
    char *adbp, *adbk;
    BOOL found;

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;

    switch (object_type)
    {
        case Olinia:
            if (((LINIA *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                i_typ0 = ((LINIA *)object_info_ad)->typ ;
            else return;
            break;
        case Ookrag:
        case Okolo:
            if (((OKRAG *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                i_typ0 = ((OKRAG *)object_info_ad)->typ;
            else return;
            break;
        case Oellipse:
        case Ofilledellipse:
            if (((ELLIPSE *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                i_typ0 = ((ELLIPSE *)object_info_ad)->typ;
            else return;
            break;
        case Oluk:
            if (((LUK *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                i_typ0 = ((LUK *)object_info_ad)->typ;
            else return;
            break;
        case Oellipticalarc:
            if (((ELLIPTICALARC *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                i_typ0 = ((ELLIPTICALARC *)object_info_ad)->typ;
            else return;
            break;
        case Ospline:
            if (((SPLINE *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                i_typ0 = ((SPLINE *)object_info_ad)->typ;
            else return;
            break;
        case Ovector:
                i_typ0 = ((AVECTOR *)object_info_ad)->typ ;
            break;
        case OdBLOK:
               adbp = (char *)object_info_ad + sizeof(NAGLOWEK)+B3+((BLOK*)object_info_ad)->dlugosc_opisu_obiektu;
               adbk = (char *)object_info_ad + sizeof(NAGLOWEK) + ((BLOK*)object_info_ad)->n - 1;
               found=FALSE;
               while (adbp<adbk)
               {
                   if (((NAGLOWEK*)adbp)->atrybut!=Abad) {
                       switch (((NAGLOWEK *) adbp)->obiekt) {
                           case Olinia:
                               if (((LINIA *) adbp)->typ !=
                                   HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                               {
                                   i_typ0 = ((LINIA *) adbp)->typ;
                                   found = TRUE;
                               }
                               break;
                           case Oluk:
                               if (((LUK *) adbp)->typ !=
                                   HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                               {
                                   i_typ0 = ((LUK *) adbp)->typ;
                                   found = TRUE;
                               }
                               break;
                           case Oellipticalarc:
                               if (((ELLIPTICALARC *) adbp)->typ !=
                                   HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                               {
                                   i_typ0 = ((ELLIPTICALARC *) adbp)->typ;
                                   found = TRUE;
                               }
                               break;
                           case Oellipse:
                               if (((ELLIPSE *) adbp)->typ !=
                                   HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                               {
                                   i_typ0 = ((ELLIPSE *) adbp)->typ;
                                   found = TRUE;
                               }
                               break;
                           case Owwielokat:
                               break;
                           case Osolidarc:
                               break;
                           case Ospline:
                               i_typ0 = ((SPLINE *) adbp)->typ;
                               found = TRUE;
                               break;
                           default:
                               break;
                       }
                       if (found == TRUE) break;
                   }
                   adbp+=sizeof(NAGLOWEK) + ((NAGLOWEK*)adbp)->n;
               }
            break;
        default:
            break;
    }

    frame_off(&mInfoAboutA);

    Test_Menu (&mTypyLinii);

    i_width=(i_typ0 & 224 ) / 32 ;
    i_typ=(i_typ0 & 31);

    if ((mTypyLinii.maxw==0) || (mTypyLinii.maxw>i_typ))
    {
        mTypyLinii.foff=0;
        mTypyLinii.poz=i_typ;
    }
    else
    {
        mTypyLinii.foff=i_typ - mTypyLinii.maxw + 1;
        mTypyLinii.poz=mTypyLinii.maxw-1;
    }

    menu_level++;
    menu_address[menu_level-1]=&mTypyLinii;
    n = Simple_Menu_Proc(&mTypyLinii);

    if (n)
    {
        i_typ=n-1;
        switch (object_type) {
            case Olinia:
                if (((LINIA *) object_info_ad)->typ !=HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                {
                    rysuj_obiekt(object_info_ad, COPY_PUT, 0);
                    ((LINIA *) object_info_ad)->typ = Set_TypeL_to_Type(((LINIA *) object_info_ad)->typ, i_typ);
                    rysuj_obiekt(object_info_ad, COPY_PUT, 1);
                }
                break;
            case Ookrag:
            case Okolo:
                if (((OKRAG *) object_info_ad)->typ !=HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                {
                rysuj_obiekt(object_info_ad, COPY_PUT, 0);
                ((OKRAG *) object_info_ad)->typ = Set_TypeL_to_Type(((OKRAG *) object_info_ad)->typ, i_typ);
                rysuj_obiekt(object_info_ad, COPY_PUT, 1);
                }
                break;
            case Oellipse:
            case Ofilledellipse:
                if (((ELLIPSE *) object_info_ad)->typ !=HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                {
                    rysuj_obiekt(object_info_ad, COPY_PUT, 0);
                    ((ELLIPSE *) object_info_ad)->typ = Set_TypeL_to_Type(((ELLIPSE *) object_info_ad)->typ, i_typ);
                    rysuj_obiekt(object_info_ad, COPY_PUT, 1);
                }
                break;
            case Oluk:
                if (((LUK *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                {
                    rysuj_obiekt(object_info_ad, COPY_PUT,0);
                    ((LUK *) object_info_ad)->typ = Set_TypeL_to_Type(((LUK *) object_info_ad)->typ, i_typ);
                    rysuj_obiekt(object_info_ad, COPY_PUT, 1);
                }
                break;
            case Oellipticalarc:
                if (((ELLIPTICALARC *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                {
                    rysuj_obiekt(object_info_ad, COPY_PUT,0);
                    ((ELLIPTICALARC *) object_info_ad)->typ = Set_TypeL_to_Type(((ELLIPTICALARC *) object_info_ad)->typ, i_typ);
                    rysuj_obiekt(object_info_ad, COPY_PUT, 1);
                }
                break;
            case Ospline:
                if (((SPLINE *)object_info_ad)->typ != HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                {
                    rysuj_obiekt(object_info_ad, COPY_PUT,0);
                    ((SPLINE *) object_info_ad)->typ = Set_TypeL_to_Type(((SPLINE *) object_info_ad)->typ, i_typ);
                    rysuj_obiekt(object_info_ad, COPY_PUT, 1);
                }
                break;
            case Ovector:
                    rysuj_obiekt(object_info_ad, COPY_PUT, 0);
                    ((AVECTOR *) object_info_ad)->typ = Set_TypeL_to_Type(((AVECTOR *) object_info_ad)->typ, i_typ);
                    rysuj_obiekt(object_info_ad, COPY_PUT, 1);
                break;
            case OdBLOK:
                adbp = (char *)object_info_ad + sizeof(NAGLOWEK)+B3+((BLOK*)object_info_ad)->dlugosc_opisu_obiektu;
                adbk = (char *)object_info_ad + sizeof(NAGLOWEK) + ((BLOK*)object_info_ad)->n - 1;
                while (adbp<adbk)
                {
                    if (((NAGLOWEK*)adbp)->atrybut!=Abad) {
                        switch (((NAGLOWEK *) adbp)->obiekt) {
                            case Olinia:

                                if (((LINIA *) adbp)->typ !=
                                    HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                                {
                                    rysuj_obiekt(adbp, COPY_PUT, 0);
                                    ((LINIA *) adbp)->typ = Set_TypeL_to_Type(((LINIA *) adbp)->typ, i_typ);
                                    rysuj_obiekt(adbp, COPY_PUT, 1);
                                }
                                break;
                            case Oluk:
                                if (((LUK *) adbp)->typ !=
                                    HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                                {
                                    rysuj_obiekt(adbp, COPY_PUT, 0);
                                    ((LUK *) adbp)->typ = Set_TypeL_to_Type(((LUK *) adbp)->typ, i_typ);
                                    rysuj_obiekt(adbp, COPY_PUT, 1);
                                }
                                break;
                            case Oellipticalarc:
                                if (((ELLIPTICALARC *) adbp)->typ !=
                                    HATCH_OUTLINE_TYPE) //do not change type for invisible / hatch outline
                                {
                                    rysuj_obiekt(adbp, COPY_PUT, 0);
                                    ((ELLIPTICALARC *) adbp)->typ = Set_TypeL_to_Type(((ELLIPTICALARC *) adbp)->typ, i_typ);
                                    rysuj_obiekt(adbp, COPY_PUT, 1);
                                }
                                break;
                            case Owwielokat:
                                break;
                            case Ospline:
                                rysuj_obiekt(adbp, COPY_PUT, 0);
                                ((SPLINE *) adbp)->typ = Set_TypeL_to_Type(((SPLINE *) adbp)->typ, i_typ);
                                rysuj_obiekt(adbp, COPY_PUT, 1);
                                break;
                            default:
                                break;
                        }
                    }
                    adbp+=(sizeof(NAGLOWEK) + ((NAGLOWEK*)adbp)->n);
                }
                break;
            default:
                baronoff_(&mInfoAboutA);
                baronoff(&mInfoAboutA);
                return;
                break;
        }

        strcpy(sk,view_type_tab[n-1]);
        menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk);

        if ((mTypyLinii.maxw==0) || (mTypyLinii.maxw>i_typ))
        {
            mTypyLinii.foff=0;
            mTypyLinii.poz=i_typ;
        }
        else
        {
            mTypyLinii.foff=i_typ - mTypyLinii.maxw + 1;
            mTypyLinii.poz=mTypyLinii.maxw-1;
        }

        Change = TRUE;
    }

    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_x_ (int nr)
{   double lx1;
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double x0, y0;
    double x1, y1;

    frame_off(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
    {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    x1 = buf_ret [0] ;
    //conversion to global coordinates

    rysuj_obiekt(object_info_ad, COPY_PUT,0);
    switch (object_type)
    {
        case Olinia:
            switch (nr) {
                case 0:
                    x0 = ((LINIA *) object_info_ad)->x1;
                    y0 = ((LINIA *) object_info_ad)->y1;
                    break;
                case 1:
                    x0 = ((LINIA *) object_info_ad)->x2;
                    y0 = ((LINIA *) object_info_ad)->y2;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            y1 = milimetryobyl(x0, y0) ;
            sprintf(sk_info, "%-12.9f",x1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((LINIA *) object_info_ad)->x1 = (float) x1;
                    ((LINIA *) object_info_ad)->y1 = (float) y1;
                    break;
                case 1:
                    ((LINIA *) object_info_ad)->x2 = (float) x1;
                    ((LINIA *) object_info_ad)->y2 = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Ovector:
            switch (nr) {
                case 0:
                    x0 = ((AVECTOR *) object_info_ad)->x1;
                    y0 = ((AVECTOR *) object_info_ad)->y1;
                    break;
                case 1:
                    x0 = ((AVECTOR *) object_info_ad)->x2;
                    y0 = ((AVECTOR *) object_info_ad)->y2;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            y1 = milimetryobyl(x0, y0) ;
            sprintf(sk_info, "%-12.9f",x1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((AVECTOR *) object_info_ad)->x1 = (float) x1;
                    ((AVECTOR *) object_info_ad)->y1 = (float) y1;
                    break;
                case 1:
                    ((AVECTOR *) object_info_ad)->x2 = (float) x1;
                    ((AVECTOR *) object_info_ad)->y2 = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Ookrag:
        case Okolo:
            switch (nr) {
                case 0:
                    x0 = ((OKRAG *) object_info_ad)->x;
                    y0 = ((OKRAG *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            y1 = milimetryobyl(x0, y0) ;
            sprintf(sk_info, "%-12.9f",x1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((OKRAG *) object_info_ad)->x = (float) x1;
                    ((OKRAG *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Oellipse:
        case Ofilledellipse:
            switch (nr) {
                case 0:
                    x0 = ((ELLIPSE *) object_info_ad)->x;
                    y0 = ((ELLIPSE *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            y1 = milimetryobyl(x0, y0) ;
            sprintf(sk_info, "%-12.9f",x1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((ELLIPSE *) object_info_ad)->x = (float) x1;
                    ((ELLIPSE *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Oluk:
            switch (nr) {
                case 0:
                    x0 = ((LUK *) object_info_ad)->x;
                    y0 = ((LUK *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            y1 = milimetryobyl(x0, y0) ;
            sprintf(sk_info, "%-12.9f",x1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((LUK *) object_info_ad)->x = (float) x1;
                    ((LUK *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Osolidarc:
            switch (nr) {
                case 0:
                    x0 = ((SOLIDARC *) object_info_ad)->x;
                    y0 = ((SOLIDARC *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            y1 = milimetryobyl(x0, y0) ;
            sprintf(sk_info, "%-12.9f",x1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((SOLIDARC *) object_info_ad)->x = (float) x1;
                    ((SOLIDARC *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Oellipticalarc:
            switch (nr) {
                case 0:
                    x0 = ((ELLIPTICALARC *) object_info_ad)->x;
                    y0 = ((ELLIPTICALARC *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            y1 = milimetryobyl(x0, y0) ;
            sprintf(sk_info, "%-12.9f",x1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((ELLIPTICALARC *) object_info_ad)->x = (float) x1;
                    ((ELLIPTICALARC *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Opoint:
            switch (nr) {
                case 0:
                    x0 = ((T_Point *) object_info_ad)->x;
                    y0 = ((T_Point *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            y1 = milimetryobyl(x0, y0) ;
            sprintf(sk_info, "%-12.9f",x1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((T_Point *) object_info_ad)->x = (float) x1;
                    ((T_Point *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Otekst:
            switch (nr) {
                case 0:
                    x0 = ((TEXT *) object_info_ad)->x;
                    y0 = ((TEXT *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            y1 = milimetryobyl(x0, y0) ;
            sprintf(sk_info, "%-12.9f",x1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((TEXT *) object_info_ad)->x = (float) x1;
                    ((TEXT *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
            Change = TRUE;
            break;
        case Owwielokat:
            x0 = ((WIELOKAT *) object_info_ad)->xy[nr*2];
            y0 = ((WIELOKAT *) object_info_ad)->xy[nr*2+1];

            y1 = milimetryobyl(x0, y0) ;
            sprintf(sk_info, "%-12.9f",x1);
            get_global_coords (&x1, &y1);

            ((WIELOKAT*) object_info_ad)->xy[nr*2] = (float) x1;
            ((WIELOKAT *) object_info_ad)->xy[nr*2+1] = (float) y1;

            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Ospline:
            x0 = ((SPLINE *) object_info_ad)->xy[nr*2];
            y0 = ((SPLINE *) object_info_ad)->xy[nr*2+1];

            y1 = milimetryobyl(x0, y0) ;
            sprintf(sk_info, "%-12.9f",x1);
            get_global_coords (&x1, &y1);

            ((SPLINE*) object_info_ad)->xy[nr*2] = (float) x1;
            ((SPLINE *) object_info_ad)->xy[nr*2+1] = (float) y1;

            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Opcx:
                switch (nr) {
                    case 0:
                        x0 = ((B_PCX *) object_info_ad)->x;
                        y0 = ((B_PCX *) object_info_ad)->y;
                        break;
                    default:
                        baronoff_(&mInfoAboutA);
                        baronoff(&mInfoAboutA);
                        return;
                        break;
                }
                y1 = milimetryobyl(x0, y0) ;
                sprintf(sk_info, "%-12.9f",x1);
                get_global_coords (&x1, &y1);

                ((B_PCX *) object_info_ad)->x = (float) x1;
                ((B_PCX *) object_info_ad)->y = (float) y1;

                menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);

                Change = TRUE;
                regen_ctx=TRUE;
                TTF_redraw=TRUE;
            break;
        default:
            break;
    }
    rysuj_obiekt(object_info_ad, COPY_PUT,1);

    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_y_ (int nr)
{
    double lx1;
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double x0, y0;
    double x1, y1;

    frame_off(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
    {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    y1 = buf_ret [0] ;
    //conversion to global coordinates

    rysuj_obiekt(object_info_ad, COPY_PUT,0);
    switch (object_type)
    {
        case Olinia:
            switch (nr) {
                case 0: x0 = ((LINIA *) object_info_ad)->x1;
                        y0 = ((LINIA *) object_info_ad)->y1;
                break;
                case 1: x0 = ((LINIA *) object_info_ad)->x2;
                        y0 = ((LINIA *) object_info_ad)->y2;
                break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                break;
            }
            x1 = milimetryobxl(x0, y0);
            sprintf(sk_info, "%-12.9f",y1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((LINIA *) object_info_ad)->x1 = (float) x1;
                    ((LINIA *) object_info_ad)->y1 = (float) y1;
                    break;
                case 1:
                    ((LINIA *) object_info_ad)->x2 = (float) x1;
                    ((LINIA *) object_info_ad)->y2 = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Ovector:
            switch (nr) {
                case 0: x0 = ((AVECTOR *) object_info_ad)->x1;
                    y0 = ((AVECTOR *) object_info_ad)->y1;
                    break;
                case 1: x0 = ((AVECTOR *) object_info_ad)->x2;
                    y0 = ((AVECTOR *) object_info_ad)->y2;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            x1 = milimetryobxl(x0, y0);
            sprintf(sk_info, "%-12.9f",y1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((AVECTOR *) object_info_ad)->x1 = (float) x1;
                    ((AVECTOR *) object_info_ad)->y1 = (float) y1;
                    break;
                case 1:
                    ((AVECTOR *) object_info_ad)->x2 = (float) x1;
                    ((AVECTOR *) object_info_ad)->y2 = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Ookrag:
        case Okolo:
            switch (nr) {
                case 0: x0 = ((OKRAG *) object_info_ad)->x;
                    y0 = ((OKRAG *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            x1 = milimetryobxl(x0, y0);
            sprintf(sk_info, "%-12.9f",y1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((OKRAG *) object_info_ad)->x = (float) x1;
                    ((OKRAG *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Oellipse:
        case Ofilledellipse:
            switch (nr) {
                case 0: x0 = ((ELLIPSE *) object_info_ad)->x;
                    y0 = ((ELLIPSE *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            x1 = milimetryobxl(x0, y0);
            sprintf(sk_info, "%-12.9f",y1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((ELLIPSE *) object_info_ad)->x = (float) x1;
                    ((ELLIPSE *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Oluk:
            switch (nr) {
                case 0: x0 = ((LUK *) object_info_ad)->x;
                    y0 = ((LUK *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            x1 = milimetryobxl(x0, y0);
            sprintf(sk_info, "%-12.9f",y1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((LUK *) object_info_ad)->x = (float) x1;
                    ((LUK *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Osolidarc:
            switch (nr) {
                case 0: x0 = ((SOLIDARC *) object_info_ad)->x;
                    y0 = ((SOLIDARC *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            x1 = milimetryobxl(x0, y0);
            sprintf(sk_info, "%-12.9f",y1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((SOLIDARC *) object_info_ad)->x = (float) x1;
                    ((SOLIDARC *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Oellipticalarc:
            switch (nr) {
                case 0: x0 = ((ELLIPTICALARC *) object_info_ad)->x;
                    y0 = ((ELLIPTICALARC *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            x1 = milimetryobxl(x0, y0);
            sprintf(sk_info, "%-12.9f",y1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((ELLIPTICALARC *) object_info_ad)->x = (float) x1;
                    ((ELLIPTICALARC *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Opoint:
            switch (nr) {
                case 0: x0 = ((T_Point *) object_info_ad)->x;
                    y0 = ((T_Point *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            x1 = milimetryobxl(x0, y0);
            sprintf(sk_info, "%-12.9f",y1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((T_Point *) object_info_ad)->x = (float) x1;
                    ((T_Point *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Otekst:
            switch (nr) {
                case 0: x0 = ((TEXT *) object_info_ad)->x;
                    y0 = ((TEXT *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            x1 = milimetryobxl(x0, y0);
            sprintf(sk_info, "%-12.9f",y1);
            get_global_coords (&x1, &y1);
            switch (nr) {
                case 0:
                    ((TEXT *) object_info_ad)->x = (float) x1;
                    ((TEXT *) object_info_ad)->y = (float) y1;
                    break;
            }
            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
            Change = TRUE;
            break;
        case Owwielokat:

            x0 = ((WIELOKAT *) object_info_ad)->xy[nr*2+1];
            y0 = ((WIELOKAT *) object_info_ad)->xy[nr*2];

            x1 = milimetryobxl(x0, y0);
            sprintf(sk_info, "%-12.9f",y1);
            get_global_coords (&x1, &y1);
            ((WIELOKAT *) object_info_ad)->xy[nr*2] = (float) x1;
            ((WIELOKAT *) object_info_ad)->xy[nr*2+1] = (float) y1;

            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Ospline:

            x0 = ((SPLINE *) object_info_ad)->xy[nr*2+1];
            y0 = ((SPLINE *) object_info_ad)->xy[nr*2];

            x1 = milimetryobxl(x0, y0);
            sprintf(sk_info, "%-12.9f",y1);
            get_global_coords (&x1, &y1);
            ((SPLINE *) object_info_ad)->xy[nr*2] = (float) x1;
            ((SPLINE *) object_info_ad)->xy[nr*2+1] = (float) y1;

            //menu update
            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Opcx:
            switch (nr) {
                case 0:
                    x0 = ((B_PCX *) object_info_ad)->x;
                    y0 = ((B_PCX *) object_info_ad)->y;
                    break;
                default:
                    baronoff_(&mInfoAboutA);
                    baronoff(&mInfoAboutA);
                    return;
                    break;
            }
            x1 = milimetryobxl(x0, y0) ;
            sprintf(sk_info, "%-12.9f",y1);
            get_global_coords (&x1, &y1);

            ((B_PCX *) object_info_ad)->x = (float) x1;
            ((B_PCX *) object_info_ad)->y = (float) y1;

            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk_info);

            Change = TRUE;
            regen_ctx=TRUE;
            TTF_redraw=TRUE;
            break;
        default:
            break;
    }
    rysuj_obiekt(object_info_ad, COPY_PUT,1);

    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_x1 (void)
{
    ch_x_(0);
    return;
}

void ch_y1 (void)
{
    ch_y_(0);
    return;
}

void ch_x2 (void)
{
    ch_x_(1);
    return;
}

void ch_y2 (void)
{
    ch_y_(1);
    return;
}

void ch_x3 (void)
{
    ch_x_(2);
    return;
}

void ch_y3 (void)
{
    ch_y_(2);
    return;
}

void ch_x4 (void)
{
    ch_x_(3);
    return;
}

void ch_y4 (void)
{
    ch_y_(3);
    return;
}

void ch_radius (void)
{
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double r0;
    double r1;

    frame_off(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
    {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    r0 = buf_ret [0] ;

    rysuj_obiekt(object_info_ad, COPY_PUT,0);
    switch (object_type) {
        case Ookrag:
        case Okolo:
            sprintf(sk_info, "%-12.9f", r0);
            r1 = jednostkiOb(r0);
            ((OKRAG *) object_info_ad)->r = (float) r1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Oluk:
            sprintf(sk_info, "%-12.9f", r0);
            r1 = jednostkiOb(r0);
            sprintf(sk_info, "%-12.9f", r1);
            ((LUK *) object_info_ad)->r = (float) r1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Osolidarc:
            sprintf(sk_info, "%-12.9f", r0);
            r1 = jednostkiOb(r0);
            sprintf(sk_info, "%-12.9f", r1);
            ((SOLIDARC *) object_info_ad)->r = (float) r1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Oellipse:
        case Ofilledellipse:
            sprintf(sk_info, "%-12.9f", r0);
            r1 = jednostkiOb(r0);
            ((ELLIPSE *) object_info_ad)->rx = (float) r1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Oellipticalarc:
            sprintf(sk_info, "%-12.9f", r0);
            r1 = jednostkiOb(r0);
            ((ELLIPTICALARC *) object_info_ad)->rx = (float) r1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Ovector:
            sprintf(sk_info, "%-9.4f", r0);
            ((AVECTOR *) object_info_ad)->r = (float) r0;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        default:
            break;
    }
    rysuj_obiekt(object_info_ad, COPY_PUT,1);
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_radiusy (void)
{
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double r0;
    double r1;

    frame_off(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
    {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    r0 = buf_ret [0] ;

    rysuj_obiekt(object_info_ad, COPY_PUT,0);
    switch (object_type) {
        case Oellipse:
        case Ofilledellipse:
            sprintf(sk_info, "%-12.9f", r0);
            r1 = jednostkiOb(r0);
            sprintf(sk_info, "%-12.9f", r1);
            ((ELLIPSE *) object_info_ad)->ry = (float) r1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Oellipticalarc:
            sprintf(sk_info, "%-12.9f", r0);
            r1 = jednostkiOb(r0);
            ((ELLIPTICALARC *) object_info_ad)->ry = (float) r1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        default:
            break;
    }
    rysuj_obiekt(object_info_ad, COPY_PUT,1);
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_angle (void)
{
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double a0;
    double a1;
    PLINIA PL;
    double kat, kos, koc;

    frame_off(&mInfoAboutA);

    object_type = ((NAGLOWEK *) object_info_ad)->obiekt;
    ret = get_string_str(sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator(sk_info, &retval_no, buf_ret) || retval_no < 1) {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    rysuj_obiekt(object_info_ad, COPY_PUT,0);
    switch (object_type) {
        case Oellipse:
        case Ofilledellipse:
            a0 = buf_ret[0];
            a1 = a0 + get_angle_l();
            if (a1 > 360.0) a1 -= 360.0;
            a1 = a1 * Pi2 / 360.0;
            sprintf(sk_info, "%-12.9f", a0);
            ((ELLIPSE *) object_info_ad)->angle = (float) a1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Oellipticalarc:
            a0 = buf_ret[0];
            a1 = a0 + get_angle_l();
            if (a1 > 360.0) a1 -= 360.0;
            a1 = a1 * Pi2 / 360.0;
            sprintf(sk_info, "%-12.9f", a0);
            ((ELLIPTICALARC *) object_info_ad)->angle = (float) a1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Otekst:
            a0 = buf_ret[0];
            a1 = a0 + get_angle_l();
            if (a1 > 360.0) a1 -= 360.0;
            a1 = a1 * Pi2 / 360.0;
            sprintf(sk_info, "%-12.9f", a0);
            ((TEXT *) object_info_ad)->kat = (float) a1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
            break;
        case Opcx:
            a0 = buf_ret[0];
            a1 = a0 + get_angle_l();
            if (a1 > 360.0) a1 -= 360.0;
            a1 = a1 * Pi2 / 360.0;
            sprintf(sk_info, "%-12.9f", a0);
            ((B_PCX *) object_info_ad)->kat = (float) a1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            regen_ctx=TRUE;
            TTF_redraw=TRUE;
            break;
        case Ovector:
            a0 = buf_ret[0];
            a1 = a0 + get_angle_l();
            if (a1 > 360.0) a1 -= 360.0;
            a1 = Angle_Normal (a1 * Pi2 / 360.0);
            sprintf(sk_info, "%-12.9f", a0);

            switch (((AVECTOR *) object_info_ad)->style)
            {
                case 4:
                case 7:
                    parametry_lini((AVECTOR *) object_info_ad, &PL);
                    kat=PL.kat;
                    kos=sin(a1);
                    koc=cos(a1);

                    ((AVECTOR *) object_info_ad)->x2 = ((AVECTOR *) object_info_ad)->x1 + PL.dl * koc;
                    ((AVECTOR *) object_info_ad)->y2 = ((AVECTOR *) object_info_ad)->y1 + PL.dl * kos;

                    Change = TRUE;
                    break;
            }
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            break;
        default:
            break;
    }
    rysuj_obiekt(object_info_ad, COPY_PUT,1);
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_angle1 (void)
{
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double a0;
    double a1;

    frame_off(&mInfoAboutA);

    object_type = ((NAGLOWEK *) object_info_ad)->obiekt;
    ret = get_string_str(sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator(sk_info, &retval_no, buf_ret) || retval_no < 1) {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    rysuj_obiekt(object_info_ad, COPY_PUT,0);
    switch (object_type) {
        case Oluk:
            a0 = buf_ret[0];
            a1 = a0 + get_angle_l();
            if (a1 > 360.0) a1 -= 360.0;
            a1 = a1 * Pi2 / 360.0;
            sprintf(sk_info, "%-12.9f", a0);
            ((LUK *) object_info_ad)->kat1 = (float) a1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
        break;
        case Osolidarc:
            a0 = buf_ret[0];
            a1 = a0 + get_angle_l();
            if (a1 > 360.0) a1 -= 360.0;
            a1 = a1 * Pi2 / 360.0;
            sprintf(sk_info, "%-12.9f", a0);
            ((SOLIDARC *) object_info_ad)->kat1 = (float) a1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Oellipticalarc:
            a0 = buf_ret[0];
            a1 = a0 + get_angle_l();
            if (a1 > 360.0) a1 -= 360.0;
            a1 = a1 * Pi2 / 360.0;
            sprintf(sk_info, "%-12.9f", a0);
            ((ELLIPTICALARC *) object_info_ad)->kat1 = (float) a1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        case Ovector:
            a0 = buf_ret[0];
            a1 = a0 + get_angle_l();
            if (a1 > 360.0) a1 -= 360.0;
            a1 = a1 * Pi2 / 360.0;
            sprintf(sk_info, "%-9.4f", a0);
            ((AVECTOR *) object_info_ad)->angle1 = (float) a1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            switch (((AVECTOR *) object_info_ad)->style)
            {
                case 5:
                case 6:
                    magnitude2angle(((AVECTOR *) object_info_ad), moment_magnitude);
                    break;
                case 8:
                case 9:
                    magnitude2angle((AVECTOR *) object_info_ad, rotation_magnitude);
                    break;
            }
            Change = TRUE;
            break;
        default:
        break;
    }
    rysuj_obiekt(object_info_ad, COPY_PUT,1);
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_angle2 (void)
{
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double a0;
    double a1;

    frame_off(&mInfoAboutA);

    object_type = ((NAGLOWEK *) object_info_ad)->obiekt;
    ret = get_string_str(sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator(sk_info, &retval_no, buf_ret) || retval_no < 1) {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    if (object_type==Oluk) {

        a0 = buf_ret[0];
        a1 = a0 + get_angle_l();
        if (a1 > 360.0) a1 -= 360.0;
        a1 = a1 * Pi2 / 360.0;
        rysuj_obiekt(object_info_ad, COPY_PUT, 0);
        sprintf(sk_info, "%-12.9f", a0);
        ((LUK *) object_info_ad)->kat2 = (float) a1;
        rysuj_obiekt(object_info_ad, COPY_PUT, 1);
        //menu update
        menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
        Change = TRUE;
    }
    if (object_type==Osolidarc) {

        a0 = buf_ret[0];
        a1 = a0 + get_angle_l();
        if (a1 > 360.0) a1 -= 360.0;
        a1 = a1 * Pi2 / 360.0;
        rysuj_obiekt(object_info_ad, COPY_PUT, 0);
        sprintf(sk_info, "%-12.9f", a0);
        ((SOLIDARC *) object_info_ad)->kat2 = (float) a1;
        rysuj_obiekt(object_info_ad, COPY_PUT, 1);
        //menu update
        menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
        Change = TRUE;
    }
    else
    if (object_type==Oellipticalarc) {
        a0 = buf_ret[0];
        a1 = a0 + get_angle_l();
        if (a1 > 360.0) a1 -= 360.0;
        a1 = a1 * Pi2 / 360.0;
        rysuj_obiekt(object_info_ad, COPY_PUT, 0);
        sprintf(sk_info, "%-12.9f", a0);
        ((ELLIPTICALARC *) object_info_ad)->kat2 = (float) a1;
        rysuj_obiekt(object_info_ad, COPY_PUT, 1);
        //menu update
        menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
        Change = TRUE;
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_width1 (void)
{
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double w0;
    double w1;

    frame_off(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
    {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    w0 = buf_ret [0] ;

    rysuj_obiekt(object_info_ad, COPY_PUT,0);
    switch (object_type) {
        case Osolidarc:
            sprintf(sk_info, "%-12.9f", w0);
            w1 = jednostkiOb(w0);
            if (((SOLIDARC *) object_info_ad)->reversed) ((SOLIDARC *) object_info_ad)->width1 = (float) w1;
            else ((SOLIDARC *) object_info_ad)->width2 = (float) w1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        default:
            break;
    }
    rysuj_obiekt(object_info_ad, COPY_PUT,1);
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_width2 (void)
{
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double w0;
    double w2;

    frame_off(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
    {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    w0 = buf_ret [0] ;

    rysuj_obiekt(object_info_ad, COPY_PUT,0);
    switch (object_type) {
        case Osolidarc:
            sprintf(sk_info, "%-12.9f", w0);
            w2 = jednostkiOb(w0);
            if (((SOLIDARC *) object_info_ad)->reversed) ((SOLIDARC *) object_info_ad)->width2 = (float) w2;
            else ((SOLIDARC *) object_info_ad)->width1 = (float) w2;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        default:
            break;
    }
    rysuj_obiekt(object_info_ad, COPY_PUT,1);
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_magnitude1 (void)
{
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double w0;
    double w1;

    frame_off(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
    {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    w0 = buf_ret [0] ;

    rysuj_obiekt(object_info_ad, COPY_PUT,0);
    switch (object_type) {
        case Ovector:
            sprintf(sk_info, "%-12.9f", w0);
            ((AVECTOR *) object_info_ad)->magnitude1 = (float) w0;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            switch (((AVECTOR *) object_info_ad)->style)
            {
                case 4:
                    magnitude2line((AVECTOR*)object_info_ad, force_magnitude);
                    set_decimal_format(&sk_info, ((AVECTOR *)object_info_ad)->magnitude1, force_precision);
                    //menu update
                    menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
                    break;
                case 7:
                    magnitude2line((AVECTOR*)object_info_ad, displacement_magnitude);
                    set_decimal_format(&sk_info, ((AVECTOR *)object_info_ad)->magnitude1, displacement_precision);
                    //menu update
                    menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
                    break;
                case 5:
                case 6:
                    magnitude2angle(((AVECTOR *) object_info_ad), moment_magnitude);
                    set_decimal_format(&sk_info, ((AVECTOR *)object_info_ad)->magnitude1, moment_precision);
                    //menu update
                    menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
                    break;
                case 8:
                case 9:
                    magnitude2angle(((AVECTOR *) object_info_ad), rotation_magnitude);
                    set_decimal_format(&sk_info, ((AVECTOR *)object_info_ad)->magnitude1, rotation_precision);
                    //menu update
                    menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
                    break;
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                    ((AVECTOR *) object_info_ad)->magnitude1 = (float) w0;
                    set_decimal_format(&sk_info, ((AVECTOR *)object_info_ad)->magnitude1, load_precision);
                    //menu update
                    menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
                    break;
                case 15:
                    ((AVECTOR *) object_info_ad)->magnitude1 = (float) w0;
                    set_decimal_format(&sk_info, ((AVECTOR *)object_info_ad)->magnitude1, thermal_precision);
                    //menu update
                    menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
                    break;
            }
            Change = TRUE;
            break;
        default:
            break;
    }
    rysuj_obiekt(object_info_ad, COPY_PUT,1);
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_magnitude2 (void)
{
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double w0;
    double w1;

    frame_off(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
    {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    w0 = buf_ret [0] ;

    rysuj_obiekt(object_info_ad, COPY_PUT,0);
    switch (object_type) {
        case Ovector:
            sprintf(sk_info, "%-9.5f", w0);
            ((AVECTOR *) object_info_ad)->magnitude2 = (float) w0;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        default:
            break;
    }
    rysuj_obiekt(object_info_ad, COPY_PUT,1);
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}


void ch_axis1 (void)
{
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double a0;
    double a1;

    frame_off(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
    {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    a0 = buf_ret [0] ;

    rysuj_obiekt(object_info_ad, COPY_PUT,0);
    switch (object_type) {
        case Osolidarc:
            sprintf(sk_info, "%-12.9f", a0);
            a1 = jednostkiOb(a0);
            if (((SOLIDARC *) object_info_ad)->reversed) ((SOLIDARC *) object_info_ad)->axis1 = (float) a1;
            else ((SOLIDARC *) object_info_ad)->axis2 = (float) a1;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        default:
            break;
    }
    rysuj_obiekt(object_info_ad, COPY_PUT,1);
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_axis2 (void)
{
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double a0;
    double a2;

    frame_off(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
    {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    a0 = buf_ret [0] ;

    rysuj_obiekt(object_info_ad, COPY_PUT,0);
    switch (object_type) {
        case Osolidarc:
            sprintf(sk_info, "%-12.9f", a0);
            a2 = jednostkiOb(a0);
            if (((SOLIDARC *) object_info_ad)->reversed) ((SOLIDARC *) object_info_ad)->axis2 = (float) a2;
            else ((SOLIDARC *) object_info_ad)->axis1 = (float) a2;
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            break;
        default:
            break;
    }
    rysuj_obiekt(object_info_ad, COPY_PUT,1);
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_opacity (void)
{
    int n;
    int object_type;
    unsigned char i_translucency;
    WIELOKAT *s;
    SOLIDARC *sa;
    char* translucency_ptr;
    int poz;
    double d_trans = 12.75;
    WIELOKAT solid_temp;
    char sk[10];
    int transluc;

    frame_off(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    switch (object_type) {
        case Ofilledellipse:
            i_translucency = ((ELLIPSE *) object_info_ad)->translucency;
            break;
        case Owwielokat:
            s = (WIELOKAT *)object_info_ad;
            if ((s->empty_typ == 0) && (s->pattern == 0) && (s->translucent == 1))
            {
                translucency_ptr = (char *)s->xy;
                translucency_ptr += (s->lp * sizeof(float));
                memmove(&i_translucency, translucency_ptr, sizeof(unsigned char));
            }
            else i_translucency=255;
            break;
        case Osolidarc:
            sa = (SOLIDARC *)object_info_ad;
            if ((sa->empty_typ == 0) && (sa->pattern == 0) && (sa->translucent == 1)) i_translucency=sa->translucency;
            else i_translucency=255;
            break;
    }

    poz=20-(int)((float)i_translucency/d_trans + 0.5) ;

    if ((mTranslucencyCH.maxw==0) || (mTranslucencyCH.maxw>mTranslucencyCH.poz))
    {
        mTranslucencyCH.foff=0;
        mTranslucencyCH.poz=poz;
    }
    else
    {
        mTranslucencyCH.foff=poz - mTranslucencyCH.maxw + 1;
        mTranslucencyCH.poz=mTranslucencyCH.maxw-1;
    }

    menu_level++;
    menu_address[menu_level-1]=&mTranslucencyCH;
    n = Simple_Menu_Proc(&mTranslucencyCH);

    if (n)
    {
        transluc=n-1;
        if (n!=poz) {
            i_translucency = (unsigned char)((20 - transluc) * d_trans);
            rysuj_obiekt(object_info_ad, COPY_PUT,0);
            switch (object_type) {
                case Ofilledellipse:
                    ((ELLIPSE *) object_info_ad)->translucency=i_translucency;
                    break;
                case Owwielokat:
                    if ((s->empty_typ==0) && (s->pattern==0))
                    {
                        if (s->translucent == 1) {
                            translucency_ptr = s->xy;
                            translucency_ptr += (s->lp * sizeof(float));
                            memmove(translucency_ptr, &i_translucency, sizeof(unsigned char));
                        } else //needs correction
                        {
                            memcpy(&solid_temp, s, sizeof(WIELOKAT));
                            solid_temp.translucent = 1;
                            solid_temp.n = 8 + solid_temp.lp * sizeof(float) + sizeof(unsigned char);

                            translucency_ptr = (char *) s->xy;
                            translucency_ptr += (s->lp * sizeof(float));
                            memmove(translucency_ptr, &i_translucency, sizeof(unsigned char));

                            if ((object_info_ad = korekta_obiekt((void *) object_info_ad, (void *) &solid_temp)) ==
                                NULL)
                                ErrList(15);
                        }
                    }
                    break;
                case Osolidarc:
                    if ((sa->empty_typ==0) && (sa->pattern==0))
                    {
                        sa->translucent = 1;
                        sa->translucency = i_translucency;
                    }
                    break;
            }
            rysuj_obiekt(object_info_ad, COPY_PUT,1);

            transluc = (int)((float)i_translucency/ d_trans +0.5);
            sprintf(sk, "%d%%", transluc * 5);

            menu_par_new ((*mInfoAboutA.pola)[menu_n].txt, sk);

            Change = TRUE;
        }
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_load_ch(void)
{
    int object_type;
    AVECTOR *v;
    int n0, n1, n2;
    int poz, poz1;
    static char sk[32];

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    if (object_type!=Ovector) return;
    v=(AVECTOR *)object_info_ad;
    if (v->style<4) return;
    if (v->style>15) return;

    frame_off(&mInfoAboutA);

    if (v->style==15) //thermal
    {
        if (v->load>0) poz=1;
        else poz=0;

        if (v->variant>0) poz1=(v->variant-1);
        else poz=0;

        if ((mLoad_Char_Thermal.maxw==0) || (mLoad_Char_Thermal.maxw>mLoad_Char_Thermal.poz))
        {
            mLoad_Char_Thermal.foff=0;
            mLoad_Char_Thermal.poz=poz;
        }
        else
        {
            mLoad_Char_Thermal.foff=poz - mLoad_Char_Thermal.maxw + 1;
            mLoad_Char_Thermal.poz=mLoad_Char_Thermal.maxw-1;
        }

        if ((mLoadVariant.maxw==0) || (mLoadVariant.maxw>mLoadVariant.poz))
        {
            mLoad_Char_Thermal.foff=0;
            mLoad_Char_Thermal.poz=poz1;
        }
        else
        {
            mLoadVariant.foff=poz1 - mLoadVariant.maxw + 1;
            mLoadVariant.poz=mLoadVariant.maxw-1;
        }

        menu_level++;
        menu_address[menu_level-1]=&mLoad_Char_Thermal;
        n0 = Simple_Menu_Proc(&mLoad_Char_Thermal);

        if (n0)
        {
            n1 = mLoad_Char_Thermal.foff + mLoad_Char_Thermal.poz;
            n2 = mLoadVariant.foff + mLoadVariant.poz;

            rysuj_obiekt(object_info_ad, COPY_PUT, 0);
            if (n1 > 0) v->load = 10;
            else v->load = 0;
            if (v->load == 0) v->variant = 0;
            else v->variant = n2;
            rysuj_obiekt(object_info_ad, COPY_PUT, 1);

            if (v->variant > 0)
                sprintf(sk, "%s%d", load_symbol[(int) v->load], v->variant);
            else sprintf(sk, "%s", load_symbol[(int) v->load]);

            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk);

            Change = TRUE;
        }
    }
    else
    {
        poz=v->load;
        if (v->variant>0) poz1=(v->variant-1);
        else poz=0;

        if ((mLoad_Char.maxw==0) || (mLoad_Char.maxw>mLoad_Char.poz))
        {
            mLoad_Char.foff=0;
            mLoad_Char.poz=poz;
        }
        else
        {
            mLoad_Char.foff=poz - mLoad_Char.maxw + 1;
            mLoad_Char.poz=mLoad_Char.maxw-1;
        }

        if ((mLoadVariant.maxw==0) || (mLoadVariant.maxw>mLoadVariant.poz))
        {
            mLoad_Char_Thermal.foff=0;
            mLoad_Char_Thermal.poz=poz1;
        }
        else
        {
            mLoadVariant.foff=poz1 - mLoadVariant.maxw + 1;
            mLoadVariant.poz=mLoadVariant.maxw-1;
        }

        menu_level++;
        menu_address[menu_level-1]=&mLoad_Char;
        n0 = Simple_Menu_Proc(&mLoad_Char);

        if (n0)
        {
            n1 = mLoad_Char.foff + mLoad_Char.poz;
            n2 = mLoadVariant.foff + mLoadVariant.poz;

            rysuj_obiekt(object_info_ad, COPY_PUT, 0);
            v->load = n1;
            if (v->load == 0) v->variant = 0;
            else v->variant = n2;
            rysuj_obiekt(object_info_ad, COPY_PUT, 1);

            if (v->variant > 0)
                sprintf(sk, "%s%d", load_symbol[(int) v->load], v->variant);
            else sprintf(sk, "%s", load_symbol[(int) v->load]);

            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk);

            Change = TRUE;
        }
    }

    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);

}

void ch_dx (void)
{
  return;
}

void ch_dx_pcx (void)
{
    int ret;
    double dx;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;

    object_type = ((NAGLOWEK *) object_info_ad)->obiekt;
    if (object_type==Opcx)
    {
        ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
        Restore_Pointer();
        if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
        {
            baronoff_(&mInfoAboutA);
            baronoff(&mInfoAboutA);
            return;
        }

        dx = buf_ret [0] ;
        ((B_PCX *)object_info_ad)->dx=dx;

        menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
        regen_ctx=TRUE;
        TTF_redraw=TRUE;

    }
    return;
}

void ch_dy (void)
{
    return;
}

void ch_dy_pcx (void)
{
    int ret;
    double dy;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;

    object_type = ((NAGLOWEK *) object_info_ad)->obiekt;
    if (object_type==Opcx)
    {
        ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
        Restore_Pointer();
        if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
        {
            baronoff_(&mInfoAboutA);
            baronoff(&mInfoAboutA);
            return;
        }

        dy = buf_ret [0] ;
        ((B_PCX *)object_info_ad)->dy=dy;

        menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
        regen_ctx=TRUE;
        TTF_redraw=TRUE;
    }
    return;
}

void ch_font_t (void)
{
    int n;
    int ret;
    int i_font, i_poz1;
    static char sk[64];

    frame_off(&mInfoAboutA);

    menu_level++;
    menu_address[menu_level-1]=&mCzcionka;
    Set_HEIGHT_high();

    Test_Menu (&mCzcionka);

    i_font = ((TEXT*)object_info_ad)->czcionka;
    if ((mCzcionka.maxw==0) || (mCzcionka.maxw>i_font))
    {
        mCzcionka.foff=0;
        mCzcionka.poz=(i_font);
    }
    else
    {
        mCzcionka.foff=i_font - mCzcionka.maxw + 1;
        mCzcionka.poz=mCzcionka.maxw-1;
    }

    n = Simple_Menu_Proc(&mCzcionka);

    Set_HEIGHT_back();
    ret = empty_dlg();

    if (n)
    {
        i_font=n-1;
        if (i_font >= WhNumberTextStyle)
        {
            i_poz1 = mTTF_OTF.poz + mTTF_OTF.foff;
            i_font = Add_New_Font_TTF_OTF(NULL, i_poz1, 1);
            if (i_font == -1) return;
        }

        if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;

        ((TEXT *) object_info_ad)->czcionka=i_font;

        ((TEXT *) object_info_ad)->width=0.0;
        ((TEXT *) object_info_ad)->height=0.0;

        strcpy(sk,PTRS__Text_Style[((TEXT *)object_info_ad)->czcionka]->font_name);
        menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk) ;

        Change = TRUE;

        if ((mCzcionka.maxw==0) || (mCzcionka.maxw>i_font))
        {
            mCzcionka.foff=0;
            mCzcionka.poz=(i_font);
        }
        else
        {
            mCzcionka.foff=i_font - mCzcionka.maxw + 1;
            mCzcionka.poz=mCzcionka.maxw-1;
        }

        if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;

    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_type_t (void)
{
    int n;

    frame_off(&mInfoAboutA);

    menu_level++;
    menu_address[menu_level-1]=&mTypTekstuI;
    n = Simple_Menu_Proc(&mTypTekstuI);
    if (n)
    {
        rysuj_obiekt(object_info_ad, COPY_PUT, 0);
        ((TEXT *) object_info_ad)->typ = n-1;
        rysuj_obiekt(object_info_ad, COPY_PUT, 1);
        //menu update
        menu_par_new((*mInfoAboutA.pola)[menu_n].txt, tab_typ_tekstu[((TEXT *)object_info_ad)->typ]);
        Change = TRUE;
        if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_hidden_t (void)
{
    int n;

    frame_off(&mInfoAboutA);

    menu_level++;
    menu_address[menu_level-1]=&mUkrytyI;
    n = Simple_Menu_Proc(&mUkrytyI);
    if (n)
    {
        rysuj_obiekt(object_info_ad, COPY_PUT,0);
        switch (n)
        {
            case 1:  //yes
                ((TEXT *) object_info_ad)->ukryty = 1;
                //menu update
                menu_par_new((*mInfoAboutA.pola)[menu_n].txt, _YES__);
                if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
                break;
            case 2:   //no
                ((TEXT *) object_info_ad)->ukryty = 0;
                //menu update
                menu_par_new((*mInfoAboutA.pola)[menu_n].txt, _NO__);
                if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
                break;
        }
        Change = TRUE;
        rysuj_obiekt(object_info_ad, COPY_PUT,1);
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_adjust_t (void)
{
    int n;
    frame_off(&mInfoAboutA);

    menu_level++;
    menu_address[menu_level-1]=&mJustowanieI;
    n = Simple_Menu_Proc(&mJustowanieI);
    if (n)
    {
        rysuj_obiekt(object_info_ad, COPY_PUT, 0);
        ((TEXT *) object_info_ad)->justowanie = n-1;

        rysuj_obiekt(object_info_ad, COPY_PUT, 1);
        //menu update
       menu_par_new((*mInfoAboutA.pola)[menu_n].txt, tab_justowanie[((TEXT *)object_info_ad)->typ]);
        Change = TRUE;
        if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_heigh_t (void)
{
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double h1;

    frame_off(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
    {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    h1 = buf_ret [0] ;

    switch (object_type) {
        case Otekst:
            rysuj_obiekt(object_info_ad, COPY_PUT, 0);
            sprintf(sk_info, "%-12.9f", h1);
            ((TEXT *) object_info_ad)->wysokosc = (float) h1;
            ((TEXT *) object_info_ad)->width=0.0;
            ((TEXT *) object_info_ad)->height=0.0;
            rysuj_obiekt(object_info_ad, COPY_PUT, 1);
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
            break;
        default:
            break;
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_width_t (void)
{
    int ret;
    int object_type;
    double buf_ret [1] ;
    int retval_no = 1 ;
    double w1;

    frame_off(&mInfoAboutA);

    object_type=((NAGLOWEK *)object_info_ad)->obiekt;
    ret=get_string_str (sk_info, "", MaxTextLen, 0, sk0_info);
    Restore_Pointer();
    if (FALSE == calculator (sk_info, &retval_no, buf_ret)  || retval_no < 1)
    {
        baronoff_(&mInfoAboutA);
        baronoff(&mInfoAboutA);
        return;
    }

    w1 = buf_ret [0] ;

    switch (object_type) {
        case Otekst:
            rysuj_obiekt(object_info_ad, COPY_PUT, 0);
            sprintf(sk_info, "%-12.9f", w1);
            ((TEXT *) object_info_ad)->width_factor = (float) w1;
            ((TEXT *) object_info_ad)->width=0.0;
            ((TEXT *) object_info_ad)->height=0.0;
            rysuj_obiekt(object_info_ad, COPY_PUT, 1);
            //menu update
            menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk_info);
            Change = TRUE;
            if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
            break;
        default:
            break;
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;Change = TRUE;
}

void ch_italic_t (void)
{
    int n;

    frame_off(&mInfoAboutA);

    menu_level++;
    menu_address[menu_level-1]=&mItalicsI;
    n = Simple_Menu_Proc(&mItalicsI);
    if (n)
    {
        rysuj_obiekt(object_info_ad, COPY_PUT,0);
        switch (n)
        {
            case 1:  //yes
                ((TEXT *) object_info_ad)->italics = 1;
                //menu update
                menu_par_new((*mInfoAboutA.pola)[menu_n].txt, _YES__);
                if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
                break;
            case 2:   //no
                ((TEXT *) object_info_ad)->italics = 0;
                //menu update
                menu_par_new((*mInfoAboutA.pola)[menu_n].txt, _NO__);
                if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
                break;
        }
        ((TEXT *) object_info_ad)->width=0.0;
        ((TEXT *) object_info_ad)->height=0.0;
        Change = TRUE;
        rysuj_obiekt(object_info_ad, COPY_PUT,1);
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_bold_t (void)
{
    int n;

    frame_off(&mInfoAboutA);

    menu_level++;
    menu_address[menu_level-1]=&mBoldI;
    n = Simple_Menu_Proc(&mBoldI);
    if (n)
    {
        rysuj_obiekt(object_info_ad, COPY_PUT,0);
        switch (n)
        {
            case 1:  //yes
                ((TEXT *) object_info_ad)->bold = 1;
                //menu update
                menu_par_new((*mInfoAboutA.pola)[menu_n].txt, _YES__);
                if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
                break;
            case 2:   //no
                ((TEXT *) object_info_ad)->bold = 0;
                //menu update
                menu_par_new((*mInfoAboutA.pola)[menu_n].txt, _NO__);
                if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
                break;
        }
        ((TEXT *) object_info_ad)->width=0.0;
        ((TEXT *) object_info_ad)->height=0.0;
        Change = TRUE;
        rysuj_obiekt(object_info_ad, COPY_PUT,1);
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_underline_t (void)
{
    int n;

    frame_off(&mInfoAboutA);

    menu_level++;
    menu_address[menu_level-1]=&mUnderlinedI;
    n = Simple_Menu_Proc(&mUnderlinedI);
    if (n)
    {
        rysuj_obiekt(object_info_ad, COPY_PUT,0);
        switch (n)
        {
            case 1:  //yes
                ((TEXT *) object_info_ad)->underline = 1;
                //menu update
                menu_par_new((*mInfoAboutA.pola)[menu_n].txt, _YES__);
                if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
                break;
            case 2:   //no
                ((TEXT *) object_info_ad)->underline = 0;
                //menu update
                menu_par_new((*mInfoAboutA.pola)[menu_n].txt, _NO__);
                if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
                break;
        }
        ((TEXT *) object_info_ad)->width=0.0;
        ((TEXT *) object_info_ad)->height=0.0;
        Change = TRUE;
        rysuj_obiekt(object_info_ad, COPY_PUT,1);
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}

void ch_spacing_t (void)
{
    int n;
    int i_spacing;
    char sk[64];

    frame_off(&mInfoAboutA);

    Test_Menu (&mLineSpacing);

    i_spacing=((TEXT *) object_info_ad)->spacing/2;
    if ((mLineSpacing.maxw==0) || (mLineSpacing.maxw>i_spacing))
    {
        mLineSpacing.foff=0;
        mLineSpacing.poz=i_spacing;
    }
    else
    {
        mLineSpacing.foff=i_spacing - mLineSpacing.maxw + 1;
        mLineSpacing.poz=mLineSpacing.maxw-1;
    }

    menu_level++;
    menu_address[menu_level-1]=&mLineSpacing;
    n = Simple_Menu_Proc(&mLineSpacing);
    if (n)
    {
        i_spacing=n-1;
        rysuj_obiekt(object_info_ad, COPY_PUT,0);

        ((TEXT *) object_info_ad)->spacing = (unsigned char)(i_spacing*2);

        sprintf(sk, "%.1f", (float) (((TEXT *)object_info_ad)->spacing/10.0+1.0));
        menu_par_new((*mInfoAboutA.pola)[menu_n].txt, sk) ;

        ((TEXT *) object_info_ad)->width=0.0;
        ((TEXT *) object_info_ad)->height=0.0;

        if (PTRS__Text_Style[((TEXT*)object_info_ad)->czcionka]->type == 2) TTF_redraw=TRUE;
        Change = TRUE;
        rysuj_obiekt(object_info_ad, COPY_PUT,1);

        if ((mLineSpacing.maxw==0) || (mLineSpacing.maxw>i_spacing))
        {
            mLineSpacing.foff=0;
            mLineSpacing.poz=i_spacing;
        }
        else
        {
            mLineSpacing.foff=i_spacing - mLineSpacing.maxw + 1;
            mLineSpacing.poz=mLineSpacing.maxw-1;
        }
    }
    baronoff_(&mInfoAboutA);
    baronoff(&mInfoAboutA);
    return;
}


BOOL edit_param_no0[48] = { 0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1, 1,1,1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0};

BOOL edit_param_no[48] = { 0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1, 1,1,1, 1, 1, 1, 1,1, 1, 1, 1,1, 1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0};

static void (* COMNDInfo[])(void)={nooop_, ch_layer, ch_color, ch_ltype, ch_lwidth, ch_ptype, ch_stype,ch_x1, ch_y1, ch_x2, ch_y2, ch_x3, ch_y3, ch_x4, ch_y4,
                                   ch_radius, ch_radiusy, ch_angle, ch_angle1, ch_angle2, ch_width1, ch_width2, ch_magnitude1, ch_magnitude1, ch_magnitude2,ch_axis1, ch_axis2, ch_load_ch,  ch_opacity,
                                   nooop_, ch_dx, ch_dy, nooop_, ch_font_t, ch_type_t, ch_hidden_t, ch_adjust_t, ch_heigh_t,
                                   ch_width_t, ch_italic_t, ch_bold_t, ch_underline_t, ch_spacing_t, ch_dx_pcx, ch_dy_pcx, nooop_, nooop_};


int  getwsp2(TMENU *menu, char *ad, int param_no[])
{
    int n;
    int no_param;

    while(1)
    { if((n=getwsp2_(menu))==0)
            if(Error) { ClearErr(); continue; }
        if(Info) { ClearInfo(); continue; }

        if (n==0)  break;

        no_param=param_no[n-1];
        if (edit_param_no[no_param]==1)
        {
            //edit parameter
            object_info_ad=ad;
            return_menu_par0((*mInfoAboutA.pola)[n-1].txt, sk0_info) ;
            return_menu_par((*mInfoAboutA.pola)[n-1].txt, sk_info) ;
            sk1_info=sk_info;
            while ((strlen(sk1_info)>0) && (sk1_info[0]=='\040'))
            {
                sk1_info+=1;
            }
            menu_n=n-1;

            (*COMNDInfo[no_param])();
        }

    }

    return n;
}

void  restorow(void)
{TMENU  *menu;
 for(menu=ACTW,ACTW=NULL;menu!=NULL;menu=menu->next)
     openw(menu);
}

/*--------------------------------------------*/

EVENT *GetEvent(TMENU *menu)
{ int n;
  while(1)
  { 
 	  if ((n = getwsp(menu)) == 0)   //n=0 -> esc
	  if (Error) { ClearErr(); continue;}
	  if (Info) { ClearInfo(); continue;}
	  break;  
  }
  Event.Number=n;
  return &Event;
}

/*-------------------------------------------*/

static void ini_simple_menu (char typ)
//------------------------------------
{
  static void (*mv)(int x, int y) ;
  static int (*ptr__KLS)(void);
  int n1;
  switch (typ)
  {
    case 0 :
      opwin_s = 1 ;
      typokn =  1 ;
      skokm = SKOKM_VER ; 
      ptr__KLS=KLS;        KLS = kls ;
      global_menu_flags = Get_Global_Menu_Flags () ;
      SW [0] = SERV [72] ;   SERV [72] = (void*)cprev ;
      SW [1] = SERV [80] ;   SERV [80] = (void*)cnext ;
      SW [2] = SERV [71] ;   SERV [71] = (void*)cbegin ;
      SW [3] = SERV [73] ;   SERV [73] = (void*)cpgprev ;
      SW [4] = SERV [79] ;   SERV [79] = (void*)cend ;
      SW [5] = SERV [81] ;   SERV [81] = (void*)cpgnext ;
      SW [6] = SERV [75] ; SERV [75] = noop ;
      SW [7] = SERV [77] ; SERV [77] = noop ;
	  SW[8] = SERV[82]; SERV[82] = noop;

	  SERV[172] = (void*)cprev_min;
	  SERV[180] = (void*)cnext_min;
	  SERV[177] = noop;
	  SERV[175] = noop;

      /*wylaczenie klawiszy funkcyjnych*/
  
      for (n1 = 0; n1 < SVMAX-84 ; n1++)
         {
			SW[n1 + 9] = SERV[n1 + 84];
			SERV[n1 + 84] = noop;
         }
      SW [100] = SERV [47];
      SERV [47] = noop;

      SW [101] = SERV [58];
      SW [102] = SERV [59];
      SW [103] = SERV [60];
      SW [104] = SERV [61];
      SW [105] = SERV [62];
      SW [106] = SERV [63];
      SW [107] = SERV [64];

      SERV [58] = noop;
      SERV [59] = noop;
      SERV [60] = noop;
      SERV [61] = noop;
      SERV [62] = noop;
      SERV [63] = noop;
      SERV [64] = noop;

	  SERV[135] = (void*)cprev_min_count; //nooop1;
	  SERV[134] = (void*)cnext_min_count; //nooop1;
	  mouse_dz = 1;
               
      mv = MSERV ;     
      if (!BAR_POINTER) MSERV = usk;
      else MSERV = pusk;
 

      break ;
     case 1 :
      opwin_s = 0 ;
      MSERV = mv ;
      KLS = ptr__KLS ;
      SERV [72] = SW [0] ;
      SERV [80] = SW [1] ;
      SERV [71] = SW [2] ;
      SERV [73] = SW [3] ;
      SERV [79] = SW [4] ;
      SERV [81] = SW [5] ;
      SERV [75] = SW [6] ;
      SERV [77] = SW [7] ;
      SERV [82] = SW [8] ;
      /*wlaczenie klawiszy funkcyjnych*/

      for(n1 = 0 ;n1 < SVMAX-84 ; n1++)
        {
          SERV [n1+84] = SW [n1+9];
        }
      SERV [47] = SW [100];

      SERV [58] = SW [101];
      SERV [59] = SW [102];
      SERV [60] = SW [103];
      SERV [61] = SW [104];
      SERV [62] = SW [105];
      SERV [63] = SW [106];
      SERV [64] = SW [107];

	  SERV[134] = (void*)SkalaZ_Minus; //nooop1;
	  SERV[135] = (void*)SkalaZ_Plus; //nooop1;
	  mouse_dz = 2;

      break ;
  }
}

int Simple_Menu_Proc (TMENU *menu)
//--------------------------------
{
  EVENT *ev ;
  int x0, y0, x00, y00;

    if (dynamic_menu==TRUE)
    {
        x00 = pikseleX0(X) +20;
        y00 = pikseleY0(Y) +get_pYk() - 24;

        if (menu->flags>=0)
        {
            x0=x00/(WIDTH *SKALA);

            if (x0<1) x0=1;
            if (menu_level>1)
                menu->xpcz=x0+(menu_level*4);
            else menu->xpcz=x0;
        }
        if (menu->flags&ICONS)
        {
            if (menu->maxw==0)
                y0 = (y00 - menu->max*(32))/32;
            else
                y0 = (y00 - menu->maxw*(32))/32;
        }
        else
        {
            if (menu->maxw==0) y0=(y00/(HEIGHT*SKALA)) - menu->max - 1;
            else y0=(y00/(HEIGHT*SKALA)) - menu->maxw - 1;
        }
        if (y0<3) y0=3;
        if (menu_level>1) menu->ypcz=y0+1; else menu->ypcz=y0;
    }

    menu->flags = menu->flags | NAWOPEN;

  menu_address[menu_level - 1] = menu;

  show_mouse(NULL);
  openwh (menu) ;
  show_mouse(screen);

  show_mouse(NULL);
  ini_simple_menu (0) ;
  show_mouse(screen);

  show_mouse(NULL);
  baron (menu) ;
  show_mouse(screen);

  while (1)
  {
      if (BAR_POINTER) set_cursor_pointer();
      if (!BAR_POINTER) lock_mouse();
      ev = GetEvent (menu) ;
      if (!BAR_POINTER) _free_mouse();

      show_mouse(NULL);
      closewh (menu) ;
      show_mouse(screen);
      show_mouse(NULL);
      ini_simple_menu (1) ;
      if (editing_text)
      {
          show_mouse(screen);
          set_cursor_edit();
      }

      return  ev->Number == 0 ? 0 : menu->foff + menu->poz + 1 ;
  }
}

int Simple_Menu_Proc_(TMENU *menu)
//--------------------------------
{
	EVENT *ev;
	int x0, y0, x00, y00;
	int n;

	if (menu == NULL) return  (COMNDmnr-1);
	if (dynamic_menu == TRUE)
	{
		x00 = pikseleX0(X) + 20;
		y00 = pikseleY0(Y) - 24;

		if (menu->flags >= 0)
		{
			x0 = x00 / (WIDTH*SKALA);

			if (x0 < 1) x0 = 1;
			if (menu_level > 1)
				menu->xpcz = x0 + (menu_level * 4);
			else menu->xpcz = x0;
		}
		if (menu->flags&ICONS)
		{
			if (menu->maxw == 0) y0 = (y00 / (32)) - menu->max - 1;
			else y0 = (y00 / (32)) - menu->maxw - 1;
		}
		else
		{
			if (menu->maxw == 0) y0 = (y00 / (HEIGHT*SKALA)) - menu->max - 1;
			else y0 = (y00 / (HEIGHT*SKALA)) - menu->maxw - 1;
		}
		if (y0 < 3) y0 = 3;
		if (menu_level > 1) menu->ypcz = y0 + 1; else menu->ypcz = y0;
	}

	n=getwsp1_(menu);

	return n;

	openw(menu);
	while (1)
	{
		ev = GetEvent(menu);
		if (ev->What == evKeyDown &&
			(ev->Number == 0 || ev->Number == ENTER))
		{
            closew(menu);
			return  ev->Number == 0 ? 0 : menu->foff + menu->poz + 1;
		}
	}
}

int GET_CONTROL_KEYS(int key)
{
	if (key < 32) return CONTROL_KEYS[key];
	else return key;
}

#undef __B_MENU__re