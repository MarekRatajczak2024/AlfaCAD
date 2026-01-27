#include "alfini.h"

////////////////   vesavbe.c
/*DEFINICJE*/
#define NET_VERSION

#define POSTSCRIPT  //opcja definiowania dlugosci stropy przy wydruku

#define ALLEGWIN

#define big_pcx

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LINUX
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif
///////////////////////////////

extern BOOL SCREEN_BY_SOLID_OVERRIDIND;
//extern DRIVER_STRUCT drv_master;
//extern DRIVER_STRUCT drv_slave;
extern int last_active_layer;
extern int last_active_layer_arch;
extern int error_layer;
extern BOOL force_redraw;
extern int poll_kbd;
extern int last_mouse_z;
extern BOOL exit_alf;
extern int GFX_WIN;
extern int X_WIN_ORIG, Y_WIN_ORIG, WIN_WIDTH, WIN_HEIGHT;
extern char GFX_ID[5];
extern BOOL just_file;
extern BOOL	use_getopenfilename;
extern BOOL	window_was_resized;
extern BOOL sleep_state;
extern BOOL mouse_out;
extern char	HATCH_TEMP_FILE[13];
extern char RYSUJ$1[13];
extern int Number_of_clients;
extern int Client_number;
extern int Alf_window_number;
extern BOOL	original_color_print;
extern BOOL normalize_text;
extern char background_pcx_file[MAXPATH];
extern WARSTWY_ON_PCX  Warstwy_On_PCX[256], Warstwy_On_Solids[256];
extern int aspect_ratio;
extern int start_disk;
extern int gfx_gd;
extern int gfx_x;
extern int gfx_y;
extern int c_depth;
extern BOOL SHEET_IS_PRINTED;
extern BOOL ENABLE_FOUND;
extern BOOL NULL_FOUND;
extern BOOL PRINT_POLYGONE;  //wydruk obcinany do polygonow
extern double Underl_del;  //przesuniecie linii podkreslenia od tekstu
extern BOOL PCX_gray;
extern BOOL PCX_printing;
extern int licznik_redraw;
extern BOOL redraw_ctx;
extern BOOL regen_ctx;
extern double SkalaF_ctx;
extern double SkalaFA_ctx;
extern unsigned long_long Wielkosc_Plata;

extern char * imageBMPstart;
extern BOOL Break_Edit_Text;
extern char Czcionka_Pulpitu[60];
extern char Desktop_Font_File[MAXPATH];
extern unsigned long_long Bufor_Wydruku;

extern int Kolory_Paper;
extern BOOL select_no_edit_layers;
extern BOOL hide_non;
extern unsigned long_long TRIAL_dane_size;
extern BOOL select_element_only;
extern int tab_dokladnosc[16];
extern double odleglosc_prec;
extern double rzedna_i_prec;
extern double rzedna_p_prec;
extern double rzedna_k_prec;
extern double rzedna_ti_prec;
extern double rzedna_tp_prec;
extern double spadek_prec;
extern double zaglebienie_prec;
extern int odleglosc_poz;
extern int rzedna_i_poz;
extern int rzedna_p_poz;
extern int rzedna_k_poz;
extern int rzedna_ti_poz;
extern int rzedna_tp_poz;
extern int spadek_poz;
extern int zaglebienie_poz;
extern BOOL BLOK_SIEC;
extern double null_var[32];

extern double Last_GX, Last_GY;
extern int ggetmaxx;
extern int ggetmaxy;
extern BOOL deleted_pcx;
extern BOOL deleted_on_front;
extern BOOL deleted_TTF;
extern BOOL deleted_pattern;
extern BOOL bitmap_exist;
extern BOOL bitmap_png_exist;
extern BOOL bitmap_vector_exist;
extern BOOL bitmap_pattern_exist;
extern BOOL solid_translucent_exist;
extern BOOL bitmap_on_front_exist;
extern BOOL bitmap_view;

///////////////////////////////

extern unsigned long_long imageBMPsize;
extern unsigned long_long imageBMPsize_used;
extern unsigned long_long imageBMPsize_total_used;
extern unsigned long_long Bufor_PCX;

extern char *bufor_pcx;

extern unsigned long memory_size;

extern LTYPE TypyLinii[MaxLiczbaTypowLinii];
extern int LiczbaWzorcowLinii;
extern LAYER *layers;
extern int No_Layers;
extern int No_Layers_DXF;
extern int dxf_encoding;
extern TEXT TextG;
extern double krok_s,krok_g;

/////////////////////////////////

extern double grubosc_plyty_szalunkowej;
extern double ciezar_zelbetu;
extern double area_df_x;
extern double area_df_y;
extern double counted_area0;
extern double counted_area;
extern BOOL out_pole_ignore;
extern BOOL force_dim_text;
extern char *Dim_Text_Adr;
extern BOOL do_not_dim_DIM;
extern float pan_dxy;
extern int OBRYS_line_typ; //typ linii obrysu stropu

