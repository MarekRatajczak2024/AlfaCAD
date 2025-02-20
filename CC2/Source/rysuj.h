/*dane globalne*/
#define big_pcx

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef BOOL
#define BOOL int
#endif

#define MaxNumberTextStyle 128

BOOL SCREEN_BY_SOLID_OVERRIDIND = TRUE; // FALSE;

int last_active_layer=-1;
int last_active_layer_arch=-1;
int error_layer=0;
BOOL force_redraw=FALSE;
int last_mouse_z=0;
BOOL exit_alf=FALSE;
int GFX_WIN=0;
int X_WIN_ORIG=0, Y_WIN_ORIG=0, WIN_WIDTH=800, WIN_HEIGHT=600;
char GFX_ID[5]="    ";
BOOL just_file=TRUE;
BOOL use_getopenfilename=TRUE;
BOOL window_was_resized=FALSE;
BOOL sleep_state=FALSE;
BOOL mouse_out = FALSE;
char HATCH_TEMP_FILE[13]="ALFHATCH.000";
char RYSUJ$1[13]="RYSUJ$.000";
int Number_of_clients=0;
int Client_number=0;
int Alf_window_number=0;
BOOL original_color_print;
BOOL normalize_text=TRUE;
char background_pcx_file[MAXPATH]="Background\\gregre.pcx";
WARSTWY_ON_PCX  Warstwy_On_PCX[256], Warstwy_On_Solids[256];
int aspect_ratio=0;
int start_disk=2;
int gfx_gd=7;
int gfx_x=800;
int gfx_y=600;
int c_depth=8;
BOOL SHEET_IS_PRINTED=FALSE;
BOOL ENABLE_FOUND=FALSE;
BOOL NULL_FOUND=FALSE;
BOOL PRINT_POLYGONE=FALSE;
double Underl_del=0.75;  //przesuniecie linii podkreslenia od tekstu

BOOL PCX_gray=FALSE;
BOOL PCX_printing=FALSE;
int licznik_redraw=0;
BOOL redraw_ctx=0;
BOOL regen_ctx=0;
double SkalaF_ctx=0;
double SkalaFA_ctx=0;
unsigned long_long Wielkosc_Plata=131072000L;
unsigned long_long imageBMPsize=0;
unsigned long_long imageBMPsize_used=0;
unsigned long_long imageBMPsize_total_used = 0;
char * imageBMPstart=NULL;
BOOL Break_Edit_Text=FALSE;
char Czcionka_Pulpitu[60];
int ttf_digits27_len;
int ttf_digits13_len;
float ttf_width_w;
char Desktop_Font_File[MAXPATH];
static char font_family_name[255];
unsigned long_long Bufor_Wydruku = 67108864; // 20000000;  //2097152;
unsigned long_long Bufor_PCX=2097152;
int Kolory_Paper=0;
BOOL select_no_edit_layers=0;
unsigned long_long TRIAL_dane_size=261120;
BOOL select_element_only=0;
int tab_dokladnosc[16] = {2,2,3,2,3,2,2,2,2,2,2,2,2,2,2,2};

double odleglosc_prec=0.01;   //[m]
double rzedna_i_prec=0.001;   //[m]
double rzedna_p_prec=0.001;   //[m]
double rzedna_k_prec=0.001;   //[m]
double rzedna_ti_prec=0.01;    //[m]
double rzedna_tp_prec=0.01;    //[m]
double spadek_prec=0.1;      //[%.]
double zaglebienie_prec=0.01;    //[m]
int odleglosc_poz;
int rzedna_i_poz;
int rzedna_p_poz;
int rzedna_k_poz;
int rzedna_ti_poz;
int rzedna_tp_poz;
int spadek_poz;
int zaglebienie_poz;

int odleglosc_poz=2;          //liczba pozycji dziesietnych - obliczana na podstawie dokladnosci.
int rzedna_i_poz=3;
int rzedna_p_poz=3;
int rzedna_ti_poz=2;
int rzedna_tp_poz=2;
int zaglebienie_poz=2;
int spadek_poz=1;
BOOL BLOK_SIEC = FALSE;
double null_var[32]={0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0};
double Last_GX, Last_GY;
int ggetmaxx=639;
int ggetmaxy=479;
BOOL deleted_pcx=FALSE;
BOOL deleted_on_front = FALSE;
BOOL deleted_TTF = FALSE;
BOOL deleted_pattern = FALSE;
BOOL bitmap_exist=FALSE;
BOOL bitmap_png_exist = FALSE;
BOOL bitmap_vector_exist = FALSE;
BOOL bitmap_pattern_exist = FALSE;
BOOL solid_translucent_exist = FALSE;
BOOL bitmap_on_front_exist = FALSE;
BOOL bitmap_view=TRUE;
#ifdef big_pcx
char *bufor_pcx;
#else
//char bufor_pcx[2001200];
char *bufor_pcx;
#endif
double area_df_x;
double area_df_y;
double counted_area0 = 0;
double counted_area = 0;
BOOL out_pole_ignore=FALSE;
BOOL force_dim_text=FALSE;
char *Dim_Text_Adr=NULL;
BOOL do_not_dim_DIM = 0;
float pan_dxy = 0.1;
int OBRYS_line_typ=96; //domyslny typ linii obrysu stropu

