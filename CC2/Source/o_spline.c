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

#define __O_SPLINE__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "o_object.h"

#include "menu.h"

#include "leak_detector_c.h"

static char *add_block(char b_type, char cltype);

extern double Grid_to_Rad(double angle);
extern int Set_WidthL_to_Type(int type, int width);
extern int Get_Current_Color(void);
extern void Normalize_Spline(SPLINE *);
extern void setwritemode(int mode);
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);
extern void orto_l(LINIA *L, int *Orto_Dir);
extern void view_line_type(LINIA *L);
extern double get_last_bnods_direction(SPLINE *ptr_spline);
extern double get_first_bnods_direction(SPLINE* ptr_spline);
extern void calc_tangents(int closed, int* node_count, double* bnodes_2_tangent);
extern void set_bnodes(SPLINE* ptrs_spline, float *ad_xy, int* node_count_);

extern void get_tens_value(char* st, double tens);

extern BOOL get_pattern_count(void);
extern void set_pattern_count(BOOL p_count);
extern void set_pattern_offset(int offs);

extern NODE bnodes[MAX_NODES];
extern double curviness;
extern int PL366;

extern void rysuj_spline_(SPLINE *ad, float *ad_xy, int mode, int kolor, BOOL count_mvcurb, BOOL partial, BOOL moving);
extern void get_control_points_reversed(NODE n1, NODE n2, float points[]);
extern void get_control_points(NODE n1, NODE n2, float points[]);
extern void get_control_points_end(NODE n1, NODE n2, float points[]);
extern BOOL trans_scale_arc_shadow(LUK *ptrs_arc, double df_basex, double df_basey, double df_sx, double df_sy, int to_block);
extern BOOL trans_scale_circle_shadow(OKRAG *ptrs_circle, double df_basex, double df_basey, double df_sx, double df_sy, int to_block);
extern void Out_Edited_Draw_Param (ESTR *lps_et, BOOL out) ;
extern void koniecl_(double *x,double *y,void *adr);
extern void koniecs_(double *x, double *y, void *adr);
extern void get_control_points_end(NODE n1, NODE n2, float points[8]);
static void(*cursor_on)(double, double);
static void(*cursor_off)(double, double);
static void set_spline_corner_nomax(int);
static double bspline_z[5];

static BOOL S3D = FALSE;
static BOOL next_spline = FALSE;
static double last_z3d = 0;
static void redraw_spline(void);

extern int Get_Ellipse_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double xy[]);
extern int Get_EllipticalArc_Points (double df_xc, double df_yc, double df_xaxis, double df_yaxis, double df_angle, double df_sangle, double df_eangle, double xy[]);
extern int solidarc_elements(SOLIDARC *sa, LINIA *L_left, LINIA *L_right, LUK *l_inner1, LUK *l_inner2, LUK *l_outer1, LUK *l_outer2);

int bnpts = 4;

static LINIA line_temp = Ldef, line_g = Ldef;
SPLINE bspline = Splinedef; 		
static int i__corner_no, i__spline_corner_nomax = 4;
static BOOL block_added = FALSE;

float bspline_x_base;
float bspline_y_base;

BOOL set_curviness = FALSE;
BOOL set_curviness_pre = FALSE;
BOOL set_curviness_only = FALSE;
double curviness_back, curviness_new;
static double curviness_set = 0.75;
static BOOL bspline__closed = FALSE;

double angle_orig;
int orto_orig;
BOOL angle_changed;

#define r18 18
#define r22 16

static int strwyj;
static double Lx2, Ly2;

//static int np_tension=-1;

static int MAX_BUF_SPLINE=1000;
static int MAX_BUF_ARC=1000;
static int MAX_BUF_SOLIDARC=1000;
static int MAX_BUF_CIRCLE=1000;
static int MAX_BUF_ELLIPTICALARC=1000;
static int MAX_BUF_ELLIPSE=1000;
double MIN_SHADOW_SEGMENT[3] = { 1.0, 1.0, 2.5 };  //0.5, 1.0, 2.5
SPLINE **buf_spline;
LUK **buf_arc;
SOLIDARC **buf_solidarc;
OKRAG **buf_circle;
ELLIPTICALARC **buf_ellipticalarc;
ELLIPSE **buf_ellipse;
int i_spline_no;
int i_arc_no;
int i_solidarc_no;
int i_circle_no;
int i_ellipticalarc_no;
int i_ellipse_no;

static TMENU* menu_spline;

#define go_close_spline -283
#define go_end_spline -284

typedef
struct
{
	double xs, ys;
	double xc, yc;
	double xe, ye;
	double x2, y2;
	double a;
	double L;
	double r;
	double d;
	void *ptr_con; /*kontynuacja, xs, ys : wsp. konca, do kontynuacji*/
	BOOL b_first_end;	/*ktory koniec objektu jest ostatni*/
	double con_angle; /*kat kontynuacji*/
} PLUK;

static PLUK pl;

////////////////////////////////  NURBS  ////////////////////////////////////
/*   Subroutine to generate a B-spline open knot vector with multiplicity
	equal to the order at the ends.

	c            = order of the basis function
	n            = the number of defining polygon vertices
	nplus2       = index of x() for the first occurence of the maximum knot vector value
	nplusc       = maximum value of the knot vector -- $n + c$
	x()          = array containing the knot vector
*/


void knot(int n, int c, int x[]) {
	int nplusc, nplus2, i;

	nplusc = n + c;
	nplus2 = n + 2;

	x[1] = 0;
	for (i = 2; i <= nplusc; i++) {
		if ((i > c) && (i < nplus2))
			x[i] = x[i - 1] + 1;
		else
			x[i] = x[i - 1];
	}
}


/*
   c        = order of the B-spline basis function
   d        = first term of the basis function recursion relation
   e        = second term of the basis function recursion relation
   h[]	     = array containing the homogeneous weights
   npts     = number of defining polygon vertices
   nplusc   = constant -- npts + c -- maximum number of knot values
   r[]      = array containing the rationalbasis functions
			  r[1] contains the basis function associated with B1 etc.
   t        = parameter value
   temp[]   = temporary array
   x[]      = knot vector
*/

void rbasis(int c, float t, int npts, int x[], float h[], float r[]) {
	int nplusc;
	int i, j, k;
	float d, e;
	float sum;
	float temp[36];

	nplusc = npts + c;

	/* calculate the first order nonrational basis functions n[i]	*/

	for (i = 1; i <= nplusc - 1; i++) {
		if ((t >= x[i]) && (t < x[i + 1]))
			temp[i] = 1;
		else
			temp[i] = 0;
	}

	/* calculate the higher order nonrational basis functions */

	for (k = 2; k <= c; k++) {
		for (i = 1; i <= nplusc - k; i++) {
			if (temp[i] != 0)    /* if the lower order basis function is zero skip the calculation */
				d = ((t - x[i])*temp[i]) / (x[i + k - 1] - x[i]);
			else
				d = 0;

			if (temp[i + 1] != 0)     /* if the lower order basis function is zero skip the calculation */
				e = ((x[i + k] - t)*temp[i + 1]) / (x[i + k] - x[i + 1]);
			else
				e = 0;

			temp[i] = d + e;
		}
	}

	if (t == (float)x[nplusc]) {		/*    pick up last point	*/
		temp[npts] = 1;
	}

	/* calculate sum for denominator of rational basis functions */

	sum = 0.;
	for (i = 1; i <= npts; i++) {
		sum = sum + temp[i] * h[i];
	}

	/* form rational basis functions and put in r vector */

	for (i = 1; i <= npts; i++) {
		if (sum != 0) {
			r[i] = (temp[i] * h[i]) / (sum);
		}
		else
			r[i] = 0;
	}
}

/*   b[]         = array containing the defining polygon vertices
				  b[1] contains the x-component of the vertex
				  b[2] contains the y-component of the vertex
				  b[3] contains the z-component of the vertex
	h[]			= array containing the homogeneous weighting factors
	k           = order of the B-spline basis function
	nbasis      = array containing the basis functions for a single value of t
	nplusc      = number of knot values
	npts        = number of defining polygon vertices
	p[,]        = array containing the curve points
				  p[1] contains the x-component of the point
				  p[2] contains the y-component of the point
				  p[3] contains the z-component of the point
	p1          = number of points to be calculated on the curve
	t           = parameter value 0 <= t <= npts - k + 1
	x[]         = array containing the knot vector
*/

//should be :  void calc_bspline(int n, float points[], int npts, float *out_x, float *out_y)

//number of control points, ,points on curve, control points coordinates, weightsn of points, curve points
//void rbspline(int npts, int k, int p1, float b[], float h[], float p[])
void calc_bspline_other(int n, float points[], int p1, float *out_x, float *out_y) {
	int i, j, icount, jcount;
	int i1;
	int x[120];		/* allows for 20 data points with basis function of order 5 */
	int nplusc;

	float step;
	float t;
	float nbasis[90];
	float temp;
	////float p[271];
	float h[90];
	float b[16];

	int k = 4;

	int npts = n;
	if (npts == 0) npts = 4;

	for (i=0; i<90; i++) h[i] = 1;

	b[0] = 0.0;

	for (i = 0; i < npts; i++)
	{
		b[i*3+1] = points[i*2];  
		b[i*3+2] = points[i*2 + 1]; 
		b[i*3+3] = 1.0; 
	}

	nplusc = npts + k;

	/*  zero and redimension the knot vector and the basis array */

	for (i = 0; i <= npts; i++) {
		nbasis[i] = 0.;
	}

	for (i = 0; i <= nplusc; i++) {
		x[i] = 0.;
	}

	/* generate the uniform open knot vector */

	knot(npts, k, x);

	icount = 0;

	/*    calculate the points on the rational B-spline curve */

	t = 0;
	step = ((float)x[nplusc]) / ((float)(p1 - 1));

	for (i1 = 1; i1 <= p1; i1++) {

		if ((float)x[nplusc] - t < 5e-6) {
			t = (float)x[nplusc];
		}

		rbasis(k, t, npts, x, h, nbasis);      /* generate the basis function for this value of t */

		out_x[icount] = 0.;
		out_y[icount] = 0.;

		for (j = 1; j <= 3; j++) 
		{      /* generate a point on the curve */
			jcount = j;

			for (i = 1; i <= npts; i++) { /* Do local matrix multiplication */
				temp = nbasis[i] * b[jcount];

				if (j==1) out_x[icount] = out_x[icount] + temp;
				else if (j==2) out_y[icount] = out_y[icount] + temp;


				jcount = jcount + 3;
			}
		}

		icount ++;
		t = t + step;
	}

}


int test_NURBS(void) {

	int i;
	int npts, k, p1;

	float b[31];  /* allows for up to 10  control vertices */
	float h[11];  /* allows for up to 10  control vertices */
	float p[103]; /* allows for up to 100 points on curve */

	npts = 5;
	k = 3;     /* third order, change for other orders */
	p1 = 11;   /* eleven points on curve */

	for (i = 1; i <= 3 * npts; i++) {
		b[i] = 0.;
	}

	/*	set all homogeneous weighting factros to 1.0 */

	for (i = 1; i <= npts; i++) {
		h[i] = 1.0;
	}

	/*  vary the homogeneous weighting factor 0, 0.25, 1.0, 5.0 */

	h[3] = 1;

	for (i = 1; i <= 3 * p1; i++) {
		p[i] = 0.;
	}

	/*
		Define the control polygon, Ex. 4.1, p. 140 in the z=1 plane because
		this is three dimensional routine. x=b[1], y=b[2], z=b[3], etc.
	*/
	b[1] = 0;
	b[2] = 0;
	b[3] = 1;
	b[4] = 1;
	b[5] = 2;
	b[6] = 1;
	b[7] = 2.5;
	b[8] = 0;
	b[9] = 1;
	b[10] = 4;
	b[11] = 2;
	b[12] = 1;
	b[13] = 5;
	b[14] = 0;
	b[15] = 1;

}


