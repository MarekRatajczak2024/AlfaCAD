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

#define __O_OKRAG__
#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include <float.h> // Required for FLT_MAX and DBL_MAX
#include<stdio.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "bib_edit.h"

#include "menu.h"

static BOOL  b_okrag ;
static LINIA line_g = Ldef ;
static LINIA line_g0 = Ldef;
double df__dx, df__dy ;
static int np = -1;


extern int przeciecieLL_tt (double *x, double *y, void *adr, void *adr1, double *t1, double *t2);
extern void setwritemode( int mode );
extern int linestyle_xor(int typ);
extern void orto_l(LINIA *L, int *Orto_Dir);
extern void view_line_type(LINIA *L);

extern BOOL get_pattern_count(void);
extern void set_pattern_count(BOOL p_count);
extern void set_pattern_offset(int offs);

static void (*cursor_on)(double ,double)=out_cur_on;
static void (*cursor_off)(double ,double)=out_cur_off;

enum CIRCLE_MODE {CIR_CEN_RAD = 0, CIR_CEN_DIA, CIR_2POINT, CIR_3POINT, CIR_TTR} ;

typedef struct
{
  double df_xc, df_yc ;		//srodek okregu
  double df_x1, df_y1 ;		//punkt przez ktory przechodzi okrag
  double df_x2, df_y2 ;		//punkt przez ktory przechodzi okrag
  double df_x3, df_y3 ;		//punkt przez ktory przechodzi okrag
  double df_rad, df_dia ;	//promien, srednica
  int i_mode ;			//sposob rysowania okregu
}
t_circle ;

typedef struct
{
  void *ptr_ob1 ;
  void *ptr_ob2 ;
  double df_xc, df_yc;
  double df_rad ;
  double df_x1, df_y1 ;
  double df_x2, df_y2 ;
  double df_xr, df_yr;
  int sector;
}
t_circle_TTR ;

static t_circle  s__circle = {0,0, 0,0, 0,0, 0,0, 0,0, CIR_CEN_RAD} ;

static t_circle_TTR  s__circle_TTR = { 0,0, 0,0, 10, 0,0, 0,0 };

static char *ptrsz__circle_type;
static double df__rad_TTR = 10 ;

static double df_x0, df_y0;

static TMENU mCircle= {5, 0,0,19,56,4,ICONS | TADD,CMNU,CMBR,CMTX,0,COMNDmnr, 0,0,0,&pmCircle, NULL,NULL} ;

static void set_circle_mode (int) ;
static void change_mode (int) ;
static void out_circle_message0 (int) ;
static BOOL get_point (double *, double *) ;
static void out_circle_rad_param (BOOL) ;
void circle_cen_rad_dia(double, double);
static void redcr_circle(int);
static void out_circle(OKRAG *, int, int);
static BOOL circle_ttr(double, double, BOOL);

static int strwyj;
static double Rstr;

/*-----------------------------------------------------------------------*/

//enum PlaneType { XY_PLANE, XZ_PLANE, YZ_PLANE };

/*
 * Transform a circle in the specified plane (with center in Cartesian screen coordinates and radius r in world units)
 * to an equivalent ellipse in Cartesian screen coordinates for isometric projection.
 *
 * Inputs:
 *   plane - The plane of the circle (XY horizontal, XZ or YZ vertical)
 *   x, y  - Center of the circle in Cartesian (screen) coordinates
 *   r     - Radius of the circle in world units (same as krok_g units)
 *
 * Outputs:
 *   xe, ye - Center of the ellipse in Cartesian coordinates (same as input x,y)
 *   rx     - Semi-axis along Cartesian X (before rotation)
 *   ry     - Semi-axis along Cartesian Y (before rotation)
 *   a      - Rotation angle of the ellipse in degrees (angle of the X semi-axis relative to Cartesian X-axis)
 *
 * Returns: 0 on success, -1 on invalid pointers
 */
int circle_to_isometric_ellipse(enum PlaneType plane, double x, double y, double r,
                                double *xe, double *ye, double *rx, double *ry, double *a) {
    if (!xe || !ye || !rx || !ry || !a || r < 0.0) return -1;

    // Center remains the same in screen coordinates
    *xe = x;
    *ye = y;

    // Precomputed scales
    double sqrt_3_over_2 = sqrt(3.0 / 2.0);  // ≈ 1.224744871391589
    double one_over_sqrt_2 = 1.0 / sqrt(2.0);  // ≈ 0.7071067811865475

    switch (plane) {
        case XY_PLANE:  // Horizontal: axis-aligned, wider in X
            *rx = r * sqrt_3_over_2;
            *ry = r * one_over_sqrt_2;
            *a = 0.0;
            break;

        case XZ_PLANE:  // Vertical XZ: rotated -30°, taller in Y
            *rx = r * one_over_sqrt_2;
            *ry = r * sqrt_3_over_2;
            *a = -30.0;
            break;

        case YZ_PLANE:  // Vertical YZ: rotated +30°, taller in Y
            *rx = r * one_over_sqrt_2;
            *ry = r * sqrt_3_over_2;
            *a = 30.0;
            break;

        default:
            return -1;
    }

    return 0;
}

int circle_to_isometric_ellipse_o_e(enum PlaneType plane, OKRAG *o, ELLIPSE *e)
        {
    if (!o || !e || o->r <= 0.0) return -1;

    // Center remains the same in screen coordinates
    e->x = o->x;
    e->y = o->y;

    // Precomputed scales
    double sqrt_3_over_2 = sqrt(3.0 / 2.0);  // ≈ 1.224744871391589
    double one_over_sqrt_2 = 1.0 / sqrt(2.0);  // ≈ 0.7071067811865475

    switch (plane) {
        case XY_PLANE:  // Horizontal: axis-aligned, wider in X
            e->rx = o->r * (float)sqrt_3_over_2;
            e->ry = o->r * (float)one_over_sqrt_2;
            e->angle = 0.0f;
            break;

        case XZ_PLANE:  // Vertical XZ: rotated -30°, taller in Y
            e->rx = o->r * (float)one_over_sqrt_2;
            e->ry = o->r * (float)sqrt_3_over_2;
            e->angle = 5.759586532f; //-30.0f;
            break;

        case YZ_PLANE:  // Vertical YZ: rotated +30°, taller in Y
            e->rx = o->r * (float)one_over_sqrt_2;
            e->ry = o->r * (float)sqrt_3_over_2;
            e->angle = 0.523598776f; //30.0f;
            break;

        default:
            return -1;
    }

    return 0;
}

static BOOL circle_center_TTR_OL(/*t_circle_TTR *ptrs__circle_TTR,*/ OKRAG *o, LINIA *l, const double *x0,  const double *y0, double r, double *x,  double *y)
{
    double dx = l->x2 - l->x1;
    double dy = l->y2 - l->y1;
    double L = sqrt(dx * dx + dy * dy);
    if (L == 0) return FALSE;

    // Normal vector (a, b) and line constant c for ax + by + c_orig = 0
    double a = -dy / L;
    double b = dx / L;
    double c_orig = -(a * l->x1 + b * l->y1);

    double R = o->r + r;
    double best_x, best_y;
    double min_dist = DBL_MAX;
    BOOL found = FALSE;

    double offsets[2] = { r, -r };

    for (int i = 0; i < 2; i++) {
        double cc = c_orig + offsets[i];

        // Shortest distance from circle center (x, y) to this offset line
        double dist_to_line = fabs(a * o->x + b * o->y + cc);

        // A tangent circle only exists if the expanded radius can reach the line
        if (dist_to_line <= R) {
            double sol_x[2], sol_y[2];

            if (fabs(a) > fabs(b)) {
                double m = -b / a, k = -cc / a - o->x;
                double AA = m * m + 1, BB = 2 * m * k - 2 * o->y, CC = k * k + o->y * o->y - R * R;
                double det = sqrt(fmax(0, BB * BB - 4 * AA * CC));
                sol_y[0] = (-BB + det) / (2 * AA); sol_y[1] = (-BB - det) / (2 * AA);
                sol_x[0] = (-b * sol_y[0] - cc) / a; sol_x[1] = (-b * sol_y[1] - cc) / a;
            } else {
                double m = -a / b, k = -cc / b - o->y;
                double AA = m * m + 1, BB = 2 * m * k - 2 * o->x, CC = k * k + o->x * o->x - R * R;
                double det = sqrt(fmax(0, BB * BB - 4 * AA * CC));
                sol_x[0] = (-BB + det) / (2 * AA); sol_x[1] = (-BB - det) / (2 * AA);
                sol_y[0] = (-a * sol_x[0] - cc) / b; sol_y[1] = (-a * sol_x[1] - cc) / b;
            }

            for (int j = 0; j < 2; j++) {
                double d = hypot(sol_x[j] - *x0, sol_y[j] - *y0);
                if (d < min_dist) {
                    min_dist = d;
                    best_x = sol_x[j];
                    best_y = sol_y[j];
                    found = TRUE;
                }
            }
        }
    }

    if (found) {
        *x = best_x;
        *y = best_y;
        return TRUE;
    }

    return FALSE;
}