BOOL wymiarowanie_ok=FALSE;
int show_tools=0;
BOOL on_tools=FALSE;
A_WINDOW tools_window={0,0,0,0};
BOOL was_dialog=FALSE;
int now_is_dialog=0;
BOOL do_not_draw_cur=FALSE;
BOOL do_not_draw_keys = FALSE;
char *menu_address[10]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,};
int MP_SIZE=13;
int ED_INF_HEIGHT =  13;
int BAR_G=11;
int HEIGHT=12;
int WIDTH=8;
int SKALA=1;
int FONTNUMBER=0;
BOOL lock_functions=FALSE;
BOOL edit_functions = FALSE;
int font_handle, font_handle1;
int font_handle_tab[12], font_handle1_tab[12];
BOOL dynamic_menu=TRUE;
BOOL desktop_instruction=TRUE;
BOOL auto_korekta_blokow=FALSE;

SEKTORY sektory_arkusza = { 0,0,0,1,0,75 };
SEKTORY_EXT sektory_arkusza_ext = { 75,2.5,7,7,0,1,2,0,0,0};

char EXPLORER_EXE [MAXPATH] = "C:/WINDOWS/EXPLORER.EXE";

char log_dir [MAXPATH] = "LOG";

short int SolidHatch = 0;

short int SolidFillTyp = 0;
short int SolidHatchPattern = 0;
short int SolidPattern = 0;
short int TracePattern = 0;
short int SolidTranslucent = 0;
short int TraceTranslucent = 0;
short int SolidHatchTranslucent = 0;

unsigned char SolidTranslucency = 255;
unsigned char  TraceTranslucency = 255;
unsigned char  SolidHatchTranslucency = 255;
unsigned char FilledEllipseTranslucency = 255;

short int SolidPatternScale = 3;
short int SolidHatchPatternScale = 3;
short int TracePatternScale = 3;
short int SolidPatternAngle = 0;
short int SolidHatchPatternAngle = 0;
short int TracePatternAngle = 0;
short int SolidPatternDx = 0;
short int SolidPatternDy = 0;
short int SolidHatchPatternDx = 0;
short int SolidHatchPatternDy = 0;
short int TracePatternDx = 0;
short int TracePatternDy = 0;
char SolidPatternName[MAXPATH] = "";
char SolidHatchPatternName[MAXPATH] = "";
char TracePatternName[MAXPATH] = "";


BOOL change_color_wsp = FALSE;
BOOL auto_pan_block = FALSE;
BOOL auto_zoom_block = FALSE;
char global_kom [100] ="";
BOOL auto_tekst = FALSE;
int tekst_interwal = 1;
BOOL hold_scrsaver = FALSE;
BOOL print_inversion = FALSE;
BOOL print_rotation = FALSE;
LAYER *layers;
double last_X = 0;
double last_Y = 0;
BOOL first_shoot_biblio = TRUE;
BOOL draw_hatch_pattern = FALSE;
BOOL draw_hatch_image_pattern = FALSE;
char s_hatch_param_df_name [MAXPATH] = "////////";
double s_hatch_param_df_distans = 1;
double s_hatch_param_df_scale = 1;
double s_hatch_param_df_angle = 0;
BOOL blok_xorput = FALSE;
double DF_PRECISION = 1.0E+5;
BOOL b_first_open = TRUE;
unsigned long memory_size = 32000000L;
int killpline = 0;
int killblock = 0;
OPTIONS1 options1 = {0,0,0,0,0,0,0,0};
BOOL undo_avail = FALSE;
UNDO_TAB_REC UNDO_TAB [4] = {0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0};
UNDO_TAB_REC UNDO_REC = {0,0,0,0,0,0,0,0,0};  
UNDO_TAB_REC UNDO_REC_NULL = {0,0,0,0,0,0,0,0,0};                            
UNDO_TAB_REC UNDO_REC_255 = {0,0,0,0,0,0,0,0,255};
char Previous_File [MAXHISTORYFILES][MAXPATH] = { "","","","","","","","","", "","","","","","",""};
char Current_File_Directory [MAXPATH] = "";

