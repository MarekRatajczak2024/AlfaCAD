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

#define __O_HATCH3__
#include "forwin.h"
#define ALLEGWIN
#include "allegext.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef LINUX
#include <dos.h>
#endif
#include <math.h>
#include "bib_e.h"
#include "bib_blok.h"
#include "b_menu.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "o_osnap.h"
#include "o_object.h"
#include "b_buf.h"
#include "o_hatch.h"
#include "b_messag.h"
#include "b_libfun.h"
#include "o_measur.h"

#include "message.h"

#include "leak_detector_cpp.hpp"

extern BOOL Add_String_To_List1 (char *ptr_string) ;
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

#define HATCHOUTLINE

#define BIGNOD 1e200
#define BIGNOF 1e20

#define TRAPSNO_MAX 10000

extern "C" {
extern T_PTR_Hatch_Def_Param Get_Hatch_Def_Table(void);
extern T_PTR_Hatch_Line_Def Get_Hatch_Line_Def_Table(void);
extern int i__hatch_pattern_no;
extern double insulation_hatch_scale[2];
extern double insulation_hatch_scale_div[2];
extern int insulation_hatch_no;

extern int my_kbhit(void);
extern int my_getch(void);
extern char *get_units(void);

extern void line_place (LINIA *l, double *xmin, double *ymin, double *xmax, double *ymax);
extern void okrag_place (OKRAG *o, double *xmin, double *ymin, double *xmax, double *ymax);
extern void luk_place (LUK *l, double *xmin, double *ymin, double *xmax, double *ymax);

BOOL Make_Hatch(T_PTR_Hatch_Param ptrs_hatch_param0,
                T_PTR_Area ptrs_area, int comput_area,
                double df_apx1, double df_apy1, double df_apx2, double df_apy2, double df_pointx, double df_pointy,
                /*void *abd */ long_long le_abd, int to_block = 0, int draw_line=TRUE);
}

extern long_long global_block;

extern T_PTR_Hatch_Def_Param Get_Hatch_Def_Table999 (void);
extern T_PTR_Hatch_Line_Def Get_Hatch_Line_Def_Table999 (void);

static int comput_area1 ;
static int i__trapsno, i__trapsno_max ;	/*liczba aktywnych trapezow*/
static T_PTR_Trapizium ptrs__traps ;

static T_PTR_Area ptrs__area0 ;	 	/*adres tablicy obszarow*/
static int *ptri__tab_ver_no;
static double pole_hatch ;
static double pole_hatch1 ;
static double mom_bezwl_x ;
static double mom_bezwl_y ;
static double wskazn_wytrz_x ;
static double wskazn_wytrz_y ;
static double min_xw, max_xw, min_yw, max_yw ;
static double mom_bezwl;
static double mom_dewiacji ;
static double moment ;
static double mom_stat_x;
static double mom_stat_y;
static double df_x11m ;
static double df_x22m ;
static int pole_hatch_add ;
static double df_dist_ ;

static double xmin_g = MAXDOUBLE ;
static double xmax_g = -MAXDOUBLE ;
static double ymin_g = MAXDOUBLE ;
static double ymax_g = -MAXDOUBLE ;

double g_dfymin;
double g_dfymax;
double g_dfxmin;
double g_dfxmax;

static double df_pointx_gl;
static double df_pointy_gl;

BLOK *global_dane=NULL;
char *global_dane_ob;
long_long global_dist;

/*wszystkie dzialania wykonuje sie w jednostkach rysunku*/

static BOOL hatch_trap (T_PTR_Trapizium,T_PTR_Hatch_Param,T_PTR_Hatch_Line_Def, T_PTR_Trapizium, int) ;
static BOOL add_trap (T_PTR_Trapizium) ;
static char *add_block(BOOL hatchoutline, BLOK *adb, int atrybut) ;
char *add_block_hatch (BOOL hatchoutline) ;
char *add_block_hatch_in_block (BOOL hatchoutline) ;
static void out_pole (double p0) ;
static void ret_pole (double p0) ;
static void out_srodek_ciezkosci (double x_s, double y_s);
static void out_moment_statyczny (double mom_stat);
static void out_moment_bezwladnosci (double mom_bezwl);
static void out_centr_moment_bezwl (double mom_bezwl_x, double mom_bezwl_y, double wskazn_wytrz_x, double wskazn_wytrz_y);
static void  nooop(void)
{}
#define noop (void ( *)(int x,int y))nooop

int Get_Key (void)
/*---------------*/
{
   int ret ;
   void (*CUR)(int ,int);

   CUR=MVCUR;
   MVCUR=noop;
   ret = getkey ();
   MVCUR=CUR;
   return ret;
}

static BOOL Add_Point1 (double df_x, double df_y)
/*---------------------------------------*/
{
  BOOL b_ret ;

  PointG.x = df_x ;
  PointG.y = df_y ;
  b_ret = FALSE ;
  if (NULL != dodaj_obiekt (NULL, (void*)&PointG))
  {
    b_ret = TRUE ;
    rysuj_obiekt ((char*)&PointG, COPY_PUT, 1) ;
  }
  return b_ret ;
}

static BOOL Add_Line1(double df_x1, double df_y1, double df_x2, double df_y2, double df_basex, double df_basey, double df_angle, int draw_line)
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
{
	BOOL b_ret;
	double df_si, df_co;
	unsigned char LiniaGtyp;
	
	LiniaGtyp = LiniaG.typ;

	LiniaG.typ = HATCH_OUTLINE_TYPE;

	df_si = sin(df_angle);
	df_co = cos(df_angle);
	
	LiniaG.x1 = df_x1;
	LiniaG.y1 = df_y1;
	LiniaG.x2 = df_x2;
	LiniaG.y2 = df_y2;

	LiniaG.blok = 1;

	Rotate_Line((void *)&LiniaG, df_basex, df_basey, df_si, df_co);

	b_ret = FALSE;
    PTR__GTMP6=(char*)global_dane;
    if (NULL != dodaj_obiekt(global_dane, (void*)&LiniaG))
	{
		b_ret = TRUE;
		if (draw_line) rysuj_obiekt((char*)&LiniaG, COPY_PUT, 1);
	}
    global_dane=(BLOK*)PTR__GTMP6;

	LiniaG.typ = LiniaGtyp;

	return b_ret;
}


static BOOL Add_Arc1(double x, double y, double r, double a1, double a2, double df_basex, double df_basey, double df_angle, int draw_line)
/*--------------------------------------------------------------------------------------------------------------------------------------*/
{
	BOOL b_ret;
	double df_si, df_co;
	unsigned char LukGtyp;

	LukGtyp = LukG.typ;

	LukG.typ = HATCH_OUTLINE_TYPE;

	df_si = sin(df_angle);
	df_co = cos(df_angle);

	LukG.x = x;
	LukG.y = y;
	LukG.r = r;
	LukG.kat1 = a1;
	LukG.kat2 = a2;

    //overlapping the arc
    if (LukG.kat1<LukG.kat2)
    {
        LukG.kat1= Angle_Normal(LukG.kat1-0.005);
        LukG.kat2= Angle_Normal(LukG.kat2+0.005);
    }
    else
    {
        LukG.kat2+=Pi2;
        LukG.kat1= Angle_Normal(LukG.kat1-0.005);
        LukG.kat2= Angle_Normal(LukG.kat2+0.005);
    }

	LukG.blok = 1;

	Rotate_Arc((void *)&LukG, df_basex, df_basey, df_si, df_co);

	b_ret = FALSE;
    PTR__GTMP6=(char*)global_dane;
    if (NULL != dodaj_obiekt(global_dane, (void*)&LukG))
	{
		b_ret = TRUE;
		if (draw_line) rysuj_obiekt((char*)&LukG, COPY_PUT, 1);
	}
    global_dane=(BLOK*)PTR__GTMP6;
	LukG.typ = LukGtyp;

	return b_ret;
}


static BOOL Add_Circle1(double x, double y, double r, double df_basex, double df_basey, double df_angle, int draw_line)
/*-------------------------------------------------------------------------------------------------------------------*/
{
	BOOL b_ret;
	double df_si, df_co;
	unsigned char OkragGtyp;
	double xs, ys;

	OkragGtyp = OkragG.typ;

    OkragG.typ = HATCH_OUTLINE_TYPE;

	df_si = sin(df_angle);
	df_co = cos(df_angle);

	OkragG.r = r;
	
	OkragG.blok = 1;

	Rotate_Point(df_si, df_co, df_basex, df_basey, x, y, &xs, &ys);
	OkragG.x = xs;
	OkragG.y = ys;

	b_ret = FALSE;
    PTR__GTMP6=(char*)global_dane;
    if (NULL != dodaj_obiekt(global_dane, (void*)&OkragG))
	{
		b_ret = TRUE;
		if (draw_line) rysuj_obiekt((char*)&OkragG, COPY_PUT, 1);
	}
    global_dane=(BLOK*)PTR__GTMP6;
	OkragG.typ = OkragGtyp;

	return b_ret;
}


static double atan2_my  (double y, double x)
{
  if (fabs(y) < 1.0E-10 && fabs (x) < 1.0E-10)
  {
   Internal_Error (__LINE__,__FILE__) ;
   return 0 ;
  }
  return atan2 (y, x) ;
}


static T_PTR_Vertex get_vertex_ (T_PTR_Area ptrs_area, int i_vertex_no)
//-----------------------------------------------
{
  return &ptrs_area->s_vertexs_alloc.ptrs_vertexs [i_vertex_no] ;
}


static T_PTR_Vertex get_vertex (int i_vertex_no)
/*-----------------------------------------------*/
{
  int i_ver_no ;

  if ((i_vertex_no < 0) || (i_vertex_no>100000))
	  return NULL;

	i_ver_no = ptri__tab_ver_no[i_vertex_no];
	return get_vertex_(ptrs__area0, i_ver_no);
 
}


static void
rotation (double 		df_angle,
	  T_PTR_Hatch_Param ptrs_hatch_param)
/*-------------------------------------------------*/
{
   T_PTR_AArc ptrs_arcs ;
   T_PTR_Vertex ptrs_vertexs ;
   int i_arcsno, i ;
   int i_vertexes ;
   double df_si, df_co ;
   LUK s_arc ;

   df_si = sin (df_angle) ;
   df_co = cos (df_angle) ;
   ptrs_vertexs = ptrs__area0->s_vertexs_alloc.ptrs_vertexs;
   ptrs_arcs = ptrs__area0->s_arcs_cpy_alloc.ptrs_arcs ;
   i_vertexes = ptrs__area0->s_vertexs_alloc.i_size ;
   i_arcsno = ptrs__area0->s_arcs_cpy_alloc.i_size ;
   for (i = 0 ; i < i_vertexes; i++) 	/*dla kazdego wierzcholka*/
   {
     Rotate_Point (df_si, df_co,
		   ptrs_hatch_param->df_basex,
		   ptrs_hatch_param->df_basey,
		ptrs_vertexs [i].x, ptrs_vertexs [i].y,
		&ptrs_vertexs [i].x, &ptrs_vertexs [i].y) ;	/*rotate point*/
   }
   for (i = 0 ; i < i_arcsno ; i++) 	/*dla kazdego luku*/
   {
     s_arc.r = ptrs_arcs [i].r ;
     s_arc.x = ptrs_arcs [i].x ;
     s_arc.y = ptrs_arcs [i].y ;
     s_arc.kat1 = ptrs_arcs [i].a1 ;
     s_arc.kat2 = ptrs_arcs [i].a2 ;
     Rotate_Arc ((void *)&s_arc,
		  ptrs_hatch_param->df_basex,
		  ptrs_hatch_param->df_basey,
		  df_si, df_co) ;
     ptrs_arcs [i].r = s_arc.r ;
     ptrs_arcs [i].x = s_arc.x ;
     ptrs_arcs [i].y = s_arc.y ;
     ptrs_arcs [i].a1 = s_arc.kat1 ;
     ptrs_arcs [i].a2 = s_arc.kat2 ;
   }
}

static int fcmp_sort_vertex (const void *val1, const void *val2)
/*-------------------------------------------------------*/
{
  T_PTR_Vertex ptrs_vertex1, ptrs_vertex2 ;

  ptrs_vertex1 = (T_PTR_Vertex)val1 ;
  ptrs_vertex2 = (T_PTR_Vertex)val2 ;

  if (TRUE == Check_if_LT (ptrs_vertex1->y, ptrs_vertex2->y))
    return -1 ;
  else
  if (TRUE == Check_if_GT (ptrs_vertex1->y, ptrs_vertex2->y))
    return 1 ;
  else
  if (TRUE == Check_if_LT (ptrs_vertex1->x, ptrs_vertex2->x))
    return -1 ;
  else
  if (TRUE == Check_if_GT (ptrs_vertex1->x, ptrs_vertex2->x))
    return 1 ;
  else
  {
    if (ptrs_vertex1->vertex_no != ptrs_vertex2->vertex_no &&
	TRUE == ptrs_vertex1->b_multi &&
	TRUE == ptrs_vertex2->b_multi)
    {
      ptrs_vertex1->b_single = FALSE ;
      ptrs_vertex2->b_single = FALSE ;
	}
	return 0;
  }
}

/////////////////////
typedef struct tagXY
{
    double X, Y;
} XY;

double Magnitude( XY *Point1, XY *Point2 )
{
    XY Vector;

    Vector.X = Point2->X - Point1->X;
    Vector.Y = Point2->Y - Point1->Y;

    return (double)sqrt( Vector.X * Vector.X + Vector.Y * Vector.Y);
}

int DistancePointLine( XY *Point, XY *LineStart, XY *LineEnd, double *Distance )
{
    double LineMag;
    double U;
    XY Intersection;

    LineMag = Magnitude( LineEnd, LineStart );

    U = ( ( ( Point->X - LineStart->X ) * ( LineEnd->X - LineStart->X ) ) +
          ( ( Point->Y - LineStart->Y ) * ( LineEnd->Y - LineStart->Y ) ) ) /
        ( LineMag * LineMag );

    Intersection.X = LineStart->X + U * ( LineEnd->X - LineStart->X );
    Intersection.Y = LineStart->Y + U * ( LineEnd->Y - LineStart->Y );

    *Distance = Magnitude( Point, &Intersection );

    return 1;
}
//////////
void line_angle (XY *xyl1, XY *xyl2, double *angle)
/*-----------------------------------------------*/
{
    double dx,dy,dl;

    dx = xyl2->X - xyl1->X;
    dy = xyl2->Y - xyl1->Y;
    if (TRUE == Check_if_Equal (dy, 0))
    {
        *angle=(dx>=0.0 ? 0.0 : PI);
    }
    else
    if (TRUE == Check_if_Equal (dx, 0))
    {
        *angle=(dy>=0.0 ? PI2 : PI2*3);
    }
    else
    {
        dl=sqrt(dx*dx+dy*dy);
        *angle=Atan2(dy,dx);
        if (*angle<0.0) *angle+=(2*PI);
    }
}