static BOOL circle_center_TTR_OO(/*t_circle_TTR *ptrs__circle_TTR,*/ OKRAG *o1, OKRAG *o2, const double *x0,  const double *y0, double r, double *x,  double *y)
{
    // 1. Calculate side lengths of the triangle formed by the centers
    double side_a = o2->r + r;
    double side_b = o1->r + r;
    double dx = o2->x - o1->x;
    double dy = o2->y - o1->y;
    double side_c = hypot(dx, dy);

    // Check for existence: third circle must be able to touch both
    if (side_c > (side_a + side_b) || side_c < fabs(side_a - side_b) || side_c == 0) return FALSE;

    // 2. Use Law of Cosines to find the angle at center 1
    double cos_alpha = (side_b * side_b + side_c * side_c - side_a * side_a) / (2 * side_b * side_c);
    double alpha = acos(cos_alpha);

    // 3. Base angle of the vector connecting center 1 and center 2
    double base_angle = atan2(dy, dx);

    // 4. Calculate the two possible center points
    double sol1_x = o1->x + side_b * cos(base_angle + alpha);
    double sol1_y = o1->y + side_b * sin(base_angle + alpha);
    double sol2_x = o1->x + side_b * cos(base_angle - alpha);
    double sol2_y = o1->y + side_b * sin(base_angle - alpha);

    // 5. Select the one closer to the provided "desire position" <*x, *y>
    double dist1 = hypot(sol1_x - *x0, sol1_y - *y0);
    double dist2 = hypot(sol2_x - *x0, sol2_y - *y0);

    if (dist1 < dist2) {
        *x = sol1_x;
        *y = sol1_y;
    } else {
        *x = sol2_x;
        *y = sol2_y;
    }
    return TRUE;
}


