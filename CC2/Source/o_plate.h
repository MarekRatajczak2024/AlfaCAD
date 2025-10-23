
// Created by marek on 9/4/25.
//

#ifndef ALFACAD_O_PLATE_H
#define ALFACAD_O_PLATE_H

extern void Plate_analysis(void);
extern void redcrsb(char typ, int n);
extern void select_blok(void);
extern void select_blok_items(int items);

typedef struct
{
    double x;
    double y;
    float d;
} PL_NODE;

typedef struct
{
    double x;
    double y;
    float d;
    int body;  //number of 0 plate or 1 zone
    int body_no;  //number of plate or zone
    int emb_no;
} PL_NODE_EMB;

typedef struct
{
    int style;  //0 plate, 1 hole, 2 wall, 3 zone
    int number;
    int body_number;
    int node1;
    int node2;
    int node3;
    int type;  //0 line, 1 arc
    int inverted; //0 regular, 1 inverted
    int restraint;  //0 or 5 free, 6 simple support, 7 fixed
    int k;
} PL_EDGE;

typedef struct
{
    AVECTOR *adr;
    int body;     //number of 0 plate or 1 zone
    int body_no;  //number of plate or zone
    int load;     //0 undefined, 1 dead, 2 live, 3 live roof load, 4 wind, 5 snow, 6 seismic, 7 rainwater load or ice water load, 8 hydraulic loads from soil, 9  F = hydraulic loads from fluids
    int variant;  //0 undefined, 1..255 number of load character with different factors
    int flags;    //0  regular   1 inverted
    int factor_record;
    int take_it;
    int type;     //0 uniformly distributed load, 1 concentrated force load
    float x1;
    float y1;
    float x2;
    float y2;
    double magnitude1;
    double magnitude2;
} PL_LOAD;

typedef struct
{
    BLOK *adr;
    int first_edge;
    int last_edge;
    int property_number;
    int k;  //body number - 1
    int ps;  //parent style  0 plate, 1 hole, 2 wall, 3 zone, initiated as -1
    int pn;  //parent number initiated as - 1
    int load;
} PLATE_PROPERTY;


typedef struct
{
    int number;
    int flag;
    double x;
    double y;
} MESH_NODE;

typedef struct
{
    int number;
    int body_number;
    int type;
    int node1;
    int node2;
    int node3;
    int node4;
} MESH_ELEMENT;


typedef struct
{
    int n1;
    int n2;
    int n3;
    int n4;
    int n5;
    int n6;
    int n7;
} MESH_BOUNDARY;

typedef struct
{
    int n1;
    int n2;
    LINIA L;
} GEO_LINE;

typedef struct
{
    int n;
    int i;
} PERM;

/*
typedef struct
{
    int el_number;
    float f1;
    float f2;
    float f3;
    float f4;
} FE_DATA;
 */
//Finite Element Data
//parameter and component //deflection 1..6, stress_xx, yy, etc, epsilon.tot.xx, yy etc
//combintion ULS, SLS, CULS, CSLS
//are taken from the blok name

//typedef struct
//{
//    double u;
//} DEFLECTION;


/*
typedef struct {              //                                                      SI                IMP
    double h_f; //depth of cross section in y axis                                   mm                  in
    double hm_f; //depth of cross section in y axis                                  mm->m               in
    double A_f; //area in mm^2 of cross section                                      cm^2 -> mm^2        in^2
    double Am_f; //area in m^2 of cross section                                      mm^2 -> m^2         in^2
    double I_f; //aread moment of inertia                                            cm^4 -> mm^4        in^4
    double Wm_f; //elastic section modulus in m^3
    double Wmm_f; //elastic section modulus in m^3      //                                 cm^3 -> m^3         in^3
    double E_f; //Young's Modulus of elasticity and Shear modulus of elasticity      GPa -> MPa          ksi
    double d_f; //mass density                                                       kg/m^3 -> t/m^3     kip/in^3/g
    double a_f; //Coefficient of thermal expansion                                   /C                  /F
    double g_f; //gravitational acceleration                                         m/s^2 -> mm/s^2     in/s^2
    double F_f; //forces                                                             kN -> N             kip
    double M_f; //moments                                                            kNm -> Nmm          kip*in
    double q_f; //load                                                               kN/m -> N/mm        kip/in
    double R_f; //Reactions  (out)                                                   t -> kN             kip
    double S_f; //Stress  (out)                                                      kPa -> Mpa          kip
    double Ma_f;//Mass                                                               kPa -> tone         kip
    double c_f; //concrete cover  should be no less than 35 mm / 1.5" in for beams and 55 mm / 2"  for columns
} UNIT_FACTORS;
*/
/*
typedef struct {
    double dim_precision;
    double A_precision;
    double J_precision;
    double I_precision;
    double E_precision;
    double G_precision;
    double p_precision;
    double d_precision;
    double a_precision;
} PROP_PRECISIONS;
 */
