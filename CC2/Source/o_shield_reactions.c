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

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "forwin.h"
#include "o_shield_reactions.h"

// Returns: nodal reaction force (Rx, Ry) in global coordinates
// Also fills *Rn (normal reaction per unit length) and *Rt (tangential)

void compute_membrane_support_reaction(
        double t,                                      // thickness
        double supp_sigx, double supp_sigy, double supp_sigxy,
        double cos_theta, double sin_theta,            // direction of edge: tangent = (cosθ, sinθ)
        double trib_length,                            // precomputed: sum of half-segments
        SupportType support_type,
        double *Rx, double *Ry,                        // global nodal reactions
        double *Rn, double *Rt)                             // normal & tangential per unit length (optional)
{
    const double TOL = 1e-12;

    *Rx = *Ry = *Rn = *Rt = 0.0;

    if (trib_length < TOL) return;

    // === 1. Edge tangent vector (you provide it — perfect!) ===
    double tx = cos_theta;
    double ty = sin_theta;

    // Normalize just in case (defensive)
    double tlen = sqrt(tx*tx + ty*ty);
    if (tlen > TOL) { tx /= tlen; ty /= tlen; }

    // === 2 Inward normal: rotate tangent +90° counterclockwise ===
    // (standard convention: left side when walking along tangent = inside)
    double nx = -ty;    // ← this is the correct, consistent choice
    double ny =  tx;

    // === 3. Cauchy stress vector on the cut (normal points INTO domain) ===
    double traction_x = supp_sigx  * nx + supp_sigxy * ny;
    double traction_y = supp_sigxy * nx + supp_sigy  * ny;

    // Reaction force per unit length = -traction (action-reaction)
    double Rx_per_m = -traction_x * t;
    double Ry_per_m = -traction_y * t;

    // Normal and tangential reactions (positive = support pushes INTO domain)
    double normal_per_m    = Rx_per_m * nx + Ry_per_m * ny;
    double tangential_per_m = Rx_per_m * tx + Ry_per_m * ty;

    // === 4. Apply support type (roller logic) ===
    bool react_normal     = true;
    bool react_tangential = true;

    if (support_type == SUPPORT_ROLLER_X) {
        // Roller allows movement in global X → no reaction in X → only normal component remains
        // But easier: zero tangential if edge is nearly horizontal
        if (fabs(tx) > 0.866)  // |cosθ| > √3/2 → edge within ±30° of X-axis
            react_tangential = false;
    }
    else if (support_type == SUPPORT_ROLLER_Y) {
        if (fabs(ty) > 0.866)  // nearly vertical edge
            react_tangential = false;
    }

    double final_normal_per_m = react_normal     ? normal_per_m    : 0.0;
    double final_tang_per_m   = react_tangential ? tangential_per_m : 0.0;

    // === 5. Output per-unit-length values (optional) ===
    if (Rn) *Rn = final_normal_per_m;
    if (Rt) *Rt = final_tang_per_m;

    // === 6. Nodal reactions in global coordinates ===
    double Fx = final_normal_per_m * nx + final_tang_per_m * tx;
    double Fy = final_normal_per_m * ny + final_tang_per_m * ty;

    *Rx = Fx * trib_length;
    *Ry = Fy * trib_length;
}




void compute_membrane_support_reaction_(
        double t,                                   // thickness
        double supp_x, double supp_y,
        double supp_sigx, double supp_sigy, double supp_sigxy,
        int num_adj_supp,
        const double adj_supp_x[], const double adj_supp_y[],
        int num_inside,
        const double inside_x[], const double inside_y[],
        SupportType support_type,
        double *Rx, double *Ry,                     // output: global reactions at node
        double *Rn, double *Rt)                     // output: normal & tangential per unit length
{

    supp_sigxy=0.0;

    const double TOL = 1e-9;

    *Rx = 0.0;
    *Ry = 0.0;
    *Rn = 0.0;
    *Rt = 0.0;

    if (num_adj_supp < 1) return;  // isolated node → no tributary

    // === 1. Determine edge direction from adjacent support nodes ===
    // Take average direction of connected support segments
    double nx = 0.0, ny = 0.0;  // inward normal (will be normalized)
    double tx = 0.0, ty = 0.0;  // tangent vector (along edge)

    for (int i = 0; i < num_adj_supp; ++i) {
        double dx = adj_supp_x[i] - supp_x;
        double dy = adj_supp_y[i] - supp_y;
        double len = sqrt(dx*dx + dy*dy);
        if (len < TOL) continue;

        // Tangent vector (along edge)
        tx += dx / len;
        ty += dy / len;
    }

    if (num_adj_supp > 0) {
        double tlen = sqrt(tx*tx + ty*ty);
        if (tlen > TOL) {
            tx /= tlen;
            ty /= tlen;
        } else {
            tx = 1.0; ty = 0.0;  // fallback
        }
    } else {
        tx = 1.0; ty = 0.0;
    }

    // Inward normal = rotated tangent 90° clockwise (or counterclockwise — consistent)
    // We choose: n = (ty, -tx) → points "inside" assuming CCW boundary
    nx =  ty;
    ny = -tx;

    // Normalize normal
    double nlen = sqrt(nx*nx + ny*ny);
    if (nlen > TOL) {
        nx /= nlen;
        ny /= nlen;
    } else {
        nx = 0.0; ny = 1.0;  // fallback
    }

    // === 2. Cauchy stress vector on the supported edge ===
    // Traction vector t = σ · n  (inward normal)
    double tx_traction = supp_sigx * nx + supp_sigxy * ny;
    double ty_traction = supp_sigxy * nx + supp_sigy * ny;

    // Normal and tangential components (per unit length, per unit thickness)
    double normal_force_per_length_per_t = tx_traction * nx + ty_traction * ny;  // N · n
    double tang_force_per_length_per_t  = tx_traction * tx + ty_traction * ty;  // N · t

    // Total force per unit length
    double N_per_m = normal_force_per_length_per_t * t;   // [N/m]
    double T_per_m = tang_force_per_length_per_t * t;     // [N/m]

    // === 3. Apply support type constraints ===
    bool block_normal     = true;
    bool block_tangential = true;

    if (support_type == SUPPORT_ROLLER_X || support_type == SUPPORT_ROLLER_Y) {
        // Determine if roller allows movement along tangent or normal
        double cos_angle = fabs(tx * 1.0 + ty * 0.0);  // dot with global X
        if (support_type == SUPPORT_ROLLER_X && cos_angle > 0.7) {  // roughly horizontal edge
            block_tangential = false;   // free in X → no tangential reaction
        } else if (support_type == SUPPORT_ROLLER_Y && cos_angle < 0.3) {  // roughly vertical
            block_tangential = false;
        } else {
            // fallback: assume normal is blocked, tangential free
            block_tangential = false;
        }
    }

    // Final reactions per unit length
    *Rn = block_normal     ? N_per_m : 0.0;
    *Rt = block_tangential ? T_per_m : 0.0;

    // === 4. Tributary length (same as plate) ===
    double trib_len = 0.0;
    for (int i = 0; i < num_adj_supp; ++i) {
        double dx = adj_supp_x[i] - supp_x;
        double dy = adj_supp_y[i] - supp_y;
        double len = sqrt(dx*dx + dy*dy);
        if (len > TOL) trib_len += len / 2.0;
    }

    // === 5. Nodal reactions in global coordinates ===
    double Fx = (*Rn) * nx + (*Rt) * tx;   // normal + tangential
    double Fy = (*Rn) * ny + (*Rt) * ty;

    *Rx = Fx * trib_len;
    *Ry = Fy * trib_len;
}