static BOOL circle_center_TTR_LL(t_circle_TTR *ptrs__circle_TTR, double *cx0, double *cy0, double *tx, double *ty)
//-------------------------------------------------------------------------------------------------------------------
{
	LINIA *ptrs_line1, *ptrs_line2;
	double ax1, bx1, ay1, by1, ax2, bx2, ay2, by2;
	double s1x, s1y, s2x, s2y, k1, k2, v1x, v1y, v2x, v2y;
	double le1, le2;
	double R;
	double px1, py1, px2, py2, sp1x, sp1y, sp2x, sp2y;
	double den, cx, cy, cx1, cy1;
	double tx1, ty1, tx2, ty2;
	double PRECISION = 1.0E-5;
	double cx00[4], cy00[4], tx00[4], ty00[4];
	int i,ii;
	double mindistance = 1.0E+9;
	double distance;
	double middlepoint_x, middlepoint_y;

	//Let line s1 from A1 = { ax1,ay1 } to B1 = { bx1,by1 } and s2 from A2 = { ax2,ay2 } to B2 = { bx2,by2 }
    //Using parametric representation, a point in s1 is
	ptrs_line1 = (LINIA*)ptrs__circle_TTR->ptr_ob1;
	ptrs_line2 = (LINIA*)ptrs__circle_TTR->ptr_ob2;
	R = ptrs__circle_TTR->df_rad;
    
	if ((ptrs__circle_TTR->sector == 0) || (ptrs__circle_TTR->sector == 1))
	{
		//LEFT
		ax1 = ptrs_line1->x1;
		ay1 = ptrs_line1->y1;
		bx1 = ptrs_line1->x2;
		by1 = ptrs_line1->y2;

		ax2 = ptrs_line2->x1;
		ay2 = ptrs_line2->y1;
		bx2 = ptrs_line2->x2;
		by2 = ptrs_line2->y2;

		// k1 is a different value for each point and{ v1x,v1y } is unitary vector in direction A1, B1:
		le1 = sqrt((bx1 - ax1)*(bx1 - ax1) + (by1 - ay1)*(by1 - ay1));     //length of A1-B1 segment
		if (fabs(le1) < PRECISION) return FALSE;
		v1x = (bx1 - ax1) / le1;
		v1y = (by1 - ay1) / le1;

		////s1x = ax1 + k1 * v1x;
		////s1y = ay1 + k1 * v1y;

		//Same goes for line s2 :
		//with
		le2 = sqrt((bx2 - ax2)*(bx2 - ax2) + (by2 - ay2)*(by2 - ay2)); //length of A2-B2 segment
		if (fabs(le2) < PRECISION) return FALSE;

		v2x = (bx2 - ax2) / le2;
		v2y = (by2 - ay2) / le2;

		////s2x = ax2 + k2 * v2x;
		////s2y = ay2 + k2 * v2y;

		//Now, to get a parallel to s1 we just change point A1 by adding a vector vp1 perpendicular to A1 - B1 with length d = R.The perpendicular unitary vector is : vpu1 = { -v1y, v1x }.Attention : if you want the other solution, at the other side of both lines, then use{ v1y, -v1x } instead.
		//So, with vp1 = vpu1R and vp2 = { -v2y, v2x }R the new points for parallels to s1 and s2 are
		px1 = ax1 - v1y * R;
		py1 = ay1 + v1x * R;
		px2 = ax2 - v2y * R;
		py2 = ay2 + v2x * R;

		//with
		//sp1x = sp2x
		//sp1y = sp2y
		//Solving for k1, k2, sp1x, sp1y we get :
		den = v1x * v2y - v2x * v1y;
		if (fabs(den) < PRECISION) return FALSE;

		k1 = (v2y*(px2 - px1) - v2x * (py2 - py1)) / den;
		k2 = (v1y*(px2 - px1) - v1x * (py2 - py1)) / den;

		//Now we must find the intersection C of theses lines :
		sp1x = px1 + k1 * v1x;
		sp1y = py1 + k1 * v1y;
		sp2x = px2 + k2 * v2x;
		sp2y = py2 + k2 * v2y;

		cx = px1 + k1 * v1x;
		cx1 = px2 + k2 * v2x;  //choose one
		cy = py1 + k1 * v1y;
		cy1 = py2 + k2 * v2y;  //choose one
		//Pay attention to the value of den.It abs(den) < smallValue(IOW, this denominator is close to zero), then s1 and s2 are parallel or the same straight.In this case, there isn't a solution.
		//Finally, the tangent points are now easy :
		tx1 = ax1 + k1 * v1x;
		ty1 = ay1 + k1 * v1y;
		tx2 = ax2 + k2 * v2x;
		ty2 = ay2 + k2 * v2x;

		cx00[0] = cx;
		cy00[0] = cy;

		tx00[0] = tx1;
		ty00[0] = ty1;
	}
	if ((ptrs__circle_TTR->sector == 0) || (ptrs__circle_TTR->sector == 2))
	{
		//TOP

		ax1 = ptrs_line1->x2;
		ay1 = ptrs_line1->y2;
		bx1 = ptrs_line1->x1;
		by1 = ptrs_line1->y1;


		ax2 = ptrs_line2->x1;
		ay2 = ptrs_line2->y1;
		bx2 = ptrs_line2->x2;
		by2 = ptrs_line2->y2;


		// k1 is a different value for each point and{ v1x,v1y } is unitary vector in direction A1, B1:
		le1 = sqrt((bx1 - ax1)*(bx1 - ax1) + (by1 - ay1)*(by1 - ay1));     //length of A1-B1 segment
		if (fabs(le1) < PRECISION) return FALSE;
		v1x = (bx1 - ax1) / le1;
		v1y = (by1 - ay1) / le1;

		////s1x = ax1 + k1 * v1x;
		////s1y = ay1 + k1 * v1y;

		//Same goes for line s2 :
		//with
		le2 = sqrt((bx2 - ax2)*(bx2 - ax2) + (by2 - ay2)*(by2 - ay2)); //length of A2-B2 segment
		if (fabs(le2) < PRECISION) return FALSE;

		v2x = (bx2 - ax2) / le2;
		v2y = (by2 - ay2) / le2;

		////s2x = ax2 + k2 * v2x;
		////s2y = ay2 + k2 * v2y;

		//Now, to get a parallel to s1 we just change point A1 by adding a vector vp1 perpendicular to A1 - B1 with length d = R.The perpendicular unitary vector is : vpu1 = { -v1y, v1x }.Attention : if you want the other solution, at the other side of both lines, then use{ v1y, -v1x } instead.
		//So, with vp1 = vpu1R and vp2 = { -v2y, v2x }R the new points for parallels to s1 and s2 are
		px1 = ax1 - v1y * R;
		py1 = ay1 + v1x * R;
		px2 = ax2 - v2y * R;
		py2 = ay2 + v2x * R;

		//with
		//sp1x = sp2x
		//sp1y = sp2y
		//Solving for k1, k2, sp1x, sp1y we get :
		den = v1x * v2y - v2x * v1y;
		if (fabs(den) < PRECISION) return FALSE;

		k1 = (v2y*(px2 - px1) - v2x * (py2 - py1)) / den;
		k2 = (v1y*(px2 - px1) - v1x * (py2 - py1)) / den;

		//Now we must find the intersection C of theses lines :
		sp1x = px1 + k1 * v1x;
		sp1y = py1 + k1 * v1y;
		sp2x = px2 + k2 * v2x;
		sp2y = py2 + k2 * v2y;

		cx = px1 + k1 * v1x;
		cx1 = px2 + k2 * v2x;  //choose one
		cy = py1 + k1 * v1y;
		cy1 = py2 + k2 * v2y;  //choose one
		//Pay attention to the value of den.It abs(den) < smallValue(IOW, this denominator is close to zero), then s1 and s2 are parallel or the same straight.In this case, there isn't a solution.
		//Finally, the tangent points are now easy :
		tx1 = ax1 + k1 * v1x;
		ty1 = ay1 + k1 * v1y;
		tx2 = ax2 + k2 * v2x;
		ty2 = ay2 + k2 * v2x;

		cx00[1] = cx;
		cy00[1] = cy;

		tx00[1] = tx1;
		ty00[1] = ty1;
	}

	if ((ptrs__circle_TTR->sector == 0) || (ptrs__circle_TTR->sector == 3))
	{
		//RIGHT

		ax1 = ptrs_line1->x2;
		ay1 = ptrs_line1->y2;
		bx1 = ptrs_line1->x1;
		by1 = ptrs_line1->y1;

		ax2 = ptrs_line2->x2;
		ay2 = ptrs_line2->y2;
		bx2 = ptrs_line2->x1;
		by2 = ptrs_line2->y1;

		// k1 is a different value for each point and{ v1x,v1y } is unitary vector in direction A1, B1:
		le1 = sqrt((bx1 - ax1)*(bx1 - ax1) + (by1 - ay1)*(by1 - ay1));     //length of A1-B1 segment
		if (fabs(le1) < PRECISION) return FALSE;
		v1x = (bx1 - ax1) / le1;
		v1y = (by1 - ay1) / le1;

		////s1x = ax1 + k1 * v1x;
		////s1y = ay1 + k1 * v1y;

		//Same goes for line s2 :
		//with
		le2 = sqrt((bx2 - ax2)*(bx2 - ax2) + (by2 - ay2)*(by2 - ay2)); //length of A2-B2 segment
		if (fabs(le2) < PRECISION) return FALSE;

		v2x = (bx2 - ax2) / le2;
		v2y = (by2 - ay2) / le2;

		////s2x = ax2 + k2 * v2x;
		////s2y = ay2 + k2 * v2y;

		//Now, to get a parallel to s1 we just change point A1 by adding a vector vp1 perpendicular to A1 - B1 with length d = R.The perpendicular unitary vector is : vpu1 = { -v1y, v1x }.Attention : if you want the other solution, at the other side of both lines, then use{ v1y, -v1x } instead.
		//So, with vp1 = vpu1R and vp2 = { -v2y, v2x }R the new points for parallels to s1 and s2 are
		px1 = ax1 - v1y * R;
		py1 = ay1 + v1x * R;
		px2 = ax2 - v2y * R;
		py2 = ay2 + v2x * R;

		//with
		//sp1x = sp2x
		//sp1y = sp2y
		//Solving for k1, k2, sp1x, sp1y we get :
		den = v1x * v2y - v2x * v1y;
		if (fabs(den) < PRECISION) return FALSE;

		k1 = (v2y*(px2 - px1) - v2x * (py2 - py1)) / den;
		k2 = (v1y*(px2 - px1) - v1x * (py2 - py1)) / den;

		//Now we must find the intersection C of theses lines :
		sp1x = px1 + k1 * v1x;
		sp1y = py1 + k1 * v1y;
		sp2x = px2 + k2 * v2x;
		sp2y = py2 + k2 * v2y;

		cx = px1 + k1 * v1x;
		cx1 = px2 + k2 * v2x;  //choose one
		cy = py1 + k1 * v1y;
		cy1 = py2 + k2 * v2y;  //choose one
		//Pay attention to the value of den.It abs(den) < smallValue(IOW, this denominator is close to zero), then s1 and s2 are parallel or the same straight.In this case, there isn't a solution.
		//Finally, the tangent points are now easy :
		tx1 = ax1 + k1 * v1x;
		ty1 = ay1 + k1 * v1y;
		tx2 = ax2 + k2 * v2x;
		ty2 = ay2 + k2 * v2x;

		cx00[2] = cx;
		cy00[2] = cy;

		tx00[2] = tx1;
		ty00[2] = ty1;

	}

	if ((ptrs__circle_TTR->sector == 0) || (ptrs__circle_TTR->sector == 4))
	{
		//BOTTOM

		ax1 = ptrs_line1->x1;
		ay1 = ptrs_line1->y1;
		bx1 = ptrs_line1->x2;
		by1 = ptrs_line1->y2;


		ax2 = ptrs_line2->x2;
		ay2 = ptrs_line2->y2;
		bx2 = ptrs_line2->x1;
		by2 = ptrs_line2->y1;

		// k1 is a different value for each point and{ v1x,v1y } is unitary vector in direction A1, B1:
		le1 = sqrt((bx1 - ax1)*(bx1 - ax1) + (by1 - ay1)*(by1 - ay1));     //length of A1-B1 segment
		if (fabs(le1) < PRECISION) return FALSE;
		v1x = (bx1 - ax1) / le1;
		v1y = (by1 - ay1) / le1;

		////s1x = ax1 + k1 * v1x;
		////s1y = ay1 + k1 * v1y;

		//Same goes for line s2 :
		//with
		le2 = sqrt((bx2 - ax2)*(bx2 - ax2) + (by2 - ay2)*(by2 - ay2)); //length of A2-B2 segment
		if (fabs(le2) < PRECISION) return FALSE;

		v2x = (bx2 - ax2) / le2;
		v2y = (by2 - ay2) / le2;

		////s2x = ax2 + k2 * v2x;
		////s2y = ay2 + k2 * v2y;

		//Now, to get a parallel to s1 we just change point A1 by adding a vector vp1 perpendicular to A1 - B1 with length d = R.The perpendicular unitary vector is : vpu1 = { -v1y, v1x }.Attention : if you want the other solution, at the other side of both lines, then use{ v1y, -v1x } instead.
		//So, with vp1 = vpu1R and vp2 = { -v2y, v2x }R the new points for parallels to s1 and s2 are
		px1 = ax1 - v1y * R;
		py1 = ay1 + v1x * R;
		px2 = ax2 - v2y * R;
		py2 = ay2 + v2x * R;

		//with
		//sp1x = sp2x
		//sp1y = sp2y
		//Solving for k1, k2, sp1x, sp1y we get :
		den = v1x * v2y - v2x * v1y;
		if (fabs(den) < PRECISION) return FALSE;

		k1 = (v2y*(px2 - px1) - v2x * (py2 - py1)) / den;
		k2 = (v1y*(px2 - px1) - v1x * (py2 - py1)) / den;

		//Now we must find the intersection C of theses lines :
		sp1x = px1 + k1 * v1x;
		sp1y = py1 + k1 * v1y;
		sp2x = px2 + k2 * v2x;
		sp2y = py2 + k2 * v2y;

		cx = px1 + k1 * v1x;
		cx1 = px2 + k2 * v2x;  //choose one
		cy = py1 + k1 * v1y;
		cy1 = py2 + k2 * v2y;  //choose one
		//Pay attention to the value of den.It abs(den) < smallValue(IOW, this denominator is close to zero), then s1 and s2 are parallel or the same straight.In this case, there isn't a solution.
		//Finally, the tangent points are now easy :
		tx1 = ax1 + k1 * v1x;
		ty1 = ay1 + k1 * v1y;
		tx2 = ax2 + k2 * v2x;
		ty2 = ay2 + k2 * v2x;

		cx00[3] = cx;
		cy00[3] = cy;

		tx00[3] = tx1;
		ty00[3] = ty1;
	}

	if (ptrs__circle_TTR->sector == 0)
	{

		middlepoint_x = (ptrs__circle_TTR->df_x1 + ptrs__circle_TTR->df_x2) / 2.0;
		middlepoint_y = (ptrs__circle_TTR->df_y1 + ptrs__circle_TTR->df_y2) / 2.0;

		ii = 0;

		for (i = 0; i < 4; i++)
		{
			distance = ((cx00[i] - middlepoint_x)*(cx00[i] - middlepoint_x) + (cy00[i] - middlepoint_y)*(cy00[i] - middlepoint_y));  ///square of distance
			if (distance < mindistance)
			{
				mindistance = distance;
				ii = i;
			}
		}
	

		*cx0 = cx00[ii];
		*cy0 = cy00[ii];
		*tx = tx00[ii];
		*ty = ty00[ii];
		ptrs__circle_TTR->sector = ii + 1;

	}
	else
	{
		*cx0 = cx;
		*cy0 = cy;
		*tx = tx1;
		*ty = ty2;
	}
	

	return TRUE;
}