/////////
static void get_draw_outline(T_PTR_Hatch_Param ptrs_hatch_param, BOOL draw, int draw_line)
{
	T_PTR_Vertex ptrs_vertexs;
	int i_verno, i; // j;

    T_PTR_ALine ptrs_lines;
	int vertex1_no, vertex2_no;
	int i_linno;

	T_PTR_AArc ptrs_arcs;
	double arcr, arca1, arca2, arcx, arcy;	/*parametry luku*/
	int i_arcno;

    double df_basex = ptrs_hatch_param->df_basex;
    double df_basey = ptrs_hatch_param->df_basey;
    double df_angle = 0.0;
    double l_max=0.0;
    double dx,dy,dl;
    int longest_line_i;
    XY xyp, xyl1, xyl2;
    double d_max=0.0;
    double distance;
    int ret;

	i_verno = ptrs__area0->s_vertexs_alloc.i_size;
	ptrs_vertexs = (T_PTR_Vertex)malloc(sizeof(T_Vertex)*i_verno);
	memmove(ptrs_vertexs, ptrs__area0->s_vertexs_alloc.ptrs_vertexs, sizeof(T_Vertex)*i_verno);

	i_linno = ptrs__area0->s_lines_alloc.i_size;
	ptrs_lines = (T_PTR_ALine)malloc(sizeof(T_ALine)*i_linno);
	memmove(ptrs_lines, ptrs__area0->s_lines_alloc.ptrs_lines, sizeof(T_ALine)*i_linno);

	i_arcno = ptrs__area0->s_arcs_alloc.i_size;
	ptrs_arcs = (T_PTR_AArc)malloc(sizeof(T_AArc)*i_arcno);
	memmove(ptrs_arcs, ptrs__area0->s_arcs_alloc.ptrs_arcs, sizeof(T_AArc)*i_arcno);

    if (draw==TRUE) {

        for (i = 0; i < i_linno; i++) {
            vertex1_no = ptrs_lines[i].vertex1_no;
            vertex2_no = ptrs_lines[i].vertex2_no;
            Add_Line1(ptrs_vertexs[vertex1_no].x, ptrs_vertexs[vertex1_no].y, ptrs_vertexs[vertex2_no].x,
                      ptrs_vertexs[vertex2_no].y, df_basex, df_basey, df_angle, draw_line);
        }

        for (i = 0; i < i_arcno; i++) {
            vertex1_no = ptrs_arcs[i].vertex1_no;
            vertex2_no = ptrs_arcs[i].vertex2_no;
            arcr = ptrs_arcs[i].r;
            arca1 = ptrs_arcs[i].a1;
            arca2 = ptrs_arcs[i].a2;
            arcx = ptrs_arcs[i].x;
            arcy = ptrs_arcs[i].y;

            //checking if maybe circle
            if ((i_arcno - i) >= 3) {
                if ((TRUE == Check_if_Equal(ptrs_arcs[i].a1, 0.0)) && (TRUE == Check_if_Equal(ptrs_arcs[i].a2, PI2)) &&
                    (TRUE == Check_if_Equal(ptrs_arcs[i + 1].a1, PI2)) &&
                    (TRUE == Check_if_Equal(ptrs_arcs[i + 1].a2, PI)) &&
                    (TRUE == Check_if_Equal(ptrs_arcs[i + 2].a1, PI)) &&
                    (TRUE == Check_if_Equal(ptrs_arcs[i + 2].a2, 0.0)) &&
                    (TRUE == Check_if_Equal(ptrs_arcs[i].x, ptrs_arcs[i + 1].x)) &&
                    (TRUE == Check_if_Equal(ptrs_arcs[i + 1].x, ptrs_arcs[i + 2].x)) &&
                    (TRUE == Check_if_Equal(ptrs_arcs[i].y, ptrs_arcs[i + 1].y)) &&
                    (TRUE == Check_if_Equal(ptrs_arcs[i + 1].y, ptrs_arcs[i + 2].y)) &&
                    (TRUE == Check_if_Equal(ptrs_arcs[i].r, ptrs_arcs[i + 1].r)) &&
                    (TRUE == Check_if_Equal(ptrs_arcs[i + 1].r, ptrs_arcs[i + 2].r))) {
                    Add_Circle1(arcx, arcy, arcr, df_basex, df_basey, df_angle, draw_line);
                    i += 2;
                }
                //i += 2;
                else Add_Arc1(arcx, arcy, arcr, arca1, arca2, df_basex, df_basey, df_angle, draw_line);
            } else {
                Add_Arc1(arcx, arcy, arcr, arca1, arca2, df_basex, df_basey, df_angle, draw_line);
            }
        }
    }

    if (ptrs_hatch_param->insulation_hatch==TRUE)  //determining the scale, angle, df_basex, df_basey
    {
        //searching for longest line
        if (i_linno>0) {  //minimum line straight line
            for (i = 0; i < i_linno; i++) {
                vertex1_no = ptrs_lines[i].vertex1_no;
                vertex2_no = ptrs_lines[i].vertex2_no;
                dx = ptrs_vertexs[vertex2_no].x - ptrs_vertexs[vertex1_no].x;
                dy = ptrs_vertexs[vertex2_no].y - ptrs_vertexs[vertex1_no].y;
                dl=sqrt(dx*dx+dy*dy);
                if (dl>l_max)
                {
                    longest_line_i=i;
                    l_max=dl;
                }
            }
            if (l_max>0.0)  //searching for point with longest distance from longest line
            {
                vertex1_no = ptrs_lines[longest_line_i].vertex1_no;
                vertex2_no = ptrs_lines[longest_line_i].vertex2_no;
                xyl1.X=ptrs_vertexs[vertex1_no].x;
                xyl1.Y=ptrs_vertexs[vertex1_no].y;
                xyl2.X=ptrs_vertexs[vertex2_no].x;
                xyl2.Y=ptrs_vertexs[vertex2_no].y;

                for (i = 0; i < i_linno; i++) {
                    if (i!=longest_line_i) {
                        vertex1_no = ptrs_lines[i].vertex1_no;
                        vertex2_no = ptrs_lines[i].vertex2_no;

                        xyp.X=ptrs_vertexs[vertex1_no].x;
                        xyp.Y=ptrs_vertexs[vertex1_no].y;
                        ret = DistancePointLine( &xyp, &xyl1, &xyl2, &distance);
                        if (distance>d_max) d_max = distance;

                        xyp.X=ptrs_vertexs[vertex2_no].x;
                        xyp.Y=ptrs_vertexs[vertex2_no].y;
                        ret = DistancePointLine( &xyp, &xyl1, &xyl2, &distance);
                        if (distance>d_max) d_max = distance;
                    }
                }

                if (d_max>0.0)
                {
                    vertex1_no = ptrs_lines[longest_line_i].vertex1_no;
                    vertex2_no = ptrs_lines[longest_line_i].vertex2_no;
                    xyl1.X=ptrs_vertexs[vertex1_no].x;
                    xyl1.Y=ptrs_vertexs[vertex1_no].y;
                    xyl2.X=ptrs_vertexs[vertex2_no].x;
                    xyl2.Y=ptrs_vertexs[vertex2_no].y;

                    //point origin
                    ptrs_hatch_param->df_basex=ptrs_vertexs[vertex1_no].x;
                    ptrs_hatch_param->df_basey=ptrs_vertexs[vertex1_no].y;
                    //angle
                    line_angle(&xyl1, &xyl2, &ptrs_hatch_param->df_angle);
                    //scale
                    ptrs_hatch_param->df_scale=d_max/insulation_hatch_scale[insulation_hatch_no]/insulation_hatch_scale_div[insulation_hatch_no];   //1.0; 100.0;
                }
            }
        }
    }

	free(ptrs_vertexs);
	free(ptrs_lines);
	free(ptrs_arcs);
}


static void get_range_outline(T_PTR_Hatch_Param ptrs_hatch_param)
{
    T_PTR_Vertex ptrs_vertexs;
    int i_verno, i; // j;

    T_PTR_ALine ptrs_lines;
    int vertex1_no, vertex2_no;
    int i_linno;

    T_PTR_AArc ptrs_arcs;
    double arcr, arca1, arca2, arcx, arcy;	/*parametry luku*/
    int i_arcno;

    double df_basex = ptrs_hatch_param->df_basex;
    double df_basey = ptrs_hatch_param->df_basey;
    double df_angle = 0.0;
    double l_max=0.0;
    double dx,dy,dl;
    int longest_line_i;
    XY xyp, xyl1, xyl2;
    double d_max=0.0;
    double distance;
    int ret;

    xmin_g = MAXDOUBLE ;
    xmax_g = -MAXDOUBLE ;
    ymin_g = MAXDOUBLE ;
    ymax_g = -MAXDOUBLE ;

    LINIA L=Ldef;
    OKRAG o=Odef;
    LUK l=ldef;

    i_verno = ptrs__area0->s_vertexs_alloc.i_size;
    ptrs_vertexs = (T_PTR_Vertex)malloc(sizeof(T_Vertex)*i_verno);
    memmove(ptrs_vertexs, ptrs__area0->s_vertexs_alloc.ptrs_vertexs, sizeof(T_Vertex)*i_verno);

    i_linno = ptrs__area0->s_lines_alloc.i_size;
    ptrs_lines = (T_PTR_ALine)malloc(sizeof(T_ALine)*i_linno);
    memmove(ptrs_lines, ptrs__area0->s_lines_alloc.ptrs_lines, sizeof(T_ALine)*i_linno);

    i_arcno = ptrs__area0->s_arcs_alloc.i_size;
    ptrs_arcs = (T_PTR_AArc)malloc(sizeof(T_AArc)*i_arcno);
    memmove(ptrs_arcs, ptrs__area0->s_arcs_alloc.ptrs_arcs, sizeof(T_AArc)*i_arcno);

    for (i = 0; i < i_linno; i++) {
        vertex1_no = ptrs_lines[i].vertex1_no;
        vertex2_no = ptrs_lines[i].vertex2_no;
        //Add_Line1(ptrs_vertexs[vertex1_no].x, ptrs_vertexs[vertex1_no].y, ptrs_vertexs[vertex2_no].x,
        //          ptrs_vertexs[vertex2_no].y, df_basex, df_basey, df_angle, draw_line);
        L.x1=ptrs_vertexs[vertex1_no].x;
        L.y1=ptrs_vertexs[vertex1_no].y;
        L.x2=ptrs_vertexs[vertex2_no].x;
        L.y2=ptrs_vertexs[vertex2_no].y;
        line_place (&L, &xmin_g, &ymin_g, &xmax_g, &ymax_g) ;
    }

    for (i = 0; i < i_arcno; i++) {
        vertex1_no = ptrs_arcs[i].vertex1_no;
        vertex2_no = ptrs_arcs[i].vertex2_no;
        arcr = ptrs_arcs[i].r;
        arca1 = ptrs_arcs[i].a1;
        arca2 = ptrs_arcs[i].a2;
        arcx = ptrs_arcs[i].x;
        arcy = ptrs_arcs[i].y;

        //checking if maybe circle
        if ((i_arcno - i) >= 3) {
            if ((TRUE == Check_if_Equal(ptrs_arcs[i].a1, 0.0)) && (TRUE == Check_if_Equal(ptrs_arcs[i].a2, PI2)) &&
                (TRUE == Check_if_Equal(ptrs_arcs[i + 1].a1, PI2)) &&
                (TRUE == Check_if_Equal(ptrs_arcs[i + 1].a2, PI)) &&
                (TRUE == Check_if_Equal(ptrs_arcs[i + 2].a1, PI)) &&
                (TRUE == Check_if_Equal(ptrs_arcs[i + 2].a2, 0.0)) &&
                (TRUE == Check_if_Equal(ptrs_arcs[i].x, ptrs_arcs[i + 1].x)) &&
                (TRUE == Check_if_Equal(ptrs_arcs[i + 1].x, ptrs_arcs[i + 2].x)) &&
                (TRUE == Check_if_Equal(ptrs_arcs[i].y, ptrs_arcs[i + 1].y)) &&
                (TRUE == Check_if_Equal(ptrs_arcs[i + 1].y, ptrs_arcs[i + 2].y)) &&
                (TRUE == Check_if_Equal(ptrs_arcs[i].r, ptrs_arcs[i + 1].r)) &&
                (TRUE == Check_if_Equal(ptrs_arcs[i + 1].r, ptrs_arcs[i + 2].r))) {
                //Add_Circle1(arcx, arcy, arcr, df_basex, df_basey, df_angle, draw_line);
                o.x=arcx;
                o.y=arcy;
                o.r=arcr;
                okrag_place (&o, &xmin_g, &ymin_g, &xmax_g, &ymax_g);
                i += 2;
            }
                //i += 2;
            else {
                //Add_Arc1(arcx, arcy, arcr, arca1, arca2, df_basex, df_basey, df_angle, draw_line);
                l.x=arcx;
                l.y=arcy;
                l.r=arcr;
                l.kat1=arca1;
                l.kat2=arca2;
                luk_place (&l, &xmin_g, &ymin_g, &xmax_g, &ymax_g);
            }
        } else
        {
            //Add_Arc1(arcx, arcy, arcr, arca1, arca2, df_basex, df_basey, df_angle, draw_line);
            l.x=arcx;
            l.y=arcy;
            l.r=arcr;
            l.kat1=arca1;
            l.kat2=arca2;
            luk_place (&l, &xmin_g, &ymin_g, &xmax_g, &ymax_g);
        }
    }

    free(ptrs_vertexs);
    free(ptrs_lines);
    free(ptrs_arcs);
}


static void sort_vertexs(void)
/*----------------------------*/
{
	T_PTR_Vertex ptrs_vertexs;
	int i_verno, i, j;
	double df_x1, df_x2, df_y1, df_y2;


	ptrs_vertexs = ptrs__area0->s_vertexs_alloc.ptrs_vertexs;
	i_verno = ptrs__area0->s_vertexs_alloc.i_size;
    
	for (i = 0; i < i_verno; i++)
	{
		ptrs_vertexs[i].b_single = TRUE;
		ptrs_vertexs[i].b_equel = FALSE;
	}
	qsort(ptrs_vertexs, i_verno, ptrs__area0->s_vertexs_alloc.i_unit_size, fcmp_sort_vertex);
	for (i = 0; i < i_verno; i++)
	{
		ptri__tab_ver_no[ptrs_vertexs[i].vertex_no] = i;
	}
	for (i = 0; i < i_verno; i++)
	{
		if (ptrs_vertexs[i].b_single == TRUE)
		{
			continue;
		}
		df_x1 = ptrs_vertexs[i].x;
		df_y1 = ptrs_vertexs[i].y;
		for (j = i + 1; j < i_verno; j++)
		{
			if (ptrs_vertexs[j].b_single == TRUE)
			{
				break;
			}
			df_x2 = ptrs_vertexs[j].x;
			df_y2 = ptrs_vertexs[j].y;
			if (TRUE == Check_if_Equal(df_x1, df_x2) &&
				TRUE == Check_if_Equal(df_y1, df_y2))
			{
				ptrs_vertexs[j].b_equel = TRUE;
			}
			else
			{
				break;
			}
		}
		i = j - 1;
	}
	
}

static int get_trap_vertex_no(int i_corner, T_PTR_Trapizium ptrs_trap)
/*-------------------------------------------------------------------*/
{
	if (i_corner == TRC_LD || i_corner == TRC_LU)
	{
		if (1 == ptrs_trap->b_line_left)
		{
			return ((i_corner == TRC_LD) ?
				ptrs_trap->s_line_l.vertex1_no : ptrs_trap->s_line_l.vertex2_no);
		}
		else
		{
			return ((i_corner == TRC_LD) ?
				ptrs_trap->s_arc_l.vertex1_no : ptrs_trap->s_arc_l.vertex2_no);
		}
	}
	if (i_corner == TRC_RD || i_corner == TRC_RU)
	{
		if (1 == ptrs_trap->b_line_right)
		{
			return ((i_corner == TRC_RD) ?
				ptrs_trap->s_line_r.vertex1_no : ptrs_trap->s_line_r.vertex2_no);
		}
		else
		{
			return ((i_corner == TRC_RD) ?
				ptrs_trap->s_arc_r.vertex1_no : ptrs_trap->s_arc_r.vertex2_no);
		}
	}

	return 0;
}

static BOOL
comp_vertex(int i_ver1, int i_ver2)
/*---------------------------------*/
{
	double df_x1, df_y1, df_x2, df_y2;
	BOOL b_ret;

    if ((i_ver1 < 0) || (i_ver1>100000) || (i_ver2 < 0) || (i_ver2>100000))
        return FALSE;

 if (i_ver1 == i_ver2)
 {
	return TRUE;
 }
 if ((i_ver1 <0) || (i_ver2<0))
 {
	 return TRUE;
 }
  df_x1 = get_vertex (i_ver1)->x ;
  df_y1 = get_vertex (i_ver1)->y ;
  df_x2 = get_vertex (i_ver2)->x ;
  df_y2 = get_vertex (i_ver2)->y ;
  b_ret = FALSE ;
  if (TRUE == Check_if_Equal (df_x1, df_x2) &&
      TRUE == Check_if_Equal (df_y1, df_y2))
  {
    b_ret = TRUE ;
  }
  return b_ret ;
}

static void ret_pole (double p0_)
{
  double p0, p01;

  p0=milimetryob(milimetryob(p0_));

  if (Jednostki==1)   //mm
   {
    p01=p0/1000000.0;
    counted_area=p01;
    counted_area0=p0/SkalaF/SkalaF;
   }
    else if (Jednostki==10)
      {
        p01=p0/10000.0;
        counted_area0=p0*100.0/SkalaF/SkalaF;
        counted_area=p01;
      }
         else if (Jednostki==1000)
          {
			p01=p0;
            counted_area0=p0*1000000.0/SkalaF/SkalaF;
            counted_area=p01;
          }
           else
            {
			  p01=p0;
              counted_area0=p0*1000000.0/SkalaF/SkalaF;
              counted_area=p01;
            }
}