void compute_membrane_support_reaction__(
        double t,
        double supp_x, double supp_y,
        double supp_sigx, double supp_sigy, double supp_sigxy,
        int num_adj_supp,
        double adj_supp_x[], const double adj_supp_y[],
        int num_inside,
        const double inside_x[], const double inside_y[],
        SupportType support_type,
        double *Rx, double *Ry,
        double *Rn, double *Rt)
{
    const double TOL = 1e-9;
    *Rx = *Ry = *Rn = *Rt = 0.0;

    if (num_adj_supp < 1 || num_inside < 1) return;

    // === 1. Compute average tangent from adjacent support nodes ===
    double tx = 0.0, ty = 0.0;
    int tcount = 0;
    for (int i = 0; i < num_adj_supp; ++i) {
        double dx = adj_supp_x[i] - supp_x;
        double dy = adj_supp_y[i] - supp_y;
        double len = sqrt(dx*dx + dy*dy);
        if (len < TOL) continue;
        tx += dx / len;
        ty += dy / len;
        tcount++;
    }
    if (tcount == 0) { tx = 1.0; ty = 0.0; }
    else {
        double len = sqrt(tx*tx + ty*ty);
        if (len > TOL) { tx /= len; ty /= len; }
    }

    // === 2. Two possible normals: +90° and -90° from tangent ===
    double nx1 =  ty;   double ny1 = -tx;   // +90° (CCW)
    double nx2 = -ty;   double ny2 =  tx;   // -90° (CW)

    // === 3. Choose the normal that points TOWARD the inside nodes ===
    double score1 = 0.0, score2 = 0.0;
    int votes = 0;

    for (int i = 0; i < num_inside; ++i) {
        double dx = inside_x[i] - supp_x;
        double dy = inside_y[i] - supp_y;
        if (dx*dx + dy*dy < TOL*TOL) continue;

        double dot1 = dx*nx1 + dy*ny1;
        double dot2 = dx*nx2 + dy*ny2;

        if (dot1 > 0) score1 += 1.0;
        else if (dot1 < 0) score1 -= 1.0;

        if (dot2 > 0) score2 += 1.0;
        else if (dot2 < 0) score2 -= 1.0;

        votes++;
    }

    double nx, ny;
    if (votes > 0 && fabs(score1) >= fabs(score2)) {
        nx = nx1; ny = ny1;
        if (score1 < 0) { nx = -nx1; ny = -ny1; }  // flip if needed
    } else {
        nx = nx2; ny = ny2;
        if (score2 < 0) { nx = -nx2; ny = -ny2; }
    }

    // === 4. Cauchy traction on the CUT FACE (normal = inward to domain) ===
    double traction_x = supp_sigx  * nx + supp_sigxy * ny;
    double traction_y = supp_sigxy * nx + supp_sigy  * ny;

    // Reaction = -traction (because support pushes back)
    double Rx_per_m = -traction_x * t;
    double Ry_per_m = -traction_y * t;

    // Normal and tangential reactions (positive = support pushes into domain)
    double normal    = Rx_per_m * nx + Ry_per_m * ny;  // N · n
    double tangential = Rx_per_m * tx + Ry_per_m * ty;  // N · t

    *Rn = normal;
    *Rt = tangential;

    // === 5. Tributary length ===
    double trib = 0.0;
    for (int i = 0; i < num_adj_supp; ++i) {
        double len = sqrt(pow(adj_supp_x[i]-supp_x,2) + pow(adj_supp_y[i]-supp_y,2));
        if (len > TOL) trib += len / 2.0;
    }

    // === 6. Final nodal reactions ===
    *Rx = Rx_per_m * trib;
    *Ry = Ry_per_m * trib;
}