extern BOOL wymiarowanie_ok;
extern int show_tools;
extern BOOL on_tools;
extern A_WINDOW tools_window;
extern BOOL was_dialog;
extern int now_is_dialog;
extern BOOL do_not_draw_cur;
extern BOOL do_not_draw_keys;
extern char *menu_address[10];
extern int MP_SIZE;        /*wielkosc pola menu poziomego*/
extern int ED_INF_HEIGHT;
extern int BAR_G;
extern int HEIGHT;
extern int WIDTH;
extern int SKALA;
extern int FONTNUMBER;
extern BOOL lock_functions;
extern BOOL edit_functions;
extern int font_handle, font_handle1;
extern int font_handle_tab[12], font_handle1_tab[12];
extern BOOL dynamic_menu;
extern BOOL desktop_instruction;
extern BOOL auto_korekta_blokow;

extern SEKTORY sektory_arkusza;
extern SEKTORY_EXT sektory_arkusza_ext;
extern STATIC_COLORS static_colors;
extern STATIC_STRESS_COLORS static_stress_colors;
extern STATIC_COLORS static_colors0;
extern STATIC_STRESS_COLORS static_stress_colors0;

extern short int SolidHatch;
extern short int SolidFillTyp;
extern short int SolidHatchPattern;
extern short int SolidPattern;
extern short int SolidTranslucent;
extern short int TraceTranslucent;
extern short int SolidHatchTranslucent;

extern unsigned char SolidTranslucency;
extern unsigned char  TraceTranslucency;
extern unsigned char  SolidHatchTranslucency;
extern unsigned char FilledEllipseTranslucency;

extern short int TracePattern;
extern short int SolidPatternScale;
extern short int SolidHatchPatternScale;
extern short int TracePatternScale;
extern short int SolidPatternAngle;
extern short int SolidHatchPatternAngle;
extern short int TracePatternAngle;
extern short int SolidPatternDx;
extern short int SolidPatternDy;
extern short int SolidHatchPatternDx;
extern short int SolidHatchPatternDy;
extern short int TracePatternDx;
extern short int TracePatternDy;
extern char SolidPatternName[MAXPATH];
extern char SolidHatchPatternName[MAXPATH];
extern char TracePatternName[MAXPATH];

extern BOOL change_color_wsp;
extern BOOL auto_pan_block;
extern BOOL auto_zoom_block;
extern char global_kom [100];
extern char EXPLORER_EXE [MAXPATH];
extern BOOL auto_tekst;
extern int tekst_interwal;
extern BOOL hold_scrsaver;
extern BOOL print_inversion;
extern BOOL print_rotation;
extern char bloki_dir [MAXPATH];
extern char katalogi_dir[MAXPATH];
extern char log_dir [MAXPATH];

extern double last_X;
extern double last_Y;
extern BOOL first_shoot_biblio;
extern BOOL draw_hatch_pattern;
extern BOOL draw_hatch_image_pattern;
extern char s_hatch_param_df_name [MAXPATH];
extern double s_hatch_param_df_distans;
extern double s_hatch_param_df_scale;
extern double s_hatch_param_df_angle;
extern BOOL blok_xorput;
extern double DF_PRECISION;
extern BOOL b_first_open;

extern int killpline;
extern int killblock;
extern OPTIONS1 options1;
extern BOOL undo_avail;
extern UNDO_TAB_REC UNDO_TAB [4];
extern UNDO_TAB_REC UNDO_REC;
extern UNDO_TAB_REC UNDO_REC_NULL;
extern UNDO_TAB_REC UNDO_REC_255;
extern char Previous_File [MAXHISTORYFILES][MAXPATH];
extern char Current_File_Directory [MAXPATH];
extern char bloki_cfg [MAXPATH];
extern char biblioteka_cfg [MAXPATH];
extern BOOL d_t__pline;
extern BOOL bspline__pline;
extern BOOL ini_cursor;
extern BOOL wez_warstwe_typ_kolor;
extern char argv0 [MAXPATH];
extern int DYNAMIC_MOUSE;
extern int SKOKM_VER; /*2   4   8  *12  16  20   24*/
extern int SKOKM_HOR; /*8  12  18  *24  30  36   42*/
extern int text_mode1;
extern BOOL kasowanie_licznikow;
extern char old_mask_file [MAXFILE + MAXEXT] ;
extern char old_mask_blok [MAXFILE + MAXEXT] ;
extern char old_mask_pcx  [MAXFILE + MAXEXT] ;
extern char old_mask_pro  [MAXFILE + MAXEXT] ;
extern char old_mask_geo  [MAXFILE + MAXEXT] ;
extern BOOL skala_punktu;
extern int pznaki_aster3;
extern int code_page;
extern int dxf_encoding; 