////////////////////////////////////// END OF NURBS /////////////////////////////

void calc_quadratic_bspline(int n, float points[], int npts, float* out_x, float* out_y)
{
	double x, y, x1, y1, x2, y2, dx1, dy1, dx2, dy2, ddx, ddy;
	double nptsd;
	int i;

	nptsd=(double)(npts-1);

	dx1 = (points[2] - points[0]) / nptsd;
	dy1 = (points[3] - points[1]) / nptsd;

	dx2 = (points[4] - points[2]) / nptsd;
	dy2 = (points[5] - points[3]) / nptsd;


	out_x[0] = points[0];
	out_y[0] = points[1];

	x1 = points[0];
	y1 = points[1];

	x2 = points[2];
	y2 = points[3];

	for (i = 1; i < npts-1; i++)
	{
		x1 += dx1;
		y1 += dy1;
		x2 += dx2;
		y2 += dy2;

		ddx = (x2 - x1) / nptsd;
		ddy = (y2 - y1) / nptsd;

		out_x[i] = x1 + (i * ddx);
		out_y[i] = y1 + (i * ddy);
	}

	out_x[npts-1] = points[4];
	out_y[npts-1] = points[5];
	
}

/* calc_bspline:
 *  Calculates a set of pixels for the bezier spline defined by the four
 *  points specified in the points array. The required resolution
 *  is specified by the npts parameter, which controls how many output
 *  pixels will be stored in the x and y arrays.
 */
void calc_bspline(int lp, int n, float points[], int npts, float *out_x, float *out_y)
{
   /* Derivatives of x(t) and y(t). */
   double x, dx, ddx, dddx;
   double y, dy, ddy, dddy;
   int i;

   /* Temp variables used in the setup. */
   double dt, dt2, dt3;
   double xdt2_term, xdt3_term;
   double ydt2_term, ydt3_term;

   if (n < 5)
   {
	  // if ((points[4] == points[6]) && (points[5] == points[7]))  lp = 6;
	   if (lp == 6) n = 3;
   }

    if (n == 5)
    {
        // if ((points[4] == points[6]) && (points[5] == points[7]))  lp = 6;
        if (lp == 6)
            n = 3;
    }

   if ((lp==6) && (n==3) )
   {
	   calc_quadratic_bspline(n, points, npts, out_x, out_y);
	   return;
   }

   dt = 1.0 / (double)(npts-1);
   dt2 = (dt * dt);
   dt3 = (dt2 * dt);

   /* x coordinates. */
   xdt2_term = 3.0 * (points[4] - 2.0*points[2] + points[0]);
   xdt3_term = points[6] + 3.0 * (-points[4] + points[2]) - points[0];

   xdt2_term = dt2 * xdt2_term;
   xdt3_term = dt3 * xdt3_term;

   dddx = 6.0*xdt3_term;
   ddx = -6.0*xdt3_term + 2.0*xdt2_term;
   dx = xdt3_term - xdt2_term + 3.0 * dt * (points[2] - points[0]);
   x = points[0];

   out_x[0] = points[0];

   for (i=1; i<npts; i++) {
      ddx += dddx;
      dx += ddx;
      x += dx;

	  out_x[i] = x; // (int)x;
   }

   /* y coordinates. */
   ydt2_term = 3.0 * (points[5] - 2.0*points[3] + points[1]);
   ydt3_term = points[7] + 3.0 * (-points[5] + points[3]) - points[1];

   ydt2_term = dt2 * ydt2_term;
   ydt3_term = dt3 * ydt3_term;

   dddy = 6.0*ydt3_term;
   ddy = -6.0*ydt3_term + 2.0*ydt2_term;
   dy = ydt3_term - ydt2_term + dt * 3.0 * (points[3] - points[1]);
   y = points[1];

   out_y[0] = points[1];

   for (i=1; i<npts; i++) {
      ddy += dddy;
      dy += ddy;
      y += dy;

	  out_y[i] = y; // (int)y;
   }
}


TMENU mSpline = { 8,0,0,30, 56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr,0,0,0,&pmSpline,NULL,NULL };

static void out_temp_line(void)
/*-----------------------------*/
{
	LINIA L=Ldef;
	char buff_tens[16];

	if (orto)
	{
		L.x1 = line_g.x1;
		L.y1 = line_g.y1;
		L.x2 = line_g.x2;
		L.y2 = line_g.y2;
		orto_l(&L, &Orto_Dir);
		line_temp.x2 = L.x2;
		line_temp.y2 = L.y2;


	}
	else
	{
		L.x2 = line_g.x2;
		L.y2 = line_g.y2;
	}

	if (set_curviness == TRUE)
	{
		curviness = (double /*float*/)pikseleX00((line_g.x2 - line_g.x1) * (line_g.x2 - line_g.x1) + (line_g.y2 - line_g.y1) * (line_g.y2 - line_g.y1));
		curviness /= 25000.0;
		if (curviness < 0.01) curviness = 0.01;
		if (curviness > 1.0) curviness = 1.0;


		get_tens_value(eTens.st, curviness);

		Out_Edited_Draw_Param((ESTR*)&eTens, TRUE);
	}

	setwritemode(COPY_PUT);
	SetColorAC(Get_Current_Color());

	setlinestyle1(DOTTED_LINE, 0, NORM_WIDTH);

	okno_r();

	if ((i__corner_no < 2) && (bspline.npts<5))
	{
		bspline.xy[2] = L.x2; //line_g.x2;
		bspline.xy[3] = L.y2; // line_g.y2;
	}
	if ((i__corner_no < 3)  && (bspline.npts < 5))
	{
		bspline.xy[4] = L.x2; //line_g.x2;
		bspline.xy[5] = L.y2; // line_g.y2;
	}
	if ((i__corner_no < 4)  && (bspline.npts < 5))
	{
		bspline.xy[6] = L.x2; //line_g.x2;
		bspline.xy[7] = L.y2; // line_g.y2;
	}
	else
	{
		bspline.xy[i__corner_no * 2] = L.x2;
		bspline.xy[i__corner_no * 2 + 1] = L.y2;
	}

	bspline.kolor = Get_Current_Color(); //kolory.blok; //????
	bspline.typ = LiniaG.typ;
	bspline.warstwa = Current_Layer;  //??????????????????????????

	if (!set_curviness_only) bspline.lp = i__corner_no * 2 + 2;

	if (bspline.npts>4) bspline.xy[bspline.lp] = curviness;

	if ((bspline.lp > 4) || (!orto))
        rysuj_spline_(&bspline, &bspline.xy, COPY_PUT, -1, TRUE, FALSE, FALSE);

	okno_all();

	if (set_curviness_pre == TRUE)
	{
		set_curviness_pre = FALSE;
		set_curviness = TRUE;
	}
}

static void  cur_off(double x, double y)
{
    flip_screen();
}


static void  cur_on(double x, double y)
/*--------------------------------------*/
{
	line_g.x2 = x;
	line_g.y2 = y;
	line_g.kolor = 7; // kolory.blok // Get_Current_Color();
	outlineor(&line_g, COPY_PUT, 1);
	line_temp.x2 = line_g.x2;
	line_temp.y2 = line_g.y2;
    redraw_spline();
	out_temp_line();
	cursor_on(x, y);
}

static int L_p(BOOL b_graph_value)
/*----------------*/
{
	PLINIA PL;
	double l, k;
	double angle_l;

	b_graph_value = b_graph_value;
	if (eL.val_no < 1) return 0;
	l = eL.values[0];
	if (orto != 0 || eL.val_no == 1)
	{
		parametry_linior(&line_g, &PL);
		k = PL.kat;
	}
	else
	{
		angle_l = get_angle_l();
		if (angle_l != 0)
		{
			k = eL.values[1] + angle_l;
			if (k < 0) k += 360;
		}
		else
			k = eL.values[1];
	}
	l = jednostkiOb(l);
	k = Grid_to_Rad(k);
	Lx2 = line_g.x1 + l * cos(k);
	Ly2 = line_g.y1 + l * sin(k);
	strwyj = 1;
	return 1;
}

static void redraw_spline1(BOOL b_redraw)
/*-------------------------------------*/
{
	LINIA lg;
	int i, j;

	i = 0;
	if (i__corner_no > 1)
	{

		memmove(&lg, &line_g, sizeof(LINIA));
		lg.typ = 7; // 65;
		lg.x1 = bspline.xy[i++];
		lg.y1 = bspline.xy[i++];
		lg.x2 = bspline.xy[i++];
		lg.y2 = bspline.xy[i++];

        rysuj_obiekt((char*)&lg, COPY_PUT, b_redraw);


		if (i__corner_no > 2)
		{
			lg.x1 = lg.x2;
			lg.y1 = lg.y2;
			lg.x2 = bspline.xy[i++];
			lg.y2 = bspline.xy[i++];
			rysuj_obiekt((char*)&lg, COPY_PUT, b_redraw);
		}

		if (i__corner_no > 3)
		{
			lg.x1 = lg.x2;
			lg.y1 = lg.y2;
			lg.x2 = bspline.xy[i++];
			lg.y2 = bspline.xy[i++];
			rysuj_obiekt((char*)&lg, COPY_PUT, b_redraw);
		}

		if (i__corner_no > 4)  //for multipoints
		{
			for (j = 4; j < i__corner_no; j++)
			{
				lg.x1 = lg.x2;
				lg.y1 = lg.y2;
				lg.x2 = bspline.xy[i++];
				lg.y2 = bspline.xy[i++];
				rysuj_obiekt((char*)&lg, COPY_PUT, b_redraw);
			}
		}

	}
}


static void redraw_spline(void)
/*-----------------------------*/
{
	redraw_spline1(TRUE);
}

static void next_point(double x0, double y0,  BOOL strwyj)
/*-----------------------------------------*/
{
	char sks[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];
	int a;

	if (strwyj)
	{
		line_g.x2 = Double_to_Float(Lx2);
		line_g.y2 = Double_to_Float(Ly2);
	}
	else
	{
		line_g.x2 = x0;
		line_g.y2 = y0;
	}
	if (orto)
	{
		orto_l(&line_g, &Orto_Dir);
		Orto_Dir = I_Orto_NoDir;
	}
	line_g.typ = 7; // 65;
	outline(&line_g, COPY_PUT, 0);


	if (line_g.x1 != line_g.x2 || line_g.y1 != line_g.y2)
	{
		i__corner_no++;

		bspline.lp = (i__corner_no - 1) * 2;

		bspline.xy[bspline.lp++] = line_g.x2;
		bspline.xy[bspline.lp++] = line_g.y2;
		
	}

	line_g.x1 = line_g.x2;
	line_g.y1 = line_g.y2;
	if (S3D == TRUE)
	{
		//pytanie o wspolrzedne LiniaG3D.z1; LiniaG3D.z2
		sprintf(sks, "%f", last_z3d);
		if (!get_string(sks, "", MaxTextLen, 0, 144)) return;
		if (FALSE == calculator(sks, &retval_no, buf_ret) || retval_no < 1)
		{
			return;
		}
		last_z3d = buf_ret[0];
		bspline_z[i__corner_no] = milimetryob(last_z3d);
	}

	line_g.x2 = X;
	line_g.y2 = Y;
}

