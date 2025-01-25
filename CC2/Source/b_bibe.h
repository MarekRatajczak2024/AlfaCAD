#define Max_No_Spec 16
#define Max_No_Spec_copy 16
#define Max_Spec_Value_copy Max_No_Spec_copy
#define Max_Spec_Name_copy 32

typedef struct
{
  char sz_prot [MAXPATH] ;
  BOOL b_prot ;
}
T_Prototype, *T_PTR_Prototype ;

typedef struct
{
	char spec_value[Max_Spec_Value_copy];
	char spec_name[Max_Spec_Name_copy];
} T_spec_name_copy;

typedef struct
{
	int x0;
	int y0;
	int ww;
	int wh;
} Ctx_Par_copy;

typedef struct
{
    double xp;
    double yp;
    double xk;
    double yk;
} PRIV_PRN_WINDOW ;

typedef struct DRAWING_PARAMS
{
	double Xp;
	double Yp;
	double Xmin;
	double Ymin;
	double Xmax;
	double Ymax;
	double skala;
	double X_max;
	double Y_max;
	double X;
	double Y;
	double krok_s;
	double krok_g;
	int	grid_;
	int	snap_;
	int	Current_Layer;
	int	No_Layers;
	LAYER Layers[MAX_NUMBER_OF_LAYERS];
	int	FormatX;
	int	FormatY;
	double SkalaF;
	double DokladnoscF;
	double Jednostki;
	ZMIENNE	zmwym;
    int zmwym_fraction;
    int zmwym_reserve;
	int	orto;
	double df__Point_Size;
	int	b__Point_View;
	double df__Trace_Width;
	double df__Sketch_Dist_Max;
	double local_x;
	double local_y;
	double angle_l;
	int TRANSLUCENCY;
	SEKTORY_EXT sektory_arkusza_ext;
	int KursorS;
	BOOL ciagniecie_linii;
	BOOL rozcinanie_linii;
	BOOL ukrywanie_typu;
	BOOL type_view;
	BOOL swobodny_blok;
	BOOL swobodny_tekst;
	OPTIONS1 options1;
	SEL	sel;
	char *dane;
	long dane_size;
	T_spec_name_copy s__table_spec[Max_No_Spec];
	int	kolory_paper;
	TEXT TextG;
	BOOL Change;
	BOOL bitmap_exist;
	BOOL bitmap_png_exist;
    BOOL bitmap_vector_exist;
	BOOL bitmap_pattern_exist;
	BOOL solid_translucent_exist;
	BOOL bitmap_on_front_exist;

	unsigned long long imageBMPsize_used;
	BOOL ctx_created;
	int i_ctx_created;
	int i_ctx;
	BOOL ctx_ok;
	BOOL regen_ctx;
	Ctx_Par_copy ctx_par[32];
	char *ctx_bitmap[32];
	char zbior_danych[MAXPATH];
	T_change_param s__change_param;

    PRIV_PRN_WINDOW priv_prn_window;

    double depth_magnitude;
    double thermal_magnitude;
    double load_magnitude;
    double force_magnitude;
    double moment_magnitude;
    double displacement_magnitude;
    double rotation_magnitude;

    double thermal_precision;
    double load_precision;
    double force_precision;
    double moment_precision;
    double displacement_precision;
    double rotation_precision;
    double stress_precision;

    double n_magnitude;
    double v_magnitude;
    double m_magnitude;
    double d_magnitude;
    double r_magnitude;
    double rm_magnitude;
    double s_magnitude;
    double q_magnitude;
    double p_magnitude;

    STATIC_COLORS static_colors;
    STATIC_STRESS_COLORS static_stress_colors;

} DRAWING_PARAMS;


////DRAWING_PARAMS Drawing_Params[MAX_NUMBER_OF_WINDOWS];  //TO CHECK

#ifdef __cplusplus
extern "C" {
#endif

extern int GetColorAC (int) ;
extern void SetColorAC (int) ;
extern T_PTR_Prototype Get_Prototype (void) ;

#ifdef __cplusplus
}
#endif
