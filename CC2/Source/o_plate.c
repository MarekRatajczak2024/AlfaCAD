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

extern int hatch_proc_test (long_long, long_long, double, double, T_PTR_Hatch_Param, int comput_area,
                            double df_apx1, double df_apy1, double df_apx2, double df_apy2 );

extern BOOL Check_if_Equal (double x, double y);
extern BOOL Check_if_Equal2(double x, double y);
extern BOOL Check_if_Equal3(double x, double y);

extern BOOL Check_Attribute (int i_attribute, int i_check);
extern unsigned char str2load(char *ptr);
extern void get_blocks_setup(int layer_no, char **block_names, int *block_names_no, int max_no, int max_len);

extern int ask_question_static(int n_buttons, char* esc_string, char* ok_string, char* cont_string, char* comment_string, int color_comment, char* comment1_string, int color1_comment, int cien, int image, int *combinantion, int *geometri_stiffness, int *inertia, int *st_dynamic_no, BOOL *PINNABLE);
extern int EditText(char *mytext, int edit_params, int nCmdShow, int *single, int *tab);
extern int ask_question(int n_buttons, char* esc_string, char* ok_string, char* cont_string, char* comment_string, int color_comment, char* comment1_string, int color1_comment, int cien, int image);

#ifndef LINUX
extern BOOL Is64BitOperatingSystem(void);
#endif

extern int mynCmdShow;

extern ST_LOAD_FACTORS st_load_factors_EU_0[];
extern ST_LOAD_FACTORS st_load_factors_ASCE_0[];
extern ST_LOAD_FACTORS st_load_factors_ICC_0[];

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

UNIT_FACTORS unit_factors_pl_si={0.001, 1.0, 0.0001, 1.0, 0.00000001, 0.000001, 0.000000001, 1.0e9,  1.0, 1.0, 1.0, 1000.0, 1000, 1000.0, 0.001, .000001, 9.81, 0.001};
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

static ST_LOAD_FACTORS *load_factors_pl=NULL;

static ST_LOAD_FACTORS *pl_load_factors=NULL;
int pl_load_factors_no=0;

static PL_NODE *pl_node=NULL;
static PL_EDGE *pl_edge=NULL;
static PL_LOAD *pl_load=NULL;
static GEO_LINE *geo_line=NULL;
LINIA Ldsp = Ldef;

#define MAX_L_BLOCKS 16
#define MAX_L_BLOCKS_LEN 64
static char block_names[MAX_L_BLOCKS][MAX_L_BLOCKS_LEN+1];


static double pl_min_x, pl_min_y, pl_max_x, pl_max_y;

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
int pl_edge_no=0;
int plate_no=0;
int hole_no=0;
int wall_no=0;
int zone_no=0;
int pl_load_no=0;
int mesh_node_no=0;
int mesh_element_no=0;
int mesh_boundary_no=0;
int geo_line_no=0;

int PL_PROPERTY_MAX=10;
int PL_LOAD_FACTORS_MAX=100;
int PL_NODE_MAX=100;
int PL_EDGE_MAX=100;
int PL_LOAD_MAX=10;
int PL_PLATE_MAX=10;
int PL_HOLE_MAX=10;
int PL_WALL_MAX=10;
int PL_ZONE_MAX=10;
int GEO_LINE_MAX=1000;

float dxl=0.1f;
float dxr=0.25f;

float dxl_min=0.1f;
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
void add_load_factors_pl(void)
{
    pl_load_factors_no++;
    if (pl_load_factors_no==PL_LOAD_FACTORS_MAX)
    {
        PL_LOAD_FACTORS_MAX+=100;
        pl_load_factors=realloc(pl_load_factors, PL_LOAD_FACTORS_MAX * sizeof(ST_LOAD_FACTORS));
    }

    memmove(&pl_load_factors[pl_load_factors_no], &load_factors_pl[0], sizeof(ST_LOAD_FACTORS));
}

void add_property_pl(void)
{
    pl_property_no++;
    if (pl_property_no==PL_PROPERTY_MAX) {
        PL_PROPERTY_MAX+=10;
        pl_property=realloc(pl_property, PL_PROPERTY_MAX * sizeof(ST_PROPERTY));
    }
    memmove(&pl_property[pl_property_no], &prt_def_pl, sizeof(ST_PROPERTY));
    if (prop_precisions_pl == &SI_precisions) pl_property[pl_property_no].c=35; //[mm]
    else pl_property[pl_property_no].c=1.5; //[in]
}