static BOOL close_spline(void)
{
	

		bspline.closed = ! bspline.closed;
		bspline__closed = bspline.closed;

		if (bspline__closed == TRUE)  menu_par_new((*mSpline.pola)[7].txt, _YES__);
		else menu_par_new((*mSpline.pola)[7].txt, _NO__);

		return bspline.closed; // TRUE;

}

static int previous_point(void) //(double x0, double y0)
/*--------------------------------------------*/
{
	char sks[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];

	if (i__corner_no > 1)
	{
		redraw_spline1(FALSE);
		cur_off(X, Y);
		i__corner_no--;

		bspline.lp -= 2;

        line_g.x1 = bspline.xy[bspline.lp-4];
        line_g.y1 = bspline.xy[bspline.lp-3];

		redraw_spline1(TRUE);
		cur_on(X, Y);
	}
	return 0; //-285;

	if (strwyj)
	{
		line_g.x2 = Double_to_Float(Lx2);
		line_g.y2 = Double_to_Float(Ly2);
	}
	else
	{
		line_g.x2 = X; // x0;
		line_g.y2 = Y; // y0;
	}
	if (orto)
	{
		orto_l(&line_g, &Orto_Dir);
		Orto_Dir = I_Orto_NoDir;
	}

	line_g.typ = 7; // 65;
	outline(&line_g, COPY_PUT, 0);

	if (i__corner_no>1)
	{
		i__corner_no--;
		bspline.lp -= 2;

	}

	if (S3D == TRUE)
	{
		//pytanie o wspolrzedne LiniaG3D.z1; LiniaG3D.z2
		sprintf(sks, "%f", last_z3d);
		if (!get_string(sks, "", MaxTextLen, 0, 144)) return -285;
		if (FALSE == calculator(sks, &retval_no, buf_ret) || retval_no < 1)
		{
			return 0; //-285;
		}
		last_z3d = buf_ret[0];
		bspline_z[i__corner_no] = milimetryob(last_z3d);
	}


}

static BOOL add_spline(void)
//tymczasowo wprowadzono dla S3D wartosci zerowe z1, z2, z3, z4
/*------------------------------------------------------------*/
{
	int k, kk;
	SPLINE bspline_temp;
	double dx, dy, dl, al;


	redraw_spline1(FALSE);
	next_spline = TRUE;

	bspline.kolor = Get_Current_Color();
	bspline.typ = LiniaG.typ;
	bspline.warstwa = Current_Layer;
	if (S3D == FALSE)
	{

		if (bspline.npts < 5)
		{
			
			if (bspline.lp == 6)
			{
				kk = 2;
			}
			else
			{
				kk = 4;
			}
			
			bspline.obiekt = Ospline;
			bspline.n = 8 + bspline.lp * sizeof(float);
		}
		else
		{
			bspline.lp -=2; //but added curviness after xy points

			bspline.xy[bspline.lp] = curviness;

			bspline.obiekt = Ospline;
			bspline.n = 8 + (bspline.lp + 1 ) * sizeof(float);

			kk = bspline.lp - 4;
		}
	}
	else
	{
		bspline.obiekt = Ospline;
		if (bspline.lp == 6)
		{
			bspline.xy[6] = bspline_z[0];
			bspline.xy[7] = bspline_z[1];
			bspline.xy[8] = bspline_z[2];
			bspline.n = 8 + (bspline.lp + 3) * sizeof(float);
		}
		else
		{
			bspline.xy[8] = bspline_z[0];
			bspline.xy[9] = bspline_z[1];
			bspline.xy[10] = bspline_z[2];
			bspline.xy[11] = bspline_z[3];
			bspline.n = 8 + (bspline.lp + 4) * sizeof(float);
		}
	}


	if (bspline__pline == TRUE)
	{
		if (block_added == FALSE)
		{
			if (NULL == add_block(B_SHADOW, PL_CURVE))
			{
				return FALSE;
			}
			block_added = TRUE;
		}
		bspline.blok = ElemBlok;
		bspline.obiektt2 = O2BlockPline;
	}
	else
	{
		bspline.blok = NoElemBlok;
		bspline.obiektt2 = O2NoBlockS;
	}

	memcpy(&bspline_temp, &bspline, sizeof(bspline));


	if (NULL != dodaj_obiekt(((bspline__pline == TRUE) ? (BLOK*)dane : NULL), &bspline_temp))
	{
        Set_Second_Screen();
		rysuj_obiekt((char *)&bspline, COPY_PUT, 1);
        Set_Screen();
        flip_screen();
	}

	i__corner_no = 1; // 2;
	k = bspline.lp - 4;


	bspline_x_base = bspline.xy[kk];
	bspline_y_base = bspline.xy[kk+1];
	k++;
	k++;

	bspline.xy[0] = bspline.xy[k++];
	bspline.xy[1] = bspline.xy[k++];
	bspline_z[0] = bspline_z[2];
	bspline.lp = 2; // 4;
	line_temp.x1 = bspline.xy[0];
	line_temp.y1 = bspline.xy[1];


	angle_orig = get_angle_l();
	orto_orig = orto;
	angle_changed = TRUE;
	
	if (bspline.npts < 5)
	{
		al = Atan2(bspline.xy[1] - bspline_y_base, bspline.xy[0] - bspline_x_base);
		al *= 180 / Pi;

		if (!orto) put_angle_l(al);
		orto = 1;
	}
	else //multipoint
	{
		;
	}

	curviness_set = curviness;

	return TRUE;
}

static void redcr(char typ, double x0, double y0)
/*----------------------------------------------*/
{
	static void(*CUR_oN)(double, double);
	static void(*CUR_oFF)(double, double);
	static int KR0, np, np1;
	static void(*REDdod)(void);

	if (typ == 0)
	{
		CUR_OFF(X, Y);
		i__corner_no = 1;
		bspline.xy[0] = x0;
		bspline.xy[1] = y0;
		bspline.lp = 2;
		KR0 = Komunikat_R0;
		Komunikat_R0 = 124;
		komunikat0(Komunikat_R0);
		REDdod = REDDOD; REDDOD = redraw_spline;
		poczL = 1;
		np = dodajstr(&eL);
		CUR_oFF = CUR_OFF;  CUR_OFF = cur_off;
		CUR_oN = CUR_ON;   CUR_ON = cur_on;
		CUR_ON(X, Y);
	}
	else
	{
		redraw_spline1(FALSE);
		CUR_OFF = CUR_oFF;
		CUR_ON = CUR_oN;
		Komunikat_R0 = KR0;
		poczL = 0;
		REDDOD = REDdod;
		komunikat0(Komunikat_R0);
		usunstr(np);
		i__corner_no = 0;
	}
}

static void redcr_amend(char typ, double x0, double y0)
/*----------------------------------------------*/
{
	static void(*CUR_oN)(double, double);
	static void(*CUR_oFF)(double, double);
	static int KR0, np, np1;
	static void(*REDdod)(void);

	if (typ == 0)
	{
		CUR_OFF(X, Y);
		KR0 = Komunikat_R0;
		Komunikat_R0 = 142;
		komunikat0(Komunikat_R0);
		REDdod = REDDOD; REDDOD = redraw_spline;
		poczL = 1;
		np = dodajstr(&eL);
		np1 = dodajstr(&eTens);
		CUR_oFF = CUR_OFF;  CUR_OFF = cur_off;
		CUR_oN = CUR_ON;   CUR_ON = cur_on;
		CUR_ON(X, Y);
	}
	else
	{
		redraw_spline1(FALSE);
		CUR_OFF = CUR_oFF;
		CUR_ON = CUR_oN;
		Komunikat_R0 = KR0;
		poczL = 0;
		REDDOD = REDdod;
		komunikat0(Komunikat_R0);
		usunstr(np1);
		usunstr(np);
	}
}

int close_spline_point(void)  //Home
{
	return go_close_spline;
}


int end_spline_point(void)  //End
{
	return go_end_spline;
}

static void redcr0(char typ)
{
	static void(*CUR_oN)(double, double);
	static void(*CUR_oFF)(double, double);
	static int(*SW[2])(), akt;
	static TMENU *menu;
    static char st[POLE_TXT_MAX];
    unsigned short cod = L' ';
	static int iconno = 0;

	if (typ == 0)
	{
		CUR_OFF(X, Y);
		i__corner_no = 0;
		komunikat(0);
		komunikat0(123);
		akt = sel.akt; sel.akt = ASel;
		cursor_off = cursel_off;
		cursor_on = cursel_on;
		eL.x = maxX - PL266;
		eL.y = ESTR_Y;
		eL.val_no_max = 2;
		eL.mode = GV_VECTOR;
		eL.format = "%#12.9lg\0%#12.9lg;%#8.3lf";
		eL.lmax = r22;
		eL.ESTRF = L_p;
		eL.extend = 0;
		getmenupini(&menu, st, &cod, &iconno);

	    menupini(&mSpline, _SPLINE_, _SPLINE_C_, 516);
		
		CUR_oFF = CUR_OFF;  CUR_OFF = cursel_off;
		CUR_oN = CUR_ON;   CUR_ON = cursel_on;
		SW[0] = SERV[73];  SERV[73] = sel_t;
		SW[1] = SERV[81];  SERV[81] = sel_d;
		SW[4] = SERV[83];  SERV[83] = previous_point;  //Del

		SW[3] = SERV[71];  SERV[71] = close_spline_point;  //Home
		SW[2] = SERV[79];  SERV[79] = end_spline_point;  //End
		CUR_ON(X, Y);
	}
	else
	{
		CUR_OFF(X, Y);
		CUR_OFF = CUR_oFF;
		CUR_ON = CUR_oN;
		SERV[73] = SW[0];
		SERV[81] = SW[1];
		SERV[83] = SW[4];

		SERV[71] = SW[3];
		SERV[79] = SW[2];

		sel.akt = akt;
		CUR_ON(X, Y);
		menupini(menu, st, cod, iconno);
		komunikat(0);
		komunikat0(0);
	}
}

void redcr_spline0(char typ)
{
	static void(*CUR_oN)(double, double);
	static void(*CUR_oFF)(double, double);
	static int(*SW[2])(), akt;
	static TMENU* menu;
    static char st[POLE_TXT_MAX];
    unsigned short cod = L' ';
	static int iconno = 0;

	if (typ == 0)
	{
		CUR_OFF(X, Y);
		i__corner_no = bspline.lp / 2;
		komunikat(0);
		komunikat0(142);
		akt = sel.akt; sel.akt = ASel;
		cursor_off = cursel_off;
		cursor_on = cursel_on;
		eL.x = maxX - PL266;
		eL.y = ESTR_Y;
		eL.val_no_max = 2;
		eL.mode = GV_VECTOR;
		eL.format = "%#12.9lg\0%#12.9lg;%#8.3lf";
		eL.lmax = r22;
		eL.ESTRF = L_p;
		eL.extend = 0;

		CUR_oFF = CUR_OFF;  CUR_OFF = cursel_off;
		CUR_oN = CUR_ON;   CUR_ON = cursel_on;
		SW[0] = SERV[73];  SERV[73] = sel_t;
		SW[1] = SERV[81];  SERV[81] = sel_d;

		CUR_ON(X, Y);
	}
	else if (typ == 10)
	{
		;
	}
	else
	{
		CUR_OFF(X, Y);
		CUR_OFF = CUR_oFF;
		CUR_ON = CUR_oN;
		SERV[73] = SW[0];
		SERV[81] = SW[1];

		sel.akt = akt;
		CUR_ON(X, Y);

		komunikat(0);
		komunikat0(0);
	}
}