static void out_pole (double p0_)
{
  char buf [MaxTextLen] ;
  double p0, p01, p02;

  p0=milimetryob(milimetryob(p0_));

  if (Jednostki==1)   //mm
   {
    p01=p0/1000000;
    p02=p0/pow(10.0, 2);
    //sprintf (buf, u8"%s = %-12.9lg mm²,  %-12.9lg cm²,  %-12.9lg m² ", vocabulary[1],p0,p02,p01 ) ;
    sprintf (buf, u8"%s=%.9lg mm²,  %.9lg cm²,  %.9lg m² ", vocabulary[1],p0,p02,p01 ) ;
   }
    else if (Jednostki==10) //cm
      {
        p01=p0/10000;
        p02=p0;
        //sprintf (buf, u8"%s = %-12.9lg cm²,  %-12.9lg m² ", vocabulary[1],p0,p01 ) ;
        sprintf (buf, u8"%s=%.9lg cm²,  %.9lg m² ", vocabulary[1],p0,p01 ) ;
      }
     else if (Jednostki==1000)  //m
      {
        p02=p0/pow(0.01, 2); //in cm2
        //sprintf (buf, u8"%s = %-12.9lg cm²,  %-12.9lg m² ", vocabulary[1],p02,p0) ;
        sprintf (buf, u8"%s=%.9lg cm²,  %.9lg m² ", vocabulary[1],p02,p0) ;
      }
      else if (Jednostki==1000000) //km
      {
          p01=p0*1000000;
          p02=p0/pow(0.00001, 2);
          //sprintf (buf, u8"%s = %-12.9lg km²,  %-12.9lg cm²,  %-12.9lg m² ", vocabulary[1],p0,p02,p01 ) ;
          sprintf (buf, u8"%s=%-.9lg km²,  %-.9lg cm²,  %-.9lg m² ", vocabulary[1],p0,p02,p01 ) ;
      }
      else if (Jednostki==25.4) //"
      {
          p01=p0/pow(39.3700787, 2);
          p02=p01;
          //sprintf (buf, u8"%s = %-12.9lg in²,  %-12.9lg m² ", vocabulary[1],p0,p01 ) ;
          sprintf (buf, u8"%s=%.9lg in²,  %.9lg m² ", vocabulary[1],p0,p01 ) ;
      }
      else if (Jednostki==304.8) //'
      {
          p01=p0/pow(3.280839, 2);
          p02=p01 * pow(12, 4);
          //sprintf (buf, u8"%s = %-12.9lg ft²,  %-12.9lg in²,  %-12.9lg m² ", vocabulary[1],p0,p02,p01 ) ;
          sprintf (buf, u8"%s=%.9lg ft²,  %.9lg in²,  %.9lg m² ", vocabulary[1],p0,p02,p01 ) ;
      }
      else if (Jednostki==914.4) //yd
      {
          p01=p0/pow(1.094092, 2);
          p02=p01 * pow(36, 4);
          //sprintf (buf, u8"%s = %-12.9lg yd²,  %-12.9lg in²,  %-12.9lg m² ", vocabulary[1],p0,p02,p01 ) ;
          sprintf (buf, u8"%s=%.9lg yd²,  %.9lg in²,  %.9lg m² ", vocabulary[1],p0,p02,p01 ) ;
      }
      else if (Jednostki==1609344) //mi
      {
          p01=p0/pow(0.000621371, 2);
          p02=p01 * pow(63360, 4);
          //sprintf (buf, u8"%s = %-12.9lg mi²,  %-12.9lg in²,  %-12.9lg m² ", vocabulary[1],p0,p02,p01 ) ;
          sprintf (buf, u8"%s=%.9lg mi²,  %.9lg in²,  %.9lg m² ", vocabulary[1],p0,p02,p01 ) ;
      }
       else ///sprintf (buf, u8"%s = %-12.9lg", vocabulary[1],p0) ;
            sprintf (buf, u8"%s=%.9lg", vocabulary[1],p0) ;
  Add_String_To_List1 (buf) ;
  strcat (buf, vocabulary[0]) ;
  komunikat_str (buf) ;
  Get_Key () ;
}

static void out_srodek_ciezkosci (double x_s0, double y_s0)
{
  char buf [MaxTextLen] ;
  double x_s, y_s ;
  
  x_s = milimetryobx (x_s0);
  y_s = milimetryoby (y_s0);
  //sprintf (buf, u8"%s: x = %-12.8lg  y = %-12.8lg ", vocabulary[2],x_s, y_s) ;
  sprintf (buf, u8"%s: x = %.8lg  y = %.8lg ", vocabulary[2],x_s, y_s) ;
  Add_String_To_List1 (buf) ;
  strcat (buf, vocabulary[0]) ;
  komunikat_str (buf) ;
  Get_Key () ;
}

static void out_moment_statyczny (double mom_stat_)
{
  double mom_stat;
  char buf [MaxTextLen] ;
  double mom_stat1, mom_stat2;

  mom_stat=milimetryob(milimetryob(milimetryob(mom_stat_)));

  if (Jednostki == 1)   //mm
  {
      mom_stat1 = mom_stat / pow(1000.0, 3); //in m3
      mom_stat2 = mom_stat / pow(10.0, 3); //in cm3
  }
  else if (Jednostki == 10) //cm
  {
      mom_stat1 = mom_stat / pow(100.0, 3);
      mom_stat2 = mom_stat;
  }
  else if (Jednostki == 1000)  //m
  {
      mom_stat1 = mom_stat / 1.0;
      mom_stat2 = mom_stat / pow(0.01, 3); //in cm3
  }
  else if (Jednostki == 1000000)  //km
  {
      mom_stat1 = mom_stat / pow(0.001, 3);
      mom_stat2 = mom_stat / pow(0.00001, 3);
  }
  else if (Jednostki == 25.4)  //"
  {
      mom_stat1 = mom_stat / pow(39.3700787, 3);
      mom_stat2 = mom_stat1;
  }
  else if (Jednostki == 304.8)  //'
  {
      mom_stat1 = mom_stat / pow(3.280839, 3);
      mom_stat2 = mom_stat1 * pow(12, 3);
  }
  else if (Jednostki == 914.4)  //yd
  {
      mom_stat1 = mom_stat / pow(1.094092, 3);
      mom_stat2 = mom_stat1 * pow(36, 3);
  }
  else if (Jednostki == 1609344)  //mi
  {
      mom_stat1 = mom_stat / pow(0.000621371, 3);
      mom_stat2 = mom_stat1 * pow(63360, 3);
  }
	  

  //sprintf(buf, u8"%s: S = %.8lg m³,  %.8lg cm³ ", vocabulary[3], mom_stat1, mom_stat2);
  sprintf(buf, u8"%s: S=%.8lg m³,  %.8lg cm³ ", vocabulary[3], mom_stat1, mom_stat2);
  Add_String_To_List1 (buf) ;
  strcat (buf, vocabulary[0]) ;
  komunikat_str (buf) ;
  Get_Key () ;
}

static void out_moment_bezwladnosci (double mom_bezwl_)
{
  double mom_bezwl;
  char buf [MaxTextLen] ;
  double mom_bezwl1, mom_bezwl2;

  mom_bezwl=milimetryob(milimetryob(milimetryob(milimetryob(mom_bezwl_))));

  if (Jednostki == 1)   //mm
  {
      mom_bezwl1 = mom_bezwl / pow(1000.0, 4);
      mom_bezwl2 = mom_bezwl / pow(10.0, 4);
  }
  else if (Jednostki == 10) //cm
  {
      mom_bezwl1 = mom_bezwl / pow(100.0, 4);
      mom_bezwl2 = mom_bezwl;
  }
  else if (Jednostki == 1000)  //m
  {
      mom_bezwl1 = mom_bezwl / 1.0;
      mom_bezwl2 = mom_bezwl / pow(0.01, 4); //in cm4
  }
  else if (Jednostki == 1000000)  //km
  {
      mom_bezwl1 = mom_bezwl / pow(0.001, 4);
      mom_bezwl2 = mom_bezwl / pow(0.00001, 4);
  }
  else if (Jednostki == 25.4)  //"
  {
      mom_bezwl1 = mom_bezwl / pow(39.3700787, 4);
      mom_bezwl2 = mom_bezwl1;
  }
  else if (Jednostki == 304.8)  //'
  {
      mom_bezwl1 = mom_bezwl / pow(3.280839, 4);
      mom_bezwl2 = mom_bezwl1 * pow(12, 4);
  }
  else if (Jednostki == 914.4)  //yd
  {
      mom_bezwl1 = mom_bezwl / pow(1.094092, 4);
      mom_bezwl2 = mom_bezwl1 * pow(36, 4);
  }
  else if (Jednostki == 1609344)  //mi
  {
      mom_bezwl1 = mom_bezwl / pow(0.000621371, 4);
      mom_bezwl2 = mom_bezwl1 * pow(63360, 4);
  }

  //sprintf (buf, u8"%s: I = %-12.8lg m^4,  %-12.8lg cm^4 ", vocabulary[4],mom_bezwl1, mom_bezwl2) ;
  sprintf (buf, u8"%s: I=%.8lg m⁴,  %.8lg cm⁴ ", vocabulary[4],mom_bezwl1, mom_bezwl2) ;
  Add_String_To_List1 (buf) ;
  strcat (buf, vocabulary[0]) ;
  komunikat_str (buf) ;
  Get_Key () ;
}

static void out_centr_moment_bezwl (double mom_bezwl_x_, double mom_bezwl_y_, double wskazn_wytrz_x_, double wskazn_wytrz_y_)

{
  char buf0 [320] ;
  char buf[4][128];
  double mom_bezwl_x, mom_bezwl_y, wskazn_wytrz_x, wskazn_wytrz_y;
  double mom_bezwl_x1, mom_bezwl_y1, wskazn_wytrz_x1, wskazn_wytrz_y1;
  double mom_bezwl_x2, mom_bezwl_y2, wskazn_wytrz_x2, wskazn_wytrz_y2;
#ifdef LINUX
  char divider[6]=" \n";
#else
  char divider[6] = " \r\n";
#endif

    mom_bezwl_x=milimetryob(milimetryob(milimetryob(milimetryob(mom_bezwl_x_))));
    mom_bezwl_y=milimetryob(milimetryob(milimetryob(milimetryob(mom_bezwl_y_))));
    wskazn_wytrz_x=milimetryob(milimetryob(milimetryob(wskazn_wytrz_x_)));
    wskazn_wytrz_y=milimetryob(milimetryob(milimetryob(wskazn_wytrz_y_)));

  ////mom_bezwl

  if (Jednostki == 1)   //mm
  {
	  mom_bezwl_x1 = mom_bezwl_x / pow(1000.0, 4);
	  mom_bezwl_y1 = mom_bezwl_y / pow(1000.0, 4);

      mom_bezwl_x2 = mom_bezwl_x / pow(10.0, 4);
      mom_bezwl_y2 = mom_bezwl_y / pow(10.0, 4);
  }
  else if (Jednostki == 10) //cm
  {
	  mom_bezwl_x1 = mom_bezwl_x / pow(100.0, 4);
	  mom_bezwl_y1 = mom_bezwl_y / pow(100.0, 4);

      mom_bezwl_x2 = mom_bezwl_x;
      mom_bezwl_y2 = mom_bezwl_y;
  }
  else if (Jednostki == 1000)  //m
  {
	  mom_bezwl_x1 = mom_bezwl_x / 1.0;
	  mom_bezwl_y1 = mom_bezwl_y / 1.0;

      mom_bezwl_x2 = mom_bezwl_x / pow(0.01, 4); //in cm4
      mom_bezwl_y2 = mom_bezwl_y / pow(0.01, 4); //in cm4
  }
  else if (Jednostki == 1000000)  //km
  {
      mom_bezwl_x1 = mom_bezwl_x / pow(0.001, 4);
      mom_bezwl_y1 = mom_bezwl_y / pow(0.001, 4);

      mom_bezwl_x2 = mom_bezwl_x / pow(0.00001, 4);
      mom_bezwl_y2 = mom_bezwl_y / pow(0.00001, 4);
  }
  else if (Jednostki == 25.4)  //"
  {
	  mom_bezwl_x1 = mom_bezwl_x / pow(39.3700787, 4);
	  mom_bezwl_y1 = mom_bezwl_y / pow(39.3700787, 4);

      mom_bezwl_x2 = mom_bezwl_x1;
      mom_bezwl_y2 = mom_bezwl_y1;
  }
  else if (Jednostki == 304.8)  //'
  {
	  mom_bezwl_x1 = mom_bezwl_x / pow(3.280839, 4);
	  mom_bezwl_y1 = mom_bezwl_y / pow(3.280839, 4);

      mom_bezwl_x2 = mom_bezwl_x1 * pow(12, 4);
      mom_bezwl_y2 = mom_bezwl_y1 * pow(12, 4);
  }
  else if (Jednostki == 914.4)  //yd
  {
      mom_bezwl_x1 = mom_bezwl_x / pow(1.094092, 4);
      mom_bezwl_y1 = mom_bezwl_y / pow(1.094092, 4);

      mom_bezwl_x2 = mom_bezwl_x1 * pow(36, 4);
      mom_bezwl_y2 = mom_bezwl_y1 * pow(36, 4);
  }

  else if (Jednostki == 1609344)  //mi
  {
      mom_bezwl_x1 = mom_bezwl_x / pow(0.000621371, 4);
      mom_bezwl_y1 = mom_bezwl_y / pow(0.000621371, 4);

      mom_bezwl_x2 = mom_bezwl_x1 * pow(63360, 4);
      mom_bezwl_y2 = mom_bezwl_y1 * pow(63360, 4);
  }

  ////wskazn_wytrz

  if (Jednostki == 1)   //mm
  {
	  wskazn_wytrz_x1 = wskazn_wytrz_x / pow(1000.0, 3);
	  wskazn_wytrz_y1 = wskazn_wytrz_y / pow(1000.0, 3);

      wskazn_wytrz_x2 = wskazn_wytrz_x / pow(10.0, 3);
      wskazn_wytrz_y2 = wskazn_wytrz_y / pow(10.0, 3);
  }
  else if (Jednostki == 10) //cm
  {
	  wskazn_wytrz_x1 = wskazn_wytrz_x / pow(100.0, 3);
	  wskazn_wytrz_y1 = wskazn_wytrz_y / pow(100.0, 3);

      wskazn_wytrz_x2 = wskazn_wytrz_x;
      wskazn_wytrz_y2 = wskazn_wytrz_y;
  }
  else if (Jednostki == 1000)  //m
  {
      wskazn_wytrz_x1 = wskazn_wytrz_x / 1.0;
      wskazn_wytrz_y1 = wskazn_wytrz_y / 1.0;

      wskazn_wytrz_x2 = wskazn_wytrz_x / pow(0.01, 3); //in cm4
      wskazn_wytrz_y2 = wskazn_wytrz_y / pow(0.01, 3); //in cm4
  }
  else if (Jednostki == 1000000)  //km
  {
      wskazn_wytrz_x1 = wskazn_wytrz_x / pow(0.001, 3);
      wskazn_wytrz_y1 = wskazn_wytrz_y / pow(0.001, 3);

      wskazn_wytrz_x2 = wskazn_wytrz_x / pow(0.00001, 3);
      wskazn_wytrz_y2 = wskazn_wytrz_y / pow(0.00001, 3);
  }
  else if (Jednostki == 25.4)  //"
  {
	  wskazn_wytrz_x1 = wskazn_wytrz_x / pow(39.3700787, 3);
	  wskazn_wytrz_y1 = wskazn_wytrz_y / pow(39.3700787, 3);

      wskazn_wytrz_x2 = wskazn_wytrz_x1;
      wskazn_wytrz_y2 = wskazn_wytrz_y1;
  }
  else if (Jednostki == 304.8)  //'
  {
	  wskazn_wytrz_x1 = wskazn_wytrz_x / pow(3.280839, 3);
	  wskazn_wytrz_y1 = wskazn_wytrz_y / pow(3.280839, 3);

      wskazn_wytrz_x2 = wskazn_wytrz_x1 * pow(12, 3);;
      wskazn_wytrz_y2 = wskazn_wytrz_y1 * pow(12, 3);;
  }
    else if (Jednostki == 914.4)  //yd
    {
        wskazn_wytrz_x1 = wskazn_wytrz_x / pow(1.094092, 3);
        wskazn_wytrz_y1 = wskazn_wytrz_y / pow(1.094092, 3);

        wskazn_wytrz_x2 = wskazn_wytrz_x1 * pow(36, 3);
        wskazn_wytrz_y2 = wskazn_wytrz_y1 * pow(36, 3);
    }
    else if (Jednostki == 1609344)  //mi
    {
        wskazn_wytrz_x1 = wskazn_wytrz_x / pow(0.000621371, 3);
        wskazn_wytrz_y1 = wskazn_wytrz_y / pow(0.000621371, 3);

        wskazn_wytrz_x2 = wskazn_wytrz_x1 * pow(63360, 3);;
        wskazn_wytrz_y2 = wskazn_wytrz_y1 * pow(63360, 3);;
    }

    //sprintf(buf[0], u8"Iy=%-11.8lg m⁴, %-11.8lg cm⁴", mom_bezwl_x1, mom_bezwl_x2) ;
    //sprintf(buf[1], u8"Iz=%-11.8lg m⁴, %-11.8lg cm⁴", mom_bezwl_y1, mom_bezwl_y2);
    //sprintf(buf[2], u8"Wy=%-11.8lg m³, %-11.8lg cm³", wskazn_wytrz_x1, wskazn_wytrz_x2);
    //sprintf(buf[3], u8"Wz=%-11.8lg m³, %-11.8lg cm³ ", wskazn_wytrz_y1, wskazn_wytrz_y2);

    sprintf(buf[0], u8"Iy=%.8lg m⁴, %.8lg cm⁴", mom_bezwl_x1, mom_bezwl_x2) ;
    sprintf(buf[1], u8"Iz=%.8lg m⁴, %.8lg cm⁴", mom_bezwl_y1, mom_bezwl_y2);
    sprintf(buf[2], u8"Wy=%.8lg m³, %.8lg cm³", wskazn_wytrz_x1, wskazn_wytrz_x2);
    sprintf(buf[3], u8"Wz=%.8lg m³, %.8lg cm³ ", wskazn_wytrz_y1, wskazn_wytrz_y2);

  //Add_String_To_List1(buf[0]) ;
  //Add_String_To_List1(buf[1]);
  //Add_String_To_List1(buf[2]);
  //Add_String_To_List1(buf[3]);
  strcpy(buf0, buf[0]);
    strcat(buf0, divider);
  strcat(buf0, buf[1]);
    strcat(buf0, divider);
  strcat(buf0, buf[2]);
    strcat(buf0, divider);
  strcat(buf0, buf[3]);

  Add_String_To_List1(buf0);

  strcat(buf0, divider);
  strcat (buf0, vocabulary[0]) ;
  komunikat_str (buf0) ;
  Get_Key () ;
}