extern int return_back;
extern char aktualny_plik[MAXPATH];
extern char poprzedni_plik[MAXPATH];
extern int aktualny_poz;
extern int aktualny_foff;
extern int poprzedni_poz;
extern int poprzedni_foff;
extern BOOL ciagniecie_linii;
extern BOOL rozcinanie_linii;
extern BOOL ukrywanie_typu;
extern BOOL type_view;
extern int dodatkowe_linie;
extern int dodatkowe_linie_all;
extern LINIA LINE_TAB[max_dodatkowe_linie + 2];
extern int ignoruj_aktywacja;
extern int numer_zbioru_hierarchii;
extern char nazwa_zbioru_hierarchii[256][MAXPATH];
extern float PIN_TAB_X[MaxPinCount+2];
extern float PIN_TAB_Y[MaxPinCount+2];
extern int PIN_COUNT;
extern BOOL ADD_NEW_PIN;
extern char find_text[MaxTextLen];
extern char change_text_source[MaxTextLen];
extern char change_text_dest[MaxTextLen];
extern char *last_find_addr;
extern char *last_change_addr;
extern BOOL komunikaty_bloku;
extern BOOL swobodny_blok;
extern BOOL swobodny_tekst;
extern int poziom_menu;
extern char plt_file0[10];
extern unsigned short dos_version;
extern long DelDaneSize;
extern double text_line_width ;  /* grubosc linii dla teskstu */
extern int procesp;
extern ESTR eO,eL,el,elc,eTens,eA,eEA,eVf,eVf1,eVd,eVd1,eVm,eVr,eVe, eVn;
extern int selq;
extern TMENU mTypyLinii;
extern KOLORY kolory;
extern KOLORY_ASTER kolorya;
extern KOLORY_PROFILI koloryp;
extern KOLORY_STROPU kolorys;
extern KOLORY_SIECI koloryps;
extern volatile MVCURB mvcurb;
extern int snap_;
extern int LiczbaTypowLinii;

extern TEXT3D TextG3D;
extern LINIA LiniaG;
extern AVECTOR VectorG;
extern LINIA L_axis;
extern LINIA3D LiniaG3D;
extern OKRAG OkragG;
extern OKRAG3D OkragG3D;
extern LUK LukG;
extern LUK3D LukG3D;
extern T_Point PointG ;
extern T_Point3D PointG3D;
extern WIELOKAT SolidG;
extern int poczL;  /* linia, osnap, prostopadle */
extern int Error;
extern int (*TABFUN)(void);
extern int (*TABFUN1)(char);
extern int(*MEDITFUN)(void);
extern int Info;
extern BOOL  Change, Auto_Pan, bRedrow;
extern BOOL bMouse ;
extern int Orto_Dir;
extern double df__Point_Size ; 	   /*rozmiar punktu w jednostkach rysunku*/
extern BOOL b__Point_View ; /*widocznosc punktu*/
extern BOOL b__Control_Points_View ;
extern BOOL b__Bitmap_Outline;
extern double df__Trace_Width ;    /*szerokosc sladu w jednostkach rysunku*/
extern double df__Trace_Axis ; /*przesuniecie osi w sladzie*/
extern double df__Sketch_Dist_Max ; /*max. dlugosc odcinka szkicu*/
extern long_long l__Off_Bl_Beg, l__Off_Bl_End ; /*odpowiedniki ADP, ADK*/
extern char *PTR__GTMP1, *PTR__GTMP2, *PTR__GTMP3 ;
extern char *PTR__GTMP4, *PTR__GTMP5, *PTR__GTMP6 ;
extern char *PTR__GTMP7;
extern char* PTR__GTMPBLOCK;
extern char* PTR__GTMPFIRST;
extern char* PTR__GTMPLAST;
extern BOOL (*INITIAL_SEL_TEST)(void*) ;
extern BOOL (*INTERSECT_ADD_TEST)(void*, void*) ;

/*dane globalne - interface */
extern int xp,yp,xk,yk,Ky; /*  os Y do gory Ky=-1 */
extern double skala,sk_x,sk_x_drv_master,sk_x_drv_slave;
extern double WD,Pi;

extern int grid_,X_Y_kursora,Komunikat_R,Komunikat_R0, Komunikat_R00, Error,orto,Driver,Mode,Dzwonek ;
extern double Xp,Yp,Xk,Yk,Xmax,Ymax,Xmin,Ymin,X_max,Y_max;
extern double Xd,Yd,X2d,Y2d,Xd_,Yd_;
extern double X,Y;
/* extern double localx, localy; */
extern int maxX,maxY,maxXr,maxYr;
extern int paper_kom,ink_kom;
extern int wyj,BlokK;
extern int Current_Layer;

