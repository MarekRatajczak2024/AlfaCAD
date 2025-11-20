#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "forwin.h"
//#include "bib_e.h"

typedef int BOOL;

double compute_support_reaction(
        double H,
        double supp_x, double supp_y,
        double supp_sigx, double supp_sigy, double supp_sigxy,
        int    num_adj_supp,
        const double adj_supp_x[],
        const double adj_supp_y[],
        int    num_inside,
        const double h[],
        const double inside_x[],
        const double inside_y[],
        const double inside_sigx[],
        const double inside_sigy[],
        const double inside_sigxy[],
        double *Qn
);


static int getSignFloat(double num) {
    if (num == 0.0) {
        return 0;
    } else if (signbit(num)) {
        return -1;
    } else {
        return 1;
    }
}

/*=====================================================================
  compute_support_reaction
  --------------------------------------------------------------
  Input
      H            – plate thickness at the support node
      supp_x,y     – coordinates of the support node
      supp_sigx,y  – bending stresses σ_xx, σ_yy at the support node
                     (average of top & bottom fibres, as you already do)
      num_adj_supp – number of adjacent supported nodes (for tributary)
      adj_supp_x[] – their x-coordinates
      adj_supp_y[] – their y-coordinates
      num_inside   – number of interior neighbours
      h[]          – thickness of every interior neighbour
      inside_x[]   – x-coordinates of interior neighbours
      inside_y[]   – y-coordinates of interior neighbours
      inside_sigx[]– σ_xx of interior neighbours
      inside_sigy[]– σ_yy of interior neighbours
      inside_sigxy[]– σ_xy of interior neighbours   (NEW!)
  Output
      Q_vertical   – Qx  (shear force normal to a vertical edge)
      Q_horizontal – Qy  (shear force normal to a horizontal edge)
  Return
      Rz  = Qx·trib_v + Qy·trib_h   (upward positive, N)
  =====================================================================*/

double compute_support_reaction(
        double H,
        double supp_x, double supp_y,
        double supp_sigx, double supp_sigy, double supp_sigxy,
        int    num_adj_supp,
        const double adj_supp_x[],
        const double adj_supp_y[],
        int    num_inside,
        const double h[],
        const double inside_x[],
        const double inside_y[],
        const double inside_sigx[],
        const double inside_sigy[],
        const double inside_sigxy[],
        double *Qn)
{
    double Q_vertical;
    double Q_horizontal;
    const double TOL = 1e-9;
    const double TOLdxy = 1e-3;  // it maybe necessary sometimes

    /* ---- 1. moments at the support node -------------------------------- */
    double Mx_s  = supp_sigx  * (H*H/6.0);       // σ·(h²/6) = M
    double My_s  = supp_sigy  * (H*H/6.0);
    double Mxy_s = supp_sigxy * (H*H/6.0);

    /* ---- 2. tributary lengths ----------------------------- */
    double trib_v = 0.0, trib_h = 0.0;
    bool   has_v = false, has_h = false;

    for (int i = 0; i < num_adj_supp; ++i) {
        double dx = fabs(adj_supp_x[i] - supp_x);
        double dy = fabs(adj_supp_y[i] - supp_y);

        if (dy > TOL) {                         // vertical edge neighbour
            has_v = true;
            trib_v += fabs(adj_supp_y[i] - supp_y) / 2.0;
        }
        if (dx > TOL) {                  // horizontal edge neighbour
            has_h = true;
            trib_h += fabs(adj_supp_x[i] - supp_x) / 2.0;
        }
    }

    /* ---- 3. least-squares shear forces -------------------------------- */
    double Qx = 0.0, Qy = 0.0;

    /*  LS matrices for Qx and Qy  */
    double Axx = 0.0, Ax  = 0.0, Bx = 0.0;   // Qx :  Σw·dx²   Σw·dx   Σw·(ΔMxx + ΔMxy·dy/dx)
    double Ayy = 0.0, Ay  = 0.0, By = 0.0;   // Qy :  Σw·dy²   Σw·dy   Σw·(ΔMyy + ΔMxy·dx/dy)

    for (int i = 0; i < num_inside; ++i) {
        double dx = inside_x[i] - supp_x;
        double dy = inside_y[i] - supp_y;
        double r2 = dx*dx + dy*dy;  //r2 is a measure of contribution, bigger r2 - smaller influence of inside node moments on result
        if (r2 < TOL*TOL) continue;

        double Mx_i  = inside_sigx[i]  * (h[i]*h[i]/6.0);
        double My_i  = inside_sigy[i]  * (h[i]*h[i]/6.0);
        double Mxy_i = inside_sigxy[i] * (h[i]*h[i]/6.0);

        double dMx  = Mx_i  - Mx_s;
        double dMy  = My_i  - My_s;
        double dMxy = Mxy_i - Mxy_s;

        double w = 1.0 / r2;                     // inverse-distance weighting

        // ----- contribution to Qx = ∂Mx/∂x + ∂Mxy/∂y -----
        if (fabs(dx) > TOL) {
            double term = dMx - dMxy * dy/dx;   // (∂Mxy/∂y)≈ΔMxy/(dy)  (dy/dx is a helper)   (sign minus front of dMxy is based on experience)
            Axx += w * dx*dx;
            Ax  += w * fabs(dx); //this is probably not necessary, instead Axx is used to calculate Qx
            Bx  += w * term * fabs(dx);
        }

        // ----- contribution to Qy = ∂My/∂y + ∂Mxy/∂x -----
        if (fabs(dy) > TOL) {
            double term = dMy - dMxy * dx/dy;   // (∂Mxy/∂x)≈ΔMxy/(dx)  (dx/dy is a helper)   (sign minus front of dMxy is based on experience)
            Ayy += w * dy*dy;
            Ay  += w * fabs(dy); //this is probably not necessary, instead Ayy is used to calculate Qy
            By  += w * term * fabs(dy);
        }
    }

    if (Axx > TOL) Qx = Bx / Axx;      // = Σ(w·term·dx) / Σ(w·dx)
    if (Ayy > TOL) Qy = By / Ayy;

    Q_vertical   = (has_v ? Qx : 0.0);  // this is little bit questionable: if there is a single point supporting part of plate, isn't there any reaction ? in such case has_v will be zero
    Q_horizontal = (has_h ? Qy : 0.0);  // same notice as above

    ////*Qn = Q_vertical + Q_horizontal;  //Qn is shear force around the node in N/m or lbf / in
    /* ---- 4. nodal reaction ------------------------------------------- */
    double Rz = Q_vertical * trib_v + Q_horizontal * trib_h;
    *Qn = Rz / sqrt(trib_v*trib_v + trib_h*trib_h);  //Qn is shear force around the node in N/m or lbf / in
    return Rz;   //Rz is reaction in the node in N or lbf
}