char *add_block(char b_type, char c_pltype)
/*---------------------------------------*/
{

    BLOK s_blockd = Bdef;  //TO CHECK
	BLOK *ptrs_block;


	s_blockd.n = B3 + sizeof(c_pltype);
	s_blockd.kod_obiektu = b_type; //B_PLINE;
	s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype);
	if ((ptrs_block = (BLOK*)dodaj_obiekt(NULL, &s_blockd)) == NULL)
	{
		return NULL;
	}

	memmove(&(ptrs_block->opis_obiektu[0]), &c_pltype, sizeof(c_pltype));
	return ptrs_block;
}

static void spline_pline_on(void)
/*---------------------------*/
{

	bspline__pline = TRUE;

	menu_par_new((*mSpline.pola)[2].txt, _YES__);
}

static void spline_pline_off(void)
/*---------------------------*/
{

	
	bspline__pline = FALSE;
	block_added = FALSE;
	
	menu_par_new((*mSpline.pola)[2].txt, _NO__);
}


static int
L_axis2_curviness(BOOL b_graph_value)
/*---------------------------------*/
{
	double df_len;


	b_graph_value = b_graph_value;
	if (eTens.val_no < 1)
	{
		return 0;
	}
	df_len = eTens.values[0];
	if (df_len > 0)
	{
		curviness_new = df_len;
		strwyj = 2;
		return 1;
	}
	else return 0;
}

void set_spline_tension_only(int typ)
{
	set_curviness_only = typ;
}

void set_spline_tension_force(int typ)
{
	set_curviness = typ;
}

void set_spline_tension(int typ, BOOL set_np)
{
	static int np_tension;

	if (typ == 1)
	{
		curviness_back = curviness;
		set_curviness_pre = TRUE;

		
		eTens.x = maxX - PL366;
		eTens.y = ESTR_Y;
		eTens.lmax = r22;
		eTens.val_no_max = 1;
		eTens.val_no = 1;
		eTens.mode = GV_DIST;
		eTens.format = "%#12.9f";
		eTens.ESTRF = L_axis2_curviness;
		eTens.extend = 0;
		
		if (set_np) np_tension = dodajstr(&eTens);
		

	}
	else if (typ==0)
	{
		curviness = curviness_back;
		set_curviness = FALSE;
		if  (set_np)  usunstr(np_tension);
	}
	else //2
	{
		set_curviness = FALSE;
		if (set_np)  usunstr(np_tension);
		
	}
}


void spline_amend_proc(double x0, double y0)
/*----------------------------------------*/
{
	char sks[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];
	EVENT* ev;
	BOOL ret;

	line_g.x1 = line_temp.x1 = x0;
	line_g.y1 = line_temp.y1 = y0;
	line_g.x2 = line_temp.x2 = X;
	line_g.y2 = line_temp.y2 = Y;

	line_g.typ = 7;

	Orto_Dir = I_Orto_NoDir;
	redcr_amend(0, x0, y0);
	if (S3D == TRUE)
	{
		//pytanie o wspolrzedne LiniaG3D.z1; LiniaG3D.z2
		sprintf(sks, "%f", last_z3d);
		if (!get_string(sks, "", MaxTextLen, 0, 144)) return;
		if (FALSE == calculator(sks, &retval_no, buf_ret) || retval_no < 1)
		{
			if (angle_changed)
			{
				put_angle_l(angle_orig);
				orto = orto_orig;
			}
			return;
		}
		last_z3d = buf_ret[0];
		bspline_z[i__corner_no] = milimetryob(last_z3d);
	}
	while (1)
	{
		strwyj = 0;
		view_line_type(&LiniaG);
		ev = Get_Event_Point(NULL, &x0, &y0);
		if (ev->What == evKeyDown && ev->Number == 0)
		{
			CUR_OFF(X, Y);

			set_spline_tension(0, FALSE);

			bspline.xy[bspline.lp] = curviness;

			redcr_amend(1, 0, 0);
			CUR_ON(X, Y);
			if (angle_changed)
			{
				put_angle_l(angle_orig);
				orto = orto_orig;
			}
			return;
			
		}
		if ((ev->What == evKeyDown && ev->Number == ENTER) || strwyj)
		{

			CUR_OFF(X, Y);

			if (strwyj == 2) bspline.xy[bspline.lp] = curviness_new;
			
			set_spline_tension(2, FALSE);
		
			redcr_amend(1, 0, 0);
			CUR_ON(X, Y);
			if (angle_changed)
			{
				put_angle_l(angle_orig);
				orto = orto_orig;
			}
			return;
		}
	}
}

void spline_proc(double x0, double y0)
/*-----------------------------------*/
{
	char sks[MaxTextLen] = "";
	int retval_no = 1;
	double buf_ret[1];
	EVENT *ev;
	BOOL ret;
	static int np_tension;

	line_g.x1 = line_temp.x1 = x0;
	line_g.y1 = line_temp.y1 = y0;
	line_g.x2 = line_temp.x2 = X;
	line_g.y2 = line_temp.y2 = Y;

	line_g.typ = 7;

	Orto_Dir = I_Orto_NoDir;
	redcr(0, x0, y0);
	if (S3D == TRUE)
	{
		//pytanie o wspolrzedne LiniaG3D.z1; LiniaG3D.z2
		sprintf(sks, "%f", last_z3d);
		if (!get_string(sks, "", MaxTextLen, 0, 144)) return;
		if (FALSE == calculator(sks, &retval_no, buf_ret) || retval_no < 1)
		{
			if (angle_changed)
			{
				put_angle_l(angle_orig);
				orto = orto_orig;
			}
			return;
		}
		last_z3d = buf_ret[0];
		bspline_z[i__corner_no] = milimetryob(last_z3d);
	}
	while (1)
	{
		strwyj = 0;
		view_line_type(&LiniaG);
		ev = Get_Event_Point(NULL, &x0, &y0);
		if (ev->What == evKeyDown && ev->Number == 0)
		{
			CUR_OFF(X, Y);

			if (set_curviness == FALSE)
			{
				if ((bspline.npts < 5) || (i__corner_no < 3))
				{
					redcr(1, 0, 0);
					CUR_ON(X, Y);
					if (angle_changed)
					{
						put_angle_l(angle_orig);
						orto = orto_orig;
					}
					return;
				}
				else
				{

					line_g.x2 = x0;
					line_g.y2 = y0;

					if (orto)
					{
						orto_l(&line_g, &Orto_Dir);
						Orto_Dir = I_Orto_NoDir;
					}

					line_g.typ = 7; // 65;

					add_spline();
					redcr(1, 0, 0);
					CUR_ON(X, Y);
					if (angle_changed)
					{
						put_angle_l(angle_orig);
						orto = orto_orig;
					}
					return;
				}
			}
			else
			{
				set_spline_tension(0, TRUE);
				
			
				if (set_curviness_only)
				{

					redcr(1, 0, 0);
					CUR_ON(X, Y);
					if (angle_changed)
					{
						put_angle_l(angle_orig);
						orto = orto_orig;
					}
					return;
				}
				else CUR_ON(X, Y);
			}
		}
		if ((ev->What == evKeyDown && ev->Number == ENTER) || strwyj)
		{
			
			CUR_OFF(X, Y);

			if (set_curviness == FALSE)
			{
				next_point(x0, y0, strwyj);
				if (i__corner_no >= i__spline_corner_nomax)
				{
					add_spline();
				}
				else
				{
					if (angle_changed)
					{
						put_angle_l(angle_orig);
						orto = orto_orig;
					}
				}
			}
			else
			{
				if (strwyj==2) curviness = curviness_new;
				set_spline_tension(2, TRUE);
				
				if (set_curviness_only)
				{
					redcr(1, 0, 0);
					CUR_ON(X, Y);
					if (angle_changed)
					{
						put_angle_l(angle_orig);
						orto = orto_orig;
					}
					return;
				}
			}

			CUR_ON(X, Y);
		}
		if (ev->What == evCommandP)
		{
			if (ev->Number == 8 || ev->Number == 9 || ev->Number == 10 || ev->Number == 11)
			{
				set_spline_corner_nomax(ev->Number);
			}
			else if (ev->Number == 12)  //polyline yes
			{
				spline_pline_on();
			}
			else if (ev->Number == 13)  //polyline no
			{
				spline_pline_off();
			}
			else if (ev->Number == 1)  //quadratic now
			{
				if (i__corner_no == 3)
				{
					CUR_OFF(X, Y);
					bspline.xy[6] = bspline.xy[4];
					bspline.xy[7] = bspline.xy[5];
					bspline.lp = 6;
					bspline.npts = 3;
					add_spline();
					CUR_ON(X, Y);
				}
				else if (i__corner_no == 2)
				{
					CUR_OFF(X, Y);
					next_point(X, Y, 0);
					bspline.xy[6] = bspline.xy[4];
					bspline.xy[7] = bspline.xy[5];
					add_spline();
					CUR_ON(X, Y);
				}
			  
			}
			else if (ev->Number == 3)  //remove
			{
				previous_point();
			}
			else if (ev->Number == 7)  //close
			{
				if (bspline.npts > 4)
				{
					CUR_OFF(X, Y);
					ret=close_spline();
					CUR_ON(X, Y);
				}	
			}
			else if (ev->Number == 5)  //end
			{
				if (bspline.npts > 4)
				{
					CUR_OFF(X, Y);

					line_g.x2 = x0;
					line_g.y2 = y0;

					if (orto)
					{
						orto_l(&line_g, &Orto_Dir);
						Orto_Dir = I_Orto_NoDir;
					}

					line_g.typ = 7; // 65;
					outline(&line_g, COPY_PUT, 0);

					bspline.lp += 2;

					add_spline();

					redcr(1, 0, 0);
					CUR_ON(X, Y);
					if (angle_changed)
					{
						put_angle_l(angle_orig);
						orto = orto_orig;
					}

					return;
				}
				
			}
			else if (ev->Number == 6)  //tension
			{
				set_spline_tension(1, TRUE);
			}
		}
	}
}


static void set_spline_corner_nomax(int poz)
/*----------------------------------*/
{
	char sks[10];

	if (poz == 8)
	{
		if (i__spline_corner_nomax == 3) return;
		CUR_OFF(X, Y);
		i__spline_corner_nomax = 3;
		bspline.npts = bnpts = 3;  //4 ? there are always 4 points even if just 3 reference points
		bspline.closed = 0;
		strcpy(sks, _Q_);
		mSpline.max = 5;
		CUR_ON(X, Y);
	}
	else if (poz == 9)
	{
		if (i__spline_corner_nomax == 4) return;
		CUR_OFF(X, Y);
		i__spline_corner_nomax = 4;
		bspline.npts = bnpts = 4;
		bspline.closed = 0;
		strcpy(sks, _C_);
		mSpline.max = 5;
		CUR_ON(X, Y);
	}
	else if (poz == 10) //cardinal
	{
		if (i__spline_corner_nomax == 5) return;
		CUR_OFF(X, Y);
		i__spline_corner_nomax = NumSplinePoints;
		bspline.npts = bnpts = 5; //5 means Cardinal multipoints
		bspline.closed = bspline__closed; //closed is always on demand, and is referred just to multipoint curve
		strcpy(sks, _M_);
		mSpline.max = 8;
		CUR_ON(X, Y);
	}
	else if (poz == 11) //B-spline
	{
		if (i__spline_corner_nomax == 6) return;
		CUR_OFF(X, Y);
		i__spline_corner_nomax = NumSplinePoints;
		bspline.npts = bnpts = 6; //6 means B-Spline multipoints
		bspline.closed = bspline__closed; //closed is always on demand, and is referred just to multipoint curve
		strcpy(sks, _B_);
		mSpline.max = 8;
		CUR_ON(X, Y);
	}
	menu_par_new((*mSpline.pola)[0].txt, sks);
}