extern long dane_size;
extern long dane_size0;
extern char  *dane;


//extern char (*SERV[SVMAX])(void);
extern int ( *SERV[SVMAX])(void);
extern void ( *LASTFUN)(void);  //int
extern void (*MVCUR)(int x,int y);
extern void (*CUR_OFF)(double, double);
extern void (*CUR_ON)(double, double);
extern int (*KLS)(void);
extern void (*REDDOD)(void);
extern void (*REDRAW_ADD_MENU)(void);
extern void (*MPINI)(int n);
extern TMENU menug,menup,mTekstm,mZmienne,mParametrym;
extern POLE pmTak_Nie[],pmKolor[]; //,pmKolorL[],pmKolorT[];
extern unsigned long buf_mem_menu;
extern char *bufor_makra, *aktmakro, *mkbuf, *mbufmx;
extern char *bufor_makra_esc[];
extern LAYER Layers[MAX_NUMBER_OF_LAYERS];
extern LAYER Layers_DXF[MAX_NUMBER_OF_LAYERS_DXF];
extern int FormatX, FormatY;
extern double FormatX_S, FormatY_S, FormatX_S0, FormatY_S0;
extern double SkalaF;
extern int KursorS;
extern int AXIS; //os dla aplikacji STROPY
extern int AXIS_DIV; //linia podzialu plyt w aplikacji STROPY
extern int OBRYS; //obrys budynku;
extern int PLATE; //slab plate
extern int HOLE; //hole in plate
extern int WALL;
extern int ZONE;
extern int SIEC_P; //siec na mapie
extern double L50; //dlugosc osi pomocniczej podzialu plyty
extern WARSTWY_STROPU Warstwy_Stropu;
extern WARSTWY_PREFABRYKATU Warstwy_Prefabrykatu;
extern double DokladnoscF, Jednostki;
extern char st_jedn[10];
extern char zbior_danych[];
extern char zbior_danych_2[];
extern char add_zbior_2;
extern ZMIENNE zmwym;
extern int zmwym_fraction;
extern int zmwym_reserve;
extern int BlokM;
extern char *ADP, *ADK, *ADPQ, *ADKQ;
extern BLOK *LASTb;
extern SEL sel;
extern COMMON *c;
extern FUNKCJE *F;

extern void find_font_face(char *Font_File);
extern BOOL Get_Control_Points_View(void);
extern void Set_Control_Points_View(BOOL b_pv);
extern void Restore_View_Preview(void);
extern void out_cur_off(double X, double Y);
extern void out_cur_on(double X, double Y);
extern void Quick_Exit(int errorno);

extern double PATTERN_SCALE;
extern double PATTERN_SCALE_PRN;
extern double FIX_PATTERN_SCALE_PRN;

/////////////////////////////////

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* static LINIA line_gee = Ldef, line_gee1 = Ldef ; */
/*16-33*/
extern void deinstal_ms(void);
extern void instal_ms(void);

/*rysuj*/
extern void noopg(void);

/*funkcje okienkowe*/
extern void openw(TMENU *menu); /*otwarcie okna bez mozliwosci manipulacji*/
extern void closew(TMENU *menu);	/*zamkniecie okna,jesli nie nastepuje autom.*/
extern int getwsp(TMENU *menu);
extern void restorow(void); 	/*odtworzenie stanu okien*/
extern void  *get_mem_menu(void);
extern void  *getmem(unsigned l);
extern void free_mem(void);
extern int  setmouse(void);
extern void  instalint(void);
extern int inkeys(TMENU *menu, BOOL search_ok);
extern void drawp(TMENU *menu);
extern int  getwsp1(TMENU *menu);
extern int  getwsp2(TMENU *menu, char *ad, int param_no[]);
extern int getwsp3(TMENU *menu);
extern EVENT *GetEvent(TMENU *menu);
extern void mskp(void);
extern void Set_Global_Menu_Flags (int flags);
extern int Get_Global_Menu_Flags (void);

/*bib_e*/
/*nowe*/

