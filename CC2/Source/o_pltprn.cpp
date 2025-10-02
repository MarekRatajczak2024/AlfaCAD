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

#include "forwin.h"
#include <math.h>
#include <string.h>
#ifndef LINUX
#include <io.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <bios.h>
#define ALLEGWIN
#include "allegext.h"
#include <values.h>
#include "bib_e.h"
#include "alffont.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "o_osnap.h"
#include "o_object.h"
#include "o_editst.h"
#include "b_textw.h"
#include "o_pltprn.h"
#include "o_prn.h"
#include "jpgalleg.h"
#include "hpdf.h"

#pragma optimize("agp",off)

#define min(a,b)    (((a) < (b)) ? (a) : (b))
#define max(a,b)    (((a) > (b)) ? (a) : (b))

#define w1_5 1.5

BOOL PIN_IS_PRINTABLE=FALSE;

extern int MakeGrayColor(int kolor);
extern int MakeColor(int kolor);
extern T_PTR_Prn_Ini_Date *get_ptrs__prn_ini_date(void);
extern double get_condensed_wsp(void);
#define COLOR_BLACK 16

double df__width_fill_pcx;  /*grubosc linii dla wypelnien*/

extern void Draw_TTF_text_to_matrix(TEXT *t, char *zn, double x01, double y01, double font_scale, int font_index, int kolor0, BOOL bw, BOOL grey, BOOL index_pos);
extern void Init_TTF_text_to_matrix(TEXT *t, double font_scale, int font_index, int kolor256, BOOL bw, BOOL grey);
extern void Amend_TTF_text_to_matrix(TEXT *t, double font_scale, int font_index);
extern void Draw_TTF_char_to_matrix(TEXT *t, unsigned int unicode, double x01, double y01, double font_scale, int font_index, int kolor256, BOOL bw, BOOL grey);


extern BITMAP *allegro_prn_bmp;
extern int units_to_prn_x(double jednostkiX, double jednostkiY);
extern int units_to_prn_y(double jednostkiX, double jednostkiY);
extern int units_h_to_byte_prn(double jednostkid);
extern int units_y_to_prn(double jednostkid);
extern int units_x_to_prn(double jednostkid);

extern unsigned char *GetBMPpallete(void);

extern void Draw_Solid_Pattern(int numpoints, double* xy, char* s_pattern, int t_printer, int bw, int grey, float origin_x, float origin_y);
extern void Draw_Solidarc_Pattern(int numpoints, double* xy, char* s_pattern, int t_printer, int bw, int grey, float origin_x, float origin_y);
extern BOOL Draw_Line_To_Prn_Pattern(double x1, double y1, double x2, double y2);
extern void Draw_Solid_Gradient(int numpoints, float *xy, int bw, int grey, unsigned char translucency, GRADIENT *gradient, Print_Rect *window_to_print);


extern double get_matrix_head_xp(void);
extern double get_matrix_head_xk(void);

extern double get_matrix_head_yp(void);
extern double get_matrix_head_yk(void);

extern double get_matrix_head_width(void);
extern double get_matrix_head_height(void);

extern int jednostki_w_to_bit_prn(double jednostkid);
extern int jednostki_h_to_bit_prn(double jednostkid);

extern void line_prn_translucent(double x01, double y01, double x02, double y02, int color);
extern int get_Allegro_color(int kolor256, BOOL bw, BOOL grey);

extern void jednostki_to_prn_xy(int numpoints, double *xy, int* pxy);
extern void jednostki_to_prn_xy_l(int numpoints, double *xy, long* pxy);
extern BOOL draw_lined_solid_to_drive(double* xy);

extern void line_prn_trans(double x01, double y01, double x02, double y02, int kolor256, double text_line_width, BOOL bw, BOOL grey);
extern BOOL Draw_Line_To_Prn_trans(double x1, double y1, double x2, double y2, int kolor0, double text_line_width, BOOL bw, BOOL grey);

extern byte_ get_grey_intensity(COLOR_ color);
extern int get_width_line_ (BOOL b_hor, double text_line_width);
extern int get_pdf_left_margin(void);
extern int get_pdf_top_margin(void);
extern int get_pdf_page_height(void);
extern void CirclePDF(int x0, int y0, float r, int width, COLOR_ color);
extern void ArcPDF(int x0, int y0, int r, double a1, double a2, int width, COLOR_ color, HPDF_LineCap cap);
extern void EllipsePDF(int x0, int y0, int rx, int ry, double angle, int width, COLOR_ color, HPDF_LineCap cap);
extern void FilledEllipsePDF(int x0, int y0, int rx, int ry, double angle, COLOR_ color, HPDF_LineCap cap, unsigned char translucency);
extern void FilledSolidPDF(int numpoints, int *xy, COLOR_ color, HPDF_LineCap cap, unsigned char translucency);

extern int line_width_type;
extern int Line_Width (int width_style);
extern int Solid_Line_Width (int solid_width_style);

extern "C" {

    extern void Clip_Solid (int num_we, T_PixelTVal  *poly_we, int *num_wy, int *poly_wy) ;
    extern BOOL Clip_Solid0 (int num_we, T_PixelTVal  *poly_we, int *num_wy, int *poly_wy) ;
    extern void clipPolygonWithColors(long *xy, unsigned int *colors, int n_vertices, int xmin, int ymin, int xmax, int ymax, int *new_xy, unsigned int *new_colors, int *outCount);


    extern void getimage(int left, int top, int right, int bottom, BITMAP* bitmap);
	
	void oknoS_(double x1, double y1, double x2, double y2);
	BOOL draw_rectangle_to_drive(double* xy, double dist, int kolor0, BOOL bw, BOOL grey, int translucency, int colorB);
	extern int32_t utf8_to_ucs2(const uint8_t * input, const uint8_t ** end_ptr);
	extern unsigned int u8toMazovia(unsigned int int_code);
	extern int my_getch(void);
	extern int my_kbhit(void);
	
	extern int qsort_by_val(const void *e1, const void *e2);
	static int nr_pola;
	
	int type__drive;
	static int type__printer;

	static double X1, Y1, X2, Y2; 		/*wsp. okna clip.*/

	static double d__luk, min__dist;
	static double df__width_line;	/* grubosc linii, dla drukarki > 0, dla plotera = 0*/
	static double df__width_fill;  /*grubosc linii dla wypelnien*/

	static BOOL draw_line_to_drive(double x1, double y1, double x2, double y2, int kolor0, double text_line_width, BOOL bw, BOOL grey);
	static BOOL draw_line_to_drive1(double x1, double y1, double x2, double y2, int kolor0, double text_line_width, BOOL bw, BOOL grey);

	static BOOL draw_wzorzec(double x0, double y0, double len,
		double si, double co, int typ, int kolor0, BOOL cont, BOOL draw_end, BOOL bw, BOOL grey);
	static void ini_arc(void);
	int  line_clip(double *x1, double  *y1, double  *x2, double *y2);
	int  point_clip(float x, float y);
	static BOOL draw_arc_drv(double, double, double, double, double, int, int, BOOL, BOOL);
	static BOOL linen_plt(double x1, double y1, double x2, double y2, int typ, int kolor0, double text_line_width, BOOL bw, BOOL grey);
    static BOOL draw_ellipse_drv(double Xr, double Yr, double Rx, double Ry, double Angle, int typ, int kolor0, BOOL bw, BOOL grey);
    static BOOL draw_filledellipse_drv(double Xr, double Yr, double Rx, double Ry, double Angle, int kolor0, unsigned char translucency, BOOL bw, BOOL grey);
    static BOOL draw_ellipticalarc_drv(double Xr, double Yr, double Rx, double Ry, double Angle, double kat1, double kat2, int typ, int kolor0, BOOL bw, BOOL grey);
	static BOOL linen_plt1(double x1, double y1, double x2, double y2, int typ, int kolor0, double text_line_width, BOOL bw, BOOL grey);
	static BOOL linen_plt2(double x1, double y1, double x2, double y2, int typ, int kolor0, double text_line_width, BOOL bw, BOOL grey);
	static BOOL prn_ok(int status);
	static BOOL get_prn_status(BOOL);
	static void get_line_type_ext(int, double *, double *);
    static BOOL draw_border_solidarc_to_drive(SOLIDARC *sa, int type__drive);
	static BOOL draw_border_solid_to_drive(WIELOKAT *w);
	static BOOL draw_border_solid_to_drive_null(WIELOKAT *w, int opcja);
	int Write_To_Device(void *buf, unsigned int len);


    //void set_trace_block_begin(char *adr);
    //void set_trace_block_end(char *adr);
    //void set_trace_block(BOOL tb);

	extern int Draw_Rectangle_To_Plt(double x1, double y1, double x2, double y2);
	extern void calc_bspline(int lp, int n, float points[], int p1, float *out_x, float *out_y);
	
	extern double INDEX_FACTOR;

	extern int Get_W_Matix_Len(int i_font);
	extern int Get_W_Matix_Len_TTF(int i_font, int height);
    extern int Get_Char_Matix_Len_TTF(unsigned int chr, int i_font, float height);
	extern double Get_TTF_Char_Left_Top(TEXT *ptrs_text, char *ptrsz_t, double font_scale, int font_index0, int *TTF_left, int *TTF_top);
	extern void Get_Face_Character_yMax(TEXT *t, int *yMax);
	extern void Init_Draw_TTF_text(TEXT *t, double kat, float wysokosc_p, COLOR_ kolor);
	extern char *alft;
	extern int Get_TTF_Char_Outline(TEXT *t, unsigned int unicode, char *alf, long *lw, int yMax);
	extern BITMAP *load_memory_pcx(AL_CONST void *buffer, PALETTE *pal);

	extern int TRANSLUCENCY;
    int PRINT_TRANSLUCENCY=255;
	extern void set_mode_solid();

	extern NODE bnodes[MAX_NODES];
	extern double curviness;
	extern void calc_tangents(int closed, int* node_count, double* bnodes_2_tangent);
	extern void set_bnodes(SPLINE *ptrs_spline, float *ad_xy, int *node_count_);
	extern void get_control_points_reversed(NODE n1, NODE n2, float points[8]);
	extern void get_control_points(NODE n1, NODE n2, float points[8]);
	extern void get_control_points_end(NODE n1, NODE n2, float points[8]);

	extern void set_mode_trans();
	extern void set_mode_solid();

	extern void set_mode_trans_prn(BITMAP* prn_bitmap);
	extern void set_mode_solid_prn(BITMAP* prn_bitmap);

	extern int compare_vectors(p_vector* vector1, p_vector* vector2);
	extern BOOL move_vector_inside(T_PixelTVal* PolyPoints, int v_number, int flag);

	extern void gray_bitmap(BITMAP* src, BITMAP* dst, int dx, int dy, int gray_sat);

	extern p_point compute2DPolygonCentroid2(T_PixelTVal* vertices, int vertexCount);
	extern p_point perpend_intersect(long x1, long y1, long x2, long y2, p_point center);

	extern unsigned char _dac_normal[256][3];

    extern double my_round(double x, unsigned int digits);

    extern int arc_clip11 (LUK *l_clip, LUK *l1_clip, LUK *l2_clip, LUK *l3_clip, double x1, double y1, double x2, double y2);

    extern int arc_clip1_xx(LUK *l_clip, LUK *l1_clip, LUK *l2_clip, LUK *l3_clip, LUK *l4_clip, double x1, double y1, double x2, double y2);
    extern int arc_clip1_yy(LUK *l_clip, LUK *l1_clip, LUK *l2_clip, LUK *l3_clip, LUK *l4_clip, double x1, double y1, double x2, double y2);

    extern double INDEX_SHIFT;

    extern int Get_Ellipse_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double xy[]);
    extern int Get_EllipticalArc_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double kat1, double kat2, double xy[]);


    extern void my_soft_polygon(BITMAP* bmp, int vertices, AL_CONST int* points, int color, int translucency, int colorB);
    extern void my_fillpoly(int numpoints, AL_CONST int* polypoints, int translucency, int kolory_paper);;

    extern double Double_Normal (double param);

    extern int solidarc_elements(SOLIDARC *sa, LINIA *L_left, LINIA *L_right, LUK *l_inner1, LUK *l_inner2, LUK *l_outer1, LUK *l_outer2);
    extern int solidfill_typ[8];
    extern int rysuj_solidarc_factory(SOLIDARC *sa, int mode, int kolor, int side, int in, double *xy, BOOL printer, int *n1, int *n2);

    extern double Get_Point_Size (void);
    extern double normalize_txt_angle(double angle0);
    extern double dim_precision;
    extern double thermal_precision;
    extern double load_precision;
    extern double force_precision;
    extern double moment_precision;
    extern double displacement_precision;
    extern double rotation_precision;
    extern void set_decimal_format(char *text, double l, double precision);
    extern void normalize_txt(TEXT *ptrs_text);
    extern double depth_magnitude; //units per mm  default 1 mm of section depth per 1 mm on drawing paper
    extern double thermal_magnitude; //units per mm  default 1 Celsius per 1 mm on drawing paper
    extern double radius_magnitude; //units per mm  default 1 Celsius per 1 mm on drawing paper
    extern double load_magnitude; //units per mm  default 10kN/m force per 1 mm on drawing paper
    extern double flood_magnitude; //units per mm  default 10kN/m² load per 1 mm on drawing paper
    extern double force_magnitude; //units per mm  default 10kN force per 1 mm on drawing paper
    extern double moment_magnitude; //units per mm  default 10kNm force per 1 mm radius on drawing paper
    extern double displacement_magnitude; //units per mm  default 1 mm desplacement per 1 mm on drawing paper
    extern double rotation_magnitude; //units per mm  default 0.001 rad desplacement per 1 mm radius on drawing paper
    extern void create_solid_on_line (LINIA *L, WIELOKAT *s, double width1, double width2, double axis);
    extern double measure_vector (float x1, float y1, float x2, float y2, BOOL b_first_end, double df_l0, double df_dl, double *df_x, double *df_y);
    extern double measure_arcvector (LUK *ptrs_arc, BOOL b_first_end, double df_l0, double df_dl, double *df_x, double *df_y);
    extern point intersectionPoint(point a1,point a2,point b1,point b2);
    extern void get_solidarc_ends(SOLIDARC *sa, double *xy);

    extern char *load_symbol[];

	typedef enum { PRN_PCX = 0, PRN_WINDOWS, PRN_PDF, PRN_9, PRN_24, PRN_48, PRN_LASER, PRN_ESC_P2 } PRN;

void draw_ttf_text_to_drive(TEXT *t, char *zn, double x01, double y01, double font_scale, int font_index, int kolor0, BOOL bw, BOOL grey, BOOL index_pos);
void draw_ttf_char_to_drive(TEXT *t, unsigned int unicode, double x01, double y01, double font_scale, int font_index, int kolor0, BOOL bw, BOOL grey);
void init_ttf_text_to_drive(TEXT *t, double font_scale, int font_index, int kolor0, BOOL bw, BOOL grey);
void amend_ttf_text_to_drive(TEXT *t, double font_scale, int font_index);
void sort_solid_xy_points(int* xy, int* xys, int lp, int *lps);
BOOL linen_plt_pattern(double x1, double y1, double x2, double y2);
void reset_last_polygon_vectors(void);
BOOL linen_plt_trans(double x1, double y1, double x2, double y2, int typ, int kolor0, double text_line_width, BOOL bw, BOOL grey);

#define LAST_POLYGONS_NO 4

static p_vector first_polygon_vector[LAST_POLYGONS_NO] = {0};
static p_vector last_polygon_vector[LAST_POLYGONS_NO] = {0};
static T_PixelTVal last_polygon[LAST_POLYGONS_NO][8] = { 0 };

static char *trace_block_begin = NULL, *trace_block_end=NULL;
static BOOL trace_block=FALSE;
static POINTF last_trace_point[2], next_trace_point[2];
/***************  GLOBAL DECLARATION  *****************************/


typedef COLOR_ COLOR[256];

typedef char T768[768];

void set_trace_block_begin(char *adr)
{
    trace_block_begin=adr;
}

void set_trace_block_end(char *adr)
{
    trace_block_end=adr;
}

void set_trace_block(BOOL tb)
{
    trace_block = tb;
}

double get_d__luk(void)
{
    return d__luk;
}

BOOL check_concave(int *xy, int n)
{
	BOOL concave = FALSE;

	if (n == 4)
	{
		p_point A, B, C, D;
		int sign_p[3];
		int sum_signs;
		int i;

		A.x = xy[0];
		A.y = xy[1];

		B.x = xy[2];
		B.y = xy[3];

		C.x = xy[4];
		C.y = xy[5];

		D.x = xy[6];
		D.y = xy[7];


		sign_p[0] = ((B.x - A.x) * (C.y - B.y) - (B.y - A.y) * (C.x - B.x));
		sign_p[1] = ((C.x - B.x) * (D.y - C.y) - (C.y - B.y) * (D.x - C.x));
		sign_p[2] = ((D.x - C.x) * (A.y - D.y) - (D.y - C.y) * (A.x - D.x));

		sum_signs = 0;
		for (i = 0; i < 3; i++) sum_signs += sign_p[i] < 0 ? -1 : 1;

		if ((sum_signs != 3) && (sum_signs != -3)) concave = TRUE;
	}

	return concave;
}


void oknoS_(double x1, double y1, double x2, double y2)
{
	
	oknoS(x1, y1, x2, y2);
}

void Set_Line_Width (double df_width)
{
  df__width_line = df_width ;
}

void Set_Type_Drive (int type, int printer, double dluk, double mindist, double widthfill)
/*--------------------------------------------------------------------------*/
{
  type__drive = type;	/* drukarka, ploter*/
  type__printer = printer; /*image, Windows*/
  d__luk = dluk;        	/* dlugosc odcinka luku*/
  min__dist = mindist;	/* minimalny krok (wielkosc kropki)*/
  df__width_fill = widthfill ; /*grubosc pisaka do wypelnien*/
  df__width_fill_pcx = widthfill;
}


void Set_Draw_Window (double x1, double y1, double x2, double y2)
/*------------------------------------------------------------*/
{
   X1 = x1;
   X2 = x2;
   Y1 = y1;
   Y2 = y2;

   //oknoS(X1, Y1, X2, Y2);  //!!!
}

void Change_Set_Draw_Window (double dx1, double dy1, double dx2, double dy2)
/*------------------------------------------------------------------------*/
{
   if (X1<dx1) X1 = dx1;
   if (X2>dx2) X2 = dx2;
   if (Y1<dy1) Y1 = dy1;
   if (Y2>dy2) Y2 = dy2;
}

void Get_Draw_Window (double *x1, double *y1, double *x2, double *y2)
/*-----------------------------------------------------------------*/
{
   *x1 = X1;
   *x2 = X2;
   *y1 = Y1;
   *y2 = Y2;
}


BOOL Draw_Fill_Circle_To_Drive (OKRAG* ptr_circle)
/*-----------------------------------------------*/
{
  double Xp1,Yp1,Xp2,Yp2;
  double sta, ea ;
  double Xr, Yr, R ;
  double da, dda;	/*kat rozwarcia luku, kat podzialu luku*/
  double si, co; 	/*sinus i cosinus kata podzialu luku*/
  long N;		/*liczba 'bokow' luku*/
  long k;
  long Num; 		/*liczba 'bokow' podzialu okregu (zalezy od promienia)*/
  double xdiff, ydiff;	/*pomocnicze*/
  double df_dist ;
  double xy [8] ;
  int kolor, kolor0;
  int translucency = 255;
  int colorB = 0xFFFFFFFF;
  double Xp0,Yp0;
  int x0, x00, y0, y00;
  int r;
  COLOR_ kolorR;
  BOOL bw, grey;
  byte_ intensity;
  int i_width;
  LUK l;

  T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_;

    if ( type__drive == PRN_DRIVE)
    {
        ptrs__prn_ini_date_ = get_ptrs__prn_ini_date();
        if ((*ptrs__prn_ini_date_)->prn_type == PRN_PDF)
        {
            if (print_inversion == TRUE)
            {
                if (print_rotation == TRUE)
                {
                    //// if reflection
                    Xp0 = FormatX_S - ptr_circle->x;
                    Yp0 = FormatY_S - ptr_circle->y;
                }
                else
                {
                    Xp0 = FormatX_S - ptr_circle->x + FormatX_S0;  //this is for reflection
                    Yp0 = FormatY_S + FormatY_S0 - ptr_circle->y;
                }
            }
            else
            {
                Xp0=ptr_circle->x;
                Yp0=ptr_circle->y;
            }


            x00 = units_to_prn_x(Xp0, Yp0);
            y00 = units_to_prn_y(Xp0, Yp0);
            r = units_y_to_prn(ptr_circle->r);

            kolor0 = MakeColor(GetColorWin(ptr_circle->kolor));

            kolorR.red = _dac_normal[kolor0][0] << 2;
            kolorR.gre = _dac_normal[kolor0][1] << 2;
            kolorR.blu = _dac_normal[kolor0][2] << 2;

            bw=Layers[ptr_circle->warstwa].bw;
            grey=Layers[ptr_circle->warstwa].grey;

            if (((*ptrs__prn_ini_date_)->color_print == 1) && (bw == 0)) {
                if (((*ptrs__prn_ini_date_)->gray_print == 1) || (grey == 1)) {
                    intensity = get_grey_intensity(kolorR);
                    kolorR = {intensity, intensity, intensity};
                }
            } else {
                if ((kolorR.red < 254) && (kolorR.gre < 254) && (kolorR.blu < 254))
                    kolorR = {0, 0, 0};
            }

            i_width = get_width_line_(TRUE, 0);

            x0 = x00 + get_pdf_left_margin();
            y0 = get_pdf_page_height() - y00 - get_pdf_top_margin();

            if (r<1) return 1;


            FilledEllipsePDF(x0, y0, r, r, 0, kolorR, HPDF_ROUND_END, 255);

            return TRUE;

        }
    }


  reset_last_polygon_vectors();

  Xr = ptr_circle->x ;
  Yr = ptr_circle->y ;
  R = ptr_circle->r ;

  
  kolor = MakeColor(GetColorWin(ptr_circle->kolor));


  if ( R == 0 )
  {
      R = min__dist ;
  }
  df_dist = df__width_fill ;

  sta = Pi2 / 4 ;
  ea = sta + Pi2 / 2 ;
  da = ea - sta ;
  Num = Pi2 * R / d__luk;
  if (Num < 12) Num = 12;

  dda = Pi2 / Num ;
  si = sin (dda) ;
  co = cos (dda) ;
  N =  (int)(da / dda);
  Xp1 = Xr + R * cos (sta) ;
  Yp1 = Yr + R * sin (sta) ;
  xy [0] = Xp1 ;
  xy [1] = Yp1 ;
  xy [2] = Xp1 ;
  xy [3] = Yp1 ;
  for(k = 0; k < N; k++)
  {
     xdiff = Xp1 - Xr ;
     ydiff = Yp1 - Yr ;
     Xp2 = Xr + xdiff * co - ydiff * si ;
     Yp2 = Yr + xdiff * si + ydiff * co ;
     xy [4] = 2 * Xr - Xp2 ;
     xy [5] = Yp2 ;
     xy [6] = Xp2 ;
     xy [7] = Yp2 ;
     if (FALSE == draw_rectangle_to_drive (xy, df_dist, kolor, Layers[ptr_circle->warstwa].bw, Layers[ptr_circle->warstwa].grey, translucency, colorB)) return FALSE ;
    
     Xp1 = Xp2;
     Yp1 = Yp2;
     xy [0] = xy [6] ;
     xy [1] = xy [7] ;
     xy [2] = xy [4] ;
     xy [3] = xy [5] ;
  }
  Xp2 = Xr + R * cos (ea) ;
  Yp2 = Yr + R * sin (ea) ;
  xy [4] = 2 * Xr - Xp2 ;
  xy [5] = Yp2 ;
  xy [6] = Xp2 ;
  xy [7] = Yp2 ;
  if (FALSE == draw_rectangle_to_drive (xy, df_dist, kolor, Layers[ptr_circle->warstwa].bw, Layers[ptr_circle->warstwa].grey, translucency, colorB)) return FALSE ;


    l.warstwa=ptr_circle->warstwa;
    l.kolor=ptr_circle->kolor;
    l.typ = 32;
    l.x = ptr_circle->x;
    l.y = ptr_circle->y;
    l.r = ptr_circle->r;
    l.kat1 = 0;
    l.kat2 = Pi2;

    if (FALSE == Draw_Arc_To_Drive(&l))  return FALSE ;

  return TRUE;
}


BOOL Draw_Fill_Trans_Circle_To_Drive (OKRAG* ptr_circle, int translucency)
/*----------------------------------------------------------------------*/
{
    double Xp1,Yp1,Xp2,Yp2;
    double sta, ea ;
    double Xr, Yr, R ;
    double da, dda;	/*kat rozwarcia luku, kat podzialu luku*/
    double si, co; 	/*sinus i cosinus kata podzialu luku*/
    long N;		/*liczba 'bokow' luku*/
    long k;
    long Num; 		/*liczba 'bokow' podzialu okregu (zalezy od promienia)*/
    double xdiff, ydiff;	/*pomocnicze*/
    double df_dist ;
    double xy [8] ;
    int kolor, kolor0;
    int colorB = 0xFFFFFFFF;
    double Xp0,Yp0;
    int x0, x00, y0, y00;
    int r;
    COLOR_ kolorR;
    BOOL bw, grey;
    byte_ intensity;
    int i_width;

    T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_;

    if ( type__drive == PRN_DRIVE)
    {
        ptrs__prn_ini_date_ = get_ptrs__prn_ini_date();
        if ((*ptrs__prn_ini_date_)->prn_type == PRN_PDF)
        {
            if (print_inversion == TRUE)
            {
                if (print_rotation == TRUE)
                {
                    //// if reflection
                    Xp0 = FormatX_S - ptr_circle->x;
                    Yp0 = FormatY_S - ptr_circle->y;
                }
                else
                {
                    Xp0 = FormatX_S - ptr_circle->x + FormatX_S0;  //this is for reflection
                    Yp0 = FormatY_S + FormatY_S0 - ptr_circle->y;
                }
            }
            else
            {
                Xp0=ptr_circle->x;
                Yp0=ptr_circle->y;
            }


            x00 = units_to_prn_x(Xp0, Yp0);
            y00 = units_to_prn_y(Xp0, Yp0);
            r = units_y_to_prn(ptr_circle->r);

            kolor0 = MakeColor(GetColorWin(ptr_circle->kolor));

            kolorR.red = _dac_normal[kolor0][0] << 2;
            kolorR.gre = _dac_normal[kolor0][1] << 2;
            kolorR.blu = _dac_normal[kolor0][2] << 2;

            bw=Layers[ptr_circle->warstwa].bw;
            grey=Layers[ptr_circle->warstwa].grey;

            if (((*ptrs__prn_ini_date_)->color_print == 1) && (bw == 0)) {
                if (((*ptrs__prn_ini_date_)->gray_print == 1) || (grey == 1)) {
                    intensity = get_grey_intensity(kolorR);
                    kolorR = {intensity, intensity, intensity};
                }
            } else {
                if ((kolorR.red < 254) && (kolorR.gre < 254) && (kolorR.blu < 254))
                    kolorR = {0, 0, 0};
            }

            i_width = get_width_line_(TRUE, 0);

            x0 = x00 + get_pdf_left_margin();
            y0 = get_pdf_page_height() - y00 - get_pdf_top_margin();

            if (r<1) return 1;


            FilledEllipsePDF(x0, y0, r, r, 0, kolorR, HPDF_ROUND_END, translucency);

            return TRUE;

        }
    }

    reset_last_polygon_vectors();

    Xr = ptr_circle->x ;
    Yr = ptr_circle->y ;
    R = ptr_circle->r ;


    kolor = MakeColor(GetColorWin(ptr_circle->kolor));


    if ( R == 0 )
    {
        R = min__dist ;
    }
    df_dist = df__width_fill ;

    sta = Pi2 / 4 ;
    ea = sta + Pi2 / 2 ;
    da = ea - sta ;
    Num = Pi2 * R / d__luk;
    if (Num < 12) Num = 12;

    dda = Pi2 / Num ;
    si = sin (dda) ;
    co = cos (dda) ;
    N =  (int)(da / dda);
    Xp1 = Xr + R * cos (sta) ;
    Yp1 = Yr + R * sin (sta) ;
    xy [0] = Xp1 ;
    xy [1] = Yp1 ;
    xy [2] = Xp1 ;
    xy [3] = Yp1 ;
    for(k = 0; k < N; k++)
    {
        xdiff = Xp1 - Xr ;
        ydiff = Yp1 - Yr ;
        Xp2 = Xr + xdiff * co - ydiff * si ;
        Yp2 = Yr + xdiff * si + ydiff * co ;
        xy [4] = 2 * Xr - Xp2 ;
        xy [5] = Yp2 ;
        xy [6] = Xp2 ;
        xy [7] = Yp2 ;
        if (FALSE == draw_rectangle_to_drive (xy, df_dist, kolor, Layers[ptr_circle->warstwa].bw, Layers[ptr_circle->warstwa].grey, translucency, colorB)) return FALSE ;

        Xp1 = Xp2;
        Yp1 = Yp2;
        xy [0] = xy [6] ;
        xy [1] = xy [7] ;
        xy [2] = xy [4] ;
        xy [3] = xy [5] ;
    }
    Xp2 = Xr + R * cos (ea) ;
    Yp2 = Yr + R * sin (ea) ;
    xy [4] = 2 * Xr - Xp2 ;
    xy [5] = Yp2 ;
    xy [6] = Xp2 ;
    xy [7] = Yp2 ;
    if (FALSE == draw_rectangle_to_drive (xy, df_dist, kolor, Layers[ptr_circle->warstwa].bw, Layers[ptr_circle->warstwa].grey, translucency, colorB)) return FALSE ;

    return TRUE;
}

/*------------------------------------------*/
/*---Draw_Wielokat_To_Drive-------------------------------*/
/*------------------------------------------*/

#define BIGNOD 1e200
#define BIGNOF 1e20
#define BIGNOI 1e9

void sort_solid_xy_points(int *xy, int *xys, int lp, int *lps)
/*--------------------------------------------------------------*/
{
	int i, j, k;
	int x_max, y_max, y_max_l;
	BOOL b_found;

	y_max_l = BIGNOI;
	k = 0;
	for (i = 0; i < lp; i += 2)
	{
		y_max = -BIGNOI;
		b_found = FALSE;
		for (j = 0; j < lp; j += 2)
		{
			if (xy[j + 1] > y_max &&
				xy[j + 1] < y_max_l &&
				(i!=j))
			{
				b_found = TRUE;
				x_max = xy[j];
				y_max = xy[j + 1];
			}
		}
		if (b_found == TRUE)
		{
		    xys[k++] = x_max;
			xys[k++] = y_max;
			y_max_l = y_max;
		}
	}
	*lps = k;
	
}

static void sort_solid_point (WIELOKAT *ptr_w_s, WIELOKAT *ptr_w)
/*--------------------------------------------------------------*/
{
  int i, j, k ;
  double x_max ,y_max, y_max_l ;
  BOOL b_found ;

  
  y_max_l = BIGNOD ;
  k = 0 ;
  for (i = 0 ; i < (int)ptr_w->lp ; i += 2)
  {
    y_max = - BIGNOD ;
    b_found = FALSE ;
    for (j = 0 ; j < (int)ptr_w->lp ; j += 2)
    {
      if (ptr_w->xy [j + 1] > y_max  &&
	  ptr_w->xy [j + 1] < y_max_l  &&
	  FALSE == Check_if_Equal (ptr_w->xy [j + 1], y_max_l))
      {
		b_found = TRUE ;
		x_max = ptr_w->xy [j] ;
		y_max = ptr_w->xy [j + 1] ;
      }
    }
    if (b_found == TRUE)
    {
      ptr_w_s->xy [k++] = x_max ;
      ptr_w_s->xy [k++] = y_max ;
      y_max_l = y_max ;
    }
  }
  ptr_w_s->lp = k ;
}

static void intersect_solid (double x1, double y1, WIELOKAT *ptr_w,
			     double *x2, double *y2)
/*----------------------------------------------------------------*/
{
  double s1, s2, p1, p2 ;
  double t_hor, t_temp, x, y ;
  LINIA L_temp, L_hor ;
  int i ;

  L_hor. x1 = x1;
  L_hor. x2 = x1 + 10;
  L_hor. y1 = y1;
  L_hor. y2 = y1;
  *x2 = x1 ;
  *y2 = y1 ;
  s1 = ptr_w->xy [ptr_w->lp - 2] ; /*punk S jest ostatnim punktem*/
  s2 = ptr_w->xy [ptr_w->lp - 1] ;
  for (i = 0 ; i < (int)ptr_w->lp ; i += 2) 	/*dla kazdego wierzcholka*/
  {
    p1 = ptr_w->xy [i] ;	    	/*punkt P jest nastepny*/
    p2 = ptr_w->xy [i + 1] ;
    L_temp.x1 = s1 ;
    L_temp.y1 = s2 ;
    L_temp.x2 = p1 ;
    L_temp.y2 = p2 ;
    if (1 == przeciecieLL_tt (&x, &y, (void *)&L_temp, (void *)&L_hor, &t_temp, &t_hor) &&
	TRUE == Check_if_GE02 (t_temp, 0.0) &&
	TRUE == Check_if_GE02 (1.0, t_temp) &&
	FALSE == Check_if_Equal (x, x1))
    {
      *x2 = x ;
      *y2 = y ;
      break ;
    }
    s1 = p1 ;
    s2 = p2 ;
  }
}

void reset_last_polygon_vectors(void)
{
	int i,j;
	for (j = 0; j < LAST_POLYGONS_NO; j++)
	{
		first_polygon_vector[j] = { 0, 0, 0, 0 };
		last_polygon_vector[j] = { 0, 0, 0, 0 };	
		for (i = 0; i < 8; i++) last_polygon[j][i] = 0;
	}
}

void my_horizontal_polygon(int *xy, int kolorA)
{
	int xd1, xd2;
	double cl, cp;
	int xl, xp, y;
	int ymin, ymin0;
	int x1a, x2a;
	double angle_l, angle_p;
	double dxp, dxl;
	int x1 = xy[0], y1 = xy[1],
		x2 = xy[2], y2 = xy[3],
		x3 = xy[4], y3 = xy[5],
		x4 = xy[6], y4 = xy[7];
	double dist;

	dist = 1.0;

	angle_l = Atan2((double)(y4 - y1), (double)(x4 - x1));
	angle_p = Atan2((double)(y3 - y2), (double)(x3 - x2));
	dxl = dist * fabs(sin(angle_l));
	dxp = dist * fabs(sin(angle_p));

	if (fabs(y1 - y4) <= dist)
	{
		line(allegro_prn_bmp, min( min(x1,x2), min(x3, x4)), y1, max(max(x1, x2), max(x3, x4)), y2, kolorA);
		return;
	}

	x1a = x1 + dxl;
	x2a = x2 - dxp;
	cl = (double)(x4 - x1) / (double)(y4 - y1);
	cp = (double)(x3 - x2) / (double)(y3 - y2);
	ymin = y1;
	ymin0 = y4;


	if (ymin > ymin0)
	{
		ymin = y4;
		ymin0 = y1;
	}
	
	y = ymin;
	
	do
	{
		xl = x1a + (int)((double)(y - y1) * cl);
		xp = x2a + (int)((double)(y - y2) * cp);
		if (xl > xp)
		{
			xl = (xl + xp) / 2.0;
			xp = xl;
		}	
		line(allegro_prn_bmp, xl, y, xp, y, kolorA);
		y += dist;
	} while (y <= ymin0);
}


int compare_vectors_prn(p_vector* vector1, p_vector* vector2)
{
	if (((vector1->x1 == vector2->x1) && (vector1->y1 == vector2->y1) && (vector1->x2 == vector2->x2) && (vector1->y2 == vector2->y2)) ||
		((vector1->x1 == vector2->x2) && (vector1->y1 == vector2->y2) && (vector1->x2 == vector2->x1) && (vector1->y2 == vector2->y1))) return 0;
	else return 1;
}


BOOL move_vector_inside_prn(T_PixelTVal* PolyPoints, int v_number, int flag)
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

		if (flag)
		{
			PolyPoints[0] += fx;
			PolyPoints[2] += fx;
		}

		PolyPoints[1] += fy;
		PolyPoints[3] += fy;

		break;
	case 4:  //last vector

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
	default:
		break;
	}

	return TRUE;

}


BOOL draw_polygon_to_drive_trans(double* xy0, int numpoints, /*double dist,*/ int kolor0, BOOL bw, BOOL grey, unsigned char translucency, int colorB)
{
    int kolorA;
#define MAXPOINT 256
    T_PixelTVal lpxy[MAXPOINT];
    int pxy[MAXPOINT],pxy1[MAXPOINT],pxy2[MAXPOINT], pxy00[MAXPOINT];
    double ppxy[MAXPOINT];
    int found;
    BOOL ignore_p;
    int i;
    double xy[MAXPOINT];
    WIELOKAT w = S4def;
    BOOL ret;
    BOOL ret_move;
    int ii;
    T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_;
    COLOR_ kolorR;
    byte_ intensity;

    ptrs__prn_ini_date_ = get_ptrs__prn_ini_date();

    memcpy(&xy, xy0, sizeof(double) * numpoints);

    if (print_inversion == TRUE)
    {
        if (print_rotation == TRUE)
        {

            for (i=0; i<numpoints; i+=2)
            {
                ppxy[i] = FormatX_S - xy[i];
                ppxy[i+1] = FormatY_S - xy[i+1];
            }
        }
        else
        {
            for (i=0; i<numpoints; i+=2)
            {
                ppxy[i] = FormatX_S - xy[i] + FormatX_S0;
                ppxy[i+1] = FormatY_S + FormatY_S0 - xy[i+1];
            }
        }
    }
    else
    {
        if (print_rotation == TRUE)
        {

            for (i=0; i<numpoints; i++) ppxy[i] = xy[i];
        }
        else
        {

            for (i=0; i<numpoints; i++) ppxy[i] = xy[i];
        }
    }


    if ((type__drive == PRN_DRIVE) && ((*ptrs__prn_ini_date_)->prn_type == PRN_PDF))
    {
        for (i=0; i<numpoints; i+=2)
        {
            pxy00[i] = units_to_prn_x(ppxy[i], ppxy[i+1]) + get_pdf_left_margin();
            pxy00[i+1]= get_pdf_page_height() - units_to_prn_y(ppxy[i], ppxy[i+1]) - get_pdf_top_margin();
        }

        kolorR.red = _dac_normal[kolor0][0] << 2;
        kolorR.gre = _dac_normal[kolor0][1] << 2;
        kolorR.blu = _dac_normal[kolor0][2] << 2;

        if (((*ptrs__prn_ini_date_)->color_print == 1) && (bw == 0)) {
            if (((*ptrs__prn_ini_date_)->gray_print == 1) || (grey == 1)) {
                intensity = get_grey_intensity(kolorR);
                kolorR = {intensity, intensity, intensity};
            }
        } else {
            if ((kolorR.red < 254) && (kolorR.gre < 254) && (kolorR.blu < 254))
                kolorR = {0, 0, 0};
        }

        FilledSolidPDF(numpoints, pxy00, kolorR, HPDF_ROUND_END, translucency);

        return TRUE;
    }

    jednostki_to_prn_xy(numpoints, ppxy, pxy);


    ignore_p = FALSE;

    if (translucency < 255)
    {

        found = 0;

        for (i = 0; i < numpoints; i++)
            lpxy[i] = pxy[i];



        for (i = 0; i < numpoints; i++)
            pxy[i] = lpxy[i];

        set_trans_blender(0, 0, 0, translucency);
        set_mode_trans();
    }
    else set_mode_solid();

    kolorA = get_Allegro_color(kolor0, bw, grey);

    my_soft_polygon(allegro_prn_bmp, numpoints/2, pxy, kolorA, translucency, colorB);

    if (translucency < 255)
    {
        set_mode_solid();
    }

    return TRUE;
}


