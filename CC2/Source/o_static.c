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

#define __O_STATIC__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <forwin.h>
#ifdef LINUX
#include <unistd.h>
#endif
#include <allegext.h>
#include "bib_e.h"
#include "o_static.h"
#include "o_bloklb.h"
#include "bib_blok.h"
#include "o_object.h"
#include "rysuj_e.h"
#include "message.h"
#include "o_loadf.h"

#include "leak_detector_c.h"

typedef unsigned long DWORD;

extern void LockMouse(void);
extern void FreeMouse(void);
extern void select_blok(void);
extern void redcrsb(char typ, int n);
extern int ask_question(int n_buttons, char* esc_string, char* ok_string, char* cont_string, char* comment_string, int color_comment, char* comment1_string, int color1_comment, int cien, int image);
extern int ask_question_static(int n_buttons, char* esc_string, char* ok_string, char* cont_string, char* comment_string, int color_comment, char* comment1_string, int color1_comment, int cien, int image, int *combinantion, int *geometri_stiffness, int *inertia, int *st_dynamic_no);
extern BOOL Check_if_Equal (double x, double y);
extern BOOL Check_if_Equal2(double x, double y);
extern BOOL Check_if_Equal3(double x, double y);
extern BOOL Check_if_LE (double x, double y);
extern BOOL Check_if_LE2 (double x, double y);
extern BOOL Check_if_GE (double x, double y);
extern BOOL Check_if_GE02 (double x, double y);
extern int EditText(char *mytext, int edit_params, int nCmdShow, int *single, int *tab);
extern double Angle_Normal (double angle);
extern BOOL CheckIsPointOnLineSegment(double x, double y, double x1, double y1, double x2, double y2, double epsilon);
extern void set_decimal_format(char *text, double l, double precision);
extern DWORD RunSilent(char* strFunct, char* strstrParams);
extern DWORD SystemSilent(char* strFunct, char* strstrParams);
extern int EditFile(char *, int adjust, int nCmdShow);
extern int mynCmdShow;
extern void Rotate_Point(double si, double co, double x1, double y1, /*center point*/ double x2, double y2, double *x, double *y); /*rotate point*/
extern void delete_all_from_layer_atrybut (int layer_no, int atrybut);
extern void delete_all_pattern_blocks_from_layer (int layer_no, char *pattern);
extern BOOL Check_Attribute (int i_attribute, int i_check);
extern void normalize_txt(TEXT *ptrs_text);
extern BOOL Load_File (char *f_name, int type, BOOL cur);
extern void get_posXY(double *pozx, double *pozy);
extern void _free_mouse(void);
extern void dialog_cursor(int on);
extern void lock_mouse(void);
extern void set_posXY(double pozx, double pozy);
extern double Get_Point_Size (void);
extern void create_solid_on_line (LINIA *L, WIELOKAT *s, double width1, double width2, double axis);
extern int __file_exists(char *name);
extern void  SetBit( unsigned int A[],  int k );
extern void  ClearBit( unsigned int A[],  int k );
extern int TestBit( unsigned int A[],  int k );
extern int text2PDF (char *in_file_name, char *out_file_name);

extern void get_console(void);
extern void free_console(void);

extern void get_display_window(void);
extern void focus_display_window(void);
extern int Expand_flex();

extern BOOL preview_blocked;

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

double n_magnitude=1.0; //10;
double v_magnitude=0.1; //10;
double m_magnitude=5.0; //10;
double d_magnitude=1;
double r_magnitude=0.1; //10;
double rm_magnitude=50.0; //0.001;
double s_magnitude=5.0; //10;  //stress
double q_magnitude=250.0; //vibrations

double n_magnitude0=1.0; //10;
double v_magnitude0=0.1; //10;
double m_magnitude0=5.0; //10;
double d_magnitude0=1;
double r_magnitude0=0.1; //10;
double rm_magnitude0=50.0; //0.001;
double s_magnitude0=5.0; //10;  //stress
double q_magnitude0=250.0; //vibrations

double def_precision=0.01;
PROP_PRECISIONS SI_precisions={0.1, 0.01, 0.01, 0.01, 0.1, 0.1, 0.001, 0.0001, 0.000000000000001};
PROP_PRECISIONS IMP_precisions={0.01, 0.001, 0.001, 0.001, 0.01, 0.01, 0.001, 0.0001, 0.000000000000001};
PROP_PRECISIONS *prop_precisions=&SI_precisions;  //just to initialize


double dim_precision=0.0001;
double t_precision=0.001;
double r_precision=0.001;
double rm_precision=0.001;

double m0999 = 0.9999;

extern TMENU mVector;

BOOL rout=FALSE;

void Static_analysis(void);

int st_layer_no=0;
int st_property_no=0;
int st_node_pre_no=0;
int st_node_no=0;
int st_reaction_no=0;
int st_element_no=0;
int st_node_force_moment_no=0;
int st_uniform_load_no=0;
int st_trapezoid_load_no=0;
int st_element_force_no=0;
int st_thermal_load_no=0;
int st_displacement_no=0;
int st_dynamic_no=0;
int st_load_no=0;
int st_load_factors_no=0;

int st_mass_nodes_no=0;
int st_mass_elements_no=0;

int stl_node_force_moment_no=0;
int stl_uniform_load_no=0;
int stl_trapezoid_load_no=0;
int stl_element_force_no=0;
int stl_thermal_load_no=0;
int stl_displacement_no=0;
int stl_dynamic_no=0;

int stlc_node_force_moment_no=0;
int stlc_uniform_load_no=0;
int stlc_trapezoid_load_no=0;
int stlc_element_force_no=0;
int stlc_thermal_load_no=0;
int stlc_displacement_no=0;
int stlc_dynamic_no=0;

int ST_PROPERTY_MAX=100;
int ST_NODE_PRE_MAX=100;
int ST_NODE_MAX=100;
int ST_REACTION_MAX=100;
int ST_ELEMENT_MAX=100;
int ST_NODE_FORCE_MOMENT_MAX=100;
int ST_UNIFORM_LOAD_MAX=100;
int ST_TRAPEZOID_LOAD_MAX=100;
int ST_ELEMENT_FORCE_MAX=100;
int ST_THERMAL_LOAD_MAX=100;
int ST_DISPLACEMENT_MAX=100;
int ST_LOAD_MAX=100;
int ST_LOAD_FACTORS_MAX=100;

int ST_MASS_NODE_MAX=100;
int ST_MASS_ELEMENT_MAX=100;

char T_text[64];

unsigned char st_layers[256];
unsigned int st_layer[8];
char st_title[MaxTextLen];
char title_id[MaxTextLen];
char *ptr_id, *ptr_id_short;
char par[13][MaxTextLen]={"","","","","","","","","","","","",""};
ST_PROPERTY *st_property;
ST_NODE_PRE *st_node_pre;
ST_NODE *st_node;
ST_REACTION *st_reaction;
ST_ELEMENT *st_element;
ST_NODE_FORCE_MOMENT *st_node_force_moment;
ST_UNIFORM_LOAD *st_uniform_load;
ST_TRAPEZOID_LOAD *st_trapezoid_load;
ST_ELEMENT_FORCE *st_element_force;
ST_THERMAL_LOAD *st_thermal_load;
ST_DISPLACEMENT *st_displacement;
ST_LOAD *st_load;
ST_LOAD_FACTORS *st_load_factors;

ST_MASS_NODE *st_mass_node=NULL;
ST_MASS_ELEMENT *st_mass_element=NULL;

COMBI_REACIONS *combi_reactions;
int *combi_reactions_table;
COMBI_ELEMENT *combi_element_uls;
COMBI_ELEMENT *combi_element_sls;
COMBI_ELEMENT *combi_element_qpsls;

int shear_deformation=0;
int geometric_tiffness=0;
int inertia=0;  //inclide mass inertia of Dead Load in vibrations
double mesh_deformations=10.0;
double zoom_scale=1.0;
double axis_increment=10.0;
double units_factor=1.0;
double m_units_factor=1000.0;

double gX=0;
double gY=0;
double gZ=0;

double Rn=0;

STATIC_COLORS static_colors={8, 1, 5, 3, 6, 2, 4, 156};
STATIC_COLORS static_colors0={8, 1, 5, 3, 6, 2, 4, 156};
STATIC_STRESS_COLORS static_stress_colors={9,13,11};
STATIC_STRESS_COLORS static_stress_colors0={9,13,11};
ST_PROPERTY prt_def={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

BOOL qpslsLC_Layer=FALSE;

//load;  //0 undefined, 1 dead, 2 live, 3 live roof load, 4 wind, 5 snow, 6 seismic, 7 rainwater load or ice water load, 8 hydraulic loads from soil, 9  F = hydraulic loads from fluids
//variant;  //0 undefined, 1..255 number of load character with different factors
//gamma;
//psi0;
//psi1;
//psi2;
//ξ;  //reduction factor for dead load

//                                    load, variant,      γ,       Ψ0,     Ψ1,    Ψ2,      ξ,     γ.inf
ST_LOAD_FACTORS st_load_factors_EU_0[]={ {/*0*/ 0,0,     1.5,    0.7,    0.5,   0.3,      0,      0},
                                         {/*DL*/1,0,     1.35,   1.0,    1.0,   1.0,      0.85,   0.9},
                                         {/*LL*/2,0,     1.5,    0.7,    0.5,   0.3,      0,      0},
                                         {/*Lr*/3,0,     1.5,    0.7,    0.5,   0.3,      0,      0},
                                         {/*WL*/4,0,     1.5,    0.6,    0.2,   0,        0,      0},
                                         {/*SL*/5,0,     1.5,    0.5,    0.2,   0,        0,      0},
                                         {/*EL*/6,0,     1.5,    0.7,    0.5,   0.3,      0,      0},
                                         {/*RL*/7,0,     1.5,    0.7,    0.5,   0.3,      0,      0},
                                         {/*HL*/8,0,     1.5,    0.7,    0.5,   0.3,      0,      0},
                                         {/*FL*/9,0,     1.35,   1.0,    1.0,   1.0,      0.85,   0.9},
                                         {/*TL*/10,0,    1.5,    0.6,    0.5,   0.0,      0,      0}};
//                                    load, variant,      γ,       Ψ0,     Ψ1,    Ψ2,      ξ,     γ.inf
ST_LOAD_FACTORS st_load_factors_ASCE_0[]={{/*0*/ 0,0,     0,       0,      0,      0,      0,      0},
                                          {/*DL*/1,0,     1.4,     0.9,    0.6,    0,      1.2,    0},
                                          {/*LL*/2,0,     1.6,     0,      0,      0.75,   0,      0},
                                          {/*Lr*/3,0,     1.6,     0.5,    0,      0.75,   0,      0},
                                          {/*WL*/4,0,     1.0,     0.5,    0.75,   0.6,    0,      0},
                                          {/*SL*/5,0,     1.6,     0.5,    0.2,    0.75,   0,      0},
                                          {/*EL*/6,0,     1.0,     0,      0.75,   0.7,    0,      0},
                                          {/*RL*/7,0,     1.6,     0.5,    0,      0.75,   0,      0},
                                          {/*HL*/8,0,     1.6,     0,      0,      0.75,   0,      0},
                                          {/*FL*/9,0,     1.4,     0.9,    0.6,    0,      1.2,    0},
                                          {/*TL*/10,0,    1.0,     0,      0,      0,      0,      0}};
//                                    load, variant,      γ,       Ψ0,     Ψ1,    Ψ2,      ξ,     γ.inf
ST_LOAD_FACTORS st_load_factors_ICC_0[]={{/*0*/  0,0,     0,       0,      0,      0,      0,      0},
                                          {/*DL*/1,0,     1.4,     0.9,    1.0,    0.6,    1.2,    0}, ////
                                          {/*LL*/2,0,     1.6,     0.5,    0,      0.75,   0,      0},
                                          {/*Lr*/3,0,     1.6,     0.5,    0,      0.75,   0,      0},
                                          {/*WL*/4,0,     1.0,     0.5,    0,      0.6,    0,      0},  ////
                                          {/*SL*/5,0,     1.6,     0.5,    0.2,    0.75,   0,      0},  ////
                                          {/*EL*/6,0,     1.0,     1.0,    0,      0.7,    0,      0},  ////
                                          {/*RL*/7,0,     1.6,     0.5,    0,      0.75,   0,      0},  ////
                                          {/*HL*/8,0,     1.6,       0,    0,      0,      0,      0},
                                          {/*FL*/9,0,     1.4,     0.9,    0,      0.6,    1.2,    0},
                                          {/*TL*/10,0,    1.0,       0,    0,      0,      0,      0}};

ST_LOAD_FACTORS *load_factors;



#define BUF_SIZE 65536 //2^16

/*
 1 kN = 0.224808 kip
 1 kNm = 8.85074579 kip-in
 1 mm = 0.0393701 in
 1 kN/m = 0.005710083 kip/in
 1 C = 1.8 F

 1 kip = 4.4482216153 kN
 1 kip-in = 0.112985 kNm
 1 in = 25.4 mm
 1 kip/in = 175.1268 kN/m
 1 F = 0.555555556 C
 */

UNIT_FACTORS *unit_factors;
UNIT_FACTORS unit_factors_si={1.0, 100.0,0.000001, 10000.0,0.000001, 1000.0,1e-12,1.0,1000.0,1000.0,1000000,1.0, 1000.0, 1000.0, 9.81};
UNIT_FACTORS unit_factors_imp={1.0, 1.0,1.0,1.0, 1.0,1.0,1.0,1.0,1.0,1.0,1.0, 1.0, 1000.0, 1.0, 1.0};

//double h_f; //depth of cross section in y axis                                   mm                  in
//double A_f; //area of cross section                                              cm^2 -> mm^2        in^2
//double I_f; //aread moment of inertia                                            cm^4 -> mm^4        in^4
//double W_f; //elastic section modulus                                            cm^3 -> mm^3        in^3
//double E_f; //Young's Modulus of elasticity and Shear modulus of elasticity      GPa -> Mpa          ksi
//double d_f; //mass density                                                       kg/m^3 -> t/mm^3     kip/in^3/g
//double a_f; //Coefficient of thermal expansion                                   /C                  /F
//double g_f; //gravitational acceleration                                         m/s^2 -> mm/s^2     in/s^2
//double F_f; //forces                                                             kN -> N             kip
//double M_f; //moments                                                            kNm -> Nmm          kip*in
//double q_f; //load
// kN/m -> N/mm        kip/in

//int combi_uls_no[255]={0};
//int combi_sls_no[255]={0};
//int combi_qpsls_no[255]={0};

int combi_uls_no=0;
int combi_sls_no=0;
int combi_qpsls_no=0;

int combi_total_numbers_no=0;
COMBI_TOTAL_NUMBER combi_total_numbers[255*3];

COMBINATION *ULSLC;
COMBINATION *SLSLC;
COMBINATION *QPSLSLC;
int *MC_ULSLC;
int *MC_SLSLC;
int *MC_QPSLSLC;

//having combi_flag combination flags can be set
//#define F1 1+256
//#define F2 2+32+128+512
#define F1 1
#define F2 2
COMBINATION EUROCODE_ULSLC[]={{F1,0},{F1+F2,0},{F1+F2+16,0},{F1+F2+16,0},{F1+F2+64,0},{F1+F2+4,0},{F1+F2+16+8,0},{F1+F2+64+8,0},{F1+F2+4+8,0},{F1+F2+16+8,0},
                             {F1+F2+64+8,0},{F1+F2+4+8,0},{F1+F2+16+8,0},{F1+F2+64+8,0},{F1+F2+4+8,0},{F1+16,0},{F1+64,0},{F1+4,0},{F1+8,0},{F1+16+8,0},
                             {F1+64+8,0},{F1+4+8,0},{F1+8+16,0},{F1+8+64,0},{F1+8+4,0},{F1+F2+8,0},{F1+F2+8,0},{F1+8,0}};  //28
//                     0, D=1,L=2,l=4,W=8,S=16,E=32,R=64,H=128,F=256,T=512
//                       1,2, 3,4, 5,6, 7,    8,  9,  10, 11,  12, 13,    14,  15, 16,17,18,19, 20,  21,  22, 23,  24,  25,26,27,28
int EUROCODE_MC_ULSLC[]={0,0,16,0,64,4,8+16,8+64,4+8,8+16,8+64,4+8, 8+16, 8+64,4+8,32,64, 4, 8,8+16,8+64,4+8,8+32,8+64,4+8,8,8,0};  //28

int EUROCODE_ULSLC_NO=28;

COMBINATION EUROCODE_SLSLC[]={{F1,0},{F1+F2,0},{F1+F2+16,0},{F1+F2+8,0},{F1+F2+16+8,0},{F1+F2+16+8,0},{F1+F2+16+8,0},{F1+16,0},{F1+8,0},{F1+16+8,0},
                             {F1+8+16,0},{F1+F2+16,0},{F1+F2+8,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
                             {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};  //13
//full stack is 1023
//                       1,2, 3,4,   5, 6, 7,  8,  9, 10,   11,  12, 13,    14,  15, 16,17,18,19, 20,  21,  22, 23,  24,  25,26,27,28
int EUROCODE_MC_SLSLC[]={0,0,16,8, 8+16,0,8+16,16, 8, 8+16, 8+16, 16, 8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //13

int EUROCODE_SLSLC_NO=13;

COMBINATION EUROCODE_QPSLSLC[]={{F1,0},{F1+F2,0},{F1+F2+16,0},{F1+F2+8,0},{F1+F2+16+8,0},{F1+16,0},{F1+8,0},{F1+16+8,0},{0,0},{0,0},
                               {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
                               {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};  //8
//                         1,2, 3,4, 5,  6, 7,  8,
int EUROCODE_MC_QPSLSLC[]={0,0,16,8,8+16,16,8, 8+16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //8

int EUROCODE_QPSLSLC_NO=8;

//-------------------------------------------------
//flags
//DL  D=1
//LL  L=2
//Lr  l=4
//WL  W=8
//SL  S=16
//EL  E=32
//RL  R=64
//HL  H=2
//FL  F=1
//TL  T=2

//D=DL+FL   D=D+F
//L=LL+HL+TL  L=L+H+T
COMBINATION ASCE_ULSLC[]={{1,0},{1+2+4,0},{1+2+16,0},{1+2+64,0},{1+2+4,0},{1+2+16,0},{1+2+64,0},{1+4+8,0},{1+8+16,0},{1+8+64,0},
                         {1+2+4+8,0},{1+2+8+16,0},{1+2+8+64,0},{1+2+16+32,0},{1+8,0},{1+32,0},{0,0},{0,0},{0,0},{0,0},
                         {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};  //16

//                   1,2, 3, 4,5,6, 7, 8,9, 10,11,12,13,14,  15, 16,17,18,19, 20,  21,  22, 23,  24,  25,26,27,28
int ASCE_MC_ULSLC[]={0,0,16,64,0,16,64,0,16,64,0, 16,64, 0,   0,  0,0,0,0,0,0,0,0,0,0,0,0,0};  //16

int ASCE_ULSLC_NO=16;         //FLC  (factored loads combination)

COMBINATION ASCE_SLSLC[]={{1,0},{1+2,0},{1+4,0},{1+16,0},{1+64,0},{1+2+4,0},{1+2+16,0},{1+2+64,0},{1+8,0},{1+32,0},
                         {1+2+4+8,0},{1+2+8+16,0},{1+2+8+64,0},{1+2+16+32,0},{1+8,0},{1+32,0},{0,0},{0,0},{0,0},{0,0},
                         {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};  //16
//                   1,2,3,4, 5, 6, 7,8, 9,10,11,12,  13,  14,15,16,17,18,19, 20,  21,  22, 23,  24,  25,26,27,28
int ASCE_MC_SLSLC[]={0,0,4,16,64,0,16,64,8,32,8, 8+16,8+64,32, 0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //16

int ASCE_SLSLC_NO=16;        //NL  (nominal loads combination)

COMBINATION ASCE_QPSLSLC[]={{1+2+8,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
                                {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
                                {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};  //1

//                     1,2, 3,4, 5,6, 7,    8,  9,  10, 11,  12, 13,    14,  15, 16,17,18,19, 20,  21,  22, 23,  24,  25,26,27,28
int ASCE_MC_QPSLSLC[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //1

int ASCE_QPSLSLC_NO=1;      //WDLC   (Wind Drift Load Combinations)

//----------------------------------------
//flags
//DL  D=1
//LL  L=2
//Lr  l=4
//WL  W=8
//SL  S=16
//EL  E=32
//RL  R=64
//HL  H=128
//FL  F=256
//TL  T=2

COMBINATION ICC_ULSLC[]={{1+256,0},{1+2+4+128+256,0},{1+2+16+128+256,0},{1+2+64+128+256,0},{1+2+4+128+256,0},{1+2+16+128+256,0},{1+2+64+128+256,0},{1+4+8+128+256,0},{1+8+16+128+256,0},{1+8+64+128+256,0},
                          {1+2+4+8+128+256,0},{1+2+8+16+128+256,0},{1+2+8+64+128+256,0},{1+2+16+32+128+256,0},{1+8+128,0},{1+32+128+256,0},{0,0},{0,0},{0,0},{0,0},
                          {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};  //16

//                     1,2, 3, 4,5,6, 7, 8,9, 10,11,12,13,14,15, 16,17,18,19, 20,  21,  22, 23,  24,  25,26,27,28
int    ICC_MC_ULSLC[]={0,0,16,64,0,16,64,0,16,64,0, 16,64,0, 0,  0,0,0,0,0,0,0,0,0,0,0,0,0};  //16

int ICC_ULSLC_NO=16;      //FLC  (factored loads combination)

COMBINATION ICC_SLSLC[]={{1+256,0},{1+2+128+256,0},{1+4+128+256,0},{1+16+128+256,0},{1+64+128+256,0},{1+2+4+128+256,0},{1+2+16+128+256,0},{1+2+64+128+256,0},{1+8+128+256,0},{1+32+128+256,0},
                          {1+2+4+8+128+256,0},{1+2+8+16+128+256,0},{1+2+8+64+128+256,0},{1+2+16+32+128+256,0},{1+8+128,0},{1+32+128+256,0},{0,0},{0,0},{0,0},{0,0},
                          {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};  //16

//                     1,2,3,4, 5, 6, 7,8, 9,10,11,12,  13,  14,15,16,17,18,19, 20,  21,  22, 23,  24,  25,26,27,28
int  ICC_MC_SLSLC[]={0,0,4,16,64,0,16,64,8,32,8, 8+16,8+64,32, 0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //16

int ICC_SLSLC_NO=16;     //BLC   (Basic load combinations)

COMBINATION ICC_QPSLSLC[]={{1+2+4,0},{1+2+16,0},{1+2+64,0},{1+2+8,0},{1+2+8+16,0},{1+2+8+16,0},{1+2+16+32,0},{1+32,0},{0,0},{0,0},
                            {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
                            {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};  //8

//                    1, 2,3, 4, 5,   6,  7, 8, 9,10,11,12,  13,  14,15,16,17,18,19, 20,  21,  22, 23,  24,  25,26,27,28
int ICC_MC_QPSLSLC[]={0,16,64,0,8+16,8+16,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //8

int ICC_QPSLSLC_NO=8;    //ABLC  (Alternative basic load combination)
        // D L l W S E R H F T
int eurocode_combi_factors_uls[28][11]= {
        {1,1,0,0,0,0,0,0,0,1,0},
        {2,1,1,0,0,0,1,0,1,1,1},
        {3,1,1,0,0,3,1,0,1,1,1},
        {4,1,3,0,0,1,3,0,3,1,3},
        {5,1,3,0,0,0,3,1,3,1,3},
        {6,1,3,1,0,0,3,0,3,1,3},
        {7,1,1,1,3,3,1,0,1,1,1},
        {8,1,1,1,3,0,1,3,1,1,1},
        {9,1,1,3,3,0,1,0,1,1,1},
        {10,1,3,0,3,1,3,0,3,1,3},

        {11,1,3,0,3,0,3,1,3,1,3},
        {12,1,3,1,3,0,3,0,3,1,3},
        {13,1,3,0,1,3,3,0,3,1,3},
        {14,1,3,0,1,0,3,3,3,1,3},
        {15,1,3,3,1,0,3,0,3,1,3},
        {16,1,0,0,0,1,0,0,0,1,0},
        {17,1,0,0,0,0,0,1,0,1,0},
        {18,1,0,1,0,0,0,0,0,1,0},
        {19,1,0,0,1,0,0,0,0,1,0},
        {20,1,0,0,3,1,0,0,0,1,0},

        {21,1,0,0,3,0,0,1,0,1,0},
        {22,1,0,1,3,0,0,0,0,1,0},
        {23,1,0,0,1,3,0,0,0,1,0},
        {24,1,0,1,1,0,0,3,0,1,0},
        {25,1,0,3,1,0,0,0,0,1,0},
        {26,1,1,0,3,0,1,0,1,1,1},
        {27,1,3,0,1,0,3,0,3,1,3},
        {28,32,0,0,1,0,0,0,0,32,0}
};

int eurocode_combi_factors_sls[13][11]= {
        //  D L l W S E R H F T
        {1, 0,0,0,0,0,0,0,0,0,0},
        {2, 0,0,0,0,0,0,0,0,0,0},
        {3, 0,0,0,0,2,0,0,0,0,0},
        {4, 0,0,0,2,0,0,0,0,0,0},
        {5, 0,0,0,2,2,0,0,0,0,0},
        {6, 0,2,0,2,0,2,0,2,0,2},
        {7, 0,2,0,0,2,2,0,2,0,2},
        {8, 0,0,0,0,0,0,0,0,0,0},
        {9, 0,0,0,0,0,0,0,0,0,0},
        {10, 0,0,0,2,0,0,0,0,0,0},
        {11, 0,0,0,0,2,0,0,0,0,0},
        {12, 0,2,0,0,0,2,0,2,0,2},
        {13, 0,2,0,0,0,2,0,2,0,2}
};

int eurocode_combi_factors_qpsls[8][11]= {
        //  D L l W S E R H F T
        {1, 0,0,0,0,0,0,0,0,0,0},
        {2, 0,8,0,0,0,8,0,8,0,8},
        {3, 0,8,0,0,8,8,0,8,0,8},
        {4, 0,8,0,8,0,8,0,8,0,8},
        {5, 0,8,0,8,8,8,0,8,0,8},
        {6, 0,0,0,0,8,0,0,0,0,0},
        {7, 0,0,0,8,0,0,0,0,0,0},
        {8, 0,0,0,8,8,0,0,0,0,0},
};

/*
if (combi_factor&1) gamma_g*=gammas->gamma_g;
if (combi_factor&2) gamma_g*=gammas->psi0;
if (combi_factor&4) gamma_g*=gammas->psi1;
if (combi_factor&8) gamma_g*=gammas->psi2;
if (combi_factor&16) gamma_g*=gammas->xi_g;
if (combi_factor&32) gamma_g*=gammas->gamma_gi;
 */
//           D L l W S E R H  F T          γ=1  Ψ0=2  Ψ1=4   Ψ2=8  ξ=16   γ.inf=32
int asce_combi_factors_uls[16][11]= {
         {1, 1,0,0,0,0,0,0,0, 1,0},

         {2,16,1,2,0,0,0,0,1,16,1},
         {3,16,1,0,0,2,0,0,1,16,1},
         {4,16,1,0,0,0,0,2,1,16,1},

         {5,16,0,1,0,0,0,0,0,16,0},
         {6,16,0,0,0,1,0,0,0,16,0},
         {7,16,0,0,0,0,0,1,0,16,0},

         {8,16,0,1,2,0,0,0,0,16,0},
         {9,16,0,0,2,1,0,0,0,16,0},
        {10,16,0,0,2,0,0,1,0,16,0},

        {11,16,0,2,0,0,0,0,0,16,0},
        {12,16,0,0,0,2,0,0,0,16,0},
        {13,16,0,0,0,0,0,2,0,16,0},

        {14,16,0,0,0,4,0,0,0,16,0},

        {15,2,0,0,0,0,0,0,0,2,0},
        {16,2,0,0,0,0,0,0,0,2,0},
};

int asce_combi_factors_sls[16][11]= {
         //  D L l W S E R H F T
         {1, 0,0,0,0,0,0,0,0,0,0},
         {2, 0,0,0,0,0,0,0,0,0,0},
         {3, 0,0,0,0,0,0,0,0,0,0},
         {4, 0,0,0,0,0,0,0,0,0,0},
         {5, 0,0,0,0,0,0,0,0,0,0},
         {6, 0,8,8,0,0,0,0,8,0,8},
         {7, 0,8,0,0,8,0,0,8,0,8},
         {8, 0,8,0,0,0,0,8,8,0,8},
         {9, 0,0,0,8,0,0,0,0,0,0},
        {10, 0,0,0,0,0,8,0,0,0,0},
        {11, 0,8,8,12,0,0,0,8,0,8},
        {12, 0,8,0,12,8,0,0,8,0,8},
        {13, 0,8,0,12,0,0,8,8,0,8},
        {14, 0,8,0,0,8,12,0,8,0,8},
        {15, 4,0,0,8,0,0,0,0,4,0},
        {16, 4,0,0,0,0,8,0,0,4,0}
};

int asce_combi_factors_qpsls[1][11]= {
        //  D L l W S E R H F T
        {1, 0,4,0,4,0,0,0,0,0,0},
};
//----------------------------------
int icc_combi_factors_uls[16][11]= {
       //    D L l W S E R H F T
         {1, 1,0,0,0,0,0,0,0, 1,0},  //1

         {2,16,1,2,0,0,0,0,1,16,1},  //2
         {3,16,1,0,0,2,0,0,1,16,1},
         {4,16,1,0,0,0,0,2,1,16,1},

         {5,16,2,1,0,0,0,0,1,16,2},  //3
         {6,16,2,0,0,1,0,0,1,16,2},
         {7,16,2,0,0,0,0,1,1,16,2},

         {8,16,0,1,2,0,0,0,1,16,0},
         {9,16,0,0,2,1,0,0,1,16,0},
        {10,16,0,0,2,0,0,1,1,16,0},

        {11,16,2,2,0,0,0,0,1,16,2},  //4
        {12,16,2,0,0,2,0,0,1,16,2},
        {13,16,2,0,0,0,0,2,1,16,2},

        {14,16,2,0,0,4,0,0,1,16,2},  //5

        {15,2,0,0,0,0,0,0,1,0,0},   //6
        {16,2,0,0,0,0,0,0,1,2,0},  //7
};

int icc_combi_factors_sls[16][11]= {
        //  D L l W S E R H F T
        {1, 0,0,0,0,0,0,0,0,0,0},
        {2, 0,0,0,0,0,0,0,0,0,0},
        {3, 0,0,0,0,0,0,0,0,0,0},
        {4, 0,0,0,0,0,0,0,0,0,0},
        {5, 0,0,0,0,0,0,0,0,0,0},
        {6, 0,8,8,0,0,0,0,0,0,8},
        {7, 0,8,0,0,8,0,0,0,0,8},
        {8, 0,8,0,0,0,0,8,0,0,8},
        {9, 0,0,0,8,0,0,0,0,0,0},
        {10, 0,0,0,0,0,8,0,0,0,0},
        {11, 0,8,8,12,0,0,0,0,0,8},
        {12, 0,8,0,12,8,0,0,0,0,8},
        {13, 0,8,0,12,0,0,8,0,0,8},
        {14, 0,8,0,0,8,12,0,0,0,8},
        {15, 8,0,0,8,0,0,0,0,0,0},
        {16, 8,0,0,0,0,8,0,0,8,0}
};

int icc_combi_factors_qpsls[8][11]= {
        //  D L l W S E R H F T
        {1, 0,0,0,0,0,0,0,0,0,0},
        {2, 0,0,0,0,0,0,0,0,0,0},
        {3, 0,0,0,0,0,0,0,0,0,0},
        {4, 0,0,0,8,0,0,0,0,0,0},
        {5, 0,0,0,8,2,0,0,0,0,0},
        {6, 0,0,0,10,0,0,0,0,0,0},
        {7, 0,0,0,0,0,4,0,0,0,0},
        {8, 1,0,0,0,0,4,0,0,0,0},
};


////////////////
//                   DL,LL,Lr,WL,SL,EL,RL,HL,FL,TL
//for EUROCODE
int load_flag_EU[11]={0,1,2,4,8,16,2,64,2,1,2};
//for ASCE
int load_flag_ASCE[11]={0,1,2,4,8,16,32,64,2,1,2};
//for ICC
int load_flag_ICC[11]={0,1,2,4,8,16,32,64,128,256,2};

int load_flag_ALL[11]={0,1,2,4,8,16,32,64,128,256,512};

int *load_flag;

int copy(const char* in_path, const char* out_path){
    size_t n;
    FILE* in=NULL, * out=NULL;
    char* buf = calloc(BUF_SIZE, 1);
    if((in = fopen(in_path, "rb")) && (out = fopen(out_path, "wb")))
        while((n = fread(buf, 1, BUF_SIZE, in)) && fwrite(buf, 1, n, out));
    free(buf);
    if(in) fclose(in);
    if(out) fclose(out);
    return EXIT_SUCCESS;
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

    if (NULL == (buf_block = (BLOK*)malloc(sizeof(NAGLOWEK) + size_block)))
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
    ptrs_desc_bl->x = x;
    ptrs_desc_bl->y = y;
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

void add_node_pre(void)
{
    st_node_pre_no++;
    if (st_node_pre_no==ST_NODE_PRE_MAX) {
        ST_NODE_PRE_MAX+=100;
        st_node_pre=realloc(st_node_pre, ST_NODE_PRE_MAX * sizeof(ST_NODE_PRE));
    }
    st_node_pre[st_node_pre_no].radius=0;
    st_node_pre[st_node_pre_no].flag=0;
}

void add_node(void)
{
    st_node_no++;
    if (st_node_no==ST_NODE_MAX) {
        ST_NODE_MAX+=100;
        st_node=realloc(st_node, ST_NODE_MAX * sizeof(ST_NODE));
    }
    st_node[st_node_no].radius=Rn;
}

void add_element(void)
{
    st_element_no++;
    if (st_element_no==ST_ELEMENT_MAX) {
        ST_ELEMENT_MAX+=100;
        st_element=realloc(st_element, ST_ELEMENT_MAX * sizeof(ST_ELEMENT));
    }
}

void add_property(void)
{
    st_property_no++;
    if (st_element_no==ST_PROPERTY_MAX) {
        ST_PROPERTY_MAX+=100;
        st_property=realloc(st_property, ST_PROPERTY_MAX * sizeof(ST_PROPERTY));
    }
    memmove(&st_property[st_property_no], &prt_def, sizeof(ST_PROPERTY));
}

void add_node_force_moment(void)
{
    st_node_force_moment_no++;
    if (st_node_force_moment_no==ST_NODE_FORCE_MOMENT_MAX) {
        ST_NODE_FORCE_MOMENT_MAX+=100;
        st_node_force_moment=realloc(st_node_force_moment, ST_NODE_FORCE_MOMENT_MAX * sizeof(ST_NODE_FORCE_MOMENT));
    }
    st_node_force_moment[st_node_force_moment_no].layer=-1;
    st_node_force_moment[st_node_force_moment_no].node=-1;
    st_node_force_moment[st_node_force_moment_no].fy=0;
    st_node_force_moment[st_node_force_moment_no].fy=0;
    st_node_force_moment[st_node_force_moment_no].mz=0;
    st_node_force_moment[st_node_force_moment_no].factor_record=-1;
    st_node_force_moment[st_node_force_moment_no].take_it=0;
}

void add_displacement(void)
{
    st_displacement_no++;
    if (st_displacement_no==ST_DISPLACEMENT_MAX)
    {
        ST_DISPLACEMENT_MAX+=100;
        st_displacement=realloc(st_displacement, ST_DISPLACEMENT_MAX * sizeof(ST_DISPLACEMENT));
    }
    st_displacement[st_displacement_no].layer=-1;
    st_displacement[st_displacement_no].node=-1;
    st_displacement[st_displacement_no].dx=0;
    st_displacement[st_displacement_no].dy=0;
    st_displacement[st_displacement_no].dzz=0;
    st_displacement[st_displacement_no].factor_record=-1;
    st_displacement[st_displacement_no].take_it=1;
}

void add_reaction(void)
{
    st_reaction_no++;
    if (st_reaction_no==ST_REACTION_MAX) {
        ST_REACTION_MAX+=100;
        st_reaction=realloc(st_reaction, ST_REACTION_MAX * sizeof(ST_REACTION));
    }
    st_reaction[st_reaction_no].node=-1;
    st_reaction[st_reaction_no].active=0;
    st_reaction[st_reaction_no].x=0;
    st_reaction[st_reaction_no].y=0;
    st_reaction[st_reaction_no].z=1;
    st_reaction[st_reaction_no].xx=0;
    st_reaction[st_reaction_no].yy=0;
    st_reaction[st_reaction_no].zz=0;
}

void add_element_force(void)
{
    st_element_force_no++;
    if (st_element_force_no==ST_ELEMENT_FORCE_MAX) {
        ST_ELEMENT_FORCE_MAX+=100;
        st_element_force=realloc(st_element_force, ST_ELEMENT_FORCE_MAX * sizeof(ST_ELEMENT_FORCE));
    }
    st_element_force[st_element_force_no].layer=-1;
    st_element_force[st_element_force_no].element=-1;
    st_element_force[st_element_force_no].fx=0;
    st_element_force[st_element_force_no].fy=0;
    st_element_force[st_element_force_no].dlx=0;
    st_element_force[st_element_force_no].factor_record=-1;
    st_element_force[st_element_force_no].take_it=0;
}

void add_thermal_load(void)
{
    st_thermal_load_no++;
    if (st_thermal_load_no==ST_THERMAL_LOAD_MAX)
    {
        ST_THERMAL_LOAD_MAX+=100;
        st_thermal_load=realloc(st_thermal_load, ST_THERMAL_LOAD_MAX * sizeof(ST_THERMAL_LOAD));
    }

    st_thermal_load[st_thermal_load_no].layer=-1;
    st_thermal_load[st_thermal_load_no].element=-1;
    st_thermal_load[st_thermal_load_no].ydepth=0;
    st_thermal_load[st_thermal_load_no].deltatplus=0;
    st_thermal_load[st_thermal_load_no].deltatminus=0;
    st_thermal_load[st_thermal_load_no].factor_record=-1;
    st_thermal_load[st_thermal_load_no].take_it=0;
}

void add_uniform_load(void)
{
    st_uniform_load_no++;
    if (st_uniform_load_no==ST_UNIFORM_LOAD_MAX)
    {
        ST_UNIFORM_LOAD_MAX+=100;
        st_uniform_load=realloc(st_uniform_load, ST_UNIFORM_LOAD_MAX * sizeof(ST_UNIFORM_LOAD));
    }

    st_uniform_load[st_uniform_load_no].layer=-1;
    st_uniform_load[st_uniform_load_no].element=-1;
    st_uniform_load[st_uniform_load_no].qx=0;
    st_uniform_load[st_uniform_load_no].qy=0;
    st_thermal_load[st_thermal_load_no].factor_record=-1;
    st_thermal_load[st_thermal_load_no].take_it=0;
}

void add_trapezoid_load(void)
{
    st_trapezoid_load_no++;
    if (st_trapezoid_load_no==ST_TRAPEZOID_LOAD_MAX)
    {
        ST_TRAPEZOID_LOAD_MAX+=100;
        st_trapezoid_load=realloc(st_trapezoid_load, ST_TRAPEZOID_LOAD_MAX * sizeof(ST_TRAPEZOID_LOAD));
    }

    st_trapezoid_load[st_trapezoid_load_no].layer=-1;
    st_trapezoid_load[st_trapezoid_load_no].element=-1;
    st_trapezoid_load[st_trapezoid_load_no].qx1=0;
    st_trapezoid_load[st_trapezoid_load_no].qy1=0;
    st_trapezoid_load[st_trapezoid_load_no].qx2=0;
    st_trapezoid_load[st_trapezoid_load_no].qy2=0;
    st_trapezoid_load[st_trapezoid_load_no].dlxx1=0;
    st_trapezoid_load[st_trapezoid_load_no].dlxy1=0;
    st_trapezoid_load[st_trapezoid_load_no].dlxx2=0;
    st_trapezoid_load[st_trapezoid_load_no].dlxy2=0;
    st_trapezoid_load[st_trapezoid_load_no].factor_record=-1;
    st_trapezoid_load[st_trapezoid_load_no].take_it=0;
}

void add_load(void)
{
    st_load_no++;
    if (st_load_no==ST_LOAD_MAX)
    {
        ST_LOAD_MAX+=100;
        st_load=realloc(st_load, ST_LOAD_MAX * sizeof(ST_LOAD));
    }

    st_load[st_load_no].element=-1;
    st_load[st_load_no].uniform=0;
    st_load[st_load_no].spread=0;
    st_load[st_load_no].partial=0;
    st_load[st_load_no].reversed=0;
    st_load[st_load_no].magnitude1=0.0;
    st_load[st_load_no].magnitude2=0.0;
    st_load[st_load_no].distance1=0.0;
    st_load[st_load_no].distance2=0.0;
    st_load[st_load_no].factor_record=-1;
    st_load[st_load_no].take_it=0;
}


void add_mass_node(void)
{
    st_mass_nodes_no++;
    if (st_mass_nodes_no == ST_MASS_NODE_MAX) {
        ST_MASS_NODE_MAX += 100;
        st_mass_node = realloc(st_mass_node, ST_MASS_NODE_MAX * sizeof(ST_MASS_NODE));
    }
}

void add_mass_element(void)
{
    st_mass_elements_no++;
    if (st_mass_elements_no == ST_MASS_ELEMENT_MAX) {
        ST_MASS_ELEMENT_MAX += 100;
        st_mass_element = realloc(st_mass_element, ST_MASS_ELEMENT_MAX * sizeof(ST_MASS_ELEMENT));
    }
}

void add_load_factors(void)
{
    st_load_factors_no++;
    if (st_load_factors_no==ST_LOAD_FACTORS_MAX)
    {
        ST_LOAD_FACTORS_MAX+=100;
        st_load_factors=realloc(st_load_factors, ST_LOAD_FACTORS_MAX * sizeof(ST_LOAD_FACTORS));
    }

    memmove(&st_load_factors[st_load_factors_no], &load_factors[0], sizeof(ST_LOAD_FACTORS));

}


void get_xy_margins(LINIA *Le, double *x1, double *y1, double *x2, double *y2)
{
    PLINIA PL;
    double koc, kos;

    parametry_lini(Le, &PL);
    kos = sin(PL.kat * Pi / 180);
    koc = cos(PL.kat * Pi / 180);

    *x1 = Le->x1 + zmwym.wysokosc * 0.5 * koc;
    *y1 = Le->y1 + zmwym.wysokosc * 0.5 * kos;
    *x2 = Le->x2 - zmwym.wysokosc * 0.5 * koc;
    *y2 = Le->y2 - zmwym.wysokosc * 0.5 * kos;
}


int draw_label(LINIA *L, LINIA *Le, double dx, double r1, double r2, double vpar, double precision, int bold)
{
    TEXT T=Tdef;
    double x, y;
    PLINIA PL, PL1;
    char *ptr_t;
    double x1, y1, x2, y2;
    double kos, koc, dlx;
    double lb, le;

    parametry_lini(L, &PL);
    parametry_lini(Le, &PL1);
    dlx=jednostkiOb(dx);

    lb=jednostkiOb(r1);
    le=PL1.dl-jednostkiOb(r2);

    set_decimal_format(T.text, vpar, precision);

    if ((strcmp(T.text,"0")==0) || (strcmp(T.text,"-0")==0)) return 0;

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

int draw_disp_label(LINIA *L, LINIA *Le, double dx, double vpar1, double vpar2, double precision, int bold)
{
    TEXT T=Tdef;
    double x, y;
    PLINIA PL, PL1;
    char *ptr_t;
    double x1, y1, x2, y2;
    double kos, koc;
    char t1[MaxTextLen]="", t2[MaxTextLen]="";
    char term[2]="";
    double Dxy;

    parametry_lini(L, &PL);

    if (!Check_if_Equal2(vpar1, 0.0))
        set_decimal_format(t1, vpar1, precision);
    if (!Check_if_Equal2(vpar2, 0.0))
        set_decimal_format(t2, vpar2, precision);

    if ((strcmp(t1,"0")==0) || (strcmp(t1,"-0")==0)) strcpy(t1,"");
    if ((strcmp(t2,"0")==0) || (strcmp(t2,"-0")==0)) strcpy(t2,"");

    if ((strlen(t1)>0) && (strlen(t2)>0)) strcpy(term,",");

    if ((strlen(t1)==0) && (strlen(t2)==0))  return 0;


    Dxy = sqrt(vpar1 * vpar1 + vpar2 * vpar2);

    //set_decimal_format(T.text, vpar, precision);
    sprintf(&T.text, "%s%s%s", t1, term, t2);

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
        if (Check_if_Equal2(dx, PL1.dl))
            T.kat = (Pi_ * (PL.kat+180) / 180);
        else
            T.kat = Pi_ * PL.kat / 180;
        T.blok=1;
        T.dl = strlen(T.text);
        T.n = T18 + T.dl;
        normalize_txt(&T);
        ptr_t = dodaj_obiekt((BLOK *) dane, (void *) &T);
        strcpy(T_text, T.text);

        return 1;
    }
    return 0;
}

int draw_geo_label(LINIA *L, int no, BOOL italics, BOOL underline, int visible)
{
    TEXT T=Tdef;
    double x, y;
    PLINIA PL;
    char *ptr_t;
    int psize;

    if ((Check_if_Equal(L->x1, L->x2)) && (Check_if_Equal(L->y1, L->y2)))
    {
        PL.kat=0;
        psize=Get_Point_Size()/2.0;
        T.x = L->x1 + psize;
        T.y = L->y1 + psize;
    }
    else
    {
        parametry_lini(L, &PL);
        T.x = L->x1 + ((L->x2 - L->x1) / 3);
        T.y = L->y1 + ((L->y2 - L->y1) / 3);
    }

    sprintf(T.text,"%d", no);
    T.justowanie = j_srodkowo;
    T.warstwa = L->warstwa;
    T.kolor = L->kolor;
    T.czcionka = zmwym.czcionka;
    T.wysokosc = zmwym.wysokosc * 0.5;
    T.italics = italics;
    T.underline = underline;
    T.ukryty = !visible;
    T.kat = Pi_ * PL.kat / 180;
    T.dl = strlen(T.text);
    T.n = T18 + T.dl;
    T.blok=1;
    normalize_txt(&T);
    ptr_t = dodaj_obiekt((BLOK *) dane, (void *) &T);

    return 1;
}

#define arrowf 1.0

void draw_reaction_xy(LINIA *L, ST_NODE *node, double Fxy, int axis)
{  LINIA L1=Ldef, Ls=Ldef, L2=Ldef;
    WIELOKAT w=S4def;
    double ra;
    char *ptr;
    double katS=25.0;
    PLINIA PL;
    double kat1, Kp2s, koc, kos, koc1, kos1, koc2, kos2;
    TEXT Rtxt=Tdef;
    double dx, dy;

    ra=Get_Point_Size () / arrowf;

    if (axis==0)  //Fx
    {
        dx=Fxy / r_magnitude;
        if (fabs(dx)<(1.5*ra))
        {
            dx=1.5*ra* copysign(1, Fxy);
            if (Fxy>0) Rtxt.justowanie=j_do_prawej;
            else Rtxt.justowanie=j_do_lewej;
        }
        else
        {
            Rtxt.justowanie=j_srodkowo;
        }
        L1.x1 = node->x;
        L1.y1 = node->y;
        L1.x2 = L1.x1 - dx;
        L1.y2 = L1.y1;

        if (Rtxt.justowanie==j_srodkowo)
        {
            Rtxt.x = (L1.x1 + L1.x2) / 2 - ra / 4;
            Rtxt.y = (L1.y1 + L1.y2) / 2 + 0.5;
        }
        else
        {
            Rtxt.x = L1.x2 - copysign(1, Fxy);
            Rtxt.y = L1.y2 - zmwym.wysokosc*0.5*0.5;
        }

        Rtxt.kat=0;
    }
    else
    {
        dy=Fxy / r_magnitude;
        if (fabs(dy)<(1.5*ra))
        {
            dy=1.5*ra* copysign(1, Fxy);
            if (Fxy>0) Rtxt.justowanie=j_do_prawej;
            else Rtxt.justowanie=j_do_lewej;
        }
        else
        {
            Rtxt.justowanie=j_srodkowo;
        }

        L1.x1 = node->x;
        L1.y1 = node->y;
        L1.x2 = L1.x1;
        L1.y2 = L1.y1 - dy;

        if (Rtxt.justowanie==j_srodkowo)
        {
            Rtxt.x = (L1.x1 + L1.x2) / 2 - ra / 4;
            Rtxt.y = (L1.y1 + L1.y2) / 2;
        }
        else
        {
            Rtxt.x = L1.x2 + zmwym.wysokosc*0.5*0.5;
            Rtxt.y = L1.y2 - copysign(1, Fxy);
        }

        Rtxt.kat=Pi_/2;
    }

    parametry_lini(&L1, &PL);
    kat1=PL.kat;

    koc=cos(Pi*(kat1)/180);
    kos=sin(Pi*(kat1)/180);

    L2.x1 = L1.x1 + ra * koc;
    L2.y1 = L1.y1 + ra * kos;
    L2.x2 = L1.x2 - ra * koc;
    L2.y2 = L1.y2 - ra * kos;

    w.warstwa=L->warstwa;
    w.kolor=L->kolor;

    Ls.x2=L1.x2;
    Ls.y2=L1.y2;
    Ls.x1=L2.x1;
    Ls.y1=L2.y1;

    w.blok=1;

    create_solid_on_line (&Ls, &w, ra/4, ra/4, 0);
    ptr = dodaj_obiekt((BLOK *) dane, (void *) &w);

    Kp2s = Get_Point_Size () / (arrowf*cos(Pi*katS/180)) ;

    parametry_lini(&Ls, &PL);
    kat1=PL.kat;

    koc1=cos(Pi*(kat1-katS)/180);
    koc2=cos(Pi*(kat1+katS)/180);
    kos1=sin(Pi*(kat1-katS)/180);
    kos2=sin(Pi*(kat1+katS)/180);

    w.xy[2]=L1.x1;
    w.xy[3]=L1.y1;
    w.xy[0]=L1.x1+Kp2s*koc1;
    w.xy[1]=L1.y1+Kp2s*kos1;
    w.xy[4]=L1.x1+Kp2s*koc2;
    w.xy[5]=L1.y1+Kp2s*kos2;

    w.lp=6;
    w.n=32;

    w.blok=1;

    ptr = dodaj_obiekt((BLOK *) dane, (void *) &w);

    Rtxt.warstwa=L->warstwa;
    Rtxt.kolor=L->kolor;
    Rtxt.czcionka=zmwym.czcionka;
    Rtxt.wysokosc=zmwym.wysokosc*0.5;
    Rtxt.width_factor=zmwym.width_factor;
    Rtxt.bold=1;
    set_decimal_format(Rtxt.text, Fxy, r_precision);
    Rtxt.dl = strlen(Rtxt.text);
    Rtxt.n = T18 + Rtxt.dl;
    Rtxt.blok=1;
    normalize_txt(&Rtxt);

    ptr = dodaj_obiekt((BLOK *) dane, (void *) &Rtxt);
}

void moment2angle(SOLIDARC *sa, double m, double factor)
{
    double del;
    del=fabs(m)/factor;
    if (del>=Pi2*0.99) del=Pi2*0.99;
    if (m>0)
    {
        sa->kat1=0;
        sa->kat2=Angle_Normal(del);
    }
    else
    {
        sa->kat2=0;
        sa->kat1=Angle_Normal(-del);
    }
    return;
}

void draw_reaction_m(LINIA *L, ST_NODE *node, double Mzz)
{
    SOLIDARC sa=sadef;
    double ra;
    char *ptr;
    double kats, kat2;
    double xs, ys;
    double n;
    double Kp2s, koc1, kos1, koc2, kos2;
    double katS=25.0;
    WIELOKAT w=S4def;
    TEXT Rtxt=Tdef;
    double fix_r=2.5;

    ra=Get_Point_Size () / arrowf;

    sa.warstwa=L->warstwa;
    sa.kolor=L->kolor;
    sa.x=node->x;
    sa.y=node->y;
    sa.r=fix_r+ra*2;

    moment2angle(&sa, Mzz, rm_magnitude);

    if (sa.kat1==0) sa.kat2=max(sa.kat2, Pi_/16);  //8
    else if (sa.kat2==0) sa.kat1=min(sa.kat1, Pi2*0.9687);  // 0.9375

    sa.width1=ra/4;
    sa.width2=ra/4;
    sa.blok=1;

    ptr = dodaj_obiekt((BLOK *) dane, (void *) &sa);

    if (Mzz>0) {
        kats = sa.kat2 - Pi_ / 2;
        xs = node->x + sa.r * cos(sa.kat2);
        ys = node->y + sa.r * sin(sa.kat2);
        n = +1;
    }
    else
    {
        kats=sa.kat1+Pi_/2;
        xs=node->x+sa.r*cos(sa.kat1);
        ys=node->y+sa.r*sin(sa.kat1);
        n=-1;
    }

    double dx=Get_Point_Size () / arrowf * cos(kats);
    double dy=Get_Point_Size () / arrowf * sin(kats);

    xs-=dx;
    ys-=dy;

    Kp2s = Get_Point_Size () / (arrowf*cos(Pi*katS/180)) ;

    koc1=cos(kats-Pi*(katS)/180);
    koc2=cos(kats+Pi*(katS)/180);
    kos1=sin(kats-Pi*(katS)/180);
    kos2=sin(kats+Pi*(katS)/180);

    w.warstwa=L->warstwa;
    w.kolor=L->kolor;
    w.xy[2]=xs;
    w.xy[3]=ys;
    w.xy[0]=xs+Kp2s*koc1;
    w.xy[1]=ys+Kp2s*kos1;
    w.xy[4]=xs+Kp2s*koc2;
    w.xy[5]=ys+Kp2s*kos2;
    w.lp=6;
    w.n=32;
    w.empty_typ=0;
    w.blok=1;

    ptr = dodaj_obiekt((BLOK *) dane, (void *) &w);

    if (sa.kat2<sa.kat1) kat2=sa.kat2+Pi2;
    else kat2=sa.kat2;
    kats=Angle_Normal((sa.kat1+kat2)/2);
    Rtxt.x=sa.x+(sa.r+0.5)*cos(kats);
    Rtxt.y=sa.y+(sa.r+0.5)*sin(kats);
    Rtxt.kat=Angle_Normal(kats-Pi_/2);
    Rtxt.justowanie=j_srodkowo;
    Rtxt.warstwa=L->warstwa;
    Rtxt.kolor=L->kolor;
    Rtxt.czcionka=zmwym.czcionka;
    Rtxt.wysokosc=zmwym.wysokosc*0.5;
    Rtxt.width_factor=zmwym.width_factor;
    Rtxt.bold=1;
    Rtxt.blok=1;

    set_decimal_format(Rtxt.text, Mzz, rm_precision);
    Rtxt.dl = strlen(Rtxt.text);
    Rtxt.n = T18 + Rtxt.dl;
    normalize_txt(&Rtxt);

    ptr = dodaj_obiekt((BLOK *) dane, (void *) &Rtxt);
}

int qsort_by_number(unsigned char *e1, unsigned char *e2)
{ int delta;
    delta=(*e1) - (*e2);
    return delta;
}


double find_node_size(float x, float y)
{  int i;
    //searching for node
    for (i = 0; i < st_node_pre_no; i++)
    {
        if ((Check_if_Equal2(x, st_node_pre[i].x)) && (Check_if_Equal2(y, st_node_pre[i].y)))
        {
            //node size vector on the i node's position
            st_node_pre[i].flag=1;
            return st_node_pre[i].radius;
            break;
        }
    }
    if (i==st_node_pre_no) return Rn;
    return 0;
}

unsigned char str2load(char *ptr)
{
    char *loads[]={"", "%DL", "%LL", "%Lr", "%WL", "%SL", "%EL", "%RL", "%HL", "%FL", "%TL"};
    for (int i=1; i<11; i++)
    {
        if (strstr(ptr, loads[i])!=NULL) return (unsigned char)i;
    }
    return (unsigned char)0;
}

int factor_record(unsigned char load, unsigned char variant)
{
    for (int i=0; i<st_load_factors_no; i++)
    {
        if (st_load_factors[i].load==load)
        {
            if (variant>0)
            {
                if (st_load_factors[i].variant==variant)
                    return i;
            }
        }
    }
    return -1;
}

void consolidate_loads(ST_UNIFORM_LOAD *st_uniform_load_, int st_uniform_load__no, ST_UNIFORM_LOAD *st_uniform_load__cons, int *st_uniform_load__no_cons, unsigned int layer[8])
{  int element_no;
    double qx, qy;
    int uniform_load_no_cons;

    for (int i=0; i<st_uniform_load__no; i++) st_uniform_load_[i].flag=0;

    uniform_load_no_cons=0;

    for (int i=0; i<st_uniform_load__no; i++)
    {
        if (TestBit(layer, st_uniform_load_[i].layer))
        {
            qx = 0.0;
            qy = 0.0;
            element_no = st_uniform_load_[i].element;
            st_uniform_load__cons[uniform_load_no_cons].element = element_no;
            st_uniform_load__cons[uniform_load_no_cons].layer = st_uniform_load_[i].layer;
            for (int j = 0; j < st_uniform_load__no; j++) {
                if ((st_uniform_load_[j].flag == 0) && (st_uniform_load_[j].element == element_no) &&
                    (st_uniform_load_[j].take_it == 1) && (TestBit(layer, st_uniform_load_[j].layer))) {
                    st_uniform_load_[j].flag = 1;
                    qx += st_uniform_load_[j].qx;
                    qy += st_uniform_load_[j].qy;
                }
            }
            if (!Check_if_Equal(qx, 0.0) || !Check_if_Equal(qy, 0.0))
            {
                st_uniform_load__cons[uniform_load_no_cons].qx = qx;
                st_uniform_load__cons[uniform_load_no_cons].qy = qy;

                st_uniform_load__cons[uniform_load_no_cons].take_it = 1;
                st_uniform_load__cons[uniform_load_no_cons].factor_record = 0;
                st_uniform_load__cons[uniform_load_no_cons].flag = 0;

                uniform_load_no_cons++;
            }
        }
    }
    *st_uniform_load__no_cons=uniform_load_no_cons;
}

// function to find local extremum
void extrema(double a[], int n, int *c)
{
    //for (int i=0; i<n; i++) c[i]=0;

    for (int i = 1; i < n - 1; i++)
    {

        if (a[i] > a[i - 1] && a[i] > a[i + 1]) c[i]=1;

        if (a[i] < a[i - 1] && a[i] < a[i + 1]) c[i]=1;
    }
}

BOOL check_if_not_virtual(double x, PLINIA PL, int rep_element_no)
{
    if (((!Check_if_Equal(x, 0.0)) || (st_element[rep_element_no - 1].node1r != -1)) &&
        ((!Check_if_Equal(jednostkiOb(x), PL.dl)) || (st_element[rep_element_no - 1].node2r != -1)))
        return TRUE;
    else return FALSE;
}

void Static_analysis(void) {
    int ret, ret_standard;
    int key;
    NAGLOWEK *nag;
    AVECTOR *v;
    TEXT *t;
    LINIA L = Ldef;
    T_Point *p;
    int i, j, li;
    double x1, y1, x2, y2;
    PLINIA PL, PL1;
    double koc, kos;
    double df_node_size = 5;  //mm
    double df_node_radius = 1;
    char *ptr, *ptrs, *ptrsy, *ptr1, *ptr_max, *ptr_min, *ptr_row;
    double Ax, Asy, Asz, Iy, Iz, E, G, r, d, h;
    int property_no;
    char prop[MAXEXT];
    char report[MaxMultitextLen] = "";
    char report_row[MaxTextLen];
    double q1x, q1y, q2x, q2y, fx, fy, dx, dy, dlx1, dlx2;
    double partial_length;
    double n1, nm;
    FILE *f;
    char params[MAXPATH];
    DWORD runcode;
    short runcode_short;
    int runcode_int;
    int hinged;
    char *ptr_block;
    char block_name[MaxTextLen];
    double node_radius1, node_radius2;
    double gamma_l = 1.0, gamma_li = 1.0, xi_l = 1.0;
    int combinations_number, combination_no;
    GAMMAS gammas_EU = {1.35, 0, 0, 0, 0.85, 1.0};
    GAMMAS gammas_ASCE = {1.4, 0.9, 0.6, 0, 1.2, 0};
    GAMMAS gammas_ICC = {1.4, 0.9, 1.0, 0.6, 1.2, 0};
    GAMMAS *gammas;

    SPLINE Sdsp = Splinedef;

    BOOL SECOND_LINE = FALSE;

    int rep_element_no = 0;

    st_property_no = 0;
    st_node_pre_no = 0;
    st_node_no = 0;
    st_reaction_no = 0;
    st_element_no = 0;
    st_node_force_moment_no = 0;
    st_uniform_load_no = 0;
    st_trapezoid_load_no = 0;
    st_element_force_no = 0;
    st_thermal_load_no = 0;
    st_displacement_no = 0;
    st_load_no = 0;
    st_load_factors_no = 0;

    Rn = 0; //initialization of node radius;

    ST_NODE_MAX = 100;
    ST_REACTION_MAX = 100;
    ST_ELEMENT_MAX = 100;
    ST_NODE_FORCE_MOMENT_MAX = 100;
    ST_UNIFORM_LOAD_MAX = 100;
    ST_TRAPEZOID_LOAD_MAX = 100;
    ST_ELEMENT_FORCE_MAX = 100;
    ST_THERMAL_LOAD_MAX = 100;
    ST_DISPLACEMENT_MAX = 100;
    ST_LOAD_FACTORS_MAX = 100;

    st_property = (ST_PROPERTY *) malloc(ST_PROPERTY_MAX * sizeof(ST_PROPERTY));
    st_node_pre = (ST_NODE_PRE *) malloc(ST_NODE_PRE_MAX * sizeof(ST_NODE_PRE));
    st_node = (ST_NODE *) malloc(ST_NODE_MAX * sizeof(ST_NODE));
    st_reaction = (ST_REACTION *) malloc(ST_REACTION_MAX * sizeof(ST_REACTION));
    st_element = (ST_ELEMENT *) malloc(ST_ELEMENT_MAX * sizeof(ST_ELEMENT));
    st_node_force_moment = (ST_NODE_FORCE_MOMENT *) malloc(ST_NODE_FORCE_MOMENT_MAX * sizeof(ST_NODE_FORCE_MOMENT));
    st_uniform_load = (ST_UNIFORM_LOAD *) malloc(ST_UNIFORM_LOAD_MAX * sizeof(ST_UNIFORM_LOAD));
    st_trapezoid_load = (ST_TRAPEZOID_LOAD *) malloc(ST_TRAPEZOID_LOAD_MAX * sizeof(ST_TRAPEZOID_LOAD));
    st_element_force = (ST_ELEMENT_FORCE *) malloc(ST_ELEMENT_FORCE_MAX * sizeof(ST_ELEMENT_FORCE));
    st_thermal_load = (ST_THERMAL_LOAD *) malloc(ST_THERMAL_LOAD_MAX * sizeof(ST_THERMAL_LOAD));
    st_displacement = (ST_DISPLACEMENT *) malloc(ST_DISPLACEMENT_MAX * sizeof(ST_DISPLACEMENT));
    st_load = (ST_LOAD *) malloc(ST_LOAD_MAX * sizeof(ST_LOAD));
    st_load_factors = (ST_LOAD_FACTORS *) malloc(ST_LOAD_FACTORS_MAX * sizeof(ST_LOAD_FACTORS));

    st_node[st_node_no].radius = Rn;

    redcrsb(0, 167);
    select_blok();
    redcrsb(1, 167);

    if ((ADP == NULL) && (ADK == NULL)) return;

    gX = 0;
    gY = 0;
    gZ = 0;

    ret_standard = ask_question_static(8, _No_, _Yes_, "", _PROCEED_STATIC_, 12, "", 11, 1, 0, &combination_no,
                                       &geometric_tiffness, &inertia, &st_dynamic_no);
    //0 - rezygnuj; 1 - Eurocode, 2 - ASCE, 3 - ICC
    if (ret_standard > 0) key = _YES_;
    else if (ret_standard == 0) key = _NO_;
    else return;

    if (key != _YES_ && key != _yes_) {
        zmien_atrybut_undo(dane, dane + dane_size);
        blokzap(ADP, ADK, Ablok, COPY_PUT, 1);
        zmien_atrybut(ADP, ADK, Ablok, Aoblok);
        redcrsb(2, 167);
        return;
    }

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

    memmove(&st_load_factors[st_load_factors_no], &load_factors[0], sizeof(ST_LOAD_FACTORS));  //TEMPORARY for EUROCODE

    //STATIC ANALYSIS
    ////properties
    /////////////////////////
    if (Jednostki == 1)   //mm
    {
        units_factor = 1.0;  //ISO
        m_units_factor = 1000.0;
        axis_increment = 250.0;
        unit_factors = &unit_factors_si;
        prop_precisions = &SI_precisions;
    } else if (Jednostki == 10) //cm
    {
        units_factor = 10.0;
        m_units_factor = 100.0;
        axis_increment = 250.0;
        unit_factors = &unit_factors_si;
        prop_precisions = &SI_precisions;
    } else if (Jednostki == 1000)  //m
    {
        units_factor = 1000.0;
        m_units_factor = 1.0;
        axis_increment = 250.0;
        unit_factors = &unit_factors_si;
        prop_precisions = &SI_precisions;
    } else if (Jednostki == 1000000) //km
    {
        units_factor = 1000000.0;
        m_units_factor = 0.001;
        axis_increment = 250.0;
        unit_factors = &unit_factors_si;
        prop_precisions = &SI_precisions;
    } else if (Jednostki == 25.4) //"
    {
        units_factor = 1.0;  //imperial
        m_units_factor = 1.0;
        axis_increment = 10.0;
        unit_factors = &unit_factors_imp;
        prop_precisions = &IMP_precisions;
    } else if (Jednostki == 304.8) //'
    {
        units_factor = 12.0;  //inti inches
        m_units_factor = 1.0;
        axis_increment = 10.0;
        unit_factors = &unit_factors_imp;
        prop_precisions = &IMP_precisions;
    } else if (Jednostki == 914.4) //yd
    {
        units_factor = 36.0;  //into inches
        m_units_factor = 1.0;
        axis_increment = 10.0;
        unit_factors = &unit_factors_imp;
        prop_precisions = &IMP_precisions;
    } else if (Jednostki == 1609344) //mi
    {
        units_factor = 63360.0;  //into inches
        m_units_factor = 1.0;
        axis_increment = 10.0;
        unit_factors = &unit_factors_imp;
        prop_precisions = &IMP_precisions;
    } else {
        units_factor = 1.0;
        m_units_factor = 1000.0;
        axis_increment = 250.0;
        unit_factors = &unit_factors_si;
        prop_precisions = &SI_precisions;
    }

    df_node_size = 5 / Jednostki;  //5 mm
    /////////////////////////

    //searching for text to generate list of element properties
    //Ax           - Cross-sectional area of a prismatic frame element  in^2 or  mm^2
    //Asy          - Shear area in the local y-axis of a prismatic frame element in^2  or mm^2
    //Jy           - Torsional moment of inertia of a frame element  in^4 or  mm^4
    //Iy           - Moment of inertia for bending about the local y axis  in^4 or  mm^4
    //Iz           - Moment of inertia for bending about the local z axis  in^4 or  mm^4
    //Wy           - Elastic section modulus about y-axis in^3 or  mm^3  //not set in .3dd file
    //Wz           - Elastic section modulus about z-axis in^3 or  mm^3  //not set in .3dd file
    //E            - Modulus of elasticity of a frame element  ksi or  MPa
    //G            - Shear modulus of elasticity of frame element  ksi or  MPa
    //p            - the roll angle of the frame element, in degrees
    //d            - mass density of a frame element (using consistent units)   kip/in^3/g where the gravitational acceleration, g, is in inch/sec^2 or tonne/mm^3
    //a            - Coefficient of thermal expansion (1/degree)
    //h            - depth of the section in y axis

    strcpy(title_id, "FRAME");
    ptr_id = title_id;
    ptr_id_short = (char *) "";

    obiekt_tok((char *) ADP, ADK, (char **) &nag, Otekst);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            t = (TEXT *) nag;
            while (1) {
                ptr = strstr(t->text, _FRAME3DD_);  //title
                if (ptr == NULL) ptr = strstr(t->text, _FRAME3DD_PL);  //title
                if (ptr == NULL) ptr = strstr(t->text, _FRAME3DD_UA);  //title
                if (ptr == NULL) ptr = strstr(t->text, _FRAME3DD_ES);  //title
                if (ptr != NULL) {
                    strncpy(st_title, ptr + 1, MaxTextLen - 1);
                    strncpy(title_id, ptr + 1, MaxTextLen - 1);
                    ptr = strchr(title_id, ':');  //title_id
                    if (ptr != NULL) {
                        ptr++;
                        ptr_id_short = ptr;
                        while (*ptr == ' ') ptr++;
                        if (*ptr != '\0') {
                            while ((*ptr != ' ') && (*ptr != '\0')) ptr++;
                            if (*ptr != '\0') *ptr = '\0';
                        }
                    }
                    break; //end of string
                }

                memmove(&st_property[st_property_no], &prt_def, sizeof(ST_PROPERTY));
                //st_property[st_property_no].ok = 0;

                //gravitational acceleration

                //gammas->gamma_g=st_load_factors_EU_0

                ptr = strstr(t->text, "%g=");
                if (ptr == NULL) ptr = strstr(t->text, "%G=");
                if (ptr != NULL) {
                    gY = atof(ptr + 3);

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

                //default node radius
                ptr = strstr(t->text, "%rn=");
                if (ptr == NULL) ptr = strstr(t->text, "%Rn=");
                if (ptr != NULL) {
                    Rn = atof(ptr + 4);
                    for (int ii = 0; ii < st_node_no; ii++)
                        st_node[ii].radius = Rn;
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
                    st_load_factors[st_load_factors_no].load = str2load(ptr);

                    //"", LL", "LL", "Lr", "WL", "SL", "EL", "RL", "HL", "FL", "TL"};
                    memmove(&st_load_factors[st_load_factors_no],
                            &load_factors[st_load_factors[st_load_factors_no].load],
                            sizeof(ST_LOAD_FACTORS));  //temporary just for Eurocode

                    if (st_load_factors[st_load_factors_no].load > 0) {
                        st_load_factors[st_load_factors_no].variant = (unsigned char) atoi(ptr + 3);
                        ptr1 = strstr(ptr, "γ=");  //gamma
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "Γ=");  //Gamma
                        if (ptr1 != NULL) st_load_factors[st_load_factors_no].gamma = atof(ptr1 + 3);

                        ptr1 = strstr(ptr, "γi=");  //gamma inf
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "Γi=");  //Gamma inf
                        if (ptr1 != NULL) st_load_factors[st_load_factors_no].gamma_inf = atof(ptr1 + 4);

                        ptr1 = strstr(ptr, "Ψ0=");  //Psi
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "ψ0=");  //psi
                        if (ptr1 != NULL) st_load_factors[st_load_factors_no].psi0 = atof(ptr1 + 4);

                        ptr1 = strstr(ptr, "Ψ1=");  //Psi1
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "ψ1=");  //psi
                        if (ptr1 != NULL) st_load_factors[st_load_factors_no].psi1 = atof(ptr1 + 4);

                        ptr1 = strstr(ptr, "Ψ2=");  //Psi2
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "ψ2=");  //psi2
                        if (ptr1 != NULL) st_load_factors[st_load_factors_no].psi2 = atof(ptr1 + 4);

                        ptr1 = strstr(ptr, "ξ=");  //xi
                        if (ptr1 == NULL) ptr1 = strstr(ptr, "Ξ=");  //Xi
                        if (ptr1 != NULL) st_load_factors[st_load_factors_no].xi = atof(ptr1 + 3);

                        add_load_factors();
                    }
                    break; //end of string
                }

                ptr = strchr(t->text, '#');
                if (ptr == NULL) break;
                ptr1 = strchr(ptr + 1, ' ');
                if (ptr1 == NULL) break;
                strncpy(prop, ptr + 1, ptr1 - ptr - 1);
                prop[ptr1 - ptr - 1] = '\0';
                st_property[st_property_no].n = atoi(prop);

                ptr = strstr(t->text, "h=");
                if (ptr == NULL) break;
                st_property[st_property_no].h = atof(ptr + 2) * unit_factors->h_f;

                st_property[st_property_no].b = st_property[st_property_no].h;

                ptr = strstr(t->text, "b=");
                if (ptr != NULL) {
                    st_property[st_property_no].b = atof(ptr + 2) * unit_factors->h_f;
                }

                ptr = strstr(t->text, "A=");
                if (ptr == NULL) break;
                st_property[st_property_no].A = atof(ptr + 2) * unit_factors->A_f;

                ptrs = strstr(t->text, "As=");
                if (ptrs != NULL) {
                    st_property[st_property_no].Asy = atof(ptrs + 3) * unit_factors->A_f;
                    st_property[st_property_no].Asz =
                            st_property[st_property_no].Asy * 0.75;  //estimation, can be taken as exact value too
                }

                ptrsy = strstr(t->text, "Asy=");  //alternatively
                if (ptrsy != NULL) {
                    st_property[st_property_no].Asy = atof(ptrsy + 4) * unit_factors->A_f;
                    st_property[st_property_no].Asz =
                            st_property[st_property_no].Asy * 0.75;  //estimation, can be taken as exact value too
                }

                if ((ptrs == NULL) && (ptrsy == NULL)) break;

                ptr = strstr(t->text, "Asz=");
                if (ptr != NULL) {
                    st_property[st_property_no].Asz = atof(ptr + 4) * unit_factors->A_f;
                }

                ptr = strstr(t->text, "Iy=");
                if (ptr == NULL) break;

                st_property[st_property_no].Iy = atof(ptr + 3) * unit_factors->I_f;

                ptr = strstr(t->text, "Iz=");
                if (ptr == NULL) break;

                st_property[st_property_no].Iz = atof(ptr + 3) * unit_factors->I_f;

                ptr = strstr(t->text, "Wy=");
                if (ptr != NULL) {
                    st_property[st_property_no].Wy = atof(ptr + 3) * unit_factors->Wm_f;
                }

                ptr = strstr(t->text, "Wz=");
                if (ptr != NULL) {
                    st_property[st_property_no].Wz = atof(ptr + 3) * unit_factors->Wm_f;
                }

                st_property[st_property_no].Jx = 1.0;

                ptr = strstr(t->text, "Jx=");
                if (ptr != NULL) {

                    st_property[st_property_no].Jx = atof(ptr + 3) * unit_factors->I_f;
                }

                ptr = strstr(t->text, "E=");
                if (ptr == NULL) break;

                st_property[st_property_no].E = atof(ptr + 2) * unit_factors->E_f;

                ptr = strstr(t->text, "G=");
                if (ptr == NULL) break;

                st_property[st_property_no].G = atof(ptr + 2) * unit_factors->E_f;

                st_property[st_property_no].r = 0.0;

                ptr = strstr(t->text, "r=");
                if (ptr != NULL) {

                    st_property[st_property_no].r = atof(ptr + 2);
                }

                st_property[st_property_no].d = 0.0;

                ptr = strstr(t->text, "d=");
                if (ptr != NULL) {

                    st_property[st_property_no].d = atof(ptr + 2) * unit_factors->d_f;
                }

                st_property[st_property_no].a = 0.0;

                ptr = strstr(t->text, "a=");
                if (ptr != NULL) {

                    st_property[st_property_no].a = atof(ptr + 2) * unit_factors->a_f;
                }

                ptr = strstr(t->text, u8"γ="); //gamma
                if (ptr == NULL) ptr = strstr(t->text, "Γ=");  //Gamma
                if (ptr != NULL) {
                    st_property[st_property_no].gamma = atof(ptr + 3);
                }

                ptr = strstr(t->text, u8"γi=");  //gamma inf
                if (ptr == NULL) ptr = strstr(t->text, "Γi=");  //Gamma inf
                if (ptr != NULL) {
                    st_property[st_property_no].gamma_inf = atof(ptr + 4);
                }

                ptr = strstr(t->text, u8"ξ="); //xi
                if (ptr == NULL) ptr1 = strstr(t->text, "Ξ=");  //Xi
                if (ptr != NULL) {
                    st_property[st_property_no].xi = atof(ptr + 3);
                }

                st_property[st_property_no].ok = 1;

                if ((st_property[st_property_no].r == 90) || (st_property[st_property_no].r == -90) ||
                    (st_property[st_property_no].r == 270) || (st_property[st_property_no].r == -270)) {
                    double Asy_ = st_property[st_property_no].Asy;
                    st_property[st_property_no].Asy = st_property[st_property_no].Asz;
                    st_property[st_property_no].Asz = Asy_;

                    double Iy_ = st_property[st_property_no].Iy;
                    st_property[st_property_no].Iy = st_property[st_property_no].Iz;
                    st_property[st_property_no].Iz = Iy_;

                    double h_ = st_property[st_property_no].h;
                    st_property[st_property_no].h = st_property[st_property_no].b;
                    st_property[st_property_no].b = h_;

                    double wy_ = st_property[st_property_no].Wy;
                    st_property[st_property_no].Wy = st_property[st_property_no].Wz;
                    st_property[st_property_no].Wz = wy_;

                    st_property[st_property_no].r = 0;
                }

                if (st_property[st_property_no].Wy == 0) {
                    st_property[st_property_no].Wy =
                            (st_property[st_property_no].Iy / (st_property[st_property_no].h * 0.5)) *
                            unit_factors->Wm_f;  //no factor necessary, all in mm, mm^3 and mm^4 or in, in^3 and in^4
                }

                if (st_property[st_property_no].Wz == 0) {
                    st_property[st_property_no].Wz =
                            (st_property[st_property_no].Iz / (st_property[st_property_no].b * 0.5)) *
                            unit_factors->Wm_f;  //  same as above
                }

                add_property();

                break;
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, Otekst);
    }
    /////////////////

    //searching for nodes size
    obiekt_tok((char *) ADP, ADK, (char **) &nag, Ovector);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            v = (AVECTOR *) nag;
            if (v->style == 16)  //node size
            {
                st_node_pre[st_node_pre_no].x = v->x1;
                st_node_pre[st_node_pre_no].y = v->y1;
                st_node_pre[st_node_pre_no].radius = v->magnitude1;
                st_node_pre[st_node_pre_no].flag = 0; //node size not used yet
                add_node_pre();
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, Ovector);
    }

    //searching for frame members to generate nodes and elements
    obiekt_tok((char *) ADP, ADK, (char **) &nag, Ovector);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            v = (AVECTOR *) nag;

            if (v->style < 4) {
                parametry_lini(v, &PL);
                kos = sin(PL.kat * Pi / 180);
                koc = cos(PL.kat * Pi / 180);

                node_radius1 = find_node_size(v->x1, v->y1);
                node_radius2 = find_node_size(v->x2, v->y2);

                x1 = v->x1 + (jednostkiOb(node_radius1 + df_node_size)) * koc;
                y1 = v->y1 + (jednostkiOb(node_radius1 + df_node_size)) * kos;
                x2 = v->x2 - (jednostkiOb(node_radius2 + df_node_size)) * koc;
                y2 = v->y2 - (jednostkiOb(node_radius2 + df_node_size)) * kos;

                switch (v->style) {
                    case 0: //rigid rigid
                        //searching for nodes
                        st_element[st_element_no].node1 = -1;
                        st_element[st_element_no].node1r = -1;
                        st_element[st_element_no].node2 = -1;
                        st_element[st_element_no].node2r = -1;
                        st_element[st_element_no].property_no = v->property_no;
                        for (i = 0; i < st_node_no; i++)  //node1
                        {
                            if ((Check_if_Equal(v->x1, st_node[i].x)) && (Check_if_Equal(v->y1, st_node[i].y))) {
                                st_element[st_element_no].node1 = i;
                                st_element[st_element_no].node1r = i;
                                break;
                            }
                        }
                        for (i = 0; i < st_node_no; i++)  //node2
                        {
                            if ((Check_if_Equal(v->x2, st_node[i].x)) && (Check_if_Equal(v->y2, st_node[i].y))) {
                                st_element[st_element_no].node2 = i;
                                st_element[st_element_no].node2r = i;
                                break;
                            }
                        }
                        if (st_element[st_element_no].node1 == -1) {
                            st_node[st_node_no].radius = node_radius1;
                            st_node[st_node_no].x = v->x1;
                            st_node[st_node_no].y = v->y1;
                            st_node[st_node_no].real = 1;
                            st_element[st_element_no].node1 = st_node_no;
                            st_element[st_element_no].node1r = st_node_no;
                            add_node();
                        }
                        if (st_element[st_element_no].node2 == -1) {
                            st_node[st_node_no].radius = node_radius2;
                            st_node[st_node_no].x = v->x2;
                            st_node[st_node_no].y = v->y2;
                            st_node[st_node_no].real = 1;
                            st_element[st_element_no].node2 = st_node_no;
                            st_element[st_element_no].node2r = st_node_no;
                            add_node();
                        }
                        st_element[st_element_no].length = sqrt(
                                ((st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x) *
                                 (st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x)) +
                                ((st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y) *
                                 (st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y)));
                        add_element();
                        break;
                    case 1: //rigid hinged
                        //rigid
                        st_element[st_element_no].node1 = -1;
                        st_element[st_element_no].node1r = -1;
                        st_element[st_element_no].node2 = -1;
                        st_element[st_element_no].node2r = -1;
                        st_element[st_element_no].property_no = v->property_no;

                        for (i = 0; i < st_node_no; i++)  //node1 rigid
                        {
                            if ((Check_if_Equal(v->x1, st_node[i].x)) && (Check_if_Equal(v->y1, st_node[i].y))) {
                                st_element[st_element_no].node1 = i;
                                st_element[st_element_no].node1r = i;
                                break;
                            }
                        }
                        if (st_element[st_element_no].node1 == -1) {
                            st_node[st_node_no].radius = node_radius1;
                            st_node[st_node_no].x = v->x1;
                            st_node[st_node_no].y = v->y1;
                            st_node[st_node_no].real = 1;
                            st_element[st_element_no].node1 = st_node_no;
                            st_element[st_element_no].node1r = st_node_no;
                            add_node();
                        }
                        //hinged side

                        st_node[st_node_no].radius = 0; //node_radius2;
                        st_node[st_node_no].x = x2;
                        st_node[st_node_no].y = y2;
                        st_node[st_node_no].real = 0;
                        st_element[st_element_no].node2 = st_node_no;
                        add_node();

                        for (i = 0; i < st_node_no; i++)  //node2 hinged element
                        {
                            if ((Check_if_Equal(v->x2, st_node[i].x)) && (Check_if_Equal(v->y2, st_node[i].y))) {
                                st_element[st_element_no].node2r = i;
                                break;
                            }
                        }
                        if (st_element[st_element_no].node2 == -1) {
                            st_node[st_node_no].radius = node_radius2;
                            st_node[st_node_no].x = v->x2;
                            st_node[st_node_no].y = v->y2;
                            st_node[st_node_no].real = 1;
                            st_element[st_element_no].node2r = st_node_no;
                            add_node();
                        }

                        st_element[st_element_no].length = sqrt(
                                ((st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x) *
                                 (st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x)) +
                                ((st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y) *
                                 (st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y)));

                        add_element();

                        st_element[st_element_no].node1 = st_element[st_element_no - 1].node2;
                        st_element[st_element_no].node1r = -1;
                        st_element[st_element_no].node2 = st_element[st_element_no - 1].node2r;
                        st_element[st_element_no].node2r = st_element[st_element_no].node2;
                        st_element[st_element_no].property_no = -v->property_no;

                        st_element[st_element_no].length = sqrt(
                                ((st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x) *
                                 (st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x)) +
                                ((st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y) *
                                 (st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y)));

                        add_element();

                        break;
                    case 2: //hinged rigid
                        //hinged side
                        st_element[st_element_no].node1 = -1;
                        st_element[st_element_no].node1r = -1;
                        st_element[st_element_no].node2 = -1;
                        st_element[st_element_no].node2r = -1;
                        st_element[st_element_no].property_no = -v->property_no;

                        for (i = 0; i < st_node_no; i++)  //node1 hinged
                        {
                            if ((Check_if_Equal(v->x1, st_node[i].x)) && (Check_if_Equal(v->y1, st_node[i].y))) {
                                st_element[st_element_no].node1 = i;
                                st_element[st_element_no].node1r = i;
                                break;
                            }
                        }
                        if (st_element[st_element_no].node1 == -1) {
                            st_node[st_node_no].radius = node_radius1;
                            st_node[st_node_no].x = v->x1;
                            st_node[st_node_no].y = v->y1;
                            st_node[st_node_no].real = 1;
                            st_element[st_element_no].node1 = st_node_no;
                            st_element[st_element_no].node1r = st_node_no;
                            add_node();
                        }

                        st_node[st_node_no].radius = 0; //node_radius1;
                        st_node[st_node_no].x = x1;
                        st_node[st_node_no].y = y1;
                        st_node[st_node_no].real = 0;
                        st_element[st_element_no].node2 = st_node_no;
                        st_element[st_element_no].node2r = -1;
                        add_node();

                        st_element[st_element_no].length = sqrt(
                                ((st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x) *
                                 (st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x)) +
                                ((st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y) *
                                 (st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y)));

                        add_element();

                        st_element[st_element_no].node1 = st_element[st_element_no - 1].node2;
                        st_element[st_element_no].node1r = st_element[st_element_no - 1].node1r;
                        st_element[st_element_no].property_no = v->property_no;
                        st_element[st_element_no].node2 = -1;
                        st_element[st_element_no].node2r = -1;

                        for (i = 0; i < st_node_no; i++)  //node2 hinged element
                        {
                            if ((Check_if_Equal(v->x2, st_node[i].x)) && (Check_if_Equal(v->y2, st_node[i].y))) {
                                st_element[st_element_no].node2 = i;
                                st_element[st_element_no].node2r = i;
                                break;
                            }
                        }
                        if (st_element[st_element_no].node2 == -1) {
                            st_node[st_node_no].radius = node_radius2;
                            st_node[st_node_no].x = v->x2;
                            st_node[st_node_no].y = v->y2;
                            st_node[st_node_no].real = 1;
                            st_element[st_element_no].node2 = st_node_no;
                            st_element[st_element_no].node2r = st_node_no;
                            add_node();
                        }

                        st_element[st_element_no].length = sqrt(
                                ((st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x) *
                                 (st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x)) +
                                ((st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y) *
                                 (st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y)));

                        add_element();

                        break;
                    case 3: //hinged hinged
                        //hinged first side
                        st_element[st_element_no].node1 = -1;
                        st_element[st_element_no].node1r = -1;
                        st_element[st_element_no].node2 = -1;
                        st_element[st_element_no].node2r = -1;
                        st_element[st_element_no].property_no = -v->property_no;

                        for (i = 0; i < st_node_no; i++)  //node1 rigid
                        {
                            if ((Check_if_Equal(v->x1, st_node[i].x)) && (Check_if_Equal(v->y1, st_node[i].y))) {
                                st_element[st_element_no].node1 = i;
                                st_element[st_element_no].node1r = i;
                                break;
                            }
                        }
                        if (st_element[st_element_no].node1 == -1) {
                            st_node[st_node_no].radius = node_radius1;
                            st_node[st_node_no].x = v->x1;
                            st_node[st_node_no].y = v->y1;
                            st_node[st_node_no].real = 1;
                            st_element[st_element_no].node1 = st_node_no;
                            st_element[st_element_no].node1r = st_node_no;
                            add_node();
                        }

                        st_node[st_node_no].radius = 0; //node_radius1;
                        st_node[st_node_no].x = x1;
                        st_node[st_node_no].y = y1;
                        st_node[st_node_no].real = 0;
                        st_element[st_element_no].node2 = st_node_no;
                        st_element[st_element_no].node2r = -1;
                        add_node();

                        st_element[st_element_no].length = sqrt(
                                ((st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x) *
                                 (st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x)) +
                                ((st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y) *
                                 (st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y)));

                        add_element();
                        //hinged middle
                        st_element[st_element_no].node2r = -1;
                        st_element[st_element_no].property_no = v->property_no;

                        st_element[st_element_no].node1 = st_element[st_element_no - 1].node2;
                        st_element[st_element_no].node1r = st_element[st_element_no - 1].node1r;

                        st_node[st_node_no].radius = 0; //node_radius2;
                        st_node[st_node_no].x = x2;
                        st_node[st_node_no].y = y2;
                        st_node[st_node_no].real = 0;
                        st_element[st_element_no].node2 = st_node_no;
                        add_node();

                        for (i = 0; i < st_node_no; i++)  //node2 hinged element
                        {
                            if ((Check_if_Equal(v->x2, st_node[i].x)) && (Check_if_Equal(v->y2, st_node[i].y))) {
                                st_element[st_element_no].node2r = i;
                                break;
                            }
                        }
                        if (st_element[st_element_no].node2r == -1) {
                            st_node[st_node_no].radius = node_radius2;
                            st_node[st_node_no].x = v->x2;
                            st_node[st_node_no].y = v->y2;
                            st_node[st_node_no].real = 1;
                            st_element[st_element_no].node2r = st_node_no;
                            add_node();
                        }

                        st_element[st_element_no].length = sqrt(
                                ((st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x) *
                                 (st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x)) +
                                ((st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y) *
                                 (st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y)));

                        add_element();
                        //hinged second side
                        st_element[st_element_no].node1 = st_element[st_element_no - 1].node2;
                        st_element[st_element_no].node1r = -1;
                        st_element[st_element_no].node2 = st_element[st_element_no - 1].node2r;
                        st_element[st_element_no].node2r = st_element[st_element_no].node2;
                        st_element[st_element_no].property_no = -v->property_no;

                        st_element[st_element_no].length = sqrt(
                                ((st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x) *
                                 (st_node[st_element[st_element_no].node2].x -
                                  st_node[st_element[st_element_no].node1].x)) +
                                ((st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y) *
                                 (st_node[st_element[st_element_no].node2].y -
                                  st_node[st_element[st_element_no].node1].y)));

                        add_element();

                        break;
                    default:
                        break;
                }
            }
        }

        obiekt_tok(NULL, ADK, (char **) &nag, Ovector);
    }

    combi_element_uls = (COMBI_ELEMENT *) malloc(st_element_no * sizeof(COMBI_ELEMENT));
    combi_element_sls = (COMBI_ELEMENT *) malloc(st_element_no * sizeof(COMBI_ELEMENT));
    combi_element_qpsls = (COMBI_ELEMENT *) malloc(st_element_no * sizeof(COMBI_ELEMENT));
    for (i = 0; i < st_element_no; i++) {
        combi_element_uls[i].nx = 0;
        combi_element_uls[i].fd = NULL;

        combi_element_sls[i].nx = 0;
        combi_element_sls[i].fd = NULL;

        combi_element_qpsls[i].nx = 0;
        combi_element_qpsls[i].fd = NULL;
    }

    //searching for nodes use
    for (i = 0; i < st_node_pre_no; i++)  //node1
    {
        if (st_node_pre[i].flag == 0) {
            sprintf(report_row, "<%f;%f> %s\n", milimetryobx(st_node_pre[i].x), milimetryoby(st_node_pre[i].y),
                    _node_size_not_associated_);
            strcat(report, report_row);
        }
    }

    /////////////////
    //checking properties
    for (j = 0; j < st_property_no; j++) {
        if (st_property[j].ok == 0) {
            sprintf(report_row, "#%d %s\n", st_property[j].n, _incorrectly_defined_);
            strcat(report, report_row);
        }
    }
    //checking elements
    for (i = 0; i < st_element_no; i++) {
        for (j = 0; j < st_property_no; j++) {
            if (abs(st_element[i].property_no) == st_property[j].n)
                break;
        }
        if (j == st_property_no) {
            sprintf(report_row, "#%d %s\n", st_element[i].property_no, _property_not_defined_);
            strcat(report, report_row);
        }
    }
    //////////////
    //REACTION
    for (i = 0; i < st_node_no; i++) {
        st_reaction[st_reaction_no].node = i;
        st_reaction[st_reaction_no].active = 0;
        st_reaction[st_reaction_no].x = 0;   //0 or 1     //moving in local axis X default is 0
        st_reaction[st_reaction_no].y = 0;   //0 or 1     //moving in local axis Y default is 0
        st_reaction[st_reaction_no].z = 1;   //1          //moving in local axis Z default is 1 mostly due to geometric stiffness and dynamics
        st_reaction[st_reaction_no].xx = 1;  //1 or 0     //rotation about local axis X default is 1 mostly due to dynamics
        st_reaction[st_reaction_no].yy = 1;  //1 or 0     //rotation about local axis Y default is 1 mostly due to dynamics
        st_reaction[st_reaction_no].zz = 0;  //0 or 1     //rotation about local axis Z default is 0
        add_reaction();
    }

    obiekt_tok((char *) ADP, ADK, (char **) &nag, Opoint);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            p = (T_Point *) nag;

            if ((p->typ > 11) && (p->typ < 29)) //supports only
            {
                for (i = 0; i < st_node_no; i++)  //node1
                {
                    if ((Check_if_Equal(p->x, st_node[i].x)) && (Check_if_Equal(p->y, st_node[i].y))) {
                        switch (p->typ) {
                            case 12:  //rigids
                            case 13:
                            case 14:
                            case 15:
                                st_reaction[i].active = 1;
                                st_reaction[i].x = 1;
                                st_reaction[i].y = 1;
                                st_reaction[i].z = 1;
                                st_reaction[i].xx = 1;
                                st_reaction[i].yy = 1;
                                st_reaction[i].zz = 1;
                                break;
                            case 16:  //hinged
                            case 17:
                            case 18:
                            case 19:
                                st_reaction[i].active = 1;
                                st_reaction[i].x = 1;
                                st_reaction[i].y = 1;
                                st_reaction[i].z = 1;
                                st_reaction[i].xx = 1;
                                st_reaction[i].yy = 1;
                                st_reaction[i].zz = 0;
                                break;
                            case 20:  //rigid with X roll
                            case 23:
                                st_reaction[i].active = 1;
                                st_reaction[i].x = 0;
                                st_reaction[i].y = 1;
                                st_reaction[i].z = 1;
                                st_reaction[i].xx = 1;
                                st_reaction[i].yy = 1;
                                st_reaction[i].zz = 1;
                                break;
                            case 21:  //rigid with Y roll
                            case 22:
                                st_reaction[i].active = 1;
                                st_reaction[i].x = 1;
                                st_reaction[i].y = 0;
                                st_reaction[i].xx = 1;
                                st_reaction[i].yy = 1;
                                st_reaction[i].zz = 1;
                                break;
                            case 24:  //hinged with X roll
                            case 27:
                                st_reaction[i].active = 1;
                                st_reaction[i].x = 0;
                                st_reaction[i].y = 1;
                                st_reaction[i].z = 1;
                                st_reaction[i].xx = 1;
                                st_reaction[i].yy = 1;
                                st_reaction[i].zz = 0;
                                break;
                            case 25:  //hinged with Y roll
                            case 26:
                                st_reaction[i].active = 1;
                                st_reaction[i].x = 1;
                                st_reaction[i].y = 0;
                                st_reaction[i].z = 1;
                                st_reaction[i].xx = 1;
                                st_reaction[i].yy = 1;
                                st_reaction[i].zz = 0;
                                break;
                            case 28:
                                st_reaction[i].active = 1;
                                st_reaction[i].x = 0;
                                st_reaction[i].y = 0;
                                st_reaction[i].z = 1;   //moving in local axis Z default is 1 mostly due to geometric stiffness and dynamics
                                st_reaction[i].xx = 1;
                                st_reaction[i].yy = 1;
                                st_reaction[i].zz = 1;
                                break;
                        }
                        //add_reaction();
                        break;
                    }
                }
                if (i == st_node_no) {
                    sprintf(report_row, "<%f;%f> %s\n", milimetryobx(p->x), milimetryoby(p->y),
                            _reaction_not_associated_);
                    strcat(report, report_row);
                }
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, Opoint);
    }

    //REACTION
    combi_reactions_table = (int *) malloc(st_node_no * sizeof(int));

    int st_active_reaction_no = 0;

    for (int iii = 0; iii < st_reaction_no; iii++) {
        if (st_reaction[iii].active == 1)  //node==node_no)
        {
            combi_reactions_table[st_reaction[iii].node] = st_active_reaction_no;
            st_active_reaction_no++;
        }
    }

    combi_reactions = (COMBI_REACIONS *) malloc(st_active_reaction_no * sizeof(COMBI_REACIONS));

    //scanning layer by layer
    //method:
    //load can be spread among layers, each layer is added to the load case combination_no
    //  before was : on each layer is different load case, what is modified, and number of case is set in prompting window with Eurocodes, ASCE and ICC standards
    //first scan counts number of load cases
    st_layer_no = 0;
    for (i = 0; i < 256; i++) st_layers[i] = 0;

    obiekt_tok((char *) ADP, ADK, (char **) &nag, Ovector);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            v = (AVECTOR *) nag;

            if ((v->style > 3) && (v->style < 16)) //any sort of load
            {
                for (li = 0; li < st_layer_no; li++)
                    if (st_layers[li] == v->warstwa) break;
                if (li == st_layer_no) {
                    st_layers[st_layer_no] = v->warstwa;
                    st_layer_no++;
                }
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, Ovector);
    }

    //sorting layers
#ifdef LINUX
    qsort(st_layers, st_layer_no, sizeof(unsigned char), (__compar_fn_t) qsort_by_number);
#else
    //good for CLion
    qsort(st_layer, st_layer_no, sizeof(unsigned char), (_CoreCrtNonSecureSearchSortCompareFunction)qsort_by_number);

    //good for VS
    //qsort(st_layer, st_layer_no, sizeof(unsigned char), (int(*)(const int*, const int*)) qsort_by_number);
#endif

    //creating layers mask
    for (int ii = 0; ii < 8; ii++) st_layer[ii] = 0;
    for (i = 0; i < st_layer_no; i++) {
        SetBit(st_layer, st_layers[i]);
    }

    //deleting old files
    if (__file_exists("alfacad.out")) unlink("alfacad.out");
    if (__file_exists("alfacad.plt")) unlink("alfacad.plt");
    if (__file_exists("alfacad.add")) unlink("alfacad.add");
    if (__file_exists("alfacad_out.CSV")) unlink("alfacad_out.CSV");
    if (__file_exists("alfastatic.pdf")) unlink("alfastatic.pdf");

    ////combinations_number = st_layer_no * 2;  //SLS and ULS
    combinations_number = 2; //SLS and ULS

    for (i = 0; i < combinations_number; i++) {
        sprintf(params, "alfacad.if%02d", i + 1);
        if (__file_exists(params)) unlink(params);
    }

    //creating data file
    f = fopen("alfacad.3dd", "wt");
    fprintf(f, "%s\n", st_title);
    fprintf(f, "\n# node data ...\n");
    fprintf(f, "%d\t\t # number of nodes\n", st_node_no);
    fprintf(f, "# X-coord  Y-coord  Z-coord  radius\n");
    fprintf(f, "#  in\t\tin\t\tin\t\tin\n");
    fprintf(f, "#  mm\t\tmm\t\tmm\t\tmm\n\n");
    for (i = 0; i < st_node_no; i++) {
        set_decimal_format(par[0], milimetryobx(st_node[i].x) * units_factor, dim_precision);
        set_decimal_format(par[1], milimetryoby(st_node[i].y) * units_factor, dim_precision);
        set_decimal_format(par[2], 0.0, dim_precision);
        set_decimal_format(par[3], st_node[i].radius * units_factor, dim_precision);

        fprintf(f, "%d %s %s %s %s\n", i + 1, par[0], par[1], par[2], par[3]);
    }

    fprintf(f, "\n# reaction data ...\n");

    fprintf(f, "%d\t\t # number of nodes with reactions\n", st_reaction_no);
    ////fprintf(f, "%d\t\t # number of nodes with reactions\n", st_active_reaction_no);

    fprintf(f, "#.n     x y z xx yy zz		1=fixed, 0=free\n\n");
    for (i = 0; i < st_reaction_no; i++) {
        //if (st_reaction[i].active)  //all nodes include reactions, at least in z direction, but for dynamic reasons, also xx and yy, so frame is always flat
            fprintf(f, "%d %d %d %d %d %d %d\n", st_reaction[i].node + 1, st_reaction[i].x, st_reaction[i].y,
                    st_reaction[i].z, st_reaction[i].xx, st_reaction[i].yy, st_reaction[i].zz);
    }

    fprintf(f, "\n# frame element data ...\n");
    fprintf(f, "%d\t\t # number of frame elements\n", st_element_no);
    fprintf(f, "#e n1 n2 Ax    Asy     Asz     Jxx     Iyy     Izz     E     G    roll  density\n");
    fprintf(f, "#. .  .  in^2  in^2    in^2    in^4    in^4    in^4    ksi   ksi  deg  kip/in^3/g\n");
    fprintf(f, "#. .  .  mm^2   mm^2    mm^2    mm^4   mm^4    mm^4    MPa   MPa  deg  tonne/mm^3\n\n");
    for (i = 0; i < st_element_no; i++) {
        //searching for property
        hinged = 0;
        for (j = 0; j < st_property_no; j++) {
            if (st_element[i].property_no < 0) hinged = 1;
            if (abs(st_element[i].property_no) == st_property[j].n)
                break;
        }

        if (j == st_property_no) {
            sprintf(report_row, "#%d %s\n", st_element[i].property_no, _property_not_defined_);
            strcat(report, report_row);
            break;
        }

        set_decimal_format(par[0], st_property[j].A, prop_precisions->A_precision);
        set_decimal_format(par[1], st_property[j].Asy, prop_precisions->A_precision);
        set_decimal_format(par[2], st_property[j].Asz, prop_precisions->A_precision);
        if (hinged == 1) {
            set_decimal_format(par[3], 1.0, prop_precisions->J_precision);
            set_decimal_format(par[4], st_property[j].Iz * 0.0001, prop_precisions->I_precision);
            set_decimal_format(par[5], st_property[j].Iy * 0.0001, prop_precisions->I_precision);
            set_decimal_format(par[6], st_property[j].E * 0.01, prop_precisions->E_precision);
        } else {
            set_decimal_format(par[3], 1.0, prop_precisions->J_precision);
            set_decimal_format(par[4], st_property[j].Iz, prop_precisions->I_precision);
            set_decimal_format(par[5], st_property[j].Iy, prop_precisions->I_precision);
            set_decimal_format(par[6], st_property[j].E, prop_precisions->E_precision);
        }
        set_decimal_format(par[7], st_property[j].G, prop_precisions->G_precision);
        set_decimal_format(par[8], st_property[j].r, prop_precisions->p_precision);
        sprintf(par[9], "%g", st_property[j].d);

        fprintf(f, "%d %d %d %s %s %s %s %s %s %s %s %s %s\n", i + 1, st_element[i].node1 + 1, st_element[i].node2 + 1,
                par[0], par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9]);
    }

    fprintf(f, "\n%d\t\t# 1: include shear deformation\n", shear_deformation);
    fprintf(f, "%d\t\t# 1: include geometric stiffness\n", geometric_tiffness);
    fprintf(f, "%f\t\t# exaggerate mesh deformations\n", mesh_deformations);
    fprintf(f, "%f\t\t# zoom scale for 3D plotting\n", zoom_scale);
    fprintf(f, "%f\t\t# x-axis increment for internal forces\n", axis_increment);
    fprintf(f, "\t\t# if dx is -1 then internal force calculations are skipped\n");

    ////combinations_number = st_layer_no * 2;  //SLS + ULS each
    ////fprintf(f,"%d\t\t# number of static load cases\n",combinations_number);
    //////////////////////////

    //for all layers, as different set of loads, we create SLS single combination, without gamma, psi and xi, and ULS with gamma but at this stage ignoring xi and psi, UNTIL Eurocode, ASCE, ICC combinations are solved

    //forces and moments in nodes and displacements and rotations st_node_force_moment[st_node_force_moment_no]  st_displacement[st_displacementno]
    obiekt_tok((char *) ADP, ADK, (char **) &nag, Ovector);

    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            v = (AVECTOR *) nag;

            if (TestBit(st_layer, v->warstwa)) {
                switch (v->style) {
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:

                        st_node_force_moment[st_node_force_moment_no].layer = v->warstwa;
                        st_node_force_moment[st_node_force_moment_no].node = -1;
                        st_node_force_moment[st_node_force_moment_no].mz = 0;
                        st_node_force_moment[st_node_force_moment_no].fx = 0;
                        st_node_force_moment[st_node_force_moment_no].fy = 0;
                        st_node_force_moment[st_node_force_moment_no].factor_record = -1;
                        st_node_force_moment[st_node_force_moment_no].take_it = 0;

                        st_displacement[st_displacement_no].layer = v->warstwa;
                        st_displacement[st_displacement_no].node = -1;
                        st_displacement[st_displacement_no].dzz = 0;
                        st_displacement[st_displacement_no].dx = 0;
                        st_displacement[st_displacement_no].dy = 0;
                        st_displacement[st_displacement_no].factor_record = -1;
                        st_displacement[st_displacement_no].take_it = 1;

                        for (i = 0; i < st_node_no; i++)  //node1
                        {
                            if ((Check_if_Equal(v->x1, st_node[i].x)) && (Check_if_Equal(v->y1, st_node[i].y))) {
                                st_node_force_moment[st_node_force_moment_no].node = i;
                                st_displacement[st_displacement_no].node = i;
                                switch (v->style) {
                                    case 4:  //force
                                        parametry_lini(v, &PL);
                                        kos = sin(Angle_Normal(PL.kat * Pi / 180));
                                        koc = cos(Angle_Normal(PL.kat * Pi / 180));

                                        st_node_force_moment[st_node_force_moment_no].fx = -v->magnitude1 * koc;
                                        st_node_force_moment[st_node_force_moment_no].fy = -v->magnitude1 * kos;
                                        if (v->variant > 0)
                                            st_node_force_moment[st_node_force_moment_no].factor_record = factor_record(
                                                    v->load, v->variant);
                                        else
                                            st_node_force_moment[st_node_force_moment_no].factor_record = -(v->load == 0
                                                                                                            ? 2
                                                                                                            : v->load);  //if no load spec, is assumed to be LL, so load=2
                                        add_node_force_moment();
                                        break;
                                    case 5:  //moment +
                                        st_node_force_moment[st_node_force_moment_no].mz = v->magnitude1;
                                        if (v->variant > 0)
                                            st_node_force_moment[st_node_force_moment_no].factor_record = factor_record(
                                                    v->load, v->variant);
                                        else
                                            st_node_force_moment[st_node_force_moment_no].factor_record = -(v->load == 0
                                                                                                            ? 2
                                                                                                            : v->load);  //if no load spec, is assumed to be LL, so load=2
                                        add_node_force_moment();
                                        break;
                                    case 6:  //moment -
                                        st_node_force_moment[st_node_force_moment_no].mz = -v->magnitude1;
                                        if (v->variant > 0)
                                            st_node_force_moment[st_node_force_moment_no].factor_record = factor_record(
                                                    v->load, v->variant);
                                        else
                                            st_node_force_moment[st_node_force_moment_no].factor_record = -(v->load == 0
                                                                                                            ? 2
                                                                                                            : v->load);  //if no load spec, is assumed to be LL, so load=2
                                        add_node_force_moment();
                                        break;
                                    case 7:  //displacement
                                        parametry_lini(v, &PL);
                                        kos = sin(Angle_Normal(PL.kat * Pi / 180));
                                        koc = cos(Angle_Normal(PL.kat * Pi / 180));

                                        st_displacement[st_displacement_no].dx = v->magnitude1 * koc;
                                        st_displacement[st_displacement_no].dy = v->magnitude1 * kos;
                                        //there are no factors taken into account for displacement
                                        add_displacement();
                                        break;
                                    case 8:  //rotation +
                                        st_displacement[st_displacement_no].dzz = v->magnitude1;
                                        //there are no factors taken into account for displacement
                                        add_displacement();
                                        break;
                                    case 9:  //rotation -
                                        st_displacement[st_displacement_no].dzz = -v->magnitude1;
                                        //there are no factors taken into account for displacement
                                        add_displacement();
                                        break;
                                }
                                break;
                            }
                        }

                }
                if ((i == st_node_no) && ((v->style == 7) || (v->style == 8) || (v->style == 9))) {
                    sprintf(report_row, "<%f;%f> [%s] %s\n", milimetryobx(v->x1), milimetryoby(v->y1),
                            (*mVector.pola)[v->style + 1].txt, _force_not_associated_);
                    strcat(report, report_row);
                }
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, Ovector);
    }

    //internal concentrated loads - element force

    obiekt_tok((char *) ADP, ADK, (char **) &nag, Ovector);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            v = (AVECTOR *) nag;

            if (TestBit(st_layer, v->warstwa)) {

                for (j = 0; j < st_node_no; j++)  //node1
                {
                    if ((Check_if_Equal(v->x1, st_node[j].x)) && (Check_if_Equal(v->y1, st_node[j].y))) {
                        break;
                    }
                }

                if (j == st_node_no)  //force doesn't load any node
                {

                    st_element_force[st_element_force_no].layer = v->warstwa;
                    st_element_force[st_element_force_no].element = -1;
                    st_element_force[st_element_force_no].fx = 0;
                    st_element_force[st_element_force_no].fy = 0;
                    st_element_force[st_element_force_no].dlx = 0;
                    st_element_force[st_element_force_no].factor_record = -1;
                    st_element_force[st_element_force_no].take_it = 0;

                    if (v->style == 4) {
                        for (i = 0; i < st_element_no; i++)  //node1
                        {
                            if (CheckIsPointOnLineSegment(v->x1, v->y1,
                                                          st_node[st_element[i].node1].x,   //node1r node2r
                                                          st_node[st_element[i].node1].y,
                                                          st_node[st_element[i].node2].x,
                                                          st_node[st_element[i].node2].y, 0.0001)) {

                                parametry_lini(v, &PL);
                                kos = sin(Angle_Normal(PL.kat * Pi / 180));
                                koc = cos(Angle_Normal(PL.kat * Pi / 180));

                                st_element_force[st_element_force_no].element = i;
                                st_element_force[st_element_force_no].fx = -v->magnitude1 * koc;
                                st_element_force[st_element_force_no].fy = -v->magnitude1 * kos;
                                st_element_force[st_element_force_no].dlx = sqrt(
                                        ((v->x1 - st_node[st_element[i].node1].x) *  //node1r node2r
                                         (v->x1 - st_node[st_element[i].node1].x)) +
                                        ((v->y1 - st_node[st_element[i].node1].y) *
                                         (v->y1 - st_node[st_element[i].node1].y)));
                                if (v->variant > 0)
                                    st_element_force[st_element_force_no].factor_record = factor_record(v->load,
                                                                                                        v->variant);
                                else
                                    st_element_force[st_element_force_no].factor_record = -(v->load == 0 ? 2
                                                                                                         : v->load);  //if no load spec, is assumed to be LL, so load=2
                                add_element_force();
                                break;
                            }
                        }
                        if (i == st_element_no) {
                            sprintf(report_row, "<%f;%f> [%s] %s\n", milimetryobx(v->x1), milimetryoby(v->y1),
                                    (*mVector.pola)[v->style + 1].txt, _load_not_associated_);
                            strcat(report, report_row);
                        }
                    } else if (v->style == 5) //moment +
                    {
                        //should be report about error
                        ;
                    } else if (v->style == 6) //moment -
                    {
                        //should be report about error
                        ;
                    }
                }

            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, Ovector);
    }

    //uniform and trapezoid loads on elements
    obiekt_tok((char *) ADP, ADK, (char **) &nag, Ovector);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            v = (AVECTOR *) nag;
            if (TestBit(st_layer, v->warstwa)) {

                st_uniform_load[st_uniform_load_no].layer = v->warstwa;
                st_uniform_load[st_uniform_load_no].element = -1;
                st_uniform_load[st_uniform_load_no].qx = 0;
                st_uniform_load[st_uniform_load_no].qy = 0;
                if (v->variant > 0)
                    st_uniform_load[st_uniform_load_no].factor_record = factor_record(v->load, v->variant);
                else
                    st_uniform_load[st_uniform_load_no].factor_record = -(v->load == 0 ? 2
                                                                                       : v->load);  //if no load spec, is assumed to be LL, so load=2
                st_uniform_load[st_uniform_load_no].take_it = 0;

                st_trapezoid_load[st_trapezoid_load_no].layer = v->warstwa;
                st_trapezoid_load[st_trapezoid_load_no].element = -1;
                st_trapezoid_load[st_trapezoid_load_no].qx1 = 0;
                st_trapezoid_load[st_trapezoid_load_no].qy1 = 0;
                st_trapezoid_load[st_trapezoid_load_no].qx2 = 0;
                st_trapezoid_load[st_trapezoid_load_no].qy2 = 0;
                st_trapezoid_load[st_trapezoid_load_no].dlxx1 = 0;
                st_trapezoid_load[st_trapezoid_load_no].dlxx2 = 0;
                st_trapezoid_load[st_trapezoid_load_no].dlxy1 = 0;
                st_trapezoid_load[st_trapezoid_load_no].dlxy2 = 0;
                if (v->variant > 0)
                    st_trapezoid_load[st_trapezoid_load_no].factor_record = factor_record(v->load, v->variant);
                else
                    st_trapezoid_load[st_trapezoid_load_no].factor_record = -(v->load == 0 ? 2
                                                                                           : v->load);  //if no load spec, is assumed to be LL, so load=2
                st_trapezoid_load[st_trapezoid_load_no].take_it = 0;

                int v_factor_record = st_uniform_load[st_uniform_load_no].factor_record;

                //element, uniform, spread, reversed
                st_load_no = 0;

                nm = 1;

                switch (v->style) {
                    case 10:
                        if (v->x1 >= v->x2) nm = -1;
                        break;
                    case 11:
                        if (v->y1 >= v->y2) nm = -1;
                        break;
                    case 12:
                        break;
                    case 13:
                        if (v->x1 > v->x2) nm = -1;
                        break;
                    case 14:
                        if (v->y1 > v->y2) nm = -1;
                        break;
                }

                switch (v->style) {
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                        parametry_lini(v, &PL);
                        partial_length = PL.dl;

                        for (i = 0; i < st_element_no; i++)  //node1
                        {
                            if (Check_if_Equal(v->x1, st_node[st_element[i].node1].x) &&
                                Check_if_Equal(v->y1, st_node[st_element[i].node1].y) &&
                                Check_if_Equal(v->x2, st_node[st_element[i].node2].x) &&
                                Check_if_Equal(v->y2, st_node[st_element[i].node2].y)) {  //same direction
                                st_load[st_load_no].element = i;
                                st_load[st_load_no].reversed = 0;
                                st_load[st_load_no].spread = 1;
                                st_load[st_load_no].partial = 0;
                                if (Check_if_Equal(v->magnitude1, v->magnitude2))
                                    st_load[st_load_no].uniform = 1; //0; //1;  UNIFORM DOESN'T WORK FOR MULTIPLE LOADS ON 1 ELEMENT
                                else st_load[st_load_no].uniform = 0;
                                st_load[st_load_no].magnitude1 = v->magnitude1 * nm;
                                st_load[st_load_no].magnitude2 = v->magnitude2 * nm;
                                st_load[st_load_no].factor_record = v_factor_record;
                                add_load();
                                break;
                            } else if (Check_if_Equal(v->x1, st_node[st_element[i].node2].x) &&
                                       Check_if_Equal(v->y1, st_node[st_element[i].node2].y) &&
                                       Check_if_Equal(v->x2, st_node[st_element[i].node1].x) &&
                                       Check_if_Equal(v->y2, st_node[st_element[i].node1].y)) {  //reversed direction
                                st_load[st_load_no].element = i;
                                st_load[st_load_no].reversed = 1;
                                st_load[st_load_no].spread = 1;
                                st_load[st_load_no].partial = 0;
                                if (Check_if_Equal(v->magnitude1, v->magnitude2))
                                    st_load[st_load_no].uniform = 1; //0; //1; UNIFORM DOESN'T WORK FOR MULTIPLE LOADS ON 1 ELEMENT
                                else st_load[st_load_no].uniform = 0;
                                st_load[st_load_no].magnitude1 = v->magnitude1 * nm;
                                st_load[st_load_no].magnitude2 = v->magnitude2 * nm;
                                st_load[st_load_no].factor_record = v_factor_record;
                                add_load();
                                break;
                            }
                                //trapezoid or uniform load inside element
                            else if ((CheckIsPointOnLineSegment(v->x1, v->y1, st_node[st_element[i].node1].x,
                                                                st_node[st_element[i].node1].y,
                                                                st_node[st_element[i].node2].x,
                                                                st_node[st_element[i].node2].y, 0.0001)) &&
                                     (CheckIsPointOnLineSegment(v->x2, v->y2, st_node[st_element[i].node1].x,
                                                                st_node[st_element[i].node1].y,
                                                                st_node[st_element[i].node2].x,
                                                                st_node[st_element[i].node2].y, 0.0001))) {
                                st_load[st_load_no].element = i;
                                st_load[st_load_no].partial = 0;
                                st_load[st_load_no].uniform = 0;
                                parametry_lini(v, &PL);
                                L.x1 = st_node[st_element[i].node1].x;
                                L.y1 = st_node[st_element[i].node1].y;
                                L.x2 = st_node[st_element[i].node2].x;
                                L.y2 = st_node[st_element[i].node2].y;
                                parametry_lini(&L, &PL1);
                                if (Check_if_Equal2(PL.kat, PL1.kat)) st_load[st_load_no].reversed = 0;
                                else st_load[st_load_no].reversed = 1;
                                st_load[st_load_no].spread = 0;
                                if (Check_if_Equal(v->magnitude1, v->magnitude2))
                                    st_load[st_load_no].uniform = 1; //0; //1; UNIFORM DOESN'T WORK FOR MULTIPLE LOADS ON 1 ELEMENT
                                else st_load[st_load_no].uniform = 0;
                                st_load[st_load_no].magnitude1 = v->magnitude1 * nm;
                                st_load[st_load_no].magnitude2 = v->magnitude2 * nm;
                                st_load[st_load_no].factor_record = v_factor_record;
                                add_load();
                                break;
                            }
                                //load spread over entire element
                            else if ((CheckIsPointOnLineSegment(st_node[st_element[i].node1].x,
                                                                st_node[st_element[i].node1].y,
                                                                v->x1, v->y1, v->x2, v->y2, 0.0001)) &&
                                     (CheckIsPointOnLineSegment(st_node[st_element[i].node2].x,
                                                                st_node[st_element[i].node2].y,
                                                                v->x1, v->y1, v->x2, v->y2, 0.0001))) {
                                st_load[st_load_no].element = i;
                                st_load[st_load_no].spread = 1;
                                st_load[st_load_no].partial = 1;
                                parametry_lini(v, &PL);
                                L.x1 = st_node[st_element[i].node1].x;
                                L.y1 = st_node[st_element[i].node1].y;
                                L.x2 = st_node[st_element[i].node2].x;
                                L.y2 = st_node[st_element[i].node2].y;
                                parametry_lini(&L, &PL1);
                                if (Check_if_Equal2(PL.kat, PL1.kat)) st_load[st_load_no].reversed = 0;
                                else st_load[st_load_no].reversed = 1;
                                if (Check_if_Equal(v->magnitude1, v->magnitude2)) {
                                    st_load[st_load_no].uniform = 1; //0  1;  UNIFORM DOESN'T WORK FOR MULTIPLE LOADS ON 1 ELEMENT
                                    st_load[st_load_no].magnitude1 = v->magnitude1 * nm;
                                    st_load[st_load_no].magnitude2 = v->magnitude1 * nm;

                                    st_load[st_load_no].distance1 = 0;
                                    st_load[st_load_no].distance2 = PL1.dl * m0999;
                                } else {
                                    st_load[st_load_no].uniform = 0;
                                    double dmag = (v->magnitude2 - v->magnitude1) / PL.dl;
                                    double dl1, dl2;

                                    dl1 = sqrt((L.x1 - v->x1) * (L.x1 - v->x1) + (L.y1 - v->y1) * (L.y1 - v->y1));
                                    dl2 = sqrt((L.x2 - v->x1) * (L.x2 - v->x1) + (L.y2 - v->y1) * (L.y2 - v->y1));
                                    st_load[st_load_no].magnitude1 = v->magnitude1 + dmag * dl1 * nm;
                                    st_load[st_load_no].magnitude2 = v->magnitude1 + dmag * dl2 * nm;

                                    st_load[st_load_no].distance1 = 0;
                                    st_load[st_load_no].distance2 = PL1.dl * m0999;

                                }
                                st_load[st_load_no].factor_record = v_factor_record;
                                add_load();

                                partial_length -= PL1.dl;
                                if (Check_if_LE2(partial_length, 0.0)) break;

                            }
                                //load spread over first node of the element and beginning of load inside element
                            else if ((CheckIsPointOnLineSegment(st_node[st_element[i].node1].x,
                                                                st_node[st_element[i].node1].y,
                                                                v->x1, v->y1, v->x2, v->y2, 0.0001)) &&
                                     ((CheckIsPointOnLineSegment(v->x1, v->y1, st_node[st_element[i].node1].x,
                                                                 st_node[st_element[i].node1].y,
                                                                 st_node[st_element[i].node2].x,
                                                                 st_node[st_element[i].node2].y, 0.0001)) ||
                                      (CheckIsPointOnLineSegment(v->x2, v->y2, st_node[st_element[i].node1].x,
                                                                 st_node[st_element[i].node1].y,
                                                                 st_node[st_element[i].node2].x,
                                                                 st_node[st_element[i].node2].y, 0.0001)))) {
                                parametry_lini(v, &PL);
                                L.x1 = st_node[st_element[i].node1].x;
                                L.y1 = st_node[st_element[i].node1].y;
                                L.x2 = st_node[st_element[i].node2].x;
                                L.y2 = st_node[st_element[i].node2].y;
                                parametry_lini(&L, &PL1);
                                double kat1 = Angle_Normal(Pi_ * PL.kat / 180);
                                double kat2 = Angle_Normal(Pi_ * PL1.kat / 180);
                                double kat2_ = Angle_Normal(Pi_ * PL1.kat + 180 / 180);
                                if ((Check_if_Equal2(kat1, kat2)) || (Check_if_Equal2(kat1, kat2_))) {
                                    st_load[st_load_no].element = i;
                                    st_load[st_load_no].spread = 0;
                                    st_load[st_load_no].partial = 1;
                                    if (Check_if_Equal2(kat1, kat2)) st_load[st_load_no].reversed = 0;
                                    else st_load[st_load_no].reversed = 1;
                                    st_load[st_load_no].uniform = 0;
                                    //caclulationg of the magnitudes
                                    double dmag = (v->magnitude2 - v->magnitude1) / PL.dl;
                                    double dl1, dl2;

                                    dl1 = sqrt((L.x1 - v->x1) * (L.x1 - v->x1) + (L.y1 - v->y1) * (L.y1 - v->y1));
                                    st_load[st_load_no].magnitude1 = v->magnitude1 + dmag * dl1 * nm;
                                    st_load[st_load_no].magnitude2 = v->magnitude2 * nm;

                                    st_load[st_load_no].distance1 = 0;
                                    st_load[st_load_no].distance2 = sqrt(
                                            (v->x2 - L.x1) * (v->x2 - L.x1) + (v->y2 - L.y1) * (v->y2 - L.y1));

                                    if (!Check_if_Equal(st_load[st_load_no].distance2, 0.0)) {
                                        partial_length -= st_load[st_load_no].distance2;
                                        st_load[st_load_no].factor_record = v_factor_record;
                                        add_load();
                                        if (Check_if_LE2(partial_length, 0.0)) break;
                                    }
                                }
                            }
                                //load spread over second node of the element and beginning of load inside element
                            else if ((CheckIsPointOnLineSegment(st_node[st_element[i].node2].x,
                                                                st_node[st_element[i].node2].y,
                                                                v->x1, v->y1, v->x2, v->y2, 0.0001)) &&
                                     ((CheckIsPointOnLineSegment(v->x1, v->y1, st_node[st_element[i].node1].x,
                                                                 st_node[st_element[i].node1].y,
                                                                 st_node[st_element[i].node2].x,
                                                                 st_node[st_element[i].node2].y, 0.0001)) ||
                                      (CheckIsPointOnLineSegment(v->x2, v->y2, st_node[st_element[i].node1].x,
                                                                 st_node[st_element[i].node1].y,
                                                                 st_node[st_element[i].node2].x,
                                                                 st_node[st_element[i].node2].y, 0.0001)))) {
                                parametry_lini(v, &PL);
                                L.x1 = st_node[st_element[i].node1].x;
                                L.y1 = st_node[st_element[i].node1].y;
                                L.x2 = st_node[st_element[i].node2].x;
                                L.y2 = st_node[st_element[i].node2].y;
                                parametry_lini(&L, &PL1);
                                double kat1 = Angle_Normal(Pi_ * PL.kat / 180);
                                double kat2 = Angle_Normal(Pi_ * PL1.kat / 180);
                                double kat2_ = Angle_Normal(Pi_ * PL1.kat + 180 / 180);
                                if ((Check_if_Equal2(kat1, kat2)) || (Check_if_Equal2(kat1, kat2_))) {
                                    st_load[st_load_no].element = i;
                                    st_load[st_load_no].spread = 0;
                                    st_load[st_load_no].partial = 1;
                                    if (Check_if_Equal2(kat1, kat2)) st_load[st_load_no].reversed = 0;
                                    else st_load[st_load_no].reversed = 1;
                                    st_load[st_load_no].uniform = 0;
                                    //caclulationg of the magnitudes
                                    double dmag = (v->magnitude2 - v->magnitude1) / PL.dl;
                                    double dl1, dl2;

                                    dl1 = sqrt((L.x1 - v->x1) * (L.x1 - v->x1) + (L.y1 - v->y1) * (L.y1 - v->y1));
                                    dl2 = sqrt((L.x2 - v->x1) * (L.x2 - v->x1) + (L.y2 - v->y1) * (L.y2 - v->y1));
                                    st_load[st_load_no].magnitude1 = v->magnitude1 * nm;
                                    st_load[st_load_no].magnitude2 = v->magnitude1 + dmag * dl2 * nm;

                                    st_load[st_load_no].distance1 = sqrt(
                                            (v->x1 - L.x1) * (v->x1 - L.x1) + (v->y1 - L.y1) * (v->y1 - L.y1));
                                    st_load[st_load_no].distance2 = PL1.dl * m0999;

                                    if (!Check_if_Equal(st_load[st_load_no].distance2,
                                                        st_load[st_load_no].distance1)) {
                                        partial_length -= (st_load[st_load_no].distance2 -
                                                           st_load[st_load_no].distance1);
                                        st_load[st_load_no].factor_record = v_factor_record;
                                        add_load();
                                        if (Check_if_LE2(partial_length, 0.0)) break;
                                    }

                                }
                            }
                        }

                        if (st_load_no > 0) {
                            for (int ii = 0; ii < st_load_no; ii++) {
                                if ((st_load[ii].uniform == 1) && (st_load[ii].spread == 1)) {
                                    if (st_load[ii].reversed == 0) n1 = 1;
                                    else n1 = -1;

                                    st_uniform_load[st_uniform_load_no].element = st_load[ii].element;
                                    st_uniform_load[st_uniform_load_no].layer = v->warstwa;
                                    switch (v->style) {
                                        case 10:
                                            parametry_lini(v, &PL);
                                            kos = sin(Angle_Normal((PL.kat) * Pi / 180));
                                            koc = cos(Angle_Normal((PL.kat) * Pi / 180));

                                            //st_uniform_load[st_uniform_load_no].element = element;
                                            if (st_load[ii].reversed == 0) {
                                                st_uniform_load[st_uniform_load_no].qx =
                                                        n1 * st_load[ii].magnitude1 * kos;
                                                st_uniform_load[st_uniform_load_no].qy =
                                                        n1 * -st_load[ii].magnitude1 * koc;
                                            } else {
                                                st_uniform_load[st_uniform_load_no].qx =
                                                        n1 * st_load[ii].magnitude1 * kos;
                                                st_uniform_load[st_uniform_load_no].qy =
                                                        n1 * -st_load[ii].magnitude1 * koc;
                                            }
                                            break;
                                        case 11:
                                            parametry_lini(v, &PL);
                                            kos = sin(Angle_Normal(PL.kat * Pi / 180));
                                            koc = cos(Angle_Normal(PL.kat * Pi / 180));

                                            //st_uniform_load[st_uniform_load_no].element = element;
                                            if (st_load[ii].reversed == 0) {
                                                st_uniform_load[st_uniform_load_no].qx =
                                                        n1 * st_load[ii].magnitude1 * koc;
                                                st_uniform_load[st_uniform_load_no].qy =
                                                        n1 * -st_load[ii].magnitude1 * kos;
                                            } else {
                                                st_uniform_load[st_uniform_load_no].qx =
                                                        n1 * st_load[ii].magnitude1 * koc;
                                                st_uniform_load[st_uniform_load_no].qy =
                                                        n1 * -st_load[ii].magnitude1 * kos;
                                            }
                                            break;
                                        case 12:

                                            if (st_load[ii].reversed == 0) {
                                                st_uniform_load[st_uniform_load_no].qx = 0;
                                                st_uniform_load[st_uniform_load_no].qy =
                                                        n1 * -st_load[ii].magnitude1;
                                            } else {
                                                st_uniform_load[st_uniform_load_no].qx = 0;
                                                st_uniform_load[st_uniform_load_no].qy =
                                                        n1 * -st_load[ii].magnitude1;
                                            }
                                            break;
                                        case 13:
                                            parametry_lini(v, &PL);
                                            kos = sin(Angle_Normal((PL.kat) * Pi / 180));
                                            koc = cos(Angle_Normal((PL.kat) * Pi / 180));

                                            if (st_load[ii].reversed == 0) {
                                                st_uniform_load[st_uniform_load_no].qx =
                                                        n1 * -(st_load[ii].magnitude1 * koc) * kos;
                                                st_uniform_load[st_uniform_load_no].qy =
                                                        n1 * -(st_load[ii].magnitude1 * koc) * koc;
                                            } else {
                                                st_uniform_load[st_uniform_load_no].qx =
                                                        n1 * -(st_load[ii].magnitude1 * koc) * kos;
                                                st_uniform_load[st_uniform_load_no].qy =
                                                        n1 * -(st_load[ii].magnitude1 * koc) * koc;
                                            }
                                            break;
                                        case 14:
                                            parametry_lini(v, &PL);
                                            kos = sin(Angle_Normal(PL.kat * Pi / 180));
                                            koc = cos(Angle_Normal(PL.kat * Pi / 180));

                                            if (st_load[ii].reversed == 0) {
                                                st_uniform_load[st_uniform_load_no].qx =
                                                        n1 * (st_load[ii].magnitude1 * kos) * koc;
                                                st_uniform_load[st_uniform_load_no].qy =
                                                        n1 * -(st_load[ii].magnitude1 * kos) * kos;
                                            } else {
                                                st_uniform_load[st_uniform_load_no].qx =
                                                        n1 * -(st_load[ii].magnitude1 * kos) * koc;
                                                st_uniform_load[st_uniform_load_no].qy =
                                                        n1 * (st_load[ii].magnitude1 * kos) * kos;
                                            }
                                            break;
                                    }
                                    st_uniform_load[st_uniform_load_no].factor_record = st_load[ii].factor_record;
                                    add_uniform_load();
                                } else  //trapezoidal
                                {
                                    st_trapezoid_load[st_trapezoid_load_no].element = st_load[ii].element;
                                    st_trapezoid_load[st_trapezoid_load_no].layer = v->warstwa;
                                    switch (v->style) {
                                        case 10:
                                            parametry_lini(v, &PL);
                                            kos = sin(Angle_Normal((PL.kat) * Pi / 180));
                                            koc = cos(Angle_Normal((PL.kat) * Pi / 180));

                                            if (st_load[ii].reversed == 0) {
                                                st_trapezoid_load[st_trapezoid_load_no].qx1 =
                                                        -st_load[ii].magnitude1 * kos;
                                                st_trapezoid_load[st_trapezoid_load_no].qy1 =
                                                        -st_load[ii].magnitude1 * koc;
                                                st_trapezoid_load[st_trapezoid_load_no].qx2 =
                                                        -st_load[ii].magnitude2 * kos;
                                                st_trapezoid_load[st_trapezoid_load_no].qy2 =
                                                        -st_load[ii].magnitude2 * koc;
                                            } else {
                                                st_trapezoid_load[st_trapezoid_load_no].qx1 =
                                                        st_load[ii].magnitude2 * kos;
                                                st_trapezoid_load[st_trapezoid_load_no].qy1 =
                                                        st_load[ii].magnitude2 * koc;
                                                st_trapezoid_load[st_trapezoid_load_no].qx2 =
                                                        st_load[ii].magnitude1 * kos;
                                                st_trapezoid_load[st_trapezoid_load_no].qy2 =
                                                        st_load[ii].magnitude1 * koc;
                                            }

                                            break;
                                        case 11:
                                            parametry_lini(v, &PL);
                                            kos = sin(Angle_Normal(PL.kat * Pi / 180));
                                            koc = cos(Angle_Normal(PL.kat * Pi / 180));

                                            if (st_load[ii].reversed == 0) {
                                                st_trapezoid_load[st_trapezoid_load_no].qx1 =
                                                        st_load[ii].magnitude1 * koc;
                                                st_trapezoid_load[st_trapezoid_load_no].qy1 =
                                                        -st_load[ii].magnitude1 * kos;
                                                st_trapezoid_load[st_trapezoid_load_no].qx2 =
                                                        st_load[ii].magnitude2 * koc;
                                                st_trapezoid_load[st_trapezoid_load_no].qy2 =
                                                        -st_load[ii].magnitude2 * kos;
                                            } else {
                                                st_trapezoid_load[st_trapezoid_load_no].qx1 =
                                                        -st_load[ii].magnitude2 * koc;
                                                st_trapezoid_load[st_trapezoid_load_no].qy1 =
                                                        st_load[ii].magnitude2 * kos;
                                                st_trapezoid_load[st_trapezoid_load_no].qx2 =
                                                        -st_load[ii].magnitude1 * koc;
                                                st_trapezoid_load[st_trapezoid_load_no].qy2 =
                                                        st_load[ii].magnitude1 * kos;
                                            }
                                            break;
                                        case 12:

                                            if (st_load[ii].reversed == 0) {
                                                st_trapezoid_load[st_trapezoid_load_no].qx1 = 0;
                                                st_trapezoid_load[st_trapezoid_load_no].qy1 = -st_load[ii].magnitude1;
                                                st_trapezoid_load[st_trapezoid_load_no].qx2 = 0;
                                                st_trapezoid_load[st_trapezoid_load_no].qy2 = -st_load[ii].magnitude2;
                                            } else {
                                                st_trapezoid_load[st_trapezoid_load_no].qx1 = 0;
                                                st_trapezoid_load[st_trapezoid_load_no].qy1 = st_load[ii].magnitude2;
                                                st_trapezoid_load[st_trapezoid_load_no].qx2 = 0;
                                                st_trapezoid_load[st_trapezoid_load_no].qy2 = st_load[ii].magnitude2;
                                            }
                                            break;
                                        case 13:
                                            parametry_lini(v, &PL);
                                            kos = sin(Angle_Normal((PL.kat) * Pi / 180));
                                            koc = cos(Angle_Normal((PL.kat) * Pi / 180));

                                            if (st_load[ii].reversed == 0) {
                                                st_trapezoid_load[st_trapezoid_load_no].qx1 =
                                                        -(st_load[ii].magnitude1 * koc) * kos;
                                                st_trapezoid_load[st_trapezoid_load_no].qy1 =
                                                        -(st_load[ii].magnitude1 * koc) * koc;
                                                st_trapezoid_load[st_trapezoid_load_no].qx2 =
                                                        -(st_load[ii].magnitude2 * koc) * kos;
                                                st_trapezoid_load[st_trapezoid_load_no].qy2 =
                                                        -(st_load[ii].magnitude2 * koc) * koc;
                                            } else {
                                                st_trapezoid_load[st_trapezoid_load_no].qx1 =
                                                        (st_load[ii].magnitude2 * koc) * kos;
                                                st_trapezoid_load[st_trapezoid_load_no].qy1 =
                                                        (st_load[ii].magnitude2 * koc) * koc;
                                                st_trapezoid_load[st_trapezoid_load_no].qx2 =
                                                        (st_load[ii].magnitude1 * koc) * kos;
                                                st_trapezoid_load[st_trapezoid_load_no].qy2 =
                                                        (st_load[ii].magnitude1 * koc) * koc;
                                            }
                                            break;
                                        case 14:
                                            parametry_lini(v, &PL);
                                            kos = sin(Angle_Normal(PL.kat * Pi / 180));
                                            koc = cos(Angle_Normal(PL.kat * Pi / 180));

                                            if (st_load[ii].reversed == 0) {
                                                st_trapezoid_load[st_trapezoid_load_no].qx1 =
                                                        (st_load[ii].magnitude1 * kos) * koc;
                                                st_trapezoid_load[st_trapezoid_load_no].qy1 =
                                                        -(st_load[ii].magnitude1 * kos) * kos;
                                                st_trapezoid_load[st_trapezoid_load_no].qx2 =
                                                        (st_load[ii].magnitude2 * kos) * koc;
                                                st_trapezoid_load[st_trapezoid_load_no].qy2 =
                                                        -(st_load[ii].magnitude2 * kos) * kos;
                                            } else {
                                                st_trapezoid_load[st_trapezoid_load_no].qx1 =
                                                        (st_load[ii].magnitude2 * kos) * koc;
                                                st_trapezoid_load[st_trapezoid_load_no].qy1 =
                                                        -(st_load[ii].magnitude2 * kos) * kos;
                                                st_trapezoid_load[st_trapezoid_load_no].qx2 =
                                                        (st_load[ii].magnitude1 * kos) * koc;
                                                st_trapezoid_load[st_trapezoid_load_no].qy2 =
                                                        -(st_load[ii].magnitude1 * kos) * kos;
                                            }
                                            break;
                                    }

                                    if (st_load[ii].partial == 1) {
                                        st_trapezoid_load[st_trapezoid_load_no].dlxx1 = st_load[ii].distance1;
                                        st_trapezoid_load[st_trapezoid_load_no].dlxy1 = st_load[ii].distance1;
                                        st_trapezoid_load[st_trapezoid_load_no].dlxx2 = st_load[ii].distance2;
                                        st_trapezoid_load[st_trapezoid_load_no].dlxy2 = st_load[ii].distance2;
                                    } else {

                                        if (st_load[ii].reversed == 0) {
                                            st_trapezoid_load[st_trapezoid_load_no].dlxx1 = sqrt(
                                                    (v->x1 - st_node[st_element[st_load[ii].element].node1].x) *
                                                    (v->x1 - st_node[st_element[st_load[ii].element].node1].x) +
                                                    (v->y1 - st_node[st_element[st_load[ii].element].node1].y) *
                                                    (v->y1 - st_node[st_element[st_load[ii].element].node1].y));

                                            if (st_trapezoid_load[st_trapezoid_load_no].dlxx1 >
                                                st_element[st_load[ii].element].length * m0999)
                                                st_trapezoid_load[st_trapezoid_load_no].dlxx1 =
                                                        st_element[st_load[ii].element].length * m0999;

                                            st_trapezoid_load[st_trapezoid_load_no].dlxy1 = st_trapezoid_load[st_trapezoid_load_no].dlxx1;

                                            st_trapezoid_load[st_trapezoid_load_no].dlxx2 = sqrt(
                                                    (v->x2 - st_node[st_element[st_load[ii].element].node1].x) *
                                                    (v->x2 - st_node[st_element[st_load[ii].element].node1].x) +
                                                    (v->y2 - st_node[st_element[st_load[ii].element].node1].y) *
                                                    (v->y2 - st_node[st_element[st_load[ii].element].node1].y));

                                            if (st_trapezoid_load[st_trapezoid_load_no].dlxx2 >
                                                st_element[st_load[ii].element].length * m0999)
                                                st_trapezoid_load[st_trapezoid_load_no].dlxx2 =
                                                        st_element[st_load[ii].element].length * m0999;

                                            st_trapezoid_load[st_trapezoid_load_no].dlxy2 = st_trapezoid_load[st_trapezoid_load_no].dlxx2;
                                        } else {
                                            st_trapezoid_load[st_trapezoid_load_no].dlxx1 = sqrt(
                                                    (v->x1 - st_node[st_element[st_load[ii].element].node2].x) *
                                                    (v->x1 - st_node[st_element[st_load[ii].element].node2].x) +
                                                    (v->y1 - st_node[st_element[st_load[ii].element].node2].y) *
                                                    (v->y1 - st_node[st_element[st_load[ii].element].node2].y));

                                            if (st_trapezoid_load[st_trapezoid_load_no].dlxx1 >
                                                st_element[st_load[ii].element].length * m0999)
                                                st_trapezoid_load[st_trapezoid_load_no].dlxx1 =
                                                        st_element[st_load[ii].element].length * m0999;

                                            st_trapezoid_load[st_trapezoid_load_no].dlxy1 = st_trapezoid_load[st_trapezoid_load_no].dlxx1;
                                            st_trapezoid_load[st_trapezoid_load_no].dlxx2 = sqrt(
                                                    (v->x2 - st_node[st_element[st_load[ii].element].node2].x) *
                                                    (v->x2 - st_node[st_element[st_load[ii].element].node2].x) +
                                                    (v->y2 - st_node[st_element[st_load[ii].element].node2].y) *
                                                    (v->y2 - st_node[st_element[st_load[ii].element].node2].y));

                                            if (st_trapezoid_load[st_trapezoid_load_no].dlxx2 >
                                                st_element[st_load[ii].element].length * m0999)
                                                st_trapezoid_load[st_trapezoid_load_no].dlxx2 =
                                                        st_element[st_load[ii].element].length * m0999;

                                            st_trapezoid_load[st_trapezoid_load_no].dlxy2 = st_trapezoid_load[st_trapezoid_load_no].dlxx2;
                                        }
                                    }
                                    st_trapezoid_load[st_trapezoid_load_no].factor_record = st_load[ii].factor_record;
                                    add_trapezoid_load();
                                }
                            }
                        } else {
                            sprintf(report_row, "<%f;%f> <%f;%f> [%s] %s\n", milimetryobx(v->x1),
                                    milimetryoby(v->y1),
                                    milimetryobx(v->x2), milimetryoby(v->y2), (*mVector.pola)[v->style + 1].txt,
                                    _load_not_associated_);
                            strcat(report, report_row);
                        }
                        break;
                }
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, Ovector);
    }

    obiekt_tok((char *) ADP, ADK, (char **) &nag, Ovector);
    while (nag != NULL) {
        if (TRUE == Check_Attribute(nag->atrybut, Ablok)) {
            v = (AVECTOR *) nag;
            parametry_lini(v, &PL);

            if (TestBit(st_layer, v->warstwa)) {
                st_thermal_load[st_thermal_load_no].layer = v->warstwa;
                st_thermal_load[st_thermal_load_no].element = -1;
                st_thermal_load[st_thermal_load_no].ydepth = 0;
                st_thermal_load[st_thermal_load_no].deltatplus = 0;
                st_thermal_load[st_thermal_load_no].deltatminus = 0;
                if (v->variant > 0)
                    st_thermal_load[st_thermal_load_no].factor_record = factor_record(v->load, v->variant);
                else
                    st_thermal_load[st_thermal_load_no].factor_record = -(v->load == 0 ? 10
                                                                                       : v->load);  //if no load spec, is assumed to be TL, so load=10
                st_thermal_load[st_thermal_load_no].take_it = 0;

                int v_factor_record = st_thermal_load[st_thermal_load_no].factor_record;

                if (v->style == 15) {
                    int th_load_el = 0;
                    for (i = 0; i < st_element_no; i++)  //node1
                    {
                        L.x1 = st_node[st_element[i].node1].x;  //node1r node2r
                        L.y1 = st_node[st_element[i].node1].y;
                        L.x2 = st_node[st_element[i].node2].x;
                        L.y2 = st_node[st_element[i].node2].y;
                        parametry_lini(&L, &PL1);

                        BOOL found = FALSE;
                        BOOL reversed = FALSE;
                        BOOL the_only_one = FALSE;
                        if (Check_if_Equal(v->x1, st_node[st_element[i].node1].x) &&  //node1r node2r
                            Check_if_Equal(v->y1, st_node[st_element[i].node1].y) &&
                            Check_if_Equal(v->x2, st_node[st_element[i].node2].x) &&
                            Check_if_Equal(v->y2, st_node[st_element[i].node2].y)) {
                            found = TRUE;
                            reversed = FALSE;
                            the_only_one = TRUE;
                            th_load_el++;
                        } else if (Check_if_Equal(v->x2, st_node[st_element[i].node1r].x) &&
                                   Check_if_Equal(v->y2, st_node[st_element[i].node1r].y) &&
                                   Check_if_Equal(v->x1, st_node[st_element[i].node2r].x) &&
                                   Check_if_Equal(v->y1, st_node[st_element[i].node2r].y)) {
                            found = TRUE;
                            reversed = TRUE;
                            the_only_one = TRUE;
                            th_load_el++;
                        }
                            //load spread over entire element
                        else if ((CheckIsPointOnLineSegment(st_node[st_element[i].node1].x,  //node1r node2r
                                                            st_node[st_element[i].node1].y,
                                                            v->x1, v->y1, v->x2, v->y2, 0.0001)) &&
                                 (CheckIsPointOnLineSegment(st_node[st_element[i].node2].x,
                                                            st_node[st_element[i].node2].y,
                                                            v->x1, v->y1, v->x2, v->y2, 0.0001))) {
                            found = TRUE;
                            if (Check_if_Equal2(PL.kat, PL1.kat)) reversed = FALSE;
                            else reversed = TRUE;
                            the_only_one = FALSE;
                            th_load_el++;
                        }
                            //thermal load inside element
                        else if ((CheckIsPointOnLineSegment(v->x1, v->y1,
                                                            st_node[st_element[i].node1].x,  //node1r node2r
                                                            st_node[st_element[i].node1].y,
                                                            st_node[st_element[i].node2].x,
                                                            st_node[st_element[i].node2].y, 0.0001)) &&
                                 (CheckIsPointOnLineSegment(v->x2, v->y2, st_node[st_element[i].node1].x,
                                                            st_node[st_element[i].node1].y,
                                                            st_node[st_element[i].node2].x,
                                                            st_node[st_element[i].node2].y, 0.0001))) {
                            //it's bad
                            sprintf(report_row, "<%f;%f> <%f;%f> [%s] %s <%f;%f> <%f;%f>\n", milimetryobx(v->x1),
                                    milimetryoby(v->y1),
                                    milimetryobx(v->x2), milimetryoby(v->y2), (*mVector.pola)[v->style + 1].txt,
                                    _thermal_load_inside_element_,
                                    milimetryobx(st_node[st_element[i].node1].x),  //node1r node2r
                                    milimetryoby(st_node[st_element[i].node1].y),
                                    milimetryobx(st_node[st_element[i].node2].x),
                                    milimetryoby(st_node[st_element[i].node2].y));
                            strcat(report, report_row);
                            th_load_el++;
                            break;
                        }
                            //load spread over first node of the element and beginning of load inside element
                        else if ((Check_if_Equal2(fabs(PL.kat - PL1.kat), 0) ||
                                  Check_if_Equal2(fabs(PL.kat - PL1.kat), 180)) &&
                                 (CheckIsPointOnLineSegment(st_node[st_element[i].node1].x,  //node1r node2r
                                                            st_node[st_element[i].node1].y,
                                                            v->x1, v->y1, v->x2, v->y2, 0.0001)) &&
                                 (!CheckIsPointOnLineSegment(st_node[st_element[i].node2].x,
                                                             st_node[st_element[i].node2].y,
                                                             v->x1, v->y1, v->x2, v->y2, 0.0001))) {
                            //it's bad
                            sprintf(report_row, "<%f;%f> <%f;%f> [%s] %s <%f;%f> <%f;%f>\n", milimetryobx(v->x1),
                                    milimetryoby(v->y1),
                                    milimetryobx(v->x2), milimetryoby(v->y2), (*mVector.pola)[v->style + 1].txt,
                                    _thermal_load_inside_element_,
                                    milimetryobx(st_node[st_element[i].node1].x),  //node1r node2r
                                    milimetryoby(st_node[st_element[i].node1].y),
                                    milimetryobx(st_node[st_element[i].node2].x),
                                    milimetryoby(st_node[st_element[i].node2].y));
                            strcat(report, report_row);
                            th_load_el++;
                            break;
                        } else if ((Check_if_Equal2(fabs(PL.kat - PL1.kat), 0) ||
                                    Check_if_Equal2(fabs(PL.kat - PL1.kat), 180)) &&
                                   (CheckIsPointOnLineSegment(st_node[st_element[i].node2].x,   //node1r node2r
                                                              st_node[st_element[i].node2].y,
                                                              v->x1, v->y1, v->x2, v->y2, 0.0001)) &&
                                   (!CheckIsPointOnLineSegment(st_node[st_element[i].node1].x,
                                                               st_node[st_element[i].node1].y,
                                                               v->x1, v->y1, v->x2, v->y2, 0.0001))) {
                            //it's bad
                            sprintf(report_row, "<%f;%f> <%f;%f> [%s] %s <%f;%f> <%f;%f>\n", milimetryobx(v->x1),
                                    milimetryoby(v->y1),
                                    milimetryobx(v->x2), milimetryoby(v->y2), (*mVector.pola)[v->style + 1].txt,
                                    _thermal_load_inside_element_,
                                    milimetryobx(st_node[st_element[i].node1].x),  //node1r node2r
                                    milimetryoby(st_node[st_element[i].node1].y),
                                    milimetryobx(st_node[st_element[i].node2].x),
                                    milimetryoby(st_node[st_element[i].node2].y));
                            strcat(report, report_row);
                            th_load_el++;
                            break;
                        }
                        if (found == TRUE) {
                            kos = sin(Angle_Normal(PL.kat * Pi / 180));
                            koc = cos(Angle_Normal(PL.kat * Pi / 180));

                            st_thermal_load[st_thermal_load_no].ydepth = 0.0; //!!!!!!!!!!  h can be zero just in case of symmetrical thermal load

                            for (j = 0; j < st_property_no; j++) {
                                if (abs(st_element[i].property_no) == st_property[j].n) {
                                    st_thermal_load[st_thermal_load_no].ydepth = st_property[j].h;
                                    if (the_only_one == TRUE) break;
                                    else continue;
                                }
                            }

                            st_thermal_load[st_thermal_load_no].element = i;
                            st_thermal_load[st_thermal_load_no].layer = v->warstwa;

                            if (reversed == FALSE) {
                                st_thermal_load[st_thermal_load_no].deltatplus = v->magnitude1;
                                st_thermal_load[st_thermal_load_no].deltatminus = v->magnitude2;
                            } else {
                                st_thermal_load[st_thermal_load_no].deltatplus = v->magnitude2;
                                st_thermal_load[st_thermal_load_no].deltatminus = v->magnitude1;
                            }
                            st_thermal_load[st_thermal_load_no].factor_record = v_factor_record;
                            add_thermal_load();
                            if (the_only_one == TRUE) break;
                            else continue;
                        }
                    }

                    if (th_load_el == 0) {
                        sprintf(report_row, "<%f;%f> <%f;%f> [%s] %s\n", milimetryobx(v->x1), milimetryoby(v->y1),
                                milimetryobx(v->x2), milimetryoby(v->y2), (*mVector.pola)[v->style + 1].txt,
                                _load_not_associated_);
                        strcat(report, report_row);
                    }
                }
            }
        }
        obiekt_tok(NULL, ADK, (char **) &nag, Ovector);
    }


    int combi_uls_no_total = 0;
    int combi_sls_no_total = 0;
    int combi_qpsls_no_total = 0;

    int ULSLC_NO;
    int SLSLC_NO;
    int QPSLSLC_NO;

    combi_total_numbers_no = 0;

    combi_uls_no = 0;
    combi_sls_no = 0;
    combi_qpsls_no = 0;

    //preparing combinations
    int combi_flag;
    int flag_;

    if (ret_standard == 1)  //EUROCODE
    {
        ULSLC = &EUROCODE_ULSLC;
        SLSLC = &EUROCODE_SLSLC;
        QPSLSLC = &EUROCODE_QPSLSLC;

        MC_ULSLC = &EUROCODE_MC_ULSLC;
        MC_SLSLC = &EUROCODE_MC_SLSLC;
        MC_QPSLSLC = &EUROCODE_MC_QPSLSLC;

        ULSLC_NO = EUROCODE_ULSLC_NO;
        SLSLC_NO = EUROCODE_SLSLC_NO;
        QPSLSLC_NO = EUROCODE_QPSLSLC_NO;

        load_flag = load_flag_EU;

    } else if (ret_standard == 2)  //ASCE
    {
        ULSLC = &ASCE_ULSLC;
        SLSLC = &ASCE_SLSLC;
        QPSLSLC = &ASCE_QPSLSLC;

        MC_ULSLC = &ASCE_MC_ULSLC;
        MC_SLSLC = &ASCE_MC_SLSLC;
        MC_QPSLSLC = &ASCE_MC_QPSLSLC;

        ULSLC_NO = ASCE_ULSLC_NO;
        SLSLC_NO = ASCE_SLSLC_NO;
        QPSLSLC_NO = ASCE_QPSLSLC_NO;

        load_flag = load_flag_ASCE;
    } else if (ret_standard == 3)  //ICC
    {
        ULSLC = &ICC_ULSLC;
        SLSLC = &ICC_SLSLC;
        QPSLSLC = &ICC_QPSLSLC;

        MC_ULSLC = &ICC_MC_ULSLC;
        MC_SLSLC = &ICC_MC_SLSLC;
        MC_QPSLSLC = &ICC_MC_QPSLSLC;

        ULSLC_NO = ICC_ULSLC_NO;
        SLSLC_NO = ICC_SLSLC_NO;
        QPSLSLC_NO = ICC_QPSLSLC_NO;

        load_flag = load_flag_ICC;
    } else {
        sprintf(report_row, "%s\n", _unknown_standard_);
        strcat(report, report_row);
    }

    //for (i = 0; i < ULSLC_NO; i++)  ULSLC[i].flag = 0;
    //for (i = 0; i < SLSLC_NO; i++) SLSLC[i].flag = 0;
    //for (i = 0; i < QPSLSLC_NO; i++) QPSLSLC[i].flag = 0;

    if (Check_if_Equal(fabs(gY), 0)) combi_flag = 0;
    else combi_flag = 1;

    //nodes forces and moments
    stlc_node_force_moment_no = 0;
    for (i = 0; i < st_node_force_moment_no; i++) {
        if (TestBit(st_layer, st_node_force_moment[i].layer)) {
            if (st_node_force_moment[i].factor_record >= 0) {
                gamma_l = st_load_factors[st_node_force_moment[i].factor_record].gamma;
                flag_ = load_flag[st_load_factors[st_node_force_moment[i].factor_record].load];
                flag_ = load_flag[st_load_factors[st_node_force_moment[i].factor_record].load];
            } else {
                gamma_l = load_factors[abs(st_node_force_moment[i].factor_record)].gamma;
                flag_ = load_flag[load_factors[abs(st_node_force_moment[i].factor_record)].load];
            }
            combi_flag |= flag_;
            stlc_node_force_moment_no++;
        }
    }
    //elements uniform load
    stlc_uniform_load_no = 0;
    for (i = 0; i < st_uniform_load_no; i++) {
        if (TestBit(st_layer, st_uniform_load[i].layer)) {
            if (st_uniform_load[i].factor_record >= 0) {
                gamma_l = st_load_factors[st_uniform_load[i].factor_record].gamma;
                flag_ = load_flag[st_load_factors[st_uniform_load[i].factor_record].load];
            } else {
                gamma_l = load_factors[abs(st_uniform_load[i].factor_record)].gamma;
                flag_ = load_flag[load_factors[abs(st_uniform_load[i].factor_record)].load];
            }
            combi_flag |= flag_;
            stlc_uniform_load_no++;
        }
    }

    //elements trapezoid load
    stlc_trapezoid_load_no = 0;
    for (i = 0; i < st_trapezoid_load_no; i++) {
        if (TestBit(st_layer, st_trapezoid_load[i].layer)) {

            if (st_trapezoid_load[i].factor_record >= 0) {
                gamma_l = st_load_factors[st_trapezoid_load[i].factor_record].gamma;
                flag_ = load_flag[st_load_factors[st_trapezoid_load[i].factor_record].load];
            } else {
                gamma_l = load_factors[abs(st_trapezoid_load[i].factor_record)].gamma;
                flag_ = load_flag[load_factors[abs(st_trapezoid_load[i].factor_record)].load];
            }
            combi_flag |= flag_;
            stlc_trapezoid_load_no++;
        }
    }
    //internal concentrated loads
    stlc_element_force_no = 0;
    for (i = 0; i < st_element_force_no; i++) {
        if (TestBit(st_layer, st_element_force[i].layer)) {
            if (st_element_force[i].factor_record >= 0) {
                gamma_l = st_load_factors[st_element_force[i].factor_record].gamma;
                flag_ = load_flag[st_load_factors[st_element_force[i].factor_record].load];
            } else {
                gamma_l = load_factors[abs(st_element_force[i].factor_record)].gamma;
                flag_ = load_flag[load_factors[abs(st_element_force[i].factor_record)].load];
            }
            combi_flag |= flag_;
            stlc_element_force_no++;
        }
    }
    //temperature loads
    stlc_thermal_load_no = 0;
    for (i = 0; i < st_thermal_load_no; i++) {
        if (TestBit(st_layer, st_thermal_load[i].layer)) {
            if (st_thermal_load[i].factor_record >= 0) {
                gamma_l = st_load_factors[st_thermal_load[i].factor_record].gamma;
                flag_ = load_flag[st_load_factors[st_thermal_load[i].factor_record].load];
            } else {
                gamma_l = load_factors[abs(st_thermal_load[i].factor_record)].gamma;
                flag_ = load_flag[load_factors[abs(st_thermal_load[i].factor_record)].load];
            }
            combi_flag |= flag_;
            stlc_thermal_load_no++;
        }
    }
    //prescribed displacements
    stlc_displacement_no = 0;
    for (i = 0; i < st_displacement_no; i++) {
        if (TestBit(st_layer, st_displacement[i].layer)) {
            if (st_displacement[i].factor_record >= 0) {
                //gamma_l = st_load_factors[st_displacement[i].factor_record].gamma;
                gamma_l = 1.0;  //always 1
                //flag = load_flag[st_load_factors[st_displacement[i].factor_record].load];
            } else {
                //gamma_l = load_factors[abs(st_displacement[i].factor_record)].gamma;
                gamma_l = 1.0;  //always 1
                //flag = load_flag[load_factors[abs(st_displacement[i].factor_record)].load];
            }
            //combi_flag |= flag;  // is taken always
            stlc_displacement_no++;
        }
    }

    //having combi_flag combination flags can be set

    combi_total_numbers[combi_total_numbers_no].case_no = li;
    combi_total_numbers[combi_total_numbers_no].combi = 0;  //SLS
    combi_total_numbers[combi_total_numbers_no].combination = 0;
    combi_total_numbers[combi_total_numbers_no].first = 1;
    combi_total_numbers[combi_total_numbers_no].last = 1;
    combi_total_numbers_no++;
    combi_total_numbers[combi_total_numbers_no].case_no = li;
    combi_total_numbers[combi_total_numbers_no].combi = 1;  //ULS
    combi_total_numbers[combi_total_numbers_no].combination = 0;
    combi_total_numbers[combi_total_numbers_no].first = 1;
    combi_total_numbers[combi_total_numbers_no].last = 1;
    combi_total_numbers_no++;

    int first = 1;
    int last = 0;
    for (i = 0; i < ULSLC_NO; i++) {
        if ((combi_flag & ULSLC[i].mask) &&
            ((MC_ULSLC[i] & combi_flag) || (MC_ULSLC[i] == 0))) {   //some loads included in mask
            ULSLC[i].flag = 1;
            combi_uls_no++;
            combi_total_numbers[combi_total_numbers_no].case_no = li;
            combi_total_numbers[combi_total_numbers_no].combi = 2;  //ULSLC
            combi_total_numbers[combi_total_numbers_no].combination = i + 1;
            combi_total_numbers[combi_total_numbers_no].first = first;
            combi_total_numbers[combi_total_numbers_no].last = 0;
            first = 0;
            combi_total_numbers_no++;
        } else ULSLC[i].flag = 0;
    }
    if (combi_total_numbers_no > 0) combi_total_numbers[combi_total_numbers_no - 1].last = 1;
    if (rout) printf("combi_uls_no=%d\n", combi_uls_no);

    first = 1;
    for (i = 0; i < SLSLC_NO; i++) {
        if ((combi_flag & SLSLC[i].mask) && ((MC_SLSLC[i] & combi_flag) || (MC_SLSLC[i] == 0))) {
            SLSLC[i].flag = 1;
            combi_sls_no++;
            combi_total_numbers[combi_total_numbers_no].case_no = li;
            combi_total_numbers[combi_total_numbers_no].combi = 3;  //SLSLC
            combi_total_numbers[combi_total_numbers_no].combination = i + 1;
            combi_total_numbers[combi_total_numbers_no].first = first;
            combi_total_numbers[combi_total_numbers_no].last = 0;
            first = 0;
            combi_total_numbers_no++;
        } else SLSLC[i].flag = 0;
    }
    if (combi_total_numbers_no > 0) combi_total_numbers[combi_total_numbers_no - 1].last = 1;
    if (rout) printf("combi_sls_no=%d\n", combi_sls_no);

    first = 1;
    for (i = 0; i < QPSLSLC_NO; i++) {
        if ((combi_flag & QPSLSLC[i].mask) && ((MC_QPSLSLC[i] & combi_flag) || (MC_QPSLSLC[i] == 0))) {
            QPSLSLC[i].flag = 1;
            combi_qpsls_no++;
            combi_total_numbers[combi_total_numbers_no].case_no = li;
            combi_total_numbers[combi_total_numbers_no].combi = 4;  //QPSLSLC
            combi_total_numbers[combi_total_numbers_no].combination = i + 1;
            combi_total_numbers[combi_total_numbers_no].first = first;
            combi_total_numbers[combi_total_numbers_no].last = 0;
            first = 0;
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

    combinations_number = 2 + combi_uls_no_total + combi_sls_no_total +
                          combi_qpsls_no_total;  //SLS + ULS for each st_layer_no plus total number of combinations for each state

    fprintf(f, "%d\t\t# number of static load cases\n", combinations_number);

    for (i = 0; i < combinations_number; i++) {
        sprintf(params, "alfacad.if%02d", i + 1);
        if (__file_exists(params)) unlink(params);
    }

    int case_number = 0;
    ST_UNIFORM_LOAD *st_uniform_load_comb, *st_uniform_load_cons;
    int st_uniform_load_no_cons;

    st_uniform_load_comb = malloc((st_uniform_load_no + 1) * sizeof(ST_UNIFORM_LOAD));
    st_uniform_load_cons = malloc((st_uniform_load_no + 1) * sizeof(ST_UNIFORM_LOAD));

    ///// SLS BEGINS
    case_number++;
    fprintf(f, "\t\t# Begin Static Load Case SLS %d (%d)\n\n", combination_no, case_number);

    fprintf(f, "# gravitational acceleration for self-weight loading (global)\n");
    fprintf(f, "# gX		gY		gZ\n");
    fprintf(f, "# in/s^2\t\tin/s^2\t\tin/s^2\n");
    fprintf(f, "# mm/s^2\t\tmm/s^2\t\tmm/s^2\n\n");
    fprintf(f, "%f %f %f\n\n", gX, -gY * unit_factors->g_f, gZ);

    stl_node_force_moment_no = 0;
    for (i = 0; i < st_node_force_moment_no; i++) {
        if (TestBit(st_layer, st_node_force_moment[i].layer)) stl_node_force_moment_no++;
    }

    //nodes forces and moments
    fprintf(f, "%d\t\t# number of loaded nodes (global)\n", stl_node_force_moment_no);
    fprintf(f, "#.n     Fx      Fy     Fz     Mxx     Myy     Mzz\n");
    fprintf(f, "#.      kip     kip    kip    kip-in  kip-in  kip-in\n");
    fprintf(f, "#.       N       N      N     Nmm     Nmm     Nmm\n\n");
    for (i = 0; i < st_node_force_moment_no; i++) {
        if (TestBit(st_layer, st_node_force_moment[i].layer)) {
            set_decimal_format(par[0], st_node_force_moment[i].fx * unit_factors->F_f, force_precision);
            set_decimal_format(par[1], st_node_force_moment[i].fy * unit_factors->F_f, force_precision);
            set_decimal_format(par[2], 0.0, force_precision);
            set_decimal_format(par[3], 0.0, force_precision);
            set_decimal_format(par[4], 0.0, force_precision);
            set_decimal_format(par[5], st_node_force_moment[i].mz * unit_factors->M_f, moment_precision);
            fprintf(f, "%d %s %s %s %s %s %s\n", st_node_force_moment[i].node + 1, par[0], par[1], par[2], par[3],
                    par[4], par[5]);
        }
    }

    //elements uniform load
    stl_uniform_load_no = 0;
    for (i = 0; i < st_uniform_load_no; i++) {
        if (TestBit(st_layer, st_uniform_load[i].layer)) stl_uniform_load_no++;
    }

    memmove(st_uniform_load_comb, st_uniform_load, sizeof(ST_UNIFORM_LOAD) * st_uniform_load_no);

    for (i = 0; i < st_uniform_load_no; i++) {
        if (TestBit(st_layer, st_uniform_load[i].layer)) {
            st_uniform_load_comb[i].qx = st_uniform_load[i].qx;
            st_uniform_load_comb[i].qy = st_uniform_load[i].qy;

            st_uniform_load_comb[i].take_it = 1;
        }
    }

    st_uniform_load_no_cons = 0;
    consolidate_loads(st_uniform_load_comb, st_uniform_load_no, st_uniform_load_cons, &st_uniform_load_no_cons,
                      st_layer);

    fprintf(f, "\n%d\t\t# number of uniformly-distributed element loads (local)\n", st_uniform_load_no_cons);
    fprintf(f, "#.elmnt  X-load   Y-load   Z-load   uniform member loads in member coordinates\n");
    fprintf(f, "# .      kip/in   kip/in   kip/in\n");
    fprintf(f, "# .      N/mm     N/mm     N/mm\n\n");

    for (i = 0; i < st_uniform_load_no_cons; i++) {
        if (TestBit(st_layer, st_uniform_load_cons[i].layer)) {
            set_decimal_format(par[0], st_uniform_load_cons[i].qx * unit_factors->q_f, load_precision);
            set_decimal_format(par[1], st_uniform_load_cons[i].qy * unit_factors->q_f, load_precision);
            set_decimal_format(par[2], 0.0, load_precision);

            fprintf(f, "%d %s %s %s\n", st_uniform_load_cons[i].element + 1, par[0], par[1], par[2]);
        }
    }

    //elements trapezoid load
    stl_trapezoid_load_no = 0;
    for (i = 0; i < st_trapezoid_load_no; i++) {
        if (TestBit(st_layer, st_trapezoid_load[i].layer)) stl_trapezoid_load_no++;
    }
    fprintf(f, "\n%d\t\t# number of trapezoidal loads\n", stl_trapezoid_load_no);
    fprintf(f, "#       start    stop     start    stop\n");
    fprintf(f, "#.elmnt loc'n    loc'n    load     load\n");
    fprintf(f, "# .      in       in      kip/in   kip/in\n");
    fprintf(f, "# .      mm       mm      N/mm     N/mm\n\n");

    for (i = 0; i < st_trapezoid_load_no; i++) {
        if (TestBit(st_layer, st_trapezoid_load[i].layer)) {
            set_decimal_format(par[0], milimetryob(st_trapezoid_load[i].dlxx1 * units_factor), dim_precision);
            set_decimal_format(par[1], milimetryob(st_trapezoid_load[i].dlxx2 * units_factor), dim_precision);
            set_decimal_format(par[2], st_trapezoid_load[i].qx1 * unit_factors->q_f, load_precision);
            set_decimal_format(par[3], st_trapezoid_load[i].qx2 * unit_factors->q_f, load_precision);

            set_decimal_format(par[4], milimetryob(st_trapezoid_load[i].dlxy1 * units_factor), dim_precision);
            set_decimal_format(par[5], milimetryob(st_trapezoid_load[i].dlxy2 * units_factor), dim_precision);
            set_decimal_format(par[6], st_trapezoid_load[i].qy1 * unit_factors->q_f, load_precision);
            set_decimal_format(par[7], st_trapezoid_load[i].qy2 * unit_factors->q_f, load_precision);

            set_decimal_format(par[8], 0.0, dim_precision);
            set_decimal_format(par[9], 0.0, dim_precision);
            set_decimal_format(par[10], 0.0, load_precision);
            set_decimal_format(par[11], 0.0, load_precision);

            fprintf(f, "\t\t%d %s %s %s %s %s %s %s %s %s %s %s %s\n", st_trapezoid_load[i].element + 1, par[0], par[1],
                    par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9], par[10], par[11]);
        }
    }

    //internal concentrated loads
    stl_element_force_no = 0;
    for (i = 0; i < st_element_force_no; i++) {
        if (TestBit(st_layer, st_element_force[i].layer)) stl_element_force_no++;
    }
    fprintf(f, "\n%d\t\t# number of concentrated interior point loads (local)\n", stl_element_force_no);
    fprintf(f, "#.elmnt  X-load   Y-load   Z-load    x-loc'n  point loads in member coordinates\n");
    fprintf(f, "#.       kip      kip       kip     in\n");
    fprintf(f, "#.       N        N         N       mm\n\n");

    for (i = 0; i < st_element_force_no; i++) {
        if (TestBit(st_layer, st_element_force[i].layer)) {
            set_decimal_format(par[0], st_element_force[i].fx * unit_factors->F_f, force_precision);
            set_decimal_format(par[1], st_element_force[i].fy * unit_factors->F_f, force_precision);
            set_decimal_format(par[2], 0.0, force_precision);
            set_decimal_format(par[3], milimetryob(st_element_force[i].dlx * units_factor), dim_precision);

            fprintf(f, "%d %s %s %s %s\n", st_element_force[i].element + 1, par[0], par[1], par[2], par[3]);
        }
    }

    //tenperature loads
    stl_thermal_load_no = 0;
    for (i = 0; i < st_thermal_load_no; i++) {
        if (TestBit(st_layer, st_thermal_load[i].layer)) stl_thermal_load_no++;
    }
    fprintf(f, "\n%d\t\t#  number of frame elements with temperature changes (local)\n", stl_thermal_load_no);
    fprintf(f, "#.elmnt   coef.  y-depth  z-depth  deltaTy+  deltaTy-  deltaTz+  deltaTz-\n");
    fprintf(f, "#.         /deg.F    in       in     deg.F     deg.F     deg.F     deg.F\n");
    fprintf(f, "#.         /deg.C    mm       mm     deg.C     deg.C     deg.C     deg.C\n\n");

    for (i = 0; i < st_thermal_load_no; i++) {
        if (TestBit(st_layer, st_thermal_load[i].layer)) {


            for (j = 0; j < st_property_no; j++) {
                if (abs(st_element[st_thermal_load[i].element].property_no) == st_property[j].n)
                    break;
            }

            if (j == st_property_no) {
                sprintf(report_row, "#%d %s\n", st_element[st_thermal_load[i].element].property_no,
                        _property_not_defined_);
                strcat(report, report_row);
                break;
            }

            sprintf(par[0], "%e", st_property[j].a);
            set_decimal_format(par[1], st_thermal_load[i].ydepth, prop_precisions->dim_precision);
            set_decimal_format(par[2], st_thermal_load[i].ydepth, prop_precisions->dim_precision);  //same as y axis
            set_decimal_format(par[3], st_thermal_load[i].deltatplus, t_precision);
            set_decimal_format(par[4], st_thermal_load[i].deltatminus, t_precision);
            set_decimal_format(par[5], 0.0, t_precision);
            set_decimal_format(par[6], 0.0, t_precision);

            fprintf(f, "%d %s %s %s %s %s %s %s\n", st_thermal_load[i].element + 1, par[0], par[1], par[2], par[3],
                    par[4], par[5], par[6]);
        }
    }

    //prescribed displacements
    stl_displacement_no = 0;
    for (i = 0; i < st_displacement_no; i++) {
        if (TestBit(st_layer, st_displacement[i].layer)) stl_displacement_no++;
    }
    fprintf(f, "\n%d\t\t# number of nodes with prescribed displacements  nD<=nR (global)\n", stl_displacement_no);
    fprintf(f, "#.node   X-displ  Y-displ  Z-displ  X-rot'n   Y-rot'n   Z-rot'n\n");
    fprintf(f, "#.         in       in       in      radian    radian    radian\n");
    fprintf(f, "#.         mm       mm       mm      radian    radian    radian\n");

    for (i = 0; i < st_displacement_no; i++) {
        if (TestBit(st_layer, st_displacement[i].layer)) {
            set_decimal_format(par[0], st_displacement[i].dx, dim_precision);
            set_decimal_format(par[1], st_displacement[i].dy, dim_precision);
            set_decimal_format(par[2], 0.0, dim_precision);
            set_decimal_format(par[3], 0.0, dim_precision);
            set_decimal_format(par[4], 0.0, dim_precision);
            set_decimal_format(par[5], st_displacement[i].dzz, dim_precision);

            fprintf(f, "%d %s %s %s %s %s %s\n", st_displacement[i].node + 1, par[0], par[1], par[2], par[3], par[4],
                    par[5]);
        }
    }

    fprintf(f, "\n\t\t# End Static Load Case SLS %d (%d)\n\n", combination_no, case_number);
    //// SLS ENDS


    ///// ULS BEGINS
    case_number++;
    fprintf(f, "\t\t# Begin Static Load Case ULS %d (%d)\n\n", combination_no, case_number);

    fprintf(f, "# gravitational acceleration for self-weight loading (global)\n");
    fprintf(f, "# gX		gY		gZ\n");
    fprintf(f, "# in/s^2\t\tin/s^2\t\tin/s^2\n");
    fprintf(f, "# mm/s^2\t\tmm/s^2\t\tmm/s^2\n\n");
    fprintf(f, "%f %f %f\n\n", gX, -gY * gammas->gamma_g * unit_factors->g_f, gZ);

    stl_node_force_moment_no = 0;
    for (i = 0; i < st_node_force_moment_no; i++) {
        if (TestBit(st_layer, st_node_force_moment[i].layer)) stl_node_force_moment_no++;
    }

    //nodes forces and moments
    fprintf(f, "%d\t\t# number of loaded nodes (global)\n", stl_node_force_moment_no);
    fprintf(f, "#.n     Fx      Fy     Fz     Mxx     Myy     Mzz\n");
    fprintf(f, "#.      kip     kip    kip    kip-in  kip-in  kip-in\n");
    fprintf(f, "#.       N       N      N     Nmm     Nmm     Nmm\n\n");
    for (i = 0; i < st_node_force_moment_no; i++) {
        if (TestBit(st_layer, st_node_force_moment[i].layer)) {
            if (st_node_force_moment[i].factor_record >= 0)
                gamma_l = st_load_factors[st_node_force_moment[i].factor_record].gamma;
            else gamma_l = load_factors[abs(st_node_force_moment[i].factor_record)].gamma;

            set_decimal_format(par[0], st_node_force_moment[i].fx * gamma_l * unit_factors->F_f, force_precision);
            set_decimal_format(par[1], st_node_force_moment[i].fy * gamma_l * unit_factors->F_f, force_precision);
            set_decimal_format(par[2], 0.0, force_precision);
            set_decimal_format(par[3], 0.0, force_precision);
            set_decimal_format(par[4], 0.0, force_precision);
            set_decimal_format(par[5], st_node_force_moment[i].mz * gamma_l * unit_factors->M_f, moment_precision);
            fprintf(f, "%d %s %s %s %s %s %s\n", st_node_force_moment[i].node + 1, par[0], par[1], par[2], par[3],
                    par[4], par[5]);
        }
    }

    //elements uniform load
    stl_uniform_load_no = 0;
    for (i = 0; i < st_uniform_load_no; i++) {
        if (TestBit(st_layer, st_uniform_load[i].layer)) stl_uniform_load_no++;
    }

    //////////////
    memmove(st_uniform_load_comb, st_uniform_load, sizeof(ST_UNIFORM_LOAD) * st_uniform_load_no);

    for (i = 0; i < st_uniform_load_no; i++) {
        if (TestBit(st_layer, st_uniform_load[i].layer)) {
            if (st_uniform_load[i].factor_record >= 0)
                gamma_l = st_load_factors[st_uniform_load[i].factor_record].gamma;
            else gamma_l = load_factors[abs(st_uniform_load[i].factor_record)].gamma;

            st_uniform_load_comb[i].qx = st_uniform_load[i].qx * gamma_l;
            st_uniform_load_comb[i].qy = st_uniform_load[i].qy * gamma_l;

            st_uniform_load_comb[i].take_it = 1;
        }
    }

    st_uniform_load_no_cons = 0;
    consolidate_loads(st_uniform_load_comb, st_uniform_load_no, st_uniform_load_cons, &st_uniform_load_no_cons,
                      st_layer);

    fprintf(f, "\n%d\t\t# number of uniformly-distributed element loads (local)\n", st_uniform_load_no_cons);
    fprintf(f, "#.elmnt  X-load   Y-load   Z-load   uniform member loads in member coordinates\n");
    fprintf(f, "# .      kip/in   kip/in   kip/in\n");
    fprintf(f, "# .      N/mm     N/mm     N/mm\n\n");

    for (i = 0; i < st_uniform_load_no_cons; i++) {
        if (TestBit(st_layer, st_uniform_load_cons[i].layer)) {
            set_decimal_format(par[0], st_uniform_load_cons[i].qx * unit_factors->q_f, load_precision);
            set_decimal_format(par[1], st_uniform_load_cons[i].qy * unit_factors->q_f, load_precision);
            set_decimal_format(par[2], 0.0, load_precision);

            fprintf(f, "%d %s %s %s\n", st_uniform_load_cons[i].element + 1, par[0], par[1], par[2]);
        }
    }
    ///////////////////

    //elements trapezoid load
    stl_trapezoid_load_no = 0;
    for (i = 0; i < st_trapezoid_load_no; i++) {
        if (TestBit(st_layer, st_trapezoid_load[i].layer)) stl_trapezoid_load_no++;
    }
    fprintf(f, "\n%d\t\t# number of trapezoidal loads\n", stl_trapezoid_load_no);
    fprintf(f, "#       start    stop     start    stop\n");
    fprintf(f, "#.elmnt loc'n    loc'n    load     load\n");
    fprintf(f, "# .      in       in      kip/in   kip/in\n");
    fprintf(f, "# .      mm       mm      N/mm     N/mm\n\n");

    for (i = 0; i < st_trapezoid_load_no; i++) {
        if (TestBit(st_layer, st_trapezoid_load[i].layer)) {
            if (st_trapezoid_load[i].factor_record >= 0)
                gamma_l = st_load_factors[st_trapezoid_load[i].factor_record].gamma;
            else gamma_l = load_factors[abs(st_trapezoid_load[i].factor_record)].gamma;

            set_decimal_format(par[0], milimetryob(st_trapezoid_load[i].dlxx1 * units_factor), dim_precision);
            set_decimal_format(par[1], milimetryob(st_trapezoid_load[i].dlxx2 * units_factor), dim_precision);
            set_decimal_format(par[2], st_trapezoid_load[i].qx1 * gamma_l * unit_factors->q_f, load_precision);
            set_decimal_format(par[3], st_trapezoid_load[i].qx2 * gamma_l * unit_factors->q_f, load_precision);

            set_decimal_format(par[4], milimetryob(st_trapezoid_load[i].dlxy1 * units_factor), dim_precision);
            set_decimal_format(par[5], milimetryob(st_trapezoid_load[i].dlxy2 * units_factor), dim_precision);
            set_decimal_format(par[6], st_trapezoid_load[i].qy1 * gamma_l * unit_factors->q_f, load_precision);
            set_decimal_format(par[7], st_trapezoid_load[i].qy2 * gamma_l * unit_factors->q_f, load_precision);

            set_decimal_format(par[8], 0.0, dim_precision);
            set_decimal_format(par[9], 0.0, dim_precision);
            set_decimal_format(par[10], 0.0, load_precision);
            set_decimal_format(par[11], 0.0, load_precision);

            fprintf(f, "\t\t%d %s %s %s %s %s %s %s %s %s %s %s %s\n", st_trapezoid_load[i].element + 1, par[0], par[1],
                    par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9], par[10], par[11]);
        }
    }

    //internal concentrated loads
    stl_element_force_no = 0;
    for (i = 0; i < st_element_force_no; i++) {
        if (TestBit(st_layer, st_element_force[i].layer)) stl_element_force_no++;
    }
    fprintf(f, "\n%d\t\t# number of concentrated interior point loads (local)\n", stl_element_force_no);
    fprintf(f, "#.elmnt  X-load   Y-load   Z-load    x-loc'n  point loads in member coordinates\n");
    fprintf(f, "#.       kip      kip       kip     in\n");
    fprintf(f, "#.       N        N         N       mm\n\n");

    for (i = 0; i < st_element_force_no; i++) {
        if (TestBit(st_layer, st_element_force[i].layer)) {
            if (st_element_force[i].factor_record >= 0)
                gamma_l = st_load_factors[st_element_force[i].factor_record].gamma;
            else gamma_l = load_factors[abs(st_element_force[i].factor_record)].gamma;

            set_decimal_format(par[0], st_element_force[i].fx * gamma_l * unit_factors->F_f, force_precision);
            set_decimal_format(par[1], st_element_force[i].fy * gamma_l * unit_factors->F_f, force_precision);
            set_decimal_format(par[2], 0.0, force_precision);
            set_decimal_format(par[3], milimetryob(st_element_force[i].dlx * units_factor), dim_precision);

            fprintf(f, "%d %s %s %s %s\n", st_element_force[i].element + 1, par[0], par[1], par[2], par[3]);
        }
    }

    //tenperature loads
    stl_thermal_load_no = 0;
    for (i = 0; i < st_thermal_load_no; i++) {
        if (TestBit(st_layer, st_thermal_load[i].layer)) stl_thermal_load_no++;
    }
    fprintf(f, "\n%d\t\t#  number of frame elements with temperature changes (local)\n", stl_thermal_load_no);
    fprintf(f, "#.elmnt   coef.  y-depth  z-depth  deltaTy+  deltaTy-  deltaTz+  deltaTz-\n");
    fprintf(f, "#.         /deg.F    in       in     deg.F     deg.F     deg.F     deg.F\n");
    fprintf(f, "#.         /deg.C    mm       mm     deg.C     deg.C     deg.C     deg.C\n\n");

    for (i = 0; i < st_thermal_load_no; i++) {
        if (TestBit(st_layer, st_thermal_load[i].layer)) {
            if (st_thermal_load[i].factor_record >= 0)
                gamma_l = st_load_factors[st_thermal_load[i].factor_record].gamma;
            else gamma_l = load_factors[abs(st_thermal_load[i].factor_record)].gamma;

            for (j = 0; j < st_property_no; j++) {
                if (abs(st_element[st_thermal_load[i].element].property_no) == st_property[j].n)
                    break;
            }

            if (j == st_property_no) {
                sprintf(report_row, "#%d %s\n", st_element[st_thermal_load[i].element].property_no,
                        _property_not_defined_);
                strcat(report, report_row);
                break;
            }

            sprintf(par[0], "%e", st_property[j].a);
            set_decimal_format(par[1], st_thermal_load[i].ydepth, prop_precisions->dim_precision);
            set_decimal_format(par[2], st_thermal_load[i].ydepth, prop_precisions->dim_precision);  //same as y axis
            set_decimal_format(par[3], st_thermal_load[i].deltatplus * gamma_l, t_precision);
            set_decimal_format(par[4], st_thermal_load[i].deltatminus * gamma_l, t_precision);
            set_decimal_format(par[5], 0.0, t_precision);
            set_decimal_format(par[6], 0.0, t_precision);

            fprintf(f, "%d %s %s %s %s %s %s %s\n", st_thermal_load[i].element + 1, par[0], par[1], par[2], par[3],
                    par[4], par[5], par[6]);
        }
    }

    //prescribed displacements
    stl_displacement_no = 0;
    for (i = 0; i < st_displacement_no; i++) {
        if (TestBit(st_layer, st_displacement[i].layer)) stl_displacement_no++;
    }
    fprintf(f, "\n%d\t\t# number of nodes with prescribed displacements  nD<=nR (global)\n", stl_displacement_no);
    fprintf(f, "#.node   X-displ  Y-displ  Z-displ  X-rot'n   Y-rot'n   Z-rot'n\n");
    fprintf(f, "#.         in       in       in      radian    radian    radian\n");
    fprintf(f, "#.         mm       mm       mm      radian    radian    radian\n");

    for (i = 0; i < st_displacement_no; i++) {
        if (TestBit(st_layer, st_displacement[i].layer)) {
            //no partial factors (assumed)

            set_decimal_format(par[0], st_displacement[i].dx, dim_precision);
            set_decimal_format(par[1], st_displacement[i].dy, dim_precision);
            set_decimal_format(par[2], 0.0, dim_precision);
            set_decimal_format(par[3], 0.0, dim_precision);
            set_decimal_format(par[4], 0.0, dim_precision);
            set_decimal_format(par[5], st_displacement[i].dzz, dim_precision);

            fprintf(f, "%d %s %s %s %s %s %s\n", st_displacement[i].node + 1, par[0], par[1], par[2], par[3], par[4],
                    par[5]);
        }
    }

    fprintf(f, "\n\t\t# End Static Load Case ULS %d (%d)\n\n", combination_no, case_number);
    //// ULS ENDS

    //// ULS COMBINATIONS BEGIN
    for (int ci = 0; ci < ULSLC_NO; ci++) {
        if (ULSLC[ci].flag == 1) {
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

            case_number++;
            fprintf(f, "\t\t# Begin Static Load Case ULS %d Combination %d (%d)\n\n", combination_no, ci + 1,
                    case_number);

            fprintf(f, "# gravitational acceleration for self-weight loading (global)\n");
            fprintf(f, "# gX		gY		gZ\n");
            fprintf(f, "# in/s^2\t\tin/s^2\t\tin/s^2\n");
            fprintf(f, "# mm/s^2\t\tmm/s^2\t\tmm/s^2\n\n");
            fprintf(f, "%f %f %f\n\n", gX, -gY * gammas->gamma_g * unit_factors->g_f, gZ);

            stl_node_force_moment_no = 0;
            for (i = 0; i < st_node_force_moment_no; i++) {
                if (TestBit(st_layer, st_node_force_moment[i].layer)) {
                    if (st_node_force_moment[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_node_force_moment[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_node_force_moment[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_node_force_moment[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_node_force_moment[i].factor_record)].load];
                    }
                    if (flag & ULSLC[ci].mask) {
                        st_node_force_moment[i].take_it = 1;
                        stl_node_force_moment_no++;
                    } else st_node_force_moment[i].take_it = 0;
                }
            }

            //nodes forces and moments
            fprintf(f, "%d\t\t# number of loaded nodes (global)\n", stl_node_force_moment_no);
            fprintf(f, "#.n     Fx      Fy     Fz     Mxx     Myy     Mzz\n");
            fprintf(f, "#.      kip     kip    kip    kip-in  kip-in  kip-in\n");
            fprintf(f, "#.       N       N      N     Nmm     Nmm     Nmm\n\n");
            for (i = 0; i < st_node_force_moment_no; i++) {
                if ((TestBit(st_layer, st_node_force_moment[i].layer)) && (st_node_force_moment[i].take_it)) {
                    if (st_node_force_moment[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_node_force_moment[i].factor_record];
                        load = st_load_factors[st_node_force_moment[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_node_force_moment[i].factor_record)];
                        load = load_factors[abs(st_node_force_moment[i].factor_record)].load;
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

                    set_decimal_format(par[0], st_node_force_moment[i].fx * gamma_l * unit_factors->F_f,
                                       force_precision);
                    set_decimal_format(par[1], st_node_force_moment[i].fy * gamma_l * unit_factors->F_f,
                                       force_precision);
                    set_decimal_format(par[2], 0.0, force_precision);
                    set_decimal_format(par[3], 0.0, force_precision);
                    set_decimal_format(par[4], 0.0, force_precision);
                    set_decimal_format(par[5], st_node_force_moment[i].mz * gamma_l * unit_factors->M_f,
                                       moment_precision);
                    fprintf(f, "%d %s %s %s %s %s %s\n", st_node_force_moment[i].node + 1, par[0], par[1], par[2],
                            par[3], par[4], par[5]);
                }
            }

            //elements uniform load
            stl_uniform_load_no = 0;
            for (i = 0; i < st_uniform_load_no; i++) {
                if (TestBit(st_layer, st_uniform_load[i].layer)) //stl_uniform_load_no++;
                {
                    if (st_uniform_load[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_uniform_load[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_uniform_load[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_uniform_load[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_uniform_load[i].factor_record)].load];
                    }
                    if (flag & ULSLC[ci].mask) {
                        st_uniform_load[i].take_it = 1;
                        stl_uniform_load_no++;
                    } else st_uniform_load[i].take_it = 0;
                }
            }

            memmove(st_uniform_load_comb, st_uniform_load, sizeof(ST_UNIFORM_LOAD) * st_uniform_load_no);

            for (i = 0; i < st_uniform_load_no; i++) {
                if ((TestBit(st_layer, st_uniform_load[i].layer)) && (st_uniform_load[i].take_it)) {
                    if (st_uniform_load[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_uniform_load[i].factor_record];
                        load = st_load_factors[st_uniform_load[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_uniform_load[i].factor_record)];
                        load = load_factors[abs(st_uniform_load[i].factor_record)].load;
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

                    st_uniform_load_comb[i].qx = st_uniform_load[i].qx * gamma_l;
                    st_uniform_load_comb[i].qy = st_uniform_load[i].qy * gamma_l;

                }
            }

            //////////////

            st_uniform_load_no_cons = 0;
            consolidate_loads(st_uniform_load_comb, st_uniform_load_no, st_uniform_load_cons, &st_uniform_load_no_cons,
                              st_layer);

            fprintf(f, "\n%d\t\t# number of uniformly-distributed element loads (local)\n", st_uniform_load_no_cons);
            fprintf(f, "#.elmnt  X-load   Y-load   Z-load   uniform member loads in member coordinates\n");
            fprintf(f, "# .      kip/in   kip/in   kip/in\n");
            fprintf(f, "# .      N/mm     N/mm     N/mm\n\n");

            for (i = 0; i < st_uniform_load_no_cons; i++) {
                if (TestBit(st_layer, st_uniform_load_cons[i].layer)) {
                    set_decimal_format(par[0], st_uniform_load_cons[i].qx * unit_factors->q_f, load_precision);
                    set_decimal_format(par[1], st_uniform_load_cons[i].qy * unit_factors->q_f, load_precision);
                    set_decimal_format(par[2], 0.0, load_precision);

                    fprintf(f, "%d %s %s %s\n", st_uniform_load_cons[i].element + 1, par[0], par[1], par[2]);
                }
            }
            ///////////////////

            //elements trapezoid load
            stl_trapezoid_load_no = 0;
            for (i = 0; i < st_trapezoid_load_no; i++) {
                if (TestBit(st_layer, st_trapezoid_load[i].layer))
                {
                    if (st_trapezoid_load[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_trapezoid_load[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_trapezoid_load[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_trapezoid_load[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_trapezoid_load[i].factor_record)].load];
                    }
                    if (flag & ULSLC[ci].mask) {
                        st_trapezoid_load[i].take_it = 1;
                        stl_trapezoid_load_no++;
                    } else st_trapezoid_load[i].take_it = 0;
                }
            }
            fprintf(f, "\n%d\t\t# number of trapezoidal loads\n", stl_trapezoid_load_no);
            fprintf(f, "#       start    stop     start    stop\n");
            fprintf(f, "#.elmnt loc'n    loc'n    load     load\n");
            fprintf(f, "# .      in       in      kip/in   kip/in\n");
            fprintf(f, "# .      mm       mm      N/mm     N/mm\n\n");

            for (i = 0; i < st_trapezoid_load_no; i++) {
                if ((TestBit(st_layer, st_trapezoid_load[i].layer)) && (st_trapezoid_load[i].take_it)) {
                    if (st_trapezoid_load[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_trapezoid_load[i].factor_record];
                        load = st_load_factors[st_trapezoid_load[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_trapezoid_load[i].factor_record)];
                        load = load_factors[abs(st_trapezoid_load[i].factor_record)].load;
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

                    set_decimal_format(par[0], milimetryob(st_trapezoid_load[i].dlxx1 * units_factor), dim_precision);
                    set_decimal_format(par[1], milimetryob(st_trapezoid_load[i].dlxx2 * units_factor), dim_precision);
                    set_decimal_format(par[2], st_trapezoid_load[i].qx1 * gamma_l * unit_factors->q_f, load_precision);
                    set_decimal_format(par[3], st_trapezoid_load[i].qx2 * gamma_l * unit_factors->q_f, load_precision);

                    set_decimal_format(par[4], milimetryob(st_trapezoid_load[i].dlxy1 * units_factor), dim_precision);
                    set_decimal_format(par[5], milimetryob(st_trapezoid_load[i].dlxy2 * units_factor), dim_precision);
                    set_decimal_format(par[6], st_trapezoid_load[i].qy1 * gamma_l * unit_factors->q_f, load_precision);
                    set_decimal_format(par[7], st_trapezoid_load[i].qy2 * gamma_l * unit_factors->q_f, load_precision);

                    set_decimal_format(par[8], 0.0, dim_precision);
                    set_decimal_format(par[9], 0.0, dim_precision);
                    set_decimal_format(par[10], 0.0, load_precision);
                    set_decimal_format(par[11], 0.0, load_precision);

                    fprintf(f, "\t\t%d %s %s %s %s %s %s %s %s %s %s %s %s\n", st_trapezoid_load[i].element + 1, par[0],
                            par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9], par[10], par[11]);
                }
            }

            //internal concentrated loads
            stl_element_force_no = 0;
            for (i = 0; i < st_element_force_no; i++) {
                if (TestBit(st_layer, st_element_force[i].layer))
                {
                    if (st_element_force[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_element_force[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_element_force[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_element_force[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_element_force[i].factor_record)].load];
                    }
                    if (flag & ULSLC[ci].mask) {
                        st_element_force[i].take_it = 1;
                        stl_element_force_no++;
                    } else st_element_force[i].take_it = 0;
                }
            }
            fprintf(f, "\n%d\t\t# number of concentrated interior point loads (local)\n", stl_element_force_no);
            fprintf(f, "#.elmnt  X-load   Y-load   Z-load    x-loc'n  point loads in member coordinates\n");
            fprintf(f, "#.       kip      kip       kip     in\n");
            fprintf(f, "#.       N        N         N       mm\n\n");

            for (i = 0; i < st_element_force_no; i++) {
                if ((TestBit(st_layer, st_element_force[i].layer)) && (st_element_force[i].take_it)) {
                    if (st_element_force[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_element_force[i].factor_record];
                        load = st_load_factors[st_element_force[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_element_force[i].factor_record)];
                        load = load_factors[abs(st_element_force[i].factor_record)].load;
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

                    set_decimal_format(par[0], st_element_force[i].fx * gamma_l * unit_factors->F_f, force_precision);
                    set_decimal_format(par[1], st_element_force[i].fy * gamma_l * unit_factors->F_f, force_precision);
                    set_decimal_format(par[2], 0.0, force_precision);
                    set_decimal_format(par[3], milimetryob(st_element_force[i].dlx * units_factor), dim_precision);

                    fprintf(f, "%d %s %s %s %s\n", st_element_force[i].element + 1, par[0], par[1], par[2], par[3]);
                }
            }

            //tenperature loads
            stl_thermal_load_no = 0;
            for (i = 0; i < st_thermal_load_no; i++) {
                if (TestBit(st_layer, st_thermal_load[i].layer))
                {
                    if (st_thermal_load[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_thermal_load[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_thermal_load[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_thermal_load[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_thermal_load[i].factor_record)].load];
                    }
                    if (flag & ULSLC[ci].mask) {
                        st_thermal_load[i].take_it = 1;
                        stl_thermal_load_no++;
                    } else st_thermal_load[i].take_it = 0;
                }
            }
            fprintf(f, "\n%d\t\t#  number of frame elements with temperature changes (local)\n", stl_thermal_load_no);
            fprintf(f, "#.elmnt   coef.  y-depth  z-depth  deltaTy+  deltaTy-  deltaTz+  deltaTz-\n");
            fprintf(f, "#.         /deg.F    in       in     deg.F     deg.F     deg.F     deg.F\n");
            fprintf(f, "#.         /deg.C    mm       mm     deg.C     deg.C     deg.C     deg.C\n\n");

            for (i = 0; i < st_thermal_load_no; i++) {
                if ((TestBit(st_layer, st_thermal_load[i].layer)) && (st_thermal_load[i].take_it)) {
                    if (st_thermal_load[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_thermal_load[i].factor_record];
                        load = st_load_factors[st_thermal_load[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_thermal_load[i].factor_record)];
                        load = load_factors[abs(st_thermal_load[i].factor_record)].load;
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
                    if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                    if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                    if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                    if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                    if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                    if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                    for (j = 0; j < st_property_no; j++) {
                        if (abs(st_element[st_thermal_load[i].element].property_no) == st_property[j].n)
                            break;
                    }

                    if (j == st_property_no) {
                        sprintf(report_row, "#%d %s\n", st_element[st_thermal_load[i].element].property_no,
                                _property_not_defined_);
                        strcat(report, report_row);
                        break;
                    }

                    sprintf(par[0], "%e", st_property[j].a);
                    set_decimal_format(par[1], st_thermal_load[i].ydepth, prop_precisions->dim_precision);
                    set_decimal_format(par[2], st_thermal_load[i].ydepth, prop_precisions->dim_precision);  //same as y axis
                    set_decimal_format(par[3], st_thermal_load[i].deltatplus * gamma_l, t_precision);
                    set_decimal_format(par[4], st_thermal_load[i].deltatminus * gamma_l, t_precision);
                    set_decimal_format(par[5], 0.0, t_precision);
                    set_decimal_format(par[6], 0.0, t_precision);

                    fprintf(f, "%d %s %s %s %s %s %s %s\n", st_thermal_load[i].element + 1, par[0], par[1], par[2],
                            par[3], par[4], par[5], par[6]);
                }
            }

            //prescribed displacements
            stl_displacement_no = 0;
            for (i = 0; i < st_displacement_no; i++) {
                if (TestBit(st_layer, st_displacement[i].layer)) stl_displacement_no++;
            }
            fprintf(f, "\n%d\t\t# number of nodes with prescribed displacements  nD<=nR (global)\n",
                    stl_displacement_no);
            fprintf(f, "#.node   X-displ  Y-displ  Z-displ  X-rot'n   Y-rot'n   Z-rot'n\n");
            fprintf(f, "#.         in       in       in      radian    radian    radian\n");
            fprintf(f, "#.         mm       mm       mm      radian    radian    radian\n");

            for (i = 0; i < st_displacement_no; i++) {
                if (TestBit(st_layer, st_displacement[i].layer)) {
                    //no partial factors (assumed)

                    set_decimal_format(par[0], st_displacement[i].dx, dim_precision);
                    set_decimal_format(par[1], st_displacement[i].dy, dim_precision);
                    set_decimal_format(par[2], 0.0, dim_precision);
                    set_decimal_format(par[3], 0.0, dim_precision);
                    set_decimal_format(par[4], 0.0, dim_precision);
                    set_decimal_format(par[5], st_displacement[i].dzz, dim_precision);

                    fprintf(f, "%d %s %s %s %s %s %s\n", st_displacement[i].node + 1, par[0], par[1], par[2], par[3],
                            par[4], par[5]);
                }
            }

            fprintf(f, "\n\t\t# End Static Load Case ULS %d Combination %d (%d)\n\n", combination_no, ci + 1,
                    case_number);

            //////////////////////////////
        }
    }
    //// ULS COMBINATIONS ENDS
    //// SLS COMBINATIONS BEGIN
    for (int ci = 0; ci < SLSLC_NO; ci++) {
        if (SLSLC[ci].flag == 1) {
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

            case_number++;
            fprintf(f, "\t\t# Begin Static Load Case SLS %d Combination %d (%d)\n\n", combination_no, ci + 1,
                    case_number);

            fprintf(f, "# gravitational acceleration for self-weight loading (global)\n");
            fprintf(f, "# gX		gY		gZ\n");
            fprintf(f, "# in/s^2\t\tin/s^2\t\tin/s^2\n");
            fprintf(f, "# mm/s^2\t\tmm/s^2\t\tmm/s^2\n\n");
            fprintf(f, "%f %f %f\n\n", gX, -gY * gammas->gamma_g * unit_factors->g_f, gZ);

            stl_node_force_moment_no = 0;
            for (i = 0; i < st_node_force_moment_no; i++) {
                if (TestBit(st_layer, st_node_force_moment[i].layer)) {
                    if (st_node_force_moment[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_node_force_moment[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_node_force_moment[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_node_force_moment[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_node_force_moment[i].factor_record)].load];
                    }
                    if (flag & SLSLC[ci].mask) {
                        st_node_force_moment[i].take_it = 1;
                        stl_node_force_moment_no++;
                    } else st_node_force_moment[i].take_it = 0;
                }
            }

            //nodes forces and moments
            fprintf(f, "%d\t\t# number of loaded nodes (global)\n", stl_node_force_moment_no);
            fprintf(f, "#.n     Fx      Fy     Fz     Mxx     Myy     Mzz\n");
            fprintf(f, "#.      kip     kip    kip    kip-in  kip-in  kip-in\n");
            fprintf(f, "#.       N       N      N     Nmm     Nmm     Nmm\n\n");
            for (i = 0; i < st_node_force_moment_no; i++) {
                if ((TestBit(st_layer, st_node_force_moment[i].layer)) && (st_node_force_moment[i].take_it)) {

                    if (st_node_force_moment[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_node_force_moment[i].factor_record];
                        load = st_load_factors[st_node_force_moment[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_node_force_moment[i].factor_record)];
                        load = load_factors[abs(st_node_force_moment[i].factor_record)].load;
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

                    set_decimal_format(par[0], st_node_force_moment[i].fx * gamma_l * unit_factors->F_f,
                                       force_precision);
                    set_decimal_format(par[1], st_node_force_moment[i].fy * gamma_l * unit_factors->F_f,
                                       force_precision);
                    set_decimal_format(par[2], 0.0, force_precision);
                    set_decimal_format(par[3], 0.0, force_precision);
                    set_decimal_format(par[4], 0.0, force_precision);
                    set_decimal_format(par[5], st_node_force_moment[i].mz * gamma_l * unit_factors->M_f,
                                       moment_precision);
                    fprintf(f, "%d %s %s %s %s %s %s\n", st_node_force_moment[i].node + 1, par[0], par[1], par[2],
                            par[3], par[4], par[5]);
                }
            }

            //elements uniform load
            stl_uniform_load_no = 0;
            for (i = 0; i < st_uniform_load_no; i++) {
                if (TestBit(st_layer, st_uniform_load[i].layer))
                {
                    if (st_uniform_load[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_uniform_load[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_uniform_load[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_uniform_load[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_uniform_load[i].factor_record)].load];
                    }
                    if (flag & SLSLC[ci].mask) {
                        st_uniform_load[i].take_it = 1;
                        stl_uniform_load_no++;
                    } else st_uniform_load[i].take_it = 0;
                }
            }

            memmove(st_uniform_load_comb, st_uniform_load, sizeof(ST_UNIFORM_LOAD) * st_uniform_load_no);

            for (i = 0; i < st_uniform_load_no; i++) {
                if ((TestBit(st_layer, st_uniform_load[i].layer)) && (st_uniform_load[i].take_it)) {
                    if (st_uniform_load[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_uniform_load[i].factor_record];
                        load = st_load_factors[st_uniform_load[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_uniform_load[i].factor_record)];
                        load = load_factors[abs(st_uniform_load[i].factor_record)].load;
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


                    st_uniform_load_comb[i].qx = st_uniform_load[i].qx * gamma_l;
                    st_uniform_load_comb[i].qy = st_uniform_load[i].qy * gamma_l;

                }
            }

            //////////////

            st_uniform_load_no_cons = 0;
            consolidate_loads(st_uniform_load_comb, st_uniform_load_no, st_uniform_load_cons, &st_uniform_load_no_cons,
                              st_layer);

            fprintf(f, "\n%d\t\t# number of uniformly-distributed element loads (local)\n", st_uniform_load_no_cons);
            fprintf(f, "#.elmnt  X-load   Y-load   Z-load   uniform member loads in member coordinates\n");
            fprintf(f, "# .      kip/in   kip/in   kip/in\n");
            fprintf(f, "# .      N/mm     N/mm     N/mm\n\n");

            for (i = 0; i < st_uniform_load_no_cons; i++) {
                if (TestBit(st_layer, st_uniform_load_cons[i].layer)) {
                    set_decimal_format(par[0], st_uniform_load_cons[i].qx * unit_factors->q_f, load_precision);
                    set_decimal_format(par[1], st_uniform_load_cons[i].qy * unit_factors->q_f, load_precision);
                    set_decimal_format(par[2], 0.0, load_precision);

                    fprintf(f, "%d %s %s %s\n", st_uniform_load_cons[i].element + 1, par[0], par[1], par[2]);
                }
            }

            //elements trapezoid load
            stl_trapezoid_load_no = 0;
            for (i = 0; i < st_trapezoid_load_no; i++) {
                if (TestBit(st_layer, st_trapezoid_load[i].layer))
                {
                    if (st_trapezoid_load[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_trapezoid_load[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_trapezoid_load[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_trapezoid_load[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_trapezoid_load[i].factor_record)].load];
                    }
                    if (flag & SLSLC[ci].mask) {
                        st_trapezoid_load[i].take_it = 1;
                        stl_trapezoid_load_no++;
                    } else st_trapezoid_load[i].take_it = 0;
                }
            }
            fprintf(f, "\n%d\t\t# number of trapezoidal loads\n", stl_trapezoid_load_no);
            fprintf(f, "#       start    stop     start    stop\n");
            fprintf(f, "#.elmnt loc'n    loc'n    load     load\n");
            fprintf(f, "# .      in       in      kip/in   kip/in\n");
            fprintf(f, "# .      mm       mm      N/mm     N/mm\n\n");

            for (i = 0; i < st_trapezoid_load_no; i++) {
                if ((TestBit(st_layer, st_trapezoid_load[i].layer)) && (st_trapezoid_load[i].take_it)) {
                    if (st_trapezoid_load[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_trapezoid_load[i].factor_record];
                        load = st_load_factors[st_trapezoid_load[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_trapezoid_load[i].factor_record)];
                        load = load_factors[abs(st_trapezoid_load[i].factor_record)].load;
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

                    set_decimal_format(par[0], milimetryob(st_trapezoid_load[i].dlxx1 * units_factor), dim_precision);
                    set_decimal_format(par[1], milimetryob(st_trapezoid_load[i].dlxx2 * units_factor), dim_precision);
                    set_decimal_format(par[2], st_trapezoid_load[i].qx1 * gamma_l * unit_factors->q_f, load_precision);
                    set_decimal_format(par[3], st_trapezoid_load[i].qx2 * gamma_l * unit_factors->q_f, load_precision);

                    set_decimal_format(par[4], milimetryob(st_trapezoid_load[i].dlxy1 * units_factor), dim_precision);
                    set_decimal_format(par[5], milimetryob(st_trapezoid_load[i].dlxy2 * units_factor), dim_precision);
                    set_decimal_format(par[6], st_trapezoid_load[i].qy1 * gamma_l * unit_factors->q_f, load_precision);
                    set_decimal_format(par[7], st_trapezoid_load[i].qy2 * gamma_l * unit_factors->q_f, load_precision);

                    set_decimal_format(par[8], 0.0, dim_precision);
                    set_decimal_format(par[9], 0.0, dim_precision);
                    set_decimal_format(par[10], 0.0, load_precision);
                    set_decimal_format(par[11], 0.0, load_precision);

                    fprintf(f, "\t\t%d %s %s %s %s %s %s %s %s %s %s %s %s\n", st_trapezoid_load[i].element + 1, par[0],
                            par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9], par[10], par[11]);
                }
            }

            //internal concentrated loads
            stl_element_force_no = 0;
            for (i = 0; i < st_element_force_no; i++) {
                if (TestBit(st_layer, st_element_force[i].layer))
                {
                    if (st_element_force[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_element_force[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_element_force[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_element_force[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_element_force[i].factor_record)].load];
                    }
                    if (flag & SLSLC[ci].mask) {
                        st_element_force[i].take_it = 1;
                        stl_element_force_no++;
                    } else st_element_force[i].take_it = 0;
                }
            }
            fprintf(f, "\n%d\t\t# number of concentrated interior point loads (local)\n", stl_element_force_no);
            fprintf(f, "#.elmnt  X-load   Y-load   Z-load    x-loc'n  point loads in member coordinates\n");
            fprintf(f, "#.       kip      kip       kip     in\n");
            fprintf(f, "#.       N        N         N       mm\n\n");

            for (i = 0; i < st_element_force_no; i++) {
                if ((TestBit(st_layer, st_element_force[i].layer)) && (st_element_force[i].take_it)) {
                    if (st_element_force[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_element_force[i].factor_record];
                        load = st_load_factors[st_element_force[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_element_force[i].factor_record)];
                        load = load_factors[abs(st_element_force[i].factor_record)].load;
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

                    set_decimal_format(par[0], st_element_force[i].fx * gamma_l * unit_factors->F_f, force_precision);
                    set_decimal_format(par[1], st_element_force[i].fy * gamma_l * unit_factors->F_f, force_precision);
                    set_decimal_format(par[2], 0.0, force_precision);
                    set_decimal_format(par[3], milimetryob(st_element_force[i].dlx * units_factor), dim_precision);

                    fprintf(f, "%d %s %s %s %s\n", st_element_force[i].element + 1, par[0], par[1], par[2], par[3]);
                }
            }

            //tenmperature loads
            stl_thermal_load_no = 0;
            for (i = 0; i < st_thermal_load_no; i++) {
                if (TestBit(st_layer, st_thermal_load[i].layer))
                {
                    if (st_thermal_load[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_thermal_load[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_thermal_load[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_thermal_load[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_thermal_load[i].factor_record)].load];
                    }
                    if (flag & SLSLC[ci].mask) {
                        st_thermal_load[i].take_it = 1;
                        stl_thermal_load_no++;
                    } else st_thermal_load[i].take_it = 0;
                }
            }
            fprintf(f, "\n%d\t\t#  number of frame elements with temperature changes (local)\n", stl_thermal_load_no);
            fprintf(f, "#.elmnt   coef.  y-depth  z-depth  deltaTy+  deltaTy-  deltaTz+  deltaTz-\n");
            fprintf(f, "#.         /deg.F    in       in     deg.F     deg.F     deg.F     deg.F\n");
            fprintf(f, "#.         /deg.C    mm       mm     deg.C     deg.C     deg.C     deg.C\n\n");

            for (i = 0; i < st_thermal_load_no; i++) {
                if ((TestBit(st_layer, st_thermal_load[i].layer)) && (st_thermal_load[i].take_it)) {
                    if (st_thermal_load[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_thermal_load[i].factor_record];
                        load = st_load_factors[st_thermal_load[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_thermal_load[i].factor_record)];
                        load = load_factors[abs(st_thermal_load[i].factor_record)].load;
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
                    if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                    if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                    if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                    if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                    if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                    if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                    for (j = 0; j < st_property_no; j++) {
                        if (abs(st_element[st_thermal_load[i].element].property_no) == st_property[j].n)
                            break;
                    }

                    if (j == st_property_no) {
                        sprintf(report_row, "#%d %s\n", st_element[st_thermal_load[i].element].property_no,
                                _property_not_defined_);
                        strcat(report, report_row);
                        break;
                    }

                    sprintf(par[0], "%e", st_property[j].a);
                    set_decimal_format(par[1], st_thermal_load[i].ydepth, prop_precisions->dim_precision);
                    set_decimal_format(par[2], st_thermal_load[i].ydepth, prop_precisions->dim_precision);  //same as y axis
                    set_decimal_format(par[3], st_thermal_load[i].deltatplus * gamma_l, t_precision);
                    set_decimal_format(par[4], st_thermal_load[i].deltatminus * gamma_l, t_precision);
                    set_decimal_format(par[5], 0.0, t_precision);
                    set_decimal_format(par[6], 0.0, t_precision);

                    fprintf(f, "%d %s %s %s %s %s %s %s\n", st_thermal_load[i].element + 1, par[0], par[1], par[2],
                            par[3], par[4], par[5], par[6]);
                }
            }

            //prescribed displacements
            stl_displacement_no = 0;
            for (i = 0; i < st_displacement_no; i++) {
                if (TestBit(st_layer, st_displacement[i].layer)) stl_displacement_no++;
            }
            fprintf(f, "\n%d\t\t# number of nodes with prescribed displacements  nD<=nR (global)\n",
                    stl_displacement_no);
            fprintf(f, "#.node   X-displ  Y-displ  Z-displ  X-rot'n   Y-rot'n   Z-rot'n\n");
            fprintf(f, "#.         in       in       in      radian    radian    radian\n");
            fprintf(f, "#.         mm       mm       mm      radian    radian    radian\n");

            for (i = 0; i < st_displacement_no; i++) {
                if (TestBit(st_layer, st_displacement[i].layer)) {
                    //no partial factors (assumed)

                    set_decimal_format(par[0], st_displacement[i].dx, dim_precision);
                    set_decimal_format(par[1], st_displacement[i].dy, dim_precision);
                    set_decimal_format(par[2], 0.0, dim_precision);
                    set_decimal_format(par[3], 0.0, dim_precision);
                    set_decimal_format(par[4], 0.0, dim_precision);
                    set_decimal_format(par[5], st_displacement[i].dzz, dim_precision);

                    fprintf(f, "%d %s %s %s %s %s %s\n", st_displacement[i].node + 1, par[0], par[1], par[2], par[3],
                            par[4], par[5]);
                }
            }

            //fprintf(f,"\n\t\t# End Static Load Case %d of %d\n\n",li+1, st_layer_no);
            fprintf(f, "\n\t\t# End Static Load Case SLS %d Combination %d (%d)\n\n", combination_no, ci + 1,
                    case_number);

            //////////////////////////////
        }
    }

    //// SLS COMBINATIONS ENDS
    //// QPSLS COMBINATIONS BEGIN
    for (int ci = 0; ci < QPSLSLC_NO; ci++) {
        if (QPSLSLC[ci].flag == 1) {
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

            case_number++;
            fprintf(f, "\t\t# Begin Static Load Case QPSLS %d Combination %d (%d)\n\n", combination_no, ci + 1,
                    case_number);

            fprintf(f, "# gravitational acceleration for self-weight loading (global)\n");
            fprintf(f, "# gX		gY		gZ\n");
            fprintf(f, "# in/s^2\t\tin/s^2\t\tin/s^2\n");
            fprintf(f, "# mm/s^2\t\tmm/s^2\t\tmm/s^2\n\n");
            fprintf(f, "%f %f %f\n\n", gX, -gY * gammas->gamma_g * unit_factors->g_f, gZ);

            stl_node_force_moment_no = 0;
            for (i = 0; i < st_node_force_moment_no; i++) {
                if (TestBit(st_layer, st_node_force_moment[i].layer)) {
                    if (st_node_force_moment[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_node_force_moment[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_node_force_moment[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_node_force_moment[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_node_force_moment[i].factor_record)].load];
                    }
                    //if ((flag & QPSLSLC[ci].mask) == flag) {
                    if (flag & QPSLSLC[ci].mask) {
                        st_node_force_moment[i].take_it = 1;
                        stl_node_force_moment_no++;
                    } else st_node_force_moment[i].take_it = 0;
                }
            }

            //nodes forces and moments
            fprintf(f, "%d\t\t# number of loaded nodes (global)\n", stl_node_force_moment_no);
            fprintf(f, "#.n     Fx      Fy     Fz     Mxx     Myy     Mzz\n");
            fprintf(f, "#.      kip     kip    kip    kip-in  kip-in  kip-in\n");
            fprintf(f, "#.       N       N      N     Nmm     Nmm     Nmm\n\n");
            for (i = 0; i < st_node_force_moment_no; i++) {
                if ((TestBit(st_layer, st_node_force_moment[i].layer)) && (st_node_force_moment[i].take_it)) {

                    if (st_node_force_moment[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_node_force_moment[i].factor_record];
                        load = st_load_factors[st_node_force_moment[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_node_force_moment[i].factor_record)];
                        load = load_factors[abs(st_node_force_moment[i].factor_record)].load;
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

                    set_decimal_format(par[0], st_node_force_moment[i].fx * gamma_l * unit_factors->F_f,
                                       force_precision);
                    set_decimal_format(par[1], st_node_force_moment[i].fy * gamma_l * unit_factors->F_f,
                                       force_precision);
                    set_decimal_format(par[2], 0.0, force_precision);
                    set_decimal_format(par[3], 0.0, force_precision);
                    set_decimal_format(par[4], 0.0, force_precision);
                    set_decimal_format(par[5], st_node_force_moment[i].mz * gamma_l * unit_factors->M_f,
                                       moment_precision);
                    fprintf(f, "%d %s %s %s %s %s %s\n", st_node_force_moment[i].node + 1, par[0], par[1], par[2],
                            par[3], par[4], par[5]);
                }
            }

            //elements uniform load
            stl_uniform_load_no = 0;
            for (i = 0; i < st_uniform_load_no; i++) {
                if (TestBit(st_layer, st_uniform_load[i].layer))
                {
                    if (st_uniform_load[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_uniform_load[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_uniform_load[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_uniform_load[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_uniform_load[i].factor_record)].load];
                    }
                    if (flag & QPSLSLC[ci].mask) {
                        st_uniform_load[i].take_it = 1;
                        stl_uniform_load_no++;
                    } else st_uniform_load[i].take_it = 0;
                }
            }

            memmove(st_uniform_load_comb, st_uniform_load, sizeof(ST_UNIFORM_LOAD) * st_uniform_load_no);

            for (i = 0; i < st_uniform_load_no; i++) {
                if ((TestBit(st_layer, st_uniform_load[i].layer)) && (st_uniform_load[i].take_it)) {
                    if (st_uniform_load[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_uniform_load[i].factor_record];
                        load = st_load_factors[st_uniform_load[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_uniform_load[i].factor_record)];
                        load = load_factors[abs(st_uniform_load[i].factor_record)].load;
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

                    st_uniform_load_comb[i].qx = st_uniform_load[i].qx * gamma_l;
                    st_uniform_load_comb[i].qy = st_uniform_load[i].qy * gamma_l;

                }
            }

            st_uniform_load_no_cons = 0;
            consolidate_loads(st_uniform_load_comb, st_uniform_load_no, st_uniform_load_cons, &st_uniform_load_no_cons,
                              st_layer);

            fprintf(f, "\n%d\t\t# number of uniformly-distributed element loads (local)\n", st_uniform_load_no_cons);
            fprintf(f, "#.elmnt  X-load   Y-load   Z-load   uniform member loads in member coordinates\n");
            fprintf(f, "# .      kip/in   kip/in   kip/in\n");
            fprintf(f, "# .      N/mm     N/mm     N/mm\n\n");

            for (i = 0; i < st_uniform_load_no_cons; i++) {
                if (TestBit(st_layer, st_uniform_load_cons[i].layer)) {
                    set_decimal_format(par[0], st_uniform_load_cons[i].qx * unit_factors->q_f, load_precision);
                    set_decimal_format(par[1], st_uniform_load_cons[i].qy * unit_factors->q_f, load_precision);
                    set_decimal_format(par[2], 0.0, load_precision);

                    fprintf(f, "%d %s %s %s\n", st_uniform_load_cons[i].element + 1, par[0], par[1], par[2]);
                }
            }

            //elements trapezoid load
            stl_trapezoid_load_no = 0;
            for (i = 0; i < st_trapezoid_load_no; i++) {
                if (TestBit(st_layer, st_trapezoid_load[i].layer))
                {
                    if (st_trapezoid_load[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_trapezoid_load[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_trapezoid_load[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_trapezoid_load[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_trapezoid_load[i].factor_record)].load];
                    }
                    if (flag & QPSLSLC[ci].mask) {
                        st_trapezoid_load[i].take_it = 1;
                        stl_trapezoid_load_no++;
                    } else st_trapezoid_load[i].take_it = 0;
                }
            }
            fprintf(f, "\n%d\t\t# number of trapezoidal loads\n", stl_trapezoid_load_no);
            fprintf(f, "#       start    stop     start    stop\n");
            fprintf(f, "#.elmnt loc'n    loc'n    load     load\n");
            fprintf(f, "# .      in       in      kip/in   kip/in\n");
            fprintf(f, "# .      mm       mm      N/mm     N/mm\n\n");

            for (i = 0; i < st_trapezoid_load_no; i++) {
                if ((TestBit(st_layer, st_trapezoid_load[i].layer)) && (st_trapezoid_load[i].take_it)) {
                    if (st_trapezoid_load[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_trapezoid_load[i].factor_record];
                        load = st_load_factors[st_trapezoid_load[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_trapezoid_load[i].factor_record)];
                        load = load_factors[abs(st_trapezoid_load[i].factor_record)].load;
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

                    set_decimal_format(par[0], milimetryob(st_trapezoid_load[i].dlxx1 * units_factor), dim_precision);
                    set_decimal_format(par[1], milimetryob(st_trapezoid_load[i].dlxx2 * units_factor), dim_precision);
                    set_decimal_format(par[2], st_trapezoid_load[i].qx1 * gamma_l * unit_factors->q_f, load_precision);
                    set_decimal_format(par[3], st_trapezoid_load[i].qx2 * gamma_l * unit_factors->q_f, load_precision);

                    set_decimal_format(par[4], milimetryob(st_trapezoid_load[i].dlxy1 * units_factor), dim_precision);
                    set_decimal_format(par[5], milimetryob(st_trapezoid_load[i].dlxy2 * units_factor), dim_precision);
                    set_decimal_format(par[6], st_trapezoid_load[i].qy1 * gamma_l * unit_factors->q_f, load_precision);
                    set_decimal_format(par[7], st_trapezoid_load[i].qy2 * gamma_l * unit_factors->q_f, load_precision);

                    set_decimal_format(par[8], 0.0, dim_precision);
                    set_decimal_format(par[9], 0.0, dim_precision);
                    set_decimal_format(par[10], 0.0, load_precision);
                    set_decimal_format(par[11], 0.0, load_precision);

                    fprintf(f, "\t\t%d %s %s %s %s %s %s %s %s %s %s %s %s\n", st_trapezoid_load[i].element + 1, par[0],
                            par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9], par[10], par[11]);
                }
            }

            //internal concentrated loads
            stl_element_force_no = 0;
            for (i = 0; i < st_element_force_no; i++) {
                if (TestBit(st_layer, st_element_force[i].layer))
                {
                    if (st_element_force[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_element_force[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_element_force[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_element_force[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_element_force[i].factor_record)].load];
                    }
                    if (flag & QPSLSLC[ci].mask) {
                        st_element_force[i].take_it = 1;
                        stl_element_force_no++;
                    } else st_element_force[i].take_it = 0;
                }
            }
            fprintf(f, "\n%d\t\t# number of concentrated interior point loads (local)\n", stl_element_force_no);
            fprintf(f, "#.elmnt  X-load   Y-load   Z-load    x-loc'n  point loads in member coordinates\n");
            fprintf(f, "#.       kip      kip       kip     in\n");
            fprintf(f, "#.       N        N         N       mm\n\n");

            for (i = 0; i < st_element_force_no; i++) {
                if ((TestBit(st_layer, st_element_force[i].layer)) && (st_element_force[i].take_it)) {
                    if (st_element_force[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_element_force[i].factor_record];
                        load = st_load_factors[st_element_force[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_element_force[i].factor_record)];
                        load = load_factors[abs(st_element_force[i].factor_record)].load;
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

                    set_decimal_format(par[0], st_element_force[i].fx * gamma_l * unit_factors->F_f, force_precision);
                    set_decimal_format(par[1], st_element_force[i].fy * gamma_l * unit_factors->F_f, force_precision);
                    set_decimal_format(par[2], 0.0, force_precision);
                    set_decimal_format(par[3], milimetryob(st_element_force[i].dlx * units_factor), dim_precision);

                    fprintf(f, "%d %s %s %s %s\n", st_element_force[i].element + 1, par[0], par[1], par[2], par[3]);
                }
            }

            //temperature loads
            stl_thermal_load_no = 0;
            for (i = 0; i < st_thermal_load_no; i++) {
                if (TestBit(st_layer, st_thermal_load[i].layer))
                {
                    if (st_thermal_load[i].factor_record >= 0) {
                        gamma_l = st_load_factors[st_thermal_load[i].factor_record].gamma;
                        flag = load_flag[st_load_factors[st_thermal_load[i].factor_record].load];
                    } else {
                        gamma_l = load_factors[abs(st_thermal_load[i].factor_record)].gamma;
                        flag = load_flag[load_factors[abs(st_thermal_load[i].factor_record)].load];
                    }
                    if (flag & QPSLSLC[ci].mask) {
                        st_thermal_load[i].take_it = 1;
                        stl_thermal_load_no++;
                    } else st_thermal_load[i].take_it = 0;
                }
            }
            fprintf(f, "\n%d\t\t#  number of frame elements with temperature changes (local)\n", stl_thermal_load_no);
            fprintf(f, "#.elmnt   coef.  y-depth  z-depth  deltaTy+  deltaTy-  deltaTz+  deltaTz-\n");
            fprintf(f, "#.         /deg.F    in       in     deg.F     deg.F     deg.F     deg.F\n");
            fprintf(f, "#.         /deg.C    mm       mm     deg.C     deg.C     deg.C     deg.C\n\n");

            for (i = 0; i < st_thermal_load_no; i++) {
                if ((TestBit(st_layer, st_thermal_load[i].layer)) && (st_thermal_load[i].take_it)) {
                    if (st_thermal_load[i].factor_record >= 0) {
                        combi_load_factor = &st_load_factors[st_thermal_load[i].factor_record];
                        load = st_load_factors[st_thermal_load[i].factor_record].load;
                    } else {
                        combi_load_factor = &load_factors[abs(st_thermal_load[i].factor_record)];
                        load = load_factors[abs(st_thermal_load[i].factor_record)].load;
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
                    if (combi_factor & 1) gamma_l *= combi_load_factor->gamma;
                    if (combi_factor & 2) gamma_l *= combi_load_factor->psi0;
                    if (combi_factor & 4) gamma_l *= combi_load_factor->psi1;
                    if (combi_factor & 8) gamma_l *= combi_load_factor->psi2;
                    if (combi_factor & 16) gamma_l *= combi_load_factor->xi;
                    if (combi_factor & 32) gamma_l *= combi_load_factor->gamma_inf;

                    for (j = 0; j < st_property_no; j++) {
                        if (abs(st_element[st_thermal_load[i].element].property_no) == st_property[j].n)
                            break;
                    }

                    if (j == st_property_no) {
                        sprintf(report_row, "#%d %s\n", st_element[st_thermal_load[i].element].property_no,
                                _property_not_defined_);
                        strcat(report, report_row);
                        break;
                    }

                    sprintf(par[0], "%e", st_property[j].a);
                    set_decimal_format(par[1], st_thermal_load[i].ydepth, prop_precisions->dim_precision);
                    set_decimal_format(par[2], st_thermal_load[i].ydepth,
                                       prop_precisions->dim_precision);  //same as y axis
                    set_decimal_format(par[3], st_thermal_load[i].deltatplus * gamma_l, t_precision);
                    set_decimal_format(par[4], st_thermal_load[i].deltatminus * gamma_l, t_precision);
                    set_decimal_format(par[5], 0.0, t_precision);
                    set_decimal_format(par[6], 0.0, t_precision);

                    fprintf(f, "%d %s %s %s %s %s %s %s\n", st_thermal_load[i].element + 1, par[0], par[1], par[2],
                            par[3], par[4], par[5], par[6]);
                }
            }

            //prescribed displacements
            stl_displacement_no = 0;
            for (i = 0; i < st_displacement_no; i++) {
                if (TestBit(st_layer, st_displacement[i].layer)) stl_displacement_no++;
            }
            fprintf(f, "\n%d\t\t# number of nodes with prescribed displacements  nD<=nR (global)\n",
                    stl_displacement_no);
            fprintf(f, "#.node   X-displ  Y-displ  Z-displ  X-rot'n   Y-rot'n   Z-rot'n\n");
            fprintf(f, "#.         in       in       in      radian    radian    radian\n");
            fprintf(f, "#.         mm       mm       mm      radian    radian    radian\n");

            for (i = 0; i < st_displacement_no; i++) {
                if (TestBit(st_layer, st_displacement[i].layer)) {
                    //no partial factors (assumed)

                    set_decimal_format(par[0], st_displacement[i].dx, dim_precision);
                    set_decimal_format(par[1], st_displacement[i].dy, dim_precision);
                    set_decimal_format(par[2], 0.0, dim_precision);
                    set_decimal_format(par[3], 0.0, dim_precision);
                    set_decimal_format(par[4], 0.0, dim_precision);
                    set_decimal_format(par[5], st_displacement[i].dzz, dim_precision);

                    fprintf(f, "%d %s %s %s %s %s %s\n", st_displacement[i].node + 1, par[0], par[1], par[2], par[3],
                            par[4], par[5]);
                }
            }

            fprintf(f, "\n\t\t# End Static Load Case QPSLS %d Combination %d (%d)\n\n", combination_no, ci + 1,
                    case_number);

            //////////////////////////////
        }
    }

    //// QPSLS COMBINATIONS ENDS

    fprintf(f, "%d\t\t# number of dynamic modes of vibration\n\n", st_dynamic_no);

    fprintf(f, "1        # 1: subspace Jacobi     2: Stodola\n");
    fprintf(f, "0	    # 0: consistent mass ... 1: lumped mass matrix\n");
    fprintf(f, "1e-9     # mode shape tolerance\n");
    fprintf(f, "0.0		# shift value ... for unrestrained structures\n");
    fprintf(f, "%.1f     # exaggerate modal mesh deformations\n", q_magnitude);    //exaggerate_modal

    int load;
    int m_node;
    int m_element;
    int m_node1, m_node2;
    double m_length;
    double m_mass;
    double m_qx, m_qy;
    int mi;
    BOOL no_error;

    st_mass_node = NULL;
    st_mass_element = NULL;

    st_mass_nodes_no = 0;
    st_mass_elements_no = 0;

    if (inertia) {
        st_mass_node = (ST_MASS_NODE *) malloc(ST_MASS_NODE_MAX * sizeof(ST_MASS_NODE));
        st_mass_element = (ST_MASS_ELEMENT *) malloc(ST_MASS_ELEMENT_MAX * sizeof(ST_MASS_ELEMENT));

        for (i = 0; i < st_node_force_moment_no; i++) {
            load = load_factors[abs(st_node_force_moment[i].factor_record)].load;
            if (load == 1) {
                if ((!Check_if_Equal(st_node_force_moment[i].fy, 0.0)) ||
                    (!Check_if_Equal(st_node_force_moment[i].fx, 0.0))) {
                    m_node = st_node_force_moment[i].node;  //enumered from 0
                    m_mass = sqrt((st_node_force_moment[i].fx * st_node_force_moment[i].fx) +
                                  (st_node_force_moment[i].fy * st_node_force_moment[i].fy)) /
                             unit_factors->Ma_f; //in t or kip
                    //searching for existing node
                    for (mi = 0; mi < st_mass_nodes_no; mi++) {
                        if (st_mass_node[mi].node == m_node)
                            break;
                    }
                    if (mi < st_mass_nodes_no) {
                        st_mass_node[mi].mass += m_mass;
                    } else {
                        st_mass_node[st_mass_nodes_no].node = m_node;
                        st_mass_node[st_mass_nodes_no].mass = m_mass;
                        add_mass_node();
                    }
                }
            }
        }
        for (i = 0; i < st_uniform_load_no; i++) {
            load = load_factors[abs(st_uniform_load[i].factor_record)].load;
            if (load == 1) {
                m_element = st_uniform_load[i].element;
                m_node1 = st_element[m_element].node1;
                m_node2 = st_element[m_element].node2;
                m_length = sqrt(
                        ((st_node[m_node2].x - st_node[m_node1].x) * (st_node[m_node2].x - st_node[m_node1].x)) +
                        ((st_node[m_node2].y - st_node[m_node1].y) * (st_node[m_node2].y - st_node[m_node1].y)));

                m_length = milimetryob(m_length) / m_units_factor;

                m_qx = st_uniform_load[i].qx;
                m_qy = st_uniform_load[i].qy;
                m_mass = sqrt((m_qx * m_qx) + (m_qy * m_qy)) * m_length / unit_factors->Ma_f;  //in t or kip

                //searching for existing element
                for (mi = 0; mi < st_mass_elements_no; mi++) {
                    if (st_mass_element[mi].element == m_element)
                        break;
                }
                if (mi < st_mass_elements_no) {
                    st_mass_element[mi].mass += m_mass;
                } else {
                    st_mass_element[st_mass_elements_no].element = m_element;  //enumerated from 0
                    st_mass_element[st_mass_elements_no].mass = m_mass;
                    add_mass_element();
                }
            }
        }
        for (i = 0; i < st_trapezoid_load_no; i++) {
            load = load_factors[abs(st_trapezoid_load[i].factor_record)].load;
            if (load == 1) {
                m_element = st_trapezoid_load[i].element;
                m_node1 = st_element[m_element].node1;
                m_node2 = st_element[m_element].node2;
                m_length = st_trapezoid_load[i].dlxx2 - st_trapezoid_load[i].dlxx1;

                m_length = milimetryob(m_length) / m_units_factor;

                m_qx = (st_trapezoid_load[i].qx1 + st_trapezoid_load[i].qx2) / 2;
                m_qy = (st_trapezoid_load[i].qy1 + st_trapezoid_load[i].qy2) / 2;
                m_mass = sqrt((m_qx * m_qx) + (m_qy * m_qy)) * m_length / unit_factors->Ma_f;  //in t or kip
                //searching for existing element
                for (mi = 0; mi < st_mass_elements_no; mi++) {
                    if (st_mass_element[mi].element == m_element)
                        break;
                }
                if (mi < st_mass_elements_no) {
                    st_mass_element[mi].mass += m_mass;
                } else {
                    st_mass_element[st_mass_elements_no].element = m_element;  //enumerated from 0
                    st_mass_element[st_mass_elements_no].mass = m_mass;
                    add_mass_element();
                }
            }
        }
        for (i = 0; i < st_element_force_no; i++) {
            load = load_factors[abs(st_element_force[i].factor_record)].load;
            if (load == 1) {
                m_element = st_element_force[i].element;
                m_mass = sqrt((st_element_force[i].fx * st_element_force[i].fx) +
                              (st_element_force[i].fy * st_element_force[i].fy)) / unit_factors->Ma_f; //in t or kip
                //searching for existing element
                for (mi = 0; mi < st_mass_elements_no; mi++) {
                    if (st_mass_element[mi].element == m_element)
                        break;
                }
                if (mi < st_mass_elements_no) {
                    st_mass_element[mi].mass += m_mass;
                } else {
                    st_mass_element[st_mass_elements_no].element = m_element;  //enumerated from 0
                    st_mass_element[st_mass_elements_no].mass = m_mass;
                    add_mass_element();
                }
            }
        }
    }

    fprintf(f, "\n%d         # number of nodes with extra mass or inertia\n", st_mass_nodes_no);
    fprintf(f, "#.n      Mass   Ixx      Iyy      Izz\n");
    fprintf(f, "#        ton    ton.mm^2 ton.mm^2 ton.mm^2\n");
    fprintf(f, "#        kip    kip.in^2 kip.in^2 kip.in^2\n");
    for (i = 0; i < st_mass_nodes_no; i++) {
        set_decimal_format(par[0], st_mass_node[i].mass, force_precision);
        fprintf(f, "%d     %s     0        0        0\n", st_mass_node[i].node + 1, par[0]);
    }

    fprintf(f, "\n%d       # number of frame elements with extra mass\n", st_mass_elements_no);
    fprintf(f, "#.e  M   Mass\n");
    fprintf(f, "#        ton\n");
    fprintf(f, "#        kip\n");
    for (i = 0; i < st_mass_elements_no; i++) {
        set_decimal_format(par[0], st_mass_element[i].mass, force_precision);
        fprintf(f, "%d     %s\n", st_mass_element[i].element + 1, par[0]);
    }

    fprintf(f, "\n%d		# number of modes to animate, nA\n", st_dynamic_no);
    char modes_list[MaxTextLen];
    char mode_anim[16];
    strcpy(modes_list, "");
    for (int mn = 0; mn < st_dynamic_no; mn++) {
        sprintf(mode_anim, "%d   ", mn + 1);
        strcat(modes_list, mode_anim);
    }
    fprintf(f, "%s# list of modes to animate - omit if nA == 0\n", modes_list);
    fprintf(f, "2        # pan rate during animation\n\n");

    fprintf(f, "\n# End of input data file for %s\n", st_title);

    fclose(f);

    redcrsb(2, 167);

    if (st_mass_node != NULL) free(st_mass_node);
    if (st_mass_element != NULL) free(st_mass_element);

    free(st_uniform_load_comb);
    free(st_uniform_load_cons);

    free(st_node_pre);
    free(st_node_force_moment);
    free(st_uniform_load);
    free(st_trapezoid_load);
    free(st_element_force);
    free(st_thermal_load);
    free(st_displacement);
    free(st_load);
    free(st_load_factors);

    if (strlen(report) > 0) {
        int edit_params = 0;
        int tab;
        int single = 0;
        int mynCmdShow = 0;
        ret = EditText(report, edit_params, mynCmdShow, &single, &tab);
    } else {
        sprintf(params, "-i %s -o %s > frame3dd.ret", "alfacad.3dd", "alfacad.out");
        runcode = RunSilent("frame3dd.exe", params);

        printf("\nframe3dd runcode:%d\n", runcode);
        runcode_short = runcode >> 8;
        //runcode_short = runcode & 0xFF;
        printf("\nframe3dd runcode_shoort:%d\n", runcode_short);

        if (runcode_short > 0) {
            ret = ask_question(1, "", "OK", "", __FRAME3DD__, 12, frame3dd[runcode_short], 11, 1, 0);
        }


        no_error=TRUE;
        ////CREATING RESULT BLOCKS

        if ((runcode_short == 0) || ((runcode_short > 180) && (runcode_short < 200)) ||
            (strcmp(frame3dd[runcode_short], _ERROR_FREE_COMPLETION_) == 0)) {

            double x0 = get_localx();
            double y0 = get_localy();
            int N1, N2, nx;
            double X1, Y1, Z1, X2, Y2, Z2;
            double xdsp, ydsp, zdsp, xdsp1, ydsp1;
            BOOL new_line = TRUE, new_line1 = TRUE;
            LINIA Ldsp = Ldef, Ldsp1, Ldsp01, Ldsp11;
            char desired_layer[maxlen_w];
            char desired_layer_bak[maxlen_w];
            int desired_layer_no;
            int ii, il;
            BOOL hiding;

            Ldsp.blok = 1;
            Ldsp.kolor = static_colors.deformation_color;

            //Geometry
            //opening file just to confirm
            sprintf(params, "alfacad.out");
            f = fopen(params, "rt");
            if (f == NULL) {
                ret = ask_question(1, "", (char *) confirm, "", (char *) _CANNOT_CREATE_RESULTS_FILE_, 12, "", 11, 1,
                                   62);
                no_error=FALSE;
                goto error;
            }

            /////////////////////
            //Layers
            sprintf(desired_layer, "%s_%s", ptr_id, "geometry");

            sprintf(block_name, "%s$Nodes&Elms", ptr_id_short);

            for (ii = 0; ii < No_Layers; ii++) {
                if (strcmp(Layers[ii].name, desired_layer) == 0)

                    break;
            }
            if (ii < No_Layers) {
                desired_layer_no = ii;
                delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
            } else {
                if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {
                    ret = ask_question(1, "", (char *) confirm, "",
                                       (char *) _CANNOT_CREATE_NEW_LAYER_, 12, "", 11,
                                       1, 62);
                    no_error=FALSE;
                    goto error;
                }
                if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {
                    ret = ask_question(1, "", (char *) confirm, "",
                                       (char *) _CANNOT_CREATE_NEW_LAYER_, 12, "", 11,
                                       1, 62);
                    no_error=FALSE;
                    goto error;
                }
                No_Layers++;
                desired_layer_no = No_Layers - 1;
                Layers[No_Layers - 1].on = 1;
                Layers[No_Layers - 1].edit = 1;
                Layers[No_Layers - 1].point = 1;
                strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
                Layers[No_Layers - 1].color = Ldsp.kolor;
            }

            L.warstwa = desired_layer_no;
            L.kolor = static_colors.node_element_color;

            ptr_block = add_block(st_node[0].x, st_node[0].y, 'E', block_name, FALSE);
            if (ptr_block == NULL) {
                fclose(f);
                ret = ask_question(1, "", (char *) confirm, "",
                                   (char *) _CANNOT_CREATE_NODES_AND_ELEMENTS_BLOCK_, 12, "", 11,
                                   1, 62);
                no_error=FALSE;
                goto error;
            }

            for (i = 0; i < st_node_no; i++) {
                L.x1 = L.x2 = st_node[i].x;
                L.y1 = L.y2 = st_node[i].y;
                draw_geo_label(&L, i + 1, TRUE, FALSE, st_node[i].real);
            }
            for (i = 0; i < st_element_no; i++) {
                if ((st_element[i].node1r > -1) && (st_element[i].node2r > -1)) {
                    L.x1 = st_node[st_element[i].node1r].x;
                    L.y1 = st_node[st_element[i].node1r].y;
                    L.x2 = st_node[st_element[i].node2r].x;
                    L.y2 = st_node[st_element[i].node2r].y;
                    draw_geo_label(&L, i + 1, FALSE, TRUE, 1);
                } else {
                    L.x1 = st_node[st_element[i].node1].x;
                    L.y1 = st_node[st_element[i].node1].y;
                    L.x2 = st_node[st_element[i].node2].x;
                    L.y2 = st_node[st_element[i].node2].y;
                    draw_geo_label(&L, i + 1, FALSE, TRUE, 0);
                }
            }
            /////////////////////
            char SLS_ULS[12];
            char SLS_ULS_BAK[12];
            char SLS_ULS_BLK[12];
            //reactions
            int layer, layer_no;
            BOOL reactions = FALSE;
            int node_no;
            double Fx, Fy, Fz, Mxx, Myy, Mzz;

            double psize = Get_Point_Size() / 2.0;

            while (fgets(report_row, MaxTextLen, f)) {
                ptr = strstr(report_row, "L O A D   C A S E");
                ptr_max = strstr(report_row, "R E A C T I O N S");
                ptr_min = strstr(report_row, "  Node        Fx");
                if (ptr) {
                    //selecting layer
                    ret = sscanf(ptr + 18, "%d", &layer);
                    ptr = strstr(report_row, "O F");
                    if (ptr != 0) ret = sscanf(ptr + 4, "%d", &layer_no);
                        //else layer_no=st_layer_no;
                    else layer_no = combination_no;
                    reactions = FALSE;
                } else if (ptr_max) {
                    //next row header of reactions begins
                    reactions = TRUE;
                } else if (ptr_min) {
                    //next row reactions begins
                    if (reactions == TRUE) {
                        il = combi_total_numbers[layer - 1].case_no;

                        if (combi_total_numbers[layer - 1].combi == 0) {
                            strcpy(SLS_ULS, "sls");
                            strcpy(SLS_ULS_BAK, "case");
                            strcpy(SLS_ULS_BLK, "sls");
                        }
                        else if (combi_total_numbers[layer - 1].combi == 1) {
                            strcpy(SLS_ULS, "ULS");
                            strcpy(SLS_ULS_BAK, "ULS");
                            strcpy(SLS_ULS_BLK, "ULS");
                        }
                        else {
                            if (combi_total_numbers[layer - 1].combi == 2) {
                                strcpy(SLS_ULS, "ULSLC");
                                strcpy(SLS_ULS_BAK, "ULSLC");
                                strcpy(SLS_ULS_BLK, "ULSLC");
                            }
                            else if (combi_total_numbers[layer - 1].combi == 3) {
                                strcpy(SLS_ULS, "slsLC");
                                strcpy(SLS_ULS_BAK, "slsLC");
                                strcpy(SLS_ULS_BLK, "slsLC");
                            }
                            else if (combi_total_numbers[layer - 1].combi == 4) {
                                if (qpslsLC_Layer) {
                                    strcpy(SLS_ULS, "qpslsLC");
                                    strcpy(SLS_ULS_BAK, "qpslsLC");
                                    strcpy(SLS_ULS_BLK, "qpslsLC");
                                }
                                else {
                                    strcpy(SLS_ULS, "slsLC");
                                    strcpy(SLS_ULS_BAK, "slsLC");
                                    strcpy(SLS_ULS_BLK, "qpslsLC");
                                }
                            }
                        }

                        sprintf(desired_layer_bak, "%s_%s_%s_%d", ptr_id, "reactions", SLS_ULS_BAK, combination_no);
                        sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "reactions", SLS_ULS, combination_no);
                        sprintf(block_name, "%s$R_%s_%d_%d", ptr_id_short, SLS_ULS_BLK, combination_no,
                                combi_total_numbers[layer - 1].combination);

                        hiding = (combi_total_numbers[layer - 1].combi != 2);  //ULSLC only

                        for (ii = 0; ii < No_Layers; ii++) {
                            if (strcmp(Layers[ii].name, desired_layer_bak) == 0) {
                                strcpy(Layers[ii].name, desired_layer);
                                break;
                            } else if (strcmp(Layers[ii].name, desired_layer) == 0)
                                break;
                        }
                        if (ii < No_Layers) {
                            desired_layer_no = ii;
                            if ((combi_total_numbers[layer - 1].first) &&
                                ((combi_total_numbers[layer - 1].combi != 4) || (qpslsLC_Layer == TRUE)))
                                delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
                        } else {
                            if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {
                                ret = ask_question(1, "", (char *) confirm, "",
                                                   (char *) _CANNOT_CREATE_NEW_LAYER_, 12, "", 11,
                                                   1, 62);
                                no_error=FALSE;
                                goto error;
                            }
                            if ((combi_total_numbers[layer - 1].combi < 2) ||
                                (combi_total_numbers[layer - 1].last == 1)) {
                                No_Layers++;
                                desired_layer_no = No_Layers - 1;
                                Layers[No_Layers - 1].on = 1;
                                Layers[No_Layers - 1].edit = 1;
                                Layers[No_Layers - 1].point = 1;
                                strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
                                Layers[No_Layers - 1].color = Ldsp.kolor;
                            }
                        }

                        Ldsp.kolor = static_colors.reaction_color;  //cyan
                        Ldsp.warstwa = desired_layer_no;

                        L.warstwa = Ldsp.warstwa;
                        L.kolor = Ldsp.kolor;

                        ////sprintf(params, "$R_%d", i + 1);

                        if ((combi_total_numbers[layer - 1].combi < 2) || (combi_total_numbers[layer - 1].last == 1)) {
                            ptr_block = add_block(st_node[0].x, st_node[0].y, 'E', block_name, hiding);
                            if (ptr_block == NULL) {
                                fclose(f);
                                ret = ask_question(1, "", (char *) confirm, "",
                                                   (char *) _CANNOT_CREATE_REACTIONS_BLOCK_, 12, "", 11,
                                                   1, 62);
                                no_error=FALSE;
                                goto error;
                            }
                        }

                        for (ii = 0; ii < st_node_no; ii++) {
                            fgets(report_row, MaxTextLen, f);
                            ret = sscanf(report_row, "%d %lf %lf %lf %lf %lf %lf", &node_no, &Fx, &Fy, &Fz, &Mxx, &Myy,
                                         &Mzz);
                            node_no--;

                            BOOL active = FALSE;
                            for (int iii = 0; iii < st_reaction_no; iii++) {
                                if (st_reaction[iii].node == node_no) {
                                    active = st_reaction[iii].active;
                                    break;
                                }
                            }

                            if (active) {
                                double Fx_min;
                                double Fx_max;
                                double Fy_min;
                                double Fy_max;
                                double Mzz_min;
                                double Mzz_max;

                                if (combi_total_numbers[layer - 1].combi > 1)  //combi
                                {
                                    if (combi_total_numbers[layer - 1].first ==
                                        1)  //creating max reactions data and initializing values
                                    {
                                        switch (combi_total_numbers[layer - 1].combi) {
                                            case 2:
                                                combi_reactions[combi_reactions_table[node_no]].uls.Fx_min = Fx;
                                                combi_reactions[combi_reactions_table[node_no]].uls.Fx_max = Fx;
                                                combi_reactions[combi_reactions_table[node_no]].uls.Fy_min = Fy;
                                                combi_reactions[combi_reactions_table[node_no]].uls.Fy_max = Fy;
                                                combi_reactions[combi_reactions_table[node_no]].uls.Mzz_min = Mzz;
                                                combi_reactions[combi_reactions_table[node_no]].uls.Mzz_max = Mzz;
                                                break;
                                            case 3:
                                                combi_reactions[combi_reactions_table[node_no]].sls.Fx_min = Fx;
                                                combi_reactions[combi_reactions_table[node_no]].sls.Fx_max = Fx;
                                                combi_reactions[combi_reactions_table[node_no]].sls.Fy_min = Fy;
                                                combi_reactions[combi_reactions_table[node_no]].sls.Fy_max = Fy;
                                                combi_reactions[combi_reactions_table[node_no]].sls.Mzz_min = Mzz;
                                                combi_reactions[combi_reactions_table[node_no]].sls.Mzz_max = Mzz;
                                                break;
                                            case 4:
                                                combi_reactions[combi_reactions_table[node_no]].qpsls.Fx_min = Fx;
                                                combi_reactions[combi_reactions_table[node_no]].qpsls.Fx_max = Fx;
                                                combi_reactions[combi_reactions_table[node_no]].qpsls.Fy_min = Fy;
                                                combi_reactions[combi_reactions_table[node_no]].qpsls.Fy_max = Fy;
                                                combi_reactions[combi_reactions_table[node_no]].qpsls.Mzz_min = Mzz;
                                                combi_reactions[combi_reactions_table[node_no]].qpsls.Mzz_max = Mzz;
                                                break;
                                        }
                                    } else if (combi_total_numbers[layer - 1].last == 1)  //getting results from data
                                    {
                                        switch (combi_total_numbers[layer - 1].combi) {
                                            case 2:
                                                Fx_min = min(combi_reactions[combi_reactions_table[node_no]].uls.Fx_min,
                                                             Fx);
                                                Fx_max = max(combi_reactions[combi_reactions_table[node_no]].uls.Fx_max,
                                                             Fx);
                                                Fy_min = min(combi_reactions[combi_reactions_table[node_no]].uls.Fy_min,
                                                             Fy);
                                                Fy_max = max(combi_reactions[combi_reactions_table[node_no]].uls.Fy_max,
                                                             Fy);
                                                Mzz_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].uls.Mzz_min,
                                                        Mzz);
                                                Mzz_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].uls.Mzz_max,
                                                        Mzz);
                                                break;
                                            case 3:
                                                Fx_min = min(combi_reactions[combi_reactions_table[node_no]].sls.Fx_min,
                                                             Fx);
                                                Fx_max = max(combi_reactions[combi_reactions_table[node_no]].sls.Fx_max,
                                                             Fx);
                                                Fy_min = min(combi_reactions[combi_reactions_table[node_no]].sls.Fy_min,
                                                             Fy);
                                                Fy_max = max(combi_reactions[combi_reactions_table[node_no]].sls.Fy_max,
                                                             Fy);
                                                Mzz_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].sls.Mzz_min,
                                                        Mzz);
                                                Mzz_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].sls.Mzz_max,
                                                        Mzz);
                                                break;
                                            case 4:
                                                Fx_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].qpsls.Fx_min,
                                                        Fx);
                                                Fx_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].qpsls.Fx_max,
                                                        Fx);
                                                Fy_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].qpsls.Fy_min,
                                                        Fy);
                                                Fy_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].qpsls.Fy_max,
                                                        Fy);
                                                Mzz_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].qpsls.Mzz_min,
                                                        Mzz);
                                                Mzz_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].qpsls.Mzz_max,
                                                        Mzz);
                                                break;
                                        }

                                    } else //upgrade
                                    {
                                        switch (combi_total_numbers[layer - 1].combi) {
                                            case 2:
                                                combi_reactions[combi_reactions_table[node_no]].uls.Fx_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].uls.Fx_min, Fx);
                                                combi_reactions[combi_reactions_table[node_no]].uls.Fx_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].uls.Fx_max, Fx);
                                                combi_reactions[combi_reactions_table[node_no]].uls.Fy_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].uls.Fy_min, Fy);
                                                combi_reactions[combi_reactions_table[node_no]].uls.Fy_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].uls.Fy_max, Fy);
                                                combi_reactions[combi_reactions_table[node_no]].uls.Mzz_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].uls.Mzz_min,
                                                        Mzz);
                                                combi_reactions[combi_reactions_table[node_no]].uls.Mzz_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].uls.Mzz_max,
                                                        Mzz);
                                                break;
                                            case 3:
                                                combi_reactions[combi_reactions_table[node_no]].sls.Fx_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].sls.Fx_min, Fx);
                                                combi_reactions[combi_reactions_table[node_no]].sls.Fx_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].sls.Fx_max, Fx);
                                                combi_reactions[combi_reactions_table[node_no]].sls.Fy_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].sls.Fy_min, Fy);
                                                combi_reactions[combi_reactions_table[node_no]].sls.Fy_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].sls.Fy_max, Fy);
                                                combi_reactions[combi_reactions_table[node_no]].sls.Mzz_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].sls.Mzz_min,
                                                        Mzz);
                                                combi_reactions[combi_reactions_table[node_no]].sls.Mzz_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].sls.Mzz_max,
                                                        Mzz);
                                                break;
                                            case 4:
                                                combi_reactions[combi_reactions_table[node_no]].qpsls.Fx_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].qpsls.Fx_min,
                                                        Fx);
                                                combi_reactions[combi_reactions_table[node_no]].qpsls.Fx_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].qpsls.Fx_max,
                                                        Fx);
                                                combi_reactions[combi_reactions_table[node_no]].qpsls.Fy_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].qpsls.Fy_min,
                                                        Fy);
                                                combi_reactions[combi_reactions_table[node_no]].qpsls.Fy_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].qpsls.Fy_max,
                                                        Fy);
                                                combi_reactions[combi_reactions_table[node_no]].qpsls.Mzz_min = min(
                                                        combi_reactions[combi_reactions_table[node_no]].qpsls.Mzz_min,
                                                        Mzz);
                                                combi_reactions[combi_reactions_table[node_no]].qpsls.Mzz_max = max(
                                                        combi_reactions[combi_reactions_table[node_no]].qpsls.Mzz_max,
                                                        Mzz);
                                                break;
                                        }

                                    }
                                }


                                if ((combi_total_numbers[layer - 1].combi < 2) ||
                                    (combi_total_numbers[layer - 1].last == 1)) {
                                    if (combi_total_numbers[layer - 1].combi < 2) {
                                        Fx /= unit_factors->R_f;
                                        Fy /= unit_factors->R_f;
                                        Mzz /= unit_factors->M_f;

                                        if (!Check_if_Equal(Fx, 0.0)) {
                                            draw_reaction_xy(&L, &st_node[node_no], Fx, 0);
                                        }
                                        if (!Check_if_Equal(Fy, 0.0)) {
                                            draw_reaction_xy(&L, &st_node[node_no], Fy, 1);
                                        }
                                        if (!Check_if_Equal(Mzz, 0.0)) {
                                            draw_reaction_m(&L, &st_node[node_no], Mzz);
                                        }
                                    } else {
                                        if (copysign(1.0, Fx_min) == copysign(1.0, Fx_max)) {
                                            if (fabs(Fx_min) < fabs(Fx_max)) Fx_min = 0;
                                            else Fx_max = 0;
                                        }

                                        if (copysign(1.0, Fy_min) == copysign(1.0, Fy_max)) {
                                            if (fabs(Fy_min) < fabs(Fy_max)) Fy_min = 0;
                                            else Fy_max = 0;
                                        }

                                        if (copysign(1.0, Mzz_min) == copysign(1.0, Mzz_max)) {
                                            if (fabs(Mzz_min) < fabs(Mzz_max)) Mzz_min = 0;
                                            else Mzz_max = 0;
                                        }

                                        Fx_min /= unit_factors->R_f;
                                        Fy_min /= unit_factors->R_f;
                                        Mzz_min /= unit_factors->M_f;

                                        if (!Check_if_Equal(Fx_min, 0.0)) {
                                            draw_reaction_xy(&L, &st_node[node_no], Fx_min, 0);
                                        }
                                        if (!Check_if_Equal(Fy_min, 0.0)) {
                                            draw_reaction_xy(&L, &st_node[node_no], Fy_min, 1);
                                        }
                                        if (!Check_if_Equal(Mzz_min, 0.0)) {
                                            draw_reaction_m(&L, &st_node[node_no], Mzz_min);
                                        }

                                        Fx_max /= unit_factors->R_f;
                                        Fy_max /= unit_factors->R_f;
                                        Mzz_max /= unit_factors->M_f;

                                        if (!Check_if_Equal(Fx_max, 0.0)) {
                                            draw_reaction_xy(&L, &st_node[node_no], Fx_max, 0);
                                        }
                                        if (!Check_if_Equal(Fy_max, 0.0)) {
                                            draw_reaction_xy(&L, &st_node[node_no], Fy_max, 1);
                                        }
                                        if (!Check_if_Equal(Mzz_max, 0.0)) {
                                            draw_reaction_m(&L, &st_node[node_no], Mzz_max);
                                        }
                                    }

                                }
                            }
                        }
                    }
                }
            }

            fclose(f);

            /////////////////////
            //Internal deformations and forces
            Ldsp.blok = 1;
            Ldsp.kolor = static_colors.deformation_color;

            DATA_ROW *dr;
            int *cfp, *cfm;
            BOOL not_ignored = TRUE;  //TEMPORARY
            double S, Sp, Sm, Ss;  //sigma, sigma plus, sigma minus
            double NxM, VyM, VzM, TxM, MyM, MzM, DxM, DyM, DzM, RxM, SpM, SmM, SsM; //sigma plus max, sigma minus max
            double Nxm, Vym, Vzm, Txm, Mym, Mzm, Dxm, Dym, Dzm, Rxm, Spm, Smm, Ssm; //sigma plus min, sigma minus min
            double Dxy, Dxy0, Nx0, Vy0, Mz0, Dxy01, Nx01, Vy01, Mz01, Sp0, Sm0, Ss0, Sp01, Sm01, Ss01, Ax, Wy;
            double x11, y11, x11_, y11_;
            LINIA Le = Ldef;
            PLINIA PL1;
            int n1, n2;
            BOOL ignore_element = FALSE;
            double kos1, koc1;
            char *ptr_l, *ptr_l_;
            double xd, xdmin;

            LINIA Ldsp_, Ldsp1_;

            //Dx, Dy

            for (i = 0; i < combinations_number; i++)   //st_layer_no
            {
                /////////////////////

                il = combi_total_numbers[i].case_no;

                if (combi_total_numbers[i].combi == 0) {
                    strcpy(SLS_ULS, "sls");
                    strcpy(SLS_ULS_BAK, "case");
                    strcpy(SLS_ULS_BLK, "sls");
                }
                else if (combi_total_numbers[i].combi == 1) {
                    strcpy(SLS_ULS, "ULS");
                    strcpy(SLS_ULS_BAK, "ULS");
                    strcpy(SLS_ULS_BLK, "ULS");
                }
                else {
                    if (combi_total_numbers[i].combi == 2) {
                        strcpy(SLS_ULS, "ULSLC");
                        strcpy(SLS_ULS_BAK, "ULSLC");
                        strcpy(SLS_ULS_BLK, "ULSLC");
                    }
                    else if (combi_total_numbers[i].combi == 3) {
                        strcpy(SLS_ULS, "slsLC");
                        strcpy(SLS_ULS_BAK, "slsLC");
                        strcpy(SLS_ULS_BLK, "slsLC");
                    }
                    else if (combi_total_numbers[i].combi == 4) {
                        if (qpslsLC_Layer) {
                            strcpy(SLS_ULS, "qpslsLC");
                            strcpy(SLS_ULS_BAK, "qpslsLC");
                            strcpy(SLS_ULS_BLK, "qpslsLC");
                        }
                        else {
                            strcpy(SLS_ULS, "slsLC");
                            strcpy(SLS_ULS_BAK, "slsLC");
                            strcpy(SLS_ULS_BLK, "qpslsLC");
                        }
                    }
                }


                sprintf(desired_layer_bak, "%s_%s_%s_%d", ptr_id, "deformations", SLS_ULS_BAK, combination_no);
                sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "deformations", SLS_ULS, combination_no);
                sprintf(block_name, "%s$D_%s_%d_%d", ptr_id_short, SLS_ULS_BLK, combination_no,
                        combi_total_numbers[i].combination);


                //hiding=(i%2==1);
                hiding = (combi_total_numbers[i].combi != 3);  //SLSCL only

                for (ii = 0; ii < No_Layers; ii++) {
                    if (strcmp(Layers[ii].name, desired_layer_bak) == 0) {
                        strcpy(Layers[ii].name, desired_layer);
                        break;
                    }
                    if (strcmp(Layers[ii].name, desired_layer) == 0)
                        break;
                }
                if (ii < No_Layers) {
                    desired_layer_no = ii;
                    if ((combi_total_numbers[i].first) &&
                        ((combi_total_numbers[i].combi != 4) || (qpslsLC_Layer == TRUE)))
                        delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
                } else {
                    if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {
                        ret = ask_question(1, "", (char *) confirm, "",
                                           (char *) _CANNOT_CREATE_NEW_LAYER_, 12, "", 11,
                                           1, 62);
                        no_error=FALSE;
                        goto error;
                    }

                    if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                        No_Layers++;
                        desired_layer_no = No_Layers - 1;
                        Layers[No_Layers - 1].on = 1;
                        Layers[No_Layers - 1].edit = 1;
                        Layers[No_Layers - 1].point = 1;
                        strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
                        Layers[No_Layers - 1].color = Ldsp.kolor;
                    }
                }

                Ldsp.warstwa = desired_layer_no;
                memmove(&Ldsp_, &Ldsp, sizeof(LINIA));
                /////////////////////
                memmove(&Ldsp1, &Ldsp, sizeof(LINIA));
                /////////////////////

                memmove(&Ldsp_, &Ldsp, sizeof(LINIA));

                sprintf(params, "alfacad.if%02d", i + 1);
                f = fopen(params, "rt");
                if (f == NULL) {

                    ret = ask_question(1, "", (char *) confirm, "",
                                       (char *) _CANNOT_OPEN_RESULTS_DATA_FILE_, 12, "", 11,
                                       1, 62);
                    no_error=FALSE;
                    goto error;
                }

                if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                    ptr_block = add_block(st_node[0].x, st_node[0].y, 'E', block_name, hiding);
                    if (ptr_block == NULL) {
                        fclose(f);
                        ret = ask_question(1, "", (char *) confirm, "",
                                           (char *) _CANNOT_CREATE_DEFORMATION_BLOCK_, 12, "", 11,
                                           1, 62);
                        no_error=FALSE;
                        goto error;
                    }
                }

                new_line = TRUE;
                new_line1 = TRUE;


                while (fgets(report_row, MaxTextLen, f)) {
                    double x, Nx, Vy, Vz, Tx, My, Mz, Dx, Dy, Dz, Rx;
                    ptr = strstr(report_row, "# @");
                    ptr_max = strstr(report_row, "# MAXIMUM");
                    ptr_min = strstr(report_row, "# MINIMUM");
                    ptr_row = strstr(report_row, "#.x");
                    if (ptr) {
                        ret = sscanf(ptr + 4, "%d %d %d %lf %lf %lf %lf %lf %lf %d", &rep_element_no, &N1, &N2, &X1,
                                     &Y1, &Z1, &X2, &Y2, &Z2, &nx);

                        dr = (DATA_ROW *) malloc(nx * sizeof(DATA_ROW));
                        cfp = (int *) malloc(nx * sizeof(int));
                        cfm = (int *) malloc(nx * sizeof(int));

                        n1 = st_element[rep_element_no - 1].node1;
                        n2 = st_element[rep_element_no - 1].node2;
                        Le.x1 = st_node[n1].x;
                        Le.y1 = st_node[n1].y;
                        Le.x2 = st_node[n2].x;
                        Le.y2 = st_node[n2].y;
                        parametry_lini(&Le, &PL);
                        kos = sin(PL.kat * Pi / 180);
                        koc = cos(PL.kat * Pi / 180);

                        kos1 = sin(Angle_Normal((PL.kat + 90) * Pi / 180));
                        koc1 = cos(Angle_Normal((PL.kat + 90) * Pi / 180));

                        int property_no = st_element[rep_element_no - 1].property_no;
                        //search for properties
                        for (int ip = 0; ip < st_property_no; ip++) {
                            if (st_property[ip].n == property_no) {
                                Ax = st_property[ip].A * unit_factors->Am_f;
                                Asy = st_property[ip].Asy * unit_factors->Am_f;
                                Wy = st_property[ip].Wy;
                                break;
                            }
                            if (ip == st_property_no) {
                                sprintf(report_row, "#%d %s\n", st_element[st_thermal_load[i].element].property_no,
                                        _property_not_defined_);
                                strcat(report, report_row);
                                break;
                            }
                        }

                        Dxy0 = 0;
                        Nx0 = 0;
                        Vy0 = 0;
                        Mz0 = 0;
                        Dxy01 = 0;
                        Nx01 = 0;
                        Vy01 = 0;
                        Mz01 = 0;
                        strcpy(T_text, "");
                        new_line = TRUE;
                        new_line1 = TRUE;


                        if (combi_total_numbers[i].combi > 1)  //combi
                        {
                            if (combi_total_numbers[i].first ==
                                1)  //creating min max values data for element and initializing values, keep them until stress are generated in next steps
                            {
                                if (combi_total_numbers[i].combi == 2)   //initiation on first state, so ULSLC
                                {
                                    combi_element_uls[rep_element_no - 1].nx = nx;
                                    combi_element_uls[rep_element_no - 1].fd = (COMBI_FORCES *) malloc(
                                            nx * sizeof(COMBI_FORCES));

                                    combi_element_sls[rep_element_no - 1].nx = nx;
                                    combi_element_sls[rep_element_no - 1].fd = (COMBI_FORCES *) malloc(
                                            nx * sizeof(COMBI_FORCES));

                                    combi_element_qpsls[rep_element_no - 1].nx = nx;
                                    combi_element_qpsls[rep_element_no - 1].fd = (COMBI_FORCES *) malloc(
                                            nx * sizeof(COMBI_FORCES));
                                }
                            } else if (combi_total_numbers[i].last == 1)  //final upgrade and freeing
                            {

                            } else  //upgrade
                            {

                            }
                        }
                    } else if (ptr_max) {
                        ret = sscanf(ptr_max + 10, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &NxM, &VyM, &VzM, &TxM,
                                     &MyM, &MzM, &DxM, &DyM, &DzM, &RxM);
                        if (combi_total_numbers[i].combi > 1)  //combi
                        {
                            COMBI_ELEMENT *combi_element;
                            if (combi_total_numbers[i].combi == 2) combi_element = combi_element_uls;
                            else if (combi_total_numbers[i].combi == 3) combi_element = combi_element_sls;
                            else combi_element = combi_element_qpsls;

                            if (combi_total_numbers[i].first ==
                                1)  //creating min max values data for element and initializing values
                            {

                                combi_element[rep_element_no - 1].NxM = NxM;
                                combi_element[rep_element_no - 1].VyM = VyM;
                                combi_element[rep_element_no - 1].MzM = MzM;
                                combi_element[rep_element_no - 1].DxM = DxM;
                                combi_element[rep_element_no - 1].DyM = DyM;

                                Nxm /= unit_factors->F_f;
                                Vym /= unit_factors->F_f;
                                Mzm /= unit_factors->M_f;

                                Spm = (Nxm / Ax + fabs(Mzm) / Wy) / unit_factors->S_f;
                                Smm = (Nxm / Ax - fabs(Mzm) / Wy) / unit_factors->S_f;
                                combi_element[rep_element_no - 1].SpM = Spm;
                                combi_element[rep_element_no - 1].Spm = Spm;
                                combi_element[rep_element_no - 1].SmM = Smm;
                                combi_element[rep_element_no - 1].Smm = Smm;
                            } else if (combi_total_numbers[i].last == 1)  //final upgrade
                            {
                                combi_element[rep_element_no - 1].NxM = max(combi_element[rep_element_no - 1].NxM, NxM);
                                combi_element[rep_element_no - 1].VyM = max(combi_element[rep_element_no - 1].VyM, VyM);
                                combi_element[rep_element_no - 1].MzM = max(combi_element[rep_element_no - 1].MzM, MzM);
                                combi_element[rep_element_no - 1].DxM = max(combi_element[rep_element_no - 1].DxM, DxM);
                                combi_element[rep_element_no - 1].DyM = max(combi_element[rep_element_no - 1].DyM, DyM);

                                Nxm /= unit_factors->F_f;
                                Vym /= unit_factors->F_f;
                                Mzm /= unit_factors->M_f;

                                Spm = (Nxm / Ax + fabs(Mzm) / Wy) / unit_factors->S_f;
                                Smm = (Nxm / Ax - fabs(Mzm) / Wy) / unit_factors->S_f;
                                combi_element[rep_element_no - 1].SpM = max(combi_element[rep_element_no - 1].SpM, Spm);
                                combi_element[rep_element_no - 1].Spm = min(combi_element[rep_element_no - 1].Spm, Spm);
                                combi_element[rep_element_no - 1].SmM = max(combi_element[rep_element_no - 1].SmM, Smm);
                                combi_element[rep_element_no - 1].Smm = min(combi_element[rep_element_no - 1].Smm, Smm);
                            } else  //upgrade
                            {
                                combi_element[rep_element_no - 1].NxM = max(combi_element[rep_element_no - 1].NxM, NxM);
                                combi_element[rep_element_no - 1].VyM = max(combi_element[rep_element_no - 1].VyM, VyM);
                                combi_element[rep_element_no - 1].MzM = max(combi_element[rep_element_no - 1].MzM, MzM);
                                combi_element[rep_element_no - 1].DxM = max(combi_element[rep_element_no - 1].DxM, DxM);
                                combi_element[rep_element_no - 1].DyM = max(combi_element[rep_element_no - 1].DyM, DyM);

                                Nxm /= unit_factors->F_f;
                                Vym /= unit_factors->F_f;
                                Mzm /= unit_factors->M_f;

                                Spm = (Nxm / Ax + fabs(Mzm) / Wy) / unit_factors->S_f;
                                Smm = (Nxm / Ax - fabs(Mzm) / Wy) / unit_factors->S_f;
                                combi_element[rep_element_no - 1].SpM = max(combi_element[rep_element_no - 1].SpM, Spm);
                                combi_element[rep_element_no - 1].Spm = min(combi_element[rep_element_no - 1].Spm, Spm);
                                combi_element[rep_element_no - 1].SmM = max(combi_element[rep_element_no - 1].SmM, Smm);
                                combi_element[rep_element_no - 1].Smm = min(combi_element[rep_element_no - 1].Smm, Smm);
                            }
                        }

                        NxM /= unit_factors->F_f;
                        VyM /= unit_factors->F_f;
                        MzM /= unit_factors->M_f;

                    } else if (ptr_min) {
                        ret = sscanf(ptr_min + 10, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &Nxm, &Vym, &Vzm, &Txm,
                                     &Mym, &Mzm, &Dxm, &Dym, &Dzm, &Rxm);

                        if (combi_total_numbers[i].combi > 1)  //combi
                        {
                            COMBI_ELEMENT *combi_element;
                            if (combi_total_numbers[i].combi == 2) combi_element = combi_element_uls;
                            else if (combi_total_numbers[i].combi == 3) combi_element = combi_element_sls;
                            else combi_element = combi_element_qpsls;

                            double Dxm_ = Dxm;
                            double DxM_ = combi_element[rep_element_no - 1].DxM; //DxM;
                            double Dym_ = Dym;
                            double DyM_ = combi_element[rep_element_no - 1].DyM; //DyM;

                            if (fabs(Dxm_) < fabs(DxM_)) {
                                if (copysign(1.0, Dxm_) == copysign(1.0, DxM_)) Dxm_ = 0.0;
                            } else {
                                if (copysign(1.0, DxM_) == copysign(1.0, Dxm_)) DxM_ = 0.0;
                            }
                            if (fabs(Dym_) < fabs(DyM_)) {
                                if (copysign(1.0, Dym_) == copysign(1.0, DyM_)) Dym_ = 0.0;
                            } else {
                                if (copysign(1.0, DyM_) == copysign(1.0, Dym_)) DyM_ = 0.0;
                            }

                            if (combi_total_numbers[i].first ==
                                1)  //creating min max values data for element and initializing values
                            {
                                combi_element[rep_element_no - 1].Nxm = Nxm;
                                combi_element[rep_element_no - 1].Vym = Vym;
                                combi_element[rep_element_no - 1].Mzm = Mzm;
                                combi_element[rep_element_no - 1].Dxm = Dxm_;
                                combi_element[rep_element_no - 1].Dym = Dym_;

                                //correction
                                combi_element[rep_element_no - 1].DxM = DxM_;
                                combi_element[rep_element_no - 1].DyM = DyM_;

                                Nxm /= unit_factors->F_f;
                                Vym /= unit_factors->F_f;
                                Mzm /= unit_factors->M_f;

                                combi_element[rep_element_no - 1].Spm = (Nxm / Ax + fabs(Mzm) / Wy) / unit_factors->S_f;
                                combi_element[rep_element_no - 1].Smm = (Nxm / Ax - fabs(Mzm) / Wy) / unit_factors->S_f;
                                combi_element[rep_element_no - 1].SpM = (Nxm / Ax + fabs(Mzm) / Wy) / unit_factors->S_f;
                                combi_element[rep_element_no - 1].SmM = (Nxm / Ax - fabs(Mzm) / Wy) / unit_factors->S_f;
                            } else if (combi_total_numbers[i].last == 1)  //final upgrade
                            {
                                combi_element[rep_element_no - 1].Nxm = min(combi_element[rep_element_no - 1].Nxm, Nxm);
                                combi_element[rep_element_no - 1].Vym = min(combi_element[rep_element_no - 1].Vym, Vym);
                                combi_element[rep_element_no - 1].Mzm = min(combi_element[rep_element_no - 1].Mzm, Mzm);
                                combi_element[rep_element_no - 1].Dxm = min(combi_element[rep_element_no - 1].Dxm,
                                                                            Dxm_);
                                combi_element[rep_element_no - 1].Dym = min(combi_element[rep_element_no - 1].Dym,
                                                                            Dym_);

                                //correction
                                combi_element[rep_element_no - 1].DxM = max(combi_element[rep_element_no - 1].DxM,
                                                                            DxM_);
                                combi_element[rep_element_no - 1].DyM = max(combi_element[rep_element_no - 1].DyM,
                                                                            DyM_);

                                Nxm /= unit_factors->F_f;
                                Vym /= unit_factors->F_f;
                                Mzm /= unit_factors->M_f;

                                Spm = (Nxm / Ax + fabs(Mzm) / Wy) / unit_factors->S_f;
                                Smm = (Nxm / Ax - fabs(Mzm) / Wy) / unit_factors->S_f;
                                combi_element[rep_element_no - 1].SpM = max(combi_element[rep_element_no - 1].SpM, Spm);
                                combi_element[rep_element_no - 1].Spm = min(combi_element[rep_element_no - 1].Spm, Spm);
                                combi_element[rep_element_no - 1].SmM = max(combi_element[rep_element_no - 1].SmM, Smm);
                                combi_element[rep_element_no - 1].Smm = min(combi_element[rep_element_no - 1].Smm, Smm);
                            } else  //upgrade
                            {
                                combi_element[rep_element_no - 1].Nxm = min(combi_element[rep_element_no - 1].Nxm, Nxm);
                                combi_element[rep_element_no - 1].Vym = min(combi_element[rep_element_no - 1].Vym, Vym);
                                combi_element[rep_element_no - 1].Mzm = min(combi_element[rep_element_no - 1].Mzm, Mzm);
                                combi_element[rep_element_no - 1].Dxm = min(combi_element[rep_element_no - 1].Dxm,
                                                                            Dxm_);
                                combi_element[rep_element_no - 1].Dym = min(combi_element[rep_element_no - 1].Dym,
                                                                            Dym_);

                                //correction
                                combi_element[rep_element_no - 1].DxM = max(combi_element[rep_element_no - 1].DxM,
                                                                            DxM_);
                                combi_element[rep_element_no - 1].DyM = max(combi_element[rep_element_no - 1].DyM,
                                                                            DyM_);

                                Nxm /= unit_factors->F_f;
                                Vym /= unit_factors->F_f;
                                Mzm /= unit_factors->M_f;

                                Spm = (Nxm / Ax + fabs(Mzm) / Wy) / unit_factors->S_f;
                                Smm = (Nxm / Ax - fabs(Mzm) / Wy) / unit_factors->S_f;
                                combi_element[rep_element_no - 1].SpM = max(combi_element[rep_element_no - 1].SpM, Spm);
                                combi_element[rep_element_no - 1].Spm = min(combi_element[rep_element_no - 1].Spm, Spm);
                                combi_element[rep_element_no - 1].SmM = max(combi_element[rep_element_no - 1].SmM, Smm);
                                combi_element[rep_element_no - 1].Smm = min(combi_element[rep_element_no - 1].Smm, Smm);
                            }
                        }

                        Nxm /= unit_factors->F_f;
                        Vym /= unit_factors->F_f;
                        Mzm /= unit_factors->M_f;

                        if (fabs(Dxm) < fabs(DxM)) {
                            if (copysign(1.0, Dxm) == copysign(1.0, DxM)) Dxm = 0.0;
                        } else {
                            if (copysign(1.0, DxM) == copysign(1.0, Dxm)) DxM = 0.0;
                        }

                        if (fabs(Dym) < fabs(DyM)) {
                            if (copysign(1.0, Dym) == copysign(1.0, DyM)) Dym = 0.0;
                        } else {
                            if (copysign(1.0, DyM) == copysign(1.0, Dym)) DyM = 0.0;
                        }

                    } else if (ptr_row) {
                        if (not_ignored) {
                            int inxi = 0;
                            while (inxi < nx) {
                                fgets(report_row, MaxTextLen, f);
                                if ((strlen(report_row) > 0) && (strchr(report_row, '#') == NULL)) {
                                    if (sscanf(report_row, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &dr[inxi].x,
                                               &dr[inxi].Nx, &dr[inxi].Vy, &dr[inxi].Vz, &dr[inxi].Tx, &dr[inxi].My,
                                               &dr[inxi].Mz, &dr[inxi].Dx, &dr[inxi].Dy, &dr[inxi].Dz, &dr[inxi].Rx) ==
                                        11)
                                        inxi++;
                                }
                            }
                            for (int inx = 0; inx < inxi; inx++) {
                                double Nx_min, Nx_max, Vy_min, Vy_max, Mz_min, Mz_max, Dx_min, Dx_max, Dy_min, Dy_max, Sp_min, Sp, Sm, Ss, Sp_max, Sm_min, Sm_max, Ss_min, Ss_max;
                                double Dl, DlM, Dlm;

                                if (combi_total_numbers[i].combi > 1)  //combi
                                {
                                    COMBI_ELEMENT *combi_element;
                                    if (combi_total_numbers[i].combi == 2) combi_element = combi_element_uls;
                                    else if (combi_total_numbers[i].combi == 3) combi_element = combi_element_sls;
                                    else combi_element = combi_element_qpsls;

                                    if (combi_total_numbers[i].first ==
                                        1)  //creating min max values data for element and initializing values
                                    {
                                        COMBI_FORCES *fd = combi_element[rep_element_no - 1].fd;
                                        fd[inx].Nx_min = dr[inx].Nx;
                                        fd[inx].Nx_max = dr[inx].Nx;
                                        fd[inx].Vy_min = dr[inx].Vy;
                                        fd[inx].Vy_max = dr[inx].Vy;
                                        fd[inx].Mz_min = dr[inx].Mz;
                                        fd[inx].Mz_max = dr[inx].Mz;
                                        fd[inx].Dx_min = dr[inx].Dx;
                                        fd[inx].Dx_max = dr[inx].Dx;
                                        fd[inx].Dy_min = dr[inx].Dy;
                                        fd[inx].Dy_max = dr[inx].Dy;

                                        Nxm = dr[inx].Nx / unit_factors->F_f;
                                        Vym = dr[inx].Vy / unit_factors->F_f;
                                        Mzm = dr[inx].Mz / unit_factors->M_f;

                                        fd[inx].Sp_min = (Nxm / Ax + fabs(Mzm) / Wy) / unit_factors->S_f;
                                        fd[inx].Sp_max = (Nxm / Ax + fabs(Mzm) / Wy) / unit_factors->S_f;
                                        fd[inx].Sm_min = (Nxm / Ax - fabs(Mzm) / Wy) / unit_factors->S_f;
                                        fd[inx].Sm_max = (Nxm / Ax - fabs(Mzm) / Wy) / unit_factors->S_f;

                                    }
                                    if (combi_total_numbers[i].last == 1)  //final upgrade and freeing
                                    {
                                        COMBI_FORCES *fd = combi_element[rep_element_no - 1].fd;
                                        fd[inx].Nx_min = min(fd[inx].Nx_min, dr[inx].Nx);
                                        fd[inx].Nx_max = max(fd[inx].Nx_max, dr[inx].Nx);
                                        fd[inx].Vy_min = min(fd[inx].Vy_min, dr[inx].Vy);
                                        fd[inx].Vy_max = max(fd[inx].Vy_max, dr[inx].Vy);
                                        fd[inx].Mz_min = min(fd[inx].Mz_min, dr[inx].Mz);
                                        fd[inx].Mz_max = max(fd[inx].Mz_max, dr[inx].Mz);

                                        Dl = dr[inx].Dx * dr[inx].Dx + dr[inx].Dy * dr[inx].Dy;
                                        Dlm = fd[inx].Dx_min * fd[inx].Dx_min + fd[inx].Dy_min * fd[inx].Dy_min;
                                        DlM = fd[inx].Dx_max * fd[inx].Dx_max + fd[inx].Dy_max * fd[inx].Dy_max;
                                        if (Dl < Dlm) {
                                            fd[inx].Dx_min = dr[inx].Dx;
                                            fd[inx].Dy_min = dr[inx].Dy;
                                        }
                                        if (Dl > DlM) {
                                            fd[inx].Dx_max = dr[inx].Dx;
                                            fd[inx].Dy_max = dr[inx].Dy;
                                        }


                                        Nxm = dr[inx].Nx / unit_factors->F_f;
                                        Vym = dr[inx].Vy / unit_factors->F_f;
                                        Mzm = dr[inx].Mz / unit_factors->M_f;

                                        Sp = (Nxm / Ax + fabs(Mzm) / Wy) / unit_factors->S_f;
                                        Sm = (Nxm / Ax - fabs(Mzm) / Wy) / unit_factors->S_f;

                                        fd[inx].Sp_min = min(fd[inx].Sp_min, Sp);
                                        fd[inx].Sp_max = max(fd[inx].Sp_max, Sp);
                                        fd[inx].Sm_min = min(fd[inx].Sm_min, Sm);
                                        fd[inx].Sm_max = max(fd[inx].Sm_max, Sm);


                                        Nx_min = fd[inx].Nx_min;
                                        Nx_max = fd[inx].Nx_max;
                                        Vy_min = fd[inx].Vy_min;
                                        Vy_max = fd[inx].Vy_max;
                                        Mz_min = fd[inx].Mz_min;
                                        Mz_max = fd[inx].Mz_max;
                                        Dx_min = fd[inx].Dx_min;
                                        Dx_max = fd[inx].Dx_max;
                                        Dy_min = fd[inx].Dy_min;
                                        Dy_max = fd[inx].Dy_max;

                                        Sp_min = fd[inx].Sp_min;
                                        Sp_max = fd[inx].Sp_max;
                                        Sm_min = fd[inx].Sm_min;
                                        Sm_max = fd[inx].Sm_max;

                                        Dxm = combi_element[rep_element_no - 1].Dxm;
                                        DxM = combi_element[rep_element_no - 1].DxM;
                                        Dym = combi_element[rep_element_no - 1].Dym;
                                        DyM = combi_element[rep_element_no - 1].DyM;

                                    } else  //upgrade
                                    {
                                        COMBI_FORCES *fd = combi_element[rep_element_no - 1].fd;
                                        fd[inx].Nx_min = min(fd[inx].Nx_min, dr[inx].Nx);
                                        fd[inx].Nx_max = max(fd[inx].Nx_max, dr[inx].Nx);
                                        fd[inx].Vy_min = min(fd[inx].Vy_min, dr[inx].Vy);
                                        fd[inx].Vy_max = max(fd[inx].Vy_max, dr[inx].Vy);
                                        fd[inx].Mz_min = min(fd[inx].Mz_min, dr[inx].Mz);
                                        fd[inx].Mz_max = max(fd[inx].Mz_max, dr[inx].Mz);

                                        Dl = dr[inx].Dx * dr[inx].Dx + dr[inx].Dy * dr[inx].Dy;
                                        Dlm = fd[inx].Dx_min * fd[inx].Dx_min + fd[inx].Dy_min * fd[inx].Dy_min;
                                        DlM = fd[inx].Dx_max * fd[inx].Dx_max + fd[inx].Dy_max * fd[inx].Dy_max;
                                        if (Dl < Dlm) {
                                            fd[inx].Dx_min = dr[inx].Dx;
                                            fd[inx].Dy_min = dr[inx].Dy;
                                        }
                                        if (Dl > DlM) {
                                            fd[inx].Dx_max = dr[inx].Dx;
                                            fd[inx].Dy_max = dr[inx].Dy;
                                        }

                                        Nxm = dr[inx].Nx / unit_factors->F_f;
                                        Vym = dr[inx].Vy / unit_factors->F_f;
                                        Mzm = dr[inx].Mz / unit_factors->M_f;

                                        Sp = (Nxm / Ax + fabs(Mzm) / Wy) / unit_factors->S_f;
                                        Sm = (Nxm / Ax - fabs(Mzm) / Wy) / unit_factors->S_f;

                                        fd[inx].Sp_min = min(fd[inx].Sp_min, Sp);
                                        fd[inx].Sp_max = max(fd[inx].Sp_max, Sp);
                                        fd[inx].Sm_min = min(fd[inx].Sm_min, Sm);
                                        fd[inx].Sm_max = max(fd[inx].Sm_max, Sm);

                                    }
                                    if (combi_total_numbers[i].last ==
                                        1)  //final upgrade and freeing, and can be a sinble load in the combination
                                    {
                                        COMBI_FORCES *fd = combi_element[rep_element_no - 1].fd;
                                        fd[inx].Nx_min = min(fd[inx].Nx_min, dr[inx].Nx);
                                        fd[inx].Nx_max = max(fd[inx].Nx_max, dr[inx].Nx);
                                        fd[inx].Vy_min = min(fd[inx].Vy_min, dr[inx].Vy);
                                        fd[inx].Vy_max = max(fd[inx].Vy_max, dr[inx].Vy);
                                        fd[inx].Mz_min = min(fd[inx].Mz_min, dr[inx].Mz);
                                        fd[inx].Mz_max = max(fd[inx].Mz_max, dr[inx].Mz);

                                        Dl = dr[inx].Dx * dr[inx].Dx + dr[inx].Dy * dr[inx].Dy;
                                        Dlm = fd[inx].Dx_min * fd[inx].Dx_min + fd[inx].Dy_min * fd[inx].Dy_min;
                                        DlM = fd[inx].Dx_max * fd[inx].Dx_max + fd[inx].Dy_max * fd[inx].Dy_max;
                                        if (Dl < Dlm) {
                                            fd[inx].Dx_min = dr[inx].Dx;
                                            fd[inx].Dy_min = dr[inx].Dy;
                                        }
                                        if (Dl > DlM) {
                                            fd[inx].Dx_max = dr[inx].Dx;
                                            fd[inx].Dy_max = dr[inx].Dy;
                                        }

                                        Nxm = dr[inx].Nx / unit_factors->F_f;
                                        Vym = dr[inx].Vy / unit_factors->F_f;
                                        Mzm = dr[inx].Mz / unit_factors->M_f;

                                        Sp = (Nxm / Ax + fabs(Mzm) / Wy) / unit_factors->S_f;
                                        Sm = (Nxm / Ax - fabs(Mzm) / Wy) / unit_factors->S_f;

                                        fd[inx].Sp_min = min(fd[inx].Sp_min, Sp);
                                        fd[inx].Sp_max = max(fd[inx].Sp_max, Sp);
                                        fd[inx].Sm_min = min(fd[inx].Sm_min, Sm);
                                        fd[inx].Sm_max = max(fd[inx].Sm_max, Sm);

                                        Nx_min = fd[inx].Nx_min;
                                        Nx_max = fd[inx].Nx_max;
                                        Vy_min = fd[inx].Vy_min;
                                        Vy_max = fd[inx].Vy_max;
                                        Mz_min = fd[inx].Mz_min;
                                        Mz_max = fd[inx].Mz_max;
                                        Dx_min = fd[inx].Dx_min;
                                        Dx_max = fd[inx].Dx_max;
                                        Dy_min = fd[inx].Dy_min;
                                        Dy_max = fd[inx].Dy_max;

                                        Sp_min = fd[inx].Sp_min;
                                        Sp_max = fd[inx].Sp_max;
                                        Sm_min = fd[inx].Sm_min;
                                        Sm_max = fd[inx].Sm_max;

                                        Dxm = combi_element[rep_element_no - 1].Dxm;
                                        DxM = combi_element[rep_element_no - 1].DxM;
                                        Dym = combi_element[rep_element_no - 1].Dym;
                                        DyM = combi_element[rep_element_no - 1].DyM;

                                    }

                                }
                                if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {

                                    if (combi_total_numbers[i].combi > 1) {

                                        Dx = Dx_min;
                                        Dy = Dy_min;
                                    } else {
                                        Dx = dr[inx].Dx;
                                        Dy = dr[inx].Dy;
                                    }

                                    x = dr[inx].x / units_factor;

                                    x11 = Le.x1 + jednostkiOb(x) * koc;
                                    y11 = Le.y1 + jednostkiOb(x) * kos;

                                    x11_ = Le.x1 + jednostkiOb(x) + Dx / d_magnitude;
                                    y11_ = Le.y1 + Dy / d_magnitude;

                                    Rotate_Point(kos, koc, Le.x1, Le.y1, x11_, Le.y1, &xdsp1, &ydsp1);

                                    Rotate_Point(kos, koc, Le.x1, Le.y1, x11_, y11_, &xdsp, &ydsp);

                                    if (new_line) {

                                        Ldsp.x1 = xdsp;
                                        Ldsp.y1 = ydsp;
                                        new_line = FALSE;
                                        Ldsp_.x1 = xdsp1;
                                        Ldsp_.y1 = ydsp1;
                                        Ldsp_.x2 = Ldsp.x1;
                                        Ldsp_.y2 = Ldsp.y1;
                                        Ldsp_.blok = 1;

                                        if (!Check_if_Equal2(Dy, 0.0) &&
                                            ((Check_if_Equal2(Dy, DyM)) || (Check_if_Equal2(Dy, Dym))) &&
                                            ((st_element[rep_element_no - 1].node1r != -1) &&
                                             (st_element[rep_element_no - 1].node2r != -1))) {
                                            Dxy = sqrt(Dx * Dx + Dy * Dy);

                                            if (!Check_if_Equal2(Dxy, Dxy0)) {
                                                if (draw_label(&Ldsp_, &Le, x, 0, 0, fabs(Dy), def_precision,
                                                               FALSE)) {
                                                    Ldsp_.typ = 68;
                                                    ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                    Ldsp_.typ = 64;
                                                }
                                            }
                                            Dxy0 = Dxy;
                                        }
                                    } else {

                                        Ldsp.x2 = xdsp;
                                        Ldsp.y2 = ydsp;
                                        Ldsp.blok = 1;

                                        Ldsp_.x1 = xdsp1;
                                        Ldsp_.y1 = ydsp1;
                                        Ldsp_.x2 = Ldsp.x2;
                                        Ldsp_.y2 = Ldsp.y2;
                                        Ldsp_.blok = 1;

                                        if (!Check_if_Equal2(Dy, 0.0) &&
                                            ((Check_if_Equal2(Dy, DyM)) || (Check_if_Equal2(Dy, Dym))) &&
                                            ((st_element[rep_element_no - 1].node1r != -1) &&
                                             (st_element[rep_element_no - 1].node2r != -1))) {
                                            Dxy = sqrt(Dx * Dx + Dy * Dy);

                                            if (!Check_if_Equal2(Dxy, Dxy0)) {
                                                if (draw_label(&Ldsp_, &Le, x, 0, 0, fabs(Dy) /*Dxy*/, def_precision,
                                                               FALSE)) {
                                                    Ldsp_.typ = 68;
                                                    ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                    Ldsp_.typ = 64;
                                                }
                                            }
                                            Dxy0 = Dxy;
                                        }

                                        if (dodaj_obiekt((BLOK *) dane, (void *) &Ldsp) == NULL) {
                                            fclose(f);
                                            ret = ask_question(1, "", (char *) confirm, "",
                                                               (char *) _CANNOT_CREATE_DEFORMATION_BLOCK_, 12,
                                                               "",
                                                               11,
                                                               1, 62);

                                            no_error=FALSE;
                                            goto error;
                                        }

                                        Ldsp.x1 = Ldsp.x2;
                                        Ldsp.y1 = Ldsp.y2;
                                    }

                                    if ((combi_total_numbers[i].combi > 1))
                                    {

                                        Dx = Dx_max;
                                        Dy = Dy_max;


                                        x11 = Le.x1 + jednostkiOb(x) * koc;
                                        y11 = Le.y1 + jednostkiOb(x) * kos;

                                        x11_ = Le.x1 + jednostkiOb(x) + Dx / d_magnitude;
                                        y11_ = Le.y1 + Dy / d_magnitude;

                                        Rotate_Point(kos, koc, Le.x1, Le.y1, x11_, Le.y1, &xdsp1, &ydsp1);

                                        Rotate_Point(kos, koc, Le.x1, Le.y1, x11_, y11_, &xdsp, &ydsp);

                                        if (new_line1) {

                                            Ldsp1.x1 = xdsp;
                                            Ldsp1.y1 = ydsp;
                                            new_line1 = FALSE;
                                            Ldsp_.x1 = xdsp1;
                                            Ldsp_.y1 = ydsp1;
                                            Ldsp_.x2 = Ldsp1.x1;
                                            Ldsp_.y2 = Ldsp1.y1;
                                            Ldsp_.blok = 1;

                                            if (!Check_if_Equal2(Dy, 0.0) &&
                                                ((Check_if_Equal2(Dy, DyM)) || (Check_if_Equal2(Dy, Dym))) &&
                                                ((st_element[rep_element_no - 1].node1r != -1) &&
                                                 (st_element[rep_element_no - 1].node2r != -1))) {
                                                Dxy = sqrt(Dx * Dx + Dy * Dy);

                                                if (!Check_if_Equal2(Dxy, Dxy01)) {
                                                    if (draw_label(&Ldsp_, &Le, x, 0, 0, fabs(Dy), def_precision,
                                                                   FALSE)) {
                                                        Ldsp_.typ = 68;
                                                        ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                        Ldsp_.typ = 64;
                                                    }
                                                }
                                                Dxy01 = Dxy;
                                            }
                                        } else {

                                            Ldsp1.x2 = xdsp;
                                            Ldsp1.y2 = ydsp;
                                            Ldsp1.blok = 1;

                                            Ldsp_.x1 = xdsp1;
                                            Ldsp_.y1 = ydsp1;
                                            Ldsp_.x2 = Ldsp1.x2;
                                            Ldsp_.y2 = Ldsp1.y2;
                                            Ldsp_.blok = 1;

                                            if (!Check_if_Equal2(Dy, 0.0) &&
                                                ((Check_if_Equal2(Dy, DyM)) || (Check_if_Equal2(Dy, Dym))) &&
                                                ((st_element[rep_element_no - 1].node1r != -1) &&
                                                 (st_element[rep_element_no - 1].node2r != -1))) {
                                                Dxy = sqrt(Dx * Dx + Dy * Dy);

                                                if (!Check_if_Equal2(Dxy, Dxy01)) {
                                                    if (draw_label(&Ldsp_, &Le, x, 0, 0, fabs(Dy) ,
                                                                   def_precision,
                                                                   FALSE)) {
                                                        Ldsp_.typ = 68;
                                                        ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                        Ldsp_.typ = 64;
                                                    }
                                                }
                                                Dxy01 = Dxy;
                                            }

                                            if (dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1) == NULL) {
                                                fclose(f);
                                                ret = ask_question(1, "", (char *) confirm, "",
                                                                   (char *) _CANNOT_CREATE_DEFORMATION_BLOCK_, 12,
                                                                   "",
                                                                   11,
                                                                   1, 62);

                                                no_error=FALSE;
                                                goto error;
                                            }

                                            Ldsp1.x1 = Ldsp1.x2;
                                            Ldsp1.y1 = Ldsp1.y2;
                                        }
                                    }

                                }
                            }
                        }
                        free(cfp);
                        free(cfm);
                        free(dr);
                    }
                    if (rout) printf("%s", report_row);
                }
                fclose(f);
            }

            //Nx
            Ldsp.kolor = static_colors.compression_color; //1 red or 5 blue

            for (i = 0; i < combinations_number; i++)   //st_layer_no
            {
                //for combinations all is already set in the buffer, so it's not necessary to read it again for building blocks
                if ((combi_total_numbers[i].combi > 1) && (combi_total_numbers[i].last != 1)) continue;
                /////////////////////

                il = combi_total_numbers[i].case_no;

                if (combi_total_numbers[i].combi == 0) {
                    strcpy(SLS_ULS, "sls");
                    strcpy(SLS_ULS_BAK, "case");
                    strcpy(SLS_ULS_BLK, "sls");
                }
                else if (combi_total_numbers[i].combi == 1) {
                    strcpy(SLS_ULS, "ULS");
                    strcpy(SLS_ULS_BAK, "ULS");
                    strcpy(SLS_ULS_BLK, "ULS");
                }
                else {
                    if (combi_total_numbers[i].combi == 2) {
                        strcpy(SLS_ULS, "ULSLC");
                        strcpy(SLS_ULS_BAK, "ULSLC");
                        strcpy(SLS_ULS_BLK, "ULSLC");
                    }
                    else if (combi_total_numbers[i].combi == 3) {
                        strcpy(SLS_ULS, "slsLC");
                        strcpy(SLS_ULS_BAK, "slsLC");
                        strcpy(SLS_ULS_BLK, "slsLC");
                    }
                    else if (combi_total_numbers[i].combi == 4) {
                        if (qpslsLC_Layer) {
                            strcpy(SLS_ULS, "qpslsLC");
                            strcpy(SLS_ULS_BAK, "qpslsLC");
                            strcpy(SLS_ULS_BLK, "qpslsLC");
                        }
                        else {
                            strcpy(SLS_ULS, "slsLC");
                            strcpy(SLS_ULS_BAK, "slsLC");
                            strcpy(SLS_ULS_BLK, "qpslsLC");
                        }
                    }
                }

                sprintf(desired_layer_bak, "%s_%s_%s_%d", ptr_id, "axial_forces", SLS_ULS_BAK, combination_no);
                sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "axial_forces", SLS_ULS, combination_no);
                sprintf(block_name, "%s$N_%s_%d_%d", ptr_id_short, SLS_ULS_BLK, combination_no,
                        combi_total_numbers[i].combination);

                hiding = (combi_total_numbers[i].combi != 2);  //ULSLC only

                for (ii = 0; ii < No_Layers; ii++) {
                    if (strcmp(Layers[ii].name, desired_layer_bak) == 0) {
                        strcpy(Layers[ii].name, desired_layer);
                        break;
                    }
                    if (strcmp(Layers[ii].name, desired_layer) == 0)
                        break;
                }
                if (ii < No_Layers) {
                    desired_layer_no = ii;
                    if (((combi_total_numbers[i].first) || (combi_total_numbers[i].last)) &&
                        ((combi_total_numbers[i].combi != 4) || (qpslsLC_Layer == TRUE)))
                        delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
                } else {
                    if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {
                        ret = ask_question(1, "", (char *) confirm, "",
                                           (char *) _CANNOT_CREATE_NEW_LAYER_, 12, "", 11,
                                           1, 62);
                        no_error=FALSE;
                        goto error;
                    }
                    if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                        No_Layers++;
                        desired_layer_no = No_Layers - 1;
                        Layers[No_Layers - 1].on = 1;
                        Layers[No_Layers - 1].edit = 1;
                        Layers[No_Layers - 1].point = 1;
                        strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
                        Layers[No_Layers - 1].color = Ldsp.kolor;
                    }
                }

                Ldsp.warstwa = desired_layer_no;
                /////////////////////
                memmove(&Ldsp1, &Ldsp, sizeof(LINIA));
                ////////////////////
                memmove(&Ldsp_, &Ldsp, sizeof(LINIA));

                sprintf(params, "alfacad.if%02d", i + 1);
                f = fopen(params, "rt");
                if (f == NULL) {

                    ret = ask_question(1, "", (char *) confirm, "",
                                       (char *) _CANNOT_OPEN_RESULTS_DATA_FILE_, 12, "", 11,
                                       1, 62);
                    no_error=FALSE;
                    goto error;
                }

                ////sprintf(params, "$N_%dof%d", i + 1, st_layer_no);
                if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                    ptr_block = add_block(st_node[0].x, st_node[0].y, 'E', block_name, hiding);
                    if (ptr_block == NULL) {
                        fclose(f);
                        ret = ask_question(1, "", (char *) confirm, "",
                                           (char *) _CANNOT_CREATE_FORCE_BLOCK_, 12, "", 11, 1, 62);
                        no_error=FALSE;
                        goto error;
                    }
                }

                new_line = TRUE;
                new_line1 = TRUE;

                while (fgets(report_row, MaxTextLen, f)) {
                    double x, Nx, Vy, Vz, Tx, My, Mz, Dx, Dy, Dz, Rx;
                    ptr = strstr(report_row, "# @");
                    ptr_max = strstr(report_row, "# MAXIMUM");
                    ptr_min = strstr(report_row, "# MINIMUM");
                    ptr_row = strstr(report_row, "#.x");
                    if (ptr) {
                        ret = sscanf(ptr + 4, "%d %d %d %lf %lf %lf %lf %lf %lf %d", &rep_element_no, &N1, &N2, &X1,
                                     &Y1, &Z1, &X2, &Y2, &Z2, &nx);

                        dr = (DATA_ROW *) malloc(nx * sizeof(DATA_ROW));

                        n1 = st_element[rep_element_no - 1].node1;
                        n2 = st_element[rep_element_no - 1].node2;
                        Le.x1 = st_node[n1].x;
                        Le.y1 = st_node[n1].y;
                        Le.x2 = st_node[n2].x;
                        Le.y2 = st_node[n2].y;
                        parametry_lini(&Le, &PL);
                        kos = sin(PL.kat * Pi / 180);
                        koc = cos(PL.kat * Pi / 180);

                        kos1 = sin(Angle_Normal((PL.kat + 90) * Pi / 180));
                        koc1 = cos(Angle_Normal((PL.kat + 90) * Pi / 180));

                        Dxy0 = 0;
                        Nx0 = 0;
                        Vy0 = 0;
                        Mz0 = 0;
                        Dxy01 = 0;
                        Nx01 = 0;
                        Vy01 = 0;
                        Mz01 = 0;
                        strcpy(T_text, "");
                        new_line = TRUE;
                        new_line1 = TRUE;

                        //nxi=0;
                    } else if (ptr_max) {
                        ret = sscanf(ptr_max + 10, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &NxM, &VyM, &VzM,
                                     &TxM, &MyM, &MzM, &DxM, &DyM, &DzM, &RxM);

                        NxM /= unit_factors->F_f;
                        VyM /= unit_factors->F_f;
                        MzM /= unit_factors->M_f;

                    } else if (ptr_min) {
                        ret = sscanf(ptr_min + 10, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &Nxm, &Vym, &Vzm,
                                     &Txm, &Mym, &Mzm, &Dxm, &Dym, &Dzm, &Rxm);

                        Nxm /= unit_factors->F_f;
                        Vym /= unit_factors->F_f;
                        Mzm /= unit_factors->M_f;

                    } else if (ptr_row) {
                        if (not_ignored) {
                            int inxi = 0;
                            while (inxi < nx) {
                                fgets(report_row, MaxTextLen, f);
                                if ((strlen(report_row) > 0) && (strchr(report_row, '#') == NULL)) {
                                    if (sscanf(report_row, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &dr[inxi].x,
                                               &dr[inxi].Nx, &dr[inxi].Vy, &dr[inxi].Vz, &dr[inxi].Tx, &dr[inxi].My,
                                               &dr[inxi].Mz, &dr[inxi].Dx, &dr[inxi].Dy, &dr[inxi].Dz, &dr[inxi].Rx) ==
                                        11)
                                        inxi++;
                                }
                            }

                            for (int inx = 0; inx < inxi; inx++) {
                                double Nx_min, Nx_max, Vy_min, Vy_max, Mz_min, Mz_max, Dx_min, Dx_max, Dy_min, Dy_max;

                                if (combi_total_numbers[i].combi > 1)  //combi
                                {
                                    COMBI_ELEMENT *combi_element;
                                    if (combi_total_numbers[i].combi == 2) combi_element = combi_element_uls;
                                    else if (combi_total_numbers[i].combi == 3) combi_element = combi_element_sls;
                                    else combi_element = combi_element_qpsls;

                                    COMBI_FORCES *fd = combi_element[rep_element_no - 1].fd;

                                    Nx_min = fd[inx].Nx_min;
                                    Nx_max = fd[inx].Nx_max;
                                    Vy_min = fd[inx].Vy_min;
                                    Vy_max = fd[inx].Vy_max;
                                    Mz_min = fd[inx].Mz_min;
                                    Mz_max = fd[inx].Mz_max;
                                    Dx_min = fd[inx].Dx_min;
                                    Dx_max = fd[inx].Dx_max;
                                    Dy_min = fd[inx].Dy_min;
                                    Dy_max = fd[inx].Dy_max;

                                    Nxm = combi_element[rep_element_no - 1].Nxm / unit_factors->F_f;
                                    NxM = combi_element[rep_element_no - 1].NxM / unit_factors->F_f;

                                    if (copysign(1.0, Nx_min) == copysign(1.0, Nx_max)) {
                                        if (fabs(Nx_min) < fabs(Nx_max)) Nx_min = 0;
                                        else Nx_max = 0;
                                    }

                                }
                                if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                                    if (((st_element[rep_element_no - 1].node1r != -1) &&
                                         (st_element[rep_element_no - 1].node2r != -1)) ||
                                        (st_node[st_element[rep_element_no - 1].node1].radius > 0) ||
                                        (st_node[st_element[rep_element_no - 1].node2].radius > 0)) {
                                        if (combi_total_numbers[i].combi > 1) {
                                            Nx = Nx_min;
                                        } else Nx = dr[inx].Nx;

                                        Nx /= unit_factors->F_f;

                                        x = dr[inx].x / units_factor;

                                        xd = jednostkiOb(x);
                                        xdmin = jednostkiOb(axis_increment / units_factor);

                                        if (Nx >= 0)
                                            Ldsp.kolor = static_colors.tension_color;
                                        else Ldsp.kolor = static_colors.compression_color;   // positive are tension, red 1, negative are compression, blue 5,
                                        Ldsp_.kolor = Ldsp.kolor;
                                        x11 = Le.x1 + jednostkiOb(x) * koc;
                                        y11 = Le.y1 + jednostkiOb(x) * kos;

                                        Rotate_Point(kos1, koc1, x11, y11, x11 + Nx / n_magnitude, y11, &xdsp, &ydsp);

                                        if (new_line) {

                                            Ldsp.x1 = xdsp;
                                            Ldsp.y1 = ydsp;
                                            new_line = FALSE;
                                            Ldsp_.x1 = x11;
                                            Ldsp_.y1 = y11;
                                            Ldsp_.x2 = Ldsp.x1;
                                            Ldsp_.y2 = Ldsp.y1;
                                            Ldsp_.blok = 1;
                                            ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                            if (check_if_not_virtual(x, PL, rep_element_no)) {
                                                if (!Check_if_Equal(Nx, 0.0) &&
                                                    ((Check_if_Equal(Nx, NxM)) || (Check_if_Equal(Nx, Nxm)))) {
                                                    if (Check_if_Equal(NxM, Nxm)) {
                                                        if ((PL.dl * 2 / 3 - xd) < xdmin)
                                                            draw_label(&Ldsp_, &Le, x, 0, 0, Nx, force_precision,
                                                                       FALSE);
                                                    } else {
                                                        if (!Check_if_Equal(Nx, Nx0))
                                                            draw_label(&Ldsp_, &Le, x, 0, 0, Nx, force_precision,
                                                                       FALSE);
                                                    }
                                                    Nx0 = Nx;
                                                }
                                            }
                                        } else {

                                            Ldsp.x2 = xdsp;
                                            Ldsp.y2 = ydsp;
                                            Ldsp.blok = 1;

                                            Ldsp_.x1 = x11;
                                            Ldsp_.y1 = y11;
                                            Ldsp_.x2 = Ldsp.x2;
                                            Ldsp_.y2 = Ldsp.y2;
                                            Ldsp_.blok = 1;
                                            ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                            if (check_if_not_virtual(x, PL, rep_element_no)) {
                                                if (!Check_if_Equal(Nx, 0.0) &&
                                                    ((Check_if_Equal(Nx, NxM)) || (Check_if_Equal(Nx, Nxm)))) {
                                                    if (Check_if_Equal(NxM, Nxm)) {
                                                        if ((PL.dl * 2 / 3 - xd) < xdmin)
                                                            draw_label(&Ldsp_, &Le, x, 0, 0, Nx, force_precision,
                                                                       FALSE);
                                                    } else {
                                                        if (!Check_if_Equal(Nx, Nx0))
                                                            draw_label(&Ldsp_, &Le, x, 0, 0, Nx, force_precision,
                                                                       FALSE);
                                                    }
                                                    Nx0 = Nx;
                                                }
                                            }

                                            if (dodaj_obiekt((BLOK *) dane, (void *) &Ldsp) == NULL) {
                                                fclose(f);
                                                ret = ask_question(1, "", (char *) confirm, "",
                                                                   (char *) _CANNOT_CREATE_FORCE_BLOCK_, 12, "", 11, 1,
                                                                   62);

                                                no_error=FALSE;
                                                goto error;
                                            }

                                            Ldsp.x1 = Ldsp.x2;
                                            Ldsp.y1 = Ldsp.y2;
                                        }

                                        if ((combi_total_numbers[i].combi > 1)) // & (SECOND_LINE))
                                        {

                                            Nx = Nx_max;

                                            Nx /= unit_factors->F_f;

                                            xd = jednostkiOb(x);
                                            xdmin = jednostkiOb(axis_increment / units_factor);

                                            if (Nx >= 0)
                                                Ldsp1.kolor = static_colors.tension_color;
                                            else Ldsp1.kolor = static_colors.compression_color;   // positive are tension, red 1, negative are compression, blue 5,
                                            Ldsp_.kolor = Ldsp1.kolor;
                                            x11 = Le.x1 + jednostkiOb(x) * koc;
                                            y11 = Le.y1 + jednostkiOb(x) * kos;

                                            Rotate_Point(kos1, koc1, x11, y11, x11 + Nx / n_magnitude, y11, &xdsp,
                                                         &ydsp);

                                            if (new_line1) {

                                                Ldsp1.x1 = xdsp;
                                                Ldsp1.y1 = ydsp;
                                                new_line1 = FALSE;
                                                Ldsp_.x1 = x11;
                                                Ldsp_.y1 = y11;
                                                Ldsp_.x2 = Ldsp1.x1;
                                                Ldsp_.y2 = Ldsp1.y1;
                                                Ldsp_.blok = 1;
                                                ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                if (check_if_not_virtual(x, PL, rep_element_no)) {
                                                    if (!Check_if_Equal(Nx, 0.0) &&
                                                        ((Check_if_Equal(Nx, NxM)) || (Check_if_Equal(Nx, Nxm)))) {
                                                        if (Check_if_Equal(NxM, Nxm)) {
                                                            if ((PL.dl * 2 / 3 - xd) < xdmin)
                                                                draw_label(&Ldsp_, &Le, x, 0, 0, Nx, force_precision,
                                                                           FALSE);
                                                        } else {
                                                            if (!Check_if_Equal(Nx, Nx01))
                                                                draw_label(&Ldsp_, &Le, x, 0, 0, Nx, force_precision,
                                                                           FALSE);
                                                        }
                                                        Nx01 = Nx;
                                                    }
                                                }
                                            } else {

                                                Ldsp1.x2 = xdsp;
                                                Ldsp1.y2 = ydsp;
                                                Ldsp1.blok = 1;

                                                Ldsp_.x1 = x11;
                                                Ldsp_.y1 = y11;
                                                Ldsp_.x2 = Ldsp1.x2;
                                                Ldsp_.y2 = Ldsp1.y2;
                                                Ldsp_.blok = 1;
                                                ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                if (check_if_not_virtual(x, PL, rep_element_no)) {
                                                    if (!Check_if_Equal(Nx, 0.0) &&
                                                        ((Check_if_Equal(Nx, NxM)) || (Check_if_Equal(Nx, Nxm)))) {
                                                        if (Check_if_Equal(NxM, Nxm)) {
                                                            if ((PL.dl * 2 / 3 - xd) < xdmin)
                                                                draw_label(&Ldsp_, &Le, x, 0, 0, Nx, force_precision,
                                                                           FALSE);
                                                        } else {
                                                            if (!Check_if_Equal(Nx, Nx01))
                                                                draw_label(&Ldsp_, &Le, x, 0, 0, Nx, force_precision,
                                                                           FALSE);
                                                        }
                                                        Nx01 = Nx;
                                                    }
                                                }

                                                if (dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1) == NULL) {
                                                    fclose(f);
                                                    ret = ask_question(1, "", (char *) confirm, "",
                                                                       (char *) _CANNOT_CREATE_FORCE_BLOCK_, 12, "", 11,
                                                                       1,
                                                                       62);

                                                    no_error=FALSE;
                                                    goto error;
                                                }

                                                Ldsp1.x1 = Ldsp1.x2;
                                                Ldsp1.y1 = Ldsp1.y2;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        free(dr);
                    }
                    if (rout) printf("%s", report_row);
                }
                fclose(f);
            }
            //Vy
            Ldsp.kolor = static_colors.shear_color;

            for (i = 0; i < combinations_number; i++)   //st_layer_no
            {
                /////////////////////
                //for combinations all is already set in the buffer, so it's not necessary to read it again for building blocks
                if ((combi_total_numbers[i].combi > 1) && (combi_total_numbers[i].last != 1)) continue;

                il = combi_total_numbers[i].case_no;
                if (combi_total_numbers[i].combi == 0) {
                    strcpy(SLS_ULS, "sls");
                    strcpy(SLS_ULS_BAK, "case");
                    strcpy(SLS_ULS_BLK, "sls");
                }
                else if (combi_total_numbers[i].combi == 1) {
                    strcpy(SLS_ULS, "ULS");
                    strcpy(SLS_ULS_BAK, "ULS");
                    strcpy(SLS_ULS_BLK, "ULS");
                }
                else {
                    if (combi_total_numbers[i].combi == 2) {
                        strcpy(SLS_ULS, "ULSLC");
                        strcpy(SLS_ULS_BAK, "ULSLC");
                        strcpy(SLS_ULS_BLK, "ULSLC");
                    }
                    else if (combi_total_numbers[i].combi == 3) {
                        strcpy(SLS_ULS, "slsLC");
                        strcpy(SLS_ULS_BAK, "slsLC");
                        strcpy(SLS_ULS_BLK, "slsLC");
                    }
                    else if (combi_total_numbers[i].combi == 4) {
                        if (qpslsLC_Layer) {
                            strcpy(SLS_ULS, "qpslsLC");
                            strcpy(SLS_ULS_BAK, "qpslsLC");
                            strcpy(SLS_ULS_BLK, "qpslsLC");
                        }
                        else {
                            strcpy(SLS_ULS, "slsLC");
                            strcpy(SLS_ULS_BAK, "slsLC");
                            strcpy(SLS_ULS_BLK, "qpslsLC");
                        }
                    }
                }

                sprintf(desired_layer_bak, "%s_%s_%s_%d", ptr_id, "cutting_forces", SLS_ULS_BAK, combination_no);
                sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "cutting_forces", SLS_ULS, combination_no);
                sprintf(block_name, "%s$V_%s_%d_%d", ptr_id_short, SLS_ULS_BLK, combination_no,
                        combi_total_numbers[i].combination);

                hiding = (combi_total_numbers[i].combi != 2);  //ULSLC only

                for (ii = 0; ii < No_Layers; ii++) {
                    if (strcmp(Layers[ii].name, desired_layer_bak) == 0) {
                        strcpy(Layers[ii].name, desired_layer);
                        break;
                    }
                    if (strcmp(Layers[ii].name, desired_layer) == 0)
                        break;
                }
                if (ii < No_Layers) {
                    desired_layer_no = ii;
                    //if ((combi_total_numbers[i].first) || (combi_total_numbers[i].last))
                    if (((combi_total_numbers[i].first) || (combi_total_numbers[i].last)) &&
                        ((combi_total_numbers[i].combi != 4) || (qpslsLC_Layer == TRUE)))
                        delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
                } else {
                    if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {
                        ret = ask_question(1, "", (char *) confirm, "",
                                           (char *) _CANNOT_CREATE_NEW_LAYER_, 12, "", 11,
                                           1, 62);
                        no_error=FALSE;
                        goto error;
                    }
                    if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                        No_Layers++;
                        desired_layer_no = No_Layers - 1;
                        Layers[No_Layers - 1].on = 1;
                        Layers[No_Layers - 1].edit = 1;
                        Layers[No_Layers - 1].point = 1;
                        strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
                        Layers[No_Layers - 1].color = Ldsp.kolor;
                    }
                }

                Ldsp.warstwa = desired_layer_no;
                /////////////////////
                memmove(&Ldsp1, &Ldsp, sizeof(LINIA));
                ////////////////////
                memmove(&Ldsp_, &Ldsp, sizeof(LINIA));


                sprintf(params, "alfacad.if%02d", i + 1);
                f = fopen(params, "rt");
                if (f == NULL) {
                    ret = ask_question(1, "", (char *) confirm, "",
                                       (char *) _CANNOT_OPEN_RESULTS_DATA_FILE_, 12, "", 11,
                                       1, 62);
                    no_error=FALSE;
                    goto error;
                }

                if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                    ptr_block = add_block(st_node[0].x, st_node[0].y, 'E', block_name, hiding);
                    if (ptr_block == NULL) {
                        fclose(f);
                        ret = ask_question(1, "", (char *) confirm, "",
                                           (char *) _CANNOT_CREATE_FORCE_BLOCK_, 12, "", 11, 1, 62);
                        no_error=FALSE;
                        goto error;
                    }
                }

                new_line = TRUE;
                new_line1 = TRUE;

                while (fgets(report_row, MaxTextLen, f)) {
                    double x, Nx, Vy, Vz, Tx, My, Mz, Dx, Dy, Dz, Rx;
                    ptr = strstr(report_row, "# @");
                    ptr_max = strstr(report_row, "# MAXIMUM");
                    ptr_min = strstr(report_row, "# MINIMUM");
                    ptr_row = strstr(report_row, "#.x");
                    if (ptr) {
                        ret = sscanf(ptr + 4, "%d %d %d %lf %lf %lf %lf %lf %lf %d", &rep_element_no, &N1, &N2, &X1,
                                     &Y1, &Z1, &X2, &Y2, &Z2, &nx);

                        dr = (DATA_ROW *) malloc(nx * sizeof(DATA_ROW));
                        cfp = (int *) malloc(nx * sizeof(int));
                        cfm = (int *) malloc(nx * sizeof(int));

                        n1 = st_element[rep_element_no - 1].node1;
                        n2 = st_element[rep_element_no - 1].node2;
                        Le.x1 = st_node[n1].x;
                        Le.y1 = st_node[n1].y;
                        Le.x2 = st_node[n2].x;
                        Le.y2 = st_node[n2].y;
                        parametry_lini(&Le, &PL);
                        kos = sin(PL.kat * Pi / 180);
                        koc = cos(PL.kat * Pi / 180);

                        kos1 = sin(Angle_Normal((PL.kat - 90) * Pi / 180));
                        koc1 = cos(Angle_Normal((PL.kat - 90) * Pi / 180));

                        Dxy0 = 0;
                        Nx0 = 0;
                        Vy0 = 0;
                        Mz0 = 0;
                        Dxy01 = 0;
                        Nx01 = 0;
                        Vy01 = 0;
                        Mz01 = 0;
                        strcpy(T_text, "");
                        new_line = TRUE;
                        new_line1 = TRUE;

                    } else if (ptr_max) {
                        ret = sscanf(ptr_max + 10, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &NxM, &VyM, &VzM,
                                     &TxM, &MyM, &MzM, &DxM, &DyM, &DzM, &RxM);
                        NxM /= unit_factors->F_f;
                        VyM /= unit_factors->F_f;
                        MzM /= unit_factors->M_f;

                    } else if (ptr_min) {
                        ret = sscanf(ptr_min + 10, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &Nxm, &Vym, &Vzm,
                                     &Txm, &Mym, &Mzm, &Dxm, &Dym, &Dzm, &Rxm);
                        Nxm /= unit_factors->F_f;
                        Vym /= unit_factors->F_f;
                        Mzm /= unit_factors->M_f;

                    } else if (ptr_row) {
                        if (not_ignored) {
                            int inxi = 0;
                            while (inxi < nx) {
                                fgets(report_row, MaxTextLen, f);
                                if ((strlen(report_row) > 0) && (strchr(report_row, '#') == NULL)) {
                                    if (sscanf(report_row, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &dr[inxi].x,
                                               &dr[inxi].Nx, &dr[inxi].Vy, &dr[inxi].Vz, &dr[inxi].Tx, &dr[inxi].My,
                                               &dr[inxi].Mz, &dr[inxi].Dx, &dr[inxi].Dy, &dr[inxi].Dz, &dr[inxi].Rx) ==
                                        11)
                                        inxi++;
                                }
                            }

                            if (combi_total_numbers[i].combi > 1)  //combi
                            {
                                COMBI_ELEMENT *combi_element;
                                if (combi_total_numbers[i].combi == 2) combi_element = combi_element_uls;
                                else if (combi_total_numbers[i].combi == 3) combi_element = combi_element_sls;
                                else combi_element = combi_element_qpsls;

                                COMBI_FORCES *fd = combi_element[rep_element_no - 1].fd;

                                double Mz_min, Mz_max;

                                for (int k = 0; k < nx; k++) cfp[k] = cfm[k] = 0;
                                for (int k = 1; k < nx - 1; k++) {
                                    if (fd[k].Vy_max > fd[k - 1].Vy_max && fd[k].Vy_max > fd[k + 1].Vy_max) cfp[k] = 1;
                                    if (fd[k].Vy_min < fd[k - 1].Vy_min && fd[k].Vy_min < fd[k + 1].Vy_min) cfm[k] = 1;
                                }
                            } else//uls or sls
                            {
                                for (int k = 0; k < nx; k++) cfp[k] = cfm[k] = 0;
                                for (int k = 1; k < nx - 1; k++) {
                                    if (fabs(dr[k].Vy) > fabs(dr[k - 1].Vy) && fabs(dr[k].Vy) > fabs(dr[k + 1].Vy))
                                        cfm[k] = 1;
                                }
                            }

                            for (int inx = 0; inx < inxi; inx++) {
                                double Nx_min, Nx_max, Vy_min, Vy_max, Mz_min, Mz_max, Dx_min, Dx_max, Dy_min, Dy_max;

                                if (combi_total_numbers[i].combi > 1)  //combi
                                {
                                    COMBI_ELEMENT *combi_element;
                                    if (combi_total_numbers[i].combi == 2) combi_element = combi_element_uls;
                                    else if (combi_total_numbers[i].combi == 3) combi_element = combi_element_sls;
                                    else combi_element = combi_element_qpsls;

                                    COMBI_FORCES *fd = combi_element[rep_element_no - 1].fd;

                                    Nx_min = fd[inx].Nx_min;
                                    Nx_max = fd[inx].Nx_max;
                                    Vy_min = fd[inx].Vy_min;
                                    Vy_max = fd[inx].Vy_max;
                                    Mz_min = fd[inx].Mz_min;
                                    Mz_max = fd[inx].Mz_max;
                                    Dx_min = fd[inx].Dx_min;
                                    Dx_max = fd[inx].Dx_max;
                                    Dy_min = fd[inx].Dy_min;
                                    Dy_max = fd[inx].Dy_max;

                                    Vym = combi_element[rep_element_no - 1].Vym / unit_factors->F_f;
                                    VyM = combi_element[rep_element_no - 1].VyM / unit_factors->F_f;

                                    if (copysign(1.0, Vy_min) == copysign(1.0, Vy_max)) {
                                        if (fabs(Vy_min) < fabs(Vy_max)) Vy_min = 0;
                                        else Vy_max = 0;
                                    }

                                }
                                if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                                    if (((st_element[rep_element_no - 1].node1r != -1) &&
                                         (st_element[rep_element_no - 1].node2r != -1)) ||
                                        (st_node[st_element[rep_element_no - 1].node1].radius > 0) ||
                                        (st_node[st_element[rep_element_no - 1].node2].radius > 0)) {
                                        if (combi_total_numbers[i].combi > 1) {
                                            Vy = Vy_min;
                                        } else Vy = dr[inx].Vy;

                                        Vy /= unit_factors->F_f;

                                        x = dr[inx].x;

                                        x /= units_factor;

                                        xd = jednostkiOb(x);
                                        xdmin = jednostkiOb(axis_increment / units_factor);

                                        x11 = Le.x1 + jednostkiOb(x) * koc;
                                        y11 = Le.y1 + jednostkiOb(x) * kos;


                                        Rotate_Point(kos1, koc1, x11, y11, x11 + Vy / v_magnitude, y11, &xdsp, &ydsp);


                                        if (new_line) {

                                            Ldsp.x1 = xdsp;
                                            Ldsp.y1 = ydsp;
                                            Ldsp.blok = 1;
                                            new_line = FALSE;
                                            Ldsp_.x1 = x11;
                                            Ldsp_.y1 = y11;
                                            Ldsp_.x2 = Ldsp.x1;
                                            Ldsp_.y2 = Ldsp.y1;
                                            Ldsp_.blok = 1;
                                            ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                            if (check_if_not_virtual(x, PL, rep_element_no)) {
                                                if (!Check_if_Equal(Vy, 0.0) &&
                                                    ((Check_if_Equal(Vy, VyM) || Check_if_Equal(Vy, Vym)) ||
                                                     (Check_if_Equal(x, 0.0) ||
                                                      Check_if_Equal(jednostkiOb(x), PL.dl)) || cfm[inx])) {
                                                    if (Check_if_Equal(VyM, Vym)) {
                                                        if ((PL.dl * 3 / 5 - xd) < xdmin)
                                                            draw_label(&Ldsp_, &Le, x, 0, 0, Vy, force_precision,
                                                                       FALSE);
                                                    } else {
                                                        if (!Check_if_Equal(Vy, Vy0))
                                                            draw_label(&Ldsp_, &Le, x, 0, 0, Vy, force_precision,
                                                                       FALSE);
                                                    }
                                                    Vy0 = Vy;
                                                }
                                            }
                                        } else {

                                            Ldsp.x2 = xdsp;
                                            Ldsp.y2 = ydsp;
                                            Ldsp.blok = 1;

                                            Ldsp_.x1 = x11;
                                            Ldsp_.y1 = y11;
                                            Ldsp_.x2 = Ldsp.x2;
                                            Ldsp_.y2 = Ldsp.y2;
                                            Ldsp_.blok = 1;
                                            ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                            if (check_if_not_virtual(x, PL, rep_element_no)) {
                                                if (!Check_if_Equal(Vy, 0.0) &&
                                                    ((Check_if_Equal(Vy, VyM) || Check_if_Equal(Vy, Vym)) ||
                                                     (Check_if_Equal(x, 0.0) ||
                                                      Check_if_Equal(jednostkiOb(x), PL.dl)) || cfm[inx])) {
                                                    if (Check_if_Equal(VyM, Vym)) {
                                                        if ((PL.dl * 3 / 5 - xd) < xdmin)
                                                            draw_label(&Ldsp_, &Le, x, 0, 0, Vy, force_precision,
                                                                       FALSE);
                                                    } else {
                                                        if (!Check_if_Equal(Vy, Vy0))
                                                            draw_label(&Ldsp_, &Le, x, 0, 0, Vy, force_precision,
                                                                       FALSE);
                                                    }
                                                    Vy0 = Vy;
                                                }
                                            }

                                            if (dodaj_obiekt((BLOK *) dane, (void *) &Ldsp) == NULL) {
                                                fclose(f);
                                                ret = ask_question(1, "", (char *) confirm, "",
                                                                   (char *) _CANNOT_CREATE_FORCE_BLOCK_, 12, "", 11, 1,
                                                                   62);

                                                no_error=FALSE;
                                                goto error;
                                            }

                                            Ldsp.x1 = Ldsp.x2;
                                            Ldsp.y1 = Ldsp.y2;
                                        }

                                        if ((combi_total_numbers[i].combi > 1)) // & (SECOND_LINE))
                                        {

                                            Vy = Vy_max;

                                            Vy /= unit_factors->F_f;


                                            xd = jednostkiOb(x);
                                            xdmin = jednostkiOb(axis_increment / units_factor);

                                            x11 = Le.x1 + jednostkiOb(x) * koc;
                                            y11 = Le.y1 + jednostkiOb(x) * kos;


                                            Rotate_Point(kos1, koc1, x11, y11, x11 + Vy / v_magnitude, y11, &xdsp,
                                                         &ydsp);


                                            if (new_line1) {

                                                Ldsp1.x1 = xdsp;
                                                Ldsp1.y1 = ydsp;
                                                Ldsp1.blok = 1;
                                                new_line1 = FALSE;
                                                Ldsp_.x1 = x11;
                                                Ldsp_.y1 = y11;
                                                Ldsp_.x2 = Ldsp1.x1;
                                                Ldsp_.y2 = Ldsp1.y1;
                                                Ldsp_.blok = 1;
                                                ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                if (check_if_not_virtual(x, PL, rep_element_no)) {
                                                    if (!Check_if_Equal(Vy, 0.0) &&
                                                        ((Check_if_Equal(Vy, VyM) || Check_if_Equal(Vy, Vym)) ||
                                                         (Check_if_Equal(x, 0.0) ||
                                                          Check_if_Equal(jednostkiOb(x), PL.dl)) || cfp[inx])) {
                                                        if (Check_if_Equal(VyM, Vym)) {
                                                            if ((PL.dl * 3 / 5 - xd) < xdmin)
                                                                draw_label(&Ldsp_, &Le, x, 0, 0, Vy, force_precision,
                                                                           FALSE);
                                                        } else {
                                                            if (!Check_if_Equal(Vy, Vy01))
                                                                draw_label(&Ldsp_, &Le, x, 0, 0, Vy, force_precision,
                                                                           FALSE);
                                                        }
                                                        Vy01 = Vy;
                                                    }
                                                }
                                            } else {

                                                Ldsp1.x2 = xdsp;
                                                Ldsp1.y2 = ydsp;
                                                Ldsp1.blok = 1;

                                                Ldsp_.x1 = x11;
                                                Ldsp_.y1 = y11;
                                                Ldsp_.x2 = Ldsp1.x2;
                                                Ldsp_.y2 = Ldsp1.y2;
                                                Ldsp_.blok = 1;
                                                ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                if (check_if_not_virtual(x, PL, rep_element_no)) {
                                                    if (!Check_if_Equal(Vy, 0.0) &&
                                                        ((Check_if_Equal(Vy, VyM) || Check_if_Equal(Vy, Vym)) ||
                                                         (Check_if_Equal(x, 0.0) ||
                                                          Check_if_Equal(jednostkiOb(x), PL.dl)) || cfp[inx])) {
                                                        if (Check_if_Equal(VyM, Vym)) {
                                                            if ((PL.dl * 3 / 5 - xd) < xdmin)
                                                                draw_label(&Ldsp_, &Le, x, 0, 0, Vy, force_precision,
                                                                           FALSE);
                                                        } else {
                                                            if (!Check_if_Equal(Vy, Vy01))
                                                                draw_label(&Ldsp_, &Le, x, 0, 0, Vy, force_precision,
                                                                           FALSE);
                                                        }
                                                        Vy01 = Vy;
                                                    }
                                                }

                                                if (dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1) == NULL) {
                                                    fclose(f);
                                                    ret = ask_question(1, "", (char *) confirm, "",
                                                                       (char *) _CANNOT_CREATE_FORCE_BLOCK_, 12, "", 11,
                                                                       1,
                                                                       62);

                                                    no_error=FALSE;
                                                    goto error;
                                                }

                                                Ldsp1.x1 = Ldsp1.x2;
                                                Ldsp1.y1 = Ldsp1.y2;
                                            }
                                        }

                                    }
                                }
                            }
                        }
                        free(cfp);
                        free(cfm);
                        free(dr);
                    }
                    if (rout) printf("%s", report_row);
                }
                fclose(f);
            }
            //Mz
            Ldsp.kolor = static_colors.moment_color;

            for (i = 0; i < combinations_number; i++)   //st_layer_no
            {
                /////////////////////
                //for combinations all is already set in the buffer, so it's not necessary to read it again for building blocks
                if ((combi_total_numbers[i].combi > 1) && (combi_total_numbers[i].last != 1)) continue;
                il = combi_total_numbers[i].case_no;

                if (combi_total_numbers[i].combi == 0) {
                    strcpy(SLS_ULS, "sls");
                    strcpy(SLS_ULS_BAK, "case");
                    strcpy(SLS_ULS_BLK, "sls");
                }
                else if (combi_total_numbers[i].combi == 1) {
                    strcpy(SLS_ULS, "ULS");
                    strcpy(SLS_ULS_BAK, "ULS");
                    strcpy(SLS_ULS_BLK, "ULS");
                }
                else {
                    if (combi_total_numbers[i].combi == 2) {
                        strcpy(SLS_ULS, "ULSLC");
                        strcpy(SLS_ULS_BAK, "ULSLC");
                        strcpy(SLS_ULS_BLK, "ULSLC");
                    }
                    else if (combi_total_numbers[i].combi == 3) {
                        strcpy(SLS_ULS, "slsLC");
                        strcpy(SLS_ULS_BAK, "slsLC");
                        strcpy(SLS_ULS_BLK, "slsLC");
                    }
                    else if (combi_total_numbers[i].combi == 4) {
                        if (qpslsLC_Layer) {
                            strcpy(SLS_ULS, "qpslsLC");
                            strcpy(SLS_ULS_BAK, "qpslsLC");
                            strcpy(SLS_ULS_BLK, "qpslsLC");
                        }
                        else {
                            strcpy(SLS_ULS, "slsLC");
                            strcpy(SLS_ULS_BAK, "slsLC");
                            strcpy(SLS_ULS_BLK, "qpslsLC");
                        }
                    }
                }

                sprintf(desired_layer_bak, "%s_%s_%s_%d", ptr_id, "bending_moments", SLS_ULS_BAK, combination_no);
                sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "bending_moments", SLS_ULS, combination_no);
                sprintf(block_name, "%s$M_%s_%d_%d", ptr_id_short, SLS_ULS_BLK, combination_no,
                        combi_total_numbers[i].combination);

                hiding = (combi_total_numbers[i].combi != 2);  //ULSLC only

                for (ii = 0; ii < No_Layers; ii++) {
                    if (strcmp(Layers[ii].name, desired_layer_bak) == 0) {
                        strcpy(Layers[ii].name, desired_layer);
                        break;
                    }
                    if (strcmp(Layers[ii].name, desired_layer) == 0)
                        break;
                }
                if (ii < No_Layers) {
                    desired_layer_no = ii;
                    if (((combi_total_numbers[i].first) || (combi_total_numbers[i].last)) &&
                        ((combi_total_numbers[i].combi != 4) || (qpslsLC_Layer == TRUE)))
                        delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
                } else {
                    if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {
                        ret = ask_question(1, "", (char *) confirm, "",
                                           (char *) _CANNOT_CREATE_NEW_LAYER_, 12, "", 11,
                                           1, 62);
                        no_error=FALSE;
                        goto error;
                    }
                    if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                        No_Layers++;
                        desired_layer_no = No_Layers - 1;
                        Layers[No_Layers - 1].on = 1;
                        Layers[No_Layers - 1].edit = 1;
                        Layers[No_Layers - 1].point = 1;
                        strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
                        Layers[No_Layers - 1].color = Ldsp.kolor;
                    }
                }

                Ldsp.warstwa = desired_layer_no;
                /////////////////////
                memmove(&Ldsp1, &Ldsp, sizeof(LINIA));
                /////////////////////
                memmove(&Ldsp_, &Ldsp, sizeof(LINIA));

                sprintf(params, "alfacad.if%02d", i + 1);
                f = fopen(params, "rt");
                if (f == NULL) {

                    ret = ask_question(1, "", (char *) confirm, "",
                                       (char *) _CANNOT_OPEN_RESULTS_DATA_FILE_, 12, "", 11,
                                       1, 62);
                    no_error=FALSE;
                    goto error;
                }

                if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                    ptr_block = add_block(st_node[0].x, st_node[0].y, 'E', block_name, hiding);
                    if (ptr_block == NULL) {
                        fclose(f);
                        ret = ask_question(1, "", (char *) confirm, "",
                                           (char *) _CANNOT_CREATE_MOMENT_BLOCK_, 12, "", 11, 1, 62);
                        no_error=FALSE;
                        goto error;
                    }
                }

                new_line = TRUE;
                new_line1 = TRUE;

                while (fgets(report_row, MaxTextLen, f)) {
                    double x, Nx, Vy, Vz, Tx, My, Mz, Dx, Dy, Dz, Rx;
                    ptr = strstr(report_row, "# @");
                    ptr_max = strstr(report_row, "# MAXIMUM");
                    ptr_min = strstr(report_row, "# MINIMUM");
                    ptr_row = strstr(report_row, "#.x");
                    if (ptr) {
                        ret = sscanf(ptr + 4, "%d %d %d %lf %lf %lf %lf %lf %lf %d", &rep_element_no, &N1, &N2, &X1,
                                     &Y1, &Z1, &X2, &Y2, &Z2, &nx);

                        dr = (DATA_ROW *) malloc(nx * sizeof(DATA_ROW));
                        cfp = (int *) malloc(nx * sizeof(int));
                        cfm = (int *) malloc(nx * sizeof(int));

                        n1 = st_element[rep_element_no - 1].node1;
                        n2 = st_element[rep_element_no - 1].node2;
                        Le.x1 = st_node[n1].x;
                        Le.y1 = st_node[n1].y;
                        Le.x2 = st_node[n2].x;
                        Le.y2 = st_node[n2].y;
                        parametry_lini(&Le, &PL);
                        kos = sin(PL.kat * Pi / 180);
                        koc = cos(PL.kat * Pi / 180);

                        kos1 = sin(Angle_Normal((PL.kat - 90) * Pi / 180));
                        koc1 = cos(Angle_Normal((PL.kat - 90) * Pi / 180));

                        Dxy0 = 0;
                        Nx0 = 0;
                        Vy0 = 0;
                        Mz0 = 0;
                        Dxy01 = 0;
                        Nx01 = 0;
                        Vy01 = 0;
                        Mz01 = 0;
                        strcpy(T_text, "");
                        new_line = TRUE;
                        new_line1 = TRUE;

                    } else if (ptr_max) {
                        ret = sscanf(ptr_max + 10, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &NxM, &VyM, &VzM,
                                     &TxM, &MyM, &MzM, &DxM, &DyM, &DzM, &RxM);
                        NxM /= unit_factors->F_f;
                        VyM /= unit_factors->F_f;
                        MzM /= unit_factors->M_f;

                    } else if (ptr_min) {
                        ret = sscanf(ptr_min + 10, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &Nxm, &Vym, &Vzm,
                                     &Txm, &Mym, &Mzm, &Dxm, &Dym, &Dzm, &Rxm);
                        Nxm /= unit_factors->F_f;
                        Vym /= unit_factors->F_f;
                        Mzm /= unit_factors->M_f;

                    } else if (ptr_row) {
                        if (not_ignored) {
                            int inxi = 0;
                            while (inxi < nx) {
                                fgets(report_row, MaxTextLen, f);
                                if ((strlen(report_row) > 0) && (strchr(report_row, '#') == NULL)) {
                                    if (sscanf(report_row, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &dr[inxi].x,
                                               &dr[inxi].Nx, &dr[inxi].Vy, &dr[inxi].Vz, &dr[inxi].Tx, &dr[inxi].My,
                                               &dr[inxi].Mz, &dr[inxi].Dx, &dr[inxi].Dy, &dr[inxi].Dz, &dr[inxi].Rx) ==
                                        11)
                                        inxi++;
                                }
                            }

                            if (combi_total_numbers[i].combi > 1)  //combi
                            {
                                COMBI_ELEMENT *combi_element;
                                if (combi_total_numbers[i].combi == 2) combi_element = combi_element_uls;
                                else if (combi_total_numbers[i].combi == 3) combi_element = combi_element_sls;
                                else combi_element = combi_element_qpsls;

                                COMBI_FORCES *fd = combi_element[rep_element_no - 1].fd;

                                double Mz_min, Mz_max;

                                for (int k = 0; k < nx; k++) cfp[k] = cfm[k] = 0;
                                for (int k = 1; k < nx - 1; k++) {
                                    if (fd[k].Mz_max > fd[k - 1].Mz_max && fd[k].Mz_max > fd[k + 1].Mz_max) cfp[k] = 1;
                                    if (fd[k].Mz_min < fd[k - 1].Mz_min && fd[k].Mz_min < fd[k + 1].Mz_min) cfm[k] = 1;
                                }
                            } else//uls or sls
                            {
                                for (int k = 0; k < nx; k++) cfp[k] = cfm[k] = 0;
                                for (int k = 1; k < nx - 1; k++) {
                                    if (fabs(dr[k].Mz) > fabs(dr[k - 1].Mz) && fabs(dr[k].Mz) > fabs(dr[k + 1].Mz))
                                        cfm[k] = 1;
                                }
                            }

                            for (int inx = 0; inx < inxi; inx++) {
                                double Nx_min, Nx_max, Vy_min, Vy_max, Mz_min, Mz_max, Dx_min, Dx_max, Dy_min, Dy_max;

                                if (combi_total_numbers[i].combi > 1)  //combi
                                {
                                    COMBI_ELEMENT *combi_element;
                                    if (combi_total_numbers[i].combi == 2) combi_element = combi_element_uls;
                                    else if (combi_total_numbers[i].combi == 3) combi_element = combi_element_sls;
                                    else combi_element = combi_element_qpsls;

                                    COMBI_FORCES *fd = combi_element[rep_element_no - 1].fd;

                                    Nx_min = fd[inx].Nx_min;
                                    Nx_max = fd[inx].Nx_max;
                                    Vy_min = fd[inx].Vy_min;
                                    Vy_max = fd[inx].Vy_max;
                                    Mz_min = fd[inx].Mz_min;
                                    Mz_max = fd[inx].Mz_max;
                                    Dx_min = fd[inx].Dx_min;
                                    Dx_max = fd[inx].Dx_max;
                                    Dy_min = fd[inx].Dy_min;
                                    Dy_max = fd[inx].Dy_max;

                                    Mzm = combi_element[rep_element_no - 1].Mzm / unit_factors->M_f;
                                    MzM = combi_element[rep_element_no - 1].MzM / unit_factors->M_f;

                                    if (copysign(1.0, Mz_min) == copysign(1.0, Mz_max)) {
                                        if (fabs(Mz_min) < fabs(Mz_max)) Mz_min = 0;
                                        else Mz_max = 0;
                                    }

                                }
                                if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                                    if (((st_element[rep_element_no - 1].node1r != -1) &&
                                         (st_element[rep_element_no - 1].node2r != -1)))
                                    {
                                        if (combi_total_numbers[i].combi > 1) {
                                            Mz = Mz_min;
                                        } else Mz = dr[inx].Mz;

                                        Mz /= unit_factors->M_f;

                                        x = dr[inx].x;

                                        x /= units_factor;

                                        x11 = Le.x1 + jednostkiOb(x) * koc;
                                        y11 = Le.y1 + jednostkiOb(x) * kos;

                                        Rotate_Point(kos1, koc1, x11, y11, x11 + Mz / m_magnitude, y11, &xdsp, &ydsp);

                                        if (new_line) {
                                            Ldsp.x1 = xdsp;
                                            Ldsp.y1 = ydsp;
                                            Ldsp.blok = 1;
                                            new_line = FALSE;
                                            Ldsp_.x1 = x11;
                                            Ldsp_.y1 = y11;
                                            Ldsp_.x2 = Ldsp.x1;
                                            Ldsp_.y2 = Ldsp.y1;
                                            Ldsp_.blok = 1;
                                            ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                            if (!Check_if_Equal(Mz, 0.0) &&
                                                ((Check_if_Equal(Mz, MzM) || Check_if_Equal(Mz, Mzm)) ||
                                                 (Check_if_Equal(x, 0.0) ||
                                                  Check_if_Equal(jednostkiOb(x), PL.dl)) || cfm[inx])) {
                                                if (!Check_if_Equal(Mz, Mz0))
                                                    draw_label(&Ldsp_, &Le, x, 0, 0, Mz, moment_precision, FALSE);
                                                Mz0 = Mz;
                                            }
                                        } else {
                                            Ldsp.x2 = xdsp;
                                            Ldsp.y2 = ydsp;
                                            Ldsp.blok = 1;

                                            Ldsp_.x1 = x11;
                                            Ldsp_.y1 = y11;
                                            Ldsp_.x2 = Ldsp.x2;
                                            Ldsp_.y2 = Ldsp.y2;
                                            Ldsp_.blok = 1;
                                            ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                            if (!Check_if_Equal(Mz, 0.0) &&
                                                ((Check_if_Equal(Mz, MzM) || Check_if_Equal(Mz, Mzm)) ||
                                                 (Check_if_Equal(x, 0.0) ||
                                                  Check_if_Equal(jednostkiOb(x), PL.dl)) || cfm[inx])) {
                                                if (!Check_if_Equal(Mz, Mz0))
                                                    draw_label(&Ldsp_, &Le, x, 0, 0, Mz, moment_precision, FALSE);
                                                Mz0 = Mz;
                                            }

                                            if (dodaj_obiekt((BLOK *) dane, (void *) &Ldsp) == NULL) {
                                                fclose(f);
                                                ret = ask_question(1, "", (char *) confirm, "",
                                                                   (char *) _CANNOT_CREATE_MOMENT_BLOCK_, 12, "", 11, 1,
                                                                   62);

                                                no_error=FALSE;
                                                goto error;
                                            }

                                            Ldsp.x1 = Ldsp.x2;
                                            Ldsp.y1 = Ldsp.y2;
                                        }
                                        if ((combi_total_numbers[i].combi > 1))// & (SECOND_LINE))
                                        {
                                            Mz = Mz_max;

                                            Mz /= unit_factors->M_f;

                                            x11 = Le.x1 + jednostkiOb(x) * koc;
                                            y11 = Le.y1 + jednostkiOb(x) * kos;

                                            Rotate_Point(kos1, koc1, x11, y11, x11 + Mz / m_magnitude, y11, &xdsp,
                                                         &ydsp);


                                            if (new_line1) {
                                                Ldsp1.x1 = xdsp;
                                                Ldsp1.y1 = ydsp;
                                                Ldsp1.blok = 1;
                                                new_line1 = FALSE;
                                                Ldsp_.x1 = x11;
                                                Ldsp_.y1 = y11;
                                                Ldsp_.x2 = Ldsp1.x1;
                                                Ldsp_.y2 = Ldsp1.y1;
                                                Ldsp_.blok = 1;
                                                ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                if (!Check_if_Equal(Mz, 0.0) &&
                                                    ((Check_if_Equal(Mz, MzM) || Check_if_Equal(Mz, Mzm)) ||
                                                     (Check_if_Equal(x, 0.0) ||
                                                      Check_if_Equal(jednostkiOb(x), PL.dl)) || cfp[inx])) {
                                                    if (!Check_if_Equal(Mz, Mz01))
                                                        draw_label(&Ldsp_, &Le, x, 0, 0, Mz, moment_precision,
                                                                   FALSE);
                                                    Mz01 = Mz;
                                                }
                                            } else {
                                                Ldsp1.x2 = xdsp;
                                                Ldsp1.y2 = ydsp;
                                                Ldsp1.blok = 1;

                                                Ldsp_.x1 = x11;
                                                Ldsp_.y1 = y11;
                                                Ldsp_.x2 = Ldsp1.x2;
                                                Ldsp_.y2 = Ldsp1.y2;
                                                Ldsp_.blok = 1;
                                                ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                if (!Check_if_Equal(Mz, 0.0) &&
                                                    ((Check_if_Equal(Mz, MzM) || Check_if_Equal(Mz, Mzm)) ||
                                                     (Check_if_Equal(x, 0.0) ||
                                                      Check_if_Equal(jednostkiOb(x), PL.dl)) || cfp[inx])) {
                                                    if (!Check_if_Equal(Mz, Mz01))
                                                        draw_label(&Ldsp_, &Le, x, 0, 0, Mz, moment_precision,
                                                                   FALSE);
                                                    Mz01 = Mz;
                                                }

                                                if (dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1) == NULL) {
                                                    fclose(f);
                                                    ret = ask_question(1, "", (char *) confirm, "",
                                                                       (char *) _CANNOT_CREATE_MOMENT_BLOCK_, 12, "",
                                                                       11, 1,
                                                                       62);

                                                    no_error=FALSE;
                                                    goto error;
                                                }

                                                Ldsp1.x1 = Ldsp1.x2;
                                                Ldsp1.y1 = Ldsp1.y2;
                                            }

                                        }
                                    }
                                }
                            }
                        }
                        free(cfp);
                        free(cfm);
                        free(dr);
                    }
                    if (rout) printf("%s", report_row);
                }
                fclose(f);
            }
            //σ  sigma  stress check  - just within element lenght outside nodes radius
            //stress_color is built from 2;
            Ldsp.kolor = static_stress_colors.axial_stress_plus_color;

            memmove(&Ldsp_, &Ldsp, sizeof(LINIA));
            memmove(&Ldsp1, &Ldsp, sizeof(LINIA));
            memmove(&Ldsp01, &Ldsp, sizeof(LINIA));

            Ldsp1.kolor = static_stress_colors.axial_stress_minus_color;
            memmove(&Ldsp11, &Ldsp1, sizeof(LINIA));
            memmove(&Ldsp1_, &Ldsp1, sizeof(LINIA));

            for (i = 0; i < combinations_number; i++)
            {
                /////////////////////
                //for combinations all is already set in the buffer, so it's not necessary to read it again for building blocks
                if ((combi_total_numbers[i].combi > 1) && (combi_total_numbers[i].last != 1)) continue;
                il = combi_total_numbers[i].case_no;

                if (combi_total_numbers[i].combi == 0) {
                    strcpy(SLS_ULS, "sls");
                    strcpy(SLS_ULS_BAK, "case");
                    strcpy(SLS_ULS_BLK, "sls");
                }
                else if (combi_total_numbers[i].combi == 1) {
                    strcpy(SLS_ULS, "ULS");
                    strcpy(SLS_ULS_BAK, "ULS");
                    strcpy(SLS_ULS_BLK, "ULS");
                }
                else {
                    if (combi_total_numbers[i].combi == 2) {
                        strcpy(SLS_ULS, "ULSLC");
                        strcpy(SLS_ULS_BAK, "ULSLC");
                        strcpy(SLS_ULS_BLK, "ULSLC");
                    }
                    else if (combi_total_numbers[i].combi == 3) {
                        strcpy(SLS_ULS, "slsLC");
                        strcpy(SLS_ULS_BAK, "slsLC");
                        strcpy(SLS_ULS_BLK, "slsLC");
                    }
                    else if (combi_total_numbers[i].combi == 4) {
                        if (qpslsLC_Layer) {
                            strcpy(SLS_ULS, "qpslsLC");
                            strcpy(SLS_ULS_BAK, "qpslsLC");
                            strcpy(SLS_ULS_BLK, "qpslsLC");
                        }
                        else {
                            strcpy(SLS_ULS, "slsLC");
                            strcpy(SLS_ULS_BAK, "slsLC");
                            strcpy(SLS_ULS_BLK, "qpslsLC");
                        }
                    }
                }

                sprintf(desired_layer_bak, "%s_%s_%s_%d", ptr_id, "stress", SLS_ULS_BAK, combination_no);
                sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "stress", SLS_ULS, combination_no);
                sprintf(block_name, u8"%sσ$_%s_%d_%d", ptr_id_short, SLS_ULS_BLK, combination_no,
                        combi_total_numbers[i].combination);

                hiding = (combi_total_numbers[i].combi != 2);  //ULSLC only

                for (ii = 0; ii < No_Layers; ii++) {
                    if (strcmp(Layers[ii].name, desired_layer_bak) == 0) {
                        strcpy(Layers[ii].name, desired_layer);
                        break;
                    }
                    if (strcmp(Layers[ii].name, desired_layer) == 0)
                        break;
                }
                if (ii < No_Layers) {
                    desired_layer_no = ii;
                    if (((combi_total_numbers[i].first) || (combi_total_numbers[i].last)) &&
                        ((combi_total_numbers[i].combi != 4) || (qpslsLC_Layer == TRUE)))
                        delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
                } else {
                    if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {
                        ret = ask_question(1, "", (char *) confirm, "",
                                           (char *) _CANNOT_CREATE_NEW_LAYER_, 12, "", 11,
                                           1, 62);
                        no_error=FALSE;
                        goto error;
                    }
                    if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                        No_Layers++;
                        desired_layer_no = No_Layers - 1;
                        Layers[No_Layers - 1].on = 1;
                        Layers[No_Layers - 1].edit = 1;
                        Layers[No_Layers - 1].point = 1;
                        strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
                        Layers[No_Layers - 1].color = Ldsp.kolor;
                    }
                }

                Ldsp.warstwa = desired_layer_no;
                Ldsp01.warstwa = desired_layer_no;
                Ldsp_.warstwa = desired_layer_no;

                Ldsp1.warstwa = desired_layer_no;
                Ldsp11.warstwa = desired_layer_no;
                Ldsp1_.warstwa = desired_layer_no;
                /////////////////////

                sprintf(params, "alfacad.if%02d", i + 1);
                f = fopen(params, "rt");
                if (f == NULL) {

                    ret = ask_question(1, "", (char *) confirm, "",
                                       (char *) _CANNOT_OPEN_RESULTS_DATA_FILE_, 12, "", 11,
                                       1, 62);
                    no_error=FALSE;
                    goto error;
                }

                if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                    ptr_block = add_block(st_node[0].x, st_node[0].y, 'E', block_name, hiding);
                    if (ptr_block == NULL) {
                        fclose(f);
                        ret = ask_question(1, "", (char *) confirm, "",
                                           (char *) _CANNOT_CREATE_STRESS_BLOCK_, 12, "", 11, 1, 62);
                        no_error=FALSE;
                        goto error;
                    }
                }

                new_line = TRUE;
                new_line1 = TRUE;

                while (fgets(report_row, MaxTextLen, f)) {
                    double x, Nx, Vy, Vz, Tx, My, Mz, Dx, Dy, Dz, Rx;
                    ptr = strstr(report_row, "# @");
                    ptr_max = strstr(report_row, "# MAXIMUM");
                    ptr_min = strstr(report_row, "# MINIMUM");
                    ptr_row = strstr(report_row, "#.x");
                    if (ptr) {
                        ret = sscanf(ptr + 4, "%d %d %d %lf %lf %lf %lf %lf %lf %d", &rep_element_no, &N1, &N2, &X1,
                                     &Y1, &Z1, &X2, &Y2, &Z2, &nx);

                        dr = (DATA_ROW *) malloc(nx * sizeof(DATA_ROW));
                        cfp = (int *) malloc(nx * sizeof(int));
                        cfm = (int *) malloc(nx * sizeof(int));

                        n1 = st_element[rep_element_no - 1].node1;
                        n2 = st_element[rep_element_no - 1].node2;
                        Le.x1 = st_node[n1].x;
                        Le.y1 = st_node[n1].y;
                        Le.x2 = st_node[n2].x;
                        Le.y2 = st_node[n2].y;
                        parametry_lini(&Le, &PL);
                        kos = sin(PL.kat * Pi / 180);
                        koc = cos(PL.kat * Pi / 180);

                        kos1 = sin(Angle_Normal((PL.kat - 90) * Pi / 180));
                        koc1 = cos(Angle_Normal((PL.kat - 90) * Pi / 180));

                        int property_no = st_element[rep_element_no - 1].property_no;
                        //search for properties
                        for (int ip = 0; ip < st_property_no; ip++) {
                            if (st_property[ip].n == property_no) {
                                Ax = st_property[ip].A * unit_factors->Am_f;
                                Asy = st_property[ip].Asy * unit_factors->Am_f;
                                Wy = st_property[ip].Wy;
                                break;
                            }
                            if (ip == st_property_no) {
                                sprintf(report_row, "#%d %s\n", st_element[st_thermal_load[i].element].property_no,
                                        _property_not_defined_);
                                strcat(report, report_row);
                                break;
                            }
                        }

                        Dxy0 = 0;
                        Nx0 = 0;
                        Vy0 = 0;
                        Mz0 = 0, Sp0 = 0, Sm0 = 0;
                        Dxy01 = 0;
                        Nx01 = 0;
                        Vy01 = 0;
                        Mz01 = 0, Sp01 = 0, Sm01 = 0;
                        strcpy(T_text, "");
                        new_line = TRUE;
                        new_line1 = TRUE;

                    } else if (ptr_max) {
                        ret = sscanf(ptr_max + 10, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &NxM, &VyM, &VzM,
                                     &TxM, &MyM, &MzM, &DxM, &DyM, &DzM, &RxM);

                        NxM /= unit_factors->F_f;
                        VyM /= unit_factors->F_f;
                        MzM /= unit_factors->M_f;

                        SpM = (NxM / Ax + fabs(MzM) / Wy) / unit_factors->S_f;
                        SmM = (NxM / Ax - fabs(MzM) / Wy) / unit_factors->S_f;

                    } else if (ptr_min) {
                        ret = sscanf(ptr_min + 10, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &Nxm, &Vym, &Vzm,
                                     &Txm, &Mym, &Mzm, &Dxm, &Dym, &Dzm, &Rxm);
                        Nxm /= unit_factors->F_f;
                        Vym /= unit_factors->F_f;
                        Mzm /= unit_factors->M_f;

                        Spm = (Nxm / Ax + fabs(Mzm) / Wy) / unit_factors->S_f;
                        Smm = (Nxm / Ax - fabs(Mzm) / Wy) / unit_factors->S_f;

                    } else if (ptr_row) {
                        if (not_ignored) {
                            int inxi = 0;
                            while (inxi < nx) {
                                fgets(report_row, MaxTextLen, f);
                                if ((strlen(report_row) > 0) && (strchr(report_row, '#') == NULL)) {
                                    if (sscanf(report_row, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &dr[inxi].x,
                                               &dr[inxi].Nx, &dr[inxi].Vy, &dr[inxi].Vz, &dr[inxi].Tx, &dr[inxi].My,
                                               &dr[inxi].Mz, &dr[inxi].Dx, &dr[inxi].Dy, &dr[inxi].Dz, &dr[inxi].Rx) ==
                                        11) {
                                        Nx = dr[inxi].Nx / unit_factors->F_f;
                                        Mz = dr[inxi].Mz / unit_factors->M_f;

                                        dr[inxi].Sp = (Nx / Ax + (Mz) / Wy) / unit_factors->S_f;
                                        dr[inxi].Sm = (Nx / Ax - (Mz) / Wy) / unit_factors->S_f;

                                        inxi++;
                                    }
                                }
                            }

                            if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                                if ((st_element[rep_element_no - 1].node1r != -1) &&
                                    (st_element[rep_element_no - 1].node2r != -1)) {
                                    if (combi_total_numbers[i].combi > 1)  //combi
                                    {
                                        COMBI_ELEMENT *combi_element;
                                        if (combi_total_numbers[i].combi == 2) combi_element = combi_element_uls;
                                        else if (combi_total_numbers[i].combi == 3) combi_element = combi_element_sls;
                                        else combi_element = combi_element_qpsls;

                                        COMBI_FORCES *fd = combi_element[rep_element_no - 1].fd;

                                        for (int k = 0; k < nx; k++) cfp[k] = cfm[k] = 0;
                                        for (int k = 1; k < nx - 1; k++) {
                                            if (copysign(1.0, fd[k].Sp_min) == copysign(1.0, fd[k].Sp_max)) {
                                                if (fabs(fd[k].Sp_min) < fabs(fd[k].Sp_max)) {
                                                    if (fd[k].Sp_max > fd[k - 1].Sp_max &&
                                                        fd[k].Sp_max > fd[k + 1].Sp_max)
                                                        cfp[k] = 1;
                                                } else {
                                                    if (fd[k].Sp_min < fd[k - 1].Sp_min &&
                                                        fd[k].Sp_min < fd[k + 1].Sp_min)
                                                        cfp[k] = 1;
                                                }
                                            } else {
                                                if (fd[k].Sp_max > fd[k - 1].Sp_max &&
                                                    fd[k].Sp_max > fd[k + 1].Sp_max)
                                                    cfp[k] = 1;
                                                if (fd[k].Sp_min < fd[k - 1].Sp_min &&
                                                    fd[k].Sp_min < fd[k + 1].Sp_min)
                                                    cfp[k] = 1;
                                            }

                                        }
                                        for (int k = 1; k < nx - 1; k++) {
                                            if (copysign(1.0, fd[k].Sm_min) == copysign(1.0, fd[k].Sm_max)) {
                                                if (fabs(fd[k].Sm_min) < fabs(fd[k].Sm_max)) {
                                                    if (fd[k].Sm_max > fd[k - 1].Sm_max &&
                                                        fd[k].Sm_max > fd[k + 1].Sm_max)
                                                        cfm[k] = 1;
                                                } else {
                                                    if (fd[k].Sm_min < fd[k - 1].Sm_min &&
                                                        fd[k].Sm_min < fd[k + 1].Sm_min)
                                                        cfm[k] = 1;
                                                }
                                            } else {
                                                if (fd[k].Sm_max > fd[k - 1].Sm_max &&
                                                    fd[k].Sm_max > fd[k + 1].Sm_max)
                                                    cfm[k] = 1;
                                                if (fd[k].Sm_min < fd[k - 1].Sm_min &&
                                                    fd[k].Sm_min < fd[k + 1].Sm_min)
                                                    cfm[k] = 1;
                                            }
                                        }
                                    } else //uls or sls
                                    {
                                        for (int k = 0; k < nx; k++) cfp[k] = cfm[k] = 0;
                                        for (int k = 1; k < nx - 1; k++) {
                                            if (dr[k].Sp > dr[k - 1].Sp && dr[k].Sp > dr[k + 1].Sp) cfp[k] = 1;
                                            if (dr[k].Sp < dr[k - 1].Sp && dr[k].Sp < dr[k + 1].Sp) cfp[k] = 1;
                                        }
                                        for (int k = 1; k < nx - 1; k++) {
                                            if (dr[k].Sm > dr[k - 1].Sm && dr[k].Sm > dr[k + 1].Sm) cfm[k] = 1;
                                            if (dr[k].Sm < dr[k - 1].Sm && dr[k].Sm < dr[k + 1].Sm) cfm[k] = 1;
                                        }
                                    }
                                }
                            }

                            for (int inx = 0; inx < inxi; inx++) {
                                double Nx_min, Nx_max, Vy_min, Vy_max, Mz_min, Mz_max, Dx_min, Dx_max, Dy_min, Dy_max, Sp_min, Sp_max, Sm_min, Sm_max, Ss_min, Ss_max;

                                n1 = st_element[rep_element_no - 1].node1;
                                n2 = st_element[rep_element_no - 1].node2;
                                double r1 = st_node[n1].radius;
                                double r2 = st_node[n2].radius;

                                double lb = r1 * units_factor;
                                double le = (milimetryob(st_element[rep_element_no - 1].length) - r2) * units_factor;

                                if ((Check_if_GE02(dr[inx].x, lb)) && (Check_if_LE2(dr[inx].x, le))) {
                                    if (combi_total_numbers[i].combi > 1)  //combi
                                    {
                                        COMBI_ELEMENT *combi_element;
                                        if (combi_total_numbers[i].combi == 2) combi_element = combi_element_uls;
                                        else if (combi_total_numbers[i].combi == 3) combi_element = combi_element_sls;
                                        else combi_element = combi_element_qpsls;

                                        COMBI_FORCES *fd = combi_element[rep_element_no - 1].fd;

                                        Nx_min = fd[inx].Nx_min;
                                        Nx_max = fd[inx].Nx_max;
                                        Vy_min = fd[inx].Vy_min;
                                        Vy_max = fd[inx].Vy_max;
                                        Mz_min = fd[inx].Mz_min;
                                        Mz_max = fd[inx].Mz_max;
                                        Dx_min = fd[inx].Dx_min;
                                        Dx_max = fd[inx].Dx_max;
                                        Dy_min = fd[inx].Dy_min;
                                        Dy_max = fd[inx].Dy_max;
                                        Sp_min = fd[inx].Sp_min;
                                        Sp_max = fd[inx].Sp_max;
                                        Sm_min = fd[inx].Sm_min;
                                        Sm_max = fd[inx].Sm_max;


                                        if (copysign(1.0, Sp_min) == copysign(1.0, Sp_max)) {
                                            if (fabs(Sp_min) < fabs(Sp_max)) Sp_min = 0;
                                            else Sp_max = 0;
                                        }

                                        if (copysign(1.0, Sm_min) == copysign(1.0, Sm_max)) {
                                            if (fabs(Sm_min) < fabs(Sm_max)) Sm_min = 0;
                                            else Sm_max = 0;
                                        }

                                        SpM = combi_element[rep_element_no - 1].SpM;
                                        SmM = combi_element[rep_element_no - 1].SmM;

                                        Spm = combi_element[rep_element_no - 1].Spm;
                                        Smm = combi_element[rep_element_no - 1].Smm;

                                    }
                                    if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                                        if ((st_element[rep_element_no - 1].node1r != -1) &&
                                            (st_element[rep_element_no - 1].node2r != -1)) {
                                            if (combi_total_numbers[i].combi > 1) {
                                                Sp = Sp_min;
                                                Sm = Sm_min;

                                            } else {

                                                Sp = dr[inx].Sp;
                                                Sm = dr[inx].Sm;
                                            }

                                            ////if (Sp<0) Ldsp.kolor=Ldsp_.kolor=static_stress_colors.axial_stress_minus_color;
                                            ////else Ldsp.kolor=Ldsp_.kolor=static_stress_colors.axial_stress_plus_color;
                                            ////if (Sm<0) Ldsp1.kolor=Ldsp1_.kolor=static_stress_colors.axial_stress_minus_color;
                                            ////else Ldsp1.kolor=Ldsp1_.kolor=static_stress_colors.axial_stress_plus_color;

                                            x = dr[inx].x;

                                            x /= units_factor;

                                            x11 = Le.x1 + jednostkiOb(x) * koc;
                                            y11 = Le.y1 + jednostkiOb(x) * kos;

                                            Rotate_Point(kos1, koc1, x11, y11, x11 + fabs(Sp / s_magnitude), y11, &xdsp,
                                                         &ydsp);
                                            Rotate_Point(kos1, koc1, x11, y11, x11 - fabs(Sm / s_magnitude), y11,
                                                         &xdsp1,
                                                         &ydsp1);

                                            if (new_line) {
                                                Ldsp.x1 = xdsp;
                                                Ldsp.y1 = ydsp;
                                                Ldsp.blok = 1;
                                                new_line = FALSE;
                                                Ldsp_.x1 = x11;
                                                Ldsp_.y1 = y11;
                                                Ldsp_.x2 = Ldsp.x1;
                                                Ldsp_.y2 = Ldsp.y1;
                                                Ldsp_.blok = 1;

                                                if (Sp < 0)
                                                    Ldsp.kolor = Ldsp_.kolor = static_stress_colors.axial_stress_minus_color;
                                                else Ldsp.kolor = Ldsp_.kolor = static_stress_colors.axial_stress_plus_color;
                                                if (Sm < 0)
                                                    Ldsp1.kolor = Ldsp1_.kolor = static_stress_colors.axial_stress_minus_color;
                                                else Ldsp1.kolor = Ldsp1_.kolor = static_stress_colors.axial_stress_plus_color;

                                                ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                if (!Check_if_Equal(Sp, 0.0) &&
                                                    (((Check_if_Equal(dr[inx].Mz, MzM) ||
                                                       Check_if_Equal(dr[inx].Mz, Mzm)) ||
                                                      (Check_if_Equal(Sp, SpM) || Check_if_Equal(Sp, Spm)) ||
                                                      (Check_if_Equal(x, 0.0) ||
                                                       Check_if_Equal(jednostkiOb(x), PL.dl))) ||
                                                     ((Check_if_Equal2(dr[inx].x, lb)) ||
                                                      (Check_if_Equal2(dr[inx].x, le)) || cfp[inx]))) {
                                                    if (!Check_if_Equal(Sp, Sp0))
                                                        draw_label(&Ldsp_, &Le, x, r1, r2, Sp, stress_precision, FALSE);
                                                    Sp0 = Sp;
                                                }

                                                Ldsp1.x1 = xdsp1;
                                                Ldsp1.y1 = ydsp1;
                                                Ldsp1.blok = 1;
                                                Ldsp1_.x1 = x11;
                                                Ldsp1_.y1 = y11;
                                                Ldsp1_.x2 = Ldsp1.x1;
                                                Ldsp1_.y2 = Ldsp1.y1;
                                                Ldsp1_.blok = 1;
                                                ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1_);
                                                if (!Check_if_Equal(Sm, 0.0) &&
                                                    (((Check_if_Equal(dr[inx].Mz, MzM) ||
                                                       Check_if_Equal(dr[inx].Mz, Mzm)) ||
                                                      (Check_if_Equal(Sm, SmM) || Check_if_Equal(Sm, Smm)) ||
                                                      (Check_if_Equal(x, 0.0) ||
                                                       Check_if_Equal(jednostkiOb(x), PL.dl)))) ||
                                                    ((Check_if_Equal2(dr[inx].x, lb)) ||
                                                     (Check_if_Equal2(dr[inx].x, le))) || cfm[inx]) {
                                                    if (!Check_if_Equal(Sm, Sm0))
                                                        draw_label(&Ldsp1_, &Le, x, r1, r2, Sm, stress_precision,
                                                                   FALSE);
                                                    Sm0 = Sm;
                                                }

                                            } else {
                                                Ldsp.x2 = xdsp;
                                                Ldsp.y2 = ydsp;
                                                Ldsp.blok = 1;

                                                Ldsp_.x1 = x11;
                                                Ldsp_.y1 = y11;
                                                Ldsp_.x2 = Ldsp.x2;
                                                Ldsp_.y2 = Ldsp.y2;
                                                Ldsp_.blok = 1;

                                                ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp);

                                                if (Sp < 0)
                                                    Ldsp.kolor = Ldsp_.kolor = static_stress_colors.axial_stress_minus_color;
                                                else Ldsp.kolor = Ldsp_.kolor = static_stress_colors.axial_stress_plus_color;

                                                ptr_l_ = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);

                                                if (!Check_if_Equal(Sp, 0.0) &&
                                                    (((Check_if_Equal(dr[inx].Mz, MzM) ||
                                                       Check_if_Equal(dr[inx].Mz, Mzm)) ||
                                                      (Check_if_Equal(Sp, SpM) || Check_if_Equal(Sp, Spm)) ||
                                                      (Check_if_Equal(x, 0.0) ||
                                                       Check_if_Equal(jednostkiOb(x), PL.dl))) ||
                                                     ((Check_if_Equal2(dr[inx].x, lb)) ||
                                                      (Check_if_Equal2(dr[inx].x, le))) || cfp[inx])) {
                                                    if (!Check_if_Equal(Sp, Sp0))
                                                        draw_label(&Ldsp_, &Le, x, r1, r2, Sp, stress_precision, FALSE);
                                                    Sp0 = Sp;
                                                }

                                                if ((ptr_l == NULL) || (ptr_l_ == NULL)) {
                                                    fclose(f);
                                                    ret = ask_question(1, "", (char *) confirm, "",
                                                                       (char *) _CANNOT_CREATE_MOMENT_BLOCK_, 12, "",
                                                                       11, 1,
                                                                       62);

                                                    no_error=FALSE;
                                                    goto error;
                                                }

                                                Ldsp1.x2 = xdsp1;
                                                Ldsp1.y2 = ydsp1;
                                                Ldsp1.blok = 1;

                                                Ldsp1_.x1 = x11;
                                                Ldsp1_.y1 = y11;
                                                Ldsp1_.x2 = Ldsp1.x2;
                                                Ldsp1_.y2 = Ldsp1.y2;
                                                Ldsp1_.blok = 1;

                                                ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1);

                                                if (Sm < 0)
                                                    Ldsp1.kolor = Ldsp1_.kolor = static_stress_colors.axial_stress_minus_color;
                                                else Ldsp1.kolor = Ldsp1_.kolor = static_stress_colors.axial_stress_plus_color;

                                                ptr_l_ = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1_);

                                                if (!Check_if_Equal(Sm, 0.0) &&
                                                    (((Check_if_Equal(dr[inx].Mz, MzM) ||
                                                       Check_if_Equal(dr[inx].Mz, Mzm)) ||
                                                      (Check_if_Equal(Sm, SmM) || Check_if_Equal(Sm, Smm)) ||
                                                      (Check_if_Equal(x, 0.0) ||
                                                       Check_if_Equal(jednostkiOb(x), PL.dl)))) ||
                                                    ((Check_if_Equal2(dr[inx].x, lb)) ||
                                                     (Check_if_Equal2(dr[inx].x, le))) || cfm[inx]) {
                                                    if (!Check_if_Equal(Sm, Sm0))
                                                        draw_label(&Ldsp1_, &Le, x, r1, r2, Sm, stress_precision,
                                                                   FALSE);
                                                    Sm0 = Sm;
                                                }

                                                if ((ptr_l == NULL) || (ptr_l_ == NULL)) {
                                                    fclose(f);
                                                    ret = ask_question(1, "", (char *) confirm, "",
                                                                       (char *) _CANNOT_CREATE_MOMENT_BLOCK_, 12, "",
                                                                       11, 1,
                                                                       62);

                                                    no_error=FALSE;
                                                    goto error;
                                                }

                                                Ldsp.x1 = Ldsp.x2;
                                                Ldsp.y1 = Ldsp.y2;

                                                Ldsp1.x1 = Ldsp1.x2;
                                                Ldsp1.y1 = Ldsp1.y2;
                                            }

                                            if ((combi_total_numbers[i].combi > 1)) //& (SECOND_LINE))
                                            {

                                                Sp = Sp_max;
                                                Sm = Sm_max;

                                                ////if (Sp<0) Ldsp.kolor=Ldsp_.kolor=static_stress_colors.axial_stress_minus_color;
                                                ////else Ldsp.kolor=Ldsp_.kolor=static_stress_colors.axial_stress_plus_color;
                                                ////if (Sm<0) Ldsp1.kolor=Ldsp1_.kolor=static_stress_colors.axial_stress_minus_color;
                                                ////else Ldsp1.kolor=Ldsp1_.kolor=static_stress_colors.axial_stress_plus_color;


                                                x11 = Le.x1 + jednostkiOb(x) * koc;
                                                y11 = Le.y1 + jednostkiOb(x) * kos;

                                                Rotate_Point(kos1, koc1, x11, y11, x11 + fabs(Sp / s_magnitude), y11,
                                                             &xdsp,
                                                             &ydsp);
                                                Rotate_Point(kos1, koc1, x11, y11, x11 - fabs(Sm / s_magnitude), y11,
                                                             &xdsp1,
                                                             &ydsp1);

                                                if (new_line1) {
                                                    Ldsp01.x1 = xdsp;
                                                    Ldsp01.y1 = ydsp;
                                                    Ldsp01.blok = 1;
                                                    new_line1 = FALSE;
                                                    Ldsp_.x1 = x11;
                                                    Ldsp_.y1 = y11;
                                                    Ldsp_.x2 = Ldsp01.x1;
                                                    Ldsp_.y2 = Ldsp01.y1;
                                                    Ldsp_.blok = 1;

                                                    if (Sp < 0)
                                                        Ldsp01.kolor = Ldsp_.kolor = static_stress_colors.axial_stress_minus_color;
                                                    else Ldsp01.kolor = Ldsp_.kolor = static_stress_colors.axial_stress_plus_color;
                                                    if (Sm < 0)
                                                        Ldsp11.kolor = Ldsp1_.kolor = static_stress_colors.axial_stress_minus_color;
                                                    else Ldsp11.kolor = Ldsp1_.kolor = static_stress_colors.axial_stress_plus_color;

                                                    ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                    if (!Check_if_Equal(Sp, 0.0) &&
                                                        (((Check_if_Equal(dr[inx].Mz, MzM) ||
                                                           Check_if_Equal(dr[inx].Mz, Mzm)) ||
                                                          (Check_if_Equal(Sp, SpM) || Check_if_Equal(Sp, Spm)) ||
                                                          (Check_if_Equal(x, 0.0) ||
                                                           Check_if_Equal(jednostkiOb(x), PL.dl))) ||
                                                         ((Check_if_Equal2(dr[inx].x, lb)) ||
                                                          (Check_if_Equal2(dr[inx].x, le))) || cfp[inx])) {
                                                        if (!Check_if_Equal(Sp, Sp0))
                                                            draw_label(&Ldsp_, &Le, x, r1, r2, Sp, stress_precision,
                                                                       FALSE);
                                                        Sp0 = Sp;
                                                    }

                                                    Ldsp11.x1 = xdsp1;
                                                    Ldsp11.y1 = ydsp1;
                                                    Ldsp11.blok = 1;
                                                    Ldsp1_.x1 = x11;
                                                    Ldsp1_.y1 = y11;
                                                    Ldsp1_.x2 = Ldsp11.x1;
                                                    Ldsp1_.y2 = Ldsp11.y1;
                                                    Ldsp1_.blok = 1;
                                                    ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1_);
                                                    if (!Check_if_Equal(Sm, 0.0) &&
                                                        (((Check_if_Equal(dr[inx].Mz, MzM) ||
                                                           Check_if_Equal(dr[inx].Mz, Mzm)) ||
                                                          (Check_if_Equal(Sm, SmM) || Check_if_Equal(Sm, Smm)) ||
                                                          (Check_if_Equal(x, 0.0) ||
                                                           Check_if_Equal(jednostkiOb(x), PL.dl)))) ||
                                                        ((Check_if_Equal2(dr[inx].x, lb)) ||
                                                         (Check_if_Equal2(dr[inx].x, le))) || cfm[inx]) {
                                                        if (!Check_if_Equal(Sm, Sm01))
                                                            draw_label(&Ldsp1_, &Le, x, r1, r2, Sm, stress_precision,
                                                                       FALSE);
                                                        Sm01 = Sm;
                                                    }

                                                } else {
                                                    Ldsp01.x2 = xdsp;
                                                    Ldsp01.y2 = ydsp;
                                                    Ldsp01.blok = 1;

                                                    Ldsp_.x1 = x11;
                                                    Ldsp_.y1 = y11;
                                                    Ldsp_.x2 = Ldsp01.x2;
                                                    Ldsp_.y2 = Ldsp01.y2;
                                                    Ldsp_.blok = 1;

                                                    ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp01);

                                                    if (Sp < 0)
                                                        Ldsp01.kolor = Ldsp_.kolor = static_stress_colors.axial_stress_minus_color;
                                                    else Ldsp01.kolor = Ldsp_.kolor = static_stress_colors.axial_stress_plus_color;

                                                    ptr_l_ = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);

                                                    if (!Check_if_Equal(Sp, 0.0) &&
                                                        (((Check_if_Equal(dr[inx].Mz, MzM) ||
                                                           Check_if_Equal(dr[inx].Mz, Mzm)) ||
                                                          (Check_if_Equal(Sp, SpM) || Check_if_Equal(Sp, Spm)) ||
                                                          (Check_if_Equal(x, 0.0) ||
                                                           Check_if_Equal(jednostkiOb(x), PL.dl))) ||
                                                         ((Check_if_Equal2(dr[inx].x, lb)) ||
                                                          (Check_if_Equal2(dr[inx].x, le))) || cfp[inx])) {
                                                        if (!Check_if_Equal(Sp, Sp01))
                                                            draw_label(&Ldsp_, &Le, x, r1, r2, Sp, stress_precision,
                                                                       FALSE);
                                                        Sp01 = Sp;
                                                    }

                                                    if ((ptr_l == NULL) || (ptr_l_ == NULL)) {
                                                        fclose(f);
                                                        ret = ask_question(1, "", (char *) confirm, "",
                                                                           (char *) _CANNOT_CREATE_MOMENT_BLOCK_, 12,
                                                                           "",
                                                                           11, 1,
                                                                           62);

                                                        no_error=FALSE;
                                                        goto error;
                                                    }

                                                    Ldsp11.x2 = xdsp1;
                                                    Ldsp11.y2 = ydsp1;
                                                    Ldsp11.blok = 1;

                                                    Ldsp1_.x1 = x11;
                                                    Ldsp1_.y1 = y11;
                                                    Ldsp1_.x2 = Ldsp11.x2;
                                                    Ldsp1_.y2 = Ldsp11.y2;
                                                    Ldsp1_.blok = 1;

                                                    ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp11);

                                                    if (Sm < 0)
                                                        Ldsp11.kolor = Ldsp1_.kolor = static_stress_colors.axial_stress_minus_color;
                                                    else Ldsp11.kolor = Ldsp1_.kolor = static_stress_colors.axial_stress_plus_color;

                                                    ptr_l_ = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp1_);

                                                    if (!Check_if_Equal(Sm, 0.0) &&
                                                        (((Check_if_Equal(dr[inx].Mz, MzM) ||
                                                           Check_if_Equal(dr[inx].Mz, Mzm)) ||
                                                          (Check_if_Equal(Sm, SmM) || Check_if_Equal(Sm, Smm)) ||
                                                          (Check_if_Equal(x, 0.0) ||
                                                           Check_if_Equal(jednostkiOb(x), PL.dl)))) ||
                                                        ((Check_if_Equal2(dr[inx].x, lb)) ||
                                                         (Check_if_Equal2(dr[inx].x, le))) || cfm[inx]) {
                                                        if (!Check_if_Equal(Sm, Sm01))
                                                            draw_label(&Ldsp1_, &Le, x, r1, r2, Sm, stress_precision,
                                                                       FALSE);
                                                        Sm01 = Sm;
                                                    }

                                                    if ((ptr_l == NULL) || (ptr_l_ == NULL)) {
                                                        fclose(f);
                                                        ret = ask_question(1, "", (char *) confirm, "",
                                                                           (char *) _CANNOT_CREATE_MOMENT_BLOCK_, 12,
                                                                           "",
                                                                           11, 1,
                                                                           62);

                                                        no_error=FALSE;
                                                        goto error;
                                                    }

                                                    Ldsp01.x1 = Ldsp01.x2;
                                                    Ldsp01.y1 = Ldsp01.y2;

                                                    Ldsp11.x1 = Ldsp11.x2;
                                                    Ldsp11.y1 = Ldsp11.y2;
                                                }

                                            }
                                        }
                                    }
                                }
                            }
                        }
                        free(cfp);
                        free(cfm);
                        free(dr);
                    }
                    if (rout) printf("%s", report_row);
                }
                fclose(f);
            }

            /////////
            //τ  tau shear stress check
            //stress_color is built from 2;
            Ldsp.kolor = static_stress_colors.shear_stress_color;

            memmove(&Ldsp_, &Ldsp, sizeof(LINIA));
            memmove(&Ldsp1, &Ldsp, sizeof(LINIA));

            memmove(&Ldsp1_, &Ldsp, sizeof(LINIA));

            //for (i=0; i<st_layer_no; i++)
            for (i = 0; i < combinations_number; i++)   //st_layer_no
            {
                /////////////////////
                //for combinations all is already set in the buffer, so it's not necessary to read it again for building blocks
                if ((combi_total_numbers[i].combi > 1) && (combi_total_numbers[i].last != 1)) continue;
                il = combi_total_numbers[i].case_no;

                if (combi_total_numbers[i].combi == 0) {
                    strcpy(SLS_ULS, "sls");
                    strcpy(SLS_ULS_BAK, "case");
                    strcpy(SLS_ULS_BLK, "sls");
                }
                else if (combi_total_numbers[i].combi == 1) {
                    strcpy(SLS_ULS, "ULS");
                    strcpy(SLS_ULS_BAK, "ULS");
                    strcpy(SLS_ULS_BLK, "ULS");
                }
                else {
                    if (combi_total_numbers[i].combi == 2) {
                        strcpy(SLS_ULS, "ULSLC");
                        strcpy(SLS_ULS_BAK, "ULSLC");
                        strcpy(SLS_ULS_BLK, "ULSLC");
                    }
                    else if (combi_total_numbers[i].combi == 3) {
                        strcpy(SLS_ULS, "slsLC");
                        strcpy(SLS_ULS_BAK, "slsLC");
                        strcpy(SLS_ULS_BLK, "slsLC");
                    }
                    else if (combi_total_numbers[i].combi == 4) {
                        if (qpslsLC_Layer) {
                            strcpy(SLS_ULS, "qpslsLC");
                            strcpy(SLS_ULS_BAK, "qpslsLC");
                            strcpy(SLS_ULS_BLK, "qpslsLC");
                        }
                        else {
                            strcpy(SLS_ULS, "slsLC");
                            strcpy(SLS_ULS_BAK, "slsLC");
                            strcpy(SLS_ULS_BLK, "qpslsLC");
                        }
                    }
                }

                sprintf(desired_layer_bak, "%s_%s_%s_%d", ptr_id, "shear_stress", SLS_ULS_BAK, combination_no);
                sprintf(desired_layer, "%s_%s_%s_%d", ptr_id, "shear_stress", SLS_ULS, combination_no);
                sprintf(block_name, u8"%sτ$_%s_%d_%d", ptr_id_short, SLS_ULS_BLK, combination_no,
                        combi_total_numbers[i].combination);

                hiding = (combi_total_numbers[i].combi != 2);  //ULSLC only

                for (ii = 0; ii < No_Layers; ii++) {
                    if (strcmp(Layers[ii].name, desired_layer_bak) == 0) {
                        strcpy(Layers[ii].name, desired_layer);
                        break;
                    }
                    if (strcmp(Layers[ii].name, desired_layer) == 0)
                        break;
                }
                if (ii < No_Layers) {
                    desired_layer_no = ii;
                    if (((combi_total_numbers[i].first) || (combi_total_numbers[i].last)) &&
                        ((combi_total_numbers[i].combi != 4) || (qpslsLC_Layer == TRUE)))
                        delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
                } else {
                    if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {
                        ret = ask_question(1, "", (char *) confirm, "",
                                           (char *) _CANNOT_CREATE_NEW_LAYER_, 12, "", 11,
                                           1, 62);
                        no_error=FALSE;
                        goto error;
                    }
                    if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                        No_Layers++;
                        desired_layer_no = No_Layers - 1;
                        Layers[No_Layers - 1].on = 1;
                        Layers[No_Layers - 1].edit = 1;
                        Layers[No_Layers - 1].point = 1;
                        strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
                        Layers[No_Layers - 1].color = Ldsp.kolor;
                    }
                }

                Ldsp.warstwa = desired_layer_no;
                Ldsp_.warstwa = desired_layer_no;

                Ldsp1.warstwa = desired_layer_no;
                Ldsp1_.warstwa = desired_layer_no;
                /////////////////////

                sprintf(params, "alfacad.if%02d", i + 1);
                f = fopen(params, "rt");
                if (f == NULL) {

                    ret = ask_question(1, "", (char *) confirm, "",
                                       (char *) _CANNOT_OPEN_RESULTS_DATA_FILE_, 12, "", 11,
                                       1, 62);
                    no_error=FALSE;
                    goto error;
                }

                if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                    ptr_block = add_block(st_node[0].x, st_node[0].y, 'E', block_name, hiding);
                    if (ptr_block == NULL) {
                        fclose(f);
                        ret = ask_question(1, "", (char *) confirm, "",
                                           (char *) _CANNOT_CREATE_SHEAR_STRESS_BLOCK_, 12, "", 11, 1, 62);
                        no_error=FALSE;
                        goto error;
                    }
                }

                new_line = TRUE;
                new_line1 = TRUE;

                while (fgets(report_row, MaxTextLen, f)) {
                    double x, Nx, Vy, Vz, Tx, My, Mz, Dx, Dy, Dz, Rx;
                    ptr = strstr(report_row, "# @");
                    ptr_max = strstr(report_row, "# MAXIMUM");
                    ptr_min = strstr(report_row, "# MINIMUM");
                    ptr_row = strstr(report_row, "#.x");
                    if (ptr) {
                        ret = sscanf(ptr + 4, "%d %d %d %lf %lf %lf %lf %lf %lf %d", &rep_element_no, &N1, &N2, &X1,
                                     &Y1, &Z1, &X2, &Y2, &Z2, &nx);

                        dr = (DATA_ROW *) malloc(nx * sizeof(DATA_ROW));

                        n1 = st_element[rep_element_no - 1].node1;
                        n2 = st_element[rep_element_no - 1].node2;
                        Le.x1 = st_node[n1].x;
                        Le.y1 = st_node[n1].y;
                        Le.x2 = st_node[n2].x;
                        Le.y2 = st_node[n2].y;
                        parametry_lini(&Le, &PL);
                        kos = sin(PL.kat * Pi / 180);
                        koc = cos(PL.kat * Pi / 180);

                        kos1 = sin(Angle_Normal((PL.kat - 90) * Pi / 180));
                        koc1 = cos(Angle_Normal((PL.kat - 90) * Pi / 180));

                        int property_no = st_element[rep_element_no - 1].property_no;
                        //search for properties
                        for (int ip = 0; ip < st_property_no; ip++) {
                            if (st_property[ip].n == property_no) {
                                Ax = st_property[ip].A * unit_factors->Am_f;
                                Asy = st_property[ip].Asy * unit_factors->Am_f;
                                Wy = st_property[ip].Wy;
                                break;
                            }
                            if (ip == st_property_no) {
                                sprintf(report_row, "#%d %s\n", st_element[st_thermal_load[i].element].property_no,
                                        _property_not_defined_);
                                strcat(report, report_row);
                                break;
                            }
                        }

                        Dxy0 = 0;
                        Nx0 = 0;
                        Vy0 = 0;
                        Mz0 = 0, Sp0 = 0, Sm0 = 0;
                        Ss0 = 0;
                        Ss01 = 0;
                        strcpy(T_text, "");
                        new_line = TRUE;
                        new_line1 = TRUE;

                    } else if (ptr_max) {
                        ret = sscanf(ptr_max + 10, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &NxM, &VyM, &VzM,
                                     &TxM, &MyM, &MzM, &DxM, &DyM, &DzM, &RxM);

                        VyM /= unit_factors->F_f;

                        SsM = (VyM / Asy) / unit_factors->S_f;

                    } else if (ptr_min) {
                        ret = sscanf(ptr_min + 10, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &Nxm, &Vym, &Vzm,
                                     &Txm, &Mym, &Mzm, &Dxm, &Dym, &Dzm, &Rxm);

                        Vym /= unit_factors->F_f;

                        Ssm = (Vym / Asy) / unit_factors->S_f;

                    } else if (ptr_row) {
                        if (not_ignored) {
                            int inxi = 0;
                            while (inxi < nx) {
                                fgets(report_row, MaxTextLen, f);
                                if ((strlen(report_row) > 0) && (strchr(report_row, '#') == NULL)) {
                                    if (sscanf(report_row, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &dr[inxi].x,
                                               &dr[inxi].Nx, &dr[inxi].Vy, &dr[inxi].Vz, &dr[inxi].Tx, &dr[inxi].My,
                                               &dr[inxi].Mz, &dr[inxi].Dx, &dr[inxi].Dy, &dr[inxi].Dz, &dr[inxi].Rx) ==
                                        11)
                                        inxi++;
                                }
                            }

                            for (int inx = 0; inx < inxi; inx++) {
                                double Nx_min, Nx_max, Vy_min, Vy_max, Mz_min, Mz_max, Dx_min, Dx_max, Dy_min, Dy_max, Sp_min, Sp_max, Sm_min, Sm_max, Ss_min, Ss_max;

                                n1 = st_element[rep_element_no - 1].node1;
                                n2 = st_element[rep_element_no - 1].node2;
                                double r1 = st_node[n1].radius;
                                double r2 = st_node[n2].radius;

                                double lb = r1 * units_factor;
                                double le = (milimetryob(st_element[rep_element_no - 1].length) - r2) * units_factor;

                                if ((Check_if_GE02(dr[inx].x, lb)) && (Check_if_LE2(dr[inx].x, le))) {
                                    if (combi_total_numbers[i].combi > 1)  //combi
                                    {
                                        COMBI_ELEMENT *combi_element;
                                        if (combi_total_numbers[i].combi == 2) combi_element = combi_element_uls;
                                        else if (combi_total_numbers[i].combi == 3) combi_element = combi_element_sls;
                                        else combi_element = combi_element_qpsls;

                                        COMBI_FORCES *fd = combi_element[rep_element_no - 1].fd;

                                        Nx_min = fd[inx].Nx_min;
                                        Nx_max = fd[inx].Nx_max;
                                        Vy_min = fd[inx].Vy_min;
                                        Vy_max = fd[inx].Vy_max;
                                        Mz_min = fd[inx].Mz_min;
                                        Mz_max = fd[inx].Mz_max;
                                        Dx_min = fd[inx].Dx_min;
                                        Dx_max = fd[inx].Dx_max;
                                        Dy_min = fd[inx].Dy_min;
                                        Dy_max = fd[inx].Dy_max;
                                        Sp_min = fd[inx].Sp_min;
                                        Sp_max = fd[inx].Sp_max;
                                        Sm_min = fd[inx].Sm_min;
                                        Sm_max = fd[inx].Sm_max;

                                        VyM = fabs(combi_element[rep_element_no - 1].VyM);
                                        Vym = fabs(combi_element[rep_element_no - 1].Vym);

                                        Vy = max(fabs(Vy_min), fabs(Vy_max));

                                    }
                                    if ((combi_total_numbers[i].combi < 2) || (combi_total_numbers[i].last == 1)) {
                                        if ((st_element[rep_element_no - 1].node1r != -1) &&
                                            (st_element[rep_element_no - 1].node2r != -1)) {
                                            if (combi_total_numbers[i].combi > 1) { ; //Vy = Vy_min;
                                            } else Vy = dr[inx].Vy;

                                            Vy /= unit_factors->F_f;

                                            Ss = fabs((Vy / Asy) / unit_factors->S_f);

                                            SsM = (VyM / Asy) / unit_factors->S_f;
                                            Ssm = (Vym / Asy) / unit_factors->S_f;

                                            x = dr[inx].x;

                                            x /= units_factor;

                                            x11 = Le.x1 + jednostkiOb(x) * koc;
                                            y11 = Le.y1 + jednostkiOb(x) * kos;

                                            Rotate_Point(kos1, koc1, x11, y11, x11 + fabs(Ss / s_magnitude), y11, &xdsp,
                                                         &ydsp);

                                            if (new_line) {
                                                Ldsp.x1 = xdsp;
                                                Ldsp.y1 = ydsp;
                                                Ldsp.blok = 1;
                                                new_line = FALSE;
                                                Ldsp_.x1 = x11;
                                                Ldsp_.y1 = y11;
                                                Ldsp_.x2 = Ldsp.x1;
                                                Ldsp_.y2 = Ldsp.y1;
                                                Ldsp_.blok = 1;
                                                ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                if (!Check_if_Equal(Ss, 0.0) &&
                                                    (((Check_if_Equal(Ss, SsM) || Check_if_Equal(Ss, Ssm)) ||
                                                      (Check_if_Equal(x, 0.0) ||
                                                       Check_if_Equal(jednostkiOb(x), PL.dl)))) ||
                                                    ((Check_if_Equal2(dr[inx].x, lb)) ||
                                                     (Check_if_Equal2(dr[inx].x, le)))) {
                                                    if (!Check_if_Equal(Ss, Ss0))
                                                        draw_label(&Ldsp_, &Le, x, r1, r2, fabs(Ss), stress_precision,
                                                                   FALSE);
                                                    Ss0 = Ss;
                                                }

                                            } else {
                                                Ldsp.x2 = xdsp;
                                                Ldsp.y2 = ydsp;
                                                Ldsp.blok = 1;

                                                Ldsp_.x1 = x11;
                                                Ldsp_.y1 = y11;
                                                Ldsp_.x2 = Ldsp.x2;
                                                Ldsp_.y2 = Ldsp.y2;
                                                Ldsp_.blok = 1;
                                                ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Ldsp_);
                                                if (!Check_if_Equal(Ss, 0.0) &&
                                                    (((Check_if_Equal(Ss, SsM) || Check_if_Equal(Ss, Ssm)) ||
                                                      (Check_if_Equal(x, 0.0) ||
                                                       Check_if_Equal(jednostkiOb(x), PL.dl)))) ||
                                                    ((Check_if_Equal2(dr[inx].x, lb)) ||
                                                     (Check_if_Equal2(dr[inx].x, le)))) {
                                                    if (!Check_if_Equal(Ss, Ss0))
                                                        draw_label(&Ldsp_, &Le, x, r1, r2, fabs(Ss), stress_precision,
                                                                   FALSE);
                                                    Ss0 = Ss;
                                                }

                                                if (dodaj_obiekt((BLOK *) dane, (void *) &Ldsp) == NULL) {
                                                    fclose(f);
                                                    ret = ask_question(1, "", (char *) confirm, "",
                                                                       (char *) _CANNOT_CREATE_MOMENT_BLOCK_, 12, "",
                                                                       11, 1,
                                                                       62);

                                                    no_error=FALSE;
                                                    goto error;
                                                }

                                                Ldsp.x1 = Ldsp.x2;
                                                Ldsp.y1 = Ldsp.y2;

                                            }
                                        }
                                    }
                                }
                            }
                        }
                        free(dr);
                    }
                    if (rout) printf("%s", report_row);
                }
                fclose(f);
            }
            //////////  DYNAMIC

            if (st_dynamic_no > 0)
            {

                float curviness = 0.75;

                Sdsp.npts = 5;
                Sdsp.multiple = 1;  //26 frames
                Sdsp.lp = 11 * 2;

                Sdsp.xy[Sdsp.lp] = curviness;
                Sdsp.xy[Sdsp.lp + 1] = 0;  //reserve

                Sdsp.obiekt = Ospline;

                Sdsp.n = sizeof(NAGLOWEK) + (Sdsp.lp * 26 + 2) * sizeof(float);  //TEMPORARY

                Sdsp.kolor = static_colors.dynamic_color;
                Sdsp.typ = 64;
                Sdsp.blok = ElemBlok;

                sprintf(desired_layer, "%s_%s", ptr_id, "dynamic");

                for (ii = 0; ii < No_Layers; ii++) {
                    if (strcmp(Layers[ii].name, desired_layer) == 0)
                        break;
                }
                if (ii < No_Layers) {
                    desired_layer_no = ii;
                    delete_all_from_layer_atrybut(desired_layer_no, ANieOkreslony);
                } else {
                    if (No_Layers == MAX_NUMBER_OF_LAYERS - 1) {
                        ret = ask_question(1, "", (char *) confirm, "",
                                           (char *) _CANNOT_CREATE_NEW_LAYER_, 12, "", 11,
                                           1, 62);
                        no_error=FALSE;
                        goto error;
                    }

                    No_Layers++;
                    desired_layer_no = No_Layers - 1;
                    Layers[No_Layers - 1].on = 1;
                    Layers[No_Layers - 1].edit = 1;
                    Layers[No_Layers - 1].point = 1;
                    strncpy(Layers[No_Layers - 1].name, desired_layer, 64);
                    Layers[No_Layers - 1].color = Sdsp.kolor;

                }

                Sdsp.warstwa = desired_layer_no;

                sprintf(params, "alfacad.add");
                f = fopen(params, "rt");
                if (f == NULL) {

                    ret = ask_question(1, "", (char *) confirm, "",
                                       (char *) _CANNOT_OPEN_DYNAMIC_RESULTS_DATA_FILE_, 12, "", 11,
                                       1, 62);
                    no_error=FALSE;
                    goto error;
                }

                char *ptr_modes_no;
                char *ptr_ex;
                char *ptr_mode;
                char *ptr_element;
                char *ptr_frame;

                int modes_no = 0;
                int mode = 0;
                float mode_ex = 1.0;
                char mode_str[128];
                char mode_ex_str[128];
                int element = 0;
                int frame = 0;
                int index = 0;

                double xdisp, ydisp, zdisp;

                while (fgets(report_row, MaxTextLen, f)) {
                    if (report_row[0] == '#') continue;
                    if (strlen(report_row) == 0) continue;
                    ptr_modes_no = strstr(report_row, "%MODES NUMBER");
                    ptr_ex = strstr(report_row, "%EXAGGERATION");
                    ptr_mode = strstr(report_row, "%MODE");
                    ptr_element = strstr(report_row, "%element");
                    ptr_frame = strstr(report_row, "%frame");
                    if (ptr_modes_no) {
                        ret = sscanf(ptr_modes_no + 14, "%d", &modes_no);
                    } else if (ptr_ex) {
                        ret = sscanf(ptr_ex + 14, "%f", &mode_ex);
                    } else if (ptr_mode) {
                        if (element > 0) {
                            ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Sdsp);
                            index = 0;
                        }

                        element = 0;

                        strncpy(&mode_str, ptr_mode + 1, 63);
                        mode_str[64] = '\0';
                        if (mode_str[strlen(mode_str) - 1] == '\n') mode_str[strlen(mode_str) - 1] = '\0';

                        mode++;

                        sprintf(mode_ex_str, "%s-%d x%.0f", ptr_id_short, mode, mode_ex);
                        strcat(mode_str, mode_ex_str);

                        if (mode == 1) hiding = 1;  //first mode is also invisible, to not disturb animations
                        else hiding = 1;

                        ptr_block = add_block(st_node[st_element[0].node1].x, st_node[st_element[0].node1].y, 'E',
                                              mode_str, hiding);
                        if (ptr_block == NULL) {
                            fclose(f);
                            ret = ask_question(1, "", (char *) confirm, "",
                                               (char *) _CANNOT_CREATE_SHEAR_STRESS_BLOCK_, 12, "", 11, 1, 62);
                            no_error=FALSE;
                            goto error;
                        }

                        index = 0;

                        //creating new block
                    } else if (ptr_element) {
                        //adding current element
                        if (element > 0) {
                            ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Sdsp);
                        }

                        ret = sscanf(ptr_element + 8, "%d", &element);

                        index = 0;
                        //creating new element
                    } else if (ptr_frame) {
                        ret = sscanf(ptr_frame + 6, "%d", &frame);
                        //if frame ==0 creating block
                        if (frame == 0) {
                            index = 0;
                        } else if (frame == 1)  //space for curviness
                        {

                            if (index != 22) {
                                int a = 0;
                            }

                            Sdsp.xy[index++] = curviness;
                            Sdsp.xy[index++] = 0;  //reserve
                        }
                    } else {
                        ret = sscanf(report_row, "%lf %lf %lf", &xdsp, &ydsp, &zdsp);
                        if ((ret == 3) && (frame < 26)) {
                            //adding control points;
                            xdsp /= units_factor;
                            ydsp /= units_factor;

                            if (index > 574) {
                                int a = 0;
                            }

                            Sdsp.xy[index++] = jednostkiOb(xdsp) + x0;
                            Sdsp.xy[index++] = jednostkiOb(ydsp) + y0;
                        }
                    }

                }

                //the last element - no MODE anymore
                if (element > 0) {
                    ptr_l = dodaj_obiekt((BLOK *) dane, (void *) &Sdsp);
                    index = 0;
                }

                fclose(f);
            }
            /////////
        }
        else no_error=FALSE;

    }

    for (i = 0; i < combinations_number; i++) {
        sprintf(params, "alfacad.if%02d", i + 1);
        if (__file_exists(params)) unlink(params);
    }

    error:

    zmien_atrybut_undo(dane, dane + dane_size);

    blokzap(dane, dane + dane_size, Ablok, COPY_PUT, 1);
    zmien_atrybut(dane, dane + dane_size, Ablok, Aoblok);

    free(st_property);
    free(st_element);
    free(st_reaction);
    free(combi_reactions);
    free(combi_reactions_table);

    for (i = 0; i < rep_element_no; i++) {
        free(combi_element_uls[i].fd);
        free(combi_element_sls[i].fd);
        free(combi_element_qpsls[i].fd);
    }

    free(combi_element_uls);
    free(combi_element_sls);
    free(combi_element_qpsls);

    free(st_node);

    redraw();

    //showing text file


    char *okular_="okular";
    char *org_kde_okular_="org.kde.okular";
    char *okular;

    int pdf_viewed = 0;

    if (no_error)
    {
        if (text2PDF("alfacad.out", "alfastatic.pdf")) {
            //checking okular
            FILE *pp;
            int pdf_view = 0;
#ifdef LINUX
            sprintf(params,"%s --version", org_kde_okular_);
            pp = popen(params, "r");
            if (pp != NULL) {
                while (1) {
                    char *line;
                    char buf[1000];
                    line = fgets(buf, sizeof(buf), pp);
                    if (line == NULL) break;
                    if ((strstr(strlwr(line), "okular") != NULL) && (strstr(strlwr(line), "not found") == NULL)) {
                        pdf_view = 1;
                        okular=org_kde_okular_;
                        break;
                    }
                }
                pclose(pp);
            }


            if (pdf_view == 0) {
                sprintf(params,"%s --version", okular_);
                pp = popen(params, "r");
                if (pp != NULL) {
                    while (1) {
                        char *line;
                        char buf[1000];
                        line = fgets(buf, sizeof(buf), pp);
                        if (line == NULL) break;
                        if ((strstr(strlwr(line), "okular") != NULL) && (strstr(strlwr(line), "not found") == NULL)) {
                            pdf_view = 1;
                            okular=okular_;
                            break;
                        }
                    }
                    pclose(pp);
                }
            }

            if (pdf_view == 0) {
                pp = popen("evince --version", "r");
                if (pp != NULL) {
                    while (1) {
                        char *line;
                        char buf[1000];
                        line = fgets(buf, sizeof(buf), pp);
                        if (line == NULL) break;
                        if ((strstr(strlwr(line), "gnome") != NULL) && (strstr(strlwr(line), "not found") == NULL)) {
                            pdf_view = 2;
                            break;
                        }
                    }
                    pclose(pp);
                }
            }
            if (pdf_view == 0) {
                pp = popen("ls ./mupdf-x11", "r");
                if (pp != NULL) {
                    while (1) {
                        char *line;
                        char buf[1000];
                        line = fgets(buf, sizeof(buf), pp);
                        if (line == NULL) break;
                        if ((strstr(strlwr(line), "./mupdf") != NULL) &&
                            (strstr(strlwr(line), "no such file") == NULL)) {
                            pdf_view = 3;
                            break;
                        }
                    }
                    pclose(pp);
                }
            }
            if (pdf_view == 0) {
                ret = ask_question(1, "", (char *) confirm, "", (char *) _CANNOT_OPEN_RESULTS_PDF_FILE_, 12,
                                   _INSTALL_PDF_VIEWER_, 11, 1, 62);
            } else {
                if (pdf_view == 3)
                    ret = ask_question(1, "", (char *) confirm, "", "", 12, _INSTALL_PDF_VIEWER_, 11, 1, 61);

                sprintf(params, "alfastatic.pdf &");

                if (pdf_view == 1) runcode = SystemSilent(okular, params);
                else if (pdf_view == 2) runcode = SystemSilent("evince", params);
                else runcode = SystemSilent("./mupdf-x11 -I -A 8 -r 150", params);
                printf("%d", runcode);

                ret = Expand_flex();
                simulate_keypress(14592);
                simulate_keypress(13);

                runcode = 255;

                pdf_viewed = 1;
            }
#endif
#ifndef LINUX
            //AllocConsole();
//ShowWindow(GetConsoleWindow(), SW_HIDE);
get_console();

pp = _popen("where okular", "r");
if (pp != NULL)
{
while (1)
{
char* line;
char buf[1000];
line = fgets(buf, sizeof(buf), pp);
if (line == NULL) break;
if ((strstr(strlwr(line), "okular.exe") != NULL) && (strstr(strlwr(line), "not find") == NULL))
{
pdf_view = 1;
break;
}
//if (line[0] == 'd') printf("%s\n", line); // line includes '\n'
}
_pclose(pp);
}

/*
You can permanently add a path to PATH with the setx command:
setx /M path "%path%;C:\your\path\here\"

for example:
setx /M path "%path%;C:\Program Files\Scribus 1.6.2\"

it must be done as administrator

Remove the /M flag if you want to set the user PATH instead of the system PATH.
*/

if (pdf_view == 0)
{
//pp = _popen("dir %userprofile%\\Desktop\\evince.lnk", "r");
pp = _popen("IF EXIST %userprofile%\\Desktop\\evince.lnk ECHO exists", "r");
if (pp != NULL)
{
while (1)
{
char* line;
char buf[1000];
line = fgets(buf, sizeof(buf), pp);
if (line == NULL) break;
if (strstr(strlwr(line), "exists") != NULL)
{
pdf_view = 2;
break;
}
//if (line[0] == 'd') printf("%s\n", line); /* line includes '\n' */
}
_pclose(pp);
}
}
if (pdf_view == 0)
{
#ifdef BIT64
pp = _popen("dir sumatrapdf64.exe", "r");
#else
pp = _popen("dir sumatrapdf32.exe", "r");
#endif
if (pp != NULL)
{
while (1)
{
char* line;
char buf[1000];
line = fgets(buf, sizeof(buf), pp);
if (line == NULL) break;
#ifdef BIT64
if ((strstr(strlwr(line), "sumatrapdf64.exe") != NULL) && (strstr(strlwr(line), "file not found") == NULL))
#endif
#ifndef BIT64
if ((strstr(strlwr(line), "sumatrapdf32.exe") != NULL) && (strstr(strlwr(line), "file not found") == NULL))
#endif
{
pdf_view = 3;
break;
}
//if (line[0] == 'd') printf("%s\n", line); /* line includes '\n' */
}
_pclose(pp);
}
}
//FreeConsole();
free_console();

if (pdf_view==0)
{
ret = ask_question(1, "", (char *) confirm, "", (char *) _CANNOT_OPEN_RESULTS_PDF_FILE_, 12, _INSTALL_PDF_VIEWER_, 11, 1, 62);
}
else
{
sprintf(params, "alfastatic.pdf &");
if (pdf_view==1) runcode = RunSilent("okular", params);
else if (pdf_view==2) runcode = RunSilent("%userprofile%\\Desktop\\evince.lnk", params);
else
#ifdef BIT64
runcode = RunSilent("sumatrapdf64 -new-window", params);
#endif
#ifndef BIT64
runcode = RunSilent("sumatrapdf32 -new-window", params);
#endif
printf("%d", runcode);

simulate_keypress(14592);

runcode = 255;

pdf_viewed=1;
}
#endif
        } else {
            ret = ask_question(1, "", (char *) confirm, "", (char *) _CANNOT_CREATE_RESULTS_PDF_FILE_, 12, "", 11, 1, 0/*62*/);
        }


        if ((no_error) && (pdf_viewed == 0)) {
#ifdef LINUX
            sprintf(params, "--textbox alfacad.out");
            runcode = SystemSilent("./kdialog4alfa", params);
            printf("%d", runcode);

            FreeMouse();
            LockMouse();
#endif
#ifndef LINUX
            sprintf(params, "alfacad.out");
            runcode = RunSilent("notepad", params);
            printf("%d", runcode);

            runcode = 255;
#endif
        }


        if (runcode == 0) {
            char sk[MAXPATH] = "";
            if (Load_File(&sk, ZAPIS_OUT, TRUE) == FALSE) {
                return;
            }
            copy("alfacad.out", sk);
        }
    }

    return;
}


#undef __O_STATIC__