/*
typedef struct
{
    unsigned char load;  //0 undefined, 1 dead, 2 live, 3 live roof load, 4 wind, 5 snow, 6 seismic, 7 rainwater load or ice water load, 8 hydraulic loads from soil, 9  F = hydraulic loads from fluids
    unsigned char variant;  //0 undefined, 1..255 number of load character with different factors
    double gamma;
    double psi0;
    double psi1;
    double psi2;
    double xi;  //reduction factor for dead load
    double gamma_inf;
} ST_LOAD_FACTORS;
*/

/*
typedef struct
{
    double gamma_g;
    double psi0;
    double psi1;
    double psi2;
    double xi_g;
    double gamma_gi;
} GAMMAS;
*/
/*
typedef struct
{
    int n;     //Property number
    double h;  //Cross-sectional depth in y axis in mm or in
    double b;  //Cross-sectional depth in z axis  in mm or in
    double A;  //Cross-sectional area of a prismatic frame element in mm^2 or in^2
    double Asy; //Shear area in the local y-axis of a prismatic frame element in mm^2 or in^2
    double Asz; //Shear area in the local y-axis of a prismatic frame element in mm^2 or in^2
    double Jx; //Torsional moment of inertia of a frame element in mm^4
    double Iy; //Moment of inertia for bending about the local y axis in mm^4
    double Iz; //Moment of inertia for bending about the local z axis in mm^4
    double Wy; //Elastic section modulus about y-axis in m^3  or in^3
    double Wz; //Elastic section modulus about z-axis  in m^3 or in^3
    double E;  //Modulus of elasticity of a frame element in
    double G;  //Shear modulus of elasticity of frame element
    double r;  //the roll angle of the frame element, in degrees
    double d;  //mass density of a frame element (using consistent units)
    double a;  //Coefficient of thermal expansion (1/degree)
    double gamma;  //partial factor for permanent load if gravity accelerration is defined
    double xi;  //reduction factor for permanent load if gravity accelerration is defined
    double gamma_inf;  //partial factor for permanent load (lower value) if gravity accelerration is defined
    int RC_flag; //0 - steel or wood, >0 Reinforced Concrete
    double c;  //concrete cover as a distance from bottom/top concrete edge to the centroid of bottom/top reinforcement layer or the centroid of all bottom/top reinforcement layers
    double zeta; //parameter dependent on the share of concrete in transferring the bending moment. In preliminary dimensioning, ζ=0.85 can be assumed
    double fyd; //Design yield strength of reinforcing steel fyd=fyk/γs where fyk characteristic yield strength of reinforcing steel; γs partial safety factor for reinforcing steel
    //fyd must be indicated in properties for RC element, in Mpa (e.g. 350) or in kpsi (e.g. 50)
    //According to Eurocode 2, the characteristic yield strength fyk of steel reinforcement in reinforced concrete should be between 400 and 600 MPa.
    //The partial safety factor for rebar in Eurocodes is γs = 1.15 for reinforcement and γc = 1.5 for concrete
    int ok;
} ST_PROPERTY;
*/
#endif //ALFACAD_O_PLATE_H