BOOL d_t__pline = TRUE;
BOOL bspline__pline = TRUE;
BOOL ini_cursor = FALSE;
BOOL wez_warstwe_typ_kolor = FALSE;
int DYNAMIC_MOUSE = 0; /*dynamiczna mysz w menu*/
int SKOKM_VER = 24;
int SKOKM_HOR = 48;
BOOL text_mode1 = FALSE;
BOOL kasowanie_licznikow = FALSE;
char old_mask_file [MAXFILE + MAXEXT] = "";
char old_mask_blok [MAXFILE + MAXEXT] = "";
char old_mask_pcx  [MAXFILE + MAXEXT] = "";
char old_mask_pro  [MAXFILE + MAXEXT] = "";
char old_mask_geo  [MAXFILE + MAXEXT] = "";
BOOL skala_punktu = FALSE;
char argv0 [MAXPATH] = "";
int pznaki_aster3 = 0;
int code_page = 1250;  //Windows Latin2
int dxf_encoding = 1250;  //Acad14 for Windows

int return_back = 0;
char aktualny_plik[MAXPATH] = "BLOKI.CFG";
char poprzedni_plik[MAXPATH] = "BLOKI.CFG";
int aktualny_poz = 0;
int aktualny_foff = 0;
int poprzedni_poz = 0;
int poprzedni_foff = 0;
BOOL ciagniecie_linii = TRUE;
BOOL rozcinanie_linii = TRUE;
BOOL ukrywanie_typu = FALSE;
BOOL type_view = FALSE;
int dodatkowe_linie = 0;
int dodatkowe_linie_all = 0;
LINIA LINE_TAB[max_dodatkowe_linie + 2];
int ignoruj_aktywacja = 0;
int numer_zbioru_hierarchii = 0;
char nazwa_zbioru_hierarchii[256][MAXPATH];
float PIN_TAB_X[MaxPinCount+2];
float PIN_TAB_Y[MaxPinCount+2];
int PIN_COUNT=0;
BOOL ADD_NEW_PIN = FALSE;
char find_text[MaxTextLen]="";
char change_text_source[MaxTextLen]="";
char change_text_dest[MaxTextLen]="";
char *last_find_addr=NULL;
char *last_change_addr=NULL;
BOOL komunikaty_bloku = FALSE;
BOOL swobodny_blok = FALSE;
BOOL swobodny_tekst = FALSE;
int poziom_menu = 0;
char plt_file0[10]="";
long DelDaneSize=32768; //16384;
unsigned short dos_version=1792; /*DOS7.0=WINDOWS 95*/
int procesp=0;
ESTR eO,eL,el,elc,eTens,eA,eEA;       /* interface */
int selq=0;            /* interface */
KOLORY kolory=Defkolory;
KOLORY_ASTER kolorya=Defkolorya;  /*dla potrzeb astera*/
KOLORY_PROFILI koloryp=Defkoloryp;  /*dla potrzeb profili*/
KOLORY_STROPU kolorys=Defkolorys;  /*dla potrzeb stropow*/
KOLORY_SIECI koloryps=Defkoloryps; /*dla potrzeb sieci*/
volatile MVCURB mvcurb=Defmvcurb;
int snap_=0;            		/* interface */
int LiczbaTypowLinii=15;                 /* interface */
int LiczbaWzorcowLinii = 5;              /*interface*/
LTYPE TypyLinii[MaxLiczbaTypowLinii]=DefLineType;   /* interface */
int WhNumberTextStyle=1;
int WhNumberTextStyle0=1;
int WhNumberTextStyleDXF=0;
T_Font_Style s__Fonts_Style [MaxNumberTextStyle] ;
T_Font_Header *PTRS__Text_Style [128];  //[MaxNumberTextStyle] ;
T_Font_Header_DXF PTRS__Text_Style_DXF [MaxNumberTextStyleDXF] ;
T_Font_Header_DXF_TABLE PTRS__Text_Style_DXF_TABLE [MaxNumberTextStyleDXF] ;
TEXT TextG=Tdef;
LINIA LiniaG=Ldef;
LINIA L_axis=Ldef;
AVECTOR VectorG=Vdef;
OKRAG OkragG=Odef;
WIELOKAT SolidG = S4def;
LUK LukG=ldef;
T_Point PointG=PointDef;
B_PCX PcxG=PCXdef;
int poczL=0;
int (*TABFUN)(void)=NULL;   /*interface*/
int (*TABFUN1)(char)=NULL;   /*interface*/
int (*MEDITFUN)(void) = NULL;   /*interface*/
int Info=0;
BOOL Change = FALSE, Auto_Pan = TRUE, bRedrow = FALSE;
BOOL bMouse = FALSE;
int Orto_Dir = I_Orto_NoDir;
double df__Point_Size = 2.5 ; 	/*rozmiar punktu w jednostkach rysunku*/
BOOL b__Point_View = TRUE ;	/*widocznosc punktu*/
BOOL b__Control_Points_View = FALSE;
BOOL b__Bitmap_Outline = FALSE;
double df__Trace_Width = 1 ;    /*szerokosc sladu w jednostkach rysunku*/
double df__Trace_Axis = 0;      /*przesuniecie osi w sladzie*/
double df__Sketch_Dist_Max  = 1.0 ; /*max. dlugosc odcinka szkicu*/
long_long l__Off_Bl_Beg = -1, l__Off_Bl_End = -1 ; /*odpowiedniki ADP, ADK*/
char *PTR__GTMP1 = NULL, *PTR__GTMP2 = NULL ;
char *PTR__GTMP3 = NULL, *PTR__GTMP4 = NULL ;
char *PTR__GTMP5 = NULL, *PTR__GTMP6 = NULL ;
char *PTR__GTMP7 = NULL;
char* PTR__GTMPBLOCK = NULL;
char* PTR__GTMPFIRST = NULL;
char* PTR__GTMPLAST = NULL;
BOOL (*INITIAL_SEL_TEST)(void*) = NULL ;
BOOL (*INTERSECT_ADD_TEST)(void*, void*) = NULL ;