extern void czytaj_dane_profilu(MyDane *dane_prof);
extern void zapisz_dane_profilu(MyDane *dane_prof);
/*  */
extern void ini_e(void);
extern void pozycja_kursora(void);
extern void out_cur_(double X,double Y, int on);
extern void ClearInfo(void);
extern void InfoList(int n);
extern void ClearErr(void);
extern void ErrList(int n);
extern void ErrListStr(char *);
extern void  Internal_Error (int line, char * file);
extern void komunikat(int);
extern void komunikat_len(int);
extern void komunikat_str(char  *st);
extern void komunikat_str_short_reset(void);
extern void komunikat_str_short_mask(BOOL center);
extern void komunikat_str_short(char *st, BOOL stay, BOOL center);
extern void remove_short_notice(void);
extern void komunikat_str_len(char  *st);
extern void komunikat0(int);
extern void komunikat0_str (int n, char  *st);
extern long pikseleX (double jednostki) ;
extern long pikseleX0 (double jednostki);
extern long pikseleX00 (double jednostki);
extern long pikseleY00 (double jednostki);
extern long pikseleDX (double jednostki);
extern long pikseleY (double jednostki);
extern long pikseleY0 (double jednostki);
extern long pikseleDY (double jednostki);
extern double pikseleFDY(double jednostki);
extern double jednostkiX(long piksele);
extern double jednostkiY(long piksele);
extern double jednostkiplt(double mmplt) ;
extern double milimetryplt(double jednostki) ;
extern double jednostkiOb(double mmob) ;
extern float jednostkiObXm(double mob);
extern float jednostkiObYm(double mob);
extern float jednostkiObXi(double mob);
extern float jednostkiObYi(double mob);
extern double get_skala_profilu_x(void);
extern double get_poziom_pp(void);
extern double get_typ_profilu(void);
extern double get_hektometr_p(void);
extern double get_hektometr_k(void);
extern int get_znacznik_aplikacji(void);
extern int put_znacznik_aplikacji(int);
extern int put_skala_profilu_x(double skala_px);
extern int put_poziom_pp(double rpp0);
extern int put_typ_profilu(double typ_profilu);
extern int put_hektometr(double hektometr, int opcja);
extern int put_dynamic_block(double dynamic_block_a);
extern double get_dynamic_block(void);
extern int put_localx(double local_x);
extern int put_localy(double local_y);
extern int put_angle_l(double a_l);
extern int put_angle_lr(double a_l0, double a_l);
extern double get_localx(void);
extern double get_localy(void);
extern double get_angle_l(void);
extern void set_angle_l(double angle);
extern double get_tana(void);
extern double get_tana1(void);
extern double get_sina(void);
extern double get_cosa(void);
extern double milimetryob(double jednostki) ;
extern double milimetryobx(double jednostki) ;
extern double milimetryoby(double jednostki) ;
extern double milimetryobxl(double x0, double y0) ;
extern double milimetryobyl(double x0, double y0) ;
extern double inch_to_mm (double) ;
extern double mm_to_inch (double) ;
extern double jednostki_d_to_plt_d (double jednostki_d) ;
extern double plt_d_to_jednostki_d (double mmplt) ;
extern void okno_r(void);
extern void okno_all(void);
extern void grid_off(void);
extern void grid_on(void);
extern void cursorT_off(int x,int y);
extern void cursorT_on(int x,int y);
extern int read_esc0(char  *tekst,int lmax);
extern int read_esc(char  *,int,int);
extern void Cur_offd(double x,double y);
extern void Cur_ond(double x,double y);
extern void mvcurp(double dx, double dy);
extern int linestyle(int typ);
extern void  Clear_Client_Screen (void);
extern void redraw(void);
extern void d_2d(void);
extern void d_2d_p(void);
extern void Xp_Yp(void);
extern void Xp_Yp_p(double xmin, double ymin);
extern void menu_par_new(char * pole, const char * par);
extern int ladujexe(char *nz,long size);
extern void Wypelnienie(int hatch_type);
extern void dzwonek(void);
extern void zwolnienie_pamieci(void);
extern int korekta_parblok(void  *st,long n);
extern char *dodaj_obiekt(BLOK  *adb,void  *ad);
extern char *dodaj_obiekt_reversed(BLOK  *adb,void  *ad);
extern char *dodaj_obiektFD(BLOK *adb, int handle);
extern char *dodaj_obiektFD1_1(BLOK *adb, int handle);
extern void obiekt_tok1(char *adr,char *adrk,char **ad,int obiekt);
extern void obiekt_tok(char *adr,char *adrk,char **ad,int obiekt);
extern void obiekt_tok_all(char  *adr, char  *adrk, char  **ad, int obiekt);
extern void obiekt_tok_all2 (char  *adr, char  *adrk, char  **ad, int obiekt1, int obiekt2);
extern void ObiektTok (long_long *off, long_long offk, long_long *ad, int obiekt);
extern void User(void);
extern void Xk_Yk(void);
extern int czytaj_rysunek(char *rys, BOOL b_err);
extern int GetColorAC(int color);
extern int GetColorWin(unsigned char color);
extern int GetColorHP0(int color, int color_mode, int bank_kolorow, int horizontal_bit);
extern int GetColorHP(int color, int color_mode, int bank_kolorow, int horizontal_bit, int liczba_pol);
extern int color256[];
extern void SetColorAC(int color);
extern int SetBufferDaneSize(long l);
extern void InitDane(void);