static void circle_TTR_LL_proc (t_circle_TTR *ptrs__circle_TTR)
//-------------------------------------------------------------
{
  LINIA *ptrs_line1, *ptrs_line2 ;
  double df_xi, df_yi, df_t1i, df_t2i ;
  EVENT *ev;
  double df_x, df_y;


  ptrs_line1 = (LINIA*)ptrs__circle_TTR->ptr_ob1 ;
  ptrs_line2 = (LINIA*)ptrs__circle_TTR->ptr_ob2 ;

 if (0 == przeciecieLL_tt (&df_xi, &df_yi,
			(void *)ptrs_line1, (void *)ptrs_line2,
			&df_t1i, &df_t2i))
 {
   ErrList (29) ;
   return ;
 }


 redcr_circle(0);
 while (1)
 {
	 strwyj = 0;
	 ev = Get_Event_Point(NULL, &df_x, &df_y);
	 if (ev->What == evKeyDown && ev->Number == 0)
	 {
		 Cur_offd(X, Y);
		 redcr_circle(1);
		 CUR_ON(X, Y);
		 return;
	 }
	 if ((ev->What == evKeyDown && ev->Number == ENTER) || strwyj)
	 {
							
		 if (!strwyj)
		 {
			 Cur_offd(X, Y);
			out_krz(df_x, df_y);
			if (circle_ttr(df_x, df_y, TRUE))
            {
                OkragG.x = s__circle.df_xc;
                OkragG.y = s__circle.df_yc;
                OkragG.r = s__circle.df_rad;
            }
            else
            {
                //error
                redcr_circle(1);
                CUR_ON(X, Y);
                return;
            }
		 }
		 else
		 {
			 Cur_offd(X, Y);
			 out_krz(df_x, df_y);

			 s__circle_TTR.df_rad= Double_to_Float(Rstr);

			 if (circle_ttr(df_x, df_y, FALSE))
             {
                 OkragG.x = s__circle.df_xc;
                 OkragG.y = s__circle.df_yc;

                 OkragG.r = Double_to_Float(Rstr);
             }
             else
             {//error
                 redcr_circle(1);
                 CUR_ON(X, Y);
                 return;
             }
		 }


		 if (b_okrag == FALSE)
		 {
			 OkragG.obiekt = Okolo;
			 rysuj_obiekt((char *)&OkragG, COPY_PUT, 1);
		 }
		 else
		 {
			 out_circle(&OkragG, COPY_PUT, 0);
		 }
		 /*----------------------*/
		 if (OkragG.r) {
             rysuj_obiekt((char *)&OkragG, COPY_PUT, 1);
             if (dodaj_obiekt(NULL, &OkragG) == NULL);
         }

		 OkragG.obiekt = Ookrag;
		 /*----------------------*/

		 redcr_circle(1);
		 CUR_ON(X, Y);
		 return;
	 }
 }

}

static void circle_TTR_OO_proc (t_circle_TTR *ptrs__circle_TTR)  //ptrs__circle_TTR  for consistency, and maybe future extrensions
//------------------------------------------------------------
{
    //OKRAG *ptrs_circle1, *ptrs_circle2 ;
    //double df_xi, df_yi, df_t1i, df_t2i ;
    EVENT *ev;
    double df_x, df_y;
    
    //ptrs_circle1 = (OKRAG*)ptrs__circle_TTR->ptr_ob1 ;
    //ptrs_circle2 = (OKRAG*)ptrs__circle_TTR->ptr_ob2 ;

    redcr_circle(0);
    while (1)
    {
        strwyj = 0;
        ev = Get_Event_Point(NULL, &df_x, &df_y);
        if (ev->What == evKeyDown && ev->Number == 0)
        {
            Cur_offd(X, Y);
            redcr_circle(1);
            CUR_ON(X, Y);
            return;
        }
        if ((ev->What == evKeyDown && ev->Number == ENTER) || strwyj)
        {

            if (!strwyj)
            {
                Cur_offd(X, Y);
                out_krz(df_x, df_y);
                if (circle_ttr(df_x, df_y, TRUE))
                {
                    OkragG.x = s__circle.df_xc;
                    OkragG.y = s__circle.df_yc;
                    OkragG.r = s__circle.df_rad;
                }
                else
                {
                    //error
                    redcr_circle(1);
                    CUR_ON(X, Y);
                    return;
                }
            }
            else
            {
                Cur_offd(X, Y);
                out_krz(df_x, df_y);

                s__circle_TTR.df_rad= Double_to_Float(Rstr);

                if (circle_ttr(df_x, df_y, FALSE)) {
                    OkragG.x = s__circle.df_xc;
                    OkragG.y = s__circle.df_yc;
                    OkragG.r = Double_to_Float(Rstr);
                }
                else
                {//error
                    redcr_circle(1);
                    CUR_ON(X, Y);
                    return;
                }
            }

            if (b_okrag == FALSE)
            {
                OkragG.obiekt = Okolo;
                rysuj_obiekt((char *)&OkragG, COPY_PUT, 1);
            }
            else
            {
                out_circle(&OkragG, COPY_PUT, 0);
            }
            /*----------------------*/
            if (OkragG.r) {
                rysuj_obiekt((char *)&OkragG, COPY_PUT, 1);
                if (dodaj_obiekt(NULL, &OkragG) == NULL);
            }

            OkragG.obiekt = Ookrag;
            /*----------------------*/

            redcr_circle(1);
            CUR_ON(X, Y);
            return;
        }
    }

}


static void circle_TTR_OL_proc (t_circle_TTR *ptrs__circle_TTR)  //ptrs__circle_TTR  for consistency, and maybe future extrensions
//-----------------------------------------------------------------
{
    //OKRAG *ptrs_circle;
    //LINIA *ptrs_line;
    double df_xi, df_yi, df_t1i, df_t2i ;
    EVENT *ev;
    double df_x, df_y;

    //if (((NAGLOWEK*)(ptrs__circle_TTR->ptr_ob1))->obiekt==Olinia)
    //{
    //    ptrs_line = (LINIA*)ptrs__circle_TTR->ptr_ob1 ;
    //    ptrs_circle = (OKRAG*)ptrs__circle_TTR->ptr_ob2 ;
    //}
    //else
    //{
    //    ptrs_circle = (OKRAG *) ptrs__circle_TTR->ptr_ob1;
    //    ptrs_line = (LINIA *) ptrs__circle_TTR->ptr_ob2;
    //}

    redcr_circle(0);
    while (1)
    {
        strwyj = 0;
        ev = Get_Event_Point(NULL, &df_x, &df_y);
        if (ev->What == evKeyDown && ev->Number == 0)
        {
            Cur_offd(X, Y);
            redcr_circle(1);
            CUR_ON(X, Y);
            return;
        }
        if ((ev->What == evKeyDown && ev->Number == ENTER) || strwyj)
        {

            if (!strwyj)
            {
                Cur_offd(X, Y);
                out_krz(df_x, df_y);
                if (circle_ttr(df_x, df_y, TRUE))
                {
                    OkragG.x = s__circle.df_xc;
                    OkragG.y = s__circle.df_yc;
                    OkragG.r = s__circle.df_rad;
                }
                else
                {
                    //error
                    redcr_circle(1);
                    CUR_ON(X, Y);
                    return;
                }
            }
            else
            {
                Cur_offd(X, Y);
                out_krz(df_x, df_y);

                s__circle_TTR.df_rad= Double_to_Float(Rstr);

                if (circle_ttr(df_x, df_y, FALSE))
                {
                    OkragG.x = s__circle.df_xc;
                    OkragG.y = s__circle.df_yc;

                    OkragG.r = Double_to_Float(Rstr);
                }
                else
                {//error
                    redcr_circle(1);
                    CUR_ON(X, Y);
                    return;
                }
            }


            if (b_okrag == FALSE)
            {
                OkragG.obiekt = Okolo;
                rysuj_obiekt((char *)&OkragG, COPY_PUT, 1);
            }
            else
            {
                out_circle(&OkragG, COPY_PUT, 0);
            }
            /*----------------------*/
            if (OkragG.r) {
                rysuj_obiekt((char *)&OkragG, COPY_PUT, 1);
                if (dodaj_obiekt(NULL, &OkragG) == NULL);
            }

            OkragG.obiekt = Ookrag;
            /*----------------------*/

            redcr_circle(1);
            CUR_ON(X, Y);
            return;
        }
    }

}