BOOL draw_rectangle_to_drive_trans(double* xy0, double dist, int kolor0, BOOL bw, BOOL grey, int translucency, int colorB)
{
	int kolorA;

	T_PixelTVal lpxy[8];
	int pxy[8],pxy1[8],pxy2[8],pxy00[8];;
	double ppxy[8];
	int found;
	BOOL ignore_p;
	int i;
	double xy[8];
	WIELOKAT w = S4def;
	BOOL ret;
	BOOL ret_move;
	int ii;
    T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_;
    COLOR_ kolorR;
    byte_ intensity;

    ptrs__prn_ini_date_ = get_ptrs__prn_ini_date();

	memcpy(&xy, xy0, sizeof(double) * 8);

	if (print_inversion == TRUE)
	{
		if (print_rotation == TRUE)
		{

            for (i=0; i<8; i+=2)
            {
                ppxy[i] = FormatX_S - xy[i];
                ppxy[i+1] = FormatY_S - xy[i+1];
            }

		}
		else
		{

            for (i=0; i<8; i+=2)
            {
                ppxy[i] = FormatX_S - xy[i] + FormatX_S0;
                ppxy[i+1] = FormatY_S + FormatY_S0 - xy[i+1];
            }
		}
	}
	else
	{
		if (print_rotation == TRUE)
		{

            for (i=0; i<8; i++) ppxy[i] = xy[i];
			
		}
		else
		{

            for (i=0; i<8; i++) ppxy[i] = xy[i];
		}
	}


    if ((type__drive == PRN_DRIVE) && ((*ptrs__prn_ini_date_)->prn_type == PRN_PDF)) // && (translucency==255))
    {
        for (i=0; i<8; i+=2)
        {
            pxy00[i] = units_to_prn_x(ppxy[i], ppxy[i+1]) + get_pdf_left_margin();
            pxy00[i+1]= get_pdf_page_height() - units_to_prn_y(ppxy[i], ppxy[i+1]) - get_pdf_top_margin();
        }

        kolorR.red = _dac_normal[kolor0][0] << 2;
        kolorR.gre = _dac_normal[kolor0][1] << 2;
        kolorR.blu = _dac_normal[kolor0][2] << 2;

        if (((*ptrs__prn_ini_date_)->color_print == 1) && (bw == 0)) {
            if (((*ptrs__prn_ini_date_)->gray_print == 1) || (grey == 1)) {
                intensity = get_grey_intensity(kolorR);
                kolorR = {intensity, intensity, intensity};
            }
        } else {
            if ((kolorR.red < 254) && (kolorR.gre < 254) && (kolorR.blu < 254))
                kolorR = {0, 0, 0};
        }

        FilledSolidPDF(8, pxy00, kolorR, HPDF_ROUND_END, translucency);

        return TRUE;
    }


	jednostki_to_prn_xy(8, ppxy, pxy);

	ignore_p = FALSE;

	if (translucency < 255)
	{

		found = 0;

		for (i = 0; i < 8; i++)
			lpxy[i] = pxy[i];
		//eliminations of overlaps
		/*

		for (ii = 0; ii < LAST_POLYGONS_NO; ii++)
		{

			if (compare_vectors_prn(&last_polygon_vector[ii], (p_vector*)&lpxy[0]) == 0)
			{
				// first vector is equal to previous last
				found = 1;
				//eliminates full overlap  //TEMPORARY
				if (first_polygon_vector[ii].y1 == lpxy[4]) { found = 5; break; }
			}
			else if (compare_vectors_prn(&last_polygon_vector[ii], (p_vector*)&lpxy[4]) == 0)
			{
				// last vector is equal to previous last
				found = 2;
				//eliminates full overlap  //TEMPORARY
				if (first_polygon_vector[ii].y1 == lpxy[1]) { found = 5; break; }
			}
			else if (compare_vectors_prn(&first_polygon_vector[ii], (p_vector*)&lpxy[0]) == 0)
			{
				// first vector is equal to previous first
				found = 3;
				//eliminates full overlap  //TEMPORARY
				if (last_polygon_vector[ii].y1 == lpxy[4]) { found = 5; break; }
			}
			else if (compare_vectors_prn(&first_polygon_vector[ii], (p_vector*)&lpxy[4]) == 0)
			{
				// last vector is equal to previous first
				found = 4;
				//eliminates full overlap  //TEMPORARY
				if (last_polygon_vector[ii].y1 == lpxy[1]) { found = 5; break; }
			}
		}

		switch (found)
		{
		case 0:
			break;
		case 1:
		case 3:   //moving first vector to the center
			ret_move = move_vector_inside_prn(&lpxy[0], 1, 1); //0
			if (!ret_move) ignore_p = TRUE;
			break;
		case 2:   //moving last vector to the center
		case 4:
			ret_move = move_vector_inside_prn(&lpxy[0], 4, 1); //0
			if (!ret_move) ignore_p = TRUE;
			break;
		case 5:
			ignore_p = TRUE;
			break;
		default:
			break;
		}
		*/

		//eliminations of overlaps
		/*

		for (ii = 0; ii < LAST_POLYGONS_NO; ii++)
		{

			if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[0]) == 0)
			{
				// first vector is equal to previous last
				found = 1;
				//eliminates full overlap  //TEMPORARY
				if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[4]) == 0) { found = 5; break; }
				if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[6]) == 0) { found = 5; break; }
				if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[2]) == 0) { found = 5; break; }
				break;
			}
			else if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[2]) == 0)
			{
				// second vector is equal to previous last
				found = 2;
				//eliminates full overlap  //TEMPORARY
				if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[6]) == 0) { found = 5; break; }
				if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[0]) == 0) { found = 5; break; }
				if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[4]) == 0) { found = 5; break; }
				break;
			}
			else if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[4]) == 0)
			{
				// third vector is equal to previous last
				found = 3;
				//eliminates full overlap  //TEMPORARY
				if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[0]) == 0) { found = 5; break; }
				if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[2]) == 0) { found = 5; break; }
				if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[6]) == 0) { found = 5; break; }
				break;
			}
			else if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[6]) == 0)
			{
				// last vector is equal to previous last
				found = 4;
				//eliminates full overlap  //TEMPORARY
				if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[2]) == 0) { found = 5; break; }
				if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[4]) == 0) { found = 5; break; }
				if (compare_vectors((p_vector*)&last_polygon[ii], (p_vector*)&lpxy[0]) == 0) { found = 5; break; }
				break;
			}
		}

		switch (found)
		{
		case 0:
			break;
		case 1://moving first vector to the center
			ret_move = move_vector_inside((T_PixelTVal*)&lpxy, 1, 1);
			if (!ret_move) ignore_p = TRUE;
			break;
		case 2://moving second vector to the center
			ret_move = move_vector_inside((T_PixelTVal*)&lpxy, 2, 1);
			if (!ret_move) ignore_p = TRUE;
			break;
		case 3:   //moving third vector to the center
			ret_move = move_vector_inside((T_PixelTVal*)&lpxy, 3, 1);
			if (!ret_move) ignore_p = TRUE;
			break;
		case 4: //moving last vector to the center
			ret_move = move_vector_inside((T_PixelTVal*)&lpxy, 4, 1);
			if (!ret_move) ignore_p = TRUE;
			break;
		case 5:
			ignore_p = TRUE;
			break;
		default:
			break;
		}

		*/

		for (i = 0; i < 8; i++)
			pxy[i] = lpxy[i];

		set_trans_blender(0, 0, 0, translucency);
		set_mode_trans();
	}

	kolorA = get_Allegro_color(kolor0, bw, grey);

	my_soft_polygon(allegro_prn_bmp, 4, pxy, kolorA, translucency, colorB);

	if (translucency < 255)
	{
		set_mode_solid();
	}
	
	return TRUE;
}

BOOL draw_rectangle_to_drive_trans___(double* xy, double dist, int kolor, BOOL bw, BOOL grey, int translucency)
/*----------------------------------------------------------------------------------------------------------*/
{
	unsigned w_tmp;
	double xd1, xd2, xd_o, yd_o;
	double cl, cp;
	double xl, xp, y;
	double ymin, ymin0;
	double x1a, x2a, angle_l, angle_p;
	
	double dxp, dxl;
	double x1 = xy[0], y1 = xy[1],
		x2 = xy[2], y2 = xy[3],
		x3 = xy[4], y3 = xy[5],
		x4 = xy[6], y4 = xy[7];
	

	if (translucency < 255)
	{
		set_trans_blender(0, 0, 0, translucency);
		
		set_mode_trans();
	}


	Set_Line_Width(jednostki_d_to_plt_d(dist));


	xd_o = yd_o = 0; 	/*to avoid warnings*/

	angle_l = Atan2(y4 - y1, x4 - x1);
	angle_p = Atan2(y3 - y2, x3 - x2);
	dxl = dist * fabs(sin(angle_l));
	dxp = dist * fabs(sin(angle_p));

	if (fabs(y1 - y4) <= dist)
	{
		
		return linen_plt_trans((x1 + x4) / 2 + dxl, (y1 + y4) / 2, (x2 + x3) / 2 - dxp, (y1 + y4) / 2, 0, kolor, 0, bw, grey);  //solid mode
	}

	x1a = x1 + dxl;
	x2a = x2 - dxp;
	cl = (x4 - x1) / (y4 - y1);
	cp = (x3 - x2) / (y3 - y2);
	ymin = y1;
	ymin0 = y4;
	w_tmp = 0;


	do
	{
		ymin -= dist;
		y = ymin + dist / 2;
		if (ymin < ymin0)
		{
			y = ymin0 + dist / 2;
		}
		xl = x1a + (y - y1) * cl;
		xp = x2a + (y - y2) * cp;
		if (xl > xp)
		{
			xl = (xl + xp) / 2;
			xp = xl;
		}
		xd1 = xl;
		xd2 = xp;
		if (w_tmp % 2 == 1)
		{
			xd1 = xp;
			xd2 = xl;
		}
		
		if (linen_plt_trans(xd1, y, xd2, y, 0, kolor, 0, bw, grey) == FALSE) return FALSE;

		xd_o = xd2;
		yd_o = y;
		w_tmp++;
	} while (Check_if_GE(ymin, ymin0));

	if (translucency < 255)
	{
		set_mode_solid();
	}

	return TRUE;
}

BOOL draw_polygon_to_drive_pattern(int numpoints, double* xy0)
{
    int kolorA, kolorB;

    T_PixelTVal lpxy[256];
    int pxy[256], pxy1[256], pxy2[256];
    double ppxy[256];
    int found;
    BOOL ignore_p;
    int i;
    double xy[256];
    WIELOKAT w = S4def;
    BOOL ret;

    memcpy(&xy, xy0, sizeof(double) * numpoints);

    if (print_inversion == TRUE)
    {
        if (print_rotation == TRUE)
        {

            for (i=0; i<numpoints; i+=2)
            {
                ppxy[i] = FormatX_S - xy[i];
                ppxy[i+1] = FormatY_S - xy[i+1];
            }

        }
        else
        {
            for (i=0; i<numpoints; i+=2)
            {
                ppxy[i] = FormatX_S - xy[i] + FormatX_S0;
                ppxy[i+1] = FormatY_S + FormatY_S0 - xy[i+1];
            }

        }
    }
    else
    {
        if (print_rotation == TRUE)
        {
            for (i=0; i<numpoints; i+=2)
            {
                ppxy[i] = xy[i];
                ppxy[i+1] = xy[i+1];
            }

        }
        else
        {
            for (i=0; i<numpoints; i+=2)
            {
                ppxy[i] = xy[i];
                ppxy[i+1] = xy[i+1];
            }

        }
    }

    jednostki_to_prn_xy(numpoints, ppxy, pxy);

    kolorA = 0xFFFFFF;

    kolorB = 0xFFFFFFFF;

    my_soft_polygon(allegro_prn_bmp, numpoints/2, pxy, kolorA, 255, kolorB);


    solid_mode();

    return TRUE;
}


BOOL draw_polygon_to_drive_gradient(int numpoints, float *xy, unsigned char translucency, GRADIENT *gradient, Print_Rect *window_to_print)
{
    int kolorA, kolorB;

    //T_PixelTVal lpxy[256];
    long pxy[256], pxy1[256]; //, pxy2[256];
    double ppxy[256];
    int found;
    BOOL ignore_p;
    int i;
    WIELOKAT w = S4def;
    BOOL ret;
    V3D_f v_f[8];
    GRADIENT8 gradient8;
    int num_wy, num_we ;
    int *poly_wy;
    int wl, wg, wp, wd;
    int ik;
#define MaxNumPolygonPoints 200

    if (print_inversion == TRUE)
    {
        if (print_rotation == TRUE)
        {

            for (i=0; i<numpoints; i+=2)
            {
                ppxy[i] = FormatX_S - xy[i];
                ppxy[i+1] = FormatY_S - xy[i+1];
            }

        }
        else
        {
            for (i=0; i<numpoints; i+=2)
            {
                ppxy[i] = FormatX_S - xy[i] + FormatX_S0;
                ppxy[i+1] = FormatY_S + FormatY_S0 - xy[i+1];
            }

        }
    }
    else
    {
        if (print_rotation == TRUE)
        {
            for (i=0; i<numpoints; i+=2)
            {
                ppxy[i] = xy[i];
                ppxy[i+1] = xy[i+1];
            }

        }
        else
        {
            for (i=0; i<numpoints; i+=2)
            {
                ppxy[i] = xy[i];
                ppxy[i+1] = xy[i+1];
            }

        }
    }

    jednostki_to_prn_xy_l(numpoints, ppxy, pxy);

    if (NULL == (poly_wy = (int*)malloc((MaxNumPolygonPoints + 2) * sizeof(poly_wy[0]))))
    {
        return TRUE;
    }

    if (gradient!=NULL) memmove(&gradient8, gradient, sizeof(GRADIENT));

    for (int k=0; k<numpoints; k++)
    {
       poly_wy[k]=(int)pxy[k];
    }
    num_wy=numpoints/2;

    //if (FALSE == Clip_Solid0 (numpoints/2, pxy, &num_wy, poly_wy))
    //{
        /*
        int ik=0;
        for (int k=0; k<num_wy; k++)
        {
            if (pxy1[k] != poly_wy[k])
                ik++;
        }
        num_we=num_wy;
         */

        if (gradient==NULL)
            Clip_Solid (numpoints/2, pxy, &num_wy, poly_wy) ;
        else
        {
            //Get_View_Coord(&wl, &wg, &wp, &wd);
            wl=allegro_prn_bmp->cl;
            wg=allegro_prn_bmp->ct;
            wp=allegro_prn_bmp->cr;
            wd=allegro_prn_bmp->cb;

            clipPolygonWithColors(pxy, (unsigned int *) gradient, numpoints/2, wl, wg, wp, wd, poly_wy,(unsigned int *) &gradient8, &num_wy);
        }
    //}

    //kolorA = 0xFFFFFF;
    //kolorB = 0xFFFFFFFF;
    //kolorA = 0x000000;
    //kolorB = 0x000000FF;
    //my_soft_polygon(allegro_prn_bmp, numpoints/2, pxy, kolorA, 255, kolorB);

    v_f[0].x=(float)poly_wy[0];
    v_f[0].y=(float)poly_wy[1];
    v_f[0].z=0.0f;
    v_f[0].u=0;
    v_f[0].v=0;
    v_f[0].c=gradient8.c1;

    v_f[1].x=(float)poly_wy[2];
    v_f[1].y=(float)poly_wy[3];
    v_f[1].z=0.0f;  //0
    v_f[1].u=0;
    v_f[1].v=0;
    v_f[1].c=gradient8.c2;

    v_f[2].x=(float)poly_wy[4];
    v_f[2].y=(float)poly_wy[5];
    v_f[2].z=0.0f;
    v_f[2].u=0;
    v_f[2].v=0;
    v_f[2].c=gradient8.c3;

    if (num_wy>3)
    {
        v_f[3].x = (float)poly_wy[6];
        v_f[3].y = (float)poly_wy[7];
        v_f[3].z = 0.0f;  //0
        v_f[3].u = 0;
        v_f[3].v = 0;
        v_f[3].c = gradient8.c4;

        if (num_wy>4)
        {
            v_f[4].x = (float)poly_wy[8];
            v_f[4].y = (float)poly_wy[9];
            v_f[4].z = 0.0f;  //0
            v_f[4].u = 0;
            v_f[4].v = 0;
            v_f[4].c = gradient8.c5;

            if (num_wy>5)
            {
                v_f[5].x = (float)poly_wy[10];
                v_f[5].y = (float)poly_wy[11];
                v_f[5].z = 0.0f;  //0
                v_f[5].u = 0;
                v_f[5].v = 0;
                v_f[5].c = gradient8.c6;
            }
            if (num_wy>6)
            {
                v_f[6].x = (float)poly_wy[12];
                v_f[6].y = (float)poly_wy[13];
                v_f[6].z = 0.0f;  //0
                v_f[6].u = 0;
                v_f[6].v = 0;
                v_f[6].c = gradient8.c7;
            }
            if (num_wy>7)
            {
                v_f[7].x = (float)poly_wy[14];
                v_f[7].y = (float)poly_wy[15];
                v_f[7].z = 0.0f;  //0
                v_f[7].u = 0;
                v_f[7].v = 0;
                v_f[7].c = gradient8.c8;
            }

            ////polygon3d_f(screenplay, POLYTYPE_GRGB, NULL, num_wy, (V3D_f **)v_f);   //this is crashing
            ////temporary removed due to crash, instead is using quad3d_f
            quad3d_f(allegro_prn_bmp, POLYTYPE_GRGB, NULL, &v_f[0], &v_f[1], &v_f[2], &v_f[3]);
            if (num_wy==8)
            {
                quad3d_f(allegro_prn_bmp, POLYTYPE_GRGB, NULL, &v_f[3], &v_f[4], &v_f[5], &v_f[0]);
                quad3d_f(allegro_prn_bmp, POLYTYPE_GRGB, NULL, &v_f[5], &v_f[6], &v_f[7], &v_f[0]);
            }
            else if (num_wy==7)
            {
                quad3d_f(allegro_prn_bmp, POLYTYPE_GRGB, NULL, &v_f[3], &v_f[4], &v_f[5], &v_f[0]);
                triangle3d_f(allegro_prn_bmp, POLYTYPE_GRGB, NULL, &v_f[5], &v_f[6], &v_f[0]);
            }
            else if (num_wy==6)
            {
                quad3d_f(allegro_prn_bmp, POLYTYPE_GRGB, NULL, &v_f[3], &v_f[4], &v_f[5], &v_f[0]);
                //triangle3d_f(screenplay, POLYTYPE_GRGB, NULL, &v_f[5], &v_f[6], &v_f[0]);
            }
            else if (num_wy==5)
            {
                triangle3d_f(allegro_prn_bmp, POLYTYPE_GRGB, NULL, &v_f[3], &v_f[4], &v_f[0]);
            }
        }
        else quad3d_f(allegro_prn_bmp, POLYTYPE_GRGB, NULL, &v_f[0], &v_f[1], &v_f[2], &v_f[3]);
    }
    else {
        triangle3d_f(allegro_prn_bmp, POLYTYPE_GRGB, NULL, &v_f[0], &v_f[1], &v_f[2]);
    }

        free(poly_wy);
        solid_mode();

    return TRUE;
}


BOOL draw_rectangle_to_drive_gradient(double* xy0)
{
    int kolorA, kolorB;

    T_PixelTVal lpxy[8];
    int pxy[8], pxy1[8], pxy2[8];
    double ppxy[8];
    int found;
    BOOL ignore_p;
    int i;
    double xy[8];
    WIELOKAT w = S4def;
    BOOL ret;

    memcpy(&xy, xy0, sizeof(double) * 8);

    if (print_inversion == TRUE)
    {
        if (print_rotation == TRUE)
        {


            ppxy[0] = FormatX_S - xy[0];
            ppxy[2] = FormatX_S - xy[2];
            ppxy[4] = FormatX_S - xy[4];
            ppxy[6] = FormatX_S - xy[6];
            ppxy[1] = FormatY_S - xy[1];
            ppxy[3] = FormatY_S - xy[3];
            ppxy[5] = FormatY_S - xy[5];
            ppxy[7] = FormatY_S - xy[7];

        }
        else
        {


            ppxy[0] = FormatX_S - xy[0] + FormatX_S0;
            ppxy[2] = FormatX_S - xy[2] + FormatX_S0;
            ppxy[4] = FormatX_S - xy[4] + FormatX_S0;
            ppxy[6] = FormatX_S - xy[6] + FormatX_S0;
            ppxy[1] = FormatY_S + FormatY_S0 - xy[1];
            ppxy[3] = FormatY_S + FormatY_S0 - xy[3];
            ppxy[5] = FormatY_S + FormatY_S0 - xy[5];
            ppxy[7] = FormatY_S + FormatY_S0 - xy[7];
        }
    }
    else
    {
        if (print_rotation == TRUE)
        {


            ppxy[0] = xy[0];
            ppxy[2] = xy[2];
            ppxy[4] = xy[4];
            ppxy[6] = xy[6];
            ppxy[1] = xy[1];
            ppxy[3] = xy[3];
            ppxy[5] = xy[5];
            ppxy[7] = xy[7];

        }
        else
        {
            ppxy[0] = xy[0];
            ppxy[2] = xy[2];
            ppxy[4] = xy[4];
            ppxy[6] = xy[6];
            ppxy[1] = xy[1];
            ppxy[3] = xy[3];
            ppxy[5] = xy[5];
            ppxy[7] = xy[7];
        }
    }

    jednostki_to_prn_xy(8, ppxy, pxy);

    kolorA = 0xFFFFFF;

    kolorB = 0xFFFFFFFF;

    my_soft_polygon(allegro_prn_bmp, 4, pxy, kolorA, 255, kolorB);

    solid_mode();

    return TRUE;
}


BOOL draw_rectangle_to_drive_pattern(double* xy0)
{
	int kolorA, kolorB;

	T_PixelTVal lpxy[8];
	int pxy[8], pxy1[8], pxy2[8];
	double ppxy[8];
	int found;
	BOOL ignore_p;
	int i;
	double xy[8];
	WIELOKAT w = S4def;
	BOOL ret;

	memcpy(&xy, xy0, sizeof(double) * 8);

	if (print_inversion == TRUE)
	{
		if (print_rotation == TRUE)
		{
			

			ppxy[0] = FormatX_S - xy[0];
			ppxy[2] = FormatX_S - xy[2];
			ppxy[4] = FormatX_S - xy[4];
			ppxy[6] = FormatX_S - xy[6];
			ppxy[1] = FormatY_S - xy[1];
			ppxy[3] = FormatY_S - xy[3];
			ppxy[5] = FormatY_S - xy[5];
			ppxy[7] = FormatY_S - xy[7];

		}
		else
		{

			
			ppxy[0] = FormatX_S - xy[0] + FormatX_S0;
			ppxy[2] = FormatX_S - xy[2] + FormatX_S0;
			ppxy[4] = FormatX_S - xy[4] + FormatX_S0;
			ppxy[6] = FormatX_S - xy[6] + FormatX_S0;
			ppxy[1] = FormatY_S + FormatY_S0 - xy[1];
			ppxy[3] = FormatY_S + FormatY_S0 - xy[3];
			ppxy[5] = FormatY_S + FormatY_S0 - xy[5];
			ppxy[7] = FormatY_S + FormatY_S0 - xy[7];
		}
	}
	else
	{
		if (print_rotation == TRUE)
		{
			

			ppxy[0] = xy[0];
			ppxy[2] = xy[2];
			ppxy[4] = xy[4];
			ppxy[6] = xy[6];
			ppxy[1] = xy[1];
			ppxy[3] = xy[3];
			ppxy[5] = xy[5];
			ppxy[7] = xy[7];

		}
		else
		{
			ppxy[0] = xy[0];
			ppxy[2] = xy[2];
			ppxy[4] = xy[4];
			ppxy[6] = xy[6];
			ppxy[1] = xy[1];
			ppxy[3] = xy[3];
			ppxy[5] = xy[5];
			ppxy[7] = xy[7];
		}
	}

	jednostki_to_prn_xy(8, ppxy, pxy);

	kolorA = 0xFFFFFF;

	kolorB = 0xFFFFFFFF;

	my_soft_polygon(allegro_prn_bmp, 4, pxy, kolorA, 255, kolorB);

    solid_mode();

	return TRUE;
}


 BOOL draw_rectangle_to_drive_solid (double *xy, double dist, int kolor, BOOL bw, BOOL grey, int translucency)
/*----------------------------------------------------------------------------------------------------------*/
{
  unsigned w_tmp ;
  double xd1, xd2, xd_o, yd_o ;
  double cl, cp ;
  double xl, xp, y ;
  double ymin, ymin0 ;
  double x1a, x2a, angle_l, angle_p ;
  double dxp, dxl;
  double x1 = xy [0], y1 = xy [1],
	 x2 = xy [2], y2 = xy [3],
	 x3 = xy [4], y3 = xy [5],
	 x4 = xy [6], y4 = xy [7] ;
  
  Set_Line_Width(jednostki_d_to_plt_d(dist));

  xd_o = yd_o = 0 ; 	/*to avoid warnings*/
  
  angle_l = Atan2 (y4 - y1, x4 - x1) ;
  angle_p = Atan2 (y3 - y2, x3 - x2) ;
  dxl = dist * fabs (sin (angle_l)) ;
  dxp = dist * fabs (sin (angle_p)) ;
  
  if (fabs (y1 - y4) <= dist)
  {
                  
    return linen_plt1 ((x1 + x4) / 2 + dxl, (y1 + y4) / 2, (x2 + x3) / 2 - dxp, (y1 + y4) / 2, 0, kolor, 0, bw, grey) ;  //solid mode
  }
  
  x1a = x1 + dxl ;
  x2a = x2 - dxp ;
  cl = (x4 - x1) / (y4 - y1) ;
  cp = (x3 - x2) / (y3 - y2) ;
  ymin = y1 ;
  ymin0 = y4 ;
  w_tmp = 0 ;


  do
  {
    ymin -= dist ;
    y = ymin + dist / 2 ;
    if (ymin < ymin0)
    {
      y = ymin0 + dist / 2 ;
    }
    xl = x1a + (y - y1) * cl ;
    xp = x2a + (y - y2) * cp ;
    if (xl > xp)
    {
      xl = (xl + xp) / 2 ;
      xp = xl ;
    }
    xd1 = xl ;
    xd2 = xp ;
    if (w_tmp % 2 == 1)
    {
      xd1 = xp ;
      xd2 = xl ;
    }
   
      if ( linen_plt1 (xd1, y, xd2, y, 0, kolor, 0, bw, grey) == FALSE ) return FALSE;

    xd_o = xd2 ;
    yd_o = y ;
    w_tmp++ ;
  }
  while (Check_if_GE(ymin, ymin0));


  return TRUE ;
}

 BOOL draw_rectangle_to_drive(double* xy, double dist, int kolor0, BOOL bw, BOOL grey, int translucency, int colorB)
 {
	 if (type__drive != PRN_DRIVE) //|| (translucency==255))
		 return draw_rectangle_to_drive_solid(xy, dist, kolor0, bw, grey, translucency);
	 else
		 return draw_rectangle_to_drive_trans(xy, dist, kolor0, bw, grey, translucency, colorB);
 }


 void do_nothing(void)
 {
    return;
 }

BOOL Draw_Wielokat_To_Drive (WIELOKAT *ptr_w, Print_Rect *window_to_print)
/*---------------------------------------------------------------------*/
{
  int i;
  BOOL b_ret ;
  int kolor ;
  int ptr_w_lp;
  byte_ int_intensity;
  byte_ intensity;
  float intensity_tab[4] = { 0.2, 1.0, 0.8, 0.6 };

  SOLID_PATTERN solid_pattern = { 2,0,0,0,0,"" };
  char* scale_ptr;
  char* dx_ptr;
  char* angle_ptr;
  char* dy_ptr;
  char* name_ptr;

  char* scale_ptr1;
  char* dx_ptr1;
  char* angle_ptr1;
  char* dy_ptr1;
  char* name_ptr1;

  char* scale_ptr2;
  char* dx_ptr2;
  char* angle_ptr2;
  char* dy_ptr2;
  char* name_ptr2;

  char* translucency_ptr, *translucency_w1_ptr, *translucency_w2_ptr;
  int kolory_paper, colorB, kolor256, r,g,b;

  T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_;

  if (type__drive == PRN_DRIVE)
  {
	  ptrs__prn_ini_date_ = get_ptrs__prn_ini_date();
	  if ((*ptrs__prn_ini_date_)->background == TRUE)
	  {
		  kolor256 = GetColorAC(kolory.paper);

		  r = _dac_normal[kolor256][0] << 2;
		  g = _dac_normal[kolor256][1] << 2;
		  b = _dac_normal[kolor256][2] << 2;

		  colorB = makeacol32(r, g, b, 255);

          if (((*ptrs__prn_ini_date_)->gray_print) || !((*ptrs__prn_ini_date_)->color_print) || ((*ptrs__prn_ini_date_)->PCX_gray))
          {
              intensity = 0.2989 * r + 0.5870 * g + 0.1140 * b;

              if (intensity < 252) intensity *= intensity_tab[(*ptrs__prn_ini_date_)->gray_saturation];

              int_intensity = (byte_)intensity;
              colorB = makecol(int_intensity, int_intensity, int_intensity);

          }
          else if (!((*ptrs__prn_ini_date_)->color_print))
          {
              if ((r < 252) && (g < 252) && (b < 252))
              {
                  int_intensity = 0;
                  colorB = makecol(int_intensity, int_intensity, int_intensity);
              }
          }
	  }
	  else colorB = 0xFFFFFFFF;
  }
  else colorB = 0xFFFFFFFF;

  b_ret = TRUE ;

  kolor = MakeColor(GetColorWin(ptr_w->kolor));

  BOOL concave = FALSE;
  float sign_p[3];
  int sum_signs;
  int concave_no;

 if ((type__drive == PRN_DRIVE) && (ptr_w->empty_typ == 0) && (ptr_w->gradient == 1))
 {
     unsigned char translucency = 255;
     char *gradient_ptr=NULL;

     translucency_ptr = (char *) ptr_w->xy;
     translucency_ptr += (ptr_w->lp * sizeof(float));
     memmove(&translucency, translucency_ptr, sizeof(unsigned char));

     gradient_ptr=translucency_ptr+sizeof(unsigned char);

     Draw_Solid_Gradient(ptr_w->lp, ptr_w->xy, Layers[ptr_w->warstwa].bw, Layers[ptr_w->warstwa].grey, translucency, (GRADIENT*)gradient_ptr, window_to_print);

    return TRUE;
 }

 if ((type__drive == PRN_DRIVE) &&  ((*ptrs__prn_ini_date_)->prn_type == PRN_PDF) && (ptr_w->empty_typ == 0) && (ptr_w->pattern == 0) && (ptr_w->translucent == 1))
 {
    unsigned char translucency = 255;
     double xy[MaxNumPoints];

    translucency_ptr = (char *) ptr_w->xy;
    translucency_ptr += (ptr_w->lp * sizeof(float));
    memmove(&translucency, translucency_ptr, sizeof(unsigned char));

     for (i = 0; i < (int) ptr_w->lp; i ++)
     {
         xy[i]=ptr_w->xy[i];
     }

     if (ptr_w->lp == 6)
     {
         xy[6]=xy[4];
         xy[7]=xy[5];
     }

    if (FALSE == draw_rectangle_to_drive(xy, 0 /*df_dist*/, kolor, Layers[ptr_w->warstwa].bw, Layers[ptr_w->warstwa].grey,(int) translucency, colorB))
        return FALSE;
 }
 else
 {
     if (ptr_w->lp == 8) {
         pf_point A, B, C, D;

         A.x = ptr_w->xy[0];
         A.y = ptr_w->xy[1];

         B.x = ptr_w->xy[2];
         B.y = ptr_w->xy[3];

         C.x = ptr_w->xy[4];
         C.y = ptr_w->xy[5];

         D.x = ptr_w->xy[6];
         D.y = ptr_w->xy[7];

         sign_p[0] = ((B.x - A.x) * (C.y - B.y) - (B.y - A.y) * (C.x - B.x));
         sign_p[1] = ((C.x - B.x) * (D.y - C.y) - (C.y - B.y) * (D.x - C.x));
         sign_p[2] = ((D.x - C.x) * (A.y - D.y) - (D.y - C.y) * (A.x - D.x));

         sum_signs = 0;
         for (i = 0; i < 3; i++) sum_signs += sign_p[i] < 0 ? -1 : 1;

         if ((sum_signs != 3) && (sum_signs != -3)) concave = TRUE;
     }

     if ((ptr_w->lp == 8 && ptr_w->obiektt1 != SIMPW) || (concave == TRUE))
     {
         WIELOKAT w1 = Stdef, w2 = Stdef;

         if (ptr_w->obiektt1 != SIMPW) Get_Rec_2TrAngle(ptr_w, &w1, &w2);
         else //concave
         {
             concave_no = 0;
             //finding concave vertex
             if (sum_signs > 0) //searching for negative
             {
                 for (i = 0; i < 3; i++) {
                     if (sign_p[i] < 0.0) {
                         concave_no = (i + 1) % 3;
                         break;
                     }
                 }
             } else {
                 for (i = 0; i < 3; i++) {
                     if (sign_p[i] >= 0.0) {
                         concave_no = (i + 1) % 3;
                         break;
                     }
                 }
             }

             //so we start from concave_no

             for (i = 0; i < +6; i++)
                 w1.xy[i] = ptr_w->xy[((concave_no * 2) + i) % 8];

             for (i = 0; i < 6; i++)
                 w2.xy[i] = ptr_w->xy[(((concave_no * 2) + 4) + i) % 8];

         }

         Normalize_Solid(&w1);
         Normalize_Solid(&w2);

         w1.warstwa = ptr_w->warstwa;
         w2.warstwa = ptr_w->warstwa;

         w1.kolor = ptr_w->kolor;
         w2.kolor = ptr_w->kolor;

         w1.empty_typ = ptr_w->empty_typ;
         w2.empty_typ = ptr_w->empty_typ;

         w1.pattern = ptr_w->pattern;
         w2.pattern = ptr_w->pattern;

         w1.translucent = ptr_w->translucent;
         w2.translucent = ptr_w->translucent;

         if (ptr_w->empty_typ == 0) {
             if (ptr_w->pattern == 1) {


                 scale_ptr = (char *) ptr_w->xy;
                 scale_ptr += (ptr_w->lp * sizeof(float));
                 dx_ptr = scale_ptr;
                 dx_ptr += sizeof(short int);
                 angle_ptr = dx_ptr;
                 angle_ptr += sizeof(short int);
                 dy_ptr = angle_ptr;
                 dy_ptr += sizeof(short int);
                 name_ptr = dy_ptr;
                 name_ptr += sizeof(short int);

                 scale_ptr1 = (char *) w1.xy;
                 scale_ptr1 += (w1.lp * sizeof(float));
                 dx_ptr1 = scale_ptr1;
                 dx_ptr1 += sizeof(short int);
                 angle_ptr1 = dx_ptr1;
                 angle_ptr1 += sizeof(short int);
                 dy_ptr1 = angle_ptr1;
                 dy_ptr1 += sizeof(short int);
                 name_ptr1 = dy_ptr1;
                 name_ptr1 += sizeof(short int);

                 memmove(scale_ptr1, scale_ptr, sizeof(short int));
                 memmove(dx_ptr1, dx_ptr, sizeof(short int));
                 memmove(angle_ptr1, angle_ptr, sizeof(short int));
                 memmove(dy_ptr1, dy_ptr, sizeof(short int));
                 memmove(name_ptr1, name_ptr, strlen(name_ptr) + 1);
                 w1.n = 8 + w1.lp * sizeof(float) + 4 * sizeof(short int) + (int) strlen(name_ptr) + 1;

                 scale_ptr2 = (char *) w2.xy;
                 scale_ptr2 += (w2.lp * sizeof(float));
                 dx_ptr2 = scale_ptr2;
                 dx_ptr2 += sizeof(short int);
                 angle_ptr2 = dx_ptr2;
                 angle_ptr2 += sizeof(short int);
                 dy_ptr2 = angle_ptr2;
                 dy_ptr2 += sizeof(short int);
                 name_ptr2 = dy_ptr2;
                 name_ptr2 += sizeof(short int);
                 memmove(scale_ptr2, scale_ptr, sizeof(short int));
                 memmove(dx_ptr2, dx_ptr, sizeof(short int));
                 memmove(angle_ptr2, angle_ptr, sizeof(short int));
                 memmove(dy_ptr2, dy_ptr, sizeof(short int));
                 memmove(name_ptr2, name_ptr, strlen(name_ptr) + 1);
                 w2.n = 8 + w2.lp * sizeof(float) + 4 * sizeof(short int) + (int) strlen(name_ptr) + 1;

             } else {
                 if (ptr_w->translucent == 1) {
                     translucency_ptr = (char *) ptr_w->xy;
                     translucency_ptr += (ptr_w->lp * sizeof(float));

                     translucency_w1_ptr = (char *) w1.xy;
                     translucency_w1_ptr += (w1.lp * sizeof(float));
                     memmove(translucency_w1_ptr, translucency_ptr, sizeof(unsigned char));
                     w1.n = 8 + w1.lp * sizeof(float) + sizeof(unsigned char);

                     translucency_w2_ptr = (char *) w2.xy;
                     translucency_w2_ptr += (w2.lp * sizeof(float));
                     memmove(translucency_w2_ptr, translucency_ptr, sizeof(unsigned char));
                     w2.n = 8 + w2.lp * sizeof(float) + sizeof(unsigned char);
                 }
             }
         }


         if (FALSE == Draw_Wielokat_To_Drive(&w1, window_to_print) ||
             FALSE == Draw_Wielokat_To_Drive(&w2, window_to_print))
             return FALSE;
     } else {
         WIELOKAT w, w0;
         double x, y, x1, y1, x2, y2;
         double df_dist;
         double xy[MaxNumPoints];
         float origin_x_, origin_y_;
         unsigned char translucency = 255;


         df_dist = df__width_fill;


         memcpy((void *) &w0, (void *) ptr_w, sizeof(WIELOKAT));

         memcpy((void *) &w, (void *) &w0, sizeof(WIELOKAT));

         origin_x_ = 0;
         origin_y_ = 0;

         sort_solid_point(&w, &w0);  //????

         x1 = w.xy[0];
         y1 = w.xy[1];
         intersect_solid(x1, y1, ptr_w, &x2, &y2);

         for (i = 2; i < (int) w.lp; i += 2) {
             xy[0] = x1;
             xy[1] = y1;
             xy[2] = x2;
             xy[3] = y2;
             xy[4] = w.xy[i];
             xy[5] = w.xy[i + 1];
             intersect_solid(w.xy[i], w.xy[i + 1], ptr_w, &x, &y);
             xy[6] = x;
             xy[7] = y;
             //  przestawienie wsp. x ponktow ( wsp. y sa takie same)

             if (xy[0] > xy[2]) {
                 double x_temp;

                 x_temp = xy[0];
                 xy[0] = xy[2];
                 xy[2] = x_temp;
             }

             if (xy[6] > xy[4]) {
                 double x_temp;

                 x_temp = xy[4];
                 xy[4] = xy[6];
                 xy[6] = x_temp;
             }

             if (ptr_w->empty_typ == 0) {
                 if (ptr_w->pattern == 0)
                 {
                     if (ptr_w->translucent == 1) {
                         translucency_ptr = (char *) ptr_w->xy;
                         translucency_ptr += (ptr_w->lp * sizeof(float));
                         memmove(&translucency, translucency_ptr, sizeof(unsigned char));
                     }
                     if (FALSE == draw_rectangle_to_drive(xy, df_dist, kolor, Layers[ptr_w->warstwa].bw,
                                                          Layers[ptr_w->warstwa].grey, (int) translucency, colorB))
                         return FALSE;

                 } else if (type__drive == PRN_DRIVE) {
                     if (((*ptrs__prn_ini_date_)->bitmap_only == TRUE) || (*ptrs__prn_ini_date_)->prn_type != PRN_PDF) {

                         ptr_w_lp = 8;

                         scale_ptr = (char *) ptr_w->xy;
                         scale_ptr += (ptr_w->lp * sizeof(float));
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
                         strcpy(solid_pattern.pattern, name_ptr);

                         Draw_Solid_Pattern(ptr_w_lp, xy, (char *) &solid_pattern, type__printer,
                                            Layers[ptr_w->warstwa].bw, Layers[ptr_w->warstwa].grey, origin_x_,
                                            origin_y_);
                     }
                 }
             } else if (ptr_w->empty_typ == 6) {
                 if (FALSE ==
                     draw_rectangle_to_drive(xy, df_dist, -1, Layers[ptr_w->warstwa].bw, Layers[ptr_w->warstwa].grey,
                                             (int) translucency, colorB))
                     return FALSE;
             } else if (ptr_w->empty_typ == 7) {
                 if (FALSE ==
                     draw_rectangle_to_drive(xy, df_dist, -2, Layers[ptr_w->warstwa].bw, Layers[ptr_w->warstwa].grey,
                                             (int) translucency, colorB))
                     return FALSE;
             }
             x1 = xy[6];
             y1 = xy[7];
             x2 = xy[4];
             y2 = xy[5];

         }

     }
     if (ptr_w->empty_typ == 6) b_ret = draw_border_solid_to_drive_null(ptr_w, -1);
     else if (ptr_w->empty_typ == 7) b_ret = draw_border_solid_to_drive_null(ptr_w, -2);

     else if (((ptr_w->empty_typ != 0) && (ptr_w->pattern == 0) && (ptr_w->translucent == 0)) ||
              (type__drive != PRN_DRIVE)) {
         b_ret = draw_border_solid_to_drive(ptr_w); // ???????
     }
 }
  return b_ret ;
}

