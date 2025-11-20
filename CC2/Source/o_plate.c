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

#define __O_PLATE__
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
#ifndef _plate_
#define _plate_ "plate/"
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
#ifndef _plate_
#define _plate_ "plate\\"
#endif
#endif

#define BIGNOD 1e200
#define BIGNOF 1e20

//#define REACTIONSNSEW

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

//extern int hatch_proc_test (long_long, long_long, double, double, T_PTR_Hatch_Param, int comput_area, double df_apx1, double df_apy1, double df_apx2, double df_apy2, BOOL shadow );
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
extern void utf8Upper(char* text);
extern int isInside(POINTF *A, POINTF *B, POINTF *C, POINTF *P, double *alpha, double *beta, double *gamma);
extern void Rotate_Point(double si, double co, double x1, double y1, /*center point*/ double x2, double y2, double *x, double *y); /*rotate point*/
//int draw_label(LINIA *L, LINIA *Le, double dx, double r1, double r2, double vpar, double precision, int bold, char *suffix, char *prefix);
//extern double r_magnitude;

static char T_text[64];

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
                T.kat = (Pi_ * (PL.kat + 180) / 180);
            else if ((Check_if_Equal2(dlx, 0.0) || Check_if_Equal2(dlx, lb)) && (L->x2 > L->x1))
                T.kat = (Pi_ * (PL.kat + 180) / 180);
            else
                T.kat = Pi_ * PL.kat / 180;
        }
        else {
            if ((Check_if_Equal2(dlx, PL1.dl) || Check_if_Equal2(dlx, le)) && (L->y1 > L->y2))
                T.kat = (Pi_ * (PL.kat + 180) / 180);
            else if ((Check_if_Equal2(dlx, 0.0) || Check_if_Equal2(dlx, lb)) && (L->y2 > L->y1))
                T.kat = (Pi_ * (PL.kat + 180) / 180);
            else
                T.kat = Pi_ * PL.kat / 180;
        }
        T.blok=1;
        //set_decimal_format(T.text, vpar, precision);
        T.dl = strlen(T.text);
        T.n = T18 + T.dl;
        normalize_txt(&T);
        ptr_t = dodaj_obiekt((BLOK *) dane, (void *) &T);
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

UNIT_FACTORS *unit_factors_pl;
/*
typedef struct {              //                                                      SI                            IMP
    double h_f; //depth of cross section in y axis                                   m //mm                         in
    double hm_f; //depth of cross section in y axis                                  m->m //mm->m                   in
    double A_f; //area in mm^2 of cross section                                      cm^2 -> m^2 //cm^2 -> mm^2     in^2
    double Am_f; //area in m^2 of cross section                                      m^2 -> m^2 //mm^2 -> m^2                    in^2
    double I_f; //aread moment of inertia                                            cm^4 -> m^4 //cm^4 -> mm^4                   in^4
    double Wm_f; //elastic section modulus in m^3                                    cm^3 -> m^3 //cm^3 -> m^3         in^3
    double Wmm_f; //elastic section modulus in m^3                                   cm^3 -> m^3 //cm^3 -> m^3         in^3  //not in use for plates
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

UNIT_FACTORS unit_factors_pl_si={0.001, 1.0, 0.0001, 1.0, 0.00000001, 0.000001, 0.000000001, 1.0e9,  1.0, 1.0, 1.0, 1000.0, 1000, 1000.0, 1000.0, .000001, 9.81, 0.001};
UNIT_FACTORS unit_factors_pl_imp={1.0, 1.0, 1.0,1.0,1.0, 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0, 1.0, 1000.0, 1.0, 1.0, 1.0};

extern PROP_PRECISIONS SI_precisions;
extern PROP_PRECISIONS IMP_precisions;
PROP_PRECISIONS *prop_precisions_pl=&SI_precisions;  //just to initialize

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

double dim_precision_pl=0.0001;

extern char *UNITS;
extern char *SI;
extern char *IMP;

char *unit;
char *_mm_="[mm]";
char *_inch_="[\"]";
char *_rad_="[rad]";
char *_MPa_="[MPa]";
char *_ksi_="[ksi]";

#define DEFLECTION_NUMBER 6
#define STRESS_NUMBER 7
#define EPSILON_NUMBER 6

static T_Hatch_Param 		    s_hatch_param = {1, 0, 1, 0, 0, 0} ;

static ST_PROPERTY *pl_property=NULL;
int pl_property_no=0;

static ST_PROPERTY prt_def_pl={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.85,0,0};

//static ST_LOAD_FACTORS *load_factors_pl=NULL;

static ST_LOAD_FACTORS *pl_load_factors=NULL;
int pl_load_factors_no=0;

static PL_NODE *pl_node=NULL;
static PL_NODE_EMB *pl_node_emb=NULL;
static PL_EDGE *pl_edge=NULL;
static PL_LOAD *pl_load=NULL;
static PL_LOAD *pl_point_load=NULL;
static GEO_LINE *geo_line=NULL;
LINIA Ldsp = Ldef;
LINIA Ldspm = Ldef;
LINIA Ldsp1 = Ldef;

#define MAX_L_BLOCKS 16
#define MAX_L_BLOCKS_LEN 64

static char **block_names=NULL;

static double pl_min_x, pl_min_y, pl_max_x, pl_max_y;
int theta_=0, sigma_eq_=0, epsilon_=0;
/*
 * typedef struct
{
    int number;
    int flag;
    double x;
    double y;
    double z;
} MESH_NODE;

typedef struct
{
    int number;
    int body_number;
    int index;
    int node1;
    int node2;
    int node3;
    int node4;
} MESH_ELEMENT;
 */

static MESH_NODE *mesh_node=NULL;
static MESH_ELEMENT *mesh_element=NULL;
static MESH_BOUNDARY *mesh_boundary=NULL;

static PLATE_PROPERTY *plate_property=NULL;
static PLATE_PROPERTY *hole_property=NULL;
static PLATE_PROPERTY *wall_property=NULL;
static PLATE_PROPERTY *zone_property=NULL;

int pl_node_no=0;
int pl_node_emb_no=0;
int pl_edge_no=0;
int plate_no=0;
int hole_no=0;
int wall_no=0;
int zone_no=0;
int pl_load_no=0;  //uniforme
int pl_point_load_no=0;
int mesh_node_no=0;
int mesh_element_no=0;
int mesh_boundary_no=0;
int geo_line_no=0;


int pllc_node_force_moment_no=0;
int pllc_uniform_load_no=0;

int PL_PROPERTY_MAX=10;
int PL_LOAD_FACTORS_MAX=100;
int PL_NODE_MAX=100;
int PL_NODE_EMB_MAX=10;
int PL_EDGE_MAX=100;
int PL_LOAD_MAX=10;
int PL_POINT_LOAD_MAX=10;
int PL_PLATE_MAX=10;
int PL_HOLE_MAX=10;
int PL_WALL_MAX=10;
int PL_ZONE_MAX=10;
int GEO_LINE_MAX=1000;

float dxl=0.1f;
float dxr=0.25f;

float dxl_min=0.1f;


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

COMBINATION *ULS_SLS_LC;

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

//float dxr_min=0.1f;
/*
unsigned char str2load(char *ptr)
{
    char *loads[]={"", "%DL", "%LL", "%Lr", "%WL", "%SL", "%EL", "%RL", "%HL", "%FL", "%TL"};
    for (int i=1; i<11; i++)
    {
        if (strstr(ptr, loads[i])!=NULL) return (unsigned char)i;
    }
    return (unsigned char)0;
}
*/
#define Utf8Char unsigned char	// must be 1 byte, 8 bits, can be char, the UTF consortium specify unsigned
extern Utf8Char* Utf8StrMakeUprUtf8Str(const Utf8Char *pUtf8);

void add_load_factors_pl(void)
{
    pl_load_factors_no++;
    if (pl_load_factors_no==PL_LOAD_FACTORS_MAX)
    {
        PL_LOAD_FACTORS_MAX+=100;
        pl_load_factors=realloc(pl_load_factors, PL_LOAD_FACTORS_MAX * sizeof(ST_LOAD_FACTORS));
    }

    memmove(&pl_load_factors[pl_load_factors_no], &load_factors[0], sizeof(ST_LOAD_FACTORS));
}

void add_property_pl(void)
{
    pl_property_no++;
    if (pl_property_no==PL_PROPERTY_MAX) {
        PL_PROPERTY_MAX+=10;
        pl_property=realloc(pl_property, PL_PROPERTY_MAX * sizeof(ST_PROPERTY));
    }
    memmove(&pl_property[pl_property_no], &prt_def_pl, sizeof(ST_PROPERTY));
    //if (prop_precisions_pl == &SI_precisions)
    if (UNITS==SI) pl_property[pl_property_no].c=0.03; //[m]
    else pl_property[pl_property_no].c=1.18; //[in]
}

void add_node_pl(void)
{
    pl_node_no++;
    if (pl_node_no==PL_NODE_MAX) {
        PL_NODE_MAX+=100;
        pl_node=realloc(pl_node, PL_NODE_MAX * sizeof(PL_NODE));
    }
}

void add_node_emb_pl(void)
{
    pl_node_emb_no++;
    if (pl_node_emb_no==PL_NODE_EMB_MAX) {
        PL_NODE_EMB_MAX+=10;
        pl_node_emb=realloc(pl_node, PL_NODE_EMB_MAX * sizeof(PL_NODE_EMB));
    }
}

void add_edge(void)
{
    pl_edge_no++;
    if (pl_edge_no==PL_EDGE_MAX) {
        PL_EDGE_MAX+=100;
        pl_edge=realloc(pl_edge, PL_EDGE_MAX * sizeof(PL_EDGE));
    }
}

void add_plate_property(void)
{
    plate_no++;
    if (plate_no==PL_PLATE_MAX) {
        PL_PLATE_MAX+=10;
        plate_property=realloc(plate_property, PL_PLATE_MAX * sizeof(PLATE_PROPERTY));
    }
}

void add_hole_property(void)
{
    hole_no++;
    if (hole_no==PL_HOLE_MAX) {
        PL_HOLE_MAX+=10;
        hole_property=realloc(hole_property, PL_HOLE_MAX * sizeof(PLATE_PROPERTY));
    }
}

void add_wall_property(void)
{
    wall_no++;
    if (wall_no==PL_WALL_MAX) {
        PL_WALL_MAX+=10;
        wall_property=realloc(wall_property, PL_WALL_MAX * sizeof(PLATE_PROPERTY));
    }
}

void add_zone_property(void)
{
    zone_no++;
    if (zone_no==PL_ZONE_MAX) {
        PL_ZONE_MAX+=10;
        zone_property=realloc(zone_property, PL_ZONE_MAX * sizeof(PLATE_PROPERTY));
    }
}

void add_load_pl(void)
{
    pl_load_no++;
    if (pl_load_no==PL_LOAD_MAX) {
        PL_LOAD_MAX+=10;
        pl_load=realloc(pl_load, PL_LOAD_MAX * sizeof(PL_LOAD));
    }
}

void add_point_load_pl(void)
{
    pl_point_load_no++;
    if (pl_point_load_no==PL_POINT_LOAD_MAX) {
        PL_POINT_LOAD_MAX+=10;
        pl_point_load=realloc(pl_point_load, PL_POINT_LOAD_MAX * sizeof(PL_LOAD));
    }
}

void add_geo_line(void)
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

int create_plate(BLOK *b, int style, int number, int body_no, int *first, int *last, int *property_no, BOOL *is_closed) {
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
    *first = pl_edge_no;

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

            pl_edge[pl_edge_no].style = style;
            pl_edge[pl_edge_no].number = number;
            pl_edge[pl_edge_no].body_number = body_no;
            pl_edge[pl_edge_no].node1 = -1;
            pl_edge[pl_edge_no].node2 = -1;
            pl_edge[pl_edge_no].node3 = -1;
            pl_edge[pl_edge_no].type = 0;  //line
            pl_edge[pl_edge_no].inverted = 0;

            for (i = 0; i < pl_node_no; i++)  //node1
            {
                if ((Check_if_Equal(L->x1, pl_node[i].x)) && (Check_if_Equal(L->y1, pl_node[i].y))) {
                    pl_edge[pl_edge_no].node1 = i;
                    if (pl_node[i].d > dl) pl_node[i].d = dl;  //shrinking the division for FE
                    break;
                }
            }
            for (i = 0; i < pl_node_no; i++)  //node2
            {
                if ((Check_if_Equal(L->x2, pl_node[i].x)) && (Check_if_Equal(L->y2, pl_node[i].y))) {
                    pl_edge[pl_edge_no].node2 = i;
                    if (pl_node[i].d > dl) pl_node[i].d = dl; //shrinking the division for FE
                    break;
                }
            }

            if (pl_edge[pl_edge_no].node1 == -1) {  //not found
                pl_node[pl_node_no].x = L->x1;
                pl_node[pl_node_no].y = L->y1;
                pl_node[pl_node_no].d = dl; //setting the division for FE
                pl_edge[pl_edge_no].node1 = pl_node_no;
                add_node_pl();
            }
            if (pl_edge[pl_edge_no].node2 == -1) {  //not found
                pl_node[pl_node_no].x = L->x2;
                pl_node[pl_node_no].y = L->y2;
                pl_node[pl_node_no].d = dl; //setting the division for FE
                pl_edge[pl_edge_no].node2 = pl_node_no;
                add_node_pl();
            }

            if (style == 2)  //wall
            {
                pl_edge[pl_edge_no].restraint = max(6, L->obiektt2);  //6 is default
            }
            else pl_edge[pl_edge_no].restraint = L->obiektt2;  //could be check logically, 0, 5,6 or 7
            //// to match with next element
            last_x = L->x2;
            last_y = L->y2;

            if (style == 0)  //plate
            {
                line_place(L, &pl_min_x, &pl_min_y, &pl_max_x, &pl_max_y);
            }

            add_edge();

            el_no++;
            adp += sizeof(NAGLOWEK) + L->n;

        } else if (nag->obiekt == Oluk) {
            l = (LUK *) nag;
            dr = l->r * dxr;

            pl_edge[pl_edge_no].node1 = -1;
            pl_edge[pl_edge_no].node2 = -1;
            pl_edge[pl_edge_no].node3 = -1;
            pl_edge[pl_edge_no].type = 1;  //arc

            //calculating end points
            x1 = l->x + l->r * cos(l->kat1);
            y1 = l->y + l->r * sin(l->kat1);
            x2 = l->x + l->r * cos(l->kat2);
            y2 = l->y + l->r * sin(l->kat2);

            if ((Check_if_Equal(x1, last_x) == TRUE) && (Check_if_Equal(y1, last_y) == TRUE))
                pl_edge[pl_edge_no].inverted = 0;
            else pl_edge[pl_edge_no].inverted = 1;

            for (i = 0; i < pl_node_no; i++)  //node1
            {
                if ((Check_if_Equal(x1, pl_node[i].x)) && (Check_if_Equal(y1, pl_node[i].y))) {
                    pl_edge[pl_edge_no].node1 = i;
                    if (pl_node[i].d > dr) pl_node[i].d = dr; //shrinking the division for FE
                    break;
                }
            }
            for (i = 0; i < pl_node_no; i++)  //node2
            {
                if ((Check_if_Equal(l->x, pl_node[i].x)) && (Check_if_Equal(l->y, pl_node[i].y))) {
                    pl_edge[pl_edge_no].node2 = i;
                    if (pl_node[i].d > dr) pl_node[i].d = dr; //shrinking the division for FE
                    break;
                }
            }
            for (i = 0; i < pl_node_no; i++)  //node3
            {
                if ((Check_if_Equal(x2, pl_node[i].x)) && (Check_if_Equal(y2, pl_node[i].y))) {
                    pl_edge[pl_edge_no].node3 = i;
                    if (pl_node[i].d > dr) pl_node[i].d = dr; //shrinking the division for FE
                    break;
                }
            }

            if (pl_edge[pl_edge_no].node1 == -1) {  //not found
                pl_node[pl_node_no].x = x1;
                pl_node[pl_node_no].y = y1;
                pl_node[pl_node_no].d = dr; //setting the division for FE
                pl_edge[pl_edge_no].node1 = pl_node_no;
                add_node_pl();
            }

            if (pl_edge[pl_edge_no].node2 == -1) {  //not found
                pl_node[pl_node_no].x = l->x;
                pl_node[pl_node_no].y = l->y;
                pl_node[pl_node_no].d = dr; //setting the division for FE
                pl_edge[pl_edge_no].node2 = pl_node_no;
                add_node_pl();
            }

            if (pl_edge[pl_edge_no].node3 == -1) {  //not found
                pl_node[pl_node_no].x = x2;
                pl_node[pl_node_no].y = y2;
                pl_node[pl_node_no].d = dr; //setting the division for FE
                pl_edge[pl_edge_no].node3 = pl_node_no;
                add_node_pl();
            }

            //pl_edge[pl_edge_no].inverted = 0;  //has to be confronted with the endpoint of previous segment

            pl_edge[pl_edge_no].restraint = L->obiektt2;  //could be check logically, 0, 5,6 or 7
            //// to match with next element

            if (pl_edge[pl_edge_no].inverted==1)
            {
                last_x = x1;
                last_y = y1;
            }
            else
            {
                last_x = x2;
                last_y = y2;
            }

            if (style == 0)  //plate
            {
                luk_place(l, &pl_min_x, &pl_min_y, &pl_max_x, &pl_max_y);
            }

            add_edge();

            el_no++;
            adp += sizeof(NAGLOWEK) + l->n;
        } else //some garbage
        {
            adp += sizeof(NAGLOWEK) + nag->n;
        }
    }
    *last = pl_edge_no;

    if ((style == 0) || (style == 3)) {
        if (t_no == 0) {
            ////TO DO ERROR MESSAGE
            return 0;
        }
    }

    if (el_no > 2) {
        //check if closed
        if ((pl_edge[*first].node1==pl_edge[*last-1].node2) ||
            (pl_edge[*first].node1==pl_edge[*last-1].node1) ||
            (pl_edge[*first].node2==pl_edge[*last-1].node1) ||
            (pl_edge[*first].node2==pl_edge[*last-1].node2))
            *is_closed=TRUE;

        return 1;
    }
    else if (style==2)
    {
        if (el_no>0) return 1;
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
    BLOK *ptrs_block, *buf_block;
    int  len_type, len_desc;
    T_Desc_Ex_Block *ptrs_desc_bl;

    len_type = (int) strlen(blok_type) + 1;
    len_desc = sizeof(unsigned) + 2 * sizeof(float) + sizeof(len_type) + len_type;
    size_block += len_desc;

    if (NULL == (buf_block = (BLOK*)malloc(sizeof(NAGLOWEK) + size_block)))   //100 is just for a case
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

void my_free(void* adr)
{
    return;
}

static int factor_record(unsigned char load, unsigned char variant)
{
    for (int i=0; i<st_load_factors_no; i++)
    {
        if (pl_load_factors[i].load==load)
        {
            if (variant>0)
            {
                if (pl_load_factors[i].variant==variant)
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

char *chapters[]={u8"δ.x", u8"δ.y", u8"δ.z", u8"θ.xx", u8"θ.yy", u8"θ.zz", u8"σ.x", u8"σ.y", u8"σ.z", u8"τ.xy", u8"τ.xz", u8"τ.yz", u8"σ.eq",
                  u8"ε.xx", u8"ε.yy", u8"ε.zz", u8"ε.xy", u8"ε.xz", u8"ε.yz"};

void modify_chapter(char *chapter)
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

void embed_node(int load_no, double geo_units_factor)
{
    int k;
//check if point must be embeded
    for (k = 0; k < pl_node_no; k++)
    {
        if ((Check_if_Equal(pl_node[k].x, pl_load[load_no].x1) == TRUE) &&
            (Check_if_Equal(pl_node[k].y, pl_load[load_no].y1) == TRUE))
            break;  //no need to embed
    }
    if (k == pl_node_no)  //checking if already embeded
    {
        for (k = 0; k < pl_node_emb_no; k++) {
            if ((Check_if_Equal(pl_node_emb[k].x, pl_load[load_no].x1) == TRUE) &&
                (Check_if_Equal(pl_node_emb[k].y, pl_load[load_no].y1) == TRUE))
                break;  //no need to embed
        }
    }
    if (k == pl_node_emb_no)  //need to embed
    {
        pl_node_emb[pl_node_emb_no].x = pl_load[load_no].x1;
        pl_node_emb[pl_node_emb_no].y = pl_load[load_no].y1;
        pl_node_emb[pl_node_emb_no].d = (float)(dxl_min / geo_units_factor);  //for compatibility
        pl_node_emb[pl_node_emb_no].body = pl_load[load_no].point_body;
        pl_node_emb[pl_node_emb_no].body_no = pl_load[load_no].point_body_no;
        add_node_emb_pl();
    }
}

//#include <math.h>
//#include <stdio.h>

/**
 * Calculate reinforcement areas (As, As_prime), ratios (rho, rho_prime, total_rho),
 * and pseudo-elastic stress (sigma_c_max) for a rectangular RC section under Eurocode 2,
 * handling cases where K > 0.283 (doubly reinforced with x = c) or K <= 0.283 (singly reinforced).
 * Assumes N = 0 for pure bending. Units: m, Pa, N, Nm.
 *
 * Parameters:
 *   M: Design moment (Nm)
 *   h: Section height (m)
 *   b: Section width (m)
 *   c: Concrete cover (m, same top/bottom)
 *   fck: Characteristic concrete compressive strength (Pa)
 *   fcd: Design concrete compressive strength (Pa, typically fck/1.5)
 *   fyd: Design steel yield strength (Pa, typically fyk/1.15)
 *   eta: Stress block factor (typically 1.0 for fck <= 50 MPa)
 *   lambda: Stress block depth factor (typically 0.8 for fck <= 50 MPa)
 *   Es: Steel modulus (Pa, typically 200e9)
 *   As: Pointer to store tension rebar area (m²)
 *   As_prime: Pointer to store compression rebar area (m²)
 *   rho: Pointer to store tension reinforcement ratio
 *   rho_prime: Pointer to store compression reinforcement ratio
 *   total_rho: Pointer to store total reinforcement ratio (rho + rho_prime)
 *   sigma_c_max: Pointer to store pseudo-elastic compressive stress (Pa)
 *
 * Returns:
 *   0 on success, -1 if infeasible (negative reinforcement areas or invalid neutral axis)
 */
int calculate_rebars_eurocode_simplified(double M, double h, double b, double cc,
                              double fck, double fcd, double fyd,
                              double eta, double lambda, double Es, int use_min_reinf,
                              double *As, double *As_prime, double *rho, double *rho_prime,
                              double *total_rho, double *sigma_c_max) {
    double d = h - cc; // Effective depth
    double d_prime = cc; // Compression rebar depth
    double Ecm = 22.0 * pow((fck * 1e-6 + 8.0) / 10.0, 0.3) * 1e9; // Pa
    double n = Es / Ecm; // Modular ratio
    double fctm = 0.3 * pow(fck * 1e-6, 2.0 / 3.0) * 1e6; // Pa  ////due to use_min_reinf
    double As_min = use_min_reinf ? fmax(0.26 * fctm / fyd * b * d, 0.0013 * b * d) : 0.0;  //// due to  use_min_reinf
    double K = M / (b * d * d * fck);

    if (K <= 0.283) {
        // Singly reinforced
        double sqrt_term = sqrt(1.0 - 3.53 * K);
        if (sqrt_term < 0) {
            printf("Error: Negative sqrt for K = %.4f.\n", K);
            return -1;
        }
        double z = d * (1.0 + sqrt_term) / 2.0;
        if (z > 0.95 * d) z = 0.95 * d;
        *As = M / (fyd * z);

        *As = fmax(*As, As_min);  //// due to  use_min_reinf

        *As_prime = 0.0;
        *rho = *As / (b * d);
        *rho_prime = 0.0;
        *total_rho = *rho + *rho_prime;

        // Pseudo-elastic stress with x = d - z
        double x = d - z; // Neutral axis from compression face
        double m = n * (*rho); // Tension reinforcement only
        double I_cr = (b * x * x * x) / 3.0 + m * b * d * (d - x) * (d - x);
        double A_cr = b * x + m * b * d;
        *sigma_c_max = M * x / I_cr;

        // Check limits
#ifndef NDEBUG
        if (*total_rho > 0.02) {
            printf("Warning: Total reinforcement ratio (%.2f%%) exceeds 2%%.\n", *total_rho * 100);
        }
        if (*sigma_c_max > fcd) {
            printf("Warning: sigma_c_max (%.2f MPa) exceeds fcd (%.2f MPa).\n",
                   *sigma_c_max / 1e6, fcd / 1e6);
        }
        printf("Singly reinforced: As = %.2f mm², rho = %.2f%%, sigma_c_max = %.2f MPa\n",
               *As * 1e6, *rho * 100, *sigma_c_max / 1e6);
#endif
        return 0;
    } else {
        // Doubly reinforced (K > 0.283, use x = cc)
        double x = cc;
        double concrete_force = eta * fcd * lambda * x * b;
        double As_term = concrete_force / fyd;
        double lever_arm_concrete = h / 2.0 - (lambda * x) / 2.0;
        double lever_arm_As_prime = h / 2.0 - x;
        double lever_arm_As = d - h / 2.0;
        double concrete_moment = concrete_force * lever_arm_concrete;
        double coeff_As_prime = fyd * (lever_arm_As_prime + lever_arm_As);
        double constant_term = concrete_moment + fyd * lever_arm_As * As_term;
        *As_prime = (M - constant_term) / coeff_As_prime;
        *As = *As_prime + As_term;

        /*
        if (*As < 0 || *As_prime < 0) {
            printf("Error: Negative reinforcement areas (As = %.2f mm², As_prime = %.2f mm²).\n",
                   *As * 1e6, *As_prime * 1e6);
            return -1;
        }
        */
        //// due to  use_min_reinf
        if (*As_prime < 0) {
            *As_prime = 0.0;
            *As = fmax(As_term, As_min);
        }

        if (*As < 0) {
            printf("Error: Negative reinforcement area (As = %.2f mm²).\n", *As * 1e6);
            return -1;
        }

        *rho = *As / (b * d);
        *rho_prime = *As_prime / (b * d);
        *total_rho = *rho + *rho_prime;
        double m = n * (*rho); // Tension reinforcement only
        double I_cr = (b * x * x * x) / 3.0 + m * b * d * (d - x) * (d - x);
        double A_cr = b * x + m * b * d;
        *sigma_c_max = M * x / I_cr;

        // Check limits
#ifndef NDEBUG
        if (*total_rho > 0.02) {
            printf("Warning: Total reinforcement ratio (%.2f%%) exceeds 2%%.\n", *total_rho * 100);
        }
        if (*sigma_c_max > fcd) {
            printf("Warning: sigma_c_max (%.2f MPa) exceeds fcd (%.2f MPa).\n",
                   *sigma_c_max / 1e6, fcd / 1e6);
        }
        printf("Doubly reinforced: As = %.2f mm², As_prime = %.2f mm², rho = %.2f%%, rho_prime = %.2f%%, total_rho = %.2f%%, sigma_c_max = %.2f MPa\n",
               *As * 1e6, *As_prime * 1e6, *rho * 100, *rho_prime * 100, *total_rho * 100, *sigma_c_max / 1e6);
#endif
        return 0;
    }
}

/**
 * Example usage with test data
 */
int foo_test_p_eurocode() {
    // Test case: K > 0.283
    double M = 150000.0; // Nm (adjusted to force K > 0.283)
    double h = 0.1;      // m
    double b = 2.0;      // m
    double cc = 0.02;     // m
    double fck = 30e6;   // Pa
    double fcd = 20e6;   // Pa
    double fyd = 350e6;  // Pa
    double eta = 1.0;
    double lambda = 0.8;
    double Es = 200e9;   // Pa
    double As, As_prime, rho, rho_prime, total_rho, sigma_c_max;
    int use_min_reinf = 0;

    int result = calculate_rebars_eurocode_simplified(M, h, b, cc, fck, fcd, fyd, eta, lambda, Es, use_min_reinf,
                                           &As, &As_prime, &rho, &rho_prime, &total_rho, &sigma_c_max);
    if (result == 0) {
        printf("Calculation successful.\n");
    } else {
        printf("Calculation failed.\n");
    }

    return 0;
}

//#include <math.h>
//#include <stdio.h>

/**
 * Simplified ACI 318 procedure for pure bending (N=0) in a plate (per 1 m width) to calculate
 * reinforcement areas (As, As_prime), ratios (rho, rho_prime, total_rho), and pseudo-elastic
 * stress (sigma_c_max). Handles sqrt_term < 0 by switching to doubly reinforced with c_na = c.
 * Units: m, Pa, N, Nm/m (coherent SI).
 *
 * Parameters:
 *   M: Factored moment per meter (Nm/m)
 *   h: Section height (m)
 *   c: Concrete cover (m, same top/bottom)
 *   fyk: Characteristic steel yield strength (Pa)
 *   fyd: Design steel yield strength (Pa, typically fyk for ACI)
 *   fck: Characteristic concrete compressive strength (Pa)
 *   fcd: Design concrete compressive strength (Pa, typically fck for ACI stress block)
 *   As: Pointer to store tension rebar area (m²/m)
 *   As_prime: Pointer to store compression rebar area (m²/m)
 *   rho: Pointer to store tension reinforcement ratio
 *   rho_prime: Pointer to store compression reinforcement ratio
 *   total_rho: Pointer to store total reinforcement ratio (rho + rho_prime)
 *   sigma_c_max: Pointer to store pseudo-elastic compressive stress (Pa)
 *
 * Returns:
 *   0 on success, -1 if infeasible (negative reinforcement areas or invalid neutral axis)
 */
int calculate_rebars_aci_simplified_SI(double M, double h, double cc,
                                    double fyk, double fyd, double fck, double fcd,
                                    double *As, double *As_prime, double *rho, double *rho_prime,
                                    double *total_rho, double *sigma_c_max)
{
    double b = 1.0; // Width per meter for plate
    double d = h - cc; // Effective depth
    double d_prime = cc; // Compression rebar depth
    double fi = 0.9; // Phi for tension-controlled
    double Es = 200e9; // Steel modulus (Pa)
    double Ec = 4700 * sqrt(fck / 1e6) * 1e6; // Concrete modulus (Pa)
    double n = Es / Ec; // Modular ratio
    double beta1 = (fck <= 28e6) ? 0.85 : 0.85 - 0.05 * (fck / 1e6 - 28) / 4; // ACI 318-19
    if (beta1 < 0.65) beta1 = 0.65;

    // Step 1: Try singly reinforced
    double R = M / (fi * b * d * d);
    double sqrt_term = 1.0 - 2.0 * R / (0.85 * fck);

    double c_na;

    if (sqrt_term >= 0.0)
    {
        // Singly reinforced
        *rho = (0.85 * fck / fyk) * (1.0 - sqrt(sqrt_term));
        *As = *rho * b * d;
        *As_prime = 0.0;
        *rho_prime = 0.0;
        *total_rho = *rho + *rho_prime;

        // Check tension-controlled (epsilon_s >= 0.005)
        c_na = *rho * fyk * d / (0.85 * fck * beta1); // c = a / beta1
        double epsilon_s = 0.003 * (d - c_na) / c_na;
        if (epsilon_s < 0.005) {
#ifndef NDEBUG
            printf("Warning: Section not tension-controlled (epsilon_s = %.6f < 0.005).\n", epsilon_s);
#endif
            fi = 0.65 + (epsilon_s - 0.002) * (0.9 - 0.65) / (0.005 - 0.002);
            if (fi < 0.65) fi = 0.65;
            // Recalculate with adjusted phi
            R = M / (fi * b * d * d);
            sqrt_term = 1.0 - 2.0 * R / (0.85 * fck);
            if (sqrt_term < 0.0) goto doubly_reinforced;
            *rho = (0.85 * fck / fyk) * (1.0 - sqrt(sqrt_term));
            *As = *rho * b * d;
            c_na = *rho * fyk * d / (0.85 * fck * beta1);
        }

        // Pseudo-elastic stress
        double m = n * (*rho); // Tension reinforcement only
        double I_cr = (b * c_na * c_na * c_na) / 3.0 + m * b * d * (d - c_na) * (d - c_na);
        double A_cr = b * c_na + m * b * d;
        *sigma_c_max = M * c_na / I_cr;

        // Check limits
        double rho_max = 0.75 * 0.85 * beta1 * (fck / fyk) * (0.003 / (0.003 + 0.005));
#ifndef NDEBUG
        if (*total_rho > rho_max || *total_rho > 0.02) {
            printf("Warning: Total reinforcement ratio (%.2f%%) exceeds limits (rho_max = %.2f%% or 2%%).\n",
                   *total_rho * 100, rho_max * 100);
        }
        if (*sigma_c_max > 0.85 * fck) {
            printf("Warning: sigma_c_max (%.2f MPa) exceeds 0.85*fck (%.2f MPa).\n",
                   *sigma_c_max / 1e6, 0.85 * fck / 1e6);
        }
        printf("Singly reinforced: As = %.2f mm²/m, rho = %.2f%%, sigma_c_max = %.2f MPa\n",
               *As * 1e6, *rho * 100, *sigma_c_max / 1e6);
#endif
        return 0;
    }

doubly_reinforced:
    /* Step 2: Doubly reinforced (sqrt_term < 0, use c_na = c)*/
    c_na = cc;
    double concrete_force = 0.85 * fcd * beta1 * cc * b;
    double As_term = concrete_force / fyd;
    double lever_arm_concrete = h / 2.0 - (beta1 * cc) / 2.0;
    double lever_arm_As_prime = h / 2.0 - cc;
    double lever_arm_As = d - h / 2.0;
    double concrete_moment = concrete_force * lever_arm_concrete;
    double coeff_As_prime = fyd * (lever_arm_As_prime + lever_arm_As);
    double constant_term = concrete_moment + fyd * lever_arm_As * As_term;
    *As_prime = (M - constant_term) / coeff_As_prime;
    *As = *As_prime + As_term;

    if (*As < 0 || *As_prime < 0) {
        printf("Error: Negative reinforcement areas (As = %.2f mm²/m, As_prime = %.2f mm²/m).\n",
               *As * 1e6, *As_prime * 1e6);
        return -1;
    }

    *rho = *As / (b * d);
    *rho_prime = *As_prime / (b * d);
    *total_rho = *rho + *rho_prime;
    double m = n * (*rho); // Tension reinforcement only
    double I_cr = (b * c_na * c_na * c_na) / 3.0 + m * b * d * (d - c_na) * (d - c_na);
    double A_cr = b * c_na + m * b * d;
    *sigma_c_max = M * c_na / I_cr;

    // Check limits
    double rho_max = 0.75 * 0.85 * beta1 * (fck / fyk) * (0.003 / (0.003 + 0.005));

#ifndef NDEBUG
    if (*total_rho > rho_max || *total_rho > 0.02) {
        printf("Warning: Total reinforcement ratio (%.2f%%) exceeds limits (rho_max = %.2f%% or 2%%).\n",
               *total_rho * 100, rho_max * 100);
    }
    if (*sigma_c_max > 0.85 * fck) {
        printf("Warning: sigma_c_max (%.2f MPa) exceeds 0.85*fck (%.2f MPa).\n",
               *sigma_c_max / 1e6, 0.85 * fck / 1e6);
    }
    printf("Doubly reinforced: As = %.2f mm²/m, As_prime = %.2f mm²/m, rho = %.2f%%, rho_prime = %.2f%%, total_rho = %.2f%%, sigma_c_max = %.2f MPa\n",
           *As * 1e6, *As_prime * 1e6, *rho * 100, *rho_prime * 100, *total_rho * 100, *sigma_c_max / 1e6);
#endif
    return 0;
}

/**
 * Example usage with provided data
 */
int foo_p_aci() {
    // Test case
    double M = 218000.0; // Nm/m
    double h = 0.15;     // m (adjusted for d = 0.13)
    double c = 0.02;     // m
    double fyk = 350e6;  // Pa
    double fyd = 350e6;  // Pa (fyk for ACI)
    double fck = 30e6;   // Pa
    double fcd = 30e6;   // Pa (fck for ACI stress block)
    double As, As_prime, rho, rho_prime, total_rho, sigma_c_max;

    int result = calculate_rebars_aci_simplified_SI(M, h, c, fyk, fyd, fck, fcd,
                                                 &As, &As_prime, &rho, &rho_prime, &total_rho, &sigma_c_max);
    if (result == 0) {
        printf("Calculation successful.\n");
    } else {
        printf("Calculation failed.\n");
    }

    return 0;
}


//#include <math.h>
//#include <stdio.h>

// Simplified ACI 318-19 procedure for RC section (SI: Pa, m, Nm; Imperial: psi, in, lbf-in)
int calculate_rebars_aci_simplified_UNI_with_N(double M, double N, double h, double b, double c,
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

// Example test
int foo_test_with_N() {
    double M_si = 37919.83, N_si = -47070.49; // Nm, N
    double M_imp = 335573.0, N_imp = -10580.0; // lbf-in, lbf
    double h = 0.3, b = 0.3, c = 0.04; // m (SI) or in (Imperial)
    double fck_si = 30e6, fcd_si = 20e6, fyd_si = 350e6; // Pa
    double fck_imp = 4350, fcd_imp = 2900, fyd_imp = 60000; // psi
    double eta = 1.0, lambda = 0.8, Es = 200e9; // Es in Pa (SI) or psi (Imperial)
    int use_min_reinf = 1;
    double As, As_prime, rho, rho_prime, total_rho, sigma_c_max;

    // SI test
    printf("SI Units Test:\n");
    calculate_rebars_aci_simplified_UNI_with_N(M_si, N_si, h, b, c, fck_si, fcd_si, fyd_si, eta, lambda, Es,
                                    use_min_reinf, 1, &As, &As_prime, &rho, &rho_prime, &total_rho, &sigma_c_max);

    // Imperial test
    printf("\nImperial Units Test:\n");
    calculate_rebars_aci_simplified_UNI_with_N(M_imp, N_imp, h, b, c, fck_imp, fcd_imp, fyd_imp, eta, lambda, Es,
                                    use_min_reinf, 0, &As, &As_prime, &rho, &rho_prime, &total_rho, &sigma_c_max);
    return 0;
}


//#include <math.h>
//#include <stdio.h>

/**
 * Simplified ACI 318 procedure for pure bending (N=0) in a plate (per 1 m or 1 ft width) to calculate
 * reinforcement areas (As, As_prime), ratios (rho, rho_prime, total_rho), and pseudo-elastic
 * stress (sigma_c_max). Handles sqrt_term < 0 by switching to doubly reinforced with c_na = c.
 * Supports SI (m, Nm/m, Pa) or Imperial (in, in-lbf/ft, psi) units based on fyk magnitude.
 *
 * Parameters:
 *   M: Factored moment per meter (Nm/m) or per foot (in-lbf/ft)
 *   h: Section height (m or in)
 *   c: Concrete cover (m or in, same top/bottom)
 *   fyk: Characteristic steel yield strength (Pa or psi)
 *   fyd: Design steel yield strength (Pa or psi, typically fyk for ACI)
 *   fck: Characteristic concrete compressive strength (Pa or psi)
 *   fcd: Design concrete compressive strength (Pa or psi, typically fck for ACI stress block)
 *   As: Pointer to store tension rebar area (m²/m or in²/ft)
 *   As_prime: Pointer to store compression rebar area (m²/m or in²/ft)
 *   rho: Pointer to store tension reinforcement ratio
 *   rho_prime: Pointer to store compression reinforcement ratio
 *   total_rho: Pointer to store total reinforcement ratio (rho + rho_prime)
 *   sigma_c_max: Pointer to store pseudo-elastic compressive stress (Pa or psi)
 *
 * Returns:
 *   0 on success, -1 if infeasible (negative reinforcement areas or invalid neutral axis)
 */
int calculate_rebars_aci_simplified_UNI(double M, double h, double b, double cc,
                                    double fyk, double fyd, double fck, double fcd, int use_min_reinf,
                                    double *As, double *As_prime, double *rho, double *rho_prime,
                                    double *total_rho, double *sigma_c_max) {
    // Detect unit system based on fyk magnitude
    int is_si_units = (fyk > 1e8); // Pa (>100 MPa) for SI, psi for Imperial
    ////double b = is_si_units ? 1.0 : 12.0; // 1 m for SI, 12 in (1 ft) for Imperial
    double unit_factor = is_si_units ? 1e6 : 1.0; // Convert Pa to MPa or psi to psi
    double area_display_factor = is_si_units ? 1e6 : 1.0; // m²/m to mm²/m or in²/ft
    const char *length_unit = is_si_units ? "m" : "ft";
    const char *area_unit = is_si_units ? "mm²/m" : "in²/ft";
    const char *stress_unit = is_si_units ? "MPa" : "psi";

    double d = h - cc; // Effective depth
    double d_prime = cc; // Compression rebar depth
    double fi = 0.9; // Phi for tension-controlled
    double Es = is_si_units ? 200e9 : 29e6; // 200 GPa or 29e6 psi
    double Ec = is_si_units ? 4700 * sqrt(fck / 1e6) * 1e6 : 57000 * sqrt(fck); // Pa or psi
    double n = Es / Ec; // Modular ratio


    //double f_ctm = 0.3 * pow(fck / 1e6, 2.0 / 3.0) * 1e6; // Eurocode f_ctm for C30/37  ////due to use_min_reinf
    //double As_min = use_min_reinf ? fmax(0.26 * f_ctm / fyd * b * d, 0.0013 * b * d) : 0.0; // Eurocode As_min  ////due to use_min_reinf
    // Calculate f_ctm and As_min based on unit system
    double f_ctm, As_min;
    if (is_si_units) {
        f_ctm = 0.3 * pow(fck / 1e6, 2.0 / 3.0) * 1e6; // f_ctm in Pa
        // Use Eurocode formula for preliminary design
        As_min = use_min_reinf ? fmax(0.26 * f_ctm / fyd * b * d, 0.0013 * b * d) : 0.0; // As_min in m²
        // Optional: ACI 318-19 formula (uncomment to use)
        // As_min = use_min_reinf ? fmax(0.249 * sqrt(fck / 1e6) / (fyd / 1e6) * b * d, 1.379 / (fyd / 1e6) * b * d) : 0.0; // As_min in m²
    } else {
        double fck_mpa = fck / 145.038; // Convert fck from psi to MPa
        f_ctm = 0.3 * pow(fck_mpa, 2.0 / 3.0) * 145.038; // f_ctm in psi
        // Use Eurocode formula for preliminary design
        As_min = use_min_reinf ? fmax(0.26 * f_ctm / fyd * b * d, 0.0013 * b * d) : 0.0; // As_min in in²
        // Optional: ACI 318-19 formula (uncomment to use)
        // As_min = use_min_reinf ? fmax(3.0 * sqrt(fck) / fyd * b * d, 200.0 / fyd * b * d) : 0.0;
    }

    double beta1 = (fck / unit_factor <= 28.0) ? 0.85 : 0.85 - 0.05 * (fck / unit_factor - 28.0) / 4.0; // ACI 318-19
    if (beta1 < 0.65) beta1 = 0.65;

    // Step 1: Try singly reinforced
    double R = M / (fi * b * d * d);
    double sqrt_term = 1.0 - 2.0 * R / (0.85 * fck);

    double c_na;

    if (sqrt_term >= 0.0) {
        // Singly reinforced
        *rho = (0.85 * fck / fyk) * (1.0 - sqrt(sqrt_term));
        *As = *rho * b * d;

        *As = fmax(*As, As_min); ////due to use_min_reinf
        *rho = *As / (b * d);  ////due to use_min_reinf

        *As_prime = 0.0;
        *rho_prime = 0.0;
        *total_rho = *rho + *rho_prime;

        // Check tension-controlled (epsilon_s >= 0.005)
        c_na = *rho * fyk * d / (0.85 * fck * beta1); // c = a / beta1
        double epsilon_s = 0.003 * (d - c_na) / c_na;
        if (epsilon_s < 0.005) {
#ifndef NDEBUG
            printf("Warning: Section not tension-controlled (epsilon_s = %.6f < 0.005).\n", epsilon_s);
#endif
            fi = 0.65 + (epsilon_s - 0.002) * (0.9 - 0.65) / (0.005 - 0.002);
            if (fi < 0.65) fi = 0.65;
            // Recalculate with adjusted phi
            R = M / (fi * b * d * d);
            sqrt_term = 1.0 - 2.0 * R / (0.85 * fck);
            if (sqrt_term < 0.0) goto doubly_reinforced;
            *rho = (0.85 * fck / fyk) * (1.0 - sqrt(sqrt_term));
            *As = *rho * b * d;

            *As = fmax(*As, As_min);  //due to use_min_reinf
            *rho = *As / (b * d);  //due to use_min_reinf

            c_na = *rho * fyk * d / (0.85 * fck * beta1);
        }

        // Pseudo-elastic stress
        double m = n * (*rho); // Tension reinforcement only
        double I_cr = (b * c_na * c_na * c_na) / 3.0 + m * b * d * (d - c_na) * (d - c_na);
        double A_cr = b * c_na + m * b * d;
        *sigma_c_max = M * c_na / I_cr;

        // Check limits
        double rho_max = 0.75 * 0.85 * beta1 * (fck / fyk) * (0.003 / (0.003 + 0.005));
#ifndef NDEBUG
        if (*total_rho > rho_max || *total_rho > 0.02) {
            printf("Warning: Total reinforcement ratio (%.2f%%) exceeds limits (rho_max = %.2f%% or 2%%).\n",
                   *total_rho * 100, rho_max * 100);
        }
        if (*sigma_c_max > 0.85 * fck) {
            printf("Warning: sigma_c_max (%.2f %s) exceeds 0.85*fck (%.2f %s).\n",
                   *sigma_c_max / unit_factor, stress_unit, 0.85 * fck / unit_factor, stress_unit);
        }
        printf("Singly reinforced: As = %.2f %s, rho = %.2f%%, sigma_c_max = %.2f %s\n",
               *As * area_display_factor, area_unit, *rho * 100, *sigma_c_max / unit_factor, stress_unit);
#endif
        return 0;
    }

    doubly_reinforced:
    // Step 2: Doubly reinforced (sqrt_term < 0, use c_na = c)
    c_na = cc;
    double concrete_force = 0.85 * fcd * beta1 * cc * b;
    double As_term = concrete_force / fyd;
    double lever_arm_concrete = h / 2.0 - (beta1 * cc) / 2.0;
    double lever_arm_As_prime = h / 2.0 - cc;
    double lever_arm_As = d - h / 2.0;
    double concrete_moment = concrete_force * lever_arm_concrete;
    double coeff_As_prime = fyd * (lever_arm_As_prime + lever_arm_As);
    double constant_term = concrete_moment + fyd * lever_arm_As * As_term;
    *As_prime = (M - constant_term) / coeff_As_prime;
    *As = *As_prime + As_term;

    /*
    if (*As < 0 || *As_prime < 0) {
        printf("Error: Negative reinforcement areas (As = %.2f %s, As_prime = %.2f %s).\n",
               *As * area_display_factor, area_unit, *As_prime * area_display_factor, area_unit);
        return -1;
    }
    */
    ////due to use_min_reinf
    if (*As_prime < 0) {
        *As_prime = 0.0;
        *As = fmax(As_term, As_min);
    }

    if (*As < 0) {
        printf("Error: Negative reinforcement area (As = %.2f mm²). M=%.2f Nm, h=%.3f m, b=%.3f m, c=%.3f m\n", *As * 1e6,M,h,b,cc);
        return -1;
    }

    *rho = *As / (b * d);
    *rho_prime = *As_prime / (b * d);
    *total_rho = *rho + *rho_prime;
    double m = n * (*rho); // Tension reinforcement only
    double I_cr = (b * c_na * c_na * c_na) / 3.0 + m * b * d * (d - c_na) * (d - c_na);
    double A_cr = b * c_na + m * b * d;
    *sigma_c_max = M * c_na / I_cr;

    // Check limits
    double rho_max = 0.75 * 0.85 * beta1 * (fck / fyk) * (0.003 / (0.003 + 0.005));
#ifndef NDEBUG
    if (*total_rho > rho_max || *total_rho > 0.02) {
        printf("Warning: Total reinforcement ratio (%.2f%%) exceeds limits (rho_max = %.2f%% or 2%%).\n",
               *total_rho * 100, rho_max * 100);
    }
    if (*sigma_c_max > 0.85 * fck) {
        printf("Warning: sigma_c_max (%.2f %s) exceeds 0.85*fck (%.2f %s).\n",
               *sigma_c_max / unit_factor, stress_unit, 0.85 * fck / unit_factor, stress_unit);
    }
    printf("Doubly reinforced: As = %.2f %s, As_prime = %.2f %s, rho = %.2f%%, rho_prime = %.2f%%, total_rho = %.2f%%, sigma_c_max = %.2f %s\n",
           *As * area_display_factor, area_unit, *As_prime * area_display_factor, area_unit,
           *rho * 100, *rho_prime * 100, *total_rho * 100, *sigma_c_max / unit_factor, stress_unit);
#endif
    return 0;
}

/**
 * Example usage with provided data
 */
int foo_test_UNI() {
    // Test case (SI units)
    double M = 218000.0; // Nm/m
    double h = 0.15;     // m
    double b = 1.0;  //m
    double cc = 0.02;     // m
    double fyk = 350e6;  // Pa
    double fyd = 350e6;  // Pa (fyk for ACI)
    double fck = 30e6;   // Pa
    double fcd = 30e6;   // Pa (fck for ACI stress block)
    double As, As_prime, rho, rho_prime, total_rho, sigma_c_max;
    int use_min_reinf = 1;

    printf("SI Units Test:\n");
    int result = calculate_rebars_aci_simplified_UNI(M, h, b, cc, fyk, fyd, fck, fcd, use_min_reinf,
                                                 &As, &As_prime, &rho, &rho_prime, &total_rho, &sigma_c_max);
    if (result == 0) {
        printf("Calculation successful.\n");
    } else {
        printf("Calculation failed.\n");
    }

    // Test case (Imperial units)
    M = 588100.0; // in-lbf/ft (218000 Nm/m * 2.6988)
    h = 5.90551;   // in (0.15 m * 39.3701)
    b = 12; //ft
    cc = 0.787402;  // in (0.02 m * 39.3701)
    fyk = 50763.2; // psi (350 MPa / 6.89476)
    fyd = 50763.2; // psi
    fck = 4351.13; // psi (30 MPa / 6.89476)
    fcd = 4351.13; // psi

    printf("\nImperial Units Test:\n");
    result = calculate_rebars_aci_simplified_UNI(M, h, b, cc, fyk, fyd, fck, fcd, use_min_reinf,
                                             &As, &As_prime, &rho, &rho_prime, &total_rho, &sigma_c_max);
    if (result == 0) {
        printf("Calculation successful.\n");
    } else {
        printf("Calculation failed.\n");
    }

    return 0;
}

void correct_pline(char *adp, char *adk)
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
    if (((*(PLATE_PROPERTY *)e1).ps==3) && ((*(PLATE_PROPERTY *)e2).ps==3))
    delta=((*(PLATE_PROPERTY *)e1).pn - (*(PLATE_PROPERTY *)e2).pn);
    else
    {
        if ((*(PLATE_PROPERTY *)e1).ps==-1) delta=1;
        else delta=-1;
    }
    return delta;
}

// Function to calculate the area of a triangle given its vertex coordinates
double calculateTriangleArea(double x1, double y1, double x2, double y2, double x3, double y3) {
    double area = 0.5 * fabs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
    return area;
}

double nofabs(double value) {
    return value;
}


/*
// Function to calculate the "cross product" (Z-component) of two 2D vectors
double point_cross_product(POINTD a, POINTD b, POINTD p)
{
    // Vector AB: (b.x - a.x, b.y - a.y)
    // Vector AP: (p.x - a.x, p.y - a.y)
    return (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
}

// Function to check if a point is inside a triangle
int is_point_in_triangle(POINTD pa, POINTD pb, POINTD pc, POINTD pp) {
    double cp1 = point_cross_product(pa, pb, pp);
    double cp2 = point_cross_product(pb, pc, pp);
    double cp3 = point_cross_product(pc, pa, pp);

    // Check if all cross products have the same sign or are zero
    // This handles cases where the point is on an edge
    return (cp1 >= 0 && cp2 >= 0 && cp3 >= 0) || (cp1 <= 0 && cp2 <= 0 && cp3 <= 0);
}
*/

static int getSignFloat(double num) {
    if (num == 0.0) {
        return 0;
    } else if (signbit(num)) {
        return -1;
    } else {
        return 1;
    }
}

void Plate_analysis(void) {

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
    char par[3][MaxTextLen] = {"", "", ""};
    int sif_body;
    int sif_material;
    int sif_body_force;
    int sif_boundary_condition;
    int sif_boundary_condition_simple;
    int sif_boundary_condition_fixed;
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
    char desired_layer[maxlen_w];
    char desired_layer_upper[maxlen_w];
    char layer_name_upper[maxlen_w];
    char desired_layer_bak[maxlen_w];
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
    double stress_min_RC, stress_max_RC, stress_min_RC_rho, stress_max_RC_rho, ps;
    BOOL RC_flag_exists=FALSE;
    char deflection_chapter[DEFLECTION_NUMBER][MaxTextLen];
    char stress_chapter[STRESS_NUMBER][MaxTextLen];
    char epsilon_chapter[EPSILON_NUMBER][MaxTextLen];
    WIELOKAT w = Stdef;
    GRADIENT gradient;
    FE_DATA fe_data;
    FE_DATA_EX fe_data_ex;
    char *fe_data_ptr;
    char *fe_data_ex_ptr;
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
    int *body_property;
    ST_PROPERTY *body_prop;
    int body_prop_no;
    BOOL ULSLC_flag[5];

    float (*jednostkiObX)(double mob);
    float (*jednostkiObY)(double mob);
    ST_UNIFORM_LOAD *st_uniform_load_comb=NULL, *st_uniform_load_cons=NULL;
    double Rzmin=0.0;
    double Rzmax=0.0;
    double Rzmin_back=0.0;
    double Rzmax_back=0.0;
    double area;
    double *body_area=NULL;
    double *surface_load[64]={NULL};  //there is no more than 64 cases
    double point_load[64];
    int case_files[5];
    double total_load;
    double load_ratio, load_ratio_d;
    POINTD point_b, point_c;
    double NSEW_dist_div=4.;

    was_refreshed = FALSE;

    pl_node_no = 0;
    pl_node_emb_no = 0;
    pl_edge_no = 0;
    pl_load_no = 0;
    plate_no = 0;
    hole_no = 0;
    wall_no = 0;
    zone_no = 0;

    PL_PROPERTY_MAX = 10;
    PL_LOAD_FACTORS_MAX = 100;
    PL_NODE_MAX = 100;
    PL_NODE_EMB_MAX = 10;
    PL_EDGE_MAX = 100;
    PL_LOAD_MAX = 100;
    PL_PLATE_MAX = 10;
    PL_HOLE_MAX = 10;
    PL_WALL_MAX = 10;
    PL_ZONE_MAX = 10;

    plate_no = hole_no = wall_no = zone_no = pl_load_no = 0;

    pl_property_no = 0;
    pl_load_factors_no = 0;
    gZ = 0.0;

    redcrsb(0, 171);
    //select_blok();
    select_blok_items(Blinia | Bluk | Bvector | BtekstNoType15);  //for plate  //Btext
    redcrsb(1, 171);

    if ((ADP == NULL) && (ADK == NULL)) return;

    //ret_standard = 1;   //TEMPORARY, for EU

    ClearErr();
    ClearInfo();

    ret_standard = ask_question_static(6, (char *) _No_, (char *) _Yes_, (char *) "", (char *) _PROCEED_PLATE_FEM_, 12,
                                       (char *) "", 11, 1, 0, &combination_no, &geometric_tiffness, &inertia,
                                       &st_dynamic_no, &PINNABLE, &theta_, &sigma_eq_, &epsilon_);
    //0 - rezygnuj; 1 - Eurocode, 2 - ASCE, 3 - ICC
    if (ret_standard > 0) key1 = _YES_;
    else if (ret_standard == 0) key1 = _NO_;
    else key1 = _NO_;

    if (key1 != _YES_ && key1 != _yes_) {
        zmien_atrybut_undo(dane, dane + dane_size);
        blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
        zmien_atrybut(ADP, ADK, Ablok, Aoblok);
        redcrsb(2, 167);
        return;
    }


    // Allocate memory for 16 char pointers (for the array of strings)
    block_names = (char **)malloc(MAX_L_BLOCKS * sizeof(char *));

    // Allocate memory for each individual string
    for (i = 0; i < MAX_L_BLOCKS; i++)
    {
        block_names[i] = (char *)malloc((MAX_L_BLOCKS_LEN + 1) * sizeof(char));
        if (block_names[i] == NULL) {
            perror("Failed to allocate memory for a string");
            // Free previously allocated memory before exiting
            for (int j = 0; j < i; j++) {
                free(block_names[j]);
            }
            free(block_names);
            block_names=NULL;
        }
        // Optional: Initialize the strings (e.g., to empty strings)
        block_names[i][0] = '\0';
    }


    pl_property = (ST_PROPERTY *) malloc(PL_PROPERTY_MAX * sizeof(ST_PROPERTY) + 100);
    pl_load_factors = (ST_LOAD_FACTORS *) malloc(PL_LOAD_FACTORS_MAX * sizeof(ST_LOAD_FACTORS) + 100);
    pl_node = (PL_NODE *) malloc(PL_NODE_MAX * sizeof(PL_NODE) + 100);
    pl_node_emb = (PL_NODE_EMB *) malloc(PL_NODE_EMB_MAX * sizeof(PL_NODE_EMB) + 100);
    pl_edge = (PL_EDGE *) malloc(PL_EDGE_MAX * sizeof(PL_EDGE) + 100);
    pl_load = (PL_LOAD *) malloc(PL_LOAD_MAX * sizeof(PL_LOAD) + 100);

    plate_property = (PLATE_PROPERTY *) malloc(PL_PLATE_MAX * sizeof(PLATE_PROPERTY) + 100);
    hole_property = (PLATE_PROPERTY *) malloc(PL_HOLE_MAX * sizeof(PLATE_PROPERTY) + 100);
    wall_property = (PLATE_PROPERTY *) malloc(PL_WALL_MAX * sizeof(PLATE_PROPERTY) + 100);
    zone_property = (PLATE_PROPERTY *) malloc(PL_ZONE_MAX * sizeof(PLATE_PROPERTY) + 100);

    pl_load_factors = (ST_LOAD_FACTORS *) malloc(PL_LOAD_FACTORS_MAX * sizeof(ST_LOAD_FACTORS));


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

    memmove(&pl_load_factors[pl_load_factors_no], &load_factors[0], sizeof(ST_LOAD_FACTORS));  //TEMPORARY for EUROCODE

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
        unit_factors_pl = &unit_factors_pl_si;
        prop_precisions_pl = &SI_precisions;
        UNITS = SI;
    } else if (Jednostki == 10) //cm
    {
        units_factor = 10.0;
        m_units_factor = 100.0;
        geo_units_factor = 0.01;
        //E_units_factor=100000.0;
        //h_units_factor=0.001;
        axis_increment = 0.25;
        unit_factors_pl = &unit_factors_pl_si;
        prop_precisions_pl = &SI_precisions;
        UNITS = SI;
    } else if (Jednostki == 1000)  //m
    {
        units_factor = 1000.0;
        m_units_factor = 1.0;
        geo_units_factor = 1.0;
        //E_units_factor=100000.0;
        //h_units_factor=0.001;
        axis_increment = 0.25;
        unit_factors_pl = &unit_factors_pl_si;
        prop_precisions_pl = &SI_precisions;
        UNITS = SI;
    } else if (Jednostki == 1000000) //km
    {
        units_factor = 1000000.0;
        m_units_factor = 0.001;
        geo_units_factor = 1000;
        //E_units_factor=100000.0;
        //h_units_factor=0.001;
        axis_increment = 0.25;
        unit_factors_pl = &unit_factors_pl_si;
        prop_precisions_pl = &SI_precisions;
        UNITS = SI;
    } else if (Jednostki == 25.4) //"
    {
        units_factor = 1.0;  //imperial
        m_units_factor = 1.0;
        geo_units_factor = 1.0;
        //E_units_factor=1.0;
        //h_units_factor=1.0;
        axis_increment = 10.0;
        unit_factors_pl = &unit_factors_pl_imp;
        prop_precisions_pl = &IMP_precisions;
        UNITS = IMP;
    } else if (Jednostki == 304.8) //'
    {
        units_factor = 12.0;  //inti inches
        m_units_factor = 1.0;
        geo_units_factor = 12.0;
        //E_units_factor=1.0;
        //h_units_factor=1.0;
        axis_increment = 10.0;
        unit_factors_pl = &unit_factors_pl_imp;
        prop_precisions_pl = &IMP_precisions;
        UNITS = IMP;
    } else if (Jednostki == 914.4) //yd
    {
        units_factor = 36.0;  //into inches
        m_units_factor = 1.0;
        geo_units_factor = 36.0;
        //E_units_factor=1.0;
        //h_units_factor=1.0;
        axis_increment = 10.0;
        unit_factors_pl = &unit_factors_pl_imp;
        prop_precisions_pl = &IMP_precisions;
        UNITS = IMP;
    } else if (Jednostki == 1609344) //mi
    {
        units_factor = 63360.0;  //into inches
        m_units_factor = 1.0;
        geo_units_factor = 63360.0;  //into inches
        //E_units_factor=1.0;
        //h_units_factor=1.0;
        axis_increment = 10.0;
        unit_factors_pl = &unit_factors_pl_imp;
        prop_precisions_pl = &IMP_precisions;
        UNITS = IMP;
    } else {
        units_factor = 1.0;
        m_units_factor = 1000.0;
        geo_units_factor = 0.001;
        //E_units_factor=1.0;
        //h_units_factor=1.0;
        axis_increment = 10.0;
        unit_factors_pl = &unit_factors_pl_si;
        prop_precisions_pl = &SI_precisions;
        UNITS = SI;
    }

    if (UNITS!=SI) dxl_min=4.0f;  //inches
    else dxl_min=0.1f; //meters

    strcpy(title_id, "PLATE");
    ptr_id = title_id;
    ptr_id_short = (char *) "";

    ////properties
    ////////////////
    obiekt_tok((char *) ADP, ADK, (char **) &nag, Otekst);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            t = (TEXT *) nag;
            while (1) {
                ptr = strstr(t->text, _PLATE_);  //title
                if (ptr == NULL) ptr = strstr(t->text, _PLATE_PL);  //title
                if (ptr == NULL) ptr = strstr(t->text, _PLATE_UA);  //title
                if (ptr == NULL) ptr = strstr(t->text, _PLATE_ES);  //title
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

                memmove(&pl_property[pl_property_no], &prt_def_pl, sizeof(ST_PROPERTY));
                //if (prop_precisions_pl == &SI_precisions)
                if (UNITS==SI) pl_property[pl_property_no].c = 0.03; //[m]
                else pl_property[pl_property_no].c = 1.18; //[in]
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
                    pl_load_factors[pl_load_factors_no].load = str2load(ptr);

                    //"", LL", "LL", "Lr", "WL", "SL", "EL", "RL", "HL", "FL", "TL"};
                    memmove(&pl_load_factors[pl_load_factors_no],
                            &load_factors[pl_load_factors[pl_load_factors_no].load],
                            sizeof(ST_LOAD_FACTORS));  //temporary just for Eurocode

                    if (pl_load_factors[pl_load_factors_no].load > 0) {
                        pl_load_factors[pl_load_factors_no].variant = (unsigned char) atoi(ptr + 3);
                        ptr1 = strstr(ptr, "γ=");  //gamma
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "Γ=");  //Gamma
                        if (ptr1 != NULL) pl_load_factors[pl_load_factors_no].gamma = atof(ptr1 + 3);

                        ptr1 = strstr(ptr, "γi=");  //gamma inf
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "Γi=");  //Gamma inf
                        if (ptr1 != NULL) pl_load_factors[pl_load_factors_no].gamma_inf = atof(ptr1 + 4);

                        ptr1 = strstr(ptr, "Ψ0=");  //Psi
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "ψ0=");  //psi
                        if (ptr1 != NULL) pl_load_factors[pl_load_factors_no].psi0 = atof(ptr1 + 4);

                        ptr1 = strstr(ptr, "Ψ1=");  //Psi1
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "ψ1=");  //psi
                        if (ptr1 != NULL) pl_load_factors[pl_load_factors_no].psi1 = atof(ptr1 + 4);

                        ptr1 = strstr(ptr, "Ψ2=");  //Psi2
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "ψ2=");  //psi2
                        if (ptr1 != NULL) pl_load_factors[pl_load_factors_no].psi2 = atof(ptr1 + 4);

                        ptr1 = strstr(ptr, "ξ=");  //xi
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "Ξ=");  //Xi
                        if (ptr1 != NULL) pl_load_factors[pl_load_factors_no].xi = atof(ptr1 + 3);

                        add_load_factors_pl();
                    }
                    break; //end of string
                }

                ptr = strchr(t->text, '#');
                if (ptr == NULL) break;
                ptr1 = strchr(ptr + 1, ' ');
                if (ptr1 == NULL) break;
                strncpy(prop, ptr + 1, ptr1 - ptr - 1);
                prop[ptr1 - ptr - 1] = '\0';
                pl_property[pl_property_no].n = atoi(prop);

                ptr = strstr(t->text, " RC ");  //should be separated with spaces
                if (ptr != NULL)
                    pl_property[pl_property_no].RC_flag = 1; //TEMPORARY: assumption that it's rectangular cross section

                ptr = strstr(t->text, "h=");
                if (ptr == NULL) break;
                pl_property[pl_property_no].h =
                        atof(ptr + 2) * unit_factors_pl->h_f;  //for RC in mm or in, same as steel and wood

                pl_property[pl_property_no].b = pl_property[pl_property_no].h;

                ptr = strstr(t->text, "b=");
                if (ptr != NULL) {
                    pl_property[pl_property_no].b =
                            atof(ptr + 2) * unit_factors_pl->h_f;  //for RC in mm or in, same as steel and wood
                    if (pl_property[pl_property_no].RC_flag == 1) //it's RC and b is set
                    {
                        pl_property[pl_property_no].A = pl_property[pl_property_no].h *
                                                        pl_property[pl_property_no].b; //Cross-sectional area in mm^2
                        pl_property[pl_property_no].Asy = pl_property[pl_property_no].A; //Shear area in the local y-axis
                        pl_property[pl_property_no].Asz = pl_property[pl_property_no].A; //Shear area in the local z-axis
                        pl_property[pl_property_no].Iy =
                                pl_property[pl_property_no].h * pow(pl_property[pl_property_no].b, 3) /
                                12; //Moment of inertia for bending about the local y axis in mm^4  or in^4:  h*b^3/12
                        pl_property[pl_property_no].Iz =
                                pow(pl_property[pl_property_no].h, 3) * pl_property[pl_property_no].b /
                                12; //Moment of inertia for bending about the local z axis in mm^4  or in^4:  h^3*b/12
                        pl_property[pl_property_no].Wy =
                                pl_property[pl_property_no].h * pow(pl_property[pl_property_no].b, 2) *
                                unit_factors_pl->Wmm_f /
                                6; //Elastic section modulus about y-axis in m^3  or in^3:  h*b^2/6
                        pl_property[pl_property_no].Wz =
                                pow(pl_property[pl_property_no].h, 2) * pl_property[pl_property_no].b *
                                unit_factors_pl->Wmm_f /
                                6; //Elastic section modulus about z-axis  in m^3 or in^3:  h^2*b/6
                        pl_property[pl_property_no].Jx =
                                ((pl_property[pl_property_no].h * pl_property[pl_property_no].b) / 12) *
                                (pow(pl_property[pl_property_no].h, 2) +
                                 pow(pl_property[pl_property_no].b, 2)); //Torsional moment of inertia  h*b/12*(h^2+b^2)
                    }
                }

                /*
                ptr = strstr(t->text, "A=");
                if ((ptr == NULL) && (pl_property[pl_property_no].RC_flag == 0)) break;
                if (ptr != NULL) pl_property[pl_property_no].A = atof(ptr + 2) * unit_factors_pl->A_f;

                ptrs = strstr(t->text, "As=");
                if (ptrs != NULL) {
                    pl_property[pl_property_no].Asy = atof(ptrs + 3) * unit_factors_pl->A_f;
                    pl_property[pl_property_no].Asz =
                            pl_property[pl_property_no].Asy * 0.75;  //estimation, can be taken as exact value too
                }

                ptrsy = strstr(t->text, "Asy=");  //alternatively
                if (ptrsy != NULL) {
                    pl_property[pl_property_no].Asy = atof(ptrsy + 4) * unit_factors_pl->A_f;
                    pl_property[pl_property_no].Asz =
                            pl_property[pl_property_no].Asy * 0.75;  //estimation, can be taken as exact value too
                }

                //if ((ptrs == NULL) && (ptrsy == NULL)) break;
                if ((ptrs == NULL) && (ptrsy == NULL) && (pl_property[pl_property_no].RC_flag == 0)) break;

                ptr = strstr(t->text, "Asz=");
                if (ptr != NULL) {
                    pl_property[pl_property_no].Asz = atof(ptr + 4) * unit_factors_pl->A_f;
                }

                ptr = strstr(t->text, "Iy=");
                //if (ptr == NULL) break;
                if ((ptr == NULL) && (pl_property[pl_property_no].RC_flag == 0)) break;
                if (ptr != NULL) pl_property[pl_property_no].Iy = atof(ptr + 3) * unit_factors_pl->I_f;

                ptr = strstr(t->text, "Iz=");
                //if (ptr == NULL) break;
                if ((ptr == NULL) && (pl_property[pl_property_no].RC_flag == 0)) break;
                if (ptr != NULL) pl_property[pl_property_no].Iz = atof(ptr + 3) * unit_factors_pl->I_f;

                ptr = strstr(t->text, "Wy=");
                if (ptr != NULL) {
                    pl_property[pl_property_no].Wy = atof(ptr + 3) * unit_factors_pl->Wm_f;
                }

                ptr = strstr(t->text, "Wz=");
                if (ptr != NULL) {
                    pl_property[pl_property_no].Wz = atof(ptr + 3) * unit_factors_pl->Wm_f;
                }

                pl_property[pl_property_no].Jx = 1.0;

                ptr = strstr(t->text, "Jx=");
                if (ptr != NULL) {
                    pl_property[pl_property_no].Jx = atof(ptr + 3) * unit_factors_pl->I_f;
                }
                */
                ptr = strstr(t->text, "E=");
                if (ptr == NULL) break;

                pl_property[pl_property_no].E = atof(ptr + 2) * unit_factors_pl->E_f;

                ptr = strstr(t->text, "G=");
                if (ptr == NULL) break;

                pl_property[pl_property_no].G = atof(ptr + 2) * unit_factors_pl->E_f;

                pl_property[pl_property_no].r = 0.0;

                /*
                ptr = strstr(t->text, "r=");
                if (ptr != NULL) {

                    pl_property[pl_property_no].r = atof(ptr + 2);
                }
                 */

                pl_property[pl_property_no].d = 0.0;

                ptr = strstr(t->text, "d=");
                if (ptr != NULL) {

                    pl_property[pl_property_no].d = atof(ptr + 2) * unit_factors_pl->d_f;
                }

                pl_property[pl_property_no].a = 0.0;

                ptr = strstr(t->text, "a=");
                if (ptr != NULL) {

                    pl_property[pl_property_no].a = atof(ptr + 2) * unit_factors_pl->a_f;
                }

                ptr = strstr(t->text, "c=");  //concrete cover, default 35 mm
                if (ptr != NULL) {
                    pl_property[pl_property_no].c = atof(ptr + 2) * unit_factors_pl->c_f;
                }

                ptr = strstr(t->text, "ζ=");  //zeta, default 0.85
                if (ptr != NULL) {
                    pl_property[pl_property_no].zeta = atof(ptr + 3);
                }

                ptr = strstr(t->text, "fck=");  //no default, in Mpa or kpsi
                if (ptr != NULL) {
                    pl_property[pl_property_no].fck = atof(ptr + 4); // * unit_factors_pl->E_f;
                } ////else if (pl_property[pl_property_no].RC_flag == 1) break;

                ptr = strstr(t->text, "fcd=");  //no default, in Mpa or kpsi
                if (ptr != NULL) {
                    pl_property[pl_property_no].fcd = atof(ptr + 4); // * unit_factors_pl->E_f;
                } ////else if (pl_property[pl_property_no].RC_flag == 1) break;

                ptr = strstr(t->text, "fyk=");  //no default, in Mpa or kpsi
                if (ptr != NULL) {
                    pl_property[pl_property_no].fyk = atof(ptr + 4); // * unit_factors_pl->E_f;
                } ////else if (pl_property[pl_property_no].RC_flag == 1) break;

                ptr = strstr(t->text, "fyd=");  //no default, in Mpa or kpsi
                if (ptr != NULL) {
                    pl_property[pl_property_no].fyd = atof(ptr + 4); // * unit_factors_pl->E_f;
                } ////else if (pl_property[pl_property_no].RC_flag == 1) break;

                ptr = strstr(t->text, u8"γ="); //gamma
                if (ptr == NULL) ptr = strstr(t->text, "Γ=");  //Gamma
                if (ptr != NULL) {
                    pl_property[pl_property_no].gamma = atof(ptr + 3);
                }

                ptr = strstr(t->text, u8"γi=");  //gamma inf
                if (ptr == NULL) ptr = strstr(t->text, "Γi=");  //Gamma inf
                if (ptr != NULL) {
                    pl_property[pl_property_no].gamma_inf = atof(ptr + 4);
                }

                ptr = strstr(t->text, u8"ξ="); //xi
                if (ptr == NULL) ptr1 = strstr(t->text, "Ξ=");  //Xi
                if (ptr != NULL) {
                    pl_property[pl_property_no].xi = atof(ptr + 3);
                }

                pl_property[pl_property_no].ok = 1;

                /*
                if ((pl_property[pl_property_no].r == 90) || (pl_property[pl_property_no].r == -90) ||
                    (pl_property[pl_property_no].r == 270) || (pl_property[pl_property_no].r == -270)) {
                    double Asy_ = pl_property[pl_property_no].Asy;
                    pl_property[pl_property_no].Asy = pl_property[pl_property_no].Asz;
                    pl_property[pl_property_no].Asz = Asy_;

                    double Iy_ = pl_property[pl_property_no].Iy;
                    pl_property[pl_property_no].Iy = pl_property[pl_property_no].Iz;
                    pl_property[pl_property_no].Iz = Iy_;

                    double h_ = pl_property[pl_property_no].h;
                    pl_property[pl_property_no].h = pl_property[pl_property_no].b;
                    pl_property[pl_property_no].b = h_;

                    double wy_ = pl_property[pl_property_no].Wy;
                    pl_property[pl_property_no].Wy = pl_property[pl_property_no].Wz;
                    pl_property[pl_property_no].Wz = wy_;

                    pl_property[pl_property_no].r = 0;
                }

                if (pl_property[pl_property_no].Wy == 0) {
                    pl_property[pl_property_no].Wy =
                            (pl_property[pl_property_no].Iy / (pl_property[pl_property_no].h * 0.5)) *
                            unit_factors_pl->Wm_f;  //no factor necessary, all in mm, mm^3 and mm^4 or in, in^3 and in^4
                }

                if (pl_property[pl_property_no].Wz == 0) {
                    pl_property[pl_property_no].Wz =
                            (pl_property[pl_property_no].Iz / (pl_property[pl_property_no].b * 0.5)) *
                            unit_factors_pl->Wm_f;  //  same as above
                }

                */

                add_property_pl();

                break;
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, Otekst);
    }
    /////////////////
    ////searching for plate, hole, wall and zone polyline
    //searching for nodes size

    body_number = 0;
    ////searching for plates
    obiekt_tok((char *) ADP, ADK, (char **) &nag, OdBLOK);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            b = (BLOK *) nag;
            if (b->kod_obiektu == B_PLINE) {
                if (b->opis_obiektu[0] == PL_PLATE) {
                    pl_min_x = 99999.;
                    pl_min_y = 99999.;
                    pl_max_x = -99999.;
                    pl_max_y = -99999.;

                    if (create_plate(b, 0, plate_no, body_number, &first, &last, &property_number, &is_closed)) {
                        if (is_closed) {
                            plate_property[plate_no].adr = b;
                            plate_property[plate_no].property_number = property_number;
                            plate_property[plate_no].first_edge = first;
                            plate_property[plate_no].last_edge = last;
                            plate_property[plate_no].ps = -1;
                            plate_property[plate_no].pn = -1;
                            plate_property[plate_no].load = 0;
                            add_plate_property();
                            body_number++;
                        } else {
                            sprintf(report_row, "%s %d %s%s", _THE_PLATE_, plate_no + 1, _POLYLINE_IS_NOT_CLOSED_,rn);
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
                    if (create_plate(b, 1, hole_no, body_number, &first, &last, &property_number, &is_closed)) {
                        if (is_closed) {
                            hole_property[hole_no].adr = b;
                            hole_property[hole_no].property_number = property_number;
                            hole_property[hole_no].first_edge = first;
                            hole_property[hole_no].last_edge = last;
                            hole_property[hole_no].ps = -1;
                            hole_property[hole_no].pn = -1;
                            hole_property[hole_no].load = 0;
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
                    if (create_plate(b, 2, wall_no, body_number, &first, &last, &property_number, &is_closed)) {
                        if ((is_closed) || (!is_closed)) {
                            wall_property[wall_no].adr = b;
                            //wall_property[wall_no].property_number = property_number;
                            //inheriting property from plate
                            wall_property[wall_no].property_number = plate_property[0].property_number;
                            wall_property[wall_no].first_edge = first;
                            wall_property[wall_no].last_edge = last;
                            wall_property[wall_no].ps = -1;
                            wall_property[wall_no].pn = -1;
                            wall_property[wall_no].load = 0;
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
                    if (create_plate(b, 3, zone_no, body_number, &first, &last, &property_number, &is_closed)) {
                        if (is_closed) {
                            zone_property[zone_no].adr = b;
                            zone_property[zone_no].property_number = property_number;
                            zone_property[zone_no].first_edge = first;
                            zone_property[zone_no].last_edge = last;
                            zone_property[zone_no].ps = -1;
                            zone_property[zone_no].pn = -1;
                            zone_property[zone_no].load = 0;
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
    if (plate_no == 0)  //no plate
    {
        sprintf(report_row, "%s%s", _THE_PLATE_POLYLINE_NUMBER_IS_EQUAL_ZERO_,rn);
        strcat(report, report_row);
        //goto pl_error;
    }
    else if (plate_no > 1)  //to many plates - TEMPORARY
    {
        sprintf(report_row, "%s%s", _THE_PLATE_POLYLINE_NUMBER_IS_GREATER_THAN_ONE_,rn);
        strcat(report, report_row);
        //goto pl_error;
    }

    if (strlen(report) > 0) goto pl_error;

    ////searching for load
    obiekt_tok((char *) ADP, ADK, (char **) &nag, Ovector);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            v = (AVECTOR *) nag;
            if (v->style == 17) {
                pl_load[pl_load_no].adr = v;
                pl_load[pl_load_no].x1 = v->x1;
                pl_load[pl_load_no].y1 = v->y1;
                pl_load[pl_load_no].x2 = v->x2;
                pl_load[pl_load_no].y2 = v->y2;
                pl_load[pl_load_no].magnitude1 = v->magnitude1;
                pl_load[pl_load_no].magnitude2 = v->magnitude2;
                pl_load[pl_load_no].load = v->load;
                pl_load[pl_load_no].variant = v->variant;
                pl_load[pl_load_no].body = -1;  //not yet assigned
                pl_load[pl_load_no].body_no = -1;  //not yet assigned
                pl_load[pl_load_no].type = 0;  //uniformly distributed load


                if (v->variant > 0)
                    pl_load[pl_load_no].factor_record = factor_record(v->load, v->variant);
                else
                    pl_load[pl_load_no].factor_record = -(v->load == 0 ? 2
                                                                       : v->load);  //if no load spec, is assumed to be LL, so load=2
                pl_load[pl_load_no].take_it = 0;

                int v_factor_record = pl_load[pl_load_no].factor_record;

                add_load_pl();
            }
            else if (v->style == 18) {
                pl_load[pl_load_no].adr = v;
                pl_load[pl_load_no].x1 = v->x1;
                pl_load[pl_load_no].y1 = v->y1;
                pl_load[pl_load_no].x2 = v->x1;  //same point
                pl_load[pl_load_no].y2 = v->y1;

                parametry_lini((LINIA*)v, &PL);
                kos = sin(Angle_Normal(PL.kat * Pi / 180.));
                koc = cos(Angle_Normal(PL.kat * Pi / 180.));

                //pl_load[pl_load_no].magnitude1 = v->magnitude1;
                //pl_load[pl_load_no].magnitude2 = v->magnitude2;

                pl_load[pl_load_no].magnitude1 = v->magnitude1 * kos;
                pl_load[pl_load_no].magnitude2 = pl_load[pl_load_no].magnitude1;

                pl_load[pl_load_no].load = v->load;
                pl_load[pl_load_no].variant = v->variant;
                pl_load[pl_load_no].body = -1;  //not yet assigned
                pl_load[pl_load_no].body_no = -1;  //not yet assigned
                pl_load[pl_load_no].type = 1;  //concentrated force load

                if (v->variant > 0)
                    pl_load[pl_load_no].factor_record = factor_record(v->load, v->variant);
                else
                    pl_load[pl_load_no].factor_record = -(v->load == 0 ? 2
                                                                       : v->load);  //if no load spec, is assumed to be LL, so load=2
                pl_load[pl_load_no].take_it = 0;

                int v_factor_record = pl_load[pl_load_no].factor_record;

                add_load_pl();
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

    for (j=0; j<plate_no; j++)
    {
        ADPB = (char *) plate_property[j].adr;
        ADKB = ADPB + sizeof(NAGLOWEK) + plate_property[j].adr->n - 1;
        correct_pline(ADPB, ADKB);
    }
    */

    //assigning:
    // each hole to zone or plate
    // each wall to zone or plate, wall cannot belong to any hole
    // each zone to plate
    // we have for each element first_edge and last edge
    // checking if each endpoint of each edges lays inside another element

    BOOL missed = FALSE;


    //ZONES in ZONES  -  FOR GRADIATION
    for (i=0; i<zone_no; i++)
    {
        zone_property[i].pnz_n = 0;  //no parent zones
        zone_property[i].pnz = 0;  //parent zones flags zeroed

        //matching with plate
        for (j = 0; j < zone_no; j++)
        {
            if (j!=i)
            {
                ADPB = (char *) zone_property[j].adr;
                ADKB = ADPB + sizeof(NAGLOWEK) + zone_property[j].adr->n - 1;
                zmien_atrybut(ADPB, ADKB, Aoblok, Ablok);
                //BOOL found = FALSE;
                missed = FALSE;
                for (k = zone_property[i].first_edge; k < zone_property[i].last_edge; k++) {
                    df_x1 = pl_node[pl_edge[k].node1].x;
                    df_y1 = pl_node[pl_edge[k].node1].y;
                    df_x2 = pl_node[pl_edge[k].node2].x;
                    df_y2 = pl_node[pl_edge[k].node2].y;
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
    //qsort(zone_property, zone_no, sizeof(PLATE_PROPERTY), qsort_by_val_pnz);
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
    //qsort(zone_property, zone_no, sizeof(PLATE_PROPERTY), qsort_by_val);
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

        PLATE_PROPERTY *sorted_zone_property = (PLATE_PROPERTY*) malloc(zone_no * sizeof(PLATE_PROPERTY));

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
                    memmove(&sorted_zone_property[j], &zone_property[sorted_zone[j]], sizeof(PLATE_PROPERTY));
                    //.pn need to be changed
                    sorted_zone_property[j].pn=pn_table[sorted_zone_property[j].pn];
                }
                //finally overwrite zone_properties
                memmove(zone_property, sorted_zone_property, zone_no * sizeof(PLATE_PROPERTY));
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
            zmien_atrybut(ADPB, ADKB, Aoblok, Ablok);
            //BOOL found = FALSE;
            missed = FALSE;
            for (k=hole_property[i].first_edge; k<hole_property[i].last_edge; k++)
            {
                df_x1 = pl_node[pl_edge[k].node1].x;
                df_y1 = pl_node[pl_edge[k].node1].y;
                df_x2 = pl_node[pl_edge[k].node2].x;
                df_y2 = pl_node[pl_edge[k].node2].y;
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

    //HOLES in PLATES
    for (i=0; i<hole_no; i++)
    {
        if (hole_property[i].ps<0) //not yet assigned
        {
            //matching with plate
            for (j = 0; j < plate_no; j++) {
                ADPB = (char *) plate_property[j].adr;
                ADKB = ADPB + sizeof(NAGLOWEK) + plate_property[j].adr->n - 1;

                zmien_atrybut(ADPB, ADKB, Aoblok, Ablok);
                //BOOL found = FALSE;
                missed = FALSE;
                for (k = hole_property[i].first_edge; k < hole_property[i].last_edge; k++) {
                    df_x1 = pl_node[pl_edge[k].node1].x;
                    df_y1 = pl_node[pl_edge[k].node1].y;
                    df_x2 = pl_node[pl_edge[k].node2].x;
                    df_y2 = pl_node[pl_edge[k].node2].y;
                    ret1 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x1, df_y1,&s_hatch_param, 1, 0, 0, 0, 0, FALSE);
                    if (!ret1) {
                        missed = TRUE;
                        break;
                    }
                    ret2 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x2, df_y2, &s_hatch_param, 1, 0, 0, 0, 0, 0);
                    if (!ret2) {
                        missed = TRUE;
                        break;
                    }
                }
                zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
                if (missed == FALSE) {
                    hole_property[i].ps = 0; //plate
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
            zmien_atrybut(ADPB, ADKB, Aoblok, Ablok);
            //BOOL found = FALSE;
            missed = FALSE;
            for (k=wall_property[i].first_edge; k<wall_property[i].last_edge; k++)
            {
                df_x1 = pl_node[pl_edge[k].node1].x;
                df_y1 = pl_node[pl_edge[k].node1].y;
                df_x2 = pl_node[pl_edge[k].node2].x;
                df_y2 = pl_node[pl_edge[k].node2].y;
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

    //WALLS in PLATES
    for (i=0; i<wall_no; i++)
    {
        if (wall_property[i].ps<0) //not yet assigned
        {
            //matching with plate
            for (j = 0; j < plate_no; j++) {
                ADPB = (char *) plate_property[j].adr;
                ADKB = ADPB + sizeof(NAGLOWEK) + plate_property[j].adr->n - 1;
                zmien_atrybut(ADPB, ADKB, Aoblok, Ablok);
                //BOOL found = FALSE;
                missed = FALSE;
                for (k = wall_property[i].first_edge; k < wall_property[i].last_edge; k++) {
                    df_x1 = pl_node[pl_edge[k].node1].x;
                    df_y1 = pl_node[pl_edge[k].node1].y;
                    df_x2 = pl_node[pl_edge[k].node2].x;
                    df_y2 = pl_node[pl_edge[k].node2].y;
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
                    wall_property[i].ps = 0; //plate
                    wall_property[i].pn = j;
                    break;
                }
            }
        }
    }

    //ZONES in PLATES
    for (i=0; i<zone_no; i++)
    {

        if (zone_property[i].ps<0) //not yet assigned
        {
            //matching with plate
            for (j = 0; j < plate_no; j++)
            {
                ADPB = (char *) plate_property[j].adr;
                ADKB = ADPB + sizeof(NAGLOWEK) + plate_property[j].adr->n - 1;
                zmien_atrybut(ADPB, ADKB, Aoblok, Ablok);
                //BOOL found = FALSE;
                missed = FALSE;
                for (k = zone_property[i].first_edge; k < zone_property[i].last_edge; k++) {
                    df_x1 = pl_node[pl_edge[k].node1].x;
                    df_y1 = pl_node[pl_edge[k].node1].y;
                    df_x2 = pl_node[pl_edge[k].node2].x;
                    df_y2 = pl_node[pl_edge[k].node2].y;
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
                    zone_property[i].ps = 0; //plate
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
            sprintf(report_row, "%s%s", _THE_HOLE_NOT_ASSIGNED_TO_ZONE_OR_PLATE_,rn);
            strcat(report, report_row);
        }
    }

    for (i=0; i<wall_no; i++)
    {
        if (wall_property[i].ps<0) //ALERT
        {
            sprintf(report_row, "%s%s", _THE_WALL_NOT_ASSIGNED_TO_ZONE_OR_PLATE_,rn);
            strcat(report, report_row);
        }
    }

    for (i=0; i<zone_no; i++)
    {
        if (zone_property[i].ps<0) //ALERT
        {
            sprintf(report_row, "%s%s", _THE_ZONE_NOT_ASSIGNED_TO_PLATE_,rn);
            strcat(report, report_row);
        }
    }

    if (strlen(report) > 0) goto pl_error;

    ////LOAD assignement
    for (i = 0; i < pl_load_no; i++) {
        df_x1 = pl_load[i].x1;
        df_y1 = pl_load[i].y1;
        df_x2 = pl_load[i].x2;
        df_y2 = pl_load[i].y2;

        ////marking wall
        for (j = 0; j < wall_no; j++)
        {
            ADPB = (char *) wall_property[j].adr;
            ADKB = ADPB + sizeof(NAGLOWEK) + wall_property[j].adr->n - 1;
            zmien_atrybut(ADPB, ADKB, Aoblok, Ablok);
            ret1 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x1, df_y1, &s_hatch_param,
                                   1, 0, 0, 0, 0, 0);
            if (pl_load[i].type==0)
                ret2 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x2, df_y2, &s_hatch_param,
                                       1, 0, 0, 0, 0, 0);
            else ret2=ret1;  //same point
            zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
            if ((ret1 == 1) && (ret2 == 1)) {
                //pl_load[i].body = 2; //wall
                //pl_load[i].body_no = j;
                pl_load[i].body = wall_property[j].ps;
                pl_load[i].body_no = wall_property[j].pn;
                pl_load[i].point_body = wall_property[j].ps;   //2
                pl_load[i].point_body_no = wall_property[j].pn;   //j

                if (pl_load[i].type == 1)  //point load
                    embed_node(i, geo_units_factor);

                if (pl_load[i].body==3) zone_property[j].load++;
                else plate_property[j].load++;
                break;
            }
        }
        if (j == wall_no) //not found in walls
        {
            ////marking zone
            for (j = 0; j < zone_no; j++)
            {
                ADPB = (char *) zone_property[j].adr;
                ADKB = ADPB + sizeof(NAGLOWEK) + zone_property[j].adr->n - 1;
                zmien_atrybut(ADPB, ADKB, Aoblok, Ablok);
                ret1 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x1, df_y1,
                                       &s_hatch_param,
                                       1, 0, 0, 0, 0, 0);
                if (pl_load[i].type == 0)
                    ret2 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x2, df_y2,
                                           &s_hatch_param,
                                           1, 0, 0, 0, 0, 0);
                else ret2 = ret1;  //same point
                zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
                if ((ret1 == 1) && (ret2 == 1))
                {
                    //check if wall belongs to zone or plate
                    pl_load[i].body = 3; //zone
                    pl_load[i].body_no = j;
                    pl_load[i].point_body = 3; //zone
                    pl_load[i].point_body_no = j;

                    if (pl_load[i].type == 1)  //point load
                        embed_node(i, geo_units_factor);

                    zone_property[j].load++;
                    break;
                }
            }
            //if assigned to zone, won't be assigned to plate
            if (j == zone_no)  //not found in zones
            {
                ////marking plate
                for (j = 0; j < plate_no; j++) {
                    ADPB = (char *) plate_property[j].adr;
                    ADKB = ADPB + sizeof(NAGLOWEK) + plate_property[j].adr->n - 1;
                    zmien_atrybut(ADPB, ADKB, Aoblok, Ablok);
                    ret1 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x1, df_y1,
                                           &s_hatch_param, 1, 0, 0, 0, 0, 0);
                    if (pl_load[i].type == 0)
                        ret2 = hatch_proc_test((long_long) (ADPB - dane), (long_long) (ADKB - dane), df_x2, df_y2,
                                               &s_hatch_param, 1, 0, 0, 0, 0, 0);
                    else ret2 = ret1;  //same point
                    zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
                    if ((ret1 == 1) && (ret2 == 1)) {
                        pl_load[i].body = 0; //plate
                        pl_load[i].body_no = j;
                        pl_load[i].point_body = 0; //plate
                        pl_load[i].point_body_no = j;

                        if (pl_load[i].type == 1)  //point load
                            embed_node(i, geo_units_factor);

                        plate_property[j].load++;
                        break;
                    }
                }
            }
        }
    }

    ClearErr();
    ClearInfo();

#ifndef LINUX
#ifndef BIT64
    if (Is64BitOperatingSystem() == FALSE)
    {
        ret = ask_question(1, (char*)"", (char*)confirm, (char*)"", (char*)_CANNOT_PROCEED_IN_32BIT_, 12, (char*)_BUY_NEW_COMPUTER_, 11, 1, 62);
        goto pl_error;
    }
#endif
#endif

    if ((pl_load_no == 0) && (gZ == 0)) //no load, no self weight
    {
        sprintf(report_row, "%s%s", _NO_LOAD_ASSIGNED_,rn);
        strcat(report, report_row);
        goto pl_error;
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

    sprintf(params, "%s%s", _STATIC_, _plate_);
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
            sprintf(report_row, "%s %s%s%s", _cannot_create_folder_, _STATIC_, _plate_,rn);
            strcat(report, report_row);
        }
    }

    if (!my_directory_exists(_plate_)) {
#ifdef LINUX
        // Create a directory with read, write, and execute permissions for the owner
        // and read and execute permissions for group and others.
        mode_t file_mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
        check = mkdir(_plate_, file_mode);
#else
        check = _mkdir(_plate_);
#endif

        if (check) {
            sprintf(report_row, "%s %s%s", _cannot_create_folder_, _plate_,rn);
            strcat(report, report_row);
        }
    }

    ////deleting old files
    sprintf(params, "%splate.geo", _STATIC_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%s%splate.sif", _STATIC_, _plate_);
    if (my_file_exists(params)) unlink(params);
    ////results
    sprintf(params, "%s%splate.result", _STATIC_, _plate_);
    if (my_file_exists(params)) unlink(params);
    ////results in plate folder due to ElmerSolver error
    sprintf(params, "%splate_sls.result", _plate_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%splate_uls.result", _plate_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%splate_ulslc.result", _plate_, i);
    if (my_file_exists(params)) unlink(params);
    for (i = 0; i < 28; i++) {
        sprintf(params, "%splate_ulslc%d.result", _plate_, i);
        if (my_file_exists(params)) unlink(params);
    }
    sprintf(params, "%splate_slslc.result", _plate_, i);
    if (my_file_exists(params)) unlink(params);
    for (i = 0; i < 13; i++) {
        sprintf(params, "%splate_slslc%d.result", _plate_, i);
        if (my_file_exists(params)) unlink(params);
    }
    sprintf(params, "%splate_qpslslc.result", _plate_, i);
    if (my_file_exists(params)) unlink(params);
    for (i = 0; i < 8; i++) {
        sprintf(params, "%splate_qpslslc%d.result", _plate_, i);
        if (my_file_exists(params)) unlink(params);
    }
    ////mesh
    sprintf(params, "%s%smesh.nodes", _STATIC_, _plate_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%s%smesh.header", _STATIC_, _plate_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%s%smesh.elements", _STATIC_, _plate_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%s%smesh.boundary", _STATIC_, _plate_);
    if (my_file_exists(params)) unlink(params);
    ////

    //creating geo file
    sprintf(params, "%splate.geo", _STATIC_);
    f = fopen(params, "wt");

    if (f == NULL) {
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_RESULTS_FILE_, 12,
                           (char *) "", 11, 1,
                           62);
        no_error = FALSE;
        goto pl_error;
    }

    fprintf(f, "\n// NODES\n");
    //nodes
    for (i = 0; i < pl_node_no; i++) {
        set_decimal_format(par[0], milimetryobx(pl_node[i].x) * geo_units_factor, dim_precision_pl);
        set_decimal_format(par[1], milimetryoby(pl_node[i].y) * geo_units_factor, dim_precision_pl);
        set_decimal_format(par[2], max(milimetryob((double) pl_node[i].d) * geo_units_factor, dxl_min),
                           dim_precision_pl);

        fprintf(f, "Point(%d) = {%s, %s, 0, %s};\n", i + 1, par[0], par[1], par[2]);
    }

    //Embeded points and setting emb_no
    for (i=0; i<pl_node_emb_no; i++)
    {
        set_decimal_format(par[0], milimetryobx(pl_node_emb[i].x) * geo_units_factor, dim_precision_pl);
        set_decimal_format(par[1], milimetryoby(pl_node_emb[i].y) * geo_units_factor, dim_precision_pl);
        set_decimal_format(par[2], max(milimetryob((double) pl_node_emb[i].d) * geo_units_factor, dxl_min),
                           dim_precision_pl);

        fprintf(f, "Point(%d) = {%s, %s, 0, %s};\n", pl_node_no + i + 1, par[0], par[1], par[2]);
        pl_node_emb[i].emb_no=pl_node_no + i + 1;
    }

    //fprintf(f, "Point(32) = {2.500, 2.500, 0, 0.45};\n");

    //edges
    fprintf(f, "\n// EDGES\n");
    k = 0;
    //the first are plates
    fprintf(f, "\n// PLATES\n");
    for (i = 0; i < plate_no; i++) {
        for (j = plate_property[i].first_edge; j < plate_property[i].last_edge; j++) {
            if (pl_edge[j].type == 0)  //line
            {
                fprintf(f, "Line(%d) = {%d, %d};\n", k + 1, pl_edge[j].node1 + 1, pl_edge[j].node2 + 1);
                pl_edge[j].k = k;
            } else if (pl_edge[j].type == 1)  //circle
            {
                fprintf(f, "Circle(%d) = {%d, %d, %d};\n", k + 1, pl_edge[j].node1 + 1, pl_edge[j].node2 + 1,
                        pl_edge[j].node3 + 1);
                pl_edge[j].k = k;
            }
            k++;
        }
    }
    //the second are holes
    fprintf(f, "\n// HOLES\n");
    for (i = 0; i < hole_no; i++) {
        for (j = hole_property[i].first_edge; j < hole_property[i].last_edge; j++) {
            if (pl_edge[j].type == 0)  //line
            {
                fprintf(f, "Line(%d) = {%d, %d};\n", k + 1, pl_edge[j].node1 + 1, pl_edge[j].node2 + 1);
                pl_edge[j].k = k;
            } else if (pl_edge[j].type == 1)  //circle
            {
                fprintf(f, "Circle(%d) = {%d, %d, %d};\n", k + 1, pl_edge[j].node1 + 1, pl_edge[j].node2 + 1,
                        pl_edge[j].node3 + 1);
                pl_edge[j].k = k;
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
            if (pl_edge[j].type == 0)  //line
            {
                fprintf(f, "Line(%d) = {%d, %d};\n", k + 1, pl_edge[j].node1 + 1, pl_edge[j].node2 + 1);
                pl_edge[j].k = k;
            } else if (pl_edge[j].type == 1)  //circle
            {
                fprintf(f, "Circle(%d) = {%d, %d, %d};\n", k + 1, pl_edge[j].node1 + 1, pl_edge[j].node2 + 1,
                        pl_edge[j].node3 + 1);
                pl_edge[j].k = k;
            }
            k++;
        }
    }
    //int last_walls_line=k+1;


    //the last are zones
    fprintf(f, "\n// ZONES\n");
    for (i = 0; i < zone_no; i++) {
        for (j = zone_property[i].first_edge; j < zone_property[i].last_edge; j++) {
            if (pl_edge[j].type == 0)  //line
            {
                fprintf(f, "Line(%d) = {%d, %d};\n", k + 1, pl_edge[j].node1 + 1, pl_edge[j].node2 + 1);
                pl_edge[j].k = k;
            } else if (pl_edge[j].type == 1)  //circle
            {
                fprintf(f, "Circle(%d) = {%d, %d, %d};\n", k + 1, pl_edge[j].node1 + 1, pl_edge[j].node2 + 1,
                        pl_edge[j].node3 + 1);
                pl_edge[j].k = k;
            }
            k++;
        }
    }

    //curves
    fprintf(f, "\n// CURVES\n");
    //the first are holes
    fprintf(f, "\n// HOLES\n");
    for (i = 0; i < hole_no; i++) {
        fprintf(f, "Curve Loop (%d) = {", k + 1);

        for (j = hole_property[i].first_edge; j < hole_property[i].last_edge; j++) {
            if (pl_edge[j].inverted == 0) fprintf(f, "%d", pl_edge[j].k + 1);
            else fprintf(f, "-%d", pl_edge[j].k + 1);
            if (j < (hole_property[i].last_edge - 1)) fprintf(f, ", ");
        }
        fprintf(f, "};\n");
        hole_property[i].k = k;
        k++;
    }

    /*

    //the second are walls
    fprintf(f, "\n// WALLS\n");
    for (i = 0; i < wall_no; i++) {
        fprintf(f, "Curve Loop (%d) = {", k + 1);

        for (j = wall_property[i].first_edge; j < wall_property[i].last_edge; j++) {
            if (pl_edge[j].inverted == 0) fprintf(f, "%d", pl_edge[j].k + 1);
            else fprintf(f, "-%d", pl_edge[j].k + 1);
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
        for (j=0; j<pl_node_emb_no; j++)
        {
            if ((pl_node_emb[j].body==2) && (pl_node_emb[j].body_no==i)) //is wall and it's this wall
            {
                fprintf(f, "Point{%d} In Surface{%d};\n", pl_node_emb[j].emb_no, k+1);
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
    //qsort(zone_property, zone_no, sizeof(PLATE_PROPERTY), qsort_by_val);
    //for (i = 0; i < zone_no; i++) printf("%d %d\n",zone_property[i].ps, zone_property[i].pn);
    //printf("---------------\n");


    //the third are zones
    fprintf(f, "\n// ZONES\n");
    for (i = 0; i < zone_no; i++)
    {
        fprintf(f, "Curve Loop (%d) = {", k + 1);

        for (j = zone_property[i].first_edge; j < zone_property[i].last_edge; j++) {
            if (pl_edge[j].inverted == 0) fprintf(f, "%d", pl_edge[j].k + 1);
            else fprintf(f, "-%d", pl_edge[j].k + 1);
            //if (pl_edge[j].inverted == 0) fprintf(f, "%d", j + 1);
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
        for (j=0; j<pl_node_emb_no; j++)
        {
            if ((pl_node_emb[j].body==3) && (pl_node_emb[j].body_no==i)) //is zone and it's this zone
            {
                fprintf(f, "Point{%d} In Surface{%d};\n", pl_node_emb[j].emb_no, k+1);
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
                    if (pl_edge[jj].type==0)  //line
                        fprintf(f, "Line{%d} In Surface {%d};\n", jj + 1, k + 1);
                    else
                        fprintf(f, "Circle{%d} In Surface {%d};\n", jj+ 1, k + 1);
                }
            }
        }

        k++;
    }

    //the last are plates
    //the third are zones
    fprintf(f, "\n// PLATES\n");
    for (i = 0; i < plate_no; i++) {
        fprintf(f, "Curve Loop (%d) = {", k + 1);

        for (j = plate_property[i].first_edge; j < plate_property[i].last_edge; j++) {
            if (pl_edge[j].inverted == 0) fprintf(f, "%d", pl_edge[j].k + 1);
            else fprintf(f, "-%d", pl_edge[j].k + 1);
            if (j < (plate_property[i].last_edge - 1)) fprintf(f, ", ");
        }
        fprintf(f, "};\n");
        plate_property[i].k = k;
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
        for (j=0; j<pl_node_emb_no; j++)
        {
            if ((pl_node_emb[j].body==0) && (pl_node_emb[j].body_no==i)) //is plate and it's this plate
            {
                fprintf(f, "Point{%d} In Surface{%d};\n", pl_node_emb[j].emb_no, k+1);
            }
        }

        //Embedding wall lines
        fprintf(f, "\n// WALLS in PLATE\n");
        for (j = 0; j<wall_no; j++)
        {
            if ((wall_property[j].ps==0) && (wall_property[j].pn==i))
            {
                for (int jj=wall_property[j].first_edge; jj<wall_property[j].last_edge; jj++)
                {
                    if (pl_edge[jj].type==0)  //line
                        fprintf(f, "Line{%d} In Surface {%d};\n", jj + 1, k + 1);
                    else
                        fprintf(f, "Circle{%d} In Surface {%d};\n", jj+ 1, k + 1);
                }
            }
        }

        k++;
    }

    fprintf(f, "\n// THE END\n");

    fclose(f);

    //So there is no more than k bodies.
    //allocation of body_area;
    body_area=(double *)malloc(k*sizeof(double));  //max k bodies
    for (i=0; i<k; i++) body_area[i]=0.0;

    for (j=0; j<64; j++)  //for max 64 cases
    {
        surface_load[j] = (double *) malloc(k * sizeof(double));  //max k bodies
        for (i = 0; i < k; i++) surface_load[j][i] = 0.0;

        point_load[j]=0.0;
    }

    //creating geo file
    sprintf(params, "%s%splate.sif", _STATIC_, _plate_);
    f = fopen(params, "wt");

    if (f == NULL) {
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_RESULTS_FILE_, 12,
                           (char *) "", 11, 1,
                           62);
        no_error = FALSE;
        goto pl_error;
    }

    fprintf(f, "Header\n");
    fprintf(f, "  CHECK KEYWORDS Warn\n");
    fprintf(f, "  Mesh DB \"%s\" \"%s\"\n", "Static", "plate");
    fprintf(f, "  Include Path \"\"\n");
    fprintf(f, "  Results Directory \"%s%s\"\n", _STATIC_, "plate");
    fprintf(f, "End\n\n");

    fprintf(f, "Simulation\n");
    fprintf(f, "  Max Output Level = 5\n");
    fprintf(f, "  Coordinate System = Cartesian\n");
    fprintf(f, "  Coordinate Mapping(3) = 1 2 3\n");
    fprintf(f, "  Simulation Type = Steady state\n");
    fprintf(f, "  Steady State Max Iterations = 1\n");
    fprintf(f, "  Output Intervals(1) = 1\n");
    fprintf(f, "  Solver Input File = plate.sif\n");
    fprintf(f, "  Output File = \"plate_sls.result\"\n");
    fprintf(f, "  Post File = plate.vtu\n");
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
    //fprintf(f, "  Variable Output = Logical TRUE\n");
    //fprintf(f, "  Variable 1 = \"Force 3\"\n");
    //fprintf(f, "  Variable 2 = \"Force 4\"\n");
    //fprintf(f, "  Variable 3 = \"Force 5\"\n");
    //fprintf(f, "  Variable 4 = \"Force 6\"\n");
    //fprintf(f, "  Calculate Loads = Logical TRUE\n");
    //fprintf(f, "  Calculate Reactions = Logical TRUE\n");
    fprintf(f, "End\n\n");

    /*
    fprintf(f, "! New Solver section to calculate and save reaction forces\n");
    fprintf(f, "Solver 2\n");
    fprintf(f, "  Equation = SaveReactions\n");
    fprintf(f, "  Procedure = \"SaveData\" \"SaveReactions\"\n");
    fprintf(f, "  ! Name of the boundary condition you want to monitor\n");
    fprintf(f, "  ! (e.g., the name given to your support boundary condition)\n");
    fprintf(f, "  ! Boundary Id = 1\n");
    fprintf(f, "  Filename = \"total_reaction_forces.dat\"\n");
    fprintf(f, "  Save Body Forces = Logical False\n");
    fprintf(f, "  Save Surface Forces = Logical True\n");
    fprintf(f, "  ! Optional: specify which direction forces to sum\n");
    fprintf(f, "  ! Save Reaction 1 = Logical True ! for Fx\n");
    fprintf(f, "  ! Save Reaction 2 = Logical True ! for Fy\n");
    fprintf(f, "  Save Reaction 3 = Logical True ! for Fz\n");
    fprintf(f, "  ! By default, it might save all. Check Elmer documentation.\n");
    fprintf(f, "End\n\n");
    */

/*
    fprintf(f, "Solver 2\n");
    fprintf(f, "  Equation = \"SaveScalars\"\n");
    //fprintf(f, "  Exec Solver = After Timestep\n");
    fprintf(f, "  Exec Solver = After Simulation\n");
    //fprintf(f, "  Procedure = \"SaveScalars\" \"SaveBoundaryLoads\" ! Use SaveBoundaryLoads for forces on a boundary\n");
    fprintf(f, "  Procedure = \"SaveData\" \"SaveScalars\"\n");
    fprintf(f, "  Filename = \"shell_loads.dat\"\n");
    fprintf(f, "  ! Specify the boundary ID (e.g., 4)\n");
    //fprintf(f, "  Boundary Target = 1\n");
    fprintf(f, "  ! Specify what to save: \"Loads\"\n");
    fprintf(f, "  Variable 1 = \"Load 1\"\n");
    //fprintf(f, "  Scalar 1 = \"Load 3\"\n");
    fprintf(f, "  ! Specify the components if needed (e.g. for Force in Z direction)\n");
    //fprintf(f, "  Scalar 2 = \"Load 3\"\n");
    //fprintf(f, "  Save also = \"Load 1\" \"Load 2\" ! or other components\n");
    fprintf(f, "End\n\n");
*/
/*
    fprintf(f, "Solver 2\n");
    fprintf(f, "  Equation = \"CalculateLoads\"\n");
    fprintf(f, "  Exec Solver = After Simulation\n");
    fprintf(f, "  Procedure = \"StressSolve\" \"CalculateLoads\"\n");
    fprintf(f, "  Filename = \"shell_loads.dat\"\n");
    //fprintf(f, "  Loads on Boundary = Logical TRUE\n");
    //fprintf(f, "  Output Table = Logical TRUE\n");
    fprintf(f, "End\n\n");
*/

    /*
    fprintf(f, "Solver 2\n");
    fprintf(f, "  !Equation = Linear elasticity\n");
    fprintf(f, "  !Procedure = \"StressSolve\" \"StressSolver\"\n");
    fprintf(f, "  Equation = \"Nonlinear elasticity\"\n");
    fprintf(f, "  Procedure = \"ElasticSolve\" \"ElasticSolver\"\n");
    fprintf(f, "  Variable = -dofs 3 Displacement\n");
    fprintf(f, "  Calculate Loads = True\n");
    fprintf(f, "  Calculate Stresses = True\n");
    fprintf(f, "  Exec Solver = Always\n");
    fprintf(f, "  Stabilize = True\n");
    fprintf(f, "  Bubbles = False\n");
    fprintf(f, "  Lumped Mass Matrix = False\n");
    fprintf(f, "  Optimize Bandwidth = True\n");
    fprintf(f, "  Steady State Convergence Tolerance = 1.0e-5\n");
    fprintf(f, "  Nonlinear System Convergence Tolerance = 1.0e-8\n");
    fprintf(f, "  Nonlinear System Max Iterations = 20\n");
    fprintf(f, "  Nonlinear System Newton After Iterations = 3\n");
    fprintf(f, "  Nonlinear System Newton After Tolerance = 1.0e-3\n");
    fprintf(f, "  Nonlinear System Relaxation Factor = 1\n");
    fprintf(f, "  Linear System Solver = Iterative\n");
    fprintf(f, "  Linear System Iterative Method = BiCGStab\n");
    fprintf(f, "  Linear System Max Iterations = 500\n");
    fprintf(f, "  Linear System Convergence Tolerance = 1.0e-8\n");
    fprintf(f, "  Linear System Preconditioning = ILU5\n");
    fprintf(f, "  Linear System ILUT Tolerance = 1.0e-3\n");
    fprintf(f, "  Linear System Abort Not Converged = False\n");
    fprintf(f, "  Linear System Residual Output = 1\n");
    fprintf(f, "  Linear System Precondition Recompute = 1\n");
    fprintf(f, "  Normalize By Area = Logical True\n");
    fprintf(f, "End\n\n");
     */

    /*
    fprintf(f, "Solver 2\n");
    fprintf(f, "  Equation = \"Shell equations\"\n");
    //fprintf(f, "  Procedure = \"ShellSolver\" \"ShellSolver\"\n");
    fprintf(f, "  Procedure = \"FacetShellSolve\" \"ShellSolver\"\n");
    fprintf(f, "  !\n");
    fprintf(f, "          ! Computing nodal loads wouldn't work with the default component names:\n");
    fprintf(f, "  !\n");
    fprintf(f, "          Variable = \"U\"\n");

    fprintf(f, "  Large Deflection = False\n");
    fprintf(f, "  Displace Mesh = False\n");
    fprintf(f, "  Nonlinear System Max Iterations = 1\n");
    fprintf(f, "  Nonlinear System Convergence Tolerance = 1e-7\n");

    fprintf(f, "  Linear System Solver = \"Iterative\"\n");
    fprintf(f, "  Linear System Preconditioning = Diagonal\n");
    fprintf(f, "  Linear System Max Iterations = 1000\n");
    fprintf(f, "  Linear System Convergence Tolerance = 1e-9\n");
    fprintf(f, "  Linear System Iterative Method = GCR\n");
    fprintf(f, "  Linear System GCR Restart = 100\n");
    fprintf(f, "  Linear System Abort Not Converged = True\n");
    fprintf(f, "  Steady State Convergence Tolerance = 1e-09\n");
    ////fprintf(f, "  Calculate Loads = True\n");
    fprintf(f, "End\n\n");
    */

    sif_body = 0;

    for (i = 0; i < zone_no; i++) {
        fprintf(f, "Body %d\n", sif_body + 1);
        fprintf(f, "  Target Bodies(1) = %d\n", zone_property[i].k + 1);
        fprintf(f, "  Name = \"Body Property %d\"\n", sif_body + 1);
        fprintf(f, "  Equation = 1\n");
        fprintf(f, "  Material = %d\n", zone_property[i].property_number);
        fprintf(f, "  Body Force = %d\n", sif_body + 1); //TO DO
        ////fprintf(f, "  Normalize By Area = Logical True\n");
        fprintf(f, "End\n\n");
        sif_body++;
    }

    for (i = 0; i < plate_no; i++) {
        fprintf(f, "Body %d\n", sif_body + 1);
        fprintf(f, "  Target Bodies(1) = %d\n", plate_property[i].k + 1);
        fprintf(f, "  Name = \"Body Property %d\"\n", sif_body + 1);
        fprintf(f, "  Equation = 1\n");
        fprintf(f, "  Material = %d\n", plate_property[i].property_number);
        fprintf(f, "  Body Force = %d\n", sif_body + 1); //TO DO
        ////fprintf(f, "  Normalize By Area = Logical True\n");
        fprintf(f, "End\n\n");
        sif_body++;
    }

    fprintf(f, "Equation 1\n");
    fprintf(f, "  Name = \"Equation 1\"\n");
    fprintf(f, "  Active Solvers(1) = 1\n");
    ////fprintf(f, "  Active Solvers(2) = 1 2\n");
    fprintf(f, "End\n\n");

    sif_material = 0;
    for (i = 0; i < zone_no; i++) {
        //searching for properties
        for (j = 0; j < pl_property_no; j++) {
            if (pl_property[j].n == zone_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        fprintf(f, "Material %d\n", sif_material + 1);
        fprintf(f, "  Name = \"Material %d\"\n", sif_material + 1);
        fprintf(f, "  Youngs modulus = %g\n", pl_property[this_property].E);
        fprintf(f, "  Density = %g\n", pl_property[this_property].d);
        fprintf(f, "  Poisson ratio = %f\n", (pl_property[this_property].E / (2. * pl_property[this_property].G)) - 1);
        set_decimal_format(par[1], pl_property[this_property].h, dim_precision_pl);
        fprintf(f, "  Thickness = %s\n", par[1]);
        fprintf(f, "End\n\n");
        sif_material++;
    }

    for (i = 0; i < plate_no; i++) {
        //searching for properties
        for (j = 0; j < pl_property_no; j++) {
            if (pl_property[j].n == plate_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        fprintf(f, "Material %d\n", sif_material + 1);
        fprintf(f, "  Name = \"Material %d\"\n", sif_material + 1);
        fprintf(f, "  Youngs modulus = %g\n", pl_property[this_property].E);
        fprintf(f, "  Density = %g\n", pl_property[this_property].d);
        fprintf(f, "  Poisson ratio = %f\n", (pl_property[this_property].E / (2. * pl_property[this_property].G)) - 1.);
        set_decimal_format(par[1], pl_property[this_property].h, dim_precision_pl);
        fprintf(f, "  Thickness = %s\n", par[1]);
        fprintf(f, "End\n\n");
        sif_material++;
    }


    ////Boundary conditions
    sif_boundary_condition = 0;
    sif_boundary_condition_simple = 0;
    sif_boundary_condition_fixed = 0;

    //simply supported edges
    for (i = 0; i < pl_edge_no; i++) {
        if (pl_edge[i].restraint == 6) sif_boundary_condition_simple++;
        else if (pl_edge[i].restraint == 7) sif_boundary_condition_fixed++;
    }

    if (sif_boundary_condition_simple > 0) {
        fprintf(f, "Boundary Condition %d\n", sif_boundary_condition + 1);
        sprintf(par[1], "  Target Boundaries(%d) =", sif_boundary_condition_simple);
        for (i = 0; i < pl_edge_no; i++) {
            if (pl_edge[i].restraint == 6) {
                sprintf(par[2], " %d", pl_edge[i].k + 1);
                strcat(par[1], par[2]);
            }
        }
        fprintf(f, "%s\n", par[1]);
        fprintf(f, "  Name = \"Boundary Condition %d\"\n", sif_boundary_condition + 1);
        fprintf(f, "  Displacement 1 = 0\n");
        fprintf(f, "  Displacement 2 = 0\n");
        fprintf(f, "  Displacement 3 = 0\n");
        fprintf(f, "  Deflection 1 = 0\n");
        fprintf(f, "  Deflection 2 = 0\n");
        fprintf(f, "  Deflection 3 = 0\n");
        fprintf(f, "  u 1 = 0\n");
        fprintf(f, "  u 2 = 0\n");
        fprintf(f, "  u 3 = 0\n");
        ////fprintf(f, "  Normalize By Area = Logical True\n");
        ////fprintf(f, "  Resultant Force = Logical True\n");
        fprintf(f, "End\n");

        sif_boundary_condition++;
    }

    if (sif_boundary_condition_fixed > 0) {
        fprintf(f, "Boundary Condition %d\n", sif_boundary_condition + 1);
        sprintf(par[1], "  Target Boundaries(%d) =", sif_boundary_condition_fixed);
        for (i = 0; i < pl_edge_no; i++) {
            if (pl_edge[i].restraint == 7) {
                sprintf(par[2], " %d", pl_edge[i].k + 1);
                strcat(par[1], par[2]);
            }
        }
        fprintf(f, "%s\n", par[1]);
        fprintf(f, "  Name = \"Boundary Condition %d\"\n", sif_boundary_condition + 1);
        fprintf(f, "  Displacement 1 = 0\n");
        fprintf(f, "  Displacement 2 = 0\n");
        fprintf(f, "  Displacement 3 = 0\n");
        fprintf(f, "  Displacement 4 = 0\n");
        fprintf(f, "  Displacement 5 = 0\n");
        fprintf(f, "  Displacement 6 = 0\n");
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
        fprintf(f, "End\n");

        sif_boundary_condition++;
    }
    /////


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
    pllc_node_force_moment_no = 0;
    for (i = 0; i < pl_point_load_no; i++) {
        //if (TestBit(st_layer, st_node_force_moment[i].layer)) {
        if (pl_point_load[i].factor_record >= 0) {
            gamma_l = pl_load_factors[pl_point_load[i].factor_record].gamma;
            flag_ = load_flag[pl_load_factors[pl_point_load[i].factor_record].load];
            flag_ = load_flag[pl_load_factors[pl_point_load[i].factor_record].load];
        } else {
            gamma_l = pl_load_factors[abs(pl_point_load[i].factor_record)].gamma;
            flag_ = load_flag[pl_load_factors[abs(pl_point_load[i].factor_record)].load];
        }
        combi_flag |= flag_;
        pllc_node_force_moment_no++;
        //}
    }

    //plate uniform load
    pllc_uniform_load_no = 0;
    for (i = 0; i < pl_load_no; i++) {
        //if (TestBit(st_layer, pl_load[i].layer)) {
        if (pl_load[i].factor_record >= 0) {
            gamma_l = pl_load_factors[pl_load[i].factor_record].gamma;
            flag_ = load_flag[pl_load_factors[pl_load[i].factor_record].load];
        } else {
            gamma_l = pl_load_factors[abs(pl_load[i].factor_record)].gamma;
            flag_ = load_flag[pl_load_factors[abs(pl_load[i].factor_record)].load];
        }
        combi_flag |= flag_;
        pllc_uniform_load_no++;
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

    for (j = 0; j < pl_load_no; j++) { ;
    }


    combinations_number = 2 + combi_uls_no_total + combi_sls_no_total +
                          combi_qpsls_no_total;  //SLS + ULS for each st_layer_no plus total number of combinations for each state

    printf("%d\t\t# number of static load cases\n", combinations_number);

    //ST_UNIFORM_LOAD *st_uniform_load_comb=NULL, *st_uniform_load_cons=NULL;
    int st_uniform_load_no_cons;

    st_uniform_load_comb = malloc((pl_load_no + 1) * sizeof(ST_UNIFORM_LOAD));
    st_uniform_load_cons = malloc((pl_load_no + 1) * sizeof(ST_UNIFORM_LOAD));

    int case_number = 0;

    ///////////////////////////
    //// LOADS SLS BEGINS

    case_files[0]=case_number;
    case_number++;
    sif_body_force = 0;

    for (i = 0; i < zone_no; i++)
    {
        //searching for properties
        for (j = 0; j < pl_property_no; j++) {
            if (pl_property[j].n == zone_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        fprintf(f, "Body Force %d\n", sif_body_force + 1);
        fprintf(f, "  Name = \"BodyForce %d\"\n", sif_body_force + 1);
        ////fprintf(f, "  Normalize By Area = Logical True\n");
        strcpy(load_formula, "");

        self_weight = 0.0;
        self_weight = gZ * pl_property[this_property].d * pl_property[this_property].h;
        if (self_weight > 0.0) {
            set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
            strcat(load_formula, "(");
            strcat(load_formula, par[1]);
            strcat(load_formula, ")");

            surface_load[case_number-1][zone_property[i].k] += self_weight;
        }

        if (zone_property[i].property_number==plate_property[0].property_number)  //load zone
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
                for (j = 0; j < pl_load_no; j++) //all load
                {
                    if (pl_load[j].type == 0 && (pl_load[j].body == 3) && (pl_load[j].body_no == i)) //uniformly distributed, zone and number
                    {
                        set_decimal_format(par[1], -pl_load[j].magnitude1 * unit_factors_pl->q_f,
                                           load_precision);  //is assumed that magnitude1=magnitude2
                        if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                        strcat(load_formula, "(");
                        strcat(load_formula, par[1]);
                        strcat(load_formula, ")");

                        surface_load[case_number-1][zone_property[i].k] += (pl_load[j].magnitude1 * unit_factors_pl->q_f);
                    }
                }
            }
        }
        else  //property zone, inherits from plate 0 + zona local load
        {
            for (j = 0; j < pl_load_no; j++) //all load
            {
                if ((pl_load[j].type == 0) &&   //uniformly distributed, zone and number
                    (((pl_load[j].body == 3) && (pl_load[j].body_no == i)) ||  //local load on zone
                     (pl_load[j].body == 0) && (pl_load[j].body_no == 0)))  //inherited load on plate
                {
                    set_decimal_format(par[1], -pl_load[j].magnitude1 * unit_factors_pl->q_f,
                                       load_precision);  //is assumed that magnitude1=magnitude2
                    if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][zone_property[i].k] += (pl_load[j].magnitude1 * unit_factors_pl->q_f);
                }
            }
        }

        ////fprintf(f,"  Direction of Outward Normal = –1\n");

        if (strlen(load_formula) > 0) fprintf(f, "  Normal Pressure = Real MATC \"%s\"\n", load_formula);

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

        for (j = 0; j < pl_load_no; j++) //all load
        {
            if (pl_load[j].type==1 && (pl_load[j].body == 3) && (pl_load[j].body_no == i)) //concentrated, zone and number
            {
                strcpy(load_formula0, "");
                set_decimal_format(par[0], milimetryobx(pl_load[j].x1) * geo_units_factor, dim_precision_pl);
                set_decimal_format(par[1], milimetryoby(pl_load[j].y1) * geo_units_factor, dim_precision_pl);
                set_decimal_format(par[2], pl_load[j].magnitude1 * unit_factors_pl->F_f,force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                sprintf(load_formula0," %s %s 0.0 0.0 0.0 %s 0.0 0.0 0.0",par[0],par[1],par[2]);
                strcat(load_formula, load_formula0);
                f_no++;

                point_load[zone_property[i].k]+= (pl_load[j].magnitude1 * unit_factors_pl->F_f);
            }
        }

        if (f_no>0)
            fprintf(f, "  Point Load (%d, 9) = Real %s\n", f_no, load_formula);
        ///////////////// concentrated load

        set_decimal_format(par[1], pl_property[this_property].h, dim_precision_pl);
        fprintf(f, "  Thickness = Real %s\n", par[1]);

        fprintf(f, "End\n\n");
        sif_body_force++;
    }

    for (i = 0; i < plate_no; i++) {
        //searching for properties
        for (j = 0; j < pl_property_no; j++) {
            if (pl_property[j].n == plate_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        fprintf(f, "Body Force %d\n", sif_body_force + 1);
        fprintf(f, "  Name = \"BodyForce %d\"\n", sif_body_force + 1);

        strcpy(load_formula, "");

        self_weight = 0.0;
        self_weight = gZ * pl_property[this_property].d * pl_property[this_property].h;
        if (self_weight > 0.0) {
            set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
            strcat(load_formula, "(");
            strcat(load_formula, par[1]);
            strcat(load_formula, ")");

            surface_load[case_number-1][plate_property[i].k] += self_weight;
        }

        for (j = 0; j < pl_load_no; j++)   //all load
        {
            if (pl_load[j].type==0 && (pl_load[j].body == 0) && (pl_load[j].body_no == i)) //uniformly distributed, plate and number
            {
                set_decimal_format(par[1], -pl_load[j].magnitude1 * unit_factors_pl->q_f,
                                   load_precision);  //is assumed that magnitude1=magnitude2
                if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                strcat(load_formula, "(");
                strcat(load_formula, par[1]);
                strcat(load_formula, ")");

                surface_load[case_number-1][plate_property[i].k] += (pl_load[j].magnitude1 * unit_factors_pl->q_f);
            }
        }

        ////fprintf(f,"  Direction of Outward Normal = –1\n");
        if (strlen(load_formula) > 0) fprintf(f, "  Normal Pressure = Real MATC \"%s\"\n", load_formula);

        ///////////////// Concentrated Load
        strcpy(load_formula, "");
        int f_no=0;

        for (j = 0; j < pl_load_no; j++) //all load
        {
            if (pl_load[j].type==1 && (pl_load[j].body == 0) && (pl_load[j].body_no == i)) //concentrated, plate and number
            {
                strcpy(load_formula0, "");
                set_decimal_format(par[0], milimetryobx(pl_load[j].x1) * geo_units_factor, dim_precision_pl);
                set_decimal_format(par[1], milimetryoby(pl_load[j].y1) * geo_units_factor, dim_precision_pl);
                set_decimal_format(par[2], pl_load[j].magnitude1 * unit_factors_pl->F_f,force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                sprintf(load_formula0," %s %s 0.0 0.0 0.0 %s 0.0 0.0 0.0",par[0],par[1],par[2]);
                strcat(load_formula, load_formula0);
                f_no++;

                point_load[plate_property[i].k]+= (pl_load[j].magnitude1 * unit_factors_pl->F_f);
            }
        }

        if (f_no>0)
            fprintf(f, "  Point Load (%d, 9) = Real %s\n", f_no, load_formula);
        //////// concentrated load

        set_decimal_format(par[1], pl_property[this_property].h, dim_precision_pl);
        fprintf(f, "  Thickness = Real %s\n", par[1]);
        fprintf(f, "End\n\n");
        sif_body_force++;
    }
    ////SLS ENDS
    fprintf(f, "RUN\n\n");

    ////LOAD VARIANTS ULS BEGINS
    fprintf(f, "Simulation::Output File = \"plate_uls.result\"\n");

    case_files[1]=case_number;
    case_number++;
    sif_body_force = 0;

    for (i = 0; i < zone_no; i++) {
        //searching for properties
        for (j = 0; j < pl_property_no; j++) {
            if (pl_property[j].n == zone_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        //fprintf(f, "Body Force %d::", sif_body_force + 1);

        strcpy(load_formula, "");

        self_weight = 0.0;
        self_weight = gZ * gammas->gamma_g * pl_property[this_property].d * pl_property[this_property].h;
        if (self_weight > 0.0) {
            set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
            strcat(load_formula, "(");
            strcat(load_formula, par[1]);
            strcat(load_formula, ")");

            surface_load[case_number-1][zone_property[i].k] += self_weight;
        }

        if (zone_property[i].property_number==plate_property[0].property_number)  //load zone
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
                for (j = 0; j < pl_load_no; j++) //all load
                {
                    if (pl_load[j].type == 0 && (pl_load[j].body == 3) && (pl_load[j].body_no == i)) //uniformly distributed, zone and number
                    {
                        if (pl_load[i].factor_record >= 0)
                            gamma_l = pl_load_factors[pl_load[i].factor_record].gamma;
                        else gamma_l = load_factors[abs(pl_load[i].factor_record)].gamma;

                        set_decimal_format(par[1], -pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f,
                                           load_precision);  //is assumed that magnitude1=magnitude2
                        if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                        strcat(load_formula, "(");
                        strcat(load_formula, par[1]);
                        strcat(load_formula, ")");

                        surface_load[case_number-1][zone_property[i].k] += (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f);
                    }
                }
            }
        }
        else  //property zone, inherits from plate 0 + zona local load
        {
            for (j = 0; j < pl_load_no; j++) //all load
            {
                if ((pl_load[j].type == 0) &&   //uniformly distributed, zone and number
                    (((pl_load[j].body == 3) && (pl_load[j].body_no == i)) ||  //local load on zone
                     (pl_load[j].body == 0) && (pl_load[j].body_no == 0)))  //inherited load on plate
                {
                    if (pl_load[i].factor_record >= 0)
                        gamma_l = pl_load_factors[pl_load[i].factor_record].gamma;
                    else gamma_l = load_factors[abs(pl_load[i].factor_record)].gamma;

                    set_decimal_format(par[1], -pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f,
                                       load_precision);  //is assumed that magnitude1=magnitude2
                    if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][zone_property[i].k] += (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f);
                }
            }
        }

        if (strlen(load_formula) > 0)
        {
            fprintf(f, "Body Force %d::", sif_body_force + 1);
            fprintf(f, "Normal Pressure = Real MATC \"%s\"\n", load_formula);
        }

        ///////////////// Concentrated Load
        strcpy(load_formula, "");
        int f_no=0;

        for (j = 0; j < pl_load_no; j++) //all load
        {
            if (pl_load[j].type==1 && (pl_load[j].body == 3) && (pl_load[j].body_no == i)) //concentrated, zone and number
            {
                if (pl_load[i].factor_record >= 0)
                    gamma_l = pl_load_factors[pl_load[i].factor_record].gamma;
                else gamma_l = load_factors[abs(pl_load[i].factor_record)].gamma;

                strcpy(load_formula0, "");
                set_decimal_format(par[0], milimetryobx(pl_load[j].x1) * geo_units_factor, dim_precision_pl);
                set_decimal_format(par[1], milimetryoby(pl_load[j].y1) * geo_units_factor, dim_precision_pl);
                set_decimal_format(par[2], pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f,force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                sprintf(load_formula0," %s %s 0.0 0.0 0.0 %s 0.0 0.0 0.0",par[0],par[1],par[2]);
                strcat(load_formula, load_formula0);
                f_no++;

                point_load[zone_property[i].k]+= (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f);
            }
        }

        if (f_no>0)
        {
            fprintf(f, "Body Force %d::", sif_body_force + 1);
            fprintf(f, "Point Load (%d, 9) = Real %s\n", f_no, load_formula);
        }
        //////// concentrated load

        sif_body_force++;
    }

    for (i = 0; i < plate_no; i++) {
        //searching for properties
        for (j = 0; j < pl_property_no; j++) {
            if (pl_property[j].n == plate_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        //fprintf(f, "Body Force %d::", sif_body_force + 1);

        strcpy(load_formula, "");

        self_weight = 0.0;
        self_weight = gZ * gammas->gamma_g * pl_property[this_property].d * pl_property[this_property].h;
        if (self_weight > 0.0) {
            set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
            strcat(load_formula, "(");
            strcat(load_formula, par[1]);
            strcat(load_formula, ")");

            surface_load[case_number-1][plate_property[i].k] += self_weight;
        }

        for (j = 0; j < pl_load_no; j++) //all load
        {
            if (pl_load[j].type==0 && (pl_load[j].body == 0) && (pl_load[j].body_no == i)) //uniformly distributed, plate and number
            {
                if (pl_load[i].factor_record >= 0)
                    gamma_l = pl_load_factors[pl_load[i].factor_record].gamma;
                else gamma_l = load_factors[abs(pl_load[i].factor_record)].gamma;

                set_decimal_format(par[1], -pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f,
                                   load_precision);  //is assumed that magnitude1=magnitude2
                //fprintf(f, "  Normal Pressure = %s\n", par[1]);
                if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                strcat(load_formula, "(");
                strcat(load_formula, par[1]);
                strcat(load_formula, ")");

                surface_load[case_number-1][plate_property[i].k] += (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f);
            }
        }
        if (strlen(load_formula) > 0)
        {
            fprintf(f, "Body Force %d::", sif_body_force + 1);
            fprintf(f, "Normal Pressure = Real MATC \"%s\"\n", load_formula);
        }

        ///////////////// Concentrated Load
        strcpy(load_formula, "");
        int f_no=0;

        for (j = 0; j < pl_load_no; j++) //all load
        {
            if (pl_load[j].type==1 && (pl_load[j].body == 0) && (pl_load[j].body_no == i)) //concentrated, plate and number
            {
                if (pl_load[i].factor_record >= 0)
                    gamma_l = pl_load_factors[pl_load[i].factor_record].gamma;
                else gamma_l = load_factors[abs(pl_load[i].factor_record)].gamma;

                strcpy(load_formula0, "");
                set_decimal_format(par[0], milimetryobx(pl_load[j].x1) * geo_units_factor, dim_precision_pl);
                set_decimal_format(par[1], milimetryoby(pl_load[j].y1) * geo_units_factor, dim_precision_pl);
                set_decimal_format(par[2], pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f,force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                sprintf(load_formula0," %s %s 0.0 0.0 0.0 %s 0.0 0.0 0.0",par[0],par[1],par[2]);
                strcat(load_formula, load_formula0);
                f_no++;

                point_load[plate_property[i].k]+= (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f);
            }
        }

        if (f_no>0)
        {
            fprintf(f, "Body Force %d::", sif_body_force + 1);
            fprintf(f, "Point Load (%d, 9) = Real %s\n", f_no, load_formula);
        }
        //////// concentrated load

        sif_body_force++;
    }

    //// Body Force 1::Normal Pressure = Real MATC "-2452.5 + (-4000 * 2)"
    //// Body Force 2::Normal Pressure = Real MATC "-2452.5 + (-3000 * 2)"
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

            //combi_ulslc_ino++;

            ////fprintf(f, "Simulation::Output File = \"plate_ulslc%d.result\"\n", combi_ulslc_ino);

            sif_body_force = 0;

            strcpy(all_formula, "");

            for (i = 0; i < zone_no; i++) {
                //searching for properties
                for (j = 0; j < pl_property_no; j++) {
                    if (pl_property[j].n == zone_property[i].property_number) {
                        this_property = j;
                        break;
                    }
                }
                sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                strcat(all_formula, par[0]);

                strcpy(load_formula, "");

                self_weight = 0.0;
                self_weight = gZ * gamma_g * pl_property[this_property].d * pl_property[this_property].h;
                if (self_weight > 0.0) {
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][zone_property[i].k] += self_weight;
                }

                if (zone_property[i].property_number==plate_property[0].property_number)  //load zone
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
                        for (j = 0; j < pl_load_no; j++) //all load
                        {
                            if (pl_load[j].type == 0 && (pl_load[j].body == 3) && (pl_load[j].body_no == i)) //uniformly distributed, zone and number
                            {
                                if (pl_load[i].factor_record >= 0) {
                                    combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                                    load = pl_load_factors[pl_load[i].factor_record].load;
                                } else {
                                    combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                                    load = load_factors[abs(pl_load[i].factor_record)].load;
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

                                set_decimal_format(par[1], -pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f,
                                                   load_precision);  //is assumed that magnitude1=magnitude2
                                if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                                strcat(load_formula, "(");
                                strcat(load_formula, par[1]);
                                strcat(load_formula, ")");

                                surface_load[case_number-1][zone_property[i].k] += (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f);
                            }
                        }
                    }
                }
                else //property zone, inherits from plate 0 + zona local load
                {
                    for (j = 0; j < pl_load_no; j++) //all load
                    {
                        if ((pl_load[j].type == 0) &&   //uniformly distributed, zone and number
                            (((pl_load[j].body == 3) && (pl_load[j].body_no == i)) ||  //local load on zone
                             (pl_load[j].body == 0) && (pl_load[j].body_no == 0)))  //inherited load on plate
                        {
                            if (pl_load[i].factor_record >= 0) {
                                combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                                load = pl_load_factors[pl_load[i].factor_record].load;
                            } else {
                                combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                                load = load_factors[abs(pl_load[i].factor_record)].load;
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

                            set_decimal_format(par[1], -pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f,
                                               load_precision);  //is assumed that magnitude1=magnitude2
                            if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                            strcat(load_formula, "(");
                            strcat(load_formula, par[1]);
                            strcat(load_formula, ")");

                            surface_load[case_number-1][zone_property[i].k] += (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f);
                        }
                    }
                }
                if (strlen(load_formula) > 0)
                    sprintf(all_load_formula, "Normal Pressure = Real MATC \"%s\"\n", load_formula);
                strcat(all_formula, all_load_formula);

                ///////////////// Concentrated Load
                strcpy(load_formula, "");
                int f_no=0;

                for (j = 0; j < pl_load_no; j++) //all load
                {
                    if (pl_load[j].type==1 && (pl_load[j].body == 3) && (pl_load[j].body_no == i)) //concentrated, zone and number
                    {
                        if (pl_load[i].factor_record >= 0)
                        {
                            combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                            load = pl_load_factors[pl_load[i].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                            load = load_factors[abs(pl_load[i].factor_record)].load;
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
                        set_decimal_format(par[0], milimetryobx(pl_load[j].x1) * geo_units_factor, dim_precision_pl);
                        set_decimal_format(par[1], milimetryoby(pl_load[j].y1) * geo_units_factor, dim_precision_pl);
                        set_decimal_format(par[2], pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f,force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                        sprintf(load_formula0," %s %s 0.0 0.0 0.0 %s 0.0 0.0 0.0",par[0],par[1],par[2]);
                        strcat(load_formula, load_formula0);
                        f_no++;

                        point_load[zone_property[i].k]+= (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f);
                    }
                }

                if (f_no>0)
                {
                    sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                    sprintf(all_load_formula, "%sPoint Load (%d, 9) = Real %s\n", par[0], f_no, load_formula);
                    strcat(all_formula, all_load_formula);
                }
                //////// concentrated load

                sif_body_force++;
            }

            for (i = 0; i < plate_no; i++) {
                //searching for properties
                for (j = 0; j < pl_property_no; j++) {
                    if (pl_property[j].n == plate_property[i].property_number) {
                        this_property = j;
                        break;
                    }
                }
                sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                strcat(all_formula, par[0]);

                strcpy(load_formula, "");

                self_weight = 0.0;
                self_weight = gZ * gamma_g * pl_property[this_property].d * pl_property[this_property].h;
                if (self_weight > 0.0) {
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][plate_property[i].k] += self_weight;
                }

                for (j = 0; j < pl_load_no; j++) //all load
                {
                    if (pl_load[j].type==0 && (pl_load[j].body == 0) && (pl_load[j].body_no == i)) //uniformly distributed, plate and number
                    {
                        if (pl_load[i].factor_record >= 0)
                        {
                            combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                            load = pl_load_factors[pl_load[i].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                            load = load_factors[abs(pl_load[i].factor_record)].load;
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

                        set_decimal_format(par[1], -pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f,
                                           load_precision);  //is assumed that magnitude1=magnitude2
                        //fprintf(f, "  Normal Pressure = %s\n", par[1]);
                        if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                        strcat(load_formula, "(");
                        strcat(load_formula, par[1]);
                        strcat(load_formula, ")");

                        surface_load[case_number-1][plate_property[i].k] += (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f);
                    }
                }
                if (strlen(load_formula) > 0)
                    sprintf(all_load_formula, "Normal Pressure = Real MATC \"%s\"\n", load_formula);
                strcat(all_formula, all_load_formula);

                ///////////////// Concentrated Load
                strcpy(load_formula, "");
                int f_no=0;

                for (j = 0; j < pl_load_no; j++) //all load
                {
                    if (pl_load[j].type==1 && (pl_load[j].body == 0) && (pl_load[j].body_no == i)) //concentrated, plate and number
                    {
                        if (pl_load[i].factor_record >= 0)
                        {
                            combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                            load = pl_load_factors[pl_load[i].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                            load = load_factors[abs(pl_load[i].factor_record)].load;
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
                        set_decimal_format(par[0], milimetryobx(pl_load[j].x1) * geo_units_factor, dim_precision_pl);
                        set_decimal_format(par[1], milimetryoby(pl_load[j].y1) * geo_units_factor, dim_precision_pl);
                        set_decimal_format(par[2], pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f,force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                        sprintf(load_formula0," %s %s 0.0 0.0 0.0 %s 0.0 0.0 0.0",par[0],par[1],par[2]);
                        strcat(load_formula, load_formula0);
                        f_no++;

                        point_load[plate_property[i].k]+= (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f);
                    }
                }

                if (f_no>0)
                {
                    sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                    sprintf(all_load_formula, "%sPoint Load (%d, 9) = Real %s\n", par[0], f_no, load_formula);
                    strcat(all_formula, all_load_formula);
                }
                //////// concentrated load

                sif_body_force++;
            }

            go_ahead = TRUE;
            if (all_formula_bak != NULL) {
                if (strcmp(all_formula_bak, all_formula) == 0)
                    go_ahead = FALSE;
            }

            if (go_ahead) {

                combi_ulslc_ino++;

                //fprintf(f, "Simulation::Output File = \"plate_ulslc%d.result\"\n", combi_ulslc_ino);
                fprintf(f, "Simulation::Output File = \"plate_ulslc%d.result\"\n", ci+1);
                fprintf(f, "%s",all_formula);
                //// Body Force 1::Normal Pressure = Real MATC "-2452.5 + (-4000 * 0.5)"
                //// Body Force 2::Normal Pressure = Real MATC "-2452.5 + (-3000 *0.5)"
                fprintf(f, "RUN\n\n");

                //ULSLC[ci].flag = 2;
                //ULSLC[combi_ulslc_ino-1].flag = 2;
                ULSLC[ci].flag = 2;
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

            //combi_slslc_ino++;

            ////fprintf(f, "Simulation::Output File = \"plate_slslc%d.result\"\n", combi_slslc_ino);

            sif_body_force = 0;

            strcpy(all_formula, "");

            for (i = 0; i < zone_no; i++) {
                //searching for properties
                for (j = 0; j < pl_property_no; j++) {
                    if (pl_property[j].n == zone_property[i].property_number) {
                        this_property = j;
                        break;
                    }
                }
                sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                strcat(all_formula, par[0]);

                strcpy(load_formula, "");

                self_weight = 0.0;
                self_weight = gZ * gamma_g * pl_property[this_property].d * pl_property[this_property].h;
                if (self_weight > 0.0) {
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][zone_property[i].k] += self_weight;
                }

                if (zone_property[i].property_number==plate_property[0].property_number)  //load zone
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
                        for (j = 0; j < pl_load_no; j++) //all load
                        {
                            if (pl_load[j].type == 0 && (pl_load[j].body == 3) && (pl_load[j].body_no == i)) //uniformly distributed, zone and number
                            {
                                if (pl_load[i].factor_record >= 0) {
                                    combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                                    load = pl_load_factors[pl_load[i].factor_record].load;
                                } else {
                                    combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                                    load = load_factors[abs(pl_load[i].factor_record)].load;
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

                                set_decimal_format(par[1], -pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f,
                                                   load_precision);  //is assumed that magnitude1=magnitude2
                                if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                                strcat(load_formula, "(");
                                strcat(load_formula, par[1]);
                                strcat(load_formula, ")");

                                surface_load[case_number-1][zone_property[i].k] += (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f);
                            }
                        }
                    }
                }
                else //property zone, inherits from plate 0 + zona local load
                {
                    for (j = 0; j < pl_load_no; j++) //all load
                    {
                        if ((pl_load[j].type == 0) &&   //uniformly distributed, zone and number
                            (((pl_load[j].body == 3) && (pl_load[j].body_no == i)) ||  //local load on zone
                             (pl_load[j].body == 0) && (pl_load[j].body_no == 0)))  //inherited load on plate
                        {
                            if (pl_load[i].factor_record >= 0) {
                                combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                                load = pl_load_factors[pl_load[i].factor_record].load;
                            } else {
                                combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                                load = load_factors[abs(pl_load[i].factor_record)].load;
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

                            set_decimal_format(par[1], -pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f,
                                               load_precision);  //is assumed that magnitude1=magnitude2
                            if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                            strcat(load_formula, "(");
                            strcat(load_formula, par[1]);
                            strcat(load_formula, ")");

                            surface_load[case_number-1][zone_property[i].k] += (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f);
                        }
                    }

                }
                if (strlen(load_formula) > 0)
                    sprintf(all_load_formula, "Normal Pressure = Real MATC \"%s\"\n", load_formula);
                strcat(all_formula, all_load_formula);

                ///////////////// Concentrated Load
                strcpy(load_formula, "");
                int f_no=0;

                for (j = 0; j < pl_load_no; j++) //all load
                {
                    if (pl_load[j].type==1 && (pl_load[j].body == 3) && (pl_load[j].body_no == i)) //concentrated, zone and number
                    {
                        if (pl_load[i].factor_record >= 0)
                        {
                            combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                            load = pl_load_factors[pl_load[i].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                            load = load_factors[abs(pl_load[i].factor_record)].load;
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
                        set_decimal_format(par[0], milimetryobx(pl_load[j].x1) * geo_units_factor, dim_precision_pl);
                        set_decimal_format(par[1], milimetryoby(pl_load[j].y1) * geo_units_factor, dim_precision_pl);
                        set_decimal_format(par[2], pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f,force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                        sprintf(load_formula0," %s %s 0.0 0.0 0.0 %s 0.0 0.0 0.0",par[0],par[1],par[2]);
                        strcat(load_formula, load_formula0);
                        f_no++;

                        point_load[zone_property[i].k]+= (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f);
                    }
                }

                if (f_no>0)
                {
                    sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                    sprintf(all_load_formula, "%sPoint Load (%d, 9) = Real %s\n", par[0], f_no, load_formula);
                    strcat(all_formula, all_load_formula);
                }
                //////// concentrated load

                sif_body_force++;
            }

            for (i = 0; i < plate_no; i++) {
                //searching for properties
                for (j = 0; j < pl_property_no; j++) {
                    if (pl_property[j].n == plate_property[i].property_number) {
                        this_property = j;
                        break;
                    }
                }
                sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                strcat(all_formula, par[0]);

                strcpy(load_formula, "");

                self_weight = 0.0;
                self_weight = gZ * gamma_g * pl_property[this_property].d * pl_property[this_property].h;
                if (self_weight > 0.0) {
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][plate_property[i].k] += self_weight;
                }

                for (j = 0; j < pl_load_no; j++) //all load
                {
                    if (pl_load[j].type==0 && (pl_load[j].body == 0) && (pl_load[j].body_no == i)) //uniformly distributed, plate and number
                    {
                        if (pl_load[i].factor_record >= 0)
                        {
                            combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                            load = pl_load_factors[pl_load[i].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                            load = load_factors[abs(pl_load[i].factor_record)].load;
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

                        set_decimal_format(par[1], -pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f,
                                           load_precision);  //is assumed that magnitude1=magnitude2
                        //fprintf(f, "  Normal Pressure = %s\n", par[1]);
                        if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                        strcat(load_formula, "(");
                        strcat(load_formula, par[1]);
                        strcat(load_formula, ")");

                        surface_load[case_number-1][plate_property[i].k] += (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f);
                    }
                }
                if (strlen(load_formula) > 0)
                    sprintf(all_load_formula, "Normal Pressure = Real MATC \"%s\"\n", load_formula);
                strcat(all_formula, all_load_formula);

                ///////////////// Concentrated Load
                strcpy(load_formula, "");
                int f_no=0;

                for (j = 0; j < pl_load_no; j++) //all load
                {
                    if (pl_load[j].type==1 && (pl_load[j].body == 0) && (pl_load[j].body_no == i)) //concentrated, plate and number
                    {
                        if (pl_load[i].factor_record >= 0)
                        {
                            combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                            load = pl_load_factors[pl_load[i].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                            load = load_factors[abs(pl_load[i].factor_record)].load;
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
                        set_decimal_format(par[0], milimetryobx(pl_load[j].x1) * geo_units_factor, dim_precision_pl);
                        set_decimal_format(par[1], milimetryoby(pl_load[j].y1) * geo_units_factor, dim_precision_pl);
                        set_decimal_format(par[2], pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f,force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                        sprintf(load_formula0," %s %s 0.0 0.0 0.0 %s 0.0 0.0 0.0",par[0],par[1],par[2]);
                        strcat(load_formula, load_formula0);
                        f_no++;

                        point_load[plate_property[i].k]+= (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f);
                    }
                }

                if (f_no>0)
                {
                    sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                    sprintf(all_load_formula, "%sPoint Load (%d, 9) = Real %s\n", par[0], f_no, load_formula);
                    strcat(all_formula, all_load_formula);
                }
                //////// concentrated load

                sif_body_force++;
            }

            go_ahead = TRUE;
            if (all_formula_bak != NULL) {
                if (strcmp(all_formula_bak, all_formula) == 0)
                    go_ahead = FALSE;
            }

            if (go_ahead) {

                combi_slslc_ino++;

                //fprintf(f, "Simulation::Output File = \"plate_slslc%d.result\"\n", combi_slslc_ino);
                fprintf(f, "Simulation::Output File = \"plate_slslc%d.result\"\n", ci+1);
                fprintf(f, "%s",all_formula);
                //// Body Force 1::Normal Pressure = Real MATC "-2452.5 + (-4000 * 0.5)"
                //// Body Force 2::Normal Pressure = Real MATC "-2452.5 + (-3000 *0.5)"
                fprintf(f, "RUN\n\n");

                //SLSLC[ci].flag = 2;
                //SLSLC[combi_slslc_ino-1].flag = 2;
                SLSLC[ci].flag = 2;
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

            //combi_qpslslc_ino++;

            ////fprintf(f, "Simulation::Output File = \"plate_qpslslc%d.result\"\n", combi_qpslslc_ino);

            sif_body_force = 0;

            strcpy(all_formula, "");

            for (i = 0; i < zone_no; i++) {
                //searching for properties
                for (j = 0; j < pl_property_no; j++) {
                    if (pl_property[j].n == zone_property[i].property_number) {
                        this_property = j;
                        break;
                    }
                }
                sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                strcat(all_formula, par[0]);

                strcpy(load_formula, "");

                self_weight = 0.0;
                self_weight = gZ * gamma_g * pl_property[this_property].d * pl_property[this_property].h;
                if (self_weight > 0.0) {
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][zone_property[i].k] += self_weight;
                }

                if (zone_property[i].property_number==plate_property[0].property_number)  //load zone
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
                        for (j = 0; j < pl_load_no; j++) //all load
                        {
                            if (pl_load[j].type == 0 && (pl_load[j].body == 3) && (pl_load[j].body_no == i)) //uniformly distributed, zone and number
                            {
                                if (pl_load[i].factor_record >= 0) {
                                    combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                                    load = pl_load_factors[pl_load[i].factor_record].load;
                                } else {
                                    combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                                    load = load_factors[abs(pl_load[i].factor_record)].load;
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

                                set_decimal_format(par[1], -pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f,
                                                   load_precision);  //is assumed that magnitude1=magnitude2
                                if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                                strcat(load_formula, "(");
                                strcat(load_formula, par[1]);
                                strcat(load_formula, ")");

                                surface_load[case_number-1][zone_property[i].k] += (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f);
                            }
                        }
                    }
                }
                else
                {
                    for (j = 0; j < pl_load_no; j++) //all load
                    {
                        if ((pl_load[j].type == 0) &&   //uniformly distributed, zone and number
                            (((pl_load[j].body == 3) && (pl_load[j].body_no == i)) ||  //local load on zone
                             (pl_load[j].body == 0) && (pl_load[j].body_no == 0)))  //inherited load on plate
                        {
                            if (pl_load[i].factor_record >= 0) {
                                combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                                load = pl_load_factors[pl_load[i].factor_record].load;
                            } else {
                                combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                                load = load_factors[abs(pl_load[i].factor_record)].load;
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

                            set_decimal_format(par[1], -pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f,
                                               load_precision);  //is assumed that magnitude1=magnitude2
                            if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                            strcat(load_formula, "(");
                            strcat(load_formula, par[1]);
                            strcat(load_formula, ")");

                            surface_load[case_number-1][zone_property[i].k] += (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f);
                        }
                    }
                }
                if (strlen(load_formula) > 0)
                    sprintf(all_load_formula, "Normal Pressure = Real MATC \"%s\"\n", load_formula);
                strcat(all_formula, all_load_formula);

                ///////////////// Concentrated Load
                strcpy(load_formula, "");
                int f_no=0;

                for (j = 0; j < pl_load_no; j++) //all load
                {
                    if (pl_load[j].type==1 && (pl_load[j].body == 3) && (pl_load[j].body_no == i)) //concentrated, zone and number
                    {
                        if (pl_load[i].factor_record >= 0)
                        {
                            combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                            load = pl_load_factors[pl_load[i].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                            load = load_factors[abs(pl_load[i].factor_record)].load;
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
                        set_decimal_format(par[0], milimetryobx(pl_load[j].x1) * geo_units_factor, dim_precision_pl);
                        set_decimal_format(par[1], milimetryoby(pl_load[j].y1) * geo_units_factor, dim_precision_pl);
                        set_decimal_format(par[2], pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f,force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                        sprintf(load_formula0," %s %s 0.0 0.0 0.0 %s 0.0 0.0 0.0",par[0],par[1],par[2]);
                        strcat(load_formula, load_formula0);
                        f_no++;

                        point_load[zone_property[i].k]+= (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f);
                    }
                }

                if (f_no>0)
                {
                    sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                    sprintf(all_load_formula, "%sPoint Load (%d, 9) = Real %s\n", par[0], f_no, load_formula);
                    strcat(all_formula, all_load_formula);
                }
                //////// concentrated load

                sif_body_force++;
            }

            for (i = 0; i < plate_no; i++) {
                //searching for properties
                for (j = 0; j < pl_property_no; j++) {
                    if (pl_property[j].n == plate_property[i].property_number) {
                        this_property = j;
                        break;
                    }
                }
                sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                strcat(all_formula, par[0]);

                strcpy(load_formula, "");

                self_weight = 0.0;
                self_weight = gZ * gamma_g * pl_property[this_property].d * pl_property[this_property].h;
                if (self_weight > 0.0) {
                    set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
                    strcat(load_formula, "(");
                    strcat(load_formula, par[1]);
                    strcat(load_formula, ")");

                    surface_load[case_number-1][plate_property[i].k] += self_weight;
                }

                for (j = 0; j < pl_load_no; j++) //all load
                {
                    if (pl_load[j].type==0 && (pl_load[j].body == 0) && (pl_load[j].body_no == i)) //uniformly distributed, plate and number
                    {
                        if (pl_load[i].factor_record >= 0)
                        {
                            combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                            load = pl_load_factors[pl_load[i].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                            load = load_factors[abs(pl_load[i].factor_record)].load;
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

                        set_decimal_format(par[1], -pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f,
                                           load_precision);  //is assumed that magnitude1=magnitude2
                        //fprintf(f, "  Normal Pressure = %s\n", par[1]);
                        if (strlen(load_formula) > 0) strcat(load_formula, " + ");
                        strcat(load_formula, "(");
                        strcat(load_formula, par[1]);
                        strcat(load_formula, ")");

                        surface_load[case_number-1][plate_property[i].k] += (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->q_f);
                    }
                }
                if (strlen(load_formula) > 0)
                    sprintf(all_load_formula, "Normal Pressure = Real MATC \"%s\"\n", load_formula);
                strcat(all_formula, all_load_formula);

                ///////////////// Concentrated Load
                strcpy(load_formula, "");
                int f_no=0;

                for (j = 0; j < pl_load_no; j++) //all load
                {
                    if (pl_load[j].type==1 && (pl_load[j].body == 0) && (pl_load[j].body_no == i)) //concentrated, plate and number
                    {
                        if (pl_load[i].factor_record >= 0)
                        {
                            combi_load_factor = &pl_load_factors[pl_load[i].factor_record];
                            load = pl_load_factors[pl_load[i].factor_record].load;
                        } else {
                            combi_load_factor = &load_factors[abs(pl_load[i].factor_record)];
                            load = load_factors[abs(pl_load[i].factor_record)].load;
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
                        set_decimal_format(par[0], milimetryobx(pl_load[j].x1) * geo_units_factor, dim_precision_pl);
                        set_decimal_format(par[1], milimetryoby(pl_load[j].y1) * geo_units_factor, dim_precision_pl);
                        set_decimal_format(par[2], pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f,force_precision);  //is assumed that magnitude1=magnitude2 and magnitude is normalized to z axis and vector direction
                        sprintf(load_formula0," %s %s 0.0 0.0 0.0 %s 0.0 0.0 0.0",par[0],par[1],par[2]);
                        strcat(load_formula, load_formula0);
                        f_no++;

                        point_load[plate_property[i].k]+= (pl_load[j].magnitude1 * gamma_l * unit_factors_pl->F_f);
                    }
                }

                if (f_no>0)
                {
                    sprintf(par[0], "Body Force %d::", sif_body_force + 1);
                    sprintf(all_load_formula, "%sPoint Load (%d, 9) = Real %s\n", par[0], f_no, load_formula);
                    strcat(all_formula, all_load_formula);
                }
                //////// concentrated load

                sif_body_force++;
            }

            go_ahead = TRUE;
            if (all_formula_bak != NULL) {
                if (strcmp(all_formula_bak, all_formula) == 0)
                    go_ahead = FALSE;
            }

            if (go_ahead) {
                combi_qpslslc_ino++;

                //fprintf(f, "Simulation::Output File = \"plate_qpslslc%d.result\"\n", combi_qpslslc_ino);
                fprintf(f, "Simulation::Output File = \"plate_qpslslc%d.result\"\n", ci+1);
                fprintf(f, "%s",all_formula);
                //// Body Force 1::Normal Pressure = Real MATC "-2452.5 + (-4000 * 0.5)"
                //// Body Force 2::Normal Pressure = Real MATC "-2452.5 + (-3000 *0.5)"
                fprintf(f, "RUN\n\n");

                //QPSLSLC[ci].flag = 2;
                //QPSLSLC[combi_qpslslc_ino-1].flag = 2;
                QPSLSLC[ci].flag = 2;
            }

            all_formula_bak = all_formula;
            current = !current;
            all_formula = all_formulas[current];
            ////QPSLSLC ENDS
        }
    }

    fclose(f);

    ClearErr();
    ClearInfo();

    sprintf(params, "%splate.geo", _STATIC_);
    if (my_file_exists(params)) {
        //generating msh
        //  gmsh Static/plate.geo -o Static/plate.msh -2
        sprintf(params, "%splate.geo -o %splate.msh -2 > %sgmsh.log", _STATIC_, _STATIC_, _STATIC_);
        sprintf(program, "%sgmsh", _ELMER_);
        //execute gmsh
#ifdef LINUX
        runcode = SystemSilent(program, params);
        runcode_short = runcode >> 8;
#else
        runcode = RunSilent(program, params);
        runcode_short = runcode >> 8;
#endif
        printf("\ngmsh runcode:%lu runcode_short:%d\n", runcode, runcode_short);
        sprintf(params, "%splate.msh", _STATIC_);
        if ((runcode_short > 1/*!= 0*/) || (!my_file_exists(params))) {
            ret = ask_question(2, (char *) "Log", (char *) "OK", (char *) "", (char *) "gmsh", 12, (char *) _gmsh_error_,
                               11, 1, 0);
            if (ret==0)
            {
                //show log gmsh.log
                sprintf(params, "%sgmsh.log", _STATIC_);
                ret = EditFile(params, 0, mynCmdShow);
            }
            goto pl_error;
        }

        if (my_file_exists(params)) {
            //generating mesh
            // ElmerGrid 14 2 Static/plate.msh -out Static/plate
            sprintf(params, "14 2 %splate.msh -out %splate -unite > %sgrid.log", _STATIC_, _STATIC_, _STATIC_);  //-merge 1.e-8
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
            sprintf(params, "%s%smesh.header", _STATIC_, _plate_);
            if ((runcode_short != 0) || (!my_file_exists(params))) {
                ret = ask_question(2, (char *) "Log", (char *) "OK", (char *) "", (char *) "ElmerGrid", 12,
                                   (char *) _ElmerGrid_error_, 11, 1, 0);
                if (ret==0)
                {
                    //show log grid.log
                    sprintf(params, "%sgrid.log", _STATIC_);
                    ret = EditFile(params, 0, mynCmdShow);
                }
                goto pl_error;
            }

            if (my_file_exists(params)) {
                sprintf(params, "%s%splate.sif  > %ssolver.log", _STATIC_, _plate_, _STATIC_);
                //execute ElmerSolver, it will save results in plate folder
#ifdef LINUX
                sprintf(program, "%s%sElmerSolver", _ELMER_, _BIN_);
                runcode = SystemSilent(program, params);
                runcode_short = runcode >> 8;
#else
                sprintf(program,"%s%sElmerSolver.exe", _ELMER_, _BIN_);
                runcode = RunSilent(program, params);
                runcode_short = runcode >> 8;
#endif
                printf("\nElmerSolver runcode:%lu runcode_short:%d\n", runcode, runcode_short);
                sprintf(params, "%splate_sls.result", _plate_);
                if ((runcode_short != 0) || (!my_file_exists(params))) {
                    ret = ask_question(2, (char *) "Log", (char *) "OK", (char *) "", (char *) "ElmerSolver", 12,
                                       (char *) _ElmerSolver_error_, 11, 1, 0);
                    if (ret==0)
                    {
                        //show log solver.log
                        sprintf(params, "%ssolver.log", _STATIC_);
                        ret = EditFile(params, 0, mynCmdShow);
                    }
                    goto pl_error;
                }

            }
        }
    }

    //getting geometry
    //mesh.header
    //5079   9894   592   //number of nodes in mesh.nodes   number of elements in mesh.elements  number of boundaries in mesh.baunday
    //3  //Maximum Element Nodes
    //101    26  //number of baundary nodes - can be ignored
    //202    566  //number of baundary elements - to read
    //303    9894  //number of all elements - alread read in first row
    //nodes -
    sprintf(params, "%s%smesh.header", _STATIC_, _plate_);
    f = fopen(params, "rt");

    if (f == NULL) {
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_OPEN_RESULTS_FILE_, 12,
                           (char *) "mesh.header", 11, 1,
                           62);
        no_error = FALSE;
        goto pl_error;
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

    mesh_node = (MESH_NODE *) malloc(mesh_nodes_no * sizeof(MESH_NODE) + 100);
    mesh_element = (MESH_ELEMENT *) malloc(mesh_elements_no * sizeof(MESH_ELEMENT) + 100);
    mesh_boundary = (MESH_BOUNDARY *) malloc(mesh_boundaries_no * sizeof(MESH_BOUNDARY) + 1000);

    MESH_BOUNDARY mesh_boundary_ini={0,0,0,0,0,0,0,0,0,0.0,0.0,0.0,0.0,0.0,0.0,
        {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
        {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}};


    for (i=0; i<mesh_boundaries_no; i++)
        memmove(&mesh_boundary[i], &mesh_boundary_ini, sizeof(MESH_BOUNDARY));

    sprintf(params, "%s%smesh.nodes", _STATIC_, _plate_);
    f = fopen(params, "rt");

    if (f == NULL) {
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_OPEN_RESULTS_FILE_, 12,
                           (char *) "mesh.nodes", 11, 1,
                           62);
        no_error = FALSE;
        goto pl_error;
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
            goto pl_error1;
        }
        mesh_node[mesh_node_no].restraint=0;
        mesh_node_no++;
    }
    fclose(f);

    sprintf(params, "%s%smesh.elements", _STATIC_, _plate_);
    f = fopen(params, "rt");

    if (f == NULL) {
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_OPEN_RESULTS_FILE_, 12,
                           (char *) "mesh.elements", 11, 1,
                           62);
        no_error = FALSE;
        goto pl_error;
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
            goto pl_error1;
        }

        //calculate area of element
        area=calculateTriangleArea(mesh_node[mesh_element[mesh_element_no].node1-1].x, mesh_node[mesh_element[mesh_element_no].node1-1].y,
                                   mesh_node[mesh_element[mesh_element_no].node2-1].x, mesh_node[mesh_element[mesh_element_no].node2-1].y,
                                   mesh_node[mesh_element[mesh_element_no].node3-1].x, mesh_node[mesh_element[mesh_element_no].node3-1].y);
        body_area[mesh_element[mesh_element_no].body_number-1]+=area;

        mesh_element_no++;
    }
    fclose(f);

    printf("TRACE A\n");

    sprintf(params, "%s%smesh.boundary", _STATIC_, _plate_);
    f = fopen(params, "rt");

    if (f == NULL) {
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_OPEN_RESULTS_FILE_, 12,
                           (char *) "mesh.boundary", 11, 1,
                           62);
        no_error = FALSE;
        goto pl_error;
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
            goto pl_error1;
        }

        for (i = 0; i < pl_edge_no; i++)
        {
            if ((pl_edge[i].k+1)==mesh_boundary[mesh_boundary_no].edge)
            {
                mesh_boundary[mesh_boundary_no].restraint = pl_edge[i].restraint;
                mesh_boundary[mesh_boundary_no].inverted = pl_edge[i].inverted;
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
            mesh_boundary[mesh_boundary_no].kos = sin(PLB.kat * Pi / 180);
            mesh_boundary[mesh_boundary_no].koc = cos(PLB.kat * Pi / 180);
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

        mesh_boundary_no++;
    }
    fclose(f);

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
            goto pl_error1;
        }
        if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

            ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_, 12,
                               (char *) "", 11, 1, 62);
            no_error = FALSE;
            goto pl_error1;
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
        goto pl_error1;
    }
    GEO_LINE_MAX = 1000;
    geo_line_no = 0;
    geo_line = (GEO_LINE *) malloc(GEO_LINE_MAX * sizeof(GEO_LINE) + 100);
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
        goto pl_error1;
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

    for (i = 0; i < pl_node_no; i++) {
        T.x = (float) pl_node[i].x;
        T.y = (float) pl_node[i].y;
        sprintf(T.text, "%d", i + 1);

        T.dl = strlen(T.text);
        T.n = T18 + T.dl;
        normalize_txt(&T);
        adr = dodaj_obiekt((BLOK *) dane, (void *) &T);
    }

    T.italics = 1;
    T.underline = 1;
    /*
    for (i=0; i<pl_edge_no; i++)
    {
        T.x=(float)((pl_node[pl_edge[i].node1].x+pl_node[pl_edge[i].node2].x)/2.);
        T.y=(float)((pl_node[pl_edge[i].node1].y+pl_node[pl_edge[i].node2].y)/2.);

        sprintf(T.text, "%d", i);

        T.dl = strlen(T.text);
        T.n = T18 + T.dl;
        normalize_txt(&T);
        adr = dodaj_obiekt((BLOK *) dane, (void *) &T);
    }
    */

    for (i = 0; i < pl_edge_no; i++) {
        Lt.x1 = (float) pl_node[pl_edge[i].node1].x;
        Lt.y1 = (float) pl_node[pl_edge[i].node1].y;
        Lt.x2 = (float) pl_node[pl_edge[i].node2].x;
        Lt.y2 = (float) pl_node[pl_edge[i].node2].y;

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

        sprintf(T.text, "%d", i + 1);

        T.dl = strlen(T.text);
        T.n = T18 + T.dl;

        normalize_txt(&T);
        adr = dodaj_obiekt((BLOK *) dane, (void *) &T);
    }

    //mesh nodes
    sprintf(block_name, "%s$Mesh_Nodes", ptr_id_short);

    ptr_block = add_block(blok_origin.x, blok_origin.y, 'E', block_name, FALSE);
    if (ptr_block == NULL) {
        fclose(f);
        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "",
                           (char *) _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_, 12, (char *) "", 11, 1, 62);
        no_error = FALSE;
        goto pl_error1;
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
        adr = dodaj_obiekt((BLOK *) dane, (void *) &T);
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
        adr = dodaj_obiekt((BLOK *) dane, (void *) &T);
    }

    free(geo_line);
    geo_line = NULL;

    //consolidation of results
    //plate_ulslc.result
    //plate_slslc.result
    //plate_qpslslc.result
    //will be created

    //deflection_ini = 0;
    //stress_ini = 0;
    //epsilon_ini = 0;


    //let's create body_height table
    int max_body_no=plate_property[plate_no-1].k+1;

    body_property = (int*)malloc(max_body_no*sizeof(int));

    for (i=0; i<max_body_no; i++) body_property[i]=-1;

    //let's prepare table body_h
    /*   //ignoring walls
    for (i=0; i<wall_no; i++)
    {
        //searching for properties
        for (j = 0; j < pl_property_no; j++) {
            if (pl_property[j].n == wall_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        if (j<pl_property_no)  //found it
        {
            body_property[wall_property[i].k]=this_property;
        }
    }
     */
    for (i=0; i<zone_no; i++)
    {
        //searching for properties
        for (j = 0; j < pl_property_no; j++) {
            if (pl_property[j].n == zone_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        if (j<pl_property_no)  //found it
        {
            body_property[zone_property[i].k]=this_property;
        }
    }
    for (i=0; i<plate_no; i++)
    {
        //searching for properties
        for (j = 0; j < pl_property_no; j++) {
            if (pl_property[j].n == plate_property[i].property_number) {
                this_property = j;
                break;
            }
        }
        if (j<pl_property_no)  //found it
        {
            body_property[plate_property[i].k]=this_property;
        }
    }

    /////////////////
    //setting cross section height for each element, for shear and reactions purpose
    for (i=0; i<mesh_element_no; i++)
    {
        body_prop_no = body_property[mesh_element[i].body_number - 1];
        if (body_prop_no > -1)
            body_prop = &pl_property[body_prop_no];
        else body_prop = NULL;

        if (body_prop != NULL) //WARNING, RC_flag should be inherited from plate
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
        double *supp1_sigx_max=(double*)malloc(mesh_boundaries_no*sizeof(double));
        double *supp1_sigy_max=(double*)malloc(mesh_boundaries_no*sizeof(double));
        double *supp1_sigxy_max=(double*)malloc(mesh_boundaries_no*sizeof(double));

        double *supp2_sigx_max=(double*)malloc(mesh_boundaries_no*sizeof(double));
        double *supp2_sigy_max=(double*)malloc(mesh_boundaries_no*sizeof(double));
        double *supp2_sigxy_max=(double*)malloc(mesh_boundaries_no*sizeof(double));

        double **inside1_sigx_max=(double**)malloc(mesh_boundaries_no*sizeof(double*));
        double **inside1_sigy_max=(double**)malloc(mesh_boundaries_no*sizeof(double*));
        double **inside1_sigxy_max=(double**)malloc(mesh_boundaries_no*sizeof(double*));

        for (i = 0; i < mesh_boundaries_no; i++)
        {
            inside1_sigx_max[i] = (double *) malloc(12 * sizeof(double));
            inside1_sigy_max[i] = (double *) malloc(12 * sizeof(double));
            inside1_sigxy_max[i] = (double *) malloc(12 * sizeof(double));
        }

        double **inside2_sigx_max=(double**)malloc(mesh_boundaries_no*sizeof(double*));
        double **inside2_sigy_max=(double**)malloc(mesh_boundaries_no*sizeof(double*));
        double **inside2_sigxy_max=(double**)malloc(mesh_boundaries_no*sizeof(double*));

        for (i = 0; i < mesh_boundaries_no; i++)
        {
            inside2_sigx_max[i] = (double *) malloc(12 * sizeof(double));
            inside2_sigy_max[i] = (double *) malloc(12 * sizeof(double));
            inside2_sigxy_max[i] = (double *) malloc(12 * sizeof(double));
        }

        if (sti<2) goto singles;

        switch (sti)
        {
            case 2:
                sprintf(params0, "plate_ulslc");
                ULS_SLS_LC_NO=ULSLC_NO;
                strncpy(SLS_ULS, "ULSLC", 5);
                ULS_SLS_LC = ULSLC;
                break;
            case 3:
                sprintf(params0, "plate_slslc");
                ULS_SLS_LC_NO=SLSLC_NO;
                strncpy(SLS_ULS, "slsLC", 5);
                ULS_SLS_LC = SLSLC;
                break;
            case 4:
                sprintf(params0, "plate_qpslslc");
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

        for (int ci = 0; ci < ULS_SLS_LC_NO; ci++)
        {
            if (ULS_SLS_LC[ci].flag == 2)
            {
                file_no++;
                //sprintf(params, "%s%s%d.result", _plate_, params0, file_no);
                sprintf(params, "%s%s%d.result", _plate_, params0, ci + 1);
                f = fopen(params, "rt");

                if (f != NULL)
                {
                    /////////////////////////
                    //file exists
                    //restoring total_load for a case
                    total_load = 0.0;
                    int first_case_file = case_files[sti];

                    for (i = 0; i < zone_no; i++) {
                        total_load += surface_load[first_case_file + ci][zone_property[i].k] * body_area[zone_property[i].k];
                    }
                    for (i = 0; i < plate_no; i++) {
                        total_load += surface_load[first_case_file + ci][plate_property[i].k] * body_area[plate_property[i].k];
                    }
                    total_load += point_load[first_case_file + ci - 1];  //so we have total load for each combination of each state

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
                                perm_d = (int *) malloc(perm_d_node_no * sizeof(int) + 100);
                                for (i = 0; i < perm_d_node_no; i++) perm_d[i] = -1;
                                perm_d_b = 1;
                                for (i = 0; i < perm_d_index_no; i++) {
                                    fgets(data_row, MaxTextLen, f);
                                    ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                                    //perm_d[node_number-1]=index_number-1;
                                    perm_d[node_number - 1] = i;
                                }
                                for (i = 0; i < DEFLECTION_NUMBER; i++) {
                                    deflection[i] = (double *) malloc(perm_d_index_no * sizeof(double) + 100);
                                    deflection_min[i] = BIGNOF;
                                    deflection_max[i] = -BIGNOF;
                                    if (!deflection_ini) {
                                        min_deflection_min[i] = BIGNOF;
                                        max_deflection_max[i] = -BIGNOF;
                                        min_deflection[i] = (double *) malloc(perm_d_index_no * sizeof(double) + 100);
                                        max_deflection[i] = (double *) malloc(perm_d_index_no * sizeof(double) + 100);

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
                                                    perm_d = (int *) malloc(perm_d_node_no * sizeof(int) + 100);
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
                                    perm_s = (int *) malloc(perm_s_node_no * sizeof(int) + 100);
                                    for (i = 0; i < perm_d_node_no; i++) perm_s[i] = -1;
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
                                stress[i] = (double *) malloc(perm_s_index_no * sizeof(double) + 100);
                                stress_min[i] = BIGNOF;
                                stress_max[i] = -BIGNOF;
                                if (!stress_ini) {
                                    min_stress_min[i] = BIGNOF;
                                    max_stress_max[i] = -BIGNOF;
                                    min_stress[i] = (double *) malloc(perm_s_index_no * sizeof(double) + 100);
                                    max_stress[i] = (double *) malloc(perm_s_index_no * sizeof(double) + 100);

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
                                                perm_s = (int *) malloc(perm_s_node_no * sizeof(int) + 100);
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
                                    double H = 0.15;
                                    double h[12] = {0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15,
                                                    0.15,
                                                    0.15, 0.15, 0.15}; //TEMPORARY

#ifdef REACTIONSNSEW
                                    if ((stress[j] != NULL) && (j == 1)) //stress_yy
                                    {
                                        for (int jk = 0; jk < mesh_boundaries_no; jk++) {
                                            if (mesh_boundary[jk].restraint > 5)  //6 simple supported, 7 fixed
                                            {
                                                if (mesh_boundary[jk].node1 > 0)  //node exists
                                                {
                                                    int adj_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                                    int num_adj_supp = 0;
                                                    int inside_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                                    int num_inside = 0;
                                                    int num_inside_NSEW = 4;
                                                    int node1 = 0, node2 = 0;
                                                    int jj;
                                                    double stress1, stress2;


                                                    //searching for all elements containing node1 for reaction procedure
                                                    for (jj = 0; jj < mesh_element_no; jj++)
                                                    {
                                                        int found = 0;
                                                        if (mesh_element[jj].node1 == mesh_boundary[jk].node1)
                                                        {
                                                            node1 = mesh_element[jj].node2;
                                                            node2 = mesh_element[jj].node3;
                                                            inside_list[num_inside] = jj;
                                                            num_inside++;
                                                            found = 1;
                                                        }
                                                        else if  (mesh_element[jj].node2 == mesh_boundary[jk].node1)
                                                        {
                                                            node1 = mesh_element[jj].node1;
                                                            node2 = mesh_element[jj].node3;
                                                            inside_list[num_inside] = jj;
                                                            num_inside++;
                                                            found = 1;
                                                        }
                                                        else if (mesh_element[jj].node3 == mesh_boundary[jk].node1)
                                                        {
                                                            node1 = mesh_element[jj].node1;
                                                            node2 = mesh_element[jj].node2;
                                                            inside_list[num_inside] = jj;
                                                            num_inside++;
                                                            found = 1;
                                                        }
                                                        if (found)
                                                        {
                                                            //check if node1 and node2 are adjacent supported nodes, or inside (non-supported) neighboring nodes
                                                            if (node1 > 0)
                                                            {
                                                                if (mesh_node[node1 - 1].restraint > 5)
                                                                {
                                                                    int kk;
                                                                    for (kk = 0; kk < num_adj_supp; kk++)
                                                                        if (adj_list[kk] == node1) break;
                                                                    if (kk == num_adj_supp)  //not found
                                                                    {
                                                                        adj_list[num_adj_supp] = node1;
                                                                        num_adj_supp++;
                                                                    }
                                                                }
                                                            }
                                                            if (node2 > 0)
                                                            {
                                                                if (mesh_node[node2 - 1].restraint > 5)
                                                                {
                                                                    int kk;
                                                                    for (kk = 0; kk < num_adj_supp; kk++)
                                                                        if (adj_list[kk] == node2) break;
                                                                    if (kk == num_adj_supp)  //not found
                                                                    {
                                                                        adj_list[num_adj_supp] = node2;
                                                                        num_adj_supp++;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                    //calculating stresses for node1 in reaction procedure
                                                    double supp_x, supp_y;      // Supported node coordinates (m)
                                                    //double supp_sigx_min=0.0, supp_sigy_min=0.0;
                                                    //double supp_sigx_max=0.0, supp_sigy_max=0.0;

                                                    double adj_supp_x[12];         // Array: x-coords of adj supported nodes
                                                    double adj_supp_y[12];

                                                    double inside_x[12];           // Array: x-coords of inside nodes
                                                    double inside_y[12];

                                                    //double h[12] = {0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15,
                                                    //                          0.15,
                                                    //                          0.15, 0.15, 0.15}; //TEMPORARY

                                                    //double Q_vertical_min;             // Output: shear per unit length Q_x (N/m) for vertical edge (x=const); 0 if no vertical edge
                                                    //double Q_vertical_max;
                                                    //double Q_horizontal_min;
                                                    //double Q_horizontal_max;
                                                    double Q_foo;

                                                    double Rz;  //reaction
                                                    double Rzmax, Rzmin;
                                                    BOOL is_simple;

                                                    supp_x = mesh_node[mesh_boundary[jk].node1 - 1].x;
                                                    supp_y = mesh_node[mesh_boundary[jk].node1 - 1].y;

                                                    for (jj = 0; jj < mesh_element_no; jj++) {
                                                        if (mesh_element[jj].node1 == mesh_boundary[jk].node1)
                                                        {
                                                            supp1_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            supp1_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            break;
                                                        } else if (mesh_element[jj].node2 == mesh_boundary[jk].node1)
                                                        {
                                                            supp1_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            supp1_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            break;
                                                        } else if (mesh_element[jj].node3 == mesh_boundary[jk].node1)
                                                        {

                                                            supp1_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            supp1_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            break;
                                                        }
                                                    }

                                                    for (jj = 0; jj < num_adj_supp; jj++) {
                                                        adj_supp_x[jj] = mesh_node[adj_list[jj] - 1].x;
                                                        adj_supp_y[jj] = mesh_node[adj_list[jj] - 1].y;
                                                    }

                                                    //calculating stresses for node1 in reaction procedure
                                                    //we have :
                                                    // inside_list[num_inside] = jj;
                                                    // num_inside++;
                                                    //mesh_boundary point 1 coordinates
                                                    // mesh_boundary[jk].x1;
                                                    // mesh_boundary[jk].y1;
                                                    // we create virtual points where NSEW stresses will be calculated from mesh elements stress interpolation
                                                    // first we try with distance NSEW_dist equal half of the distance between current boundary point and the next one
                                                    // or if this is the last point in boundary series - distance between previous boundary point and the current one
                                                    double NSEW_dist;
                                                    POINTF pa, pb, pc, pp;
                                                    double alpha, beta, gamma;
                                                    //double s_value;

                                                    if (mesh_boundary[jk].node2 > 0)  //next node exists
                                                    {
                                                        NSEW_dist = sqrt((mesh_boundary[jk].x2 - mesh_boundary[jk].x1) * (mesh_boundary[jk].x2 - mesh_boundary[jk].x1) +
                                                                         (mesh_boundary[jk].y2 - mesh_boundary[jk].y1) * (mesh_boundary[jk].y2 - mesh_boundary[jk].y1)) / NSEW_dist_div;
                                                    } else if ((jk > 0) && (mesh_boundary[jk - 1].node1 > 0))  //previous node exists
                                                    {
                                                        NSEW_dist = sqrt((mesh_boundary[jk].x1 - mesh_boundary[jk - 1].x1) * (mesh_boundary[jk].x1 - mesh_boundary[jk - 1].x1) +
                                                                         (mesh_boundary[jk].y1 - mesh_boundary[jk - 1].y1) * (mesh_boundary[jk].y1 - mesh_boundary[jk - 1].y1)) / NSEW_dist_div;
                                                    } else  //in case there is single point
                                                    {
                                                        if (UNITS == SI)
                                                            NSEW_dist = 0.1;  //10 cm
                                                        else
                                                            NSEW_dist = 4.;   //4 inches
                                                    }
                                                    //searching for element which contains point NORTH, SOUTH, EAST and WETS
                                                    int y_offset_i[4] = {1, -1, 0, 0};  //1 EAST, -1 WETS 0 NONE
                                                    int x_offset_i[4] = {0, 0, 1, -1};  //0 NONE, 1 NORTH, -1 SOUTH
                                                    int stress_selector[4] = {1, 1, 0, 0};  //1 stress_yy, 0 stress_xx

                                                    for (int kk=0; kk<4; kk++) {
                                                        inside1_sigx_max[jk][kk] = 0.;
                                                        inside1_sigy_max[jk][kk] = 0.;
                                                    }

                                                    num_inside_NSEW = 4;
                                                    int b_inside[4] = {0, 0, 0, 0};

                                                    for (int kk = 0; kk < 4; kk++)   //N, S, E, W
                                                    {
                                                        pp.x = (float) (mesh_boundary[jk].x1 + NSEW_dist * x_offset_i[kk]);
                                                        pp.y = (float) (mesh_boundary[jk].y1 + NSEW_dist * y_offset_i[kk]);

                                                        for (jj = 0; jj < num_inside; jj++)
                                                        {
                                                            pa.x = (float) mesh_node[mesh_element[inside_list[jj]].node1 - 1].x;
                                                            pa.y = (float) mesh_node[mesh_element[inside_list[jj]].node1 - 1].y;

                                                            pb.x = (float) mesh_node[mesh_element[inside_list[jj]].node2 - 1].x;
                                                            pb.y = (float) mesh_node[mesh_element[inside_list[jj]].node2 - 1].y;

                                                            pc.x = (float) mesh_node[mesh_element[inside_list[jj]].node3 - 1].x;
                                                            pc.y = (float) mesh_node[mesh_element[inside_list[jj]].node3 - 1].y;

                                                            //if (is_point_in_triangle(pa, pb, pc, pp))
                                                            if (isInside(&pa, &pb, &pc, &pp, &alpha, &beta, &gamma)) {
                                                                inside_x[kk] = (mesh_boundary[jk].x1 + NSEW_dist * x_offset_i[kk]);
                                                                inside_y[kk] = (mesh_boundary[jk].y1 + NSEW_dist * y_offset_i[kk]);
                                                                b_inside[kk] = 1;
                                                                //point inside, so now interpolation of stress_yy
                                                                // Calculate and return the weighted average value
                                                                if (stress_selector[kk]==0)
                                                                    inside1_sigx_max[jk][kk] = ((alpha * stress[0][perm_s[mesh_element[inside_list[jj]].node1 - 1]]) + (beta * stress[0][perm_s[mesh_element[inside_list[jj]].node2 - 1]]) + (gamma * stress[0][perm_s[mesh_element[inside_list[jj]].node3 - 1]]));
                                                                else
                                                                    inside1_sigy_max[jk][kk] = ((alpha * stress[1][perm_s[mesh_element[inside_list[jj]].node1 - 1]]) + (beta * stress[1][perm_s[mesh_element[inside_list[jj]].node2 - 1]]) + (gamma * stress[1][perm_s[mesh_element[inside_list[jj]].node3 - 1]]));
                                                                break;
                                                            }
                                                        }
                                                    }

                                                    int sti5 = 5;
                                                    //calculating reactions for node1 in reaction procedure

                                                    //// REACTION CAL:CULATION FOR NODE 1

                                                    is_simple = (mesh_boundary[jk].restraint < 7) ? 1 : 0;
                                                    point_b.x = mesh_boundary[jk].x1;
                                                    point_b.y = mesh_boundary[jk].y1;
                                                    point_c.x = mesh_boundary[jk].x2;
                                                    point_c.y = mesh_boundary[jk].y2;

                                                    mesh_boundary[jk].state1[sti5].Rz = compute_NSEW_reaction(point_b, point_c, mesh_boundary[jk].kos, mesh_boundary[jk].koc, H, is_simple, supp_x, supp_y, supp1_sigx_max[jk], supp1_sigy_max[jk], num_adj_supp, adj_supp_x, adj_supp_y, num_inside_NSEW, b_inside, h, inside_x, inside_y, inside1_sigx_max[jk], inside1_sigy_max[jk], &mesh_boundary[jk].state1[sti5].Qn);

                                                    //if (last_node != mesh_boundary[jk].node1) {
                                                        printf("node=%d\n", mesh_boundary[jk].node1);
                                                        printf("H=%f\n", H);
                                                        printf("supp.x=%f  supp.y=%f  supp1_sigx=%f  supp1_sigy=%f\n", supp_x, supp_y, supp1_sigx_max[j], supp1_sigy_max[j]);
                                                        printf("num_adj_supp=%d\n", num_adj_supp);
                                                        for (int iii = 0; iii < num_adj_supp; iii++) {
                                                            printf("adj_supp_x[%d]=%f  adj_supp_y[%d]=%f\n", iii, adj_supp_x[iii], iii, adj_supp_y[iii]);
                                                        }
                                                        printf("num_inside_NSEW=%d\n", num_inside_NSEW);
                                                        for (int iii = 0; iii < num_inside_NSEW; iii++) {
                                                            if (b_inside[iii])
                                                            printf("inside_x[%d]=%f  inside_y[%d]=%f  inside1_sigx[%d]=%f  inside1_sigy[%d]=%f\n", iii, inside_x[iii], iii, inside_y[iii], iii, inside1_sigx_max[jk][iii], iii, inside1_sigy_max[jk][iii]);
                                                            else printf("[%d] ignored\n",iii);
                                                        }

                                                        printf("Results: Qn=%f Rz=%f\n\n", mesh_boundary[jk].state1[sti5].Qn, mesh_boundary[jk].state1[sti5].Rz);
                                                    //}
                                                    last_node = mesh_boundary[jk].node1;

                                                }  //node1

                                                if (mesh_boundary[jk].node2 > 0)  //node exists
                                                {
                                                    int adj_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                                    int num_adj_supp = 0;
                                                    int inside_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                                    int num_inside = 0;
                                                    int num_inside_NSEW = 4;
                                                    int node1 = 0, node2 = 0;
                                                    int jj;
                                                    double stress1, stress2;

                                                    //searching for all elements containing node2 for reaction procedure

                                                    for (jj = 0; jj < mesh_element_no; jj++)
                                                    {
                                                        int found = 0;
                                                        if (mesh_element[jj].node1 == mesh_boundary[jk].node2)
                                                        {
                                                            node1 = mesh_element[jj].node2;
                                                            node2 = mesh_element[jj].node3;
                                                            inside_list[num_inside] = jj;
                                                            num_inside++;
                                                            found = 1;
                                                        }
                                                        else if  (mesh_element[jj].node2 == mesh_boundary[jk].node2)
                                                        {
                                                            node1 = mesh_element[jj].node1;
                                                            node2 = mesh_element[jj].node3;
                                                            inside_list[num_inside] = jj;
                                                            num_inside++;
                                                            found = 1;
                                                        }
                                                        else if (mesh_element[jj].node3 == mesh_boundary[jk].node2)
                                                        {
                                                            node1 = mesh_element[jj].node1;
                                                            node2 = mesh_element[jj].node2;
                                                            inside_list[num_inside] = jj;
                                                            num_inside++;
                                                            found = 1;
                                                        }
                                                        if (found)
                                                        {
                                                            //check if node1 and node2 are adjacent supported nodes, or inside (non-supported) neighboring nodes
                                                            if (node1 > 0)
                                                            {
                                                                if (mesh_node[node1 - 1].restraint > 5)
                                                                {
                                                                    int kk;
                                                                    for (kk = 0; kk < num_adj_supp; kk++)
                                                                        if (adj_list[kk] == node1) break;
                                                                    if (kk == num_adj_supp)  //not found
                                                                    {
                                                                        adj_list[num_adj_supp] = node1;
                                                                        num_adj_supp++;
                                                                    }
                                                                }
                                                            }
                                                            if (node2 > 0)
                                                            {
                                                                if (mesh_node[node2 - 1].restraint > 5)
                                                                {
                                                                    int kk;
                                                                    for (kk = 0; kk < num_adj_supp; kk++)
                                                                        if (adj_list[kk] == node2) break;
                                                                    if (kk == num_adj_supp)  //not found
                                                                    {
                                                                        adj_list[num_adj_supp] = node2;
                                                                        num_adj_supp++;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                    //calculating stresses for node2 in reaction procedure

                                                    double supp_x, supp_y;      // Supported node coordinates (m)
                                                    //double supp_sigx_min=0.0, supp_sigy_min=0.0;
                                                    //double supp_sigx_max=0.0, supp_sigy_max=0.0;

                                                    double adj_supp_x[12];         // Array: x-coords of adj supported nodes
                                                    double adj_supp_y[12];

                                                    double inside_x[12];           // Array: x-coords of inside nodes
                                                    double inside_y[12];

                                                    double Q_foo;

                                                    double Rz;  //reaction
                                                    double Rzmax, Rzmin;
                                                    BOOL is_simple;

                                                    supp_x = mesh_node[mesh_boundary[jk].node2 - 1].x;
                                                    supp_y = mesh_node[mesh_boundary[jk].node2 - 1].y;

                                                    for (jj = 0; jj < mesh_element_no; jj++) {
                                                        if (mesh_element[jj].node1 == mesh_boundary[jk].node2)
                                                        {
                                                            supp2_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            supp2_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            break;
                                                        } else if (mesh_element[jj].node2 == mesh_boundary[jk].node2)
                                                        {
                                                            supp2_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            supp2_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            break;
                                                        } else if (mesh_element[jj].node3 == mesh_boundary[jk].node2) {
                                                            supp2_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            supp2_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            break;
                                                        }
                                                    }

                                                    for (jj = 0; jj < num_adj_supp; jj++) {
                                                        adj_supp_x[jj] = mesh_node[adj_list[jj] - 1].x;
                                                        adj_supp_y[jj] = mesh_node[adj_list[jj] - 1].y;
                                                    }

                                                    //calculating stresses for node1 in reaction procedure
                                                    //we have :
                                                    // inside_ist[num_inside] = jj;
                                                    // num_inside++;
                                                    //mesh_boundary point 1 coordinates
                                                    // mesh_boundary[jk].x1;
                                                    // mesh_boundary[jk].y1;
                                                    // we create virtual points where NSEW stresses will be calculated from mesh elements stress interpolation
                                                    // first we try with distance NSEW_dist equal half of the distance between current boundary point and the next one
                                                    // or if this is the last point in boundary series - distance between previous boundary point and the current one
                                                    double NSEW_dist;
                                                    POINTF pa, pb, pc, pp;
                                                    double alpha, beta, gamma;
                                                    //double s_value;

                                                    if ((jk < (mesh_boundary_no - 1)) && (mesh_boundary[jk + 1].node2 > 0))  //node 2 of next boundary exists
                                                    {
                                                        NSEW_dist = sqrt((mesh_boundary[jk + 1].x2 - mesh_boundary[jk].x2) * (mesh_boundary[jk + 1].x2 - mesh_boundary[jk].x2) +
                                                                         (mesh_boundary[jk + 1].y2 - mesh_boundary[jk].y2) * (mesh_boundary[jk + 1].y2 - mesh_boundary[jk].y2)) / NSEW_dist_div;
                                                    } else if (mesh_boundary[jk].node1 > 0) //previous node exists
                                                    {
                                                        NSEW_dist = sqrt((mesh_boundary[jk].x2 - mesh_boundary[jk].x1) * (mesh_boundary[jk].x2 - mesh_boundary[jk].x1) +
                                                                         (mesh_boundary[jk].y2 - mesh_boundary[jk].y1) * (mesh_boundary[jk].y2 - mesh_boundary[jk].y1)) / NSEW_dist_div;
                                                    } else  //in case there is single point
                                                    {
                                                        if (UNITS == SI)
                                                            NSEW_dist = 0.1;  //10 cm
                                                        else
                                                            NSEW_dist = 4.;   //4 inches
                                                    }
                                                    //searching for element which contains point NORTH, SOUTH, EAST and WETS
                                                    int y_offset_i[4] = {1, -1, 0, 0};  //1 EAST, -1 WETS 0 NONE
                                                    int x_offset_i[4] = {0, 0, 1, -1};  //0 NONE, 1 NORTH, -1 SOUTH
                                                    int stress_selector[4] = {1, 1, 0, 0};  //1 stress_yy, 0 stress_xx

                                                    for (int kk=0; kk<4; kk++) {
                                                        inside2_sigx_max[jk][kk] = 0.;
                                                        inside2_sigy_max[jk][kk] = 0.;
                                                    }

                                                    num_inside_NSEW = 4;
                                                    int b_inside[4] = {0, 0, 0, 0};

                                                    for (int kk = 0; kk < 4; kk++)   //N, S, E, W
                                                    {
                                                        pp.x = (float) (mesh_boundary[jk].x2 + NSEW_dist * x_offset_i[kk]);
                                                        pp.y = (float) (mesh_boundary[jk].y2 + NSEW_dist * y_offset_i[kk]);

                                                        for (jj = 0; jj < num_inside; jj++)
                                                        {
                                                            pa.x = (float) mesh_node[mesh_element[inside_list[jj]].node1 - 1].x;
                                                            pa.y = (float) mesh_node[mesh_element[inside_list[jj]].node1 - 1].y;

                                                            pb.x = (float) mesh_node[mesh_element[inside_list[jj]].node2 - 1].x;
                                                            pb.y = (float) mesh_node[mesh_element[inside_list[jj]].node2 - 1].y;

                                                            pc.x = (float) mesh_node[mesh_element[inside_list[jj]].node3 - 1].x;
                                                            pc.y = (float) mesh_node[mesh_element[inside_list[jj]].node3 - 1].y;

                                                            //if (is_point_in_triangle(pa, pb, pc, pp))
                                                            if (isInside(&pa, &pb, &pc, &pp, &alpha, &beta, &gamma))
                                                            {
                                                                inside_x[kk] = (mesh_boundary[jk].x2 + NSEW_dist * x_offset_i[kk]);
                                                                inside_y[kk] = (mesh_boundary[jk].y2 + NSEW_dist * y_offset_i[kk]);
                                                                b_inside[kk] = 1;
                                                                //point inside, so now interpolation of stress_yy
                                                                // Calculate and return the weighted average value
                                                                if (stress_selector[kk]==0)
                                                                    inside2_sigx_max[jk][kk] = ((alpha * stress[0][perm_s[mesh_element[inside_list[jj]].node1 - 1]]) + (beta * stress[0][perm_s[mesh_element[inside_list[jj]].node2 - 1]]) + (gamma * stress[0][perm_s[mesh_element[inside_list[jj]].node3 - 1]]));
                                                                else
                                                                    inside2_sigy_max[jk][kk] = ((alpha * stress[1][perm_s[mesh_element[inside_list[jj]].node1 - 1]]) + (beta * stress[1][perm_s[mesh_element[inside_list[jj]].node2 - 1]]) + (gamma * stress[1][perm_s[mesh_element[inside_list[jj]].node3 - 1]]));
                                                                break;
                                                            }
                                                        }
                                                    }

                                                    int sti5 = 5;
                                                    //calculating reactions for node1 in reaction procedure
                                                    //// REACTION CAL:CULATION FOR NODE 2

                                                    is_simple = (mesh_boundary[jk].restraint < 7) ? 1 : 0;
                                                    point_b.x = mesh_boundary[jk].x1;
                                                    point_b.y = mesh_boundary[jk].y1;
                                                    point_c.x = mesh_boundary[jk].x2;
                                                    point_c.y = mesh_boundary[jk].y2;

                                                    mesh_boundary[jk].state2[sti5].Rz = compute_NSEW_reaction(point_b, point_c, mesh_boundary[jk].kos, mesh_boundary[jk].koc, H, is_simple, supp_x, supp_y, supp2_sigx_max[jk], supp2_sigy_max[jk], num_adj_supp, adj_supp_x, adj_supp_y, num_inside_NSEW, b_inside, h, inside_x, inside_y, inside2_sigx_max[jk], inside2_sigy_max[jk], &mesh_boundary[jk].state2[sti5].Qn);

                                                    //if (last_node != mesh_boundary[jk].node2) {
                                                        printf("node=%d\n", mesh_boundary[jk].node2);
                                                        printf("H=%f\n", H);
                                                        printf("supp.x=%f  supp.y=%f  supp2_sigx=%f  supp2_sigy=%f\n", supp_x, supp_y, supp2_sigx_max[j], supp2_sigy_max[j]);
                                                        printf("num_adj_supp=%d\n", num_adj_supp);
                                                        for (int iii = 0; iii < num_adj_supp; iii++) {
                                                            printf("adj_supp_x[%d]=%f  adj_supp_y[%d]=%f\n", iii, adj_supp_x[iii], iii, adj_supp_y[iii]);
                                                        }
                                                        printf("num_inside_NSEW=%d\n", num_inside_NSEW);
                                                        for (int iii = 0; iii < num_inside_NSEW; iii++) {
                                                            if (b_inside[iii])
                                                            printf("inside_x[%d]=%f  inside_y[%d]=%f  inside2_sigx[%d]=%f  inside2_sigy[%d]=%f\n", iii, inside_x[iii], iii, inside_y[iii], iii, inside2_sigx_max[jk][iii], iii, inside2_sigy_max[jk][iii]);
                                                            else printf("[%d] ignored\n",iii);
                                                        }
                                                        printf("Results: Qn=%f Rz=%f\n\n", mesh_boundary[jk].state2[sti5].Qn, mesh_boundary[jk].state2[sti5].Rz);
                                                    //}
                                                    last_node = mesh_boundary[jk].node2;

                                                }  //node2
                                            }  //restrain > 5
                                            //////////////!!!!!!!!!!!!!//////////
                                        }  //jk
                                        ////////1///

                                        //stresses are completed

                                        /////////////////////////////////
                                        //REACTIONS for combination, se they are collected as state no=5 (countiny from 0)
                                        int last_node = 0;
                                        int sti5 = 5;

                                        //Rzmin = 0.0;
                                        Rzmax = 0.0;

                                        //Rzmin_back = 0.0;
                                        Rzmax_back = 0.0;

                                        printf("\nState %s-%d(%d)\n", SLS_ULS, ci + 1, ULS_SLS_LC_NO);

                                        for (i = 0; i < mesh_boundaries_no; i++) {
                                            if (mesh_boundary[i].restraint > 5) {
                                                if (mesh_boundary[i].node1 != last_node) {
                                                    printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti5].Rz, mesh_boundary[i].state1[sti5].Qn);

                                                    Rzmax += nofabs(mesh_boundary[i].state1[sti5].Rz);
                                                }
                                                last_node = mesh_boundary[i].node1;
                                                if (mesh_boundary[i].node2 != last_node) {
                                                    printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti5].Rz, mesh_boundary[i].state2[sti5].Qn);

                                                    Rzmax += nofabs(mesh_boundary[i].state2[sti5].Rz);
                                                }
                                                last_node = mesh_boundary[i].node2;
                                            }
                                        }
                                        printf("SUM:  Rz_total_i=%f\n", Rzmax);

                                        //Rzmin_back = Rzmin;
                                        Rzmax_back = Rzmax;

                                        printf("\nTotal load in state %s-%d(%d):  %f\n", SLS_ULS, ci + 1, ULS_SLS_LC_NO, total_load);

                                        load_ratio = total_load / Rzmax_back;

                                        //Rzmin = 0.0;
                                        Rzmax = 0.0;

                                        printf("\nState %s-%d(%d) with correction ratio %f\n", SLS_ULS, ci + 1, ULS_SLS_LC_NO, load_ratio);

                                        for (i = 0; i < mesh_boundaries_no; i++) {
                                            if (mesh_boundary[i].restraint > 5) {
                                                if (mesh_boundary[i].node1 != last_node) {
                                                    printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti5].Rz * load_ratio, mesh_boundary[i].state1[sti5].Qn * load_ratio);

                                                    Rzmax += (nofabs(mesh_boundary[i].state1[sti5].Rz) * load_ratio);
                                                }
                                                last_node = mesh_boundary[i].node1;
                                                if (mesh_boundary[i].node2 != last_node) {
                                                    printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti5].Rz * load_ratio, mesh_boundary[i].state2[sti5].Qn * load_ratio);

                                                    Rzmax += nofabs(mesh_boundary[i].state2[sti5].Rz) * load_ratio;
                                                }
                                                last_node = mesh_boundary[i].node2;
                                            }
                                        }
                                        printf("SUM:  Rz_total_i=%f\n", Rzmax);

                                        //upgradeing state min max
                                        for (i = 0; i < mesh_boundaries_no; i++) {
                                            if (ci == 0)  //first of combination
                                            {
                                                mesh_boundary[i].state1[sti].Qnmin = mesh_boundary[i].state1[sti5].Qn * load_ratio;
                                                mesh_boundary[i].state1[sti].Qnmax = mesh_boundary[i].state1[sti5].Qn * load_ratio;
                                                mesh_boundary[i].state1[sti].Rzmin = mesh_boundary[i].state1[sti5].Rz * load_ratio;
                                                mesh_boundary[i].state1[sti].Rzmax = mesh_boundary[i].state1[sti5].Rz * load_ratio;

                                                mesh_boundary[i].state2[sti].Qnmin = mesh_boundary[i].state2[sti5].Qn * load_ratio;
                                                mesh_boundary[i].state2[sti].Qnmax = mesh_boundary[i].state2[sti5].Qn * load_ratio;
                                                mesh_boundary[i].state2[sti].Rzmin = mesh_boundary[i].state2[sti5].Rz * load_ratio;
                                                mesh_boundary[i].state2[sti].Rzmax = mesh_boundary[i].state2[sti5].Rz * load_ratio;
                                            } else {
                                                mesh_boundary[i].state1[sti].Qnmin = min(mesh_boundary[i].state1[sti].Qnmin, mesh_boundary[i].state1[sti5].Qn * load_ratio);
                                                mesh_boundary[i].state1[sti].Qnmax = max(mesh_boundary[i].state1[sti].Qnmax, mesh_boundary[i].state1[sti5].Qn * load_ratio);
                                                mesh_boundary[i].state1[sti].Rzmin = min(mesh_boundary[i].state1[sti].Rzmin, mesh_boundary[i].state1[sti5].Rz * load_ratio);
                                                mesh_boundary[i].state1[sti].Rzmax = max(mesh_boundary[i].state1[sti].Rzmax, mesh_boundary[i].state1[sti5].Rz * load_ratio);

                                                mesh_boundary[i].state2[sti].Qnmin = min(mesh_boundary[i].state2[sti].Qnmin, mesh_boundary[i].state2[sti5].Qn * load_ratio);
                                                mesh_boundary[i].state2[sti].Qnmax = max(mesh_boundary[i].state2[sti].Qnmax, mesh_boundary[i].state2[sti5].Qn * load_ratio);
                                                mesh_boundary[i].state2[sti].Rzmin = min(mesh_boundary[i].state2[sti].Rzmin, mesh_boundary[i].state2[sti5].Rz * load_ratio);
                                                mesh_boundary[i].state2[sti].Rzmax = max(mesh_boundary[i].state2[sti].Rzmax, mesh_boundary[i].state2[sti5].Rz * load_ratio);
                                            }
                                        }
                                    }
#endif
                                    ///////////////////////// 
                                    if ((stress[j] != NULL) && (j == 3)) //stress_xy
                                    {
                                        for (int jk = 0; jk < mesh_boundaries_no; jk++)
                                        {
                                            if (mesh_boundary[jk].restraint > 5)  //6 simple supported, 7 fixed
                                            {
                                                if (mesh_boundary[jk].node1 > 0)  //node 1 exists
                                                {
                                                    int adj_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                                    int num_adj_supp = 0;
                                                    int inside_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                                    double inside_list_stress_x[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0};
                                                    double inside_list_stress_y[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0};
                                                    double inside_list_stress_xy[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0};

                                                    int num_inside = 0;
                                                    int node1 = 0, node2 = 0;
                                                    int jj;
                                                    double stress1x, stress1y, stress1xy, stress2x, stress2y, stress2xy;
                                                    //searching for all elements containing node1
                                                    for (jj = 0; jj < mesh_element_no; jj++) {
                                                        int found = 0;
                                                        if (mesh_element[jj].node1 == mesh_boundary[jk].node1) {
                                                            node1 = mesh_element[jj].node2;
                                                            node2 = mesh_element[jj].node3;

                                                            stress1x = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            stress1y = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            stress1xy = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;

                                                            stress2x = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            stress2y = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            stress2xy = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            found = 1;
                                                        } else if (mesh_element[jj].node2 == mesh_boundary[jk].node1) {
                                                            node1 = mesh_element[jj].node1;
                                                            node2 = mesh_element[jj].node3;

                                                            stress1x = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            stress1y = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            stress1xy = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;

                                                            stress2x = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            stress2y = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            stress2xy = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            found = 1;
                                                        } else if (mesh_element[jj].node3 == mesh_boundary[jk].node1) {
                                                            node1 = mesh_element[jj].node1;
                                                            node2 = mesh_element[jj].node2;

                                                            stress1x = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            stress1y = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            stress1xy = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;

                                                            stress2x = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            stress2y = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            stress2xy = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            found = 1;
                                                        }
                                                        if (found) {
                                                            //check if node1 and node2 are adjacent supported nodes, or inside (non-supported) neighboring nodes
                                                            if (node1 > 0) {
                                                                if (mesh_node[node1 - 1].restraint > 5) {
                                                                    int kk;
                                                                    for (kk = 0; kk < num_adj_supp; kk++)
                                                                        if (adj_list[kk] == node1) break;
                                                                    if (kk == num_adj_supp)  //not found
                                                                    {
                                                                        adj_list[num_adj_supp] = node1;
                                                                        num_adj_supp++;
                                                                    }
                                                                } else {
                                                                    int kk;
                                                                    for (kk = 0; kk < num_inside; kk++)
                                                                        if (inside_list[kk] == node1) break;
                                                                    if (kk == num_inside)  //not found
                                                                    {
                                                                        inside_list[num_inside] = node1;
                                                                        inside_list_stress_x[num_inside] = stress1x;
                                                                        inside_list_stress_y[num_inside] = stress1y;
                                                                        inside_list_stress_xy[num_inside] = stress1xy;
                                                                        num_inside++;
                                                                    }
                                                                }
                                                            }
                                                            if (node2 > 0) {
                                                                if (mesh_node[node2 - 1].restraint > 5) {
                                                                    int kk;
                                                                    for (kk = 0; kk < num_adj_supp; kk++)
                                                                        if (adj_list[kk] == node2) break;
                                                                    if (kk == num_adj_supp)  //not found
                                                                    {
                                                                        adj_list[num_adj_supp] = node2;
                                                                        num_adj_supp++;
                                                                    }
                                                                } else {
                                                                    int kk;
                                                                    for (kk = 0; kk < num_inside; kk++)
                                                                        if (inside_list[kk] == node2) break;
                                                                    if (kk == num_inside)  //not found
                                                                    {
                                                                        inside_list[num_inside] = node2;
                                                                        inside_list_stress_x[num_inside] = stress2x;
                                                                        inside_list_stress_y[num_inside] = stress2y;
                                                                        inside_list_stress_xy[num_inside] = stress2xy;
                                                                        num_inside++;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                    //calculating reactions for node1
                                                    double supp_x, supp_y;      // Supported node coordinates (m)

                                                    double adj_supp_x[12];         // Array: x-coords of adj supported nodes
                                                    double adj_supp_y[12];

                                                    double inside_x[12];           // Array: x-coords of inside nodes
                                                    double inside_y[12];

                                                    double H = 0.15, h[12] = {0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15,
                                                                              0.15,
                                                                              0.15, 0.15, 0.15}; //TEMPORARY

                                                    double Q_foo;

                                                    double Rz;  //reaction
                                                    double Rzmax, Rzmin;

                                                    supp_x = mesh_node[mesh_boundary[jk].node1 - 1].x;
                                                    supp_y = mesh_node[mesh_boundary[jk].node1 - 1].y;

                                                    for (jj = 0; jj < mesh_element_no; jj++) {
                                                        if (mesh_element[jj].node1 == mesh_boundary[jk].node1)
                                                        {
                                                            supp1_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            supp1_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            supp1_sigxy_max[jk] = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            break;
                                                        }
                                                        else if (mesh_element[jj].node2 == mesh_boundary[jk].node1)
                                                        {
                                                            supp1_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            supp1_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            supp1_sigxy_max[jk] = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            break;
                                                        }
                                                        else if (mesh_element[jj].node3 == mesh_boundary[jk].node1)
                                                        {
                                                            supp1_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            supp1_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            supp1_sigxy_max[jk] = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            break;
                                                        }
                                                    }

                                                    for (jj = 0; jj < num_adj_supp; jj++)
                                                    {
                                                        adj_supp_x[jj] = mesh_node[adj_list[jj] - 1].x;
                                                        adj_supp_y[jj] = mesh_node[adj_list[jj] - 1].y;
                                                    }
                                                    for (jj = 0; jj < num_inside; jj++)
                                                    {
                                                        inside_x[jj] = mesh_node[inside_list[jj] - 1].x;
                                                        inside_y[jj] = mesh_node[inside_list[jj] - 1].y;

                                                        inside1_sigx_max[jk][jj] = inside_list_stress_x[jj]; //stress1x;  //just max
                                                        inside1_sigy_max[jk][jj] = inside_list_stress_y[jj]; //stress1y;  //just max
                                                        inside1_sigxy_max[jk][jj] = inside_list_stress_xy[jj]; //stress1xy;  //just max
                                                    }

                                                    int sti5 = 5;

                                                    //// REACTIONS
                                                    mesh_boundary[jk].state1[sti5].Rz1 = compute_support_reaction(H, supp_x,supp_y,supp1_sigx_max[jk],supp1_sigy_max[jk],supp1_sigxy_max[jk],num_adj_supp,adj_supp_x,adj_supp_y,num_inside,h,inside_x,inside_y,inside1_sigx_max[jk],inside1_sigy_max[jk],inside1_sigxy_max[jk],&mesh_boundary[jk].state1[sti5].Qn1);
#ifndef NDEBUG
                                                    //if (last_node != mesh_boundary[jk].node1)
                                                    //{
                                                    printf("ALTERNATIVE\n");
                                                    printf("node=%d\n", mesh_boundary[jk].node1);
                                                    printf("H=%f\n", H);
                                                    printf("supp.x=%f  supp.y=%f  supp1_sigx=%f  supp1_sigy=%f  supp1_sigxy=%f\n",
                                                           supp_x, supp_y, supp1_sigx_max[jk], supp1_sigy_max[jk], supp1_sigxy_max[jk]);
                                                    printf("num_adj_supp=%d\n", num_adj_supp);
                                                    for (int iii = 0; iii < num_adj_supp; iii++) {
                                                        printf("adj_supp_x[%d]=%f  adj_supp_y[%d]=%f\n",iii,adj_supp_x[iii],iii, adj_supp_y[iii]);
                                                    }
                                                    printf("num_inside=%d\n", num_inside);
                                                    for (int iii = 0; iii < num_inside; iii++) {
                                                        printf("inside_x[%d]=%f  inside_y[%d]=%f  inside1_sigx[%d]=%f  inside1_sigy[%d]=%f  inside1_sigxy[%d]=%f\n",iii,inside_x[iii],iii,inside_y[iii],iii,inside1_sigx_max[j][iii],iii,inside1_sigy_max[j][iii],iii,inside1_sigxy_max[j][iii]);
                                                    }

                                                    printf("Results: Qn1=%f Rz1=%f\n\n", mesh_boundary[jk].state1[sti].Qn1, mesh_boundary[jk].state1[sti].Rz1);
                                                    //}
                                                    //last_node=mesh_boundary[jk].node1;
#endif
                                                }


                                                if (mesh_boundary[jk].node2 > 0)  //node 2 exists
                                                {
                                                    int adj_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                                    int num_adj_supp = 0;
                                                    int inside_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                                    double inside_list_stress_x[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0};
                                                    double inside_list_stress_y[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0};
                                                    double inside_list_stress_xy[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0};

                                                    int num_inside = 0;
                                                    int node1 = 0, node2 = 0;
                                                    int jj;
                                                    double stress1x, stress1y, stress1xy, stress2x, stress2y, stress2xy;
                                                    //searching for all elements containing node1
                                                    for (jj = 0; jj < mesh_element_no; jj++) {
                                                        int found = 0;
                                                        if (mesh_element[jj].node1 == mesh_boundary[jk].node2) {
                                                            node1 = mesh_element[jj].node2;
                                                            node2 = mesh_element[jj].node3;

                                                            stress1x = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            stress1y = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            stress1xy = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;

                                                            stress2x = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            stress2y = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            stress2xy = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            found = 1;
                                                        } else if (mesh_element[jj].node2 == mesh_boundary[jk].node2) {
                                                            node1 = mesh_element[jj].node1;
                                                            node2 = mesh_element[jj].node3;

                                                            stress1x = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            stress1y = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            stress1xy = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;

                                                            stress2x = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            stress2y = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            stress2xy = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            found = 1;
                                                        } else if (mesh_element[jj].node3 == mesh_boundary[jk].node2) {
                                                            node1 = mesh_element[jj].node1;
                                                            node2 = mesh_element[jj].node2;

                                                            stress1x = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            stress1y = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            stress1xy = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;

                                                            stress2x = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            stress2y = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            stress2xy = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            found = 1;
                                                        }
                                                        if (found) {
                                                            //check if node1 and node2 are adjacent supported nodes, or inside (non-supported) neighboring nodes
                                                            if (node1 > 0) {
                                                                if (mesh_node[node1 - 1].restraint > 5) {
                                                                    int kk;
                                                                    for (kk = 0; kk < num_adj_supp; kk++)
                                                                        if (adj_list[kk] == node1) break;
                                                                    if (kk == num_adj_supp)  //not found
                                                                    {
                                                                        adj_list[num_adj_supp] = node1;
                                                                        num_adj_supp++;
                                                                    }
                                                                } else {
                                                                    int kk;
                                                                    for (kk = 0; kk < num_inside; kk++)
                                                                        if (inside_list[kk] == node1) break;
                                                                    if (kk == num_inside)  //not found
                                                                    {
                                                                        inside_list[num_inside] = node1;
                                                                        inside_list_stress_x[num_inside] = stress1x;
                                                                        inside_list_stress_y[num_inside] = stress1y;
                                                                        inside_list_stress_xy[num_inside] = stress1xy;
                                                                        num_inside++;
                                                                    }
                                                                }
                                                            }
                                                            if (node2 > 0) {
                                                                if (mesh_node[node2 - 1].restraint > 5) {
                                                                    int kk;
                                                                    for (kk = 0; kk < num_adj_supp; kk++)
                                                                        if (adj_list[kk] == node2) break;
                                                                    if (kk == num_adj_supp)  //not found
                                                                    {
                                                                        adj_list[num_adj_supp] = node2;
                                                                        num_adj_supp++;
                                                                    }
                                                                } else {
                                                                    int kk;
                                                                    for (kk = 0; kk < num_inside; kk++)
                                                                        if (inside_list[kk] == node2) break;
                                                                    if (kk == num_inside)  //not found
                                                                    {
                                                                        inside_list[num_inside] = node2;
                                                                        inside_list_stress_x[num_inside] = stress2x;
                                                                        inside_list_stress_y[num_inside] = stress2y;
                                                                        inside_list_stress_xy[num_inside] = stress2xy;
                                                                        num_inside++;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                    //calculating reactions for node2
                                                    double supp_x, supp_y;      // Supported node coordinates (m)

                                                    double adj_supp_x[12];         // Array: x-coords of adj supported nodes
                                                    double adj_supp_y[12];

                                                    double inside_x[12];           // Array: x-coords of inside nodes
                                                    double inside_y[12];

                                                    double H = 0.15, h[12] = {0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15,
                                                                              0.15,
                                                                              0.15, 0.15, 0.15}; //TEMPORARY

                                                    double Q_foo;

                                                    double Rz;  //reaction
                                                    double Rzmax, Rzmin;

                                                    supp_x = mesh_node[mesh_boundary[jk].node2 - 1].x;
                                                    supp_y = mesh_node[mesh_boundary[jk].node2 - 1].y;

                                                    for (jj = 0; jj < mesh_element_no; jj++) {
                                                        if (mesh_element[jj].node1 == mesh_boundary[jk].node2)
                                                        {
                                                            supp2_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            supp2_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            supp2_sigxy_max[jk] = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                            break;
                                                        }
                                                        else if (mesh_element[jj].node2 == mesh_boundary[jk].node2)
                                                        {
                                                            supp2_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            supp2_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            supp2_sigxy_max[jk] = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                            break;
                                                        }
                                                        else if (mesh_element[jj].node3 == mesh_boundary[jk].node2)
                                                        {
                                                            supp2_sigx_max[jk] = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            supp2_sigy_max[jk] = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            supp2_sigxy_max[jk] = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                            break;
                                                        }
                                                    }

                                                    for (jj = 0; jj < num_adj_supp; jj++)
                                                    {
                                                        adj_supp_x[jj] = mesh_node[adj_list[jj] - 1].x;
                                                        adj_supp_y[jj] = mesh_node[adj_list[jj] - 1].y;
                                                    }
                                                    for (jj = 0; jj < num_inside; jj++)
                                                    {
                                                        inside_x[jj] = mesh_node[inside_list[jj] - 1].x;
                                                        inside_y[jj] = mesh_node[inside_list[jj] - 1].y;

                                                        inside2_sigx_max[jk][jj] = inside_list_stress_x[jj]; //stress1x;  //just max
                                                        inside2_sigy_max[jk][jj] = inside_list_stress_y[jj]; //stress1y;  //just max
                                                        inside2_sigxy_max[jk][jj] = inside_list_stress_xy[jj]; //stress1xy;  //just max
                                                    }

                                                    int sti5 = 5;

                                                    //// REACTIONS
                                                    mesh_boundary[jk].state2[sti5].Rz1 = compute_support_reaction(H, supp_x,supp_y,supp2_sigx_max[jk],supp2_sigy_max[jk],supp2_sigxy_max[jk],num_adj_supp,adj_supp_x,adj_supp_y,num_inside,h,inside_x,inside_y,inside2_sigx_max[jk],inside2_sigy_max[jk],inside2_sigxy_max[jk],&mesh_boundary[jk].state2[sti5].Qn1);
#ifndef NDEBUG
                                                    //if (last_node != mesh_boundary[jk].node1)
                                                    //{
                                                    printf("ALTERNATIVE\n");
                                                    printf("node=%d\n", mesh_boundary[jk].node2);
                                                    printf("H=%f\n", H);
                                                    printf("supp.x=%f  supp.y=%f  supp2_sigx=%f  supp2_sigy=%f  supp2_sigxy=%f\n",
                                                           supp_x, supp_y, supp2_sigx_max[jk], supp2_sigy_max[jk], supp2_sigxy_max[jk]);
                                                    printf("num_adj_supp=%d\n", num_adj_supp);
                                                    for (int iii = 0; iii < num_adj_supp; iii++) {
                                                        printf("adj_supp_x[%d]=%f  adj_supp_y[%d]=%f\n",iii,adj_supp_x[iii],iii, adj_supp_y[iii]);
                                                    }
                                                    printf("num_inside=%d\n", num_inside);
                                                    for (int iii = 0; iii < num_inside; iii++) {
                                                        printf("inside_x[%d]=%f  inside_y[%d]=%f  inside2_sigx[%d]=%f  inside2_sigy[%d]=%f  inside2_sigxy[%d]=%f\n",iii,inside_x[iii],iii,inside_y[iii],iii,inside2_sigx_max[j][iii],iii,inside2_sigy_max[j][iii],iii,inside2_sigxy_max[j][iii]);
                                                    }

                                                    printf("Results: Qn1=%f Rz1=%f\n\n", mesh_boundary[jk].state2[sti].Qn1, mesh_boundary[jk].state2[sti].Rz1);
                                                    //}
                                                    //last_node=mesh_boundary[jk].node1;
#endif
                                                }

                                            }
                                        }

                                        /////////////////////////////////
                                        //REACTIONS for combination, se they are collected as state no=5 (countiny from 0)
                                        int last_node = 0;
                                        int sti5 = 5;

                                        //Rzmin = 0.0;
                                        Rzmax = 0.0;

                                        //Rzmin_back = 0.0;
                                        Rzmax_back = 0.0;
#ifndef NDEBUG
                                        printf("\nState %s-%d(%d)\n", SLS_ULS, ci + 1, ULS_SLS_LC_NO);
#endif
                                        for (i = 0; i < mesh_boundaries_no; i++) {
                                            if (mesh_boundary[i].restraint > 5) {
                                                if (mesh_boundary[i].node1 != last_node)
                                                {
#ifndef NDEBUG
                                                    printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti5].Rz1, mesh_boundary[i].state1[sti5].Qn1);
#endif
                                                    Rzmax += nofabs(mesh_boundary[i].state1[sti5].Rz1);
                                                }
                                                last_node = mesh_boundary[i].node1;
                                                if (mesh_boundary[i].node2 != last_node)
                                                {
#ifndef NDEBUG
                                                    printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti5].Rz1, mesh_boundary[i].state2[sti5].Qn1);
#endif
                                                    Rzmax += nofabs(mesh_boundary[i].state2[sti5].Rz1);
                                                }
                                                last_node = mesh_boundary[i].node2;
                                            }
                                        }
#ifndef NDEBUG
                                        printf("SUM:  Rz_total_i=%f\n", Rzmax);
#endif
                                        //Rzmin_back = Rzmin;
                                        Rzmax_back = Rzmax;
#ifndef NDEBUG
                                        printf("\nTotal load in state %s-%d(%d):  %f\n", SLS_ULS, ci + 1, ULS_SLS_LC_NO, total_load);
#endif
                                        load_ratio = total_load / Rzmax_back;

                                        //Rzmin = 0.0;
                                        Rzmax = 0.0;
#ifndef NDEBUG
                                        printf("\nState %s-%d(%d) with correction ratio %f\n", SLS_ULS, ci + 1, ULS_SLS_LC_NO, load_ratio);
#endif
                                        for (i = 0; i < mesh_boundaries_no; i++) {
                                            if (mesh_boundary[i].restraint > 5) {
                                                if (mesh_boundary[i].node1 != last_node)
                                                {
#ifndef NDEBUG
                                                    printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti5].Rz1 * load_ratio, mesh_boundary[i].state1[sti5].Qn1 * load_ratio);
#endif
                                                    Rzmax += (nofabs(mesh_boundary[i].state1[sti5].Rz1) * load_ratio);
                                                }
                                                last_node = mesh_boundary[i].node1;
                                                if (mesh_boundary[i].node2 != last_node)
                                                {
#ifndef NDEBUG
                                                    printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti5].Rz1 * load_ratio, mesh_boundary[i].state2[sti5].Qn1 * load_ratio);
#endif
                                                    Rzmax += nofabs(mesh_boundary[i].state2[sti5].Rz1) * load_ratio;
                                                }
                                                last_node = mesh_boundary[i].node2;
                                            }
                                        }
#ifndef NDEBUG
                                        printf("SUM:  Rz_total_i=%f\n", Rzmax);
#endif
                                        //upgradeing state min max
                                        for (i = 0; i < mesh_boundaries_no; i++) {
                                            if (ci == 0)  //first of combination
                                            {
                                                mesh_boundary[i].state1[sti].Qnmin1 = mesh_boundary[i].state1[sti5].Qn1 * load_ratio;
                                                mesh_boundary[i].state1[sti].Qnmax1 = mesh_boundary[i].state1[sti5].Qn1 * load_ratio;
                                                mesh_boundary[i].state1[sti].Rzmin1 = mesh_boundary[i].state1[sti5].Rz1 * load_ratio;
                                                mesh_boundary[i].state1[sti].Rzmax1 = mesh_boundary[i].state1[sti5].Rz1 * load_ratio;

                                                mesh_boundary[i].state2[sti].Qnmin1 = mesh_boundary[i].state2[sti5].Qn1 * load_ratio;
                                                mesh_boundary[i].state2[sti].Qnmax1 = mesh_boundary[i].state2[sti5].Qn1 * load_ratio;
                                                mesh_boundary[i].state2[sti].Rzmin1 = mesh_boundary[i].state2[sti5].Rz1 * load_ratio;
                                                mesh_boundary[i].state2[sti].Rzmax1 = mesh_boundary[i].state2[sti5].Rz1 * load_ratio;
                                            } else {
                                                mesh_boundary[i].state1[sti].Qnmin1 = min(mesh_boundary[i].state1[sti].Qnmin1, mesh_boundary[i].state1[sti5].Qn1 * load_ratio);
                                                mesh_boundary[i].state1[sti].Qnmax1 = max(mesh_boundary[i].state1[sti].Qnmax1, mesh_boundary[i].state1[sti5].Qn1 * load_ratio);
                                                mesh_boundary[i].state1[sti].Rzmin1 = min(mesh_boundary[i].state1[sti].Rzmin1, mesh_boundary[i].state1[sti5].Rz1 * load_ratio);
                                                mesh_boundary[i].state1[sti].Rzmax1 = max(mesh_boundary[i].state1[sti].Rzmax1, mesh_boundary[i].state1[sti5].Rz1 * load_ratio);

                                                mesh_boundary[i].state2[sti].Qnmin1 = min(mesh_boundary[i].state2[sti].Qnmin1, mesh_boundary[i].state2[sti5].Qn1 * load_ratio);
                                                mesh_boundary[i].state2[sti].Qnmax1 = max(mesh_boundary[i].state2[sti].Qnmax1, mesh_boundary[i].state2[sti5].Qn1 * load_ratio);
                                                mesh_boundary[i].state2[sti].Rzmin1 = min(mesh_boundary[i].state2[sti].Rzmin1, mesh_boundary[i].state2[sti5].Rz1 * load_ratio);
                                                mesh_boundary[i].state2[sti].Rzmax1 = max(mesh_boundary[i].state2[sti].Rzmax1, mesh_boundary[i].state2[sti5].Rz1 * load_ratio);
                                            }
                                        }
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
                                    perm_e = (int *) malloc(perm_e_node_no * sizeof(int) + 100);
                                    for (i = 0; i < perm_d_node_no; i++) perm_e[i] = -1;
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
                                epsilon[i] = (double *) malloc(perm_e_index_no * sizeof(double) + 100);
                                epsilon_min[i] = BIGNOF;
                                epsilon_max[i] = -BIGNOF;
                                if (!epsilon_ini) {
                                    min_epsilon_min[i] = BIGNOF;
                                    max_epsilon_max[i] = -BIGNOF;
                                    min_epsilon[i] = (double *) malloc(perm_e_index_no * sizeof(double) + 100);
                                    max_epsilon[i] = (double *) malloc(perm_e_index_no * sizeof(double) + 100);

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
                                                perm_e = (int *) malloc(perm_e_node_no * sizeof(int) + 100);
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
            sprintf(params, "%splate_sls.result", _plate_);
            strncpy(SLS_ULS, "sls", 3);  //temporary
            break;
        case 1: //uls
            sprintf(params, "%splate_uls.result", _plate_);
            strncpy(SLS_ULS, "ULS", 3);  //temporary
            break;
    }
    //results
    f = fopen(params, "rt");

    if (f == NULL)
    {

        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_OPEN_RESULTS_FILE_,
                           12,
                           params, 11, 1, 62);
        no_error = FALSE;
        goto pl_error1;

    }

    //restoring total_load for a case
    total_load = 0.0;
    for (i = 0; i < zone_no; i++) {
        total_load += surface_load[sti][zone_property[i].k] * body_area[zone_property[i].k];
    }
    for (i = 0; i < plate_no; i++) {
        total_load += surface_load[sti][plate_property[i].k] * body_area[plate_property[i].k];
    }
    total_load += point_load[sti];

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
                perm_d = (int *) malloc(perm_d_node_no * sizeof(int) + 100);
                for (i = 0; i < perm_d_node_no; i++) perm_d[i] = -1;
                perm_d_b = 1;
                for (i = 0; i < perm_d_index_no; i++) {
                    fgets(data_row, MaxTextLen, f);
                    ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                    //perm_d[node_number-1]=index_number-1;
                    perm_d[node_number - 1] = i;
                }
                for (i = 0; i < DEFLECTION_NUMBER; i++) {
                    deflection[i] = (double *) malloc(perm_d_index_no * sizeof(double) + 100);
                    deflection_min[i] = BIGNOF;
                    deflection_max[i] = -BIGNOF;
                }
                //deflection 1...EPSILON_NUMBER
                for (j = 0; j < DEFLECTION_NUMBER; j++) {
                    for (i = 0; i < perm_d_index_no; i++) {
                        fgets(data_row, MaxTextLen, f);
                        ret = sscanf(data_row, "%lf", &deflection[j][i]);
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
                                    perm_d = (int *) malloc(perm_d_node_no * sizeof(int) + 100);
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
                    perm_s = (int *) malloc(perm_s_node_no * sizeof(int) + 100);
                    for (i = 0; i < perm_d_node_no; i++) perm_s[i] = -1;
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
                stress[i] = (double *) malloc(perm_s_index_no * sizeof(double) + 100);
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
                                perm_s = (int *) malloc(perm_s_node_no * sizeof(int) + 100);
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
                    perm_e = (int *) malloc(perm_e_node_no * sizeof(int) + 100);
                    for (i = 0; i < perm_d_node_no; i++) perm_e[i] = -1;
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
                epsilon[i] = (double *) malloc(perm_e_index_no * sizeof(double) + 100);
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
                                perm_e = (int *) malloc(perm_e_node_no * sizeof(int) + 100);
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
            goto pl_error1;
        }
        if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

            ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_,
                               12, (char *) "", 11, 1, 62);
            no_error = FALSE;
            goto pl_error1;
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
                    goto pl_error1;
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
                            if (perm_d[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, deflection_amax);
                            else gradient.c1 = getRGB(-min_deflection[i][perm_d[mesh_element[j].node1 - 1]], deflection_amax);
                            if (perm_d[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, deflection_amax);
                            else gradient.c2 = getRGB(-min_deflection[i][perm_d[mesh_element[j].node2 - 1]], deflection_amax);
                            if (perm_d[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, deflection_amax);
                            else gradient.c3 = getRGB(-min_deflection[i][perm_d[mesh_element[j].node3 - 1]], deflection_amax);
                            gradient.c4 = getRGB(0.0, deflection_amax);

                            fe_data.el_number = j + 1;
                            fe_data.flag = 0;
                            fe_data.f1 = (float) min_deflection[i][perm_d[mesh_element[j].node1 - 1]];
                            fe_data.f2 = (float) min_deflection[i][perm_d[mesh_element[j].node2 - 1]];
                            fe_data.f3 = (float) min_deflection[i][perm_d[mesh_element[j].node3 - 1]];
                            fe_data.f4 = 0.0f;
                        } else {
                            if (perm_d[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, deflection_amax);
                            else gradient.c1 = getRGB(-max_deflection[i][perm_d[mesh_element[j].node1 - 1]], deflection_amax);
                            if (perm_d[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, deflection_amax);
                            else gradient.c2 = getRGB(-max_deflection[i][perm_d[mesh_element[j].node2 - 1]], deflection_amax);
                            if (perm_d[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, deflection_amax);
                            else gradient.c3 = getRGB(-max_deflection[i][perm_d[mesh_element[j].node3 - 1]], deflection_amax);
                            gradient.c4 = getRGB(0.0, deflection_amax);

                            fe_data.el_number = j + 1;
                            fe_data.flag = 0;
                            fe_data.f1 = (float) max_deflection[i][perm_d[mesh_element[j].node1 - 1]];
                            fe_data.f2 = (float) max_deflection[i][perm_d[mesh_element[j].node2 - 1]];
                            fe_data.f3 = (float) max_deflection[i][perm_d[mesh_element[j].node3 - 1]];
                            fe_data.f4 = 0.0f;
                        }
                    } else {
                        if (perm_d[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, deflection_amax);
                        else gradient.c1 = getRGB(-deflection[i][perm_d[mesh_element[j].node1 - 1]], deflection_amax);
                        if (perm_d[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, deflection_amax);
                        else gradient.c2 = getRGB(-deflection[i][perm_d[mesh_element[j].node2 - 1]], deflection_amax);
                        if (perm_d[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, deflection_amax);
                        else gradient.c3 = getRGB(-deflection[i][perm_d[mesh_element[j].node3 - 1]], deflection_amax);
                        gradient.c4 = getRGB(0.0, deflection_amax);

                        fe_data.el_number = j + 1;
                        fe_data.flag = 0;
                        fe_data.f1 = (float) deflection[i][perm_d[mesh_element[j].node1 - 1]];
                        fe_data.f2 = (float) deflection[i][perm_d[mesh_element[j].node2 - 1]];
                        fe_data.f3 = (float) deflection[i][perm_d[mesh_element[j].node3 - 1]];
                        fe_data.f4 = 0.0f;
                    }

                    w.lp = 6;
                    w.gradient = 1;

                    w.translucent = 1;
                    translucency_ptr = (char *) w.xy;
                    translucency_ptr += (w.lp * sizeof(float));
                    memmove(translucency_ptr, &HalfTranslucency, sizeof(unsigned char));

                    gradient_ptr = translucency_ptr + sizeof(unsigned char);
                    memmove(gradient_ptr, &gradient, sizeof(GRADIENT));

                    fe_data_ptr = gradient_ptr + sizeof(GRADIENT);
                    memmove(fe_data_ptr, &fe_data, sizeof(FE_DATA));

                    w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT) + sizeof(FE_DATA);
                    adr = dodaj_obiekt((BLOK *) dane, &w);
                }

                //colorbar
                if (deflection_amax > 0.0) {
                    w.xy[0] = (float) pl_min_x + 10.f;
                    w.xy[1] = (float) pl_min_y - 10.f;

                    w.xy[2] = w.xy[0] + 50.f;
                    w.xy[3] = w.xy[1];

                    w.xy[4] = w.xy[2];
                    w.xy[5] = w.xy[3] - 5.f;

                    w.xy[6] = w.xy[0];
                    w.xy[7] = w.xy[5];

                    if (nom_max) {
                        if (bi == 0) {
                            gradient.c1 = getRGB(-min_deflection_min[i], deflection_amax);
                            gradient.c2 = getRGB(-max_deflection_max[i], deflection_amax);
                            gradient.c3 = getRGB(-max_deflection_max[i], deflection_amax);
                            gradient.c4 = getRGB(-min_deflection_min[i], deflection_amax);

                            fe_data.el_number = 0;
                            fe_data.flag = 0;
                            fe_data.f1 = (float) -min_deflection_min[i];
                            fe_data.f2 = (float) -max_deflection_max[i];
                            fe_data.f3 = (float) -max_deflection_max[i];
                            fe_data.f4 = (float) -min_deflection_min[i];
                        } else {
                            gradient.c1 = getRGB(-min_deflection_min[i], deflection_amax);
                            gradient.c2 = getRGB(-max_deflection_max[i], deflection_amax);
                            gradient.c3 = getRGB(-max_deflection_max[i], deflection_amax);
                            gradient.c4 = getRGB(-min_deflection_min[i], deflection_amax);

                            fe_data.el_number = 0;
                            fe_data.flag = 0;
                            fe_data.f1 = (float) -min_deflection_min[i];
                            fe_data.f2 = (float) -max_deflection_max[i];
                            fe_data.f3 = (float) -max_deflection_max[i];
                            fe_data.f4 = (float) -min_deflection_min[i];
                        }
                    } else {
                        gradient.c1 = getRGB(-deflection_min[i], deflection_amax);
                        gradient.c2 = getRGB(-deflection_max[i], deflection_amax);
                        gradient.c3 = getRGB(-deflection_max[i], deflection_amax);
                        gradient.c4 = getRGB(-deflection_min[i], deflection_amax);

                        fe_data.el_number = 0;
                        fe_data.flag = 0;
                        fe_data.f1 = (float) -deflection_min[i];
                        fe_data.f2 = (float) -deflection_max[i];
                        fe_data.f3 = (float) -deflection_max[i];
                        fe_data.f4 = (float) -deflection_min[i];
                    }

                    w.lp = 8;
                    w.gradient = 1;
                    w.translucent = 1;
                    translucency_ptr = (char *) w.xy;
                    translucency_ptr += (w.lp * sizeof(float));
                    memmove(translucency_ptr, &HalfTranslucency, sizeof(unsigned char));

                    gradient_ptr = translucency_ptr + sizeof(unsigned char);
                    memmove(gradient_ptr, &gradient, sizeof(GRADIENT));

                    fe_data_ptr = gradient_ptr + sizeof(GRADIENT);
                    memmove(fe_data_ptr, &fe_data, sizeof(FE_DATA));

                    w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT) + sizeof(FE_DATA);
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
                    adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                    T.x = w.xy[2];
                    T.y = w.xy[3] + 0.5f;
                    T.justowanie = j_do_prawej;
                    sprintf(T.text, "%.4f", deflection_max_);

                    T.dl = strlen(T.text);
                    T.n = T18 + T.dl;
                    normalize_txt(&T);
                    adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                    T.x = (w.xy[0] + w.xy[2]) / 2.f;
                    T.y = w.xy[3] + 0.5f;
                    T.justowanie = j_srodkowo;
                    sprintf(T.text, "%s  %s%s %s", Utf8StrMakeUprUtf8Str(SLS_ULS), deflection_chapter[i], min_max, unit);

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
            goto pl_error1;
        }
        if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

            ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_,
                               12, (char *) "", 11, 1, 62);
            no_error = FALSE;
            goto pl_error1;
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
                stress_min_RC_rho = BIGNOD;
                stress_max_RC_rho = -BIGNOD;
                RC_flag_exists = FALSE;

                if (stress_amax > 0.0) {
                    ptr_block = add_block(blok_origin.x, blok_origin.y, B_GRAPH, block_name, hiding);
                    if (ptr_block == NULL) {
                        fclose(f);
                        ret = ask_question(1, (char *) "", (char *) confirm, (char *) "",
                                           (char *) _CANNOT_CREATE_STRESS_BLOCK_, 12, (char *) "", 11, 1, 62);
                        no_error = FALSE;
                        goto pl_error1;
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
                                    body_prop = &pl_property[body_prop_no];
                                else body_prop = NULL;

                                if (body_prop != NULL) //WARNING, RC_flag should be inherited from plate
                                {
                                    if (body_prop->RC_flag) {
                                        //calculating reinforcement ratio and normalized compression stress
                                        RC_flag = TRUE; //so there will be just one block for both min and max
                                    }
                                }
                            }
                            if ((RC_flag == TRUE) && (i < 3))  //only σ.xx, σ.yy, σ.zz
                            {
                                RC_flag_exists = TRUE;
                                //changing kolor
                                w.kolor = 255;  //this is indicator of showing percent for negatives, and stress for positives
                                //double vmax_node[3];
                                //double vmin_node[3];
                                double v_node[3];
                                double d;  //effective depth
                                double M;  //moment of force
                                double M_;  //absolute value moment of force
                                double K;  //non-dimentional moment
                                double z;  //lever arm
                                double As, Asc;    //surface of reinforcing bars in tension, reinforcing bars in compression
                                double p[3];  //reinforcement ratio in each node
                                double sigma[3];  //effective stress in concrete in each node
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

                                if (bi == 0) {
                                    //vmin_node[0] = min_stress[i][perm_s[mesh_element[j].node1 - 1]];
                                    //vmin_node[1] = min_stress[i][perm_s[mesh_element[j].node2 - 1]];
                                    //vmin_node[2] = min_stress[i][perm_s[mesh_element[j].node3 - 1]];
                                    v_node[0] = min_stress[i][perm_s[mesh_element[j].node1 - 1]];
                                    v_node[1] = min_stress[i][perm_s[mesh_element[j].node2 - 1]];
                                    v_node[2] = min_stress[i][perm_s[mesh_element[j].node3 - 1]];
                                } else {
                                    //vmax_node[0] = max_stress[i][perm_s[mesh_element[j].node1 - 1]];
                                    //vmax_node[1] = max_stress[i][perm_s[mesh_element[j].node2 - 1]];
                                    //vmax_node[2] = max_stress[i][perm_s[mesh_element[j].node3 - 1]];
                                    v_node[0] = max_stress[i][perm_s[mesh_element[j].node1 - 1]];
                                    v_node[1] = max_stress[i][perm_s[mesh_element[j].node2 - 1]];
                                    v_node[2] = max_stress[i][perm_s[mesh_element[j].node3 - 1]];
                                }

                                if (ret_standard == 1) //Eurocodes
                                {
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

                                    for (int ni = 0; ni < 3; ni++) {
                                        //reinforcement ratio
                                        //M=v_node[ni]*h*h/6.;

                                        M = v_node[ni] * body_prop->h * body_prop->h / 6.;

                                        if (UNITS != SI)  //need to be converted
                                        {
                                            M *= 0.112985 * 1000.0 / 25.4;   //lbs-inch to Nm
                                        }
                                        M_ = fabs(M);

                                        if (Check_if_Equal(M, 0.0)) {
                                            sigma[ni] = 0.;
                                            p[ni] = 0.;
                                            continue;
                                        }

                                        double lambda = 0.8;  //λ
                                        double eta = 1.0;  //η
                                        double rhot, rhoc, total_rho, sigma_c_max;
                                        int use_min_reinf = 1;

                                        int ret_p = calculate_rebars_eurocode_simplified(M_, h, 1.0, cc, fck, fcd, fyd, eta, lambda, E_s, use_min_reinf, &As, &Asc, &rhot, &rhoc, &total_rho, &sigma_c_max);


                                        if (ret_p == 0) {
                                            p[ni] = total_rho;
                                            sigma[ni] = sigma_c_max;
                                        } else //Negative reinforcement area
                                        {
                                            p[ni] = 0.0;  //or NaN
                                            sigma[ni] = 0.0;  //or NaN
                                        }

                                        if (UNITS != SI)  //need to be converted
                                        {
                                            sigma[ni] *= 0.000145038;  //Pa to psi
                                        }

                                        stress_min_RC = min(stress_min_RC, sigma[ni]);
                                        stress_max_RC = max(stress_max_RC, sigma[ni]);

                                        ps = p[ni] * (signbit(M) ? -1 : 1);
                                        stress_min_RC_rho = min(stress_min_RC_rho, ps);
                                        stress_max_RC_rho = max(stress_max_RC_rho, ps);
                                    }
                                } else // ASCE & ICC
                                {
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

                                        if (fabs(v_node[ni]) > 11000) {
                                            int abc = 0;
                                        }
                                        M = v_node[ni] * h * h / 6.;

                                        if (UNITS != SI) {
                                            //M *= 1000.0/304.8;   //Nm/m to in-lbs/ft
                                            M *= 12.;
                                        }

                                        M_ = fabs(M);
                                        if (Check_if_Equal(M, 0.0)) {
                                            sigma[ni] = 0.;
                                            p[ni] = 0.;
                                            continue;
                                        }

                                        double rhot, rhoc, total_rho, sigma_c_max;
                                        int use_min_reinf = 1;

                                        int ret_p = calculate_rebars_aci_simplified_UNI(M_, h, bb, cc, fyk, fyd, fck, fcd, use_min_reinf, &As, &Asc, &rhot, &rhoc, &total_rho, &sigma_c_max);
                                        //int ret_p = calculate_rebars_aci_simplified_SI(M_, h, cc, fyk, fyd, fck, fcd, &As, &Asc, &rhot, &rhoc,&total_rho, &sigma_c_max);

                                        if (ret_p == 0) {
                                            p[ni] = total_rho;
                                            sigma[ni] = sigma_c_max;
                                        } else {
                                            p[ni] = 0.0;   //or NaN
                                            sigma[ni] = 0.0;  //or Nan
                                        }

                                        stress_min_RC = min(stress_min_RC, sigma[ni]);
                                        stress_max_RC = max(stress_max_RC, sigma[ni]);
                                        ps = p[ni] * (signbit(M) ? -1 : 1);
                                        stress_min_RC_rho = min(stress_min_RC_rho, ps);
                                        stress_max_RC_rho = max(stress_max_RC_rho, ps);
                                    }
                                }

                                if (bi == 0) {
                                    if (perm_d[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, stress_amax);
                                    else gradient.c1 = getRGB(min_stress[i][perm_s[mesh_element[j].node1 - 1]], stress_amax);
                                    if (perm_d[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, stress_amax);
                                    else gradient.c2 = getRGB(min_stress[i][perm_s[mesh_element[j].node2 - 1]], stress_amax);
                                    if (perm_d[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, stress_amax);
                                    else gradient.c3 = getRGB(min_stress[i][perm_s[mesh_element[j].node3 - 1]], stress_amax);
                                    gradient.c4 = getRGB(0.0, stress_amax);

                                    fe_data.el_number = j + 1;
                                    fe_data.flag = 1;

                                    fe_data.f1 = (float) sigma[0];
                                    fe_data.f2 = (float) sigma[1];
                                    fe_data.f3 = (float) sigma[2];
                                    fe_data.f4 = 0.0f;

                                    fe_data_ex.f1 = (float) p[0];  //negative means %
                                    fe_data_ex.f2 = (float) p[1];  //negative means %
                                    fe_data_ex.f3 = (float) p[2];  //negative means %
                                    fe_data_ex.f4 = 0.0f;
                                } else {
                                    if (perm_d[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, stress_amax);
                                    else gradient.c1 = getRGB(max_stress[i][perm_s[mesh_element[j].node1 - 1]], stress_amax);
                                    if (perm_d[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, stress_amax);
                                    else gradient.c2 = getRGB(max_stress[i][perm_s[mesh_element[j].node2 - 1]], stress_amax);
                                    if (perm_d[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, stress_amax);
                                    else gradient.c3 = getRGB(max_stress[i][perm_s[mesh_element[j].node3 - 1]], stress_amax);
                                    gradient.c4 = getRGB(0.0, stress_amax);

                                    fe_data.el_number = j + 1;
                                    fe_data.flag = 1;

                                    fe_data.f1 = (float) sigma[0];
                                    fe_data.f2 = (float) sigma[1];
                                    fe_data.f3 = (float) sigma[2];
                                    fe_data.f4 = 0.0f;

                                    fe_data_ex.f1 = (float) p[0];  //negative means %
                                    fe_data_ex.f2 = (float) p[1];  //negative means %
                                    fe_data_ex.f3 = (float) p[2];  //negative means %
                                    fe_data_ex.f4 = 0.0f;

                                }
                            } else if (bi == 0) {
                                if (perm_d[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, stress_amax);
                                else gradient.c1 = getRGB(min_stress[i][perm_s[mesh_element[j].node1 - 1]], stress_amax);
                                if (perm_d[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, stress_amax);
                                else gradient.c2 = getRGB(min_stress[i][perm_s[mesh_element[j].node2 - 1]], stress_amax);
                                if (perm_d[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, stress_amax);
                                else gradient.c3 = getRGB(min_stress[i][perm_s[mesh_element[j].node3 - 1]], stress_amax);
                                gradient.c4 = getRGB(0.0, stress_amax);

                                fe_data.el_number = j + 1;
                                fe_data.flag = 0;
                                fe_data.f1 = (float) min_stress[i][perm_s[mesh_element[j].node1 - 1]];
                                fe_data.f2 = (float) min_stress[i][perm_s[mesh_element[j].node2 - 1]];
                                fe_data.f3 = (float) min_stress[i][perm_s[mesh_element[j].node3 - 1]];
                                fe_data.f4 = 0.0f;
                            } else if (bi == 1) {
                                if (perm_d[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, stress_amax);
                                else gradient.c1 = getRGB(max_stress[i][perm_s[mesh_element[j].node1 - 1]], stress_amax);
                                if (perm_d[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, stress_amax);
                                else gradient.c2 = getRGB(max_stress[i][perm_s[mesh_element[j].node2 - 1]], stress_amax);
                                if (perm_d[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, stress_amax);
                                else gradient.c3 = getRGB(max_stress[i][perm_s[mesh_element[j].node3 - 1]], stress_amax);
                                gradient.c4 = getRGB(0.0, stress_amax);

                                fe_data.el_number = j + 1;
                                fe_data.flag = 0;
                                fe_data.f1 = (float) max_stress[i][perm_s[mesh_element[j].node1 - 1]];
                                fe_data.f2 = (float) max_stress[i][perm_s[mesh_element[j].node2 - 1]];
                                fe_data.f3 = (float) max_stress[i][perm_s[mesh_element[j].node3 - 1]];
                                fe_data.f4 = 0.0f;
                            }
                        } else {
                            if (perm_s[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, stress_amax);
                            else gradient.c1 = getRGB(stress[i][perm_s[mesh_element[j].node1 - 1]], stress_amax);
                            if (perm_s[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, stress_amax);
                            else gradient.c2 = getRGB(stress[i][perm_s[mesh_element[j].node2 - 1]], stress_amax);
                            if (perm_s[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, stress_amax);
                            else gradient.c3 = getRGB(stress[i][perm_s[mesh_element[j].node3 - 1]], stress_amax);
                            gradient.c4 = getRGB(0.0, stress_amax);

                            fe_data.el_number = j + 1;
                            fe_data.flag = 0;
                            fe_data.f1 = (float) stress[i][perm_s[mesh_element[j].node1 - 1]];
                            fe_data.f2 = (float) stress[i][perm_s[mesh_element[j].node2 - 1]];
                            fe_data.f3 = (float) stress[i][perm_s[mesh_element[j].node3 - 1]];
                            fe_data.f4 = 0.0f;
                        }

                        w.lp = 6;
                        w.gradient = 1;

                        w.translucent = 1;
                        translucency_ptr = (char *) w.xy;
                        translucency_ptr += (w.lp * sizeof(float));
                        memmove(translucency_ptr, &HalfTranslucency, sizeof(unsigned char));

                        gradient_ptr = translucency_ptr + sizeof(unsigned char);
                        memmove(gradient_ptr, &gradient, sizeof(GRADIENT));

                        fe_data_ptr = gradient_ptr + sizeof(GRADIENT);
                        memmove(fe_data_ptr, &fe_data, sizeof(FE_DATA));

                        if (fe_data.flag == 1)  //extended
                        {
                            fe_data_ex_ptr = fe_data_ptr + sizeof(FE_DATA);
                            memmove(fe_data_ex_ptr, &fe_data_ex, sizeof(FE_DATA_EX));
                            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT) + sizeof(FE_DATA) + sizeof(FE_DATA_EX);
                        } else {
                            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT) + sizeof(FE_DATA);
                        }
                        adr = dodaj_obiekt((BLOK *) dane, &w);
                    }

                    //colorbar
                    if (stress_amax > 0.0) {
                        w.xy[0] = (float) pl_min_x + 10.f;
                        w.xy[1] = (float) pl_min_y - 10.f;

                        w.xy[2] = w.xy[0] + 50.f;
                        w.xy[3] = w.xy[1];

                        w.xy[4] = w.xy[2];
                        w.xy[5] = w.xy[3] - 5.f;

                        w.xy[6] = w.xy[0];
                        w.xy[7] = w.xy[5];

                        if (nom_max) {
                            if (bi == 0) {
                                gradient.c1 = getRGB(min_stress_min[i], stress_amax);
                                gradient.c2 = getRGB(max_stress_max[i], stress_amax);
                                gradient.c3 = getRGB(max_stress_max[i], stress_amax);
                                gradient.c4 = getRGB(min_stress_min[i], stress_amax);

                                fe_data.el_number = 0;
                                fe_data.flag = 0;
                                fe_data.f1 = (float) min_stress_min[i];
                                fe_data.f2 = (float) max_stress_max[i];
                                fe_data.f3 = (float) max_stress_max[i];
                                fe_data.f4 = (float) min_stress_min[i];
                            } else {
                                gradient.c1 = getRGB(min_stress_min[i], stress_amax);
                                gradient.c2 = getRGB(max_stress_max[i], stress_amax);
                                gradient.c3 = getRGB(max_stress_max[i], stress_amax);
                                gradient.c4 = getRGB(min_stress_min[i], stress_amax);

                                fe_data.el_number = 0;
                                fe_data.flag = 0;
                                fe_data.f1 = (float) min_stress_min[i];
                                fe_data.f2 = (float) max_stress_max[i];
                                fe_data.f3 = (float) max_stress_max[i];
                                fe_data.f4 = (float) min_stress_min[i];
                            }
                        } else {
                            gradient.c1 = getRGB(stress_min[i], stress_amax);
                            gradient.c2 = getRGB(stress_max[i], stress_amax);
                            gradient.c3 = getRGB(stress_max[i], stress_amax);
                            gradient.c4 = getRGB(stress_min[i], stress_amax);

                            fe_data.el_number = 0;
                            fe_data.flag = 0;
                            fe_data.f1 = (float) stress_min[i];
                            fe_data.f2 = (float) stress_max[i];
                            fe_data.f3 = (float) stress_max[i];
                            fe_data.f4 = (float) stress_min[i];
                        }

                        w.lp = 8;
                        w.gradient = 1;
                        w.translucent = 1;
                        translucency_ptr = (char *) w.xy;
                        translucency_ptr += (w.lp * sizeof(float));
                        memmove(translucency_ptr, &HalfTranslucency, sizeof(unsigned char));

                        gradient_ptr = translucency_ptr + sizeof(unsigned char);
                        memmove(gradient_ptr, &gradient, sizeof(GRADIENT));

                        fe_data_ptr = gradient_ptr + sizeof(GRADIENT);
                        memmove(fe_data_ptr, &fe_data, sizeof(FE_DATA));

                        w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT) + sizeof(FE_DATA);
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

                        if (RC_flag_exists) {
                            T.x = w.xy[0];
                            T.y = w.xy[5] - T.wysokosc - 0.5f;
                            T.justowanie = j_do_lewej;
                            sprintf(T.text, "%.2f", fabs(stress_min_RC_rho) * 100.);

                            T.dl = strlen(T.text);
                            T.n = T18 + T.dl;
                            normalize_txt(&T);
                            adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                            T.x = w.xy[2];
                            T.y = w.xy[5] - T.wysokosc - 0.5f;
                            T.justowanie = j_do_prawej;
                            sprintf(T.text, "%.2f", fabs(stress_max_RC_rho) * 100.);

                            T.dl = strlen(T.text);
                            T.n = T18 + T.dl;
                            normalize_txt(&T);
                            adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                            T.x = (w.xy[0] + w.xy[2]) / 2.f;
                            T.y = w.xy[5] + -T.wysokosc - 0.5f;
                            T.justowanie = j_srodkowo;
                            sprintf(T.text, "%s", "ρ [%]");

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
                        double H = 0.15;
                        double h[12] = {0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15,
                                        0.15,
                                        0.15, 0.15, 0.15}; //TEMPORARY
#ifdef REACTIONSNSEW
                        if ((stress[i] != NULL) && (i == 1)) //stress_yy
                        {
                            for (j = 0; j < mesh_boundaries_no; j++) {
                                if (mesh_boundary[j].restraint > 5)  //6 simple supported, 7 fixed
                                {
                                    if (mesh_boundary[j].node1 > 0)  //node exists
                                    {
                                        int adj_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                        int num_adj_supp = 0;
                                        int inside_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                        int num_inside = 0;
                                        int num_inside_NSEW = 4;
                                        int node1 = 0, node2 = 0;
                                        int jj;
                                        double stress1, stress2;

                                        if (mesh_boundary[j].node1 == 24)
                                        {
                                            int a=0;
                                        }

                                        //searching for all elements containing node1 for reaction procedure
                                        for (jj = 0; jj < mesh_element_no; jj++)
                                        {
                                            int found = 0;
                                            if (mesh_element[jj].node1 == mesh_boundary[j].node1)
                                            {
                                                node1 = mesh_element[jj].node2;
                                                node2 = mesh_element[jj].node3;
                                                inside_list[num_inside] = jj;
                                                num_inside++;
                                                found = 1;
                                            }
                                            else if  (mesh_element[jj].node2 == mesh_boundary[j].node1)
                                            {
                                                node1 = mesh_element[jj].node1;
                                                node2 = mesh_element[jj].node3;
                                                inside_list[num_inside] = jj;
                                                num_inside++;
                                                found = 1;
                                            }
                                            else if (mesh_element[jj].node3 == mesh_boundary[j].node1)
                                            {
                                                node1 = mesh_element[jj].node1;
                                                node2 = mesh_element[jj].node2;
                                                inside_list[num_inside] = jj;
                                                num_inside++;
                                                found = 1;
                                            }
                                            if (found)
                                            {
                                                //check if node1 and node2 are adjacent supported nodes, or inside (non-supported) neighboring nodes
                                                if (node1 > 0)
                                                {
                                                    if (mesh_node[node1 - 1].restraint > 5)
                                                    {
                                                        int kk;
                                                        for (kk = 0; kk < num_adj_supp; kk++)
                                                            if (adj_list[kk] == node1) break;
                                                        if (kk == num_adj_supp)  //not found
                                                        {
                                                            adj_list[num_adj_supp] = node1;
                                                            num_adj_supp++;
                                                        }
                                                    }
                                                }
                                                if (node2 > 0)
                                                {
                                                    if (mesh_node[node2 - 1].restraint > 5)
                                                    {
                                                        int kk;
                                                        for (kk = 0; kk < num_adj_supp; kk++)
                                                            if (adj_list[kk] == node2) break;
                                                        if (kk == num_adj_supp)  //not found
                                                        {
                                                            adj_list[num_adj_supp] = node2;
                                                            num_adj_supp++;
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        //calculating stresses for node1 in reaction procedure
                                        double supp_x, supp_y;      // Supported node coordinates (m)
                                        //double supp_sigx_min=0.0, supp_sigy_min=0.0;
                                        //double supp_sigx_max=0.0, supp_sigy_max=0.0;

                                        double adj_supp_x[12];         // Array: x-coords of adj supported nodes
                                        double adj_supp_y[12];

                                        double inside_x[12];           // Array: x-coords of inside nodes
                                        double inside_y[12];

                                        //double H = 0.15, h[12] = {0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15,
                                        //                          0.15,
                                        //                          0.15, 0.15, 0.15}; //TEMPORARY

                                        //double Q_vertical_min;             // Output: shear per unit length Q_x (N/m) for vertical edge (x=const); 0 if no vertical edge
                                        //double Q_vertical_max;
                                        //double Q_horizontal_min;
                                        //double Q_horizontal_max;
                                        double Q_foo;

                                        double Rz;  //reaction
                                        double Rzmax, Rzmin;
                                        BOOL is_simple;

                                        supp_x = mesh_node[mesh_boundary[j].node1 - 1].x;
                                        supp_y = mesh_node[mesh_boundary[j].node1 - 1].y;

                                        for (jj = 0; jj < mesh_element_no; jj++) {
                                            if (mesh_element[jj].node1 == mesh_boundary[j].node1)
                                            {
                                                supp1_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                supp1_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                break;
                                            } else if (mesh_element[jj].node2 == mesh_boundary[j].node1)
                                            {
                                                supp1_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                supp1_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                break;
                                            } else if (mesh_element[jj].node3 == mesh_boundary[j].node1)
                                            {
                                                supp1_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                supp1_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                break;
                                            }
                                        }

                                        for (jj = 0; jj < num_adj_supp; jj++) {
                                            adj_supp_x[jj] = mesh_node[adj_list[jj] - 1].x;
                                            adj_supp_y[jj] = mesh_node[adj_list[jj] - 1].y;
                                        }

                                        //calculating stresses for node1 in reaction procedure
                                        //we have :
                                        // inside_list[num_inside] = jj;
                                        // num_inside++;
                                        //mesh_boundary point 1 coordinates
                                        // mesh_boundary[jk].x1;
                                        // mesh_boundary[jk].y1;
                                        // we create virtual points where NSEW stresses will be calculated from mesh elements stress interpolation
                                        // first we try with distance NSEW_dist equal half of the distance between current boundary point and the next one
                                        // or if this is the last point in boundary series - distance between previous boundary point and the current one
                                        double NSEW_dist;
                                        POINTF pa, pb, pc, pp;
                                        double alpha, beta, gamma;
                                        //double s_value;

                                        if (mesh_boundary[j].node2 > 0)  //next node exists
                                        {
                                            NSEW_dist = sqrt((mesh_boundary[j].x2 - mesh_boundary[j].x1) * (mesh_boundary[j].x2 - mesh_boundary[j].x1) +
                                                             (mesh_boundary[j].y2 - mesh_boundary[j].y1) * (mesh_boundary[j].y2 - mesh_boundary[j].y1)) / NSEW_dist_div;
                                        } else if ((j > 0) && (mesh_boundary[j - 1].node1 > 0))  //previous node exists
                                        {
                                            NSEW_dist = sqrt((mesh_boundary[j].x1 - mesh_boundary[j - 1].x1) * (mesh_boundary[j].x1 - mesh_boundary[j - 1].x1) +
                                                             (mesh_boundary[j].y1 - mesh_boundary[j - 1].y1) * (mesh_boundary[j].y1 - mesh_boundary[j - 1].y1)) / NSEW_dist_div;
                                        } else  //in case there is single point
                                        {
                                            if (UNITS == SI)
                                                NSEW_dist = 0.1;  //10 cm
                                            else
                                                NSEW_dist = 4.;   //4 inches
                                        }
                                        //searching for element which contains point NORTH, SOUTH, EAST and WETS
                                        int y_offset_i[4] = {1, -1, 0, 0};  //1 EAST, -1 WETS 0 NONE
                                        int x_offset_i[4] = {0, 0, 1, -1};  //0 NONE, 1 NORTH, -1 SOUTH
                                        int stress_selector[4] = {1, 1, 0, 0};  //1 stress_yy, 0 stress_xx

                                        for (int kk=0; kk<4; kk++) {
                                            inside1_sigx_max[j][kk] = 0.;
                                            inside1_sigy_max[j][kk] = 0.;
                                        }

                                        num_inside_NSEW = 4;
                                        int b_inside[4] = {0, 0, 0, 0};

                                        for (int kk = 0; kk < 4; kk++)   //N, S, E, W
                                        {
                                            pp.x = (float) (mesh_boundary[j].x1 + NSEW_dist * x_offset_i[kk]);
                                            pp.y = (float) (mesh_boundary[j].y1 + NSEW_dist * y_offset_i[kk]);

                                            for (jj = 0; jj < num_inside; jj++)
                                            {
                                                pa.x = (float) mesh_node[mesh_element[inside_list[jj]].node1 - 1].x;
                                                pa.y = (float) mesh_node[mesh_element[inside_list[jj]].node1 - 1].y;

                                                pb.x = (float) mesh_node[mesh_element[inside_list[jj]].node2 - 1].x;
                                                pb.y = (float) mesh_node[mesh_element[inside_list[jj]].node2 - 1].y;

                                                pc.x = (float) mesh_node[mesh_element[inside_list[jj]].node3 - 1].x;
                                                pc.y = (float) mesh_node[mesh_element[inside_list[jj]].node3 - 1].y;

                                                //if (is_point_in_triangle(pa, pb, pc, pp))
                                                if (isInside(&pa, &pb, &pc, &pp, &alpha, &beta, &gamma))
                                                {
                                                    inside_x[kk] = (mesh_boundary[j].x1 + NSEW_dist * x_offset_i[kk]);
                                                    inside_y[kk] = (mesh_boundary[j].y1 + NSEW_dist * y_offset_i[kk]);
                                                    b_inside[kk] = 1;
                                                    //point inside, so now interpolation of stress_yy
                                                    // Calculate and return the weighted average value
                                                    if (stress_selector[kk]==0)  //stress_xx
                                                        inside1_sigx_max[j][kk] = ((alpha * stress[0][perm_s[mesh_element[inside_list[jj]].node1 - 1]]) + (beta * stress[0][perm_s[mesh_element[inside_list[jj]].node2 - 1]]) + (gamma * stress[0][perm_s[mesh_element[inside_list[jj]].node3 - 1]]));
                                                    else
                                                        inside1_sigy_max[j][kk] = ((alpha * stress[1][perm_s[mesh_element[inside_list[jj]].node1 - 1]]) + (beta * stress[1][perm_s[mesh_element[inside_list[jj]].node2 - 1]]) + (gamma * stress[1][perm_s[mesh_element[inside_list[jj]].node3 - 1]]));
                                                    break;
                                                }
                                            }
                                        }

                                        //calculating reactions for node1 in reaction procedure

                                        //// REACTION CAL:CULATION FOR NODE 1

                                        is_simple = (mesh_boundary[j].restraint < 7) ? 1 : 0;
                                        point_b.x = mesh_boundary[j].x1;
                                        point_b.y = mesh_boundary[j].y1;
                                        point_c.x = mesh_boundary[j].x2;
                                        point_c.y = mesh_boundary[j].y2;

                                        mesh_boundary[j].state1[sti].Rz = compute_NSEW_reaction(point_b, point_c, mesh_boundary[j].kos, mesh_boundary[j].koc, H, is_simple, supp_x, supp_y, supp1_sigx_max[j], supp1_sigy_max[j], num_adj_supp, adj_supp_x, adj_supp_y, num_inside_NSEW, b_inside, h, inside_x, inside_y, inside1_sigx_max[j], inside1_sigy_max[j], &mesh_boundary[j].state1[sti].Qn);

                                        //if (last_node != mesh_boundary[j].node1) {
                                            printf("node=%d\n", mesh_boundary[j].node1);
                                            printf("H=%f\n", H);
                                            printf("supp.x=%f  supp.y=%f  supp1_sigx=%f  supp1_sigy=%f\n", supp_x, supp_y, supp1_sigx_max[j], supp1_sigy_max[j]);
                                            printf("num_adj_supp=%d\n", num_adj_supp);
                                            for (int iii = 0; iii < num_adj_supp; iii++) {
                                                printf("adj_supp_x[%d]=%f  adj_supp_y[%d]=%f\n", iii, adj_supp_x[iii], iii, adj_supp_y[iii]);
                                            }
                                            printf("num_inside_NSEW=%d\n", num_inside_NSEW);
                                            for (int iii = 0; iii < num_inside_NSEW; iii++) {
                                                if (b_inside[iii])
                                                    printf("inside_x[%d]=%f  inside_y[%d]=%f  inside1_sigx[%d]=%f  inside1_sigy[%d]=%f\n", iii, inside_x[iii], iii, inside_y[iii], iii, inside1_sigx_max[j][iii], iii, inside1_sigy_max[j][iii]);
                                                else printf("[%d] ignored\n",iii);
                                            }

                                            printf("Results: Qn=%f Rz=%f\n\n", mesh_boundary[j].state1[sti].Qn, mesh_boundary[j].state1[sti].Rz);
                                        //}
                                        last_node = mesh_boundary[j].node1;

                                        if (mesh_boundary[j].node2 > 0)  //node exists
                                        {
                                            int adj_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                            int num_adj_supp = 0;
                                            int inside_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                            int num_inside = 0;
                                            int num_inside_NSEW = 4;
                                            int node1 = 0, node2 = 0;
                                            int jj;
                                            double stress1, stress2;
                                            //searching for all elements containing node2 for reaction procedure


                                            for (jj = 0; jj < mesh_element_no; jj++)
                                            {
                                                int found = 0;
                                                if (mesh_element[jj].node1 == mesh_boundary[j].node2)
                                                {
                                                    node1 = mesh_element[jj].node2;
                                                    node2 = mesh_element[jj].node3;
                                                    inside_list[num_inside] = jj;
                                                    num_inside++;
                                                    found = 1;
                                                }
                                                else if  (mesh_element[jj].node2 == mesh_boundary[j].node2)
                                                {
                                                    node1 = mesh_element[jj].node1;
                                                    node2 = mesh_element[jj].node3;
                                                    inside_list[num_inside] = jj;
                                                    num_inside++;
                                                    found = 1;
                                                }
                                                else if (mesh_element[jj].node3 == mesh_boundary[j].node2)
                                                {
                                                    node1 = mesh_element[jj].node1;
                                                    node2 = mesh_element[jj].node2;
                                                    inside_list[num_inside] = jj;
                                                    num_inside++;
                                                    found = 1;
                                                }
                                                if (found)
                                                {
                                                    //check if node1 and node2 are adjacent supported nodes, or inside (non-supported) neighboring nodes
                                                    if (node1 > 0)
                                                    {
                                                        if (mesh_node[node1 - 1].restraint > 5)
                                                        {
                                                            int kk;
                                                            for (kk = 0; kk < num_adj_supp; kk++)
                                                                if (adj_list[kk] == node1) break;
                                                            if (kk == num_adj_supp)  //not found
                                                            {
                                                                adj_list[num_adj_supp] = node1;
                                                                num_adj_supp++;
                                                            }
                                                        }
                                                    }
                                                    if (node2 > 0)
                                                    {
                                                        if (mesh_node[node2 - 1].restraint > 5)
                                                        {
                                                            int kk;
                                                            for (kk = 0; kk < num_adj_supp; kk++)
                                                                if (adj_list[kk] == node2) break;
                                                            if (kk == num_adj_supp)  //not found
                                                            {
                                                                adj_list[num_adj_supp] = node2;
                                                                num_adj_supp++;
                                                            }
                                                        }
                                                    }
                                                }
                                            }

                                            //calculating stresses for node2 in reaction procedure

                                            double supp_x, supp_y;      // Supported node coordinates (m)
                                            //double supp_sigx_min=0.0, supp_sigy_min=0.0;
                                            //double supp_sigx_max=0.0, supp_sigy_max=0.0;

                                            double adj_supp_x[12];         // Array: x-coords of adj supported nodes
                                            double adj_supp_y[12];

                                            double inside_x[12];           // Array: x-coords of inside nodes
                                            double inside_y[12];

                                            double Q_foo;

                                            double Rz;  //reaction
                                            double Rzmax, Rzmin;
                                            BOOL is_simple;

                                            supp_x = mesh_node[mesh_boundary[j].node2 - 1].x;
                                            supp_y = mesh_node[mesh_boundary[j].node2 - 1].y;

                                            for (jj = 0; jj < mesh_element_no; jj++) {
                                                if (mesh_element[jj].node1 == mesh_boundary[j].node2)
                                                {
                                                    supp2_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                    supp2_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                    break;
                                                } else if (mesh_element[jj].node2 == mesh_boundary[j].node2)
                                                {
                                                    supp2_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                    supp2_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                    break;
                                                } else if (mesh_element[jj].node3 == mesh_boundary[j].node2)
                                                {
                                                    supp2_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                    supp2_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                    break;
                                                }
                                            }

                                            for (jj = 0; jj < num_adj_supp; jj++) {
                                                adj_supp_x[jj] = mesh_node[adj_list[jj] - 1].x;
                                                adj_supp_y[jj] = mesh_node[adj_list[jj] - 1].y;
                                            }

                                            //calculating stresses for node2 in reaction procedure
                                            //we have :
                                            // inside_list[num_inside] = jj;
                                            // num_inside++;
                                            //mesh_boundary point 1 coordinates
                                            // mesh_boundary[jk].x1;
                                            // mesh_boundary[jk].y1;
                                            // we create virtual points where NSEW stresses will be calculated from mesh elements stress interpolation
                                            // first we try with distance NSEW_dist equal half of the distance between current boundary point and the next one
                                            // or if this is the last point in boundary series - distance between previous boundary point and the current one
                                            double NSEW_dist;
                                            POINTF pa, pb, pc, pp;
                                            double alpha, beta, gamma;
                                            //double s_value;

                                            if ((j < (mesh_boundary_no - 1)) && (mesh_boundary[j + 1].node2 > 0))  //node 2 of next boundary exists
                                            {
                                                NSEW_dist = sqrt((mesh_boundary[j + 1].x2 - mesh_boundary[j].x2) * (mesh_boundary[j + 1].x2 - mesh_boundary[j].x2) +
                                                                 (mesh_boundary[j + 1].y2 - mesh_boundary[j].y2) * (mesh_boundary[j + 1].y2 - mesh_boundary[j].y2)) / NSEW_dist_div;
                                            } else if (mesh_boundary[j].node1 > 0) //previous node exists
                                            {
                                                NSEW_dist = sqrt((mesh_boundary[j].x2 - mesh_boundary[j].x1) * (mesh_boundary[j].x2 - mesh_boundary[j].x1) +
                                                                 (mesh_boundary[j].y2 - mesh_boundary[j].y1) * (mesh_boundary[j].y2 - mesh_boundary[j].y1)) / NSEW_dist_div;
                                            } else  //in case there is single point
                                            {
                                                if (UNITS == SI)
                                                    NSEW_dist = 0.1;  //10 cm
                                                else
                                                    NSEW_dist = 4.;   //4 inches
                                            }
                                            //searching for element which contains point NORTH, SOUTH, EAST and WETS
                                            int y_offset_i[4] = {1, -1, 0, 0};  //1 EAST, -1 WETS 0 NONE
                                            int x_offset_i[4] = {0, 0, 1, -1};  //0 NONE, 1 NORTH, -1 SOUTH
                                            int stress_selector[4] = {1, 1, 0, 0};  //1 stress_yy, 0 stress_xx

                                            for (int kk=0; kk<4; kk++) {
                                                inside2_sigx_max[j][kk] = 0.;
                                                inside2_sigy_max[j][kk] = 0.;
                                            }

                                            num_inside_NSEW = 4;
                                            int b_inside[4] = {0, 0, 0, 0};

                                            for (int kk = 0; kk < 4; kk++)   //N, S, E, W
                                            {
                                                pp.x = (float) (mesh_boundary[j].x2 + NSEW_dist * x_offset_i[kk]);
                                                pp.y = (float) (mesh_boundary[j].y2 + NSEW_dist * y_offset_i[kk]);

                                                for (int jj = 0; jj < num_inside; jj++) {
                                                    pa.x = (float) mesh_node[mesh_element[inside_list[jj]].node1 - 1].x;
                                                    pa.y = (float) mesh_node[mesh_element[inside_list[jj]].node1 - 1].y;

                                                    pb.x = (float) mesh_node[mesh_element[inside_list[jj]].node2 - 1].x;
                                                    pb.y = (float) mesh_node[mesh_element[inside_list[jj]].node2 - 1].y;

                                                    pc.x = (float) mesh_node[mesh_element[inside_list[jj]].node3 - 1].x;
                                                    pc.y = (float) mesh_node[mesh_element[inside_list[jj]].node3 - 1].y;

                                                    //if (is_point_in_triangle(pa, pb, pc, pp))
                                                    if (isInside(&pa, &pb, &pc, &pp, &alpha, &beta, &gamma)) {
                                                        inside_x[kk] = (mesh_boundary[j].x2 + NSEW_dist * x_offset_i[kk]);
                                                        inside_y[kk] = (mesh_boundary[j].y2 + NSEW_dist * y_offset_i[kk]);
                                                        b_inside[kk] = 1;
                                                        //point inside, so now interpolation of stress_yy
                                                        // Calculate and return the weighted average value
                                                        if (stress_selector[kk]==0)
                                                            inside2_sigx_max[j][kk] = ((alpha * stress[0][perm_s[mesh_element[inside_list[jj]].node1 - 1]]) + (beta * stress[0][perm_s[mesh_element[inside_list[jj]].node2 - 1]]) + (gamma * stress[0][perm_s[mesh_element[inside_list[jj]].node3 - 1]]));
                                                        else
                                                            inside2_sigy_max[j][kk] = ((alpha * stress[1][perm_s[mesh_element[inside_list[jj]].node1 - 1]]) + (beta * stress[1][perm_s[mesh_element[inside_list[jj]].node2 - 1]]) + (gamma * stress[1][perm_s[mesh_element[inside_list[jj]].node3 - 1]]));
                                                        break;
                                                    }
                                                }
                                            }

                                            //calculating reactions for node1 in reaction procedure

                                            //// REACTION CAL:CULATION FOR NODE 1

                                            is_simple = (mesh_boundary[j].restraint < 7) ? 1 : 0;
                                            point_b.x = mesh_boundary[j].x1;
                                            point_b.y = mesh_boundary[j].y1;
                                            point_c.x = mesh_boundary[j].x2;
                                            point_c.y = mesh_boundary[j].y2;

                                            mesh_boundary[j].state2[sti].Rz = compute_NSEW_reaction(point_b, point_c, mesh_boundary[j].kos, mesh_boundary[j].koc, H, is_simple, supp_x, supp_y, supp2_sigx_max[j], supp2_sigy_max[j], num_adj_supp, adj_supp_x, adj_supp_y, num_inside_NSEW, b_inside, h, inside_x, inside_y, inside2_sigx_max[j], inside2_sigy_max[j], &mesh_boundary[j].state2[sti].Qn);

                                            //if (last_node != mesh_boundary[j].node2) {
                                                printf("node=%d\n", mesh_boundary[j].node2);
                                                printf("H=%f\n", H);
                                                printf("supp.x=%f  supp.y=%f  supp2_sigx=%f  supp2_sigy=%f\n", supp_x, supp_y, supp2_sigx_max[j], supp2_sigy_max[j]);
                                                printf("num_adj_supp=%d\n", num_adj_supp);
                                                for (int iii = 0; iii < num_adj_supp; iii++) {
                                                    printf("adj_supp_x[%d]=%f  adj_supp_y[%d]=%f\n", iii, adj_supp_x[iii], iii, adj_supp_y[iii]);
                                                }
                                                printf("num_inside_NSEW=%d\n", num_inside_NSEW);
                                                for (int iii = 0; iii < num_inside_NSEW; iii++) {
                                                    if (b_inside[iii])
                                                    printf("inside_x[%d]=%f  inside_y[%d]=%f  inside2_sigx[%d]=%f  inside2_sigy[%d]=%f\n", iii, inside_x[iii], iii, inside_y[iii], iii, inside2_sigx_max[j][iii], iii, inside2_sigy_max[j][iii]);
                                                    else printf("[%d] ignored\n",iii);
                                                }
                                                printf("Results: Qn=%f Rz=%f\n\n", mesh_boundary[j].state2[sti].Qn, mesh_boundary[j].state2[sti].Rz);
                                            //}
                                            last_node = mesh_boundary[j].node2;

                                        }
                                    }
                                }
                            }
                        }
#endif
                        if ((stress[i] != NULL) && (i == 3)) //stress_xy
                        {
                            printf("TRACE 1\n");
                            for (j = 0; j < mesh_boundaries_no; j++)
                            {
                                if (mesh_boundary[j].restraint > 5)  //6 simple supported, 7 fixed
                                {
                                    if (mesh_boundary[j].node1 > 0)  //node 1 exists
                                    {
                                        int adj_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                        int num_adj_supp = 0;
                                        int inside_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                        double inside_list_stress_x[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0};
                                        double inside_list_stress_y[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0};
                                        double inside_list_stress_xy[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0};

                                        int num_inside = 0;
                                        int node1 = 0, node2 = 0;
                                        int jj;
                                        double stress1x, stress1y, stress1xy, stress2x, stress2y, stress2xy;
                                        //searching for all elements containing node1
                                        for (jj = 0; jj < mesh_element_no; jj++) {
                                            int found = 0;
                                            if (mesh_element[jj].node1 == mesh_boundary[j].node1) {
                                                node1 = mesh_element[jj].node2;
                                                node2 = mesh_element[jj].node3;

                                                stress1x = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                stress1y = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                stress1xy = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;

                                                stress2x = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                stress2y = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                stress2xy = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                found = 1;
                                            } else if (mesh_element[jj].node2 == mesh_boundary[j].node1) {
                                                node1 = mesh_element[jj].node1;
                                                node2 = mesh_element[jj].node3;

                                                stress1x = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                stress1y = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                stress1xy = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;

                                                stress2x = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                stress2y = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                stress2xy = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                found = 1;
                                            } else if (mesh_element[jj].node3 == mesh_boundary[j].node1) {
                                                node1 = mesh_element[jj].node1;
                                                node2 = mesh_element[jj].node2;

                                                stress1x = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                stress1y = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                stress1xy = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;

                                                stress2x = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                stress2y = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                stress2xy = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                found = 1;
                                            }
                                            if (found) {
                                                //check if node1 and node2 are adjacent supported nodes, or inside (non-supported) neighboring nodes
                                                if (node1 > 0) {
                                                    if (mesh_node[node1 - 1].restraint > 5) {
                                                        int kk;
                                                        for (kk = 0; kk < num_adj_supp; kk++)
                                                            if (adj_list[kk] == node1) break;
                                                        if (kk == num_adj_supp)  //not found
                                                        {
                                                            adj_list[num_adj_supp] = node1;
                                                            num_adj_supp++;
                                                        }
                                                    } else {
                                                        int kk;
                                                        for (kk = 0; kk < num_inside; kk++)
                                                            if (inside_list[kk] == node1) break;
                                                        if (kk == num_inside)  //not found
                                                        {
                                                            inside_list[num_inside] = node1;
                                                            inside_list_stress_x[num_inside] = stress1x;
                                                            inside_list_stress_y[num_inside] = stress1y;
                                                            inside_list_stress_xy[num_inside] = stress1xy;
                                                            num_inside++;
                                                        }
                                                    }
                                                }
                                                if (node2 > 0) {
                                                    if (mesh_node[node2 - 1].restraint > 5) {
                                                        int kk;
                                                        for (kk = 0; kk < num_adj_supp; kk++)
                                                            if (adj_list[kk] == node2) break;
                                                        if (kk == num_adj_supp)  //not found
                                                        {
                                                            adj_list[num_adj_supp] = node2;
                                                            num_adj_supp++;
                                                        }
                                                    } else {
                                                        int kk;
                                                        for (kk = 0; kk < num_inside; kk++)
                                                            if (inside_list[kk] == node2) break;
                                                        if (kk == num_inside)  //not found
                                                        {
                                                            inside_list[num_inside] = node2;
                                                            inside_list_stress_x[num_inside] = stress2x;
                                                            inside_list_stress_y[num_inside] = stress2y;
                                                            inside_list_stress_xy[num_inside] = stress2xy;
                                                            num_inside++;
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        //calculating reactions for node1
                                        double supp_x, supp_y;      // Supported node coordinates (m)

                                        double adj_supp_x[12];         // Array: x-coords of adj supported nodes
                                        double adj_supp_y[12];

                                        double inside_x[12];           // Array: x-coords of inside nodes
                                        double inside_y[12];

                                        double H = 0.15, h[12] = {0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15,
                                                                  0.15,
                                                                  0.15, 0.15, 0.15}; //TEMPORARY

                                        double Q_foo;

                                        double Rz;  //reaction
                                        double Rzmax, Rzmin;

                                        supp_x = mesh_node[mesh_boundary[j].node1 - 1].x;
                                        supp_y = mesh_node[mesh_boundary[j].node1 - 1].y;

                                        for (jj = 0; jj < mesh_element_no; jj++) {
                                            if (mesh_element[jj].node1 == mesh_boundary[j].node1)
                                            {
                                                supp1_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                supp1_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                supp1_sigxy_max[j] = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                break;
                                            }
                                            else if (mesh_element[jj].node2 == mesh_boundary[j].node1)
                                            {
                                                supp1_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                supp1_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                supp1_sigxy_max[j] = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                break;
                                            }
                                            else if (mesh_element[jj].node3 == mesh_boundary[j].node1)
                                            {
                                                supp1_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                supp1_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                supp1_sigxy_max[j] = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                break;
                                            }
                                        }

                                        for (jj = 0; jj < num_adj_supp; jj++)
                                        {
                                            adj_supp_x[jj] = mesh_node[adj_list[jj] - 1].x;
                                            adj_supp_y[jj] = mesh_node[adj_list[jj] - 1].y;
                                        }
                                        for (jj = 0; jj < num_inside; jj++)
                                        {
                                            inside_x[jj] = mesh_node[inside_list[jj] - 1].x;
                                            inside_y[jj] = mesh_node[inside_list[jj] - 1].y;

                                            inside1_sigx_max[j][jj] = inside_list_stress_x[jj]; //stress1x;  //just max
                                            inside1_sigy_max[j][jj] = inside_list_stress_y[jj]; //stress1y;  //just max
                                            inside1_sigxy_max[j][jj] = inside_list_stress_xy[jj]; //stress1xy;  //just max
                                        }

                                        //// REACTIONS
                                        mesh_boundary[j].state1[sti].Rz1 = compute_support_reaction(H, supp_x,supp_y,supp1_sigx_max[j],supp1_sigy_max[j],supp1_sigxy_max[j],num_adj_supp,adj_supp_x,adj_supp_y,num_inside,h,inside_x,inside_y,inside1_sigx_max[j],inside1_sigy_max[j],inside1_sigxy_max[j],&mesh_boundary[j].state1[sti].Qn1);
#ifndef NDEBUG
                                        //if (last_node != mesh_boundary[j].node1)
                                        //{
                                            printf("ALTERNATIVE\n");
                                            printf("node=%d\n", mesh_boundary[j].node1);
                                            printf("H=%f\n", H);
                                            printf("supp.x=%f  supp.y=%f  supp1_sigx=%f  supp1_sigy=%f  supp1_sigxy=%f\n",
                                                   supp_x, supp_y, supp1_sigx_max[j], supp1_sigy_max[j], supp1_sigxy_max[j]);
                                            printf("num_adj_supp=%d\n", num_adj_supp);
                                            for (int iii = 0; iii < num_adj_supp; iii++) {
                                                printf("adj_supp_x[%d]=%f  adj_supp_y[%d]=%f\n",iii,adj_supp_x[iii],iii, adj_supp_y[iii]);
                                            }
                                            printf("num_inside=%d\n", num_inside);
                                            for (int iii = 0; iii < num_inside; iii++) {
                                                printf("inside_x[%d]=%f  inside_y[%d]=%f  inside1_sigx[%d]=%f  inside1_sigy[%d]=%f  inside1_sigxy[%d]=%f\n",iii,inside_x[iii],iii,inside_y[iii],iii,inside1_sigx_max[j][iii],iii,inside1_sigy_max[j][iii],iii,inside1_sigxy_max[j][iii]);
                                            }

                                            printf("Results: Qn1=%f Rz1=%f\n\n", mesh_boundary[j].state1[sti].Qn1, mesh_boundary[j].state1[sti].Rz1);
                                        //}
                                        //last_node=mesh_boundary[j].node1;
#endif
                                    }


                                    if (mesh_boundary[j].node2 > 0)  //node 2 exists
                                    {
                                        int adj_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                        int num_adj_supp = 0;
                                        int inside_list[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                        double inside_list_stress_x[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0};
                                        double inside_list_stress_y[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0};
                                        double inside_list_stress_xy[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0};

                                        int num_inside = 0;
                                        int node1 = 0, node2 = 0;
                                        int jj;
                                        double stress1x, stress1y, stress1xy, stress2x, stress2y, stress2xy;
                                        //searching for all elements containing node1
                                        for (jj = 0; jj < mesh_element_no; jj++) {
                                            int found = 0;
                                            if (mesh_element[jj].node1 == mesh_boundary[j].node2) {
                                                node1 = mesh_element[jj].node2;
                                                node2 = mesh_element[jj].node3;

                                                stress1x = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                stress1y = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                stress1xy = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;

                                                stress2x = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                stress2y = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                stress2xy = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                found = 1;
                                            } else if (mesh_element[jj].node2 == mesh_boundary[j].node2) {
                                                node1 = mesh_element[jj].node1;
                                                node2 = mesh_element[jj].node3;

                                                stress1x = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                stress1y = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                stress1xy = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;

                                                stress2x = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                stress2y = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                stress2xy = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                found = 1;
                                            } else if (mesh_element[jj].node3 == mesh_boundary[j].node2) {
                                                node1 = mesh_element[jj].node1;
                                                node2 = mesh_element[jj].node2;

                                                stress1x = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                stress1y = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                stress1xy = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;

                                                stress2x = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                stress2y = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                stress2xy = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                found = 1;
                                            }
                                            if (found) {
                                                //check if node1 and node2 are adjacent supported nodes, or inside (non-supported) neighboring nodes
                                                if (node1 > 0) {
                                                    if (mesh_node[node1 - 1].restraint > 5) {
                                                        int kk;
                                                        for (kk = 0; kk < num_adj_supp; kk++)
                                                            if (adj_list[kk] == node1) break;
                                                        if (kk == num_adj_supp)  //not found
                                                        {
                                                            adj_list[num_adj_supp] = node1;
                                                            num_adj_supp++;
                                                        }
                                                    } else {
                                                        int kk;
                                                        for (kk = 0; kk < num_inside; kk++)
                                                            if (inside_list[kk] == node1) break;
                                                        if (kk == num_inside)  //not found
                                                        {
                                                            inside_list[num_inside] = node1;
                                                            inside_list_stress_x[num_inside] = stress1x;
                                                            inside_list_stress_y[num_inside] = stress1y;
                                                            inside_list_stress_xy[num_inside] = stress1xy;
                                                            num_inside++;
                                                        }
                                                    }
                                                }
                                                if (node2 > 0) {
                                                    if (mesh_node[node2 - 1].restraint > 5) {
                                                        int kk;
                                                        for (kk = 0; kk < num_adj_supp; kk++)
                                                            if (adj_list[kk] == node2) break;
                                                        if (kk == num_adj_supp)  //not found
                                                        {
                                                            adj_list[num_adj_supp] = node2;
                                                            num_adj_supp++;
                                                        }
                                                    } else {
                                                        int kk;
                                                        for (kk = 0; kk < num_inside; kk++)
                                                            if (inside_list[kk] == node2) break;
                                                        if (kk == num_inside)  //not found
                                                        {
                                                            inside_list[num_inside] = node2;
                                                            inside_list_stress_x[num_inside] = stress2x;
                                                            inside_list_stress_y[num_inside] = stress2y;
                                                            inside_list_stress_xy[num_inside] = stress2xy;
                                                            num_inside++;
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        //calculating reactions for node1
                                        double supp_x, supp_y;      // Supported node coordinates (m)

                                        double adj_supp_x[12];         // Array: x-coords of adj supported nodes
                                        double adj_supp_y[12];

                                        double inside_x[12];           // Array: x-coords of inside nodes
                                        double inside_y[12];

                                        double H = 0.15, h[12] = {0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15,
                                                                  0.15,
                                                                  0.15, 0.15, 0.15}; //TEMPORARY

                                        double Q_foo;

                                        double Rz;  //reaction
                                        double Rzmax, Rzmin;

                                        supp_x = mesh_node[mesh_boundary[j].node2 - 1].x;
                                        supp_y = mesh_node[mesh_boundary[j].node2 - 1].y;

                                        for (jj = 0; jj < mesh_element_no; jj++) {
                                            if (mesh_element[jj].node1 == mesh_boundary[j].node2)
                                            {
                                                supp2_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                supp2_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                supp2_sigxy_max[j] = stress[3][perm_s[mesh_element[jj].node1 - 1]];// * 1.0e-6;
                                                break;
                                            }
                                            else if (mesh_element[jj].node2 == mesh_boundary[j].node2)
                                            {
                                                supp2_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                supp2_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                supp2_sigxy_max[j] = stress[3][perm_s[mesh_element[jj].node2 - 1]];// * 1.0e-6;
                                                break;
                                            }
                                            else if (mesh_element[jj].node3 == mesh_boundary[j].node2)
                                            {
                                                supp2_sigx_max[j] = stress[0][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                supp2_sigy_max[j] = stress[1][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                supp2_sigxy_max[j] = stress[3][perm_s[mesh_element[jj].node3 - 1]];// * 1.0e-6;
                                                break;
                                            }
                                        }

                                        for (jj = 0; jj < num_adj_supp; jj++)
                                        {
                                            adj_supp_x[jj] = mesh_node[adj_list[jj] - 1].x;
                                            adj_supp_y[jj] = mesh_node[adj_list[jj] - 1].y;
                                        }
                                        for (jj = 0; jj < num_inside; jj++)
                                        {
                                            inside_x[jj] = mesh_node[inside_list[jj] - 1].x;
                                            inside_y[jj] = mesh_node[inside_list[jj] - 1].y;

                                            inside2_sigx_max[j][jj] = inside_list_stress_x[jj]; //stress1x;  //just max
                                            inside2_sigy_max[j][jj] = inside_list_stress_y[jj]; //stress1y;  //just max
                                            inside2_sigxy_max[j][jj] = inside_list_stress_xy[jj]; //stress1xy;  //just max
                                        }

                                        //// REACTIONS
                                        mesh_boundary[j].state2[sti].Rz1 = compute_support_reaction(H, supp_x,supp_y,supp2_sigx_max[j],supp2_sigy_max[j],supp2_sigxy_max[j],num_adj_supp,adj_supp_x,adj_supp_y,num_inside,h,inside_x,inside_y,inside2_sigx_max[j],inside2_sigy_max[j],inside2_sigxy_max[j],&mesh_boundary[j].state2[sti].Qn1);
#ifndef NDEBUG
                                        //if (last_node != mesh_boundary[j].node1)
                                        //{
                                        printf("ALTERNATIVE\n");
                                        printf("node=%d\n", mesh_boundary[j].node2);
                                        printf("H=%f\n", H);
                                        printf("supp.x=%f  supp.y=%f  supp2_sigx=%f  supp2_sigy=%f  supp2_sigxy=%f\n",
                                               supp_x, supp_y, supp2_sigx_max[j], supp2_sigy_max[j], supp2_sigxy_max[j]);
                                        printf("num_adj_supp=%d\n", num_adj_supp);
                                        for (int iii = 0; iii < num_adj_supp; iii++) {
                                            printf("adj_supp_x[%d]=%f  adj_supp_y[%d]=%f\n",iii,adj_supp_x[iii],iii, adj_supp_y[iii]);
                                        }
                                        printf("num_inside=%d\n", num_inside);
                                        for (int iii = 0; iii < num_inside; iii++) {
                                            printf("inside_x[%d]=%f  inside_y[%d]=%f  inside2_sigx[%d]=%f  inside2_sigy[%d]=%f  inside2_sigxy[%d]=%f\n",iii,inside_x[iii],iii,inside_y[iii],iii,inside2_sigx_max[j][iii],iii,inside2_sigy_max[j][iii],iii,inside2_sigxy_max[j][iii]);
                                        }

                                        printf("Results: Qn1=%f Rz1=%f\n\n", mesh_boundary[j].state2[sti].Qn1, mesh_boundary[j].state2[sti].Rz1);
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
                goto pl_error1;
            }
            if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

                ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_,
                                   12, (char *) "", 11, 1, 62);
                no_error = FALSE;
                goto pl_error1;
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
                            goto pl_error1;
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
                                    if (perm_d[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, epsilon_amax);
                                    else gradient.c1 = getRGB(min_epsilon[i][perm_e[mesh_element[j].node1 - 1]], epsilon_amax);
                                    if (perm_d[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, epsilon_amax);
                                    else gradient.c2 = getRGB(min_epsilon[i][perm_e[mesh_element[j].node2 - 1]], epsilon_amax);
                                    if (perm_d[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, epsilon_amax);
                                    else gradient.c3 = getRGB(min_epsilon[i][perm_e[mesh_element[j].node3 - 1]], epsilon_amax);
                                    gradient.c4 = getRGB(0.0, epsilon_amax);

                                    fe_data.el_number = j + 1;
                                    fe_data.flag = 0;
                                    fe_data.f1 = (float) min_epsilon[i][perm_e[mesh_element[j].node1 - 1]];
                                    fe_data.f2 = (float) min_epsilon[i][perm_e[mesh_element[j].node2 - 1]];
                                    fe_data.f3 = (float) min_epsilon[i][perm_e[mesh_element[j].node3 - 1]];
                                    fe_data.f4 = 0.0f;
                                } else {
                                    if (perm_d[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, epsilon_amax);
                                    else gradient.c1 = getRGB(max_epsilon[i][perm_e[mesh_element[j].node1 - 1]], epsilon_amax);
                                    if (perm_d[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, epsilon_amax);
                                    else gradient.c2 = getRGB(max_epsilon[i][perm_e[mesh_element[j].node2 - 1]], epsilon_amax);
                                    if (perm_d[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, epsilon_amax);
                                    else gradient.c3 = getRGB(max_epsilon[i][perm_e[mesh_element[j].node3 - 1]], epsilon_amax);
                                    gradient.c4 = getRGB(0.0, epsilon_amax);

                                    fe_data.el_number = j + 1;
                                    fe_data.flag = 0;
                                    fe_data.f1 = (float) max_epsilon[i][perm_e[mesh_element[j].node1 - 1]];
                                    fe_data.f2 = (float) max_epsilon[i][perm_e[mesh_element[j].node2 - 1]];
                                    fe_data.f3 = (float) max_epsilon[i][perm_e[mesh_element[j].node3 - 1]];
                                    fe_data.f4 = 0.0f;
                                }
                            } else {
                                if (perm_e[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, epsilon_amax);
                                else gradient.c1 = getRGB(epsilon[i][perm_e[mesh_element[j].node1 - 1]], epsilon_amax);
                                if (perm_e[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, epsilon_amax);
                                else gradient.c2 = getRGB(epsilon[i][perm_e[mesh_element[j].node2 - 1]], epsilon_amax);
                                if (perm_e[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, epsilon_amax);
                                else gradient.c3 = getRGB(epsilon[i][perm_e[mesh_element[j].node3 - 1]], epsilon_amax);
                                gradient.c4 = getRGB(0.0, epsilon_amax);

                                fe_data.el_number = j + 1;
                                fe_data.flag = 0;
                                fe_data.f1 = (float) epsilon[i][perm_e[mesh_element[j].node1 - 1]];
                                fe_data.f2 = (float) epsilon[i][perm_e[mesh_element[j].node2 - 1]];
                                fe_data.f3 = (float) epsilon[i][perm_e[mesh_element[j].node3 - 1]];
                                fe_data.f4 = 0.0f;
                            }

                            w.lp = 6;
                            w.gradient = 1;

                            w.translucent = 1;
                            translucency_ptr = (char *) w.xy;
                            translucency_ptr += (w.lp * sizeof(float));
                            memmove(translucency_ptr, &HalfTranslucency, sizeof(unsigned char));

                            gradient_ptr = translucency_ptr + sizeof(unsigned char);
                            memmove(gradient_ptr, &gradient, sizeof(GRADIENT));

                            fe_data_ptr = gradient_ptr + sizeof(GRADIENT);
                            memmove(fe_data_ptr, &fe_data, sizeof(FE_DATA));

                            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT) + sizeof(FE_DATA);
                            adr = dodaj_obiekt((BLOK *) dane, &w);

                        }

                        //colorbar
                        if (epsilon_amax > 0.0) {
                            w.xy[0] = (float) pl_min_x + 10.f;
                            w.xy[1] = (float) pl_min_y - 10.f;

                            w.xy[2] = w.xy[0] + 50.f;
                            w.xy[3] = w.xy[1];

                            w.xy[4] = w.xy[2];
                            w.xy[5] = w.xy[3] - 5.f;

                            w.xy[6] = w.xy[0];
                            w.xy[7] = w.xy[5];

                            if (nom_max) {
                                if (bi == 0) {
                                    gradient.c1 = getRGB(min_epsilon_min[i], epsilon_amax);
                                    gradient.c2 = getRGB(max_epsilon_max[i], epsilon_amax);
                                    gradient.c3 = getRGB(max_epsilon_max[i], epsilon_amax);
                                    gradient.c4 = getRGB(min_epsilon_min[i], epsilon_amax);

                                    fe_data.el_number = 0;
                                    fe_data.flag = 0;
                                    fe_data.f1 = (float) min_epsilon_min[i];
                                    fe_data.f2 = (float) max_epsilon_max[i];
                                    fe_data.f3 = (float) max_epsilon_max[i];
                                    fe_data.f4 = (float) min_epsilon_min[i];
                                } else {
                                    gradient.c1 = getRGB(min_epsilon_min[i], epsilon_amax);
                                    gradient.c2 = getRGB(max_epsilon_max[i], epsilon_amax);
                                    gradient.c3 = getRGB(max_epsilon_max[i], epsilon_amax);
                                    gradient.c4 = getRGB(min_epsilon_min[i], epsilon_amax);

                                    fe_data.el_number = 0;
                                    fe_data.flag = 0;
                                    fe_data.f1 = (float) min_epsilon_min[i];
                                    fe_data.f2 = (float) max_epsilon_max[i];
                                    fe_data.f3 = (float) max_epsilon_max[i];
                                    fe_data.f4 = (float) min_epsilon_min[i];
                                }
                            } else {
                                gradient.c1 = getRGB(epsilon_min[i], epsilon_amax);
                                gradient.c2 = getRGB(epsilon_max[i], epsilon_amax);
                                gradient.c3 = getRGB(epsilon_max[i], epsilon_amax);
                                gradient.c4 = getRGB(epsilon_min[i], epsilon_amax);

                                fe_data.el_number = 0;
                                fe_data.flag = 0;
                                fe_data.f1 = (float) epsilon_min[i];
                                fe_data.f2 = (float) epsilon_max[i];
                                fe_data.f3 = (float) epsilon_max[i];
                                fe_data.f4 = (float) epsilon_min[i];
                            }

                            w.lp = 8;
                            w.gradient = 1;
                            w.translucent = 1;
                            translucency_ptr = (char *) w.xy;
                            translucency_ptr += (w.lp * sizeof(float));
                            memmove(translucency_ptr, &HalfTranslucency, sizeof(unsigned char));

                            gradient_ptr = translucency_ptr + sizeof(unsigned char);
                            memmove(gradient_ptr, &gradient, sizeof(GRADIENT));

                            fe_data_ptr = gradient_ptr + sizeof(GRADIENT);
                            memmove(fe_data_ptr, &fe_data, sizeof(FE_DATA));

                            w.n = 8 + w.lp * sizeof(float) + sizeof(unsigned char) + sizeof(GRADIENT) + sizeof(FE_DATA);
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

        pl_error2:

        if (sti == 1)  //ULS
        {
            //stress backup
            for (int is = 0; is < STRESS_NUMBER; is++) {
                min_stress_min_bak[is] = stress_min[is];
                max_stress_max_bak[is] = stress_max[is];
                min_stress_bak[is] = (double *) malloc(perm_s_index_no * sizeof(double) + 100);
                max_stress_bak[is] = (double *) malloc(perm_s_index_no * sizeof(double) + 100);
                memmove(min_stress_bak[is], stress[is], perm_s_index_no * sizeof(double));
                memmove(max_stress_bak[is], stress[is], perm_s_index_no * sizeof(double));
            }
            perm_s_bak = (int *) malloc(perm_s_node_no * sizeof(int) + 100);
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

#ifdef REACIONSNSEW

            Rzmin = 0.0;
            Rzmax = 0.0;

            Rzmin_back = 0.0;
            Rzmax_back = 0.0;

#ifndef NDEBUG
            printf("\nState %s\n", SLS_ULS);
#endif

            for (i = 0; i < mesh_boundaries_no; i++) {
                if (mesh_boundary[i].restraint > 5) {
                    if (mesh_boundary[i].node1 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti].Rz, mesh_boundary[i].state1[sti].Qn);
#endif
                        Rzmax += nofabs(mesh_boundary[i].state1[sti].Rz);
                    }
                    last_node = mesh_boundary[i].node1;
                    if (mesh_boundary[i].node2 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti].Rz, mesh_boundary[i].state2[sti].Qn);
#endif
                        Rzmax += nofabs(mesh_boundary[i].state2[sti].Rz);
                    }
                    last_node = mesh_boundary[i].node2;
                }
            }
#ifndef NDEBUG
            printf("SUM:  Rz_total=%f\n", Rzmax);
#endif

            Rzmin_back = Rzmin;
            Rzmax_back = Rzmax;
#ifndef NDEBUG
            printf("\nTotal load in state %s:  %f\n", SLS_ULS, total_load);
#endif
            load_ratio = total_load / Rzmax_back;

            Rzmin = 0.0;
            Rzmax = 0.0;
#ifndef NDEBUG
            printf("\nState %s with correction ratio %f\n", SLS_ULS, load_ratio);
#endif
            for (i = 0; i < mesh_boundaries_no; i++) {
                if (mesh_boundary[i].restraint > 5)
                {
                    mesh_boundary[i].state1[sti].Qn *= load_ratio;
                    mesh_boundary[i].state1[sti].Rz *= load_ratio;
                    mesh_boundary[i].state2[sti].Qn *= load_ratio;
                    mesh_boundary[i].state2[sti].Rz *= load_ratio;

                    if (mesh_boundary[i].node1 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti].Rz, mesh_boundary[i].state1[sti].Qn);
#endif
                        Rzmax += (nofabs(mesh_boundary[i].state1[sti].Rz));
                    }
                    last_node = mesh_boundary[i].node1;
                    if (mesh_boundary[i].node2 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti].Rz, mesh_boundary[i].state2[sti].Qn);
#endif
                        Rzmax += (nofabs(mesh_boundary[i].state2[sti].Rz));
                    }
                    last_node = mesh_boundary[i].node2;
                }
            }
#ifndef NDEBUG
            printf("SUM:  Rz_total=%f\n", Rzmax);
#endif
#endif
            //ALTERNATIVE

            last_node = 0;

            Rzmin = 0.0;
            Rzmax = 0.0;

            Rzmin_back = 0.0;
            Rzmax_back = 0.0;
#ifndef NDEBUG
            printf("\nALTERNATIVE\n", SLS_ULS);
            printf("\nState %s\n", SLS_ULS);
#endif

            for (i = 0; i < mesh_boundaries_no; i++) {
                if (mesh_boundary[i].restraint > 5)
                {
                    if (mesh_boundary[i].node1 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz1=%f  Qn1=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti].Rz1, mesh_boundary[i].state1[sti].Qn1);
#endif
                        Rzmax += nofabs(mesh_boundary[i].state1[sti].Rz1);
                    }
                    last_node = mesh_boundary[i].node1;
                    if (mesh_boundary[i].node2 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz1=%f  Qn1=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti].Rz1, mesh_boundary[i].state2[sti].Qn1);
#endif
                        Rzmax += nofabs(mesh_boundary[i].state2[sti].Rz1);
                    }
                    last_node = mesh_boundary[i].node2;
                }
            }
#ifndef NDEBUG
            printf("SUM:  Rz1_total=%f\n", Rzmax);
#endif
            Rzmin_back = Rzmin;
            Rzmax_back = Rzmax;
#ifndef NDEBUG
            printf("\nTotal load in state %s:  %f\n", SLS_ULS, total_load);
#endif
            load_ratio = total_load / Rzmax_back;

            Rzmin = 0.0;
            Rzmax = 0.0;
#ifndef NDEBUG
            printf("\nState %s with correction ratio %f\n", SLS_ULS, load_ratio);
#endif
            for (i = 0; i < mesh_boundaries_no; i++) {
                if (mesh_boundary[i].restraint > 5)
                {
                    mesh_boundary[i].state1[sti].Qn1 *= load_ratio;
                    mesh_boundary[i].state1[sti].Rz1 *= load_ratio;
                    mesh_boundary[i].state2[sti].Qn1 *= load_ratio;
                    mesh_boundary[i].state2[sti].Rz1 *= load_ratio;

                    if (mesh_boundary[i].node1 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz1=%f  Qn1=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti].Rz1, mesh_boundary[i].state1[sti].Qn1);
#endif
                        Rzmax += (nofabs(mesh_boundary[i].state1[sti].Rz1));
                    }
                    last_node = mesh_boundary[i].node1;
                    if (mesh_boundary[i].node2 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz1=%f  Qn1=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti].Rz1, mesh_boundary[i].state2[sti].Qn1);
#endif
                        Rzmax += (nofabs(mesh_boundary[i].state2[sti].Rz1));
                    }
                    last_node = mesh_boundary[i].node2;
                }
            }
#ifndef NDEBUG
            printf("SUM:  Rz1_total=%f\n", Rzmax);
#endif
        }
        else if ((sti!=2) || (ULSLC_flag[2]!=0)) // if ULSLC, flag must exist
        {
            int last_node = 0;

#ifdef REACIONSNSEW
            Rzmin = 0.0;
#ifndef NDEBUG
            printf("\nState %s\n", SLS_ULS);
            printf("\nMIN\n");
#endif

            for (i = 0; i < mesh_boundaries_no; i++) {
                if (mesh_boundary[i].restraint > 5) {
                    if (mesh_boundary[i].node1 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti].Rzmin, mesh_boundary[i].state1[sti].Qnmin);
#endif
                        Rzmin += nofabs(mesh_boundary[i].state1[sti].Rzmin);
                    }
                    last_node = mesh_boundary[i].node1;
                    if (mesh_boundary[i].node2 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti].Rzmin, mesh_boundary[i].state2[sti].Qnmin);
#endif
                        Rzmin += nofabs(mesh_boundary[i].state2[sti].Rzmin);
                    }
                    last_node = mesh_boundary[i].node2;
                }
            }
#ifndef NDEBUG
            printf("SUM:  Rz_total_min=%f\n", Rzmin);
#endif

            last_node = 0;
            Rzmax = 0.0;

#ifndef NDEBUG
            printf("\nMAX\n");
#endif
            for (i = 0; i < mesh_boundaries_no; i++) {
                if (mesh_boundary[i].restraint > 5)
                {
                    if (mesh_boundary[i].node1 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti].Rzmax, mesh_boundary[i].state1[sti].Qnmax);
#endif
                        Rzmax += nofabs(mesh_boundary[i].state1[sti].Rzmax);
                    }
                    last_node = mesh_boundary[i].node1;
                    if (mesh_boundary[i].node2 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz=%f  Qn=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti].Rzmax, mesh_boundary[i].state2[sti].Qnmax);
#endif
                        Rzmax += nofabs(mesh_boundary[i].state2[sti].Rzmax);
                    }
                    last_node = mesh_boundary[i].node2;
                }
            }
#ifndef NDEBUG
            printf("SUM:  Rz_total_max=%f\n", Rzmax);
#endif
#endif
            //ALTERNATIVE

            last_node = 0;
            Rzmin = 0.0;
#ifndef NDEBUG
            printf("\nALTERNATIVE\n", SLS_ULS);
            printf("\nState %s\n", SLS_ULS);
            printf("\nMIN\n");
#endif

            for (i = 0; i < mesh_boundaries_no; i++) {
                if (mesh_boundary[i].restraint > 5) {
                    if (mesh_boundary[i].node1 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz1=%f  Qn1=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti].Rzmin1, mesh_boundary[i].state1[sti].Qnmin1);
#endif
                        Rzmin += nofabs(mesh_boundary[i].state1[sti].Rzmin1);
                    }
                    last_node = mesh_boundary[i].node1;
                    if (mesh_boundary[i].node2 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz1=%f  Qn1=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti].Rzmin1, mesh_boundary[i].state2[sti].Qnmin1);
#endif
                        Rzmin += nofabs(mesh_boundary[i].state2[sti].Rzmin1);
                    }
                    last_node = mesh_boundary[i].node2;
                }
            }
#ifndef NDEBUG
            printf("SUM:  Rz1_total_min=%f\n", Rzmin);
#endif
            last_node = 0;
            Rzmax = 0.0;
#ifndef NDEBUG
            printf("\nMAX\n");
#endif

            for (i = 0; i < mesh_boundaries_no; i++) {
                if (mesh_boundary[i].restraint > 5) {
                    if (mesh_boundary[i].node1 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz1=%f  Qn1=%f\n", mesh_boundary[i].node1, mesh_boundary[i].state1[sti].Rzmax1, mesh_boundary[i].state1[sti].Qnmax1);
#endif
                        Rzmax += nofabs(mesh_boundary[i].state1[sti].Rzmax1);
                    }
                    last_node = mesh_boundary[i].node1;
                    if (mesh_boundary[i].node2 != last_node)
                    {
#ifndef NDEBUG
                        printf("node=%d  Rz1=%f  Qn1=%f\n", mesh_boundary[i].node2, mesh_boundary[i].state2[sti].Rzmax1, mesh_boundary[i].state2[sti].Qnmax1);
#endif
                        Rzmax += nofabs(mesh_boundary[i].state2[sti].Rzmax1);
                    }
                    last_node = mesh_boundary[i].node2;
                }
            }
#ifndef NDEBUG
            printf("SUM:  Rz1_total_max=%f\n", Rzmax);
#endif
        }

        ////creating reaction blocks

        Ldsp.blok = Ldsp1.blok = 1;
        Ldsp.kolor = Ldsp1.kolor =static_colors.reaction_color;
        Ldsp.typ = Ldsp1.typ = 64;  //thick
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
                    goto pl_error1;
                }
                if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

                    ret = ask_question(1, (char *) "", (char *) confirm, (char *) "", (char *) _CANNOT_CREATE_NEW_LAYER_, 12,
                                       (char *) "", 11, 1, 62);
                    no_error = FALSE;
                    goto pl_error1;
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

        Ldsp.warstwa = Ldsp1.warstwa = desired_reactions_layer_no;

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
            goto pl_error3;
        }

        //Ldsp.x1=(float)blok_origin.x; //TEST
        //Ldsp.y1=(float)blok_origin.y;
        //Ldsp.x2=blok_origin.x+100;
        //Ldsp.y2=blok_origin.y+100;
        //adr = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp);
        SPLINE s=Splinedef;
        SPLINE s1=Splinedef;
        s.warstwa=s1.warstwa=Ldsp.warstwa;
        s.kolor=s1.kolor=Ldsp.kolor;
        s.typ=s1.typ=Ldsp.typ;
        s.blok=s1.blok=Ldsp.blok;

        double edge_x1, edge_y1, edge_x2, edge_y2;
        double r_edge_x1, r_edge_y1, r_edge_x2, r_edge_y2;

        double edge_x01;
        double edge_y01;

        double Qn1, Qn2, Qn1m, Qn2m, Qn01, Qn02, Qn01m, Qn02m;
        double Qn1graph, Qn2graph, Qn1mgraph, Qn2mgraph;
        double Rz1, Rz2, Rz1m, Rz2m;
        double Rzsum, Rzsummin;
        double Qnave, Qnavemin;
        double edge_dl, edge_l;
        const double TOL = 1e-9;
        double Qnexplus, Qnexminus;
        double Qnmexplus, Qnmexminus;
        double dxexplus, dxexminus, dxmexplus, dxmexminus;
        double x1dspexplus,y1dspexplus,x2dspexplus,y2dspexplus;
        double x1dspexminus,y1dspexminus,x2dspexminus,y2dspexminus;
        double x1mdspexplus,y1mdspexplus,x2mdspexplus,y2mdspexplus;
        double x1mdspexminus,y1mdspexminus,x2mdspexminus,y2mdspexminus;

        double Qngraphmax=50.0;   //50 mm on drawing, no more, for reasonable graphic view

        int edge=0;
        i = 0;
        while (i < mesh_boundaries_no)
        {
            if (mesh_boundary[i].edge!=edge) //new edge
            {
                edge=mesh_boundary[i].edge;
                int jj=0;
                int last_node=0;
                Rzsum=Rzsummin=0.;
                Qnave=Qnavemin=0.;
                Qnexplus=Qnexminus=0.;
                Qnmexplus=Qnmexminus=0.;
                edge_l=0.;
                dxexplus=dxexminus=dxmexplus=dxmexminus=-1.;  //set initially out of range
                while ((edge==mesh_boundary[i].edge) && (i < mesh_boundaries_no))
                {
                    if (mesh_boundary[i].restraint > 5)
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
                        }
                        else
                        {
                            kos1 = mesh_boundary[i].koc;  //sin(θ+90°)=cos(θ)
                            koc1 = -mesh_boundary[i].kos; //cos(θ+90°)=-sin(θ)
                        }

                        double x1dsp, y1dsp, x2dsp, y2dsp;
                        double x1dspm, y1dspm, x2dspm, y2dspm;

                        if (sti<2)
                        {
                            Qn1=(mesh_boundary[i].state1[sti].Qn1 / unit_factors_pl->R_f);
                            Qn2=(mesh_boundary[i].state2[sti].Qn1 / unit_factors_pl->R_f);

                            Qn1graph = Qn1 / shear_magnitude;
                            Qn2graph = Qn2 / shear_magnitude;
                            //limiting graphs
                            if (getSignFloat(Qn1graph)==1) Qn1graph=min(Qn1graph, Qngraphmax);
                            else Qn1graph=max(Qn1graph, -Qngraphmax);
                            if (getSignFloat(Qn2graph)==1) Qn2graph=min(Qn2graph, Qngraphmax);
                            else Qn2graph=max(Qn2graph, -Qngraphmax);

                            Rz1=(mesh_boundary[i].state1[sti].Rz1 / unit_factors_pl->R_f);
                            Rz2=(mesh_boundary[i].state2[sti].Rz1 / unit_factors_pl->R_f);
                            Rotate_Point(kos1, koc1, edge_x1, edge_y1, edge_x1 + Qn1graph, edge_y1, &x1dsp, &y1dsp);
                            Rotate_Point(kos1, koc1, edge_x2, edge_y2, edge_x2 + Qn2graph, edge_y2, &x2dsp, &y2dsp);

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
                            Qn1=(mesh_boundary[i].state1[sti].Qnmax1 / unit_factors_pl->R_f);
                            Qn2=(mesh_boundary[i].state2[sti].Qnmax1 / unit_factors_pl->R_f);
                            Qn1graph = Qn1 / shear_magnitude;
                            Qn2graph = Qn2 / shear_magnitude;
                            //limiting graphs
                            if (getSignFloat(Qn1graph)==1) Qn1graph=min(Qn1graph, Qngraphmax);
                            else Qn1graph=max(Qn1graph, -Qngraphmax);
                            if (getSignFloat(Qn2graph)==1) Qn2graph=min(Qn2graph, Qngraphmax);
                            else Qn2graph=max(Qn2graph, -Qngraphmax);

                            Rz1=(mesh_boundary[i].state1[sti].Rzmax1 / unit_factors_pl->R_f);
                            Rz2=(mesh_boundary[i].state2[sti].Rzmax1 / unit_factors_pl->R_f);
                            Rotate_Point(kos1, koc1, edge_x1, edge_y1, edge_x1 + Qn1graph, edge_y1, &x1dsp, &y1dsp);
                            Rotate_Point(kos1, koc1, edge_x2, edge_y2, edge_x2 + Qn2graph, edge_y2, &x2dsp, &y2dsp);

                            //min
                            Qn1m=(mesh_boundary[i].state1[sti].Qnmin1 / unit_factors_pl->R_f);
                            Qn2m=(mesh_boundary[i].state2[sti].Qnmin1 / unit_factors_pl->R_f);
                            Qn1mgraph = Qn1m / shear_magnitude;
                            Qn2mgraph = Qn2m / shear_magnitude;
                            //limiting graphs
                            if (getSignFloat(Qn1mgraph)==1) Qn1mgraph=min(Qn1mgraph, Qngraphmax);
                            else Qn1mgraph=max(Qn1mgraph, -Qngraphmax);
                            if (getSignFloat(Qn2mgraph)==1) Qn2mgraph=min(Qn2mgraph, Qngraphmax);
                            else Qn2mgraph=max(Qn2mgraph, -Qngraphmax);

                            Rz1m=(mesh_boundary[i].state1[sti].Rzmin1 / unit_factors_pl->R_f);
                            Rz2m=(mesh_boundary[i].state2[sti].Rzmin1 / unit_factors_pl->R_f);
                            Rotate_Point(kos1, koc1, edge_x1, edge_y1, edge_x1 + Qn1mgraph, edge_y1, &x1dspm, &y1dspm);
                            Rotate_Point(kos1, koc1, edge_x2, edge_y2, edge_x2 + Qn2mgraph, edge_y2, &x2dspm, &y2dspm);

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
                        }
                        Ldsp.x1=(float)x1dsp;
                        Ldsp.y1=(float)y1dsp;
                        Ldsp.x2=(float)x2dsp;
                        Ldsp.y2=(float)y2dsp;

                        if (jj==0) {
                            Ldsp1.x1 = (float) edge_x1;
                            Ldsp1.y1 = (float) edge_y1;
                            Ldsp1.x2 = Ldsp.x1;
                            Ldsp1.y2 = Ldsp.y1;
                            adr = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1);

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

                        if (sti>1)
                        {
                            Qnavemin+=Qn1m*edge_dl;
                            Qnavemin+=Qn2m*edge_dl;
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

                            jj++;
                            s.xy[jj] = Ldsp.y1;

                            if (sti>1) //set only if it's challenging s
                            {
                                if ((getSignFloat(Qn1m)!=getSignFloat(Qn1)) || (fabs(Qn1m)>fabs(Qn1)))
                                    s1.xy[jj] = Ldspm.y1;
                                else
                                    s1.xy[jj] = (float)edge_y1;
                            }

                            jj++;
                            Rzsum+=Rz1;
                            if (sti>1)
                            {
                                Rzsummin+=Rz1m;
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

                        jj++;
                        s.xy[jj]=Ldsp.y2;

                        if (sti>1) //set only if it's challenging s
                        {
                            if ((getSignFloat(Qn2m)!=getSignFloat(Qn2)) || (fabs(Qn2m)>fabs(Qn2)))
                                s1.xy[jj] = Ldspm.y2;
                            else
                                s1.xy[jj] = (float)edge_y2;
                        }

                        jj++;
                        Rzsum+=Rz2;

                        if (sti>1)
                        {
                            Rzsummin+=Rz2m;
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
                    //the sum
                    Ldsp1.x1=(float)edge_x01;
                    Ldsp1.y1=(float)edge_y01;
                    Ldsp1.x2=(float)edge_x2;
                    Ldsp1.y2=(float)edge_y2;
                    PLINIA PL1;
                    parametry_lini(&Ldsp1, &PL1);
                    dx=sqrt((edge_x2-edge_x01)*(edge_x2-edge_x01)+(edge_y2-edge_y01)*(edge_y2-edge_y01))/2.;

                    Ldsp1.x1=Ldsp1.x1-(float)(d*PL1.sin);
                    Ldsp1.y1=Ldsp1.y1+(float)(d*PL1.cos);
                    Ldsp1.x2=Ldsp1.x2-(float)(d*PL1.sin);
                    Ldsp1.y2=Ldsp1.y2+(float)(d*PL1.cos);

                    if (!Check_if_Equal(Rzsum, 0.0))
                       draw_label(&Ldsp1, &Le, dx, 0, 0, Rzsum, r_precision, FALSE, "", "Σ");

                    //average value
                    Ldsp1.x1=Ldsp1.x1-(float)(d1*PL1.sin);
                    Ldsp1.y1=Ldsp1.y1+(float)(d1*PL1.cos);
                    Ldsp1.x2=Ldsp1.x2-(float)(d1*PL1.sin);
                    Ldsp1.y2=Ldsp1.y2+(float)(d1*PL1.cos);

                    if (edge_l>TOL) Qnave /= edge_l;
                    if (!Check_if_Equal(Qnave, 0.0))
                       draw_label(&Ldsp1, &Le, dx, 0, 0, Qnave, r_precision, FALSE, "", "μ");

                    //min
                    if (sti>1)
                    {
                        s1.lp = jj;
                        s1.xy[s1.lp]=0.75f;
                        s1.n = 8 + (s1.lp + 1 ) * sizeof(float);
                        s1.npts=6; //6 means B-Spline multipoints,  //5 means Cardinal multipoints
                        s1.closed=0;
                        adr = dodaj_obiekt((BLOK *) dane, (void *) &s1);

                        Ldsp1.x1 = Ldspm.x2;
                        Ldsp1.y1 = Ldspm.y2;
                        Ldsp1.x2 = (float) edge_x2;
                        Ldsp1.y2 = (float) edge_y2;
                        adr = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1);

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
                        dx=sqrt((edge_x2-edge_x01)*(edge_x2-edge_x01)+(edge_y2-edge_y01)*(edge_y2-edge_y01))/2.;

                        Ldsp1.x1=Ldsp1.x1-(float)(d*PL1.sin);
                        Ldsp1.y1=Ldsp1.y1+(float)(d*PL1.cos);
                        Ldsp1.x2=Ldsp1.x2-(float)(d*PL1.sin);
                        Ldsp1.y2=Ldsp1.y2+(float)(d*PL1.cos);

                        if (!Check_if_Equal(Rzsummin, 0.0))
                        {
                            if ((getSignFloat(Rzsummin)!=getSignFloat(Rzsum)) || (fabs(Rzsummin)>fabs(Rzsum)))
                               draw_label(&Ldsp1, &Le, dx, 0, 0, Rzsummin, r_precision, FALSE, "", "Σ");
                        }

                        //average min value
                        Ldsp1.x1=Ldsp1.x1-(float)(d1*PL1.sin);
                        Ldsp1.y1=Ldsp1.y1+(float)(d1*PL1.cos);
                        Ldsp1.x2=Ldsp1.x2-(float)(d1*PL1.sin);
                        Ldsp1.y2=Ldsp1.y2+(float)(d1*PL1.cos);

                        if (edge_l>TOL) Qnavemin /= edge_l;
                        if (!Check_if_Equal(Qnavemin, 0.0))
                        {
                            if ((getSignFloat(Qnavemin)!=getSignFloat(Qnave)) || (fabs(Qnavemin)>fabs(Qnave)))
                                draw_label(&Ldsp1, &Le, dx, 0, 0, Qnavemin, r_precision, FALSE, "", "μ");
                        }
                    }
                }
            }
        }

pl_error3:
        ////reactions block end

        free(supp1_sigx_max);
        free(supp1_sigy_max);
        free(supp1_sigxy_max);

        free(supp2_sigx_max);
        free(supp2_sigy_max);
        free(supp2_sigxy_max);

        for (i = 0; i < mesh_boundaries_no; i++) {
            free(inside1_sigx_max[i]);
            free(inside1_sigy_max[i]);
            free(inside1_sigxy_max[i]);

            free(inside2_sigx_max[i]);
            free(inside2_sigy_max[i]);
            free(inside2_sigxy_max[i]);
        }

        free(inside1_sigx_max);
        free(inside1_sigy_max);
        free(inside1_sigxy_max);

        free(inside2_sigx_max);
        free(inside2_sigy_max);
        free(inside2_sigxy_max);
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

    free(perm_s_bak);

    pl_error1:

    if (mesh_node) free(mesh_node);
    if (mesh_element) free(mesh_element);
    if (mesh_boundary) free(mesh_boundary);

    if (body_property) free(body_property);

    pl_error:


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

    if (st_uniform_load_comb) free(st_uniform_load_comb);
    if (st_uniform_load_cons) free(st_uniform_load_cons);

    if (pl_property) free(pl_property);
    if (pl_load_factors)free(pl_load_factors);
    if (pl_node) free(pl_node);
    if (pl_node_emb) free(pl_node_emb);
    if (pl_edge) free(pl_edge);
    if (pl_load) free(pl_load);
    if (plate_property) free(plate_property);
    if (hole_property) free(hole_property);
    if (wall_property) free(wall_property);
    if (zone_property) free(zone_property);

    // Free the allocated memory when no longer needed
    for (int i = 0; i < MAX_L_BLOCKS; i++) {
        free(block_names[i]);
    }
    free(block_names);
    ////////////////

    push:
    redraw();

    my_sleep(100);
    return;

}
#undef __O_PLATE__