static void circle_TTR_proc (t_circle_TTR *ptrs__circle_TTR)
//---------------------------------------------------------
{
  NAGLOWEK *ptrs_ob1, *ptrs_ob2 ;

  ptrs_ob1 = (NAGLOWEK*)ptrs__circle_TTR->ptr_ob1 ;
  ptrs_ob2 = (NAGLOWEK*)ptrs__circle_TTR->ptr_ob2 ;
  if (Olinia == ptrs_ob1->obiekt && Olinia == ptrs_ob2->obiekt)
  {
     circle_TTR_LL_proc (ptrs__circle_TTR) ;
  }
  else if ((Ookrag == ptrs_ob1->obiekt || Okolo == ptrs_ob1->obiekt || Oluk == ptrs_ob1->obiekt) &&
      (Ookrag == ptrs_ob2->obiekt || Okolo == ptrs_ob2->obiekt || Oluk == ptrs_ob2->obiekt))
  {
     circle_TTR_OO_proc (ptrs__circle_TTR) ;
  }
  else if (((Ookrag == ptrs_ob1->obiekt || Okolo == ptrs_ob1->obiekt || Oluk == ptrs_ob1->obiekt) &&
           (Olinia == ptrs_ob2->obiekt)) ||
           ((Olinia == ptrs_ob1->obiekt) &&
           (Ookrag == ptrs_ob2->obiekt || Okolo == ptrs_ob2->obiekt || Oluk == ptrs_ob2->obiekt)))
  {
      circle_TTR_OL_proc (ptrs__circle_TTR) ;
  }
}


static  void *check_if_sel_ob (unsigned u_ob_type, double df_x, double df_y)
/*-----------------------------------------------------*/
{
  void *ptr_object ;
  unsigned u_tmp ;

  df_x = df_x ;
  df_y = df_y ;
  u_tmp = u_ob_type ;
  if (NULL == (ptr_object = select_w(&u_tmp, NULL)))
  {
    return NULL ;
  }
  if (!u_ob_type & u_tmp)
  {
    ptr_object = NULL ;
  }
  return ptr_object ;
}


static void circle_TTR (double df_x, double df_y)
/*---------------------------------*/
{
  EVENT *ev;
  void *ptr_ob ;
  unsigned u_ob_type;

  u_ob_type = Blinia | Bokrag  | Okolo | Bluk ;
  if (NULL == (ptr_ob = check_if_sel_ob (u_ob_type, df_x, df_y)))
  {
aa:
     while (1)
     {
       ev = Get_Event_Point (NULL, &df_x, &df_y) ;
       if (ev->What == evKeyDown  && ev->Number== 0)
       {
	 return ;
       }
       if ( ev->What == evKeyDown  && ev->Number== ENTER &&
	  NULL != (ptr_ob = check_if_sel_ob (u_ob_type, df_x, df_y)))
       {
	 break;
       }
     }
  }
  out_krz (df_x, df_y) ;
  s__circle_TTR.ptr_ob1 = ptr_ob ;
  s__circle_TTR.df_x1 = df_x ;
  s__circle_TTR.df_y1 = df_y ;

  komunikat0_str (19, ptrsz__circle_type) ;
  while (1)
  {
    ev = Get_Event_Point(NULL, &df_x, &df_y) ;
    if (ev->What == evKeyDown && ev->Number == 0)
    {
      komunikat0_str (18, ptrsz__circle_type) ;
      goto aa ;
    }
    if (ev->What == evKeyDown  && ev->Number== ENTER &&
       NULL != (ptr_ob = check_if_sel_ob(u_ob_type, df_x, df_y)))
    {
      break;
    }
  }
  out_krz (df_x, df_y) ;
  s__circle_TTR.ptr_ob2 = ptr_ob ;
  s__circle_TTR.df_x2 = df_x ;
  s__circle_TTR.df_y2 = df_y ;

  s__circle_TTR.sector = 0;
  s__circle_TTR.df_rad = 10;

  komunikat0_str(24, ptrsz__circle_type);

  CUR_OFF(X, Y);

  sel.cur = 0;

  CUR_OFF = cursel_off;
  CUR_ON = cursel_on;

  CUR_ON(X, Y);


  while (1)
  {
	  ev = Get_Event_Point(NULL, &df_x, &df_y);
	  if (ev->What == evKeyDown && ev->Number == 0)
	  {
		  komunikat0_str(18, ptrsz__circle_type);
		  goto aa;
	  }
	  if (ev->What == evKeyDown && ev->Number == ENTER)
	  {
		  break;
	  }
  }

  df_x0=df_x;
  df_y0=df_y;

    line_g.x1=(float)df_x0;
    line_g.y1=(float)df_y0;

  CUR_OFF(X, Y);
  sel.cur = 1;
  CUR_OFF = out_sel_off;
  CUR_ON = out_sel_on;

  CUR_ON(X, Y);

  out_krz(df_x, df_y);
  s__circle_TTR.df_xr = df_x;
  s__circle_TTR.df_yr = df_y;

  s__circle_TTR.df_rad = df__rad_TTR ;
  circle_TTR_proc (&s__circle_TTR) ;
}

static void circle_3p(double df_x, double df_y)
/*---------------------------------------------*/
{
	double x0, y0, x, y, xe, ye, xs, ys, dl, si, co, xp, yp;

	s__circle.df_x3 = df_x;
	s__circle.df_y3 = df_y;
	x0 = (s__circle.df_x2 + s__circle.df_x1) / 2;
	y0 = (s__circle.df_y2 + s__circle.df_y1) / 2;
	xs = s__circle.df_x1 - x0;
	ys = s__circle.df_y1 - y0;
	dl = sqrt(xs * xs + ys * ys);
	si = ys / dl;
	co = xs / dl;
	obru(si, co, xs, ys, &xs, &ys);
	obru(si, co, df_x - x0, df_y - y0, &xe, &ye);
	if (TRUE == Check_if_Equal(ye, 0))
	{
		s__circle.df_xc = df_x;
		s__circle.df_yc = df_y;
		s__circle.df_rad = 0;

		return;
	}
	x = 0;
	y = (xe * xe + ye * ye - xs * xs) / (2 * ye);
	s__circle.df_rad = sqrt(xs * xs + y * y);
	obrd(si, co, x, y, &xp, &yp);
	s__circle.df_xc = xp + x0;
	s__circle.df_yc = yp + y0;
}

