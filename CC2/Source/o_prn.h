#define MaxComandLen 16

#define NO_REFLECTION FALSE

typedef struct
{
    char line_feed_n216 [MaxComandLen];	/*ESC "J" CHR$(n)*/
    int line_feed_specjal ;
    int vertical_density_EPSON ;
    int vertical_density_IBM ;
    int vertical_density_high_EPSON ;
    int vertical_density_high_IBM ;
	int vertical_density_super_high_EPSON;
	int vertical_density_super_high_IBM;
    char mode_draft [MaxComandLen];
    int  density_draft;
    int  density_draft_IBM;
    char mode_normal [MaxComandLen];
    int  density_normal;
    int  density_normal_IBM;
    char mode_double [MaxComandLen];
    int  density_double;
    int  density_double_IBM;
    char mode_high [MaxComandLen];
    int  mode_high_IBM;
	char mode_super_high[MaxComandLen];
	int  mode_super_high_IBM;
    int  density_high;
    int  density_high_IBM;
	int  density_super_high;
	int  density_super_high_IBM;

    float l_luk;		/*dlugosc odcinka prostego luku w milimetrach*/
    float dot_width;		/*srednica punktu (grubosc igly)*/
} T_Prn_Config, *T_PTR_Prn_Config ;

typedef struct		/*dane ustawiane przed drukowaniem*/
{
    int prn_type;		/*typ drukarki- 9, 24, 48, laser*/
    int prn_mode ;		/*tryb drukarki - EPSON, IBM*/
    BOOL condensed;	/*kondensacja do rozmiarow papieru, wydruk  w skali*/
    int density;	/*gestosc drukowania*/
    BOOL all_sheet;	/*caly rysunek, okno*/

	double xp; /* okno do plotowania*/
	double yp;
	double xk;
	double yk;
	double yk0; /* for TTF use in inversion*/
	double xp_w; 
	double yp_w;
	double xk_w;
	double yk_w; /* wewnetrzne okno do plotowania*/
	
    BOOL rotation;       /*obrot rysunku 1 - tak, 0 - nie*/
    BOOL inversion;
	BOOL reflection;
    BOOL out_to_file;	/*wydruk na drukarke czy do zbioru*/
    int first_page ;    /*numer pierwszej strony do wydruku*/
    double margin;      /*zakladka*/
    double line_thinest_width;	/*grubosc w jednostkach rysunku linii bardzo cienkiej na wydruku*/
    double line_thin_width;	/*grubosc w jednostkach rysunku linii cienkiej na wydruku*/
    double line_norm_width;	/*w pliku konfiguracyjnym grubosci podane sa*/
    double line_thick_width;    /*w milimetrach na papierze*/
    double line_thickest_width;
    double top_margin ;         /*gorny margines*/
	double bottom_margin;         /*dolny margines*/
    double left_margin ;        /*lewy margines*/
	double right_margin;        /*prawy margines*/
    double width_paper ;        /*szerokosc wydruku*/
    double height_paper ;       /*wysokosc wydruku*/
    double scale ;              /*skala dla wydruku w skali*/
	BOOL actual_size;
    BOOL print_pause ;         /*pauza na zmiane arkusza*/
    BOOL change_page ;         /*zmiana strony*/
    BOOL color_print ;         /*wydruk w kolorze*/
    BOOL PCX_gray ;            /*wydruk PCX w odcieniach szarosci*/
	BOOL gray_print;           /*gray*/
    int color_mode ;           /*tryb koloru KCMY, YMCK*/
    BOOL set_page ;            /*ustaw dlugosc strony*/
    BOOL print_window ;        /*wydruk okna*/
    BOOL print_sheet ;          /*wydruk arkusza*/
    char *sheet_adr ;            /*adres bloku arkusza*/
    int  port_num ;             /*numer portu*/
    BOOL text_mode ;            /*tryb przelaczania na tryb tekstowy w czasie wydruku*/
    BOOL buforowanie ;
    BOOL kompresja ;
    BOOL ploter ;
	BOOL oce;
    BOOL fixed_name ;
    char plik [MAXPATH] ;
    int copies;
    int thin_point_PCX;
	double line_thick_correction_H;
	double line_thick_correction_V;
	int roll;
	int WINPRINT_DEF;
	char image_format[16];
	int image_format_i;
	BOOL bitmap_only;
	BOOL import_png;
} T_Prn_Ini_Date_org, *T_PTR_Prn_Ini_Date_org ;