/*bib_e1*/
void movmem_(void *src,void * dest,long nbyte);
extern void usun_obiekt(void  *);
extern void  *korekta_obiekt(void  *ad,void  *obiekt);
extern void zmien_atrybut(char *,char *,int ,int );
extern void zmien_atrybut_for_objects(char *,char *,int ,int, int );
extern void zmien_atrybut_undo(char *,char * );
extern void usun_blok(char *,char *);
extern void out_kolo(OKRAG *);
extern void out_wypelnienie(WIELOKAT *);
extern void outokno(OKNO *O,double dx,double dy);
extern void outline(LINIA  *L,int mode,int pl);
extern void outlineor(LINIA  *L,int mode,int pl);
extern void parametry_lini(LINIA *L,PLINIA *PL);
extern void parametry_linior(LINIA *L,PLINIA *PL);
extern void clear_parametry_lini(void);
extern void outokrag(OKRAG *O,int mode,int po);
extern void outluk(LUK *l,int mode,int po);
extern void out_parametry_luku(LUK *l);
extern void out_krz(double X, double Y) ;  /*interf*/
extern void out_krz_sc(double X, double Y) ;  /*interf*/
extern void out_krz_sc_p(int x0, int y0) ;  /*interf*/
extern int lineC(long x1, long y1, long x2, long y2);

/*bib_par*/
extern void Parametry(void);

/*bib_wyj*/
extern void Wyjscie(void);
extern void Close_window(void);
extern void Koniec(void);
extern void Opcje(void);


/*bib_text*/
extern void  Tekst(void);
extern void  Multitekst(void);
//extern void  Atrybut(void);
extern void outtextxy_r(int x,int y,char  *textstring);
extern void outtext_r(char  *textstring);
extern void outtextxy_w(TEXT  *t,int mode);
extern void outtextxy_wP(TEXT  *t,int mode);

/*bib_line*/
extern void Linia(void);
extern void Vector(void);
extern void Linia3D(void);

/*bib_zb*/
extern int czytaj_zbior(char *, BOOL, BOOL, BOOL);
extern int pisz_zbior(char *, BOOL, int);
extern int WriteBlock(char *fn,double Px,double Py, char *buf_name, char *buf_type, int every, int bak);
extern int ReadBlock(char *fn,double *Px,double *Py,RYSPOZ *adp,RYSPOZ *adk);

/*bib_sel */
extern void oknoS(double X1,double Y1,double X2,double Y2);
extern void sel_on_off(void);
extern void cursel_off(double x,double y);
extern void cursel_on(double x,double y);
extern int punkt_w_prostokacie(double x,double y);
extern int linia_w_prostokacie(LINIA *ad);
extern int tekst_w_prostokacie(TEXT *ad);
extern int luk_w_prostokacie(LUK *ad);
extern int solidarc_w_prostokacie(SOLIDARC *ad);
extern int okrag_w_prostokacie(OKRAG *ad);
extern int wielokat_w_prostokacie(WIELOKAT *ad);
extern int spline_w_prostokacie(SPLINE *ad);
extern int linia_wybrana(LINIA *ad);
extern int vector_wybrany(AVECTOR *ad);
extern int spline_wybrany(SPLINE *ad);
extern int pcx_wybrany(B_PCX* ad, BOOL printing);
extern int pcx_w_prostokacie(B_PCX *ad);
extern int tekst_wybrany(TEXT *ad);
extern int tekst_wybrany_prn(TEXT *ad);
extern int luk_wybrany(LUK *ad);
extern int luk_wybrany_prn(LUK *ad);
extern int prostokat_luk(LUK *ad);
extern int prostokat_solidarc(SOLIDARC *ad);
extern int prostokat_ellipticalarc(ELLIPTICALARC *ad);
extern int okrag_wybrany(OKRAG *ad);
extern int elipsa_wybrana(ELLIPSE *ad);
extern int elipsa_wybrana_prec(ELLIPSE *ad);
extern int elipsa_w_prostokacie(ELLIPSE *ad);
extern int lukeliptyczny_wybrany (ELLIPTICALARC *ad);
extern int lukeliptyczny_wybrany_prec(ELLIPTICALARC *ad);
extern int lukeliptyczny_w_prostokacie(ELLIPTICALARC *ad);
extern int wielokat_wybrany(WIELOKAT*ad);
extern int solidarc_wybrany(SOLIDARC *sa);
extern int qkoniecS(double *x, double *y);
extern int selectS(double *x, double *y, int nr_kom_);
extern void *select_w(unsigned *typ, void **adr);
extern void *select_n(unsigned *typ, void **adr, int nr_kom_);
extern void *select_edwym(unsigned *typ);
extern void out_sel(double X,double Y);
extern void out_sel_off(double X, double Y);
extern void out_sel_on(double X, double Y);
extern int  sel_t(void);
extern int  sel_d(void);