int DrawSolid04_To_Drive(double x1, double y1, double x2, double y2, int kolor, int warstwa)
{
 
  WIELOKAT Solid=Stdef;
  
  int ret_val;

  if ( !line_clip (&x1, &y1, &x2, &y2) ) return 1;
  ret_val = Draw_Rectangle_To_Plt (x1, y1, x2, y2);
  return ret_val ;
}

BOOL Draw_Solidarc_To_Drive (SOLIDARC *ptr_sa, Print_Rect *window_to_print)
/*-----------------------------------------------------------------------*/
{
    unsigned char translucency = 255;
    T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_;
    int kolor ;
    int kolory_paper, colorB, kolor256, r,g,b;
    BOOL b_ret ;
    double df_dist;
    SOLID_PATTERN solid_pattern = { 2,0,0,0,0,"" };
    float origin_x_, origin_y_;

    SOLIDARC sa1, sa2;
    double midkat;
    double midwidth;
    double kat1, kat2, dkat;
    RECTD rectd;
    double xy[264];
    int in;
    double xs, ys;
    int n1, n2;

    origin_x_ = 0;
    origin_y_ = 0;

    if (type__drive == PRN_DRIVE)
    {
        ptrs__prn_ini_date_ = get_ptrs__prn_ini_date();
        if ((*ptrs__prn_ini_date_)->background == TRUE)
        {
            kolor256 = GetColorAC(kolory.paper);

            r = _dac_normal[kolor256][0] << 2;
            g = _dac_normal[kolor256][1] << 2;
            b = _dac_normal[kolor256][2] << 2;

            colorB = makeacol32(r, g, b, 255);
        }
        else colorB = 0xFFFFFFFF;
    }
    else colorB = 0xFFFFFFFF;

    b_ret = TRUE ;

    kolor = MakeColor(GetColorWin(ptr_sa->kolor));

    df_dist = df__width_fill;

    if (ptr_sa->empty_typ == 0)
    {

        if (type__drive != PRN_DRIVE)
        {
            return draw_border_solidarc_to_drive(ptr_sa, type__drive);
        }
        else {
            ////// creating polygone
            kat1 = Angle_Normal(ptr_sa->kat1);
            kat2 = Angle_Normal(ptr_sa->kat2);
            if (kat2 < kat1) kat2 += Pi2;
            dkat = kat2 - kat1;
            if (Check_if_LE(dkat, Pi_)) in = rysuj_solidarc_factory(ptr_sa, COPY_PUT, kolor, 0, 0, xy, TRUE, &n1, &n2);
            else {
                memmove(&sa1, ptr_sa, sizeof(SOLIDARC));
                memmove(&sa2, ptr_sa, sizeof(SOLIDARC));

                midwidth = (ptr_sa->width1 + ptr_sa->width2) / 2;

                midkat = (kat2 + kat1) / 2;

                sa1.kat1 = kat1;
                sa1.kat2 = midkat;

                sa2.kat1 = midkat;
                sa2.kat2 = kat2;

                if (sa1.reversed) {
                    sa1.width2 = ptr_sa->width2;
                    sa1.width1 = midwidth;
                    sa2.width2 = midwidth;
                    sa2.width1 = ptr_sa->width1;
                } else {
                    sa1.width1 = midwidth; //sa->width1;
                    sa1.width2 = ptr_sa->width2; //midwidth;
                    sa2.width1 = ptr_sa->width1; //midwidth;
                    sa2.width2 = midwidth; //sa->width2;
                }


                in = rysuj_solidarc_factory(&sa1, COPY_PUT, kolor, 1, 0, xy, TRUE, &n1, &n2);
                in = rysuj_solidarc_factory(&sa2, COPY_PUT, kolor, 2, in, xy, TRUE, &n1, &n2);
            }


            /////////////
            if (ptr_sa->pattern == 0) {
                if (ptr_sa->translucent == 1) translucency = ptr_sa->translucency;


                if (FALSE == draw_polygon_to_drive_trans(xy, in, /*df_dist,*/ kolor, Layers[ptr_sa->warstwa].bw,
                                                    Layers[ptr_sa->warstwa].grey, (int) translucency, colorB))
                    return FALSE;
            } else if (type__drive == PRN_DRIVE) {
                if (((*ptrs__prn_ini_date_)->bitmap_only == TRUE) || (*ptrs__prn_ini_date_)->prn_type != PRN_PDF) {

                    solid_pattern.scale = ptr_sa->scale;
                    solid_pattern.dx = ptr_sa->dx;
                    solid_pattern.angle = ptr_sa->angle;
                    solid_pattern.dy = ptr_sa->dy;
                    strcpy(solid_pattern.pattern, ptr_sa->patternname);

                    Draw_Solidarc_Pattern(in, xy, (char *) &solid_pattern, type__printer, Layers[ptr_sa->warstwa].bw,
                                          Layers[ptr_sa->warstwa].grey, origin_x_, origin_y_);
                }
            }
        }
    }

    else if ((ptr_sa->empty_typ != 0) && (ptr_sa->pattern == 0) && (ptr_sa->translucent == 0))
    {
        b_ret = draw_border_solidarc_to_drive(ptr_sa, type__drive);
    }

  return b_ret;
}

void draw_arrow_to_drive(double x0, double y0, double x1, double y1, double x2, double y2, double koc1, double kos1, double koc2, double kos2, double psize, AVECTOR *v)
{
    LINIA L=Ldef;
    WIELOKAT w=Stdef;
    int ret;

    L.x1=x1;
    L.y1=y1;
    L.x2=x2;
    L.y2=y2;
    L.warstwa=w.warstwa=v->warstwa;
    L.kolor=w.kolor=v->kolor;

    if (v->style<V_EDGE_SIMPLE) {
        L.typ = 64;

        line_width_type = Line_Width(L.typ);
        ret = Draw_Line_To_Drive(&L);
    }

    if ((v->style==V_EDGE_FIXED) || (v->style==V_EDGE_FIXED_INV) || (v->style==V_EDGE_ARC_FIXED) || (v->style==V_EDGE_ARC_FIXED_INV))
    {
        L.typ = V_FIXED_LINE; //128;
        L.x1 = x0;
        L.y1 = y0;
        L.x2 = x0 + psize * koc1;
        L.y2= y0 - psize * kos1;

        line_width_type = Line_Width(L.typ);
        ret = Draw_Line_To_Drive(&L);
    }

    else {
        w.xy[2] = x0;
        w.xy[3] = y0;
        w.xy[0] = x0 + psize * koc1;
        w.xy[1] = y0 - psize * kos1;
        w.xy[4] = x0 + psize * koc2;
        w.xy[5] = y0 - psize * kos2;

        w.lp = 6;
        w.n = 32;

        line_width_type = Solid_Line_Width(w.empty_typ);
        ret = Draw_Wielokat_To_Drive(&w, NULL);
    }

}


void draw_wave_to_drive(double x0, double y0, double x1, double y1, double x2, double y2, double koc, double kos, double n1, double ra, AVECTOR *v)
{
    SPLINE s=Splinedef;
    LINIA L=Ldef;
    double dl, dlm, dx, dy;
    double  xp, yp;
    int i;
    int ret;

    if ((v->magnitude1==0) && (v->magnitude2==0)) return;

    dx=ra;
    dy=ra/4;

    s.warstwa=v->warstwa;
    if (n1==1) s.kolor=1;
    else s.kolor=5;
    s.typ=64;

    dl=n1*sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

    if (fabs(dl)>(3*dx))
    {
        dlm=0;
        s.xy[0]=x1;
        s.xy[1]=y1;
        dlm+=(n1*dx);
        s.xy[2]=x1+dlm;
        s.xy[3]=y1+dy;
        i=4;
        dlm+=(2*n1*dx);
        while (fabs(dlm)<fabs(dl))
        {
            dy=-dy;
            s.xy[i]=x1+dlm;
            s.xy[i+1]=y1+dy;
            i+=2;
            if (i>=NumSplinePoints*2) break;
            dlm+=(2*n1*dx);
        }

        dlm-=(n1*dx);
        dy=-dy;
        s.xy[i]=x1+dlm;
        s.xy[i+1]=y1;
        i+=2;

        s.lp=i;
        s.xy[s.lp]=0.75;
        s.n = 8 + (s.lp + 1 ) * sizeof(float);

        s.npts=5;
        s.closed=0;

        for (i = 0; i < (int)s.lp; i += 2)
        {
            Rotate_Point(kos, koc, x1, y1, s.xy[i], s.xy[i + 1], &xp, &yp);
            s.xy[i] = xp; s.xy[i + 1] = yp;
        }

        ret=Draw_Spline_To_Drive(&s);
    }
    else if (fabs(dl)>0.001)
    {
        L.warstwa=v->warstwa;
        if (n1==1) L.kolor=1;
        else L.kolor=5;
        L.typ=64;
        L.x1=x1;
        L.y1=y1;
        L.x2=x2;
        L.y2=y2;

        line_width_type = Line_Width (L.typ);
        ret=Draw_Line_To_Drive(&L);
    }
}


void make_arcarrows_to_drive(LUK *l, AVECTOR *v, double kat)
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

    //T_Point P;

    df_psize = Get_Point_Size ();
    df_seg_len=df_psize;

    ra=df_seg_len / 2;
    ra1=0.9*ra;

    if ((v->style==24) || (v->style==25))
    {
        katS = Pi_ * 45.0 / 180;
        df_seg_len_dens = df_seg_len / 2.0;
    }
    else df_seg_len_dens = df_seg_len;

    i = 0 ;
    df_l0 = -df_seg_len_dens/2; //0 ;
    do
    {
        df_line_rem = measure_arcvector(l, b_first_end, df_l0, df_seg_len_dens, &df_x, &df_y);

        if (TRUE == Check_if_GT (df_line_rem, df_seg_len_dens/4))   //or maybe df_seg_len_dens/2
        {

            Lt1.x1 = df_x;
            Lt1.y1 = df_y;
            Lt1.x2 = l->x;
            Lt1.y2 = l->y;
            parametry_lini(&Lt1, &PL1);

            //angle=Angle_Normal(PL1.kat*Pi2/180);
            //koc=cos(angle);
            //kos=sin(angle);

            if ((v->style==23) || (v->style==25)) PL1.kat+=180;

            angle = Angle_Normal(-(PL1.kat) * Pi / 180);
            if (fabs(angle - Pi2) < 0.00001) angle = 0;
            if (fabs(angle) < 0.00001) angle = 0;
            koc1 = cos(angle - katS);
            koc2 = cos(angle + katS);
            kos1 = sin(angle - katS);
            kos2 = sin(angle + katS);

            n1 = -1;

            draw_arrow_to_drive(df_x, df_y, df_x, df_y, df_x, df_y, koc1, kos1, koc2, kos2, n1 * ra, v);
        }

        //P.x=df_x;
        //P.y=df_y;
        //P.kolor=7;
        //rysuj_punkt_(&P, COPY_PUT, 1);

        df_l0 += df_seg_len_dens ;
        i++ ;
    }
    while (TRUE == Check_if_GT (df_line_rem, df_seg_len/2 /*0*/)) ;

}


void make_arrows_to_drive(float x1, float y1, float x2, float y2, float x11, float y11, float x12, float y12, double angle0, AVECTOR *v, double kat)
{
    //arrows
    int i;
    double df_l0;
    double df_line_rem;
    BOOL b_first_end=TRUE;
    double df_seg_len,df_seg_len_dens;
    double df_x0, df_y0, df_x, df_y, df_x1, df_y1, df_x2, df_y2;
    double df_psize;
    point a1, a2, b1, b2, p;
    double koc, kos;
    double ra, ra1;
    float n, n1;
    double angle, angle_rev;
    double katS=Pi_*25.0/180;
    double koc1, kos1, koc2, kos2;
    PLINIA PL1;
    LINIA Lt1;
    double del_angle;


    df_psize = Get_Point_Size ();


    if (v->style!=15) df_seg_len=df_psize;
    else df_seg_len=df_psize*0.66;  //THERMAL

    if ((v->style==V_EDGE_FIXED) || (v->style==V_EDGE_FIXED_INV))
        katS = Pi_ * 45.0 / 180;

    angle=Angle_Normal(angle0);

    koc=cos(angle);
    kos=sin(angle);

    ra=df_seg_len / 2;
    ra1=0.9*ra;

    switch (v->style)
    {
        case 10:
        case 17:
            if (x1<=x2) angle=Angle_Normal(angle0+Pi_);
            break;
        case 11:
            if (y1<=y2) angle=Angle_Normal(angle0+Pi_);
            break;
        case 12:
            angle=Angle_Normal(-angle0);
            if (fabs(angle-Pi2)<0.00001) angle=0;
            if (fabs(angle)<0.00001) angle=0;
            break;
        case V_EDGE_SIMPLE:  //simple supported edge
        case V_EDGE_SIMPLE_INV:  //simple supported edge reversed
        case V_EDGE_FIXED:  //fixed edge
        case V_EDGE_FIXED_INV:  //fixed edge reversed
            angle=Angle_Normal(-angle0);
            if (fabs(angle-Pi2)<0.00001) angle=0;
            if (fabs(angle)<0.00001) angle=0;
            break;
        case 13:
            if (x1<=x2) angle=Angle_Normal(angle0+Pi_);
            break;
        case 14:
            if (y1<=y2) angle=Angle_Normal(angle0+Pi_);
            break;
        case 15:
            angle=Angle_Normal(-angle0);
            if (fabs(angle-Pi2)<0.00001) angle=0;
            if (fabs(angle)<0.00001) angle=0;
            break;
        default:
            break;
    }

    koc1=cos(angle-katS);
    koc2=cos(angle+katS);
    kos1=sin(angle-katS);
    kos2=sin(angle+katS);

    angle_rev=Angle_Normal(angle+Pi_);

    i = 0 ;

    if ((v->style==V_EDGE_FIXED) || (v->style==V_EDGE_FIXED_INV))
        df_seg_len_dens=df_seg_len/2.0;
    else df_seg_len_dens=df_seg_len;

    df_l0 = -df_seg_len/2;
    do
    {

        df_line_rem = measure_vector (x1, y1, x2, y2, b_first_end, df_l0,  df_seg_len_dens, &df_x, &df_y) ;
        if (TRUE == Check_if_GT (df_line_rem, df_seg_len_dens/4 /*0*/))
        {

            if ((Check_if_Equal(angle, Pi_/2))   || (Check_if_Equal(angle, Pi_*3/2)))  //vertical
            {
                a1.x=df_x; a1.y=-100.0;
                a2.x=df_x; a2.y=100.0;
                b1.x=(double)x11; b1.y=(double)y11;
                b2.x=(double)x12; b2.y=(double)y12;
                p=intersectionPoint(a1,a2,b1,b2);
                if (isnan(p.x))
                {
                    p.x = b2.x;
                    p.y = b2.y;
                }
            }
            else if ((Check_if_Equal(angle, 0)) || (Check_if_Equal(angle, Pi_))) //horizontal
            {
                a1.x=-100.0; a1.y=df_y;
                a2.x=100.0; a2.y=df_y;
                b1.x=(double)x11; b1.y=(double)y11;
                b2.x=(double)x12; b2.y=(double)y12;
                p=intersectionPoint(a1,a2,b1,b2);
                if (isnan(p.x)) {
                    p.x = b1.x;
                    p.y = b1.y;
                }
            }
            else
            {
                a1.x=df_x-100.0*koc; a1.y=df_y-100.0*kos;
                a2.x=df_x+100.0*koc; a2.y=df_y+100.0*kos;
                b1.x=(double)x11; b1.y=(double)y11;
                b2.x=(double)x12; b2.y=(double)y12;
                p=intersectionPoint(a1,a2,b1,b2);
            }

            if (isnan(p.x)) return;

            n1=1;

            switch (v->style)
            {
                case 10:
                case 17:
                    if (v->flags & 1)
                    {
                        df_x0 = p.x;
                        df_y0 = p.y;

                        df_x2 = df_x;
                        df_y2 = df_y;

                        df_x1 = df_x;

                        if ((p.y - df_y) > -0.001) {
                            df_y1 = p.y - ra1 * kos;
                            if (x1 > x2) n1 = -1;
                        } else {
                            df_y1 = p.y + ra1 * kos;
                            if (x1 < x2) n1 = -1;
                        }
                        n1*=-1;

                    }
                    else
                    {
                        df_x0=df_x;
                        df_y0=df_y;

                        df_x2 = p.x;
                        df_y2 = p.y;

                        df_x1 = df_x;

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

                        df_y1 = df_y;

                        if ((p.x - df_x) > 0.001) {
                            df_x1 = p.x - ra1 * koc;
                            if (y1 < y2) n1 = -1;
                        } else {
                            df_x1 = p.x + ra1 * koc;
                            if (y1 > y2) n1 = -1;
                        }
                        n1*=-1;
                    }
                    else {
                        df_x0 = df_x;
                        df_y0 = df_y;

                        df_x2 = p.x;
                        df_y2 = p.y;

                        df_y1 = df_y;

                        if ((p.x - df_x) > 0.001) {
                            df_x1 = df_x + ra1 * koc;
                            if (y1 < y2) n1 = -1;
                        } else {
                            df_x1 = df_x - ra1 * koc;
                            if (y1 > y2) n1 = -1;
                        }
                    }
                    break;
                case 12:
                case V_EDGE_SIMPLE:  //simple supported edge
                case V_EDGE_SIMPLE_INV:  //simple supported edge reversed
                case V_EDGE_FIXED:  //fixed edge
                case V_EDGE_FIXED_INV:  //fixed edge reversed
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

                            if (fabs(del_angle - 90) > 1.0) n1 = -1;
                        }

                        df_x1 = p.x - n1 * ra1 * koc;
                        df_y1 = p.y - n1 * ra1 * kos;

                        n1*=-1;

                    }
                    else {
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

                            if (fabs(del_angle - 90) > 1.0) n1 = -1;
                        }

                        df_x1 = df_x + n1 * ra1 * koc;
                        df_y1 = df_y + n1 * ra1 * kos;
                    }
                    break;
                case 13:
                    if (v->flags & 1)
                    {
                        df_x0 = p.x;
                        df_y0 = p.y;

                        df_x2 = df_x;
                        df_y2 = df_y;

                        df_x1 = df_x;
                        if (x1 < x2) {
                            if (p.y < df_y) n1 = -1;
                            df_y1 = p.y - n1 * ra1 * kos;
                        } else {
                            if (p.y > df_y) n1 = -1;
                            df_y1 = p.y + n1 * ra1 * kos;
                        }
                        n1*=-1;
                    }
                    else {
                        df_x0 = df_x;
                        df_y0 = df_y;

                        df_x2 = p.x;
                        df_y2 = p.y;

                        df_x1 = df_x;
                        if (x1 < x2) {
                            if (p.y < df_y) n1 = -1;
                            df_y1 = df_y + n1 * ra1 * kos;
                        } else {
                            if (p.y > df_y) n1 = -1;
                            df_y1 = df_y - n1 * ra1 * kos;
                        }
                    }
                    break;
                case 14:
                    if (v->flags & 1)
                    {
                        df_x0 = p.x;
                        df_y0 = p.y;

                        df_x2 = df_x;
                        df_y2 = df_y;

                        df_y1 = df_y;

                        if (y1 < y2) {
                            if (p.x > df_x) n1 = -1;
                            df_x1 = p.x + n1 * ra1 * koc;
                        } else {
                            if (p.x < df_x) n1 = -1;
                            df_x1 = p.x - n1 * ra1 * koc;
                        }
                        n1*=-1;
                    }
                    else
                    {
                        df_x0 = df_x;
                        df_y0 = df_y;

                        df_x2 = p.x;
                        df_y2 = p.y;

                        df_y1 = df_y;

                        if (y1 < y2) {
                            if (p.x > df_x) n1 = -1;
                            df_x1 = df_x - n1 * ra1 * koc;
                        } else {
                            if (p.x < df_x) n1 = -1;
                            df_x1 = df_x + n1 * ra1 * koc;
                        }
                    }

                    break;
                case 15:

                    df_x0=df_x;
                    df_y0=df_y;

                    n1=1;

                    Lt1.x1=df_x;
                    Lt1.y1=df_y;
                    Lt1.x2=p.x;
                    Lt1.y2=p.y;

                    parametry_lini(&Lt1, &PL1);
                    if (PL1.dl>0)
                    {
                        if (PL1.kat < kat) PL1.kat += 360;
                        del_angle = PL1.kat - kat;

                        if (fabs(del_angle - 90) > 1.0) n1 = -1;
                    }

                    df_x1 = df_x + n1*ra1 * koc;
                    df_y1 = df_y + n1*ra1 * kos;

                    df_x2 = p.x - n1*ra1*0.5 * koc;
                    df_y2 = p.y - n1*ra1*0.5 * kos;
                    break;
                default:
                    df_x1 = df_x;
                    if (x1<x2) df_y1 = df_y - ra1 * kos;
                    else df_y1 = df_y + ra1 * kos;
                    break;
            }

            if (v->style!=15) draw_arrow_to_drive(df_x0, df_y0, df_x1, df_y1, df_x2, df_y2, koc1, kos1, koc2, kos2, n1*ra, v);
            else draw_wave_to_drive(df_x0, df_y0, df_x1, df_y1, df_x2, df_y2, koc, kos, n1, ra, v);
            df_l0 += df_seg_len_dens ;
            i++ ;
        }
    }
    while (TRUE == Check_if_GT (df_line_rem, df_seg_len/2)) ;
}