BOOL interrupted(void)
{
	if (my_kbhit() && my_getch() == ESC)
	{
		while (my_kbhit())
		{
			my_getch();
		}
		return TRUE;
	}
	else return FALSE;
}

static BOOL
hatch_ver_reg__ (T_PTR_Hatch_Param 	ptrs_hatch_param,
	       T_PTR_Hatch_Line_Def  	ptrs_hatch_line_def,
	       T_PTR_Trapizium 		ptrs_trap,
	       int 			i_ver, int draw_line)
/*---------------------------------------------------*/
{
  T_PTR_Trapizium ptrs_traps ;
  int i, i_ver_trap ;
  int key;
  T_PTR_Trapizium ptrs_trap_next;

  ptrs_traps = ptrs__traps ;
  for (i = 0 ; i < i__trapsno; i++)
  {
		  ptrs_trap_next= &ptrs_traps[i+1];

    if (ptrs_traps[i].b_line_left) {

        i_ver_trap = get_trap_vertex_no(TRC_LU, &ptrs_traps[i]);
        if (TRUE == comp_vertex(i_ver_trap, i_ver)) {
            ptrs_traps[i].vertex_up_no = i_ver;
            if (FALSE == hatch_trap(&ptrs_traps[i], ptrs_hatch_param,
                                    ptrs_hatch_line_def, ptrs_trap_next, draw_line)) {
                return FALSE;
            }
            ptrs_traps[i].vertex_dn_no = i_ver;
            ptrs_traps[i].b_line_left = ptrs_trap->b_line_right;
            memcpy(ptrs_traps[i].bank_l, ptrs_trap->bank_r, BANK_S);
            /* out_pole (pole_hatch) ; */
            return TRUE;
        }
    }


    if (ptrs_traps[i].b_line_right) {
        i_ver_trap = get_trap_vertex_no(TRC_RU, &ptrs_traps[i]);
        if (TRUE == comp_vertex(i_ver_trap, i_ver)) {
            ptrs_traps[i].vertex_up_no = i_ver;
            if (FALSE == hatch_trap(&ptrs_traps[i], ptrs_hatch_param,
                                    ptrs_hatch_line_def, ptrs_trap_next, draw_line)) {
                return FALSE;
            }
            ptrs_traps[i].vertex_dn_no = i_ver;
            ptrs_traps[i].b_line_right = ptrs_trap->b_line_right;
            memcpy(ptrs_traps[i].bank_r, ptrs_trap->bank_r, BANK_S);
            return TRUE;
        }
    }

	if (interrupted()) return FALSE;
  }

  Internal_Error (__LINE__,__FILE__) ;  

  return FALSE ;  //TO REMOVE ALREADY GENERATED PART OF HATCH
  //return TRUE ;  //TO CORRECT
}

static BOOL
hatch_ver_reg (T_PTR_Hatch_Param 	ptrs_hatch_param,
               T_PTR_Hatch_Line_Def  	ptrs_hatch_line_def,
               T_PTR_Trapizium 		ptrs_trap,
               int 			i_ver, int draw_line)
/*---------------------------------------------------*/
{
    T_PTR_Trapizium ptrs_traps ;
    int i, i_ver_trap ;
    int key;

    T_PTR_Trapizium ptrs_trap_next;

    ptrs_traps = ptrs__traps ;
    for (i = 0 ; i < i__trapsno; i++)
    {
        if (i < (i__trapsno - 1))   ////  ?????????????
        //if (i < (i__trapsno))   ////  ?????????????
            ptrs_trap_next = &ptrs_traps[i + 1];
        else ptrs_trap_next = NULL;  //// ?????????

            i_ver_trap = get_trap_vertex_no(TRC_LU, &ptrs_traps[i]);
            if (TRUE == comp_vertex(i_ver_trap, i_ver))
            {
                ptrs_traps[i].vertex_up_no = i_ver;
                if (FALSE == hatch_trap(&ptrs_traps[i], ptrs_hatch_param,
                                        ptrs_hatch_line_def, ptrs_trap_next, draw_line))
                {
                    return FALSE;
                }
                ptrs_traps[i].vertex_dn_no = i_ver;
                ptrs_traps[i].b_line_left = ptrs_trap->b_line_right;
                memcpy(ptrs_traps[i].bank_l, ptrs_trap->bank_r, BANK_S);
                /* out_pole (pole_hatch) ; */
                return TRUE;
            }

            i_ver_trap = get_trap_vertex_no(TRC_RU, &ptrs_traps[i]);
            if (TRUE == comp_vertex(i_ver_trap, i_ver)) {
                ptrs_traps[i].vertex_up_no = i_ver;
                if (FALSE == hatch_trap(&ptrs_traps[i], ptrs_hatch_param,
                                        ptrs_hatch_line_def, ptrs_trap_next, draw_line)) {
                    return FALSE;
                }
                ptrs_traps[i].vertex_dn_no = i_ver;
                ptrs_traps[i].b_line_right = ptrs_trap->b_line_right;
                memcpy(ptrs_traps[i].bank_r, ptrs_trap->bank_r, BANK_S);
                /*   out_pole (pole_hatch) ; */
                return TRUE;
            }


        if (interrupted()) return FALSE;
    }

    Internal_Error (__LINE__,__FILE__) ;

    return FALSE ;
//    return TRUE ;
}

/* - gdy punkt lezy po lewej stronie odcinka + gdy po prawej*/
static double
check_vertex_line (double 	 df_x,
		   double 	 df_y,
		   T_PTR_ALine   ptrs_line)
/*------------------------------------------------*/
{
  double df_x1, df_y1, df_x2, df_y2 ;
  double df_ret ;
  int i_ver1, i_ver2 ;

  i_ver1 = ptrs_line->vertex1_no ;
  i_ver2 = ptrs_line->vertex2_no ;
  df_x1 = get_vertex (i_ver1)->x ;
  df_y1 = get_vertex (i_ver1)->y ;
  df_x2 = get_vertex (i_ver2)->x ;
  df_y2 = get_vertex (i_ver2)->y ;
  df_ret = (df_x2 - df_x1) * (df_y2 - df_y) - (df_y2 - df_y1) * (df_x2 - df_x) ;
  return df_ret ;
}

/* - gdy punkt lezy po lewej stronie luku
   + gdy po prawej*/
static double
check_vertex_arc ( double 	df_x,
		   double 	df_y,
		   T_PTR_AArc 	ptrs_arc)
/*----------------------------------------------*/
{
  double df_x1, df_dd ;
  double df_ret ;
  LUK s_arc ;

  s_arc.r = ptrs_arc->r ;
  s_arc.x = ptrs_arc->x ;
  s_arc.y = ptrs_arc->y ;
  s_arc.kat1 = ptrs_arc->a1 ;
  s_arc.kat2 = ptrs_arc->a2 ;
  df_dd = ptrs_arc->r * ptrs_arc->r -
	  (df_y - ptrs_arc->y) * (df_y - ptrs_arc->y) ;

  if (df_dd < 0)
  {
    df_dd = 0 ;
  }
  df_x1 = ptrs_arc->x + sqrt (df_dd) ;
  if (TRUE == Point_Arc_Prec (df_x1, df_y, &s_arc, MPREC_3))
  {
    df_ret = df_x - df_x1 ;
  }
  else
  {
    df_x1 = ptrs_arc->x - sqrt (df_dd) ;
    df_ret = df_x - df_x1 ;
  }
  return df_ret ;
}


/*funkcja sprawdza czy wierzcholek lezy wewnatrz aktywnego trapezu*/
/*i zwraca jego numer, -1 gdy nie znajdzie*/
static int
check_ver_in_trap (int i_ver)
/*---------------------------*/
{
  int i ;
  int i_ret ;
  double df_x, df_y ;
  T_PTR_Trapizium ptrs_traps ;

  i_ret = -1 ;
  ptrs_traps = ptrs__traps ;
  df_x = get_vertex (i_ver)->x ;
  df_y = get_vertex (i_ver)->y ;
  for (i = 0 ; i < i__trapsno; i++)
  {
    if (1 == ptrs_traps [i].b_line_left)
    {
      if (check_vertex_line (df_x, df_y, &ptrs_traps [i].s_line_l) <= 0)
	continue ;
    }
    else
    {
      if (check_vertex_arc (df_x, df_y, &ptrs_traps [i].s_arc_l) <= 0)
	continue ;
    }
    if (1 == ptrs_traps [i].b_line_right)
    {
      if (check_vertex_line (df_x, df_y, &ptrs_traps [i].s_line_r) >= 0)
	continue ;
    }
    else
    {
      if (check_vertex_arc (df_x, df_y, &ptrs_traps [i].s_arc_r) >= 0)
	continue ;
    }
    i_ret = i ;
    break ;
  }
  return i_ret ;
}

static BOOL
add_hatch_break_line (
		double 		      df_x1,
		double 		      df_x2,
		double 		      df_y,
		double 		      df_pattern_remx,
		T_PTR_Hatch_Param     ptrs_hatch_param,
		T_PTR_Hatch_Line_Def  ptrs_hatch_line_def,
        int draw_line)
/*----------------------------------------------------------------------------------*/
{
  double df_si, df_co, df_angle ;
  double df_scale, df_len_pattern ;
  double df_len_lines ;
  double df_x, df_len, df_line_len ;
  int i_def_size, i, i_last_elem, i_sign ;
  BOOL b_down, b_rem ;

  df_len = df_x2 - df_x1 ;

  if (df_len < 0.0) 
	  return TRUE;
  df_angle = ptrs_hatch_line_def->df_angle + ptrs_hatch_param->df_angle ;
  df_si = sin (df_angle) ;
  df_co = cos (df_angle) ;
  df_scale = ptrs_hatch_param->df_scale ;
  df_len_pattern = ptrs_hatch_line_def->df_len_pattern * df_scale ;
  i_def_size = ptrs_hatch_line_def->i_def_size ;
  for (i = 0, df_len_lines = 0 ; i < i_def_size ; i++)
  {
     df_len_lines += fabs (ptrs_hatch_line_def->df_def_line [i]) * df_scale ;
     if (TRUE == Check_if_GE (df_len_lines, df_len_pattern - df_pattern_remx))
     {
       break ;
     }
  }
  i_last_elem = i ;
  b_rem = TRUE ;
  while (TRUE)
  {
    for ( i = i_last_elem ; i < i_def_size ; i++)
    {
      df_line_len = ptrs_hatch_line_def->df_def_line [i] * df_scale ;
      if (b_rem == TRUE)
      {
		i_sign = (df_line_len > 0) ? 1 : -1 ;
		df_line_len = i_sign * (df_len_lines - (df_len_pattern - df_pattern_remx)) ;

		b_rem = FALSE ;
      }
      b_down = TRUE ;
      if (df_line_len < 0)
      {
	  df_line_len = fabs (df_line_len) ;
	  b_down = FALSE ;
      }

      if (df_line_len > df_len)
      {
		
		df_line_len = df_len ;

		
      }
      df_x = df_x1 + df_line_len ;
      df_len -= df_line_len ;
      if (b_down == TRUE)
      {

	LiniaG.x1 = df_x1 ;
	LiniaG.y1 = df_y ;
	LiniaG.x2 = df_x ;
	LiniaG.y2 = df_y ;
	Rotate_Line ((void *)&LiniaG,
		   ptrs_hatch_param->df_basex,
		   ptrs_hatch_param->df_basey,
		   df_si, df_co) ;

    PTR__GTMP6=(char*)global_dane;
	global_dane_ob = dodaj_obiekt(global_dane, &LiniaG);
    global_dane=(BLOK*)PTR__GTMP6;

	if (NULL==global_dane_ob)
	{
	  if (ERR_BLOCK_SIZE == Get_Error_No ())
	  {
		global_dane = (BLOK *)add_block(FALSE, NULL, Anormalny);
	    if (global_dane==NULL) return FALSE;
		global_dane = (BLOK *)dodaj_obiekt((BLOK*)dane, &LiniaG);
		if (global_dane == NULL) return FALSE;
		global_dane = (BLOK *)dane;
	  }
	  else
	  {
	    return FALSE ;
	  }
	}
	if (draw_line) rysuj_obiekt ((char *)&LiniaG, COPY_PUT, 1) ;

      }
      if (TRUE == Check_if_Equal (df_len, 0))
      {
	  return TRUE ;
      }
      df_x1 = df_x ;
    }
    i_last_elem = 0 ;
  }

}

static BOOL
add_hatch_line (double 		      df_x1,
		double 		      df_x2,
		double 		      df_y,
		double 		      df_basex,
		double 		      df_basey,
		T_PTR_Hatch_Param     ptrs_hatch_param,
		T_PTR_Hatch_Line_Def  ptrs_hatch_line_def,
        int draw_line)
