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

#define __O_SHIELD__
#include <stdlib.h>
#ifdef LINUX
#include <dirent.h>
#else
#include <direct.h>
#endif
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <forwin.h>
#ifdef LINUX
#include <unistd.h>
#include <sys/stat.h>
#endif

#include <allegext.h>

#pragma pack(4)  //to avoid warning //WARNING !!!!

#include "bib_e.h"
#include "o_plate.h"
#include "o_shield.h"
#include "o_shield_reactions.h"
#include "o_static.h"
#include "o_bloklb.h"
#include "bib_blok.h"
#include "o_object.h"
#include "rysuj_e.h"
#include "message.h"
#include "o_loadf.h"
#include "o_hatch.h"

#include "leak_detector_c.h"
#define COPY_PUT 0

#ifdef LINUX
#ifndef _STATIC_
#define _STATIC_ "Static/"
#endif
#ifndef _ELMER_
#define _ELMER_ "Elmer/"
#endif
#ifndef _BIN_
#define _BIN_ "bin/"
#endif
#ifndef _shield_
#define _shield_ "shield/"
#endif
#else
#ifndef _STATIC_
#define _STATIC_ "Static\\"
#endif
#ifndef _ELMER_
#define _ELMER_ "Elmer\\"
#endif
#ifndef _BIN_
#define _BIN_ "bin\\"
#endif
#ifndef _shield_
#define _shield_ "shield\\"
#endif
#endif

#define BIGNOD 1e200
#define BIGNOF 1e20

#define M100 100

typedef unsigned long DWORD;
extern DWORD RunSilent(char* strFunct, char* strstrParams);
extern DWORD SystemSilent(char* strFunct, char* strstrParams);
extern void set_cursor_pointer(void);
extern unsigned int getRGB(double m, double max_m);
extern void line_place (LINIA *l, double *xmin, double *ymin, double *xmax, double *ymax);
extern void luk_place (LUK *l, double *xmin, double *ymin, double *xmax, double *ymax);

extern void delete_all_from_layer_atrybut (int layer_no, int atrybut);
extern void normalize_txt(TEXT *ptrs_text);
extern double Get_Point_Size (void);
extern void lock_mouse(void);

extern int my_file_exists(char *name);
extern int my_directory_exists(const char *path);
extern int ask_question(int n_buttons, char* esc_string, char* ok_string, char* cont_string, char* comment_string, int color_comment, char* comment1_string, int color1_comment, int cien, int image);

extern void set_decimal_format(char *text, double l, double precision);

extern BOOL  hatch_proc_test (long_long l_offb, long_long l_offe, double df_pointx, double df_pointy, T_PTR_Hatch_Param ptrs_hatch_param, int comput_area, double df_apx1, double df_apy1, double df_apx2, double df_apy2, BOOL shadow);

extern BOOL Check_if_Equal (double x, double y);
extern BOOL Check_if_Equal2(double x, double y);
extern BOOL Check_if_Equal3(double x, double y);

extern BOOL Check_Attribute (int i_attribute, int i_check);
extern unsigned char str2load(char *ptr);

extern void get_blocks_setup(int layer_no, char **block_names, int *block_names_no, int max_no, int max_len);

extern int ask_question_static(int n_buttons, char* esc_string, char* ok_string, char* cont_string, char* comment_string, int color_comment, char* comment1_string, int color1_comment, int cien, int image, int *combinantion, int *geometri_stiffness, int *inertia, int *st_dynamic_no, BOOL *PINNABLE,
                               int *theta, int *sigma_eq, int *epsilon);
extern int EditText(char *mytext, int edit_params, int nCmdShow, int *single, int *tab);
extern int EditFile(char *, int adjust, int nCmdShow);
extern double Angle_Normal (double angle);
extern double Angle_Simple (double angle);
extern void utf8Upper(char* text);
extern int isInside(POINTF *A, POINTF *B, POINTF *C, POINTF *P, double *alpha, double *beta, double *gamma);
extern void Rotate_Point(double si, double co, double x1, double y1, /*center point*/ double x2, double y2, double *x, double *y); /*rotate point*/

extern BOOL check_if_point_on_line_segment(double p1x, double p1y, double p2x, double p2y, double px, double py);
extern BOOL check_if_point_on_arc_segment(double xc, double yc, double r, double startAngle, double endAngle, double x, double y);

extern TMENU mVector;
extern BOOL glb_silent;
extern double Atan2 (double y, double x);

#ifdef LINUX
extern int run_with_timeout(char* command, int timeout);
#else
extern DWORD run_with_timeout(char* command, int timeout);
#endif

static char T_text[64];

#define TIMEOUT_SECONDS 15

static int draw_label(LINIA *L, LINIA *Le, double dx, double r1, double r2, double vpar, double precision, int bold, char *suffix, char *prefix)
{
    TEXT T=Tdef;
    double x, y;
    PLINIA PL, PL1;
    char *ptr_t;
    double x1, y1, x2, y2;
    double kos, koc, dlx;
    double lb, le;
    char *ptr;

    parametry_lini(L, &PL);
    parametry_lini(Le, &PL1);
    dlx=jednostkiOb(dx);

    lb=jednostkiOb(r1);
    le=PL1.dl-jednostkiOb(r2);

    strcpy(T.text, prefix);
    ptr=T.text+strlen(prefix);

    //set_decimal_format(T.text, vpar, precision);
    set_decimal_format(ptr, vpar, precision);

    if ((strcmp(T.text,"0")==0) || (strcmp(T.text,"-0")==0)) return 0;

    strcat(T.text, suffix);

    if (strcmp(T.text, T_text)!=0)
    {
        T.x = (L->x1 + L->x2) / 2;
        T.y = (L->y1 + L->y2) / 2;

        T.justowanie = j_srodkowo;
        T.bold = bold;
        T.warstwa = L->warstwa;
        T.kolor = L->kolor;
        T.czcionka = zmwym.czcionka;
        T.wysokosc = zmwym.wysokosc * 0.5;

        if (Check_if_Equal(L->y1,L->y2))
        {
            if ((Check_if_Equal2(dlx, PL1.dl) || Check_if_Equal2(dlx, le)) && (L->x1 > L->x2))
                T.kat = (float)(Pi_ * (PL.kat + 180) / 180);
            else if ((Check_if_Equal2(dlx, 0.0) || Check_if_Equal2(dlx, lb)) && (L->x2 > L->x1))
                T.kat = (float)(Pi_ * (PL.kat + 180) / 180);
            else
                T.kat = (float)(Pi_ * PL.kat / 180);
        }
        else {
            if ((Check_if_Equal2(dlx, PL1.dl) || Check_if_Equal2(dlx, le)) && (L->y1 > L->y2))
                T.kat = (float)(Pi_ * (PL.kat + 180) / 180);
            else if ((Check_if_Equal2(dlx, 0.0) || Check_if_Equal2(dlx, lb)) && (L->y2 > L->y1))
                T.kat = (float)(Pi_ * (PL.kat + 180) / 180);
            else
                T.kat = (float)(Pi_ * PL.kat / 180);
        }
        T.blok=1;
        //set_decimal_format(T.text, vpar, precision);
        T.dl = strlen(T.text);
        T.n = T18 + T.dl;
        normalize_txt(&T);
        ptr_t = dodaj_obiekt((BLOK *) dane, &T);
        strcpy(T_text, T.text);

        return 1;
    }
    return 0;
}


extern double compute_support_reaction(
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

void compute_membrane_support_reaction(
        double t,                                   // thickness
        //double supp_x, double supp_y,
        double supp_sigx, double supp_sigy, double supp_sigxy,
        //int num_adj_supp,
        //const double adj_supp_x[], const double adj_supp_y[],
        //int num_inside,
        //const double inside_x[], const double inside_y[],
        double cos_theta, double sin_theta,
        double trib_length,
        SupportType support_type,
        double *Rx, double *Ry,                     // output: global reactions at node
        double *Rn, double *Rt                      // output: normal & tangential per unit length;
);

#ifndef LINUX
extern BOOL Is64BitOperatingSystem(void);
#endif

extern int mynCmdShow;

extern ST_LOAD_FACTORS st_load_factors_EU_0[];
extern ST_LOAD_FACTORS st_load_factors_ASCE_0[];
extern ST_LOAD_FACTORS st_load_factors_ICC_0[];

extern ST_LOAD_FACTORS *load_factors;

extern int st_load_factors_no;
//extern char *VALUE_NAME;
//extern char *VALUE_NAME1;
//ST_LOAD_FACTORS *st_load_factors;
//extern ST_LOAD_FACTORS *load_factors;

UNIT_FACTORS *unit_factors_sh;
/*
typedef struct {              //                                                      SI                            IMP
    double h_f; //depth of cross section in y axis                                   m //mm                         in
    double hm_f; //depth of cross section in y axis                                  m->m //mm->m                   in
    double A_f; //area in mm^2 of cross section                                      cm^2 -> m^2 //cm^2 -> mm^2     in^2
    double Am_f; //area in m^2 of cross section                                      m^2 -> m^2 //mm^2 -> m^2                    in^2
    double I_f; //aread moment of inertia                                            cm^4 -> m^4 //cm^4 -> mm^4                   in^4
    double Wm_f; //elastic section modulus in m^3                                    cm^3 -> m^3 //cm^3 -> m^3         in^3
    double Wmm_f; //elastic section modulus in m^3                                   cm^3 -> m^3 //cm^3 -> m^3         in^3  //not in use for shields
    double E_f; //Young's Modulus of elasticity and Shear modulus of elasticity      GPa -> Pa //GPa -> MPa          ksi
    double d_f; //mass density                                                       kg/m^3 -> kg/m^3 //kg/m^3 -> t/m^3     kip/in^3/g
    double a_f; //Coefficient of thermal expansion                                   /C                  /F
    double g_f; //gravitational acceleration                                         m/s^2 -> m/s^2 //m/s^2 -> mm/s^2     in/s^2
    double F_f; //forces                                                             kN -> N             kip
    double M_f; //moments                                                            kNm -> Nm //kNm -> Nmm          kip*in
    double q_f; //load                                                               kN/m -> N/m //kN/m -> N/mm        kip/in
    double R_f; //Reactions  (out)                                                   N -> kN   //t -> kN             kip
    double S_f; //Stress  (out)                                                      Pa -> Mpa  //kPa -> Mpa          kip
    double Ma_f;//Mass                                                               kPa -> tone         kip
    double c_f; //concrete cover  should be no less than 35 mm / 1.5" in for beams and 55 mm / 2"  for columns  //actually not in use
} UNIT_FACTORS;
*/
///UNIT_FACTORS unit_factors_si={1.0, 0.001, 100.0,0.000001, 10000.0,0.000001, 0.000000001,1000.0,1e-12,1.0,1000.0,1000.0,1000000,1.0, 1000.0, 1000.0, 9.81, 0.001};

UNIT_FACTORS unit_factors_sh_si={0.001, 1.0, 0.0001, 1.0, 0.00000001, 0.000001, 0.000000001, 1.0e9,  1.0, 1.0, 1.0, 1000.0, 1000, 1000.0, 1000.0, .000001, 9.81, 0.001};
UNIT_FACTORS unit_factors_sh_imp={1.0, 1.0, 1.0,1.0,1.0, 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0, 1.0, 1000.0, 1.0, 1.0, 1.0};

extern PROP_PRECISIONS SI_precisions;
extern PROP_PRECISIONS IMP_precisions;
PROP_PRECISIONS *prop_precisions_sh=&SI_precisions;  //just to initialize

extern double dim_precision;
extern double t_precision;
extern double r_precision;
extern double rm_precision;

extern double thermal_precision;
extern double force_precision;
extern double moment_precision;
extern double stress_precision;
extern double displacement_precision;
extern double rotation_precision;
extern double load_precision;

extern double force_magnitude;
extern double moment_magnitude;
extern double displacement_magnitude;
extern double load_magnitude;
extern double flood_magnitude;
extern double shear_magnitude;

double dim_precision_sh=0.0001;

extern char *UNITS;
extern char *SI;
extern char *IMP;

static char *unit;
static char *_mm_="[mm]";
static char *_inch_="[\"]";
static char *_rad_="[rad]";
static char *_MPa_="[MPa]";
static char *_ksi_="[ksi]";

#define DEFLECTION_NUMBER 6
#define STRESS_NUMBER 7
#define EPSILON_NUMBER 6

static T_Hatch_Param 		    s_hatch_param = {1, 0, 1, 0, 0, 0} ;

static ST_PROPERTY *sh_property=NULL;
int sh_property_no=0;

static ST_PROPERTY prt_def_sh={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.85,0,0};

static ST_LOAD_FACTORS *sh_load_factors=NULL;
int sh_load_factors_no=0;

static SH_NODE *sh_node=NULL;
static SH_NODE_EMB *sh_node_emb=NULL;
static SH_NODE_EMB_EDGE *sh_node_emb_edge=NULL;
static SH_EDGE *sh_edge=NULL;
static SH_EDGE *sh_edge_emb=NULL;
static SH_LOAD *sh_load=NULL;
//static SH_LOAD *sh_point_load=NULL;
static GEO_LINE *geo_line=NULL;
static LINIA Ldsp = Ldef;
static LINIA Ldspm = Ldef;
static LINIA Ldsp1 = Ldef;

static LINIA Ldspt = Ldef;
static LINIA Ldspmt = Ldef;
static LINIA Ldsp1t = Ldef;

#define MAX_L_BLOCKS 64 //32 //16
#define MAX_L_BLOCKS_LEN 64

static char **block_names=NULL;

static double sh_min_x, sh_min_y, sh_max_x, sh_max_y;
extern int theta_, sigma_eq_, epsilon_;

static MESH_NODE *mesh_node=NULL;
static MESH_ELEMENT *mesh_element=NULL;
static MESH_BOUNDARY *mesh_boundary=NULL;

static SHIELD_PROPERTY *shield_property=NULL;
static SHIELD_PROPERTY *hole_property=NULL;
static SHIELD_PROPERTY *wall_property=NULL;
static SHIELD_PROPERTY *zone_property=NULL;

static int sh_node_no=0;
static int sh_node_emb_no=0;
static int sh_node_emb_edge_no=0;
static int sh_edge_no=0;
static int sh_edge_emb_no=0;
static int shield_no=0;
static int hole_no=0;
static int wall_no=0;
static int zone_no=0;
static int sh_load_no=0;  //uniforme
//static int sh_point_load_no=0;
static int mesh_node_no=0;
static int mesh_element_no=0;
static int mesh_boundary_no=0;
static int geo_line_no=0;

//static int pllc_node_force_moment_no=0;
static int pllc_sh_load_no=0;

#define MAX_SHIELD_NO 100

int SH_PROPERTY_MAX=10;
int SH_LOAD_FACTORS_MAX=100;
int SH_NODE_MAX=100;
int SH_NODE_EMB_MAX=10;
int SH_NODE_EMB_EDGE_MAX=10;
int SH_EDGE_MAX=100;
int SH_EDGE_EMB_MAX=10;
int SH_LOAD_MAX=10;
int SH_POINT_LOAD_MAX=10;
int SH_SHIELD_MAX=10;
int SH_HOLE_MAX=10;
int SH_WALL_MAX=10;
int SH_ZONE_MAX=10;
static int GEO_LINE_MAX=1000;

static float dxl=0.1f;   //this could be dynamic from 0.1f fo 1.0f
static float dxr=0.25f;   //this could be dynamic from 0.25 to 2.5;

static float dxl_min=0.1f;  //this could be dynamic from 0.1f fo 1.0f

extern int combi_uls_no;
extern int combi_sls_no;
extern int combi_qpsls_no;

extern int combi_total_numbers_no;
extern COMBI_TOTAL_NUMBER combi_total_numbers[255*3];

extern COMBINATION *ULSLC;
extern COMBINATION *SLSLC;
extern COMBINATION *QPSLSLC;
extern int *MC_ULSLC;
extern int *MC_SLSLC;
extern int *MC_QPSLSLC;

static COMBINATION *ULS_SLS_LC;

extern COMBINATION EUROCODE_ULSLC[];
extern int EUROCODE_MC_ULSLC[];
extern int EUROCODE_ULSLC_NO;

extern COMBINATION EUROCODE_SLSLC[];
extern int EUROCODE_MC_SLSLC[];
extern int EUROCODE_SLSLC_NO;

extern COMBINATION EUROCODE_QPSLSLC[];
extern int EUROCODE_MC_QPSLSLC[];
extern int EUROCODE_QPSLSLC_NO;


extern COMBINATION ASCE_ULSLC[];
extern int ASCE_MC_ULSLC[];
extern int ASCE_ULSLC_NO;

extern COMBINATION ASCE_SLSLC[];
extern int ASCE_MC_SLSLC[];
extern int ASCE_SLSLC_NO;

extern COMBINATION ASCE_QPSLSLC[];
extern int ASCE_MC_QPSLSLC[];
extern int ASCE_QPSLSLC_NO;

extern COMBINATION ICC_ULSLC[];
extern int ICC_MC_ULSLC[];
extern int ICC_ULSLC_NO;

extern COMBINATION ICC_SLSLC[];
extern int ICC_MC_SLSLC[];
extern int ICC_SLSLC_NO;

extern COMBINATION ICC_QPSLSLC[];
extern int ICC_MC_QPSLSLC[];
extern int ICC_QPSLSLC_NO;

extern int eurocode_combi_factors_uls[28][11];
extern int eurocode_combi_factors_sls[13][11];
extern int eurocode_combi_factors_qpsls[8][11];

extern int asce_combi_factors_uls[16][11];
extern int asce_combi_factors_sls[16][11];
extern int asce_combi_factors_qpsls[1][11];

extern int icc_combi_factors_uls[16][11];
extern int icc_combi_factors_sls[16][11];
extern int icc_combi_factors_qpsls[8][11];

//for EUROCODE
extern int load_flag_EU[11];
//for ASCE
extern int load_flag_ASCE[11];
//for ICC
extern int load_flag_ICC[11];

extern int load_flag_ALL[11];

extern int *load_flag;

#define Utf8Char unsigned char	// must be 1 byte, 8 bits, can be char, the UTF consortium specify unsigned
extern Utf8Char* Utf8StrMakeUprUtf8Str(const Utf8Char *pUtf8);

static void add_load_factors_sh(void)
{
    sh_load_factors_no++;
    if (sh_load_factors_no==SH_LOAD_FACTORS_MAX)
    {
        SH_LOAD_FACTORS_MAX+=100;
        sh_load_factors=realloc(sh_load_factors, SH_LOAD_FACTORS_MAX * sizeof(ST_LOAD_FACTORS));
    }

    memmove(&sh_load_factors[sh_load_factors_no], &load_factors[0], sizeof(ST_LOAD_FACTORS));
}

static void add_property_sh(void)
{
    sh_property_no++;
    if (sh_property_no==SH_PROPERTY_MAX) {
        SH_PROPERTY_MAX+=10;
        sh_property=realloc(sh_property, SH_PROPERTY_MAX * sizeof(ST_PROPERTY));
    }
    memmove(&sh_property[sh_property_no], &prt_def_sh, sizeof(ST_PROPERTY));
    //if (prop_precisions_sh == &SI_precisions)
    if (UNITS==SI) sh_property[sh_property_no].c=0.03; //[m]
    else sh_property[sh_property_no].c=1.18; //[in]
}

static void add_node_sh(void)
{
    sh_node_no++;
    if (sh_node_no==SH_NODE_MAX) {
        SH_NODE_MAX+=100;
        sh_node=realloc(sh_node, SH_NODE_MAX * sizeof(SH_NODE));
    }
}

static void add_node_emb_sh(void)
{
    sh_node_emb_no++;
    if (sh_node_emb_no==SH_NODE_EMB_MAX) {
        SH_NODE_EMB_MAX+=10;
        sh_node_emb=realloc(sh_node_emb, SH_NODE_EMB_MAX * sizeof(SH_NODE_EMB));
    }
}

static void add_node_emb_edge_sh(void)
{
    sh_node_emb_edge_no++;
    if (sh_node_emb_edge_no==SH_NODE_EMB_EDGE_MAX) {
        SH_NODE_EMB_EDGE_MAX+=10;
        sh_node_emb_edge=realloc(sh_node_emb_edge, SH_NODE_EMB_EDGE_MAX * sizeof(SH_NODE_EMB_EDGE));
    }
}

static void add_edge_sh(void)
{
    sh_edge_no++;
    if (sh_edge_no==SH_EDGE_MAX) {
        SH_EDGE_MAX+=100;
        sh_edge=realloc(sh_edge, SH_EDGE_MAX * sizeof(SH_EDGE));
    }
}

static void add_edge_emb_sh(void)
{
    sh_edge_emb_no++;
    if (sh_edge_emb_no==SH_EDGE_EMB_MAX) {
        SH_EDGE_EMB_MAX+=10;
        sh_edge_emb=realloc(sh_edge_emb, SH_EDGE_EMB_MAX * sizeof(SH_EDGE));
    }
}

static void add_shield_property(void)
{
    shield_no++;
    if (shield_no==SH_SHIELD_MAX) {
        SH_SHIELD_MAX+=10;
        shield_property=realloc(shield_property, SH_SHIELD_MAX * sizeof(SHIELD_PROPERTY));
    }
}

static void add_hole_property(void)
{
    hole_no++;
    if (hole_no==SH_HOLE_MAX) {
        SH_HOLE_MAX+=10;
        hole_property=realloc(hole_property, SH_HOLE_MAX * sizeof(SHIELD_PROPERTY));
    }
}

static void add_wall_property(void)
{
    wall_no++;
    if (wall_no==SH_WALL_MAX) {
        SH_WALL_MAX+=10;
        wall_property=realloc(wall_property, SH_WALL_MAX * sizeof(SHIELD_PROPERTY));
    }
}

static void add_zone_property(void)
{
    zone_no++;
    if (zone_no==SH_ZONE_MAX) {
        SH_ZONE_MAX+=10;
        zone_property=realloc(zone_property, SH_ZONE_MAX * sizeof(SHIELD_PROPERTY));
    }
}

static void add_load_sh(void)
{
    sh_load_no++;
    if (sh_load_no==SH_LOAD_MAX) {
        SH_LOAD_MAX+=10;
        sh_load=realloc(sh_load, SH_LOAD_MAX * sizeof(SH_LOAD));
    }
}

//static void add_point_load_sh(void)
//{
//    sh_point_load_no++;
//    if (sh_point_load_no==SH_POINT_LOAD_MAX) {
//        SH_POINT_LOAD_MAX+=10;
//        sh_point_load=realloc(sh_point_load, SH_POINT_LOAD_MAX * sizeof(SH_LOAD));
//    }
//}

static void add_geo_line(void)
{
    geo_line_no++;
    if (geo_line_no==GEO_LINE_MAX)
    {
        GEO_LINE_MAX+=1000;
        geo_line = realloc(geo_line, GEO_LINE_MAX * sizeof(GEO_LINE));
    }
    memmove(&geo_line[geo_line_no].L, &Ldsp, sizeof(LINIA));
    geo_line[geo_line_no].n1=-1;
    geo_line[geo_line_no].n2=-1;
}

int create_shield(BLOK *b, int style, int number, int body_no, int *first, int *last, int *property_no, BOOL *is_closed) {
    NAGLOWEK *nag;
    char *adp, *adk;
    TEXT *t;
    LINIA *L;
    LUK *l;
    BLOK *b1;
    int i, el_no, t_no;
    double x1, y1, x2, y2;
    PLINIA PL;
    float dl, dr;
    double last_x, last_y;
    int ret, no;
    double xmin, ymin, xmax, ymax;

    *is_closed = 0;
    el_no = 0;
    t_no = 0;
    *property_no = -1;
    *first = sh_edge_no;

    adp = (char *) b + sizeof(NAGLOWEK) + B3 + b->dlugosc_opisu_obiektu;
    adk = (char *) b + sizeof(NAGLOWEK) + b->n - 1;
    while (adp < adk) {
        nag = (NAGLOWEK *) adp;
        if (nag->obiekt == OdBLOK)  //in case there is joined block
        {
            b1 = (BLOK *) nag;
            adp += sizeof(NAGLOWEK) + B3 + b1->dlugosc_opisu_obiektu;
        } else if (nag->obiekt == Otekst) {
            t = (TEXT *) nag;
            // Use sscanf to match '#' and then read an integer
            if (sscanf(t->text, "%*[^0123456789]%d", property_no) == 1) {
                printf("Found number: %d\n", *property_no);
                t_no++;
            } else {
                printf("Number after '#' not found or not in expected format.\n");
            }
            adp += sizeof(NAGLOWEK) + t->n;
        } else if (nag->obiekt == Olinia) {
            L = (LINIA *) nag;
            parametry_lini(L, &PL);
            dl = PL.dl * dxl;

            sh_edge[sh_edge_no].style = style;
            sh_edge[sh_edge_no].number = number;
            sh_edge[sh_edge_no].body_number = body_no;
            sh_edge[sh_edge_no].node1 = -1;
            sh_edge[sh_edge_no].node2 = -1;
            sh_edge[sh_edge_no].node3 = -1;
            sh_edge[sh_edge_no].type = 0;  //line
            sh_edge[sh_edge_no].inverted = 0;

            //calculate midpoint
            sh_edge[sh_edge_no].xm = (L->x1+L->x2)/2.0f;
            sh_edge[sh_edge_no].ym = (L->y1+L->y2)/2.0f;

            for (i = 0; i < sh_node_no; i++)  //node1
            {
                if ((Check_if_Equal(L->x1, sh_node[i].x)) && (Check_if_Equal(L->y1, sh_node[i].y))) {
                    sh_edge[sh_edge_no].node1 = i;  //counting from 0
                    if (sh_node[i].d > dl) sh_node[i].d = dl;  //shrinking the division for FE
                    break;
                }
            }
            for (i = 0; i < sh_node_no; i++)  //node2
            {
                if ((Check_if_Equal(L->x2, sh_node[i].x)) && (Check_if_Equal(L->y2, sh_node[i].y))) {
                    sh_edge[sh_edge_no].node2 = i;
                    if (sh_node[i].d > dl) sh_node[i].d = dl; //shrinking the division for FE
                    break;
                }
            }

            if (sh_edge[sh_edge_no].node1 == -1) {  //not found
                sh_node[sh_node_no].x = L->x1;
                sh_node[sh_node_no].y = L->y1;
                sh_node[sh_node_no].d = dl; //setting the division for FE
                sh_edge[sh_edge_no].node1 = sh_node_no;
                add_node_sh();
            }
            if (sh_edge[sh_edge_no].node2 == -1) {  //not found
                sh_node[sh_node_no].x = L->x2;
                sh_node[sh_node_no].y = L->y2;
                sh_node[sh_node_no].d = dl; //setting the division for FE
                sh_edge[sh_edge_no].node2 = sh_node_no;
                add_node_sh();
            }

            if (style == 2)  //wall
            {
                sh_edge[sh_edge_no].restraint = max(6, L->obiektt2);  //6 is default
            }
            else sh_edge[sh_edge_no].restraint = L->obiektt2;  //could be check logically, 0, 5,6,7 or 4

            sh_edge[sh_edge_no].adr=(char*)L;
            sh_edge[sh_edge_no].load_no=-1;
            //// to match with next element
            last_x = L->x2;
            last_y = L->y2;

            if (style == 0)  //shield
            {
                line_place(L, &sh_min_x, &sh_min_y, &sh_max_x, &sh_max_y);
            }

            add_edge_sh();

            el_no++;
            adp += sizeof(NAGLOWEK) + L->n;

        } else if (nag->obiekt == Oluk) {
            l = (LUK *) nag;
            dr = l->r * dxr;

            sh_edge[sh_edge_no].node1 = -1;
            sh_edge[sh_edge_no].node2 = -1;
            sh_edge[sh_edge_no].node3 = -1;
            sh_edge[sh_edge_no].type = 1;  //arc

            //calculating end points
            x1 = l->x + l->r * cos(l->kat1);
            y1 = l->y + l->r * sin(l->kat1);
            x2 = l->x + l->r * cos(l->kat2);
            y2 = l->y + l->r * sin(l->kat2);

            //calculate midpoint
            double a1 = Angle_Normal (l->kat1) ;
            double a2 = Angle_Normal (l->kat2) ;
            if(a1>a2) a2+=Pi2;
            sh_edge[sh_edge_no].xm=(float)(l->x+l->r*cos((a1+a2)/2.));
            sh_edge[sh_edge_no].ym=(float)(l->y+l->r*sin((a1+a2)/2.));

            //sh_edge[sh_edge_no].xm = l->x + l->r * cos((l->kat2+l->kat1)/2.0f);
            //sh_edge[sh_edge_no].ym = l->y + l->r * sin((l->kat2+l->kat1)/2.0f);

            if ((Check_if_Equal(x1, last_x) == TRUE) && (Check_if_Equal(y1, last_y) == TRUE))
                sh_edge[sh_edge_no].inverted = 0;
            else sh_edge[sh_edge_no].inverted = 1;

            for (i = 0; i < sh_node_no; i++)  //node1
            {
                if ((Check_if_Equal(x1, sh_node[i].x)) && (Check_if_Equal(y1, sh_node[i].y))) {
                    sh_edge[sh_edge_no].node1 = i;
                    if (sh_node[i].d > dr) sh_node[i].d = dr; //shrinking the division for FE
                    break;
                }
            }
            for (i = 0; i < sh_node_no; i++)  //node2
            {
                if ((Check_if_Equal(l->x, sh_node[i].x)) && (Check_if_Equal(l->y, sh_node[i].y))) {
                    sh_edge[sh_edge_no].node2 = i;
                    if (sh_node[i].d > dr) sh_node[i].d = dr; //shrinking the division for FE
                    break;
                }
            }
            for (i = 0; i < sh_node_no; i++)  //node3
            {
                if ((Check_if_Equal(x2, sh_node[i].x)) && (Check_if_Equal(y2, sh_node[i].y))) {
                    sh_edge[sh_edge_no].node3 = i;
                    if (sh_node[i].d > dr) sh_node[i].d = dr; //shrinking the division for FE
                    break;
                }
            }

            if (sh_edge[sh_edge_no].node1 == -1) {  //not found
                sh_node[sh_node_no].x = x1;
                sh_node[sh_node_no].y = y1;
                sh_node[sh_node_no].d = dr; //setting the division for FE
                sh_edge[sh_edge_no].node1 = sh_node_no;
                add_node_sh();
            }

            if (sh_edge[sh_edge_no].node2 == -1) {  //not found
                sh_node[sh_node_no].x = l->x;
                sh_node[sh_node_no].y = l->y;
                sh_node[sh_node_no].d = dr; //setting the division for FE
                sh_edge[sh_edge_no].node2 = sh_node_no;
                add_node_sh();
            }

            if (sh_edge[sh_edge_no].node3 == -1) {  //not found
                sh_node[sh_node_no].x = x2;
                sh_node[sh_node_no].y = y2;
                sh_node[sh_node_no].d = dr; //setting the division for FE
                sh_edge[sh_edge_no].node3 = sh_node_no;
                add_node_sh();
            }

            //sh_edge[sh_edge_no].inverted = 0;  //has to be confronted with the endpoint of previous segment

            sh_edge[sh_edge_no].restraint = l->obiektt2;  //could be check logically, 0, 5,6,7 or 4

            sh_edge[sh_edge_no].adr=(char*)l;
            sh_edge[sh_edge_no].load_no=-1;
            //// to match with next element

            if (sh_edge[sh_edge_no].inverted==1)
            {
                last_x = x1;
                last_y = y1;
            }
            else
            {
                last_x = x2;
                last_y = y2;
            }

            if (style == 0)  //shield
            {
                luk_place(l, &sh_min_x, &sh_min_y, &sh_max_x, &sh_max_y);
            }

            add_edge_sh();

            el_no++;
            adp += sizeof(NAGLOWEK) + l->n;
        } else //some garbage
        {
            adp += sizeof(NAGLOWEK) + nag->n;
        }
    }
    *last = sh_edge_no;

    if ((style == 0) || (style == 3)) {
        if (t_no == 0) {
            ////TO DO ERROR MESSAGE
            return 0;
        }
    }

    if (el_no > 2) {
        //check if closed
        if (sh_edge[*first].type==0) //line
        {
            if (sh_edge[*last - 1].type==0) //line
            {
                if ((sh_edge[*first].node1 == sh_edge[*last - 1].node2) ||
                    (sh_edge[*first].node1 == sh_edge[*last - 1].node1) ||
                    (sh_edge[*first].node2 == sh_edge[*last - 1].node1) ||
                    (sh_edge[*first].node2 == sh_edge[*last - 1].node2))
                    *is_closed = TRUE;
            }
            else //circle
            {
                if ((sh_edge[*first].node1 == sh_edge[*last - 1].node3) ||
                    (sh_edge[*first].node1 == sh_edge[*last - 1].node1) ||
                    (sh_edge[*first].node2 == sh_edge[*last - 1].node1) ||
                    (sh_edge[*first].node2 == sh_edge[*last - 1].node3))
                    *is_closed = TRUE;
            }
        }
        else  //circle
        {
            if (sh_edge[*last - 1].type==0) //line
            {
                if ((sh_edge[*first].node1 == sh_edge[*last - 1].node2) ||
                    (sh_edge[*first].node1 == sh_edge[*last - 1].node1) ||
                    (sh_edge[*first].node3 == sh_edge[*last - 1].node1) ||
                    (sh_edge[*first].node3 == sh_edge[*last - 1].node2))
                    *is_closed = TRUE;
            }
            else //circle
            {
                if ((sh_edge[*first].node1 == sh_edge[*last - 1].node3) ||
                    (sh_edge[*first].node1 == sh_edge[*last - 1].node1) ||
                    (sh_edge[*first].node3 == sh_edge[*last - 1].node1) ||
                    (sh_edge[*first].node3 == sh_edge[*last - 1].node3))
                    *is_closed = TRUE;
            }
        }
        return 1;
    }
    else if (style==2)
    {
        if (el_no>0) return 1;
        else return 0;
    }
    else return 0;
}

static char *add_block(double x, double y, char kod_obiektu, char *blok_type, BOOL hiding)
/*--------------------------------------------------------------------------------------*/
{
    unsigned size_block = B3;
    BLOK s_blockd = Bdef;
#ifndef LINUX
    BLOK blokd = Bdef;
#else
    BLOK blokd = Bdef;
#endif
    BLOK *ptrs_block, *buf_block=NULL;
    int  len_type, len_desc;
    T_Desc_Ex_Block *ptrs_desc_bl;

    len_type = (int) strlen(blok_type) + 1;
    len_desc = sizeof(unsigned) + 2 * sizeof(float) + sizeof(len_type) + len_type;
    size_block += len_desc;

    if (NULL == (buf_block = (BLOK*)malloc(sizeof(NAGLOWEK) + size_block + M100)))   //100 is just for a case  //12-10-2025
    {
        return NULL;
    }
    memmove(buf_block, &blokd, sizeof(blokd));
    buf_block->n = size_block;
    buf_block->kod_obiektu = kod_obiektu;
    buf_block->dlugosc_opisu_obiektu = len_desc;

    buf_block->flag = 1 | (hiding*2);  //this is new, for hiding block purpose flag: bit 1: ability to hide, bit 2: hidden

    ptrs_desc_bl = (T_Desc_Ex_Block *) (&buf_block->opis_obiektu[0]);
    ptrs_desc_bl->flags = EBF_IP;
    ptrs_desc_bl->x = (float)x;
    ptrs_desc_bl->y = (float)y;
    ptrs_desc_bl->len = len_type;
    strcpy(&ptrs_desc_bl->sz_type[0], blok_type);

    if ((ptrs_block = (BLOK*)dodaj_obiekt(NULL, buf_block)) == NULL)
    {
        free(buf_block);
        return NULL;
    }

    free(buf_block);
    return (char*)ptrs_block ;
}

//static void my_free(void* adr)
//{
//    return;
//}

static int factor_record(unsigned char load, unsigned char variant)
{
    for (int i=0; i<st_load_factors_no; i++)
    {
        if (sh_load_factors[i].load==load)
        {
            if (variant>0)
            {
                if (sh_load_factors[i].variant==variant)
                    return i;
            }
        }
    }
    return -1;
}

extern BOOL rout;

static void do_nothing(void)
{
    return;
}

//char *chapters[]={u8"δ.x", u8"δ.y", u8"δ.z", u8"θ.xx", u8"θ.yy", u8"θ.zz", u8"σ.x", u8"σ.y", u8"σ.z", u8"τ.xy", u8"τ.xz", u8"τ.yz", u8"σ.eq",
//                 u8"ε.xx", u8"ε.yy", u8"ε.zz", u8"ε.xy", u8"ε.xz", u8"ε.yz"};
extern char *chapters[];  //taken from o_plate.c

static void modify_chapter(char *chapter)
{
    //changing block name and colorbar description
         if (strstr(chapter, "deflection 1")) strcpy(chapter, chapters[0]);else if (strstr(chapter, "deflection 2")) strcpy(chapter, chapters[1]);
    else if (strstr(chapter, "deflection 3")) strcpy(chapter, chapters[2]);
    else if (strstr(chapter, "deflection 4")) strcpy(chapter, chapters[3]);
    else if (strstr(chapter, "deflection 5")) strcpy(chapter, chapters[4]);
    else if (strstr(chapter, "deflection 6")) strcpy(chapter, chapters[5]);
    else if (strstr(chapter, "stress.xx")) strcpy(chapter, chapters[6]);
    else if (strstr(chapter, "stress.yy")) strcpy(chapter, chapters[7]);
    else if (strstr(chapter, "stress.zz")) strcpy(chapter, chapters[8]);
    else if (strstr(chapter, "stress.xy")) strcpy(chapter, chapters[9]);
    else if (strstr(chapter, "stress.xz")) strcpy(chapter, chapters[10]);
    else if (strstr(chapter, "stress.yz")) strcpy(chapter, chapters[11]);
    else if (strstr(chapter, "stress.eq")) strcpy(chapter, chapters[12]);

    else if (strstr(chapter, "epsilon.tot.xx")) strcpy(chapter, chapters[13]);
    else if (strstr(chapter, "epsilon.tot.yy")) strcpy(chapter, chapters[14]);
    else if (strstr(chapter, "epsilon.tot.zz")) strcpy(chapter, chapters[15]);
    else if (strstr(chapter, "epsilon.tot.xy")) strcpy(chapter, chapters[16]);
    else if (strstr(chapter, "epsilon.tot.xz")) strcpy(chapter, chapters[17]);
    else if (strstr(chapter, "epsilon.tot.yz")) strcpy(chapter, chapters[18]);

}

static void embed_node(int load_no, double geo_units_factor)
{
    int k;
//check if point must be embeded
    for (k = 0; k < sh_node_no; k++)
    {
        if ((Check_if_Equal(sh_node[k].x, sh_load[load_no].x1) == TRUE) &&
            (Check_if_Equal(sh_node[k].y, sh_load[load_no].y1) == TRUE))
            break;  //no need to embed
    }
    if (k == sh_node_no)  //checking if already embeded
    {
        for (k = 0; k < sh_node_emb_no; k++) {
            if ((Check_if_Equal(sh_node_emb[k].x, sh_load[load_no].x1) == TRUE) &&
                (Check_if_Equal(sh_node_emb[k].y, sh_load[load_no].y1) == TRUE))
                break;  //no need to embed
        }
    }
    if (k == sh_node_emb_no)  //need to embed
    {
        sh_node_emb[sh_node_emb_no].x = sh_load[load_no].x1;
        sh_node_emb[sh_node_emb_no].y = sh_load[load_no].y1;
        sh_node_emb[sh_node_emb_no].d = (float)(dxl_min / geo_units_factor);  //for compatibility
        sh_node_emb[sh_node_emb_no].body = sh_load[load_no].point_body;
        sh_node_emb[sh_node_emb_no].body_no = sh_load[load_no].point_body_no;
        add_node_emb_sh();
    }
}

static void embed_nodes_edge(int load_no, int style, int number, int body_no, double geo_units_factor)
{
    int k1, k2;
//check if point 1 must be embeded
    for (k1 = 0; k1 < sh_node_no; k1++)
    {
        if ((Check_if_Equal(sh_node[k1].x, sh_load[load_no].x1) == TRUE) &&
            (Check_if_Equal(sh_node[k1].y, sh_load[load_no].y1) == TRUE))
            break;  //no need to embed
    }
    if (k1 == sh_node_no)  //checking if already embeded
    {
        for (k1 = 0; k1 < sh_node_emb_no; k1++) {
            if ((Check_if_Equal(sh_node_emb[k1].x, sh_load[load_no].x1) == TRUE) &&
                (Check_if_Equal(sh_node_emb[k1].y, sh_load[load_no].y1) == TRUE))
                break;  //no need to embed
        }
    }

    if (k1 == sh_node_emb_no)  //need to embed
    {
        sh_node_emb[sh_node_emb_no].x = sh_load[load_no].x1;
        sh_node_emb[sh_node_emb_no].y = sh_load[load_no].y1;
        sh_node_emb[sh_node_emb_no].d = (float)(dxl_min / geo_units_factor);  //for compatibility
        sh_node_emb[sh_node_emb_no].body = sh_load[load_no].point_body;
        sh_node_emb[sh_node_emb_no].body_no = sh_load[load_no].point_body_no;
        k1 = sh_node_no + sh_node_emb_no;
        add_node_emb_sh();
    }
    //check if point 2 must be embeded
    for (k2 = 0; k2 < sh_node_no; k2++)
    {
        if ((Check_if_Equal(sh_node[k2].x, sh_load[load_no].x2) == TRUE) &&
            (Check_if_Equal(sh_node[k2].y, sh_load[load_no].y2) == TRUE))
            break;  //no need to embed
    }
    if (k2 == sh_node_no)  //checking if already embeded
    {
        for (k2 = 0; k2 < sh_node_emb_no; k2++) {
            if ((Check_if_Equal(sh_node_emb[k2].x, sh_load[load_no].x2) == TRUE) &&
                (Check_if_Equal(sh_node_emb[k2].y, sh_load[load_no].y2) == TRUE))
                break;  //no need to embed
        }
    }

    if (k2 == sh_node_emb_no)  //need to embed
    {
        sh_node_emb[sh_node_emb_no].x = sh_load[load_no].x2;
        sh_node_emb[sh_node_emb_no].y = sh_load[load_no].y2;
        sh_node_emb[sh_node_emb_no].d = (float)(dxl_min / geo_units_factor);  //for compatibility
        sh_node_emb[sh_node_emb_no].body = sh_load[load_no].point_body;
        sh_node_emb[sh_node_emb_no].body_no = sh_load[load_no].point_body_no;
        k2 = sh_node_no + sh_node_emb_no;
        add_node_emb_sh();
    }
    //embed edge based on k1 and k2 points
    sh_edge_emb[sh_edge_emb_no].node1 = k1;
    sh_edge_emb[sh_edge_emb_no].node2 = k2;
    sh_edge_emb[sh_edge_emb_no].node3 = -1;
    sh_edge_emb[sh_edge_emb_no].type = 0; //line
    sh_edge_emb[sh_edge_emb_no].restraint = 0;
    sh_edge_emb[sh_edge_emb_no].inverted = 0;
    sh_edge_emb[sh_edge_emb_no].style = style;
    sh_edge_emb[sh_edge_emb_no].number = number;
    sh_edge_emb[sh_edge_emb_no].body_number = body_no;
    sh_edge_emb[sh_edge_emb_no].load_no = load_no;

    add_edge_emb_sh();
}

static void embed_node_edge(int load_no, double geo_units_factor)
{
    int k;
//check if point must be embeded
    for (k = 0; k < sh_node_no; k++)
    {
        if ((Check_if_Equal(sh_node[k].x, sh_load[load_no].x1) == TRUE) &&
            (Check_if_Equal(sh_node[k].y, sh_load[load_no].y1) == TRUE))
            break;  //no need to embed
    }
    if (k == sh_node_no)  //checking if already embeded on edge
    {
        for (k = 0; k < sh_node_emb_edge_no; k++) {
            if ((Check_if_Equal(sh_node_emb_edge[k].x, sh_load[load_no].x1) == TRUE) &&
                (Check_if_Equal(sh_node_emb_edge[k].y, sh_load[load_no].y1) == TRUE))
                break;  //no need to embed on edge
        }
    }
    if (k == sh_node_emb_edge_no)  //need to embed on edge
    {
        sh_node_emb_edge[sh_node_emb_edge_no].x = sh_load[load_no].x1;
        sh_node_emb_edge[sh_node_emb_edge_no].y = sh_load[load_no].y1;
        sh_node_emb_edge[sh_node_emb_edge_no].d = (float)(dxl_min / geo_units_factor);  //for compatibility
        sh_node_emb_edge[sh_node_emb_edge_no].body = sh_load[load_no].point_body;
        sh_node_emb_edge[sh_node_emb_edge_no].body_no = sh_load[load_no].point_body_no;
        sh_node_emb_edge[sh_node_emb_edge_no].edge_no = sh_load[load_no].point_edge_no;
        add_node_emb_edge_sh();
    }
}

// Simplified ACI 318-19 procedure for RC section (SI: Pa, m, Nm; Imperial: psi, in, lbf-in)
static int calculate_rebars_aci_simplified_UNI_with_N(double M, double N, double h, double b, double c,
                                    double fck, double fcd, double fyd, double eta, double lambda,
                                    double Es, int use_min_reinf, int is_si_units,
                                    double *As, double *As_prime, double *rho, double *rho_prime,
                                    double *total_rho, double *sigma_c_max) {
    double d = h - c;
    double A_g = b * h;
    double Ec = is_si_units ? 4700 * sqrt(fck / 1e6) * 1e6 : 57000 * sqrt(fck); // SI: Pa, Imperial: psi
    double n = Es / Ec;
    double beta1 = is_si_units ? (fck <= 28e6 ? 0.85 : 0.85 - 0.05 * (fck / 1e6 - 28) / 4) :
                   (fck <= 4000 ? 0.85 : 0.85 - 0.05 * (fck / 1000 - 4));
    if (beta1 < 0.65) beta1 = 0.65;
    double phi = 0.9;

    // Calculate f_ctm and As_min based on unit system
    double f_ctm, As_min;
    if (is_si_units) {
        f_ctm = 0.3 * pow(fck / 1e6, 2.0 / 3.0) * 1e6; // f_ctm in Pa
        As_min = use_min_reinf ? fmax(0.26 * f_ctm / fyd * b * d, 0.0013 * b * d) : 0.0; // As_min in m²
    } else {
        f_ctm = 0.3 * pow(fck / 1000, 2.0 / 3.0) * 1000; // f_ctm in psi
        As_min = use_min_reinf ? fmax(0.26 * f_ctm / fyd * b * d, 0.0013 * b * d) : 0.0; // As_min in in²
    }

    // Simplified moment capacity check
    double a = M / (phi * 0.85 * fcd * b);
    *As = (0.85 * fcd * a * b - (-N)) / fyd;
    *As = fmax(*As, As_min); // Ensure minimum reinforcement
    *As_prime = 0.0;
    *rho = *As / (b * d);
    *rho_prime = 0.0;
    *total_rho = *rho;
    *sigma_c_max = fabs(N) / A_g + 6 * fabs(M) / (b * h * h);
    *sigma_c_max = fabs(*sigma_c_max);

#ifndef NDEBUG
    // Warnings for reinforcement limits
    if (*total_rho > 0.08) {
        printf("Warning: Total reinforcement ratio (%.2f%%) exceeds 8%% (ACI 318-19).\n", *total_rho * 100);
    }
    if (*sigma_c_max > (is_si_units ? 0.85 * fck / 1.5 : 0.85 * fck)) {
        printf("Warning: sigma_c_max (%.2f %s) exceeds limit (%.2f %s).\n",
               *sigma_c_max / (is_si_units ? 1e6 : 1000), is_si_units ? "MPa" : "ksi",
               (is_si_units ? 0.85 * fck / 1.5 / 1e6 : 0.85 * fck / 1000), is_si_units ? "MPa" : "ksi");
    }
    printf("Doubly reinforced: As = %.2f %s, As_prime = %.2f %s, rho = %.2f%%, sigma_c_max = %.2f %s\n",
           *As * (is_si_units ? 1e6 : 1), is_si_units ? "mm²" : "in²",
           *As_prime * (is_si_units ? 1e6 : 1), is_si_units ? "mm²" : "in²",
           *rho * 100, *sigma_c_max / (is_si_units ? 1e6 : 1000), is_si_units ? "MPa" : "ksi");
#endif
    return 0;
}

////NEW STUFF
/**
 * Shield/Tunnel Lining Reinforcement per Eurocode 2 (or ACI with minor changes)
 * Handles pure compression + minimum eccentricity → always doubly reinforced
 * Units: SI (m, Pa, N, Nm)
 *
 * Parameters:
 *   N       : Design axial force (positive = compression) [N/m]  ← per unit length!
 *   t       : Shield thickness (h) [m]
 *   b       : Width = 1.0 m (always)
 *   cc      : Concrete cover (both faces) [m]
 *   fck, fcd, fyd, eta, lambda → same as before
 *   min_ecc : Minimum eccentricity (default h/30 = t/30) [m]
 *   use_aci : false = EC2, true = ACI 318 (minor differences)
 *
 * Output:
 *   As_inner, As_outer → reinforcement areas [m²/m] both faces
 *   rho_total          → total reinforcement ratio
 *   sigma_c            → actual max concrete stress (for serviceability check)
 */
int calculate_shield_reinforcement_eurocode(
        double N,           // [N/m] positive = compression
        double t,
        double cc,
        double fck, double fcd, double fyd,
        double eta, double lambda,
        double min_ecc_ratio,   // e.g. 30.0 → t/30
        int    use_min_reinf,
        double *As_inner,
        double *As_outer,
        double *rho_total,
        double *sigma_c_max)
{
    const double b = 1.0;                    // per meter
    double h = t;
    double d1 = cc;                          // distance from compression face to inner rebar
    double d2 = h - cc;                      // distance from compression face to outer rebar

    // Minimum eccentricity per EC2 6.2.3(2): e_min = max(h/30, 20 mm)
    double e_min = fmax(h / min_ecc_ratio, 0.020);
    double M = N * e_min;                    // [Nm/m] — the only moment we allow

    // Effective depth for tension side (outer face under accidental bending)
    double d = h - cc;                       // same as plate

    // Total compressive force capacity of concrete
    double Fc_max = eta * fcd * lambda * h * b;   // full section in compression

    if (N >= Fc_max * 0.99) {
        // Almost pure compression → symmetric reinforcement
        double As_sym = N / fyd;             // both layers take equal share
        *As_inner = As_sym / 2.0;
        *As_outer = As_sym / 2.0;
    }
    else {
        // Standard interaction: N + M = N·e_min
        // Use rectangular stress block
        double x = (N + M / (d - h/2.0 + d1)) / (eta * fcd * lambda * b);
        x = fmax(x, 0.0);
        x = fmin(x, h);

        double Fc = eta * fcd * lambda * x * b;
        double z_conc = h/2.0 - lambda*x/2.0;

        // Force equilibrium: Fc + Fs' = N + Fs
        double Fs_prime = Fc - N;                    // compression steel force
        if (Fs_prime < 0) Fs_prime = 0;

        *As_outer = (N + M / (d - z_conc)) / fyd;    // tension steel
        *As_inner = Fs_prime / fyd;                  // compression steel

        // Ensure minimum reinforcement
        if (use_min_reinf) {
            double fctm = 0.3 * pow(fck * 1e-6, 2.0/3.0) * 1e6;
            double As_min = fmax(0.26 * fctm / fyd * b * d, 0.0013 * b * d);
            *As_outer = fmax(*As_outer, As_min / 2.0);
            *As_inner = fmax(*As_inner, As_min / 2.0);
        }
    }

    // Final symmetric fallback if needed
    if (*As_inner < 0) *As_inner = *As_outer;

    *rho_total = (*As_inner + *As_outer) / (b * h);

    // Concrete stress (for crack width, fatigue, etc.)
    *sigma_c_max = N / (b * h) + M * (h/2.0) / (b * h*h / 6.0);  // elastic
    if (*sigma_c_max <= 0) *sigma_c_max = fcd * 0.6;  // serviceability limit

    // Optional: cap at fcd
    if (*sigma_c_max > fcd) *sigma_c_max = fcd;

    return 0;
}

/**
 * UNIVERSAL ACI 318 Shield/Tunnel Lining Reinforcement (N + e_min)
 * Works in SI (Pa, m) or Imperial (psi, in) — auto-detected by fyk magnitude
 * Positive N = compression (you pass -σ_avg * t)
 *
 * Returns:
 *   As_inner / As_outer → both faces [m²/m or in²/ft]
 *   rho_total           → total reinforcement ratio
 *   sigma_c_max         → max concrete stress (for crack control)
 */
int calculate_shield_rebars_aci_UNI(
        double N_comp,          // Axial compression force per unit width [N/m or lbf/ft] → positive!
        double t,               // thickness [m or in]
        double cc,              // cover both sides [m or in]
        double fyk,             // steel yield [Pa or psi]
        double fyd,             // design steel strength = fyk / 1.0 typically for ACI
        double fck,             // concrete strength [Pa or psi]
        double fcd,             // = 0.85*fck typically for ACI stress block
        int    use_min_reinf,
        double *As_inner,
        double *As_outer,
        double *rho_total,
        double *sigma_c_max)
{
    // ────── UNIT SYSTEM DETECTION ──────
    int is_si = (fyk > 1e8); // > ~100 MPa → SI units
    double b  = is_si ? 1.0   : 12.0;                    // 1 m or 1 ft
    double h = t;
    double to_display_area = is_si ? 1e6 : 1.0;          // mm²/m or in²/ft
    double to_MPa_psi      = is_si ? 1e6 : 1.0;

    // ────── ACI PARAMETERS ──────
    double phi = 0.90;
    double beta1 = (fck/to_MPa_psi <= 4000) ? 0.85 :
                   0.85 - 0.05*(fck/to_MPa_psi - 4000)/1000;
    if (beta1 < 0.65) beta1 = 0.65;

    // Minimum eccentricity per ACI 318-19 §6.6.1.2
    double e_min_in = 0.60 + h/60.0;        // inches
    double e_min    = is_si ? fmax(h/30.0, 0.020) : fmax(e_min_in, 1.0); // m or in
    double M        = N_comp * e_min;       // induced moment

    double d  = h - cc;                     // effective depth to tension steel
    double d1 = cc;                         // distance to compression steel

    // ────── FULL SECTION COMPRESSION CAPACITY ──────
    double Fc_full = 0.85 * fcd * beta1 * h * b;

    *As_inner = 0.0;
    *As_outer = 0.0;

    if (N_comp >= 0.95 * Fc_full) {
        // Nearly pure compression → symmetric reinforcement
        double As_total = N_comp / fyd;
        *As_inner = *As_outer = As_total * 0.5;
    }
    else {
        // N + M interaction using rectangular stress block
        // Solve for neutral axis depth c
        double a = (N_comp + M / (d - h/2.0)) / (0.85 * fcd * b);
        double c = a / beta1;
        c = fmax(c, 0.0);
        c = fmin(c, h);

        double Fc = 0.85 * fcd * beta1 * c * b;
        double z_c = h/2.0 - beta1*c/2.0;

        // Compression steel force
        double Fs_prime = Fc - N_comp;
        if (Fs_prime < 0) Fs_prime = 0.0;

        // Tension steel
        *As_outer = (N_comp + M / (d - z_c)) / fyd;
        *As_inner = Fs_prime / fyd;
    }

    // ────── MINIMUM REINFORCEMENT (ACI 318-19 Table 9.6.1.2 or EC2 fallback) ──────
    if (use_min_reinf) {
        double As_min;
        if (is_si) {
            double fctm = 0.3 * pow(fck/1e6, 2.0/3.0) * 1e6;
            As_min = fmax(0.26 * fctm / fyd * b * d, 0.0013 * b * d);
        } else {
            // ACI 318-19: 3√f'c / fy  or 200/fy (psi)
            double term1 = 3.0 * sqrt(fck) / fyd * b * d;
            double term2 = 200.0 / fyd * b * d;
            As_min = fmax(term1, term2);
        }
        double As_min_per_face = As_min * 0.5;
        if (*As_outer < As_min_per_face) *As_outer = As_min_per_face;
        if (*As_inner < As_min_per_face) *As_inner = As_min_per_face;
    }

    // Ensure symmetry if compression steel negative
    if (*As_inner < 0) *As_inner = *As_outer;

    *rho_total = (*As_inner + *As_outer) / (b * h);

    // Max concrete stress (elastic for crack width)
    double I_g = b * h*h*h / 12.0;
    *sigma_c_max = (N_comp / (b*h)) + (M * (h/2.0)) / I_g;
    if (*sigma_c_max <= 0) *sigma_c_max = 0.45 * fck;  // typical service limit

#ifndef NDEBUG
    printf("SHIELD ACI | N=%.2f kN/m | As_inner=%.1f %s | As_outer=%.1f %s | rho=%.3f%% | sigma_c=%.1f %s\n",
           N_comp/1e3, *As_inner*to_display_area, is_si?"mm²/m":"in²/ft",
           *As_outer*to_display_area, is_si?"mm²/m":"in²/ft",
           *rho_total*100, *sigma_c_max/to_MPa_psi, is_si?"MPa":"psi");
#endif

    return 0;
}

///////////////////////FINAL VERSION
///for SLS state:
// φ = chosen bar diameter (mm)
// rho_eff = total steel ratio for the more tensioned face
// In shield: both faces work → rho_eff ≈ (As_circ + As_long) / (t * 1000)   [mm²/m / mm]
////double phi_mm = 20.0;   // you let user choose
////double rho_eff = (As_circ + As_long) / (t * 1000.0);   // mm²/mm
////double wk_mm = 0.25 * (phi_mm / rho_eff);             // ← magic formula used everywhere
////if (wk_mm > 0.25) wk_mm = 0.25;   // plateau
////shield_results[i].crack_width_mm = wk_mm;
//FINAL SLS OUTPUT YOU SHOULD SHOW (add these four numbers)
////shield_results[i].rho_total_percent_SLS   = … (quasi-permanent)
////shield_results[i].crack_width_mm          = wk_mm;
////shield_results[i].sigma_c_max_SLS_MPa     = max_compression_under_quasi_perm;
////shield_results[i].sls_status              = (wk_mm <= 0.20 && rho_SLS < 3.0) ? OK : WARNING;
///for ULS as below

//IMPORTANT FOR ALL STANDARDS:
/*
// You already run three combinations:
1. ULS / FLC                → for strength & %ρ_total ULS
2. SLS characteristic       → rarely used for tunnels
3. Quasi-permanent / NL     → the one that matters for durability

// Therefore:
if (code == EUROCODE) {
// Crack width + concrete stress limit → use QUASI-PERMANENT
run_shield_calculation(load_combination = QUASI_PERMANENT);
shield_results[i].wk_mm        = calculate_crack_width(...);
shield_results[i].sigma_c_SLS  = max(|σc|, |σl|) under quasi-permanent;
shield_results[i].rho_SLS_percent = 100.0 * (As_c + As_l) / (t*1000);
}
else if (code == ACI || code == ASCE || code == ICC) {
// Crack width + concrete stress limit → use NOMINAL / BASIC load combo
run_shield_calculation(load_combination = NOMINAL_OR_BLC);
shield_results[i].wk_mm        = calculate_crack_width(...);
shield_results[i].sigma_c_SLS  = max(|σc|, |σl|) under service loads;
shield_results[i].rho_SLS_percent = 100.0 * (As_c + As_l) / (t*1000);
}
 */
/**
 * FINAL — 100% CODE-COMPLIANT AXIAL MEMBER DESIGN FOR SHIELDS
 * Works for both Eurocode 2 and ACI 318-19
 * Input: N_comp > 0 (compression force per meter, N/m)
 * Output: Total required steel area per meter (both faces together)
 */

typedef struct {
    //double sigma_x, sigma_y, tau_xy;
    //double Rn, Rt;               // reactions (from earlier)
    double As_longitudinal;              // reinforcement longitudinal (along tunnel axis)
    double As_circumferential;              // reinforcement circumferential (hoop)
    double rho_longitudinal;
    double rho_circumferential;
    double rho_total;
    double principal_angle_deg;      // angle of σ₁ (degrees)
    double As_x;              // reinforcement in X axis
    double As_y;              // reinforcement in Y axis
    double rho_x;
    double rho_y;
    double rho_total_xy;
    // … other results
} SHIELD_POINT_T;

void convert_principal_to_global_reinforcement(
        double As_long,             // [m²/m] in longitudinal direction
        double As_circ,             // [m²/m] in circumferential direction
        double principal_angle_deg, // [degrees] from global X-axis
        double min_rho_total,       // code min total rho (e.g., 0.0013 or 0.0018)
        double h,                   // thickness [m]
        double *As_x,               // output: [m²/m] in global X
        double *As_y)               // output: [m²/m] in global Y
{
    double theta_rad = principal_angle_deg * M_PI / 180.0;

    double c = cos(theta_rad);
    double s = sin(theta_rad);

    // Transformation (assuming As_long along θ + 90°, As_circ along θ — adjust if needed)
    *As_x = As_long * (c*c) + As_circ * (s*s);
    *As_y = As_long * (s*s) + As_circ * (c*c);

    // Total remains the same (invariant)
    double As_total = *As_x + *As_y;   // = As_long + As_circ

    // Apply code minimum rho per direction (e.g., EC2 0.0013 bd per direction)
    double As_min_per_dir = min_rho_total * h * 1.0;   // [m²/m] for b=1.0 m

    *As_x = fmax(*As_x, As_min_per_dir);
    *As_y = fmax(*As_y, As_min_per_dir);

    // Optional: if after min rho, total > max rho (e.g., 0.04), warn user
    if (*As_x + *As_y > 0.04 * h * 1.0) {
        printf("Warning: Total rho after rotation and min exceeds 4%% — check section.\n");
    }
}

double design_axial_member_shield(
        double N_comp,          // [N/m] positive = compression
        double h,               // thickness [m]
        double cc,              // concrete cover both sides [m]
        double fck,             // [Pa]
        double fcd,             // [Pa] = fck/γc
        double fyk,             // [Pa]
        double fyd,             // [Pa] = fyk/γs
        int    code,            // 0 = Eurocode 2, 1 = ACI 318
        double *min_rho_total)
{
    double b = 1.0;         // per meter
    double d = h - cc;      // effective depth

    // ─────────────────────── MINIMUM ECCENTRICITY ───────────────────────
    double e_min;
    if (code == 0) {
        // Eurocode 2 §6.1(4) + EN 1992-2 (tunnels): e_min = max(h/30, 20 mm)
        e_min = fmax(h / 30.0, 0.020);
    } else {
        // ACI 318-19 §6.6.4.5.4 (columns) or §11.5.4 (walls): e_min = (0.6 + 0.03h) inches
        double h_in = h * 39.3701;
        double e_min_in = 0.6 + 0.03 * h_in;
        e_min = fmax(e_min_in * 0.0254, 0.0254);  // convert to meters, min 1 inch
    }
    double Mu = N_comp * e_min;   // [Nm/m] — the design moment

    // ─────────────────────── STRESS BLOCK PARAMETERS ───────────────────────
    double alpha_cc = (code == 0) ? 1.0 : 0.85;          // EC2: 1.0, ACI: 0.85
    double lambda   = (code == 0) ? 0.80 : 0.0;         // EC2: 0.8, ACI: beta1
    double beta1    = 0.85;
    if (code == 1) {
        double fc_MPa = fck / 1e6;
        if (fc_MPa > 30) beta1 = 0.85 - 0.05*(fc_MPa - 30)/10.0;
        if (beta1 < 0.65) beta1 = 0.65;
        lambda = beta1;
    }

    double fcd_block = alpha_cc * fcd;   // design strength in block

    // ──────────────────────── RECTANGULAR STRESS BLOCK SOLUTION ───────────────────────
    // We solve: N = Fc + Fs' - Fs
    //           M = Fc·z_c + Fs'·z_s' - Fs·z_s
    // → exact solution for c (neutral axis depth)

    double A = fcd_block * lambda * b;
    double B = N_comp + Mu / (d - h/2.0 + cc);   // simplified lever arm
    double c = B / A;

    c = fmax(c, 0.0);
    c = fmin(c, h);

    double a = lambda * c;
    double Fc = fcd_block * a * b;

    double Fs_compr = fmax(0.0, Fc - N_comp);           // compression steel force
    double As_compr = Fs_compr / fyd;

    double Fs_tens  = N_comp + Mu / (d - (h/2.0 - a/2.0)); // tension steel
    double As_tens  = Fs_tens / fyd;

    double As_total = As_tens + As_compr;

    // ─────────────────────── MINIMUM REINFORCEMENT (BOTH CODES) ───────────────────────
    double As_min = 0.0;
    if (code == 0) {
        // EC2 §9.1N + EN 1992-2 (tunnels): often 0.2% both faces → 0.004 total
        // But minimum per face 0.0013·b·d (from beams)
        double fctm = 0.30 * pow(fck/1e6, 2.0/3.0) * 1e6;
        As_min = fmax(0.26 * fctm / fyd * b * d, 0.0013 * b * d);
        As_min *= 1.5;  // conservative: apply to both faces
    } else {
        // ACI 318-19 §9.6.1.2 (flexural members → also used for walls
        double term1 = 3.0 * sqrt(fck) / fyd * b * d;
        double term2 = 200000.0 / fyd * b * d;  // 200 psi
        As_min = fmax(term1, term2);
        As_min *= 1.0;  // already total for both faces in ACI practice
    }

    As_total = fmax(As_total, As_min);

    // Optional: maximum reinforcement (EC2 4%, ACI ~6%)
    double As_max = (code == 0) ? 0.04 * b * h : 0.06 * b * h;
    As_total = fmin(As_total, As_max);

    if (min_rho_total) {
        *min_rho_total = As_min / (b * h);   // total minimum rho (both faces)
    }

    return As_total;   // [m²/m] — total for both layers
}

int calculate_shield_reinforcement_final(
        double sigx, double sigy, double tauxy,   // FEM output (compression = negative)
        double t, double cc,
        double fck, double fcd, double fyk, double fyd,
        int code, SHIELD_POINT_T *result)                                 // 0=EC2, 1=ACI
{
    // 1. Principal stresses
    double sig_avg = (sigx + sigy) / 2.0;
    double R = sqrt(pow((sigx - sigy)/2.0, 2) + pow(tauxy, 2));
    double sigma1 = sig_avg + R;   // algebraically larger
    double sigma2 = sig_avg - R;   // more compressive

    double theta_rad = /*Angle_Normal*/(0.5 * atan2(2*tauxy, sigx - sigy));  // correct principal angle
    double theta_deg = theta_rad * 180.0 / M_PI;

    // 2. Design forces (positive = compression)
    double N1 = -sigma1 * t;
    double N2 = -sigma2 * t;

    double min_rho_total;

    // 3. Reinforcement in each principal direction
    double As_principal1 = design_axial_member_shield(N1, t, cc, fck, fcd, fyk, fyd, code, &min_rho_total);
    double As_principal2 = design_axial_member_shield(N2, t, cc, fck, fcd, fyk, fyd, code, &min_rho_total);

#ifndef NDEBUG
    printf("sigx=%lf sigy=%lf tauxy=%lf t=%lf cc=%lf fck=%lf fcd=%lf fyk=%lf fyd=%lf code=%d\n",sigx, sigy, tauxy, t, cc, fck, fcd, fyk, fyd, code);
#endif

    result->rho_longitudinal = As_principal2 / (t * 1.0);
    result->rho_circumferential = As_principal1 / (t * 1.0);
    result->rho_total = (As_principal1 + As_principal2) / (t * 1.0);
    // t in [m] → t*1000 = mm → As in [mm²/m] → rho in %

    // 4. Usually you provide two orthogonal meshes → assign to global directions
    // Example: longitudinal (along tunnel) and circumferential (hoop)
    result->As_longitudinal  = As_principal2;   // usually stronger
    result->As_circumferential = As_principal1;
    result->principal_angle_deg = theta_deg;


    convert_principal_to_global_reinforcement(
            result->As_longitudinal,
            result->As_circumferential,
            result->principal_angle_deg,
            min_rho_total, //0.0013,   // min rho (code-specific)
            t,
            &result->As_x,
            &result->As_y);

    result->rho_x = result->As_x / (t * 1.0);
    result->rho_y = result->As_y / (t * 1.0);
    result->rho_total_xy = result->rho_x + result->rho_y;

    if (result->rho_y>1.)
    {
        int a=0;
    }

    if (fabs(result->rho_total_xy - result->rho_total) > 1e-10) {
        printf("ERROR: Steel area not preserved after rotation!\n");
    }

// Now display As_x, As_y as "longitudinal" and "hoop" if angle ≈90°, or show both + angle

    return 0;  //all OK
}

////////////////////////////////////
static void correct_pline(char *adp, char *adk)
{  NAGLOWEK *nag;
    LINIA *L;
    LUK *l;
    double Lx1, Ly1, Lx2, Ly2;
    double lx1, ly1, lx2, ly2;
    double dx1, dy1, dx2, dy2;
    double dx, dy, d1x, d1y;
    int last_obj;
    ////searching lines and arcs
    last_obj=0;
    obiekt_tok((char *) adp, adk, (char **) &nag, ONieOkreslony);
    while (nag != NULL) {
        switch (nag->obiekt)
        {
            case Olinia:
                L=(LINIA*)nag;
                Lx1=L->x1;
                Ly1=L->y1;
                Lx2=L->x2;
                Ly2=L->y2;

                if (last_obj==Oluk)
                {
                    //checking points offset
                    dx=Lx1-lx2;
                    dy=Ly1-ly2;

                    dx1=Lx1-lx1;
                    dy1=Ly1-ly1;

                    if ((dx*dx+dy*dy) < (dx1*dx1+dy1*dy1))
                    {
                        Lx1=(float)lx2;
                        Ly1=(float)ly2;
                    }
                    else
                    {
                        Lx1=(float)lx1;
                        Ly1=(float)ly1;
                    }
                }
                last_obj=Olinia;
                break;
            case Oluk:
                l=(LUK*)nag;
                lx1=l->x+l->r*cos(l->kat1);
                ly1=l->y+l->r*sin(l->kat1);
                lx2=l->x+l->r*cos(l->kat2);
                ly2=l->y+l->r*sin(l->kat2);
                if (last_obj==Olinia)
                {
                    //checking points offset
                    dx=lx1-Lx2;
                    dy=ly1-Ly2;

                    dx1=lx2-Lx2;
                    dy1=ly2-Ly2;

                    if ((dx*dx+dy*dy) < (dx1*dx1+dy1*dy1))
                    {
                        Lx2=(float)lx1;
                        Ly2=(float)ly1;
                    }
                    else
                    {
                        Lx2=(float)lx2;
                        Ly2=(float)ly2;
                    }
                }
                last_obj=Oluk;
                break;
            case OdBLOK :
                break;
            default:
                break;
        }
        obiekt_tok(NULL, adk, (char **) &nag, ONieOkreslony);
    }
}

static int qsort_by_val(const void *e1, const void *e2)
{ int delta;
    if (((*(SHIELD_PROPERTY *)e1).ps==3) && ((*(SHIELD_PROPERTY *)e2).ps==3))
    delta=((*(SHIELD_PROPERTY *)e1).pn - (*(SHIELD_PROPERTY *)e2).pn);
    else
    {
        if ((*(SHIELD_PROPERTY *)e1).ps==-1) delta=1;
        else delta=-1;
    }
    return delta;
}

// Function to calculate the area of a triangle given its vertex coordinates
static double calculateTriangleArea(double x1, double y1, double x2, double y2, double x3, double y3) {
    double area = 0.5 * fabs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
    return area;
}

static double nofabs(double value) {
    return value;
}

static int getSignFloat(double num) {
    if (num == 0.0) {
        return 0;
    } else if (signbit(num)) {
        return -1;
    } else {
        return 1;
    }
}

double load_correction(double R_raw, double sum_R_raw, double F_applied, double total_abs_R)
{
    double residual = F_applied - sum_R_raw;

    // ONLY scale if there is a REAL net applied load in this direction
    if (fabs(F_applied) > 1e-6 && fabs(sum_R_raw) > 1e-6) {
        return R_raw * F_applied / sum_R_raw;  // pure pro-rata scaling
    }
    else {
        // self-equilibrium case — fix only numerical error pro-rata by magnitude
        double weight = total_abs_R > 1e-6 ? fabs(R_raw) / total_abs_R : 0.0;
        return R_raw + residual * weight;
    }
}

void split_sub_vector(
        double x1, double y1, double x2, double y2,  // Main vector endpoints
        double x01, double y01, double x02, double y02,  // Sub-vector endpoints
        double px, double py,  // Splitting point
        double Q1, double Q2,  // Q values at sub-vector ends
        // Output parameters for first part
        double *x011, double *y011, double *x012, double *y012, double *Q_start1, double *Q_end1,
        // Output parameters for second part
        double *x021, double *y021, double *x022, double *y022, double *Q_start2, double *Q_end2
) {
    double Dx = x2 - x1;
    double Dy = y2 - y1;
    double denom = Dx * Dx + Dy * Dy;

    if (denom == 0.0) {
        // Degenerate main vector (point), set all to NAN
        *x011 = *y011 = *x012 = *y012 = *Q_start1 = *Q_end1 = NAN;
        *x021 = *y021 = *x022 = *y022 = *Q_start2 = *Q_end2 = NAN;
        return;
    }

    // Compute parameters t for sub-start, sub-end, and split point
    double t01 = ((x01 - x1) * Dx + (y01 - y1) * Dy) / denom;
    double t02 = ((x02 - x1) * Dx + (y02 - y1) * Dy) / denom;
    double tp = ((px - x1) * Dx + (py - y1) * Dy) / denom;

    // Assume points are collinear and 0 <= t01 <= t02 <= 1, 0 <= tp <= 1
    // No checks for collinearity or bounds here; add if needed

    if (tp <= t01) {
        // Sub-vector entirely in second main part; first part empty
        *x011 = *y011 = *x012 = *y012 = *Q_start1 = *Q_end1 = NAN;
        *x021 = x01;
        *y021 = y01;
        *x022 = x02;
        *y022 = y02;
        *Q_start2 = Q1;
        *Q_end2 = Q2;
    } else if (tp >= t02) {
        // Sub-vector entirely in first main part; second part empty
        *x011 = x01;
        *y011 = y01;
        *x012 = x02;
        *y012 = y02;
        *Q_start1 = Q1;
        *Q_end1 = Q2;
        *x021 = *y021 = *x022 = *y022 = *Q_start2 = *Q_end2 = NAN;
    } else {
        // Split occurs within sub-vector
        double s = (tp - t01) / (t02 - t01);
        double Q_mid = Q1 + s * (Q2 - Q1);

        // First part: from sub-start to split
        *x011 = x01;
        *y011 = y01;
        *x012 = px;
        *y012 = py;
        *Q_start1 = Q1;
        *Q_end1 = Q_mid;

        // Second part: from split to sub-end
        *x021 = px;
        *y021 = py;
        *x022 = x02;
        *y022 = y02;
        *Q_start2 = Q_mid;
        *Q_end2 = Q2;
    }
}

/* ---------------------------------------------------------
   Split a linearly distributed 2-component load (Qx, Qy)
   that lies on the main line segment <x1,y1> - <x2,y2>
   by the point <px,py> that also lies on the main segment.
   The BEST option
   --------------------------------------------------------- */
void split_vector_load_2D(
        /* Main segment */
        double x1, double y1, double x2, double y2,

        /* Inner load segment (completely inside or equal to main) */
        double x01, double y01,          // start of load segment
        double x02, double y02,          // end   of load segment

        /* Load values at the two ends of the inner segment */
        double Q1x, double Q1y,          // Q at (x01,y01)
        double Q2x, double Q2y,          // Q at (x02,y02)

        /* Splitting point on the main segment */
        double px, double py,

        /* ----- OUTPUT -------------------------------------------------
           First part  (from load-start to split point)
           Second part (from split point to load-end)
           If a part does not exist (split point outside the load segment)
           the corresponding coordinates are set to NAN.
           ------------------------------------------------------------- */
        double *x11, double *y11, double *Qx11, double *Qy11,   // start of part 1 (= original start)
        double *x12, double *y12, double *Qx12, double *Qy12,   // end   of part 1 (= split point)

        double *x21, double *y21, double *Qx21, double *Qy21,   // start of part 2 (= split point)
        double *x22, double *y22, double *Qx22, double *Qy22    // end   of part 2 (= original end)
) {
    double Dx = x2 - x1;
    double Dy = y2 - y1;
    double denom = Dx*Dx + Dy*Dy;

    /* Degenerate main segment -> everything invalid */
    if (denom == 0.0) {
        *x11 = *y11 = *x12 = *y12 = *x21 = *y21 = *x22 = *y22 = NAN;
        *Qx11 = *Qy11 = *Qx12 = *Qy12 = *Qx21 = *Qy21 = *Qx22 = *Qy22 = NAN;
        return;
    }

    /* Parameter t along the main segment (t = 0 at (x1,y1), t = 1 at (x2,y2)) */
    double t01 = ((x01 - x1)*Dx + (y01 - y1)*Dy) / denom;
    double t02 = ((x02 - x1)*Dx + (y02 - y1)*Dy) / denom;
    double tp  = ((px  - x1)*Dx + (py  - y1)*Dy) / denom;

    /* Ensure correct ordering (in case user gave points backwards) */
    if (t01 > t02) {
        /* swap */
        double tmp_t = t01; t01 = t02; t02 = tmp_t;
        double tmp_x = x01; x01 = x02; x02 = tmp_x;
        double tmp_y = y01; y01 = y02; y02 = tmp_y;
        double tmp_Qx = Q1x; Q1x = Q2x; Q2x = tmp_Qx;
        double tmp_Qy = Q1y; Q1y = Q2y; Q2y = tmp_Qy;
    }

    /* ------------------------------------------------------------------
       Three possible situations
       ------------------------------------------------------------------ */
    if (tp <= t01) {
        /* Split point is before the whole load -> only second part exists */
        *x11 = *y11 = *x12 = *y12 = NAN;
        *Qx11 = *Qy11 = *Qx12 = *Qy12 = NAN;

        *x21 = x01;  *y21 = y01;  *Qx21 = Q1x;  *Qy21 = Q1y;
        *x22 = x02;  *y22 = y02;  *Qx22 = Q2x;  *Qy22 = Q2y;
    }
    else if (tp >= t02) {
        /* Split point is after the whole load -> only first part exists */
        *x21 = *y21 = *x22 = *y22 = NAN;
        *Qx21 = *Qy21 = *Qx22 = *Qy22 = NAN;

        *x11 = x01;  *y11 = y01;  *Qx11 = Q1x;  *Qy11 = Q1y;
        *x12 = x02;  *y12 = y02;  *Qx12 = Q2x;  *Qy12 = Q2y;
    }
    else {
        /* Real split inside the load segment */
        double s = (tp - t01) / (t02 - t01);               // parameter along the load segment [0,1]

        double Qmid_x = Q1x + s * (Q2x - Q1x);
        double Qmid_y = Q1y + s * (Q2y - Q1y);

        /* ---- First part ---- */
        *x11 = x01;   *y11 = y01;   *Qx11 = Q1x;     *Qy11 = Q1y;
        *x12 = px;    *y12 = py;    *Qx12 = Qmid_x;  *Qy12 = Qmid_y;

        /* ---- Second part ---- */
        *x21 = px;    *y21 = py;    *Qx21 = Qmid_x;  *Qy21 = Qmid_y;
        *x22 = x02;   *y22 = y02;   *Qx22 = Q2x;     *Qy22 = Q2y;
    }
}

//////////////////////
void insert_sh_edge(int index, SH_EDGE *sh_edge_buf, double x1, double y1, double x2, double y2, double splitpx, double splitpy, BOOL split_load)
{   LINIA L;

    double x11, y11, Qx11, Qy11;   // start of part 1 (= original start)
    double x12, y12, Qx12, Qy12;   // end   of part 1 (= split point)
    double x21, y21, Qx21, Qy21;   // start of part 2 (= split point)
    double x22, y22, Qx22, Qy22;   // end   of part 2 (= original end)

    //shifting all sh_edge[i] starting from index by 1 (index-1 stays, it was already modified)
    //inserting sh_edge_buf as index element
    //shifing .first_edge and .last_edge of all surfaces if they are bigger than index

    add_edge_sh();  //makeing space

    for (int i=sh_edge_no-2; i>=index; i--)   //we have 100 elements, now make space for 101 so sh_edge_no=101, so we starting from i=99 to mode to i=100
        memmove(&sh_edge[i+1], &sh_edge[i], sizeof(SH_EDGE));   //99 int 100
    //embedding buffer
    memmove(&sh_edge[index], sh_edge_buf, sizeof(SH_EDGE));

    //shifting .first_edge and .last_edge
    for (int j=0; j<shield_no; j++)
    {
        if (shield_property[j].first_edge>index) shield_property[j].first_edge++;
        if (shield_property[j].last_edge>index) shield_property[j].last_edge++;
    }
    for (int j=0; j<hole_no; j++)
    {
        if (hole_property[j].first_edge>index) hole_property[j].first_edge++;
        if (hole_property[j].last_edge>index) hole_property[j].last_edge++;
    }
    for (int j=0; j<wall_no; j++)
    {
        if (wall_property[j].first_edge>index) wall_property[j].first_edge++;
        if (wall_property[j].last_edge>index) wall_property[j].last_edge++;
    }
    for (int j=0; j<zone_no; j++)
    {
        if (zone_property[j].first_edge>index) zone_property[j].first_edge++;
        if (zone_property[j].last_edge>index) zone_property[j].last_edge++;
    }

    //load
    for (int j=0; j<sh_load_no; j++)
    {
        if (sh_load[j].k>=index) sh_load[j].k++;
    }


    if (split_load) {
        for (int j = 0; j < sh_load_no; j++) {
            if ((sh_load[j].k == (index - 1)) && (sh_load[j].style>9) && (sh_load[j].style<15))  //split between sh_edge[index-1] and sh_edge[index]
            {
                //adding new load by copying
                memmove(&sh_load[sh_load_no], &sh_load[j], sizeof(SH_LOAD));

                split_vector_load_2D(x1, y1, x2, y2, sh_load[j].x1, sh_load[j].y1, sh_load[j].x2, sh_load[j].y2,
                                     sh_load[j].magnitude1x, sh_load[j].magnitude1y, sh_load[j].magnitude2x, sh_load[j].magnitude2y,
                                     splitpx, splitpy,
                                     &x11, &y11, &Qx11, &Qy11,   // start of part 1 (= original start)
                                     &x12, &y12, &Qx12, &Qy12,   // end   of part 1 (= split point)
                                     &x21, &y21, &Qx21, &Qy21,   // start of part 2 (= split point)
                                     &x22, &y22, &Qx22, &Qy22);    // end   of part 2 (= original end)

                // Print results
                if (isnan(x11))
                {
#ifndef NDEBUG
                    printf("Part 1 is empty.\n");
#endif
                }
                else
                {
#ifndef NDEBUG
                   printf("Part 1: (%.3f,%.3f) -- Q=(%.3f,%.3f)  →  (%.3f,%.3f) -- Q=(%.3f,%.3f)\n", x11,y11, Qx11,Qy11, x12,y12, Qx12,Qy12);
#endif
                    sh_load[j].x1=(float)x11;
                    sh_load[j].y1=(float)y11;
                    sh_load[j].x2=(float)x12;
                    sh_load[j].y2=(float)y12;

                    sh_load[j].magnitude1x=Qx11;
                    sh_load[j].magnitude1y=Qy11;
                    sh_load[j].magnitude2x=Qx12;
                    sh_load[j].magnitude2y=Qy12;

                    sh_load[j].k=(index - 1); //stays

                }
                if (isnan(x21))
                {
#ifndef NDEBUG
                    printf("Part 2 is empty.\n");
#endif
                }
                else
                {
#ifndef NDEBUG
                   printf("Part 2: (%.3f,%.3f) -- Q=(%.3f,%.3f)  →  (%.3f,%.3f) -- Q=(%.3f,%.3f)\n", x21,y21, Qx21,Qy21, x22,y22, Qx22,Qy22);
#endif
                    //new load
                    sh_load[sh_load_no].x1=(float)x21;
                    sh_load[sh_load_no].y1=(float)y21;
                    sh_load[sh_load_no].x2=(float)x22;
                    sh_load[sh_load_no].y2=(float)y22;

                    sh_load[sh_load_no].magnitude1x=Qx21;
                    sh_load[sh_load_no].magnitude1y=Qy21;
                    sh_load[sh_load_no].magnitude2x=Qx22;
                    sh_load[sh_load_no].magnitude2y=Qy22;

                    sh_load[j].k=index; //inserted

                    add_load_sh();
                }
                break;
            }
        }
    }

}

void Shield_analysis(void) {

    int i, j, k, ii, li=0;
    int ret, ret_standard, ret_exp;
    BOOL ret1, ret2;
    double units_factor = 1.0;
    double m_units_factor = 1000.0;
    double geo_units_factor = 1000.0;
    //double E_units_factor=1000000.0;
    //double h_units_factor=0.001;
    char st_title[MaxTextLen * 2];
    char title_id[MaxTextLen];
    char *ptr_id, *ptr_id_short;
    NAGLOWEK *nag;
    AVECTOR *v;
    TEXT *t;
    LINIA *L;
    LUK *l;
    BLOK *b;
    char *ptr, *ptrs, *ptrsy, *ptr1;
    double gZ = 0;
    double gamma_l = 1.0, gamma_li = 1.0, xi_l = 1.0;
    int combinations_number, combination_no;
    GAMMAS gammas_EU = {1.35, 0, 0, 0, 0.85, 1.0};
    GAMMAS gammas_ASCE = {1.4, 0.9, 0.6, 0, 1.2, 0};
    GAMMAS gammas_ICC = {1.4, 0.9, 1.0, 0.6, 1.2, 0};
    GAMMAS *gammas;
    double axis_increment = 0.25;
    char prop[MAXEXT];
    int body_number;
    BOOL is_closed;
    int first, last, property_number;
    int check;
    char report[MaxMultitextLen] = "";
    char report_row[MaxTextLen];
    char program[MAXPATH];
    char params[MAXPATH];
#ifdef LINUX
    char rn[3] = "\n";
#else
    char rn[3] = "\r\n";
#endif
    FILE *f, *f1;
    BOOL no_error = TRUE;
    char parf[4][MaxMultitextLen] = {"", "","", ""};
    char par[16][MaxTextLen] = {"", "", "","", "", "","", "", "", "","", "", "","", "", ""};
    char par_l[2][MaxMultitextLen] = {"", ""};
    int sif_body;
    int sif_material;
    int sif_body_force;
    int sif_body_zone[100];
    int sif_body_shield[100];
    int sif_boundary_condition;
    int sif_boundary_condition_simple;
    int sif_boundary_condition_fixed;
    int sif_boundary_condition_roll_x;  //can roll along x axis, restrained in y axis
    int sif_boundary_condition_roll_y;  //can roll along y axis, restrained in x axis
    int this_property;
    double df_x1, df_y1, df_x2, df_y2;
    char *ADPB, *ADKB;
    DWORD runcode;
    int runcode_short;
    char *data_row;
    char data_row_[2][MaxTextLen];
    int i_row = 0;
    //char data_row2[MaxTextLen];
    char *line;
    int mesh_nodes_no, mesh_elements_no, mesh_boundaries_no, total_mesh_boundaries_no, mesh_101_boundaries_no, max_element_nodes, mesh_202_boundaries_no, mesh_303_boundaries_no, flag101_no, flag202_no, flag303_no;
    double mesh_node_z;
    double koc, kos;
    char desired_layer[maxlen_w+2];
    char desired_layer_upper[maxlen_w+2];
    char layer_name_upper[maxlen_w+2];
    char desired_layer_bak[maxlen_w+2];
    int desired_layer_no, desired_reactions_layer_no;
    char block_name[MaxTextLen];
    char *ptr_block;
    POINTD blok_origin;
    float x1, y1, x2, y2;
    char *Perm;
    char *use_previous;
    int perm_d_node_no, perm_d_index_no, perm_s_node_no, perm_s_index_no, perm_e_node_no, perm_e_index_no;
    //PERM *perm_d=NULL;
    //PERM *perm_s=NULL;
    //PERM *perm_e=NULL;
    int *perm_d = NULL;
    int *perm_s = NULL;
    int *perm_s_bak = NULL;
    int *perm_e = NULL;
    int perm_d_b = 0;
    int perm_s_b = 0;
    int perm_e_b = 0;
    double *deflection[DEFLECTION_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL};
    double *min_deflection[DEFLECTION_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL};
    double *max_deflection[DEFLECTION_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL};
    double *stress[STRESS_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    double *min_stress[STRESS_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    double *max_stress[STRESS_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    double *min_stress_bak[STRESS_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    double *max_stress_bak[STRESS_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    double *epsilon[EPSILON_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL};
    double *epsilon1[EPSILON_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL};
    double *min_epsilon[EPSILON_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL};
    double *max_epsilon[EPSILON_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL};
    int node_number, index_number;
    double deflection_min[DEFLECTION_NUMBER] = {0, 0, 0, 0, 0, 0}, deflection_max[DEFLECTION_NUMBER] = {0, 0, 0, 0, 0,
                                                                                                        0};
    double min_deflection_min[DEFLECTION_NUMBER] = {0, 0, 0, 0, 0, 0}, max_deflection_max[DEFLECTION_NUMBER] = {0, 0, 0,
                                                                                                                0, 0,
                                                                                                                0};
    double stress_min[STRESS_NUMBER] = {0, 0, 0, 0, 0, 0, 0}, stress_max[STRESS_NUMBER] = {0, 0, 0, 0, 0, 0, 0};
    double min_stress_min[STRESS_NUMBER] = {0, 0, 0, 0, 0, 0, 0}, max_stress_max[STRESS_NUMBER] = {0, 0, 0, 0, 0, 0, 0};
    double min_stress_min_bak[STRESS_NUMBER] = {0, 0, 0, 0, 0, 0, 0}, max_stress_max_bak[STRESS_NUMBER] = {0, 0, 0, 0, 0, 0, 0};
    double epsilon_min[EPSILON_NUMBER] = {0, 0, 0, 0, 0, 0}, epsilon_max[EPSILON_NUMBER] = {0, 0, 0, 0, 0, 0};
    double min_epsilon_min[EPSILON_NUMBER] = {0, 0, 0, 0, 0, 0}, max_epsilon_max[EPSILON_NUMBER] = {0, 0, 0, 0, 0, 0};
    double deflection_amax, stress_amax, epsilon_amax;
    double stress_min_RC, stress_max_RC, stress_min_RC_rho_l, stress_max_RC_rho_l, stress_min_RC_rho_c, stress_max_RC_rho_c, ps;
    double stress_min_RC_rho_x, stress_max_RC_rho_x, stress_min_RC_rho_y, stress_max_RC_rho_y;
    BOOL RC_flag_exists=FALSE;
    char deflection_chapter[DEFLECTION_NUMBER][MaxTextLen];
    char stress_chapter[STRESS_NUMBER][MaxTextLen];
    char epsilon_chapter[EPSILON_NUMBER][MaxTextLen];
    WIELOKAT w = Stdef;
    GRADIENT3 gradient3;
    GRADIENT4 gradient4;
    FE_DATA3 fe_data3;
    FE_DATA4 fe_data4;
    FE_DATA3_EX fe_data3_ex, fe_data3_ex1, fe_data3_ex2, fe_data3_ex3, fe_data3_ex4;
    char *fe_data_ptr;
    char *fe_data_ex_ptr, *fe_data_ex1_ptr, *fe_data_ex2_ptr, *fe_data_ex3_ptr, *fe_data_ex4_ptr;
    char *gradient_ptr;
    char *translucency_ptr;
    unsigned char HalfTranslucency = 128;
    char *adr;
    TEXT T=Tdef, Tcb=Tdef;
    LINIA Lt;
    PLINIA PL;
    double psize;
    BOOL hiding;
    char load_formula0[MaxTextLen];
    char load_formula[MaxMultitextLen];
    char all_load_formula[MaxMultitextLen];
    char all_formulas[2][MaxMultitextLen];
    ////TEMPORARY
    char boundary_load_formula0[MaxTextLen];
    char boundary_load_formula[MaxMultitextLen];
    char boundary_all_load_formula[MaxMultitextLen];
    char boundary_all_formulas[2][MaxMultitextLen];
    ////
    char *all_formula, *all_formula_bak;
    int current = 0;
    int block_names_no = 0;
    int key1;
    double self_weight = 0.0;

    int geometric_tiffness = 0;
    int inertia = 0;
    int st_dynamic_no = 0;
    BOOL PINNABLE = TRUE;
    char SLS_ULS[12] = "";
    BOOL was_refreshed;
    BOOL go_ahead;
    int deflection_ini, stress_ini, epsilon_ini;
    int file_no;
    int sti_no;
    int nom_max;
    char *min_max;
    char _min_[]="_min";
    char _max_[]="_max";
    char _nope_[]="";
    int bim, bi;
    double deflection_min_, deflection_max_, stress_min_, stress_max_;
    int *body_property=NULL;
    ST_PROPERTY *body_prop;
    int body_prop_no;
    BOOL ULSLC_flag[5];

    float (*jednostkiObX)(double mob);
    float (*jednostkiObY)(double mob);

    ////ST_UNIFORM_LOAD *st_uniform_load_comb=NULL, *st_uniform_load_cons=NULL;

    double Rxmin=0.0;
    double Rxmax=0.0;
    double Rxmin_back=0.0;
    double Rxmax_back=0.0;
    double Rymin=0.0;
    double Rymax=0.0;
    double Rymin_back=0.0;
    double Rymax_back=0.0;

    double Rxabsmin=0.0;
    double Rxabsmax=0.0;
    double Rxabsmin_back=0.0;
    double Rxabsmax_back=0.0;
    double Ryabsmin=0.0;
    double Ryabsmax=0.0;
    double Ryabsmin_back=0.0;
    double Ryabsmax_back=0.0;
    //normal
    double Rnmin=0.0;
    double Rnmax=0.0;
    double Rnmin_back=0.0;
    double Rnmax_back=0.0;
    //tangential
    double Rtmin=0.0;
    double Rtmax=0.0;
    double Rtmin_back=0.0;
    double Rtmax_back=0.0;

    double area;
    double *body_area=NULL;
    double *surface_load[64]={NULL};  //there is no more than 64 cases
    double point_load_x[64], point_load_y[64];
    double distributed_load_x[64], distributed_load_y[64];
    int case_files[5];
    double total_load_x, total_load_y;
    double load_ratio_x, load_ratio_x_d, load_ratio_y, load_ratio_y_d, load_ratio_n, load_ratio_n_d, load_ratio_t, load_ratio_t_d;
    POINTD point_b, point_c;
    double NSEW_dist_div=4.;

    was_refreshed = FALSE;

    sh_node_no = 0;
    sh_node_emb_no = 0;
    sh_node_emb_edge_no = 0;
    sh_edge_no = 0;
    sh_edge_emb_no = 0;
    sh_load_no = 0;
    shield_no = 0;
    hole_no = 0;
    wall_no = 0;
    zone_no = 0;

    SH_PROPERTY_MAX = 10;
    SH_LOAD_FACTORS_MAX = 100;
    SH_NODE_MAX = 100;
    SH_NODE_EMB_MAX = 10;
    SH_NODE_EMB_EDGE_MAX = 10;
    SH_EDGE_MAX = 100;
    SH_EDGE_EMB_MAX = 10;
    SH_LOAD_MAX = 100;
    SH_SHIELD_MAX = 10;
    SH_HOLE_MAX = 10;
    SH_WALL_MAX = 10;
    SH_ZONE_MAX = 10;

    shield_no = hole_no = wall_no = zone_no = sh_load_no = 0;

    sh_property_no = 0;
    sh_load_factors_no = 0;
    gZ = 0.0;

    redcrsb(0, 172);
    //select_blok();
    select_blok_items(Blinia | Bluk | Bvector | BtekstNoType15);  //for shield  //Btext
    redcrsb(1, 171);

    if ((ADP == NULL) && (ADK == NULL)) return;

    //ret_standard = 1;   //TEMPORARY, for EU

    ClearErr();
    ClearInfo();

    ret_standard = ask_question_static(5, (char *) _No_, (char *) _Yes_, (char *) "", (char *) _PROCEED_SHIELD_FEM_, 12,
                                       (char *) "", 11, 1, 0, &combination_no, &geometric_tiffness, &inertia,
                                       &st_dynamic_no, &PINNABLE, &theta_, &sigma_eq_, &epsilon_);
    //0 - rezygnuj; 1 - Eurocode, 2 - ASCE, 3 - ICC
    if (ret_standard > 0) key1 = _YES_;
    else if (ret_standard == 0) key1 = _NO_;
    else key1 = _NO_;

    sigma_eq_=1;

    if (key1 != _YES_ && key1 != _yes_) {
        zmien_atrybut_undo(dane, dane + dane_size);
        blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
        zmien_atrybut(ADP, ADK, Ablok, Aoblok);
        redcrsb(2, 167);
        return;
    }

    glb_silent=TRUE;

    komunikat_str_short(_PREPARING_DATA_, FALSE, TRUE);

    // Allocate memory for MAX_L_BLOCKS char pointers (for the array of strings)
    block_names = (char **)malloc(MAX_L_BLOCKS * sizeof(char *));

    // Allocate memory for each individual string
    for (i = 0; i < MAX_L_BLOCKS; i++)
    {
        block_names[i] = (char *)malloc((MAX_L_BLOCKS_LEN + 2) * sizeof(char));
        if (block_names[i] == NULL) {
            perror("Failed to allocate memory for a string");
            // Free previously allocated memory before exiting
            for (int jk = 0; jk < i; jk++) {
                free(block_names[jk]);
            }
            free(block_names);
            block_names=NULL;
        }
        // Optional: Initialize the strings (e.g., to empty strings)
        block_names[i][0] = '\0';
    }


    sh_property = (ST_PROPERTY *) malloc(SH_PROPERTY_MAX * sizeof(ST_PROPERTY) + M100);
    sh_load_factors = (ST_LOAD_FACTORS *) malloc(SH_LOAD_FACTORS_MAX * sizeof(ST_LOAD_FACTORS) + M100);
    sh_node = (SH_NODE *) malloc(SH_NODE_MAX * sizeof(SH_NODE) + M100);
    sh_node_emb = (SH_NODE_EMB *) malloc(SH_NODE_EMB_MAX * sizeof(SH_NODE_EMB) + M100);
    sh_node_emb_edge = (SH_NODE_EMB_EDGE *) malloc(SH_NODE_EMB_EDGE_MAX * sizeof(SH_NODE_EMB_EDGE) + M100);
    sh_edge = (SH_EDGE *) malloc(SH_EDGE_MAX * sizeof(SH_EDGE) + M100);
    sh_edge_emb = (SH_EDGE *) malloc(SH_EDGE_EMB_MAX * sizeof(SH_EDGE) + M100);
    sh_load = (SH_LOAD *) malloc(SH_LOAD_MAX * sizeof(SH_LOAD) + M100);

    shield_property = (SHIELD_PROPERTY *) malloc(SH_SHIELD_MAX * sizeof(SHIELD_PROPERTY) + M100);
    hole_property = (SHIELD_PROPERTY *) malloc(SH_HOLE_MAX * sizeof(SHIELD_PROPERTY) + M100);
    wall_property = (SHIELD_PROPERTY *) malloc(SH_WALL_MAX * sizeof(SHIELD_PROPERTY) + M100);
    zone_property = (SHIELD_PROPERTY *) malloc(SH_ZONE_MAX * sizeof(SHIELD_PROPERTY) + M100);

    sh_load_factors = (ST_LOAD_FACTORS *) malloc(SH_LOAD_FACTORS_MAX * sizeof(ST_LOAD_FACTORS) + M100);


    switch (ret_standard) {
        case 1:
            load_factors = st_load_factors_EU_0;
            gammas = &gammas_EU;
            break;
        case 2:
            load_factors = st_load_factors_ASCE_0;
            gammas = &gammas_ASCE;
            break;
        case 3:
            load_factors = st_load_factors_ICC_0;
            gammas = &gammas_ICC;
            break;
        default:
            load_factors = st_load_factors_EU_0;
            gammas = &gammas_EU;
            break;
    }

    memmove(&sh_load_factors[sh_load_factors_no], &load_factors[0], sizeof(ST_LOAD_FACTORS));  //TEMPORARY for EUROCODE


    ////properties
    /////////////////////////
    if (Jednostki == 1)   //mm
    {
        units_factor = 1.0;  //ISO
        m_units_factor = 1000.0;
        geo_units_factor = 0.001;
        //E_units_factor=100000.0;
        //h_units_factor=0.001;
        axis_increment = 0.25;
        unit_factors_sh = &unit_factors_sh_si;
        prop_precisions_sh = &SI_precisions;
        UNITS = SI;
    } else if (Jednostki == 10) //cm
    {
        units_factor = 10.0;
        m_units_factor = 100.0;
        geo_units_factor = 0.01;
        //E_units_factor=100000.0;
        //h_units_factor=0.001;
        axis_increment = 0.25;
        unit_factors_sh = &unit_factors_sh_si;
        prop_precisions_sh = &SI_precisions;
        UNITS = SI;
    } else if (Jednostki == 1000)  //m
    {
        units_factor = 1000.0;
        m_units_factor = 1.0;
        geo_units_factor = 1.0;
        //E_units_factor=100000.0;
        //h_units_factor=0.001;
        axis_increment = 0.25;
        unit_factors_sh = &unit_factors_sh_si;
        prop_precisions_sh = &SI_precisions;
        UNITS = SI;
    } else if (Jednostki == 1000000) //km
    {
        units_factor = 1000000.0;
        m_units_factor = 0.001;
        geo_units_factor = 1000;
        //E_units_factor=100000.0;
        //h_units_factor=0.001;
        axis_increment = 0.25;
        unit_factors_sh = &unit_factors_sh_si;
        prop_precisions_sh = &SI_precisions;
        UNITS = SI;
    } else if (Jednostki == 25.4) //"
    {
        units_factor = 1.0;  //imperial
        m_units_factor = 1.0;
        geo_units_factor = 1.0;
        //E_units_factor=1.0;
        //h_units_factor=1.0;
        axis_increment = 10.0;
        unit_factors_sh = &unit_factors_sh_imp;
        prop_precisions_sh = &IMP_precisions;
        UNITS = IMP;
    } else if (Jednostki == 304.8) //'
    {
        units_factor = 12.0;  //inti inches
        m_units_factor = 1.0;
        geo_units_factor = 12.0;
        //E_units_factor=1.0;
        //h_units_factor=1.0;
        axis_increment = 10.0;
        unit_factors_sh = &unit_factors_sh_imp;
        prop_precisions_sh = &IMP_precisions;
        UNITS = IMP;
    } else if (Jednostki == 914.4) //yd
    {
        units_factor = 36.0;  //into inches
        m_units_factor = 1.0;
        geo_units_factor = 36.0;
        //E_units_factor=1.0;
        //h_units_factor=1.0;
        axis_increment = 10.0;
        unit_factors_sh = &unit_factors_sh_imp;
        prop_precisions_sh = &IMP_precisions;
        UNITS = IMP;
    } else if (Jednostki == 1609344) //mi
    {
        units_factor = 63360.0;  //into inches
        m_units_factor = 1.0;
        geo_units_factor = 63360.0;  //into inches
        //E_units_factor=1.0;
        //h_units_factor=1.0;
        axis_increment = 10.0;
        unit_factors_sh = &unit_factors_sh_imp;
        prop_precisions_sh = &IMP_precisions;
        UNITS = IMP;
    } else {
        units_factor = 1.0;
        m_units_factor = 1000.0;
        geo_units_factor = 0.001;
        //E_units_factor=1.0;
        //h_units_factor=1.0;
        axis_increment = 10.0;
        unit_factors_sh = &unit_factors_sh_si;
        prop_precisions_sh = &SI_precisions;
        UNITS = SI;
    }

    if (UNITS!=SI) dxl_min=4.0f;  //inches
    else dxl_min=0.1f; //meters

    strcpy(title_id, "SHIELD");
    ptr_id = title_id;
    ptr_id_short = (char *) "";

    ////properties
    ////////////////
    obiekt_tok((char *) ADP, ADK, (char **) &nag, Otekst);
    while (nag != NULL)
    {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            t = (TEXT *) nag;
            while (1) {
                ptr = strstr(t->text, _SHIELD_);  //title
                if (ptr == NULL) ptr = strstr(t->text, _SHIELD_PL);  //title
                if (ptr == NULL) ptr = strstr(t->text, _SHIELD_UA);  //title
                if (ptr == NULL) ptr = strstr(t->text, _SHIELD_ES);  //title
                if (ptr != NULL) {
                    strncpy(st_title, ptr + 1, MaxTextLen * 2 - 12);
                    strncpy(title_id, ptr + 1, MaxTextLen - 1);
                    ptr = strchr(title_id, ':');  //title_id
                    if (ptr != NULL) {
                        ptr++;
                        //ptr_id_short = ptr;
                        while (*ptr == ' ') ptr++;
                        ptr_id_short = ptr;
                        if (*ptr != '\0') {
                            while ((*ptr != ' ') && (*ptr != '\0')) ptr++;
                            if (*ptr != '\0') *ptr = '\0';
                        }
                    }
                    break; //end of string
                }

                memmove(&sh_property[sh_property_no], &prt_def_sh, sizeof(ST_PROPERTY));
                //if (prop_precisions_sh == &SI_precisions)
                if (UNITS==SI) sh_property[sh_property_no].c = 0.03; //[m]
                else sh_property[sh_property_no].c = 1.18; //[in]
                //st_property[st_property_no].ok = 0;

                //gravitational acceleration

                //gammas->gamma_g=st_load_factors_EU_0

                ptr = strstr(t->text, "%g=");
                if (ptr == NULL) ptr = strstr(t->text, "%G=");
                if (ptr != NULL) {
                    gZ = atof(ptr + 3);

                    //if (UNITS!=SI) gZ=1.0;  //Self-Weight (lb/in²) = Density (lb/in³) × Thickness (in)  in IMP  this is abandoned due to representing mass density divided by g

                    ptr1 = strstr(t->text, u8"γ=");
                    if (ptr1 == NULL) ptr1 = strstr(t->text, "Γ=");  //Gamma
                    if (ptr1 != NULL) {
                        gammas->gamma_g = atof(ptr1 + 3);
                    }

                    ptr1 = strstr(t->text, "Ψ0=");  //Psi
                    if (ptr1 == NULL) ptr1 = strstr(t->text, "ψ0=");  //psi
                    if (ptr1 != NULL) {
                        gammas->psi0 = atof(ptr1 + 4);
                    }

                    ptr1 = strstr(t->text, "Ψ1=");  //Psi1
                    if (ptr1 == NULL) ptr1 = strstr(t->text, "ψ1=");  //psi1
                    if (ptr1 != NULL) {
                        gammas->psi1 = atof(ptr1 + 4);
                    }

                    ptr1 = strstr(t->text, "Ψ2=");  //Psi2
                    if (ptr1 == NULL) ptr1 = strstr(t->text, "ψ2=");  //psi2
                    if (ptr1 != NULL) {
                        gammas->psi2 = atof(ptr1 + 4);
                    }

                    ptr1 = strstr(t->text, u8"ξ=");
                    if (ptr1 == NULL) ptr1 = strstr(t->text, "Ξ=");  //Xi
                    if (ptr1 != NULL) {
                        gammas->xi_g = atof(ptr1 + 3);
                    }

                    ptr1 = strstr(t->text, u8"γi=");
                    if (ptr1 == NULL) ptr1 = strstr(t->text, "Γi=");  //Gamma inf
                    if (ptr1 != NULL) {
                        gammas->gamma_gi = atof(ptr1 + 4);
                    }

                    break; //end of string
                }

                //default element length increment
                ptr = strstr(t->text, "%dx=");
                if (ptr == NULL) ptr = strstr(t->text, "%Dx=");
                if (ptr != NULL) {
                    double Dx = atof(ptr + 4);
                    axis_increment = fabs(Dx); //default 250 mm or 10 inch
                    break; //end of string
                }

                //load factors for load characters

                ptr = strstr(t->text, "%DL");  //Dead Load
                if (ptr == NULL) ptr = strstr(t->text, "%LL");  //Live Load
                if (ptr == NULL) ptr = strstr(t->text, "%LL");  //Live Load
                if (ptr == NULL) ptr = strstr(t->text, "%Lr");  //Rive roof Load
                if (ptr == NULL) ptr = strstr(t->text, "%WL");  //Wind Load
                if (ptr == NULL) ptr = strstr(t->text, "%SL");  //Snow Load
                if (ptr == NULL) ptr = strstr(t->text, "%EL");  //Earthquake Load
                if (ptr == NULL) ptr = strstr(t->text, "%RL");  //Rainwater Load or ice water load
                if (ptr == NULL) ptr = strstr(t->text, "%HL");  //Hydraulic Load from soil
                if (ptr == NULL) ptr = strstr(t->text, "%FL");  //hydraulic Load from Fluids
                if (ptr == NULL) ptr = strstr(t->text, "%TL");  //Thermal Load
                if (ptr != NULL) {
                    sh_load_factors[sh_load_factors_no].load = str2load(ptr);

                    //"", LL", "LL", "Lr", "WL", "SL", "EL", "RL", "HL", "FL", "TL"};
                    memmove(&sh_load_factors[sh_load_factors_no],
                            &load_factors[sh_load_factors[sh_load_factors_no].load],
                            sizeof(ST_LOAD_FACTORS));  //temporary just for Eurocode

                    if (sh_load_factors[sh_load_factors_no].load > 0) {
                        sh_load_factors[sh_load_factors_no].variant = (unsigned char) atoi(ptr + 3);
                        ptr1 = strstr(ptr, "γ=");  //gamma
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "Γ=");  //Gamma
                        if (ptr1 != NULL) sh_load_factors[sh_load_factors_no].gamma = atof(ptr1 + 3);

                        ptr1 = strstr(ptr, "γi=");  //gamma inf
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "Γi=");  //Gamma inf
                        if (ptr1 != NULL) sh_load_factors[sh_load_factors_no].gamma_inf = atof(ptr1 + 4);

                        ptr1 = strstr(ptr, "Ψ0=");  //Psi
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "ψ0=");  //psi
                        if (ptr1 != NULL) sh_load_factors[sh_load_factors_no].psi0 = atof(ptr1 + 4);

                        ptr1 = strstr(ptr, "Ψ1=");  //Psi1
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "ψ1=");  //psi
                        if (ptr1 != NULL) sh_load_factors[sh_load_factors_no].psi1 = atof(ptr1 + 4);

                        ptr1 = strstr(ptr, "Ψ2=");  //Psi2
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "ψ2=");  //psi2
                        if (ptr1 != NULL) sh_load_factors[sh_load_factors_no].psi2 = atof(ptr1 + 4);

                        ptr1 = strstr(ptr, "ξ=");  //xi
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "Ξ=");  //Xi
                        if (ptr1 != NULL) sh_load_factors[sh_load_factors_no].xi = atof(ptr1 + 3);

                        add_load_factors_sh();
                    }
                    break; //end of string
                }

                ptr = strchr(t->text, '#');
                if (ptr == NULL) break;
                ptr1 = strchr(ptr + 1, ' ');
                if (ptr1 == NULL) break;
                strncpy(prop, ptr + 1, ptr1 - ptr - 1);
                prop[ptr1 - ptr - 1] = '\0';
                sh_property[sh_property_no].n = atoi(prop);

                ptr = strstr(t->text, " RC ");  //should be separated with spaces
                if (ptr != NULL)
                    sh_property[sh_property_no].RC_flag = 1; //TEMPORARY: assumption that it's rectangular cross section

                ptr = strstr(t->text, "t=");
                if (ptr == NULL) break;
                sh_property[sh_property_no].h =
                        atof(ptr + 2) * unit_factors_sh->h_f;  //for RC in mm or in, same as steel and wood

                sh_property[sh_property_no].b = sh_property[sh_property_no].h;

                ptr = strstr(t->text, "b=");
                if (ptr != NULL) {
                    sh_property[sh_property_no].b =
                            atof(ptr + 2) * unit_factors_sh->h_f;  //for RC in mm or in, same as steel and wood
                    if (sh_property[sh_property_no].RC_flag == 1) //it's RC and b is set
                    {
                        sh_property[sh_property_no].A = sh_property[sh_property_no].h *
                                                        sh_property[sh_property_no].b; //Cross-sectional area in mm^2
                        sh_property[sh_property_no].Asy = sh_property[sh_property_no].A; //Shear area in the local y-axis
                        sh_property[sh_property_no].Asz = sh_property[sh_property_no].A; //Shear area in the local z-axis
                        sh_property[sh_property_no].Iy =
                                sh_property[sh_property_no].h * pow(sh_property[sh_property_no].b, 3) /
                                12; //Moment of inertia for bending about the local y axis in mm^4  or in^4:  h*b^3/12
                        sh_property[sh_property_no].Iz =
                                pow(sh_property[sh_property_no].h, 3) * sh_property[sh_property_no].b /
                                12; //Moment of inertia for bending about the local z axis in mm^4  or in^4:  h^3*b/12
                        sh_property[sh_property_no].Wy =
                                sh_property[sh_property_no].h * pow(sh_property[sh_property_no].b, 2) *
                                unit_factors_sh->Wmm_f /
                                6; //Elastic section modulus about y-axis in m^3  or in^3:  h*b^2/6
                        sh_property[sh_property_no].Wz =
                                pow(sh_property[sh_property_no].h, 2) * sh_property[sh_property_no].b *
                                unit_factors_sh->Wmm_f /
                                6; //Elastic section modulus about z-axis  in m^3 or in^3:  h^2*b/6
                        sh_property[sh_property_no].Jx =
                                ((sh_property[sh_property_no].h * sh_property[sh_property_no].b) / 12) *
                                (pow(sh_property[sh_property_no].h, 2) +
                                 pow(sh_property[sh_property_no].b, 2)); //Torsional moment of inertia  h*b/12*(h^2+b^2)
                    }
                }

                ptr = strstr(t->text, "E=");
                if (ptr == NULL) break;

                sh_property[sh_property_no].E = atof(ptr + 2) * unit_factors_sh->E_f;

                ptr = strstr(t->text, "G=");
                if (ptr == NULL) break;

                sh_property[sh_property_no].G = atof(ptr + 2) * unit_factors_sh->E_f;

                sh_property[sh_property_no].r = 0.0;


                sh_property[sh_property_no].d = 0.0;

                ptr = strstr(t->text, "d=");
                if (ptr != NULL) {

                    sh_property[sh_property_no].d = atof(ptr + 2) * unit_factors_sh->d_f;
                }

                sh_property[sh_property_no].a = 0.0;

                ptr = strstr(t->text, "a=");
                if (ptr != NULL) {

                    sh_property[sh_property_no].a = atof(ptr + 2) * unit_factors_sh->a_f;
                }

                ptr = strstr(t->text, "c=");  //concrete cover, default 35 mm
                if (ptr != NULL) {
                    sh_property[sh_property_no].c = atof(ptr + 2) * unit_factors_sh->c_f;
                }

                ptr = strstr(t->text, "ζ=");  //zeta, default 0.85
                if (ptr != NULL) {
                    sh_property[sh_property_no].zeta = atof(ptr + 3);
                }

                ptr = strstr(t->text, "fck=");  //no default, in Mpa or kpsi
                if (ptr != NULL) {
                    sh_property[sh_property_no].fck = atof(ptr + 4); // * unit_factors_sh->E_f;
                } ////else if (sh_property[sh_property_no].RC_flag == 1) break;

                ptr = strstr(t->text, "fcd=");  //no default, in Mpa or kpsi
                if (ptr != NULL) {
                    sh_property[sh_property_no].fcd = atof(ptr + 4); // * unit_factors_sh->E_f;
                } ////else if (sh_property[sh_property_no].RC_flag == 1) break;

                ptr = strstr(t->text, "fyk=");  //no default, in Mpa or kpsi
                if (ptr != NULL) {
                    sh_property[sh_property_no].fyk = atof(ptr + 4); // * unit_factors_sh->E_f;
                } ////else if (sh_property[sh_property_no].RC_flag == 1) break;

                ptr = strstr(t->text, "fyd=");  //no default, in Mpa or kpsi
                if (ptr != NULL) {
                    sh_property[sh_property_no].fyd = atof(ptr + 4); // * unit_factors_sh->E_f;
                } ////else if (sh_property[sh_property_no].RC_flag == 1) break;

                ptr = strstr(t->text, u8"γ="); //gamma
                if (ptr == NULL) ptr = strstr(t->text, "Γ=");  //Gamma
                if (ptr != NULL) {
                    sh_property[sh_property_no].gamma = atof(ptr + 3);
                }

                ptr = strstr(t->text, u8"γi=");  //gamma inf
                if (ptr == NULL) ptr = strstr(t->text, "Γi=");  //Gamma inf
                if (ptr != NULL) {
                    sh_property[sh_property_no].gamma_inf = atof(ptr + 4);
                }

                ptr = strstr(t->text, u8"ξ="); //xi
                if (ptr == NULL) ptr1 = strstr(t->text, "Ξ=");  //Xi
                if (ptr != NULL) {
                    sh_property[sh_property_no].xi = atof(ptr + 3);
                }

                sh_property[sh_property_no].ok = 1;

                add_property_sh();

                break;
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, Otekst);
    }

    if (strlen(ptr_id_short)==0)
    {
        sprintf(report_row, "%s", _SHIELD_ID_NOT_FOUND_);
        strcat(report, report_row);
    }
    /////////////////
    ////searching for shield, hole, wall and zone polyline
    //searching for nodes size

    sh_min_x = 99999.;
    sh_min_y = 99999.;
    sh_max_x = -99999.;
    sh_max_y = -99999.;

    body_number = 0;
    ////searching for shields
    obiekt_tok((char *) ADP, ADK, (char **) &nag, OdBLOK);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            b = (BLOK *) nag;
            if (b->kod_obiektu == B_PLINE) {
                if (b->opis_obiektu[0] == PL_SHIELD) {

                    if (create_shield(b, 0, shield_no, body_number, &first, &last, &property_number, &is_closed)) {
                        if (is_closed) {
                            shield_property[shield_no].adr = b;
                            shield_property[shield_no].property_number = property_number;
                            shield_property[shield_no].first_edge = first;
                            shield_property[shield_no].last_edge = last;
                            shield_property[shield_no].ps = -1;
                            shield_property[shield_no].pn = -1;
                            shield_property[shield_no].load = 0;
                            shield_property[shield_no].body_number = body_number;
                            add_shield_property();
                            body_number++;
                        } else {
                            sprintf(report_row, "%s %d %s%s", _THE_SHIELD_, shield_no + 1, _POLYLINE_IS_NOT_CLOSED_,rn);
                            strcat(report, report_row);
                        }
                    }
                }
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, OdBLOK);
    }

    ////searching for holes
    obiekt_tok((char *) ADP, ADK, (char **) &nag, OdBLOK);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            b = (BLOK *) nag;
            if (b->kod_obiektu == B_PLINE) {
                if (b->opis_obiektu[0] == PL_HOLE) {
                    if (create_shield(b, 1, hole_no, body_number, &first, &last, &property_number, &is_closed)) {
                        if (is_closed) {
                            hole_property[hole_no].adr = b;
                            hole_property[hole_no].property_number = property_number;
                            hole_property[hole_no].first_edge = first;
                            hole_property[hole_no].last_edge = last;
                            hole_property[hole_no].ps = -1;
                            hole_property[hole_no].pn = -1;
                            hole_property[hole_no].load = 0;
                            hole_property[hole_no].body_number = body_number;
                            add_hole_property();
                            body_number++;
                        } else {
                            sprintf(report_row, "%s %d %s%s", _THE_HOLE_, hole_no + 1, _POLYLINE_IS_NOT_CLOSED_,rn);
                            strcat(report, report_row);
                        }
                    }
                }
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, OdBLOK);
    }

    ////searching for walls
    obiekt_tok((char *) ADP, ADK, (char **) &nag, OdBLOK);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            b = (BLOK *) nag;
            if (b->kod_obiektu == B_PLINE) {
                if (b->opis_obiektu[0] == PL_WALL) {
                    if (create_shield(b, 2, wall_no, body_number, &first, &last, &property_number, &is_closed)) {
                        if ((is_closed) || (!is_closed)) {
                            wall_property[wall_no].adr = b;
                            //wall_property[wall_no].property_number = property_number;
                            //inheriting property from shield
                            wall_property[wall_no].property_number = shield_property[0].property_number;
                            wall_property[wall_no].first_edge = first;
                            wall_property[wall_no].last_edge = last;
                            wall_property[wall_no].ps = -1;
                            wall_property[wall_no].pn = -1;
                            wall_property[wall_no].load = 0;
                            wall_property[wall_no].body_number = body_number;
                            add_wall_property();
                            body_number++;
                        } else {
                            sprintf(report_row, "%s %d %s%s", _THE_WALL_, wall_no + 1, _POLYLINE_IS_NOT_CLOSED_,rn);
                            strcat(report, report_row);
                        }
                    }
                }

            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, OdBLOK);
    }

    ////searching for zones
    obiekt_tok((char *) ADP, ADK, (char **) &nag, OdBLOK);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            b = (BLOK *) nag;
            if (b->kod_obiektu == B_PLINE) {
                if (b->opis_obiektu[0] == PL_ZONE) {
                    if (create_shield(b, 3, zone_no, body_number, &first, &last, &property_number, &is_closed)) {
                        if (is_closed) {
                            zone_property[zone_no].adr = b;
                            zone_property[zone_no].property_number = property_number;
                            zone_property[zone_no].first_edge = first;
                            zone_property[zone_no].last_edge = last;
                            zone_property[zone_no].ps = -1;
                            zone_property[zone_no].pn = -1;
                            zone_property[zone_no].load = 0;
                            zone_property[zone_no].body_number = body_number;
                            add_zone_property();
                            body_number++;
                        } else {
                            sprintf(report_row, "%s %d %s%s", _THE_ZONE_, zone_no + 1, _POLYLINE_IS_NOT_CLOSED_,rn);
                            strcat(report, report_row);
                        }
                    }
                }
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, OdBLOK);
    }

    ////counting objects
    if (shield_no == 0)  //no shield
    {
        sprintf(report_row, "%s%s", _THE_SHIELD_POLYLINE_NUMBER_IS_EQUAL_ZERO_,rn);
        strcat(report, report_row);
        //goto sh_error;
    }
    else if (shield_no > MAX_SHIELD_NO)  //to many shields - TEMPORARY
    {
        sprintf(report_row, "%s %d%s", _THE_SHIELD_POLYLINE_NUMBER_IS_GREATER_THAN_,MAX_SHIELD_NO,rn);
        strcat(report, report_row);
        //goto sh_error;
    }

    if (strlen(report) > 0) goto sh_error;

    ////searching for load
    obiekt_tok((char *) ADP, ADK, (char **) &nag, Ovector);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok))
        {
            v = (AVECTOR *) nag;

            if (((v->style>3) && (v->style<7)) || ((v->style>9) && (v->style<15)) || ((v->style>16) && (v->style<19)))
            {
                sh_load[sh_load_no].adr = v;
                sh_load[sh_load_no].x1 = v->x1;
                sh_load[sh_load_no].y1 = v->y1;
                sh_load[sh_load_no].x2 = v->x2;
                sh_load[sh_load_no].y2 = v->y2;

                parametry_lini((LINIA *) v, &PL);
                kos = sin(Angle_Normal(PL.kat * Pi / 180.));
                koc = cos(Angle_Normal(PL.kat * Pi / 180.));

                sh_load[sh_load_no].load = v->load;
                sh_load[sh_load_no].variant = v->variant;
                sh_load[sh_load_no].body = -1;  //not yet assigned
                sh_load[sh_load_no].body_no = -1;  //not yet assigned

                sh_load[sh_load_no].style = v->style;

                if (v->variant > 0)
                    sh_load[sh_load_no].factor_record = factor_record(v->load, v->variant);
                else
                    sh_load[sh_load_no].factor_record = -(v->load == 0 ? 2 : v->load);  //if no load spec, is assumed to be LL, so load=2
                sh_load[sh_load_no].take_it = 0;

                int v_factor_record = sh_load[sh_load_no].factor_record;

                switch (v->style)
                {
                    case 17: //distributed on the are
                        sh_load[sh_load_no].magnitude1y = v->magnitude1;
                        sh_load[sh_load_no].magnitude2y = v->magnitude2;
                        sh_load[sh_load_no].magnitude1x = sh_load[sh_load_no].magnitude2x = 0.0;  //
                        sh_load[sh_load_no].type=1;  //uniformly distributed load in xy plane downward
                        break;
                    case 18: //parallel to xy plane  - could be ignored, but we use it as normal force, but is always in y axis
                        sh_load[sh_load_no].magnitude1x = sh_load[sh_load_no].magnitude2x = 0.0;
                        sh_load[sh_load_no].magnitude1y = -v->magnitude1;
                        sh_load[sh_load_no].magnitude2y = -v->magnitude1;  //ignored anyway
                        sh_load[sh_load_no].type=0;  //concentrated force load in xy plane Fx, Fy,
                        break;
                    case 4: //parallel to xy plane in x and y axis
                        sh_load[sh_load_no].magnitude1x = -v->magnitude1 * koc;  //Fx
                        sh_load[sh_load_no].magnitude2x = sh_load[sh_load_no].magnitude1x;  //for consistency
                        sh_load[sh_load_no].magnitude1y = -v->magnitude1 * kos;  //Fy
                        sh_load[sh_load_no].magnitude2y = sh_load[sh_load_no].magnitude1y;  //for consistency
                        sh_load[sh_load_no].type=0; //concentrated force
                        break;
                    case 5: //moment +
                        sh_load[sh_load_no].magnitude1x = v->magnitude1;  //Mxy
                        sh_load[sh_load_no].magnitude1y = v->magnitude1;  //Mxy
                        sh_load[sh_load_no].magnitude2x = 0.;  //ignored
                        sh_load[sh_load_no].magnitude2y = 0.;  //ignored
                        sh_load[sh_load_no].type=0; //concentrated force
                        break;
                    case 6:  //moment -
                        sh_load[sh_load_no].magnitude1x = -v->magnitude1;  //Mxy
                        sh_load[sh_load_no].magnitude1y = -v->magnitude1;  //Mxy
                        sh_load[sh_load_no].magnitude2x = 0.;  //ignored
                        sh_load[sh_load_no].magnitude2y = 0.;  //ignored
                        sh_load[sh_load_no].type=0; //concentrated force
                        break;
                        //distributed along the line
                        //here is simplification assuming that each load lays within 1 single edge, never overlaps one edge
                        //however, if egde is split because extra concentrated load, in this case the load span over both part of entire chain
                        //here we only prepare basic information about load not assigning yet among edges
                        //all load is in global coordinates system, so only Normal load is spit into Qx and Qy components
                        //assuming is not reversed
                    case 10: //Y
                        sh_load[sh_load_no].magnitude1x = 0.;  //Qx
                        sh_load[sh_load_no].magnitude1y = -v->magnitude1;  //Qy
                        sh_load[sh_load_no].magnitude2x = 0.;  //Qx
                        sh_load[sh_load_no].magnitude2y = -v->magnitude2;  //Qy
                        sh_load[sh_load_no].type=2; //linear evenly (uniformly) or trapezoidally distributed load on edge in xy plane Qx, Qy
                        break;
                    case 11: //X
                        sh_load[sh_load_no].magnitude1x = -v->magnitude1;  //Qx
                        sh_load[sh_load_no].magnitude1y = 0.;  //Qy
                        sh_load[sh_load_no].magnitude2x = -v->magnitude2;  //Qx
                        sh_load[sh_load_no].magnitude2y = 0.;  //Qy
                        sh_load[sh_load_no].type=2; //linear
                        break;
                    case 12: //N
                        sh_load[sh_load_no].magnitude1x = v->magnitude1 * kos;  //Qx
                        sh_load[sh_load_no].magnitude1y = -v->magnitude1 * koc;  //Qy
                        sh_load[sh_load_no].magnitude2x = v->magnitude2 * kos;  //Qx
                        sh_load[sh_load_no].magnitude2y = -v->magnitude2 * koc;  //Qy
                        sh_load[sh_load_no].type=2; //linear
                        break;
                    case 13: //H  horizontal projection
                        sh_load[sh_load_no].magnitude1x = 0.;  //Qx
                        sh_load[sh_load_no].magnitude1y = -v->magnitude1 * koc;  //Qy
                        sh_load[sh_load_no].magnitude2x = 0.;  //Qx
                        sh_load[sh_load_no].magnitude2y = -v->magnitude2 * koc;  //Qy
                        sh_load[sh_load_no].type=2; //linear
                        break;
                    case 14: //V  vertical projection
                        sh_load[sh_load_no].magnitude1x = v->magnitude1 * kos;  //Qx
                        sh_load[sh_load_no].magnitude1y = 0.;  //Qy
                        sh_load[sh_load_no].magnitude2x = v->magnitude2 * kos;  //Qx
                        sh_load[sh_load_no].magnitude2y = 0.;  //Qy
                        sh_load[sh_load_no].type=2; //linear
                        break;
                    default:
                        break;
                }
                sh_load[sh_load_no].k=-1; //not defined yet
                add_load_sh();
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, Ovector);
    }

    ///removing Ablok flags to assign load to bodies
    zmien_atrybut_undo(dane, dane + dane_size);
    blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
    zmien_atrybut(ADP, ADK, Ablok, Aoblok);
    was_refreshed = TRUE;

    //this is actually not necessary
    /*
    for (j=0; j<zone_no; j++)
    {
        ADPB = (char *) zone_property[j].adr;
        ADKB = ADPB + sizeof(NAGLOWEK) + zone_property[j].adr->n - 1;
        correct_pline(ADPB, ADKB);
    }

    for (j=0; j<hole_no; j++)
    {
        ADPB = (char *) hole_property[j].adr;
        ADKB = ADPB + sizeof(NAGLOWEK) + hole_property[j].adr->n - 1;
        correct_pline(ADPB, ADKB);
    }

    for (j=0; j<wall_no; j++)
    {
        ADPB = (char *) wall_property[j].adr;
        ADKB = ADPB + sizeof(NAGLOWEK) + wall_property[j].adr->n - 1;
        correct_pline(ADPB, ADKB);
    }

    for (j=0; j<shield_no; j++)
    {
        ADPB = (char *) shield_property[j].adr;
        ADKB = ADPB + sizeof(NAGLOWEK) + shield_property[j].adr->n - 1;
        correct_pline(ADPB, ADKB);
    }
    */

    //assigning:
    // each hole to zone or shield
    // each wall to zone or shield, wall cannot belong to any hole
    // each zone to shield
    // we have for each element first_edge and last edge
    // checking if each endpoint of each edges lays inside another element

    BOOL missed = FALSE;


    //ZONES in ZONES  -  FOR GRADIATION
    for (i=0; i<zone_no; i++)
    {
        zone_property[i].pnz_n = 0;  //no parent zones
        zone_property[i].pnz = 0;  //parent zones flags zeroed

        //matching with shield
        for (j = 0; j < zone_no; j++)
        {
            if (j!=i)
            {
                ADPB = (char *) zone_property[j].adr;
                ADKB = ADPB + sizeof(NAGLOWEK) + zone_property[j].adr->n - 1;
                zmien_atrybut_for_objects(ADPB, ADKB, Aoblok, Ablok, Blinia | Bluk);
                //BOOL found = FALSE;
                missed = FALSE;
                for (k = zone_property[i].first_edge; k < zone_property[i].last_edge; k++) {
                    df_x1 = sh_node[sh_edge[k].node1].x;
                    df_y1 = sh_node[sh_edge[k].node1].y;
                    df_x2 = sh_node[sh_edge[k].node2].x;
                    df_y2 = sh_node[sh_edge[k].node2].y;
                    ret1 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x1, df_y1,
                                           &s_hatch_param, 1, 0, 0, 0, 0, 0);
                    if (!ret1) {
                        missed = TRUE;
                        break;
                    }
                    ret2 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x2, df_y2,
                                           &s_hatch_param, 1, 0, 0, 0, 0, 0);
                    if (!ret2) {
                        missed = TRUE;
                        break;
                    }
                }
                zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
                if (missed == FALSE) {
                    zone_property[i].ps = 3; //zone
                    zone_property[i].pnz_n++;
                    // Set a bit of parent zone
                    zone_property[i].pnz = zone_property[i].pnz | (1LL << j);
                    //to be continued for all zones
                }
            }
        }
    }

    //ZONES  GRADIATION
    long long mask;
    for (i=0; i<zone_no; i++)
    {
        if ((zone_property[i].ps == 3) && (zone_property[i].pnz_n>1)) //zone in zone(s), more than 1
        {
            for (j=0; j<nest_bits; j++)  //nest bits
            {
               mask = (1LL << j);
               if ((zone_property[i].pnz & mask) !=0)  //bit is on
               {
                   if ((zone_property[j].pnz==0) && (zone_property[i].pnz_n>1)) //this j zone doesn't belong to any other and i zone belongs to more than 1 zones
                   {
                       zone_property[i].pnz &= ~(1LL << j);  //removing external zone
                   }
                   else
                   {
                       //checking j zone
                       for (k = 0; k < nest_bits; k++)  //nest bits
                       {
                           mask = (1LL << k);
                           if ((zone_property[j].pnz & mask) != 0)  //bit is on
                               zone_property[i].pnz &= ~(1LL << k);
                       }
                   }
               }
            }
        }
    }

    //this is useless
    //for (i = 0; i < zone_no; i++) printf("%d %d\n",zone_property[i].ps, zone_property[i].pn);
    //printf("---------------\n");
    //qsort(zone_property, zone_no, sizeof(SHIELD_PROPERTY), qsort_by_val_pnz);
    //for (i = 0; i < zone_no; i++) printf("%d %d\n",zone_property[i].ps, zone_property[i].pn);
    //printf("---------------\n");

    //CONSOLIDATING ZONES
    for (i=0; i<zone_no; i++)
    {
        for (j=0; j<nest_bits; j++)  //nest bits
        {
            mask = (1LL << j);
            if ((zone_property[i].pnz & mask) !=0)  //bit is on
            {
                zone_property[i].pn=j;  //j is a number of closest zone which i zone belongs to
            }
        }
    }

    //TEMPORARY - if zones ios included in another zone, it stops
    for (i=0; i<zone_no; i++)
    {
        if (zone_property[i].ps==3)
        {
            sprintf(report_row, "%s: #%d%s", _THE_ZONE_IS_ASSIGNED_TO_ANOTHER_ZONE_,zone_property[i].property_number,rn);
            strcat(report, report_row);
        }
    }


    //this is useless
    //for (i = 0; i < zone_no; i++) printf("%d %d\n",zone_property[i].ps, zone_property[i].pn);
    //printf("---------------\n");
    //qsort(zone_property, zone_no, sizeof(SHIELD_PROPERTY), qsort_by_val);
    //for (i = 0; i < zone_no; i++) printf("%d %d\n",zone_property[i].ps, zone_property[i].pn);
    //printf("---------------\n");

    ////

    //sorting zones  -  THIS IS TEMPORARY OFF
    /*
    if (zone_no>0)
    {
        int *top_zone = (int *) malloc(zone_no * sizeof(int));
        int top_zone_no = 0;
        int tier0=0, tier1=0, tier2=0, tier3=0, tier4=0;
        //int next_tier;
        int **tier_zone = (int**) malloc(zone_no * sizeof(int *));
        int *tier_zone_no = (int*) malloc(zone_no * sizeof(int *));

        int *sorted_zone = (int*) malloc(zone_no * sizeof(int *));
        int sorted_zone_no=0;

        SHIELD_PROPERTY *sorted_zone_property = (SHIELD_PROPERTY*) malloc(zone_no * sizeof(SHIELD_PROPERTY));

        for (i = 0; i < zone_no; i++)
        {
            if (zone_property[i].ps == -1) {
                top_zone[top_zone_no] = i;
                top_zone_no++;
            }
        }
        for (i=0; i<top_zone_no; i++)
        {
            tier_zone[tier0] = (int *) malloc(zone_no * sizeof(int));
            tier_zone_no[tier0] = 0;
            for (j = 0; j < zone_no; j++)
            {
                if (j!=top_zone[i])  //not with itself
                {
                    if ((zone_property[j].ps==3) && (zone_property[j].pn==top_zone[i])) //zone belongs to top
                    {
                        tier_zone[tier0][tier_zone_no[tier0]]=j;
                        tier_zone_no[tier0]++;
                    }
                }
            }
            //so we got tier 0
            for (j=0; j<tier_zone_no[tier0]; j++)
            {
                tier1 = tier0 + 1;
                tier_zone[tier1] = (int *) malloc(zone_no * sizeof(int));
                tier_zone_no[tier1] = 0;
                for (int jj = 0; jj < zone_no; jj++) {
                    if (jj != tier_zone[tier0][j])  //not with itself
                    {
                        if ((zone_property[jj].ps == 3) && (zone_property[jj].pn == tier_zone[tier0][j])) //zone belongs to top
                        {
                            tier_zone[tier1][tier_zone_no[tier1]] = jj;
                            tier_zone_no[tier1]++;
                        }
                    }
                }
                //so we got tier 1
                for (j = 0; j < tier_zone_no[tier1]; j++)
                {
                    tier2 = tier1 + 1;
                    tier_zone[tier2] = (int *) malloc(zone_no * sizeof(int));
                    tier_zone_no[tier2] = 0;
                    for (int jj = 0; jj < zone_no; jj++) {
                        if (jj != tier_zone[tier1][j])  //not with itself
                        {
                            if ((zone_property[jj].ps == 3) && (zone_property[jj].pn == tier_zone[tier1][j])) //zone belongs to top
                            {
                                tier_zone[tier2][tier_zone_no[tier2]] = jj;
                                tier_zone_no[tier2]++;
                            }
                        }
                    }
                    //so we got tier 2
                    for (j = 0; j < tier_zone_no[tier2]; j++)
                    {
                        tier3 = tier2 + 1;
                        tier_zone[tier3] = (int *) malloc(zone_no * sizeof(int));
                        tier_zone_no[tier3] = 0;
                        for (int jj = 0; jj < zone_no; jj++) {
                            if (jj != tier_zone[tier2][j])  //not with itself
                            {
                                if ((zone_property[jj].ps == 3) && (zone_property[jj].pn == tier_zone[tier2][j])) //zone belongs to top
                                {
                                    tier_zone[tier3][tier_zone_no[tier3]] = jj;
                                    tier_zone_no[tier3]++;
                                }
                            }
                        }
                        //so we got tier 3
                        for (j = 0; j < tier_zone_no[tier3]; j++)
                        {
                            tier4 = tier3 + 1;
                            tier_zone[tier4] = (int *) malloc(zone_no * sizeof(int));
                            tier_zone_no[tier4] = 0;
                            for (int jj = 0; jj < zone_no; jj++) {
                                if (jj != tier_zone[tier3][j])  //not with itself
                                {
                                    if ((zone_property[jj].ps == 3) && (zone_property[jj].pn == tier_zone[tier3][j])) //zone belongs to top
                                    {
                                        tier_zone[tier4][tier_zone_no[tier4]] = jj;
                                        tier_zone_no[tier4]++;
                                    }
                                }
                            }
                            //so we got tier 4 - let's stop here
                            //collecting data
                            for (j=0; j<tier_zone_no[tier4]; j++)
                            {
                                sorted_zone[sorted_zone_no]=tier_zone[tier4][j];
                                sorted_zone_no++;
                            }
                            //releasing tier4
                            free(tier_zone[tier4]);

                        }
                        //collecting data
                        for (j=0; j<tier_zone_no[tier3]; j++)
                        {
                            sorted_zone[sorted_zone_no]=tier_zone[tier3][j];
                            sorted_zone_no++;
                        }
                        //releasing tier3
                        free(tier_zone[tier3]);
                    }
                    //collecting data
                    for (j=0; j<tier_zone_no[tier2]; j++)
                    {
                        sorted_zone[sorted_zone_no]=tier_zone[tier2][j];
                        sorted_zone_no++;
                    }
                    //releasing tier2
                    free(tier_zone[tier2]);
                }
                //collecting data
                for (j=0; j<tier_zone_no[tier1]; j++)
                {
                    sorted_zone[sorted_zone_no]=tier_zone[tier1][j];
                    sorted_zone_no++;
                }
                //releasing tier1
                free(tier_zone[tier1]);
            }
            //collecting data
            for (j=0; j<tier_zone_no[tier0]; j++)
            {
                sorted_zone[sorted_zone_no]=tier_zone[tier0][j];
                sorted_zone_no++;
            }
            //releasing tier0
            free(tier_zone[tier0]);
        }
        //collecting data
        for (j=0; j<top_zone_no; j++)
        {
            sorted_zone[sorted_zone_no]=top_zone[j];
            sorted_zone_no++;
        }
        //releasing tier0
        free(top_zone);

        int *pn_table=(int*) malloc(zone_no*sizeof(int));

        if (sorted_zone_no>0)
        {
            //creating pn_table

            for (j=0; j<zone_no; j++)
            {
                //pn_table[zone_property[sorted_zone[j]].pn]=j;
                pn_table[j]=sorted_zone[j];
            }

            if (sorted_zone_no==zone_no)
            {
                for (j = 0; j < sorted_zone_no; j++) {
                    memmove(&sorted_zone_property[j], &zone_property[sorted_zone[j]], sizeof(SHIELD_PROPERTY));
                    //.pn need to be changed
                    sorted_zone_property[j].pn=pn_table[sorted_zone_property[j].pn];
                }
                //finally overwrite zone_properties
                memmove(zone_property, sorted_zone_property, zone_no * sizeof(SHIELD_PROPERTY));
            }
        }
        free(sorted_zone_property);
        free(pn_table);
    }
    */

    //HOLES in ZONES

    for (i=0; i<hole_no; i++)
    {
        hole_property[i].pnz_n = 0;  //no parent zones
        hole_property[i].pnz = 0;  //parent zones flags zeroed
        //matching with zones
        for (j=0; j<zone_no; j++)
        {
            ADPB = (char *) zone_property[j].adr;
            ADKB = ADPB + sizeof(NAGLOWEK) + zone_property[j].adr->n - 1;
            zmien_atrybut_for_objects(ADPB, ADKB, Aoblok, Ablok, Blinia | Bluk);
            //BOOL found = FALSE;
            missed = FALSE;
            for (k=hole_property[i].first_edge; k<hole_property[i].last_edge; k++)
            {
                df_x1 = sh_node[sh_edge[k].node1].x;
                df_y1 = sh_node[sh_edge[k].node1].y;
                if (sh_edge[k].type==0) {
                    df_x2 = sh_node[sh_edge[k].node2].x;
                    df_y2 = sh_node[sh_edge[k].node2].y;
                } else
                {
                    df_x2 = sh_node[sh_edge[k].node3].x;
                    df_y2 = sh_node[sh_edge[k].node3].y;
                }
                ret1 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x1, df_y1, &s_hatch_param,1, 0, 0, 0, 0, 0);
                if (!ret1) {missed=TRUE; break;}
                ret2 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x2, df_y2, &s_hatch_param,1, 0, 0, 0, 0, 0);
                if (!ret2) {missed=TRUE; break;}
            }
            zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
            if (missed==FALSE)
            {
                hole_property[i].ps=3; //zone
                //hole_property[i].pn=j;
                hole_property[i].pnz_n++;
                // Set a bit of parent zone
                hole_property[i].pnz = hole_property[i].pnz | (1LL << j);
                //to be continued for all zones
            }
        }
    }

    //HOLES in ZONES GRADIATION
    for (i=0; i<hole_no; i++)
    {
        if ((hole_property[i].ps == 3) && (hole_property[i].pnz_n>1)) //hole in zone(s), more than 1
        {
            for (j=0; j<nest_bits; j++)  //nest bits
            {
                mask = (1LL << j);
                if ((hole_property[i].pnz & mask) !=0)  //bit is on
                {
                    if ((zone_property[j].pnz==0) && (hole_property[i].pnz_n>1)) //this j zone doesn't belong to any other and i zone belongs to more than 1 zones
                    {
                        hole_property[i].pnz &= ~(1LL << j);  //removing external zone
                    }
                    else
                    {
                        //checking j zone
                        for (k = 0; k < nest_bits; k++)  //nest bits
                        {
                            mask = (1LL << k);
                            if ((zone_property[j].pnz & mask) != 0)  //bit is on
                                hole_property[i].pnz &= ~(1LL << k);
                        }
                    }
                }
            }
        }
    }

    //CONSOLIDATING HOLES
    for (i=0; i<hole_no; i++)
    {
        for (j=0; j<nest_bits; j++)  //nest bits
        {
            mask = (1LL << j);
            if ((hole_property[i].pnz & mask) !=0)  //bit is on
            {
                hole_property[i].pn=j;  //j is a number of closest zone which i hole belongs to
            }
        }
    }

    //HOLES in SHIELDS
    for (i=0; i<hole_no; i++)
    {
        if (hole_property[i].ps<0) //not yet assigned
        {
            //matching with shield
            for (j = 0; j < shield_no; j++) {
                ADPB = (char *) shield_property[j].adr;
                ADKB = ADPB + sizeof(NAGLOWEK) + shield_property[j].adr->n - 1;

                zmien_atrybut_for_objects(ADPB, ADKB, Aoblok, Ablok, Blinia | Bluk);
                //BOOL found = FALSE;
                missed = FALSE;
                for (k = hole_property[i].first_edge; k < hole_property[i].last_edge; k++)
                {
                    df_x1 = sh_node[sh_edge[k].node1].x;
                    df_y1 = sh_node[sh_edge[k].node1].y;
                    if (sh_edge[k].type==0)  //line
                    {
                        df_x2 = sh_node[sh_edge[k].node2].x;
                        df_y2 = sh_node[sh_edge[k].node2].y;
                    } else  //arc
                    {
                        df_x2 = sh_node[sh_edge[k].node3].x;
                        df_y2 = sh_node[sh_edge[k].node3].y;
                    }
                    ret1 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x1, df_y1,&s_hatch_param, 1, 0, 0, 0, 0, FALSE);
                    //ret1=1; //TEMPORARY

                    if (!ret1) {
                        missed = TRUE;
                        break;
                    }
                    ret2 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x2, df_y2, &s_hatch_param, 1, 0, 0, 0, 0, 0);
                    //ret2=1; //TEMPORARY

                    if (!ret2) {
                        missed = TRUE;
                        break;
                    }
                }
                zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
                if (missed == FALSE) {
                    hole_property[i].ps = 0; //shield
                    hole_property[i].pn = j;
                    break;
                }
            }
        }
    }

    //WALLS in ZONES
    for (i=0; i<wall_no; i++)
    {
        wall_property[i].pnz_n = 0;  //no parent zones
        wall_property[i].pnz = 0;  //parent zones flags zeroed
        //matching with zones
        for (j=0; j<zone_no; j++)
        {
            ADPB = (char *) zone_property[j].adr;
            ADKB = ADPB + sizeof(NAGLOWEK) + zone_property[j].adr->n - 1;
            zmien_atrybut_for_objects(ADPB, ADKB, Aoblok, Ablok, Blinia | Bluk);
            //BOOL found = FALSE;
            missed = FALSE;
            for (k=wall_property[i].first_edge; k<wall_property[i].last_edge; k++)
            {
                df_x1 = sh_node[sh_edge[k].node1].x;
                df_y1 = sh_node[sh_edge[k].node1].y;
                if (sh_edge[k].type==0) {
                    df_x2 = sh_node[sh_edge[k].node2].x;
                    df_y2 = sh_node[sh_edge[k].node2].y;
                } else
                {
                    df_x2 = sh_node[sh_edge[k].node3].x;
                    df_y2 = sh_node[sh_edge[k].node3].y;
                }
                ret1 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x1, df_y1, &s_hatch_param,1, 0, 0, 0, 0, 0);
                if (!ret1) {missed=TRUE; break;}
                ret2 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x2, df_y2, &s_hatch_param,1, 0, 0, 0, 0, 0);
                if (!ret2) {missed=TRUE; break;}
            }
            zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
            if (missed==FALSE)
            {
                wall_property[i].ps=3; //zone
                //wall_property[i].pn=j;
                wall_property[i].pnz_n++;
                // Set a bit of parent zone
                wall_property[i].pnz = wall_property[i].pnz | (1LL << j);
                //to be continued for all zones
            }
        }
    }

    //WALLS in ZONES GRADIATION
    for (i=0; i<wall_no; i++)
    {
        if ((wall_property[i].ps == 3) && (wall_property[i].pnz_n>1)) //wall in zone(s), more than 1
        {
            for (j=0; j<nest_bits; j++)  //nest bits
            {
                mask = (1LL << j);
                if ((wall_property[i].pnz & mask) !=0)  //bit is on
                {
                    if ((zone_property[j].pnz==0) && (wall_property[i].pnz_n>1)) //this j zone doesn't belong to any other and i zone belongs to more than 1 zones
                    {
                        wall_property[i].pnz &= ~(1LL << j);  //removing external zone
                    }
                    else
                    {
                        //checking j zone
                        for (k = 0; k < nest_bits; k++)  //nest bits
                        {
                            mask = (1LL << k);
                            if ((zone_property[j].pnz & mask) != 0)  //bit is on
                                wall_property[i].pnz &= ~(1LL << k);
                        }
                    }
                }
            }
        }
    }

    //CONSOLIDATING WALLS
    for (i=0; i<wall_no; i++)
    {
        for (j=0; j<nest_bits; j++)  //nest bits
        {
            mask = (1LL << j);
            if ((wall_property[i].pnz & mask) !=0)  //bit is on
            {
                wall_property[i].pn=j;  //j is a number of closest zone which i wall belongs to
            }
        }
    }

    //WALLS in SHIELDS
    for (i=0; i<wall_no; i++)
    {
        if (wall_property[i].ps<0) //not yet assigned
        {
            //matching with shield
            for (j = 0; j < shield_no; j++) {
                ADPB = (char *) shield_property[j].adr;
                ADKB = ADPB + sizeof(NAGLOWEK) + shield_property[j].adr->n - 1;
                zmien_atrybut_for_objects(ADPB, ADKB, Aoblok, Ablok, Blinia | Bluk);
                //BOOL found = FALSE;
                missed = FALSE;
                for (k = wall_property[i].first_edge; k < wall_property[i].last_edge; k++) {
                    df_x1 = sh_node[sh_edge[k].node1].x;
                    df_y1 = sh_node[sh_edge[k].node1].y;
                    if (sh_edge[k].type==0) {
                        df_x2 = sh_node[sh_edge[k].node2].x;
                        df_y2 = sh_node[sh_edge[k].node2].y;
                    } else
                    {
                        df_x2 = sh_node[sh_edge[k].node3].x;
                        df_y2 = sh_node[sh_edge[k].node3].y;
                    }
                    ret1 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x1, df_y1,
                                           &s_hatch_param, 1, 0, 0, 0, 0, 0);
                    if (!ret1) {
                        missed = TRUE;
                        break;
                    }
                    ret2 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x2, df_y2,
                                           &s_hatch_param, 1, 0, 0, 0, 0, 0);
                    if (!ret2) {
                        missed = TRUE;
                        break;
                    }
                }
                zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
                if (missed == FALSE) {
                    wall_property[i].ps = 0; //shield
                    wall_property[i].pn = j;
                    break;
                }
            }
        }
    }

    //ZONES in SHIELDS
    for (i=0; i<zone_no; i++)
    {

        if (zone_property[i].ps<0) //not yet assigned
        {
            //matching with shield
            for (j = 0; j < shield_no; j++)
            {
                ADPB = (char *) shield_property[j].adr;
                ADKB = ADPB + sizeof(NAGLOWEK) + shield_property[j].adr->n - 1;
                zmien_atrybut_for_objects(ADPB, ADKB, Aoblok, Ablok, Blinia | Bluk);
                //BOOL found = FALSE;
                missed = FALSE;
                for (k = zone_property[i].first_edge; k < zone_property[i].last_edge; k++) {
                    df_x1 = sh_node[sh_edge[k].node1].x;
                    df_y1 = sh_node[sh_edge[k].node1].y;
                    if (sh_edge[k].type==0) {
                        df_x2 = sh_node[sh_edge[k].node2].x;
                        df_y2 = sh_node[sh_edge[k].node2].y;
                    } else
                    {
                        df_x2 = sh_node[sh_edge[k].node2].x;
                        df_y2 = sh_node[sh_edge[k].node2].y;
                    }
                    ret1 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x1, df_y1,
                                           &s_hatch_param, 1, 0, 0, 0, 0, 0);
                    if (!ret1) {
                        missed = TRUE;
                        break;
                    }
                    ret2 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x2, df_y2,
                                           &s_hatch_param, 1, 0, 0, 0, 0, 0);
                    if (!ret2) {
                        missed = TRUE;
                        break;
                    }
                }
                zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
                if (missed == FALSE) {
                    zone_property[i].ps = 0; //shield
                    zone_property[i].pn = j;
                    break;
                }
            }
        }
    }

    //checking assignement
    for (i=0; i<hole_no; i++)
    {
        if (hole_property[i].ps<0) //ALERT
        {
            sprintf(report_row, "%s%s", _THE_HOLE_NOT_ASSIGNED_TO_ZONE_OR_SHIELD_,rn);
            strcat(report, report_row);
        }
    }

    for (i=0; i<wall_no; i++)
    {
        if (wall_property[i].ps<0) //ALERT
        {
            sprintf(report_row, "%s%s", _THE_WALL_NOT_ASSIGNED_TO_ZONE_OR_SHIELD_,rn);
            strcat(report, report_row);
        }
    }

    for (i=0; i<zone_no; i++)
    {
        if (zone_property[i].ps<0) //ALERT
        {
            sprintf(report_row, "%s%s", _THE_ZONE_NOT_ASSIGNED_TO_SHIELD_,rn);
            strcat(report, report_row);
        }
    }

    if (strlen(report) > 0) goto sh_error;

    ////LOAD assignement
    ////If there is point load inside surfece:
    //// We define a new point
    ////Point(5) = {0.02, 0.02, 0, lc};
    //// We force this point to be included ("embedded") in the 2D mesh, using
    //// the `Point In Surface' command:
    ////Point{5} In Surface{1};
    //// In the same way, if this is linear load, e.g uniformly or trapezoidally distributedone,
    /// we force a curve to be embedded in the 2D mesh using the `Curve in Surface' command:
    ////Point(6) = {0.02, 0.12, 0, lc};
    ////Point(7) = {0.04, 0.18, 0, lc};
    ////Line(5) = {6, 7};
    ////Curve{5} In Surface{1};
    ////In the load is applied on the edge of zone or shield, edge is split into 2 lines or 2 arcs, recursively,
    ////it means edge line or arc can be split several times, if severa point forces are applied on the edge
    ///Concentrated force with X and Y components and and moment XY can be applied.
    ///Then for point load in sif file:
    //// Body Force 1
    ////   Name = "BodyForce 1"
    ////   Body Force 2 = Real MATC "(-3678.75) + (-5000)"
    ////   Point Load (5, 9) = Real  27 7.7607 0.0 -0 -50000 0.0 0.0 0.0 0.0 28 7.7607 0.0 -0 -50000 0.0 0.0 0.0 0.0 31 7.7607 0.0 -0 -50000 0.0 0.0 0.0 0.0 30 7.7607 0.0 -0 -50000 0.0 0.0 0.0 0.0 29 7.7607 0.0 -0 -50000 0.0 0.0 0.0 0.0
    ////   Thickness = Real 0.15
    //// End
    ////For linear load:
    ////
    ////  Boundary Condition 2
    ////    ! load on top edge
    ////    Target Boundaries(1) = 1
    ////    Name = "EdgeLoad"
    ////    ! load in Y axis
    ////    Body Force 1 = Real 0.0
    ////    Body Force 2 = Real MATC "(-9.81 * 2500)"
    ////    Body Force 3 = Real 0.0
    ////  End
    ////or for variable load, e.g. trapezoidally distributed load:
    ////  Boundary Condition 2
    ////    ! load on top and bottom edge
    ////    Target Boundaries(2) = 1 3
    ////    ! trapezoidal load
    ////    Name = "VariableLoad"
    ////    Force 2 = Variable Coordinate 1
    ////      Real
    ////      0 0
    ////      10 -25000
    ////    End
    ////  End
    //// For self-weight and uniformly distributed load on body:
    ////  Body Force 1
    ////    Name = "BodyForce 1"
    ////    Normal Pressure = Real MATC "(-2500)"
    ////    !self-weight
    ////    Body Force 1 = Real 0.0
    ////    Body Force 2 = Real MATC "(-9.81 * 2500)"
    ////    ! or
    ////    ! Body Force 2 = Real -25000
    ////    Body Force 3 = Real 0.0
    ////    Thickness = Real 0.15
    ////  End


    double Xbak;
    double Ybak;

    SH_EDGE sh_edge_buf;

    //Load assignment is done in two steps:
    //step 1 for linear load .style > 9  style < 15
    //setp 2 for pointwise load
    //if edge must be split for parts:
    // for each load which has assigned the edge index higher than split edge, edge index is shifted by 1
    // for the load which has assigned the edge index equal to split edge, load is spleat between two new segments of edge

    for (int pointwise=0; pointwise<2; pointwise++)
    {
        for (i = 0; i < sh_load_no; i++)
        {
            if ((sh_load[i].type==0 && pointwise==1) || (sh_load[i].type>0 && pointwise==0))
            {
                df_x1 = sh_load[i].x1;
                df_y1 = sh_load[i].y1;
                df_x2 = sh_load[i].x2;
                df_y2 = sh_load[i].y2;

                ////marking wall - it's for consistency, there are no walls in shields, but maybe will appear in the future
                for (j = 0; j < wall_no; j++)
                {
                    ADPB = (char *) wall_property[j].adr;
                    ADKB = ADPB + sizeof(NAGLOWEK) + wall_property[j].adr->n - 1;
                    zmien_atrybut_for_objects(ADPB, ADKB, Aoblok, Ablok, Blinia | Bluk);
                    ret1 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x1, df_y1, &s_hatch_param,
                                           1, 0, 0, 0, 0, 0);
                    if (sh_load[i].type > 0)   //two points of distribution or range
                        ret2 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x2, df_y2, &s_hatch_param,
                                               1, 0, 0, 0, 0, 0);
                    else ret2 = ret1;  //same point
                    zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
                    if ((ret1 == 1) && (ret2 == 1)) {
                        //sh_load[i].body = 2; //wall
                        //sh_load[i].body_no = j;
                        sh_load[i].body = wall_property[j].ps;
                        sh_load[i].body_no = wall_property[j].pn;
                        sh_load[i].point_body = wall_property[j].ps;   //2
                        sh_load[i].point_body_no = wall_property[j].pn;   //j
                        sh_load[i].point_edge_no = 0;   //not installed on edge

                        if (sh_load[i].type == 0)  //point load
                            embed_node(i, geo_units_factor);
                        else if (sh_load[i].type == 2)  //distributed load
                        {
                            ////Point(6) = {0.02, 0.12, 0, lc};
                            ////Point(7) = {0.04, 0.18, 0, lc};
                            ////Line(5) = {6, 7};
                            ////Curve{5} In Surface{1};
                            embed_nodes_edge(i, 2 /*wall*/, j /*number*/, wall_property[j].body_number, geo_units_factor);
                        }

                        if (sh_load[i].body == 3) zone_property[j].load++;
                        else shield_property[j].load++;
                        break;
                    } else {
                        //the procedure of checking edges and splitting edges - like for zones and shields - can be added in the future, when walls will be added for some reason
                        ;
                    }
                }
                if (j == wall_no) //not found in walls
                {
                    ////marking zone
                    for (j = 0; j < zone_no; j++)
                    {
                            BOOL found=FALSE;
                            //checking holes in the zone
                            for (int jh = 0; jh < hole_no; jh++)
                            {
                                if ((hole_property[jh].ps == 3) && (hole_property[jh].pn == j)) //zone and this zone
                                {
                                    for (int jk = hole_property[jh].first_edge; jk < hole_property[jh].last_edge; jk++)
                                    {
                                        double loadx1, loady1, loadx2, loady2;
                                        loadx1 = sh_load[i].x1;
                                        loady1 = sh_load[i].y1;
                                        loadx2 = sh_load[i].x2;
                                        loady2 = sh_load[i].y2;
                                        if (sh_edge[jk].type == 0)  //line
                                        {
                                            Xbak = X;
                                            Ybak = Y;
                                            X = loadx1;
                                            Y = loady1;

                                            X = Xbak;
                                            Y = Ybak;
                                            if (check_if_point_on_line_segment(sh_node[sh_edge[jk].node1].x, sh_node[sh_edge[jk].node1].y, sh_node[sh_edge[jk].node2].x, sh_node[sh_edge[jk].node2].y, loadx1, loady1)) {
                                                sh_load[i].body = 3; //zone
                                                sh_load[i].body_no = j;
                                                sh_load[i].point_body = 3; //zone
                                                sh_load[i].point_body_no = j;
                                                sh_load[i].point_edge_no = jk;

                                                zone_property[j].load++;

                                                //checking if point exists
                                                int kk;
                                                for (kk = 0; kk < sh_node_no; kk++) {
                                                    if ((Check_if_Equal(sh_node[kk].x, loadx1) == TRUE) && (Check_if_Equal(sh_node[kk].y, loady1) == TRUE)) //point in point
                                                        break;
                                                }
                                                if (sh_load[i].type == 0)  //point load, so extra point is needed if necessary
                                                {
                                                    if (kk == sh_node_no) //needs to add new point
                                                    {
                                                        //adding point
                                                        //embed_node_edge(i, geo_units_factor);
                                                        sh_node[sh_node_no].x = loadx1;
                                                        sh_node[sh_node_no].y = loady1;
                                                        sh_node[sh_node_no].d = sh_node[sh_edge[jk].node2].d; //setting the division for FE

                                                        double xx01 = sh_node[sh_edge[jk].node1].x;
                                                        double yy01 = sh_node[sh_edge[jk].node1].y;
                                                        double xx02 = sh_node[sh_edge[jk].node2].x;
                                                        double yy02 = sh_node[sh_edge[jk].node2].y;

                                                        //spitting edge
                                                        //SH_EDGE sh_edge_buf;
                                                        memmove(&sh_edge_buf, &sh_edge[jk], sizeof(SH_EDGE));

                                                        sh_edge[jk].node2 = sh_node_no;
                                                        //calculate midpoint
                                                        sh_edge[jk].xm = (float) (sh_node[sh_edge[jk].node1].x + loadx1) / 2.0f;
                                                        sh_edge[jk].ym = (float) (sh_node[sh_edge[jk].node1].y + loady1) / 2.0f;

                                                        sh_edge_buf.node1 = sh_node_no;
                                                        sh_edge_buf.xm = (float) (sh_node[sh_edge[jk].node2].x + loadx1) / 2.0f;
                                                        sh_edge_buf.ym = (float) (sh_node[sh_edge[jk].node2].y + loady1) / 2.0f;
                                                        add_node_sh();

                                                        //inserting sh_edge_buf
                                                        insert_sh_edge(jk + 1, &sh_edge_buf, xx01, yy01, xx02, yy02, loadx1, loady1, TRUE);  //sh_edge[jk] stays, was already modified, loadx1, loady1 is split point

                                                        found = TRUE;
                                                        break;
                                                    }
                                                } else if (sh_load[i].type == 2) //linear uniformly or trapezoidally distributed load
                                                {
                                                    //checking the second point to make sure the load lays on edge
                                                    Xbak = X;
                                                    Ybak = Y;
                                                    X = loadx2;
                                                    Y = loady2;

                                                    X = Xbak;
                                                    Y = Ybak;
                                                    if (check_if_point_on_line_segment(sh_node[sh_edge[jk].node1].x, sh_node[sh_edge[jk].node1].y, sh_node[sh_edge[jk].node2].x, sh_node[sh_edge[jk].node2].y, loadx2, loady2)) {
                                                        //load can be assigned to the edge
                                                        sh_load[i].k = shield_no + hole_no + wall_no + jk;
                                                    } else {
                                                        sprintf(report_row, "<%f;%f> <%f;%f> [%s] %s%s", milimetryobx(sh_load[i].x1), milimetryoby(sh_load[i].y1),
                                                                milimetryobx(sh_load[i].x2), milimetryoby(sh_load[i].y2),
                                                                (*mVector.pola)[sh_load[i].style + 1].txt, _load_not_associated_, rn);
                                                        strcat(report, report_row);

                                                        break;
                                                    }
                                                }
                                            }
                                        } else //arc
                                        {
                                            if (jk == 17) {
                                                int a = 0;
                                            }
                                            Xbak = X;
                                            Ybak = Y;
                                            X = loadx1;
                                            Y = loady1;

                                            double xs = sh_node[sh_edge[jk].node1].x - sh_node[sh_edge[jk].node2].x;
                                            double ys = sh_node[sh_edge[jk].node1].y - sh_node[sh_edge[jk].node2].y;
                                            double xe = sh_node[sh_edge[jk].node3].x - sh_node[sh_edge[jk].node2].x;
                                            double ye = sh_node[sh_edge[jk].node3].y - sh_node[sh_edge[jk].node2].y;

                                            double r_edge = sqrt(xs * xs + ys * ys);
                                            double kat1_edge = (float) Atan2(ys, xs);
                                            double kat2_edge = (float) Atan2(ye, xe);

                                            X = Xbak;
                                            Y = Ybak;
                                            if (check_if_point_on_arc_segment(sh_node[sh_edge[jk].node2].x, sh_node[sh_edge[jk].node2].y, r_edge, kat1_edge, kat2_edge, loadx1, loady1)) {
                                                sh_load[i].body = 3; //zone
                                                sh_load[i].body_no = j;
                                                sh_load[i].point_body = 3; //zone
                                                sh_load[i].point_body_no = j;
                                                sh_load[i].point_edge_no = jk;

                                                zone_property[j].load++;

                                                //checking if point exists
                                                int kk;
                                                for (kk = 0; kk < sh_node_no; kk++) {
                                                    if ((Check_if_Equal(sh_node[kk].x, loadx1) == TRUE) && (Check_if_Equal(sh_node[kk].y, loady1) == TRUE)) //point in point
                                                        break;
                                                }
                                                if (kk < sh_node_no) break;  //no needs to add new point

                                                //adding point
                                                //embed_node_edge(i, geo_units_factor);
                                                sh_node[sh_node_no].x = loadx1;
                                                sh_node[sh_node_no].y = loady1;
                                                sh_node[sh_node_no].d = sh_node[sh_edge[jk].node2].d; //setting the division for FE

                                                //spitting edge
                                                //SH_EDGE sh_edge_buf;
                                                memmove(&sh_edge_buf, &sh_edge[jk], sizeof(SH_EDGE));

                                                sh_edge[jk].node3 = sh_node_no;
                                                //calculate midpoint
                                                double dx, dy, kat1, kat2, r, a1, a2;

                                                dx = sh_node[sh_edge[jk].node1].x - sh_node[sh_edge[jk].node2].x;
                                                dy = sh_node[sh_edge[jk].node1].y - sh_node[sh_edge[jk].node2].y;
                                                //if (Check_if_Equal(dx,0.)) kat1=Pi_;
                                                //else
                                                kat1 = atan2(dy, dx);

                                                dx = loadx1 - sh_node[sh_edge[jk].node2].x;
                                                dy = loady1 - sh_node[sh_edge[jk].node2].y;
                                                //if (Check_if_Equal(dx,0.)) kat2=Pi_;
                                                //else
                                                kat2 = atan2(dy, dx);

                                                a1 = Angle_Normal(kat1);
                                                a2 = Angle_Normal(kat2);
                                                r = sqrt(dx * dx + dy * dy);
                                                if (a1 > a2) a2 += Pi2;
                                                sh_edge[jk].xm = (float) (sh_node[sh_edge[jk].node2].x + r * cos((a1 + a2) / 2.));
                                                sh_edge[jk].ym = (float) (sh_node[sh_edge[jk].node2].y + r * sin((a1 + a2) / 2.));

                                                //new segment
                                                sh_edge_buf.node1 = sh_node_no;

                                                a1 = a2;
                                                dx = sh_node[sh_edge_buf.node3].x - sh_node[sh_edge[jk].node2].x;
                                                dy = sh_node[sh_edge_buf.node3].y - sh_node[sh_edge[jk].node2].y;
                                                //if (Check_if_Equal(dx,0.)) kat2=Pi_;
                                                //else
                                                kat2 = atan2(dy, dx);
                                                a2 = Angle_Normal(kat2);
                                                if (a1 > a2) a2 += Pi2;
                                                sh_edge_buf.xm = (float) (sh_node[sh_edge[jk].node2].x + r * cos((a1 + a2) / 2.));
                                                sh_edge_buf.ym = (float) (sh_node[sh_edge[jk].node2].y + r * sin((a1 + a2) / 2.));

                                                //node 2 is common for both parts
                                                add_node_sh();

                                                //inserting sh_edge_buf
                                                insert_sh_edge(jk + 1, &sh_edge_buf, 0, 0, 0, 0, 0, 0, FALSE);  //sh_edge[jk] stays, was already modified, loadx1, loady1 is split point

                                                found = TRUE;

                                                break;
                                            }
                                        }
                                    }
                                }
                            }

                            // trying to embed point on curve, for example:   -> this doesn't work:  Point{2} In Curve{2};
                            // so instead of inserting new point, the segment is divided into 2, it means new sh_edge must be inserted after effected sh_edge
                            // then all xxx_property[i].first_edge and .last edge must be shifted by 1 if .first_edge > effected sh_edge
                            if (found==FALSE) //not found in holes
                            {
                                for (int jk = zone_property[j].first_edge; jk < zone_property[j].last_edge; jk++)
                                {
                                    double loadx1, loady1, loadx2, loady2;
                                    loadx1 = sh_load[i].x1;
                                    loady1 = sh_load[i].y1;
                                    loadx2 = sh_load[i].x2;
                                    loady2 = sh_load[i].y2;
                                    if (sh_edge[jk].type == 0)  //line
                                    {
                                        Xbak = X;
                                        Ybak = Y;
                                        X = loadx1;
                                        Y = loady1;

                                        X = Xbak;
                                        Y = Ybak;
                                        if (check_if_point_on_line_segment(sh_node[sh_edge[jk].node1].x, sh_node[sh_edge[jk].node1].y, sh_node[sh_edge[jk].node2].x, sh_node[sh_edge[jk].node2].y, loadx1, loady1)) {
                                            sh_load[i].body = 3; //zone
                                            sh_load[i].body_no = j;
                                            sh_load[i].point_body = 3; //zone
                                            sh_load[i].point_body_no = j;
                                            sh_load[i].point_edge_no = jk;

                                            zone_property[j].load++;

                                            //checking if point exists
                                            int kk;
                                            for (kk = 0; kk < sh_node_no; kk++) {
                                                if ((Check_if_Equal(sh_node[kk].x, loadx1) == TRUE) && (Check_if_Equal(sh_node[kk].y, loady1) == TRUE)) //point in point
                                                    break;
                                            }
                                            if (sh_load[i].type == 0)  //point load, so extra point is needed if necessary
                                            {
                                                if (kk == sh_node_no) //needs to add new point
                                                {
                                                    //adding point
                                                    //embed_node_edge(i, geo_units_factor);
                                                    sh_node[sh_node_no].x = loadx1;
                                                    sh_node[sh_node_no].y = loady1;
                                                    sh_node[sh_node_no].d = sh_node[sh_edge[jk].node2].d; //setting the division for FE

                                                    double xx01 = sh_node[sh_edge[jk].node1].x;
                                                    double yy01 = sh_node[sh_edge[jk].node1].y;
                                                    double xx02 = sh_node[sh_edge[jk].node2].x;
                                                    double yy02 = sh_node[sh_edge[jk].node2].y;

                                                    //spitting edge
                                                    //SH_EDGE sh_edge_buf;
                                                    memmove(&sh_edge_buf, &sh_edge[jk], sizeof(SH_EDGE));

                                                    sh_edge[jk].node2 = sh_node_no;
                                                    //calculate midpoint
                                                    sh_edge[jk].xm = (float) (sh_node[sh_edge[jk].node1].x + loadx1) / 2.0f;
                                                    sh_edge[jk].ym = (float) (sh_node[sh_edge[jk].node1].y + loady1) / 2.0f;

                                                    sh_edge_buf.node1 = sh_node_no;
                                                    sh_edge_buf.xm = (float) (sh_node[sh_edge[jk].node2].x + loadx1) / 2.0f;
                                                    sh_edge_buf.ym = (float) (sh_node[sh_edge[jk].node2].y + loady1) / 2.0f;
                                                    add_node_sh();

                                                    //inserting sh_edge_buf
                                                    insert_sh_edge(jk + 1, &sh_edge_buf, xx01, yy01, xx02, yy02, loadx1, loady1, TRUE);  //sh_edge[jk] stays, was already modified, loadx1, loady1 is split point

                                                    found = TRUE;
                                                    break;
                                                }
                                            } else if (sh_load[i].type == 2) //linear uniformly or trapezoidally distributed load
                                            {
                                                //checking the second point to make sure the load lays on edge
                                                Xbak = X;
                                                Ybak = Y;
                                                X = loadx2;
                                                Y = loady2;

                                                X = Xbak;
                                                Y = Ybak;
                                                if (check_if_point_on_line_segment(sh_node[sh_edge[jk].node1].x, sh_node[sh_edge[jk].node1].y, sh_node[sh_edge[jk].node2].x, sh_node[sh_edge[jk].node2].y, loadx2, loady2)) {
                                                    //load can be assigned to the edge
                                                    sh_load[i].k = shield_no + hole_no + wall_no + jk;

                                                    found = TRUE;
                                                } else {
                                                    sprintf(report_row, "<%f;%f> <%f;%f> [%s] %s%s", milimetryobx(sh_load[i].x1), milimetryoby(sh_load[i].y1),
                                                            milimetryobx(sh_load[i].x2), milimetryoby(sh_load[i].y2),
                                                            (*mVector.pola)[sh_load[i].style + 1].txt, _load_not_associated_, rn);
                                                    strcat(report, report_row);

                                                    break;
                                                }
                                            }
                                        }
                                    } else //arc
                                    {
                                        if (jk == 17) {
                                            int a = 0;
                                        }
                                        Xbak = X;
                                        Ybak = Y;
                                        X = loadx1;
                                        Y = loady1;

                                        double xs = sh_node[sh_edge[jk].node1].x - sh_node[sh_edge[jk].node2].x;
                                        double ys = sh_node[sh_edge[jk].node1].y - sh_node[sh_edge[jk].node2].y;
                                        double xe = sh_node[sh_edge[jk].node3].x - sh_node[sh_edge[jk].node2].x;
                                        double ye = sh_node[sh_edge[jk].node3].y - sh_node[sh_edge[jk].node2].y;

                                        double r_edge = sqrt(xs * xs + ys * ys);
                                        double kat1_edge = (float) Atan2(ys, xs);
                                        double kat2_edge = (float) Atan2(ye, xe);

                                        X = Xbak;
                                        Y = Ybak;
                                        if (check_if_point_on_arc_segment(sh_node[sh_edge[jk].node2].x, sh_node[sh_edge[jk].node2].y, r_edge, kat1_edge, kat2_edge, loadx1, loady1)) {
                                            sh_load[i].body = 3; //zone
                                            sh_load[i].body_no = j;
                                            sh_load[i].point_body = 3; //zone
                                            sh_load[i].point_body_no = j;
                                            sh_load[i].point_edge_no = jk;

                                            zone_property[j].load++;

                                            //checking if point exists
                                            int kk;
                                            for (kk = 0; kk < sh_node_no; kk++) {
                                                if ((Check_if_Equal(sh_node[kk].x, loadx1) == TRUE) && (Check_if_Equal(sh_node[kk].y, loady1) == TRUE)) //point in point
                                                    break;
                                            }
                                            if (kk < sh_node_no) break;  //no needs to add new point

                                            //adding point
                                            //embed_node_edge(i, geo_units_factor);
                                            sh_node[sh_node_no].x = loadx1;
                                            sh_node[sh_node_no].y = loady1;
                                            sh_node[sh_node_no].d = sh_node[sh_edge[jk].node2].d; //setting the division for FE

                                            //spitting edge
                                            //SH_EDGE sh_edge_buf;
                                            memmove(&sh_edge_buf, &sh_edge[jk], sizeof(SH_EDGE));

                                            sh_edge[jk].node3 = sh_node_no;
                                            //calculate midpoint
                                            double dx, dy, kat1, kat2, r, a1, a2;

                                            dx = sh_node[sh_edge[jk].node1].x - sh_node[sh_edge[jk].node2].x;
                                            dy = sh_node[sh_edge[jk].node1].y - sh_node[sh_edge[jk].node2].y;
                                            //if (Check_if_Equal(dx,0.)) kat1=Pi_;
                                            //else
                                            kat1 = atan2(dy, dx);

                                            dx = loadx1 - sh_node[sh_edge[jk].node2].x;
                                            dy = loady1 - sh_node[sh_edge[jk].node2].y;
                                            //if (Check_if_Equal(dx,0.)) kat2=Pi_;
                                            //else
                                            kat2 = atan2(dy, dx);

                                            a1 = Angle_Normal(kat1);
                                            a2 = Angle_Normal(kat2);
                                            r = sqrt(dx * dx + dy * dy);
                                            if (a1 > a2) a2 += Pi2;
                                            sh_edge[jk].xm = (float) (sh_node[sh_edge[jk].node2].x + r * cos((a1 + a2) / 2.));
                                            sh_edge[jk].ym = (float) (sh_node[sh_edge[jk].node2].y + r * sin((a1 + a2) / 2.));

                                            //new segment
                                            sh_edge_buf.node1 = sh_node_no;

                                            a1 = a2;
                                            dx = sh_node[sh_edge_buf.node3].x - sh_node[sh_edge[jk].node2].x;
                                            dy = sh_node[sh_edge_buf.node3].y - sh_node[sh_edge[jk].node2].y;
                                            //if (Check_if_Equal(dx,0.)) kat2=Pi_;
                                            //else
                                            kat2 = atan2(dy, dx);
                                            a2 = Angle_Normal(kat2);
                                            if (a1 > a2) a2 += Pi2;
                                            sh_edge_buf.xm = (float) (sh_node[sh_edge[jk].node2].x + r * cos((a1 + a2) / 2.));
                                            sh_edge_buf.ym = (float) (sh_node[sh_edge[jk].node2].y + r * sin((a1 + a2) / 2.));

                                            //node 2 is common for both parts
                                            add_node_sh();

                                            //inserting sh_edge_buf
                                            insert_sh_edge(jk + 1, &sh_edge_buf, 0, 0, 0, 0, 0, 0, FALSE);  //sh_edge[jk] stays, was already modified, loadx1, loady1 is split point

                                            found = TRUE;

                                            break;
                                        }
                                    }
                                }
                            }
                            if (found==FALSE)
                            {
                                ADPB = (char *) zone_property[j].adr;
                                ADKB = ADPB + sizeof(NAGLOWEK) + zone_property[j].adr->n - 1;
                                zmien_atrybut_for_objects(ADPB, ADKB, Aoblok, Ablok, Blinia | Bluk);
                                ret1 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x1, df_y1,
                                                       &s_hatch_param,
                                                       1, 0, 0, 0, 0, 0);
                                if (sh_load[i].type > 0)   //two points of distribution or range
                                    ret2 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x2, df_y2,
                                                           &s_hatch_param,
                                                           1, 0, 0, 0, 0, 0);
                                else ret2 = ret1;  //same point
                                zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
                                if ((ret1 == 1) && (ret2 == 1))
                                {
                                    //check if wall belongs to zone or shield
                                    sh_load[i].body = 3; //zone
                                    sh_load[i].body_no = j;
                                    sh_load[i].point_body = 3; //zone
                                    sh_load[i].point_body_no = j;
                                    sh_load[i].point_edge_no = 0;   //not installed on edge

                                    if (sh_load[i].type == 0)  //point load
                                        embed_node(i, geo_units_factor);
                                    else if (sh_load[i].type == 2)  //distributed load
                                    {
                                        ////Point(6) = {0.02, 0.12, 0, lc};
                                        ////Point(7) = {0.04, 0.18, 0, lc};
                                        ////Line(5) = {6, 7};
                                        ////Curve{5} In Surface{1};
                                        embed_nodes_edge(i, 3, j, zone_property[j].body_number, geo_units_factor);
                                    }

                                    zone_property[j].load++;
                                    break;
                                }
                            }
                    }
                    //if assigned to zone, won't be assigned to shield
                    if (j == zone_no)  //not found in zones
                    {
                        ////marking shield
                        for (j = 0; j < shield_no; j++)
                        {
                            BOOL found=FALSE;
                            //checking holes in the zone
                            for (int jh = 0; jh < hole_no; jh++)
                            {
                                if ((hole_property[jh].ps == 0) && (hole_property[jh].pn == j)) //shield and this shield
                                {
                                    for (int jk = hole_property[jh].first_edge; jk < hole_property[jh].last_edge; jk++)
                                    {
                                        double loadx1, loady1, loadx2, loady2;
                                        loadx1 = sh_load[i].x1;
                                        loady1 = sh_load[i].y1;
                                        loadx2 = sh_load[i].x2;
                                        loady2 = sh_load[i].y2;
                                        if (sh_edge[jk].type == 0)  //line
                                        {
                                            Xbak = X;
                                            Ybak = Y;
                                            X = loadx1;
                                            Y = loady1;

                                            X = Xbak;
                                            Y = Ybak;
                                            if (check_if_point_on_line_segment(sh_node[sh_edge[jk].node1].x, sh_node[sh_edge[jk].node1].y, sh_node[sh_edge[jk].node2].x, sh_node[sh_edge[jk].node2].y, loadx1, loady1)) {
                                                sh_load[i].body = 0; //shield
                                                sh_load[i].body_no = j;
                                                sh_load[i].point_body = 0; //shield
                                                sh_load[i].point_body_no = j;
                                                sh_load[i].point_edge_no = jk;

                                                shield_property[j].load++;

                                                //checking if point exists
                                                int kk;
                                                for (kk = 0; kk < sh_node_no; kk++) {
                                                    if ((Check_if_Equal(sh_node[kk].x, loadx1) == TRUE) && (Check_if_Equal(sh_node[kk].y, loady1) == TRUE)) //point in point
                                                        break;
                                                }
                                                if (sh_load[i].type == 0)  //point load, so extra point is needed if necessary
                                                {
                                                    if (kk == sh_node_no) //needs to add new point
                                                    {
                                                        //adding point
                                                        //embed_node_edge(i, geo_units_factor);
                                                        sh_node[sh_node_no].x = loadx1;
                                                        sh_node[sh_node_no].y = loady1;
                                                        sh_node[sh_node_no].d = sh_node[sh_edge[jk].node2].d; //setting the division for FE

                                                        double xx01 = sh_node[sh_edge[jk].node1].x;
                                                        double yy01 = sh_node[sh_edge[jk].node1].y;
                                                        double xx02 = sh_node[sh_edge[jk].node2].x;
                                                        double yy02 = sh_node[sh_edge[jk].node2].y;

                                                        //spitting edge
                                                        //SH_EDGE sh_edge_buf;
                                                        memmove(&sh_edge_buf, &sh_edge[jk], sizeof(SH_EDGE));

                                                        sh_edge[jk].node2 = sh_node_no;
                                                        //calculate midpoint
                                                        sh_edge[jk].xm = (float) (sh_node[sh_edge[jk].node1].x + loadx1) / 2.0f;
                                                        sh_edge[jk].ym = (float) (sh_node[sh_edge[jk].node1].y + loady1) / 2.0f;

                                                        sh_edge_buf.node1 = sh_node_no;
                                                        sh_edge_buf.xm = (float) (sh_node[sh_edge[jk].node2].x + loadx1) / 2.0f;
                                                        sh_edge_buf.ym = (float) (sh_node[sh_edge[jk].node2].y + loady1) / 2.0f;
                                                        add_node_sh();

                                                        //inserting sh_edge_buf
                                                        insert_sh_edge(jk + 1, &sh_edge_buf, xx01, yy01, xx02, yy02, loadx1, loady1, TRUE);  //sh_edge[jk] stays, was already modified, loadx1, loady1 is split point

                                                        found = TRUE;
                                                        break;
                                                    }
                                                } else if (sh_load[i].type == 2) //linear uniformly or trapezoidally distributed load
                                                {
                                                    //checking the second point to make sure the load lays on edge
                                                    Xbak = X;
                                                    Ybak = Y;
                                                    X = loadx2;
                                                    Y = loady2;

                                                    X = Xbak;
                                                    Y = Ybak;
                                                    if (check_if_point_on_line_segment(sh_node[sh_edge[jk].node1].x, sh_node[sh_edge[jk].node1].y, sh_node[sh_edge[jk].node2].x, sh_node[sh_edge[jk].node2].y, loadx2, loady2)) {
                                                        //load can be assigned to the edge
                                                        sh_load[i].k = shield_no + hole_no + wall_no + jk;
                                                    } else {
                                                        sprintf(report_row, "<%f;%f> <%f;%f> [%s] %s%s", milimetryobx(sh_load[i].x1), milimetryoby(sh_load[i].y1),
                                                                milimetryobx(sh_load[i].x2), milimetryoby(sh_load[i].y2),
                                                                (*mVector.pola)[sh_load[i].style + 1].txt, _load_not_associated_, rn);
                                                        strcat(report, report_row);

                                                        break;
                                                    }
                                                }
                                            }
                                        } else //arc
                                        {
                                            if (jk == 17) {
                                                int a = 0;
                                            }
                                            Xbak = X;
                                            Ybak = Y;
                                            X = loadx1;
                                            Y = loady1;

                                            double xs = sh_node[sh_edge[jk].node1].x - sh_node[sh_edge[jk].node2].x;
                                            double ys = sh_node[sh_edge[jk].node1].y - sh_node[sh_edge[jk].node2].y;
                                            double xe = sh_node[sh_edge[jk].node3].x - sh_node[sh_edge[jk].node2].x;
                                            double ye = sh_node[sh_edge[jk].node3].y - sh_node[sh_edge[jk].node2].y;

                                            double r_edge = sqrt(xs * xs + ys * ys);
                                            double kat1_edge = (float) Atan2(ys, xs);
                                            double kat2_edge = (float) Atan2(ye, xe);

                                            X = Xbak;
                                            Y = Ybak;
                                            if (check_if_point_on_arc_segment(sh_node[sh_edge[jk].node2].x, sh_node[sh_edge[jk].node2].y, r_edge, kat1_edge, kat2_edge, loadx1, loady1)) {
                                                sh_load[i].body = 0; //shield
                                                sh_load[i].body_no = j;
                                                sh_load[i].point_body = 0; //shield
                                                sh_load[i].point_body_no = j;
                                                sh_load[i].point_edge_no = jk;

                                                zone_property[j].load++;

                                                //checking if point exists
                                                int kk;
                                                for (kk = 0; kk < sh_node_no; kk++) {
                                                    if ((Check_if_Equal(sh_node[kk].x, loadx1) == TRUE) && (Check_if_Equal(sh_node[kk].y, loady1) == TRUE)) //point in point
                                                        break;
                                                }
                                                if (kk < sh_node_no) break;  //no needs to add new point

                                                //adding point
                                                //embed_node_edge(i, geo_units_factor);
                                                sh_node[sh_node_no].x = loadx1;
                                                sh_node[sh_node_no].y = loady1;
                                                sh_node[sh_node_no].d = sh_node[sh_edge[jk].node2].d; //setting the division for FE

                                                //spitting edge
                                                //SH_EDGE sh_edge_buf;
                                                memmove(&sh_edge_buf, &sh_edge[jk], sizeof(SH_EDGE));

                                                sh_edge[jk].node3 = sh_node_no;
                                                //calculate midpoint
                                                double dx, dy, kat1, kat2, r, a1, a2;

                                                dx = sh_node[sh_edge[jk].node1].x - sh_node[sh_edge[jk].node2].x;
                                                dy = sh_node[sh_edge[jk].node1].y - sh_node[sh_edge[jk].node2].y;
                                                //if (Check_if_Equal(dx,0.)) kat1=Pi_;
                                                //else
                                                kat1 = atan2(dy, dx);

                                                dx = loadx1 - sh_node[sh_edge[jk].node2].x;
                                                dy = loady1 - sh_node[sh_edge[jk].node2].y;
                                                //if (Check_if_Equal(dx,0.)) kat2=Pi_;
                                                //else
                                                kat2 = atan2(dy, dx);

                                                a1 = Angle_Normal(kat1);
                                                a2 = Angle_Normal(kat2);
                                                r = sqrt(dx * dx + dy * dy);
                                                if (a1 > a2) a2 += Pi2;
                                                sh_edge[jk].xm = (float) (sh_node[sh_edge[jk].node2].x + r * cos((a1 + a2) / 2.));
                                                sh_edge[jk].ym = (float) (sh_node[sh_edge[jk].node2].y + r * sin((a1 + a2) / 2.));

                                                //new segment
                                                sh_edge_buf.node1 = sh_node_no;

                                                a1 = a2;
                                                dx = sh_node[sh_edge_buf.node3].x - sh_node[sh_edge[jk].node2].x;
                                                dy = sh_node[sh_edge_buf.node3].y - sh_node[sh_edge[jk].node2].y;
                                                //if (Check_if_Equal(dx,0.)) kat2=Pi_;
                                                //else
                                                kat2 = atan2(dy, dx);
                                                a2 = Angle_Normal(kat2);
                                                if (a1 > a2) a2 += Pi2;
                                                sh_edge_buf.xm = (float) (sh_node[sh_edge[jk].node2].x + r * cos((a1 + a2) / 2.));
                                                sh_edge_buf.ym = (float) (sh_node[sh_edge[jk].node2].y + r * sin((a1 + a2) / 2.));

                                                //node 2 is common for both parts
                                                add_node_sh();

                                                //inserting sh_edge_buf
                                                insert_sh_edge(jk + 1, &sh_edge_buf, 0, 0, 0, 0, 0, 0, FALSE);  //sh_edge[jk] stays, was already modified, loadx1, loady1 is split point

                                                found = TRUE;

                                                break;
                                            }
                                        }
                                    }
                                }
                            }

                            // trying to embed point on curve, for example:   -> this doesn't work:  Point{2} In Curve{2};
                            // so instead of inserting new point, the segment is divided into 2, it means new sh_edge must be inserted after effected sh_edge
                            // then all xxx_property[i].first_edge and .last edge must be shifted by 1 if .first_edge > effected sh_edge
                            if (found==FALSE) //not found in holes
                            {
                                for (int jk = shield_property[j].first_edge; jk < shield_property[j].last_edge; jk++)
                                {
                                    double loadx1, loady1, loadx2, loady2;
                                    loadx1 = sh_load[i].x1;
                                    loady1 = sh_load[i].y1;
                                    loadx2 = sh_load[i].x2;
                                    loady2 = sh_load[i].y2;
                                    if (sh_edge[jk].type == 0)  //line
                                    {
                                        Xbak = X;
                                        Ybak = Y;
                                        X = loadx1;
                                        Y = loady1;

                                        X = Xbak;
                                        Y = Ybak;

                                        if (check_if_point_on_line_segment(sh_node[sh_edge[jk].node1].x, sh_node[sh_edge[jk].node1].y, sh_node[sh_edge[jk].node2].x, sh_node[sh_edge[jk].node2].y, loadx1, loady1))
                                        {
                                            sh_load[i].body = 0; //shield
                                            sh_load[i].body_no = j;
                                            sh_load[i].point_body = 0; //shield
                                            sh_load[i].point_body_no = j;
                                            sh_load[i].point_edge_no = jk;

                                            shield_property[j].load++;

                                            //checking if point 1 exists
                                            int kk;
                                            for (kk = 0; kk < sh_node_no; kk++) {
                                                if ((Check_if_Equal(sh_node[kk].x, loadx1) == TRUE) && (Check_if_Equal(sh_node[kk].y, loady1) == TRUE)) //point in point
                                                    break;
                                            }
                                            if (sh_load[i].type == 0)  //point load, so extra point is needed if necessary
                                            {
                                                if (kk == sh_node_no) //needs to add new point
                                                {
                                                    //adding point
                                                    //embed_node_edge(i, geo_units_factor);
                                                    sh_node[sh_node_no].x = loadx1;
                                                    sh_node[sh_node_no].y = loady1;
                                                    sh_node[sh_node_no].d = sh_node[sh_edge[jk].node2].d; //setting the division for FE

                                                    double xx01=sh_node[sh_edge[jk].node1].x;
                                                    double yy01=sh_node[sh_edge[jk].node1].y;
                                                    double xx02=sh_node[sh_edge[jk].node2].x;
                                                    double yy02=sh_node[sh_edge[jk].node2].y;

                                                    //spitting edge
                                                    //SH_EDGE sh_edge_buf;
                                                    memmove(&sh_edge_buf, &sh_edge[jk], sizeof(SH_EDGE));

                                                    sh_edge[jk].node2 = sh_node_no;
                                                    //calculate midpoint
                                                    sh_edge[jk].xm = (float) (sh_node[sh_edge[jk].node1].x + loadx1) / 2.0f;
                                                    sh_edge[jk].ym = (float) (sh_node[sh_edge[jk].node1].y + loady1) / 2.0f;

                                                    sh_edge_buf.node1 = sh_node_no;
                                                    sh_edge_buf.xm = (float) (sh_node[sh_edge[jk].node2].x + loadx1) / 2.0f;
                                                    sh_edge_buf.ym = (float) (sh_node[sh_edge[jk].node2].y + loady1) / 2.0f;
                                                    add_node_sh();

                                                    //inserting sh_edge_buf
                                                    insert_sh_edge(jk + 1, &sh_edge_buf, xx01, yy01, xx02, yy02, loadx1, loady1, TRUE);  //sh_edge[jk] stays, was already modified, loadx1, loady1 is split point

                                                    found = TRUE;
                                                    break;
                                                }
                                            } else if (sh_load[i].type == 2) //linear uniformly or trapezoidally distributed load
                                            {
                                                //checking the second point to make sure the load lays on edge
                                                Xbak = X;
                                                Ybak = Y;
                                                X = loadx2;
                                                Y = loady2;

                                                X = Xbak;
                                                Y = Ybak;
                                                if (check_if_point_on_line_segment(sh_node[sh_edge[jk].node1].x, sh_node[sh_edge[jk].node1].y, sh_node[sh_edge[jk].node2].x, sh_node[sh_edge[jk].node2].y, loadx2, loady2))
                                                    {
                                                    //load can be assigned to the edge
                                                    sh_load[i].k = jk;
                                                    found = TRUE;
                                                    break;
                                                } else {
                                                    /*
                                                    sprintf(report_row, "<%f;%f> <%f;%f> [%s] %s%s", milimetryobx(sh_load[i].x1), milimetryoby(sh_load[i].y1),
                                                            milimetryobx(sh_load[i].x2), milimetryoby(sh_load[i].y2),
                                                            (*mVector.pola)[sh_load[i].style + 1].txt, _load_not_associated_,rn);
                                                    strcat(report, report_row);

                                                    break;
                                                     */
                                                }
                                            }
                                        }
                                    } else //arc
                                    {
                                        if (jk == 17) {
                                            int a = 0;
                                        }
                                        Xbak = X;
                                        Ybak = Y;
                                        X = loadx1;
                                        Y = loady1;

                                        double xs=sh_node[sh_edge[jk].node1].x-sh_node[sh_edge[jk].node2].x;
                                        double ys=sh_node[sh_edge[jk].node1].y-sh_node[sh_edge[jk].node2].y;
                                        double xe=sh_node[sh_edge[jk].node3].x-sh_node[sh_edge[jk].node2].x;
                                        double ye=sh_node[sh_edge[jk].node3].y-sh_node[sh_edge[jk].node2].y;

                                        double r_edge=sqrt(xs*xs+ys*ys);
                                        double kat1_edge = (float)Atan2(ys, xs);
                                        double kat2_edge = (float)Atan2(ye, xe);

                                        X = Xbak;
                                        Y = Ybak;

                                        if (check_if_point_on_arc_segment(sh_node[sh_edge[jk].node2].x, sh_node[sh_edge[jk].node2].y, r_edge, kat1_edge, kat2_edge, loadx1, loady1))
                                            {
                                            sh_load[i].body = 0; //shield
                                            sh_load[i].body_no = j;
                                            sh_load[i].point_body = 0; //shield
                                            sh_load[i].point_body_no = j;
                                            sh_load[i].point_edge_no = jk;

                                            shield_property[j].load++;

                                            //checking if point exists
                                            int kk;
                                            for (kk = 0; kk < sh_node_no; kk++) {
                                                if ((Check_if_Equal(sh_node[kk].x, loadx1) == TRUE) && (Check_if_Equal(sh_node[kk].y, loady1) == TRUE)) //point in point
                                                    break;
                                            }
                                            if (kk < sh_node_no) break;  //no needs to add new point

                                            //adding point
                                            //embed_node_edge(i, geo_units_factor);
                                            sh_node[sh_node_no].x = loadx1;
                                            sh_node[sh_node_no].y = loady1;
                                            sh_node[sh_node_no].d = sh_node[sh_edge[jk].node2].d; //setting the division for FE

                                            //spitting edge
                                            //SH_EDGE sh_edge_buf;
                                            memmove(&sh_edge_buf, &sh_edge[jk], sizeof(SH_EDGE));

                                            sh_edge[jk].node3 = sh_node_no;
                                            //calculate midpoint
                                            double dx, dy, kat1, kat2, r, a1, a2;

                                            dx = sh_node[sh_edge[jk].node1].x - sh_node[sh_edge[jk].node2].x;
                                            dy = sh_node[sh_edge[jk].node1].y - sh_node[sh_edge[jk].node2].y;
                                            //if (Check_if_Equal(dx,0.)) kat1=Pi_;
                                            //else
                                            kat1 = atan2(dy, dx);

                                            dx = loadx1 - sh_node[sh_edge[jk].node2].x;
                                            dy = loady1 - sh_node[sh_edge[jk].node2].y;
                                            //if (Check_if_Equal(dx,0.)) kat2=Pi_;
                                            //else
                                            kat2 = atan2(dy, dx);

                                            a1 = Angle_Normal(kat1);
                                            a2 = Angle_Normal(kat2);
                                            r = sqrt(dx * dx + dy * dy);
                                            if (a1 > a2) a2 += Pi2;
                                            sh_edge[jk].xm = (float) (sh_node[sh_edge[jk].node2].x + r * cos((a1 + a2) / 2.));
                                            sh_edge[jk].ym = (float) (sh_node[sh_edge[jk].node2].y + r * sin((a1 + a2) / 2.));

                                            //new segment
                                            sh_edge_buf.node1 = sh_node_no;

                                            a1 = a2;
                                            dx = sh_node[sh_edge_buf.node3].x - sh_node[sh_edge[jk].node2].x;
                                            dy = sh_node[sh_edge_buf.node3].y - sh_node[sh_edge[jk].node2].y;
                                            //if (Check_if_Equal(dx,0.)) kat2=Pi_;
                                            //else
                                            kat2 = atan2(dy, dx);
                                            a2 = Angle_Normal(kat2);
                                            if (a1 > a2) a2 += Pi2;
                                            sh_edge_buf.xm = (float) (sh_node[sh_edge[jk].node2].x + r * cos((a1 + a2) / 2.));
                                            sh_edge_buf.ym = (float) (sh_node[sh_edge[jk].node2].y + r * sin((a1 + a2) / 2.));

                                            //node 2 is common for both parts
                                            add_node_sh();

                                            //inserting sh_edge_buf
                                            insert_sh_edge(jk + 1, &sh_edge_buf, 0, 0, 0, 0, 0, 0, FALSE);  //sh_edge[jk] stays, was already modified, loadx1, loady1 is split point

                                            found = TRUE;

                                            break;
                                        }
                                    }
                                }
                            }
                            if (found==FALSE)
                            {
                                ADPB = (char *) shield_property[j].adr;
                                ADKB = ADPB + sizeof(NAGLOWEK) + shield_property[j].adr->n - 1;
                                zmien_atrybut_for_objects(ADPB, ADKB, Aoblok, Ablok, Blinia | Bluk);
                                ret1 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x1, df_y1,
                                                       &s_hatch_param, 1, 0, 0, 0, 0, 0);
                                if (sh_load[i].type > 0)  //two points of distribution or range
                                    ret2 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x2, df_y2,
                                                           &s_hatch_param, 1, 0, 0, 0, 0, 0);
                                else ret2 = ret1;  //same point
                                zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);

                                if ((ret1 == 1) && (ret2 == 1))
                                {
                                    sh_load[i].body = 0; //shield
                                    sh_load[i].body_no = j;
                                    sh_load[i].point_body = 0; //shield
                                    sh_load[i].point_body_no = j;
                                    sh_load[i].point_edge_no = 0;  //not installed on edge

                                    if (sh_load[i].type == 0)  //point load
                                        embed_node(i, geo_units_factor);

                                    else if (sh_load[i].type == 2)  //distributed load
                                    {
                                        ////Point(6) = {0.02, 0.12, 0, lc};
                                        ////Point(7) = {0.04, 0.18, 0, lc};
                                        ////Line(5) = {6, 7};
                                        ////Curve{5} In Surface{1};
                                        embed_nodes_edge(i, 0, j , zone_property[j].body_number, geo_units_factor);
                                    }

                                    shield_property[j].load++;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        } //sh_load iteration
    }  //pointwise iteration
    ////load assignement

    ClearErr();
    ClearInfo();

    if (strlen(report) > 0) goto sh_error;

#ifndef LINUX
#ifndef BIT64
    if (Is64BitOperatingSystem() == FALSE)
    {
        ret = ask_question(1, (char*)"", (char*)confirm, (char*)"", (char*)_CANNOT_PROCEED_IN_32BIT_, 12, (char*)_BUY_NEW_COMPUTER_, 11, 1, 62);
        goto sh_error;
    }
#endif
#endif

    if ((sh_load_no == 0) && (gZ == 0)) //no load, no self weight
    {
        sprintf(report_row, "%s%s", _NO_LOAD_ASSIGNED_,rn);
        strcat(report, report_row);
        goto sh_error;
    }

    ////writing Static/alfacad.geo
    //if Static folder doesn't exist

    if (!my_directory_exists(_STATIC_)) {
#ifdef LINUX
        // Create a directory with read, write, and execute permissions for the owner
        // and read and execute permissions for group and others.
        mode_t file_mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
        check = mkdir(_STATIC_, file_mode);
#else
        check = _mkdir(_STATIC_);
#endif

        if (check) {
            sprintf(report_row, "%s %s%s", _cannot_create_folder_, _STATIC_,rn);
            strcat(report, report_row);
        }
    }

    sprintf(params, "%s%s", _STATIC_, _shield_);
    if (!my_directory_exists(params)) {
#ifdef LINUX
        // Create a directory with read, write, and execute permissions for the owner
        // and read and execute permissions for group and others.
        mode_t file_mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
        check = mkdir(params, file_mode);
#else
        check = _mkdir(params);
#endif

        if (check) {
            sprintf(report_row, "%s %s%s%s", _cannot_create_folder_, _STATIC_, _shield_,rn);
            strcat(report, report_row);
        }
    }

    if (!my_directory_exists(_shield_)) {
#ifdef LINUX
        // Create a directory with read, write, and execute permissions for the owner
        // and read and execute permissions for group and others.
        mode_t file_mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
        check = mkdir(_shield_, file_mode);
#else
        check = _mkdir(_shield_);
#endif

        if (check) {
            sprintf(report_row, "%s %s%s", _cannot_create_folder_, _shield_,rn);
            strcat(report, report_row);
        }
    }

    ////deleting old files
    sprintf(params, "%sshield.geo", _STATIC_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%s%sshield.sif", _STATIC_, _shield_);
    if (my_file_exists(params)) unlink(params);
    ////results
    sprintf(params, "%s%sshield.result", _STATIC_, _shield_);
    if (my_file_exists(params)) unlink(params);
    ////results in shield folder due to ElmerSolver error
    sprintf(params, "%sshield_sls.result", _shield_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%sshield_uls.result", _shield_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%sshield_ulslc.result", _shield_);
    if (my_file_exists(params)) unlink(params);
    for (i = 0; i < 28; i++) {
        sprintf(params, "%sshield_ulslc%d.result", _shield_, i);
        if (my_file_exists(params)) unlink(params);
    }
    sprintf(params, "%sshield_slslc.result", _shield_);
    if (my_file_exists(params)) unlink(params);
    for (i = 0; i < 13; i++) {
        sprintf(params, "%sshield_slslc%d.result", _shield_, i);
        if (my_file_exists(params)) unlink(params);
    }
    sprintf(params, "%sshield_qpslslc.result", _shield_);
    if (my_file_exists(params)) unlink(params);
    for (i = 0; i < 8; i++) {
        sprintf(params, "%sshield_qpslslc%d.result", _shield_, i);
        if (my_file_exists(params)) unlink(params);
    }
    ////mesh
    sprintf(params, "%s%smesh.nodes", _STATIC_, _shield_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%s%smesh.header", _STATIC_, _shield_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%s%smesh.elements", _STATIC_, _shield_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%s%smesh.boundary", _STATIC_, _shield_);
    if (my_file_exists(params)) unlink(params);
    ////

    //creating geo file
    sprintf(params, "%sshield.geo", _STATIC_);
    f = fopen(params, "wt");

    if (f == NULL) {
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_RESULTS_FILE_, 12,
                           (char *) "", 11, 1,
                           62);
        no_error = FALSE;
        goto sh_error;
    }

    fprintf(f, "\n// NODES\n");
    //nodes
    for (i = 0; i < sh_node_no; i++) {
        set_decimal_format(par[0], milimetryobx(sh_node[i].x) * geo_units_factor, dim_precision_sh);
        set_decimal_format(par[1], milimetryoby(sh_node[i].y) * geo_units_factor, dim_precision_sh);
        set_decimal_format(par[2], max(milimetryob((double) sh_node[i].d) * geo_units_factor, dxl_min),
                           dim_precision_sh);

        fprintf(f, "Point(%d) = {%s, %s, 0, %s};\n", i + 1, par[0], par[1], par[2]);
    }

    //Embeded points and setting emb_no
    for (i=0; i<sh_node_emb_no; i++)
    {
        set_decimal_format(par[0], milimetryobx(sh_node_emb[i].x) * geo_units_factor, dim_precision_sh);
        set_decimal_format(par[1], milimetryoby(sh_node_emb[i].y) * geo_units_factor, dim_precision_sh);
        set_decimal_format(par[2], max(milimetryob((double) sh_node_emb[i].d) * geo_units_factor, dxl_min),
                           dim_precision_sh);

        fprintf(f, "Point(%d) = {%s, %s, 0, %s};\n", sh_node_no + i + 1, par[0], par[1], par[2]);
        sh_node_emb[i].emb_no=sh_node_no + i + 1;
    }
    //fprintf(f, "Point(32) = {2.500, 2.500, 0, 0.45};\n");

    //Embeded edge points and setting emb_no
    for (i=0; i<sh_node_emb_edge_no; i++)
    {
        set_decimal_format(par[0], milimetryobx(sh_node_emb_edge[i].x) * geo_units_factor, dim_precision_sh);
        set_decimal_format(par[1], milimetryoby(sh_node_emb_edge[i].y) * geo_units_factor, dim_precision_sh);
        set_decimal_format(par[2], max(milimetryob((double) sh_node_emb_edge[i].d) * geo_units_factor, dxl_min),
                           dim_precision_sh);

        fprintf(f, "Point(%d) = {%s, %s, 0, %s};\n", sh_node_no + sh_node_emb_no + i + 1, par[0], par[1], par[2]);
        sh_node_emb_edge[i].emb_no=sh_node_no + sh_node_emb_no + i + 1;
    }

    //edges
    fprintf(f, "\n// EDGES\n");
    k = 0;
    //the first are shields
    fprintf(f, "\n// SHIELDS\n");
    for (i = 0; i < shield_no; i++) {
        for (j = shield_property[i].first_edge; j < shield_property[i].last_edge; j++) {
            if (sh_edge[j].type == 0)  //line
            {
                fprintf(f, "Line(%d) = {%d, %d};\n", k + 1, sh_edge[j].node1 + 1, sh_edge[j].node2 + 1);
                sh_edge[j].k = k;
            } else if (sh_edge[j].type == 1)  //circle
            {
                fprintf(f, "Circle(%d) = {%d, %d, %d};\n", k + 1, sh_edge[j].node1 + 1, sh_edge[j].node2 + 1,
                        sh_edge[j].node3 + 1);
                sh_edge[j].k = k;
            }
            k++;
        }
    }
    //the second are holes
    fprintf(f, "\n// HOLES\n");
    for (i = 0; i < hole_no; i++) {
        for (j = hole_property[i].first_edge; j < hole_property[i].last_edge; j++) {
            if (sh_edge[j].type == 0)  //line
            {
                fprintf(f, "Line(%d) = {%d, %d};\n", k + 1, sh_edge[j].node1 + 1, sh_edge[j].node2 + 1);
                sh_edge[j].k = k;
            } else if (sh_edge[j].type == 1)  //circle
            {
                fprintf(f, "Circle(%d) = {%d, %d, %d};\n", k + 1, sh_edge[j].node1 + 1, sh_edge[j].node2 + 1,
                        sh_edge[j].node3 + 1);
                sh_edge[j].k = k;
            }
            k++;
        }
    }


    //the third are walls
    fprintf(f, "\n// WALLS\n");
    //int first_walls_line=k+1;
    for (i = 0; i < wall_no; i++)
    {
        for (j = wall_property[i].first_edge; j < wall_property[i].last_edge; j++) {
            if (sh_edge[j].type == 0)  //line
            {
                fprintf(f, "Line(%d) = {%d, %d};\n", k + 1, sh_edge[j].node1 + 1, sh_edge[j].node2 + 1);
                sh_edge[j].k = k;
            } else if (sh_edge[j].type == 1)  //circle
            {
                fprintf(f, "Circle(%d) = {%d, %d, %d};\n", k + 1, sh_edge[j].node1 + 1, sh_edge[j].node2 + 1,
                        sh_edge[j].node3 + 1);
                sh_edge[j].k = k;
            }
            k++;
        }
    }
    //int last_walls_line=k+1;


    //the last are zones
    fprintf(f, "\n// ZONES\n");
    for (i = 0; i < zone_no; i++) {
        for (j = zone_property[i].first_edge; j < zone_property[i].last_edge; j++) {
            if (sh_edge[j].type == 0)  //line
            {
                fprintf(f, "Line(%d) = {%d, %d};\n", k + 1, sh_edge[j].node1 + 1, sh_edge[j].node2 + 1);
                sh_edge[j].k = k;
            } else if (sh_edge[j].type == 1)  //circle
            {
                fprintf(f, "Circle(%d) = {%d, %d, %d};\n", k + 1, sh_edge[j].node1 + 1, sh_edge[j].node2 + 1,
                        sh_edge[j].node3 + 1);
                sh_edge[j].k = k;
            }
            k++;
        }
    }

    fprintf(f, "\n// EMBEDED LINES\n");
    for (j = 0; j < sh_edge_emb_no; j++)
    {
        if (sh_edge_emb[j].type == 0)  //line
        {
            fprintf(f, "Line(%d) = {%d, %d};\n", k + 1, sh_edge_emb[j].node1 + 1, sh_edge_emb[j].node2 + 1);
            sh_edge_emb[j].k = k;
        } else if (sh_edge_emb[j].type == 1)  //circle
        {
            fprintf(f, "Circle(%d) = {%d, %d, %d};\n", k + 1, sh_edge_emb[j].node1 + 1, sh_edge_emb[j].node2 + 1, sh_edge_emb[j].node3 + 1);
            sh_edge_emb[j].k = k;
        }
        k++;
    }

    //curves
    fprintf(f, "\n// CURVES\n");
    //the first are holes
    fprintf(f, "\n// HOLES\n");
    for (i = 0; i < hole_no; i++) {
        fprintf(f, "Curve Loop (%d) = {", k + 1);

        for (j = hole_property[i].first_edge; j < hole_property[i].last_edge; j++) {
            if (sh_edge[j].inverted == 0) fprintf(f, "%d", sh_edge[j].k + 1);
            else fprintf(f, "-%d", sh_edge[j].k + 1);
            if (j < (hole_property[i].last_edge - 1)) fprintf(f, ", ");
        }
        fprintf(f, "};\n");
        hole_property[i].k = k;
        k++;
    }

    /*

    //the second are walls - they do not exist in shields
    fprintf(f, "\n// WALLS\n");
    for (i = 0; i < wall_no; i++) {
        fprintf(f, "Curve Loop (%d) = {", k + 1);

        for (j = wall_property[i].first_edge; j < wall_property[i].last_edge; j++) {
            if (sh_edge[j].inverted == 0) fprintf(f, "%d", sh_edge[j].k + 1);
            else fprintf(f, "-%d", sh_edge[j].k + 1);
            if (j < (wall_property[i].last_edge - 1)) fprintf(f, ", ");
        }
        fprintf(f, "};\n");
        wall_property[i].k = k;
        k++;
        fprintf(f, "Plane Surface(%d) = {%d};\n", k + 1, k);

        //Embeding points
        // Embed the point (dimension 0) into the surface (dimension 2)
        // Embed { point_tags } In { surface_tags };
        //fprintf(f, "Point{32} In Surface{%d};\n", k+1);
        for (j=0; j<sh_node_emb_no; j++)
        {
            if ((sh_node_emb[j].body==2) && (sh_node_emb[j].body_no==i)) //is wall and it's this wall
            {
                fprintf(f, "Point{%d} In Surface{%d};\n", sh_node_emb[j].emb_no, k+1);
            }
        }

        k++;
    }

     */

    //zones have to be sorted, so if zone 4 belongs to 3 firstly 4 has to be defined, next 3
    //so we are looking to hole_property[j].ps and hole_property[j].pn
    //and we are sorting after hole_property[j].pn

    //for (i = 0; i < zone_no; i++) printf("%d %d\n",zone_property[i].ps, zone_property[i].pn);
    //printf("---------------\n");
    //qsort(zone_property, zone_no, sizeof(SHIELD_PROPERTY), qsort_by_val);
    //for (i = 0; i < zone_no; i++) printf("%d %d\n",zone_property[i].ps, zone_property[i].pn);
    //printf("---------------\n");


    //the third are zones
    fprintf(f, "\n// ZONES\n");
    for (i = 0; i < zone_no; i++)
    {
        fprintf(f, "Curve Loop (%d) = {", k + 1);

        for (j = zone_property[i].first_edge; j < zone_property[i].last_edge; j++) {
            if (sh_edge[j].inverted == 0) fprintf(f, "%d", sh_edge[j].k + 1);
            else fprintf(f, "-%d", sh_edge[j].k + 1);
            //if (sh_edge[j].inverted == 0) fprintf(f, "%d", j + 1);
            //else fprintf(f, "-%d", j + 1);
            if (j < (zone_property[i].last_edge - 1)) fprintf(f, ", ");
        }
        fprintf(f, "};\n");
        zone_property[i].k = k;
        k++;

        //fprintf(f, "Plane Surface(%d) = {%d", k + 1, k);
        fprintf(f, "Plane Surface(%d) = {%d", k + 1, zone_property[i].k + 1);
        for (j = 0; j < hole_no; j++)
        {
            if ((hole_property[j].ps==3) && (hole_property[j].pn==i))
                fprintf(f, ", %d", hole_property[j].k + 1);
        }
        /*
        for (j = 0; j < wall_no; j++)
        {
            if ((wall_property[j].ps==3) && (wall_property[j].pn==i))
                fprintf(f, ", %d", wall_property[j].k + 1);
        }
         */
        for (j = 0; j < zone_no; j++)  //this is not working, reserved for the future
        {
            if (j!=i)  //doesn't take itself
            {
                if ((zone_property[j].ps == 3) && (zone_property[j].pn == i))
                    fprintf(f, ", %d", zone_property[j].k + 1);
            }
        }
        fprintf(f, "};\n");

        //Embeding points
        // Embed the point (dimension 0) into the surface (dimension 2)
        // Embed { point_tags } In { surface_tags };
        //fprintf(f, "Point{32} In Surface{%d};\n", k+1);
        for (j=0; j<sh_node_emb_no; j++)
        {
            if ((sh_node_emb[j].body==3) && (sh_node_emb[j].body_no==i)) //is zone and it's this zone
            {
                fprintf(f, "Point{%d} In Surface{%d};\n", sh_node_emb[j].emb_no, k+1);
            }
        }

        for (j=0; j<sh_edge_emb_no; j++)
        {
            if ((sh_edge_emb[j].style==3) && (sh_edge_emb[j].number==i)) //is shield and it's this shield
            {
                ////Curve{5} In Surface{1};
                fprintf(f, "Line{%d} In Surface{%d};\n", sh_edge_emb[j].k + 1, k + 1);
                sh_load[sh_edge_emb[j].load_no].k=sh_edge_emb[j].k;
            }
        }

        //Embedding wall lines
        fprintf(f, "\n// WALLS in ZONE\n");
        for (j = 0; j<wall_no; j++)
        {
            if ((wall_property[j].ps==3) && (wall_property[j].pn==i))
            {
                for (int jj=wall_property[j].first_edge; jj<wall_property[j].last_edge; jj++)
                {
                    if (sh_edge[jj].type==0)  //line
                        fprintf(f, "Line{%d} In Surface {%d};\n", jj + 1, k + 1);
                    else
                        fprintf(f, "Circle{%d} In Surface {%d};\n", jj+ 1, k + 1);
                }
            }
        }

        k++;
    }

    //the last are shields
    //the third are zones
    fprintf(f, "\n// SHIELDS\n");
    for (i = 0; i < shield_no; i++) {
        fprintf(f, "Curve Loop (%d) = {", k + 1);

        for (j = shield_property[i].first_edge; j < shield_property[i].last_edge; j++) {
            if (sh_edge[j].inverted == 0) fprintf(f, "%d", sh_edge[j].k + 1);
            else fprintf(f, "-%d", sh_edge[j].k + 1);
            if (j < (shield_property[i].last_edge - 1)) fprintf(f, ", ");
        }
        fprintf(f, "};\n");
        shield_property[i].k = k;
        k++;
        fprintf(f, "Plane Surface(%d) = {%d", k + 1, k);
        for (j = 0; j < hole_no; j++)
        {
            if ((hole_property[j].ps==0) && (hole_property[j].pn==i))
                fprintf(f, ", %d", hole_property[j].k + 1);
        }

        /*
        for (j = 0; j < wall_no; j++)
        {
            if ((wall_property[j].ps==0) && (wall_property[j].pn==i))
                fprintf(f, ", %d", wall_property[j].k + 1);
        }
         */
        for (j = 0; j < zone_no; j++)
        {
            if ((zone_property[j].ps==0) && (zone_property[j].pn==i))
                fprintf(f, ", %d", zone_property[j].k + 1);
        }
        fprintf(f, "};\n");

        //Embeding points
        // Embed the point (dimension 0) into the surface (dimension 2)
        // Embed { point_tags } In { surface_tags };
        //fprintf(f, "Point{32} In Surface{%d};\n", k+1);
        for (j=0; j<sh_node_emb_no; j++)
        {
            if ((sh_node_emb[j].body==0) && (sh_node_emb[j].body_no==i)) //is shield and it's this shield
            {
                fprintf(f, "Point{%d} In Surface{%d};\n", sh_node_emb[j].emb_no, k+1);
            }
        }

        for (j=0; j<sh_edge_emb_no; j++)
        {
            if ((sh_edge_emb[j].style==0) && (sh_edge_emb[j].number==i)) //is shield and it's this shield
            {
                ////Curve{5} In Surface{1};
                fprintf(f, "Line{%d} In Surface{%d};\n", sh_edge_emb[j].k + 1, k + 1);
                sh_load[sh_edge_emb[j].load_no].k=sh_edge_emb[j].k;
            }
        }

        //Embedding wall lines
        fprintf(f, "\n// WALLS in SHIELD\n");
        for (j = 0; j<wall_no; j++)
        {
            if ((wall_property[j].ps==0) && (wall_property[j].pn==i))
            {
                for (int jj=wall_property[j].first_edge; jj<wall_property[j].last_edge; jj++)
                {
                    if (sh_edge[jj].type==0)  //line
                        fprintf(f, "Line{%d} In Surface {%d};\n", jj + 1, k + 1);
                    else
                        fprintf(f, "Circle{%d} In Surface {%d};\n", jj+ 1, k + 1);
                }
            }
        }
        fprintf(f, "\n////\n");

        k++;
    }

    fprintf(f, "\n// THE END\n");

    fclose(f);

    //So there is no more than k bodies.
    //allocation of body_area;
    body_area=(double *)malloc(k*sizeof(double) + M100);  //max k bodies  //10-12-2025
    for (i=0; i<k; i++) body_area[i]=0.0;

    for (j=0; j<64; j++)  //for max 64 cases
    {
        surface_load[j] = (double *) malloc(k * sizeof(double) + M100);  //max k bodies
        for (i = 0; i < k; i++) surface_load[j][i] = 0.0;

        point_load_x[j]=0.0;
        point_load_y[j]=0.0;
        distributed_load_x[j]=0.0;
        distributed_load_y[j]=0.0;
    }

    //creating geo file
    sprintf(params, "%s%sshield.sif", _STATIC_, _shield_);
    f = fopen(params, "wt");

    if (f == NULL) {
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_RESULTS_FILE_, 12,
                           (char *) "", 11, 1,
                           62);
        no_error = FALSE;
        goto sh_error;
    }

    fprintf(f, "Header\n");
    fprintf(f, "  CHECK KEYWORDS Warn\n");
    fprintf(f, "  Mesh DB \"%s\" \"%s\"\n", "Static", "shield");
    fprintf(f, "  Include Path \"\"\n");
    fprintf(f, "  Results Directory \"%s%s\"\n", _STATIC_, "shield");
    fprintf(f, "End\n\n");

    fprintf(f, "Simulation\n");
    fprintf(f, "  Max Output Level = 5\n");
    fprintf(f, "  Coordinate System = Cartesian\n");
    fprintf(f, "  Coordinate Mapping(3) = 1 2 3\n");
    fprintf(f, "  Simulation Type = Steady state\n");
    fprintf(f, "  Steady State Max Iterations = 1\n");
    fprintf(f, "  Output Intervals(1) = 1\n");
    fprintf(f, "  Solver Input File = shield.sif\n");
    fprintf(f, "  Output File = \"shield_sls.result\"\n");
    fprintf(f, "  Post File = shield.vtu\n");
    fprintf(f, "End\n\n");

    fprintf(f, "Constants\n");
    fprintf(f, "  Gravity(4) = 0 -1 0 9.81\n");
    fprintf(f, "  Stefan Boltzmann = 5.670374419e-08\n");
    fprintf(f, "  Permittivity of Vacuum = 8.85418781e-12\n");
    fprintf(f, "  Permeability of Vacuum = 1.25663706e-6\n");
    fprintf(f, "  Boltzmann Constant = 1.380649e-23\n");
    fprintf(f, "  Unit Charge = 1.6021766e-19\n");
    fprintf(f, "End\n\n");

    fprintf(f, "Solver 1\n");
    fprintf(f, "  Equation = \"Shell Solver\"\n");
    fprintf(f, "  Procedure = \"ShellMultiSolver\" \"ShellSolver\"\n");
    //fprintf(f, "  Procedure = \"ShellSolver\" \"ShellSolver\"\n");
    fprintf(f, "  Linear System Solver = Direct\n");
    fprintf(f, "  !Linear System Max Iterations = 10000\n");
    fprintf(f, "  !Linear System Iterative Method = BiCGStab\n");
    fprintf(f, "  !Linear System Preconditioning = ILU0\n");
    fprintf(f, "  Linear System Convergence Tolerance = 1.0e-10\n");
    fprintf(f, "  Nonlinear System Newton After Tolerance = 1.0e-8\n");
    fprintf(f, "  Nonlinear System Newton After Iterations = 20\n");
    //fprintf(f, "  Nonlinear System Max Iterations = 1\n");  //????
    fprintf(f, "  Nonlinear System Max Iterations = 5\n");  //????
    fprintf(f, "  Nonlinear System Convergence Tolerance = 1.0e-8\n");
    fprintf(f, "  Nonlinear System Relaxation Factor = 1.0\n");
    fprintf(f, "  Steady State Convergence Tolerance = 1.0e-8\n");
    fprintf(f, "\n");
    fprintf(f, "  Shear Stabilization Parameter = Real 0.0\n");
    //fprintf(f, "  Shear Stabilization Parameter = Real 0.05\n");
    fprintf(f, "  Drilling Stabilization Parameter = Real 1.0\n");
    fprintf(f, "  Calculate Stresses = Logical TRUE\n");
    fprintf(f, "  !Compute Membrane Stress = Logical TRUE\n");
    fprintf(f, "  Compute Bending Stress = Logical FALSE\n");  //or TRUE
    //fprintf(f, "  Compute Bending Stress = Logical TRUE\n");  //or TRUE
    fprintf(f, "  Compute Total Stress = Logical TRUE\n");  //or FALSE
    fprintf(f, "  Bottom Side Stress = Logical TRUE\n");
    fprintf(f, "\n");
    fprintf(f, "  Membrane Only = Logical FALSE\n");
    fprintf(f, "  Use DKT Triangle = Logical FALSE\n");    //no 2
    fprintf(f, "  Use RMITC3 Element = Logical TRUE\n");   //no 1
    fprintf(f, "  Use SMITC Element = Logical FALSE\n");   //no 3
    fprintf(f, "  Use MITC3 Element = Logical FALSE\n");   //no 4
    fprintf(f, "  Integration Points = Integer 3\n");
    fprintf(f, "End\n\n");

    sif_body = 0;

    for (i = 0; i < zone_no; i++) {
        fprintf(f, "Body %d\n", sif_body + 1);
        fprintf(f, "  Target Bodies(1) = %d\n", zone_property[i].k + 1);
        fprintf(f, "  Name = \"Body Property %d\"\n", sif_body + 1);
        fprintf(f, "  Equation = 1\n");
        fprintf(f, "  Material = %d\n", zone_property[i].property_number);
        fprintf(f, "  Body Force = %d\n", sif_body + 1);
        sif_body_zone[i]=sif_body; //TO DO - dynamic
        fprintf(f, "End\n\n");
        sif_body++;
    }

    for (i = 0; i < shield_no; i++) {
        fprintf(f, "Body %d\n", sif_body + 1);
        fprintf(f, "  Target Bodies(1) = %d\n", shield_property[i].k + 1);
        fprintf(f, "  Name = \"Body Property %d\"\n", sif_body + 1);
        fprintf(f, "  Equation = 1\n");
        fprintf(f, "  Material = %d\n", shield_property[i].property_number);
        fprintf(f, "  Body Force = %d\n", sif_body + 1);
        sif_body_shield[i]=sif_body; //TO DO - dynamic
        fprintf(f, "End\n\n");
        sif_body++;
    }

    fprintf(f, "Equation 1\n");
    fprintf(f, "  Name = \"Equation 1\"\n");
    fprintf(f, "  Active Solvers(1) = 1\n");
    fprintf(f, "End\n\n");

    sif_material = 0;
    for (i = 0; i < zone_no; i++)
    {
        this_property = -1;
        //searching for properties
        for (j = 0; j < sh_property_no; j++) {
            if (sh_property[j].n == zone_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        if (this_property == -1)
        {
            sprintf(report_row, "%s %d%s%s", _property_not_defined_, zone_property[i].property_number, rn,rn);
            strcat(report, report_row);
            fclose(f);
            goto sh_error;
        }
    }

    for (i = 0; i < shield_no; i++)
    {
        this_property = -1;
        //searching for properties
        for (j = 0; j < sh_property_no; j++) {
            if (sh_property[j].n == shield_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        if (this_property == -1)
        {
            sprintf(report_row, "%s %d%s%s", _property_not_defined_, shield_property[i].property_number, rn,rn);
            strcat(report, report_row);
            fclose(f);
            goto sh_error;
        }
    }

    //now it's seperated from zones and shields
    for (i = 0; i < sh_property_no; i++)
    {
        sif_material=sh_property[i].n;
        fprintf(f, "Material %d\n", sif_material);
        fprintf(f, "  Name = \"Material %d\"\n", sif_material);
        fprintf(f, "  Youngs modulus = %g\n", sh_property[i].E);
        fprintf(f, "  Density = %g\n", sh_property[i].d);
        fprintf(f, "  Poisson ratio = %f\n", (sh_property[i].E / (2. * sh_property[i].G)) - 1.);
        set_decimal_format(par[1], sh_property[i].h, dim_precision_sh);
        fprintf(f, "  Thickness = %s\n", par[1]);
        fprintf(f, "End\n\n");
        //sif_material++;
    }


    ////Boundary conditions
    sif_boundary_condition = 0;
    sif_boundary_condition_simple = 0;
    sif_boundary_condition_fixed = 0;
    sif_boundary_condition_roll_x = 0;
    sif_boundary_condition_roll_y = 0;

    //simply supported edges
    for (i = 0; i < sh_edge_no; i++) {
        if (sh_edge[i].restraint == 6) sif_boundary_condition_simple++;
        else if (sh_edge[i].restraint == 7) sif_boundary_condition_fixed++;
        else if (sh_edge[i].restraint == 4)  //with correction
        {
            if (Check_if_Equal(sh_node[sh_edge[i].node1].x, sh_node[sh_edge[i].node2].x)==TRUE)  //vertical
            {
                if (Check_if_Equal(sh_node[sh_edge[i].node1].y, sh_node[sh_edge[i].node2].y)==TRUE)  //is vertical but also horizontal, so eventually pointwise
                {
                    sh_edge[i].restraint = 6;  //changed qualification from roll to simple
                    sif_boundary_condition_simple++;
                }
                else
                {
                    sh_edge[i].restraint = 3;  //changed to distinguish each other, so from now vertical edge has restraint 3
                    sif_boundary_condition_roll_y++;
                }
            }
            else if (Check_if_Equal(sh_node[sh_edge[i].node1].y, sh_node[sh_edge[i].node2].y)==TRUE)  //horizontal
            {
                if (Check_if_Equal(sh_node[sh_edge[i].node1].x, sh_node[sh_edge[i].node2].x)==TRUE)  //is horizontal but also vertical, so eventually pointwise
                {
                    sh_edge[i].restraint = 6;  //changed qualification from roll to simple
                    sif_boundary_condition_simple++;
                }
                else sif_boundary_condition_roll_x++;
            }
            else  //not horizontal, not vertical
            {
                sh_edge[i].restraint = 6;  //changed qualification from roll to simple
                sif_boundary_condition_simple++;
                //notice should be sent to user about changing qualification of support from roll to simple
            }
        }
    }
    ////simple
    if (sif_boundary_condition_simple > 0) {
        fprintf(f, "Boundary Condition %d\n", sif_boundary_condition + 1);
        sprintf(par[1], "  Target Boundaries(%d) =", sif_boundary_condition_simple);
        for (i = 0; i < sh_edge_no; i++) {
            if (sh_edge[i].restraint == 6) {
                sprintf(par[2], " %d", sh_edge[i].k + 1);
                strcat(par[1], par[2]);
            }
        }
        fprintf(f, "%s\n", par[1]);
        fprintf(f, "  Name = \"Boundary Condition %d\"\n", sif_boundary_condition + 1);
        //fprintf(f, "  Displacement 1 = 0\n");
        //fprintf(f, "  Displacement 2 = 0\n");
        //fprintf(f, "  Displacement 3 = 0\n");
        fprintf(f, "  Deflection 1 = 0\n");
        fprintf(f, "  Deflection 2 = 0\n");
        fprintf(f, "  Deflection 3 = 0\n");
        fprintf(f, "  u 1 = 0\n");
        fprintf(f, "  u 2 = 0\n");
        fprintf(f, "  u 3 = 0\n");
        fprintf(f, "End\n\n");

        sif_boundary_condition++;
    }
    ////fixed
    if (sif_boundary_condition_fixed > 0) {
        fprintf(f, "Boundary Condition %d\n", sif_boundary_condition + 1);
        sprintf(par[1], "  Target Boundaries(%d) =", sif_boundary_condition_fixed);
        for (i = 0; i < sh_edge_no; i++) {
            if (sh_edge[i].restraint == 7) {
                sprintf(par[2], " %d", sh_edge[i].k + 1);
                strcat(par[1], par[2]);
            }
        }
        fprintf(f, "%s\n", par[1]);
        fprintf(f, "  Name = \"Boundary Condition %d\"\n", sif_boundary_condition + 1);
        fprintf(f, "  Deflection 1 = 0\n");
        fprintf(f, "  Deflection 2 = 0\n");
        fprintf(f, "  Deflection 3 = 0\n");
        fprintf(f, "  Deflection 4 = 0\n");
        fprintf(f, "  Deflection 5 = 0\n");
        fprintf(f, "  Deflection 6 = 0\n");
        fprintf(f, "  u 1 = 0\n");
        fprintf(f, "  u 2 = 0\n");
        fprintf(f, "  u 3 = 0\n");
        fprintf(f, "  dnu 1 = 0\n");
        fprintf(f, "  dnu 2 = 0\n");
        fprintf(f, "  dnu 3 = 0\n");
        ////fprintf(f, "  Normalize By Area = Logical True\n");
        fprintf(f, "End\n\n");

        sif_boundary_condition++;
    }
    ////roll x
    if (sif_boundary_condition_roll_x > 0) {
        fprintf(f, "Boundary Condition %d\n", sif_boundary_condition + 1);
        ////fprintf(f, "  Normal-Tangential Displacement = Logical TRUE\n");
        sprintf(par[1], "  Target Boundaries(%d) =", sif_boundary_condition_roll_x);
        for (i = 0; i < sh_edge_no; i++) {
            if (sh_edge[i].restraint == 4) {
                sprintf(par[2], " %d", sh_edge[i].k + 1);
                strcat(par[1], par[2]);
            }
        }
        fprintf(f, "%s\n", par[1]);
        fprintf(f, "  Name = \"Boundary Condition %d\"\n", sif_boundary_condition + 1);
        fprintf(f, "  Deflection 2 = 0\n");
        fprintf(f, "  u 2 = 0\n");
        fprintf(f, "End\n\n");

        sif_boundary_condition++;
    }
    ////roll y
    if (sif_boundary_condition_roll_y > 0) {
        fprintf(f, "Boundary Condition %d\n", sif_boundary_condition + 1);
        ////fprintf(f, "  Normal-Tangential Displacement = Logical TRUE\n");
        sprintf(par[1], "  Target Boundaries(%d) =", sif_boundary_condition_roll_y);
        for (i = 0; i < sh_edge_no; i++) {
            if (sh_edge[i].restraint == 3) {
                sprintf(par[2], " %d", sh_edge[i].k + 1);
                strcat(par[1], par[2]);
            }
        }
        fprintf(f, "%s\n", par[1]);
        fprintf(f, "  Name = \"Boundary Condition %d\"\n", sif_boundary_condition + 1);
        fprintf(f, "  Deflection 1 = 0\n");
        fprintf(f, "  u 1 = 0\n");
        fprintf(f, "End\n\n");

        sif_boundary_condition++;
    }
    ////////

    int combi_uls_no_total = 0;
    int combi_sls_no_total = 0;
    int combi_qpsls_no_total = 0;

    int combi_ulslc_ino;
    int combi_slslc_ino;
    int combi_qpslslc_ino;

    int ULSLC_NO;
    int SLSLC_NO;
    int QPSLSLC_NO;

    int combi_flag;
    int flag_;

    combi_total_numbers_no = 0;

    combi_uls_no = 0;
    combi_sls_no = 0;
    combi_qpsls_no = 0;

    if (ret_standard == 1)  //EUROCODE
    {
        ULSLC = (COMBINATION *) &EUROCODE_ULSLC;
        SLSLC = (COMBINATION *) &EUROCODE_SLSLC;
        QPSLSLC = (COMBINATION *) &EUROCODE_QPSLSLC;

        MC_ULSLC = (int *) &EUROCODE_MC_ULSLC;
        MC_SLSLC = (int *) &EUROCODE_MC_SLSLC;
        MC_QPSLSLC = (int *) &EUROCODE_MC_QPSLSLC;

        ULSLC_NO = EUROCODE_ULSLC_NO;
        SLSLC_NO = EUROCODE_SLSLC_NO;
        QPSLSLC_NO = EUROCODE_QPSLSLC_NO;

        load_flag = load_flag_EU;

    } else if (ret_standard == 2)  //ASCE
    {
        ULSLC = (COMBINATION *) &ASCE_ULSLC;
        SLSLC = (COMBINATION *) &ASCE_SLSLC;
        QPSLSLC = (COMBINATION *) &ASCE_QPSLSLC;

        MC_ULSLC = (int *) &ASCE_MC_ULSLC;
        MC_SLSLC = (int *) &ASCE_MC_SLSLC;
        MC_QPSLSLC = (int *) &ASCE_MC_QPSLSLC;

        ULSLC_NO = ASCE_ULSLC_NO;
        SLSLC_NO = ASCE_SLSLC_NO;
        QPSLSLC_NO = ASCE_QPSLSLC_NO;

        load_flag = load_flag_ASCE;
    } else if (ret_standard == 3)  //ICC
    {
        ULSLC = (COMBINATION *) &ICC_ULSLC;
        SLSLC = (COMBINATION *) &ICC_SLSLC;
        QPSLSLC = (COMBINATION *) &ICC_QPSLSLC;

        MC_ULSLC = (int *) &ICC_MC_ULSLC;
        MC_SLSLC = (int *) &ICC_MC_SLSLC;
        MC_QPSLSLC = (int *) &ICC_MC_QPSLSLC;

        ULSLC_NO = ICC_ULSLC_NO;
        SLSLC_NO = ICC_SLSLC_NO;
        QPSLSLC_NO = ICC_QPSLSLC_NO;

        load_flag = load_flag_ICC;
    } else {
        sprintf(report_row, "%s%s%s", _unknown_standard_, rn,rn);
        strcat(report, report_row);
    }

    if (Check_if_Equal(fabs(gZ), 0)) combi_flag = 0;
    else combi_flag = 1;

    //nodes forces and moments
    /*
    pllc_node_force_moment_no = 0;
    for (i = 0; i < sh_point_load_no; i++) {
        //if (TestBit(st_layer, st_node_force_moment[i].layer)) {
        if (sh_point_load[i].factor_record >= 0) {
            gamma_l = sh_load_factors[sh_point_load[i].factor_record].gamma;
            flag_ = load_flag[sh_load_factors[sh_point_load[i].factor_record].load];
            flag_ = load_flag[sh_load_factors[sh_point_load[i].factor_record].load];
        } else {
            gamma_l = load_factors[abs(sh_point_load[i].factor_record)].gamma;
            flag_ = load_flag[load_factors[abs(sh_point_load[i].factor_record)].load];
        }
        combi_flag |= flag_;
        pllc_node_force_moment_no++;
        //}
    }
    */

    //shield uniform load
    pllc_sh_load_no = 0;
    for (i = 0; i < sh_load_no; i++) {
        //if (TestBit(st_layer, sh_load[i].layer)) {
        if (sh_load[i].factor_record >= 0) {
            gamma_l = sh_load_factors[sh_load[i].factor_record].gamma;
            flag_ = load_flag[sh_load_factors[sh_load[i].factor_record].load];
        } else {
            gamma_l = load_factors[abs(sh_load[i].factor_record)].gamma;
            flag_ = load_flag[load_factors[abs(sh_load[i].factor_record)].load];
        }
        combi_flag |= flag_;
        pllc_sh_load_no++;
        //}
    }
    /////////////////////////
    //having combi_flag combination flags can be set

    combi_total_numbers[combi_total_numbers_no].case_no = 0; //li;
    combi_total_numbers[combi_total_numbers_no].combi = 0;  //SLS
    combi_total_numbers[combi_total_numbers_no].combination = 0;
    combi_total_numbers[combi_total_numbers_no].first = 1;
    combi_total_numbers[combi_total_numbers_no].last = 1;
    combi_total_numbers_no++;
    combi_total_numbers[combi_total_numbers_no].case_no = 0; //li;
    combi_total_numbers[combi_total_numbers_no].combi = 1;  //ULS
    combi_total_numbers[combi_total_numbers_no].combination = 0;
    combi_total_numbers[combi_total_numbers_no].first = 1;
    combi_total_numbers[combi_total_numbers_no].last = 1;
    combi_total_numbers_no++;

    int lfirst = 1;
    int llast = 0;
    for (i = 0; i < ULSLC_NO; i++) {
        if ((combi_flag & ULSLC[i].mask) &&
            ((MC_ULSLC[i] & combi_flag) || (MC_ULSLC[i] == 0))) {   //some loads included in mask
            ULSLC[i].flag = 1;
            combi_uls_no++;
            combi_total_numbers[combi_total_numbers_no].case_no = 0; //li;
            combi_total_numbers[combi_total_numbers_no].combi = 2;  //ULSLC
            combi_total_numbers[combi_total_numbers_no].combination = i + 1;
            combi_total_numbers[combi_total_numbers_no].first = lfirst;
            combi_total_numbers[combi_total_numbers_no].last = 0;
            lfirst = 0;
            combi_total_numbers_no++;
        } else ULSLC[i].flag = 0;
    }
    if (combi_total_numbers_no > 0) combi_total_numbers[combi_total_numbers_no - 1].last = 1;
    if (rout) printf("combi_uls_no=%d\n", combi_uls_no);

    lfirst = 1;
    for (i = 0; i < SLSLC_NO; i++) {
        if ((combi_flag & SLSLC[i].mask) && ((MC_SLSLC[i] & combi_flag) || (MC_SLSLC[i] == 0))) {
            SLSLC[i].flag = 1;
            combi_sls_no++;
            combi_total_numbers[combi_total_numbers_no].case_no = 0; //li;
            combi_total_numbers[combi_total_numbers_no].combi = 3;  //SLSLC
            combi_total_numbers[combi_total_numbers_no].combination = i + 1;
            combi_total_numbers[combi_total_numbers_no].first = lfirst;
            combi_total_numbers[combi_total_numbers_no].last = 0;
            lfirst = 0;
            combi_total_numbers_no++;
        } else SLSLC[i].flag = 0;
    }
    if (combi_total_numbers_no > 0) combi_total_numbers[combi_total_numbers_no - 1].last = 1;
    if (rout) printf("combi_sls_no=%d\n", combi_sls_no);

    lfirst = 1;
    for (i = 0; i < QPSLSLC_NO; i++) {
        if ((combi_flag & QPSLSLC[i].mask) && ((MC_QPSLSLC[i] & combi_flag) || (MC_QPSLSLC[i] == 0))) {
            QPSLSLC[i].flag = 1;
            combi_qpsls_no++;
            combi_total_numbers[combi_total_numbers_no].case_no = 0; //li;
            combi_total_numbers[combi_total_numbers_no].combi = 4;  //QPSLSLC
            combi_total_numbers[combi_total_numbers_no].combination = i + 1;
            combi_total_numbers[combi_total_numbers_no].first = lfirst;
            combi_total_numbers[combi_total_numbers_no].last = 0;
            lfirst = 0;
            combi_total_numbers_no++;
        } else QPSLSLC[i].flag = 0;
    }
    if (combi_total_numbers_no > 0) combi_total_numbers[combi_total_numbers_no - 1].last = 1;
    if (rout) printf("combi_qpsls_no=%d\n", combi_qpsls_no);

    //combi_uls_no_total+=combi_uls_no[li];
    //combi_sls_no_total+=combi_sls_no[li];
    //combi_qpsls_no_total+=combi_qpsls_no[li];

    combi_uls_no_total += combi_uls_no;
    combi_sls_no_total += combi_sls_no;
    combi_qpsls_no_total += combi_qpsls_no;

    for (j = 0; j < sh_load_no; j++) { ;
    }


    combinations_number = 2 + combi_uls_no_total + combi_sls_no_total +
                          combi_qpsls_no_total;  //SLS + ULS for each st_layer_no plus total number of combinations for each state

    printf("%d\t\t# number of static load cases\n", combinations_number);

    //ST_UNIFORM_LOAD *st_uniform_load_comb=NULL, *st_uniform_load_cons=NULL;
    int st_uniform_load_no_cons;

    ////st_uniform_load_comb = malloc((sh_load_no + 1) * sizeof(ST_UNIFORM_LOAD));
    ////st_uniform_load_cons = malloc((sh_load_no + 1) * sizeof(ST_UNIFORM_LOAD));

    int case_number = 0;

    ///////////////////////////
    //// LOADS SLS BEGINS

    case_files[0]=case_number;
    case_number++;
    sif_body_force = 0;

    int *zone_variable_load_no=NULL;
    int *shield_variable_load_no=NULL;
    int *zone_case_variable_load_no=NULL;
    int *shield_case_variable_load_no=NULL;
    int **sif_boundary_condition_sls_zone=NULL;
    int **sif_boundary_condition_sls_shield=NULL;

    int total_zone_variable_load_no=0;
    int total_shield_variable_load_no=0;

    //allocation for Boundary conditions
    zone_variable_load_no=(int*)malloc(zone_no * sizeof(int) + M100);  //WARNING  +1    10-1202-25
    zone_case_variable_load_no=(int*)malloc(zone_no * sizeof(int) + M100);
    shield_variable_load_no=(int*)malloc(shield_no * sizeof(int) + M100);
    shield_case_variable_load_no=(int*)malloc(shield_no * sizeof(int) + M100);
    sif_boundary_condition_sls_zone=(int**)malloc(zone_no * sizeof(int *) + M100);
    if (sif_boundary_condition_sls_zone == NULL) {
        fprintf(stderr, "Memory allocation failed for array of pointers\n");
    }
    sif_boundary_condition_sls_shield=(int**)malloc(shield_no * sizeof(int *) + M100);
    if (sif_boundary_condition_sls_shield == NULL) {
        fprintf(stderr, "Memory allocation failed for array of pointers\n");
    }
    for (i=0; i<zone_no; i++)
    {
        sif_boundary_condition_sls_zone[i]=(int*)malloc(sh_load_no*2 * sizeof(int) + M100);  //*2 due to possibility of increasing sh_load_no
        if (sif_boundary_condition_sls_zone[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for vector %d\n", i);
        }
    }
    for (i=0; i<shield_no+10; i++)  //juest for a case
    {
        sif_boundary_condition_sls_shield[i]=(int*)malloc(sh_load_no*2 * sizeof(int) + M100);  //*2 due to possibility of increasing sh_load_no
        if (sif_boundary_condition_sls_shield[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for vector %d\n", i);
        }
    }
    for (i = 0; i < zone_no; i++)
    {
        //searching for properties
        for (j = 0; j < sh_property_no; j++) {
            if (sh_property[j].n == zone_property[i].property_number) {
                this_property = j;
                break;
            }
        }

        zone_variable_load_no[i]=0;

        for (j=0; j<sh_load_no; j++)
        {
            if ((sh_load[j].body == 3) && (sh_load[j].body_no == i)) {
                if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                {
                    zone_variable_load_no[i]++;
                    total_zone_variable_load_no++;
                }
            }
        }

        if (zone_variable_load_no[i]>0)  //from now we know
        {
            for (j = 0; j < sh_load_no; j++)
            {
                if ((sh_load[j].body == 3) && (sh_load[j].body_no == i))
                {
                    if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                    {
                        /////Distributed load on edge
                        sif_boundary_condition_sls_zone[i][j]=sif_boundary_condition;
                        ////Load on boundaries
                        fprintf(f, "Boundary Condition %d\n", sif_boundary_condition + 1);
                        fprintf(f, "  Name = \"Boundary Loads %d\"\n", sif_boundary_condition + 1);


                        fprintf(f, "  Target Boundaries(1) = %d\n", sh_load[j].k + 1);
                        fprintf(f, "  ! trapezoidal load\n");
                        fprintf(f, "  Name = \"VariableLoad%d\"\n", zone_variable_load_no[i] + 1);
                        //component Qx
                        if ((Check_if_Equal(sh_load[j].magnitude1x, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2x, 0.0) == FALSE)) {
                            set_decimal_format(parf[0], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[1], sh_load[j].magnitude1x * unit_factors_sh->q_f, force_precision);
                            set_decimal_format(parf[2], milimetryoby(sh_load[j].y2) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[3], sh_load[j].magnitude2x * unit_factors_sh->q_f, force_precision);
                            fprintf(f, "  Force 1 = Variable Coordinate 2\n");
                            fprintf(f, "  Real\n");
                            fprintf(f, "  %s %s\n", parf[0], parf[1]);
                            fprintf(f, "  %s %s\n", parf[2], parf[3]);
                            fprintf(f, "  End\n");
                        }
                        //component Qy
                        if ((Check_if_Equal(sh_load[j].magnitude1y, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2y, 0.0) == FALSE)) {
                            set_decimal_format(parf[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[1], sh_load[j].magnitude1y * unit_factors_sh->q_f, force_precision);
                            set_decimal_format(parf[2], milimetryobx(sh_load[j].x2) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[3], sh_load[j].magnitude2y * unit_factors_sh->q_f, force_precision);
                            fprintf(f, "  Force 2 = Variable Coordinate 1\n");
                            fprintf(f, "  Real\n");
                            fprintf(f, "  %s %s\n", parf[0], parf[1]);
                            fprintf(f, "  %s %s\n", parf[2], parf[3]);
                            fprintf(f, "  End\n");
                        }

                        double ddlx = fabs((double) (milimetryobx(sh_load[j].x2) - milimetryobx(sh_load[j].x1))) * geo_units_factor;
                        double ddly = fabs((double) (milimetryoby(sh_load[j].y2) - milimetryoby(sh_load[j].y1))) * geo_units_factor;
                        distributed_load_x[case_number - 1] += ((sh_load[j].magnitude1x + sh_load[j].magnitude2x) * unit_factors_sh->q_f) * ddly / 2.;
                        distributed_load_y[case_number - 1] += ((sh_load[j].magnitude1y + sh_load[j].magnitude2y) * unit_factors_sh->q_f) * ddlx / 2.;

                        fprintf(f, "End\n\n");

                        sif_boundary_condition++;
                    }
                }
            }
        }
        /////////////////  distributed load on edge

        fprintf(f, "Body Force %d\n", sif_body_zone[i] + 1);
        fprintf(f, "  Name = \"BodyForce zone%d\"\n", sif_body_zone[i] + 1);
        ////fprintf(f, "  Normalize By Area = Logical True\n");
        strcpy(load_formula, "");

        //self_weight = 0.0;
        self_weight = gZ * sh_property[this_property].d; // * sh_property[this_property].h;  in Body Force 2 it will be multiplied by thickness by solver
        if (self_weight > 0.0) {
            set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
            strcat(load_formula, "(");
            strcat(load_formula, par[1]);
            strcat(load_formula, ")");

            surface_load[case_number-1][zone_property[i].k] += (self_weight * sh_property[this_property].h);
        }

        if (zone_property[i].property_number==shield_property[0].property_number)  //load zone
        {
            if ((zone_property[i].load == 0) && (self_weight == 0.0)) //no load
            {
                if (self_weight == 0.0) //no load, no self load
                {
                    //load simulation
                    set_decimal_format(par[1], 0, load_precision);
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");
                }
            } else {
                for (j = 0; j < sh_load_no; j++) //all load
                {
                    if ((sh_load[j].take_it == 1) &&   //active
                        (sh_load[j].type == 1) &&  //uniformly distributed on area
                        (sh_load[j].body == 3) &&  //zone
                        (sh_load[j].body_no == i)) //zone number
                    {
                        set_decimal_format(par[1], -sh_load[j].magnitude1y * unit_factors_sh->q_f / sh_property[this_property].h,
                                           load_precision);  //is assumed that magnitude1=magnitude2
                        if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                        strcat(load_formula, "(");
                        strcat(load_formula, par[1]);
                        strcat(load_formula, ")");

                        surface_load[case_number-1][zone_property[i].k] += (sh_load[j].magnitude1y * unit_factors_sh->q_f);
                    }
                }
            }
        }
        else  //property zone, inherits from shield 0 + zona local load
        {
            for (j = 0; j < sh_load_no; j++) //all load
            {
                if ((sh_load[j].take_it == 1) &&   //active
                    (sh_load[j].type == 1) &&   //uniformly distributed on area, zone and number
                    (((sh_load[j].body == 3) && (sh_load[j].body_no == i)) ||  //local load on zone and zone number
                    (sh_load[j].body == 0) && (sh_load[j].body_no ==  zone_property[i].pn)))  //inherited load on shield and shield number
                {
                    set_decimal_format(par[1], -sh_load[j].magnitude1y * unit_factors_sh->q_f / sh_property[this_property].h,
                                       load_precision);  //is assumed that magnitude1=magnitude2
                    if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][zone_property[i].k] += (sh_load[j].magnitude1y * unit_factors_sh->q_f);
                }
            }
        }

        ////fprintf(f,"  Direction of Outward Normal = –1\n");

        ////if (strlen(load_formula) > 0) fprintf(f, "  Normal Pressure = Real MATC \"%s\"\n", load_formula);
        if (strlen(load_formula) > 0) fprintf(f, "  Body Force 2 = Real MATC \"%s\"\n", load_formula);

        ///////////////// Concentrated Load
        /*
        Point Load (m,n) = Real
        Point load can be specified as concentrated load at a point on the body. It is to be
        specified as an array of 9 columns and as many rows as the number of points. The
        quantities in the column are xp, yp, zp, Fx, Fy, Fz, Mx, My, Mz. First three are the
        coordinates of the point of application of load, which should exactly match with the
        coordinates of any one of the nodes. (Preferably the corner points of the body should be
        chosen because the nodes will always be located at the corners.) Next three are the
        forces in axial directions and last three values represent moment about axes. All 9
        components should be specified even though some of these are absent. An example of
        specifying load Fx at two points is given here.

        Point Load (2,9) = Real 2.0 0.0 0.0 10000.0 0.0 0.0 0.0 0.0 0.0 2.0 0.2 0.0 10000.0 0.0 0.0 0.0 0.0 0.0
        */

        //fprintf(f, "  Point Load (2,9) = Real 2.0 2.0 0.0 0.0 0.0 -10000.0 0.0 0.0 0.0 10.5 3.5 0.0 0.0 0.0 -10000.0 0.0 0.0 0.0\n");  //TEST

        strcpy(load_formula, "");
        int f_no=0;

        for (j = 0; j < sh_load_no; j++) //all load
        {
            if (sh_load[j].type==0 && (sh_load[j].body == 3) && (sh_load[j].body_no == i)) //concentrated, zone and number
            {
                strcpy(load_formula0, "");
                if ((sh_load[j].style==4) || (sh_load[j].style==18)) //Fx, Fy
                {
                    set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[2], sh_load[j].magnitude1x * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                    set_decimal_format(par[3], sh_load[j].magnitude1y * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                    sprintf(load_formula0, " %s %s 0.0 %s %s 0.0 0.0 0.0 0.0", par[0], par[1], par[2], par[3]);  //x,y,z,Fx,Fy,Fz,Mx,My,Mz

                    point_load_x[case_number - 1] += (sh_load[j].magnitude1x * unit_factors_sh->F_f);
                    point_load_y[case_number - 1] -= (sh_load[j].magnitude1y * unit_factors_sh->F_f);
                    f_no++;
                }
                else if ((sh_load[j].style==5) || (sh_load[j].style==6))  //Mxy  //-Mxy
                {
                    set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[2], sh_load[j].magnitude1x * unit_factors_sh->M_f, moment_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                    sprintf(load_formula0, " %s %s 0.0 0.0 0.0 0.0 0.0 0.0 %s", par[0], par[1], par[2]);  //x,y,z,Fx,Fy,Fz,Mx,My,Mz
                    f_no++;
                }
                strcat(load_formula, load_formula0);
            }
        }

        if (f_no>0)
            fprintf(f, "  Point Load (%d, 9) = Real %s\n", f_no, load_formula);
        ///////////////// concentrated load

        set_decimal_format(par[1], sh_property[this_property].h, dim_precision_sh);
        fprintf(f, "  Thickness = Real %s\n", par[1]);

        fprintf(f, "End\n\n");
        sif_body_force++;
    }

    for (i = 0; i < shield_no; i++)
    {
        this_property = -1;
        //searching for properties
        for (j = 0; j < sh_property_no; j++) {
            if (sh_property[j].n == shield_property[i].property_number) {
                this_property = j;
                break;
            }
        }

        shield_variable_load_no[i]=0;

        for (j=0; j<sh_load_no; j++)
        {
            if ((sh_load[j].body == 0) && (sh_load[j].body_no == i)) {
                if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                {
                    shield_variable_load_no[i]++;
                    total_shield_variable_load_no++;
                }
            }
        }

        if (shield_variable_load_no[i]>0)  //from now we know
        {
            for (j = 0; j < sh_load_no; j++)
            {
                if ((sh_load[j].body == 0) && (sh_load[j].body_no == i))
                {
                    if ((sh_load[j].style > 9) && (sh_load[j].style < 15)) //all linear
                    {
                        /////Distributed load on edge
                        sif_boundary_condition_sls_shield[i][j] = sif_boundary_condition;
                        ////Load on boundaries
                        fprintf(f, "Boundary Condition %d\n", sif_boundary_condition + 1);
                        fprintf(f, "  Name = \"Boundary Loads %d\"\n", sif_boundary_condition + 1);

                        fprintf(f, "  Target Boundaries(1) = %d\n", sh_load[j].k + 1);
                        fprintf(f, "  ! trapezoidal load\n");
                        fprintf(f, "  Name = \"VariableLoad%d\"\n", shield_variable_load_no[i] + 1);
                        //component Qx
                        if ((Check_if_Equal(sh_load[j].magnitude1x, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2x, 0.0) == FALSE)) {
                            set_decimal_format(parf[0], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[1], sh_load[j].magnitude1x * unit_factors_sh->q_f, force_precision);
                            set_decimal_format(parf[2], milimetryoby(sh_load[j].y2) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[3], sh_load[j].magnitude2x * unit_factors_sh->q_f, force_precision);
                            fprintf(f, "  Force 1 = Variable Coordinate 2\n");
                            fprintf(f, "  Real\n");
                            fprintf(f, "  %s %s\n", parf[0], parf[1]);
                            fprintf(f, "  %s %s\n", parf[2], parf[3]);
                            fprintf(f, "  End\n");
                        }
                        //component Qy
                        if ((Check_if_Equal(sh_load[j].magnitude1y, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2y, 0.0) == FALSE)) {
                            set_decimal_format(parf[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[1], sh_load[j].magnitude1y * unit_factors_sh->q_f, force_precision);
                            set_decimal_format(parf[2], milimetryobx(sh_load[j].x2) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[3], sh_load[j].magnitude2y * unit_factors_sh->q_f, force_precision);
                            fprintf(f, "  Force 2 = Variable Coordinate 1\n");
                            fprintf(f, "  Real\n");
                            fprintf(f, "  %s %s\n", parf[0], parf[1]);
                            fprintf(f, "  %s %s\n", parf[2], parf[3]);
                            fprintf(f, "  End\n");
                        }

                        double ddlx = fabs((double) (milimetryobx(sh_load[j].x2) - milimetryobx(sh_load[j].x1))) * geo_units_factor;
                        double ddly = fabs((double) (milimetryoby(sh_load[j].y2) - milimetryoby(sh_load[j].y1))) * geo_units_factor;
                        distributed_load_x[case_number - 1] += ((sh_load[j].magnitude1x + sh_load[j].magnitude2x) * unit_factors_sh->q_f) * ddly / 2.;
                        distributed_load_y[case_number - 1] += ((sh_load[j].magnitude1y + sh_load[j].magnitude2y) * unit_factors_sh->q_f) * ddlx / 2.;

                        fprintf(f, "End\n\n");
                        sif_boundary_condition++;
                    }
                }
            }
        }
        /////////////////  distributed load on edge

        fprintf(f, "Body Force %d\n", sif_body_shield[i] + 1);
        fprintf(f, "  Name = \"BodyForce shield%d\"\n", sif_body_shield[i] + 1);

        strcpy(load_formula, "");

        //self_weight = 0.0;
        self_weight = gZ * sh_property[this_property].d; // * sh_property[this_property].h;  in Body Force 2 it will be multiplied by thickness by solver
        if (self_weight > 0.0) {
            set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
            strcat(load_formula, "(");
            strcat(load_formula, par[1]);
            strcat(load_formula, ")");

            surface_load[case_number-1][shield_property[i].k] += (self_weight * sh_property[this_property].h);
        }

        for (j = 0; j < sh_load_no; j++)   //all load
        {
            if ((sh_load[j].take_it == 1) &&  //active
                (sh_load[j].type == 1) &&  //uniformly distributed on area
                (sh_load[j].body == 0) && //shield
                (sh_load[j].body_no == i)) //shield number
            {
                set_decimal_format(par[1], -sh_load[j].magnitude1y * unit_factors_sh->q_f / sh_property[this_property].h,
                                   load_precision);  //is assumed that magnitude1=magnitude2
                if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                strcat(load_formula, "(");
                strcat(load_formula, par[1]);
                strcat(load_formula, ")");

                surface_load[case_number-1][shield_property[i].k] += (sh_load[j].magnitude1y * unit_factors_sh->q_f);
            }
        }

        ////fprintf(f,"  Direction of Outward Normal = –1\n");
        ////if (strlen(load_formula) > 0) fprintf(f, "  Normal Pressure = Real MATC \"%s\"\n", load_formula);

        if (strlen(load_formula) > 0) fprintf(f, "  Body Force 2 = Real MATC \"%s\"\n", load_formula);
        ////if (strlen(load_formula) > 0) fprintf(f, "  Body Force 2 =  %s\n", par[1]);

        ///////////////// Concentrated Load
        strcpy(load_formula, "");
        int f_no=0;

        for (j = 0; j < sh_load_no; j++) //all load
        {
            if (sh_load[j].type==0 && (sh_load[j].body == 0) && (sh_load[j].body_no == i)) //concentrated, shield and number
            {
                strcpy(load_formula0, "");
                if ((sh_load[j].style==4) || (sh_load[j].style==18)) //Fx, Fy
                {
                    set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[2], sh_load[j].magnitude1x * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                    set_decimal_format(par[3], sh_load[j].magnitude1y * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                    sprintf(load_formula0, " %s %s 0.0 %s %s 0.0 0.0 0.0 0.0", par[0], par[1], par[2], par[3]);

                    point_load_x[case_number - 1] += (sh_load[j].magnitude1x * unit_factors_sh->F_f);
                    point_load_y[case_number - 1] -= (sh_load[j].magnitude1y * unit_factors_sh->F_f);
                    f_no++;
                }
                else if ((sh_load[j].style==5) || (sh_load[j].style==6))  //Mxy  //-Mxy
                {
                    set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[2], sh_load[j].magnitude1x * unit_factors_sh->M_f, moment_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                    sprintf(load_formula0, " %s %s 0.0 0.0 0.0 0.0 0.0 0.0 %s", par[0], par[1], par[2]);  //x,y,z,Fx,Fy,Fz,Mx,My,Mz
                    f_no++;
                }
                strcat(load_formula, load_formula0);
            }
        }

        if (f_no>0)
            fprintf(f, "  Point Load (%d, 9) = Real %s\n", f_no, load_formula);
        //////// concentrated load

        set_decimal_format(par[1], sh_property[this_property].h, dim_precision_sh);
        fprintf(f, "  Thickness = Real %s\n", par[1]);
        fprintf(f, "End\n\n");
        sif_body_force++;
    }
    ////SLS ENDS
    fprintf(f, "RUN\n\n");

    ////LOAD VARIANTS ULS BEGINS
    fprintf(f, "Simulation::Output File = \"shield_uls.result\"\n");

    case_files[1]=case_number;
    case_number++;
    sif_body_force = 0;

    for (i = 0; i < zone_no; i++)
    {
        //searching for properties
        for (j = 0; j < sh_property_no; j++) {
            if (sh_property[j].n == zone_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        if (zone_variable_load_no[i]>0)  //we know from SLS
        {
            for (j = 0; j < sh_load_no; j++)
            {
                if ((sh_load[j].body == 3) && (sh_load[j].body_no == i))
                {
                    if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                    {
                        if (sh_load[j].factor_record >= 0)
                            gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                        else gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;

                        /////Distributed load on edge
                        ////Load on boundaries
                        fprintf(f, "Boundary Condition %d\n", sif_boundary_condition_sls_zone[i][j] + 1);
                        fprintf(f, "  Name = \"Boundary Loads %d\"\n", sif_boundary_condition_sls_zone[i][j] + 1);

                        fprintf(f, "  Target Boundaries(1) = %d\n", sh_load[j].k + 1);
                        fprintf(f, "  ! trapezoidal load\n");
                        fprintf(f, "  Name = \"VariableLoad%d\"\n", zone_variable_load_no[i] + 1);
                        //compnent Qx
                        if ((Check_if_Equal(sh_load[j].magnitude1x, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2x, 0.0) == FALSE)) {
                            set_decimal_format(parf[0], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[1], sh_load[j].magnitude1x * unit_factors_sh->q_f, force_precision);
                            set_decimal_format(parf[2], milimetryoby(sh_load[j].y2) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[3], sh_load[j].magnitude2x * unit_factors_sh->q_f, force_precision);
                            fprintf(f, "  Force 1 = Variable Coordinate 2\n");
                            fprintf(f, "  Real\n");
                            fprintf(f, "  %s %s\n", parf[0], parf[1]);
                            fprintf(f, "  %s %s\n", parf[2], parf[3]);
                            fprintf(f, "  End\n");
                        }
                        //compnent Qy
                        if ((Check_if_Equal(sh_load[j].magnitude1y, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2y, 0.0) == FALSE)) {
                            set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[1], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f, force_precision);
                            set_decimal_format(par[2], milimetryobx(sh_load[j].x2) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[3], sh_load[j].magnitude2y * gamma_l * unit_factors_sh->q_f, force_precision);
                            fprintf(f, "  Force 2 = Variable Coordinate 1\n");
                            fprintf(f, "  Real\n");
                            fprintf(f, "  %s %s\n", parf[0], parf[1]);
                            fprintf(f, "  %s %s\n", parf[2], parf[3]);
                            fprintf(f, "  End\n");
                        }

                        double ddlx = fabs((double) (milimetryobx(sh_load[j].x2) - milimetryobx(sh_load[j].x1))) * geo_units_factor;
                        double ddly = fabs((double) (milimetryoby(sh_load[j].y2) - milimetryoby(sh_load[j].y1))) * geo_units_factor;
                        distributed_load_x[case_number - 1] += ((sh_load[j].magnitude1x + sh_load[j].magnitude2x) * gamma_l * unit_factors_sh->q_f) * ddly / 2.;
                        distributed_load_y[case_number - 1] += ((sh_load[j].magnitude1y + sh_load[j].magnitude2y) * gamma_l * unit_factors_sh->q_f) * ddlx / 2.;

                        fprintf(f, "End\n\n");
                        //sif_boundary_condition++;
                    }
                }
            }
        }
        /////////////////  distributed load on edge

        fprintf(f, "Body Force %d\n", sif_body_zone[i] + 1);  //repeating entire block
        fprintf(f, "  Name = \"BodyForce zone%d\"\n", sif_body_zone[i] + 1);

        strcpy(load_formula, "");
        strcpy(all_load_formula, "");

        //self_weight = 0.0;
        self_weight = gZ * gammas->gamma_g * sh_property[this_property].d; // * sh_property[this_property].h;  in Body Force 2 it will be multiplied by thickness by solver
        if (self_weight > 0.0) {
            set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
            strcat(load_formula, "(");
            strcat(load_formula, par[1]);
            strcat(load_formula, ")");

            surface_load[case_number-1][zone_property[i].k] += (self_weight * sh_property[this_property].h);
        }

        if (zone_property[i].property_number==shield_property[0].property_number)  //load zone
        {
            if ((zone_property[i].load == 0) && (self_weight == 0.0)) //no load
            {
                if (self_weight == 0.0) //no load, no self load
                {
                    //load simulation
                    set_decimal_format(par[1], 0, load_precision);
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");
                }
            } else {
                for (j = 0; j < sh_load_no; j++) //all load
                {
                    if ((sh_load[j].take_it == 1) &&   //active
                        (sh_load[j].type == 1) &&  //uniformly distributed on area,
                        (sh_load[j].body == 3) &&   //zone
                        (sh_load[j].body_no == i)) //zone number
                    {
                        if (sh_load[j].factor_record >= 0)
                            gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                        else gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;

                        set_decimal_format(par[1], -sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f / sh_property[this_property].h,
                                           load_precision);  //is assumed that magnitude1=magnitude2
                        if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                        strcat(load_formula, "(");
                        strcat(load_formula, par[1]);
                        strcat(load_formula, ")");

                        surface_load[case_number-1][zone_property[i].k] += (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f);
                    }
                }
            }
        }
        else  //property zone, inherits from shield 0 + zona local load
        {
            for (j = 0; j < sh_load_no; j++) //all load
            {
                if ((sh_load[j].take_it == 1) && //active
                    (sh_load[j].type == 1) &&   //uniformly distributed on area
                    (((sh_load[j].body == 3) && (sh_load[j].body_no == i)) ||  //local load on zone and zone number
                     (sh_load[j].body == 0) && (sh_load[j].body_no ==  zone_property[i].pn)))  //inherited load on shield and shield number
                {
                    if (sh_load[j].factor_record >= 0)
                        gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                    else gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;

                    set_decimal_format(par[1], -sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f / sh_property[this_property].h,
                                       load_precision);  //is assumed that magnitude1=magnitude2
                    if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][zone_property[i].k] += (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f);
                }
            }
        }

        if (strlen(load_formula) > 0)
        {
            //fprintf(f, "Body Force %d::", sif_body_force + 1);
            fprintf(f, "  Body Force 2 = Real MATC \"%s\"\n", load_formula);
        }

        ///////////////// Concentrated Load
        strcpy(load_formula, "");
        int f_no=0;

        for (j = 0; j < sh_load_no; j++) //all load
        {
            if (sh_load[j].type==0 && (sh_load[j].body == 3) && (sh_load[j].body_no == i)) //concentrated, zone and number
            {
                if (sh_load[j].factor_record >= 0)
                    gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                else gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;

                strcpy(load_formula0, "");
                if ((sh_load[j].style==4) || (sh_load[j].style==18)) //Fx, Fy
                {
                    set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[2], sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                    set_decimal_format(par[3], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                    sprintf(load_formula0, " %s %s 0.0 %s %s 0.0 0.0 0.0 0.0", par[0], par[1], par[2], par[3]);

                    point_load_x[case_number - 1] += (sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f);
                    point_load_y[case_number - 1] -= (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f);
                    f_no++;
                }
                else if ((sh_load[j].style==5) || (sh_load[j].style==6))  //Mxy  //-Mxy
                {
                    set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[2], sh_load[j].magnitude1x * unit_factors_sh->M_f, moment_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                    sprintf(load_formula0, " %s %s 0.0 0.0 0.0 0.0 0.0 0.0 %s", par[0], par[1], par[2]);  //x,y,z,Fx,Fy,Fz,Mx,My,Mz
                    f_no++;
                }
                strcat(load_formula, load_formula0);
            }
        }

        if (f_no>0)
        {
            ////fprintf(f, "Body Force %d::", sif_body_force + 1);
            fprintf(f, "  Point Load (%d, 9) = Real %s\n", f_no, load_formula);
        }
        //////// concentrated load
        set_decimal_format(par[1], sh_property[this_property].h, dim_precision_sh);
        fprintf(f, "  Thickness = Real %s\n", par[1]);
        fprintf(f, "End\n\n");

        sif_body_force++;
    }

    for (i = 0; i < shield_no; i++)
    {
        //searching for properties
        for (j = 0; j < sh_property_no; j++) {
            if (sh_property[j].n == shield_property[i].property_number) {
                this_property = j;
                break;
            }
        }

        /////Distributed load on edge
        ////Load on boundaries
        if (shield_variable_load_no[i]>0)  //we know from SLS
        {
            for (j = 0; j < sh_load_no; j++)
            {
                if ((sh_load[j].body == 0) && (sh_load[j].body_no == i))
                {
                    if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                    {
                        if (sh_load[j].factor_record >= 0)
                            gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                        else gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;

                        fprintf(f, "Boundary Condition %d\n", sif_boundary_condition_sls_shield[i][j] + 1);
                        fprintf(f, "  Name = \"Boundary Loads %d\"\n", sif_boundary_condition_sls_shield[i][j] + 1);

                        fprintf(f, "  Target Boundaries(1) = %d\n", sh_load[j].k + 1);
                        fprintf(f, "  ! trapezoidal load\n");
                        fprintf(f, "  Name = \"VariableLoad%d\"\n", shield_variable_load_no[i] + 1);
                        //compnent Qx
                        if ((Check_if_Equal(sh_load[j].magnitude1x, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2x, 0.0) == FALSE)) {
                            set_decimal_format(parf[0], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[1], sh_load[j].magnitude1x * unit_factors_sh->q_f, force_precision);
                            set_decimal_format(parf[2], milimetryoby(sh_load[j].y2) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[3], sh_load[j].magnitude2x * unit_factors_sh->q_f, force_precision);
                            fprintf(f, "  Force 1 = Variable Coordinate 2\n");
                            fprintf(f, "  Real\n");
                            fprintf(f, "  %s %s\n", parf[0], parf[1]);
                            fprintf(f, "  %s %s\n", parf[2], parf[3]);
                            fprintf(f, "  End\n");
                        }
                        //compnent Qy
                        if ((Check_if_Equal(sh_load[j].magnitude1y, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2y, 0.0) == FALSE)) {
                            set_decimal_format(parf[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[1], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f, force_precision);
                            set_decimal_format(parf[2], milimetryobx(sh_load[j].x2) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(parf[3], sh_load[j].magnitude2y * gamma_l * unit_factors_sh->q_f, force_precision);
                            fprintf(f, "  Force 2 = Variable Coordinate 1\n");
                            fprintf(f, "  Real\n");
                            fprintf(f, "  %s %s\n", parf[0], parf[1]);
                            fprintf(f, "  %s %s\n", parf[2], parf[3]);
                            fprintf(f, "  End\n");
                        }

                        double ddlx = fabs((double) (milimetryobx(sh_load[j].x2) - milimetryobx(sh_load[j].x1))) * geo_units_factor;
                        double ddly = fabs((double) (milimetryoby(sh_load[j].y2) - milimetryoby(sh_load[j].y1))) * geo_units_factor;
                        distributed_load_x[case_number - 1] += ((sh_load[j].magnitude1x + sh_load[j].magnitude2x) * gamma_l * unit_factors_sh->q_f) * ddly / 2.;
                        distributed_load_y[case_number - 1] += ((sh_load[j].magnitude1y + sh_load[j].magnitude2y) * gamma_l * unit_factors_sh->q_f) * ddlx / 2.;

                        fprintf(f, "End\n\n");
                        //sif_boundary_condition++;
                    }
                }
            }
        }
        /////////////////  distributed load on edge

        fprintf(f, "Body Force %d\n", sif_body_shield[i] + 1);
        fprintf(f, "  Name = \"BodyForce shield%d\"\n", sif_body_shield[i] + 1);

        strcpy(load_formula, "");

        //self_weight = 0.0;
        self_weight = gZ * gammas->gamma_g * sh_property[this_property].d; // * sh_property[this_property].h;  in Body Force 2 it will be multiplied by thickness by solver
        if (self_weight > 0.0) {
            set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
            strcat(load_formula, "(");
            strcat(load_formula, par[1]);
            strcat(load_formula, ")");

            surface_load[case_number-1][shield_property[i].k] += (self_weight * sh_property[this_property].h);
        }

        for (j = 0; j < sh_load_no; j++) //all load
        {
            if ((sh_load[j].take_it == 1) && //active
                (sh_load[j].type == 1) &&  //uniformly distributed on area
                (sh_load[j].body == 0) &&  //ahield
                (sh_load[j].body_no == i)) //shield number
            {
                if (sh_load[j].factor_record >= 0)
                    gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                else gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;

                set_decimal_format(par[1], -sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f / sh_property[this_property].h,
                                   load_precision);  //is assumed that magnitude1=magnitude2
                //fprintf(f, "  Normal Pressure = %s\n", par[1]);
                if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                strcat(load_formula, "(");
                strcat(load_formula, par[1]);
                strcat(load_formula, ")");

                surface_load[case_number-1][shield_property[i].k] += (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f);
            }
        }
        if (strlen(load_formula) > 0)
        {
            //fprintf(f, "Body Force %d::", sif_body_force + 1);
            fprintf(f, "  Body Force 2 = Real MATC \"%s\"\n", load_formula);
        }

        ///////////////// Concentrated Load
        strcpy(load_formula, "");
        int f_no=0;

        for (j = 0; j < sh_load_no; j++) //all load
        {
            if (sh_load[j].type==0 && (sh_load[j].body == 0) && (sh_load[j].body_no == i)) //concentrated, shield and number
            {
                if (sh_load[j].factor_record >= 0)
                    gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                else gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;

                strcpy(load_formula0, "");
                if ((sh_load[j].style==4) || (sh_load[j].style==18)) //Fx, Fy
                {
                    set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[2], sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                    set_decimal_format(par[3], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                    sprintf(load_formula0, " %s %s 0.0 %s %s 0.0 0.0 0.0 0.0", par[0], par[1], par[2], par[3]);

                    point_load_x[case_number - 1] += (sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f);
                    point_load_y[case_number - 1] -= (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f);
                    f_no++;
                }
                else if ((sh_load[j].style==5) || (sh_load[j].style==6))  //Mxy  //-Mxy
                {
                    set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                    set_decimal_format(par[2], sh_load[j].magnitude1x * unit_factors_sh->M_f, moment_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                    sprintf(load_formula0, " %s %s 0.0 0.0 0.0 0.0 0.0 0.0 %s", par[0], par[1], par[2]);  //x,y,z,Fx,Fy,Fz,Mx,My,Mz
                    f_no++;
                }
                strcat(load_formula, load_formula0);
            }
        }

        if (f_no>0)
        {
            //fprintf(f, "Body Force %d::", sif_body_force + 1);
            fprintf(f, "  Point Load (%d, 9) = Real %s\n", f_no, load_formula);
        }
        //////// concentrated load

        set_decimal_format(par[1], sh_property[this_property].h, dim_precision_sh);
        fprintf(f, "  Thickness = Real %s\n", par[1]);
        fprintf(f, "End\n\n");

        sif_body_force++;
    }

    fprintf(f, "RUN\n\n");
    ////ULS ENDS

    ////LOAD VARIANTS ULSLC BEGINS

    case_files[2]=case_number;
    combi_ulslc_ino = 0;

    current = 0;
    all_formula = all_formulas[current];
    all_formula_bak = NULL;

    for (int ci = 0; ci < ULSLC_NO; ci++)
    {
        case_number++;  //index is changing always, no matter case will be executed

        if (ULSLC[ci].flag == 1)
        {
            int load;
            int flag;
            ST_LOAD_FACTORS *combi_load_factor;
            /////////////////////////////
            int combi_factor_g;
            if (ret_standard == 1)  //EUROCODE
                combi_factor_g = eurocode_combi_factors_uls[ci][1];
            else if (ret_standard == 2)  //ASCE
                combi_factor_g = asce_combi_factors_uls[ci][1];
            if (ret_standard == 3)  //ICC
                combi_factor_g = icc_combi_factors_uls[ci][1];

            double gamma_g = 1.0;

            if (combi_factor_g & 1) gamma_g *= gammas->gamma_g;
            if (combi_factor_g & 2) gamma_g *= gammas->psi0;
            if (combi_factor_g & 4) gamma_g *= gammas->psi1;
            if (combi_factor_g & 8) gamma_g *= gammas->psi2;
            if (combi_factor_g & 16) gamma_g *= gammas->xi_g;
            if (combi_factor_g & 32) gamma_g *= gammas->gamma_gi;

            sif_body_force = 0;

            strcpy(all_formula, "");


            for (j = 0; j < sh_load_no; j++)
            {
                if (sh_load[j].factor_record >= 0) {
                    gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                    flag = load_flag[sh_load_factors[sh_load[j].factor_record].load];
                } else {
                    gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;
                    flag = load_flag[load_factors[abs(sh_load[j].factor_record)].load];
                }
                if (flag & ULSLC[ci].mask) {
                    sh_load[j].take_it = 1;
                    zone_case_variable_load_no[i]++;
                } else sh_load[j].take_it = 0;
            }

            for (i = 0; i < zone_no; i++)
            {
                //searching for properties
                for (j = 0; j < sh_property_no; j++) {
                    if (sh_property[j].n == zone_property[i].property_number) {
                        this_property = j;
                        break;
                    }
                }

                /////Distributed load on edge
                zone_case_variable_load_no[i]=0;
                if (zone_variable_load_no[i]>0)  //we know from SLS
                {
                    /*
                    for (j = 0; j < sh_load_no; j++)
                    {
                        if ((sh_load[j].body == 3) && (sh_load[j].body_no == i)) {
                            if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                            {
                                if (sh_load[j].factor_record >= 0) {
                                    gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                                    flag = load_flag[sh_load_factors[sh_load[j].factor_record].load];
                                } else {
                                    gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;
                                    flag = load_flag[load_factors[abs(sh_load[j].factor_record)].load];
                                }
                                if (flag & ULSLC[ci].mask) {
                                    sh_load[j].take_it = 1;
                                    zone_case_variable_load_no[i]++;
                                } else sh_load[j].take_it = 0;
                            }
                        }
                    }
                     */

                    for (j = 0; j < sh_load_no; j++)
                    {
                        if ((sh_load[j].body == 3) && (sh_load[j].body_no == i)) {
                            if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                            {
                                if (sh_load[j].take_it == 0)
                                {
                                    sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_zone[i][j] + 1);
                                    sprintf(par[1], "  Active Boundary Condition = Logical False\n");
                                    sprintf(par[2], "End\n\n");
                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[2]);
                                    continue;
                                }

                                if (sh_load[j].factor_record >= 0) {
                                    combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                                    load = sh_load_factors[sh_load[j].factor_record].load;
                                } else {
                                    combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                                    load = load_factors[abs(sh_load[j].factor_record)].load;
                                }

                                int combi_factor;
                                if (ret_standard == 1)  //EUROCODE
                                    combi_factor = eurocode_combi_factors_uls[ci][load];
                                else if (ret_standard == 2)  //ASCE
                                    combi_factor = asce_combi_factors_uls[ci][load];
                                if (ret_standard == 3)  //ICC
                                    combi_factor = icc_combi_factors_uls[ci][load];

                                gamma_l = 1.0;
                                if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                                if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                                if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                                if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                                if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                                if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                                ////Load on boundaries
                                sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_zone[i][j] + 1);
                                sprintf(par[1], "  Name = \"Boundary Loads %d\"\n", sif_boundary_condition_sls_zone[i][j] + 1);

                                sprintf(par[2], "  Target Boundaries(1) = %d\n", sh_load[j].k + 1);
                                sprintf(par[3], "  ! trapezoidal load\n");
                                sprintf(par[4], "  Name = \"VariableLoad%d\"\n", zone_variable_load_no[i] + 1);

                                BOOL zone_case_variable_load_x=FALSE;
                                BOOL zone_case_variable_load_y=FALSE;
                                //component Qx
                                if ((Check_if_Equal(sh_load[j].magnitude1x, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2x, 0.0) == FALSE)) {
                                    set_decimal_format(parf[0], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[1], sh_load[j].magnitude1x * unit_factors_sh->q_f, force_precision);
                                    set_decimal_format(parf[2], milimetryoby(sh_load[j].y2) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[3], sh_load[j].magnitude2x * unit_factors_sh->q_f, force_precision);
                                    sprintf(par[5], "  Force 1 = Variable Coordinate 2\n");
                                    sprintf(par[6], "  Real\n");
                                    sprintf(par[7], "  %s %s\n", parf[0], parf[1]);
                                    sprintf(par[8], "  %s %s\n", parf[2], parf[3]);
                                    sprintf(par[9], "  End\n");
                                    zone_case_variable_load_x=TRUE;
                                }
                                //component Qy
                                if ((Check_if_Equal(sh_load[j].magnitude1y, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2y, 0.0) == FALSE)) {
                                    set_decimal_format(parf[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[1], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f, force_precision);
                                    set_decimal_format(parf[2], milimetryobx(sh_load[j].x2) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[3], sh_load[j].magnitude2y * gamma_l * unit_factors_sh->q_f, force_precision);
                                    sprintf(par[10], "  Force 2 = Variable Coordinate 1\n");
                                    sprintf(par[11], "  Real\n");
                                    sprintf(par[12], "  %s %s\n", parf[0], parf[1]);
                                    sprintf(par[13], "  %s %s\n", parf[2], parf[3]);
                                    sprintf(par[14], "  End\n");
                                    zone_case_variable_load_y=TRUE;
                                }

                                double ddlx = fabs((double) (milimetryobx(sh_load[j].x2) - milimetryobx(sh_load[j].x1))) * geo_units_factor;
                                double ddly = fabs((double) (milimetryoby(sh_load[j].y2) - milimetryoby(sh_load[j].y1))) * geo_units_factor;
                                distributed_load_x[case_number - 1] += ((sh_load[j].magnitude1x + sh_load[j].magnitude2x) * gamma_l * unit_factors_sh->q_f) * ddly / 2.;
                                distributed_load_y[case_number - 1] += ((sh_load[j].magnitude1y + sh_load[j].magnitude2y) * gamma_l * unit_factors_sh->q_f) * ddlx / 2.;

                                sprintf(par[15], "End\n\n");

                                if ((zone_case_variable_load_x) || (zone_case_variable_load_y))
                                {

                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[3]);
                                    strcat(all_formula, par[4]);
                                    if (zone_case_variable_load_x)
                                    {
                                        strcat(all_formula, par[5]);
                                        strcat(all_formula, par[6]);
                                        strcat(all_formula, par[7]);
                                        strcat(all_formula, par[8]);
                                        strcat(all_formula, par[9]);
                                    }
                                    if (zone_case_variable_load_y)
                                    {
                                        strcat(all_formula, par[10]);
                                        strcat(all_formula, par[11]);
                                        strcat(all_formula, par[12]);
                                        strcat(all_formula, par[13]);
                                        strcat(all_formula, par[14]);
                                    }
                                    strcat(all_formula, par[15]);
                                    zone_case_variable_load_no[i]++;
                                }
                                else
                                {
                                    sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_zone[i][j] + 1);
                                    sprintf(par[1], "  Active Boundary Condition = Logical False\n");
                                    sprintf(par[2], "End\n\n");
                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[2]);
                                }
                            }
                        }
                    }
                }
                /////////////////  distributed load on edge

                sprintf(par[0], "Body Force %d\n", sif_body_zone[i] + 1);
                sprintf(par[1], "  Name = \"BodyForce zone%d\"\n", sif_body_zone[i] + 1);
                strcat(all_formula, par[0]);
                strcat(all_formula, par[1]);

                strcpy(load_formula, "");

                //self_weight = 0.0;
                self_weight = gZ * gamma_g * sh_property[this_property].d; // * sh_property[this_property].h;  in Body Force 2 it will be multiplied by thickness by solver
                if (self_weight > 0.0) {
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][zone_property[i].k] += (self_weight * sh_property[this_property].h);
                }

                if (zone_property[i].property_number==shield_property[0].property_number)  //load zone
                {
                    if ((zone_property[i].load == 0) && (self_weight == 0.0)) //no load
                    {
                        if (self_weight == 0.0) //no load, no self load
                        {
                            //load simulation
                            set_decimal_format(par[1], 0, load_precision);
                            strcat(load_formula, "(");
                            strcat(load_formula, par[1]);
                            strcat(load_formula, ")");
                        }
                    } else {
                        for (j = 0; j < sh_load_no; j++) //all load
                        {
                            if ((sh_load[j].take_it == 1) &&   //active
                                (sh_load[j].type == 1) &&   //uniformly distributed on area
                                (sh_load[j].body == 3) &&   //zone
                                (sh_load[j].body_no == i)) //number
                                {
                                if (sh_load[j].factor_record >= 0) {
                                    combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                                    load = sh_load_factors[sh_load[j].factor_record].load;
                                } else {
                                    combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                                    load = load_factors[abs(sh_load[j].factor_record)].load;
                                }

                                int combi_factor;
                                if (ret_standard == 1)  //EUROCODE
                                    combi_factor = eurocode_combi_factors_uls[ci][load];
                                else if (ret_standard == 2)  //ASCE
                                    combi_factor = asce_combi_factors_uls[ci][load];
                                if (ret_standard == 3)  //ICC
                                    combi_factor = icc_combi_factors_uls[ci][load];

                                gamma_l = 1.0;
                                if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                                if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                                if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                                if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                                if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                                if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                                set_decimal_format(par[1], -sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f / sh_property[this_property].h,
                                                   load_precision);  //is assumed that magnitude1=magnitude2
                                if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                                strcat(load_formula, "(");
                                strcat(load_formula, par[1]);
                                strcat(load_formula, ")");

                                surface_load[case_number-1][zone_property[i].k] += (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f);
                            }
                        }
                    }
                }
                else //property zone, inherits from shield 0 + zona local load
                {
                    for (j = 0; j < sh_load_no; j++) //all load
                    {
                        if ((sh_load[j].take_it == 1) && //active
                            (sh_load[j].type == 1) &&   //uniformly distributed on area
                            (((sh_load[j].body == 3) && (sh_load[j].body_no == i)) ||  //local load on zone and number
                             (sh_load[j].body == 0) && (sh_load[j].body_no ==  zone_property[i].pn)))  //inherited load on shield and its number
                        {
                            if (sh_load[j].factor_record >= 0) {
                                combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                                load = sh_load_factors[sh_load[j].factor_record].load;
                            } else {
                                combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                                load = load_factors[abs(sh_load[j].factor_record)].load;
                            }

                            int combi_factor;
                            if (ret_standard == 1)  //EUROCODE
                                combi_factor = eurocode_combi_factors_uls[ci][load];
                            else if (ret_standard == 2)  //ASCE
                                combi_factor = asce_combi_factors_uls[ci][load];
                            if (ret_standard == 3)  //ICC
                                combi_factor = icc_combi_factors_uls[ci][load];

                            gamma_l = 1.0;
                            if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                            if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                            if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                            if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                            if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                            if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                            set_decimal_format(par[1], -sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f / sh_property[this_property].h,
                                               load_precision);  //is assumed that magnitude1=magnitude2
                            if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                            strcat(load_formula, "(");
                            strcat(load_formula, par[1]);
                            strcat(load_formula, ")");

                            surface_load[case_number-1][zone_property[i].k] += (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f);
                        }
                    }
                }
                if (strlen(load_formula) > 0)
                {
                    sprintf(all_load_formula, "  Body Force 2 = Real MATC \"%s\"\n", load_formula);
                    strcat(all_formula, all_load_formula);
                }

                ///////////////// Concentrated Load
                strcpy(load_formula, "");
                int f_no=0;

                for (j = 0; j < sh_load_no; j++) //all load
                {
                    if ((sh_load[j].take_it == 1) && //active
                        (sh_load[j].type==0) &&  //concentrated
                        (sh_load[j].body == 3) &&  //zone
                        (sh_load[j].body_no == i)) //zone number
                    {
                        if (sh_load[j].factor_record >= 0)
                        {
                            combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                            load = sh_load_factors[sh_load[j].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                            load = load_factors[abs(sh_load[j].factor_record)].load;
                        }

                        int combi_factor;
                        if (ret_standard == 1)  //EUROCODE
                            combi_factor = eurocode_combi_factors_uls[ci][load];
                        else if (ret_standard == 2)  //ASCE
                            combi_factor = asce_combi_factors_uls[ci][load];
                        if (ret_standard == 3)  //ICC
                            combi_factor = icc_combi_factors_uls[ci][load];

                        gamma_l = 1.0;
                        if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                        if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                        if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                        if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                        if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                        if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                        strcpy(load_formula0, "");
                        if ((sh_load[j].style==4) || (sh_load[j].style==18)) //Fx, Fy
                        {
                            set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[2], sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            set_decimal_format(par[3], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            sprintf(load_formula0, " %s %s 0.0 %s %s 0.0 0.0 0.0 0.0", par[0], par[1], par[2], par[3]);

                            point_load_x[case_number - 1] += (sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f);
                            point_load_y[case_number - 1] -= (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f);
                            f_no++;
                        }
                        else if ((sh_load[j].style==5) || (sh_load[j].style==6))  //Mxy  //-Mxy
                        {
                            set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[2], sh_load[j].magnitude1x * unit_factors_sh->M_f, moment_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            sprintf(load_formula0, " %s %s 0.0 0.0 0.0 0.0 0.0 0.0 %s", par[0], par[1], par[2]);  //x,y,z,Fx,Fy,Fz,Mx,My,Mz
                            f_no++;
                        }
                        strcat(load_formula, load_formula0);
                    }
                }

                if (f_no>0)
                {
                    //sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                    sprintf(all_load_formula, "  Point Load (%d, 9) = Real %s\n", f_no, load_formula);
                    strcat(all_formula, all_load_formula);
                }
                //////// concentrated load

                set_decimal_format(par[0], sh_property[this_property].h, dim_precision_sh);
                sprintf(par[1], "  Thickness = Real %s\n", par[0]);
                sprintf(par[2], "End\n\n");
                strcat(all_formula, par[1]);
                strcat(all_formula, par[2]);

                sif_body_force++;
            }

            for (i = 0; i < shield_no; i++)
            {
                //searching for properties
                for (j = 0; j < sh_property_no; j++) {
                    if (sh_property[j].n == shield_property[i].property_number) {
                        this_property = j;
                        break;
                    }
                }

                /////Distributed load on edge
                if (shield_variable_load_no[i]>0)  //we know from SLS
                {
                    shield_case_variable_load_no[i]=0;
                    /*
                    for (j = 0; j < sh_load_no; j++)
                    {
                        if ((sh_load[j].body == 0) && (sh_load[j].body_no == i)) {
                            if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                            {
                                if (sh_load[j].factor_record >= 0) {
                                    gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                                    flag = load_flag[sh_load_factors[sh_load[j].factor_record].load];
                                } else {
                                    gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;
                                    flag = load_flag[load_factors[abs(sh_load[j].factor_record)].load];
                                }
                                if (flag & ULSLC[ci].mask) {
                                    sh_load[j].take_it = 1;
                                    shield_case_variable_load_no[i]++;
                                } else sh_load[j].take_it = 0;
                            }
                        }
                    }
                    */
                    for (j = 0; j < sh_load_no; j++)
                    {
                        if ((sh_load[j].body == 0) && (sh_load[j].body_no == i)) {
                            if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                            {
                                if (sh_load[j].take_it == 0)
                                {
                                    sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_shield[i][j] + 1);
                                    sprintf(par[1], "  Active Boundary Condition = Logical False\n");
                                    sprintf(par[2], "End\n\n");
                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[2]);
                                    continue;
                                }

                                if (sh_load[j].factor_record >= 0) {
                                    combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                                    load = sh_load_factors[sh_load[j].factor_record].load;
                                } else {
                                    combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                                    load = load_factors[abs(sh_load[j].factor_record)].load;
                                }

                                int combi_factor;
                                if (ret_standard == 1)  //EUROCODE
                                    combi_factor = eurocode_combi_factors_uls[ci][load];
                                else if (ret_standard == 2)  //ASCE
                                    combi_factor = asce_combi_factors_uls[ci][load];
                                if (ret_standard == 3)  //ICC
                                    combi_factor = icc_combi_factors_uls[ci][load];

                                gamma_l = 1.0;
                                if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                                if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                                if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                                if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                                if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                                if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                                ////Load on boundaries
                                sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_shield[i][j] + 1);
                                sprintf(par[1], "  Name = \"Boundary Loads %d\"\n", sif_boundary_condition_sls_shield[i][j] + 1);

                                sprintf(par[2], "  Target Boundaries(1) = %d\n", sh_load[j].k + 1);
                                sprintf(par[3], "  ! trapezoidal load\n");
                                sprintf(par[4], "  Name = \"VariableLoad%d\"\n", shield_variable_load_no[i] + 1);

                                BOOL shield_case_variable_load_x=FALSE;
                                BOOL shield_case_variable_load_y=FALSE;
                                //component Qx
                                if ((Check_if_Equal(sh_load[j].magnitude1x, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2x, 0.0) == FALSE)) {
                                    set_decimal_format(parf[0], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[1], sh_load[j].magnitude1x * unit_factors_sh->q_f, force_precision);
                                    set_decimal_format(parf[2], milimetryoby(sh_load[j].y2) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[3], sh_load[j].magnitude2x * unit_factors_sh->q_f, force_precision);
                                    sprintf(par[5], "  Force 1 = Variable Coordinate 2\n");
                                    sprintf(par[6], "  Real\n");
                                    sprintf(par[7], "  %s %s\n", parf[0], parf[1]);
                                    sprintf(par[8], "  %s %s\n", parf[2], parf[3]);
                                    sprintf(par[9], "  End\n");
                                    shield_case_variable_load_x=TRUE;
                                }
                                //component Qy
                                if ((Check_if_Equal(sh_load[j].magnitude1y, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2y, 0.0) == FALSE)) {
                                    set_decimal_format(parf[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[1], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f, force_precision);
                                    set_decimal_format(parf[2], milimetryobx(sh_load[j].x2) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[3], sh_load[j].magnitude2y * gamma_l * unit_factors_sh->q_f, force_precision);
                                    sprintf(par[10], "  Force 2 = Variable Coordinate 1\n");
                                    sprintf(par[11], "  Real\n");
                                    sprintf(par[12], "  %s %s\n", parf[0], parf[1]);
                                    sprintf(par[13], "  %s %s\n", parf[2], parf[3]);
                                    sprintf(par[14], "  End\n");
                                    shield_case_variable_load_y=TRUE;
                                }

                                double ddlx = fabs((double) (milimetryobx(sh_load[j].x2) - milimetryobx(sh_load[j].x1))) * geo_units_factor;
                                double ddly = fabs((double) (milimetryoby(sh_load[j].y2) - milimetryoby(sh_load[j].y1))) * geo_units_factor;
                                distributed_load_x[case_number - 1] += ((sh_load[j].magnitude1x + sh_load[j].magnitude2x) * gamma_l * unit_factors_sh->q_f) * ddly / 2.;
                                distributed_load_y[case_number - 1] += ((sh_load[j].magnitude1y + sh_load[j].magnitude2y) * gamma_l * unit_factors_sh->q_f) * ddlx / 2.;

                                sprintf(par[15], "End\n\n");

                                if ((shield_case_variable_load_x) || (shield_case_variable_load_y))
                                {

                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[3]);
                                    strcat(all_formula, par[4]);
                                    if (shield_case_variable_load_x)
                                    {
                                        strcat(all_formula, par[5]);
                                        strcat(all_formula, par[6]);
                                        strcat(all_formula, par[7]);
                                        strcat(all_formula, par[8]);
                                        strcat(all_formula, par[9]);
                                    }
                                    if (shield_case_variable_load_y)
                                    {
                                        strcat(all_formula, par[10]);
                                        strcat(all_formula, par[11]);
                                        strcat(all_formula, par[12]);
                                        strcat(all_formula, par[13]);
                                        strcat(all_formula, par[14]);
                                    }
                                    strcat(all_formula, par[15]);
                                    shield_case_variable_load_no[i]++;
                                }
                                else
                                {
                                    sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_shield[i][j] + 1);
                                    sprintf(par[1], "  Active Boundary Condition = Logical False\n");
                                    sprintf(par[2], "End\n\n");
                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[2]);
                                }
                            }
                        }
                    }
                }
                /////////////////  distributed load on edge

                sprintf(par[0], "Body Force %d\n", sif_body_shield[i] + 1);
                sprintf(par[1], "  Name = \"BodyForce shield%d\"\n", sif_body_shield[i] + 1);
                strcat(all_formula, par[0]);
                strcat(all_formula, par[1]);

                strcpy(load_formula, "");

                //self_weight = 0.0;
                self_weight = gZ * gamma_g * sh_property[this_property].d; // * sh_property[this_property].h;  in Body Force 2 it will be multiplied by thickness by solver
                if (self_weight > 0.0) {
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][shield_property[i].k] += (self_weight * sh_property[this_property].h);
                }

                for (j = 0; j < sh_load_no; j++) //all load
                {
                    if ((sh_load[j].take_it == 1) && //active
                        (sh_load[j].type == 1) &&  //uniformly distributed on area
                        (sh_load[j].body == 0) &&  //shield
                        (sh_load[j].body_no == i)) //shield number
                    {
                        if (sh_load[j].factor_record >= 0)
                        {
                            combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                            load = sh_load_factors[sh_load[j].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                            load = load_factors[abs(sh_load[j].factor_record)].load;
                        }

                        int combi_factor;
                        if (ret_standard == 1)  //EUROCODE
                            combi_factor = eurocode_combi_factors_uls[ci][load];
                        else if (ret_standard == 2)  //ASCE
                            combi_factor = asce_combi_factors_uls[ci][load];
                        if (ret_standard == 3)  //ICC
                            combi_factor = icc_combi_factors_uls[ci][load];

                        gamma_l = 1.0;
                        if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                        if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                        if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                        if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                        if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                        if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                        set_decimal_format(par[1], -sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f / sh_property[this_property].h,
                                           load_precision);  //is assumed that magnitude1=magnitude2
                        //fprintf(f, "  Normal Pressure = %s\n", par[1]);
                        if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                        strcat(load_formula, "(");
                        strcat(load_formula, par[1]);
                        strcat(load_formula, ")");

                        surface_load[case_number-1][shield_property[i].k] += (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f);
                    }
                }
                if (strlen(load_formula) > 0)
                {
                    sprintf(all_load_formula, "  Body Force 2 = Real MATC \"%s\"\n", load_formula);
                    strcat(all_formula, all_load_formula);
                }

                ///////////////// Concentrated Load
                strcpy(load_formula, "");
                int f_no=0;

                for (j = 0; j < sh_load_no; j++) //all load
                {
                    if ((sh_load[j].take_it == 1) &&  //active
                        (sh_load[j].type==0) && //concentrated
                        (sh_load[j].body == 0) &&  //shield
                        (sh_load[j].body_no == i)) //shield number
                    {
                        if (sh_load[j].factor_record >= 0)
                        {
                            combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                            load = sh_load_factors[sh_load[j].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                            load = load_factors[abs(sh_load[j].factor_record)].load;
                        }

                        int combi_factor;
                        if (ret_standard == 1)  //EUROCODE
                            combi_factor = eurocode_combi_factors_uls[ci][load];
                        else if (ret_standard == 2)  //ASCE
                            combi_factor = asce_combi_factors_uls[ci][load];
                        if (ret_standard == 3)  //ICC
                            combi_factor = icc_combi_factors_uls[ci][load];

                        gamma_l = 1.0;
                        if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                        if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                        if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                        if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                        if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                        if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                        strcpy(load_formula0, "");
                        if ((sh_load[j].style==4) || (sh_load[j].style==18)) //Fx, Fy
                        {
                            set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[2], sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            set_decimal_format(par[3], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            sprintf(load_formula0, " %s %s 0.0 %s %s 0.0 0.0 0.0 0.0", par[0], par[1], par[2], par[3]);
                            point_load_x[case_number - 1] += (sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f);
                            point_load_y[case_number - 1] -= (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f);
                            f_no++;
                        }
                        else if ((sh_load[j].style==5) || (sh_load[j].style==6))  //Mxy  //-Mxy
                        {
                            set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[2], sh_load[j].magnitude1x * unit_factors_sh->M_f, moment_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            sprintf(load_formula0, " %s %s 0.0 0.0 0.0 0.0 0.0 0.0 %s", par[0], par[1], par[2]);  //x,y,z,Fx,Fy,Fz,Mx,My,Mz
                            f_no++;
                        }
                        strcat(load_formula, load_formula0);
                    }
                }

                if (f_no>0)
                {
                    //sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                    sprintf(all_load_formula, "  Point Load (%d, 9) = Real %s\n", f_no, load_formula);
                    strcat(all_formula, all_load_formula);
                }
                //////// concentrated load

                set_decimal_format(par[0], sh_property[this_property].h, dim_precision_sh);
                sprintf(par[1], "  Thickness = Real %s\n", par[0]);
                sprintf(par[2], "End\n\n");
                strcat(all_formula, par[1]);
                strcat(all_formula, par[2]);

                sif_body_force++;
            }

            go_ahead = TRUE;
            if (all_formula_bak != NULL) {
                if (strcmp(all_formula_bak, all_formula) == 0)
                    go_ahead = FALSE;
            }

            if ((strlen(all_formula)>0) &&(go_ahead)) // || (total_zone_variable_load_no>0) || (total_shield_variable_load_no>0))
            {
                fprintf(f, "Simulation::Output File = \"shield_ulslc%d.result\"\n", ci+1);
                combi_ulslc_ino++;
                if (go_ahead)
                {
                    fprintf(f, "%s", all_formula);
                }
                ULSLC[ci].flag = 2;
                fprintf(f, "RUN\n\n");
            }

            all_formula_bak = all_formula;
            current = !current;
            all_formula = all_formulas[current];
            ////ULSLC ENDS
        }
    }

    ////LOAD VARIANTS SLSLC BEGINS
    case_files[3]=case_number;
    combi_slslc_ino = 0;

    for (int ci = 0; ci < SLSLC_NO; ci++)
    {
        case_number++;  //index is changing always, no matter case will be executed

        if (SLSLC[ci].flag == 1)
        {
            int load;
            int flag;
            ST_LOAD_FACTORS *combi_load_factor;
            /////////////////////////////
            int combi_factor_g;
            if (ret_standard == 1)  //EUROCODE
                combi_factor_g = eurocode_combi_factors_sls[ci][1];
            else if (ret_standard == 2)  //ASCE
                combi_factor_g = asce_combi_factors_sls[ci][1];
            if (ret_standard == 3)  //ICC
                combi_factor_g = icc_combi_factors_sls[ci][1];
            double gamma_g = 1.0;

            if (combi_factor_g & 1) gamma_g *= gammas->gamma_g;
            if (combi_factor_g & 2) gamma_g *= gammas->psi0;
            if (combi_factor_g & 4) gamma_g *= gammas->psi1;
            if (combi_factor_g & 8) gamma_g *= gammas->psi2;
            if (combi_factor_g & 16) gamma_g *= gammas->xi_g;
            if (combi_factor_g & 32) gamma_g *= gammas->gamma_gi;

            sif_body_force = 0;

            strcpy(all_formula, "");


            for (j = 0; j < sh_load_no; j++)
            {
                if (sh_load[j].factor_record >= 0) {
                    gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                    flag = load_flag[sh_load_factors[sh_load[j].factor_record].load];
                } else {
                    gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;
                    flag = load_flag[load_factors[abs(sh_load[j].factor_record)].load];
                }
                if (flag & SLSLC[ci].mask) {
                    sh_load[j].take_it = 1;
                    zone_case_variable_load_no[i]++;
                } else sh_load[j].take_it = 0;
            }

            for (i = 0; i < zone_no; i++)
            {
                //searching for properties
                for (j = 0; j < sh_property_no; j++) {
                    if (sh_property[j].n == zone_property[i].property_number) {
                        this_property = j;
                        break;
                    }
                }

                /////Distributed load on edge
                zone_case_variable_load_no[i]=0;
                if (zone_variable_load_no[i]>0)  //we know from SLS
                {
                    /*
                    for (j = 0; j < sh_load_no; j++)
                    {
                        if ((sh_load[j].body == 3) && (sh_load[j].body_no == i)) {
                            if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                            {
                                if (sh_load[j].factor_record >= 0) {
                                    gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                                    flag = load_flag[sh_load_factors[sh_load[j].factor_record].load];
                                } else {
                                    gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;
                                    flag = load_flag[load_factors[abs(sh_load[j].factor_record)].load];
                                }
                                if (flag & ULSLC[ci].mask) {
                                    sh_load[j].take_it = 1;
                                    zone_case_variable_load_no[i]++;
                                } else sh_load[j].take_it = 0;
                            }
                        }
                    }
                     */

                    for (j = 0; j < sh_load_no; j++)
                    {
                        if ((sh_load[j].body == 3) && (sh_load[j].body_no == i)) {
                            if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                            {
                                if (sh_load[j].take_it == 0)
                                {
                                    sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_zone[i][j] + 1);
                                    sprintf(par[1], "  Active Boundary Condition = Logical False\n");
                                    sprintf(par[2], "End\n\n");
                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[2]);
                                    continue;
                                }
                                if (sh_load[j].factor_record >= 0) {
                                    combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                                    load = sh_load_factors[sh_load[j].factor_record].load;
                                } else {
                                    combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                                    load = load_factors[abs(sh_load[j].factor_record)].load;
                                }

                                int combi_factor;
                                if (ret_standard == 1)  //EUROCODE
                                    combi_factor = eurocode_combi_factors_sls[ci][load];
                                else if (ret_standard == 2)  //ASCE
                                    combi_factor = asce_combi_factors_sls[ci][load];
                                if (ret_standard == 3)  //ICC
                                    combi_factor = icc_combi_factors_sls[ci][load];

                                gamma_l = 1.0;
                                if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                                if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                                if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                                if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                                if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                                if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                                ////Load on boundaries
                                sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_zone[i][j] + 1);
                                sprintf(par[1], "  Name = \"Boundary Loads %d\"\n", sif_boundary_condition_sls_zone[i][j] + 1);

                                sprintf(par[2], "  Target Boundaries(1) = %d\n", sh_load[j].k + 1);
                                sprintf(par[3], "  ! trapezoidal load\n");
                                sprintf(par[4], "  Name = \"VariableLoad%d\"\n", zone_variable_load_no[i] + 1);

                                BOOL zone_case_variable_load_x=FALSE;
                                BOOL zone_case_variable_load_y=FALSE;
                                //component Qx
                                if ((Check_if_Equal(sh_load[j].magnitude1x, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2x, 0.0) == FALSE)) {
                                    set_decimal_format(parf[0], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[1], sh_load[j].magnitude1x * unit_factors_sh->q_f, force_precision);
                                    set_decimal_format(parf[2], milimetryoby(sh_load[j].y2) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[3], sh_load[j].magnitude2x * unit_factors_sh->q_f, force_precision);
                                    sprintf(par[5], "  Force 1 = Variable Coordinate 2\n");
                                    sprintf(par[6], "  Real\n");
                                    sprintf(par[7], "  %s %s\n", parf[0], parf[1]);
                                    sprintf(par[8], "  %s %s\n", parf[2], parf[3]);
                                    sprintf(par[9], "  End\n");
                                    zone_case_variable_load_x=TRUE;
                                }
                                //compnent Qy
                                if ((Check_if_Equal(sh_load[j].magnitude1y, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2y, 0.0) == FALSE)) {
                                    set_decimal_format(parf[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[1], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f, force_precision);
                                    set_decimal_format(parf[2], milimetryobx(sh_load[j].x2) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[3], sh_load[j].magnitude2y * gamma_l * unit_factors_sh->q_f, force_precision);
                                    sprintf(par[10], "  Force 2 = Variable Coordinate 1\n");
                                    sprintf(par[11], "  Real\n");
                                    sprintf(par[12], "  %s %s\n", parf[0], parf[1]);
                                    sprintf(par[13], "  %s %s\n", parf[2], parf[3]);
                                    sprintf(par[14], "  End\n");
                                    zone_case_variable_load_y=TRUE;
                                }

                                double ddlx = fabs((double) (milimetryobx(sh_load[j].x2) - milimetryobx(sh_load[j].x1))) * geo_units_factor;
                                double ddly = fabs((double) (milimetryoby(sh_load[j].y2) - milimetryoby(sh_load[j].y1))) * geo_units_factor;
                                distributed_load_x[case_number - 1] += ((sh_load[j].magnitude1x + sh_load[j].magnitude2x) * gamma_l * unit_factors_sh->q_f) * ddly / 2.;
                                distributed_load_y[case_number - 1] += ((sh_load[j].magnitude1y + sh_load[j].magnitude2y) * gamma_l * unit_factors_sh->q_f) * ddlx / 2.;

                                sprintf(par[15], "End\n\n");

                                if ((zone_case_variable_load_x) || (zone_case_variable_load_y))
                                {

                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[3]);
                                    strcat(all_formula, par[4]);
                                    if (zone_case_variable_load_x)
                                    {
                                        strcat(all_formula, par[5]);
                                        strcat(all_formula, par[6]);
                                        strcat(all_formula, par[7]);
                                        strcat(all_formula, par[8]);
                                        strcat(all_formula, par[9]);
                                    }
                                    if (zone_case_variable_load_y)
                                    {
                                        strcat(all_formula, par[10]);
                                        strcat(all_formula, par[11]);
                                        strcat(all_formula, par[12]);
                                        strcat(all_formula, par[13]);
                                        strcat(all_formula, par[14]);
                                    }
                                    strcat(all_formula, par[15]);
                                    zone_case_variable_load_no[i]++;
                                }
                                else
                                {
                                    sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_zone[i][j] + 1);
                                    sprintf(par[1], "  Active Boundary Condition = Logical False\n");
                                    sprintf(par[2], "End\n\n");
                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[2]);
                                }
                            }
                        }
                    }
                }
                /////////////////  distributed load on edge

                sprintf(par[0], "Body Force %d\n", sif_body_zone[i] + 1);
                sprintf(par[1], "  Name = \"BodyForce zone%d\"\n", sif_body_zone[i] + 1);
                strcat(all_formula, par[0]);
                strcat(all_formula, par[1]);

                strcpy(load_formula, "");

                //self_weight = 0.0;
                self_weight = gZ * gamma_g * sh_property[this_property].d; // * sh_property[this_property].h;  in Body Force 2 it will be multiplied by thickness by solver
                if (self_weight > 0.0) {
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][zone_property[i].k] += (self_weight * sh_property[this_property].h);
                }

                if (zone_property[i].property_number==shield_property[0].property_number)  //load zone
                {
                    if ((zone_property[i].load == 0) && (self_weight == 0.0)) //no load
                    {
                        if (self_weight == 0.0) //no load, no self load
                        {
                            //load simulation
                            set_decimal_format(par[1], 0, load_precision);
                            strcat(load_formula, "(");
                            strcat(load_formula, par[1]);
                            strcat(load_formula, ")");
                        }
                    } else {
                        for (j = 0; j < sh_load_no; j++) //all load
                        {
                            if ((sh_load[j].take_it == 1) && //active
                                (sh_load[j].type == 1) && //uniformly distributed
                                (sh_load[j].body == 3) && //zone
                                (sh_load[j].body_no == i)) //zone number
                            {
                                if (sh_load[j].factor_record >= 0) {
                                    combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                                    load = sh_load_factors[sh_load[j].factor_record].load;
                                } else {
                                    combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                                    load = load_factors[abs(sh_load[j].factor_record)].load;
                                }

                                int combi_factor;
                                if (ret_standard == 1)  //EUROCODE
                                    combi_factor = eurocode_combi_factors_sls[ci][load];
                                else if (ret_standard == 2)  //ASCE
                                    combi_factor = asce_combi_factors_sls[ci][load];
                                if (ret_standard == 3)  //ICC
                                    combi_factor = icc_combi_factors_sls[ci][load];

                                gamma_l = 1.0;
                                if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                                if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                                if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                                if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                                if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                                if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                                set_decimal_format(par[1], -sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f / sh_property[this_property].h,
                                                   load_precision);  //is assumed that magnitude1=magnitude2
                                if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                                strcat(load_formula, "(");
                                strcat(load_formula, par[1]);
                                strcat(load_formula, ")");

                                surface_load[case_number-1][zone_property[i].k] += (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f);
                            }
                        }
                    }
                }
                else //property zone, inherits from shield 0 + zona local load
                {
                    for (j = 0; j < sh_load_no; j++) //all load
                    {
                        if ((sh_load[j].take_it == 1) && //active
                            (sh_load[j].type == 1) &&   //uniformly distributed on area
                            (((sh_load[j].body == 3) && (sh_load[j].body_no == i)) ||  //local load on zone and zone number
                             (sh_load[j].body == 0) && (sh_load[j].body_no ==  zone_property[i].pn)))  //inherited load on shield and shield number
                        {
                            if (sh_load[j].factor_record >= 0) {
                                combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                                load = sh_load_factors[sh_load[j].factor_record].load;
                            } else {
                                combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                                load = load_factors[abs(sh_load[j].factor_record)].load;
                            }

                            int combi_factor;
                            if (ret_standard == 1)  //EUROCODE
                                combi_factor = eurocode_combi_factors_sls[ci][load];
                            else if (ret_standard == 2)  //ASCE
                                combi_factor = asce_combi_factors_sls[ci][load];
                            if (ret_standard == 3)  //ICC
                                combi_factor = icc_combi_factors_sls[ci][load];

                            gamma_l = 1.0;
                            if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                            if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                            if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                            if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                            if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                            if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                            set_decimal_format(par[1], -sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f / sh_property[this_property].h,
                                               load_precision);  //is assumed that magnitude1=magnitude2
                            if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                            strcat(load_formula, "(");
                            strcat(load_formula, par[1]);
                            strcat(load_formula, ")");

                            surface_load[case_number-1][zone_property[i].k] += (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f);
                        }
                    }

                }
                if (strlen(load_formula) > 0)
                {
                    sprintf(all_load_formula, "  Body Force 2 = Real MATC \"%s\"\n", load_formula);
                    strcat(all_formula, all_load_formula);
                }

                ///////////////// Concentrated Load
                strcpy(load_formula, "");
                int f_no=0;

                for (j = 0; j < sh_load_no; j++) //all load
                {
                    if ((sh_load[j].take_it == 1) && //active
                        (sh_load[j].type==0) && //concentrated
                        (sh_load[j].body == 3) &&  //zone
                        (sh_load[j].body_no == i)) //zone number
                    {
                        if (sh_load[j].factor_record >= 0)
                        {
                            combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                            load = sh_load_factors[sh_load[j].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                            load = load_factors[abs(sh_load[j].factor_record)].load;
                        }

                        int combi_factor;
                        if (ret_standard == 1)  //EUROCODE
                            combi_factor = eurocode_combi_factors_sls[ci][load];
                        else if (ret_standard == 2)  //ASCE
                            combi_factor = asce_combi_factors_sls[ci][load];
                        if (ret_standard == 3)  //ICC
                            combi_factor = icc_combi_factors_sls[ci][load];

                        gamma_l = 1.0;
                        if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                        if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                        if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                        if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                        if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                        if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                        strcpy(load_formula0, "");
                        if ((sh_load[j].style==4) || (sh_load[j].style==18)) //Fx, Fy
                        {
                            set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[2], sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            set_decimal_format(par[3], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            sprintf(load_formula0, " %s %s 0.0 %s %s 0.0 0.0 0.0 0.0", par[0], par[1], par[2], par[3]);

                            point_load_x[case_number - 1] += (sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f);
                            point_load_y[case_number - 1] -= (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f);
                            f_no++;
                        }
                        else if ((sh_load[j].style==5) || (sh_load[j].style==6))  //Mxy  //-Mxy
                        {
                            set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[2], sh_load[j].magnitude1x * unit_factors_sh->M_f, moment_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            sprintf(load_formula0, " %s %s 0.0 0.0 0.0 0.0 0.0 0.0 %s", par[0], par[1], par[2]);  //x,y,z,Fx,Fy,Fz,Mx,My,Mz
                            f_no++;
                        }
                        strcat(load_formula, load_formula0);
                    }
                }

                if (f_no>0)
                {
                    //sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                    sprintf(all_load_formula, "  Point Load (%d, 9) = Real %s\n", f_no, load_formula);
                    strcat(all_formula, all_load_formula);
                }
                //////// concentrated load

                set_decimal_format(par[0], sh_property[this_property].h, dim_precision_sh);
                sprintf(par[1], "  Thickness = Real %s\n", par[0]);
                sprintf(par[2], "End\n\n");
                strcat(all_formula, par[1]);
                strcat(all_formula, par[2]);

                sif_body_force++;
            }

            for (i = 0; i < shield_no; i++) {
                //searching for properties
                for (j = 0; j < sh_property_no; j++) {
                    if (sh_property[j].n == shield_property[i].property_number) {
                        this_property = j;
                        break;
                    }
                }

                /////Distributed load on edge
                shield_case_variable_load_no[i]=0;
                if (shield_variable_load_no[i]>0)  //we know from SLS
                {
                    /*
                    for (j = 0; j < sh_load_no; j++)
                    {
                        if ((sh_load[j].body == 0) && (sh_load[j].body_no == i)) {
                            if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                            {
                                if (sh_load[j].factor_record >= 0) {
                                    gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                                    flag = load_flag[sh_load_factors[sh_load[j].factor_record].load];
                                } else {
                                    gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;
                                    flag = load_flag[load_factors[abs(sh_load[j].factor_record)].load];
                                }
                                if (flag & ULSLC[ci].mask) {
                                    sh_load[j].take_it = 1;
                                    shield_case_variable_load_no[i]++;
                                } else sh_load[j].take_it = 0;
                            }
                        }
                    }
                     */

                    for (j = 0; j < sh_load_no; j++)
                    {
                        if ((sh_load[j].body == 0) && (sh_load[j].body_no == i))
                        {
                            if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                            {
                                if (sh_load[j].take_it == 0)
                                {
                                    sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_shield[i][j] + 1);
                                    sprintf(par[1], "  Active Boundary Condition = Logical False\n");
                                    sprintf(par[2], "End\n\n");
                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[2]);
                                    continue;
                                }

                                if (sh_load[j].factor_record >= 0) {
                                    combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                                    load = sh_load_factors[sh_load[j].factor_record].load;
                                } else {
                                    combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                                    load = load_factors[abs(sh_load[j].factor_record)].load;
                                }

                                int combi_factor;
                                if (ret_standard == 1)  //EUROCODE
                                    combi_factor = eurocode_combi_factors_sls[ci][load];
                                else if (ret_standard == 2)  //ASCE
                                    combi_factor = asce_combi_factors_sls[ci][load];
                                if (ret_standard == 3)  //ICC
                                    combi_factor = icc_combi_factors_sls[ci][load];

                                gamma_l = 1.0;
                                if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                                if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                                if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                                if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                                if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                                if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                                ////Load on boundaries
                                sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_shield[i][j] + 1);
                                sprintf(par[1], "  Name = \"Boundary Loads %d\"\n", sif_boundary_condition_sls_shield[i][j] + 1);

                                sprintf(par[2], "  Target Boundaries(1) = %d\n", sh_load[j].k + 1);
                                sprintf(par[3], "  ! trapezoidal load\n");
                                sprintf(par[4], "  Name = \"VariableLoad%d\"\n", shield_variable_load_no[i] + 1);

                                BOOL shield_case_variable_load_x=FALSE;
                                BOOL shield_case_variable_load_y=FALSE;
                                //component Qx
                                if ((Check_if_Equal(sh_load[j].magnitude1x, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2x, 0.0) == FALSE)) {
                                    set_decimal_format(parf[0], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[1], sh_load[j].magnitude1x * unit_factors_sh->q_f, force_precision);
                                    set_decimal_format(parf[2], milimetryoby(sh_load[j].y2) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[3], sh_load[j].magnitude2x * unit_factors_sh->q_f, force_precision);
                                    sprintf(par[5], "  Force 1 = Variable Coordinate 2\n");
                                    sprintf(par[6], "  Real\n");
                                    sprintf(par[7], "  %s %s\n", parf[0], parf[1]);
                                    sprintf(par[8], "  %s %s\n", parf[2], parf[3]);
                                    sprintf(par[9], "  End\n");
                                    shield_case_variable_load_x=TRUE;
                                }
                                //component Qy
                                if ((Check_if_Equal(sh_load[j].magnitude1y, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2y, 0.0) == FALSE)) {
                                    set_decimal_format(parf[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[1], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f, force_precision);
                                    set_decimal_format(parf[2], milimetryobx(sh_load[j].x2) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[3], sh_load[j].magnitude2y * gamma_l * unit_factors_sh->q_f, force_precision);
                                    sprintf(par[10], "  Force 2 = Variable Coordinate 1\n");
                                    sprintf(par[11], "  Real\n");
                                    sprintf(par[12], "  %s %s\n", parf[0], parf[1]);
                                    sprintf(par[13], "  %s %s\n", parf[2], parf[3]);
                                    sprintf(par[14], "  End\n");
                                    shield_case_variable_load_y=TRUE;
                                }

                                double ddlx = fabs((double) (milimetryobx(sh_load[j].x2) - milimetryobx(sh_load[j].x1))) * geo_units_factor;
                                double ddly = fabs((double) (milimetryoby(sh_load[j].y2) - milimetryoby(sh_load[j].y1))) * geo_units_factor;
                                distributed_load_x[case_number - 1] += ((sh_load[j].magnitude1x + sh_load[j].magnitude2x) * gamma_l * unit_factors_sh->q_f) * ddly / 2.;
                                distributed_load_y[case_number - 1] += ((sh_load[j].magnitude1y + sh_load[j].magnitude2y) * gamma_l * unit_factors_sh->q_f) * ddlx / 2.;

                                sprintf(par[15], "End\n\n");

                                if ((shield_case_variable_load_x) || (shield_case_variable_load_y))
                                {

                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[3]);
                                    strcat(all_formula, par[4]);
                                    if (shield_case_variable_load_x)
                                    {
                                        strcat(all_formula, par[5]);
                                        strcat(all_formula, par[6]);
                                        strcat(all_formula, par[7]);
                                        strcat(all_formula, par[8]);
                                        strcat(all_formula, par[9]);
                                    }
                                    if (shield_case_variable_load_y)
                                    {
                                        strcat(all_formula, par[10]);
                                        strcat(all_formula, par[11]);
                                        strcat(all_formula, par[12]);
                                        strcat(all_formula, par[13]);
                                        strcat(all_formula, par[14]);
                                    }
                                    strcat(all_formula, par[15]);
                                    shield_case_variable_load_no[i]++;
                                }
                                else
                                {
                                    sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_shield[i][j] + 1);
                                    sprintf(par[1], "  Active Boundary Condition = Logical False\n");
                                    sprintf(par[2], "End\n\n");
                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[2]);
                                }
                            }
                        }
                    }
                }
                /////////////////  distributed load on edge

                sprintf(par[0], "Body Force %d\n", sif_body_shield[i] + 1);
                sprintf(par[1], "  Name = \"BodyForce shield%d\"\n", sif_body_shield[i] + 1);
                strcat(all_formula, par[0]);
                strcat(all_formula, par[1]);

                strcpy(load_formula, "");

                //self_weight = 0.0;
                self_weight = gZ * gamma_g * sh_property[this_property].d; // * sh_property[this_property].h;  in Body Force 2 it will be multiplied by thickness by solver
                if (self_weight > 0.0) {
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][shield_property[i].k] += (self_weight * sh_property[this_property].h);
                }

                for (j = 0; j < sh_load_no; j++) //all load
                {
                    if ((sh_load[j].take_it == 1) && //active
                        (sh_load[j].type == 1) && //uniformly distributed on area
                        (sh_load[j].body == 0) && //shield
                        (sh_load[j].body_no == i)) //shield number
                    {
                        if (sh_load[j].factor_record >= 0)
                        {
                            combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                            load = sh_load_factors[sh_load[j].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                            load = load_factors[abs(sh_load[j].factor_record)].load;
                        }

                        int combi_factor;
                        if (ret_standard == 1)  //EUROCODE
                            combi_factor = eurocode_combi_factors_sls[ci][load];
                        else if (ret_standard == 2)  //ASCE
                            combi_factor = asce_combi_factors_sls[ci][load];
                        if (ret_standard == 3)  //ICC
                            combi_factor = icc_combi_factors_sls[ci][load];

                        gamma_l = 1.0;
                        if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                        if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                        if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                        if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                        if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                        if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                        set_decimal_format(par[1], -sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f / sh_property[this_property].h,
                                           load_precision);  //is assumed that magnitude1=magnitude2
                        //fprintf(f, "  Normal Pressure = %s\n", par[1]);
                        if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                        strcat(load_formula, "(");
                        strcat(load_formula, par[1]);
                        strcat(load_formula, ")");

                        surface_load[case_number-1][shield_property[i].k] += (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f);
                    }
                }
                if (strlen(load_formula) > 0)
                {
                    sprintf(all_load_formula, "  Body Force 2 = Real MATC \"%s\"\n", load_formula);
                    strcat(all_formula, all_load_formula);
                }

                ///////////////// Concentrated Load
                strcpy(load_formula, "");
                int f_no=0;

                for (j = 0; j < sh_load_no; j++) //all load
                {
                    if ((sh_load[j].take_it == 1) && //active
                        (sh_load[j].type==0) && //concentrated
                        (sh_load[j].body == 0) &&   //shield
                        (sh_load[j].body_no == i)) //shield number
                    {
                        if (sh_load[j].factor_record >= 0)
                        {
                            combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                            load = sh_load_factors[sh_load[j].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                            load = load_factors[abs(sh_load[j].factor_record)].load;
                        }

                        int combi_factor;
                        if (ret_standard == 1)  //EUROCODE
                            combi_factor = eurocode_combi_factors_sls[ci][load];
                        else if (ret_standard == 2)  //ASCE
                            combi_factor = asce_combi_factors_sls[ci][load];
                        if (ret_standard == 3)  //ICC
                            combi_factor = icc_combi_factors_sls[ci][load];

                        gamma_l = 1.0;
                        if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                        if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                        if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                        if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                        if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                        if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                        strcpy(load_formula0, "");
                        if ((sh_load[j].style==4) || (sh_load[j].style==18)) //Fx, Fy
                        {
                            set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[2], sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            set_decimal_format(par[3], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            sprintf(load_formula0, " %s %s 0.0 %s %s 0.0 0.0 0.0 0.0", par[0], par[1], par[2], par[3]);

                            point_load_x[case_number - 1] += (sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f);
                            point_load_y[case_number - 1] -= (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f);
                            f_no++;
                        }
                        else if ((sh_load[j].style==5) || (sh_load[j].style==6))  //Mxy  //-Mxy
                        {
                            set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[2], sh_load[j].magnitude1x * unit_factors_sh->M_f, moment_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            sprintf(load_formula0, " %s %s 0.0 0.0 0.0 0.0 0.0 0.0 %s", par[0], par[1], par[2]);  //x,y,z,Fx,Fy,Fz,Mx,My,Mz
                            f_no++;
                        }
                        strcat(load_formula, load_formula0);
                    }
                }

                if (f_no>0)
                {
                    //sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                    sprintf(all_load_formula, "  Point Load (%d, 9) = Real %s\n", f_no, load_formula);
                    strcat(all_formula, all_load_formula);
                }
                //////// concentrated load

                set_decimal_format(par[0], sh_property[this_property].h, dim_precision_sh);
                sprintf(par[1], "  Thickness = Real %s\n", par[0]);
                sprintf(par[2], "End\n\n");
                strcat(all_formula, par[1]);
                strcat(all_formula, par[2]);

                sif_body_force++;
            }

            go_ahead = TRUE;
            if (all_formula_bak != NULL) {
                if (strcmp(all_formula_bak, all_formula) == 0)
                    go_ahead = FALSE;
            }

            if ((strlen(all_formula)>0) &&(go_ahead)) //|| (total_zone_variable_load_no>0) || (total_shield_variable_load_no>0))
            {
                fprintf(f, "Simulation::Output File = \"shield_slslc%d.result\"\n", ci+1);
                if (go_ahead)
                {
                    combi_slslc_ino++;
                    fprintf(f, "%s",all_formula);
                    SLSLC[ci].flag = 2;
                }
                fprintf(f, "RUN\n\n");
            }

            all_formula_bak = all_formula;
            current = !current;
            all_formula = all_formulas[current];

            ////SLSLC ENDS
        }
    }

    ////LOAD VARIANTS QPSLSLC BEGINS
    case_files[4]=case_number;
    combi_qpslslc_ino = 0;

    for (int ci = 0; ci < QPSLSLC_NO; ci++)
    {
        case_number++;  //index is changing always, no matter case will be executed

        if (QPSLSLC[ci].flag == 1)
        {
            int load;
            int flag;
            ST_LOAD_FACTORS *combi_load_factor;
            /////////////////////////////
            int combi_factor_g;
            if (ret_standard == 1)  //EUROCODE
                combi_factor_g = eurocode_combi_factors_qpsls[ci][1];
            else if (ret_standard == 2)  //ASCE
                combi_factor_g = asce_combi_factors_qpsls[ci][1];
            if (ret_standard == 3)  //ICC
                combi_factor_g = icc_combi_factors_qpsls[ci][1];
            double gamma_g = 1.0;

            if (combi_factor_g & 1) gamma_g *= gammas->gamma_g;
            if (combi_factor_g & 2) gamma_g *= gammas->psi0;
            if (combi_factor_g & 4) gamma_g *= gammas->psi1;
            if (combi_factor_g & 8) gamma_g *= gammas->psi2;
            if (combi_factor_g & 16) gamma_g *= gammas->xi_g;
            if (combi_factor_g & 32) gamma_g *= gammas->gamma_gi;

            sif_body_force = 0;

            strcpy(all_formula, "");

            for (j = 0; j < sh_load_no; j++)
            {
                if (sh_load[j].factor_record >= 0) {
                    gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                    flag = load_flag[sh_load_factors[sh_load[j].factor_record].load];
                } else {
                    gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;
                    flag = load_flag[load_factors[abs(sh_load[j].factor_record)].load];
                }
                if (flag & QPSLSLC[ci].mask) {
                    sh_load[j].take_it = 1;
                    shield_case_variable_load_no[i]++;
                } else sh_load[j].take_it = 0;
            }

            for (i = 0; i < zone_no; i++) {
                //searching for properties
                for (j = 0; j < sh_property_no; j++) {
                    if (sh_property[j].n == zone_property[i].property_number) {
                        this_property = j;
                        break;
                    }
                }

                /////Distributed load on edge
                shield_case_variable_load_no[i]=0;
                if (zone_variable_load_no[i]>0)  //we know from SLS
                {
                    /*
                    for (j = 0; j < sh_load_no; j++)
                    {
                        if ((sh_load[j].body == 3) && (sh_load[j].body_no == i)) {
                            if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                            {
                                if (sh_load[j].factor_record >= 0) {
                                    gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                                    flag = load_flag[sh_load_factors[sh_load[j].factor_record].load];
                                } else {
                                    gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;
                                    flag = load_flag[load_factors[abs(sh_load[j].factor_record)].load];
                                }
                                if (flag & ULSLC[ci].mask) {
                                    sh_load[j].take_it = 1;
                                    shield_case_variable_load_no[i]++;
                                } else sh_load[j].take_it = 0;
                            }
                        }
                    }
                     */

                    for (j = 0; j < sh_load_no; j++)
                    {
                        if ((sh_load[j].body == 3) && (sh_load[j].body_no == i))
                        {
                            if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                            {
                                if (sh_load[j].take_it == 0)
                                {
                                    sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_zone[i][j] + 1);
                                    sprintf(par[1], "  Active Boundary Condition = Logical False\n");
                                    sprintf(par[2], "End\n\n");
                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[2]);
                                    continue;
                                }

                                if (sh_load[j].factor_record >= 0) {
                                    combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                                    load = sh_load_factors[sh_load[j].factor_record].load;
                                } else {
                                    combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                                    load = load_factors[abs(sh_load[j].factor_record)].load;
                                }

                                int combi_factor;
                                if (ret_standard == 1)  //EUROCODE
                                    combi_factor = eurocode_combi_factors_qpsls[ci][load];
                                else if (ret_standard == 2)  //ASCE
                                    combi_factor = asce_combi_factors_qpsls[ci][load];
                                if (ret_standard == 3)  //ICC
                                    combi_factor = icc_combi_factors_qpsls[ci][load];

                                gamma_l = 1.0;
                                if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                                if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                                if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                                if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                                if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                                if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                                ////Load on boundaries
                                sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_zone[i][j] + 1);
                                sprintf(par[1], "  Name = \"Boundary Loads %d\"\n", sif_boundary_condition_sls_zone[i][j] + 1);

                                sprintf(par[2], "  Target Boundaries(1) = %d\n", sh_load[j].k + 1);
                                sprintf(par[3], "  ! trapezoidal load\n");
                                sprintf(par[4], "  Name = \"VariableLoad%d\"\n", zone_variable_load_no[i] + 1);

                                BOOL zone_case_variable_load_x=FALSE;
                                BOOL zone_case_variable_load_y=FALSE;
                                //component Qx
                                if ((Check_if_Equal(sh_load[j].magnitude1x, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2x, 0.0) == FALSE)) {
                                    set_decimal_format(parf[0], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[1], sh_load[j].magnitude1x * unit_factors_sh->q_f, force_precision);
                                    set_decimal_format(parf[2], milimetryoby(sh_load[j].y2) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[3], sh_load[j].magnitude2x * unit_factors_sh->q_f, force_precision);
                                    sprintf(par[5], "  Force 1 = Variable Coordinate 2\n");
                                    sprintf(par[6], "  Real\n");
                                    sprintf(par[7], "  %s %s\n", parf[0], parf[1]);
                                    sprintf(par[8], "  %s %s\n", parf[2], parf[3]);
                                    sprintf(par[9], "  End\n");
                                    zone_case_variable_load_x=TRUE;
                                }
                                //compnent Qy
                                if ((Check_if_Equal(sh_load[j].magnitude1y, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2y, 0.0) == FALSE)) {
                                    set_decimal_format(parf[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[1], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f, force_precision);
                                    set_decimal_format(parf[2], milimetryobx(sh_load[j].x2) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[3], sh_load[j].magnitude2y * gamma_l * unit_factors_sh->q_f, force_precision);
                                    sprintf(par[10], "  Force 2 = Variable Coordinate 1\n");
                                    sprintf(par[11], "  Real\n");
                                    sprintf(par[12], "  %s %s\n", parf[0], parf[1]);
                                    sprintf(par[13], "  %s %s\n", parf[2], parf[3]);
                                    sprintf(par[14], "  End\n");
                                    zone_case_variable_load_y=TRUE;
                                }

                                double ddlx = fabs((double) (milimetryobx(sh_load[j].x2) - milimetryobx(sh_load[j].x1))) * geo_units_factor;
                                double ddly = fabs((double) (milimetryoby(sh_load[j].y2) - milimetryoby(sh_load[j].y1))) * geo_units_factor;
                                distributed_load_x[case_number - 1] += ((sh_load[j].magnitude1x + sh_load[j].magnitude2x) * gamma_l * unit_factors_sh->q_f) * ddly / 2.;
                                distributed_load_y[case_number - 1] += ((sh_load[j].magnitude1y + sh_load[j].magnitude2y) * gamma_l * unit_factors_sh->q_f) * ddlx / 2.;

                                sprintf(par[15], "End\n\n");

                                if ((zone_case_variable_load_x) || (zone_case_variable_load_y))
                                {

                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[3]);
                                    strcat(all_formula, par[4]);
                                    if (zone_case_variable_load_x)
                                    {
                                        strcat(all_formula, par[5]);
                                        strcat(all_formula, par[6]);
                                        strcat(all_formula, par[7]);
                                        strcat(all_formula, par[8]);
                                        strcat(all_formula, par[9]);
                                    }
                                    if (zone_case_variable_load_y)
                                    {
                                        strcat(all_formula, par[10]);
                                        strcat(all_formula, par[11]);
                                        strcat(all_formula, par[12]);
                                        strcat(all_formula, par[13]);
                                        strcat(all_formula, par[14]);
                                    }
                                    strcat(all_formula, par[15]);
                                    zone_case_variable_load_no[i]++;
                                }
                                else
                                {
                                    sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_zone[i][j] + 1);
                                    sprintf(par[1], "  Active Boundary Condition = Logical False\n");
                                    sprintf(par[2], "End\n\n");
                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[2]);
                                }
                            }
                        }
                    }
                }
                /////////////////  distributed load on edge


                sprintf(par[0], "Body Force %d\n", sif_body_zone[i] + 1);
                sprintf(par[1], "  Name = \"BodyForce zone%d\"\n", sif_body_zone[i] + 1);
                strcat(all_formula, par[0]);
                strcat(all_formula, par[1]);

                strcpy(load_formula, "");

                //self_weight = 0.0;
                self_weight = gZ * gamma_g * sh_property[this_property].d; // * sh_property[this_property].h;  in Body Force 2 it will be multiplied by thickness by solver
                if (self_weight > 0.0) {
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][zone_property[i].k] += (self_weight * sh_property[this_property].h);
                }

                if (zone_property[i].property_number==shield_property[0].property_number)  //load zone
                {
                    if ((zone_property[i].load == 0) && (self_weight == 0.0)) //no load
                    {
                        if (self_weight == 0.0) //no load, no self load
                        {
                            //load simulation
                            set_decimal_format(par[1], 0, load_precision);
                            strcat(load_formula, "(");
                            strcat(load_formula, par[1]);
                            strcat(load_formula, ")");
                        }
                    } else {
                        for (j = 0; j < sh_load_no; j++) //all load
                        {
                            if ((sh_load[j].take_it == 1) && //active
                                (sh_load[j].type == 1) && //uniformly distributed on area
                                (sh_load[j].body == 3) &&  //zone
                                (sh_load[j].body_no == i)) //zone number
                            {
                                if (sh_load[j].factor_record >= 0) {
                                    combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                                    load = sh_load_factors[sh_load[j].factor_record].load;
                                } else {
                                    combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                                    load = load_factors[abs(sh_load[j].factor_record)].load;
                                }

                                int combi_factor;
                                if (ret_standard == 1)  //EUROCODE
                                    combi_factor = eurocode_combi_factors_qpsls[ci][load];
                                else if (ret_standard == 2)  //ASCE
                                    combi_factor = asce_combi_factors_qpsls[ci][load];
                                if (ret_standard == 3)  //ICC
                                    combi_factor = icc_combi_factors_qpsls[ci][load];

                                gamma_l = 1.0;
                                if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                                if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                                if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                                if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                                if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                                if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                                set_decimal_format(par[1], -sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f / sh_property[this_property].h,
                                                   load_precision);  //is assumed that magnitude1=magnitude2
                                if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                                strcat(load_formula, "(");
                                strcat(load_formula, par[1]);
                                strcat(load_formula, ")");

                                surface_load[case_number-1][zone_property[i].k] += (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f);
                            }
                        }
                    }
                }
                else
                {
                    for (j = 0; j < sh_load_no; j++) //all load
                    {
                        if ((sh_load[j].take_it == 1) && //active
                            (sh_load[j].type == 1) &&   //uniformly distributed on area
                            (((sh_load[j].body == 3) && (sh_load[j].body_no == i)) ||  //local load on zone and number
                             (sh_load[j].body == 0) && (sh_load[j].body_no ==  zone_property[i].pn)))  //inherited load on shield and number
                        {
                            if (sh_load[j].factor_record >= 0) {
                                combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                                load = sh_load_factors[sh_load[j].factor_record].load;
                            } else {
                                combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                                load = load_factors[abs(sh_load[j].factor_record)].load;
                            }

                            int combi_factor;
                            if (ret_standard == 1)  //EUROCODE
                                combi_factor = eurocode_combi_factors_qpsls[ci][load];
                            else if (ret_standard == 2)  //ASCE
                                combi_factor = asce_combi_factors_qpsls[ci][load];
                            if (ret_standard == 3)  //ICC
                                combi_factor = icc_combi_factors_qpsls[ci][load];

                            gamma_l = 1.0;
                            if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                            if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                            if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                            if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                            if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                            if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                            set_decimal_format(par[1], -sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f / sh_property[this_property].h,
                                               load_precision);  //is assumed that magnitude1=magnitude2
                            if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                            strcat(load_formula, "(");
                            strcat(load_formula, par[1]);
                            strcat(load_formula, ")");

                            surface_load[case_number-1][zone_property[i].k] += (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f);
                        }
                    }
                }
                if (strlen(load_formula) > 0)
                {
                    sprintf(all_load_formula, "  Body Force 2 = Real MATC \"%s\"\n", load_formula);
                    strcat(all_formula, all_load_formula);
                }

                ///////////////// Concentrated Load
                strcpy(load_formula, "");
                int f_no=0;

                for (j = 0; j < sh_load_no; j++) //all load
                {
                    if ((sh_load[j].take_it == 1) && //active
                        (sh_load[j].type==0) && //concentrated
                        (sh_load[j].body == 3) && //zone
                        (sh_load[j].body_no == i)) //zone number
                    {
                        if (sh_load[j].factor_record >= 0)
                        {
                            combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                            load = sh_load_factors[sh_load[j].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                            load = load_factors[abs(sh_load[j].factor_record)].load;
                        }

                        int combi_factor;
                        if (ret_standard == 1)  //EUROCODE
                            combi_factor = eurocode_combi_factors_qpsls[ci][load];
                        else if (ret_standard == 2)  //ASCE
                            combi_factor = asce_combi_factors_qpsls[ci][load];
                        if (ret_standard == 3)  //ICC
                            combi_factor = icc_combi_factors_qpsls[ci][load];

                        gamma_l = 1.0;
                        if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                        if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                        if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                        if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                        if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                        if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                        strcpy(load_formula0, "");
                        if ((sh_load[j].style==4) || (sh_load[j].style==18)) //Fx, Fy
                        {
                            set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[2], sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            set_decimal_format(par[3], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            sprintf(load_formula0, " %s %s 0.0 %s %s 0.0 0.0 0.0 0.0", par[0], par[1], par[2], par[3]);

                            point_load_x[case_number - 1] += (sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f);
                            point_load_y[case_number - 1] -= (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f);
                            f_no++;
                        }
                        else if ((sh_load[j].style==5) || (sh_load[j].style==6))  //Mxy  //-Mxy
                        {
                            set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[2], sh_load[j].magnitude1x * unit_factors_sh->M_f, moment_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            sprintf(load_formula0, " %s %s 0.0 0.0 0.0 0.0 0.0 0.0 %s", par[0], par[1], par[2]);  //x,y,z,Fx,Fy,Fz,Mx,My,Mz
                            f_no++;
                        }
                        strcat(load_formula, load_formula0);
                    }
                }

                if (f_no>0)
                {
                    //sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                    sprintf(all_load_formula, " Point Load (%d, 9) = Real %s\n", f_no, load_formula);
                    strcat(all_formula, all_load_formula);
                }
                //////// concentrated load

                set_decimal_format(par[0], sh_property[this_property].h, dim_precision_sh);
                sprintf(par[1], "  Thickness = Real %s\n", par[0]);
                sprintf(par[2], "End\n\n");
                strcat(all_formula, par[1]);
                strcat(all_formula, par[2]);


                sif_body_force++;
            }

            for (i = 0; i < shield_no; i++) {
                //searching for properties
                for (j = 0; j < sh_property_no; j++) {
                    if (sh_property[j].n == shield_property[i].property_number) {
                        this_property = j;
                        break;
                    }
                }

                /////Distributed load on edge
                shield_case_variable_load_no[i]=0;
                if (shield_variable_load_no[i]>0)  //we know from SLS
                {
                    /*
                    for (j = 0; j < sh_load_no; j++)
                    {
                        if ((sh_load[j].body == 0) && (sh_load[j].body_no == i)) {
                            if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                            {
                                if (sh_load[j].factor_record >= 0) {
                                    gamma_l = sh_load_factors[sh_load[j].factor_record].gamma;
                                    flag = load_flag[sh_load_factors[sh_load[j].factor_record].load];
                                } else {
                                    gamma_l = load_factors[abs(sh_load[j].factor_record)].gamma;
                                    flag = load_flag[load_factors[abs(sh_load[j].factor_record)].load];
                                }
                                if (flag & ULSLC[ci].mask) {
                                    sh_load[j].take_it = 1;
                                    shield_case_variable_load_no[i]++;
                                } else sh_load[j].take_it = 0;
                            }
                        }
                    }
                    */

                    for (j = 0; j < sh_load_no; j++)
                    {
                        if ((sh_load[j].body == 0) && (sh_load[j].body_no == i))
                        {
                            if ((sh_load[j].style > 9) && (sh_load[j].style < 15))  //all linear
                            {
                                if (sh_load[j].take_it == 0)
                                {
                                    sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_shield[i][j] + 1);
                                    sprintf(par[1], "  Active Boundary Condition = Logical False\n");
                                    sprintf(par[2], "End\n\n");
                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[2]);
                                    continue;
                                }

                                if (sh_load[j].factor_record >= 0) {
                                    combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                                    load = sh_load_factors[sh_load[j].factor_record].load;
                                } else {
                                    combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                                    load = load_factors[abs(sh_load[j].factor_record)].load;
                                }

                                int combi_factor;
                                if (ret_standard == 1)  //EUROCODE
                                    combi_factor = eurocode_combi_factors_qpsls[ci][load];
                                else if (ret_standard == 2)  //ASCE
                                    combi_factor = asce_combi_factors_qpsls[ci][load];
                                if (ret_standard == 3)  //ICC
                                    combi_factor = icc_combi_factors_qpsls[ci][load];

                                gamma_l = 1.0;
                                if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                                if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                                if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                                if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                                if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                                if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                                ////Load on boundaries
                                sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_shield[i][j] + 1);
                                sprintf(par[1], "  Name = \"Boundary Loads %d\"\n", sif_boundary_condition_sls_shield[i][j] + 1);

                                sprintf(par[2], "  Target Boundaries(1) = %d\n", sh_load[j].k + 1);
                                sprintf(par[3], "  ! trapezoidal load\n");
                                sprintf(par[4], "  Name = \"VariableLoad%d\"\n", shield_variable_load_no[i] + 1);

                                BOOL shield_case_variable_load_x=FALSE;
                                BOOL shield_case_variable_load_y=FALSE;
                                //component Qx
                                if ((Check_if_Equal(sh_load[j].magnitude1x, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2x, 0.0) == FALSE)) {
                                    set_decimal_format(parf[0], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[1], sh_load[j].magnitude1x * unit_factors_sh->q_f, force_precision);
                                    set_decimal_format(parf[2], milimetryoby(sh_load[j].y2) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[3], sh_load[j].magnitude2x * unit_factors_sh->q_f, force_precision);
                                    sprintf(par[5], "  Force 1 = Variable Coordinate 2\n");
                                    sprintf(par[6], "  Real\n");
                                    sprintf(par[7], "  %s %s\n", parf[0], parf[1]);
                                    sprintf(par[8], "  %s %s\n", parf[2], parf[3]);
                                    sprintf(par[9], "  End\n");
                                    shield_case_variable_load_x=TRUE;
                                }
                                //comopnent Qy
                                if ((Check_if_Equal(sh_load[j].magnitude1y, 0.0) == FALSE) && (Check_if_Equal(sh_load[j].magnitude2y, 0.0) == FALSE)) {
                                    set_decimal_format(parf[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[1], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f, force_precision);
                                    set_decimal_format(parf[2], milimetryobx(sh_load[j].x2) * geo_units_factor, dim_precision_sh);
                                    set_decimal_format(parf[3], sh_load[j].magnitude2y * gamma_l * unit_factors_sh->q_f, force_precision);
                                    sprintf(par[10], "  Force 2 = Variable Coordinate 1\n");
                                    sprintf(par[11], "  Real\n");
                                    sprintf(par[12], "  %s %s\n", parf[0], parf[1]);
                                    sprintf(par[13], "  %s %s\n", parf[2], parf[3]);
                                    sprintf(par[14], "  End\n");
                                    shield_case_variable_load_y=TRUE;
                                }

                                double ddlx = fabs((double) (milimetryobx(sh_load[j].x2) - milimetryobx(sh_load[j].x1))) * geo_units_factor;
                                double ddly = fabs((double) (milimetryoby(sh_load[j].y2) - milimetryoby(sh_load[j].y1))) * geo_units_factor;
                                distributed_load_x[case_number - 1] += ((sh_load[j].magnitude1x + sh_load[j].magnitude2x) * gamma_l * unit_factors_sh->q_f) * ddly / 2.;
                                distributed_load_y[case_number - 1] += ((sh_load[j].magnitude1y + sh_load[j].magnitude2y) * gamma_l * unit_factors_sh->q_f) * ddlx / 2.;

                                sprintf(par[15], "End\n\n");

                                if ((shield_case_variable_load_x) || (shield_case_variable_load_y))
                                {

                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[3]);
                                    strcat(all_formula, par[4]);
                                    if (shield_case_variable_load_x)
                                    {
                                        strcat(all_formula, par[5]);
                                        strcat(all_formula, par[6]);
                                        strcat(all_formula, par[7]);
                                        strcat(all_formula, par[8]);
                                        strcat(all_formula, par[9]);
                                    }
                                    if (shield_case_variable_load_y)
                                    {
                                        strcat(all_formula, par[10]);
                                        strcat(all_formula, par[11]);
                                        strcat(all_formula, par[12]);
                                        strcat(all_formula, par[13]);
                                        strcat(all_formula, par[14]);
                                    }
                                    strcat(all_formula, par[15]);
                                    shield_case_variable_load_no[i]++;
                                }
                                else
                                {
                                    sprintf(par[0], "Boundary Condition %d\n", sif_boundary_condition_sls_shield[i][j] + 1);
                                    sprintf(par[1], "  Active Boundary Condition = Logical False\n");
                                    sprintf(par[2], "End\n\n");
                                    strcat(all_formula, par[0]);
                                    strcat(all_formula, par[1]);
                                    strcat(all_formula, par[2]);
                                }

                            }
                        }
                    }
                }
                /////////////////  distributed load on edge


                sprintf(par[0], "Body Force %d\n", sif_body_shield[i] + 1);
                sprintf(par[1], "  Name = \"BodyForce shield%d\"\n", sif_body_shield[i] + 1);
                strcat(all_formula, par[0]);
                strcat(all_formula, par[1]);

                strcpy(load_formula, "");

                //self_weight = 0.0;
                self_weight = gZ * gamma_g * sh_property[this_property].d; // * sh_property[this_property].h;  in Body Force 2 it will be multiplied by thickness by solver
                if (self_weight > 0.0) {
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][shield_property[i].k] += (self_weight * sh_property[this_property].h);
                }

                for (j = 0; j < sh_load_no; j++) //all load
                {
                    if ((sh_load[j].take_it == 1) && //active
                        (sh_load[j].type == 1) && //uniformly distributed on area
                        (sh_load[j].body == 0) &&  //shield
                        (sh_load[j].body_no == i)) //shield number
                    {
                        if (sh_load[j].factor_record >= 0)
                        {
                            combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                            load = sh_load_factors[sh_load[j].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                            load = load_factors[abs(sh_load[j].factor_record)].load;
                        }

                        int combi_factor;
                        if (ret_standard == 1)  //EUROCODE
                            combi_factor = eurocode_combi_factors_qpsls[ci][load];
                        else if (ret_standard == 2)  //ASCE
                            combi_factor = asce_combi_factors_qpsls[ci][load];
                        if (ret_standard == 3)  //ICC
                            combi_factor = icc_combi_factors_qpsls[ci][load];

                        gamma_l = 1.0;
                        if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                        if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                        if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                        if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                        if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                        if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                        set_decimal_format(par[1], -sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f / sh_property[this_property].h,
                                           load_precision);  //is assumed that magnitude1=magnitude2
                        //fprintf(f, "  Normal Pressure = %s\n", par[1]);
                        if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                        strcat(load_formula, "(");
                        strcat(load_formula, par[1]);
                        strcat(load_formula, ")");

                        surface_load[case_number-1][shield_property[i].k] += (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->q_f);
                    }
                }
                if (strlen(load_formula) > 0)
                {
                    sprintf(all_load_formula, "  Body Force 2 = Real MATC \"%s\"\n", load_formula);
                    strcat(all_formula, all_load_formula);
                }

                ///////////////// Concentrated Load
                strcpy(load_formula, "");
                int f_no=0;

                for (j = 0; j < sh_load_no; j++) //all load
                {
                    if ((sh_load[j].take_it == 1) && //active
                        (sh_load[j].type==0) && //concentrated
                        (sh_load[j].body == 0) && //shield
                        (sh_load[j].body_no == i)) //shield number
                    {
                        if (sh_load[j].factor_record >= 0)
                        {
                            combi_load_factor = &sh_load_factors[sh_load[j].factor_record];
                            load = sh_load_factors[sh_load[j].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(sh_load[j].factor_record)];
                            load = load_factors[abs(sh_load[j].factor_record)].load;
                        }

                        int combi_factor;
                        if (ret_standard == 1)  //EUROCODE
                            combi_factor = eurocode_combi_factors_qpsls[ci][load];
                        else if (ret_standard == 2)  //ASCE
                            combi_factor = asce_combi_factors_qpsls[ci][load];
                        if (ret_standard == 3)  //ICC
                            combi_factor = icc_combi_factors_qpsls[ci][load];

                        gamma_l = 1.0;
                        if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                        if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                        if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                        if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                        if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                        if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                        strcpy(load_formula0, "");
                        if ((sh_load[j].style==4) || (sh_load[j].style==18)) //Fx, Fy
                        {
                            set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[2], sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            set_decimal_format(par[3], sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f, force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            sprintf(load_formula0, " %s %s 0.0 %s %s 0.0 0.0 0.0 0.0", par[0], par[1], par[2], par[3]);

                            point_load_x[case_number - 1] += (sh_load[j].magnitude1x * gamma_l * unit_factors_sh->F_f);
                            point_load_y[case_number - 1] -= (sh_load[j].magnitude1y * gamma_l * unit_factors_sh->F_f);
                            f_no++;
                        }
                        else if ((sh_load[j].style==5) || (sh_load[j].style==6))  //Mxy  //-Mxy
                        {
                            set_decimal_format(par[0], milimetryobx(sh_load[j].x1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[1], milimetryoby(sh_load[j].y1) * geo_units_factor, dim_precision_sh);
                            set_decimal_format(par[2], sh_load[j].magnitude1x * unit_factors_sh->M_f, moment_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                            sprintf(load_formula0, " %s %s 0.0 0.0 0.0 0.0 0.0 0.0 %s", par[0], par[1], par[2]);  //x,y,z,Fx,Fy,Fz,Mx,My,Mz
                            f_no++;
                        }
                        strcat(load_formula, load_formula0);
                    }
                }

                if (f_no>0)
                {
                    //sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                    sprintf(all_load_formula, "  Point Load (%d, 9) = Real %s\n", f_no, load_formula);
                    strcat(all_formula, all_load_formula);
                }
                //////// concentrated load

                set_decimal_format(par[0], sh_property[this_property].h, dim_precision_sh);
                sprintf(par[1], "  Thickness = Real %s\n", par[0]);
                sprintf(par[2], "End\n\n");
                strcat(all_formula, par[1]);
                strcat(all_formula, par[2]);


                sif_body_force++;
            }

            go_ahead = TRUE;
            if (all_formula_bak != NULL) {
                if (strcmp(all_formula_bak, all_formula) == 0)
                    go_ahead = FALSE;
            }

            if ((strlen(all_formula)>0) &&(go_ahead)) //|| (total_zone_variable_load_no>0) || (total_shield_variable_load_no>0))
            {
                fprintf(f, "Simulation::Output File = \"shield_qpslslc%d.result\"\n", ci+1);
                combi_qpslslc_ino++;
                if (go_ahead)
                {
                    fprintf(f, "%s",all_formula);
                }
                fprintf(f, "RUN\n\n");
                QPSLSLC[ci].flag = 2;
            }

            all_formula_bak = all_formula;
            current = !current;
            all_formula = all_formulas[current];
            ////QPSLSLC ENDS
        }
    }

    fclose(f);

    //dealocation boundary conditions
    for (i=0; i<zone_no; i++)
    {
        if (sif_boundary_condition_sls_zone[i]) { free(sif_boundary_condition_sls_zone[i]); sif_boundary_condition_sls_zone[i]=NULL;}
    }
    for (i=0; i<shield_no; i++)
    {
        if (sif_boundary_condition_sls_shield[i]) { free(sif_boundary_condition_sls_shield[i]); sif_boundary_condition_sls_shield[i]=NULL;};
    }
    if (zone_variable_load_no) {free(zone_variable_load_no); zone_variable_load_no=NULL;}
    if (shield_variable_load_no) {free(shield_variable_load_no); shield_variable_load_no=NULL;}

    if (zone_case_variable_load_no) {free(zone_case_variable_load_no); zone_case_variable_load_no=NULL;}
    if (shield_case_variable_load_no) {free(shield_case_variable_load_no); shield_case_variable_load_no=NULL;}

    ClearErr();
    ClearInfo();

    komunikat_str_short(_BUILDING_MESH_, FALSE, TRUE);

    my_sleep(100);
    sprintf(params, "%sshield.geo", _STATIC_);
    if (my_file_exists(params)) {
        //generating msh
        //  gmsh Static/shield.geo -o Static/shield.msh -2
        sprintf(params, "%sshield.geo -o %sshield.msh -2 > %sgmsh.log 2>&1", _STATIC_, _STATIC_, _STATIC_);
        sprintf(program, "%sgmsh", _ELMER_);
        //execute gmsh
#ifdef LINUX
        //runcode = SystemSilent(program, params);
        //runcode_short = runcode >> 8;

        //ALTERNATIVE with timeout
        strcat(program," ");
        strcat(program,params);
        runcode_short = run_with_timeout(program, TIMEOUT_SECONDS);
        runcode = runcode_short;
#else
        //runcode = RunSilent(program, params);
        //runcode_short = runcode >> 8;

        //ALTERNATIVE with timeout
        strcat(program," ");
        strcat(program,params);
        runcode = run_with_timeout(program, TIMEOUT_SECONDS);
        runcode_short = runcode >> 8;
#endif
        printf("\ngmsh runcode:%lu runcode_short:%d\n", runcode, runcode_short);
        sprintf(params, "%sshield.msh", _STATIC_);
        if ((runcode_short != 0) || (!my_file_exists(params))) {
            ret = ask_question(2, (char *) "Log", (char *) "OK", (char *) "", (char *) "gmsh", 12, (char *) _gmsh_error_,
                               11, 1, 0);
            if (ret==0)
            {
                //show log gmsh.log
                sprintf(params, "%sgmsh.log", _STATIC_);
                ret = EditFile(params, 0, mynCmdShow);
            }
            goto sh_error;
        }

        if (my_file_exists(params))
        {
            komunikat_str_short(_BUILDING_GRID_, FALSE, TRUE);
            //generating mesh
            // ElmerGrid 14 2 Static/shield.msh -out Static/shield
            sprintf(params, "14 2 %sshield.msh -out %sshield -unite -merge 1.0e-10 > %sgrid.log", _STATIC_, _STATIC_, _STATIC_);  //-merge 1.e-8
            sprintf(program, "%s%sElmerGrid", _ELMER_, _BIN_);
            //execute ElmerGrid
#ifdef LINUX
            runcode = SystemSilent(program, params);
            runcode_short = runcode >> 8;
#else
            runcode = RunSilent(program, params);
            runcode_short = runcode >> 8;
#endif
            printf("\nElmerGrid runcode:%lu runcode_short:%d\n", runcode, runcode_short);
            sprintf(params, "%s%smesh.header", _STATIC_, _shield_);
            if ((runcode_short != 0) || (!my_file_exists(params))) {
                ret = ask_question(2, (char *) "Log", (char *) "OK", (char *) "", (char *) "ElmerGrid", 12,
                                   (char *) _ElmerGrid_error_, 11, 1, 0);
                if (ret==0)
                {
                    //show log grid.log
                    sprintf(params, "%sgrid.log", _STATIC_);
                    ret = EditFile(params, 0, mynCmdShow);
                }
                goto sh_error;
            }

            if (my_file_exists(params))
            {
                komunikat_str_short(_SOLVING_,FALSE, TRUE);

                sprintf(params, "%s%sshield.sif  > %ssolver.log", _STATIC_, _shield_, _STATIC_);
                //execute ElmerSolver, it will save results in shield folder
#ifdef LINUX
                sprintf(program, "%s%sElmerSolver", _ELMER_, _BIN_);
                runcode = SystemSilent(program, params);
                runcode_short = runcode >> 8;

                //ALTERNATIVE with timeout
                //strcat(program," ");
                //strcat(program,params);
                //runcode_short = run_with_timeout(program, TIMEOUT_SECONDS);
                //runcode = runcode_short;
#else
                sprintf(program,"%s%sElmerSolver.exe", _ELMER_, _BIN_);
                runcode = RunSilent(program, params);
                runcode_short = runcode >> 8;
#endif
                printf("\nElmerSolver runcode:%lu runcode_short:%d\n", runcode, runcode_short);
                sprintf(params, "%sshield_sls.result", _shield_);
                if ((runcode_short != 0) || (!my_file_exists(params))) {
                    ret = ask_question(2, (char *) "Log", (char *) "OK", (char *) "", (char *) "ElmerSolver", 12,
                                       (char *) _ElmerSolver_error_, 11, 1, 0);
                    if (ret==0)
                    {
                        //show log solver.log
                        sprintf(params, "%ssolver.log", _STATIC_);
                        ret = EditFile(params, 0, mynCmdShow);
                    }
                    goto sh_error;
                }
            }
        }
    }

    komunikat_str_short(_GEOMETRY_, FALSE, TRUE);
    //getting geometry
    //mesh.header
    //5079   9894   592   //number of nodes in mesh.nodes   number of elements in mesh.elements  number of boundaries in mesh.baunday
    //3  //Maximum Element Nodes
    //101    26  //number of baundary nodes - can be ignored
    //202    566  //number of baundary elements - to read
    //303    9894  //number of all elements - alread read in first row
    //nodes -
    sprintf(params, "%s%smesh.header", _STATIC_, _shield_);
    f = fopen(params, "rt");

    if (f == NULL) {
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_OPEN_RESULTS_FILE_, 12,
                           (char *) "mesh.header", 11, 1,
                           62);
        no_error = FALSE;
        goto sh_error;
    }

    i_row = 0;
    data_row = data_row_[i_row];

    line = fgets(data_row, MaxTextLen, f);
    ret = sscanf(data_row, "%d %d %d", &mesh_nodes_no, &mesh_elements_no, &mesh_boundaries_no);
    line = fgets(data_row, MaxTextLen, f);
    ret = sscanf(data_row, "%d", &max_element_nodes);
    line = fgets(data_row, MaxTextLen, f);
    ret = sscanf(data_row, "%d %d", &flag101_no, &mesh_101_boundaries_no);
    line = fgets(data_row, MaxTextLen, f);
    ret = sscanf(data_row, "%d %d", &flag202_no, &mesh_202_boundaries_no);
    line = fgets(data_row, MaxTextLen, f);
    ret = sscanf(data_row, "%d %d", &flag303_no, &mesh_303_boundaries_no);
    line = fgets(data_row, MaxTextLen, f);
    while (fgets(data_row, MaxTextLen, f)) //another rows
    { ;
    }
    fclose(f);

    mesh_node_no = 0;
    mesh_element_no = 0;
    mesh_boundary_no = 0;

    mesh_node = (MESH_NODE *) malloc(mesh_nodes_no * sizeof(MESH_NODE) + M100);
    mesh_element = (MESH_ELEMENT *) malloc(mesh_elements_no * sizeof(MESH_ELEMENT) + M100);
    mesh_boundary = (MESH_BOUNDARY *) malloc(mesh_boundaries_no * sizeof(MESH_BOUNDARY) + (M100*10));

    MESH_BOUNDARY mesh_boundary_ini={0,0,0,0,0,0,0,0,0,0.0,0.0,0.0,0.0,0.0,0.0, 0.0, 0.0,
        {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}};


    for (i=0; i<mesh_boundaries_no; i++)
        memmove(&mesh_boundary[i], &mesh_boundary_ini, sizeof(MESH_BOUNDARY));

    sprintf(params, "%s%smesh.nodes", _STATIC_, _shield_);
    f = fopen(params, "rt");

    if (f == NULL) {
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_OPEN_RESULTS_FILE_, 12,
                           (char *) "mesh.nodes", 11, 1,
                           62);
        no_error = FALSE;
        goto sh_error;
    }

    while (fgets(data_row, MaxTextLen, f)) //another rows
    {
        ret = sscanf(data_row, "%d %d %lf %lf %lf", &mesh_node[mesh_node_no].number, &mesh_node[mesh_node_no].flag,
                     &mesh_node[mesh_node_no].x, &mesh_node[mesh_node_no].y, &mesh_node_z);
        if (ret != 5)
        {
            ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_READ_RESULTS_FILE_, 12,
                               (char *) "hesh.nodes", 11,
                               1,
                               62);
            no_error = FALSE;
            fclose(f);
            goto sh_error1;
        }
        mesh_node[mesh_node_no].restraint=0;
        mesh_node_no++;
    }
    fclose(f);

    sprintf(params, "%s%smesh.elements", _STATIC_, _shield_);
    f = fopen(params, "rt");

    if (f == NULL) {
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_OPEN_RESULTS_FILE_, 12,
                           (char *) "mesh.elements", 11, 1,
                           62);
        no_error = FALSE;
        goto sh_error;
    }

    while (fgets(data_row, MaxTextLen, f)) //another rows
    {
        ret = sscanf(data_row, "%d %d %d %d %d %d %d", &mesh_element[mesh_element_no].number,
                     &mesh_element[mesh_element_no].body_number, &mesh_element[mesh_element_no].type,
                     &mesh_element[mesh_element_no].node1, &mesh_element[mesh_element_no].node2,
                     &mesh_element[mesh_element_no].node3, &mesh_element[mesh_element_no].node4);
        if (mesh_element[mesh_element_no].type == 303) ret_exp = 6; else ret_exp = 7;
        mesh_element[mesh_element_no].h=0.0;
        if (ret != ret_exp) {
            ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_READ_RESULTS_FILE_, 12,
                               (char *) "mesh.elements", 11,
                               1,
                               62);
            no_error = FALSE;
            fclose(f);
            goto sh_error1;
        }

        //calculate area of element
        area=calculateTriangleArea(mesh_node[mesh_element[mesh_element_no].node1-1].x, mesh_node[mesh_element[mesh_element_no].node1-1].y,
                                   mesh_node[mesh_element[mesh_element_no].node2-1].x, mesh_node[mesh_element[mesh_element_no].node2-1].y,
                                   mesh_node[mesh_element[mesh_element_no].node3-1].x, mesh_node[mesh_element[mesh_element_no].node3-1].y);
        body_area[mesh_element[mesh_element_no].body_number-1]+=area;

        mesh_element_no++;
    }
    fclose(f);

    sprintf(params, "%s%smesh.boundary", _STATIC_, _shield_);
    f = fopen(params, "rt");

    if (f == NULL) {
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_OPEN_RESULTS_FILE_, 12,
                           (char *) "mesh.boundary", 11, 1,
                           62);
        no_error = FALSE;
        goto sh_error;
    }

    while (fgets(data_row, MaxTextLen, f)) //another rows
    {
        ret = sscanf(data_row, "%d %d %d %d %d %d %d", &mesh_boundary[mesh_boundary_no].no,
                     &mesh_boundary[mesh_boundary_no].edge, &mesh_boundary[mesh_boundary_no].element1,
                     &mesh_boundary[mesh_boundary_no].element2, &mesh_boundary[mesh_boundary_no].flag,
                     &mesh_boundary[mesh_boundary_no].node1, &mesh_boundary[mesh_boundary_no].node2);
        if (mesh_boundary[mesh_boundary_no].flag==202) ret_exp=7; else ret_exp=6;
        if (ret!=ret_exp)
        //if (ret < 6)
        {
            ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_READ_RESULTS_FILE_, 12,
                               (char *) "mesh.boundary", 11,
                               1,
                               62);
            no_error = FALSE;
            fclose(f);
            goto sh_error1;
        }

        for (i = 0; i < sh_edge_no; i++)
        {
            if ((sh_edge[i].k+1)==mesh_boundary[mesh_boundary_no].edge)
            {
                mesh_boundary[mesh_boundary_no].restraint = sh_edge[i].restraint;
                mesh_boundary[mesh_boundary_no].inverted = sh_edge[i].inverted;
                break;
            }
        }

        //mesh nodes inherit restraint from mesh_boundary
        //check if mesh_node belongs to mesh_boundary[] then inherit from mesh_boundary[mesh_boundary_no].restraint
        for (i = 0; i < mesh_nodes_no; i++)
        {
            if (mesh_boundary[mesh_boundary_no].node1 == mesh_node[i].number /*(i+1)*/)
                mesh_node[i].restraint = max(mesh_node[i].restraint, mesh_boundary[mesh_boundary_no].restraint);
            else if (mesh_boundary[mesh_boundary_no].node2 == mesh_node[i].number /*(i+1)*/)
                mesh_node[i].restraint = max(mesh_node[i].restraint, mesh_boundary[mesh_boundary_no].restraint);
        }

        if (ret_exp == 7)
        {
            //kos, koc
            LINIA BL;
            PLINIA PLB;
            if (mesh_boundary[mesh_boundary_no].node1 > 0)
            {
                BL.x1 = (float)mesh_node[mesh_boundary[mesh_boundary_no].node1 - 1].x;
                BL.y1 = (float)mesh_node[mesh_boundary[mesh_boundary_no].node1 - 1].y;
            }
            else
            {
                BL.x1 = 0.0f;
                BL.y1 = 0.0f;
            }
            if (mesh_boundary[mesh_boundary_no].node2 > 0)
            {
                BL.x2 = (float)mesh_node[mesh_boundary[mesh_boundary_no].node2 - 1].x;
                BL.y2 = (float)mesh_node[mesh_boundary[mesh_boundary_no].node2 - 1].y;
            }
            else
            {
                BL.x2 = 1.0f;  //to be different from BL.x1
                BL.y2 = 0.0f;
            }
            parametry_lini(&BL, &PLB);
      
            mesh_boundary[mesh_boundary_no].kos = sin(Angle_Simple(PLB.kat* Pi / 180));
            mesh_boundary[mesh_boundary_no].koc = cos(Angle_Simple(PLB.kat* Pi / 180));
        }
        else
        {
            mesh_boundary[mesh_boundary_no].kos = 0.0;
            mesh_boundary[mesh_boundary_no].koc = 0.0;
        }

        if (mesh_boundary[mesh_boundary_no].node1 > 0)
        {
            mesh_boundary[mesh_boundary_no].x1 = mesh_node[mesh_boundary[mesh_boundary_no].node1 - 1].x;
            mesh_boundary[mesh_boundary_no].y1 = mesh_node[mesh_boundary[mesh_boundary_no].node1 - 1].y;
        }
        if (mesh_boundary[mesh_boundary_no].node2 > 0)
        {
            mesh_boundary[mesh_boundary_no].x2 = mesh_node[mesh_boundary[mesh_boundary_no].node2 - 1].x;
            mesh_boundary[mesh_boundary_no].y2 = mesh_node[mesh_boundary[mesh_boundary_no].node2 - 1].y;
        }

        mesh_boundary[mesh_boundary_no].l1=0.0;
        mesh_boundary[mesh_boundary_no].l2=0.0;

        mesh_boundary_no++;
    }
    fclose(f);


    //calculating the length of boundary for reactions purpose
    double r_edge_x1_, r_edge_y1_, r_edge_x2_, r_edge_y2_;
    double edge_dl_;
    int edge_=0;
    int i_edge_=0;
    for (i = 0; i < mesh_boundary_no; i++)
    {
        if ((mesh_boundary[i].restraint>5) || (mesh_boundary[i].restraint==4))
        {
            if (mesh_boundary[i].edge != edge_) //new edge starts
            {
                i_edge_ = 0;
                edge_ = mesh_boundary[i].edge;
            }
            else
            {
                if (i_edge_ > 0)  //is not first edge
                    mesh_boundary[i].l1 += edge_dl_;
            }

            r_edge_x1_ = mesh_boundary[i].x1;
            r_edge_y1_ = mesh_boundary[i].y1;
            r_edge_x2_ = mesh_boundary[i].x2;
            r_edge_y2_ = mesh_boundary[i].y2;

            edge_dl_ = sqrt((r_edge_x2_ - r_edge_x1_) * (r_edge_x2_ - r_edge_x1_) + (r_edge_y2_ - r_edge_y1_) * (r_edge_y2_ - r_edge_y1_)) / 2.;

            if (i_edge_ > 0)  //is not first edge
            {
                mesh_boundary[i - 1].l2 += edge_dl_;
            }
            mesh_boundary[i].l1 += edge_dl_;
            mesh_boundary[i].l2 += edge_dl_;

            i_edge_++;
        }
    }


    ////creating geometry block
    Ldsp.blok = 1;
    Ldsp.kolor = static_colors.node_element_color;
    Ldsp.typ = 0;  //thinest
    /////////////////////
    //Layers
    sprintf(desired_layer, "%s_%s", ptr_id, "geometry");

    memcpy(desired_layer_upper, desired_layer, strlen(desired_layer)+1);
    utf8Upper(desired_layer_upper);

    for (ii = 0; ii < No_Layers; ii++) {
        memcpy(layer_name_upper, Layers[ii].name, strlen(Layers[ii].name)+1);
        utf8Upper(layer_name_upper);

        if (strcmp(layer_name_upper, desired_layer_upper) == 0)
            break;
    }
    if (ii < No_Layers) {
        desired_layer_no = ii;
        delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
    } else {
        if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

            ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_, 12,
                               (char *) "", 11, 1, 62);
            no_error = FALSE;
            goto sh_error1;
        }
        if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

            ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_, 12,
                               (char *) "", 11, 1, 62);
            no_error = FALSE;
            goto sh_error1;
        }
        No_Layers++;
        desired_layer_no = No_Layers - 1;
        Layers[No_Layers - 1].on = 1;
        Layers[No_Layers - 1].edit = 1;
        Layers[No_Layers - 1].point = 1;
        strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
        Layers[No_Layers - 1].color = Ldsp.kolor;
    }

    Ldsp.warstwa = desired_layer_no;

    if (UNITS==SI)
    {
        jednostkiObX=jednostkiObXm;
        jednostkiObY=jednostkiObYm;
    }
    else
    {
        jednostkiObX=jednostkiObXi;
        jednostkiObY=jednostkiObYi;
    }


    //from meters to drawing units
    blok_origin.x = jednostkiObX(mesh_node[0].x);
    blok_origin.y = jednostkiObY(mesh_node[0].y);

    sprintf(block_name, "%s$mesh", ptr_id_short);

    ptr_block = add_block(blok_origin.x, blok_origin.y, 'E', block_name, FALSE);
    if (ptr_block == NULL) {
        fclose(f);
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "",
                           (char *) _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_, 12, (char *) "", 11, 1, 62);
        no_error = FALSE;
        goto sh_error1;
    }
    GEO_LINE_MAX = 1000;
    geo_line_no = 0;
    geo_line = (GEO_LINE *) malloc(GEO_LINE_MAX * sizeof(GEO_LINE) + M100);
    memmove(&geo_line[geo_line_no].L, &Ldsp, sizeof(LINIA));
    geo_line[geo_line_no].n1 = -1;
    geo_line[geo_line_no].n2 = -1;

    for (i = 0; i < mesh_element_no; i++) {
        //edge 1
        geo_line[geo_line_no].L.x1 = jednostkiObX(mesh_node[mesh_element[i].node1 - 1].x);
        geo_line[geo_line_no].L.y1 = jednostkiObY(mesh_node[mesh_element[i].node1 - 1].y);
        geo_line[geo_line_no].L.x2 = jednostkiObX(mesh_node[mesh_element[i].node2 - 1].x);
        geo_line[geo_line_no].L.y2 = jednostkiObY(mesh_node[mesh_element[i].node2 - 1].y);
        geo_line[geo_line_no].n1 = mesh_element[i].node1;
        geo_line[geo_line_no].n2 = mesh_element[i].node2;

        for (j = 0; j < geo_line_no; j++) {
            if (((geo_line[geo_line_no].n1 == geo_line[j].n1) &&
                 (geo_line[geo_line_no].n2 == geo_line[j].n2)) ||
                ((geo_line[geo_line_no].n1 == geo_line[j].n2) &&
                 (geo_line[geo_line_no].n2 == geo_line[j].n1)))
                break;
        }
        if (j == geo_line_no)
            add_geo_line();
        //edge 2
        geo_line[geo_line_no].L.x1 = jednostkiObX(mesh_node[mesh_element[i].node2 - 1].x);
        geo_line[geo_line_no].L.y1 = jednostkiObY(mesh_node[mesh_element[i].node2 - 1].y);
        geo_line[geo_line_no].L.x2 = jednostkiObX(mesh_node[mesh_element[i].node3 - 1].x);
        geo_line[geo_line_no].L.y2 = jednostkiObY(mesh_node[mesh_element[i].node3 - 1].y);
        geo_line[geo_line_no].n1 = mesh_element[i].node2;
        geo_line[geo_line_no].n2 = mesh_element[i].node3;

        for (j = 0; j < geo_line_no; j++) {
            if (((geo_line[geo_line_no].n1 == geo_line[j].n1) &&
                 (geo_line[geo_line_no].n2 == geo_line[j].n2)) ||
                ((geo_line[geo_line_no].n1 == geo_line[j].n2) &&
                 (geo_line[geo_line_no].n2 == geo_line[j].n1)))
                break;
        }
        if (j == geo_line_no)
            add_geo_line();
        //edge 3
        geo_line[geo_line_no].L.x1 = jednostkiObX(mesh_node[mesh_element[i].node3 - 1].x);
        geo_line[geo_line_no].L.y1 = jednostkiObY(mesh_node[mesh_element[i].node3 - 1].y);
        geo_line[geo_line_no].L.x2 = jednostkiObX(mesh_node[mesh_element[i].node1 - 1].x);
        geo_line[geo_line_no].L.y2 = jednostkiObY(mesh_node[mesh_element[i].node1 - 1].y);
        geo_line[geo_line_no].n1 = mesh_element[i].node3;
        geo_line[geo_line_no].n2 = mesh_element[i].node1;

        for (j = 0; j < geo_line_no; j++) {
            if (((geo_line[geo_line_no].n1 == geo_line[j].n1) &&
                 (geo_line[geo_line_no].n2 == geo_line[j].n2)) ||
                ((geo_line[geo_line_no].n1 == geo_line[j].n2) &&
                 (geo_line[geo_line_no].n2 == geo_line[j].n1)))
                break;
        }
        if (j == geo_line_no)
            add_geo_line();
    }

    for (i = 0; i < geo_line_no; i++) {
        dodaj_obiekt((BLOK *) dane, &geo_line[i].L);
    }

    //geometry nodes and element
    sprintf(block_name, "%s$Geo_Nodes&Elms", ptr_id_short);

    ptr_block = add_block(blok_origin.x, blok_origin.y, 'E', block_name, FALSE);
    if (ptr_block == NULL) {
        fclose(f);
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "",
                           (char *) _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_, 12, (char *) "", 11, 1, 62);
        no_error = FALSE;
        goto sh_error1;
    }

    T.justowanie = j_srodkowo;
    T.bold = 0;
    T.warstwa = Ldsp.warstwa;
    T.kolor = Ldsp.kolor;
    T.czcionka = zmwym.czcionka;
    T.wysokosc = (float) (zmwym.wysokosc * 0.5);
    T.kat = 0.0f;
    T.blok = 1;
    T.width_factor = (float) zmwym.width_factor;
    T.italics = 0;
    T.underline = 0;
    T.width = 0;
    T.kolor = 0;

    for (i = 0; i < sh_node_no; i++) {
        T.x = (float) sh_node[i].x;
        T.y = (float) sh_node[i].y;
        sprintf(T.text, "%d", i + 1);

        T.dl = strlen(T.text);
        T.n = T18 + T.dl;
        normalize_txt(&T);
        adr = dodaj_obiekt((BLOK *) dane, &T);
    }

    T.italics = 1;
    T.underline = 1;
    /*
    for (i=0; i<sh_edge_no; i++)
    {
        T.x=(float)((sh_node[sh_edge[i].node1].x+sh_node[sh_edge[i].node2].x)/2.);
        T.y=(float)((sh_node[sh_edge[i].node1].y+sh_node[sh_edge[i].node2].y)/2.);

        sprintf(T.text, "%d", i);

        T.dl = strlen(T.text);
        T.n = T18 + T.dl;
        normalize_txt(&T);
        adr = dodaj_obiekt((BLOK *) dane, &T);
    }
    */

    for (i = 0; i < sh_edge_no; i++) {
        if (sh_edge[i].type==0) //line
        {
            Lt.x1 = (float) sh_node[sh_edge[i].node1].x;
            Lt.y1 = (float) sh_node[sh_edge[i].node1].y;
            Lt.x2 = (float) sh_node[sh_edge[i].node2].x;
            Lt.y2 = (float) sh_node[sh_edge[i].node2].y;

            if ((Check_if_Equal(Lt.x1, Lt.x2)) && (Check_if_Equal(Lt.y1, Lt.y2))) {
                PL.kat = 0;
                psize = Get_Point_Size() / 2.0;
                T.x = (float) (Lt.x1 + psize);
                T.y = (float) (Lt.y1 + psize);
            } else {
                parametry_lini(&Lt, &PL);
                T.kat = Pi_ * PL.kat / 180;
                //T.x = Lt.x1 + ((Lt.x2 - Lt.x1) / 3);
                //T.y = Lt.y1 + ((Lt.y2 - Lt.y1) / 3);
                T.x = (float) ((Lt.x1 + Lt.x2) / 2.);
                T.y = (float) ((Lt.y1 + Lt.y2) / 2.);
            }
        }
        else //arc
        {
            Lt.x1 = (float) sh_node[sh_edge[i].node1].x;
            Lt.y1 = (float) sh_node[sh_edge[i].node1].y;
            Lt.x2 = (float) sh_node[sh_edge[i].node3].x;
            Lt.y2 = (float) sh_node[sh_edge[i].node3].y;
            parametry_lini(&Lt, &PL);
            T.kat = Pi_ * PL.kat / 180;
            T.x = sh_edge[i].xm;
            T.y = sh_edge[i].ym;
        }

        sprintf(T.text, "%d", i + 1);

        T.dl = strlen(T.text);
        T.n = T18 + T.dl;

        normalize_txt(&T);
        adr = dodaj_obiekt((BLOK *) dane, &T);
    }

    //mesh nodes
    sprintf(block_name, "%s$Mesh_Nodes", ptr_id_short);

    ptr_block = add_block(blok_origin.x, blok_origin.y, 'E', block_name, FALSE);
    if (ptr_block == NULL) {
        fclose(f);
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "",
                           (char *) _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_, 12, (char *) "", 11, 1, 62);
        no_error = FALSE;
        goto sh_error1;
    }

    //mesh nodes
    T.italics = 0;
    T.underline = 0;
    T.wysokosc = (float) (zmwym.wysokosc * 0.05);
    T.kat = 0.0f;
    T.ukryty = 1;
    for (i = 0; i < mesh_nodes_no; i++) {
        T.x = jednostkiObX(mesh_node[i].x);
        T.y = jednostkiObY(mesh_node[i].y);
        sprintf(T.text, "%d", i + 1);

        T.dl = strlen(T.text);
        T.n = T18 + T.dl;
        normalize_txt(&T);
        adr = dodaj_obiekt((BLOK *) dane, &T);
    }


    //mesh elements
    T.italics = 1;
    T.underline = 1;
    T.wysokosc = (float) (zmwym.wysokosc * 0.05);
    T.kat = 0.0f;
    T.ukryty = 1;
    for (i = 0; i < mesh_elements_no; i++) {
        T.x = jednostkiObX((mesh_node[mesh_element[i].node1 - 1].x + mesh_node[mesh_element[i].node2 - 1].x +
                             mesh_node[mesh_element[i].node3 - 1].x) / 3.);
        T.y = jednostkiObY((mesh_node[mesh_element[i].node1 - 1].y + mesh_node[mesh_element[i].node2 - 1].y +
                             mesh_node[mesh_element[i].node3 - 1].y) / 3.);
        sprintf(T.text, "%d", i + 1);

        T.dl = strlen(T.text);
        T.n = T18 + T.dl;
        normalize_txt(&T);
        adr = dodaj_obiekt((BLOK *) dane, &T);
    }

    free(geo_line);
    geo_line = NULL;

    //consolidation of results
    //shield_ulslc.result
    //shield_slslc.result
    //shield_qpslslc.result
    //will be created

    //deflection_ini = 0;
    //stress_ini = 0;
    //epsilon_ini = 0;


    //let's create body_height table
    int max_body_no=shield_property[shield_no-1].k+1;

    body_property = (int*)malloc(max_body_no*sizeof(int) + M100);  //10-12-2025

    for (i=0; i<max_body_no; i++) body_property[i]=-1;

    //let's prepare table body_h
    /*   //ignoring walls
    for (i=0; i<wall_no; i++)
    {
        //searching for properties
        for (j = 0; j < sh_property_no; j++) {
            if (sh_property[j].n == wall_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        if (j<sh_property_no)  //found it
        {
            body_property[wall_property[i].k]=this_property;
        }
    }
     */
    for (i=0; i<zone_no; i++)
    {
        //searching for properties
        for (j = 0; j < sh_property_no; j++) {
            if (sh_property[j].n == zone_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        if (j<sh_property_no)  //found it
        {
            body_property[zone_property[i].k]=this_property;
        }
    }
    for (i=0; i<shield_no; i++)
    {
        //searching for properties
        for (j = 0; j < sh_property_no; j++) {
            if (sh_property[j].n == shield_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        if (j<sh_property_no)  //found it
        {
            body_property[shield_property[i].k]=this_property;
        }
    }

    /////////////////
    //setting cross section height for each element, for shear and reactions purpose
    for (i=0; i<mesh_element_no; i++)
    {
        body_prop_no = body_property[mesh_element[i].body_number - 1];
        if (body_prop_no > -1)
            body_prop = &sh_property[body_prop_no];
        else body_prop = NULL;

        if (body_prop != NULL) //WARNING, RC_flag should be inherited from shield
        {
            mesh_element[i].h = body_prop->h;
            if (body_prop->RC_flag)
            {
                mesh_element[i].RC_flag = 1;
            }
        }
    }
    /////////////////

    char params0[32];
    int ULS_SLS_LC_NO;

    for (i=0; i<5; i++) ULSLC_flag[i]=1;

    //allocation space for block names
    block_names_no = 0;
    //
    for (int sti = 0; sti < 5; sti++)
    {
        double *supp1_sigx_max=(double*)malloc(mesh_boundaries_no*sizeof(double) + M100);
        double *supp1_sigy_max=(double*)malloc(mesh_boundaries_no*sizeof(double) + M100);
        double *supp1_sigxy_max=(double*)malloc(mesh_boundaries_no*sizeof(double) + M100);

        double *supp2_sigx_max=(double*)malloc(mesh_boundaries_no*sizeof(double) + M100);
        double *supp2_sigy_max=(double*)malloc(mesh_boundaries_no*sizeof(double) + M100);
        double *supp2_sigxy_max=(double*)malloc(mesh_boundaries_no*sizeof(double) + M100);

        char building_blocks[MaxTextLen];

        if (sti<2) goto singles;

        switch (sti)
        {
        case 2:
            sprintf(params0, "shield_ulslc");
            ULS_SLS_LC_NO=ULSLC_NO;
            strncpy(SLS_ULS, "ULSLC", 5);
            ULS_SLS_LC = ULSLC;
            break;
        case 3:
            sprintf(params0, "shield_slslc");
            ULS_SLS_LC_NO=SLSLC_NO;
            strncpy(SLS_ULS, "slsLC", 5);
            ULS_SLS_LC = SLSLC;
            break;
        case 4:
            sprintf(params0, "shield_qpslslc");
            ULS_SLS_LC_NO=QPSLSLC_NO;
            strncpy(SLS_ULS, "qpslsLC", 7);
            ULS_SLS_LC = QPSLSLC;
            break;
        }

        file_no=0;
        sti_no=0;
        deflection_ini = 0;
        stress_ini = 0;
        epsilon_ini = 0;

        sprintf(building_blocks,"%s %s", _BUILDING_BLOCKS_, SLS_ULS);
        komunikat_str_short(building_blocks, FALSE, TRUE);

        for (int ci = 0; ci < ULS_SLS_LC_NO; ci++)
        {
            if (ULS_SLS_LC[ci].flag == 2)
            {
                file_no++;
                //sprintf(params, "%s%s%d.result", _shield_, params0, file_no);
                sprintf(params, "%s%s%d.result", _shield_, params0, ci + 1);
                f = fopen(params, "rt");

                if (f != NULL)
                {
                    /////////////////////////
                    //file exists
                    //restoring total_load for a case
                    total_load_x = 0.0;
                    total_load_y = 0.0;
                    int first_case_file = case_files[sti];

                    for (i = 0; i < zone_no; i++) {
                        total_load_y += surface_load[first_case_file + ci][zone_property[i].k] * body_area[zone_property[i].k];
                    }
                    for (i = 0; i < shield_no; i++) {
                        total_load_y += surface_load[first_case_file + ci][shield_property[i].k] * body_area[shield_property[i].k];
                    }
                    total_load_x += point_load_x[first_case_file + ci];  //so we have total load for each combination of each state
                    total_load_y += point_load_y[first_case_file + ci];  //so we have total load for each combination of each state

                    total_load_x -= distributed_load_x[first_case_file + ci];  //so we have total load for each combination of each state
                    total_load_y -= distributed_load_y[first_case_file + ci];  //so we have total load for each combination of each state

                    sti_no++;
                    i_row = 0;
                    data_row = data_row_[i_row];
                    //deflection
                    while (fgets(data_row, MaxTextLen, f))
                    {
                        Perm = strstr(data_row, "Perm:");
                        if (Perm != NULL) {
                            strcpy(deflection_chapter[0], data_row_[!i_row]);
                            deflection_chapter[0][strlen(deflection_chapter[0]) - 1] = '\0';
                            modify_chapter((char *) deflection_chapter[0]);
                            ret = sscanf(data_row + 6, "%d %d", &perm_d_node_no, &perm_d_index_no);
                            if ((ret == 2) && (perm_d_node_no > 0) && (perm_d_index_no > 0)) {
                                //perm_d = (PERM *) malloc(perm_d_node_no * sizeof(PERM *));
                                perm_d = (int *) malloc(perm_d_node_no * sizeof(int) + M100);
                                for (i = 0; i < perm_d_node_no; i++) perm_d[i] = -1;
                                perm_d_b = 1;
                                for (i = 0; i < perm_d_index_no; i++) {
                                    fgets(data_row, MaxTextLen, f);
                                    ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                                    //perm_d[node_number-1]=index_number-1;
                                    perm_d[node_number - 1] = i;
                                }
                                for (i = 0; i < DEFLECTION_NUMBER; i++) {
                                    deflection[i] = (double *) malloc(perm_d_index_no * sizeof(double) + M100);
                                    deflection_min[i] = BIGNOF;
                                    deflection_max[i] = -BIGNOF;
                                    if (!deflection_ini) {
                                        min_deflection_min[i] = BIGNOF;
                                        max_deflection_max[i] = -BIGNOF;
                                        min_deflection[i] = (double *) malloc(perm_d_index_no * sizeof(double) + M100);
                                        max_deflection[i] = (double *) malloc(perm_d_index_no * sizeof(double) + M100);

                                        for (j = 0; j < perm_d_index_no; j++) {
                                            min_deflection[i][j] = BIGNOF;
                                            max_deflection[i][j] = -BIGNOF;
                                        }
                                    }
                                }
                                deflection_ini = 1;
                                //deflection 1...EPSILON_NUMBER
                                for (j = 0; j < DEFLECTION_NUMBER; j++) {
                                    for (i = 0; i < perm_d_index_no; i++) {
                                        fgets(data_row, MaxTextLen, f);
                                        ret = sscanf(data_row, "%lf", &deflection[j][i]);

                                        if (deflection[j][i]>0.3)
                                        {
                                            int a=0;
                                        }

                                        deflection_min[j] = min(deflection_min[j], deflection[j][i]);
                                        deflection_max[j] = max(deflection_max[j], deflection[j][i]);

                                        ////combination max
                                        min_deflection_min[j] = min(min_deflection_min[j], deflection_min[j]);
                                        max_deflection_max[j] = max(max_deflection_max[j], deflection_max[j]);

                                        min_deflection[j][i] = min(min_deflection[j][i], deflection[j][i]);
                                        max_deflection[j][i] = max(max_deflection[j][i], deflection[j][i]);
                                        ////
                                    }
                                    if (j < (DEFLECTION_NUMBER - 1)) {
                                        fgets(data_row, MaxTextLen, f);
                                        strcpy(deflection_chapter[j + 1], data_row);
                                        deflection_chapter[j + 1][strlen(deflection_chapter[j + 1]) - 1] = '\0';
                                        modify_chapter((char *) deflection_chapter[j + 1]);
                                        fgets(data_row, MaxTextLen, f);
                                        /////////////////////////
                                        Perm = strstr(data_row, "Perm:");
                                        use_previous = strstr(data_row, "use previous");
                                        if (Perm != NULL) {
                                            if (use_previous != NULL) { ; //do nothing
                                            } else {
                                                ret = sscanf(data_row + 6, "%d %d", &perm_d_node_no, &perm_d_index_no);
                                                if (ret == 2) {
                                                    if (perm_d_b) free(perm_d);
                                                    perm_d = (int *) malloc(perm_d_node_no * sizeof(int) + M100);
                                                    for (i = 0; i < perm_d_node_no; i++) perm_s[i] = -1;
                                                    perm_d_b = 1;
                                                    for (i = 0; i < perm_d_index_no; i++) {
                                                        fgets(data_row, MaxTextLen, f);
                                                        ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                                                        perm_d[node_number - 1] = i;
                                                    }
                                                } else { ; //TO DO - ERROR
                                                }
                                            }
                                        }
                                        /////////////////////////
                                    }
                                }
                                break;
                            }
                        }
                        i_row = !i_row;
                        data_row = data_row_[i_row];
                    }

                    i_row = 0;
                    data_row = data_row_[i_row];

                    //stress
                    perm_s_b = 0;
                    while (fgets(data_row, MaxTextLen, f))
                    {
                        Perm = strstr(data_row, "Perm:");
                        use_previous = strstr(data_row, "use previous");
                        if (Perm != NULL) {
                            strcpy(stress_chapter[0], data_row_[!i_row]);
                            stress_chapter[0][strlen(stress_chapter[0]) - 1] = '\0';
                            modify_chapter((char *) stress_chapter[0]);
                            if (use_previous != NULL) {
                                perm_s = perm_d;
                                perm_s_node_no = perm_d_node_no;
                                perm_s_index_no = perm_d_index_no;
                            } else {
                                ret = sscanf(data_row + 6, "%d %d", &perm_s_node_no, &perm_s_index_no);
                                if (ret == 2) {
                                    perm_s = (int *) malloc(perm_s_node_no * sizeof(int) + M100);
                                    for (i = 0; i < perm_s_node_no; i++) perm_s[i] = -1;
                                    perm_s_b = 1;
                                    for (i = 0; i < perm_s_index_no; i++) {
                                        fgets(data_row, MaxTextLen, f);
                                        ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                                        //perm_s[node_number-1]=index_number-1;
                                        perm_s[node_number - 1] = i;
                                    }
                                } else { ; //TO DO - ERROR
                                }
                            }
                            for (i = 0; i < STRESS_NUMBER; i++)
                            {
                                stress[i] = (double *) malloc(perm_s_index_no * sizeof(double) + M100);
                                stress_min[i] = BIGNOF;
                                stress_max[i] = -BIGNOF;
                                if (!stress_ini) {
                                    min_stress_min[i] = BIGNOF;
                                    max_stress_max[i] = -BIGNOF;
                                    min_stress[i] = (double *) malloc(perm_s_index_no * sizeof(double) + M100);
                                    max_stress[i] = (double *) malloc(perm_s_index_no * sizeof(double) + M100);

                                    for (j = 0; j < perm_s_index_no; j++) {
                                        min_stress[i][j] = BIGNOF;
                                        max_stress[i][j] = -BIGNOF;
                                    }
                                }
                            }
                            stress_ini = 1;
                            //stress 1...6
                            for (j = 0; j < STRESS_NUMBER; j++) {
                                for (i = 0; i < perm_s_index_no; i++) {
                                    fgets(data_row, MaxTextLen, f);
                                    ret = sscanf(data_row, "%lf", &stress[j][i]);
                                    stress_min[j] = min(stress_min[j], stress[j][i]);
                                    stress_max[j] = max(stress_max[j], stress[j][i]);

                                    ////combination max
                                    min_stress_min[j] = min(min_stress_min[j], stress_min[j]);
                                    max_stress_max[j] = max(max_stress_max[j], stress_max[j]);

                                    min_stress[j][i] = min(min_stress[j][i], stress[j][i]);
                                    max_stress[j][i] = max(max_stress[j][i], stress[j][i]);
                                    /////
                                }
                                if (j < (STRESS_NUMBER - 1)) {
                                    fgets(data_row, MaxTextLen, f);
                                    strcpy(stress_chapter[j + 1], data_row);
                                    stress_chapter[j + 1][strlen(stress_chapter[j + 1]) - 1] = '\0';
                                    modify_chapter((char *) stress_chapter[j + 1]);
                                    fgets(data_row, MaxTextLen, f);
                                    /////////////////////////
                                    Perm = strstr(data_row, "Perm:");
                                    use_previous = strstr(data_row, "use previous");
                                    if (Perm != NULL) {
                                        if (use_previous != NULL) { ; //do nothing
                                        } else {
                                            ret = sscanf(data_row + 6, "%d %d", &perm_s_node_no, &perm_s_index_no);
                                            if (ret == 2) {
                                                if (perm_s_b) free(perm_s);
                                                perm_s = (int *) malloc(perm_s_node_no * sizeof(int) + M100);
                                                for (i = 0; i < perm_s_node_no; i++) perm_s[i] = -1;
                                                perm_s_b = 1;
                                                for (i = 0; i < perm_s_index_no; i++) {
                                                    fgets(data_row, MaxTextLen, f);
                                                    ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                                                    perm_s[node_number - 1] = i;
                                                }
                                            } else { ; //TO DO - ERROR
                                            }
                                        }
                                    }

                                    //////REACTIONS
                                    //calculating shear forces and reaction for supported nodes and saving their envelopes
                                    //////////////!!!!!!!!!!!!!!/////////
                                    //reaction based on stress variant
                                    //if ((sti!=2) || (ULSLC_flag[2]!=0)) // if ULSLC, flag must exist
                                    //{
                                    ////all elements done
                                    ////we got:
                                    //// stress[i][perm_s[mesh_element[j].node1 - 1]];
                                    //// stress[i][perm_s[mesh_element[j].node2 - 1]];
                                    //// stress[i][perm_s[mesh_element[j].node3 - 1]];
                                    //// settling now Qx and Qy for element j for limit state i and min-max bi
                                    //// checking if element number j belongs to mesh_boundaries[mesh_boundaries_no]
                                    int last_node = 0;

                                    /////////////////////////
                                    if ((stress[j] != NULL) && (j == 3)) //stress_xy
                                    {
                                        for (int jk = 0; jk < mesh_boundaries_no; jk++)
                                        {
                                            if ((mesh_boundary[jk].restraint > 5)  || (mesh_boundary[jk].restraint == 4)) //6 simple supported, 7 fixed, 4 rolled
                                            {
                                                if (mesh_boundary[jk].node1 > 0)  //node 1 exists
                                                {
                                                    int node1 = 0, node2 = 0;
                                                    int jj;


                                                    //calculating reactions for node1

                                                    double supp_x, supp_y;      // Supported node coordinates (m)
                                                    double H;

                                                    supp_x = mesh_node[mesh_boundary[jk].node1 - 1].x;
                                                    supp_y = mesh_node[mesh_boundary[jk].node1 - 1].y;

                                                    for (jj = 0; jj < mesh_element_no; jj++)
                                                    {
                                                        if (mesh_element[jj].node1 == mesh_boundary[jk].node1)
                                                        {
                                                            supp1_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            supp1_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            supp1_sigxy_max[jk] = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            H=mesh_element[jj].h;
                                                            break;
                                                        }
                                                        else if (mesh_element[jj].node2 == mesh_boundary[jk].node1)
                                                        {
                                                            supp1_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            supp1_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            supp1_sigxy_max[jk] = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            H=mesh_element[jj].h;
                                                            break;
                                                        }
                                                        else if (mesh_element[jj].node3 == mesh_boundary[jk].node1)
                                                        {
                                                            supp1_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            supp1_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            supp1_sigxy_max[jk] = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            H=mesh_element[jj].h;
                                                            break;
                                                        }
                                                    }

                                                    int sti5 = 5;

                                                    //// REACTIONS
                                                    compute_membrane_support_reaction(H, supp1_sigx_max[jk],supp1_sigy_max[jk],supp1_sigxy_max[jk],mesh_boundary[jk].koc, mesh_boundary[jk].kos,mesh_boundary[jk].l1,mesh_boundary[jk].restraint,&mesh_boundary[jk].state1[sti5].Rx,&mesh_boundary[jk].state1[sti5].Ry,&mesh_boundary[jk].state1[sti5].Rn,&mesh_boundary[jk].state1[sti5].Rt);
#ifndef NDEBUG
                                                    //if (last_node != mesh_boundary[jk].node1)
                                                    //{
                                                    printf("node=%d\n", mesh_boundary[jk].node1);
                                                    printf("H=%f\n", H);
                                                    printf("supp.x=%f  supp.y=%f  supp1_sigx=%f  supp1_sigy=%f  supp1_sigxy=%f\n",
                                                           supp_x, supp_y, supp1_sigx_max[jk], supp1_sigy_max[jk], supp1_sigxy_max[jk]);

                                                    printf("l1=%f\n", mesh_boundary[jk].l1);

                                                    printf("Results: Rx=%f Ry=%f Rn=%f Rt=%f\n\n", mesh_boundary[jk].state1[sti].Rx, mesh_boundary[jk].state1[sti].Ry, mesh_boundary[jk].state1[sti].Rn, mesh_boundary[jk].state1[sti].Rt);

                                                    //}
                                                    //last_node=mesh_boundary[jk].node1;
#endif
                                                }


                                                if (mesh_boundary[jk].node2 > 0)  //node 2 exists
                                                {
                                                    int node1 = 0, node2 = 0;
                                                    int jj;

                                                    //calculating reactions for node2

                                                    double supp_x, supp_y;      // Supported node coordinates (m)
                                                    double H;

                                                    supp_x = mesh_node[mesh_boundary[jk].node2 - 1].x;
                                                    supp_y = mesh_node[mesh_boundary[jk].node2 - 1].y;

                                                    for (jj = 0; jj < mesh_element_no; jj++) {
                                                        if (mesh_element[jj].node1 == mesh_boundary[jk].node2)
                                                        {
                                                            supp2_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            supp2_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            supp2_sigxy_max[jk] = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            H=mesh_element[jj].h;
                                                            break;
                                                        }
                                                        else if (mesh_element[jj].node2 == mesh_boundary[jk].node2)
                                                        {
                                                            supp2_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            supp2_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            supp2_sigxy_max[jk] = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            H=mesh_element[jj].h;
                                                            break;
                                                        }
                                                        else if (mesh_element[jj].node3 == mesh_boundary[jk].node2)
                                                        {
                                                            supp2_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            supp2_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            supp2_sigxy_max[jk] = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            H=mesh_element[jj].h;
                                                            break;
                                                        }
                                                    }

                                                    int sti5 = 5;

                                                    //// REACTIONS
                                                    compute_membrane_support_reaction(H, supp2_sigx_max[jk],supp2_sigy_max[jk],supp2_sigxy_max[jk],mesh_boundary[jk].koc,mesh_boundary[jk].kos,mesh_boundary[jk].l2,mesh_boundary[jk].restraint,&mesh_boundary[jk].state2[sti5].Rx,&mesh_boundary[jk].state2[sti5].Ry,&mesh_boundary[jk].state2[sti5].Rn,&mesh_boundary[jk].state2[sti5].Rt);
#ifndef NDEBUG
                                                    //if (last_node != mesh_boundary[jk].node1)
                                                    //{
                                                    printf("node=%d\n", mesh_boundary[jk].node2);
                                                    printf("H=%f\n", H);
                                                    printf("supp.x=%f  supp.y=%f  supp2_sigx=%f  supp2_sigy=%f  supp2_sigxy=%f\n",
                                                           supp_x, supp_y, supp2_sigx_max[jk], supp2_sigy_max[jk], supp2_sigxy_max[jk]);


                                                    printf("l2=%f\n", mesh_boundary[jk].l2);

                                                    printf("Results: Rx=%f Ry=%f Rn=%f Rt=%f\n\n", mesh_boundary[jk].state2[sti].Rx, mesh_boundary[jk].state2[sti].Ry, mesh_boundary[jk].state2[sti].Rn, mesh_boundary[jk].state2[sti].Rt);

                                                    //}
                                                    //last_node=mesh_boundary[jk].node1;
#endif
                                                }

                                            }
                                        }

                                        /////////////////////////////////
                                        //REACTIONS for combination, se they are collected as state no=5 (countiny from 0)
                                        //int
                                        last_node = 0;
                                        int sti5 = 5;

                                        Rxmax = 0.0;
                                        Rymax = 0.0;
                                        Rxabsmax = 0.0;
                                        Ryabsmax = 0.0;
                                        Rnmax = 0.0;
                                        Rtmax = 0.0;

                                        Rxmax_back = 0.0;
                                        Rymax_back = 0.0;
                                        Rxabsmax_back = 0.0;
                                        Ryabsmax_back = 0.0;
                                        Rnmax_back = 0.0;
                                        Rtmax_back = 0.0;
#ifndef NDEBUG
                                        printf("\nState %s-%d(%d)\n", SLS_ULS, ci + 1, ULS_SLS_LC_NO);
#endif
                                        for (i = 0; i < mesh_boundaries_no; i++) {
                                            if ((mesh_boundary[i].restraint > 5) || (mesh_boundary[i].restraint == 4))
                                            {
                                                if (mesh_boundary[i].node1 != last_node)
                                                {
#ifndef NDEBUG
                                                    printf("node=%d  Rx=%f  Ry=%f  Rn=%f  Rt=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti5].Rx, mesh_boundary[i].state1[sti5].Ry, mesh_boundary[i].state1[sti5].Rn, mesh_boundary[i].state1[sti5].Rt);
#endif
                                                    Rxmax += nofabs(mesh_boundary[i].state1[sti5].Rx);
                                                    Rymax += nofabs(mesh_boundary[i].state1[sti5].Ry);
                                                    Rxabsmax += fabs(mesh_boundary[i].state1[sti5].Rx);
                                                    Ryabsmax += fabs(mesh_boundary[i].state1[sti5].Ry);
                                                    Rnmax += (nofabs(mesh_boundary[i].state1[sti5].Rn)*mesh_boundary[i].l1);
                                                    Rtmax += (nofabs(mesh_boundary[i].state1[sti5].Rt)*mesh_boundary[i].l1);
                                                }
                                                last_node = mesh_boundary[i].node1;
                                                if (mesh_boundary[i].node2 != last_node)
                                                {
#ifndef NDEBUG
                                                    printf("node=%d  Rx=%f  Ry=%f  Rn=%f  Rt=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti5].Rx, mesh_boundary[i].state2[sti5].Ry, mesh_boundary[i].state2[sti5].Rn, mesh_boundary[i].state2[sti5].Rt);
#endif
                                                    Rxmax += nofabs(mesh_boundary[i].state2[sti5].Rx);
                                                    Rymax += nofabs(mesh_boundary[i].state2[sti5].Ry);
                                                    Rxabsmax += fabs(mesh_boundary[i].state2[sti5].Rx);
                                                    Ryabsmax += fabs(mesh_boundary[i].state2[sti5].Ry);
                                                    Rnmax += (nofabs(mesh_boundary[i].state2[sti5].Rn)*mesh_boundary[i].l2);
                                                    Rtmax += (nofabs(mesh_boundary[i].state2[sti5].Rt)*mesh_boundary[i].l2);
                                                }
                                                last_node = mesh_boundary[i].node2;
                                            }
                                        }
#ifndef NDEBUG
                                        printf("SUM:  Rx_total_i=%lf  Ry_total_i=%lf  Rn_total_i=%lf  Rt_total_i=%lf\n", Rxmax, Rymax, Rnmax, Rtmax);
#endif
                                        Rxmax_back = Rxmax;
                                        Rymax_back = Rymax;
                                        Rxabsmax_back = Rxabsmax;
                                        Ryabsmax_back = Ryabsmax;
                                        Rnmax_back = Rnmax;
                                        Rtmax_back = Rtmax;
#ifndef NDEBUG
                                        printf("\nTotal load in state %s-%d(%d):  x %f   y %f\n", SLS_ULS, ci + 1, ULS_SLS_LC_NO, total_load_x, total_load_y);
#endif
                                        // Residuals (the error we must eliminate)
                                        double residual_x = total_load_x - Rxmax_back;
                                        double residual_y = total_load_y - Rymax_back;

                                        // Case 1: There is a non-zero applied load in that direction → classic scaling
                                        if (fabs(total_load_x) > 1e-6) {
                                            load_ratio_x = total_load_x / Rxmax_back;
                                        } else {
                                            load_ratio_x = 1.0;   // keep internal balance, only add correction below
                                        }
                                        if (fabs(total_load_y) > 1e-6) {
                                            load_ratio_y = total_load_y / Rymax_back;
                                        } else {
                                            load_ratio_y = 1.0;
                                        }

                                        Rxmax = 0.0;
                                        Rymax = 0.0;
                                        Rxabsmax = 0.0;
                                        Ryabsmax = 0.0;
                                        Rnmax = 0.0;
                                        Rtmax = 0.0;
#ifndef NDEBUG
                                        printf("\nState %s-%d(%d) with correction ratio x %f   y %f\n", SLS_ULS, ci + 1, ULS_SLS_LC_NO, load_ratio_x, load_ratio_y);
#endif
                                        for (i = 0; i < mesh_boundaries_no; i++) {
                                            if ((mesh_boundary[i].restraint > 5) || (mesh_boundary[i].restraint == 4))
                                            {
                                                double nx = -mesh_boundary[i].kos; //-sin_theta;
                                                double ny = mesh_boundary[i].koc; //+cos_theta;
                                                double tx = mesh_boundary[i].koc; // cos_theta;
                                                double ty = mesh_boundary[i].kos; // sin_theta;

                                                if (ci == 0)  //first of combination
                                                {

                                                    mesh_boundary[i].state1[sti5].Rx = load_correction(mesh_boundary[i].state1[sti5].Rx, Rxmax_back, total_load_x, Rxabsmax_back);
                                                    mesh_boundary[i].state1[sti5].Ry = load_correction(mesh_boundary[i].state1[sti5].Ry, Rymax_back, total_load_y, Ryabsmax_back);

                                                    mesh_boundary[i].state1[sti5].Rn = (mesh_boundary[i].state1[sti5].Rx * nx + mesh_boundary[i].state1[sti5].Ry * ny)/mesh_boundary[i].l1;
                                                    mesh_boundary[i].state1[sti5].Rt = (mesh_boundary[i].state1[sti5].Rx * tx + mesh_boundary[i].state1[sti5].Ry * ty)/mesh_boundary[i].l1;

                                                    mesh_boundary[i].state1[sti].Rxmin = mesh_boundary[i].state1[sti5].Rx;
                                                    mesh_boundary[i].state1[sti].Rxmax = mesh_boundary[i].state1[sti5].Rx;
                                                    mesh_boundary[i].state1[sti].Rymin = mesh_boundary[i].state1[sti5].Ry;
                                                    mesh_boundary[i].state1[sti].Rymax = mesh_boundary[i].state1[sti5].Ry;

                                                    mesh_boundary[i].state1[sti].Rnmin = mesh_boundary[i].state1[sti5].Rn;
                                                    mesh_boundary[i].state1[sti].Rnmax = mesh_boundary[i].state1[sti5].Rn;
                                                    mesh_boundary[i].state1[sti].Rtmin = mesh_boundary[i].state1[sti5].Rt;
                                                    mesh_boundary[i].state1[sti].Rtmax = mesh_boundary[i].state1[sti5].Rt;

                                                    mesh_boundary[i].state2[sti5].Rx = load_correction(mesh_boundary[i].state2[sti5].Rx, Rxmax_back, total_load_x, Rxabsmax_back);
                                                    mesh_boundary[i].state2[sti5].Ry = load_correction(mesh_boundary[i].state2[sti5].Ry, Rymax_back, total_load_y, Ryabsmax_back);

                                                    mesh_boundary[i].state2[sti5].Rn = (mesh_boundary[i].state2[sti5].Rx * nx + mesh_boundary[i].state2[sti5].Ry * ny)/mesh_boundary[i].l2;
                                                    mesh_boundary[i].state2[sti5].Rt = (mesh_boundary[i].state2[sti5].Rx * tx + mesh_boundary[i].state2[sti5].Ry * ty)/mesh_boundary[i].l2;

                                                    mesh_boundary[i].state2[sti].Rxmin = mesh_boundary[i].state2[sti5].Rx;
                                                    mesh_boundary[i].state2[sti].Rxmax = mesh_boundary[i].state2[sti5].Rx;
                                                    mesh_boundary[i].state2[sti].Rymin = mesh_boundary[i].state2[sti5].Ry;
                                                    mesh_boundary[i].state2[sti].Rymax = mesh_boundary[i].state2[sti5].Ry;

                                                    mesh_boundary[i].state2[sti].Rnmin = mesh_boundary[i].state2[sti5].Rn;
                                                    mesh_boundary[i].state2[sti].Rnmax = mesh_boundary[i].state2[sti5].Rn;
                                                    mesh_boundary[i].state2[sti].Rtmin = mesh_boundary[i].state2[sti5].Rt;
                                                    mesh_boundary[i].state2[sti].Rtmax = mesh_boundary[i].state2[sti5].Rt;

                                                } else {

                                                    mesh_boundary[i].state1[sti5].Rx = load_correction(mesh_boundary[i].state1[sti5].Rx, Rxmax_back, total_load_x, Rxabsmax_back);
                                                    mesh_boundary[i].state1[sti5].Ry = load_correction(mesh_boundary[i].state1[sti5].Ry, Rymax_back, total_load_y, Ryabsmax_back);

                                                    mesh_boundary[i].state1[sti5].Rn = (mesh_boundary[i].state1[sti5].Rx * nx + mesh_boundary[i].state1[sti5].Ry * ny)/mesh_boundary[i].l1;
                                                    mesh_boundary[i].state1[sti5].Rt = (mesh_boundary[i].state1[sti5].Rx * tx + mesh_boundary[i].state1[sti5].Ry * ty)/mesh_boundary[i].l1;

                                                    mesh_boundary[i].state1[sti].Rxmin = min(mesh_boundary[i].state1[sti].Rxmin, mesh_boundary[i].state1[sti5].Rx);
                                                    mesh_boundary[i].state1[sti].Rxmax = max(mesh_boundary[i].state1[sti].Rxmax, mesh_boundary[i].state1[sti5].Rx);
                                                    mesh_boundary[i].state1[sti].Rymin = min(mesh_boundary[i].state1[sti].Rymin, mesh_boundary[i].state1[sti5].Ry);
                                                    mesh_boundary[i].state1[sti].Rymax = max(mesh_boundary[i].state1[sti].Rymax, mesh_boundary[i].state1[sti5].Ry);

                                                    mesh_boundary[i].state1[sti].Rnmin = min(mesh_boundary[i].state1[sti].Rnmin, mesh_boundary[i].state1[sti5].Rn);
                                                    mesh_boundary[i].state1[sti].Rnmax = max(mesh_boundary[i].state1[sti].Rnmax, mesh_boundary[i].state1[sti5].Rn);
                                                    mesh_boundary[i].state1[sti].Rtmin = min(mesh_boundary[i].state1[sti].Rtmin, mesh_boundary[i].state1[sti5].Rt);
                                                    mesh_boundary[i].state1[sti].Rtmax = max(mesh_boundary[i].state1[sti].Rtmax, mesh_boundary[i].state1[sti5].Rt);

                                                    mesh_boundary[i].state2[sti5].Rx = load_correction(mesh_boundary[i].state2[sti5].Rx, Rxmax_back, total_load_x, Rxabsmax_back);
                                                    mesh_boundary[i].state2[sti5].Ry = load_correction(mesh_boundary[i].state2[sti5].Ry, Rymax_back, total_load_y, Ryabsmax_back);

                                                    mesh_boundary[i].state2[sti5].Rn = (mesh_boundary[i].state2[sti5].Rx * nx + mesh_boundary[i].state2[sti5].Ry * ny)/mesh_boundary[i].l2;
                                                    mesh_boundary[i].state2[sti5].Rt = (mesh_boundary[i].state2[sti5].Rx * tx + mesh_boundary[i].state2[sti5].Ry * ty)/mesh_boundary[i].l2;

                                                    mesh_boundary[i].state2[sti].Rxmin = min(mesh_boundary[i].state2[sti].Rxmin, mesh_boundary[i].state2[sti5].Rx);
                                                    mesh_boundary[i].state2[sti].Rxmax = max(mesh_boundary[i].state2[sti].Rxmax, mesh_boundary[i].state2[sti5].Rx);
                                                    mesh_boundary[i].state2[sti].Rymin = min(mesh_boundary[i].state2[sti].Rymin, mesh_boundary[i].state2[sti5].Ry);
                                                    mesh_boundary[i].state2[sti].Rymax = max(mesh_boundary[i].state2[sti].Rymax, mesh_boundary[i].state2[sti5].Ry);

                                                    mesh_boundary[i].state2[sti].Rnmin = min(mesh_boundary[i].state2[sti].Rnmin, mesh_boundary[i].state2[sti5].Rn);
                                                    mesh_boundary[i].state2[sti].Rnmax = max(mesh_boundary[i].state2[sti].Rnmax, mesh_boundary[i].state2[sti5].Rn);
                                                    mesh_boundary[i].state2[sti].Rtmin = min(mesh_boundary[i].state2[sti].Rtmin, mesh_boundary[i].state2[sti5].Rt);
                                                    mesh_boundary[i].state2[sti].Rtmax = max(mesh_boundary[i].state2[sti].Rtmax, mesh_boundary[i].state2[sti5].Rt);
                                                }


                                                if (mesh_boundary[i].node1 != last_node)
                                                {
#ifndef NDEBUG
                                                    printf("node=%d  Rx=%f  Ry=%f  Rn=%f  Rt=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti5].Rx, mesh_boundary[i].state1[sti5].Ry, mesh_boundary[i].state1[sti5].Rn, mesh_boundary[i].state1[sti5].Rt);
#endif
                                                    Rxmax += (nofabs(mesh_boundary[i].state1[sti5].Rx));
                                                    Rymax += (nofabs(mesh_boundary[i].state1[sti5].Ry));
                                                    Rnmax += (nofabs(mesh_boundary[i].state1[sti5].Rn)*mesh_boundary[i].l1);
                                                    Rtmax += (nofabs(mesh_boundary[i].state1[sti5].Rt)*mesh_boundary[i].l1);
                                                }
                                                last_node = mesh_boundary[i].node1;
                                                if (mesh_boundary[i].node2 != last_node)
                                                {
#ifndef NDEBUG
                                                    printf("node=%d  Rx=%f  Ry=%f  Rn=%f  Rt=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti5].Rx, mesh_boundary[i].state2[sti5].Ry, mesh_boundary[i].state2[sti5].Rn, mesh_boundary[i].state2[sti5].Rt);
#endif
                                                    Rxmax += nofabs(mesh_boundary[i].state2[sti5].Rx);
                                                    Rymax += nofabs(mesh_boundary[i].state2[sti5].Ry);
                                                    Rnmax += (nofabs(mesh_boundary[i].state2[sti5].Rn)*mesh_boundary[i].l2);
                                                    Rtmax += (nofabs(mesh_boundary[i].state2[sti5].Rt)*mesh_boundary[i].l2);
                                                }
                                                last_node = mesh_boundary[i].node2;
                                            }
                                        }
#ifndef NDEBUG
                                        printf("SUM:  Rx_total_i=%f  Ry_total_i=%f  Rn_total_i=%f  Rt_total_i=%f\n", Rxmax, Rymax, Rnmax, Rtmax);
#endif
                                    }
                                    ///////////////////////// REACTIONS
                                }
                            }
                            break;
                        }

                        i_row = !i_row;
                        data_row = data_row_[i_row];
                    }

                    i_row = 0;
                    data_row = data_row_[i_row];

                    //epsilon
                    perm_e_b = 0;
                    while (fgets(data_row, MaxTextLen, f))
                    {
                        Perm = strstr(data_row, "Perm:");
                        use_previous = strstr(data_row, "use previous");
                        if (Perm != NULL)
                        {
                            strcpy(epsilon_chapter[0], data_row_[!i_row]);
                            epsilon_chapter[0][strlen(epsilon_chapter[0]) - 1] = '\0';
                            modify_chapter((char *) epsilon_chapter[0]);
                            if (use_previous != NULL)
                            {
                                perm_e = perm_s;
                                perm_e_node_no = perm_s_node_no;
                                perm_e_index_no = perm_s_index_no;
                            } else {
                                ret = sscanf(data_row + 6, "%d %d", &perm_e_node_no, &perm_e_index_no);
                                if (ret == 2) {
                                    perm_e = (int *) malloc(perm_e_node_no * sizeof(int) + M100);
                                    for (i = 0; i < perm_e_node_no; i++) perm_e[i] = -1;
                                    perm_e_b = 1;
                                    for (i = 0; i < perm_e_index_no; i++) {
                                        fgets(data_row, MaxTextLen, f);
                                        ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                                        perm_e[node_number - 1] = i;
                                    }
                                } else { ; //TO DO - ERROR
                                }
                            }
                            for (i = 0; i < EPSILON_NUMBER; i++) {
                                epsilon[i] = (double *) malloc(perm_e_index_no * sizeof(double) + M100);
                                epsilon_min[i] = BIGNOF;
                                epsilon_max[i] = -BIGNOF;
                                if (!epsilon_ini) {
                                    min_epsilon_min[i] = BIGNOF;
                                    max_epsilon_max[i] = -BIGNOF;
                                    min_epsilon[i] = (double *) malloc(perm_e_index_no * sizeof(double) + M100);
                                    max_epsilon[i] = (double *) malloc(perm_e_index_no * sizeof(double) + M100);

                                    for (j = 0; j < perm_e_index_no; j++) {
                                        min_epsilon[i][j] = BIGNOF;
                                        max_epsilon[i][j] = -BIGNOF;
                                    }
                                }
                            }
                            epsilon_ini = 1;
                            //epsilon 1...6
                            for (j = 0; j < EPSILON_NUMBER; j++)
                            {
                                for (i = 0; i < perm_e_index_no; i++) {
                                    fgets(data_row, MaxTextLen, f);
                                    ret = sscanf(data_row, "%lf", &epsilon[j][i]);
                                    epsilon_min[j] = min(epsilon_min[j], epsilon[j][i]);
                                    epsilon_max[j] = max(epsilon_max[j], epsilon[j][i]);

                                    ////combination max
                                    min_epsilon_min[j] = min(min_epsilon_min[j], epsilon_min[j]);
                                    max_epsilon_max[j] = max(max_epsilon_max[j], epsilon_max[j]);

                                    min_epsilon[j][i] = min(min_epsilon[j][i], epsilon[j][i]);
                                    max_epsilon[j][i] = max(max_epsilon[j][i], epsilon[j][i]);
                                    //////
                                }
                                if (j < (EPSILON_NUMBER - 1)) {
                                    fgets(data_row, MaxTextLen, f);
                                    strcpy(epsilon_chapter[j + 1], data_row);
                                    epsilon_chapter[j + 1][strlen(epsilon_chapter[j + 1]) - 1] = '\0';
                                    modify_chapter((char *) epsilon_chapter[j + 1]);
                                    fgets(data_row, MaxTextLen, f);
                                    /////////////////////////
                                    Perm = strstr(data_row, "Perm:");
                                    use_previous = strstr(data_row, "use previous");
                                    if (Perm != NULL) {
                                        if (use_previous != NULL) { ; //do nothing
                                        } else {
                                            ret = sscanf(data_row + 6, "%d %d", &perm_e_node_no, &perm_e_index_no);
                                            if (ret == 2) {
                                                if (perm_e_b) free(perm_e);
                                                perm_e = (int *) malloc(perm_e_node_no * sizeof(int) + M100);
                                                for (i = 0; i < perm_e_node_no; i++) perm_e[i] = -1;
                                                perm_e_b = 1;
                                                for (i = 0; i < perm_e_index_no; i++) {
                                                    fgets(data_row, MaxTextLen, f);
                                                    ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                                                    perm_e[node_number - 1] = i;
                                                }
                                            } else { ; //TO DO - ERROR
                                            }
                                        }
                                        /////////////////////////
                                    }
                                }
                            }
                            break;
                        }
                        i_row = !i_row;
                        data_row = data_row_[i_row];
                    }

                    i_row = 0;
                    data_row = data_row_[i_row];

                    fclose(f);
                }
            }
        }

        ///creating results for combinationsd
        if (sti_no > 0) {
            nom_max = 1;
            goto singles_multiples;
        } else {
            if (sti == 2)  //ULSLC
            {
                ULSLC_flag[2] = 0;
                nom_max = 1;
                for (int is = 0; is < STRESS_NUMBER; is++) {
                    min_stress[is] = min_stress_bak[is];
                    max_stress[is] = max_stress_bak[is];
                    min_stress_min[is] = min_stress_min_bak[is];
                    max_stress_max[is] = max_stress_max_bak[is];
                }
                perm_s = perm_s_bak;
                perm_d_b = 0;
                perm_s_b = 0;
                perm_e_b = 0;

                sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "stress", SLS_ULS, combination_no);

                memcpy(desired_layer_upper, desired_layer, strlen(desired_layer) + 1);
                utf8Upper(desired_layer_upper);

                for (ii = 0; ii < No_Layers; ii++) {
                    memcpy(layer_name_upper, Layers[ii].name, strlen(Layers[ii].name) + 1);
                    utf8Upper(layer_name_upper);

                    if (strcmp(layer_name_upper, desired_layer_upper) == 0)

                        break;
                }
                if (ii < No_Layers) {
                    desired_layer_no = ii;
                    if (block_names)
                        get_blocks_setup(desired_layer_no, block_names, &block_names_no, MAX_L_BLOCKS, MAX_L_BLOCKS_LEN);  //to remember only visible blocks
                    delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
                }


                goto singles_multiples;
            } else if (sti == 3) //SLSLC
            {
                ULSLC_flag[3] = 0;

                sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "stress", SLS_ULS, combination_no);

                memcpy(desired_layer_upper, desired_layer, strlen(desired_layer) + 1);
                utf8Upper(desired_layer_upper);

                for (ii = 0; ii < No_Layers; ii++) {
                    memcpy(layer_name_upper, Layers[ii].name, strlen(Layers[ii].name) + 1);
                    utf8Upper(layer_name_upper);

                    if (strcmp(layer_name_upper, desired_layer_upper) == 0)
                        break;
                }
                if (ii < No_Layers) {
                    desired_layer_no = ii;
                    if (block_names)
                        get_blocks_setup(desired_layer_no, block_names, &block_names_no, MAX_L_BLOCKS,MAX_L_BLOCKS_LEN);  //to remember only visible blocks
                    delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
                }

                continue;
            } else if (sti == 4)  //QPSLSLC
            {
                ULSLC_flag[4] = 0;

                sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "stress", SLS_ULS, combination_no);

                memcpy(desired_layer_upper, desired_layer, strlen(desired_layer) + 1);
                utf8Upper(desired_layer_upper);

                for (ii = 0; ii < No_Layers; ii++) {

                    memcpy(layer_name_upper, Layers[ii].name, strlen(Layers[ii].name) + 1);
                    utf8Upper(layer_name_upper);

                    if (strcmp(layer_name_upper, desired_layer_upper) == 0)
                        break;
                }
                if (ii < No_Layers) {
                    desired_layer_no = ii;
                    if (block_names)
                        get_blocks_setup(desired_layer_no, block_names, &block_names_no, MAX_L_BLOCKS,MAX_L_BLOCKS_LEN);  //to remember only visible blocks
                    delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
                }

                continue;
            } else continue;
        }

        singles:

        nom_max = 0;

        //results
        switch (sti)
        {
        case 0: //sls
            sprintf(params, "%sshield_sls.result", _shield_);
            strncpy(SLS_ULS, "sls", 3);  //temporary
            break;
        case 1: //uls
            sprintf(params, "%sshield_uls.result", _shield_);
            strncpy(SLS_ULS, "ULS", 3);  //temporary
            break;
        }

        sprintf(building_blocks,"%s %s", _BUILDING_BLOCKS_, SLS_ULS);
        komunikat_str_short(building_blocks, FALSE, TRUE);

        //results
        f = fopen(params, "rt");

        if (f == NULL)
        {

            ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_OPEN_RESULTS_FILE_,
                               12,
                               params, 11, 1, 62);
            no_error = FALSE;
            goto sh_error1;

        }

        //restoring total_load for a case
        total_load_x = 0.0;
        total_load_y = 0.0;
        for (i = 0; i < zone_no; i++) {
            total_load_y += surface_load[sti][zone_property[i].k] * body_area[zone_property[i].k];
        }
        for (i = 0; i < shield_no; i++) {
            total_load_y += surface_load[sti][shield_property[i].k] * body_area[shield_property[i].k];
        }
        total_load_x += point_load_x[sti];
        total_load_y += point_load_y[sti];

        total_load_x -= distributed_load_x[sti];
        total_load_y -= distributed_load_y[sti];

        i_row = 0;
        data_row = data_row_[i_row];
        //deflection
        while (fgets(data_row, MaxTextLen, f))
        {
            Perm = strstr(data_row, "Perm:");
            if (Perm != NULL) {
                strcpy(deflection_chapter[0], data_row_[!i_row]);
                deflection_chapter[0][strlen(deflection_chapter[0]) - 1] = '\0';
                modify_chapter((char *) deflection_chapter[0]);
                ret = sscanf(data_row + 6, "%d %d", &perm_d_node_no, &perm_d_index_no);
                if ((ret == 2) && (perm_d_node_no > 0) && (perm_d_index_no > 0)) {
                    //perm_d = (PERM *) malloc(perm_d_node_no * sizeof(PERM *));
                    perm_d = (int *) malloc(perm_d_node_no * sizeof(int) + M100);
                    for (i = 0; i < perm_d_node_no; i++) perm_d[i] = -1;
                    perm_d_b = 1;
                    for (i = 0; i < perm_d_index_no; i++) {
                        fgets(data_row, MaxTextLen, f);
                        ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                        //perm_d[node_number-1]=index_number-1;
                        perm_d[node_number - 1] = i;
                    }
                    for (i = 0; i < DEFLECTION_NUMBER; i++) {
                        deflection[i] = (double *) malloc(perm_d_index_no * sizeof(double) + M100);
                        deflection_min[i] = BIGNOF;
                        deflection_max[i] = -BIGNOF;
                    }
                    //deflection 1...EPSILON_NUMBER
                    for (j = 0; j < DEFLECTION_NUMBER; j++) {
                        for (i = 0; i < perm_d_index_no; i++) {
                            fgets(data_row, MaxTextLen, f);
                            ret = sscanf(data_row, "%lf", &deflection[j][i]);

                            if (deflection[j][i]>0.3)
                            {
                                int a=0;
                            }

                            deflection_min[j] = min(deflection_min[j], deflection[j][i]);
                            deflection_max[j] = max(deflection_max[j], deflection[j][i]);
                        }
                        if (j < (DEFLECTION_NUMBER - 1)) {
                            fgets(data_row, MaxTextLen, f);
                            strcpy(deflection_chapter[j + 1], data_row);
                            deflection_chapter[j + 1][strlen(deflection_chapter[j + 1]) - 1] = '\0';
                            modify_chapter((char *) deflection_chapter[j + 1]);
                            fgets(data_row, MaxTextLen, f);
                            /////////////////////////
                            Perm = strstr(data_row, "Perm:");
                            use_previous = strstr(data_row, "use previous");
                            if (Perm != NULL) {
                                if (use_previous != NULL) { ; //do nothing
                                } else {
                                    ret = sscanf(data_row + 6, "%d %d", &perm_d_node_no, &perm_d_index_no);
                                    if (ret == 2) {
                                        if (perm_d_b) free(perm_d);
                                        perm_d = (int *) malloc(perm_d_node_no * sizeof(int) + M100);
                                        for (i = 0; i < perm_d_node_no; i++) perm_s[i] = -1;
                                        perm_d_b = 1;
                                        for (i = 0; i < perm_d_index_no; i++) {
                                            fgets(data_row, MaxTextLen, f);
                                            ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                                            perm_d[node_number - 1] = i;
                                        }
                                    } else { ; //TO DO - ERROR
                                    }
                                }
                            }
                            /////////////////////////
                        }
                    }
                    break;
                }
            }
            i_row = !i_row;
            data_row = data_row_[i_row];
        }

        i_row = 0;
        data_row = data_row_[i_row];

        //stress
        perm_s_b = 0;
        while (fgets(data_row, MaxTextLen, f))
        {
            Perm = strstr(data_row, "Perm:");
            use_previous = strstr(data_row, "use previous");
            if (Perm != NULL) {
                strcpy(stress_chapter[0], data_row_[!i_row]);
                stress_chapter[0][strlen(stress_chapter[0]) - 1] = '\0';
                modify_chapter((char *) stress_chapter[0]);
                if (use_previous != NULL) {
                    perm_s = perm_d;
                    perm_s_node_no = perm_d_node_no;
                    perm_s_index_no = perm_d_index_no;
                } else {
                    ret = sscanf(data_row + 6, "%d %d", &perm_s_node_no, &perm_s_index_no);
                    if (ret == 2) {
                        perm_s = (int *) malloc(perm_s_node_no * sizeof(int) + M100);
                        for (i = 0; i < perm_s_node_no; i++) perm_s[i] = -1;
                        perm_s_b = 1;
                        for (i = 0; i < perm_s_index_no; i++) {
                            fgets(data_row, MaxTextLen, f);
                            ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                            //perm_s[node_number-1]=index_number-1;
                            perm_s[node_number - 1] = i;
                        }
                    } else { ; //TO DO - ERROR
                    }
                }
                for (i = 0; i < STRESS_NUMBER; i++) {
                    stress[i] = (double *) malloc(perm_s_index_no * sizeof(double) + M100);
                    stress_min[i] = BIGNOF;
                    stress_max[i] = -BIGNOF;
                }
                //stress 1...6
                for (j = 0; j < STRESS_NUMBER; j++) {
                    for (i = 0; i < perm_s_index_no; i++) {
                        fgets(data_row, MaxTextLen, f);
                        ret = sscanf(data_row, "%lf", &stress[j][i]);
                        stress_min[j] = min(stress_min[j], stress[j][i]);
                        stress_max[j] = max(stress_max[j], stress[j][i]);
                    }
                    if (j < (STRESS_NUMBER - 1)) {
                        fgets(data_row, MaxTextLen, f);
                        strcpy(stress_chapter[j + 1], data_row);
                        stress_chapter[j + 1][strlen(stress_chapter[j + 1]) - 1] = '\0';
                        modify_chapter((char *) stress_chapter[j + 1]);
                        fgets(data_row, MaxTextLen, f);
                        /////////////////////////
                        Perm = strstr(data_row, "Perm:");
                        use_previous = strstr(data_row, "use previous");
                        if (Perm != NULL) {
                            if (use_previous != NULL) { ; //do nothing
                            } else {
                                ret = sscanf(data_row + 6, "%d %d", &perm_s_node_no, &perm_s_index_no);
                                if (ret == 2) {
                                    if (perm_s_b) free(perm_s);
                                    perm_s = (int *) malloc(perm_s_node_no * sizeof(int) + M100);
                                    for (i = 0; i < perm_s_node_no; i++) perm_s[i] = -1;
                                    perm_s_b = 1;
                                    for (i = 0; i < perm_s_index_no; i++) {
                                        fgets(data_row, MaxTextLen, f);
                                        ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                                        perm_s[node_number - 1] = i;
                                    }
                                } else { ; //TO DO - ERROR
                                }
                            }
                        }
                        /////////////////////////
                    }
                }
                break;
            }
            i_row = !i_row;
            data_row = data_row_[i_row];
        }

        i_row = 0;
        data_row = data_row_[i_row];

        //epsilon
        perm_e_b = 0;
        while (fgets(data_row, MaxTextLen, f))
        {
            Perm = strstr(data_row, "Perm:");
            use_previous = strstr(data_row, "use previous");
            if (Perm != NULL) {
                strcpy(epsilon_chapter[0], data_row_[!i_row]);
                epsilon_chapter[0][strlen(epsilon_chapter[0]) - 1] = '\0';
                modify_chapter((char *) epsilon_chapter[0]);
                if (use_previous != NULL) {
                    perm_e = perm_s;
                    perm_e_node_no = perm_s_node_no;
                    perm_e_index_no = perm_s_index_no;
                } else {
                    ret = sscanf(data_row + 6, "%d %d", &perm_e_node_no, &perm_e_index_no);
                    if (ret == 2) {
                        perm_e = (int *) malloc(perm_e_node_no * sizeof(int) + M100);
                        for (i = 0; i < perm_e_node_no; i++) perm_e[i] = -1;
                        perm_e_b = 1;
                        for (i = 0; i < perm_e_index_no; i++) {
                            fgets(data_row, MaxTextLen, f);
                            ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                            perm_e[node_number - 1] = i;
                        }
                    } else { ; //TO DO - ERROR
                    }
                }
                for (i = 0; i < EPSILON_NUMBER; i++) {
                    epsilon[i] = (double *) malloc(perm_e_index_no * sizeof(double) + M100);
                    epsilon_min[i] = BIGNOF;
                    epsilon_max[i] = -BIGNOF;
                }
                //epsilon 1...6
                for (j = 0; j < EPSILON_NUMBER; j++) {
                    for (i = 0; i < perm_e_index_no; i++) {
                        fgets(data_row, MaxTextLen, f);
                        ret = sscanf(data_row, "%lf", &epsilon[j][i]);
                        epsilon_min[j] = min(epsilon_min[j], epsilon[j][i]);
                        epsilon_max[j] = max(epsilon_max[j], epsilon[j][i]);
                    }
                    if (j < (EPSILON_NUMBER - 1)) {
                        fgets(data_row, MaxTextLen, f);
                        strcpy(epsilon_chapter[j + 1], data_row);
                        epsilon_chapter[j + 1][strlen(epsilon_chapter[j + 1]) - 1] = '\0';
                        modify_chapter((char *) epsilon_chapter[j + 1]);
                        fgets(data_row, MaxTextLen, f);
                        /////////////////////////
                        Perm = strstr(data_row, "Perm:");
                        use_previous = strstr(data_row, "use previous");
                        if (Perm != NULL) {
                            if (use_previous != NULL) { ; //do nothing
                            } else {
                                ret = sscanf(data_row + 6, "%d %d", &perm_e_node_no, &perm_e_index_no);
                                if (ret == 2) {
                                    if (perm_e_b) free(perm_e);
                                    perm_e = (int *) malloc(perm_e_node_no * sizeof(int) + M100);
                                    for (i = 0; i < perm_e_node_no; i++) perm_e[i] = -1;
                                    perm_e_b = 1;
                                    for (i = 0; i < perm_e_index_no; i++) {
                                        fgets(data_row, MaxTextLen, f);
                                        ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                                        perm_e[node_number - 1] = i;
                                    }
                                } else { ; //TO DO - ERROR
                                }
                            }
                        }
                        /////////////////////////
                    }
                }
                break;
            }
            i_row = !i_row;
            data_row = data_row_[i_row];
        }

        i_row = 0;
        data_row = data_row_[i_row];

        fclose(f);

        singles_multiples:

        my_sleep(10);

        ////creating deflection blocks

        Ldsp.kolor = static_colors.deformation_color;
        Ldsp.typ = 64;

        sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "deflection", SLS_ULS, combination_no);

        memcpy(desired_layer_upper, desired_layer, strlen(desired_layer) + 1);
        utf8Upper(desired_layer_upper);

        for (ii = 0; ii < No_Layers; ii++) {
            memcpy(layer_name_upper, Layers[ii].name, strlen(Layers[ii].name) + 1);
            utf8Upper(layer_name_upper);

            if (strcmp(layer_name_upper, desired_layer_upper) == 0)
                break;
        }
        if (ii < No_Layers) {
            desired_layer_no = ii;
            if (block_names)
                get_blocks_setup(desired_layer_no, block_names, &block_names_no, MAX_L_BLOCKS,MAX_L_BLOCKS_LEN);  //to remember only visible blocks
            delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
        } else {
            if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

                ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_,
                                   12, (char *) "", 11, 1, 62);
                no_error = FALSE;
                goto sh_error1;
            }
            if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

                ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_,
                                   12, (char *) "", 11, 1, 62);
                no_error = FALSE;
                goto sh_error1;
            }
            No_Layers++;
            desired_layer_no = No_Layers - 1;
            Layers[No_Layers - 1].on = 1;
            Layers[No_Layers - 1].edit = 1;
            Layers[No_Layers - 1].point = 1;
            strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
            Layers[No_Layers - 1].color = Ldsp.kolor;
        }

        if ((sti == 2) && (ULSLC_flag[2] == 0)) // non existing ULSLC
            goto stress_block;

        for (i = 0; i < DEFLECTION_NUMBER; i++) {
            if ((theta_ == 0) && (i == (DEFLECTION_NUMBER - 3)))
                break;

            Ldsp.warstwa = desired_layer_no;
            w.warstwa = Ldsp.warstwa;
            w.kolor = Ldsp.kolor;
            w.blok = 1;

            //from meters to drawing units
            blok_origin.x = jednostkiObX(mesh_node[0].x);
            blok_origin.y = jednostkiObY(mesh_node[0].y);


            if (nom_max) {
                bim = 2;
                min_max = _min_;
            } else {
                bim = 1;
                min_max = _nope_;
            }

            for (bi = 0; bi < bim; bi++) {

                if (bi > 0) min_max = _max_;

                if (nom_max) {
                    //checking bipolarity
                    if (bi == 0)  //potentially minimal
                    {
                        if ((max_deflection_max[i] > 0.) && (min_deflection_min[i] >= 0.))
                            continue;  //we do not create zero unipolar graph
                    }
                    if (bi == 1)  //potentially maximal
                    {
                        if ((max_deflection_max[i] <= 0.) && (min_deflection_min[i] < 0))
                            continue; //we do not create zero unipolar graph
                    }
                }

                sprintf(block_name, "%s$%s_%s%s_%d", ptr_id_short, deflection_chapter[i], SLS_ULS, min_max, combination_no);

                hiding = TRUE;
                for (j = 0; j < block_names_no; j++) {
                    if (strcmp(block_names[j], block_name) == 0) {
                        hiding = FALSE;
                        break;
                    }
                }

                if (nom_max) deflection_amax = max(fabs(max_deflection_max[i]), fabs(min_deflection_min[i]));
                else deflection_amax = max(fabs(deflection_max[i]), fabs(deflection_min[i]));

                if (deflection_amax > 0.0) {
                    ptr_block = add_block(blok_origin.x, blok_origin.y, B_GRAPH, block_name, hiding);
                    if (ptr_block == NULL) {
                        fclose(f);
                        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "",
                                           (char *) _CANNOT_CREATE_DEFLECTION_BLOCK_, 12, (char *) "", 11, 1, 62);
                        no_error = FALSE;
                        goto sh_error1;
                    }

                    for (j = 0; j < mesh_element_no; j++) {
                        w.xy[0] = jednostkiObX(mesh_node[mesh_element[j].node1 - 1].x);
                        w.xy[1] = jednostkiObY(mesh_node[mesh_element[j].node1 - 1].y);

                        w.xy[2] = jednostkiObX(mesh_node[mesh_element[j].node2 - 1].x);
                        w.xy[3] = jednostkiObY(mesh_node[mesh_element[j].node2 - 1].y);

                        w.xy[4] = jednostkiObX(mesh_node[mesh_element[j].node3 - 1].x);
                        w.xy[5] = jednostkiObY(mesh_node[mesh_element[j].node3 - 1].y);

                        if (nom_max) {
                            if (bi == 0) {
                                if (perm_d[mesh_element[j].node1 - 1] == -1) gradient3.c1 = getRGB(0, deflection_amax);
                                else gradient3.c1 = getRGB(-min_deflection[i][perm_d[mesh_element[j].node1 - 1]], deflection_amax);
                                if (perm_d[mesh_element[j].node2 - 1] == -1) gradient3.c2 = getRGB(0, deflection_amax);
                                else gradient3.c2 = getRGB(-min_deflection[i][perm_d[mesh_element[j].node2 - 1]], deflection_amax);
                                if (perm_d[mesh_element[j].node3 - 1] == -1) gradient3.c3 = getRGB(0, deflection_amax);
                                else gradient3.c3 = getRGB(-min_deflection[i][perm_d[mesh_element[j].node3 - 1]], deflection_amax);
                                //gradient.c4 = getRGB(0.0, deflection_amax);

                                fe_data3.el_number = j + 1;
                                fe_data3.flag = 0;
                                fe_data3.f1 = (float) min_deflection[i][perm_d[mesh_element[j].node1 - 1]];
                                fe_data3.f2 = (float) min_deflection[i][perm_d[mesh_element[j].node2 - 1]];
                                fe_data3.f3 = (float) min_deflection[i][perm_d[mesh_element[j].node3 - 1]];
                                //fe_data.f4 = 0.0f;
                            } else {
                                if (perm_d[mesh_element[j].node1 - 1] == -1) gradient3.c1 = getRGB(0, deflection_amax);
                                else gradient3.c1 = getRGB(-max_deflection[i][perm_d[mesh_element[j].node1 - 1]], deflection_amax);
                                if (perm_d[mesh_element[j].node2 - 1] == -1) gradient3.c2 = getRGB(0, deflection_amax);
                                else gradient3.c2 = getRGB(-max_deflection[i][perm_d[mesh_element[j].node2 - 1]], deflection_amax);
                                if (perm_d[mesh_element[j].node3 - 1] == -1) gradient3.c3 = getRGB(0, deflection_amax);
                                else gradient3.c3 = getRGB(-max_deflection[i][perm_d[mesh_element[j].node3 - 1]], deflection_amax);
                                //gradient.c4 = getRGB(0.0, deflection_amax);

                                fe_data3.el_number = j + 1;
                                fe_data3.flag = 0;
                                fe_data3.f1 = (float) max_deflection[i][perm_d[mesh_element[j].node1 - 1]];
                                fe_data3.f2 = (float) max_deflection[i][perm_d[mesh_element[j].node2 - 1]];
                                fe_data3.f3 = (float) max_deflection[i][perm_d[mesh_element[j].node3 - 1]];
                                //fe_data.f4 = 0.0f;
                            }
                        } else {
                            if (perm_d[mesh_element[j].node1 - 1] == -1) gradient3.c1 = getRGB(0, deflection_amax);
                            else gradient3.c1 = getRGB(-deflection[i][perm_d[mesh_element[j].node1 - 1]], deflection_amax);
                            if (perm_d[mesh_element[j].node2 - 1] == -1) gradient3.c2 = getRGB(0, deflection_amax);
                            else gradient3.c2 = getRGB(-deflection[i][perm_d[mesh_element[j].node2 - 1]], deflection_amax);
                            if (perm_d[mesh_element[j].node3 - 1] == -1) gradient3.c3 = getRGB(0, deflection_amax);
                            else gradient3.c3 = getRGB(-deflection[i][perm_d[mesh_element[j].node3 - 1]], deflection_amax);
                            //gradient.c4 = getRGB(0.0, deflection_amax);

                            fe_data3.el_number = j + 1;
                            fe_data3.flag = 0;
                            fe_data3.f1 = (float) deflection[i][perm_d[mesh_element[j].node1 - 1]];
                            fe_data3.f2 = (float) deflection[i][perm_d[mesh_element[j].node2 - 1]];
                            fe_data3.f3 = (float) deflection[i][perm_d[mesh_element[j].node3 - 1]];
                            //fe_data.f4 = 0.0f;
                        }

                        w.lp = 6;
                        w.gradient = 1;

                        w.translucent = 1;
                        translucency_ptr = (char *) w.xy;
                        translucency_ptr += (w.lp * sizeof(float));
                        memmove(translucency_ptr, &HalfTranslucency, sizeof(unsigned char));

                        gradient_ptr = translucency_ptr + sizeof(unsigned char);
                        memmove(gradient_ptr, &gradient3, sizeof(GRADIENT3));

                        fe_data_ptr = gradient_ptr + sizeof(GRADIENT3);
                        memmove(fe_data_ptr, &fe_data3, sizeof(FE_DATA3));

                        w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT3) + sizeof(FE_DATA3);
                        adr = dodaj_obiekt((BLOK *) dane, &w);
                    }

                    //colorbar
                    if (deflection_amax > 0.0) {
                        w.xy[0] = (float) sh_min_x + 10.f;
                        w.xy[1] = (float) sh_min_y - 10.f;

                        w.xy[2] = w.xy[0] + 50.f;
                        w.xy[3] = w.xy[1];

                        w.xy[4] = w.xy[2];
                        w.xy[5] = w.xy[3] - 5.f;

                        w.xy[6] = w.xy[0];
                        w.xy[7] = w.xy[5];

                        if (nom_max) {
                            if (bi == 0) {
                                gradient4.c1 = getRGB(-min_deflection_min[i], deflection_amax);
                                gradient4.c2 = getRGB(-max_deflection_max[i], deflection_amax);
                                gradient4.c3 = getRGB(-max_deflection_max[i], deflection_amax);
                                gradient4.c4 = getRGB(-min_deflection_min[i], deflection_amax);

                                fe_data4.el_number = 0;
                                fe_data4.flag = 0;
                                fe_data4.f1 = (float) -min_deflection_min[i];
                                fe_data4.f2 = (float) -max_deflection_max[i];
                                fe_data4.f3 = (float) -max_deflection_max[i];
                                fe_data4.f4 = (float) -min_deflection_min[i];
                            } else {
                                gradient4.c1 = getRGB(-min_deflection_min[i], deflection_amax);
                                gradient4.c2 = getRGB(-max_deflection_max[i], deflection_amax);
                                gradient4.c3 = getRGB(-max_deflection_max[i], deflection_amax);
                                gradient4.c4 = getRGB(-min_deflection_min[i], deflection_amax);

                                fe_data4.el_number = 0;
                                fe_data4.flag = 0;
                                fe_data4.f1 = (float) -min_deflection_min[i];
                                fe_data4.f2 = (float) -max_deflection_max[i];
                                fe_data4.f3 = (float) -max_deflection_max[i];
                                fe_data4.f4 = (float) -min_deflection_min[i];
                            }
                        } else {
                            gradient4.c1 = getRGB(-deflection_min[i], deflection_amax);
                            gradient4.c2 = getRGB(-deflection_max[i], deflection_amax);
                            gradient4.c3 = getRGB(-deflection_max[i], deflection_amax);
                            gradient4.c4 = getRGB(-deflection_min[i], deflection_amax);

                            fe_data4.el_number = 0;
                            fe_data4.flag = 0;
                            fe_data4.f1 = (float) -deflection_min[i];
                            fe_data4.f2 = (float) -deflection_max[i];
                            fe_data4.f3 = (float) -deflection_max[i];
                            fe_data4.f4 = (float) -deflection_min[i];
                        }

                        w.lp = 8;
                        w.gradient = 1;
                        w.translucent = 1;
                        translucency_ptr = (char *) w.xy;
                        translucency_ptr += (w.lp * sizeof(float));
                        memmove(translucency_ptr, &HalfTranslucency, sizeof(unsigned char));

                        gradient_ptr = translucency_ptr + sizeof(unsigned char);
                        memmove(gradient_ptr, &gradient4, sizeof(GRADIENT4));

                        fe_data_ptr = gradient_ptr + sizeof(GRADIENT4);
                        memmove(fe_data_ptr, &fe_data4, sizeof(FE_DATA4));

                        w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT4) + sizeof(FE_DATA4);
                        adr = dodaj_obiekt((BLOK *) dane, &w);

                        //adjust deflection to proper units
                        if (i < 3)  //in [mm]
                        {
                            if (UNITS == SI) {
                                if (nom_max) {
                                    deflection_min_ = min_deflection_min[i] * 1000.0; //in mm
                                    deflection_max_ = max_deflection_max[i] * 1000.0; //in mm
                                } else {
                                    deflection_min_ = deflection_min[i] * 1000.0; //in mm
                                    deflection_max_ = deflection_max[i] * 1000.0; //in mm
                                }
                                unit = _mm_;
                            } else //IMP stays as is  in inches
                            {
                                if (nom_max) {
                                    deflection_min_ = min_deflection_min[i]; //in inch
                                    deflection_max_ = max_deflection_max[i]; //in inch
                                } else {
                                    deflection_min_ = deflection_min[i]; //in inch
                                    deflection_max_ = deflection_max[i]; //in inch
                                }
                                unit = _inch_;
                            }
                        } else {
                            //in radians
                            if (nom_max) {
                                deflection_min_ = min_deflection_min[i]; //in inch
                                deflection_max_ = max_deflection_max[i]; //in inch
                            } else {
                                deflection_min_ = deflection_min[i]; //in inch
                                deflection_max_ = deflection_max[i]; //in inch
                            }
                            unit = _rad_;
                        }

                        T.warstwa = Ldsp.warstwa;
                        T.kolor = Ldsp.kolor;
                        T.italics = 0;
                        T.underline = 0;
                        T.wysokosc = (float) (zmwym.wysokosc * 0.5);
                        T.kat = 0.0f;
                        T.ukryty = 0;

                        T.typ = 15;

                        T.x = w.xy[0];
                        T.y = w.xy[1] + 0.5f;
                        T.justowanie = j_do_lewej;
                        sprintf(T.text, "%.5f", deflection_min_);

                        T.dl = strlen(T.text);
                        T.n = T18 + T.dl;
                        normalize_txt(&T);
                        adr = dodaj_obiekt((BLOK *) dane, &T);

                        T.x = w.xy[2];
                        T.y = w.xy[3] + 0.5f;
                        T.justowanie = j_do_prawej;
                        sprintf(T.text, "%.4f", deflection_max_);

                        T.dl = strlen(T.text);
                        T.n = T18 + T.dl;
                        normalize_txt(&T);
                        adr = dodaj_obiekt((BLOK *) dane, &T);

                        T.x = (w.xy[0] + w.xy[2]) / 2.f;
                        T.y = w.xy[3] + 0.5f;
                        T.justowanie = j_srodkowo;
                        sprintf(T.text, "%s  %s%s %s", Utf8StrMakeUprUtf8Str(SLS_ULS), deflection_chapter[i], min_max, unit);

                        T.dl = strlen(T.text);
                        T.n = T18 + T.dl;

                        normalize_txt(&T);
                        adr = dodaj_obiekt((BLOK *) dane, &T);

                        //changing back
                        T.typ = 0;
                    }
                }
            }
        }

        my_sleep(10);
        ////creating stress blocks

        Ldsp.kolor = static_colors.node_element_color;
        Ldsp.typ = 64;

        sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "stress", SLS_ULS, combination_no);

        memcpy(desired_layer_upper, desired_layer, strlen(desired_layer) + 1);
        utf8Upper(desired_layer_upper);

        for (ii = 0; ii < No_Layers; ii++) {
            memcpy(layer_name_upper, Layers[ii].name, strlen(Layers[ii].name) + 1);
            utf8Upper(layer_name_upper);

            if (strcmp(layer_name_upper, desired_layer_upper) == 0)
                break;
        }

        if (ii < No_Layers) {
            desired_layer_no = ii;
            if (block_names)
                get_blocks_setup(desired_layer_no, block_names, &block_names_no, MAX_L_BLOCKS,MAX_L_BLOCKS_LEN);  //to remember only visible blocks
            delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
        } else {
            if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

                ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_,
                                   12, (char *) "", 11, 1, 62);
                no_error = FALSE;
                goto sh_error1;
            }
            if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

                ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_,
                                   12, (char *) "", 11, 1, 62);
                no_error = FALSE;
                goto sh_error1;
            }
            No_Layers++;
            desired_layer_no = No_Layers - 1;
            Layers[No_Layers - 1].on = 1;
            Layers[No_Layers - 1].edit = 1;
            Layers[No_Layers - 1].point = 1;
            strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
            Layers[No_Layers - 1].color = Ldsp.kolor;
        }
        stress_block:

        if (perm_s != NULL) {

            for (i = 0; i < STRESS_NUMBER; i++) {
                if ((sigma_eq_ == 0) && (i == (STRESS_NUMBER - 1)))
                    break;
                Ldsp.warstwa = desired_layer_no;
                w.warstwa = Ldsp.warstwa;
                w.kolor = Ldsp.kolor;
                w.blok = 1;

                //from meters to drawing units
                blok_origin.x = jednostkiObX(mesh_node[0].x);
                blok_origin.y = jednostkiObY(mesh_node[0].y);

                if (nom_max) {
                    bim = 2;
                    min_max = _min_;
                } else {
                    bim = 1;
                    min_max = _nope_;
                }

                for (bi = 0; bi < bim; bi++) {
                    if (bi > 0) min_max = _max_;

                    if (nom_max) {
                        //checking bipolarity
                        if (bi == 0)  //potentially minimal
                        {
                            if ((max_stress_max[i] > 0.) && (min_stress_min[i] >= 0.))
                                do_nothing(); //continue;  //we do not create zero unipolar graph
                        }
                        if (bi == 1)  //potentially maximal
                        {
                            if ((max_stress_max[i] <= 0.) && (min_stress_min[i] < 0.))
                                do_nothing(); //continue; //we do not create zero unipolar graph
                        }
                    }

                    sprintf(block_name, "%s$%s_%s%s_%d", ptr_id_short, stress_chapter[i], SLS_ULS, min_max, combination_no);

                    hiding = TRUE;
                    for (j = 0; j < block_names_no; j++) {
                        if (strcmp(block_names[j], block_name) == 0) {
                            hiding = FALSE;
                            break;
                        }
                    }

                    if (nom_max) stress_amax = max(fabs(max_stress_max[i]), fabs(min_stress_min[i]));
                    else stress_amax = max(fabs(stress_max[i]), fabs(stress_min[i]));

                    stress_min_RC = BIGNOD;
                    stress_max_RC = -BIGNOD;
                    //stress_min_RC_rho = BIGNOD;
                    //stress_max_RC_rho = -BIGNOD;
                    stress_min_RC_rho_l = BIGNOD;
                    stress_max_RC_rho_l = -BIGNOD;
                    stress_min_RC_rho_c = BIGNOD;
                    stress_max_RC_rho_c = -BIGNOD;
                    stress_min_RC_rho_x = BIGNOD;
                    stress_max_RC_rho_x = -BIGNOD;
                    stress_min_RC_rho_y = BIGNOD;
                    stress_max_RC_rho_y = -BIGNOD;
                    RC_flag_exists = FALSE;

                    if (stress_amax > 0.0) {
                        ptr_block = add_block(blok_origin.x, blok_origin.y, B_GRAPH, block_name, hiding);
                        if (ptr_block == NULL) {
                            fclose(f);
                            ret = ask_question(1, (char *) "", (char *) confirm, (char *) "",
                                               (char *) _CANNOT_CREATE_STRESS_BLOCK_, 12, (char *) "", 11, 1, 62);
                            no_error = FALSE;
                            goto sh_error1;
                        }

                        for (j = 0; j < mesh_element_no; j++) {
                            w.xy[0] = jednostkiObX(mesh_node[mesh_element[j].node1 - 1].x);
                            w.xy[1] = jednostkiObY(mesh_node[mesh_element[j].node1 - 1].y);

                            w.xy[2] = jednostkiObX(mesh_node[mesh_element[j].node2 - 1].x);
                            w.xy[3] = jednostkiObY(mesh_node[mesh_element[j].node2 - 1].y);

                            w.xy[4] = jednostkiObX(mesh_node[mesh_element[j].node3 - 1].x);
                            w.xy[5] = jednostkiObY(mesh_node[mesh_element[j].node3 - 1].y);

                            if (nom_max) //ULS_SLS
                            {
                                BOOL RC_flag = FALSE;
                                w.kolor = Ldsp.kolor;
                                if (sti == 2) //ULS
                                {
                                    //checking cross section
                                    body_prop_no = body_property[mesh_element[j].body_number - 1];
                                    if (body_prop_no > -1)
                                        body_prop = &sh_property[body_prop_no];
                                    else body_prop = NULL;

                                    if (body_prop != NULL) //WARNING, RC_flag should be inherited from shield
                                    {
                                        if (body_prop->RC_flag) {
                                            //calculating reinforcement ratio and normalized compression stress
                                            RC_flag = TRUE; //so there will be just one block for both min and max
                                        }
                                    }
                                }
                                if ((RC_flag == TRUE) && (i == 6))  //6 when σ.eq comes    //(i < 3)
                                {
                                    RC_flag_exists = TRUE;
                                    //changing kolor
                                    w.kolor = 255;  //this is indicator of showing percent for negatives, and stress for positives

                                    double d;  //effective depth
                                    double K;  //non-dimentional moment
                                    double z;  //lever arm
                                    double As, Asc;    //surface of reinforcing bars in tension, reinforcing bars in compression
                                    //double p[3];  //reinforcement ratio in each node
                                    double sigeq[3];  //effective stress in concrete in each node
                                    double pl[3];  //longitudinal reinforcement ratio in each node
                                    double pc[3];  //circumferential reinforcement ratio in each node
                                    double px[3];  //x-axis reinforcement ratio in each node
                                    double py[3];  //y-axis reinforcement ratio in each node
                                    double p_angle[3];  //angle of σ₁ (degrees)
                                    double cb; //concrete cross section b
                                    double cc;
                                    double h; //cross section height
                                    double bb; //width 1 m or 12 inch
                                    double fck, fcd, fyk, fyd;  //in ACI 318   fyd=fyk, fcd=fyd
                                    double n, m;
                                    double E_cm;
                                    double E_c;
                                    double E_s; // = 200;  //GPa in SI
                                    double dk;  //relative neutral axis depth
                                    double x; //neutral axis depth
                                    double I_cr; //cracked moment of inertia
                                    double R;  //moment strength paramete
                                    double fi;
                                    double a; //compression block depth
                                    double nac; //neutral axis
                                    double beta1;
                                    double epsilont; //tensile strain

                                    double sigx[3], sigy[3], tauxy[3];
                                    int code;

                                    if (bi == 0) {
                                        sigx[0] = min_stress[0][perm_s[mesh_element[j].node1 - 1]];
                                        sigx[1] = min_stress[0][perm_s[mesh_element[j].node2 - 1]];
                                        sigx[2] = min_stress[0][perm_s[mesh_element[j].node3 - 1]];

                                        sigy[0] = min_stress[1][perm_s[mesh_element[j].node1 - 1]];
                                        sigy[1] = min_stress[1][perm_s[mesh_element[j].node2 - 1]];
                                        sigy[2] = min_stress[1][perm_s[mesh_element[j].node3 - 1]];

                                        tauxy[0] = min_stress[3][perm_s[mesh_element[j].node1 - 1]];
                                        tauxy[1] = min_stress[3][perm_s[mesh_element[j].node2 - 1]];
                                        tauxy[2] = min_stress[3][perm_s[mesh_element[j].node3 - 1]];

                                        sigeq[0] = min_stress[6][perm_s[mesh_element[j].node1 - 1]];
                                        sigeq[1] = min_stress[6][perm_s[mesh_element[j].node2 - 1]];
                                        sigeq[2] = min_stress[6][perm_s[mesh_element[j].node3 - 1]];
                                    } else {
                                        sigx[0] = max_stress[0][perm_s[mesh_element[j].node1 - 1]];
                                        sigx[1] = max_stress[0][perm_s[mesh_element[j].node2 - 1]];
                                        sigx[2] = max_stress[0][perm_s[mesh_element[j].node3 - 1]];

                                        sigy[0] = max_stress[1][perm_s[mesh_element[j].node1 - 1]];
                                        sigy[1] = max_stress[1][perm_s[mesh_element[j].node2 - 1]];
                                        sigy[2] = max_stress[1][perm_s[mesh_element[j].node3 - 1]];

                                        tauxy[0] = max_stress[3][perm_s[mesh_element[j].node1 - 1]];
                                        tauxy[1] = max_stress[3][perm_s[mesh_element[j].node2 - 1]];
                                        tauxy[2] = max_stress[3][perm_s[mesh_element[j].node3 - 1]];

                                        sigeq[0] = max_stress[6][perm_s[mesh_element[j].node1 - 1]];
                                        sigeq[1] = max_stress[6][perm_s[mesh_element[j].node2 - 1]];
                                        sigeq[2] = max_stress[6][perm_s[mesh_element[j].node3 - 1]];
                                    }

                                    if (ret_standard == 1) //Eurocodes
                                    {
                                        code=0;
                                        if (UNITS != SI)  //impoerial, need to be converted
                                        {
                                            //concrete
                                            if (body_prop->fck > 0.) {
                                                fck = body_prop->fck;
                                                if (body_prop->fcd > 0.) fcd = body_prop->fcd;  //psi
                                                else fcd = body_prop->fck / 1.5;
                                            } else {
                                                if (body_prop->fcd > 0.) {
                                                    fcd = body_prop->fcd;
                                                    fck = fcd * 1.5;
                                                } else {
                                                    fck = 4351.13; // equivalent of 30.0e6 Pa
                                                    fcd = fck / 1.5;
                                                }
                                            }
                                            //steel
                                            E_s = 29000000;  //psi

                                            if (body_prop->fyk > 0.) {
                                                fyk = body_prop->fyk;
                                                if (body_prop->fyd > 0.) fyd = body_prop->fyd;  //psi
                                                else fyd = body_prop->fyk / 1.15;
                                            } else {
                                                if (body_prop->fyd > 0.) {
                                                    fyd = body_prop->fyd; //psi
                                                    fyk = fyd * 1.15;
                                                } else {
                                                    fyk = 58015.1; //equivalent of 400.0e6 Pa
                                                    fyd = fyk / 1.15;
                                                }
                                            }
                                            if (body_prop->c > 0.) cc = body_prop->c;
                                            else cc = 1.18;  //equivalent of 30 mm

                                            h = body_prop->h;
                                            d = h - cc;

                                            //conversion to IS
                                            //converstion from psi to Pa
                                            fck *= 6894.76;
                                            fcd *= 6894.76;
                                            fyk *= 6894.76;
                                            fyd *= 6894.76;
                                            E_s *= 6894.76;

                                            h *= 0.0254;   //inch to m
                                            d *= 0.0254;   //inch to m
                                            cc *= 0.0254;   //inch to m
                                        } else  //SI
                                        {
                                            //concrete
                                            if (body_prop->fck > 0.) {
                                                fck = body_prop->fck;
                                                if (body_prop->fcd > 0.) fcd = body_prop->fcd * 1e6;  //MPa to Pa
                                                else fcd = body_prop->fck * 1e6 / 1.5; //MPa to Pa
                                            } else {
                                                if (body_prop->fcd > 0.) {
                                                    fcd = body_prop->fcd * 1e6;  //MPa to Pa
                                                    fck = fcd * 1.5;
                                                } else {
                                                    fck = 30.0e6;
                                                    fcd = fck / 1.5;
                                                }
                                            }

                                            E_s = 200e9;  //Pa
                                            //steel
                                            if (body_prop->fyk > 0.) {
                                                fyk = body_prop->fyk;
                                                if (body_prop->fyd > 0.) fyd = body_prop->fyd * 1e6;  //MPa to Pa
                                                else fyd = body_prop->fyk * 1e6 / 1.15; //MPa to Pa
                                            } else {
                                                if (body_prop->fyd > 0.) {
                                                    fyd = body_prop->fyd * 1e6; //MPa to Pa
                                                    fyk = fyd * 1.15;
                                                } else {
                                                    fyk = 400.0e6;
                                                    fyd = fyk / 1.15;
                                                }
                                            }
                                            if (body_prop->c > 0.) cc = body_prop->c;
                                            else cc = 0.03;  //30 mm

                                            h = body_prop->h;
                                            d = h - cc;
                                        }

                                        for (int ni = 0; ni < 3; ni++)
                                        {

                                            if (UNITS != SI)  //need to be converted
                                            {
                                                ////M *= 0.112985 * 1000.0 / 25.4;   //lbs-inch to Nm
                                                sigx[ni] *= 6894.76;
                                                sigy[ni] *= 6894.76;
                                                tauxy[ni] *= 6894.76;
                                            }

                                            double lambda = 0.8;  //λ
                                            double eta = 1.0;  //η
                                            double rhot, rhoc, total_rho, sigma_c_max;
                                            int use_min_reinf = 1;
                                            SHIELD_POINT_T result;

                                            //void calculate_shield_reinforcement_final(
                                            //        double sigx, double sigy, double tauxy,   // FEM output (compression = negative)
                                            //        double t, double cc,
                                            //        double fck, double fcd, double fyk, double fyd,
                                            //        int code, SHIELD_POINT_T *result)
                                            int ret_p=calculate_shield_reinforcement_final(sigx[ni], sigy[ni], tauxy[ni], h, cc, fck, fcd, fyk, fyd, code, &result);

                                            if (ret_p == 0) {
                                                pl[ni] = result.rho_longitudinal;
                                                pc[ni] = result.rho_circumferential;
                                                px[ni] = result.rho_x;
                                                py[ni] = result.rho_y;
                                                p_angle[ni] = result.principal_angle_deg;
                                            } else //Negative reinforcement area or anything wrong
                                            {
                                                pl[ni] = 0.0;  //or NaN
                                                pc[ni] = 0.0;  //or NaN
                                                px[ni] = 0.0;  //or NaN
                                                py[ni] = 0.0;  //or NaN
                                                p_angle[ni] = 0.0;  //or NaN
                                            }

                                            //longitudinal
                                            stress_min_RC_rho_l = min(stress_min_RC_rho_l, pl[ni]);
                                            stress_max_RC_rho_l = max(stress_max_RC_rho_l, pl[ni]);

                                            //circumferential
                                            stress_min_RC_rho_c = min(stress_min_RC_rho_c, pc[ni]);
                                            stress_max_RC_rho_c = max(stress_max_RC_rho_c, pc[ni]);

                                            //x-axis
                                            stress_min_RC_rho_x = min(stress_min_RC_rho_x, px[ni]);
                                            stress_max_RC_rho_x = max(stress_max_RC_rho_x, px[ni]);

                                            //y-axis
                                            stress_min_RC_rho_y = min(stress_min_RC_rho_y, py[ni]);
                                            stress_max_RC_rho_y = max(stress_max_RC_rho_y, py[ni]);
                                        }
                                    } else // ASCE & ICC
                                    {
                                        code=1;
                                        if (UNITS != SI)  //imperial, need to be converted
                                        {
                                            if (body_prop->fck > 0.) {
                                                fck = body_prop->fck;
                                                if (body_prop->fcd > 0.) fcd = body_prop->fcd;  //ksi
                                                else fcd = body_prop->fck / 1.5;
                                            } else {
                                                if (body_prop->fcd > 0.) {
                                                    fcd = body_prop->fcd;
                                                    fck = fcd * 1.5;
                                                } else {
                                                    fck = 4351.13;  //psi
                                                    fcd = fck / 1.0;
                                                }
                                            }

                                            E_s = 29.0e6;  //psi

                                            if (body_prop->fyk > 0.) {
                                                fyk = body_prop->fyk;
                                                if (body_prop->fyd > 0.) fcd = body_prop->fyd;  //ksi
                                                else fcd = body_prop->fyk / 1.15;
                                            } else {
                                                if (body_prop->fyd > 0.) {
                                                    fyd = body_prop->fyd;  //ksi
                                                    fyk = fyd * 1.15;
                                                } else {
                                                    fyk = 60000.;  //psi
                                                    fyd = fyk / 1.0;
                                                }
                                            }

                                            bb = 12.0;
                                        } else //SI
                                        {
                                            if (body_prop->fck > 0.) {
                                                fck = body_prop->fck;
                                                if (body_prop->fcd > 0.) fcd = body_prop->fcd * 1e6;  //MPa to Pa;
                                                else fcd = body_prop->fck / 1.5;
                                            } else {
                                                if (body_prop->fcd > 0.) {
                                                    fcd = body_prop->fcd * 1e6;  //MPa to Pa;
                                                    fck = fcd * 1.5;
                                                } else {
                                                    fck = 30.0e6;  //in Pa
                                                    fcd = fck / 1.0;
                                                }
                                            }

                                            E_s = 200e9;  //Pa

                                            if (body_prop->fyk > 0.) {
                                                fyk = body_prop->fyk;
                                                if (body_prop->fyd > 0.) fcd = body_prop->fyd * 1e6;  //MPa to Pa;
                                                else fcd = body_prop->fyk / 1.15;
                                            } else {
                                                if (body_prop->fyd > 0.) {
                                                    fyd = body_prop->fyd * 1e6;  //MPa to Pa;;
                                                    fyk = fyd * 1.15;
                                                } else {
                                                    fyk = 400.0e6; //in Pa
                                                    fyd = fyk / 1.0;
                                                }
                                            }
                                            bb = 1.0;
                                        }

                                        h = body_prop->h;
                                        if (body_prop->c > 0.) cc = body_prop->c;
                                        else cc = 1.0;  //onr inch
                                        fi = 0.9;
                                        d = h - cc;

                                        for (int ni = 0; ni < 3; ni++) {

                                            if (UNITS != SI)  //need to be converted
                                            {
                                                sigx[ni] *= 6894.76;
                                                sigy[ni] *= 6894.76;
                                                tauxy[ni] *= 6894.76;
                                            }


                                            double rhot, rhoc, total_rho, sigma_c_max;
                                            int use_min_reinf = 1;
                                            SHIELD_POINT_T result;

                                            int ret_p=calculate_shield_reinforcement_final(sigx[ni], sigy[ni], tauxy[ni], h, cc, fck, fcd, fyk, fyd, code, &result);

                                            if (ret_p == 0) {
                                                pl[ni] = result.rho_longitudinal;
                                                pc[ni] = result.rho_circumferential;
                                                px[ni] = result.rho_x;
                                                py[ni] = result.rho_y;
                                                p_angle[ni] = result.principal_angle_deg;
                                            } else //Negative reinforcement area or anything wrong
                                            {
                                                pl[ni] = 0.0;  //or NaN
                                                pc[ni] = 0.0;  //or NaN
                                                px[ni] = 0.0;  //or NaN
                                                py[ni] = 0.0;  //or NaN
                                                p_angle[ni] = 0.0;  //or NaN
                                            }

                                            //longitudinal
                                            stress_min_RC_rho_l = min(stress_min_RC_rho_l, pl[ni]);
                                            stress_max_RC_rho_l = max(stress_max_RC_rho_l, pl[ni]);

                                            //circumferential
                                            stress_min_RC_rho_c = min(stress_min_RC_rho_c, pc[ni]);
                                            stress_max_RC_rho_c = max(stress_max_RC_rho_c, pc[ni]);

                                            //x-axis
                                            stress_min_RC_rho_x = min(stress_min_RC_rho_x, px[ni]);
                                            stress_max_RC_rho_x = max(stress_max_RC_rho_x, px[ni]);

                                            //y-axis
                                            stress_min_RC_rho_y = min(stress_min_RC_rho_y, py[ni]);
                                            stress_max_RC_rho_y = max(stress_max_RC_rho_y, py[ni]);
                                        }
                                    }

                                    if (bi == 0)
                                    {
                                        if (perm_s[mesh_element[j].node1 - 1] == -1) gradient3.c1 = getRGB(0, stress_amax);
                                        else gradient3.c1 = getRGB(min_stress[i][perm_s[mesh_element[j].node1 - 1]], stress_amax);
                                        if (perm_s[mesh_element[j].node2 - 1] == -1) gradient3.c2 = getRGB(0, stress_amax);
                                        else gradient3.c2 = getRGB(min_stress[i][perm_s[mesh_element[j].node2 - 1]], stress_amax);
                                        if (perm_s[mesh_element[j].node3 - 1] == -1) gradient3.c3 = getRGB(0, stress_amax);
                                        else gradient3.c3 = getRGB(min_stress[i][perm_s[mesh_element[j].node3 - 1]], stress_amax);

                                        fe_data3.el_number = j + 1;
                                        fe_data3.flag = 3;   //tripple extend

                                        fe_data3.f1 = (float) sigeq[0];
                                        fe_data3.f2 = (float) sigeq[1];
                                        fe_data3.f3 = (float) sigeq[2];

                                        fe_data3_ex.f1 = (float) pl[0];  //negative means %
                                        fe_data3_ex.f2 = (float) pl[1];  // -//-
                                        fe_data3_ex.f3 = (float) pl[2];  // -//-

                                        fe_data3_ex1.f1 = (float) pc[0];  //negative means %
                                        fe_data3_ex1.f2 = (float) pc[1];  // -//-
                                        fe_data3_ex1.f3 = (float) pc[2];  // -//-

                                        fe_data3_ex2.f1 = (float) p_angle[0];
                                        fe_data3_ex2.f2 = (float) p_angle[1];
                                        fe_data3_ex2.f3 = (float) p_angle[2];

                                        fe_data3_ex3.f1 = (float) px[0];  //negative means %
                                        fe_data3_ex3.f2 = (float) px[1];  // -//-
                                        fe_data3_ex3.f3 = (float) px[2];  // -//-

                                        fe_data3_ex4.f1 = (float) py[0];  //negative means %
                                        fe_data3_ex4.f2 = (float) py[1];  // -//-
                                        fe_data3_ex4.f3 = (float) py[2];  // -//-

                                    } else {
                                        if (perm_s[mesh_element[j].node1 - 1] == -1) gradient3.c1 = getRGB(0, stress_amax);
                                        else gradient3.c1 = getRGB(max_stress[i][perm_s[mesh_element[j].node1 - 1]], stress_amax);
                                        if (perm_s[mesh_element[j].node2 - 1] == -1) gradient3.c2 = getRGB(0, stress_amax);
                                        else gradient3.c2 = getRGB(max_stress[i][perm_s[mesh_element[j].node2 - 1]], stress_amax);
                                        if (perm_s[mesh_element[j].node3 - 1] == -1) gradient3.c3 = getRGB(0, stress_amax);
                                        else gradient3.c3 = getRGB(max_stress[i][perm_s[mesh_element[j].node3 - 1]], stress_amax);

                                        fe_data3.el_number = j + 1;
                                        fe_data3.flag = 3;  //tripple extend

                                        fe_data3.f1 = (float) sigeq[0];
                                        fe_data3.f2 = (float) sigeq[1];
                                        fe_data3.f3 = (float) sigeq[2];

                                        fe_data3_ex.f1 = (float) pl[0];  //negative means %
                                        fe_data3_ex.f2 = (float) pl[1];  // -//-
                                        fe_data3_ex.f3 = (float) pl[2];  // -//-

                                        fe_data3_ex1.f1 = (float) pc[0];  //negative means %
                                        fe_data3_ex1.f2 = (float) pc[1];  // -//-
                                        fe_data3_ex1.f3 = (float) pc[2];  // -//-

                                        fe_data3_ex2.f1 = (float) p_angle[0];
                                        fe_data3_ex2.f2 = (float) p_angle[1];
                                        fe_data3_ex2.f3 = (float) p_angle[2];

                                        fe_data3_ex3.f1 = (float) px[0];  //negative means %
                                        fe_data3_ex3.f2 = (float) px[1];  // -//-
                                        fe_data3_ex3.f3 = (float) px[2];  // -//-

                                        fe_data3_ex4.f1 = (float) py[0];  //negative means %
                                        fe_data3_ex4.f2 = (float) py[1];  // -//-
                                        fe_data3_ex4.f3 = (float) py[2];  // -//-

                                    }
                                }
                                else if (bi == 0)
                                {
                                    if (perm_s[mesh_element[j].node1 - 1] == -1) gradient3.c1 = getRGB(0, stress_amax);
                                    else gradient3.c1 = getRGB(min_stress[i][perm_s[mesh_element[j].node1 - 1]], stress_amax);
                                    if (perm_s[mesh_element[j].node2 - 1] == -1) gradient3.c2 = getRGB(0, stress_amax);
                                    else gradient3.c2 = getRGB(min_stress[i][perm_s[mesh_element[j].node2 - 1]], stress_amax);
                                    if (perm_s[mesh_element[j].node3 - 1] == -1) gradient3.c3 = getRGB(0, stress_amax);
                                    else gradient3.c3 = getRGB(min_stress[i][perm_s[mesh_element[j].node3 - 1]], stress_amax);

                                    fe_data3.el_number = j + 1;
                                    fe_data3.flag = 0;
                                    fe_data3.f1 = (float) min_stress[i][perm_s[mesh_element[j].node1 - 1]];
                                    fe_data3.f2 = (float) min_stress[i][perm_s[mesh_element[j].node2 - 1]];
                                    fe_data3.f3 = (float) min_stress[i][perm_s[mesh_element[j].node3 - 1]];

                                } else if (bi == 1) {
                                    if (perm_s[mesh_element[j].node1 - 1] == -1) gradient3.c1 = getRGB(0, stress_amax);
                                    else gradient3.c1 = getRGB(max_stress[i][perm_s[mesh_element[j].node1 - 1]], stress_amax);
                                    if (perm_s[mesh_element[j].node2 - 1] == -1) gradient3.c2 = getRGB(0, stress_amax);
                                    else gradient3.c2 = getRGB(max_stress[i][perm_s[mesh_element[j].node2 - 1]], stress_amax);
                                    if (perm_s[mesh_element[j].node3 - 1] == -1) gradient3.c3 = getRGB(0, stress_amax);
                                    else gradient3.c3 = getRGB(max_stress[i][perm_s[mesh_element[j].node3 - 1]], stress_amax);

                                    fe_data3.el_number = j + 1;
                                    fe_data3.flag = 0;
                                    fe_data3.f1 = (float) max_stress[i][perm_s[mesh_element[j].node1 - 1]];
                                    fe_data3.f2 = (float) max_stress[i][perm_s[mesh_element[j].node2 - 1]];
                                    fe_data3.f3 = (float) max_stress[i][perm_s[mesh_element[j].node3 - 1]];

                                }
                            } else {
                                if (perm_s[mesh_element[j].node1 - 1] == -1) gradient3.c1 = getRGB(0, stress_amax);
                                else gradient3.c1 = getRGB(stress[i][perm_s[mesh_element[j].node1 - 1]], stress_amax);
                                if (perm_s[mesh_element[j].node2 - 1] == -1) gradient3.c2 = getRGB(0, stress_amax);
                                else gradient3.c2 = getRGB(stress[i][perm_s[mesh_element[j].node2 - 1]], stress_amax);
                                if (perm_s[mesh_element[j].node3 - 1] == -1) gradient3.c3 = getRGB(0, stress_amax);
                                else gradient3.c3 = getRGB(stress[i][perm_s[mesh_element[j].node3 - 1]], stress_amax);

                                fe_data3.el_number = j + 1;
                                fe_data3.flag = 0;
                                fe_data3.f1 = (float) stress[i][perm_s[mesh_element[j].node1 - 1]];
                                fe_data3.f2 = (float) stress[i][perm_s[mesh_element[j].node2 - 1]];
                                fe_data3.f3 = (float) stress[i][perm_s[mesh_element[j].node3 - 1]];

                            }

                            w.lp = 6;
                            w.gradient = 1;

                            w.translucent = 1;
                            translucency_ptr = (char *) w.xy;
                            translucency_ptr += (w.lp * sizeof(float));
                            memmove(translucency_ptr, &HalfTranslucency, sizeof(unsigned char));

                            gradient_ptr = translucency_ptr + sizeof(unsigned char);
                            memmove(gradient_ptr, &gradient3, sizeof(GRADIENT3));

                            fe_data_ptr = gradient_ptr + sizeof(GRADIENT3);
                            memmove(fe_data_ptr, &fe_data3, sizeof(FE_DATA3));

                            if (fe_data3.flag == 1)  //extended
                            {
                                fe_data_ex_ptr = fe_data_ptr + sizeof(FE_DATA3);
                                memmove(fe_data_ex_ptr, &fe_data3_ex, sizeof(FE_DATA3_EX));
                                w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT3) + sizeof(FE_DATA3) + sizeof(FE_DATA3_EX);
                            }
                            else if (fe_data3.flag == 2)  //extended twice
                            {
                                fe_data_ex_ptr = fe_data_ptr + sizeof(FE_DATA3);
                                memmove(fe_data_ex_ptr, &fe_data3_ex, sizeof(FE_DATA3_EX));
                                fe_data_ex1_ptr = fe_data_ex_ptr + sizeof(FE_DATA3_EX);
                                memmove(fe_data_ex1_ptr, &fe_data3_ex1, sizeof(FE_DATA3_EX));
                                w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT3) + sizeof(FE_DATA3) + 2*sizeof(FE_DATA3_EX);
                            }
                            else if (fe_data3.flag == 3)  //extended triple and now even five times
                            {
                                fe_data_ex_ptr = fe_data_ptr + sizeof(FE_DATA3);
                                memmove(fe_data_ex_ptr, &fe_data3_ex, sizeof(FE_DATA3_EX));
                                fe_data_ex1_ptr = fe_data_ex_ptr + sizeof(FE_DATA3_EX);
                                memmove(fe_data_ex1_ptr, &fe_data3_ex1, sizeof(FE_DATA3_EX));
                                fe_data_ex2_ptr = fe_data_ex1_ptr + sizeof(FE_DATA3_EX);
                                memmove(fe_data_ex2_ptr, &fe_data3_ex2, sizeof(FE_DATA3_EX));
                                //orthogonal reinforcement
                                fe_data_ex3_ptr = fe_data_ex2_ptr + sizeof(FE_DATA3_EX);
                                memmove(fe_data_ex3_ptr, &fe_data3_ex3, sizeof(FE_DATA3_EX));
                                fe_data_ex4_ptr = fe_data_ex3_ptr + sizeof(FE_DATA3_EX);
                                memmove(fe_data_ex4_ptr, &fe_data3_ex4, sizeof(FE_DATA3_EX));

                                w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT3) + sizeof(FE_DATA3) + 5*sizeof(FE_DATA3_EX);
                            }
                            else
                            {
                                w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT3) + sizeof(FE_DATA3);
                            }
                            adr = dodaj_obiekt((BLOK *) dane, &w);
                        }

                        //colorbar
                        if (stress_amax > 0.0) {
                            w.xy[0] = (float) sh_min_x + 10.f;
                            w.xy[1] = (float) sh_min_y - 10.f;

                            w.xy[2] = w.xy[0] + 50.f;
                            w.xy[3] = w.xy[1];

                            w.xy[4] = w.xy[2];
                            w.xy[5] = w.xy[3] - 5.f;

                            w.xy[6] = w.xy[0];
                            w.xy[7] = w.xy[5];

                            if (nom_max) {
                                if (bi == 0) {
                                    gradient4.c1 = getRGB(min_stress_min[i], stress_amax);
                                    gradient4.c2 = getRGB(max_stress_max[i], stress_amax);
                                    gradient4.c3 = getRGB(max_stress_max[i], stress_amax);
                                    gradient4.c4 = getRGB(min_stress_min[i], stress_amax);

                                    fe_data4.el_number = 0;
                                    fe_data4.flag = 0;
                                    fe_data4.f1 = (float) min_stress_min[i];
                                    fe_data4.f2 = (float) max_stress_max[i];
                                    fe_data4.f3 = (float) max_stress_max[i];
                                    fe_data4.f4 = (float) min_stress_min[i];
                                } else {
                                    gradient4.c1 = getRGB(min_stress_min[i], stress_amax);
                                    gradient4.c2 = getRGB(max_stress_max[i], stress_amax);
                                    gradient4.c3 = getRGB(max_stress_max[i], stress_amax);
                                    gradient4.c4 = getRGB(min_stress_min[i], stress_amax);

                                    fe_data4.el_number = 0;
                                    fe_data4.flag = 0;
                                    fe_data4.f1 = (float) min_stress_min[i];
                                    fe_data4.f2 = (float) max_stress_max[i];
                                    fe_data4.f3 = (float) max_stress_max[i];
                                    fe_data4.f4 = (float) min_stress_min[i];
                                }
                            } else {
                                gradient4.c1 = getRGB(stress_min[i], stress_amax);
                                gradient4.c2 = getRGB(stress_max[i], stress_amax);
                                gradient4.c3 = getRGB(stress_max[i], stress_amax);
                                gradient4.c4 = getRGB(stress_min[i], stress_amax);

                                fe_data4.el_number = 0;
                                fe_data4.flag = 0;
                                fe_data4.f1 = (float) stress_min[i];
                                fe_data4.f2 = (float) stress_max[i];
                                fe_data4.f3 = (float) stress_max[i];
                                fe_data4.f4 = (float) stress_min[i];
                            }

                            w.lp = 8;
                            w.gradient = 1;
                            w.translucent = 1;
                            translucency_ptr = (char *) w.xy;
                            translucency_ptr += (w.lp * sizeof(float));
                            memmove(translucency_ptr, &HalfTranslucency, sizeof(unsigned char));

                            gradient_ptr = translucency_ptr + sizeof(unsigned char);
                            memmove(gradient_ptr, &gradient4, sizeof(GRADIENT4));

                            fe_data_ptr = gradient_ptr + sizeof(GRADIENT4);
                            memmove(fe_data_ptr, &fe_data4, sizeof(FE_DATA4));

                            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT4) + sizeof(FE_DATA4);
                            adr = dodaj_obiekt((BLOK *) dane, &w);

                            //adjust stress to proper units
                            if (UNITS == SI) {
                                if (nom_max) {
                                    if (bi == 0) {
                                        stress_min_ = min(min_stress_min[i], stress_min_RC) * 0.000001; //in MPa
                                        stress_max_ = max(max_stress_max[i], stress_max_RC) * 0.000001; //in MPa
                                    } else {
                                        stress_min_ = min(min_stress_min[i], stress_min_RC) * 0.000001; //in MPa
                                        stress_max_ = max(max_stress_max[i], stress_max_RC) * 0.000001; //in MPa
                                    }
                                } else {
                                    stress_min_ = stress_min[i] * 0.000001; //in MPa
                                    stress_max_ = stress_max[i] * 0.000001; //in MPa
                                }
                                unit = _MPa_;
                            } else //IMP stays as is  in inches
                            {
                                if (nom_max) {
                                    if (bi == 0) {
                                        stress_min_ = min(min_stress_min[i], stress_min_RC) * 0.001; //in ksp
                                        stress_max_ = max(max_stress_max[i], stress_max_RC) * 0.001; //in ksi
                                    } else {
                                        stress_min_ = min(min_stress_min[i], stress_min_RC) * 0.001; //ksi mm
                                        stress_max_ = max(max_stress_max[i], stress_max_RC) * 0.001; //ksi mm
                                    }
                                } else {
                                    stress_min_ = stress_min[i] * 0.001; //in ksi
                                    stress_max_ = stress_max[i] * 0.001; //in ksi
                                }
                                unit = _ksi_;
                            }

                            T.warstwa = Ldsp.warstwa;
                            T.kolor = Ldsp.kolor;
                            T.italics = 0;
                            T.underline = 0;
                            T.wysokosc = (float) (zmwym.wysokosc * 0.5);
                            T.kat = 0.0f;
                            T.ukryty = 0;

                            T.typ = 15;

                            T.x = w.xy[0];
                            T.y = w.xy[1] + 0.5f;
                            T.justowanie = j_do_lewej;
                            sprintf(T.text, "%.4f", stress_min_);

                            T.dl = strlen(T.text);
                            T.n = T18 + T.dl;
                            normalize_txt(&T);
                            adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                            T.x = w.xy[2];
                            T.y = w.xy[3] + 0.5f;
                            T.justowanie = j_do_prawej;
                            sprintf(T.text, "%.4f", stress_max_);

                            T.dl = strlen(T.text);
                            T.n = T18 + T.dl;
                            normalize_txt(&T);
                            adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                            T.x = (w.xy[0] + w.xy[2]) / 2.f;
                            T.y = w.xy[3] + 0.5f;
                            T.justowanie = j_srodkowo;
                            sprintf(T.text, "%s  %s%s %s", Utf8StrMakeUprUtf8Str(SLS_ULS), stress_chapter[i], min_max, unit);

                            T.dl = strlen(T.text);
                            T.n = T18 + T.dl;
                            normalize_txt(&T);
                            adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                            if (RC_flag_exists)
                            {
                                //pl
                                T.x = w.xy[0];
                                T.y = w.xy[5] - T.wysokosc - 0.5f;
                                T.justowanie = j_do_lewej;
                                sprintf(T.text, "%.2f;  %.2f", fabs(stress_min_RC_rho_l) * 100., fabs(stress_min_RC_rho_x) * 100.);

                                T.dl = strlen(T.text);
                                T.n = T18 + T.dl;
                                normalize_txt(&T);
                                adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                                T.x = w.xy[2];
                                T.y = w.xy[5] - T.wysokosc - 0.5f;
                                T.justowanie = j_do_prawej;
                                sprintf(T.text, "%.2f;  %.2f", fabs(stress_max_RC_rho_l) * 100., fabs(stress_max_RC_rho_x) * 100.);

                                T.dl = strlen(T.text);
                                T.n = T18 + T.dl;
                                normalize_txt(&T);
                                adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                                T.x = (w.xy[0] + w.xy[2]) / 2.f;
                                T.y = w.xy[5] + -T.wysokosc - 0.5f;
                                T.justowanie = j_srodkowo;
                                sprintf(T.text, "%s;  %s", "ρl [%]", "ρx [%]");

                                T.dl = strlen(T.text);
                                T.n = T18 + T.dl;
                                normalize_txt(&T);
                                adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                                //pc
                                T.x = w.xy[0];
                                T.y = w.xy[5] - 2.* T.wysokosc - 1.0f;
                                T.justowanie = j_do_lewej;
                                sprintf(T.text, "%.2f;  %.2f", fabs(stress_min_RC_rho_c) * 100., fabs(stress_min_RC_rho_y) * 100.);

                                T.dl = strlen(T.text);
                                T.n = T18 + T.dl;
                                normalize_txt(&T);
                                adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                                T.x = w.xy[2];
                                T.y = w.xy[5] - 2. * T.wysokosc - 1.0f;
                                T.justowanie = j_do_prawej;
                                sprintf(T.text, "%.2f;  %.2f", fabs(stress_max_RC_rho_c) * 100., fabs(stress_max_RC_rho_y) * 100.);

                                T.dl = strlen(T.text);
                                T.n = T18 + T.dl;
                                normalize_txt(&T);
                                adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                                T.x = (w.xy[0] + w.xy[2]) / 2.f;
                                T.y = w.xy[5] - 2. * T.wysokosc - 1.0f;
                                T.justowanie = j_srodkowo;
                                sprintf(T.text, "%s; %s", "ρc [%]", "ρy [%]");

                                T.dl = strlen(T.text);
                                T.n = T18 + T.dl;
                                normalize_txt(&T);
                                adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                            }
                            //changing back
                            T.typ = 0;
                        }

                        /////REACTIONS

                        //reaction based on stress variant, but just for SLS and ULS
                        //if ((sti!=2) || (ULSLC_flag[2]!=0)) // if ULSLC, flag must exist
                        if (sti < 2)
                        {
                            ////all elements done
                            ////we got:
                            //// stress[i][perm_s[mesh_element[j].node1 - 1]];
                            //// stress[i][perm_s[mesh_element[j].node2 - 1]];
                            //// stress[i][perm_s[mesh_element[j].node3 - 1]];
                            //// settling now Qx and Qy for element j for limit state i and min-max bi
                            //// checking if element number j belongs to mesh_boundaries[mesh_boundaries_no]
                            int last_node = 0;

                            if ((stress[i] != NULL) && (i == 3)) //stress_xy
                            {
                                for (j = 0; j < mesh_boundaries_no; j++)
                                {
                                    if ((mesh_boundary[j].restraint > 5)  || (mesh_boundary[j].restraint == 4)) //4 rolling, 6 simple supported, 7 fixed, 4 rolled
                                    {
                                        if (mesh_boundary[j].node1 > 0)  //node 1 exists
                                        {
                                            int node1 = 0, node2 = 0;
                                            int jj;

                                            //calculating reactions for node1

                                            double supp_x, supp_y;      // Supported node coordinates (m)
                                            double H;

                                            supp_x = mesh_node[mesh_boundary[j].node1 - 1].x;
                                            supp_y = mesh_node[mesh_boundary[j].node1 - 1].y;

                                            for (jj = 0; jj < mesh_element_no; jj++)
                                            {
                                                if (mesh_element[jj].node1 == mesh_boundary[j].node1)
                                                {
                                                    supp1_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                    supp1_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                    supp1_sigxy_max[j] = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                    H=mesh_element[jj].h;
                                                    break;
                                                }
                                                else if (mesh_element[jj].node2 == mesh_boundary[j].node1)
                                                {
                                                    supp1_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                    supp1_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                    supp1_sigxy_max[j] = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                    H=mesh_element[jj].h;
                                                    break;
                                                }
                                                else if (mesh_element[jj].node3 == mesh_boundary[j].node1)
                                                {
                                                    supp1_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                    supp1_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                    supp1_sigxy_max[j] = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                    H=mesh_element[jj].h;
                                                    break;
                                                }
                                            }

                                            //// REACTIONS
                                            compute_membrane_support_reaction(H, supp1_sigx_max[j],supp1_sigy_max[j],supp1_sigxy_max[j],mesh_boundary[j].koc,mesh_boundary[j].kos,mesh_boundary[j].l1,mesh_boundary[j].restraint,&mesh_boundary[j].state1[sti].Rx,&mesh_boundary[j].state1[sti].Ry,&mesh_boundary[j].state1[sti].Rn,&mesh_boundary[j].state1[sti].Rt);
#ifndef NDEBUG
                                            //if (last_node != mesh_boundary[j].node1)
                                            //{
                                            printf("node=%d\n", mesh_boundary[j].node1);
                                            printf("H=%f\n", H);
                                            printf("supp.x=%f  supp.y=%f  supp1_sigx=%f  supp1_sigy=%f  supp1_sigxy=%f\n",
                                                   supp_x, supp_y, supp1_sigx_max[j], supp1_sigy_max[j], supp1_sigxy_max[j]);

                                            printf("l1=%f\n", mesh_boundary[j].l1);

                                            printf("Results: Rx=%f Ry=%f Rn=%f Rt=%f\n\n", mesh_boundary[j].state1[sti].Rx, mesh_boundary[j].state1[sti].Ry, mesh_boundary[j].state1[sti].Rn, mesh_boundary[j].state1[sti].Rt);
                                            //}
                                            //last_node=mesh_boundary[j].node1;
#endif
                                        }

                                        if (mesh_boundary[j].node2 > 0)  //node 2 exists
                                        {
                                            int node1 = 0, node2 = 0;
                                            int jj;

                                            //calculating reactions for node2

                                            double supp_x, supp_y;      // Supported node coordinates (m)
                                            double H;

                                            supp_x = mesh_node[mesh_boundary[j].node2 - 1].x;
                                            supp_y = mesh_node[mesh_boundary[j].node2 - 1].y;

                                            for (jj = 0; jj < mesh_element_no; jj++) {
                                                if (mesh_element[jj].node1 == mesh_boundary[j].node2)
                                                {
                                                    supp2_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                    supp2_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                    supp2_sigxy_max[j] = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                    H=mesh_element[jj].h;
                                                    break;
                                                }
                                                else if (mesh_element[jj].node2 == mesh_boundary[j].node2)
                                                {
                                                    supp2_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                    supp2_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                    supp2_sigxy_max[j] = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                    H=mesh_element[jj].h;
                                                    break;
                                                }
                                                else if (mesh_element[jj].node3 == mesh_boundary[j].node2)
                                                {
                                                    supp2_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                    supp2_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                    supp2_sigxy_max[j] = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                    H=mesh_element[jj].h;
                                                    break;
                                                }
                                            }

                                            //// REACTIONS
                                            compute_membrane_support_reaction(H, supp2_sigx_max[j],supp2_sigy_max[j],supp2_sigxy_max[j],mesh_boundary[j].koc,mesh_boundary[j].kos,mesh_boundary[j].l2,mesh_boundary[j].restraint,&mesh_boundary[j].state2[sti].Rx,&mesh_boundary[j].state2[sti].Ry,&mesh_boundary[j].state2[sti].Rn,&mesh_boundary[j].state2[sti].Rt);
#ifndef NDEBUG
                                            //if (last_node != mesh_boundary[j].node1)
                                            //{
                                            printf("node=%d\n", mesh_boundary[j].node2);
                                            printf("H=%f\n", H);
                                            printf("supp.x=%f  supp.y=%f  supp2_sigx=%f  supp2_sigy=%f  supp2_sigxy=%f\n",
                                                   supp_x, supp_y, supp2_sigx_max[j], supp2_sigy_max[j], supp2_sigxy_max[j]);

                                            printf("l2=%f\n", mesh_boundary[j].l2);

                                            printf("Results: Rx=%f Ry=%f Rn=%f Rt=%f\n\n", mesh_boundary[j].state2[sti].Rx, mesh_boundary[j].state2[sti].Ry, mesh_boundary[j].state2[sti].Rn, mesh_boundary[j].state2[sti].Rt);
                                            //}
                                            //last_node=mesh_boundary[j].node1;
#endif
                                        }

                                    }
                                }

                            }

                        }
                        ////////////REACTIONS
                    }
                }
            }
        }

        my_sleep(10);

        ////creating epsilon blocks

        Ldsp.kolor = static_colors.node_element_color;
        Ldsp.typ = 64;

        sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "epsilon", SLS_ULS, combination_no);

        memcpy(desired_layer_upper, desired_layer, strlen(desired_layer) + 1);
        utf8Upper(desired_layer_upper);

        for (ii = 0; ii < No_Layers; ii++) {
            memcpy(layer_name_upper, Layers[ii].name, strlen(Layers[ii].name) + 1);
            utf8Upper(layer_name_upper);

            if (strcmp(layer_name_upper, desired_layer_upper) == 0)
                break;
        }
        if (ii < No_Layers) {
            desired_layer_no = ii;
            if (block_names)
                get_blocks_setup(desired_layer_no, block_names, &block_names_no, MAX_L_BLOCKS,MAX_L_BLOCKS_LEN);  //to remember only visible blocks
            delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
        } else {
            if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

                ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_,
                                   12, (char *) "", 11, 1, 62);
                no_error = FALSE;
                goto sh_error1;
            }
            if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

                ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_,
                                   12, (char *) "", 11, 1, 62);
                no_error = FALSE;
                goto sh_error1;
            }
            No_Layers++;
            desired_layer_no = No_Layers - 1;
            Layers[No_Layers - 1].on = 1;
            Layers[No_Layers - 1].edit = 1;
            Layers[No_Layers - 1].point = 1;
            strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
            Layers[No_Layers - 1].color = Ldsp.kolor;
        }

        if ((sti == 2) && (ULSLC_flag[2] == 0)) // non existing ULSLC
            goto end_block;

        if (perm_e != NULL) {
            for (i = 0; i < EPSILON_NUMBER; i++) {
                if (epsilon_ == 0)
                    break;

                Ldsp.warstwa = desired_layer_no;
                w.warstwa = Ldsp.warstwa;
                w.kolor = Ldsp.kolor;
                w.blok = 1;
                //from meters to drawing units
                blok_origin.x = jednostkiObX(mesh_node[0].x);
                blok_origin.y = jednostkiObY(mesh_node[0].y);

                if (nom_max) {
                    bim = 2;
                    min_max = _min_;
                } else {
                    bim = 1;
                    min_max = _nope_;
                }

                for (bi = 0; bi < bim; bi++) {
                    if (bi > 0) min_max = _max_;

                    if (nom_max) {
                        //checking bipolarity
                        if (bi == 0)  //potentially minimal
                        {
                            if ((max_epsilon_max[i] > 0.) && (min_epsilon_min[i] >= 0.))
                                continue;  //we do not create zero unipolar graph
                        }
                        if (bi == 1)  //potentially maximal
                        {
                            if ((max_epsilon_max[i] <= 0.) && (min_epsilon_min[i] < 0))
                                continue; //we do not create zero unipolar graph
                        }
                    }

                    sprintf(block_name, "%s$%s_%s%s_%d", ptr_id_short, epsilon_chapter[i], SLS_ULS, min_max, combination_no);

                    hiding = TRUE;
                    for (j = 0; j < block_names_no; j++) {
                        if (strcmp(block_names[j], block_name) == 0) {
                            hiding = FALSE;
                            break;
                        }
                    }
                    if (nom_max) epsilon_amax = max(fabs(max_epsilon_max[i]), fabs(min_epsilon_min[i]));
                    else epsilon_amax = max(fabs(epsilon_max[i]), fabs(epsilon_min[i]));

                    if (epsilon_amax > 0.0) {
                        ptr_block = add_block(blok_origin.x, blok_origin.y, B_GRAPH, block_name, hiding);
                        if (ptr_block == NULL) {
                            fclose(f);
                            ret = ask_question(1, (char *) "", (char *) confirm, (char *) "",
                                               (char *) _CANNOT_CREATE_STRAIN_BLOCK_, 12, (char *) "", 11, 1, 62);
                            no_error = FALSE;
                            goto sh_error1;
                        }


                        for (j = 0; j < mesh_element_no; j++) {
                            w.xy[0] = jednostkiObX(mesh_node[mesh_element[j].node1 - 1].x);
                            w.xy[1] = jednostkiObY(mesh_node[mesh_element[j].node1 - 1].y);

                            w.xy[2] = jednostkiObX(mesh_node[mesh_element[j].node2 - 1].x);
                            w.xy[3] = jednostkiObY(mesh_node[mesh_element[j].node2 - 1].y);

                            w.xy[4] = jednostkiObX(mesh_node[mesh_element[j].node3 - 1].x);
                            w.xy[5] = jednostkiObY(mesh_node[mesh_element[j].node3 - 1].y);

                            if (nom_max) {
                                if (bi == 0) {
                                    if (perm_e[mesh_element[j].node1 - 1] == -1) gradient3.c1 = getRGB(0, epsilon_amax);
                                    else gradient3.c1 = getRGB(min_epsilon[i][perm_e[mesh_element[j].node1 - 1]], epsilon_amax);
                                    if (perm_e[mesh_element[j].node2 - 1] == -1) gradient3.c2 = getRGB(0, epsilon_amax);
                                    else gradient3.c2 = getRGB(min_epsilon[i][perm_e[mesh_element[j].node2 - 1]], epsilon_amax);
                                    if (perm_e[mesh_element[j].node3 - 1] == -1) gradient3.c3 = getRGB(0, epsilon_amax);
                                    else gradient3.c3 = getRGB(min_epsilon[i][perm_e[mesh_element[j].node3 - 1]], epsilon_amax);

                                    fe_data3.el_number = j + 1;
                                    fe_data3.flag = 0;
                                    fe_data3.f1 = (float) min_epsilon[i][perm_e[mesh_element[j].node1 - 1]];
                                    fe_data3.f2 = (float) min_epsilon[i][perm_e[mesh_element[j].node2 - 1]];
                                    fe_data3.f3 = (float) min_epsilon[i][perm_e[mesh_element[j].node3 - 1]];

                                } else {
                                    if (perm_e[mesh_element[j].node1 - 1] == -1) gradient3.c1 = getRGB(0, epsilon_amax);
                                    else gradient3.c1 = getRGB(max_epsilon[i][perm_e[mesh_element[j].node1 - 1]], epsilon_amax);
                                    if (perm_e[mesh_element[j].node2 - 1] == -1) gradient3.c2 = getRGB(0, epsilon_amax);
                                    else gradient3.c2 = getRGB(max_epsilon[i][perm_e[mesh_element[j].node2 - 1]], epsilon_amax);
                                    if (perm_e[mesh_element[j].node3 - 1] == -1) gradient3.c3 = getRGB(0, epsilon_amax);
                                    else gradient3.c3 = getRGB(max_epsilon[i][perm_e[mesh_element[j].node3 - 1]], epsilon_amax);

                                    fe_data3.el_number = j + 1;
                                    fe_data3.flag = 0;
                                    fe_data3.f1 = (float) max_epsilon[i][perm_e[mesh_element[j].node1 - 1]];
                                    fe_data3.f2 = (float) max_epsilon[i][perm_e[mesh_element[j].node2 - 1]];
                                    fe_data3.f3 = (float) max_epsilon[i][perm_e[mesh_element[j].node3 - 1]];

                                }
                            } else {
                                if (perm_e[mesh_element[j].node1 - 1] == -1) gradient3.c1 = getRGB(0, epsilon_amax);
                                else gradient3.c1 = getRGB(epsilon[i][perm_e[mesh_element[j].node1 - 1]], epsilon_amax);
                                if (perm_e[mesh_element[j].node2 - 1] == -1) gradient3.c2 = getRGB(0, epsilon_amax);
                                else gradient3.c2 = getRGB(epsilon[i][perm_e[mesh_element[j].node2 - 1]], epsilon_amax);
                                if (perm_e[mesh_element[j].node3 - 1] == -1) gradient3.c3 = getRGB(0, epsilon_amax);
                                else gradient3.c3 = getRGB(epsilon[i][perm_e[mesh_element[j].node3 - 1]], epsilon_amax);

                                fe_data3.el_number = j + 1;
                                fe_data3.flag = 0;
                                fe_data3.f1 = (float) epsilon[i][perm_e[mesh_element[j].node1 - 1]];
                                fe_data3.f2 = (float) epsilon[i][perm_e[mesh_element[j].node2 - 1]];
                                fe_data3.f3 = (float) epsilon[i][perm_e[mesh_element[j].node3 - 1]];

                            }

                            w.lp = 6;
                            w.gradient = 1;

                            w.translucent = 1;
                            translucency_ptr = (char *) w.xy;
                            translucency_ptr += (w.lp * sizeof(float));
                            memmove(translucency_ptr, &HalfTranslucency, sizeof(unsigned char));

                            gradient_ptr = translucency_ptr + sizeof(unsigned char);
                            memmove(gradient_ptr, &gradient3, sizeof(GRADIENT3));

                            fe_data_ptr = gradient_ptr + sizeof(GRADIENT3);
                            memmove(fe_data_ptr, &fe_data3, sizeof(FE_DATA3));

                            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT3) + sizeof(FE_DATA3);
                            adr = dodaj_obiekt((BLOK *) dane, &w);

                        }

                        //colorbar
                        if (epsilon_amax > 0.0) {
                            w.xy[0] = (float) sh_min_x + 10.f;
                            w.xy[1] = (float) sh_min_y - 10.f;

                            w.xy[2] = w.xy[0] + 50.f;
                            w.xy[3] = w.xy[1];

                            w.xy[4] = w.xy[2];
                            w.xy[5] = w.xy[3] - 5.f;

                            w.xy[6] = w.xy[0];
                            w.xy[7] = w.xy[5];

                            if (nom_max) {
                                if (bi == 0) {
                                    gradient4.c1 = getRGB(min_epsilon_min[i], epsilon_amax);
                                    gradient4.c2 = getRGB(max_epsilon_max[i], epsilon_amax);
                                    gradient4.c3 = getRGB(max_epsilon_max[i], epsilon_amax);
                                    gradient4.c4 = getRGB(min_epsilon_min[i], epsilon_amax);

                                    fe_data4.el_number = 0;
                                    fe_data4.flag = 0;
                                    fe_data4.f1 = (float) min_epsilon_min[i];
                                    fe_data4.f2 = (float) max_epsilon_max[i];
                                    fe_data4.f3 = (float) max_epsilon_max[i];
                                    fe_data4.f4 = (float) min_epsilon_min[i];
                                } else {
                                    gradient4.c1 = getRGB(min_epsilon_min[i], epsilon_amax);
                                    gradient4.c2 = getRGB(max_epsilon_max[i], epsilon_amax);
                                    gradient4.c3 = getRGB(max_epsilon_max[i], epsilon_amax);
                                    gradient4.c4 = getRGB(min_epsilon_min[i], epsilon_amax);

                                    fe_data4.el_number = 0;
                                    fe_data4.flag = 0;
                                    fe_data4.f1 = (float) min_epsilon_min[i];
                                    fe_data4.f2 = (float) max_epsilon_max[i];
                                    fe_data4.f3 = (float) max_epsilon_max[i];
                                    fe_data4.f4 = (float) min_epsilon_min[i];
                                }
                            } else {
                                gradient4.c1 = getRGB(epsilon_min[i], epsilon_amax);
                                gradient4.c2 = getRGB(epsilon_max[i], epsilon_amax);
                                gradient4.c3 = getRGB(epsilon_max[i], epsilon_amax);
                                gradient4.c4 = getRGB(epsilon_min[i], epsilon_amax);

                                fe_data4.el_number = 0;
                                fe_data4.flag = 0;
                                fe_data4.f1 = (float) epsilon_min[i];
                                fe_data4.f2 = (float) epsilon_max[i];
                                fe_data4.f3 = (float) epsilon_max[i];
                                fe_data4.f4 = (float) epsilon_min[i];
                            }

                            w.lp = 8;
                            w.gradient = 1;
                            w.translucent = 1;
                            translucency_ptr = (char *) w.xy;
                            translucency_ptr += (w.lp * sizeof(float));
                            memmove(translucency_ptr, &HalfTranslucency, sizeof(unsigned char));

                            gradient_ptr = translucency_ptr + sizeof(unsigned char);
                            memmove(gradient_ptr, &gradient4, sizeof(GRADIENT4));

                            fe_data_ptr = gradient_ptr + sizeof(GRADIENT4);
                            memmove(fe_data_ptr, &fe_data4, sizeof(FE_DATA4));

                            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT4) + sizeof(FE_DATA4);
                            adr = dodaj_obiekt((BLOK *) dane, &w);

                            T.warstwa = Ldsp.warstwa;
                            T.kolor = Ldsp.kolor;
                            T.italics = 0;
                            T.underline = 0;
                            T.wysokosc = (float) (zmwym.wysokosc * 0.5);
                            T.kat = 0.0f;
                            T.ukryty = 0;

                            T.typ = 15;

                            T.x = w.xy[0];
                            T.y = w.xy[1] + 0.5f;
                            T.justowanie = j_do_lewej;
                            sprintf(T.text, "%.5f", epsilon_min[i]);

                            T.dl = strlen(T.text);
                            T.n = T18 + T.dl;
                            normalize_txt(&T);
                            adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                            T.x = w.xy[2];
                            T.y = w.xy[3] + 0.5f;
                            T.justowanie = j_do_prawej;
                            sprintf(T.text, "%.4f", epsilon_max[i]);

                            T.dl = strlen(T.text);
                            T.n = T18 + T.dl;
                            normalize_txt(&T);
                            adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                            T.x = (w.xy[0] + w.xy[2]) / 2.f;
                            T.y = w.xy[3] + 0.5f;
                            T.justowanie = j_srodkowo;
                            sprintf(T.text, "%s  %s%s", Utf8StrMakeUprUtf8Str(SLS_ULS), epsilon_chapter[i], min_max);

                            T.dl = strlen(T.text);
                            T.n = T18 + T.dl;
                            normalize_txt(&T);
                            adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                            //changing back
                            T.typ = 0;
                        }
                    }
                }
            }
        }

        end_block:

        sh_error2:

        if (sti == 1)  //ULS
        {
            //stress backup
            for (int is = 0; is < STRESS_NUMBER; is++) {
                min_stress_min_bak[is] = stress_min[is];
                max_stress_max_bak[is] = stress_max[is];
                min_stress_bak[is] = (double *) malloc(perm_s_index_no * sizeof(double) + M100);
                max_stress_bak[is] = (double *) malloc(perm_s_index_no * sizeof(double) + M100);
                memmove(min_stress_bak[is], stress[is], perm_s_index_no * sizeof(double));
                memmove(max_stress_bak[is], stress[is], perm_s_index_no * sizeof(double));
            }
            perm_s_bak = (int *) malloc(perm_s_node_no * sizeof(int) + M100);
            memmove(perm_s_bak, perm_s, perm_s_node_no * sizeof(int));
        }

        if (geo_line) free(geo_line);
        if (perm_d_b == 1) {
            if (perm_d) free(perm_d);
        }
        if (perm_s_b == 1) {
            if (perm_s) free(perm_s);
        }
        if (perm_e_b == 1) {
            if (perm_e) free(perm_e);
        }

        for (i = 0; i < DEFLECTION_NUMBER; i++) {
            if (deflection[i] != NULL) {
                free(deflection[i]);
                deflection[i] = NULL;
            }
        }
        for (i = 0; i < DEFLECTION_NUMBER; i++) {
            if (min_deflection[i] != NULL) {
                free(min_deflection[i]);
                min_deflection[i] = NULL;
            }
        }
        for (i = 0; i < DEFLECTION_NUMBER; i++) {
            if (max_deflection[i] != NULL) {
                free(max_deflection[i]);
                max_deflection[i] = NULL;
            }
        }

        for (i = 0; i < STRESS_NUMBER; i++) {
            if (stress[i] != NULL) {
                free(stress[i]);
                stress[i] = NULL;
            }
        }
        for (i = 0; i < STRESS_NUMBER; i++) {
            if (ULSLC_flag[sti]) {
                if (min_stress[i] != NULL) {
                    free(min_stress[i]);
                    min_stress[i] = NULL;
                }
            }
        }

        for (i = 0; i < STRESS_NUMBER; i++) {
            if (ULSLC_flag[sti]) {
                if (max_stress[i] != NULL) {
                    free(max_stress[i]);
                    max_stress[i] = NULL;
                }
            }
        }
        for (i = 0; i < EPSILON_NUMBER; i++) {
            if (epsilon[i] != NULL) {
                free(epsilon[i]);
                epsilon[i] = NULL;
            }
        }
        for (i = 0; i < EPSILON_NUMBER; i++) {
            if (min_epsilon[i] != NULL) {
                free(min_epsilon[i]);
                min_epsilon[i] = NULL;
            }
        }
        for (i = 0; i < EPSILON_NUMBER; i++) {
            if (max_epsilon[i] != NULL) {
                free(max_epsilon[i]);
                max_epsilon[i] = NULL;
            }
        }

        //REACTIONS just for SLS and ULS
        if (sti < 2)
        {
            int last_node = 0;

            last_node = 0;

            Rxmin = 0.0;
            Rxmax = 0.0;
            Rymin = 0.0;
            Rymax = 0.0;

            Rxabsmin = 0.0;
            Rxabsmax = 0.0;
            Ryabsmin = 0.0;
            Ryabsmax = 0.0;

            Rnmin = 0.0;
            Rnmax = 0.0;
            Rtmin = 0.0;
            Rtmax = 0.0;

            Rxmin_back = 0.0;
            Rxmax_back = 0.0;
            Rymin_back = 0.0;
            Rymax_back = 0.0;

            Rxabsmin_back = 0.0;
            Rxabsmax_back = 0.0;
            Ryabsmin_back = 0.0;
            Ryabsmax_back = 0.0;

            Rnmin_back = 0.0;
            Rnmax_back = 0.0;
            Rtmin_back = 0.0;
            Rtmax_back = 0.0;
#ifndef NDEBUG
            printf("\nState %s\n", SLS_ULS);
#endif

            for (i = 0; i < mesh_boundaries_no; i++) {
                if ((mesh_boundary[i].restraint > 5) || (mesh_boundary[i].restraint == 4))
                {
                    if (mesh_boundary[i].node1 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rx=%f Ry=%f Rn=%f Rt=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti].Rx, mesh_boundary[i].state1[sti].Ry, mesh_boundary[i].state1[sti].Rn, mesh_boundary[i].state1[sti].Rt);
#endif
                        Rxmax += nofabs(mesh_boundary[i].state1[sti].Rx);
                        Rymax += nofabs(mesh_boundary[i].state1[sti].Ry);
                        Rxabsmax += fabs(mesh_boundary[i].state1[sti].Rx);
                        Ryabsmax += fabs(mesh_boundary[i].state1[sti].Ry);
                        Rnmax += (nofabs(mesh_boundary[i].state1[sti].Rn)*mesh_boundary[i].l1);
                        Rtmax += (nofabs(mesh_boundary[i].state1[sti].Rt)*mesh_boundary[i].l1);
                    }
                    last_node = mesh_boundary[i].node1;
                    if (mesh_boundary[i].node2 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rx=%f Ry=%f  Rn=%f  Rt=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti].Rx , mesh_boundary[i].state2[sti].Ry, mesh_boundary[i].state2[sti].Rn, mesh_boundary[i].state2[sti].Rt);
#endif
                        Rxmax += nofabs(mesh_boundary[i].state2[sti].Rx);
                        Rymax += nofabs(mesh_boundary[i].state2[sti].Ry);
                        Rxabsmax += fabs(mesh_boundary[i].state2[sti].Rx);
                        Ryabsmax += fabs(mesh_boundary[i].state2[sti].Ry);
                        Rnmax += (nofabs(mesh_boundary[i].state2[sti].Rn)*mesh_boundary[i].l2);
                        Rtmax += (nofabs(mesh_boundary[i].state2[sti].Rt)*mesh_boundary[i].l2);
                    }
                    last_node = mesh_boundary[i].node2;
                }
            }
#ifndef NDEBUG
            printf("SUM:  Rx_total=%f  Ry_total=%f  Rn_total=%f  Rt_total=%f\n", Rxmax, Rymax, Rnmax, Rtmax);
#endif
            Rxmin_back = Rxmin;
            Rxmax_back = Rxmax;
            Rymin_back = Rymin;
            Rymax_back = Rymax;

            Rxabsmin_back = Rxabsmin;
            Rxabsmax_back = Rxabsmax;
            Ryabsmin_back = Ryabsmin;
            Ryabsmax_back = Ryabsmax;

            Rnmin_back = Rnmin;
            Rnmax_back = Rnmax;
            Rtmin_back = Rtmin;
            Rtmax_back = Rtmax;

#ifndef NDEBUG
            printf("\nTotal load in state %s: x %f  y %f\n", SLS_ULS, total_load_x, total_load_y);
#endif
            // Residuals (the error we must eliminate)
            double residual_x = total_load_x - Rxmax_back;
            double residual_y = total_load_y - Rymax_back;

            // Case 1: There is a non-zero applied load in that direction → classic scaling
            if (fabs(total_load_x) > 1e-6) {
                load_ratio_x = total_load_x / Rxmax_back;
            } else {
                load_ratio_x = 1.0;   // keep internal balance, only add correction below
            }
            if (fabs(total_load_y) > 1e-6) {
                load_ratio_y = total_load_y / Rymax_back;
            } else {
                load_ratio_y = 1.0;
            }

            Rxmin = 0.0;
            Rxmax = 0.0;
            Rymin = 0.0;
            Rymax = 0.0;
            Rxabsmin = 0.0;
            Rxabsmax = 0.0;
            Ryabsmin = 0.0;
            Ryabsmax = 0.0;
            Rnmin = 0.0;
            Rnmax = 0.0;
            Rtmin = 0.0;
            Rtmax = 0.0;
#ifndef NDEBUG
            printf("\nState %s with correction ratio x %f  y %f\n", SLS_ULS, load_ratio_x, load_ratio_y);
#endif
            for (i = 0; i < mesh_boundaries_no; i++) {
                if ((mesh_boundary[i].restraint > 5) || (mesh_boundary[i].restraint == 4))
                {
                    double nx = -mesh_boundary[i].kos; //-sin_theta;
                    double ny = mesh_boundary[i].koc; //+cos_theta;
                    double tx = mesh_boundary[i].koc; // cos_theta;
                    double ty = mesh_boundary[i].kos; // sin_theta;

                    //double load_correction(double R_raw, double sum_R_raw, double load_ratio, double residual)
                    mesh_boundary[i].state1[sti].Rx = load_correction(mesh_boundary[i].state1[sti].Rx, Rxmax_back, total_load_x, Rxabsmax_back);
                    mesh_boundary[i].state1[sti].Ry = load_correction(mesh_boundary[i].state1[sti].Ry, Rymax_back, total_load_y, Ryabsmax_back);

                    // === FINAL: Project back to local Rn, Rt ===
                    //Rn_final[i] = Rx_final * nx + Ry_final * ny;
                    //Rt_final[i] = Rx_final * tx + Ry_final * ty;
                    mesh_boundary[i].state1[sti].Rn = (mesh_boundary[i].state1[sti].Rx * nx + mesh_boundary[i].state1[sti].Ry * ny)/mesh_boundary[i].l1;
                    mesh_boundary[i].state1[sti].Rt = (mesh_boundary[i].state1[sti].Rx * tx + mesh_boundary[i].state1[sti].Ry * ty)/mesh_boundary[i].l1;

                    mesh_boundary[i].state2[sti].Rx = load_correction(mesh_boundary[i].state2[sti].Rx, Rxmax_back, total_load_x, Rxabsmax_back);
                    mesh_boundary[i].state2[sti].Ry = load_correction(mesh_boundary[i].state2[sti].Ry, Rymax_back, total_load_y, Ryabsmax_back);

                    mesh_boundary[i].state2[sti].Rn = (mesh_boundary[i].state2[sti].Rx * nx + mesh_boundary[i].state2[sti].Ry * ny)/mesh_boundary[i].l2;
                    mesh_boundary[i].state2[sti].Rt = (mesh_boundary[i].state2[sti].Rx * tx + mesh_boundary[i].state2[sti].Ry * ty)/mesh_boundary[i].l2;

                    if (mesh_boundary[i].node1 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rx=%f Ry=%f Rn=%f Rt=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti].Rx, mesh_boundary[i].state1[sti].Ry, mesh_boundary[i].state1[sti].Rn, mesh_boundary[i].state1[sti].Rt);
#endif
                        Rxmax += (nofabs(mesh_boundary[i].state1[sti].Rx));
                        Rymax += (nofabs(mesh_boundary[i].state1[sti].Ry));
                        Rnmax += (nofabs(mesh_boundary[i].state1[sti].Rn)*mesh_boundary[i].l1);
                        Rtmax += (nofabs(mesh_boundary[i].state1[sti].Rt)*mesh_boundary[i].l1);
                    }
                    last_node = mesh_boundary[i].node1;
                    if (mesh_boundary[i].node2 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rx=%f  Ry=%f  Rn=%f  Rt=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti].Rx, mesh_boundary[i].state2[sti].Ry, mesh_boundary[i].state2[sti].Rn, mesh_boundary[i].state2[sti].Rt);
#endif
                        Rxmax += (nofabs(mesh_boundary[i].state2[sti].Rx));
                        Rymax += (nofabs(mesh_boundary[i].state2[sti].Ry));
                        Rnmax += (nofabs(mesh_boundary[i].state2[sti].Rn)*mesh_boundary[i].l2);
                        Rtmax += (nofabs(mesh_boundary[i].state2[sti].Rt)*mesh_boundary[i].l2);
                    }
                    last_node = mesh_boundary[i].node2;
                }
            }
#ifndef NDEBUG
            printf("SUM:  Rx_total=%f  Ry_total=%f  Rn_total=%f  Rt_total=%f\n", Rxmax, Rymax, Rnmax, Rtmax);
#endif
        }
        else if ((sti!=2) || (ULSLC_flag[2]!=0)) // if ULSLC, flag must exist
        {
            int last_node = 0;

            last_node = 0;
            Rxmin = 0.0;
            Rymin = 0.0;
            Rnmin = 0.0;
            Rtmin = 0.0;
#ifndef NDEBUG
            printf("\nState %s\n", SLS_ULS);
            printf("\nMIN\n");
#endif

            for (i = 0; i < mesh_boundaries_no; i++) {
                if ((mesh_boundary[i].restraint > 5) || (mesh_boundary[i].restraint == 4))
                {
                    if (mesh_boundary[i].node1 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rx=%f Ry=%f  Rn=%f  Rt=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti].Rxmin, mesh_boundary[i].state1[sti].Rymin, mesh_boundary[i].state1[sti].Rnmin, mesh_boundary[i].state1[sti].Rtmin);
#endif
                        Rxmin += nofabs(mesh_boundary[i].state1[sti].Rxmin);
                        Rymin += nofabs(mesh_boundary[i].state1[sti].Rymin);
                        Rnmin += (nofabs(mesh_boundary[i].state1[sti].Rnmin)*mesh_boundary[i].l1);
                        Rtmin += (nofabs(mesh_boundary[i].state1[sti].Rtmin)*mesh_boundary[i].l1);
                    }
                    last_node = mesh_boundary[i].node1;
                    if (mesh_boundary[i].node2 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rx=%f  Ry=%f  Rn=%f  Rt=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti].Rxmin, mesh_boundary[i].state2[sti].Rymin, mesh_boundary[i].state2[sti].Rnmin, mesh_boundary[i].state2[sti].Rtmin);
#endif
                        Rxmin += nofabs(mesh_boundary[i].state2[sti].Rxmin);
                        Rymin += nofabs(mesh_boundary[i].state2[sti].Rymin);
                        Rnmin += (nofabs(mesh_boundary[i].state2[sti].Rnmin)*mesh_boundary[i].l2);
                        Rtmin += (nofabs(mesh_boundary[i].state2[sti].Rtmin)*mesh_boundary[i].l2);
                    }
                    last_node = mesh_boundary[i].node2;
                }
            }
#ifndef NDEBUG
            printf("SUM:  Rx_total_min=%f  Ry_total_min=%f  Rn_total_min=%f  Rt_total_min=%f\n", Rxmin, Rymin, Rnmin, Rtmin);
#endif
            last_node = 0;
            Rxmax = 0.0;
            Rymax = 0.0;
            Rnmax = 0.0;
            Rtmax = 0.0;
#ifndef NDEBUG
            printf("\nMAX\n");
#endif

            for (i = 0; i < mesh_boundaries_no; i++) {
                if ((mesh_boundary[i].restraint > 5) || (mesh_boundary[i].restraint == 4))
                {
                    if (mesh_boundary[i].node1 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rx=%f  Ry=%f  Rn=%f  Rt=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti].Rxmax, mesh_boundary[i].state1[sti].Rymax, mesh_boundary[i].state1[sti].Rnmax, mesh_boundary[i].state1[sti].Rtmax);
#endif
                        Rxmax += nofabs(mesh_boundary[i].state1[sti].Rxmax);
                        Rymax += nofabs(mesh_boundary[i].state1[sti].Rymax);
                        Rnmax += (nofabs(mesh_boundary[i].state1[sti].Rnmax)*mesh_boundary[i].l1);
                        Rtmax += (nofabs(mesh_boundary[i].state1[sti].Rtmax)*mesh_boundary[i].l1);
                    }
                    last_node = mesh_boundary[i].node1;
                    if (mesh_boundary[i].node2 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rx=%f  Ry=%f  Rn=%f  Rt=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti].Rxmax, mesh_boundary[i].state2[sti].Rymax, mesh_boundary[i].state2[sti].Rnmax, mesh_boundary[i].state2[sti].Rtmax);
#endif
                        Rxmax += nofabs(mesh_boundary[i].state2[sti].Rxmax);
                        Rymax += nofabs(mesh_boundary[i].state2[sti].Rymax);
                        Rnmax += (nofabs(mesh_boundary[i].state2[sti].Rnmax)*mesh_boundary[i].l2);
                        Rtmax += (nofabs(mesh_boundary[i].state2[sti].Rtmax)*mesh_boundary[i].l2);
                    }
                    last_node = mesh_boundary[i].node2;
                }
            }
#ifndef NDEBUG
            printf("SUM:  Rx_total_max=%f  Ry_total_max=%f  Rn_total_max=%f  Rt_total_max=%f\n", Rxmax, Rymax, Rnmax, Rtmax);
#endif
        }

        ////creating reaction blocks

        Ldsp.blok = Ldsp1.blok = Ldspt.blok = Ldsp1t.blok =1;
        Ldsp.kolor = Ldsp1.kolor = Ldspt.kolor = Ldsp1t.kolor = static_colors.reaction_color;
        Ldsp.typ = Ldsp1.typ = 64;  //thick
        Ldspt.typ = Ldsp1t.typ = 64+7;  //thick hidden
        /////////////////////
        //Layers
        if (sti == 0) //selecting and creating layer if doesn't exist
        {
            sprintf(desired_layer, "%s_%s", ptr_id, "reactions");

            memcpy(desired_layer_upper, desired_layer, strlen(desired_layer) + 1);
            utf8Upper(desired_layer_upper);

            for (ii = 0; ii < No_Layers; ii++) {
                memcpy(layer_name_upper, Layers[ii].name, strlen(Layers[ii].name) + 1);
                utf8Upper(layer_name_upper);

                if (strcmp(layer_name_upper, desired_layer_upper) == 0)
                    break;
            }
            if (ii < No_Layers) {
                desired_reactions_layer_no = ii;
                if (block_names)
                    get_blocks_setup(desired_reactions_layer_no, block_names, &block_names_no, MAX_L_BLOCKS,MAX_L_BLOCKS_LEN);  //to remember only visible blocks
                delete_all_from_layer_atrybut(desired_reactions_layer_no, ANieOkreslony);
            } else {
                if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

                    ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_, 12,
                                       (char *) "", 11, 1, 62);
                    no_error = FALSE;
                    goto sh_error1;
                }
                if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

                    ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_, 12,
                                       (char *) "", 11, 1, 62);
                    no_error = FALSE;
                    goto sh_error1;
                }
                No_Layers++;
                desired_reactions_layer_no = No_Layers - 1;
                Layers[No_Layers - 1].on = 1;
                Layers[No_Layers - 1].edit = 1;
                Layers[No_Layers - 1].point = 1;
                strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
                Layers[No_Layers - 1].color = Ldsp.kolor;
            }
        }

        Ldsp.warstwa = Ldsp1.warstwa = Ldspt.warstwa = Ldsp1t.warstwa = desired_reactions_layer_no;

        if (UNITS==SI)
        {
            jednostkiObX=jednostkiObXm;
            jednostkiObY=jednostkiObYm;
        }
        else
        {
            jednostkiObX=jednostkiObXi;
            jednostkiObY=jednostkiObYi;
        }


        //from meters to drawing units
        blok_origin.x = jednostkiObX(mesh_node[0].x);
        blok_origin.y = jednostkiObY(mesh_node[0].y);

        sprintf(block_name, "%s$Qn_%s_%d", ptr_id_short, SLS_ULS, combination_no);

        //checking if any of Qn exists
        char block_name_part[MAX_L_BLOCKS_LEN];
        BOOL reaction_block_exists=FALSE;
        sprintf(block_name_part, "%s$Qn", ptr_id_short);
        for (j = 0; j < block_names_no; j++) {
            if (strstr(block_names[j], block_name_part) !=NULL) {
                reaction_block_exists = TRUE;
                break;
            }
        }

        if ((reaction_block_exists==FALSE) && (sti==2)) //block doesn't exist and it's ULSLC
            hiding = FALSE;
        else {
            hiding = TRUE;
            for (j = 0; j < block_names_no; j++) {
                if (strcmp(block_names[j], block_name) == 0) {
                    hiding = FALSE;
                    break;
                }
            }
        }

        ptr_block = add_block(blok_origin.x, blok_origin.y, 'E', block_name, hiding);
        if (ptr_block == NULL) {
            fclose(f);
            ret = ask_question(1, (char *) "", (char *) confirm, (char *) "",
                               (char *) _CANNOT_CREATE_REACTIONS_BLOCK_, 12, (char *) "", 11, 1, 62);
            no_error = FALSE;
            goto sh_error3;
        }

        SPLINE s=Splinedef;
        SPLINE s1=Splinedef;
        SPLINE st=Splinedef;
        SPLINE st1=Splinedef;
        s.warstwa=s1.warstwa=st.warstwa=st1.warstwa=Ldsp.warstwa;
        s.kolor=s1.kolor=st.kolor=st1.kolor=Ldsp.kolor;
        s.typ=s1.typ=Ldsp.typ;
        st.typ=st1.typ=Ldsp.typ+7;  //hidden
        s.blok=s1.blok=st.blok=st1.blok=Ldsp.blok;

        double edge_x1, edge_y1, edge_x2, edge_y2;
        double r_edge_x1, r_edge_y1, r_edge_x2, r_edge_y2;

        double edge_x01;
        double edge_y01;

        double Qn1, Qn2, Qn1m, Qn2m, Qn01, Qn02, Qn01m, Qn02m;
        double Qt1, Qt2, Qt1m, Qt2m, Qt01, Qt02, Qt01m, Qt02m;
        double Qn1graph, Qn2graph, Qn1mgraph, Qn2mgraph;
        double Qt1graph, Qt2graph, Qt1mgraph, Qt2mgraph;
        //double Rz1, Rz2, Rz1m, Rz2m;
        double Rx1, Rx2, Rx1m, Rx2m;
        double Ry1, Ry2, Ry1m, Ry2m;
        double Rxsum, Rxsummin;
        double Rysum, Rysummin;
        double Qnsum, Qnsummin;
        double Qtsum, Qtsummin;
        double Qnave, Qnavemin;
        double Qtave, Qtavemin;
        double edge_dl, edge_l;
        const double TOL = 1e-9;
        double Qnexplus, Qnexminus;
        double Qnmexplus, Qnmexminus;
        double Qtexplus, Qtexminus;
        double Qtmexplus, Qtmexminus;
        double dxexplus, dxexminus, dxmexplus, dxmexminus;
        double x1dspexplus,y1dspexplus,x2dspexplus,y2dspexplus;
        double x1dspexminus,y1dspexminus,x2dspexminus,y2dspexminus;
        double x1mdspexplus,y1mdspexplus,x2mdspexplus,y2mdspexplus;
        double x1mdspexminus,y1mdspexminus,x2mdspexminus,y2mdspexminus;

        double Qngraphmax=50.0;   //50 mm on drawing, no more, for reasonable graphic view
        double Qtgraphmax=50.0;

        int edge=0;
        i = 0;
        while (i < mesh_boundaries_no)
        {
            if (mesh_boundary[i].edge!=edge) //new edge
            {
                edge=mesh_boundary[i].edge;
                int jj=0;
                int last_node=0;
                Rxsum=Rxsummin=0.;
                Rysum=Rysummin=0.;
                Qnsum=Qnsummin=0.;
                Qtsum=Qtsummin=0.;
                Qnave=Qnavemin=0.;
                Qnexplus=Qnexminus=0.;
                Qnmexplus=Qnmexminus=0.;
                Qtave=Qtavemin=0.;
                Qtexplus=Qtexminus=0.;
                Qtmexplus=Qtmexminus=0.;
                edge_l=0.;
                dxexplus=dxexminus=dxmexplus=dxmexminus=-1.;  //set initially out of range
                while ((edge==mesh_boundary[i].edge) && (i < mesh_boundaries_no))
                {
                    if ((mesh_boundary[i].restraint > 5) || (mesh_boundary[i].restraint == 4))
                    {
                        edge_x1 = jednostkiObX(mesh_boundary[i].x1);
                        edge_y1 = jednostkiObY(mesh_boundary[i].y1);
                        edge_x2 = jednostkiObX(mesh_boundary[i].x2);
                        edge_y2 = jednostkiObY(mesh_boundary[i].y2);

                        r_edge_x1 = mesh_boundary[i].x1;
                        r_edge_y1 = mesh_boundary[i].y1;
                        r_edge_x2 = mesh_boundary[i].x2;
                        r_edge_y2 = mesh_boundary[i].y2;

                        double kos1;
                        double koc1;

                        if (mesh_boundary[i].inverted)
                        {
                            kos1 = -mesh_boundary[i].koc;  //sin(θ-90°)=-cos(θ)
                            koc1 = mesh_boundary[i].kos; //cos(θ-90°)=sin(θ)

                            ////alternatively, if we want to flip envelope, but it will take more space on drawing
                            //kos1 = mesh_boundary[i].koc;  //sin(θ+90°)=cos(θ)
                            //koc1 = -mesh_boundary[i].kos; //cos(θ+90°)=-sin(θ)
                        }
                        else
                        {
                            kos1 = mesh_boundary[i].koc;  //sin(θ+90°)=cos(θ)
                            koc1 = -mesh_boundary[i].kos; //cos(θ+90°)=-sin(θ)

                            ////alternatively, if we want to flip envelope
                            //kos1 = -mesh_boundary[i].koc;  //sin(θ-90°)=-cos(θ)
                            //koc1 = mesh_boundary[i].kos; //cos(θ-90°)=sin(θ)
                        }

                        double x1dsp, y1dsp, x2dsp, y2dsp;
                        double x1dspt, y1dspt, x2dspt, y2dspt;
                        double x1dspm, y1dspm, x2dspm, y2dspm;
                        double x1dspmt, y1dspmt, x2dspmt, y2dspmt;

                        if (sti<2)
                        {
                            Qn1=(mesh_boundary[i].state1[sti].Rn / unit_factors_sh->R_f);
                            Qn2=(mesh_boundary[i].state2[sti].Rn / unit_factors_sh->R_f);

                            Qt1=(mesh_boundary[i].state1[sti].Rt / unit_factors_sh->R_f);
                            Qt2=(mesh_boundary[i].state2[sti].Rt / unit_factors_sh->R_f);

                            Qn1graph = Qn1 / shear_magnitude;
                            Qn2graph = Qn2 / shear_magnitude;

                            Qt1graph = Qt1 / shear_magnitude;
                            Qt2graph = Qt2 / shear_magnitude;
                            //limiting graphs
                            if (getSignFloat(Qn1graph)==1) Qn1graph=min(Qn1graph, Qngraphmax);
                            else Qn1graph=max(Qn1graph, -Qngraphmax);
                            if (getSignFloat(Qn2graph)==1) Qn2graph=min(Qn2graph, Qngraphmax);
                            else Qn2graph=max(Qn2graph, -Qngraphmax);

                            if (getSignFloat(Qt1graph)==1) Qt1graph=min(Qt1graph, Qtgraphmax);
                            else Qt1graph=max(Qt1graph, -Qtgraphmax);
                            if (getSignFloat(Qt2graph)==1) Qt2graph=min(Qt2graph, Qtgraphmax);
                            else Qt2graph=max(Qt2graph, -Qtgraphmax);

                            Rx1=(mesh_boundary[i].state1[sti].Rx / unit_factors_sh->R_f);
                            Rx2=(mesh_boundary[i].state2[sti].Rx / unit_factors_sh->R_f);
                            Ry1=(mesh_boundary[i].state1[sti].Ry / unit_factors_sh->R_f);
                            Ry2=(mesh_boundary[i].state2[sti].Ry / unit_factors_sh->R_f);

                            Rotate_Point(kos1, koc1, edge_x1, edge_y1, edge_x1 + Qn1graph, edge_y1, &x1dsp, &y1dsp);
                            Rotate_Point(kos1, koc1, edge_x2, edge_y2, edge_x2 + Qn2graph, edge_y2, &x2dsp, &y2dsp);

                            Rotate_Point(kos1, koc1, edge_x1, edge_y1, edge_x1 + Qt1graph, edge_y1, &x1dspt, &y1dspt);
                            Rotate_Point(kos1, koc1, edge_x2, edge_y2, edge_x2 + Qt2graph, edge_y2, &x2dspt, &y2dspt);

                            if (Qnexplus<Qn1)
                            {
                                Qnexplus=Qn1;
                                if (jj==0) dxexplus=0.;
                                else dxexplus=sqrt((edge_x1-edge_x01)*(edge_x1-edge_x01)+(edge_y1-edge_y01)*(edge_y1-edge_y01));
                                x1dspexplus=edge_x1;
                                y1dspexplus=edge_y1;
                                x2dspexplus=x1dsp;
                                y2dspexplus=y1dsp;
                            }

                            if (Qnexminus>Qn1)
                            {
                                Qnexminus=Qn1;
                                if (jj==0) dxexminus=0.;
                                else dxexminus=sqrt((edge_x1-edge_x01)*(edge_x1-edge_x01)+(edge_y1-edge_y01)*(edge_y1-edge_y01));
                                x1dspexminus=edge_x1;
                                y1dspexminus=edge_y1;
                                x2dspexminus=x1dsp;
                                y2dspexminus=y1dsp;
                            }

                            if (Qnexplus<Qn2)
                            {
                                Qnexplus=Qn2;
                                if (jj==0) dxexplus=sqrt((edge_x2-edge_x1)*(edge_x2-edge_x1)+(edge_y2-edge_y1)*(edge_y2-edge_y1));
                                else dxexplus=sqrt((edge_x2-edge_x01)*(edge_x2-edge_x01)+(edge_y2-edge_y01)*(edge_y2-edge_y01));
                                x1dspexplus=edge_x2;
                                y1dspexplus=edge_y2;
                                x2dspexplus=x2dsp;
                                y2dspexplus=y2dsp;
                            }

                            if (Qnexminus>Qn2)
                            {
                                Qnexminus=Qn2;
                                if (jj==0) dxexminus=sqrt((edge_x2-edge_x1)*(edge_x2-edge_x1)+(edge_y2-edge_y1)*(edge_y2-edge_y1));
                                else dxexminus=sqrt((edge_x2-edge_x01)*(edge_x2-edge_x01)+(edge_y2-edge_y01)*(edge_y2-edge_y01));
                                x1dspexminus=edge_x2;
                                y1dspexminus=edge_y2;
                                x2dspexminus=x2dsp;
                                y2dspexminus=y2dsp;
                            }
                        }
                        else
                        {  //max
                            Qn1=(mesh_boundary[i].state1[sti].Rnmax / unit_factors_sh->R_f);
                            Qn2=(mesh_boundary[i].state2[sti].Rnmax / unit_factors_sh->R_f);

                            Qt1=(mesh_boundary[i].state1[sti].Rtmax / unit_factors_sh->R_f);
                            Qt2=(mesh_boundary[i].state2[sti].Rtmax / unit_factors_sh->R_f);

                            Qn1graph = Qn1 / shear_magnitude;
                            Qn2graph = Qn2 / shear_magnitude;

                            Qt1graph = Qt1 / shear_magnitude;
                            Qt2graph = Qt2 / shear_magnitude;
                            //limiting graphs
                            if (getSignFloat(Qn1graph)==1) Qn1graph=min(Qn1graph, Qngraphmax);
                            else Qn1graph=max(Qn1graph, -Qngraphmax);
                            if (getSignFloat(Qn2graph)==1) Qn2graph=min(Qn2graph, Qngraphmax);
                            else Qn2graph=max(Qn2graph, -Qngraphmax);

                            if (getSignFloat(Qt1graph)==1) Qt1graph=min(Qt1graph, Qtgraphmax);
                            else Qt1graph=max(Qt1graph, -Qtgraphmax);
                            if (getSignFloat(Qt2graph)==1) Qt2graph=min(Qt2graph, Qtgraphmax);
                            else Qt2graph=max(Qt2graph, -Qtgraphmax);

                            Rx1=(mesh_boundary[i].state1[sti].Rxmax / unit_factors_sh->R_f);
                            Rx2=(mesh_boundary[i].state2[sti].Rxmax / unit_factors_sh->R_f);
                            Ry1=(mesh_boundary[i].state1[sti].Rymax / unit_factors_sh->R_f);
                            Ry2=(mesh_boundary[i].state2[sti].Rymax / unit_factors_sh->R_f);

                            Rotate_Point(kos1, koc1, edge_x1, edge_y1, edge_x1 + Qn1graph, edge_y1, &x1dsp, &y1dsp);
                            Rotate_Point(kos1, koc1, edge_x2, edge_y2, edge_x2 + Qn2graph, edge_y2, &x2dsp, &y2dsp);

                            Rotate_Point(kos1, koc1, edge_x1, edge_y1, edge_x1 + Qt1graph, edge_y1, &x1dspt, &y1dspt);
                            Rotate_Point(kos1, koc1, edge_x2, edge_y2, edge_x2 + Qt2graph, edge_y2, &x2dspt, &y2dspt);

                            //min
                            Qn1m=(mesh_boundary[i].state1[sti].Rnmin / unit_factors_sh->R_f);
                            Qn2m=(mesh_boundary[i].state2[sti].Rnmin / unit_factors_sh->R_f);

                            Qt1m=(mesh_boundary[i].state1[sti].Rtmin / unit_factors_sh->R_f);
                            Qt2m=(mesh_boundary[i].state2[sti].Rtmin / unit_factors_sh->R_f);

                            Qn1mgraph = Qn1m / shear_magnitude;
                            Qn2mgraph = Qn2m / shear_magnitude;

                            Qt1mgraph = Qt1m / shear_magnitude;
                            Qt2mgraph = Qt2m / shear_magnitude;
                            //limiting graphs
                            if (getSignFloat(Qn1mgraph)==1) Qn1mgraph=min(Qn1mgraph, Qngraphmax);
                            else Qn1mgraph=max(Qn1mgraph, -Qngraphmax);
                            if (getSignFloat(Qn2mgraph)==1) Qn2mgraph=min(Qn2mgraph, Qngraphmax);
                            else Qn2mgraph=max(Qn2mgraph, -Qngraphmax);

                            if (getSignFloat(Qt1mgraph)==1) Qt1mgraph=min(Qt1mgraph, Qtgraphmax);
                            else Qt1mgraph=max(Qt1mgraph, -Qtgraphmax);
                            if (getSignFloat(Qt2mgraph)==1) Qt2mgraph=min(Qt2mgraph, Qtgraphmax);
                            else Qt2mgraph=max(Qt2mgraph, -Qtgraphmax);

                            Rx1m=(mesh_boundary[i].state1[sti].Rxmin / unit_factors_sh->R_f);
                            Rx2m=(mesh_boundary[i].state2[sti].Rxmin / unit_factors_sh->R_f);
                            Ry1m=(mesh_boundary[i].state1[sti].Rymin / unit_factors_sh->R_f);
                            Ry2m=(mesh_boundary[i].state2[sti].Rymin / unit_factors_sh->R_f);

                            Rotate_Point(kos1, koc1, edge_x1, edge_y1, edge_x1 + Qn1mgraph, edge_y1, &x1dspm, &y1dspm);
                            Rotate_Point(kos1, koc1, edge_x2, edge_y2, edge_x2 + Qn2mgraph, edge_y2, &x2dspm, &y2dspm);

                            Rotate_Point(kos1, koc1, edge_x1, edge_y1, edge_x1 + Qt1mgraph, edge_y1, &x1dspmt, &y1dspmt);
                            Rotate_Point(kos1, koc1, edge_x2, edge_y2, edge_x2 + Qt2mgraph, edge_y2, &x2dspmt, &y2dspmt);

                            if (Qnexplus<Qn1)
                            {
                                Qnexplus=Qn1;
                                if (jj==0) dxexplus=0.;
                                else dxexplus=sqrt((edge_x1-edge_x01)*(edge_x1-edge_x01)+(edge_y1-edge_y01)*(edge_y1-edge_y01));
                                x1dspexplus=edge_x1;
                                y1dspexplus=edge_y1;
                                x2dspexplus=x1dsp;
                                y2dspexplus=y1dsp;
                            }

                            if (Qnexminus>Qn1)
                            {
                                Qnexminus=Qn1;
                                if (jj==0) dxexminus=0.;
                                else dxexminus=sqrt((edge_x1-edge_x01)*(edge_x1-edge_x01)+(edge_y1-edge_y01)*(edge_y1-edge_y01));
                                x1dspexminus=edge_x1;
                                y1dspexminus=edge_y1;
                                x2dspexminus=x1dsp;
                                y2dspexminus=y1dsp;
                            }

                            if (Qnexplus<Qn2)
                            {
                                Qnexplus=Qn2;
                                if (jj==0) dxexplus=sqrt((edge_x2-edge_x1)*(edge_x2-edge_x1)+(edge_y2-edge_y1)*(edge_y2-edge_y1));
                                else dxexplus=sqrt((edge_x2-edge_x01)*(edge_x2-edge_x01)+(edge_y2-edge_y01)*(edge_y2-edge_y01));
                                x1dspexplus=edge_x2;
                                y1dspexplus=edge_y2;
                                x2dspexplus=x2dsp;
                                y2dspexplus=y2dsp;
                            }

                            if (Qnexminus>Qn2)
                            {
                                Qnexminus=Qn2;
                                if (jj==0) dxexminus=sqrt((edge_x2-edge_x1)*(edge_x2-edge_x1)+(edge_y2-edge_y1)*(edge_y2-edge_y1));
                                else dxexminus=sqrt((edge_x2-edge_x01)*(edge_x2-edge_x01)+(edge_y2-edge_y01)*(edge_y2-edge_y01));
                                x1dspexminus=edge_x2;
                                y1dspexminus=edge_y2;
                                x2dspexminus=x2dsp;
                                y2dspexminus=y2dsp;
                            }

                            //for minimums
                            if (Qnmexplus<Qn1m)
                            {
                                Qnmexplus=Qn1m;
                                if (jj==0) dxmexplus=0.;
                                else dxmexplus=sqrt((edge_x1-edge_x01)*(edge_x1-edge_x01)+(edge_y1-edge_y01)*(edge_y1-edge_y01));
                                x1mdspexplus=edge_x1;
                                y1mdspexplus=edge_y1;
                                x2mdspexplus=x1dsp;
                                y2mdspexplus=y1dsp;
                            }

                            if (Qnmexminus>Qn1m)
                            {
                                Qnmexminus=Qn1m;
                                if (jj==0) dxmexminus=0.;
                                else dxmexminus=sqrt((edge_x1-edge_x01)*(edge_x1-edge_x01)+(edge_y1-edge_y01)*(edge_y1-edge_y01));
                                x1mdspexminus=edge_x1;
                                y1mdspexminus=edge_y1;
                                x2mdspexminus=x1dsp;
                                y2mdspexminus=y1dsp;
                            }

                            if (Qnmexplus<Qn2m)
                            {
                                Qnmexplus=Qn2m;
                                if (jj==0) dxmexplus=sqrt((edge_x2-edge_x1)*(edge_x2-edge_x1)+(edge_y2-edge_y1)*(edge_y2-edge_y1));
                                else dxmexplus=sqrt((edge_x2-edge_x01)*(edge_x2-edge_x01)+(edge_y2-edge_y01)*(edge_y2-edge_y01));
                                x1mdspexplus=edge_x2;
                                y1mdspexplus=edge_y2;
                                x2mdspexplus=x2dsp;
                                y2mdspexplus=y2dsp;
                            }

                            if (Qnexminus>Qn2m)
                            {
                                Qnmexminus=Qn2m;
                                if (jj==0) dxmexminus=sqrt((edge_x2-edge_x1)*(edge_x2-edge_x1)+(edge_y2-edge_y1)*(edge_y2-edge_y1));
                                else dxmexminus=sqrt((edge_x2-edge_x01)*(edge_x2-edge_x01)+(edge_y2-edge_y01)*(edge_y2-edge_y01));
                                x1mdspexminus=edge_x2;
                                y1mdspexminus=edge_y2;
                                x2mdspexminus=x2dsp;
                                y2mdspexminus=y2dsp;
                            }
                            Ldspm.x1=(float)x1dspm;
                            Ldspm.y1=(float)y1dspm;
                            Ldspm.x2=(float)x2dspm;
                            Ldspm.y2=(float)y2dspm;

                            Ldspmt.x1=(float)x1dspmt;
                            Ldspmt.y1=(float)y1dspmt;
                            Ldspmt.x2=(float)x2dspmt;
                            Ldspmt.y2=(float)y2dspmt;
                        }
                        Ldsp.x1=(float)x1dsp;
                        Ldsp.y1=(float)y1dsp;
                        Ldsp.x2=(float)x2dsp;
                        Ldsp.y2=(float)y2dsp;

                        Ldspt.x1=(float)x1dspt;
                        Ldspt.y1=(float)y1dspt;
                        Ldspt.x2=(float)x2dspt;
                        Ldspt.y2=(float)y2dspt;

                        if (jj==0) {
                            Ldsp1.x1 = (float) edge_x1;
                            Ldsp1.y1 = (float) edge_y1;
                            Ldsp1.x2 = Ldsp.x1;
                            Ldsp1.y2 = Ldsp.y1;
                            adr = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1);

                            Ldsp1t.x1 = (float) edge_x1;
                            Ldsp1t.y1 = (float) edge_y1;
                            Ldsp1t.x2 = Ldspt.x1;
                            Ldsp1t.y2 = Ldspt.y1;
                            adr = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1t);

                            //label
                            LINIA Le=Ldef;
                            Le.x1=(float)edge_x1;
                            Le.y1=(float)edge_y1;
                            Le.x2=(float)edge_x2;
                            Le.y2=(float)edge_y2;
                            double dx=0.;
                            if (!Check_if_Equal(Qn1, 0.0))
                                draw_label(&Ldsp1, &Le, dx, 0, 0, Qn1, r_precision, FALSE, "", "");

                            Qn01=Qn1;

                            //min
                            if (sti>1)
                            {
                                Ldsp1.x1 = (float) edge_x1;
                                Ldsp1.y1 = (float) edge_y1;
                                Ldsp1.x2 = Ldspm.x1;
                                Ldsp1.y2 = Ldspm.y1;
                                adr = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1);

                                Ldsp1t.x1 = (float) edge_x1;
                                Ldsp1t.y1 = (float) edge_y1;
                                Ldsp1t.x2 = Ldspmt.x1;
                                Ldsp1t.y2 = Ldspmt.y1;
                                adr = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1t);

                                //min label
                                Le.x1=(float)edge_x1;
                                Le.y1=(float)edge_y1;
                                Le.x2=(float)edge_x2;
                                Le.y2=(float)edge_y2;
                                if (!Check_if_Equal(Qn1m, 0.0))
                                {
                                    if ((getSignFloat(Qn1m)!=getSignFloat(Qn1)) || (fabs(Qn1m)>fabs(Qn1)))
                                        draw_label(&Ldsp1, &Le, dx, 0, 0, Qn1m, r_precision, FALSE, "", "");
                                }
                                Qn01m=Qn1m;
                            }

                            edge_x01=edge_x1;
                            edge_y01=edge_y1;
                        }

                        edge_dl=sqrt((r_edge_x2-r_edge_x1)*(r_edge_x2-r_edge_x1)+(r_edge_y2-r_edge_y1)*(r_edge_y2-r_edge_y1))/2.;

                        Qnave+=Qn1*edge_dl;
                        Qnave+=Qn2*edge_dl;
                        Qtave+=Qt1*edge_dl;
                        Qtave+=Qt2*edge_dl;

                        if (sti>1)
                        {
                            Qnavemin+=Qn1m*edge_dl;
                            Qnavemin+=Qn2m*edge_dl;
                            Qtavemin+=Qt1m*edge_dl;
                            Qtavemin+=Qt2m*edge_dl;
                        }

                        if (last_node!=mesh_boundary[i].node1)
                        {
                            s.xy[jj] = Ldsp.x1;

                            if (sti>1)  //set only if it's challenging s
                            {
                                if ((getSignFloat(Qn1m)!=getSignFloat(Qn1)) || (fabs(Qn1m)>fabs(Qn1)))
                                    s1.xy[jj] = Ldspm.x1;
                                else
                                    s1.xy[jj] = (float)edge_x1;
                            }

                            //tangential
                            st.xy[jj] = (float)x1dspt;

                            if (sti>1)  //set only if it's challenging s
                            {
                                if ((getSignFloat(Qt1m)!=getSignFloat(Qt1)) || (fabs(Qt1m)>fabs(Qt1)))
                                    st1.xy[jj] = (float)x1dspmt;
                                else
                                    st1.xy[jj] = (float)edge_x1;
                            }

                            jj++;
                            s.xy[jj] = Ldsp.y1;

                            if (sti>1) //set only if it's challenging s
                            {
                                if ((getSignFloat(Qn1m)!=getSignFloat(Qn1)) || (fabs(Qn1m)>fabs(Qn1)))
                                    s1.xy[jj] = Ldspm.y1;
                                else
                                    s1.xy[jj] = (float)edge_y1;
                            }

                            //tangential
                            st.xy[jj] = (float)y1dspt;

                            if (sti>1) //set only if it's challenging s
                            {
                                if ((getSignFloat(Qt1m)!=getSignFloat(Qt1)) || (fabs(Qt1m)>fabs(Qt1)))
                                    st1.xy[jj] = (float)y1dspmt;
                                else
                                    st1.xy[jj] = (float)edge_y1;
                            }

                            jj++;
                            Rxsum+=Rx1;
                            Rysum+=Ry1;
                            Qnsum+=Qn1*mesh_boundary[i].l1;
                            Qtsum+=Qt1*mesh_boundary[i].l1;
                            if (sti>1)
                            {
                                Rxsummin+=Rx1m;
                                Rysummin+=Ry1m;
                                Qnsummin+=Qn1m*mesh_boundary[i].l1;
                                Qtsummin+=Qt1m*mesh_boundary[i].l1;
                            }
                        }
                        last_node=mesh_boundary[i].node1;
                        s.xy[jj]=Ldsp.x2;

                        if (sti>1) //set only if it's challenging s
                        {
                            if ((getSignFloat(Qn2m)!=getSignFloat(Qn2)) || (fabs(Qn2m)>fabs(Qn2)))
                                s1.xy[jj] = Ldspm.x2;
                            else
                                s1.xy[jj] = (float)edge_x2;
                        }

                        //tangential
                        st.xy[jj] = (float)x2dspt;

                        if (sti>1) //set only if it's challenging s
                        {
                            if ((getSignFloat(Qt2m)!=getSignFloat(Qt2)) || (fabs(Qt2m)>fabs(Qt2)))
                                st1.xy[jj] = (float)x2dspmt;
                            else
                                st1.xy[jj] = (float)edge_x2;
                        }

                        jj++;
                        s.xy[jj]=Ldsp.y2;

                        if (sti>1) //set only if it's challenging s
                        {
                            if ((getSignFloat(Qn2m)!=getSignFloat(Qn2)) || (fabs(Qn2m)>fabs(Qn2)))
                                s1.xy[jj] = Ldspm.y2;
                            else
                                s1.xy[jj] = (float)edge_y2;
                        }

                        //tangential
                        st.xy[jj] = (float)y2dspt;

                        if (sti>1) //set only if it's challenging s
                        {
                            if ((getSignFloat(Qt2m)!=getSignFloat(Qt2)) || (fabs(Qt2m)>fabs(Qt2)))
                                st1.xy[jj] = (float)y2dspmt;
                            else
                                st1.xy[jj] = (float)edge_y2;
                        }

                        jj++;
                        Rxsum+=Rx2;
                        Rysum+=Ry2;
                        Qnsum+=Qn2*mesh_boundary[i].l2;
                        Qtsum+=Qt2*mesh_boundary[i].l2;

                        if (sti>1)
                        {
                            Rxsummin+=Rx2m;
                            Rysummin+=Ry2m;
                            Qnsummin+=Qn2*mesh_boundary[i].l2;
                            Qtsummin+=Qt2*mesh_boundary[i].l2;
                        }
                        last_node=mesh_boundary[i].node2;
                        edge_l+=sqrt((r_edge_x2-r_edge_x1)*(r_edge_x2-r_edge_x1)+(r_edge_y2-r_edge_y1)*(r_edge_y2-r_edge_y1));
                    }
                    i++;
                }
                if (jj>0)
                {
                    s.lp = jj;
                    s.xy[s.lp]=0.75f;
                    s.n = 8 + (s.lp + 1 ) * sizeof(float);
                    s.npts=6; //6 means B-Spline multipoints,  //5 means Cardinal multipoints
                    s.closed=0;
                    adr = dodaj_obiekt((BLOK *) dane, (void *) &s);

                    Ldsp1.x1 = Ldsp.x2;
                    Ldsp1.y1 = Ldsp.y2;
                    Ldsp1.x2 = (float) edge_x2;
                    Ldsp1.y2 = (float) edge_y2;
                    adr = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1);

                    //label
                    LINIA Le=Ldef;
                    Le.x1=(float)edge_x1;
                    Le.y1=(float)edge_y1;
                    Le.x2=(float)edge_x2;
                    Le.y2=(float)edge_y2;
                    double dx=sqrt((edge_x2-edge_x1)*(edge_x2-edge_x1)+(edge_y2-edge_y1)*(edge_y2-edge_y1));
                    if (!Check_if_Equal(Qn2, 0.0))
                        draw_label(&Ldsp1, &Le, dx, 0, 0, Qn2, r_precision, FALSE, "", "");

                    //tangential
                    st.lp = jj;
                    st.xy[st.lp]=0.75f;
                    st.n = 8 + (st.lp + 1 ) * sizeof(float);
                    st.npts=6; //6 means B-Spline multipoints,  //5 means Cardinal multipoints
                    st.closed=0;
                    adr = dodaj_obiekt((BLOK *) dane, &st);  ////1
                    //ending line
                    Ldsp1t.x1 = Ldspt.x2;
                    Ldsp1t.y1 = Ldspt.y2;
                    Ldsp1t.x2 = (float) edge_x2;
                    Ldsp1t.y2 = (float) edge_y2;
                    adr = dodaj_obiekt((BLOK *) dane, &Ldsp1t);   ////2

                    Qn02=Qn2;

                    //extreme maximums
                    if ((Qnexplus>Qn01) && (Qnexplus>Qn02) && (!Check_if_Equal(Qnexplus, 0.0)))
                    {
                        Ldsp1.x1 = (float)x1dspexplus;
                        Ldsp1.y1 = (float)y1dspexplus;
                        Ldsp1.x2 = (float)x2dspexplus;
                        Ldsp1.y2 = (float)y2dspexplus;
                        draw_label(&Ldsp1, &Le, dxexplus, 0, 0, Qnexplus, r_precision, FALSE, "", "");
                    }

                    if ((Qnexminus<Qn01) && (Qnexminus<Qn02) && (!Check_if_Equal(Qnexminus, 0.0)))
                    {
                        Ldsp1.x1 = (float)x1dspexminus;
                        Ldsp1.y1 = (float)y1dspexminus;
                        Ldsp1.x2 = (float)x2dspexminus;
                        Ldsp1.y2 = (float)y2dspexminus;
                        draw_label(&Ldsp1, &Le, dxexminus, 0, 0, Qnexminus, r_precision, FALSE, "", "");
                    }

                    double d=1.5; //2.0;  //the distance from supporting edge in mm on drawing
                    double d1=0.5 + zmwym.wysokosc * 0.5;
                    double dt= -0.5 - zmwym.wysokosc * 0.5;
                    double d1t= -0.5 - zmwym.wysokosc * 0.5;

                    int nd1=0;
                    int ndt1=0;

                    //the sum
                    Ldsp1.x1=(float)edge_x01;
                    Ldsp1.y1=(float)edge_y01;
                    Ldsp1.x2=(float)edge_x2;
                    Ldsp1.y2=(float)edge_y2;
                    PLINIA PL1;
                    parametry_lini(&Ldsp1, &PL1);
                    //angle correction
                    double bad_angle=PL1.kat * Pi2 / 360.0;
                    double good_angle=Angle_Simple(PL1.kat * Pi2 / 360.0);
                    //PL1.sin=sin(good_angle);
                    //PL1.cos=cos(good_angle);
                    if (Check_if_Equal(good_angle,bad_angle)==TRUE)
                    {
                        d= -1.5 - zmwym.wysokosc * 0.5;
                        d1= -0.5 - zmwym.wysokosc * 0.5;

                        //dt=1.5;
                        dt=0.5;
                        d1t=0.5 + zmwym.wysokosc * 0.5;
                    }

                    dx=sqrt((edge_x2-edge_x01)*(edge_x2-edge_x01)+(edge_y2-edge_y01)*(edge_y2-edge_y01))/2.;

                    Ldsp1t.x1=Ldsp1.x1-(float)(dt*PL1.sin);
                    Ldsp1t.y1=Ldsp1.y1+(float)(dt*PL1.cos);
                    Ldsp1t.x2=Ldsp1.x2-(float)(dt*PL1.sin);
                    Ldsp1t.y2=Ldsp1.y2+(float)(dt*PL1.cos);

                    Ldsp1.x1=Ldsp1.x1-(float)(d*PL1.sin);
                    Ldsp1.y1=Ldsp1.y1+(float)(d*PL1.cos);
                    Ldsp1.x2=Ldsp1.x2-(float)(d*PL1.sin);
                    Ldsp1.y2=Ldsp1.y2+(float)(d*PL1.cos);

                    if (!Check_if_Equal(Qnsum, 0.0))
                    {
                        if ((sti<2) ||
                            ((getSignFloat(Qnsummin)!=getSignFloat(Qnsum)) || (fabs(Qnsum)>=fabs(Qnsummin)))) {
                            draw_label(&Ldsp1, &Le, dx, 0, 0, Qnsum, r_precision, FALSE, "", "Σ");
                            nd1++;
                            }
                    }

                    if (!Check_if_Equal(Qtsum, 0.0))
                    {   if ((sti<2) ||
                            ((getSignFloat(Qtsummin)!=getSignFloat(Qtsum)) || (fabs(Qtsum)>=fabs(Qtsummin)))) {
                        draw_label(&Ldsp1t, &Le, dx, 0, 0, Qtsum, r_precision, FALSE, "", "Σ~t^");
                        ndt1++;
                            }
                    }

                    //average value
                    Ldsp1.x1=Ldsp1.x1-(float)(d1*PL1.sin);
                    Ldsp1.y1=Ldsp1.y1+(float)(d1*PL1.cos);
                    Ldsp1.x2=Ldsp1.x2-(float)(d1*PL1.sin);
                    Ldsp1.y2=Ldsp1.y2+(float)(d1*PL1.cos);

                    Ldsp1t.x1=Ldsp1t.x1-(float)(d1t*PL1.sin);
                    Ldsp1t.y1=Ldsp1t.y1+(float)(d1t*PL1.cos);
                    Ldsp1t.x2=Ldsp1t.x2-(float)(d1t*PL1.sin);
                    Ldsp1t.y2=Ldsp1t.y2+(float)(d1t*PL1.cos);

                    if (edge_l>TOL) Qnave /= edge_l;
                    if (sti>1) if (edge_l>TOL) Qnavemin /= edge_l;

                    if (!Check_if_Equal(Qnave, 0.0)) {
                        if ((sti<2) ||
                           ((getSignFloat(Qnavemin)!=getSignFloat(Qnave)) || (fabs(Qnave)>=fabs(Qnavemin)))) {
                            draw_label(&Ldsp1, &Le, dx, 0, 0, Qnave, r_precision, FALSE, "", "μ");
                            nd1++;
                           }
                    }

                    if (edge_l>TOL) Qtave /= edge_l;
                    if (sti>1) if (edge_l>TOL) Qtavemin /= edge_l;

                    if (!Check_if_Equal(Qtave, 0.0)) {
                        if ((sti<2) ||
                            ((getSignFloat(Qtavemin)!=getSignFloat(Qtave)) || (fabs(Qtave)>=fabs(Qtavemin)))) {
                            draw_label(&Ldsp1t, &Le, dx, 0, 0, Qtave, r_precision, FALSE, "", "μ~t^");
                            ndt1++;
                            }
                    }

                    //min
                    if (sti>1)
                    {
                        s1.lp = jj;
                        s1.xy[s1.lp]=0.75f;
                        s1.n = 8 + (s1.lp + 1 ) * sizeof(float);
                        s1.npts=6; //6 means B-Spline multipoints,  //5 means Cardinal multipoints
                        s1.closed=0;
                        adr = dodaj_obiekt((BLOK *) dane, &s1);

                        Ldsp1.x1 = Ldspm.x2;
                        Ldsp1.y1 = Ldspm.y2;
                        Ldsp1.x2 = (float) edge_x2;
                        Ldsp1.y2 = (float) edge_y2;
                        adr = dodaj_obiekt((BLOK *) dane, &Ldsp1);

                        //min label
                        Le.x1=(float)edge_x1;
                        Le.y1=(float)edge_y1;
                        Le.x2=(float)edge_x2;
                        Le.y2=(float)edge_y2;

                        if (!Check_if_Equal(Qn2m, 0.0))
                        {
                            if ((getSignFloat(Qn2m)!=getSignFloat(Qn2)) || (fabs(Qn2m)>fabs(Qn2)))
                                draw_label(&Ldsp1, &Le, dx, 0, 0, Qn2m, r_precision, FALSE, "", "");
                        }

                        //tangental
                        st1.lp = jj;
                        st1.xy[st1.lp]=0.75f;
                        st1.n = 8 + (st1.lp + 1 ) * sizeof(float);
                        st1.npts=6; //6 means B-Spline multipoints,  //5 means Cardinal multipoints
                        st1.closed=0;
                        adr = dodaj_obiekt((BLOK *) dane, &st1);  ////3
                        //closing line
                        Ldsp1t.x1 = Ldspmt.x2;
                        Ldsp1t.y1 = Ldspmt.y2;
                        Ldsp1t.x2 = (float) edge_x2;
                        Ldsp1t.y2 = (float) edge_y2;

                        adr = dodaj_obiekt((BLOK *) dane, &Ldsp1t);  ////4

                        Qn02m=Qn2m;

                        //extreme minimums
                        if ((Qnmexplus>Qn01m) && (Qnmexplus>Qn02m) && (!Check_if_Equal(Qnmexplus, 0.0)))
                        {
                            if ((getSignFloat(Qnmexplus)!=getSignFloat(Qnexplus)) || (fabs(Qnmexplus)>fabs(Qnexplus)))
                            {
                                Ldsp1.x1 = (float) x1mdspexplus;
                                Ldsp1.y1 = (float) y1mdspexplus;
                                Ldsp1.x2 = (float) x2mdspexplus;
                                Ldsp1.y2 = (float) y2mdspexplus;
                                draw_label(&Ldsp1, &Le, dxmexplus, 0, 0, Qnmexplus, r_precision, FALSE, "", "");
                            }
                        }

                        if ((Qnmexminus<Qn01m) && (Qnexminus<Qn02m) && (!Check_if_Equal(Qnmexminus, 0.0)))
                        {
                            if ((getSignFloat(Qnmexminus)!=getSignFloat(Qnexminus)) || (fabs(Qnmexminus)>fabs(Qnexminus)))
                            {
                                Ldsp1.x1 = (float) x1mdspexminus;
                                Ldsp1.y1 = (float) y1mdspexminus;
                                Ldsp1.x2 = (float) x2mdspexminus;
                                Ldsp1.y2 = (float) y2mdspexminus;
                                draw_label(&Ldsp1, &Le, dxmexminus, 0, 0, Qnmexminus, r_precision, FALSE, "", "");
                            }
                        }

                        //the min sum
                        Ldsp1.x1=(float)edge_x01;
                        Ldsp1.y1=(float)edge_y01;
                        Ldsp1.x2=(float)edge_x2;
                        Ldsp1.y2=(float)edge_y2;
                        parametry_lini(&Ldsp1, &PL1);

                        //angle correction
                        double bad_angle=PL1.kat * Pi2 / 360.0;
                        double good_angle=Angle_Simple(PL1.kat * Pi2 / 360.0);
                        //PL1.sin=sin(good_angle);
                        //PL1.cos=cos(good_angle);
                        if (Check_if_Equal(good_angle,bad_angle)==TRUE)
                        {
                            d= -1.5 - zmwym.wysokosc * 0.5;
                            d1= -0.5 - zmwym.wysokosc * 0.5;

                            //dt=1.5;
                            dt=0.5;
                            d1t=0.5 + zmwym.wysokosc * 0.5;
                        }

                        dx=sqrt((edge_x2-edge_x01)*(edge_x2-edge_x01)+(edge_y2-edge_y01)*(edge_y2-edge_y01))/2.;

                        double d2=d+nd1*d1;
                        double dt2=dt+ndt1*d1t;

                        Ldsp1t.x1=Ldsp1.x1-(float)(dt2*PL1.sin);
                        Ldsp1t.y1=Ldsp1.y1+(float)(dt2*PL1.cos);
                        Ldsp1t.x2=Ldsp1.x2-(float)(dt2*PL1.sin);
                        Ldsp1t.y2=Ldsp1.y2+(float)(dt2*PL1.cos);

                        Ldsp1.x1=Ldsp1.x1-(float)(d2*PL1.sin);
                        Ldsp1.y1=Ldsp1.y1+(float)(d2*PL1.cos);
                        Ldsp1.x2=Ldsp1.x2-(float)(d2*PL1.sin);
                        Ldsp1.y2=Ldsp1.y2+(float)(d2*PL1.cos);

                        if (!Check_if_Equal(Qnsummin, 0.0))
                        {
                            if ((getSignFloat(Qnsummin)!=getSignFloat(Qnsum)) || (fabs(Qnsummin)>fabs(Qnsum)))
                                draw_label(&Ldsp1, &Le, dx, 0, 0, Qnsummin, r_precision, FALSE, "", "Σ");
                        }

                        if (!Check_if_Equal(Qtsummin, 0.0))
                        {
                            if ((getSignFloat(Qtsummin)!=getSignFloat(Qtsum)) || (fabs(Qtsummin)>fabs(Qtsum)))
                                draw_label(&Ldsp1t, &Le, dx, 0, 0, Qtsummin, r_precision, FALSE, "", "Σ~t^");
                        }

                        //average min value
                        Ldsp1.x1=Ldsp1.x1-(float)(d1*PL1.sin);
                        Ldsp1.y1=Ldsp1.y1+(float)(d1*PL1.cos);
                        Ldsp1.x2=Ldsp1.x2-(float)(d1*PL1.sin);
                        Ldsp1.y2=Ldsp1.y2+(float)(d1*PL1.cos);

                        Ldsp1t.x1=Ldsp1t.x1-(float)(d1t*PL1.sin);
                        Ldsp1t.y1=Ldsp1t.y1+(float)(d1t*PL1.cos);
                        Ldsp1t.x2=Ldsp1t.x2-(float)(d1t*PL1.sin);
                        Ldsp1t.y2=Ldsp1t.y2+(float)(d1t*PL1.cos);

                        if (!Check_if_Equal(Qnavemin, 0.0))
                        {
                            if ((getSignFloat(Qnavemin)!=getSignFloat(Qnave)) || (fabs(Qnavemin)>fabs(Qnave)))
                                draw_label(&Ldsp1, &Le, dx, 0, 0, Qnavemin, r_precision, FALSE, "", "μ");
                        }

                        if (!Check_if_Equal(Qtavemin, 0.0))
                        {
                            if ((getSignFloat(Qtavemin)!=getSignFloat(Qtave)) || (fabs(Qtavemin)>fabs(Qtave)))
                                draw_label(&Ldsp1t, &Le, dx, 0, 0, Qtavemin, r_precision, FALSE, "", "μ~t^");
                        }
                    }
                }
            }
        }

        sh_error3:
                ////reactions block end

                ////FREEING

        if (supp1_sigx_max) {free(supp1_sigx_max); supp1_sigx_max=NULL;}
        if (supp1_sigy_max) {free(supp1_sigy_max); supp1_sigy_max=NULL;}
        if (supp1_sigxy_max) {free(supp1_sigxy_max); supp1_sigxy_max=NULL;}

        if (supp2_sigx_max) {free(supp2_sigx_max); supp2_sigx_max=NULL;}
        if (supp2_sigy_max) {free(supp2_sigy_max); supp2_sigy_max=NULL;}
        if (supp2_sigxy_max) {free(supp2_sigxy_max); supp2_sigxy_max=NULL;}

        my_sleep(100);

    }  //sti done

    for (i = 0; i < STRESS_NUMBER; i++) {
        if (min_stress_bak[i] != NULL) {
            free(min_stress_bak[i]);
            min_stress_bak[i] = NULL;
        }
    }

    for (i = 0; i < STRESS_NUMBER; i++) {
        if (max_stress_bak[i] != NULL) {
            free(max_stress[i]);
            max_stress_bak[i] = NULL;
        }
    }

    if (perm_s_bak) {free(perm_s_bak); perm_s_bak=NULL;}

    sh_error1:

    if (mesh_node) {free(mesh_node); mesh_node=NULL;}
    if (mesh_element) {free(mesh_element); mesh_element=NULL;}
    if (mesh_boundary) {free(mesh_boundary); mesh_boundary=NULL;}

    if (body_property) {free(body_property); body_property=NULL;}

    sh_error:

    if (was_refreshed == FALSE) {
        ///removing Ablok flags and refresh
        zmien_atrybut_undo(dane, dane + dane_size);
        blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
        zmien_atrybut(ADP, ADK, Ablok, Aoblok);
        was_refreshed = TRUE;
    }

    if (strlen(report) > 0) {
        int edit_params = 0;
        int tab;
#ifndef LINUX
        int single = 0;  //info
#else
        int single = 2;  //info
#endif
        ret = EditText(report, edit_params, mynCmdShow, &single, &tab);
        report[0] = '\0';
    }

    if (body_area != NULL) {
        free(body_area);
        body_area = NULL;
    }

    if (surface_load[0] != NULL) {
        for (i = 0; i < 64; i++) {
            if (surface_load[i] != NULL) {
                free(surface_load[i]);
                surface_load[i] = NULL;
            }
        }
    }

    ////if (st_uniform_load_comb) free(st_uniform_load_comb);
    ////if (st_uniform_load_cons) free(st_uniform_load_cons);

    if (sh_property) {free(sh_property); sh_property=NULL;}
    if (sh_load_factors) {free(sh_load_factors); sh_load_factors=NULL;}
    if (sh_node) {free(sh_node); sh_node=NULL;}
    if (sh_node_emb) {free(sh_node_emb); sh_node_emb=NULL;}
    if (sh_node_emb_edge) {free(sh_node_emb_edge); sh_node_emb_edge=NULL;}
    if (sh_edge) {free(sh_edge); sh_edge=NULL;}
    if (sh_edge_emb) {free(sh_edge_emb); sh_edge_emb=NULL;}
    if (sh_load) {free(sh_load); sh_load=NULL;}
    if (shield_property) {free(shield_property); shield_property=NULL;}
    if (hole_property) {free(hole_property); hole_property=NULL;}
    if (wall_property) {free(wall_property); wall_property=NULL;}
    if (zone_property) {free(zone_property); zone_property=NULL;}

    // Free the allocated memory when no longer needed
    for (i = 0; i < MAX_L_BLOCKS; i++) {
        if (block_names[i]) {free(block_names[i]); block_names[i]=NULL;}
    }
    if (block_names) {free(block_names); block_names=NULL;}
    ////////////////

    push:

    remove_short_notice();

    glb_silent=FALSE;

    redraw();

    my_sleep(100);

}
#undef __O_SHIELD__