static void redcr_continue(char typ)
/*----------------------------------*/
{
	static void(*CUR_oN)(double, double);
	static void(*CUR_oFF)(double, double);
	static int(*SW[3])(), akt, sel_cur;

	if (typ == 0)
	{
		komunikat0(81);
		komunikat(0);
		akt = sel.akt; sel.akt = ASel;
		sel_cur = sel.cur; sel.cur = 1;
		Cur_offd(X, Y);
		CUR_oFF = CUR_OFF;  CUR_OFF = out_sel_off;
		CUR_oN = CUR_ON;   CUR_ON = out_sel_on;
		SW[0] = SERV[73];  SERV[73] = sel_t;
		SW[1] = SERV[81];  SERV[81] = sel_d;
		SW[4] = SERV[83];  SERV[83] = previous_point;

		SW[3] = SERV[71];  SERV[71] = close_spline_point;  //Home
		SW[2] = SERV[79];  SERV[79] = end_spline_point;  //End
		Cur_ond(X, Y);
	}
	else
	{
		Cur_offd(X, Y);
		CUR_OFF = CUR_oFF;
		CUR_ON = CUR_oN;
		SERV[73] = SW[0];
		SERV[81] = SW[1];
		SERV[83] = SW[4];

		SERV[71] = SW[3];
		SERV[79] = SW[2];
		Cur_ond(X, Y);
		sel.akt = akt;
		sel.cur = sel_cur;
	}
}

static void get_param_end_ob(void   *ptr_object,
	BOOL    *b_first_end,
	double  *df_xend,
	double  *df_yend)
	/*-----------------------------------------------*/
{
	LINIA * ptrs_line;
	LUK * ptrs_arc;
	SPLINE * ptrs_spline;
	double x1, y1;

	ptrs_line = (LINIA*)ptr_object;
	ptrs_arc = (LUK*)ptr_object;
	ptrs_spline = (SPLINE*)ptr_object;
	*df_xend = X; 	/*dla koniecL_, koniecl_ */
	*df_yend = Y;
	if ((((NAGLOWEK*)ptr_object)->obiekt == Olinia))
	{
		koniecL_(df_xend, df_yend, ptr_object);
		x1 = ptrs_line->x1;
		y1 = ptrs_line->y1;
	}
	else if ((((NAGLOWEK*)ptr_object)->obiekt == Oluk))
	{
		koniecl_(df_xend, df_yend, ptr_object);
		x1 = ptrs_arc->x + ptrs_arc->r * cos(ptrs_arc->kat1);
		y1 = ptrs_arc->y + ptrs_arc->r * sin(ptrs_arc->kat1);
	}
	else //spline
	{
		koniecs_(df_xend, df_yend, ptr_object);
		x1 = ptrs_spline->xy[0];
		y1 = ptrs_spline->xy[1];
	}
	*b_first_end = FALSE;
	if (TRUE == Check_if_Equal(x1, *df_xend) &&
		TRUE == Check_if_Equal(y1, *df_yend))
	{
		*b_first_end = TRUE;
	}
}

static double get_continue_angle(void *ptr_con, BOOL b_first_end)
/*------------------------------------------------------------*/
{
	double angle, x1, y1, x2, y2;
	LINIA *ptrs_line;
	LUK  *ptrs_arc;
	SPLINE  *ptrs_spline;

	if ((Olinia == ((NAGLOWEK *)ptr_con)->obiekt))
	{
		ptrs_line = (LINIA *)ptr_con;
		if (TRUE == b_first_end)
		{
			angle = Atan2(ptrs_line->y1 - ptrs_line->y2, ptrs_line->x1 - ptrs_line->x2);
		}
		else
		{
			angle = Atan2(ptrs_line->y2 - ptrs_line->y1, ptrs_line->x2 - ptrs_line->x1);
		}
	}
	else if ((Oluk == ((NAGLOWEK *)ptr_con)->obiekt))
	{
		ptrs_arc = (LUK *)ptr_con;
		angle = ((b_first_end == TRUE) ? ptrs_arc->kat1 : ptrs_arc->kat2);
		obrd(sin(angle), cos(angle), ptrs_arc->r,
			((b_first_end == TRUE) ? -1 : 1) * 1, &x2, &y2);
		x2 += ptrs_arc->x;
		y2 += ptrs_arc->y;
		x1 = ptrs_arc->x + ptrs_arc->r * cos(ptrs_arc->kat1);
		y1 = ptrs_arc->y + ptrs_arc->r * sin(ptrs_arc->kat1);
		if (b_first_end == FALSE)
		{
			x1 = ptrs_arc->x + ptrs_arc->r * cos(ptrs_arc->kat2);
			y1 = ptrs_arc->y + ptrs_arc->r * sin(ptrs_arc->kat2);
		}
		angle = Atan2(y2 - y1, x2 - x1);
	}
	else
	{
		ptrs_spline = (SPLINE *)ptr_con;

		if (ptrs_spline->npts < 5)
		{

			if (TRUE == b_first_end)
			{
				angle = Atan2(ptrs_spline->xy[1] - ptrs_spline->xy[3], ptrs_spline->xy[0] - ptrs_spline->xy[2]);
			}
			else
			{
				if ((ptrs_spline->xy[5] == ptrs_spline->xy[7]) && (ptrs_spline->xy[4] == ptrs_spline->xy[6]))
					angle = Atan2(ptrs_spline->xy[5] - ptrs_spline->xy[3], ptrs_spline->xy[4] - ptrs_spline->xy[2]);
				else
					angle = Atan2(ptrs_spline->xy[7] - ptrs_spline->xy[5], ptrs_spline->xy[6] - ptrs_spline->xy[4]);
			}
		}
		else  //multipoint
		{

			if (TRUE == b_first_end)
			{
				//searching for construction points of first segment
				angle = get_first_bnods_direction(ptrs_spline);
			}
			else
			{
				
				//searching for construction points of last segment
				angle = get_last_bnods_direction(ptrs_spline);
			}
		}
	}

	angle *= 180 / Pi;
	return angle;
}


static void *get_obiect_to_continue(BOOL    *b_first_end,
	double  *df_xend,
	double  *df_yend)
	/*---------------------------------------------------*/
{
	EVENT *ev;
	double X0, Y0;
	void *ptr_object;
	unsigned type_object;

	redcr_continue(0);
	ptr_object = NULL;
	while (1)
	{
		view_line_type(&LiniaG);
		ev = Get_Event_Point(NULL, &X0, &Y0);
		if (ev->What == evKeyDown && ev->Number == 0)
		{
			break;
		}
		if (ev->What == evKeyDown && ev->Number == ENTER)
		{
			type_object = Blinia | Bluk | Bspline;
			if (NULL != (ptr_object = select_w(&type_object, NULL)))
			{
				get_param_end_ob(ptr_object, b_first_end, df_xend, df_yend);
				break;
			}
		}
	}
	redcr_continue(1);
	return ptr_object;
}

static BOOL set_spline_continue_param(void)
/*--------------------------------------*/
{
	double df_xend, df_yend;
	BOOL b_ret;
	double al;
    static char st[POLE_TXT_MAX];
    unsigned short cod = L' ';
	static int iconno = 0;

	getmenupini(&menu_spline, st, &cod, &iconno);
	b_ret = FALSE;
	menupini(NULL, "", ' ', 0);
	if (NULL != (pl.ptr_con = get_obiect_to_continue(&(pl.b_first_end), &df_xend, &df_yend)))
	{
		pl.xs = df_xend;
		pl.ys = df_yend;
		pl.xe = pl.xs;
		pl.ye = pl.ys;
		pl.con_angle = get_continue_angle(pl.ptr_con, pl.b_first_end);
		out_krz(pl.xs, pl.ys);

		angle_orig = get_angle_l();
		orto_orig = orto;
		angle_changed = TRUE;
			
		put_angle_l(pl.con_angle);
		orto = 1;

		komunikat0(124);

		menupini(menu_spline, st, cod, iconno);

		spline_proc(pl.xs, pl.ys);

		b_ret = TRUE;
	}
	else menupini(menu_spline, st, cod, iconno);
	return b_ret;
}

void Spline_(void)
/*---------------*/
{
	EVENT *ev;
	double X0, Y0;
	BOOL ret;

	angle_changed = FALSE;
	redcr0(0);
	line_g.typ = 7; // 65;
	bspline.kolor = 7; // kolory.blok; // Get_Current_Color();  //????
	bspline.typ = LiniaG.typ;
	bspline.warstwa = Current_Layer;

	bspline.closed = bspline__closed;

	while (1)
	{
		block_added = FALSE;
		next_spline = FALSE;
		view_line_type(&LiniaG);
		ev = Get_Event_Point(NULL, &X0, &Y0);
		switch (ev->What)
		{
		case evKeyDown:
			if (ev->Number == 0)
			{
				redcr0(1);
				return;
			}
			if (ev->Number == ENTER)
			{
				i__corner_no = 1;
				spline_proc(X0, Y0);
			}
			break;
		case evCommandP:
			if (ev->Number == 8 || ev->Number == 9 || ev->Number == 10 || ev->Number == 11) 
			    set_spline_corner_nomax(ev->Number);
			
			else if (ev->Number == 12)  //polyline yes
			{
				spline_pline_on();
			}
			else if (ev->Number == 13)  //polyline no
			{
				spline_pline_off();
			}
			else if (ev->Number == 1)  //quadratic now
			{
				if (bspline.lp == 6)
				{
					CUR_OFF(X, Y);
					add_spline();
					CUR_ON(X, Y);
				}
			}
			else if (ev->Number == 3)  //remove
			{
				previous_point();
			}
			else if (ev->Number == 4)  //continuation
			{
				if (i__corner_no < 2)
				{
					if (TRUE == set_spline_continue_param())
					{

						;
					}
				}
			}
			else if (ev->Number == 7)  //close
			{
				ret = close_spline();  //before staring the spline
			}
			else if (ev->Number == 6)  //tension
			{
				set_spline_tension(1, TRUE);
			}

			break;
		default:
			break;
		}
	}
}

void Spline(void)
/*---------------*/
{
	S3D = FALSE;

	i__corner_no = 0;


 	if (bspline__pline == TRUE)  menu_par_new((*mSpline.pola)[2].txt, _YES__);
	else menu_par_new((*mSpline.pola)[2].txt, _NO__);

	if (bspline__closed == TRUE)  menu_par_new((*mSpline.pola)[7].txt, _YES__);
	else menu_par_new((*mSpline.pola)[7].txt, _NO__);

	if (bnpts > 4) mSpline.max = 8;
	else mSpline.max = 5;

	bspline.npts = bnpts;
	curviness = curviness_set;

	Spline_();
}