/*------------------------------------------------*/
{
  double df_si, df_co, df_angle ;
  double df_scale, df_len_pattern, df_pattern_remx ;
  long l_row ;
  double df_x1m, df_x2m, df_x3m, delta_x, delta_y ;
  double del_y;

  if (TRUE == Check_Break_Key ())
  {
    return FALSE ;
  }
  if ((0 == ptrs_hatch_line_def->i_def_size) || ( comput_area1>0 ))
  {
     if ( comput_area1>0 )
     {
     df_angle = 0;
     }
     else
     {
     df_angle = ptrs_hatch_line_def->df_angle +
		ptrs_hatch_param->df_angle ;
     }
     df_si = sin (df_angle) ;
     df_co = cos (df_angle) ;
     LiniaG.x1 = df_x1 ;
     LiniaG.y1 = df_y ;
     LiniaG.x2 = df_x2 ;
     LiniaG.y2 = df_y ;
     if ( comput_area1>0 )
     {
     }
	 else
	 {
		 Rotate_Line((void *)&LiniaG,
			 ptrs_hatch_param->df_basex,
			 ptrs_hatch_param->df_basey,
			 df_si, df_co);


		 /*    przy wymiarowaniu pola nalezy usunac dodawanie obiektow */

         PTR__GTMP6=(char*)global_dane;
		 global_dane_ob = dodaj_obiekt(global_dane, &LiniaG);
         global_dane=(BLOK*)PTR__GTMP6;


		 if (NULL == global_dane_ob)
		 {
			 if (ERR_BLOCK_SIZE == Get_Error_No())
			 {
				 global_dane = (BLOK *)add_block(FALSE, NULL, Anormalny);
				 if (global_dane == NULL) return FALSE;
				 global_dane = (BLOK *)dodaj_obiekt((BLOK*)dane, &LiniaG);
				 if (global_dane == NULL) return FALSE;
				 global_dane = (BLOK *)dane;
			 }
			 else
			 {
				 return FALSE;
			 }
		 }
	 }
     if ((out_pole_ignore==FALSE) && (draw_line)) rysuj_obiekt ((char *)&LiniaG, COPY_PUT, 1) ;
      if ( comput_area1>0 )
     {
      /*
      df_x1m = milimetryob(LiniaG.x1) ;     // milimetryob
      df_x2m = milimetryob(LiniaG.x2) ;
      df_x3m = milimetryob(df_dist_) ;    // milimetryob
      if ( pole_hatch == 0 )
      {
      pole_hatch1 = ( ( df_x2m - df_x1m ) * df_x3m ) ;
      pole_hatch = pole_hatch + pole_hatch1 ;
      min_xw=df_x1m;
      max_xw=df_x2m;
      min_yw=df_y;
      max_yw=df_y+df_x3m;
      }
      else
      {
      pole_hatch1 = (0.5 * (( df_x2m - df_x1m ) + ( df_x22m - df_x11m)) * df_x3m ) ;
      pole_hatch = pole_hatch + pole_hatch1 ;
      if (df_x1m<min_xw) min_xw=df_x1m;
      if (df_x2m>max_xw) max_xw=df_x2m;
      if (df_y<min_yw) min_yw=df_y;
      if ((df_y+df_x3m)>max_yw) max_yw=(df_y+df_x3m);
      }
      // momenty statyczne
      moment = pole_hatch1 * (LiniaG.y1+(0.5*df_x3m));
      mom_stat_x = mom_stat_x + moment;
      moment = pole_hatch1 * (0.5 * (LiniaG.x1 + LiniaG.x2));
      mom_stat_y = mom_stat_y + moment;
      // momenty bezwladnosci
      delta_x=df_x2m - df_x1m;
      delta_y=milimetryob(LiniaG.y1)+(0.5*df_x3m);
      moment = ((delta_x)*df_x3m*df_x3m*df_x3m/12)
		+ (pole_hatch1*delta_y*delta_y);
      mom_bezwl_x=mom_bezwl_x+moment;


      moment = (df_x3m*delta_x*delta_x*delta_x/12)
		+ (pole_hatch1*((df_x1m+df_x2m)*0.5)*((df_x1m+df_x2m)*0.5));
      mom_bezwl_y=mom_bezwl_y+moment;
      // moment dewiacji
      moment = delta_x*df_x3m*(0.5*(df_x1m+df_x2m))*delta_y;
      mom_dewiacji = mom_dewiacji + moment;

      df_x11m = df_x1m ;
      df_x22m = df_x2m ;
       */

         df_x1m = LiniaG.x1 ;
         df_x2m = LiniaG.x2 ;
         df_x3m = df_dist_ ;
         if ( pole_hatch == 0 )
         {

             //del_y=(LiniaG.y1-ymin_g);
             //if (del_y<(df_dist_/2)) df_x3m-=(df_dist_/2)-del_y; //half of the stripe for first line

         pole_hatch1 = ( ( df_x2m - df_x1m ) * df_x3m ) ;
         pole_hatch = pole_hatch + pole_hatch1 ;
         min_xw=df_x1m;
         max_xw=df_x2m;
         min_yw=df_y;
         max_yw=df_y+df_x3m;
         }
         else
         {

             /*
             del_y=(ymax_g-LiniaG.y1);

             if (del_y<0)
             {
                 if (del_y>(-df_dist_/2))
                     df_x3m=(df_dist_/2)+del_y;
             }
             else if (del_y<(df_dist_/2))
                 df_x3m=(df_dist_/2)+del_y;
                 */

         pole_hatch1 = (0.5 * (( df_x2m - df_x1m ) + ( df_x22m - df_x11m)) * df_x3m ) ;
         pole_hatch = pole_hatch + pole_hatch1 ;
         if (df_x1m<min_xw) min_xw=df_x1m;
         if (df_x2m>max_xw) max_xw=df_x2m;
         if (df_y<min_yw) min_yw=df_y;
         if ((df_y+df_x3m)>max_yw) max_yw=(df_y+df_x3m);
         }
         // momenty statyczne
         moment = pole_hatch1 * (LiniaG.y1+(0.5*df_x3m));
         mom_stat_x = mom_stat_x + moment;
         moment = pole_hatch1 * (0.5 * (LiniaG.x1 + LiniaG.x2));
         mom_stat_y = mom_stat_y + moment;
         // momenty bezwladnosci
         delta_x=df_x2m - df_x1m;
         delta_y=LiniaG.y1+(0.5*df_x3m);
         moment = ((delta_x)*df_x3m*df_x3m*df_x3m/12)
           + (pole_hatch1*delta_y*delta_y);
         mom_bezwl_x=mom_bezwl_x+moment;

         moment = (df_x3m*delta_x*delta_x*delta_x/12)
           + (pole_hatch1*((df_x1m+df_x2m)*0.5)*((df_x1m+df_x2m)*0.5));
         mom_bezwl_y=mom_bezwl_y+moment;
         // moment dewiacji
         moment = delta_x*df_x3m*(0.5*(df_x1m+df_x2m))*delta_y;
         mom_dewiacji = mom_dewiacji + moment;

         df_x11m = df_x1m ;
         df_x22m = df_x2m ;

     }
     return TRUE ;
  }
  else
  {
    df_scale = ptrs_hatch_param->df_scale ;
    l_row = floor (0.5 + (df_y - df_basey) / (ptrs_hatch_line_def->df_dist *
					df_scale)) ;
    df_basex += ptrs_hatch_line_def->df_move * df_scale * l_row ;
    df_len_pattern = ptrs_hatch_line_def->df_len_pattern * df_scale ;
    if (df_x1 > df_basex)
    {
      df_pattern_remx = df_len_pattern - fmod (df_x1 - df_basex, df_len_pattern) ;
    }
    else
    {
      df_pattern_remx = fmod (df_basex - df_x1, df_len_pattern) ;
    }
    return add_hatch_break_line (df_x1, df_x2, df_y, df_pattern_remx,
			ptrs_hatch_param, ptrs_hatch_line_def, draw_line) ;
  }
}


/*dlugosci w T_PTR_Hatch_Param i T_PTR_Hatch_Line_Def podane sa w jednostkach*/
/*rysunku*/
static BOOL
hatch_trap (T_PTR_Trapizium 	  ptrs_trap,
	    T_PTR_Hatch_Param 	  ptrs_hatch_param,
	    T_PTR_Hatch_Line_Def  ptrs_hatch_line_def, T_PTR_Trapizium ptrs_trap_next,
        int draw_line)
/*------------------------------------------------------------------------------*/
{
  double df_cl, df_cr ;
  double df_xl, df_xr, df_y ;
  double df_ymin, df_ymax, df_dist; // df_ymin_next, df_ymax_next;
  double df_x1, df_y1, df_x2, df_y2, df_x3, df_y3, df_x4, df_y4 ;
  double df_basex, df_basey ;	/*punkt bazowy w jednostkach rysunku*/
  double df_angle ;
  double df_dd ;
  T_AArc t_aarc = { 0, 0, FALSE, FALSE, 0, 0, 0, 0, 0 };
  T_PTR_AArc ptrs_arc;
  LUK s_arc=ldef ;
  double dfymin, dfymax, dx, dy;
  char* scale_ptr;
  char* dx_ptr;
  char* angle_ptr;
  char* dy_ptr;
  char* name_ptr;
  char* translucency_ptr;
  
  double df__xl;

  ptrs_arc = &t_aarc;

  if ( comput_area1>0 )
   {
    df_basex = 0;
    df_basey = 0;
    df_angle = 0;
    df_dist = 0.001;  //was 0.1 before
   }
   else
   {

    df_basex = ptrs_hatch_param->df_basex ;
    df_basey = ptrs_hatch_param->df_basey ;
    df_basex += ptrs_hatch_line_def->df_startx * ptrs_hatch_param->df_scale ;
    df_basey += ptrs_hatch_line_def->df_starty * ptrs_hatch_param->df_scale ;
    df_angle = - (ptrs_hatch_line_def->df_angle + ptrs_hatch_param->df_angle) ;

    Rotate_Point (sin (ptrs_hatch_param->df_angle), cos (ptrs_hatch_param->df_angle),
		ptrs_hatch_param->df_basex,
		ptrs_hatch_param->df_basey,
		df_basex, df_basey, &df_basex, &df_basey) ;

    Rotate_Point (sin (df_angle), cos (df_angle),
		ptrs_hatch_param->df_basex,
		ptrs_hatch_param->df_basey,
		df_basex, df_basey, &df_basex, &df_basey) ;

	if (i__hatch_pattern_no > 0)
        df_dist = ptrs_hatch_line_def->df_dist * ptrs_hatch_param->df_scale ;
	else df_dist = 0.001;
   }
  df_ymin = get_vertex (ptrs_trap->vertex_dn_no)->y ;
  df_ymax = get_vertex (ptrs_trap->vertex_up_no)->y ;
  if (df_ymin > df_basey)
  {
    df_y =  df_ymin + df_dist - fmod (df_ymin - df_basey, df_dist) ;
  }
  else
  {
    df_y =  df_ymin + fmod (df_basey - df_ymin, df_dist) ;
  }
  if (1 == ptrs_trap->b_line_left)
  {
    df_x1 = get_vertex (ptrs_trap->s_line_l.vertex1_no)->x ;
    df_y1 = get_vertex (ptrs_trap->s_line_l.vertex1_no)->y ;
    df_x4 = get_vertex (ptrs_trap->s_line_l.vertex2_no)->x ;
    df_y4 = get_vertex (ptrs_trap->s_line_l.vertex2_no)->y ;
    df_cl = 0 ;
    if (FALSE == Check_if_Equal (df_y4, df_y1))
    {
      df_cl = (df_x4 - df_x1) / (df_y4 - df_y1) ;
    }
  }
  if (1 == ptrs_trap->b_line_right)
  {
    df_x2 = get_vertex (ptrs_trap->s_line_r.vertex1_no)->x ;
    df_y2 = get_vertex (ptrs_trap->s_line_r.vertex1_no)->y ;
    df_x3 = get_vertex (ptrs_trap->s_line_r.vertex2_no)->x ;
    df_y3 = get_vertex (ptrs_trap->s_line_r.vertex2_no)->y ;
    df_cr = 0 ;
    if (FALSE == Check_if_Equal (df_y2, df_y3))
    {
      df_cr = (df_x3 - df_x2) / (df_y3 - df_y2) ;
    }
  }

  if (i__hatch_pattern_no == 0)
  {
	  if (TRUE == Check_if_LT(df_y, df_ymax)  && (1 == ptrs_trap->b_line_left) && (1 == ptrs_trap->b_line_right))  //LE
	  {
		  SolidG.obiektt1 = 0;
		  SolidG.blok = 1;
		  SolidG.empty_typ = 0; // SolidFillTyp;
		  SolidG.pattern = SolidHatchPattern;
		  SolidG.kolor = Get_Current_Color();
		  SolidG.warstwa = Current_Layer;
		  SolidG.obiekt = Owwielokat;
		  SolidG.obiektt3 = O3HatchSolid;
		  SolidG.atrybut = LiniaG.atrybut;
		  SolidG.lp = 8;

		  if (SolidG.pattern == 1)
		  {
			  scale_ptr = (char *) & SolidG.xy;
			  scale_ptr += (SolidG.lp * sizeof(float));
			  dx_ptr = scale_ptr;
			  dx_ptr += sizeof(short int);
			  angle_ptr = dx_ptr;
			  angle_ptr += sizeof(short int);
			  dy_ptr = angle_ptr;
			  dy_ptr += sizeof(short int);
			  name_ptr = dy_ptr;
			  name_ptr += sizeof(short int);
			  memmove(scale_ptr, &SolidHatchPatternScale, sizeof(short int));
			  memmove(dx_ptr, &SolidHatchPatternDx, sizeof(short int));
			  memmove(angle_ptr, &SolidHatchPatternAngle, sizeof(short int));
			  memmove(dy_ptr, &SolidHatchPatternDy, sizeof(short int));
			  memmove(name_ptr, &SolidHatchPatternName, strlen(SolidHatchPatternName) + 1);

			  SolidG.n = 8 + SolidG.lp * sizeof(float) + 4 * sizeof(short int) + (int)strlen(SolidHatchPatternName) + 1;
		  }
		  else
		  {
			  SolidG.translucent = SolidHatchTranslucent;
			  if (SolidG.translucent == 0)
				  SolidG.n = 8 + SolidG.lp * sizeof(float);
			  else
			  {
				  translucency_ptr = (char *)SolidG.xy;
				  translucency_ptr += (SolidG.lp * sizeof(float));
				  memmove(translucency_ptr, &SolidHatchTranslucency, sizeof(unsigned char));

				  SolidG.n = 8 + SolidG.lp * sizeof(float) + sizeof(unsigned char);
			  }
		  }

		  SolidG.xy[0] = df_x1;
		  SolidG.xy[1] = df_y1;
		  SolidG.xy[2] = df_x2;
		  SolidG.xy[3] = df_y2;
		  SolidG.xy[4] = df_x3;
		  SolidG.xy[5] = df_y3;
		  SolidG.xy[6] = df_x4;
		  SolidG.xy[7] = df_y4;

		  dfymin = max(df_y1, df_y2);
		  dfymax = min(df_y3, df_y4);

		  if (dfymax > df_ymax) dfymax = df_ymax;

		  if (!((df_x1 > g_dfxmax) || (df_x2 < g_dfxmin)))
		  {
			  if (dfymin < g_dfymax)
			  {
				  if (dfymax > g_dfymax)
					  dfymin = g_dfymax;
			  }
		  }

		  //left edge
		  dx = df_x4 - df_x1;
		  dy = df_y4 - df_y1;
		  SolidG.xy[0] = df_x1 + (dfymin - df_y1) / dy * dx;
		  SolidG.xy[6] = df_x4 - (df_y4 - dfymax) / dy * dx;
		  SolidG.xy[1] = dfymin;
		  SolidG.xy[7] = dfymax;

		  //right edge
		  dx = df_x3 - df_x2;
		  dy = df_y3 - df_y2;
		  SolidG.xy[2] = df_x2 + (dfymin - df_y2) / dy * dx;
		  SolidG.xy[4] = df_x3 - (df_y3 - dfymax) / dy * dx;
		  SolidG.xy[3] = dfymin;
		  SolidG.xy[5] = dfymax;

		  g_dfymin = dfymin;
		  g_dfymax = dfymax;
		  g_dfxmin = SolidG.xy[6];
		  g_dfxmax = SolidG.xy[4];

		  if (Check_if_Equal(df_y1, df_y4) && Check_if_Equal(df_y2, df_y3)) return TRUE;

		  Normalize_Solid(&SolidG);

          PTR__GTMP6=(char*)global_dane;

		  global_dane_ob = dodaj_obiekt(global_dane, &SolidG);

          global_dane=(BLOK*)PTR__GTMP6;

		  if (NULL == global_dane_ob)
		  {
			  if (ERR_BLOCK_SIZE == Get_Error_No())
			  {
				  global_dane = (BLOK *)add_block(FALSE, NULL, Anormalny);
				  if (global_dane == NULL) return FALSE;
				  global_dane = (BLOK *)dodaj_obiekt((BLOK*)dane, &SolidG);
				  if (global_dane == NULL) return FALSE;
				  global_dane = (BLOK *)dane;
			  }
			  else
			  {
				  SolidG.empty_typ = SolidFillTyp;
				  return FALSE;
			  }
		  }

		   if (draw_line) rysuj_obiekt((char*)&SolidG, COPY_PUT, 1);

 		   SolidG.empty_typ = SolidFillTyp;
		   SolidG.pattern = 0;
		   return TRUE;

	  }
	  else return TRUE;
  }

 
  //while (TRUE == Check_if_LE (df_y, df_ymax))
  while (df_y <= df_ymax)
  {
    if (1 == ptrs_trap->b_line_left)
    {
      df_xl = df_x1 + (df_y - df_y1) * df_cl ;
    }
    else
    {
      ptrs_arc = &ptrs_trap->s_arc_l ;
	  if ((ptrs_arc->x > BIGNOF) || (ptrs_arc->x < -BIGNOF) || (ptrs_arc->y > BIGNOF) || (ptrs_arc->y < -BIGNOF))
		  //df_xl = 0.0;
		  goto ignore;
	  else if ((ptrs_arc->r < 0) || (ptrs_arc->r > BIGNOF))
		  //df_xl = 0.0;
		  goto ignore;
	  else
	  {
		  s_arc.r = ptrs_arc->r;
		  s_arc.x = ptrs_arc->x;
		  s_arc.y = ptrs_arc->y;
		  s_arc.kat1 = ptrs_arc->a1;
		  s_arc.kat2 = ptrs_arc->a2;
		  df_dd = ptrs_arc->r * ptrs_arc->r -
			  (df_y - ptrs_arc->y) * (df_y - ptrs_arc->y);
		  if (df_dd < 0)
		  {
			  df_dd = 0;
		  }
		  df_dd = sqrt(df_dd);
		  df_xl = ptrs_arc->x + df_dd;
		  if (FALSE == Point_Arc_Prec(df_xl, df_y, &s_arc, MPREC_1))
		  {
			  df_xl = ptrs_arc->x - df_dd;
		  }
	  }
    }
    if (1 == ptrs_trap->b_line_right)
    {
      df_xr = df_x2 + (df_y - df_y2) * df_cr ;
	  if (df_xr < -1E10)
	  {
		  df_xr = 0.0;
	  }
    }
    else
    {
		ptrs_arc = &ptrs_trap->s_arc_r;
		if ((ptrs_arc->x > BIGNOF) || (ptrs_arc->x < -BIGNOF) || (ptrs_arc->y > BIGNOF) || (ptrs_arc->y < -BIGNOF))
			//df_xr = 0.0;
			goto ignore;
		else if ((ptrs_arc->r < 0) || (ptrs_arc->r > BIGNOF))
			//df_xr = 0.0;
			goto ignore;
		else
		{
			s_arc.r = ptrs_arc->r;
			s_arc.x = ptrs_arc->x;
			s_arc.y = ptrs_arc->y;
			s_arc.kat1 = ptrs_arc->a1;
			s_arc.kat2 = ptrs_arc->a2;
			df_dd = ptrs_arc->r * ptrs_arc->r -
				(df_y - ptrs_arc->y) * (df_y - ptrs_arc->y);
			if (df_dd < 0)
			{
				df_dd = 0;
			}
			df_dd = sqrt(df_dd);
			df_xr = ptrs_arc->x + df_dd;
			if (df_xr < -1E10)
			{
				df_xr = 0.0;
			}
			if (FALSE == Point_Arc_Prec(df_xr, df_y, &s_arc, MPREC_1))
			{
				df_xr = ptrs_arc->x - df_dd;
				if (df_xr < -1E10)
				{
					df_xr = 0.0;
				}
			}
		}
    }
    df_dist_ = df_dist ;

	if (df_xr < -1E10)
	{
		df_xr = 0.0;
	}
	if ((df_xr - df_xl) < 0.0)  // TO CHECK
	{
		df__xl = df_xl;
		df_xl = df_xr;
		df_xr = df__xl;
	}
    if (FALSE == add_hatch_line (df_xl, df_xr, df_y, df_basex, df_basey,
	ptrs_hatch_param, ptrs_hatch_line_def, draw_line))
    {
      return FALSE ;
    }
ignore:
    df_y += df_dist ;

	if (interrupted()) return FALSE;
  }
  return TRUE ;
}