BOOL Draw_Vector_To_Drive(AVECTOR *ptrs_vector, Print_Rect *window_to_print)
{
    double df_psize, df_psize1 ;
    PLINIA PL, PL1, PLth;
    LINIA L1=Ldef, L2=Ldef, L3=Ldef, L4=Ldef, Lt=Ldef, Ls=Ldef, Lth=Ldef, Ln=Ldef, Ln1=Ldef;
    SOLIDARC sa=sadef;
    WIELOKAT w=S4def;
    OKRAG O=Odef;
    OKRAG K=Kdef;
    LUK l=ldef;
    double kat1, kos, koc, kats, kata2;
    double ra;
    double katS=25.0;
    TEXT Vtxt=Tdef, Vtxt1=Tdef, Vltxt=Tdef;
    double n, dx, dy, dx1, dy1, dx2, dy2, Kp2s, K1_5, Ltx, Lty;;
    int kolorS;
    double koc1, kos1, koc2, kos2, koc1th, kos1th;
    int grubosc, s;
    double  xs, ys;
    int TRANS=25;
    float xmax, ymax;
    char* translucency_ptr;
    int translucency;
    BOOL ret;
    double Lcx, Lcy;

    double t_len_mm;
    double direction;
    double Kp2sn;

    double kat0=10;
    ELLIPSE load_ellipse=FEdef;

#define arrowf 1.0

    ra=Get_Point_Size () / arrowf;

    df_psize = Get_Point_Size () / 2;   //2
    df_psize1 = Get_Point_Size () / 4 ; //2

    L1.warstwa=O.warstwa=l.warstwa=K.warstwa=w.warstwa=sa.warstwa=ptrs_vector->warstwa;
    L1.kolor=O.kolor=l.kolor=K.kolor=w.kolor=sa.kolor=ptrs_vector->kolor;
    L1.typ=ptrs_vector->typ;

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
    L1.x2=ptrs_vector->x2;
    L1.y2=ptrs_vector->y2;
    parametry_lini(&L1, &PL);
    kat1=PL.kat;
    kos=sin(PL.kat*Pi/180);
    koc=cos(PL.kat*Pi/180);

    Vtxt.kat=normalize_txt_angle(PL.kat*Pi/180);
    Vtxt.justowanie=j_srodkowo;

    Vtxt.warstwa=ptrs_vector->warstwa;
    Vtxt.kolor=ptrs_vector->kolor;
    Vtxt.czcionka=zmwym.czcionka;
    Vtxt.wysokosc=zmwym.wysokosc;
    Vtxt.width_factor=zmwym.width_factor;
    memmove(&Vtxt1, &Vtxt, sizeof(TEXT));
    memmove(&Vltxt, &Vtxt, sizeof(TEXT));
    Vltxt.wysokosc=Vltxt.wysokosc/2;

    strcpy(Vtxt.text, "");
    strcpy(Vtxt1.text, "");

    switch (ptrs_vector->style)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            Vtxt.x=(L1.x1+L1.x2)/2;
            Vtxt.y=(L1.y1+L1.y2)/2;
            if (ptrs_vector->property_no>0) sprintf(Vtxt.text, "#%d", ptrs_vector->property_no);
            break;
        case 4:

            kos1=sin(Angle_Normal((PL.kat-90)*Pi/180));
            koc1=cos(Angle_Normal((PL.kat-90)*Pi/180));

            if (L1.x1<=L1.x2) n=1; else n=-1;

            Vtxt.x=(L1.x1+L1.x2)/2-((n*ra/4)*koc1);
            Vtxt.y=(L1.y1+L1.y2)/2-((n*ra/4)*kos1);

            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, force_precision);
            normalize_txt(&Vtxt);

            Vltxt.x=(L1.x1+L1.x2)/2+n*((Vltxt.wysokosc+ra/4)*koc1);
            Vltxt.y=(L1.y1+L1.y2)/2+n*((Vltxt.wysokosc+ra/4)*kos1);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 7:

            kos1=sin(Angle_Normal((PL.kat-90)*Pi/180));
            koc1=cos(Angle_Normal((PL.kat-90)*Pi/180));

            if (L1.x1<=L1.x2) n=1; else n=-1;

            Vtxt.x=(L1.x1+L1.x2)/2-((n*ra/4)*koc1);
            Vtxt.y=(L1.y1+L1.y2)/2-((n*ra/4)*kos1);
            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, displacement_precision);
            normalize_txt(&Vtxt);

            Vltxt.x=(L1.x1+L1.x2)/2+n*((Vltxt.wysokosc+ra/4)*koc1);
            Vltxt.y=(L1.y1+L1.y2)/2+n*((Vltxt.wysokosc+ra/4)*kos1);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 5:
        case 6:
            if (ptrs_vector->angle2<ptrs_vector->angle1)
                kata2=ptrs_vector->angle2+Pi2;
            else kata2=ptrs_vector->angle2;
            kats=Angle_Normal((ptrs_vector->angle1+kata2)/2);
            Vtxt.x=ptrs_vector->x1+(ptrs_vector->r+0.5)*cos(kats);
            Vtxt.y=ptrs_vector->y1+(ptrs_vector->r+0.5)*sin(kats);

            Vltxt.x=ptrs_vector->x1+(ptrs_vector->r - Vltxt.wysokosc - 0.5)*cos(kats);
            Vltxt.y=ptrs_vector->y1+(ptrs_vector->r - Vltxt.wysokosc - 0.5)*sin(kats);

            Vtxt.kat=Vltxt.kat=Angle_Normal(kats-Pi_/2);
            Vtxt.justowanie=Vltxt.justowanie=j_srodkowo;
            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, moment_precision);
            normalize_txt(&Vtxt);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);

            break;
        case V_EDGE_ARC_SIMPLE:  //simple supported edge
        case V_EDGE_ARC_SIMPLE_INV:  //simple supported edge flipped
        case V_EDGE_ARC_FIXED:  //fixed edge
        case V_EDGE_ARC_FIXED_INV:  //fixed edge flipped
            if (ptrs_vector->angle2<ptrs_vector->angle1)
                kata2=ptrs_vector->angle2+Pi2;
            else kata2=ptrs_vector->angle2;
            kats=Angle_Normal((ptrs_vector->angle1+kata2)/2);
            break;
        case 8:
        case 9:
            if (ptrs_vector->angle2<ptrs_vector->angle1)
                kata2=ptrs_vector->angle2+Pi2;
            else kata2=ptrs_vector->angle2;
            kats=Angle_Normal((ptrs_vector->angle1+kata2)/2);
            Vtxt.x=ptrs_vector->x1+(ptrs_vector->r+0.5)*cos(kats);
            Vtxt.y=ptrs_vector->y1+(ptrs_vector->r+0.5)*sin(kats);

            Vltxt.x=ptrs_vector->x1+(ptrs_vector->r - Vltxt.wysokosc - 0.5)*cos(kats);
            Vltxt.y=ptrs_vector->y1+(ptrs_vector->r - Vltxt.wysokosc - 0.5)*sin(kats);

            Vtxt.kat=Vltxt.kat=Angle_Normal(kats-Pi_/2);
            Vtxt.justowanie=Vltxt.justowanie=j_srodkowo;

            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, rotation_precision);
            normalize_txt(&Vtxt);

            if (ptrs_vector->variant>0)
                sprintf(Vltxt.text, "%s%d",load_symbol[(int)ptrs_vector->load], ptrs_vector->variant);
            else sprintf(Vltxt.text, "%s",load_symbol[(int)ptrs_vector->load]);
            normalize_txt(&Vltxt);
            break;
        case 10:  //trapezium Y
        case 17:  //trapezium Y slab

            if (Check_if_Equal(L1.x1, L1.x2))
            {
                L1.x2+=0.001;

                parametry_lini(&L1, &PL);
                kat1=PL.kat;
                kos=sin(PL.kat*Pi/180);
                koc=cos(PL.kat*Pi/180);

            }

            if (L1.x1<L1.x2) n=1;
            else n=-1;

            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = L1.x1;
            Lt.y1 = L1.y1 + n*(ptrs_vector->magnitude1/((ptrs_vector->style==10) ? load_magnitude : flood_magnitude));
            Lt.x2 = L1.x2;
            Lt.y2 = L1.y2 + n*(ptrs_vector->magnitude2/((ptrs_vector->style==10) ? load_magnitude : flood_magnitude));

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos1=sin(Pi*(PL1.kat+90)/180);
            koc1=cos(Pi*(PL1.kat+90)/180);

            if (Lt.x1<Lt.x2)
            {   if (Lt.y1<L1.y1)
                {
                    dx=-Vtxt.wysokosc*koc1;
                    dy=-Vtxt.wysokosc*kos1;
                }
                else
                {
                    dx = 0;
                    dy = 0;
                }
                if (Lt.y2<L1.y2)
                {
                    dx1=-Vtxt.wysokosc*koc1;
                    dy1=-Vtxt.wysokosc*kos1;

                    dx2=-Vltxt.wysokosc*koc1;
                    dy2=-Vltxt.wysokosc*kos1;
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
                    dx = Vtxt.wysokosc * koc1;
                    dy = Vtxt.wysokosc * kos1;
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
                    dx1 = Vtxt.wysokosc * koc1;
                    dy1 = Vtxt.wysokosc * kos1;
                    dx2 = Vltxt.wysokosc * koc1;
                    dy2 = Vltxt.wysokosc * kos1;
                }
            }

            Vtxt.x=Lt.x1+dx;
            Vtxt.y=Lt.y1+dy;
            Vtxt.kat=normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt.justowanie=j_do_lewej;
            else Vtxt.justowanie=j_do_prawej;
            Vtxt1.x=Lt.x2+dx1;
            Vtxt1.y=Lt.y2+dy1;
            Vtxt1.kat=Vltxt.kat=normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt1.justowanie=j_do_prawej;
            else  Vtxt1.justowanie=j_do_lewej;

            Vltxt.x=Ltx+dx2;
            Vltxt.y=Lty+dy2;
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
                L1.y2+=0.001;

                parametry_lini(&L1, &PL);
                kat1=PL.kat;
                kos=sin(PL.kat*Pi/180);
                koc=cos(PL.kat*Pi/180);

            }
        
            if (L1.y1<L1.y2) n=1;
            else n=-1;

            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = L1.x1 - n*(ptrs_vector->magnitude1/load_magnitude);
            Lt.y1 = L1.y1;
            Lt.x2 = L1.x2 - n*(ptrs_vector->magnitude2/load_magnitude);
            Lt.y2 = L1.y2;

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos1=sin(Pi*(PL1.kat+90)/180);
            koc1=cos(Pi*(PL1.kat+90)/180);

            if (Lt.y1<Lt.y2)
            {   if (Lt.x1>L1.x1)
                {
                    dx=-Vtxt.wysokosc*koc1;
                    dy=-Vtxt.wysokosc*kos1;
                }
                else
                {
                    dx = 0;
                    dy = 0;
                }
                if (Lt.x2>L1.x2)
                {
                    dx1=-Vtxt.wysokosc*koc1;
                    dy1=-Vtxt.wysokosc*kos1;
                    dx2=-Vltxt.wysokosc*koc1;
                    dy2=-Vltxt.wysokosc*kos1;
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
                    dx = -Vtxt.wysokosc * koc1;
                    dy = -Vtxt.wysokosc * kos1;
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
                    dx1 = -Vtxt.wysokosc * koc1;
                    dy1 = -Vtxt.wysokosc * kos1;
                    dx2 = -Vltxt.wysokosc * koc1;
                    dy2 = -Vltxt.wysokosc * kos1;
                }
            }

            Vtxt.x=Lt.x1+dx;
            Vtxt.y=Lt.y1+dy;
            Vtxt.kat=(PL1.kat*Pi/180);
            Vtxt.justowanie=j_do_lewej;
            Vtxt1.x=Lt.x2+dx1;
            Vtxt1.y=Lt.y2+dy1;
            Vtxt1.kat=Vltxt.kat=(PL1.kat*Pi/180);
            Vtxt1.justowanie=j_do_prawej;

            Vltxt.x=Ltx+dx2;
            Vltxt.y=Lty+dy2;
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

            kos1=sin(Pi*(PL.kat+90)/180);
            koc1=cos(Pi*(PL.kat+90)/180);

            n=1;
            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = L1.x1 + n*(ptrs_vector->magnitude1/load_magnitude)*koc1;
            Lt.y1 = L1.y1 + n*(ptrs_vector->magnitude1/load_magnitude)*kos1;
            Lt.x2 = L1.x2 + n*(ptrs_vector->magnitude2/load_magnitude)*koc1;
            Lt.y2 = L1.y2 + n*(ptrs_vector->magnitude2/load_magnitude)*kos1;

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos2=sin(Pi*(PL1.kat+90)/180);
            koc2=cos(Pi*(PL1.kat+90)/180);

            dx = 0;
            dy = 0;
            dx1 = 0;
            dy1 = 0;
            dx2 = 0;
            dy2 = 0;

            //if (ptrs_vector->magnitude1<0)
            if (((ptrs_vector->magnitude1<0) && !(ptrs_vector->flags & 1)) ||
                ((ptrs_vector->magnitude1>0) && (ptrs_vector->flags & 1)))
            {
                dx = -Vtxt.wysokosc * koc2;
                dy = -Vtxt.wysokosc * kos2;
            }

            //if (ptrs_vector->magnitude2<0)
            if (((ptrs_vector->magnitude2<0) && !(ptrs_vector->flags & 1)) ||
                ((ptrs_vector->magnitude2>0) && (ptrs_vector->flags & 1)))
            {
                dx1 = -Vtxt.wysokosc * koc2;
                dy1 = -Vtxt.wysokosc * kos2;

                dx2 = -Vltxt.wysokosc * koc2;
                dy2 = -Vltxt.wysokosc * kos2;
            }

            Vtxt.x=Lt.x1+dx;
            Vtxt.y=Lt.y1+dy;
            Vtxt.kat=(PL1.kat*Pi/180);
            Vtxt.justowanie=j_do_lewej;
            Vtxt1.x=Lt.x2+dx1;
            Vtxt1.y=Lt.y2+dy1;
            Vtxt1.kat=Vltxt.kat=(PL1.kat*Pi/180);
            Vtxt1.justowanie=j_do_prawej;

            Vltxt.x=Ltx+dx2;
            Vltxt.y=Lty+dy2;
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
            if ((ptrs_vector->style==V_EDGE_SIMPLE) || (ptrs_vector->style==V_EDGE_FIXED))
                ptrs_vector->magnitude1=ptrs_vector->magnitude2=0.01;  //just to simulate
            else ptrs_vector->magnitude1=ptrs_vector->magnitude2=-0.01;  //just to simulate
            kos1=sin(Pi*(PL.kat+90)/180);
            koc1=cos(Pi*(PL.kat+90)/180);

            n=1;

            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = L1.x1 + n*(ptrs_vector->magnitude1/load_magnitude)*koc1;
            Lt.y1 = L1.y1 + n*(ptrs_vector->magnitude1/load_magnitude)*kos1;
            Lt.x2 = L1.x2 + n*(ptrs_vector->magnitude2/load_magnitude)*koc1;
            Lt.y2 = L1.y2 + n*(ptrs_vector->magnitude2/load_magnitude)*kos1;

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos2=sin(Pi*(PL1.kat+90)/180);
            koc2=cos(Pi*(PL1.kat+90)/180);
            break;

        case 13:  //trapezium H
            if (L1.x1<L1.x2)
            {
                n=1;
                ymax=max(L1.y1, L1.y2);
            }
            else
            {
                n=-1;
                ymax=min(L1.y1, L1.y2);
            }

            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = L1.x1;
            Lt.y1 = ymax + n*(ptrs_vector->magnitude1/load_magnitude);
            Lt.x2 = L1.x2;
            Lt.y2 = ymax + n*(ptrs_vector->magnitude2/load_magnitude);

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos1=sin(Pi*(PL1.kat+90)/180);
            koc1=cos(Pi*(PL1.kat+90)/180);

            dx=0;
            dy=0;
            dx1=0;
            dy1=0;
            dx2=0;
            dy2=0;

            if (Lt.x1<Lt.x2)
            {
                if (((ptrs_vector->magnitude1<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude1>0) && (ptrs_vector->flags & 1)))
                {
                    dx=-Vtxt.wysokosc*koc1;
                    dy=-Vtxt.wysokosc*kos1;
                }
                if (((ptrs_vector->magnitude2<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude2>0) && (ptrs_vector->flags & 1)))
                {
                    dx1=-Vtxt.wysokosc*koc1;
                    dy1=-Vtxt.wysokosc*kos1;

                    dx2=-Vltxt.wysokosc*koc1;
                    dy2=-Vltxt.wysokosc*kos1;
                }
            }
            else
            {
                if (((ptrs_vector->magnitude1>=0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude1<0) && (ptrs_vector->flags & 1)))
                {
                    dx = Vtxt.wysokosc * koc1;
                    dy = Vtxt.wysokosc * kos1;
                }
                if (((ptrs_vector->magnitude2>=0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude2<0) && (ptrs_vector->flags & 1)))
                {
                    dx1 = Vtxt.wysokosc * koc1;
                    dy1 = Vtxt.wysokosc * kos1;

                    dx2 = Vltxt.wysokosc * koc1;
                    dy2 = Vltxt.wysokosc * kos1;
                }
            }

            Vtxt.x=Lt.x1+dx;
            Vtxt.y=Lt.y1+dy;
            Vtxt.kat=normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt.justowanie=j_do_lewej;
            else Vtxt.justowanie=j_do_prawej;
            Vtxt1.x=Lt.x2+dx1;
            Vtxt1.y=Lt.y2+dy1;
            Vtxt1.kat=Vltxt.kat=normalize_txt_angle(PL1.kat*Pi/180);
            if (Lt.x1<Lt.x2) Vtxt1.justowanie=j_do_prawej;
            else  Vtxt1.justowanie=j_do_lewej;

            Vltxt.x=Ltx+dx2;
            Vltxt.y=Lty+dy2;
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

            if (L1.y1<L1.y2)
            {
                n=1;
                xmax=min(L1.x1, L1.x2);
            }
            else
            {
                n=-1;
                xmax=max(L1.x1, L1.x2);
            }

            if (ptrs_vector->flags & 1) n*=-1;

            Lt.x1 = xmax - n*(ptrs_vector->magnitude1/load_magnitude);
            Lt.y1 = L1.y1;
            Lt.x2 = xmax - n*(ptrs_vector->magnitude2/load_magnitude);
            Lt.y2 = L1.y2;

            Ltx=(Lt.x1 + Lt.x2)/2;
            Lty=(Lt.y1 + Lt.y2)/2;

            parametry_lini(&Lt, &PL1);
            kos1=sin(Pi*(PL1.kat+90)/180);
            koc1=cos(Pi*(PL1.kat+90)/180);

            dx=0;
            dy=0;
            dx1=0;
            dy1=0;
            dx2=0;
            dy2=0;

            if (Lt.y1<Lt.y2)
            {
                if (((ptrs_vector->magnitude1<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude1>0) && (ptrs_vector->flags & 1)))
                {
                    dx=-Vtxt.wysokosc*koc1;
                    dy=-Vtxt.wysokosc*kos1;
                }
                if (((ptrs_vector->magnitude2<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude2>0) && (ptrs_vector->flags & 1)))
                {
                    dx1=-Vtxt.wysokosc*koc1;
                    dy1=-Vtxt.wysokosc*kos1;

                    dx2=-Vltxt.wysokosc*koc1;
                    dy2=-Vltxt.wysokosc*kos1;
                }
            }
            else
            {
                if (((ptrs_vector->magnitude1<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude1>0) && (ptrs_vector->flags & 1)))
                {
                    dx = -Vtxt.wysokosc * koc1;
                    dy = -Vtxt.wysokosc * kos1;
                }
                if (((ptrs_vector->magnitude2<0) && !(ptrs_vector->flags & 1)) ||
                    ((ptrs_vector->magnitude2>0) && (ptrs_vector->flags & 1)))
                {
                    dx1 = -Vtxt.wysokosc * koc1;
                    dy1 = -Vtxt.wysokosc * kos1;

                    dx2 = -Vltxt.wysokosc * koc1;
                    dy2 = -Vltxt.wysokosc * kos1;
                }
            }

            Vtxt.x=Lt.x1+dx;
            Vtxt.y=Lt.y1+dy;
            Vtxt.kat=(PL1.kat*Pi/180);
            Vtxt.justowanie=j_do_lewej;
            Vtxt1.x=Lt.x2+dx1;
            Vtxt1.y=Lt.y2+dy1;
            Vtxt1.kat=Vltxt.kat=(PL1.kat*Pi/180);
            Vtxt1.justowanie=j_do_prawej;

            Vltxt.x=Ltx+dx2;
            Vltxt.y=Lty+dy2;
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
        case 15:

            kos1=sin(Pi*(PL.kat+90)/180);
            koc1=cos(Pi*(PL.kat+90)/180);

            Lth.x1 = (L1.x1+L1.x2)/2 + (ptrs_vector->r/depth_magnitude)*koc1;  //thermal_depth_size
            Lth.y1 = (L1.y1+L1.y2)/2 + (ptrs_vector->r/depth_magnitude)*kos1;
            Lth.x2 = (L1.x1+L1.x2)/2 - (ptrs_vector->r/depth_magnitude)*koc1;
            Lth.y2 = (L1.y1+L1.y2)/2 - (ptrs_vector->r/depth_magnitude)*kos1;

            parametry_lini(&Lth, &PLth);
            kos1th=sin(Pi*(PLth.kat+90)/180);
            koc1th=cos(Pi*(PLth.kat+90)/180);

            Lt.x1 = Lth.x1 + (ptrs_vector->magnitude1/thermal_magnitude)*koc1th;
            Lt.y1 = Lth.y1 + (ptrs_vector->magnitude1/thermal_magnitude)*kos1th;
            Lt.x2 = Lth.x2 + (ptrs_vector->magnitude2/thermal_magnitude)*koc1th;
            Lt.y2 = Lth.y2 + (ptrs_vector->magnitude2/thermal_magnitude)*kos1th;

            parametry_lini(&Lt, &PL1);
            kos2=sin(Pi*(PL1.kat+90)/180);
            koc2=cos(Pi*(PL1.kat+90)/180);

            dx = 0;
            dy = 0;
            dx1 = 0;
            dy1 = 0;

            if (ptrs_vector->magnitude1<0)
            {
                dx = -Vtxt.wysokosc * koc2;
                dy = -Vtxt.wysokosc * kos2;
            }

            if (ptrs_vector->magnitude2<0)
            {
                dx1 = -Vtxt.wysokosc * koc2;
                dy1 = -Vtxt.wysokosc * kos2;
            }

            Vtxt.x=Lt.x1+dx;
            Vtxt.y=Lt.y1+dy;
            Vtxt.kat=(PL1.kat*Pi/180);
            Vtxt.justowanie=j_do_lewej;
            Vtxt1.x=Lt.x2+dx1;
            Vtxt1.y=Lt.y2+dy1;
            Vtxt1.kat=(PL1.kat*Pi/180);
            Vtxt1.justowanie=j_do_prawej;

            set_decimal_format(Vtxt.text, ptrs_vector->magnitude1, thermal_precision);
            set_decimal_format(Vtxt1.text, ptrs_vector->magnitude2, thermal_precision);

            normalize_txt(&Vtxt);
            normalize_txt(&Vtxt1);
            break;
        case 16:
            Ln.x1 = L1.x1 + (ptrs_vector->r/radius_magnitude)*koc;   //thermal_depth_size
            Ln.y1 = L1.y1 + (ptrs_vector->r/radius_magnitude)*kos;
            Ln.x2 = L1.x2;
            Ln.y2 = L1.y2;

            Vtxt.x=L1.x2;
            Vtxt.y=L1.y2;
            Vtxt.kat=0;
            Vtxt.wysokosc=zmwym.wysokosc*0.75;
            Vtxt.justowanie=j_do_lewej;

            set_decimal_format(Vtxt1.text, ptrs_vector->magnitude1, dim_precision);
            sprintf(Vtxt.text,"R%s",Vtxt1.text);
            break;
        default:
            break;
    }

    Vtxt.dl = strlen(Vtxt.text);
    Vtxt1.dl = strlen(Vtxt1.text);
    Vtxt.n = T18 + Vtxt.dl;
    Vtxt1.n = T18 + Vtxt1.dl;

    L2.x1 = L1.x1 + ra * koc;
    L2.y1 = L1.y1 + ra * kos;
    L2.x2 = L1.x2 - ra * koc;
    L2.y2 = L1.y2 - ra * kos;

    L3.x1 = L1.x1 + df_psize * koc;
    L3.y1 = L1.y1 + df_psize * kos;
    L3.x2 = L1.x2 - df_psize * koc;
    L3.y2 = L1.y2 - df_psize * kos;

    L4.x1 = L1.x1 + df_psize1 * koc;
    L4.y1 = L1.y1 + df_psize1 * kos;
    L4.x2 = L1.x2 - df_psize1 * koc;
    L4.y2 = L1.y2 - df_psize1 * kos;

    switch(ptrs_vector->style)
    {
        case 0:

            line_width_type = Line_Width (L4.typ);
            if (Draw_Line_To_Drive(&L4) == FALSE) return FALSE;

            K.x=ptrs_vector->x1;
            K.y=ptrs_vector->y1;
            K.r=df_psize1;

            line_width_type = LINE_MIN_WIDTH ;
            if (Draw_Fill_Circle_To_Drive (&K) == FALSE) return FALSE;

            K.x=ptrs_vector->x2;
            K.y=ptrs_vector->y2;
            if (Draw_Fill_Circle_To_Drive (&K) == FALSE) return FALSE;


            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;

            break;
        case 1:

            L4.x2=L3.x2;
            L4.y2=L3.y2;
            line_width_type = Line_Width (L4.typ);
            if (Draw_Line_To_Drive(&L4) == FALSE) return FALSE;

            K.x=ptrs_vector->x1;
            K.y=ptrs_vector->y1;
            K.r=df_psize1;

            line_width_type = LINE_MIN_WIDTH ;
            if (Draw_Fill_Circle_To_Drive (&K) == FALSE) return FALSE;

            l.typ = 64;
            l.x = ptrs_vector->x2;
            l.y = ptrs_vector->y2;
            l.r = df_psize;
            l.kat1 = 0;
            l.kat2 = Pi2;

            line_width_type = Line_Width (l.typ);
            if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;

            //out_v_krz (pikseleX0 (ptrs_vector->x2), pikseleY0(ptrs_vector->y2));

            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;

            break;
        case 2:

            L4.x1=L3.x1;
            L4.y1=L3.y1;
            line_width_type = Line_Width (L4.typ);
            if (Draw_Line_To_Drive(&L4) == FALSE) return FALSE;

            K.x=ptrs_vector->x2;
            K.y=ptrs_vector->y2;
            K.r=df_psize1;
            line_width_type = LINE_MIN_WIDTH ;
            if (Draw_Fill_Circle_To_Drive (&K) == FALSE) return FALSE;

            l.typ = 64;
            l.x = ptrs_vector->x1;
            l.y = ptrs_vector->y1;
            l.r = df_psize;
            l.kat1 = 0;
            l.kat2 = Pi2;
            line_width_type = Line_Width (l.typ);
            if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;

            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;

            break;
        case 3:
            line_width_type = Line_Width (L3.typ);
            if (Draw_Line_To_Drive(&L3) == FALSE) return FALSE;

            l.typ = 64;
            l.x = ptrs_vector->x1;
            l.y = ptrs_vector->y1;
            l.r = df_psize;
            l.kat1 = 0;
            l.kat2 = Pi2;
             line_width_type = Line_Width (l.typ);
            if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;

            l.x = ptrs_vector->x2;
            l.y = ptrs_vector->y2;
            if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;

            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;

            break;
        case 4:  //force
            w.atrybut=ptrs_vector->atrybut;
            w.warstwa=ptrs_vector->warstwa;
            w.kolor=ptrs_vector->kolor;

            if (PL.dl>ra)
            {
                Ls.x2 = ptrs_vector->x2;
                Ls.y2 = ptrs_vector->y2;
                Ls.x1 = L2.x1;
                Ls.y1 = L2.y1;

                create_solid_on_line(&Ls, &w, ra / 4, ra / 4, 0);

                line_width_type = Solid_Line_Width(w.empty_typ);
                if (FALSE == Draw_Wielokat_To_Drive(&w, window_to_print)) return FALSE;
            }

            Kp2s = Get_Point_Size () / (arrowf*cos(Pi*katS/180)) ;

            koc1=cos(Pi*(kat1-katS)/180);
            koc2=cos(Pi*(kat1+katS)/180);
            kos1=sin(Pi*(kat1-katS)/180);
            kos2=sin(Pi*(kat1+katS)/180);

            w.xy[2]=ptrs_vector->x1;
            w.xy[3]=ptrs_vector->y1;
            w.xy[0]=ptrs_vector->x1+Kp2s*koc1;
            w.xy[1]=ptrs_vector->y1+Kp2s*kos1;
            w.xy[4]=ptrs_vector->x1+Kp2s*koc2;
            w.xy[5]=ptrs_vector->y1+Kp2s*kos2;

            w.lp=6;
            w.n=32;

            line_width_type = Solid_Line_Width (w.empty_typ);
            if (FALSE == Draw_Wielokat_To_Drive(&w, window_to_print)) return FALSE;

            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vltxt, 0, 1, 1) == FALSE) return FALSE;
            break;
        case 7:  //displacement

            grubosc=(ptrs_vector->typ & 224 ) / 32;

            L3.x2=L2.x2;
            L3.y2=L2.y2;
            L3.typ=grubosc*32 + 1;

            line_width_type = Line_Width (L3.typ);
            if (Draw_Line_To_Drive(&L3) == FALSE) return FALSE;

            l.typ = 64;
            l.x = ptrs_vector->x1;
            l.y = ptrs_vector->y1;
            l.r = df_psize;
            l.kat1 = 0;
            l.kat2 = Pi2;

            line_width_type = Line_Width (l.typ);
            if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;

            Kp2s = Get_Point_Size () / (arrowf*cos(Pi*katS/180)) ;

            koc1=cos(Pi*(kat1-katS)/180);
            koc2=cos(Pi*(kat1+katS)/180);
            kos1=sin(Pi*(kat1-katS)/180);
            kos2=sin(Pi*(kat1+katS)/180);

            w.xy[2]=ptrs_vector->x2;
            w.xy[3]=ptrs_vector->y2;
            w.xy[0]=ptrs_vector->x2-Kp2s*koc1;
            w.xy[1]=ptrs_vector->y2-Kp2s*kos1;
            w.xy[4]=ptrs_vector->x2-Kp2s*koc2;
            w.xy[5]=ptrs_vector->y2-Kp2s*kos2;

            w.lp=6;
            w.n=32;
            w.empty_typ=3;

            line_width_type = Solid_Line_Width (w.empty_typ);

            if (FALSE == Draw_Wielokat_To_Drive(&w, window_to_print)) return FALSE;

            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vltxt, 0, 1, 1) == FALSE) return FALSE;
            break;
        case 5:  //moment
        case 6:
        case 8:  //rotation
        case 9:
        case V_EDGE_ARC_SIMPLE:  //simple supported edge
        case V_EDGE_ARC_SIMPLE_INV:  //simple supported edge flipped
        case V_EDGE_ARC_FIXED:  //fixed edge
        case V_EDGE_ARC_FIXED_INV:  //fixed edge flipped

            if (ptrs_vector->style==5)
            {
                linestyle(ptrs_vector->typ);
                kats = ptrs_vector->angle2 - Pi_ / 2;
                xs = ptrs_vector->x1 + ptrs_vector->r * cos(ptrs_vector->angle2);
                ys = ptrs_vector->y1 + ptrs_vector->r * sin(ptrs_vector->angle2);
                n = +1;
                s = 0;

                sa.atrybut = ptrs_vector->atrybut;
                sa.warstwa = ptrs_vector->warstwa;
                sa.kolor = ptrs_vector->kolor;
                sa.x = ptrs_vector->x1;
                sa.y = ptrs_vector->y1;
                sa.r = ptrs_vector->r;
                sa.kat1 = ptrs_vector->angle1;
                sa.kat2 = ptrs_vector->angle2;
                sa.width1 = ra / 4;
                sa.width2 = ra / 4;
                line_width_type = Solid_Line_Width (sa.empty_typ);
                if (Draw_Solidarc_To_Drive(&sa, window_to_print) == FALSE) return FALSE;
            }
            else if (ptrs_vector->style==6)
            {
                linestyle(ptrs_vector->typ);
                kats = ptrs_vector->angle1 + Pi_ / 2;
                xs = ptrs_vector->x1 + ptrs_vector->r * cos(ptrs_vector->angle1);
                ys = ptrs_vector->y1 + ptrs_vector->r * sin(ptrs_vector->angle1);
                n = -1;
                s = 0;

                sa.atrybut = ptrs_vector->atrybut;
                sa.warstwa = ptrs_vector->warstwa;
                sa.kolor = ptrs_vector->kolor;
                sa.x = ptrs_vector->x1;
                sa.y = ptrs_vector->y1;
                sa.r = ptrs_vector->r;
                sa.kat1 = ptrs_vector->angle1;
                sa.kat2 = ptrs_vector->angle2;
                sa.width1 = ra / 4;
                sa.width2 = ra / 4;
                line_width_type = Solid_Line_Width (sa.empty_typ);
                if (Draw_Solidarc_To_Drive(&sa, window_to_print) == FALSE) return FALSE;
            }
            else if (ptrs_vector->style==8)
            {
                grubosc = (ptrs_vector->typ & 224) / 32;
                linestyle(grubosc * 32 + 1);
                kats = ptrs_vector->angle2 - Pi_ / 2;
                n = +1;
                s = 1;
                xs = ptrs_vector->x1 + ptrs_vector->r * cos(ptrs_vector->angle2);
                ys = ptrs_vector->y1 + ptrs_vector->r * sin(ptrs_vector->angle2);

                l.x = ptrs_vector->x1;
                l.y = ptrs_vector->y1;
                l.r = ptrs_vector->r;
                l.kat1 = ptrs_vector->angle1;
                l.kat2 = ptrs_vector->angle2;
                l.typ = grubosc * 32 + 1;
                line_width_type = Line_Width (l.typ);
                if (Draw_Arc_To_Drive(&l) == FALSE) return FALSE;
            }
            else if (ptrs_vector->style==9)
            {
                grubosc = (ptrs_vector->typ & 224) / 32;
                linestyle(grubosc * 32 + 1);
                kats = ptrs_vector->angle1 + Pi_ / 2;
                n = -1;
                s = 1;
                xs = ptrs_vector->x1 + ptrs_vector->r * cos(ptrs_vector->angle1);
                ys = ptrs_vector->y1 + ptrs_vector->r * sin(ptrs_vector->angle1);

                l.x = ptrs_vector->x1;
                l.y = ptrs_vector->y1;
                l.r = ptrs_vector->r;
                l.kat1 = ptrs_vector->angle1;
                l.kat2 = ptrs_vector->angle2;
                l.typ = grubosc * 32 + 1;
                line_width_type = Line_Width (l.typ);
                if (Draw_Arc_To_Drive(&l) == FALSE) return FALSE;
            }
            else if ((ptrs_vector->style==V_EDGE_ARC_SIMPLE) ||  //simple supported edge
                     (ptrs_vector->style==V_EDGE_ARC_SIMPLE_INV) ||  //simple supported edge flipped
                     (ptrs_vector->style==V_EDGE_ARC_FIXED) ||  //fixed edge
                     (ptrs_vector->style==V_EDGE_ARC_FIXED_INV))  //fixed edge flipped
            {
                grubosc = (ptrs_vector->typ & 224) / 32;
                linestyle(grubosc * 32 + 1);
                kats=ptrs_vector->angle2-Pi_/2;
                n=+1;
                s=1;
                xs=ptrs_vector->x1+ptrs_vector->r*cos(ptrs_vector->angle2);
                ys=ptrs_vector->y1+ptrs_vector->r*sin(ptrs_vector->angle2);

                l.x=ptrs_vector->x1;
                l.y=ptrs_vector->y1;
                l.r=ptrs_vector->r;
                l.kat1=ptrs_vector->angle1;
                l.kat2=ptrs_vector->angle2;
                l.typ=ptrs_vector->typ;
                line_width_type = Line_Width (l.typ);
                if (Draw_Arc_To_Drive(&l) == FALSE) return FALSE;
                //triangles or dashes
                make_arcarrows_to_drive(&l, ptrs_vector, PL.kat);

                break;
            }
            else return FALSE;

            dx = Get_Point_Size() / arrowf * cos(kats);
            dy = Get_Point_Size() / arrowf * sin(kats);

            xs -= dx;
            ys -= dy;


            Kp2s = Get_Point_Size() / (arrowf * cos(Pi * katS / 180));

            koc1 = cos(kats - Pi * (katS) / 180);
            koc2 = cos(kats + Pi * (katS) / 180);
            kos1 = sin(kats - Pi * (katS) / 180);
            kos2 = sin(kats + Pi * (katS) / 180);

            w.xy[2] = xs;
            w.xy[3] = ys;
            w.xy[0] = xs + Kp2s * koc1;
            w.xy[1] = ys + Kp2s * kos1;
            w.xy[4] = xs + Kp2s * koc2;
            w.xy[5] = ys + Kp2s * kos2;
            w.lp = 6;
            w.n = 32;

            if (s == 0) {
                w.empty_typ = 0;
            } else {
                w.empty_typ = 3;
            }

            line_width_type = Solid_Line_Width (w.empty_typ);
            if (FALSE == Draw_Wielokat_To_Drive(&w, window_to_print)) return FALSE;

            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vltxt, 0, 1, 1) == FALSE) return FALSE;

            break;
            case 10:  //trapezium Y
            case 17:  //trapezium Y slab

                if (ptrs_vector->style==17)  //slab load
                {
                    load_ellipse.kolor=ptrs_vector->kolor;
                    load_ellipse.warstwa=ptrs_vector->warstwa;
                    load_ellipse.widoczny=ptrs_vector->widoczny;
                    load_ellipse.translucency=51;
                    load_ellipse.typ=64;
                    load_ellipse.x=(float)((ptrs_vector->x1+ptrs_vector->x2)/2.);
                    load_ellipse.y=(float)((ptrs_vector->y1+ptrs_vector->y2)/2.);
                    load_ellipse.angle=(float)(PL.kat*Pi/180);
                    load_ellipse.rx=(float)(PL.dl/2.);
                    load_ellipse.ry=(float)(load_ellipse.rx/3.);
                    Draw_Ellipse_To_Drive(&load_ellipse);
                }

            L1.typ = 64;
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            w.empty_typ = 0;

            w.xy[0] = L1.x1;
            w.xy[1] = L1.y1;
            w.xy[2] = L1.x2;
            w.xy[3] = L1.y2;
            w.xy[4] = L1.x2;
            w.xy[5] = Lt.y2;
            w.xy[6] = L1.x1;
            w.xy[7] = Lt.y1;
            w.lp = 8;
            w.n = 40;

            Normalize_Solid(&w);

            L1.typ = 64;
            L1.x1 = w.xy[2];
            L1.y1 = w.xy[3];
            L1.x2 = w.xy[4];
            L1.y2 = w.xy[5];
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x1 = w.xy[6];
            L1.y1 = w.xy[7];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x2 = w.xy[0];
            L1.y2 = w.xy[1];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            w.translucent = 1;
            translucency = TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            line_width_type = Solid_Line_Width (w.empty_typ);

            if (type__drive != PLT_DRIVE) {
                if (FALSE == Draw_Wielokat_To_Drive(&w, window_to_print)) return FALSE;
            }
            make_arrows_to_drive(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], Pi_ / 2, ptrs_vector, PL.kat);

            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vtxt1, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vltxt, 0, 1, 1) == FALSE) return FALSE;

            break;
        case 11:  //trapezium X

            L1.typ=64;
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            w.empty_typ=0;

            w.xy[0]=L1.x1;
            w.xy[1]=L1.y1;
            w.xy[2]=L1.x2;
            w.xy[3]=L1.y2;
            w.xy[4]=Lt.x2;
            w.xy[5]=L1.y2;
            w.xy[6]=Lt.x1;
            w.xy[7]=L1.y1;
            
            w.lp=8;
            w.n=40;

            Normalize_Solid(&w);

            L1.typ=64;
            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=w.xy[4];
            L1.y2=w.xy[5];
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x1=w.xy[6];
            L1.y1=w.xy[7];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x2=w.xy[0];
            L1.y2=w.xy[1];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            line_width_type = Solid_Line_Width (w.empty_typ);
            if (type__drive != PLT_DRIVE) {
                if (FALSE == Draw_Wielokat_To_Drive(&w, window_to_print)) return FALSE;
            }

            make_arrows_to_drive(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], 0, ptrs_vector, PL.kat);

            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vtxt1, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vltxt, 0, 1, 1) == FALSE) return FALSE;

            break;
        case 12:  //trapezium N
            L1.typ=64;
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

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

            Normalize_Solid(&w);

            L1.typ=64;
            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=w.xy[4];
            L1.y2=w.xy[5];
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x1=w.xy[6];
            L1.y1=w.xy[7];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x2=w.xy[0];
            L1.y2=w.xy[1];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            line_width_type = Solid_Line_Width (w.empty_typ);
            if (type__drive != PLT_DRIVE) {
                if (FALSE == Draw_Wielokat_To_Drive(&w, window_to_print)) return FALSE;
            }

            make_arrows_to_drive(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], Pi_*(PL.kat+90.0)/180.0, ptrs_vector, PL.kat);

            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vtxt1, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vltxt, 0, 1, 1) == FALSE) return FALSE;

            break;
        case V_EDGE_SIMPLE:  //simple supported edge
        case V_EDGE_SIMPLE_INV:  //simple supported edge reversed
        case V_EDGE_FIXED:  //fixed edge
        case V_EDGE_FIXED_INV:  //fixed edge reversed
            L1.typ=ptrs_vector->typ; //64;
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

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

            make_arrows_to_drive(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], Pi_ * (PL.kat + 90.0) / 180.0, ptrs_vector, PL.kat);

            break;
        case 13:  //trapezium H
            L1.typ=64;
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            if (L1.x1<L1.x2)
            {
                n=1;
                w.xy[1]=w.xy[3]=max(L1.y1, L1.y2);
            }
            else
            {
                n=-1;
                w.xy[1]=w.xy[3]=min(L1.y1, L1.y2);
            }

            w.xy[0]=L1.x1;
            w.xy[2]=L1.x2;
            w.xy[4]=L1.x2;
            w.xy[5]=Lt.y2;
            w.xy[6]=L1.x1;
            w.xy[7]=Lt.y1;

            Normalize_Solid(&w);

            L1.typ=33;
            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=L1.x1;
            L1.y2=L1.y1;
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=L1.x2;
            L1.y2=L1.y2;
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.typ=64;
            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=w.xy[2];
            L1.y2=w.xy[3];
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x1=w.xy[4];
            L1.y1=w.xy[5];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x2=w.xy[6];
            L1.y2=w.xy[7];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            line_width_type = Solid_Line_Width (w.empty_typ);
            if (type__drive != PLT_DRIVE) {
                if (FALSE == Draw_Wielokat_To_Drive(&w, window_to_print)) return FALSE;
            }

            make_arrows_to_drive(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], Pi_/2, ptrs_vector, PL.kat);

            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vtxt1, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vltxt, 0, 1, 1) == FALSE) return FALSE;

            break;
        case 14:  //trapezium V

            L1.typ=64;
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;



            if (L1.y1<L1.y2)
            {
                n=1;
                w.xy[0]=w.xy[2]=min(L1.x1, L1.x2);
            }
            else
            {
                n=-1;
                w.xy[0]=w.xy[2]=max(L1.x1, L1.x2);
            }

            w.xy[1]=L1.y1;
            w.xy[3]=L1.y2;
            w.xy[4]=Lt.x2;
            w.xy[5]=L1.y2;
            w.xy[6]=Lt.x1;
            w.xy[7]=L1.y1;

            Normalize_Solid(&w);

            L1.typ=33;
            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=L1.x1;
            L1.y2=L1.y1;
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=L1.x2;
            L1.y2=L1.y2;
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.typ=64;
            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            L1.x2=w.xy[2];
            L1.y2=w.xy[3];
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x1=w.xy[4];
            L1.y1=w.xy[5];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x2=w.xy[6];
            L1.y2=w.xy[7];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x1=w.xy[0];
            L1.y1=w.xy[1];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            line_width_type = Solid_Line_Width (w.empty_typ);
            if (type__drive != PLT_DRIVE) {
                if (FALSE == Draw_Wielokat_To_Drive(&w, window_to_print)) return FALSE;
            }

            make_arrows_to_drive(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], 0, ptrs_vector, PL.kat);

            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vtxt1, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vltxt, 0, 1, 1) == FALSE) return FALSE;

            break;

        case 15: //thermal
            L1.typ=64;
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            if (Draw_Line_To_Drive(&Lth) == FALSE) return FALSE;

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

            Normalize_Solid(&w);

            L1.typ=64;
            L1.x1=w.xy[2];
            L1.y1=w.xy[3];
            L1.x2=w.xy[4];
            L1.y2=w.xy[5];
            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x1=w.xy[6];
            L1.y1=w.xy[7];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            L1.x2=w.xy[0];
            L1.y2=w.xy[1];
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            w.translucent=1;
            translucency=TRANS;

            translucency_ptr = (char*)w.xy;
            translucency_ptr += (w.lp * sizeof(float));
            memmove(translucency_ptr, &translucency, sizeof(unsigned char));

            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char);

            line_width_type = Solid_Line_Width (w.empty_typ);
            if (type__drive != PLT_DRIVE) {
                if (FALSE == Draw_Wielokat_To_Drive(&w, window_to_print)) return FALSE;
            }

            make_arrows_to_drive(w.xy[0], w.xy[1], w.xy[2], w.xy[3], w.xy[6], w.xy[7], w.xy[4], w.xy[5], Pi_*(PLth.kat+90.0)/180.0, ptrs_vector, PLth.kat);

            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;
            if (Draw_Tekst_To_Drive(&Vtxt1, 0, 1, 1) == FALSE) return FALSE;

            break;
        case 16: //node radius

            PRINT_TRANSLUCENCY=TRANS;
            K.x=ptrs_vector->x1;
            K.y=ptrs_vector->y1;
            K.r=ptrs_vector->r;

            line_width_type = LINE_MIN_WIDTH ;
            if (Draw_Fill_Trans_Circle_To_Drive (&K, PRINT_TRANSLUCENCY) == FALSE) return FALSE;

            PRINT_TRANSLUCENCY=255;

            l.typ = 64;
            l.x=ptrs_vector->x1;
            l.y=ptrs_vector->y1;
            l.r=ptrs_vector->r;
            l.kat1 = 0;
            l.kat2 = Pi2;

            line_width_type = Line_Width (l.typ);
            if (Draw_Arc_To_Drive (&l) == FALSE) return FALSE;

            L1.typ=Ln1.typ=32;

            line_width_type = Line_Width (L1.typ);
            if (Draw_Line_To_Drive(&L1) == FALSE) return FALSE;

            Ln1.x1=L1.x2;
            Ln1.y1=L1.y2;
            Ln1.y2=Ln1.y1;

            t_len_mm = Get_TextLen(&Vtxt, Vtxt.text);
            direction=L1.x2-L1.x1;

            if (direction > 0) //to the right
            {
                Vtxt.x = L1.x2 + Vtxt.wysokosc / 4.0;
                Vtxt.y = L1.y2 + Vtxt.wysokosc / 4.0;
                Vtxt.justowanie = j_do_lewej;

                if (t_len_mm == 0.0) Ln1.x2 = L1.x2 + 0.01;
                else Ln1.x2 = L1.x2 + t_len_mm + Vtxt.wysokosc / 2.5;
            } else  //to the left
            {
                Vtxt.x = L1.x2 - Vtxt.wysokosc / 4.0;
                Vtxt.y = L1.y2 + Vtxt.wysokosc / 4.0;
                Vtxt.justowanie = j_do_prawej;
                if (t_len_mm == 0.0) Ln1.x2 = L1.x2 - 0.01;
                else Ln1.x2 = L1.x2 - t_len_mm - Vtxt.wysokosc / 2.0;
            }

            if (Draw_Line_To_Drive(&Ln1) == FALSE) return FALSE;

            K1_5=jednostkiplt(w1_5);
            Kp2sn = 1.686909582 * K1_5;

            n=-1;
            w.xy[2]=Ln.x1;
            w.xy[3]=Ln.y1;
            w.xy[0]=Ln.x1-n*Kp2sn*cos(Pi*(kat1-kat0)/180);
            w.xy[1]=Ln.y1-n*Kp2sn*sin(Pi*(kat1-kat0)/180);
            w.xy[4]=Ln.x1-n*Kp2sn*cos(Pi*(kat1+kat0)/180);
            w.xy[5]=Ln.y1-n*Kp2sn*sin(Pi*(kat1+kat0)/180);
            w.lp = 6;
            w.n = 8 + w.lp * sizeof (float) ;

            if (FALSE == Draw_Wielokat_To_Drive(&w, window_to_print)) return FALSE;

            line_width_type = TEXT_LINE_TYPE;
            if (Draw_Tekst_To_Drive(&Vtxt, 0, 1, 1) == FALSE) return FALSE;

            break;
        default:
            break;
    }

    return TRUE;
}

void kasuj_nr_pola(void)
{
  nr_pola=0;
}

int GetColor_DJGPP__(unsigned char color)
{
  if (color > 255)
  {
    color = 15 ;
  }
  return color;
}

int GetColor_DJGPP_(unsigned char color)
{ int colorDJGPP;
                  //    0   1   2   3   4   5   6   7  8   9 10 11 12 13 14 15 16  17
  static int transf1[]={0,13, 11, 12,  9, 14, 10, 15, 8,  5, 3, 4, 1, 6, 2, 7,0};
  
  if (color < 16)
  {
   colorDJGPP=transf1[color];
   return colorDJGPP;
  } 
  if (color > 255)
  {
    color = 15 ;
  }
  return color;
}  

int fix_color_d(B_PCX *pcx, int kolor, int t_drive)
{
	
	if (pcx->inwersja)
	{
		
		pcx->background = 15;
	}

	if (kolor > 0)
		kolor = kolor;

	
	if (t_drive == 0)
	{
		;
	}
	else
	{
		if ((pcx->ignore_background || pcx->ignore_print_background || pcx->inwersja) && (kolor == pcx->background)) return -1;
	}

	
	if ((pcx->set_foreground) && (kolor != pcx->background))
	{
		if (t_drive == 0)
		{
			if (pcx->foreground == 0) return GetColor_DJGPP_(kolor);
			else return GetColor_DJGPP_/*_*/(pcx->foreground);
		}
		else
		{
			if (pcx->foreground == 0) return GetColor_DJGPP_(15);
			else return GetColor_DJGPP_(pcx->foreground);
		}
	}
	else
	{
		if (t_drive != 0)
		{
			if (kolor == 0) kolor = 15;
		}
		if (t_drive == 0)  return GetColor_DJGPP__(kolor);
		else  return GetColor_DJGPP_(kolor);
	}
}



int fix_color_d__(B_PCX *pcx, int kolor, int t_drive)
{ int a;

if (kolor != 15)
    a = 0;
  if (pcx->inwersja)
   {
     if (kolor==15) kolor=0;
       else if (kolor==0) kolor=15;
    
   }
  else
  {
	  if (kolor == 15) kolor = 15;
	  else kolor = 0;
  }

  return kolor;

 

  if (t_drive==0)
   {
	  ;
   }
    else
     {
      if ((pcx->ignore_background || pcx->ignore_print_background || pcx->inwersja) && (kolor==pcx->background)) return -1;
     }
   
  if ((pcx->set_foreground) && (kolor != pcx->background))
   {
     if (t_drive==0)
       {
         if (pcx->foreground==0) return GetColor_DJGPP_(kolor);
            else return GetColor_DJGPP_/*_*/(pcx->foreground);
       }
        else
         {
          if (pcx->foreground==0) return GetColor_DJGPP_(15);
            else return GetColor_DJGPP_(pcx->foreground);
         }
   }
    else
    {
      if (t_drive!=0) 
	  {
		  if (kolor==0) kolor=15;
	  }
      if (t_drive==0)  return GetColor_DJGPP__(kolor);
        else  return GetColor_DJGPP_(kolor);
    }
}