/*bib_wym*/
extern void  Wymiarowanie(void);
extern int WymNowy(void);
extern int WymPunkt(double X, double Y, int tryb);
extern void WymInterF(int k);

/*bib_mak*/
extern int delay_1(void);
extern int delay_2(void);
extern int delay_3(void);
extern void Makro(void);
extern void brkmakro(void);
extern unsigned Get_Buf_Mak_Size (void);
extern void test_aktmakro(void);

/*bib_blok*/
void transformacja_blok(char *adr,char *adrk,double x,double y,double k1,double k2,int trans,double z);
extern void rysuj_obiekt_(char *ad, int mode,int kolor);
extern void rysuj_obiekt(char *ad, int mode,int kolor);
extern void mvcurbp(unsigned i);
extern void blokzap(char *adp,char *adk,int atrybut,int mode, int kolor);
extern void blokzap_v(char *adp,char *adk,int atrybut,int mode, int kolor);
extern void out_blok(void);
extern void out_blok1(double x,double y,double k1,double k2,int trans,double z);
extern int dodaj_blok(char *adr,char *adrk,double x,double y,double k1,double k2,int trans);
extern void undo_blok(void);
extern void Global_Undo(void);
extern void blok(int (*DZI)(void *), int (*ODZI)(void *), void (*REDROWd)(void),const void (*COMND[])(void));
extern void blok_items(int items, int (*DZI)(void *), int (*ODZI)(void *), void (*REDROWd)(void),const void (*COMND[])(void));
BOOL check_if_hidden (char *adp,char *adk,int atrybut);

/*bib_cia*/
extern void ciagnij(void);
extern void ciagnij_quad(void);

/*bib_arc*/
extern void mvcurbplL(unsigned i);
extern void DrawCircle(long Xr,long Yr,long R, int mode) ;
extern void DrawEllipse(ELLIPSE *e, int mode, int kolor, int kolor0) ;
extern void DrawFilledEllipse(ELLIPSE *e, int mode, int kolor, int kolor0) ;
extern void DrawEllipticalArc(ELLIPTICALARC *ea, int mode, int kolor, int kolor0) ;
extern void DrawArc(long Xr,long Yr,double sta,double ea,long R, int mode);

/*bib_menu*/
extern int menupoz(void);
extern int menupoz_32(void);
extern void menupini(TMENU *menu,char *st,unsigned short cod, int iconno);
extern void getmenupini(TMENU **menu,char *st,unsigned short *cod, int *iconno);
extern void Ini_Place_Marker (void);

/*bib_snap*/
extern void obru(double si,double co, double xp, double yp, double *xn,double *yn);
extern void obrd(double si,double co, double xp, double yp, double *xn, double *yn);
extern void koniecL_(double *x, double *y, void *adr);
extern void koniecEA_XY_(double *x,double *y,int numpoints, double xy[]);
extern int przeciecieLL_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieLO_(double *x, double *y, void *adr, void *adr1);
extern int przeciecieOO_(double *x, double *y, void *adr, void *adr1);
extern int prostopadleL_(double *x,double *y,void *adr);
extern int przeciecieLl_(double *x, double *y, void *adr, void *adr1);
extern int przecieciell_(double *x, double *y, void *adr, void *adr1);
extern int przeciecielO_(double *x, double *y, void *adr, void *adr1);
extern void najblizszyO_(double *x,double *y,void *adr);
extern void obru(double si,double co, double xp, double yp, double *xn,double *yn);
extern void obrd(double si,double co, double xp, double yp, double *xn, double *yn);
extern void koniecS(void);
extern void srodek(void);
extern void najblizszy(void);
extern void koniec(void);
extern void przeciecie(void);
extern void centrum(void);
extern void prostopadle(void);
extern void stycznie(void);
extern EVENT *Get_Event_Point(TMENU *, double *, double *);

/*bib_okrg*/
extern OKRAG *okragm(void);
extern void out_parametry_okregu(OKRAG *O);
extern void Okrag(void);

/*inter*/
extern int  interfacec(int k);

/*bib_edit*/
extern int dodajstr(ESTR *e);
extern void usunstr(int n);
extern int edycjastr(void);

/*bib_prof*/
extern void profilowanie(void);

/*bib_luk*/
extern void Luk(void);

/*b_blok*/
extern void Edit_Obiekt(void);
extern void Kopiuj(void);
extern void Przesun(void);
extern void PrzesunZ(void);
extern void Usun(void);
extern void Obrot(void);
extern void Obrot1(void);
extern void ObrotXZ(void);
extern void ObrotYZ(void);
extern void Skala(void);
extern void Export(void);
extern void Import(void);