BLOK *add_shadow_block(int atrybut, int to_block)
/*--------------------------------------------*/
{
    BLOK s_blockd = Bdef;
	BLOK *ptrs_block=NULL;
	char c_pltype;

	s_blockd.n = B3 + sizeof(c_pltype);
	s_blockd.kod_obiektu = B_PLINE;
	s_blockd.atrybut = atrybut;
	s_blockd.dlugosc_opisu_obiektu = sizeof(c_pltype);
    if (to_block==1) {
        s_blockd.blok=1;
        if ((ptrs_block = (BLOK *) dodaj_obiekt_reversed((BLOK*)dane, &s_blockd)) == NULL) return NULL;
    }
    else
    {
        s_blockd.blok=0;
        if ((ptrs_block = (BLOK *) dodaj_obiekt_reversed(NULL, &s_blockd)) == NULL) return NULL;
    }
	c_pltype = PL_SHADOW;
	memmove(&(ptrs_block->opis_obiektu[0]), &c_pltype, sizeof(c_pltype));
	return ptrs_block;
}

int remove_spline_shadow_flag(long_long l_offb, long_long l_offe, BOOL make_Ablok)
{
	int i = 0;
	NAGLOWEK* nag;
	long_long l_off;
    BLOK *ptr_parent;

	Object_Tok_All(&l_offb, l_offe, &l_off, ONieOkreslony);
	while (l_off != -1)
	{
		nag = (NAGLOWEK*)(dane + l_off);
        switch (nag->obiekt)
        {
            case Ospline:
                if (((SPLINE *) nag)->shadowed == 1)
                {
                    if (make_Ablok) nag->atrybut = Ablok;
                    else if (nag->atrybut == Abad) nag->atrybut = Anormalny;
                    ((SPLINE *) nag)->shadowed = 0;
                    i++;
                    ////rysuj_obiekt(nag, COPY_PUT, 1);  //be silent
                    ptr_parent = FIRSTB(nag);
                    if (ptr_parent != NULL) {
                        if (ptr_parent->kod_obiektu == B_PLINE) {
                            if (ptr_parent->atrybut == Abad) ptr_parent->atrybut = Anormalny;
                        }
                    }
                }
                break;
            case Osolidarc:
                if (((SOLIDARC *) nag)->shadowed == 1)
                {
                    if (make_Ablok) nag->atrybut = Ablok;
                    else if (nag->atrybut == Abad) nag->atrybut = Anormalny;
                    ((SOLIDARC *) nag)->shadowed = 0;
                    i++;
                    ////rysuj_obiekt(nag, COPY_PUT, 1);  //be silent
                    ptr_parent = FIRSTB(nag);
                    if (ptr_parent != NULL) {
                        if (ptr_parent->kod_obiektu == B_PLINE) {
                            if (ptr_parent->atrybut == Abad) ptr_parent->atrybut = Anormalny;
                        }
                    }
                }
                break;
            case Oellipticalarc:
                if (((ELLIPTICALARC *) nag)->shadowed == 1)
                {
                    if (make_Ablok) nag->atrybut = Ablok;
                    else if (nag->atrybut == Abad) nag->atrybut = Anormalny;
                    ((ELLIPTICALARC *) nag)->shadowed = 0;
                    i++;
                    ////rysuj_obiekt(nag, COPY_PUT, 1);  //be silent
                    ptr_parent = FIRSTB(nag);
                    if (ptr_parent != NULL) {
                        if (ptr_parent->kod_obiektu == B_PLINE) {
                            if (ptr_parent->atrybut == Abad) ptr_parent->atrybut = Anormalny;
                        }
                    }
                }
                break;
            case Oellipse:
            case Ofilledellipse:
                if (((ELLIPSE *) nag)->przec == 1)
                {
                    if (make_Ablok) nag->atrybut = Ablok;
                    else if (nag->atrybut == Abad) nag->atrybut = Anormalny;
                    ((ELLIPSE *) nag)->przec = 0;
                    i++;
                    ////rysuj_obiekt(nag, COPY_PUT, 1);  //be silent
                    ptr_parent = FIRSTB(nag);
                    if (ptr_parent != NULL) {
                        if (ptr_parent->kod_obiektu == B_PLINE) {
                            if (ptr_parent->atrybut == Abad) ptr_parent->atrybut = Anormalny;
                        }
                    }
                }
                break;
            case Oluk:
                if (((LUK *) nag)->shadowed == 1)
                {
                    if (make_Ablok) nag->atrybut = Ablok;
                    else if (nag->atrybut == Abad) nag->atrybut = Anormalny;
                    ((LUK *) nag)->shadowed = 0;
                    i++;
                    ////rysuj_obiekt(nag, COPY_PUT, 1);  //be silent
                    ptr_parent = FIRSTB(nag);
                    if (ptr_parent != NULL) {
                        if (ptr_parent->kod_obiektu == B_PLINE) {
                            if (ptr_parent->atrybut == Abad) ptr_parent->atrybut = Anormalny;
                        }
                    }
                }
                break;
            case Ookrag:
            case Okolo:
                if (((OKRAG *) nag)->przec == 1)
                {
                    if (make_Ablok) nag->atrybut = Ablok;
                    else if (nag->atrybut == Abad) nag->atrybut = Anormalny;
                    ((OKRAG *) nag)->przec = 0;
                    i++;
                    ////rysuj_obiekt(nag, COPY_PUT, 1);  //be silent
                    ptr_parent = FIRSTB(nag);
                    if (ptr_parent != NULL) {
                        if (ptr_parent->kod_obiektu == B_PLINE) {
                            if (ptr_parent->atrybut == Abad) ptr_parent->atrybut = Anormalny;
                        }
                    }
                }
                break;
            default:
                break;
        }

		Object_Tok_All(&l_offb, l_offe, &l_off, ONieOkreslony /*Ospline*/);
	}

	return i;
}

void delete_spline_shadows(BOOL make_Ablok)
{
	NAGLOWEK *nag;
	BLOK *blok;
	long_long l_off;
	int i, j;
	long_long l_offb, l_offe;
	int to_delete;
	int ret;

	l_offb = 0;
	l_offe = dane_size;
	to_delete = 0;

	Object_Tok(&l_offb, l_offe, &l_off, OdBLOK, TOK_LAYER_ON);
	while (l_off != -1)
	{
		nag = (NAGLOWEK*)(dane + l_off);
		if (nag->obiekt == OdBLOK)
		{
			blok = (BLOK *)nag;
			if (blok->opis_obiektu[0] == PL_SHADOW)
			{
                blokzap((char*)nag, (char*)((char*)nag + nag->n), ANieOkreslony, COPY_PUT, 0);
				blok->atrybut = Ausuniety;
				to_delete++;
			}
		}

		Object_Tok(&l_offb, l_offe, &l_off, OdBLOK, TOK_LAYER_ON);
	}

	ret = remove_spline_shadow_flag(0, dane_size, make_Ablok);

	if (to_delete > 0) usun_blok(dane, dane + dane_size);

	return;
}

void spline_shadows_to_plines(void)
{
	NAGLOWEK *nag;
	BLOK *blok;
	long_long l_off;
	int i, j;
	long_long l_offb, l_offe;
	int to_delete;

	l_offb = 0;
	l_offe = dane_size;
	to_delete = 0;

	Object_Tok(&l_offb, l_offe, &l_off, OdBLOK, TOK_LAYER_ON);
	while (l_off != -1)
	{
		nag = (NAGLOWEK*)(dane + l_off);
		if (nag->obiekt == OdBLOK)
		{
			blok = (BLOK *)nag;
			if (blok->opis_obiektu[0] == PL_SHADOW)
			{
				blok->opis_obiektu[0] = PL_PLINE;
			}
		}

		Object_Tok(&l_offb, l_offe, &l_off, OdBLOK, TOK_LAYER_ON);
	}

	return;
}