static BOOL
close_trap (int 		  i_trap_no,
	    T_PTR_Hatch_Param 	  ptrs_hatch_param,
	    T_PTR_Hatch_Line_Def  ptrs_hatch_line_def,
        int draw_line)
/*-----------------------------------------------------*/
{

  if (FALSE == hatch_trap (&ptrs__traps [i_trap_no],
			   ptrs_hatch_param, ptrs_hatch_line_def, NULL, draw_line))
  {
    return FALSE ;
  }
  if (i__trapsno > 1 && i__trapsno - 1 != i_trap_no)
  {
    memcpy (&ptrs__traps [i_trap_no],
	    &ptrs__traps [i__trapsno - 1],
	    sizeof (T_Trapizium)) ;
  }
  i__trapsno-- ;
  return TRUE ;
}

static void
change_trap_vertex_no (T_PTR_Trapizium ptrs_trap)
/*----------------------------------------------*/
{
  int i_ver1, i_ver2 ;
  double df_y1, df_y2 ;

  if (1 == ptrs_trap->b_line_left)
  {
    i_ver1 = ptrs_trap->s_line_l.vertex1_no ;
    i_ver2 = ptrs_trap->s_line_l.vertex2_no ;
    df_y1 = get_vertex (ptrs_trap->s_line_l.vertex1_no)->y ;
    df_y2 = get_vertex (ptrs_trap->s_line_l.vertex2_no)->y ;
    if (TRUE == Check_if_GT (df_y1, df_y2) ||
	(TRUE == Check_if_Equal (df_y1, df_y2) &&
	 get_vertex (i_ver1)->x > get_vertex (i_ver2)->x))
    {
      ptrs_trap->s_line_l.vertex1_no = i_ver2 ;
      ptrs_trap->s_line_l.vertex2_no = i_ver1 ;
    }
  }
  else
  {
    i_ver1 = ptrs_trap->s_arc_l.vertex1_no ;
    i_ver2 = ptrs_trap->s_arc_l.vertex2_no ;
    df_y1 = get_vertex (i_ver1)->y ;
    df_y2 = get_vertex (i_ver2)->y ;
    if (TRUE == Check_if_GT (df_y1, df_y2) ||
	(TRUE == Check_if_Equal (df_y1, df_y2) &&
	 get_vertex (i_ver1)->x > get_vertex (i_ver2)->x))
    {
      ptrs_trap->s_arc_l.vertex1_no = i_ver2 ;
      ptrs_trap->s_arc_l.vertex2_no = i_ver1 ;
    }
  }
  if (1 == ptrs_trap->b_line_right)
  {
    i_ver1 = ptrs_trap->s_line_r.vertex1_no ;
    i_ver2 = ptrs_trap->s_line_r.vertex2_no ;
    df_y1 = get_vertex (i_ver1)->y ;
    df_y2 = get_vertex (i_ver2)->y ;
    if (TRUE == Check_if_GT (df_y1, df_y2) ||
       (TRUE == Check_if_Equal (df_y1, df_y2) &&
	get_vertex (i_ver1)->x > get_vertex (i_ver2)->x))
    {
      ptrs_trap->s_line_r.vertex1_no = i_ver2 ;
      ptrs_trap->s_line_r.vertex2_no = i_ver1 ;
    }
  }
  else
  {
    i_ver1 = ptrs_trap->s_arc_r.vertex1_no ;
    i_ver2 = ptrs_trap->s_arc_r.vertex2_no ;
    df_y1 = get_vertex (i_ver1)->y ;
    df_y2 = get_vertex (i_ver2)->y ;
    if (TRUE == Check_if_GT (df_y1, df_y2) ||
       (TRUE == Check_if_Equal (df_y1, df_y2) &&
	get_vertex (i_ver1)->x > get_vertex (i_ver2)->x))
    {
      ptrs_trap->s_arc_r.vertex1_no = i_ver2 ;
      ptrs_trap->s_arc_r.vertex2_no = i_ver1 ;
    }
  }
}


static BOOL add_trap (T_PTR_Trapizium ptrs_trap_src)
/*--------------------------------------------*/
{
  T_PTR_Trapizium ptrs_trap ;

  if (i__trapsno >= i__trapsno_max)
  {
    ErrList (18) ;
    return FALSE ;
  }
  ptrs_trap = &ptrs__traps [i__trapsno++] ;
  memcpy (ptrs_trap, ptrs_trap_src, sizeof(T_Trapizium)) ;
  return TRUE ;
}


/*lokalne minimum*/
static BOOL
hatch_ver_stalt (T_PTR_Hatch_Param 	ptrs_hatch_param,
		 T_PTR_Hatch_Line_Def  	ptrs_hatch_line_def,
		 T_PTR_Trapizium ptrs_trap,
		 int i_ver_no, int draw_line)
/*-----------------------------------------------------*/
{
  T_Trapizium s_trap1, s_trap2  ;
  int i_trap_no ;
  BOOL b_ret ;
  T_PTR_Trapizium ptrs_traps ;

  ptrs_traps = ptrs__traps;
  if (-1  != (i_trap_no = check_ver_in_trap (i_ver_no)))
  {
    ptrs_traps [i_trap_no].vertex_up_no = i_ver_no ;
    memcpy (&s_trap1, &ptrs_traps [i_trap_no], sizeof (T_Trapizium)) ;
    memcpy (&s_trap2, &ptrs_traps [i_trap_no], sizeof (T_Trapizium)) ;
    memcpy (s_trap1.bank_r, ptrs_trap->bank_l, BANK_S) ;
    memcpy (s_trap2.bank_l, ptrs_trap->bank_r, BANK_S) ;
    s_trap1.vertex_dn_no = i_ver_no ;
    s_trap2.vertex_dn_no = i_ver_no ;
    s_trap1.b_line_right = ptrs_trap->b_line_left ;
    s_trap2.b_line_left= ptrs_trap->b_line_right ;
    b_ret = TRUE ;
    if (FALSE == close_trap (i_trap_no, ptrs_hatch_param ,ptrs_hatch_line_def, draw_line) ||
	FALSE == add_trap (&s_trap1) ||
	FALSE == add_trap (&s_trap2))
    {
      b_ret = FALSE ;
    }
  }
  else
  {
    ptrs_trap->vertex_dn_no = i_ver_no ;
    b_ret = add_trap (ptrs_trap) ;
  }
  return b_ret ;
}


/*lokalne maksimum*/
static BOOL
hatch_ver_stalg (T_PTR_Hatch_Param 	ptrs_hatch_param,
		 T_PTR_Hatch_Line_Def  	ptrs_hatch_line_def,
		 int 			i_ver,
         int draw_line)
/*-------------------------------------------------------*/
{
  T_PTR_Trapizium ptrs_traps ;
  T_Trapizium s_trap ;
  int i, i_ver_trap, i_trap_no ;
  int i_trap_l , i_trap_r, i_temp ;
  int key;

  i_trap_l = 1; 
  i_trap_r = 0;

  i_trap_no = 0 ;
  ptrs_traps = ptrs__traps;
  for (i = 0 ; i < i__trapsno ; i++)
  {
    i_ver_trap = get_trap_vertex_no (TRC_LU, &ptrs_traps [i]) ;
    if (TRUE == comp_vertex (i_ver_trap, i_ver))
    {
      ptrs_traps [i].vertex_up_no = i_ver ;
      i_ver_trap = get_trap_vertex_no (TRC_RU, &ptrs_traps [i]) ;
      memcpy (&s_trap.bank_r, &ptrs_traps [i].bank_r, BANK_S) ;
      s_trap.b_line_right = ptrs_traps [i].b_line_right ;
      s_trap.vertex_dn_no = i_ver ;
      i_trap_l = i ;
    if (TRUE == comp_vertex (i_ver_trap, i_ver))
      {
	return close_trap (i_trap_l, ptrs_hatch_param, ptrs_hatch_line_def, draw_line) ;
      }
      i_trap_no++ ;
      if (i_trap_no == 2)
      {
	if (i_trap_l < i_trap_r)
	{ 			/*przestawienie konieczne, efekt uboczny funkcji close_trap*/
	  i_temp = i_trap_l ;
	  i_trap_l = i_trap_r ;
	  i_trap_r = i_temp ;
	}
	if (TRUE == close_trap (i_trap_l, ptrs_hatch_param, ptrs_hatch_line_def, draw_line) &&
	    TRUE == close_trap (i_trap_r, ptrs_hatch_param, ptrs_hatch_line_def, draw_line) &&
	    TRUE == add_trap (&s_trap))
	  return TRUE ;
	else
	  return FALSE ;
      }
      continue ;
    }
    i_ver_trap = get_trap_vertex_no (TRC_RU, &ptrs_traps [i]) ;
    if (TRUE == comp_vertex (i_ver_trap, i_ver))
    {
      ptrs_traps [i].vertex_up_no = i_ver ;
      i_ver_trap = get_trap_vertex_no (TRC_LU, &ptrs_traps [i]) ;
      memcpy (&s_trap.bank_l, &ptrs_traps [i].bank_l, BANK_S) ;
      s_trap.b_line_left = ptrs_traps [i].b_line_left ;
      s_trap.vertex_dn_no = i_ver ;
      i_trap_r = i ;
      if (TRUE == comp_vertex (i_ver_trap, i_ver))
      {
	return close_trap (i_trap_r, ptrs_hatch_param, ptrs_hatch_line_def, draw_line) ;
      }
      i_trap_no++ ;
      if (i_trap_no == 2)
      {
	if (i_trap_l < i_trap_r)
	{ 			/*przestawienie konieczne, efekt uboczny funkcji close_trap*/
	  i_temp = i_trap_l ;
	  i_trap_l = i_trap_r ;
	  i_trap_r = i_temp ;
	}
	if (TRUE == close_trap (i_trap_l, ptrs_hatch_param, ptrs_hatch_line_def, draw_line) &&
	    TRUE == close_trap (i_trap_r, ptrs_hatch_param, ptrs_hatch_line_def, draw_line) &&
	    TRUE == add_trap (&s_trap))
	  return TRUE ;
	else
	  return FALSE ;
      }
      continue ;
    }

	if (interrupted()) return FALSE;
  }
  Internal_Error (__LINE__,__FILE__) ;
  return FALSE ;   //TO REMOVE ALREADY GENERATED PART OF HATCH
  //return TRUE ;  //TO CORRECT
}

BOOL check_vertex (int  i_vertex_no,
		   BOOL b_single,
		   int  i_vertex1_no,
		   int  i_vertex2_no,
		   int  *ptri_vertex_find_no)
/*------------------------------------------*/
{
  BOOL b_ret ;
  double df_x1, df_y1, df_x, df_y, df_x2, df_y2 ;

  b_ret = FALSE ;

  if (b_single == TRUE)
  {
    if (i_vertex1_no == i_vertex_no)
    {
      *ptri_vertex_find_no = i_vertex2_no ;
      b_ret = TRUE ;
    }
    else
    if (i_vertex2_no == i_vertex_no)
    {
      *ptri_vertex_find_no = i_vertex1_no ;
      b_ret = TRUE ;
    }
  }
  else
  {
    df_x1 = get_vertex (i_vertex1_no)->x ;
    df_y1 = get_vertex (i_vertex1_no)->y ;
    df_x2 = get_vertex (i_vertex2_no)->x ;
    df_y2 = get_vertex (i_vertex2_no)->y ;
    df_x = get_vertex (i_vertex_no)->x ;
    df_y = get_vertex (i_vertex_no)->y ;
    if (TRUE == Check_if_Equal (df_x1, df_x) &&
	TRUE == Check_if_Equal (df_y1, df_y))
    {
      *ptri_vertex_find_no = i_vertex2_no ;
      b_ret = TRUE ;
    }
    else
    if (TRUE == Check_if_Equal (df_x2, df_x) &&
	TRUE == Check_if_Equal (df_y2, df_y))
    {
      *ptri_vertex_find_no = i_vertex1_no ;
      b_ret = TRUE ;
    }
  }
  return b_ret ;
}


static BOOL
find_corner (int 	     i_vertex_no,
	     BOOL 	     b_single,
	     T_PTR_Trapizium ptrs_trap,
	     int 	     *i_ver,
	     double 	     *df_a)