void get_pcx_solid(double pcx_x0, double pcx_y0, double pcx_x, double pcx_y, double pcx_dx, double pcx_dy, double kos, double koc, BOOL kat0, PCX_SOLID *pcx_s)
{
	if (kat0 == TRUE)
	{
		
		pcx_s->xy[0] = pcx_x;
		pcx_s->xy[1] = pcx_y-pcx_dy;
		pcx_s->xy[2] = pcx_x+pcx_dx;
		pcx_s->xy[3] = pcx_y - pcx_dy;
		pcx_s->xy[4] = pcx_x + pcx_dx;
		pcx_s->xy[5] = pcx_y;
		pcx_s->xy[6] = pcx_x;
		pcx_s->xy[7] = pcx_y;
	}
	else
	{
		pcx_s->xy[0] = pcx_x0 + (pcx_x - pcx_x0) * koc - (pcx_y - pcx_y0 - pcx_dy) * kos;
		pcx_s->xy[1] = pcx_y0 + (pcx_x - pcx_x0) * kos + (pcx_y - pcx_y0 - pcx_dy) * koc;

		pcx_s->xy[2] = pcx_x0 + (pcx_x - pcx_x0 + pcx_dx) * koc - (pcx_y - pcx_y0 - pcx_dy) * kos;
		pcx_s->xy[3] = pcx_y0 + (pcx_x - pcx_x0 + pcx_dx) * kos + (pcx_y - pcx_y0 - pcx_dy) * koc;

		pcx_s->xy[4] = pcx_x0 + (pcx_x - pcx_x0 + pcx_dx) * koc - (pcx_y - pcx_y0) * kos;
		pcx_s->xy[5] = pcx_y0 + (pcx_x - pcx_x0 + pcx_dx) * kos + (pcx_y - pcx_y0) * koc;

		pcx_s->xy[6] = pcx_x0 + (pcx_x - pcx_x0) * koc - (pcx_y - pcx_y0) * kos;
		pcx_s->xy[7] = pcx_y0 + (pcx_x - pcx_x0) * kos + (pcx_y - pcx_y0) * koc;
	}
}

/** find the max and min in array of 'n' doubles `ad`.
 *  update 'max' and 'min' pointers so values are available back
 *  in calling function.
 */
int maxmin_dbl(double *ad, double *max, double *min, size_t n)
{
	if (!ad || !max || !min) return 0;  /* validate parameters */

	*max = (double)LLONG_MIN;   /* initialize max/min to sufficiently */
	*min = (double)LLONG_MAX;   /* large negative/positive values.    */

	size_t i;
	for (i = 0; i < n; i++) {
		if (ad[i] > *max) *max = ad[i];  /* test for new max */
		if (ad[i] < *min) *min = ad[i];  /* test for new min */
	}

	return 1;
}

void get_pcx_solid_mm(PCX_SOLID *pcx_s, PCX_MM *pcx_mm)
{
	double xx[4], yy[4];
	int i, ret;

	for (i = 0; i < 8; i += 2)
	{
		xx[i / 2] = pcx_s->xy[i];
		yy[i / 2] = pcx_s->xy[i + 1];
	}
	

	ret = maxmin_dbl(xx, &pcx_mm->xmax, &pcx_mm->xmin, 4);
	ret = maxmin_dbl(yy, &pcx_mm->ymax, &pcx_mm->ymin, 4);

}

void getimageprn(int left, int top, int right, int bottom, BITMAP* bitmap)
{
	int width, height;
	
	BITMAP* new_bitmap;
	BITMAP* old_bitmap;

	if (top < 0)
		top = 0;

	if (left < 0)
		left = 0;


	width = right - left + 1;
	height = bottom - top + 1;

	old_bitmap = create_sub_bitmap(allegro_prn_bmp, left, top, width, height);
	new_bitmap = bitmap;
	clear_bitmap(new_bitmap);
	blit(old_bitmap, new_bitmap, 0, 0, 0, 0, width, height);
	destroy_bitmap(old_bitmap);

	return;
}

typedef struct
{
	float R;
	float G;
	float B;
	float A;
} RGBAcolorF;

typedef struct
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
} RGBAcolorI;


void blend_32_to_24(BITMAP* destin_bmp, BITMAP* source_bmp, int bx1, int by1, int bx0, int by0)
{
	int i, j;
	int bw, bw1, bh, bh1;
	int pxld, pxls;
	int new_pxld;
	int tr, tg, tb, ta, t1r, t1g, t1b; 
	RGBAcolorF r, fg, bg;
	RGBAcolorI Target; 


	bw = source_bmp->w;
	bh = source_bmp->h;
	bw1 = destin_bmp->w;
	bh1 = destin_bmp->h;
	for (j=0; j<bh; j++)
		for (i = 0; i < bw; i++)
		{
			//taking pixel from ARG
			if ((j + by1) < 0) return;
			pxld=_getpixel24(destin_bmp, i+bx1, j+by1);
			//taking pixel from ARGA
			if ((j + by0) < 0) return;
			pxls= _getpixel32(source_bmp, i+bx0, j+by0);
			//blending
			t1r = (pxld & 0xFF0000) >> 16; 
			t1g = (pxld & 0x00FF00) >> 8;
			t1b = (pxld & 0x0000FF);

			tr = (pxls & 0xFF000000) >> 24;
			tg = (pxls & 0x00FF0000) >> 16;
			tb = (pxls & 0x0000FF00) >> 8;
			ta = (pxls & 0x000000FF);

			bg.A = 1.0;
			bg.R = (float)t1r;
			bg.G = (float)t1g;
			bg.B = (float)t1b;
			
			bg.A = 1.0;
			bg.R = 255.0f;
			bg.G = 255.0f;
			bg.B = 255.0f;
			
			
			fg.A = (float)ta/256.f;
			fg.R = (float)tr+128;
			fg.G = (float)tg;
			fg.B = (float)tb;

		
			if (ta > 0)
			{
				
				r.R = (fg.R * fg.A) + (bg.R * (1.0 - fg.A));
				r.G = (fg.G * fg.A) + (bg.G * (1.0 - fg.A));
				r.B = (fg.B * fg.A) + (bg.B * (1.0 - fg.A));

				if (r.R > 255) 
					r.R = 255;
				if (r.G > 255) 
					r.G = 255;
				if (r.B > 255) 
					r.B = 255;
				
				Target.R = (unsigned char)r.R;
				Target.G= (unsigned char)r.G;
				Target.B= (unsigned char)r.B;

				new_pxld = (Target.R << 16 | Target.G << 8 | Target.B);
				_putpixel24(destin_bmp, i + bx1, j + by1, new_pxld);
			}

		}
		
}

void correct_colors_PCX(BITMAP* png)
{
	int i, j;
	int color, red, green, blue, blue_;
	int new_color; 


	for (j = 0; j < png->h; j++)
	{
		for (i = 0; i < png->w; i++)
		{
			color = _getpixel24(png, i, j);
			red = getr(color);
			green = getg(color);
			blue = getb(color);

			blue_ = blue;
			blue = red;
			red = blue_;

			new_color = makecol(red, green, blue);

			_putpixel24(png, i, j, new_color);
		}
	}
}


void make_grey32(BITMAP* png, T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_, BOOL grey, BOOL bw)
{
	int i, j;
	int color, red, green, blue, alpha; // blue_;
	int gray;
	byte_ int_intensity;
	byte_ intensity;
	float intensity_tab[4] = { 0.2, 1.0, 0.8, 0.6 };
	int depth;

	depth=bitmap_color_depth(png);

	for (j = 0; j < png->h; j++)
	{
		for (i = 0; i < png->w; i++)
		{
			color = _getpixel32(png, i, j);
			red = getr(color);
			green = getg(color);
			blue = getb(color);
			alpha = geta(color);


			if ((grey) || (bw))
			{
				intensity = 0.2989 * red + 0.5870 * green + 0.1140 * blue;

				if (intensity < 252) intensity *= intensity_tab[(*ptrs__prn_ini_date_)->gray_saturation];

				int_intensity = (byte_)intensity;
				gray = makeacol(int_intensity, int_intensity, int_intensity, alpha); 
				
			}
			else if (bw)
			{
				if ((red < 252) && (green < 252) && (blue < 252))
				{
					//int_intensity = 0xFF;
                    int_intensity = 0;
					gray = makeacol(int_intensity, int_intensity, int_intensity, alpha); 
				}
			}

			_putpixel32(png, i, j, gray);
		}
	}
}


void make_grey24(BITMAP* png, T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_, BOOL grey, BOOL bw)
{
	int i, j;
	int color, red, green, blue, alpha; // blue_;
	int gray;
	byte_ int_intensity;
	byte_ intensity;
	float intensity_tab[4] = { 0.2, 1.0, 0.8, 0.6 };

	for (j = 0; j < png->h; j++)
	{
		for (i = 0; i < png->w; i++)
		{
            color = _getpixel32(png, i, j);
			red = getr(color);
			green = getg(color);
			blue = getb(color);
            alpha = geta(color);


			if ((grey) || (bw))
			{
				intensity = 0.2989 * red + 0.5870 * green + 0.1140 * blue;

				if (intensity < 252) intensity *= intensity_tab[(*ptrs__prn_ini_date_)->gray_saturation];

				int_intensity = (byte_)intensity;
				gray = makecol(int_intensity, int_intensity, int_intensity);
				
			}
			else if (bw)
			{
				if ((red < 252) && (green < 252) && (blue < 252))
				{
                    //int_intensity = 0xFF;
					int_intensity = 0;
					gray = makecol(int_intensity, int_intensity, int_intensity);
				}
			}

            _putpixel32(png, i, j, gray);
		}
	}
}

void make_grey8(BITMAP* png, T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_, BOOL grey, BOOL bw)
{
	int i, j;
	int color, red, green, blue; // blue_;
	int gray;
	byte_ int_intensity;
	byte_ intensity;
	float intensity_tab[4] = { 0.2, 1.0, 0.8, 0.6 };

	for (j = 0; j < png->h; j++)
	{
		for (i = 0; i < png->w; i++)
		{
			color = _getpixel(png, i, j);
			red = getr(color);
			green = getg(color);
			blue = getb(color);


			if ((grey) || (bw))
			{
				intensity = 0.2989 * red + 0.5870 * green + 0.1140 * blue;

				if (intensity < 252) intensity *= intensity_tab[(*ptrs__prn_ini_date_)->gray_saturation];

				int_intensity = (byte_)intensity;
				gray = makecol8(int_intensity, int_intensity, int_intensity);

			}
			else if (bw)
			{
				if ((red < 252) && (green < 252) && (blue < 252))
				{
					//int_intensity = 0xFF;
                    int_intensity = 0;
					gray = makecol8(int_intensity, int_intensity, int_intensity);

				}
			}
			_putpixel(png, i, j, gray);
		}
	}
}


BOOL Draw_png_To_Drive(B_PCX *pcx, int t_drive)
{
	
	int i;
	int aa = 0, bb = 0;
	double cc = 0, dd = 0, ee = 0;
	REAL_PCX *rpcx;
	int width, width1, height;
	double pcx_x, pcx_y;
	int byte_x;
	int count = 0;          // current pixel number in the image
	int total;
	BOOL empty_line = TRUE;
	double pcx_dx, pcx_dy, pcx_x0, pcx_y0;
	double kos, koc;
	double kos0, koc0;
	
	double kos1, koc1;
	BOOL bw, grey;

	double full_width, full_height, half_width, half_height;
	BITMAP *png, *png1;
	PALETTE pal;
	char *png_data;
	long png_mem;
	double x0, y0; 
	double pcx_x1, pcx_y1, pcx_x2, pcx_y2;
	double pcx_x01, pcx_y01;
	double scalex;
	int x1, y1;
	char *png_dat;
	double pcx_kat, kat_rot;

	float intensity_tab[4] = { 0.2, 1.0, 0.8, 0.6 };

	T_PTR_Prn_Ini_Date *ptrs__prn_ini_date_;

	double pcx_x_d, pcx_y_d, pcx_x_d1, pcx_y_d1;

	int del_x, del_y;
	int sizew, sizeh;
	double unitx[4], unity[4];
	double dsizew, dsizeh;
	double ddelx, ddely;
	double del_xx, del_yy, del_xxx, del_yyy;

	int delta_x, delta_y;

	BITMAP* buffer;

	ptrs__prn_ini_date_ = get_ptrs__prn_ini_date();
	double condensed_wsp = get_condensed_wsp();

	bw = Layers[pcx->warstwa].bw;
	grey = Layers[pcx->warstwa].grey;

	nr_pola++;

	rpcx = (REAL_PCX*)pcx->pcx;

	width = rpcx->header.xmax - rpcx->header.xmin;
	height = rpcx->header.ymax - rpcx->header.ymin; 

	png_data = pcx->pcx + sizeof(PCXheader);
	png_mem = width * height * 4 + 100;
	
	if (pcx->kod_obiektu==1) png1 = load_memory_pcx(pcx->pcx, &pal);
	  else if (pcx->kod_obiektu == 2) png1 = load_memory_png(png_data, png_mem, (RGB *)&pal);
	    else if (pcx->kod_obiektu == 3) png1 = load_memory_jpg(png_data, png_mem, (RGB*)&pal);
	
	if (png1 == NULL) return 1;

	//if flip

	if (pcx->v_flip == 1)
	{
		png = create_bitmap_ex(32, png1->w * (pcx->dx / pcx->dy), png1->h);
		if (png == NULL) return 1;
		draw_sprite_v_flip(png, png1, 0, 0);
		destroy_bitmap(png1);
		png1 = png;
	}

	if (pcx->h_flip == 1)
	{
		png = create_bitmap_ex(32, png1->w * (pcx->dx / pcx->dy), png1->h);
		if (png == NULL) return 1;
		draw_sprite_h_flip(png, png1, 0, 0);
		destroy_bitmap(png1);
		png1 = png;
	}


	scalex = pcx->dx / pcx->dy;

	if (Check_if_Equal(pcx->dx, pcx->dy) == FALSE)
	{
        png = create_bitmap_ex(32, png1->w*(pcx->dx / pcx->dy), png1->h);

		if (png == NULL) return 1;

		stretch_blit(png1, png, 0, 0, png1->w, png1->h, 0, 0, png->w, png->h);
		destroy_bitmap(png1);
	}
	else png = png1;


	if (width % 2 && width != rpcx->header.bytes_per_line)
	{
		width1 = width + 1;
		total = (width + 1) * height;
	}
	else
	{
		width1 = width;
		total = width * height;
	}

	pcx_dx = pcx->dx;
	pcx_dy = pcx->dy;

	pcx_x0 = pcx->x;
	byte_x = 0;
	pcx_y0 = pcx->y; 

	if (print_inversion == TRUE)
	{
		if (print_rotation == TRUE)
		{
			pcx_x0 = FormatX_S - pcx_x0;
			pcx_y0 = FormatY_S - pcx_y0;
		}
		else
		{
			pcx_x0 = FormatX_S - pcx_x0 + FormatX_S0;
			pcx_y0 = FormatY_S + FormatY_S0 - pcx_y0;
		}
	}
	
	kat_rot = 0.0;

	if (print_rotation == TRUE) kat_rot += PI2;
	if (print_inversion == TRUE) kat_rot += PI2*2;	

	kos0 = Double_Normal(sin(pcx->kat));
	koc0 = Double_Normal(cos(pcx->kat));

	kos1 = Double_Normal(sin(Pi2 - pcx->kat));
	koc1 = Double_Normal(cos(Pi2 - pcx->kat));

	kos = Double_Normal(sin(Pi2 + kat_rot));
	koc = Double_Normal(cos(Pi2 + kat_rot));
	

	pcx_x = pcx_x0;
	pcx_y = pcx_y0;

	double dscale = (double)units_y_to_prn(pcx->dy * 10000) / 10000;

	fixed scale = ftofix(dscale);


	//rotation is around central point, so point origin must be moved

	full_width = (pcx->dx*(double)width);
	full_height = (pcx->dy*(double)height);
	half_width = full_width / 2.0;
	half_height = full_height / 2.0;

	x0 = pcx_x0 + half_width;
	y0 = pcx_y0 + half_height;

	Rotate_Point(kos1, koc1, x0, y0, pcx_x0, pcx_y0, &pcx_x2, &pcx_y2);

	pcx_x_d1 = pcx_x2 - pcx_x0;
	pcx_y_d1 = pcx_y2 - pcx_y0;

	Rotate_Point(kos, koc, x0, y0, pcx_x0, pcx_y0, &pcx_x1, &pcx_y1);

	double left_margin = plt_d_to_jednostki_d((*ptrs__prn_ini_date_)->left_margin) / condensed_wsp;
	//double top_margin = plt_d_to_jednostki_d((*ptrs__prn_ini_date_)->top_margin) / condensed_wsp;
	//double right_margin = plt_d_to_jednostki_d((*ptrs__prn_ini_date_)->right_margin) / condensed_wsp;
	//double bottom_margin = plt_d_to_jednostki_d((*ptrs__prn_ini_date_)->bottom_margin) / condensed_wsp;

	if ((print_rotation == TRUE) && (print_inversion == TRUE))
	{
		pcx_x_d = full_width;
		pcx_y_d = full_width;

		pcx_x01 = pcx_x1 - pcx_x_d + pcx_x_d1;
		pcx_y01 = pcx_y1 - pcx_y_d - pcx_y_d1 + left_margin;
	}
	else
	if (print_rotation == TRUE)
	{
		pcx_x_d = full_height;
		pcx_y_d = full_height;

		pcx_x01 = pcx_x1 - pcx_x_d - pcx_x_d1;
		pcx_y01 = pcx_y1 - pcx_y_d + pcx_y_d1 + left_margin;
	}
	else if (print_inversion == TRUE)
	{

		pcx_x_d = full_width;
		pcx_y_d = full_height;

		pcx_x01 = (pcx_x0 + (pcx_x0 - pcx_x1)) + pcx_x_d1 + left_margin;
		pcx_y01 = pcx_y1 - pcx_y_d1;
	}
	else //straight
	{
		pcx_x_d = full_width; 
		pcx_y_d = full_height;

		pcx_x01 = (pcx_x0 + (pcx_x0 - pcx_x1)) - pcx_x_d1 + left_margin;
	    pcx_y01 = pcx_y1 + pcx_y_d1;
	}
	
	x1 = units_to_prn_x(pcx_x01, pcx_y01);
	y1 = units_to_prn_y(pcx_x01, pcx_y01);

	long length = png->w * png->h * 4;

	png_dat = (char *)png->dat;

	if ((*ptrs__prn_ini_date_)->PCX_gray==TRUE)
	{
		grey = TRUE;
		bw = FALSE;
	}
	else if (((*ptrs__prn_ini_date_)->color_print == 1) && (bw == 0))
	{
		if (((*ptrs__prn_ini_date_)->gray_print == 1) || (grey == 1) || (*ptrs__prn_ini_date_)->PCX_gray == 1)
		{
			grey = TRUE;
			bw = FALSE;
		}
	}
	else
	{
		bw = TRUE;
		grey = FALSE;
	}


	pcx_kat = -pcx->kat + kat_rot;
	if (rpcx->header.bits_per_pixel == 32)  //32
	{
		if ((grey) || (bw))
		{
			make_grey32(png, ptrs__prn_ini_date_, grey, bw);
		}

		unitx[0] = 0.0;
		unity[0] = 0.0;

		dsizew = (int)((float)png->w * dscale);
		dsizeh = (int)((float)png->h * dscale);

		Rotate_Point(kos1, koc1, 0.0, 0.0, 0.0, dsizeh, &unitx[1], &unity[1]);
		Rotate_Point(kos1, koc1, 0.0, 0.0, dsizew, dsizeh, &unitx[2], &unity[2]);
		Rotate_Point(kos1, koc1, 0.0, 0.0, dsizew, 0.0, &unitx[3], &unity[3]);

		double minx = BIGNOF;
		double miny = BIGNOF;
		double maxx = -BIGNOF;
		double maxy = -BIGNOF;

		for (i = 0; i < 4; i++)
		{
			if (unitx[i] < minx) minx = unitx[i];
			if (unitx[i] > maxx) maxx = unitx[i];

			if (unity[i] < miny) miny = unity[i];
			if (unity[i] > maxy) maxy = unity[i];
		}

		minx = x1 + minx;
		maxx = x1 + maxx;
		miny = y1 + miny;
		maxy = y1 + maxy;

		int sizew_0 = maxx - minx;
		int sizeh_0 = maxy - miny;

		ddely = 0.0;
		ddelx = 0.0;

		int sizew_ = maxx - minx;
		int sizeh_ = maxy - miny;

		sizew = sizew_;
		sizeh = sizeh_;

		
		if (print_rotation == TRUE)
		{
			del_xx = ((float)((sizeh_0 - dsizew) / 2.0 + 0.5 - ddelx));
			del_yy = ((float)((sizew_0 - dsizeh) / 2.0 + 0.5 + ddely));
		}
		else
		{
			del_xx = ((float)((sizew_0 - dsizew) / 2.0 + 0.5 - ddelx));
			del_yy = ((float)((sizeh_0 - dsizeh) / 2.0 + 0.5 + ddely));
		}
		
		del_x = (int)del_xx;
		del_y = (int)del_yy;

		if (print_rotation == TRUE) buffer = create_bitmap_ex(32, sizeh, sizew);  //TO CHANGE
		else buffer = create_bitmap_ex(32, sizew, sizeh);  //TO CHANGE

		if (buffer != NULL)
		{
			clear_to_color(buffer, bitmap_mask_color(buffer));
				
			rotate_scaled_sprite(buffer, png, del_x, del_y, ftofix(pcx_kat * 256.0f / (2.0 * PI)), scale);
			
			set_alpha_blender();

			draw_trans_sprite(allegro_prn_bmp, buffer, x1 - del_x, y1 - del_y);

			destroy_bitmap(buffer);
			
			set_trans_blender(0, 0, 0, TRANSLUCENCY);

			set_mode_solid();
		}
		else //ignore transparency
		{
			if ((grey) || (bw))
			{
				make_grey32(png, ptrs__prn_ini_date_, grey, bw);
			}
            if (Check_if_Equal(pcx_kat, 0.0)==TRUE)
            {
                if (png->vtable->color_depth != allegro_prn_bmp->vtable->color_depth)
                {
                    buffer = create_bitmap_ex(allegro_prn_bmp->vtable->color_depth, png->w, png->w);
                    if (buffer != NULL)
                    {
                        blit(png, buffer, 0, 0, 0, 0, png->w, png->h);
                    }
                    stretch_blit(buffer, allegro_prn_bmp, 0, 0, buffer->w, buffer->h, x1, y1, (int) ((buffer->w * dscale) + 0.5),(int) ((buffer->h * dscale) + 0.5));
                    //stretch_sprite(allegro_prn_bmp, buffer, x1, y1, (int) ((buffer->w * dscale) + 0.5),(int) ((buffer->h * dscale) + 0.5));
                    destroy_bitmap(buffer);
                }
                else {
                    stretch_blit(png, allegro_prn_bmp, 0, 0, png->w, png->h, x1, y1, (int) ((png->w * dscale) + 0.5),
                                 (int) ((png->h * dscale) + 0.5));
                    //stretch_sprite(allegro_prn_bmp, png, x1, y1, (int) ((png->w * dscale) + 0.5),(int) ((png->h * dscale) + 0.5));
                }
            }
		else
			rotate_scaled_sprite(allegro_prn_bmp, png, x1, y1, ftofix(pcx_kat * 256.0f / (2.0 * PI)), scale);
		}
	}
	else
	{
		if ((grey) || (bw))
		{
			make_grey24(png, ptrs__prn_ini_date_, grey, bw);
		}
        if (Check_if_Equal(pcx_kat, 0.0)==TRUE)
        {
            if (png->vtable->color_depth != allegro_prn_bmp->vtable->color_depth)
            {
                buffer = create_bitmap_ex(allegro_prn_bmp->vtable->color_depth, png->w, png->h);
                if (buffer != NULL)
                {
                    blit(png, buffer, 0, 0, 0, 0, png->w, png->h);
                }
                stretch_blit(buffer, allegro_prn_bmp, 0, 0, buffer->w, buffer->h, x1, y1, (int) ((buffer->w * dscale) + 0.5),(int) ((buffer->h * dscale) + 0.5));
                //stretch_sprite(allegro_prn_bmp, buffer, x1, y1, (int) ((buffer->w * dscale) + 0.5),(int) ((buffer->h * dscale) + 0.5));
                destroy_bitmap(buffer);
            }
            else {
                stretch_blit(png, allegro_prn_bmp, 0, 0, png->w, png->h, x1, y1, (int) ((png->w * dscale) + 0.5),
                             (int) ((png->h * dscale) + 0.5));
                //stretch_sprite(allegro_prn_bmp, png, x1, y1, (int) ((png->w * dscale) + 0.5),(int) ((png->h * dscale) + 0.5));
            }
        }
		else
            rotate_scaled_sprite(allegro_prn_bmp, png, x1, y1, ftofix(pcx_kat * 256.0f / (2.0 * PI)), scale);
	}
	destroy_bitmap(png);
	
	return 1;
}


BOOL Draw_pcx_8_To_Drive(B_PCX *pcx,int t_drive)
{
 unsigned char key_i;
 int key_ii;
 int  i;
 int aa = 0, bb = 0;
 double cc=0,dd=0,ee=0;
 REAL_PCX *rpcx;
 int width, width1, height;
 double pcx_x,pcx_y,pcx_x0,pcx_y0;
 int byte_x;
 unsigned char key0;
 int count=0;          // current pixel number in the image
 int total;
 char *key_rr;
 int num_bytes;        // number of bytes in a "run"
  BOOL empty_line = TRUE;
  long nr_byte;
  double pcx_dx, pcx_dy;
  double kos, koc;
  PCX_SOLID pcx_s;
  PCX_MM pcx_mm;
  BOOL kat0;
  double kos1, koc1;
  double mmxd[4],mmyd[4];
  double x_[4], y_[4];
  double Xx1,Yy1,Xx2,Yy2;
  double pcx_xxx[4],pcx_yyy[4],pcx_xmin,pcx_ymin,pcx_xmax,pcx_ymax;
  double width11,height11;

  COLOR *pcxpalette;
  COLOR_ pcxcolor;
  BOOL bw, grey;
  
  bw = Layers[pcx->warstwa].bw;
  grey = Layers[pcx->warstwa].grey;
 
  nr_pola++;
  
  rpcx = (REAL_PCX*) pcx->pcx;

  pcxpalette = (COLOR *)(pcx->pcx + pcx->len_pcx - 768);
  key0 = 0;
  width  = rpcx->header.xmax - rpcx->header.xmin + 1;         // image dimensions...
  height = rpcx->header.ymax - rpcx->header.ymin + 1;

  if(rpcx->header.manufacturer   != 10         // check for errors
   ||rpcx->header.version         <  5
   ||rpcx->header.encoding       !=  1
   ||rpcx->header.bits_per_pixel !=  8
   ||rpcx->header.xmin  >=  rpcx->header.xmax
   ||rpcx->header.ymin  >=  rpcx->header.ymax)
  {
    return 0;
  }

  if(width%2 && width != rpcx->header.bytes_per_line)
   {
    width1=width+1;
    total = (width+1) * height;
   }
  else
   {
    width1=width;
    total = width * height;
   }

  pcx_dx=pcx->dx;
  pcx_dy=pcx->dy;

  pcx_x0 = pcx->x;
  byte_x = 0;
  pcx_y0 = pcx->y; // + pcx->dy;

  if (print_inversion == TRUE)
   {
	  if (print_rotation == TRUE)
	  {
		  pcx_y0 = FormatY_S - pcx_y0;// +FormatY_S0;
		  pcx_x0 = FormatX_S - pcx_x0;// +FormatX_S0;
		  pcx_dx *= -1;
		  pcx_dy *= -1;
	  }
	  else
	  {
		  pcx_y0 = FormatY_S - pcx_y0 +FormatY_S0;
		  pcx_x0 = FormatX_S - pcx_x0 +FormatX_S0;
		  pcx_dx *= -1;
		  pcx_dy *= -1;
	  }
   }


   /////////////////////////
  if (Check_if_Equal(pcx->kat,0.0)==FALSE)
   {
    //obrot ekranu o kat pcx->kat wzgledem punktu pcx->x, pcx->y
    kos1=sin(-(pcx->kat));
    koc1=cos(-(pcx->kat));

    x_[0]=X1; y_[0]=Y1;
    x_[1]=X2; y_[1]=Y1;
    x_[2]=X2; y_[2]=Y2;
    x_[3]=X1; y_[3]=Y2;

    for (i=0; i<4; i++)
     {
       mmxd[i] =  pcx_x0 + (x_[i]-pcx_x0) * koc1 - (y_[i]-pcx_y0) * kos1;
       mmyd[i] =  pcx_y0 + (x_[i]-pcx_x0) * kos1 + (y_[i]-pcx_y0) * koc1;
     }

    qsort(mmxd,4,sizeof(double),qsort_by_val);
    qsort(mmyd,4,sizeof(double),qsort_by_val);

    if (print_inversion == TRUE)
     {
      Xx1=mmxd[0];
      Yy1=mmyd[0];
      Xx2=mmxd[3];
      Yy2=mmyd[3];
     }
      else
       {
        Xx1=mmxd[0];
        Yy1=mmyd[0];
        Xx2=mmxd[3];
        Yy2=mmyd[3];
       }

    width11  = (rpcx->header.xmax - rpcx->header.xmin)+1;         // image dimensions...
    height11 = (rpcx->header.ymax - rpcx->header.ymin)+1;

    width11 *= pcx->dx;
    height11 *= pcx->dy;

    pcx_xxx[0] = pcx->x;
    pcx_yyy[0] = pcx->y;

    pcx_xxx[1] = pcx->x - (height11) * kos1 ;
    pcx_yyy[1] = pcx->y - (height11) * koc1 ;

    pcx_xxx[2] = pcx->x + width11 * koc1 - height11 * kos1 ;
    pcx_yyy[2] = pcx->y - width11 * kos1 - height11 * koc1 ;

    pcx_xxx[3] = pcx->x + width11 * koc1;
    pcx_yyy[3] = pcx->y - width11 * kos1;

	x_[0] = pcx->x ;
    y_[0] = pcx->y ;

	x_[1] = pcx->x - height * kos1 ;
    y_[1] = pcx->y - height * koc1 ;

	x_[2] = pcx->x + width * koc1 - height * kos1 ;
    y_[2] = pcx->y - width * kos1 - height * koc1 ;

	x_[3] = pcx->x + width * koc1 ;
    y_[3] = pcx->y - width * kos1 ;

    qsort(pcx_xxx,4,sizeof(double),qsort_by_val);
    qsort(pcx_yyy,4,sizeof(double),qsort_by_val);

    pcx_xmin=pcx_xxx[0];
    pcx_ymin=pcx_yyy[0];
    pcx_xmax=pcx_xxx[3];
    pcx_ymax=pcx_yyy[3];
   }
    else
     {
       Xx1=X1;
       Yy1=Y1;
       Xx2=X2;
       Yy2=Y2;
       pcx_xmin=pcx_x0;
       pcx_xmax=pcx_x0+((rpcx->header.xmax - rpcx->header.xmin)*pcx_dx);
       pcx_ymax=pcx_y0;
       pcx_ymin=pcx_x0-(height*pcx_dy);
     }


  if (pcx_xmax < pcx_xmin)
  {
	  double pcx_xmin_ = pcx_xmin;
	  pcx_xmin = pcx_xmax;
	  pcx_xmax = pcx_xmin_;
  }
  if (pcx_ymax < pcx_ymin)
  {
	  double pcx_ymin_ = pcx_ymin;
	  pcx_ymin = pcx_ymax;
	  pcx_ymax = pcx_ymin_;
  }

  if ((pcx_xmin>X2 || pcx_xmax<X1 || pcx_ymin>Y2 || pcx_ymax<Y1)) return 1;


  if (Check_if_Equal(pcx->kat,0.0)==FALSE)
   {
     kos=sin(pcx->kat);
     koc=cos(pcx->kat);
     kat0=FALSE;
   }
    else
     {
       kos=0.0;
       koc=1.0;
       kat0=TRUE;
     }

  pcx_x = pcx_x0;
  pcx_y = pcx_y0;

   
  nr_byte = 128;

  key_rr = pcx->pcx + sizeof(PCXheader);
  i=-1;

  bb=0;
  
  while(count<total)
  {
    i++;
    key_i=*key_rr;
    nr_byte++;
    
    if(key_i>191)                 // if > 191, it is a run-length code.
    {
      num_bytes = key_i - 192;    // pixels in this "run" can be up to 63
      i++;
      key_rr++;
      key_i=*key_rr;
      nr_byte++;

	  if (type__drive == PLT_DRIVE) key_ii=fix_color_d(pcx,pcx->color_key[key_i],t_drive);
	  
	  if (type__printer == PRN_PCX)
	  {
		  pcxcolor.red = *(byte_*)(pcx->pcx + pcx->len_pcx - 768 + key_i * 3);
		  pcxcolor.gre = *(byte_*)(pcx->pcx + pcx->len_pcx - 768 + key_i * 3 + 1);
		  pcxcolor.blu = *(byte_*)(pcx->pcx + pcx->len_pcx - 768 + key_i * 3 + 2);
	  }
	  else
	  {
		  pcxcolor.red = *(byte_*)(pcx->pcx + pcx->len_pcx - 768 + key_i * 3);
		  pcxcolor.gre = *(byte_*)(pcx->pcx + pcx->len_pcx - 768 + key_i * 3 + 1);
		  pcxcolor.blu = *(byte_*)(pcx->pcx + pcx->len_pcx - 768 + key_i * 3 + 2);
	  }


      //The next line of code shouldn't be needed.  It prevents the image
      //from "running" past the end of the buffer we put it into...
      //if(num_bytes + count > total) num_bytes = total-count;

      while(num_bytes>0)        // ... and place the color several times
       {
       // write the pixel value unless is is a "filler" byte
        if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line)
        {
        //wstawienie punktu w kolorze key

          byte_x+=1;
		  if (byte_x >= width)
		  {
			  get_pcx_solid(pcx_x0, pcx_y0, pcx_x, pcx_y, pcx_dx, pcx_dy, kos, koc, kat0, &pcx_s);
			  get_pcx_solid_mm(&pcx_s, &pcx_mm);
			 
			  
			if ((pcx_mm.xmin <= X2 && pcx_mm.xmax >= X1 && pcx_mm.ymin <= Y2 && pcx_mm.ymax >= Y1))
			{
				if (type__drive == PRN_DRIVE)
				{
					solid04_prn(&pcx_s, pcxcolor, kos, koc, bw, grey);
				}
				else
				{
					if (key_ii > 0)
						DrawSolid04_To_Drive(pcx_s.xy[0], pcx_s.xy[1], pcx_s.xy[0] + pcx_dx, pcx_s.xy[1] - pcx_dy, key_ii, pcx->warstwa);
				}
			}
		    
           
            pcx_x=pcx_x0;
            pcx_y-=pcx_dy;


            if (print_inversion == TRUE)
             {
              if (pcx_y>Yy2) return 1;
             }
              else
               {
                if (pcx_y<Yy1) return 1;
               }
            byte_x=0;
           }
          num_bytes--;
        }
        count++;
		get_pcx_solid(pcx_x0, pcx_y0, pcx_x, pcx_y, pcx_dx, pcx_dy, kos, koc, kat0, &pcx_s);
		get_pcx_solid_mm(&pcx_s, &pcx_mm);

		
		if (byte_x <= width && byte_x > 0)
		{
			
			if ((pcx_mm.xmin <= X2 && pcx_mm.xmax >= X1 && pcx_mm.ymin <= Y2 && pcx_mm.ymax >= Y1))
			   {
				if (type__drive == PRN_DRIVE)
				{
					solid04_prn(&pcx_s, pcxcolor, kos, koc, bw, grey);
				}
				else
				{
					if (key_ii > 0)
					    DrawSolid04_To_Drive(pcx_s.xy[0], pcx_s.xy[1], pcx_s.xy[0] + pcx_dx, pcx_s.xy[1] - pcx_dy, key_ii, pcx->warstwa);
				}
			   }
			pcx_x += pcx_dx;  // 1
		}

		
       }
    }
    else
    {
	if (type__drive == PLT_DRIVE) key_ii=fix_color_d(pcx,pcx->color_key[key_i],t_drive);

	  if (type__printer == PRN_PCX)
	  {

		  pcxcolor.red = *(byte_*)(pcx->pcx + pcx->len_pcx - 768 + key_i * 3);
		  pcxcolor.gre = *(byte_*)(pcx->pcx + pcx->len_pcx - 768 + key_i * 3 + 1);
		  pcxcolor.blu = *(byte_*)(pcx->pcx + pcx->len_pcx - 768 + key_i * 3 + 2);
	  }
	  else
	  {
		  pcxcolor.red = *(byte_*)(pcx->pcx + pcx->len_pcx - 768 + key_i * 3);
		  pcxcolor.gre = *(byte_*)(pcx->pcx + pcx->len_pcx - 768 + key_i * 3 + 1);
		  pcxcolor.blu = *(byte_*)(pcx->pcx + pcx->len_pcx - 768 + key_i * 3 + 2);
	  }

     // write the pixel value unless is is a "filler" byte
      if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line )
      {
		  get_pcx_solid(pcx_x0, pcx_y0, pcx_x, pcx_y, pcx_dx, pcx_dy, kos, koc, kat0, &pcx_s);
		  get_pcx_solid_mm(&pcx_s, &pcx_mm);
		 
		
		if ((pcx_mm.xmin <= X2 && pcx_mm.xmax >= X1 && pcx_mm.ymin <= Y2 && pcx_mm.ymax >= Y1))
		 {
			 if (type__drive == PRN_DRIVE)
			 {
				 solid04_prn(&pcx_s, pcxcolor, kos, koc, bw, grey);
			 }
			 else
			 {
				 if (key_ii > 0)
				     DrawSolid04_To_Drive(pcx_s.xy[0], pcx_s.xy[1], pcx_s.xy[0] + pcx_dx, pcx_s.xy[1] - pcx_dy, key_ii, pcx->warstwa);
			 }
		 }
	  

	  byte_x += 1;
           
 
 
      if (byte_x>=width )
       {
        pcx_x=pcx_x0;
        pcx_y-=pcx_dy;
		
        if (print_inversion == TRUE)
             {
               if (pcx_y>Yy2) return 1;
             }
              else
               {
                if (pcx_y<Yy1) return 1;
               }

        byte_x=0;
       }
      }
      count++;
     if ( my_kbhit() )
       {
	     if (my_getch() == ESC)
	      {
  	       while (my_kbhit ())
	        {
	          my_getch();
	        }
           return 0;
  	      }
       }
	    get_pcx_solid(pcx_x0, pcx_y0, pcx_x, pcx_y, pcx_dx, pcx_dy, kos, koc, kat0, &pcx_s);
		get_pcx_solid_mm(&pcx_s, &pcx_mm);
		
	   if ((byte_x > 0) && (byte_x <= width))
	   {
		 
		 if ((pcx_mm.xmin <= X2 && pcx_mm.xmax >= X1 && pcx_mm.ymin <= Y2 && pcx_mm.ymax >= Y1))
		   {
			   if (type__drive == PRN_DRIVE)
			   {
				   solid04_prn(&pcx_s, pcxcolor, kos, koc, bw, grey);
			   }
			   else
			   {
				   if (key_ii > 0)
				       DrawSolid04_To_Drive(pcx_s.xy[0], pcx_s.xy[1], pcx_s.xy[0] + pcx_dx, pcx_s.xy[1] - pcx_dy, key_ii, pcx->warstwa);
			   }
		   }
		   pcx_x += pcx_dx;
	   }

       if (print_inversion == TRUE)
             {
               if (pcx_y>Yy2) return 1;
             }
              else
               {
                if (pcx_y<Yy1) return 1;
               }
	   
    }
   key_rr++;
  }