static BOOL circle_ttr(double df_x, double df_y, BOOL setrad)
/*---------------------------------------------------------*/
  {

	  double x0, y0, tx, ty, r;
      float tr;

      line_g.x1 = (float)df_x0;
      line_g.y1 = (float)df_y0;
      
      if ((((NAGLOWEK*)(s__circle_TTR.ptr_ob1))->obiekt==Olinia) &&
         (((NAGLOWEK*)(s__circle_TTR.ptr_ob2))->obiekt==Olinia)) 
      {
          if (circle_center_TTR_LL(&s__circle_TTR, &x0, &y0, &tx, &ty) == TRUE) {
              s__circle.df_xc = x0;
              s__circle.df_yc = y0;
              s__circle_TTR.df_xc = x0;
              s__circle_TTR.df_yc = y0;

              if (setrad) {
                  r = sqrt((df_x - s__circle_TTR.df_xr) * (df_x - s__circle_TTR.df_xr) + (df_y - s__circle_TTR.df_yr) * (df_y - s__circle_TTR.df_yr));
                  s__circle_TTR.df_rad = Double_to_Float(r);
              }

              s__circle.df_rad = s__circle_TTR.df_rad;

              line_g.x1 = (float)s__circle_TTR.df_xr;
              line_g.y1 = (float)s__circle_TTR.df_yr;
          }
      }
      else if (((NAGLOWEK*)(s__circle_TTR.ptr_ob1))->obiekt==Olinia) //first is line, second is circle, arc or disk
      {
          if (setrad) {
              //r = sqrt((df_x - s__circle_TTR.df_xr) * (df_x - s__circle_TTR.df_xr) + (df_y - s__circle_TTR.df_yr) * (df_y - s__circle_TTR.df_yr));
              r=sqrt((df_x-df_x0)*(df_x-df_x0)+(df_y-df_y0)*(df_y-df_y0));
              s__circle_TTR.df_rad = Double_to_Float(r);
          }

          if (circle_center_TTR_OL(/*&s__circle_TTR,*/ (OKRAG*)(s__circle_TTR.ptr_ob2), (LINIA*)(s__circle_TTR.ptr_ob1), &df_x0, &df_y0, s__circle_TTR.df_rad, &x0, &y0) == TRUE) {
              s__circle.df_xc = x0;
              s__circle.df_yc = y0;
              s__circle_TTR.df_xc = x0;
              s__circle_TTR.df_yc = y0;

              s__circle.df_rad = s__circle_TTR.df_rad;

              //line_g.x1 = (float)df_x0;
              //line_g.y1 = (float)df_y0;
          }
          else return FALSE;
          //line_g.x1 = (float)df_x0;
          //line_g.y1 = (float)df_y0;
      }
      else if (((NAGLOWEK*)(s__circle_TTR.ptr_ob2))->obiekt==Olinia) //second is line, first is circle, arc or disk
      {
          if (setrad) {
              //r = sqrt((df_x - s__circle_TTR.df_xr) * (df_x - s__circle_TTR.df_xr) + (df_y - s__circle_TTR.df_yr) * (df_y - s__circle_TTR.df_yr));
              r=sqrt((df_x-df_x0)*(df_x-df_x0)+(df_y-df_y0)*(df_y-df_y0));
              s__circle_TTR.df_rad = Double_to_Float(r);
          }

          if (circle_center_TTR_OL(/*&s__circle_TTR,*/ (OKRAG*)(s__circle_TTR.ptr_ob1), (LINIA*)(s__circle_TTR.ptr_ob2), &df_x0, &df_y0, s__circle_TTR.df_rad, &x0, &y0) == TRUE) {
              s__circle.df_xc = x0;
              s__circle.df_yc = y0;
              s__circle_TTR.df_xc = x0;
              s__circle_TTR.df_yc = y0;

              s__circle.df_rad = s__circle_TTR.df_rad;

              //line_g.x1 = (float)df_x0;
              //line_g.y1 = (float)df_y0;
          }
          else return FALSE;
          //line_g.x1 = (float)df_x0;
          //line_g.y1 = (float)df_y0;

      }
      else  //assumed they are circles, arcs or disks
      {
          if (setrad) {
              //r = sqrt((df_x - s__circle_TTR.df_xr) * (df_x - s__circle_TTR.df_xr) + (df_y - s__circle_TTR.df_yr) * (df_y - s__circle_TTR.df_yr));
              r=sqrt((df_x-df_x0)*(df_x-df_x0)+(df_y-df_y0)*(df_y-df_y0));
              s__circle_TTR.df_rad = Double_to_Float(r);
          }

          if (circle_center_TTR_OO(/*&s__circle_TTR,*/ (OKRAG*)(s__circle_TTR.ptr_ob1), (OKRAG*)(s__circle_TTR.ptr_ob2), &df_x0, &df_y0, s__circle_TTR.df_rad, &x0, &y0) == TRUE) {
              s__circle.df_xc = x0;
              s__circle.df_yc = y0;
              s__circle_TTR.df_xc = x0;
              s__circle_TTR.df_yc = y0;

              s__circle.df_rad = s__circle_TTR.df_rad;

              //line_g.x1 = (float)df_x0;
              //line_g.y1 = (float)df_y0;
          }
          else return FALSE;
          //line_g.x1 = (float)df_x0;
          //line_g.y1 = (float)df_y0;
      }

      //line_g.x1 = (float)df_x0;
      //line_g.y1 = (float)df_y0;
      return TRUE;
  }

  /*
static void circle_oottr(double df_x, double df_y, BOOL setrad)
//-------------------------------------------------------------
{

    double x0, y0, tx, ty, r;

    if (circle_center_OOTTR_LL(&s__circle_TTR, &x0, &y0, &tx, &ty) == TRUE)
    {
        s__circle.df_xc = x0;
        s__circle.df_yc = y0;
        s__circle_TTR.df_xc = x0;
        s__circle_TTR.df_yc = y0;

        if(setrad)
        {
            r = sqrt((df_x - s__circle_TTR.df_xr)*(df_x - s__circle_TTR.df_xr) + (df_y - s__circle_TTR.df_yr)*(df_y - s__circle_TTR.df_yr));
            s__circle_TTR.df_rad= Double_to_Float(r);

        }

        s__circle.df_rad = s__circle_TTR.df_rad;

        line_g.x1 = s__circle_TTR.df_xr;
        line_g.y1 = s__circle_TTR.df_yr;
    }
}
*/

static void out_circle_rad_param (BOOL b_draw)
//------------------------------------------
{
  if (b_draw == TRUE)
  {
    np = dodajstr (&eO) ;
    eO.st [0] = '\0' ;
    sprintf (eO.st, eO.format, milimetryob (df__rad_TTR)) ;
    Out_Edited_Draw_Param ((ESTR *)&eO, TRUE) ;
  }
  else
  if (np != -1)
  {
    usunstr (np) ;
  }
}




static void change_mode (int i_mode)
/*-------------------------------*/
{
  switch (i_mode)
  {
     case CIR_CEN_RAD:
     case CIR_CEN_DIA:
     case CIR_2POINT:
     case CIR_3POINT:
       if (s__circle.i_mode == CIR_TTR)
       {
		  CUR_OFF(X,Y);
		  sel.cur = 0 ;
		  CUR_OFF=cursel_off;
		  CUR_ON=cursel_on;
		  CUR_ON(X,Y);
       }
       break;
     case CIR_TTR:
       if (s__circle.i_mode != CIR_TTR)
       {
		  CUR_OFF (X, Y) ;
		  sel.cur = 1 ;
		  CUR_OFF = out_sel_off;
		  CUR_ON = out_sel_on;
		  CUR_ON (X, Y) ;
       }
       break ;
  }
  set_circle_mode (i_mode) ;
  out_circle_message0 (s__circle.i_mode) ;
}

static void set_circle_mode (int i_mode)
/*------------------------------------*/
{
  s__circle.i_mode = i_mode ;
}

static void out_circle_message0 (int i_mode)
/*-----------------------------------------*/
{
  switch (i_mode)
  {
     case CIR_CEN_RAD :
	komunikat0_str (10, ptrsz__circle_type) ;
	break ;
     case CIR_CEN_DIA :
	komunikat0_str (20, ptrsz__circle_type) ;
	break ;
     case CIR_2POINT :
	komunikat0_str (13, ptrsz__circle_type) ;
	break ;
     case CIR_3POINT :
	komunikat0_str (15, ptrsz__circle_type) ;
	break ;
     case CIR_TTR :
	komunikat0_str (18, ptrsz__circle_type) ;
	break ;
     default :
	break ;
  }
}


void OProm(OKRAG *O, double x,double y)
{ O->r=sqrt((O->x-x)*(O->x-x)+(O->y-y)*(O->y-y));}