/*-----------------------------------------*/
{
  T_PTR_ALine ptrs_lines ;
  T_PTR_AArc ptrs_arcs ;
  LUK s_arc ;
  int i_linesno, i_arcsno ;
  int i, i_find ;
  double df_x1, df_y1, df_x0, df_y0 ;
  BOOL b_dir ;
  int i_vertex_find_no ;

  ptrs_lines = ptrs__area0->s_lines_alloc.ptrs_lines ;
  i_linesno = ptrs__area0->s_lines_alloc.i_size ;
  ptrs_arcs = ptrs__area0->s_arcs_alloc.ptrs_arcs ;
  i_arcsno = ptrs__area0->s_arcs_alloc.i_size ;
  df_x0 = get_vertex (i_vertex_no)->x ;
  df_y0 = get_vertex (i_vertex_no)->y ;
  i = 0 ;
  i_find = 0 ;
  while (i < i_linesno && i_find < 2)
  {
    if (TRUE == check_vertex (i_vertex_no,
		   b_single,
		   ptrs_lines [i].vertex1_no,
		   ptrs_lines [i].vertex2_no,
		   &i_vertex_find_no))
    {
      i_ver [i_find] = i_vertex_find_no ;
      df_x1 = get_vertex (i_vertex_find_no)->x ;
      df_y1 = get_vertex (i_vertex_find_no)->y ;
      df_a [i_find] = Angle_Normal (atan2_my (df_y1 - df_y0, df_x1 - df_x0)) ;
      if (i_find == 0)
      {
	ptrs_trap->b_line_left = 1 ;
	memcpy (&ptrs_trap->s_line_l, &ptrs_lines [i], sizeof (T_ALine)) ;
      }
      else
      {
	ptrs_trap->b_line_right = 1 ;
	memcpy (&ptrs_trap->s_line_r, &ptrs_lines [i], sizeof (T_ALine)) ;
      }
      i_find++ ;
    }
    i++ ;
  }
  i = 0 ;
  if (i_find < 2)
  {
    while (i < i_arcsno && i_find < 2)
    {
      if (TRUE == check_vertex (i_vertex_no,
		     b_single,
		     ptrs_arcs [i].vertex1_no,
		     ptrs_arcs [i].vertex2_no,
		     &i_vertex_find_no))
      {
	i_ver [i_find] = i_vertex_find_no ;
	df_x1 = get_vertex (i_vertex_find_no)->x ;
	df_y1 = get_vertex (i_vertex_find_no)->y ;
	s_arc.r = ptrs_arcs [i].r ;
	s_arc.x = ptrs_arcs [i].x ;
	s_arc.y = ptrs_arcs [i].y ;
	s_arc.kat1 = ptrs_arcs [i].a1 ;
	s_arc.kat2 = ptrs_arcs [i].a2 ;
	b_dir = ptrs_arcs [i].b_dir ;
	if (ptrs_arcs [i].vertex1_no == i_vertex_find_no)
	{
	  b_dir = !b_dir ;
	}
	Get_Tangent_Ob (&s_arc, df_x0, df_y0, &df_x1, &df_y1, b_dir) ;
	df_a [i_find] = Angle_Normal (atan2_my (df_y1 - df_y0, df_x1 - df_x0)) ;
	if (i_find== 0)
	{
	  ptrs_trap->b_line_left = 0 ;
	  memcpy (&ptrs_trap->s_arc_l, &ptrs_arcs [i], sizeof (T_AArc)) ;
	}
	else
	{
	  ptrs_trap->b_line_right = 0 ;
	  memcpy (&ptrs_trap->s_arc_r, &ptrs_arcs [i], sizeof (T_AArc)) ;
	}
	i_find++ ;
      }
    i++ ;
    }
  }
  if (i_find < 2)
  {
    return FALSE ;	/*dla wezlow krawedzi wspolnych dzior*/
  }
  return TRUE ;
}

static double get_arc_x (T_PTR_AArc ptrs_arc,
			 double	    df_y)
/*-----------------------------------------*/
{
  LUK s_arc ;
  double df_xret, df_dd ;

  s_arc.r = ptrs_arc->r ;
  s_arc.x = ptrs_arc->x ;
  s_arc.y = ptrs_arc->y ;
  s_arc.kat1 = ptrs_arc->a1 ;
  s_arc.kat2 = ptrs_arc->a2 ;
  df_dd = ptrs_arc->r * ptrs_arc->r -
	  (df_y - ptrs_arc->y) * (df_y - ptrs_arc->y) ;
  if (df_dd < 0)
  {
    df_dd = 0 ;
  }
  df_dd = sqrt (df_dd) ;
  df_xret = ptrs_arc->x + df_dd ;
  if (FALSE == Point_Arc_Prec (df_xret, df_y, &s_arc, MPREC_1))
  {
    df_xret = ptrs_arc->x - df_dd ;
  }
  return df_xret ;
}

static BOOL change_trap_branch (T_PTR_Trapizium ptrs_trap,
				double 		df_y,
				int 		i_vertex_no)
/*--------------------------------------------------------*/
{
  double df_xl, df_xr ;
  int i_ver1, i_ver2 ;

  if (TRUE == ptrs_trap->b_line_left)
  {
    i_ver1 = ptrs_trap->s_line_l.vertex1_no ;
    i_ver2 = ptrs_trap->s_line_l.vertex2_no ;
    df_xl = get_vertex (i_vertex_no)->x ;
    if (FALSE == Check_if_Equal (get_vertex (i_ver1)->y, get_vertex (i_ver2)->y))
    {
      df_xl = get_vertex (i_vertex_no)->x +
		(df_y - get_vertex (i_vertex_no)->y) *
		(get_vertex (i_ver1)->x - get_vertex (i_ver2)->x) /
		(get_vertex (i_ver1)->y - get_vertex (i_ver2)->y) ;
    }
  }
  else
  {
    df_xl = get_arc_x (&ptrs_trap->s_arc_l, df_y) ;
  }
  if (TRUE == ptrs_trap->b_line_right)
  {
    i_ver1 = ptrs_trap->s_line_r.vertex1_no ;
    i_ver2 = ptrs_trap->s_line_r.vertex2_no ;
    df_xr = get_vertex (i_vertex_no)->x ;
    if (FALSE == Check_if_Equal (get_vertex (i_ver1)->y, get_vertex (i_ver2)->y))
    {
      df_xr = get_vertex (i_vertex_no)->x +
		(df_y - get_vertex (i_vertex_no)->y) *
		(get_vertex (i_ver1)->x - get_vertex (i_ver2)->x) /
		(get_vertex (i_ver1)->y - get_vertex (i_ver2)->y) ;
    }
  }
  else
  {
    df_xr = get_arc_x (&ptrs_trap->s_arc_r, df_y) ;
  }
  return (df_xl > df_xr ? TRUE : FALSE) ;
}


static int find_trap (int i_vertex_no,
		      BOOL b_single,
		      T_PTR_Trapizium ptrs_trap)
/*---------------------------------------------*/
{
  T_Trapizium s_trap ;
  int i_ver [2], i_ret ;
  double df_x1, df_y1, df_x0, df_y0, df_x2, df_y2 ;
  double df_a [2], df_a1, df_a2 ;
  BOOL b_change ;

  df_x0 = get_vertex (i_vertex_no)->x ;
  df_y0 = get_vertex (i_vertex_no)->y ;
  if (FALSE == find_corner (i_vertex_no, b_single, ptrs_trap, i_ver, df_a))
  {
    return VT_UNKNOWN1 ;
  }
  df_x1 = get_vertex (i_ver [0])->x ;
  df_y1 = get_vertex (i_ver [0])->y ;
  df_x2 = get_vertex (i_ver [1])->x ;
  df_y2 = get_vertex (i_ver [1])->y ;
  df_a1 = df_a [0] ;
  df_a2 = df_a [1] ;
  b_change = FALSE ;
  if ((TRUE == Check_if_GT (df_y1, df_y0) &&
	TRUE == Check_if_GT (df_y2, df_y0))         ||
     (TRUE == Check_if_Equal (df_y1, df_y0) &&
	TRUE == Check_if_GT (df_y2, df_y0)  &&
	TRUE == Check_if_GT (df_x1, df_x0))         ||
     (TRUE == Check_if_Equal (df_y2, df_y0) &&
	TRUE == Check_if_GT (df_y1, df_y0)  &&
	TRUE == Check_if_GT (df_x2, df_x0)))
  {
    i_ret = VT_stalaktyt ;
    if (fabs (df_a1 - Pi2) < Get_Prec (MPREC_2, 1, 1))
    {
      df_a1 = 0 ;
    }
    if (fabs (df_a2 - Pi2) < Get_Prec (MPREC_2, 1, 1))
    {
      df_a2 = 0 ;
    }
    if (df_a2 > df_a1 + Get_Prec (MPREC_4, 1, 1) ||
     TRUE == change_trap_branch (ptrs_trap, min (df_y1, df_y2), i_vertex_no))
    {
      b_change = TRUE ;
    }
  }
  else
  if ((TRUE == Check_if_LT (df_y1, df_y0) &&
	TRUE == Check_if_LT (df_y2, df_y0))        ||
     (TRUE == Check_if_Equal (df_y1, df_y0) &&
	TRUE == Check_if_LT (df_y2, df_y0)  &&
	TRUE == Check_if_LT (df_x1, df_x0))        ||
     (TRUE == Check_if_Equal (df_y2, df_y0) &&
	TRUE == Check_if_LT (df_y1, df_y0)  &&
	TRUE == Check_if_LT (df_x2, df_x0)))
  {
    i_ret = VT_stalagmit ;
    if (fabs (df_a1) < Get_Prec (MPREC_2, 1, 1))
    {
      df_a1 = Pi2 ;
    }
    if (fabs (df_a2) < Get_Prec (MPREC_2, 1, 1))
    {
      df_a2 = Pi2 ;
    }
    if (df_a1 > df_a2 + Get_Prec (MPREC_4, 1, 1) ||
     TRUE == change_trap_branch (ptrs_trap, max (df_y1, df_y2), i_vertex_no))
    {
      b_change = TRUE ;
    }
  }
  else
  {
    i_ret = VT_REGULAR ;
    if (TRUE == Check_if_GT (df_y1, df_y2))
    {
      b_change = TRUE ;
    }
    else
    if (TRUE == Check_if_Equal (df_y1, df_y2) &&
	TRUE == Check_if_GT (df_x1, df_x2))
    {
      b_change = TRUE ;
    }
			    /*1 seg nizej, 2 wyzej*/
				    /* 2 */
				    /* > */
				    /* 1 */
  }
  if (b_change == TRUE)
  {
     memcpy (&s_trap, ptrs_trap, sizeof (T_Trapizium)) ;
     ptrs_trap->b_line_left = s_trap.b_line_right ;
     ptrs_trap->b_line_right = s_trap.b_line_left ;
     memcpy (ptrs_trap->bank_l, s_trap.bank_r, BANK_S) ;
     memcpy (ptrs_trap->bank_r, s_trap.bank_l, BANK_S) ;
  }
  change_trap_vertex_no (ptrs_trap) ;
  return i_ret ;
}


static char *add_block(BOOL hatchoutline, BLOK *adb, int atrybut)
//---------------------------------------------------------------
{
  BLOK s_blockd = Bdef ;
  BLOK *ptrs_block, *buf_block;
  char c_pltype ;
  char blok_type[31];
  BLOKD blokd = BDdef;
  unsigned size_block = B3;

  int  len_type, len_desc;

  T_Desc_Ex_Block 	*ptrs_desc_bl;

  s_blockd.atrybut=atrybut;

  if (hatchoutline == TRUE)
  {
	  strcpy(blok_type, "");
	  len_type = (int)strlen(blok_type) + 1;
	  len_desc = sizeof(unsigned) + 2 * sizeof(float) + sizeof(len_type) + len_type;
	  size_block += len_desc;

	  if (NULL == (buf_block = (BLOK*)malloc /*getmem*/(sizeof(NAGLOWEK) + size_block)))
	  {
		  return NULL;
	  }
	  memcpy(buf_block, &blokd, sizeof(blokd));
	  buf_block->n = size_block;
	  buf_block->kod_obiektu = B_HATCH;
	  buf_block->dlugosc_opisu_obiektu = len_desc;
	  buf_block->obiektt2 = 0;

      if (adb!=NULL) buf_block->blok=1;

	  ptrs_desc_bl = (T_Desc_Ex_Block *)(&buf_block->opis_obiektu[0]);
	  ptrs_desc_bl->flags = EBF_IP;
	  ptrs_desc_bl->x = df_pointx_gl;
	  ptrs_desc_bl->y = df_pointy_gl;
	  ptrs_desc_bl->len = len_type;
	  strcpy(&ptrs_desc_bl->sz_type[0], blok_type);

	  ptrs_block = (BLOK*)dodaj_obiekt(adb, buf_block);

      free(buf_block);
	  
	  return (char *)ptrs_block;
  }
  else
  {
	  s_blockd.kod_obiektu = B_HATCH;
	  s_blockd.n = B3 + sizeof(c_pltype);
	  s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype);
	  if ((ptrs_block = (BLOK*)dodaj_obiekt(adb, &s_blockd)) != NULL)
	  {
		  c_pltype = PL_OTHER;
		  memmove(&(ptrs_block->opis_obiektu[0]), &c_pltype, sizeof(c_pltype));
	  }
	  return (char *)ptrs_block;
  }
  
}

char *add_block_hatch(BOOL hatchoutline)
{
    return add_block(hatchoutline, NULL, Ablok);
}

char *add_block_hatch_in_block(BOOL hatchoutline)
{
    char *ptrs_block ;
    ptrs_block = add_block(hatchoutline, (BLOK*)dane, Ablok);
    global_block=(char *) ptrs_block - dane;
    return ptrs_block;
}


static BOOL get_mem_trap_table (void)
/*----------------------------------*/
{
  long l_free ;
  BOOL b_ret ;

  b_ret = FALSE ;
  l_free = get_free_mem_size (1) ;
  i__trapsno_max = (int)((l_free - 10000) / sizeof (T_Trapizium)) ;    /*1000 - rezerwa */
  if (i__trapsno_max>TRAPSNO_MAX) i__trapsno_max=TRAPSNO_MAX;
  if (i__trapsno_max > 10)
  {
	ptrs__traps = (T_PTR_Trapizium)malloc(i__trapsno_max * sizeof(T_Trapizium));
    for (int i=0; i<i__trapsno_max; i++)
    {
        ptrs__traps[i].vertex_up_no=0;
        ptrs__traps[i].vertex_dn_no=0;

        ptrs__traps[i].s_line_l.vertex1_no=0;
        ptrs__traps[i].s_line_l.vertex2_no=0;
        ptrs__traps[i].s_line_l.b_hole=0;

        ptrs__traps[i].s_line_r.vertex1_no=0;
        ptrs__traps[i].s_line_r.vertex2_no=0;
        ptrs__traps[i].s_line_r.b_hole=0;

        ptrs__traps[i].s_arc_l.vertex1_no=0;
        ptrs__traps[i].s_arc_l.vertex2_no=0;
        ptrs__traps[i].s_arc_l.b_hole=0;

        ptrs__traps[i].s_arc_r.vertex1_no=0;
        ptrs__traps[i].s_arc_r.vertex2_no=0;
        ptrs__traps[i].s_arc_r.b_hole=0;
    }
    b_ret = TRUE ;
  }
  else
  {
    ErrList (18) ;
  }
  return b_ret ;
}

static BOOL arcs_copy (void)
/*-------------------------*/
{
  int i_arcsno, i_verno ;
  BOOL b_ret = FALSE ;

  i_verno = ptrs__area0->s_vertexs_alloc.i_size ;
  if (0 == (i_arcsno = ptrs__area0->s_arcs_alloc.i_size))
  {
    return TRUE ;
  }
  if (TRUE == h_realloc ((T_PTR_Units)&ptrs__area0->s_arcs_cpy_alloc, i_arcsno) &&
      TRUE == h_realloc ((T_PTR_Units)&ptrs__area0->s_vertexs_cpy_alloc, i_verno))
  {
    memcpy (ptrs__area0->s_arcs_cpy_alloc.ptrs_arcs,
	    ptrs__area0->s_arcs_alloc.ptrs_arcs,
	    i_arcsno * sizeof (T_AArc)) ;
    memcpy (ptrs__area0->s_vertexs_cpy_alloc.ptrs_vertexs,
	    ptrs__area0->s_vertexs_alloc.ptrs_vertexs,
	    i_verno * sizeof (T_Vertex)) ;
    ptrs__area0->s_arcs_cpy_alloc.i_size = i_arcsno ;
    ptrs__area0->s_vertexs_cpy_alloc.i_size = i_verno ;
    b_ret = TRUE ;
  }
  return b_ret ;
}


static void
arcs_rot_copy (void)
/*-----------------*/
{
  int i_arcsno, i_verno ;

  if (0 == (i_arcsno = ptrs__area0->s_arcs_cpy_alloc.i_size))
  {
    return ;
  }
  i_verno = ptrs__area0->s_vertexs_cpy_alloc.i_size ;
  memcpy (ptrs__area0->s_vertexs_alloc.ptrs_vertexs,
	  ptrs__area0->s_vertexs_cpy_alloc.ptrs_vertexs,
	  i_verno * sizeof (T_Vertex)) ;
  ptrs__area0->s_vertexs_alloc.i_size = i_verno ;
}

static BOOL
add_arc0 (T_PTR_AArc ptrs_arc)
/*------------------------------*/
{
  T_PTR_AArc_Alloc ptrs_arcs_alloc ;
  int i_size ;

  ptrs_arcs_alloc = &ptrs__area0->s_arcs_alloc ;
  if (FALSE == h_realloc ((T_PTR_Units)ptrs_arcs_alloc))
  {
    return FALSE ;
  }
  i_size = ptrs_arcs_alloc->i_size ;
  ptrs_arcs_alloc->i_size++ ;
  memcpy (&ptrs_arcs_alloc->ptrs_arcs [i_size], ptrs_arc, sizeof (T_AArc)) ;
  return TRUE ;
}