return 1;
}


unsigned char tab_bit1[8]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

static int fix_color_d1(B_PCX *pcx, unsigned char kolor0, int iii, int t_drive)
{ int key_ii;
  int kolor_tla0, kolor_tla, kolor_punktu, ignore_bk;
  unsigned char pixels;
  unsigned char kolor;

      kolor=kolor0;
      

      if (t_drive==0)
       {
		if (pcx->ignore_background) ignore_bk=1; else ignore_bk=0;
       }
        else
         {
          if (pcx->ignore_background || pcx->ignore_print_background) ignore_bk=1; else ignore_bk=0;
         }
       
      kolor_tla0=pcx->background;
      kolor_tla=pcx->color_key[1];
      if (pcx->set_foreground==1) kolor_punktu=pcx->foreground; else kolor_punktu=pcx->color_key[0];

      pixels = (kolor & tab_bit1[iii]);

      if (kolor_tla0!=255) //ustawione tlo
       {
        if (!pixels)  //[0]
        {
         if (kolor_punktu==kolor_tla0)  //to jest tlo
          {
           if (ignore_bk==0)
             {
              key_ii=fix_color_d(pcx,kolor_punktu,t_drive);
             }
             else 
			 {
				 if (t_drive==0) key_ii=-3;
					 else key_ii=0;
			 }
          }
          else //to nie jest tlo tylko linia
           {
             key_ii=fix_color_d(pcx,kolor_punktu,t_drive);
           }
        }
         else //[1]
           {
             if (kolor_tla==kolor_tla0)  //to jest tlo
              {
               if (ignore_bk==0)
                {
                  key_ii=fix_color_d(pcx,kolor_tla0,t_drive);
                }
                 else 
				 {
					if (t_drive==0) key_ii=15;
						else key_ii=0;
				 }
					
              }
               else //to nie jest tlo tylko linia
                {
                  key_ii=fix_color_d(pcx,kolor_tla,t_drive);
                }
           }
       }
        else  //nie jest ustawione tlo
         {
          if (pixels==0)           //kolor 0
           {
              key_ii=fix_color_d(pcx,kolor_punktu,t_drive);
           }
           else                     //kolor 1
           {
             key_ii=fix_color_d(pcx,kolor_tla,t_drive);
           }
         }

   return key_ii;
}



BOOL Draw_pcx_1_To_Drive(B_PCX *pcx, int t_drive)
{
 unsigned char key_i;
 int key_ii;
 int i;
 int  aa = 0, bb = 0;
 double cc=0,dd=0,ee=0;
 REAL_PCX *rpcx;
 int width, width1, height;
 double pcx_x,pcx_y,pcx_x0,pcx_y0;
 int byte_x;
 unsigned char key0;
 int count=0;          // current pixel number in the image
 int total;
 char *key_rr;
 int num_bytes;        // number of bytes in a "run"
  BOOL empty_line = TRUE;
  long nr_byte;
  double pcx_dx, pcx_dy, pcx_x1, pcx_x2; // pcx_y1;
  int n8;
  int nn;
  double width_d;
  int width_r;
  int iii;
  double kos,koc,pcx_xx,pcx_yy;
  BOOL kat0;
  double kos1, koc1;
  double mmxd[4],mmyd[4];
  double x_[4], y_[4];
  double Xx1,Yy1,Xx2,Yy2;
  double pcx_xxx[4],pcx_yyy[4],pcx_xmin,pcx_ymin,pcx_xmax,pcx_ymax;
  double width11,height11;
  BOOL bw, grey;

  bw = Layers[pcx->warstwa].bw;
  grey = Layers[pcx->warstwa].grey;

  nr_pola++;

  rpcx = (REAL_PCX*) pcx->pcx;
  key0 = 0;
 
  width_d = ((rpcx->header.xmax - rpcx->header.xmin)+8)/8;         // image dimensions...
  width_r = (rpcx->header.xmax - rpcx->header.xmin)+1;
  width = (int)width_d;
  height = rpcx->header.ymax - rpcx->header.ymin; //+ 1;

  if(rpcx->header.manufacturer   != 10         // check for errors
   ||rpcx->header.version         <  5
   ||rpcx->header.encoding       !=  1
   ||rpcx->header.bits_per_pixel !=  1
   ||rpcx->header.xmin  >=  rpcx->header.xmax
   ||rpcx->header.ymin  >=  rpcx->header.ymax)
  {
    return 0;
  }

  if(width%2 && width != rpcx->header.bytes_per_line)
   {
    width1=width+1;
    total = (width+1) * height;
   }
  else
   {
    width1=width;
    total = width * height;
   }

  pcx_x0 = pcx->x; // - pcx_dx;
  byte_x = 0;
  pcx_y0 = pcx->y; // + pcx_dy;

  pcx_dx=pcx->dx;
  pcx_dy=pcx->dy;

  if (print_inversion == TRUE)
  {
	  if (print_rotation == TRUE)
	  {
		  pcx_y0 = FormatY_S - pcx_y0;// +FormatY_S0;
		  pcx_x0 = FormatX_S - pcx_x0;// +FormatX_S0;
		  pcx_dx *= -1;
		  pcx_dy *= -1;
	  }
	  else
	  {
		  pcx_y0 = FormatY_S - pcx_y0 + FormatY_S0;
		  pcx_x0 = FormatX_S - pcx_x0 + FormatX_S0;
		  pcx_dx *= -1;
		  pcx_dy *= -1;
	  }
  }


  if (Check_if_Equal(pcx->kat,0.0)==FALSE)
   {
    //obrot ekranu o kat pcx->kat wzgledem punktu pcx->x, pcx->y
    kos1=sin(-(pcx->kat));
    koc1=cos(-(pcx->kat));

    x_[0]=X1; y_[0]=Y1;
    x_[1]=X2; y_[1]=Y1;
    x_[2]=X2; y_[2]=Y2;
    x_[3]=X1; y_[3]=Y2;

    for (i=0; i<4; i++)
     {
       mmxd[i] =  pcx_x0 + (x_[i]-pcx_x0) * koc1 - (y_[i]-pcx_y0) * kos1;
       mmyd[i] =  pcx_y0 + (x_[i]-pcx_x0) * kos1 + (y_[i]-pcx_y0) * koc1;
     }

    qsort(mmxd,4,sizeof(double),qsort_by_val);
    qsort(mmyd,4,sizeof(double),qsort_by_val);

    if (print_inversion == TRUE)
     {
      Xx1=mmxd[0];
      Yy1=mmyd[0];
      Xx2=mmxd[3];
      Yy2=mmyd[3];
     }
      else
       {
        Xx1=mmxd[0];
        Yy1=mmyd[0];
        Xx2=mmxd[3];
        Yy2=mmyd[3];
       }

    width11  = (((rpcx->header.xmax - rpcx->header.xmin)+8)/8)*8;         // image dimensions...
    height11 = (rpcx->header.ymax - rpcx->header.ymin)+1;

    width11 *= pcx->dx;
    height11 *= pcx->dy;

    pcx_xxx[0] = pcx->x;
    pcx_yyy[0] = pcx->y;

    pcx_xxx[1] = pcx->x - (height11) * kos1 ;
    pcx_yyy[1] = pcx->y - (height11) * koc1 ;

    pcx_xxx[2] = pcx->x + width11 * koc1 - height11 * kos1 ;
    pcx_yyy[2] = pcx->y - width11 * kos1 - height11 * koc1 ;

    pcx_xxx[3] = pcx->x + width11 * koc1;
    pcx_yyy[3] = pcx->y - width11 * kos1;

	x_[0] = pcx->x ;
    y_[0] = pcx->y ;

	x_[1] = pcx->x - height * kos1 ;
    y_[1] = pcx->y - height * koc1 ;

	x_[2] = pcx->x + width * koc1 - height * kos1 ;
    y_[2] = pcx->y - width * kos1 - height * koc1 ;

	x_[3] = pcx->x + width * koc1 ;
    y_[3] = pcx->y - width * kos1 ;

    qsort(pcx_xxx,4,sizeof(double),qsort_by_val);
    qsort(pcx_yyy,4,sizeof(double),qsort_by_val);

    pcx_xmin=pcx_xxx[0];
    pcx_ymin=pcx_yyy[0];
    pcx_xmax=pcx_xxx[3];
    pcx_ymax=pcx_yyy[3];
   }
    else
     {
       Xx1=X1;
       Yy1=Y1;
       Xx2=X2;
       Yy2=Y2;
       pcx_xmin=pcx_x0;
       pcx_xmax=pcx_x0+((rpcx->header.xmax - rpcx->header.xmin)*pcx_dx);
       pcx_ymax=pcx_y0;
       pcx_ymin=pcx_y0-(height*pcx_dy);
     }

  if (pcx_xmax < pcx_xmin)
  {
	  double pcx_xmin_ = pcx_xmin;
	  pcx_xmin = pcx_xmax;
	  pcx_xmax = pcx_xmin_;
  }
  if (pcx_ymax < pcx_ymin)
  {
	  double pcx_ymin_ = pcx_ymin;
	  pcx_ymin = pcx_ymax;
	  pcx_ymax = pcx_ymin_;
  }

if ((pcx_xmin>X2 || pcx_xmax<X1 || pcx_ymin>Y2 || pcx_ymax<Y1)) return 1;  //!!!!!!!  ????????????
                


  if (Check_if_Equal(pcx->kat,0.0)==FALSE)
    {
     kos=sin(pcx->kat);
     koc=cos(pcx->kat);
     kat0=FALSE;
    }
     else
      {
       kos=0.0;
       koc=1.0;
       kat0=TRUE;
      }

  pcx_x = pcx_x0;
  pcx_y = pcx_y0;

  nr_byte = 128;

  key_rr =  pcx->pcx + sizeof(PCXheader);
  i=-1;

  bb=0;
  
  while(count<total)
  {
    i++;
    key_i=*key_rr;
    nr_byte++;
    
    if(key_i>191)                 // if > 191, it is a run-length code.
    {
      num_bytes = key_i - 192;    // pixels in this "run" can be up to 63
      i++;
      key_rr++;
      key_i=*key_rr;
      nr_byte++;

      //The next line of code shouldn't be needed.  It prevents the image
      //from "running" past the end of the buffer we put it into...
      //if(num_bytes + count > total) num_bytes = total-count;

      while(num_bytes>0)        // ... and place the color several times
       {
       // write the pixel value unless is is a "filler" byte
        if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line)
        {
        //wstawienie punktu w kolorze key

          byte_x+=1;
          if (byte_x>=width)
           {
             nn=((byte_x-1)*8.0)-width_r;
             if (nn<8)
              {
               n8=nn;
              }
               else n8=8;

             for (iii=0; iii<n8; iii++)
             {
              
             pcx_x1=pcx_x+(iii*pcx_dx);
             pcx_x2=pcx_x+((iii+1)*pcx_dx);

             if (kat0==TRUE)
               {
                pcx_xx = pcx_x1;
                pcx_yy = pcx_y;
               }
                else
                  {
                   pcx_xx = pcx_x0 + (pcx_x1 - pcx_x0) * koc - (pcx_y - pcx_y0) * kos ;
                   pcx_yy = pcx_y0 + (pcx_x1 - pcx_x0) * kos + (pcx_y - pcx_y0) * koc ;
                  }
            
              if ((pcx_xx<=X2 && pcx_xx>=X1 && pcx_yy>=Y1 && pcx_yy<=Y2))
               {
                 key_ii=fix_color_d1(pcx,key_i,iii,t_drive);
                 
				 if (type__drive == PRN_DRIVE)
				 {
					 solid04_prn_old(pcx_xx, pcx_yy, pcx_xx + pcx_dx, pcx_yy - pcx_dy, key_ii, kos, koc, bw, grey);
					 //solid04N_prn(pcx_xx, pcx_yy, pcx_dx, pcx_dy, pcxcolor, pcx_x0, pcx_y0, kos, koc, bw, grey);
				 }
				 else
				 {
					 if (key_ii > 0)
					     DrawSolid04_To_Drive(pcx_xx, pcx_yy, pcx_xx + pcx_dx, pcx_yy - pcx_dy, key_ii, pcx->warstwa);
				 }
                
               }
          
             }

            pcx_x=pcx_x0;
            pcx_y-=pcx_dy;

            if (print_inversion == TRUE)
             {
               if (pcx_y>Yy2) return 1;
             }
              else
               {
                  if (pcx_y<Yy1) return 1;
               }
            byte_x=0;
           }
          num_bytes--;
        }
        count++;
             if (byte_x>0)
              {
               for (iii=0; iii<8; iii++)
                {
               
                pcx_x1=pcx_x+(iii*pcx_dx);
                pcx_x2=pcx_x+((iii+1)*pcx_dx);
                if (kat0==TRUE)
				   {
					pcx_xx = pcx_x1;
					pcx_yy = pcx_y;
				   }
                  else
                  {
                   pcx_xx = pcx_x0 + (pcx_x1 - pcx_x0) * koc - (pcx_y - pcx_y0) * kos ;
                   pcx_yy = pcx_y0 + (pcx_x1 - pcx_x0) * kos + (pcx_y - pcx_y0) * koc ;
                  }
             
                if (pcx_xx<=X2 && pcx_xx>=X1 && pcx_yy>=Y1 && pcx_yy<=Y2)
                 {

					 key_ii=fix_color_d1(pcx,key_i,iii,t_drive);
                 
					 if (type__drive == PRN_DRIVE)
					 {
						 solid04_prn_old(pcx_xx, pcx_yy, pcx_xx + pcx_dx, pcx_yy - pcx_dy, key_ii, kos, koc, bw, grey);
						 //solid04N_prn(pcx_xx, pcx_yy, pcx_dx, pcx_dy, pcxcolor, pcx_x0, pcx_y0, kos, koc, bw, grey);
					 }
					 else
					 {
						 if (key_ii > 0)
						     DrawSolid04_To_Drive(pcx_xx, pcx_yy, pcx_xx + pcx_dx, pcx_yy - pcx_dy, key_ii, pcx->warstwa);
					 }
					 
                 }
                }
             
                pcx_x+=(pcx_dx*8.0);
              }

         if (kat0==TRUE)
           {
            pcx_xx = pcx_x;
            pcx_yy = pcx_y;
           }
            else
              {
               pcx_xx = pcx_x0 + (pcx_x - pcx_x0) * koc - (pcx_y - pcx_y0) * kos ;
               pcx_yy = pcx_y0 + (pcx_x - pcx_x0) * kos + (pcx_y - pcx_y0) * koc ;
              }

        if (print_inversion == TRUE)
          {
            if (pcx_y>Yy2) return 1;
          }
           else
            {
              if (pcx_y<Yy1) return 1;
            }
       }
    }
    else
    {
     
     // write the pixel value unless is is a "filler" byte

      if(width == rpcx->header.bytes_per_line || count % rpcx->header.bytes_per_line )
      {
      
      byte_x+=1;
      if (byte_x>=width )
       {

       
        nn=((byte_x-1)*8.0)-width_r;
             if (nn<8)
              {
               n8=nn;
              }
               else n8=8;

             for (iii=0; iii<n8; iii++)
             {
            
             pcx_x1=pcx_x+(iii*pcx_dx);
             pcx_x2=pcx_x+((iii+1)*pcx_dx);

             if (kat0==TRUE)
               {
                pcx_xx = pcx_x1;
                pcx_yy = pcx_y;
               }
                else
                  {
                   pcx_xx = pcx_x0 + (pcx_x1 - pcx_x0) * koc - (pcx_y - pcx_y0) * kos ;
                   pcx_yy = pcx_y0 + (pcx_x1 - pcx_x0) * kos + (pcx_y - pcx_y0) * koc ;
                  }
            
			 if ((pcx_xx <= X2 && pcx_xx >= X1 && pcx_yy >= Y1 && pcx_yy <= Y2))
			 {
				 key_ii = fix_color_d1(pcx, key_i, iii, t_drive);

				 if (type__drive == PRN_DRIVE)
				 {
					 solid04_prn_old(pcx_xx, pcx_yy, pcx_xx + pcx_dx, pcx_yy - pcx_dy, key_ii, kos, koc, bw, grey);
					 //solid04N_prn(pcx_xx, pcx_yy, pcx_dx, pcx_dy, key_ii, pcx_x0, pcx_y0, kos, koc, bw, grey);
				 }
				 else
				 {
					 if (key_ii > 0)
					     DrawSolid04_To_Drive(pcx_xx, pcx_yy, pcx_xx + pcx_dx, pcx_yy - pcx_dy, key_ii, pcx->warstwa);
				 }
                 
               }
         
             }

        pcx_x=pcx_x0;
        pcx_y-=pcx_dy;

        if (kat0==TRUE)
         {
           pcx_xx = pcx_x;
           pcx_yy = pcx_y;
         }
          else
           {
            pcx_xx = pcx_x0 + (pcx_x - pcx_x0) * koc - (pcx_y - pcx_y0) * kos ;
            pcx_yy = pcx_y0 + (pcx_x - pcx_x0) * kos + (pcx_y - pcx_y0) * koc ;
           }

        if (print_inversion == TRUE)
             {
                if (pcx_y>Yy2) return 1;
             }
              else
               {
                  if (pcx_y<Yy1) return 1;
               }
        byte_x=0;
       }
      }
      count++;

        if (byte_x>0)
         {
          for (iii=0; iii<8; iii++)
          {
          
             pcx_x1=pcx_x+(iii*pcx_dx);
             pcx_x2=pcx_x+((iii+1)*pcx_dx);
             if (kat0==TRUE)
               {
                pcx_xx = pcx_x1;
                pcx_yy = pcx_y;
               }
                else
                  {
                   pcx_xx = pcx_x0 + (pcx_x1 - pcx_x0) * koc - (pcx_y - pcx_y0) * kos ;
                   pcx_yy = pcx_y0 + (pcx_x1 - pcx_x0) * kos + (pcx_y - pcx_y0) * koc ;
                  }
             
          if (pcx_xx<=X2 && pcx_xx>=X1 && pcx_yy>=Y1 && pcx_yy<=Y2)
           {
             key_ii=fix_color_d1(pcx,key_i,iii,t_drive);

			 if (type__drive == PRN_DRIVE)
			 {
				 solid04_prn_old(pcx_xx, pcx_yy, pcx_xx + pcx_dx, pcx_yy - pcx_dy, key_ii, kos, koc, bw, grey);
				 //solid04N_prn(pcx_xx, pcx_yy, pcx_dx, pcx_dy, pcxcolor, pcx_x0, pcx_y0, kos, koc, bw, grey);
			 }
			 else
			 {
				 if (key_ii > 0)
				     DrawSolid04_To_Drive(pcx_xx, pcx_yy, pcx_xx + pcx_dx, pcx_yy - pcx_dy, key_ii, pcx->warstwa);
			 }
           }
          }
          pcx_x+=(pcx_dx*8.0);
         }

       if ( my_kbhit() )
       {
	     if (my_getch() == ESC)
	      {
  	       while (my_kbhit ())
	        {
	          my_getch();
	        }
           return 0;
  	      }
       }

       if (kat0==TRUE)
         {
           pcx_xx = pcx_x;
           pcx_yy = pcx_y;
          }
           else
             {
               pcx_xx = pcx_x0 + (pcx_x - pcx_x0) * koc - (pcx_y - pcx_y0) * kos ;
               pcx_yy = pcx_y0 + (pcx_x - pcx_x0) * kos + (pcx_y - pcx_y0) * koc ;
             }

       if (print_inversion == TRUE)
             {
                if (pcx_y>Yy2) return 1;
             }
              else
               {
                  if (pcx_y<Yy1) return 1;
               }
    }
   key_rr++;
  }

return 1;
}


BOOL Draw_Pcx_To_Drive(B_PCX *adr_pcx,int t_drive)
{ REAL_PCX *rpcx;
BOOL ret=TRUE;

 PCX_printing=TRUE;
 if (adr_pcx->kod_obiektu == 1) //PCX
 {
	 rpcx = (REAL_PCX*)adr_pcx->pcx;
	 //if ((Layers[adr_pcx->warstwa].grey==1) || (Layers[adr_pcx->warstwa].bw==1)) PCX_gray=TRUE;

	 if (rpcx->header.bits_per_pixel == 8)
	 {
		 if (rpcx->header.num_color_planes==1)
		     ret = Draw_pcx_8_To_Drive(adr_pcx, t_drive);
		 else ret = Draw_png_To_Drive(adr_pcx, t_drive);
	 }
	 else ret = Draw_pcx_1_To_Drive(adr_pcx, t_drive);
	 //PCX_gray=FALSE;
 }
 else if ((adr_pcx->kod_obiektu == 2) || (adr_pcx->kod_obiektu == 3)) //PNG, JPG
 {
	 ret = Draw_png_To_Drive(adr_pcx, t_drive);
 }
 PCX_printing=FALSE;
 return ret;
}

static BOOL draw_border_solidarc_to_drive (SOLIDARC *ptr_sa, int type__drive)
/*-------------------------------------------------------------------------*/
{
    int kolor;
    LINIA L_left, L_right;
    LUK l_inner1, l_inner2, l_outer1, l_outer2;
    int ln;
    int typ;

    NAGLOWEK *nag1;
    WIELOKAT  *w1;
    SOLIDARC *sa1;

    BOOL begin_line=1, end_line=1;

    if ((ptr_sa->empty_typ>0) && (ptr_sa->empty_typ<6) && (trace_block==TRUE) && (((char*)ptr_sa+ptr_sa->n+sizeof(NAGLOWEK)-1)<=trace_block_end))  //is outlined and can be the last one
    {
        double xy[8];
        get_solidarc_ends(ptr_sa, xy);

        if (((char*)ptr_sa+ptr_sa->n+sizeof(NAGLOWEK))>trace_block_begin)  //this is not first element in polyline
        {
            //checking the last coordinates
            if ((Check_if_Equal(xy[6],last_trace_point[0].x)) &&
                (Check_if_Equal(xy[7],last_trace_point[0].y)) &&
                (Check_if_Equal(xy[4],last_trace_point[1].x)) &&
                (Check_if_Equal(xy[5],last_trace_point[1].y)))
            {
                if (!ptr_sa->reversed) end_line=0;
                else begin_line=0;
            }
        }

        nag1=(NAGLOWEK*)((char*)ptr_sa+sizeof(NAGLOWEK)+ptr_sa->n);

        if (nag1->obiekt==Owwielokat)
        {
            w1=(WIELOKAT*)nag1;
            if ((Check_if_Equal(w1->xy[2], xy[2])) &&
                (Check_if_Equal(w1->xy[3], xy[3])) &&
                (Check_if_Equal(w1->xy[0], xy[0])) &&
                (Check_if_Equal(w1->xy[1], xy[1])))
                if (!ptr_sa->reversed) begin_line=0;
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
                if (!ptr_sa->reversed) begin_line=0;
                else end_line=0;
        }

        last_trace_point[0].x=xy[0];
        last_trace_point[0].y=xy[1];
        last_trace_point[1].x=xy[2];
        last_trace_point[1].y=xy[3];
    }


    kolor = MakeColor(GetColorWin(ptr_sa->kolor));

    ln=solidarc_elements(ptr_sa, &L_left, &L_right, &l_inner1, &l_inner2,&l_outer1, &l_outer2);

    //if (type__drive!=PRN_DRIVE) typ=0;
    //else typ=solidfill_typ[ptr_sa->empty_typ];
    typ=0;

    if (begin_line) {
        if (linen_plt1 /*linen_plt*/ (L_left.x1, L_left.y1, L_left.x2, L_left.y2, 0, kolor, 0, Layers[ptr_sa->warstwa].bw, Layers[ptr_sa->warstwa].grey) == FALSE)return FALSE;
    }
    if ( draw_arc_drv( l_inner1.x, l_inner1.y, (double)(l_inner1.kat1), (double)(l_inner1.kat2), l_inner1.r, typ, kolor, Layers[ptr_sa->warstwa].bw, Layers[ptr_sa->warstwa].grey) == FALSE) return FALSE;
    if (ln>1)
        {
            if ( draw_arc_drv( l_inner2.x, l_inner2.y, (double)(l_inner2.kat1), (double)(l_inner2.kat2), l_inner2.r, typ, kolor, Layers[ptr_sa->warstwa].bw, Layers[ptr_sa->warstwa].grey) == FALSE) return FALSE;
        }
    if (end_line) {
    if ( linen_plt1 /*linen_plt*/ ( L_right.x1, L_right.y1, L_right.x2, L_right.y2, 0, kolor, 0, Layers[ptr_sa->warstwa].bw, Layers[ptr_sa->warstwa].grey) == FALSE ) return FALSE;
    }
    if ( draw_arc_drv( l_outer1.x, l_outer1.y, (double)(l_outer1.kat1), (double)(l_outer1.kat2), l_outer1.r, typ, kolor, Layers[ptr_sa->warstwa].bw, Layers[ptr_sa->warstwa].grey) == FALSE) return FALSE;
    if (ln>1)
        {
            if ( draw_arc_drv( l_outer2.x, l_outer2.y, (double)(l_outer2.kat1), (double)(l_outer2.kat2), l_outer2.r, typ, kolor, Layers[ptr_sa->warstwa].bw, Layers[ptr_sa->warstwa].grey) == FALSE) return FALSE;
        }

    return TRUE;

}

static BOOL draw_border_solid_to_drive (WIELOKAT *ptr_w)
/*----------------------------------------------------*/
{
  int i;
  double s1, s2, p1, p2 ;
  int kolor;
  NAGLOWEK *nag1;
  WIELOKAT  *w1;
  SOLIDARC *sa1;
  BOOL skip;

  kolor = MakeColor(GetColorWin(ptr_w->kolor));

  s1 = ptr_w->xy [ptr_w->lp - 2] ; /*punk S jest ostatnim punktem*/
  s2 = ptr_w->xy [ptr_w->lp - 1] ;
  for (i = 0 ; i < (int)ptr_w->lp ; i += 2) 	/*dla kazdego wierzcholka*/
  {
      skip=FALSE;
      if ((ptr_w->lp>6) && (trace_block==TRUE) && (((char*)ptr_w+ptr_w->n+sizeof(NAGLOWEK)-1)<=trace_block_end))
      {
          if (i == 6)  //endpoint
          {
              nag1=(NAGLOWEK*)((char*)ptr_w+sizeof(NAGLOWEK)+ptr_w->n);
              if (nag1->obiekt==Owwielokat)
              {
                  w1=(WIELOKAT*)nag1;
                  if ((Check_if_Equal(ptr_w->xy[4], w1->xy[2])) &&
                      (Check_if_Equal(ptr_w->xy[5], w1->xy[3])) &&
                      (Check_if_Equal(ptr_w->xy[6], w1->xy[0])) &&
                      (Check_if_Equal(ptr_w->xy[7], w1->xy[1])))
                      skip=TRUE;
              }
              else if (nag1->obiekt==Osolidarc)
              {
                  double xy[8];
                  sa1=(SOLIDARC*)nag1;
                  get_solidarc_ends(sa1, xy);

                  if ((Check_if_Equal(ptr_w->xy[4], xy[4])) &&
                      (Check_if_Equal(ptr_w->xy[5], xy[5])) &&
                      (Check_if_Equal(ptr_w->xy[6], xy[6])) &&
                      (Check_if_Equal(ptr_w->xy[7], xy[7])))
                      skip=TRUE;
              }
              //it has to be also checked if beginning is not identical with the end of the last element in pline block
          }
          else if (i == 2)   //startpoint
          {
              if (((char*)ptr_w>trace_block_begin) &&
                  (Check_if_Equal(ptr_w->xy[0],last_trace_point[0].x)) &&
                  (Check_if_Equal(ptr_w->xy[1],last_trace_point[0].y)) &&
                  (Check_if_Equal(ptr_w->xy[2],last_trace_point[1].x)) &&
                  (Check_if_Equal(ptr_w->xy[3],last_trace_point[1].y)))
                  skip=TRUE;
              //it has to be also checked if beginning is not identical with the end of the last element in pline block
          }
      }


    p1 = ptr_w->xy [i] ;	    	/*punkt P jest nastepny*/
    p2 = ptr_w->xy [i + 1] ;
    if (!skip) {
    if ( linen_plt1 /*linen_plt*/ ( s1, s2, p1, p2, 0, kolor, 0, Layers[ptr_w->warstwa].bw, Layers[ptr_w->warstwa].grey) == FALSE ) return FALSE;
    }
    s1 = p1 ;
    s2 = p2 ;
  }
    last_trace_point[0].x=ptr_w->xy[6];
    last_trace_point[0].y=ptr_w->xy[7];
    last_trace_point[1].x=ptr_w->xy[4];
    last_trace_point[1].y=ptr_w->xy[5];
  return TRUE;
}

static BOOL draw_border_solid_to_drive_null (WIELOKAT *ptr_w, int opcja)
/*--------------------------------------------------------------------*/
{
  int i;
  double s1, s2, p1, p2 ;

  s1 = ptr_w->xy [ptr_w->lp - 2] ; /*punk S jest ostatnim punktem*/
  s2 = ptr_w->xy [ptr_w->lp - 1] ;
  for (i = 0 ; i < (int)ptr_w->lp ; i += 2) 	/*dla kazdego wierzcholka*/
  {
    p1 = ptr_w->xy [i] ;	    	/*punkt P jest nastepny*/
    p2 = ptr_w->xy [i + 1] ;
    if ( linen_plt1 /*linen_plt*/ ( s1, s2, p1, p2, 0, opcja, 0, Layers[ptr_w->warstwa].bw, Layers[ptr_w->warstwa].grey) == FALSE ) return FALSE;
    s1 = p1 ;
    s2 = p2 ;
  }
  return TRUE;
}


#ifdef adadf
static BOOL draw_border_solid_to_drive (WIELOKAT *w)
/*----------------------------------------------*/
{
  int i;
  double x1, y1, x2, y2;
  int kolor;

  kolor=w->kolor;
  if (Layers[w->warstwa].bw==1) kolor=COLOR_BLACK;
  else if (Layers[w->warstwa].flags==1) kolor=MakeGrayColor(GetColorWin(w->kolor));
  else kolor = MakeColor(GetColorWin(w->kolor));

  x1 = w->xy[0] ;
  y1 = w->xy[1] ;

  for(i = 2;    i < w->lp  &&  i < MaxNumPoints;    i += 2)
   {
     x2 = w->xy[i] ;
     y2 = w->xy[i + 1] ;
     if ( linen_plt ( x1, y1, x2, y2, 0, kolor, 0) == FALSE ) return FALSE;
     x1 = x2;
     y1 = y2;
   }
  if ( linen_plt ( x1, y1, w->xy[0], w->xy[1], 0, kolor, 0) == FALSE ) return FALSE;
  return TRUE;
}
#endif


BOOL Draw_Arc_To_Drive (LUK *l)
/*--------------------------*/
{
  int typ;
  int ret;
  int kolor;

  if (l->typ == HATCH_OUTLINE_TYPE) return TRUE;  //invisible

  typ = ( l->typ & 31 );
 
  kolor = MakeColor(GetColorWin(l->kolor));
  
  ret=draw_arc_drv( l->x, l->y, (double)(l->kat1), (double)(l->kat2), l->r, typ, kolor, Layers[l->warstwa].bw, Layers[l->warstwa].grey) ;
  return ret;
}

BOOL Draw_Ellipse_To_Drive(ELLIPSE *e)
/*----------------------------------*/
{
    int typ;
    int ret;
    int kolor;

    //if (e->typ == HATCH_OUTLINE_TYPE) return TRUE;  //invisible

    typ = ( e->typ & 31 );
    kolor = MakeColor(GetColorWin(e->kolor));

    if (e->obiekt==Oellipse) {
        typ = (e->typ & 31);
        ret = draw_ellipse_drv(e->x, e->y, e->rx, e->ry, e->angle, typ, kolor, Layers[e->warstwa].bw, Layers[e->warstwa].grey);
    }
    else if (e->obiekt==Ofilledellipse)
        ret = draw_filledellipse_drv(e->x, e->y, e->rx, e->ry, e->angle, kolor, e->translucency, Layers[e->warstwa].bw, Layers[e->warstwa].grey);

    return ret;
}

BOOL Draw_EllipticalArc_To_Drive(ELLIPTICALARC *ea)
/*----------------------------------------------*/
{
    int typ;
    int ret;
    int kolor;

    //if (ea->typ == HATCH_OUTLINE_TYPE) return TRUE;  //invisible

    typ = ( ea->typ & 31 );
    kolor = MakeColor(GetColorWin(ea->kolor));

    ret = draw_ellipticalarc_drv(ea->x, ea->y, ea->rx, ea->ry, ea->angle, ea->kat1, ea->kat2, typ, kolor, Layers[ea->warstwa].bw, Layers[ea->warstwa].grey);

    return ret;
}


BOOL Draw_Line_To_Drive(LINIA *L)
/*---------------------------------*/
{
	int typ;
	PLINIA PL;
	int kolor;
	BOOL do_count, do_end;
	BLOK *ptrs_block;

	if (L->typ == HATCH_OUTLINE_TYPE) return TRUE;  //invisible

	parametry_lini(L, &PL);

	kolor = MakeColor(GetColorWin(L->kolor));

	typ = (L->typ & 31);

	if (typ == 0)
	{
		if (draw_line_to_drive1(L->x1, L->y1, L->x2, L->y2, kolor, 0, Layers[L->warstwa].bw, Layers[L->warstwa].grey) == FALSE) return FALSE;
		return TRUE;
	}

	do_count = FALSE;
	do_end = TRUE;

	if (L->blok == ElemBlok &&
		NULL != (ptrs_block = Parent_Block((char  *)L)) &&
		ptrs_block->kod_obiektu == 'P')
	{
		do_count = TRUE;
		do_end = FALSE;
	}
	
	if (draw_wzorzec(L->x1, L->y1, PL.dl, PL.sin, PL.cos, typ, kolor, do_count, do_end, Layers[L->warstwa].bw, Layers[L->warstwa].grey) == FALSE) return FALSE;
    
  return TRUE;
}