// Structure to represent a point in 2D space
typedef struct {
    double x;
    double y;
} Point;

// Function to determine if a point is left, right, or on a line segment
int isLeft(Point a, Point b, Point c) {
    double cross_product = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);

    if (cross_product > 0) {
        return 1; // Left
    } else if (cross_product < 0) {
        return -1; // Right
    } else {
        return 0; // Collinear
    }
}


typedef struct
{
    double x;
    double y;
} POINTD;
///this is bad idea
double compute_NSEW_reaction(
        POINTD b, POINTD c,
        double kos, double koc,
        double H, BOOL is_simple,
        double supp_x, double supp_y,
        double supp_sigx, double supp_sigy,
        int    num_adj_supp,
        const double adj_supp_x[],
        const double adj_supp_y[],
        int    num_inside,
        int b_inside[],
        const double h[],
        const double inside_x[],
        const double inside_y[],
        const double inside_sigx[],
        const double inside_sigy[],
        double *Q_normal
)
{
    const double TOL = 1e-6;

    /* ---- 1. moments at the support node -------------------------------- */
    double Mx_s  = supp_sigx  * (H*H/6.0);       // σ·(h²/6) = M
    double My_s  = supp_sigy  * (H*H/6.0);


    double trib = 0.0;
    for (int i = 0; i < num_adj_supp; ++i) {
        double L = sqrt(pow(adj_supp_x[i]-supp_x,2) + pow(adj_supp_y[i]-supp_y,2));
        if (L > TOL) trib += L / 2.0;
    }

    /* ---- 3. least-squares shear forces -------------------------------- */
    double Qx = 0.0, Qy = 0.0;

    for (int i = 0; i < num_inside/2; ++i) //North, South
    {
        if (b_inside[i]==1) {
            double dy = fabs(inside_y[i] - supp_y);
            if (dy < TOL * TOL) continue;

            /* keep only points that lie on the *interior* side of the edge */
            double My_i = inside_sigy[i] * (h[i] * h[i] / 6.0);

            double dMy = My_i - My_s;
            double w = 1.0 / dy;

            /* ----- contribution to Qy = ∂My/∂y----- */
            if (fabs(dy) > TOL) {
                Qy += dMy / dy;
            }
        }
    }

    for (int i = num_inside/2; i < num_inside; ++i)
    {
        if (b_inside[i]==1) {
            double dx = fabs(inside_x[i] - supp_x);
            if (dx < TOL * TOL) continue;

            /* keep only points that lie on the *interior* side of the edge */

            double Mx_i = inside_sigx[i] * (h[i] * h[i] / 6.0);
            double dMx = Mx_i - Mx_s;

            /* ----- contribution to Qx = ∂Mx/∂x ----- */
            if (fabs(dx) > TOL) {
                Qx += dMx / dx;
            }
        }
    }

    if (Q_normal) *Q_normal = Qx + Qy;
    /* ---- 4. nodal reaction ------------------------------------------- */
    double Rz = (Qx + Qy) * trib;
    return Rz;
}


/*
 * 1. Run FEM → Get σ_xx, σ_yy at support nodes
2. Compute Rz_i = Q_n * tributary_i
3. Sum all Rz_i → Rz_total
4. Scale: Rz_final_i = Rz_i * (50000 / Rz_total)
 *
 * ### **Reaction Computation Module**
- **Input**: `σ_xx`, `σ_yy` at support and interior nodes (mid-plane)
- **Method**:
  - `M_n = M_xx n_x² + M_yy n_y²` (M_xy = 0 approximation)
  - `Q_n = dM_n/dn` via weighted least squares
  - Tributary length weighting
- **Boundary Conditions**:
  - Simple support: `M_n = 0`
  - Fixed support: `M_n` from FEM stress
- **Scaling**: All reactions scaled to total load = 50,000 N
- **Limitations**:
  - Rotated fixed supports: `M_xy` neglected → approximate
  - Simple supports: highly accurate
  - Horizontal fixed: good accuracy
 */