typedef struct		/*dane ustawiane przed drukowaniem*/
{
	int prn_type;		/*typ drukarki- 9, 24, 48, laser*/
	int prn_mode;		/*tryb drukarki - EPSON, IBM*/
	BOOL condensed;	/*kondensacja do rozmiarow papieru, wydruk  w skali*/
	int density;	/*gestosc drukowania*/
	BOOL all_sheet;	/*caly rysunek, okno*/

	float xp; // okno do plotowania
	float yp;
	float xk;
	float yk;
	float yk0; // for TTF use in inversion
	float xp_w;
	float yp_w;
	float xk_w;
	float yk_w; // wewnetrzne okno do plotowania
    
	BOOL rotation;       /*obrot rysunku 1 - tak, 0 - nie*/
	BOOL inversion;
	BOOL reflection;
	BOOL out_to_file;	/*wydruk na drukarke czy do zbioru*/
	int first_page;    /*numer pierwszej strony do wydruku*/
	float margin;      /*zakladka*/
	float line_thinest_width;	/*grubosc w jednostkach rysunku linii bardzo cienkiej na wydruku*/
	float line_thin_width;	/*grubosc w jednostkach rysunku linii cienkiej na wydruku*/
	float line_norm_width;	/*w pliku konfiguracyjnym grubosci podane sa*/
	float line_thick_width;    /*w milimetrach na papierze*/
	float line_thickest_width;
	float top_margin;         /*gorny margines*/
	float bottom_margin;         /*dolny margines*/
	float left_margin;        /*lewy margines*/
	float right_margin;        /*prawy margines*/
	float width_paper;        /*szerokosc wydruku*/
	float height_paper;       /*wysokosc wydruku*/
	float prn_width_paper;        /*szerokosc wydruku drukarki*/
	float prn_height_paper;       /*wysokosc wydruku drukarki*/
	float scale;              /*skala dla wydruku w skali*/
	BOOL actual_size;         /*actual scale*/
	BOOL print_pause;         /*pauza na zmiane arkusza*/
	BOOL change_page;         /*zmiana strony*/
	BOOL color_print;         /*wydruk w kolorze*/
	BOOL PCX_gray;            /*wydruk PCX w odcieniach szarosci*/
	BOOL gray_print;           /*gray*/
	int color_mode;           /*tryb koloru KCMY, YMCK*/
	BOOL set_page;            /*ustaw dlugosc strony*/
	BOOL print_window;        /*wydruk okna*/
	BOOL print_sheet;          /*wydruk arkusza*/
	char *sheet_adr;            /*adres bloku arkusza*/
	int  port_num;             /*numer portu*/
	BOOL text_mode;            /*tryb przelaczania na tryb tekstowy w czasie wydruku*/
	BOOL buforowanie;
	BOOL kompresja;
	BOOL ploter;
	BOOL oce;
	BOOL fixed_name;
	char plik[MAXPATH];
	int copies;
	int thin_point_PCX;
	float line_thick_correction_H;
	float line_thick_correction_V;
	int roll;
	int WINPRINT_DEF;
	char image_format[16];
	int image_format_i;
	BOOL bitmap_only;
	BOOL import_png;
	BOOL print_translucent_solids;
	int gray_saturation;
	BOOL background;
    char printer[34];
} T_Prn_Ini_Date, *T_PTR_Prn_Ini_Date;

#ifdef __cplusplus
extern "C" {
#endif

extern int Prn_Draw_Param (char *, T_PTR_Prn_Config *, T_PTR_Prn_Ini_Date *, float prn_width_paper, float prn_height_paper, int serial_mode) ;
extern BOOL Draw_to_Prn_Proc (char *, T_PTR_Prn_Config, T_PTR_Prn_Ini_Date, int init_prn_dlg, int serial_mode) ;

#ifdef __cplusplus
}
#endif