/*b_ltini*/
extern int ReadTypyLinii(void);
extern int ReadTextStyle(void);

/*b_buf*/
extern void InitBuffers(void);
extern char *ReInitBuffers(char  **P, long size);
extern void DoneBuffers(int dwg_no);
extern void DoneBuffMacro(void);
extern char *NewBuffer(char **P);
extern void DisposeBuffer(char  *P);
extern long GetBufferSize(char  * P);
extern long_long GetFreeMemSize(void);
extern int SetBufferSize(char *P, long Size);

/*b_text*/
//extern int editstring(unsigned char *, char *, int, float, BOOL, int, BOOL, int y_3_) ;
extern int editstring(unsigned char *s, char *legal, int maxlength, float width0, BOOL b_graph_value, int expand, BOOL last_edit, int y_4_, int y_3_);
extern int get_string (char *, char *, int , int , int );
extern int get_string_str (char *, char *, int , int , char *);
extern int getkey(void);
extern int Get_Legal_Key (char *legal);

/*o_plrprn*/
extern void Set_Type_Drive (int , int, double , double , double) ;
extern void Set_Draw_Window (double x1, double y1, double x2, double y2);
extern BOOL Draw_Arc_To_Drive (LUK *l);
extern BOOL Draw_Line_To_Drive ( LINIA *L0);
extern BOOL Draw_Spline_To_Drive(SPLINE *S0);
extern BOOL Draw_Tekst_To_Drive(TEXT *t, int ink_plotter, int pen , int plt_type/*, double pen_width_ink*/);

/*o_plt*/
extern BOOL Draw_Line_To_Plt ( double x1, double y1, double x2, double y2);

/*o_prn*/
extern BOOL Draw_Line_To_Prn ( double x1, double y1, double x2, double y2, int kolor, double text_line_width, BOOL bw, BOOL grey);
extern BOOL Draw_Line_To_Prn1 ( double x1, double y1, double x2, double y2, int kolor, double text_line_width, BOOL bw, BOOL grey);

extern void DrawToPrn (void);

/*o_inicnf*/

extern BOOL Get_Private_Profile_Strings	(T_Fstring , BOOL (*)(T_Fstring, T_Fstring));

/*layers*/
extern void Set_Layers (void);

/*ctrl-R*/
extern int Kopiuj_Ctrl_R(void);

/*b_help*/
extern int help(TMENU *menu);

/*o_loadf*/

/*b_clip*/
extern void Draw_Solid(int numpoints, T_PixelTVal* polypoints, unsigned int pcx_solid, BOOL hatch_solid, double origin_x, double origin_y, char* s_pattern, int translucency, GRADIENT4 *gradient);

/*o_calculator*/
extern int calculator (char *buf, int *retval_no, double *buf_ret);

extern void my_sleep(int sleepMs);
extern void flip_screen(void);
extern void Set_Second_Screen(void);
extern void Set_Screen(void);
extern void okno_r_second(void);
extern void okno_all_second(void);

//extern void Sleep(long miliseconds);
/*o_measure*/
extern int Pan_Shift();
extern int Skala_In_();
extern int Skala_Out_();
extern int trim_line_to_quad(LINIA *ptrs_line, QUAD *quad, LINIA *line_tmp, LINIA *line_tmp1);
extern int trim_arc_to_quad(LUK *ptrs_arc, QUAD *quad, LUK *arc_tmp, LUK *arc_tmp1);
extern int trim_ellipticalarc_to_quad(ELLIPTICALARC *ptrs_ea, QUAD *quad, ELLIPTICALARC *ea_tmp, ELLIPTICALARC *ea_tmp1);
extern void Normalize_Quad (QUAD *quad);
extern void calc_bspline(int lp, int n, float points[], int p1, float* out_x, float* out_y);

extern int Save_Update_flex(int save_upd, int *curr_h, int *curr_v);
extern int get_empty_buffer(void);
extern void New_window_factory(int NEW_DRAWING_NUMBER, char *sk);

extern int alfaplugins(void);
extern int CUSTOM_KOM;

extern double thermal_axis_size;
extern double node_size;

    extern void my_scare_mouse(void);
    extern void my_unscare_mouse(void);

#ifdef ALLEGRO5
extern void set_semaphore(BOOL sem);
extern void set_timer_speed(int speed);
#endif
extern void set_forget_mouse(int exp_x, int exp_y);
extern BOOL RETINA;
/*
extern void position_mouse_xy_r(int x, int y);
extern void position_mouse_r(int x, int y);
extern void position_mouse_r_(int x, int y);
*/

#ifdef __cplusplus
}
#endif