int make_spline_shadows(long_long l_offb, long_long l_offe, int atrybut, BOOL join, int s_atrybut, int prec, int to_block)
{
	NAGLOWEK *nag;
	long_long l_off;
	int i, j, k, ij;
	LINIA s_line = Ldef;
	float out_x[MaxNpts], out_y[MaxNpts];
	char *ptrs_line;
	float del_l;
	int dNpts, dNptsN;
	SPLINE *temp_spline;

	SPLINE tmp_spline = Splinedef;
	double bnodes_2_tangent_org, bnodes_2_tangent;
	int node_count;
    BLOK *ptr_block, *ptr_parent;

    MAX_BUF_SPLINE=1000;
    buf_spline=(SPLINE**)malloc(MAX_BUF_SPLINE*sizeof(SPLINE*));

	if (prec == 1) 
		MIN_SHADOW_SEGMENT[prec] = s_hatch_param_df_distans;


	i_spline_no = 0;
	Object_Tok(&l_offb, l_offe, &l_off, Ospline, TOK_LAYER_ON);
	while (l_off != -1)
	{
		nag = (NAGLOWEK*)(dane + l_off);
		if (nag->atrybut == Ablok)
		{
			//buforowanie spline
            if (i_spline_no == MAX_BUF_SPLINE)
            {
                MAX_BUF_SPLINE+=1000;
                buf_spline=(SPLINE**)realloc(buf_spline, MAX_BUF_SPLINE*sizeof(SPLINE*));
            }

			if (i_spline_no < MAX_BUF_SPLINE)
			{
				buf_spline[i_spline_no] = malloc(sizeof(SPLINE));
				memcpy(buf_spline[i_spline_no], nag, sizeof(SPLINE));
				i_spline_no++;
				nag->atrybut = s_atrybut;
				((SPLINE*)nag)->shadowed = 1;
                rysuj_obiekt(nag, COPY_PUT, 0);
                ptr_parent = FIRSTB(nag);
                if (ptr_parent != NULL) {
                    if (ptr_parent->kod_obiektu == B_PLINE)
                    {
                        ptr_parent->atrybut =s_atrybut;
                    }
                }
			}
		}
		Object_Tok(&l_offb, l_offe, &l_off, Ospline, TOK_LAYER_ON);
	}


    //if (i_spline_no==MAX_BUF_SPLINE)
    //{
     //   for (j = 0; j < i_spline_no; j++) free(buf_spline[j]);
     //   return 0;
    //}
	
	//generating shadows and freeing memory
	for (j = 0; j < i_spline_no; j++)
	{
		//add_shadow_block(buf_spline[j]->atrybut);
		if ((join == FALSE) || (j==0)) {
            ptr_block=add_shadow_block(atrybut, to_block);
        }
		dNpts = 90;  //for more precission

		if (buf_spline[j]->npts < 5)
		{
			calc_bspline(buf_spline[j]->lp, buf_spline[j]->npts, buf_spline[j]->xy, dNpts, out_x, out_y);
            del_l=0.0;
            for (i=0; i<(dNpts-1); i++)
                del_l += sqrt((out_x[i+1] - out_x[i]) * (out_x[i+1] - out_x[i]) + (out_y[i+1] - out_y[i]) * (out_y[i+1] - out_y[i]));
            ////del_l2 shouldn't be shorter than MIN_SHADOW_SEGMENT[prec] e,g, 1 mm but dNpts shouldn't be more than 90
            dNptsN=(int)(del_l*MIN_SHADOW_SEGMENT[prec]);

			if (dNptsN < dNpts)
			{
                dNpts=max(dNptsN, 10);
				calc_bspline(buf_spline[j]->lp, buf_spline[j]->npts, buf_spline[j]->xy, dNpts, out_x, out_y);
			}

			for (i = 0; i < dNpts - 1; i++)
			{
				s_line.atrybut = atrybut;
				s_line.obiektt2 = O2BlockPline;
				s_line.kolor = buf_spline[j]->kolor;
				s_line.blok = ElemBlok;
				s_line.warstwa = buf_spline[j]->warstwa;
				s_line.typ = buf_spline[j]->typ;
				s_line.x1 = out_x[i];
				s_line.y1 = out_y[i];
				s_line.x2 = out_x[i + 1];
				s_line.y2 = out_y[i + 1];

                PTR__GTMP7 = ptr_block;
				ptrs_line = dodaj_obiekt_reversed(ptr_block, &s_line);
                ptr_block=PTR__GTMP7;
			}
		}
		else
		{
			curviness = buf_spline[j]->xy[buf_spline[j]->lp];

			tmp_spline.npts = 4;
			tmp_spline.lp = 8;
			tmp_spline.kolor = buf_spline[j]->kolor;
			tmp_spline.typ = buf_spline[j]->typ;

			node_count = 1;

			set_bnodes(buf_spline[j], buf_spline[j]->xy, &node_count);

			calc_tangents(buf_spline[j]->closed, &node_count, &bnodes_2_tangent);

			if (buf_spline[j]->closed == 0)
			{
				dNpts = 90;

				bnodes_2_tangent_org = bnodes[2].tangent;
				bnodes[2].tangent = bnodes_2_tangent;
				get_control_points_reversed(bnodes[2], bnodes[1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, dNpts, out_x, out_y);
				//we take into account the second segment instead of 1
                del_l=0.0;
                for (i=0; i<(dNpts-1); i++)
                    del_l += sqrt((out_x[i+1] - out_x[i]) * (out_x[i+1] - out_x[i]) + (out_y[i+1] - out_y[i]) * (out_y[i+1] - out_y[i]));
				////del_l2 shouldn't be shorter than MIN_SHADOW_SEGMENT[prec] e,g, 1 mm but dNpts shouldn't be more than 90
                dNptsN=(int)(del_l*MIN_SHADOW_SEGMENT[prec]+0.5);

				if (dNptsN < dNpts)
				{
                    dNpts=max(dNptsN, 10);
					calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, dNpts, out_x, out_y);
				}

				for (i = 0; i < dNpts - 1; i++)
				{
					s_line.atrybut = atrybut;
					s_line.obiektt2 = O2BlockPline;
					s_line.kolor = buf_spline[j]->kolor;
					s_line.blok = ElemBlok;
					s_line.warstwa = buf_spline[j]->warstwa;
					s_line.typ = buf_spline[j]->typ;
					s_line.x1 = out_x[i];
					s_line.y1 = out_y[i];
					s_line.x2 = out_x[i + 1];
					s_line.y2 = out_y[i + 1];

                    PTR__GTMP7 = ptr_block;
					ptrs_line = dodaj_obiekt_reversed(ptr_block, &s_line);
                    ptr_block=PTR__GTMP7;
				}

				bnodes[2].tangent = bnodes_2_tangent_org;

			}

			for (k = 2; k < node_count - 2; k++)  //1 - extra_node
			{
				dNpts = 90;

				get_control_points(bnodes[k], bnodes[k + 1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, dNpts, out_x, out_y);
                del_l=0.0;
                for (i=0; i<(dNpts-1); i++)
                    del_l += sqrt((out_x[i+1] - out_x[i]) * (out_x[i+1] - out_x[i]) + (out_y[i+1] - out_y[i]) * (out_y[i+1] - out_y[i]));
                ////del_l2 shouldn't be shorter than MIN_SHADOW_SEGMENT[prec] e,g, 1 mm but dNpts shouldn't be more than 90
                dNptsN=(int)(del_l*MIN_SHADOW_SEGMENT[prec]);

				if (dNptsN < dNpts)
				{
                    dNpts=max(dNptsN, 10);
					calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, dNpts, out_x, out_y);
				}

				for (i = 0; i < dNpts - 1; i++)
				{
					s_line.atrybut = atrybut;
					s_line.obiektt2 = O2BlockPline;
					s_line.kolor = buf_spline[j]->kolor;
					s_line.blok = ElemBlok;
					s_line.warstwa = buf_spline[j]->warstwa;
					s_line.typ = buf_spline[j]->typ;
					s_line.x1 = out_x[i];
					s_line.y1 = out_y[i];
					s_line.x2 = out_x[i + 1];
					s_line.y2 = out_y[i + 1];

                    PTR__GTMP7 = ptr_block;
					ptrs_line = dodaj_obiekt_reversed(ptr_block, &s_line);
                    ptr_block=PTR__GTMP7;
				}
			}

			if ((node_count > 3) && (buf_spline[j]->closed == 0))
			{
				dNpts = 90;

				get_control_points_end(bnodes[node_count - 2], bnodes[node_count - 1], tmp_spline.xy);
				calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, dNpts, out_x, out_y);
                del_l=0.0;
                for (i=0; i<(dNpts-1); i++)
                    del_l += sqrt((out_x[i+1] - out_x[i]) * (out_x[i+1] - out_x[i]) + (out_y[i+1] - out_y[i]) * (out_y[i+1] - out_y[i]));
                ////del_l2 shouldn't be shorter than MIN_SHADOW_SEGMENT[prec] e,g, 1 mm but dNpts shouldn't be more than 90
                dNptsN=(int)(del_l*MIN_SHADOW_SEGMENT[prec]);

				if (dNptsN < dNpts)
				{
                    dNpts=max(dNptsN, 10);
					calc_bspline(tmp_spline.lp, tmp_spline.npts, tmp_spline.xy, dNpts, out_x, out_y);
				}

				for (i = 0; i < dNpts - 1; i++)
				{
					s_line.atrybut = atrybut;
					s_line.obiektt2 = O2BlockPline;
					s_line.kolor = buf_spline[j]->kolor;
					s_line.blok = ElemBlok;
					s_line.warstwa = buf_spline[j]->warstwa;
					s_line.typ = buf_spline[j]->typ;
					s_line.x1 = out_x[i];
					s_line.y1 = out_y[i];
					s_line.x2 = out_x[i + 1];
					s_line.y2 = out_y[i + 1];

                    PTR__GTMP7 = ptr_block;
					ptrs_line = dodaj_obiekt_reversed(ptr_block, &s_line);
                    ptr_block=PTR__GTMP7;
				}

			}

		}

		free(buf_spline[j]);
	}

    free(buf_spline);

	if ((i_spline_no>0) && (join == TRUE)) return 1;
	else return i_spline_no;
}

int make_elliptical_shadows(long_long l_offb, long_long l_offe, int atrybut, BOOL join, int s_atrybut, int prec, int to_block)
{
    NAGLOWEK *nag;
    long_long l_off;
    int i, j, k, ij;
    LINIA s_line = Ldef;
    float out_x[MaxNpts], out_y[MaxNpts];
    char *ptrs_line;
    float del_l2, a;
    int dNpts, dNptsN;
    ELLIPSE *temp_ellipse;
    ELLIPSE tmp_ellipse = Edef;
    double bnodes_2_tangent_org, bnodes_2_tangent;
    int node_count;
    BLOK *ptr_block;
    int numpoints;
    double xy[256];
    long_long l_offb_, l_offe_;

    MAX_BUF_ELLIPSE=1000;
    MAX_BUF_ELLIPTICALARC=1000;
    buf_ellipse=(ELLIPSE**)malloc(MAX_BUF_ELLIPSE*sizeof(ELLIPSE*));
    buf_ellipticalarc=(ELLIPTICALARC**)malloc(MAX_BUF_ELLIPTICALARC*sizeof(ELLIPTICALARC *));

    l_offb_ = l_offb;
    l_offe_ = l_offe;

    if (prec == 1) MIN_SHADOW_SEGMENT[prec] = s_hatch_param_df_distans;

    i_ellipse_no = 0;
    Object_Tok(&l_offb, l_offe, &l_off, Oellipse, TOK_LAYER_ON);
    while (l_off != -1)
    {
        nag = (NAGLOWEK*)(dane + l_off);
        if (nag->atrybut == Ablok)
        {
            //buforowanie ellipse
            if (i_ellipse_no == MAX_BUF_ELLIPSE)
            {
                MAX_BUF_ELLIPSE+=1000;
                buf_ellipse=(ELLIPSE**)realloc(buf_ellipse, MAX_BUF_ELLIPSE*sizeof(ELLIPSE*));
            }

            if (i_ellipse_no < MAX_BUF_ELLIPSE)
            {
                buf_ellipse[i_ellipse_no] = malloc(sizeof(ELLIPSE));
                memcpy(buf_ellipse[i_ellipse_no], nag, sizeof(ELLIPSE));
                i_ellipse_no++;
                nag->atrybut = s_atrybut;
                ((ELLIPSE*)nag)->przec = 1;  //no shadowed
                ////rysuj_obiekt(nag, COPY_PUT, 1);  //be silent
            }
        }
        Object_Tok(&l_offb, l_offe, &l_off, Oellipse, TOK_LAYER_ON);
    }

    //if (i_ellipse_no==MAX_BUF_ELLIPSE)
    //{
    //    for (j = 0; j < i_ellipse_no; j++) free(buf_ellipse[j]);
    //    return 0;
    //}

    l_offb=l_offb_;
    l_offe=l_offe_;

    Object_Tok(&l_offb, l_offe, &l_off, Ofilledellipse, TOK_LAYER_ON);
    while (l_off != -1)
    {
        nag = (NAGLOWEK*)(dane + l_off);
        if (nag->atrybut == Ablok)
        {
            //buforowanie filled ellipse
            if (i_ellipse_no == MAX_BUF_ELLIPSE)
            {
                MAX_BUF_ELLIPSE+=1000;
                buf_ellipse=(ELLIPSE**)realloc(buf_ellipse, MAX_BUF_ELLIPSE*sizeof(ELLIPSE*));
            }
            if (i_ellipse_no < MAX_BUF_ELLIPSE)
            {
                buf_ellipse[i_ellipse_no] = malloc(sizeof(ELLIPSE));
                memcpy(buf_ellipse[i_ellipse_no], nag, sizeof(ELLIPSE));
                i_ellipse_no++;
                nag->atrybut = s_atrybut;
                ((ELLIPSE*)nag)->przec = 1; //no shaowed
                ////rysuj_obiekt(nag, COPY_PUT, 1);  //be silent
            }
        }
        Object_Tok(&l_offb, l_offe, &l_off, Oellipse, TOK_LAYER_ON);
    }

    //if (i_ellipse_no==MAX_BUF_ELLIPSE)
    //{
    //    for (j = 0; j < i_ellipse_no; j++) free(buf_ellipse[j]);
    //    return 0;
    //}

    l_offb=l_offb_;
    l_offe=l_offe_;

    i_ellipticalarc_no = 0;
    Object_Tok(&l_offb, l_offe, &l_off, Oellipticalarc, TOK_LAYER_ON);
    while (l_off != -1)
    {
        nag = (NAGLOWEK*)(dane + l_off);
        if (nag->atrybut == Ablok)
        {
            //buforowanie elliptical arc
            if (i_ellipticalarc_no == MAX_BUF_ELLIPTICALARC)
            {
                MAX_BUF_ELLIPTICALARC+=1000;
                buf_ellipticalarc=(ELLIPTICALARC**)realloc(buf_ellipticalarc, MAX_BUF_ELLIPTICALARC*sizeof(ELLIPTICALARC*));
            }
            if (i_ellipticalarc_no < MAX_BUF_ELLIPTICALARC)
            {
                buf_ellipticalarc[i_ellipticalarc_no] = malloc(sizeof(ELLIPTICALARC));
                memcpy(buf_ellipticalarc[i_ellipticalarc_no], nag, sizeof(ELLIPTICALARC));
                i_ellipticalarc_no++;
                nag->atrybut = s_atrybut;
                ((ELLIPTICALARC*)nag)->shadowed = 1;
                ////rysuj_obiekt(nag, COPY_PUT, 1);  //be silent
            }
        }
        Object_Tok(&l_offb, l_offe, &l_off, Oellipticalarc, TOK_LAYER_ON);
    }

    //if (i_ellipticalarc_no==MAX_BUF_ELLIPTICALARC)
    //{
    //    for (j = 0; j < i_ellipticalarc_no; j++) free(buf_ellipticalarc[j]);
    //    return 0;
    //}

    //generating shadows and freeing memory
    for (j = 0; j < i_ellipse_no; j++)
    {
        //add_shadow_block(buf_spline[j]->atrybut);
        if ((join == FALSE) || (j==0)) {
            ptr_block=add_shadow_block(atrybut, to_block);
        }

        numpoints=Get_Ellipse_Points(buf_ellipse[j]->x, buf_ellipse[j]->y, buf_ellipse[j]->rx, buf_ellipse[j]->ry, buf_ellipse[j]->angle, xy);
        xy[numpoints] = xy[0];
        xy[numpoints+1] = xy[1];
        numpoints+=2;

        for (i = 0; i < numpoints - 2; i+=2)
            {
                s_line.atrybut = atrybut;
                s_line.obiektt2 = O2BlockPline;
                s_line.kolor = buf_ellipse[j]->kolor;
                s_line.blok = ElemBlok;
                s_line.warstwa = buf_ellipse[j]->warstwa;
                s_line.typ = buf_ellipse[j]->typ;
                s_line.x1 = xy[i];
                s_line.y1 = xy[i+1];
                s_line.x2 = xy[i + 2];
                s_line.y2 = xy[i + 3];

                PTR__GTMP7 = ptr_block;
                ptrs_line = dodaj_obiekt_reversed(ptr_block, &s_line);
                ptr_block=PTR__GTMP7;

            }

        free(buf_ellipse[j]);
    }

    free(buf_ellipse);

    for (j = 0; j < i_ellipticalarc_no; j++)
    {
        //add_shadow_block(buf_spline[j]->atrybut);
        if ((join == FALSE) || (j==0)) {
            ptr_block=add_shadow_block(atrybut, to_block);
        }

        numpoints=Get_EllipticalArc_Points(buf_ellipticalarc[j]->x, buf_ellipticalarc[j]->y, buf_ellipticalarc[j]->rx, buf_ellipticalarc[j]->ry, buf_ellipticalarc[j]->angle, buf_ellipticalarc[j]->kat1, buf_ellipticalarc[j]->kat2, xy);

        for (i = 0; i < numpoints - 2; i+=2)
        {
            s_line.atrybut = atrybut;
            s_line.obiektt2 = O2BlockPline;
            s_line.kolor = buf_ellipticalarc[j]->kolor;
            s_line.blok = ElemBlok;
            s_line.warstwa = buf_ellipticalarc[j]->warstwa;
            s_line.typ = buf_ellipticalarc[j]->typ;
            s_line.x1 = xy[i];
            s_line.y1 = xy[i+1];
            s_line.x2 = xy[i + 2];
            s_line.y2 = xy[i + 3];

            PTR__GTMP7 = ptr_block;
            ptrs_line = dodaj_obiekt_reversed(ptr_block, &s_line);
            ptr_block=PTR__GTMP7;

        }

        free(buf_ellipticalarc[j]);
    }

    free(buf_ellipticalarc);

    if (((i_ellipse_no+i_ellipticalarc_no)>0) && (join == TRUE)) return 1;
    else return (i_ellipse_no+i_ellipticalarc_no);
}