BOOL Draw_Point_To_Drive(T_Point *ptrs_point)
/*-----------------------------------------*/
{
    double df_psize ;
    double x1, y1, x2, y2, x12, y12;
    int kolor;
    double dxy, dd, dd1;
    OKRAG Kolo;
    LINIA L;
    LUK l;
    int ret;

    if (ptrs_point->typ<8) df_psize = Get_Point_Size () / 2;
    else df_psize = Get_Point_Size () / 4 ;

    x1 = (ptrs_point->x - df_psize) ;
    y1 = (ptrs_point->y - df_psize) ;
    x2 = (ptrs_point->x + df_psize) ;
    y2 = (ptrs_point->y + df_psize) ;


    x12 = ptrs_point->x;
    y12 = ptrs_point->y;

    line_width_type = LINE_NORM_WIDTH;
    kolor = MakeColor(GetColorWin(ptrs_point->kolor));

    l.warstwa=Kolo.warstwa=ptrs_point->warstwa;
    l.kolor=Kolo.kolor=ptrs_point->kolor;
    l.typ = Kolo.typ = 64;
    l.r = df_psize*2;
    l.kat1 = 0;
    l.kat2 = Pi2;
    Kolo.x=l.x=ptrs_point->x;
    Kolo.y=l.y=ptrs_point->y;
    Kolo.r = df_psize*1.5;  //*1.0


    switch (ptrs_point->typ)
    {
        case 7: /*junction*/
            line_width_type = LINE_MIN_WIDTH;
            Kolo.r = 2*df_psize/5.0;
            if (Draw_Fill_Circle_To_Drive(&Kolo) == FALSE) return FALSE;
        break;
        case 8: /*pin*/
            if (PIN_IS_PRINTABLE)
            {
                line_width_type = LINE_NORM_WIDTH;
                l.r = 2 * df_psize / 3.0;
                if (draw_line_to_drive1(x1, y1, x2, y2, kolor, 0, Layers[ptrs_point->warstwa].bw,
                                        Layers[ptrs_point->warstwa].grey) == FALSE)
                    return FALSE;
                if (Draw_Arc_To_Drive(&l) == FALSE) return FALSE;
            }
        break;
        case 9: /*pin_g*/
            if (PIN_IS_PRINTABLE)
            {
                line_width_type = LINE_NORM_WIDTH;
                l.r = 2 * df_psize / 3.0;
                if (draw_line_to_drive1(x12, y12, x12, y2, kolor, 0, Layers[ptrs_point->warstwa].bw,
                                        Layers[ptrs_point->warstwa].grey) == FALSE)
                    return FALSE;
                if (draw_line_to_drive1(x1, y1, x2, y2, kolor, 0, Layers[ptrs_point->warstwa].bw,
                                        Layers[ptrs_point->warstwa].grey) == FALSE)
                    return FALSE;
                if (Draw_Arc_To_Drive(&l) == FALSE) return FALSE;
            }
        break;
        case 10: /*pin_d*/
            if (PIN_IS_PRINTABLE)
            {
                line_width_type = LINE_NORM_WIDTH;
                l.r = 2 * df_psize / 3.0;
                if (draw_line_to_drive1(x12, y12, x12, y1, kolor, 0, Layers[ptrs_point->warstwa].bw,
                                        Layers[ptrs_point->warstwa].grey) == FALSE)
                    return FALSE;
                if (draw_line_to_drive1(x1, y1, x2, y2, kolor, 0, Layers[ptrs_point->warstwa].bw,
                                        Layers[ptrs_point->warstwa].grey) == FALSE)
                    return FALSE;
                if (Draw_Arc_To_Drive(&l) == FALSE) return FALSE;
            }
        break;
        case 11: /*pin_s*/
            if (PIN_IS_PRINTABLE)
            {
                line_width_type = LINE_NORM_WIDTH;
                l.r = 2 * df_psize / 3.0;
                if (draw_line_to_drive1(x1, y12, x2, y12, kolor, 0, Layers[ptrs_point->warstwa].bw,
                                        Layers[ptrs_point->warstwa].grey) == FALSE)
                    return FALSE;
                if (draw_line_to_drive1(x1, y1, x2, y2, kolor, 0, Layers[ptrs_point->warstwa].bw,
                                        Layers[ptrs_point->warstwa].grey) == FALSE)
                    return FALSE;
                if (Draw_Arc_To_Drive(&l) == FALSE) return FALSE;
            }
        break;

        case 12: /*fixed D*/
        case 13: /*fixed L*/
        case 14: /*fixed R*/
        case 15: /*fixed U*/


            line_width_type = LINE_THIN_WIDTH;
            if (draw_line_to_drive1(x12, y1, x12, y2, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
            if (draw_line_to_drive1(x1, y12, x2, y12, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;

            line_width_type = LINE_NORM_WIDTH;

            if (Draw_Fill_Circle_To_Drive (&Kolo) == FALSE) return FALSE;

            line_width_type = LINE_THICK_WIDTH;
            dxy= df_psize*2.5;  //2
            dd = df_psize * 1.5;  //*1.0
            dd1= df_psize;
            switch (ptrs_point->typ)
            {
                case 12:
                    if (draw_line_to_drive1(x1 - dxy, y12 - dd, x2 + dxy, y12 - dd, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
                case 13:
                    if (draw_line_to_drive1(x12 - dd, y1 - dxy, x12 - dd, y2 + dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
                case 14:
                    if (draw_line_to_drive1(x12 + dd, y1 - dxy, x12 + dd, y2 + dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
                case 15:
                    if (draw_line_to_drive1(x1 - dxy, y12 + dd, x2 + dxy, y12 + dd, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
            }
            break;
        case 16: /*pinned D*/
        case 17: /*pinned L*/
        case 18: /*pinned R*/
        case 19: /*pinned U*/
            line_width_type = LINE_THIN_WIDTH;
            if (draw_line_to_drive1(x12, y1, x12, y2, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
            if (draw_line_to_drive1(x1, y12, x2, y12, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;

            line_width_type = LINE_NORM_WIDTH;
            if (Draw_Arc_To_Drive(&l) == FALSE) return FALSE;

            dxy= df_psize*3;
            dd = df_psize*2;
            dd1= df_psize;
            line_width_type = LINE_THICK_WIDTH;
            switch (ptrs_point->typ)
            {
                case 16:
                    if (draw_line_to_drive1(x1 - dxy, y12 - dd, x2 + dxy, y12 - dd, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
                case 17:
                    if (draw_line_to_drive1(x12 - dd, y1 - dxy, x12 - dd, y2 + dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
                case 18:
                    if (draw_line_to_drive1(x12 + dd, y1 - dxy, x12 + dd, y2 + dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
                case 19:
                    if (draw_line_to_drive1(x1 - dxy, y12 + dd, x2 + dxy, y12 + dd, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
            }
            break;
        case 20: /*fixed roller D*/
        case 21: /*fixed roller L*/
        case 22: /*fixed roller R*/
        case 23: /*fixed roller U*/

            line_width_type = LINE_THIN_WIDTH;

            if (draw_line_to_drive1(x12, y1, x12, y2, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
            if (draw_line_to_drive1(x1, y12, x2, y12, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;

            if (Draw_Fill_Circle_To_Drive (&Kolo) == FALSE) return FALSE;

            dxy= df_psize*2.5;  //2.0
            dd=  df_psize*1.5;  //1.0
            dd1= df_psize;
            line_width_type = LINE_THICK_WIDTH;
            switch (ptrs_point->typ)
            {
                case 20:
                    if (draw_line_to_drive1(x1 - dxy, y12 - dd, x2 + dxy, y12 - dd, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    if (draw_line_to_drive1(x1 - dxy, y12 - dd - dd1, x2 + dxy, y12 - dd - dd1, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
                case 21:
                    if (draw_line_to_drive1(x12 - dd, y1 - dxy, x12 - dd, y2 + dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    if (draw_line_to_drive1(x12 - dd - dd1, y1 - dxy, x12 - dd - dd1, y2 + dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
                case 22:
                    if (draw_line_to_drive1(x12 + dd, y1 - dxy, x12 + dd, y2 + dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    if (draw_line_to_drive1(x12 + dd + dd1, y1 - dxy, x12 + dd + dd1, y2 + dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
                case 23:
                    if (draw_line_to_drive1(x1 - dxy, y12 + dd, x2 + dxy, y12 + dd, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    if (draw_line_to_drive1(x1 - dxy, y12 + dd + dd1, x2 + dxy, y12 + dd + dd1, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
            }
            break;
        case 24: /*pinned roller D*/
        case 25: /*pinned roller L*/
        case 26: /*pinned roller R*/
        case 27: /*pinned roller U*/
            line_width_type = LINE_THIN_WIDTH;
            if (draw_line_to_drive1(x12, y1, x12, y2, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
            if (draw_line_to_drive1(x1, y12, x2, y12, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;

            line_width_type = LINE_NORM_WIDTH;
            if (Draw_Arc_To_Drive(&l) == FALSE) return FALSE;
            dxy= df_psize*3;
            dd = df_psize*2;
            dd1= df_psize;
            line_width_type = LINE_THICK_WIDTH;
            switch (ptrs_point->typ)
            {
                case 24:
                    if (draw_line_to_drive1(x1 - dxy, y12 - dd, x2 + dxy, y12 - dd, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    if (draw_line_to_drive1(x1 - dxy, y12 - dd - dd1, x2 + dxy, y12 - dd - dd1, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
                case 25:
                    if (draw_line_to_drive1(x12 - dd, y1 - dxy, x12 - dd, y2 + dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    if (draw_line_to_drive1(x12 - dd - dd1, y1 - dxy, x12 - dd - dd1, y2 + dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
                case 26:
                    if (draw_line_to_drive1(x12 + dd, y1 - dxy, x12 + dd, y2 + dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    if (draw_line_to_drive1(x12 + dd + dd1, y1 - dxy, x12 + dd + dd1, y2 + dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
                case 27:
                    if (draw_line_to_drive1(x1 - dxy, y12 + dd, x2 + dxy, y12 + dd, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    if (draw_line_to_drive1(x1 - dxy, y12 + dd + dd1, x2 + dxy, y12 + dd + dd1, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
                    break;
            }
            break;
        case 28: /*no rotation Z*/
            line_width_type = LINE_THIN_WIDTH;

            if (draw_line_to_drive1(x12, y1, x12, y2, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
            if (draw_line_to_drive1(x1, y12, x2, y12, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;

            if (Draw_Fill_Circle_To_Drive (&Kolo) == FALSE) return FALSE;

            linestyle(96);
            dxy=df_psize*2.325;  //2.5
            dd=2*df_psize*0.25; //  sin(22)/1.5 sin(30)/1.5
            dd1=2*df_psize*0.618;  // cos(22)/1.5; cos(30)/1.5;
            line_width_type = LINE_THICK_WIDTH;

            if (draw_line_to_drive1(x12-dxy, y12+dd, x12-dd1, y12+dd, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
            if (draw_line_to_drive1(x12+dd1, y12+dd, x12+dxy, y12+dd, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
            if (draw_line_to_drive1(x12-dxy, y12-dd, x12-dd1, y12-dd, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
            if (draw_line_to_drive1(x12+dd1, y12-dd, x12+dxy, y12-dd, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;

            if (draw_line_to_drive1(x12-dd, y12+dxy, x12-dd, y12+dd1, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
            if (draw_line_to_drive1(x12-dd, y12-dd1, x12-dd, y12-dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
            if (draw_line_to_drive1(x12+dd, y12+dxy, x12+dd, y12+dd1, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;
            if (draw_line_to_drive1(x12+dd, y12-dd1, x12+dd, y12-dxy, kolor, 0, Layers[ptrs_point->warstwa].bw, Layers[ptrs_point->warstwa].grey) == FALSE) return FALSE;

            break;

    }
    return TRUE;
}


BOOL Draw_Spline_To_Drive(SPLINE *S)
/*---------------------------------*/
{
	int typ;
	PLINIA PL;
	LINIA L = Ldef;
	int kolor;
	int npts;
	float out_x[MaxNpts], out_y[MaxNpts];
	long TotL;
	int i;
	BOOL ret=FALSE;
	double PL_dl = 0;

	double bnodes_2_tangent_org, bnodes_2_tangent;
	SPLINE tmp_spline = Splinedef;
	float curviness_1;
	int node_count;
	

#define NPA 90 //56

	if (S->npts < 5)
	{
		kolor = MakeColor(GetColorWin(S->kolor));
		typ = (S->typ & 31);
		TotL = 0;

		for (i = 0; i < ((int)S->lp - 2) && i < (MaxNumPoints); i += 2)
		{
			TotL += (long)sqrt((float)((S->xy[i + 2] - S->xy[i]) * (S->xy[i + 2] - S->xy[i])) + (float)((S->xy[i + 3] - S->xy[i + 1]) * (S->xy[i + 3] - S->xy[i + 1])));
		}

		if (typ == 0)
			npts = 8 + TotL / d__luk; //identical as for arc
		else
			npts = TotL / 8; //lenght of pattern

		if (npts > NPA)
		{
			npts = NPA;
		}

		calc_bspline(S->lp, S->npts, S->xy, npts, out_x, out_y);

		for (i = 0; i < (npts - 1); i++)
		{

			L.x1 = out_x[i];
			L.y1 = out_y[i];
			L.x2 = out_x[i + 1];
			L.y2 = out_y[i + 1];

			parametry_lini(&L, &PL);

			PL_dl += PL.dl;

			if (typ == 0)
			{
				ret = (draw_line_to_drive(L.x1, L.y1, L.x2, L.y2, kolor, 0, Layers[S->warstwa].bw, Layers[S->warstwa].grey) == FALSE);
			}
			else ret = (draw_wzorzec(L.x1, L.y1, PL.dl, PL.sin, PL.cos, typ, kolor, TRUE, FALSE /*TRUE*/, Layers[S->warstwa].bw, Layers[S->warstwa].grey) == FALSE);
		}
	}
	else  //multipoints
	{
		curviness = S->xy[S->lp];

		tmp_spline.npts = 4;
		tmp_spline.lp = 8;
		tmp_spline.kolor = S->kolor;
		tmp_spline.typ = S->typ;
		tmp_spline.warstwa = S->warstwa;

		node_count = 1;

		set_bnodes(S, S->xy, &node_count);

		calc_tangents(S->closed, &node_count, &bnodes_2_tangent);

		if (S->closed == 0)
		{
			bnodes_2_tangent_org = bnodes[2].tangent;
			bnodes[2].tangent = bnodes_2_tangent;
			get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
			Draw_Spline_To_Drive(&tmp_spline);
			bnodes[2].tangent = bnodes_2_tangent_org;
		}

		for (i = 2; i < node_count - 2; i++)  //1 - extra_node
		{
			get_control_points(bnodes[i], bnodes[i + 1], tmp_spline.xy);
			Draw_Spline_To_Drive(&tmp_spline);
		}

		if ((node_count > 3) && (S->closed == 0))
		{
			get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
			Draw_Spline_To_Drive(&tmp_spline);
		}

	}

	return TRUE;
}

/*----------Draw_Tekst_To_Drive--------------------------------------------*/

BOOL Draw_Tekst_To_Drive(TEXT *t, int ink_plotter, int pen ,  int plt_type/*, double pen_width_ink*/)
/*--------------------------------------------------------------------------------------------*/
{
  //int i;
  long li, ad;
  double x1,y1,x2,y2;
  double x01, y01, ttf_x01, ttf_y01, x001, y001; 
  double x01__, y01__, x01___, y01___;
  double xu01, yu01, xu01_, yu01_ ;
  unsigned char *zn, *zn0;
  long lw;
  double tsin,tcos;
  double df_scale_x, df_scale_y, df_scale_x0, df_scale_y0, WysokoscT, df_width_marg;
  double  df_tl, df_tl_, df_tl0,  df_tl1, df_tl2 ;
  char *alf;
  double tsin_it ;
  BOOL b_hor ;
  T_Font_Off * ptrs_off ;
  unsigned int i_first, i_num ;
  double text_line_width, text_line_width0; /* grubosc linii dla teskstu */
  double tdl;
  BOOL underlining;
  int kolor;
  int f_type;
  float x1_f, y1_f, x2_f, y2_f;
  char *ptr;
  BOOL out_break;
  double ddf_tl;
  BOOL bw, grey;
  double font_scale = 1.0;
  double xind = 0, yind = 0;
  int font_index =0;
  unsigned int u8zn, Mzn;
  const uint8_t *end_ptr;
  char *ttf_zn;
  int TTF_left, TTF_top;
  int yMax;

  double matrix[2048];
  int i_matrix=0;
  int i_zn=0;
  bool index_pos=FALSE;
  

  bw = Layers[t->warstwa].bw;
  grey = Layers[t->warstwa].grey;
  
  if ((t->ukryty == 1) || (t->typ == n_typ_atrybut))
   {
    return TRUE;
   }

  kolor = MakeColor(GetColorWin(t->kolor));

  underlining=FALSE;
  
  switch (t->justowanie)
  {
  case 0:
	  Underl_del = 0.25*(t->wysokosc);
	  break;
  case 1:
	  Underl_del = 0.25*(t->wysokosc);
	  break;
  case 2:
	  Underl_del = 0.25*(t->wysokosc);
	  break;
  case 3:
	  Underl_del = 0.75*(t->wysokosc);
	  break;
  }
  
  if (t->italics == 1)
  {
   tsin_it = sin (ITALICS_ANGLE * Pi / 180) ;
  }
  WysokoscT = t->wysokosc;

  f_type = PTRS__Text_Style[t->czcionka]->type;
  if ((f_type == 2) && (type__drive == PLT_DRIVE))
  {
	  df_scale_y0 = t->wysokosc;
	  df_scale_x0 = df_scale_y0 * t->width_factor;
  }
  else Get_Text_Scale(t, &df_scale_x0, &df_scale_y0);
 
  df_scale_x = df_scale_x0;
  df_scale_y = df_scale_y0;

  if (f_type < 2)
  {
	  //df_width_marg = Get_W_Matix_Len(t->czcionka) * df_scale_x;
      df_width_marg = Get_Char_Matix_Len((unsigned int)'W', t->czcionka) * df_scale_x;
  }
  else
  {
	  //df_width_marg = Get_W_Matix_Len_TTF(t->czcionka, t->wysokosc);
      df_width_marg = Get_Char_Matix_Len_TTF((unsigned int)'W', t->czcionka, t->wysokosc);
  }

  
  text_line_width0 = t->wysokosc * s__Fonts_Style [t->czcionka].df_width_line ;
  text_line_width = text_line_width0;

  if (t->bold == 1) text_line_width *= 1.5;
  
  tsin=sin(t->kat);
  tcos=cos(t->kat);    

  x01=t->x;
  y01=t->y;
  ttf_x01 = x01;
  ttf_y01 = y01;


  x001 = x01;
  y001 = y01;

   zn=(unsigned char*)(t->text);
   tdl = Get_Text_Len (t, (char*)zn, font_scale, font_index, NULL, NULL, matrix, &i_matrix) ;

   if (f_type == 2)
   {

	   df_tl_ = -Get_TTF_Char_Left_Top(t, (char*)zn, font_scale, font_index, &TTF_left, &TTF_top);
   }
   else  df_tl_ = 0.0;

   if (df_tl_ > 0.0) df_tl_ = 0.0;

   df_tl = df_tl_;

   if (FALSE == Check_if_Equal (tdl, 0))
   {
     switch (t->justowanie)
	 {
	 case 0:
		 x01 = x001 + df_tl * tcos;  // to the left
		 y01 = y001 - Ky * df_tl * tsin;  //+Ky
		 break;
	  case 1: 
		  x01 = x001 + (- tdl + df_tl) * tcos;  // to the right
		  y01 = y001 + Ky * (tdl - df_tl) * tsin;  //+Ky
		  break;
	  case 2: 
		  x01 = x001 + ((- tdl + df_tl) / 2) * tcos;  //middle
		  y01 = y001 + (Ky * (tdl - df_tl)) / 2 * tsin;  //+Ky
		  break;
	  case 3: 
		  x01 = x001 + ((-tdl + df_tl) / 2) * tcos + (t->wysokosc / 2) * tsin;  //central
		  y01 = y001 + Ky * (tdl / 2 - df_tl) * tsin - (t->wysokosc / 2) * tcos;  //+Ky
		  break;
	 }
   }

   ttf_x01 = x01;
   ttf_y01 = y01;
  
  b_hor = FALSE ;
  if (TRUE == Check_if_Equal (t->kat, 0))
  {
     b_hor = TRUE ;
  }

  out_break = FALSE;
  ptrs_off = PTRS__Text_Style [t->czcionka]->ptrs_off ;
  alf = PTRS__Text_Style [t->czcionka]->ptrsz_font ;

  i_first = PTRS__Text_Style [t->czcionka]->first_char ;
  i_num = PTRS__Text_Style[t->czcionka]->num_char;
  df_tl0 = tdl;

  if ( type__drive != PRN_DRIVE)
   {  
    if (ink_plotter==1)
     {

       plt_pen_width_ink (pen, text_line_width);
     }
   }  
  
  zn0 = zn;
  ttf_zn = (char*)zn;


  if (f_type == 2)
  {
	  Get_Face_Character_yMax(t, &yMax);

	  if ((type__drive == PRN_DRIVE) && (type__printer != PRN_PDF)) init_ttf_text_to_drive(t, font_scale, font_index, kolor, bw, grey);
  }

  while ((!out_break) && (*zn != '\0'))
  {
	  zn0 = zn;

	  while (*zn != '\0')
	  {
		  if (*zn == '\r')
		  {
			  zn += 1;
			  continue;
		  }

		  if (*zn == '\n')
		  {
			  if (t->underline == TRUE)
			  {
					df_tl1 = df_tl0 - Get_Text_Len(t, (char*)(zn0), 1.0, 0, NULL, NULL, NULL, NULL); //_
					df_tl1 += df_tl_;
					df_tl2 = df_tl0 - Get_Text_Len(t, (char*)zn, font_scale, font_index, NULL, NULL, NULL, NULL); //_
					df_tl2 += df_tl_;

					if (df_tl1 != df_tl2)
					{

						switch (t->justowanie)
						{
						case 0:
							break;
						case 1:
							ddf_tl = (df_tl2 - df_tl1);
							df_tl1 -= ddf_tl;
							df_tl2 -= ddf_tl;
							break;
						case 2:
							ddf_tl = (df_tl2 - df_tl1) / 2.0;
							df_tl1 -= ddf_tl;
							df_tl2 -= ddf_tl;
							break;
						case 3:
							ddf_tl = (df_tl2 - df_tl1) / 2.0;
							df_tl1 -= ddf_tl;
							df_tl2 -= ddf_tl;
							break;
						}

						df_tl1 = df_tl1 - df_width_marg * 0.1;
						df_tl2 = df_tl2 + df_width_marg * 0.1;

						x01__ = x001 + df_tl1 * tcos;
						y01__ = y001 - Ky * df_tl1 * tsin;

						x01___ = x001 + df_tl2 * tcos;
						y01___ = y001 - Ky * df_tl2 * tsin;

						xu01_ = x01__ + Underl_del * tsin;
						yu01_ = y01__ - Underl_del * tcos;
						xu01 = x01___ + Underl_del * tsin;
						yu01 = y01___ - Underl_del * tcos;

						if (linen_plt1 /*linen_plt*/(xu01_, yu01_, xu01, yu01, 0, kolor, text_line_width, bw, grey) == FALSE) return FALSE;
					}

			  }

			  if ((f_type == 2) && (type__drive == PRN_DRIVE) && (type__printer == PRN_PDF))
			  {
				
				  draw_ttf_text_to_drive(t, ttf_zn, ttf_x01, ttf_y01, font_scale, font_index, kolor, bw, grey, index_pos);
			  }

			  zn += 1;

			  zn0 = zn;
			  ttf_zn = (char *)zn;

              i_matrix=0;
              i_zn=0;

			  df_tl0 = Get_Text_Len(t, (char*)zn, font_scale, font_index, NULL, NULL, matrix, &i_matrix);

			  if (f_type == 2) df_tl_ = -Get_TTF_Char_Left_Top(t, (char*)zn, font_scale, font_index, &TTF_left, &TTF_top);
			  else df_tl_ = 0.0;
			  if (df_tl_ > 0.0) df_tl_ = 0.0;
			  df_tl = df_tl_;

			  x001 += WysokoscT * (float)((t->spacing) / 10.0 + 1.0)*tsin; //SPACING
			  y001 -= WysokoscT * (float)((t->spacing) / 10.0 +1.0)*tcos; //SPACING	 

			  switch (t->justowanie)
			  {
			  case 0: //to the left
				  x01 = x001 + df_tl * tcos;
				  y01 = y001 - Ky * df_tl * tsin;  //-Ky
				  break;
			  case 1: //to the right
				  x01 = x001 + (-df_tl0 + df_tl) * tcos;
				  y01 = y001 - Ky * (-df_tl0 + df_tl) * tsin;  //-Ky
				  break;
			  case 2: //middle
				  x01 = x001 + (-df_tl0 / 2 + df_tl) * tcos;
				  y01 = y001 - Ky * (-df_tl0 / 2 + df_tl) * tsin; //-Ky
				  break;
			  case 3: //central
				  x01 = x001 + (-df_tl0 / 2 + df_tl) * tcos + (WysokoscT / 2) * tsin;  /*centralnie*/
				  y01 = y001 - Ky * (-df_tl0 / 2 + df_tl) * tsin - (WysokoscT / 2) * tcos;  //-Ky
				  break;
			  }

			  ttf_x01 = x01;
			  ttf_y01 = y01;

			  if (*zn == '\r')
			  {
				  zn += 1;
				  continue;
			  }

              if (*zn == '\n')
              {
                  continue;
              }

			  if (*zn == '\0')
			  {
				  break;
			  }
		  }

		  if ((*zn == '\45') &&
			  (*(zn + 1) == '\45') &&
			  ((*(zn + 2) == '\125') || (*(zn + 2) == '\165')))
		  {
			  if (underlining == FALSE) underlining = TRUE;
			  else underlining = FALSE;
			  zn += 3;
			  if (*zn == '\0') break;
		  }
          if (*zn == '|')
          {
              if (index_pos)
              {
                  zn++;
                  continue;
              }
          }
		  if (*zn == '\176') //~
		  {
			  if ((f_type == 2) && (type__drive == PRN_DRIVE) && (type__printer == PRN_PDF))
				  //printing from last front char 
				  draw_ttf_text_to_drive(t, ttf_zn, ttf_x01, ttf_y01, font_scale, font_index, kolor, bw, grey, index_pos);
              zn++;
              font_index -= 1;
              index_pos=TRUE;

			  font_scale = pow(INDEX_FACTOR, abs(font_index));
			  text_line_width = text_line_width0 * font_scale;
			  df_scale_x = df_scale_x0 * font_scale;
			  df_scale_y = df_scale_y0 * font_scale;
			  
			  ttf_zn = (char *)zn;
			  ttf_x01 = x01;
			  ttf_y01 = y01;

			  if (font_index > 0)
                  yind = WysokoscT * (1.0 - font_scale + INDEX_SHIFT);
              else if (font_index < 0)
                  yind = WysokoscT * (-INDEX_SHIFT);
				else yind = 0.0;
			  xind = 0.0;
					  
			  if (t->italics == 1) xind += yind * tsin_it;
			  if (b_hor == FALSE) obrd(tsin, tcos, xind, yind, &xind, &yind);
					 
			  ttf_x01 += xind;
			  ttf_y01 += yind;

			  if ((f_type == 2) && (type__drive == PRN_DRIVE) && (type__printer != PRN_PDF))
						amend_ttf_text_to_drive(t, font_scale, font_index);
				  
			  if (*zn == '\40') zn++;
              continue;
		  }
		  else if (*zn == '\136') //^
		  {
			  if ((f_type == 2) && (type__drive == PRN_DRIVE) && (type__printer == PRN_PDF))
				  draw_ttf_text_to_drive(t, ttf_zn, ttf_x01, ttf_y01, font_scale, font_index, kolor, bw, grey, index_pos);
				
			  zn++;  
			  font_index += 1;

              index_pos=TRUE;

			  font_scale = pow(INDEX_FACTOR, abs(font_index));
			  text_line_width = text_line_width0 * font_scale;
			  df_scale_x = df_scale_x0 * font_scale;
			  df_scale_y = df_scale_y0 * font_scale;

			  ttf_zn = (char *)zn;
		      ttf_x01 = x01;
			  ttf_y01 = y01;

			  if (font_index > 0)
                  yind = WysokoscT * (1.0 - font_scale + INDEX_SHIFT);
              else if (font_index < 0)
                  yind = WysokoscT * (-INDEX_SHIFT);
				else yind = 0.0;
			  xind = 0.0;

			  if (t->italics == 1) xind += yind * tsin_it;
					 
			  if (b_hor == FALSE)  obrd(tsin, tcos, xind, yind, &xind, &yind);
					 
			  ttf_x01 += xind;
			  ttf_y01 += yind;

			  if ((f_type == 2) && (type__drive == PRN_DRIVE) && (type__printer != PRN_PDF))
					amend_ttf_text_to_drive(t, font_scale, font_index);
				
			  if (*zn == '\40') zn++;
			  continue;
		  }
		  else
		  {
				if (*zn >= 127)
				{
					//convert to UNICODE and shift index
					u8zn = utf8_to_ucs2(zn, &end_ptr);

                    if (u8zn == 8308) //⁴ e.g. m⁴
                        zn+=2;
                    else {
                        if (u8zn > 1920) u8zn = 32;
                        zn++;
                    }
				}
				else u8zn = (unsigned int)zn[0];

				if (u8zn < 32)
				{
					zn++;
					continue;
				}

				if (f_type == 0)
				{
					//conver to Mazovia
					Mzn = u8toMazovia(u8zn);
					ad = ptrs_off[Mzn - i_first].i_offset;
				}
				else if ((f_type == 1) || (f_type == 3))
				{
					ad = ptrs_off[u8zn].i_offset;
					ptr = alf + ad;
				}
				else if ((f_type == 2) && (type__drive == PLT_DRIVE))
				{
					ad = Get_TTF_Char_Outline(t, u8zn, alft, &lw, yMax);	 
					ptr = alft;
				}

				if ((f_type < 2) || (f_type == 3) || ((f_type == 2) && (type__drive == PLT_DRIVE)))
				{

					if ((ad != OFFEMPTY) && (u8zn <= (i_first + i_num)))
					{
						if (f_type == 0)
						{
							//conver to Mazovia
							Mzn = u8toMazovia(u8zn);
							lw = ptrs_off[Mzn - i_first].vectorsno;
						}
						else if ((f_type == 1) || (f_type == 3))
						{
							lw = ptrs_off[u8zn].vectorsno;
						}

						if (font_index > 0)
                            yind = WysokoscT * (1.0 - font_scale + INDEX_SHIFT);
                        else if (font_index < 0)
                            yind = WysokoscT * (-INDEX_SHIFT);
						else yind = 0.0;

						if (type__drive == PLT_DRIVE)
						{
							if (plt_type == 99)
							{
								plt_mark_char(*zn);
							}
						}

						for (li = 1; li <= lw; li++)
						{
							if (f_type > 0)
							{
								memmove(&x1_f, ptr, sizeof(float));
								ptr += sizeof(float);
								memmove(&y1_f, ptr, sizeof(float));
								ptr += sizeof(float);
								memmove(&x2_f, ptr, sizeof(float));
								ptr += sizeof(float);
								memmove(&y2_f, ptr, sizeof(float));
								ptr += sizeof(float);
								x1 = x1_f * df_scale_x *4.5;
								y1 = y1_f * df_scale_y *4.5;
								x2 = x2_f * df_scale_x *4.5;
								y2 = y2_f * df_scale_y *4.5;
							}
							else
							{
								
								x1 = alf[ad++] * df_scale_x;
								y1 = alf[ad++] * df_scale_y;
								x2 = alf[ad++] * df_scale_x;
								y2 = alf[ad++] * df_scale_y;
							}

							y1 += yind;
							y2 += yind;

							if (t->italics == 1)
							{
								x1 += y1 * tsin_it;
								x2 += y2 * tsin_it;
							}
							if (b_hor == FALSE)
							{
								obrd(tsin, tcos, x1, y1, &x1, &y1);
								obrd(tsin, tcos, x2, y2, &x2, &y2);
							}
							if (linen_plt2 /*linen_plt*/(x01 + x1, y01 + y1, x01 + x2, y01 + y2, 0, kolor, text_line_width, bw, grey) == FALSE) return FALSE;
						}
					}
				}
				else if (type__printer != PRN_PDF)
				{
					if (font_index > 0)
                        yind = WysokoscT * (1.0 - font_scale + INDEX_SHIFT);
                    else if (font_index < 0)
                        yind = WysokoscT * (-INDEX_SHIFT);
					else yind = 0.0;
					xind = 0.0;

					if (t->italics == 1)
					{
						xind += yind * tsin_it;
					}
					if (b_hor == FALSE)
					{
						obrd(tsin, tcos, xind, yind, &xind, &yind);
					}

					draw_ttf_char_to_drive(t, u8zn, x01+xind, y01+yind, font_scale, font_index, kolor, bw, grey);
				}

              df_tl = matrix[i_zn];
			  
			  df_tl += df_tl_;

              i_zn++;

              zn++;

			  switch (t->justowanie)
			  {
			  case 0: //do lewej
				  x01 = x001 + df_tl * tcos;
				  y01 = y001 - Ky * df_tl * tsin;
				  break;
			  case 1: //do prawej
				  x01 = x001 + (-df_tl0 + df_tl) * tcos;
				  y01 = y001 - Ky * (-df_tl0 + df_tl) * tsin;
				  break;
			  case 2: //srodkowo
				  x01 = x001 + (-df_tl0 / 2 + df_tl) * tcos;
				  y01 = y001 - Ky * (-df_tl0 / 2 + df_tl) * tsin;
				  break;
			  case 3: //centralnie
				  x01 = x001 + (-df_tl0 / 2 + df_tl) * tcos + (WysokoscT / 2) * tsin;  /*centralnie*/
				  y01 = y001 - Ky * (-df_tl0 / 2 + df_tl) * tsin - (WysokoscT / 2) * tcos;
				  break;
			  }
		  }
	  }
   }

  
  if ((f_type == 2) && (type__drive == PRN_DRIVE) && (type__printer == PRN_PDF))
  {
	  if (strlen(ttf_zn) > 0)
	  {
		  draw_ttf_text_to_drive(t, ttf_zn, ttf_x01, ttf_y01, font_scale, font_index, kolor, bw, grey, index_pos);
	  }
  }
  

   if (t->underline == TRUE)
   {
	   df_tl1 = df_tl0 - Get_Text_Len(t, (char*)(zn0), 1.0, 0, NULL, NULL, NULL, NULL); //_
	   df_tl1 += df_tl_;
	   df_tl2 = df_tl0 - Get_Text_Len(t, (char*)zn, font_scale, font_index, NULL, NULL, NULL, NULL); //_
	   df_tl2 += df_tl_;

	   if (df_tl1 != df_tl2)
	   {

		   switch (t->justowanie)
		   {
		   case 0:
			   break;
		   case 1:
			   ddf_tl = (df_tl2 - df_tl1);
			   df_tl1 -= ddf_tl;
			   df_tl2 -= ddf_tl;
			   break;
		   case 2:
			   ddf_tl = (df_tl2 - df_tl1) / 2.0;
			   df_tl1 -= ddf_tl;
			   df_tl2 -= ddf_tl;
			   break;
		   case 3:
			   ddf_tl = (df_tl2 - df_tl1) / 2.0;
			   df_tl1 -= ddf_tl;
			   df_tl2 -= ddf_tl;
			   break;
		   }


		   df_tl1 = df_tl1 - df_width_marg * 0.1;

		   x01__ = x001 + df_tl1 * tcos;
		   y01__ = y001 - Ky * df_tl1 * tsin;

		   x01___ = x001 + df_tl2 * tcos;
		   y01___ = y001 - Ky * df_tl2 * tsin;

		   xu01_ = x01__ + Underl_del * tsin;
		   yu01_ = y01__ - Underl_del * tcos;
		   xu01 = x01___ + Underl_del * tsin;
		   yu01 = y01___ - Underl_del * tcos;

		   if (linen_plt1 /*linen_plt*/(xu01_, yu01_, xu01, yu01, 0, kolor, text_line_width, bw, grey) == FALSE) return FALSE;
	   }

   }

  if ( type__drive != PRN_DRIVE)
   {  
    if (ink_plotter==1)
     {
      /*przywrocenie grubosci piorka*/
      write_ter ();
     } 
   }   

  return TRUE;
}

/*------------------------------------------------------------*/

/***************  LOCAL DECLARATION  *****************************/

static int inside(double x, double x1, double x2)
/*----------------------------------------------*/
{ int ix;
  ix=0;
  if (x<x1) ix=-1;
  else if (x>x2) ix=1;
  return ix;
}

static int inside_width(double x, double x1, double x2, double text_line_width)
/*----------------------------------------------------------------------------*/
{
	int ix;
	ix = 0;
	if ((x + text_line_width/2) < x1) ix = -1;
	else if ((x - text_line_width/2) > x2) ix = 1;
	return ix;
}

int  point_clip(float x, float y)
/*----------------------------------------------*/
{
  if ((x>=X1) &&
      (x<=X2) &&
      (y>=Y1) &&
      (y<=Y2)) return 1;
  return 0;
}


int line_clip(double *x1, double  *y1, double  *x2, double *y2)
/*-------------------------------------------------------------------*/
{
  int ix1,ix2,iy1,iy2;
  double  xx,yy;
  double a,a1;

  if (TRUE == Check_if_Equal (*x1, *x2))
  {
    *x1 = *x2 ;		//dla unikniecia "float"
  }
  if (TRUE == Check_if_Equal (*y1, *y2))
  {
    *y1 = *y2 ;
  }

  ix1=inside(*x1,X1,X2);
  ix2=inside(*x2,X1,X2);
  iy1=inside(*y1,Y1,Y2);
  iy2=inside(*y2,Y1,Y2);
  if (ix1*ix2==1 || iy1*iy2==1) return 0;
  if(*x1!=*x2)  a= (*y2-*y1)/(*x2-*x1);
  if(*y1!=*y2) a1= (*x2-*x1)/(*y2-*y1);
  if (ix1)
   { xx=ix1>0 ? X2 : X1;
     *y1=*y1 + a*(xx-*x1);
     *x1=xx;
	 iy1=inside(*y1,Y1,Y2);
	 if (iy1*iy2==1) return 0;
   }
  if (iy1)
   { yy=iy1>0 ? Y2 : Y1;
     *x1=*x1 + a1*(yy-*y1);
     *y1=yy;
	 ix1=inside(*x1,X1,X2);
     if (ix1) return 0;
   }
  if (ix2)
   { xx=ix2>0 ? X2 : X1;
     *y2=*y2 + a*(xx-*x2);
     *x2=xx;
     iy2=inside(*y2,Y1,Y2);
   }
  if (iy2)
   { yy=iy2>0 ? Y2 : Y1;
     *x2=*x2 + a1*(yy-*y2);
	 *y2=yy;
   }
  return 1;
}

int line_clip_width(double *x1, double  *y1, double  *x2, double *y2, double text_line_width)
/*----------------------------------------------------------------------------------------------*/
{
	int ix1, ix2, iy1, iy2;
	double  xx, yy;
	double a, a1;

	text_line_width /=2.0;

	if (TRUE == Check_if_Equal(*x1, *x2))
	{
		*x1 = *x2;		//dla unikniecia "float"
	}
	if (TRUE == Check_if_Equal(*y1, *y2))
	{
		*y1 = *y2;
	}

	ix1 = inside_width(*x1, X1, X2, text_line_width);
	ix2 = inside_width(*x2, X1, X2, text_line_width);
	iy1 = inside_width(*y1, Y1, Y2, text_line_width);
	iy2 = inside_width(*y2, Y1, Y2, text_line_width);

	if (ix1*ix2 == 1 || iy1 * iy2 == 1) return 0;
	if (*x1 != *x2)  a = (*y2 - *y1) / (*x2 - *x1);
	if (*y1 != *y2) a1 = (*x2 - *x1) / (*y2 - *y1);
	if (ix1)
	{
		xx = ix1 > 0 ? X2 : X1;
		*y1 = *y1 + a * (xx - *x1);
		*x1 = xx;
		iy1 = inside_width(*y1, Y1, Y2, text_line_width);
		if (iy1*iy2 == 1) return 0;
	}
	if (iy1)
	{
		yy = iy1 > 0 ? Y2 : Y1;
		*x1 = *x1 + a1 * (yy - *y1);
		*y1 = yy;
		ix1 = inside_width(*x1, X1, X2, text_line_width);
		if (ix1) return 0;
	}
	if (ix2)
	{
		xx = ix2 > 0 ? X2 : X1;
		*y2 = *y2 + a * (xx - *x2);
		*x2 = xx;
		iy2 = inside_width(*y2, Y1, Y2, text_line_width);
	}
	if (iy2)
	{
		yy = iy2 > 0 ? Y2 : Y1;
		*x2 = *x2 + a1 * (yy - *y2);
		*y2 = yy;
	}
	return 1;
}

int arc_clip(LUK *l)
 { int i;
   LINIA L=Ldef;
   double kata, katb;
   double L_x1, L_y1, L_x2, L_y2;
   double kat1, kat2;
   double d_kat;
   int nd_kat;

   if ((l->x+l->r<X1) || (l->x-l->r>X2) ||
       (l->y+l->r<Y1) || (l->y-l->r>Y2)) return 0;    //pominiecie luku

   if ((l->x-l->r>=X1) && (l->x+l->r<=X2) &&
       (l->y-l->r>=Y1) && (l->y+l->r<=Y2)) return 1;  //przerysowanie luku
   //zamiana na wektory - przyjmujemy odcinek luku 5 deg

   L.warstwa=Current_Layer;
   L.kolor=l->kolor;
   L.typ=l->typ;
   L.blok=1;
   L.obiektt1=0;
   L.obiektt2=l->obiektt2;
   L.obiektt3=0;

   //podzial luku na 36 czesci ->kat1,  ->kat2
   kat1=l->kat1;
   kat2=l->kat2;
   if (kat1>kat2) kat1-=(Pi*2);
   if ((kat2-kat1) > Pi) nd_kat=36; else nd_kat=18;
   d_kat=(kat2-kat1)/nd_kat;


   for (i=0; i<nd_kat; i++)
    {
      kata =i;
      kata *= d_kat;
      kata += kat1;
      katb =i+1;
      katb *= d_kat;
      katb += kat1;
      L_x1=l->x+l->r*cos(kata);
      L_y1=l->y+l->r*sin(kata);
      L_x2=l->x+l->r*cos(katb);
      L_y2=l->y+l->r*sin(katb);
      if (line_clip(&L_x1, &L_y1, &L_x2, &L_y2))
       {
        L.x1=L_x1;
        L.y1=L_y1;
        L.x2=L_x2;
        L.y2=L_y2;
        if (dodaj_obiekt((BLOK*)dane, (void*)&L)==NULL) return 0;
       }
    }
   return 2;
 }


int circle_clip(OKRAG *O)
 { int i;
   LINIA L=Ldef;
   double kata, katb;
   double L_x1, L_y1, L_x2, L_y2;

   if ((O->x+O->r<X1) || (O->x-O->r>X2) ||
       (O->y+O->r<Y1) || (O->y-O->r>Y2)) return 0;    //pominiecie okregu

   if ((O->x-O->r>=X1) && (O->x+O->r<=X2) &&
       (O->y-O->r>=Y1) && (O->y+O->r<=Y2)) return 1;  //przerysowanie okregu
   //zamiana na wektory - przyjmujemy odcinek luku 10 deg

   L.warstwa=Current_Layer;
   L.kolor=O->kolor;
   L.typ=O->typ;
   L.blok=1;
   L.obiektt1=0;
   L.obiektt2=O->obiektt2;
   L.obiektt3=0;


   for (i=0; i<36; i++)
    {
      kata=i;
      kata /= 18;
      kata *= Pi;
      katb=i+1;
      katb /=18;
      katb *= Pi;
      L_x1=O->x+O->r*cos(kata);
      L_y1=O->y+O->r*sin(kata);
      L_x2=O->x+O->r*cos(katb);
      L_y2=O->y+O->r*sin(katb);
      if (line_clip(&L_x1, &L_y1, &L_x2, &L_y2))
       {
        L.x1=L_x1;
        L.y1=L_y1;
        L.x2=L_x2;
        L.y2=L_y2;
        if (dodaj_obiekt((BLOK*)dane, (void*)&L)==NULL) return 0;
       }
    }
   return 2;
 }


int solid_clip(WIELOKAT *S)
/*-----------------------*/
{ double x_s[8];
  double y_s[8];
  float S_x[8], S_y[8];
  int i;
  int k1,k2,k3,k4;
  int lp0,lp1;
  //wyznaczenie dodatkowych punktow przeciec
  x_s[0]=S->xy[0];
  y_s[0]=S->xy[1];
  x_s[1]=S->xy[2];
  y_s[1]=S->xy[3];

  x_s[2]=S->xy[2];
  y_s[2]=S->xy[3];
  x_s[3]=S->xy[4];
  y_s[3]=S->xy[5];

  if (S->lp==6)
   {
    x_s[4]=S->xy[4];
    y_s[4]=S->xy[5];
    x_s[5]=S->xy[0];
    y_s[5]=S->xy[1];
   }
    else
     {
      x_s[4]=S->xy[4];
      y_s[4]=S->xy[5];
      x_s[5]=S->xy[6];
      y_s[5]=S->xy[7];

      x_s[6]=S->xy[6];
      y_s[6]=S->xy[7];
      x_s[7]=S->xy[0];
      y_s[7]=S->xy[1];
     }

  k1=k2=k3=k4=0;
  k1=line_clip(&x_s[0],&y_s[0],&x_s[1],&y_s[1]);
  k2=line_clip(&x_s[2],&y_s[2],&x_s[3],&y_s[3]);
  k3=line_clip(&x_s[4],&y_s[4],&x_s[5],&y_s[5]);
  if (S->lp>6) k4=line_clip(&x_s[6],&y_s[6],&x_s[7],&y_s[7]);
  lp0=0;
  if (k1)
   {
    lp0++;
    S_x[lp0-1]=x_s[0];
    S_y[lp0-1]=y_s[0];
    lp0++;
    S_x[lp0-1]=x_s[1];
    S_y[lp0-1]=y_s[1];
   }
  if (k2)
   {
    if ((Check_if_Equal(S_x[lp0-1],x_s[2])==FALSE) ||
        (Check_if_Equal(S_x[lp0-1],y_s[2])==FALSE))
     {
      lp0++;
      S_x[lp0-1]=x_s[2];
      S_y[lp0-1]=y_s[2];
     }
    lp0++;
    S_x[lp0-1]=x_s[3];
    S_y[lp0-1]=y_s[3];
   }
  if (k3)
  {
    if ((Check_if_Equal(S_x[lp0-1],x_s[4])==FALSE) ||
        (Check_if_Equal(S_x[lp0-1],y_s[4])==FALSE))
     {
      lp0++;
      S_x[lp0-1]=x_s[4];
      S_y[lp0-1]=y_s[4];
     }
    lp0++;
    S_x[lp0-1]=x_s[5];
    S_y[lp0-1]=y_s[5];
  }
 if (k4)
  {
    if ((Check_if_Equal(S_x[lp0-1],x_s[6])==FALSE) ||
        (Check_if_Equal(S_x[lp0-1],y_s[6])==FALSE))
     {
      lp0++;
      S_x[lp0-1]=x_s[6];
      S_y[lp0-1]=y_s[6];
     }
    lp0++;
    S_x[lp0-1]=x_s[7];
    S_y[lp0-1]=y_s[7];
  }
  if (lp0<3) return 0;
  lp1=0;
  for (i=0; i<lp0; i++)
   {
     lp1++;
     S->xy[lp1-1]=S_x[i];
     lp1++;
     S->xy[lp1-1]=S_y[i];
   }
  S->lp=lp0*2;
  S->n = 8 + S->lp * sizeof (float) ;
  Normalize_Solid (S) ;
  return 1;
}

static BOOL draw_line_to_drive ( double x1, double y1, double x2, double y2, int kolor0, double text_line_width, BOOL bw, BOOL grey)
/*---------------------------------------------------------------------------------------------------------------------------------*/
{
    BOOL ret_val;
    int kolor;
	
    if (print_inversion == TRUE)
     {
		if (print_rotation == TRUE)
		{
			//// if reflection
		   x1 = FormatX_S - x1;
	       x2 = FormatX_S - x2;
		   y1 = FormatY_S - y1;
		   y2 = FormatY_S - y2;
		}
		else
		{

			x1 = FormatX_S - x1 + FormatX_S0;
			x2 = FormatX_S - x2 + FormatX_S0;
			y1 = FormatY_S + FormatY_S0 - y1;
			y2 = FormatY_S + FormatY_S0 - y2;
			
		}
     }
	
    kolor=kolor0;
	if ((!print_inversion) || (print_rotation))
	{
		if (!line_clip_width(&x1, &y1, &x2, &y2, text_line_width)) return TRUE;
	}
    if ( type__drive == PRN_DRIVE)
      ret_val = Draw_Line_To_Prn (x1, y1, x2, y2, kolor, text_line_width, bw, grey);
    else
      ret_val = Draw_Line_To_Plt (x1, y1, x2, y2);
    return ret_val;
}


void init_ttf_text_to_drive(TEXT *t, double font_scale, int font_index, int kolor0, BOOL bw, BOOL grey)
{
	Init_TTF_text_to_matrix(t, font_scale, font_index, kolor0, bw, grey);
}

void amend_ttf_text_to_drive(TEXT *t, double font_scale, int font_index)
{
	Amend_TTF_text_to_matrix(t, font_scale, font_index);
}


void draw_ttf_text_to_drive(TEXT *t, char *zn, double x01, double y01, double font_scale, int font_index, int kolor0, BOOL bw, BOOL grey, BOOL index_pos)
{
	
	if (print_inversion == TRUE)
	{
		if (print_rotation == TRUE)
		{
			//// if reflection
			x01 = FormatX_S - x01;
			y01 = FormatY_S - y01;
		}
		else
		{
			x01 = FormatX_S - x01 + FormatX_S0;
			y01 = FormatY_S + FormatY_S0 - y01;
		}
	}

	Draw_TTF_text_to_matrix(t, zn, x01, y01, font_scale, font_index, kolor0, bw, grey, index_pos);
}


void draw_ttf_char_to_drive(TEXT *t, unsigned int unicode, double x01, double y01, double font_scale, int font_index, int kolor0, BOOL bw, BOOL grey)
{


	if (print_inversion == TRUE)
	{
		if (print_rotation == TRUE)
		{
			//// if reflection
			x01 = FormatX_S - x01;
			y01 = FormatY_S - y01;
		}
		else
		{
			x01 = FormatX_S - x01 + FormatX_S0;
			y01 = FormatY_S + FormatY_S0 - y01;
		}
	}

	Draw_TTF_char_to_matrix(t, unicode, x01, y01, font_scale, font_index, kolor0, bw, grey);

}

static BOOL draw_line_to_drive1 ( double x1, double y1, double x2, double y2, int kolor0, double text_line_width, BOOL bw, BOOL grey)
/*---------------------------------------------------------------------------------------------------------------------------------*/
{
    BOOL ret_val;
    int kolor;

	if (print_inversion == TRUE)
	{
		if (print_rotation == TRUE)
		{
			//// if reflection
			x1 = FormatX_S - x1;
			x2 = FormatX_S - x2;
			y1 = FormatY_S - y1;
			y2 = FormatY_S - y2;
		}
		else
		{
			x1 = FormatX_S - x1 + FormatX_S0;  //this is for reflection
			x2 = FormatX_S - x2 + FormatX_S0;  //this is for reflection
			
			y1 = FormatY_S + FormatY_S0 - y1;
			y2 = FormatY_S + FormatY_S0 - y2;
		}
	}
	
    kolor=kolor0;
	
	
		if (!line_clip_width(&x1, &y1, &x2, &y2, text_line_width)) return TRUE;  //TEMPORARY CONDITIONAL CLIP
	
    if ( type__drive == PRN_DRIVE)
      ret_val = Draw_Line_To_Prn1 (x1, y1, x2, y2, kolor, text_line_width, bw, grey);
    else
      ret_val = Draw_Line_To_Plt (x1, y1, x2, y2);
    return ret_val;
}


static BOOL draw_line_to_drive_trans(double x1, double y1, double x2, double y2, int kolor0, double text_line_width, BOOL bw, BOOL grey)
/*---------------------------------------------------------------------------------------------------------------------------------*/
{
	BOOL ret_val;
	int kolor;

	if (print_inversion == TRUE)
	{
		if (print_rotation == TRUE)
		{
			//// if reflection
			x1 = FormatX_S - x1;
			x2 = FormatX_S - x2;
			y1 = FormatY_S - y1;
			y2 = FormatY_S - y2;
		}
		else
		{
			x1 = FormatX_S - x1 + FormatX_S0;  //this is for reflection
			x2 = FormatX_S - x2 + FormatX_S0;  //this is for reflection
			
			y1 = FormatY_S + FormatY_S0 - y1;
			y2 = FormatY_S + FormatY_S0 - y2;
		}
	}

	kolor = kolor0;

	
	if (!line_clip_width(&x1, &y1, &x2, &y2, text_line_width)) return TRUE;  //TEMPORARY CONDITIONAL CLIP

	ret_val = Draw_Line_To_Prn_trans(x1, y1, x2, y2, kolor, text_line_width, bw, grey);
	
	return ret_val;
}



static BOOL draw_line_to_drive_pattern(double x1, double y1, double x2, double y2)
/*-------------------------------------------------------------------------------*/
{
	
	if (print_inversion == TRUE)
	{
		if (print_rotation == TRUE)
		{
			//// if reflection
			x1 = FormatX_S - x1;
			x2 = FormatX_S - x2;
			y1 = FormatY_S - y1;
			y2 = FormatY_S - y2;
		}
		else
		{
			x1 = FormatX_S - x1 + FormatX_S0;  //this is for reflection
			x2 = FormatX_S - x2 + FormatX_S0;  //this is for reflection
			
			y1 = FormatY_S + FormatY_S0 - y1;
			y2 = FormatY_S + FormatY_S0 - y2;
		}
	}

	
	
	if (!line_clip(&x1, &y1, &x2, &y2)) return TRUE;  //TEMPORARY CONDITIONAL CLIP

	
	return Draw_Line_To_Prn_Pattern(x1, y1, x2, y2);

}


static BOOL draw_line_to_drive2(double x1, double y1, double x2, double y2, int kolor0, double text_line_width, BOOL bw, BOOL grey)
/*---------------------------------------------------------------------------------------------------------------------------------*/
{
	BOOL ret_val;
	int kolor;

	if (print_inversion == TRUE)
	{
		if (print_rotation == TRUE)
		{
			x1 = FormatX_S - x1;
			x2 = FormatX_S - x2;
			y1 = FormatY_S - y1;
			y2 = FormatY_S - y2;
		}
		else
		{
			x1 = FormatX_S - x1 + FormatX_S0;  //not for vector texts
			x2 = FormatX_S - x2 + FormatX_S0;  //not for vector texts
			
			y1 = FormatY_S + FormatY_S0 - y1;
			y2 = FormatY_S + FormatY_S0 - y2;
		}
	}

	kolor = kolor0;

	
		if (!line_clip_width(&x1, &y1, &x2, &y2, text_line_width)) return TRUE;  //TEMPORARY CONDITIONAL CLIP
	
	if (type__drive == PRN_DRIVE)
		ret_val = Draw_Line_To_Prn1(x1, y1, x2, y2, kolor, text_line_width, bw, grey);
	else
		ret_val = Draw_Line_To_Plt(x1, y1, x2, y2);
	return ret_val;
}

static void get_line_type_ext (int type, double *df_min, double *df_max)
/*----------------------------------------------------------------------*/
{
   int i, n ;
   double df_len_line ;

   *df_min = MAXDOUBLE ;
   *df_max = -1 ;
   n = TypyLinii [type].IloscElementow ;
   for (i = 0 ; i < n ; i++)
   {
     if (0 <= (df_len_line = TypyLinii[type].kreski[i]))
     {
       if (*df_min > df_len_line)
       {
	 *df_min = df_len_line ;
       }
       if (*df_max < df_len_line)
       {
	 *df_max = df_len_line ;
       }
     }
   }
   if (*df_min == 0)
   {
     *df_min = df__width_line ;
   }
   if (*df_max == 0)
   {
     *df_max = df__width_line ;
   }
   else
   if (*df_max < *df_min)
   {
     *df_min = 0 ;
   }
}


    static int 	  last_element;
    static double last_elem_draw;

static void ini_arc (void)
/*------------------------*/
{
  last_element = 0;
  last_elem_draw = 0;
}


static BOOL draw_wzorzec (double x0, double y0, double len,
	double si, double co, int typ, int kolor0, BOOL cont, BOOL draw_end, BOOL bw, BOOL grey)
/*-----------------------------------------------------------------------------------------*/
{
    int ilosc_elementow;
    int kolor;
    double x, y, l_elem;
    BOOL down; 	 /*rysowac kreske czy tylko  przemiescic pisak*/
    double df_len_line ;
    double df_min, df_max ;
    int i;

    kolor=kolor0;

    if (cont == FALSE)
     {
	   last_element = 0;
	   last_elem_draw = 0;
     }
    ilosc_elementow = TypyLinii [typ].IloscElementow;
	if (ilosc_elementow == 0) return TRUE;  //ignored
    while (TRUE)
    {
      for ( i = last_element ; i < ilosc_elementow; i++)
   	{
	    down = TRUE;
       df_len_line = TypyLinii[typ].kreski[i] ;
	    if (df_len_line == 0)
	     {
	      df_len_line = df__width_line ;
	     }
	    if (df_len_line < 0 )
	     {
		   down = FALSE;
	     }
	    l_elem = fabs (df_len_line) - last_elem_draw;
	    if (l_elem <= len)
	     {
  		   x = x0 + l_elem * co;
		   y = y0 + l_elem * si;
		   len -= l_elem;
		   last_element ++;
		   last_elem_draw = 0;
	     }
	     else
	     {
		   df_min = 0 ;
		   df_max = 0 ;
		   if (down == FALSE && draw_end == TRUE)
		    {
		     get_line_type_ext (typ, &df_min, &df_max) ;
		     if (len > df_max)
		      {
		       double df_temp = (df_max + df_min) / 2 ;
		       x0 += (len - df_temp) * co;
		       y0 += (len - df_temp) * si;
		       x = x0 + df_temp * co;
		       y = y0 + df_temp * si;
		      }
            else
             {
              x = x0 + len * co;
		        y = y0 + len * si;
              down = TRUE;
             }
		    }
		     else
		      {
		       x = x0 + len * co;
		       y = y0 + len * si;
		      }
		    last_elem_draw += len;
		    len = 0;
		    if (draw_end) down = TRUE ;
		    if (df_max == -1) down = FALSE ; /*dla wzorca bez kresek dodatnich*/
	     }
	    if (down == TRUE)
	     {
	      if (draw_line_to_drive (x0, y0, x, y, kolor, 0, bw, grey) == FALSE) return FALSE;
	     }
	    if (len == 0)
	     {
	      return TRUE;
	     }
	    x0 = x;
	    y0 = y;
	   }
	  last_element = 0;
    }
}


/*----------draw_arc_drv----------------------------------------*/


static BOOL first ;

static BOOL linen_plt_typ_wzor ( double x1, double y1, double x2, double y2, int typ, int kolor0, BOOL bw, BOOL grey)
/*-----------------------------------------------------------------------------------------------------------------*/
{
  LINIA L;
  PLINIA PL;
  int kolor;

  kolor=kolor0;


  if (first == TRUE)
   {
     ini_arc ();
     first = FALSE;
   }
  L.x1 = x1;	/*gdy dl. linii == 0 bedzie dzielenie przez 0 (sin=sy/dl)*/
  L.y1 = y1;
  L.x2 = x2;
  L.y2 = y2;
  L.kolor = kolor;
  parametry_lini(&L, &PL);
  return  draw_wzorzec (x1, y1, PL.dl, PL.sin, PL.cos, typ, kolor, TRUE, FALSE, bw, grey);
}


static BOOL linen_plt ( double x1, double y1, double x2, double y2, int typ, int kolor0, double text_line_width, BOOL bw, BOOL grey)
/*---------------------------------------------------------------------------------------------------------------------------------*/
{ int kolor;

  kolor=kolor0;

  if (typ == 0)
      return draw_line_to_drive ( x1, y1, x2, y2, kolor, text_line_width, bw, grey);
  else
      return linen_plt_typ_wzor ( x1, y1, x2, y2, typ, kolor, bw, grey);
}

BOOL linen_plt1 ( double x1, double y1, double x2, double y2, int typ, int kolor0, double text_line_width, BOOL bw, BOOL grey)
/*-------------------------------------------------------------------------------------------------------------------------*/
{ int kolor;

  kolor=kolor0;

  if (typ == 0)
      return draw_line_to_drive1 ( x1, y1, x2, y2, kolor, text_line_width, bw, grey);
  else
      return linen_plt_typ_wzor ( x1, y1, x2, y2, typ, kolor, bw, grey);
}

BOOL linen_plt_trans(double x1, double y1, double x2, double y2, int typ, int kolor0, double text_line_width, BOOL bw, BOOL grey)
/*-------------------------------------------------------------------------------------------------------------------------*/
{
    return draw_line_to_drive_trans(x1, y1, x2, y2, kolor0, text_line_width, bw, grey);
}

BOOL linen_plt_pattern(double x1, double y1, double x2, double y2)
/*---------------------------------------------------------------*/
{
	
	return draw_line_to_drive_pattern(x1, y1, x2, y2);
	
}

BOOL linen_plt2(double x1, double y1, double x2, double y2, int typ, int kolor0, double text_line_width, BOOL bw, BOOL grey)
/*-------------------------------------------------------------------------------------------------------------------------*/
{
	int kolor;

	kolor = kolor0;

	if (typ == 0)
		return draw_line_to_drive2(x1, y1, x2, y2, kolor, text_line_width, bw, grey);
	else
		return linen_plt_typ_wzor(x1, y1, x2, y2, typ, kolor, bw, grey);
}


static BOOL draw_arc_drv (double Xr,double Yr,double sta,double ea,double R, int typ, int kolor0, BOOL bw, BOOL grey)
/*-----------------------------------------------------------------------------------------------------------------*/
{
  double Xp1,Yp1,Xp2,Yp2;
  double Xp0,Yp0;
  double da, dda;	/*kat rozwarcia luku, kat podzialu luku*/
  double si, co; 	/*sinus i cosinus kata podzialu luku*/
  long N;		/*liczba 'bokow' luku*/
  long k;
  long Num; 		/*liczba 'bokow' podzialu okregu (zalezy od promienia)*/
  double xdiff, ydiff;	/*pomocnicze*/
  int kolor;
  int x0, x00, y0, y00;
  int r;
  COLOR_ kolorR;
  byte_ intensity;
  int i_width;
  double sta_PDF, ea_PDF;
  int arc_no, arc_no1, arc_no2, arc_no3;
  LUK l_clip0[5]={ldef, ldef,ldef, ldef, ldef};
  LUK l_clip[5]={ldef, ldef,ldef, ldef, ldef};

  T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_;

    if ((typ == 0) && ( type__drive == PRN_DRIVE))
    {
        ptrs__prn_ini_date_ = get_ptrs__prn_ini_date();
        if ((*ptrs__prn_ini_date_)->prn_type == PRN_PDF)
        {
            if (print_inversion == TRUE)
            {
                if (print_rotation == TRUE)
                {
                    //// if reflection
                    Xp0 = FormatX_S - Xr;
                    Yp0 = FormatY_S - Yr;
                    sta += (0.25*Pi2);
                    ea += (0.25*Pi2);
                }
                else
                {
                    Xp0 = FormatX_S - Xr + FormatX_S0;  //this is for reflection
                    Yp0 = FormatY_S + FormatY_S0 - Yr;
                    sta -= (0.50*Pi2);
                    ea -= (0.50*Pi2);
                }
            }
            else
            {
                Xp0=Xr;
                Yp0=Yr;
                if (print_rotation == TRUE) {
                    sta -= (0.25*Pi2);
                    ea -= (0.25*Pi2);
                }
            }

            if (ea<sta) ea+=Pi2;


                    x00 = units_to_prn_x(Xp0, Yp0);
                    y00 = units_to_prn_y(Xp0, Yp0);
                    r = units_y_to_prn(R);

                    kolorR.red = _dac_normal[kolor0][0] << 2;
                    kolorR.gre = _dac_normal[kolor0][1] << 2;
                    kolorR.blu = _dac_normal[kolor0][2] << 2;

                    if (((*ptrs__prn_ini_date_)->color_print == 1) && (bw == 0)) {
                        if (((*ptrs__prn_ini_date_)->gray_print == 1) || (grey == 1)) {
                            intensity = get_grey_intensity(kolorR);
                            kolorR = {intensity, intensity, intensity};
                        }
                    } else {
                        if ((kolorR.red < 254) && (kolorR.gre < 254) && (kolorR.blu < 254))
                            kolorR = {0, 0, 0};
                    }

                    i_width = get_width_line_(TRUE, 0);

                    x0 = x00 + get_pdf_left_margin();
                    y0 = get_pdf_page_height() - y00 - get_pdf_top_margin();

                    if (Check_if_Equal(fabs(sta - ea), Pi2) == TRUE)
                        CirclePDF(x0, y0, r, i_width, kolorR);
                    else {
                        sta_PDF = my_round(360.0 - (sta * 360.0 / Pi2 - 90.0), 4);
                        ea_PDF = my_round(360.0 - (ea * 360.0 / Pi2 - 90.0), 4);
                        //if (Check_if_Equal(sta_PDF, 360)==TRUE) sta_PDF=0;
                        //if (Check_if_Equal(ea_PDF, 360)==TRUE) ea_PDF=0;
                        if (sta_PDF < 0) sta_PDF += 360;
                        else if (sta_PDF > 360) sta_PDF -= 360;
                        if (ea_PDF < 0) ea_PDF += 360;
                        else if (ea_PDF > 360) ea_PDF -= 360;

                        if (Check_if_Equal(ea_PDF, sta_PDF)) return TRUE;

                        if (ea_PDF > sta_PDF) {
                            sta_PDF += 360.0;
                        }

                        if (r<1) return 1;


                        ArcPDF(x0, y0, r, ea_PDF, sta_PDF, i_width, kolorR, HPDF_ROUND_END);

                    }

            return TRUE;

        }
    }

  kolor=kolor0;
  first = TRUE;
  if ( R == 0 )
  {
      R = min__dist ;
  }

  if (TRUE == Check_if_Equal (sta, 0) &&   /*  dla okregu da =  Pi2 */
      TRUE == Check_if_Equal (ea, Pi2))
  {
     sta = 0 ;
     ea = Pi2 - o_male ;
     da = ea ;
  }
  else
  {
    sta = Angle_Normal (sta) ;
    ea = Angle_Normal (ea) ;
    da = ea - sta ;
    if (sta > ea)
    {
      da += Pi2 ;
    }
  }
  Num = Pi2 * R / d__luk;
  if (Num < 12/*9*/) Num = 12/*9*/;  /* ??????*/

  dda = Pi2 / Num ;
  si = sin (dda) ;
  co = cos (dda) ;
  N =  (int)(da / dda);
  Xp1 = Xr + R * cos (sta) ;
  Yp1 = Yr + R * sin (sta) ;
  for(k = 0; k < N; k++)
  {
     xdiff = Xp1 - Xr ;
     ydiff = Yp1 - Yr ;
     Xp2 = Xr + xdiff * co - ydiff * si ;
     Yp2 = Yr + xdiff * si + ydiff * co ;
     if (linen_plt1 /*linen_plt*/ (Xp1, Yp1, Xp2, Yp2, typ, kolor, 0, bw, grey) == FALSE) return FALSE;
     Xp1 = Xp2;
     Yp1 = Yp2;
  }
  Xp2 = Xr + R * cos (ea) ;
  Yp2 = Yr + R * sin (ea) ;
  if (linen_plt1 /*linen_plt*/ (Xp1, Yp1, Xp2, Yp2, typ, kolor, 0, bw, grey) == FALSE) return FALSE;
  return TRUE;
}



static BOOL draw_ellipse_drv(double Xr, double Yr, double Rx, double Ry, double Angle, int typ, int kolor0, BOOL bw, BOOL grey)
{
    double Xp1,Yp1,Xp2,Yp2;
    double Xp0,Yp0;
    double da, dda;	/*kat rozwarcia luku, kat podzialu luku*/
    double si, co; 	/*sinus i cosinus kata podzialu luku*/
    long N;		/*liczba 'bokow' luku*/
    long k;
    long Num; 		/*liczba 'bokow' podzialu okregu (zalezy od promienia)*/
    double xdiff, ydiff;	/*pomocnicze*/
    int kolor;
    int x0, x00, y0, y00;
    int rx, ry, r_;
    COLOR_ kolorR;
    byte_ intensity;
    int i_width;
    double angle_PDF;
    int arc_no, arc_no1, arc_no2, arc_no3;
    double xy[256];
    int numpoints, i;
    LUK l_clip0[5]={ldef, ldef,ldef, ldef, ldef};
    LUK l_clip[5]={ldef, ldef,ldef, ldef, ldef};

    T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_;

    ptrs__prn_ini_date_ = get_ptrs__prn_ini_date();

    if ((typ == 0) && (Check_if_Equal(Angle, 0.0) ||
         Check_if_Equal(Angle, Pi/2.0) ||
         Check_if_Equal(Angle, Pi) ||
         Check_if_Equal(Angle, 3*Pi/2.0) ||
         Check_if_Equal(Rx, Ry) ||
         Check_if_Equal(Angle, Pi2)) && ( type__drive == PRN_DRIVE) && ((*ptrs__prn_ini_date_)->prn_type == PRN_PDF))
    {
        rx = units_y_to_prn(Rx);
        ry = units_y_to_prn(Ry);

        if ((Check_if_Equal(Angle, Pi/2.0)) || (Check_if_Equal(Angle, 3*Pi/2.0)))
        {
            r_=ry;
            ry=rx;
            rx=r_;
        }

        if (print_inversion == TRUE)
        {
            if (print_rotation == TRUE)
            {
                //// if reflection
                Xp0 = FormatX_S - Xr;
                Yp0 = FormatY_S - Yr;
                //Angle += (0.25*Pi2);
                r_=ry;
                ry=rx;
                rx=r_;
            }
            else
            {
                Xp0 = FormatX_S - Xr + FormatX_S0;  //this is for reflection
                Yp0 = FormatY_S + FormatY_S0 - Yr;
            }
        }
        else
        {
            Xp0=Xr;
            Yp0=Yr;
            if (print_rotation == TRUE) {
                r_=ry;
                ry=rx;
                rx=r_;
            }
        }

        x00 = units_to_prn_x(Xp0, Yp0);
        y00 = units_to_prn_y(Xp0, Yp0);

        kolorR.red = _dac_normal[kolor0][0] << 2;
        kolorR.gre = _dac_normal[kolor0][1] << 2;
        kolorR.blu = _dac_normal[kolor0][2] << 2;

        if (((*ptrs__prn_ini_date_)->color_print == 1) && (bw == 0)) {
            if (((*ptrs__prn_ini_date_)->gray_print == 1) || (grey == 1)) {
                intensity = get_grey_intensity(kolorR);
                kolorR = {intensity, intensity, intensity};
            }
        } else {
            if ((kolorR.red < 254) && (kolorR.gre < 254) && (kolorR.blu < 254))
                kolorR = {0, 0, 0};
        }

        i_width = get_width_line_(TRUE, 0);

        x0 = x00 + get_pdf_left_margin();
        y0 = get_pdf_page_height() - y00 - get_pdf_top_margin();

        angle_PDF = my_round(360.0 - (Angle * 360.0 / Pi2 - 90.0), 4);
        if (angle_PDF < 0) angle_PDF += 360;
        else if (angle_PDF > 360) angle_PDF -= 360;

        if ((rx<1) || (ry<1)) return 1;

        EllipsePDF(x0, y0, rx, ry, angle_PDF, i_width, kolorR, HPDF_ROUND_END);

        return TRUE;

    }

    numpoints = Get_Ellipse_Points (Xr, Yr, Rx, Ry, Angle, xy);
    for (i=0; i<(numpoints-2); i+=2)
    {
        if (linen_plt1  (xy[i], xy[i+1], xy[i+2], xy[i+3], typ, kolor0, 0, bw, grey) == FALSE) return FALSE;
    }
    if (linen_plt1  (xy[i], xy[i+1], xy[0], xy[1], typ, kolor0, 0, bw, grey) == FALSE) return FALSE;

    return TRUE;

}

static BOOL draw_filledellipse_drv(double Xr, double Yr, double Rx, double Ry, double Angle, int kolor0, unsigned char translucency, BOOL bw, BOOL grey)
{
    double Xp1,Yp1,Xp2,Yp2;
    double Xp0,Yp0;
    double da, dda;	/*kat rozwarcia luku, kat podzialu luku*/
    double si, co; 	/*sinus i cosinus kata podzialu luku*/
    long N;		/*liczba 'bokow' luku*/
    long k;
    long Num; 		/*liczba 'bokow' podzialu okregu (zalezy od promienia)*/
    double xdiff, ydiff;	/*pomocnicze*/
    int kolor;
    int x0, x00, y0, y00;
    int rx, ry, r_;
    COLOR_ kolorR;
    byte_ intensity;
    int i_width;
    double angle_PDF;
    int arc_no, arc_no1, arc_no2, arc_no3;
    LUK l_clip0[5]={ldef, ldef,ldef, ldef, ldef};
    LUK l_clip[5]={ldef, ldef,ldef, ldef, ldef};
    int kolory_paper, kolor256, r,g,b;
    double xy[256];
    int numpoints;
    int ret;
    int colorB = 0xFFFFFFFF;
    double df_dist ;

    T_PTR_Prn_Ini_Date* ptrs__prn_ini_date_;

    ptrs__prn_ini_date_ = get_ptrs__prn_ini_date();

    if ((Check_if_Equal(Angle, 0.0) ||
        Check_if_Equal(Angle, Pi/2.0) ||
        Check_if_Equal(Angle, Pi) ||
        Check_if_Equal(Angle, 3*Pi/2.0) ||
        Check_if_Equal(Rx, Ry) ||
        Check_if_Equal(Angle, Pi2)) && ( type__drive == PRN_DRIVE) && ((*ptrs__prn_ini_date_)->prn_type == PRN_PDF))
    {
            rx = units_y_to_prn(Rx);
            ry = units_y_to_prn(Ry);

        if ((Check_if_Equal(Angle, Pi/2.0)) || (Check_if_Equal(Angle, 3*Pi/2.0)))
        {
            r_=ry;
            ry=rx;
            rx=r_;
        }

            if (print_inversion == TRUE)
            {
                if (print_rotation == TRUE)
                {
                    //// if reflection
                    Xp0 = FormatX_S - Xr;
                    Yp0 = FormatY_S - Yr;
                    r_=ry;
                    ry=rx;
                    rx=r_;
                }
                else
                {
                    Xp0 = FormatX_S - Xr + FormatX_S0;  //this is for reflection
                    Yp0 = FormatY_S + FormatY_S0 - Yr;
                }
            }
            else
            {
                Xp0=Xr;
                Yp0=Yr;
                if (print_rotation == TRUE) {
                    r_=ry;
                    ry=rx;
                    rx=r_;
                }
            }


            x00 = units_to_prn_x(Xp0, Yp0);
            y00 = units_to_prn_y(Xp0, Yp0);

            kolorR.red = _dac_normal[kolor0][0] << 2;
            kolorR.gre = _dac_normal[kolor0][1] << 2;
            kolorR.blu = _dac_normal[kolor0][2] << 2;

            if (((*ptrs__prn_ini_date_)->color_print == 1) && (bw == 0)) {
                if (((*ptrs__prn_ini_date_)->gray_print == 1) || (grey == 1)) {
                    intensity = get_grey_intensity(kolorR);
                    kolorR = {intensity, intensity, intensity};
                }
            } else {
                if ((kolorR.red < 254) && (kolorR.gre < 254) && (kolorR.blu < 254))
                    kolorR = {0, 0, 0};
            }

            i_width = get_width_line_(TRUE, 0);

            x0 = x00 + get_pdf_left_margin();
            y0 = get_pdf_page_height() - y00 - get_pdf_top_margin();

            angle_PDF = my_round(360.0 - (Angle * 360.0 / Pi2 - 90.0), 4);
            if (angle_PDF < 0) angle_PDF += 360;
            else if (angle_PDF > 360) angle_PDF -= 360;


            if ((rx<1) || (ry<1)) return 1;


            FilledEllipsePDF(x0, y0, rx, ry, angle_PDF, kolorR, HPDF_ROUND_END, translucency);

            return TRUE;

    }

    if (type__drive == PRN_DRIVE)
    {
        ptrs__prn_ini_date_ = get_ptrs__prn_ini_date();
        if ((*ptrs__prn_ini_date_)->background == TRUE)
        {
            kolor256 = GetColorAC(kolory.paper);

            r = _dac_normal[kolor256][0] << 2;
            g = _dac_normal[kolor256][1] << 2;
            b = _dac_normal[kolor256][2] << 2;

            colorB = makeacol32(r, g, b, 255);
        }
        else colorB = 0xFFFFFFFF;


        numpoints = Get_Ellipse_Points (Xr, Yr, Rx, Ry, Angle, xy);

        ret = draw_polygon_to_drive_trans(xy, numpoints, /*df_dist,*/ kolor0, bw, grey, translucency, colorB);

    }
    else
    {
        int i;
        colorB = 0xFFFFFFFF;

        numpoints = Get_Ellipse_Points(Xr, Yr, Rx, Ry, Angle, xy);

        for (i = 0; i < (numpoints - 2); i += 2) {
            if (linen_plt1(xy[i], xy[i + 1], xy[i + 2], xy[i + 3], 0, kolor0, 0, bw, grey) == FALSE) return FALSE;
        }
        if (linen_plt1(xy[numpoints - 2], xy[numpoints - 1], xy[0], xy[1], 0, kolor0, 0, bw, grey) == FALSE) return FALSE;
    }

    return TRUE;

}

static BOOL draw_ellipticalarc_drv(double Xr, double Yr, double Rx, double Ry, double Angle, double kat1, double kat2, int typ, int kolor0, BOOL bw, BOOL grey)
{
    double xy[256];
    int numpoints, i;

    numpoints = Get_EllipticalArc_Points (Xr, Yr, Rx, Ry, Angle, kat1, kat2, xy);
    for (i=0; i<(numpoints-2); i+=2)
    {
        if (linen_plt1  (xy[i], xy[i+1], xy[i+2], xy[i+3], typ, kolor0, 0, bw, grey) == FALSE) return FALSE;
    }

    return TRUE;

}

/*------------------------------------------------------------*/
#define PORTNUM 0    /* port number for LPT1 */
#define PRINTER_WRITE	0	/* send abyte to printer */
#define PRINTER_INIT	1	/* initialize printer */
#define PRINTER_STATUS	2	/* read printer status */

static int i__port_num = PORTNUM;
static int i__f ;     //handler zbioru
static BOOL b__to_file = TRUE ;

BOOL Ini_Output_Device (char *file_name, BOOL to_file, int port_num, BOOL text_mode)
/*--------------------------------------------------------------------------------*/
{
  BOOL retval;

  b__to_file = to_file ;
  i__port_num = port_num - 1 ;
  if (to_file == TRUE)
  {
    retval = TRUE;
#ifndef LINUX
      i__f=open(file_name, O_CREAT|O_TRUNC|O_BINARY|O_WRONLY,S_IWRITE);
#else
      i__f=open(file_name, O_CREAT|O_TRUNC|O_RDWR, 0666); //S_IRWXU | S_IRWXG | S_IRWXO);
#endif
    if (i__f == -1)
     {
       retval = FALSE;
       ErrList ( 31 );
     }
  }
  else
  {
    retval = get_prn_status (text_mode);
    if (retval == FALSE)
    {
       ErrList (41);
    }  
  }
  return retval;
}


int Write_To_Device (void *buf, unsigned int len)
/*----------------------------------------------*/
{
  int retval=0, i, status;

  if (b__to_file == TRUE)
  {
    if ( len != (retval = write (i__f, buf, len)))
    {
     ErrList (8);
    }
  }
#ifndef LINUX
  else
  {
    for ( i = 0; i < (int)len; i++)
    {
      status = biosprint(PRINTER_WRITE, (int)(((char*)buf) [i]), i__port_num);
      retval = prn_ok (status);
      if (retval == FALSE)
      {
	if (FALSE == get_prn_status (text_mode1))
	{
	  ErrList (41);
	  break;
	}
	else
	{
	 i--;
	}
      }
    }
    retval = i;
  }
#endif
  return retval;
}


int Close_Output_Device (void)
/*----------------------------*/
{
  int retval = TRUE;

  if (b__to_file == TRUE)
  {
    retval = close (i__f) ;
  }
  return retval ;
}



static BOOL prn_ok (int status)
/*----------------------------*/
{
  BOOL retval = TRUE;

  if (
       status & 0x01   /*time out error */
    || status & 0x08   /*I/O error*/
    || status & 0x20   /*Out of paper*/
     )
  {
    retval = FALSE;
  }
  return retval;


#ifdef aaa
    || status & 0x10   /*Selected*/
    || status & 0x40   /*Acknowledge*/
    || status & 0x80   /*Printer is ready, ( not busy) */
#endif
}

#ifndef LINUX
static BOOL get_prn_status (BOOL text_mode)
/*---------------------------------------*/
{
   int status, abyte=0, key;
   BOOL retval ;

   while (1)
   {
     status = biosprint(PRINTER_STATUS, abyte, i__port_num);
     retval = prn_ok (status);
     if (retval == FALSE)
     {
       ErrList (42);
       key = Get_Legal_Key ("kKpP");
       ClearErr ();
       if (key == 'p' || key == 'P')
       {
	 break;
       }
     }
     else
     {
       komunikat(46);
       break;
     }
   }
   return retval;
}
#else
static BOOL get_prn_status (BOOL text_mode)
/*---------------------------------------*/
{
    return TRUE;
}
#endif
}  //extern "C"

#pragma optimize("",on)