/*dane globalne - interface */
int xp=0,yp=20,xk=0,yk=0,Ky=-1; /*  os Y do gory Ky=-1 */
double skala=1.0,sk_x=1.0,sk_x_drv_master=1.0,sk_x_drv_slave=1.0;
double WD = TEXT_WIDTH_FACTOR, Pi=3.14159265358979;
double krok_s=0,krok_g=0;
int grid_=0,X_Y_kursora=1,Komunikat_R=0,Komunikat_R0=0, Komunikat_R00 = 0,
Error=0,orto=0,Driver=0,Mode=0,Dzwonek=1;
double Xp=0,Yp=0,Xk=0,Yk=0,Xmax=10000,Ymax=10000,Xmin=0,Ymin=0,
Y_max=10000,X_max=10000;
double Xd=0,Yd=0,X2d=0,Y2d=0,Xd_=0,Yd_=0;
double X=0,Y=0;
int maxX=0,maxY=0,maxXr=0,maxYr=0;
int paper_kom=0,ink_kom=0;
int wyj=0, BlokK=0;
char *dane=NULL;

long dane_size=0;
long dane_size0=0;
int ( *SERV[SVMAX])(void)={ NULL };

void ( *LASTFUN)(void)=NULL;  //int
BOOL Semaphore = FALSE;
BOOL Cust_Semaphore = FALSE;
void (*MVCUR)(int x,int y)=NULL;
void (*CUR_OFF)(double, double)=NULL;
void (*CUR_ON)(double, double)=NULL;
int (*KLS)(void)=NULL;
void (*REDDOD)(void)=NULL;
void (*REDRAW_ADD_MENU)(void)=NULL;
void (*MPINI)(int n)=NULL;
unsigned long buf_mem_menu=68000L;
int Current_Layer=0, No_Layers = 1, No_Layers_DXF = 0;
char *bufor_makra_esc[] = { '\0', '\0', '\0' };
char *bufor_makra=NULL, *aktmakro=NULL, *mkbuf=NULL, *mbufmx = NULL;
LAYER Layers[MAX_NUMBER_OF_LAYERS];
LAYER Layers_DXF[MAX_NUMBER_OF_LAYERS_DXF];
int FormatX=0,FormatY=0 ;
double FormatX_S = 0.0, FormatY_S = 0.0, FormatX_S0 = 0.0, FormatY_S0 = 0.0;;
double SkalaF=0;
int KursorS = 0;
int AXIS=0;
int AXIS_DIV=0;
int OBRYS=0;
int PLYTA=0;
int SIEC_P=0;
int OTWOR_P=0;
double L50=50;
WARSTWY_STROPU Warstwy_Stropu={59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80};
WARSTWY_PREFABRYKATU Warstwy_Prefabrykatu={0,1,2,3,4};
double DokladnoscF=0, Jednostki=0;
char st_jedn[10]="";

char zbior_danych [MAXPATH]="";
char zbior_danych_2 [MAXPATH]="";
char add_zbior_2;
ZMIENNE zmwym=Defzmwym;
ZMIENNE_VECTOR zvector=Defzvector;  // TO DO
int zmwym_fraction=64;
int zmwym_reserve=0;
int BlokM=0;
char *ADP=NULL,  *ADK=NULL;
char *ADPQ=NULL,  *ADKQ=NULL;
BLOK  *LASTb=NULL;
SEL sel=Defsel;

double PATTERN_SCALE = 1.0;
double PATTERN_SCALE_PRN = 1.0;
double FIX_PATTERN_SCALE_PRN = 0.6;

int CUSTOM_KOM = 166;

#if defined (INTER)
#include "rysujf.h"
COMMON *c=&xp;
FUNKCJE *F;
#endif