void out_parametry_okregu (OKRAG *O)
/*---------------------------------------*/
{
  double l;

  if (O == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
  l = milimetryob (O->r);
  if (s__circle.i_mode == CIR_CEN_DIA)
  {
    l *= 2 ;
  }
  eO.st [0] = '\0' ;
  sprintf (eO.st, "%#12.9lg", l) ;
  Out_Edited_Draw_Param ((ESTR *)&eO, TRUE) ;
}

static void out_circle (OKRAG *O,int mode,int po)
/*----------------------------------*/
{

  if (O == NULL)
  {
    Internal_Error (__LINE__,__FILE__);
    return;
  }
  
  setwritemode(mode);
  if (mode == XOR_PUT)
  {
	  linestyle_xor(O->typ);
	  if (O->kolor == 0) SetColorAC(15); else SetColorAC(O->kolor);
  }
  else
  {
	  linestyle(O->typ);
	  SetColorAC(O->kolor);
  }
  okno_r();
  if (pikseleDX(O->r)< 0)
  {
	  O->r = 0;
  }
  set_pattern_count(TRUE);
  DrawCircle(pikseleX0(O->x),pikseleY0(O->y),pikseleDX(O->r), mode);
  set_pattern_count(FALSE);
  set_pattern_offset(0);
  okno_all();
  if (po) out_parametry_okregu(O);
}

static void  cur_off(double x,double y)
/*----------------------------------*/
{
  flip_screen();
}


static void  cur_on(double x,double y)
/*----------------------------------*/
{
    int i_pl ;
    float line_x2, line_y2;

    i_pl = 0 ;
    if (s__circle.i_mode == CIR_2POINT)
    {
        i_pl = 1 ;
    }
    cursor_on(x,y);
    if (s__circle.i_mode == CIR_CEN_RAD ||
        s__circle.i_mode == CIR_CEN_DIA ||
        s__circle.i_mode == CIR_2POINT  ||
        s__circle.i_mode == CIR_TTR)
    {
        line_g.x2 = x ;
        line_g.y2 = y ;
        outlineor (&line_g, COPY_PUT, i_pl) ;
        memcpy(&line_g0, &line_g, sizeof(line_g));
        if (orto)
        {
            line_x2=line_g.x2;
            line_y2=line_g.y2;
            orto_l(&line_g, &Orto_Dir);
            x = line_g.x2;
            y = line_g.y2;
            line_g.x2=line_x2;
            line_g.y2=line_y2;
        }
    }
    switch (s__circle.i_mode)
    {
        case CIR_CEN_RAD:
            OProm (&OkragG, x, y) ;
            mvcurb.mvc=0;
            out_circle (&OkragG,COPY_PUT,1);
            break ;
        case CIR_CEN_DIA:
            OProm (&OkragG, x, y) ;
            OkragG.r /= 2 ;
            mvcurb.mvc=0;
            out_circle (&OkragG,COPY_PUT,1);
            break;
        case CIR_2POINT:
            OkragG.x = (s__circle.df_x1 + x) / 2 ;
            OkragG.y = (s__circle.df_y1 + y) / 2 ;
            OProm(&OkragG,x,y);
            mvcurb.mvc=0;
            out_circle (&OkragG,COPY_PUT,0);
            break ;
        case CIR_3POINT:
            circle_3p (x, y) ;
            OkragG.x = s__circle.df_xc ;
            OkragG.y = s__circle.df_yc ;
            OkragG.r = s__circle.df_rad ;
            mvcurb.mvc=0;
            out_circle (&OkragG,COPY_PUT,0);
            break ;
        case CIR_TTR:
            if (circle_ttr(x, y, TRUE))  //_ttr
            {
                OkragG.x = s__circle.df_xc;
                OkragG.y = s__circle.df_yc;
                OkragG.r = s__circle.df_rad;
                mvcurb.mvc = 0;
                out_circle(&OkragG, COPY_PUT, 1);
            }
            break;
        default:
            break;
    }
}


#define r18 18
#define r22 16 //22



static int O_r(BOOL b_graph_value)
/*----------------------------------*/
{
  double r;

  b_graph_value = b_graph_value ;
  if (eO.val_no < 1)
  {
    return 0 ;
  }
  r = eO.values [0] ;
  if (r > 0)
  {
    Rstr = jednostkiOb (r) ;
    df__rad_TTR = Rstr ;
    strwyj = 1 ;
    return 1 ;
  }
  else return 0;
}

static int L_p (BOOL b_graph_value)
/*------------------------------*/
{
  PLINIA PL;
  double l,k;
  double angle_l;

  b_graph_value = b_graph_value ;
  if (eL.val_no < 1)
  {
    return 0 ;
  }
  l = eL.values [0] ;
  if ( orto != 0 || eL.val_no == 1)
  {
    parametry_linior (&line_g, &PL) ;
    k = PL.kat ;
  }
  else
  {
    angle_l=get_angle_l();
    if (angle_l!=0)
    {
     k = eL.values [1] + angle_l;
     if (k<0) k+=360;
    }
     else
       k = eL.values [1] ;
  }
  l = jednostkiOb(l);
  k = k * Pi / 180.0;
  df__dx = Double_to_Float (l * cos (k));
  df__dy = Double_to_Float (l * sin (k));
  strwyj=1;
  return 1;
}

static void redcr_circle (int typ)
/*----------------------------------*/
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  int i_kom_no ;
  if (typ==0)
  {
     switch (s__circle.i_mode)
     {
       case CIR_CEN_RAD:
	  i_kom_no = 11 ;
	  np=dodajstr(&eO);   //&eO
	  break ;
       case CIR_CEN_DIA:
	  i_kom_no = 12 ;
	  np=dodajstr(&eO);  //&eO
	  break ;
       case CIR_2POINT:
	  np=dodajstr(&eL);
	  i_kom_no = 14 ;
	  break ;
       case CIR_3POINT:
	  i_kom_no = 17 ;
	  break ;
	  case CIR_TTR:
	  i_kom_no = 25;
	  np = dodajstr(&eO);  //&eO
	  break;
       default :
	  break ;
     }
     komunikat0_str (i_kom_no, ptrsz__circle_type) ;
     CUR_OFF (X, Y) ;
     CUR_oFF=CUR_OFF;  CUR_OFF=cur_off;
     CUR_oN=CUR_ON;   CUR_ON=cur_on;
     Cur_ond (X, Y) ;
   }
  else
   { CUR_OFF=CUR_oFF;
     CUR_ON=CUR_oN;
     if (s__circle.i_mode != CIR_3POINT)
     {
       usunstr(np);
     }
     CUR_OFF(X, Y);
     CUR_ON(X,Y);
   }
}

static void circle_2point (double X0, double Y0)
/*---------------------------------------------*/
{
  EVENT *ev;
  LINIA L;

  s__circle.df_x1 = X0 ;
  s__circle.df_y1 = Y0 ;
  s__circle.df_x2 = X0 ;
  s__circle.df_y2 = Y0 ;
  OkragG.x=X0;
  OkragG.y=Y0;
  OkragG.r = 0 ;
  line_g.x1 = X0 ;
  line_g.y1 = Y0 ;
  line_g.x2 = X0 ;
  line_g.y2 = Y0 ;
  redcr_circle (0);
  while(1)
  {
    strwyj = 0 ;
    ev = Get_Event_Point (NULL, &X0, &Y0) ;
    if(ev->What == evKeyDown &&  ev->Number==0)
    {
      Cur_offd(X,Y);
      redcr_circle (1);
      CUR_ON(X,Y);
      return;
    }
    if((ev->What == evKeyDown &&  ev->Number== ENTER)  || strwyj)
    {
      Cur_offd (X, Y) ;
      if (!strwyj)
      {
		if (orto)
		{
		  L.x1=s__circle.df_x1;
		  L.y1=s__circle.df_y1;
		  L.x2=X0;
		  L.y2=Y0;
		  orto_l(&L, &Orto_Dir);
		  OkragG.x = (s__circle.df_x1 + L.x2) / 2 ;
		  OkragG.y = (s__circle.df_y1 + L.y2) / 2 ;
		  OProm(&OkragG,L.x2,L.y2); /*X0,Y0*/
		}
		else
		{
		 OkragG.x = (s__circle.df_x1 + X0) / 2 ;
		 OkragG.y = (s__circle.df_y1 + Y0) / 2 ;
		 OProm(&OkragG,X0,Y0);
		}
      }
      else
      {
		OkragG.x = s__circle.df_x1 + df__dx / 2 ;
		OkragG.y = s__circle.df_y1 + df__dy / 2 ;
		OProm (&OkragG, s__circle.df_x1, s__circle.df_y1);
       }
      if (b_okrag == FALSE)
      {
	 OkragG.obiekt = Okolo;
	rysuj_obiekt ((char *)&OkragG, COPY_PUT, 1) ;
      }
      else
      {
	out_circle (&OkragG,COPY_PUT,0);
      }
      /*----------------------*/
      if(OkragG.r) {
          rysuj_obiekt((char *)&OkragG, COPY_PUT, 1);
          if (dodaj_obiekt(NULL, &OkragG) == NULL);
      }

      OkragG.obiekt = Ookrag;
      /*----------------------*/

      redcr_circle (1);
      CUR_ON(X,Y);
      return;
    }
  }
}