static BOOL
add_arc (T_PTR_AArc ptrs_arc, double df_a)
/*-------------------------------------------*/
{
  double  df_x, df_y, df_a2 ;
  int i_ver1_no, i_ver2_no, i_ver_no ;

  i_ver1_no = ptrs_arc->vertex1_no ;
  i_ver2_no = ptrs_arc->vertex2_no ;
  df_a2 = ptrs_arc->a2 ;
  df_x = ptrs_arc->x + ptrs_arc->r * cos (df_a) ;
  df_y = ptrs_arc->y + ptrs_arc->r * sin (df_a) ;
  ptrs_arc->a2 = df_a ;
  if (-1 == (i_ver_no = add_vertex (df_x, df_y)))
  {
    return FALSE ;
  }
  if (TRUE == ptrs_arc->b_dir)
  {
    ptrs_arc->vertex2_no = i_ver_no ;
  }
  else
  {
    ptrs_arc->vertex1_no = i_ver_no ;
  }
  if (FALSE == add_arc0 (ptrs_arc))
  {
    return FALSE ;
  }
  ptrs_arc->a1 = df_a ;
  ptrs_arc->a2 = df_a2 ;
  if (TRUE == ptrs_arc->b_dir)
  {
    ptrs_arc->vertex1_no = i_ver_no ;
    ptrs_arc->vertex2_no = i_ver2_no ;
  }
  else
  {
    ptrs_arc->vertex1_no = i_ver1_no ;
    ptrs_arc->vertex2_no = i_ver_no ;
  }
  return TRUE ;
}

static BOOL break_up_arcs (void)
/*-----------------------------*/
{
  int i_arcsno, i ;
  T_PTR_AArc ptrs_arcs ;
  T_AArc s_arc ;

  if (0 == (i_arcsno = ptrs__area0->s_arcs_cpy_alloc.i_size))
  {
    return TRUE ;
  }
  ptrs__area0->s_arcs_alloc.i_size = 0 ;
  for (i = 0 ; i < i_arcsno ; i++)
  {
    ptrs_arcs = ptrs__area0->s_arcs_cpy_alloc.ptrs_arcs ;
    memcpy (&s_arc, &ptrs_arcs [i], sizeof (T_AArc)) ;

    if (TRUE == Check_Angle (s_arc.a1, Pi2 * 3. / 4, Pi2 / 4) &&
	TRUE == Sharp_Check_Angle (Pi2 / 4,  s_arc.a1, s_arc.a2))
    {
       if (FALSE == add_arc (&s_arc, Pi2 / 4))
       {
	 return FALSE ;
       }
    }
    if (TRUE == Check_if_LT (s_arc.a1, Pi2 * 3. / 4) &&
	TRUE == Sharp_Check_Angle (Pi2 * 3. / 4,  s_arc.a1, s_arc.a2))
    {
       if (FALSE == add_arc (&s_arc, Pi2 * 3. / 4))
       {
	 return FALSE ;
       }
    }
    if (TRUE == Check_Angle (s_arc.a1, Pi2 * 3. / 4, Pi2 / 4) &&
	TRUE == Sharp_Check_Angle (Pi2 / 4,  s_arc.a1, s_arc.a2))
    {
      if (FALSE == add_arc (&s_arc, Pi2 / 4))
      {
	return FALSE ;
      }
    }
    if (FALSE == add_arc0 (&s_arc))
    {
      return FALSE ;
    }
  }

  return TRUE;
}

extern "C" {
BOOL Make_Hatch (T_PTR_Hatch_Param ptrs_hatch_param0,
	    T_PTR_Area ptrs_area, int comput_area,
	    double df_apx1, double df_apy1, double df_apx2, double df_apy2, double df_pointx, double df_pointy, /*void *abd */ long_long le_abd, int to_block, int draw_line)
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------*/
{
    T_PTR_Hatch_Def_Param ptrs_hatch_defs_param;
    T_PTR_Hatch_Line_Def ptrs_hatch_line_def;
    T_Trapizium s_trap;
    int i_verno;
    int i, j, i_ver_type, i_ver_no;
    int i_number_hatch;    /*numer wzoru wypelnienia*/
    int i_hatch_lineno;    /*liczba linii definiujacych wzor*/
    static double df_cur_angle;        /*aktualny kat obrotu obiektow obszaru*/
    BOOL b_ret, b_single;
    long l_dane_size;
    double x_s, y_s, x_p, y_p;
    double delta_x, delta_y, delta_l, delta_l1, delta_l2;
    double alfa; /*kat obrotu ukladu wspolrzednych*/
    double zmax, zmax1;
    LINIA LiniaG1;
    int key;


    if (le_abd < 0) global_dane = (BLOK *) dane;
    else global_dane = (BLOK *) (dane + dane_size - le_abd);


    df_pointx_gl = df_pointx;
    df_pointy_gl = df_pointy;

    T_PTR_Hatch_Param ptrs_hatch_param;
    T_Hatch_Param hatch_param;

    memcpy(&hatch_param, ptrs_hatch_param0, sizeof(T_Hatch_Param));
    ptrs_hatch_param = &hatch_param;

    if (ptrs_hatch_param->i_number_hatch == 0)        /*numer wzoru wypelnienia w tablicy definicji wypelnien*/
    {
        ptrs_hatch_param->df_angle = 0;        /*kat pochylenia linii (w radianach !!!)*/
        ptrs_hatch_param->df_scale = 1.0;        /*wspolczynnik skali*/
        ptrs_hatch_param->df_basex = 0.0;
        ptrs_hatch_param->df_basey = 0.0;
    }

    pole_hatch = 0;
    moment = 0;
    mom_stat_x = 0;
    mom_stat_y = 0;
    mom_bezwl_x = 0;
    mom_bezwl_y = 0;
    mom_dewiacji = 0;
    wskazn_wytrz_x = 0;
    wskazn_wytrz_y = 0;
    min_xw = 0;
    max_xw = 0;
    min_yw = 0;
    max_yw = 0;

    g_dfymax = -1E10;
    g_dfymin = 1E10;
    g_dfymax = -1E10;
    g_dfymin = 1E10;

    comput_area1 = comput_area;
    if (comput_area1 == 10) comput_area1 = 0;

    l_dane_size = dane_size;
    b_ret = FALSE;
    i__trapsno = 0;
    ptrs__traps = NULL;
    ptri__tab_ver_no = NULL;
    df_cur_angle = 0;
    ptrs__area0 = ptrs_area;

    i__hatch_pattern_no = ptrs_hatch_param->i_number_hatch;

    if (i__hatch_pattern_no == 999) {
        ptrs_hatch_defs_param = Get_Hatch_Def_Table999();
        ptrs_hatch_line_def = Get_Hatch_Line_Def_Table999();
        i_number_hatch = ptrs_hatch_param->i_number_hatch;
    } else {
        ptrs_hatch_defs_param = Get_Hatch_Def_Table();
        ptrs_hatch_line_def = Get_Hatch_Line_Def_Table();
        i_number_hatch = ptrs_hatch_param->i_number_hatch;
        ptrs_hatch_line_def += ptrs_hatch_defs_param->i_number_line;// [i_number_hatch]
    }

    i_hatch_lineno = ptrs_hatch_defs_param->i_size;   /*[i_number_hatch]*/

    if (comput_area != 10) {
        if (to_block == 1) global_dane = (BLOK *) add_block(TRUE, (BLOK *) dane, Anormalny);
        else
            global_dane = (BLOK *) add_block(TRUE, NULL, Anormalny);
    }


    if (NULL == global_dane ||
        FALSE == arcs_copy()) {
        return FALSE;
    }

    if (FALSE == get_mem_trap_table()) {
        goto aa;
    }
    InfoList(3);

    if (((comput_area == 0) /*|| (comput_area == 10)*/) && (i_hatch_lineno > 0))
    {

        get_draw_outline(ptrs_hatch_param, TRUE, draw_line);

    } else if ((comput_area == 10) && (ptrs_hatch_param->insulation_hatch == TRUE))
    {
        get_draw_outline(ptrs_hatch_param, FALSE, draw_line);
    }
    //else if ((comput_area > 0) && (comput_area<6))
    //{
    //   get_range_outline(ptrs_hatch_param);
    //}

    for (j = 0; j < i_hatch_lineno; j++) {
        pole_hatch_add = j;
        if (i__trapsno != 0) {
            Internal_Error(__LINE__, __FILE__);
            key = Get_Legal_Key("\033\015");
            komunikat(0);
            goto aa;
            ////continue;  //TO CORRECT
        }
        df_cur_angle = ptrs_hatch_line_def[j].df_angle + ptrs_hatch_param->df_angle;
        arcs_rot_copy();
        rotation(-df_cur_angle, ptrs_hatch_param);
        if (FALSE == break_up_arcs()) {
            goto aa;
        }
        h_free((void *) ptri__tab_ver_no);
        i_verno = ptrs__area0->s_vertexs_alloc.i_size;
        if (FALSE == h_alloc((char **) &ptri__tab_ver_no,
                             sizeof(int) * i_verno)) {
            goto aa;
        }
        sort_vertexs();
        for (i = 0; i < ptrs__area0->s_vertexs_alloc.i_size; i++) {

            i_ver_no = ptrs__area0->s_vertexs_alloc.ptrs_vertexs[i].vertex_no;
            if (TRUE == get_vertex(i_ver_no)->b_equel) {
                continue;
            }
            b_single = get_vertex(i_ver_no)->b_single;
            i_ver_type = find_trap(i_ver_no, b_single, &s_trap);

            switch (i_ver_type) {
                case VT_REGULAR :
                    if (FALSE == hatch_ver_reg(ptrs_hatch_param,
                                               &ptrs_hatch_line_def[j],
                                               &s_trap, i_ver_no, draw_line)) {
                        goto aa;
                    }
                    break;
                case VT_stalaktyt :
                    if (FALSE == hatch_ver_stalt(ptrs_hatch_param,
                                                 &ptrs_hatch_line_def[j],
                                                 &s_trap, i_ver_no, draw_line)) {
                        goto aa;
                    }
                    break;
                case VT_stalagmit :
                    if (FALSE == hatch_ver_stalg(ptrs_hatch_param,
                                                 &ptrs_hatch_line_def[j],
                                                 i_ver_no, draw_line)) {
                        goto aa;
                    }
                    break;
                default :
                    break;
            }
        }
        rotation(df_cur_angle, ptrs_hatch_param);

        if (interrupted()) {
            goto aa;
        }
    }

    b_ret = TRUE;
    aa :
    if (b_ret == FALSE) {
        while (l_dane_size < dane_size) {
            blokzap(dane, dane + ((NAGLOWEK *) dane)->n - 1, Anormalny, COPY_PUT, 0);
            Usun_Object((void *) dane, TRUE);
        }
    }
    h_free((void *) ptri__tab_ver_no);
    free(ptrs__traps);

    if ((comput_area > 0) && (comput_area < 10)) {
        CUR_OFF(X, Y);
        if (comput_area == 1)  // pole
        {
            if (out_pole_ignore == FALSE) out_pole(pole_hatch);
            else ret_pole(pole_hatch);
        } else {
            x_s = mom_stat_y / pole_hatch;
            y_s = mom_stat_x / pole_hatch;
            if (comput_area == 2) // srodek ciezkosci
            {
                Add_Point1(x_s, y_s);
                out_srodek_ciezkosci(x_s, y_s);
            } else if (comput_area == 3) // moment statyczny
            {
                x_p = x_s;
                y_p = y_s;
                LiniaG.x1 = x_s;
                LiniaG.y1 = y_s;
                LiniaG1.x1 = df_apx1;
                LiniaG1.y1 = df_apy1;
                LiniaG1.x2 = df_apx2;
                LiniaG1.y2 = df_apy2;

                poczL = 1;
                prostopadleL_(&x_p, &y_p, &LiniaG1);
                poczL = 0;
                delta_x = x_p - x_s;
                delta_y = y_p - y_s;
                ////delta_l = milimetryob(sqrt((delta_x * delta_x) + (delta_y * delta_y)));
                delta_l = sqrt((delta_x * delta_x) + (delta_y * delta_y));
                mom_stat_x = pole_hatch * delta_l;
                out_moment_statyczny(mom_stat_x);
            } else if (comput_area == 4) // moment bezwladnosci
            {
                //moment dla ukladu obroconego o kat alfa
                //wyznaczenie kata alfa
                if (TRUE == Check_if_Equal(df_apx2, df_apx1)) {
                    alfa = 1.570796327;  /* 0.5*pi */
                } else alfa = atan((df_apy2 - df_apy1) / (df_apx2 - df_apx1));
                mom_bezwl = (mom_bezwl_x * cos(alfa) * cos(alfa)) + (mom_bezwl_y * sin(alfa) * sin(alfa)) -
                            (mom_dewiacji * sin(2 * alfa));
                //moment dla ukladu przesunietego o deltay
                // odleglosc 1
                x_p = x_s;  //srodek ciezkosci
                y_p = y_s;
                LiniaG.x1 = x_s;
                LiniaG.y1 = y_s;
                // linia rownolegla do zadanej przechodzaca przez punkt 0,0
                LiniaG1.x1 = 0;
                LiniaG1.y1 = 0;
                if (TRUE == Check_if_Equal(df_apx2, df_apx1)) {
                    LiniaG1.x2 = 0;
                    LiniaG1.y2 = 100;   //przykladowo
                } else {
                    LiniaG1.x2 = 100;
                    LiniaG1.y2 = (df_apy2 - df_apy1) / (df_apx2 - df_apx1) * 100; //przykladowo
                }
                poczL = 1;
                prostopadleL_(&x_p, &y_p, &LiniaG1);
                poczL = 0;
                delta_x = x_p - x_s;
                delta_y = y_p - y_s;
                ////delta_l1 = milimetryob(sqrt((delta_x * delta_x) + (delta_y * delta_y)));
                delta_l1 = sqrt((delta_x * delta_x) + (delta_y * delta_y));
                // odleglosc 2

                x_p = x_s;
                y_p = y_s;
                LiniaG.x1 = x_s;              // srodek ciezkosci
                LiniaG.y1 = y_s;
                LiniaG1.x1 = df_apx1;         //  zadana linia
                LiniaG1.y1 = df_apy1;
                LiniaG1.x2 = df_apx2;
                LiniaG1.y2 = df_apy2;

                poczL = 1;
                prostopadleL_(&x_p, &y_p, &LiniaG1);
                poczL = 0;
                delta_x = x_p - x_s;
                delta_y = y_p - y_s;
                ////delta_l2 = milimetryob(sqrt((delta_x * delta_x) + (delta_y * delta_y)));
                delta_l2 = sqrt((delta_x * delta_x) + (delta_y * delta_y));

                delta_l = (delta_l2 - delta_l1);
                moment = mom_bezwl + (2 * pole_hatch * delta_l1 * delta_l) + (pole_hatch * delta_l * delta_l);
                mom_bezwl = moment;
                out_moment_bezwladnosci(mom_bezwl);
            } else if (comput_area == 5) /* centralne momenty bezwladnosci i wskazniki wytrzymalosci */
            {
                //moment dla ukladu przesunietego o y_s
                // odleglosc 1
                x_p = x_s;  //srodek ciezkosci
                y_p = y_s;
                ////delta_l1 = milimetryob(y_s);
                delta_l1 = y_s;
                ////delta_l = milimetryob(-y_s);
                delta_l = -y_s;
                moment = mom_bezwl_x + (2 * pole_hatch * delta_l1 * delta_l) + (pole_hatch * delta_l * delta_l);
                mom_bezwl_x = moment;
                ////delta_l1 = milimetryob(x_s);
                delta_l1 = x_s;
                ////delta_l = milimetryob(-x_s);
                delta_l = -x_s;
                moment = mom_bezwl_y + (2 * pole_hatch * delta_l1 * delta_l) + (pole_hatch * delta_l * delta_l);
                mom_bezwl_y = moment;
                zmax = fabs(max_yw - y_s);
                zmax1 = fabs(min_yw - y_s);
                if (zmax1 > zmax) zmax = zmax1;
                ////if (zmax > 0) wskazn_wytrz_x = mom_bezwl_x / (milimetryob(zmax));
                if (zmax > 0) wskazn_wytrz_x = mom_bezwl_x / zmax;
                ////zmax = fabs(max_xw - milimetryob(x_s));
                zmax = fabs(max_xw - x_s);
                ////zmax1 = fabs(min_xw - milimetryob(x_s));
                zmax1 = fabs(min_xw - x_s);
                if (zmax1 > zmax) zmax = zmax1;
                if (zmax > 0) wskazn_wytrz_y = mom_bezwl_y / zmax;
                out_centr_moment_bezwl(mom_bezwl_x, mom_bezwl_y, wskazn_wytrz_x, wskazn_wytrz_y);
            }


        }
        CUR_ON(X, Y);
    } /* comput_area>0 */
    return b_ret;
}
}


#define __O_HATCH3__