void add_node_pl(void)
{
    pl_node_no++;
    if (pl_node_no==PL_NODE_MAX) {
        PL_NODE_MAX+=100;
        pl_node=realloc(pl_node, PL_NODE_MAX * sizeof(ST_NODE));
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
                pl_edge[pl_edge_no].restraint = 6;
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

void Plate_analysis(void) {

    int i, j, k, ii;
    int ret, ret1, ret2, ret_standard, ret_exp;
    double units_factor=1.0;
    double m_units_factor=1000.0;
    double geo_units_factor=1000.0;
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
    double gZ=0;
    GAMMAS gammas_EU = {1.35, 0, 0, 0, 0.85, 1.0};
    GAMMAS gammas_ASCE = {1.4, 0.9, 0.6, 0, 1.2, 0};
    GAMMAS gammas_ICC = {1.4, 0.9, 1.0, 0.6, 1.2, 0};
    GAMMAS *gammas;
    double axis_increment=0.25;
    char prop[MAXEXT];
    int body_number;
    BOOL is_closed;
    int first, last, property_number;
    int check;
    char report[MaxMultitextLen] = "";
    char report_row[MaxTextLen];
    char program[MAXPATH];
    char params[MAXPATH];
    FILE *f;
    BOOL no_error = TRUE;
    char par[14][MaxTextLen]={"","","","","","","","","","","","","",""};
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
    int i_row=0;
    //char data_row2[MaxTextLen];
    char *line;
    int mesh_nodes_no, mesh_elements_no, mesh_boundaries_no;
    double mesh_node_z;
    char desired_layer[maxlen_w];
    char desired_layer_bak[maxlen_w];
    int desired_layer_no;
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
    int *perm_d=NULL;
    int *perm_s=NULL;
    int *perm_e=NULL;
    int perm_d_b=0;
    int perm_s_b=0;
    int perm_e_b=0;
    double *deflection[DEFLECTION_NUMBER]={NULL, NULL, NULL, NULL, NULL, NULL};
    double *stress[STRESS_NUMBER]={NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    double *epsilon[EPSILON_NUMBER]={NULL, NULL, NULL, NULL, NULL, NULL};
    int node_number, index_number;
    double deflection_min[DEFLECTION_NUMBER]={0,0,0,0,0,0}, deflection_max[DEFLECTION_NUMBER]={0,0,0,0,0,0};
    double stress_min[STRESS_NUMBER]={0,0,0,0,0,0, 0}, stress_max[STRESS_NUMBER]={0,0,0,0,0,0, 0};
    double epsilon_min[EPSILON_NUMBER]={0,0,0,0,0,0}, epsilon_max[EPSILON_NUMBER]={0,0,0,0,0,0};
    double deflection_amax, stress_amax, epsilon_amax;
    char deflection_chapter[DEFLECTION_NUMBER][MaxTextLen];
    char stress_chapter[STRESS_NUMBER][MaxTextLen];
    char epsilon_chapter[EPSILON_NUMBER][MaxTextLen];
    WIELOKAT w=Stdef;
    GRADIENT gradient;
    FE_DATA fe_data;
    char *fe_data_ptr;
    char *gradient_ptr;
    char *translucency_ptr;
    unsigned char HalfTranslucency=128;
    char *adr;
    TEXT T=Tdef;
    LINIA Lt;
    PLINIA PL;
    double psize;
    BOOL hiding;
    char load_formula[MaxTextLen];
    int block_names_no=0;
    int key1;
    double self_weight=0.0;

    int geometric_tiffness=0;
    int inertia=0;
    int st_dynamic_no=0;
    BOOL PINNABLE=TRUE;
    int combinations_number, combination_no;
    char SLS_ULS[12]="";
    BOOL was_refreshed;

    was_refreshed=FALSE;

    pl_node_no=0;
    pl_edge_no=0;
    pl_load_no=0;
    plate_no=0;
    hole_no=0;
    wall_no=0;
    zone_no=0;

    PL_PROPERTY_MAX=10;
    PL_LOAD_FACTORS_MAX=100;
    PL_NODE_MAX=100;
    PL_EDGE_MAX=100;
    PL_LOAD_MAX=100;
    PL_PLATE_MAX=10;
    PL_HOLE_MAX=10;
    PL_WALL_MAX=10;
    PL_ZONE_MAX=10;

    plate_no=hole_no=wall_no=zone_no=pl_load_no=0;

    pl_property_no = 0;
    pl_load_factors_no = 0;
    gZ=0.0;

    redcrsb(0, 171);
    select_blok();
    redcrsb(1, 171);

    if ((ADP == NULL) && (ADK == NULL)) return;

    //ret_standard = 1;   //TEMPORARY, for EU

    ClearErr();
    ClearInfo();

    ret_standard = ask_question_static(6, (char*)_No_, (char*)_Yes_, (char*)"", (char*)_PROCEED_PLATE_FEM_, 12, (char*)"", 11, 1, 0, &combination_no, &geometric_tiffness, &inertia, &st_dynamic_no, &PINNABLE);
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

    pl_property = (ST_PROPERTY *) malloc(PL_PROPERTY_MAX * sizeof(ST_PROPERTY) + 100);
    pl_load_factors = (ST_LOAD_FACTORS *) malloc(PL_LOAD_FACTORS_MAX * sizeof(ST_LOAD_FACTORS) + 100);
    pl_node = (PL_NODE *) malloc(PL_NODE_MAX * sizeof(PL_NODE) + 100);
    pl_edge = (PL_EDGE *) malloc(PL_EDGE_MAX * sizeof(PL_EDGE) + 100);
    pl_load = (PL_LOAD *) malloc(PL_LOAD_MAX * sizeof(PL_LOAD) + 100);

    plate_property=(PLATE_PROPERTY *) malloc(PL_PLATE_MAX * sizeof(PLATE_PROPERTY) + 100);
    hole_property=(PLATE_PROPERTY *) malloc(PL_HOLE_MAX * sizeof(PLATE_PROPERTY) + 100);
    wall_property=(PLATE_PROPERTY *) malloc(PL_WALL_MAX * sizeof(PLATE_PROPERTY) + 100);
    zone_property=(PLATE_PROPERTY *) malloc(PL_ZONE_MAX * sizeof(PLATE_PROPERTY) + 100);


    switch (ret_standard) {
        case 1:
            load_factors_pl = st_load_factors_EU_0;
            gammas = &gammas_EU;
            break;
        case 2:
            load_factors_pl = st_load_factors_ASCE_0;
            gammas = &gammas_ASCE;
            break;
        case 3:
            load_factors_pl = st_load_factors_ICC_0;
            gammas = &gammas_ICC;
            break;
        default:
            load_factors_pl = st_load_factors_EU_0;
            gammas = &gammas_EU;
            break;
    }

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
        UNITS=SI;
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
        UNITS=SI;
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
        UNITS=SI;
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
        UNITS=SI;
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
        UNITS=IMP;
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
        UNITS=IMP;
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
        UNITS=IMP;
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
        UNITS=IMP;
    } else {
        units_factor = 1.0;
        m_units_factor = 1000.0;
        geo_units_factor = 0.001;
        //E_units_factor=1.0;
        //h_units_factor=1.0;
        axis_increment = 10.0;
        unit_factors_pl = &unit_factors_pl_si;
        prop_precisions_pl = &SI_precisions;
        UNITS=SI;
    }

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
                    strncpy(st_title, ptr + 1, MaxTextLen*2 - 12);
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
                if (prop_precisions_pl == &SI_precisions) pl_property[pl_property_no].c=35; //[mm]
                else pl_property[pl_property_no].c=1.5; //[in]
                //st_property[st_property_no].ok = 0;

                //gravitational acceleration

                //gammas->gamma_g=st_load_factors_EU_0

                ptr = strstr(t->text, "%g=");
                if (ptr == NULL) ptr = strstr(t->text, "%G=");
                if (ptr != NULL) {
                    gZ = atof(ptr + 3);

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
                            &load_factors_pl[pl_load_factors[pl_load_factors_no].load],
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
                if (ptr != NULL) pl_property[pl_property_no].RC_flag = 1; //TEMPORARY: assumption that it's rectangular cross section

                ptr = strstr(t->text, "h=");
                if (ptr == NULL) break;
                pl_property[pl_property_no].h = atof(ptr + 2) * unit_factors_pl->h_f;  //for RC in mm or in, same as steel and wood

                pl_property[pl_property_no].b = pl_property[pl_property_no].h;

                ptr = strstr(t->text, "b=");
                if (ptr != NULL)
                {
                    pl_property[pl_property_no].b = atof(ptr + 2) * unit_factors_pl->h_f;  //for RC in mm or in, same as steel and wood
                    if (pl_property[pl_property_no].RC_flag == 1) //it's RC and b is set
                    {
                        pl_property[pl_property_no].A = pl_property[pl_property_no].h * pl_property[pl_property_no].b; //Cross-sectional area in mm^2
                        pl_property[pl_property_no].Asy = pl_property[pl_property_no].A; //Shear area in the local y-axis
                        pl_property[pl_property_no].Asz = pl_property[pl_property_no].A; //Shear area in the local z-axis
                        pl_property[pl_property_no].Iy =  pl_property[pl_property_no].h * pow(pl_property[pl_property_no].b, 3) / 12; //Moment of inertia for bending about the local y axis in mm^4  or in^4:  h*b^3/12
                        pl_property[pl_property_no].Iz =  pow(pl_property[pl_property_no].h,3) * pl_property[pl_property_no].b / 12; //Moment of inertia for bending about the local z axis in mm^4  or in^4:  h^3*b/12
                        pl_property[pl_property_no].Wy =  pl_property[pl_property_no].h * pow(pl_property[pl_property_no].b, 2) * unit_factors_pl->Wmm_f / 6; //Elastic section modulus about y-axis in m^3  or in^3:  h*b^2/6
                        pl_property[pl_property_no].Wz =  pow(pl_property[pl_property_no].h,2) * pl_property[pl_property_no].b * unit_factors_pl->Wmm_f / 6; //Elastic section modulus about z-axis  in m^3 or in^3:  h^2*b/6
                        pl_property[pl_property_no].Jx =  ((pl_property[pl_property_no].h * pl_property[pl_property_no].b)/12)*(pow(pl_property[pl_property_no].h, 2)+ pow(pl_property[pl_property_no].b, 2)); //Torsional moment of inertia  h*b/12*(h^2+b^2)
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
                    pl_property[pl_property_no].c = atof(ptr + 2); // * unit_factors_pl->c_f;
                }

                ptr = strstr(t->text, "ζ=");  //zeta, default 0.85
                if (ptr != NULL) {
                    pl_property[pl_property_no].zeta = atof(ptr + 3);
                }

                ptr = strstr(t->text, "fyd=");  //no default, in Mpa or kpsi
                if (ptr != NULL) {
                    pl_property[pl_property_no].fyd = atof(ptr + 4); // * unit_factors_pl->E_f;
                }
                else if (pl_property[pl_property_no].RC_flag == 1) break;

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

    body_number=0;
    ////searching for plates
    obiekt_tok((char *) ADP, ADK, (char **) &nag, OdBLOK);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok))
        {
            b = (BLOK *) nag;
            if (b->kod_obiektu==B_PLINE)
            {
                if (b->opis_obiektu [0] == PL_PLATE) {
                    pl_min_x=99999.;
                    pl_min_y=99999.;
                    pl_max_x=-99999.;
                    pl_max_y=-99999.;

                    if (create_plate(b, 0, plate_no, body_number, &first, &last, &property_number, &is_closed))
                    {
                        if (is_closed) {
                            plate_property[plate_no].adr = b;
                            plate_property[plate_no].property_number = property_number;
                            plate_property[plate_no].first_edge = first;
                            plate_property[plate_no].last_edge = last;
                            add_plate_property();
                            body_number++;
                        }
                        else
                        {
                            sprintf(report_row, "%s %d %s\n",_THE_PLATE_, plate_no+1, _POLYLINE_IS_NOT_CLOSED_);
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
        if (TRUE == Check_Attribute(nag->atrybut, Ablok))
        {
            b = (BLOK *) nag;
            if (b->kod_obiektu==B_PLINE)
            {
                if (b->opis_obiektu [0] == PL_HOLE) {
                    if (create_plate(b, 1, hole_no, body_number, &first, &last, &property_number, &is_closed))
                    {
                        if (is_closed) {
                            hole_property[hole_no].adr = b;
                            hole_property[hole_no].property_number = property_number;
                            hole_property[hole_no].first_edge = first;
                            hole_property[hole_no].last_edge = last;
                            add_hole_property();
                            body_number++;
                        }
                        else
                        {
                            sprintf(report_row, "%s %d %s\n",_THE_HOLE_, hole_no+1, _POLYLINE_IS_NOT_CLOSED_);
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
        if (TRUE == Check_Attribute(nag->atrybut, Ablok))
        {
            b = (BLOK *) nag;
            if (b->kod_obiektu==B_PLINE)
            {
                if (b->opis_obiektu [0] == PL_WALL) {
                    if (create_plate(b, 2, wall_no, body_number, &first, &last, &property_number, &is_closed))
                    {
                        if (is_closed) {
                            wall_property[wall_no].adr = b;
                            wall_property[wall_no].property_number = property_number;
                            wall_property[wall_no].first_edge = first;
                            wall_property[wall_no].last_edge = last;
                            add_wall_property();
                            body_number++;
                        }
                        else
                        {
                            sprintf(report_row, "%s %d %s\n",_THE_WALL_, wall_no+1, _POLYLINE_IS_NOT_CLOSED_);
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
        if (TRUE == Check_Attribute(nag->atrybut, Ablok))
        {
            b = (BLOK *) nag;
            if (b->kod_obiektu==B_PLINE)
            {
                if (b->opis_obiektu [0] == PL_ZONE) {
                    if (create_plate(b, 3, zone_no, body_number, &first, &last, &property_number, &is_closed))
                    {
                        if (is_closed) {
                            zone_property[zone_no].adr = b;
                            zone_property[zone_no].property_number = property_number;
                            zone_property[zone_no].first_edge = first;
                            zone_property[zone_no].last_edge = last;
                            add_zone_property();
                            body_number++;
                        }
                        else
                        {
                            sprintf(report_row, "%s %d %s\n",_THE_ZONE_, zone_no+1, _POLYLINE_IS_NOT_CLOSED_);
                            strcat(report, report_row);
                        }
                    }
                }
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, OdBLOK);
    }

    ////counting objects
    if (plate_no==0)  //no plate
    {
        sprintf(report_row, "%s\n",_THE_PLATE_POLYLINE_NUMBER_IS_EQUAL_ZERO_);
        strcat(report, report_row);
        goto pl_error;
    }

    ////searching for load
    obiekt_tok((char *) ADP, ADK, (char **) &nag, Ovector);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok))
        {
            v = (AVECTOR *) nag;
            if (v->style==17) {
                pl_load[pl_load_no].adr=v;
                pl_load[pl_load_no].x1=v->x1;
                pl_load[pl_load_no].y1=v->y1;
                pl_load[pl_load_no].x2=v->x2;
                pl_load[pl_load_no].y2=v->y2;
                pl_load[pl_load_no].magnitude1=v->magnitude1;
                pl_load[pl_load_no].magnitude2=v->magnitude2;
                pl_load[pl_load_no].load=v->load;
                pl_load[pl_load_no].variant=v->variant;
                pl_load[pl_load_no].body=-1;  //not yet assigned
                pl_load[pl_load_no].body_no=-1;  //not yet assigned
                add_load_pl();
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, Ovector);
    }

    ///removing Ablok flags to assign load to bodies
    zmien_atrybut_undo(dane, dane + dane_size);
    blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
    zmien_atrybut(ADP, ADK, Ablok, Aoblok);
    was_refreshed=TRUE;

    ////LOAD assignement
    for (i=0; i<pl_load_no; i++)
    {
        df_x1=pl_load[i].x1;
        df_y1=pl_load[i].y1;
        df_x2=pl_load[i].x2;
        df_y2=pl_load[i].y2;
        ////marking zone
        for (j=0; j<zone_no; j++)
        {
            ADPB=(char*)zone_property[j].adr;
            ADKB=ADPB+sizeof(NAGLOWEK)+zone_property[j].adr->n-1;
            zmien_atrybut(ADPB, ADKB, Aoblok, Ablok);
            ret1 = hatch_proc_test((long_long)(ADPB-dane), (long_long)(ADKB-dane), df_x1, df_y1, &s_hatch_param, 1, 0, 0, 0, 0);
            ret2 = hatch_proc_test((long_long)(ADPB-dane), (long_long)(ADKB-dane), df_x2, df_y2, &s_hatch_param, 1, 0, 0, 0, 0);
            zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
            if ((ret1==1) && (ret2==1))
            {
                pl_load[i].body=1; //zone
                pl_load[i].body_no=j;
                break;
            }
        }
        //if assigned to zone, won't be assigned to plate
        if (j==zone_no)  //not found in zones
        {
            ////marking plate
            for (j=0; j<plate_no; j++)
            {
                ADPB=(char*)plate_property[j].adr;
                ADKB=ADPB+sizeof(NAGLOWEK)+plate_property[j].adr->n-1;
                zmien_atrybut(ADPB, ADKB, Aoblok, Ablok);
                ret1 = hatch_proc_test((long_long)(ADPB-dane), (long_long)(ADKB-dane), df_x1, df_y1, &s_hatch_param, 1, 0, 0, 0, 0);
                ret2 = hatch_proc_test((long_long)(ADPB-dane), (long_long)(ADKB-dane), df_x2, df_y2, &s_hatch_param, 1, 0, 0, 0, 0);
                zmien_atrybut(ADPB, ADKB, Ablok, Aoblok);
                if ((ret1==1) && (ret2==1))
                {
                    pl_load[i].body=0; //plate
                    pl_load[i].body_no=j;
                    break;
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

    if ((pl_load_no==0) && (gZ==0)) //no load, no self weight
    {
        sprintf(report_row, "%s\n",_NO_LOAD_ASSIGNED_);
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
            sprintf(report_row, "%s %s",_cannot_create_folder_, _STATIC_);
            strcat(report, report_row);
        }
    }

    sprintf(params, "%s%s", _STATIC_,_plate_);
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
            sprintf(report_row, "%s %s%s",_cannot_create_folder_, _STATIC_,_plate_);
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
            sprintf(report_row, "%s %s",_cannot_create_folder_, _plate_);
            strcat(report, report_row);
        }
    }

    ////deleting old files
    sprintf(params, "%splate.geo", _STATIC_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%s%splate.sif", _STATIC_,_plate_);
    if (my_file_exists(params)) unlink(params);
    ////results
    sprintf(params, "%s%splate.result", _STATIC_,_plate_);
    if (my_file_exists(params)) unlink(params);
    ////results in plate folder due to ElmerSolver error
    sprintf(params, "%splate.result", _plate_);
    if (my_file_exists(params)) unlink(params);
    ////mesh
    sprintf(params, "%s%smesh.nodes", _STATIC_,_plate_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%s%smesh.header", _STATIC_,_plate_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%s%smesh.elements", _STATIC_,_plate_);
    if (my_file_exists(params)) unlink(params);
    sprintf(params, "%s%smesh.boundary", _STATIC_,_plate_);
    if (my_file_exists(params)) unlink(params);
    ////

    //creating geo file
    sprintf(params, "%splate.geo", _STATIC_);
    f = fopen(params, "wt");

    if (f == NULL) {
        ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_RESULTS_FILE_, 12, (char*)"", 11, 1,
                           62);
        no_error=FALSE;
        goto pl_error;
    }

    fprintf(f, "\n// NODES\n");
    //nodes
    for (i=0; i<pl_node_no; i++)
    {
        set_decimal_format(par[0], milimetryobx(pl_node[i].x) * geo_units_factor, dim_precision_pl);
        set_decimal_format(par[1], milimetryoby(pl_node[i].y) * geo_units_factor, dim_precision_pl);
        set_decimal_format(par[2], max(milimetryob((double)pl_node[i].d) * geo_units_factor, dxl_min), dim_precision_pl);

        fprintf(f, "Point(%d) = {%s, %s, 0, %s};\n", i+1, par[0], par[1], par[2]);
    }
    //edges
    fprintf(f, "\n// EDGES\n");
    k=0;
    //the first are plates
    fprintf(f, "\n// PLATES\n");
    for (i=0; i<plate_no; i++)
    {
        for (j=plate_property[i].first_edge; j<plate_property[i].last_edge; j++)
        {
            if (pl_edge[j].type==0)  //line
            {
                fprintf(f, "Line(%d) = {%d, %d};\n", k + 1, pl_edge[j].node1+1, pl_edge[j].node2+1);
                pl_edge[j].k=k;
            }
            else if (pl_edge[j].type==1)  //circle
            {
                fprintf(f, "Circle(%d) = {%d, %d, %d};\n", k + 1, pl_edge[j].node1+1, pl_edge[j].node2+1, pl_edge[j].node3+1);
                pl_edge[j].k=k;
            }
            k++;
        }
    }
    //the second are holes
    fprintf(f, "\n// HOLES\n");
    for (i=0; i<hole_no; i++)
    {
        for (j=hole_property[i].first_edge; j<hole_property[i].last_edge; j++)
        {
            if (pl_edge[j].type==0)  //line
            {
                fprintf(f, "Line(%d) = {%d, %d};\n", k + 1, pl_edge[j].node1+1, pl_edge[j].node2+1);
                pl_edge[j].k=k;
            }
            else if (pl_edge[j].type==1)  //circle
            {
                fprintf(f, "Circle(%d) = {%d, %d, %d};\n", k + 1, pl_edge[j].node1+1, pl_edge[j].node2+1, pl_edge[j].node3+1);
                pl_edge[j].k=k;
            }
            k++;
        }
    }

    //the third are walls
    fprintf(f, "\n// WALLS\n");
    for (i=0; i<wall_no; i++)
    {
        for (j=wall_property[i].first_edge; j<wall_property[i].last_edge; j++)
        {
            if (pl_edge[j].type==0)  //line
            {
                fprintf(f, "Line(%d) = {%d, %d};\n", k + 1, pl_edge[j].node1+1, pl_edge[j].node2+1);
                pl_edge[j].k=k;
            }
            else if (pl_edge[j].type==1)  //circle
            {
                fprintf(f, "Circle(%d) = {%d, %d, %d};\n", k + 1, pl_edge[j].node1+1, pl_edge[j].node2+1, pl_edge[j].node3+1);
                pl_edge[j].k=k;
            }
            k++;
        }
    }

    //the last are zones
    fprintf(f, "\n// ZONES\n");
    for (i=0; i<zone_no; i++)
    {
        for (j=zone_property[i].first_edge; j<zone_property[i].last_edge; j++)
        {
            if (pl_edge[j].type==0)  //line
            {
                fprintf(f, "Line(%d) = {%d, %d};\n", k + 1, pl_edge[j].node1+1, pl_edge[j].node2+1);
                pl_edge[j].k=k;
            }
            else if (pl_edge[j].type==1)  //circle
            {
                fprintf(f, "Circle(%d) = {%d, %d, %d};\n", k + 1, pl_edge[j].node1+1, pl_edge[j].node2+1, pl_edge[j].node3+1);
                pl_edge[j].k=k;
            }
            k++;
        }
    }

    //curves
    fprintf(f, "\n// CURVES\n");
    //the first are holes
    fprintf(f, "\n// HOLES\n");
    for (i=0; i<hole_no; i++)
    {
        fprintf(f, "Curve Loop (%d) = {",k+1);

        for (j=hole_property[i].first_edge; j<hole_property[i].last_edge; j++)
        {
            if (pl_edge[j].inverted==0) fprintf(f, "%d", pl_edge[j].k+1);
            else fprintf(f, "-%d", pl_edge[j].k+1);
            if (j<(hole_property[i].last_edge-1)) fprintf(f, ", ");
        }
        fprintf(f, "};\n");
        hole_property[i].k=k;
        k++;
    }

    //the second are walls
    fprintf(f, "\n// WALLS\n");
    for (i=0; i<wall_no; i++)
    {
        fprintf(f, "Curve Loop (%d) = {",k+1);

        for (j=wall_property[i].first_edge; j<wall_property[i].last_edge; j++)
        {
            if (pl_edge[j].inverted==0) fprintf(f, "%d", pl_edge[j].k+1);
            else fprintf(f, "-%d", pl_edge[j].k+1);
            if (j<(wall_property[i].last_edge-1)) fprintf(f, ", ");
        }
        fprintf(f, "};\n");
        wall_property[i].k=k;
        k++;
        fprintf(f, "Plane Surface(%d) = {%d};\n", k+1, k);
        k++;
    }

    //the third are zones
    fprintf(f, "\n// ZONES\n");
    for (i=0; i<zone_no; i++)
    {
        fprintf(f, "Curve Loop (%d) = {",k+1);

        for (j=zone_property[i].first_edge; j<zone_property[i].last_edge; j++)
        {
            if (pl_edge[j].inverted==0) fprintf(f, "%d", pl_edge[j].k+1);
            else fprintf(f, "-%d", pl_edge[j].k+1);
            if (j<(zone_property[i].last_edge-1)) fprintf(f, ", ");
        }
        fprintf(f, "};\n");
        zone_property[i].k=k;
        k++;
        fprintf(f, "Plane Surface(%d) = {%d};\n", k+1, k);
        k++;
    }

    //the last are plates
    //the third are zones
    fprintf(f, "\n// PLATES\n");
    for (i=0; i<plate_no; i++)
    {
        fprintf(f, "Curve Loop (%d) = {",k+1);

        for (j=plate_property[i].first_edge; j<plate_property[i].last_edge; j++)
        {
            if (pl_edge[j].inverted==0) fprintf(f, "%d", pl_edge[j].k+1);
            else fprintf(f, "-%d", pl_edge[j].k+1);
            if (j<(plate_property[i].last_edge-1)) fprintf(f, ", ");
        }
        fprintf(f, "};\n");
        plate_property[i].k=k;
        k++;
        fprintf(f, "Plane Surface(%d) = {%d",k+1, k);
        for (j=0; j<hole_no; j++)
            fprintf(f, ", %d", hole_property[j].k+1);
        for (j=0; j<wall_no; j++)
            fprintf(f, ", %d", wall_property[j].k+1);
        for (j=0; j<zone_no; j++)
            fprintf(f, ", %d", zone_property[j].k+1);
        fprintf(f, "};\n");
        k++;
    }

    fprintf(f, "\n// THE END\n");

    fclose(f);

    //creating geo file
    sprintf(params, "%s%splate.sif", _STATIC_,_plate_);
    f = fopen(params, "wt");

    if (f == NULL) {
        ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_RESULTS_FILE_, 12, (char*)"", 11, 1,
                           62);
        no_error=FALSE;
        goto pl_error;
    }

    fprintf(f, "Header\n");
    fprintf(f, "  CHECK KEYWORDS Warn\n");
    fprintf(f, "  Mesh DB \"%s\" \"%s\"\n","Static","plate");
    fprintf(f, "  Include Path \"\"\n");
    fprintf(f, "  Results Directory \"%s%s\"\n",_STATIC_,"plate");
    fprintf(f, "End\n\n");

    fprintf(f, "Simulation\n");
    fprintf(f, "  Max Output Level = 5\n");
    fprintf(f, "  Coordinate System = Cartesian\n");
    fprintf(f, "  Coordinate Mapping(3) = 1 2 3\n");
    fprintf(f, "  Simulation Type = Steady state\n");
    fprintf(f, "  Steady State Max Iterations = 1\n");
    fprintf(f, "  Output Intervals(1) = 1\n");
    fprintf(f, "  Solver Input File = plate.sif\n");
    fprintf(f, "  Output File = \"plate.result\"\n");
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
    fprintf(f, "  Equation = Shell Solver\n");
    fprintf(f, "  Procedure = \"ShellMultiSolver\" \"ShellSolver\"\n");
    //fprintf(f, "  Procedure = \"ShellSolver\" \"ShellSolver\"\n");
    fprintf(f, "  Linear System Solver = Direct\n");
    fprintf(f, "  !Linear System Max Iterations = 10000\n");
    fprintf(f, "  !Linear System Iterative Method = BiCGStab\n");
    fprintf(f, "  !Linear System Preconditioning = ILU0\n");
    fprintf(f, "  Linear System Convergence Tolerance = 1.0e-10\n");
    fprintf(f, "  Nonlinear System Newton After Tolerance = 1.0e-8\n");
    fprintf(f, "  Nonlinear System Newton After Iterations = 20\n");
    fprintf(f, "  Nonlinear System Max Iterations = 1\n");
    fprintf(f, "  Nonlinear System Convergence Tolerance = 1.0e-8\n");
    fprintf(f, "  Nonlinear System Relaxation Factor = 1.0\n");
    fprintf(f, "  Steady State Convergence Tolerance = 1.0e-8\n");
    fprintf(f, "\n");
    fprintf(f, "  Shear Stabilization Parameter = Real 0.0\n");
    fprintf(f, "  Drilling Stabilization Parameter = Real 1.0\n");
    fprintf(f, "  Calculate Stresses = Logical TRUE\n");
    fprintf(f, "  !Compute Membrane Stress = Logical TRUE\n");
    fprintf(f, "  !Compute Bending Stress = Logical TRUE\n");
    fprintf(f, "  Compute Total Stress = Logical TRUE\n");
    fprintf(f, "  Bottom Side Stress = Logical TRUE\n");
    fprintf(f, "\n");
    fprintf(f, "  Membrane Only = Logical FALSE\n");
    fprintf(f, "  Use DKT Triangle = Logical FALSE\n");
    fprintf(f, "  Use RMITC3 Element = Logical FALSE\n");
    fprintf(f, "  Use SMITC Element = Logical TRUE\n");
    fprintf(f, "  Use MITC3 Element = Logical FALSE\n");
    fprintf(f, "  Integration Points = Integer 3\n");
    fprintf(f, "End\n\n");

    sif_body=0;

    for (i=0; i<zone_no; i++)
    {
        fprintf(f, "Body %d\n", sif_body+1);
        fprintf(f, "  Target Bodies(1) = %d\n",zone_property[i].k+1);
        fprintf(f, "  Name = \"Body Property %d\"\n",sif_body+1);
        fprintf(f, "  Equation = 1\n");
        fprintf(f, "  Material = %d\n",zone_property[i].property_number);
        fprintf(f, "  Body Force = %d\n", sif_body+1); //TO DO
        fprintf(f, "End\n\n");
        sif_body++;
    }

    for (i=0; i<plate_no; i++)
    {
        fprintf(f, "Body %d\n", sif_body+1);
        fprintf(f, "  Target Bodies(1) = %d\n",plate_property[i].k+1);
        fprintf(f, "  Name = \"Body Property %d\"\n",sif_body+1);
        fprintf(f, "  Equation = 1\n");
        fprintf(f, "  Material = %d\n",plate_property[i].property_number);
        fprintf(f, "  Body Force = %d\n", sif_body+1); //TO DO
        fprintf(f, "End\n\n");
        sif_body++;
    }

    fprintf(f, "Equation 1\n");
    fprintf(f, "  Name = \"Equation 1\"\n");
    fprintf(f, "  Active Solvers(1) = 1\n");
    fprintf(f, "End\n\n");

    sif_material=0;
    for (i=0; i<zone_no; i++)
    {
        //searching for properties
        for (j=0; j<pl_property_no; j++)
        {
            if (pl_property[j].n==zone_property[i].property_number)
            {
                this_property=j;
                break;
            }
        }
        fprintf(f, "Material %d\n", sif_material+1);
        fprintf(f, "  Name = \"Material %d\"\n", sif_material+1);
        fprintf(f, "  Youngs modulus = %g\n", pl_property[this_property].E);
        fprintf(f, "  Density = %g\n", pl_property[this_property].d);
        fprintf(f, "  Poisson ratio = %f\n", (pl_property[this_property].E / (2.*pl_property[this_property].G)) - 1);
        set_decimal_format(par[1], pl_property[this_property].h, dim_precision_pl);
        fprintf(f, "  Thickness = %s\n", par[1]);
        fprintf(f, "End\n\n");
        sif_material++;
    }

    for (i=0; i<plate_no; i++)
    {
        //searching for properties
        for (j=0; j<pl_property_no; j++)
        {
            if (pl_property[j].n==plate_property[i].property_number)
            {
                this_property=j;
                break;
            }
        }
        fprintf(f, "Material %d\n", sif_material+1);
        fprintf(f, "  Name = \"Material %d\"\n", sif_material+1);
        fprintf(f, "  Youngs modulus = %g\n", pl_property[this_property].E);
        fprintf(f, "  Density = %g\n", pl_property[this_property].d);
        fprintf(f, "  Poisson ratio = %f\n", (pl_property[this_property].E / (2.*pl_property[this_property].G)) -1.);
        set_decimal_format(par[1], pl_property[this_property].h, dim_precision_pl);
        fprintf(f, "  Thickness = %s\n", par[1]);
        fprintf(f, "End\n\n");
        sif_material++;
    }

    sif_body_force=0;

    for (i=0; i<zone_no; i++)
    {
        //searching for properties
        for (j=0; j<pl_property_no; j++)
        {
            if (pl_property[j].n==zone_property[i].property_number)
            {
                this_property=j;
                break;
            }
        }
        fprintf(f, "Body Force %d\n", sif_body_force+1);
        fprintf(f, "  Name = \"BodyForce %d\"\n",sif_body_force+1);
        strcpy(load_formula,"");

        self_weight=0.0;
        self_weight=gZ * pl_property[this_property].d * pl_property[this_property].h;
        if (self_weight>0.0)
        {
            set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
            strcat(load_formula, par[1]);
        }

        for (j=0; j<pl_load_no; j++)
        {
            if ((pl_load[j].body==1) && (pl_load[j].body_no==i)) //zone and number
            {
                set_decimal_format(par[1], -pl_load[j].magnitude1*unit_factors_pl->q_f, load_precision);  //is assumed that magnitude1=magnitude2
                if (strlen(load_formula)>0) strcat(load_formula, " + ");
                strcat(load_formula, "(");
                strcat(load_formula, par[1]);
                strcat(load_formula, ")");
            }
        }
        if (strlen(load_formula)>0) fprintf(f, "  Normal Pressure = Real MATC \"%s\"\n", load_formula);

        //fprintf(f, "  Normal Pressure = -5000\n");
        set_decimal_format(par[1], pl_property[this_property].h, dim_precision_pl);
        fprintf(f, "  Thickness = Real %s\n", par[1]);
        fprintf(f, "End\n\n");
        sif_body_force++;
    }

    for (i=0; i<plate_no; i++)
    {
        //searching for properties
        for (j=0; j<pl_property_no; j++)
        {
            if (pl_property[j].n==plate_property[i].property_number)
            {
                this_property=j;
                break;
            }
        }
        fprintf(f, "Body Force %d\n", sif_body_force+1);
        fprintf(f, "  Name = \"BodyForce %d\"\n",sif_body_force+1);

        strcpy(load_formula,"");

        self_weight=0.0;
        self_weight=gZ * pl_property[this_property].d * pl_property[this_property].h;
        if (self_weight>0.0)
        {
            set_decimal_format(par[1], -self_weight, load_precision);  //is assumed that magnitude1=magnitude2
            strcat(load_formula, par[1]);
        }

        for (j=0; j<pl_load_no; j++)
        {
            if ((pl_load[j].body==0) && (pl_load[j].body_no==i)) //plate and number
            {
                set_decimal_format(par[1], -pl_load[j].magnitude1*unit_factors_pl->q_f, load_precision);  //is assumed that magnitude1=magnitude2
                //fprintf(f, "  Normal Pressure = %s\n", par[1]);
                if (strlen(load_formula)>0) strcat(load_formula, " + ");
                strcat(load_formula, "(");
                strcat(load_formula, par[1]);
                strcat(load_formula, ")");
            }
        }
        if (strlen(load_formula)>0) fprintf(f, "  Normal Pressure = Real MATC \"%s\"\n", load_formula);

        //fprintf(f, "  Normal Pressure = -5000\n");
        set_decimal_format(par[1], pl_property[this_property].h, dim_precision_pl);
        fprintf(f, "  Thickness = Real %s\n", par[1]);
        fprintf(f, "End\n\n");
        sif_body_force++;
    }

    sif_boundary_condition=0;
    sif_boundary_condition_simple=0;
    sif_boundary_condition_fixed=0;

    //simply supported edges
    for (i=0; i<pl_edge_no; i++) {
          if (pl_edge[i].restraint==6) sif_boundary_condition_simple++;
          else if (pl_edge[i].restraint==7) sif_boundary_condition_fixed++;
    }

    if (sif_boundary_condition_simple>0)
    {
        fprintf(f, "Boundary Condition %d\n", sif_boundary_condition+1);
        sprintf(par[1],"  Target Boundaries(%d) =",sif_boundary_condition_simple);
        for (i=0; i<pl_edge_no; i++) {
            if (pl_edge[i].restraint == 6)
            {
                sprintf(par[2], " %d", pl_edge[i].k+1);
                strcat(par[1],par[2]);
            }
        }
        fprintf(f, "%s\n", par[1]);
        fprintf(f, "  Name = \"Boundary Condition %d\"\n",sif_boundary_condition+1);
        fprintf(f, "  Displacement 1 = 0\n");
        fprintf(f, "  Displacement 2 = 0\n");
        fprintf(f, "  Displacement 3 = 0\n");
        fprintf(f, "  Deflection 1 = 0\n");
        fprintf(f, "  Deflection 2 = 0\n");
        fprintf(f, "  Deflection 3 = 0\n");
        fprintf(f, "  u 1 = 0\n");
        fprintf(f, "  u 2 = 0\n");
        fprintf(f, "  u 3 = 0\n");
        fprintf(f, "End\n");

        sif_boundary_condition++;
    }

    if (sif_boundary_condition_fixed>0)
    {
        fprintf(f, "Boundary Condition %d\n", sif_boundary_condition+1);
        sprintf(par[1],"  Target Boundaries(%d) =",sif_boundary_condition_fixed);
        for (i=0; i<pl_edge_no; i++) {
            if (pl_edge[i].restraint == 7)
            {
                sprintf(par[2], " %d", pl_edge[i].k+1);
                strcat(par[1],par[2]);
            }
        }
        fprintf(f, "%s\n", par[1]);
        fprintf(f, "  Name = \"Boundary Condition %d\"\n",sif_boundary_condition+1);
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
        fprintf(f, "End\n");

        sif_boundary_condition++;
    }

    fclose(f);

    ClearErr();
    ClearInfo();

    sprintf(params, "%splate.geo", _STATIC_);
    if (my_file_exists(params))
    {
        //generating msh
        //  gmsh Static/plate.geo -o Static/plate.msh -2
        sprintf(params, "%splate.geo -o %splate.msh -2", _STATIC_, _STATIC_);
        sprintf(program,"%sgmsh", _ELMER_);
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
        if ((runcode_short!=0) || (!my_file_exists(params)))
        {
            ret = ask_question(1, (char*)"", (char*)"OK", (char*)"", (char*)"gmsh", 12, (char*)_gmsh_error_, 11, 1, 0);
            goto pl_error;
        }

        if (my_file_exists(params))
        {
            //generating mesh
            // ElmerGrid 14 2 Static/plate.msh -out Static/plate
            sprintf(params, "14 2 %splate.msh -out %splate", _STATIC_,_STATIC_);
            sprintf(program,"%s%sElmerGrid", _ELMER_, _BIN_);
            //execute ElmerGrid
#ifdef LINUX
            runcode = SystemSilent(program, params);
            runcode_short = runcode >> 8;
#else
            runcode = RunSilent(program, params);
            runcode_short = runcode >> 8;
#endif
            printf("\nElmerGrid runcode:%lu runcode_short:%d\n", runcode, runcode_short);
            sprintf(params, "%s%smesh.header", _STATIC_,_plate_);
            if ((runcode_short!=0) || (!my_file_exists(params)))
            {
                ret = ask_question(1, (char*)"", (char*)"OK", (char*)"", (char*)"ElmerGrid", 12, (char*)_ElmerGrid_error_, 11, 1, 0);
                goto pl_error;
            }

            if (my_file_exists(params))
            {
                sprintf(params, "%s%splate.sif", _STATIC_,_plate_);
                //execute ElmerSolver, it will save results in plate folder
#ifdef LINUX
                sprintf(program,"%s%sElmerSolver", _ELMER_, _BIN_);
                runcode = SystemSilent(program, params);
                runcode_short = runcode >> 8;
#else
                sprintf(program,"%s%sElmerSolver.exe", _ELMER_, _BIN_);
                runcode = RunSilent(program, params);
                runcode_short = runcode >> 8;
#endif
                printf("\nElmerSolver runcode:%lu runcode_short:%d\n", runcode, runcode_short);
                sprintf(params, "%splate.result",_plate_);
                if ((runcode_short!=0) || (!my_file_exists(params)))
                {
                    ret = ask_question(1, (char*)"", (char*)"OK", (char*)"", (char*)"ElmerSolver", 12, (char*)_ElmerSolver_error_, 11, 1, 0);
                    goto pl_error;
                }

            }
        }
    }

    //getting geometry
    //mesh.header
    //5079   9894   592   //number of nodes in mesh.nodes   number of elements in mesh.elements  number of boundaries in mesh.baunday
    //3
    //101    26
    //202    566
    //303    9894
    //nodes -
    sprintf(params, "%s%smesh.header", _STATIC_,_plate_);
    f = fopen(params, "rt");

    if (f == NULL) {
        ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_OPEN_RESULTS_FILE_, 12, (char*)"mesh.header", 11, 1,
                           62);
        no_error=FALSE;
        goto pl_error;
    }

    i_row=0;
    data_row=data_row_[i_row];

    line=fgets(data_row, MaxTextLen, f);
    ret = sscanf(data_row, "%d %d %d", &mesh_nodes_no, &mesh_elements_no, &mesh_boundaries_no);
    while (fgets(data_row, MaxTextLen, f)) //another rows
    {
        ;
    }
    fclose(f);

    mesh_node_no=0;
    mesh_element_no=0;
    mesh_boundary_no=0;

    mesh_node = (MESH_NODE *) malloc(mesh_nodes_no * sizeof(MESH_NODE) + 100);
    mesh_element = (MESH_ELEMENT *) malloc(mesh_elements_no * sizeof(MESH_ELEMENT) + 100);
    mesh_boundary = (MESH_BOUNDARY *) malloc(mesh_boundaries_no * sizeof(MESH_BOUNDARY) + 100);

    sprintf(params, "%s%smesh.nodes", _STATIC_,_plate_);
    f = fopen(params, "rt");

    if (f == NULL) {
        ret = ask_question(1, (char*)"", (char *) confirm,(char*)"", (char *) _CANNOT_OPEN_RESULTS_FILE_, 12, (char*)"mesh.nodes", 11, 1,
                           62);
        no_error=FALSE;
        goto pl_error;
    }

    while (fgets(data_row, MaxTextLen, f)) //another rows
    {
        ret = sscanf(data_row, "%d %d %lf %lf %lf", &mesh_node[mesh_node_no].number, &mesh_node[mesh_node_no].flag, &mesh_node[mesh_node_no].x, &mesh_node[mesh_node_no].y, &mesh_node_z);
        if (ret!=5) {
            ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_READ_RESULTS_FILE_, 12, (char*)"hesh.nodes", 11,
                               1,
                               62);
            no_error = FALSE;
            fclose(f);
            goto pl_error1;
        }
        mesh_node_no++;
    }
    fclose(f);

    sprintf(params, "%s%smesh.elements", _STATIC_,_plate_);
    f = fopen(params, "rt");

    if (f == NULL) {
        ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_OPEN_RESULTS_FILE_, 12, (char*)"mesh.elements", 11, 1,
                           62);
        no_error=FALSE;
        goto pl_error;
    }

    while (fgets(data_row, MaxTextLen, f)) //another rows
    {
        ret = sscanf(data_row, "%d %d %d %d %d %d %d", &mesh_element[mesh_element_no].number, &mesh_element[mesh_element_no].body_number, &mesh_element[mesh_element_no].type, &mesh_element[mesh_element_no].node1, &mesh_element[mesh_element_no].node2, &mesh_element[mesh_element_no].node3, &mesh_element[mesh_element_no].node4);
        if (mesh_element[mesh_element_no].type==303) ret_exp=6; else ret_exp=7;
        if (ret!=ret_exp) {
            ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_READ_RESULTS_FILE_, 12, (char*)"mesh.elements", 11,
                               1,
                               62);
            no_error = FALSE;
            fclose(f);
            goto pl_error1;
        }
        mesh_element_no++;
    }
    fclose(f);

    sprintf(params, "%s%smesh.boundary", _STATIC_,_plate_);
    f = fopen(params, "rt");

    if (f == NULL) {
        ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_OPEN_RESULTS_FILE_, 12, (char*)"mesh.boundary", 11, 1,
                           62);
        no_error=FALSE;
        goto pl_error;
    }

    while (fgets(data_row, MaxTextLen, f)) //another rows
    {
        ret = sscanf(data_row, "%d %d %d %d %d %d %d", &mesh_boundary[mesh_boundary_no].n1, &mesh_boundary[mesh_boundary_no].n2, &mesh_boundary[mesh_boundary_no].n3, &mesh_boundary[mesh_boundary_no].n4, &mesh_boundary[mesh_boundary_no].n5, &mesh_boundary[mesh_boundary_no].n6, &mesh_boundary[mesh_boundary_no].n7);
        //if (mesh_boundary[mesh_boundary_no].n5==202) ret_exp=7; else ret_exp=6;
        //if (ret!=ret_exp)
        if (ret<6)
        {
            ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_READ_RESULTS_FILE_, 12, (char*)"mesh.boundary", 11,
                               1,
                               62);
            no_error = FALSE;
            fclose(f);
            goto pl_error1;
        }
        mesh_boundary_no++;
    }
    fclose(f);

    //creating geometry block
    Ldsp.blok = 1;
    Ldsp.kolor = static_colors.node_element_color;
    Ldsp.typ=0;  //thinest
    /////////////////////
    //Layers
    sprintf(desired_layer, "%s_%s", ptr_id, "geometry");

    for (ii = 0; ii < No_Layers; ii++) {
        if (strcmp(Layers[ii].name, desired_layer) == 0)

            break;
    }
    if (ii < No_Layers) {
        desired_layer_no = ii;
        delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
    } else {
        if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

            ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_NEW_LAYER_, 12, (char*)"", 11, 1, 62);
            no_error=FALSE;
            goto pl_error1;
        }
        if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

            ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_NEW_LAYER_, 12, (char*)"", 11, 1, 62);
            no_error=FALSE;
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

    //from meters to drawing units
    blok_origin.x=jednostkiObXm(mesh_node[0].x);
    blok_origin.y=jednostkiObYm(mesh_node[0].y);

    sprintf(block_name, "%s$mesh", ptr_id_short);

    ptr_block = add_block(blok_origin.x, blok_origin.y, 'E', block_name, FALSE);
    if (ptr_block == NULL) {
        fclose(f);
        ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_, 12, (char*)"", 11, 1, 62);
        no_error=FALSE;
        goto pl_error1;
    }
    GEO_LINE_MAX=1000;
    geo_line_no=0;
    geo_line=(GEO_LINE*)malloc(GEO_LINE_MAX * sizeof(GEO_LINE) + 100);
    memmove(&geo_line[geo_line_no].L, &Ldsp, sizeof(LINIA));
    geo_line[geo_line_no].n1=-1;
    geo_line[geo_line_no].n2=-1;

    for (i=0; i<mesh_element_no; i++)
    {
        //edge 1
        geo_line[geo_line_no].L.x1 = jednostkiObXm(mesh_node[mesh_element[i].node1 - 1].x);
        geo_line[geo_line_no].L.y1 = jednostkiObYm(mesh_node[mesh_element[i].node1 - 1].y);
        geo_line[geo_line_no].L.x2 = jednostkiObXm(mesh_node[mesh_element[i].node2 - 1].x);
        geo_line[geo_line_no].L.y2 = jednostkiObYm(mesh_node[mesh_element[i].node2 - 1].y);
        geo_line[geo_line_no].n1=mesh_element[i].node1;
        geo_line[geo_line_no].n2=mesh_element[i].node2;

        for (j = 0; j < geo_line_no; j++) {
            if (((geo_line[geo_line_no].n1==geo_line[j].n1) &&
                 (geo_line[geo_line_no].n2==geo_line[j].n2)) ||
                 ((geo_line[geo_line_no].n1==geo_line[j].n2) &&
                  (geo_line[geo_line_no].n2==geo_line[j].n1)))
                break;
        }
        if (j==geo_line_no)
            add_geo_line();
        //edge 2
        geo_line[geo_line_no].L.x1 = jednostkiObXm(mesh_node[mesh_element[i].node2 - 1].x);
        geo_line[geo_line_no].L.y1 = jednostkiObYm(mesh_node[mesh_element[i].node2 - 1].y);
        geo_line[geo_line_no].L.x2 = jednostkiObXm(mesh_node[mesh_element[i].node3 - 1].x);
        geo_line[geo_line_no].L.y2 = jednostkiObYm(mesh_node[mesh_element[i].node3 - 1].y);
        geo_line[geo_line_no].n1=mesh_element[i].node2;
        geo_line[geo_line_no].n2=mesh_element[i].node3;

        for (j = 0; j < geo_line_no; j++) {
            if (((geo_line[geo_line_no].n1==geo_line[j].n1) &&
                 (geo_line[geo_line_no].n2==geo_line[j].n2)) ||
                ((geo_line[geo_line_no].n1==geo_line[j].n2) &&
                 (geo_line[geo_line_no].n2==geo_line[j].n1)))
                break;
        }
        if (j==geo_line_no)
            add_geo_line();
        //edge 3
        geo_line[geo_line_no].L.x1 = jednostkiObXm(mesh_node[mesh_element[i].node3 - 1].x);
        geo_line[geo_line_no].L.y1 = jednostkiObYm(mesh_node[mesh_element[i].node3 - 1].y);
        geo_line[geo_line_no].L.x2 = jednostkiObXm(mesh_node[mesh_element[i].node1 - 1].x);
        geo_line[geo_line_no].L.y2 = jednostkiObYm(mesh_node[mesh_element[i].node1 - 1].y);
        geo_line[geo_line_no].n1=mesh_element[i].node3;
        geo_line[geo_line_no].n2=mesh_element[i].node1;

        for (j = 0; j < geo_line_no; j++) {
            if (((geo_line[geo_line_no].n1==geo_line[j].n1) &&
                 (geo_line[geo_line_no].n2==geo_line[j].n2)) ||
                ((geo_line[geo_line_no].n1==geo_line[j].n2) &&
                 (geo_line[geo_line_no].n2==geo_line[j].n1)))
                break;
        }
        if (j==geo_line_no)
            add_geo_line();
    }

    for (i=0; i<geo_line_no; i++)
    {
        dodaj_obiekt((BLOK *)dane, &geo_line[i].L);
    }


    //geometry nodes and element
    sprintf(block_name, "%s$Geo_Nodes&Elms", ptr_id_short);

    ptr_block = add_block(blok_origin.x, blok_origin.y, 'E', block_name, FALSE);
    if (ptr_block == NULL) {
        fclose(f);
        ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_, 12, (char*)"", 11, 1, 62);
        no_error=FALSE;
        goto pl_error1;
    }

    T.justowanie = j_srodkowo;
    T.bold = 0;
    T.warstwa = Ldsp.warstwa;
    T.kolor = Ldsp.kolor;
    T.czcionka = zmwym.czcionka;
    T.wysokosc = (float)(zmwym.wysokosc * 0.5);
    T.kat = 0.0f;
    T.blok=1;
    T.width_factor=(float)zmwym.width_factor;
    T.italics=0;
    T.underline=0;
    T.width=0;
    T.kolor=0;

    for (i=0; i<pl_node_no; i++)
    {
        T.x=(float)pl_node[i].x;
        T.y=(float)pl_node[i].y;
        sprintf(T.text, "%d", i+1);

        T.dl = strlen(T.text);
        T.n = T18 + T.dl;
        normalize_txt(&T);
        adr = dodaj_obiekt((BLOK *) dane, (void *) &T);
    }

    T.italics=1;
    T.underline=1;
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

    for (i = 0; i < pl_edge_no; i++)
    {
            Lt.x1 = (float)pl_node[pl_edge[i].node1].x;
            Lt.y1 = (float)pl_node[pl_edge[i].node1].y;
            Lt.x2 = (float)pl_node[pl_edge[i].node2].x;
            Lt.y2 = (float)pl_node[pl_edge[i].node2].y;

        if ((Check_if_Equal(Lt.x1, Lt.x2)) && (Check_if_Equal(Lt.y1, Lt.y2)))
        {
            PL.kat=0;
            psize=Get_Point_Size()/2.0;
            T.x = (float)(Lt.x1 + psize);
            T.y = (float)(Lt.y1 + psize);
        }
        else
        {
            parametry_lini(&Lt, &PL);
            T.kat = Pi_ * PL.kat / 180;
            //T.x = Lt.x1 + ((Lt.x2 - Lt.x1) / 3);
            //T.y = Lt.y1 + ((Lt.y2 - Lt.y1) / 3);
            T.x = (float)((Lt.x1 + Lt.x2) / 2.);
            T.y = (float)((Lt.y1 + Lt.y2) / 2.);
        }

        sprintf(T.text, "%d", i+1);

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
        ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_, 12, (char*)"", 11, 1, 62);
        no_error=FALSE;
        goto pl_error1;
    }

    //mesh nodes
    T.italics=0;
    T.underline=0;
    T.wysokosc = (float)(zmwym.wysokosc * 0.05);
    T.kat = 0.0f;
    T.ukryty=1;
    for (i=0; i<mesh_nodes_no; i++)
    {
        T.x=jednostkiObXm(mesh_node[i].x);
        T.y=jednostkiObYm(mesh_node[i].y);
        sprintf(T.text, "%d", i+1);

        T.dl = strlen(T.text);
        T.n = T18 + T.dl;
        normalize_txt(&T);
        adr = dodaj_obiekt((BLOK *) dane, (void *) &T);
    }


    //mesh elements
    T.italics=1;
    T.underline=1;
    T.wysokosc = (float)(zmwym.wysokosc * 0.05);
    T.kat = 0.0f;
    T.ukryty=1;
    for (i=0; i<mesh_elements_no; i++)
    {
        T.x=jednostkiObXm((mesh_node[mesh_element[i].node1-1].x+mesh_node[mesh_element[i].node2-1].x+mesh_node[mesh_element[i].node3-1].x)/3.);
        T.y=jednostkiObYm((mesh_node[mesh_element[i].node1-1].y+mesh_node[mesh_element[i].node2-1].y+mesh_node[mesh_element[i].node3-1].y)/3.);
        sprintf(T.text, "%d", i+1);

        T.dl = strlen(T.text);
        T.n = T18 + T.dl;
        normalize_txt(&T);
        adr = dodaj_obiekt((BLOK *) dane, (void *) &T);
    }

    free(geo_line);
    geo_line=NULL;

    //results
    sprintf(params, "%splate.result", _plate_);
    f = fopen(params, "rt");

    if (f == NULL) {
        ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_OPEN_RESULTS_FILE_, 12, (char*)"plate.result", 11, 1, 62);
        no_error=FALSE;
        goto pl_error1;
    }

    i_row=0;
    data_row=data_row_[i_row];
    //deflection
    while (fgets(data_row, MaxTextLen, f))
    {
        Perm = strstr(data_row, "Perm:");
        if (Perm != NULL)
        {
            strcpy(deflection_chapter[0], data_row_[!i_row]);
            deflection_chapter[0][strlen(deflection_chapter[0])-1]='\0';
            ret = sscanf(data_row + 6, "%d %d", &perm_d_node_no, &perm_d_index_no);
            if ((ret == 2) && (perm_d_node_no > 0) && (perm_d_index_no > 0)) {
                //perm_d = (PERM *) malloc(perm_d_node_no * sizeof(PERM *));
                perm_d = (int *) malloc(perm_d_node_no * sizeof(int)  + 100);
                for (i = 0; i < perm_d_node_no; i++) perm_d[i]=-1;
                perm_d_b = 1;
                for (i = 0; i < perm_d_index_no; i++) {
                    fgets(data_row, MaxTextLen, f);
                    ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                    //perm_d[node_number-1]=index_number-1;
                    perm_d[node_number-1]=i;
                }
                for (i = 0; i < DEFLECTION_NUMBER; i++)
                    deflection[i] = (double *) malloc(perm_d_index_no * sizeof(double)  + 100);
                //deflection 1...EPSILON_NUMBER
                for (j = 0; j < DEFLECTION_NUMBER; j++)
                {
                    for (i = 0; i < perm_d_index_no; i++)
                    {
                        fgets(data_row, MaxTextLen, f);
                        ret = sscanf(data_row, "%lf", &deflection[j][i]);
                        deflection_min[j]=min(deflection_min[j], deflection[j][i]);
                        deflection_max[j]=max(deflection_max[j], deflection[j][i]);
                    }
                    if (j < (DEFLECTION_NUMBER-1))
                    {
                        fgets(data_row, MaxTextLen, f);
                        strcpy(deflection_chapter[j+1], data_row);
                        deflection_chapter[j+1][strlen(deflection_chapter[j+1])-1]='\0';
                        fgets(data_row, MaxTextLen, f);
                        /////////////////////////
                        Perm = strstr(data_row, "Perm:");
                        use_previous = strstr(data_row, "use previous");
                        if (Perm != NULL)
                        {
                            if (use_previous != NULL) { ; //do nothing
                            } else {
                                ret = sscanf(data_row + 6, "%d %d", &perm_d_node_no, &perm_d_index_no);
                                if (ret == 2) {
                                    if (perm_d_b) free(perm_d);
                                    perm_d = (int *) malloc(perm_d_node_no * sizeof(int)  + 100);
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
        i_row=!i_row;
        data_row=data_row_[i_row];
    }

    i_row=0;
    data_row=data_row_[i_row];

    //stress
    perm_s_b=0;
    while (fgets(data_row, MaxTextLen, f))
    {
        Perm = strstr(data_row, "Perm:");
        use_previous = strstr(data_row, "use previous");
        if (Perm != NULL) {
            strcpy(stress_chapter[0], data_row_[!i_row]);
            stress_chapter[0][strlen(stress_chapter[0])-1]='\0';
            if (use_previous != NULL) {
                perm_s = perm_d;
                perm_s_node_no = perm_d_node_no;
                perm_s_index_no = perm_d_index_no;
            } else
            {
                ret = sscanf(data_row + 6, "%d %d", &perm_s_node_no, &perm_s_index_no);
                if (ret == 2)
                {
                    perm_s = (int *) malloc(perm_s_node_no * sizeof(int)  + 100);
                    for (i = 0; i < perm_d_node_no; i++) perm_s[i]=-1;
                    perm_s_b = 1;
                    for (i = 0; i < perm_s_index_no; i++) {
                        fgets(data_row, MaxTextLen, f);
                        ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                        //perm_s[node_number-1]=index_number-1;
                        perm_s[node_number-1]=i;
                    }
                }
                else
                {
                    ; //TO DO - ERROR
                }
            }
            for (i = 0; i < STRESS_NUMBER; i++)
                stress[i] = (double *) malloc(perm_s_index_no * sizeof(double)  + 100);
            //stress 1...6
            for (j = 0; j < STRESS_NUMBER; j++) {
                for (i = 0; i < perm_s_index_no; i++) {
                    fgets(data_row, MaxTextLen, f);
                    ret = sscanf(data_row, "%lf", &stress[j][i]);
                    stress_min[j]=min(stress_min[j], stress[j][i]);
                    stress_max[j]=max(stress_max[j], stress[j][i]);
                }
                if (j < (STRESS_NUMBER-1)) {
                    fgets(data_row, MaxTextLen, f);
                    strcpy(stress_chapter[j+1], data_row);
                    stress_chapter[j+1][strlen(stress_chapter[j+1])-1]='\0';
                    fgets(data_row, MaxTextLen, f);
                    /////////////////////////
                    Perm = strstr(data_row, "Perm:");
                    use_previous = strstr(data_row, "use previous");
                    if (Perm != NULL)
                    {
                        if (use_previous != NULL)
                        { ; //do nothing
                        } else
                        {
                            ret = sscanf(data_row + 6, "%d %d", &perm_s_node_no, &perm_s_index_no);
                            if (ret == 2)
                            {
                                if (perm_s_b) free(perm_s);
                                perm_s = (int *) malloc(perm_s_node_no * sizeof(int)  + 100);
                                for (i = 0; i < perm_s_node_no; i++) perm_s[i] = -1;
                                perm_s_b = 1;
                                for (i = 0; i < perm_s_index_no; i++) {
                                    fgets(data_row, MaxTextLen, f);
                                    ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                                    perm_s[node_number - 1] = i;
                                }
                            } else
                            { ; //TO DO - ERROR
                            }
                        }
                    }
                    /////////////////////////
                }
            }
            break;
        }
        i_row=!i_row;
        data_row=data_row_[i_row];
    }

    i_row=0;
    data_row=data_row_[i_row];

    //epsilon
    perm_e_b = 0;
    while (fgets(data_row, MaxTextLen, f))
    {
        Perm = strstr(data_row, "Perm:");
        use_previous = strstr(data_row, "use previous");
        if (Perm != NULL) {
            strcpy(epsilon_chapter[0], data_row_[!i_row]);
            epsilon_chapter[0][strlen(epsilon_chapter[0])-1]='\0';
            if (use_previous != NULL) {
                perm_e = perm_s;
                perm_e_node_no = perm_s_node_no;
                perm_e_index_no = perm_s_index_no;
            } else
            {
                ret = sscanf(data_row + 6, "%d %d", &perm_e_node_no, &perm_e_index_no);
                if (ret == 2)
                {
                    perm_e = (int *) malloc(perm_e_node_no * sizeof(int)  + 100);
                    for (i = 0; i < perm_d_node_no; i++) perm_e[i]=-1;
                    perm_e_b = 1;
                    for (i = 0; i < perm_e_index_no; i++) {
                        fgets(data_row, MaxTextLen, f);
                        ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                        perm_e[node_number-1]=i;
                    }
                }
                else
                {
                    ; //TO DO - ERROR
                }
            }
            for (i = 0; i < EPSILON_NUMBER; i++)
                epsilon[i] = (double *) malloc(perm_e_index_no * sizeof(double)  + 100);
            //epsilon 1...6
            for (j = 0; j < EPSILON_NUMBER; j++) {
                for (i = 0; i < perm_e_index_no; i++) {
                    fgets(data_row, MaxTextLen, f);
                    ret = sscanf(data_row, "%lf", &epsilon[j][i]);
                    epsilon_min[j]=min(epsilon_min[j], epsilon[j][i]);
                    epsilon_max[j]=max(epsilon_max[j], epsilon[j][i]);
                }
                if (j < (EPSILON_NUMBER-1))
                {
                    fgets(data_row, MaxTextLen, f);
                    strcpy(epsilon_chapter[j+1], data_row);
                    epsilon_chapter[j+1][strlen(epsilon_chapter[j+1])-1]='\0';
                    fgets(data_row, MaxTextLen, f);
                    /////////////////////////
                    Perm = strstr(data_row, "Perm:");
                    use_previous = strstr(data_row, "use previous");
                    if (Perm != NULL)
                    {
                        if (use_previous != NULL)
                        {
                            ; //do nothing
                        } else
                        {
                            ret = sscanf(data_row + 6, "%d %d", &perm_e_node_no, &perm_e_index_no);
                            if (ret == 2)
                            {
                                if (perm_e_b) free(perm_e);
                                perm_e = (int *) malloc(perm_e_node_no * sizeof(int)  + 100);
                                for (i = 0; i < perm_e_node_no; i++) perm_e[i] = -1;
                                perm_e_b = 1;
                                for (i = 0; i < perm_e_index_no; i++) {
                                    fgets(data_row, MaxTextLen, f);
                                    ret = sscanf(data_row, "%d %d", &node_number, &index_number);
                                    perm_e[node_number - 1] = i;
                                }
                            } else
                            { ; //TO DO - ERROR
                            }
                        }
                    }
                        /////////////////////////
                }
            }
            break;
        }
        i_row=!i_row;
        data_row=data_row_[i_row];
    }

    i_row=0;
    data_row=data_row_[i_row];

    fclose(f);

    //allocation space for block names
    block_names_no=0;

    my_sleep(10);
    strncpy(SLS_ULS, "sls", 3);  //temporary
    //deflection blocks
    //sprintf(desired_layer, "%s_%s", ptr_id, "deflection");
    sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "deflection", SLS_ULS, combination_no);

    for (ii = 0; ii < No_Layers; ii++) {
        if (strcmp(Layers[ii].name, desired_layer) == 0)

            break;
    }
    if (ii < No_Layers) {
        desired_layer_no = ii;
        get_blocks_setup(desired_layer_no, block_names, &block_names_no, MAX_L_BLOCKS, MAX_L_BLOCKS_LEN);  //to remember only visible blocks
        delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
    } else {
        if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

            ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_NEW_LAYER_, 12, (char*)"", 11, 1, 62);
            no_error=FALSE;
            goto pl_error1;
        }
        if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

            ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_NEW_LAYER_, 12, (char*)"", 11, 1, 62);
            no_error=FALSE;
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

    for (i=0; i<DEFLECTION_NUMBER; i++)
    {
        Ldsp.warstwa = desired_layer_no;
        w.warstwa = Ldsp.warstwa;
        w.kolor = Ldsp.kolor;
        w.blok = 1;

        //from meters to drawing units
        blok_origin.x = jednostkiObXm(mesh_node[0].x);
        blok_origin.y = jednostkiObYm(mesh_node[0].y);

        sprintf(block_name, "%s$%s_%s_%d", ptr_id_short, deflection_chapter[i], SLS_ULS, combination_no);

        hiding = TRUE;
        for (j = 0; j < block_names_no; j++) {
            if (strcmp(block_names[j], block_name) == 0) {
                hiding = FALSE;
                break;
            }
        }

        deflection_amax = max(fabs(deflection_max[i]), fabs(deflection_min[i]));

        if (deflection_amax>0.0)
        {
            ptr_block = add_block(blok_origin.x, blok_origin.y, B_GRAPH, block_name, hiding);
            if (ptr_block == NULL) {
                fclose(f);
                ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_DEFLECTION_BLOCK_, 12, (char*)"", 11, 1, 62);
                no_error = FALSE;
                goto pl_error1;
            }

            for (j = 0; j < mesh_element_no; j++) {
                w.xy[0] = jednostkiObXm(mesh_node[mesh_element[j].node1 - 1].x);
                w.xy[1] = jednostkiObYm(mesh_node[mesh_element[j].node1 - 1].y);

                w.xy[2] = jednostkiObXm(mesh_node[mesh_element[j].node2 - 1].x);
                w.xy[3] = jednostkiObYm(mesh_node[mesh_element[j].node2 - 1].y);

                w.xy[4] = jednostkiObXm(mesh_node[mesh_element[j].node3 - 1].x);
                w.xy[5] = jednostkiObYm(mesh_node[mesh_element[j].node3 - 1].y);

                if (perm_d[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, deflection_amax);
                else gradient.c1 = getRGB(-deflection[i][perm_d[mesh_element[j].node1 - 1]], deflection_amax);
                if (perm_d[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, deflection_amax);
                else gradient.c2 = getRGB(-deflection[i][perm_d[mesh_element[j].node2 - 1]], deflection_amax);
                if (perm_d[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, deflection_amax);
                else gradient.c3 = getRGB(-deflection[i][perm_d[mesh_element[j].node3 - 1]], deflection_amax);
                gradient.c4 = getRGB(0.0, deflection_amax);

                fe_data.el_number = j + 1;
                fe_data.f1 = (float) deflection[i][perm_d[mesh_element[j].node1 - 1]];
                fe_data.f2 = (float) deflection[i][perm_d[mesh_element[j].node2 - 1]];
                fe_data.f3 = (float) deflection[i][perm_d[mesh_element[j].node3 - 1]];
                fe_data.f4 = 0.0f;

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

                gradient.c1 = getRGB(-deflection_min[i], deflection_amax);
                gradient.c2 = getRGB(-deflection_max[i], deflection_amax);
                gradient.c3 = getRGB(-deflection_max[i], deflection_amax);
                gradient.c4 = getRGB(-deflection_min[i], deflection_amax);

                fe_data.el_number = 0;
                fe_data.f1 = (float) -deflection_min[i];
                fe_data.f2 = (float) -deflection_max[i];
                fe_data.f3 = (float) -deflection_max[i];
                fe_data.f4 = (float) -deflection_min[i];

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
                if (i<3)  //in [mm]
                {
                    if (UNITS == SI) {
                        deflection_min[i] *= 1000.0; //in mm
                        deflection_max[i] *= 1000.0; //in mm
                        unit = _mm_;
                    } else //IMP stays as is  in inches
                    {
                        unit = _inch_;
                    }
                }
                else
                {
                    //in radians
                    unit = _rad_;
                }

                T.warstwa = Ldsp.warstwa;
                T.kolor = Ldsp.kolor;
                T.italics = 0;
                T.underline = 0;
                T.wysokosc = (float) (zmwym.wysokosc * 0.5);
                T.kat = 0.0f;
                T.ukryty = 0;

                T.x = w.xy[0];
                T.y = w.xy[1] + 0.5f;
                T.justowanie = j_do_lewej;
                sprintf(T.text, "%.5f", -deflection_min[i]);

                T.dl = strlen(T.text);
                T.n = T18 + T.dl;
                normalize_txt(&T);
                adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                T.x = w.xy[2];
                T.y = w.xy[3] + 0.5f;
                T.justowanie = j_do_prawej;
                sprintf(T.text, "%.4f", deflection_max[i]);

                T.dl = strlen(T.text);
                T.n = T18 + T.dl;
                normalize_txt(&T);
                adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                T.x = (w.xy[0] + w.xy[2]) / 2.f;
                T.y = w.xy[3] + 0.5f;
                T.justowanie = j_srodkowo;
                sprintf(T.text, "%s %s", deflection_chapter[i], unit);

                T.dl = strlen(T.text);
                T.n = T18 + T.dl;
                normalize_txt(&T);
                adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

            }
        }
    }

    my_sleep(10);
    //stress blocks
    //sprintf(desired_layer, "%s_%s", ptr_id, "stress");
    strcpy(SLS_ULS, "sls"); //temporary
    sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "stress", SLS_ULS, combination_no);

    for (ii = 0; ii < No_Layers; ii++) {
        if (strcmp(Layers[ii].name, desired_layer) == 0)

            break;
    }
    if (ii < No_Layers) {
        desired_layer_no = ii;
        get_blocks_setup(desired_layer_no, block_names, &block_names_no, MAX_L_BLOCKS, MAX_L_BLOCKS_LEN);  //to remember only visible blocks
        delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
    } else {
        if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

            ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_NEW_LAYER_, 12, (char*)"", 11, 1, 62);
            no_error=FALSE;
            goto pl_error1;
        }
        if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

            ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_NEW_LAYER_, 12, (char*)"", 11, 1, 62);
            no_error=FALSE;
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

    if (perm_s!=NULL)
    {
        for (i = 0; i < STRESS_NUMBER; i++)
        {
            sprintf(block_name, "%s$%s_%s_%d", ptr_id_short, stress_chapter[i], SLS_ULS, combination_no);

            hiding=TRUE;
            for (j=0; j<block_names_no; j++)
            {
                if (strcmp(block_names[j], block_name)==0)
                {
                    hiding=FALSE;
                    break;
                }
            }

            Ldsp.warstwa = desired_layer_no;
            w.warstwa = Ldsp.warstwa;
            w.kolor = Ldsp.kolor;
            w.blok = 1;

            //from meters to drawing units
            blok_origin.x = jednostkiObXm(mesh_node[0].x);
            blok_origin.y = jednostkiObYm(mesh_node[0].y);

            stress_amax = max(fabs(stress_max[i]), fabs(stress_min[i]));
            if (stress_amax>0)
            {
                ptr_block = add_block(blok_origin.x, blok_origin.y, B_GRAPH, block_name, hiding);
                if (ptr_block == NULL) {
                    fclose(f);
                    ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_DEFLECTION_BLOCK_, 12, (char*)"", 11, 1, 62);
                    no_error = FALSE;
                    goto pl_error1;
                }


                for (j = 0; j < mesh_element_no; j++) {
                    w.xy[0] = jednostkiObXm(mesh_node[mesh_element[j].node1 - 1].x);
                    w.xy[1] = jednostkiObYm(mesh_node[mesh_element[j].node1 - 1].y);

                    w.xy[2] = jednostkiObXm(mesh_node[mesh_element[j].node2 - 1].x);
                    w.xy[3] = jednostkiObYm(mesh_node[mesh_element[j].node2 - 1].y);

                    w.xy[4] = jednostkiObXm(mesh_node[mesh_element[j].node3 - 1].x);
                    w.xy[5] = jednostkiObYm(mesh_node[mesh_element[j].node3 - 1].y);

                    if (perm_s[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, stress_amax);
                    else gradient.c1 = getRGB(stress[i][perm_s[mesh_element[j].node1 - 1]], stress_amax);
                    if (perm_s[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, stress_amax);
                    else gradient.c2 = getRGB(stress[i][perm_s[mesh_element[j].node2 - 1]], stress_amax);
                    if (perm_s[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, stress_amax);
                    else gradient.c3 = getRGB(stress[i][perm_s[mesh_element[j].node3 - 1]], stress_amax);
                    gradient.c4 = getRGB(0.0, stress_amax);

                    fe_data.el_number = j + 1;
                    fe_data.f1 = (float) stress[i][perm_s[mesh_element[j].node1 - 1]];
                    fe_data.f2 = (float) stress[i][perm_s[mesh_element[j].node2 - 1]];
                    fe_data.f3 = (float) stress[i][perm_s[mesh_element[j].node3 - 1]];
                    fe_data.f4 = 0.0f;

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
                if (stress_amax > 0.0) {
                    w.xy[0] = (float) pl_min_x + 10.f;
                    w.xy[1] = (float) pl_min_y - 10.f;

                    w.xy[2] = w.xy[0] + 50.f;
                    w.xy[3] = w.xy[1];

                    w.xy[4] = w.xy[2];
                    w.xy[5] = w.xy[3] - 5.f;

                    w.xy[6] = w.xy[0];
                    w.xy[7] = w.xy[5];

                    gradient.c1 = getRGB(stress_min[i], stress_amax);
                    gradient.c2 = getRGB(stress_max[i], stress_amax);
                    gradient.c3 = getRGB(stress_max[i], stress_amax);
                    gradient.c4 = getRGB(stress_min[i], stress_amax);

                    fe_data.el_number = 0;
                    fe_data.f1 = (float) stress_min[i];
                    fe_data.f2 = (float) stress_max[i];
                    fe_data.f3 = (float) stress_max[i];
                    fe_data.f4 = (float) stress_min[i];

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
                    if (UNITS == SI) {
                        stress_min[i] *= 0.000001; //in mm
                        stress_max[i] *= 0.000001; //in mm
                        unit = _MPa_;
                    } else //IMP stays as is  in inches
                    {
                        stress_min[i] *= 0.001; //in mm
                        stress_max[i] *= 0.001; //in mm
                        unit = _ksi_;
                    }

                    T.warstwa = Ldsp.warstwa;
                    T.kolor = Ldsp.kolor;
                    T.italics = 0;
                    T.underline = 0;
                    T.wysokosc = (float) (zmwym.wysokosc * 0.5);
                    T.kat = 0.0f;
                    T.ukryty = 0;

                    T.x = w.xy[0];
                    T.y = w.xy[1] + 0.5f;
                    T.justowanie = j_do_lewej;
                    sprintf(T.text, "%.5f", stress_min[i]);

                    T.dl = strlen(T.text);
                    T.n = T18 + T.dl;
                    normalize_txt(&T);
                    adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                    T.x = w.xy[2];
                    T.y = w.xy[3] + 0.5f;
                    T.justowanie = j_do_prawej;
                    sprintf(T.text, "%.4f", stress_max[i]);

                    T.dl = strlen(T.text);
                    T.n = T18 + T.dl;
                    normalize_txt(&T);
                    adr = dodaj_obiekt((BLOK *) dane, (void *) &T);

                    T.x = (w.xy[0] + w.xy[2]) / 2.f;
                    T.y = w.xy[3] + 0.5f;
                    T.justowanie = j_srodkowo;
                    sprintf(T.text, "%s %s", stress_chapter[i], unit);

                    T.dl = strlen(T.text);
                    T.n = T18 + T.dl;
                    normalize_txt(&T);
                    adr = dodaj_obiekt((BLOK *) dane, (void *) &T);
                }
            }
        }
    }

    my_sleep(10);
    //epsilon blocks
    //sprintf(desired_layer, "%s_%s", ptr_id, "epsilon");
    strcpy(SLS_ULS, "sls"); //temporary
    sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "epsilon", SLS_ULS, combination_no);

    for (ii = 0; ii < No_Layers; ii++) {
        if (strcmp(Layers[ii].name, desired_layer) == 0)

            break;
    }
    if (ii < No_Layers) {
        desired_layer_no = ii;
        get_blocks_setup(desired_layer_no, block_names, &block_names_no, MAX_L_BLOCKS, MAX_L_BLOCKS_LEN);  //to remember only visible blocks
        delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
    } else {
        if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

            ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_NEW_LAYER_, 12, (char*)"", 11, 1, 62);
            no_error=FALSE;
            goto pl_error1;
        }
        if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {

            ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_NEW_LAYER_, 12, (char*)"", 11, 1, 62);
            no_error=FALSE;
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

    if (perm_e!=NULL)
    {
        for (i = 0; i < EPSILON_NUMBER; i++)
        {
            sprintf(block_name, "%s$%s_%s_%d", ptr_id_short, epsilon_chapter[i], SLS_ULS, combination_no);

            hiding=TRUE;
            for (j=0; j<block_names_no; j++)
            {
                if (strcmp(block_names[j], block_name)==0)
                {
                    hiding=FALSE;
                    break;
                }
            }

            Ldsp.warstwa = desired_layer_no;
            w.warstwa = Ldsp.warstwa;
            w.kolor = Ldsp.kolor;
            w.blok = 1;
            //from meters to drawing units
            blok_origin.x = jednostkiObXm(mesh_node[0].x);
            blok_origin.y = jednostkiObYm(mesh_node[0].y);

            epsilon_amax = max(fabs(epsilon_max[i]), fabs(epsilon_min[i]));
            if (epsilon_amax>0)
            {
                ptr_block = add_block(blok_origin.x, blok_origin.y, B_GRAPH, block_name, hiding);
                if (ptr_block == NULL) {
                    fclose(f);
                    ret = ask_question(1, (char*)"", (char *) confirm, (char*)"", (char *) _CANNOT_CREATE_DEFLECTION_BLOCK_, 12, (char*)"", 11, 1, 62);
                    no_error = FALSE;
                    goto pl_error1;
                }


                for (j = 0; j < mesh_element_no; j++) {
                    w.xy[0] = jednostkiObXm(mesh_node[mesh_element[j].node1 - 1].x);
                    w.xy[1] = jednostkiObYm(mesh_node[mesh_element[j].node1 - 1].y);

                    w.xy[2] = jednostkiObXm(mesh_node[mesh_element[j].node2 - 1].x);
                    w.xy[3] = jednostkiObYm(mesh_node[mesh_element[j].node2 - 1].y);

                    w.xy[4] = jednostkiObXm(mesh_node[mesh_element[j].node3 - 1].x);
                    w.xy[5] = jednostkiObYm(mesh_node[mesh_element[j].node3 - 1].y);

                    if (perm_e[mesh_element[j].node1 - 1] == -1) gradient.c1 = getRGB(0, epsilon_amax);
                    else gradient.c1 = getRGB(epsilon[i][perm_e[mesh_element[j].node1 - 1]], epsilon_amax);
                    if (perm_e[mesh_element[j].node2 - 1] == -1) gradient.c2 = getRGB(0, epsilon_amax);
                    else gradient.c2 = getRGB(epsilon[i][perm_e[mesh_element[j].node2 - 1]], epsilon_amax);
                    if (perm_e[mesh_element[j].node3 - 1] == -1) gradient.c3 = getRGB(0, epsilon_amax);
                    else gradient.c3 = getRGB(epsilon[i][perm_e[mesh_element[j].node3 - 1]], epsilon_amax);
                    gradient.c4 = getRGB(0.0, epsilon_amax);

                    fe_data.el_number = j + 1;
                    fe_data.f1 = (float) epsilon[i][perm_e[mesh_element[j].node1 - 1]];
                    fe_data.f2 = (float) epsilon[i][perm_e[mesh_element[j].node2 - 1]];
                    fe_data.f3 = (float) epsilon[i][perm_e[mesh_element[j].node3 - 1]];
                    fe_data.f4 = 0.0f;

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

                    gradient.c1 = getRGB(epsilon_min[i], epsilon_amax);
                    gradient.c2 = getRGB(epsilon_max[i], epsilon_amax);
                    gradient.c3 = getRGB(epsilon_max[i], epsilon_amax);
                    gradient.c4 = getRGB(epsilon_min[i], epsilon_amax);

                    fe_data.el_number = 0;
                    fe_data.f1 = (float) epsilon_min[i];
                    fe_data.f2 = (float) epsilon_max[i];
                    fe_data.f3 = (float) epsilon_max[i];
                    fe_data.f4 = (float) epsilon_min[i];

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
                    sprintf(T.text, "%s", epsilon_chapter[i]);

                    T.dl = strlen(T.text);
                    T.n = T18 + T.dl;
                    normalize_txt(&T);
                    adr = dodaj_obiekt((BLOK *) dane, (void *) &T);
                }
            }
        }
    }

pl_error2:

    if (geo_line) free(geo_line);
    if (perm_d) free(perm_d);
    if (perm_s_b==1) {
        if (perm_s) free(perm_s);
    }
    if (perm_e_b==1) {
        if (perm_e) free(perm_e);
    }

    for (i=0; i<DEFLECTION_NUMBER; i++) if (deflection[i]!=NULL)
    {
        free(deflection[i]);
        deflection[i]=NULL;
    }
    for (i=0; i<STRESS_NUMBER; i++) if (stress[i]!=NULL)
    {
        free(stress[i]);
        stress[i]=NULL;
    }
    for (i=0; i<EPSILON_NUMBER; i++) if (epsilon[i]!=NULL)
    {
        free(epsilon[i]);
        epsilon[i]=NULL;
    }

pl_error1:

    if (mesh_node) free(mesh_node);
    if (mesh_element) free(mesh_element);
    if (mesh_boundary) free(mesh_boundary);

pl_error:


    if (was_refreshed==FALSE) {
        ///removing Ablok flags and refresh
        zmien_atrybut_undo(dane, dane + dane_size);
        blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
        zmien_atrybut(ADP, ADK, Ablok, Aoblok);
        was_refreshed = TRUE;
    }

    if (strlen(report) > 0) {
        int edit_params = 0;
        int tab;
        int single = 2;  //info
        ret = EditText(report, edit_params, mynCmdShow, &single, &tab);
        report[0] = '\0';
    }

    if (pl_property) free(pl_property);
    if (pl_load_factors)free(pl_load_factors);
    if (pl_node) free(pl_node);
    if (pl_edge) free(pl_edge);
    if (pl_load) free(pl_load);
    if (plate_property) free(plate_property);
    if (hole_property) free(hole_property);
    if (wall_property) free(wall_property);
    if (zone_property) free(zone_property);
    ////////////////

push:
    redraw();

    my_sleep(100);
    return;
}

#undef __O_PLATE__