static BOOL get_point (double *ptrdf_x, double *ptrdf_y)
/*-----------------------------------------------------*/
{
  EVENT *ev ;

  while (1)
  {
    ev = Get_Event_Point (NULL, ptrdf_x, ptrdf_y) ;
    if(ev->What == evKeyDown && ev->Number == 0)
    {
      return FALSE ;
    }
    if(ev->What == evKeyDown && ev->Number == ENTER)
    {
      return TRUE ;
    }
  }
}



static void circle_3point (double X0, double Y0)
/*---------------------------------------------*/
{
  EVENT *ev;
  double df_x, df_y ;


  s__circle.df_x1 = X0 ;
  s__circle.df_y1 = Y0 ;
  komunikat0_str (16, ptrsz__circle_type) ;
  out_krz (X0, Y0) ;
  if (FALSE == get_point (&s__circle.df_x2, &s__circle.df_y2))
  {
    return ;
  }
  out_krz (s__circle.df_x2, s__circle.df_y2) ;
  s__circle.df_x3 = s__circle.df_x2 ;
  s__circle.df_y3 = s__circle.df_y2 ;
  redcr_circle (0);
  while(1)
  {
    ev = Get_Event_Point (NULL, &df_x, &df_y) ;
    if(ev->What == evKeyDown &&  ev->Number==0)
    {
      Cur_offd(X,Y);
      redcr_circle (1);
      CUR_ON(X,Y);
      return;
    }
    if (ev->What == evKeyDown &&  ev->Number== ENTER)
    {
      Cur_offd (X, Y) ;
      out_krz (df_x, df_y) ;
      circle_3p (df_x, df_y) ;
      OkragG.x = s__circle.df_xc ;
      OkragG.y = s__circle.df_yc ;
      OkragG.r = s__circle.df_rad ;
      if (b_okrag == FALSE)
      {
	 OkragG.obiekt = Okolo;
	rysuj_obiekt ((char *)&OkragG, COPY_PUT, 1) ;
      }
      else
      {
	out_circle (&OkragG,COPY_PUT,0);
      }
      /*----------------------*/
      if(OkragG.r) {
          rysuj_obiekt((char *)&OkragG, COPY_PUT, 1);
          if (dodaj_obiekt(NULL, &OkragG) == NULL);
      }

      OkragG.obiekt = Ookrag;
      /*----------------------*/

      redcr_circle (1);
      CUR_ON(X,Y);
      return;
    }
 }
}


void circle_cen_rad_dia (double X0, double Y0)
/*----------------------------------*/
{
  EVENT *ev;
  LINIA L;

  OkragG.x=X0;
  OkragG.y=Y0;
  line_g.x1 = X0 ;
  line_g.y1 = Y0 ;
  line_g.x2 = X0 ;
  line_g.y2 = Y0 ;
  OProm(&OkragG,X,Y);
  if (s__circle.i_mode == CIR_CEN_DIA)
  {
    OkragG.r /= 2 ;
  }
  redcr_circle (0);
  while(1)
   { strwyj=0;
     ev=Get_Event_Point(NULL, &X0, &Y0);
     if(ev->What == evKeyDown &&  ev->Number==0)
      { Cur_offd(X,Y);
	redcr_circle (1);
	CUR_ON(X,Y);
	return;
       }
     if((ev->What == evKeyDown &&  ev->Number== ENTER)  || strwyj)
      {
	 Cur_offd(X,Y);
	if (!strwyj)
	 {
	   if (orto)
	   {
	    L.x1=OkragG.x;
	    L.y1=OkragG.y;
	    L.x2=X0;
	    L.y2=Y0;
	    orto_l(&L, &Orto_Dir);
	    OProm(&OkragG,L.x2,L.y2); /*X0,Y0*/
	   }
	   else OProm(&OkragG,X0,Y0);
	 }
	else
	{
	  OkragG.r = Double_to_Float (Rstr) ;
	 }
	 if (s__circle.i_mode == CIR_CEN_DIA)
	 {
	   OkragG.r /= 2 ;
	 }

	if (b_okrag == FALSE)
	{
	   OkragG.obiekt = Okolo;
	}
	else
	{
        OkragG.obiekt = Ookrag;
	}

	/*----------------------*/
	if(OkragG.r) {
        rysuj_obiekt((char *) &OkragG, COPY_PUT, 1);
        if (dodaj_obiekt(NULL, &OkragG) == NULL);
    }

	OkragG.obiekt = Ookrag;
	/*----------------------*/

	redcr_circle (1);
	return;
      }
   }
}


static void redcr0(char typ)
/*----------------------------------*/
{ static void (*CUR_oN)(double ,double);
  static void (*CUR_oFF)(double ,double);
  static int ( *SW[2])(),akt, sel_cur ;
  if (typ==0)
  {
    komunikat(0);
    mvcurb.O=1;
    eL.x = maxX - PL266 ;
    eL.y = ESTR_Y;
    eL.val_no_max = 2;
    eL.lmax = r22 ;
    eL.mode = GV_VECTOR ;
    eL.format = "%#12.9lg\0%#12.9lg;%#8.3lf" ;
    eL.ESTRF = L_p ;
	eL.extend = 0;
    eO.x=maxX-PL266 ;
    eO.y= ESTR_Y;
    eO.val_no_max = 1;
    eO.lmax=r22;
    eO.mode = GV_DIST ;
    eO.format = "%#12.9lg" ;
    eO.ESTRF=O_r;
	eO.extend = 0;
	CUR_OFF(X, Y);
    akt=sel.akt; sel.akt=ASel;
    cursor_off= cursel_off;
	cursor_on=cursel_on;
    CUR_oFF = CUR_OFF ;
    CUR_oN = CUR_ON ;
    sel_cur = sel.cur ;
    if (CIR_TTR == s__circle.i_mode)
    {
      sel.cur = 1 ;
      CUR_OFF = out_sel_off;
      CUR_ON = out_sel_on;
    }
    else
    {
      sel.cur = 0 ;
      CUR_OFF=cursel_off;
      CUR_ON=cursel_on;
    }
    SW[0]=SERV[73];  SERV[73]=sel_t;
    SW[1]=SERV[81];  SERV[81]=sel_d;
    CUR_ON(X,Y);
    out_circle_message0 (s__circle.i_mode) ;
  }
  else
  {
    CUR_OFF(X,Y);
    CUR_OFF=CUR_oFF;
    CUR_ON=CUR_oN;
    SERV[73]=SW[0];
    SERV[81]=SW[1];
    
    sel.akt=akt;
    sel.cur = sel_cur ;

    CUR_ON(X,Y);

    komunikat (0);
    komunikat0 (0);
    out_circle_rad_param (FALSE) ;
  }
}


void Okrag_Kolo (void)
/*---------------*/
{
  EVENT *ev ;
  double df_x, df_y ;

  redcr0 (0) ;
  while(1)
  {
    if (b_okrag == TRUE)
    {
      menupini (&mCircle, ptrsz__circle_type, _CIRCLE_C_, 14) ;
    }
    else
    {
      menupini (&mCircle, ptrsz__circle_type, _DISC_C_, 18) ;
    }
    view_line_type(&LiniaG);
    ev = Get_Event_Point (NULL, &df_x, &df_y) ;
    menupini (NULL, "", ' ', 0) ;
    switch (ev->What)
    {
	case evKeyDown :
	   if(ev->Number == 0)
	   {
	     redcr0 (1) ;
	     return ;
	   }
	   if(ev->Number == ENTER)
	   {
	     switch (s__circle.i_mode)
	     {
		case CIR_CEN_RAD:
		case CIR_CEN_DIA:
		   circle_cen_rad_dia (df_x, df_y) ;
		   break ;
		case CIR_2POINT:
		   circle_2point (df_x, df_y) ;
		   break ;
		case CIR_3POINT:
		   circle_3point (df_x, df_y) ;
		   break ;
		case CIR_TTR:
		   circle_TTR (df_x, df_y) ;
		   break ;
		default :
		   break ;
	     }
	     out_circle_message0 (s__circle.i_mode) ;
	   }
	   break ;
	case evCommandP :
	   change_mode (ev->Number) ;
	   break ;
	default :
	   break ;
    }
  }
}


void Okrag(void)
/*----------------*/
{
  b_okrag = TRUE;
  ptrsz__circle_type = CIRCLE ;
  Okrag_Kolo();
}

void Kolo(void)
/*----------------*/
{
  b_okrag = FALSE;
  ptrsz__circle_type = CIRCLE_FILL ;
  Okrag_Kolo();
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

#undef __O_OKRAG__


