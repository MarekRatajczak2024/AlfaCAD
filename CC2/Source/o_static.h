
#ifndef LIBFUN_PL_H_O_STATIC_H
#define LIBFUN_PL_H_O_STATIC_H

extern void Static_analysis(void);
extern void Cross_section_forces(void);

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

typedef struct
{
    double x;
    double y;
    double radius;
    int real;
} ST_NODE;

typedef struct
{
    double x;
    double y;
    double radius;
    int flag;
} ST_NODE_PRE;

typedef struct
{
    int node;
    int active;
    int x;
    int y;
    int z;
    int xx;
    int yy;
    int zz;
} ST_REACTION;

typedef struct
{
    int node1;
    int node2;
    int node1r;
    int node2r;
    int property_no;
    double length;
} ST_ELEMENT;

typedef struct
{
    int layer;
    int node;
    double fx;
    double fy;
    double mz;
    int factor_record;
    int take_it;
} ST_NODE_FORCE_MOMENT;

typedef struct
{
    int layer;
    int element;
    double qx;
    double qy;
    int factor_record;
    int take_it;
    int flag;
} ST_UNIFORM_LOAD;

typedef struct
{
    int layer;
    int element;
    double dlxx1;
    double dlxx2;
    double qx1;
    double qx2;
    double dlxy1;
    double dlxy2;
    double qy1;
    double qy2;
    int factor_record;
    int take_it;
} ST_TRAPEZOID_LOAD;

typedef struct
{
    int layer;
    int element;
    double fx;
    double fy;
    double dlx;
    int factor_record;
    int take_it;
} ST_ELEMENT_FORCE;

typedef struct
{
    int layer;
    int element;
    double ydepth;
    double deltatplus;
    double deltatminus;
    int factor_record;
    int take_it;
} ST_THERMAL_LOAD;

typedef struct
{
    int layer;
    int node;
    double dx;
    double dy;
    double dzz;
    int factor_record;
    int take_it;
} ST_DISPLACEMENT;

typedef struct
{
    int element;
    int reversed;
    int uniform;
    int spread;
    int partial;
    double magnitude1;
    double magnitude2;
    double distance1;
    double distance2;
    int factor_record;
    int take_it;
} ST_LOAD;

typedef struct
{
    int node;
    double mass;
} ST_MASS_NODE;

typedef struct
{
    int element;
    double mass;
} ST_MASS_ELEMENT;

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

typedef struct
{
    double gamma_g;
    double psi0;
    double psi1;
    double psi2;
    double xi_g;
    double gamma_gi;
} GAMMAS;

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

typedef struct {
   int mask;
   int flag;
} COMBINATION;


typedef struct {
    int number;
    int DL;
    int LL;
    int Lr;
    int WL;
    int SL;
    int EL;
    int RL;
    int HL;
    int FL;
    int TL;
}COMBI_FACTOR;

typedef struct
{
    COMBI_FACTOR factor[28];
} COMBI_FACTORS;

typedef struct
{
    int case_no;
    int combi;
    int combination;
    int first;
    int last;
} COMBI_TOTAL_NUMBER;

typedef struct {
    double Fx_min;
    double Fx_max;
    double Fy_min;
    double Fy_max;
    double Mzz_min;
    double Mzz_max;
} COMBI_REACION;

typedef struct {
    COMBI_REACION uls;
    COMBI_REACION sls;
    COMBI_REACION qpsls;
} COMBI_REACIONS;


typedef struct {
    double x;
    double Nx_min;
    double Nx_max;
    double Vy_min;
    double Vy_max;
    double Mz_min;
    double Mz_max;
    double Dx_min;
    double Dx_max;
    double Dy_min;
    double Dy_max;
    double Sp_min;
    double Sp_max;
    double Sm_min;
    double Sm_max;
} COMBI_FORCES;

typedef struct {
    int nx;
    double NxM;
    double Nxm;
    double VyM;
    double Vym;
    double MzM;
    double Mzm;
    double DxM;
    double Dxm;
    double DyM;
    double Dym;
    double SpM;
    double Spm;
    double SmM;
    double Smm;
    COMBI_FORCES *fd;
} COMBI_ELEMENT;

typedef struct {
  double x;
  double Nx;
  double Vy;
  double Vz;
  double Tx;
  double My;
  double Mz;
  double Dx;
  double Dy;
  double Dz;
  double Rx;
  double Sp;
  double Sm;
  double Ss;
} DATA_ROW;


#endif //LIBFUN_PL_H_O_STATIC_H