int make_arc_shadows(long_long l_offb, long_long l_offe, int atrybut, BOOL join, int s_atrybut, int prec, int to_block)
{
	NAGLOWEK *nag;
	int i, j;
	LINIA s_line = Ldef;
	char *ptrs_line;
	BOOL b_ret;

    //LUK **buf_arc_;

	long_long l_offp, l_off;
	BLOK *ptrs_bl;

	double df_basex = 0.0; 
	double df_basey = 0.0;
	double df_sx = 1.0;
	double df_sy = 1.0;

    MAX_BUF_ARC=1000;
    MAX_BUF_CIRCLE=1000;
    buf_arc=(LUK**)malloc(MAX_BUF_ARC*sizeof(LUK*));
    buf_circle=(OKRAG**)malloc(MAX_BUF_CIRCLE*sizeof(OKRAG*));

	i_arc_no = 0;
    i_circle_no = 0;

	
	Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON);
	while (l_off != -1)
	{
		nag = (NAGLOWEK*)(dane + l_off);
		if (nag->atrybut == Ablok)
		{
			if (nag->obiekt == Oluk)
			{

                if (i_arc_no == MAX_BUF_ARC)
                {
                    MAX_BUF_ARC+=1000;
                    buf_arc=(LUK**)realloc(buf_arc, MAX_BUF_ARC*sizeof(LUK*));
                    //buf_arc=buf_arc_;
                }
				//buforowanie luku
				if (i_arc_no < MAX_BUF_ARC)
				{
					buf_arc[i_arc_no] = malloc(sizeof(LUK));
					memcpy(buf_arc[i_arc_no], nag, sizeof(LUK));
					i_arc_no++;
					nag->atrybut = s_atrybut;
                    ((LUK*)nag)->shadowed = 1;
                    ////rysuj_obiekt(nag, COPY_PUT, 1);  //be silent
				}
			}
			else if ((nag->obiekt == Ookrag) || (nag->obiekt == Okolo))
            {
                //buforowanie okregu
                if (i_circle_no == MAX_BUF_CIRCLE) {
                    MAX_BUF_CIRCLE += 1000;
                    buf_circle = (OKRAG **) realloc(buf_circle, MAX_BUF_CIRCLE * sizeof(OKRAG *));
                }
                if (i_circle_no < MAX_BUF_CIRCLE)
                {
                    buf_circle[i_circle_no] = malloc(sizeof(OKRAG));
                    memcpy(buf_circle[i_circle_no], nag, sizeof(OKRAG));
                    i_circle_no++;
                    nag->atrybut = s_atrybut;
                    ((OKRAG *) nag)->przec = 1;
                    ////rysuj_obiekt(nag, COPY_PUT, 1);  //be silent
                }
            }
		}

		Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON);
	}

    //if ((i_arc_no==MAX_BUF_ARC) || (i_circle_no==MAX_BUF_CIRCLE))
    //{
    //    for (j = 0; j < i_arc_no; j++) free(buf_arc[j]);
    //    for (j = 0; j < i_circle_no; j++) free(buf_circle[j]);
    //    return 0;
    //}

		//generating arcs and freeing memory
		for (j = 0; j < i_arc_no; j++)
		{
			b_ret = trans_scale_arc_shadow(buf_arc[j], df_basex, df_basey, df_sx, df_sy, to_block);
			free(buf_arc[j]);
		}

    free(buf_arc);

        //generating circles and freeing memory
		for (j = 0; j < i_circle_no; j++)
		{
			b_ret = trans_scale_circle_shadow(buf_circle[j], df_basex, df_basey, df_sx, df_sy, to_block);
			free(buf_circle[j]);
		}

    free(buf_circle);

	if (join == TRUE) return 1;
	else return i_arc_no + i_circle_no;
}

int make_solidarc_shadows(long_long l_offb, long_long l_offe, int atrybut, BOOL join, int s_atrybut, int prec, int to_block)
{
    long_long l_offp, l_off;
    NAGLOWEK *nag;
    int j;
    int b_ret;
    LINIA L_left=Ldef, L_right=Ldef;
    LUK l_inner1=ldef, l_inner2=ldef, l_outer1=ldef, l_outer2=ldef;
    BLOK *ptr_block;
    int ln;

    MAX_BUF_SOLIDARC=1000;
    buf_solidarc=(SOLIDARC**)malloc(MAX_BUF_SOLIDARC*sizeof(SOLIDARC*));

    i_solidarc_no=0;

    Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON);
    while (l_off != -1)
    {
        nag = (NAGLOWEK*)(dane + l_off);
        if (nag->atrybut == Ablok)
        {
            if (nag->obiekt == Osolidarc)
            {
                if (i_solidarc_no == MAX_BUF_SOLIDARC)
                {
                    MAX_BUF_SOLIDARC+=1000;
                    buf_solidarc=(SOLIDARC**)realloc(buf_solidarc, MAX_BUF_SOLIDARC*sizeof(SOLIDARC*));
                }
                //solidarc buffering
                if (i_solidarc_no < MAX_BUF_SOLIDARC)
                {
                    buf_solidarc[i_solidarc_no] = malloc(sizeof(SOLIDARC));
                    memcpy(buf_solidarc[i_solidarc_no], nag, sizeof(SOLIDARC));
                    i_solidarc_no++;
                    nag->atrybut = s_atrybut;
                    ((SOLIDARC*)nag)->shadowed = 1;
                    ////rysuj_obiekt(nag, COPY_PUT, 1);  //be silent
                }
            }
        }

        Object_Tok(&l_offb, l_offe, &l_off, ONieOkreslony, TOK_LAYER_ON);
    }

    //if (i_solidarc_no==MAX_BUF_SOLIDARC)
    //{
    //    for (j = 0; j < i_solidarc_no; j++) free(buf_solidarc[j]);
    //    return 0;
    //}

    //generating solidarc elements and freeing memory
    for (j = 0; j < i_solidarc_no; j++)
    {
        ptr_block=add_block(B_SHADOW, PL_SHADOW);
        if (ptr_block==NULL) return 0;
        ptr_block->atrybut=atrybut;
        ln=solidarc_elements(buf_solidarc[j], &L_left, &L_right, &l_inner1, &l_inner2, &l_outer1, &l_outer2);
        L_left.blok=ElemBlok;
        L_left.atrybut=atrybut;
        L_right.blok=ElemBlok;
        L_right.atrybut=atrybut;
        l_inner1.blok=ElemBlok;
        l_inner1.atrybut=atrybut;
        l_outer1.blok=ElemBlok;
        l_outer1.atrybut=atrybut;
        dodaj_obiekt((BLOK*)dane, &L_right);
        dodaj_obiekt((BLOK*)dane, &L_left);
        dodaj_obiekt((BLOK*)dane, &l_inner1);
        dodaj_obiekt((BLOK*)dane, &l_outer1);
        if (ln>1)
        {
            l_inner2.blok=ElemBlok;
            l_inner2.atrybut=atrybut;
            l_outer2.blok=ElemBlok;
            l_outer2.atrybut=atrybut;
            dodaj_obiekt((BLOK*)dane, &l_inner2);
            dodaj_obiekt((BLOK*)dane, &l_outer2);
        }

        free(buf_solidarc[j]);
    }
    free(buf_solidarc);

   return i_solidarc_no;
}

BOOL Get_Curviness_Only(void)
{
	return set_curviness_only;
}

#undef __O_SPLINE__
